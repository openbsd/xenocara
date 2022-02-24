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

#include "GL/gl.h"

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
   DISPATCH_CMD_ArrayElement,
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
   DISPATCH_CMD_CopyColorTable,
   DISPATCH_CMD_CopyColorSubTable,
   DISPATCH_CMD_ConvolutionParameterf,
   DISPATCH_CMD_ConvolutionParameteri,
   DISPATCH_CMD_CopyConvolutionFilter1D,
   DISPATCH_CMD_CopyConvolutionFilter2D,
   DISPATCH_CMD_Histogram,
   DISPATCH_CMD_Minmax,
   DISPATCH_CMD_ResetHistogram,
   DISPATCH_CMD_ResetMinmax,
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
   DISPATCH_CMD_MultiDrawElementsEXT,
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
   DISPATCH_CMD_ShaderSource,
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
   DISPATCH_CMD_DrawArraysInstancedARB,
   DISPATCH_CMD_DrawElementsInstancedARB,
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
   DISPATCH_CMD_SignalSemaphoreEXT,
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
uint32_t _mesa_unmarshal_NewList(struct gl_context *ctx, const struct marshal_cmd_NewList *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_NewList(GLuint list, GLenum mode);
struct marshal_cmd_EndList;
uint32_t _mesa_unmarshal_EndList(struct gl_context *ctx, const struct marshal_cmd_EndList *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_EndList(void);
struct marshal_cmd_CallList;
uint32_t _mesa_unmarshal_CallList(struct gl_context *ctx, const struct marshal_cmd_CallList *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_CallList(GLuint list);
struct marshal_cmd_CallLists;
uint32_t _mesa_unmarshal_CallLists(struct gl_context *ctx, const struct marshal_cmd_CallLists *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_CallLists(GLsizei n, GLenum type, const GLvoid * lists);
struct marshal_cmd_DeleteLists;
uint32_t _mesa_unmarshal_DeleteLists(struct gl_context *ctx, const struct marshal_cmd_DeleteLists *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_DeleteLists(GLuint list, GLsizei range);
GLuint GLAPIENTRY _mesa_marshal_GenLists(GLsizei range);
struct marshal_cmd_ListBase;
uint32_t _mesa_unmarshal_ListBase(struct gl_context *ctx, const struct marshal_cmd_ListBase *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ListBase(GLuint base);
struct marshal_cmd_Begin;
uint32_t _mesa_unmarshal_Begin(struct gl_context *ctx, const struct marshal_cmd_Begin *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Begin(GLenum mode);
struct marshal_cmd_Bitmap;
uint32_t _mesa_unmarshal_Bitmap(struct gl_context *ctx, const struct marshal_cmd_Bitmap *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Bitmap(GLsizei width, GLsizei height, GLfloat xorig, GLfloat yorig, GLfloat xmove, GLfloat ymove, const GLubyte * bitmap);
struct marshal_cmd_Color3b;
uint32_t _mesa_unmarshal_Color3b(struct gl_context *ctx, const struct marshal_cmd_Color3b *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Color3b(GLbyte red, GLbyte green, GLbyte blue);
struct marshal_cmd_Color3bv;
uint32_t _mesa_unmarshal_Color3bv(struct gl_context *ctx, const struct marshal_cmd_Color3bv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Color3bv(const GLbyte * v);
struct marshal_cmd_Color3d;
uint32_t _mesa_unmarshal_Color3d(struct gl_context *ctx, const struct marshal_cmd_Color3d *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Color3d(GLdouble red, GLdouble green, GLdouble blue);
struct marshal_cmd_Color3dv;
uint32_t _mesa_unmarshal_Color3dv(struct gl_context *ctx, const struct marshal_cmd_Color3dv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Color3dv(const GLdouble * v);
struct marshal_cmd_Color3f;
uint32_t _mesa_unmarshal_Color3f(struct gl_context *ctx, const struct marshal_cmd_Color3f *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Color3f(GLfloat red, GLfloat green, GLfloat blue);
struct marshal_cmd_Color3fv;
uint32_t _mesa_unmarshal_Color3fv(struct gl_context *ctx, const struct marshal_cmd_Color3fv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Color3fv(const GLfloat * v);
struct marshal_cmd_Color3i;
uint32_t _mesa_unmarshal_Color3i(struct gl_context *ctx, const struct marshal_cmd_Color3i *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Color3i(GLint red, GLint green, GLint blue);
struct marshal_cmd_Color3iv;
uint32_t _mesa_unmarshal_Color3iv(struct gl_context *ctx, const struct marshal_cmd_Color3iv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Color3iv(const GLint * v);
struct marshal_cmd_Color3s;
uint32_t _mesa_unmarshal_Color3s(struct gl_context *ctx, const struct marshal_cmd_Color3s *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Color3s(GLshort red, GLshort green, GLshort blue);
struct marshal_cmd_Color3sv;
uint32_t _mesa_unmarshal_Color3sv(struct gl_context *ctx, const struct marshal_cmd_Color3sv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Color3sv(const GLshort * v);
struct marshal_cmd_Color3ub;
uint32_t _mesa_unmarshal_Color3ub(struct gl_context *ctx, const struct marshal_cmd_Color3ub *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Color3ub(GLubyte red, GLubyte green, GLubyte blue);
struct marshal_cmd_Color3ubv;
uint32_t _mesa_unmarshal_Color3ubv(struct gl_context *ctx, const struct marshal_cmd_Color3ubv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Color3ubv(const GLubyte * v);
struct marshal_cmd_Color3ui;
uint32_t _mesa_unmarshal_Color3ui(struct gl_context *ctx, const struct marshal_cmd_Color3ui *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Color3ui(GLuint red, GLuint green, GLuint blue);
struct marshal_cmd_Color3uiv;
uint32_t _mesa_unmarshal_Color3uiv(struct gl_context *ctx, const struct marshal_cmd_Color3uiv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Color3uiv(const GLuint * v);
struct marshal_cmd_Color3us;
uint32_t _mesa_unmarshal_Color3us(struct gl_context *ctx, const struct marshal_cmd_Color3us *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Color3us(GLushort red, GLushort green, GLushort blue);
struct marshal_cmd_Color3usv;
uint32_t _mesa_unmarshal_Color3usv(struct gl_context *ctx, const struct marshal_cmd_Color3usv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Color3usv(const GLushort * v);
struct marshal_cmd_Color4b;
uint32_t _mesa_unmarshal_Color4b(struct gl_context *ctx, const struct marshal_cmd_Color4b *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Color4b(GLbyte red, GLbyte green, GLbyte blue, GLbyte alpha);
struct marshal_cmd_Color4bv;
uint32_t _mesa_unmarshal_Color4bv(struct gl_context *ctx, const struct marshal_cmd_Color4bv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Color4bv(const GLbyte * v);
struct marshal_cmd_Color4d;
uint32_t _mesa_unmarshal_Color4d(struct gl_context *ctx, const struct marshal_cmd_Color4d *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Color4d(GLdouble red, GLdouble green, GLdouble blue, GLdouble alpha);
struct marshal_cmd_Color4dv;
uint32_t _mesa_unmarshal_Color4dv(struct gl_context *ctx, const struct marshal_cmd_Color4dv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Color4dv(const GLdouble * v);
struct marshal_cmd_Color4f;
uint32_t _mesa_unmarshal_Color4f(struct gl_context *ctx, const struct marshal_cmd_Color4f *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Color4f(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
struct marshal_cmd_Color4fv;
uint32_t _mesa_unmarshal_Color4fv(struct gl_context *ctx, const struct marshal_cmd_Color4fv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Color4fv(const GLfloat * v);
struct marshal_cmd_Color4i;
uint32_t _mesa_unmarshal_Color4i(struct gl_context *ctx, const struct marshal_cmd_Color4i *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Color4i(GLint red, GLint green, GLint blue, GLint alpha);
struct marshal_cmd_Color4iv;
uint32_t _mesa_unmarshal_Color4iv(struct gl_context *ctx, const struct marshal_cmd_Color4iv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Color4iv(const GLint * v);
struct marshal_cmd_Color4s;
uint32_t _mesa_unmarshal_Color4s(struct gl_context *ctx, const struct marshal_cmd_Color4s *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Color4s(GLshort red, GLshort green, GLshort blue, GLshort alpha);
struct marshal_cmd_Color4sv;
uint32_t _mesa_unmarshal_Color4sv(struct gl_context *ctx, const struct marshal_cmd_Color4sv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Color4sv(const GLshort * v);
struct marshal_cmd_Color4ub;
uint32_t _mesa_unmarshal_Color4ub(struct gl_context *ctx, const struct marshal_cmd_Color4ub *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Color4ub(GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha);
struct marshal_cmd_Color4ubv;
uint32_t _mesa_unmarshal_Color4ubv(struct gl_context *ctx, const struct marshal_cmd_Color4ubv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Color4ubv(const GLubyte * v);
struct marshal_cmd_Color4ui;
uint32_t _mesa_unmarshal_Color4ui(struct gl_context *ctx, const struct marshal_cmd_Color4ui *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Color4ui(GLuint red, GLuint green, GLuint blue, GLuint alpha);
struct marshal_cmd_Color4uiv;
uint32_t _mesa_unmarshal_Color4uiv(struct gl_context *ctx, const struct marshal_cmd_Color4uiv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Color4uiv(const GLuint * v);
struct marshal_cmd_Color4us;
uint32_t _mesa_unmarshal_Color4us(struct gl_context *ctx, const struct marshal_cmd_Color4us *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Color4us(GLushort red, GLushort green, GLushort blue, GLushort alpha);
struct marshal_cmd_Color4usv;
uint32_t _mesa_unmarshal_Color4usv(struct gl_context *ctx, const struct marshal_cmd_Color4usv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Color4usv(const GLushort * v);
struct marshal_cmd_EdgeFlag;
uint32_t _mesa_unmarshal_EdgeFlag(struct gl_context *ctx, const struct marshal_cmd_EdgeFlag *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_EdgeFlag(GLboolean flag);
struct marshal_cmd_EdgeFlagv;
uint32_t _mesa_unmarshal_EdgeFlagv(struct gl_context *ctx, const struct marshal_cmd_EdgeFlagv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_EdgeFlagv(const GLboolean * flag);
struct marshal_cmd_End;
uint32_t _mesa_unmarshal_End(struct gl_context *ctx, const struct marshal_cmd_End *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_End(void);
struct marshal_cmd_Indexd;
uint32_t _mesa_unmarshal_Indexd(struct gl_context *ctx, const struct marshal_cmd_Indexd *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Indexd(GLdouble c);
struct marshal_cmd_Indexdv;
uint32_t _mesa_unmarshal_Indexdv(struct gl_context *ctx, const struct marshal_cmd_Indexdv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Indexdv(const GLdouble * c);
struct marshal_cmd_Indexf;
uint32_t _mesa_unmarshal_Indexf(struct gl_context *ctx, const struct marshal_cmd_Indexf *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Indexf(GLfloat c);
struct marshal_cmd_Indexfv;
uint32_t _mesa_unmarshal_Indexfv(struct gl_context *ctx, const struct marshal_cmd_Indexfv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Indexfv(const GLfloat * c);
struct marshal_cmd_Indexi;
uint32_t _mesa_unmarshal_Indexi(struct gl_context *ctx, const struct marshal_cmd_Indexi *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Indexi(GLint c);
struct marshal_cmd_Indexiv;
uint32_t _mesa_unmarshal_Indexiv(struct gl_context *ctx, const struct marshal_cmd_Indexiv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Indexiv(const GLint * c);
struct marshal_cmd_Indexs;
uint32_t _mesa_unmarshal_Indexs(struct gl_context *ctx, const struct marshal_cmd_Indexs *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Indexs(GLshort c);
struct marshal_cmd_Indexsv;
uint32_t _mesa_unmarshal_Indexsv(struct gl_context *ctx, const struct marshal_cmd_Indexsv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Indexsv(const GLshort * c);
struct marshal_cmd_Normal3b;
uint32_t _mesa_unmarshal_Normal3b(struct gl_context *ctx, const struct marshal_cmd_Normal3b *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Normal3b(GLbyte nx, GLbyte ny, GLbyte nz);
struct marshal_cmd_Normal3bv;
uint32_t _mesa_unmarshal_Normal3bv(struct gl_context *ctx, const struct marshal_cmd_Normal3bv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Normal3bv(const GLbyte * v);
struct marshal_cmd_Normal3d;
uint32_t _mesa_unmarshal_Normal3d(struct gl_context *ctx, const struct marshal_cmd_Normal3d *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Normal3d(GLdouble nx, GLdouble ny, GLdouble nz);
struct marshal_cmd_Normal3dv;
uint32_t _mesa_unmarshal_Normal3dv(struct gl_context *ctx, const struct marshal_cmd_Normal3dv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Normal3dv(const GLdouble * v);
struct marshal_cmd_Normal3f;
uint32_t _mesa_unmarshal_Normal3f(struct gl_context *ctx, const struct marshal_cmd_Normal3f *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Normal3f(GLfloat nx, GLfloat ny, GLfloat nz);
struct marshal_cmd_Normal3fv;
uint32_t _mesa_unmarshal_Normal3fv(struct gl_context *ctx, const struct marshal_cmd_Normal3fv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Normal3fv(const GLfloat * v);
struct marshal_cmd_Normal3i;
uint32_t _mesa_unmarshal_Normal3i(struct gl_context *ctx, const struct marshal_cmd_Normal3i *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Normal3i(GLint nx, GLint ny, GLint nz);
struct marshal_cmd_Normal3iv;
uint32_t _mesa_unmarshal_Normal3iv(struct gl_context *ctx, const struct marshal_cmd_Normal3iv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Normal3iv(const GLint * v);
struct marshal_cmd_Normal3s;
uint32_t _mesa_unmarshal_Normal3s(struct gl_context *ctx, const struct marshal_cmd_Normal3s *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Normal3s(GLshort nx, GLshort ny, GLshort nz);
struct marshal_cmd_Normal3sv;
uint32_t _mesa_unmarshal_Normal3sv(struct gl_context *ctx, const struct marshal_cmd_Normal3sv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Normal3sv(const GLshort * v);
struct marshal_cmd_RasterPos2d;
uint32_t _mesa_unmarshal_RasterPos2d(struct gl_context *ctx, const struct marshal_cmd_RasterPos2d *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_RasterPos2d(GLdouble x, GLdouble y);
struct marshal_cmd_RasterPos2dv;
uint32_t _mesa_unmarshal_RasterPos2dv(struct gl_context *ctx, const struct marshal_cmd_RasterPos2dv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_RasterPos2dv(const GLdouble * v);
struct marshal_cmd_RasterPos2f;
uint32_t _mesa_unmarshal_RasterPos2f(struct gl_context *ctx, const struct marshal_cmd_RasterPos2f *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_RasterPos2f(GLfloat x, GLfloat y);
struct marshal_cmd_RasterPos2fv;
uint32_t _mesa_unmarshal_RasterPos2fv(struct gl_context *ctx, const struct marshal_cmd_RasterPos2fv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_RasterPos2fv(const GLfloat * v);
struct marshal_cmd_RasterPos2i;
uint32_t _mesa_unmarshal_RasterPos2i(struct gl_context *ctx, const struct marshal_cmd_RasterPos2i *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_RasterPos2i(GLint x, GLint y);
struct marshal_cmd_RasterPos2iv;
uint32_t _mesa_unmarshal_RasterPos2iv(struct gl_context *ctx, const struct marshal_cmd_RasterPos2iv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_RasterPos2iv(const GLint * v);
struct marshal_cmd_RasterPos2s;
uint32_t _mesa_unmarshal_RasterPos2s(struct gl_context *ctx, const struct marshal_cmd_RasterPos2s *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_RasterPos2s(GLshort x, GLshort y);
struct marshal_cmd_RasterPos2sv;
uint32_t _mesa_unmarshal_RasterPos2sv(struct gl_context *ctx, const struct marshal_cmd_RasterPos2sv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_RasterPos2sv(const GLshort * v);
struct marshal_cmd_RasterPos3d;
uint32_t _mesa_unmarshal_RasterPos3d(struct gl_context *ctx, const struct marshal_cmd_RasterPos3d *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_RasterPos3d(GLdouble x, GLdouble y, GLdouble z);
struct marshal_cmd_RasterPos3dv;
uint32_t _mesa_unmarshal_RasterPos3dv(struct gl_context *ctx, const struct marshal_cmd_RasterPos3dv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_RasterPos3dv(const GLdouble * v);
struct marshal_cmd_RasterPos3f;
uint32_t _mesa_unmarshal_RasterPos3f(struct gl_context *ctx, const struct marshal_cmd_RasterPos3f *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_RasterPos3f(GLfloat x, GLfloat y, GLfloat z);
struct marshal_cmd_RasterPos3fv;
uint32_t _mesa_unmarshal_RasterPos3fv(struct gl_context *ctx, const struct marshal_cmd_RasterPos3fv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_RasterPos3fv(const GLfloat * v);
struct marshal_cmd_RasterPos3i;
uint32_t _mesa_unmarshal_RasterPos3i(struct gl_context *ctx, const struct marshal_cmd_RasterPos3i *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_RasterPos3i(GLint x, GLint y, GLint z);
struct marshal_cmd_RasterPos3iv;
uint32_t _mesa_unmarshal_RasterPos3iv(struct gl_context *ctx, const struct marshal_cmd_RasterPos3iv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_RasterPos3iv(const GLint * v);
struct marshal_cmd_RasterPos3s;
uint32_t _mesa_unmarshal_RasterPos3s(struct gl_context *ctx, const struct marshal_cmd_RasterPos3s *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_RasterPos3s(GLshort x, GLshort y, GLshort z);
struct marshal_cmd_RasterPos3sv;
uint32_t _mesa_unmarshal_RasterPos3sv(struct gl_context *ctx, const struct marshal_cmd_RasterPos3sv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_RasterPos3sv(const GLshort * v);
struct marshal_cmd_RasterPos4d;
uint32_t _mesa_unmarshal_RasterPos4d(struct gl_context *ctx, const struct marshal_cmd_RasterPos4d *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_RasterPos4d(GLdouble x, GLdouble y, GLdouble z, GLdouble w);
struct marshal_cmd_RasterPos4dv;
uint32_t _mesa_unmarshal_RasterPos4dv(struct gl_context *ctx, const struct marshal_cmd_RasterPos4dv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_RasterPos4dv(const GLdouble * v);
struct marshal_cmd_RasterPos4f;
uint32_t _mesa_unmarshal_RasterPos4f(struct gl_context *ctx, const struct marshal_cmd_RasterPos4f *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_RasterPos4f(GLfloat x, GLfloat y, GLfloat z, GLfloat w);
struct marshal_cmd_RasterPos4fv;
uint32_t _mesa_unmarshal_RasterPos4fv(struct gl_context *ctx, const struct marshal_cmd_RasterPos4fv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_RasterPos4fv(const GLfloat * v);
struct marshal_cmd_RasterPos4i;
uint32_t _mesa_unmarshal_RasterPos4i(struct gl_context *ctx, const struct marshal_cmd_RasterPos4i *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_RasterPos4i(GLint x, GLint y, GLint z, GLint w);
struct marshal_cmd_RasterPos4iv;
uint32_t _mesa_unmarshal_RasterPos4iv(struct gl_context *ctx, const struct marshal_cmd_RasterPos4iv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_RasterPos4iv(const GLint * v);
struct marshal_cmd_RasterPos4s;
uint32_t _mesa_unmarshal_RasterPos4s(struct gl_context *ctx, const struct marshal_cmd_RasterPos4s *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_RasterPos4s(GLshort x, GLshort y, GLshort z, GLshort w);
struct marshal_cmd_RasterPos4sv;
uint32_t _mesa_unmarshal_RasterPos4sv(struct gl_context *ctx, const struct marshal_cmd_RasterPos4sv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_RasterPos4sv(const GLshort * v);
struct marshal_cmd_Rectd;
uint32_t _mesa_unmarshal_Rectd(struct gl_context *ctx, const struct marshal_cmd_Rectd *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Rectd(GLdouble x1, GLdouble y1, GLdouble x2, GLdouble y2);
struct marshal_cmd_Rectdv;
uint32_t _mesa_unmarshal_Rectdv(struct gl_context *ctx, const struct marshal_cmd_Rectdv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Rectdv(const GLdouble * v1, const GLdouble * v2);
struct marshal_cmd_Rectf;
uint32_t _mesa_unmarshal_Rectf(struct gl_context *ctx, const struct marshal_cmd_Rectf *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Rectf(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2);
struct marshal_cmd_Rectfv;
uint32_t _mesa_unmarshal_Rectfv(struct gl_context *ctx, const struct marshal_cmd_Rectfv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Rectfv(const GLfloat * v1, const GLfloat * v2);
struct marshal_cmd_Recti;
uint32_t _mesa_unmarshal_Recti(struct gl_context *ctx, const struct marshal_cmd_Recti *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Recti(GLint x1, GLint y1, GLint x2, GLint y2);
struct marshal_cmd_Rectiv;
uint32_t _mesa_unmarshal_Rectiv(struct gl_context *ctx, const struct marshal_cmd_Rectiv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Rectiv(const GLint * v1, const GLint * v2);
struct marshal_cmd_Rects;
uint32_t _mesa_unmarshal_Rects(struct gl_context *ctx, const struct marshal_cmd_Rects *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Rects(GLshort x1, GLshort y1, GLshort x2, GLshort y2);
struct marshal_cmd_Rectsv;
uint32_t _mesa_unmarshal_Rectsv(struct gl_context *ctx, const struct marshal_cmd_Rectsv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Rectsv(const GLshort * v1, const GLshort * v2);
struct marshal_cmd_TexCoord1d;
uint32_t _mesa_unmarshal_TexCoord1d(struct gl_context *ctx, const struct marshal_cmd_TexCoord1d *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TexCoord1d(GLdouble s);
struct marshal_cmd_TexCoord1dv;
uint32_t _mesa_unmarshal_TexCoord1dv(struct gl_context *ctx, const struct marshal_cmd_TexCoord1dv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TexCoord1dv(const GLdouble * v);
struct marshal_cmd_TexCoord1f;
uint32_t _mesa_unmarshal_TexCoord1f(struct gl_context *ctx, const struct marshal_cmd_TexCoord1f *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TexCoord1f(GLfloat s);
struct marshal_cmd_TexCoord1fv;
uint32_t _mesa_unmarshal_TexCoord1fv(struct gl_context *ctx, const struct marshal_cmd_TexCoord1fv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TexCoord1fv(const GLfloat * v);
struct marshal_cmd_TexCoord1i;
uint32_t _mesa_unmarshal_TexCoord1i(struct gl_context *ctx, const struct marshal_cmd_TexCoord1i *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TexCoord1i(GLint s);
struct marshal_cmd_TexCoord1iv;
uint32_t _mesa_unmarshal_TexCoord1iv(struct gl_context *ctx, const struct marshal_cmd_TexCoord1iv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TexCoord1iv(const GLint * v);
struct marshal_cmd_TexCoord1s;
uint32_t _mesa_unmarshal_TexCoord1s(struct gl_context *ctx, const struct marshal_cmd_TexCoord1s *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TexCoord1s(GLshort s);
struct marshal_cmd_TexCoord1sv;
uint32_t _mesa_unmarshal_TexCoord1sv(struct gl_context *ctx, const struct marshal_cmd_TexCoord1sv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TexCoord1sv(const GLshort * v);
struct marshal_cmd_TexCoord2d;
uint32_t _mesa_unmarshal_TexCoord2d(struct gl_context *ctx, const struct marshal_cmd_TexCoord2d *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TexCoord2d(GLdouble s, GLdouble t);
struct marshal_cmd_TexCoord2dv;
uint32_t _mesa_unmarshal_TexCoord2dv(struct gl_context *ctx, const struct marshal_cmd_TexCoord2dv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TexCoord2dv(const GLdouble * v);
struct marshal_cmd_TexCoord2f;
uint32_t _mesa_unmarshal_TexCoord2f(struct gl_context *ctx, const struct marshal_cmd_TexCoord2f *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TexCoord2f(GLfloat s, GLfloat t);
struct marshal_cmd_TexCoord2fv;
uint32_t _mesa_unmarshal_TexCoord2fv(struct gl_context *ctx, const struct marshal_cmd_TexCoord2fv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TexCoord2fv(const GLfloat * v);
struct marshal_cmd_TexCoord2i;
uint32_t _mesa_unmarshal_TexCoord2i(struct gl_context *ctx, const struct marshal_cmd_TexCoord2i *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TexCoord2i(GLint s, GLint t);
struct marshal_cmd_TexCoord2iv;
uint32_t _mesa_unmarshal_TexCoord2iv(struct gl_context *ctx, const struct marshal_cmd_TexCoord2iv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TexCoord2iv(const GLint * v);
struct marshal_cmd_TexCoord2s;
uint32_t _mesa_unmarshal_TexCoord2s(struct gl_context *ctx, const struct marshal_cmd_TexCoord2s *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TexCoord2s(GLshort s, GLshort t);
struct marshal_cmd_TexCoord2sv;
uint32_t _mesa_unmarshal_TexCoord2sv(struct gl_context *ctx, const struct marshal_cmd_TexCoord2sv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TexCoord2sv(const GLshort * v);
struct marshal_cmd_TexCoord3d;
uint32_t _mesa_unmarshal_TexCoord3d(struct gl_context *ctx, const struct marshal_cmd_TexCoord3d *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TexCoord3d(GLdouble s, GLdouble t, GLdouble r);
struct marshal_cmd_TexCoord3dv;
uint32_t _mesa_unmarshal_TexCoord3dv(struct gl_context *ctx, const struct marshal_cmd_TexCoord3dv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TexCoord3dv(const GLdouble * v);
struct marshal_cmd_TexCoord3f;
uint32_t _mesa_unmarshal_TexCoord3f(struct gl_context *ctx, const struct marshal_cmd_TexCoord3f *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TexCoord3f(GLfloat s, GLfloat t, GLfloat r);
struct marshal_cmd_TexCoord3fv;
uint32_t _mesa_unmarshal_TexCoord3fv(struct gl_context *ctx, const struct marshal_cmd_TexCoord3fv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TexCoord3fv(const GLfloat * v);
struct marshal_cmd_TexCoord3i;
uint32_t _mesa_unmarshal_TexCoord3i(struct gl_context *ctx, const struct marshal_cmd_TexCoord3i *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TexCoord3i(GLint s, GLint t, GLint r);
struct marshal_cmd_TexCoord3iv;
uint32_t _mesa_unmarshal_TexCoord3iv(struct gl_context *ctx, const struct marshal_cmd_TexCoord3iv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TexCoord3iv(const GLint * v);
struct marshal_cmd_TexCoord3s;
uint32_t _mesa_unmarshal_TexCoord3s(struct gl_context *ctx, const struct marshal_cmd_TexCoord3s *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TexCoord3s(GLshort s, GLshort t, GLshort r);
struct marshal_cmd_TexCoord3sv;
uint32_t _mesa_unmarshal_TexCoord3sv(struct gl_context *ctx, const struct marshal_cmd_TexCoord3sv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TexCoord3sv(const GLshort * v);
struct marshal_cmd_TexCoord4d;
uint32_t _mesa_unmarshal_TexCoord4d(struct gl_context *ctx, const struct marshal_cmd_TexCoord4d *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TexCoord4d(GLdouble s, GLdouble t, GLdouble r, GLdouble q);
struct marshal_cmd_TexCoord4dv;
uint32_t _mesa_unmarshal_TexCoord4dv(struct gl_context *ctx, const struct marshal_cmd_TexCoord4dv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TexCoord4dv(const GLdouble * v);
struct marshal_cmd_TexCoord4f;
uint32_t _mesa_unmarshal_TexCoord4f(struct gl_context *ctx, const struct marshal_cmd_TexCoord4f *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TexCoord4f(GLfloat s, GLfloat t, GLfloat r, GLfloat q);
struct marshal_cmd_TexCoord4fv;
uint32_t _mesa_unmarshal_TexCoord4fv(struct gl_context *ctx, const struct marshal_cmd_TexCoord4fv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TexCoord4fv(const GLfloat * v);
struct marshal_cmd_TexCoord4i;
uint32_t _mesa_unmarshal_TexCoord4i(struct gl_context *ctx, const struct marshal_cmd_TexCoord4i *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TexCoord4i(GLint s, GLint t, GLint r, GLint q);
struct marshal_cmd_TexCoord4iv;
uint32_t _mesa_unmarshal_TexCoord4iv(struct gl_context *ctx, const struct marshal_cmd_TexCoord4iv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TexCoord4iv(const GLint * v);
struct marshal_cmd_TexCoord4s;
uint32_t _mesa_unmarshal_TexCoord4s(struct gl_context *ctx, const struct marshal_cmd_TexCoord4s *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TexCoord4s(GLshort s, GLshort t, GLshort r, GLshort q);
struct marshal_cmd_TexCoord4sv;
uint32_t _mesa_unmarshal_TexCoord4sv(struct gl_context *ctx, const struct marshal_cmd_TexCoord4sv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TexCoord4sv(const GLshort * v);
struct marshal_cmd_Vertex2d;
uint32_t _mesa_unmarshal_Vertex2d(struct gl_context *ctx, const struct marshal_cmd_Vertex2d *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Vertex2d(GLdouble x, GLdouble y);
struct marshal_cmd_Vertex2dv;
uint32_t _mesa_unmarshal_Vertex2dv(struct gl_context *ctx, const struct marshal_cmd_Vertex2dv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Vertex2dv(const GLdouble * v);
struct marshal_cmd_Vertex2f;
uint32_t _mesa_unmarshal_Vertex2f(struct gl_context *ctx, const struct marshal_cmd_Vertex2f *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Vertex2f(GLfloat x, GLfloat y);
struct marshal_cmd_Vertex2fv;
uint32_t _mesa_unmarshal_Vertex2fv(struct gl_context *ctx, const struct marshal_cmd_Vertex2fv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Vertex2fv(const GLfloat * v);
struct marshal_cmd_Vertex2i;
uint32_t _mesa_unmarshal_Vertex2i(struct gl_context *ctx, const struct marshal_cmd_Vertex2i *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Vertex2i(GLint x, GLint y);
struct marshal_cmd_Vertex2iv;
uint32_t _mesa_unmarshal_Vertex2iv(struct gl_context *ctx, const struct marshal_cmd_Vertex2iv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Vertex2iv(const GLint * v);
struct marshal_cmd_Vertex2s;
uint32_t _mesa_unmarshal_Vertex2s(struct gl_context *ctx, const struct marshal_cmd_Vertex2s *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Vertex2s(GLshort x, GLshort y);
struct marshal_cmd_Vertex2sv;
uint32_t _mesa_unmarshal_Vertex2sv(struct gl_context *ctx, const struct marshal_cmd_Vertex2sv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Vertex2sv(const GLshort * v);
struct marshal_cmd_Vertex3d;
uint32_t _mesa_unmarshal_Vertex3d(struct gl_context *ctx, const struct marshal_cmd_Vertex3d *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Vertex3d(GLdouble x, GLdouble y, GLdouble z);
struct marshal_cmd_Vertex3dv;
uint32_t _mesa_unmarshal_Vertex3dv(struct gl_context *ctx, const struct marshal_cmd_Vertex3dv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Vertex3dv(const GLdouble * v);
struct marshal_cmd_Vertex3f;
uint32_t _mesa_unmarshal_Vertex3f(struct gl_context *ctx, const struct marshal_cmd_Vertex3f *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Vertex3f(GLfloat x, GLfloat y, GLfloat z);
struct marshal_cmd_Vertex3fv;
uint32_t _mesa_unmarshal_Vertex3fv(struct gl_context *ctx, const struct marshal_cmd_Vertex3fv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Vertex3fv(const GLfloat * v);
struct marshal_cmd_Vertex3i;
uint32_t _mesa_unmarshal_Vertex3i(struct gl_context *ctx, const struct marshal_cmd_Vertex3i *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Vertex3i(GLint x, GLint y, GLint z);
struct marshal_cmd_Vertex3iv;
uint32_t _mesa_unmarshal_Vertex3iv(struct gl_context *ctx, const struct marshal_cmd_Vertex3iv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Vertex3iv(const GLint * v);
struct marshal_cmd_Vertex3s;
uint32_t _mesa_unmarshal_Vertex3s(struct gl_context *ctx, const struct marshal_cmd_Vertex3s *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Vertex3s(GLshort x, GLshort y, GLshort z);
struct marshal_cmd_Vertex3sv;
uint32_t _mesa_unmarshal_Vertex3sv(struct gl_context *ctx, const struct marshal_cmd_Vertex3sv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Vertex3sv(const GLshort * v);
struct marshal_cmd_Vertex4d;
uint32_t _mesa_unmarshal_Vertex4d(struct gl_context *ctx, const struct marshal_cmd_Vertex4d *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Vertex4d(GLdouble x, GLdouble y, GLdouble z, GLdouble w);
struct marshal_cmd_Vertex4dv;
uint32_t _mesa_unmarshal_Vertex4dv(struct gl_context *ctx, const struct marshal_cmd_Vertex4dv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Vertex4dv(const GLdouble * v);
struct marshal_cmd_Vertex4f;
uint32_t _mesa_unmarshal_Vertex4f(struct gl_context *ctx, const struct marshal_cmd_Vertex4f *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Vertex4f(GLfloat x, GLfloat y, GLfloat z, GLfloat w);
struct marshal_cmd_Vertex4fv;
uint32_t _mesa_unmarshal_Vertex4fv(struct gl_context *ctx, const struct marshal_cmd_Vertex4fv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Vertex4fv(const GLfloat * v);
struct marshal_cmd_Vertex4i;
uint32_t _mesa_unmarshal_Vertex4i(struct gl_context *ctx, const struct marshal_cmd_Vertex4i *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Vertex4i(GLint x, GLint y, GLint z, GLint w);
struct marshal_cmd_Vertex4iv;
uint32_t _mesa_unmarshal_Vertex4iv(struct gl_context *ctx, const struct marshal_cmd_Vertex4iv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Vertex4iv(const GLint * v);
struct marshal_cmd_Vertex4s;
uint32_t _mesa_unmarshal_Vertex4s(struct gl_context *ctx, const struct marshal_cmd_Vertex4s *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Vertex4s(GLshort x, GLshort y, GLshort z, GLshort w);
struct marshal_cmd_Vertex4sv;
uint32_t _mesa_unmarshal_Vertex4sv(struct gl_context *ctx, const struct marshal_cmd_Vertex4sv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Vertex4sv(const GLshort * v);
struct marshal_cmd_ClipPlane;
uint32_t _mesa_unmarshal_ClipPlane(struct gl_context *ctx, const struct marshal_cmd_ClipPlane *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ClipPlane(GLenum plane, const GLdouble * equation);
struct marshal_cmd_ColorMaterial;
uint32_t _mesa_unmarshal_ColorMaterial(struct gl_context *ctx, const struct marshal_cmd_ColorMaterial *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ColorMaterial(GLenum face, GLenum mode);
struct marshal_cmd_CullFace;
uint32_t _mesa_unmarshal_CullFace(struct gl_context *ctx, const struct marshal_cmd_CullFace *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_CullFace(GLenum mode);
struct marshal_cmd_Fogf;
uint32_t _mesa_unmarshal_Fogf(struct gl_context *ctx, const struct marshal_cmd_Fogf *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Fogf(GLenum pname, GLfloat param);
struct marshal_cmd_Fogfv;
uint32_t _mesa_unmarshal_Fogfv(struct gl_context *ctx, const struct marshal_cmd_Fogfv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Fogfv(GLenum pname, const GLfloat * params);
struct marshal_cmd_Fogi;
uint32_t _mesa_unmarshal_Fogi(struct gl_context *ctx, const struct marshal_cmd_Fogi *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Fogi(GLenum pname, GLint param);
struct marshal_cmd_Fogiv;
uint32_t _mesa_unmarshal_Fogiv(struct gl_context *ctx, const struct marshal_cmd_Fogiv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Fogiv(GLenum pname, const GLint * params);
struct marshal_cmd_FrontFace;
uint32_t _mesa_unmarshal_FrontFace(struct gl_context *ctx, const struct marshal_cmd_FrontFace *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_FrontFace(GLenum mode);
struct marshal_cmd_Hint;
uint32_t _mesa_unmarshal_Hint(struct gl_context *ctx, const struct marshal_cmd_Hint *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Hint(GLenum target, GLenum mode);
struct marshal_cmd_Lightf;
uint32_t _mesa_unmarshal_Lightf(struct gl_context *ctx, const struct marshal_cmd_Lightf *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Lightf(GLenum light, GLenum pname, GLfloat param);
struct marshal_cmd_Lightfv;
uint32_t _mesa_unmarshal_Lightfv(struct gl_context *ctx, const struct marshal_cmd_Lightfv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Lightfv(GLenum light, GLenum pname, const GLfloat * params);
struct marshal_cmd_Lighti;
uint32_t _mesa_unmarshal_Lighti(struct gl_context *ctx, const struct marshal_cmd_Lighti *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Lighti(GLenum light, GLenum pname, GLint param);
struct marshal_cmd_Lightiv;
uint32_t _mesa_unmarshal_Lightiv(struct gl_context *ctx, const struct marshal_cmd_Lightiv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Lightiv(GLenum light, GLenum pname, const GLint * params);
struct marshal_cmd_LightModelf;
uint32_t _mesa_unmarshal_LightModelf(struct gl_context *ctx, const struct marshal_cmd_LightModelf *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_LightModelf(GLenum pname, GLfloat param);
struct marshal_cmd_LightModelfv;
uint32_t _mesa_unmarshal_LightModelfv(struct gl_context *ctx, const struct marshal_cmd_LightModelfv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_LightModelfv(GLenum pname, const GLfloat * params);
struct marshal_cmd_LightModeli;
uint32_t _mesa_unmarshal_LightModeli(struct gl_context *ctx, const struct marshal_cmd_LightModeli *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_LightModeli(GLenum pname, GLint param);
struct marshal_cmd_LightModeliv;
uint32_t _mesa_unmarshal_LightModeliv(struct gl_context *ctx, const struct marshal_cmd_LightModeliv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_LightModeliv(GLenum pname, const GLint * params);
struct marshal_cmd_LineStipple;
uint32_t _mesa_unmarshal_LineStipple(struct gl_context *ctx, const struct marshal_cmd_LineStipple *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_LineStipple(GLint factor, GLushort pattern);
struct marshal_cmd_LineWidth;
uint32_t _mesa_unmarshal_LineWidth(struct gl_context *ctx, const struct marshal_cmd_LineWidth *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_LineWidth(GLfloat width);
struct marshal_cmd_Materialf;
uint32_t _mesa_unmarshal_Materialf(struct gl_context *ctx, const struct marshal_cmd_Materialf *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Materialf(GLenum face, GLenum pname, GLfloat param);
struct marshal_cmd_Materialfv;
uint32_t _mesa_unmarshal_Materialfv(struct gl_context *ctx, const struct marshal_cmd_Materialfv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Materialfv(GLenum face, GLenum pname, const GLfloat * params);
struct marshal_cmd_Materiali;
uint32_t _mesa_unmarshal_Materiali(struct gl_context *ctx, const struct marshal_cmd_Materiali *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Materiali(GLenum face, GLenum pname, GLint param);
struct marshal_cmd_Materialiv;
uint32_t _mesa_unmarshal_Materialiv(struct gl_context *ctx, const struct marshal_cmd_Materialiv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Materialiv(GLenum face, GLenum pname, const GLint * params);
struct marshal_cmd_PointSize;
uint32_t _mesa_unmarshal_PointSize(struct gl_context *ctx, const struct marshal_cmd_PointSize *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_PointSize(GLfloat size);
struct marshal_cmd_PolygonMode;
uint32_t _mesa_unmarshal_PolygonMode(struct gl_context *ctx, const struct marshal_cmd_PolygonMode *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_PolygonMode(GLenum face, GLenum mode);
struct marshal_cmd_PolygonStipple;
uint32_t _mesa_unmarshal_PolygonStipple(struct gl_context *ctx, const struct marshal_cmd_PolygonStipple *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_PolygonStipple(const GLubyte * mask);
struct marshal_cmd_Scissor;
uint32_t _mesa_unmarshal_Scissor(struct gl_context *ctx, const struct marshal_cmd_Scissor *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Scissor(GLint x, GLint y, GLsizei width, GLsizei height);
struct marshal_cmd_ShadeModel;
uint32_t _mesa_unmarshal_ShadeModel(struct gl_context *ctx, const struct marshal_cmd_ShadeModel *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ShadeModel(GLenum mode);
struct marshal_cmd_TexParameterf;
uint32_t _mesa_unmarshal_TexParameterf(struct gl_context *ctx, const struct marshal_cmd_TexParameterf *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TexParameterf(GLenum target, GLenum pname, GLfloat param);
struct marshal_cmd_TexParameterfv;
uint32_t _mesa_unmarshal_TexParameterfv(struct gl_context *ctx, const struct marshal_cmd_TexParameterfv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TexParameterfv(GLenum target, GLenum pname, const GLfloat * params);
struct marshal_cmd_TexParameteri;
uint32_t _mesa_unmarshal_TexParameteri(struct gl_context *ctx, const struct marshal_cmd_TexParameteri *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TexParameteri(GLenum target, GLenum pname, GLint param);
struct marshal_cmd_TexParameteriv;
uint32_t _mesa_unmarshal_TexParameteriv(struct gl_context *ctx, const struct marshal_cmd_TexParameteriv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TexParameteriv(GLenum target, GLenum pname, const GLint * params);
struct marshal_cmd_TexImage1D;
uint32_t _mesa_unmarshal_TexImage1D(struct gl_context *ctx, const struct marshal_cmd_TexImage1D *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TexImage1D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, const GLvoid * pixels);
struct marshal_cmd_TexImage2D;
uint32_t _mesa_unmarshal_TexImage2D(struct gl_context *ctx, const struct marshal_cmd_TexImage2D *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid * pixels);
struct marshal_cmd_TexEnvf;
uint32_t _mesa_unmarshal_TexEnvf(struct gl_context *ctx, const struct marshal_cmd_TexEnvf *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TexEnvf(GLenum target, GLenum pname, GLfloat param);
struct marshal_cmd_TexEnvfv;
uint32_t _mesa_unmarshal_TexEnvfv(struct gl_context *ctx, const struct marshal_cmd_TexEnvfv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TexEnvfv(GLenum target, GLenum pname, const GLfloat * params);
struct marshal_cmd_TexEnvi;
uint32_t _mesa_unmarshal_TexEnvi(struct gl_context *ctx, const struct marshal_cmd_TexEnvi *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TexEnvi(GLenum target, GLenum pname, GLint param);
struct marshal_cmd_TexEnviv;
uint32_t _mesa_unmarshal_TexEnviv(struct gl_context *ctx, const struct marshal_cmd_TexEnviv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TexEnviv(GLenum target, GLenum pname, const GLint * params);
struct marshal_cmd_TexGend;
uint32_t _mesa_unmarshal_TexGend(struct gl_context *ctx, const struct marshal_cmd_TexGend *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TexGend(GLenum coord, GLenum pname, GLdouble param);
struct marshal_cmd_TexGendv;
uint32_t _mesa_unmarshal_TexGendv(struct gl_context *ctx, const struct marshal_cmd_TexGendv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TexGendv(GLenum coord, GLenum pname, const GLdouble * params);
struct marshal_cmd_TexGenf;
uint32_t _mesa_unmarshal_TexGenf(struct gl_context *ctx, const struct marshal_cmd_TexGenf *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TexGenf(GLenum coord, GLenum pname, GLfloat param);
struct marshal_cmd_TexGenfv;
uint32_t _mesa_unmarshal_TexGenfv(struct gl_context *ctx, const struct marshal_cmd_TexGenfv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TexGenfv(GLenum coord, GLenum pname, const GLfloat * params);
struct marshal_cmd_TexGeni;
uint32_t _mesa_unmarshal_TexGeni(struct gl_context *ctx, const struct marshal_cmd_TexGeni *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TexGeni(GLenum coord, GLenum pname, GLint param);
struct marshal_cmd_TexGeniv;
uint32_t _mesa_unmarshal_TexGeniv(struct gl_context *ctx, const struct marshal_cmd_TexGeniv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TexGeniv(GLenum coord, GLenum pname, const GLint * params);
void GLAPIENTRY _mesa_marshal_FeedbackBuffer(GLsizei size, GLenum type, GLfloat * buffer);
void GLAPIENTRY _mesa_marshal_SelectBuffer(GLsizei size, GLuint * buffer);
GLint GLAPIENTRY _mesa_marshal_RenderMode(GLenum mode);
struct marshal_cmd_InitNames;
uint32_t _mesa_unmarshal_InitNames(struct gl_context *ctx, const struct marshal_cmd_InitNames *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_InitNames(void);
struct marshal_cmd_LoadName;
uint32_t _mesa_unmarshal_LoadName(struct gl_context *ctx, const struct marshal_cmd_LoadName *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_LoadName(GLuint name);
struct marshal_cmd_PassThrough;
uint32_t _mesa_unmarshal_PassThrough(struct gl_context *ctx, const struct marshal_cmd_PassThrough *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_PassThrough(GLfloat token);
struct marshal_cmd_PopName;
uint32_t _mesa_unmarshal_PopName(struct gl_context *ctx, const struct marshal_cmd_PopName *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_PopName(void);
struct marshal_cmd_PushName;
uint32_t _mesa_unmarshal_PushName(struct gl_context *ctx, const struct marshal_cmd_PushName *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_PushName(GLuint name);
struct marshal_cmd_DrawBuffer;
uint32_t _mesa_unmarshal_DrawBuffer(struct gl_context *ctx, const struct marshal_cmd_DrawBuffer *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_DrawBuffer(GLenum mode);
struct marshal_cmd_Clear;
uint32_t _mesa_unmarshal_Clear(struct gl_context *ctx, const struct marshal_cmd_Clear *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Clear(GLbitfield mask);
struct marshal_cmd_ClearAccum;
uint32_t _mesa_unmarshal_ClearAccum(struct gl_context *ctx, const struct marshal_cmd_ClearAccum *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ClearAccum(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
struct marshal_cmd_ClearIndex;
uint32_t _mesa_unmarshal_ClearIndex(struct gl_context *ctx, const struct marshal_cmd_ClearIndex *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ClearIndex(GLfloat c);
struct marshal_cmd_ClearColor;
uint32_t _mesa_unmarshal_ClearColor(struct gl_context *ctx, const struct marshal_cmd_ClearColor *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ClearColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
struct marshal_cmd_ClearStencil;
uint32_t _mesa_unmarshal_ClearStencil(struct gl_context *ctx, const struct marshal_cmd_ClearStencil *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ClearStencil(GLint s);
struct marshal_cmd_ClearDepth;
uint32_t _mesa_unmarshal_ClearDepth(struct gl_context *ctx, const struct marshal_cmd_ClearDepth *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ClearDepth(GLclampd depth);
struct marshal_cmd_StencilMask;
uint32_t _mesa_unmarshal_StencilMask(struct gl_context *ctx, const struct marshal_cmd_StencilMask *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_StencilMask(GLuint mask);
struct marshal_cmd_ColorMask;
uint32_t _mesa_unmarshal_ColorMask(struct gl_context *ctx, const struct marshal_cmd_ColorMask *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ColorMask(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
struct marshal_cmd_DepthMask;
uint32_t _mesa_unmarshal_DepthMask(struct gl_context *ctx, const struct marshal_cmd_DepthMask *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_DepthMask(GLboolean flag);
struct marshal_cmd_IndexMask;
uint32_t _mesa_unmarshal_IndexMask(struct gl_context *ctx, const struct marshal_cmd_IndexMask *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_IndexMask(GLuint mask);
struct marshal_cmd_Accum;
uint32_t _mesa_unmarshal_Accum(struct gl_context *ctx, const struct marshal_cmd_Accum *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Accum(GLenum op, GLfloat value);
struct marshal_cmd_Disable;
uint32_t _mesa_unmarshal_Disable(struct gl_context *ctx, const struct marshal_cmd_Disable *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Disable(GLenum cap);
struct marshal_cmd_Enable;
uint32_t _mesa_unmarshal_Enable(struct gl_context *ctx, const struct marshal_cmd_Enable *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Enable(GLenum cap);
void GLAPIENTRY _mesa_marshal_Finish(void);
struct marshal_cmd_Flush;
uint32_t _mesa_unmarshal_Flush(struct gl_context *ctx, const struct marshal_cmd_Flush *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Flush(void);
struct marshal_cmd_PopAttrib;
uint32_t _mesa_unmarshal_PopAttrib(struct gl_context *ctx, const struct marshal_cmd_PopAttrib *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_PopAttrib(void);
struct marshal_cmd_PushAttrib;
uint32_t _mesa_unmarshal_PushAttrib(struct gl_context *ctx, const struct marshal_cmd_PushAttrib *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_PushAttrib(GLbitfield mask);
void GLAPIENTRY _mesa_marshal_Map1d(GLenum target, GLdouble u1, GLdouble u2, GLint stride, GLint order, const GLdouble * points);
void GLAPIENTRY _mesa_marshal_Map1f(GLenum target, GLfloat u1, GLfloat u2, GLint stride, GLint order, const GLfloat * points);
void GLAPIENTRY _mesa_marshal_Map2d(GLenum target, GLdouble u1, GLdouble u2, GLint ustride, GLint uorder, GLdouble v1, GLdouble v2, GLint vstride, GLint vorder, const GLdouble * points);
void GLAPIENTRY _mesa_marshal_Map2f(GLenum target, GLfloat u1, GLfloat u2, GLint ustride, GLint uorder, GLfloat v1, GLfloat v2, GLint vstride, GLint vorder, const GLfloat * points);
struct marshal_cmd_MapGrid1d;
uint32_t _mesa_unmarshal_MapGrid1d(struct gl_context *ctx, const struct marshal_cmd_MapGrid1d *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_MapGrid1d(GLint un, GLdouble u1, GLdouble u2);
struct marshal_cmd_MapGrid1f;
uint32_t _mesa_unmarshal_MapGrid1f(struct gl_context *ctx, const struct marshal_cmd_MapGrid1f *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_MapGrid1f(GLint un, GLfloat u1, GLfloat u2);
struct marshal_cmd_MapGrid2d;
uint32_t _mesa_unmarshal_MapGrid2d(struct gl_context *ctx, const struct marshal_cmd_MapGrid2d *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_MapGrid2d(GLint un, GLdouble u1, GLdouble u2, GLint vn, GLdouble v1, GLdouble v2);
struct marshal_cmd_MapGrid2f;
uint32_t _mesa_unmarshal_MapGrid2f(struct gl_context *ctx, const struct marshal_cmd_MapGrid2f *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_MapGrid2f(GLint un, GLfloat u1, GLfloat u2, GLint vn, GLfloat v1, GLfloat v2);
struct marshal_cmd_EvalCoord1d;
uint32_t _mesa_unmarshal_EvalCoord1d(struct gl_context *ctx, const struct marshal_cmd_EvalCoord1d *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_EvalCoord1d(GLdouble u);
struct marshal_cmd_EvalCoord1dv;
uint32_t _mesa_unmarshal_EvalCoord1dv(struct gl_context *ctx, const struct marshal_cmd_EvalCoord1dv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_EvalCoord1dv(const GLdouble * u);
struct marshal_cmd_EvalCoord1f;
uint32_t _mesa_unmarshal_EvalCoord1f(struct gl_context *ctx, const struct marshal_cmd_EvalCoord1f *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_EvalCoord1f(GLfloat u);
struct marshal_cmd_EvalCoord1fv;
uint32_t _mesa_unmarshal_EvalCoord1fv(struct gl_context *ctx, const struct marshal_cmd_EvalCoord1fv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_EvalCoord1fv(const GLfloat * u);
struct marshal_cmd_EvalCoord2d;
uint32_t _mesa_unmarshal_EvalCoord2d(struct gl_context *ctx, const struct marshal_cmd_EvalCoord2d *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_EvalCoord2d(GLdouble u, GLdouble v);
struct marshal_cmd_EvalCoord2dv;
uint32_t _mesa_unmarshal_EvalCoord2dv(struct gl_context *ctx, const struct marshal_cmd_EvalCoord2dv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_EvalCoord2dv(const GLdouble * u);
struct marshal_cmd_EvalCoord2f;
uint32_t _mesa_unmarshal_EvalCoord2f(struct gl_context *ctx, const struct marshal_cmd_EvalCoord2f *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_EvalCoord2f(GLfloat u, GLfloat v);
struct marshal_cmd_EvalCoord2fv;
uint32_t _mesa_unmarshal_EvalCoord2fv(struct gl_context *ctx, const struct marshal_cmd_EvalCoord2fv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_EvalCoord2fv(const GLfloat * u);
struct marshal_cmd_EvalMesh1;
uint32_t _mesa_unmarshal_EvalMesh1(struct gl_context *ctx, const struct marshal_cmd_EvalMesh1 *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_EvalMesh1(GLenum mode, GLint i1, GLint i2);
struct marshal_cmd_EvalPoint1;
uint32_t _mesa_unmarshal_EvalPoint1(struct gl_context *ctx, const struct marshal_cmd_EvalPoint1 *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_EvalPoint1(GLint i);
struct marshal_cmd_EvalMesh2;
uint32_t _mesa_unmarshal_EvalMesh2(struct gl_context *ctx, const struct marshal_cmd_EvalMesh2 *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_EvalMesh2(GLenum mode, GLint i1, GLint i2, GLint j1, GLint j2);
struct marshal_cmd_EvalPoint2;
uint32_t _mesa_unmarshal_EvalPoint2(struct gl_context *ctx, const struct marshal_cmd_EvalPoint2 *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_EvalPoint2(GLint i, GLint j);
struct marshal_cmd_AlphaFunc;
uint32_t _mesa_unmarshal_AlphaFunc(struct gl_context *ctx, const struct marshal_cmd_AlphaFunc *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_AlphaFunc(GLenum func, GLclampf ref);
struct marshal_cmd_BlendFunc;
uint32_t _mesa_unmarshal_BlendFunc(struct gl_context *ctx, const struct marshal_cmd_BlendFunc *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_BlendFunc(GLenum sfactor, GLenum dfactor);
struct marshal_cmd_LogicOp;
uint32_t _mesa_unmarshal_LogicOp(struct gl_context *ctx, const struct marshal_cmd_LogicOp *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_LogicOp(GLenum opcode);
struct marshal_cmd_StencilFunc;
uint32_t _mesa_unmarshal_StencilFunc(struct gl_context *ctx, const struct marshal_cmd_StencilFunc *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_StencilFunc(GLenum func, GLint ref, GLuint mask);
struct marshal_cmd_StencilOp;
uint32_t _mesa_unmarshal_StencilOp(struct gl_context *ctx, const struct marshal_cmd_StencilOp *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_StencilOp(GLenum fail, GLenum zfail, GLenum zpass);
struct marshal_cmd_DepthFunc;
uint32_t _mesa_unmarshal_DepthFunc(struct gl_context *ctx, const struct marshal_cmd_DepthFunc *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_DepthFunc(GLenum func);
struct marshal_cmd_PixelZoom;
uint32_t _mesa_unmarshal_PixelZoom(struct gl_context *ctx, const struct marshal_cmd_PixelZoom *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_PixelZoom(GLfloat xfactor, GLfloat yfactor);
struct marshal_cmd_PixelTransferf;
uint32_t _mesa_unmarshal_PixelTransferf(struct gl_context *ctx, const struct marshal_cmd_PixelTransferf *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_PixelTransferf(GLenum pname, GLfloat param);
struct marshal_cmd_PixelTransferi;
uint32_t _mesa_unmarshal_PixelTransferi(struct gl_context *ctx, const struct marshal_cmd_PixelTransferi *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_PixelTransferi(GLenum pname, GLint param);
struct marshal_cmd_PixelStoref;
uint32_t _mesa_unmarshal_PixelStoref(struct gl_context *ctx, const struct marshal_cmd_PixelStoref *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_PixelStoref(GLenum pname, GLfloat param);
struct marshal_cmd_PixelStorei;
uint32_t _mesa_unmarshal_PixelStorei(struct gl_context *ctx, const struct marshal_cmd_PixelStorei *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_PixelStorei(GLenum pname, GLint param);
struct marshal_cmd_PixelMapfv;
uint32_t _mesa_unmarshal_PixelMapfv(struct gl_context *ctx, const struct marshal_cmd_PixelMapfv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_PixelMapfv(GLenum map, GLsizei mapsize, const GLfloat * values);
struct marshal_cmd_PixelMapuiv;
uint32_t _mesa_unmarshal_PixelMapuiv(struct gl_context *ctx, const struct marshal_cmd_PixelMapuiv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_PixelMapuiv(GLenum map, GLsizei mapsize, const GLuint * values);
struct marshal_cmd_PixelMapusv;
uint32_t _mesa_unmarshal_PixelMapusv(struct gl_context *ctx, const struct marshal_cmd_PixelMapusv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_PixelMapusv(GLenum map, GLsizei mapsize, const GLushort * values);
struct marshal_cmd_ReadBuffer;
uint32_t _mesa_unmarshal_ReadBuffer(struct gl_context *ctx, const struct marshal_cmd_ReadBuffer *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ReadBuffer(GLenum mode);
struct marshal_cmd_CopyPixels;
uint32_t _mesa_unmarshal_CopyPixels(struct gl_context *ctx, const struct marshal_cmd_CopyPixels *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_CopyPixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum type);
struct marshal_cmd_ReadPixels;
uint32_t _mesa_unmarshal_ReadPixels(struct gl_context *ctx, const struct marshal_cmd_ReadPixels *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ReadPixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid * pixels);
struct marshal_cmd_DrawPixels;
uint32_t _mesa_unmarshal_DrawPixels(struct gl_context *ctx, const struct marshal_cmd_DrawPixels *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_DrawPixels(GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid * pixels);
void GLAPIENTRY _mesa_marshal_GetBooleanv(GLenum pname, GLboolean * params);
void GLAPIENTRY _mesa_marshal_GetClipPlane(GLenum plane, GLdouble * equation);
void GLAPIENTRY _mesa_marshal_GetDoublev(GLenum pname, GLdouble * params);
GLenum GLAPIENTRY _mesa_marshal_GetError(void);
void GLAPIENTRY _mesa_marshal_GetFloatv(GLenum pname, GLfloat * params);
struct marshal_cmd_GetIntegerv;
uint32_t _mesa_unmarshal_GetIntegerv(struct gl_context *ctx, const struct marshal_cmd_GetIntegerv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_GetIntegerv(GLenum pname, GLint * params);
void GLAPIENTRY _mesa_marshal_GetLightfv(GLenum light, GLenum pname, GLfloat * params);
void GLAPIENTRY _mesa_marshal_GetLightiv(GLenum light, GLenum pname, GLint * params);
void GLAPIENTRY _mesa_marshal_GetMapdv(GLenum target, GLenum query, GLdouble * v);
void GLAPIENTRY _mesa_marshal_GetMapfv(GLenum target, GLenum query, GLfloat * v);
void GLAPIENTRY _mesa_marshal_GetMapiv(GLenum target, GLenum query, GLint * v);
void GLAPIENTRY _mesa_marshal_GetMaterialfv(GLenum face, GLenum pname, GLfloat * params);
void GLAPIENTRY _mesa_marshal_GetMaterialiv(GLenum face, GLenum pname, GLint * params);
struct marshal_cmd_GetPixelMapfv;
uint32_t _mesa_unmarshal_GetPixelMapfv(struct gl_context *ctx, const struct marshal_cmd_GetPixelMapfv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_GetPixelMapfv(GLenum map, GLfloat * values);
struct marshal_cmd_GetPixelMapuiv;
uint32_t _mesa_unmarshal_GetPixelMapuiv(struct gl_context *ctx, const struct marshal_cmd_GetPixelMapuiv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_GetPixelMapuiv(GLenum map, GLuint * values);
struct marshal_cmd_GetPixelMapusv;
uint32_t _mesa_unmarshal_GetPixelMapusv(struct gl_context *ctx, const struct marshal_cmd_GetPixelMapusv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_GetPixelMapusv(GLenum map, GLushort * values);
struct marshal_cmd_GetPolygonStipple;
uint32_t _mesa_unmarshal_GetPolygonStipple(struct gl_context *ctx, const struct marshal_cmd_GetPolygonStipple *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_GetPolygonStipple(GLubyte * mask);
const GLubyte * GLAPIENTRY _mesa_marshal_GetString(GLenum name);
void GLAPIENTRY _mesa_marshal_GetTexEnvfv(GLenum target, GLenum pname, GLfloat * params);
void GLAPIENTRY _mesa_marshal_GetTexEnviv(GLenum target, GLenum pname, GLint * params);
void GLAPIENTRY _mesa_marshal_GetTexGendv(GLenum coord, GLenum pname, GLdouble * params);
void GLAPIENTRY _mesa_marshal_GetTexGenfv(GLenum coord, GLenum pname, GLfloat * params);
void GLAPIENTRY _mesa_marshal_GetTexGeniv(GLenum coord, GLenum pname, GLint * params);
struct marshal_cmd_GetTexImage;
uint32_t _mesa_unmarshal_GetTexImage(struct gl_context *ctx, const struct marshal_cmd_GetTexImage *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_GetTexImage(GLenum target, GLint level, GLenum format, GLenum type, GLvoid * pixels);
void GLAPIENTRY _mesa_marshal_GetTexParameterfv(GLenum target, GLenum pname, GLfloat * params);
void GLAPIENTRY _mesa_marshal_GetTexParameteriv(GLenum target, GLenum pname, GLint * params);
void GLAPIENTRY _mesa_marshal_GetTexLevelParameterfv(GLenum target, GLint level, GLenum pname, GLfloat * params);
void GLAPIENTRY _mesa_marshal_GetTexLevelParameteriv(GLenum target, GLint level, GLenum pname, GLint * params);
GLboolean GLAPIENTRY _mesa_marshal_IsEnabled(GLenum cap);
GLboolean GLAPIENTRY _mesa_marshal_IsList(GLuint list);
struct marshal_cmd_DepthRange;
uint32_t _mesa_unmarshal_DepthRange(struct gl_context *ctx, const struct marshal_cmd_DepthRange *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_DepthRange(GLclampd zNear, GLclampd zFar);
struct marshal_cmd_Frustum;
uint32_t _mesa_unmarshal_Frustum(struct gl_context *ctx, const struct marshal_cmd_Frustum *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Frustum(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);
struct marshal_cmd_LoadIdentity;
uint32_t _mesa_unmarshal_LoadIdentity(struct gl_context *ctx, const struct marshal_cmd_LoadIdentity *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_LoadIdentity(void);
struct marshal_cmd_LoadMatrixf;
uint32_t _mesa_unmarshal_LoadMatrixf(struct gl_context *ctx, const struct marshal_cmd_LoadMatrixf *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_LoadMatrixf(const GLfloat * m);
struct marshal_cmd_LoadMatrixd;
uint32_t _mesa_unmarshal_LoadMatrixd(struct gl_context *ctx, const struct marshal_cmd_LoadMatrixd *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_LoadMatrixd(const GLdouble * m);
struct marshal_cmd_MatrixMode;
uint32_t _mesa_unmarshal_MatrixMode(struct gl_context *ctx, const struct marshal_cmd_MatrixMode *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_MatrixMode(GLenum mode);
struct marshal_cmd_MultMatrixf;
uint32_t _mesa_unmarshal_MultMatrixf(struct gl_context *ctx, const struct marshal_cmd_MultMatrixf *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_MultMatrixf(const GLfloat * m);
struct marshal_cmd_MultMatrixd;
uint32_t _mesa_unmarshal_MultMatrixd(struct gl_context *ctx, const struct marshal_cmd_MultMatrixd *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_MultMatrixd(const GLdouble * m);
struct marshal_cmd_Ortho;
uint32_t _mesa_unmarshal_Ortho(struct gl_context *ctx, const struct marshal_cmd_Ortho *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Ortho(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);
struct marshal_cmd_PopMatrix;
uint32_t _mesa_unmarshal_PopMatrix(struct gl_context *ctx, const struct marshal_cmd_PopMatrix *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_PopMatrix(void);
struct marshal_cmd_PushMatrix;
uint32_t _mesa_unmarshal_PushMatrix(struct gl_context *ctx, const struct marshal_cmd_PushMatrix *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_PushMatrix(void);
struct marshal_cmd_Rotated;
uint32_t _mesa_unmarshal_Rotated(struct gl_context *ctx, const struct marshal_cmd_Rotated *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Rotated(GLdouble angle, GLdouble x, GLdouble y, GLdouble z);
struct marshal_cmd_Rotatef;
uint32_t _mesa_unmarshal_Rotatef(struct gl_context *ctx, const struct marshal_cmd_Rotatef *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Rotatef(GLfloat angle, GLfloat x, GLfloat y, GLfloat z);
struct marshal_cmd_Scaled;
uint32_t _mesa_unmarshal_Scaled(struct gl_context *ctx, const struct marshal_cmd_Scaled *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Scaled(GLdouble x, GLdouble y, GLdouble z);
struct marshal_cmd_Scalef;
uint32_t _mesa_unmarshal_Scalef(struct gl_context *ctx, const struct marshal_cmd_Scalef *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Scalef(GLfloat x, GLfloat y, GLfloat z);
struct marshal_cmd_Translated;
uint32_t _mesa_unmarshal_Translated(struct gl_context *ctx, const struct marshal_cmd_Translated *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Translated(GLdouble x, GLdouble y, GLdouble z);
struct marshal_cmd_Translatef;
uint32_t _mesa_unmarshal_Translatef(struct gl_context *ctx, const struct marshal_cmd_Translatef *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Translatef(GLfloat x, GLfloat y, GLfloat z);
struct marshal_cmd_Viewport;
uint32_t _mesa_unmarshal_Viewport(struct gl_context *ctx, const struct marshal_cmd_Viewport *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Viewport(GLint x, GLint y, GLsizei width, GLsizei height);
struct marshal_cmd_ArrayElement;
uint32_t _mesa_unmarshal_ArrayElement(struct gl_context *ctx, const struct marshal_cmd_ArrayElement *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ArrayElement(GLint i);
struct marshal_cmd_ColorPointer;
uint32_t _mesa_unmarshal_ColorPointer(struct gl_context *ctx, const struct marshal_cmd_ColorPointer *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ColorPointer(GLint size, GLenum type, GLsizei stride, const GLvoid * pointer);
struct marshal_cmd_DisableClientState;
uint32_t _mesa_unmarshal_DisableClientState(struct gl_context *ctx, const struct marshal_cmd_DisableClientState *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_DisableClientState(GLenum array);
struct marshal_cmd_DrawArrays;
uint32_t _mesa_unmarshal_DrawArrays(struct gl_context *ctx, const struct marshal_cmd_DrawArrays *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_DrawArrays(GLenum mode, GLint first, GLsizei count);
struct marshal_cmd_DrawElements;
uint32_t _mesa_unmarshal_DrawElements(struct gl_context *ctx, const struct marshal_cmd_DrawElements *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_DrawElements(GLenum mode, GLsizei count, GLenum type, const GLvoid * indices);
struct marshal_cmd_EdgeFlagPointer;
uint32_t _mesa_unmarshal_EdgeFlagPointer(struct gl_context *ctx, const struct marshal_cmd_EdgeFlagPointer *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_EdgeFlagPointer(GLsizei stride, const GLvoid * pointer);
struct marshal_cmd_EnableClientState;
uint32_t _mesa_unmarshal_EnableClientState(struct gl_context *ctx, const struct marshal_cmd_EnableClientState *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_EnableClientState(GLenum array);
void GLAPIENTRY _mesa_marshal_GetPointerv(GLenum pname, GLvoid ** params);
struct marshal_cmd_IndexPointer;
uint32_t _mesa_unmarshal_IndexPointer(struct gl_context *ctx, const struct marshal_cmd_IndexPointer *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_IndexPointer(GLenum type, GLsizei stride, const GLvoid * pointer);
struct marshal_cmd_InterleavedArrays;
uint32_t _mesa_unmarshal_InterleavedArrays(struct gl_context *ctx, const struct marshal_cmd_InterleavedArrays *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_InterleavedArrays(GLenum format, GLsizei stride, const GLvoid * pointer);
struct marshal_cmd_NormalPointer;
uint32_t _mesa_unmarshal_NormalPointer(struct gl_context *ctx, const struct marshal_cmd_NormalPointer *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_NormalPointer(GLenum type, GLsizei stride, const GLvoid * pointer);
struct marshal_cmd_TexCoordPointer;
uint32_t _mesa_unmarshal_TexCoordPointer(struct gl_context *ctx, const struct marshal_cmd_TexCoordPointer *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TexCoordPointer(GLint size, GLenum type, GLsizei stride, const GLvoid * pointer);
struct marshal_cmd_VertexPointer;
uint32_t _mesa_unmarshal_VertexPointer(struct gl_context *ctx, const struct marshal_cmd_VertexPointer *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexPointer(GLint size, GLenum type, GLsizei stride, const GLvoid * pointer);
struct marshal_cmd_PolygonOffset;
uint32_t _mesa_unmarshal_PolygonOffset(struct gl_context *ctx, const struct marshal_cmd_PolygonOffset *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_PolygonOffset(GLfloat factor, GLfloat units);
struct marshal_cmd_CopyTexImage1D;
uint32_t _mesa_unmarshal_CopyTexImage1D(struct gl_context *ctx, const struct marshal_cmd_CopyTexImage1D *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_CopyTexImage1D(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLint border);
struct marshal_cmd_CopyTexImage2D;
uint32_t _mesa_unmarshal_CopyTexImage2D(struct gl_context *ctx, const struct marshal_cmd_CopyTexImage2D *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_CopyTexImage2D(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
struct marshal_cmd_CopyTexSubImage1D;
uint32_t _mesa_unmarshal_CopyTexSubImage1D(struct gl_context *ctx, const struct marshal_cmd_CopyTexSubImage1D *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_CopyTexSubImage1D(GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width);
struct marshal_cmd_CopyTexSubImage2D;
uint32_t _mesa_unmarshal_CopyTexSubImage2D(struct gl_context *ctx, const struct marshal_cmd_CopyTexSubImage2D *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_CopyTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
struct marshal_cmd_TexSubImage1D;
uint32_t _mesa_unmarshal_TexSubImage1D(struct gl_context *ctx, const struct marshal_cmd_TexSubImage1D *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TexSubImage1D(GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const GLvoid * pixels);
struct marshal_cmd_TexSubImage2D;
uint32_t _mesa_unmarshal_TexSubImage2D(struct gl_context *ctx, const struct marshal_cmd_TexSubImage2D *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid * pixels);
GLboolean GLAPIENTRY _mesa_marshal_AreTexturesResident(GLsizei n, const GLuint * textures, GLboolean * residences);
struct marshal_cmd_BindTexture;
uint32_t _mesa_unmarshal_BindTexture(struct gl_context *ctx, const struct marshal_cmd_BindTexture *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_BindTexture(GLenum target, GLuint texture);
struct marshal_cmd_DeleteTextures;
uint32_t _mesa_unmarshal_DeleteTextures(struct gl_context *ctx, const struct marshal_cmd_DeleteTextures *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_DeleteTextures(GLsizei n, const GLuint * textures);
void GLAPIENTRY _mesa_marshal_GenTextures(GLsizei n, GLuint * textures);
GLboolean GLAPIENTRY _mesa_marshal_IsTexture(GLuint texture);
struct marshal_cmd_PrioritizeTextures;
uint32_t _mesa_unmarshal_PrioritizeTextures(struct gl_context *ctx, const struct marshal_cmd_PrioritizeTextures *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_PrioritizeTextures(GLsizei n, const GLuint * textures, const GLclampf * priorities);
struct marshal_cmd_Indexub;
uint32_t _mesa_unmarshal_Indexub(struct gl_context *ctx, const struct marshal_cmd_Indexub *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Indexub(GLubyte c);
struct marshal_cmd_Indexubv;
uint32_t _mesa_unmarshal_Indexubv(struct gl_context *ctx, const struct marshal_cmd_Indexubv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Indexubv(const GLubyte * c);
struct marshal_cmd_PopClientAttrib;
uint32_t _mesa_unmarshal_PopClientAttrib(struct gl_context *ctx, const struct marshal_cmd_PopClientAttrib *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_PopClientAttrib(void);
struct marshal_cmd_PushClientAttrib;
uint32_t _mesa_unmarshal_PushClientAttrib(struct gl_context *ctx, const struct marshal_cmd_PushClientAttrib *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_PushClientAttrib(GLbitfield mask);
struct marshal_cmd_BlendColor;
uint32_t _mesa_unmarshal_BlendColor(struct gl_context *ctx, const struct marshal_cmd_BlendColor *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_BlendColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
struct marshal_cmd_BlendEquation;
uint32_t _mesa_unmarshal_BlendEquation(struct gl_context *ctx, const struct marshal_cmd_BlendEquation *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_BlendEquation(GLenum mode);
struct marshal_cmd_DrawRangeElements;
uint32_t _mesa_unmarshal_DrawRangeElements(struct gl_context *ctx, const struct marshal_cmd_DrawRangeElements *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_DrawRangeElements(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const GLvoid * indices);
void GLAPIENTRY _mesa_marshal_ColorTable(GLenum target, GLenum internalformat, GLsizei width, GLenum format, GLenum type, const GLvoid * table);
void GLAPIENTRY _mesa_marshal_ColorTableParameterfv(GLenum target, GLenum pname, const GLfloat * params);
void GLAPIENTRY _mesa_marshal_ColorTableParameteriv(GLenum target, GLenum pname, const GLint * params);
struct marshal_cmd_CopyColorTable;
uint32_t _mesa_unmarshal_CopyColorTable(struct gl_context *ctx, const struct marshal_cmd_CopyColorTable *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_CopyColorTable(GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width);
void GLAPIENTRY _mesa_marshal_GetColorTable(GLenum target, GLenum format, GLenum type, GLvoid * table);
void GLAPIENTRY _mesa_marshal_GetColorTableParameterfv(GLenum target, GLenum pname, GLfloat * params);
void GLAPIENTRY _mesa_marshal_GetColorTableParameteriv(GLenum target, GLenum pname, GLint * params);
void GLAPIENTRY _mesa_marshal_ColorSubTable(GLenum target, GLsizei start, GLsizei count, GLenum format, GLenum type, const GLvoid * data);
struct marshal_cmd_CopyColorSubTable;
uint32_t _mesa_unmarshal_CopyColorSubTable(struct gl_context *ctx, const struct marshal_cmd_CopyColorSubTable *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_CopyColorSubTable(GLenum target, GLsizei start, GLint x, GLint y, GLsizei width);
void GLAPIENTRY _mesa_marshal_ConvolutionFilter1D(GLenum target, GLenum internalformat, GLsizei width, GLenum format, GLenum type, const GLvoid * image);
void GLAPIENTRY _mesa_marshal_ConvolutionFilter2D(GLenum target, GLenum internalformat, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid * image);
struct marshal_cmd_ConvolutionParameterf;
uint32_t _mesa_unmarshal_ConvolutionParameterf(struct gl_context *ctx, const struct marshal_cmd_ConvolutionParameterf *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ConvolutionParameterf(GLenum target, GLenum pname, GLfloat params);
void GLAPIENTRY _mesa_marshal_ConvolutionParameterfv(GLenum target, GLenum pname, const GLfloat * params);
struct marshal_cmd_ConvolutionParameteri;
uint32_t _mesa_unmarshal_ConvolutionParameteri(struct gl_context *ctx, const struct marshal_cmd_ConvolutionParameteri *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ConvolutionParameteri(GLenum target, GLenum pname, GLint params);
void GLAPIENTRY _mesa_marshal_ConvolutionParameteriv(GLenum target, GLenum pname, const GLint * params);
struct marshal_cmd_CopyConvolutionFilter1D;
uint32_t _mesa_unmarshal_CopyConvolutionFilter1D(struct gl_context *ctx, const struct marshal_cmd_CopyConvolutionFilter1D *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_CopyConvolutionFilter1D(GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width);
struct marshal_cmd_CopyConvolutionFilter2D;
uint32_t _mesa_unmarshal_CopyConvolutionFilter2D(struct gl_context *ctx, const struct marshal_cmd_CopyConvolutionFilter2D *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_CopyConvolutionFilter2D(GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height);
void GLAPIENTRY _mesa_marshal_GetConvolutionFilter(GLenum target, GLenum format, GLenum type, GLvoid * image);
void GLAPIENTRY _mesa_marshal_GetConvolutionParameterfv(GLenum target, GLenum pname, GLfloat * params);
void GLAPIENTRY _mesa_marshal_GetConvolutionParameteriv(GLenum target, GLenum pname, GLint * params);
void GLAPIENTRY _mesa_marshal_GetSeparableFilter(GLenum target, GLenum format, GLenum type, GLvoid * row, GLvoid * column, GLvoid * span);
void GLAPIENTRY _mesa_marshal_SeparableFilter2D(GLenum target, GLenum internalformat, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid * row, const GLvoid * column);
void GLAPIENTRY _mesa_marshal_GetHistogram(GLenum target, GLboolean reset, GLenum format, GLenum type, GLvoid * values);
void GLAPIENTRY _mesa_marshal_GetHistogramParameterfv(GLenum target, GLenum pname, GLfloat * params);
void GLAPIENTRY _mesa_marshal_GetHistogramParameteriv(GLenum target, GLenum pname, GLint * params);
void GLAPIENTRY _mesa_marshal_GetMinmax(GLenum target, GLboolean reset, GLenum format, GLenum type, GLvoid * values);
void GLAPIENTRY _mesa_marshal_GetMinmaxParameterfv(GLenum target, GLenum pname, GLfloat * params);
void GLAPIENTRY _mesa_marshal_GetMinmaxParameteriv(GLenum target, GLenum pname, GLint * params);
struct marshal_cmd_Histogram;
uint32_t _mesa_unmarshal_Histogram(struct gl_context *ctx, const struct marshal_cmd_Histogram *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Histogram(GLenum target, GLsizei width, GLenum internalformat, GLboolean sink);
struct marshal_cmd_Minmax;
uint32_t _mesa_unmarshal_Minmax(struct gl_context *ctx, const struct marshal_cmd_Minmax *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Minmax(GLenum target, GLenum internalformat, GLboolean sink);
struct marshal_cmd_ResetHistogram;
uint32_t _mesa_unmarshal_ResetHistogram(struct gl_context *ctx, const struct marshal_cmd_ResetHistogram *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ResetHistogram(GLenum target);
struct marshal_cmd_ResetMinmax;
uint32_t _mesa_unmarshal_ResetMinmax(struct gl_context *ctx, const struct marshal_cmd_ResetMinmax *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ResetMinmax(GLenum target);
struct marshal_cmd_TexImage3D;
uint32_t _mesa_unmarshal_TexImage3D(struct gl_context *ctx, const struct marshal_cmd_TexImage3D *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TexImage3D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid * pixels);
struct marshal_cmd_TexSubImage3D;
uint32_t _mesa_unmarshal_TexSubImage3D(struct gl_context *ctx, const struct marshal_cmd_TexSubImage3D *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TexSubImage3D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const GLvoid * pixels);
struct marshal_cmd_CopyTexSubImage3D;
uint32_t _mesa_unmarshal_CopyTexSubImage3D(struct gl_context *ctx, const struct marshal_cmd_CopyTexSubImage3D *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_CopyTexSubImage3D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);
struct marshal_cmd_ActiveTexture;
uint32_t _mesa_unmarshal_ActiveTexture(struct gl_context *ctx, const struct marshal_cmd_ActiveTexture *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ActiveTexture(GLenum texture);
struct marshal_cmd_ClientActiveTexture;
uint32_t _mesa_unmarshal_ClientActiveTexture(struct gl_context *ctx, const struct marshal_cmd_ClientActiveTexture *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ClientActiveTexture(GLenum texture);
struct marshal_cmd_MultiTexCoord1d;
uint32_t _mesa_unmarshal_MultiTexCoord1d(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord1d *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_MultiTexCoord1d(GLenum target, GLdouble s);
struct marshal_cmd_MultiTexCoord1dv;
uint32_t _mesa_unmarshal_MultiTexCoord1dv(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord1dv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_MultiTexCoord1dv(GLenum target, const GLdouble * v);
struct marshal_cmd_MultiTexCoord1fARB;
uint32_t _mesa_unmarshal_MultiTexCoord1fARB(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord1fARB *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_MultiTexCoord1fARB(GLenum target, GLfloat s);
struct marshal_cmd_MultiTexCoord1fvARB;
uint32_t _mesa_unmarshal_MultiTexCoord1fvARB(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord1fvARB *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_MultiTexCoord1fvARB(GLenum target, const GLfloat * v);
struct marshal_cmd_MultiTexCoord1i;
uint32_t _mesa_unmarshal_MultiTexCoord1i(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord1i *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_MultiTexCoord1i(GLenum target, GLint s);
struct marshal_cmd_MultiTexCoord1iv;
uint32_t _mesa_unmarshal_MultiTexCoord1iv(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord1iv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_MultiTexCoord1iv(GLenum target, const GLint * v);
struct marshal_cmd_MultiTexCoord1s;
uint32_t _mesa_unmarshal_MultiTexCoord1s(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord1s *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_MultiTexCoord1s(GLenum target, GLshort s);
struct marshal_cmd_MultiTexCoord1sv;
uint32_t _mesa_unmarshal_MultiTexCoord1sv(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord1sv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_MultiTexCoord1sv(GLenum target, const GLshort * v);
struct marshal_cmd_MultiTexCoord2d;
uint32_t _mesa_unmarshal_MultiTexCoord2d(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord2d *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_MultiTexCoord2d(GLenum target, GLdouble s, GLdouble t);
struct marshal_cmd_MultiTexCoord2dv;
uint32_t _mesa_unmarshal_MultiTexCoord2dv(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord2dv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_MultiTexCoord2dv(GLenum target, const GLdouble * v);
struct marshal_cmd_MultiTexCoord2fARB;
uint32_t _mesa_unmarshal_MultiTexCoord2fARB(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord2fARB *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_MultiTexCoord2fARB(GLenum target, GLfloat s, GLfloat t);
struct marshal_cmd_MultiTexCoord2fvARB;
uint32_t _mesa_unmarshal_MultiTexCoord2fvARB(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord2fvARB *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_MultiTexCoord2fvARB(GLenum target, const GLfloat * v);
struct marshal_cmd_MultiTexCoord2i;
uint32_t _mesa_unmarshal_MultiTexCoord2i(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord2i *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_MultiTexCoord2i(GLenum target, GLint s, GLint t);
struct marshal_cmd_MultiTexCoord2iv;
uint32_t _mesa_unmarshal_MultiTexCoord2iv(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord2iv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_MultiTexCoord2iv(GLenum target, const GLint * v);
struct marshal_cmd_MultiTexCoord2s;
uint32_t _mesa_unmarshal_MultiTexCoord2s(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord2s *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_MultiTexCoord2s(GLenum target, GLshort s, GLshort t);
struct marshal_cmd_MultiTexCoord2sv;
uint32_t _mesa_unmarshal_MultiTexCoord2sv(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord2sv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_MultiTexCoord2sv(GLenum target, const GLshort * v);
struct marshal_cmd_MultiTexCoord3d;
uint32_t _mesa_unmarshal_MultiTexCoord3d(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord3d *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_MultiTexCoord3d(GLenum target, GLdouble s, GLdouble t, GLdouble r);
struct marshal_cmd_MultiTexCoord3dv;
uint32_t _mesa_unmarshal_MultiTexCoord3dv(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord3dv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_MultiTexCoord3dv(GLenum target, const GLdouble * v);
struct marshal_cmd_MultiTexCoord3fARB;
uint32_t _mesa_unmarshal_MultiTexCoord3fARB(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord3fARB *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_MultiTexCoord3fARB(GLenum target, GLfloat s, GLfloat t, GLfloat r);
struct marshal_cmd_MultiTexCoord3fvARB;
uint32_t _mesa_unmarshal_MultiTexCoord3fvARB(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord3fvARB *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_MultiTexCoord3fvARB(GLenum target, const GLfloat * v);
struct marshal_cmd_MultiTexCoord3i;
uint32_t _mesa_unmarshal_MultiTexCoord3i(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord3i *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_MultiTexCoord3i(GLenum target, GLint s, GLint t, GLint r);
struct marshal_cmd_MultiTexCoord3iv;
uint32_t _mesa_unmarshal_MultiTexCoord3iv(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord3iv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_MultiTexCoord3iv(GLenum target, const GLint * v);
struct marshal_cmd_MultiTexCoord3s;
uint32_t _mesa_unmarshal_MultiTexCoord3s(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord3s *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_MultiTexCoord3s(GLenum target, GLshort s, GLshort t, GLshort r);
struct marshal_cmd_MultiTexCoord3sv;
uint32_t _mesa_unmarshal_MultiTexCoord3sv(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord3sv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_MultiTexCoord3sv(GLenum target, const GLshort * v);
struct marshal_cmd_MultiTexCoord4d;
uint32_t _mesa_unmarshal_MultiTexCoord4d(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord4d *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_MultiTexCoord4d(GLenum target, GLdouble s, GLdouble t, GLdouble r, GLdouble q);
struct marshal_cmd_MultiTexCoord4dv;
uint32_t _mesa_unmarshal_MultiTexCoord4dv(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord4dv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_MultiTexCoord4dv(GLenum target, const GLdouble * v);
struct marshal_cmd_MultiTexCoord4fARB;
uint32_t _mesa_unmarshal_MultiTexCoord4fARB(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord4fARB *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_MultiTexCoord4fARB(GLenum target, GLfloat s, GLfloat t, GLfloat r, GLfloat q);
struct marshal_cmd_MultiTexCoord4fvARB;
uint32_t _mesa_unmarshal_MultiTexCoord4fvARB(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord4fvARB *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_MultiTexCoord4fvARB(GLenum target, const GLfloat * v);
struct marshal_cmd_MultiTexCoord4i;
uint32_t _mesa_unmarshal_MultiTexCoord4i(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord4i *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_MultiTexCoord4i(GLenum target, GLint s, GLint t, GLint r, GLint q);
struct marshal_cmd_MultiTexCoord4iv;
uint32_t _mesa_unmarshal_MultiTexCoord4iv(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord4iv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_MultiTexCoord4iv(GLenum target, const GLint * v);
struct marshal_cmd_MultiTexCoord4s;
uint32_t _mesa_unmarshal_MultiTexCoord4s(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord4s *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_MultiTexCoord4s(GLenum target, GLshort s, GLshort t, GLshort r, GLshort q);
struct marshal_cmd_MultiTexCoord4sv;
uint32_t _mesa_unmarshal_MultiTexCoord4sv(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord4sv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_MultiTexCoord4sv(GLenum target, const GLshort * v);
struct marshal_cmd_LoadTransposeMatrixf;
uint32_t _mesa_unmarshal_LoadTransposeMatrixf(struct gl_context *ctx, const struct marshal_cmd_LoadTransposeMatrixf *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_LoadTransposeMatrixf(const GLfloat * m);
struct marshal_cmd_LoadTransposeMatrixd;
uint32_t _mesa_unmarshal_LoadTransposeMatrixd(struct gl_context *ctx, const struct marshal_cmd_LoadTransposeMatrixd *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_LoadTransposeMatrixd(const GLdouble * m);
struct marshal_cmd_MultTransposeMatrixf;
uint32_t _mesa_unmarshal_MultTransposeMatrixf(struct gl_context *ctx, const struct marshal_cmd_MultTransposeMatrixf *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_MultTransposeMatrixf(const GLfloat * m);
struct marshal_cmd_MultTransposeMatrixd;
uint32_t _mesa_unmarshal_MultTransposeMatrixd(struct gl_context *ctx, const struct marshal_cmd_MultTransposeMatrixd *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_MultTransposeMatrixd(const GLdouble * m);
struct marshal_cmd_SampleCoverage;
uint32_t _mesa_unmarshal_SampleCoverage(struct gl_context *ctx, const struct marshal_cmd_SampleCoverage *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_SampleCoverage(GLclampf value, GLboolean invert);
struct marshal_cmd_CompressedTexImage3D;
uint32_t _mesa_unmarshal_CompressedTexImage3D(struct gl_context *ctx, const struct marshal_cmd_CompressedTexImage3D *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_CompressedTexImage3D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const GLvoid * data);
struct marshal_cmd_CompressedTexImage2D;
uint32_t _mesa_unmarshal_CompressedTexImage2D(struct gl_context *ctx, const struct marshal_cmd_CompressedTexImage2D *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_CompressedTexImage2D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid * data);
struct marshal_cmd_CompressedTexImage1D;
uint32_t _mesa_unmarshal_CompressedTexImage1D(struct gl_context *ctx, const struct marshal_cmd_CompressedTexImage1D *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_CompressedTexImage1D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei imageSize, const GLvoid * data);
struct marshal_cmd_CompressedTexSubImage3D;
uint32_t _mesa_unmarshal_CompressedTexSubImage3D(struct gl_context *ctx, const struct marshal_cmd_CompressedTexSubImage3D *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_CompressedTexSubImage3D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const GLvoid * data);
struct marshal_cmd_CompressedTexSubImage2D;
uint32_t _mesa_unmarshal_CompressedTexSubImage2D(struct gl_context *ctx, const struct marshal_cmd_CompressedTexSubImage2D *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_CompressedTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const GLvoid * data);
struct marshal_cmd_CompressedTexSubImage1D;
uint32_t _mesa_unmarshal_CompressedTexSubImage1D(struct gl_context *ctx, const struct marshal_cmd_CompressedTexSubImage1D *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_CompressedTexSubImage1D(GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const GLvoid * data);
struct marshal_cmd_GetCompressedTexImage;
uint32_t _mesa_unmarshal_GetCompressedTexImage(struct gl_context *ctx, const struct marshal_cmd_GetCompressedTexImage *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_GetCompressedTexImage(GLenum target, GLint level, GLvoid * img);
struct marshal_cmd_BlendFuncSeparate;
uint32_t _mesa_unmarshal_BlendFuncSeparate(struct gl_context *ctx, const struct marshal_cmd_BlendFuncSeparate *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_BlendFuncSeparate(GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha);
struct marshal_cmd_FogCoordfEXT;
uint32_t _mesa_unmarshal_FogCoordfEXT(struct gl_context *ctx, const struct marshal_cmd_FogCoordfEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_FogCoordfEXT(GLfloat coord);
struct marshal_cmd_FogCoordfvEXT;
uint32_t _mesa_unmarshal_FogCoordfvEXT(struct gl_context *ctx, const struct marshal_cmd_FogCoordfvEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_FogCoordfvEXT(const GLfloat * coord);
struct marshal_cmd_FogCoordd;
uint32_t _mesa_unmarshal_FogCoordd(struct gl_context *ctx, const struct marshal_cmd_FogCoordd *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_FogCoordd(GLdouble coord);
struct marshal_cmd_FogCoorddv;
uint32_t _mesa_unmarshal_FogCoorddv(struct gl_context *ctx, const struct marshal_cmd_FogCoorddv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_FogCoorddv(const GLdouble * coord);
struct marshal_cmd_FogCoordPointer;
uint32_t _mesa_unmarshal_FogCoordPointer(struct gl_context *ctx, const struct marshal_cmd_FogCoordPointer *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_FogCoordPointer(GLenum type, GLsizei stride, const GLvoid * pointer);
struct marshal_cmd_MultiDrawArrays;
uint32_t _mesa_unmarshal_MultiDrawArrays(struct gl_context *ctx, const struct marshal_cmd_MultiDrawArrays *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_MultiDrawArrays(GLenum mode, const GLint * first, const GLsizei * count, GLsizei primcount);
struct marshal_cmd_MultiDrawElementsEXT;
uint32_t _mesa_unmarshal_MultiDrawElementsEXT(struct gl_context *ctx, const struct marshal_cmd_MultiDrawElementsEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_MultiDrawElementsEXT(GLenum mode, const GLsizei * count, GLenum type, const GLvoid * const * indices, GLsizei primcount);
struct marshal_cmd_PointParameterf;
uint32_t _mesa_unmarshal_PointParameterf(struct gl_context *ctx, const struct marshal_cmd_PointParameterf *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_PointParameterf(GLenum pname, GLfloat param);
struct marshal_cmd_PointParameterfv;
uint32_t _mesa_unmarshal_PointParameterfv(struct gl_context *ctx, const struct marshal_cmd_PointParameterfv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_PointParameterfv(GLenum pname, const GLfloat * params);
struct marshal_cmd_PointParameteri;
uint32_t _mesa_unmarshal_PointParameteri(struct gl_context *ctx, const struct marshal_cmd_PointParameteri *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_PointParameteri(GLenum pname, GLint param);
struct marshal_cmd_PointParameteriv;
uint32_t _mesa_unmarshal_PointParameteriv(struct gl_context *ctx, const struct marshal_cmd_PointParameteriv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_PointParameteriv(GLenum pname, const GLint * params);
struct marshal_cmd_SecondaryColor3b;
uint32_t _mesa_unmarshal_SecondaryColor3b(struct gl_context *ctx, const struct marshal_cmd_SecondaryColor3b *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_SecondaryColor3b(GLbyte red, GLbyte green, GLbyte blue);
struct marshal_cmd_SecondaryColor3bv;
uint32_t _mesa_unmarshal_SecondaryColor3bv(struct gl_context *ctx, const struct marshal_cmd_SecondaryColor3bv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_SecondaryColor3bv(const GLbyte * v);
struct marshal_cmd_SecondaryColor3d;
uint32_t _mesa_unmarshal_SecondaryColor3d(struct gl_context *ctx, const struct marshal_cmd_SecondaryColor3d *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_SecondaryColor3d(GLdouble red, GLdouble green, GLdouble blue);
struct marshal_cmd_SecondaryColor3dv;
uint32_t _mesa_unmarshal_SecondaryColor3dv(struct gl_context *ctx, const struct marshal_cmd_SecondaryColor3dv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_SecondaryColor3dv(const GLdouble * v);
struct marshal_cmd_SecondaryColor3fEXT;
uint32_t _mesa_unmarshal_SecondaryColor3fEXT(struct gl_context *ctx, const struct marshal_cmd_SecondaryColor3fEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_SecondaryColor3fEXT(GLfloat red, GLfloat green, GLfloat blue);
struct marshal_cmd_SecondaryColor3fvEXT;
uint32_t _mesa_unmarshal_SecondaryColor3fvEXT(struct gl_context *ctx, const struct marshal_cmd_SecondaryColor3fvEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_SecondaryColor3fvEXT(const GLfloat * v);
struct marshal_cmd_SecondaryColor3i;
uint32_t _mesa_unmarshal_SecondaryColor3i(struct gl_context *ctx, const struct marshal_cmd_SecondaryColor3i *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_SecondaryColor3i(GLint red, GLint green, GLint blue);
struct marshal_cmd_SecondaryColor3iv;
uint32_t _mesa_unmarshal_SecondaryColor3iv(struct gl_context *ctx, const struct marshal_cmd_SecondaryColor3iv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_SecondaryColor3iv(const GLint * v);
struct marshal_cmd_SecondaryColor3s;
uint32_t _mesa_unmarshal_SecondaryColor3s(struct gl_context *ctx, const struct marshal_cmd_SecondaryColor3s *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_SecondaryColor3s(GLshort red, GLshort green, GLshort blue);
struct marshal_cmd_SecondaryColor3sv;
uint32_t _mesa_unmarshal_SecondaryColor3sv(struct gl_context *ctx, const struct marshal_cmd_SecondaryColor3sv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_SecondaryColor3sv(const GLshort * v);
struct marshal_cmd_SecondaryColor3ub;
uint32_t _mesa_unmarshal_SecondaryColor3ub(struct gl_context *ctx, const struct marshal_cmd_SecondaryColor3ub *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_SecondaryColor3ub(GLubyte red, GLubyte green, GLubyte blue);
struct marshal_cmd_SecondaryColor3ubv;
uint32_t _mesa_unmarshal_SecondaryColor3ubv(struct gl_context *ctx, const struct marshal_cmd_SecondaryColor3ubv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_SecondaryColor3ubv(const GLubyte * v);
struct marshal_cmd_SecondaryColor3ui;
uint32_t _mesa_unmarshal_SecondaryColor3ui(struct gl_context *ctx, const struct marshal_cmd_SecondaryColor3ui *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_SecondaryColor3ui(GLuint red, GLuint green, GLuint blue);
struct marshal_cmd_SecondaryColor3uiv;
uint32_t _mesa_unmarshal_SecondaryColor3uiv(struct gl_context *ctx, const struct marshal_cmd_SecondaryColor3uiv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_SecondaryColor3uiv(const GLuint * v);
struct marshal_cmd_SecondaryColor3us;
uint32_t _mesa_unmarshal_SecondaryColor3us(struct gl_context *ctx, const struct marshal_cmd_SecondaryColor3us *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_SecondaryColor3us(GLushort red, GLushort green, GLushort blue);
struct marshal_cmd_SecondaryColor3usv;
uint32_t _mesa_unmarshal_SecondaryColor3usv(struct gl_context *ctx, const struct marshal_cmd_SecondaryColor3usv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_SecondaryColor3usv(const GLushort * v);
struct marshal_cmd_SecondaryColorPointer;
uint32_t _mesa_unmarshal_SecondaryColorPointer(struct gl_context *ctx, const struct marshal_cmd_SecondaryColorPointer *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_SecondaryColorPointer(GLint size, GLenum type, GLsizei stride, const GLvoid * pointer);
struct marshal_cmd_WindowPos2d;
uint32_t _mesa_unmarshal_WindowPos2d(struct gl_context *ctx, const struct marshal_cmd_WindowPos2d *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_WindowPos2d(GLdouble x, GLdouble y);
struct marshal_cmd_WindowPos2dv;
uint32_t _mesa_unmarshal_WindowPos2dv(struct gl_context *ctx, const struct marshal_cmd_WindowPos2dv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_WindowPos2dv(const GLdouble * v);
struct marshal_cmd_WindowPos2f;
uint32_t _mesa_unmarshal_WindowPos2f(struct gl_context *ctx, const struct marshal_cmd_WindowPos2f *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_WindowPos2f(GLfloat x, GLfloat y);
struct marshal_cmd_WindowPos2fv;
uint32_t _mesa_unmarshal_WindowPos2fv(struct gl_context *ctx, const struct marshal_cmd_WindowPos2fv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_WindowPos2fv(const GLfloat * v);
struct marshal_cmd_WindowPos2i;
uint32_t _mesa_unmarshal_WindowPos2i(struct gl_context *ctx, const struct marshal_cmd_WindowPos2i *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_WindowPos2i(GLint x, GLint y);
struct marshal_cmd_WindowPos2iv;
uint32_t _mesa_unmarshal_WindowPos2iv(struct gl_context *ctx, const struct marshal_cmd_WindowPos2iv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_WindowPos2iv(const GLint * v);
struct marshal_cmd_WindowPos2s;
uint32_t _mesa_unmarshal_WindowPos2s(struct gl_context *ctx, const struct marshal_cmd_WindowPos2s *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_WindowPos2s(GLshort x, GLshort y);
struct marshal_cmd_WindowPos2sv;
uint32_t _mesa_unmarshal_WindowPos2sv(struct gl_context *ctx, const struct marshal_cmd_WindowPos2sv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_WindowPos2sv(const GLshort * v);
struct marshal_cmd_WindowPos3d;
uint32_t _mesa_unmarshal_WindowPos3d(struct gl_context *ctx, const struct marshal_cmd_WindowPos3d *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_WindowPos3d(GLdouble x, GLdouble y, GLdouble z);
struct marshal_cmd_WindowPos3dv;
uint32_t _mesa_unmarshal_WindowPos3dv(struct gl_context *ctx, const struct marshal_cmd_WindowPos3dv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_WindowPos3dv(const GLdouble * v);
struct marshal_cmd_WindowPos3f;
uint32_t _mesa_unmarshal_WindowPos3f(struct gl_context *ctx, const struct marshal_cmd_WindowPos3f *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_WindowPos3f(GLfloat x, GLfloat y, GLfloat z);
struct marshal_cmd_WindowPos3fv;
uint32_t _mesa_unmarshal_WindowPos3fv(struct gl_context *ctx, const struct marshal_cmd_WindowPos3fv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_WindowPos3fv(const GLfloat * v);
struct marshal_cmd_WindowPos3i;
uint32_t _mesa_unmarshal_WindowPos3i(struct gl_context *ctx, const struct marshal_cmd_WindowPos3i *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_WindowPos3i(GLint x, GLint y, GLint z);
struct marshal_cmd_WindowPos3iv;
uint32_t _mesa_unmarshal_WindowPos3iv(struct gl_context *ctx, const struct marshal_cmd_WindowPos3iv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_WindowPos3iv(const GLint * v);
struct marshal_cmd_WindowPos3s;
uint32_t _mesa_unmarshal_WindowPos3s(struct gl_context *ctx, const struct marshal_cmd_WindowPos3s *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_WindowPos3s(GLshort x, GLshort y, GLshort z);
struct marshal_cmd_WindowPos3sv;
uint32_t _mesa_unmarshal_WindowPos3sv(struct gl_context *ctx, const struct marshal_cmd_WindowPos3sv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_WindowPos3sv(const GLshort * v);
struct marshal_cmd_BindBuffer;
uint32_t _mesa_unmarshal_BindBuffer(struct gl_context *ctx, const struct marshal_cmd_BindBuffer *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_BindBuffer(GLenum target, GLuint buffer);
struct marshal_cmd_BufferData;
uint32_t _mesa_unmarshal_BufferData(struct gl_context *ctx, const struct marshal_cmd_BufferData *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_BufferData(GLenum target, GLsizeiptr size, const GLvoid * data, GLenum usage);
struct marshal_cmd_BufferSubData;
uint32_t _mesa_unmarshal_BufferSubData(struct gl_context *ctx, const struct marshal_cmd_BufferSubData *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_BufferSubData(GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid * data);
struct marshal_cmd_DeleteBuffers;
uint32_t _mesa_unmarshal_DeleteBuffers(struct gl_context *ctx, const struct marshal_cmd_DeleteBuffers *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_DeleteBuffers(GLsizei n, const GLuint * buffer);
void GLAPIENTRY _mesa_marshal_GenBuffers(GLsizei n, GLuint * buffer);
void GLAPIENTRY _mesa_marshal_GetBufferParameteriv(GLenum target, GLenum pname, GLint * params);
void GLAPIENTRY _mesa_marshal_GetBufferPointerv(GLenum target, GLenum pname, GLvoid ** params);
void GLAPIENTRY _mesa_marshal_GetBufferSubData(GLenum target, GLintptr offset, GLsizeiptr size, GLvoid * data);
GLboolean GLAPIENTRY _mesa_marshal_IsBuffer(GLuint buffer);
GLvoid * GLAPIENTRY _mesa_marshal_MapBuffer(GLenum target, GLenum access);
struct marshal_cmd_UnmapBuffer;
uint32_t _mesa_unmarshal_UnmapBuffer(struct gl_context *ctx, const struct marshal_cmd_UnmapBuffer *cmd, const uint64_t *last);
GLboolean GLAPIENTRY _mesa_marshal_UnmapBuffer(GLenum target);
void GLAPIENTRY _mesa_marshal_GenQueries(GLsizei n, GLuint * ids);
struct marshal_cmd_DeleteQueries;
uint32_t _mesa_unmarshal_DeleteQueries(struct gl_context *ctx, const struct marshal_cmd_DeleteQueries *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_DeleteQueries(GLsizei n, const GLuint * ids);
GLboolean GLAPIENTRY _mesa_marshal_IsQuery(GLuint id);
struct marshal_cmd_BeginQuery;
uint32_t _mesa_unmarshal_BeginQuery(struct gl_context *ctx, const struct marshal_cmd_BeginQuery *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_BeginQuery(GLenum target, GLuint id);
struct marshal_cmd_EndQuery;
uint32_t _mesa_unmarshal_EndQuery(struct gl_context *ctx, const struct marshal_cmd_EndQuery *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_EndQuery(GLenum target);
void GLAPIENTRY _mesa_marshal_GetQueryiv(GLenum target, GLenum pname, GLint * params);
void GLAPIENTRY _mesa_marshal_GetQueryObjectiv(GLuint id, GLenum pname, GLint * params);
void GLAPIENTRY _mesa_marshal_GetQueryObjectuiv(GLuint id, GLenum pname, GLuint * params);
struct marshal_cmd_BlendEquationSeparate;
uint32_t _mesa_unmarshal_BlendEquationSeparate(struct gl_context *ctx, const struct marshal_cmd_BlendEquationSeparate *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_BlendEquationSeparate(GLenum modeRGB, GLenum modeA);
struct marshal_cmd_DrawBuffers;
uint32_t _mesa_unmarshal_DrawBuffers(struct gl_context *ctx, const struct marshal_cmd_DrawBuffers *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_DrawBuffers(GLsizei n, const GLenum * bufs);
struct marshal_cmd_StencilFuncSeparate;
uint32_t _mesa_unmarshal_StencilFuncSeparate(struct gl_context *ctx, const struct marshal_cmd_StencilFuncSeparate *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_StencilFuncSeparate(GLenum face, GLenum func, GLint ref, GLuint mask);
struct marshal_cmd_StencilOpSeparate;
uint32_t _mesa_unmarshal_StencilOpSeparate(struct gl_context *ctx, const struct marshal_cmd_StencilOpSeparate *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_StencilOpSeparate(GLenum face, GLenum sfail, GLenum zfail, GLenum zpass);
struct marshal_cmd_StencilMaskSeparate;
uint32_t _mesa_unmarshal_StencilMaskSeparate(struct gl_context *ctx, const struct marshal_cmd_StencilMaskSeparate *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_StencilMaskSeparate(GLenum face, GLuint mask);
struct marshal_cmd_AttachShader;
uint32_t _mesa_unmarshal_AttachShader(struct gl_context *ctx, const struct marshal_cmd_AttachShader *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_AttachShader(GLuint program, GLuint shader);
struct marshal_cmd_BindAttribLocation;
uint32_t _mesa_unmarshal_BindAttribLocation(struct gl_context *ctx, const struct marshal_cmd_BindAttribLocation *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_BindAttribLocation(GLuint program, GLuint index, const GLchar * name);
struct marshal_cmd_CompileShader;
uint32_t _mesa_unmarshal_CompileShader(struct gl_context *ctx, const struct marshal_cmd_CompileShader *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_CompileShader(GLuint shader);
GLuint GLAPIENTRY _mesa_marshal_CreateProgram(void);
GLuint GLAPIENTRY _mesa_marshal_CreateShader(GLenum type);
struct marshal_cmd_DeleteProgram;
uint32_t _mesa_unmarshal_DeleteProgram(struct gl_context *ctx, const struct marshal_cmd_DeleteProgram *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_DeleteProgram(GLuint program);
struct marshal_cmd_DeleteShader;
uint32_t _mesa_unmarshal_DeleteShader(struct gl_context *ctx, const struct marshal_cmd_DeleteShader *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_DeleteShader(GLuint program);
struct marshal_cmd_DetachShader;
uint32_t _mesa_unmarshal_DetachShader(struct gl_context *ctx, const struct marshal_cmd_DetachShader *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_DetachShader(GLuint program, GLuint shader);
struct marshal_cmd_DisableVertexAttribArray;
uint32_t _mesa_unmarshal_DisableVertexAttribArray(struct gl_context *ctx, const struct marshal_cmd_DisableVertexAttribArray *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_DisableVertexAttribArray(GLuint index);
struct marshal_cmd_EnableVertexAttribArray;
uint32_t _mesa_unmarshal_EnableVertexAttribArray(struct gl_context *ctx, const struct marshal_cmd_EnableVertexAttribArray *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_EnableVertexAttribArray(GLuint index);
void GLAPIENTRY _mesa_marshal_GetActiveAttrib(GLuint program, GLuint index, GLsizei  bufSize, GLsizei * length, GLint * size, GLenum * type, GLchar * name);
struct marshal_cmd_GetActiveUniform;
uint32_t _mesa_unmarshal_GetActiveUniform(struct gl_context *ctx, const struct marshal_cmd_GetActiveUniform *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_GetActiveUniform(GLuint program, GLuint index, GLsizei bufSize, GLsizei * length, GLint * size, GLenum * type, GLchar * name);
void GLAPIENTRY _mesa_marshal_GetAttachedShaders(GLuint program, GLsizei maxCount, GLsizei * count, GLuint * obj);
GLint GLAPIENTRY _mesa_marshal_GetAttribLocation(GLuint program, const GLchar * name);
void GLAPIENTRY _mesa_marshal_GetProgramiv(GLuint program, GLenum pname, GLint * params);
void GLAPIENTRY _mesa_marshal_GetProgramInfoLog(GLuint program, GLsizei bufSize, GLsizei * length, GLchar * infoLog);
void GLAPIENTRY _mesa_marshal_GetShaderiv(GLuint shader, GLenum pname, GLint * params);
void GLAPIENTRY _mesa_marshal_GetShaderInfoLog(GLuint shader, GLsizei bufSize, GLsizei * length, GLchar * infoLog);
void GLAPIENTRY _mesa_marshal_GetShaderSource(GLuint shader, GLsizei bufSize, GLsizei * length, GLchar * source);
struct marshal_cmd_GetUniformLocation;
uint32_t _mesa_unmarshal_GetUniformLocation(struct gl_context *ctx, const struct marshal_cmd_GetUniformLocation *cmd, const uint64_t *last);
GLint GLAPIENTRY _mesa_marshal_GetUniformLocation(GLuint program, const GLchar * name);
void GLAPIENTRY _mesa_marshal_GetUniformfv(GLuint program, GLint location, GLfloat * params);
void GLAPIENTRY _mesa_marshal_GetUniformiv(GLuint program, GLint location, GLint * params);
void GLAPIENTRY _mesa_marshal_GetVertexAttribdv(GLuint index, GLenum pname, GLdouble * params);
void GLAPIENTRY _mesa_marshal_GetVertexAttribfv(GLuint index, GLenum pname, GLfloat * params);
void GLAPIENTRY _mesa_marshal_GetVertexAttribiv(GLuint index, GLenum pname, GLint * params);
void GLAPIENTRY _mesa_marshal_GetVertexAttribPointerv(GLuint index, GLenum pname, GLvoid ** pointer);
GLboolean GLAPIENTRY _mesa_marshal_IsProgram(GLuint program);
GLboolean GLAPIENTRY _mesa_marshal_IsShader(GLuint shader);
struct marshal_cmd_LinkProgram;
uint32_t _mesa_unmarshal_LinkProgram(struct gl_context *ctx, const struct marshal_cmd_LinkProgram *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_LinkProgram(GLuint program);
struct marshal_cmd_ShaderSource;
uint32_t _mesa_unmarshal_ShaderSource(struct gl_context *ctx, const struct marshal_cmd_ShaderSource *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ShaderSource(GLuint shader, GLsizei count, const GLchar * const * string, const GLint * length);
struct marshal_cmd_UseProgram;
uint32_t _mesa_unmarshal_UseProgram(struct gl_context *ctx, const struct marshal_cmd_UseProgram *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_UseProgram(GLuint program);
struct marshal_cmd_Uniform1f;
uint32_t _mesa_unmarshal_Uniform1f(struct gl_context *ctx, const struct marshal_cmd_Uniform1f *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Uniform1f(GLint location, GLfloat v0);
struct marshal_cmd_Uniform2f;
uint32_t _mesa_unmarshal_Uniform2f(struct gl_context *ctx, const struct marshal_cmd_Uniform2f *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Uniform2f(GLint location, GLfloat v0, GLfloat v1);
struct marshal_cmd_Uniform3f;
uint32_t _mesa_unmarshal_Uniform3f(struct gl_context *ctx, const struct marshal_cmd_Uniform3f *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Uniform3f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
struct marshal_cmd_Uniform4f;
uint32_t _mesa_unmarshal_Uniform4f(struct gl_context *ctx, const struct marshal_cmd_Uniform4f *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Uniform4f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
struct marshal_cmd_Uniform1i;
uint32_t _mesa_unmarshal_Uniform1i(struct gl_context *ctx, const struct marshal_cmd_Uniform1i *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Uniform1i(GLint location, GLint v0);
struct marshal_cmd_Uniform2i;
uint32_t _mesa_unmarshal_Uniform2i(struct gl_context *ctx, const struct marshal_cmd_Uniform2i *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Uniform2i(GLint location, GLint v0, GLint v1);
struct marshal_cmd_Uniform3i;
uint32_t _mesa_unmarshal_Uniform3i(struct gl_context *ctx, const struct marshal_cmd_Uniform3i *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Uniform3i(GLint location, GLint v0, GLint v1, GLint v2);
struct marshal_cmd_Uniform4i;
uint32_t _mesa_unmarshal_Uniform4i(struct gl_context *ctx, const struct marshal_cmd_Uniform4i *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Uniform4i(GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
struct marshal_cmd_Uniform1fv;
uint32_t _mesa_unmarshal_Uniform1fv(struct gl_context *ctx, const struct marshal_cmd_Uniform1fv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Uniform1fv(GLint location, GLsizei count, const GLfloat * value);
struct marshal_cmd_Uniform2fv;
uint32_t _mesa_unmarshal_Uniform2fv(struct gl_context *ctx, const struct marshal_cmd_Uniform2fv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Uniform2fv(GLint location, GLsizei count, const GLfloat * value);
struct marshal_cmd_Uniform3fv;
uint32_t _mesa_unmarshal_Uniform3fv(struct gl_context *ctx, const struct marshal_cmd_Uniform3fv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Uniform3fv(GLint location, GLsizei count, const GLfloat * value);
struct marshal_cmd_Uniform4fv;
uint32_t _mesa_unmarshal_Uniform4fv(struct gl_context *ctx, const struct marshal_cmd_Uniform4fv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Uniform4fv(GLint location, GLsizei count, const GLfloat * value);
struct marshal_cmd_Uniform1iv;
uint32_t _mesa_unmarshal_Uniform1iv(struct gl_context *ctx, const struct marshal_cmd_Uniform1iv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Uniform1iv(GLint location, GLsizei count, const GLint * value);
struct marshal_cmd_Uniform2iv;
uint32_t _mesa_unmarshal_Uniform2iv(struct gl_context *ctx, const struct marshal_cmd_Uniform2iv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Uniform2iv(GLint location, GLsizei count, const GLint * value);
struct marshal_cmd_Uniform3iv;
uint32_t _mesa_unmarshal_Uniform3iv(struct gl_context *ctx, const struct marshal_cmd_Uniform3iv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Uniform3iv(GLint location, GLsizei count, const GLint * value);
struct marshal_cmd_Uniform4iv;
uint32_t _mesa_unmarshal_Uniform4iv(struct gl_context *ctx, const struct marshal_cmd_Uniform4iv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Uniform4iv(GLint location, GLsizei count, const GLint * value);
struct marshal_cmd_UniformMatrix2fv;
uint32_t _mesa_unmarshal_UniformMatrix2fv(struct gl_context *ctx, const struct marshal_cmd_UniformMatrix2fv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_UniformMatrix2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat * value);
struct marshal_cmd_UniformMatrix3fv;
uint32_t _mesa_unmarshal_UniformMatrix3fv(struct gl_context *ctx, const struct marshal_cmd_UniformMatrix3fv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_UniformMatrix3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat * value);
struct marshal_cmd_UniformMatrix4fv;
uint32_t _mesa_unmarshal_UniformMatrix4fv(struct gl_context *ctx, const struct marshal_cmd_UniformMatrix4fv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_UniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat * value);
struct marshal_cmd_ValidateProgram;
uint32_t _mesa_unmarshal_ValidateProgram(struct gl_context *ctx, const struct marshal_cmd_ValidateProgram *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ValidateProgram(GLuint program);
struct marshal_cmd_VertexAttrib1d;
uint32_t _mesa_unmarshal_VertexAttrib1d(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib1d *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttrib1d(GLuint index, GLdouble x);
struct marshal_cmd_VertexAttrib1dv;
uint32_t _mesa_unmarshal_VertexAttrib1dv(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib1dv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttrib1dv(GLuint index, const GLdouble * v);
struct marshal_cmd_VertexAttrib1fARB;
uint32_t _mesa_unmarshal_VertexAttrib1fARB(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib1fARB *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttrib1fARB(GLuint index, GLfloat x);
struct marshal_cmd_VertexAttrib1fvARB;
uint32_t _mesa_unmarshal_VertexAttrib1fvARB(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib1fvARB *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttrib1fvARB(GLuint index, const GLfloat * v);
struct marshal_cmd_VertexAttrib1s;
uint32_t _mesa_unmarshal_VertexAttrib1s(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib1s *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttrib1s(GLuint index, GLshort x);
struct marshal_cmd_VertexAttrib1sv;
uint32_t _mesa_unmarshal_VertexAttrib1sv(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib1sv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttrib1sv(GLuint index, const GLshort * v);
struct marshal_cmd_VertexAttrib2d;
uint32_t _mesa_unmarshal_VertexAttrib2d(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib2d *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttrib2d(GLuint index, GLdouble x, GLdouble y);
struct marshal_cmd_VertexAttrib2dv;
uint32_t _mesa_unmarshal_VertexAttrib2dv(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib2dv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttrib2dv(GLuint index, const GLdouble * v);
struct marshal_cmd_VertexAttrib2fARB;
uint32_t _mesa_unmarshal_VertexAttrib2fARB(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib2fARB *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttrib2fARB(GLuint index, GLfloat x, GLfloat y);
struct marshal_cmd_VertexAttrib2fvARB;
uint32_t _mesa_unmarshal_VertexAttrib2fvARB(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib2fvARB *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttrib2fvARB(GLuint index, const GLfloat * v);
struct marshal_cmd_VertexAttrib2s;
uint32_t _mesa_unmarshal_VertexAttrib2s(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib2s *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttrib2s(GLuint index, GLshort x, GLshort y);
struct marshal_cmd_VertexAttrib2sv;
uint32_t _mesa_unmarshal_VertexAttrib2sv(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib2sv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttrib2sv(GLuint index, const GLshort * v);
struct marshal_cmd_VertexAttrib3d;
uint32_t _mesa_unmarshal_VertexAttrib3d(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib3d *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttrib3d(GLuint index, GLdouble x, GLdouble y, GLdouble z);
struct marshal_cmd_VertexAttrib3dv;
uint32_t _mesa_unmarshal_VertexAttrib3dv(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib3dv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttrib3dv(GLuint index, const GLdouble * v);
struct marshal_cmd_VertexAttrib3fARB;
uint32_t _mesa_unmarshal_VertexAttrib3fARB(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib3fARB *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttrib3fARB(GLuint index, GLfloat x, GLfloat y, GLfloat z);
struct marshal_cmd_VertexAttrib3fvARB;
uint32_t _mesa_unmarshal_VertexAttrib3fvARB(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib3fvARB *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttrib3fvARB(GLuint index, const GLfloat * v);
struct marshal_cmd_VertexAttrib3s;
uint32_t _mesa_unmarshal_VertexAttrib3s(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib3s *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttrib3s(GLuint index, GLshort x, GLshort y, GLshort z);
struct marshal_cmd_VertexAttrib3sv;
uint32_t _mesa_unmarshal_VertexAttrib3sv(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib3sv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttrib3sv(GLuint index, const GLshort * v);
struct marshal_cmd_VertexAttrib4Nbv;
uint32_t _mesa_unmarshal_VertexAttrib4Nbv(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4Nbv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttrib4Nbv(GLuint index, const GLbyte * v);
struct marshal_cmd_VertexAttrib4Niv;
uint32_t _mesa_unmarshal_VertexAttrib4Niv(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4Niv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttrib4Niv(GLuint index, const GLint * v);
struct marshal_cmd_VertexAttrib4Nsv;
uint32_t _mesa_unmarshal_VertexAttrib4Nsv(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4Nsv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttrib4Nsv(GLuint index, const GLshort * v);
struct marshal_cmd_VertexAttrib4Nub;
uint32_t _mesa_unmarshal_VertexAttrib4Nub(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4Nub *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttrib4Nub(GLuint index, GLubyte x, GLubyte y, GLubyte z, GLubyte w);
struct marshal_cmd_VertexAttrib4Nubv;
uint32_t _mesa_unmarshal_VertexAttrib4Nubv(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4Nubv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttrib4Nubv(GLuint index, const GLubyte * v);
struct marshal_cmd_VertexAttrib4Nuiv;
uint32_t _mesa_unmarshal_VertexAttrib4Nuiv(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4Nuiv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttrib4Nuiv(GLuint index, const GLuint * v);
struct marshal_cmd_VertexAttrib4Nusv;
uint32_t _mesa_unmarshal_VertexAttrib4Nusv(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4Nusv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttrib4Nusv(GLuint index, const GLushort * v);
struct marshal_cmd_VertexAttrib4bv;
uint32_t _mesa_unmarshal_VertexAttrib4bv(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4bv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttrib4bv(GLuint index, const GLbyte * v);
struct marshal_cmd_VertexAttrib4d;
uint32_t _mesa_unmarshal_VertexAttrib4d(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4d *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttrib4d(GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
struct marshal_cmd_VertexAttrib4dv;
uint32_t _mesa_unmarshal_VertexAttrib4dv(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4dv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttrib4dv(GLuint index, const GLdouble * v);
struct marshal_cmd_VertexAttrib4fARB;
uint32_t _mesa_unmarshal_VertexAttrib4fARB(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4fARB *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttrib4fARB(GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
struct marshal_cmd_VertexAttrib4fvARB;
uint32_t _mesa_unmarshal_VertexAttrib4fvARB(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4fvARB *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttrib4fvARB(GLuint index, const GLfloat * v);
struct marshal_cmd_VertexAttrib4iv;
uint32_t _mesa_unmarshal_VertexAttrib4iv(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4iv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttrib4iv(GLuint index, const GLint * v);
struct marshal_cmd_VertexAttrib4s;
uint32_t _mesa_unmarshal_VertexAttrib4s(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4s *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttrib4s(GLuint index, GLshort x, GLshort y, GLshort z, GLshort w);
struct marshal_cmd_VertexAttrib4sv;
uint32_t _mesa_unmarshal_VertexAttrib4sv(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4sv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttrib4sv(GLuint index, const GLshort * v);
struct marshal_cmd_VertexAttrib4ubv;
uint32_t _mesa_unmarshal_VertexAttrib4ubv(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4ubv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttrib4ubv(GLuint index, const GLubyte * v);
struct marshal_cmd_VertexAttrib4uiv;
uint32_t _mesa_unmarshal_VertexAttrib4uiv(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4uiv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttrib4uiv(GLuint index, const GLuint * v);
struct marshal_cmd_VertexAttrib4usv;
uint32_t _mesa_unmarshal_VertexAttrib4usv(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4usv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttrib4usv(GLuint index, const GLushort * v);
struct marshal_cmd_VertexAttribPointer;
uint32_t _mesa_unmarshal_VertexAttribPointer(struct gl_context *ctx, const struct marshal_cmd_VertexAttribPointer *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid * pointer);
struct marshal_cmd_UniformMatrix2x3fv;
uint32_t _mesa_unmarshal_UniformMatrix2x3fv(struct gl_context *ctx, const struct marshal_cmd_UniformMatrix2x3fv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_UniformMatrix2x3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat * value);
struct marshal_cmd_UniformMatrix3x2fv;
uint32_t _mesa_unmarshal_UniformMatrix3x2fv(struct gl_context *ctx, const struct marshal_cmd_UniformMatrix3x2fv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_UniformMatrix3x2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat * value);
struct marshal_cmd_UniformMatrix2x4fv;
uint32_t _mesa_unmarshal_UniformMatrix2x4fv(struct gl_context *ctx, const struct marshal_cmd_UniformMatrix2x4fv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_UniformMatrix2x4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat * value);
struct marshal_cmd_UniformMatrix4x2fv;
uint32_t _mesa_unmarshal_UniformMatrix4x2fv(struct gl_context *ctx, const struct marshal_cmd_UniformMatrix4x2fv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_UniformMatrix4x2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat * value);
struct marshal_cmd_UniformMatrix3x4fv;
uint32_t _mesa_unmarshal_UniformMatrix3x4fv(struct gl_context *ctx, const struct marshal_cmd_UniformMatrix3x4fv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_UniformMatrix3x4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat * value);
struct marshal_cmd_UniformMatrix4x3fv;
uint32_t _mesa_unmarshal_UniformMatrix4x3fv(struct gl_context *ctx, const struct marshal_cmd_UniformMatrix4x3fv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_UniformMatrix4x3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat * value);
struct marshal_cmd_ProgramStringARB;
uint32_t _mesa_unmarshal_ProgramStringARB(struct gl_context *ctx, const struct marshal_cmd_ProgramStringARB *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ProgramStringARB(GLenum target, GLenum format, GLsizei len, const GLvoid * string);
struct marshal_cmd_BindProgramARB;
uint32_t _mesa_unmarshal_BindProgramARB(struct gl_context *ctx, const struct marshal_cmd_BindProgramARB *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_BindProgramARB(GLenum target, GLuint program);
struct marshal_cmd_DeleteProgramsARB;
uint32_t _mesa_unmarshal_DeleteProgramsARB(struct gl_context *ctx, const struct marshal_cmd_DeleteProgramsARB *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_DeleteProgramsARB(GLsizei n, const GLuint * programs);
void GLAPIENTRY _mesa_marshal_GenProgramsARB(GLsizei n, GLuint * programs);
GLboolean GLAPIENTRY _mesa_marshal_IsProgramARB(GLuint program);
struct marshal_cmd_ProgramEnvParameter4dARB;
uint32_t _mesa_unmarshal_ProgramEnvParameter4dARB(struct gl_context *ctx, const struct marshal_cmd_ProgramEnvParameter4dARB *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ProgramEnvParameter4dARB(GLenum target, GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
struct marshal_cmd_ProgramEnvParameter4dvARB;
uint32_t _mesa_unmarshal_ProgramEnvParameter4dvARB(struct gl_context *ctx, const struct marshal_cmd_ProgramEnvParameter4dvARB *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ProgramEnvParameter4dvARB(GLenum target, GLuint index, const GLdouble * params);
struct marshal_cmd_ProgramEnvParameter4fARB;
uint32_t _mesa_unmarshal_ProgramEnvParameter4fARB(struct gl_context *ctx, const struct marshal_cmd_ProgramEnvParameter4fARB *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ProgramEnvParameter4fARB(GLenum target, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
struct marshal_cmd_ProgramEnvParameter4fvARB;
uint32_t _mesa_unmarshal_ProgramEnvParameter4fvARB(struct gl_context *ctx, const struct marshal_cmd_ProgramEnvParameter4fvARB *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ProgramEnvParameter4fvARB(GLenum target, GLuint index, const GLfloat * params);
struct marshal_cmd_ProgramLocalParameter4dARB;
uint32_t _mesa_unmarshal_ProgramLocalParameter4dARB(struct gl_context *ctx, const struct marshal_cmd_ProgramLocalParameter4dARB *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ProgramLocalParameter4dARB(GLenum target, GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
struct marshal_cmd_ProgramLocalParameter4dvARB;
uint32_t _mesa_unmarshal_ProgramLocalParameter4dvARB(struct gl_context *ctx, const struct marshal_cmd_ProgramLocalParameter4dvARB *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ProgramLocalParameter4dvARB(GLenum target, GLuint index, const GLdouble * params);
struct marshal_cmd_ProgramLocalParameter4fARB;
uint32_t _mesa_unmarshal_ProgramLocalParameter4fARB(struct gl_context *ctx, const struct marshal_cmd_ProgramLocalParameter4fARB *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ProgramLocalParameter4fARB(GLenum target, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
struct marshal_cmd_ProgramLocalParameter4fvARB;
uint32_t _mesa_unmarshal_ProgramLocalParameter4fvARB(struct gl_context *ctx, const struct marshal_cmd_ProgramLocalParameter4fvARB *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ProgramLocalParameter4fvARB(GLenum target, GLuint index, const GLfloat * params);
void GLAPIENTRY _mesa_marshal_GetProgramEnvParameterdvARB(GLenum target, GLuint index, GLdouble * params);
void GLAPIENTRY _mesa_marshal_GetProgramEnvParameterfvARB(GLenum target, GLuint index, GLfloat * params);
void GLAPIENTRY _mesa_marshal_GetProgramLocalParameterdvARB(GLenum target, GLuint index, GLdouble * params);
void GLAPIENTRY _mesa_marshal_GetProgramLocalParameterfvARB(GLenum target, GLuint index, GLfloat * params);
void GLAPIENTRY _mesa_marshal_GetProgramivARB(GLenum target, GLenum pname, GLint * params);
void GLAPIENTRY _mesa_marshal_GetProgramStringARB(GLenum target, GLenum pname, GLvoid * string);
struct marshal_cmd_DeleteObjectARB;
uint32_t _mesa_unmarshal_DeleteObjectARB(struct gl_context *ctx, const struct marshal_cmd_DeleteObjectARB *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_DeleteObjectARB(GLhandleARB obj);
GLhandleARB GLAPIENTRY _mesa_marshal_GetHandleARB(GLenum pname);
struct marshal_cmd_DetachObjectARB;
uint32_t _mesa_unmarshal_DetachObjectARB(struct gl_context *ctx, const struct marshal_cmd_DetachObjectARB *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_DetachObjectARB(GLhandleARB containerObj, GLhandleARB attachedObj);
GLhandleARB GLAPIENTRY _mesa_marshal_CreateShaderObjectARB(GLenum shaderType);
GLhandleARB GLAPIENTRY _mesa_marshal_CreateProgramObjectARB(void);
struct marshal_cmd_AttachObjectARB;
uint32_t _mesa_unmarshal_AttachObjectARB(struct gl_context *ctx, const struct marshal_cmd_AttachObjectARB *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_AttachObjectARB(GLhandleARB containerObj, GLhandleARB obj);
void GLAPIENTRY _mesa_marshal_GetObjectParameterfvARB(GLhandleARB obj, GLenum pname, GLfloat * params);
void GLAPIENTRY _mesa_marshal_GetObjectParameterivARB(GLhandleARB obj, GLenum pname, GLint * params);
void GLAPIENTRY _mesa_marshal_GetInfoLogARB(GLhandleARB obj, GLsizei maxLength, GLsizei * length, GLcharARB * infoLog);
void GLAPIENTRY _mesa_marshal_GetAttachedObjectsARB(GLhandleARB containerObj, GLsizei maxLength, GLsizei * length, GLhandleARB * infoLog);
struct marshal_cmd_ClampColor;
uint32_t _mesa_unmarshal_ClampColor(struct gl_context *ctx, const struct marshal_cmd_ClampColor *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ClampColor(GLenum target, GLenum clamp);
struct marshal_cmd_DrawArraysInstancedARB;
uint32_t _mesa_unmarshal_DrawArraysInstancedARB(struct gl_context *ctx, const struct marshal_cmd_DrawArraysInstancedARB *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_DrawArraysInstancedARB(GLenum mode, GLint first, GLsizei count, GLsizei primcount);
struct marshal_cmd_DrawElementsInstancedARB;
uint32_t _mesa_unmarshal_DrawElementsInstancedARB(struct gl_context *ctx, const struct marshal_cmd_DrawElementsInstancedARB *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_DrawElementsInstancedARB(GLenum mode, GLsizei count, GLenum type, const GLvoid * indices, GLsizei primcount);
GLboolean GLAPIENTRY _mesa_marshal_IsRenderbuffer(GLuint renderbuffer);
struct marshal_cmd_BindRenderbuffer;
uint32_t _mesa_unmarshal_BindRenderbuffer(struct gl_context *ctx, const struct marshal_cmd_BindRenderbuffer *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_BindRenderbuffer(GLenum target, GLuint renderbuffer);
struct marshal_cmd_DeleteRenderbuffers;
uint32_t _mesa_unmarshal_DeleteRenderbuffers(struct gl_context *ctx, const struct marshal_cmd_DeleteRenderbuffers *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_DeleteRenderbuffers(GLsizei n, const GLuint * renderbuffers);
void GLAPIENTRY _mesa_marshal_GenRenderbuffers(GLsizei n, GLuint * renderbuffers);
struct marshal_cmd_RenderbufferStorage;
uint32_t _mesa_unmarshal_RenderbufferStorage(struct gl_context *ctx, const struct marshal_cmd_RenderbufferStorage *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_RenderbufferStorage(GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
struct marshal_cmd_RenderbufferStorageMultisample;
uint32_t _mesa_unmarshal_RenderbufferStorageMultisample(struct gl_context *ctx, const struct marshal_cmd_RenderbufferStorageMultisample *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_RenderbufferStorageMultisample(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height);
void GLAPIENTRY _mesa_marshal_GetRenderbufferParameteriv(GLenum target, GLenum pname, GLint * params);
GLboolean GLAPIENTRY _mesa_marshal_IsFramebuffer(GLuint framebuffer);
struct marshal_cmd_BindFramebuffer;
uint32_t _mesa_unmarshal_BindFramebuffer(struct gl_context *ctx, const struct marshal_cmd_BindFramebuffer *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_BindFramebuffer(GLenum target, GLuint framebuffer);
struct marshal_cmd_DeleteFramebuffers;
uint32_t _mesa_unmarshal_DeleteFramebuffers(struct gl_context *ctx, const struct marshal_cmd_DeleteFramebuffers *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_DeleteFramebuffers(GLsizei n, const GLuint * framebuffers);
void GLAPIENTRY _mesa_marshal_GenFramebuffers(GLsizei n, GLuint * framebuffers);
GLenum GLAPIENTRY _mesa_marshal_CheckFramebufferStatus(GLenum target);
struct marshal_cmd_FramebufferTexture1D;
uint32_t _mesa_unmarshal_FramebufferTexture1D(struct gl_context *ctx, const struct marshal_cmd_FramebufferTexture1D *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_FramebufferTexture1D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
struct marshal_cmd_FramebufferTexture2D;
uint32_t _mesa_unmarshal_FramebufferTexture2D(struct gl_context *ctx, const struct marshal_cmd_FramebufferTexture2D *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_FramebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
struct marshal_cmd_FramebufferTexture3D;
uint32_t _mesa_unmarshal_FramebufferTexture3D(struct gl_context *ctx, const struct marshal_cmd_FramebufferTexture3D *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_FramebufferTexture3D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint layer);
struct marshal_cmd_FramebufferTextureLayer;
uint32_t _mesa_unmarshal_FramebufferTextureLayer(struct gl_context *ctx, const struct marshal_cmd_FramebufferTextureLayer *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_FramebufferTextureLayer(GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer);
struct marshal_cmd_FramebufferRenderbuffer;
uint32_t _mesa_unmarshal_FramebufferRenderbuffer(struct gl_context *ctx, const struct marshal_cmd_FramebufferRenderbuffer *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_FramebufferRenderbuffer(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
void GLAPIENTRY _mesa_marshal_GetFramebufferAttachmentParameteriv(GLenum target, GLenum attachment, GLenum pname, GLint * params);
struct marshal_cmd_BlitFramebuffer;
uint32_t _mesa_unmarshal_BlitFramebuffer(struct gl_context *ctx, const struct marshal_cmd_BlitFramebuffer *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_BlitFramebuffer(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter);
struct marshal_cmd_GenerateMipmap;
uint32_t _mesa_unmarshal_GenerateMipmap(struct gl_context *ctx, const struct marshal_cmd_GenerateMipmap *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_GenerateMipmap(GLenum target);
struct marshal_cmd_VertexAttribDivisor;
uint32_t _mesa_unmarshal_VertexAttribDivisor(struct gl_context *ctx, const struct marshal_cmd_VertexAttribDivisor *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttribDivisor(GLuint index, GLuint divisor);
struct marshal_cmd_VertexArrayVertexAttribDivisorEXT;
uint32_t _mesa_unmarshal_VertexArrayVertexAttribDivisorEXT(struct gl_context *ctx, const struct marshal_cmd_VertexArrayVertexAttribDivisorEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexArrayVertexAttribDivisorEXT(GLuint vaobj, GLuint index, GLuint divisor);
GLvoid * GLAPIENTRY _mesa_marshal_MapBufferRange(GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access);
struct marshal_cmd_FlushMappedBufferRange;
uint32_t _mesa_unmarshal_FlushMappedBufferRange(struct gl_context *ctx, const struct marshal_cmd_FlushMappedBufferRange *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_FlushMappedBufferRange(GLenum target, GLintptr offset, GLsizeiptr length);
struct marshal_cmd_TexBuffer;
uint32_t _mesa_unmarshal_TexBuffer(struct gl_context *ctx, const struct marshal_cmd_TexBuffer *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TexBuffer(GLenum target, GLenum internalFormat, GLuint buffer);
struct marshal_cmd_BindVertexArray;
uint32_t _mesa_unmarshal_BindVertexArray(struct gl_context *ctx, const struct marshal_cmd_BindVertexArray *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_BindVertexArray(GLuint array);
struct marshal_cmd_DeleteVertexArrays;
uint32_t _mesa_unmarshal_DeleteVertexArrays(struct gl_context *ctx, const struct marshal_cmd_DeleteVertexArrays *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_DeleteVertexArrays(GLsizei n, const GLuint * arrays);
void GLAPIENTRY _mesa_marshal_GenVertexArrays(GLsizei n, GLuint * arrays);
GLboolean GLAPIENTRY _mesa_marshal_IsVertexArray(GLuint array);
void GLAPIENTRY _mesa_marshal_GetUniformIndices(GLuint program, GLsizei uniformCount, const GLchar * const * uniformNames, GLuint * uniformIndices);
void GLAPIENTRY _mesa_marshal_GetActiveUniformsiv(GLuint program, GLsizei uniformCount, const GLuint * uniformIndices, GLenum pname, GLint * params);
void GLAPIENTRY _mesa_marshal_GetActiveUniformName(GLuint program, GLuint uniformIndex, GLsizei bufSize, GLsizei * length, GLchar * uniformName);
GLuint GLAPIENTRY _mesa_marshal_GetUniformBlockIndex(GLuint program, const GLchar * uniformBlockName);
void GLAPIENTRY _mesa_marshal_GetActiveUniformBlockiv(GLuint program, GLuint uniformBlockIndex, GLenum pname, GLint * params);
void GLAPIENTRY _mesa_marshal_GetActiveUniformBlockName(GLuint program, GLuint uniformBlockIndex, GLsizei bufSize, GLsizei * length, GLchar * uniformBlockName);
struct marshal_cmd_UniformBlockBinding;
uint32_t _mesa_unmarshal_UniformBlockBinding(struct gl_context *ctx, const struct marshal_cmd_UniformBlockBinding *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_UniformBlockBinding(GLuint program, GLuint uniformBlockIndex, GLuint uniformBlockBinding);
struct marshal_cmd_CopyBufferSubData;
uint32_t _mesa_unmarshal_CopyBufferSubData(struct gl_context *ctx, const struct marshal_cmd_CopyBufferSubData *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_CopyBufferSubData(GLenum readTarget, GLenum writeTarget, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size);
struct marshal_cmd_DrawElementsBaseVertex;
uint32_t _mesa_unmarshal_DrawElementsBaseVertex(struct gl_context *ctx, const struct marshal_cmd_DrawElementsBaseVertex *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_DrawElementsBaseVertex(GLenum mode, GLsizei count, GLenum type, const GLvoid * indices, GLint basevertex);
struct marshal_cmd_DrawRangeElementsBaseVertex;
uint32_t _mesa_unmarshal_DrawRangeElementsBaseVertex(struct gl_context *ctx, const struct marshal_cmd_DrawRangeElementsBaseVertex *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_DrawRangeElementsBaseVertex(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const GLvoid * indices, GLint basevertex);
struct marshal_cmd_MultiDrawElementsBaseVertex;
uint32_t _mesa_unmarshal_MultiDrawElementsBaseVertex(struct gl_context *ctx, const struct marshal_cmd_MultiDrawElementsBaseVertex *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_MultiDrawElementsBaseVertex(GLenum mode, const GLsizei * count, GLenum type, const GLvoid * const * indices, GLsizei primcount, const GLint * basevertex);
struct marshal_cmd_DrawElementsInstancedBaseVertex;
uint32_t _mesa_unmarshal_DrawElementsInstancedBaseVertex(struct gl_context *ctx, const struct marshal_cmd_DrawElementsInstancedBaseVertex *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_DrawElementsInstancedBaseVertex(GLenum mode, GLsizei count, GLenum type, const GLvoid * indices, GLsizei primcount, GLint basevertex);
GLsync GLAPIENTRY _mesa_marshal_FenceSync(GLenum condition, GLbitfield flags);
GLboolean GLAPIENTRY _mesa_marshal_IsSync(GLsync sync);
struct marshal_cmd_DeleteSync;
uint32_t _mesa_unmarshal_DeleteSync(struct gl_context *ctx, const struct marshal_cmd_DeleteSync *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_DeleteSync(GLsync sync);
GLenum GLAPIENTRY _mesa_marshal_ClientWaitSync(GLsync sync, GLbitfield flags, GLuint64 timeout);
struct marshal_cmd_WaitSync;
uint32_t _mesa_unmarshal_WaitSync(struct gl_context *ctx, const struct marshal_cmd_WaitSync *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_WaitSync(GLsync sync, GLbitfield flags, GLuint64 timeout);
void GLAPIENTRY _mesa_marshal_GetInteger64v(GLenum pname, GLint64 * params);
void GLAPIENTRY _mesa_marshal_GetSynciv(GLsync sync, GLenum pname, GLsizei bufSize, GLsizei * length, GLint * values);
struct marshal_cmd_TexImage2DMultisample;
uint32_t _mesa_unmarshal_TexImage2DMultisample(struct gl_context *ctx, const struct marshal_cmd_TexImage2DMultisample *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TexImage2DMultisample(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations);
struct marshal_cmd_TexImage3DMultisample;
uint32_t _mesa_unmarshal_TexImage3DMultisample(struct gl_context *ctx, const struct marshal_cmd_TexImage3DMultisample *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TexImage3DMultisample(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations);
void GLAPIENTRY _mesa_marshal_GetMultisamplefv(GLenum pname, GLuint index, GLfloat * val);
struct marshal_cmd_SampleMaski;
uint32_t _mesa_unmarshal_SampleMaski(struct gl_context *ctx, const struct marshal_cmd_SampleMaski *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_SampleMaski(GLuint index, GLbitfield mask);
struct marshal_cmd_BlendEquationiARB;
uint32_t _mesa_unmarshal_BlendEquationiARB(struct gl_context *ctx, const struct marshal_cmd_BlendEquationiARB *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_BlendEquationiARB(GLuint buf, GLenum mode);
struct marshal_cmd_BlendEquationSeparateiARB;
uint32_t _mesa_unmarshal_BlendEquationSeparateiARB(struct gl_context *ctx, const struct marshal_cmd_BlendEquationSeparateiARB *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_BlendEquationSeparateiARB(GLuint buf, GLenum modeRGB, GLenum modeA);
struct marshal_cmd_BlendFunciARB;
uint32_t _mesa_unmarshal_BlendFunciARB(struct gl_context *ctx, const struct marshal_cmd_BlendFunciARB *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_BlendFunciARB(GLuint buf, GLenum src, GLenum dst);
struct marshal_cmd_BlendFuncSeparateiARB;
uint32_t _mesa_unmarshal_BlendFuncSeparateiARB(struct gl_context *ctx, const struct marshal_cmd_BlendFuncSeparateiARB *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_BlendFuncSeparateiARB(GLuint buf, GLenum srcRGB, GLenum dstRGB, GLenum srcA, GLenum dstA);
struct marshal_cmd_MinSampleShading;
uint32_t _mesa_unmarshal_MinSampleShading(struct gl_context *ctx, const struct marshal_cmd_MinSampleShading *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_MinSampleShading(GLfloat value);
struct marshal_cmd_NamedStringARB;
uint32_t _mesa_unmarshal_NamedStringARB(struct gl_context *ctx, const struct marshal_cmd_NamedStringARB *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_NamedStringARB(GLenum type, GLint namelen, const GLchar * name, GLint stringlen, const GLchar * string);
struct marshal_cmd_DeleteNamedStringARB;
uint32_t _mesa_unmarshal_DeleteNamedStringARB(struct gl_context *ctx, const struct marshal_cmd_DeleteNamedStringARB *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_DeleteNamedStringARB(GLint namelen, const GLchar * name);
void GLAPIENTRY _mesa_marshal_CompileShaderIncludeARB(GLuint shader, GLsizei count, const GLchar * const * path, const GLint * length);
GLboolean GLAPIENTRY _mesa_marshal_IsNamedStringARB(GLint namelen, const GLchar * name);
void GLAPIENTRY _mesa_marshal_GetNamedStringARB(GLint namelen, const GLchar * name, GLsizei bufSize, GLint * stringlen, GLchar * string);
void GLAPIENTRY _mesa_marshal_GetNamedStringivARB(GLint namelen, const GLchar * name, GLenum pname, GLint * params);
struct marshal_cmd_BindFragDataLocationIndexed;
uint32_t _mesa_unmarshal_BindFragDataLocationIndexed(struct gl_context *ctx, const struct marshal_cmd_BindFragDataLocationIndexed *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_BindFragDataLocationIndexed(GLuint program, GLuint colorNumber, GLuint index, const GLchar * name);
GLint GLAPIENTRY _mesa_marshal_GetFragDataIndex(GLuint program, const GLchar * name);
void GLAPIENTRY _mesa_marshal_GenSamplers(GLsizei count, GLuint * samplers);
struct marshal_cmd_DeleteSamplers;
uint32_t _mesa_unmarshal_DeleteSamplers(struct gl_context *ctx, const struct marshal_cmd_DeleteSamplers *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_DeleteSamplers(GLsizei count, const GLuint * samplers);
GLboolean GLAPIENTRY _mesa_marshal_IsSampler(GLuint sampler);
struct marshal_cmd_BindSampler;
uint32_t _mesa_unmarshal_BindSampler(struct gl_context *ctx, const struct marshal_cmd_BindSampler *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_BindSampler(GLuint unit, GLuint sampler);
struct marshal_cmd_SamplerParameteri;
uint32_t _mesa_unmarshal_SamplerParameteri(struct gl_context *ctx, const struct marshal_cmd_SamplerParameteri *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_SamplerParameteri(GLuint sampler, GLenum pname, GLint param);
struct marshal_cmd_SamplerParameterf;
uint32_t _mesa_unmarshal_SamplerParameterf(struct gl_context *ctx, const struct marshal_cmd_SamplerParameterf *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_SamplerParameterf(GLuint sampler, GLenum pname, GLfloat param);
struct marshal_cmd_SamplerParameteriv;
uint32_t _mesa_unmarshal_SamplerParameteriv(struct gl_context *ctx, const struct marshal_cmd_SamplerParameteriv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_SamplerParameteriv(GLuint sampler, GLenum pname, const GLint * params);
struct marshal_cmd_SamplerParameterfv;
uint32_t _mesa_unmarshal_SamplerParameterfv(struct gl_context *ctx, const struct marshal_cmd_SamplerParameterfv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_SamplerParameterfv(GLuint sampler, GLenum pname, const GLfloat * params);
struct marshal_cmd_SamplerParameterIiv;
uint32_t _mesa_unmarshal_SamplerParameterIiv(struct gl_context *ctx, const struct marshal_cmd_SamplerParameterIiv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_SamplerParameterIiv(GLuint sampler, GLenum pname, const GLint * params);
struct marshal_cmd_SamplerParameterIuiv;
uint32_t _mesa_unmarshal_SamplerParameterIuiv(struct gl_context *ctx, const struct marshal_cmd_SamplerParameterIuiv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_SamplerParameterIuiv(GLuint sampler, GLenum pname, const GLuint * params);
void GLAPIENTRY _mesa_marshal_GetSamplerParameteriv(GLuint sampler, GLenum pname, GLint * params);
void GLAPIENTRY _mesa_marshal_GetSamplerParameterfv(GLuint sampler, GLenum pname, GLfloat * params);
void GLAPIENTRY _mesa_marshal_GetSamplerParameterIiv(GLuint sampler, GLenum pname, GLint * params);
void GLAPIENTRY _mesa_marshal_GetSamplerParameterIuiv(GLuint sampler, GLenum pname, GLuint * params);
void GLAPIENTRY _mesa_marshal_GetQueryObjecti64v(GLuint id, GLenum pname, GLint64 * params);
void GLAPIENTRY _mesa_marshal_GetQueryObjectui64v(GLuint id, GLenum pname, GLuint64 * params);
struct marshal_cmd_QueryCounter;
uint32_t _mesa_unmarshal_QueryCounter(struct gl_context *ctx, const struct marshal_cmd_QueryCounter *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_QueryCounter(GLuint id, GLenum target);
struct marshal_cmd_VertexP2ui;
uint32_t _mesa_unmarshal_VertexP2ui(struct gl_context *ctx, const struct marshal_cmd_VertexP2ui *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexP2ui(GLenum type, GLuint value);
struct marshal_cmd_VertexP3ui;
uint32_t _mesa_unmarshal_VertexP3ui(struct gl_context *ctx, const struct marshal_cmd_VertexP3ui *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexP3ui(GLenum type, GLuint value);
struct marshal_cmd_VertexP4ui;
uint32_t _mesa_unmarshal_VertexP4ui(struct gl_context *ctx, const struct marshal_cmd_VertexP4ui *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexP4ui(GLenum type, GLuint value);
struct marshal_cmd_VertexP2uiv;
uint32_t _mesa_unmarshal_VertexP2uiv(struct gl_context *ctx, const struct marshal_cmd_VertexP2uiv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexP2uiv(GLenum type, const GLuint * value);
struct marshal_cmd_VertexP3uiv;
uint32_t _mesa_unmarshal_VertexP3uiv(struct gl_context *ctx, const struct marshal_cmd_VertexP3uiv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexP3uiv(GLenum type, const GLuint * value);
struct marshal_cmd_VertexP4uiv;
uint32_t _mesa_unmarshal_VertexP4uiv(struct gl_context *ctx, const struct marshal_cmd_VertexP4uiv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexP4uiv(GLenum type, const GLuint * value);
struct marshal_cmd_TexCoordP1ui;
uint32_t _mesa_unmarshal_TexCoordP1ui(struct gl_context *ctx, const struct marshal_cmd_TexCoordP1ui *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TexCoordP1ui(GLenum type, GLuint coords);
struct marshal_cmd_TexCoordP2ui;
uint32_t _mesa_unmarshal_TexCoordP2ui(struct gl_context *ctx, const struct marshal_cmd_TexCoordP2ui *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TexCoordP2ui(GLenum type, GLuint coords);
struct marshal_cmd_TexCoordP3ui;
uint32_t _mesa_unmarshal_TexCoordP3ui(struct gl_context *ctx, const struct marshal_cmd_TexCoordP3ui *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TexCoordP3ui(GLenum type, GLuint coords);
struct marshal_cmd_TexCoordP4ui;
uint32_t _mesa_unmarshal_TexCoordP4ui(struct gl_context *ctx, const struct marshal_cmd_TexCoordP4ui *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TexCoordP4ui(GLenum type, GLuint coords);
struct marshal_cmd_TexCoordP1uiv;
uint32_t _mesa_unmarshal_TexCoordP1uiv(struct gl_context *ctx, const struct marshal_cmd_TexCoordP1uiv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TexCoordP1uiv(GLenum type, const GLuint * coords);
struct marshal_cmd_TexCoordP2uiv;
uint32_t _mesa_unmarshal_TexCoordP2uiv(struct gl_context *ctx, const struct marshal_cmd_TexCoordP2uiv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TexCoordP2uiv(GLenum type, const GLuint * coords);
struct marshal_cmd_TexCoordP3uiv;
uint32_t _mesa_unmarshal_TexCoordP3uiv(struct gl_context *ctx, const struct marshal_cmd_TexCoordP3uiv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TexCoordP3uiv(GLenum type, const GLuint * coords);
struct marshal_cmd_TexCoordP4uiv;
uint32_t _mesa_unmarshal_TexCoordP4uiv(struct gl_context *ctx, const struct marshal_cmd_TexCoordP4uiv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TexCoordP4uiv(GLenum type, const GLuint * coords);
struct marshal_cmd_MultiTexCoordP1ui;
uint32_t _mesa_unmarshal_MultiTexCoordP1ui(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoordP1ui *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_MultiTexCoordP1ui(GLenum texture, GLenum type, GLuint coords);
struct marshal_cmd_MultiTexCoordP2ui;
uint32_t _mesa_unmarshal_MultiTexCoordP2ui(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoordP2ui *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_MultiTexCoordP2ui(GLenum texture, GLenum type, GLuint coords);
struct marshal_cmd_MultiTexCoordP3ui;
uint32_t _mesa_unmarshal_MultiTexCoordP3ui(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoordP3ui *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_MultiTexCoordP3ui(GLenum texture, GLenum type, GLuint coords);
struct marshal_cmd_MultiTexCoordP4ui;
uint32_t _mesa_unmarshal_MultiTexCoordP4ui(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoordP4ui *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_MultiTexCoordP4ui(GLenum texture, GLenum type, GLuint coords);
struct marshal_cmd_MultiTexCoordP1uiv;
uint32_t _mesa_unmarshal_MultiTexCoordP1uiv(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoordP1uiv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_MultiTexCoordP1uiv(GLenum texture, GLenum type, const GLuint * coords);
struct marshal_cmd_MultiTexCoordP2uiv;
uint32_t _mesa_unmarshal_MultiTexCoordP2uiv(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoordP2uiv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_MultiTexCoordP2uiv(GLenum texture, GLenum type, const GLuint * coords);
struct marshal_cmd_MultiTexCoordP3uiv;
uint32_t _mesa_unmarshal_MultiTexCoordP3uiv(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoordP3uiv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_MultiTexCoordP3uiv(GLenum texture, GLenum type, const GLuint * coords);
struct marshal_cmd_MultiTexCoordP4uiv;
uint32_t _mesa_unmarshal_MultiTexCoordP4uiv(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoordP4uiv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_MultiTexCoordP4uiv(GLenum texture, GLenum type, const GLuint * coords);
struct marshal_cmd_NormalP3ui;
uint32_t _mesa_unmarshal_NormalP3ui(struct gl_context *ctx, const struct marshal_cmd_NormalP3ui *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_NormalP3ui(GLenum type, GLuint coords);
struct marshal_cmd_NormalP3uiv;
uint32_t _mesa_unmarshal_NormalP3uiv(struct gl_context *ctx, const struct marshal_cmd_NormalP3uiv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_NormalP3uiv(GLenum type, const GLuint * coords);
struct marshal_cmd_ColorP3ui;
uint32_t _mesa_unmarshal_ColorP3ui(struct gl_context *ctx, const struct marshal_cmd_ColorP3ui *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ColorP3ui(GLenum type, GLuint color);
struct marshal_cmd_ColorP4ui;
uint32_t _mesa_unmarshal_ColorP4ui(struct gl_context *ctx, const struct marshal_cmd_ColorP4ui *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ColorP4ui(GLenum type, GLuint color);
struct marshal_cmd_ColorP3uiv;
uint32_t _mesa_unmarshal_ColorP3uiv(struct gl_context *ctx, const struct marshal_cmd_ColorP3uiv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ColorP3uiv(GLenum type, const GLuint * color);
struct marshal_cmd_ColorP4uiv;
uint32_t _mesa_unmarshal_ColorP4uiv(struct gl_context *ctx, const struct marshal_cmd_ColorP4uiv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ColorP4uiv(GLenum type, const GLuint * color);
struct marshal_cmd_SecondaryColorP3ui;
uint32_t _mesa_unmarshal_SecondaryColorP3ui(struct gl_context *ctx, const struct marshal_cmd_SecondaryColorP3ui *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_SecondaryColorP3ui(GLenum type, GLuint color);
struct marshal_cmd_SecondaryColorP3uiv;
uint32_t _mesa_unmarshal_SecondaryColorP3uiv(struct gl_context *ctx, const struct marshal_cmd_SecondaryColorP3uiv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_SecondaryColorP3uiv(GLenum type, const GLuint * color);
struct marshal_cmd_VertexAttribP1ui;
uint32_t _mesa_unmarshal_VertexAttribP1ui(struct gl_context *ctx, const struct marshal_cmd_VertexAttribP1ui *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttribP1ui(GLuint index, GLenum type, GLboolean normalized, GLuint value);
struct marshal_cmd_VertexAttribP2ui;
uint32_t _mesa_unmarshal_VertexAttribP2ui(struct gl_context *ctx, const struct marshal_cmd_VertexAttribP2ui *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttribP2ui(GLuint index, GLenum type, GLboolean normalized, GLuint value);
struct marshal_cmd_VertexAttribP3ui;
uint32_t _mesa_unmarshal_VertexAttribP3ui(struct gl_context *ctx, const struct marshal_cmd_VertexAttribP3ui *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttribP3ui(GLuint index, GLenum type, GLboolean normalized, GLuint value);
struct marshal_cmd_VertexAttribP4ui;
uint32_t _mesa_unmarshal_VertexAttribP4ui(struct gl_context *ctx, const struct marshal_cmd_VertexAttribP4ui *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttribP4ui(GLuint index, GLenum type, GLboolean normalized, GLuint value);
struct marshal_cmd_VertexAttribP1uiv;
uint32_t _mesa_unmarshal_VertexAttribP1uiv(struct gl_context *ctx, const struct marshal_cmd_VertexAttribP1uiv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttribP1uiv(GLuint index, GLenum type, GLboolean normalized, const GLuint * value);
struct marshal_cmd_VertexAttribP2uiv;
uint32_t _mesa_unmarshal_VertexAttribP2uiv(struct gl_context *ctx, const struct marshal_cmd_VertexAttribP2uiv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttribP2uiv(GLuint index, GLenum type, GLboolean normalized, const GLuint * value);
struct marshal_cmd_VertexAttribP3uiv;
uint32_t _mesa_unmarshal_VertexAttribP3uiv(struct gl_context *ctx, const struct marshal_cmd_VertexAttribP3uiv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttribP3uiv(GLuint index, GLenum type, GLboolean normalized, const GLuint * value);
struct marshal_cmd_VertexAttribP4uiv;
uint32_t _mesa_unmarshal_VertexAttribP4uiv(struct gl_context *ctx, const struct marshal_cmd_VertexAttribP4uiv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttribP4uiv(GLuint index, GLenum type, GLboolean normalized, const GLuint * value);
GLint GLAPIENTRY _mesa_marshal_GetSubroutineUniformLocation(GLuint program, GLenum shadertype, const GLchar * name);
GLuint GLAPIENTRY _mesa_marshal_GetSubroutineIndex(GLuint program, GLenum shadertype, const GLchar * name);
void GLAPIENTRY _mesa_marshal_GetActiveSubroutineUniformiv(GLuint program, GLenum shadertype, GLuint index, GLenum pname, GLint * values);
void GLAPIENTRY _mesa_marshal_GetActiveSubroutineUniformName(GLuint program, GLenum shadertype, GLuint index, GLsizei bufsize, GLsizei * length, GLchar * name);
void GLAPIENTRY _mesa_marshal_GetActiveSubroutineName(GLuint program, GLenum shadertype, GLuint index, GLsizei bufsize, GLsizei * length, GLchar * name);
struct marshal_cmd_UniformSubroutinesuiv;
uint32_t _mesa_unmarshal_UniformSubroutinesuiv(struct gl_context *ctx, const struct marshal_cmd_UniformSubroutinesuiv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_UniformSubroutinesuiv(GLenum shadertype, GLsizei count, const GLuint * indices);
void GLAPIENTRY _mesa_marshal_GetUniformSubroutineuiv(GLenum shadertype, GLint location, GLuint * params);
void GLAPIENTRY _mesa_marshal_GetProgramStageiv(GLuint program, GLenum shadertype, GLenum pname, GLint * values);
struct marshal_cmd_PatchParameteri;
uint32_t _mesa_unmarshal_PatchParameteri(struct gl_context *ctx, const struct marshal_cmd_PatchParameteri *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_PatchParameteri(GLenum pname, GLint value);
struct marshal_cmd_PatchParameterfv;
uint32_t _mesa_unmarshal_PatchParameterfv(struct gl_context *ctx, const struct marshal_cmd_PatchParameterfv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_PatchParameterfv(GLenum pname, const GLfloat * values);
struct marshal_cmd_DrawArraysIndirect;
uint32_t _mesa_unmarshal_DrawArraysIndirect(struct gl_context *ctx, const struct marshal_cmd_DrawArraysIndirect *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_DrawArraysIndirect(GLenum mode, const GLvoid * indirect);
struct marshal_cmd_DrawElementsIndirect;
uint32_t _mesa_unmarshal_DrawElementsIndirect(struct gl_context *ctx, const struct marshal_cmd_DrawElementsIndirect *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_DrawElementsIndirect(GLenum mode, GLenum type, const GLvoid * indirect);
struct marshal_cmd_MultiDrawArraysIndirect;
uint32_t _mesa_unmarshal_MultiDrawArraysIndirect(struct gl_context *ctx, const struct marshal_cmd_MultiDrawArraysIndirect *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_MultiDrawArraysIndirect(GLenum mode, const GLvoid * indirect, GLsizei primcount, GLsizei stride);
struct marshal_cmd_MultiDrawElementsIndirect;
uint32_t _mesa_unmarshal_MultiDrawElementsIndirect(struct gl_context *ctx, const struct marshal_cmd_MultiDrawElementsIndirect *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_MultiDrawElementsIndirect(GLenum mode, GLenum type, const GLvoid * indirect, GLsizei primcount, GLsizei stride);
struct marshal_cmd_Uniform1d;
uint32_t _mesa_unmarshal_Uniform1d(struct gl_context *ctx, const struct marshal_cmd_Uniform1d *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Uniform1d(GLint location, GLdouble x);
struct marshal_cmd_Uniform2d;
uint32_t _mesa_unmarshal_Uniform2d(struct gl_context *ctx, const struct marshal_cmd_Uniform2d *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Uniform2d(GLint location, GLdouble x, GLdouble y);
struct marshal_cmd_Uniform3d;
uint32_t _mesa_unmarshal_Uniform3d(struct gl_context *ctx, const struct marshal_cmd_Uniform3d *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Uniform3d(GLint location, GLdouble x, GLdouble y, GLdouble z);
struct marshal_cmd_Uniform4d;
uint32_t _mesa_unmarshal_Uniform4d(struct gl_context *ctx, const struct marshal_cmd_Uniform4d *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Uniform4d(GLint location, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
struct marshal_cmd_Uniform1dv;
uint32_t _mesa_unmarshal_Uniform1dv(struct gl_context *ctx, const struct marshal_cmd_Uniform1dv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Uniform1dv(GLint location, GLsizei count, const GLdouble * value);
struct marshal_cmd_Uniform2dv;
uint32_t _mesa_unmarshal_Uniform2dv(struct gl_context *ctx, const struct marshal_cmd_Uniform2dv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Uniform2dv(GLint location, GLsizei count, const GLdouble * value);
struct marshal_cmd_Uniform3dv;
uint32_t _mesa_unmarshal_Uniform3dv(struct gl_context *ctx, const struct marshal_cmd_Uniform3dv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Uniform3dv(GLint location, GLsizei count, const GLdouble * value);
struct marshal_cmd_Uniform4dv;
uint32_t _mesa_unmarshal_Uniform4dv(struct gl_context *ctx, const struct marshal_cmd_Uniform4dv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Uniform4dv(GLint location, GLsizei count, const GLdouble * value);
struct marshal_cmd_UniformMatrix2dv;
uint32_t _mesa_unmarshal_UniformMatrix2dv(struct gl_context *ctx, const struct marshal_cmd_UniformMatrix2dv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_UniformMatrix2dv(GLint location, GLsizei count, GLboolean transpose, const GLdouble * value);
struct marshal_cmd_UniformMatrix3dv;
uint32_t _mesa_unmarshal_UniformMatrix3dv(struct gl_context *ctx, const struct marshal_cmd_UniformMatrix3dv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_UniformMatrix3dv(GLint location, GLsizei count, GLboolean transpose, const GLdouble * value);
struct marshal_cmd_UniformMatrix4dv;
uint32_t _mesa_unmarshal_UniformMatrix4dv(struct gl_context *ctx, const struct marshal_cmd_UniformMatrix4dv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_UniformMatrix4dv(GLint location, GLsizei count, GLboolean transpose, const GLdouble * value);
struct marshal_cmd_UniformMatrix2x3dv;
uint32_t _mesa_unmarshal_UniformMatrix2x3dv(struct gl_context *ctx, const struct marshal_cmd_UniformMatrix2x3dv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_UniformMatrix2x3dv(GLint location, GLsizei count, GLboolean transpose, const GLdouble * value);
struct marshal_cmd_UniformMatrix2x4dv;
uint32_t _mesa_unmarshal_UniformMatrix2x4dv(struct gl_context *ctx, const struct marshal_cmd_UniformMatrix2x4dv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_UniformMatrix2x4dv(GLint location, GLsizei count, GLboolean transpose, const GLdouble * value);
struct marshal_cmd_UniformMatrix3x2dv;
uint32_t _mesa_unmarshal_UniformMatrix3x2dv(struct gl_context *ctx, const struct marshal_cmd_UniformMatrix3x2dv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_UniformMatrix3x2dv(GLint location, GLsizei count, GLboolean transpose, const GLdouble * value);
struct marshal_cmd_UniformMatrix3x4dv;
uint32_t _mesa_unmarshal_UniformMatrix3x4dv(struct gl_context *ctx, const struct marshal_cmd_UniformMatrix3x4dv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_UniformMatrix3x4dv(GLint location, GLsizei count, GLboolean transpose, const GLdouble * value);
struct marshal_cmd_UniformMatrix4x2dv;
uint32_t _mesa_unmarshal_UniformMatrix4x2dv(struct gl_context *ctx, const struct marshal_cmd_UniformMatrix4x2dv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_UniformMatrix4x2dv(GLint location, GLsizei count, GLboolean transpose, const GLdouble * value);
struct marshal_cmd_UniformMatrix4x3dv;
uint32_t _mesa_unmarshal_UniformMatrix4x3dv(struct gl_context *ctx, const struct marshal_cmd_UniformMatrix4x3dv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_UniformMatrix4x3dv(GLint location, GLsizei count, GLboolean transpose, const GLdouble * value);
void GLAPIENTRY _mesa_marshal_GetUniformdv(GLuint program, GLint location, GLdouble * params);
struct marshal_cmd_ProgramUniform1d;
uint32_t _mesa_unmarshal_ProgramUniform1d(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform1d *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ProgramUniform1d(GLuint program, GLint location, GLdouble x);
struct marshal_cmd_ProgramUniform2d;
uint32_t _mesa_unmarshal_ProgramUniform2d(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform2d *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ProgramUniform2d(GLuint program, GLint location, GLdouble x, GLdouble y);
struct marshal_cmd_ProgramUniform3d;
uint32_t _mesa_unmarshal_ProgramUniform3d(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform3d *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ProgramUniform3d(GLuint program, GLint location, GLdouble x, GLdouble y, GLdouble z);
struct marshal_cmd_ProgramUniform4d;
uint32_t _mesa_unmarshal_ProgramUniform4d(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform4d *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ProgramUniform4d(GLuint program, GLint location, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
struct marshal_cmd_ProgramUniform1dv;
uint32_t _mesa_unmarshal_ProgramUniform1dv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform1dv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ProgramUniform1dv(GLuint program, GLint location, GLsizei count, const GLdouble * value);
struct marshal_cmd_ProgramUniform2dv;
uint32_t _mesa_unmarshal_ProgramUniform2dv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform2dv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ProgramUniform2dv(GLuint program, GLint location, GLsizei count, const GLdouble * value);
struct marshal_cmd_ProgramUniform3dv;
uint32_t _mesa_unmarshal_ProgramUniform3dv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform3dv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ProgramUniform3dv(GLuint program, GLint location, GLsizei count, const GLdouble * value);
struct marshal_cmd_ProgramUniform4dv;
uint32_t _mesa_unmarshal_ProgramUniform4dv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform4dv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ProgramUniform4dv(GLuint program, GLint location, GLsizei count, const GLdouble * value);
struct marshal_cmd_ProgramUniformMatrix2dv;
uint32_t _mesa_unmarshal_ProgramUniformMatrix2dv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniformMatrix2dv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ProgramUniformMatrix2dv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble * value);
struct marshal_cmd_ProgramUniformMatrix3dv;
uint32_t _mesa_unmarshal_ProgramUniformMatrix3dv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniformMatrix3dv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ProgramUniformMatrix3dv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble * value);
struct marshal_cmd_ProgramUniformMatrix4dv;
uint32_t _mesa_unmarshal_ProgramUniformMatrix4dv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniformMatrix4dv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ProgramUniformMatrix4dv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble * value);
struct marshal_cmd_ProgramUniformMatrix2x3dv;
uint32_t _mesa_unmarshal_ProgramUniformMatrix2x3dv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniformMatrix2x3dv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ProgramUniformMatrix2x3dv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble * value);
struct marshal_cmd_ProgramUniformMatrix2x4dv;
uint32_t _mesa_unmarshal_ProgramUniformMatrix2x4dv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniformMatrix2x4dv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ProgramUniformMatrix2x4dv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble * value);
struct marshal_cmd_ProgramUniformMatrix3x2dv;
uint32_t _mesa_unmarshal_ProgramUniformMatrix3x2dv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniformMatrix3x2dv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ProgramUniformMatrix3x2dv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble * value);
struct marshal_cmd_ProgramUniformMatrix3x4dv;
uint32_t _mesa_unmarshal_ProgramUniformMatrix3x4dv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniformMatrix3x4dv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ProgramUniformMatrix3x4dv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble * value);
struct marshal_cmd_ProgramUniformMatrix4x2dv;
uint32_t _mesa_unmarshal_ProgramUniformMatrix4x2dv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniformMatrix4x2dv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ProgramUniformMatrix4x2dv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble * value);
struct marshal_cmd_ProgramUniformMatrix4x3dv;
uint32_t _mesa_unmarshal_ProgramUniformMatrix4x3dv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniformMatrix4x3dv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ProgramUniformMatrix4x3dv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble * value);
struct marshal_cmd_DrawTransformFeedbackStream;
uint32_t _mesa_unmarshal_DrawTransformFeedbackStream(struct gl_context *ctx, const struct marshal_cmd_DrawTransformFeedbackStream *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_DrawTransformFeedbackStream(GLenum mode, GLuint id, GLuint stream);
struct marshal_cmd_BeginQueryIndexed;
uint32_t _mesa_unmarshal_BeginQueryIndexed(struct gl_context *ctx, const struct marshal_cmd_BeginQueryIndexed *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_BeginQueryIndexed(GLenum target, GLuint index, GLuint id);
struct marshal_cmd_EndQueryIndexed;
uint32_t _mesa_unmarshal_EndQueryIndexed(struct gl_context *ctx, const struct marshal_cmd_EndQueryIndexed *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_EndQueryIndexed(GLenum target, GLuint index);
void GLAPIENTRY _mesa_marshal_GetQueryIndexediv(GLenum target, GLuint index, GLenum pname, GLint * params);
struct marshal_cmd_UseProgramStages;
uint32_t _mesa_unmarshal_UseProgramStages(struct gl_context *ctx, const struct marshal_cmd_UseProgramStages *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_UseProgramStages(GLuint pipeline, GLbitfield stages, GLuint program);
struct marshal_cmd_ActiveShaderProgram;
uint32_t _mesa_unmarshal_ActiveShaderProgram(struct gl_context *ctx, const struct marshal_cmd_ActiveShaderProgram *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ActiveShaderProgram(GLuint pipeline, GLuint program);
GLuint GLAPIENTRY _mesa_marshal_CreateShaderProgramv(GLenum type, GLsizei count, const GLchar * const * strings);
struct marshal_cmd_BindProgramPipeline;
uint32_t _mesa_unmarshal_BindProgramPipeline(struct gl_context *ctx, const struct marshal_cmd_BindProgramPipeline *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_BindProgramPipeline(GLuint pipeline);
struct marshal_cmd_DeleteProgramPipelines;
uint32_t _mesa_unmarshal_DeleteProgramPipelines(struct gl_context *ctx, const struct marshal_cmd_DeleteProgramPipelines *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_DeleteProgramPipelines(GLsizei n, const GLuint * pipelines);
void GLAPIENTRY _mesa_marshal_GenProgramPipelines(GLsizei n, GLuint * pipelines);
GLboolean GLAPIENTRY _mesa_marshal_IsProgramPipeline(GLuint pipeline);
void GLAPIENTRY _mesa_marshal_GetProgramPipelineiv(GLuint pipeline, GLenum pname, GLint * params);
struct marshal_cmd_ProgramUniform1i;
uint32_t _mesa_unmarshal_ProgramUniform1i(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform1i *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ProgramUniform1i(GLuint program, GLint location, GLint x);
struct marshal_cmd_ProgramUniform2i;
uint32_t _mesa_unmarshal_ProgramUniform2i(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform2i *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ProgramUniform2i(GLuint program, GLint location, GLint x, GLint y);
struct marshal_cmd_ProgramUniform3i;
uint32_t _mesa_unmarshal_ProgramUniform3i(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform3i *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ProgramUniform3i(GLuint program, GLint location, GLint x, GLint y, GLint z);
struct marshal_cmd_ProgramUniform4i;
uint32_t _mesa_unmarshal_ProgramUniform4i(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform4i *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ProgramUniform4i(GLuint program, GLint location, GLint x, GLint y, GLint z, GLint w);
struct marshal_cmd_ProgramUniform1ui;
uint32_t _mesa_unmarshal_ProgramUniform1ui(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform1ui *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ProgramUniform1ui(GLuint program, GLint location, GLuint x);
struct marshal_cmd_ProgramUniform2ui;
uint32_t _mesa_unmarshal_ProgramUniform2ui(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform2ui *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ProgramUniform2ui(GLuint program, GLint location, GLuint x, GLuint y);
struct marshal_cmd_ProgramUniform3ui;
uint32_t _mesa_unmarshal_ProgramUniform3ui(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform3ui *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ProgramUniform3ui(GLuint program, GLint location, GLuint x, GLuint y, GLuint z);
struct marshal_cmd_ProgramUniform4ui;
uint32_t _mesa_unmarshal_ProgramUniform4ui(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform4ui *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ProgramUniform4ui(GLuint program, GLint location, GLuint x, GLuint y, GLuint z, GLuint w);
struct marshal_cmd_ProgramUniform1f;
uint32_t _mesa_unmarshal_ProgramUniform1f(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform1f *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ProgramUniform1f(GLuint program, GLint location, GLfloat x);
struct marshal_cmd_ProgramUniform2f;
uint32_t _mesa_unmarshal_ProgramUniform2f(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform2f *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ProgramUniform2f(GLuint program, GLint location, GLfloat x, GLfloat y);
struct marshal_cmd_ProgramUniform3f;
uint32_t _mesa_unmarshal_ProgramUniform3f(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform3f *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ProgramUniform3f(GLuint program, GLint location, GLfloat x, GLfloat y, GLfloat z);
struct marshal_cmd_ProgramUniform4f;
uint32_t _mesa_unmarshal_ProgramUniform4f(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform4f *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ProgramUniform4f(GLuint program, GLint location, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
struct marshal_cmd_ProgramUniform1iv;
uint32_t _mesa_unmarshal_ProgramUniform1iv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform1iv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ProgramUniform1iv(GLuint program, GLint location, GLsizei count, const GLint * value);
struct marshal_cmd_ProgramUniform2iv;
uint32_t _mesa_unmarshal_ProgramUniform2iv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform2iv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ProgramUniform2iv(GLuint program, GLint location, GLsizei count, const GLint * value);
struct marshal_cmd_ProgramUniform3iv;
uint32_t _mesa_unmarshal_ProgramUniform3iv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform3iv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ProgramUniform3iv(GLuint program, GLint location, GLsizei count, const GLint * value);
struct marshal_cmd_ProgramUniform4iv;
uint32_t _mesa_unmarshal_ProgramUniform4iv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform4iv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ProgramUniform4iv(GLuint program, GLint location, GLsizei count, const GLint * value);
struct marshal_cmd_ProgramUniform1uiv;
uint32_t _mesa_unmarshal_ProgramUniform1uiv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform1uiv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ProgramUniform1uiv(GLuint program, GLint location, GLsizei count, const GLuint * value);
struct marshal_cmd_ProgramUniform2uiv;
uint32_t _mesa_unmarshal_ProgramUniform2uiv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform2uiv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ProgramUniform2uiv(GLuint program, GLint location, GLsizei count, const GLuint * value);
struct marshal_cmd_ProgramUniform3uiv;
uint32_t _mesa_unmarshal_ProgramUniform3uiv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform3uiv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ProgramUniform3uiv(GLuint program, GLint location, GLsizei count, const GLuint * value);
struct marshal_cmd_ProgramUniform4uiv;
uint32_t _mesa_unmarshal_ProgramUniform4uiv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform4uiv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ProgramUniform4uiv(GLuint program, GLint location, GLsizei count, const GLuint * value);
struct marshal_cmd_ProgramUniform1fv;
uint32_t _mesa_unmarshal_ProgramUniform1fv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform1fv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ProgramUniform1fv(GLuint program, GLint location, GLsizei count, const GLfloat * value);
struct marshal_cmd_ProgramUniform2fv;
uint32_t _mesa_unmarshal_ProgramUniform2fv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform2fv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ProgramUniform2fv(GLuint program, GLint location, GLsizei count, const GLfloat * value);
struct marshal_cmd_ProgramUniform3fv;
uint32_t _mesa_unmarshal_ProgramUniform3fv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform3fv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ProgramUniform3fv(GLuint program, GLint location, GLsizei count, const GLfloat * value);
struct marshal_cmd_ProgramUniform4fv;
uint32_t _mesa_unmarshal_ProgramUniform4fv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform4fv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ProgramUniform4fv(GLuint program, GLint location, GLsizei count, const GLfloat * value);
struct marshal_cmd_ProgramUniformMatrix2fv;
uint32_t _mesa_unmarshal_ProgramUniformMatrix2fv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniformMatrix2fv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ProgramUniformMatrix2fv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat * value);
struct marshal_cmd_ProgramUniformMatrix3fv;
uint32_t _mesa_unmarshal_ProgramUniformMatrix3fv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniformMatrix3fv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ProgramUniformMatrix3fv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat * value);
struct marshal_cmd_ProgramUniformMatrix4fv;
uint32_t _mesa_unmarshal_ProgramUniformMatrix4fv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniformMatrix4fv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ProgramUniformMatrix4fv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat * value);
struct marshal_cmd_ProgramUniformMatrix2x3fv;
uint32_t _mesa_unmarshal_ProgramUniformMatrix2x3fv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniformMatrix2x3fv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ProgramUniformMatrix2x3fv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat * value);
struct marshal_cmd_ProgramUniformMatrix3x2fv;
uint32_t _mesa_unmarshal_ProgramUniformMatrix3x2fv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniformMatrix3x2fv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ProgramUniformMatrix3x2fv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat * value);
struct marshal_cmd_ProgramUniformMatrix2x4fv;
uint32_t _mesa_unmarshal_ProgramUniformMatrix2x4fv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniformMatrix2x4fv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ProgramUniformMatrix2x4fv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat * value);
struct marshal_cmd_ProgramUniformMatrix4x2fv;
uint32_t _mesa_unmarshal_ProgramUniformMatrix4x2fv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniformMatrix4x2fv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ProgramUniformMatrix4x2fv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat * value);
struct marshal_cmd_ProgramUniformMatrix3x4fv;
uint32_t _mesa_unmarshal_ProgramUniformMatrix3x4fv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniformMatrix3x4fv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ProgramUniformMatrix3x4fv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat * value);
struct marshal_cmd_ProgramUniformMatrix4x3fv;
uint32_t _mesa_unmarshal_ProgramUniformMatrix4x3fv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniformMatrix4x3fv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ProgramUniformMatrix4x3fv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat * value);
struct marshal_cmd_ValidateProgramPipeline;
uint32_t _mesa_unmarshal_ValidateProgramPipeline(struct gl_context *ctx, const struct marshal_cmd_ValidateProgramPipeline *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ValidateProgramPipeline(GLuint pipeline);
void GLAPIENTRY _mesa_marshal_GetProgramPipelineInfoLog(GLuint pipeline, GLsizei bufSize, GLsizei * length, GLchar * infoLog);
struct marshal_cmd_VertexAttribL1d;
uint32_t _mesa_unmarshal_VertexAttribL1d(struct gl_context *ctx, const struct marshal_cmd_VertexAttribL1d *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttribL1d(GLuint index, GLdouble x);
struct marshal_cmd_VertexAttribL2d;
uint32_t _mesa_unmarshal_VertexAttribL2d(struct gl_context *ctx, const struct marshal_cmd_VertexAttribL2d *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttribL2d(GLuint index, GLdouble x, GLdouble y);
struct marshal_cmd_VertexAttribL3d;
uint32_t _mesa_unmarshal_VertexAttribL3d(struct gl_context *ctx, const struct marshal_cmd_VertexAttribL3d *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttribL3d(GLuint index, GLdouble x, GLdouble y, GLdouble z);
struct marshal_cmd_VertexAttribL4d;
uint32_t _mesa_unmarshal_VertexAttribL4d(struct gl_context *ctx, const struct marshal_cmd_VertexAttribL4d *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttribL4d(GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
struct marshal_cmd_VertexAttribL1dv;
uint32_t _mesa_unmarshal_VertexAttribL1dv(struct gl_context *ctx, const struct marshal_cmd_VertexAttribL1dv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttribL1dv(GLuint index, const GLdouble * v);
struct marshal_cmd_VertexAttribL2dv;
uint32_t _mesa_unmarshal_VertexAttribL2dv(struct gl_context *ctx, const struct marshal_cmd_VertexAttribL2dv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttribL2dv(GLuint index, const GLdouble * v);
struct marshal_cmd_VertexAttribL3dv;
uint32_t _mesa_unmarshal_VertexAttribL3dv(struct gl_context *ctx, const struct marshal_cmd_VertexAttribL3dv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttribL3dv(GLuint index, const GLdouble * v);
struct marshal_cmd_VertexAttribL4dv;
uint32_t _mesa_unmarshal_VertexAttribL4dv(struct gl_context *ctx, const struct marshal_cmd_VertexAttribL4dv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttribL4dv(GLuint index, const GLdouble * v);
struct marshal_cmd_VertexAttribLPointer;
uint32_t _mesa_unmarshal_VertexAttribLPointer(struct gl_context *ctx, const struct marshal_cmd_VertexAttribLPointer *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttribLPointer(GLuint index, GLint size, GLenum type, GLsizei stride, const GLvoid * pointer);
void GLAPIENTRY _mesa_marshal_GetVertexAttribLdv(GLuint index, GLenum pname, GLdouble * params);
struct marshal_cmd_VertexArrayVertexAttribLOffsetEXT;
uint32_t _mesa_unmarshal_VertexArrayVertexAttribLOffsetEXT(struct gl_context *ctx, const struct marshal_cmd_VertexArrayVertexAttribLOffsetEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexArrayVertexAttribLOffsetEXT(GLuint vaobj, GLuint buffer, GLuint index, GLint size, GLenum type, GLsizei stride, GLintptr offset);
void GLAPIENTRY _mesa_marshal_GetShaderPrecisionFormat(GLenum shadertype, GLenum precisiontype, GLint * range, GLint * precision);
struct marshal_cmd_ReleaseShaderCompiler;
uint32_t _mesa_unmarshal_ReleaseShaderCompiler(struct gl_context *ctx, const struct marshal_cmd_ReleaseShaderCompiler *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ReleaseShaderCompiler(void);
struct marshal_cmd_ShaderBinary;
uint32_t _mesa_unmarshal_ShaderBinary(struct gl_context *ctx, const struct marshal_cmd_ShaderBinary *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ShaderBinary(GLsizei n, const GLuint * shaders, GLenum binaryformat, const GLvoid * binary, GLsizei length);
struct marshal_cmd_ClearDepthf;
uint32_t _mesa_unmarshal_ClearDepthf(struct gl_context *ctx, const struct marshal_cmd_ClearDepthf *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ClearDepthf(GLclampf depth);
struct marshal_cmd_DepthRangef;
uint32_t _mesa_unmarshal_DepthRangef(struct gl_context *ctx, const struct marshal_cmd_DepthRangef *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_DepthRangef(GLclampf zNear, GLclampf zFar);
void GLAPIENTRY _mesa_marshal_GetProgramBinary(GLuint program, GLsizei bufSize, GLsizei * length, GLenum * binaryFormat, GLvoid * binary);
struct marshal_cmd_ProgramBinary;
uint32_t _mesa_unmarshal_ProgramBinary(struct gl_context *ctx, const struct marshal_cmd_ProgramBinary *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ProgramBinary(GLuint program, GLenum binaryFormat, const GLvoid * binary, GLsizei length);
struct marshal_cmd_ProgramParameteri;
uint32_t _mesa_unmarshal_ProgramParameteri(struct gl_context *ctx, const struct marshal_cmd_ProgramParameteri *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ProgramParameteri(GLuint program, GLenum pname, GLint value);
struct marshal_cmd_DebugMessageControl;
uint32_t _mesa_unmarshal_DebugMessageControl(struct gl_context *ctx, const struct marshal_cmd_DebugMessageControl *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_DebugMessageControl(GLenum source, GLenum type, GLenum severity, GLsizei count, const GLuint * ids, GLboolean enabled);
struct marshal_cmd_DebugMessageInsert;
uint32_t _mesa_unmarshal_DebugMessageInsert(struct gl_context *ctx, const struct marshal_cmd_DebugMessageInsert *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_DebugMessageInsert(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar * buf);
void GLAPIENTRY _mesa_marshal_DebugMessageCallback(GLDEBUGPROC callback, const GLvoid * userParam);
GLuint GLAPIENTRY _mesa_marshal_GetDebugMessageLog(GLuint count, GLsizei bufsize, GLenum * sources, GLenum * types, GLuint * ids, GLenum * severities, GLsizei * lengths, GLchar * messageLog);
GLenum GLAPIENTRY _mesa_marshal_GetGraphicsResetStatusARB(void);
void GLAPIENTRY _mesa_marshal_GetnMapdvARB(GLenum target, GLenum query, GLsizei bufSize, GLdouble * v);
void GLAPIENTRY _mesa_marshal_GetnMapfvARB(GLenum target, GLenum query, GLsizei bufSize, GLfloat * v);
void GLAPIENTRY _mesa_marshal_GetnMapivARB(GLenum target, GLenum query, GLsizei bufSize, GLint * v);
void GLAPIENTRY _mesa_marshal_GetnPixelMapfvARB(GLenum map, GLsizei bufSize, GLfloat * values);
void GLAPIENTRY _mesa_marshal_GetnPixelMapuivARB(GLenum map, GLsizei bufSize, GLuint * values);
void GLAPIENTRY _mesa_marshal_GetnPixelMapusvARB(GLenum map, GLsizei bufSize, GLushort * values);
struct marshal_cmd_GetnPolygonStippleARB;
uint32_t _mesa_unmarshal_GetnPolygonStippleARB(struct gl_context *ctx, const struct marshal_cmd_GetnPolygonStippleARB *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_GetnPolygonStippleARB(GLsizei bufSize, GLubyte * pattern);
struct marshal_cmd_GetnTexImageARB;
uint32_t _mesa_unmarshal_GetnTexImageARB(struct gl_context *ctx, const struct marshal_cmd_GetnTexImageARB *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_GetnTexImageARB(GLenum target, GLint level, GLenum format, GLenum type, GLsizei bufSize, GLvoid * img);
struct marshal_cmd_ReadnPixelsARB;
uint32_t _mesa_unmarshal_ReadnPixelsARB(struct gl_context *ctx, const struct marshal_cmd_ReadnPixelsARB *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ReadnPixelsARB(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLsizei bufSize, GLvoid * data);
void GLAPIENTRY _mesa_marshal_GetnColorTableARB(GLenum target, GLenum format, GLenum type, GLsizei bufSize, GLvoid * table);
void GLAPIENTRY _mesa_marshal_GetnConvolutionFilterARB(GLenum target, GLenum format, GLenum type, GLsizei bufSize, GLvoid * image);
void GLAPIENTRY _mesa_marshal_GetnSeparableFilterARB(GLenum target, GLenum format, GLenum type, GLsizei rowBufSize, GLvoid * row, GLsizei columnBufSize, GLvoid * column, GLvoid * span);
void GLAPIENTRY _mesa_marshal_GetnHistogramARB(GLenum target, GLboolean reset, GLenum format, GLenum type, GLsizei bufSize, GLvoid * values);
void GLAPIENTRY _mesa_marshal_GetnMinmaxARB(GLenum target, GLboolean reset, GLenum format, GLenum type, GLsizei bufSize, GLvoid * values);
struct marshal_cmd_GetnCompressedTexImageARB;
uint32_t _mesa_unmarshal_GetnCompressedTexImageARB(struct gl_context *ctx, const struct marshal_cmd_GetnCompressedTexImageARB *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_GetnCompressedTexImageARB(GLenum target, GLint lod, GLsizei bufSize, GLvoid * img);
void GLAPIENTRY _mesa_marshal_GetnUniformfvARB(GLuint program, GLint location, GLsizei bufSize, GLfloat * params);
void GLAPIENTRY _mesa_marshal_GetnUniformivARB(GLuint program, GLint location, GLsizei bufSize, GLint * params);
void GLAPIENTRY _mesa_marshal_GetnUniformuivARB(GLuint program, GLint location, GLsizei bufSize, GLuint * params);
void GLAPIENTRY _mesa_marshal_GetnUniformdvARB(GLuint program, GLint location, GLsizei bufSize, GLdouble * params);
struct marshal_cmd_DrawArraysInstancedBaseInstance;
uint32_t _mesa_unmarshal_DrawArraysInstancedBaseInstance(struct gl_context *ctx, const struct marshal_cmd_DrawArraysInstancedBaseInstance *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_DrawArraysInstancedBaseInstance(GLenum mode, GLint first, GLsizei count, GLsizei primcount, GLuint baseinstance);
struct marshal_cmd_DrawElementsInstancedBaseInstance;
uint32_t _mesa_unmarshal_DrawElementsInstancedBaseInstance(struct gl_context *ctx, const struct marshal_cmd_DrawElementsInstancedBaseInstance *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_DrawElementsInstancedBaseInstance(GLenum mode, GLsizei count, GLenum type, const GLvoid * indices, GLsizei primcount, GLuint baseinstance);
struct marshal_cmd_DrawElementsInstancedBaseVertexBaseInstance;
uint32_t _mesa_unmarshal_DrawElementsInstancedBaseVertexBaseInstance(struct gl_context *ctx, const struct marshal_cmd_DrawElementsInstancedBaseVertexBaseInstance *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_DrawElementsInstancedBaseVertexBaseInstance(GLenum mode, GLsizei count, GLenum type, const GLvoid * indices, GLsizei primcount, GLint basevertex, GLuint baseinstance);
struct marshal_cmd_DrawTransformFeedbackInstanced;
uint32_t _mesa_unmarshal_DrawTransformFeedbackInstanced(struct gl_context *ctx, const struct marshal_cmd_DrawTransformFeedbackInstanced *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_DrawTransformFeedbackInstanced(GLenum mode, GLuint id, GLsizei primcount);
struct marshal_cmd_DrawTransformFeedbackStreamInstanced;
uint32_t _mesa_unmarshal_DrawTransformFeedbackStreamInstanced(struct gl_context *ctx, const struct marshal_cmd_DrawTransformFeedbackStreamInstanced *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_DrawTransformFeedbackStreamInstanced(GLenum mode, GLuint id, GLuint stream, GLsizei primcount);
void GLAPIENTRY _mesa_marshal_GetInternalformativ(GLenum target, GLenum internalformat, GLenum pname, GLsizei bufSize, GLint * params);
void GLAPIENTRY _mesa_marshal_GetActiveAtomicCounterBufferiv(GLuint program, GLuint bufferIndex, GLenum pname, GLint * params);
struct marshal_cmd_BindImageTexture;
uint32_t _mesa_unmarshal_BindImageTexture(struct gl_context *ctx, const struct marshal_cmd_BindImageTexture *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_BindImageTexture(GLuint unit, GLuint texture, GLint level, GLboolean layered, GLint layer, GLenum access, GLenum format);
struct marshal_cmd_MemoryBarrier;
uint32_t _mesa_unmarshal_MemoryBarrier(struct gl_context *ctx, const struct marshal_cmd_MemoryBarrier *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_MemoryBarrier(GLbitfield barriers);
struct marshal_cmd_TexStorage1D;
uint32_t _mesa_unmarshal_TexStorage1D(struct gl_context *ctx, const struct marshal_cmd_TexStorage1D *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TexStorage1D(GLenum target, GLsizei levels, GLenum internalFormat, GLsizei width);
struct marshal_cmd_TexStorage2D;
uint32_t _mesa_unmarshal_TexStorage2D(struct gl_context *ctx, const struct marshal_cmd_TexStorage2D *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TexStorage2D(GLenum target, GLsizei levels, GLenum internalFormat, GLsizei width, GLsizei height);
struct marshal_cmd_TexStorage3D;
uint32_t _mesa_unmarshal_TexStorage3D(struct gl_context *ctx, const struct marshal_cmd_TexStorage3D *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TexStorage3D(GLenum target, GLsizei levels, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth);
struct marshal_cmd_TextureStorage1DEXT;
uint32_t _mesa_unmarshal_TextureStorage1DEXT(struct gl_context *ctx, const struct marshal_cmd_TextureStorage1DEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TextureStorage1DEXT(GLuint texture, GLenum target, GLsizei levels, GLenum internalFormat, GLsizei width);
struct marshal_cmd_TextureStorage2DEXT;
uint32_t _mesa_unmarshal_TextureStorage2DEXT(struct gl_context *ctx, const struct marshal_cmd_TextureStorage2DEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TextureStorage2DEXT(GLuint texture, GLenum target, GLsizei levels, GLenum internalFormat, GLsizei width, GLsizei height);
struct marshal_cmd_TextureStorage3DEXT;
uint32_t _mesa_unmarshal_TextureStorage3DEXT(struct gl_context *ctx, const struct marshal_cmd_TextureStorage3DEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TextureStorage3DEXT(GLuint texture, GLenum target, GLsizei levels, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth);
struct marshal_cmd_PushDebugGroup;
uint32_t _mesa_unmarshal_PushDebugGroup(struct gl_context *ctx, const struct marshal_cmd_PushDebugGroup *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_PushDebugGroup(GLenum source, GLuint id, GLsizei length, const GLchar * message);
struct marshal_cmd_PopDebugGroup;
uint32_t _mesa_unmarshal_PopDebugGroup(struct gl_context *ctx, const struct marshal_cmd_PopDebugGroup *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_PopDebugGroup(void);
struct marshal_cmd_ObjectLabel;
uint32_t _mesa_unmarshal_ObjectLabel(struct gl_context *ctx, const struct marshal_cmd_ObjectLabel *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ObjectLabel(GLenum identifier, GLuint name, GLsizei length, const GLchar * label);
void GLAPIENTRY _mesa_marshal_GetObjectLabel(GLenum identifier, GLuint name, GLsizei bufSize, GLsizei * length, GLchar * label);
void GLAPIENTRY _mesa_marshal_ObjectPtrLabel(const GLvoid * ptr, GLsizei length, const GLchar * label);
void GLAPIENTRY _mesa_marshal_GetObjectPtrLabel(const GLvoid * ptr, GLsizei bufSize, GLsizei * length, GLchar * label);
void GLAPIENTRY _mesa_marshal_ClearBufferData(GLenum target, GLenum internalformat, GLenum format, GLenum type, const GLvoid * data);
void GLAPIENTRY _mesa_marshal_ClearBufferSubData(GLenum target, GLenum internalformat, GLintptr offset, GLsizeiptr size, GLenum format, GLenum type, const GLvoid * data);
void GLAPIENTRY _mesa_marshal_ClearNamedBufferDataEXT(GLuint buffer, GLenum internalformat, GLenum format, GLenum type, const GLvoid * data);
void GLAPIENTRY _mesa_marshal_ClearNamedBufferSubDataEXT(GLuint buffer, GLenum internalformat, GLintptr offset, GLsizeiptr size, GLenum format, GLenum type, const GLvoid * data);
struct marshal_cmd_DispatchCompute;
uint32_t _mesa_unmarshal_DispatchCompute(struct gl_context *ctx, const struct marshal_cmd_DispatchCompute *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_DispatchCompute(GLuint num_groups_x, GLuint num_groups_y, GLuint num_groups_z);
struct marshal_cmd_DispatchComputeIndirect;
uint32_t _mesa_unmarshal_DispatchComputeIndirect(struct gl_context *ctx, const struct marshal_cmd_DispatchComputeIndirect *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_DispatchComputeIndirect(GLintptr indirect);
struct marshal_cmd_CopyImageSubData;
uint32_t _mesa_unmarshal_CopyImageSubData(struct gl_context *ctx, const struct marshal_cmd_CopyImageSubData *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_CopyImageSubData(GLuint srcName, GLenum srcTarget, GLint srcLevel, GLint srcX, GLint srcY, GLint srcZ, GLuint dstName, GLenum dstTarget, GLint dstLevel, GLint dstX, GLint dstY, GLint dstZ, GLsizei srcWidth, GLsizei srcHeight, GLsizei srcDepth);
struct marshal_cmd_TextureView;
uint32_t _mesa_unmarshal_TextureView(struct gl_context *ctx, const struct marshal_cmd_TextureView *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TextureView(GLuint texture, GLenum target, GLuint origtexture, GLenum internalformat, GLuint minlevel, GLuint numlevels, GLuint minlayer, GLuint numlayers);
struct marshal_cmd_BindVertexBuffer;
uint32_t _mesa_unmarshal_BindVertexBuffer(struct gl_context *ctx, const struct marshal_cmd_BindVertexBuffer *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_BindVertexBuffer(GLuint bindingindex, GLuint buffer, GLintptr offset, GLsizei stride);
struct marshal_cmd_VertexAttribFormat;
uint32_t _mesa_unmarshal_VertexAttribFormat(struct gl_context *ctx, const struct marshal_cmd_VertexAttribFormat *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttribFormat(GLuint attribindex, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset);
struct marshal_cmd_VertexAttribIFormat;
uint32_t _mesa_unmarshal_VertexAttribIFormat(struct gl_context *ctx, const struct marshal_cmd_VertexAttribIFormat *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttribIFormat(GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset);
struct marshal_cmd_VertexAttribLFormat;
uint32_t _mesa_unmarshal_VertexAttribLFormat(struct gl_context *ctx, const struct marshal_cmd_VertexAttribLFormat *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttribLFormat(GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset);
struct marshal_cmd_VertexAttribBinding;
uint32_t _mesa_unmarshal_VertexAttribBinding(struct gl_context *ctx, const struct marshal_cmd_VertexAttribBinding *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttribBinding(GLuint attribindex, GLuint bindingindex);
struct marshal_cmd_VertexBindingDivisor;
uint32_t _mesa_unmarshal_VertexBindingDivisor(struct gl_context *ctx, const struct marshal_cmd_VertexBindingDivisor *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexBindingDivisor(GLuint bindingindex, GLuint divisor);
struct marshal_cmd_VertexArrayBindVertexBufferEXT;
uint32_t _mesa_unmarshal_VertexArrayBindVertexBufferEXT(struct gl_context *ctx, const struct marshal_cmd_VertexArrayBindVertexBufferEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexArrayBindVertexBufferEXT(GLuint vaobj, GLuint bindingindex, GLuint buffer, GLintptr offset, GLsizei stride);
struct marshal_cmd_VertexArrayVertexAttribFormatEXT;
uint32_t _mesa_unmarshal_VertexArrayVertexAttribFormatEXT(struct gl_context *ctx, const struct marshal_cmd_VertexArrayVertexAttribFormatEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexArrayVertexAttribFormatEXT(GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset);
struct marshal_cmd_VertexArrayVertexAttribIFormatEXT;
uint32_t _mesa_unmarshal_VertexArrayVertexAttribIFormatEXT(struct gl_context *ctx, const struct marshal_cmd_VertexArrayVertexAttribIFormatEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexArrayVertexAttribIFormatEXT(GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset);
struct marshal_cmd_VertexArrayVertexAttribLFormatEXT;
uint32_t _mesa_unmarshal_VertexArrayVertexAttribLFormatEXT(struct gl_context *ctx, const struct marshal_cmd_VertexArrayVertexAttribLFormatEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexArrayVertexAttribLFormatEXT(GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset);
struct marshal_cmd_VertexArrayVertexAttribBindingEXT;
uint32_t _mesa_unmarshal_VertexArrayVertexAttribBindingEXT(struct gl_context *ctx, const struct marshal_cmd_VertexArrayVertexAttribBindingEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexArrayVertexAttribBindingEXT(GLuint vaobj, GLuint attribindex, GLuint bindingindex);
struct marshal_cmd_VertexArrayVertexBindingDivisorEXT;
uint32_t _mesa_unmarshal_VertexArrayVertexBindingDivisorEXT(struct gl_context *ctx, const struct marshal_cmd_VertexArrayVertexBindingDivisorEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexArrayVertexBindingDivisorEXT(GLuint vaobj, GLuint bindingindex, GLuint divisor);
struct marshal_cmd_FramebufferParameteri;
uint32_t _mesa_unmarshal_FramebufferParameteri(struct gl_context *ctx, const struct marshal_cmd_FramebufferParameteri *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_FramebufferParameteri(GLenum target, GLenum pname, GLint param);
void GLAPIENTRY _mesa_marshal_GetFramebufferParameteriv(GLenum target, GLenum pname, GLint * params);
struct marshal_cmd_NamedFramebufferParameteriEXT;
uint32_t _mesa_unmarshal_NamedFramebufferParameteriEXT(struct gl_context *ctx, const struct marshal_cmd_NamedFramebufferParameteriEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_NamedFramebufferParameteriEXT(GLuint framebuffer, GLenum pname, GLint param);
void GLAPIENTRY _mesa_marshal_GetNamedFramebufferParameterivEXT(GLuint framebuffer, GLenum pname, GLint* params);
void GLAPIENTRY _mesa_marshal_GetInternalformati64v(GLenum target, GLenum internalformat, GLenum pname, GLsizei bufSize, GLint64 * params);
struct marshal_cmd_InvalidateTexSubImage;
uint32_t _mesa_unmarshal_InvalidateTexSubImage(struct gl_context *ctx, const struct marshal_cmd_InvalidateTexSubImage *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_InvalidateTexSubImage(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth);
struct marshal_cmd_InvalidateTexImage;
uint32_t _mesa_unmarshal_InvalidateTexImage(struct gl_context *ctx, const struct marshal_cmd_InvalidateTexImage *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_InvalidateTexImage(GLuint texture, GLint level);
struct marshal_cmd_InvalidateBufferSubData;
uint32_t _mesa_unmarshal_InvalidateBufferSubData(struct gl_context *ctx, const struct marshal_cmd_InvalidateBufferSubData *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_InvalidateBufferSubData(GLuint buffer, GLintptr offset, GLsizeiptr length);
struct marshal_cmd_InvalidateBufferData;
uint32_t _mesa_unmarshal_InvalidateBufferData(struct gl_context *ctx, const struct marshal_cmd_InvalidateBufferData *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_InvalidateBufferData(GLuint buffer);
struct marshal_cmd_InvalidateSubFramebuffer;
uint32_t _mesa_unmarshal_InvalidateSubFramebuffer(struct gl_context *ctx, const struct marshal_cmd_InvalidateSubFramebuffer *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_InvalidateSubFramebuffer(GLenum target, GLsizei numAttachments, const GLenum * attachments, GLint x, GLint y, GLsizei width, GLsizei height);
struct marshal_cmd_InvalidateFramebuffer;
uint32_t _mesa_unmarshal_InvalidateFramebuffer(struct gl_context *ctx, const struct marshal_cmd_InvalidateFramebuffer *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_InvalidateFramebuffer(GLenum target, GLsizei numAttachments, const GLenum * attachments);
void GLAPIENTRY _mesa_marshal_GetProgramInterfaceiv(GLuint program, GLenum programInterface, GLenum pname, GLint * params);
GLuint GLAPIENTRY _mesa_marshal_GetProgramResourceIndex(GLuint program, GLenum programInterface, const GLchar * name);
void GLAPIENTRY _mesa_marshal_GetProgramResourceName(GLuint program, GLenum programInterface, GLuint index, GLsizei  bufSize, GLsizei * length, GLchar * name);
void GLAPIENTRY _mesa_marshal_GetProgramResourceiv(GLuint program, GLenum programInterface, GLuint index, GLsizei  propCount, const GLenum * props, GLsizei  bufSize, GLsizei * length, GLint * params);
GLint GLAPIENTRY _mesa_marshal_GetProgramResourceLocation(GLuint program, GLenum programInterface, const GLchar * name);
GLint GLAPIENTRY _mesa_marshal_GetProgramResourceLocationIndex(GLuint program, GLenum programInterface, const GLchar * name);
struct marshal_cmd_ShaderStorageBlockBinding;
uint32_t _mesa_unmarshal_ShaderStorageBlockBinding(struct gl_context *ctx, const struct marshal_cmd_ShaderStorageBlockBinding *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ShaderStorageBlockBinding(GLuint program, GLuint shaderStorageBlockIndex, GLuint shaderStorageBlockBinding);
struct marshal_cmd_TexBufferRange;
uint32_t _mesa_unmarshal_TexBufferRange(struct gl_context *ctx, const struct marshal_cmd_TexBufferRange *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TexBufferRange(GLenum target, GLenum internalformat, GLuint buffer, GLintptr offset, GLsizeiptr size);
struct marshal_cmd_TextureBufferRangeEXT;
uint32_t _mesa_unmarshal_TextureBufferRangeEXT(struct gl_context *ctx, const struct marshal_cmd_TextureBufferRangeEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TextureBufferRangeEXT(GLuint texture, GLenum target, GLenum internalformat, GLuint buffer, GLintptr offset, GLsizeiptr size);
struct marshal_cmd_TexStorage2DMultisample;
uint32_t _mesa_unmarshal_TexStorage2DMultisample(struct gl_context *ctx, const struct marshal_cmd_TexStorage2DMultisample *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TexStorage2DMultisample(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations);
struct marshal_cmd_TexStorage3DMultisample;
uint32_t _mesa_unmarshal_TexStorage3DMultisample(struct gl_context *ctx, const struct marshal_cmd_TexStorage3DMultisample *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TexStorage3DMultisample(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations);
struct marshal_cmd_TextureStorage2DMultisampleEXT;
uint32_t _mesa_unmarshal_TextureStorage2DMultisampleEXT(struct gl_context *ctx, const struct marshal_cmd_TextureStorage2DMultisampleEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TextureStorage2DMultisampleEXT(GLuint texture, GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations);
struct marshal_cmd_TextureStorage3DMultisampleEXT;
uint32_t _mesa_unmarshal_TextureStorage3DMultisampleEXT(struct gl_context *ctx, const struct marshal_cmd_TextureStorage3DMultisampleEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TextureStorage3DMultisampleEXT(GLuint texture, GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations);
void GLAPIENTRY _mesa_marshal_BufferStorage(GLenum target, GLsizeiptr size, const GLvoid * data, GLbitfield flags);
void GLAPIENTRY _mesa_marshal_NamedBufferStorageEXT(GLuint buffer, GLsizeiptr size, const GLvoid * data, GLbitfield flags);
void GLAPIENTRY _mesa_marshal_ClearTexImage(GLuint texture, GLint level, GLenum format, GLenum type, const GLvoid * data);
void GLAPIENTRY _mesa_marshal_ClearTexSubImage(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const GLvoid * data);
struct marshal_cmd_BindBuffersBase;
uint32_t _mesa_unmarshal_BindBuffersBase(struct gl_context *ctx, const struct marshal_cmd_BindBuffersBase *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_BindBuffersBase(GLenum target, GLuint first, GLsizei count, const GLuint * buffers);
struct marshal_cmd_BindBuffersRange;
uint32_t _mesa_unmarshal_BindBuffersRange(struct gl_context *ctx, const struct marshal_cmd_BindBuffersRange *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_BindBuffersRange(GLenum target, GLuint first, GLsizei count, const GLuint * buffers, const GLintptr * offsets, const GLsizeiptr * sizes);
struct marshal_cmd_BindTextures;
uint32_t _mesa_unmarshal_BindTextures(struct gl_context *ctx, const struct marshal_cmd_BindTextures *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_BindTextures(GLuint first, GLsizei count, const GLuint * textures);
struct marshal_cmd_BindSamplers;
uint32_t _mesa_unmarshal_BindSamplers(struct gl_context *ctx, const struct marshal_cmd_BindSamplers *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_BindSamplers(GLuint first, GLsizei count, const GLuint * samplers);
struct marshal_cmd_BindImageTextures;
uint32_t _mesa_unmarshal_BindImageTextures(struct gl_context *ctx, const struct marshal_cmd_BindImageTextures *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_BindImageTextures(GLuint first, GLsizei count, const GLuint * textures);
struct marshal_cmd_BindVertexBuffers;
uint32_t _mesa_unmarshal_BindVertexBuffers(struct gl_context *ctx, const struct marshal_cmd_BindVertexBuffers *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_BindVertexBuffers(GLuint first, GLsizei count, const GLuint * buffers, const GLintptr * offsets, const GLsizei * strides);
GLuint64 GLAPIENTRY _mesa_marshal_GetTextureHandleARB(GLuint texture);
GLuint64 GLAPIENTRY _mesa_marshal_GetTextureSamplerHandleARB(GLuint texture, GLuint sampler);
struct marshal_cmd_MakeTextureHandleResidentARB;
uint32_t _mesa_unmarshal_MakeTextureHandleResidentARB(struct gl_context *ctx, const struct marshal_cmd_MakeTextureHandleResidentARB *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_MakeTextureHandleResidentARB(GLuint64 handle);
struct marshal_cmd_MakeTextureHandleNonResidentARB;
uint32_t _mesa_unmarshal_MakeTextureHandleNonResidentARB(struct gl_context *ctx, const struct marshal_cmd_MakeTextureHandleNonResidentARB *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_MakeTextureHandleNonResidentARB(GLuint64 handle);
GLuint64 GLAPIENTRY _mesa_marshal_GetImageHandleARB(GLuint texture, GLint level, GLboolean layered, GLint layer, GLenum format);
struct marshal_cmd_MakeImageHandleResidentARB;
uint32_t _mesa_unmarshal_MakeImageHandleResidentARB(struct gl_context *ctx, const struct marshal_cmd_MakeImageHandleResidentARB *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_MakeImageHandleResidentARB(GLuint64 handle, GLenum access);
struct marshal_cmd_MakeImageHandleNonResidentARB;
uint32_t _mesa_unmarshal_MakeImageHandleNonResidentARB(struct gl_context *ctx, const struct marshal_cmd_MakeImageHandleNonResidentARB *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_MakeImageHandleNonResidentARB(GLuint64 handle);
struct marshal_cmd_UniformHandleui64ARB;
uint32_t _mesa_unmarshal_UniformHandleui64ARB(struct gl_context *ctx, const struct marshal_cmd_UniformHandleui64ARB *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_UniformHandleui64ARB(GLint location, GLuint64 value);
struct marshal_cmd_UniformHandleui64vARB;
uint32_t _mesa_unmarshal_UniformHandleui64vARB(struct gl_context *ctx, const struct marshal_cmd_UniformHandleui64vARB *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_UniformHandleui64vARB(GLint location, GLsizei count, const GLuint64 * value);
struct marshal_cmd_ProgramUniformHandleui64ARB;
uint32_t _mesa_unmarshal_ProgramUniformHandleui64ARB(struct gl_context *ctx, const struct marshal_cmd_ProgramUniformHandleui64ARB *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ProgramUniformHandleui64ARB(GLuint program, GLint location, GLuint64 value);
struct marshal_cmd_ProgramUniformHandleui64vARB;
uint32_t _mesa_unmarshal_ProgramUniformHandleui64vARB(struct gl_context *ctx, const struct marshal_cmd_ProgramUniformHandleui64vARB *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ProgramUniformHandleui64vARB(GLuint program, GLint location, GLsizei count, const GLuint64 * value);
GLboolean GLAPIENTRY _mesa_marshal_IsTextureHandleResidentARB(GLuint64 handle);
GLboolean GLAPIENTRY _mesa_marshal_IsImageHandleResidentARB(GLuint64 handle);
struct marshal_cmd_VertexAttribL1ui64ARB;
uint32_t _mesa_unmarshal_VertexAttribL1ui64ARB(struct gl_context *ctx, const struct marshal_cmd_VertexAttribL1ui64ARB *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttribL1ui64ARB(GLuint index, GLuint64EXT x);
struct marshal_cmd_VertexAttribL1ui64vARB;
uint32_t _mesa_unmarshal_VertexAttribL1ui64vARB(struct gl_context *ctx, const struct marshal_cmd_VertexAttribL1ui64vARB *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttribL1ui64vARB(GLuint index, const GLuint64EXT * v);
void GLAPIENTRY _mesa_marshal_GetVertexAttribLui64vARB(GLuint index, GLenum pname, GLuint64EXT * params);
struct marshal_cmd_DispatchComputeGroupSizeARB;
uint32_t _mesa_unmarshal_DispatchComputeGroupSizeARB(struct gl_context *ctx, const struct marshal_cmd_DispatchComputeGroupSizeARB *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_DispatchComputeGroupSizeARB(GLuint num_groups_x, GLuint num_groups_y, GLuint num_groups_z, GLuint group_size_x, GLuint group_size_y, GLuint group_size_z);
struct marshal_cmd_MultiDrawArraysIndirectCountARB;
uint32_t _mesa_unmarshal_MultiDrawArraysIndirectCountARB(struct gl_context *ctx, const struct marshal_cmd_MultiDrawArraysIndirectCountARB *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_MultiDrawArraysIndirectCountARB(GLenum mode, GLintptr indirect, GLintptr drawcount, GLsizei maxdrawcount, GLsizei stride);
struct marshal_cmd_MultiDrawElementsIndirectCountARB;
uint32_t _mesa_unmarshal_MultiDrawElementsIndirectCountARB(struct gl_context *ctx, const struct marshal_cmd_MultiDrawElementsIndirectCountARB *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_MultiDrawElementsIndirectCountARB(GLenum mode, GLenum type, GLintptr indirect, GLintptr drawcount, GLsizei maxdrawcount, GLsizei stride);
struct marshal_cmd_ClipControl;
uint32_t _mesa_unmarshal_ClipControl(struct gl_context *ctx, const struct marshal_cmd_ClipControl *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ClipControl(GLenum origin, GLenum depth);
void GLAPIENTRY _mesa_marshal_CreateTransformFeedbacks(GLsizei n, GLuint * ids);
struct marshal_cmd_TransformFeedbackBufferBase;
uint32_t _mesa_unmarshal_TransformFeedbackBufferBase(struct gl_context *ctx, const struct marshal_cmd_TransformFeedbackBufferBase *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TransformFeedbackBufferBase(GLuint xfb, GLuint index, GLuint buffer);
struct marshal_cmd_TransformFeedbackBufferRange;
uint32_t _mesa_unmarshal_TransformFeedbackBufferRange(struct gl_context *ctx, const struct marshal_cmd_TransformFeedbackBufferRange *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TransformFeedbackBufferRange(GLuint xfb, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size);
void GLAPIENTRY _mesa_marshal_GetTransformFeedbackiv(GLuint xfb, GLenum pname, GLint * param);
void GLAPIENTRY _mesa_marshal_GetTransformFeedbacki_v(GLuint xfb, GLenum pname, GLuint index, GLint * param);
void GLAPIENTRY _mesa_marshal_GetTransformFeedbacki64_v(GLuint xfb, GLenum pname, GLuint index, GLint64 * param);
void GLAPIENTRY _mesa_marshal_CreateBuffers(GLsizei n, GLuint * buffers);
void GLAPIENTRY _mesa_marshal_NamedBufferStorage(GLuint buffer, GLsizeiptr size, const GLvoid * data, GLbitfield flags);
struct marshal_cmd_NamedBufferData;
uint32_t _mesa_unmarshal_NamedBufferData(struct gl_context *ctx, const struct marshal_cmd_NamedBufferData *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_NamedBufferData(GLuint buffer, GLsizeiptr size, const GLvoid * data, GLenum usage);
struct marshal_cmd_NamedBufferSubData;
uint32_t _mesa_unmarshal_NamedBufferSubData(struct gl_context *ctx, const struct marshal_cmd_NamedBufferSubData *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_NamedBufferSubData(GLuint buffer, GLintptr offset, GLsizeiptr size, const GLvoid * data);
struct marshal_cmd_CopyNamedBufferSubData;
uint32_t _mesa_unmarshal_CopyNamedBufferSubData(struct gl_context *ctx, const struct marshal_cmd_CopyNamedBufferSubData *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_CopyNamedBufferSubData(GLuint readBuffer, GLuint writeBuffer, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size);
void GLAPIENTRY _mesa_marshal_ClearNamedBufferData(GLuint buffer, GLenum internalformat, GLenum format, GLenum type, const GLvoid * data);
void GLAPIENTRY _mesa_marshal_ClearNamedBufferSubData(GLuint buffer, GLenum internalformat, GLintptr offset, GLsizeiptr size, GLenum format, GLenum type, const GLvoid * data);
GLvoid * GLAPIENTRY _mesa_marshal_MapNamedBuffer(GLuint buffer, GLenum access);
GLvoid * GLAPIENTRY _mesa_marshal_MapNamedBufferRange(GLuint buffer, GLintptr offset, GLsizeiptr length, GLbitfield access);
struct marshal_cmd_UnmapNamedBufferEXT;
uint32_t _mesa_unmarshal_UnmapNamedBufferEXT(struct gl_context *ctx, const struct marshal_cmd_UnmapNamedBufferEXT *cmd, const uint64_t *last);
GLboolean GLAPIENTRY _mesa_marshal_UnmapNamedBufferEXT(GLuint buffer);
struct marshal_cmd_FlushMappedNamedBufferRange;
uint32_t _mesa_unmarshal_FlushMappedNamedBufferRange(struct gl_context *ctx, const struct marshal_cmd_FlushMappedNamedBufferRange *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_FlushMappedNamedBufferRange(GLuint buffer, GLintptr offset, GLsizeiptr length);
void GLAPIENTRY _mesa_marshal_GetNamedBufferParameteriv(GLuint buffer, GLenum pname, GLint * params);
void GLAPIENTRY _mesa_marshal_GetNamedBufferParameteri64v(GLuint buffer, GLenum pname, GLint64 * params);
void GLAPIENTRY _mesa_marshal_GetNamedBufferPointerv(GLuint buffer, GLenum pname, GLvoid ** params);
void GLAPIENTRY _mesa_marshal_GetNamedBufferSubData(GLuint buffer, GLintptr offset, GLsizeiptr size, GLvoid * data);
void GLAPIENTRY _mesa_marshal_CreateFramebuffers(GLsizei n, GLuint * framebuffers);
struct marshal_cmd_NamedFramebufferRenderbuffer;
uint32_t _mesa_unmarshal_NamedFramebufferRenderbuffer(struct gl_context *ctx, const struct marshal_cmd_NamedFramebufferRenderbuffer *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_NamedFramebufferRenderbuffer(GLuint framebuffer, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
struct marshal_cmd_NamedFramebufferParameteri;
uint32_t _mesa_unmarshal_NamedFramebufferParameteri(struct gl_context *ctx, const struct marshal_cmd_NamedFramebufferParameteri *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_NamedFramebufferParameteri(GLuint framebuffer, GLenum pname, GLint param);
struct marshal_cmd_NamedFramebufferTexture;
uint32_t _mesa_unmarshal_NamedFramebufferTexture(struct gl_context *ctx, const struct marshal_cmd_NamedFramebufferTexture *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_NamedFramebufferTexture(GLuint framebuffer, GLenum attachment, GLuint texture, GLint level);
struct marshal_cmd_NamedFramebufferTextureLayer;
uint32_t _mesa_unmarshal_NamedFramebufferTextureLayer(struct gl_context *ctx, const struct marshal_cmd_NamedFramebufferTextureLayer *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_NamedFramebufferTextureLayer(GLuint framebuffer, GLenum attachment, GLuint texture, GLint level, GLint layer);
struct marshal_cmd_NamedFramebufferDrawBuffer;
uint32_t _mesa_unmarshal_NamedFramebufferDrawBuffer(struct gl_context *ctx, const struct marshal_cmd_NamedFramebufferDrawBuffer *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_NamedFramebufferDrawBuffer(GLuint framebuffer, GLenum buf);
struct marshal_cmd_NamedFramebufferDrawBuffers;
uint32_t _mesa_unmarshal_NamedFramebufferDrawBuffers(struct gl_context *ctx, const struct marshal_cmd_NamedFramebufferDrawBuffers *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_NamedFramebufferDrawBuffers(GLuint framebuffer, GLsizei n, const GLenum * bufs);
struct marshal_cmd_NamedFramebufferReadBuffer;
uint32_t _mesa_unmarshal_NamedFramebufferReadBuffer(struct gl_context *ctx, const struct marshal_cmd_NamedFramebufferReadBuffer *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_NamedFramebufferReadBuffer(GLuint framebuffer, GLenum buf);
struct marshal_cmd_InvalidateNamedFramebufferData;
uint32_t _mesa_unmarshal_InvalidateNamedFramebufferData(struct gl_context *ctx, const struct marshal_cmd_InvalidateNamedFramebufferData *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_InvalidateNamedFramebufferData(GLuint framebuffer, GLsizei numAttachments, const GLenum * attachments);
struct marshal_cmd_InvalidateNamedFramebufferSubData;
uint32_t _mesa_unmarshal_InvalidateNamedFramebufferSubData(struct gl_context *ctx, const struct marshal_cmd_InvalidateNamedFramebufferSubData *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_InvalidateNamedFramebufferSubData(GLuint framebuffer, GLsizei numAttachments, const GLenum * attachments, GLint x, GLint y, GLsizei width, GLsizei height);
struct marshal_cmd_ClearNamedFramebufferiv;
uint32_t _mesa_unmarshal_ClearNamedFramebufferiv(struct gl_context *ctx, const struct marshal_cmd_ClearNamedFramebufferiv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ClearNamedFramebufferiv(GLuint framebuffer, GLenum buffer, GLint drawbuffer, const GLint * value);
struct marshal_cmd_ClearNamedFramebufferuiv;
uint32_t _mesa_unmarshal_ClearNamedFramebufferuiv(struct gl_context *ctx, const struct marshal_cmd_ClearNamedFramebufferuiv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ClearNamedFramebufferuiv(GLuint framebuffer, GLenum buffer, GLint drawbuffer, const GLuint * value);
struct marshal_cmd_ClearNamedFramebufferfv;
uint32_t _mesa_unmarshal_ClearNamedFramebufferfv(struct gl_context *ctx, const struct marshal_cmd_ClearNamedFramebufferfv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ClearNamedFramebufferfv(GLuint framebuffer, GLenum buffer, GLint drawbuffer, const GLfloat * value);
struct marshal_cmd_ClearNamedFramebufferfi;
uint32_t _mesa_unmarshal_ClearNamedFramebufferfi(struct gl_context *ctx, const struct marshal_cmd_ClearNamedFramebufferfi *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ClearNamedFramebufferfi(GLuint framebuffer, GLenum buffer, GLint drawbuffer, GLfloat depth, GLint stencil);
struct marshal_cmd_BlitNamedFramebuffer;
uint32_t _mesa_unmarshal_BlitNamedFramebuffer(struct gl_context *ctx, const struct marshal_cmd_BlitNamedFramebuffer *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_BlitNamedFramebuffer(GLuint readFramebuffer, GLuint drawFramebuffer, GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter);
GLenum GLAPIENTRY _mesa_marshal_CheckNamedFramebufferStatus(GLuint framebuffer, GLenum target);
void GLAPIENTRY _mesa_marshal_GetNamedFramebufferParameteriv(GLuint framebuffer, GLenum pname, GLint * param);
void GLAPIENTRY _mesa_marshal_GetNamedFramebufferAttachmentParameteriv(GLuint framebuffer, GLenum attachment, GLenum pname, GLint * params);
void GLAPIENTRY _mesa_marshal_CreateRenderbuffers(GLsizei n, GLuint * renderbuffers);
struct marshal_cmd_NamedRenderbufferStorage;
uint32_t _mesa_unmarshal_NamedRenderbufferStorage(struct gl_context *ctx, const struct marshal_cmd_NamedRenderbufferStorage *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_NamedRenderbufferStorage(GLuint renderbuffer, GLenum internalformat, GLsizei width, GLsizei height);
struct marshal_cmd_NamedRenderbufferStorageMultisample;
uint32_t _mesa_unmarshal_NamedRenderbufferStorageMultisample(struct gl_context *ctx, const struct marshal_cmd_NamedRenderbufferStorageMultisample *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_NamedRenderbufferStorageMultisample(GLuint renderbuffer, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height);
void GLAPIENTRY _mesa_marshal_GetNamedRenderbufferParameteriv(GLuint renderbuffer, GLenum pname, GLint * params);
void GLAPIENTRY _mesa_marshal_CreateTextures(GLenum target, GLsizei n, GLuint * textures);
struct marshal_cmd_TextureBuffer;
uint32_t _mesa_unmarshal_TextureBuffer(struct gl_context *ctx, const struct marshal_cmd_TextureBuffer *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TextureBuffer(GLuint texture, GLenum internalformat, GLuint buffer);
struct marshal_cmd_TextureBufferRange;
uint32_t _mesa_unmarshal_TextureBufferRange(struct gl_context *ctx, const struct marshal_cmd_TextureBufferRange *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TextureBufferRange(GLuint texture, GLenum internalformat, GLuint buffer, GLintptr offset, GLsizeiptr size);
struct marshal_cmd_TextureStorage1D;
uint32_t _mesa_unmarshal_TextureStorage1D(struct gl_context *ctx, const struct marshal_cmd_TextureStorage1D *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TextureStorage1D(GLuint texture, GLsizei levels, GLenum internalformat, GLsizei width);
struct marshal_cmd_TextureStorage2D;
uint32_t _mesa_unmarshal_TextureStorage2D(struct gl_context *ctx, const struct marshal_cmd_TextureStorage2D *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TextureStorage2D(GLuint texture, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height);
struct marshal_cmd_TextureStorage3D;
uint32_t _mesa_unmarshal_TextureStorage3D(struct gl_context *ctx, const struct marshal_cmd_TextureStorage3D *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TextureStorage3D(GLuint texture, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth);
struct marshal_cmd_TextureStorage2DMultisample;
uint32_t _mesa_unmarshal_TextureStorage2DMultisample(struct gl_context *ctx, const struct marshal_cmd_TextureStorage2DMultisample *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TextureStorage2DMultisample(GLuint texture, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations);
struct marshal_cmd_TextureStorage3DMultisample;
uint32_t _mesa_unmarshal_TextureStorage3DMultisample(struct gl_context *ctx, const struct marshal_cmd_TextureStorage3DMultisample *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TextureStorage3DMultisample(GLuint texture, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations);
struct marshal_cmd_TextureSubImage1D;
uint32_t _mesa_unmarshal_TextureSubImage1D(struct gl_context *ctx, const struct marshal_cmd_TextureSubImage1D *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TextureSubImage1D(GLuint texture, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const GLvoid * pixels);
struct marshal_cmd_TextureSubImage2D;
uint32_t _mesa_unmarshal_TextureSubImage2D(struct gl_context *ctx, const struct marshal_cmd_TextureSubImage2D *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TextureSubImage2D(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid * pixels);
struct marshal_cmd_TextureSubImage3D;
uint32_t _mesa_unmarshal_TextureSubImage3D(struct gl_context *ctx, const struct marshal_cmd_TextureSubImage3D *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TextureSubImage3D(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const GLvoid * pixels);
struct marshal_cmd_CompressedTextureSubImage1D;
uint32_t _mesa_unmarshal_CompressedTextureSubImage1D(struct gl_context *ctx, const struct marshal_cmd_CompressedTextureSubImage1D *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_CompressedTextureSubImage1D(GLuint texture, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const GLvoid * data);
struct marshal_cmd_CompressedTextureSubImage2D;
uint32_t _mesa_unmarshal_CompressedTextureSubImage2D(struct gl_context *ctx, const struct marshal_cmd_CompressedTextureSubImage2D *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_CompressedTextureSubImage2D(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const GLvoid * data);
struct marshal_cmd_CompressedTextureSubImage3D;
uint32_t _mesa_unmarshal_CompressedTextureSubImage3D(struct gl_context *ctx, const struct marshal_cmd_CompressedTextureSubImage3D *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_CompressedTextureSubImage3D(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const GLvoid * data);
struct marshal_cmd_CopyTextureSubImage1D;
uint32_t _mesa_unmarshal_CopyTextureSubImage1D(struct gl_context *ctx, const struct marshal_cmd_CopyTextureSubImage1D *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_CopyTextureSubImage1D(GLuint texture, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width);
struct marshal_cmd_CopyTextureSubImage2D;
uint32_t _mesa_unmarshal_CopyTextureSubImage2D(struct gl_context *ctx, const struct marshal_cmd_CopyTextureSubImage2D *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_CopyTextureSubImage2D(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
struct marshal_cmd_CopyTextureSubImage3D;
uint32_t _mesa_unmarshal_CopyTextureSubImage3D(struct gl_context *ctx, const struct marshal_cmd_CopyTextureSubImage3D *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_CopyTextureSubImage3D(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);
struct marshal_cmd_TextureParameterf;
uint32_t _mesa_unmarshal_TextureParameterf(struct gl_context *ctx, const struct marshal_cmd_TextureParameterf *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TextureParameterf(GLuint texture, GLenum pname, GLfloat param);
struct marshal_cmd_TextureParameterfv;
uint32_t _mesa_unmarshal_TextureParameterfv(struct gl_context *ctx, const struct marshal_cmd_TextureParameterfv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TextureParameterfv(GLuint texture, GLenum pname, const GLfloat * param);
struct marshal_cmd_TextureParameteri;
uint32_t _mesa_unmarshal_TextureParameteri(struct gl_context *ctx, const struct marshal_cmd_TextureParameteri *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TextureParameteri(GLuint texture, GLenum pname, GLint param);
struct marshal_cmd_TextureParameterIiv;
uint32_t _mesa_unmarshal_TextureParameterIiv(struct gl_context *ctx, const struct marshal_cmd_TextureParameterIiv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TextureParameterIiv(GLuint texture, GLenum pname, const GLint * params);
struct marshal_cmd_TextureParameterIuiv;
uint32_t _mesa_unmarshal_TextureParameterIuiv(struct gl_context *ctx, const struct marshal_cmd_TextureParameterIuiv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TextureParameterIuiv(GLuint texture, GLenum pname, const GLuint * params);
struct marshal_cmd_TextureParameteriv;
uint32_t _mesa_unmarshal_TextureParameteriv(struct gl_context *ctx, const struct marshal_cmd_TextureParameteriv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TextureParameteriv(GLuint texture, GLenum pname, const GLint * param);
struct marshal_cmd_GenerateTextureMipmap;
uint32_t _mesa_unmarshal_GenerateTextureMipmap(struct gl_context *ctx, const struct marshal_cmd_GenerateTextureMipmap *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_GenerateTextureMipmap(GLuint texture);
struct marshal_cmd_BindTextureUnit;
uint32_t _mesa_unmarshal_BindTextureUnit(struct gl_context *ctx, const struct marshal_cmd_BindTextureUnit *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_BindTextureUnit(GLuint unit, GLuint texture);
struct marshal_cmd_GetTextureImage;
uint32_t _mesa_unmarshal_GetTextureImage(struct gl_context *ctx, const struct marshal_cmd_GetTextureImage *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_GetTextureImage(GLuint texture, GLint level, GLenum format, GLenum type, GLsizei bufSize, GLvoid * pixels);
struct marshal_cmd_GetCompressedTextureImage;
uint32_t _mesa_unmarshal_GetCompressedTextureImage(struct gl_context *ctx, const struct marshal_cmd_GetCompressedTextureImage *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_GetCompressedTextureImage(GLuint texture, GLint level, GLsizei bufSize, GLvoid * pixels);
void GLAPIENTRY _mesa_marshal_GetTextureLevelParameterfv(GLuint texture, GLint level, GLenum pname, GLfloat * params);
void GLAPIENTRY _mesa_marshal_GetTextureLevelParameteriv(GLuint texture, GLint level, GLenum pname, GLint * params);
void GLAPIENTRY _mesa_marshal_GetTextureParameterfv(GLuint texture, GLenum pname, GLfloat * params);
void GLAPIENTRY _mesa_marshal_GetTextureParameterIiv(GLuint texture, GLenum pname, GLint * params);
void GLAPIENTRY _mesa_marshal_GetTextureParameterIuiv(GLuint texture, GLenum pname, GLuint * params);
void GLAPIENTRY _mesa_marshal_GetTextureParameteriv(GLuint texture, GLenum pname, GLint * params);
void GLAPIENTRY _mesa_marshal_CreateVertexArrays(GLsizei n, GLuint * arrays);
struct marshal_cmd_DisableVertexArrayAttrib;
uint32_t _mesa_unmarshal_DisableVertexArrayAttrib(struct gl_context *ctx, const struct marshal_cmd_DisableVertexArrayAttrib *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_DisableVertexArrayAttrib(GLuint vaobj, GLuint index);
struct marshal_cmd_EnableVertexArrayAttrib;
uint32_t _mesa_unmarshal_EnableVertexArrayAttrib(struct gl_context *ctx, const struct marshal_cmd_EnableVertexArrayAttrib *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_EnableVertexArrayAttrib(GLuint vaobj, GLuint index);
struct marshal_cmd_VertexArrayElementBuffer;
uint32_t _mesa_unmarshal_VertexArrayElementBuffer(struct gl_context *ctx, const struct marshal_cmd_VertexArrayElementBuffer *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexArrayElementBuffer(GLuint vaobj, GLuint buffer);
struct marshal_cmd_VertexArrayVertexBuffer;
uint32_t _mesa_unmarshal_VertexArrayVertexBuffer(struct gl_context *ctx, const struct marshal_cmd_VertexArrayVertexBuffer *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexArrayVertexBuffer(GLuint vaobj, GLuint bindingindex, GLuint buffer, GLintptr offset, GLsizei stride);
struct marshal_cmd_VertexArrayVertexBuffers;
uint32_t _mesa_unmarshal_VertexArrayVertexBuffers(struct gl_context *ctx, const struct marshal_cmd_VertexArrayVertexBuffers *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexArrayVertexBuffers(GLuint vaobj, GLuint first, GLsizei count, const GLuint * buffers, const GLintptr * offsets, const GLsizei * strides);
struct marshal_cmd_VertexArrayAttribFormat;
uint32_t _mesa_unmarshal_VertexArrayAttribFormat(struct gl_context *ctx, const struct marshal_cmd_VertexArrayAttribFormat *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexArrayAttribFormat(GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset);
struct marshal_cmd_VertexArrayAttribIFormat;
uint32_t _mesa_unmarshal_VertexArrayAttribIFormat(struct gl_context *ctx, const struct marshal_cmd_VertexArrayAttribIFormat *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexArrayAttribIFormat(GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset);
struct marshal_cmd_VertexArrayAttribLFormat;
uint32_t _mesa_unmarshal_VertexArrayAttribLFormat(struct gl_context *ctx, const struct marshal_cmd_VertexArrayAttribLFormat *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexArrayAttribLFormat(GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset);
struct marshal_cmd_VertexArrayAttribBinding;
uint32_t _mesa_unmarshal_VertexArrayAttribBinding(struct gl_context *ctx, const struct marshal_cmd_VertexArrayAttribBinding *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexArrayAttribBinding(GLuint vaobj, GLuint attribindex, GLuint bindingindex);
struct marshal_cmd_VertexArrayBindingDivisor;
uint32_t _mesa_unmarshal_VertexArrayBindingDivisor(struct gl_context *ctx, const struct marshal_cmd_VertexArrayBindingDivisor *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexArrayBindingDivisor(GLuint vaobj, GLuint bindingindex, GLuint divisor);
void GLAPIENTRY _mesa_marshal_GetVertexArrayiv(GLuint vaobj, GLenum pname, GLint * param);
void GLAPIENTRY _mesa_marshal_GetVertexArrayIndexediv(GLuint vaobj, GLuint index, GLenum pname, GLint * param);
void GLAPIENTRY _mesa_marshal_GetVertexArrayIndexed64iv(GLuint vaobj, GLuint index, GLenum pname, GLint64 * param);
void GLAPIENTRY _mesa_marshal_CreateSamplers(GLsizei n, GLuint * samplers);
void GLAPIENTRY _mesa_marshal_CreateProgramPipelines(GLsizei n, GLuint * pipelines);
void GLAPIENTRY _mesa_marshal_CreateQueries(GLenum target, GLsizei n, GLuint * ids);
struct marshal_cmd_GetQueryBufferObjectiv;
uint32_t _mesa_unmarshal_GetQueryBufferObjectiv(struct gl_context *ctx, const struct marshal_cmd_GetQueryBufferObjectiv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_GetQueryBufferObjectiv(GLuint id, GLuint buffer, GLenum pname, GLintptr offset);
struct marshal_cmd_GetQueryBufferObjectuiv;
uint32_t _mesa_unmarshal_GetQueryBufferObjectuiv(struct gl_context *ctx, const struct marshal_cmd_GetQueryBufferObjectuiv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_GetQueryBufferObjectuiv(GLuint id, GLuint buffer, GLenum pname, GLintptr offset);
struct marshal_cmd_GetQueryBufferObjecti64v;
uint32_t _mesa_unmarshal_GetQueryBufferObjecti64v(struct gl_context *ctx, const struct marshal_cmd_GetQueryBufferObjecti64v *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_GetQueryBufferObjecti64v(GLuint id, GLuint buffer, GLenum pname, GLintptr offset);
struct marshal_cmd_GetQueryBufferObjectui64v;
uint32_t _mesa_unmarshal_GetQueryBufferObjectui64v(struct gl_context *ctx, const struct marshal_cmd_GetQueryBufferObjectui64v *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_GetQueryBufferObjectui64v(GLuint id, GLuint buffer, GLenum pname, GLintptr offset);
struct marshal_cmd_GetTextureSubImage;
uint32_t _mesa_unmarshal_GetTextureSubImage(struct gl_context *ctx, const struct marshal_cmd_GetTextureSubImage *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_GetTextureSubImage(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, GLsizei bufSize, GLvoid * pixels);
struct marshal_cmd_GetCompressedTextureSubImage;
uint32_t _mesa_unmarshal_GetCompressedTextureSubImage(struct gl_context *ctx, const struct marshal_cmd_GetCompressedTextureSubImage *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_GetCompressedTextureSubImage(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLsizei bufSize, GLvoid * pixels);
struct marshal_cmd_TextureBarrierNV;
uint32_t _mesa_unmarshal_TextureBarrierNV(struct gl_context *ctx, const struct marshal_cmd_TextureBarrierNV *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TextureBarrierNV(void);
struct marshal_cmd_BufferPageCommitmentARB;
uint32_t _mesa_unmarshal_BufferPageCommitmentARB(struct gl_context *ctx, const struct marshal_cmd_BufferPageCommitmentARB *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_BufferPageCommitmentARB(GLenum target, GLintptr offset, GLsizeiptr size, GLboolean commit);
struct marshal_cmd_NamedBufferPageCommitmentEXT;
uint32_t _mesa_unmarshal_NamedBufferPageCommitmentEXT(struct gl_context *ctx, const struct marshal_cmd_NamedBufferPageCommitmentEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_NamedBufferPageCommitmentEXT(GLuint buffer, GLintptr offset, GLsizeiptr size, GLboolean commit);
struct marshal_cmd_NamedBufferPageCommitmentARB;
uint32_t _mesa_unmarshal_NamedBufferPageCommitmentARB(struct gl_context *ctx, const struct marshal_cmd_NamedBufferPageCommitmentARB *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_NamedBufferPageCommitmentARB(GLuint buffer, GLintptr offset, GLsizeiptr size, GLboolean commit);
struct marshal_cmd_PrimitiveBoundingBox;
uint32_t _mesa_unmarshal_PrimitiveBoundingBox(struct gl_context *ctx, const struct marshal_cmd_PrimitiveBoundingBox *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_PrimitiveBoundingBox(GLfloat minX, GLfloat minY, GLfloat minZ, GLfloat minW, GLfloat maxX, GLfloat maxY, GLfloat maxZ, GLfloat maxW);
struct marshal_cmd_BlendBarrier;
uint32_t _mesa_unmarshal_BlendBarrier(struct gl_context *ctx, const struct marshal_cmd_BlendBarrier *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_BlendBarrier(void);
struct marshal_cmd_Uniform1i64ARB;
uint32_t _mesa_unmarshal_Uniform1i64ARB(struct gl_context *ctx, const struct marshal_cmd_Uniform1i64ARB *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Uniform1i64ARB(GLint location, GLint64 x);
struct marshal_cmd_Uniform2i64ARB;
uint32_t _mesa_unmarshal_Uniform2i64ARB(struct gl_context *ctx, const struct marshal_cmd_Uniform2i64ARB *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Uniform2i64ARB(GLint location, GLint64 x, GLint64 y);
struct marshal_cmd_Uniform3i64ARB;
uint32_t _mesa_unmarshal_Uniform3i64ARB(struct gl_context *ctx, const struct marshal_cmd_Uniform3i64ARB *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Uniform3i64ARB(GLint location, GLint64 x, GLint64 y, GLint64 z);
struct marshal_cmd_Uniform4i64ARB;
uint32_t _mesa_unmarshal_Uniform4i64ARB(struct gl_context *ctx, const struct marshal_cmd_Uniform4i64ARB *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Uniform4i64ARB(GLint location, GLint64 x, GLint64 y, GLint64 z, GLint64 w);
struct marshal_cmd_Uniform1i64vARB;
uint32_t _mesa_unmarshal_Uniform1i64vARB(struct gl_context *ctx, const struct marshal_cmd_Uniform1i64vARB *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Uniform1i64vARB(GLint location, GLsizei count, const GLint64 * value);
struct marshal_cmd_Uniform2i64vARB;
uint32_t _mesa_unmarshal_Uniform2i64vARB(struct gl_context *ctx, const struct marshal_cmd_Uniform2i64vARB *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Uniform2i64vARB(GLint location, GLsizei count, const GLint64 * value);
struct marshal_cmd_Uniform3i64vARB;
uint32_t _mesa_unmarshal_Uniform3i64vARB(struct gl_context *ctx, const struct marshal_cmd_Uniform3i64vARB *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Uniform3i64vARB(GLint location, GLsizei count, const GLint64 * value);
struct marshal_cmd_Uniform4i64vARB;
uint32_t _mesa_unmarshal_Uniform4i64vARB(struct gl_context *ctx, const struct marshal_cmd_Uniform4i64vARB *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Uniform4i64vARB(GLint location, GLsizei count, const GLint64 * value);
struct marshal_cmd_Uniform1ui64ARB;
uint32_t _mesa_unmarshal_Uniform1ui64ARB(struct gl_context *ctx, const struct marshal_cmd_Uniform1ui64ARB *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Uniform1ui64ARB(GLint location, GLuint64 x);
struct marshal_cmd_Uniform2ui64ARB;
uint32_t _mesa_unmarshal_Uniform2ui64ARB(struct gl_context *ctx, const struct marshal_cmd_Uniform2ui64ARB *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Uniform2ui64ARB(GLint location, GLuint64 x, GLuint64 y);
struct marshal_cmd_Uniform3ui64ARB;
uint32_t _mesa_unmarshal_Uniform3ui64ARB(struct gl_context *ctx, const struct marshal_cmd_Uniform3ui64ARB *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Uniform3ui64ARB(GLint location, GLuint64 x, GLuint64 y, GLuint64 z);
struct marshal_cmd_Uniform4ui64ARB;
uint32_t _mesa_unmarshal_Uniform4ui64ARB(struct gl_context *ctx, const struct marshal_cmd_Uniform4ui64ARB *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Uniform4ui64ARB(GLint location, GLuint64 x, GLuint64 y, GLuint64 z, GLuint64 w);
struct marshal_cmd_Uniform1ui64vARB;
uint32_t _mesa_unmarshal_Uniform1ui64vARB(struct gl_context *ctx, const struct marshal_cmd_Uniform1ui64vARB *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Uniform1ui64vARB(GLint location, GLsizei count, const GLuint64 * value);
struct marshal_cmd_Uniform2ui64vARB;
uint32_t _mesa_unmarshal_Uniform2ui64vARB(struct gl_context *ctx, const struct marshal_cmd_Uniform2ui64vARB *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Uniform2ui64vARB(GLint location, GLsizei count, const GLuint64 * value);
struct marshal_cmd_Uniform3ui64vARB;
uint32_t _mesa_unmarshal_Uniform3ui64vARB(struct gl_context *ctx, const struct marshal_cmd_Uniform3ui64vARB *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Uniform3ui64vARB(GLint location, GLsizei count, const GLuint64 * value);
struct marshal_cmd_Uniform4ui64vARB;
uint32_t _mesa_unmarshal_Uniform4ui64vARB(struct gl_context *ctx, const struct marshal_cmd_Uniform4ui64vARB *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Uniform4ui64vARB(GLint location, GLsizei count, const GLuint64 * value);
void GLAPIENTRY _mesa_marshal_GetUniformi64vARB(GLuint program, GLint location, GLint64 * params);
void GLAPIENTRY _mesa_marshal_GetUniformui64vARB(GLuint program, GLint location, GLuint64 * params);
void GLAPIENTRY _mesa_marshal_GetnUniformi64vARB(GLuint program, GLint location, GLsizei bufSize, GLint64 * params);
void GLAPIENTRY _mesa_marshal_GetnUniformui64vARB(GLuint program, GLint location, GLsizei bufSize, GLuint64 * params);
struct marshal_cmd_ProgramUniform1i64ARB;
uint32_t _mesa_unmarshal_ProgramUniform1i64ARB(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform1i64ARB *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ProgramUniform1i64ARB(GLuint program, GLint location, GLint64 x);
struct marshal_cmd_ProgramUniform2i64ARB;
uint32_t _mesa_unmarshal_ProgramUniform2i64ARB(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform2i64ARB *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ProgramUniform2i64ARB(GLuint program, GLint location, GLint64 x, GLint64 y);
struct marshal_cmd_ProgramUniform3i64ARB;
uint32_t _mesa_unmarshal_ProgramUniform3i64ARB(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform3i64ARB *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ProgramUniform3i64ARB(GLuint program, GLint location, GLint64 x, GLint64 y, GLint64 z);
struct marshal_cmd_ProgramUniform4i64ARB;
uint32_t _mesa_unmarshal_ProgramUniform4i64ARB(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform4i64ARB *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ProgramUniform4i64ARB(GLuint program, GLint location, GLint64 x, GLint64 y, GLint64 z, GLint64 w);
struct marshal_cmd_ProgramUniform1i64vARB;
uint32_t _mesa_unmarshal_ProgramUniform1i64vARB(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform1i64vARB *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ProgramUniform1i64vARB(GLuint program, GLint location, GLsizei count, const GLint64 * value);
struct marshal_cmd_ProgramUniform2i64vARB;
uint32_t _mesa_unmarshal_ProgramUniform2i64vARB(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform2i64vARB *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ProgramUniform2i64vARB(GLuint program, GLint location, GLsizei count, const GLint64 * value);
struct marshal_cmd_ProgramUniform3i64vARB;
uint32_t _mesa_unmarshal_ProgramUniform3i64vARB(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform3i64vARB *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ProgramUniform3i64vARB(GLuint program, GLint location, GLsizei count, const GLint64 * value);
struct marshal_cmd_ProgramUniform4i64vARB;
uint32_t _mesa_unmarshal_ProgramUniform4i64vARB(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform4i64vARB *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ProgramUniform4i64vARB(GLuint program, GLint location, GLsizei count, const GLint64 * value);
struct marshal_cmd_ProgramUniform1ui64ARB;
uint32_t _mesa_unmarshal_ProgramUniform1ui64ARB(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform1ui64ARB *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ProgramUniform1ui64ARB(GLuint program, GLint location, GLuint64 x);
struct marshal_cmd_ProgramUniform2ui64ARB;
uint32_t _mesa_unmarshal_ProgramUniform2ui64ARB(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform2ui64ARB *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ProgramUniform2ui64ARB(GLuint program, GLint location, GLuint64 x, GLuint64 y);
struct marshal_cmd_ProgramUniform3ui64ARB;
uint32_t _mesa_unmarshal_ProgramUniform3ui64ARB(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform3ui64ARB *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ProgramUniform3ui64ARB(GLuint program, GLint location, GLuint64 x, GLuint64 y, GLuint64 z);
struct marshal_cmd_ProgramUniform4ui64ARB;
uint32_t _mesa_unmarshal_ProgramUniform4ui64ARB(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform4ui64ARB *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ProgramUniform4ui64ARB(GLuint program, GLint location, GLuint64 x, GLuint64 y, GLuint64 z, GLuint64 w);
struct marshal_cmd_ProgramUniform1ui64vARB;
uint32_t _mesa_unmarshal_ProgramUniform1ui64vARB(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform1ui64vARB *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ProgramUniform1ui64vARB(GLuint program, GLint location, GLsizei count, const GLuint64 * value);
struct marshal_cmd_ProgramUniform2ui64vARB;
uint32_t _mesa_unmarshal_ProgramUniform2ui64vARB(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform2ui64vARB *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ProgramUniform2ui64vARB(GLuint program, GLint location, GLsizei count, const GLuint64 * value);
struct marshal_cmd_ProgramUniform3ui64vARB;
uint32_t _mesa_unmarshal_ProgramUniform3ui64vARB(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform3ui64vARB *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ProgramUniform3ui64vARB(GLuint program, GLint location, GLsizei count, const GLuint64 * value);
struct marshal_cmd_ProgramUniform4ui64vARB;
uint32_t _mesa_unmarshal_ProgramUniform4ui64vARB(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform4ui64vARB *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ProgramUniform4ui64vARB(GLuint program, GLint location, GLsizei count, const GLuint64 * value);
struct marshal_cmd_MaxShaderCompilerThreadsKHR;
uint32_t _mesa_unmarshal_MaxShaderCompilerThreadsKHR(struct gl_context *ctx, const struct marshal_cmd_MaxShaderCompilerThreadsKHR *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_MaxShaderCompilerThreadsKHR(GLuint count);
struct marshal_cmd_SpecializeShaderARB;
uint32_t _mesa_unmarshal_SpecializeShaderARB(struct gl_context *ctx, const struct marshal_cmd_SpecializeShaderARB *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_SpecializeShaderARB(GLuint shader, const GLchar * pEntryPoint, GLuint numSpecializationConstants, const GLuint * pConstantIndex, const GLuint * pConstantValue);
struct marshal_cmd_ColorPointerEXT;
uint32_t _mesa_unmarshal_ColorPointerEXT(struct gl_context *ctx, const struct marshal_cmd_ColorPointerEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ColorPointerEXT(GLint size, GLenum type, GLsizei stride, GLsizei count, const GLvoid * pointer);
struct marshal_cmd_EdgeFlagPointerEXT;
uint32_t _mesa_unmarshal_EdgeFlagPointerEXT(struct gl_context *ctx, const struct marshal_cmd_EdgeFlagPointerEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_EdgeFlagPointerEXT(GLsizei stride, GLsizei count, const GLboolean * pointer);
struct marshal_cmd_IndexPointerEXT;
uint32_t _mesa_unmarshal_IndexPointerEXT(struct gl_context *ctx, const struct marshal_cmd_IndexPointerEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_IndexPointerEXT(GLenum type, GLsizei stride, GLsizei count, const GLvoid * pointer);
struct marshal_cmd_NormalPointerEXT;
uint32_t _mesa_unmarshal_NormalPointerEXT(struct gl_context *ctx, const struct marshal_cmd_NormalPointerEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_NormalPointerEXT(GLenum type, GLsizei stride, GLsizei count, const GLvoid * pointer);
struct marshal_cmd_TexCoordPointerEXT;
uint32_t _mesa_unmarshal_TexCoordPointerEXT(struct gl_context *ctx, const struct marshal_cmd_TexCoordPointerEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TexCoordPointerEXT(GLint size, GLenum type, GLsizei stride, GLsizei count, const GLvoid * pointer);
struct marshal_cmd_VertexPointerEXT;
uint32_t _mesa_unmarshal_VertexPointerEXT(struct gl_context *ctx, const struct marshal_cmd_VertexPointerEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexPointerEXT(GLint size, GLenum type, GLsizei stride, GLsizei count, const GLvoid * pointer);
struct marshal_cmd_LockArraysEXT;
uint32_t _mesa_unmarshal_LockArraysEXT(struct gl_context *ctx, const struct marshal_cmd_LockArraysEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_LockArraysEXT(GLint first, GLsizei count);
struct marshal_cmd_UnlockArraysEXT;
uint32_t _mesa_unmarshal_UnlockArraysEXT(struct gl_context *ctx, const struct marshal_cmd_UnlockArraysEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_UnlockArraysEXT(void);
struct marshal_cmd_ViewportArrayv;
uint32_t _mesa_unmarshal_ViewportArrayv(struct gl_context *ctx, const struct marshal_cmd_ViewportArrayv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ViewportArrayv(GLuint first, GLsizei count, const GLfloat * v);
struct marshal_cmd_ViewportIndexedf;
uint32_t _mesa_unmarshal_ViewportIndexedf(struct gl_context *ctx, const struct marshal_cmd_ViewportIndexedf *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ViewportIndexedf(GLuint index, GLfloat x, GLfloat y, GLfloat w, GLfloat h);
struct marshal_cmd_ViewportIndexedfv;
uint32_t _mesa_unmarshal_ViewportIndexedfv(struct gl_context *ctx, const struct marshal_cmd_ViewportIndexedfv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ViewportIndexedfv(GLuint index, const GLfloat * v);
struct marshal_cmd_ScissorArrayv;
uint32_t _mesa_unmarshal_ScissorArrayv(struct gl_context *ctx, const struct marshal_cmd_ScissorArrayv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ScissorArrayv(GLuint first, GLsizei count, const int * v);
struct marshal_cmd_ScissorIndexed;
uint32_t _mesa_unmarshal_ScissorIndexed(struct gl_context *ctx, const struct marshal_cmd_ScissorIndexed *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ScissorIndexed(GLuint index, GLint left, GLint bottom, GLsizei width, GLsizei height);
struct marshal_cmd_ScissorIndexedv;
uint32_t _mesa_unmarshal_ScissorIndexedv(struct gl_context *ctx, const struct marshal_cmd_ScissorIndexedv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ScissorIndexedv(GLuint index, const GLint * v);
struct marshal_cmd_DepthRangeArrayv;
uint32_t _mesa_unmarshal_DepthRangeArrayv(struct gl_context *ctx, const struct marshal_cmd_DepthRangeArrayv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_DepthRangeArrayv(GLuint first, GLsizei count, const GLclampd * v);
struct marshal_cmd_DepthRangeIndexed;
uint32_t _mesa_unmarshal_DepthRangeIndexed(struct gl_context *ctx, const struct marshal_cmd_DepthRangeIndexed *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_DepthRangeIndexed(GLuint index, GLclampd n, GLclampd f);
void GLAPIENTRY _mesa_marshal_GetFloati_v(GLenum target, GLuint index, GLfloat * data);
void GLAPIENTRY _mesa_marshal_GetDoublei_v(GLenum target, GLuint index, GLdouble * data);
struct marshal_cmd_FramebufferSampleLocationsfvARB;
uint32_t _mesa_unmarshal_FramebufferSampleLocationsfvARB(struct gl_context *ctx, const struct marshal_cmd_FramebufferSampleLocationsfvARB *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_FramebufferSampleLocationsfvARB(GLenum target, GLuint start, GLsizei count, const GLfloat * v);
struct marshal_cmd_NamedFramebufferSampleLocationsfvARB;
uint32_t _mesa_unmarshal_NamedFramebufferSampleLocationsfvARB(struct gl_context *ctx, const struct marshal_cmd_NamedFramebufferSampleLocationsfvARB *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_NamedFramebufferSampleLocationsfvARB(GLuint framebuffer, GLuint start, GLsizei count, const GLfloat * v);
struct marshal_cmd_EvaluateDepthValuesARB;
uint32_t _mesa_unmarshal_EvaluateDepthValuesARB(struct gl_context *ctx, const struct marshal_cmd_EvaluateDepthValuesARB *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_EvaluateDepthValuesARB(void);
struct marshal_cmd_WindowPos4dMESA;
uint32_t _mesa_unmarshal_WindowPos4dMESA(struct gl_context *ctx, const struct marshal_cmd_WindowPos4dMESA *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_WindowPos4dMESA(GLdouble x, GLdouble y, GLdouble z, GLdouble w);
struct marshal_cmd_WindowPos4dvMESA;
uint32_t _mesa_unmarshal_WindowPos4dvMESA(struct gl_context *ctx, const struct marshal_cmd_WindowPos4dvMESA *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_WindowPos4dvMESA(const GLdouble * v);
struct marshal_cmd_WindowPos4fMESA;
uint32_t _mesa_unmarshal_WindowPos4fMESA(struct gl_context *ctx, const struct marshal_cmd_WindowPos4fMESA *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_WindowPos4fMESA(GLfloat x, GLfloat y, GLfloat z, GLfloat w);
struct marshal_cmd_WindowPos4fvMESA;
uint32_t _mesa_unmarshal_WindowPos4fvMESA(struct gl_context *ctx, const struct marshal_cmd_WindowPos4fvMESA *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_WindowPos4fvMESA(const GLfloat * v);
struct marshal_cmd_WindowPos4iMESA;
uint32_t _mesa_unmarshal_WindowPos4iMESA(struct gl_context *ctx, const struct marshal_cmd_WindowPos4iMESA *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_WindowPos4iMESA(GLint x, GLint y, GLint z, GLint w);
struct marshal_cmd_WindowPos4ivMESA;
uint32_t _mesa_unmarshal_WindowPos4ivMESA(struct gl_context *ctx, const struct marshal_cmd_WindowPos4ivMESA *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_WindowPos4ivMESA(const GLint * v);
struct marshal_cmd_WindowPos4sMESA;
uint32_t _mesa_unmarshal_WindowPos4sMESA(struct gl_context *ctx, const struct marshal_cmd_WindowPos4sMESA *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_WindowPos4sMESA(GLshort x, GLshort y, GLshort z, GLshort w);
struct marshal_cmd_WindowPos4svMESA;
uint32_t _mesa_unmarshal_WindowPos4svMESA(struct gl_context *ctx, const struct marshal_cmd_WindowPos4svMESA *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_WindowPos4svMESA(const GLshort * v);
struct marshal_cmd_MultiModeDrawArraysIBM;
uint32_t _mesa_unmarshal_MultiModeDrawArraysIBM(struct gl_context *ctx, const struct marshal_cmd_MultiModeDrawArraysIBM *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_MultiModeDrawArraysIBM(const GLenum * mode, const GLint * first, const GLsizei * count, GLsizei primcount, GLint modestride);
struct marshal_cmd_MultiModeDrawElementsIBM;
uint32_t _mesa_unmarshal_MultiModeDrawElementsIBM(struct gl_context *ctx, const struct marshal_cmd_MultiModeDrawElementsIBM *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_MultiModeDrawElementsIBM(const GLenum * mode, const GLsizei * count, GLenum type, const GLvoid * const * indices, GLsizei primcount, GLint modestride);
struct marshal_cmd_VertexAttrib1sNV;
uint32_t _mesa_unmarshal_VertexAttrib1sNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib1sNV *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttrib1sNV(GLuint index, GLshort x);
struct marshal_cmd_VertexAttrib1svNV;
uint32_t _mesa_unmarshal_VertexAttrib1svNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib1svNV *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttrib1svNV(GLuint index, const GLshort * v);
struct marshal_cmd_VertexAttrib2sNV;
uint32_t _mesa_unmarshal_VertexAttrib2sNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib2sNV *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttrib2sNV(GLuint index, GLshort x, GLshort y);
struct marshal_cmd_VertexAttrib2svNV;
uint32_t _mesa_unmarshal_VertexAttrib2svNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib2svNV *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttrib2svNV(GLuint index, const GLshort * v);
struct marshal_cmd_VertexAttrib3sNV;
uint32_t _mesa_unmarshal_VertexAttrib3sNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib3sNV *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttrib3sNV(GLuint index, GLshort x, GLshort y, GLshort z);
struct marshal_cmd_VertexAttrib3svNV;
uint32_t _mesa_unmarshal_VertexAttrib3svNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib3svNV *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttrib3svNV(GLuint index, const GLshort * v);
struct marshal_cmd_VertexAttrib4sNV;
uint32_t _mesa_unmarshal_VertexAttrib4sNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4sNV *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttrib4sNV(GLuint index, GLshort x, GLshort y, GLshort z, GLshort w);
struct marshal_cmd_VertexAttrib4svNV;
uint32_t _mesa_unmarshal_VertexAttrib4svNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4svNV *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttrib4svNV(GLuint index, const GLshort * v);
struct marshal_cmd_VertexAttrib1fNV;
uint32_t _mesa_unmarshal_VertexAttrib1fNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib1fNV *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttrib1fNV(GLuint index, GLfloat x);
struct marshal_cmd_VertexAttrib1fvNV;
uint32_t _mesa_unmarshal_VertexAttrib1fvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib1fvNV *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttrib1fvNV(GLuint index, const GLfloat * v);
struct marshal_cmd_VertexAttrib2fNV;
uint32_t _mesa_unmarshal_VertexAttrib2fNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib2fNV *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttrib2fNV(GLuint index, GLfloat x, GLfloat y);
struct marshal_cmd_VertexAttrib2fvNV;
uint32_t _mesa_unmarshal_VertexAttrib2fvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib2fvNV *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttrib2fvNV(GLuint index, const GLfloat * v);
struct marshal_cmd_VertexAttrib3fNV;
uint32_t _mesa_unmarshal_VertexAttrib3fNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib3fNV *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttrib3fNV(GLuint index, GLfloat x, GLfloat y, GLfloat z);
struct marshal_cmd_VertexAttrib3fvNV;
uint32_t _mesa_unmarshal_VertexAttrib3fvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib3fvNV *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttrib3fvNV(GLuint index, const GLfloat * v);
struct marshal_cmd_VertexAttrib4fNV;
uint32_t _mesa_unmarshal_VertexAttrib4fNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4fNV *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttrib4fNV(GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
struct marshal_cmd_VertexAttrib4fvNV;
uint32_t _mesa_unmarshal_VertexAttrib4fvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4fvNV *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttrib4fvNV(GLuint index, const GLfloat * v);
struct marshal_cmd_VertexAttrib1dNV;
uint32_t _mesa_unmarshal_VertexAttrib1dNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib1dNV *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttrib1dNV(GLuint index, GLdouble x);
struct marshal_cmd_VertexAttrib1dvNV;
uint32_t _mesa_unmarshal_VertexAttrib1dvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib1dvNV *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttrib1dvNV(GLuint index, const GLdouble * v);
struct marshal_cmd_VertexAttrib2dNV;
uint32_t _mesa_unmarshal_VertexAttrib2dNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib2dNV *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttrib2dNV(GLuint index, GLdouble x, GLdouble y);
struct marshal_cmd_VertexAttrib2dvNV;
uint32_t _mesa_unmarshal_VertexAttrib2dvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib2dvNV *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttrib2dvNV(GLuint index, const GLdouble * v);
struct marshal_cmd_VertexAttrib3dNV;
uint32_t _mesa_unmarshal_VertexAttrib3dNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib3dNV *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttrib3dNV(GLuint index, GLdouble x, GLdouble y, GLdouble z);
struct marshal_cmd_VertexAttrib3dvNV;
uint32_t _mesa_unmarshal_VertexAttrib3dvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib3dvNV *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttrib3dvNV(GLuint index, const GLdouble * v);
struct marshal_cmd_VertexAttrib4dNV;
uint32_t _mesa_unmarshal_VertexAttrib4dNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4dNV *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttrib4dNV(GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
struct marshal_cmd_VertexAttrib4dvNV;
uint32_t _mesa_unmarshal_VertexAttrib4dvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4dvNV *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttrib4dvNV(GLuint index, const GLdouble * v);
struct marshal_cmd_VertexAttrib4ubNV;
uint32_t _mesa_unmarshal_VertexAttrib4ubNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4ubNV *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttrib4ubNV(GLuint index, GLubyte x, GLubyte y, GLubyte z, GLubyte w);
struct marshal_cmd_VertexAttrib4ubvNV;
uint32_t _mesa_unmarshal_VertexAttrib4ubvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4ubvNV *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttrib4ubvNV(GLuint index, const GLubyte * v);
struct marshal_cmd_VertexAttribs1svNV;
uint32_t _mesa_unmarshal_VertexAttribs1svNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttribs1svNV *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttribs1svNV(GLuint index, GLsizei n, const GLshort * v);
struct marshal_cmd_VertexAttribs2svNV;
uint32_t _mesa_unmarshal_VertexAttribs2svNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttribs2svNV *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttribs2svNV(GLuint index, GLsizei n, const GLshort * v);
struct marshal_cmd_VertexAttribs3svNV;
uint32_t _mesa_unmarshal_VertexAttribs3svNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttribs3svNV *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttribs3svNV(GLuint index, GLsizei n, const GLshort * v);
struct marshal_cmd_VertexAttribs4svNV;
uint32_t _mesa_unmarshal_VertexAttribs4svNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttribs4svNV *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttribs4svNV(GLuint index, GLsizei n, const GLshort * v);
struct marshal_cmd_VertexAttribs1fvNV;
uint32_t _mesa_unmarshal_VertexAttribs1fvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttribs1fvNV *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttribs1fvNV(GLuint index, GLsizei n, const GLfloat * v);
struct marshal_cmd_VertexAttribs2fvNV;
uint32_t _mesa_unmarshal_VertexAttribs2fvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttribs2fvNV *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttribs2fvNV(GLuint index, GLsizei n, const GLfloat * v);
struct marshal_cmd_VertexAttribs3fvNV;
uint32_t _mesa_unmarshal_VertexAttribs3fvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttribs3fvNV *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttribs3fvNV(GLuint index, GLsizei n, const GLfloat * v);
struct marshal_cmd_VertexAttribs4fvNV;
uint32_t _mesa_unmarshal_VertexAttribs4fvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttribs4fvNV *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttribs4fvNV(GLuint index, GLsizei n, const GLfloat * v);
struct marshal_cmd_VertexAttribs1dvNV;
uint32_t _mesa_unmarshal_VertexAttribs1dvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttribs1dvNV *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttribs1dvNV(GLuint index, GLsizei n, const GLdouble * v);
struct marshal_cmd_VertexAttribs2dvNV;
uint32_t _mesa_unmarshal_VertexAttribs2dvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttribs2dvNV *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttribs2dvNV(GLuint index, GLsizei n, const GLdouble * v);
struct marshal_cmd_VertexAttribs3dvNV;
uint32_t _mesa_unmarshal_VertexAttribs3dvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttribs3dvNV *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttribs3dvNV(GLuint index, GLsizei n, const GLdouble * v);
struct marshal_cmd_VertexAttribs4dvNV;
uint32_t _mesa_unmarshal_VertexAttribs4dvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttribs4dvNV *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttribs4dvNV(GLuint index, GLsizei n, const GLdouble * v);
struct marshal_cmd_VertexAttribs4ubvNV;
uint32_t _mesa_unmarshal_VertexAttribs4ubvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttribs4ubvNV *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttribs4ubvNV(GLuint index, GLsizei n, const GLubyte * v);
GLuint GLAPIENTRY _mesa_marshal_GenFragmentShadersATI(GLuint range);
struct marshal_cmd_BindFragmentShaderATI;
uint32_t _mesa_unmarshal_BindFragmentShaderATI(struct gl_context *ctx, const struct marshal_cmd_BindFragmentShaderATI *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_BindFragmentShaderATI(GLuint id);
struct marshal_cmd_DeleteFragmentShaderATI;
uint32_t _mesa_unmarshal_DeleteFragmentShaderATI(struct gl_context *ctx, const struct marshal_cmd_DeleteFragmentShaderATI *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_DeleteFragmentShaderATI(GLuint id);
struct marshal_cmd_BeginFragmentShaderATI;
uint32_t _mesa_unmarshal_BeginFragmentShaderATI(struct gl_context *ctx, const struct marshal_cmd_BeginFragmentShaderATI *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_BeginFragmentShaderATI(void);
struct marshal_cmd_EndFragmentShaderATI;
uint32_t _mesa_unmarshal_EndFragmentShaderATI(struct gl_context *ctx, const struct marshal_cmd_EndFragmentShaderATI *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_EndFragmentShaderATI(void);
struct marshal_cmd_PassTexCoordATI;
uint32_t _mesa_unmarshal_PassTexCoordATI(struct gl_context *ctx, const struct marshal_cmd_PassTexCoordATI *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_PassTexCoordATI(GLuint dst, GLuint coord, GLenum swizzle);
struct marshal_cmd_SampleMapATI;
uint32_t _mesa_unmarshal_SampleMapATI(struct gl_context *ctx, const struct marshal_cmd_SampleMapATI *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_SampleMapATI(GLuint dst, GLuint interp, GLenum swizzle);
struct marshal_cmd_ColorFragmentOp1ATI;
uint32_t _mesa_unmarshal_ColorFragmentOp1ATI(struct gl_context *ctx, const struct marshal_cmd_ColorFragmentOp1ATI *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ColorFragmentOp1ATI(GLenum op, GLuint dst, GLuint dstMask, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod);
struct marshal_cmd_ColorFragmentOp2ATI;
uint32_t _mesa_unmarshal_ColorFragmentOp2ATI(struct gl_context *ctx, const struct marshal_cmd_ColorFragmentOp2ATI *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ColorFragmentOp2ATI(GLenum op, GLuint dst, GLuint dstMask, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod, GLuint arg2, GLuint arg2Rep, GLuint arg2Mod);
struct marshal_cmd_ColorFragmentOp3ATI;
uint32_t _mesa_unmarshal_ColorFragmentOp3ATI(struct gl_context *ctx, const struct marshal_cmd_ColorFragmentOp3ATI *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ColorFragmentOp3ATI(GLenum op, GLuint dst, GLuint dstMask, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod, GLuint arg2, GLuint arg2Rep, GLuint arg2Mod, GLuint arg3, GLuint arg3Rep, GLuint arg3Mod);
struct marshal_cmd_AlphaFragmentOp1ATI;
uint32_t _mesa_unmarshal_AlphaFragmentOp1ATI(struct gl_context *ctx, const struct marshal_cmd_AlphaFragmentOp1ATI *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_AlphaFragmentOp1ATI(GLenum op, GLuint dst, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod);
struct marshal_cmd_AlphaFragmentOp2ATI;
uint32_t _mesa_unmarshal_AlphaFragmentOp2ATI(struct gl_context *ctx, const struct marshal_cmd_AlphaFragmentOp2ATI *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_AlphaFragmentOp2ATI(GLenum op, GLuint dst, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod, GLuint arg2, GLuint arg2Rep, GLuint arg2Mod);
struct marshal_cmd_AlphaFragmentOp3ATI;
uint32_t _mesa_unmarshal_AlphaFragmentOp3ATI(struct gl_context *ctx, const struct marshal_cmd_AlphaFragmentOp3ATI *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_AlphaFragmentOp3ATI(GLenum op, GLuint dst, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod, GLuint arg2, GLuint arg2Rep, GLuint arg2Mod, GLuint arg3, GLuint arg3Rep, GLuint arg3Mod);
struct marshal_cmd_SetFragmentShaderConstantATI;
uint32_t _mesa_unmarshal_SetFragmentShaderConstantATI(struct gl_context *ctx, const struct marshal_cmd_SetFragmentShaderConstantATI *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_SetFragmentShaderConstantATI(GLuint dst, const GLfloat * value);
struct marshal_cmd_ActiveStencilFaceEXT;
uint32_t _mesa_unmarshal_ActiveStencilFaceEXT(struct gl_context *ctx, const struct marshal_cmd_ActiveStencilFaceEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ActiveStencilFaceEXT(GLenum face);
GLenum GLAPIENTRY _mesa_marshal_ObjectPurgeableAPPLE(GLenum objectType, GLuint name, GLenum option);
GLenum GLAPIENTRY _mesa_marshal_ObjectUnpurgeableAPPLE(GLenum objectType, GLuint name, GLenum option);
void GLAPIENTRY _mesa_marshal_GetObjectParameterivAPPLE(GLenum objectType, GLuint name, GLenum pname, GLint * value);
struct marshal_cmd_DepthBoundsEXT;
uint32_t _mesa_unmarshal_DepthBoundsEXT(struct gl_context *ctx, const struct marshal_cmd_DepthBoundsEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_DepthBoundsEXT(GLclampd zmin, GLclampd zmax);
struct marshal_cmd_BindRenderbufferEXT;
uint32_t _mesa_unmarshal_BindRenderbufferEXT(struct gl_context *ctx, const struct marshal_cmd_BindRenderbufferEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_BindRenderbufferEXT(GLenum target, GLuint renderbuffer);
struct marshal_cmd_BindFramebufferEXT;
uint32_t _mesa_unmarshal_BindFramebufferEXT(struct gl_context *ctx, const struct marshal_cmd_BindFramebufferEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_BindFramebufferEXT(GLenum target, GLuint framebuffer);
void GLAPIENTRY _mesa_marshal_StringMarkerGREMEDY(GLsizei len, const GLvoid * string);
struct marshal_cmd_ProvokingVertex;
uint32_t _mesa_unmarshal_ProvokingVertex(struct gl_context *ctx, const struct marshal_cmd_ProvokingVertex *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ProvokingVertex(GLenum mode);
struct marshal_cmd_ColorMaski;
uint32_t _mesa_unmarshal_ColorMaski(struct gl_context *ctx, const struct marshal_cmd_ColorMaski *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ColorMaski(GLuint buf, GLboolean r, GLboolean g, GLboolean b, GLboolean a);
void GLAPIENTRY _mesa_marshal_GetBooleani_v(GLenum value, GLuint index, GLboolean * data);
void GLAPIENTRY _mesa_marshal_GetIntegeri_v(GLenum value, GLuint index, GLint * data);
struct marshal_cmd_Enablei;
uint32_t _mesa_unmarshal_Enablei(struct gl_context *ctx, const struct marshal_cmd_Enablei *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Enablei(GLenum target, GLuint index);
struct marshal_cmd_Disablei;
uint32_t _mesa_unmarshal_Disablei(struct gl_context *ctx, const struct marshal_cmd_Disablei *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Disablei(GLenum target, GLuint index);
GLboolean GLAPIENTRY _mesa_marshal_IsEnabledi(GLenum target, GLuint index);
void GLAPIENTRY _mesa_marshal_GetPerfMonitorGroupsAMD(GLint * numGroups, GLsizei groupsSize, GLuint * groups);
void GLAPIENTRY _mesa_marshal_GetPerfMonitorCountersAMD(GLuint group, GLint * numCounters, GLint * maxActiveCounters, GLsizei countersSize, GLuint * counters);
void GLAPIENTRY _mesa_marshal_GetPerfMonitorGroupStringAMD(GLuint group, GLsizei bufSize, GLsizei * length, GLchar * groupString);
void GLAPIENTRY _mesa_marshal_GetPerfMonitorCounterStringAMD(GLuint group, GLuint counter, GLsizei bufSize, GLsizei * length, GLchar * counterString);
void GLAPIENTRY _mesa_marshal_GetPerfMonitorCounterInfoAMD(GLuint group, GLuint counter, GLenum pname, GLvoid * data);
void GLAPIENTRY _mesa_marshal_GenPerfMonitorsAMD(GLsizei n, GLuint * monitors);
struct marshal_cmd_DeletePerfMonitorsAMD;
uint32_t _mesa_unmarshal_DeletePerfMonitorsAMD(struct gl_context *ctx, const struct marshal_cmd_DeletePerfMonitorsAMD *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_DeletePerfMonitorsAMD(GLsizei n, GLuint * monitors);
struct marshal_cmd_SelectPerfMonitorCountersAMD;
uint32_t _mesa_unmarshal_SelectPerfMonitorCountersAMD(struct gl_context *ctx, const struct marshal_cmd_SelectPerfMonitorCountersAMD *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_SelectPerfMonitorCountersAMD(GLuint monitor, GLboolean enable, GLuint group, GLint numCounters, GLuint * counterList);
struct marshal_cmd_BeginPerfMonitorAMD;
uint32_t _mesa_unmarshal_BeginPerfMonitorAMD(struct gl_context *ctx, const struct marshal_cmd_BeginPerfMonitorAMD *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_BeginPerfMonitorAMD(GLuint monitor);
struct marshal_cmd_EndPerfMonitorAMD;
uint32_t _mesa_unmarshal_EndPerfMonitorAMD(struct gl_context *ctx, const struct marshal_cmd_EndPerfMonitorAMD *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_EndPerfMonitorAMD(GLuint monitor);
void GLAPIENTRY _mesa_marshal_GetPerfMonitorCounterDataAMD(GLuint monitor, GLenum pname, GLsizei dataSize, GLuint * data, GLint * bytesWritten);
struct marshal_cmd_CopyImageSubDataNV;
uint32_t _mesa_unmarshal_CopyImageSubDataNV(struct gl_context *ctx, const struct marshal_cmd_CopyImageSubDataNV *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_CopyImageSubDataNV(GLuint srcName, GLenum srcTarget, GLint srcLevel, GLint srcX, GLint srcY, GLint srcZ, GLuint dstName, GLenum dstTarget, GLint dstLevel, GLint dstX, GLint dstY, GLint dstZ, GLsizei width, GLsizei height, GLsizei depth);
struct marshal_cmd_MatrixLoadfEXT;
uint32_t _mesa_unmarshal_MatrixLoadfEXT(struct gl_context *ctx, const struct marshal_cmd_MatrixLoadfEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_MatrixLoadfEXT(GLenum matrixMode, const GLfloat * m);
struct marshal_cmd_MatrixLoaddEXT;
uint32_t _mesa_unmarshal_MatrixLoaddEXT(struct gl_context *ctx, const struct marshal_cmd_MatrixLoaddEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_MatrixLoaddEXT(GLenum matrixMode, const GLdouble * m);
struct marshal_cmd_MatrixMultfEXT;
uint32_t _mesa_unmarshal_MatrixMultfEXT(struct gl_context *ctx, const struct marshal_cmd_MatrixMultfEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_MatrixMultfEXT(GLenum matrixMode, const GLfloat * m);
struct marshal_cmd_MatrixMultdEXT;
uint32_t _mesa_unmarshal_MatrixMultdEXT(struct gl_context *ctx, const struct marshal_cmd_MatrixMultdEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_MatrixMultdEXT(GLenum matrixMode, const GLdouble * m);
struct marshal_cmd_MatrixLoadIdentityEXT;
uint32_t _mesa_unmarshal_MatrixLoadIdentityEXT(struct gl_context *ctx, const struct marshal_cmd_MatrixLoadIdentityEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_MatrixLoadIdentityEXT(GLenum matrixMode);
struct marshal_cmd_MatrixRotatefEXT;
uint32_t _mesa_unmarshal_MatrixRotatefEXT(struct gl_context *ctx, const struct marshal_cmd_MatrixRotatefEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_MatrixRotatefEXT(GLenum matrixMode, GLfloat angle, GLfloat x, GLfloat y, GLfloat z);
struct marshal_cmd_MatrixRotatedEXT;
uint32_t _mesa_unmarshal_MatrixRotatedEXT(struct gl_context *ctx, const struct marshal_cmd_MatrixRotatedEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_MatrixRotatedEXT(GLenum matrixMode, GLdouble angle, GLdouble x, GLdouble y, GLdouble z);
struct marshal_cmd_MatrixScalefEXT;
uint32_t _mesa_unmarshal_MatrixScalefEXT(struct gl_context *ctx, const struct marshal_cmd_MatrixScalefEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_MatrixScalefEXT(GLenum matrixMode, GLfloat x, GLfloat y, GLfloat z);
struct marshal_cmd_MatrixScaledEXT;
uint32_t _mesa_unmarshal_MatrixScaledEXT(struct gl_context *ctx, const struct marshal_cmd_MatrixScaledEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_MatrixScaledEXT(GLenum matrixMode, GLdouble x, GLdouble y, GLdouble z);
struct marshal_cmd_MatrixTranslatefEXT;
uint32_t _mesa_unmarshal_MatrixTranslatefEXT(struct gl_context *ctx, const struct marshal_cmd_MatrixTranslatefEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_MatrixTranslatefEXT(GLenum matrixMode, GLfloat x, GLfloat y, GLfloat z);
struct marshal_cmd_MatrixTranslatedEXT;
uint32_t _mesa_unmarshal_MatrixTranslatedEXT(struct gl_context *ctx, const struct marshal_cmd_MatrixTranslatedEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_MatrixTranslatedEXT(GLenum matrixMode, GLdouble x, GLdouble y, GLdouble z);
struct marshal_cmd_MatrixOrthoEXT;
uint32_t _mesa_unmarshal_MatrixOrthoEXT(struct gl_context *ctx, const struct marshal_cmd_MatrixOrthoEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_MatrixOrthoEXT(GLenum matrixMode, GLdouble l, GLdouble r, GLdouble b, GLdouble t, GLdouble n, GLdouble f);
struct marshal_cmd_MatrixFrustumEXT;
uint32_t _mesa_unmarshal_MatrixFrustumEXT(struct gl_context *ctx, const struct marshal_cmd_MatrixFrustumEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_MatrixFrustumEXT(GLenum matrixMode, GLdouble l, GLdouble r, GLdouble b, GLdouble t, GLdouble n, GLdouble f);
struct marshal_cmd_MatrixPushEXT;
uint32_t _mesa_unmarshal_MatrixPushEXT(struct gl_context *ctx, const struct marshal_cmd_MatrixPushEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_MatrixPushEXT(GLenum matrixMode);
struct marshal_cmd_MatrixPopEXT;
uint32_t _mesa_unmarshal_MatrixPopEXT(struct gl_context *ctx, const struct marshal_cmd_MatrixPopEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_MatrixPopEXT(GLenum matrixMode);
struct marshal_cmd_ClientAttribDefaultEXT;
uint32_t _mesa_unmarshal_ClientAttribDefaultEXT(struct gl_context *ctx, const struct marshal_cmd_ClientAttribDefaultEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ClientAttribDefaultEXT(GLbitfield mask);
struct marshal_cmd_PushClientAttribDefaultEXT;
uint32_t _mesa_unmarshal_PushClientAttribDefaultEXT(struct gl_context *ctx, const struct marshal_cmd_PushClientAttribDefaultEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_PushClientAttribDefaultEXT(GLbitfield mask);
void GLAPIENTRY _mesa_marshal_GetTextureParameterivEXT(GLuint texture, GLenum target, GLenum pname, GLint * params);
void GLAPIENTRY _mesa_marshal_GetTextureParameterfvEXT(GLuint texture, GLenum target, GLenum pname, float * params);
void GLAPIENTRY _mesa_marshal_GetTextureLevelParameterivEXT(GLuint texture, GLenum target, GLint level, GLenum pname, GLint * params);
void GLAPIENTRY _mesa_marshal_GetTextureLevelParameterfvEXT(GLuint texture, GLenum target, GLint level, GLenum pname, float * params);
struct marshal_cmd_TextureParameteriEXT;
uint32_t _mesa_unmarshal_TextureParameteriEXT(struct gl_context *ctx, const struct marshal_cmd_TextureParameteriEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TextureParameteriEXT(GLuint texture, GLenum target, GLenum pname, int param);
struct marshal_cmd_TextureParameterivEXT;
uint32_t _mesa_unmarshal_TextureParameterivEXT(struct gl_context *ctx, const struct marshal_cmd_TextureParameterivEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TextureParameterivEXT(GLuint texture, GLenum target, GLenum pname, const GLint * params);
struct marshal_cmd_TextureParameterfEXT;
uint32_t _mesa_unmarshal_TextureParameterfEXT(struct gl_context *ctx, const struct marshal_cmd_TextureParameterfEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TextureParameterfEXT(GLuint texture, GLenum target, GLenum pname, float param);
struct marshal_cmd_TextureParameterfvEXT;
uint32_t _mesa_unmarshal_TextureParameterfvEXT(struct gl_context *ctx, const struct marshal_cmd_TextureParameterfvEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TextureParameterfvEXT(GLuint texture, GLenum target, GLenum pname, const float * params);
struct marshal_cmd_TextureImage1DEXT;
uint32_t _mesa_unmarshal_TextureImage1DEXT(struct gl_context *ctx, const struct marshal_cmd_TextureImage1DEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TextureImage1DEXT(GLuint texture, GLenum target, GLint level, GLint internalFormat, GLsizei width, GLint border, GLenum format, GLenum type, const GLvoid * pixels);
struct marshal_cmd_TextureImage2DEXT;
uint32_t _mesa_unmarshal_TextureImage2DEXT(struct gl_context *ctx, const struct marshal_cmd_TextureImage2DEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TextureImage2DEXT(GLuint texture, GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid * pixels);
struct marshal_cmd_TextureImage3DEXT;
uint32_t _mesa_unmarshal_TextureImage3DEXT(struct gl_context *ctx, const struct marshal_cmd_TextureImage3DEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TextureImage3DEXT(GLuint texture, GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid * pixels);
struct marshal_cmd_TextureSubImage1DEXT;
uint32_t _mesa_unmarshal_TextureSubImage1DEXT(struct gl_context *ctx, const struct marshal_cmd_TextureSubImage1DEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TextureSubImage1DEXT(GLuint texture, GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const GLvoid * pixels);
struct marshal_cmd_TextureSubImage2DEXT;
uint32_t _mesa_unmarshal_TextureSubImage2DEXT(struct gl_context *ctx, const struct marshal_cmd_TextureSubImage2DEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TextureSubImage2DEXT(GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid * pixels);
struct marshal_cmd_TextureSubImage3DEXT;
uint32_t _mesa_unmarshal_TextureSubImage3DEXT(struct gl_context *ctx, const struct marshal_cmd_TextureSubImage3DEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TextureSubImage3DEXT(GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const GLvoid * pixels);
struct marshal_cmd_CopyTextureImage1DEXT;
uint32_t _mesa_unmarshal_CopyTextureImage1DEXT(struct gl_context *ctx, const struct marshal_cmd_CopyTextureImage1DEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_CopyTextureImage1DEXT(GLuint texture, GLenum target, GLint level, GLenum internalFormat, GLint x, GLint y, GLsizei width, int border);
struct marshal_cmd_CopyTextureImage2DEXT;
uint32_t _mesa_unmarshal_CopyTextureImage2DEXT(struct gl_context *ctx, const struct marshal_cmd_CopyTextureImage2DEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_CopyTextureImage2DEXT(GLuint texture, GLenum target, GLint level, GLenum internalFormat, GLint x, GLint y, GLsizei width, GLsizei height, int border);
struct marshal_cmd_CopyTextureSubImage1DEXT;
uint32_t _mesa_unmarshal_CopyTextureSubImage1DEXT(struct gl_context *ctx, const struct marshal_cmd_CopyTextureSubImage1DEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_CopyTextureSubImage1DEXT(GLuint texture, GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width);
struct marshal_cmd_CopyTextureSubImage2DEXT;
uint32_t _mesa_unmarshal_CopyTextureSubImage2DEXT(struct gl_context *ctx, const struct marshal_cmd_CopyTextureSubImage2DEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_CopyTextureSubImage2DEXT(GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
struct marshal_cmd_CopyTextureSubImage3DEXT;
uint32_t _mesa_unmarshal_CopyTextureSubImage3DEXT(struct gl_context *ctx, const struct marshal_cmd_CopyTextureSubImage3DEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_CopyTextureSubImage3DEXT(GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);
struct marshal_cmd_GetTextureImageEXT;
uint32_t _mesa_unmarshal_GetTextureImageEXT(struct gl_context *ctx, const struct marshal_cmd_GetTextureImageEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_GetTextureImageEXT(GLuint texture, GLenum target, GLint level, GLenum format, GLenum type, GLvoid * pixels);
struct marshal_cmd_BindMultiTextureEXT;
uint32_t _mesa_unmarshal_BindMultiTextureEXT(struct gl_context *ctx, const struct marshal_cmd_BindMultiTextureEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_BindMultiTextureEXT(GLenum texunit, GLenum target, GLuint texture);
struct marshal_cmd_EnableClientStateiEXT;
uint32_t _mesa_unmarshal_EnableClientStateiEXT(struct gl_context *ctx, const struct marshal_cmd_EnableClientStateiEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_EnableClientStateiEXT(GLenum array, GLuint index);
struct marshal_cmd_DisableClientStateiEXT;
uint32_t _mesa_unmarshal_DisableClientStateiEXT(struct gl_context *ctx, const struct marshal_cmd_DisableClientStateiEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_DisableClientStateiEXT(GLenum array, GLuint index);
void GLAPIENTRY _mesa_marshal_GetPointerIndexedvEXT(GLenum target, GLuint index, GLvoid** params);
struct marshal_cmd_MultiTexEnviEXT;
uint32_t _mesa_unmarshal_MultiTexEnviEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexEnviEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_MultiTexEnviEXT(GLenum texunit, GLenum target, GLenum pname, GLint param);
struct marshal_cmd_MultiTexEnvivEXT;
uint32_t _mesa_unmarshal_MultiTexEnvivEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexEnvivEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_MultiTexEnvivEXT(GLenum texunit, GLenum target, GLenum pname, const GLint * param);
struct marshal_cmd_MultiTexEnvfEXT;
uint32_t _mesa_unmarshal_MultiTexEnvfEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexEnvfEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_MultiTexEnvfEXT(GLenum texunit, GLenum target, GLenum pname, GLfloat param);
struct marshal_cmd_MultiTexEnvfvEXT;
uint32_t _mesa_unmarshal_MultiTexEnvfvEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexEnvfvEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_MultiTexEnvfvEXT(GLenum texunit, GLenum target, GLenum pname, const GLfloat * param);
void GLAPIENTRY _mesa_marshal_GetMultiTexEnvivEXT(GLenum texunit, GLenum target, GLenum pname, GLint * param);
void GLAPIENTRY _mesa_marshal_GetMultiTexEnvfvEXT(GLenum texunit, GLenum target, GLenum pname, GLfloat * param);
struct marshal_cmd_MultiTexParameteriEXT;
uint32_t _mesa_unmarshal_MultiTexParameteriEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexParameteriEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_MultiTexParameteriEXT(GLenum texunit, GLenum target, GLenum pname, GLint param);
struct marshal_cmd_MultiTexParameterivEXT;
uint32_t _mesa_unmarshal_MultiTexParameterivEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexParameterivEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_MultiTexParameterivEXT(GLenum texunit, GLenum target, GLenum pname, const GLint* param);
struct marshal_cmd_MultiTexParameterfEXT;
uint32_t _mesa_unmarshal_MultiTexParameterfEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexParameterfEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_MultiTexParameterfEXT(GLenum texunit, GLenum target, GLenum pname, GLfloat param);
struct marshal_cmd_MultiTexParameterfvEXT;
uint32_t _mesa_unmarshal_MultiTexParameterfvEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexParameterfvEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_MultiTexParameterfvEXT(GLenum texunit, GLenum target, GLenum pname, const GLfloat* param);
void GLAPIENTRY _mesa_marshal_GetMultiTexParameterivEXT(GLenum texunit, GLenum target, GLenum pname, GLint* params);
void GLAPIENTRY _mesa_marshal_GetMultiTexParameterfvEXT(GLenum texunit, GLenum target, GLenum pname, GLfloat* params);
struct marshal_cmd_GetMultiTexImageEXT;
uint32_t _mesa_unmarshal_GetMultiTexImageEXT(struct gl_context *ctx, const struct marshal_cmd_GetMultiTexImageEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_GetMultiTexImageEXT(GLenum texunit, GLenum target, GLint level, GLenum format, GLenum type, GLvoid* pixels);
void GLAPIENTRY _mesa_marshal_GetMultiTexLevelParameterivEXT(GLenum texunit, GLenum target, GLint level, GLenum pname, GLint* params);
void GLAPIENTRY _mesa_marshal_GetMultiTexLevelParameterfvEXT(GLenum texunit, GLenum target, GLint level, GLenum pname, GLfloat* params);
struct marshal_cmd_MultiTexImage1DEXT;
uint32_t _mesa_unmarshal_MultiTexImage1DEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexImage1DEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_MultiTexImage1DEXT(GLenum texunit, GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, const GLvoid* pixels);
struct marshal_cmd_MultiTexImage2DEXT;
uint32_t _mesa_unmarshal_MultiTexImage2DEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexImage2DEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_MultiTexImage2DEXT(GLenum texunit, GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid* pixels);
struct marshal_cmd_MultiTexImage3DEXT;
uint32_t _mesa_unmarshal_MultiTexImage3DEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexImage3DEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_MultiTexImage3DEXT(GLenum texunit, GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid* pixels);
struct marshal_cmd_MultiTexSubImage1DEXT;
uint32_t _mesa_unmarshal_MultiTexSubImage1DEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexSubImage1DEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_MultiTexSubImage1DEXT(GLenum texunit, GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const GLvoid* pixels);
struct marshal_cmd_MultiTexSubImage2DEXT;
uint32_t _mesa_unmarshal_MultiTexSubImage2DEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexSubImage2DEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_MultiTexSubImage2DEXT(GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid* pixels);
struct marshal_cmd_MultiTexSubImage3DEXT;
uint32_t _mesa_unmarshal_MultiTexSubImage3DEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexSubImage3DEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_MultiTexSubImage3DEXT(GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const GLvoid* pixels);
struct marshal_cmd_CopyMultiTexImage1DEXT;
uint32_t _mesa_unmarshal_CopyMultiTexImage1DEXT(struct gl_context *ctx, const struct marshal_cmd_CopyMultiTexImage1DEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_CopyMultiTexImage1DEXT(GLenum texunit, GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLint border);
struct marshal_cmd_CopyMultiTexImage2DEXT;
uint32_t _mesa_unmarshal_CopyMultiTexImage2DEXT(struct gl_context *ctx, const struct marshal_cmd_CopyMultiTexImage2DEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_CopyMultiTexImage2DEXT(GLenum texunit, GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
struct marshal_cmd_CopyMultiTexSubImage1DEXT;
uint32_t _mesa_unmarshal_CopyMultiTexSubImage1DEXT(struct gl_context *ctx, const struct marshal_cmd_CopyMultiTexSubImage1DEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_CopyMultiTexSubImage1DEXT(GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width);
struct marshal_cmd_CopyMultiTexSubImage2DEXT;
uint32_t _mesa_unmarshal_CopyMultiTexSubImage2DEXT(struct gl_context *ctx, const struct marshal_cmd_CopyMultiTexSubImage2DEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_CopyMultiTexSubImage2DEXT(GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
struct marshal_cmd_CopyMultiTexSubImage3DEXT;
uint32_t _mesa_unmarshal_CopyMultiTexSubImage3DEXT(struct gl_context *ctx, const struct marshal_cmd_CopyMultiTexSubImage3DEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_CopyMultiTexSubImage3DEXT(GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);
struct marshal_cmd_MultiTexGendEXT;
uint32_t _mesa_unmarshal_MultiTexGendEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexGendEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_MultiTexGendEXT(GLenum texunit, GLenum coord, GLenum pname, GLdouble param);
struct marshal_cmd_MultiTexGendvEXT;
uint32_t _mesa_unmarshal_MultiTexGendvEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexGendvEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_MultiTexGendvEXT(GLenum texunit, GLenum coord, GLenum pname, const GLdouble* param);
struct marshal_cmd_MultiTexGenfEXT;
uint32_t _mesa_unmarshal_MultiTexGenfEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexGenfEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_MultiTexGenfEXT(GLenum texunit, GLenum coord, GLenum pname, GLfloat param);
struct marshal_cmd_MultiTexGenfvEXT;
uint32_t _mesa_unmarshal_MultiTexGenfvEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexGenfvEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_MultiTexGenfvEXT(GLenum texunit, GLenum coord, GLenum pname, const GLfloat * param);
struct marshal_cmd_MultiTexGeniEXT;
uint32_t _mesa_unmarshal_MultiTexGeniEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexGeniEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_MultiTexGeniEXT(GLenum texunit, GLenum coord, GLenum pname, GLint param);
struct marshal_cmd_MultiTexGenivEXT;
uint32_t _mesa_unmarshal_MultiTexGenivEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexGenivEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_MultiTexGenivEXT(GLenum texunit, GLenum coord, GLenum pname, const GLint * param);
void GLAPIENTRY _mesa_marshal_GetMultiTexGendvEXT(GLenum texunit, GLenum coord, GLenum pname, GLdouble * param);
void GLAPIENTRY _mesa_marshal_GetMultiTexGenfvEXT(GLenum texunit, GLenum coord, GLenum pname, GLfloat * param);
void GLAPIENTRY _mesa_marshal_GetMultiTexGenivEXT(GLenum texunit, GLenum coord, GLenum pname, GLint * param);
struct marshal_cmd_MultiTexCoordPointerEXT;
uint32_t _mesa_unmarshal_MultiTexCoordPointerEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoordPointerEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_MultiTexCoordPointerEXT(GLenum texunit, GLint size, GLenum type, GLsizei stride, const GLvoid * pointer);
struct marshal_cmd_MatrixLoadTransposefEXT;
uint32_t _mesa_unmarshal_MatrixLoadTransposefEXT(struct gl_context *ctx, const struct marshal_cmd_MatrixLoadTransposefEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_MatrixLoadTransposefEXT(GLenum matrixMode, const GLfloat * m);
struct marshal_cmd_MatrixLoadTransposedEXT;
uint32_t _mesa_unmarshal_MatrixLoadTransposedEXT(struct gl_context *ctx, const struct marshal_cmd_MatrixLoadTransposedEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_MatrixLoadTransposedEXT(GLenum matrixMode, const GLdouble * m);
struct marshal_cmd_MatrixMultTransposefEXT;
uint32_t _mesa_unmarshal_MatrixMultTransposefEXT(struct gl_context *ctx, const struct marshal_cmd_MatrixMultTransposefEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_MatrixMultTransposefEXT(GLenum matrixMode, const GLfloat * m);
struct marshal_cmd_MatrixMultTransposedEXT;
uint32_t _mesa_unmarshal_MatrixMultTransposedEXT(struct gl_context *ctx, const struct marshal_cmd_MatrixMultTransposedEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_MatrixMultTransposedEXT(GLenum matrixMode, const GLdouble * m);
struct marshal_cmd_CompressedTextureImage1DEXT;
uint32_t _mesa_unmarshal_CompressedTextureImage1DEXT(struct gl_context *ctx, const struct marshal_cmd_CompressedTextureImage1DEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_CompressedTextureImage1DEXT(GLuint texture, GLenum target, GLint level, GLenum internalFormat, GLsizei width, GLsizei border, GLsizei imageSize, const GLvoid * data);
struct marshal_cmd_CompressedTextureImage2DEXT;
uint32_t _mesa_unmarshal_CompressedTextureImage2DEXT(struct gl_context *ctx, const struct marshal_cmd_CompressedTextureImage2DEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_CompressedTextureImage2DEXT(GLuint texture, GLenum target, GLint level, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei border, GLsizei imageSize, const GLvoid * data);
struct marshal_cmd_CompressedTextureImage3DEXT;
uint32_t _mesa_unmarshal_CompressedTextureImage3DEXT(struct gl_context *ctx, const struct marshal_cmd_CompressedTextureImage3DEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_CompressedTextureImage3DEXT(GLuint texture, GLenum target, GLint level, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLsizei border, GLsizei imageSize, const GLvoid * data);
struct marshal_cmd_CompressedTextureSubImage1DEXT;
uint32_t _mesa_unmarshal_CompressedTextureSubImage1DEXT(struct gl_context *ctx, const struct marshal_cmd_CompressedTextureSubImage1DEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_CompressedTextureSubImage1DEXT(GLuint texture, GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const GLvoid * data);
struct marshal_cmd_CompressedTextureSubImage2DEXT;
uint32_t _mesa_unmarshal_CompressedTextureSubImage2DEXT(struct gl_context *ctx, const struct marshal_cmd_CompressedTextureSubImage2DEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_CompressedTextureSubImage2DEXT(GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const GLvoid * data);
struct marshal_cmd_CompressedTextureSubImage3DEXT;
uint32_t _mesa_unmarshal_CompressedTextureSubImage3DEXT(struct gl_context *ctx, const struct marshal_cmd_CompressedTextureSubImage3DEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_CompressedTextureSubImage3DEXT(GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const GLvoid * data);
struct marshal_cmd_GetCompressedTextureImageEXT;
uint32_t _mesa_unmarshal_GetCompressedTextureImageEXT(struct gl_context *ctx, const struct marshal_cmd_GetCompressedTextureImageEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_GetCompressedTextureImageEXT(GLuint texture, GLenum target, GLint level, GLvoid * img);
struct marshal_cmd_CompressedMultiTexImage1DEXT;
uint32_t _mesa_unmarshal_CompressedMultiTexImage1DEXT(struct gl_context *ctx, const struct marshal_cmd_CompressedMultiTexImage1DEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_CompressedMultiTexImage1DEXT(GLenum texunit, GLenum target, GLint level, GLenum internalFormat, GLsizei width, GLsizei border, GLsizei imageSize, const GLvoid * data);
struct marshal_cmd_CompressedMultiTexImage2DEXT;
uint32_t _mesa_unmarshal_CompressedMultiTexImage2DEXT(struct gl_context *ctx, const struct marshal_cmd_CompressedMultiTexImage2DEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_CompressedMultiTexImage2DEXT(GLenum texunit, GLenum target, GLint level, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei border, GLsizei imageSize, const GLvoid * data);
struct marshal_cmd_CompressedMultiTexImage3DEXT;
uint32_t _mesa_unmarshal_CompressedMultiTexImage3DEXT(struct gl_context *ctx, const struct marshal_cmd_CompressedMultiTexImage3DEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_CompressedMultiTexImage3DEXT(GLenum texunit, GLenum target, GLint level, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLsizei border, GLsizei imageSize, const GLvoid * data);
struct marshal_cmd_CompressedMultiTexSubImage1DEXT;
uint32_t _mesa_unmarshal_CompressedMultiTexSubImage1DEXT(struct gl_context *ctx, const struct marshal_cmd_CompressedMultiTexSubImage1DEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_CompressedMultiTexSubImage1DEXT(GLenum texunit, GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const GLvoid * data);
struct marshal_cmd_CompressedMultiTexSubImage2DEXT;
uint32_t _mesa_unmarshal_CompressedMultiTexSubImage2DEXT(struct gl_context *ctx, const struct marshal_cmd_CompressedMultiTexSubImage2DEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_CompressedMultiTexSubImage2DEXT(GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const GLvoid * data);
struct marshal_cmd_CompressedMultiTexSubImage3DEXT;
uint32_t _mesa_unmarshal_CompressedMultiTexSubImage3DEXT(struct gl_context *ctx, const struct marshal_cmd_CompressedMultiTexSubImage3DEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_CompressedMultiTexSubImage3DEXT(GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const GLvoid * data);
struct marshal_cmd_GetCompressedMultiTexImageEXT;
uint32_t _mesa_unmarshal_GetCompressedMultiTexImageEXT(struct gl_context *ctx, const struct marshal_cmd_GetCompressedMultiTexImageEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_GetCompressedMultiTexImageEXT(GLenum texunit, GLenum target, GLint level, GLvoid * img);
struct marshal_cmd_NamedBufferDataEXT;
uint32_t _mesa_unmarshal_NamedBufferDataEXT(struct gl_context *ctx, const struct marshal_cmd_NamedBufferDataEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_NamedBufferDataEXT(GLuint buffer, GLsizeiptr size, const GLvoid * data, GLenum usage);
struct marshal_cmd_NamedBufferSubDataEXT;
uint32_t _mesa_unmarshal_NamedBufferSubDataEXT(struct gl_context *ctx, const struct marshal_cmd_NamedBufferSubDataEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_NamedBufferSubDataEXT(GLuint buffer, GLintptr offset, GLsizeiptr size, const GLvoid * data);
GLvoid * GLAPIENTRY _mesa_marshal_MapNamedBufferEXT(GLuint buffer, GLenum access);
void GLAPIENTRY _mesa_marshal_GetNamedBufferSubDataEXT(GLuint buffer, GLintptr offset, GLsizeiptr size, GLvoid * data);
void GLAPIENTRY _mesa_marshal_GetNamedBufferPointervEXT(GLuint buffer, GLenum pname, GLvoid ** params);
void GLAPIENTRY _mesa_marshal_GetNamedBufferParameterivEXT(GLuint buffer, GLenum pname, GLint * params);
struct marshal_cmd_FlushMappedNamedBufferRangeEXT;
uint32_t _mesa_unmarshal_FlushMappedNamedBufferRangeEXT(struct gl_context *ctx, const struct marshal_cmd_FlushMappedNamedBufferRangeEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_FlushMappedNamedBufferRangeEXT(GLuint buffer, GLintptr offset, GLsizeiptr length);
GLvoid * GLAPIENTRY _mesa_marshal_MapNamedBufferRangeEXT(GLuint buffer, GLintptr offset, GLsizeiptr length, GLbitfield access);
struct marshal_cmd_FramebufferDrawBufferEXT;
uint32_t _mesa_unmarshal_FramebufferDrawBufferEXT(struct gl_context *ctx, const struct marshal_cmd_FramebufferDrawBufferEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_FramebufferDrawBufferEXT(GLuint framebuffer, GLenum mode);
struct marshal_cmd_FramebufferDrawBuffersEXT;
uint32_t _mesa_unmarshal_FramebufferDrawBuffersEXT(struct gl_context *ctx, const struct marshal_cmd_FramebufferDrawBuffersEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_FramebufferDrawBuffersEXT(GLuint framebuffer, GLsizei n, const GLenum * bufs);
struct marshal_cmd_FramebufferReadBufferEXT;
uint32_t _mesa_unmarshal_FramebufferReadBufferEXT(struct gl_context *ctx, const struct marshal_cmd_FramebufferReadBufferEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_FramebufferReadBufferEXT(GLuint framebuffer, GLenum mode);
void GLAPIENTRY _mesa_marshal_GetFramebufferParameterivEXT(GLuint framebuffer, GLenum pname, GLint * param);
GLenum GLAPIENTRY _mesa_marshal_CheckNamedFramebufferStatusEXT(GLuint framebuffer, GLenum target);
struct marshal_cmd_NamedFramebufferTexture1DEXT;
uint32_t _mesa_unmarshal_NamedFramebufferTexture1DEXT(struct gl_context *ctx, const struct marshal_cmd_NamedFramebufferTexture1DEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_NamedFramebufferTexture1DEXT(GLuint framebuffer, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
struct marshal_cmd_NamedFramebufferTexture2DEXT;
uint32_t _mesa_unmarshal_NamedFramebufferTexture2DEXT(struct gl_context *ctx, const struct marshal_cmd_NamedFramebufferTexture2DEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_NamedFramebufferTexture2DEXT(GLuint framebuffer, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
struct marshal_cmd_NamedFramebufferTexture3DEXT;
uint32_t _mesa_unmarshal_NamedFramebufferTexture3DEXT(struct gl_context *ctx, const struct marshal_cmd_NamedFramebufferTexture3DEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_NamedFramebufferTexture3DEXT(GLuint framebuffer, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset);
struct marshal_cmd_NamedFramebufferRenderbufferEXT;
uint32_t _mesa_unmarshal_NamedFramebufferRenderbufferEXT(struct gl_context *ctx, const struct marshal_cmd_NamedFramebufferRenderbufferEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_NamedFramebufferRenderbufferEXT(GLuint framebuffer, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
void GLAPIENTRY _mesa_marshal_GetNamedFramebufferAttachmentParameterivEXT(GLuint framebuffer, GLenum attachment, GLenum pname, GLint * params);
struct marshal_cmd_NamedRenderbufferStorageEXT;
uint32_t _mesa_unmarshal_NamedRenderbufferStorageEXT(struct gl_context *ctx, const struct marshal_cmd_NamedRenderbufferStorageEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_NamedRenderbufferStorageEXT(GLuint renderbuffer, GLenum internalformat, GLsizei width, GLsizei height);
void GLAPIENTRY _mesa_marshal_GetNamedRenderbufferParameterivEXT(GLuint renderbuffer, GLenum pname, GLint * params);
struct marshal_cmd_GenerateTextureMipmapEXT;
uint32_t _mesa_unmarshal_GenerateTextureMipmapEXT(struct gl_context *ctx, const struct marshal_cmd_GenerateTextureMipmapEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_GenerateTextureMipmapEXT(GLuint texture, GLenum target);
struct marshal_cmd_GenerateMultiTexMipmapEXT;
uint32_t _mesa_unmarshal_GenerateMultiTexMipmapEXT(struct gl_context *ctx, const struct marshal_cmd_GenerateMultiTexMipmapEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_GenerateMultiTexMipmapEXT(GLenum texunit, GLenum target);
struct marshal_cmd_NamedRenderbufferStorageMultisampleEXT;
uint32_t _mesa_unmarshal_NamedRenderbufferStorageMultisampleEXT(struct gl_context *ctx, const struct marshal_cmd_NamedRenderbufferStorageMultisampleEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_NamedRenderbufferStorageMultisampleEXT(GLuint renderbuffer, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height);
struct marshal_cmd_NamedCopyBufferSubDataEXT;
uint32_t _mesa_unmarshal_NamedCopyBufferSubDataEXT(struct gl_context *ctx, const struct marshal_cmd_NamedCopyBufferSubDataEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_NamedCopyBufferSubDataEXT(GLuint readBuffer, GLuint writeBuffer, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size);
struct marshal_cmd_VertexArrayVertexOffsetEXT;
uint32_t _mesa_unmarshal_VertexArrayVertexOffsetEXT(struct gl_context *ctx, const struct marshal_cmd_VertexArrayVertexOffsetEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexArrayVertexOffsetEXT(GLuint vaobj, GLuint buffer, GLint size, GLenum type, GLsizei stride, GLintptr offset);
struct marshal_cmd_VertexArrayColorOffsetEXT;
uint32_t _mesa_unmarshal_VertexArrayColorOffsetEXT(struct gl_context *ctx, const struct marshal_cmd_VertexArrayColorOffsetEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexArrayColorOffsetEXT(GLuint vaobj, GLuint buffer, GLint size, GLenum type, GLsizei stride, GLintptr offset);
struct marshal_cmd_VertexArrayEdgeFlagOffsetEXT;
uint32_t _mesa_unmarshal_VertexArrayEdgeFlagOffsetEXT(struct gl_context *ctx, const struct marshal_cmd_VertexArrayEdgeFlagOffsetEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexArrayEdgeFlagOffsetEXT(GLuint vaobj, GLuint buffer, GLsizei stride, GLintptr offset);
struct marshal_cmd_VertexArrayIndexOffsetEXT;
uint32_t _mesa_unmarshal_VertexArrayIndexOffsetEXT(struct gl_context *ctx, const struct marshal_cmd_VertexArrayIndexOffsetEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexArrayIndexOffsetEXT(GLuint vaobj, GLuint buffer, GLenum type, GLsizei stride, GLintptr offset);
struct marshal_cmd_VertexArrayNormalOffsetEXT;
uint32_t _mesa_unmarshal_VertexArrayNormalOffsetEXT(struct gl_context *ctx, const struct marshal_cmd_VertexArrayNormalOffsetEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexArrayNormalOffsetEXT(GLuint vaobj, GLuint buffer, GLenum type, GLsizei stride, GLintptr offset);
struct marshal_cmd_VertexArrayTexCoordOffsetEXT;
uint32_t _mesa_unmarshal_VertexArrayTexCoordOffsetEXT(struct gl_context *ctx, const struct marshal_cmd_VertexArrayTexCoordOffsetEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexArrayTexCoordOffsetEXT(GLuint vaobj, GLuint buffer, GLint size, GLenum type, GLsizei stride, GLintptr offset);
struct marshal_cmd_VertexArrayMultiTexCoordOffsetEXT;
uint32_t _mesa_unmarshal_VertexArrayMultiTexCoordOffsetEXT(struct gl_context *ctx, const struct marshal_cmd_VertexArrayMultiTexCoordOffsetEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexArrayMultiTexCoordOffsetEXT(GLuint vaobj, GLuint buffer, GLenum texunit, GLint size, GLenum type, GLsizei stride, GLintptr offset);
struct marshal_cmd_VertexArrayFogCoordOffsetEXT;
uint32_t _mesa_unmarshal_VertexArrayFogCoordOffsetEXT(struct gl_context *ctx, const struct marshal_cmd_VertexArrayFogCoordOffsetEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexArrayFogCoordOffsetEXT(GLuint vaobj, GLuint buffer, GLenum type, GLsizei stride, GLintptr offset);
struct marshal_cmd_VertexArraySecondaryColorOffsetEXT;
uint32_t _mesa_unmarshal_VertexArraySecondaryColorOffsetEXT(struct gl_context *ctx, const struct marshal_cmd_VertexArraySecondaryColorOffsetEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexArraySecondaryColorOffsetEXT(GLuint vaobj, GLuint buffer, GLint size, GLenum type, GLsizei stride, GLintptr offset);
struct marshal_cmd_VertexArrayVertexAttribOffsetEXT;
uint32_t _mesa_unmarshal_VertexArrayVertexAttribOffsetEXT(struct gl_context *ctx, const struct marshal_cmd_VertexArrayVertexAttribOffsetEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexArrayVertexAttribOffsetEXT(GLuint vaobj, GLuint buffer, GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, GLintptr offset);
struct marshal_cmd_VertexArrayVertexAttribIOffsetEXT;
uint32_t _mesa_unmarshal_VertexArrayVertexAttribIOffsetEXT(struct gl_context *ctx, const struct marshal_cmd_VertexArrayVertexAttribIOffsetEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexArrayVertexAttribIOffsetEXT(GLuint vaobj, GLuint buffer, GLuint index, GLint size, GLenum type, GLsizei stride, GLintptr offset);
struct marshal_cmd_EnableVertexArrayEXT;
uint32_t _mesa_unmarshal_EnableVertexArrayEXT(struct gl_context *ctx, const struct marshal_cmd_EnableVertexArrayEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_EnableVertexArrayEXT(GLuint vaobj, GLenum array);
struct marshal_cmd_DisableVertexArrayEXT;
uint32_t _mesa_unmarshal_DisableVertexArrayEXT(struct gl_context *ctx, const struct marshal_cmd_DisableVertexArrayEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_DisableVertexArrayEXT(GLuint vaobj, GLenum array);
struct marshal_cmd_EnableVertexArrayAttribEXT;
uint32_t _mesa_unmarshal_EnableVertexArrayAttribEXT(struct gl_context *ctx, const struct marshal_cmd_EnableVertexArrayAttribEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_EnableVertexArrayAttribEXT(GLuint vaobj, GLuint index);
struct marshal_cmd_DisableVertexArrayAttribEXT;
uint32_t _mesa_unmarshal_DisableVertexArrayAttribEXT(struct gl_context *ctx, const struct marshal_cmd_DisableVertexArrayAttribEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_DisableVertexArrayAttribEXT(GLuint vaobj, GLuint index);
void GLAPIENTRY _mesa_marshal_GetVertexArrayIntegervEXT(GLuint vaobj, GLenum pname, GLint* param);
void GLAPIENTRY _mesa_marshal_GetVertexArrayPointervEXT(GLuint vaobj, GLenum pname, GLvoid** param);
void GLAPIENTRY _mesa_marshal_GetVertexArrayIntegeri_vEXT(GLuint vaobj, GLuint index, GLenum pname, GLint* param);
void GLAPIENTRY _mesa_marshal_GetVertexArrayPointeri_vEXT(GLuint vaobj, GLuint index, GLenum pname, GLvoid** param);
struct marshal_cmd_NamedProgramStringEXT;
uint32_t _mesa_unmarshal_NamedProgramStringEXT(struct gl_context *ctx, const struct marshal_cmd_NamedProgramStringEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_NamedProgramStringEXT(GLuint program, GLenum target, GLenum format, GLsizei len, const GLvoid* string);
void GLAPIENTRY _mesa_marshal_GetNamedProgramStringEXT(GLuint program, GLenum target, GLenum pname, GLvoid* string);
struct marshal_cmd_NamedProgramLocalParameter4fEXT;
uint32_t _mesa_unmarshal_NamedProgramLocalParameter4fEXT(struct gl_context *ctx, const struct marshal_cmd_NamedProgramLocalParameter4fEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_NamedProgramLocalParameter4fEXT(GLuint program, GLenum target, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
struct marshal_cmd_NamedProgramLocalParameter4fvEXT;
uint32_t _mesa_unmarshal_NamedProgramLocalParameter4fvEXT(struct gl_context *ctx, const struct marshal_cmd_NamedProgramLocalParameter4fvEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_NamedProgramLocalParameter4fvEXT(GLuint program, GLenum target, GLuint index, const GLfloat* params);
void GLAPIENTRY _mesa_marshal_GetNamedProgramLocalParameterfvEXT(GLuint program, GLenum target, GLuint index, GLfloat* params);
struct marshal_cmd_NamedProgramLocalParameter4dEXT;
uint32_t _mesa_unmarshal_NamedProgramLocalParameter4dEXT(struct gl_context *ctx, const struct marshal_cmd_NamedProgramLocalParameter4dEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_NamedProgramLocalParameter4dEXT(GLuint program, GLenum target, GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
struct marshal_cmd_NamedProgramLocalParameter4dvEXT;
uint32_t _mesa_unmarshal_NamedProgramLocalParameter4dvEXT(struct gl_context *ctx, const struct marshal_cmd_NamedProgramLocalParameter4dvEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_NamedProgramLocalParameter4dvEXT(GLuint program, GLenum target, GLuint index, const GLdouble* params);
void GLAPIENTRY _mesa_marshal_GetNamedProgramLocalParameterdvEXT(GLuint program, GLenum target, GLuint index, GLdouble* params);
void GLAPIENTRY _mesa_marshal_GetNamedProgramivEXT(GLuint program, GLenum target, GLenum pname, GLint* params);
struct marshal_cmd_TextureBufferEXT;
uint32_t _mesa_unmarshal_TextureBufferEXT(struct gl_context *ctx, const struct marshal_cmd_TextureBufferEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TextureBufferEXT(GLuint texture, GLenum target, GLenum internalformat, GLuint buffer);
struct marshal_cmd_MultiTexBufferEXT;
uint32_t _mesa_unmarshal_MultiTexBufferEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexBufferEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_MultiTexBufferEXT(GLenum texunit, GLenum target, GLenum internalformat, GLuint buffer);
struct marshal_cmd_TextureParameterIivEXT;
uint32_t _mesa_unmarshal_TextureParameterIivEXT(struct gl_context *ctx, const struct marshal_cmd_TextureParameterIivEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TextureParameterIivEXT(GLuint texture, GLenum target, GLenum pname, const GLint* params);
struct marshal_cmd_TextureParameterIuivEXT;
uint32_t _mesa_unmarshal_TextureParameterIuivEXT(struct gl_context *ctx, const struct marshal_cmd_TextureParameterIuivEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TextureParameterIuivEXT(GLuint texture, GLenum target, GLenum pname, const GLuint* params);
void GLAPIENTRY _mesa_marshal_GetTextureParameterIivEXT(GLuint texture, GLenum target, GLenum pname, GLint* params);
void GLAPIENTRY _mesa_marshal_GetTextureParameterIuivEXT(GLuint texture, GLenum target, GLenum pname, GLuint* params);
struct marshal_cmd_MultiTexParameterIivEXT;
uint32_t _mesa_unmarshal_MultiTexParameterIivEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexParameterIivEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_MultiTexParameterIivEXT(GLenum texunit, GLenum target, GLenum pname, const GLint* params);
struct marshal_cmd_MultiTexParameterIuivEXT;
uint32_t _mesa_unmarshal_MultiTexParameterIuivEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexParameterIuivEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_MultiTexParameterIuivEXT(GLenum texunit, GLenum target, GLenum pname, const GLuint* params);
void GLAPIENTRY _mesa_marshal_GetMultiTexParameterIivEXT(GLenum texunit, GLenum target, GLenum pname, GLint* params);
void GLAPIENTRY _mesa_marshal_GetMultiTexParameterIuivEXT(GLenum texunit, GLenum target, GLenum pname, GLuint* params);
struct marshal_cmd_NamedProgramLocalParameters4fvEXT;
uint32_t _mesa_unmarshal_NamedProgramLocalParameters4fvEXT(struct gl_context *ctx, const struct marshal_cmd_NamedProgramLocalParameters4fvEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_NamedProgramLocalParameters4fvEXT(GLuint program, GLenum target, GLuint index, GLsizei count, const GLfloat* params);
struct marshal_cmd_BindImageTextureEXT;
uint32_t _mesa_unmarshal_BindImageTextureEXT(struct gl_context *ctx, const struct marshal_cmd_BindImageTextureEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_BindImageTextureEXT(GLuint index, GLuint texture, GLint level, GLboolean layered, GLint layer, GLenum access, GLint format);
struct marshal_cmd_SubpixelPrecisionBiasNV;
uint32_t _mesa_unmarshal_SubpixelPrecisionBiasNV(struct gl_context *ctx, const struct marshal_cmd_SubpixelPrecisionBiasNV *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_SubpixelPrecisionBiasNV(GLuint xbits, GLuint ybits);
struct marshal_cmd_ConservativeRasterParameterfNV;
uint32_t _mesa_unmarshal_ConservativeRasterParameterfNV(struct gl_context *ctx, const struct marshal_cmd_ConservativeRasterParameterfNV *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ConservativeRasterParameterfNV(GLenum pname, GLfloat param);
struct marshal_cmd_ConservativeRasterParameteriNV;
uint32_t _mesa_unmarshal_ConservativeRasterParameteriNV(struct gl_context *ctx, const struct marshal_cmd_ConservativeRasterParameteriNV *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ConservativeRasterParameteriNV(GLenum pname, GLint param);
void GLAPIENTRY _mesa_marshal_GetFirstPerfQueryIdINTEL(GLuint * queryId);
void GLAPIENTRY _mesa_marshal_GetNextPerfQueryIdINTEL(GLuint queryId, GLuint * nextQueryId);
void GLAPIENTRY _mesa_marshal_GetPerfQueryIdByNameINTEL(GLchar * queryName, GLuint * queryId);
void GLAPIENTRY _mesa_marshal_GetPerfQueryInfoINTEL(GLuint queryId, GLuint queryNameLength, GLchar * queryName, GLuint * dataSize, GLuint * noCounters, GLuint * noInstances, GLuint * capsMask);
void GLAPIENTRY _mesa_marshal_GetPerfCounterInfoINTEL(GLuint queryId, GLuint counterId, GLuint counterNameLength, GLchar * counterName, GLuint counterDescLength, GLchar * counterDesc, GLuint * counterOffset, GLuint * counterDataSize, GLuint * counterTypeEnum, GLuint * counterDataTypeEnum, GLuint64 * rawCounterMaxValue);
void GLAPIENTRY _mesa_marshal_CreatePerfQueryINTEL(GLuint queryId, GLuint * queryHandle);
struct marshal_cmd_DeletePerfQueryINTEL;
uint32_t _mesa_unmarshal_DeletePerfQueryINTEL(struct gl_context *ctx, const struct marshal_cmd_DeletePerfQueryINTEL *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_DeletePerfQueryINTEL(GLuint queryHandle);
struct marshal_cmd_BeginPerfQueryINTEL;
uint32_t _mesa_unmarshal_BeginPerfQueryINTEL(struct gl_context *ctx, const struct marshal_cmd_BeginPerfQueryINTEL *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_BeginPerfQueryINTEL(GLuint queryHandle);
struct marshal_cmd_EndPerfQueryINTEL;
uint32_t _mesa_unmarshal_EndPerfQueryINTEL(struct gl_context *ctx, const struct marshal_cmd_EndPerfQueryINTEL *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_EndPerfQueryINTEL(GLuint queryHandle);
void GLAPIENTRY _mesa_marshal_GetPerfQueryDataINTEL(GLuint queryHandle, GLuint flags, GLsizei dataSize, GLvoid * data, GLuint * bytesWritten);
struct marshal_cmd_AlphaToCoverageDitherControlNV;
uint32_t _mesa_unmarshal_AlphaToCoverageDitherControlNV(struct gl_context *ctx, const struct marshal_cmd_AlphaToCoverageDitherControlNV *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_AlphaToCoverageDitherControlNV(GLenum mode);
struct marshal_cmd_PolygonOffsetClampEXT;
uint32_t _mesa_unmarshal_PolygonOffsetClampEXT(struct gl_context *ctx, const struct marshal_cmd_PolygonOffsetClampEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_PolygonOffsetClampEXT(GLfloat factor, GLfloat units, GLfloat clamp);
struct marshal_cmd_WindowRectanglesEXT;
uint32_t _mesa_unmarshal_WindowRectanglesEXT(struct gl_context *ctx, const struct marshal_cmd_WindowRectanglesEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_WindowRectanglesEXT(GLenum mode, GLsizei count, const GLint * box);
struct marshal_cmd_FramebufferFetchBarrierEXT;
uint32_t _mesa_unmarshal_FramebufferFetchBarrierEXT(struct gl_context *ctx, const struct marshal_cmd_FramebufferFetchBarrierEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_FramebufferFetchBarrierEXT(void);
struct marshal_cmd_RenderbufferStorageMultisampleAdvancedAMD;
uint32_t _mesa_unmarshal_RenderbufferStorageMultisampleAdvancedAMD(struct gl_context *ctx, const struct marshal_cmd_RenderbufferStorageMultisampleAdvancedAMD *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_RenderbufferStorageMultisampleAdvancedAMD(GLenum target, GLsizei samples, GLsizei storageSamples, GLenum internalformat, GLsizei width, GLsizei height);
struct marshal_cmd_NamedRenderbufferStorageMultisampleAdvancedAMD;
uint32_t _mesa_unmarshal_NamedRenderbufferStorageMultisampleAdvancedAMD(struct gl_context *ctx, const struct marshal_cmd_NamedRenderbufferStorageMultisampleAdvancedAMD *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_NamedRenderbufferStorageMultisampleAdvancedAMD(GLuint renderbuffer, GLsizei samples, GLsizei storageSamples, GLenum internalformat, GLsizei width, GLsizei height);
struct marshal_cmd_StencilFuncSeparateATI;
uint32_t _mesa_unmarshal_StencilFuncSeparateATI(struct gl_context *ctx, const struct marshal_cmd_StencilFuncSeparateATI *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_StencilFuncSeparateATI(GLenum frontfunc, GLenum backfunc, GLint ref, GLuint mask);
struct marshal_cmd_ProgramEnvParameters4fvEXT;
uint32_t _mesa_unmarshal_ProgramEnvParameters4fvEXT(struct gl_context *ctx, const struct marshal_cmd_ProgramEnvParameters4fvEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ProgramEnvParameters4fvEXT(GLenum target, GLuint index, GLsizei count, const GLfloat * params);
struct marshal_cmd_ProgramLocalParameters4fvEXT;
uint32_t _mesa_unmarshal_ProgramLocalParameters4fvEXT(struct gl_context *ctx, const struct marshal_cmd_ProgramLocalParameters4fvEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ProgramLocalParameters4fvEXT(GLenum target, GLuint index, GLsizei count, const GLfloat * params);
struct marshal_cmd_InternalBufferSubDataCopyMESA;
uint32_t _mesa_unmarshal_InternalBufferSubDataCopyMESA(struct gl_context *ctx, const struct marshal_cmd_InternalBufferSubDataCopyMESA *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_InternalBufferSubDataCopyMESA(GLintptr srcBuffer, GLuint srcOffset, GLuint dstTargetOrName, GLintptr dstOffset, GLsizeiptr size, GLboolean named, GLboolean ext_dsa);
struct marshal_cmd_InternalSetError;
uint32_t _mesa_unmarshal_InternalSetError(struct gl_context *ctx, const struct marshal_cmd_InternalSetError *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_InternalSetError(GLenum error);
void GLAPIENTRY _mesa_marshal_EGLImageTargetTexture2DOES(GLenum target, GLvoid * writeOffset);
void GLAPIENTRY _mesa_marshal_EGLImageTargetRenderbufferStorageOES(GLenum target, GLvoid * writeOffset);
void GLAPIENTRY _mesa_marshal_EGLImageTargetTexStorageEXT(GLenum target, GLvoid * image, const GLint * attrib_list);
void GLAPIENTRY _mesa_marshal_EGLImageTargetTextureStorageEXT(GLuint texture, GLvoid * image, const GLint * attrib_list);
struct marshal_cmd_ClearColorIiEXT;
uint32_t _mesa_unmarshal_ClearColorIiEXT(struct gl_context *ctx, const struct marshal_cmd_ClearColorIiEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ClearColorIiEXT(GLint r, GLint g, GLint b, GLint a);
struct marshal_cmd_ClearColorIuiEXT;
uint32_t _mesa_unmarshal_ClearColorIuiEXT(struct gl_context *ctx, const struct marshal_cmd_ClearColorIuiEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ClearColorIuiEXT(GLuint r, GLuint g, GLuint b, GLuint a);
struct marshal_cmd_TexParameterIiv;
uint32_t _mesa_unmarshal_TexParameterIiv(struct gl_context *ctx, const struct marshal_cmd_TexParameterIiv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TexParameterIiv(GLenum target, GLenum pname, const GLint * params);
struct marshal_cmd_TexParameterIuiv;
uint32_t _mesa_unmarshal_TexParameterIuiv(struct gl_context *ctx, const struct marshal_cmd_TexParameterIuiv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TexParameterIuiv(GLenum target, GLenum pname, const GLuint * params);
void GLAPIENTRY _mesa_marshal_GetTexParameterIiv(GLenum target, GLenum pname, GLint * params);
void GLAPIENTRY _mesa_marshal_GetTexParameterIuiv(GLenum target, GLenum pname, GLuint * params);
struct marshal_cmd_VertexAttribI1iEXT;
uint32_t _mesa_unmarshal_VertexAttribI1iEXT(struct gl_context *ctx, const struct marshal_cmd_VertexAttribI1iEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttribI1iEXT(GLuint index, GLint x);
struct marshal_cmd_VertexAttribI2iEXT;
uint32_t _mesa_unmarshal_VertexAttribI2iEXT(struct gl_context *ctx, const struct marshal_cmd_VertexAttribI2iEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttribI2iEXT(GLuint index, GLint x, GLint y);
struct marshal_cmd_VertexAttribI3iEXT;
uint32_t _mesa_unmarshal_VertexAttribI3iEXT(struct gl_context *ctx, const struct marshal_cmd_VertexAttribI3iEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttribI3iEXT(GLuint index, GLint x, GLint y, GLint z);
struct marshal_cmd_VertexAttribI4iEXT;
uint32_t _mesa_unmarshal_VertexAttribI4iEXT(struct gl_context *ctx, const struct marshal_cmd_VertexAttribI4iEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttribI4iEXT(GLuint index, GLint x, GLint y, GLint z, GLint w);
struct marshal_cmd_VertexAttribI1uiEXT;
uint32_t _mesa_unmarshal_VertexAttribI1uiEXT(struct gl_context *ctx, const struct marshal_cmd_VertexAttribI1uiEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttribI1uiEXT(GLuint index, GLuint x);
struct marshal_cmd_VertexAttribI2uiEXT;
uint32_t _mesa_unmarshal_VertexAttribI2uiEXT(struct gl_context *ctx, const struct marshal_cmd_VertexAttribI2uiEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttribI2uiEXT(GLuint index, GLuint x, GLuint y);
struct marshal_cmd_VertexAttribI3uiEXT;
uint32_t _mesa_unmarshal_VertexAttribI3uiEXT(struct gl_context *ctx, const struct marshal_cmd_VertexAttribI3uiEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttribI3uiEXT(GLuint index, GLuint x, GLuint y, GLuint z);
struct marshal_cmd_VertexAttribI4uiEXT;
uint32_t _mesa_unmarshal_VertexAttribI4uiEXT(struct gl_context *ctx, const struct marshal_cmd_VertexAttribI4uiEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttribI4uiEXT(GLuint index, GLuint x, GLuint y, GLuint z, GLuint w);
struct marshal_cmd_VertexAttribI1iv;
uint32_t _mesa_unmarshal_VertexAttribI1iv(struct gl_context *ctx, const struct marshal_cmd_VertexAttribI1iv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttribI1iv(GLuint index, const GLint * v);
struct marshal_cmd_VertexAttribI2ivEXT;
uint32_t _mesa_unmarshal_VertexAttribI2ivEXT(struct gl_context *ctx, const struct marshal_cmd_VertexAttribI2ivEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttribI2ivEXT(GLuint index, const GLint * v);
struct marshal_cmd_VertexAttribI3ivEXT;
uint32_t _mesa_unmarshal_VertexAttribI3ivEXT(struct gl_context *ctx, const struct marshal_cmd_VertexAttribI3ivEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttribI3ivEXT(GLuint index, const GLint * v);
struct marshal_cmd_VertexAttribI4ivEXT;
uint32_t _mesa_unmarshal_VertexAttribI4ivEXT(struct gl_context *ctx, const struct marshal_cmd_VertexAttribI4ivEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttribI4ivEXT(GLuint index, const GLint * v);
struct marshal_cmd_VertexAttribI1uiv;
uint32_t _mesa_unmarshal_VertexAttribI1uiv(struct gl_context *ctx, const struct marshal_cmd_VertexAttribI1uiv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttribI1uiv(GLuint index, const GLuint * v);
struct marshal_cmd_VertexAttribI2uivEXT;
uint32_t _mesa_unmarshal_VertexAttribI2uivEXT(struct gl_context *ctx, const struct marshal_cmd_VertexAttribI2uivEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttribI2uivEXT(GLuint index, const GLuint * v);
struct marshal_cmd_VertexAttribI3uivEXT;
uint32_t _mesa_unmarshal_VertexAttribI3uivEXT(struct gl_context *ctx, const struct marshal_cmd_VertexAttribI3uivEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttribI3uivEXT(GLuint index, const GLuint * v);
struct marshal_cmd_VertexAttribI4uivEXT;
uint32_t _mesa_unmarshal_VertexAttribI4uivEXT(struct gl_context *ctx, const struct marshal_cmd_VertexAttribI4uivEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttribI4uivEXT(GLuint index, const GLuint * v);
struct marshal_cmd_VertexAttribI4bv;
uint32_t _mesa_unmarshal_VertexAttribI4bv(struct gl_context *ctx, const struct marshal_cmd_VertexAttribI4bv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttribI4bv(GLuint index, const GLbyte * v);
struct marshal_cmd_VertexAttribI4sv;
uint32_t _mesa_unmarshal_VertexAttribI4sv(struct gl_context *ctx, const struct marshal_cmd_VertexAttribI4sv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttribI4sv(GLuint index, const GLshort * v);
struct marshal_cmd_VertexAttribI4ubv;
uint32_t _mesa_unmarshal_VertexAttribI4ubv(struct gl_context *ctx, const struct marshal_cmd_VertexAttribI4ubv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttribI4ubv(GLuint index, const GLubyte * v);
struct marshal_cmd_VertexAttribI4usv;
uint32_t _mesa_unmarshal_VertexAttribI4usv(struct gl_context *ctx, const struct marshal_cmd_VertexAttribI4usv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttribI4usv(GLuint index, const GLushort * v);
struct marshal_cmd_VertexAttribIPointer;
uint32_t _mesa_unmarshal_VertexAttribIPointer(struct gl_context *ctx, const struct marshal_cmd_VertexAttribIPointer *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttribIPointer(GLuint index, GLint size, GLenum type, GLsizei stride, const GLvoid * pointer);
void GLAPIENTRY _mesa_marshal_GetVertexAttribIiv(GLuint index, GLenum pname, GLint * params);
void GLAPIENTRY _mesa_marshal_GetVertexAttribIuiv(GLuint index, GLenum pname, GLuint * params);
struct marshal_cmd_Uniform1ui;
uint32_t _mesa_unmarshal_Uniform1ui(struct gl_context *ctx, const struct marshal_cmd_Uniform1ui *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Uniform1ui(GLint location, GLuint x);
struct marshal_cmd_Uniform2ui;
uint32_t _mesa_unmarshal_Uniform2ui(struct gl_context *ctx, const struct marshal_cmd_Uniform2ui *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Uniform2ui(GLint location, GLuint x, GLuint y);
struct marshal_cmd_Uniform3ui;
uint32_t _mesa_unmarshal_Uniform3ui(struct gl_context *ctx, const struct marshal_cmd_Uniform3ui *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Uniform3ui(GLint location, GLuint x, GLuint y, GLuint z);
struct marshal_cmd_Uniform4ui;
uint32_t _mesa_unmarshal_Uniform4ui(struct gl_context *ctx, const struct marshal_cmd_Uniform4ui *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Uniform4ui(GLint location, GLuint x, GLuint y, GLuint z, GLuint w);
struct marshal_cmd_Uniform1uiv;
uint32_t _mesa_unmarshal_Uniform1uiv(struct gl_context *ctx, const struct marshal_cmd_Uniform1uiv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Uniform1uiv(GLint location, GLsizei count, const GLuint * value);
struct marshal_cmd_Uniform2uiv;
uint32_t _mesa_unmarshal_Uniform2uiv(struct gl_context *ctx, const struct marshal_cmd_Uniform2uiv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Uniform2uiv(GLint location, GLsizei count, const GLuint * value);
struct marshal_cmd_Uniform3uiv;
uint32_t _mesa_unmarshal_Uniform3uiv(struct gl_context *ctx, const struct marshal_cmd_Uniform3uiv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Uniform3uiv(GLint location, GLsizei count, const GLuint * value);
struct marshal_cmd_Uniform4uiv;
uint32_t _mesa_unmarshal_Uniform4uiv(struct gl_context *ctx, const struct marshal_cmd_Uniform4uiv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Uniform4uiv(GLint location, GLsizei count, const GLuint * value);
void GLAPIENTRY _mesa_marshal_GetUniformuiv(GLuint program, GLint location, GLuint * params);
struct marshal_cmd_BindFragDataLocation;
uint32_t _mesa_unmarshal_BindFragDataLocation(struct gl_context *ctx, const struct marshal_cmd_BindFragDataLocation *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_BindFragDataLocation(GLuint program, GLuint colorNumber, const GLchar * name);
GLint GLAPIENTRY _mesa_marshal_GetFragDataLocation(GLuint program, const GLchar * name);
struct marshal_cmd_ClearBufferiv;
uint32_t _mesa_unmarshal_ClearBufferiv(struct gl_context *ctx, const struct marshal_cmd_ClearBufferiv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ClearBufferiv(GLenum buffer, GLint drawbuffer, const GLint * value);
struct marshal_cmd_ClearBufferuiv;
uint32_t _mesa_unmarshal_ClearBufferuiv(struct gl_context *ctx, const struct marshal_cmd_ClearBufferuiv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ClearBufferuiv(GLenum buffer, GLint drawbuffer, const GLuint * value);
struct marshal_cmd_ClearBufferfv;
uint32_t _mesa_unmarshal_ClearBufferfv(struct gl_context *ctx, const struct marshal_cmd_ClearBufferfv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ClearBufferfv(GLenum buffer, GLint drawbuffer, const GLfloat * value);
struct marshal_cmd_ClearBufferfi;
uint32_t _mesa_unmarshal_ClearBufferfi(struct gl_context *ctx, const struct marshal_cmd_ClearBufferfi *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ClearBufferfi(GLenum buffer, GLint drawbuffer, GLfloat depth, GLint stencil);
const GLubyte * GLAPIENTRY _mesa_marshal_GetStringi(GLenum name, GLuint index);
struct marshal_cmd_BeginTransformFeedback;
uint32_t _mesa_unmarshal_BeginTransformFeedback(struct gl_context *ctx, const struct marshal_cmd_BeginTransformFeedback *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_BeginTransformFeedback(GLenum mode);
struct marshal_cmd_EndTransformFeedback;
uint32_t _mesa_unmarshal_EndTransformFeedback(struct gl_context *ctx, const struct marshal_cmd_EndTransformFeedback *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_EndTransformFeedback(void);
struct marshal_cmd_BindBufferRange;
uint32_t _mesa_unmarshal_BindBufferRange(struct gl_context *ctx, const struct marshal_cmd_BindBufferRange *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_BindBufferRange(GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size);
struct marshal_cmd_BindBufferBase;
uint32_t _mesa_unmarshal_BindBufferBase(struct gl_context *ctx, const struct marshal_cmd_BindBufferBase *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_BindBufferBase(GLenum target, GLuint index, GLuint buffer);
void GLAPIENTRY _mesa_marshal_TransformFeedbackVaryings(GLuint program, GLsizei count, const GLchar * const * varyings, GLenum bufferMode);
void GLAPIENTRY _mesa_marshal_GetTransformFeedbackVarying(GLuint program, GLuint index, GLsizei bufSize, GLsizei * length, GLsizei * size, GLenum * type, GLchar * name);
struct marshal_cmd_BeginConditionalRender;
uint32_t _mesa_unmarshal_BeginConditionalRender(struct gl_context *ctx, const struct marshal_cmd_BeginConditionalRender *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_BeginConditionalRender(GLuint query, GLenum mode);
struct marshal_cmd_EndConditionalRender;
uint32_t _mesa_unmarshal_EndConditionalRender(struct gl_context *ctx, const struct marshal_cmd_EndConditionalRender *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_EndConditionalRender(void);
struct marshal_cmd_PrimitiveRestartIndex;
uint32_t _mesa_unmarshal_PrimitiveRestartIndex(struct gl_context *ctx, const struct marshal_cmd_PrimitiveRestartIndex *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_PrimitiveRestartIndex(GLuint index);
void GLAPIENTRY _mesa_marshal_GetInteger64i_v(GLenum cap, GLuint index, GLint64 * data);
void GLAPIENTRY _mesa_marshal_GetBufferParameteri64v(GLenum target, GLenum pname, GLint64 * params);
struct marshal_cmd_FramebufferTexture;
uint32_t _mesa_unmarshal_FramebufferTexture(struct gl_context *ctx, const struct marshal_cmd_FramebufferTexture *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_FramebufferTexture(GLenum target, GLenum attachment, GLuint texture, GLint level);
struct marshal_cmd_PrimitiveRestartNV;
uint32_t _mesa_unmarshal_PrimitiveRestartNV(struct gl_context *ctx, const struct marshal_cmd_PrimitiveRestartNV *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_PrimitiveRestartNV(void);
struct marshal_cmd_BindBufferOffsetEXT;
uint32_t _mesa_unmarshal_BindBufferOffsetEXT(struct gl_context *ctx, const struct marshal_cmd_BindBufferOffsetEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_BindBufferOffsetEXT(GLenum target, GLuint index, GLuint buffer, GLintptr offset);
struct marshal_cmd_BindTransformFeedback;
uint32_t _mesa_unmarshal_BindTransformFeedback(struct gl_context *ctx, const struct marshal_cmd_BindTransformFeedback *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_BindTransformFeedback(GLenum target, GLuint id);
struct marshal_cmd_DeleteTransformFeedbacks;
uint32_t _mesa_unmarshal_DeleteTransformFeedbacks(struct gl_context *ctx, const struct marshal_cmd_DeleteTransformFeedbacks *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_DeleteTransformFeedbacks(GLsizei n, const GLuint * ids);
void GLAPIENTRY _mesa_marshal_GenTransformFeedbacks(GLsizei n, GLuint * ids);
GLboolean GLAPIENTRY _mesa_marshal_IsTransformFeedback(GLuint id);
struct marshal_cmd_PauseTransformFeedback;
uint32_t _mesa_unmarshal_PauseTransformFeedback(struct gl_context *ctx, const struct marshal_cmd_PauseTransformFeedback *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_PauseTransformFeedback(void);
struct marshal_cmd_ResumeTransformFeedback;
uint32_t _mesa_unmarshal_ResumeTransformFeedback(struct gl_context *ctx, const struct marshal_cmd_ResumeTransformFeedback *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ResumeTransformFeedback(void);
struct marshal_cmd_DrawTransformFeedback;
uint32_t _mesa_unmarshal_DrawTransformFeedback(struct gl_context *ctx, const struct marshal_cmd_DrawTransformFeedback *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_DrawTransformFeedback(GLenum mode, GLuint id);
void GLAPIENTRY _mesa_marshal_VDPAUInitNV(const GLvoid * vdpDevice, const GLvoid * getProcAddress);
struct marshal_cmd_VDPAUFiniNV;
uint32_t _mesa_unmarshal_VDPAUFiniNV(struct gl_context *ctx, const struct marshal_cmd_VDPAUFiniNV *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VDPAUFiniNV(void);
GLintptr GLAPIENTRY _mesa_marshal_VDPAURegisterVideoSurfaceNV(const GLvoid * vdpSurface, GLenum target, GLsizei numTextureNames, const GLuint * textureNames);
GLintptr GLAPIENTRY _mesa_marshal_VDPAURegisterOutputSurfaceNV(const GLvoid * vdpSurface, GLenum target, GLsizei numTextureNames, const GLuint * textureNames);
GLboolean GLAPIENTRY _mesa_marshal_VDPAUIsSurfaceNV(GLintptr surface);
struct marshal_cmd_VDPAUUnregisterSurfaceNV;
uint32_t _mesa_unmarshal_VDPAUUnregisterSurfaceNV(struct gl_context *ctx, const struct marshal_cmd_VDPAUUnregisterSurfaceNV *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VDPAUUnregisterSurfaceNV(GLintptr surface);
void GLAPIENTRY _mesa_marshal_VDPAUGetSurfaceivNV(GLintptr surface, GLenum pname, GLsizei bufSize, GLsizei * length, GLint * values);
struct marshal_cmd_VDPAUSurfaceAccessNV;
uint32_t _mesa_unmarshal_VDPAUSurfaceAccessNV(struct gl_context *ctx, const struct marshal_cmd_VDPAUSurfaceAccessNV *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VDPAUSurfaceAccessNV(GLintptr surface, GLenum access);
struct marshal_cmd_VDPAUMapSurfacesNV;
uint32_t _mesa_unmarshal_VDPAUMapSurfacesNV(struct gl_context *ctx, const struct marshal_cmd_VDPAUMapSurfacesNV *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VDPAUMapSurfacesNV(GLsizei numSurfaces, const GLintptr * surfaces);
struct marshal_cmd_VDPAUUnmapSurfacesNV;
uint32_t _mesa_unmarshal_VDPAUUnmapSurfacesNV(struct gl_context *ctx, const struct marshal_cmd_VDPAUUnmapSurfacesNV *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VDPAUUnmapSurfacesNV(GLsizei numSurfaces, const GLintptr * surfaces);
void GLAPIENTRY _mesa_marshal_GetUnsignedBytevEXT(GLenum pname, GLubyte * data);
void GLAPIENTRY _mesa_marshal_GetUnsignedBytei_vEXT(GLenum target, GLuint index, GLubyte * data);
struct marshal_cmd_DeleteMemoryObjectsEXT;
uint32_t _mesa_unmarshal_DeleteMemoryObjectsEXT(struct gl_context *ctx, const struct marshal_cmd_DeleteMemoryObjectsEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_DeleteMemoryObjectsEXT(GLsizei n, const GLuint * memoryObjects);
GLboolean GLAPIENTRY _mesa_marshal_IsMemoryObjectEXT(GLuint memoryObject);
void GLAPIENTRY _mesa_marshal_CreateMemoryObjectsEXT(GLsizei n, GLuint * memoryObjects);
struct marshal_cmd_MemoryObjectParameterivEXT;
uint32_t _mesa_unmarshal_MemoryObjectParameterivEXT(struct gl_context *ctx, const struct marshal_cmd_MemoryObjectParameterivEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_MemoryObjectParameterivEXT(GLuint memoryObject, GLenum pname, const GLint * params);
void GLAPIENTRY _mesa_marshal_GetMemoryObjectParameterivEXT(GLuint memoryObject, GLenum pname, GLint * params);
struct marshal_cmd_TexStorageMem2DEXT;
uint32_t _mesa_unmarshal_TexStorageMem2DEXT(struct gl_context *ctx, const struct marshal_cmd_TexStorageMem2DEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TexStorageMem2DEXT(GLenum target, GLsizei levels, GLenum internalFormat, GLsizei width, GLsizei height, GLuint memory, GLuint64 offset);
struct marshal_cmd_TexStorageMem2DMultisampleEXT;
uint32_t _mesa_unmarshal_TexStorageMem2DMultisampleEXT(struct gl_context *ctx, const struct marshal_cmd_TexStorageMem2DMultisampleEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TexStorageMem2DMultisampleEXT(GLenum target, GLsizei samples, GLenum internalFormat, GLsizei width, GLsizei height, GLboolean fixedSampleLocations, GLuint memory, GLuint64 offset);
struct marshal_cmd_TexStorageMem3DEXT;
uint32_t _mesa_unmarshal_TexStorageMem3DEXT(struct gl_context *ctx, const struct marshal_cmd_TexStorageMem3DEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TexStorageMem3DEXT(GLenum target, GLsizei levels, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLuint memory, GLuint64 offset);
struct marshal_cmd_TexStorageMem3DMultisampleEXT;
uint32_t _mesa_unmarshal_TexStorageMem3DMultisampleEXT(struct gl_context *ctx, const struct marshal_cmd_TexStorageMem3DMultisampleEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TexStorageMem3DMultisampleEXT(GLenum target, GLsizei samples, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedSampleLocations, GLuint memory, GLuint64 offset);
struct marshal_cmd_BufferStorageMemEXT;
uint32_t _mesa_unmarshal_BufferStorageMemEXT(struct gl_context *ctx, const struct marshal_cmd_BufferStorageMemEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_BufferStorageMemEXT(GLenum target, GLsizeiptr size, GLuint memory, GLuint64 offset);
struct marshal_cmd_TextureStorageMem2DEXT;
uint32_t _mesa_unmarshal_TextureStorageMem2DEXT(struct gl_context *ctx, const struct marshal_cmd_TextureStorageMem2DEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TextureStorageMem2DEXT(GLenum texture, GLsizei levels, GLenum internalFormat, GLsizei width, GLsizei height, GLuint memory, GLuint64 offset);
struct marshal_cmd_TextureStorageMem2DMultisampleEXT;
uint32_t _mesa_unmarshal_TextureStorageMem2DMultisampleEXT(struct gl_context *ctx, const struct marshal_cmd_TextureStorageMem2DMultisampleEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TextureStorageMem2DMultisampleEXT(GLuint texture, GLsizei samples, GLenum internalFormat, GLsizei width, GLsizei height, GLboolean fixedSampleLocations, GLuint memory, GLuint64 offset);
struct marshal_cmd_TextureStorageMem3DEXT;
uint32_t _mesa_unmarshal_TextureStorageMem3DEXT(struct gl_context *ctx, const struct marshal_cmd_TextureStorageMem3DEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TextureStorageMem3DEXT(GLuint texture, GLsizei levels, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLuint memory, GLuint64 offset);
struct marshal_cmd_TextureStorageMem3DMultisampleEXT;
uint32_t _mesa_unmarshal_TextureStorageMem3DMultisampleEXT(struct gl_context *ctx, const struct marshal_cmd_TextureStorageMem3DMultisampleEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TextureStorageMem3DMultisampleEXT(GLuint texture, GLsizei samples, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedSampleLocations, GLuint memory, GLuint64 offset);
struct marshal_cmd_NamedBufferStorageMemEXT;
uint32_t _mesa_unmarshal_NamedBufferStorageMemEXT(struct gl_context *ctx, const struct marshal_cmd_NamedBufferStorageMemEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_NamedBufferStorageMemEXT(GLuint buffer, GLsizeiptr size, GLuint memory, GLuint64 offset);
struct marshal_cmd_TexStorageMem1DEXT;
uint32_t _mesa_unmarshal_TexStorageMem1DEXT(struct gl_context *ctx, const struct marshal_cmd_TexStorageMem1DEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TexStorageMem1DEXT(GLenum target, GLsizei levels, GLenum internalFormat, GLsizei width, GLuint memory, GLuint64 offset);
struct marshal_cmd_TextureStorageMem1DEXT;
uint32_t _mesa_unmarshal_TextureStorageMem1DEXT(struct gl_context *ctx, const struct marshal_cmd_TextureStorageMem1DEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TextureStorageMem1DEXT(GLuint texture, GLsizei levels, GLenum internalFormat, GLsizei width, GLuint memory, GLuint64 offset);
void GLAPIENTRY _mesa_marshal_GenSemaphoresEXT(GLsizei n, GLuint * semaphores);
struct marshal_cmd_DeleteSemaphoresEXT;
uint32_t _mesa_unmarshal_DeleteSemaphoresEXT(struct gl_context *ctx, const struct marshal_cmd_DeleteSemaphoresEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_DeleteSemaphoresEXT(GLsizei n, const GLuint * semaphores);
GLboolean GLAPIENTRY _mesa_marshal_IsSemaphoreEXT(GLuint semaphore);
struct marshal_cmd_SemaphoreParameterui64vEXT;
uint32_t _mesa_unmarshal_SemaphoreParameterui64vEXT(struct gl_context *ctx, const struct marshal_cmd_SemaphoreParameterui64vEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_SemaphoreParameterui64vEXT(GLuint semaphore, GLenum pname, const GLuint64 * params);
void GLAPIENTRY _mesa_marshal_GetSemaphoreParameterui64vEXT(GLuint semaphore, GLenum pname, GLuint64 * params);
struct marshal_cmd_WaitSemaphoreEXT;
uint32_t _mesa_unmarshal_WaitSemaphoreEXT(struct gl_context *ctx, const struct marshal_cmd_WaitSemaphoreEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_WaitSemaphoreEXT(GLuint semaphore, GLuint numBufferBarriers, const GLuint * buffers, GLuint numTextureBarriers, const GLuint * textures, const GLenum * srcLayouts);
struct marshal_cmd_SignalSemaphoreEXT;
uint32_t _mesa_unmarshal_SignalSemaphoreEXT(struct gl_context *ctx, const struct marshal_cmd_SignalSemaphoreEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_SignalSemaphoreEXT(GLuint semaphore, GLuint numBufferBarriers, const GLuint * buffers, GLuint numTextureBarriers, const GLuint * textures, const GLenum * dstLayouts);
struct marshal_cmd_ImportMemoryFdEXT;
uint32_t _mesa_unmarshal_ImportMemoryFdEXT(struct gl_context *ctx, const struct marshal_cmd_ImportMemoryFdEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ImportMemoryFdEXT(GLuint memory, GLuint64 size, GLenum handleType, GLint fd);
struct marshal_cmd_ImportSemaphoreFdEXT;
uint32_t _mesa_unmarshal_ImportSemaphoreFdEXT(struct gl_context *ctx, const struct marshal_cmd_ImportSemaphoreFdEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ImportSemaphoreFdEXT(GLuint semaphore, GLenum handleType, GLint fd);
struct marshal_cmd_ViewportSwizzleNV;
uint32_t _mesa_unmarshal_ViewportSwizzleNV(struct gl_context *ctx, const struct marshal_cmd_ViewportSwizzleNV *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ViewportSwizzleNV(GLuint index, GLenum swizzlex, GLenum swizzley, GLenum swizzlez, GLenum swizzlew);
struct marshal_cmd_Vertex2hNV;
uint32_t _mesa_unmarshal_Vertex2hNV(struct gl_context *ctx, const struct marshal_cmd_Vertex2hNV *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Vertex2hNV(GLhalfNV x, GLhalfNV y);
struct marshal_cmd_Vertex2hvNV;
uint32_t _mesa_unmarshal_Vertex2hvNV(struct gl_context *ctx, const struct marshal_cmd_Vertex2hvNV *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Vertex2hvNV(const GLhalfNV * v);
struct marshal_cmd_Vertex3hNV;
uint32_t _mesa_unmarshal_Vertex3hNV(struct gl_context *ctx, const struct marshal_cmd_Vertex3hNV *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Vertex3hNV(GLhalfNV x, GLhalfNV y, GLhalfNV z);
struct marshal_cmd_Vertex3hvNV;
uint32_t _mesa_unmarshal_Vertex3hvNV(struct gl_context *ctx, const struct marshal_cmd_Vertex3hvNV *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Vertex3hvNV(const GLhalfNV * v);
struct marshal_cmd_Vertex4hNV;
uint32_t _mesa_unmarshal_Vertex4hNV(struct gl_context *ctx, const struct marshal_cmd_Vertex4hNV *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Vertex4hNV(GLhalfNV x, GLhalfNV y, GLhalfNV z, GLhalfNV w);
struct marshal_cmd_Vertex4hvNV;
uint32_t _mesa_unmarshal_Vertex4hvNV(struct gl_context *ctx, const struct marshal_cmd_Vertex4hvNV *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Vertex4hvNV(const GLhalfNV * v);
struct marshal_cmd_Normal3hNV;
uint32_t _mesa_unmarshal_Normal3hNV(struct gl_context *ctx, const struct marshal_cmd_Normal3hNV *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Normal3hNV(GLhalfNV nx, GLhalfNV ny, GLhalfNV nz);
struct marshal_cmd_Normal3hvNV;
uint32_t _mesa_unmarshal_Normal3hvNV(struct gl_context *ctx, const struct marshal_cmd_Normal3hvNV *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Normal3hvNV(const GLhalfNV * v);
struct marshal_cmd_Color3hNV;
uint32_t _mesa_unmarshal_Color3hNV(struct gl_context *ctx, const struct marshal_cmd_Color3hNV *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Color3hNV(GLhalfNV red, GLhalfNV green, GLhalfNV blue);
struct marshal_cmd_Color3hvNV;
uint32_t _mesa_unmarshal_Color3hvNV(struct gl_context *ctx, const struct marshal_cmd_Color3hvNV *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Color3hvNV(const GLhalfNV * v);
struct marshal_cmd_Color4hNV;
uint32_t _mesa_unmarshal_Color4hNV(struct gl_context *ctx, const struct marshal_cmd_Color4hNV *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Color4hNV(GLhalfNV red, GLhalfNV green, GLhalfNV blue, GLhalfNV alpha);
struct marshal_cmd_Color4hvNV;
uint32_t _mesa_unmarshal_Color4hvNV(struct gl_context *ctx, const struct marshal_cmd_Color4hvNV *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Color4hvNV(const GLhalfNV * v);
struct marshal_cmd_TexCoord1hNV;
uint32_t _mesa_unmarshal_TexCoord1hNV(struct gl_context *ctx, const struct marshal_cmd_TexCoord1hNV *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TexCoord1hNV(GLhalfNV s);
struct marshal_cmd_TexCoord1hvNV;
uint32_t _mesa_unmarshal_TexCoord1hvNV(struct gl_context *ctx, const struct marshal_cmd_TexCoord1hvNV *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TexCoord1hvNV(const GLhalfNV * v);
struct marshal_cmd_TexCoord2hNV;
uint32_t _mesa_unmarshal_TexCoord2hNV(struct gl_context *ctx, const struct marshal_cmd_TexCoord2hNV *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TexCoord2hNV(GLhalfNV s, GLhalfNV t);
struct marshal_cmd_TexCoord2hvNV;
uint32_t _mesa_unmarshal_TexCoord2hvNV(struct gl_context *ctx, const struct marshal_cmd_TexCoord2hvNV *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TexCoord2hvNV(const GLhalfNV * v);
struct marshal_cmd_TexCoord3hNV;
uint32_t _mesa_unmarshal_TexCoord3hNV(struct gl_context *ctx, const struct marshal_cmd_TexCoord3hNV *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TexCoord3hNV(GLhalfNV s, GLhalfNV t, GLhalfNV r);
struct marshal_cmd_TexCoord3hvNV;
uint32_t _mesa_unmarshal_TexCoord3hvNV(struct gl_context *ctx, const struct marshal_cmd_TexCoord3hvNV *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TexCoord3hvNV(const GLhalfNV * v);
struct marshal_cmd_TexCoord4hNV;
uint32_t _mesa_unmarshal_TexCoord4hNV(struct gl_context *ctx, const struct marshal_cmd_TexCoord4hNV *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TexCoord4hNV(GLhalfNV s, GLhalfNV t, GLhalfNV r, GLhalfNV q);
struct marshal_cmd_TexCoord4hvNV;
uint32_t _mesa_unmarshal_TexCoord4hvNV(struct gl_context *ctx, const struct marshal_cmd_TexCoord4hvNV *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TexCoord4hvNV(const GLhalfNV * v);
struct marshal_cmd_MultiTexCoord1hNV;
uint32_t _mesa_unmarshal_MultiTexCoord1hNV(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord1hNV *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_MultiTexCoord1hNV(GLenum target, GLhalfNV s);
struct marshal_cmd_MultiTexCoord1hvNV;
uint32_t _mesa_unmarshal_MultiTexCoord1hvNV(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord1hvNV *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_MultiTexCoord1hvNV(GLenum target, const GLhalfNV * v);
struct marshal_cmd_MultiTexCoord2hNV;
uint32_t _mesa_unmarshal_MultiTexCoord2hNV(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord2hNV *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_MultiTexCoord2hNV(GLenum target, GLhalfNV s, GLhalfNV t);
struct marshal_cmd_MultiTexCoord2hvNV;
uint32_t _mesa_unmarshal_MultiTexCoord2hvNV(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord2hvNV *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_MultiTexCoord2hvNV(GLenum target, const GLhalfNV * v);
struct marshal_cmd_MultiTexCoord3hNV;
uint32_t _mesa_unmarshal_MultiTexCoord3hNV(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord3hNV *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_MultiTexCoord3hNV(GLenum target, GLhalfNV s, GLhalfNV t, GLhalfNV r);
struct marshal_cmd_MultiTexCoord3hvNV;
uint32_t _mesa_unmarshal_MultiTexCoord3hvNV(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord3hvNV *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_MultiTexCoord3hvNV(GLenum target, const GLhalfNV * v);
struct marshal_cmd_MultiTexCoord4hNV;
uint32_t _mesa_unmarshal_MultiTexCoord4hNV(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord4hNV *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_MultiTexCoord4hNV(GLenum target, GLhalfNV s, GLhalfNV t, GLhalfNV r, GLhalfNV q);
struct marshal_cmd_MultiTexCoord4hvNV;
uint32_t _mesa_unmarshal_MultiTexCoord4hvNV(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord4hvNV *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_MultiTexCoord4hvNV(GLenum target, const GLhalfNV * v);
struct marshal_cmd_VertexAttrib1hNV;
uint32_t _mesa_unmarshal_VertexAttrib1hNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib1hNV *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttrib1hNV(GLuint index, GLhalfNV x);
struct marshal_cmd_VertexAttrib1hvNV;
uint32_t _mesa_unmarshal_VertexAttrib1hvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib1hvNV *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttrib1hvNV(GLuint index, const GLhalfNV * v);
struct marshal_cmd_VertexAttrib2hNV;
uint32_t _mesa_unmarshal_VertexAttrib2hNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib2hNV *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttrib2hNV(GLuint index, GLhalfNV x, GLhalfNV y);
struct marshal_cmd_VertexAttrib2hvNV;
uint32_t _mesa_unmarshal_VertexAttrib2hvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib2hvNV *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttrib2hvNV(GLuint index, const GLhalfNV * v);
struct marshal_cmd_VertexAttrib3hNV;
uint32_t _mesa_unmarshal_VertexAttrib3hNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib3hNV *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttrib3hNV(GLuint index, GLhalfNV x, GLhalfNV y, GLhalfNV z);
struct marshal_cmd_VertexAttrib3hvNV;
uint32_t _mesa_unmarshal_VertexAttrib3hvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib3hvNV *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttrib3hvNV(GLuint index, const GLhalfNV * v);
struct marshal_cmd_VertexAttrib4hNV;
uint32_t _mesa_unmarshal_VertexAttrib4hNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4hNV *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttrib4hNV(GLuint index, GLhalfNV x, GLhalfNV y, GLhalfNV z, GLhalfNV w);
struct marshal_cmd_VertexAttrib4hvNV;
uint32_t _mesa_unmarshal_VertexAttrib4hvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4hvNV *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttrib4hvNV(GLuint index, const GLhalfNV * v);
struct marshal_cmd_VertexAttribs1hvNV;
uint32_t _mesa_unmarshal_VertexAttribs1hvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttribs1hvNV *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttribs1hvNV(GLuint index, GLsizei n, const GLhalfNV * v);
struct marshal_cmd_VertexAttribs2hvNV;
uint32_t _mesa_unmarshal_VertexAttribs2hvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttribs2hvNV *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttribs2hvNV(GLuint index, GLsizei n, const GLhalfNV * v);
struct marshal_cmd_VertexAttribs3hvNV;
uint32_t _mesa_unmarshal_VertexAttribs3hvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttribs3hvNV *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttribs3hvNV(GLuint index, GLsizei n, const GLhalfNV * v);
struct marshal_cmd_VertexAttribs4hvNV;
uint32_t _mesa_unmarshal_VertexAttribs4hvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttribs4hvNV *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_VertexAttribs4hvNV(GLuint index, GLsizei n, const GLhalfNV * v);
struct marshal_cmd_FogCoordhNV;
uint32_t _mesa_unmarshal_FogCoordhNV(struct gl_context *ctx, const struct marshal_cmd_FogCoordhNV *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_FogCoordhNV(GLhalfNV x);
struct marshal_cmd_FogCoordhvNV;
uint32_t _mesa_unmarshal_FogCoordhvNV(struct gl_context *ctx, const struct marshal_cmd_FogCoordhvNV *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_FogCoordhvNV(const GLhalfNV * v);
struct marshal_cmd_SecondaryColor3hNV;
uint32_t _mesa_unmarshal_SecondaryColor3hNV(struct gl_context *ctx, const struct marshal_cmd_SecondaryColor3hNV *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_SecondaryColor3hNV(GLhalfNV red, GLhalfNV green, GLhalfNV blue);
struct marshal_cmd_SecondaryColor3hvNV;
uint32_t _mesa_unmarshal_SecondaryColor3hvNV(struct gl_context *ctx, const struct marshal_cmd_SecondaryColor3hvNV *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_SecondaryColor3hvNV(const GLhalfNV * v);
struct marshal_cmd_MemoryBarrierByRegion;
uint32_t _mesa_unmarshal_MemoryBarrierByRegion(struct gl_context *ctx, const struct marshal_cmd_MemoryBarrierByRegion *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_MemoryBarrierByRegion(GLbitfield barriers);
struct marshal_cmd_AlphaFuncx;
uint32_t _mesa_unmarshal_AlphaFuncx(struct gl_context *ctx, const struct marshal_cmd_AlphaFuncx *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_AlphaFuncx(GLenum func, GLclampx ref);
struct marshal_cmd_ClearColorx;
uint32_t _mesa_unmarshal_ClearColorx(struct gl_context *ctx, const struct marshal_cmd_ClearColorx *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ClearColorx(GLclampx red, GLclampx green, GLclampx blue, GLclampx alpha);
struct marshal_cmd_ClearDepthx;
uint32_t _mesa_unmarshal_ClearDepthx(struct gl_context *ctx, const struct marshal_cmd_ClearDepthx *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ClearDepthx(GLclampx depth);
struct marshal_cmd_Color4x;
uint32_t _mesa_unmarshal_Color4x(struct gl_context *ctx, const struct marshal_cmd_Color4x *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Color4x(GLfixed red, GLfixed green, GLfixed blue, GLfixed alpha);
struct marshal_cmd_DepthRangex;
uint32_t _mesa_unmarshal_DepthRangex(struct gl_context *ctx, const struct marshal_cmd_DepthRangex *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_DepthRangex(GLclampx zNear, GLclampx zFar);
struct marshal_cmd_Fogx;
uint32_t _mesa_unmarshal_Fogx(struct gl_context *ctx, const struct marshal_cmd_Fogx *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Fogx(GLenum pname, GLfixed param);
struct marshal_cmd_Fogxv;
uint32_t _mesa_unmarshal_Fogxv(struct gl_context *ctx, const struct marshal_cmd_Fogxv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Fogxv(GLenum pname, const GLfixed * params);
struct marshal_cmd_Frustumx;
uint32_t _mesa_unmarshal_Frustumx(struct gl_context *ctx, const struct marshal_cmd_Frustumx *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Frustumx(GLfixed left, GLfixed right, GLfixed bottom, GLfixed top, GLfixed zNear, GLfixed zFar);
struct marshal_cmd_LightModelx;
uint32_t _mesa_unmarshal_LightModelx(struct gl_context *ctx, const struct marshal_cmd_LightModelx *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_LightModelx(GLenum pname, GLfixed param);
struct marshal_cmd_LightModelxv;
uint32_t _mesa_unmarshal_LightModelxv(struct gl_context *ctx, const struct marshal_cmd_LightModelxv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_LightModelxv(GLenum pname, const GLfixed * params);
struct marshal_cmd_Lightx;
uint32_t _mesa_unmarshal_Lightx(struct gl_context *ctx, const struct marshal_cmd_Lightx *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Lightx(GLenum light, GLenum pname, GLfixed param);
struct marshal_cmd_Lightxv;
uint32_t _mesa_unmarshal_Lightxv(struct gl_context *ctx, const struct marshal_cmd_Lightxv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Lightxv(GLenum light, GLenum pname, const GLfixed * params);
struct marshal_cmd_LineWidthx;
uint32_t _mesa_unmarshal_LineWidthx(struct gl_context *ctx, const struct marshal_cmd_LineWidthx *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_LineWidthx(GLfixed width);
struct marshal_cmd_LoadMatrixx;
uint32_t _mesa_unmarshal_LoadMatrixx(struct gl_context *ctx, const struct marshal_cmd_LoadMatrixx *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_LoadMatrixx(const GLfixed * m);
struct marshal_cmd_Materialx;
uint32_t _mesa_unmarshal_Materialx(struct gl_context *ctx, const struct marshal_cmd_Materialx *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Materialx(GLenum face, GLenum pname, GLfixed param);
struct marshal_cmd_Materialxv;
uint32_t _mesa_unmarshal_Materialxv(struct gl_context *ctx, const struct marshal_cmd_Materialxv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Materialxv(GLenum face, GLenum pname, const GLfixed * params);
struct marshal_cmd_MultMatrixx;
uint32_t _mesa_unmarshal_MultMatrixx(struct gl_context *ctx, const struct marshal_cmd_MultMatrixx *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_MultMatrixx(const GLfixed * m);
struct marshal_cmd_MultiTexCoord4x;
uint32_t _mesa_unmarshal_MultiTexCoord4x(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord4x *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_MultiTexCoord4x(GLenum target, GLfixed s, GLfixed t, GLfixed r, GLfixed q);
struct marshal_cmd_Normal3x;
uint32_t _mesa_unmarshal_Normal3x(struct gl_context *ctx, const struct marshal_cmd_Normal3x *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Normal3x(GLfixed nx, GLfixed ny, GLfixed nz);
struct marshal_cmd_Orthox;
uint32_t _mesa_unmarshal_Orthox(struct gl_context *ctx, const struct marshal_cmd_Orthox *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Orthox(GLfixed left, GLfixed right, GLfixed bottom, GLfixed top, GLfixed zNear, GLfixed zFar);
struct marshal_cmd_PointSizex;
uint32_t _mesa_unmarshal_PointSizex(struct gl_context *ctx, const struct marshal_cmd_PointSizex *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_PointSizex(GLfixed size);
struct marshal_cmd_PolygonOffsetx;
uint32_t _mesa_unmarshal_PolygonOffsetx(struct gl_context *ctx, const struct marshal_cmd_PolygonOffsetx *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_PolygonOffsetx(GLfixed factor, GLfixed units);
struct marshal_cmd_Rotatex;
uint32_t _mesa_unmarshal_Rotatex(struct gl_context *ctx, const struct marshal_cmd_Rotatex *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Rotatex(GLfixed angle, GLfixed x, GLfixed y, GLfixed z);
struct marshal_cmd_SampleCoveragex;
uint32_t _mesa_unmarshal_SampleCoveragex(struct gl_context *ctx, const struct marshal_cmd_SampleCoveragex *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_SampleCoveragex(GLclampx value, GLboolean invert);
struct marshal_cmd_Scalex;
uint32_t _mesa_unmarshal_Scalex(struct gl_context *ctx, const struct marshal_cmd_Scalex *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Scalex(GLfixed x, GLfixed y, GLfixed z);
struct marshal_cmd_TexEnvx;
uint32_t _mesa_unmarshal_TexEnvx(struct gl_context *ctx, const struct marshal_cmd_TexEnvx *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TexEnvx(GLenum target, GLenum pname, GLfixed param);
struct marshal_cmd_TexEnvxv;
uint32_t _mesa_unmarshal_TexEnvxv(struct gl_context *ctx, const struct marshal_cmd_TexEnvxv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TexEnvxv(GLenum target, GLenum pname, const GLfixed * params);
struct marshal_cmd_TexParameterx;
uint32_t _mesa_unmarshal_TexParameterx(struct gl_context *ctx, const struct marshal_cmd_TexParameterx *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TexParameterx(GLenum target, GLenum pname, GLfixed param);
struct marshal_cmd_Translatex;
uint32_t _mesa_unmarshal_Translatex(struct gl_context *ctx, const struct marshal_cmd_Translatex *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Translatex(GLfixed x, GLfixed y, GLfixed z);
struct marshal_cmd_ClipPlanex;
uint32_t _mesa_unmarshal_ClipPlanex(struct gl_context *ctx, const struct marshal_cmd_ClipPlanex *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ClipPlanex(GLenum plane, const GLfixed * equation);
void GLAPIENTRY _mesa_marshal_GetClipPlanex(GLenum plane, GLfixed * equation);
void GLAPIENTRY _mesa_marshal_GetFixedv(GLenum pname, GLfixed * params);
void GLAPIENTRY _mesa_marshal_GetLightxv(GLenum light, GLenum pname, GLfixed * params);
void GLAPIENTRY _mesa_marshal_GetMaterialxv(GLenum face, GLenum pname, GLfixed * params);
void GLAPIENTRY _mesa_marshal_GetTexEnvxv(GLenum target, GLenum pname, GLfixed * params);
void GLAPIENTRY _mesa_marshal_GetTexParameterxv(GLenum target, GLenum pname, GLfixed * params);
struct marshal_cmd_PointParameterx;
uint32_t _mesa_unmarshal_PointParameterx(struct gl_context *ctx, const struct marshal_cmd_PointParameterx *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_PointParameterx(GLenum pname, GLfixed param);
struct marshal_cmd_PointParameterxv;
uint32_t _mesa_unmarshal_PointParameterxv(struct gl_context *ctx, const struct marshal_cmd_PointParameterxv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_PointParameterxv(GLenum pname, const GLfixed * params);
struct marshal_cmd_TexParameterxv;
uint32_t _mesa_unmarshal_TexParameterxv(struct gl_context *ctx, const struct marshal_cmd_TexParameterxv *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TexParameterxv(GLenum target, GLenum pname, const GLfixed * params);
void GLAPIENTRY _mesa_marshal_GetTexGenxvOES(GLenum coord, GLenum pname, GLfixed * params);
struct marshal_cmd_TexGenxOES;
uint32_t _mesa_unmarshal_TexGenxOES(struct gl_context *ctx, const struct marshal_cmd_TexGenxOES *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TexGenxOES(GLenum coord, GLenum pname, GLint param);
struct marshal_cmd_TexGenxvOES;
uint32_t _mesa_unmarshal_TexGenxvOES(struct gl_context *ctx, const struct marshal_cmd_TexGenxvOES *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_TexGenxvOES(GLenum coord, GLenum pname, const GLfixed * params);
struct marshal_cmd_ClipPlanef;
uint32_t _mesa_unmarshal_ClipPlanef(struct gl_context *ctx, const struct marshal_cmd_ClipPlanef *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_ClipPlanef(GLenum plane, const GLfloat * equation);
void GLAPIENTRY _mesa_marshal_GetClipPlanef(GLenum plane, GLfloat * equation);
struct marshal_cmd_Frustumf;
uint32_t _mesa_unmarshal_Frustumf(struct gl_context *ctx, const struct marshal_cmd_Frustumf *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Frustumf(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar);
struct marshal_cmd_Orthof;
uint32_t _mesa_unmarshal_Orthof(struct gl_context *ctx, const struct marshal_cmd_Orthof *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_Orthof(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar);
struct marshal_cmd_DrawTexiOES;
uint32_t _mesa_unmarshal_DrawTexiOES(struct gl_context *ctx, const struct marshal_cmd_DrawTexiOES *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_DrawTexiOES(GLint x, GLint y, GLint z, GLint width, GLint height);
struct marshal_cmd_DrawTexivOES;
uint32_t _mesa_unmarshal_DrawTexivOES(struct gl_context *ctx, const struct marshal_cmd_DrawTexivOES *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_DrawTexivOES(const GLint * coords);
struct marshal_cmd_DrawTexfOES;
uint32_t _mesa_unmarshal_DrawTexfOES(struct gl_context *ctx, const struct marshal_cmd_DrawTexfOES *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_DrawTexfOES(GLfloat x, GLfloat y, GLfloat z, GLfloat width, GLfloat height);
struct marshal_cmd_DrawTexfvOES;
uint32_t _mesa_unmarshal_DrawTexfvOES(struct gl_context *ctx, const struct marshal_cmd_DrawTexfvOES *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_DrawTexfvOES(const GLfloat * coords);
struct marshal_cmd_DrawTexsOES;
uint32_t _mesa_unmarshal_DrawTexsOES(struct gl_context *ctx, const struct marshal_cmd_DrawTexsOES *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_DrawTexsOES(GLshort x, GLshort y, GLshort z, GLshort width, GLshort height);
struct marshal_cmd_DrawTexsvOES;
uint32_t _mesa_unmarshal_DrawTexsvOES(struct gl_context *ctx, const struct marshal_cmd_DrawTexsvOES *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_DrawTexsvOES(const GLshort * coords);
struct marshal_cmd_DrawTexxOES;
uint32_t _mesa_unmarshal_DrawTexxOES(struct gl_context *ctx, const struct marshal_cmd_DrawTexxOES *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_DrawTexxOES(GLfixed x, GLfixed y, GLfixed z, GLfixed width, GLfixed height);
struct marshal_cmd_DrawTexxvOES;
uint32_t _mesa_unmarshal_DrawTexxvOES(struct gl_context *ctx, const struct marshal_cmd_DrawTexxvOES *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_DrawTexxvOES(const GLfixed * coords);
struct marshal_cmd_PointSizePointerOES;
uint32_t _mesa_unmarshal_PointSizePointerOES(struct gl_context *ctx, const struct marshal_cmd_PointSizePointerOES *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_PointSizePointerOES(GLenum type, GLsizei stride, const GLvoid * pointer);
GLbitfield GLAPIENTRY _mesa_marshal_QueryMatrixxOES(GLfixed * mantissa, GLint * exponent);
struct marshal_cmd_DiscardFramebufferEXT;
uint32_t _mesa_unmarshal_DiscardFramebufferEXT(struct gl_context *ctx, const struct marshal_cmd_DiscardFramebufferEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_DiscardFramebufferEXT(GLenum target, GLsizei numAttachments, const GLenum * attachments);
struct marshal_cmd_FramebufferTexture2DMultisampleEXT;
uint32_t _mesa_unmarshal_FramebufferTexture2DMultisampleEXT(struct gl_context *ctx, const struct marshal_cmd_FramebufferTexture2DMultisampleEXT *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_FramebufferTexture2DMultisampleEXT(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLsizei samples);
struct marshal_cmd_DepthRangeArrayfvOES;
uint32_t _mesa_unmarshal_DepthRangeArrayfvOES(struct gl_context *ctx, const struct marshal_cmd_DepthRangeArrayfvOES *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_DepthRangeArrayfvOES(GLuint first, GLsizei count, const GLfloat * v);
struct marshal_cmd_DepthRangeIndexedfOES;
uint32_t _mesa_unmarshal_DepthRangeIndexedfOES(struct gl_context *ctx, const struct marshal_cmd_DepthRangeIndexedfOES *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_DepthRangeIndexedfOES(GLuint index, GLfloat n, GLfloat f);
struct marshal_cmd_FramebufferParameteriMESA;
uint32_t _mesa_unmarshal_FramebufferParameteriMESA(struct gl_context *ctx, const struct marshal_cmd_FramebufferParameteriMESA *cmd, const uint64_t *last);
void GLAPIENTRY _mesa_marshal_FramebufferParameteriMESA(GLenum target, GLenum pname, GLint param);
void GLAPIENTRY _mesa_marshal_GetFramebufferParameterivMESA(GLenum target, GLenum pname, GLint * params);

#endif /* MARSHAL_GENERATED_H */

