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


#include "api_exec.h"
#include "glthread_marshal.h"
#include "dispatch.h"

#define COMPAT (ctx->API != API_OPENGL_CORE)

static inline int safe_mul(int a, int b)
{
    if (a < 0 || b < 0) return -1;
    if (a == 0 || b == 0) return 0;
    if (a > INT_MAX / b) return -1;
    return a * b;
}

/* GetTextureParameterfv: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetTextureParameterfv(GLuint texture, GLenum pname, GLfloat * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetTextureParameterfv");
   CALL_GetTextureParameterfv(ctx->CurrentServerDispatch, (texture, pname, params));
}


/* GetTextureParameterIiv: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetTextureParameterIiv(GLuint texture, GLenum pname, GLint * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetTextureParameterIiv");
   CALL_GetTextureParameterIiv(ctx->CurrentServerDispatch, (texture, pname, params));
}


/* GetTextureParameterIuiv: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetTextureParameterIuiv(GLuint texture, GLenum pname, GLuint * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetTextureParameterIuiv");
   CALL_GetTextureParameterIuiv(ctx->CurrentServerDispatch, (texture, pname, params));
}


/* GetTextureParameteriv: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetTextureParameteriv(GLuint texture, GLenum pname, GLint * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetTextureParameteriv");
   CALL_GetTextureParameteriv(ctx->CurrentServerDispatch, (texture, pname, params));
}


/* CreateVertexArrays: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_CreateVertexArrays(GLsizei n, GLuint * arrays)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "CreateVertexArrays");
   CALL_CreateVertexArrays(ctx->CurrentServerDispatch, (n, arrays));
   if (COMPAT) _mesa_glthread_GenVertexArrays(ctx, n, arrays);
}


/* DisableVertexArrayAttrib: marshalled asynchronously */
struct marshal_cmd_DisableVertexArrayAttrib
{
   struct marshal_cmd_base cmd_base;
   GLuint vaobj;
   GLuint index;
};
void
_mesa_unmarshal_DisableVertexArrayAttrib(struct gl_context *ctx, const struct marshal_cmd_DisableVertexArrayAttrib *cmd)
{
   const GLuint vaobj = cmd->vaobj;
   const GLuint index = cmd->index;
   CALL_DisableVertexArrayAttrib(ctx->CurrentServerDispatch, (vaobj, index));
}
void GLAPIENTRY
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
void
_mesa_unmarshal_EnableVertexArrayAttrib(struct gl_context *ctx, const struct marshal_cmd_EnableVertexArrayAttrib *cmd)
{
   const GLuint vaobj = cmd->vaobj;
   const GLuint index = cmd->index;
   CALL_EnableVertexArrayAttrib(ctx->CurrentServerDispatch, (vaobj, index));
}
void GLAPIENTRY
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
void
_mesa_unmarshal_VertexArrayElementBuffer(struct gl_context *ctx, const struct marshal_cmd_VertexArrayElementBuffer *cmd)
{
   const GLuint vaobj = cmd->vaobj;
   const GLuint buffer = cmd->buffer;
   CALL_VertexArrayElementBuffer(ctx->CurrentServerDispatch, (vaobj, buffer));
}
void GLAPIENTRY
_mesa_marshal_VertexArrayElementBuffer(GLuint vaobj, GLuint buffer)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexArrayElementBuffer);
   struct marshal_cmd_VertexArrayElementBuffer *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexArrayElementBuffer, cmd_size);
   cmd->vaobj = vaobj;
   cmd->buffer = buffer;
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
void
_mesa_unmarshal_VertexArrayVertexBuffer(struct gl_context *ctx, const struct marshal_cmd_VertexArrayVertexBuffer *cmd)
{
   const GLuint vaobj = cmd->vaobj;
   const GLuint bindingindex = cmd->bindingindex;
   const GLuint buffer = cmd->buffer;
   const GLintptr offset = cmd->offset;
   const GLsizei stride = cmd->stride;
   CALL_VertexArrayVertexBuffer(ctx->CurrentServerDispatch, (vaobj, bindingindex, buffer, offset, stride));
}
void GLAPIENTRY
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
void
_mesa_unmarshal_VertexArrayVertexBuffers(struct gl_context *ctx, const struct marshal_cmd_VertexArrayVertexBuffers *cmd)
{
   const GLuint vaobj = cmd->vaobj;
   const GLuint first = cmd->first;
   const GLsizei count = cmd->count;
   GLuint * buffers;
   GLintptr * offsets;
   GLsizei * strides;
   const char *variable_data = (const char *) (cmd + 1);
   buffers = (GLuint *) variable_data;
   variable_data += count * 1 * sizeof(GLuint);
   offsets = (GLintptr *) variable_data;
   variable_data += count * 1 * sizeof(GLintptr);
   strides = (GLsizei *) variable_data;
   CALL_VertexArrayVertexBuffers(ctx->CurrentServerDispatch, (vaobj, first, count, buffers, offsets, strides));
}
void GLAPIENTRY
_mesa_marshal_VertexArrayVertexBuffers(GLuint vaobj, GLuint first, GLsizei count, const GLuint * buffers, const GLintptr * offsets, const GLsizei * strides)
{
   GET_CURRENT_CONTEXT(ctx);
   int buffers_size = safe_mul(count, 1 * sizeof(GLuint));
   int offsets_size = safe_mul(count, 1 * sizeof(GLintptr));
   int strides_size = safe_mul(count, 1 * sizeof(GLsizei));
   int cmd_size = sizeof(struct marshal_cmd_VertexArrayVertexBuffers) + buffers_size + offsets_size + strides_size;
   struct marshal_cmd_VertexArrayVertexBuffers *cmd;
   if (unlikely(buffers_size < 0 || (buffers_size > 0 && !buffers) || offsets_size < 0 || (offsets_size > 0 && !offsets) || strides_size < 0 || (strides_size > 0 && !strides) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "VertexArrayVertexBuffers");
      CALL_VertexArrayVertexBuffers(ctx->CurrentServerDispatch, (vaobj, first, count, buffers, offsets, strides));
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
void
_mesa_unmarshal_VertexArrayAttribFormat(struct gl_context *ctx, const struct marshal_cmd_VertexArrayAttribFormat *cmd)
{
   const GLuint vaobj = cmd->vaobj;
   const GLuint attribindex = cmd->attribindex;
   const GLint size = cmd->size;
   const GLenum type = cmd->type;
   const GLboolean normalized = cmd->normalized;
   const GLuint relativeoffset = cmd->relativeoffset;
   CALL_VertexArrayAttribFormat(ctx->CurrentServerDispatch, (vaobj, attribindex, size, type, normalized, relativeoffset));
}
void GLAPIENTRY
_mesa_marshal_VertexArrayAttribFormat(GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexArrayAttribFormat);
   struct marshal_cmd_VertexArrayAttribFormat *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexArrayAttribFormat, cmd_size);
   cmd->vaobj = vaobj;
   cmd->attribindex = attribindex;
   cmd->size = size;
   cmd->type = type;
   cmd->normalized = normalized;
   cmd->relativeoffset = relativeoffset;
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
void
_mesa_unmarshal_VertexArrayAttribIFormat(struct gl_context *ctx, const struct marshal_cmd_VertexArrayAttribIFormat *cmd)
{
   const GLuint vaobj = cmd->vaobj;
   const GLuint attribindex = cmd->attribindex;
   const GLint size = cmd->size;
   const GLenum type = cmd->type;
   const GLuint relativeoffset = cmd->relativeoffset;
   CALL_VertexArrayAttribIFormat(ctx->CurrentServerDispatch, (vaobj, attribindex, size, type, relativeoffset));
}
void GLAPIENTRY
_mesa_marshal_VertexArrayAttribIFormat(GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexArrayAttribIFormat);
   struct marshal_cmd_VertexArrayAttribIFormat *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexArrayAttribIFormat, cmd_size);
   cmd->vaobj = vaobj;
   cmd->attribindex = attribindex;
   cmd->size = size;
   cmd->type = type;
   cmd->relativeoffset = relativeoffset;
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
void
_mesa_unmarshal_VertexArrayAttribLFormat(struct gl_context *ctx, const struct marshal_cmd_VertexArrayAttribLFormat *cmd)
{
   const GLuint vaobj = cmd->vaobj;
   const GLuint attribindex = cmd->attribindex;
   const GLint size = cmd->size;
   const GLenum type = cmd->type;
   const GLuint relativeoffset = cmd->relativeoffset;
   CALL_VertexArrayAttribLFormat(ctx->CurrentServerDispatch, (vaobj, attribindex, size, type, relativeoffset));
}
void GLAPIENTRY
_mesa_marshal_VertexArrayAttribLFormat(GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexArrayAttribLFormat);
   struct marshal_cmd_VertexArrayAttribLFormat *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexArrayAttribLFormat, cmd_size);
   cmd->vaobj = vaobj;
   cmd->attribindex = attribindex;
   cmd->size = size;
   cmd->type = type;
   cmd->relativeoffset = relativeoffset;
}


/* VertexArrayAttribBinding: marshalled asynchronously */
struct marshal_cmd_VertexArrayAttribBinding
{
   struct marshal_cmd_base cmd_base;
   GLuint vaobj;
   GLuint attribindex;
   GLuint bindingindex;
};
void
_mesa_unmarshal_VertexArrayAttribBinding(struct gl_context *ctx, const struct marshal_cmd_VertexArrayAttribBinding *cmd)
{
   const GLuint vaobj = cmd->vaobj;
   const GLuint attribindex = cmd->attribindex;
   const GLuint bindingindex = cmd->bindingindex;
   CALL_VertexArrayAttribBinding(ctx->CurrentServerDispatch, (vaobj, attribindex, bindingindex));
}
void GLAPIENTRY
_mesa_marshal_VertexArrayAttribBinding(GLuint vaobj, GLuint attribindex, GLuint bindingindex)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexArrayAttribBinding);
   struct marshal_cmd_VertexArrayAttribBinding *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexArrayAttribBinding, cmd_size);
   cmd->vaobj = vaobj;
   cmd->attribindex = attribindex;
   cmd->bindingindex = bindingindex;
}


/* VertexArrayBindingDivisor: marshalled asynchronously */
struct marshal_cmd_VertexArrayBindingDivisor
{
   struct marshal_cmd_base cmd_base;
   GLuint vaobj;
   GLuint bindingindex;
   GLuint divisor;
};
void
_mesa_unmarshal_VertexArrayBindingDivisor(struct gl_context *ctx, const struct marshal_cmd_VertexArrayBindingDivisor *cmd)
{
   const GLuint vaobj = cmd->vaobj;
   const GLuint bindingindex = cmd->bindingindex;
   const GLuint divisor = cmd->divisor;
   CALL_VertexArrayBindingDivisor(ctx->CurrentServerDispatch, (vaobj, bindingindex, divisor));
}
void GLAPIENTRY
_mesa_marshal_VertexArrayBindingDivisor(GLuint vaobj, GLuint bindingindex, GLuint divisor)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexArrayBindingDivisor);
   struct marshal_cmd_VertexArrayBindingDivisor *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexArrayBindingDivisor, cmd_size);
   cmd->vaobj = vaobj;
   cmd->bindingindex = bindingindex;
   cmd->divisor = divisor;
}


/* GetVertexArrayiv: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetVertexArrayiv(GLuint vaobj, GLenum pname, GLint * param)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetVertexArrayiv");
   CALL_GetVertexArrayiv(ctx->CurrentServerDispatch, (vaobj, pname, param));
}


/* GetVertexArrayIndexediv: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetVertexArrayIndexediv(GLuint vaobj, GLuint index, GLenum pname, GLint * param)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetVertexArrayIndexediv");
   CALL_GetVertexArrayIndexediv(ctx->CurrentServerDispatch, (vaobj, index, pname, param));
}


/* GetVertexArrayIndexed64iv: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetVertexArrayIndexed64iv(GLuint vaobj, GLuint index, GLenum pname, GLint64 * param)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetVertexArrayIndexed64iv");
   CALL_GetVertexArrayIndexed64iv(ctx->CurrentServerDispatch, (vaobj, index, pname, param));
}


/* CreateSamplers: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_CreateSamplers(GLsizei n, GLuint * samplers)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "CreateSamplers");
   CALL_CreateSamplers(ctx->CurrentServerDispatch, (n, samplers));
}


/* CreateProgramPipelines: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_CreateProgramPipelines(GLsizei n, GLuint * pipelines)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "CreateProgramPipelines");
   CALL_CreateProgramPipelines(ctx->CurrentServerDispatch, (n, pipelines));
}


/* CreateQueries: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_CreateQueries(GLenum target, GLsizei n, GLuint * ids)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "CreateQueries");
   CALL_CreateQueries(ctx->CurrentServerDispatch, (target, n, ids));
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
void
_mesa_unmarshal_GetQueryBufferObjectiv(struct gl_context *ctx, const struct marshal_cmd_GetQueryBufferObjectiv *cmd)
{
   const GLuint id = cmd->id;
   const GLuint buffer = cmd->buffer;
   const GLenum pname = cmd->pname;
   const GLintptr offset = cmd->offset;
   CALL_GetQueryBufferObjectiv(ctx->CurrentServerDispatch, (id, buffer, pname, offset));
}
void GLAPIENTRY
_mesa_marshal_GetQueryBufferObjectiv(GLuint id, GLuint buffer, GLenum pname, GLintptr offset)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_GetQueryBufferObjectiv);
   struct marshal_cmd_GetQueryBufferObjectiv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_GetQueryBufferObjectiv, cmd_size);
   cmd->id = id;
   cmd->buffer = buffer;
   cmd->pname = pname;
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
void
_mesa_unmarshal_GetQueryBufferObjectuiv(struct gl_context *ctx, const struct marshal_cmd_GetQueryBufferObjectuiv *cmd)
{
   const GLuint id = cmd->id;
   const GLuint buffer = cmd->buffer;
   const GLenum pname = cmd->pname;
   const GLintptr offset = cmd->offset;
   CALL_GetQueryBufferObjectuiv(ctx->CurrentServerDispatch, (id, buffer, pname, offset));
}
void GLAPIENTRY
_mesa_marshal_GetQueryBufferObjectuiv(GLuint id, GLuint buffer, GLenum pname, GLintptr offset)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_GetQueryBufferObjectuiv);
   struct marshal_cmd_GetQueryBufferObjectuiv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_GetQueryBufferObjectuiv, cmd_size);
   cmd->id = id;
   cmd->buffer = buffer;
   cmd->pname = pname;
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
void
_mesa_unmarshal_GetQueryBufferObjecti64v(struct gl_context *ctx, const struct marshal_cmd_GetQueryBufferObjecti64v *cmd)
{
   const GLuint id = cmd->id;
   const GLuint buffer = cmd->buffer;
   const GLenum pname = cmd->pname;
   const GLintptr offset = cmd->offset;
   CALL_GetQueryBufferObjecti64v(ctx->CurrentServerDispatch, (id, buffer, pname, offset));
}
void GLAPIENTRY
_mesa_marshal_GetQueryBufferObjecti64v(GLuint id, GLuint buffer, GLenum pname, GLintptr offset)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_GetQueryBufferObjecti64v);
   struct marshal_cmd_GetQueryBufferObjecti64v *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_GetQueryBufferObjecti64v, cmd_size);
   cmd->id = id;
   cmd->buffer = buffer;
   cmd->pname = pname;
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
void
_mesa_unmarshal_GetQueryBufferObjectui64v(struct gl_context *ctx, const struct marshal_cmd_GetQueryBufferObjectui64v *cmd)
{
   const GLuint id = cmd->id;
   const GLuint buffer = cmd->buffer;
   const GLenum pname = cmd->pname;
   const GLintptr offset = cmd->offset;
   CALL_GetQueryBufferObjectui64v(ctx->CurrentServerDispatch, (id, buffer, pname, offset));
}
void GLAPIENTRY
_mesa_marshal_GetQueryBufferObjectui64v(GLuint id, GLuint buffer, GLenum pname, GLintptr offset)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_GetQueryBufferObjectui64v);
   struct marshal_cmd_GetQueryBufferObjectui64v *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_GetQueryBufferObjectui64v, cmd_size);
   cmd->id = id;
   cmd->buffer = buffer;
   cmd->pname = pname;
   cmd->offset = offset;
}


/* GetTextureSubImage: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetTextureSubImage(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, GLsizei bufSize, GLvoid * pixels)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetTextureSubImage");
   CALL_GetTextureSubImage(ctx->CurrentServerDispatch, (texture, level, xoffset, yoffset, zoffset, width, height, depth, format, type, bufSize, pixels));
}


/* GetCompressedTextureSubImage: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetCompressedTextureSubImage(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLsizei bufSize, GLvoid * pixels)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetCompressedTextureSubImage");
   CALL_GetCompressedTextureSubImage(ctx->CurrentServerDispatch, (texture, level, xoffset, yoffset, zoffset, width, height, depth, bufSize, pixels));
}


/* TextureBarrierNV: marshalled asynchronously */
struct marshal_cmd_TextureBarrierNV
{
   struct marshal_cmd_base cmd_base;
};
void
_mesa_unmarshal_TextureBarrierNV(struct gl_context *ctx, const struct marshal_cmd_TextureBarrierNV *cmd)
{
   CALL_TextureBarrierNV(ctx->CurrentServerDispatch, ());
}
void GLAPIENTRY
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
void
_mesa_unmarshal_BufferPageCommitmentARB(struct gl_context *ctx, const struct marshal_cmd_BufferPageCommitmentARB *cmd)
{
   const GLenum target = cmd->target;
   const GLintptr offset = cmd->offset;
   const GLsizeiptr size = cmd->size;
   const GLboolean commit = cmd->commit;
   CALL_BufferPageCommitmentARB(ctx->CurrentServerDispatch, (target, offset, size, commit));
}
void GLAPIENTRY
_mesa_marshal_BufferPageCommitmentARB(GLenum target, GLintptr offset, GLsizeiptr size, GLboolean commit)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_BufferPageCommitmentARB);
   struct marshal_cmd_BufferPageCommitmentARB *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_BufferPageCommitmentARB, cmd_size);
   cmd->target = target;
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
void
_mesa_unmarshal_NamedBufferPageCommitmentEXT(struct gl_context *ctx, const struct marshal_cmd_NamedBufferPageCommitmentEXT *cmd)
{
   const GLuint buffer = cmd->buffer;
   const GLintptr offset = cmd->offset;
   const GLsizeiptr size = cmd->size;
   const GLboolean commit = cmd->commit;
   CALL_NamedBufferPageCommitmentEXT(ctx->CurrentServerDispatch, (buffer, offset, size, commit));
}
void GLAPIENTRY
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
void
_mesa_unmarshal_NamedBufferPageCommitmentARB(struct gl_context *ctx, const struct marshal_cmd_NamedBufferPageCommitmentARB *cmd)
{
   const GLuint buffer = cmd->buffer;
   const GLintptr offset = cmd->offset;
   const GLsizeiptr size = cmd->size;
   const GLboolean commit = cmd->commit;
   CALL_NamedBufferPageCommitmentARB(ctx->CurrentServerDispatch, (buffer, offset, size, commit));
}
void GLAPIENTRY
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
void
_mesa_unmarshal_PrimitiveBoundingBox(struct gl_context *ctx, const struct marshal_cmd_PrimitiveBoundingBox *cmd)
{
   const GLfloat minX = cmd->minX;
   const GLfloat minY = cmd->minY;
   const GLfloat minZ = cmd->minZ;
   const GLfloat minW = cmd->minW;
   const GLfloat maxX = cmd->maxX;
   const GLfloat maxY = cmd->maxY;
   const GLfloat maxZ = cmd->maxZ;
   const GLfloat maxW = cmd->maxW;
   CALL_PrimitiveBoundingBox(ctx->CurrentServerDispatch, (minX, minY, minZ, minW, maxX, maxY, maxZ, maxW));
}
void GLAPIENTRY
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
void
_mesa_unmarshal_BlendBarrier(struct gl_context *ctx, const struct marshal_cmd_BlendBarrier *cmd)
{
   CALL_BlendBarrier(ctx->CurrentServerDispatch, ());
}
void GLAPIENTRY
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
void
_mesa_unmarshal_Uniform1i64ARB(struct gl_context *ctx, const struct marshal_cmd_Uniform1i64ARB *cmd)
{
   const GLint location = cmd->location;
   const GLint64 x = cmd->x;
   CALL_Uniform1i64ARB(ctx->CurrentServerDispatch, (location, x));
}
void GLAPIENTRY
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
void
_mesa_unmarshal_Uniform2i64ARB(struct gl_context *ctx, const struct marshal_cmd_Uniform2i64ARB *cmd)
{
   const GLint location = cmd->location;
   const GLint64 x = cmd->x;
   const GLint64 y = cmd->y;
   CALL_Uniform2i64ARB(ctx->CurrentServerDispatch, (location, x, y));
}
void GLAPIENTRY
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
void
_mesa_unmarshal_Uniform3i64ARB(struct gl_context *ctx, const struct marshal_cmd_Uniform3i64ARB *cmd)
{
   const GLint location = cmd->location;
   const GLint64 x = cmd->x;
   const GLint64 y = cmd->y;
   const GLint64 z = cmd->z;
   CALL_Uniform3i64ARB(ctx->CurrentServerDispatch, (location, x, y, z));
}
void GLAPIENTRY
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
void
_mesa_unmarshal_Uniform4i64ARB(struct gl_context *ctx, const struct marshal_cmd_Uniform4i64ARB *cmd)
{
   const GLint location = cmd->location;
   const GLint64 x = cmd->x;
   const GLint64 y = cmd->y;
   const GLint64 z = cmd->z;
   const GLint64 w = cmd->w;
   CALL_Uniform4i64ARB(ctx->CurrentServerDispatch, (location, x, y, z, w));
}
void GLAPIENTRY
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
void
_mesa_unmarshal_Uniform1i64vARB(struct gl_context *ctx, const struct marshal_cmd_Uniform1i64vARB *cmd)
{
   const GLint location = cmd->location;
   const GLsizei count = cmd->count;
   GLint64 * value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLint64 *) variable_data;
   CALL_Uniform1i64vARB(ctx->CurrentServerDispatch, (location, count, value));
}
void GLAPIENTRY
_mesa_marshal_Uniform1i64vARB(GLint location, GLsizei count, const GLint64 * value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 1 * sizeof(GLint64));
   int cmd_size = sizeof(struct marshal_cmd_Uniform1i64vARB) + value_size;
   struct marshal_cmd_Uniform1i64vARB *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "Uniform1i64vARB");
      CALL_Uniform1i64vARB(ctx->CurrentServerDispatch, (location, count, value));
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
void
_mesa_unmarshal_Uniform2i64vARB(struct gl_context *ctx, const struct marshal_cmd_Uniform2i64vARB *cmd)
{
   const GLint location = cmd->location;
   const GLsizei count = cmd->count;
   GLint64 * value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLint64 *) variable_data;
   CALL_Uniform2i64vARB(ctx->CurrentServerDispatch, (location, count, value));
}
void GLAPIENTRY
_mesa_marshal_Uniform2i64vARB(GLint location, GLsizei count, const GLint64 * value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 2 * sizeof(GLint64));
   int cmd_size = sizeof(struct marshal_cmd_Uniform2i64vARB) + value_size;
   struct marshal_cmd_Uniform2i64vARB *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "Uniform2i64vARB");
      CALL_Uniform2i64vARB(ctx->CurrentServerDispatch, (location, count, value));
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
void
_mesa_unmarshal_Uniform3i64vARB(struct gl_context *ctx, const struct marshal_cmd_Uniform3i64vARB *cmd)
{
   const GLint location = cmd->location;
   const GLsizei count = cmd->count;
   GLint64 * value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLint64 *) variable_data;
   CALL_Uniform3i64vARB(ctx->CurrentServerDispatch, (location, count, value));
}
void GLAPIENTRY
_mesa_marshal_Uniform3i64vARB(GLint location, GLsizei count, const GLint64 * value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 3 * sizeof(GLint64));
   int cmd_size = sizeof(struct marshal_cmd_Uniform3i64vARB) + value_size;
   struct marshal_cmd_Uniform3i64vARB *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "Uniform3i64vARB");
      CALL_Uniform3i64vARB(ctx->CurrentServerDispatch, (location, count, value));
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
void
_mesa_unmarshal_Uniform4i64vARB(struct gl_context *ctx, const struct marshal_cmd_Uniform4i64vARB *cmd)
{
   const GLint location = cmd->location;
   const GLsizei count = cmd->count;
   GLint64 * value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLint64 *) variable_data;
   CALL_Uniform4i64vARB(ctx->CurrentServerDispatch, (location, count, value));
}
void GLAPIENTRY
_mesa_marshal_Uniform4i64vARB(GLint location, GLsizei count, const GLint64 * value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 4 * sizeof(GLint64));
   int cmd_size = sizeof(struct marshal_cmd_Uniform4i64vARB) + value_size;
   struct marshal_cmd_Uniform4i64vARB *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "Uniform4i64vARB");
      CALL_Uniform4i64vARB(ctx->CurrentServerDispatch, (location, count, value));
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
void
_mesa_unmarshal_Uniform1ui64ARB(struct gl_context *ctx, const struct marshal_cmd_Uniform1ui64ARB *cmd)
{
   const GLint location = cmd->location;
   const GLuint64 x = cmd->x;
   CALL_Uniform1ui64ARB(ctx->CurrentServerDispatch, (location, x));
}
void GLAPIENTRY
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
void
_mesa_unmarshal_Uniform2ui64ARB(struct gl_context *ctx, const struct marshal_cmd_Uniform2ui64ARB *cmd)
{
   const GLint location = cmd->location;
   const GLuint64 x = cmd->x;
   const GLuint64 y = cmd->y;
   CALL_Uniform2ui64ARB(ctx->CurrentServerDispatch, (location, x, y));
}
void GLAPIENTRY
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
void
_mesa_unmarshal_Uniform3ui64ARB(struct gl_context *ctx, const struct marshal_cmd_Uniform3ui64ARB *cmd)
{
   const GLint location = cmd->location;
   const GLuint64 x = cmd->x;
   const GLuint64 y = cmd->y;
   const GLuint64 z = cmd->z;
   CALL_Uniform3ui64ARB(ctx->CurrentServerDispatch, (location, x, y, z));
}
void GLAPIENTRY
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
void
_mesa_unmarshal_Uniform4ui64ARB(struct gl_context *ctx, const struct marshal_cmd_Uniform4ui64ARB *cmd)
{
   const GLint location = cmd->location;
   const GLuint64 x = cmd->x;
   const GLuint64 y = cmd->y;
   const GLuint64 z = cmd->z;
   const GLuint64 w = cmd->w;
   CALL_Uniform4ui64ARB(ctx->CurrentServerDispatch, (location, x, y, z, w));
}
void GLAPIENTRY
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
void
_mesa_unmarshal_Uniform1ui64vARB(struct gl_context *ctx, const struct marshal_cmd_Uniform1ui64vARB *cmd)
{
   const GLint location = cmd->location;
   const GLsizei count = cmd->count;
   GLuint64 * value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLuint64 *) variable_data;
   CALL_Uniform1ui64vARB(ctx->CurrentServerDispatch, (location, count, value));
}
void GLAPIENTRY
_mesa_marshal_Uniform1ui64vARB(GLint location, GLsizei count, const GLuint64 * value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 1 * sizeof(GLuint64));
   int cmd_size = sizeof(struct marshal_cmd_Uniform1ui64vARB) + value_size;
   struct marshal_cmd_Uniform1ui64vARB *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "Uniform1ui64vARB");
      CALL_Uniform1ui64vARB(ctx->CurrentServerDispatch, (location, count, value));
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
void
_mesa_unmarshal_Uniform2ui64vARB(struct gl_context *ctx, const struct marshal_cmd_Uniform2ui64vARB *cmd)
{
   const GLint location = cmd->location;
   const GLsizei count = cmd->count;
   GLuint64 * value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLuint64 *) variable_data;
   CALL_Uniform2ui64vARB(ctx->CurrentServerDispatch, (location, count, value));
}
void GLAPIENTRY
_mesa_marshal_Uniform2ui64vARB(GLint location, GLsizei count, const GLuint64 * value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 2 * sizeof(GLuint64));
   int cmd_size = sizeof(struct marshal_cmd_Uniform2ui64vARB) + value_size;
   struct marshal_cmd_Uniform2ui64vARB *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "Uniform2ui64vARB");
      CALL_Uniform2ui64vARB(ctx->CurrentServerDispatch, (location, count, value));
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
void
_mesa_unmarshal_Uniform3ui64vARB(struct gl_context *ctx, const struct marshal_cmd_Uniform3ui64vARB *cmd)
{
   const GLint location = cmd->location;
   const GLsizei count = cmd->count;
   GLuint64 * value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLuint64 *) variable_data;
   CALL_Uniform3ui64vARB(ctx->CurrentServerDispatch, (location, count, value));
}
void GLAPIENTRY
_mesa_marshal_Uniform3ui64vARB(GLint location, GLsizei count, const GLuint64 * value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 3 * sizeof(GLuint64));
   int cmd_size = sizeof(struct marshal_cmd_Uniform3ui64vARB) + value_size;
   struct marshal_cmd_Uniform3ui64vARB *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "Uniform3ui64vARB");
      CALL_Uniform3ui64vARB(ctx->CurrentServerDispatch, (location, count, value));
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
void
_mesa_unmarshal_Uniform4ui64vARB(struct gl_context *ctx, const struct marshal_cmd_Uniform4ui64vARB *cmd)
{
   const GLint location = cmd->location;
   const GLsizei count = cmd->count;
   GLuint64 * value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLuint64 *) variable_data;
   CALL_Uniform4ui64vARB(ctx->CurrentServerDispatch, (location, count, value));
}
void GLAPIENTRY
_mesa_marshal_Uniform4ui64vARB(GLint location, GLsizei count, const GLuint64 * value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 4 * sizeof(GLuint64));
   int cmd_size = sizeof(struct marshal_cmd_Uniform4ui64vARB) + value_size;
   struct marshal_cmd_Uniform4ui64vARB *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "Uniform4ui64vARB");
      CALL_Uniform4ui64vARB(ctx->CurrentServerDispatch, (location, count, value));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Uniform4ui64vARB, cmd_size);
   cmd->location = location;
   cmd->count = count;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, value, value_size);
}


/* GetUniformi64vARB: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetUniformi64vARB(GLuint program, GLint location, GLint64 * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetUniformi64vARB");
   CALL_GetUniformi64vARB(ctx->CurrentServerDispatch, (program, location, params));
}


/* GetUniformui64vARB: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetUniformui64vARB(GLuint program, GLint location, GLuint64 * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetUniformui64vARB");
   CALL_GetUniformui64vARB(ctx->CurrentServerDispatch, (program, location, params));
}


/* GetnUniformi64vARB: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetnUniformi64vARB(GLuint program, GLint location, GLsizei bufSize, GLint64 * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetnUniformi64vARB");
   CALL_GetnUniformi64vARB(ctx->CurrentServerDispatch, (program, location, bufSize, params));
}


/* GetnUniformui64vARB: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetnUniformui64vARB(GLuint program, GLint location, GLsizei bufSize, GLuint64 * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetnUniformui64vARB");
   CALL_GetnUniformui64vARB(ctx->CurrentServerDispatch, (program, location, bufSize, params));
}


/* ProgramUniform1i64ARB: marshalled asynchronously */
struct marshal_cmd_ProgramUniform1i64ARB
{
   struct marshal_cmd_base cmd_base;
   GLuint program;
   GLint location;
   GLint64 x;
};
void
_mesa_unmarshal_ProgramUniform1i64ARB(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform1i64ARB *cmd)
{
   const GLuint program = cmd->program;
   const GLint location = cmd->location;
   const GLint64 x = cmd->x;
   CALL_ProgramUniform1i64ARB(ctx->CurrentServerDispatch, (program, location, x));
}
void GLAPIENTRY
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
void
_mesa_unmarshal_ProgramUniform2i64ARB(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform2i64ARB *cmd)
{
   const GLuint program = cmd->program;
   const GLint location = cmd->location;
   const GLint64 x = cmd->x;
   const GLint64 y = cmd->y;
   CALL_ProgramUniform2i64ARB(ctx->CurrentServerDispatch, (program, location, x, y));
}
void GLAPIENTRY
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
void
_mesa_unmarshal_ProgramUniform3i64ARB(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform3i64ARB *cmd)
{
   const GLuint program = cmd->program;
   const GLint location = cmd->location;
   const GLint64 x = cmd->x;
   const GLint64 y = cmd->y;
   const GLint64 z = cmd->z;
   CALL_ProgramUniform3i64ARB(ctx->CurrentServerDispatch, (program, location, x, y, z));
}
void GLAPIENTRY
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
void
_mesa_unmarshal_ProgramUniform4i64ARB(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform4i64ARB *cmd)
{
   const GLuint program = cmd->program;
   const GLint location = cmd->location;
   const GLint64 x = cmd->x;
   const GLint64 y = cmd->y;
   const GLint64 z = cmd->z;
   const GLint64 w = cmd->w;
   CALL_ProgramUniform4i64ARB(ctx->CurrentServerDispatch, (program, location, x, y, z, w));
}
void GLAPIENTRY
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
void
_mesa_unmarshal_ProgramUniform1i64vARB(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform1i64vARB *cmd)
{
   const GLuint program = cmd->program;
   const GLint location = cmd->location;
   const GLsizei count = cmd->count;
   GLint64 * value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLint64 *) variable_data;
   CALL_ProgramUniform1i64vARB(ctx->CurrentServerDispatch, (program, location, count, value));
}
void GLAPIENTRY
_mesa_marshal_ProgramUniform1i64vARB(GLuint program, GLint location, GLsizei count, const GLint64 * value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 1 * sizeof(GLint64));
   int cmd_size = sizeof(struct marshal_cmd_ProgramUniform1i64vARB) + value_size;
   struct marshal_cmd_ProgramUniform1i64vARB *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "ProgramUniform1i64vARB");
      CALL_ProgramUniform1i64vARB(ctx->CurrentServerDispatch, (program, location, count, value));
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
void
_mesa_unmarshal_ProgramUniform2i64vARB(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform2i64vARB *cmd)
{
   const GLuint program = cmd->program;
   const GLint location = cmd->location;
   const GLsizei count = cmd->count;
   GLint64 * value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLint64 *) variable_data;
   CALL_ProgramUniform2i64vARB(ctx->CurrentServerDispatch, (program, location, count, value));
}
void GLAPIENTRY
_mesa_marshal_ProgramUniform2i64vARB(GLuint program, GLint location, GLsizei count, const GLint64 * value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 2 * sizeof(GLint64));
   int cmd_size = sizeof(struct marshal_cmd_ProgramUniform2i64vARB) + value_size;
   struct marshal_cmd_ProgramUniform2i64vARB *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "ProgramUniform2i64vARB");
      CALL_ProgramUniform2i64vARB(ctx->CurrentServerDispatch, (program, location, count, value));
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
void
_mesa_unmarshal_ProgramUniform3i64vARB(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform3i64vARB *cmd)
{
   const GLuint program = cmd->program;
   const GLint location = cmd->location;
   const GLsizei count = cmd->count;
   GLint64 * value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLint64 *) variable_data;
   CALL_ProgramUniform3i64vARB(ctx->CurrentServerDispatch, (program, location, count, value));
}
void GLAPIENTRY
_mesa_marshal_ProgramUniform3i64vARB(GLuint program, GLint location, GLsizei count, const GLint64 * value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 3 * sizeof(GLint64));
   int cmd_size = sizeof(struct marshal_cmd_ProgramUniform3i64vARB) + value_size;
   struct marshal_cmd_ProgramUniform3i64vARB *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "ProgramUniform3i64vARB");
      CALL_ProgramUniform3i64vARB(ctx->CurrentServerDispatch, (program, location, count, value));
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
void
_mesa_unmarshal_ProgramUniform4i64vARB(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform4i64vARB *cmd)
{
   const GLuint program = cmd->program;
   const GLint location = cmd->location;
   const GLsizei count = cmd->count;
   GLint64 * value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLint64 *) variable_data;
   CALL_ProgramUniform4i64vARB(ctx->CurrentServerDispatch, (program, location, count, value));
}
void GLAPIENTRY
_mesa_marshal_ProgramUniform4i64vARB(GLuint program, GLint location, GLsizei count, const GLint64 * value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 4 * sizeof(GLint64));
   int cmd_size = sizeof(struct marshal_cmd_ProgramUniform4i64vARB) + value_size;
   struct marshal_cmd_ProgramUniform4i64vARB *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "ProgramUniform4i64vARB");
      CALL_ProgramUniform4i64vARB(ctx->CurrentServerDispatch, (program, location, count, value));
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
void
_mesa_unmarshal_ProgramUniform1ui64ARB(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform1ui64ARB *cmd)
{
   const GLuint program = cmd->program;
   const GLint location = cmd->location;
   const GLuint64 x = cmd->x;
   CALL_ProgramUniform1ui64ARB(ctx->CurrentServerDispatch, (program, location, x));
}
void GLAPIENTRY
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
void
_mesa_unmarshal_ProgramUniform2ui64ARB(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform2ui64ARB *cmd)
{
   const GLuint program = cmd->program;
   const GLint location = cmd->location;
   const GLuint64 x = cmd->x;
   const GLuint64 y = cmd->y;
   CALL_ProgramUniform2ui64ARB(ctx->CurrentServerDispatch, (program, location, x, y));
}
void GLAPIENTRY
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
void
_mesa_unmarshal_ProgramUniform3ui64ARB(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform3ui64ARB *cmd)
{
   const GLuint program = cmd->program;
   const GLint location = cmd->location;
   const GLuint64 x = cmd->x;
   const GLuint64 y = cmd->y;
   const GLuint64 z = cmd->z;
   CALL_ProgramUniform3ui64ARB(ctx->CurrentServerDispatch, (program, location, x, y, z));
}
void GLAPIENTRY
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
void
_mesa_unmarshal_ProgramUniform4ui64ARB(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform4ui64ARB *cmd)
{
   const GLuint program = cmd->program;
   const GLint location = cmd->location;
   const GLuint64 x = cmd->x;
   const GLuint64 y = cmd->y;
   const GLuint64 z = cmd->z;
   const GLuint64 w = cmd->w;
   CALL_ProgramUniform4ui64ARB(ctx->CurrentServerDispatch, (program, location, x, y, z, w));
}
void GLAPIENTRY
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
void
_mesa_unmarshal_ProgramUniform1ui64vARB(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform1ui64vARB *cmd)
{
   const GLuint program = cmd->program;
   const GLint location = cmd->location;
   const GLsizei count = cmd->count;
   GLuint64 * value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLuint64 *) variable_data;
   CALL_ProgramUniform1ui64vARB(ctx->CurrentServerDispatch, (program, location, count, value));
}
void GLAPIENTRY
_mesa_marshal_ProgramUniform1ui64vARB(GLuint program, GLint location, GLsizei count, const GLuint64 * value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 1 * sizeof(GLuint64));
   int cmd_size = sizeof(struct marshal_cmd_ProgramUniform1ui64vARB) + value_size;
   struct marshal_cmd_ProgramUniform1ui64vARB *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "ProgramUniform1ui64vARB");
      CALL_ProgramUniform1ui64vARB(ctx->CurrentServerDispatch, (program, location, count, value));
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
void
_mesa_unmarshal_ProgramUniform2ui64vARB(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform2ui64vARB *cmd)
{
   const GLuint program = cmd->program;
   const GLint location = cmd->location;
   const GLsizei count = cmd->count;
   GLuint64 * value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLuint64 *) variable_data;
   CALL_ProgramUniform2ui64vARB(ctx->CurrentServerDispatch, (program, location, count, value));
}
void GLAPIENTRY
_mesa_marshal_ProgramUniform2ui64vARB(GLuint program, GLint location, GLsizei count, const GLuint64 * value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 2 * sizeof(GLuint64));
   int cmd_size = sizeof(struct marshal_cmd_ProgramUniform2ui64vARB) + value_size;
   struct marshal_cmd_ProgramUniform2ui64vARB *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "ProgramUniform2ui64vARB");
      CALL_ProgramUniform2ui64vARB(ctx->CurrentServerDispatch, (program, location, count, value));
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
void
_mesa_unmarshal_ProgramUniform3ui64vARB(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform3ui64vARB *cmd)
{
   const GLuint program = cmd->program;
   const GLint location = cmd->location;
   const GLsizei count = cmd->count;
   GLuint64 * value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLuint64 *) variable_data;
   CALL_ProgramUniform3ui64vARB(ctx->CurrentServerDispatch, (program, location, count, value));
}
void GLAPIENTRY
_mesa_marshal_ProgramUniform3ui64vARB(GLuint program, GLint location, GLsizei count, const GLuint64 * value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 3 * sizeof(GLuint64));
   int cmd_size = sizeof(struct marshal_cmd_ProgramUniform3ui64vARB) + value_size;
   struct marshal_cmd_ProgramUniform3ui64vARB *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "ProgramUniform3ui64vARB");
      CALL_ProgramUniform3ui64vARB(ctx->CurrentServerDispatch, (program, location, count, value));
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
void
_mesa_unmarshal_ProgramUniform4ui64vARB(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform4ui64vARB *cmd)
{
   const GLuint program = cmd->program;
   const GLint location = cmd->location;
   const GLsizei count = cmd->count;
   GLuint64 * value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLuint64 *) variable_data;
   CALL_ProgramUniform4ui64vARB(ctx->CurrentServerDispatch, (program, location, count, value));
}
void GLAPIENTRY
_mesa_marshal_ProgramUniform4ui64vARB(GLuint program, GLint location, GLsizei count, const GLuint64 * value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 4 * sizeof(GLuint64));
   int cmd_size = sizeof(struct marshal_cmd_ProgramUniform4ui64vARB) + value_size;
   struct marshal_cmd_ProgramUniform4ui64vARB *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "ProgramUniform4ui64vARB");
      CALL_ProgramUniform4ui64vARB(ctx->CurrentServerDispatch, (program, location, count, value));
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
void
_mesa_unmarshal_MaxShaderCompilerThreadsKHR(struct gl_context *ctx, const struct marshal_cmd_MaxShaderCompilerThreadsKHR *cmd)
{
   const GLuint count = cmd->count;
   CALL_MaxShaderCompilerThreadsKHR(ctx->CurrentServerDispatch, (count));
}
void GLAPIENTRY
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
void
_mesa_unmarshal_SpecializeShaderARB(struct gl_context *ctx, const struct marshal_cmd_SpecializeShaderARB *cmd)
{
   const GLuint shader = cmd->shader;
   const GLuint numSpecializationConstants = cmd->numSpecializationConstants;
   GLchar * pEntryPoint;
   GLuint * pConstantIndex;
   GLuint * pConstantValue;
   const char *variable_data = (const char *) (cmd + 1);
   pEntryPoint = (GLchar *) variable_data;
   variable_data += (strlen(pEntryPoint) + 1);
   pConstantIndex = (GLuint *) variable_data;
   variable_data += numSpecializationConstants * 1 * sizeof(GLuint);
   pConstantValue = (GLuint *) variable_data;
   CALL_SpecializeShaderARB(ctx->CurrentServerDispatch, (shader, pEntryPoint, numSpecializationConstants, pConstantIndex, pConstantValue));
}
void GLAPIENTRY
_mesa_marshal_SpecializeShaderARB(GLuint shader, const GLchar * pEntryPoint, GLuint numSpecializationConstants, const GLuint * pConstantIndex, const GLuint * pConstantValue)
{
   GET_CURRENT_CONTEXT(ctx);
   int pEntryPoint_size = (strlen(pEntryPoint) + 1);
   int pConstantIndex_size = safe_mul(numSpecializationConstants, 1 * sizeof(GLuint));
   int pConstantValue_size = safe_mul(numSpecializationConstants, 1 * sizeof(GLuint));
   int cmd_size = sizeof(struct marshal_cmd_SpecializeShaderARB) + pEntryPoint_size + pConstantIndex_size + pConstantValue_size;
   struct marshal_cmd_SpecializeShaderARB *cmd;
   if (unlikely(pEntryPoint_size < 0 || (pEntryPoint_size > 0 && !pEntryPoint) || pConstantIndex_size < 0 || (pConstantIndex_size > 0 && !pConstantIndex) || pConstantValue_size < 0 || (pConstantValue_size > 0 && !pConstantValue) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "SpecializeShaderARB");
      CALL_SpecializeShaderARB(ctx->CurrentServerDispatch, (shader, pEntryPoint, numSpecializationConstants, pConstantIndex, pConstantValue));
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
void
_mesa_unmarshal_ColorPointerEXT(struct gl_context *ctx, const struct marshal_cmd_ColorPointerEXT *cmd)
{
   const GLint size = cmd->size;
   const GLenum type = cmd->type;
   const GLsizei stride = cmd->stride;
   const GLsizei count = cmd->count;
   const GLvoid * pointer = cmd->pointer;
   CALL_ColorPointerEXT(ctx->CurrentServerDispatch, (size, type, stride, count, pointer));
}
void GLAPIENTRY
_mesa_marshal_ColorPointerEXT(GLint size, GLenum type, GLsizei stride, GLsizei count, const GLvoid * pointer)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ColorPointerEXT);
   struct marshal_cmd_ColorPointerEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ColorPointerEXT, cmd_size);
   cmd->size = size;
   cmd->type = type;
   cmd->stride = stride;
   cmd->count = count;
   cmd->pointer = pointer;
   if (COMPAT) _mesa_glthread_AttribPointer(ctx, VERT_ATTRIB_COLOR0);
}


/* EdgeFlagPointerEXT: marshalled asynchronously */
struct marshal_cmd_EdgeFlagPointerEXT
{
   struct marshal_cmd_base cmd_base;
   GLsizei stride;
   GLsizei count;
   const GLboolean * pointer;
};
void
_mesa_unmarshal_EdgeFlagPointerEXT(struct gl_context *ctx, const struct marshal_cmd_EdgeFlagPointerEXT *cmd)
{
   const GLsizei stride = cmd->stride;
   const GLsizei count = cmd->count;
   const GLboolean * pointer = cmd->pointer;
   CALL_EdgeFlagPointerEXT(ctx->CurrentServerDispatch, (stride, count, pointer));
}
void GLAPIENTRY
_mesa_marshal_EdgeFlagPointerEXT(GLsizei stride, GLsizei count, const GLboolean * pointer)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_EdgeFlagPointerEXT);
   struct marshal_cmd_EdgeFlagPointerEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_EdgeFlagPointerEXT, cmd_size);
   cmd->stride = stride;
   cmd->count = count;
   cmd->pointer = pointer;
   if (COMPAT) _mesa_glthread_AttribPointer(ctx, VERT_ATTRIB_EDGEFLAG);
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
void
_mesa_unmarshal_IndexPointerEXT(struct gl_context *ctx, const struct marshal_cmd_IndexPointerEXT *cmd)
{
   const GLenum type = cmd->type;
   const GLsizei stride = cmd->stride;
   const GLsizei count = cmd->count;
   const GLvoid * pointer = cmd->pointer;
   CALL_IndexPointerEXT(ctx->CurrentServerDispatch, (type, stride, count, pointer));
}
void GLAPIENTRY
_mesa_marshal_IndexPointerEXT(GLenum type, GLsizei stride, GLsizei count, const GLvoid * pointer)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_IndexPointerEXT);
   struct marshal_cmd_IndexPointerEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_IndexPointerEXT, cmd_size);
   cmd->type = type;
   cmd->stride = stride;
   cmd->count = count;
   cmd->pointer = pointer;
   if (COMPAT) _mesa_glthread_AttribPointer(ctx, VERT_ATTRIB_COLOR_INDEX);
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
void
_mesa_unmarshal_NormalPointerEXT(struct gl_context *ctx, const struct marshal_cmd_NormalPointerEXT *cmd)
{
   const GLenum type = cmd->type;
   const GLsizei stride = cmd->stride;
   const GLsizei count = cmd->count;
   const GLvoid * pointer = cmd->pointer;
   CALL_NormalPointerEXT(ctx->CurrentServerDispatch, (type, stride, count, pointer));
}
void GLAPIENTRY
_mesa_marshal_NormalPointerEXT(GLenum type, GLsizei stride, GLsizei count, const GLvoid * pointer)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_NormalPointerEXT);
   struct marshal_cmd_NormalPointerEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_NormalPointerEXT, cmd_size);
   cmd->type = type;
   cmd->stride = stride;
   cmd->count = count;
   cmd->pointer = pointer;
   if (COMPAT) _mesa_glthread_AttribPointer(ctx, VERT_ATTRIB_NORMAL);
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
void
_mesa_unmarshal_TexCoordPointerEXT(struct gl_context *ctx, const struct marshal_cmd_TexCoordPointerEXT *cmd)
{
   const GLint size = cmd->size;
   const GLenum type = cmd->type;
   const GLsizei stride = cmd->stride;
   const GLsizei count = cmd->count;
   const GLvoid * pointer = cmd->pointer;
   CALL_TexCoordPointerEXT(ctx->CurrentServerDispatch, (size, type, stride, count, pointer));
}
void GLAPIENTRY
_mesa_marshal_TexCoordPointerEXT(GLint size, GLenum type, GLsizei stride, GLsizei count, const GLvoid * pointer)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TexCoordPointerEXT);
   struct marshal_cmd_TexCoordPointerEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexCoordPointerEXT, cmd_size);
   cmd->size = size;
   cmd->type = type;
   cmd->stride = stride;
   cmd->count = count;
   cmd->pointer = pointer;
   if (COMPAT) _mesa_glthread_AttribPointer(ctx, VERT_ATTRIB_TEX(ctx->GLThread.ClientActiveTexture));
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
void
_mesa_unmarshal_VertexPointerEXT(struct gl_context *ctx, const struct marshal_cmd_VertexPointerEXT *cmd)
{
   const GLint size = cmd->size;
   const GLenum type = cmd->type;
   const GLsizei stride = cmd->stride;
   const GLsizei count = cmd->count;
   const GLvoid * pointer = cmd->pointer;
   CALL_VertexPointerEXT(ctx->CurrentServerDispatch, (size, type, stride, count, pointer));
}
void GLAPIENTRY
_mesa_marshal_VertexPointerEXT(GLint size, GLenum type, GLsizei stride, GLsizei count, const GLvoid * pointer)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexPointerEXT);
   struct marshal_cmd_VertexPointerEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexPointerEXT, cmd_size);
   cmd->size = size;
   cmd->type = type;
   cmd->stride = stride;
   cmd->count = count;
   cmd->pointer = pointer;
   if (COMPAT) _mesa_glthread_AttribPointer(ctx, VERT_ATTRIB_POS);
}


/* LockArraysEXT: marshalled asynchronously */
struct marshal_cmd_LockArraysEXT
{
   struct marshal_cmd_base cmd_base;
   GLint first;
   GLsizei count;
};
void
_mesa_unmarshal_LockArraysEXT(struct gl_context *ctx, const struct marshal_cmd_LockArraysEXT *cmd)
{
   const GLint first = cmd->first;
   const GLsizei count = cmd->count;
   CALL_LockArraysEXT(ctx->CurrentServerDispatch, (first, count));
}
void GLAPIENTRY
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
void
_mesa_unmarshal_UnlockArraysEXT(struct gl_context *ctx, const struct marshal_cmd_UnlockArraysEXT *cmd)
{
   CALL_UnlockArraysEXT(ctx->CurrentServerDispatch, ());
}
void GLAPIENTRY
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
void
_mesa_unmarshal_ViewportArrayv(struct gl_context *ctx, const struct marshal_cmd_ViewportArrayv *cmd)
{
   const GLuint first = cmd->first;
   const GLsizei count = cmd->count;
   GLfloat * v;
   const char *variable_data = (const char *) (cmd + 1);
   v = (GLfloat *) variable_data;
   CALL_ViewportArrayv(ctx->CurrentServerDispatch, (first, count, v));
}
void GLAPIENTRY
_mesa_marshal_ViewportArrayv(GLuint first, GLsizei count, const GLfloat * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int v_size = safe_mul(count, 4 * sizeof(GLfloat));
   int cmd_size = sizeof(struct marshal_cmd_ViewportArrayv) + v_size;
   struct marshal_cmd_ViewportArrayv *cmd;
   if (unlikely(v_size < 0 || (v_size > 0 && !v) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "ViewportArrayv");
      CALL_ViewportArrayv(ctx->CurrentServerDispatch, (first, count, v));
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
void
_mesa_unmarshal_ViewportIndexedf(struct gl_context *ctx, const struct marshal_cmd_ViewportIndexedf *cmd)
{
   const GLuint index = cmd->index;
   const GLfloat x = cmd->x;
   const GLfloat y = cmd->y;
   const GLfloat w = cmd->w;
   const GLfloat h = cmd->h;
   CALL_ViewportIndexedf(ctx->CurrentServerDispatch, (index, x, y, w, h));
}
void GLAPIENTRY
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
void
_mesa_unmarshal_ViewportIndexedfv(struct gl_context *ctx, const struct marshal_cmd_ViewportIndexedfv *cmd)
{
   const GLuint index = cmd->index;
   const GLfloat * v = cmd->v;
   CALL_ViewportIndexedfv(ctx->CurrentServerDispatch, (index, v));
}
void GLAPIENTRY
_mesa_marshal_ViewportIndexedfv(GLuint index, const GLfloat * v)
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
void
_mesa_unmarshal_ScissorArrayv(struct gl_context *ctx, const struct marshal_cmd_ScissorArrayv *cmd)
{
   const GLuint first = cmd->first;
   const GLsizei count = cmd->count;
   int * v;
   const char *variable_data = (const char *) (cmd + 1);
   v = (int *) variable_data;
   CALL_ScissorArrayv(ctx->CurrentServerDispatch, (first, count, v));
}
void GLAPIENTRY
_mesa_marshal_ScissorArrayv(GLuint first, GLsizei count, const int * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int v_size = safe_mul(count, 4 * sizeof(int));
   int cmd_size = sizeof(struct marshal_cmd_ScissorArrayv) + v_size;
   struct marshal_cmd_ScissorArrayv *cmd;
   if (unlikely(v_size < 0 || (v_size > 0 && !v) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "ScissorArrayv");
      CALL_ScissorArrayv(ctx->CurrentServerDispatch, (first, count, v));
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
void
_mesa_unmarshal_ScissorIndexed(struct gl_context *ctx, const struct marshal_cmd_ScissorIndexed *cmd)
{
   const GLuint index = cmd->index;
   const GLint left = cmd->left;
   const GLint bottom = cmd->bottom;
   const GLsizei width = cmd->width;
   const GLsizei height = cmd->height;
   CALL_ScissorIndexed(ctx->CurrentServerDispatch, (index, left, bottom, width, height));
}
void GLAPIENTRY
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
void
_mesa_unmarshal_ScissorIndexedv(struct gl_context *ctx, const struct marshal_cmd_ScissorIndexedv *cmd)
{
   const GLuint index = cmd->index;
   const GLint * v = cmd->v;
   CALL_ScissorIndexedv(ctx->CurrentServerDispatch, (index, v));
}
void GLAPIENTRY
_mesa_marshal_ScissorIndexedv(GLuint index, const GLint * v)
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
void
_mesa_unmarshal_DepthRangeArrayv(struct gl_context *ctx, const struct marshal_cmd_DepthRangeArrayv *cmd)
{
   const GLuint first = cmd->first;
   const GLsizei count = cmd->count;
   GLclampd * v;
   const char *variable_data = (const char *) (cmd + 1);
   v = (GLclampd *) variable_data;
   CALL_DepthRangeArrayv(ctx->CurrentServerDispatch, (first, count, v));
}
void GLAPIENTRY
_mesa_marshal_DepthRangeArrayv(GLuint first, GLsizei count, const GLclampd * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int v_size = safe_mul(count, 2 * sizeof(GLclampd));
   int cmd_size = sizeof(struct marshal_cmd_DepthRangeArrayv) + v_size;
   struct marshal_cmd_DepthRangeArrayv *cmd;
   if (unlikely(v_size < 0 || (v_size > 0 && !v) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "DepthRangeArrayv");
      CALL_DepthRangeArrayv(ctx->CurrentServerDispatch, (first, count, v));
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
void
_mesa_unmarshal_DepthRangeIndexed(struct gl_context *ctx, const struct marshal_cmd_DepthRangeIndexed *cmd)
{
   const GLuint index = cmd->index;
   const GLclampd n = cmd->n;
   const GLclampd f = cmd->f;
   CALL_DepthRangeIndexed(ctx->CurrentServerDispatch, (index, n, f));
}
void GLAPIENTRY
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
void GLAPIENTRY
_mesa_marshal_GetFloati_v(GLenum target, GLuint index, GLfloat * data)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetFloati_v");
   CALL_GetFloati_v(ctx->CurrentServerDispatch, (target, index, data));
}


/* GetDoublei_v: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetDoublei_v(GLenum target, GLuint index, GLdouble * data)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetDoublei_v");
   CALL_GetDoublei_v(ctx->CurrentServerDispatch, (target, index, data));
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
void
_mesa_unmarshal_FramebufferSampleLocationsfvARB(struct gl_context *ctx, const struct marshal_cmd_FramebufferSampleLocationsfvARB *cmd)
{
   const GLenum target = cmd->target;
   const GLuint start = cmd->start;
   const GLsizei count = cmd->count;
   GLfloat * v;
   const char *variable_data = (const char *) (cmd + 1);
   v = (GLfloat *) variable_data;
   CALL_FramebufferSampleLocationsfvARB(ctx->CurrentServerDispatch, (target, start, count, v));
}
void GLAPIENTRY
_mesa_marshal_FramebufferSampleLocationsfvARB(GLenum target, GLuint start, GLsizei count, const GLfloat * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int v_size = safe_mul((2 * count), 1 * sizeof(GLfloat));
   int cmd_size = sizeof(struct marshal_cmd_FramebufferSampleLocationsfvARB) + v_size;
   struct marshal_cmd_FramebufferSampleLocationsfvARB *cmd;
   if (unlikely(v_size < 0 || (v_size > 0 && !v) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "FramebufferSampleLocationsfvARB");
      CALL_FramebufferSampleLocationsfvARB(ctx->CurrentServerDispatch, (target, start, count, v));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_FramebufferSampleLocationsfvARB, cmd_size);
   cmd->target = target;
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
void
_mesa_unmarshal_NamedFramebufferSampleLocationsfvARB(struct gl_context *ctx, const struct marshal_cmd_NamedFramebufferSampleLocationsfvARB *cmd)
{
   const GLuint framebuffer = cmd->framebuffer;
   const GLuint start = cmd->start;
   const GLsizei count = cmd->count;
   GLfloat * v;
   const char *variable_data = (const char *) (cmd + 1);
   v = (GLfloat *) variable_data;
   CALL_NamedFramebufferSampleLocationsfvARB(ctx->CurrentServerDispatch, (framebuffer, start, count, v));
}
void GLAPIENTRY
_mesa_marshal_NamedFramebufferSampleLocationsfvARB(GLuint framebuffer, GLuint start, GLsizei count, const GLfloat * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int v_size = safe_mul((2 * count), 1 * sizeof(GLfloat));
   int cmd_size = sizeof(struct marshal_cmd_NamedFramebufferSampleLocationsfvARB) + v_size;
   struct marshal_cmd_NamedFramebufferSampleLocationsfvARB *cmd;
   if (unlikely(v_size < 0 || (v_size > 0 && !v) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "NamedFramebufferSampleLocationsfvARB");
      CALL_NamedFramebufferSampleLocationsfvARB(ctx->CurrentServerDispatch, (framebuffer, start, count, v));
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
void
_mesa_unmarshal_EvaluateDepthValuesARB(struct gl_context *ctx, const struct marshal_cmd_EvaluateDepthValuesARB *cmd)
{
   CALL_EvaluateDepthValuesARB(ctx->CurrentServerDispatch, ());
}
void GLAPIENTRY
_mesa_marshal_EvaluateDepthValuesARB(void)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_EvaluateDepthValuesARB);
   struct marshal_cmd_EvaluateDepthValuesARB *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_EvaluateDepthValuesARB, cmd_size);
   (void) cmd;
}


