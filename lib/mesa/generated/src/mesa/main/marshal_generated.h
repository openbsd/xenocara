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
uint32_t _mesa_unmarshal_NewList(struct gl_context *ctx, const struct marshal_cmd_NewList *cmd);
struct marshal_cmd_EndList;
uint32_t _mesa_unmarshal_EndList(struct gl_context *ctx, const struct marshal_cmd_EndList *cmd);
struct marshal_cmd_CallList;
uint32_t _mesa_unmarshal_CallList(struct gl_context *ctx, const struct marshal_cmd_CallList *cmd);
void GLAPIENTRY _mesa_marshal_CallList(GLuint list);
struct marshal_cmd_CallLists;
uint32_t _mesa_unmarshal_CallLists(struct gl_context *ctx, const struct marshal_cmd_CallLists *cmd);
struct marshal_cmd_DeleteLists;
uint32_t _mesa_unmarshal_DeleteLists(struct gl_context *ctx, const struct marshal_cmd_DeleteLists *cmd);
struct marshal_cmd_ListBase;
uint32_t _mesa_unmarshal_ListBase(struct gl_context *ctx, const struct marshal_cmd_ListBase *cmd);
struct marshal_cmd_Begin;
uint32_t _mesa_unmarshal_Begin(struct gl_context *ctx, const struct marshal_cmd_Begin *cmd);
struct marshal_cmd_Bitmap;
uint32_t _mesa_unmarshal_Bitmap(struct gl_context *ctx, const struct marshal_cmd_Bitmap *cmd);
struct marshal_cmd_Color3b;
uint32_t _mesa_unmarshal_Color3b(struct gl_context *ctx, const struct marshal_cmd_Color3b *cmd);
struct marshal_cmd_Color3bv;
uint32_t _mesa_unmarshal_Color3bv(struct gl_context *ctx, const struct marshal_cmd_Color3bv *cmd);
struct marshal_cmd_Color3d;
uint32_t _mesa_unmarshal_Color3d(struct gl_context *ctx, const struct marshal_cmd_Color3d *cmd);
struct marshal_cmd_Color3dv;
uint32_t _mesa_unmarshal_Color3dv(struct gl_context *ctx, const struct marshal_cmd_Color3dv *cmd);
struct marshal_cmd_Color3f;
uint32_t _mesa_unmarshal_Color3f(struct gl_context *ctx, const struct marshal_cmd_Color3f *cmd);
struct marshal_cmd_Color3fv;
uint32_t _mesa_unmarshal_Color3fv(struct gl_context *ctx, const struct marshal_cmd_Color3fv *cmd);
struct marshal_cmd_Color3i;
uint32_t _mesa_unmarshal_Color3i(struct gl_context *ctx, const struct marshal_cmd_Color3i *cmd);
struct marshal_cmd_Color3iv;
uint32_t _mesa_unmarshal_Color3iv(struct gl_context *ctx, const struct marshal_cmd_Color3iv *cmd);
struct marshal_cmd_Color3s;
uint32_t _mesa_unmarshal_Color3s(struct gl_context *ctx, const struct marshal_cmd_Color3s *cmd);
struct marshal_cmd_Color3sv;
uint32_t _mesa_unmarshal_Color3sv(struct gl_context *ctx, const struct marshal_cmd_Color3sv *cmd);
struct marshal_cmd_Color3ub;
uint32_t _mesa_unmarshal_Color3ub(struct gl_context *ctx, const struct marshal_cmd_Color3ub *cmd);
struct marshal_cmd_Color3ubv;
uint32_t _mesa_unmarshal_Color3ubv(struct gl_context *ctx, const struct marshal_cmd_Color3ubv *cmd);
struct marshal_cmd_Color3ui;
uint32_t _mesa_unmarshal_Color3ui(struct gl_context *ctx, const struct marshal_cmd_Color3ui *cmd);
struct marshal_cmd_Color3uiv;
uint32_t _mesa_unmarshal_Color3uiv(struct gl_context *ctx, const struct marshal_cmd_Color3uiv *cmd);
struct marshal_cmd_Color3us;
uint32_t _mesa_unmarshal_Color3us(struct gl_context *ctx, const struct marshal_cmd_Color3us *cmd);
struct marshal_cmd_Color3usv;
uint32_t _mesa_unmarshal_Color3usv(struct gl_context *ctx, const struct marshal_cmd_Color3usv *cmd);
struct marshal_cmd_Color4b;
uint32_t _mesa_unmarshal_Color4b(struct gl_context *ctx, const struct marshal_cmd_Color4b *cmd);
struct marshal_cmd_Color4bv;
uint32_t _mesa_unmarshal_Color4bv(struct gl_context *ctx, const struct marshal_cmd_Color4bv *cmd);
struct marshal_cmd_Color4d;
uint32_t _mesa_unmarshal_Color4d(struct gl_context *ctx, const struct marshal_cmd_Color4d *cmd);
struct marshal_cmd_Color4dv;
uint32_t _mesa_unmarshal_Color4dv(struct gl_context *ctx, const struct marshal_cmd_Color4dv *cmd);
struct marshal_cmd_Color4f;
uint32_t _mesa_unmarshal_Color4f(struct gl_context *ctx, const struct marshal_cmd_Color4f *cmd);
struct marshal_cmd_Color4fv;
uint32_t _mesa_unmarshal_Color4fv(struct gl_context *ctx, const struct marshal_cmd_Color4fv *cmd);
struct marshal_cmd_Color4i;
uint32_t _mesa_unmarshal_Color4i(struct gl_context *ctx, const struct marshal_cmd_Color4i *cmd);
struct marshal_cmd_Color4iv;
uint32_t _mesa_unmarshal_Color4iv(struct gl_context *ctx, const struct marshal_cmd_Color4iv *cmd);
struct marshal_cmd_Color4s;
uint32_t _mesa_unmarshal_Color4s(struct gl_context *ctx, const struct marshal_cmd_Color4s *cmd);
struct marshal_cmd_Color4sv;
uint32_t _mesa_unmarshal_Color4sv(struct gl_context *ctx, const struct marshal_cmd_Color4sv *cmd);
struct marshal_cmd_Color4ub;
uint32_t _mesa_unmarshal_Color4ub(struct gl_context *ctx, const struct marshal_cmd_Color4ub *cmd);
struct marshal_cmd_Color4ubv;
uint32_t _mesa_unmarshal_Color4ubv(struct gl_context *ctx, const struct marshal_cmd_Color4ubv *cmd);
struct marshal_cmd_Color4ui;
uint32_t _mesa_unmarshal_Color4ui(struct gl_context *ctx, const struct marshal_cmd_Color4ui *cmd);
struct marshal_cmd_Color4uiv;
uint32_t _mesa_unmarshal_Color4uiv(struct gl_context *ctx, const struct marshal_cmd_Color4uiv *cmd);
struct marshal_cmd_Color4us;
uint32_t _mesa_unmarshal_Color4us(struct gl_context *ctx, const struct marshal_cmd_Color4us *cmd);
struct marshal_cmd_Color4usv;
uint32_t _mesa_unmarshal_Color4usv(struct gl_context *ctx, const struct marshal_cmd_Color4usv *cmd);
struct marshal_cmd_EdgeFlag;
uint32_t _mesa_unmarshal_EdgeFlag(struct gl_context *ctx, const struct marshal_cmd_EdgeFlag *cmd);
struct marshal_cmd_EdgeFlagv;
uint32_t _mesa_unmarshal_EdgeFlagv(struct gl_context *ctx, const struct marshal_cmd_EdgeFlagv *cmd);
struct marshal_cmd_End;
uint32_t _mesa_unmarshal_End(struct gl_context *ctx, const struct marshal_cmd_End *cmd);
struct marshal_cmd_Indexd;
uint32_t _mesa_unmarshal_Indexd(struct gl_context *ctx, const struct marshal_cmd_Indexd *cmd);
struct marshal_cmd_Indexdv;
uint32_t _mesa_unmarshal_Indexdv(struct gl_context *ctx, const struct marshal_cmd_Indexdv *cmd);
struct marshal_cmd_Indexf;
uint32_t _mesa_unmarshal_Indexf(struct gl_context *ctx, const struct marshal_cmd_Indexf *cmd);
struct marshal_cmd_Indexfv;
uint32_t _mesa_unmarshal_Indexfv(struct gl_context *ctx, const struct marshal_cmd_Indexfv *cmd);
struct marshal_cmd_Indexi;
uint32_t _mesa_unmarshal_Indexi(struct gl_context *ctx, const struct marshal_cmd_Indexi *cmd);
struct marshal_cmd_Indexiv;
uint32_t _mesa_unmarshal_Indexiv(struct gl_context *ctx, const struct marshal_cmd_Indexiv *cmd);
struct marshal_cmd_Indexs;
uint32_t _mesa_unmarshal_Indexs(struct gl_context *ctx, const struct marshal_cmd_Indexs *cmd);
struct marshal_cmd_Indexsv;
uint32_t _mesa_unmarshal_Indexsv(struct gl_context *ctx, const struct marshal_cmd_Indexsv *cmd);
struct marshal_cmd_Normal3b;
uint32_t _mesa_unmarshal_Normal3b(struct gl_context *ctx, const struct marshal_cmd_Normal3b *cmd);
struct marshal_cmd_Normal3bv;
uint32_t _mesa_unmarshal_Normal3bv(struct gl_context *ctx, const struct marshal_cmd_Normal3bv *cmd);
struct marshal_cmd_Normal3d;
uint32_t _mesa_unmarshal_Normal3d(struct gl_context *ctx, const struct marshal_cmd_Normal3d *cmd);
struct marshal_cmd_Normal3dv;
uint32_t _mesa_unmarshal_Normal3dv(struct gl_context *ctx, const struct marshal_cmd_Normal3dv *cmd);
struct marshal_cmd_Normal3f;
uint32_t _mesa_unmarshal_Normal3f(struct gl_context *ctx, const struct marshal_cmd_Normal3f *cmd);
struct marshal_cmd_Normal3fv;
uint32_t _mesa_unmarshal_Normal3fv(struct gl_context *ctx, const struct marshal_cmd_Normal3fv *cmd);
struct marshal_cmd_Normal3i;
uint32_t _mesa_unmarshal_Normal3i(struct gl_context *ctx, const struct marshal_cmd_Normal3i *cmd);
struct marshal_cmd_Normal3iv;
uint32_t _mesa_unmarshal_Normal3iv(struct gl_context *ctx, const struct marshal_cmd_Normal3iv *cmd);
struct marshal_cmd_Normal3s;
uint32_t _mesa_unmarshal_Normal3s(struct gl_context *ctx, const struct marshal_cmd_Normal3s *cmd);
struct marshal_cmd_Normal3sv;
uint32_t _mesa_unmarshal_Normal3sv(struct gl_context *ctx, const struct marshal_cmd_Normal3sv *cmd);
struct marshal_cmd_RasterPos2d;
uint32_t _mesa_unmarshal_RasterPos2d(struct gl_context *ctx, const struct marshal_cmd_RasterPos2d *cmd);
struct marshal_cmd_RasterPos2dv;
uint32_t _mesa_unmarshal_RasterPos2dv(struct gl_context *ctx, const struct marshal_cmd_RasterPos2dv *cmd);
struct marshal_cmd_RasterPos2f;
uint32_t _mesa_unmarshal_RasterPos2f(struct gl_context *ctx, const struct marshal_cmd_RasterPos2f *cmd);
struct marshal_cmd_RasterPos2fv;
uint32_t _mesa_unmarshal_RasterPos2fv(struct gl_context *ctx, const struct marshal_cmd_RasterPos2fv *cmd);
struct marshal_cmd_RasterPos2i;
uint32_t _mesa_unmarshal_RasterPos2i(struct gl_context *ctx, const struct marshal_cmd_RasterPos2i *cmd);
struct marshal_cmd_RasterPos2iv;
uint32_t _mesa_unmarshal_RasterPos2iv(struct gl_context *ctx, const struct marshal_cmd_RasterPos2iv *cmd);
struct marshal_cmd_RasterPos2s;
uint32_t _mesa_unmarshal_RasterPos2s(struct gl_context *ctx, const struct marshal_cmd_RasterPos2s *cmd);
struct marshal_cmd_RasterPos2sv;
uint32_t _mesa_unmarshal_RasterPos2sv(struct gl_context *ctx, const struct marshal_cmd_RasterPos2sv *cmd);
struct marshal_cmd_RasterPos3d;
uint32_t _mesa_unmarshal_RasterPos3d(struct gl_context *ctx, const struct marshal_cmd_RasterPos3d *cmd);
struct marshal_cmd_RasterPos3dv;
uint32_t _mesa_unmarshal_RasterPos3dv(struct gl_context *ctx, const struct marshal_cmd_RasterPos3dv *cmd);
struct marshal_cmd_RasterPos3f;
uint32_t _mesa_unmarshal_RasterPos3f(struct gl_context *ctx, const struct marshal_cmd_RasterPos3f *cmd);
struct marshal_cmd_RasterPos3fv;
uint32_t _mesa_unmarshal_RasterPos3fv(struct gl_context *ctx, const struct marshal_cmd_RasterPos3fv *cmd);
struct marshal_cmd_RasterPos3i;
uint32_t _mesa_unmarshal_RasterPos3i(struct gl_context *ctx, const struct marshal_cmd_RasterPos3i *cmd);
struct marshal_cmd_RasterPos3iv;
uint32_t _mesa_unmarshal_RasterPos3iv(struct gl_context *ctx, const struct marshal_cmd_RasterPos3iv *cmd);
struct marshal_cmd_RasterPos3s;
uint32_t _mesa_unmarshal_RasterPos3s(struct gl_context *ctx, const struct marshal_cmd_RasterPos3s *cmd);
struct marshal_cmd_RasterPos3sv;
uint32_t _mesa_unmarshal_RasterPos3sv(struct gl_context *ctx, const struct marshal_cmd_RasterPos3sv *cmd);
struct marshal_cmd_RasterPos4d;
uint32_t _mesa_unmarshal_RasterPos4d(struct gl_context *ctx, const struct marshal_cmd_RasterPos4d *cmd);
struct marshal_cmd_RasterPos4dv;
uint32_t _mesa_unmarshal_RasterPos4dv(struct gl_context *ctx, const struct marshal_cmd_RasterPos4dv *cmd);
struct marshal_cmd_RasterPos4f;
uint32_t _mesa_unmarshal_RasterPos4f(struct gl_context *ctx, const struct marshal_cmd_RasterPos4f *cmd);
struct marshal_cmd_RasterPos4fv;
uint32_t _mesa_unmarshal_RasterPos4fv(struct gl_context *ctx, const struct marshal_cmd_RasterPos4fv *cmd);
struct marshal_cmd_RasterPos4i;
uint32_t _mesa_unmarshal_RasterPos4i(struct gl_context *ctx, const struct marshal_cmd_RasterPos4i *cmd);
struct marshal_cmd_RasterPos4iv;
uint32_t _mesa_unmarshal_RasterPos4iv(struct gl_context *ctx, const struct marshal_cmd_RasterPos4iv *cmd);
struct marshal_cmd_RasterPos4s;
uint32_t _mesa_unmarshal_RasterPos4s(struct gl_context *ctx, const struct marshal_cmd_RasterPos4s *cmd);
struct marshal_cmd_RasterPos4sv;
uint32_t _mesa_unmarshal_RasterPos4sv(struct gl_context *ctx, const struct marshal_cmd_RasterPos4sv *cmd);
struct marshal_cmd_Rectd;
uint32_t _mesa_unmarshal_Rectd(struct gl_context *ctx, const struct marshal_cmd_Rectd *cmd);
struct marshal_cmd_Rectdv;
uint32_t _mesa_unmarshal_Rectdv(struct gl_context *ctx, const struct marshal_cmd_Rectdv *cmd);
struct marshal_cmd_Rectf;
uint32_t _mesa_unmarshal_Rectf(struct gl_context *ctx, const struct marshal_cmd_Rectf *cmd);
struct marshal_cmd_Rectfv;
uint32_t _mesa_unmarshal_Rectfv(struct gl_context *ctx, const struct marshal_cmd_Rectfv *cmd);
struct marshal_cmd_Recti;
uint32_t _mesa_unmarshal_Recti(struct gl_context *ctx, const struct marshal_cmd_Recti *cmd);
struct marshal_cmd_Rectiv;
uint32_t _mesa_unmarshal_Rectiv(struct gl_context *ctx, const struct marshal_cmd_Rectiv *cmd);
struct marshal_cmd_Rects;
uint32_t _mesa_unmarshal_Rects(struct gl_context *ctx, const struct marshal_cmd_Rects *cmd);
struct marshal_cmd_Rectsv;
uint32_t _mesa_unmarshal_Rectsv(struct gl_context *ctx, const struct marshal_cmd_Rectsv *cmd);
struct marshal_cmd_TexCoord1d;
uint32_t _mesa_unmarshal_TexCoord1d(struct gl_context *ctx, const struct marshal_cmd_TexCoord1d *cmd);
struct marshal_cmd_TexCoord1dv;
uint32_t _mesa_unmarshal_TexCoord1dv(struct gl_context *ctx, const struct marshal_cmd_TexCoord1dv *cmd);
struct marshal_cmd_TexCoord1f;
uint32_t _mesa_unmarshal_TexCoord1f(struct gl_context *ctx, const struct marshal_cmd_TexCoord1f *cmd);
struct marshal_cmd_TexCoord1fv;
uint32_t _mesa_unmarshal_TexCoord1fv(struct gl_context *ctx, const struct marshal_cmd_TexCoord1fv *cmd);
struct marshal_cmd_TexCoord1i;
uint32_t _mesa_unmarshal_TexCoord1i(struct gl_context *ctx, const struct marshal_cmd_TexCoord1i *cmd);
struct marshal_cmd_TexCoord1iv;
uint32_t _mesa_unmarshal_TexCoord1iv(struct gl_context *ctx, const struct marshal_cmd_TexCoord1iv *cmd);
struct marshal_cmd_TexCoord1s;
uint32_t _mesa_unmarshal_TexCoord1s(struct gl_context *ctx, const struct marshal_cmd_TexCoord1s *cmd);
struct marshal_cmd_TexCoord1sv;
uint32_t _mesa_unmarshal_TexCoord1sv(struct gl_context *ctx, const struct marshal_cmd_TexCoord1sv *cmd);
struct marshal_cmd_TexCoord2d;
uint32_t _mesa_unmarshal_TexCoord2d(struct gl_context *ctx, const struct marshal_cmd_TexCoord2d *cmd);
struct marshal_cmd_TexCoord2dv;
uint32_t _mesa_unmarshal_TexCoord2dv(struct gl_context *ctx, const struct marshal_cmd_TexCoord2dv *cmd);
struct marshal_cmd_TexCoord2f;
uint32_t _mesa_unmarshal_TexCoord2f(struct gl_context *ctx, const struct marshal_cmd_TexCoord2f *cmd);
struct marshal_cmd_TexCoord2fv;
uint32_t _mesa_unmarshal_TexCoord2fv(struct gl_context *ctx, const struct marshal_cmd_TexCoord2fv *cmd);
struct marshal_cmd_TexCoord2i;
uint32_t _mesa_unmarshal_TexCoord2i(struct gl_context *ctx, const struct marshal_cmd_TexCoord2i *cmd);
struct marshal_cmd_TexCoord2iv;
uint32_t _mesa_unmarshal_TexCoord2iv(struct gl_context *ctx, const struct marshal_cmd_TexCoord2iv *cmd);
struct marshal_cmd_TexCoord2s;
uint32_t _mesa_unmarshal_TexCoord2s(struct gl_context *ctx, const struct marshal_cmd_TexCoord2s *cmd);
struct marshal_cmd_TexCoord2sv;
uint32_t _mesa_unmarshal_TexCoord2sv(struct gl_context *ctx, const struct marshal_cmd_TexCoord2sv *cmd);
struct marshal_cmd_TexCoord3d;
uint32_t _mesa_unmarshal_TexCoord3d(struct gl_context *ctx, const struct marshal_cmd_TexCoord3d *cmd);
struct marshal_cmd_TexCoord3dv;
uint32_t _mesa_unmarshal_TexCoord3dv(struct gl_context *ctx, const struct marshal_cmd_TexCoord3dv *cmd);
struct marshal_cmd_TexCoord3f;
uint32_t _mesa_unmarshal_TexCoord3f(struct gl_context *ctx, const struct marshal_cmd_TexCoord3f *cmd);
struct marshal_cmd_TexCoord3fv;
uint32_t _mesa_unmarshal_TexCoord3fv(struct gl_context *ctx, const struct marshal_cmd_TexCoord3fv *cmd);
struct marshal_cmd_TexCoord3i;
uint32_t _mesa_unmarshal_TexCoord3i(struct gl_context *ctx, const struct marshal_cmd_TexCoord3i *cmd);
struct marshal_cmd_TexCoord3iv;
uint32_t _mesa_unmarshal_TexCoord3iv(struct gl_context *ctx, const struct marshal_cmd_TexCoord3iv *cmd);
struct marshal_cmd_TexCoord3s;
uint32_t _mesa_unmarshal_TexCoord3s(struct gl_context *ctx, const struct marshal_cmd_TexCoord3s *cmd);
struct marshal_cmd_TexCoord3sv;
uint32_t _mesa_unmarshal_TexCoord3sv(struct gl_context *ctx, const struct marshal_cmd_TexCoord3sv *cmd);
struct marshal_cmd_TexCoord4d;
uint32_t _mesa_unmarshal_TexCoord4d(struct gl_context *ctx, const struct marshal_cmd_TexCoord4d *cmd);
struct marshal_cmd_TexCoord4dv;
uint32_t _mesa_unmarshal_TexCoord4dv(struct gl_context *ctx, const struct marshal_cmd_TexCoord4dv *cmd);
struct marshal_cmd_TexCoord4f;
uint32_t _mesa_unmarshal_TexCoord4f(struct gl_context *ctx, const struct marshal_cmd_TexCoord4f *cmd);
struct marshal_cmd_TexCoord4fv;
uint32_t _mesa_unmarshal_TexCoord4fv(struct gl_context *ctx, const struct marshal_cmd_TexCoord4fv *cmd);
struct marshal_cmd_TexCoord4i;
uint32_t _mesa_unmarshal_TexCoord4i(struct gl_context *ctx, const struct marshal_cmd_TexCoord4i *cmd);
struct marshal_cmd_TexCoord4iv;
uint32_t _mesa_unmarshal_TexCoord4iv(struct gl_context *ctx, const struct marshal_cmd_TexCoord4iv *cmd);
struct marshal_cmd_TexCoord4s;
uint32_t _mesa_unmarshal_TexCoord4s(struct gl_context *ctx, const struct marshal_cmd_TexCoord4s *cmd);
struct marshal_cmd_TexCoord4sv;
uint32_t _mesa_unmarshal_TexCoord4sv(struct gl_context *ctx, const struct marshal_cmd_TexCoord4sv *cmd);
struct marshal_cmd_Vertex2d;
uint32_t _mesa_unmarshal_Vertex2d(struct gl_context *ctx, const struct marshal_cmd_Vertex2d *cmd);
struct marshal_cmd_Vertex2dv;
uint32_t _mesa_unmarshal_Vertex2dv(struct gl_context *ctx, const struct marshal_cmd_Vertex2dv *cmd);
struct marshal_cmd_Vertex2f;
uint32_t _mesa_unmarshal_Vertex2f(struct gl_context *ctx, const struct marshal_cmd_Vertex2f *cmd);
struct marshal_cmd_Vertex2fv;
uint32_t _mesa_unmarshal_Vertex2fv(struct gl_context *ctx, const struct marshal_cmd_Vertex2fv *cmd);
struct marshal_cmd_Vertex2i;
uint32_t _mesa_unmarshal_Vertex2i(struct gl_context *ctx, const struct marshal_cmd_Vertex2i *cmd);
struct marshal_cmd_Vertex2iv;
uint32_t _mesa_unmarshal_Vertex2iv(struct gl_context *ctx, const struct marshal_cmd_Vertex2iv *cmd);
struct marshal_cmd_Vertex2s;
uint32_t _mesa_unmarshal_Vertex2s(struct gl_context *ctx, const struct marshal_cmd_Vertex2s *cmd);
struct marshal_cmd_Vertex2sv;
uint32_t _mesa_unmarshal_Vertex2sv(struct gl_context *ctx, const struct marshal_cmd_Vertex2sv *cmd);
struct marshal_cmd_Vertex3d;
uint32_t _mesa_unmarshal_Vertex3d(struct gl_context *ctx, const struct marshal_cmd_Vertex3d *cmd);
struct marshal_cmd_Vertex3dv;
uint32_t _mesa_unmarshal_Vertex3dv(struct gl_context *ctx, const struct marshal_cmd_Vertex3dv *cmd);
struct marshal_cmd_Vertex3f;
uint32_t _mesa_unmarshal_Vertex3f(struct gl_context *ctx, const struct marshal_cmd_Vertex3f *cmd);
struct marshal_cmd_Vertex3fv;
uint32_t _mesa_unmarshal_Vertex3fv(struct gl_context *ctx, const struct marshal_cmd_Vertex3fv *cmd);
struct marshal_cmd_Vertex3i;
uint32_t _mesa_unmarshal_Vertex3i(struct gl_context *ctx, const struct marshal_cmd_Vertex3i *cmd);
struct marshal_cmd_Vertex3iv;
uint32_t _mesa_unmarshal_Vertex3iv(struct gl_context *ctx, const struct marshal_cmd_Vertex3iv *cmd);
struct marshal_cmd_Vertex3s;
uint32_t _mesa_unmarshal_Vertex3s(struct gl_context *ctx, const struct marshal_cmd_Vertex3s *cmd);
struct marshal_cmd_Vertex3sv;
uint32_t _mesa_unmarshal_Vertex3sv(struct gl_context *ctx, const struct marshal_cmd_Vertex3sv *cmd);
struct marshal_cmd_Vertex4d;
uint32_t _mesa_unmarshal_Vertex4d(struct gl_context *ctx, const struct marshal_cmd_Vertex4d *cmd);
struct marshal_cmd_Vertex4dv;
uint32_t _mesa_unmarshal_Vertex4dv(struct gl_context *ctx, const struct marshal_cmd_Vertex4dv *cmd);
struct marshal_cmd_Vertex4f;
uint32_t _mesa_unmarshal_Vertex4f(struct gl_context *ctx, const struct marshal_cmd_Vertex4f *cmd);
struct marshal_cmd_Vertex4fv;
uint32_t _mesa_unmarshal_Vertex4fv(struct gl_context *ctx, const struct marshal_cmd_Vertex4fv *cmd);
struct marshal_cmd_Vertex4i;
uint32_t _mesa_unmarshal_Vertex4i(struct gl_context *ctx, const struct marshal_cmd_Vertex4i *cmd);
struct marshal_cmd_Vertex4iv;
uint32_t _mesa_unmarshal_Vertex4iv(struct gl_context *ctx, const struct marshal_cmd_Vertex4iv *cmd);
struct marshal_cmd_Vertex4s;
uint32_t _mesa_unmarshal_Vertex4s(struct gl_context *ctx, const struct marshal_cmd_Vertex4s *cmd);
struct marshal_cmd_Vertex4sv;
uint32_t _mesa_unmarshal_Vertex4sv(struct gl_context *ctx, const struct marshal_cmd_Vertex4sv *cmd);
struct marshal_cmd_ClipPlane;
uint32_t _mesa_unmarshal_ClipPlane(struct gl_context *ctx, const struct marshal_cmd_ClipPlane *cmd);
struct marshal_cmd_ColorMaterial;
uint32_t _mesa_unmarshal_ColorMaterial(struct gl_context *ctx, const struct marshal_cmd_ColorMaterial *cmd);
struct marshal_cmd_CullFace;
uint32_t _mesa_unmarshal_CullFace(struct gl_context *ctx, const struct marshal_cmd_CullFace *cmd);
struct marshal_cmd_Fogf;
uint32_t _mesa_unmarshal_Fogf(struct gl_context *ctx, const struct marshal_cmd_Fogf *cmd);
struct marshal_cmd_Fogfv;
uint32_t _mesa_unmarshal_Fogfv(struct gl_context *ctx, const struct marshal_cmd_Fogfv *cmd);
struct marshal_cmd_Fogi;
uint32_t _mesa_unmarshal_Fogi(struct gl_context *ctx, const struct marshal_cmd_Fogi *cmd);
struct marshal_cmd_Fogiv;
uint32_t _mesa_unmarshal_Fogiv(struct gl_context *ctx, const struct marshal_cmd_Fogiv *cmd);
struct marshal_cmd_FrontFace;
uint32_t _mesa_unmarshal_FrontFace(struct gl_context *ctx, const struct marshal_cmd_FrontFace *cmd);
struct marshal_cmd_Hint;
uint32_t _mesa_unmarshal_Hint(struct gl_context *ctx, const struct marshal_cmd_Hint *cmd);
struct marshal_cmd_Lightf;
uint32_t _mesa_unmarshal_Lightf(struct gl_context *ctx, const struct marshal_cmd_Lightf *cmd);
struct marshal_cmd_Lightfv;
uint32_t _mesa_unmarshal_Lightfv(struct gl_context *ctx, const struct marshal_cmd_Lightfv *cmd);
struct marshal_cmd_Lighti;
uint32_t _mesa_unmarshal_Lighti(struct gl_context *ctx, const struct marshal_cmd_Lighti *cmd);
struct marshal_cmd_Lightiv;
uint32_t _mesa_unmarshal_Lightiv(struct gl_context *ctx, const struct marshal_cmd_Lightiv *cmd);
struct marshal_cmd_LightModelf;
uint32_t _mesa_unmarshal_LightModelf(struct gl_context *ctx, const struct marshal_cmd_LightModelf *cmd);
struct marshal_cmd_LightModelfv;
uint32_t _mesa_unmarshal_LightModelfv(struct gl_context *ctx, const struct marshal_cmd_LightModelfv *cmd);
struct marshal_cmd_LightModeli;
uint32_t _mesa_unmarshal_LightModeli(struct gl_context *ctx, const struct marshal_cmd_LightModeli *cmd);
struct marshal_cmd_LightModeliv;
uint32_t _mesa_unmarshal_LightModeliv(struct gl_context *ctx, const struct marshal_cmd_LightModeliv *cmd);
struct marshal_cmd_LineStipple;
uint32_t _mesa_unmarshal_LineStipple(struct gl_context *ctx, const struct marshal_cmd_LineStipple *cmd);
struct marshal_cmd_LineWidth;
uint32_t _mesa_unmarshal_LineWidth(struct gl_context *ctx, const struct marshal_cmd_LineWidth *cmd);
struct marshal_cmd_Materialf;
uint32_t _mesa_unmarshal_Materialf(struct gl_context *ctx, const struct marshal_cmd_Materialf *cmd);
struct marshal_cmd_Materialfv;
uint32_t _mesa_unmarshal_Materialfv(struct gl_context *ctx, const struct marshal_cmd_Materialfv *cmd);
struct marshal_cmd_Materiali;
uint32_t _mesa_unmarshal_Materiali(struct gl_context *ctx, const struct marshal_cmd_Materiali *cmd);
struct marshal_cmd_Materialiv;
uint32_t _mesa_unmarshal_Materialiv(struct gl_context *ctx, const struct marshal_cmd_Materialiv *cmd);
struct marshal_cmd_PointSize;
uint32_t _mesa_unmarshal_PointSize(struct gl_context *ctx, const struct marshal_cmd_PointSize *cmd);
struct marshal_cmd_PolygonMode;
uint32_t _mesa_unmarshal_PolygonMode(struct gl_context *ctx, const struct marshal_cmd_PolygonMode *cmd);
struct marshal_cmd_PolygonStipple;
uint32_t _mesa_unmarshal_PolygonStipple(struct gl_context *ctx, const struct marshal_cmd_PolygonStipple *cmd);
struct marshal_cmd_Scissor;
uint32_t _mesa_unmarshal_Scissor(struct gl_context *ctx, const struct marshal_cmd_Scissor *cmd);
struct marshal_cmd_ShadeModel;
uint32_t _mesa_unmarshal_ShadeModel(struct gl_context *ctx, const struct marshal_cmd_ShadeModel *cmd);
struct marshal_cmd_TexParameterf;
uint32_t _mesa_unmarshal_TexParameterf(struct gl_context *ctx, const struct marshal_cmd_TexParameterf *cmd);
struct marshal_cmd_TexParameterfv;
uint32_t _mesa_unmarshal_TexParameterfv(struct gl_context *ctx, const struct marshal_cmd_TexParameterfv *cmd);
struct marshal_cmd_TexParameteri;
uint32_t _mesa_unmarshal_TexParameteri(struct gl_context *ctx, const struct marshal_cmd_TexParameteri *cmd);
struct marshal_cmd_TexParameteriv;
uint32_t _mesa_unmarshal_TexParameteriv(struct gl_context *ctx, const struct marshal_cmd_TexParameteriv *cmd);
struct marshal_cmd_TexImage1D;
uint32_t _mesa_unmarshal_TexImage1D(struct gl_context *ctx, const struct marshal_cmd_TexImage1D *cmd);
struct marshal_cmd_TexImage2D;
uint32_t _mesa_unmarshal_TexImage2D(struct gl_context *ctx, const struct marshal_cmd_TexImage2D *cmd);
struct marshal_cmd_TexEnvf;
uint32_t _mesa_unmarshal_TexEnvf(struct gl_context *ctx, const struct marshal_cmd_TexEnvf *cmd);
struct marshal_cmd_TexEnvfv;
uint32_t _mesa_unmarshal_TexEnvfv(struct gl_context *ctx, const struct marshal_cmd_TexEnvfv *cmd);
struct marshal_cmd_TexEnvi;
uint32_t _mesa_unmarshal_TexEnvi(struct gl_context *ctx, const struct marshal_cmd_TexEnvi *cmd);
struct marshal_cmd_TexEnviv;
uint32_t _mesa_unmarshal_TexEnviv(struct gl_context *ctx, const struct marshal_cmd_TexEnviv *cmd);
struct marshal_cmd_TexGend;
uint32_t _mesa_unmarshal_TexGend(struct gl_context *ctx, const struct marshal_cmd_TexGend *cmd);
struct marshal_cmd_TexGendv;
uint32_t _mesa_unmarshal_TexGendv(struct gl_context *ctx, const struct marshal_cmd_TexGendv *cmd);
struct marshal_cmd_TexGenf;
uint32_t _mesa_unmarshal_TexGenf(struct gl_context *ctx, const struct marshal_cmd_TexGenf *cmd);
struct marshal_cmd_TexGenfv;
uint32_t _mesa_unmarshal_TexGenfv(struct gl_context *ctx, const struct marshal_cmd_TexGenfv *cmd);
struct marshal_cmd_TexGeni;
uint32_t _mesa_unmarshal_TexGeni(struct gl_context *ctx, const struct marshal_cmd_TexGeni *cmd);
struct marshal_cmd_TexGeniv;
uint32_t _mesa_unmarshal_TexGeniv(struct gl_context *ctx, const struct marshal_cmd_TexGeniv *cmd);
struct marshal_cmd_InitNames;
uint32_t _mesa_unmarshal_InitNames(struct gl_context *ctx, const struct marshal_cmd_InitNames *cmd);
struct marshal_cmd_LoadName;
uint32_t _mesa_unmarshal_LoadName(struct gl_context *ctx, const struct marshal_cmd_LoadName *cmd);
struct marshal_cmd_PassThrough;
uint32_t _mesa_unmarshal_PassThrough(struct gl_context *ctx, const struct marshal_cmd_PassThrough *cmd);
struct marshal_cmd_PopName;
uint32_t _mesa_unmarshal_PopName(struct gl_context *ctx, const struct marshal_cmd_PopName *cmd);
struct marshal_cmd_PushName;
uint32_t _mesa_unmarshal_PushName(struct gl_context *ctx, const struct marshal_cmd_PushName *cmd);
struct marshal_cmd_DrawBuffer;
uint32_t _mesa_unmarshal_DrawBuffer(struct gl_context *ctx, const struct marshal_cmd_DrawBuffer *cmd);
struct marshal_cmd_Clear;
uint32_t _mesa_unmarshal_Clear(struct gl_context *ctx, const struct marshal_cmd_Clear *cmd);
struct marshal_cmd_ClearAccum;
uint32_t _mesa_unmarshal_ClearAccum(struct gl_context *ctx, const struct marshal_cmd_ClearAccum *cmd);
struct marshal_cmd_ClearIndex;
uint32_t _mesa_unmarshal_ClearIndex(struct gl_context *ctx, const struct marshal_cmd_ClearIndex *cmd);
struct marshal_cmd_ClearColor;
uint32_t _mesa_unmarshal_ClearColor(struct gl_context *ctx, const struct marshal_cmd_ClearColor *cmd);
struct marshal_cmd_ClearStencil;
uint32_t _mesa_unmarshal_ClearStencil(struct gl_context *ctx, const struct marshal_cmd_ClearStencil *cmd);
struct marshal_cmd_ClearDepth;
uint32_t _mesa_unmarshal_ClearDepth(struct gl_context *ctx, const struct marshal_cmd_ClearDepth *cmd);
struct marshal_cmd_StencilMask;
uint32_t _mesa_unmarshal_StencilMask(struct gl_context *ctx, const struct marshal_cmd_StencilMask *cmd);
struct marshal_cmd_ColorMask;
uint32_t _mesa_unmarshal_ColorMask(struct gl_context *ctx, const struct marshal_cmd_ColorMask *cmd);
struct marshal_cmd_DepthMask;
uint32_t _mesa_unmarshal_DepthMask(struct gl_context *ctx, const struct marshal_cmd_DepthMask *cmd);
struct marshal_cmd_IndexMask;
uint32_t _mesa_unmarshal_IndexMask(struct gl_context *ctx, const struct marshal_cmd_IndexMask *cmd);
struct marshal_cmd_Accum;
uint32_t _mesa_unmarshal_Accum(struct gl_context *ctx, const struct marshal_cmd_Accum *cmd);
struct marshal_cmd_Disable;
uint32_t _mesa_unmarshal_Disable(struct gl_context *ctx, const struct marshal_cmd_Disable *cmd);
struct marshal_cmd_Enable;
uint32_t _mesa_unmarshal_Enable(struct gl_context *ctx, const struct marshal_cmd_Enable *cmd);
struct marshal_cmd_Flush;
uint32_t _mesa_unmarshal_Flush(struct gl_context *ctx, const struct marshal_cmd_Flush *cmd);
struct marshal_cmd_PopAttrib;
uint32_t _mesa_unmarshal_PopAttrib(struct gl_context *ctx, const struct marshal_cmd_PopAttrib *cmd);
struct marshal_cmd_PushAttrib;
uint32_t _mesa_unmarshal_PushAttrib(struct gl_context *ctx, const struct marshal_cmd_PushAttrib *cmd);
struct marshal_cmd_MapGrid1d;
uint32_t _mesa_unmarshal_MapGrid1d(struct gl_context *ctx, const struct marshal_cmd_MapGrid1d *cmd);
struct marshal_cmd_MapGrid1f;
uint32_t _mesa_unmarshal_MapGrid1f(struct gl_context *ctx, const struct marshal_cmd_MapGrid1f *cmd);
struct marshal_cmd_MapGrid2d;
uint32_t _mesa_unmarshal_MapGrid2d(struct gl_context *ctx, const struct marshal_cmd_MapGrid2d *cmd);
struct marshal_cmd_MapGrid2f;
uint32_t _mesa_unmarshal_MapGrid2f(struct gl_context *ctx, const struct marshal_cmd_MapGrid2f *cmd);
struct marshal_cmd_EvalCoord1d;
uint32_t _mesa_unmarshal_EvalCoord1d(struct gl_context *ctx, const struct marshal_cmd_EvalCoord1d *cmd);
struct marshal_cmd_EvalCoord1dv;
uint32_t _mesa_unmarshal_EvalCoord1dv(struct gl_context *ctx, const struct marshal_cmd_EvalCoord1dv *cmd);
struct marshal_cmd_EvalCoord1f;
uint32_t _mesa_unmarshal_EvalCoord1f(struct gl_context *ctx, const struct marshal_cmd_EvalCoord1f *cmd);
struct marshal_cmd_EvalCoord1fv;
uint32_t _mesa_unmarshal_EvalCoord1fv(struct gl_context *ctx, const struct marshal_cmd_EvalCoord1fv *cmd);
struct marshal_cmd_EvalCoord2d;
uint32_t _mesa_unmarshal_EvalCoord2d(struct gl_context *ctx, const struct marshal_cmd_EvalCoord2d *cmd);
struct marshal_cmd_EvalCoord2dv;
uint32_t _mesa_unmarshal_EvalCoord2dv(struct gl_context *ctx, const struct marshal_cmd_EvalCoord2dv *cmd);
struct marshal_cmd_EvalCoord2f;
uint32_t _mesa_unmarshal_EvalCoord2f(struct gl_context *ctx, const struct marshal_cmd_EvalCoord2f *cmd);
struct marshal_cmd_EvalCoord2fv;
uint32_t _mesa_unmarshal_EvalCoord2fv(struct gl_context *ctx, const struct marshal_cmd_EvalCoord2fv *cmd);
struct marshal_cmd_EvalMesh1;
uint32_t _mesa_unmarshal_EvalMesh1(struct gl_context *ctx, const struct marshal_cmd_EvalMesh1 *cmd);
struct marshal_cmd_EvalPoint1;
uint32_t _mesa_unmarshal_EvalPoint1(struct gl_context *ctx, const struct marshal_cmd_EvalPoint1 *cmd);
struct marshal_cmd_EvalMesh2;
uint32_t _mesa_unmarshal_EvalMesh2(struct gl_context *ctx, const struct marshal_cmd_EvalMesh2 *cmd);
struct marshal_cmd_EvalPoint2;
uint32_t _mesa_unmarshal_EvalPoint2(struct gl_context *ctx, const struct marshal_cmd_EvalPoint2 *cmd);
struct marshal_cmd_AlphaFunc;
uint32_t _mesa_unmarshal_AlphaFunc(struct gl_context *ctx, const struct marshal_cmd_AlphaFunc *cmd);
struct marshal_cmd_BlendFunc;
uint32_t _mesa_unmarshal_BlendFunc(struct gl_context *ctx, const struct marshal_cmd_BlendFunc *cmd);
struct marshal_cmd_LogicOp;
uint32_t _mesa_unmarshal_LogicOp(struct gl_context *ctx, const struct marshal_cmd_LogicOp *cmd);
struct marshal_cmd_StencilFunc;
uint32_t _mesa_unmarshal_StencilFunc(struct gl_context *ctx, const struct marshal_cmd_StencilFunc *cmd);
struct marshal_cmd_StencilOp;
uint32_t _mesa_unmarshal_StencilOp(struct gl_context *ctx, const struct marshal_cmd_StencilOp *cmd);
struct marshal_cmd_DepthFunc;
uint32_t _mesa_unmarshal_DepthFunc(struct gl_context *ctx, const struct marshal_cmd_DepthFunc *cmd);
struct marshal_cmd_PixelZoom;
uint32_t _mesa_unmarshal_PixelZoom(struct gl_context *ctx, const struct marshal_cmd_PixelZoom *cmd);
struct marshal_cmd_PixelTransferf;
uint32_t _mesa_unmarshal_PixelTransferf(struct gl_context *ctx, const struct marshal_cmd_PixelTransferf *cmd);
struct marshal_cmd_PixelTransferi;
uint32_t _mesa_unmarshal_PixelTransferi(struct gl_context *ctx, const struct marshal_cmd_PixelTransferi *cmd);
struct marshal_cmd_PixelStoref;
uint32_t _mesa_unmarshal_PixelStoref(struct gl_context *ctx, const struct marshal_cmd_PixelStoref *cmd);
struct marshal_cmd_PixelStorei;
uint32_t _mesa_unmarshal_PixelStorei(struct gl_context *ctx, const struct marshal_cmd_PixelStorei *cmd);
struct marshal_cmd_PixelMapfv;
uint32_t _mesa_unmarshal_PixelMapfv(struct gl_context *ctx, const struct marshal_cmd_PixelMapfv *cmd);
struct marshal_cmd_PixelMapuiv;
uint32_t _mesa_unmarshal_PixelMapuiv(struct gl_context *ctx, const struct marshal_cmd_PixelMapuiv *cmd);
struct marshal_cmd_PixelMapusv;
uint32_t _mesa_unmarshal_PixelMapusv(struct gl_context *ctx, const struct marshal_cmd_PixelMapusv *cmd);
struct marshal_cmd_ReadBuffer;
uint32_t _mesa_unmarshal_ReadBuffer(struct gl_context *ctx, const struct marshal_cmd_ReadBuffer *cmd);
struct marshal_cmd_CopyPixels;
uint32_t _mesa_unmarshal_CopyPixels(struct gl_context *ctx, const struct marshal_cmd_CopyPixels *cmd);
struct marshal_cmd_ReadPixels;
uint32_t _mesa_unmarshal_ReadPixels(struct gl_context *ctx, const struct marshal_cmd_ReadPixels *cmd);
struct marshal_cmd_DrawPixels;
uint32_t _mesa_unmarshal_DrawPixels(struct gl_context *ctx, const struct marshal_cmd_DrawPixels *cmd);
struct marshal_cmd_GetIntegerv;
uint32_t _mesa_unmarshal_GetIntegerv(struct gl_context *ctx, const struct marshal_cmd_GetIntegerv *cmd);
void GLAPIENTRY _mesa_marshal_GetIntegerv(GLenum pname, GLint * params);
struct marshal_cmd_GetPixelMapfv;
uint32_t _mesa_unmarshal_GetPixelMapfv(struct gl_context *ctx, const struct marshal_cmd_GetPixelMapfv *cmd);
struct marshal_cmd_GetPixelMapuiv;
uint32_t _mesa_unmarshal_GetPixelMapuiv(struct gl_context *ctx, const struct marshal_cmd_GetPixelMapuiv *cmd);
struct marshal_cmd_GetPixelMapusv;
uint32_t _mesa_unmarshal_GetPixelMapusv(struct gl_context *ctx, const struct marshal_cmd_GetPixelMapusv *cmd);
struct marshal_cmd_GetPolygonStipple;
uint32_t _mesa_unmarshal_GetPolygonStipple(struct gl_context *ctx, const struct marshal_cmd_GetPolygonStipple *cmd);
struct marshal_cmd_GetTexImage;
uint32_t _mesa_unmarshal_GetTexImage(struct gl_context *ctx, const struct marshal_cmd_GetTexImage *cmd);
struct marshal_cmd_DepthRange;
uint32_t _mesa_unmarshal_DepthRange(struct gl_context *ctx, const struct marshal_cmd_DepthRange *cmd);
struct marshal_cmd_Frustum;
uint32_t _mesa_unmarshal_Frustum(struct gl_context *ctx, const struct marshal_cmd_Frustum *cmd);
struct marshal_cmd_LoadIdentity;
uint32_t _mesa_unmarshal_LoadIdentity(struct gl_context *ctx, const struct marshal_cmd_LoadIdentity *cmd);
struct marshal_cmd_LoadMatrixf;
uint32_t _mesa_unmarshal_LoadMatrixf(struct gl_context *ctx, const struct marshal_cmd_LoadMatrixf *cmd);
struct marshal_cmd_LoadMatrixd;
uint32_t _mesa_unmarshal_LoadMatrixd(struct gl_context *ctx, const struct marshal_cmd_LoadMatrixd *cmd);
struct marshal_cmd_MatrixMode;
uint32_t _mesa_unmarshal_MatrixMode(struct gl_context *ctx, const struct marshal_cmd_MatrixMode *cmd);
struct marshal_cmd_MultMatrixf;
uint32_t _mesa_unmarshal_MultMatrixf(struct gl_context *ctx, const struct marshal_cmd_MultMatrixf *cmd);
struct marshal_cmd_MultMatrixd;
uint32_t _mesa_unmarshal_MultMatrixd(struct gl_context *ctx, const struct marshal_cmd_MultMatrixd *cmd);
struct marshal_cmd_Ortho;
uint32_t _mesa_unmarshal_Ortho(struct gl_context *ctx, const struct marshal_cmd_Ortho *cmd);
struct marshal_cmd_PopMatrix;
uint32_t _mesa_unmarshal_PopMatrix(struct gl_context *ctx, const struct marshal_cmd_PopMatrix *cmd);
struct marshal_cmd_PushMatrix;
uint32_t _mesa_unmarshal_PushMatrix(struct gl_context *ctx, const struct marshal_cmd_PushMatrix *cmd);
struct marshal_cmd_Rotated;
uint32_t _mesa_unmarshal_Rotated(struct gl_context *ctx, const struct marshal_cmd_Rotated *cmd);
struct marshal_cmd_Rotatef;
uint32_t _mesa_unmarshal_Rotatef(struct gl_context *ctx, const struct marshal_cmd_Rotatef *cmd);
struct marshal_cmd_Scaled;
uint32_t _mesa_unmarshal_Scaled(struct gl_context *ctx, const struct marshal_cmd_Scaled *cmd);
struct marshal_cmd_Scalef;
uint32_t _mesa_unmarshal_Scalef(struct gl_context *ctx, const struct marshal_cmd_Scalef *cmd);
struct marshal_cmd_Translated;
uint32_t _mesa_unmarshal_Translated(struct gl_context *ctx, const struct marshal_cmd_Translated *cmd);
struct marshal_cmd_Translatef;
uint32_t _mesa_unmarshal_Translatef(struct gl_context *ctx, const struct marshal_cmd_Translatef *cmd);
struct marshal_cmd_Viewport;
uint32_t _mesa_unmarshal_Viewport(struct gl_context *ctx, const struct marshal_cmd_Viewport *cmd);
struct marshal_cmd_ColorPointer;
uint32_t _mesa_unmarshal_ColorPointer(struct gl_context *ctx, const struct marshal_cmd_ColorPointer *cmd);
struct marshal_cmd_DisableClientState;
uint32_t _mesa_unmarshal_DisableClientState(struct gl_context *ctx, const struct marshal_cmd_DisableClientState *cmd);
struct marshal_cmd_DrawArrays;
uint32_t _mesa_unmarshal_DrawArrays(struct gl_context *ctx, const struct marshal_cmd_DrawArrays *cmd);
void GLAPIENTRY _mesa_marshal_DrawArrays(GLenum mode, GLint first, GLsizei count);
struct marshal_cmd_DrawElements;
uint32_t _mesa_unmarshal_DrawElements(struct gl_context *ctx, const struct marshal_cmd_DrawElements *cmd);
void GLAPIENTRY _mesa_marshal_DrawElements(GLenum mode, GLsizei count, GLenum type, const GLvoid * indices);
struct marshal_cmd_EdgeFlagPointer;
uint32_t _mesa_unmarshal_EdgeFlagPointer(struct gl_context *ctx, const struct marshal_cmd_EdgeFlagPointer *cmd);
struct marshal_cmd_EnableClientState;
uint32_t _mesa_unmarshal_EnableClientState(struct gl_context *ctx, const struct marshal_cmd_EnableClientState *cmd);
struct marshal_cmd_IndexPointer;
uint32_t _mesa_unmarshal_IndexPointer(struct gl_context *ctx, const struct marshal_cmd_IndexPointer *cmd);
struct marshal_cmd_InterleavedArrays;
uint32_t _mesa_unmarshal_InterleavedArrays(struct gl_context *ctx, const struct marshal_cmd_InterleavedArrays *cmd);
struct marshal_cmd_NormalPointer;
uint32_t _mesa_unmarshal_NormalPointer(struct gl_context *ctx, const struct marshal_cmd_NormalPointer *cmd);
struct marshal_cmd_TexCoordPointer;
uint32_t _mesa_unmarshal_TexCoordPointer(struct gl_context *ctx, const struct marshal_cmd_TexCoordPointer *cmd);
struct marshal_cmd_VertexPointer;
uint32_t _mesa_unmarshal_VertexPointer(struct gl_context *ctx, const struct marshal_cmd_VertexPointer *cmd);
struct marshal_cmd_PolygonOffset;
uint32_t _mesa_unmarshal_PolygonOffset(struct gl_context *ctx, const struct marshal_cmd_PolygonOffset *cmd);
struct marshal_cmd_CopyTexImage1D;
uint32_t _mesa_unmarshal_CopyTexImage1D(struct gl_context *ctx, const struct marshal_cmd_CopyTexImage1D *cmd);
struct marshal_cmd_CopyTexImage2D;
uint32_t _mesa_unmarshal_CopyTexImage2D(struct gl_context *ctx, const struct marshal_cmd_CopyTexImage2D *cmd);
struct marshal_cmd_CopyTexSubImage1D;
uint32_t _mesa_unmarshal_CopyTexSubImage1D(struct gl_context *ctx, const struct marshal_cmd_CopyTexSubImage1D *cmd);
struct marshal_cmd_CopyTexSubImage2D;
uint32_t _mesa_unmarshal_CopyTexSubImage2D(struct gl_context *ctx, const struct marshal_cmd_CopyTexSubImage2D *cmd);
struct marshal_cmd_TexSubImage1D;
uint32_t _mesa_unmarshal_TexSubImage1D(struct gl_context *ctx, const struct marshal_cmd_TexSubImage1D *cmd);
struct marshal_cmd_TexSubImage2D;
uint32_t _mesa_unmarshal_TexSubImage2D(struct gl_context *ctx, const struct marshal_cmd_TexSubImage2D *cmd);
struct marshal_cmd_BindTexture;
uint32_t _mesa_unmarshal_BindTexture(struct gl_context *ctx, const struct marshal_cmd_BindTexture *cmd);
struct marshal_cmd_DeleteTextures;
uint32_t _mesa_unmarshal_DeleteTextures(struct gl_context *ctx, const struct marshal_cmd_DeleteTextures *cmd);
struct marshal_cmd_PrioritizeTextures;
uint32_t _mesa_unmarshal_PrioritizeTextures(struct gl_context *ctx, const struct marshal_cmd_PrioritizeTextures *cmd);
struct marshal_cmd_Indexub;
uint32_t _mesa_unmarshal_Indexub(struct gl_context *ctx, const struct marshal_cmd_Indexub *cmd);
struct marshal_cmd_Indexubv;
uint32_t _mesa_unmarshal_Indexubv(struct gl_context *ctx, const struct marshal_cmd_Indexubv *cmd);
struct marshal_cmd_PopClientAttrib;
uint32_t _mesa_unmarshal_PopClientAttrib(struct gl_context *ctx, const struct marshal_cmd_PopClientAttrib *cmd);
struct marshal_cmd_PushClientAttrib;
uint32_t _mesa_unmarshal_PushClientAttrib(struct gl_context *ctx, const struct marshal_cmd_PushClientAttrib *cmd);
struct marshal_cmd_BlendColor;
uint32_t _mesa_unmarshal_BlendColor(struct gl_context *ctx, const struct marshal_cmd_BlendColor *cmd);
struct marshal_cmd_BlendEquation;
uint32_t _mesa_unmarshal_BlendEquation(struct gl_context *ctx, const struct marshal_cmd_BlendEquation *cmd);
struct marshal_cmd_DrawRangeElements;
uint32_t _mesa_unmarshal_DrawRangeElements(struct gl_context *ctx, const struct marshal_cmd_DrawRangeElements *cmd);
void GLAPIENTRY _mesa_marshal_DrawRangeElements(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const GLvoid * indices);
struct marshal_cmd_TexImage3D;
uint32_t _mesa_unmarshal_TexImage3D(struct gl_context *ctx, const struct marshal_cmd_TexImage3D *cmd);
struct marshal_cmd_TexSubImage3D;
uint32_t _mesa_unmarshal_TexSubImage3D(struct gl_context *ctx, const struct marshal_cmd_TexSubImage3D *cmd);
struct marshal_cmd_CopyTexSubImage3D;
uint32_t _mesa_unmarshal_CopyTexSubImage3D(struct gl_context *ctx, const struct marshal_cmd_CopyTexSubImage3D *cmd);
struct marshal_cmd_ActiveTexture;
uint32_t _mesa_unmarshal_ActiveTexture(struct gl_context *ctx, const struct marshal_cmd_ActiveTexture *cmd);
struct marshal_cmd_ClientActiveTexture;
uint32_t _mesa_unmarshal_ClientActiveTexture(struct gl_context *ctx, const struct marshal_cmd_ClientActiveTexture *cmd);
struct marshal_cmd_MultiTexCoord1d;
uint32_t _mesa_unmarshal_MultiTexCoord1d(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord1d *cmd);
struct marshal_cmd_MultiTexCoord1dv;
uint32_t _mesa_unmarshal_MultiTexCoord1dv(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord1dv *cmd);
struct marshal_cmd_MultiTexCoord1fARB;
uint32_t _mesa_unmarshal_MultiTexCoord1fARB(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord1fARB *cmd);
struct marshal_cmd_MultiTexCoord1fvARB;
uint32_t _mesa_unmarshal_MultiTexCoord1fvARB(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord1fvARB *cmd);
struct marshal_cmd_MultiTexCoord1i;
uint32_t _mesa_unmarshal_MultiTexCoord1i(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord1i *cmd);
struct marshal_cmd_MultiTexCoord1iv;
uint32_t _mesa_unmarshal_MultiTexCoord1iv(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord1iv *cmd);
struct marshal_cmd_MultiTexCoord1s;
uint32_t _mesa_unmarshal_MultiTexCoord1s(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord1s *cmd);
struct marshal_cmd_MultiTexCoord1sv;
uint32_t _mesa_unmarshal_MultiTexCoord1sv(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord1sv *cmd);
struct marshal_cmd_MultiTexCoord2d;
uint32_t _mesa_unmarshal_MultiTexCoord2d(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord2d *cmd);
struct marshal_cmd_MultiTexCoord2dv;
uint32_t _mesa_unmarshal_MultiTexCoord2dv(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord2dv *cmd);
struct marshal_cmd_MultiTexCoord2fARB;
uint32_t _mesa_unmarshal_MultiTexCoord2fARB(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord2fARB *cmd);
struct marshal_cmd_MultiTexCoord2fvARB;
uint32_t _mesa_unmarshal_MultiTexCoord2fvARB(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord2fvARB *cmd);
struct marshal_cmd_MultiTexCoord2i;
uint32_t _mesa_unmarshal_MultiTexCoord2i(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord2i *cmd);
struct marshal_cmd_MultiTexCoord2iv;
uint32_t _mesa_unmarshal_MultiTexCoord2iv(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord2iv *cmd);
struct marshal_cmd_MultiTexCoord2s;
uint32_t _mesa_unmarshal_MultiTexCoord2s(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord2s *cmd);
struct marshal_cmd_MultiTexCoord2sv;
uint32_t _mesa_unmarshal_MultiTexCoord2sv(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord2sv *cmd);
struct marshal_cmd_MultiTexCoord3d;
uint32_t _mesa_unmarshal_MultiTexCoord3d(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord3d *cmd);
struct marshal_cmd_MultiTexCoord3dv;
uint32_t _mesa_unmarshal_MultiTexCoord3dv(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord3dv *cmd);
struct marshal_cmd_MultiTexCoord3fARB;
uint32_t _mesa_unmarshal_MultiTexCoord3fARB(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord3fARB *cmd);
struct marshal_cmd_MultiTexCoord3fvARB;
uint32_t _mesa_unmarshal_MultiTexCoord3fvARB(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord3fvARB *cmd);
struct marshal_cmd_MultiTexCoord3i;
uint32_t _mesa_unmarshal_MultiTexCoord3i(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord3i *cmd);
struct marshal_cmd_MultiTexCoord3iv;
uint32_t _mesa_unmarshal_MultiTexCoord3iv(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord3iv *cmd);
struct marshal_cmd_MultiTexCoord3s;
uint32_t _mesa_unmarshal_MultiTexCoord3s(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord3s *cmd);
struct marshal_cmd_MultiTexCoord3sv;
uint32_t _mesa_unmarshal_MultiTexCoord3sv(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord3sv *cmd);
struct marshal_cmd_MultiTexCoord4d;
uint32_t _mesa_unmarshal_MultiTexCoord4d(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord4d *cmd);
struct marshal_cmd_MultiTexCoord4dv;
uint32_t _mesa_unmarshal_MultiTexCoord4dv(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord4dv *cmd);
struct marshal_cmd_MultiTexCoord4fARB;
uint32_t _mesa_unmarshal_MultiTexCoord4fARB(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord4fARB *cmd);
struct marshal_cmd_MultiTexCoord4fvARB;
uint32_t _mesa_unmarshal_MultiTexCoord4fvARB(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord4fvARB *cmd);
struct marshal_cmd_MultiTexCoord4i;
uint32_t _mesa_unmarshal_MultiTexCoord4i(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord4i *cmd);
struct marshal_cmd_MultiTexCoord4iv;
uint32_t _mesa_unmarshal_MultiTexCoord4iv(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord4iv *cmd);
struct marshal_cmd_MultiTexCoord4s;
uint32_t _mesa_unmarshal_MultiTexCoord4s(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord4s *cmd);
struct marshal_cmd_MultiTexCoord4sv;
uint32_t _mesa_unmarshal_MultiTexCoord4sv(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord4sv *cmd);
struct marshal_cmd_LoadTransposeMatrixf;
uint32_t _mesa_unmarshal_LoadTransposeMatrixf(struct gl_context *ctx, const struct marshal_cmd_LoadTransposeMatrixf *cmd);
struct marshal_cmd_LoadTransposeMatrixd;
uint32_t _mesa_unmarshal_LoadTransposeMatrixd(struct gl_context *ctx, const struct marshal_cmd_LoadTransposeMatrixd *cmd);
struct marshal_cmd_MultTransposeMatrixf;
uint32_t _mesa_unmarshal_MultTransposeMatrixf(struct gl_context *ctx, const struct marshal_cmd_MultTransposeMatrixf *cmd);
struct marshal_cmd_MultTransposeMatrixd;
uint32_t _mesa_unmarshal_MultTransposeMatrixd(struct gl_context *ctx, const struct marshal_cmd_MultTransposeMatrixd *cmd);
struct marshal_cmd_SampleCoverage;
uint32_t _mesa_unmarshal_SampleCoverage(struct gl_context *ctx, const struct marshal_cmd_SampleCoverage *cmd);
struct marshal_cmd_CompressedTexImage3D;
uint32_t _mesa_unmarshal_CompressedTexImage3D(struct gl_context *ctx, const struct marshal_cmd_CompressedTexImage3D *cmd);
struct marshal_cmd_CompressedTexImage2D;
uint32_t _mesa_unmarshal_CompressedTexImage2D(struct gl_context *ctx, const struct marshal_cmd_CompressedTexImage2D *cmd);
struct marshal_cmd_CompressedTexImage1D;
uint32_t _mesa_unmarshal_CompressedTexImage1D(struct gl_context *ctx, const struct marshal_cmd_CompressedTexImage1D *cmd);
struct marshal_cmd_CompressedTexSubImage3D;
uint32_t _mesa_unmarshal_CompressedTexSubImage3D(struct gl_context *ctx, const struct marshal_cmd_CompressedTexSubImage3D *cmd);
struct marshal_cmd_CompressedTexSubImage2D;
uint32_t _mesa_unmarshal_CompressedTexSubImage2D(struct gl_context *ctx, const struct marshal_cmd_CompressedTexSubImage2D *cmd);
struct marshal_cmd_CompressedTexSubImage1D;
uint32_t _mesa_unmarshal_CompressedTexSubImage1D(struct gl_context *ctx, const struct marshal_cmd_CompressedTexSubImage1D *cmd);
struct marshal_cmd_GetCompressedTexImage;
uint32_t _mesa_unmarshal_GetCompressedTexImage(struct gl_context *ctx, const struct marshal_cmd_GetCompressedTexImage *cmd);
struct marshal_cmd_BlendFuncSeparate;
uint32_t _mesa_unmarshal_BlendFuncSeparate(struct gl_context *ctx, const struct marshal_cmd_BlendFuncSeparate *cmd);
struct marshal_cmd_FogCoordfEXT;
uint32_t _mesa_unmarshal_FogCoordfEXT(struct gl_context *ctx, const struct marshal_cmd_FogCoordfEXT *cmd);
struct marshal_cmd_FogCoordfvEXT;
uint32_t _mesa_unmarshal_FogCoordfvEXT(struct gl_context *ctx, const struct marshal_cmd_FogCoordfvEXT *cmd);
struct marshal_cmd_FogCoordd;
uint32_t _mesa_unmarshal_FogCoordd(struct gl_context *ctx, const struct marshal_cmd_FogCoordd *cmd);
struct marshal_cmd_FogCoorddv;
uint32_t _mesa_unmarshal_FogCoorddv(struct gl_context *ctx, const struct marshal_cmd_FogCoorddv *cmd);
struct marshal_cmd_FogCoordPointer;
uint32_t _mesa_unmarshal_FogCoordPointer(struct gl_context *ctx, const struct marshal_cmd_FogCoordPointer *cmd);
struct marshal_cmd_MultiDrawArrays;
uint32_t _mesa_unmarshal_MultiDrawArrays(struct gl_context *ctx, const struct marshal_cmd_MultiDrawArrays *cmd);
void GLAPIENTRY _mesa_marshal_MultiDrawArrays(GLenum mode, const GLint * first, const GLsizei * count, GLsizei primcount);
struct marshal_cmd_MultiDrawElements;
uint32_t _mesa_unmarshal_MultiDrawElements(struct gl_context *ctx, const struct marshal_cmd_MultiDrawElements *cmd);
void GLAPIENTRY _mesa_marshal_MultiDrawElements(GLenum mode, const GLsizei * count, GLenum type, const GLvoid * const * indices, GLsizei primcount);
struct marshal_cmd_PointParameterf;
uint32_t _mesa_unmarshal_PointParameterf(struct gl_context *ctx, const struct marshal_cmd_PointParameterf *cmd);
struct marshal_cmd_PointParameterfv;
uint32_t _mesa_unmarshal_PointParameterfv(struct gl_context *ctx, const struct marshal_cmd_PointParameterfv *cmd);
struct marshal_cmd_PointParameteri;
uint32_t _mesa_unmarshal_PointParameteri(struct gl_context *ctx, const struct marshal_cmd_PointParameteri *cmd);
struct marshal_cmd_PointParameteriv;
uint32_t _mesa_unmarshal_PointParameteriv(struct gl_context *ctx, const struct marshal_cmd_PointParameteriv *cmd);
struct marshal_cmd_SecondaryColor3b;
uint32_t _mesa_unmarshal_SecondaryColor3b(struct gl_context *ctx, const struct marshal_cmd_SecondaryColor3b *cmd);
struct marshal_cmd_SecondaryColor3bv;
uint32_t _mesa_unmarshal_SecondaryColor3bv(struct gl_context *ctx, const struct marshal_cmd_SecondaryColor3bv *cmd);
struct marshal_cmd_SecondaryColor3d;
uint32_t _mesa_unmarshal_SecondaryColor3d(struct gl_context *ctx, const struct marshal_cmd_SecondaryColor3d *cmd);
struct marshal_cmd_SecondaryColor3dv;
uint32_t _mesa_unmarshal_SecondaryColor3dv(struct gl_context *ctx, const struct marshal_cmd_SecondaryColor3dv *cmd);
struct marshal_cmd_SecondaryColor3fEXT;
uint32_t _mesa_unmarshal_SecondaryColor3fEXT(struct gl_context *ctx, const struct marshal_cmd_SecondaryColor3fEXT *cmd);
struct marshal_cmd_SecondaryColor3fvEXT;
uint32_t _mesa_unmarshal_SecondaryColor3fvEXT(struct gl_context *ctx, const struct marshal_cmd_SecondaryColor3fvEXT *cmd);
struct marshal_cmd_SecondaryColor3i;
uint32_t _mesa_unmarshal_SecondaryColor3i(struct gl_context *ctx, const struct marshal_cmd_SecondaryColor3i *cmd);
struct marshal_cmd_SecondaryColor3iv;
uint32_t _mesa_unmarshal_SecondaryColor3iv(struct gl_context *ctx, const struct marshal_cmd_SecondaryColor3iv *cmd);
struct marshal_cmd_SecondaryColor3s;
uint32_t _mesa_unmarshal_SecondaryColor3s(struct gl_context *ctx, const struct marshal_cmd_SecondaryColor3s *cmd);
struct marshal_cmd_SecondaryColor3sv;
uint32_t _mesa_unmarshal_SecondaryColor3sv(struct gl_context *ctx, const struct marshal_cmd_SecondaryColor3sv *cmd);
struct marshal_cmd_SecondaryColor3ub;
uint32_t _mesa_unmarshal_SecondaryColor3ub(struct gl_context *ctx, const struct marshal_cmd_SecondaryColor3ub *cmd);
struct marshal_cmd_SecondaryColor3ubv;
uint32_t _mesa_unmarshal_SecondaryColor3ubv(struct gl_context *ctx, const struct marshal_cmd_SecondaryColor3ubv *cmd);
struct marshal_cmd_SecondaryColor3ui;
uint32_t _mesa_unmarshal_SecondaryColor3ui(struct gl_context *ctx, const struct marshal_cmd_SecondaryColor3ui *cmd);
struct marshal_cmd_SecondaryColor3uiv;
uint32_t _mesa_unmarshal_SecondaryColor3uiv(struct gl_context *ctx, const struct marshal_cmd_SecondaryColor3uiv *cmd);
struct marshal_cmd_SecondaryColor3us;
uint32_t _mesa_unmarshal_SecondaryColor3us(struct gl_context *ctx, const struct marshal_cmd_SecondaryColor3us *cmd);
struct marshal_cmd_SecondaryColor3usv;
uint32_t _mesa_unmarshal_SecondaryColor3usv(struct gl_context *ctx, const struct marshal_cmd_SecondaryColor3usv *cmd);
struct marshal_cmd_SecondaryColorPointer;
uint32_t _mesa_unmarshal_SecondaryColorPointer(struct gl_context *ctx, const struct marshal_cmd_SecondaryColorPointer *cmd);
struct marshal_cmd_WindowPos2d;
uint32_t _mesa_unmarshal_WindowPos2d(struct gl_context *ctx, const struct marshal_cmd_WindowPos2d *cmd);
struct marshal_cmd_WindowPos2dv;
uint32_t _mesa_unmarshal_WindowPos2dv(struct gl_context *ctx, const struct marshal_cmd_WindowPos2dv *cmd);
struct marshal_cmd_WindowPos2f;
uint32_t _mesa_unmarshal_WindowPos2f(struct gl_context *ctx, const struct marshal_cmd_WindowPos2f *cmd);
struct marshal_cmd_WindowPos2fv;
uint32_t _mesa_unmarshal_WindowPos2fv(struct gl_context *ctx, const struct marshal_cmd_WindowPos2fv *cmd);
struct marshal_cmd_WindowPos2i;
uint32_t _mesa_unmarshal_WindowPos2i(struct gl_context *ctx, const struct marshal_cmd_WindowPos2i *cmd);
struct marshal_cmd_WindowPos2iv;
uint32_t _mesa_unmarshal_WindowPos2iv(struct gl_context *ctx, const struct marshal_cmd_WindowPos2iv *cmd);
struct marshal_cmd_WindowPos2s;
uint32_t _mesa_unmarshal_WindowPos2s(struct gl_context *ctx, const struct marshal_cmd_WindowPos2s *cmd);
struct marshal_cmd_WindowPos2sv;
uint32_t _mesa_unmarshal_WindowPos2sv(struct gl_context *ctx, const struct marshal_cmd_WindowPos2sv *cmd);
struct marshal_cmd_WindowPos3d;
uint32_t _mesa_unmarshal_WindowPos3d(struct gl_context *ctx, const struct marshal_cmd_WindowPos3d *cmd);
struct marshal_cmd_WindowPos3dv;
uint32_t _mesa_unmarshal_WindowPos3dv(struct gl_context *ctx, const struct marshal_cmd_WindowPos3dv *cmd);
struct marshal_cmd_WindowPos3f;
uint32_t _mesa_unmarshal_WindowPos3f(struct gl_context *ctx, const struct marshal_cmd_WindowPos3f *cmd);
struct marshal_cmd_WindowPos3fv;
uint32_t _mesa_unmarshal_WindowPos3fv(struct gl_context *ctx, const struct marshal_cmd_WindowPos3fv *cmd);
struct marshal_cmd_WindowPos3i;
uint32_t _mesa_unmarshal_WindowPos3i(struct gl_context *ctx, const struct marshal_cmd_WindowPos3i *cmd);
struct marshal_cmd_WindowPos3iv;
uint32_t _mesa_unmarshal_WindowPos3iv(struct gl_context *ctx, const struct marshal_cmd_WindowPos3iv *cmd);
struct marshal_cmd_WindowPos3s;
uint32_t _mesa_unmarshal_WindowPos3s(struct gl_context *ctx, const struct marshal_cmd_WindowPos3s *cmd);
struct marshal_cmd_WindowPos3sv;
uint32_t _mesa_unmarshal_WindowPos3sv(struct gl_context *ctx, const struct marshal_cmd_WindowPos3sv *cmd);
struct marshal_cmd_BindBuffer;
uint32_t _mesa_unmarshal_BindBuffer(struct gl_context *ctx, const struct marshal_cmd_BindBuffer *cmd);
void GLAPIENTRY _mesa_marshal_BindBuffer(GLenum target, GLuint buffer);
struct marshal_cmd_BufferData;
uint32_t _mesa_unmarshal_BufferData(struct gl_context *ctx, const struct marshal_cmd_BufferData *cmd);
void GLAPIENTRY _mesa_marshal_BufferData(GLenum target, GLsizeiptr size, const GLvoid * data, GLenum usage);
struct marshal_cmd_BufferSubData;
uint32_t _mesa_unmarshal_BufferSubData(struct gl_context *ctx, const struct marshal_cmd_BufferSubData *cmd);
void GLAPIENTRY _mesa_marshal_BufferSubData(GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid * data);
struct marshal_cmd_DeleteBuffers;
uint32_t _mesa_unmarshal_DeleteBuffers(struct gl_context *ctx, const struct marshal_cmd_DeleteBuffers *cmd);
struct marshal_cmd_UnmapBuffer;
uint32_t _mesa_unmarshal_UnmapBuffer(struct gl_context *ctx, const struct marshal_cmd_UnmapBuffer *cmd);
struct marshal_cmd_DeleteQueries;
uint32_t _mesa_unmarshal_DeleteQueries(struct gl_context *ctx, const struct marshal_cmd_DeleteQueries *cmd);
struct marshal_cmd_BeginQuery;
uint32_t _mesa_unmarshal_BeginQuery(struct gl_context *ctx, const struct marshal_cmd_BeginQuery *cmd);
struct marshal_cmd_EndQuery;
uint32_t _mesa_unmarshal_EndQuery(struct gl_context *ctx, const struct marshal_cmd_EndQuery *cmd);
struct marshal_cmd_BlendEquationSeparate;
uint32_t _mesa_unmarshal_BlendEquationSeparate(struct gl_context *ctx, const struct marshal_cmd_BlendEquationSeparate *cmd);
struct marshal_cmd_DrawBuffers;
uint32_t _mesa_unmarshal_DrawBuffers(struct gl_context *ctx, const struct marshal_cmd_DrawBuffers *cmd);
struct marshal_cmd_StencilFuncSeparate;
uint32_t _mesa_unmarshal_StencilFuncSeparate(struct gl_context *ctx, const struct marshal_cmd_StencilFuncSeparate *cmd);
struct marshal_cmd_StencilOpSeparate;
uint32_t _mesa_unmarshal_StencilOpSeparate(struct gl_context *ctx, const struct marshal_cmd_StencilOpSeparate *cmd);
struct marshal_cmd_StencilMaskSeparate;
uint32_t _mesa_unmarshal_StencilMaskSeparate(struct gl_context *ctx, const struct marshal_cmd_StencilMaskSeparate *cmd);
struct marshal_cmd_AttachShader;
uint32_t _mesa_unmarshal_AttachShader(struct gl_context *ctx, const struct marshal_cmd_AttachShader *cmd);
struct marshal_cmd_BindAttribLocation;
uint32_t _mesa_unmarshal_BindAttribLocation(struct gl_context *ctx, const struct marshal_cmd_BindAttribLocation *cmd);
struct marshal_cmd_CompileShader;
uint32_t _mesa_unmarshal_CompileShader(struct gl_context *ctx, const struct marshal_cmd_CompileShader *cmd);
struct marshal_cmd_DeleteProgram;
uint32_t _mesa_unmarshal_DeleteProgram(struct gl_context *ctx, const struct marshal_cmd_DeleteProgram *cmd);
struct marshal_cmd_DeleteShader;
uint32_t _mesa_unmarshal_DeleteShader(struct gl_context *ctx, const struct marshal_cmd_DeleteShader *cmd);
struct marshal_cmd_DetachShader;
uint32_t _mesa_unmarshal_DetachShader(struct gl_context *ctx, const struct marshal_cmd_DetachShader *cmd);
struct marshal_cmd_DisableVertexAttribArray;
uint32_t _mesa_unmarshal_DisableVertexAttribArray(struct gl_context *ctx, const struct marshal_cmd_DisableVertexAttribArray *cmd);
struct marshal_cmd_EnableVertexAttribArray;
uint32_t _mesa_unmarshal_EnableVertexAttribArray(struct gl_context *ctx, const struct marshal_cmd_EnableVertexAttribArray *cmd);
struct marshal_cmd_GetActiveUniform;
uint32_t _mesa_unmarshal_GetActiveUniform(struct gl_context *ctx, const struct marshal_cmd_GetActiveUniform *cmd);
void GLAPIENTRY _mesa_marshal_GetActiveUniform(GLuint program, GLuint index, GLsizei bufSize, GLsizei * length, GLint * size, GLenum * type, GLchar * name);
struct marshal_cmd_GetUniformLocation;
uint32_t _mesa_unmarshal_GetUniformLocation(struct gl_context *ctx, const struct marshal_cmd_GetUniformLocation *cmd);
GLint GLAPIENTRY _mesa_marshal_GetUniformLocation(GLuint program, const GLchar * name);
struct marshal_cmd_LinkProgram;
uint32_t _mesa_unmarshal_LinkProgram(struct gl_context *ctx, const struct marshal_cmd_LinkProgram *cmd);
struct marshal_cmd_UseProgram;
uint32_t _mesa_unmarshal_UseProgram(struct gl_context *ctx, const struct marshal_cmd_UseProgram *cmd);
struct marshal_cmd_Uniform1f;
uint32_t _mesa_unmarshal_Uniform1f(struct gl_context *ctx, const struct marshal_cmd_Uniform1f *cmd);
struct marshal_cmd_Uniform2f;
uint32_t _mesa_unmarshal_Uniform2f(struct gl_context *ctx, const struct marshal_cmd_Uniform2f *cmd);
struct marshal_cmd_Uniform3f;
uint32_t _mesa_unmarshal_Uniform3f(struct gl_context *ctx, const struct marshal_cmd_Uniform3f *cmd);
struct marshal_cmd_Uniform4f;
uint32_t _mesa_unmarshal_Uniform4f(struct gl_context *ctx, const struct marshal_cmd_Uniform4f *cmd);
struct marshal_cmd_Uniform1i;
uint32_t _mesa_unmarshal_Uniform1i(struct gl_context *ctx, const struct marshal_cmd_Uniform1i *cmd);
struct marshal_cmd_Uniform2i;
uint32_t _mesa_unmarshal_Uniform2i(struct gl_context *ctx, const struct marshal_cmd_Uniform2i *cmd);
struct marshal_cmd_Uniform3i;
uint32_t _mesa_unmarshal_Uniform3i(struct gl_context *ctx, const struct marshal_cmd_Uniform3i *cmd);
struct marshal_cmd_Uniform4i;
uint32_t _mesa_unmarshal_Uniform4i(struct gl_context *ctx, const struct marshal_cmd_Uniform4i *cmd);
struct marshal_cmd_Uniform1fv;
uint32_t _mesa_unmarshal_Uniform1fv(struct gl_context *ctx, const struct marshal_cmd_Uniform1fv *cmd);
struct marshal_cmd_Uniform2fv;
uint32_t _mesa_unmarshal_Uniform2fv(struct gl_context *ctx, const struct marshal_cmd_Uniform2fv *cmd);
struct marshal_cmd_Uniform3fv;
uint32_t _mesa_unmarshal_Uniform3fv(struct gl_context *ctx, const struct marshal_cmd_Uniform3fv *cmd);
struct marshal_cmd_Uniform4fv;
uint32_t _mesa_unmarshal_Uniform4fv(struct gl_context *ctx, const struct marshal_cmd_Uniform4fv *cmd);
struct marshal_cmd_Uniform1iv;
uint32_t _mesa_unmarshal_Uniform1iv(struct gl_context *ctx, const struct marshal_cmd_Uniform1iv *cmd);
struct marshal_cmd_Uniform2iv;
uint32_t _mesa_unmarshal_Uniform2iv(struct gl_context *ctx, const struct marshal_cmd_Uniform2iv *cmd);
struct marshal_cmd_Uniform3iv;
uint32_t _mesa_unmarshal_Uniform3iv(struct gl_context *ctx, const struct marshal_cmd_Uniform3iv *cmd);
struct marshal_cmd_Uniform4iv;
uint32_t _mesa_unmarshal_Uniform4iv(struct gl_context *ctx, const struct marshal_cmd_Uniform4iv *cmd);
struct marshal_cmd_UniformMatrix2fv;
uint32_t _mesa_unmarshal_UniformMatrix2fv(struct gl_context *ctx, const struct marshal_cmd_UniformMatrix2fv *cmd);
struct marshal_cmd_UniformMatrix3fv;
uint32_t _mesa_unmarshal_UniformMatrix3fv(struct gl_context *ctx, const struct marshal_cmd_UniformMatrix3fv *cmd);
struct marshal_cmd_UniformMatrix4fv;
uint32_t _mesa_unmarshal_UniformMatrix4fv(struct gl_context *ctx, const struct marshal_cmd_UniformMatrix4fv *cmd);
struct marshal_cmd_ValidateProgram;
uint32_t _mesa_unmarshal_ValidateProgram(struct gl_context *ctx, const struct marshal_cmd_ValidateProgram *cmd);
struct marshal_cmd_VertexAttrib1d;
uint32_t _mesa_unmarshal_VertexAttrib1d(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib1d *cmd);
struct marshal_cmd_VertexAttrib1dv;
uint32_t _mesa_unmarshal_VertexAttrib1dv(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib1dv *cmd);
struct marshal_cmd_VertexAttrib1fARB;
uint32_t _mesa_unmarshal_VertexAttrib1fARB(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib1fARB *cmd);
struct marshal_cmd_VertexAttrib1fvARB;
uint32_t _mesa_unmarshal_VertexAttrib1fvARB(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib1fvARB *cmd);
struct marshal_cmd_VertexAttrib1s;
uint32_t _mesa_unmarshal_VertexAttrib1s(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib1s *cmd);
struct marshal_cmd_VertexAttrib1sv;
uint32_t _mesa_unmarshal_VertexAttrib1sv(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib1sv *cmd);
struct marshal_cmd_VertexAttrib2d;
uint32_t _mesa_unmarshal_VertexAttrib2d(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib2d *cmd);
struct marshal_cmd_VertexAttrib2dv;
uint32_t _mesa_unmarshal_VertexAttrib2dv(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib2dv *cmd);
struct marshal_cmd_VertexAttrib2fARB;
uint32_t _mesa_unmarshal_VertexAttrib2fARB(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib2fARB *cmd);
struct marshal_cmd_VertexAttrib2fvARB;
uint32_t _mesa_unmarshal_VertexAttrib2fvARB(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib2fvARB *cmd);
struct marshal_cmd_VertexAttrib2s;
uint32_t _mesa_unmarshal_VertexAttrib2s(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib2s *cmd);
struct marshal_cmd_VertexAttrib2sv;
uint32_t _mesa_unmarshal_VertexAttrib2sv(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib2sv *cmd);
struct marshal_cmd_VertexAttrib3d;
uint32_t _mesa_unmarshal_VertexAttrib3d(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib3d *cmd);
struct marshal_cmd_VertexAttrib3dv;
uint32_t _mesa_unmarshal_VertexAttrib3dv(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib3dv *cmd);
struct marshal_cmd_VertexAttrib3fARB;
uint32_t _mesa_unmarshal_VertexAttrib3fARB(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib3fARB *cmd);
struct marshal_cmd_VertexAttrib3fvARB;
uint32_t _mesa_unmarshal_VertexAttrib3fvARB(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib3fvARB *cmd);
struct marshal_cmd_VertexAttrib3s;
uint32_t _mesa_unmarshal_VertexAttrib3s(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib3s *cmd);
struct marshal_cmd_VertexAttrib3sv;
uint32_t _mesa_unmarshal_VertexAttrib3sv(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib3sv *cmd);
struct marshal_cmd_VertexAttrib4Nbv;
uint32_t _mesa_unmarshal_VertexAttrib4Nbv(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4Nbv *cmd);
struct marshal_cmd_VertexAttrib4Niv;
uint32_t _mesa_unmarshal_VertexAttrib4Niv(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4Niv *cmd);
struct marshal_cmd_VertexAttrib4Nsv;
uint32_t _mesa_unmarshal_VertexAttrib4Nsv(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4Nsv *cmd);
struct marshal_cmd_VertexAttrib4Nub;
uint32_t _mesa_unmarshal_VertexAttrib4Nub(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4Nub *cmd);
struct marshal_cmd_VertexAttrib4Nubv;
uint32_t _mesa_unmarshal_VertexAttrib4Nubv(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4Nubv *cmd);
struct marshal_cmd_VertexAttrib4Nuiv;
uint32_t _mesa_unmarshal_VertexAttrib4Nuiv(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4Nuiv *cmd);
struct marshal_cmd_VertexAttrib4Nusv;
uint32_t _mesa_unmarshal_VertexAttrib4Nusv(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4Nusv *cmd);
struct marshal_cmd_VertexAttrib4bv;
uint32_t _mesa_unmarshal_VertexAttrib4bv(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4bv *cmd);
struct marshal_cmd_VertexAttrib4d;
uint32_t _mesa_unmarshal_VertexAttrib4d(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4d *cmd);
struct marshal_cmd_VertexAttrib4dv;
uint32_t _mesa_unmarshal_VertexAttrib4dv(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4dv *cmd);
struct marshal_cmd_VertexAttrib4fARB;
uint32_t _mesa_unmarshal_VertexAttrib4fARB(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4fARB *cmd);
struct marshal_cmd_VertexAttrib4fvARB;
uint32_t _mesa_unmarshal_VertexAttrib4fvARB(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4fvARB *cmd);
struct marshal_cmd_VertexAttrib4iv;
uint32_t _mesa_unmarshal_VertexAttrib4iv(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4iv *cmd);
struct marshal_cmd_VertexAttrib4s;
uint32_t _mesa_unmarshal_VertexAttrib4s(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4s *cmd);
struct marshal_cmd_VertexAttrib4sv;
uint32_t _mesa_unmarshal_VertexAttrib4sv(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4sv *cmd);
struct marshal_cmd_VertexAttrib4ubv;
uint32_t _mesa_unmarshal_VertexAttrib4ubv(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4ubv *cmd);
struct marshal_cmd_VertexAttrib4uiv;
uint32_t _mesa_unmarshal_VertexAttrib4uiv(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4uiv *cmd);
struct marshal_cmd_VertexAttrib4usv;
uint32_t _mesa_unmarshal_VertexAttrib4usv(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4usv *cmd);
struct marshal_cmd_VertexAttribPointer;
uint32_t _mesa_unmarshal_VertexAttribPointer(struct gl_context *ctx, const struct marshal_cmd_VertexAttribPointer *cmd);
struct marshal_cmd_UniformMatrix2x3fv;
uint32_t _mesa_unmarshal_UniformMatrix2x3fv(struct gl_context *ctx, const struct marshal_cmd_UniformMatrix2x3fv *cmd);
struct marshal_cmd_UniformMatrix3x2fv;
uint32_t _mesa_unmarshal_UniformMatrix3x2fv(struct gl_context *ctx, const struct marshal_cmd_UniformMatrix3x2fv *cmd);
struct marshal_cmd_UniformMatrix2x4fv;
uint32_t _mesa_unmarshal_UniformMatrix2x4fv(struct gl_context *ctx, const struct marshal_cmd_UniformMatrix2x4fv *cmd);
struct marshal_cmd_UniformMatrix4x2fv;
uint32_t _mesa_unmarshal_UniformMatrix4x2fv(struct gl_context *ctx, const struct marshal_cmd_UniformMatrix4x2fv *cmd);
struct marshal_cmd_UniformMatrix3x4fv;
uint32_t _mesa_unmarshal_UniformMatrix3x4fv(struct gl_context *ctx, const struct marshal_cmd_UniformMatrix3x4fv *cmd);
struct marshal_cmd_UniformMatrix4x3fv;
uint32_t _mesa_unmarshal_UniformMatrix4x3fv(struct gl_context *ctx, const struct marshal_cmd_UniformMatrix4x3fv *cmd);
struct marshal_cmd_ProgramStringARB;
uint32_t _mesa_unmarshal_ProgramStringARB(struct gl_context *ctx, const struct marshal_cmd_ProgramStringARB *cmd);
struct marshal_cmd_BindProgramARB;
uint32_t _mesa_unmarshal_BindProgramARB(struct gl_context *ctx, const struct marshal_cmd_BindProgramARB *cmd);
struct marshal_cmd_DeleteProgramsARB;
uint32_t _mesa_unmarshal_DeleteProgramsARB(struct gl_context *ctx, const struct marshal_cmd_DeleteProgramsARB *cmd);
struct marshal_cmd_ProgramEnvParameter4dARB;
uint32_t _mesa_unmarshal_ProgramEnvParameter4dARB(struct gl_context *ctx, const struct marshal_cmd_ProgramEnvParameter4dARB *cmd);
struct marshal_cmd_ProgramEnvParameter4dvARB;
uint32_t _mesa_unmarshal_ProgramEnvParameter4dvARB(struct gl_context *ctx, const struct marshal_cmd_ProgramEnvParameter4dvARB *cmd);
struct marshal_cmd_ProgramEnvParameter4fARB;
uint32_t _mesa_unmarshal_ProgramEnvParameter4fARB(struct gl_context *ctx, const struct marshal_cmd_ProgramEnvParameter4fARB *cmd);
struct marshal_cmd_ProgramEnvParameter4fvARB;
uint32_t _mesa_unmarshal_ProgramEnvParameter4fvARB(struct gl_context *ctx, const struct marshal_cmd_ProgramEnvParameter4fvARB *cmd);
struct marshal_cmd_ProgramLocalParameter4dARB;
uint32_t _mesa_unmarshal_ProgramLocalParameter4dARB(struct gl_context *ctx, const struct marshal_cmd_ProgramLocalParameter4dARB *cmd);
struct marshal_cmd_ProgramLocalParameter4dvARB;
uint32_t _mesa_unmarshal_ProgramLocalParameter4dvARB(struct gl_context *ctx, const struct marshal_cmd_ProgramLocalParameter4dvARB *cmd);
struct marshal_cmd_ProgramLocalParameter4fARB;
uint32_t _mesa_unmarshal_ProgramLocalParameter4fARB(struct gl_context *ctx, const struct marshal_cmd_ProgramLocalParameter4fARB *cmd);
struct marshal_cmd_ProgramLocalParameter4fvARB;
uint32_t _mesa_unmarshal_ProgramLocalParameter4fvARB(struct gl_context *ctx, const struct marshal_cmd_ProgramLocalParameter4fvARB *cmd);
struct marshal_cmd_DeleteObjectARB;
uint32_t _mesa_unmarshal_DeleteObjectARB(struct gl_context *ctx, const struct marshal_cmd_DeleteObjectARB *cmd);
struct marshal_cmd_DetachObjectARB;
uint32_t _mesa_unmarshal_DetachObjectARB(struct gl_context *ctx, const struct marshal_cmd_DetachObjectARB *cmd);
struct marshal_cmd_AttachObjectARB;
uint32_t _mesa_unmarshal_AttachObjectARB(struct gl_context *ctx, const struct marshal_cmd_AttachObjectARB *cmd);
struct marshal_cmd_ClampColor;
uint32_t _mesa_unmarshal_ClampColor(struct gl_context *ctx, const struct marshal_cmd_ClampColor *cmd);
struct marshal_cmd_DrawArraysInstanced;
uint32_t _mesa_unmarshal_DrawArraysInstanced(struct gl_context *ctx, const struct marshal_cmd_DrawArraysInstanced *cmd);
void GLAPIENTRY _mesa_marshal_DrawArraysInstanced(GLenum mode, GLint first, GLsizei count, GLsizei primcount);
struct marshal_cmd_DrawElementsInstanced;
uint32_t _mesa_unmarshal_DrawElementsInstanced(struct gl_context *ctx, const struct marshal_cmd_DrawElementsInstanced *cmd);
void GLAPIENTRY _mesa_marshal_DrawElementsInstanced(GLenum mode, GLsizei count, GLenum type, const GLvoid * indices, GLsizei primcount);
struct marshal_cmd_BindRenderbuffer;
uint32_t _mesa_unmarshal_BindRenderbuffer(struct gl_context *ctx, const struct marshal_cmd_BindRenderbuffer *cmd);
struct marshal_cmd_DeleteRenderbuffers;
uint32_t _mesa_unmarshal_DeleteRenderbuffers(struct gl_context *ctx, const struct marshal_cmd_DeleteRenderbuffers *cmd);
struct marshal_cmd_RenderbufferStorage;
uint32_t _mesa_unmarshal_RenderbufferStorage(struct gl_context *ctx, const struct marshal_cmd_RenderbufferStorage *cmd);
struct marshal_cmd_RenderbufferStorageMultisample;
uint32_t _mesa_unmarshal_RenderbufferStorageMultisample(struct gl_context *ctx, const struct marshal_cmd_RenderbufferStorageMultisample *cmd);
struct marshal_cmd_BindFramebuffer;
uint32_t _mesa_unmarshal_BindFramebuffer(struct gl_context *ctx, const struct marshal_cmd_BindFramebuffer *cmd);
struct marshal_cmd_DeleteFramebuffers;
uint32_t _mesa_unmarshal_DeleteFramebuffers(struct gl_context *ctx, const struct marshal_cmd_DeleteFramebuffers *cmd);
struct marshal_cmd_FramebufferTexture1D;
uint32_t _mesa_unmarshal_FramebufferTexture1D(struct gl_context *ctx, const struct marshal_cmd_FramebufferTexture1D *cmd);
struct marshal_cmd_FramebufferTexture2D;
uint32_t _mesa_unmarshal_FramebufferTexture2D(struct gl_context *ctx, const struct marshal_cmd_FramebufferTexture2D *cmd);
struct marshal_cmd_FramebufferTexture3D;
uint32_t _mesa_unmarshal_FramebufferTexture3D(struct gl_context *ctx, const struct marshal_cmd_FramebufferTexture3D *cmd);
struct marshal_cmd_FramebufferTextureLayer;
uint32_t _mesa_unmarshal_FramebufferTextureLayer(struct gl_context *ctx, const struct marshal_cmd_FramebufferTextureLayer *cmd);
struct marshal_cmd_FramebufferRenderbuffer;
uint32_t _mesa_unmarshal_FramebufferRenderbuffer(struct gl_context *ctx, const struct marshal_cmd_FramebufferRenderbuffer *cmd);
struct marshal_cmd_BlitFramebuffer;
uint32_t _mesa_unmarshal_BlitFramebuffer(struct gl_context *ctx, const struct marshal_cmd_BlitFramebuffer *cmd);
struct marshal_cmd_GenerateMipmap;
uint32_t _mesa_unmarshal_GenerateMipmap(struct gl_context *ctx, const struct marshal_cmd_GenerateMipmap *cmd);
struct marshal_cmd_VertexAttribDivisor;
uint32_t _mesa_unmarshal_VertexAttribDivisor(struct gl_context *ctx, const struct marshal_cmd_VertexAttribDivisor *cmd);
struct marshal_cmd_VertexArrayVertexAttribDivisorEXT;
uint32_t _mesa_unmarshal_VertexArrayVertexAttribDivisorEXT(struct gl_context *ctx, const struct marshal_cmd_VertexArrayVertexAttribDivisorEXT *cmd);
struct marshal_cmd_FlushMappedBufferRange;
uint32_t _mesa_unmarshal_FlushMappedBufferRange(struct gl_context *ctx, const struct marshal_cmd_FlushMappedBufferRange *cmd);
struct marshal_cmd_TexBuffer;
uint32_t _mesa_unmarshal_TexBuffer(struct gl_context *ctx, const struct marshal_cmd_TexBuffer *cmd);
struct marshal_cmd_BindVertexArray;
uint32_t _mesa_unmarshal_BindVertexArray(struct gl_context *ctx, const struct marshal_cmd_BindVertexArray *cmd);
struct marshal_cmd_DeleteVertexArrays;
uint32_t _mesa_unmarshal_DeleteVertexArrays(struct gl_context *ctx, const struct marshal_cmd_DeleteVertexArrays *cmd);
struct marshal_cmd_UniformBlockBinding;
uint32_t _mesa_unmarshal_UniformBlockBinding(struct gl_context *ctx, const struct marshal_cmd_UniformBlockBinding *cmd);
struct marshal_cmd_CopyBufferSubData;
uint32_t _mesa_unmarshal_CopyBufferSubData(struct gl_context *ctx, const struct marshal_cmd_CopyBufferSubData *cmd);
struct marshal_cmd_DrawElementsBaseVertex;
uint32_t _mesa_unmarshal_DrawElementsBaseVertex(struct gl_context *ctx, const struct marshal_cmd_DrawElementsBaseVertex *cmd);
void GLAPIENTRY _mesa_marshal_DrawElementsBaseVertex(GLenum mode, GLsizei count, GLenum type, const GLvoid * indices, GLint basevertex);
struct marshal_cmd_DrawRangeElementsBaseVertex;
uint32_t _mesa_unmarshal_DrawRangeElementsBaseVertex(struct gl_context *ctx, const struct marshal_cmd_DrawRangeElementsBaseVertex *cmd);
void GLAPIENTRY _mesa_marshal_DrawRangeElementsBaseVertex(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const GLvoid * indices, GLint basevertex);
struct marshal_cmd_MultiDrawElementsBaseVertex;
uint32_t _mesa_unmarshal_MultiDrawElementsBaseVertex(struct gl_context *ctx, const struct marshal_cmd_MultiDrawElementsBaseVertex *cmd);
void GLAPIENTRY _mesa_marshal_MultiDrawElementsBaseVertex(GLenum mode, const GLsizei * count, GLenum type, const GLvoid * const * indices, GLsizei primcount, const GLint * basevertex);
struct marshal_cmd_DrawElementsInstancedBaseVertex;
uint32_t _mesa_unmarshal_DrawElementsInstancedBaseVertex(struct gl_context *ctx, const struct marshal_cmd_DrawElementsInstancedBaseVertex *cmd);
void GLAPIENTRY _mesa_marshal_DrawElementsInstancedBaseVertex(GLenum mode, GLsizei count, GLenum type, const GLvoid * indices, GLsizei primcount, GLint basevertex);
struct marshal_cmd_DeleteSync;
uint32_t _mesa_unmarshal_DeleteSync(struct gl_context *ctx, const struct marshal_cmd_DeleteSync *cmd);
struct marshal_cmd_WaitSync;
uint32_t _mesa_unmarshal_WaitSync(struct gl_context *ctx, const struct marshal_cmd_WaitSync *cmd);
struct marshal_cmd_TexImage2DMultisample;
uint32_t _mesa_unmarshal_TexImage2DMultisample(struct gl_context *ctx, const struct marshal_cmd_TexImage2DMultisample *cmd);
struct marshal_cmd_TexImage3DMultisample;
uint32_t _mesa_unmarshal_TexImage3DMultisample(struct gl_context *ctx, const struct marshal_cmd_TexImage3DMultisample *cmd);
struct marshal_cmd_SampleMaski;
uint32_t _mesa_unmarshal_SampleMaski(struct gl_context *ctx, const struct marshal_cmd_SampleMaski *cmd);
struct marshal_cmd_BlendEquationiARB;
uint32_t _mesa_unmarshal_BlendEquationiARB(struct gl_context *ctx, const struct marshal_cmd_BlendEquationiARB *cmd);
struct marshal_cmd_BlendEquationSeparateiARB;
uint32_t _mesa_unmarshal_BlendEquationSeparateiARB(struct gl_context *ctx, const struct marshal_cmd_BlendEquationSeparateiARB *cmd);
struct marshal_cmd_BlendFunciARB;
uint32_t _mesa_unmarshal_BlendFunciARB(struct gl_context *ctx, const struct marshal_cmd_BlendFunciARB *cmd);
struct marshal_cmd_BlendFuncSeparateiARB;
uint32_t _mesa_unmarshal_BlendFuncSeparateiARB(struct gl_context *ctx, const struct marshal_cmd_BlendFuncSeparateiARB *cmd);
struct marshal_cmd_MinSampleShading;
uint32_t _mesa_unmarshal_MinSampleShading(struct gl_context *ctx, const struct marshal_cmd_MinSampleShading *cmd);
struct marshal_cmd_NamedStringARB;
uint32_t _mesa_unmarshal_NamedStringARB(struct gl_context *ctx, const struct marshal_cmd_NamedStringARB *cmd);
struct marshal_cmd_DeleteNamedStringARB;
uint32_t _mesa_unmarshal_DeleteNamedStringARB(struct gl_context *ctx, const struct marshal_cmd_DeleteNamedStringARB *cmd);
struct marshal_cmd_BindFragDataLocationIndexed;
uint32_t _mesa_unmarshal_BindFragDataLocationIndexed(struct gl_context *ctx, const struct marshal_cmd_BindFragDataLocationIndexed *cmd);
struct marshal_cmd_DeleteSamplers;
uint32_t _mesa_unmarshal_DeleteSamplers(struct gl_context *ctx, const struct marshal_cmd_DeleteSamplers *cmd);
struct marshal_cmd_BindSampler;
uint32_t _mesa_unmarshal_BindSampler(struct gl_context *ctx, const struct marshal_cmd_BindSampler *cmd);
struct marshal_cmd_SamplerParameteri;
uint32_t _mesa_unmarshal_SamplerParameteri(struct gl_context *ctx, const struct marshal_cmd_SamplerParameteri *cmd);
struct marshal_cmd_SamplerParameterf;
uint32_t _mesa_unmarshal_SamplerParameterf(struct gl_context *ctx, const struct marshal_cmd_SamplerParameterf *cmd);
struct marshal_cmd_SamplerParameteriv;
uint32_t _mesa_unmarshal_SamplerParameteriv(struct gl_context *ctx, const struct marshal_cmd_SamplerParameteriv *cmd);
struct marshal_cmd_SamplerParameterfv;
uint32_t _mesa_unmarshal_SamplerParameterfv(struct gl_context *ctx, const struct marshal_cmd_SamplerParameterfv *cmd);
struct marshal_cmd_SamplerParameterIiv;
uint32_t _mesa_unmarshal_SamplerParameterIiv(struct gl_context *ctx, const struct marshal_cmd_SamplerParameterIiv *cmd);
struct marshal_cmd_SamplerParameterIuiv;
uint32_t _mesa_unmarshal_SamplerParameterIuiv(struct gl_context *ctx, const struct marshal_cmd_SamplerParameterIuiv *cmd);
struct marshal_cmd_QueryCounter;
uint32_t _mesa_unmarshal_QueryCounter(struct gl_context *ctx, const struct marshal_cmd_QueryCounter *cmd);
struct marshal_cmd_VertexP2ui;
uint32_t _mesa_unmarshal_VertexP2ui(struct gl_context *ctx, const struct marshal_cmd_VertexP2ui *cmd);
struct marshal_cmd_VertexP3ui;
uint32_t _mesa_unmarshal_VertexP3ui(struct gl_context *ctx, const struct marshal_cmd_VertexP3ui *cmd);
struct marshal_cmd_VertexP4ui;
uint32_t _mesa_unmarshal_VertexP4ui(struct gl_context *ctx, const struct marshal_cmd_VertexP4ui *cmd);
struct marshal_cmd_VertexP2uiv;
uint32_t _mesa_unmarshal_VertexP2uiv(struct gl_context *ctx, const struct marshal_cmd_VertexP2uiv *cmd);
struct marshal_cmd_VertexP3uiv;
uint32_t _mesa_unmarshal_VertexP3uiv(struct gl_context *ctx, const struct marshal_cmd_VertexP3uiv *cmd);
struct marshal_cmd_VertexP4uiv;
uint32_t _mesa_unmarshal_VertexP4uiv(struct gl_context *ctx, const struct marshal_cmd_VertexP4uiv *cmd);
struct marshal_cmd_TexCoordP1ui;
uint32_t _mesa_unmarshal_TexCoordP1ui(struct gl_context *ctx, const struct marshal_cmd_TexCoordP1ui *cmd);
struct marshal_cmd_TexCoordP2ui;
uint32_t _mesa_unmarshal_TexCoordP2ui(struct gl_context *ctx, const struct marshal_cmd_TexCoordP2ui *cmd);
struct marshal_cmd_TexCoordP3ui;
uint32_t _mesa_unmarshal_TexCoordP3ui(struct gl_context *ctx, const struct marshal_cmd_TexCoordP3ui *cmd);
struct marshal_cmd_TexCoordP4ui;
uint32_t _mesa_unmarshal_TexCoordP4ui(struct gl_context *ctx, const struct marshal_cmd_TexCoordP4ui *cmd);
struct marshal_cmd_TexCoordP1uiv;
uint32_t _mesa_unmarshal_TexCoordP1uiv(struct gl_context *ctx, const struct marshal_cmd_TexCoordP1uiv *cmd);
struct marshal_cmd_TexCoordP2uiv;
uint32_t _mesa_unmarshal_TexCoordP2uiv(struct gl_context *ctx, const struct marshal_cmd_TexCoordP2uiv *cmd);
struct marshal_cmd_TexCoordP3uiv;
uint32_t _mesa_unmarshal_TexCoordP3uiv(struct gl_context *ctx, const struct marshal_cmd_TexCoordP3uiv *cmd);
struct marshal_cmd_TexCoordP4uiv;
uint32_t _mesa_unmarshal_TexCoordP4uiv(struct gl_context *ctx, const struct marshal_cmd_TexCoordP4uiv *cmd);
struct marshal_cmd_MultiTexCoordP1ui;
uint32_t _mesa_unmarshal_MultiTexCoordP1ui(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoordP1ui *cmd);
struct marshal_cmd_MultiTexCoordP2ui;
uint32_t _mesa_unmarshal_MultiTexCoordP2ui(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoordP2ui *cmd);
struct marshal_cmd_MultiTexCoordP3ui;
uint32_t _mesa_unmarshal_MultiTexCoordP3ui(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoordP3ui *cmd);
struct marshal_cmd_MultiTexCoordP4ui;
uint32_t _mesa_unmarshal_MultiTexCoordP4ui(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoordP4ui *cmd);
struct marshal_cmd_MultiTexCoordP1uiv;
uint32_t _mesa_unmarshal_MultiTexCoordP1uiv(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoordP1uiv *cmd);
struct marshal_cmd_MultiTexCoordP2uiv;
uint32_t _mesa_unmarshal_MultiTexCoordP2uiv(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoordP2uiv *cmd);
struct marshal_cmd_MultiTexCoordP3uiv;
uint32_t _mesa_unmarshal_MultiTexCoordP3uiv(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoordP3uiv *cmd);
struct marshal_cmd_MultiTexCoordP4uiv;
uint32_t _mesa_unmarshal_MultiTexCoordP4uiv(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoordP4uiv *cmd);
struct marshal_cmd_NormalP3ui;
uint32_t _mesa_unmarshal_NormalP3ui(struct gl_context *ctx, const struct marshal_cmd_NormalP3ui *cmd);
struct marshal_cmd_NormalP3uiv;
uint32_t _mesa_unmarshal_NormalP3uiv(struct gl_context *ctx, const struct marshal_cmd_NormalP3uiv *cmd);
struct marshal_cmd_ColorP3ui;
uint32_t _mesa_unmarshal_ColorP3ui(struct gl_context *ctx, const struct marshal_cmd_ColorP3ui *cmd);
struct marshal_cmd_ColorP4ui;
uint32_t _mesa_unmarshal_ColorP4ui(struct gl_context *ctx, const struct marshal_cmd_ColorP4ui *cmd);
struct marshal_cmd_ColorP3uiv;
uint32_t _mesa_unmarshal_ColorP3uiv(struct gl_context *ctx, const struct marshal_cmd_ColorP3uiv *cmd);
struct marshal_cmd_ColorP4uiv;
uint32_t _mesa_unmarshal_ColorP4uiv(struct gl_context *ctx, const struct marshal_cmd_ColorP4uiv *cmd);
struct marshal_cmd_SecondaryColorP3ui;
uint32_t _mesa_unmarshal_SecondaryColorP3ui(struct gl_context *ctx, const struct marshal_cmd_SecondaryColorP3ui *cmd);
struct marshal_cmd_SecondaryColorP3uiv;
uint32_t _mesa_unmarshal_SecondaryColorP3uiv(struct gl_context *ctx, const struct marshal_cmd_SecondaryColorP3uiv *cmd);
struct marshal_cmd_VertexAttribP1ui;
uint32_t _mesa_unmarshal_VertexAttribP1ui(struct gl_context *ctx, const struct marshal_cmd_VertexAttribP1ui *cmd);
struct marshal_cmd_VertexAttribP2ui;
uint32_t _mesa_unmarshal_VertexAttribP2ui(struct gl_context *ctx, const struct marshal_cmd_VertexAttribP2ui *cmd);
struct marshal_cmd_VertexAttribP3ui;
uint32_t _mesa_unmarshal_VertexAttribP3ui(struct gl_context *ctx, const struct marshal_cmd_VertexAttribP3ui *cmd);
struct marshal_cmd_VertexAttribP4ui;
uint32_t _mesa_unmarshal_VertexAttribP4ui(struct gl_context *ctx, const struct marshal_cmd_VertexAttribP4ui *cmd);
struct marshal_cmd_VertexAttribP1uiv;
uint32_t _mesa_unmarshal_VertexAttribP1uiv(struct gl_context *ctx, const struct marshal_cmd_VertexAttribP1uiv *cmd);
struct marshal_cmd_VertexAttribP2uiv;
uint32_t _mesa_unmarshal_VertexAttribP2uiv(struct gl_context *ctx, const struct marshal_cmd_VertexAttribP2uiv *cmd);
struct marshal_cmd_VertexAttribP3uiv;
uint32_t _mesa_unmarshal_VertexAttribP3uiv(struct gl_context *ctx, const struct marshal_cmd_VertexAttribP3uiv *cmd);
struct marshal_cmd_VertexAttribP4uiv;
uint32_t _mesa_unmarshal_VertexAttribP4uiv(struct gl_context *ctx, const struct marshal_cmd_VertexAttribP4uiv *cmd);
struct marshal_cmd_UniformSubroutinesuiv;
uint32_t _mesa_unmarshal_UniformSubroutinesuiv(struct gl_context *ctx, const struct marshal_cmd_UniformSubroutinesuiv *cmd);
struct marshal_cmd_PatchParameteri;
uint32_t _mesa_unmarshal_PatchParameteri(struct gl_context *ctx, const struct marshal_cmd_PatchParameteri *cmd);
struct marshal_cmd_PatchParameterfv;
uint32_t _mesa_unmarshal_PatchParameterfv(struct gl_context *ctx, const struct marshal_cmd_PatchParameterfv *cmd);
struct marshal_cmd_DrawArraysIndirect;
uint32_t _mesa_unmarshal_DrawArraysIndirect(struct gl_context *ctx, const struct marshal_cmd_DrawArraysIndirect *cmd);
struct marshal_cmd_DrawElementsIndirect;
uint32_t _mesa_unmarshal_DrawElementsIndirect(struct gl_context *ctx, const struct marshal_cmd_DrawElementsIndirect *cmd);
struct marshal_cmd_MultiDrawArraysIndirect;
uint32_t _mesa_unmarshal_MultiDrawArraysIndirect(struct gl_context *ctx, const struct marshal_cmd_MultiDrawArraysIndirect *cmd);
struct marshal_cmd_MultiDrawElementsIndirect;
uint32_t _mesa_unmarshal_MultiDrawElementsIndirect(struct gl_context *ctx, const struct marshal_cmd_MultiDrawElementsIndirect *cmd);
struct marshal_cmd_Uniform1d;
uint32_t _mesa_unmarshal_Uniform1d(struct gl_context *ctx, const struct marshal_cmd_Uniform1d *cmd);
struct marshal_cmd_Uniform2d;
uint32_t _mesa_unmarshal_Uniform2d(struct gl_context *ctx, const struct marshal_cmd_Uniform2d *cmd);
struct marshal_cmd_Uniform3d;
uint32_t _mesa_unmarshal_Uniform3d(struct gl_context *ctx, const struct marshal_cmd_Uniform3d *cmd);
struct marshal_cmd_Uniform4d;
uint32_t _mesa_unmarshal_Uniform4d(struct gl_context *ctx, const struct marshal_cmd_Uniform4d *cmd);
struct marshal_cmd_Uniform1dv;
uint32_t _mesa_unmarshal_Uniform1dv(struct gl_context *ctx, const struct marshal_cmd_Uniform1dv *cmd);
struct marshal_cmd_Uniform2dv;
uint32_t _mesa_unmarshal_Uniform2dv(struct gl_context *ctx, const struct marshal_cmd_Uniform2dv *cmd);
struct marshal_cmd_Uniform3dv;
uint32_t _mesa_unmarshal_Uniform3dv(struct gl_context *ctx, const struct marshal_cmd_Uniform3dv *cmd);
struct marshal_cmd_Uniform4dv;
uint32_t _mesa_unmarshal_Uniform4dv(struct gl_context *ctx, const struct marshal_cmd_Uniform4dv *cmd);
struct marshal_cmd_UniformMatrix2dv;
uint32_t _mesa_unmarshal_UniformMatrix2dv(struct gl_context *ctx, const struct marshal_cmd_UniformMatrix2dv *cmd);
struct marshal_cmd_UniformMatrix3dv;
uint32_t _mesa_unmarshal_UniformMatrix3dv(struct gl_context *ctx, const struct marshal_cmd_UniformMatrix3dv *cmd);
struct marshal_cmd_UniformMatrix4dv;
uint32_t _mesa_unmarshal_UniformMatrix4dv(struct gl_context *ctx, const struct marshal_cmd_UniformMatrix4dv *cmd);
struct marshal_cmd_UniformMatrix2x3dv;
uint32_t _mesa_unmarshal_UniformMatrix2x3dv(struct gl_context *ctx, const struct marshal_cmd_UniformMatrix2x3dv *cmd);
struct marshal_cmd_UniformMatrix2x4dv;
uint32_t _mesa_unmarshal_UniformMatrix2x4dv(struct gl_context *ctx, const struct marshal_cmd_UniformMatrix2x4dv *cmd);
struct marshal_cmd_UniformMatrix3x2dv;
uint32_t _mesa_unmarshal_UniformMatrix3x2dv(struct gl_context *ctx, const struct marshal_cmd_UniformMatrix3x2dv *cmd);
struct marshal_cmd_UniformMatrix3x4dv;
uint32_t _mesa_unmarshal_UniformMatrix3x4dv(struct gl_context *ctx, const struct marshal_cmd_UniformMatrix3x4dv *cmd);
struct marshal_cmd_UniformMatrix4x2dv;
uint32_t _mesa_unmarshal_UniformMatrix4x2dv(struct gl_context *ctx, const struct marshal_cmd_UniformMatrix4x2dv *cmd);
struct marshal_cmd_UniformMatrix4x3dv;
uint32_t _mesa_unmarshal_UniformMatrix4x3dv(struct gl_context *ctx, const struct marshal_cmd_UniformMatrix4x3dv *cmd);
struct marshal_cmd_ProgramUniform1d;
uint32_t _mesa_unmarshal_ProgramUniform1d(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform1d *cmd);
struct marshal_cmd_ProgramUniform2d;
uint32_t _mesa_unmarshal_ProgramUniform2d(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform2d *cmd);
struct marshal_cmd_ProgramUniform3d;
uint32_t _mesa_unmarshal_ProgramUniform3d(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform3d *cmd);
struct marshal_cmd_ProgramUniform4d;
uint32_t _mesa_unmarshal_ProgramUniform4d(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform4d *cmd);
struct marshal_cmd_ProgramUniform1dv;
uint32_t _mesa_unmarshal_ProgramUniform1dv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform1dv *cmd);
struct marshal_cmd_ProgramUniform2dv;
uint32_t _mesa_unmarshal_ProgramUniform2dv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform2dv *cmd);
struct marshal_cmd_ProgramUniform3dv;
uint32_t _mesa_unmarshal_ProgramUniform3dv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform3dv *cmd);
struct marshal_cmd_ProgramUniform4dv;
uint32_t _mesa_unmarshal_ProgramUniform4dv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform4dv *cmd);
struct marshal_cmd_ProgramUniformMatrix2dv;
uint32_t _mesa_unmarshal_ProgramUniformMatrix2dv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniformMatrix2dv *cmd);
struct marshal_cmd_ProgramUniformMatrix3dv;
uint32_t _mesa_unmarshal_ProgramUniformMatrix3dv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniformMatrix3dv *cmd);
struct marshal_cmd_ProgramUniformMatrix4dv;
uint32_t _mesa_unmarshal_ProgramUniformMatrix4dv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniformMatrix4dv *cmd);
struct marshal_cmd_ProgramUniformMatrix2x3dv;
uint32_t _mesa_unmarshal_ProgramUniformMatrix2x3dv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniformMatrix2x3dv *cmd);
struct marshal_cmd_ProgramUniformMatrix2x4dv;
uint32_t _mesa_unmarshal_ProgramUniformMatrix2x4dv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniformMatrix2x4dv *cmd);
struct marshal_cmd_ProgramUniformMatrix3x2dv;
uint32_t _mesa_unmarshal_ProgramUniformMatrix3x2dv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniformMatrix3x2dv *cmd);
struct marshal_cmd_ProgramUniformMatrix3x4dv;
uint32_t _mesa_unmarshal_ProgramUniformMatrix3x4dv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniformMatrix3x4dv *cmd);
struct marshal_cmd_ProgramUniformMatrix4x2dv;
uint32_t _mesa_unmarshal_ProgramUniformMatrix4x2dv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniformMatrix4x2dv *cmd);
struct marshal_cmd_ProgramUniformMatrix4x3dv;
uint32_t _mesa_unmarshal_ProgramUniformMatrix4x3dv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniformMatrix4x3dv *cmd);
struct marshal_cmd_DrawTransformFeedbackStream;
uint32_t _mesa_unmarshal_DrawTransformFeedbackStream(struct gl_context *ctx, const struct marshal_cmd_DrawTransformFeedbackStream *cmd);
struct marshal_cmd_BeginQueryIndexed;
uint32_t _mesa_unmarshal_BeginQueryIndexed(struct gl_context *ctx, const struct marshal_cmd_BeginQueryIndexed *cmd);
struct marshal_cmd_EndQueryIndexed;
uint32_t _mesa_unmarshal_EndQueryIndexed(struct gl_context *ctx, const struct marshal_cmd_EndQueryIndexed *cmd);
struct marshal_cmd_UseProgramStages;
uint32_t _mesa_unmarshal_UseProgramStages(struct gl_context *ctx, const struct marshal_cmd_UseProgramStages *cmd);
struct marshal_cmd_ActiveShaderProgram;
uint32_t _mesa_unmarshal_ActiveShaderProgram(struct gl_context *ctx, const struct marshal_cmd_ActiveShaderProgram *cmd);
struct marshal_cmd_BindProgramPipeline;
uint32_t _mesa_unmarshal_BindProgramPipeline(struct gl_context *ctx, const struct marshal_cmd_BindProgramPipeline *cmd);
struct marshal_cmd_DeleteProgramPipelines;
uint32_t _mesa_unmarshal_DeleteProgramPipelines(struct gl_context *ctx, const struct marshal_cmd_DeleteProgramPipelines *cmd);
struct marshal_cmd_ProgramUniform1i;
uint32_t _mesa_unmarshal_ProgramUniform1i(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform1i *cmd);
struct marshal_cmd_ProgramUniform2i;
uint32_t _mesa_unmarshal_ProgramUniform2i(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform2i *cmd);
struct marshal_cmd_ProgramUniform3i;
uint32_t _mesa_unmarshal_ProgramUniform3i(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform3i *cmd);
struct marshal_cmd_ProgramUniform4i;
uint32_t _mesa_unmarshal_ProgramUniform4i(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform4i *cmd);
struct marshal_cmd_ProgramUniform1ui;
uint32_t _mesa_unmarshal_ProgramUniform1ui(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform1ui *cmd);
struct marshal_cmd_ProgramUniform2ui;
uint32_t _mesa_unmarshal_ProgramUniform2ui(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform2ui *cmd);
struct marshal_cmd_ProgramUniform3ui;
uint32_t _mesa_unmarshal_ProgramUniform3ui(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform3ui *cmd);
struct marshal_cmd_ProgramUniform4ui;
uint32_t _mesa_unmarshal_ProgramUniform4ui(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform4ui *cmd);
struct marshal_cmd_ProgramUniform1f;
uint32_t _mesa_unmarshal_ProgramUniform1f(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform1f *cmd);
struct marshal_cmd_ProgramUniform2f;
uint32_t _mesa_unmarshal_ProgramUniform2f(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform2f *cmd);
struct marshal_cmd_ProgramUniform3f;
uint32_t _mesa_unmarshal_ProgramUniform3f(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform3f *cmd);
struct marshal_cmd_ProgramUniform4f;
uint32_t _mesa_unmarshal_ProgramUniform4f(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform4f *cmd);
struct marshal_cmd_ProgramUniform1iv;
uint32_t _mesa_unmarshal_ProgramUniform1iv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform1iv *cmd);
struct marshal_cmd_ProgramUniform2iv;
uint32_t _mesa_unmarshal_ProgramUniform2iv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform2iv *cmd);
struct marshal_cmd_ProgramUniform3iv;
uint32_t _mesa_unmarshal_ProgramUniform3iv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform3iv *cmd);
struct marshal_cmd_ProgramUniform4iv;
uint32_t _mesa_unmarshal_ProgramUniform4iv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform4iv *cmd);
struct marshal_cmd_ProgramUniform1uiv;
uint32_t _mesa_unmarshal_ProgramUniform1uiv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform1uiv *cmd);
struct marshal_cmd_ProgramUniform2uiv;
uint32_t _mesa_unmarshal_ProgramUniform2uiv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform2uiv *cmd);
struct marshal_cmd_ProgramUniform3uiv;
uint32_t _mesa_unmarshal_ProgramUniform3uiv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform3uiv *cmd);
struct marshal_cmd_ProgramUniform4uiv;
uint32_t _mesa_unmarshal_ProgramUniform4uiv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform4uiv *cmd);
struct marshal_cmd_ProgramUniform1fv;
uint32_t _mesa_unmarshal_ProgramUniform1fv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform1fv *cmd);
struct marshal_cmd_ProgramUniform2fv;
uint32_t _mesa_unmarshal_ProgramUniform2fv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform2fv *cmd);
struct marshal_cmd_ProgramUniform3fv;
uint32_t _mesa_unmarshal_ProgramUniform3fv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform3fv *cmd);
struct marshal_cmd_ProgramUniform4fv;
uint32_t _mesa_unmarshal_ProgramUniform4fv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform4fv *cmd);
struct marshal_cmd_ProgramUniformMatrix2fv;
uint32_t _mesa_unmarshal_ProgramUniformMatrix2fv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniformMatrix2fv *cmd);
struct marshal_cmd_ProgramUniformMatrix3fv;
uint32_t _mesa_unmarshal_ProgramUniformMatrix3fv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniformMatrix3fv *cmd);
struct marshal_cmd_ProgramUniformMatrix4fv;
uint32_t _mesa_unmarshal_ProgramUniformMatrix4fv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniformMatrix4fv *cmd);
struct marshal_cmd_ProgramUniformMatrix2x3fv;
uint32_t _mesa_unmarshal_ProgramUniformMatrix2x3fv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniformMatrix2x3fv *cmd);
struct marshal_cmd_ProgramUniformMatrix3x2fv;
uint32_t _mesa_unmarshal_ProgramUniformMatrix3x2fv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniformMatrix3x2fv *cmd);
struct marshal_cmd_ProgramUniformMatrix2x4fv;
uint32_t _mesa_unmarshal_ProgramUniformMatrix2x4fv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniformMatrix2x4fv *cmd);
struct marshal_cmd_ProgramUniformMatrix4x2fv;
uint32_t _mesa_unmarshal_ProgramUniformMatrix4x2fv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniformMatrix4x2fv *cmd);
struct marshal_cmd_ProgramUniformMatrix3x4fv;
uint32_t _mesa_unmarshal_ProgramUniformMatrix3x4fv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniformMatrix3x4fv *cmd);
struct marshal_cmd_ProgramUniformMatrix4x3fv;
uint32_t _mesa_unmarshal_ProgramUniformMatrix4x3fv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniformMatrix4x3fv *cmd);
struct marshal_cmd_ValidateProgramPipeline;
uint32_t _mesa_unmarshal_ValidateProgramPipeline(struct gl_context *ctx, const struct marshal_cmd_ValidateProgramPipeline *cmd);
struct marshal_cmd_VertexAttribL1d;
uint32_t _mesa_unmarshal_VertexAttribL1d(struct gl_context *ctx, const struct marshal_cmd_VertexAttribL1d *cmd);
struct marshal_cmd_VertexAttribL2d;
uint32_t _mesa_unmarshal_VertexAttribL2d(struct gl_context *ctx, const struct marshal_cmd_VertexAttribL2d *cmd);
struct marshal_cmd_VertexAttribL3d;
uint32_t _mesa_unmarshal_VertexAttribL3d(struct gl_context *ctx, const struct marshal_cmd_VertexAttribL3d *cmd);
struct marshal_cmd_VertexAttribL4d;
uint32_t _mesa_unmarshal_VertexAttribL4d(struct gl_context *ctx, const struct marshal_cmd_VertexAttribL4d *cmd);
struct marshal_cmd_VertexAttribL1dv;
uint32_t _mesa_unmarshal_VertexAttribL1dv(struct gl_context *ctx, const struct marshal_cmd_VertexAttribL1dv *cmd);
struct marshal_cmd_VertexAttribL2dv;
uint32_t _mesa_unmarshal_VertexAttribL2dv(struct gl_context *ctx, const struct marshal_cmd_VertexAttribL2dv *cmd);
struct marshal_cmd_VertexAttribL3dv;
uint32_t _mesa_unmarshal_VertexAttribL3dv(struct gl_context *ctx, const struct marshal_cmd_VertexAttribL3dv *cmd);
struct marshal_cmd_VertexAttribL4dv;
uint32_t _mesa_unmarshal_VertexAttribL4dv(struct gl_context *ctx, const struct marshal_cmd_VertexAttribL4dv *cmd);
struct marshal_cmd_VertexAttribLPointer;
uint32_t _mesa_unmarshal_VertexAttribLPointer(struct gl_context *ctx, const struct marshal_cmd_VertexAttribLPointer *cmd);
struct marshal_cmd_VertexArrayVertexAttribLOffsetEXT;
uint32_t _mesa_unmarshal_VertexArrayVertexAttribLOffsetEXT(struct gl_context *ctx, const struct marshal_cmd_VertexArrayVertexAttribLOffsetEXT *cmd);
struct marshal_cmd_ReleaseShaderCompiler;
uint32_t _mesa_unmarshal_ReleaseShaderCompiler(struct gl_context *ctx, const struct marshal_cmd_ReleaseShaderCompiler *cmd);
struct marshal_cmd_ShaderBinary;
uint32_t _mesa_unmarshal_ShaderBinary(struct gl_context *ctx, const struct marshal_cmd_ShaderBinary *cmd);
struct marshal_cmd_ClearDepthf;
uint32_t _mesa_unmarshal_ClearDepthf(struct gl_context *ctx, const struct marshal_cmd_ClearDepthf *cmd);
struct marshal_cmd_DepthRangef;
uint32_t _mesa_unmarshal_DepthRangef(struct gl_context *ctx, const struct marshal_cmd_DepthRangef *cmd);
struct marshal_cmd_ProgramBinary;
uint32_t _mesa_unmarshal_ProgramBinary(struct gl_context *ctx, const struct marshal_cmd_ProgramBinary *cmd);
struct marshal_cmd_ProgramParameteri;
uint32_t _mesa_unmarshal_ProgramParameteri(struct gl_context *ctx, const struct marshal_cmd_ProgramParameteri *cmd);
struct marshal_cmd_DebugMessageControl;
uint32_t _mesa_unmarshal_DebugMessageControl(struct gl_context *ctx, const struct marshal_cmd_DebugMessageControl *cmd);
struct marshal_cmd_DebugMessageInsert;
uint32_t _mesa_unmarshal_DebugMessageInsert(struct gl_context *ctx, const struct marshal_cmd_DebugMessageInsert *cmd);
struct marshal_cmd_GetnPolygonStippleARB;
uint32_t _mesa_unmarshal_GetnPolygonStippleARB(struct gl_context *ctx, const struct marshal_cmd_GetnPolygonStippleARB *cmd);
struct marshal_cmd_GetnTexImageARB;
uint32_t _mesa_unmarshal_GetnTexImageARB(struct gl_context *ctx, const struct marshal_cmd_GetnTexImageARB *cmd);
struct marshal_cmd_ReadnPixelsARB;
uint32_t _mesa_unmarshal_ReadnPixelsARB(struct gl_context *ctx, const struct marshal_cmd_ReadnPixelsARB *cmd);
struct marshal_cmd_GetnCompressedTexImageARB;
uint32_t _mesa_unmarshal_GetnCompressedTexImageARB(struct gl_context *ctx, const struct marshal_cmd_GetnCompressedTexImageARB *cmd);
struct marshal_cmd_DrawArraysInstancedBaseInstance;
uint32_t _mesa_unmarshal_DrawArraysInstancedBaseInstance(struct gl_context *ctx, const struct marshal_cmd_DrawArraysInstancedBaseInstance *cmd);
void GLAPIENTRY _mesa_marshal_DrawArraysInstancedBaseInstance(GLenum mode, GLint first, GLsizei count, GLsizei primcount, GLuint baseinstance);
struct marshal_cmd_DrawElementsInstancedBaseInstance;
uint32_t _mesa_unmarshal_DrawElementsInstancedBaseInstance(struct gl_context *ctx, const struct marshal_cmd_DrawElementsInstancedBaseInstance *cmd);
void GLAPIENTRY _mesa_marshal_DrawElementsInstancedBaseInstance(GLenum mode, GLsizei count, GLenum type, const GLvoid * indices, GLsizei primcount, GLuint baseinstance);
struct marshal_cmd_DrawElementsInstancedBaseVertexBaseInstance;
uint32_t _mesa_unmarshal_DrawElementsInstancedBaseVertexBaseInstance(struct gl_context *ctx, const struct marshal_cmd_DrawElementsInstancedBaseVertexBaseInstance *cmd);
void GLAPIENTRY _mesa_marshal_DrawElementsInstancedBaseVertexBaseInstance(GLenum mode, GLsizei count, GLenum type, const GLvoid * indices, GLsizei primcount, GLint basevertex, GLuint baseinstance);
struct marshal_cmd_DrawTransformFeedbackInstanced;
uint32_t _mesa_unmarshal_DrawTransformFeedbackInstanced(struct gl_context *ctx, const struct marshal_cmd_DrawTransformFeedbackInstanced *cmd);
struct marshal_cmd_DrawTransformFeedbackStreamInstanced;
uint32_t _mesa_unmarshal_DrawTransformFeedbackStreamInstanced(struct gl_context *ctx, const struct marshal_cmd_DrawTransformFeedbackStreamInstanced *cmd);
struct marshal_cmd_BindImageTexture;
uint32_t _mesa_unmarshal_BindImageTexture(struct gl_context *ctx, const struct marshal_cmd_BindImageTexture *cmd);
struct marshal_cmd_MemoryBarrier;
uint32_t _mesa_unmarshal_MemoryBarrier(struct gl_context *ctx, const struct marshal_cmd_MemoryBarrier *cmd);
struct marshal_cmd_TexStorage1D;
uint32_t _mesa_unmarshal_TexStorage1D(struct gl_context *ctx, const struct marshal_cmd_TexStorage1D *cmd);
struct marshal_cmd_TexStorage2D;
uint32_t _mesa_unmarshal_TexStorage2D(struct gl_context *ctx, const struct marshal_cmd_TexStorage2D *cmd);
struct marshal_cmd_TexStorage3D;
uint32_t _mesa_unmarshal_TexStorage3D(struct gl_context *ctx, const struct marshal_cmd_TexStorage3D *cmd);
struct marshal_cmd_TextureStorage1DEXT;
uint32_t _mesa_unmarshal_TextureStorage1DEXT(struct gl_context *ctx, const struct marshal_cmd_TextureStorage1DEXT *cmd);
struct marshal_cmd_TextureStorage2DEXT;
uint32_t _mesa_unmarshal_TextureStorage2DEXT(struct gl_context *ctx, const struct marshal_cmd_TextureStorage2DEXT *cmd);
struct marshal_cmd_TextureStorage3DEXT;
uint32_t _mesa_unmarshal_TextureStorage3DEXT(struct gl_context *ctx, const struct marshal_cmd_TextureStorage3DEXT *cmd);
struct marshal_cmd_PushDebugGroup;
uint32_t _mesa_unmarshal_PushDebugGroup(struct gl_context *ctx, const struct marshal_cmd_PushDebugGroup *cmd);
struct marshal_cmd_PopDebugGroup;
uint32_t _mesa_unmarshal_PopDebugGroup(struct gl_context *ctx, const struct marshal_cmd_PopDebugGroup *cmd);
struct marshal_cmd_ObjectLabel;
uint32_t _mesa_unmarshal_ObjectLabel(struct gl_context *ctx, const struct marshal_cmd_ObjectLabel *cmd);
struct marshal_cmd_DispatchCompute;
uint32_t _mesa_unmarshal_DispatchCompute(struct gl_context *ctx, const struct marshal_cmd_DispatchCompute *cmd);
struct marshal_cmd_DispatchComputeIndirect;
uint32_t _mesa_unmarshal_DispatchComputeIndirect(struct gl_context *ctx, const struct marshal_cmd_DispatchComputeIndirect *cmd);
struct marshal_cmd_CopyImageSubData;
uint32_t _mesa_unmarshal_CopyImageSubData(struct gl_context *ctx, const struct marshal_cmd_CopyImageSubData *cmd);
struct marshal_cmd_TextureView;
uint32_t _mesa_unmarshal_TextureView(struct gl_context *ctx, const struct marshal_cmd_TextureView *cmd);
struct marshal_cmd_BindVertexBuffer;
uint32_t _mesa_unmarshal_BindVertexBuffer(struct gl_context *ctx, const struct marshal_cmd_BindVertexBuffer *cmd);
struct marshal_cmd_VertexAttribFormat;
uint32_t _mesa_unmarshal_VertexAttribFormat(struct gl_context *ctx, const struct marshal_cmd_VertexAttribFormat *cmd);
struct marshal_cmd_VertexAttribIFormat;
uint32_t _mesa_unmarshal_VertexAttribIFormat(struct gl_context *ctx, const struct marshal_cmd_VertexAttribIFormat *cmd);
struct marshal_cmd_VertexAttribLFormat;
uint32_t _mesa_unmarshal_VertexAttribLFormat(struct gl_context *ctx, const struct marshal_cmd_VertexAttribLFormat *cmd);
struct marshal_cmd_VertexAttribBinding;
uint32_t _mesa_unmarshal_VertexAttribBinding(struct gl_context *ctx, const struct marshal_cmd_VertexAttribBinding *cmd);
struct marshal_cmd_VertexBindingDivisor;
uint32_t _mesa_unmarshal_VertexBindingDivisor(struct gl_context *ctx, const struct marshal_cmd_VertexBindingDivisor *cmd);
struct marshal_cmd_VertexArrayBindVertexBufferEXT;
uint32_t _mesa_unmarshal_VertexArrayBindVertexBufferEXT(struct gl_context *ctx, const struct marshal_cmd_VertexArrayBindVertexBufferEXT *cmd);
struct marshal_cmd_VertexArrayVertexAttribFormatEXT;
uint32_t _mesa_unmarshal_VertexArrayVertexAttribFormatEXT(struct gl_context *ctx, const struct marshal_cmd_VertexArrayVertexAttribFormatEXT *cmd);
struct marshal_cmd_VertexArrayVertexAttribIFormatEXT;
uint32_t _mesa_unmarshal_VertexArrayVertexAttribIFormatEXT(struct gl_context *ctx, const struct marshal_cmd_VertexArrayVertexAttribIFormatEXT *cmd);
struct marshal_cmd_VertexArrayVertexAttribLFormatEXT;
uint32_t _mesa_unmarshal_VertexArrayVertexAttribLFormatEXT(struct gl_context *ctx, const struct marshal_cmd_VertexArrayVertexAttribLFormatEXT *cmd);
struct marshal_cmd_VertexArrayVertexAttribBindingEXT;
uint32_t _mesa_unmarshal_VertexArrayVertexAttribBindingEXT(struct gl_context *ctx, const struct marshal_cmd_VertexArrayVertexAttribBindingEXT *cmd);
struct marshal_cmd_VertexArrayVertexBindingDivisorEXT;
uint32_t _mesa_unmarshal_VertexArrayVertexBindingDivisorEXT(struct gl_context *ctx, const struct marshal_cmd_VertexArrayVertexBindingDivisorEXT *cmd);
struct marshal_cmd_FramebufferParameteri;
uint32_t _mesa_unmarshal_FramebufferParameteri(struct gl_context *ctx, const struct marshal_cmd_FramebufferParameteri *cmd);
struct marshal_cmd_NamedFramebufferParameteriEXT;
uint32_t _mesa_unmarshal_NamedFramebufferParameteriEXT(struct gl_context *ctx, const struct marshal_cmd_NamedFramebufferParameteriEXT *cmd);
struct marshal_cmd_InvalidateTexSubImage;
uint32_t _mesa_unmarshal_InvalidateTexSubImage(struct gl_context *ctx, const struct marshal_cmd_InvalidateTexSubImage *cmd);
struct marshal_cmd_InvalidateTexImage;
uint32_t _mesa_unmarshal_InvalidateTexImage(struct gl_context *ctx, const struct marshal_cmd_InvalidateTexImage *cmd);
struct marshal_cmd_InvalidateBufferSubData;
uint32_t _mesa_unmarshal_InvalidateBufferSubData(struct gl_context *ctx, const struct marshal_cmd_InvalidateBufferSubData *cmd);
struct marshal_cmd_InvalidateBufferData;
uint32_t _mesa_unmarshal_InvalidateBufferData(struct gl_context *ctx, const struct marshal_cmd_InvalidateBufferData *cmd);
struct marshal_cmd_InvalidateSubFramebuffer;
uint32_t _mesa_unmarshal_InvalidateSubFramebuffer(struct gl_context *ctx, const struct marshal_cmd_InvalidateSubFramebuffer *cmd);
struct marshal_cmd_InvalidateFramebuffer;
uint32_t _mesa_unmarshal_InvalidateFramebuffer(struct gl_context *ctx, const struct marshal_cmd_InvalidateFramebuffer *cmd);
struct marshal_cmd_ShaderStorageBlockBinding;
uint32_t _mesa_unmarshal_ShaderStorageBlockBinding(struct gl_context *ctx, const struct marshal_cmd_ShaderStorageBlockBinding *cmd);
struct marshal_cmd_TexBufferRange;
uint32_t _mesa_unmarshal_TexBufferRange(struct gl_context *ctx, const struct marshal_cmd_TexBufferRange *cmd);
struct marshal_cmd_TextureBufferRangeEXT;
uint32_t _mesa_unmarshal_TextureBufferRangeEXT(struct gl_context *ctx, const struct marshal_cmd_TextureBufferRangeEXT *cmd);
struct marshal_cmd_TexStorage2DMultisample;
uint32_t _mesa_unmarshal_TexStorage2DMultisample(struct gl_context *ctx, const struct marshal_cmd_TexStorage2DMultisample *cmd);
struct marshal_cmd_TexStorage3DMultisample;
uint32_t _mesa_unmarshal_TexStorage3DMultisample(struct gl_context *ctx, const struct marshal_cmd_TexStorage3DMultisample *cmd);
struct marshal_cmd_TextureStorage2DMultisampleEXT;
uint32_t _mesa_unmarshal_TextureStorage2DMultisampleEXT(struct gl_context *ctx, const struct marshal_cmd_TextureStorage2DMultisampleEXT *cmd);
struct marshal_cmd_TextureStorage3DMultisampleEXT;
uint32_t _mesa_unmarshal_TextureStorage3DMultisampleEXT(struct gl_context *ctx, const struct marshal_cmd_TextureStorage3DMultisampleEXT *cmd);
struct marshal_cmd_BindBuffersBase;
uint32_t _mesa_unmarshal_BindBuffersBase(struct gl_context *ctx, const struct marshal_cmd_BindBuffersBase *cmd);
struct marshal_cmd_BindBuffersRange;
uint32_t _mesa_unmarshal_BindBuffersRange(struct gl_context *ctx, const struct marshal_cmd_BindBuffersRange *cmd);
struct marshal_cmd_BindTextures;
uint32_t _mesa_unmarshal_BindTextures(struct gl_context *ctx, const struct marshal_cmd_BindTextures *cmd);
struct marshal_cmd_BindSamplers;
uint32_t _mesa_unmarshal_BindSamplers(struct gl_context *ctx, const struct marshal_cmd_BindSamplers *cmd);
struct marshal_cmd_BindImageTextures;
uint32_t _mesa_unmarshal_BindImageTextures(struct gl_context *ctx, const struct marshal_cmd_BindImageTextures *cmd);
struct marshal_cmd_BindVertexBuffers;
uint32_t _mesa_unmarshal_BindVertexBuffers(struct gl_context *ctx, const struct marshal_cmd_BindVertexBuffers *cmd);
struct marshal_cmd_MakeTextureHandleResidentARB;
uint32_t _mesa_unmarshal_MakeTextureHandleResidentARB(struct gl_context *ctx, const struct marshal_cmd_MakeTextureHandleResidentARB *cmd);
struct marshal_cmd_MakeTextureHandleNonResidentARB;
uint32_t _mesa_unmarshal_MakeTextureHandleNonResidentARB(struct gl_context *ctx, const struct marshal_cmd_MakeTextureHandleNonResidentARB *cmd);
struct marshal_cmd_MakeImageHandleResidentARB;
uint32_t _mesa_unmarshal_MakeImageHandleResidentARB(struct gl_context *ctx, const struct marshal_cmd_MakeImageHandleResidentARB *cmd);
struct marshal_cmd_MakeImageHandleNonResidentARB;
uint32_t _mesa_unmarshal_MakeImageHandleNonResidentARB(struct gl_context *ctx, const struct marshal_cmd_MakeImageHandleNonResidentARB *cmd);
struct marshal_cmd_UniformHandleui64ARB;
uint32_t _mesa_unmarshal_UniformHandleui64ARB(struct gl_context *ctx, const struct marshal_cmd_UniformHandleui64ARB *cmd);
struct marshal_cmd_UniformHandleui64vARB;
uint32_t _mesa_unmarshal_UniformHandleui64vARB(struct gl_context *ctx, const struct marshal_cmd_UniformHandleui64vARB *cmd);
struct marshal_cmd_ProgramUniformHandleui64ARB;
uint32_t _mesa_unmarshal_ProgramUniformHandleui64ARB(struct gl_context *ctx, const struct marshal_cmd_ProgramUniformHandleui64ARB *cmd);
struct marshal_cmd_ProgramUniformHandleui64vARB;
uint32_t _mesa_unmarshal_ProgramUniformHandleui64vARB(struct gl_context *ctx, const struct marshal_cmd_ProgramUniformHandleui64vARB *cmd);
struct marshal_cmd_VertexAttribL1ui64ARB;
uint32_t _mesa_unmarshal_VertexAttribL1ui64ARB(struct gl_context *ctx, const struct marshal_cmd_VertexAttribL1ui64ARB *cmd);
struct marshal_cmd_VertexAttribL1ui64vARB;
uint32_t _mesa_unmarshal_VertexAttribL1ui64vARB(struct gl_context *ctx, const struct marshal_cmd_VertexAttribL1ui64vARB *cmd);
struct marshal_cmd_DispatchComputeGroupSizeARB;
uint32_t _mesa_unmarshal_DispatchComputeGroupSizeARB(struct gl_context *ctx, const struct marshal_cmd_DispatchComputeGroupSizeARB *cmd);
struct marshal_cmd_MultiDrawArraysIndirectCountARB;
uint32_t _mesa_unmarshal_MultiDrawArraysIndirectCountARB(struct gl_context *ctx, const struct marshal_cmd_MultiDrawArraysIndirectCountARB *cmd);
struct marshal_cmd_MultiDrawElementsIndirectCountARB;
uint32_t _mesa_unmarshal_MultiDrawElementsIndirectCountARB(struct gl_context *ctx, const struct marshal_cmd_MultiDrawElementsIndirectCountARB *cmd);
struct marshal_cmd_TexPageCommitmentARB;
uint32_t _mesa_unmarshal_TexPageCommitmentARB(struct gl_context *ctx, const struct marshal_cmd_TexPageCommitmentARB *cmd);
struct marshal_cmd_TexturePageCommitmentEXT;
uint32_t _mesa_unmarshal_TexturePageCommitmentEXT(struct gl_context *ctx, const struct marshal_cmd_TexturePageCommitmentEXT *cmd);
struct marshal_cmd_ClipControl;
uint32_t _mesa_unmarshal_ClipControl(struct gl_context *ctx, const struct marshal_cmd_ClipControl *cmd);
struct marshal_cmd_TransformFeedbackBufferBase;
uint32_t _mesa_unmarshal_TransformFeedbackBufferBase(struct gl_context *ctx, const struct marshal_cmd_TransformFeedbackBufferBase *cmd);
struct marshal_cmd_TransformFeedbackBufferRange;
uint32_t _mesa_unmarshal_TransformFeedbackBufferRange(struct gl_context *ctx, const struct marshal_cmd_TransformFeedbackBufferRange *cmd);
struct marshal_cmd_NamedBufferData;
uint32_t _mesa_unmarshal_NamedBufferData(struct gl_context *ctx, const struct marshal_cmd_NamedBufferData *cmd);
void GLAPIENTRY _mesa_marshal_NamedBufferData(GLuint buffer, GLsizeiptr size, const GLvoid * data, GLenum usage);
struct marshal_cmd_NamedBufferSubData;
uint32_t _mesa_unmarshal_NamedBufferSubData(struct gl_context *ctx, const struct marshal_cmd_NamedBufferSubData *cmd);
void GLAPIENTRY _mesa_marshal_NamedBufferSubData(GLuint buffer, GLintptr offset, GLsizeiptr size, const GLvoid * data);
struct marshal_cmd_CopyNamedBufferSubData;
uint32_t _mesa_unmarshal_CopyNamedBufferSubData(struct gl_context *ctx, const struct marshal_cmd_CopyNamedBufferSubData *cmd);
struct marshal_cmd_UnmapNamedBufferEXT;
uint32_t _mesa_unmarshal_UnmapNamedBufferEXT(struct gl_context *ctx, const struct marshal_cmd_UnmapNamedBufferEXT *cmd);
struct marshal_cmd_FlushMappedNamedBufferRange;
uint32_t _mesa_unmarshal_FlushMappedNamedBufferRange(struct gl_context *ctx, const struct marshal_cmd_FlushMappedNamedBufferRange *cmd);
struct marshal_cmd_NamedFramebufferRenderbuffer;
uint32_t _mesa_unmarshal_NamedFramebufferRenderbuffer(struct gl_context *ctx, const struct marshal_cmd_NamedFramebufferRenderbuffer *cmd);
struct marshal_cmd_NamedFramebufferParameteri;
uint32_t _mesa_unmarshal_NamedFramebufferParameteri(struct gl_context *ctx, const struct marshal_cmd_NamedFramebufferParameteri *cmd);
struct marshal_cmd_NamedFramebufferTexture;
uint32_t _mesa_unmarshal_NamedFramebufferTexture(struct gl_context *ctx, const struct marshal_cmd_NamedFramebufferTexture *cmd);
struct marshal_cmd_NamedFramebufferTextureLayer;
uint32_t _mesa_unmarshal_NamedFramebufferTextureLayer(struct gl_context *ctx, const struct marshal_cmd_NamedFramebufferTextureLayer *cmd);
struct marshal_cmd_NamedFramebufferDrawBuffer;
uint32_t _mesa_unmarshal_NamedFramebufferDrawBuffer(struct gl_context *ctx, const struct marshal_cmd_NamedFramebufferDrawBuffer *cmd);
struct marshal_cmd_NamedFramebufferDrawBuffers;
uint32_t _mesa_unmarshal_NamedFramebufferDrawBuffers(struct gl_context *ctx, const struct marshal_cmd_NamedFramebufferDrawBuffers *cmd);
struct marshal_cmd_NamedFramebufferReadBuffer;
uint32_t _mesa_unmarshal_NamedFramebufferReadBuffer(struct gl_context *ctx, const struct marshal_cmd_NamedFramebufferReadBuffer *cmd);
struct marshal_cmd_InvalidateNamedFramebufferData;
uint32_t _mesa_unmarshal_InvalidateNamedFramebufferData(struct gl_context *ctx, const struct marshal_cmd_InvalidateNamedFramebufferData *cmd);
struct marshal_cmd_InvalidateNamedFramebufferSubData;
uint32_t _mesa_unmarshal_InvalidateNamedFramebufferSubData(struct gl_context *ctx, const struct marshal_cmd_InvalidateNamedFramebufferSubData *cmd);
struct marshal_cmd_ClearNamedFramebufferiv;
uint32_t _mesa_unmarshal_ClearNamedFramebufferiv(struct gl_context *ctx, const struct marshal_cmd_ClearNamedFramebufferiv *cmd);
struct marshal_cmd_ClearNamedFramebufferuiv;
uint32_t _mesa_unmarshal_ClearNamedFramebufferuiv(struct gl_context *ctx, const struct marshal_cmd_ClearNamedFramebufferuiv *cmd);
struct marshal_cmd_ClearNamedFramebufferfv;
uint32_t _mesa_unmarshal_ClearNamedFramebufferfv(struct gl_context *ctx, const struct marshal_cmd_ClearNamedFramebufferfv *cmd);
struct marshal_cmd_ClearNamedFramebufferfi;
uint32_t _mesa_unmarshal_ClearNamedFramebufferfi(struct gl_context *ctx, const struct marshal_cmd_ClearNamedFramebufferfi *cmd);
struct marshal_cmd_BlitNamedFramebuffer;
uint32_t _mesa_unmarshal_BlitNamedFramebuffer(struct gl_context *ctx, const struct marshal_cmd_BlitNamedFramebuffer *cmd);
struct marshal_cmd_NamedRenderbufferStorage;
uint32_t _mesa_unmarshal_NamedRenderbufferStorage(struct gl_context *ctx, const struct marshal_cmd_NamedRenderbufferStorage *cmd);
struct marshal_cmd_NamedRenderbufferStorageMultisample;
uint32_t _mesa_unmarshal_NamedRenderbufferStorageMultisample(struct gl_context *ctx, const struct marshal_cmd_NamedRenderbufferStorageMultisample *cmd);
struct marshal_cmd_TextureBuffer;
uint32_t _mesa_unmarshal_TextureBuffer(struct gl_context *ctx, const struct marshal_cmd_TextureBuffer *cmd);
struct marshal_cmd_TextureBufferRange;
uint32_t _mesa_unmarshal_TextureBufferRange(struct gl_context *ctx, const struct marshal_cmd_TextureBufferRange *cmd);
struct marshal_cmd_TextureStorage1D;
uint32_t _mesa_unmarshal_TextureStorage1D(struct gl_context *ctx, const struct marshal_cmd_TextureStorage1D *cmd);
struct marshal_cmd_TextureStorage2D;
uint32_t _mesa_unmarshal_TextureStorage2D(struct gl_context *ctx, const struct marshal_cmd_TextureStorage2D *cmd);
struct marshal_cmd_TextureStorage3D;
uint32_t _mesa_unmarshal_TextureStorage3D(struct gl_context *ctx, const struct marshal_cmd_TextureStorage3D *cmd);
struct marshal_cmd_TextureStorage2DMultisample;
uint32_t _mesa_unmarshal_TextureStorage2DMultisample(struct gl_context *ctx, const struct marshal_cmd_TextureStorage2DMultisample *cmd);
struct marshal_cmd_TextureStorage3DMultisample;
uint32_t _mesa_unmarshal_TextureStorage3DMultisample(struct gl_context *ctx, const struct marshal_cmd_TextureStorage3DMultisample *cmd);
struct marshal_cmd_TextureSubImage1D;
uint32_t _mesa_unmarshal_TextureSubImage1D(struct gl_context *ctx, const struct marshal_cmd_TextureSubImage1D *cmd);
struct marshal_cmd_TextureSubImage2D;
uint32_t _mesa_unmarshal_TextureSubImage2D(struct gl_context *ctx, const struct marshal_cmd_TextureSubImage2D *cmd);
struct marshal_cmd_TextureSubImage3D;
uint32_t _mesa_unmarshal_TextureSubImage3D(struct gl_context *ctx, const struct marshal_cmd_TextureSubImage3D *cmd);
struct marshal_cmd_CompressedTextureSubImage1D;
uint32_t _mesa_unmarshal_CompressedTextureSubImage1D(struct gl_context *ctx, const struct marshal_cmd_CompressedTextureSubImage1D *cmd);
struct marshal_cmd_CompressedTextureSubImage2D;
uint32_t _mesa_unmarshal_CompressedTextureSubImage2D(struct gl_context *ctx, const struct marshal_cmd_CompressedTextureSubImage2D *cmd);
struct marshal_cmd_CompressedTextureSubImage3D;
uint32_t _mesa_unmarshal_CompressedTextureSubImage3D(struct gl_context *ctx, const struct marshal_cmd_CompressedTextureSubImage3D *cmd);
struct marshal_cmd_CopyTextureSubImage1D;
uint32_t _mesa_unmarshal_CopyTextureSubImage1D(struct gl_context *ctx, const struct marshal_cmd_CopyTextureSubImage1D *cmd);
struct marshal_cmd_CopyTextureSubImage2D;
uint32_t _mesa_unmarshal_CopyTextureSubImage2D(struct gl_context *ctx, const struct marshal_cmd_CopyTextureSubImage2D *cmd);
struct marshal_cmd_CopyTextureSubImage3D;
uint32_t _mesa_unmarshal_CopyTextureSubImage3D(struct gl_context *ctx, const struct marshal_cmd_CopyTextureSubImage3D *cmd);
struct marshal_cmd_TextureParameterf;
uint32_t _mesa_unmarshal_TextureParameterf(struct gl_context *ctx, const struct marshal_cmd_TextureParameterf *cmd);
struct marshal_cmd_TextureParameterfv;
uint32_t _mesa_unmarshal_TextureParameterfv(struct gl_context *ctx, const struct marshal_cmd_TextureParameterfv *cmd);
struct marshal_cmd_TextureParameteri;
uint32_t _mesa_unmarshal_TextureParameteri(struct gl_context *ctx, const struct marshal_cmd_TextureParameteri *cmd);
struct marshal_cmd_TextureParameterIiv;
uint32_t _mesa_unmarshal_TextureParameterIiv(struct gl_context *ctx, const struct marshal_cmd_TextureParameterIiv *cmd);
struct marshal_cmd_TextureParameterIuiv;
uint32_t _mesa_unmarshal_TextureParameterIuiv(struct gl_context *ctx, const struct marshal_cmd_TextureParameterIuiv *cmd);
struct marshal_cmd_TextureParameteriv;
uint32_t _mesa_unmarshal_TextureParameteriv(struct gl_context *ctx, const struct marshal_cmd_TextureParameteriv *cmd);
struct marshal_cmd_GenerateTextureMipmap;
uint32_t _mesa_unmarshal_GenerateTextureMipmap(struct gl_context *ctx, const struct marshal_cmd_GenerateTextureMipmap *cmd);
struct marshal_cmd_BindTextureUnit;
uint32_t _mesa_unmarshal_BindTextureUnit(struct gl_context *ctx, const struct marshal_cmd_BindTextureUnit *cmd);
struct marshal_cmd_GetTextureImage;
uint32_t _mesa_unmarshal_GetTextureImage(struct gl_context *ctx, const struct marshal_cmd_GetTextureImage *cmd);
struct marshal_cmd_GetCompressedTextureImage;
uint32_t _mesa_unmarshal_GetCompressedTextureImage(struct gl_context *ctx, const struct marshal_cmd_GetCompressedTextureImage *cmd);
struct marshal_cmd_DisableVertexArrayAttrib;
uint32_t _mesa_unmarshal_DisableVertexArrayAttrib(struct gl_context *ctx, const struct marshal_cmd_DisableVertexArrayAttrib *cmd);
struct marshal_cmd_EnableVertexArrayAttrib;
uint32_t _mesa_unmarshal_EnableVertexArrayAttrib(struct gl_context *ctx, const struct marshal_cmd_EnableVertexArrayAttrib *cmd);
struct marshal_cmd_VertexArrayElementBuffer;
uint32_t _mesa_unmarshal_VertexArrayElementBuffer(struct gl_context *ctx, const struct marshal_cmd_VertexArrayElementBuffer *cmd);
struct marshal_cmd_VertexArrayVertexBuffer;
uint32_t _mesa_unmarshal_VertexArrayVertexBuffer(struct gl_context *ctx, const struct marshal_cmd_VertexArrayVertexBuffer *cmd);
struct marshal_cmd_VertexArrayVertexBuffers;
uint32_t _mesa_unmarshal_VertexArrayVertexBuffers(struct gl_context *ctx, const struct marshal_cmd_VertexArrayVertexBuffers *cmd);
struct marshal_cmd_VertexArrayAttribFormat;
uint32_t _mesa_unmarshal_VertexArrayAttribFormat(struct gl_context *ctx, const struct marshal_cmd_VertexArrayAttribFormat *cmd);
struct marshal_cmd_VertexArrayAttribIFormat;
uint32_t _mesa_unmarshal_VertexArrayAttribIFormat(struct gl_context *ctx, const struct marshal_cmd_VertexArrayAttribIFormat *cmd);
struct marshal_cmd_VertexArrayAttribLFormat;
uint32_t _mesa_unmarshal_VertexArrayAttribLFormat(struct gl_context *ctx, const struct marshal_cmd_VertexArrayAttribLFormat *cmd);
struct marshal_cmd_VertexArrayAttribBinding;
uint32_t _mesa_unmarshal_VertexArrayAttribBinding(struct gl_context *ctx, const struct marshal_cmd_VertexArrayAttribBinding *cmd);
struct marshal_cmd_VertexArrayBindingDivisor;
uint32_t _mesa_unmarshal_VertexArrayBindingDivisor(struct gl_context *ctx, const struct marshal_cmd_VertexArrayBindingDivisor *cmd);
struct marshal_cmd_GetQueryBufferObjectiv;
uint32_t _mesa_unmarshal_GetQueryBufferObjectiv(struct gl_context *ctx, const struct marshal_cmd_GetQueryBufferObjectiv *cmd);
struct marshal_cmd_GetQueryBufferObjectuiv;
uint32_t _mesa_unmarshal_GetQueryBufferObjectuiv(struct gl_context *ctx, const struct marshal_cmd_GetQueryBufferObjectuiv *cmd);
struct marshal_cmd_GetQueryBufferObjecti64v;
uint32_t _mesa_unmarshal_GetQueryBufferObjecti64v(struct gl_context *ctx, const struct marshal_cmd_GetQueryBufferObjecti64v *cmd);
struct marshal_cmd_GetQueryBufferObjectui64v;
uint32_t _mesa_unmarshal_GetQueryBufferObjectui64v(struct gl_context *ctx, const struct marshal_cmd_GetQueryBufferObjectui64v *cmd);
struct marshal_cmd_GetTextureSubImage;
uint32_t _mesa_unmarshal_GetTextureSubImage(struct gl_context *ctx, const struct marshal_cmd_GetTextureSubImage *cmd);
struct marshal_cmd_GetCompressedTextureSubImage;
uint32_t _mesa_unmarshal_GetCompressedTextureSubImage(struct gl_context *ctx, const struct marshal_cmd_GetCompressedTextureSubImage *cmd);
struct marshal_cmd_TextureBarrierNV;
uint32_t _mesa_unmarshal_TextureBarrierNV(struct gl_context *ctx, const struct marshal_cmd_TextureBarrierNV *cmd);
struct marshal_cmd_BufferPageCommitmentARB;
uint32_t _mesa_unmarshal_BufferPageCommitmentARB(struct gl_context *ctx, const struct marshal_cmd_BufferPageCommitmentARB *cmd);
struct marshal_cmd_NamedBufferPageCommitmentEXT;
uint32_t _mesa_unmarshal_NamedBufferPageCommitmentEXT(struct gl_context *ctx, const struct marshal_cmd_NamedBufferPageCommitmentEXT *cmd);
struct marshal_cmd_NamedBufferPageCommitmentARB;
uint32_t _mesa_unmarshal_NamedBufferPageCommitmentARB(struct gl_context *ctx, const struct marshal_cmd_NamedBufferPageCommitmentARB *cmd);
struct marshal_cmd_PrimitiveBoundingBox;
uint32_t _mesa_unmarshal_PrimitiveBoundingBox(struct gl_context *ctx, const struct marshal_cmd_PrimitiveBoundingBox *cmd);
struct marshal_cmd_BlendBarrier;
uint32_t _mesa_unmarshal_BlendBarrier(struct gl_context *ctx, const struct marshal_cmd_BlendBarrier *cmd);
struct marshal_cmd_Uniform1i64ARB;
uint32_t _mesa_unmarshal_Uniform1i64ARB(struct gl_context *ctx, const struct marshal_cmd_Uniform1i64ARB *cmd);
struct marshal_cmd_Uniform2i64ARB;
uint32_t _mesa_unmarshal_Uniform2i64ARB(struct gl_context *ctx, const struct marshal_cmd_Uniform2i64ARB *cmd);
struct marshal_cmd_Uniform3i64ARB;
uint32_t _mesa_unmarshal_Uniform3i64ARB(struct gl_context *ctx, const struct marshal_cmd_Uniform3i64ARB *cmd);
struct marshal_cmd_Uniform4i64ARB;
uint32_t _mesa_unmarshal_Uniform4i64ARB(struct gl_context *ctx, const struct marshal_cmd_Uniform4i64ARB *cmd);
struct marshal_cmd_Uniform1i64vARB;
uint32_t _mesa_unmarshal_Uniform1i64vARB(struct gl_context *ctx, const struct marshal_cmd_Uniform1i64vARB *cmd);
struct marshal_cmd_Uniform2i64vARB;
uint32_t _mesa_unmarshal_Uniform2i64vARB(struct gl_context *ctx, const struct marshal_cmd_Uniform2i64vARB *cmd);
struct marshal_cmd_Uniform3i64vARB;
uint32_t _mesa_unmarshal_Uniform3i64vARB(struct gl_context *ctx, const struct marshal_cmd_Uniform3i64vARB *cmd);
struct marshal_cmd_Uniform4i64vARB;
uint32_t _mesa_unmarshal_Uniform4i64vARB(struct gl_context *ctx, const struct marshal_cmd_Uniform4i64vARB *cmd);
struct marshal_cmd_Uniform1ui64ARB;
uint32_t _mesa_unmarshal_Uniform1ui64ARB(struct gl_context *ctx, const struct marshal_cmd_Uniform1ui64ARB *cmd);
struct marshal_cmd_Uniform2ui64ARB;
uint32_t _mesa_unmarshal_Uniform2ui64ARB(struct gl_context *ctx, const struct marshal_cmd_Uniform2ui64ARB *cmd);
struct marshal_cmd_Uniform3ui64ARB;
uint32_t _mesa_unmarshal_Uniform3ui64ARB(struct gl_context *ctx, const struct marshal_cmd_Uniform3ui64ARB *cmd);
struct marshal_cmd_Uniform4ui64ARB;
uint32_t _mesa_unmarshal_Uniform4ui64ARB(struct gl_context *ctx, const struct marshal_cmd_Uniform4ui64ARB *cmd);
struct marshal_cmd_Uniform1ui64vARB;
uint32_t _mesa_unmarshal_Uniform1ui64vARB(struct gl_context *ctx, const struct marshal_cmd_Uniform1ui64vARB *cmd);
struct marshal_cmd_Uniform2ui64vARB;
uint32_t _mesa_unmarshal_Uniform2ui64vARB(struct gl_context *ctx, const struct marshal_cmd_Uniform2ui64vARB *cmd);
struct marshal_cmd_Uniform3ui64vARB;
uint32_t _mesa_unmarshal_Uniform3ui64vARB(struct gl_context *ctx, const struct marshal_cmd_Uniform3ui64vARB *cmd);
struct marshal_cmd_Uniform4ui64vARB;
uint32_t _mesa_unmarshal_Uniform4ui64vARB(struct gl_context *ctx, const struct marshal_cmd_Uniform4ui64vARB *cmd);
struct marshal_cmd_ProgramUniform1i64ARB;
uint32_t _mesa_unmarshal_ProgramUniform1i64ARB(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform1i64ARB *cmd);
struct marshal_cmd_ProgramUniform2i64ARB;
uint32_t _mesa_unmarshal_ProgramUniform2i64ARB(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform2i64ARB *cmd);
struct marshal_cmd_ProgramUniform3i64ARB;
uint32_t _mesa_unmarshal_ProgramUniform3i64ARB(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform3i64ARB *cmd);
struct marshal_cmd_ProgramUniform4i64ARB;
uint32_t _mesa_unmarshal_ProgramUniform4i64ARB(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform4i64ARB *cmd);
struct marshal_cmd_ProgramUniform1i64vARB;
uint32_t _mesa_unmarshal_ProgramUniform1i64vARB(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform1i64vARB *cmd);
struct marshal_cmd_ProgramUniform2i64vARB;
uint32_t _mesa_unmarshal_ProgramUniform2i64vARB(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform2i64vARB *cmd);
struct marshal_cmd_ProgramUniform3i64vARB;
uint32_t _mesa_unmarshal_ProgramUniform3i64vARB(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform3i64vARB *cmd);
struct marshal_cmd_ProgramUniform4i64vARB;
uint32_t _mesa_unmarshal_ProgramUniform4i64vARB(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform4i64vARB *cmd);
struct marshal_cmd_ProgramUniform1ui64ARB;
uint32_t _mesa_unmarshal_ProgramUniform1ui64ARB(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform1ui64ARB *cmd);
struct marshal_cmd_ProgramUniform2ui64ARB;
uint32_t _mesa_unmarshal_ProgramUniform2ui64ARB(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform2ui64ARB *cmd);
struct marshal_cmd_ProgramUniform3ui64ARB;
uint32_t _mesa_unmarshal_ProgramUniform3ui64ARB(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform3ui64ARB *cmd);
struct marshal_cmd_ProgramUniform4ui64ARB;
uint32_t _mesa_unmarshal_ProgramUniform4ui64ARB(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform4ui64ARB *cmd);
struct marshal_cmd_ProgramUniform1ui64vARB;
uint32_t _mesa_unmarshal_ProgramUniform1ui64vARB(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform1ui64vARB *cmd);
struct marshal_cmd_ProgramUniform2ui64vARB;
uint32_t _mesa_unmarshal_ProgramUniform2ui64vARB(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform2ui64vARB *cmd);
struct marshal_cmd_ProgramUniform3ui64vARB;
uint32_t _mesa_unmarshal_ProgramUniform3ui64vARB(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform3ui64vARB *cmd);
struct marshal_cmd_ProgramUniform4ui64vARB;
uint32_t _mesa_unmarshal_ProgramUniform4ui64vARB(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform4ui64vARB *cmd);
struct marshal_cmd_MaxShaderCompilerThreadsKHR;
uint32_t _mesa_unmarshal_MaxShaderCompilerThreadsKHR(struct gl_context *ctx, const struct marshal_cmd_MaxShaderCompilerThreadsKHR *cmd);
struct marshal_cmd_SpecializeShaderARB;
uint32_t _mesa_unmarshal_SpecializeShaderARB(struct gl_context *ctx, const struct marshal_cmd_SpecializeShaderARB *cmd);
struct marshal_cmd_ColorPointerEXT;
uint32_t _mesa_unmarshal_ColorPointerEXT(struct gl_context *ctx, const struct marshal_cmd_ColorPointerEXT *cmd);
struct marshal_cmd_EdgeFlagPointerEXT;
uint32_t _mesa_unmarshal_EdgeFlagPointerEXT(struct gl_context *ctx, const struct marshal_cmd_EdgeFlagPointerEXT *cmd);
struct marshal_cmd_IndexPointerEXT;
uint32_t _mesa_unmarshal_IndexPointerEXT(struct gl_context *ctx, const struct marshal_cmd_IndexPointerEXT *cmd);
struct marshal_cmd_NormalPointerEXT;
uint32_t _mesa_unmarshal_NormalPointerEXT(struct gl_context *ctx, const struct marshal_cmd_NormalPointerEXT *cmd);
struct marshal_cmd_TexCoordPointerEXT;
uint32_t _mesa_unmarshal_TexCoordPointerEXT(struct gl_context *ctx, const struct marshal_cmd_TexCoordPointerEXT *cmd);
struct marshal_cmd_VertexPointerEXT;
uint32_t _mesa_unmarshal_VertexPointerEXT(struct gl_context *ctx, const struct marshal_cmd_VertexPointerEXT *cmd);
struct marshal_cmd_LockArraysEXT;
uint32_t _mesa_unmarshal_LockArraysEXT(struct gl_context *ctx, const struct marshal_cmd_LockArraysEXT *cmd);
struct marshal_cmd_UnlockArraysEXT;
uint32_t _mesa_unmarshal_UnlockArraysEXT(struct gl_context *ctx, const struct marshal_cmd_UnlockArraysEXT *cmd);
struct marshal_cmd_ViewportArrayv;
uint32_t _mesa_unmarshal_ViewportArrayv(struct gl_context *ctx, const struct marshal_cmd_ViewportArrayv *cmd);
struct marshal_cmd_ViewportIndexedf;
uint32_t _mesa_unmarshal_ViewportIndexedf(struct gl_context *ctx, const struct marshal_cmd_ViewportIndexedf *cmd);
struct marshal_cmd_ViewportIndexedfv;
uint32_t _mesa_unmarshal_ViewportIndexedfv(struct gl_context *ctx, const struct marshal_cmd_ViewportIndexedfv *cmd);
struct marshal_cmd_ScissorArrayv;
uint32_t _mesa_unmarshal_ScissorArrayv(struct gl_context *ctx, const struct marshal_cmd_ScissorArrayv *cmd);
struct marshal_cmd_ScissorIndexed;
uint32_t _mesa_unmarshal_ScissorIndexed(struct gl_context *ctx, const struct marshal_cmd_ScissorIndexed *cmd);
struct marshal_cmd_ScissorIndexedv;
uint32_t _mesa_unmarshal_ScissorIndexedv(struct gl_context *ctx, const struct marshal_cmd_ScissorIndexedv *cmd);
struct marshal_cmd_DepthRangeArrayv;
uint32_t _mesa_unmarshal_DepthRangeArrayv(struct gl_context *ctx, const struct marshal_cmd_DepthRangeArrayv *cmd);
struct marshal_cmd_DepthRangeIndexed;
uint32_t _mesa_unmarshal_DepthRangeIndexed(struct gl_context *ctx, const struct marshal_cmd_DepthRangeIndexed *cmd);
struct marshal_cmd_FramebufferSampleLocationsfvARB;
uint32_t _mesa_unmarshal_FramebufferSampleLocationsfvARB(struct gl_context *ctx, const struct marshal_cmd_FramebufferSampleLocationsfvARB *cmd);
struct marshal_cmd_NamedFramebufferSampleLocationsfvARB;
uint32_t _mesa_unmarshal_NamedFramebufferSampleLocationsfvARB(struct gl_context *ctx, const struct marshal_cmd_NamedFramebufferSampleLocationsfvARB *cmd);
struct marshal_cmd_EvaluateDepthValuesARB;
uint32_t _mesa_unmarshal_EvaluateDepthValuesARB(struct gl_context *ctx, const struct marshal_cmd_EvaluateDepthValuesARB *cmd);
struct marshal_cmd_WindowPos4dMESA;
uint32_t _mesa_unmarshal_WindowPos4dMESA(struct gl_context *ctx, const struct marshal_cmd_WindowPos4dMESA *cmd);
struct marshal_cmd_WindowPos4dvMESA;
uint32_t _mesa_unmarshal_WindowPos4dvMESA(struct gl_context *ctx, const struct marshal_cmd_WindowPos4dvMESA *cmd);
struct marshal_cmd_WindowPos4fMESA;
uint32_t _mesa_unmarshal_WindowPos4fMESA(struct gl_context *ctx, const struct marshal_cmd_WindowPos4fMESA *cmd);
struct marshal_cmd_WindowPos4fvMESA;
uint32_t _mesa_unmarshal_WindowPos4fvMESA(struct gl_context *ctx, const struct marshal_cmd_WindowPos4fvMESA *cmd);
struct marshal_cmd_WindowPos4iMESA;
uint32_t _mesa_unmarshal_WindowPos4iMESA(struct gl_context *ctx, const struct marshal_cmd_WindowPos4iMESA *cmd);
struct marshal_cmd_WindowPos4ivMESA;
uint32_t _mesa_unmarshal_WindowPos4ivMESA(struct gl_context *ctx, const struct marshal_cmd_WindowPos4ivMESA *cmd);
struct marshal_cmd_WindowPos4sMESA;
uint32_t _mesa_unmarshal_WindowPos4sMESA(struct gl_context *ctx, const struct marshal_cmd_WindowPos4sMESA *cmd);
struct marshal_cmd_WindowPos4svMESA;
uint32_t _mesa_unmarshal_WindowPos4svMESA(struct gl_context *ctx, const struct marshal_cmd_WindowPos4svMESA *cmd);
struct marshal_cmd_MultiModeDrawArraysIBM;
uint32_t _mesa_unmarshal_MultiModeDrawArraysIBM(struct gl_context *ctx, const struct marshal_cmd_MultiModeDrawArraysIBM *cmd);
struct marshal_cmd_MultiModeDrawElementsIBM;
uint32_t _mesa_unmarshal_MultiModeDrawElementsIBM(struct gl_context *ctx, const struct marshal_cmd_MultiModeDrawElementsIBM *cmd);
struct marshal_cmd_VertexAttrib1sNV;
uint32_t _mesa_unmarshal_VertexAttrib1sNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib1sNV *cmd);
struct marshal_cmd_VertexAttrib1svNV;
uint32_t _mesa_unmarshal_VertexAttrib1svNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib1svNV *cmd);
struct marshal_cmd_VertexAttrib2sNV;
uint32_t _mesa_unmarshal_VertexAttrib2sNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib2sNV *cmd);
struct marshal_cmd_VertexAttrib2svNV;
uint32_t _mesa_unmarshal_VertexAttrib2svNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib2svNV *cmd);
struct marshal_cmd_VertexAttrib3sNV;
uint32_t _mesa_unmarshal_VertexAttrib3sNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib3sNV *cmd);
struct marshal_cmd_VertexAttrib3svNV;
uint32_t _mesa_unmarshal_VertexAttrib3svNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib3svNV *cmd);
struct marshal_cmd_VertexAttrib4sNV;
uint32_t _mesa_unmarshal_VertexAttrib4sNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4sNV *cmd);
struct marshal_cmd_VertexAttrib4svNV;
uint32_t _mesa_unmarshal_VertexAttrib4svNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4svNV *cmd);
struct marshal_cmd_VertexAttrib1fNV;
uint32_t _mesa_unmarshal_VertexAttrib1fNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib1fNV *cmd);
struct marshal_cmd_VertexAttrib1fvNV;
uint32_t _mesa_unmarshal_VertexAttrib1fvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib1fvNV *cmd);
struct marshal_cmd_VertexAttrib2fNV;
uint32_t _mesa_unmarshal_VertexAttrib2fNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib2fNV *cmd);
struct marshal_cmd_VertexAttrib2fvNV;
uint32_t _mesa_unmarshal_VertexAttrib2fvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib2fvNV *cmd);
struct marshal_cmd_VertexAttrib3fNV;
uint32_t _mesa_unmarshal_VertexAttrib3fNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib3fNV *cmd);
struct marshal_cmd_VertexAttrib3fvNV;
uint32_t _mesa_unmarshal_VertexAttrib3fvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib3fvNV *cmd);
struct marshal_cmd_VertexAttrib4fNV;
uint32_t _mesa_unmarshal_VertexAttrib4fNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4fNV *cmd);
struct marshal_cmd_VertexAttrib4fvNV;
uint32_t _mesa_unmarshal_VertexAttrib4fvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4fvNV *cmd);
struct marshal_cmd_VertexAttrib1dNV;
uint32_t _mesa_unmarshal_VertexAttrib1dNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib1dNV *cmd);
struct marshal_cmd_VertexAttrib1dvNV;
uint32_t _mesa_unmarshal_VertexAttrib1dvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib1dvNV *cmd);
struct marshal_cmd_VertexAttrib2dNV;
uint32_t _mesa_unmarshal_VertexAttrib2dNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib2dNV *cmd);
struct marshal_cmd_VertexAttrib2dvNV;
uint32_t _mesa_unmarshal_VertexAttrib2dvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib2dvNV *cmd);
struct marshal_cmd_VertexAttrib3dNV;
uint32_t _mesa_unmarshal_VertexAttrib3dNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib3dNV *cmd);
struct marshal_cmd_VertexAttrib3dvNV;
uint32_t _mesa_unmarshal_VertexAttrib3dvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib3dvNV *cmd);
struct marshal_cmd_VertexAttrib4dNV;
uint32_t _mesa_unmarshal_VertexAttrib4dNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4dNV *cmd);
struct marshal_cmd_VertexAttrib4dvNV;
uint32_t _mesa_unmarshal_VertexAttrib4dvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4dvNV *cmd);
struct marshal_cmd_VertexAttrib4ubNV;
uint32_t _mesa_unmarshal_VertexAttrib4ubNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4ubNV *cmd);
struct marshal_cmd_VertexAttrib4ubvNV;
uint32_t _mesa_unmarshal_VertexAttrib4ubvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4ubvNV *cmd);
struct marshal_cmd_VertexAttribs1svNV;
uint32_t _mesa_unmarshal_VertexAttribs1svNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttribs1svNV *cmd);
struct marshal_cmd_VertexAttribs2svNV;
uint32_t _mesa_unmarshal_VertexAttribs2svNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttribs2svNV *cmd);
struct marshal_cmd_VertexAttribs3svNV;
uint32_t _mesa_unmarshal_VertexAttribs3svNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttribs3svNV *cmd);
struct marshal_cmd_VertexAttribs4svNV;
uint32_t _mesa_unmarshal_VertexAttribs4svNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttribs4svNV *cmd);
struct marshal_cmd_VertexAttribs1fvNV;
uint32_t _mesa_unmarshal_VertexAttribs1fvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttribs1fvNV *cmd);
struct marshal_cmd_VertexAttribs2fvNV;
uint32_t _mesa_unmarshal_VertexAttribs2fvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttribs2fvNV *cmd);
struct marshal_cmd_VertexAttribs3fvNV;
uint32_t _mesa_unmarshal_VertexAttribs3fvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttribs3fvNV *cmd);
struct marshal_cmd_VertexAttribs4fvNV;
uint32_t _mesa_unmarshal_VertexAttribs4fvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttribs4fvNV *cmd);
struct marshal_cmd_VertexAttribs1dvNV;
uint32_t _mesa_unmarshal_VertexAttribs1dvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttribs1dvNV *cmd);
struct marshal_cmd_VertexAttribs2dvNV;
uint32_t _mesa_unmarshal_VertexAttribs2dvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttribs2dvNV *cmd);
struct marshal_cmd_VertexAttribs3dvNV;
uint32_t _mesa_unmarshal_VertexAttribs3dvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttribs3dvNV *cmd);
struct marshal_cmd_VertexAttribs4dvNV;
uint32_t _mesa_unmarshal_VertexAttribs4dvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttribs4dvNV *cmd);
struct marshal_cmd_VertexAttribs4ubvNV;
uint32_t _mesa_unmarshal_VertexAttribs4ubvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttribs4ubvNV *cmd);
struct marshal_cmd_BindFragmentShaderATI;
uint32_t _mesa_unmarshal_BindFragmentShaderATI(struct gl_context *ctx, const struct marshal_cmd_BindFragmentShaderATI *cmd);
struct marshal_cmd_DeleteFragmentShaderATI;
uint32_t _mesa_unmarshal_DeleteFragmentShaderATI(struct gl_context *ctx, const struct marshal_cmd_DeleteFragmentShaderATI *cmd);
struct marshal_cmd_BeginFragmentShaderATI;
uint32_t _mesa_unmarshal_BeginFragmentShaderATI(struct gl_context *ctx, const struct marshal_cmd_BeginFragmentShaderATI *cmd);
struct marshal_cmd_EndFragmentShaderATI;
uint32_t _mesa_unmarshal_EndFragmentShaderATI(struct gl_context *ctx, const struct marshal_cmd_EndFragmentShaderATI *cmd);
struct marshal_cmd_PassTexCoordATI;
uint32_t _mesa_unmarshal_PassTexCoordATI(struct gl_context *ctx, const struct marshal_cmd_PassTexCoordATI *cmd);
struct marshal_cmd_SampleMapATI;
uint32_t _mesa_unmarshal_SampleMapATI(struct gl_context *ctx, const struct marshal_cmd_SampleMapATI *cmd);
struct marshal_cmd_ColorFragmentOp1ATI;
uint32_t _mesa_unmarshal_ColorFragmentOp1ATI(struct gl_context *ctx, const struct marshal_cmd_ColorFragmentOp1ATI *cmd);
struct marshal_cmd_ColorFragmentOp2ATI;
uint32_t _mesa_unmarshal_ColorFragmentOp2ATI(struct gl_context *ctx, const struct marshal_cmd_ColorFragmentOp2ATI *cmd);
struct marshal_cmd_ColorFragmentOp3ATI;
uint32_t _mesa_unmarshal_ColorFragmentOp3ATI(struct gl_context *ctx, const struct marshal_cmd_ColorFragmentOp3ATI *cmd);
struct marshal_cmd_AlphaFragmentOp1ATI;
uint32_t _mesa_unmarshal_AlphaFragmentOp1ATI(struct gl_context *ctx, const struct marshal_cmd_AlphaFragmentOp1ATI *cmd);
struct marshal_cmd_AlphaFragmentOp2ATI;
uint32_t _mesa_unmarshal_AlphaFragmentOp2ATI(struct gl_context *ctx, const struct marshal_cmd_AlphaFragmentOp2ATI *cmd);
struct marshal_cmd_AlphaFragmentOp3ATI;
uint32_t _mesa_unmarshal_AlphaFragmentOp3ATI(struct gl_context *ctx, const struct marshal_cmd_AlphaFragmentOp3ATI *cmd);
struct marshal_cmd_SetFragmentShaderConstantATI;
uint32_t _mesa_unmarshal_SetFragmentShaderConstantATI(struct gl_context *ctx, const struct marshal_cmd_SetFragmentShaderConstantATI *cmd);
struct marshal_cmd_ActiveStencilFaceEXT;
uint32_t _mesa_unmarshal_ActiveStencilFaceEXT(struct gl_context *ctx, const struct marshal_cmd_ActiveStencilFaceEXT *cmd);
struct marshal_cmd_DepthBoundsEXT;
uint32_t _mesa_unmarshal_DepthBoundsEXT(struct gl_context *ctx, const struct marshal_cmd_DepthBoundsEXT *cmd);
struct marshal_cmd_BindRenderbufferEXT;
uint32_t _mesa_unmarshal_BindRenderbufferEXT(struct gl_context *ctx, const struct marshal_cmd_BindRenderbufferEXT *cmd);
struct marshal_cmd_BindFramebufferEXT;
uint32_t _mesa_unmarshal_BindFramebufferEXT(struct gl_context *ctx, const struct marshal_cmd_BindFramebufferEXT *cmd);
struct marshal_cmd_ProvokingVertex;
uint32_t _mesa_unmarshal_ProvokingVertex(struct gl_context *ctx, const struct marshal_cmd_ProvokingVertex *cmd);
struct marshal_cmd_ColorMaski;
uint32_t _mesa_unmarshal_ColorMaski(struct gl_context *ctx, const struct marshal_cmd_ColorMaski *cmd);
struct marshal_cmd_Enablei;
uint32_t _mesa_unmarshal_Enablei(struct gl_context *ctx, const struct marshal_cmd_Enablei *cmd);
struct marshal_cmd_Disablei;
uint32_t _mesa_unmarshal_Disablei(struct gl_context *ctx, const struct marshal_cmd_Disablei *cmd);
struct marshal_cmd_DeletePerfMonitorsAMD;
uint32_t _mesa_unmarshal_DeletePerfMonitorsAMD(struct gl_context *ctx, const struct marshal_cmd_DeletePerfMonitorsAMD *cmd);
struct marshal_cmd_SelectPerfMonitorCountersAMD;
uint32_t _mesa_unmarshal_SelectPerfMonitorCountersAMD(struct gl_context *ctx, const struct marshal_cmd_SelectPerfMonitorCountersAMD *cmd);
struct marshal_cmd_BeginPerfMonitorAMD;
uint32_t _mesa_unmarshal_BeginPerfMonitorAMD(struct gl_context *ctx, const struct marshal_cmd_BeginPerfMonitorAMD *cmd);
struct marshal_cmd_EndPerfMonitorAMD;
uint32_t _mesa_unmarshal_EndPerfMonitorAMD(struct gl_context *ctx, const struct marshal_cmd_EndPerfMonitorAMD *cmd);
struct marshal_cmd_CopyImageSubDataNV;
uint32_t _mesa_unmarshal_CopyImageSubDataNV(struct gl_context *ctx, const struct marshal_cmd_CopyImageSubDataNV *cmd);
struct marshal_cmd_MatrixLoadfEXT;
uint32_t _mesa_unmarshal_MatrixLoadfEXT(struct gl_context *ctx, const struct marshal_cmd_MatrixLoadfEXT *cmd);
struct marshal_cmd_MatrixLoaddEXT;
uint32_t _mesa_unmarshal_MatrixLoaddEXT(struct gl_context *ctx, const struct marshal_cmd_MatrixLoaddEXT *cmd);
struct marshal_cmd_MatrixMultfEXT;
uint32_t _mesa_unmarshal_MatrixMultfEXT(struct gl_context *ctx, const struct marshal_cmd_MatrixMultfEXT *cmd);
struct marshal_cmd_MatrixMultdEXT;
uint32_t _mesa_unmarshal_MatrixMultdEXT(struct gl_context *ctx, const struct marshal_cmd_MatrixMultdEXT *cmd);
struct marshal_cmd_MatrixLoadIdentityEXT;
uint32_t _mesa_unmarshal_MatrixLoadIdentityEXT(struct gl_context *ctx, const struct marshal_cmd_MatrixLoadIdentityEXT *cmd);
struct marshal_cmd_MatrixRotatefEXT;
uint32_t _mesa_unmarshal_MatrixRotatefEXT(struct gl_context *ctx, const struct marshal_cmd_MatrixRotatefEXT *cmd);
struct marshal_cmd_MatrixRotatedEXT;
uint32_t _mesa_unmarshal_MatrixRotatedEXT(struct gl_context *ctx, const struct marshal_cmd_MatrixRotatedEXT *cmd);
struct marshal_cmd_MatrixScalefEXT;
uint32_t _mesa_unmarshal_MatrixScalefEXT(struct gl_context *ctx, const struct marshal_cmd_MatrixScalefEXT *cmd);
struct marshal_cmd_MatrixScaledEXT;
uint32_t _mesa_unmarshal_MatrixScaledEXT(struct gl_context *ctx, const struct marshal_cmd_MatrixScaledEXT *cmd);
struct marshal_cmd_MatrixTranslatefEXT;
uint32_t _mesa_unmarshal_MatrixTranslatefEXT(struct gl_context *ctx, const struct marshal_cmd_MatrixTranslatefEXT *cmd);
struct marshal_cmd_MatrixTranslatedEXT;
uint32_t _mesa_unmarshal_MatrixTranslatedEXT(struct gl_context *ctx, const struct marshal_cmd_MatrixTranslatedEXT *cmd);
struct marshal_cmd_MatrixOrthoEXT;
uint32_t _mesa_unmarshal_MatrixOrthoEXT(struct gl_context *ctx, const struct marshal_cmd_MatrixOrthoEXT *cmd);
struct marshal_cmd_MatrixFrustumEXT;
uint32_t _mesa_unmarshal_MatrixFrustumEXT(struct gl_context *ctx, const struct marshal_cmd_MatrixFrustumEXT *cmd);
struct marshal_cmd_MatrixPushEXT;
uint32_t _mesa_unmarshal_MatrixPushEXT(struct gl_context *ctx, const struct marshal_cmd_MatrixPushEXT *cmd);
struct marshal_cmd_MatrixPopEXT;
uint32_t _mesa_unmarshal_MatrixPopEXT(struct gl_context *ctx, const struct marshal_cmd_MatrixPopEXT *cmd);
struct marshal_cmd_ClientAttribDefaultEXT;
uint32_t _mesa_unmarshal_ClientAttribDefaultEXT(struct gl_context *ctx, const struct marshal_cmd_ClientAttribDefaultEXT *cmd);
struct marshal_cmd_PushClientAttribDefaultEXT;
uint32_t _mesa_unmarshal_PushClientAttribDefaultEXT(struct gl_context *ctx, const struct marshal_cmd_PushClientAttribDefaultEXT *cmd);
struct marshal_cmd_TextureParameteriEXT;
uint32_t _mesa_unmarshal_TextureParameteriEXT(struct gl_context *ctx, const struct marshal_cmd_TextureParameteriEXT *cmd);
struct marshal_cmd_TextureParameterivEXT;
uint32_t _mesa_unmarshal_TextureParameterivEXT(struct gl_context *ctx, const struct marshal_cmd_TextureParameterivEXT *cmd);
struct marshal_cmd_TextureParameterfEXT;
uint32_t _mesa_unmarshal_TextureParameterfEXT(struct gl_context *ctx, const struct marshal_cmd_TextureParameterfEXT *cmd);
struct marshal_cmd_TextureParameterfvEXT;
uint32_t _mesa_unmarshal_TextureParameterfvEXT(struct gl_context *ctx, const struct marshal_cmd_TextureParameterfvEXT *cmd);
struct marshal_cmd_TextureImage1DEXT;
uint32_t _mesa_unmarshal_TextureImage1DEXT(struct gl_context *ctx, const struct marshal_cmd_TextureImage1DEXT *cmd);
struct marshal_cmd_TextureImage2DEXT;
uint32_t _mesa_unmarshal_TextureImage2DEXT(struct gl_context *ctx, const struct marshal_cmd_TextureImage2DEXT *cmd);
struct marshal_cmd_TextureImage3DEXT;
uint32_t _mesa_unmarshal_TextureImage3DEXT(struct gl_context *ctx, const struct marshal_cmd_TextureImage3DEXT *cmd);
struct marshal_cmd_TextureSubImage1DEXT;
uint32_t _mesa_unmarshal_TextureSubImage1DEXT(struct gl_context *ctx, const struct marshal_cmd_TextureSubImage1DEXT *cmd);
struct marshal_cmd_TextureSubImage2DEXT;
uint32_t _mesa_unmarshal_TextureSubImage2DEXT(struct gl_context *ctx, const struct marshal_cmd_TextureSubImage2DEXT *cmd);
struct marshal_cmd_TextureSubImage3DEXT;
uint32_t _mesa_unmarshal_TextureSubImage3DEXT(struct gl_context *ctx, const struct marshal_cmd_TextureSubImage3DEXT *cmd);
struct marshal_cmd_CopyTextureImage1DEXT;
uint32_t _mesa_unmarshal_CopyTextureImage1DEXT(struct gl_context *ctx, const struct marshal_cmd_CopyTextureImage1DEXT *cmd);
struct marshal_cmd_CopyTextureImage2DEXT;
uint32_t _mesa_unmarshal_CopyTextureImage2DEXT(struct gl_context *ctx, const struct marshal_cmd_CopyTextureImage2DEXT *cmd);
struct marshal_cmd_CopyTextureSubImage1DEXT;
uint32_t _mesa_unmarshal_CopyTextureSubImage1DEXT(struct gl_context *ctx, const struct marshal_cmd_CopyTextureSubImage1DEXT *cmd);
struct marshal_cmd_CopyTextureSubImage2DEXT;
uint32_t _mesa_unmarshal_CopyTextureSubImage2DEXT(struct gl_context *ctx, const struct marshal_cmd_CopyTextureSubImage2DEXT *cmd);
struct marshal_cmd_CopyTextureSubImage3DEXT;
uint32_t _mesa_unmarshal_CopyTextureSubImage3DEXT(struct gl_context *ctx, const struct marshal_cmd_CopyTextureSubImage3DEXT *cmd);
struct marshal_cmd_GetTextureImageEXT;
uint32_t _mesa_unmarshal_GetTextureImageEXT(struct gl_context *ctx, const struct marshal_cmd_GetTextureImageEXT *cmd);
struct marshal_cmd_BindMultiTextureEXT;
uint32_t _mesa_unmarshal_BindMultiTextureEXT(struct gl_context *ctx, const struct marshal_cmd_BindMultiTextureEXT *cmd);
struct marshal_cmd_EnableClientStateiEXT;
uint32_t _mesa_unmarshal_EnableClientStateiEXT(struct gl_context *ctx, const struct marshal_cmd_EnableClientStateiEXT *cmd);
struct marshal_cmd_DisableClientStateiEXT;
uint32_t _mesa_unmarshal_DisableClientStateiEXT(struct gl_context *ctx, const struct marshal_cmd_DisableClientStateiEXT *cmd);
struct marshal_cmd_MultiTexEnviEXT;
uint32_t _mesa_unmarshal_MultiTexEnviEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexEnviEXT *cmd);
struct marshal_cmd_MultiTexEnvivEXT;
uint32_t _mesa_unmarshal_MultiTexEnvivEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexEnvivEXT *cmd);
struct marshal_cmd_MultiTexEnvfEXT;
uint32_t _mesa_unmarshal_MultiTexEnvfEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexEnvfEXT *cmd);
struct marshal_cmd_MultiTexEnvfvEXT;
uint32_t _mesa_unmarshal_MultiTexEnvfvEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexEnvfvEXT *cmd);
struct marshal_cmd_MultiTexParameteriEXT;
uint32_t _mesa_unmarshal_MultiTexParameteriEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexParameteriEXT *cmd);
struct marshal_cmd_MultiTexParameterivEXT;
uint32_t _mesa_unmarshal_MultiTexParameterivEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexParameterivEXT *cmd);
struct marshal_cmd_MultiTexParameterfEXT;
uint32_t _mesa_unmarshal_MultiTexParameterfEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexParameterfEXT *cmd);
struct marshal_cmd_MultiTexParameterfvEXT;
uint32_t _mesa_unmarshal_MultiTexParameterfvEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexParameterfvEXT *cmd);
struct marshal_cmd_GetMultiTexImageEXT;
uint32_t _mesa_unmarshal_GetMultiTexImageEXT(struct gl_context *ctx, const struct marshal_cmd_GetMultiTexImageEXT *cmd);
struct marshal_cmd_MultiTexImage1DEXT;
uint32_t _mesa_unmarshal_MultiTexImage1DEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexImage1DEXT *cmd);
struct marshal_cmd_MultiTexImage2DEXT;
uint32_t _mesa_unmarshal_MultiTexImage2DEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexImage2DEXT *cmd);
struct marshal_cmd_MultiTexImage3DEXT;
uint32_t _mesa_unmarshal_MultiTexImage3DEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexImage3DEXT *cmd);
struct marshal_cmd_MultiTexSubImage1DEXT;
uint32_t _mesa_unmarshal_MultiTexSubImage1DEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexSubImage1DEXT *cmd);
struct marshal_cmd_MultiTexSubImage2DEXT;
uint32_t _mesa_unmarshal_MultiTexSubImage2DEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexSubImage2DEXT *cmd);
struct marshal_cmd_MultiTexSubImage3DEXT;
uint32_t _mesa_unmarshal_MultiTexSubImage3DEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexSubImage3DEXT *cmd);
struct marshal_cmd_CopyMultiTexImage1DEXT;
uint32_t _mesa_unmarshal_CopyMultiTexImage1DEXT(struct gl_context *ctx, const struct marshal_cmd_CopyMultiTexImage1DEXT *cmd);
struct marshal_cmd_CopyMultiTexImage2DEXT;
uint32_t _mesa_unmarshal_CopyMultiTexImage2DEXT(struct gl_context *ctx, const struct marshal_cmd_CopyMultiTexImage2DEXT *cmd);
struct marshal_cmd_CopyMultiTexSubImage1DEXT;
uint32_t _mesa_unmarshal_CopyMultiTexSubImage1DEXT(struct gl_context *ctx, const struct marshal_cmd_CopyMultiTexSubImage1DEXT *cmd);
struct marshal_cmd_CopyMultiTexSubImage2DEXT;
uint32_t _mesa_unmarshal_CopyMultiTexSubImage2DEXT(struct gl_context *ctx, const struct marshal_cmd_CopyMultiTexSubImage2DEXT *cmd);
struct marshal_cmd_CopyMultiTexSubImage3DEXT;
uint32_t _mesa_unmarshal_CopyMultiTexSubImage3DEXT(struct gl_context *ctx, const struct marshal_cmd_CopyMultiTexSubImage3DEXT *cmd);
struct marshal_cmd_MultiTexGendEXT;
uint32_t _mesa_unmarshal_MultiTexGendEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexGendEXT *cmd);
struct marshal_cmd_MultiTexGendvEXT;
uint32_t _mesa_unmarshal_MultiTexGendvEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexGendvEXT *cmd);
struct marshal_cmd_MultiTexGenfEXT;
uint32_t _mesa_unmarshal_MultiTexGenfEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexGenfEXT *cmd);
struct marshal_cmd_MultiTexGenfvEXT;
uint32_t _mesa_unmarshal_MultiTexGenfvEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexGenfvEXT *cmd);
struct marshal_cmd_MultiTexGeniEXT;
uint32_t _mesa_unmarshal_MultiTexGeniEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexGeniEXT *cmd);
struct marshal_cmd_MultiTexGenivEXT;
uint32_t _mesa_unmarshal_MultiTexGenivEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexGenivEXT *cmd);
struct marshal_cmd_MultiTexCoordPointerEXT;
uint32_t _mesa_unmarshal_MultiTexCoordPointerEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoordPointerEXT *cmd);
struct marshal_cmd_MatrixLoadTransposefEXT;
uint32_t _mesa_unmarshal_MatrixLoadTransposefEXT(struct gl_context *ctx, const struct marshal_cmd_MatrixLoadTransposefEXT *cmd);
struct marshal_cmd_MatrixLoadTransposedEXT;
uint32_t _mesa_unmarshal_MatrixLoadTransposedEXT(struct gl_context *ctx, const struct marshal_cmd_MatrixLoadTransposedEXT *cmd);
struct marshal_cmd_MatrixMultTransposefEXT;
uint32_t _mesa_unmarshal_MatrixMultTransposefEXT(struct gl_context *ctx, const struct marshal_cmd_MatrixMultTransposefEXT *cmd);
struct marshal_cmd_MatrixMultTransposedEXT;
uint32_t _mesa_unmarshal_MatrixMultTransposedEXT(struct gl_context *ctx, const struct marshal_cmd_MatrixMultTransposedEXT *cmd);
struct marshal_cmd_CompressedTextureImage1DEXT;
uint32_t _mesa_unmarshal_CompressedTextureImage1DEXT(struct gl_context *ctx, const struct marshal_cmd_CompressedTextureImage1DEXT *cmd);
struct marshal_cmd_CompressedTextureImage2DEXT;
uint32_t _mesa_unmarshal_CompressedTextureImage2DEXT(struct gl_context *ctx, const struct marshal_cmd_CompressedTextureImage2DEXT *cmd);
struct marshal_cmd_CompressedTextureImage3DEXT;
uint32_t _mesa_unmarshal_CompressedTextureImage3DEXT(struct gl_context *ctx, const struct marshal_cmd_CompressedTextureImage3DEXT *cmd);
struct marshal_cmd_CompressedTextureSubImage1DEXT;
uint32_t _mesa_unmarshal_CompressedTextureSubImage1DEXT(struct gl_context *ctx, const struct marshal_cmd_CompressedTextureSubImage1DEXT *cmd);
struct marshal_cmd_CompressedTextureSubImage2DEXT;
uint32_t _mesa_unmarshal_CompressedTextureSubImage2DEXT(struct gl_context *ctx, const struct marshal_cmd_CompressedTextureSubImage2DEXT *cmd);
struct marshal_cmd_CompressedTextureSubImage3DEXT;
uint32_t _mesa_unmarshal_CompressedTextureSubImage3DEXT(struct gl_context *ctx, const struct marshal_cmd_CompressedTextureSubImage3DEXT *cmd);
struct marshal_cmd_GetCompressedTextureImageEXT;
uint32_t _mesa_unmarshal_GetCompressedTextureImageEXT(struct gl_context *ctx, const struct marshal_cmd_GetCompressedTextureImageEXT *cmd);
struct marshal_cmd_CompressedMultiTexImage1DEXT;
uint32_t _mesa_unmarshal_CompressedMultiTexImage1DEXT(struct gl_context *ctx, const struct marshal_cmd_CompressedMultiTexImage1DEXT *cmd);
struct marshal_cmd_CompressedMultiTexImage2DEXT;
uint32_t _mesa_unmarshal_CompressedMultiTexImage2DEXT(struct gl_context *ctx, const struct marshal_cmd_CompressedMultiTexImage2DEXT *cmd);
struct marshal_cmd_CompressedMultiTexImage3DEXT;
uint32_t _mesa_unmarshal_CompressedMultiTexImage3DEXT(struct gl_context *ctx, const struct marshal_cmd_CompressedMultiTexImage3DEXT *cmd);
struct marshal_cmd_CompressedMultiTexSubImage1DEXT;
uint32_t _mesa_unmarshal_CompressedMultiTexSubImage1DEXT(struct gl_context *ctx, const struct marshal_cmd_CompressedMultiTexSubImage1DEXT *cmd);
struct marshal_cmd_CompressedMultiTexSubImage2DEXT;
uint32_t _mesa_unmarshal_CompressedMultiTexSubImage2DEXT(struct gl_context *ctx, const struct marshal_cmd_CompressedMultiTexSubImage2DEXT *cmd);
struct marshal_cmd_CompressedMultiTexSubImage3DEXT;
uint32_t _mesa_unmarshal_CompressedMultiTexSubImage3DEXT(struct gl_context *ctx, const struct marshal_cmd_CompressedMultiTexSubImage3DEXT *cmd);
struct marshal_cmd_GetCompressedMultiTexImageEXT;
uint32_t _mesa_unmarshal_GetCompressedMultiTexImageEXT(struct gl_context *ctx, const struct marshal_cmd_GetCompressedMultiTexImageEXT *cmd);
struct marshal_cmd_NamedBufferDataEXT;
uint32_t _mesa_unmarshal_NamedBufferDataEXT(struct gl_context *ctx, const struct marshal_cmd_NamedBufferDataEXT *cmd);
void GLAPIENTRY _mesa_marshal_NamedBufferDataEXT(GLuint buffer, GLsizeiptr size, const GLvoid * data, GLenum usage);
struct marshal_cmd_NamedBufferSubDataEXT;
uint32_t _mesa_unmarshal_NamedBufferSubDataEXT(struct gl_context *ctx, const struct marshal_cmd_NamedBufferSubDataEXT *cmd);
void GLAPIENTRY _mesa_marshal_NamedBufferSubDataEXT(GLuint buffer, GLintptr offset, GLsizeiptr size, const GLvoid * data);
struct marshal_cmd_FlushMappedNamedBufferRangeEXT;
uint32_t _mesa_unmarshal_FlushMappedNamedBufferRangeEXT(struct gl_context *ctx, const struct marshal_cmd_FlushMappedNamedBufferRangeEXT *cmd);
struct marshal_cmd_FramebufferDrawBufferEXT;
uint32_t _mesa_unmarshal_FramebufferDrawBufferEXT(struct gl_context *ctx, const struct marshal_cmd_FramebufferDrawBufferEXT *cmd);
struct marshal_cmd_FramebufferDrawBuffersEXT;
uint32_t _mesa_unmarshal_FramebufferDrawBuffersEXT(struct gl_context *ctx, const struct marshal_cmd_FramebufferDrawBuffersEXT *cmd);
struct marshal_cmd_FramebufferReadBufferEXT;
uint32_t _mesa_unmarshal_FramebufferReadBufferEXT(struct gl_context *ctx, const struct marshal_cmd_FramebufferReadBufferEXT *cmd);
struct marshal_cmd_NamedFramebufferTexture1DEXT;
uint32_t _mesa_unmarshal_NamedFramebufferTexture1DEXT(struct gl_context *ctx, const struct marshal_cmd_NamedFramebufferTexture1DEXT *cmd);
struct marshal_cmd_NamedFramebufferTexture2DEXT;
uint32_t _mesa_unmarshal_NamedFramebufferTexture2DEXT(struct gl_context *ctx, const struct marshal_cmd_NamedFramebufferTexture2DEXT *cmd);
struct marshal_cmd_NamedFramebufferTexture3DEXT;
uint32_t _mesa_unmarshal_NamedFramebufferTexture3DEXT(struct gl_context *ctx, const struct marshal_cmd_NamedFramebufferTexture3DEXT *cmd);
struct marshal_cmd_NamedFramebufferRenderbufferEXT;
uint32_t _mesa_unmarshal_NamedFramebufferRenderbufferEXT(struct gl_context *ctx, const struct marshal_cmd_NamedFramebufferRenderbufferEXT *cmd);
struct marshal_cmd_NamedRenderbufferStorageEXT;
uint32_t _mesa_unmarshal_NamedRenderbufferStorageEXT(struct gl_context *ctx, const struct marshal_cmd_NamedRenderbufferStorageEXT *cmd);
struct marshal_cmd_GenerateTextureMipmapEXT;
uint32_t _mesa_unmarshal_GenerateTextureMipmapEXT(struct gl_context *ctx, const struct marshal_cmd_GenerateTextureMipmapEXT *cmd);
struct marshal_cmd_GenerateMultiTexMipmapEXT;
uint32_t _mesa_unmarshal_GenerateMultiTexMipmapEXT(struct gl_context *ctx, const struct marshal_cmd_GenerateMultiTexMipmapEXT *cmd);
struct marshal_cmd_NamedRenderbufferStorageMultisampleEXT;
uint32_t _mesa_unmarshal_NamedRenderbufferStorageMultisampleEXT(struct gl_context *ctx, const struct marshal_cmd_NamedRenderbufferStorageMultisampleEXT *cmd);
struct marshal_cmd_NamedCopyBufferSubDataEXT;
uint32_t _mesa_unmarshal_NamedCopyBufferSubDataEXT(struct gl_context *ctx, const struct marshal_cmd_NamedCopyBufferSubDataEXT *cmd);
struct marshal_cmd_VertexArrayVertexOffsetEXT;
uint32_t _mesa_unmarshal_VertexArrayVertexOffsetEXT(struct gl_context *ctx, const struct marshal_cmd_VertexArrayVertexOffsetEXT *cmd);
struct marshal_cmd_VertexArrayColorOffsetEXT;
uint32_t _mesa_unmarshal_VertexArrayColorOffsetEXT(struct gl_context *ctx, const struct marshal_cmd_VertexArrayColorOffsetEXT *cmd);
struct marshal_cmd_VertexArrayEdgeFlagOffsetEXT;
uint32_t _mesa_unmarshal_VertexArrayEdgeFlagOffsetEXT(struct gl_context *ctx, const struct marshal_cmd_VertexArrayEdgeFlagOffsetEXT *cmd);
struct marshal_cmd_VertexArrayIndexOffsetEXT;
uint32_t _mesa_unmarshal_VertexArrayIndexOffsetEXT(struct gl_context *ctx, const struct marshal_cmd_VertexArrayIndexOffsetEXT *cmd);
struct marshal_cmd_VertexArrayNormalOffsetEXT;
uint32_t _mesa_unmarshal_VertexArrayNormalOffsetEXT(struct gl_context *ctx, const struct marshal_cmd_VertexArrayNormalOffsetEXT *cmd);
struct marshal_cmd_VertexArrayTexCoordOffsetEXT;
uint32_t _mesa_unmarshal_VertexArrayTexCoordOffsetEXT(struct gl_context *ctx, const struct marshal_cmd_VertexArrayTexCoordOffsetEXT *cmd);
struct marshal_cmd_VertexArrayMultiTexCoordOffsetEXT;
uint32_t _mesa_unmarshal_VertexArrayMultiTexCoordOffsetEXT(struct gl_context *ctx, const struct marshal_cmd_VertexArrayMultiTexCoordOffsetEXT *cmd);
struct marshal_cmd_VertexArrayFogCoordOffsetEXT;
uint32_t _mesa_unmarshal_VertexArrayFogCoordOffsetEXT(struct gl_context *ctx, const struct marshal_cmd_VertexArrayFogCoordOffsetEXT *cmd);
struct marshal_cmd_VertexArraySecondaryColorOffsetEXT;
uint32_t _mesa_unmarshal_VertexArraySecondaryColorOffsetEXT(struct gl_context *ctx, const struct marshal_cmd_VertexArraySecondaryColorOffsetEXT *cmd);
struct marshal_cmd_VertexArrayVertexAttribOffsetEXT;
uint32_t _mesa_unmarshal_VertexArrayVertexAttribOffsetEXT(struct gl_context *ctx, const struct marshal_cmd_VertexArrayVertexAttribOffsetEXT *cmd);
struct marshal_cmd_VertexArrayVertexAttribIOffsetEXT;
uint32_t _mesa_unmarshal_VertexArrayVertexAttribIOffsetEXT(struct gl_context *ctx, const struct marshal_cmd_VertexArrayVertexAttribIOffsetEXT *cmd);
struct marshal_cmd_EnableVertexArrayEXT;
uint32_t _mesa_unmarshal_EnableVertexArrayEXT(struct gl_context *ctx, const struct marshal_cmd_EnableVertexArrayEXT *cmd);
struct marshal_cmd_DisableVertexArrayEXT;
uint32_t _mesa_unmarshal_DisableVertexArrayEXT(struct gl_context *ctx, const struct marshal_cmd_DisableVertexArrayEXT *cmd);
struct marshal_cmd_EnableVertexArrayAttribEXT;
uint32_t _mesa_unmarshal_EnableVertexArrayAttribEXT(struct gl_context *ctx, const struct marshal_cmd_EnableVertexArrayAttribEXT *cmd);
struct marshal_cmd_DisableVertexArrayAttribEXT;
uint32_t _mesa_unmarshal_DisableVertexArrayAttribEXT(struct gl_context *ctx, const struct marshal_cmd_DisableVertexArrayAttribEXT *cmd);
struct marshal_cmd_NamedProgramStringEXT;
uint32_t _mesa_unmarshal_NamedProgramStringEXT(struct gl_context *ctx, const struct marshal_cmd_NamedProgramStringEXT *cmd);
struct marshal_cmd_NamedProgramLocalParameter4fEXT;
uint32_t _mesa_unmarshal_NamedProgramLocalParameter4fEXT(struct gl_context *ctx, const struct marshal_cmd_NamedProgramLocalParameter4fEXT *cmd);
struct marshal_cmd_NamedProgramLocalParameter4fvEXT;
uint32_t _mesa_unmarshal_NamedProgramLocalParameter4fvEXT(struct gl_context *ctx, const struct marshal_cmd_NamedProgramLocalParameter4fvEXT *cmd);
struct marshal_cmd_NamedProgramLocalParameter4dEXT;
uint32_t _mesa_unmarshal_NamedProgramLocalParameter4dEXT(struct gl_context *ctx, const struct marshal_cmd_NamedProgramLocalParameter4dEXT *cmd);
struct marshal_cmd_NamedProgramLocalParameter4dvEXT;
uint32_t _mesa_unmarshal_NamedProgramLocalParameter4dvEXT(struct gl_context *ctx, const struct marshal_cmd_NamedProgramLocalParameter4dvEXT *cmd);
struct marshal_cmd_TextureBufferEXT;
uint32_t _mesa_unmarshal_TextureBufferEXT(struct gl_context *ctx, const struct marshal_cmd_TextureBufferEXT *cmd);
struct marshal_cmd_MultiTexBufferEXT;
uint32_t _mesa_unmarshal_MultiTexBufferEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexBufferEXT *cmd);
struct marshal_cmd_TextureParameterIivEXT;
uint32_t _mesa_unmarshal_TextureParameterIivEXT(struct gl_context *ctx, const struct marshal_cmd_TextureParameterIivEXT *cmd);
struct marshal_cmd_TextureParameterIuivEXT;
uint32_t _mesa_unmarshal_TextureParameterIuivEXT(struct gl_context *ctx, const struct marshal_cmd_TextureParameterIuivEXT *cmd);
struct marshal_cmd_MultiTexParameterIivEXT;
uint32_t _mesa_unmarshal_MultiTexParameterIivEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexParameterIivEXT *cmd);
struct marshal_cmd_MultiTexParameterIuivEXT;
uint32_t _mesa_unmarshal_MultiTexParameterIuivEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexParameterIuivEXT *cmd);
struct marshal_cmd_NamedProgramLocalParameters4fvEXT;
uint32_t _mesa_unmarshal_NamedProgramLocalParameters4fvEXT(struct gl_context *ctx, const struct marshal_cmd_NamedProgramLocalParameters4fvEXT *cmd);
struct marshal_cmd_BindImageTextureEXT;
uint32_t _mesa_unmarshal_BindImageTextureEXT(struct gl_context *ctx, const struct marshal_cmd_BindImageTextureEXT *cmd);
struct marshal_cmd_SubpixelPrecisionBiasNV;
uint32_t _mesa_unmarshal_SubpixelPrecisionBiasNV(struct gl_context *ctx, const struct marshal_cmd_SubpixelPrecisionBiasNV *cmd);
struct marshal_cmd_ConservativeRasterParameterfNV;
uint32_t _mesa_unmarshal_ConservativeRasterParameterfNV(struct gl_context *ctx, const struct marshal_cmd_ConservativeRasterParameterfNV *cmd);
struct marshal_cmd_ConservativeRasterParameteriNV;
uint32_t _mesa_unmarshal_ConservativeRasterParameteriNV(struct gl_context *ctx, const struct marshal_cmd_ConservativeRasterParameteriNV *cmd);
struct marshal_cmd_DeletePerfQueryINTEL;
uint32_t _mesa_unmarshal_DeletePerfQueryINTEL(struct gl_context *ctx, const struct marshal_cmd_DeletePerfQueryINTEL *cmd);
struct marshal_cmd_BeginPerfQueryINTEL;
uint32_t _mesa_unmarshal_BeginPerfQueryINTEL(struct gl_context *ctx, const struct marshal_cmd_BeginPerfQueryINTEL *cmd);
struct marshal_cmd_EndPerfQueryINTEL;
uint32_t _mesa_unmarshal_EndPerfQueryINTEL(struct gl_context *ctx, const struct marshal_cmd_EndPerfQueryINTEL *cmd);
struct marshal_cmd_AlphaToCoverageDitherControlNV;
uint32_t _mesa_unmarshal_AlphaToCoverageDitherControlNV(struct gl_context *ctx, const struct marshal_cmd_AlphaToCoverageDitherControlNV *cmd);
struct marshal_cmd_PolygonOffsetClampEXT;
uint32_t _mesa_unmarshal_PolygonOffsetClampEXT(struct gl_context *ctx, const struct marshal_cmd_PolygonOffsetClampEXT *cmd);
struct marshal_cmd_WindowRectanglesEXT;
uint32_t _mesa_unmarshal_WindowRectanglesEXT(struct gl_context *ctx, const struct marshal_cmd_WindowRectanglesEXT *cmd);
struct marshal_cmd_FramebufferFetchBarrierEXT;
uint32_t _mesa_unmarshal_FramebufferFetchBarrierEXT(struct gl_context *ctx, const struct marshal_cmd_FramebufferFetchBarrierEXT *cmd);
struct marshal_cmd_RenderbufferStorageMultisampleAdvancedAMD;
uint32_t _mesa_unmarshal_RenderbufferStorageMultisampleAdvancedAMD(struct gl_context *ctx, const struct marshal_cmd_RenderbufferStorageMultisampleAdvancedAMD *cmd);
struct marshal_cmd_NamedRenderbufferStorageMultisampleAdvancedAMD;
uint32_t _mesa_unmarshal_NamedRenderbufferStorageMultisampleAdvancedAMD(struct gl_context *ctx, const struct marshal_cmd_NamedRenderbufferStorageMultisampleAdvancedAMD *cmd);
struct marshal_cmd_StencilFuncSeparateATI;
uint32_t _mesa_unmarshal_StencilFuncSeparateATI(struct gl_context *ctx, const struct marshal_cmd_StencilFuncSeparateATI *cmd);
struct marshal_cmd_ProgramEnvParameters4fvEXT;
uint32_t _mesa_unmarshal_ProgramEnvParameters4fvEXT(struct gl_context *ctx, const struct marshal_cmd_ProgramEnvParameters4fvEXT *cmd);
struct marshal_cmd_ProgramLocalParameters4fvEXT;
uint32_t _mesa_unmarshal_ProgramLocalParameters4fvEXT(struct gl_context *ctx, const struct marshal_cmd_ProgramLocalParameters4fvEXT *cmd);
struct marshal_cmd_InternalBufferSubDataCopyMESA;
uint32_t _mesa_unmarshal_InternalBufferSubDataCopyMESA(struct gl_context *ctx, const struct marshal_cmd_InternalBufferSubDataCopyMESA *cmd);
void GLAPIENTRY _mesa_marshal_InternalBufferSubDataCopyMESA(GLintptr srcBuffer, GLuint srcOffset, GLuint dstTargetOrName, GLintptr dstOffset, GLsizeiptr size, GLboolean named, GLboolean ext_dsa);
struct marshal_cmd_InternalSetError;
uint32_t _mesa_unmarshal_InternalSetError(struct gl_context *ctx, const struct marshal_cmd_InternalSetError *cmd);
void GLAPIENTRY _mesa_marshal_InternalSetError(GLenum error);
struct marshal_cmd_DrawArraysUserBuf;
uint32_t _mesa_unmarshal_DrawArraysUserBuf(struct gl_context *ctx, const struct marshal_cmd_DrawArraysUserBuf *cmd);
void GLAPIENTRY _mesa_marshal_DrawArraysUserBuf(void);
struct marshal_cmd_DrawElementsUserBuf;
uint32_t _mesa_unmarshal_DrawElementsUserBuf(struct gl_context *ctx, const struct marshal_cmd_DrawElementsUserBuf *cmd);
void GLAPIENTRY _mesa_marshal_DrawElementsUserBuf(void);
struct marshal_cmd_MultiDrawArraysUserBuf;
uint32_t _mesa_unmarshal_MultiDrawArraysUserBuf(struct gl_context *ctx, const struct marshal_cmd_MultiDrawArraysUserBuf *cmd);
void GLAPIENTRY _mesa_marshal_MultiDrawArraysUserBuf(void);
struct marshal_cmd_MultiDrawElementsUserBuf;
uint32_t _mesa_unmarshal_MultiDrawElementsUserBuf(struct gl_context *ctx, const struct marshal_cmd_MultiDrawElementsUserBuf *cmd);
void GLAPIENTRY _mesa_marshal_MultiDrawElementsUserBuf(void);
struct marshal_cmd_ClearColorIiEXT;
uint32_t _mesa_unmarshal_ClearColorIiEXT(struct gl_context *ctx, const struct marshal_cmd_ClearColorIiEXT *cmd);
struct marshal_cmd_ClearColorIuiEXT;
uint32_t _mesa_unmarshal_ClearColorIuiEXT(struct gl_context *ctx, const struct marshal_cmd_ClearColorIuiEXT *cmd);
struct marshal_cmd_TexParameterIiv;
uint32_t _mesa_unmarshal_TexParameterIiv(struct gl_context *ctx, const struct marshal_cmd_TexParameterIiv *cmd);
struct marshal_cmd_TexParameterIuiv;
uint32_t _mesa_unmarshal_TexParameterIuiv(struct gl_context *ctx, const struct marshal_cmd_TexParameterIuiv *cmd);
struct marshal_cmd_VertexAttribI1iEXT;
uint32_t _mesa_unmarshal_VertexAttribI1iEXT(struct gl_context *ctx, const struct marshal_cmd_VertexAttribI1iEXT *cmd);
struct marshal_cmd_VertexAttribI2iEXT;
uint32_t _mesa_unmarshal_VertexAttribI2iEXT(struct gl_context *ctx, const struct marshal_cmd_VertexAttribI2iEXT *cmd);
struct marshal_cmd_VertexAttribI3iEXT;
uint32_t _mesa_unmarshal_VertexAttribI3iEXT(struct gl_context *ctx, const struct marshal_cmd_VertexAttribI3iEXT *cmd);
struct marshal_cmd_VertexAttribI4iEXT;
uint32_t _mesa_unmarshal_VertexAttribI4iEXT(struct gl_context *ctx, const struct marshal_cmd_VertexAttribI4iEXT *cmd);
struct marshal_cmd_VertexAttribI1uiEXT;
uint32_t _mesa_unmarshal_VertexAttribI1uiEXT(struct gl_context *ctx, const struct marshal_cmd_VertexAttribI1uiEXT *cmd);
struct marshal_cmd_VertexAttribI2uiEXT;
uint32_t _mesa_unmarshal_VertexAttribI2uiEXT(struct gl_context *ctx, const struct marshal_cmd_VertexAttribI2uiEXT *cmd);
struct marshal_cmd_VertexAttribI3uiEXT;
uint32_t _mesa_unmarshal_VertexAttribI3uiEXT(struct gl_context *ctx, const struct marshal_cmd_VertexAttribI3uiEXT *cmd);
struct marshal_cmd_VertexAttribI4uiEXT;
uint32_t _mesa_unmarshal_VertexAttribI4uiEXT(struct gl_context *ctx, const struct marshal_cmd_VertexAttribI4uiEXT *cmd);
struct marshal_cmd_VertexAttribI1iv;
uint32_t _mesa_unmarshal_VertexAttribI1iv(struct gl_context *ctx, const struct marshal_cmd_VertexAttribI1iv *cmd);
struct marshal_cmd_VertexAttribI2ivEXT;
uint32_t _mesa_unmarshal_VertexAttribI2ivEXT(struct gl_context *ctx, const struct marshal_cmd_VertexAttribI2ivEXT *cmd);
struct marshal_cmd_VertexAttribI3ivEXT;
uint32_t _mesa_unmarshal_VertexAttribI3ivEXT(struct gl_context *ctx, const struct marshal_cmd_VertexAttribI3ivEXT *cmd);
struct marshal_cmd_VertexAttribI4ivEXT;
uint32_t _mesa_unmarshal_VertexAttribI4ivEXT(struct gl_context *ctx, const struct marshal_cmd_VertexAttribI4ivEXT *cmd);
struct marshal_cmd_VertexAttribI1uiv;
uint32_t _mesa_unmarshal_VertexAttribI1uiv(struct gl_context *ctx, const struct marshal_cmd_VertexAttribI1uiv *cmd);
struct marshal_cmd_VertexAttribI2uivEXT;
uint32_t _mesa_unmarshal_VertexAttribI2uivEXT(struct gl_context *ctx, const struct marshal_cmd_VertexAttribI2uivEXT *cmd);
struct marshal_cmd_VertexAttribI3uivEXT;
uint32_t _mesa_unmarshal_VertexAttribI3uivEXT(struct gl_context *ctx, const struct marshal_cmd_VertexAttribI3uivEXT *cmd);
struct marshal_cmd_VertexAttribI4uivEXT;
uint32_t _mesa_unmarshal_VertexAttribI4uivEXT(struct gl_context *ctx, const struct marshal_cmd_VertexAttribI4uivEXT *cmd);
struct marshal_cmd_VertexAttribI4bv;
uint32_t _mesa_unmarshal_VertexAttribI4bv(struct gl_context *ctx, const struct marshal_cmd_VertexAttribI4bv *cmd);
struct marshal_cmd_VertexAttribI4sv;
uint32_t _mesa_unmarshal_VertexAttribI4sv(struct gl_context *ctx, const struct marshal_cmd_VertexAttribI4sv *cmd);
struct marshal_cmd_VertexAttribI4ubv;
uint32_t _mesa_unmarshal_VertexAttribI4ubv(struct gl_context *ctx, const struct marshal_cmd_VertexAttribI4ubv *cmd);
struct marshal_cmd_VertexAttribI4usv;
uint32_t _mesa_unmarshal_VertexAttribI4usv(struct gl_context *ctx, const struct marshal_cmd_VertexAttribI4usv *cmd);
struct marshal_cmd_VertexAttribIPointer;
uint32_t _mesa_unmarshal_VertexAttribIPointer(struct gl_context *ctx, const struct marshal_cmd_VertexAttribIPointer *cmd);
struct marshal_cmd_Uniform1ui;
uint32_t _mesa_unmarshal_Uniform1ui(struct gl_context *ctx, const struct marshal_cmd_Uniform1ui *cmd);
struct marshal_cmd_Uniform2ui;
uint32_t _mesa_unmarshal_Uniform2ui(struct gl_context *ctx, const struct marshal_cmd_Uniform2ui *cmd);
struct marshal_cmd_Uniform3ui;
uint32_t _mesa_unmarshal_Uniform3ui(struct gl_context *ctx, const struct marshal_cmd_Uniform3ui *cmd);
struct marshal_cmd_Uniform4ui;
uint32_t _mesa_unmarshal_Uniform4ui(struct gl_context *ctx, const struct marshal_cmd_Uniform4ui *cmd);
struct marshal_cmd_Uniform1uiv;
uint32_t _mesa_unmarshal_Uniform1uiv(struct gl_context *ctx, const struct marshal_cmd_Uniform1uiv *cmd);
struct marshal_cmd_Uniform2uiv;
uint32_t _mesa_unmarshal_Uniform2uiv(struct gl_context *ctx, const struct marshal_cmd_Uniform2uiv *cmd);
struct marshal_cmd_Uniform3uiv;
uint32_t _mesa_unmarshal_Uniform3uiv(struct gl_context *ctx, const struct marshal_cmd_Uniform3uiv *cmd);
struct marshal_cmd_Uniform4uiv;
uint32_t _mesa_unmarshal_Uniform4uiv(struct gl_context *ctx, const struct marshal_cmd_Uniform4uiv *cmd);
struct marshal_cmd_BindFragDataLocation;
uint32_t _mesa_unmarshal_BindFragDataLocation(struct gl_context *ctx, const struct marshal_cmd_BindFragDataLocation *cmd);
struct marshal_cmd_ClearBufferiv;
uint32_t _mesa_unmarshal_ClearBufferiv(struct gl_context *ctx, const struct marshal_cmd_ClearBufferiv *cmd);
struct marshal_cmd_ClearBufferuiv;
uint32_t _mesa_unmarshal_ClearBufferuiv(struct gl_context *ctx, const struct marshal_cmd_ClearBufferuiv *cmd);
struct marshal_cmd_ClearBufferfv;
uint32_t _mesa_unmarshal_ClearBufferfv(struct gl_context *ctx, const struct marshal_cmd_ClearBufferfv *cmd);
struct marshal_cmd_ClearBufferfi;
uint32_t _mesa_unmarshal_ClearBufferfi(struct gl_context *ctx, const struct marshal_cmd_ClearBufferfi *cmd);
struct marshal_cmd_BeginTransformFeedback;
uint32_t _mesa_unmarshal_BeginTransformFeedback(struct gl_context *ctx, const struct marshal_cmd_BeginTransformFeedback *cmd);
struct marshal_cmd_EndTransformFeedback;
uint32_t _mesa_unmarshal_EndTransformFeedback(struct gl_context *ctx, const struct marshal_cmd_EndTransformFeedback *cmd);
struct marshal_cmd_BindBufferRange;
uint32_t _mesa_unmarshal_BindBufferRange(struct gl_context *ctx, const struct marshal_cmd_BindBufferRange *cmd);
struct marshal_cmd_BindBufferBase;
uint32_t _mesa_unmarshal_BindBufferBase(struct gl_context *ctx, const struct marshal_cmd_BindBufferBase *cmd);
struct marshal_cmd_BeginConditionalRender;
uint32_t _mesa_unmarshal_BeginConditionalRender(struct gl_context *ctx, const struct marshal_cmd_BeginConditionalRender *cmd);
struct marshal_cmd_EndConditionalRender;
uint32_t _mesa_unmarshal_EndConditionalRender(struct gl_context *ctx, const struct marshal_cmd_EndConditionalRender *cmd);
struct marshal_cmd_PrimitiveRestartIndex;
uint32_t _mesa_unmarshal_PrimitiveRestartIndex(struct gl_context *ctx, const struct marshal_cmd_PrimitiveRestartIndex *cmd);
struct marshal_cmd_FramebufferTexture;
uint32_t _mesa_unmarshal_FramebufferTexture(struct gl_context *ctx, const struct marshal_cmd_FramebufferTexture *cmd);
struct marshal_cmd_PrimitiveRestartNV;
uint32_t _mesa_unmarshal_PrimitiveRestartNV(struct gl_context *ctx, const struct marshal_cmd_PrimitiveRestartNV *cmd);
struct marshal_cmd_BindBufferOffsetEXT;
uint32_t _mesa_unmarshal_BindBufferOffsetEXT(struct gl_context *ctx, const struct marshal_cmd_BindBufferOffsetEXT *cmd);
struct marshal_cmd_BindTransformFeedback;
uint32_t _mesa_unmarshal_BindTransformFeedback(struct gl_context *ctx, const struct marshal_cmd_BindTransformFeedback *cmd);
struct marshal_cmd_DeleteTransformFeedbacks;
uint32_t _mesa_unmarshal_DeleteTransformFeedbacks(struct gl_context *ctx, const struct marshal_cmd_DeleteTransformFeedbacks *cmd);
struct marshal_cmd_PauseTransformFeedback;
uint32_t _mesa_unmarshal_PauseTransformFeedback(struct gl_context *ctx, const struct marshal_cmd_PauseTransformFeedback *cmd);
struct marshal_cmd_ResumeTransformFeedback;
uint32_t _mesa_unmarshal_ResumeTransformFeedback(struct gl_context *ctx, const struct marshal_cmd_ResumeTransformFeedback *cmd);
struct marshal_cmd_DrawTransformFeedback;
uint32_t _mesa_unmarshal_DrawTransformFeedback(struct gl_context *ctx, const struct marshal_cmd_DrawTransformFeedback *cmd);
struct marshal_cmd_VDPAUFiniNV;
uint32_t _mesa_unmarshal_VDPAUFiniNV(struct gl_context *ctx, const struct marshal_cmd_VDPAUFiniNV *cmd);
struct marshal_cmd_VDPAUUnregisterSurfaceNV;
uint32_t _mesa_unmarshal_VDPAUUnregisterSurfaceNV(struct gl_context *ctx, const struct marshal_cmd_VDPAUUnregisterSurfaceNV *cmd);
struct marshal_cmd_VDPAUSurfaceAccessNV;
uint32_t _mesa_unmarshal_VDPAUSurfaceAccessNV(struct gl_context *ctx, const struct marshal_cmd_VDPAUSurfaceAccessNV *cmd);
struct marshal_cmd_VDPAUMapSurfacesNV;
uint32_t _mesa_unmarshal_VDPAUMapSurfacesNV(struct gl_context *ctx, const struct marshal_cmd_VDPAUMapSurfacesNV *cmd);
struct marshal_cmd_VDPAUUnmapSurfacesNV;
uint32_t _mesa_unmarshal_VDPAUUnmapSurfacesNV(struct gl_context *ctx, const struct marshal_cmd_VDPAUUnmapSurfacesNV *cmd);
struct marshal_cmd_DeleteMemoryObjectsEXT;
uint32_t _mesa_unmarshal_DeleteMemoryObjectsEXT(struct gl_context *ctx, const struct marshal_cmd_DeleteMemoryObjectsEXT *cmd);
struct marshal_cmd_MemoryObjectParameterivEXT;
uint32_t _mesa_unmarshal_MemoryObjectParameterivEXT(struct gl_context *ctx, const struct marshal_cmd_MemoryObjectParameterivEXT *cmd);
struct marshal_cmd_TexStorageMem2DEXT;
uint32_t _mesa_unmarshal_TexStorageMem2DEXT(struct gl_context *ctx, const struct marshal_cmd_TexStorageMem2DEXT *cmd);
struct marshal_cmd_TexStorageMem2DMultisampleEXT;
uint32_t _mesa_unmarshal_TexStorageMem2DMultisampleEXT(struct gl_context *ctx, const struct marshal_cmd_TexStorageMem2DMultisampleEXT *cmd);
struct marshal_cmd_TexStorageMem3DEXT;
uint32_t _mesa_unmarshal_TexStorageMem3DEXT(struct gl_context *ctx, const struct marshal_cmd_TexStorageMem3DEXT *cmd);
struct marshal_cmd_TexStorageMem3DMultisampleEXT;
uint32_t _mesa_unmarshal_TexStorageMem3DMultisampleEXT(struct gl_context *ctx, const struct marshal_cmd_TexStorageMem3DMultisampleEXT *cmd);
struct marshal_cmd_BufferStorageMemEXT;
uint32_t _mesa_unmarshal_BufferStorageMemEXT(struct gl_context *ctx, const struct marshal_cmd_BufferStorageMemEXT *cmd);
struct marshal_cmd_TextureStorageMem2DEXT;
uint32_t _mesa_unmarshal_TextureStorageMem2DEXT(struct gl_context *ctx, const struct marshal_cmd_TextureStorageMem2DEXT *cmd);
struct marshal_cmd_TextureStorageMem2DMultisampleEXT;
uint32_t _mesa_unmarshal_TextureStorageMem2DMultisampleEXT(struct gl_context *ctx, const struct marshal_cmd_TextureStorageMem2DMultisampleEXT *cmd);
struct marshal_cmd_TextureStorageMem3DEXT;
uint32_t _mesa_unmarshal_TextureStorageMem3DEXT(struct gl_context *ctx, const struct marshal_cmd_TextureStorageMem3DEXT *cmd);
struct marshal_cmd_TextureStorageMem3DMultisampleEXT;
uint32_t _mesa_unmarshal_TextureStorageMem3DMultisampleEXT(struct gl_context *ctx, const struct marshal_cmd_TextureStorageMem3DMultisampleEXT *cmd);
struct marshal_cmd_NamedBufferStorageMemEXT;
uint32_t _mesa_unmarshal_NamedBufferStorageMemEXT(struct gl_context *ctx, const struct marshal_cmd_NamedBufferStorageMemEXT *cmd);
struct marshal_cmd_TexStorageMem1DEXT;
uint32_t _mesa_unmarshal_TexStorageMem1DEXT(struct gl_context *ctx, const struct marshal_cmd_TexStorageMem1DEXT *cmd);
struct marshal_cmd_TextureStorageMem1DEXT;
uint32_t _mesa_unmarshal_TextureStorageMem1DEXT(struct gl_context *ctx, const struct marshal_cmd_TextureStorageMem1DEXT *cmd);
struct marshal_cmd_DeleteSemaphoresEXT;
uint32_t _mesa_unmarshal_DeleteSemaphoresEXT(struct gl_context *ctx, const struct marshal_cmd_DeleteSemaphoresEXT *cmd);
struct marshal_cmd_SemaphoreParameterui64vEXT;
uint32_t _mesa_unmarshal_SemaphoreParameterui64vEXT(struct gl_context *ctx, const struct marshal_cmd_SemaphoreParameterui64vEXT *cmd);
struct marshal_cmd_WaitSemaphoreEXT;
uint32_t _mesa_unmarshal_WaitSemaphoreEXT(struct gl_context *ctx, const struct marshal_cmd_WaitSemaphoreEXT *cmd);
struct marshal_cmd_ImportMemoryFdEXT;
uint32_t _mesa_unmarshal_ImportMemoryFdEXT(struct gl_context *ctx, const struct marshal_cmd_ImportMemoryFdEXT *cmd);
struct marshal_cmd_ImportSemaphoreFdEXT;
uint32_t _mesa_unmarshal_ImportSemaphoreFdEXT(struct gl_context *ctx, const struct marshal_cmd_ImportSemaphoreFdEXT *cmd);
struct marshal_cmd_ViewportSwizzleNV;
uint32_t _mesa_unmarshal_ViewportSwizzleNV(struct gl_context *ctx, const struct marshal_cmd_ViewportSwizzleNV *cmd);
struct marshal_cmd_Vertex2hNV;
uint32_t _mesa_unmarshal_Vertex2hNV(struct gl_context *ctx, const struct marshal_cmd_Vertex2hNV *cmd);
struct marshal_cmd_Vertex2hvNV;
uint32_t _mesa_unmarshal_Vertex2hvNV(struct gl_context *ctx, const struct marshal_cmd_Vertex2hvNV *cmd);
struct marshal_cmd_Vertex3hNV;
uint32_t _mesa_unmarshal_Vertex3hNV(struct gl_context *ctx, const struct marshal_cmd_Vertex3hNV *cmd);
struct marshal_cmd_Vertex3hvNV;
uint32_t _mesa_unmarshal_Vertex3hvNV(struct gl_context *ctx, const struct marshal_cmd_Vertex3hvNV *cmd);
struct marshal_cmd_Vertex4hNV;
uint32_t _mesa_unmarshal_Vertex4hNV(struct gl_context *ctx, const struct marshal_cmd_Vertex4hNV *cmd);
struct marshal_cmd_Vertex4hvNV;
uint32_t _mesa_unmarshal_Vertex4hvNV(struct gl_context *ctx, const struct marshal_cmd_Vertex4hvNV *cmd);
struct marshal_cmd_Normal3hNV;
uint32_t _mesa_unmarshal_Normal3hNV(struct gl_context *ctx, const struct marshal_cmd_Normal3hNV *cmd);
struct marshal_cmd_Normal3hvNV;
uint32_t _mesa_unmarshal_Normal3hvNV(struct gl_context *ctx, const struct marshal_cmd_Normal3hvNV *cmd);
struct marshal_cmd_Color3hNV;
uint32_t _mesa_unmarshal_Color3hNV(struct gl_context *ctx, const struct marshal_cmd_Color3hNV *cmd);
struct marshal_cmd_Color3hvNV;
uint32_t _mesa_unmarshal_Color3hvNV(struct gl_context *ctx, const struct marshal_cmd_Color3hvNV *cmd);
struct marshal_cmd_Color4hNV;
uint32_t _mesa_unmarshal_Color4hNV(struct gl_context *ctx, const struct marshal_cmd_Color4hNV *cmd);
struct marshal_cmd_Color4hvNV;
uint32_t _mesa_unmarshal_Color4hvNV(struct gl_context *ctx, const struct marshal_cmd_Color4hvNV *cmd);
struct marshal_cmd_TexCoord1hNV;
uint32_t _mesa_unmarshal_TexCoord1hNV(struct gl_context *ctx, const struct marshal_cmd_TexCoord1hNV *cmd);
struct marshal_cmd_TexCoord1hvNV;
uint32_t _mesa_unmarshal_TexCoord1hvNV(struct gl_context *ctx, const struct marshal_cmd_TexCoord1hvNV *cmd);
struct marshal_cmd_TexCoord2hNV;
uint32_t _mesa_unmarshal_TexCoord2hNV(struct gl_context *ctx, const struct marshal_cmd_TexCoord2hNV *cmd);
struct marshal_cmd_TexCoord2hvNV;
uint32_t _mesa_unmarshal_TexCoord2hvNV(struct gl_context *ctx, const struct marshal_cmd_TexCoord2hvNV *cmd);
struct marshal_cmd_TexCoord3hNV;
uint32_t _mesa_unmarshal_TexCoord3hNV(struct gl_context *ctx, const struct marshal_cmd_TexCoord3hNV *cmd);
struct marshal_cmd_TexCoord3hvNV;
uint32_t _mesa_unmarshal_TexCoord3hvNV(struct gl_context *ctx, const struct marshal_cmd_TexCoord3hvNV *cmd);
struct marshal_cmd_TexCoord4hNV;
uint32_t _mesa_unmarshal_TexCoord4hNV(struct gl_context *ctx, const struct marshal_cmd_TexCoord4hNV *cmd);
struct marshal_cmd_TexCoord4hvNV;
uint32_t _mesa_unmarshal_TexCoord4hvNV(struct gl_context *ctx, const struct marshal_cmd_TexCoord4hvNV *cmd);
struct marshal_cmd_MultiTexCoord1hNV;
uint32_t _mesa_unmarshal_MultiTexCoord1hNV(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord1hNV *cmd);
struct marshal_cmd_MultiTexCoord1hvNV;
uint32_t _mesa_unmarshal_MultiTexCoord1hvNV(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord1hvNV *cmd);
struct marshal_cmd_MultiTexCoord2hNV;
uint32_t _mesa_unmarshal_MultiTexCoord2hNV(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord2hNV *cmd);
struct marshal_cmd_MultiTexCoord2hvNV;
uint32_t _mesa_unmarshal_MultiTexCoord2hvNV(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord2hvNV *cmd);
struct marshal_cmd_MultiTexCoord3hNV;
uint32_t _mesa_unmarshal_MultiTexCoord3hNV(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord3hNV *cmd);
struct marshal_cmd_MultiTexCoord3hvNV;
uint32_t _mesa_unmarshal_MultiTexCoord3hvNV(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord3hvNV *cmd);
struct marshal_cmd_MultiTexCoord4hNV;
uint32_t _mesa_unmarshal_MultiTexCoord4hNV(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord4hNV *cmd);
struct marshal_cmd_MultiTexCoord4hvNV;
uint32_t _mesa_unmarshal_MultiTexCoord4hvNV(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord4hvNV *cmd);
struct marshal_cmd_VertexAttrib1hNV;
uint32_t _mesa_unmarshal_VertexAttrib1hNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib1hNV *cmd);
struct marshal_cmd_VertexAttrib1hvNV;
uint32_t _mesa_unmarshal_VertexAttrib1hvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib1hvNV *cmd);
struct marshal_cmd_VertexAttrib2hNV;
uint32_t _mesa_unmarshal_VertexAttrib2hNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib2hNV *cmd);
struct marshal_cmd_VertexAttrib2hvNV;
uint32_t _mesa_unmarshal_VertexAttrib2hvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib2hvNV *cmd);
struct marshal_cmd_VertexAttrib3hNV;
uint32_t _mesa_unmarshal_VertexAttrib3hNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib3hNV *cmd);
struct marshal_cmd_VertexAttrib3hvNV;
uint32_t _mesa_unmarshal_VertexAttrib3hvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib3hvNV *cmd);
struct marshal_cmd_VertexAttrib4hNV;
uint32_t _mesa_unmarshal_VertexAttrib4hNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4hNV *cmd);
struct marshal_cmd_VertexAttrib4hvNV;
uint32_t _mesa_unmarshal_VertexAttrib4hvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4hvNV *cmd);
struct marshal_cmd_VertexAttribs1hvNV;
uint32_t _mesa_unmarshal_VertexAttribs1hvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttribs1hvNV *cmd);
struct marshal_cmd_VertexAttribs2hvNV;
uint32_t _mesa_unmarshal_VertexAttribs2hvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttribs2hvNV *cmd);
struct marshal_cmd_VertexAttribs3hvNV;
uint32_t _mesa_unmarshal_VertexAttribs3hvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttribs3hvNV *cmd);
struct marshal_cmd_VertexAttribs4hvNV;
uint32_t _mesa_unmarshal_VertexAttribs4hvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttribs4hvNV *cmd);
struct marshal_cmd_FogCoordhNV;
uint32_t _mesa_unmarshal_FogCoordhNV(struct gl_context *ctx, const struct marshal_cmd_FogCoordhNV *cmd);
struct marshal_cmd_FogCoordhvNV;
uint32_t _mesa_unmarshal_FogCoordhvNV(struct gl_context *ctx, const struct marshal_cmd_FogCoordhvNV *cmd);
struct marshal_cmd_SecondaryColor3hNV;
uint32_t _mesa_unmarshal_SecondaryColor3hNV(struct gl_context *ctx, const struct marshal_cmd_SecondaryColor3hNV *cmd);
struct marshal_cmd_SecondaryColor3hvNV;
uint32_t _mesa_unmarshal_SecondaryColor3hvNV(struct gl_context *ctx, const struct marshal_cmd_SecondaryColor3hvNV *cmd);
struct marshal_cmd_MemoryBarrierByRegion;
uint32_t _mesa_unmarshal_MemoryBarrierByRegion(struct gl_context *ctx, const struct marshal_cmd_MemoryBarrierByRegion *cmd);
struct marshal_cmd_AlphaFuncx;
uint32_t _mesa_unmarshal_AlphaFuncx(struct gl_context *ctx, const struct marshal_cmd_AlphaFuncx *cmd);
struct marshal_cmd_ClearColorx;
uint32_t _mesa_unmarshal_ClearColorx(struct gl_context *ctx, const struct marshal_cmd_ClearColorx *cmd);
struct marshal_cmd_ClearDepthx;
uint32_t _mesa_unmarshal_ClearDepthx(struct gl_context *ctx, const struct marshal_cmd_ClearDepthx *cmd);
struct marshal_cmd_Color4x;
uint32_t _mesa_unmarshal_Color4x(struct gl_context *ctx, const struct marshal_cmd_Color4x *cmd);
struct marshal_cmd_DepthRangex;
uint32_t _mesa_unmarshal_DepthRangex(struct gl_context *ctx, const struct marshal_cmd_DepthRangex *cmd);
struct marshal_cmd_Fogx;
uint32_t _mesa_unmarshal_Fogx(struct gl_context *ctx, const struct marshal_cmd_Fogx *cmd);
struct marshal_cmd_Fogxv;
uint32_t _mesa_unmarshal_Fogxv(struct gl_context *ctx, const struct marshal_cmd_Fogxv *cmd);
struct marshal_cmd_Frustumx;
uint32_t _mesa_unmarshal_Frustumx(struct gl_context *ctx, const struct marshal_cmd_Frustumx *cmd);
struct marshal_cmd_LightModelx;
uint32_t _mesa_unmarshal_LightModelx(struct gl_context *ctx, const struct marshal_cmd_LightModelx *cmd);
struct marshal_cmd_LightModelxv;
uint32_t _mesa_unmarshal_LightModelxv(struct gl_context *ctx, const struct marshal_cmd_LightModelxv *cmd);
struct marshal_cmd_Lightx;
uint32_t _mesa_unmarshal_Lightx(struct gl_context *ctx, const struct marshal_cmd_Lightx *cmd);
struct marshal_cmd_Lightxv;
uint32_t _mesa_unmarshal_Lightxv(struct gl_context *ctx, const struct marshal_cmd_Lightxv *cmd);
struct marshal_cmd_LineWidthx;
uint32_t _mesa_unmarshal_LineWidthx(struct gl_context *ctx, const struct marshal_cmd_LineWidthx *cmd);
struct marshal_cmd_LoadMatrixx;
uint32_t _mesa_unmarshal_LoadMatrixx(struct gl_context *ctx, const struct marshal_cmd_LoadMatrixx *cmd);
struct marshal_cmd_Materialx;
uint32_t _mesa_unmarshal_Materialx(struct gl_context *ctx, const struct marshal_cmd_Materialx *cmd);
struct marshal_cmd_Materialxv;
uint32_t _mesa_unmarshal_Materialxv(struct gl_context *ctx, const struct marshal_cmd_Materialxv *cmd);
struct marshal_cmd_MultMatrixx;
uint32_t _mesa_unmarshal_MultMatrixx(struct gl_context *ctx, const struct marshal_cmd_MultMatrixx *cmd);
struct marshal_cmd_MultiTexCoord4x;
uint32_t _mesa_unmarshal_MultiTexCoord4x(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord4x *cmd);
struct marshal_cmd_Normal3x;
uint32_t _mesa_unmarshal_Normal3x(struct gl_context *ctx, const struct marshal_cmd_Normal3x *cmd);
struct marshal_cmd_Orthox;
uint32_t _mesa_unmarshal_Orthox(struct gl_context *ctx, const struct marshal_cmd_Orthox *cmd);
struct marshal_cmd_PointSizex;
uint32_t _mesa_unmarshal_PointSizex(struct gl_context *ctx, const struct marshal_cmd_PointSizex *cmd);
struct marshal_cmd_PolygonOffsetx;
uint32_t _mesa_unmarshal_PolygonOffsetx(struct gl_context *ctx, const struct marshal_cmd_PolygonOffsetx *cmd);
struct marshal_cmd_Rotatex;
uint32_t _mesa_unmarshal_Rotatex(struct gl_context *ctx, const struct marshal_cmd_Rotatex *cmd);
struct marshal_cmd_SampleCoveragex;
uint32_t _mesa_unmarshal_SampleCoveragex(struct gl_context *ctx, const struct marshal_cmd_SampleCoveragex *cmd);
struct marshal_cmd_Scalex;
uint32_t _mesa_unmarshal_Scalex(struct gl_context *ctx, const struct marshal_cmd_Scalex *cmd);
struct marshal_cmd_TexEnvx;
uint32_t _mesa_unmarshal_TexEnvx(struct gl_context *ctx, const struct marshal_cmd_TexEnvx *cmd);
struct marshal_cmd_TexEnvxv;
uint32_t _mesa_unmarshal_TexEnvxv(struct gl_context *ctx, const struct marshal_cmd_TexEnvxv *cmd);
struct marshal_cmd_TexParameterx;
uint32_t _mesa_unmarshal_TexParameterx(struct gl_context *ctx, const struct marshal_cmd_TexParameterx *cmd);
struct marshal_cmd_Translatex;
uint32_t _mesa_unmarshal_Translatex(struct gl_context *ctx, const struct marshal_cmd_Translatex *cmd);
struct marshal_cmd_ClipPlanex;
uint32_t _mesa_unmarshal_ClipPlanex(struct gl_context *ctx, const struct marshal_cmd_ClipPlanex *cmd);
struct marshal_cmd_PointParameterx;
uint32_t _mesa_unmarshal_PointParameterx(struct gl_context *ctx, const struct marshal_cmd_PointParameterx *cmd);
struct marshal_cmd_PointParameterxv;
uint32_t _mesa_unmarshal_PointParameterxv(struct gl_context *ctx, const struct marshal_cmd_PointParameterxv *cmd);
struct marshal_cmd_TexParameterxv;
uint32_t _mesa_unmarshal_TexParameterxv(struct gl_context *ctx, const struct marshal_cmd_TexParameterxv *cmd);
struct marshal_cmd_TexGenxOES;
uint32_t _mesa_unmarshal_TexGenxOES(struct gl_context *ctx, const struct marshal_cmd_TexGenxOES *cmd);
struct marshal_cmd_TexGenxvOES;
uint32_t _mesa_unmarshal_TexGenxvOES(struct gl_context *ctx, const struct marshal_cmd_TexGenxvOES *cmd);
struct marshal_cmd_ClipPlanef;
uint32_t _mesa_unmarshal_ClipPlanef(struct gl_context *ctx, const struct marshal_cmd_ClipPlanef *cmd);
struct marshal_cmd_Frustumf;
uint32_t _mesa_unmarshal_Frustumf(struct gl_context *ctx, const struct marshal_cmd_Frustumf *cmd);
struct marshal_cmd_Orthof;
uint32_t _mesa_unmarshal_Orthof(struct gl_context *ctx, const struct marshal_cmd_Orthof *cmd);
struct marshal_cmd_DrawTexiOES;
uint32_t _mesa_unmarshal_DrawTexiOES(struct gl_context *ctx, const struct marshal_cmd_DrawTexiOES *cmd);
struct marshal_cmd_DrawTexivOES;
uint32_t _mesa_unmarshal_DrawTexivOES(struct gl_context *ctx, const struct marshal_cmd_DrawTexivOES *cmd);
struct marshal_cmd_DrawTexfOES;
uint32_t _mesa_unmarshal_DrawTexfOES(struct gl_context *ctx, const struct marshal_cmd_DrawTexfOES *cmd);
struct marshal_cmd_DrawTexfvOES;
uint32_t _mesa_unmarshal_DrawTexfvOES(struct gl_context *ctx, const struct marshal_cmd_DrawTexfvOES *cmd);
struct marshal_cmd_DrawTexsOES;
uint32_t _mesa_unmarshal_DrawTexsOES(struct gl_context *ctx, const struct marshal_cmd_DrawTexsOES *cmd);
struct marshal_cmd_DrawTexsvOES;
uint32_t _mesa_unmarshal_DrawTexsvOES(struct gl_context *ctx, const struct marshal_cmd_DrawTexsvOES *cmd);
struct marshal_cmd_DrawTexxOES;
uint32_t _mesa_unmarshal_DrawTexxOES(struct gl_context *ctx, const struct marshal_cmd_DrawTexxOES *cmd);
struct marshal_cmd_DrawTexxvOES;
uint32_t _mesa_unmarshal_DrawTexxvOES(struct gl_context *ctx, const struct marshal_cmd_DrawTexxvOES *cmd);
struct marshal_cmd_PointSizePointerOES;
uint32_t _mesa_unmarshal_PointSizePointerOES(struct gl_context *ctx, const struct marshal_cmd_PointSizePointerOES *cmd);
struct marshal_cmd_DiscardFramebufferEXT;
uint32_t _mesa_unmarshal_DiscardFramebufferEXT(struct gl_context *ctx, const struct marshal_cmd_DiscardFramebufferEXT *cmd);
struct marshal_cmd_FramebufferTexture2DMultisampleEXT;
uint32_t _mesa_unmarshal_FramebufferTexture2DMultisampleEXT(struct gl_context *ctx, const struct marshal_cmd_FramebufferTexture2DMultisampleEXT *cmd);
struct marshal_cmd_DepthRangeArrayfvOES;
uint32_t _mesa_unmarshal_DepthRangeArrayfvOES(struct gl_context *ctx, const struct marshal_cmd_DepthRangeArrayfvOES *cmd);
struct marshal_cmd_DepthRangeIndexedfOES;
uint32_t _mesa_unmarshal_DepthRangeIndexedfOES(struct gl_context *ctx, const struct marshal_cmd_DepthRangeIndexedfOES *cmd);
struct marshal_cmd_FramebufferParameteriMESA;
uint32_t _mesa_unmarshal_FramebufferParameteriMESA(struct gl_context *ctx, const struct marshal_cmd_FramebufferParameteriMESA *cmd);

#endif /* MARSHAL_GENERATED_H */

