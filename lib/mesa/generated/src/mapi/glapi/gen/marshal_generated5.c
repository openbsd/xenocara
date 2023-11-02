/* DO NOT EDIT - This file generated automatically by gl_marshal.py script */

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


#include "context.h"
#include "glthread_marshal.h"
#include "bufferobj.h"
#include "dispatch.h"

#define COMPAT (ctx->API != API_OPENGL_CORE)

UNUSED static inline int safe_mul(int a, int b)
{
    if (a < 0 || b < 0) return -1;
    if (a == 0 || b == 0) return 0;
    if (a > INT_MAX / b) return -1;
    return a * b;
}

/* GetTextureLevelParameterfv: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetTextureLevelParameterfv(GLuint texture, GLint level, GLenum pname, GLfloat *params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetTextureLevelParameterfv");
   CALL_GetTextureLevelParameterfv(ctx->Dispatch.Current, (texture, level, pname, params));
}


/* GetTextureLevelParameteriv: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetTextureLevelParameteriv(GLuint texture, GLint level, GLenum pname, GLint *params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetTextureLevelParameteriv");
   CALL_GetTextureLevelParameteriv(ctx->Dispatch.Current, (texture, level, pname, params));
}


/* GetTextureParameterfv: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetTextureParameterfv(GLuint texture, GLenum pname, GLfloat *params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetTextureParameterfv");
   CALL_GetTextureParameterfv(ctx->Dispatch.Current, (texture, pname, params));
}


/* GetTextureParameterIiv: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetTextureParameterIiv(GLuint texture, GLenum pname, GLint *params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetTextureParameterIiv");
   CALL_GetTextureParameterIiv(ctx->Dispatch.Current, (texture, pname, params));
}


/* GetTextureParameterIuiv: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetTextureParameterIuiv(GLuint texture, GLenum pname, GLuint *params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetTextureParameterIuiv");
   CALL_GetTextureParameterIuiv(ctx->Dispatch.Current, (texture, pname, params));
}


/* GetTextureParameteriv: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetTextureParameteriv(GLuint texture, GLenum pname, GLint *params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetTextureParameteriv");
   CALL_GetTextureParameteriv(ctx->Dispatch.Current, (texture, pname, params));
}


/* CreateVertexArrays: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_CreateVertexArrays(GLsizei n, GLuint *arrays)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "CreateVertexArrays");
   CALL_CreateVertexArrays(ctx->Dispatch.Current, (n, arrays));
   _mesa_glthread_GenVertexArrays(ctx, n, arrays);
}


/* DisableVertexArrayAttrib: marshalled asynchronously */
struct marshal_cmd_DisableVertexArrayAttrib
{
   struct marshal_cmd_base cmd_base;
   GLuint vaobj;
   GLuint index;
};
uint32_t
_mesa_unmarshal_DisableVertexArrayAttrib(struct gl_context *ctx, const struct marshal_cmd_DisableVertexArrayAttrib *restrict cmd)
{
   GLuint vaobj = cmd->vaobj;
   GLuint index = cmd->index;
   CALL_DisableVertexArrayAttrib(ctx->Dispatch.Current, (vaobj, index));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_DisableVertexArrayAttrib), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_DisableVertexArrayAttrib(GLuint vaobj, GLuint index)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_DisableVertexArrayAttrib);
   struct marshal_cmd_DisableVertexArrayAttrib *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_DisableVertexArrayAttrib, cmd_size);
   cmd->vaobj = vaobj;
   cmd->index = index;
   if (COMPAT) _mesa_glthread_ClientState(ctx, &vaobj, VERT_ATTRIB_GENERIC(index), false);
}


/* EnableVertexArrayAttrib: marshalled asynchronously */
struct marshal_cmd_EnableVertexArrayAttrib
{
   struct marshal_cmd_base cmd_base;
   GLuint vaobj;
   GLuint index;
};
uint32_t
_mesa_unmarshal_EnableVertexArrayAttrib(struct gl_context *ctx, const struct marshal_cmd_EnableVertexArrayAttrib *restrict cmd)
{
   GLuint vaobj = cmd->vaobj;
   GLuint index = cmd->index;
   CALL_EnableVertexArrayAttrib(ctx->Dispatch.Current, (vaobj, index));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_EnableVertexArrayAttrib), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_EnableVertexArrayAttrib(GLuint vaobj, GLuint index)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_EnableVertexArrayAttrib);
   struct marshal_cmd_EnableVertexArrayAttrib *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_EnableVertexArrayAttrib, cmd_size);
   cmd->vaobj = vaobj;
   cmd->index = index;
   if (COMPAT) _mesa_glthread_ClientState(ctx, &vaobj, VERT_ATTRIB_GENERIC(index), true);
}


/* VertexArrayElementBuffer: marshalled asynchronously */
struct marshal_cmd_VertexArrayElementBuffer
{
   struct marshal_cmd_base cmd_base;
   GLuint vaobj;
   GLuint buffer;
};
uint32_t
_mesa_unmarshal_VertexArrayElementBuffer(struct gl_context *ctx, const struct marshal_cmd_VertexArrayElementBuffer *restrict cmd)
{
   GLuint vaobj = cmd->vaobj;
   GLuint buffer = cmd->buffer;
   CALL_VertexArrayElementBuffer(ctx->Dispatch.Current, (vaobj, buffer));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexArrayElementBuffer), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_VertexArrayElementBuffer(GLuint vaobj, GLuint buffer)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexArrayElementBuffer);
   struct marshal_cmd_VertexArrayElementBuffer *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexArrayElementBuffer, cmd_size);
   cmd->vaobj = vaobj;
   cmd->buffer = buffer;
   _mesa_glthread_DSAElementBuffer(ctx, vaobj, buffer);
}


/* VertexArrayVertexBuffer: marshalled asynchronously */
struct marshal_cmd_VertexArrayVertexBuffer
{
   struct marshal_cmd_base cmd_base;
   GLuint vaobj;
   GLuint bindingindex;
   GLuint buffer;
   GLsizei stride;
   GLintptr offset;
};
uint32_t
_mesa_unmarshal_VertexArrayVertexBuffer(struct gl_context *ctx, const struct marshal_cmd_VertexArrayVertexBuffer *restrict cmd)
{
   GLuint vaobj = cmd->vaobj;
   GLuint bindingindex = cmd->bindingindex;
   GLuint buffer = cmd->buffer;
   GLintptr offset = cmd->offset;
   GLsizei stride = cmd->stride;
   CALL_VertexArrayVertexBuffer(ctx->Dispatch.Current, (vaobj, bindingindex, buffer, offset, stride));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexArrayVertexBuffer), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_VertexArrayVertexBuffer(GLuint vaobj, GLuint bindingindex, GLuint buffer, GLintptr offset, GLsizei stride)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexArrayVertexBuffer);
   struct marshal_cmd_VertexArrayVertexBuffer *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexArrayVertexBuffer, cmd_size);
   cmd->vaobj = vaobj;
   cmd->bindingindex = bindingindex;
   cmd->buffer = buffer;
   cmd->offset = offset;
   cmd->stride = stride;
   if (COMPAT) _mesa_glthread_DSAVertexBuffer(ctx, vaobj, bindingindex, buffer, offset, stride);
}


/* VertexArrayVertexBuffers: marshalled asynchronously */
struct marshal_cmd_VertexArrayVertexBuffers
{
   struct marshal_cmd_base cmd_base;
   GLuint vaobj;
   GLuint first;
   GLsizei count;
   /* Next safe_mul(count, 1 * sizeof(GLuint)) bytes are GLuint buffers[count] */
   /* Next safe_mul(count, 1 * sizeof(GLintptr)) bytes are GLintptr offsets[count] */
   /* Next safe_mul(count, 1 * sizeof(GLsizei)) bytes are GLsizei strides[count] */
};
uint32_t
_mesa_unmarshal_VertexArrayVertexBuffers(struct gl_context *ctx, const struct marshal_cmd_VertexArrayVertexBuffers *restrict cmd)
{
   GLuint vaobj = cmd->vaobj;
   GLuint first = cmd->first;
   GLsizei count = cmd->count;
   GLuint *buffers;
   GLintptr *offsets;
   GLsizei *strides;
   const char *variable_data = (const char *) (cmd + 1);
   buffers = (GLuint *) variable_data;
   variable_data += count * 1 * sizeof(GLuint);
   offsets = (GLintptr *) variable_data;
   variable_data += count * 1 * sizeof(GLintptr);
   strides = (GLsizei *) variable_data;
   CALL_VertexArrayVertexBuffers(ctx->Dispatch.Current, (vaobj, first, count, buffers, offsets, strides));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_VertexArrayVertexBuffers(GLuint vaobj, GLuint first, GLsizei count, const GLuint *buffers, const GLintptr *offsets, const GLsizei *strides)
{
   GET_CURRENT_CONTEXT(ctx);
   int buffers_size = safe_mul(count, 1 * sizeof(GLuint));
   int offsets_size = safe_mul(count, 1 * sizeof(GLintptr));
   int strides_size = safe_mul(count, 1 * sizeof(GLsizei));
   int cmd_size = sizeof(struct marshal_cmd_VertexArrayVertexBuffers) + buffers_size + offsets_size + strides_size;
   struct marshal_cmd_VertexArrayVertexBuffers *cmd;
   if (unlikely(buffers_size < 0 || (buffers_size > 0 && !buffers) || offsets_size < 0 || (offsets_size > 0 && !offsets) || strides_size < 0 || (strides_size > 0 && !strides) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "VertexArrayVertexBuffers");
      CALL_VertexArrayVertexBuffers(ctx->Dispatch.Current, (vaobj, first, count, buffers, offsets, strides));
      if (COMPAT) _mesa_glthread_DSAVertexBuffers(ctx, vaobj, first, count, buffers, offsets, strides);
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexArrayVertexBuffers, cmd_size);
   cmd->vaobj = vaobj;
   cmd->first = first;
   cmd->count = count;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, buffers, buffers_size);
   variable_data += buffers_size;
   memcpy(variable_data, offsets, offsets_size);
   variable_data += offsets_size;
   memcpy(variable_data, strides, strides_size);
   if (COMPAT) _mesa_glthread_DSAVertexBuffers(ctx, vaobj, first, count, buffers, offsets, strides);
}


/* VertexArrayAttribFormat: marshalled asynchronously */
struct marshal_cmd_VertexArrayAttribFormat
{
   struct marshal_cmd_base cmd_base;
   GLboolean normalized;
   GLenum16 type;
   GLuint vaobj;
   GLuint attribindex;
   GLint size;
   GLuint relativeoffset;
};
uint32_t
_mesa_unmarshal_VertexArrayAttribFormat(struct gl_context *ctx, const struct marshal_cmd_VertexArrayAttribFormat *restrict cmd)
{
   GLuint vaobj = cmd->vaobj;
   GLuint attribindex = cmd->attribindex;
   GLint size = cmd->size;
   GLenum type = cmd->type;
   GLboolean normalized = cmd->normalized;
   GLuint relativeoffset = cmd->relativeoffset;
   CALL_VertexArrayAttribFormat(ctx->Dispatch.Current, (vaobj, attribindex, size, type, normalized, relativeoffset));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexArrayAttribFormat), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_VertexArrayAttribFormat(GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexArrayAttribFormat);
   struct marshal_cmd_VertexArrayAttribFormat *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexArrayAttribFormat, cmd_size);
   cmd->vaobj = vaobj;
   cmd->attribindex = attribindex;
   cmd->size = size;
   cmd->type = MIN2(type, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->normalized = normalized;
   cmd->relativeoffset = relativeoffset;
   if (COMPAT) _mesa_glthread_DSAAttribFormat(ctx, vaobj, attribindex, MESA_PACK_VFORMAT(type, size, normalized, 0, 0), relativeoffset);
}


/* VertexArrayAttribIFormat: marshalled asynchronously */
struct marshal_cmd_VertexArrayAttribIFormat
{
   struct marshal_cmd_base cmd_base;
   GLenum16 type;
   GLuint vaobj;
   GLuint attribindex;
   GLint size;
   GLuint relativeoffset;
};
uint32_t
_mesa_unmarshal_VertexArrayAttribIFormat(struct gl_context *ctx, const struct marshal_cmd_VertexArrayAttribIFormat *restrict cmd)
{
   GLuint vaobj = cmd->vaobj;
   GLuint attribindex = cmd->attribindex;
   GLint size = cmd->size;
   GLenum type = cmd->type;
   GLuint relativeoffset = cmd->relativeoffset;
   CALL_VertexArrayAttribIFormat(ctx->Dispatch.Current, (vaobj, attribindex, size, type, relativeoffset));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexArrayAttribIFormat), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_VertexArrayAttribIFormat(GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexArrayAttribIFormat);
   struct marshal_cmd_VertexArrayAttribIFormat *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexArrayAttribIFormat, cmd_size);
   cmd->vaobj = vaobj;
   cmd->attribindex = attribindex;
   cmd->size = size;
   cmd->type = MIN2(type, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->relativeoffset = relativeoffset;
   if (COMPAT) _mesa_glthread_DSAAttribFormat(ctx, vaobj, attribindex, MESA_PACK_VFORMAT(type, size, 0, 1, 0), relativeoffset);
}


/* VertexArrayAttribLFormat: marshalled asynchronously */
struct marshal_cmd_VertexArrayAttribLFormat
{
   struct marshal_cmd_base cmd_base;
   GLenum16 type;
   GLuint vaobj;
   GLuint attribindex;
   GLint size;
   GLuint relativeoffset;
};
uint32_t
_mesa_unmarshal_VertexArrayAttribLFormat(struct gl_context *ctx, const struct marshal_cmd_VertexArrayAttribLFormat *restrict cmd)
{
   GLuint vaobj = cmd->vaobj;
   GLuint attribindex = cmd->attribindex;
   GLint size = cmd->size;
   GLenum type = cmd->type;
   GLuint relativeoffset = cmd->relativeoffset;
   CALL_VertexArrayAttribLFormat(ctx->Dispatch.Current, (vaobj, attribindex, size, type, relativeoffset));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexArrayAttribLFormat), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_VertexArrayAttribLFormat(GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexArrayAttribLFormat);
   struct marshal_cmd_VertexArrayAttribLFormat *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexArrayAttribLFormat, cmd_size);
   cmd->vaobj = vaobj;
   cmd->attribindex = attribindex;
   cmd->size = size;
   cmd->type = MIN2(type, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->relativeoffset = relativeoffset;
   if (COMPAT) _mesa_glthread_DSAAttribFormat(ctx, vaobj, attribindex, MESA_PACK_VFORMAT(type, size, 0, 0, 1), relativeoffset);
}


/* VertexArrayAttribBinding: marshalled asynchronously */
struct marshal_cmd_VertexArrayAttribBinding
{
   struct marshal_cmd_base cmd_base;
   GLuint vaobj;
   GLuint attribindex;
   GLuint bindingindex;
};
uint32_t
_mesa_unmarshal_VertexArrayAttribBinding(struct gl_context *ctx, const struct marshal_cmd_VertexArrayAttribBinding *restrict cmd)
{
   GLuint vaobj = cmd->vaobj;
   GLuint attribindex = cmd->attribindex;
   GLuint bindingindex = cmd->bindingindex;
   CALL_VertexArrayAttribBinding(ctx->Dispatch.Current, (vaobj, attribindex, bindingindex));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexArrayAttribBinding), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_VertexArrayAttribBinding(GLuint vaobj, GLuint attribindex, GLuint bindingindex)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexArrayAttribBinding);
   struct marshal_cmd_VertexArrayAttribBinding *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexArrayAttribBinding, cmd_size);
   cmd->vaobj = vaobj;
   cmd->attribindex = attribindex;
   cmd->bindingindex = bindingindex;
   if (COMPAT) _mesa_glthread_DSAAttribBinding(ctx, vaobj, attribindex, bindingindex);
}


/* VertexArrayBindingDivisor: marshalled asynchronously */
struct marshal_cmd_VertexArrayBindingDivisor
{
   struct marshal_cmd_base cmd_base;
   GLuint vaobj;
   GLuint bindingindex;
   GLuint divisor;
};
uint32_t
_mesa_unmarshal_VertexArrayBindingDivisor(struct gl_context *ctx, const struct marshal_cmd_VertexArrayBindingDivisor *restrict cmd)
{
   GLuint vaobj = cmd->vaobj;
   GLuint bindingindex = cmd->bindingindex;
   GLuint divisor = cmd->divisor;
   CALL_VertexArrayBindingDivisor(ctx->Dispatch.Current, (vaobj, bindingindex, divisor));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexArrayBindingDivisor), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_VertexArrayBindingDivisor(GLuint vaobj, GLuint bindingindex, GLuint divisor)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexArrayBindingDivisor);
   struct marshal_cmd_VertexArrayBindingDivisor *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexArrayBindingDivisor, cmd_size);
   cmd->vaobj = vaobj;
   cmd->bindingindex = bindingindex;
   cmd->divisor = divisor;
   if (COMPAT) _mesa_glthread_DSABindingDivisor(ctx, vaobj, bindingindex, divisor);
}


/* GetVertexArrayiv: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetVertexArrayiv(GLuint vaobj, GLenum pname, GLint *param)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetVertexArrayiv");
   CALL_GetVertexArrayiv(ctx->Dispatch.Current, (vaobj, pname, param));
}


/* GetVertexArrayIndexediv: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetVertexArrayIndexediv(GLuint vaobj, GLuint index, GLenum pname, GLint *param)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetVertexArrayIndexediv");
   CALL_GetVertexArrayIndexediv(ctx->Dispatch.Current, (vaobj, index, pname, param));
}


/* GetVertexArrayIndexed64iv: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetVertexArrayIndexed64iv(GLuint vaobj, GLuint index, GLenum pname, GLint64 *param)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetVertexArrayIndexed64iv");
   CALL_GetVertexArrayIndexed64iv(ctx->Dispatch.Current, (vaobj, index, pname, param));
}


/* CreateSamplers: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_CreateSamplers(GLsizei n, GLuint *samplers)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "CreateSamplers");
   CALL_CreateSamplers(ctx->Dispatch.Current, (n, samplers));
}


/* CreateProgramPipelines: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_CreateProgramPipelines(GLsizei n, GLuint *pipelines)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "CreateProgramPipelines");
   CALL_CreateProgramPipelines(ctx->Dispatch.Current, (n, pipelines));
}


/* CreateQueries: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_CreateQueries(GLenum target, GLsizei n, GLuint *ids)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "CreateQueries");
   CALL_CreateQueries(ctx->Dispatch.Current, (target, n, ids));
}


/* GetQueryBufferObjectiv: marshalled asynchronously */
struct marshal_cmd_GetQueryBufferObjectiv
{
   struct marshal_cmd_base cmd_base;
   GLenum16 pname;
   GLuint id;
   GLuint buffer;
   GLintptr offset;
};
uint32_t
_mesa_unmarshal_GetQueryBufferObjectiv(struct gl_context *ctx, const struct marshal_cmd_GetQueryBufferObjectiv *restrict cmd)
{
   GLuint id = cmd->id;
   GLuint buffer = cmd->buffer;
   GLenum pname = cmd->pname;
   GLintptr offset = cmd->offset;
   CALL_GetQueryBufferObjectiv(ctx->Dispatch.Current, (id, buffer, pname, offset));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_GetQueryBufferObjectiv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_GetQueryBufferObjectiv(GLuint id, GLuint buffer, GLenum pname, GLintptr offset)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_GetQueryBufferObjectiv);
   struct marshal_cmd_GetQueryBufferObjectiv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_GetQueryBufferObjectiv, cmd_size);
   cmd->id = id;
   cmd->buffer = buffer;
   cmd->pname = MIN2(pname, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->offset = offset;
}


/* GetQueryBufferObjectuiv: marshalled asynchronously */
struct marshal_cmd_GetQueryBufferObjectuiv
{
   struct marshal_cmd_base cmd_base;
   GLenum16 pname;
   GLuint id;
   GLuint buffer;
   GLintptr offset;
};
uint32_t
_mesa_unmarshal_GetQueryBufferObjectuiv(struct gl_context *ctx, const struct marshal_cmd_GetQueryBufferObjectuiv *restrict cmd)
{
   GLuint id = cmd->id;
   GLuint buffer = cmd->buffer;
   GLenum pname = cmd->pname;
   GLintptr offset = cmd->offset;
   CALL_GetQueryBufferObjectuiv(ctx->Dispatch.Current, (id, buffer, pname, offset));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_GetQueryBufferObjectuiv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_GetQueryBufferObjectuiv(GLuint id, GLuint buffer, GLenum pname, GLintptr offset)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_GetQueryBufferObjectuiv);
   struct marshal_cmd_GetQueryBufferObjectuiv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_GetQueryBufferObjectuiv, cmd_size);
   cmd->id = id;
   cmd->buffer = buffer;
   cmd->pname = MIN2(pname, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->offset = offset;
}


/* GetQueryBufferObjecti64v: marshalled asynchronously */
struct marshal_cmd_GetQueryBufferObjecti64v
{
   struct marshal_cmd_base cmd_base;
   GLenum16 pname;
   GLuint id;
   GLuint buffer;
   GLintptr offset;
};
uint32_t
_mesa_unmarshal_GetQueryBufferObjecti64v(struct gl_context *ctx, const struct marshal_cmd_GetQueryBufferObjecti64v *restrict cmd)
{
   GLuint id = cmd->id;
   GLuint buffer = cmd->buffer;
   GLenum pname = cmd->pname;
   GLintptr offset = cmd->offset;
   CALL_GetQueryBufferObjecti64v(ctx->Dispatch.Current, (id, buffer, pname, offset));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_GetQueryBufferObjecti64v), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_GetQueryBufferObjecti64v(GLuint id, GLuint buffer, GLenum pname, GLintptr offset)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_GetQueryBufferObjecti64v);
   struct marshal_cmd_GetQueryBufferObjecti64v *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_GetQueryBufferObjecti64v, cmd_size);
   cmd->id = id;
   cmd->buffer = buffer;
   cmd->pname = MIN2(pname, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->offset = offset;
}


/* GetQueryBufferObjectui64v: marshalled asynchronously */
struct marshal_cmd_GetQueryBufferObjectui64v
{
   struct marshal_cmd_base cmd_base;
   GLenum16 pname;
   GLuint id;
   GLuint buffer;
   GLintptr offset;
};
uint32_t
_mesa_unmarshal_GetQueryBufferObjectui64v(struct gl_context *ctx, const struct marshal_cmd_GetQueryBufferObjectui64v *restrict cmd)
{
   GLuint id = cmd->id;
   GLuint buffer = cmd->buffer;
   GLenum pname = cmd->pname;
   GLintptr offset = cmd->offset;
   CALL_GetQueryBufferObjectui64v(ctx->Dispatch.Current, (id, buffer, pname, offset));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_GetQueryBufferObjectui64v), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_GetQueryBufferObjectui64v(GLuint id, GLuint buffer, GLenum pname, GLintptr offset)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_GetQueryBufferObjectui64v);
   struct marshal_cmd_GetQueryBufferObjectui64v *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_GetQueryBufferObjectui64v, cmd_size);
   cmd->id = id;
   cmd->buffer = buffer;
   cmd->pname = MIN2(pname, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->offset = offset;
}


/* GetTextureSubImage: marshalled asynchronously */
struct marshal_cmd_GetTextureSubImage
{
   struct marshal_cmd_base cmd_base;
   GLenum16 format;
   GLenum16 type;
   GLuint texture;
   GLint level;
   GLint xoffset;
   GLint yoffset;
   GLint zoffset;
   GLsizei width;
   GLsizei height;
   GLsizei depth;
   GLsizei bufSize;
   GLvoid * pixels;
};
uint32_t
_mesa_unmarshal_GetTextureSubImage(struct gl_context *ctx, const struct marshal_cmd_GetTextureSubImage *restrict cmd)
{
   GLuint texture = cmd->texture;
   GLint level = cmd->level;
   GLint xoffset = cmd->xoffset;
   GLint yoffset = cmd->yoffset;
   GLint zoffset = cmd->zoffset;
   GLsizei width = cmd->width;
   GLsizei height = cmd->height;
   GLsizei depth = cmd->depth;
   GLenum format = cmd->format;
   GLenum type = cmd->type;
   GLsizei bufSize = cmd->bufSize;
   GLvoid * pixels = cmd->pixels;
   CALL_GetTextureSubImage(ctx->Dispatch.Current, (texture, level, xoffset, yoffset, zoffset, width, height, depth, format, type, bufSize, pixels));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_GetTextureSubImage), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_GetTextureSubImage(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, GLsizei bufSize, GLvoid *pixels)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_GetTextureSubImage);
   struct marshal_cmd_GetTextureSubImage *cmd;
   if (_mesa_glthread_has_no_pack_buffer(ctx)) {
      _mesa_glthread_finish_before(ctx, "GetTextureSubImage");
      CALL_GetTextureSubImage(ctx->Dispatch.Current, (texture, level, xoffset, yoffset, zoffset, width, height, depth, format, type, bufSize, pixels));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_GetTextureSubImage, cmd_size);
   cmd->texture = texture;
   cmd->level = level;
   cmd->xoffset = xoffset;
   cmd->yoffset = yoffset;
   cmd->zoffset = zoffset;
   cmd->width = width;
   cmd->height = height;
   cmd->depth = depth;
   cmd->format = MIN2(format, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->type = MIN2(type, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->bufSize = bufSize;
   cmd->pixels = pixels;
}


/* GetCompressedTextureSubImage: marshalled asynchronously */
struct marshal_cmd_GetCompressedTextureSubImage
{
   struct marshal_cmd_base cmd_base;
   GLuint texture;
   GLint level;
   GLint xoffset;
   GLint yoffset;
   GLint zoffset;
   GLsizei width;
   GLsizei height;
   GLsizei depth;
   GLsizei bufSize;
   GLvoid * pixels;
};
uint32_t
_mesa_unmarshal_GetCompressedTextureSubImage(struct gl_context *ctx, const struct marshal_cmd_GetCompressedTextureSubImage *restrict cmd)
{
   GLuint texture = cmd->texture;
   GLint level = cmd->level;
   GLint xoffset = cmd->xoffset;
   GLint yoffset = cmd->yoffset;
   GLint zoffset = cmd->zoffset;
   GLsizei width = cmd->width;
   GLsizei height = cmd->height;
   GLsizei depth = cmd->depth;
   GLsizei bufSize = cmd->bufSize;
   GLvoid * pixels = cmd->pixels;
   CALL_GetCompressedTextureSubImage(ctx->Dispatch.Current, (texture, level, xoffset, yoffset, zoffset, width, height, depth, bufSize, pixels));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_GetCompressedTextureSubImage), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_GetCompressedTextureSubImage(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLsizei bufSize, GLvoid *pixels)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_GetCompressedTextureSubImage);
   struct marshal_cmd_GetCompressedTextureSubImage *cmd;
   if (_mesa_glthread_has_no_pack_buffer(ctx)) {
      _mesa_glthread_finish_before(ctx, "GetCompressedTextureSubImage");
      CALL_GetCompressedTextureSubImage(ctx->Dispatch.Current, (texture, level, xoffset, yoffset, zoffset, width, height, depth, bufSize, pixels));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_GetCompressedTextureSubImage, cmd_size);
   cmd->texture = texture;
   cmd->level = level;
   cmd->xoffset = xoffset;
   cmd->yoffset = yoffset;
   cmd->zoffset = zoffset;
   cmd->width = width;
   cmd->height = height;
   cmd->depth = depth;
   cmd->bufSize = bufSize;
   cmd->pixels = pixels;
}


/* TextureBarrierNV: marshalled asynchronously */
struct marshal_cmd_TextureBarrierNV
{
   struct marshal_cmd_base cmd_base;
};
uint32_t
_mesa_unmarshal_TextureBarrierNV(struct gl_context *ctx, const struct marshal_cmd_TextureBarrierNV *restrict cmd)
{
   CALL_TextureBarrierNV(ctx->Dispatch.Current, ());
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_TextureBarrierNV), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_TextureBarrierNV(void)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TextureBarrierNV);
   struct marshal_cmd_TextureBarrierNV *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TextureBarrierNV, cmd_size);
   (void) cmd;
}


/* BufferPageCommitmentARB: marshalled asynchronously */
struct marshal_cmd_BufferPageCommitmentARB
{
   struct marshal_cmd_base cmd_base;
   GLboolean commit;
   GLenum16 target;
   GLintptr offset;
   GLsizeiptr size;
};
uint32_t
_mesa_unmarshal_BufferPageCommitmentARB(struct gl_context *ctx, const struct marshal_cmd_BufferPageCommitmentARB *restrict cmd)
{
   GLenum target = cmd->target;
   GLintptr offset = cmd->offset;
   GLsizeiptr size = cmd->size;
   GLboolean commit = cmd->commit;
   CALL_BufferPageCommitmentARB(ctx->Dispatch.Current, (target, offset, size, commit));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_BufferPageCommitmentARB), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_BufferPageCommitmentARB(GLenum target, GLintptr offset, GLsizeiptr size, GLboolean commit)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_BufferPageCommitmentARB);
   struct marshal_cmd_BufferPageCommitmentARB *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_BufferPageCommitmentARB, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->offset = offset;
   cmd->size = size;
   cmd->commit = commit;
}


/* NamedBufferPageCommitmentEXT: marshalled asynchronously */
struct marshal_cmd_NamedBufferPageCommitmentEXT
{
   struct marshal_cmd_base cmd_base;
   GLboolean commit;
   GLuint buffer;
   GLintptr offset;
   GLsizeiptr size;
};
uint32_t
_mesa_unmarshal_NamedBufferPageCommitmentEXT(struct gl_context *ctx, const struct marshal_cmd_NamedBufferPageCommitmentEXT *restrict cmd)
{
   GLuint buffer = cmd->buffer;
   GLintptr offset = cmd->offset;
   GLsizeiptr size = cmd->size;
   GLboolean commit = cmd->commit;
   CALL_NamedBufferPageCommitmentEXT(ctx->Dispatch.Current, (buffer, offset, size, commit));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_NamedBufferPageCommitmentEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_NamedBufferPageCommitmentEXT(GLuint buffer, GLintptr offset, GLsizeiptr size, GLboolean commit)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_NamedBufferPageCommitmentEXT);
   struct marshal_cmd_NamedBufferPageCommitmentEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_NamedBufferPageCommitmentEXT, cmd_size);
   cmd->buffer = buffer;
   cmd->offset = offset;
   cmd->size = size;
   cmd->commit = commit;
}


/* NamedBufferPageCommitmentARB: marshalled asynchronously */
struct marshal_cmd_NamedBufferPageCommitmentARB
{
   struct marshal_cmd_base cmd_base;
   GLboolean commit;
   GLuint buffer;
   GLintptr offset;
   GLsizeiptr size;
};
uint32_t
_mesa_unmarshal_NamedBufferPageCommitmentARB(struct gl_context *ctx, const struct marshal_cmd_NamedBufferPageCommitmentARB *restrict cmd)
{
   GLuint buffer = cmd->buffer;
   GLintptr offset = cmd->offset;
   GLsizeiptr size = cmd->size;
   GLboolean commit = cmd->commit;
   CALL_NamedBufferPageCommitmentARB(ctx->Dispatch.Current, (buffer, offset, size, commit));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_NamedBufferPageCommitmentARB), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_NamedBufferPageCommitmentARB(GLuint buffer, GLintptr offset, GLsizeiptr size, GLboolean commit)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_NamedBufferPageCommitmentARB);
   struct marshal_cmd_NamedBufferPageCommitmentARB *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_NamedBufferPageCommitmentARB, cmd_size);
   cmd->buffer = buffer;
   cmd->offset = offset;
   cmd->size = size;
   cmd->commit = commit;
}


/* PrimitiveBoundingBox: marshalled asynchronously */
struct marshal_cmd_PrimitiveBoundingBox
{
   struct marshal_cmd_base cmd_base;
   GLfloat minX;
   GLfloat minY;
   GLfloat minZ;
   GLfloat minW;
   GLfloat maxX;
   GLfloat maxY;
   GLfloat maxZ;
   GLfloat maxW;
};
uint32_t
_mesa_unmarshal_PrimitiveBoundingBox(struct gl_context *ctx, const struct marshal_cmd_PrimitiveBoundingBox *restrict cmd)
{
   GLfloat minX = cmd->minX;
   GLfloat minY = cmd->minY;
   GLfloat minZ = cmd->minZ;
   GLfloat minW = cmd->minW;
   GLfloat maxX = cmd->maxX;
   GLfloat maxY = cmd->maxY;
   GLfloat maxZ = cmd->maxZ;
   GLfloat maxW = cmd->maxW;
   CALL_PrimitiveBoundingBox(ctx->Dispatch.Current, (minX, minY, minZ, minW, maxX, maxY, maxZ, maxW));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_PrimitiveBoundingBox), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_PrimitiveBoundingBox(GLfloat minX, GLfloat minY, GLfloat minZ, GLfloat minW, GLfloat maxX, GLfloat maxY, GLfloat maxZ, GLfloat maxW)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_PrimitiveBoundingBox);
   struct marshal_cmd_PrimitiveBoundingBox *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_PrimitiveBoundingBox, cmd_size);
   cmd->minX = minX;
   cmd->minY = minY;
   cmd->minZ = minZ;
   cmd->minW = minW;
   cmd->maxX = maxX;
   cmd->maxY = maxY;
   cmd->maxZ = maxZ;
   cmd->maxW = maxW;
}


/* BlendBarrier: marshalled asynchronously */
struct marshal_cmd_BlendBarrier
{
   struct marshal_cmd_base cmd_base;
};
uint32_t
_mesa_unmarshal_BlendBarrier(struct gl_context *ctx, const struct marshal_cmd_BlendBarrier *restrict cmd)
{
   CALL_BlendBarrier(ctx->Dispatch.Current, ());
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_BlendBarrier), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_BlendBarrier(void)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_BlendBarrier);
   struct marshal_cmd_BlendBarrier *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_BlendBarrier, cmd_size);
   (void) cmd;
}


/* Uniform1i64ARB: marshalled asynchronously */
struct marshal_cmd_Uniform1i64ARB
{
   struct marshal_cmd_base cmd_base;
   GLint location;
   GLint64 x;
};
uint32_t
_mesa_unmarshal_Uniform1i64ARB(struct gl_context *ctx, const struct marshal_cmd_Uniform1i64ARB *restrict cmd)
{
   GLint location = cmd->location;
   GLint64 x = cmd->x;
   CALL_Uniform1i64ARB(ctx->Dispatch.Current, (location, x));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Uniform1i64ARB), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_Uniform1i64ARB(GLint location, GLint64 x)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Uniform1i64ARB);
   struct marshal_cmd_Uniform1i64ARB *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Uniform1i64ARB, cmd_size);
   cmd->location = location;
   cmd->x = x;
}


/* Uniform2i64ARB: marshalled asynchronously */
struct marshal_cmd_Uniform2i64ARB
{
   struct marshal_cmd_base cmd_base;
   GLint location;
   GLint64 x;
   GLint64 y;
};
uint32_t
_mesa_unmarshal_Uniform2i64ARB(struct gl_context *ctx, const struct marshal_cmd_Uniform2i64ARB *restrict cmd)
{
   GLint location = cmd->location;
   GLint64 x = cmd->x;
   GLint64 y = cmd->y;
   CALL_Uniform2i64ARB(ctx->Dispatch.Current, (location, x, y));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Uniform2i64ARB), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_Uniform2i64ARB(GLint location, GLint64 x, GLint64 y)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Uniform2i64ARB);
   struct marshal_cmd_Uniform2i64ARB *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Uniform2i64ARB, cmd_size);
   cmd->location = location;
   cmd->x = x;
   cmd->y = y;
}


/* Uniform3i64ARB: marshalled asynchronously */
struct marshal_cmd_Uniform3i64ARB
{
   struct marshal_cmd_base cmd_base;
   GLint location;
   GLint64 x;
   GLint64 y;
   GLint64 z;
};
uint32_t
_mesa_unmarshal_Uniform3i64ARB(struct gl_context *ctx, const struct marshal_cmd_Uniform3i64ARB *restrict cmd)
{
   GLint location = cmd->location;
   GLint64 x = cmd->x;
   GLint64 y = cmd->y;
   GLint64 z = cmd->z;
   CALL_Uniform3i64ARB(ctx->Dispatch.Current, (location, x, y, z));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Uniform3i64ARB), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_Uniform3i64ARB(GLint location, GLint64 x, GLint64 y, GLint64 z)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Uniform3i64ARB);
   struct marshal_cmd_Uniform3i64ARB *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Uniform3i64ARB, cmd_size);
   cmd->location = location;
   cmd->x = x;
   cmd->y = y;
   cmd->z = z;
}


/* Uniform4i64ARB: marshalled asynchronously */
struct marshal_cmd_Uniform4i64ARB
{
   struct marshal_cmd_base cmd_base;
   GLint location;
   GLint64 x;
   GLint64 y;
   GLint64 z;
   GLint64 w;
};
uint32_t
_mesa_unmarshal_Uniform4i64ARB(struct gl_context *ctx, const struct marshal_cmd_Uniform4i64ARB *restrict cmd)
{
   GLint location = cmd->location;
   GLint64 x = cmd->x;
   GLint64 y = cmd->y;
   GLint64 z = cmd->z;
   GLint64 w = cmd->w;
   CALL_Uniform4i64ARB(ctx->Dispatch.Current, (location, x, y, z, w));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Uniform4i64ARB), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_Uniform4i64ARB(GLint location, GLint64 x, GLint64 y, GLint64 z, GLint64 w)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Uniform4i64ARB);
   struct marshal_cmd_Uniform4i64ARB *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Uniform4i64ARB, cmd_size);
   cmd->location = location;
   cmd->x = x;
   cmd->y = y;
   cmd->z = z;
   cmd->w = w;
}


/* Uniform1i64vARB: marshalled asynchronously */
struct marshal_cmd_Uniform1i64vARB
{
   struct marshal_cmd_base cmd_base;
   GLint location;
   GLsizei count;
   /* Next safe_mul(count, 1 * sizeof(GLint64)) bytes are GLint64 value[count] */
};
uint32_t
_mesa_unmarshal_Uniform1i64vARB(struct gl_context *ctx, const struct marshal_cmd_Uniform1i64vARB *restrict cmd)
{
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLint64 *value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLint64 *) variable_data;
   CALL_Uniform1i64vARB(ctx->Dispatch.Current, (location, count, value));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_Uniform1i64vARB(GLint location, GLsizei count, const GLint64 *value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 1 * sizeof(GLint64));
   int cmd_size = sizeof(struct marshal_cmd_Uniform1i64vARB) + value_size;
   struct marshal_cmd_Uniform1i64vARB *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "Uniform1i64vARB");
      CALL_Uniform1i64vARB(ctx->Dispatch.Current, (location, count, value));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Uniform1i64vARB, cmd_size);
   cmd->location = location;
   cmd->count = count;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, value, value_size);
}


/* Uniform2i64vARB: marshalled asynchronously */
struct marshal_cmd_Uniform2i64vARB
{
   struct marshal_cmd_base cmd_base;
   GLint location;
   GLsizei count;
   /* Next safe_mul(count, 2 * sizeof(GLint64)) bytes are GLint64 value[count][2] */
};
uint32_t
_mesa_unmarshal_Uniform2i64vARB(struct gl_context *ctx, const struct marshal_cmd_Uniform2i64vARB *restrict cmd)
{
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLint64 *value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLint64 *) variable_data;
   CALL_Uniform2i64vARB(ctx->Dispatch.Current, (location, count, value));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_Uniform2i64vARB(GLint location, GLsizei count, const GLint64 *value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 2 * sizeof(GLint64));
   int cmd_size = sizeof(struct marshal_cmd_Uniform2i64vARB) + value_size;
   struct marshal_cmd_Uniform2i64vARB *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "Uniform2i64vARB");
      CALL_Uniform2i64vARB(ctx->Dispatch.Current, (location, count, value));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Uniform2i64vARB, cmd_size);
   cmd->location = location;
   cmd->count = count;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, value, value_size);
}


/* Uniform3i64vARB: marshalled asynchronously */
struct marshal_cmd_Uniform3i64vARB
{
   struct marshal_cmd_base cmd_base;
   GLint location;
   GLsizei count;
   /* Next safe_mul(count, 3 * sizeof(GLint64)) bytes are GLint64 value[count][3] */
};
uint32_t
_mesa_unmarshal_Uniform3i64vARB(struct gl_context *ctx, const struct marshal_cmd_Uniform3i64vARB *restrict cmd)
{
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLint64 *value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLint64 *) variable_data;
   CALL_Uniform3i64vARB(ctx->Dispatch.Current, (location, count, value));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_Uniform3i64vARB(GLint location, GLsizei count, const GLint64 *value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 3 * sizeof(GLint64));
   int cmd_size = sizeof(struct marshal_cmd_Uniform3i64vARB) + value_size;
   struct marshal_cmd_Uniform3i64vARB *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "Uniform3i64vARB");
      CALL_Uniform3i64vARB(ctx->Dispatch.Current, (location, count, value));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Uniform3i64vARB, cmd_size);
   cmd->location = location;
   cmd->count = count;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, value, value_size);
}


/* Uniform4i64vARB: marshalled asynchronously */
struct marshal_cmd_Uniform4i64vARB
{
   struct marshal_cmd_base cmd_base;
   GLint location;
   GLsizei count;
   /* Next safe_mul(count, 4 * sizeof(GLint64)) bytes are GLint64 value[count][4] */
};
uint32_t
_mesa_unmarshal_Uniform4i64vARB(struct gl_context *ctx, const struct marshal_cmd_Uniform4i64vARB *restrict cmd)
{
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLint64 *value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLint64 *) variable_data;
   CALL_Uniform4i64vARB(ctx->Dispatch.Current, (location, count, value));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_Uniform4i64vARB(GLint location, GLsizei count, const GLint64 *value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 4 * sizeof(GLint64));
   int cmd_size = sizeof(struct marshal_cmd_Uniform4i64vARB) + value_size;
   struct marshal_cmd_Uniform4i64vARB *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "Uniform4i64vARB");
      CALL_Uniform4i64vARB(ctx->Dispatch.Current, (location, count, value));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Uniform4i64vARB, cmd_size);
   cmd->location = location;
   cmd->count = count;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, value, value_size);
}


/* Uniform1ui64ARB: marshalled asynchronously */
struct marshal_cmd_Uniform1ui64ARB
{
   struct marshal_cmd_base cmd_base;
   GLint location;
   GLuint64 x;
};
uint32_t
_mesa_unmarshal_Uniform1ui64ARB(struct gl_context *ctx, const struct marshal_cmd_Uniform1ui64ARB *restrict cmd)
{
   GLint location = cmd->location;
   GLuint64 x = cmd->x;
   CALL_Uniform1ui64ARB(ctx->Dispatch.Current, (location, x));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Uniform1ui64ARB), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_Uniform1ui64ARB(GLint location, GLuint64 x)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Uniform1ui64ARB);
   struct marshal_cmd_Uniform1ui64ARB *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Uniform1ui64ARB, cmd_size);
   cmd->location = location;
   cmd->x = x;
}


/* Uniform2ui64ARB: marshalled asynchronously */
struct marshal_cmd_Uniform2ui64ARB
{
   struct marshal_cmd_base cmd_base;
   GLint location;
   GLuint64 x;
   GLuint64 y;
};
uint32_t
_mesa_unmarshal_Uniform2ui64ARB(struct gl_context *ctx, const struct marshal_cmd_Uniform2ui64ARB *restrict cmd)
{
   GLint location = cmd->location;
   GLuint64 x = cmd->x;
   GLuint64 y = cmd->y;
   CALL_Uniform2ui64ARB(ctx->Dispatch.Current, (location, x, y));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Uniform2ui64ARB), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_Uniform2ui64ARB(GLint location, GLuint64 x, GLuint64 y)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Uniform2ui64ARB);
   struct marshal_cmd_Uniform2ui64ARB *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Uniform2ui64ARB, cmd_size);
   cmd->location = location;
   cmd->x = x;
   cmd->y = y;
}


/* Uniform3ui64ARB: marshalled asynchronously */
struct marshal_cmd_Uniform3ui64ARB
{
   struct marshal_cmd_base cmd_base;
   GLint location;
   GLuint64 x;
   GLuint64 y;
   GLuint64 z;
};
uint32_t
_mesa_unmarshal_Uniform3ui64ARB(struct gl_context *ctx, const struct marshal_cmd_Uniform3ui64ARB *restrict cmd)
{
   GLint location = cmd->location;
   GLuint64 x = cmd->x;
   GLuint64 y = cmd->y;
   GLuint64 z = cmd->z;
   CALL_Uniform3ui64ARB(ctx->Dispatch.Current, (location, x, y, z));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Uniform3ui64ARB), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_Uniform3ui64ARB(GLint location, GLuint64 x, GLuint64 y, GLuint64 z)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Uniform3ui64ARB);
   struct marshal_cmd_Uniform3ui64ARB *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Uniform3ui64ARB, cmd_size);
   cmd->location = location;
   cmd->x = x;
   cmd->y = y;
   cmd->z = z;
}


/* Uniform4ui64ARB: marshalled asynchronously */
struct marshal_cmd_Uniform4ui64ARB
{
   struct marshal_cmd_base cmd_base;
   GLint location;
   GLuint64 x;
   GLuint64 y;
   GLuint64 z;
   GLuint64 w;
};
uint32_t
_mesa_unmarshal_Uniform4ui64ARB(struct gl_context *ctx, const struct marshal_cmd_Uniform4ui64ARB *restrict cmd)
{
   GLint location = cmd->location;
   GLuint64 x = cmd->x;
   GLuint64 y = cmd->y;
   GLuint64 z = cmd->z;
   GLuint64 w = cmd->w;
   CALL_Uniform4ui64ARB(ctx->Dispatch.Current, (location, x, y, z, w));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Uniform4ui64ARB), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_Uniform4ui64ARB(GLint location, GLuint64 x, GLuint64 y, GLuint64 z, GLuint64 w)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Uniform4ui64ARB);
   struct marshal_cmd_Uniform4ui64ARB *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Uniform4ui64ARB, cmd_size);
   cmd->location = location;
   cmd->x = x;
   cmd->y = y;
   cmd->z = z;
   cmd->w = w;
}


/* Uniform1ui64vARB: marshalled asynchronously */
struct marshal_cmd_Uniform1ui64vARB
{
   struct marshal_cmd_base cmd_base;
   GLint location;
   GLsizei count;
   /* Next safe_mul(count, 1 * sizeof(GLuint64)) bytes are GLuint64 value[count] */
};
uint32_t
_mesa_unmarshal_Uniform1ui64vARB(struct gl_context *ctx, const struct marshal_cmd_Uniform1ui64vARB *restrict cmd)
{
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLuint64 *value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLuint64 *) variable_data;
   CALL_Uniform1ui64vARB(ctx->Dispatch.Current, (location, count, value));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_Uniform1ui64vARB(GLint location, GLsizei count, const GLuint64 *value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 1 * sizeof(GLuint64));
   int cmd_size = sizeof(struct marshal_cmd_Uniform1ui64vARB) + value_size;
   struct marshal_cmd_Uniform1ui64vARB *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "Uniform1ui64vARB");
      CALL_Uniform1ui64vARB(ctx->Dispatch.Current, (location, count, value));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Uniform1ui64vARB, cmd_size);
   cmd->location = location;
   cmd->count = count;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, value, value_size);
}


/* Uniform2ui64vARB: marshalled asynchronously */
struct marshal_cmd_Uniform2ui64vARB
{
   struct marshal_cmd_base cmd_base;
   GLint location;
   GLsizei count;
   /* Next safe_mul(count, 2 * sizeof(GLuint64)) bytes are GLuint64 value[count][2] */
};
uint32_t
_mesa_unmarshal_Uniform2ui64vARB(struct gl_context *ctx, const struct marshal_cmd_Uniform2ui64vARB *restrict cmd)
{
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLuint64 *value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLuint64 *) variable_data;
   CALL_Uniform2ui64vARB(ctx->Dispatch.Current, (location, count, value));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_Uniform2ui64vARB(GLint location, GLsizei count, const GLuint64 *value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 2 * sizeof(GLuint64));
   int cmd_size = sizeof(struct marshal_cmd_Uniform2ui64vARB) + value_size;
   struct marshal_cmd_Uniform2ui64vARB *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "Uniform2ui64vARB");
      CALL_Uniform2ui64vARB(ctx->Dispatch.Current, (location, count, value));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Uniform2ui64vARB, cmd_size);
   cmd->location = location;
   cmd->count = count;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, value, value_size);
}


/* Uniform3ui64vARB: marshalled asynchronously */
struct marshal_cmd_Uniform3ui64vARB
{
   struct marshal_cmd_base cmd_base;
   GLint location;
   GLsizei count;
   /* Next safe_mul(count, 3 * sizeof(GLuint64)) bytes are GLuint64 value[count][3] */
};
uint32_t
_mesa_unmarshal_Uniform3ui64vARB(struct gl_context *ctx, const struct marshal_cmd_Uniform3ui64vARB *restrict cmd)
{
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLuint64 *value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLuint64 *) variable_data;
   CALL_Uniform3ui64vARB(ctx->Dispatch.Current, (location, count, value));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_Uniform3ui64vARB(GLint location, GLsizei count, const GLuint64 *value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 3 * sizeof(GLuint64));
   int cmd_size = sizeof(struct marshal_cmd_Uniform3ui64vARB) + value_size;
   struct marshal_cmd_Uniform3ui64vARB *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "Uniform3ui64vARB");
      CALL_Uniform3ui64vARB(ctx->Dispatch.Current, (location, count, value));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Uniform3ui64vARB, cmd_size);
   cmd->location = location;
   cmd->count = count;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, value, value_size);
}


/* Uniform4ui64vARB: marshalled asynchronously */
struct marshal_cmd_Uniform4ui64vARB
{
   struct marshal_cmd_base cmd_base;
   GLint location;
   GLsizei count;
   /* Next safe_mul(count, 4 * sizeof(GLuint64)) bytes are GLuint64 value[count][4] */
};
uint32_t
_mesa_unmarshal_Uniform4ui64vARB(struct gl_context *ctx, const struct marshal_cmd_Uniform4ui64vARB *restrict cmd)
{
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLuint64 *value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLuint64 *) variable_data;
   CALL_Uniform4ui64vARB(ctx->Dispatch.Current, (location, count, value));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_Uniform4ui64vARB(GLint location, GLsizei count, const GLuint64 *value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 4 * sizeof(GLuint64));
   int cmd_size = sizeof(struct marshal_cmd_Uniform4ui64vARB) + value_size;
   struct marshal_cmd_Uniform4ui64vARB *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "Uniform4ui64vARB");
      CALL_Uniform4ui64vARB(ctx->Dispatch.Current, (location, count, value));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Uniform4ui64vARB, cmd_size);
   cmd->location = location;
   cmd->count = count;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, value, value_size);
}


/* GetUniformi64vARB: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetUniformi64vARB(GLuint program, GLint location, GLint64 *params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetUniformi64vARB");
   CALL_GetUniformi64vARB(ctx->Dispatch.Current, (program, location, params));
}


/* GetUniformui64vARB: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetUniformui64vARB(GLuint program, GLint location, GLuint64 *params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetUniformui64vARB");
   CALL_GetUniformui64vARB(ctx->Dispatch.Current, (program, location, params));
}


/* GetnUniformi64vARB: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetnUniformi64vARB(GLuint program, GLint location, GLsizei bufSize, GLint64 *params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetnUniformi64vARB");
   CALL_GetnUniformi64vARB(ctx->Dispatch.Current, (program, location, bufSize, params));
}


/* GetnUniformui64vARB: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetnUniformui64vARB(GLuint program, GLint location, GLsizei bufSize, GLuint64 *params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetnUniformui64vARB");
   CALL_GetnUniformui64vARB(ctx->Dispatch.Current, (program, location, bufSize, params));
}


/* ProgramUniform1i64ARB: marshalled asynchronously */
struct marshal_cmd_ProgramUniform1i64ARB
{
   struct marshal_cmd_base cmd_base;
   GLuint program;
   GLint location;
   GLint64 x;
};
uint32_t
_mesa_unmarshal_ProgramUniform1i64ARB(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform1i64ARB *restrict cmd)
{
   GLuint program = cmd->program;
   GLint location = cmd->location;
   GLint64 x = cmd->x;
   CALL_ProgramUniform1i64ARB(ctx->Dispatch.Current, (program, location, x));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_ProgramUniform1i64ARB), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ProgramUniform1i64ARB(GLuint program, GLint location, GLint64 x)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ProgramUniform1i64ARB);
   struct marshal_cmd_ProgramUniform1i64ARB *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ProgramUniform1i64ARB, cmd_size);
   cmd->program = program;
   cmd->location = location;
   cmd->x = x;
}


/* ProgramUniform2i64ARB: marshalled asynchronously */
struct marshal_cmd_ProgramUniform2i64ARB
{
   struct marshal_cmd_base cmd_base;
   GLuint program;
   GLint location;
   GLint64 x;
   GLint64 y;
};
uint32_t
_mesa_unmarshal_ProgramUniform2i64ARB(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform2i64ARB *restrict cmd)
{
   GLuint program = cmd->program;
   GLint location = cmd->location;
   GLint64 x = cmd->x;
   GLint64 y = cmd->y;
   CALL_ProgramUniform2i64ARB(ctx->Dispatch.Current, (program, location, x, y));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_ProgramUniform2i64ARB), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ProgramUniform2i64ARB(GLuint program, GLint location, GLint64 x, GLint64 y)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ProgramUniform2i64ARB);
   struct marshal_cmd_ProgramUniform2i64ARB *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ProgramUniform2i64ARB, cmd_size);
   cmd->program = program;
   cmd->location = location;
   cmd->x = x;
   cmd->y = y;
}


/* ProgramUniform3i64ARB: marshalled asynchronously */
struct marshal_cmd_ProgramUniform3i64ARB
{
   struct marshal_cmd_base cmd_base;
   GLuint program;
   GLint location;
   GLint64 x;
   GLint64 y;
   GLint64 z;
};
uint32_t
_mesa_unmarshal_ProgramUniform3i64ARB(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform3i64ARB *restrict cmd)
{
   GLuint program = cmd->program;
   GLint location = cmd->location;
   GLint64 x = cmd->x;
   GLint64 y = cmd->y;
   GLint64 z = cmd->z;
   CALL_ProgramUniform3i64ARB(ctx->Dispatch.Current, (program, location, x, y, z));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_ProgramUniform3i64ARB), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ProgramUniform3i64ARB(GLuint program, GLint location, GLint64 x, GLint64 y, GLint64 z)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ProgramUniform3i64ARB);
   struct marshal_cmd_ProgramUniform3i64ARB *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ProgramUniform3i64ARB, cmd_size);
   cmd->program = program;
   cmd->location = location;
   cmd->x = x;
   cmd->y = y;
   cmd->z = z;
}


/* ProgramUniform4i64ARB: marshalled asynchronously */
struct marshal_cmd_ProgramUniform4i64ARB
{
   struct marshal_cmd_base cmd_base;
   GLuint program;
   GLint location;
   GLint64 x;
   GLint64 y;
   GLint64 z;
   GLint64 w;
};
uint32_t
_mesa_unmarshal_ProgramUniform4i64ARB(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform4i64ARB *restrict cmd)
{
   GLuint program = cmd->program;
   GLint location = cmd->location;
   GLint64 x = cmd->x;
   GLint64 y = cmd->y;
   GLint64 z = cmd->z;
   GLint64 w = cmd->w;
   CALL_ProgramUniform4i64ARB(ctx->Dispatch.Current, (program, location, x, y, z, w));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_ProgramUniform4i64ARB), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ProgramUniform4i64ARB(GLuint program, GLint location, GLint64 x, GLint64 y, GLint64 z, GLint64 w)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ProgramUniform4i64ARB);
   struct marshal_cmd_ProgramUniform4i64ARB *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ProgramUniform4i64ARB, cmd_size);
   cmd->program = program;
   cmd->location = location;
   cmd->x = x;
   cmd->y = y;
   cmd->z = z;
   cmd->w = w;
}


/* ProgramUniform1i64vARB: marshalled asynchronously */
struct marshal_cmd_ProgramUniform1i64vARB
{
   struct marshal_cmd_base cmd_base;
   GLuint program;
   GLint location;
   GLsizei count;
   /* Next safe_mul(count, 1 * sizeof(GLint64)) bytes are GLint64 value[count] */
};
uint32_t
_mesa_unmarshal_ProgramUniform1i64vARB(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform1i64vARB *restrict cmd)
{
   GLuint program = cmd->program;
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLint64 *value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLint64 *) variable_data;
   CALL_ProgramUniform1i64vARB(ctx->Dispatch.Current, (program, location, count, value));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ProgramUniform1i64vARB(GLuint program, GLint location, GLsizei count, const GLint64 *value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 1 * sizeof(GLint64));
   int cmd_size = sizeof(struct marshal_cmd_ProgramUniform1i64vARB) + value_size;
   struct marshal_cmd_ProgramUniform1i64vARB *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "ProgramUniform1i64vARB");
      CALL_ProgramUniform1i64vARB(ctx->Dispatch.Current, (program, location, count, value));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ProgramUniform1i64vARB, cmd_size);
   cmd->program = program;
   cmd->location = location;
   cmd->count = count;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, value, value_size);
}


/* ProgramUniform2i64vARB: marshalled asynchronously */
struct marshal_cmd_ProgramUniform2i64vARB
{
   struct marshal_cmd_base cmd_base;
   GLuint program;
   GLint location;
   GLsizei count;
   /* Next safe_mul(count, 2 * sizeof(GLint64)) bytes are GLint64 value[count][2] */
};
uint32_t
_mesa_unmarshal_ProgramUniform2i64vARB(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform2i64vARB *restrict cmd)
{
   GLuint program = cmd->program;
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLint64 *value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLint64 *) variable_data;
   CALL_ProgramUniform2i64vARB(ctx->Dispatch.Current, (program, location, count, value));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ProgramUniform2i64vARB(GLuint program, GLint location, GLsizei count, const GLint64 *value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 2 * sizeof(GLint64));
   int cmd_size = sizeof(struct marshal_cmd_ProgramUniform2i64vARB) + value_size;
   struct marshal_cmd_ProgramUniform2i64vARB *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "ProgramUniform2i64vARB");
      CALL_ProgramUniform2i64vARB(ctx->Dispatch.Current, (program, location, count, value));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ProgramUniform2i64vARB, cmd_size);
   cmd->program = program;
   cmd->location = location;
   cmd->count = count;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, value, value_size);
}


/* ProgramUniform3i64vARB: marshalled asynchronously */
struct marshal_cmd_ProgramUniform3i64vARB
{
   struct marshal_cmd_base cmd_base;
   GLuint program;
   GLint location;
   GLsizei count;
   /* Next safe_mul(count, 3 * sizeof(GLint64)) bytes are GLint64 value[count][3] */
};
uint32_t
_mesa_unmarshal_ProgramUniform3i64vARB(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform3i64vARB *restrict cmd)
{
   GLuint program = cmd->program;
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLint64 *value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLint64 *) variable_data;
   CALL_ProgramUniform3i64vARB(ctx->Dispatch.Current, (program, location, count, value));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ProgramUniform3i64vARB(GLuint program, GLint location, GLsizei count, const GLint64 *value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 3 * sizeof(GLint64));
   int cmd_size = sizeof(struct marshal_cmd_ProgramUniform3i64vARB) + value_size;
   struct marshal_cmd_ProgramUniform3i64vARB *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "ProgramUniform3i64vARB");
      CALL_ProgramUniform3i64vARB(ctx->Dispatch.Current, (program, location, count, value));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ProgramUniform3i64vARB, cmd_size);
   cmd->program = program;
   cmd->location = location;
   cmd->count = count;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, value, value_size);
}


/* ProgramUniform4i64vARB: marshalled asynchronously */
struct marshal_cmd_ProgramUniform4i64vARB
{
   struct marshal_cmd_base cmd_base;
   GLuint program;
   GLint location;
   GLsizei count;
   /* Next safe_mul(count, 4 * sizeof(GLint64)) bytes are GLint64 value[count][4] */
};
uint32_t
_mesa_unmarshal_ProgramUniform4i64vARB(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform4i64vARB *restrict cmd)
{
   GLuint program = cmd->program;
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLint64 *value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLint64 *) variable_data;
   CALL_ProgramUniform4i64vARB(ctx->Dispatch.Current, (program, location, count, value));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ProgramUniform4i64vARB(GLuint program, GLint location, GLsizei count, const GLint64 *value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 4 * sizeof(GLint64));
   int cmd_size = sizeof(struct marshal_cmd_ProgramUniform4i64vARB) + value_size;
   struct marshal_cmd_ProgramUniform4i64vARB *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "ProgramUniform4i64vARB");
      CALL_ProgramUniform4i64vARB(ctx->Dispatch.Current, (program, location, count, value));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ProgramUniform4i64vARB, cmd_size);
   cmd->program = program;
   cmd->location = location;
   cmd->count = count;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, value, value_size);
}


/* ProgramUniform1ui64ARB: marshalled asynchronously */
struct marshal_cmd_ProgramUniform1ui64ARB
{
   struct marshal_cmd_base cmd_base;
   GLuint program;
   GLint location;
   GLuint64 x;
};
uint32_t
_mesa_unmarshal_ProgramUniform1ui64ARB(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform1ui64ARB *restrict cmd)
{
   GLuint program = cmd->program;
   GLint location = cmd->location;
   GLuint64 x = cmd->x;
   CALL_ProgramUniform1ui64ARB(ctx->Dispatch.Current, (program, location, x));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_ProgramUniform1ui64ARB), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ProgramUniform1ui64ARB(GLuint program, GLint location, GLuint64 x)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ProgramUniform1ui64ARB);
   struct marshal_cmd_ProgramUniform1ui64ARB *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ProgramUniform1ui64ARB, cmd_size);
   cmd->program = program;
   cmd->location = location;
   cmd->x = x;
}


/* ProgramUniform2ui64ARB: marshalled asynchronously */
struct marshal_cmd_ProgramUniform2ui64ARB
{
   struct marshal_cmd_base cmd_base;
   GLuint program;
   GLint location;
   GLuint64 x;
   GLuint64 y;
};
uint32_t
_mesa_unmarshal_ProgramUniform2ui64ARB(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform2ui64ARB *restrict cmd)
{
   GLuint program = cmd->program;
   GLint location = cmd->location;
   GLuint64 x = cmd->x;
   GLuint64 y = cmd->y;
   CALL_ProgramUniform2ui64ARB(ctx->Dispatch.Current, (program, location, x, y));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_ProgramUniform2ui64ARB), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ProgramUniform2ui64ARB(GLuint program, GLint location, GLuint64 x, GLuint64 y)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ProgramUniform2ui64ARB);
   struct marshal_cmd_ProgramUniform2ui64ARB *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ProgramUniform2ui64ARB, cmd_size);
   cmd->program = program;
   cmd->location = location;
   cmd->x = x;
   cmd->y = y;
}


/* ProgramUniform3ui64ARB: marshalled asynchronously */
struct marshal_cmd_ProgramUniform3ui64ARB
{
   struct marshal_cmd_base cmd_base;
   GLuint program;
   GLint location;
   GLuint64 x;
   GLuint64 y;
   GLuint64 z;
};
uint32_t
_mesa_unmarshal_ProgramUniform3ui64ARB(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform3ui64ARB *restrict cmd)
{
   GLuint program = cmd->program;
   GLint location = cmd->location;
   GLuint64 x = cmd->x;
   GLuint64 y = cmd->y;
   GLuint64 z = cmd->z;
   CALL_ProgramUniform3ui64ARB(ctx->Dispatch.Current, (program, location, x, y, z));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_ProgramUniform3ui64ARB), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ProgramUniform3ui64ARB(GLuint program, GLint location, GLuint64 x, GLuint64 y, GLuint64 z)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ProgramUniform3ui64ARB);
   struct marshal_cmd_ProgramUniform3ui64ARB *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ProgramUniform3ui64ARB, cmd_size);
   cmd->program = program;
   cmd->location = location;
   cmd->x = x;
   cmd->y = y;
   cmd->z = z;
}


/* ProgramUniform4ui64ARB: marshalled asynchronously */
struct marshal_cmd_ProgramUniform4ui64ARB
{
   struct marshal_cmd_base cmd_base;
   GLuint program;
   GLint location;
   GLuint64 x;
   GLuint64 y;
   GLuint64 z;
   GLuint64 w;
};
uint32_t
_mesa_unmarshal_ProgramUniform4ui64ARB(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform4ui64ARB *restrict cmd)
{
   GLuint program = cmd->program;
   GLint location = cmd->location;
   GLuint64 x = cmd->x;
   GLuint64 y = cmd->y;
   GLuint64 z = cmd->z;
   GLuint64 w = cmd->w;
   CALL_ProgramUniform4ui64ARB(ctx->Dispatch.Current, (program, location, x, y, z, w));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_ProgramUniform4ui64ARB), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ProgramUniform4ui64ARB(GLuint program, GLint location, GLuint64 x, GLuint64 y, GLuint64 z, GLuint64 w)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ProgramUniform4ui64ARB);
   struct marshal_cmd_ProgramUniform4ui64ARB *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ProgramUniform4ui64ARB, cmd_size);
   cmd->program = program;
   cmd->location = location;
   cmd->x = x;
   cmd->y = y;
   cmd->z = z;
   cmd->w = w;
}


/* ProgramUniform1ui64vARB: marshalled asynchronously */
struct marshal_cmd_ProgramUniform1ui64vARB
{
   struct marshal_cmd_base cmd_base;
   GLuint program;
   GLint location;
   GLsizei count;
   /* Next safe_mul(count, 1 * sizeof(GLuint64)) bytes are GLuint64 value[count] */
};
uint32_t
_mesa_unmarshal_ProgramUniform1ui64vARB(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform1ui64vARB *restrict cmd)
{
   GLuint program = cmd->program;
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLuint64 *value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLuint64 *) variable_data;
   CALL_ProgramUniform1ui64vARB(ctx->Dispatch.Current, (program, location, count, value));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ProgramUniform1ui64vARB(GLuint program, GLint location, GLsizei count, const GLuint64 *value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 1 * sizeof(GLuint64));
   int cmd_size = sizeof(struct marshal_cmd_ProgramUniform1ui64vARB) + value_size;
   struct marshal_cmd_ProgramUniform1ui64vARB *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "ProgramUniform1ui64vARB");
      CALL_ProgramUniform1ui64vARB(ctx->Dispatch.Current, (program, location, count, value));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ProgramUniform1ui64vARB, cmd_size);
   cmd->program = program;
   cmd->location = location;
   cmd->count = count;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, value, value_size);
}


/* ProgramUniform2ui64vARB: marshalled asynchronously */
struct marshal_cmd_ProgramUniform2ui64vARB
{
   struct marshal_cmd_base cmd_base;
   GLuint program;
   GLint location;
   GLsizei count;
   /* Next safe_mul(count, 2 * sizeof(GLuint64)) bytes are GLuint64 value[count][2] */
};
uint32_t
_mesa_unmarshal_ProgramUniform2ui64vARB(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform2ui64vARB *restrict cmd)
{
   GLuint program = cmd->program;
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLuint64 *value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLuint64 *) variable_data;
   CALL_ProgramUniform2ui64vARB(ctx->Dispatch.Current, (program, location, count, value));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ProgramUniform2ui64vARB(GLuint program, GLint location, GLsizei count, const GLuint64 *value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 2 * sizeof(GLuint64));
   int cmd_size = sizeof(struct marshal_cmd_ProgramUniform2ui64vARB) + value_size;
   struct marshal_cmd_ProgramUniform2ui64vARB *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "ProgramUniform2ui64vARB");
      CALL_ProgramUniform2ui64vARB(ctx->Dispatch.Current, (program, location, count, value));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ProgramUniform2ui64vARB, cmd_size);
   cmd->program = program;
   cmd->location = location;
   cmd->count = count;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, value, value_size);
}


/* ProgramUniform3ui64vARB: marshalled asynchronously */
struct marshal_cmd_ProgramUniform3ui64vARB
{
   struct marshal_cmd_base cmd_base;
   GLuint program;
   GLint location;
   GLsizei count;
   /* Next safe_mul(count, 3 * sizeof(GLuint64)) bytes are GLuint64 value[count][3] */
};
uint32_t
_mesa_unmarshal_ProgramUniform3ui64vARB(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform3ui64vARB *restrict cmd)
{
   GLuint program = cmd->program;
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLuint64 *value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLuint64 *) variable_data;
   CALL_ProgramUniform3ui64vARB(ctx->Dispatch.Current, (program, location, count, value));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ProgramUniform3ui64vARB(GLuint program, GLint location, GLsizei count, const GLuint64 *value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 3 * sizeof(GLuint64));
   int cmd_size = sizeof(struct marshal_cmd_ProgramUniform3ui64vARB) + value_size;
   struct marshal_cmd_ProgramUniform3ui64vARB *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "ProgramUniform3ui64vARB");
      CALL_ProgramUniform3ui64vARB(ctx->Dispatch.Current, (program, location, count, value));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ProgramUniform3ui64vARB, cmd_size);
   cmd->program = program;
   cmd->location = location;
   cmd->count = count;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, value, value_size);
}


/* ProgramUniform4ui64vARB: marshalled asynchronously */
struct marshal_cmd_ProgramUniform4ui64vARB
{
   struct marshal_cmd_base cmd_base;
   GLuint program;
   GLint location;
   GLsizei count;
   /* Next safe_mul(count, 4 * sizeof(GLuint64)) bytes are GLuint64 value[count][4] */
};
uint32_t
_mesa_unmarshal_ProgramUniform4ui64vARB(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform4ui64vARB *restrict cmd)
{
   GLuint program = cmd->program;
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLuint64 *value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLuint64 *) variable_data;
   CALL_ProgramUniform4ui64vARB(ctx->Dispatch.Current, (program, location, count, value));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ProgramUniform4ui64vARB(GLuint program, GLint location, GLsizei count, const GLuint64 *value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 4 * sizeof(GLuint64));
   int cmd_size = sizeof(struct marshal_cmd_ProgramUniform4ui64vARB) + value_size;
   struct marshal_cmd_ProgramUniform4ui64vARB *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "ProgramUniform4ui64vARB");
      CALL_ProgramUniform4ui64vARB(ctx->Dispatch.Current, (program, location, count, value));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ProgramUniform4ui64vARB, cmd_size);
   cmd->program = program;
   cmd->location = location;
   cmd->count = count;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, value, value_size);
}


/* MaxShaderCompilerThreadsKHR: marshalled asynchronously */
struct marshal_cmd_MaxShaderCompilerThreadsKHR
{
   struct marshal_cmd_base cmd_base;
   GLuint count;
};
uint32_t
_mesa_unmarshal_MaxShaderCompilerThreadsKHR(struct gl_context *ctx, const struct marshal_cmd_MaxShaderCompilerThreadsKHR *restrict cmd)
{
   GLuint count = cmd->count;
   CALL_MaxShaderCompilerThreadsKHR(ctx->Dispatch.Current, (count));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MaxShaderCompilerThreadsKHR), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_MaxShaderCompilerThreadsKHR(GLuint count)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MaxShaderCompilerThreadsKHR);
   struct marshal_cmd_MaxShaderCompilerThreadsKHR *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MaxShaderCompilerThreadsKHR, cmd_size);
   cmd->count = count;
}


/* SpecializeShaderARB: marshalled asynchronously */
struct marshal_cmd_SpecializeShaderARB
{
   struct marshal_cmd_base cmd_base;
   GLuint shader;
   GLuint numSpecializationConstants;
   /* Next (strlen(pEntryPoint) + 1) bytes are GLchar pEntryPoint[(strlen(pEntryPoint) + 1)] */
   /* Next safe_mul(numSpecializationConstants, 1 * sizeof(GLuint)) bytes are GLuint pConstantIndex[numSpecializationConstants] */
   /* Next safe_mul(numSpecializationConstants, 1 * sizeof(GLuint)) bytes are GLuint pConstantValue[numSpecializationConstants] */
};
uint32_t
_mesa_unmarshal_SpecializeShaderARB(struct gl_context *ctx, const struct marshal_cmd_SpecializeShaderARB *restrict cmd)
{
   GLuint shader = cmd->shader;
   GLuint numSpecializationConstants = cmd->numSpecializationConstants;
   GLchar *pEntryPoint;
   GLuint *pConstantIndex;
   GLuint *pConstantValue;
   const char *variable_data = (const char *) (cmd + 1);
   pEntryPoint = (GLchar *) variable_data;
   variable_data += (strlen(pEntryPoint) + 1);
   pConstantIndex = (GLuint *) variable_data;
   variable_data += numSpecializationConstants * 1 * sizeof(GLuint);
   pConstantValue = (GLuint *) variable_data;
   CALL_SpecializeShaderARB(ctx->Dispatch.Current, (shader, pEntryPoint, numSpecializationConstants, pConstantIndex, pConstantValue));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_SpecializeShaderARB(GLuint shader, const GLchar *pEntryPoint, GLuint numSpecializationConstants, const GLuint *pConstantIndex, const GLuint *pConstantValue)
{
   GET_CURRENT_CONTEXT(ctx);
   int pEntryPoint_size = (strlen(pEntryPoint) + 1);
   int pConstantIndex_size = safe_mul(numSpecializationConstants, 1 * sizeof(GLuint));
   int pConstantValue_size = safe_mul(numSpecializationConstants, 1 * sizeof(GLuint));
   int cmd_size = sizeof(struct marshal_cmd_SpecializeShaderARB) + pEntryPoint_size + pConstantIndex_size + pConstantValue_size;
   struct marshal_cmd_SpecializeShaderARB *cmd;
   if (unlikely(pEntryPoint_size < 0 || (pEntryPoint_size > 0 && !pEntryPoint) || pConstantIndex_size < 0 || (pConstantIndex_size > 0 && !pConstantIndex) || pConstantValue_size < 0 || (pConstantValue_size > 0 && !pConstantValue) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "SpecializeShaderARB");
      CALL_SpecializeShaderARB(ctx->Dispatch.Current, (shader, pEntryPoint, numSpecializationConstants, pConstantIndex, pConstantValue));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_SpecializeShaderARB, cmd_size);
   cmd->shader = shader;
   cmd->numSpecializationConstants = numSpecializationConstants;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, pEntryPoint, pEntryPoint_size);
   variable_data += pEntryPoint_size;
   memcpy(variable_data, pConstantIndex, pConstantIndex_size);
   variable_data += pConstantIndex_size;
   memcpy(variable_data, pConstantValue, pConstantValue_size);
}


/* ColorPointerEXT: marshalled asynchronously */
struct marshal_cmd_ColorPointerEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 type;
   GLint size;
   GLsizei stride;
   GLsizei count;
   const GLvoid * pointer;
};
uint32_t
_mesa_unmarshal_ColorPointerEXT(struct gl_context *ctx, const struct marshal_cmd_ColorPointerEXT *restrict cmd)
{
   GLint size = cmd->size;
   GLenum type = cmd->type;
   GLsizei stride = cmd->stride;
   GLsizei count = cmd->count;
   const GLvoid * pointer = cmd->pointer;
   CALL_ColorPointerEXT(ctx->Dispatch.Current, (size, type, stride, count, pointer));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_ColorPointerEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ColorPointerEXT(GLint size, GLenum type, GLsizei stride, GLsizei count, const GLvoid *pointer)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ColorPointerEXT);
   struct marshal_cmd_ColorPointerEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ColorPointerEXT, cmd_size);
   cmd->size = size;
   cmd->type = MIN2(type, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->stride = stride;
   cmd->count = count;
   cmd->pointer = pointer;
   if (COMPAT) _mesa_glthread_AttribPointer(ctx, VERT_ATTRIB_COLOR0, MESA_PACK_VFORMAT(type, size, 1, 0, 0), stride, pointer);
}


/* EdgeFlagPointerEXT: marshalled asynchronously */
struct marshal_cmd_EdgeFlagPointerEXT
{
   struct marshal_cmd_base cmd_base;
   GLsizei stride;
   GLsizei count;
   const GLboolean * pointer;
};
uint32_t
_mesa_unmarshal_EdgeFlagPointerEXT(struct gl_context *ctx, const struct marshal_cmd_EdgeFlagPointerEXT *restrict cmd)
{
   GLsizei stride = cmd->stride;
   GLsizei count = cmd->count;
   const GLboolean * pointer = cmd->pointer;
   CALL_EdgeFlagPointerEXT(ctx->Dispatch.Current, (stride, count, pointer));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_EdgeFlagPointerEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_EdgeFlagPointerEXT(GLsizei stride, GLsizei count, const GLboolean *pointer)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_EdgeFlagPointerEXT);
   struct marshal_cmd_EdgeFlagPointerEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_EdgeFlagPointerEXT, cmd_size);
   cmd->stride = stride;
   cmd->count = count;
   cmd->pointer = pointer;
   if (COMPAT) _mesa_glthread_AttribPointer(ctx, VERT_ATTRIB_EDGEFLAG, MESA_PACK_VFORMAT(GL_UNSIGNED_BYTE, 1, 0, 0, 0), stride, pointer);
}


/* IndexPointerEXT: marshalled asynchronously */
struct marshal_cmd_IndexPointerEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 type;
   GLsizei stride;
   GLsizei count;
   const GLvoid * pointer;
};
uint32_t
_mesa_unmarshal_IndexPointerEXT(struct gl_context *ctx, const struct marshal_cmd_IndexPointerEXT *restrict cmd)
{
   GLenum type = cmd->type;
   GLsizei stride = cmd->stride;
   GLsizei count = cmd->count;
   const GLvoid * pointer = cmd->pointer;
   CALL_IndexPointerEXT(ctx->Dispatch.Current, (type, stride, count, pointer));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_IndexPointerEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_IndexPointerEXT(GLenum type, GLsizei stride, GLsizei count, const GLvoid *pointer)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_IndexPointerEXT);
   struct marshal_cmd_IndexPointerEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_IndexPointerEXT, cmd_size);
   cmd->type = MIN2(type, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->stride = stride;
   cmd->count = count;
   cmd->pointer = pointer;
   if (COMPAT) _mesa_glthread_AttribPointer(ctx, VERT_ATTRIB_COLOR_INDEX, MESA_PACK_VFORMAT(type, 1, 0, 0, 0), stride, pointer);
}


/* NormalPointerEXT: marshalled asynchronously */
struct marshal_cmd_NormalPointerEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 type;
   GLsizei stride;
   GLsizei count;
   const GLvoid * pointer;
};
uint32_t
_mesa_unmarshal_NormalPointerEXT(struct gl_context *ctx, const struct marshal_cmd_NormalPointerEXT *restrict cmd)
{
   GLenum type = cmd->type;
   GLsizei stride = cmd->stride;
   GLsizei count = cmd->count;
   const GLvoid * pointer = cmd->pointer;
   CALL_NormalPointerEXT(ctx->Dispatch.Current, (type, stride, count, pointer));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_NormalPointerEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_NormalPointerEXT(GLenum type, GLsizei stride, GLsizei count, const GLvoid *pointer)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_NormalPointerEXT);
   struct marshal_cmd_NormalPointerEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_NormalPointerEXT, cmd_size);
   cmd->type = MIN2(type, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->stride = stride;
   cmd->count = count;
   cmd->pointer = pointer;
   if (COMPAT) _mesa_glthread_AttribPointer(ctx, VERT_ATTRIB_NORMAL, MESA_PACK_VFORMAT(type, 3, 1, 0, 0), stride, pointer);
}


/* TexCoordPointerEXT: marshalled asynchronously */
struct marshal_cmd_TexCoordPointerEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 type;
   GLint size;
   GLsizei stride;
   GLsizei count;
   const GLvoid * pointer;
};
uint32_t
_mesa_unmarshal_TexCoordPointerEXT(struct gl_context *ctx, const struct marshal_cmd_TexCoordPointerEXT *restrict cmd)
{
   GLint size = cmd->size;
   GLenum type = cmd->type;
   GLsizei stride = cmd->stride;
   GLsizei count = cmd->count;
   const GLvoid * pointer = cmd->pointer;
   CALL_TexCoordPointerEXT(ctx->Dispatch.Current, (size, type, stride, count, pointer));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_TexCoordPointerEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_TexCoordPointerEXT(GLint size, GLenum type, GLsizei stride, GLsizei count, const GLvoid *pointer)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TexCoordPointerEXT);
   struct marshal_cmd_TexCoordPointerEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexCoordPointerEXT, cmd_size);
   cmd->size = size;
   cmd->type = MIN2(type, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->stride = stride;
   cmd->count = count;
   cmd->pointer = pointer;
   if (COMPAT) _mesa_glthread_AttribPointer(ctx, VERT_ATTRIB_TEX(ctx->GLThread.ClientActiveTexture), MESA_PACK_VFORMAT(type, size, 0, 0, 0), stride, pointer);
}


/* VertexPointerEXT: marshalled asynchronously */
struct marshal_cmd_VertexPointerEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 type;
   GLint size;
   GLsizei stride;
   GLsizei count;
   const GLvoid * pointer;
};
uint32_t
_mesa_unmarshal_VertexPointerEXT(struct gl_context *ctx, const struct marshal_cmd_VertexPointerEXT *restrict cmd)
{
   GLint size = cmd->size;
   GLenum type = cmd->type;
   GLsizei stride = cmd->stride;
   GLsizei count = cmd->count;
   const GLvoid * pointer = cmd->pointer;
   CALL_VertexPointerEXT(ctx->Dispatch.Current, (size, type, stride, count, pointer));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexPointerEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_VertexPointerEXT(GLint size, GLenum type, GLsizei stride, GLsizei count, const GLvoid *pointer)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexPointerEXT);
   struct marshal_cmd_VertexPointerEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexPointerEXT, cmd_size);
   cmd->size = size;
   cmd->type = MIN2(type, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->stride = stride;
   cmd->count = count;
   cmd->pointer = pointer;
   if (COMPAT) _mesa_glthread_AttribPointer(ctx, VERT_ATTRIB_POS, MESA_PACK_VFORMAT(type, size, 0, 0, 0), stride, pointer);
}


/* LockArraysEXT: marshalled asynchronously */
struct marshal_cmd_LockArraysEXT
{
   struct marshal_cmd_base cmd_base;
   GLint first;
   GLsizei count;
};
uint32_t
_mesa_unmarshal_LockArraysEXT(struct gl_context *ctx, const struct marshal_cmd_LockArraysEXT *restrict cmd)
{
   GLint first = cmd->first;
   GLsizei count = cmd->count;
   CALL_LockArraysEXT(ctx->Dispatch.Current, (first, count));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_LockArraysEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_LockArraysEXT(GLint first, GLsizei count)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_LockArraysEXT);
   struct marshal_cmd_LockArraysEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_LockArraysEXT, cmd_size);
   cmd->first = first;
   cmd->count = count;
}


/* UnlockArraysEXT: marshalled asynchronously */
struct marshal_cmd_UnlockArraysEXT
{
   struct marshal_cmd_base cmd_base;
};
uint32_t
_mesa_unmarshal_UnlockArraysEXT(struct gl_context *ctx, const struct marshal_cmd_UnlockArraysEXT *restrict cmd)
{
   CALL_UnlockArraysEXT(ctx->Dispatch.Current, ());
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_UnlockArraysEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_UnlockArraysEXT(void)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_UnlockArraysEXT);
   struct marshal_cmd_UnlockArraysEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_UnlockArraysEXT, cmd_size);
   (void) cmd;
}


/* ViewportArrayv: marshalled asynchronously */
struct marshal_cmd_ViewportArrayv
{
   struct marshal_cmd_base cmd_base;
   GLuint first;
   GLsizei count;
   /* Next safe_mul(count, 4 * sizeof(GLfloat)) bytes are GLfloat v[count][4] */
};
uint32_t
_mesa_unmarshal_ViewportArrayv(struct gl_context *ctx, const struct marshal_cmd_ViewportArrayv *restrict cmd)
{
   GLuint first = cmd->first;
   GLsizei count = cmd->count;
   GLfloat *v;
   const char *variable_data = (const char *) (cmd + 1);
   v = (GLfloat *) variable_data;
   CALL_ViewportArrayv(ctx->Dispatch.Current, (first, count, v));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ViewportArrayv(GLuint first, GLsizei count, const GLfloat *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int v_size = safe_mul(count, 4 * sizeof(GLfloat));
   int cmd_size = sizeof(struct marshal_cmd_ViewportArrayv) + v_size;
   struct marshal_cmd_ViewportArrayv *cmd;
   if (unlikely(v_size < 0 || (v_size > 0 && !v) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "ViewportArrayv");
      CALL_ViewportArrayv(ctx->Dispatch.Current, (first, count, v));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ViewportArrayv, cmd_size);
   cmd->first = first;
   cmd->count = count;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, v, v_size);
}


/* ViewportIndexedf: marshalled asynchronously */
struct marshal_cmd_ViewportIndexedf
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLfloat x;
   GLfloat y;
   GLfloat w;
   GLfloat h;
};
uint32_t
_mesa_unmarshal_ViewportIndexedf(struct gl_context *ctx, const struct marshal_cmd_ViewportIndexedf *restrict cmd)
{
   GLuint index = cmd->index;
   GLfloat x = cmd->x;
   GLfloat y = cmd->y;
   GLfloat w = cmd->w;
   GLfloat h = cmd->h;
   CALL_ViewportIndexedf(ctx->Dispatch.Current, (index, x, y, w, h));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_ViewportIndexedf), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ViewportIndexedf(GLuint index, GLfloat x, GLfloat y, GLfloat w, GLfloat h)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ViewportIndexedf);
   struct marshal_cmd_ViewportIndexedf *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ViewportIndexedf, cmd_size);
   cmd->index = index;
   cmd->x = x;
   cmd->y = y;
   cmd->w = w;
   cmd->h = h;
}


/* ViewportIndexedfv: marshalled asynchronously */
struct marshal_cmd_ViewportIndexedfv
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLfloat v[4];
};
uint32_t
_mesa_unmarshal_ViewportIndexedfv(struct gl_context *ctx, const struct marshal_cmd_ViewportIndexedfv *restrict cmd)
{
   GLuint index = cmd->index;
   const GLfloat *v = cmd->v;
   CALL_ViewportIndexedfv(ctx->Dispatch.Current, (index, v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_ViewportIndexedfv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ViewportIndexedfv(GLuint index, const GLfloat *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ViewportIndexedfv);
   struct marshal_cmd_ViewportIndexedfv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ViewportIndexedfv, cmd_size);
   cmd->index = index;
   memcpy(cmd->v, v, 4 * sizeof(GLfloat));
}


/* ScissorArrayv: marshalled asynchronously */
struct marshal_cmd_ScissorArrayv
{
   struct marshal_cmd_base cmd_base;
   GLuint first;
   GLsizei count;
   /* Next safe_mul(count, 4 * sizeof(int)) bytes are int v[count][4] */
};
uint32_t
_mesa_unmarshal_ScissorArrayv(struct gl_context *ctx, const struct marshal_cmd_ScissorArrayv *restrict cmd)
{
   GLuint first = cmd->first;
   GLsizei count = cmd->count;
   int *v;
   const char *variable_data = (const char *) (cmd + 1);
   v = (int *) variable_data;
   CALL_ScissorArrayv(ctx->Dispatch.Current, (first, count, v));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ScissorArrayv(GLuint first, GLsizei count, const int *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int v_size = safe_mul(count, 4 * sizeof(int));
   int cmd_size = sizeof(struct marshal_cmd_ScissorArrayv) + v_size;
   struct marshal_cmd_ScissorArrayv *cmd;
   if (unlikely(v_size < 0 || (v_size > 0 && !v) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "ScissorArrayv");
      CALL_ScissorArrayv(ctx->Dispatch.Current, (first, count, v));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ScissorArrayv, cmd_size);
   cmd->first = first;
   cmd->count = count;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, v, v_size);
}


/* ScissorIndexed: marshalled asynchronously */
struct marshal_cmd_ScissorIndexed
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLint left;
   GLint bottom;
   GLsizei width;
   GLsizei height;
};
uint32_t
_mesa_unmarshal_ScissorIndexed(struct gl_context *ctx, const struct marshal_cmd_ScissorIndexed *restrict cmd)
{
   GLuint index = cmd->index;
   GLint left = cmd->left;
   GLint bottom = cmd->bottom;
   GLsizei width = cmd->width;
   GLsizei height = cmd->height;
   CALL_ScissorIndexed(ctx->Dispatch.Current, (index, left, bottom, width, height));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_ScissorIndexed), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ScissorIndexed(GLuint index, GLint left, GLint bottom, GLsizei width, GLsizei height)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ScissorIndexed);
   struct marshal_cmd_ScissorIndexed *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ScissorIndexed, cmd_size);
   cmd->index = index;
   cmd->left = left;
   cmd->bottom = bottom;
   cmd->width = width;
   cmd->height = height;
}


/* ScissorIndexedv: marshalled asynchronously */
struct marshal_cmd_ScissorIndexedv
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLint v[4];
};
uint32_t
_mesa_unmarshal_ScissorIndexedv(struct gl_context *ctx, const struct marshal_cmd_ScissorIndexedv *restrict cmd)
{
   GLuint index = cmd->index;
   const GLint *v = cmd->v;
   CALL_ScissorIndexedv(ctx->Dispatch.Current, (index, v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_ScissorIndexedv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ScissorIndexedv(GLuint index, const GLint *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ScissorIndexedv);
   struct marshal_cmd_ScissorIndexedv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ScissorIndexedv, cmd_size);
   cmd->index = index;
   memcpy(cmd->v, v, 4 * sizeof(GLint));
}


/* DepthRangeArrayv: marshalled asynchronously */
struct marshal_cmd_DepthRangeArrayv
{
   struct marshal_cmd_base cmd_base;
   GLuint first;
   GLsizei count;
   /* Next safe_mul(count, 2 * sizeof(GLclampd)) bytes are GLclampd v[count][2] */
};
uint32_t
_mesa_unmarshal_DepthRangeArrayv(struct gl_context *ctx, const struct marshal_cmd_DepthRangeArrayv *restrict cmd)
{
   GLuint first = cmd->first;
   GLsizei count = cmd->count;
   GLclampd *v;
   const char *variable_data = (const char *) (cmd + 1);
   v = (GLclampd *) variable_data;
   CALL_DepthRangeArrayv(ctx->Dispatch.Current, (first, count, v));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_DepthRangeArrayv(GLuint first, GLsizei count, const GLclampd *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int v_size = safe_mul(count, 2 * sizeof(GLclampd));
   int cmd_size = sizeof(struct marshal_cmd_DepthRangeArrayv) + v_size;
   struct marshal_cmd_DepthRangeArrayv *cmd;
   if (unlikely(v_size < 0 || (v_size > 0 && !v) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "DepthRangeArrayv");
      CALL_DepthRangeArrayv(ctx->Dispatch.Current, (first, count, v));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_DepthRangeArrayv, cmd_size);
   cmd->first = first;
   cmd->count = count;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, v, v_size);
}


/* DepthRangeIndexed: marshalled asynchronously */
struct marshal_cmd_DepthRangeIndexed
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLclampd n;
   GLclampd f;
};
uint32_t
_mesa_unmarshal_DepthRangeIndexed(struct gl_context *ctx, const struct marshal_cmd_DepthRangeIndexed *restrict cmd)
{
   GLuint index = cmd->index;
   GLclampd n = cmd->n;
   GLclampd f = cmd->f;
   CALL_DepthRangeIndexed(ctx->Dispatch.Current, (index, n, f));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_DepthRangeIndexed), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_DepthRangeIndexed(GLuint index, GLclampd n, GLclampd f)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_DepthRangeIndexed);
   struct marshal_cmd_DepthRangeIndexed *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_DepthRangeIndexed, cmd_size);
   cmd->index = index;
   cmd->n = n;
   cmd->f = f;
}


/* GetFloati_v: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetFloati_v(GLenum target, GLuint index, GLfloat *data)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetFloati_v");
   CALL_GetFloati_v(ctx->Dispatch.Current, (target, index, data));
}


/* GetDoublei_v: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetDoublei_v(GLenum target, GLuint index, GLdouble *data)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetDoublei_v");
   CALL_GetDoublei_v(ctx->Dispatch.Current, (target, index, data));
}


/* FramebufferSampleLocationsfvARB: marshalled asynchronously */
struct marshal_cmd_FramebufferSampleLocationsfvARB
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLuint start;
   GLsizei count;
   /* Next safe_mul((2 * count), 1 * sizeof(GLfloat)) bytes are GLfloat v[(2 * count)] */
};
uint32_t
_mesa_unmarshal_FramebufferSampleLocationsfvARB(struct gl_context *ctx, const struct marshal_cmd_FramebufferSampleLocationsfvARB *restrict cmd)
{
   GLenum target = cmd->target;
   GLuint start = cmd->start;
   GLsizei count = cmd->count;
   GLfloat *v;
   const char *variable_data = (const char *) (cmd + 1);
   v = (GLfloat *) variable_data;
   CALL_FramebufferSampleLocationsfvARB(ctx->Dispatch.Current, (target, start, count, v));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_FramebufferSampleLocationsfvARB(GLenum target, GLuint start, GLsizei count, const GLfloat *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int v_size = safe_mul((2 * count), 1 * sizeof(GLfloat));
   int cmd_size = sizeof(struct marshal_cmd_FramebufferSampleLocationsfvARB) + v_size;
   struct marshal_cmd_FramebufferSampleLocationsfvARB *cmd;
   if (unlikely(v_size < 0 || (v_size > 0 && !v) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "FramebufferSampleLocationsfvARB");
      CALL_FramebufferSampleLocationsfvARB(ctx->Dispatch.Current, (target, start, count, v));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_FramebufferSampleLocationsfvARB, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->start = start;
   cmd->count = count;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, v, v_size);
}


/* NamedFramebufferSampleLocationsfvARB: marshalled asynchronously */
struct marshal_cmd_NamedFramebufferSampleLocationsfvARB
{
   struct marshal_cmd_base cmd_base;
   GLuint framebuffer;
   GLuint start;
   GLsizei count;
   /* Next safe_mul((2 * count), 1 * sizeof(GLfloat)) bytes are GLfloat v[(2 * count)] */
};
uint32_t
_mesa_unmarshal_NamedFramebufferSampleLocationsfvARB(struct gl_context *ctx, const struct marshal_cmd_NamedFramebufferSampleLocationsfvARB *restrict cmd)
{
   GLuint framebuffer = cmd->framebuffer;
   GLuint start = cmd->start;
   GLsizei count = cmd->count;
   GLfloat *v;
   const char *variable_data = (const char *) (cmd + 1);
   v = (GLfloat *) variable_data;
   CALL_NamedFramebufferSampleLocationsfvARB(ctx->Dispatch.Current, (framebuffer, start, count, v));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_NamedFramebufferSampleLocationsfvARB(GLuint framebuffer, GLuint start, GLsizei count, const GLfloat *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int v_size = safe_mul((2 * count), 1 * sizeof(GLfloat));
   int cmd_size = sizeof(struct marshal_cmd_NamedFramebufferSampleLocationsfvARB) + v_size;
   struct marshal_cmd_NamedFramebufferSampleLocationsfvARB *cmd;
   if (unlikely(v_size < 0 || (v_size > 0 && !v) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "NamedFramebufferSampleLocationsfvARB");
      CALL_NamedFramebufferSampleLocationsfvARB(ctx->Dispatch.Current, (framebuffer, start, count, v));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_NamedFramebufferSampleLocationsfvARB, cmd_size);
   cmd->framebuffer = framebuffer;
   cmd->start = start;
   cmd->count = count;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, v, v_size);
}


/* EvaluateDepthValuesARB: marshalled asynchronously */
struct marshal_cmd_EvaluateDepthValuesARB
{
   struct marshal_cmd_base cmd_base;
};
uint32_t
_mesa_unmarshal_EvaluateDepthValuesARB(struct gl_context *ctx, const struct marshal_cmd_EvaluateDepthValuesARB *restrict cmd)
{
   CALL_EvaluateDepthValuesARB(ctx->Dispatch.Current, ());
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_EvaluateDepthValuesARB), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_EvaluateDepthValuesARB(void)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_EvaluateDepthValuesARB);
   struct marshal_cmd_EvaluateDepthValuesARB *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_EvaluateDepthValuesARB, cmd_size);
   (void) cmd;
}


/* WindowPos4dMESA: marshalled asynchronously */
struct marshal_cmd_WindowPos4dMESA
{
   struct marshal_cmd_base cmd_base;
   GLdouble x;
   GLdouble y;
   GLdouble z;
   GLdouble w;
};
uint32_t
_mesa_unmarshal_WindowPos4dMESA(struct gl_context *ctx, const struct marshal_cmd_WindowPos4dMESA *restrict cmd)
{
   GLdouble x = cmd->x;
   GLdouble y = cmd->y;
   GLdouble z = cmd->z;
   GLdouble w = cmd->w;
   CALL_WindowPos4dMESA(ctx->Dispatch.Current, (x, y, z, w));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_WindowPos4dMESA), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_WindowPos4dMESA(GLdouble x, GLdouble y, GLdouble z, GLdouble w)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_WindowPos4dMESA);
   struct marshal_cmd_WindowPos4dMESA *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_WindowPos4dMESA, cmd_size);
   cmd->x = x;
   cmd->y = y;
   cmd->z = z;
   cmd->w = w;
}


/* WindowPos4dvMESA: marshalled asynchronously */
struct marshal_cmd_WindowPos4dvMESA
{
   struct marshal_cmd_base cmd_base;
   GLdouble v[4];
};
uint32_t
_mesa_unmarshal_WindowPos4dvMESA(struct gl_context *ctx, const struct marshal_cmd_WindowPos4dvMESA *restrict cmd)
{
   const GLdouble *v = cmd->v;
   CALL_WindowPos4dvMESA(ctx->Dispatch.Current, (v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_WindowPos4dvMESA), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_WindowPos4dvMESA(const GLdouble *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_WindowPos4dvMESA);
   struct marshal_cmd_WindowPos4dvMESA *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_WindowPos4dvMESA, cmd_size);
   memcpy(cmd->v, v, 4 * sizeof(GLdouble));
}


/* WindowPos4fMESA: marshalled asynchronously */
struct marshal_cmd_WindowPos4fMESA
{
   struct marshal_cmd_base cmd_base;
   GLfloat x;
   GLfloat y;
   GLfloat z;
   GLfloat w;
};
uint32_t
_mesa_unmarshal_WindowPos4fMESA(struct gl_context *ctx, const struct marshal_cmd_WindowPos4fMESA *restrict cmd)
{
   GLfloat x = cmd->x;
   GLfloat y = cmd->y;
   GLfloat z = cmd->z;
   GLfloat w = cmd->w;
   CALL_WindowPos4fMESA(ctx->Dispatch.Current, (x, y, z, w));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_WindowPos4fMESA), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_WindowPos4fMESA(GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_WindowPos4fMESA);
   struct marshal_cmd_WindowPos4fMESA *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_WindowPos4fMESA, cmd_size);
   cmd->x = x;
   cmd->y = y;
   cmd->z = z;
   cmd->w = w;
}


/* WindowPos4fvMESA: marshalled asynchronously */
struct marshal_cmd_WindowPos4fvMESA
{
   struct marshal_cmd_base cmd_base;
   GLfloat v[4];
};
uint32_t
_mesa_unmarshal_WindowPos4fvMESA(struct gl_context *ctx, const struct marshal_cmd_WindowPos4fvMESA *restrict cmd)
{
   const GLfloat *v = cmd->v;
   CALL_WindowPos4fvMESA(ctx->Dispatch.Current, (v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_WindowPos4fvMESA), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_WindowPos4fvMESA(const GLfloat *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_WindowPos4fvMESA);
   struct marshal_cmd_WindowPos4fvMESA *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_WindowPos4fvMESA, cmd_size);
   memcpy(cmd->v, v, 4 * sizeof(GLfloat));
}


/* WindowPos4iMESA: marshalled asynchronously */
struct marshal_cmd_WindowPos4iMESA
{
   struct marshal_cmd_base cmd_base;
   GLint x;
   GLint y;
   GLint z;
   GLint w;
};
uint32_t
_mesa_unmarshal_WindowPos4iMESA(struct gl_context *ctx, const struct marshal_cmd_WindowPos4iMESA *restrict cmd)
{
   GLint x = cmd->x;
   GLint y = cmd->y;
   GLint z = cmd->z;
   GLint w = cmd->w;
   CALL_WindowPos4iMESA(ctx->Dispatch.Current, (x, y, z, w));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_WindowPos4iMESA), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_WindowPos4iMESA(GLint x, GLint y, GLint z, GLint w)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_WindowPos4iMESA);
   struct marshal_cmd_WindowPos4iMESA *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_WindowPos4iMESA, cmd_size);
   cmd->x = x;
   cmd->y = y;
   cmd->z = z;
   cmd->w = w;
}


/* WindowPos4ivMESA: marshalled asynchronously */
struct marshal_cmd_WindowPos4ivMESA
{
   struct marshal_cmd_base cmd_base;
   GLint v[4];
};
uint32_t
_mesa_unmarshal_WindowPos4ivMESA(struct gl_context *ctx, const struct marshal_cmd_WindowPos4ivMESA *restrict cmd)
{
   const GLint *v = cmd->v;
   CALL_WindowPos4ivMESA(ctx->Dispatch.Current, (v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_WindowPos4ivMESA), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_WindowPos4ivMESA(const GLint *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_WindowPos4ivMESA);
   struct marshal_cmd_WindowPos4ivMESA *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_WindowPos4ivMESA, cmd_size);
   memcpy(cmd->v, v, 4 * sizeof(GLint));
}


/* WindowPos4sMESA: marshalled asynchronously */
struct marshal_cmd_WindowPos4sMESA
{
   struct marshal_cmd_base cmd_base;
   GLshort x;
   GLshort y;
   GLshort z;
   GLshort w;
};
uint32_t
_mesa_unmarshal_WindowPos4sMESA(struct gl_context *ctx, const struct marshal_cmd_WindowPos4sMESA *restrict cmd)
{
   GLshort x = cmd->x;
   GLshort y = cmd->y;
   GLshort z = cmd->z;
   GLshort w = cmd->w;
   CALL_WindowPos4sMESA(ctx->Dispatch.Current, (x, y, z, w));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_WindowPos4sMESA), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_WindowPos4sMESA(GLshort x, GLshort y, GLshort z, GLshort w)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_WindowPos4sMESA);
   struct marshal_cmd_WindowPos4sMESA *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_WindowPos4sMESA, cmd_size);
   cmd->x = x;
   cmd->y = y;
   cmd->z = z;
   cmd->w = w;
}


/* WindowPos4svMESA: marshalled asynchronously */
struct marshal_cmd_WindowPos4svMESA
{
   struct marshal_cmd_base cmd_base;
   GLshort v[4];
};
uint32_t
_mesa_unmarshal_WindowPos4svMESA(struct gl_context *ctx, const struct marshal_cmd_WindowPos4svMESA *restrict cmd)
{
   const GLshort *v = cmd->v;
   CALL_WindowPos4svMESA(ctx->Dispatch.Current, (v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_WindowPos4svMESA), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_WindowPos4svMESA(const GLshort *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_WindowPos4svMESA);
   struct marshal_cmd_WindowPos4svMESA *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_WindowPos4svMESA, cmd_size);
   memcpy(cmd->v, v, 4 * sizeof(GLshort));
}


/* VertexAttrib1sNV: marshalled asynchronously */
struct marshal_cmd_VertexAttrib1sNV
{
   struct marshal_cmd_base cmd_base;
   GLshort x;
   GLuint index;
};
uint32_t
_mesa_unmarshal_VertexAttrib1sNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib1sNV *restrict cmd)
{
   GLuint index = cmd->index;
   GLshort x = cmd->x;
   CALL_VertexAttrib1sNV(ctx->Dispatch.Current, (index, x));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexAttrib1sNV), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_VertexAttrib1sNV(GLuint index, GLshort x)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttrib1sNV);
   struct marshal_cmd_VertexAttrib1sNV *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttrib1sNV, cmd_size);
   cmd->index = index;
   cmd->x = x;
}


/* VertexAttrib1svNV: marshalled asynchronously */
struct marshal_cmd_VertexAttrib1svNV
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLshort v[1];
};
uint32_t
_mesa_unmarshal_VertexAttrib1svNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib1svNV *restrict cmd)
{
   GLuint index = cmd->index;
   const GLshort *v = cmd->v;
   CALL_VertexAttrib1svNV(ctx->Dispatch.Current, (index, v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexAttrib1svNV), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_VertexAttrib1svNV(GLuint index, const GLshort *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttrib1svNV);
   struct marshal_cmd_VertexAttrib1svNV *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttrib1svNV, cmd_size);
   cmd->index = index;
   memcpy(cmd->v, v, 1 * sizeof(GLshort));
}


/* VertexAttrib2sNV: marshalled asynchronously */
struct marshal_cmd_VertexAttrib2sNV
{
   struct marshal_cmd_base cmd_base;
   GLshort x;
   GLshort y;
   GLuint index;
};
uint32_t
_mesa_unmarshal_VertexAttrib2sNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib2sNV *restrict cmd)
{
   GLuint index = cmd->index;
   GLshort x = cmd->x;
   GLshort y = cmd->y;
   CALL_VertexAttrib2sNV(ctx->Dispatch.Current, (index, x, y));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexAttrib2sNV), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_VertexAttrib2sNV(GLuint index, GLshort x, GLshort y)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttrib2sNV);
   struct marshal_cmd_VertexAttrib2sNV *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttrib2sNV, cmd_size);
   cmd->index = index;
   cmd->x = x;
   cmd->y = y;
}


/* VertexAttrib2svNV: marshalled asynchronously */
struct marshal_cmd_VertexAttrib2svNV
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLshort v[2];
};
uint32_t
_mesa_unmarshal_VertexAttrib2svNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib2svNV *restrict cmd)
{
   GLuint index = cmd->index;
   const GLshort *v = cmd->v;
   CALL_VertexAttrib2svNV(ctx->Dispatch.Current, (index, v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexAttrib2svNV), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_VertexAttrib2svNV(GLuint index, const GLshort *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttrib2svNV);
   struct marshal_cmd_VertexAttrib2svNV *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttrib2svNV, cmd_size);
   cmd->index = index;
   memcpy(cmd->v, v, 2 * sizeof(GLshort));
}


/* VertexAttrib3sNV: marshalled asynchronously */
struct marshal_cmd_VertexAttrib3sNV
{
   struct marshal_cmd_base cmd_base;
   GLshort x;
   GLshort y;
   GLshort z;
   GLuint index;
};
uint32_t
_mesa_unmarshal_VertexAttrib3sNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib3sNV *restrict cmd)
{
   GLuint index = cmd->index;
   GLshort x = cmd->x;
   GLshort y = cmd->y;
   GLshort z = cmd->z;
   CALL_VertexAttrib3sNV(ctx->Dispatch.Current, (index, x, y, z));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexAttrib3sNV), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_VertexAttrib3sNV(GLuint index, GLshort x, GLshort y, GLshort z)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttrib3sNV);
   struct marshal_cmd_VertexAttrib3sNV *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttrib3sNV, cmd_size);
   cmd->index = index;
   cmd->x = x;
   cmd->y = y;
   cmd->z = z;
}


/* VertexAttrib3svNV: marshalled asynchronously */
struct marshal_cmd_VertexAttrib3svNV
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLshort v[3];
};
uint32_t
_mesa_unmarshal_VertexAttrib3svNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib3svNV *restrict cmd)
{
   GLuint index = cmd->index;
   const GLshort *v = cmd->v;
   CALL_VertexAttrib3svNV(ctx->Dispatch.Current, (index, v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexAttrib3svNV), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_VertexAttrib3svNV(GLuint index, const GLshort *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttrib3svNV);
   struct marshal_cmd_VertexAttrib3svNV *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttrib3svNV, cmd_size);
   cmd->index = index;
   memcpy(cmd->v, v, 3 * sizeof(GLshort));
}


/* VertexAttrib4sNV: marshalled asynchronously */
struct marshal_cmd_VertexAttrib4sNV
{
   struct marshal_cmd_base cmd_base;
   GLshort x;
   GLshort y;
   GLshort z;
   GLshort w;
   GLuint index;
};
uint32_t
_mesa_unmarshal_VertexAttrib4sNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4sNV *restrict cmd)
{
   GLuint index = cmd->index;
   GLshort x = cmd->x;
   GLshort y = cmd->y;
   GLshort z = cmd->z;
   GLshort w = cmd->w;
   CALL_VertexAttrib4sNV(ctx->Dispatch.Current, (index, x, y, z, w));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexAttrib4sNV), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_VertexAttrib4sNV(GLuint index, GLshort x, GLshort y, GLshort z, GLshort w)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttrib4sNV);
   struct marshal_cmd_VertexAttrib4sNV *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttrib4sNV, cmd_size);
   cmd->index = index;
   cmd->x = x;
   cmd->y = y;
   cmd->z = z;
   cmd->w = w;
}


/* VertexAttrib4svNV: marshalled asynchronously */
struct marshal_cmd_VertexAttrib4svNV
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLshort v[4];
};
uint32_t
_mesa_unmarshal_VertexAttrib4svNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4svNV *restrict cmd)
{
   GLuint index = cmd->index;
   const GLshort *v = cmd->v;
   CALL_VertexAttrib4svNV(ctx->Dispatch.Current, (index, v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexAttrib4svNV), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_VertexAttrib4svNV(GLuint index, const GLshort *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttrib4svNV);
   struct marshal_cmd_VertexAttrib4svNV *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttrib4svNV, cmd_size);
   cmd->index = index;
   memcpy(cmd->v, v, 4 * sizeof(GLshort));
}


/* VertexAttrib1fNV: marshalled asynchronously */
struct marshal_cmd_VertexAttrib1fNV
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLfloat x;
};
uint32_t
_mesa_unmarshal_VertexAttrib1fNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib1fNV *restrict cmd)
{
   GLuint index = cmd->index;
   GLfloat x = cmd->x;
   CALL_VertexAttrib1fNV(ctx->Dispatch.Current, (index, x));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexAttrib1fNV), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_VertexAttrib1fNV(GLuint index, GLfloat x)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttrib1fNV);
   struct marshal_cmd_VertexAttrib1fNV *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttrib1fNV, cmd_size);
   cmd->index = index;
   cmd->x = x;
}


/* VertexAttrib1fvNV: marshalled asynchronously */
struct marshal_cmd_VertexAttrib1fvNV
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLfloat v[1];
};
uint32_t
_mesa_unmarshal_VertexAttrib1fvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib1fvNV *restrict cmd)
{
   GLuint index = cmd->index;
   const GLfloat *v = cmd->v;
   CALL_VertexAttrib1fvNV(ctx->Dispatch.Current, (index, v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexAttrib1fvNV), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_VertexAttrib1fvNV(GLuint index, const GLfloat *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttrib1fvNV);
   struct marshal_cmd_VertexAttrib1fvNV *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttrib1fvNV, cmd_size);
   cmd->index = index;
   memcpy(cmd->v, v, 1 * sizeof(GLfloat));
}


/* VertexAttrib2fNV: marshalled asynchronously */
struct marshal_cmd_VertexAttrib2fNV
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLfloat x;
   GLfloat y;
};
uint32_t
_mesa_unmarshal_VertexAttrib2fNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib2fNV *restrict cmd)
{
   GLuint index = cmd->index;
   GLfloat x = cmd->x;
   GLfloat y = cmd->y;
   CALL_VertexAttrib2fNV(ctx->Dispatch.Current, (index, x, y));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexAttrib2fNV), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_VertexAttrib2fNV(GLuint index, GLfloat x, GLfloat y)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttrib2fNV);
   struct marshal_cmd_VertexAttrib2fNV *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttrib2fNV, cmd_size);
   cmd->index = index;
   cmd->x = x;
   cmd->y = y;
}


/* VertexAttrib2fvNV: marshalled asynchronously */
struct marshal_cmd_VertexAttrib2fvNV
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLfloat v[2];
};
uint32_t
_mesa_unmarshal_VertexAttrib2fvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib2fvNV *restrict cmd)
{
   GLuint index = cmd->index;
   const GLfloat *v = cmd->v;
   CALL_VertexAttrib2fvNV(ctx->Dispatch.Current, (index, v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexAttrib2fvNV), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_VertexAttrib2fvNV(GLuint index, const GLfloat *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttrib2fvNV);
   struct marshal_cmd_VertexAttrib2fvNV *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttrib2fvNV, cmd_size);
   cmd->index = index;
   memcpy(cmd->v, v, 2 * sizeof(GLfloat));
}


/* VertexAttrib3fNV: marshalled asynchronously */
struct marshal_cmd_VertexAttrib3fNV
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLfloat x;
   GLfloat y;
   GLfloat z;
};
uint32_t
_mesa_unmarshal_VertexAttrib3fNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib3fNV *restrict cmd)
{
   GLuint index = cmd->index;
   GLfloat x = cmd->x;
   GLfloat y = cmd->y;
   GLfloat z = cmd->z;
   CALL_VertexAttrib3fNV(ctx->Dispatch.Current, (index, x, y, z));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexAttrib3fNV), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_VertexAttrib3fNV(GLuint index, GLfloat x, GLfloat y, GLfloat z)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttrib3fNV);
   struct marshal_cmd_VertexAttrib3fNV *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttrib3fNV, cmd_size);
   cmd->index = index;
   cmd->x = x;
   cmd->y = y;
   cmd->z = z;
}


/* VertexAttrib3fvNV: marshalled asynchronously */
struct marshal_cmd_VertexAttrib3fvNV
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLfloat v[3];
};
uint32_t
_mesa_unmarshal_VertexAttrib3fvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib3fvNV *restrict cmd)
{
   GLuint index = cmd->index;
   const GLfloat *v = cmd->v;
   CALL_VertexAttrib3fvNV(ctx->Dispatch.Current, (index, v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexAttrib3fvNV), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_VertexAttrib3fvNV(GLuint index, const GLfloat *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttrib3fvNV);
   struct marshal_cmd_VertexAttrib3fvNV *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttrib3fvNV, cmd_size);
   cmd->index = index;
   memcpy(cmd->v, v, 3 * sizeof(GLfloat));
}


/* VertexAttrib4fNV: marshalled asynchronously */
struct marshal_cmd_VertexAttrib4fNV
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLfloat x;
   GLfloat y;
   GLfloat z;
   GLfloat w;
};
uint32_t
_mesa_unmarshal_VertexAttrib4fNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4fNV *restrict cmd)
{
   GLuint index = cmd->index;
   GLfloat x = cmd->x;
   GLfloat y = cmd->y;
   GLfloat z = cmd->z;
   GLfloat w = cmd->w;
   CALL_VertexAttrib4fNV(ctx->Dispatch.Current, (index, x, y, z, w));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexAttrib4fNV), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_VertexAttrib4fNV(GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttrib4fNV);
   struct marshal_cmd_VertexAttrib4fNV *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttrib4fNV, cmd_size);
   cmd->index = index;
   cmd->x = x;
   cmd->y = y;
   cmd->z = z;
   cmd->w = w;
}


/* VertexAttrib4fvNV: marshalled asynchronously */
struct marshal_cmd_VertexAttrib4fvNV
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLfloat v[4];
};
uint32_t
_mesa_unmarshal_VertexAttrib4fvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4fvNV *restrict cmd)
{
   GLuint index = cmd->index;
   const GLfloat *v = cmd->v;
   CALL_VertexAttrib4fvNV(ctx->Dispatch.Current, (index, v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexAttrib4fvNV), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_VertexAttrib4fvNV(GLuint index, const GLfloat *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttrib4fvNV);
   struct marshal_cmd_VertexAttrib4fvNV *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttrib4fvNV, cmd_size);
   cmd->index = index;
   memcpy(cmd->v, v, 4 * sizeof(GLfloat));
}


/* VertexAttrib1dNV: marshalled asynchronously */
struct marshal_cmd_VertexAttrib1dNV
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLdouble x;
};
uint32_t
_mesa_unmarshal_VertexAttrib1dNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib1dNV *restrict cmd)
{
   GLuint index = cmd->index;
   GLdouble x = cmd->x;
   CALL_VertexAttrib1dNV(ctx->Dispatch.Current, (index, x));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexAttrib1dNV), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_VertexAttrib1dNV(GLuint index, GLdouble x)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttrib1dNV);
   struct marshal_cmd_VertexAttrib1dNV *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttrib1dNV, cmd_size);
   cmd->index = index;
   cmd->x = x;
}


/* VertexAttrib1dvNV: marshalled asynchronously */
struct marshal_cmd_VertexAttrib1dvNV
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLdouble v[1];
};
uint32_t
_mesa_unmarshal_VertexAttrib1dvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib1dvNV *restrict cmd)
{
   GLuint index = cmd->index;
   const GLdouble *v = cmd->v;
   CALL_VertexAttrib1dvNV(ctx->Dispatch.Current, (index, v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexAttrib1dvNV), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_VertexAttrib1dvNV(GLuint index, const GLdouble *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttrib1dvNV);
   struct marshal_cmd_VertexAttrib1dvNV *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttrib1dvNV, cmd_size);
   cmd->index = index;
   memcpy(cmd->v, v, 1 * sizeof(GLdouble));
}


/* VertexAttrib2dNV: marshalled asynchronously */
struct marshal_cmd_VertexAttrib2dNV
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLdouble x;
   GLdouble y;
};
uint32_t
_mesa_unmarshal_VertexAttrib2dNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib2dNV *restrict cmd)
{
   GLuint index = cmd->index;
   GLdouble x = cmd->x;
   GLdouble y = cmd->y;
   CALL_VertexAttrib2dNV(ctx->Dispatch.Current, (index, x, y));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexAttrib2dNV), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_VertexAttrib2dNV(GLuint index, GLdouble x, GLdouble y)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttrib2dNV);
   struct marshal_cmd_VertexAttrib2dNV *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttrib2dNV, cmd_size);
   cmd->index = index;
   cmd->x = x;
   cmd->y = y;
}


/* VertexAttrib2dvNV: marshalled asynchronously */
struct marshal_cmd_VertexAttrib2dvNV
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLdouble v[2];
};
uint32_t
_mesa_unmarshal_VertexAttrib2dvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib2dvNV *restrict cmd)
{
   GLuint index = cmd->index;
   const GLdouble *v = cmd->v;
   CALL_VertexAttrib2dvNV(ctx->Dispatch.Current, (index, v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexAttrib2dvNV), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_VertexAttrib2dvNV(GLuint index, const GLdouble *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttrib2dvNV);
   struct marshal_cmd_VertexAttrib2dvNV *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttrib2dvNV, cmd_size);
   cmd->index = index;
   memcpy(cmd->v, v, 2 * sizeof(GLdouble));
}


/* VertexAttrib3dNV: marshalled asynchronously */
struct marshal_cmd_VertexAttrib3dNV
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLdouble x;
   GLdouble y;
   GLdouble z;
};
uint32_t
_mesa_unmarshal_VertexAttrib3dNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib3dNV *restrict cmd)
{
   GLuint index = cmd->index;
   GLdouble x = cmd->x;
   GLdouble y = cmd->y;
   GLdouble z = cmd->z;
   CALL_VertexAttrib3dNV(ctx->Dispatch.Current, (index, x, y, z));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexAttrib3dNV), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_VertexAttrib3dNV(GLuint index, GLdouble x, GLdouble y, GLdouble z)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttrib3dNV);
   struct marshal_cmd_VertexAttrib3dNV *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttrib3dNV, cmd_size);
   cmd->index = index;
   cmd->x = x;
   cmd->y = y;
   cmd->z = z;
}


/* VertexAttrib3dvNV: marshalled asynchronously */
struct marshal_cmd_VertexAttrib3dvNV
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLdouble v[3];
};
uint32_t
_mesa_unmarshal_VertexAttrib3dvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib3dvNV *restrict cmd)
{
   GLuint index = cmd->index;
   const GLdouble *v = cmd->v;
   CALL_VertexAttrib3dvNV(ctx->Dispatch.Current, (index, v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexAttrib3dvNV), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_VertexAttrib3dvNV(GLuint index, const GLdouble *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttrib3dvNV);
   struct marshal_cmd_VertexAttrib3dvNV *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttrib3dvNV, cmd_size);
   cmd->index = index;
   memcpy(cmd->v, v, 3 * sizeof(GLdouble));
}


/* VertexAttrib4dNV: marshalled asynchronously */
struct marshal_cmd_VertexAttrib4dNV
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLdouble x;
   GLdouble y;
   GLdouble z;
   GLdouble w;
};
uint32_t
_mesa_unmarshal_VertexAttrib4dNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4dNV *restrict cmd)
{
   GLuint index = cmd->index;
   GLdouble x = cmd->x;
   GLdouble y = cmd->y;
   GLdouble z = cmd->z;
   GLdouble w = cmd->w;
   CALL_VertexAttrib4dNV(ctx->Dispatch.Current, (index, x, y, z, w));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexAttrib4dNV), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_VertexAttrib4dNV(GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttrib4dNV);
   struct marshal_cmd_VertexAttrib4dNV *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttrib4dNV, cmd_size);
   cmd->index = index;
   cmd->x = x;
   cmd->y = y;
   cmd->z = z;
   cmd->w = w;
}


/* VertexAttrib4dvNV: marshalled asynchronously */
struct marshal_cmd_VertexAttrib4dvNV
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLdouble v[4];
};
uint32_t
_mesa_unmarshal_VertexAttrib4dvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4dvNV *restrict cmd)
{
   GLuint index = cmd->index;
   const GLdouble *v = cmd->v;
   CALL_VertexAttrib4dvNV(ctx->Dispatch.Current, (index, v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexAttrib4dvNV), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_VertexAttrib4dvNV(GLuint index, const GLdouble *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttrib4dvNV);
   struct marshal_cmd_VertexAttrib4dvNV *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttrib4dvNV, cmd_size);
   cmd->index = index;
   memcpy(cmd->v, v, 4 * sizeof(GLdouble));
}


/* VertexAttrib4ubNV: marshalled asynchronously */
struct marshal_cmd_VertexAttrib4ubNV
{
   struct marshal_cmd_base cmd_base;
   GLubyte x;
   GLubyte y;
   GLubyte z;
   GLubyte w;
   GLuint index;
};
uint32_t
_mesa_unmarshal_VertexAttrib4ubNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4ubNV *restrict cmd)
{
   GLuint index = cmd->index;
   GLubyte x = cmd->x;
   GLubyte y = cmd->y;
   GLubyte z = cmd->z;
   GLubyte w = cmd->w;
   CALL_VertexAttrib4ubNV(ctx->Dispatch.Current, (index, x, y, z, w));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexAttrib4ubNV), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_VertexAttrib4ubNV(GLuint index, GLubyte x, GLubyte y, GLubyte z, GLubyte w)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttrib4ubNV);
   struct marshal_cmd_VertexAttrib4ubNV *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttrib4ubNV, cmd_size);
   cmd->index = index;
   cmd->x = x;
   cmd->y = y;
   cmd->z = z;
   cmd->w = w;
}


/* VertexAttrib4ubvNV: marshalled asynchronously */
struct marshal_cmd_VertexAttrib4ubvNV
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLubyte v[4];
};
uint32_t
_mesa_unmarshal_VertexAttrib4ubvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4ubvNV *restrict cmd)
{
   GLuint index = cmd->index;
   const GLubyte *v = cmd->v;
   CALL_VertexAttrib4ubvNV(ctx->Dispatch.Current, (index, v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexAttrib4ubvNV), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_VertexAttrib4ubvNV(GLuint index, const GLubyte *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttrib4ubvNV);
   struct marshal_cmd_VertexAttrib4ubvNV *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttrib4ubvNV, cmd_size);
   cmd->index = index;
   memcpy(cmd->v, v, 4 * sizeof(GLubyte));
}


/* VertexAttribs1svNV: marshalled asynchronously */
struct marshal_cmd_VertexAttribs1svNV
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLsizei n;
   /* Next safe_mul(n, 1 * sizeof(GLshort)) bytes are GLshort v[n] */
};
uint32_t
_mesa_unmarshal_VertexAttribs1svNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttribs1svNV *restrict cmd)
{
   GLuint index = cmd->index;
   GLsizei n = cmd->n;
   GLshort *v;
   const char *variable_data = (const char *) (cmd + 1);
   v = (GLshort *) variable_data;
   CALL_VertexAttribs1svNV(ctx->Dispatch.Current, (index, n, v));
   return cmd->cmd_base.cmd_size;
}
void GLAPIENTRY
_mesa_marshal_VertexAttribs1svNV(GLuint index, GLsizei n, const GLshort *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int v_size = safe_mul(n, 1 * sizeof(GLshort));
   int cmd_size = sizeof(struct marshal_cmd_VertexAttribs1svNV) + v_size;
   struct marshal_cmd_VertexAttribs1svNV *cmd;
   if (unlikely(v_size < 0 || (v_size > 0 && !v) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "VertexAttribs1svNV");
      CALL_VertexAttribs1svNV(ctx->Dispatch.Current, (index, n, v));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttribs1svNV, cmd_size);
   cmd->index = index;
   cmd->n = n;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, v, v_size);
}


/* VertexAttribs2svNV: marshalled asynchronously */
struct marshal_cmd_VertexAttribs2svNV
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLsizei n;
   /* Next safe_mul(n, 2 * sizeof(GLshort)) bytes are GLshort v[n][2] */
};
uint32_t
_mesa_unmarshal_VertexAttribs2svNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttribs2svNV *restrict cmd)
{
   GLuint index = cmd->index;
   GLsizei n = cmd->n;
   GLshort *v;
   const char *variable_data = (const char *) (cmd + 1);
   v = (GLshort *) variable_data;
   CALL_VertexAttribs2svNV(ctx->Dispatch.Current, (index, n, v));
   return cmd->cmd_base.cmd_size;
}
void GLAPIENTRY
_mesa_marshal_VertexAttribs2svNV(GLuint index, GLsizei n, const GLshort *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int v_size = safe_mul(n, 2 * sizeof(GLshort));
   int cmd_size = sizeof(struct marshal_cmd_VertexAttribs2svNV) + v_size;
   struct marshal_cmd_VertexAttribs2svNV *cmd;
   if (unlikely(v_size < 0 || (v_size > 0 && !v) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "VertexAttribs2svNV");
      CALL_VertexAttribs2svNV(ctx->Dispatch.Current, (index, n, v));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttribs2svNV, cmd_size);
   cmd->index = index;
   cmd->n = n;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, v, v_size);
}


/* VertexAttribs3svNV: marshalled asynchronously */
struct marshal_cmd_VertexAttribs3svNV
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLsizei n;
   /* Next safe_mul(n, 3 * sizeof(GLshort)) bytes are GLshort v[n][3] */
};
uint32_t
_mesa_unmarshal_VertexAttribs3svNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttribs3svNV *restrict cmd)
{
   GLuint index = cmd->index;
   GLsizei n = cmd->n;
   GLshort *v;
   const char *variable_data = (const char *) (cmd + 1);
   v = (GLshort *) variable_data;
   CALL_VertexAttribs3svNV(ctx->Dispatch.Current, (index, n, v));
   return cmd->cmd_base.cmd_size;
}
void GLAPIENTRY
_mesa_marshal_VertexAttribs3svNV(GLuint index, GLsizei n, const GLshort *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int v_size = safe_mul(n, 3 * sizeof(GLshort));
   int cmd_size = sizeof(struct marshal_cmd_VertexAttribs3svNV) + v_size;
   struct marshal_cmd_VertexAttribs3svNV *cmd;
   if (unlikely(v_size < 0 || (v_size > 0 && !v) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "VertexAttribs3svNV");
      CALL_VertexAttribs3svNV(ctx->Dispatch.Current, (index, n, v));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttribs3svNV, cmd_size);
   cmd->index = index;
   cmd->n = n;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, v, v_size);
}


/* VertexAttribs4svNV: marshalled asynchronously */
struct marshal_cmd_VertexAttribs4svNV
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLsizei n;
   /* Next safe_mul(n, 4 * sizeof(GLshort)) bytes are GLshort v[n][4] */
};
uint32_t
_mesa_unmarshal_VertexAttribs4svNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttribs4svNV *restrict cmd)
{
   GLuint index = cmd->index;
   GLsizei n = cmd->n;
   GLshort *v;
   const char *variable_data = (const char *) (cmd + 1);
   v = (GLshort *) variable_data;
   CALL_VertexAttribs4svNV(ctx->Dispatch.Current, (index, n, v));
   return cmd->cmd_base.cmd_size;
}
void GLAPIENTRY
_mesa_marshal_VertexAttribs4svNV(GLuint index, GLsizei n, const GLshort *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int v_size = safe_mul(n, 4 * sizeof(GLshort));
   int cmd_size = sizeof(struct marshal_cmd_VertexAttribs4svNV) + v_size;
   struct marshal_cmd_VertexAttribs4svNV *cmd;
   if (unlikely(v_size < 0 || (v_size > 0 && !v) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "VertexAttribs4svNV");
      CALL_VertexAttribs4svNV(ctx->Dispatch.Current, (index, n, v));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttribs4svNV, cmd_size);
   cmd->index = index;
   cmd->n = n;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, v, v_size);
}


/* VertexAttribs1fvNV: marshalled asynchronously */
struct marshal_cmd_VertexAttribs1fvNV
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLsizei n;
   /* Next safe_mul(n, 1 * sizeof(GLfloat)) bytes are GLfloat v[n] */
};
uint32_t
_mesa_unmarshal_VertexAttribs1fvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttribs1fvNV *restrict cmd)
{
   GLuint index = cmd->index;
   GLsizei n = cmd->n;
   GLfloat *v;
   const char *variable_data = (const char *) (cmd + 1);
   v = (GLfloat *) variable_data;
   CALL_VertexAttribs1fvNV(ctx->Dispatch.Current, (index, n, v));
   return cmd->cmd_base.cmd_size;
}
void GLAPIENTRY
_mesa_marshal_VertexAttribs1fvNV(GLuint index, GLsizei n, const GLfloat *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int v_size = safe_mul(n, 1 * sizeof(GLfloat));
   int cmd_size = sizeof(struct marshal_cmd_VertexAttribs1fvNV) + v_size;
   struct marshal_cmd_VertexAttribs1fvNV *cmd;
   if (unlikely(v_size < 0 || (v_size > 0 && !v) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "VertexAttribs1fvNV");
      CALL_VertexAttribs1fvNV(ctx->Dispatch.Current, (index, n, v));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttribs1fvNV, cmd_size);
   cmd->index = index;
   cmd->n = n;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, v, v_size);
}


/* VertexAttribs2fvNV: marshalled asynchronously */
struct marshal_cmd_VertexAttribs2fvNV
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLsizei n;
   /* Next safe_mul(n, 2 * sizeof(GLfloat)) bytes are GLfloat v[n][2] */
};
uint32_t
_mesa_unmarshal_VertexAttribs2fvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttribs2fvNV *restrict cmd)
{
   GLuint index = cmd->index;
   GLsizei n = cmd->n;
   GLfloat *v;
   const char *variable_data = (const char *) (cmd + 1);
   v = (GLfloat *) variable_data;
   CALL_VertexAttribs2fvNV(ctx->Dispatch.Current, (index, n, v));
   return cmd->cmd_base.cmd_size;
}
void GLAPIENTRY
_mesa_marshal_VertexAttribs2fvNV(GLuint index, GLsizei n, const GLfloat *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int v_size = safe_mul(n, 2 * sizeof(GLfloat));
   int cmd_size = sizeof(struct marshal_cmd_VertexAttribs2fvNV) + v_size;
   struct marshal_cmd_VertexAttribs2fvNV *cmd;
   if (unlikely(v_size < 0 || (v_size > 0 && !v) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "VertexAttribs2fvNV");
      CALL_VertexAttribs2fvNV(ctx->Dispatch.Current, (index, n, v));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttribs2fvNV, cmd_size);
   cmd->index = index;
   cmd->n = n;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, v, v_size);
}


/* VertexAttribs3fvNV: marshalled asynchronously */
struct marshal_cmd_VertexAttribs3fvNV
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLsizei n;
   /* Next safe_mul(n, 3 * sizeof(GLfloat)) bytes are GLfloat v[n][3] */
};
uint32_t
_mesa_unmarshal_VertexAttribs3fvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttribs3fvNV *restrict cmd)
{
   GLuint index = cmd->index;
   GLsizei n = cmd->n;
   GLfloat *v;
   const char *variable_data = (const char *) (cmd + 1);
   v = (GLfloat *) variable_data;
   CALL_VertexAttribs3fvNV(ctx->Dispatch.Current, (index, n, v));
   return cmd->cmd_base.cmd_size;
}
void GLAPIENTRY
_mesa_marshal_VertexAttribs3fvNV(GLuint index, GLsizei n, const GLfloat *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int v_size = safe_mul(n, 3 * sizeof(GLfloat));
   int cmd_size = sizeof(struct marshal_cmd_VertexAttribs3fvNV) + v_size;
   struct marshal_cmd_VertexAttribs3fvNV *cmd;
   if (unlikely(v_size < 0 || (v_size > 0 && !v) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "VertexAttribs3fvNV");
      CALL_VertexAttribs3fvNV(ctx->Dispatch.Current, (index, n, v));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttribs3fvNV, cmd_size);
   cmd->index = index;
   cmd->n = n;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, v, v_size);
}


/* VertexAttribs4fvNV: marshalled asynchronously */
struct marshal_cmd_VertexAttribs4fvNV
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLsizei n;
   /* Next safe_mul(n, 4 * sizeof(GLfloat)) bytes are GLfloat v[n][4] */
};
uint32_t
_mesa_unmarshal_VertexAttribs4fvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttribs4fvNV *restrict cmd)
{
   GLuint index = cmd->index;
   GLsizei n = cmd->n;
   GLfloat *v;
   const char *variable_data = (const char *) (cmd + 1);
   v = (GLfloat *) variable_data;
   CALL_VertexAttribs4fvNV(ctx->Dispatch.Current, (index, n, v));
   return cmd->cmd_base.cmd_size;
}
void GLAPIENTRY
_mesa_marshal_VertexAttribs4fvNV(GLuint index, GLsizei n, const GLfloat *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int v_size = safe_mul(n, 4 * sizeof(GLfloat));
   int cmd_size = sizeof(struct marshal_cmd_VertexAttribs4fvNV) + v_size;
   struct marshal_cmd_VertexAttribs4fvNV *cmd;
   if (unlikely(v_size < 0 || (v_size > 0 && !v) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "VertexAttribs4fvNV");
      CALL_VertexAttribs4fvNV(ctx->Dispatch.Current, (index, n, v));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttribs4fvNV, cmd_size);
   cmd->index = index;
   cmd->n = n;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, v, v_size);
}


/* VertexAttribs1dvNV: marshalled asynchronously */
struct marshal_cmd_VertexAttribs1dvNV
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLsizei n;
   /* Next safe_mul(n, 1 * sizeof(GLdouble)) bytes are GLdouble v[n] */
};
uint32_t
_mesa_unmarshal_VertexAttribs1dvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttribs1dvNV *restrict cmd)
{
   GLuint index = cmd->index;
   GLsizei n = cmd->n;
   GLdouble *v;
   const char *variable_data = (const char *) (cmd + 1);
   v = (GLdouble *) variable_data;
   CALL_VertexAttribs1dvNV(ctx->Dispatch.Current, (index, n, v));
   return cmd->cmd_base.cmd_size;
}
void GLAPIENTRY
_mesa_marshal_VertexAttribs1dvNV(GLuint index, GLsizei n, const GLdouble *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int v_size = safe_mul(n, 1 * sizeof(GLdouble));
   int cmd_size = sizeof(struct marshal_cmd_VertexAttribs1dvNV) + v_size;
   struct marshal_cmd_VertexAttribs1dvNV *cmd;
   if (unlikely(v_size < 0 || (v_size > 0 && !v) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "VertexAttribs1dvNV");
      CALL_VertexAttribs1dvNV(ctx->Dispatch.Current, (index, n, v));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttribs1dvNV, cmd_size);
   cmd->index = index;
   cmd->n = n;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, v, v_size);
}


/* VertexAttribs2dvNV: marshalled asynchronously */
struct marshal_cmd_VertexAttribs2dvNV
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLsizei n;
   /* Next safe_mul(n, 2 * sizeof(GLdouble)) bytes are GLdouble v[n][2] */
};
uint32_t
_mesa_unmarshal_VertexAttribs2dvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttribs2dvNV *restrict cmd)
{
   GLuint index = cmd->index;
   GLsizei n = cmd->n;
   GLdouble *v;
   const char *variable_data = (const char *) (cmd + 1);
   v = (GLdouble *) variable_data;
   CALL_VertexAttribs2dvNV(ctx->Dispatch.Current, (index, n, v));
   return cmd->cmd_base.cmd_size;
}
void GLAPIENTRY
_mesa_marshal_VertexAttribs2dvNV(GLuint index, GLsizei n, const GLdouble *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int v_size = safe_mul(n, 2 * sizeof(GLdouble));
   int cmd_size = sizeof(struct marshal_cmd_VertexAttribs2dvNV) + v_size;
   struct marshal_cmd_VertexAttribs2dvNV *cmd;
   if (unlikely(v_size < 0 || (v_size > 0 && !v) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "VertexAttribs2dvNV");
      CALL_VertexAttribs2dvNV(ctx->Dispatch.Current, (index, n, v));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttribs2dvNV, cmd_size);
   cmd->index = index;
   cmd->n = n;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, v, v_size);
}


/* VertexAttribs3dvNV: marshalled asynchronously */
struct marshal_cmd_VertexAttribs3dvNV
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLsizei n;
   /* Next safe_mul(n, 3 * sizeof(GLdouble)) bytes are GLdouble v[n][3] */
};
uint32_t
_mesa_unmarshal_VertexAttribs3dvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttribs3dvNV *restrict cmd)
{
   GLuint index = cmd->index;
   GLsizei n = cmd->n;
   GLdouble *v;
   const char *variable_data = (const char *) (cmd + 1);
   v = (GLdouble *) variable_data;
   CALL_VertexAttribs3dvNV(ctx->Dispatch.Current, (index, n, v));
   return cmd->cmd_base.cmd_size;
}
void GLAPIENTRY
_mesa_marshal_VertexAttribs3dvNV(GLuint index, GLsizei n, const GLdouble *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int v_size = safe_mul(n, 3 * sizeof(GLdouble));
   int cmd_size = sizeof(struct marshal_cmd_VertexAttribs3dvNV) + v_size;
   struct marshal_cmd_VertexAttribs3dvNV *cmd;
   if (unlikely(v_size < 0 || (v_size > 0 && !v) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "VertexAttribs3dvNV");
      CALL_VertexAttribs3dvNV(ctx->Dispatch.Current, (index, n, v));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttribs3dvNV, cmd_size);
   cmd->index = index;
   cmd->n = n;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, v, v_size);
}


/* VertexAttribs4dvNV: marshalled asynchronously */
struct marshal_cmd_VertexAttribs4dvNV
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLsizei n;
   /* Next safe_mul(n, 4 * sizeof(GLdouble)) bytes are GLdouble v[n][4] */
};
uint32_t
_mesa_unmarshal_VertexAttribs4dvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttribs4dvNV *restrict cmd)
{
   GLuint index = cmd->index;
   GLsizei n = cmd->n;
   GLdouble *v;
   const char *variable_data = (const char *) (cmd + 1);
   v = (GLdouble *) variable_data;
   CALL_VertexAttribs4dvNV(ctx->Dispatch.Current, (index, n, v));
   return cmd->cmd_base.cmd_size;
}
void GLAPIENTRY
_mesa_marshal_VertexAttribs4dvNV(GLuint index, GLsizei n, const GLdouble *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int v_size = safe_mul(n, 4 * sizeof(GLdouble));
   int cmd_size = sizeof(struct marshal_cmd_VertexAttribs4dvNV) + v_size;
   struct marshal_cmd_VertexAttribs4dvNV *cmd;
   if (unlikely(v_size < 0 || (v_size > 0 && !v) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "VertexAttribs4dvNV");
      CALL_VertexAttribs4dvNV(ctx->Dispatch.Current, (index, n, v));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttribs4dvNV, cmd_size);
   cmd->index = index;
   cmd->n = n;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, v, v_size);
}


/* VertexAttribs4ubvNV: marshalled asynchronously */
struct marshal_cmd_VertexAttribs4ubvNV
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLsizei n;
   /* Next safe_mul(n, 4 * sizeof(GLubyte)) bytes are GLubyte v[n][4] */
};
uint32_t
_mesa_unmarshal_VertexAttribs4ubvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttribs4ubvNV *restrict cmd)
{
   GLuint index = cmd->index;
   GLsizei n = cmd->n;
   GLubyte *v;
   const char *variable_data = (const char *) (cmd + 1);
   v = (GLubyte *) variable_data;
   CALL_VertexAttribs4ubvNV(ctx->Dispatch.Current, (index, n, v));
   return cmd->cmd_base.cmd_size;
}
void GLAPIENTRY
_mesa_marshal_VertexAttribs4ubvNV(GLuint index, GLsizei n, const GLubyte *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int v_size = safe_mul(n, 4 * sizeof(GLubyte));
   int cmd_size = sizeof(struct marshal_cmd_VertexAttribs4ubvNV) + v_size;
   struct marshal_cmd_VertexAttribs4ubvNV *cmd;
   if (unlikely(v_size < 0 || (v_size > 0 && !v) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "VertexAttribs4ubvNV");
      CALL_VertexAttribs4ubvNV(ctx->Dispatch.Current, (index, n, v));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttribs4ubvNV, cmd_size);
   cmd->index = index;
   cmd->n = n;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, v, v_size);
}


/* GenFragmentShadersATI: marshalled synchronously */
static GLuint GLAPIENTRY
_mesa_marshal_GenFragmentShadersATI(GLuint range)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GenFragmentShadersATI");
   return CALL_GenFragmentShadersATI(ctx->Dispatch.Current, (range));
}


/* BindFragmentShaderATI: marshalled asynchronously */
struct marshal_cmd_BindFragmentShaderATI
{
   struct marshal_cmd_base cmd_base;
   GLuint id;
};
uint32_t
_mesa_unmarshal_BindFragmentShaderATI(struct gl_context *ctx, const struct marshal_cmd_BindFragmentShaderATI *restrict cmd)
{
   GLuint id = cmd->id;
   CALL_BindFragmentShaderATI(ctx->Dispatch.Current, (id));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_BindFragmentShaderATI), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_BindFragmentShaderATI(GLuint id)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_BindFragmentShaderATI);
   struct marshal_cmd_BindFragmentShaderATI *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_BindFragmentShaderATI, cmd_size);
   cmd->id = id;
}


/* DeleteFragmentShaderATI: marshalled asynchronously */
struct marshal_cmd_DeleteFragmentShaderATI
{
   struct marshal_cmd_base cmd_base;
   GLuint id;
};
uint32_t
_mesa_unmarshal_DeleteFragmentShaderATI(struct gl_context *ctx, const struct marshal_cmd_DeleteFragmentShaderATI *restrict cmd)
{
   GLuint id = cmd->id;
   CALL_DeleteFragmentShaderATI(ctx->Dispatch.Current, (id));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_DeleteFragmentShaderATI), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_DeleteFragmentShaderATI(GLuint id)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_DeleteFragmentShaderATI);
   struct marshal_cmd_DeleteFragmentShaderATI *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_DeleteFragmentShaderATI, cmd_size);
   cmd->id = id;
}


/* BeginFragmentShaderATI: marshalled asynchronously */
struct marshal_cmd_BeginFragmentShaderATI
{
   struct marshal_cmd_base cmd_base;
};
uint32_t
_mesa_unmarshal_BeginFragmentShaderATI(struct gl_context *ctx, const struct marshal_cmd_BeginFragmentShaderATI *restrict cmd)
{
   CALL_BeginFragmentShaderATI(ctx->Dispatch.Current, ());
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_BeginFragmentShaderATI), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_BeginFragmentShaderATI(void)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_BeginFragmentShaderATI);
   struct marshal_cmd_BeginFragmentShaderATI *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_BeginFragmentShaderATI, cmd_size);
   (void) cmd;
}


/* EndFragmentShaderATI: marshalled asynchronously */
struct marshal_cmd_EndFragmentShaderATI
{
   struct marshal_cmd_base cmd_base;
};
uint32_t
_mesa_unmarshal_EndFragmentShaderATI(struct gl_context *ctx, const struct marshal_cmd_EndFragmentShaderATI *restrict cmd)
{
   CALL_EndFragmentShaderATI(ctx->Dispatch.Current, ());
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_EndFragmentShaderATI), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_EndFragmentShaderATI(void)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_EndFragmentShaderATI);
   struct marshal_cmd_EndFragmentShaderATI *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_EndFragmentShaderATI, cmd_size);
   (void) cmd;
}


/* PassTexCoordATI: marshalled asynchronously */
struct marshal_cmd_PassTexCoordATI
{
   struct marshal_cmd_base cmd_base;
   GLenum16 swizzle;
   GLuint dst;
   GLuint coord;
};
uint32_t
_mesa_unmarshal_PassTexCoordATI(struct gl_context *ctx, const struct marshal_cmd_PassTexCoordATI *restrict cmd)
{
   GLuint dst = cmd->dst;
   GLuint coord = cmd->coord;
   GLenum swizzle = cmd->swizzle;
   CALL_PassTexCoordATI(ctx->Dispatch.Current, (dst, coord, swizzle));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_PassTexCoordATI), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_PassTexCoordATI(GLuint dst, GLuint coord, GLenum swizzle)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_PassTexCoordATI);
   struct marshal_cmd_PassTexCoordATI *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_PassTexCoordATI, cmd_size);
   cmd->dst = dst;
   cmd->coord = coord;
   cmd->swizzle = MIN2(swizzle, 0xffff); /* clamped to 0xffff (invalid enum) */
}


/* SampleMapATI: marshalled asynchronously */
struct marshal_cmd_SampleMapATI
{
   struct marshal_cmd_base cmd_base;
   GLenum16 swizzle;
   GLuint dst;
   GLuint interp;
};
uint32_t
_mesa_unmarshal_SampleMapATI(struct gl_context *ctx, const struct marshal_cmd_SampleMapATI *restrict cmd)
{
   GLuint dst = cmd->dst;
   GLuint interp = cmd->interp;
   GLenum swizzle = cmd->swizzle;
   CALL_SampleMapATI(ctx->Dispatch.Current, (dst, interp, swizzle));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_SampleMapATI), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_SampleMapATI(GLuint dst, GLuint interp, GLenum swizzle)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_SampleMapATI);
   struct marshal_cmd_SampleMapATI *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_SampleMapATI, cmd_size);
   cmd->dst = dst;
   cmd->interp = interp;
   cmd->swizzle = MIN2(swizzle, 0xffff); /* clamped to 0xffff (invalid enum) */
}


/* ColorFragmentOp1ATI: marshalled asynchronously */
struct marshal_cmd_ColorFragmentOp1ATI
{
   struct marshal_cmd_base cmd_base;
   GLenum16 op;
   GLuint dst;
   GLuint dstMask;
   GLuint dstMod;
   GLuint arg1;
   GLuint arg1Rep;
   GLuint arg1Mod;
};
uint32_t
_mesa_unmarshal_ColorFragmentOp1ATI(struct gl_context *ctx, const struct marshal_cmd_ColorFragmentOp1ATI *restrict cmd)
{
   GLenum op = cmd->op;
   GLuint dst = cmd->dst;
   GLuint dstMask = cmd->dstMask;
   GLuint dstMod = cmd->dstMod;
   GLuint arg1 = cmd->arg1;
   GLuint arg1Rep = cmd->arg1Rep;
   GLuint arg1Mod = cmd->arg1Mod;
   CALL_ColorFragmentOp1ATI(ctx->Dispatch.Current, (op, dst, dstMask, dstMod, arg1, arg1Rep, arg1Mod));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_ColorFragmentOp1ATI), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ColorFragmentOp1ATI(GLenum op, GLuint dst, GLuint dstMask, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ColorFragmentOp1ATI);
   struct marshal_cmd_ColorFragmentOp1ATI *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ColorFragmentOp1ATI, cmd_size);
   cmd->op = MIN2(op, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->dst = dst;
   cmd->dstMask = dstMask;
   cmd->dstMod = dstMod;
   cmd->arg1 = arg1;
   cmd->arg1Rep = arg1Rep;
   cmd->arg1Mod = arg1Mod;
}


/* ColorFragmentOp2ATI: marshalled asynchronously */
struct marshal_cmd_ColorFragmentOp2ATI
{
   struct marshal_cmd_base cmd_base;
   GLenum16 op;
   GLuint dst;
   GLuint dstMask;
   GLuint dstMod;
   GLuint arg1;
   GLuint arg1Rep;
   GLuint arg1Mod;
   GLuint arg2;
   GLuint arg2Rep;
   GLuint arg2Mod;
};
uint32_t
_mesa_unmarshal_ColorFragmentOp2ATI(struct gl_context *ctx, const struct marshal_cmd_ColorFragmentOp2ATI *restrict cmd)
{
   GLenum op = cmd->op;
   GLuint dst = cmd->dst;
   GLuint dstMask = cmd->dstMask;
   GLuint dstMod = cmd->dstMod;
   GLuint arg1 = cmd->arg1;
   GLuint arg1Rep = cmd->arg1Rep;
   GLuint arg1Mod = cmd->arg1Mod;
   GLuint arg2 = cmd->arg2;
   GLuint arg2Rep = cmd->arg2Rep;
   GLuint arg2Mod = cmd->arg2Mod;
   CALL_ColorFragmentOp2ATI(ctx->Dispatch.Current, (op, dst, dstMask, dstMod, arg1, arg1Rep, arg1Mod, arg2, arg2Rep, arg2Mod));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_ColorFragmentOp2ATI), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ColorFragmentOp2ATI(GLenum op, GLuint dst, GLuint dstMask, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod, GLuint arg2, GLuint arg2Rep, GLuint arg2Mod)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ColorFragmentOp2ATI);
   struct marshal_cmd_ColorFragmentOp2ATI *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ColorFragmentOp2ATI, cmd_size);
   cmd->op = MIN2(op, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->dst = dst;
   cmd->dstMask = dstMask;
   cmd->dstMod = dstMod;
   cmd->arg1 = arg1;
   cmd->arg1Rep = arg1Rep;
   cmd->arg1Mod = arg1Mod;
   cmd->arg2 = arg2;
   cmd->arg2Rep = arg2Rep;
   cmd->arg2Mod = arg2Mod;
}


/* ColorFragmentOp3ATI: marshalled asynchronously */
struct marshal_cmd_ColorFragmentOp3ATI
{
   struct marshal_cmd_base cmd_base;
   GLenum16 op;
   GLuint dst;
   GLuint dstMask;
   GLuint dstMod;
   GLuint arg1;
   GLuint arg1Rep;
   GLuint arg1Mod;
   GLuint arg2;
   GLuint arg2Rep;
   GLuint arg2Mod;
   GLuint arg3;
   GLuint arg3Rep;
   GLuint arg3Mod;
};
uint32_t
_mesa_unmarshal_ColorFragmentOp3ATI(struct gl_context *ctx, const struct marshal_cmd_ColorFragmentOp3ATI *restrict cmd)
{
   GLenum op = cmd->op;
   GLuint dst = cmd->dst;
   GLuint dstMask = cmd->dstMask;
   GLuint dstMod = cmd->dstMod;
   GLuint arg1 = cmd->arg1;
   GLuint arg1Rep = cmd->arg1Rep;
   GLuint arg1Mod = cmd->arg1Mod;
   GLuint arg2 = cmd->arg2;
   GLuint arg2Rep = cmd->arg2Rep;
   GLuint arg2Mod = cmd->arg2Mod;
   GLuint arg3 = cmd->arg3;
   GLuint arg3Rep = cmd->arg3Rep;
   GLuint arg3Mod = cmd->arg3Mod;
   CALL_ColorFragmentOp3ATI(ctx->Dispatch.Current, (op, dst, dstMask, dstMod, arg1, arg1Rep, arg1Mod, arg2, arg2Rep, arg2Mod, arg3, arg3Rep, arg3Mod));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_ColorFragmentOp3ATI), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ColorFragmentOp3ATI(GLenum op, GLuint dst, GLuint dstMask, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod, GLuint arg2, GLuint arg2Rep, GLuint arg2Mod, GLuint arg3, GLuint arg3Rep, GLuint arg3Mod)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ColorFragmentOp3ATI);
   struct marshal_cmd_ColorFragmentOp3ATI *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ColorFragmentOp3ATI, cmd_size);
   cmd->op = MIN2(op, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->dst = dst;
   cmd->dstMask = dstMask;
   cmd->dstMod = dstMod;
   cmd->arg1 = arg1;
   cmd->arg1Rep = arg1Rep;
   cmd->arg1Mod = arg1Mod;
   cmd->arg2 = arg2;
   cmd->arg2Rep = arg2Rep;
   cmd->arg2Mod = arg2Mod;
   cmd->arg3 = arg3;
   cmd->arg3Rep = arg3Rep;
   cmd->arg3Mod = arg3Mod;
}


/* AlphaFragmentOp1ATI: marshalled asynchronously */
struct marshal_cmd_AlphaFragmentOp1ATI
{
   struct marshal_cmd_base cmd_base;
   GLenum16 op;
   GLuint dst;
   GLuint dstMod;
   GLuint arg1;
   GLuint arg1Rep;
   GLuint arg1Mod;
};
uint32_t
_mesa_unmarshal_AlphaFragmentOp1ATI(struct gl_context *ctx, const struct marshal_cmd_AlphaFragmentOp1ATI *restrict cmd)
{
   GLenum op = cmd->op;
   GLuint dst = cmd->dst;
   GLuint dstMod = cmd->dstMod;
   GLuint arg1 = cmd->arg1;
   GLuint arg1Rep = cmd->arg1Rep;
   GLuint arg1Mod = cmd->arg1Mod;
   CALL_AlphaFragmentOp1ATI(ctx->Dispatch.Current, (op, dst, dstMod, arg1, arg1Rep, arg1Mod));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_AlphaFragmentOp1ATI), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_AlphaFragmentOp1ATI(GLenum op, GLuint dst, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_AlphaFragmentOp1ATI);
   struct marshal_cmd_AlphaFragmentOp1ATI *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_AlphaFragmentOp1ATI, cmd_size);
   cmd->op = MIN2(op, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->dst = dst;
   cmd->dstMod = dstMod;
   cmd->arg1 = arg1;
   cmd->arg1Rep = arg1Rep;
   cmd->arg1Mod = arg1Mod;
}


/* AlphaFragmentOp2ATI: marshalled asynchronously */
struct marshal_cmd_AlphaFragmentOp2ATI
{
   struct marshal_cmd_base cmd_base;
   GLenum16 op;
   GLuint dst;
   GLuint dstMod;
   GLuint arg1;
   GLuint arg1Rep;
   GLuint arg1Mod;
   GLuint arg2;
   GLuint arg2Rep;
   GLuint arg2Mod;
};
uint32_t
_mesa_unmarshal_AlphaFragmentOp2ATI(struct gl_context *ctx, const struct marshal_cmd_AlphaFragmentOp2ATI *restrict cmd)
{
   GLenum op = cmd->op;
   GLuint dst = cmd->dst;
   GLuint dstMod = cmd->dstMod;
   GLuint arg1 = cmd->arg1;
   GLuint arg1Rep = cmd->arg1Rep;
   GLuint arg1Mod = cmd->arg1Mod;
   GLuint arg2 = cmd->arg2;
   GLuint arg2Rep = cmd->arg2Rep;
   GLuint arg2Mod = cmd->arg2Mod;
   CALL_AlphaFragmentOp2ATI(ctx->Dispatch.Current, (op, dst, dstMod, arg1, arg1Rep, arg1Mod, arg2, arg2Rep, arg2Mod));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_AlphaFragmentOp2ATI), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_AlphaFragmentOp2ATI(GLenum op, GLuint dst, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod, GLuint arg2, GLuint arg2Rep, GLuint arg2Mod)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_AlphaFragmentOp2ATI);
   struct marshal_cmd_AlphaFragmentOp2ATI *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_AlphaFragmentOp2ATI, cmd_size);
   cmd->op = MIN2(op, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->dst = dst;
   cmd->dstMod = dstMod;
   cmd->arg1 = arg1;
   cmd->arg1Rep = arg1Rep;
   cmd->arg1Mod = arg1Mod;
   cmd->arg2 = arg2;
   cmd->arg2Rep = arg2Rep;
   cmd->arg2Mod = arg2Mod;
}


/* AlphaFragmentOp3ATI: marshalled asynchronously */
struct marshal_cmd_AlphaFragmentOp3ATI
{
   struct marshal_cmd_base cmd_base;
   GLenum16 op;
   GLuint dst;
   GLuint dstMod;
   GLuint arg1;
   GLuint arg1Rep;
   GLuint arg1Mod;
   GLuint arg2;
   GLuint arg2Rep;
   GLuint arg2Mod;
   GLuint arg3;
   GLuint arg3Rep;
   GLuint arg3Mod;
};
uint32_t
_mesa_unmarshal_AlphaFragmentOp3ATI(struct gl_context *ctx, const struct marshal_cmd_AlphaFragmentOp3ATI *restrict cmd)
{
   GLenum op = cmd->op;
   GLuint dst = cmd->dst;
   GLuint dstMod = cmd->dstMod;
   GLuint arg1 = cmd->arg1;
   GLuint arg1Rep = cmd->arg1Rep;
   GLuint arg1Mod = cmd->arg1Mod;
   GLuint arg2 = cmd->arg2;
   GLuint arg2Rep = cmd->arg2Rep;
   GLuint arg2Mod = cmd->arg2Mod;
   GLuint arg3 = cmd->arg3;
   GLuint arg3Rep = cmd->arg3Rep;
   GLuint arg3Mod = cmd->arg3Mod;
   CALL_AlphaFragmentOp3ATI(ctx->Dispatch.Current, (op, dst, dstMod, arg1, arg1Rep, arg1Mod, arg2, arg2Rep, arg2Mod, arg3, arg3Rep, arg3Mod));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_AlphaFragmentOp3ATI), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_AlphaFragmentOp3ATI(GLenum op, GLuint dst, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod, GLuint arg2, GLuint arg2Rep, GLuint arg2Mod, GLuint arg3, GLuint arg3Rep, GLuint arg3Mod)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_AlphaFragmentOp3ATI);
   struct marshal_cmd_AlphaFragmentOp3ATI *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_AlphaFragmentOp3ATI, cmd_size);
   cmd->op = MIN2(op, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->dst = dst;
   cmd->dstMod = dstMod;
   cmd->arg1 = arg1;
   cmd->arg1Rep = arg1Rep;
   cmd->arg1Mod = arg1Mod;
   cmd->arg2 = arg2;
   cmd->arg2Rep = arg2Rep;
   cmd->arg2Mod = arg2Mod;
   cmd->arg3 = arg3;
   cmd->arg3Rep = arg3Rep;
   cmd->arg3Mod = arg3Mod;
}


/* SetFragmentShaderConstantATI: marshalled asynchronously */
struct marshal_cmd_SetFragmentShaderConstantATI
{
   struct marshal_cmd_base cmd_base;
   GLuint dst;
   GLfloat value[4];
};
uint32_t
_mesa_unmarshal_SetFragmentShaderConstantATI(struct gl_context *ctx, const struct marshal_cmd_SetFragmentShaderConstantATI *restrict cmd)
{
   GLuint dst = cmd->dst;
   const GLfloat *value = cmd->value;
   CALL_SetFragmentShaderConstantATI(ctx->Dispatch.Current, (dst, value));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_SetFragmentShaderConstantATI), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_SetFragmentShaderConstantATI(GLuint dst, const GLfloat *value)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_SetFragmentShaderConstantATI);
   struct marshal_cmd_SetFragmentShaderConstantATI *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_SetFragmentShaderConstantATI, cmd_size);
   cmd->dst = dst;
   memcpy(cmd->value, value, 4 * sizeof(GLfloat));
}


/* ActiveStencilFaceEXT: marshalled asynchronously */
struct marshal_cmd_ActiveStencilFaceEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 face;
};
uint32_t
_mesa_unmarshal_ActiveStencilFaceEXT(struct gl_context *ctx, const struct marshal_cmd_ActiveStencilFaceEXT *restrict cmd)
{
   GLenum face = cmd->face;
   CALL_ActiveStencilFaceEXT(ctx->Dispatch.Current, (face));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_ActiveStencilFaceEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ActiveStencilFaceEXT(GLenum face)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ActiveStencilFaceEXT);
   struct marshal_cmd_ActiveStencilFaceEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ActiveStencilFaceEXT, cmd_size);
   cmd->face = MIN2(face, 0xffff); /* clamped to 0xffff (invalid enum) */
}


/* DepthBoundsEXT: marshalled asynchronously */
struct marshal_cmd_DepthBoundsEXT
{
   struct marshal_cmd_base cmd_base;
   GLclampd zmin;
   GLclampd zmax;
};
uint32_t
_mesa_unmarshal_DepthBoundsEXT(struct gl_context *ctx, const struct marshal_cmd_DepthBoundsEXT *restrict cmd)
{
   GLclampd zmin = cmd->zmin;
   GLclampd zmax = cmd->zmax;
   CALL_DepthBoundsEXT(ctx->Dispatch.Current, (zmin, zmax));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_DepthBoundsEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_DepthBoundsEXT(GLclampd zmin, GLclampd zmax)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_DepthBoundsEXT);
   struct marshal_cmd_DepthBoundsEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_DepthBoundsEXT, cmd_size);
   cmd->zmin = zmin;
   cmd->zmax = zmax;
}


/* BindRenderbufferEXT: marshalled asynchronously */
struct marshal_cmd_BindRenderbufferEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLuint renderbuffer;
};
uint32_t
_mesa_unmarshal_BindRenderbufferEXT(struct gl_context *ctx, const struct marshal_cmd_BindRenderbufferEXT *restrict cmd)
{
   GLenum target = cmd->target;
   GLuint renderbuffer = cmd->renderbuffer;
   CALL_BindRenderbufferEXT(ctx->Dispatch.Current, (target, renderbuffer));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_BindRenderbufferEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_BindRenderbufferEXT(GLenum target, GLuint renderbuffer)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_BindRenderbufferEXT);
   struct marshal_cmd_BindRenderbufferEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_BindRenderbufferEXT, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->renderbuffer = renderbuffer;
}


/* BindFramebufferEXT: marshalled asynchronously */
struct marshal_cmd_BindFramebufferEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLuint framebuffer;
};
uint32_t
_mesa_unmarshal_BindFramebufferEXT(struct gl_context *ctx, const struct marshal_cmd_BindFramebufferEXT *restrict cmd)
{
   GLenum target = cmd->target;
   GLuint framebuffer = cmd->framebuffer;
   CALL_BindFramebufferEXT(ctx->Dispatch.Current, (target, framebuffer));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_BindFramebufferEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_BindFramebufferEXT(GLenum target, GLuint framebuffer)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_BindFramebufferEXT);
   struct marshal_cmd_BindFramebufferEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_BindFramebufferEXT, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->framebuffer = framebuffer;
   _mesa_glthread_BindFramebuffer(ctx, target, framebuffer);
}


/* StringMarkerGREMEDY: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_StringMarkerGREMEDY(GLsizei len, const GLvoid *string)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "StringMarkerGREMEDY");
   CALL_StringMarkerGREMEDY(ctx->Dispatch.Current, (len, string));
}


/* ProvokingVertex: marshalled asynchronously */
struct marshal_cmd_ProvokingVertex
{
   struct marshal_cmd_base cmd_base;
   GLenum16 mode;
};
uint32_t
_mesa_unmarshal_ProvokingVertex(struct gl_context *ctx, const struct marshal_cmd_ProvokingVertex *restrict cmd)
{
   GLenum mode = cmd->mode;
   CALL_ProvokingVertex(ctx->Dispatch.Current, (mode));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_ProvokingVertex), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ProvokingVertex(GLenum mode)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ProvokingVertex);
   struct marshal_cmd_ProvokingVertex *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ProvokingVertex, cmd_size);
   cmd->mode = MIN2(mode, 0xffff); /* clamped to 0xffff (invalid enum) */
}


/* ColorMaski: marshalled asynchronously */
struct marshal_cmd_ColorMaski
{
   struct marshal_cmd_base cmd_base;
   GLboolean r;
   GLboolean g;
   GLboolean b;
   GLboolean a;
   GLuint buf;
};
uint32_t
_mesa_unmarshal_ColorMaski(struct gl_context *ctx, const struct marshal_cmd_ColorMaski *restrict cmd)
{
   GLuint buf = cmd->buf;
   GLboolean r = cmd->r;
   GLboolean g = cmd->g;
   GLboolean b = cmd->b;
   GLboolean a = cmd->a;
   CALL_ColorMaski(ctx->Dispatch.Current, (buf, r, g, b, a));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_ColorMaski), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ColorMaski(GLuint buf, GLboolean r, GLboolean g, GLboolean b, GLboolean a)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ColorMaski);
   struct marshal_cmd_ColorMaski *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ColorMaski, cmd_size);
   cmd->buf = buf;
   cmd->r = r;
   cmd->g = g;
   cmd->b = b;
   cmd->a = a;
}


/* GetBooleani_v: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetBooleani_v(GLenum value, GLuint index, GLboolean *data)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetBooleani_v");
   CALL_GetBooleani_v(ctx->Dispatch.Current, (value, index, data));
}


/* GetIntegeri_v: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetIntegeri_v(GLenum value, GLuint index, GLint *data)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetIntegeri_v");
   CALL_GetIntegeri_v(ctx->Dispatch.Current, (value, index, data));
}


/* Enablei: marshalled asynchronously */
struct marshal_cmd_Enablei
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLuint index;
};
uint32_t
_mesa_unmarshal_Enablei(struct gl_context *ctx, const struct marshal_cmd_Enablei *restrict cmd)
{
   GLenum target = cmd->target;
   GLuint index = cmd->index;
   CALL_Enablei(ctx->Dispatch.Current, (target, index));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Enablei), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_Enablei(GLenum target, GLuint index)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Enablei);
   struct marshal_cmd_Enablei *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Enablei, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->index = index;
}


/* Disablei: marshalled asynchronously */
struct marshal_cmd_Disablei
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLuint index;
};
uint32_t
_mesa_unmarshal_Disablei(struct gl_context *ctx, const struct marshal_cmd_Disablei *restrict cmd)
{
   GLenum target = cmd->target;
   GLuint index = cmd->index;
   CALL_Disablei(ctx->Dispatch.Current, (target, index));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Disablei), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_Disablei(GLenum target, GLuint index)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Disablei);
   struct marshal_cmd_Disablei *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Disablei, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->index = index;
}


/* IsEnabledi: marshalled synchronously */
static GLboolean GLAPIENTRY
_mesa_marshal_IsEnabledi(GLenum target, GLuint index)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "IsEnabledi");
   return CALL_IsEnabledi(ctx->Dispatch.Current, (target, index));
}


/* GetPerfMonitorGroupsAMD: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetPerfMonitorGroupsAMD(GLint *numGroups, GLsizei groupsSize, GLuint *groups)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetPerfMonitorGroupsAMD");
   CALL_GetPerfMonitorGroupsAMD(ctx->Dispatch.Current, (numGroups, groupsSize, groups));
}


/* GetPerfMonitorCountersAMD: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetPerfMonitorCountersAMD(GLuint group, GLint *numCounters, GLint *maxActiveCounters, GLsizei countersSize, GLuint *counters)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetPerfMonitorCountersAMD");
   CALL_GetPerfMonitorCountersAMD(ctx->Dispatch.Current, (group, numCounters, maxActiveCounters, countersSize, counters));
}


/* GetPerfMonitorGroupStringAMD: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetPerfMonitorGroupStringAMD(GLuint group, GLsizei bufSize, GLsizei *length, GLchar *groupString)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetPerfMonitorGroupStringAMD");
   CALL_GetPerfMonitorGroupStringAMD(ctx->Dispatch.Current, (group, bufSize, length, groupString));
}


/* GetPerfMonitorCounterStringAMD: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetPerfMonitorCounterStringAMD(GLuint group, GLuint counter, GLsizei bufSize, GLsizei *length, GLchar *counterString)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetPerfMonitorCounterStringAMD");
   CALL_GetPerfMonitorCounterStringAMD(ctx->Dispatch.Current, (group, counter, bufSize, length, counterString));
}


/* GetPerfMonitorCounterInfoAMD: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetPerfMonitorCounterInfoAMD(GLuint group, GLuint counter, GLenum pname, GLvoid *data)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetPerfMonitorCounterInfoAMD");
   CALL_GetPerfMonitorCounterInfoAMD(ctx->Dispatch.Current, (group, counter, pname, data));
}


/* GenPerfMonitorsAMD: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GenPerfMonitorsAMD(GLsizei n, GLuint *monitors)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GenPerfMonitorsAMD");
   CALL_GenPerfMonitorsAMD(ctx->Dispatch.Current, (n, monitors));
}


/* DeletePerfMonitorsAMD: marshalled asynchronously */
struct marshal_cmd_DeletePerfMonitorsAMD
{
   struct marshal_cmd_base cmd_base;
   GLsizei n;
   /* Next safe_mul(n, 1 * sizeof(GLuint)) bytes are GLuint monitors[n] */
};
uint32_t
_mesa_unmarshal_DeletePerfMonitorsAMD(struct gl_context *ctx, const struct marshal_cmd_DeletePerfMonitorsAMD *restrict cmd)
{
   GLsizei n = cmd->n;
   GLuint *monitors;
   const char *variable_data = (const char *) (cmd + 1);
   monitors = (GLuint *) variable_data;
   CALL_DeletePerfMonitorsAMD(ctx->Dispatch.Current, (n, monitors));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_DeletePerfMonitorsAMD(GLsizei n, GLuint *monitors)
{
   GET_CURRENT_CONTEXT(ctx);
   int monitors_size = safe_mul(n, 1 * sizeof(GLuint));
   int cmd_size = sizeof(struct marshal_cmd_DeletePerfMonitorsAMD) + monitors_size;
   struct marshal_cmd_DeletePerfMonitorsAMD *cmd;
   if (unlikely(monitors_size < 0 || (monitors_size > 0 && !monitors) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "DeletePerfMonitorsAMD");
      CALL_DeletePerfMonitorsAMD(ctx->Dispatch.Current, (n, monitors));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_DeletePerfMonitorsAMD, cmd_size);
   cmd->n = n;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, monitors, monitors_size);
}


/* SelectPerfMonitorCountersAMD: marshalled asynchronously */
struct marshal_cmd_SelectPerfMonitorCountersAMD
{
   struct marshal_cmd_base cmd_base;
   GLboolean enable;
   GLuint monitor;
   GLuint group;
   GLint numCounters;
   /* Next safe_mul(numCounters, 1 * sizeof(GLuint)) bytes are GLuint counterList[numCounters] */
};
uint32_t
_mesa_unmarshal_SelectPerfMonitorCountersAMD(struct gl_context *ctx, const struct marshal_cmd_SelectPerfMonitorCountersAMD *restrict cmd)
{
   GLuint monitor = cmd->monitor;
   GLboolean enable = cmd->enable;
   GLuint group = cmd->group;
   GLint numCounters = cmd->numCounters;
   GLuint *counterList;
   const char *variable_data = (const char *) (cmd + 1);
   counterList = (GLuint *) variable_data;
   CALL_SelectPerfMonitorCountersAMD(ctx->Dispatch.Current, (monitor, enable, group, numCounters, counterList));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_SelectPerfMonitorCountersAMD(GLuint monitor, GLboolean enable, GLuint group, GLint numCounters, GLuint *counterList)
{
   GET_CURRENT_CONTEXT(ctx);
   int counterList_size = safe_mul(numCounters, 1 * sizeof(GLuint));
   int cmd_size = sizeof(struct marshal_cmd_SelectPerfMonitorCountersAMD) + counterList_size;
   struct marshal_cmd_SelectPerfMonitorCountersAMD *cmd;
   if (unlikely(counterList_size < 0 || (counterList_size > 0 && !counterList) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "SelectPerfMonitorCountersAMD");
      CALL_SelectPerfMonitorCountersAMD(ctx->Dispatch.Current, (monitor, enable, group, numCounters, counterList));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_SelectPerfMonitorCountersAMD, cmd_size);
   cmd->monitor = monitor;
   cmd->enable = enable;
   cmd->group = group;
   cmd->numCounters = numCounters;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, counterList, counterList_size);
}


/* BeginPerfMonitorAMD: marshalled asynchronously */
struct marshal_cmd_BeginPerfMonitorAMD
{
   struct marshal_cmd_base cmd_base;
   GLuint monitor;
};
uint32_t
_mesa_unmarshal_BeginPerfMonitorAMD(struct gl_context *ctx, const struct marshal_cmd_BeginPerfMonitorAMD *restrict cmd)
{
   GLuint monitor = cmd->monitor;
   CALL_BeginPerfMonitorAMD(ctx->Dispatch.Current, (monitor));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_BeginPerfMonitorAMD), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_BeginPerfMonitorAMD(GLuint monitor)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_BeginPerfMonitorAMD);
   struct marshal_cmd_BeginPerfMonitorAMD *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_BeginPerfMonitorAMD, cmd_size);
   cmd->monitor = monitor;
}


/* EndPerfMonitorAMD: marshalled asynchronously */
struct marshal_cmd_EndPerfMonitorAMD
{
   struct marshal_cmd_base cmd_base;
   GLuint monitor;
};
uint32_t
_mesa_unmarshal_EndPerfMonitorAMD(struct gl_context *ctx, const struct marshal_cmd_EndPerfMonitorAMD *restrict cmd)
{
   GLuint monitor = cmd->monitor;
   CALL_EndPerfMonitorAMD(ctx->Dispatch.Current, (monitor));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_EndPerfMonitorAMD), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_EndPerfMonitorAMD(GLuint monitor)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_EndPerfMonitorAMD);
   struct marshal_cmd_EndPerfMonitorAMD *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_EndPerfMonitorAMD, cmd_size);
   cmd->monitor = monitor;
}


/* GetPerfMonitorCounterDataAMD: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetPerfMonitorCounterDataAMD(GLuint monitor, GLenum pname, GLsizei dataSize, GLuint *data, GLint *bytesWritten)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetPerfMonitorCounterDataAMD");
   CALL_GetPerfMonitorCounterDataAMD(ctx->Dispatch.Current, (monitor, pname, dataSize, data, bytesWritten));
}


/* CopyImageSubDataNV: marshalled asynchronously */
struct marshal_cmd_CopyImageSubDataNV
{
   struct marshal_cmd_base cmd_base;
   GLenum16 srcTarget;
   GLenum16 dstTarget;
   GLuint srcName;
   GLint srcLevel;
   GLint srcX;
   GLint srcY;
   GLint srcZ;
   GLuint dstName;
   GLint dstLevel;
   GLint dstX;
   GLint dstY;
   GLint dstZ;
   GLsizei width;
   GLsizei height;
   GLsizei depth;
};
uint32_t
_mesa_unmarshal_CopyImageSubDataNV(struct gl_context *ctx, const struct marshal_cmd_CopyImageSubDataNV *restrict cmd)
{
   GLuint srcName = cmd->srcName;
   GLenum srcTarget = cmd->srcTarget;
   GLint srcLevel = cmd->srcLevel;
   GLint srcX = cmd->srcX;
   GLint srcY = cmd->srcY;
   GLint srcZ = cmd->srcZ;
   GLuint dstName = cmd->dstName;
   GLenum dstTarget = cmd->dstTarget;
   GLint dstLevel = cmd->dstLevel;
   GLint dstX = cmd->dstX;
   GLint dstY = cmd->dstY;
   GLint dstZ = cmd->dstZ;
   GLsizei width = cmd->width;
   GLsizei height = cmd->height;
   GLsizei depth = cmd->depth;
   CALL_CopyImageSubDataNV(ctx->Dispatch.Current, (srcName, srcTarget, srcLevel, srcX, srcY, srcZ, dstName, dstTarget, dstLevel, dstX, dstY, dstZ, width, height, depth));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_CopyImageSubDataNV), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_CopyImageSubDataNV(GLuint srcName, GLenum srcTarget, GLint srcLevel, GLint srcX, GLint srcY, GLint srcZ, GLuint dstName, GLenum dstTarget, GLint dstLevel, GLint dstX, GLint dstY, GLint dstZ, GLsizei width, GLsizei height, GLsizei depth)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_CopyImageSubDataNV);
   struct marshal_cmd_CopyImageSubDataNV *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_CopyImageSubDataNV, cmd_size);
   cmd->srcName = srcName;
   cmd->srcTarget = MIN2(srcTarget, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->srcLevel = srcLevel;
   cmd->srcX = srcX;
   cmd->srcY = srcY;
   cmd->srcZ = srcZ;
   cmd->dstName = dstName;
   cmd->dstTarget = MIN2(dstTarget, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->dstLevel = dstLevel;
   cmd->dstX = dstX;
   cmd->dstY = dstY;
   cmd->dstZ = dstZ;
   cmd->width = width;
   cmd->height = height;
   cmd->depth = depth;
}


/* MatrixLoadfEXT: marshalled asynchronously */
struct marshal_cmd_MatrixLoadfEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 matrixMode;
   GLfloat m[16];
};
uint32_t
_mesa_unmarshal_MatrixLoadfEXT(struct gl_context *ctx, const struct marshal_cmd_MatrixLoadfEXT *restrict cmd)
{
   GLenum matrixMode = cmd->matrixMode;
   const GLfloat *m = cmd->m;
   CALL_MatrixLoadfEXT(ctx->Dispatch.Current, (matrixMode, m));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MatrixLoadfEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_MatrixLoadfEXT(GLenum matrixMode, const GLfloat *m)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MatrixLoadfEXT);
   struct marshal_cmd_MatrixLoadfEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MatrixLoadfEXT, cmd_size);
   cmd->matrixMode = MIN2(matrixMode, 0xffff); /* clamped to 0xffff (invalid enum) */
   memcpy(cmd->m, m, 16 * sizeof(GLfloat));
}


/* MatrixLoaddEXT: marshalled asynchronously */
struct marshal_cmd_MatrixLoaddEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 matrixMode;
   GLdouble m[16];
};
uint32_t
_mesa_unmarshal_MatrixLoaddEXT(struct gl_context *ctx, const struct marshal_cmd_MatrixLoaddEXT *restrict cmd)
{
   GLenum matrixMode = cmd->matrixMode;
   const GLdouble *m = cmd->m;
   CALL_MatrixLoaddEXT(ctx->Dispatch.Current, (matrixMode, m));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MatrixLoaddEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_MatrixLoaddEXT(GLenum matrixMode, const GLdouble *m)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MatrixLoaddEXT);
   struct marshal_cmd_MatrixLoaddEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MatrixLoaddEXT, cmd_size);
   cmd->matrixMode = MIN2(matrixMode, 0xffff); /* clamped to 0xffff (invalid enum) */
   memcpy(cmd->m, m, 16 * sizeof(GLdouble));
}


/* MatrixMultfEXT: marshalled asynchronously */
struct marshal_cmd_MatrixMultfEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 matrixMode;
   GLfloat m[16];
};
uint32_t
_mesa_unmarshal_MatrixMultfEXT(struct gl_context *ctx, const struct marshal_cmd_MatrixMultfEXT *restrict cmd)
{
   GLenum matrixMode = cmd->matrixMode;
   const GLfloat *m = cmd->m;
   CALL_MatrixMultfEXT(ctx->Dispatch.Current, (matrixMode, m));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MatrixMultfEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_MatrixMultfEXT(GLenum matrixMode, const GLfloat *m)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MatrixMultfEXT);
   struct marshal_cmd_MatrixMultfEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MatrixMultfEXT, cmd_size);
   cmd->matrixMode = MIN2(matrixMode, 0xffff); /* clamped to 0xffff (invalid enum) */
   memcpy(cmd->m, m, 16 * sizeof(GLfloat));
}


/* MatrixMultdEXT: marshalled asynchronously */
struct marshal_cmd_MatrixMultdEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 matrixMode;
   GLdouble m[16];
};
uint32_t
_mesa_unmarshal_MatrixMultdEXT(struct gl_context *ctx, const struct marshal_cmd_MatrixMultdEXT *restrict cmd)
{
   GLenum matrixMode = cmd->matrixMode;
   const GLdouble *m = cmd->m;
   CALL_MatrixMultdEXT(ctx->Dispatch.Current, (matrixMode, m));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MatrixMultdEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_MatrixMultdEXT(GLenum matrixMode, const GLdouble *m)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MatrixMultdEXT);
   struct marshal_cmd_MatrixMultdEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MatrixMultdEXT, cmd_size);
   cmd->matrixMode = MIN2(matrixMode, 0xffff); /* clamped to 0xffff (invalid enum) */
   memcpy(cmd->m, m, 16 * sizeof(GLdouble));
}


/* MatrixLoadIdentityEXT: marshalled asynchronously */
struct marshal_cmd_MatrixLoadIdentityEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 matrixMode;
};
uint32_t
_mesa_unmarshal_MatrixLoadIdentityEXT(struct gl_context *ctx, const struct marshal_cmd_MatrixLoadIdentityEXT *restrict cmd)
{
   GLenum matrixMode = cmd->matrixMode;
   CALL_MatrixLoadIdentityEXT(ctx->Dispatch.Current, (matrixMode));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MatrixLoadIdentityEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_MatrixLoadIdentityEXT(GLenum matrixMode)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MatrixLoadIdentityEXT);
   struct marshal_cmd_MatrixLoadIdentityEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MatrixLoadIdentityEXT, cmd_size);
   cmd->matrixMode = MIN2(matrixMode, 0xffff); /* clamped to 0xffff (invalid enum) */
}


/* MatrixRotatefEXT: marshalled asynchronously */
struct marshal_cmd_MatrixRotatefEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 matrixMode;
   GLfloat angle;
   GLfloat x;
   GLfloat y;
   GLfloat z;
};
uint32_t
_mesa_unmarshal_MatrixRotatefEXT(struct gl_context *ctx, const struct marshal_cmd_MatrixRotatefEXT *restrict cmd)
{
   GLenum matrixMode = cmd->matrixMode;
   GLfloat angle = cmd->angle;
   GLfloat x = cmd->x;
   GLfloat y = cmd->y;
   GLfloat z = cmd->z;
   CALL_MatrixRotatefEXT(ctx->Dispatch.Current, (matrixMode, angle, x, y, z));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MatrixRotatefEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_MatrixRotatefEXT(GLenum matrixMode, GLfloat angle, GLfloat x, GLfloat y, GLfloat z)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MatrixRotatefEXT);
   struct marshal_cmd_MatrixRotatefEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MatrixRotatefEXT, cmd_size);
   cmd->matrixMode = MIN2(matrixMode, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->angle = angle;
   cmd->x = x;
   cmd->y = y;
   cmd->z = z;
}


/* MatrixRotatedEXT: marshalled asynchronously */
struct marshal_cmd_MatrixRotatedEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 matrixMode;
   GLdouble angle;
   GLdouble x;
   GLdouble y;
   GLdouble z;
};
uint32_t
_mesa_unmarshal_MatrixRotatedEXT(struct gl_context *ctx, const struct marshal_cmd_MatrixRotatedEXT *restrict cmd)
{
   GLenum matrixMode = cmd->matrixMode;
   GLdouble angle = cmd->angle;
   GLdouble x = cmd->x;
   GLdouble y = cmd->y;
   GLdouble z = cmd->z;
   CALL_MatrixRotatedEXT(ctx->Dispatch.Current, (matrixMode, angle, x, y, z));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MatrixRotatedEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_MatrixRotatedEXT(GLenum matrixMode, GLdouble angle, GLdouble x, GLdouble y, GLdouble z)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MatrixRotatedEXT);
   struct marshal_cmd_MatrixRotatedEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MatrixRotatedEXT, cmd_size);
   cmd->matrixMode = MIN2(matrixMode, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->angle = angle;
   cmd->x = x;
   cmd->y = y;
   cmd->z = z;
}


/* MatrixScalefEXT: marshalled asynchronously */
struct marshal_cmd_MatrixScalefEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 matrixMode;
   GLfloat x;
   GLfloat y;
   GLfloat z;
};
uint32_t
_mesa_unmarshal_MatrixScalefEXT(struct gl_context *ctx, const struct marshal_cmd_MatrixScalefEXT *restrict cmd)
{
   GLenum matrixMode = cmd->matrixMode;
   GLfloat x = cmd->x;
   GLfloat y = cmd->y;
   GLfloat z = cmd->z;
   CALL_MatrixScalefEXT(ctx->Dispatch.Current, (matrixMode, x, y, z));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MatrixScalefEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_MatrixScalefEXT(GLenum matrixMode, GLfloat x, GLfloat y, GLfloat z)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MatrixScalefEXT);
   struct marshal_cmd_MatrixScalefEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MatrixScalefEXT, cmd_size);
   cmd->matrixMode = MIN2(matrixMode, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->x = x;
   cmd->y = y;
   cmd->z = z;
}


/* MatrixScaledEXT: marshalled asynchronously */
struct marshal_cmd_MatrixScaledEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 matrixMode;
   GLdouble x;
   GLdouble y;
   GLdouble z;
};
uint32_t
_mesa_unmarshal_MatrixScaledEXT(struct gl_context *ctx, const struct marshal_cmd_MatrixScaledEXT *restrict cmd)
{
   GLenum matrixMode = cmd->matrixMode;
   GLdouble x = cmd->x;
   GLdouble y = cmd->y;
   GLdouble z = cmd->z;
   CALL_MatrixScaledEXT(ctx->Dispatch.Current, (matrixMode, x, y, z));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MatrixScaledEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_MatrixScaledEXT(GLenum matrixMode, GLdouble x, GLdouble y, GLdouble z)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MatrixScaledEXT);
   struct marshal_cmd_MatrixScaledEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MatrixScaledEXT, cmd_size);
   cmd->matrixMode = MIN2(matrixMode, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->x = x;
   cmd->y = y;
   cmd->z = z;
}


/* MatrixTranslatefEXT: marshalled asynchronously */
struct marshal_cmd_MatrixTranslatefEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 matrixMode;
   GLfloat x;
   GLfloat y;
   GLfloat z;
};
uint32_t
_mesa_unmarshal_MatrixTranslatefEXT(struct gl_context *ctx, const struct marshal_cmd_MatrixTranslatefEXT *restrict cmd)
{
   GLenum matrixMode = cmd->matrixMode;
   GLfloat x = cmd->x;
   GLfloat y = cmd->y;
   GLfloat z = cmd->z;
   CALL_MatrixTranslatefEXT(ctx->Dispatch.Current, (matrixMode, x, y, z));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MatrixTranslatefEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_MatrixTranslatefEXT(GLenum matrixMode, GLfloat x, GLfloat y, GLfloat z)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MatrixTranslatefEXT);
   struct marshal_cmd_MatrixTranslatefEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MatrixTranslatefEXT, cmd_size);
   cmd->matrixMode = MIN2(matrixMode, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->x = x;
   cmd->y = y;
   cmd->z = z;
}


/* MatrixTranslatedEXT: marshalled asynchronously */
struct marshal_cmd_MatrixTranslatedEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 matrixMode;
   GLdouble x;
   GLdouble y;
   GLdouble z;
};
uint32_t
_mesa_unmarshal_MatrixTranslatedEXT(struct gl_context *ctx, const struct marshal_cmd_MatrixTranslatedEXT *restrict cmd)
{
   GLenum matrixMode = cmd->matrixMode;
   GLdouble x = cmd->x;
   GLdouble y = cmd->y;
   GLdouble z = cmd->z;
   CALL_MatrixTranslatedEXT(ctx->Dispatch.Current, (matrixMode, x, y, z));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MatrixTranslatedEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_MatrixTranslatedEXT(GLenum matrixMode, GLdouble x, GLdouble y, GLdouble z)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MatrixTranslatedEXT);
   struct marshal_cmd_MatrixTranslatedEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MatrixTranslatedEXT, cmd_size);
   cmd->matrixMode = MIN2(matrixMode, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->x = x;
   cmd->y = y;
   cmd->z = z;
}


/* MatrixOrthoEXT: marshalled asynchronously */
struct marshal_cmd_MatrixOrthoEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 matrixMode;
   GLdouble l;
   GLdouble r;
   GLdouble b;
   GLdouble t;
   GLdouble n;
   GLdouble f;
};
uint32_t
_mesa_unmarshal_MatrixOrthoEXT(struct gl_context *ctx, const struct marshal_cmd_MatrixOrthoEXT *restrict cmd)
{
   GLenum matrixMode = cmd->matrixMode;
   GLdouble l = cmd->l;
   GLdouble r = cmd->r;
   GLdouble b = cmd->b;
   GLdouble t = cmd->t;
   GLdouble n = cmd->n;
   GLdouble f = cmd->f;
   CALL_MatrixOrthoEXT(ctx->Dispatch.Current, (matrixMode, l, r, b, t, n, f));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MatrixOrthoEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_MatrixOrthoEXT(GLenum matrixMode, GLdouble l, GLdouble r, GLdouble b, GLdouble t, GLdouble n, GLdouble f)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MatrixOrthoEXT);
   struct marshal_cmd_MatrixOrthoEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MatrixOrthoEXT, cmd_size);
   cmd->matrixMode = MIN2(matrixMode, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->l = l;
   cmd->r = r;
   cmd->b = b;
   cmd->t = t;
   cmd->n = n;
   cmd->f = f;
}


/* MatrixFrustumEXT: marshalled asynchronously */
struct marshal_cmd_MatrixFrustumEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 matrixMode;
   GLdouble l;
   GLdouble r;
   GLdouble b;
   GLdouble t;
   GLdouble n;
   GLdouble f;
};
uint32_t
_mesa_unmarshal_MatrixFrustumEXT(struct gl_context *ctx, const struct marshal_cmd_MatrixFrustumEXT *restrict cmd)
{
   GLenum matrixMode = cmd->matrixMode;
   GLdouble l = cmd->l;
   GLdouble r = cmd->r;
   GLdouble b = cmd->b;
   GLdouble t = cmd->t;
   GLdouble n = cmd->n;
   GLdouble f = cmd->f;
   CALL_MatrixFrustumEXT(ctx->Dispatch.Current, (matrixMode, l, r, b, t, n, f));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MatrixFrustumEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_MatrixFrustumEXT(GLenum matrixMode, GLdouble l, GLdouble r, GLdouble b, GLdouble t, GLdouble n, GLdouble f)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MatrixFrustumEXT);
   struct marshal_cmd_MatrixFrustumEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MatrixFrustumEXT, cmd_size);
   cmd->matrixMode = MIN2(matrixMode, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->l = l;
   cmd->r = r;
   cmd->b = b;
   cmd->t = t;
   cmd->n = n;
   cmd->f = f;
}


/* MatrixPushEXT: marshalled asynchronously */
struct marshal_cmd_MatrixPushEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 matrixMode;
};
uint32_t
_mesa_unmarshal_MatrixPushEXT(struct gl_context *ctx, const struct marshal_cmd_MatrixPushEXT *restrict cmd)
{
   GLenum matrixMode = cmd->matrixMode;
   CALL_MatrixPushEXT(ctx->Dispatch.Current, (matrixMode));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MatrixPushEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_MatrixPushEXT(GLenum matrixMode)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MatrixPushEXT);
   struct marshal_cmd_MatrixPushEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MatrixPushEXT, cmd_size);
   cmd->matrixMode = MIN2(matrixMode, 0xffff); /* clamped to 0xffff (invalid enum) */
   _mesa_glthread_MatrixPushEXT(ctx, matrixMode);
}


/* MatrixPopEXT: marshalled asynchronously */
struct marshal_cmd_MatrixPopEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 matrixMode;
};
uint32_t
_mesa_unmarshal_MatrixPopEXT(struct gl_context *ctx, const struct marshal_cmd_MatrixPopEXT *restrict cmd)
{
   GLenum matrixMode = cmd->matrixMode;
   CALL_MatrixPopEXT(ctx->Dispatch.Current, (matrixMode));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MatrixPopEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_MatrixPopEXT(GLenum matrixMode)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MatrixPopEXT);
   struct marshal_cmd_MatrixPopEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MatrixPopEXT, cmd_size);
   cmd->matrixMode = MIN2(matrixMode, 0xffff); /* clamped to 0xffff (invalid enum) */
   _mesa_glthread_MatrixPopEXT(ctx, matrixMode);
}


/* ClientAttribDefaultEXT: marshalled asynchronously */
struct marshal_cmd_ClientAttribDefaultEXT
{
   struct marshal_cmd_base cmd_base;
   GLbitfield mask;
};
uint32_t
_mesa_unmarshal_ClientAttribDefaultEXT(struct gl_context *ctx, const struct marshal_cmd_ClientAttribDefaultEXT *restrict cmd)
{
   GLbitfield mask = cmd->mask;
   CALL_ClientAttribDefaultEXT(ctx->Dispatch.Current, (mask));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_ClientAttribDefaultEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ClientAttribDefaultEXT(GLbitfield mask)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ClientAttribDefaultEXT);
   struct marshal_cmd_ClientAttribDefaultEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ClientAttribDefaultEXT, cmd_size);
   cmd->mask = mask;
   if (COMPAT) _mesa_glthread_ClientAttribDefault(ctx, mask);
}


void
_mesa_glthread_init_dispatch5(struct gl_context *ctx, struct _glapi_table *table)
{
   if (_mesa_is_desktop_gl(ctx)) {
      SET_BindFramebufferEXT(table, _mesa_marshal_BindFramebufferEXT);
      SET_BindRenderbufferEXT(table, _mesa_marshal_BindRenderbufferEXT);
      SET_BufferPageCommitmentARB(table, _mesa_marshal_BufferPageCommitmentARB);
      SET_ClientAttribDefaultEXT(table, _mesa_marshal_ClientAttribDefaultEXT);
      SET_CopyImageSubDataNV(table, _mesa_marshal_CopyImageSubDataNV);
      SET_DepthBoundsEXT(table, _mesa_marshal_DepthBoundsEXT);
      SET_GetCompressedTextureSubImage(table, _mesa_marshal_GetCompressedTextureSubImage);
      SET_GetDoublei_v(table, _mesa_marshal_GetDoublei_v);
      SET_GetTextureSubImage(table, _mesa_marshal_GetTextureSubImage);
      SET_MatrixFrustumEXT(table, _mesa_marshal_MatrixFrustumEXT);
      SET_MatrixLoadIdentityEXT(table, _mesa_marshal_MatrixLoadIdentityEXT);
      SET_MatrixLoaddEXT(table, _mesa_marshal_MatrixLoaddEXT);
      SET_MatrixLoadfEXT(table, _mesa_marshal_MatrixLoadfEXT);
      SET_MatrixMultdEXT(table, _mesa_marshal_MatrixMultdEXT);
      SET_MatrixMultfEXT(table, _mesa_marshal_MatrixMultfEXT);
      SET_MatrixOrthoEXT(table, _mesa_marshal_MatrixOrthoEXT);
      SET_MatrixPopEXT(table, _mesa_marshal_MatrixPopEXT);
      SET_MatrixPushEXT(table, _mesa_marshal_MatrixPushEXT);
      SET_MatrixRotatedEXT(table, _mesa_marshal_MatrixRotatedEXT);
      SET_MatrixRotatefEXT(table, _mesa_marshal_MatrixRotatefEXT);
      SET_MatrixScaledEXT(table, _mesa_marshal_MatrixScaledEXT);
      SET_MatrixScalefEXT(table, _mesa_marshal_MatrixScalefEXT);
      SET_MatrixTranslatedEXT(table, _mesa_marshal_MatrixTranslatedEXT);
      SET_MatrixTranslatefEXT(table, _mesa_marshal_MatrixTranslatefEXT);
      SET_NamedBufferPageCommitmentARB(table, _mesa_marshal_NamedBufferPageCommitmentARB);
      SET_NamedBufferPageCommitmentEXT(table, _mesa_marshal_NamedBufferPageCommitmentEXT);
      SET_ProvokingVertex(table, _mesa_marshal_ProvokingVertex);
      SET_SpecializeShaderARB(table, _mesa_marshal_SpecializeShaderARB);
      SET_StringMarkerGREMEDY(table, _mesa_marshal_StringMarkerGREMEDY);
      SET_TextureBarrierNV(table, _mesa_marshal_TextureBarrierNV);
   }
   if (_mesa_is_desktop_gl(ctx) || (_mesa_is_gles2(ctx) && ctx->Version >= 30)) {
      SET_ColorMaski(table, _mesa_marshal_ColorMaski);
      SET_Disablei(table, _mesa_marshal_Disablei);
      SET_Enablei(table, _mesa_marshal_Enablei);
      SET_GetIntegeri_v(table, _mesa_marshal_GetIntegeri_v);
      SET_IsEnabledi(table, _mesa_marshal_IsEnabledi);
   }
   if (_mesa_is_desktop_gl(ctx) || (_mesa_is_gles2(ctx) && ctx->Version >= 31)) {
      SET_EvaluateDepthValuesARB(table, _mesa_marshal_EvaluateDepthValuesARB);
      SET_FramebufferSampleLocationsfvARB(table, _mesa_marshal_FramebufferSampleLocationsfvARB);
      SET_GetBooleani_v(table, _mesa_marshal_GetBooleani_v);
      SET_GetFloati_v(table, _mesa_marshal_GetFloati_v);
      SET_NamedFramebufferSampleLocationsfvARB(table, _mesa_marshal_NamedFramebufferSampleLocationsfvARB);
      SET_PrimitiveBoundingBox(table, _mesa_marshal_PrimitiveBoundingBox);
   }
   if (_mesa_is_desktop_gl(ctx) || _mesa_is_gles2(ctx)) {
      SET_BeginPerfMonitorAMD(table, _mesa_marshal_BeginPerfMonitorAMD);
      SET_BlendBarrier(table, _mesa_marshal_BlendBarrier);
      SET_DeletePerfMonitorsAMD(table, _mesa_marshal_DeletePerfMonitorsAMD);
      SET_EndPerfMonitorAMD(table, _mesa_marshal_EndPerfMonitorAMD);
      SET_GenPerfMonitorsAMD(table, _mesa_marshal_GenPerfMonitorsAMD);
      SET_GetPerfMonitorCounterDataAMD(table, _mesa_marshal_GetPerfMonitorCounterDataAMD);
      SET_GetPerfMonitorCounterInfoAMD(table, _mesa_marshal_GetPerfMonitorCounterInfoAMD);
      SET_GetPerfMonitorCounterStringAMD(table, _mesa_marshal_GetPerfMonitorCounterStringAMD);
      SET_GetPerfMonitorCountersAMD(table, _mesa_marshal_GetPerfMonitorCountersAMD);
      SET_GetPerfMonitorGroupStringAMD(table, _mesa_marshal_GetPerfMonitorGroupStringAMD);
      SET_GetPerfMonitorGroupsAMD(table, _mesa_marshal_GetPerfMonitorGroupsAMD);
      SET_MaxShaderCompilerThreadsKHR(table, _mesa_marshal_MaxShaderCompilerThreadsKHR);
      SET_SelectPerfMonitorCountersAMD(table, _mesa_marshal_SelectPerfMonitorCountersAMD);
   }
   if (_mesa_is_desktop_gl_compat(ctx)) {
      SET_ActiveStencilFaceEXT(table, _mesa_marshal_ActiveStencilFaceEXT);
      SET_AlphaFragmentOp1ATI(table, _mesa_marshal_AlphaFragmentOp1ATI);
      SET_AlphaFragmentOp2ATI(table, _mesa_marshal_AlphaFragmentOp2ATI);
      SET_AlphaFragmentOp3ATI(table, _mesa_marshal_AlphaFragmentOp3ATI);
      SET_BeginFragmentShaderATI(table, _mesa_marshal_BeginFragmentShaderATI);
      SET_BindFragmentShaderATI(table, _mesa_marshal_BindFragmentShaderATI);
      SET_ColorFragmentOp1ATI(table, _mesa_marshal_ColorFragmentOp1ATI);
      SET_ColorFragmentOp2ATI(table, _mesa_marshal_ColorFragmentOp2ATI);
      SET_ColorFragmentOp3ATI(table, _mesa_marshal_ColorFragmentOp3ATI);
      SET_ColorPointerEXT(table, _mesa_marshal_ColorPointerEXT);
      SET_DeleteFragmentShaderATI(table, _mesa_marshal_DeleteFragmentShaderATI);
      SET_EdgeFlagPointerEXT(table, _mesa_marshal_EdgeFlagPointerEXT);
      SET_EndFragmentShaderATI(table, _mesa_marshal_EndFragmentShaderATI);
      SET_GenFragmentShadersATI(table, _mesa_marshal_GenFragmentShadersATI);
      SET_IndexPointerEXT(table, _mesa_marshal_IndexPointerEXT);
      SET_LockArraysEXT(table, _mesa_marshal_LockArraysEXT);
      SET_NormalPointerEXT(table, _mesa_marshal_NormalPointerEXT);
      SET_PassTexCoordATI(table, _mesa_marshal_PassTexCoordATI);
      SET_SampleMapATI(table, _mesa_marshal_SampleMapATI);
      SET_SetFragmentShaderConstantATI(table, _mesa_marshal_SetFragmentShaderConstantATI);
      SET_TexCoordPointerEXT(table, _mesa_marshal_TexCoordPointerEXT);
      SET_UnlockArraysEXT(table, _mesa_marshal_UnlockArraysEXT);
      SET_VertexAttrib1dNV(table, _mesa_marshal_VertexAttrib1dNV);
      SET_VertexAttrib1dvNV(table, _mesa_marshal_VertexAttrib1dvNV);
      SET_VertexAttrib1fNV(table, _mesa_marshal_VertexAttrib1fNV);
      SET_VertexAttrib1fvNV(table, _mesa_marshal_VertexAttrib1fvNV);
      SET_VertexAttrib1sNV(table, _mesa_marshal_VertexAttrib1sNV);
      SET_VertexAttrib1svNV(table, _mesa_marshal_VertexAttrib1svNV);
      SET_VertexAttrib2dNV(table, _mesa_marshal_VertexAttrib2dNV);
      SET_VertexAttrib2dvNV(table, _mesa_marshal_VertexAttrib2dvNV);
      SET_VertexAttrib2fNV(table, _mesa_marshal_VertexAttrib2fNV);
      SET_VertexAttrib2fvNV(table, _mesa_marshal_VertexAttrib2fvNV);
      SET_VertexAttrib2sNV(table, _mesa_marshal_VertexAttrib2sNV);
      SET_VertexAttrib2svNV(table, _mesa_marshal_VertexAttrib2svNV);
      SET_VertexAttrib3dNV(table, _mesa_marshal_VertexAttrib3dNV);
      SET_VertexAttrib3dvNV(table, _mesa_marshal_VertexAttrib3dvNV);
      SET_VertexAttrib3fNV(table, _mesa_marshal_VertexAttrib3fNV);
      SET_VertexAttrib3fvNV(table, _mesa_marshal_VertexAttrib3fvNV);
      SET_VertexAttrib3sNV(table, _mesa_marshal_VertexAttrib3sNV);
      SET_VertexAttrib3svNV(table, _mesa_marshal_VertexAttrib3svNV);
      SET_VertexAttrib4dNV(table, _mesa_marshal_VertexAttrib4dNV);
      SET_VertexAttrib4dvNV(table, _mesa_marshal_VertexAttrib4dvNV);
      SET_VertexAttrib4fNV(table, _mesa_marshal_VertexAttrib4fNV);
      SET_VertexAttrib4fvNV(table, _mesa_marshal_VertexAttrib4fvNV);
      SET_VertexAttrib4sNV(table, _mesa_marshal_VertexAttrib4sNV);
      SET_VertexAttrib4svNV(table, _mesa_marshal_VertexAttrib4svNV);
      SET_VertexAttrib4ubNV(table, _mesa_marshal_VertexAttrib4ubNV);
      SET_VertexAttrib4ubvNV(table, _mesa_marshal_VertexAttrib4ubvNV);
      SET_VertexAttribs1dvNV(table, _mesa_marshal_VertexAttribs1dvNV);
      SET_VertexAttribs1fvNV(table, _mesa_marshal_VertexAttribs1fvNV);
      SET_VertexAttribs1svNV(table, _mesa_marshal_VertexAttribs1svNV);
      SET_VertexAttribs2dvNV(table, _mesa_marshal_VertexAttribs2dvNV);
      SET_VertexAttribs2fvNV(table, _mesa_marshal_VertexAttribs2fvNV);
      SET_VertexAttribs2svNV(table, _mesa_marshal_VertexAttribs2svNV);
      SET_VertexAttribs3dvNV(table, _mesa_marshal_VertexAttribs3dvNV);
      SET_VertexAttribs3fvNV(table, _mesa_marshal_VertexAttribs3fvNV);
      SET_VertexAttribs3svNV(table, _mesa_marshal_VertexAttribs3svNV);
      SET_VertexAttribs4dvNV(table, _mesa_marshal_VertexAttribs4dvNV);
      SET_VertexAttribs4fvNV(table, _mesa_marshal_VertexAttribs4fvNV);
      SET_VertexAttribs4svNV(table, _mesa_marshal_VertexAttribs4svNV);
      SET_VertexAttribs4ubvNV(table, _mesa_marshal_VertexAttribs4ubvNV);
      SET_VertexPointerEXT(table, _mesa_marshal_VertexPointerEXT);
      SET_WindowPos4dMESA(table, _mesa_marshal_WindowPos4dMESA);
      SET_WindowPos4dvMESA(table, _mesa_marshal_WindowPos4dvMESA);
      SET_WindowPos4fMESA(table, _mesa_marshal_WindowPos4fMESA);
      SET_WindowPos4fvMESA(table, _mesa_marshal_WindowPos4fvMESA);
      SET_WindowPos4iMESA(table, _mesa_marshal_WindowPos4iMESA);
      SET_WindowPos4ivMESA(table, _mesa_marshal_WindowPos4ivMESA);
      SET_WindowPos4sMESA(table, _mesa_marshal_WindowPos4sMESA);
      SET_WindowPos4svMESA(table, _mesa_marshal_WindowPos4svMESA);
   }
   if (_mesa_is_desktop_gl_compat(ctx) || _mesa_is_desktop_gl_core(ctx)) {
      SET_CreateProgramPipelines(table, _mesa_marshal_CreateProgramPipelines);
      SET_CreateQueries(table, _mesa_marshal_CreateQueries);
      SET_CreateSamplers(table, _mesa_marshal_CreateSamplers);
      SET_CreateVertexArrays(table, _mesa_marshal_CreateVertexArrays);
      SET_DepthRangeArrayv(table, _mesa_marshal_DepthRangeArrayv);
      SET_DepthRangeIndexed(table, _mesa_marshal_DepthRangeIndexed);
      SET_DisableVertexArrayAttrib(table, _mesa_marshal_DisableVertexArrayAttrib);
      SET_EnableVertexArrayAttrib(table, _mesa_marshal_EnableVertexArrayAttrib);
      SET_GetQueryBufferObjecti64v(table, _mesa_marshal_GetQueryBufferObjecti64v);
      SET_GetQueryBufferObjectiv(table, _mesa_marshal_GetQueryBufferObjectiv);
      SET_GetQueryBufferObjectui64v(table, _mesa_marshal_GetQueryBufferObjectui64v);
      SET_GetQueryBufferObjectuiv(table, _mesa_marshal_GetQueryBufferObjectuiv);
      SET_GetTextureLevelParameterfv(table, _mesa_marshal_GetTextureLevelParameterfv);
      SET_GetTextureLevelParameteriv(table, _mesa_marshal_GetTextureLevelParameteriv);
      SET_GetTextureParameterIiv(table, _mesa_marshal_GetTextureParameterIiv);
      SET_GetTextureParameterIuiv(table, _mesa_marshal_GetTextureParameterIuiv);
      SET_GetTextureParameterfv(table, _mesa_marshal_GetTextureParameterfv);
      SET_GetTextureParameteriv(table, _mesa_marshal_GetTextureParameteriv);
      SET_GetUniformi64vARB(table, _mesa_marshal_GetUniformi64vARB);
      SET_GetUniformui64vARB(table, _mesa_marshal_GetUniformui64vARB);
      SET_GetVertexArrayIndexed64iv(table, _mesa_marshal_GetVertexArrayIndexed64iv);
      SET_GetVertexArrayIndexediv(table, _mesa_marshal_GetVertexArrayIndexediv);
      SET_GetVertexArrayiv(table, _mesa_marshal_GetVertexArrayiv);
      SET_GetnUniformi64vARB(table, _mesa_marshal_GetnUniformi64vARB);
      SET_GetnUniformui64vARB(table, _mesa_marshal_GetnUniformui64vARB);
      SET_ProgramUniform1i64ARB(table, _mesa_marshal_ProgramUniform1i64ARB);
      SET_ProgramUniform1i64vARB(table, _mesa_marshal_ProgramUniform1i64vARB);
      SET_ProgramUniform1ui64ARB(table, _mesa_marshal_ProgramUniform1ui64ARB);
      SET_ProgramUniform1ui64vARB(table, _mesa_marshal_ProgramUniform1ui64vARB);
      SET_ProgramUniform2i64ARB(table, _mesa_marshal_ProgramUniform2i64ARB);
      SET_ProgramUniform2i64vARB(table, _mesa_marshal_ProgramUniform2i64vARB);
      SET_ProgramUniform2ui64ARB(table, _mesa_marshal_ProgramUniform2ui64ARB);
      SET_ProgramUniform2ui64vARB(table, _mesa_marshal_ProgramUniform2ui64vARB);
      SET_ProgramUniform3i64ARB(table, _mesa_marshal_ProgramUniform3i64ARB);
      SET_ProgramUniform3i64vARB(table, _mesa_marshal_ProgramUniform3i64vARB);
      SET_ProgramUniform3ui64ARB(table, _mesa_marshal_ProgramUniform3ui64ARB);
      SET_ProgramUniform3ui64vARB(table, _mesa_marshal_ProgramUniform3ui64vARB);
      SET_ProgramUniform4i64ARB(table, _mesa_marshal_ProgramUniform4i64ARB);
      SET_ProgramUniform4i64vARB(table, _mesa_marshal_ProgramUniform4i64vARB);
      SET_ProgramUniform4ui64ARB(table, _mesa_marshal_ProgramUniform4ui64ARB);
      SET_ProgramUniform4ui64vARB(table, _mesa_marshal_ProgramUniform4ui64vARB);
      SET_Uniform1i64ARB(table, _mesa_marshal_Uniform1i64ARB);
      SET_Uniform1i64vARB(table, _mesa_marshal_Uniform1i64vARB);
      SET_Uniform1ui64ARB(table, _mesa_marshal_Uniform1ui64ARB);
      SET_Uniform1ui64vARB(table, _mesa_marshal_Uniform1ui64vARB);
      SET_Uniform2i64ARB(table, _mesa_marshal_Uniform2i64ARB);
      SET_Uniform2i64vARB(table, _mesa_marshal_Uniform2i64vARB);
      SET_Uniform2ui64ARB(table, _mesa_marshal_Uniform2ui64ARB);
      SET_Uniform2ui64vARB(table, _mesa_marshal_Uniform2ui64vARB);
      SET_Uniform3i64ARB(table, _mesa_marshal_Uniform3i64ARB);
      SET_Uniform3i64vARB(table, _mesa_marshal_Uniform3i64vARB);
      SET_Uniform3ui64ARB(table, _mesa_marshal_Uniform3ui64ARB);
      SET_Uniform3ui64vARB(table, _mesa_marshal_Uniform3ui64vARB);
      SET_Uniform4i64ARB(table, _mesa_marshal_Uniform4i64ARB);
      SET_Uniform4i64vARB(table, _mesa_marshal_Uniform4i64vARB);
      SET_Uniform4ui64ARB(table, _mesa_marshal_Uniform4ui64ARB);
      SET_Uniform4ui64vARB(table, _mesa_marshal_Uniform4ui64vARB);
      SET_VertexArrayAttribBinding(table, _mesa_marshal_VertexArrayAttribBinding);
      SET_VertexArrayAttribFormat(table, _mesa_marshal_VertexArrayAttribFormat);
      SET_VertexArrayAttribIFormat(table, _mesa_marshal_VertexArrayAttribIFormat);
      SET_VertexArrayAttribLFormat(table, _mesa_marshal_VertexArrayAttribLFormat);
      SET_VertexArrayBindingDivisor(table, _mesa_marshal_VertexArrayBindingDivisor);
      SET_VertexArrayElementBuffer(table, _mesa_marshal_VertexArrayElementBuffer);
      SET_VertexArrayVertexBuffer(table, _mesa_marshal_VertexArrayVertexBuffer);
      SET_VertexArrayVertexBuffers(table, _mesa_marshal_VertexArrayVertexBuffers);
   }
   if (_mesa_is_desktop_gl_compat(ctx) || _mesa_is_desktop_gl_core(ctx) || (_mesa_is_gles2(ctx) && ctx->Version >= 31)) {
      SET_ScissorArrayv(table, _mesa_marshal_ScissorArrayv);
      SET_ScissorIndexed(table, _mesa_marshal_ScissorIndexed);
      SET_ScissorIndexedv(table, _mesa_marshal_ScissorIndexedv);
      SET_ViewportArrayv(table, _mesa_marshal_ViewportArrayv);
      SET_ViewportIndexedf(table, _mesa_marshal_ViewportIndexedf);
      SET_ViewportIndexedfv(table, _mesa_marshal_ViewportIndexedfv);
   }
}
