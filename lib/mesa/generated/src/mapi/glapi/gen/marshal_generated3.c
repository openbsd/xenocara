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

/* FramebufferRenderbuffer: marshalled asynchronously */
struct marshal_cmd_FramebufferRenderbuffer
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLenum16 attachment;
   GLenum16 renderbuffertarget;
   GLuint renderbuffer;
};
uint32_t
_mesa_unmarshal_FramebufferRenderbuffer(struct gl_context *ctx, const struct marshal_cmd_FramebufferRenderbuffer *restrict cmd)
{
   GLenum target = cmd->target;
   GLenum attachment = cmd->attachment;
   GLenum renderbuffertarget = cmd->renderbuffertarget;
   GLuint renderbuffer = cmd->renderbuffer;
   CALL_FramebufferRenderbuffer(ctx->Dispatch.Current, (target, attachment, renderbuffertarget, renderbuffer));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_FramebufferRenderbuffer), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_FramebufferRenderbuffer(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_FramebufferRenderbuffer);
   struct marshal_cmd_FramebufferRenderbuffer *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_FramebufferRenderbuffer, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->attachment = MIN2(attachment, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->renderbuffertarget = MIN2(renderbuffertarget, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->renderbuffer = renderbuffer;
}


/* GetFramebufferAttachmentParameteriv: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetFramebufferAttachmentParameteriv(GLenum target, GLenum attachment, GLenum pname, GLint *params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetFramebufferAttachmentParameteriv");
   CALL_GetFramebufferAttachmentParameteriv(ctx->Dispatch.Current, (target, attachment, pname, params));
}


/* BlitFramebuffer: marshalled asynchronously */
struct marshal_cmd_BlitFramebuffer
{
   struct marshal_cmd_base cmd_base;
   GLenum16 filter;
   GLint srcX0;
   GLint srcY0;
   GLint srcX1;
   GLint srcY1;
   GLint dstX0;
   GLint dstY0;
   GLint dstX1;
   GLint dstY1;
   GLbitfield mask;
};
uint32_t
_mesa_unmarshal_BlitFramebuffer(struct gl_context *ctx, const struct marshal_cmd_BlitFramebuffer *restrict cmd)
{
   GLint srcX0 = cmd->srcX0;
   GLint srcY0 = cmd->srcY0;
   GLint srcX1 = cmd->srcX1;
   GLint srcY1 = cmd->srcY1;
   GLint dstX0 = cmd->dstX0;
   GLint dstY0 = cmd->dstY0;
   GLint dstX1 = cmd->dstX1;
   GLint dstY1 = cmd->dstY1;
   GLbitfield mask = cmd->mask;
   GLenum filter = cmd->filter;
   CALL_BlitFramebuffer(ctx->Dispatch.Current, (srcX0, srcY0, srcX1, srcY1, dstX0, dstY0, dstX1, dstY1, mask, filter));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_BlitFramebuffer), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_BlitFramebuffer(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_BlitFramebuffer);
   struct marshal_cmd_BlitFramebuffer *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_BlitFramebuffer, cmd_size);
   cmd->srcX0 = srcX0;
   cmd->srcY0 = srcY0;
   cmd->srcX1 = srcX1;
   cmd->srcY1 = srcY1;
   cmd->dstX0 = dstX0;
   cmd->dstY0 = dstY0;
   cmd->dstX1 = dstX1;
   cmd->dstY1 = dstY1;
   cmd->mask = mask;
   cmd->filter = MIN2(filter, 0xffff); /* clamped to 0xffff (invalid enum) */
}


/* GenerateMipmap: marshalled asynchronously */
struct marshal_cmd_GenerateMipmap
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
};
uint32_t
_mesa_unmarshal_GenerateMipmap(struct gl_context *ctx, const struct marshal_cmd_GenerateMipmap *restrict cmd)
{
   GLenum target = cmd->target;
   CALL_GenerateMipmap(ctx->Dispatch.Current, (target));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_GenerateMipmap), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_GenerateMipmap(GLenum target)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_GenerateMipmap);
   struct marshal_cmd_GenerateMipmap *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_GenerateMipmap, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
}


/* VertexAttribDivisor: marshalled asynchronously */
struct marshal_cmd_VertexAttribDivisor
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLuint divisor;
};
uint32_t
_mesa_unmarshal_VertexAttribDivisor(struct gl_context *ctx, const struct marshal_cmd_VertexAttribDivisor *restrict cmd)
{
   GLuint index = cmd->index;
   GLuint divisor = cmd->divisor;
   CALL_VertexAttribDivisor(ctx->Dispatch.Current, (index, divisor));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexAttribDivisor), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_VertexAttribDivisor(GLuint index, GLuint divisor)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttribDivisor);
   struct marshal_cmd_VertexAttribDivisor *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttribDivisor, cmd_size);
   cmd->index = index;
   cmd->divisor = divisor;
   if (COMPAT) _mesa_glthread_AttribDivisor(ctx, NULL, VERT_ATTRIB_GENERIC(index), divisor);
}


/* VertexArrayVertexAttribDivisorEXT: marshalled asynchronously */
struct marshal_cmd_VertexArrayVertexAttribDivisorEXT
{
   struct marshal_cmd_base cmd_base;
   GLuint vaobj;
   GLuint index;
   GLuint divisor;
};
uint32_t
_mesa_unmarshal_VertexArrayVertexAttribDivisorEXT(struct gl_context *ctx, const struct marshal_cmd_VertexArrayVertexAttribDivisorEXT *restrict cmd)
{
   GLuint vaobj = cmd->vaobj;
   GLuint index = cmd->index;
   GLuint divisor = cmd->divisor;
   CALL_VertexArrayVertexAttribDivisorEXT(ctx->Dispatch.Current, (vaobj, index, divisor));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexArrayVertexAttribDivisorEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_VertexArrayVertexAttribDivisorEXT(GLuint vaobj, GLuint index, GLuint divisor)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexArrayVertexAttribDivisorEXT);
   struct marshal_cmd_VertexArrayVertexAttribDivisorEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexArrayVertexAttribDivisorEXT, cmd_size);
   cmd->vaobj = vaobj;
   cmd->index = index;
   cmd->divisor = divisor;
   if (COMPAT) _mesa_glthread_AttribDivisor(ctx, &vaobj, VERT_ATTRIB_GENERIC(index), divisor);
}


/* MapBufferRange: marshalled synchronously */
static GLvoid * GLAPIENTRY
_mesa_marshal_MapBufferRange(GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "MapBufferRange");
   return CALL_MapBufferRange(ctx->Dispatch.Current, (target, offset, length, access));
}


/* FlushMappedBufferRange: marshalled asynchronously */
struct marshal_cmd_FlushMappedBufferRange
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLintptr offset;
   GLsizeiptr length;
};
uint32_t
_mesa_unmarshal_FlushMappedBufferRange(struct gl_context *ctx, const struct marshal_cmd_FlushMappedBufferRange *restrict cmd)
{
   GLenum target = cmd->target;
   GLintptr offset = cmd->offset;
   GLsizeiptr length = cmd->length;
   CALL_FlushMappedBufferRange(ctx->Dispatch.Current, (target, offset, length));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_FlushMappedBufferRange), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_FlushMappedBufferRange(GLenum target, GLintptr offset, GLsizeiptr length)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_FlushMappedBufferRange);
   struct marshal_cmd_FlushMappedBufferRange *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_FlushMappedBufferRange, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->offset = offset;
   cmd->length = length;
}


/* TexBuffer: marshalled asynchronously */
struct marshal_cmd_TexBuffer
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLenum16 internalFormat;
   GLuint buffer;
};
uint32_t
_mesa_unmarshal_TexBuffer(struct gl_context *ctx, const struct marshal_cmd_TexBuffer *restrict cmd)
{
   GLenum target = cmd->target;
   GLenum internalFormat = cmd->internalFormat;
   GLuint buffer = cmd->buffer;
   CALL_TexBuffer(ctx->Dispatch.Current, (target, internalFormat, buffer));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_TexBuffer), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_TexBuffer(GLenum target, GLenum internalFormat, GLuint buffer)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TexBuffer);
   struct marshal_cmd_TexBuffer *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexBuffer, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->internalFormat = MIN2(internalFormat, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->buffer = buffer;
}


/* BindVertexArray: marshalled asynchronously */
struct marshal_cmd_BindVertexArray
{
   struct marshal_cmd_base cmd_base;
   GLuint array;
};
uint32_t
_mesa_unmarshal_BindVertexArray(struct gl_context *ctx, const struct marshal_cmd_BindVertexArray *restrict cmd)
{
   GLuint array = cmd->array;
   CALL_BindVertexArray(ctx->Dispatch.Current, (array));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_BindVertexArray), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_BindVertexArray(GLuint array)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_BindVertexArray);
   struct marshal_cmd_BindVertexArray *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_BindVertexArray, cmd_size);
   cmd->array = array;
   _mesa_glthread_BindVertexArray(ctx, array);
}


/* DeleteVertexArrays: marshalled asynchronously */
struct marshal_cmd_DeleteVertexArrays
{
   struct marshal_cmd_base cmd_base;
   GLsizei n;
   /* Next safe_mul(n, 1 * sizeof(GLuint)) bytes are GLuint arrays[n] */
};
uint32_t
_mesa_unmarshal_DeleteVertexArrays(struct gl_context *ctx, const struct marshal_cmd_DeleteVertexArrays *restrict cmd)
{
   GLsizei n = cmd->n;
   GLuint *arrays;
   const char *variable_data = (const char *) (cmd + 1);
   arrays = (GLuint *) variable_data;
   CALL_DeleteVertexArrays(ctx->Dispatch.Current, (n, arrays));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_DeleteVertexArrays(GLsizei n, const GLuint *arrays)
{
   GET_CURRENT_CONTEXT(ctx);
   int arrays_size = safe_mul(n, 1 * sizeof(GLuint));
   int cmd_size = sizeof(struct marshal_cmd_DeleteVertexArrays) + arrays_size;
   struct marshal_cmd_DeleteVertexArrays *cmd;
   if (unlikely(arrays_size < 0 || (arrays_size > 0 && !arrays) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "DeleteVertexArrays");
      CALL_DeleteVertexArrays(ctx->Dispatch.Current, (n, arrays));
      _mesa_glthread_DeleteVertexArrays(ctx, n, arrays);
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_DeleteVertexArrays, cmd_size);
   cmd->n = n;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, arrays, arrays_size);
   _mesa_glthread_DeleteVertexArrays(ctx, n, arrays);
}


/* GenVertexArrays: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GenVertexArrays(GLsizei n, GLuint *arrays)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GenVertexArrays");
   CALL_GenVertexArrays(ctx->Dispatch.Current, (n, arrays));
   _mesa_glthread_GenVertexArrays(ctx, n, arrays);
}


/* IsVertexArray: marshalled synchronously */
static GLboolean GLAPIENTRY
_mesa_marshal_IsVertexArray(GLuint array)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "IsVertexArray");
   return CALL_IsVertexArray(ctx->Dispatch.Current, (array));
}


/* GetUniformIndices: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetUniformIndices(GLuint program, GLsizei uniformCount, const GLchar * const *uniformNames, GLuint *uniformIndices)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetUniformIndices");
   CALL_GetUniformIndices(ctx->Dispatch.Current, (program, uniformCount, uniformNames, uniformIndices));
}


/* GetActiveUniformsiv: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetActiveUniformsiv(GLuint program, GLsizei uniformCount, const GLuint *uniformIndices, GLenum pname, GLint *params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetActiveUniformsiv");
   CALL_GetActiveUniformsiv(ctx->Dispatch.Current, (program, uniformCount, uniformIndices, pname, params));
}


/* GetActiveUniformName: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetActiveUniformName(GLuint program, GLuint uniformIndex, GLsizei bufSize, GLsizei *length, GLchar *uniformName)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetActiveUniformName");
   CALL_GetActiveUniformName(ctx->Dispatch.Current, (program, uniformIndex, bufSize, length, uniformName));
}


/* GetUniformBlockIndex: marshalled synchronously */
static GLuint GLAPIENTRY
_mesa_marshal_GetUniformBlockIndex(GLuint program, const GLchar *uniformBlockName)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetUniformBlockIndex");
   return CALL_GetUniformBlockIndex(ctx->Dispatch.Current, (program, uniformBlockName));
}


/* GetActiveUniformBlockiv: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetActiveUniformBlockiv(GLuint program, GLuint uniformBlockIndex, GLenum pname, GLint *params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetActiveUniformBlockiv");
   CALL_GetActiveUniformBlockiv(ctx->Dispatch.Current, (program, uniformBlockIndex, pname, params));
}


/* GetActiveUniformBlockName: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetActiveUniformBlockName(GLuint program, GLuint uniformBlockIndex, GLsizei bufSize, GLsizei *length, GLchar *uniformBlockName)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetActiveUniformBlockName");
   CALL_GetActiveUniformBlockName(ctx->Dispatch.Current, (program, uniformBlockIndex, bufSize, length, uniformBlockName));
}


/* UniformBlockBinding: marshalled asynchronously */
struct marshal_cmd_UniformBlockBinding
{
   struct marshal_cmd_base cmd_base;
   GLuint program;
   GLuint uniformBlockIndex;
   GLuint uniformBlockBinding;
};
uint32_t
_mesa_unmarshal_UniformBlockBinding(struct gl_context *ctx, const struct marshal_cmd_UniformBlockBinding *restrict cmd)
{
   GLuint program = cmd->program;
   GLuint uniformBlockIndex = cmd->uniformBlockIndex;
   GLuint uniformBlockBinding = cmd->uniformBlockBinding;
   CALL_UniformBlockBinding(ctx->Dispatch.Current, (program, uniformBlockIndex, uniformBlockBinding));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_UniformBlockBinding), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_UniformBlockBinding(GLuint program, GLuint uniformBlockIndex, GLuint uniformBlockBinding)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_UniformBlockBinding);
   struct marshal_cmd_UniformBlockBinding *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_UniformBlockBinding, cmd_size);
   cmd->program = program;
   cmd->uniformBlockIndex = uniformBlockIndex;
   cmd->uniformBlockBinding = uniformBlockBinding;
}


/* CopyBufferSubData: marshalled asynchronously */
struct marshal_cmd_CopyBufferSubData
{
   struct marshal_cmd_base cmd_base;
   GLenum16 readTarget;
   GLenum16 writeTarget;
   GLintptr readOffset;
   GLintptr writeOffset;
   GLsizeiptr size;
};
uint32_t
_mesa_unmarshal_CopyBufferSubData(struct gl_context *ctx, const struct marshal_cmd_CopyBufferSubData *restrict cmd)
{
   GLenum readTarget = cmd->readTarget;
   GLenum writeTarget = cmd->writeTarget;
   GLintptr readOffset = cmd->readOffset;
   GLintptr writeOffset = cmd->writeOffset;
   GLsizeiptr size = cmd->size;
   CALL_CopyBufferSubData(ctx->Dispatch.Current, (readTarget, writeTarget, readOffset, writeOffset, size));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_CopyBufferSubData), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_CopyBufferSubData(GLenum readTarget, GLenum writeTarget, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_CopyBufferSubData);
   struct marshal_cmd_CopyBufferSubData *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_CopyBufferSubData, cmd_size);
   cmd->readTarget = MIN2(readTarget, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->writeTarget = MIN2(writeTarget, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->readOffset = readOffset;
   cmd->writeOffset = writeOffset;
   cmd->size = size;
}


/* FenceSync: marshalled synchronously */
static GLsync GLAPIENTRY
_mesa_marshal_FenceSync(GLenum condition, GLbitfield flags)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "FenceSync");
   return CALL_FenceSync(ctx->Dispatch.Current, (condition, flags));
}


/* IsSync: marshalled synchronously */
static GLboolean GLAPIENTRY
_mesa_marshal_IsSync(GLsync sync)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "IsSync");
   return CALL_IsSync(ctx->Dispatch.Current, (sync));
}


/* DeleteSync: marshalled asynchronously */
struct marshal_cmd_DeleteSync
{
   struct marshal_cmd_base cmd_base;
   GLsync sync;
};
uint32_t
_mesa_unmarshal_DeleteSync(struct gl_context *ctx, const struct marshal_cmd_DeleteSync *restrict cmd)
{
   GLsync sync = cmd->sync;
   CALL_DeleteSync(ctx->Dispatch.Current, (sync));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_DeleteSync), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_DeleteSync(GLsync sync)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_DeleteSync);
   struct marshal_cmd_DeleteSync *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_DeleteSync, cmd_size);
   cmd->sync = sync;
}


/* ClientWaitSync: marshalled synchronously */
static GLenum GLAPIENTRY
_mesa_marshal_ClientWaitSync(GLsync sync, GLbitfield flags, GLuint64 timeout)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "ClientWaitSync");
   return CALL_ClientWaitSync(ctx->Dispatch.Current, (sync, flags, timeout));
}


/* WaitSync: marshalled asynchronously */
struct marshal_cmd_WaitSync
{
   struct marshal_cmd_base cmd_base;
   GLbitfield flags;
   GLsync sync;
   GLuint64 timeout;
};
uint32_t
_mesa_unmarshal_WaitSync(struct gl_context *ctx, const struct marshal_cmd_WaitSync *restrict cmd)
{
   GLsync sync = cmd->sync;
   GLbitfield flags = cmd->flags;
   GLuint64 timeout = cmd->timeout;
   CALL_WaitSync(ctx->Dispatch.Current, (sync, flags, timeout));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_WaitSync), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_WaitSync(GLsync sync, GLbitfield flags, GLuint64 timeout)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_WaitSync);
   struct marshal_cmd_WaitSync *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_WaitSync, cmd_size);
   cmd->sync = sync;
   cmd->flags = flags;
   cmd->timeout = timeout;
}


/* GetInteger64v: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetInteger64v(GLenum pname, GLint64 *params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetInteger64v");
   CALL_GetInteger64v(ctx->Dispatch.Current, (pname, params));
}


/* GetSynciv: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetSynciv(GLsync sync, GLenum pname, GLsizei bufSize, GLsizei *length, GLint *values)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetSynciv");
   CALL_GetSynciv(ctx->Dispatch.Current, (sync, pname, bufSize, length, values));
}


/* TexImage2DMultisample: marshalled asynchronously */
struct marshal_cmd_TexImage2DMultisample
{
   struct marshal_cmd_base cmd_base;
   GLboolean fixedsamplelocations;
   GLenum16 target;
   GLenum16 internalformat;
   GLsizei samples;
   GLsizei width;
   GLsizei height;
};
uint32_t
_mesa_unmarshal_TexImage2DMultisample(struct gl_context *ctx, const struct marshal_cmd_TexImage2DMultisample *restrict cmd)
{
   GLenum target = cmd->target;
   GLsizei samples = cmd->samples;
   GLenum internalformat = cmd->internalformat;
   GLsizei width = cmd->width;
   GLsizei height = cmd->height;
   GLboolean fixedsamplelocations = cmd->fixedsamplelocations;
   CALL_TexImage2DMultisample(ctx->Dispatch.Current, (target, samples, internalformat, width, height, fixedsamplelocations));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_TexImage2DMultisample), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_TexImage2DMultisample(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TexImage2DMultisample);
   struct marshal_cmd_TexImage2DMultisample *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexImage2DMultisample, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->samples = samples;
   cmd->internalformat = MIN2(internalformat, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->width = width;
   cmd->height = height;
   cmd->fixedsamplelocations = fixedsamplelocations;
}


/* TexImage3DMultisample: marshalled asynchronously */
struct marshal_cmd_TexImage3DMultisample
{
   struct marshal_cmd_base cmd_base;
   GLboolean fixedsamplelocations;
   GLenum16 target;
   GLenum16 internalformat;
   GLsizei samples;
   GLsizei width;
   GLsizei height;
   GLsizei depth;
};
uint32_t
_mesa_unmarshal_TexImage3DMultisample(struct gl_context *ctx, const struct marshal_cmd_TexImage3DMultisample *restrict cmd)
{
   GLenum target = cmd->target;
   GLsizei samples = cmd->samples;
   GLenum internalformat = cmd->internalformat;
   GLsizei width = cmd->width;
   GLsizei height = cmd->height;
   GLsizei depth = cmd->depth;
   GLboolean fixedsamplelocations = cmd->fixedsamplelocations;
   CALL_TexImage3DMultisample(ctx->Dispatch.Current, (target, samples, internalformat, width, height, depth, fixedsamplelocations));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_TexImage3DMultisample), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_TexImage3DMultisample(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TexImage3DMultisample);
   struct marshal_cmd_TexImage3DMultisample *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexImage3DMultisample, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->samples = samples;
   cmd->internalformat = MIN2(internalformat, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->width = width;
   cmd->height = height;
   cmd->depth = depth;
   cmd->fixedsamplelocations = fixedsamplelocations;
}


/* GetMultisamplefv: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetMultisamplefv(GLenum pname, GLuint index, GLfloat *val)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetMultisamplefv");
   CALL_GetMultisamplefv(ctx->Dispatch.Current, (pname, index, val));
}


/* SampleMaski: marshalled asynchronously */
struct marshal_cmd_SampleMaski
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLbitfield mask;
};
uint32_t
_mesa_unmarshal_SampleMaski(struct gl_context *ctx, const struct marshal_cmd_SampleMaski *restrict cmd)
{
   GLuint index = cmd->index;
   GLbitfield mask = cmd->mask;
   CALL_SampleMaski(ctx->Dispatch.Current, (index, mask));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_SampleMaski), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_SampleMaski(GLuint index, GLbitfield mask)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_SampleMaski);
   struct marshal_cmd_SampleMaski *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_SampleMaski, cmd_size);
   cmd->index = index;
   cmd->mask = mask;
}


/* BlendEquationiARB: marshalled asynchronously */
struct marshal_cmd_BlendEquationiARB
{
   struct marshal_cmd_base cmd_base;
   GLenum16 mode;
   GLuint buf;
};
uint32_t
_mesa_unmarshal_BlendEquationiARB(struct gl_context *ctx, const struct marshal_cmd_BlendEquationiARB *restrict cmd)
{
   GLuint buf = cmd->buf;
   GLenum mode = cmd->mode;
   CALL_BlendEquationiARB(ctx->Dispatch.Current, (buf, mode));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_BlendEquationiARB), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_BlendEquationiARB(GLuint buf, GLenum mode)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_BlendEquationiARB);
   struct marshal_cmd_BlendEquationiARB *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_BlendEquationiARB, cmd_size);
   cmd->buf = buf;
   cmd->mode = MIN2(mode, 0xffff); /* clamped to 0xffff (invalid enum) */
}


/* BlendEquationSeparateiARB: marshalled asynchronously */
struct marshal_cmd_BlendEquationSeparateiARB
{
   struct marshal_cmd_base cmd_base;
   GLenum16 modeRGB;
   GLenum16 modeA;
   GLuint buf;
};
uint32_t
_mesa_unmarshal_BlendEquationSeparateiARB(struct gl_context *ctx, const struct marshal_cmd_BlendEquationSeparateiARB *restrict cmd)
{
   GLuint buf = cmd->buf;
   GLenum modeRGB = cmd->modeRGB;
   GLenum modeA = cmd->modeA;
   CALL_BlendEquationSeparateiARB(ctx->Dispatch.Current, (buf, modeRGB, modeA));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_BlendEquationSeparateiARB), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_BlendEquationSeparateiARB(GLuint buf, GLenum modeRGB, GLenum modeA)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_BlendEquationSeparateiARB);
   struct marshal_cmd_BlendEquationSeparateiARB *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_BlendEquationSeparateiARB, cmd_size);
   cmd->buf = buf;
   cmd->modeRGB = MIN2(modeRGB, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->modeA = MIN2(modeA, 0xffff); /* clamped to 0xffff (invalid enum) */
}


/* BlendFunciARB: marshalled asynchronously */
struct marshal_cmd_BlendFunciARB
{
   struct marshal_cmd_base cmd_base;
   GLenum16 src;
   GLenum16 dst;
   GLuint buf;
};
uint32_t
_mesa_unmarshal_BlendFunciARB(struct gl_context *ctx, const struct marshal_cmd_BlendFunciARB *restrict cmd)
{
   GLuint buf = cmd->buf;
   GLenum src = cmd->src;
   GLenum dst = cmd->dst;
   CALL_BlendFunciARB(ctx->Dispatch.Current, (buf, src, dst));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_BlendFunciARB), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_BlendFunciARB(GLuint buf, GLenum src, GLenum dst)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_BlendFunciARB);
   struct marshal_cmd_BlendFunciARB *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_BlendFunciARB, cmd_size);
   cmd->buf = buf;
   cmd->src = MIN2(src, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->dst = MIN2(dst, 0xffff); /* clamped to 0xffff (invalid enum) */
}


/* BlendFuncSeparateiARB: marshalled asynchronously */
struct marshal_cmd_BlendFuncSeparateiARB
{
   struct marshal_cmd_base cmd_base;
   GLenum16 srcRGB;
   GLenum16 dstRGB;
   GLenum16 srcA;
   GLenum16 dstA;
   GLuint buf;
};
uint32_t
_mesa_unmarshal_BlendFuncSeparateiARB(struct gl_context *ctx, const struct marshal_cmd_BlendFuncSeparateiARB *restrict cmd)
{
   GLuint buf = cmd->buf;
   GLenum srcRGB = cmd->srcRGB;
   GLenum dstRGB = cmd->dstRGB;
   GLenum srcA = cmd->srcA;
   GLenum dstA = cmd->dstA;
   CALL_BlendFuncSeparateiARB(ctx->Dispatch.Current, (buf, srcRGB, dstRGB, srcA, dstA));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_BlendFuncSeparateiARB), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_BlendFuncSeparateiARB(GLuint buf, GLenum srcRGB, GLenum dstRGB, GLenum srcA, GLenum dstA)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_BlendFuncSeparateiARB);
   struct marshal_cmd_BlendFuncSeparateiARB *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_BlendFuncSeparateiARB, cmd_size);
   cmd->buf = buf;
   cmd->srcRGB = MIN2(srcRGB, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->dstRGB = MIN2(dstRGB, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->srcA = MIN2(srcA, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->dstA = MIN2(dstA, 0xffff); /* clamped to 0xffff (invalid enum) */
}


/* MinSampleShading: marshalled asynchronously */
struct marshal_cmd_MinSampleShading
{
   struct marshal_cmd_base cmd_base;
   GLfloat value;
};
uint32_t
_mesa_unmarshal_MinSampleShading(struct gl_context *ctx, const struct marshal_cmd_MinSampleShading *restrict cmd)
{
   GLfloat value = cmd->value;
   CALL_MinSampleShading(ctx->Dispatch.Current, (value));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MinSampleShading), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_MinSampleShading(GLfloat value)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MinSampleShading);
   struct marshal_cmd_MinSampleShading *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MinSampleShading, cmd_size);
   cmd->value = value;
}


/* NamedStringARB: marshalled asynchronously */
struct marshal_cmd_NamedStringARB
{
   struct marshal_cmd_base cmd_base;
   GLenum16 type;
   GLint namelen;
   GLint stringlen;
   /* Next namelen bytes are GLchar name[namelen] */
   /* Next stringlen bytes are GLchar string[stringlen] */
};
uint32_t
_mesa_unmarshal_NamedStringARB(struct gl_context *ctx, const struct marshal_cmd_NamedStringARB *restrict cmd)
{
   GLenum type = cmd->type;
   GLint namelen = cmd->namelen;
   GLint stringlen = cmd->stringlen;
   GLchar *name;
   GLchar *string;
   const char *variable_data = (const char *) (cmd + 1);
   name = (GLchar *) variable_data;
   variable_data += namelen;
   string = (GLchar *) variable_data;
   CALL_NamedStringARB(ctx->Dispatch.Current, (type, namelen, name, stringlen, string));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_NamedStringARB(GLenum type, GLint namelen, const GLchar *name, GLint stringlen, const GLchar *string)
{
   GET_CURRENT_CONTEXT(ctx);
   int name_size = namelen;
   int string_size = stringlen;
   int cmd_size = sizeof(struct marshal_cmd_NamedStringARB) + name_size + string_size;
   struct marshal_cmd_NamedStringARB *cmd;
   if (unlikely(name_size < 0 || (name_size > 0 && !name) || string_size < 0 || (string_size > 0 && !string) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "NamedStringARB");
      CALL_NamedStringARB(ctx->Dispatch.Current, (type, namelen, name, stringlen, string));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_NamedStringARB, cmd_size);
   cmd->type = MIN2(type, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->namelen = namelen;
   cmd->stringlen = stringlen;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, name, name_size);
   variable_data += name_size;
   memcpy(variable_data, string, string_size);
}


/* DeleteNamedStringARB: marshalled asynchronously */
struct marshal_cmd_DeleteNamedStringARB
{
   struct marshal_cmd_base cmd_base;
   GLint namelen;
   /* Next namelen bytes are GLchar name[namelen] */
};
uint32_t
_mesa_unmarshal_DeleteNamedStringARB(struct gl_context *ctx, const struct marshal_cmd_DeleteNamedStringARB *restrict cmd)
{
   GLint namelen = cmd->namelen;
   GLchar *name;
   const char *variable_data = (const char *) (cmd + 1);
   name = (GLchar *) variable_data;
   CALL_DeleteNamedStringARB(ctx->Dispatch.Current, (namelen, name));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_DeleteNamedStringARB(GLint namelen, const GLchar *name)
{
   GET_CURRENT_CONTEXT(ctx);
   int name_size = namelen;
   int cmd_size = sizeof(struct marshal_cmd_DeleteNamedStringARB) + name_size;
   struct marshal_cmd_DeleteNamedStringARB *cmd;
   if (unlikely(name_size < 0 || (name_size > 0 && !name) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "DeleteNamedStringARB");
      CALL_DeleteNamedStringARB(ctx->Dispatch.Current, (namelen, name));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_DeleteNamedStringARB, cmd_size);
   cmd->namelen = namelen;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, name, name_size);
}


/* CompileShaderIncludeARB: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_CompileShaderIncludeARB(GLuint shader, GLsizei count, const GLchar * const *path, const GLint *length)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "CompileShaderIncludeARB");
   CALL_CompileShaderIncludeARB(ctx->Dispatch.Current, (shader, count, path, length));
}


/* IsNamedStringARB: marshalled synchronously */
static GLboolean GLAPIENTRY
_mesa_marshal_IsNamedStringARB(GLint namelen, const GLchar *name)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "IsNamedStringARB");
   return CALL_IsNamedStringARB(ctx->Dispatch.Current, (namelen, name));
}


/* GetNamedStringARB: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetNamedStringARB(GLint namelen, const GLchar *name, GLsizei bufSize, GLint *stringlen, GLchar *string)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetNamedStringARB");
   CALL_GetNamedStringARB(ctx->Dispatch.Current, (namelen, name, bufSize, stringlen, string));
}


/* GetNamedStringivARB: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetNamedStringivARB(GLint namelen, const GLchar *name, GLenum pname, GLint *params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetNamedStringivARB");
   CALL_GetNamedStringivARB(ctx->Dispatch.Current, (namelen, name, pname, params));
}


/* BindFragDataLocationIndexed: marshalled asynchronously */
struct marshal_cmd_BindFragDataLocationIndexed
{
   struct marshal_cmd_base cmd_base;
   GLuint program;
   GLuint colorNumber;
   GLuint index;
   /* Next (strlen(name) + 1) bytes are GLchar name[(strlen(name) + 1)] */
};
uint32_t
_mesa_unmarshal_BindFragDataLocationIndexed(struct gl_context *ctx, const struct marshal_cmd_BindFragDataLocationIndexed *restrict cmd)
{
   GLuint program = cmd->program;
   GLuint colorNumber = cmd->colorNumber;
   GLuint index = cmd->index;
   GLchar *name;
   const char *variable_data = (const char *) (cmd + 1);
   name = (GLchar *) variable_data;
   CALL_BindFragDataLocationIndexed(ctx->Dispatch.Current, (program, colorNumber, index, name));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_BindFragDataLocationIndexed(GLuint program, GLuint colorNumber, GLuint index, const GLchar *name)
{
   GET_CURRENT_CONTEXT(ctx);
   int name_size = (strlen(name) + 1);
   int cmd_size = sizeof(struct marshal_cmd_BindFragDataLocationIndexed) + name_size;
   struct marshal_cmd_BindFragDataLocationIndexed *cmd;
   if (unlikely(name_size < 0 || (name_size > 0 && !name) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "BindFragDataLocationIndexed");
      CALL_BindFragDataLocationIndexed(ctx->Dispatch.Current, (program, colorNumber, index, name));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_BindFragDataLocationIndexed, cmd_size);
   cmd->program = program;
   cmd->colorNumber = colorNumber;
   cmd->index = index;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, name, name_size);
}


/* GetFragDataIndex: marshalled synchronously */
static GLint GLAPIENTRY
_mesa_marshal_GetFragDataIndex(GLuint program, const GLchar *name)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetFragDataIndex");
   return CALL_GetFragDataIndex(ctx->Dispatch.Current, (program, name));
}


/* GenSamplers: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GenSamplers(GLsizei count, GLuint *samplers)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GenSamplers");
   CALL_GenSamplers(ctx->Dispatch.Current, (count, samplers));
}


/* DeleteSamplers: marshalled asynchronously */
struct marshal_cmd_DeleteSamplers
{
   struct marshal_cmd_base cmd_base;
   GLsizei count;
   /* Next safe_mul(count, 1 * sizeof(GLuint)) bytes are GLuint samplers[count] */
};
uint32_t
_mesa_unmarshal_DeleteSamplers(struct gl_context *ctx, const struct marshal_cmd_DeleteSamplers *restrict cmd)
{
   GLsizei count = cmd->count;
   GLuint *samplers;
   const char *variable_data = (const char *) (cmd + 1);
   samplers = (GLuint *) variable_data;
   CALL_DeleteSamplers(ctx->Dispatch.Current, (count, samplers));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_DeleteSamplers(GLsizei count, const GLuint *samplers)
{
   GET_CURRENT_CONTEXT(ctx);
   int samplers_size = safe_mul(count, 1 * sizeof(GLuint));
   int cmd_size = sizeof(struct marshal_cmd_DeleteSamplers) + samplers_size;
   struct marshal_cmd_DeleteSamplers *cmd;
   if (unlikely(samplers_size < 0 || (samplers_size > 0 && !samplers) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "DeleteSamplers");
      CALL_DeleteSamplers(ctx->Dispatch.Current, (count, samplers));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_DeleteSamplers, cmd_size);
   cmd->count = count;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, samplers, samplers_size);
}


/* IsSampler: marshalled synchronously */
static GLboolean GLAPIENTRY
_mesa_marshal_IsSampler(GLuint sampler)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "IsSampler");
   return CALL_IsSampler(ctx->Dispatch.Current, (sampler));
}


/* BindSampler: marshalled asynchronously */
struct marshal_cmd_BindSampler
{
   struct marshal_cmd_base cmd_base;
   GLuint unit;
   GLuint sampler;
};
uint32_t
_mesa_unmarshal_BindSampler(struct gl_context *ctx, const struct marshal_cmd_BindSampler *restrict cmd)
{
   GLuint unit = cmd->unit;
   GLuint sampler = cmd->sampler;
   CALL_BindSampler(ctx->Dispatch.Current, (unit, sampler));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_BindSampler), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_BindSampler(GLuint unit, GLuint sampler)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_BindSampler);
   struct marshal_cmd_BindSampler *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_BindSampler, cmd_size);
   cmd->unit = unit;
   cmd->sampler = sampler;
}


/* SamplerParameteri: marshalled asynchronously */
struct marshal_cmd_SamplerParameteri
{
   struct marshal_cmd_base cmd_base;
   GLenum16 pname;
   GLuint sampler;
   GLint param;
};
uint32_t
_mesa_unmarshal_SamplerParameteri(struct gl_context *ctx, const struct marshal_cmd_SamplerParameteri *restrict cmd)
{
   GLuint sampler = cmd->sampler;
   GLenum pname = cmd->pname;
   GLint param = cmd->param;
   CALL_SamplerParameteri(ctx->Dispatch.Current, (sampler, pname, param));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_SamplerParameteri), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_SamplerParameteri(GLuint sampler, GLenum pname, GLint param)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_SamplerParameteri);
   struct marshal_cmd_SamplerParameteri *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_SamplerParameteri, cmd_size);
   cmd->sampler = sampler;
   cmd->pname = MIN2(pname, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->param = param;
}


/* SamplerParameterf: marshalled asynchronously */
struct marshal_cmd_SamplerParameterf
{
   struct marshal_cmd_base cmd_base;
   GLenum16 pname;
   GLuint sampler;
   GLfloat param;
};
uint32_t
_mesa_unmarshal_SamplerParameterf(struct gl_context *ctx, const struct marshal_cmd_SamplerParameterf *restrict cmd)
{
   GLuint sampler = cmd->sampler;
   GLenum pname = cmd->pname;
   GLfloat param = cmd->param;
   CALL_SamplerParameterf(ctx->Dispatch.Current, (sampler, pname, param));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_SamplerParameterf), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_SamplerParameterf(GLuint sampler, GLenum pname, GLfloat param)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_SamplerParameterf);
   struct marshal_cmd_SamplerParameterf *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_SamplerParameterf, cmd_size);
   cmd->sampler = sampler;
   cmd->pname = MIN2(pname, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->param = param;
}


/* SamplerParameteriv: marshalled asynchronously */
struct marshal_cmd_SamplerParameteriv
{
   struct marshal_cmd_base cmd_base;
   GLenum16 pname;
   GLuint sampler;
   /* Next safe_mul(_mesa_tex_param_enum_to_count(pname), 1 * sizeof(GLint)) bytes are GLint params[_mesa_tex_param_enum_to_count(pname)] */
};
uint32_t
_mesa_unmarshal_SamplerParameteriv(struct gl_context *ctx, const struct marshal_cmd_SamplerParameteriv *restrict cmd)
{
   GLuint sampler = cmd->sampler;
   GLenum pname = cmd->pname;
   GLint *params;
   const char *variable_data = (const char *) (cmd + 1);
   params = (GLint *) variable_data;
   CALL_SamplerParameteriv(ctx->Dispatch.Current, (sampler, pname, params));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_SamplerParameteriv(GLuint sampler, GLenum pname, const GLint *params)
{
   GET_CURRENT_CONTEXT(ctx);
   int params_size = safe_mul(_mesa_tex_param_enum_to_count(pname), 1 * sizeof(GLint));
   int cmd_size = sizeof(struct marshal_cmd_SamplerParameteriv) + params_size;
   struct marshal_cmd_SamplerParameteriv *cmd;
   if (unlikely(params_size < 0 || (params_size > 0 && !params) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "SamplerParameteriv");
      CALL_SamplerParameteriv(ctx->Dispatch.Current, (sampler, pname, params));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_SamplerParameteriv, cmd_size);
   cmd->sampler = sampler;
   cmd->pname = MIN2(pname, 0xffff); /* clamped to 0xffff (invalid enum) */
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, params, params_size);
}


/* SamplerParameterfv: marshalled asynchronously */
struct marshal_cmd_SamplerParameterfv
{
   struct marshal_cmd_base cmd_base;
   GLenum16 pname;
   GLuint sampler;
   /* Next safe_mul(_mesa_tex_param_enum_to_count(pname), 1 * sizeof(GLfloat)) bytes are GLfloat params[_mesa_tex_param_enum_to_count(pname)] */
};
uint32_t
_mesa_unmarshal_SamplerParameterfv(struct gl_context *ctx, const struct marshal_cmd_SamplerParameterfv *restrict cmd)
{
   GLuint sampler = cmd->sampler;
   GLenum pname = cmd->pname;
   GLfloat *params;
   const char *variable_data = (const char *) (cmd + 1);
   params = (GLfloat *) variable_data;
   CALL_SamplerParameterfv(ctx->Dispatch.Current, (sampler, pname, params));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_SamplerParameterfv(GLuint sampler, GLenum pname, const GLfloat *params)
{
   GET_CURRENT_CONTEXT(ctx);
   int params_size = safe_mul(_mesa_tex_param_enum_to_count(pname), 1 * sizeof(GLfloat));
   int cmd_size = sizeof(struct marshal_cmd_SamplerParameterfv) + params_size;
   struct marshal_cmd_SamplerParameterfv *cmd;
   if (unlikely(params_size < 0 || (params_size > 0 && !params) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "SamplerParameterfv");
      CALL_SamplerParameterfv(ctx->Dispatch.Current, (sampler, pname, params));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_SamplerParameterfv, cmd_size);
   cmd->sampler = sampler;
   cmd->pname = MIN2(pname, 0xffff); /* clamped to 0xffff (invalid enum) */
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, params, params_size);
}


/* SamplerParameterIiv: marshalled asynchronously */
struct marshal_cmd_SamplerParameterIiv
{
   struct marshal_cmd_base cmd_base;
   GLenum16 pname;
   GLuint sampler;
   /* Next safe_mul(_mesa_tex_param_enum_to_count(pname), 1 * sizeof(GLint)) bytes are GLint params[_mesa_tex_param_enum_to_count(pname)] */
};
uint32_t
_mesa_unmarshal_SamplerParameterIiv(struct gl_context *ctx, const struct marshal_cmd_SamplerParameterIiv *restrict cmd)
{
   GLuint sampler = cmd->sampler;
   GLenum pname = cmd->pname;
   GLint *params;
   const char *variable_data = (const char *) (cmd + 1);
   params = (GLint *) variable_data;
   CALL_SamplerParameterIiv(ctx->Dispatch.Current, (sampler, pname, params));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_SamplerParameterIiv(GLuint sampler, GLenum pname, const GLint *params)
{
   GET_CURRENT_CONTEXT(ctx);
   int params_size = safe_mul(_mesa_tex_param_enum_to_count(pname), 1 * sizeof(GLint));
   int cmd_size = sizeof(struct marshal_cmd_SamplerParameterIiv) + params_size;
   struct marshal_cmd_SamplerParameterIiv *cmd;
   if (unlikely(params_size < 0 || (params_size > 0 && !params) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "SamplerParameterIiv");
      CALL_SamplerParameterIiv(ctx->Dispatch.Current, (sampler, pname, params));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_SamplerParameterIiv, cmd_size);
   cmd->sampler = sampler;
   cmd->pname = MIN2(pname, 0xffff); /* clamped to 0xffff (invalid enum) */
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, params, params_size);
}


/* SamplerParameterIuiv: marshalled asynchronously */
struct marshal_cmd_SamplerParameterIuiv
{
   struct marshal_cmd_base cmd_base;
   GLenum16 pname;
   GLuint sampler;
   /* Next safe_mul(_mesa_tex_param_enum_to_count(pname), 1 * sizeof(GLuint)) bytes are GLuint params[_mesa_tex_param_enum_to_count(pname)] */
};
uint32_t
_mesa_unmarshal_SamplerParameterIuiv(struct gl_context *ctx, const struct marshal_cmd_SamplerParameterIuiv *restrict cmd)
{
   GLuint sampler = cmd->sampler;
   GLenum pname = cmd->pname;
   GLuint *params;
   const char *variable_data = (const char *) (cmd + 1);
   params = (GLuint *) variable_data;
   CALL_SamplerParameterIuiv(ctx->Dispatch.Current, (sampler, pname, params));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_SamplerParameterIuiv(GLuint sampler, GLenum pname, const GLuint *params)
{
   GET_CURRENT_CONTEXT(ctx);
   int params_size = safe_mul(_mesa_tex_param_enum_to_count(pname), 1 * sizeof(GLuint));
   int cmd_size = sizeof(struct marshal_cmd_SamplerParameterIuiv) + params_size;
   struct marshal_cmd_SamplerParameterIuiv *cmd;
   if (unlikely(params_size < 0 || (params_size > 0 && !params) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "SamplerParameterIuiv");
      CALL_SamplerParameterIuiv(ctx->Dispatch.Current, (sampler, pname, params));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_SamplerParameterIuiv, cmd_size);
   cmd->sampler = sampler;
   cmd->pname = MIN2(pname, 0xffff); /* clamped to 0xffff (invalid enum) */
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, params, params_size);
}


/* GetSamplerParameteriv: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetSamplerParameteriv(GLuint sampler, GLenum pname, GLint *params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetSamplerParameteriv");
   CALL_GetSamplerParameteriv(ctx->Dispatch.Current, (sampler, pname, params));
}


/* GetSamplerParameterfv: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetSamplerParameterfv(GLuint sampler, GLenum pname, GLfloat *params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetSamplerParameterfv");
   CALL_GetSamplerParameterfv(ctx->Dispatch.Current, (sampler, pname, params));
}


/* GetSamplerParameterIiv: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetSamplerParameterIiv(GLuint sampler, GLenum pname, GLint *params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetSamplerParameterIiv");
   CALL_GetSamplerParameterIiv(ctx->Dispatch.Current, (sampler, pname, params));
}


/* GetSamplerParameterIuiv: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetSamplerParameterIuiv(GLuint sampler, GLenum pname, GLuint *params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetSamplerParameterIuiv");
   CALL_GetSamplerParameterIuiv(ctx->Dispatch.Current, (sampler, pname, params));
}


/* GetQueryObjecti64v: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetQueryObjecti64v(GLuint id, GLenum pname, GLint64 *params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetQueryObjecti64v");
   CALL_GetQueryObjecti64v(ctx->Dispatch.Current, (id, pname, params));
}


/* GetQueryObjectui64v: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetQueryObjectui64v(GLuint id, GLenum pname, GLuint64 *params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetQueryObjectui64v");
   CALL_GetQueryObjectui64v(ctx->Dispatch.Current, (id, pname, params));
}


/* QueryCounter: marshalled asynchronously */
struct marshal_cmd_QueryCounter
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLuint id;
};
uint32_t
_mesa_unmarshal_QueryCounter(struct gl_context *ctx, const struct marshal_cmd_QueryCounter *restrict cmd)
{
   GLuint id = cmd->id;
   GLenum target = cmd->target;
   CALL_QueryCounter(ctx->Dispatch.Current, (id, target));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_QueryCounter), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_QueryCounter(GLuint id, GLenum target)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_QueryCounter);
   struct marshal_cmd_QueryCounter *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_QueryCounter, cmd_size);
   cmd->id = id;
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
}


/* VertexP2ui: marshalled asynchronously */
struct marshal_cmd_VertexP2ui
{
   struct marshal_cmd_base cmd_base;
   GLenum16 type;
   GLuint value;
};
uint32_t
_mesa_unmarshal_VertexP2ui(struct gl_context *ctx, const struct marshal_cmd_VertexP2ui *restrict cmd)
{
   GLenum type = cmd->type;
   GLuint value = cmd->value;
   CALL_VertexP2ui(ctx->Dispatch.Current, (type, value));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexP2ui), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_VertexP2ui(GLenum type, GLuint value)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexP2ui);
   struct marshal_cmd_VertexP2ui *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexP2ui, cmd_size);
   cmd->type = MIN2(type, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->value = value;
}


/* VertexP3ui: marshalled asynchronously */
struct marshal_cmd_VertexP3ui
{
   struct marshal_cmd_base cmd_base;
   GLenum16 type;
   GLuint value;
};
uint32_t
_mesa_unmarshal_VertexP3ui(struct gl_context *ctx, const struct marshal_cmd_VertexP3ui *restrict cmd)
{
   GLenum type = cmd->type;
   GLuint value = cmd->value;
   CALL_VertexP3ui(ctx->Dispatch.Current, (type, value));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexP3ui), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_VertexP3ui(GLenum type, GLuint value)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexP3ui);
   struct marshal_cmd_VertexP3ui *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexP3ui, cmd_size);
   cmd->type = MIN2(type, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->value = value;
}


/* VertexP4ui: marshalled asynchronously */
struct marshal_cmd_VertexP4ui
{
   struct marshal_cmd_base cmd_base;
   GLenum16 type;
   GLuint value;
};
uint32_t
_mesa_unmarshal_VertexP4ui(struct gl_context *ctx, const struct marshal_cmd_VertexP4ui *restrict cmd)
{
   GLenum type = cmd->type;
   GLuint value = cmd->value;
   CALL_VertexP4ui(ctx->Dispatch.Current, (type, value));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexP4ui), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_VertexP4ui(GLenum type, GLuint value)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexP4ui);
   struct marshal_cmd_VertexP4ui *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexP4ui, cmd_size);
   cmd->type = MIN2(type, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->value = value;
}


/* VertexP2uiv: marshalled asynchronously */
struct marshal_cmd_VertexP2uiv
{
   struct marshal_cmd_base cmd_base;
   GLenum16 type;
   GLuint value[1];
};
uint32_t
_mesa_unmarshal_VertexP2uiv(struct gl_context *ctx, const struct marshal_cmd_VertexP2uiv *restrict cmd)
{
   GLenum type = cmd->type;
   const GLuint *value = cmd->value;
   CALL_VertexP2uiv(ctx->Dispatch.Current, (type, value));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexP2uiv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_VertexP2uiv(GLenum type, const GLuint *value)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexP2uiv);
   struct marshal_cmd_VertexP2uiv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexP2uiv, cmd_size);
   cmd->type = MIN2(type, 0xffff); /* clamped to 0xffff (invalid enum) */
   memcpy(cmd->value, value, 1 * sizeof(GLuint));
}


/* VertexP3uiv: marshalled asynchronously */
struct marshal_cmd_VertexP3uiv
{
   struct marshal_cmd_base cmd_base;
   GLenum16 type;
   GLuint value[1];
};
uint32_t
_mesa_unmarshal_VertexP3uiv(struct gl_context *ctx, const struct marshal_cmd_VertexP3uiv *restrict cmd)
{
   GLenum type = cmd->type;
   const GLuint *value = cmd->value;
   CALL_VertexP3uiv(ctx->Dispatch.Current, (type, value));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexP3uiv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_VertexP3uiv(GLenum type, const GLuint *value)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexP3uiv);
   struct marshal_cmd_VertexP3uiv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexP3uiv, cmd_size);
   cmd->type = MIN2(type, 0xffff); /* clamped to 0xffff (invalid enum) */
   memcpy(cmd->value, value, 1 * sizeof(GLuint));
}


/* VertexP4uiv: marshalled asynchronously */
struct marshal_cmd_VertexP4uiv
{
   struct marshal_cmd_base cmd_base;
   GLenum16 type;
   GLuint value[1];
};
uint32_t
_mesa_unmarshal_VertexP4uiv(struct gl_context *ctx, const struct marshal_cmd_VertexP4uiv *restrict cmd)
{
   GLenum type = cmd->type;
   const GLuint *value = cmd->value;
   CALL_VertexP4uiv(ctx->Dispatch.Current, (type, value));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexP4uiv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_VertexP4uiv(GLenum type, const GLuint *value)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexP4uiv);
   struct marshal_cmd_VertexP4uiv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexP4uiv, cmd_size);
   cmd->type = MIN2(type, 0xffff); /* clamped to 0xffff (invalid enum) */
   memcpy(cmd->value, value, 1 * sizeof(GLuint));
}


/* TexCoordP1ui: marshalled asynchronously */
struct marshal_cmd_TexCoordP1ui
{
   struct marshal_cmd_base cmd_base;
   GLenum16 type;
   GLuint coords;
};
uint32_t
_mesa_unmarshal_TexCoordP1ui(struct gl_context *ctx, const struct marshal_cmd_TexCoordP1ui *restrict cmd)
{
   GLenum type = cmd->type;
   GLuint coords = cmd->coords;
   CALL_TexCoordP1ui(ctx->Dispatch.Current, (type, coords));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_TexCoordP1ui), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_TexCoordP1ui(GLenum type, GLuint coords)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TexCoordP1ui);
   struct marshal_cmd_TexCoordP1ui *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexCoordP1ui, cmd_size);
   cmd->type = MIN2(type, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->coords = coords;
}


/* TexCoordP2ui: marshalled asynchronously */
struct marshal_cmd_TexCoordP2ui
{
   struct marshal_cmd_base cmd_base;
   GLenum16 type;
   GLuint coords;
};
uint32_t
_mesa_unmarshal_TexCoordP2ui(struct gl_context *ctx, const struct marshal_cmd_TexCoordP2ui *restrict cmd)
{
   GLenum type = cmd->type;
   GLuint coords = cmd->coords;
   CALL_TexCoordP2ui(ctx->Dispatch.Current, (type, coords));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_TexCoordP2ui), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_TexCoordP2ui(GLenum type, GLuint coords)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TexCoordP2ui);
   struct marshal_cmd_TexCoordP2ui *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexCoordP2ui, cmd_size);
   cmd->type = MIN2(type, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->coords = coords;
}


/* TexCoordP3ui: marshalled asynchronously */
struct marshal_cmd_TexCoordP3ui
{
   struct marshal_cmd_base cmd_base;
   GLenum16 type;
   GLuint coords;
};
uint32_t
_mesa_unmarshal_TexCoordP3ui(struct gl_context *ctx, const struct marshal_cmd_TexCoordP3ui *restrict cmd)
{
   GLenum type = cmd->type;
   GLuint coords = cmd->coords;
   CALL_TexCoordP3ui(ctx->Dispatch.Current, (type, coords));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_TexCoordP3ui), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_TexCoordP3ui(GLenum type, GLuint coords)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TexCoordP3ui);
   struct marshal_cmd_TexCoordP3ui *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexCoordP3ui, cmd_size);
   cmd->type = MIN2(type, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->coords = coords;
}


/* TexCoordP4ui: marshalled asynchronously */
struct marshal_cmd_TexCoordP4ui
{
   struct marshal_cmd_base cmd_base;
   GLenum16 type;
   GLuint coords;
};
uint32_t
_mesa_unmarshal_TexCoordP4ui(struct gl_context *ctx, const struct marshal_cmd_TexCoordP4ui *restrict cmd)
{
   GLenum type = cmd->type;
   GLuint coords = cmd->coords;
   CALL_TexCoordP4ui(ctx->Dispatch.Current, (type, coords));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_TexCoordP4ui), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_TexCoordP4ui(GLenum type, GLuint coords)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TexCoordP4ui);
   struct marshal_cmd_TexCoordP4ui *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexCoordP4ui, cmd_size);
   cmd->type = MIN2(type, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->coords = coords;
}


/* TexCoordP1uiv: marshalled asynchronously */
struct marshal_cmd_TexCoordP1uiv
{
   struct marshal_cmd_base cmd_base;
   GLenum16 type;
   GLuint coords[1];
};
uint32_t
_mesa_unmarshal_TexCoordP1uiv(struct gl_context *ctx, const struct marshal_cmd_TexCoordP1uiv *restrict cmd)
{
   GLenum type = cmd->type;
   const GLuint *coords = cmd->coords;
   CALL_TexCoordP1uiv(ctx->Dispatch.Current, (type, coords));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_TexCoordP1uiv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_TexCoordP1uiv(GLenum type, const GLuint *coords)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TexCoordP1uiv);
   struct marshal_cmd_TexCoordP1uiv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexCoordP1uiv, cmd_size);
   cmd->type = MIN2(type, 0xffff); /* clamped to 0xffff (invalid enum) */
   memcpy(cmd->coords, coords, 1 * sizeof(GLuint));
}


/* TexCoordP2uiv: marshalled asynchronously */
struct marshal_cmd_TexCoordP2uiv
{
   struct marshal_cmd_base cmd_base;
   GLenum16 type;
   GLuint coords[1];
};
uint32_t
_mesa_unmarshal_TexCoordP2uiv(struct gl_context *ctx, const struct marshal_cmd_TexCoordP2uiv *restrict cmd)
{
   GLenum type = cmd->type;
   const GLuint *coords = cmd->coords;
   CALL_TexCoordP2uiv(ctx->Dispatch.Current, (type, coords));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_TexCoordP2uiv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_TexCoordP2uiv(GLenum type, const GLuint *coords)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TexCoordP2uiv);
   struct marshal_cmd_TexCoordP2uiv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexCoordP2uiv, cmd_size);
   cmd->type = MIN2(type, 0xffff); /* clamped to 0xffff (invalid enum) */
   memcpy(cmd->coords, coords, 1 * sizeof(GLuint));
}


/* TexCoordP3uiv: marshalled asynchronously */
struct marshal_cmd_TexCoordP3uiv
{
   struct marshal_cmd_base cmd_base;
   GLenum16 type;
   GLuint coords[1];
};
uint32_t
_mesa_unmarshal_TexCoordP3uiv(struct gl_context *ctx, const struct marshal_cmd_TexCoordP3uiv *restrict cmd)
{
   GLenum type = cmd->type;
   const GLuint *coords = cmd->coords;
   CALL_TexCoordP3uiv(ctx->Dispatch.Current, (type, coords));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_TexCoordP3uiv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_TexCoordP3uiv(GLenum type, const GLuint *coords)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TexCoordP3uiv);
   struct marshal_cmd_TexCoordP3uiv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexCoordP3uiv, cmd_size);
   cmd->type = MIN2(type, 0xffff); /* clamped to 0xffff (invalid enum) */
   memcpy(cmd->coords, coords, 1 * sizeof(GLuint));
}


/* TexCoordP4uiv: marshalled asynchronously */
struct marshal_cmd_TexCoordP4uiv
{
   struct marshal_cmd_base cmd_base;
   GLenum16 type;
   GLuint coords[1];
};
uint32_t
_mesa_unmarshal_TexCoordP4uiv(struct gl_context *ctx, const struct marshal_cmd_TexCoordP4uiv *restrict cmd)
{
   GLenum type = cmd->type;
   const GLuint *coords = cmd->coords;
   CALL_TexCoordP4uiv(ctx->Dispatch.Current, (type, coords));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_TexCoordP4uiv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_TexCoordP4uiv(GLenum type, const GLuint *coords)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TexCoordP4uiv);
   struct marshal_cmd_TexCoordP4uiv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexCoordP4uiv, cmd_size);
   cmd->type = MIN2(type, 0xffff); /* clamped to 0xffff (invalid enum) */
   memcpy(cmd->coords, coords, 1 * sizeof(GLuint));
}


/* MultiTexCoordP1ui: marshalled asynchronously */
struct marshal_cmd_MultiTexCoordP1ui
{
   struct marshal_cmd_base cmd_base;
   GLenum16 texture;
   GLenum16 type;
   GLuint coords;
};
uint32_t
_mesa_unmarshal_MultiTexCoordP1ui(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoordP1ui *restrict cmd)
{
   GLenum texture = cmd->texture;
   GLenum type = cmd->type;
   GLuint coords = cmd->coords;
   CALL_MultiTexCoordP1ui(ctx->Dispatch.Current, (texture, type, coords));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MultiTexCoordP1ui), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_MultiTexCoordP1ui(GLenum texture, GLenum type, GLuint coords)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexCoordP1ui);
   struct marshal_cmd_MultiTexCoordP1ui *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexCoordP1ui, cmd_size);
   cmd->texture = MIN2(texture, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->type = MIN2(type, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->coords = coords;
}


/* MultiTexCoordP2ui: marshalled asynchronously */
struct marshal_cmd_MultiTexCoordP2ui
{
   struct marshal_cmd_base cmd_base;
   GLenum16 texture;
   GLenum16 type;
   GLuint coords;
};
uint32_t
_mesa_unmarshal_MultiTexCoordP2ui(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoordP2ui *restrict cmd)
{
   GLenum texture = cmd->texture;
   GLenum type = cmd->type;
   GLuint coords = cmd->coords;
   CALL_MultiTexCoordP2ui(ctx->Dispatch.Current, (texture, type, coords));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MultiTexCoordP2ui), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_MultiTexCoordP2ui(GLenum texture, GLenum type, GLuint coords)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexCoordP2ui);
   struct marshal_cmd_MultiTexCoordP2ui *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexCoordP2ui, cmd_size);
   cmd->texture = MIN2(texture, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->type = MIN2(type, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->coords = coords;
}


/* MultiTexCoordP3ui: marshalled asynchronously */
struct marshal_cmd_MultiTexCoordP3ui
{
   struct marshal_cmd_base cmd_base;
   GLenum16 texture;
   GLenum16 type;
   GLuint coords;
};
uint32_t
_mesa_unmarshal_MultiTexCoordP3ui(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoordP3ui *restrict cmd)
{
   GLenum texture = cmd->texture;
   GLenum type = cmd->type;
   GLuint coords = cmd->coords;
   CALL_MultiTexCoordP3ui(ctx->Dispatch.Current, (texture, type, coords));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MultiTexCoordP3ui), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_MultiTexCoordP3ui(GLenum texture, GLenum type, GLuint coords)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexCoordP3ui);
   struct marshal_cmd_MultiTexCoordP3ui *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexCoordP3ui, cmd_size);
   cmd->texture = MIN2(texture, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->type = MIN2(type, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->coords = coords;
}


/* MultiTexCoordP4ui: marshalled asynchronously */
struct marshal_cmd_MultiTexCoordP4ui
{
   struct marshal_cmd_base cmd_base;
   GLenum16 texture;
   GLenum16 type;
   GLuint coords;
};
uint32_t
_mesa_unmarshal_MultiTexCoordP4ui(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoordP4ui *restrict cmd)
{
   GLenum texture = cmd->texture;
   GLenum type = cmd->type;
   GLuint coords = cmd->coords;
   CALL_MultiTexCoordP4ui(ctx->Dispatch.Current, (texture, type, coords));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MultiTexCoordP4ui), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_MultiTexCoordP4ui(GLenum texture, GLenum type, GLuint coords)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexCoordP4ui);
   struct marshal_cmd_MultiTexCoordP4ui *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexCoordP4ui, cmd_size);
   cmd->texture = MIN2(texture, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->type = MIN2(type, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->coords = coords;
}


/* MultiTexCoordP1uiv: marshalled asynchronously */
struct marshal_cmd_MultiTexCoordP1uiv
{
   struct marshal_cmd_base cmd_base;
   GLenum16 texture;
   GLenum16 type;
   GLuint coords[1];
};
uint32_t
_mesa_unmarshal_MultiTexCoordP1uiv(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoordP1uiv *restrict cmd)
{
   GLenum texture = cmd->texture;
   GLenum type = cmd->type;
   const GLuint *coords = cmd->coords;
   CALL_MultiTexCoordP1uiv(ctx->Dispatch.Current, (texture, type, coords));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MultiTexCoordP1uiv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_MultiTexCoordP1uiv(GLenum texture, GLenum type, const GLuint *coords)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexCoordP1uiv);
   struct marshal_cmd_MultiTexCoordP1uiv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexCoordP1uiv, cmd_size);
   cmd->texture = MIN2(texture, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->type = MIN2(type, 0xffff); /* clamped to 0xffff (invalid enum) */
   memcpy(cmd->coords, coords, 1 * sizeof(GLuint));
}


/* MultiTexCoordP2uiv: marshalled asynchronously */
struct marshal_cmd_MultiTexCoordP2uiv
{
   struct marshal_cmd_base cmd_base;
   GLenum16 texture;
   GLenum16 type;
   GLuint coords[1];
};
uint32_t
_mesa_unmarshal_MultiTexCoordP2uiv(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoordP2uiv *restrict cmd)
{
   GLenum texture = cmd->texture;
   GLenum type = cmd->type;
   const GLuint *coords = cmd->coords;
   CALL_MultiTexCoordP2uiv(ctx->Dispatch.Current, (texture, type, coords));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MultiTexCoordP2uiv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_MultiTexCoordP2uiv(GLenum texture, GLenum type, const GLuint *coords)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexCoordP2uiv);
   struct marshal_cmd_MultiTexCoordP2uiv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexCoordP2uiv, cmd_size);
   cmd->texture = MIN2(texture, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->type = MIN2(type, 0xffff); /* clamped to 0xffff (invalid enum) */
   memcpy(cmd->coords, coords, 1 * sizeof(GLuint));
}


/* MultiTexCoordP3uiv: marshalled asynchronously */
struct marshal_cmd_MultiTexCoordP3uiv
{
   struct marshal_cmd_base cmd_base;
   GLenum16 texture;
   GLenum16 type;
   GLuint coords[1];
};
uint32_t
_mesa_unmarshal_MultiTexCoordP3uiv(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoordP3uiv *restrict cmd)
{
   GLenum texture = cmd->texture;
   GLenum type = cmd->type;
   const GLuint *coords = cmd->coords;
   CALL_MultiTexCoordP3uiv(ctx->Dispatch.Current, (texture, type, coords));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MultiTexCoordP3uiv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_MultiTexCoordP3uiv(GLenum texture, GLenum type, const GLuint *coords)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexCoordP3uiv);
   struct marshal_cmd_MultiTexCoordP3uiv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexCoordP3uiv, cmd_size);
   cmd->texture = MIN2(texture, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->type = MIN2(type, 0xffff); /* clamped to 0xffff (invalid enum) */
   memcpy(cmd->coords, coords, 1 * sizeof(GLuint));
}


/* MultiTexCoordP4uiv: marshalled asynchronously */
struct marshal_cmd_MultiTexCoordP4uiv
{
   struct marshal_cmd_base cmd_base;
   GLenum16 texture;
   GLenum16 type;
   GLuint coords[1];
};
uint32_t
_mesa_unmarshal_MultiTexCoordP4uiv(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoordP4uiv *restrict cmd)
{
   GLenum texture = cmd->texture;
   GLenum type = cmd->type;
   const GLuint *coords = cmd->coords;
   CALL_MultiTexCoordP4uiv(ctx->Dispatch.Current, (texture, type, coords));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MultiTexCoordP4uiv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_MultiTexCoordP4uiv(GLenum texture, GLenum type, const GLuint *coords)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexCoordP4uiv);
   struct marshal_cmd_MultiTexCoordP4uiv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexCoordP4uiv, cmd_size);
   cmd->texture = MIN2(texture, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->type = MIN2(type, 0xffff); /* clamped to 0xffff (invalid enum) */
   memcpy(cmd->coords, coords, 1 * sizeof(GLuint));
}


/* NormalP3ui: marshalled asynchronously */
struct marshal_cmd_NormalP3ui
{
   struct marshal_cmd_base cmd_base;
   GLenum16 type;
   GLuint coords;
};
uint32_t
_mesa_unmarshal_NormalP3ui(struct gl_context *ctx, const struct marshal_cmd_NormalP3ui *restrict cmd)
{
   GLenum type = cmd->type;
   GLuint coords = cmd->coords;
   CALL_NormalP3ui(ctx->Dispatch.Current, (type, coords));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_NormalP3ui), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_NormalP3ui(GLenum type, GLuint coords)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_NormalP3ui);
   struct marshal_cmd_NormalP3ui *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_NormalP3ui, cmd_size);
   cmd->type = MIN2(type, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->coords = coords;
}


/* NormalP3uiv: marshalled asynchronously */
struct marshal_cmd_NormalP3uiv
{
   struct marshal_cmd_base cmd_base;
   GLenum16 type;
   GLuint coords[1];
};
uint32_t
_mesa_unmarshal_NormalP3uiv(struct gl_context *ctx, const struct marshal_cmd_NormalP3uiv *restrict cmd)
{
   GLenum type = cmd->type;
   const GLuint *coords = cmd->coords;
   CALL_NormalP3uiv(ctx->Dispatch.Current, (type, coords));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_NormalP3uiv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_NormalP3uiv(GLenum type, const GLuint *coords)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_NormalP3uiv);
   struct marshal_cmd_NormalP3uiv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_NormalP3uiv, cmd_size);
   cmd->type = MIN2(type, 0xffff); /* clamped to 0xffff (invalid enum) */
   memcpy(cmd->coords, coords, 1 * sizeof(GLuint));
}


/* ColorP3ui: marshalled asynchronously */
struct marshal_cmd_ColorP3ui
{
   struct marshal_cmd_base cmd_base;
   GLenum16 type;
   GLuint color;
};
uint32_t
_mesa_unmarshal_ColorP3ui(struct gl_context *ctx, const struct marshal_cmd_ColorP3ui *restrict cmd)
{
   GLenum type = cmd->type;
   GLuint color = cmd->color;
   CALL_ColorP3ui(ctx->Dispatch.Current, (type, color));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_ColorP3ui), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_ColorP3ui(GLenum type, GLuint color)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ColorP3ui);
   struct marshal_cmd_ColorP3ui *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ColorP3ui, cmd_size);
   cmd->type = MIN2(type, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->color = color;
}


/* ColorP4ui: marshalled asynchronously */
struct marshal_cmd_ColorP4ui
{
   struct marshal_cmd_base cmd_base;
   GLenum16 type;
   GLuint color;
};
uint32_t
_mesa_unmarshal_ColorP4ui(struct gl_context *ctx, const struct marshal_cmd_ColorP4ui *restrict cmd)
{
   GLenum type = cmd->type;
   GLuint color = cmd->color;
   CALL_ColorP4ui(ctx->Dispatch.Current, (type, color));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_ColorP4ui), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_ColorP4ui(GLenum type, GLuint color)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ColorP4ui);
   struct marshal_cmd_ColorP4ui *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ColorP4ui, cmd_size);
   cmd->type = MIN2(type, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->color = color;
}


/* ColorP3uiv: marshalled asynchronously */
struct marshal_cmd_ColorP3uiv
{
   struct marshal_cmd_base cmd_base;
   GLenum16 type;
   GLuint color[1];
};
uint32_t
_mesa_unmarshal_ColorP3uiv(struct gl_context *ctx, const struct marshal_cmd_ColorP3uiv *restrict cmd)
{
   GLenum type = cmd->type;
   const GLuint *color = cmd->color;
   CALL_ColorP3uiv(ctx->Dispatch.Current, (type, color));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_ColorP3uiv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_ColorP3uiv(GLenum type, const GLuint *color)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ColorP3uiv);
   struct marshal_cmd_ColorP3uiv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ColorP3uiv, cmd_size);
   cmd->type = MIN2(type, 0xffff); /* clamped to 0xffff (invalid enum) */
   memcpy(cmd->color, color, 1 * sizeof(GLuint));
}


/* ColorP4uiv: marshalled asynchronously */
struct marshal_cmd_ColorP4uiv
{
   struct marshal_cmd_base cmd_base;
   GLenum16 type;
   GLuint color[1];
};
uint32_t
_mesa_unmarshal_ColorP4uiv(struct gl_context *ctx, const struct marshal_cmd_ColorP4uiv *restrict cmd)
{
   GLenum type = cmd->type;
   const GLuint *color = cmd->color;
   CALL_ColorP4uiv(ctx->Dispatch.Current, (type, color));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_ColorP4uiv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_ColorP4uiv(GLenum type, const GLuint *color)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ColorP4uiv);
   struct marshal_cmd_ColorP4uiv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ColorP4uiv, cmd_size);
   cmd->type = MIN2(type, 0xffff); /* clamped to 0xffff (invalid enum) */
   memcpy(cmd->color, color, 1 * sizeof(GLuint));
}


/* SecondaryColorP3ui: marshalled asynchronously */
struct marshal_cmd_SecondaryColorP3ui
{
   struct marshal_cmd_base cmd_base;
   GLenum16 type;
   GLuint color;
};
uint32_t
_mesa_unmarshal_SecondaryColorP3ui(struct gl_context *ctx, const struct marshal_cmd_SecondaryColorP3ui *restrict cmd)
{
   GLenum type = cmd->type;
   GLuint color = cmd->color;
   CALL_SecondaryColorP3ui(ctx->Dispatch.Current, (type, color));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_SecondaryColorP3ui), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_SecondaryColorP3ui(GLenum type, GLuint color)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_SecondaryColorP3ui);
   struct marshal_cmd_SecondaryColorP3ui *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_SecondaryColorP3ui, cmd_size);
   cmd->type = MIN2(type, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->color = color;
}


/* SecondaryColorP3uiv: marshalled asynchronously */
struct marshal_cmd_SecondaryColorP3uiv
{
   struct marshal_cmd_base cmd_base;
   GLenum16 type;
   GLuint color[1];
};
uint32_t
_mesa_unmarshal_SecondaryColorP3uiv(struct gl_context *ctx, const struct marshal_cmd_SecondaryColorP3uiv *restrict cmd)
{
   GLenum type = cmd->type;
   const GLuint *color = cmd->color;
   CALL_SecondaryColorP3uiv(ctx->Dispatch.Current, (type, color));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_SecondaryColorP3uiv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_SecondaryColorP3uiv(GLenum type, const GLuint *color)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_SecondaryColorP3uiv);
   struct marshal_cmd_SecondaryColorP3uiv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_SecondaryColorP3uiv, cmd_size);
   cmd->type = MIN2(type, 0xffff); /* clamped to 0xffff (invalid enum) */
   memcpy(cmd->color, color, 1 * sizeof(GLuint));
}


/* VertexAttribP1ui: marshalled asynchronously */
struct marshal_cmd_VertexAttribP1ui
{
   struct marshal_cmd_base cmd_base;
   GLboolean normalized;
   GLenum16 type;
   GLuint index;
   GLuint value;
};
uint32_t
_mesa_unmarshal_VertexAttribP1ui(struct gl_context *ctx, const struct marshal_cmd_VertexAttribP1ui *restrict cmd)
{
   GLuint index = cmd->index;
   GLenum type = cmd->type;
   GLboolean normalized = cmd->normalized;
   GLuint value = cmd->value;
   CALL_VertexAttribP1ui(ctx->Dispatch.Current, (index, type, normalized, value));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexAttribP1ui), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_VertexAttribP1ui(GLuint index, GLenum type, GLboolean normalized, GLuint value)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttribP1ui);
   struct marshal_cmd_VertexAttribP1ui *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttribP1ui, cmd_size);
   cmd->index = index;
   cmd->type = MIN2(type, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->normalized = normalized;
   cmd->value = value;
}


/* VertexAttribP2ui: marshalled asynchronously */
struct marshal_cmd_VertexAttribP2ui
{
   struct marshal_cmd_base cmd_base;
   GLboolean normalized;
   GLenum16 type;
   GLuint index;
   GLuint value;
};
uint32_t
_mesa_unmarshal_VertexAttribP2ui(struct gl_context *ctx, const struct marshal_cmd_VertexAttribP2ui *restrict cmd)
{
   GLuint index = cmd->index;
   GLenum type = cmd->type;
   GLboolean normalized = cmd->normalized;
   GLuint value = cmd->value;
   CALL_VertexAttribP2ui(ctx->Dispatch.Current, (index, type, normalized, value));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexAttribP2ui), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_VertexAttribP2ui(GLuint index, GLenum type, GLboolean normalized, GLuint value)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttribP2ui);
   struct marshal_cmd_VertexAttribP2ui *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttribP2ui, cmd_size);
   cmd->index = index;
   cmd->type = MIN2(type, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->normalized = normalized;
   cmd->value = value;
}


/* VertexAttribP3ui: marshalled asynchronously */
struct marshal_cmd_VertexAttribP3ui
{
   struct marshal_cmd_base cmd_base;
   GLboolean normalized;
   GLenum16 type;
   GLuint index;
   GLuint value;
};
uint32_t
_mesa_unmarshal_VertexAttribP3ui(struct gl_context *ctx, const struct marshal_cmd_VertexAttribP3ui *restrict cmd)
{
   GLuint index = cmd->index;
   GLenum type = cmd->type;
   GLboolean normalized = cmd->normalized;
   GLuint value = cmd->value;
   CALL_VertexAttribP3ui(ctx->Dispatch.Current, (index, type, normalized, value));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexAttribP3ui), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_VertexAttribP3ui(GLuint index, GLenum type, GLboolean normalized, GLuint value)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttribP3ui);
   struct marshal_cmd_VertexAttribP3ui *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttribP3ui, cmd_size);
   cmd->index = index;
   cmd->type = MIN2(type, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->normalized = normalized;
   cmd->value = value;
}


/* VertexAttribP4ui: marshalled asynchronously */
struct marshal_cmd_VertexAttribP4ui
{
   struct marshal_cmd_base cmd_base;
   GLboolean normalized;
   GLenum16 type;
   GLuint index;
   GLuint value;
};
uint32_t
_mesa_unmarshal_VertexAttribP4ui(struct gl_context *ctx, const struct marshal_cmd_VertexAttribP4ui *restrict cmd)
{
   GLuint index = cmd->index;
   GLenum type = cmd->type;
   GLboolean normalized = cmd->normalized;
   GLuint value = cmd->value;
   CALL_VertexAttribP4ui(ctx->Dispatch.Current, (index, type, normalized, value));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexAttribP4ui), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_VertexAttribP4ui(GLuint index, GLenum type, GLboolean normalized, GLuint value)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttribP4ui);
   struct marshal_cmd_VertexAttribP4ui *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttribP4ui, cmd_size);
   cmd->index = index;
   cmd->type = MIN2(type, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->normalized = normalized;
   cmd->value = value;
}


/* VertexAttribP1uiv: marshalled asynchronously */
struct marshal_cmd_VertexAttribP1uiv
{
   struct marshal_cmd_base cmd_base;
   GLboolean normalized;
   GLenum16 type;
   GLuint index;
   GLuint value[1];
};
uint32_t
_mesa_unmarshal_VertexAttribP1uiv(struct gl_context *ctx, const struct marshal_cmd_VertexAttribP1uiv *restrict cmd)
{
   GLuint index = cmd->index;
   GLenum type = cmd->type;
   GLboolean normalized = cmd->normalized;
   const GLuint *value = cmd->value;
   CALL_VertexAttribP1uiv(ctx->Dispatch.Current, (index, type, normalized, value));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexAttribP1uiv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_VertexAttribP1uiv(GLuint index, GLenum type, GLboolean normalized, const GLuint *value)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttribP1uiv);
   struct marshal_cmd_VertexAttribP1uiv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttribP1uiv, cmd_size);
   cmd->index = index;
   cmd->type = MIN2(type, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->normalized = normalized;
   memcpy(cmd->value, value, 1 * sizeof(GLuint));
}


/* VertexAttribP2uiv: marshalled asynchronously */
struct marshal_cmd_VertexAttribP2uiv
{
   struct marshal_cmd_base cmd_base;
   GLboolean normalized;
   GLenum16 type;
   GLuint index;
   GLuint value[1];
};
uint32_t
_mesa_unmarshal_VertexAttribP2uiv(struct gl_context *ctx, const struct marshal_cmd_VertexAttribP2uiv *restrict cmd)
{
   GLuint index = cmd->index;
   GLenum type = cmd->type;
   GLboolean normalized = cmd->normalized;
   const GLuint *value = cmd->value;
   CALL_VertexAttribP2uiv(ctx->Dispatch.Current, (index, type, normalized, value));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexAttribP2uiv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_VertexAttribP2uiv(GLuint index, GLenum type, GLboolean normalized, const GLuint *value)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttribP2uiv);
   struct marshal_cmd_VertexAttribP2uiv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttribP2uiv, cmd_size);
   cmd->index = index;
   cmd->type = MIN2(type, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->normalized = normalized;
   memcpy(cmd->value, value, 1 * sizeof(GLuint));
}


/* VertexAttribP3uiv: marshalled asynchronously */
struct marshal_cmd_VertexAttribP3uiv
{
   struct marshal_cmd_base cmd_base;
   GLboolean normalized;
   GLenum16 type;
   GLuint index;
   GLuint value[1];
};
uint32_t
_mesa_unmarshal_VertexAttribP3uiv(struct gl_context *ctx, const struct marshal_cmd_VertexAttribP3uiv *restrict cmd)
{
   GLuint index = cmd->index;
   GLenum type = cmd->type;
   GLboolean normalized = cmd->normalized;
   const GLuint *value = cmd->value;
   CALL_VertexAttribP3uiv(ctx->Dispatch.Current, (index, type, normalized, value));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexAttribP3uiv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_VertexAttribP3uiv(GLuint index, GLenum type, GLboolean normalized, const GLuint *value)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttribP3uiv);
   struct marshal_cmd_VertexAttribP3uiv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttribP3uiv, cmd_size);
   cmd->index = index;
   cmd->type = MIN2(type, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->normalized = normalized;
   memcpy(cmd->value, value, 1 * sizeof(GLuint));
}


/* VertexAttribP4uiv: marshalled asynchronously */
struct marshal_cmd_VertexAttribP4uiv
{
   struct marshal_cmd_base cmd_base;
   GLboolean normalized;
   GLenum16 type;
   GLuint index;
   GLuint value[1];
};
uint32_t
_mesa_unmarshal_VertexAttribP4uiv(struct gl_context *ctx, const struct marshal_cmd_VertexAttribP4uiv *restrict cmd)
{
   GLuint index = cmd->index;
   GLenum type = cmd->type;
   GLboolean normalized = cmd->normalized;
   const GLuint *value = cmd->value;
   CALL_VertexAttribP4uiv(ctx->Dispatch.Current, (index, type, normalized, value));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexAttribP4uiv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_VertexAttribP4uiv(GLuint index, GLenum type, GLboolean normalized, const GLuint *value)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttribP4uiv);
   struct marshal_cmd_VertexAttribP4uiv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttribP4uiv, cmd_size);
   cmd->index = index;
   cmd->type = MIN2(type, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->normalized = normalized;
   memcpy(cmd->value, value, 1 * sizeof(GLuint));
}


/* GetSubroutineUniformLocation: marshalled synchronously */
static GLint GLAPIENTRY
_mesa_marshal_GetSubroutineUniformLocation(GLuint program, GLenum shadertype, const GLchar *name)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetSubroutineUniformLocation");
   return CALL_GetSubroutineUniformLocation(ctx->Dispatch.Current, (program, shadertype, name));
}


/* GetSubroutineIndex: marshalled synchronously */
static GLuint GLAPIENTRY
_mesa_marshal_GetSubroutineIndex(GLuint program, GLenum shadertype, const GLchar *name)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetSubroutineIndex");
   return CALL_GetSubroutineIndex(ctx->Dispatch.Current, (program, shadertype, name));
}


/* GetActiveSubroutineUniformiv: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetActiveSubroutineUniformiv(GLuint program, GLenum shadertype, GLuint index, GLenum pname, GLint *values)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetActiveSubroutineUniformiv");
   CALL_GetActiveSubroutineUniformiv(ctx->Dispatch.Current, (program, shadertype, index, pname, values));
}


/* GetActiveSubroutineUniformName: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetActiveSubroutineUniformName(GLuint program, GLenum shadertype, GLuint index, GLsizei bufsize, GLsizei *length, GLchar *name)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetActiveSubroutineUniformName");
   CALL_GetActiveSubroutineUniformName(ctx->Dispatch.Current, (program, shadertype, index, bufsize, length, name));
}


/* GetActiveSubroutineName: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetActiveSubroutineName(GLuint program, GLenum shadertype, GLuint index, GLsizei bufsize, GLsizei *length, GLchar *name)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetActiveSubroutineName");
   CALL_GetActiveSubroutineName(ctx->Dispatch.Current, (program, shadertype, index, bufsize, length, name));
}


/* UniformSubroutinesuiv: marshalled asynchronously */
struct marshal_cmd_UniformSubroutinesuiv
{
   struct marshal_cmd_base cmd_base;
   GLenum16 shadertype;
   GLsizei count;
   /* Next safe_mul(count, 1 * sizeof(GLuint)) bytes are GLuint indices[count] */
};
uint32_t
_mesa_unmarshal_UniformSubroutinesuiv(struct gl_context *ctx, const struct marshal_cmd_UniformSubroutinesuiv *restrict cmd)
{
   GLenum shadertype = cmd->shadertype;
   GLsizei count = cmd->count;
   GLuint *indices;
   const char *variable_data = (const char *) (cmd + 1);
   indices = (GLuint *) variable_data;
   CALL_UniformSubroutinesuiv(ctx->Dispatch.Current, (shadertype, count, indices));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_UniformSubroutinesuiv(GLenum shadertype, GLsizei count, const GLuint *indices)
{
   GET_CURRENT_CONTEXT(ctx);
   int indices_size = safe_mul(count, 1 * sizeof(GLuint));
   int cmd_size = sizeof(struct marshal_cmd_UniformSubroutinesuiv) + indices_size;
   struct marshal_cmd_UniformSubroutinesuiv *cmd;
   if (unlikely(indices_size < 0 || (indices_size > 0 && !indices) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "UniformSubroutinesuiv");
      CALL_UniformSubroutinesuiv(ctx->Dispatch.Current, (shadertype, count, indices));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_UniformSubroutinesuiv, cmd_size);
   cmd->shadertype = MIN2(shadertype, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->count = count;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, indices, indices_size);
}


/* GetUniformSubroutineuiv: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetUniformSubroutineuiv(GLenum shadertype, GLint location, GLuint *params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetUniformSubroutineuiv");
   CALL_GetUniformSubroutineuiv(ctx->Dispatch.Current, (shadertype, location, params));
}


/* GetProgramStageiv: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetProgramStageiv(GLuint program, GLenum shadertype, GLenum pname, GLint *values)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetProgramStageiv");
   CALL_GetProgramStageiv(ctx->Dispatch.Current, (program, shadertype, pname, values));
}


/* PatchParameteri: marshalled asynchronously */
struct marshal_cmd_PatchParameteri
{
   struct marshal_cmd_base cmd_base;
   GLenum16 pname;
   GLint value;
};
uint32_t
_mesa_unmarshal_PatchParameteri(struct gl_context *ctx, const struct marshal_cmd_PatchParameteri *restrict cmd)
{
   GLenum pname = cmd->pname;
   GLint value = cmd->value;
   CALL_PatchParameteri(ctx->Dispatch.Current, (pname, value));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_PatchParameteri), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_PatchParameteri(GLenum pname, GLint value)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_PatchParameteri);
   struct marshal_cmd_PatchParameteri *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_PatchParameteri, cmd_size);
   cmd->pname = MIN2(pname, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->value = value;
}


/* PatchParameterfv: marshalled asynchronously */
struct marshal_cmd_PatchParameterfv
{
   struct marshal_cmd_base cmd_base;
   GLenum16 pname;
   /* Next safe_mul(_mesa_patch_param_enum_to_count(pname), 1 * sizeof(GLfloat)) bytes are GLfloat values[_mesa_patch_param_enum_to_count(pname)] */
};
uint32_t
_mesa_unmarshal_PatchParameterfv(struct gl_context *ctx, const struct marshal_cmd_PatchParameterfv *restrict cmd)
{
   GLenum pname = cmd->pname;
   GLfloat *values;
   const char *variable_data = (const char *) (cmd + 1);
   values = (GLfloat *) variable_data;
   CALL_PatchParameterfv(ctx->Dispatch.Current, (pname, values));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_PatchParameterfv(GLenum pname, const GLfloat *values)
{
   GET_CURRENT_CONTEXT(ctx);
   int values_size = safe_mul(_mesa_patch_param_enum_to_count(pname), 1 * sizeof(GLfloat));
   int cmd_size = sizeof(struct marshal_cmd_PatchParameterfv) + values_size;
   struct marshal_cmd_PatchParameterfv *cmd;
   if (unlikely(values_size < 0 || (values_size > 0 && !values) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "PatchParameterfv");
      CALL_PatchParameterfv(ctx->Dispatch.Current, (pname, values));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_PatchParameterfv, cmd_size);
   cmd->pname = MIN2(pname, 0xffff); /* clamped to 0xffff (invalid enum) */
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, values, values_size);
}


/* Uniform1d: marshalled asynchronously */
struct marshal_cmd_Uniform1d
{
   struct marshal_cmd_base cmd_base;
   GLint location;
   GLdouble x;
};
uint32_t
_mesa_unmarshal_Uniform1d(struct gl_context *ctx, const struct marshal_cmd_Uniform1d *restrict cmd)
{
   GLint location = cmd->location;
   GLdouble x = cmd->x;
   CALL_Uniform1d(ctx->Dispatch.Current, (location, x));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Uniform1d), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_Uniform1d(GLint location, GLdouble x)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Uniform1d);
   struct marshal_cmd_Uniform1d *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Uniform1d, cmd_size);
   cmd->location = location;
   cmd->x = x;
}


/* Uniform2d: marshalled asynchronously */
struct marshal_cmd_Uniform2d
{
   struct marshal_cmd_base cmd_base;
   GLint location;
   GLdouble x;
   GLdouble y;
};
uint32_t
_mesa_unmarshal_Uniform2d(struct gl_context *ctx, const struct marshal_cmd_Uniform2d *restrict cmd)
{
   GLint location = cmd->location;
   GLdouble x = cmd->x;
   GLdouble y = cmd->y;
   CALL_Uniform2d(ctx->Dispatch.Current, (location, x, y));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Uniform2d), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_Uniform2d(GLint location, GLdouble x, GLdouble y)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Uniform2d);
   struct marshal_cmd_Uniform2d *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Uniform2d, cmd_size);
   cmd->location = location;
   cmd->x = x;
   cmd->y = y;
}


/* Uniform3d: marshalled asynchronously */
struct marshal_cmd_Uniform3d
{
   struct marshal_cmd_base cmd_base;
   GLint location;
   GLdouble x;
   GLdouble y;
   GLdouble z;
};
uint32_t
_mesa_unmarshal_Uniform3d(struct gl_context *ctx, const struct marshal_cmd_Uniform3d *restrict cmd)
{
   GLint location = cmd->location;
   GLdouble x = cmd->x;
   GLdouble y = cmd->y;
   GLdouble z = cmd->z;
   CALL_Uniform3d(ctx->Dispatch.Current, (location, x, y, z));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Uniform3d), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_Uniform3d(GLint location, GLdouble x, GLdouble y, GLdouble z)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Uniform3d);
   struct marshal_cmd_Uniform3d *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Uniform3d, cmd_size);
   cmd->location = location;
   cmd->x = x;
   cmd->y = y;
   cmd->z = z;
}


/* Uniform4d: marshalled asynchronously */
struct marshal_cmd_Uniform4d
{
   struct marshal_cmd_base cmd_base;
   GLint location;
   GLdouble x;
   GLdouble y;
   GLdouble z;
   GLdouble w;
};
uint32_t
_mesa_unmarshal_Uniform4d(struct gl_context *ctx, const struct marshal_cmd_Uniform4d *restrict cmd)
{
   GLint location = cmd->location;
   GLdouble x = cmd->x;
   GLdouble y = cmd->y;
   GLdouble z = cmd->z;
   GLdouble w = cmd->w;
   CALL_Uniform4d(ctx->Dispatch.Current, (location, x, y, z, w));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Uniform4d), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_Uniform4d(GLint location, GLdouble x, GLdouble y, GLdouble z, GLdouble w)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Uniform4d);
   struct marshal_cmd_Uniform4d *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Uniform4d, cmd_size);
   cmd->location = location;
   cmd->x = x;
   cmd->y = y;
   cmd->z = z;
   cmd->w = w;
}


/* Uniform1dv: marshalled asynchronously */
struct marshal_cmd_Uniform1dv
{
   struct marshal_cmd_base cmd_base;
   GLint location;
   GLsizei count;
   /* Next safe_mul(count, 1 * sizeof(GLdouble)) bytes are GLdouble value[count] */
};
uint32_t
_mesa_unmarshal_Uniform1dv(struct gl_context *ctx, const struct marshal_cmd_Uniform1dv *restrict cmd)
{
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLdouble *value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLdouble *) variable_data;
   CALL_Uniform1dv(ctx->Dispatch.Current, (location, count, value));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_Uniform1dv(GLint location, GLsizei count, const GLdouble *value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 1 * sizeof(GLdouble));
   int cmd_size = sizeof(struct marshal_cmd_Uniform1dv) + value_size;
   struct marshal_cmd_Uniform1dv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "Uniform1dv");
      CALL_Uniform1dv(ctx->Dispatch.Current, (location, count, value));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Uniform1dv, cmd_size);
   cmd->location = location;
   cmd->count = count;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, value, value_size);
}


/* Uniform2dv: marshalled asynchronously */
struct marshal_cmd_Uniform2dv
{
   struct marshal_cmd_base cmd_base;
   GLint location;
   GLsizei count;
   /* Next safe_mul(count, 2 * sizeof(GLdouble)) bytes are GLdouble value[count][2] */
};
uint32_t
_mesa_unmarshal_Uniform2dv(struct gl_context *ctx, const struct marshal_cmd_Uniform2dv *restrict cmd)
{
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLdouble *value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLdouble *) variable_data;
   CALL_Uniform2dv(ctx->Dispatch.Current, (location, count, value));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_Uniform2dv(GLint location, GLsizei count, const GLdouble *value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 2 * sizeof(GLdouble));
   int cmd_size = sizeof(struct marshal_cmd_Uniform2dv) + value_size;
   struct marshal_cmd_Uniform2dv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "Uniform2dv");
      CALL_Uniform2dv(ctx->Dispatch.Current, (location, count, value));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Uniform2dv, cmd_size);
   cmd->location = location;
   cmd->count = count;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, value, value_size);
}


/* Uniform3dv: marshalled asynchronously */
struct marshal_cmd_Uniform3dv
{
   struct marshal_cmd_base cmd_base;
   GLint location;
   GLsizei count;
   /* Next safe_mul(count, 3 * sizeof(GLdouble)) bytes are GLdouble value[count][3] */
};
uint32_t
_mesa_unmarshal_Uniform3dv(struct gl_context *ctx, const struct marshal_cmd_Uniform3dv *restrict cmd)
{
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLdouble *value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLdouble *) variable_data;
   CALL_Uniform3dv(ctx->Dispatch.Current, (location, count, value));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_Uniform3dv(GLint location, GLsizei count, const GLdouble *value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 3 * sizeof(GLdouble));
   int cmd_size = sizeof(struct marshal_cmd_Uniform3dv) + value_size;
   struct marshal_cmd_Uniform3dv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "Uniform3dv");
      CALL_Uniform3dv(ctx->Dispatch.Current, (location, count, value));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Uniform3dv, cmd_size);
   cmd->location = location;
   cmd->count = count;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, value, value_size);
}


/* Uniform4dv: marshalled asynchronously */
struct marshal_cmd_Uniform4dv
{
   struct marshal_cmd_base cmd_base;
   GLint location;
   GLsizei count;
   /* Next safe_mul(count, 4 * sizeof(GLdouble)) bytes are GLdouble value[count][4] */
};
uint32_t
_mesa_unmarshal_Uniform4dv(struct gl_context *ctx, const struct marshal_cmd_Uniform4dv *restrict cmd)
{
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLdouble *value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLdouble *) variable_data;
   CALL_Uniform4dv(ctx->Dispatch.Current, (location, count, value));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_Uniform4dv(GLint location, GLsizei count, const GLdouble *value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 4 * sizeof(GLdouble));
   int cmd_size = sizeof(struct marshal_cmd_Uniform4dv) + value_size;
   struct marshal_cmd_Uniform4dv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "Uniform4dv");
      CALL_Uniform4dv(ctx->Dispatch.Current, (location, count, value));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Uniform4dv, cmd_size);
   cmd->location = location;
   cmd->count = count;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, value, value_size);
}


/* UniformMatrix2dv: marshalled asynchronously */
struct marshal_cmd_UniformMatrix2dv
{
   struct marshal_cmd_base cmd_base;
   GLboolean transpose;
   GLint location;
   GLsizei count;
   /* Next safe_mul(count, 4 * sizeof(GLdouble)) bytes are GLdouble value[count][4] */
};
uint32_t
_mesa_unmarshal_UniformMatrix2dv(struct gl_context *ctx, const struct marshal_cmd_UniformMatrix2dv *restrict cmd)
{
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLboolean transpose = cmd->transpose;
   GLdouble *value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLdouble *) variable_data;
   CALL_UniformMatrix2dv(ctx->Dispatch.Current, (location, count, transpose, value));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_UniformMatrix2dv(GLint location, GLsizei count, GLboolean transpose, const GLdouble *value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 4 * sizeof(GLdouble));
   int cmd_size = sizeof(struct marshal_cmd_UniformMatrix2dv) + value_size;
   struct marshal_cmd_UniformMatrix2dv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "UniformMatrix2dv");
      CALL_UniformMatrix2dv(ctx->Dispatch.Current, (location, count, transpose, value));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_UniformMatrix2dv, cmd_size);
   cmd->location = location;
   cmd->count = count;
   cmd->transpose = transpose;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, value, value_size);
}


/* UniformMatrix3dv: marshalled asynchronously */
struct marshal_cmd_UniformMatrix3dv
{
   struct marshal_cmd_base cmd_base;
   GLboolean transpose;
   GLint location;
   GLsizei count;
   /* Next safe_mul(count, 9 * sizeof(GLdouble)) bytes are GLdouble value[count][9] */
};
uint32_t
_mesa_unmarshal_UniformMatrix3dv(struct gl_context *ctx, const struct marshal_cmd_UniformMatrix3dv *restrict cmd)
{
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLboolean transpose = cmd->transpose;
   GLdouble *value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLdouble *) variable_data;
   CALL_UniformMatrix3dv(ctx->Dispatch.Current, (location, count, transpose, value));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_UniformMatrix3dv(GLint location, GLsizei count, GLboolean transpose, const GLdouble *value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 9 * sizeof(GLdouble));
   int cmd_size = sizeof(struct marshal_cmd_UniformMatrix3dv) + value_size;
   struct marshal_cmd_UniformMatrix3dv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "UniformMatrix3dv");
      CALL_UniformMatrix3dv(ctx->Dispatch.Current, (location, count, transpose, value));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_UniformMatrix3dv, cmd_size);
   cmd->location = location;
   cmd->count = count;
   cmd->transpose = transpose;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, value, value_size);
}


/* UniformMatrix4dv: marshalled asynchronously */
struct marshal_cmd_UniformMatrix4dv
{
   struct marshal_cmd_base cmd_base;
   GLboolean transpose;
   GLint location;
   GLsizei count;
   /* Next safe_mul(count, 16 * sizeof(GLdouble)) bytes are GLdouble value[count][16] */
};
uint32_t
_mesa_unmarshal_UniformMatrix4dv(struct gl_context *ctx, const struct marshal_cmd_UniformMatrix4dv *restrict cmd)
{
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLboolean transpose = cmd->transpose;
   GLdouble *value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLdouble *) variable_data;
   CALL_UniformMatrix4dv(ctx->Dispatch.Current, (location, count, transpose, value));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_UniformMatrix4dv(GLint location, GLsizei count, GLboolean transpose, const GLdouble *value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 16 * sizeof(GLdouble));
   int cmd_size = sizeof(struct marshal_cmd_UniformMatrix4dv) + value_size;
   struct marshal_cmd_UniformMatrix4dv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "UniformMatrix4dv");
      CALL_UniformMatrix4dv(ctx->Dispatch.Current, (location, count, transpose, value));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_UniformMatrix4dv, cmd_size);
   cmd->location = location;
   cmd->count = count;
   cmd->transpose = transpose;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, value, value_size);
}


/* UniformMatrix2x3dv: marshalled asynchronously */
struct marshal_cmd_UniformMatrix2x3dv
{
   struct marshal_cmd_base cmd_base;
   GLboolean transpose;
   GLint location;
   GLsizei count;
   /* Next safe_mul(count, 6 * sizeof(GLdouble)) bytes are GLdouble value[count][6] */
};
uint32_t
_mesa_unmarshal_UniformMatrix2x3dv(struct gl_context *ctx, const struct marshal_cmd_UniformMatrix2x3dv *restrict cmd)
{
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLboolean transpose = cmd->transpose;
   GLdouble *value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLdouble *) variable_data;
   CALL_UniformMatrix2x3dv(ctx->Dispatch.Current, (location, count, transpose, value));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_UniformMatrix2x3dv(GLint location, GLsizei count, GLboolean transpose, const GLdouble *value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 6 * sizeof(GLdouble));
   int cmd_size = sizeof(struct marshal_cmd_UniformMatrix2x3dv) + value_size;
   struct marshal_cmd_UniformMatrix2x3dv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "UniformMatrix2x3dv");
      CALL_UniformMatrix2x3dv(ctx->Dispatch.Current, (location, count, transpose, value));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_UniformMatrix2x3dv, cmd_size);
   cmd->location = location;
   cmd->count = count;
   cmd->transpose = transpose;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, value, value_size);
}


/* UniformMatrix2x4dv: marshalled asynchronously */
struct marshal_cmd_UniformMatrix2x4dv
{
   struct marshal_cmd_base cmd_base;
   GLboolean transpose;
   GLint location;
   GLsizei count;
   /* Next safe_mul(count, 8 * sizeof(GLdouble)) bytes are GLdouble value[count][8] */
};
uint32_t
_mesa_unmarshal_UniformMatrix2x4dv(struct gl_context *ctx, const struct marshal_cmd_UniformMatrix2x4dv *restrict cmd)
{
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLboolean transpose = cmd->transpose;
   GLdouble *value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLdouble *) variable_data;
   CALL_UniformMatrix2x4dv(ctx->Dispatch.Current, (location, count, transpose, value));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_UniformMatrix2x4dv(GLint location, GLsizei count, GLboolean transpose, const GLdouble *value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 8 * sizeof(GLdouble));
   int cmd_size = sizeof(struct marshal_cmd_UniformMatrix2x4dv) + value_size;
   struct marshal_cmd_UniformMatrix2x4dv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "UniformMatrix2x4dv");
      CALL_UniformMatrix2x4dv(ctx->Dispatch.Current, (location, count, transpose, value));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_UniformMatrix2x4dv, cmd_size);
   cmd->location = location;
   cmd->count = count;
   cmd->transpose = transpose;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, value, value_size);
}


/* UniformMatrix3x2dv: marshalled asynchronously */
struct marshal_cmd_UniformMatrix3x2dv
{
   struct marshal_cmd_base cmd_base;
   GLboolean transpose;
   GLint location;
   GLsizei count;
   /* Next safe_mul(count, 6 * sizeof(GLdouble)) bytes are GLdouble value[count][6] */
};
uint32_t
_mesa_unmarshal_UniformMatrix3x2dv(struct gl_context *ctx, const struct marshal_cmd_UniformMatrix3x2dv *restrict cmd)
{
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLboolean transpose = cmd->transpose;
   GLdouble *value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLdouble *) variable_data;
   CALL_UniformMatrix3x2dv(ctx->Dispatch.Current, (location, count, transpose, value));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_UniformMatrix3x2dv(GLint location, GLsizei count, GLboolean transpose, const GLdouble *value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 6 * sizeof(GLdouble));
   int cmd_size = sizeof(struct marshal_cmd_UniformMatrix3x2dv) + value_size;
   struct marshal_cmd_UniformMatrix3x2dv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "UniformMatrix3x2dv");
      CALL_UniformMatrix3x2dv(ctx->Dispatch.Current, (location, count, transpose, value));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_UniformMatrix3x2dv, cmd_size);
   cmd->location = location;
   cmd->count = count;
   cmd->transpose = transpose;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, value, value_size);
}


/* UniformMatrix3x4dv: marshalled asynchronously */
struct marshal_cmd_UniformMatrix3x4dv
{
   struct marshal_cmd_base cmd_base;
   GLboolean transpose;
   GLint location;
   GLsizei count;
   /* Next safe_mul(count, 12 * sizeof(GLdouble)) bytes are GLdouble value[count][12] */
};
uint32_t
_mesa_unmarshal_UniformMatrix3x4dv(struct gl_context *ctx, const struct marshal_cmd_UniformMatrix3x4dv *restrict cmd)
{
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLboolean transpose = cmd->transpose;
   GLdouble *value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLdouble *) variable_data;
   CALL_UniformMatrix3x4dv(ctx->Dispatch.Current, (location, count, transpose, value));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_UniformMatrix3x4dv(GLint location, GLsizei count, GLboolean transpose, const GLdouble *value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 12 * sizeof(GLdouble));
   int cmd_size = sizeof(struct marshal_cmd_UniformMatrix3x4dv) + value_size;
   struct marshal_cmd_UniformMatrix3x4dv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "UniformMatrix3x4dv");
      CALL_UniformMatrix3x4dv(ctx->Dispatch.Current, (location, count, transpose, value));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_UniformMatrix3x4dv, cmd_size);
   cmd->location = location;
   cmd->count = count;
   cmd->transpose = transpose;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, value, value_size);
}


/* UniformMatrix4x2dv: marshalled asynchronously */
struct marshal_cmd_UniformMatrix4x2dv
{
   struct marshal_cmd_base cmd_base;
   GLboolean transpose;
   GLint location;
   GLsizei count;
   /* Next safe_mul(count, 8 * sizeof(GLdouble)) bytes are GLdouble value[count][8] */
};
uint32_t
_mesa_unmarshal_UniformMatrix4x2dv(struct gl_context *ctx, const struct marshal_cmd_UniformMatrix4x2dv *restrict cmd)
{
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLboolean transpose = cmd->transpose;
   GLdouble *value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLdouble *) variable_data;
   CALL_UniformMatrix4x2dv(ctx->Dispatch.Current, (location, count, transpose, value));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_UniformMatrix4x2dv(GLint location, GLsizei count, GLboolean transpose, const GLdouble *value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 8 * sizeof(GLdouble));
   int cmd_size = sizeof(struct marshal_cmd_UniformMatrix4x2dv) + value_size;
   struct marshal_cmd_UniformMatrix4x2dv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "UniformMatrix4x2dv");
      CALL_UniformMatrix4x2dv(ctx->Dispatch.Current, (location, count, transpose, value));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_UniformMatrix4x2dv, cmd_size);
   cmd->location = location;
   cmd->count = count;
   cmd->transpose = transpose;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, value, value_size);
}


/* UniformMatrix4x3dv: marshalled asynchronously */
struct marshal_cmd_UniformMatrix4x3dv
{
   struct marshal_cmd_base cmd_base;
   GLboolean transpose;
   GLint location;
   GLsizei count;
   /* Next safe_mul(count, 12 * sizeof(GLdouble)) bytes are GLdouble value[count][12] */
};
uint32_t
_mesa_unmarshal_UniformMatrix4x3dv(struct gl_context *ctx, const struct marshal_cmd_UniformMatrix4x3dv *restrict cmd)
{
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLboolean transpose = cmd->transpose;
   GLdouble *value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLdouble *) variable_data;
   CALL_UniformMatrix4x3dv(ctx->Dispatch.Current, (location, count, transpose, value));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_UniformMatrix4x3dv(GLint location, GLsizei count, GLboolean transpose, const GLdouble *value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 12 * sizeof(GLdouble));
   int cmd_size = sizeof(struct marshal_cmd_UniformMatrix4x3dv) + value_size;
   struct marshal_cmd_UniformMatrix4x3dv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "UniformMatrix4x3dv");
      CALL_UniformMatrix4x3dv(ctx->Dispatch.Current, (location, count, transpose, value));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_UniformMatrix4x3dv, cmd_size);
   cmd->location = location;
   cmd->count = count;
   cmd->transpose = transpose;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, value, value_size);
}


/* GetUniformdv: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetUniformdv(GLuint program, GLint location, GLdouble *params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetUniformdv");
   CALL_GetUniformdv(ctx->Dispatch.Current, (program, location, params));
}


/* ProgramUniform1d: marshalled asynchronously */
struct marshal_cmd_ProgramUniform1d
{
   struct marshal_cmd_base cmd_base;
   GLuint program;
   GLint location;
   GLdouble x;
};
uint32_t
_mesa_unmarshal_ProgramUniform1d(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform1d *restrict cmd)
{
   GLuint program = cmd->program;
   GLint location = cmd->location;
   GLdouble x = cmd->x;
   CALL_ProgramUniform1d(ctx->Dispatch.Current, (program, location, x));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_ProgramUniform1d), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ProgramUniform1d(GLuint program, GLint location, GLdouble x)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ProgramUniform1d);
   struct marshal_cmd_ProgramUniform1d *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ProgramUniform1d, cmd_size);
   cmd->program = program;
   cmd->location = location;
   cmd->x = x;
}


/* ProgramUniform2d: marshalled asynchronously */
struct marshal_cmd_ProgramUniform2d
{
   struct marshal_cmd_base cmd_base;
   GLuint program;
   GLint location;
   GLdouble x;
   GLdouble y;
};
uint32_t
_mesa_unmarshal_ProgramUniform2d(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform2d *restrict cmd)
{
   GLuint program = cmd->program;
   GLint location = cmd->location;
   GLdouble x = cmd->x;
   GLdouble y = cmd->y;
   CALL_ProgramUniform2d(ctx->Dispatch.Current, (program, location, x, y));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_ProgramUniform2d), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ProgramUniform2d(GLuint program, GLint location, GLdouble x, GLdouble y)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ProgramUniform2d);
   struct marshal_cmd_ProgramUniform2d *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ProgramUniform2d, cmd_size);
   cmd->program = program;
   cmd->location = location;
   cmd->x = x;
   cmd->y = y;
}


/* ProgramUniform3d: marshalled asynchronously */
struct marshal_cmd_ProgramUniform3d
{
   struct marshal_cmd_base cmd_base;
   GLuint program;
   GLint location;
   GLdouble x;
   GLdouble y;
   GLdouble z;
};
uint32_t
_mesa_unmarshal_ProgramUniform3d(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform3d *restrict cmd)
{
   GLuint program = cmd->program;
   GLint location = cmd->location;
   GLdouble x = cmd->x;
   GLdouble y = cmd->y;
   GLdouble z = cmd->z;
   CALL_ProgramUniform3d(ctx->Dispatch.Current, (program, location, x, y, z));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_ProgramUniform3d), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ProgramUniform3d(GLuint program, GLint location, GLdouble x, GLdouble y, GLdouble z)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ProgramUniform3d);
   struct marshal_cmd_ProgramUniform3d *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ProgramUniform3d, cmd_size);
   cmd->program = program;
   cmd->location = location;
   cmd->x = x;
   cmd->y = y;
   cmd->z = z;
}


/* ProgramUniform4d: marshalled asynchronously */
struct marshal_cmd_ProgramUniform4d
{
   struct marshal_cmd_base cmd_base;
   GLuint program;
   GLint location;
   GLdouble x;
   GLdouble y;
   GLdouble z;
   GLdouble w;
};
uint32_t
_mesa_unmarshal_ProgramUniform4d(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform4d *restrict cmd)
{
   GLuint program = cmd->program;
   GLint location = cmd->location;
   GLdouble x = cmd->x;
   GLdouble y = cmd->y;
   GLdouble z = cmd->z;
   GLdouble w = cmd->w;
   CALL_ProgramUniform4d(ctx->Dispatch.Current, (program, location, x, y, z, w));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_ProgramUniform4d), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ProgramUniform4d(GLuint program, GLint location, GLdouble x, GLdouble y, GLdouble z, GLdouble w)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ProgramUniform4d);
   struct marshal_cmd_ProgramUniform4d *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ProgramUniform4d, cmd_size);
   cmd->program = program;
   cmd->location = location;
   cmd->x = x;
   cmd->y = y;
   cmd->z = z;
   cmd->w = w;
}


/* ProgramUniform1dv: marshalled asynchronously */
struct marshal_cmd_ProgramUniform1dv
{
   struct marshal_cmd_base cmd_base;
   GLuint program;
   GLint location;
   GLsizei count;
   /* Next safe_mul(count, 1 * sizeof(GLdouble)) bytes are GLdouble value[count] */
};
uint32_t
_mesa_unmarshal_ProgramUniform1dv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform1dv *restrict cmd)
{
   GLuint program = cmd->program;
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLdouble *value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLdouble *) variable_data;
   CALL_ProgramUniform1dv(ctx->Dispatch.Current, (program, location, count, value));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ProgramUniform1dv(GLuint program, GLint location, GLsizei count, const GLdouble *value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 1 * sizeof(GLdouble));
   int cmd_size = sizeof(struct marshal_cmd_ProgramUniform1dv) + value_size;
   struct marshal_cmd_ProgramUniform1dv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "ProgramUniform1dv");
      CALL_ProgramUniform1dv(ctx->Dispatch.Current, (program, location, count, value));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ProgramUniform1dv, cmd_size);
   cmd->program = program;
   cmd->location = location;
   cmd->count = count;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, value, value_size);
}


/* ProgramUniform2dv: marshalled asynchronously */
struct marshal_cmd_ProgramUniform2dv
{
   struct marshal_cmd_base cmd_base;
   GLuint program;
   GLint location;
   GLsizei count;
   /* Next safe_mul(count, 2 * sizeof(GLdouble)) bytes are GLdouble value[count][2] */
};
uint32_t
_mesa_unmarshal_ProgramUniform2dv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform2dv *restrict cmd)
{
   GLuint program = cmd->program;
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLdouble *value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLdouble *) variable_data;
   CALL_ProgramUniform2dv(ctx->Dispatch.Current, (program, location, count, value));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ProgramUniform2dv(GLuint program, GLint location, GLsizei count, const GLdouble *value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 2 * sizeof(GLdouble));
   int cmd_size = sizeof(struct marshal_cmd_ProgramUniform2dv) + value_size;
   struct marshal_cmd_ProgramUniform2dv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "ProgramUniform2dv");
      CALL_ProgramUniform2dv(ctx->Dispatch.Current, (program, location, count, value));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ProgramUniform2dv, cmd_size);
   cmd->program = program;
   cmd->location = location;
   cmd->count = count;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, value, value_size);
}


/* ProgramUniform3dv: marshalled asynchronously */
struct marshal_cmd_ProgramUniform3dv
{
   struct marshal_cmd_base cmd_base;
   GLuint program;
   GLint location;
   GLsizei count;
   /* Next safe_mul(count, 3 * sizeof(GLdouble)) bytes are GLdouble value[count][3] */
};
uint32_t
_mesa_unmarshal_ProgramUniform3dv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform3dv *restrict cmd)
{
   GLuint program = cmd->program;
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLdouble *value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLdouble *) variable_data;
   CALL_ProgramUniform3dv(ctx->Dispatch.Current, (program, location, count, value));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ProgramUniform3dv(GLuint program, GLint location, GLsizei count, const GLdouble *value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 3 * sizeof(GLdouble));
   int cmd_size = sizeof(struct marshal_cmd_ProgramUniform3dv) + value_size;
   struct marshal_cmd_ProgramUniform3dv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "ProgramUniform3dv");
      CALL_ProgramUniform3dv(ctx->Dispatch.Current, (program, location, count, value));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ProgramUniform3dv, cmd_size);
   cmd->program = program;
   cmd->location = location;
   cmd->count = count;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, value, value_size);
}


/* ProgramUniform4dv: marshalled asynchronously */
struct marshal_cmd_ProgramUniform4dv
{
   struct marshal_cmd_base cmd_base;
   GLuint program;
   GLint location;
   GLsizei count;
   /* Next safe_mul(count, 4 * sizeof(GLdouble)) bytes are GLdouble value[count][4] */
};
uint32_t
_mesa_unmarshal_ProgramUniform4dv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform4dv *restrict cmd)
{
   GLuint program = cmd->program;
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLdouble *value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLdouble *) variable_data;
   CALL_ProgramUniform4dv(ctx->Dispatch.Current, (program, location, count, value));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ProgramUniform4dv(GLuint program, GLint location, GLsizei count, const GLdouble *value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 4 * sizeof(GLdouble));
   int cmd_size = sizeof(struct marshal_cmd_ProgramUniform4dv) + value_size;
   struct marshal_cmd_ProgramUniform4dv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "ProgramUniform4dv");
      CALL_ProgramUniform4dv(ctx->Dispatch.Current, (program, location, count, value));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ProgramUniform4dv, cmd_size);
   cmd->program = program;
   cmd->location = location;
   cmd->count = count;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, value, value_size);
}


/* ProgramUniformMatrix2dv: marshalled asynchronously */
struct marshal_cmd_ProgramUniformMatrix2dv
{
   struct marshal_cmd_base cmd_base;
   GLboolean transpose;
   GLuint program;
   GLint location;
   GLsizei count;
   /* Next safe_mul(count, 4 * sizeof(GLdouble)) bytes are GLdouble value[count][4] */
};
uint32_t
_mesa_unmarshal_ProgramUniformMatrix2dv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniformMatrix2dv *restrict cmd)
{
   GLuint program = cmd->program;
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLboolean transpose = cmd->transpose;
   GLdouble *value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLdouble *) variable_data;
   CALL_ProgramUniformMatrix2dv(ctx->Dispatch.Current, (program, location, count, transpose, value));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ProgramUniformMatrix2dv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 4 * sizeof(GLdouble));
   int cmd_size = sizeof(struct marshal_cmd_ProgramUniformMatrix2dv) + value_size;
   struct marshal_cmd_ProgramUniformMatrix2dv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "ProgramUniformMatrix2dv");
      CALL_ProgramUniformMatrix2dv(ctx->Dispatch.Current, (program, location, count, transpose, value));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ProgramUniformMatrix2dv, cmd_size);
   cmd->program = program;
   cmd->location = location;
   cmd->count = count;
   cmd->transpose = transpose;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, value, value_size);
}


/* ProgramUniformMatrix3dv: marshalled asynchronously */
struct marshal_cmd_ProgramUniformMatrix3dv
{
   struct marshal_cmd_base cmd_base;
   GLboolean transpose;
   GLuint program;
   GLint location;
   GLsizei count;
   /* Next safe_mul(count, 9 * sizeof(GLdouble)) bytes are GLdouble value[count][9] */
};
uint32_t
_mesa_unmarshal_ProgramUniformMatrix3dv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniformMatrix3dv *restrict cmd)
{
   GLuint program = cmd->program;
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLboolean transpose = cmd->transpose;
   GLdouble *value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLdouble *) variable_data;
   CALL_ProgramUniformMatrix3dv(ctx->Dispatch.Current, (program, location, count, transpose, value));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ProgramUniformMatrix3dv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 9 * sizeof(GLdouble));
   int cmd_size = sizeof(struct marshal_cmd_ProgramUniformMatrix3dv) + value_size;
   struct marshal_cmd_ProgramUniformMatrix3dv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "ProgramUniformMatrix3dv");
      CALL_ProgramUniformMatrix3dv(ctx->Dispatch.Current, (program, location, count, transpose, value));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ProgramUniformMatrix3dv, cmd_size);
   cmd->program = program;
   cmd->location = location;
   cmd->count = count;
   cmd->transpose = transpose;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, value, value_size);
}


/* ProgramUniformMatrix4dv: marshalled asynchronously */
struct marshal_cmd_ProgramUniformMatrix4dv
{
   struct marshal_cmd_base cmd_base;
   GLboolean transpose;
   GLuint program;
   GLint location;
   GLsizei count;
   /* Next safe_mul(count, 16 * sizeof(GLdouble)) bytes are GLdouble value[count][16] */
};
uint32_t
_mesa_unmarshal_ProgramUniformMatrix4dv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniformMatrix4dv *restrict cmd)
{
   GLuint program = cmd->program;
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLboolean transpose = cmd->transpose;
   GLdouble *value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLdouble *) variable_data;
   CALL_ProgramUniformMatrix4dv(ctx->Dispatch.Current, (program, location, count, transpose, value));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ProgramUniformMatrix4dv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 16 * sizeof(GLdouble));
   int cmd_size = sizeof(struct marshal_cmd_ProgramUniformMatrix4dv) + value_size;
   struct marshal_cmd_ProgramUniformMatrix4dv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "ProgramUniformMatrix4dv");
      CALL_ProgramUniformMatrix4dv(ctx->Dispatch.Current, (program, location, count, transpose, value));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ProgramUniformMatrix4dv, cmd_size);
   cmd->program = program;
   cmd->location = location;
   cmd->count = count;
   cmd->transpose = transpose;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, value, value_size);
}


/* ProgramUniformMatrix2x3dv: marshalled asynchronously */
struct marshal_cmd_ProgramUniformMatrix2x3dv
{
   struct marshal_cmd_base cmd_base;
   GLboolean transpose;
   GLuint program;
   GLint location;
   GLsizei count;
   /* Next safe_mul(count, 6 * sizeof(GLdouble)) bytes are GLdouble value[count][6] */
};
uint32_t
_mesa_unmarshal_ProgramUniformMatrix2x3dv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniformMatrix2x3dv *restrict cmd)
{
   GLuint program = cmd->program;
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLboolean transpose = cmd->transpose;
   GLdouble *value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLdouble *) variable_data;
   CALL_ProgramUniformMatrix2x3dv(ctx->Dispatch.Current, (program, location, count, transpose, value));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ProgramUniformMatrix2x3dv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 6 * sizeof(GLdouble));
   int cmd_size = sizeof(struct marshal_cmd_ProgramUniformMatrix2x3dv) + value_size;
   struct marshal_cmd_ProgramUniformMatrix2x3dv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "ProgramUniformMatrix2x3dv");
      CALL_ProgramUniformMatrix2x3dv(ctx->Dispatch.Current, (program, location, count, transpose, value));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ProgramUniformMatrix2x3dv, cmd_size);
   cmd->program = program;
   cmd->location = location;
   cmd->count = count;
   cmd->transpose = transpose;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, value, value_size);
}


/* ProgramUniformMatrix2x4dv: marshalled asynchronously */
struct marshal_cmd_ProgramUniformMatrix2x4dv
{
   struct marshal_cmd_base cmd_base;
   GLboolean transpose;
   GLuint program;
   GLint location;
   GLsizei count;
   /* Next safe_mul(count, 8 * sizeof(GLdouble)) bytes are GLdouble value[count][8] */
};
uint32_t
_mesa_unmarshal_ProgramUniformMatrix2x4dv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniformMatrix2x4dv *restrict cmd)
{
   GLuint program = cmd->program;
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLboolean transpose = cmd->transpose;
   GLdouble *value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLdouble *) variable_data;
   CALL_ProgramUniformMatrix2x4dv(ctx->Dispatch.Current, (program, location, count, transpose, value));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ProgramUniformMatrix2x4dv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 8 * sizeof(GLdouble));
   int cmd_size = sizeof(struct marshal_cmd_ProgramUniformMatrix2x4dv) + value_size;
   struct marshal_cmd_ProgramUniformMatrix2x4dv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "ProgramUniformMatrix2x4dv");
      CALL_ProgramUniformMatrix2x4dv(ctx->Dispatch.Current, (program, location, count, transpose, value));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ProgramUniformMatrix2x4dv, cmd_size);
   cmd->program = program;
   cmd->location = location;
   cmd->count = count;
   cmd->transpose = transpose;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, value, value_size);
}


/* ProgramUniformMatrix3x2dv: marshalled asynchronously */
struct marshal_cmd_ProgramUniformMatrix3x2dv
{
   struct marshal_cmd_base cmd_base;
   GLboolean transpose;
   GLuint program;
   GLint location;
   GLsizei count;
   /* Next safe_mul(count, 6 * sizeof(GLdouble)) bytes are GLdouble value[count][6] */
};
uint32_t
_mesa_unmarshal_ProgramUniformMatrix3x2dv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniformMatrix3x2dv *restrict cmd)
{
   GLuint program = cmd->program;
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLboolean transpose = cmd->transpose;
   GLdouble *value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLdouble *) variable_data;
   CALL_ProgramUniformMatrix3x2dv(ctx->Dispatch.Current, (program, location, count, transpose, value));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ProgramUniformMatrix3x2dv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 6 * sizeof(GLdouble));
   int cmd_size = sizeof(struct marshal_cmd_ProgramUniformMatrix3x2dv) + value_size;
   struct marshal_cmd_ProgramUniformMatrix3x2dv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "ProgramUniformMatrix3x2dv");
      CALL_ProgramUniformMatrix3x2dv(ctx->Dispatch.Current, (program, location, count, transpose, value));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ProgramUniformMatrix3x2dv, cmd_size);
   cmd->program = program;
   cmd->location = location;
   cmd->count = count;
   cmd->transpose = transpose;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, value, value_size);
}


/* ProgramUniformMatrix3x4dv: marshalled asynchronously */
struct marshal_cmd_ProgramUniformMatrix3x4dv
{
   struct marshal_cmd_base cmd_base;
   GLboolean transpose;
   GLuint program;
   GLint location;
   GLsizei count;
   /* Next safe_mul(count, 12 * sizeof(GLdouble)) bytes are GLdouble value[count][12] */
};
uint32_t
_mesa_unmarshal_ProgramUniformMatrix3x4dv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniformMatrix3x4dv *restrict cmd)
{
   GLuint program = cmd->program;
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLboolean transpose = cmd->transpose;
   GLdouble *value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLdouble *) variable_data;
   CALL_ProgramUniformMatrix3x4dv(ctx->Dispatch.Current, (program, location, count, transpose, value));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ProgramUniformMatrix3x4dv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 12 * sizeof(GLdouble));
   int cmd_size = sizeof(struct marshal_cmd_ProgramUniformMatrix3x4dv) + value_size;
   struct marshal_cmd_ProgramUniformMatrix3x4dv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "ProgramUniformMatrix3x4dv");
      CALL_ProgramUniformMatrix3x4dv(ctx->Dispatch.Current, (program, location, count, transpose, value));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ProgramUniformMatrix3x4dv, cmd_size);
   cmd->program = program;
   cmd->location = location;
   cmd->count = count;
   cmd->transpose = transpose;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, value, value_size);
}


/* ProgramUniformMatrix4x2dv: marshalled asynchronously */
struct marshal_cmd_ProgramUniformMatrix4x2dv
{
   struct marshal_cmd_base cmd_base;
   GLboolean transpose;
   GLuint program;
   GLint location;
   GLsizei count;
   /* Next safe_mul(count, 8 * sizeof(GLdouble)) bytes are GLdouble value[count][8] */
};
uint32_t
_mesa_unmarshal_ProgramUniformMatrix4x2dv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniformMatrix4x2dv *restrict cmd)
{
   GLuint program = cmd->program;
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLboolean transpose = cmd->transpose;
   GLdouble *value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLdouble *) variable_data;
   CALL_ProgramUniformMatrix4x2dv(ctx->Dispatch.Current, (program, location, count, transpose, value));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ProgramUniformMatrix4x2dv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 8 * sizeof(GLdouble));
   int cmd_size = sizeof(struct marshal_cmd_ProgramUniformMatrix4x2dv) + value_size;
   struct marshal_cmd_ProgramUniformMatrix4x2dv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "ProgramUniformMatrix4x2dv");
      CALL_ProgramUniformMatrix4x2dv(ctx->Dispatch.Current, (program, location, count, transpose, value));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ProgramUniformMatrix4x2dv, cmd_size);
   cmd->program = program;
   cmd->location = location;
   cmd->count = count;
   cmd->transpose = transpose;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, value, value_size);
}


/* ProgramUniformMatrix4x3dv: marshalled asynchronously */
struct marshal_cmd_ProgramUniformMatrix4x3dv
{
   struct marshal_cmd_base cmd_base;
   GLboolean transpose;
   GLuint program;
   GLint location;
   GLsizei count;
   /* Next safe_mul(count, 12 * sizeof(GLdouble)) bytes are GLdouble value[count][12] */
};
uint32_t
_mesa_unmarshal_ProgramUniformMatrix4x3dv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniformMatrix4x3dv *restrict cmd)
{
   GLuint program = cmd->program;
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLboolean transpose = cmd->transpose;
   GLdouble *value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLdouble *) variable_data;
   CALL_ProgramUniformMatrix4x3dv(ctx->Dispatch.Current, (program, location, count, transpose, value));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ProgramUniformMatrix4x3dv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 12 * sizeof(GLdouble));
   int cmd_size = sizeof(struct marshal_cmd_ProgramUniformMatrix4x3dv) + value_size;
   struct marshal_cmd_ProgramUniformMatrix4x3dv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "ProgramUniformMatrix4x3dv");
      CALL_ProgramUniformMatrix4x3dv(ctx->Dispatch.Current, (program, location, count, transpose, value));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ProgramUniformMatrix4x3dv, cmd_size);
   cmd->program = program;
   cmd->location = location;
   cmd->count = count;
   cmd->transpose = transpose;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, value, value_size);
}


/* DrawTransformFeedbackStream: marshalled asynchronously */
struct marshal_cmd_DrawTransformFeedbackStream
{
   struct marshal_cmd_base cmd_base;
   GLenum16 mode;
   GLuint id;
   GLuint stream;
};
uint32_t
_mesa_unmarshal_DrawTransformFeedbackStream(struct gl_context *ctx, const struct marshal_cmd_DrawTransformFeedbackStream *restrict cmd)
{
   GLenum mode = cmd->mode;
   GLuint id = cmd->id;
   GLuint stream = cmd->stream;
   CALL_DrawTransformFeedbackStream(ctx->Dispatch.Current, (mode, id, stream));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_DrawTransformFeedbackStream), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_DrawTransformFeedbackStream(GLenum mode, GLuint id, GLuint stream)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_DrawTransformFeedbackStream);
   struct marshal_cmd_DrawTransformFeedbackStream *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_DrawTransformFeedbackStream, cmd_size);
   cmd->mode = MIN2(mode, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->id = id;
   cmd->stream = stream;
}


/* BeginQueryIndexed: marshalled asynchronously */
struct marshal_cmd_BeginQueryIndexed
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLuint index;
   GLuint id;
};
uint32_t
_mesa_unmarshal_BeginQueryIndexed(struct gl_context *ctx, const struct marshal_cmd_BeginQueryIndexed *restrict cmd)
{
   GLenum target = cmd->target;
   GLuint index = cmd->index;
   GLuint id = cmd->id;
   CALL_BeginQueryIndexed(ctx->Dispatch.Current, (target, index, id));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_BeginQueryIndexed), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_BeginQueryIndexed(GLenum target, GLuint index, GLuint id)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_BeginQueryIndexed);
   struct marshal_cmd_BeginQueryIndexed *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_BeginQueryIndexed, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->index = index;
   cmd->id = id;
}


/* EndQueryIndexed: marshalled asynchronously */
struct marshal_cmd_EndQueryIndexed
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLuint index;
};
uint32_t
_mesa_unmarshal_EndQueryIndexed(struct gl_context *ctx, const struct marshal_cmd_EndQueryIndexed *restrict cmd)
{
   GLenum target = cmd->target;
   GLuint index = cmd->index;
   CALL_EndQueryIndexed(ctx->Dispatch.Current, (target, index));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_EndQueryIndexed), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_EndQueryIndexed(GLenum target, GLuint index)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_EndQueryIndexed);
   struct marshal_cmd_EndQueryIndexed *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_EndQueryIndexed, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->index = index;
}


/* GetQueryIndexediv: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetQueryIndexediv(GLenum target, GLuint index, GLenum pname, GLint *params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetQueryIndexediv");
   CALL_GetQueryIndexediv(ctx->Dispatch.Current, (target, index, pname, params));
}


/* UseProgramStages: marshalled asynchronously */
struct marshal_cmd_UseProgramStages
{
   struct marshal_cmd_base cmd_base;
   GLuint pipeline;
   GLbitfield stages;
   GLuint program;
};
uint32_t
_mesa_unmarshal_UseProgramStages(struct gl_context *ctx, const struct marshal_cmd_UseProgramStages *restrict cmd)
{
   GLuint pipeline = cmd->pipeline;
   GLbitfield stages = cmd->stages;
   GLuint program = cmd->program;
   CALL_UseProgramStages(ctx->Dispatch.Current, (pipeline, stages, program));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_UseProgramStages), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_UseProgramStages(GLuint pipeline, GLbitfield stages, GLuint program)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_UseProgramStages);
   struct marshal_cmd_UseProgramStages *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_UseProgramStages, cmd_size);
   cmd->pipeline = pipeline;
   cmd->stages = stages;
   cmd->program = program;
}


/* ActiveShaderProgram: marshalled asynchronously */
struct marshal_cmd_ActiveShaderProgram
{
   struct marshal_cmd_base cmd_base;
   GLuint pipeline;
   GLuint program;
};
uint32_t
_mesa_unmarshal_ActiveShaderProgram(struct gl_context *ctx, const struct marshal_cmd_ActiveShaderProgram *restrict cmd)
{
   GLuint pipeline = cmd->pipeline;
   GLuint program = cmd->program;
   CALL_ActiveShaderProgram(ctx->Dispatch.Current, (pipeline, program));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_ActiveShaderProgram), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ActiveShaderProgram(GLuint pipeline, GLuint program)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ActiveShaderProgram);
   struct marshal_cmd_ActiveShaderProgram *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ActiveShaderProgram, cmd_size);
   cmd->pipeline = pipeline;
   cmd->program = program;
}


/* CreateShaderProgramv: marshalled synchronously */
static GLuint GLAPIENTRY
_mesa_marshal_CreateShaderProgramv(GLenum type, GLsizei count, const GLchar * const *strings)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "CreateShaderProgramv");
   return CALL_CreateShaderProgramv(ctx->Dispatch.Current, (type, count, strings));
}


/* BindProgramPipeline: marshalled asynchronously */
struct marshal_cmd_BindProgramPipeline
{
   struct marshal_cmd_base cmd_base;
   GLuint pipeline;
};
uint32_t
_mesa_unmarshal_BindProgramPipeline(struct gl_context *ctx, const struct marshal_cmd_BindProgramPipeline *restrict cmd)
{
   GLuint pipeline = cmd->pipeline;
   CALL_BindProgramPipeline(ctx->Dispatch.Current, (pipeline));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_BindProgramPipeline), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_BindProgramPipeline(GLuint pipeline)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_BindProgramPipeline);
   struct marshal_cmd_BindProgramPipeline *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_BindProgramPipeline, cmd_size);
   cmd->pipeline = pipeline;
}


/* DeleteProgramPipelines: marshalled asynchronously */
struct marshal_cmd_DeleteProgramPipelines
{
   struct marshal_cmd_base cmd_base;
   GLsizei n;
   /* Next safe_mul(n, 1 * sizeof(GLuint)) bytes are GLuint pipelines[n] */
};
uint32_t
_mesa_unmarshal_DeleteProgramPipelines(struct gl_context *ctx, const struct marshal_cmd_DeleteProgramPipelines *restrict cmd)
{
   GLsizei n = cmd->n;
   GLuint *pipelines;
   const char *variable_data = (const char *) (cmd + 1);
   pipelines = (GLuint *) variable_data;
   CALL_DeleteProgramPipelines(ctx->Dispatch.Current, (n, pipelines));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_DeleteProgramPipelines(GLsizei n, const GLuint *pipelines)
{
   GET_CURRENT_CONTEXT(ctx);
   int pipelines_size = safe_mul(n, 1 * sizeof(GLuint));
   int cmd_size = sizeof(struct marshal_cmd_DeleteProgramPipelines) + pipelines_size;
   struct marshal_cmd_DeleteProgramPipelines *cmd;
   if (unlikely(pipelines_size < 0 || (pipelines_size > 0 && !pipelines) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "DeleteProgramPipelines");
      CALL_DeleteProgramPipelines(ctx->Dispatch.Current, (n, pipelines));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_DeleteProgramPipelines, cmd_size);
   cmd->n = n;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, pipelines, pipelines_size);
}


/* GenProgramPipelines: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GenProgramPipelines(GLsizei n, GLuint *pipelines)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GenProgramPipelines");
   CALL_GenProgramPipelines(ctx->Dispatch.Current, (n, pipelines));
}


/* IsProgramPipeline: marshalled synchronously */
static GLboolean GLAPIENTRY
_mesa_marshal_IsProgramPipeline(GLuint pipeline)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "IsProgramPipeline");
   return CALL_IsProgramPipeline(ctx->Dispatch.Current, (pipeline));
}


/* GetProgramPipelineiv: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetProgramPipelineiv(GLuint pipeline, GLenum pname, GLint *params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetProgramPipelineiv");
   CALL_GetProgramPipelineiv(ctx->Dispatch.Current, (pipeline, pname, params));
}


/* ProgramUniform1i: marshalled asynchronously */
struct marshal_cmd_ProgramUniform1i
{
   struct marshal_cmd_base cmd_base;
   GLuint program;
   GLint location;
   GLint x;
};
uint32_t
_mesa_unmarshal_ProgramUniform1i(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform1i *restrict cmd)
{
   GLuint program = cmd->program;
   GLint location = cmd->location;
   GLint x = cmd->x;
   CALL_ProgramUniform1i(ctx->Dispatch.Current, (program, location, x));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_ProgramUniform1i), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ProgramUniform1i(GLuint program, GLint location, GLint x)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ProgramUniform1i);
   struct marshal_cmd_ProgramUniform1i *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ProgramUniform1i, cmd_size);
   cmd->program = program;
   cmd->location = location;
   cmd->x = x;
}


/* ProgramUniform2i: marshalled asynchronously */
struct marshal_cmd_ProgramUniform2i
{
   struct marshal_cmd_base cmd_base;
   GLuint program;
   GLint location;
   GLint x;
   GLint y;
};
uint32_t
_mesa_unmarshal_ProgramUniform2i(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform2i *restrict cmd)
{
   GLuint program = cmd->program;
   GLint location = cmd->location;
   GLint x = cmd->x;
   GLint y = cmd->y;
   CALL_ProgramUniform2i(ctx->Dispatch.Current, (program, location, x, y));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_ProgramUniform2i), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ProgramUniform2i(GLuint program, GLint location, GLint x, GLint y)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ProgramUniform2i);
   struct marshal_cmd_ProgramUniform2i *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ProgramUniform2i, cmd_size);
   cmd->program = program;
   cmd->location = location;
   cmd->x = x;
   cmd->y = y;
}


/* ProgramUniform3i: marshalled asynchronously */
struct marshal_cmd_ProgramUniform3i
{
   struct marshal_cmd_base cmd_base;
   GLuint program;
   GLint location;
   GLint x;
   GLint y;
   GLint z;
};
uint32_t
_mesa_unmarshal_ProgramUniform3i(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform3i *restrict cmd)
{
   GLuint program = cmd->program;
   GLint location = cmd->location;
   GLint x = cmd->x;
   GLint y = cmd->y;
   GLint z = cmd->z;
   CALL_ProgramUniform3i(ctx->Dispatch.Current, (program, location, x, y, z));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_ProgramUniform3i), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ProgramUniform3i(GLuint program, GLint location, GLint x, GLint y, GLint z)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ProgramUniform3i);
   struct marshal_cmd_ProgramUniform3i *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ProgramUniform3i, cmd_size);
   cmd->program = program;
   cmd->location = location;
   cmd->x = x;
   cmd->y = y;
   cmd->z = z;
}


/* ProgramUniform4i: marshalled asynchronously */
struct marshal_cmd_ProgramUniform4i
{
   struct marshal_cmd_base cmd_base;
   GLuint program;
   GLint location;
   GLint x;
   GLint y;
   GLint z;
   GLint w;
};
uint32_t
_mesa_unmarshal_ProgramUniform4i(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform4i *restrict cmd)
{
   GLuint program = cmd->program;
   GLint location = cmd->location;
   GLint x = cmd->x;
   GLint y = cmd->y;
   GLint z = cmd->z;
   GLint w = cmd->w;
   CALL_ProgramUniform4i(ctx->Dispatch.Current, (program, location, x, y, z, w));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_ProgramUniform4i), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ProgramUniform4i(GLuint program, GLint location, GLint x, GLint y, GLint z, GLint w)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ProgramUniform4i);
   struct marshal_cmd_ProgramUniform4i *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ProgramUniform4i, cmd_size);
   cmd->program = program;
   cmd->location = location;
   cmd->x = x;
   cmd->y = y;
   cmd->z = z;
   cmd->w = w;
}


/* ProgramUniform1ui: marshalled asynchronously */
struct marshal_cmd_ProgramUniform1ui
{
   struct marshal_cmd_base cmd_base;
   GLuint program;
   GLint location;
   GLuint x;
};
uint32_t
_mesa_unmarshal_ProgramUniform1ui(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform1ui *restrict cmd)
{
   GLuint program = cmd->program;
   GLint location = cmd->location;
   GLuint x = cmd->x;
   CALL_ProgramUniform1ui(ctx->Dispatch.Current, (program, location, x));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_ProgramUniform1ui), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ProgramUniform1ui(GLuint program, GLint location, GLuint x)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ProgramUniform1ui);
   struct marshal_cmd_ProgramUniform1ui *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ProgramUniform1ui, cmd_size);
   cmd->program = program;
   cmd->location = location;
   cmd->x = x;
}


/* ProgramUniform2ui: marshalled asynchronously */
struct marshal_cmd_ProgramUniform2ui
{
   struct marshal_cmd_base cmd_base;
   GLuint program;
   GLint location;
   GLuint x;
   GLuint y;
};
uint32_t
_mesa_unmarshal_ProgramUniform2ui(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform2ui *restrict cmd)
{
   GLuint program = cmd->program;
   GLint location = cmd->location;
   GLuint x = cmd->x;
   GLuint y = cmd->y;
   CALL_ProgramUniform2ui(ctx->Dispatch.Current, (program, location, x, y));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_ProgramUniform2ui), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ProgramUniform2ui(GLuint program, GLint location, GLuint x, GLuint y)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ProgramUniform2ui);
   struct marshal_cmd_ProgramUniform2ui *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ProgramUniform2ui, cmd_size);
   cmd->program = program;
   cmd->location = location;
   cmd->x = x;
   cmd->y = y;
}


/* ProgramUniform3ui: marshalled asynchronously */
struct marshal_cmd_ProgramUniform3ui
{
   struct marshal_cmd_base cmd_base;
   GLuint program;
   GLint location;
   GLuint x;
   GLuint y;
   GLuint z;
};
uint32_t
_mesa_unmarshal_ProgramUniform3ui(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform3ui *restrict cmd)
{
   GLuint program = cmd->program;
   GLint location = cmd->location;
   GLuint x = cmd->x;
   GLuint y = cmd->y;
   GLuint z = cmd->z;
   CALL_ProgramUniform3ui(ctx->Dispatch.Current, (program, location, x, y, z));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_ProgramUniform3ui), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ProgramUniform3ui(GLuint program, GLint location, GLuint x, GLuint y, GLuint z)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ProgramUniform3ui);
   struct marshal_cmd_ProgramUniform3ui *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ProgramUniform3ui, cmd_size);
   cmd->program = program;
   cmd->location = location;
   cmd->x = x;
   cmd->y = y;
   cmd->z = z;
}


/* ProgramUniform4ui: marshalled asynchronously */
struct marshal_cmd_ProgramUniform4ui
{
   struct marshal_cmd_base cmd_base;
   GLuint program;
   GLint location;
   GLuint x;
   GLuint y;
   GLuint z;
   GLuint w;
};
uint32_t
_mesa_unmarshal_ProgramUniform4ui(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform4ui *restrict cmd)
{
   GLuint program = cmd->program;
   GLint location = cmd->location;
   GLuint x = cmd->x;
   GLuint y = cmd->y;
   GLuint z = cmd->z;
   GLuint w = cmd->w;
   CALL_ProgramUniform4ui(ctx->Dispatch.Current, (program, location, x, y, z, w));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_ProgramUniform4ui), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ProgramUniform4ui(GLuint program, GLint location, GLuint x, GLuint y, GLuint z, GLuint w)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ProgramUniform4ui);
   struct marshal_cmd_ProgramUniform4ui *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ProgramUniform4ui, cmd_size);
   cmd->program = program;
   cmd->location = location;
   cmd->x = x;
   cmd->y = y;
   cmd->z = z;
   cmd->w = w;
}


/* ProgramUniform1f: marshalled asynchronously */
struct marshal_cmd_ProgramUniform1f
{
   struct marshal_cmd_base cmd_base;
   GLuint program;
   GLint location;
   GLfloat x;
};
uint32_t
_mesa_unmarshal_ProgramUniform1f(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform1f *restrict cmd)
{
   GLuint program = cmd->program;
   GLint location = cmd->location;
   GLfloat x = cmd->x;
   CALL_ProgramUniform1f(ctx->Dispatch.Current, (program, location, x));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_ProgramUniform1f), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ProgramUniform1f(GLuint program, GLint location, GLfloat x)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ProgramUniform1f);
   struct marshal_cmd_ProgramUniform1f *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ProgramUniform1f, cmd_size);
   cmd->program = program;
   cmd->location = location;
   cmd->x = x;
}


/* ProgramUniform2f: marshalled asynchronously */
struct marshal_cmd_ProgramUniform2f
{
   struct marshal_cmd_base cmd_base;
   GLuint program;
   GLint location;
   GLfloat x;
   GLfloat y;
};
uint32_t
_mesa_unmarshal_ProgramUniform2f(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform2f *restrict cmd)
{
   GLuint program = cmd->program;
   GLint location = cmd->location;
   GLfloat x = cmd->x;
   GLfloat y = cmd->y;
   CALL_ProgramUniform2f(ctx->Dispatch.Current, (program, location, x, y));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_ProgramUniform2f), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ProgramUniform2f(GLuint program, GLint location, GLfloat x, GLfloat y)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ProgramUniform2f);
   struct marshal_cmd_ProgramUniform2f *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ProgramUniform2f, cmd_size);
   cmd->program = program;
   cmd->location = location;
   cmd->x = x;
   cmd->y = y;
}


/* ProgramUniform3f: marshalled asynchronously */
struct marshal_cmd_ProgramUniform3f
{
   struct marshal_cmd_base cmd_base;
   GLuint program;
   GLint location;
   GLfloat x;
   GLfloat y;
   GLfloat z;
};
uint32_t
_mesa_unmarshal_ProgramUniform3f(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform3f *restrict cmd)
{
   GLuint program = cmd->program;
   GLint location = cmd->location;
   GLfloat x = cmd->x;
   GLfloat y = cmd->y;
   GLfloat z = cmd->z;
   CALL_ProgramUniform3f(ctx->Dispatch.Current, (program, location, x, y, z));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_ProgramUniform3f), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ProgramUniform3f(GLuint program, GLint location, GLfloat x, GLfloat y, GLfloat z)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ProgramUniform3f);
   struct marshal_cmd_ProgramUniform3f *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ProgramUniform3f, cmd_size);
   cmd->program = program;
   cmd->location = location;
   cmd->x = x;
   cmd->y = y;
   cmd->z = z;
}


/* ProgramUniform4f: marshalled asynchronously */
struct marshal_cmd_ProgramUniform4f
{
   struct marshal_cmd_base cmd_base;
   GLuint program;
   GLint location;
   GLfloat x;
   GLfloat y;
   GLfloat z;
   GLfloat w;
};
uint32_t
_mesa_unmarshal_ProgramUniform4f(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform4f *restrict cmd)
{
   GLuint program = cmd->program;
   GLint location = cmd->location;
   GLfloat x = cmd->x;
   GLfloat y = cmd->y;
   GLfloat z = cmd->z;
   GLfloat w = cmd->w;
   CALL_ProgramUniform4f(ctx->Dispatch.Current, (program, location, x, y, z, w));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_ProgramUniform4f), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ProgramUniform4f(GLuint program, GLint location, GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ProgramUniform4f);
   struct marshal_cmd_ProgramUniform4f *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ProgramUniform4f, cmd_size);
   cmd->program = program;
   cmd->location = location;
   cmd->x = x;
   cmd->y = y;
   cmd->z = z;
   cmd->w = w;
}


/* ProgramUniform1iv: marshalled asynchronously */
struct marshal_cmd_ProgramUniform1iv
{
   struct marshal_cmd_base cmd_base;
   GLuint program;
   GLint location;
   GLsizei count;
   /* Next safe_mul(count, 1 * sizeof(GLint)) bytes are GLint value[count] */
};
uint32_t
_mesa_unmarshal_ProgramUniform1iv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform1iv *restrict cmd)
{
   GLuint program = cmd->program;
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLint *value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLint *) variable_data;
   CALL_ProgramUniform1iv(ctx->Dispatch.Current, (program, location, count, value));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ProgramUniform1iv(GLuint program, GLint location, GLsizei count, const GLint *value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 1 * sizeof(GLint));
   int cmd_size = sizeof(struct marshal_cmd_ProgramUniform1iv) + value_size;
   struct marshal_cmd_ProgramUniform1iv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "ProgramUniform1iv");
      CALL_ProgramUniform1iv(ctx->Dispatch.Current, (program, location, count, value));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ProgramUniform1iv, cmd_size);
   cmd->program = program;
   cmd->location = location;
   cmd->count = count;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, value, value_size);
}


/* ProgramUniform2iv: marshalled asynchronously */
struct marshal_cmd_ProgramUniform2iv
{
   struct marshal_cmd_base cmd_base;
   GLuint program;
   GLint location;
   GLsizei count;
   /* Next safe_mul(count, 2 * sizeof(GLint)) bytes are GLint value[count][2] */
};
uint32_t
_mesa_unmarshal_ProgramUniform2iv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform2iv *restrict cmd)
{
   GLuint program = cmd->program;
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLint *value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLint *) variable_data;
   CALL_ProgramUniform2iv(ctx->Dispatch.Current, (program, location, count, value));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ProgramUniform2iv(GLuint program, GLint location, GLsizei count, const GLint *value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 2 * sizeof(GLint));
   int cmd_size = sizeof(struct marshal_cmd_ProgramUniform2iv) + value_size;
   struct marshal_cmd_ProgramUniform2iv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "ProgramUniform2iv");
      CALL_ProgramUniform2iv(ctx->Dispatch.Current, (program, location, count, value));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ProgramUniform2iv, cmd_size);
   cmd->program = program;
   cmd->location = location;
   cmd->count = count;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, value, value_size);
}


/* ProgramUniform3iv: marshalled asynchronously */
struct marshal_cmd_ProgramUniform3iv
{
   struct marshal_cmd_base cmd_base;
   GLuint program;
   GLint location;
   GLsizei count;
   /* Next safe_mul(count, 3 * sizeof(GLint)) bytes are GLint value[count][3] */
};
uint32_t
_mesa_unmarshal_ProgramUniform3iv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform3iv *restrict cmd)
{
   GLuint program = cmd->program;
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLint *value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLint *) variable_data;
   CALL_ProgramUniform3iv(ctx->Dispatch.Current, (program, location, count, value));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ProgramUniform3iv(GLuint program, GLint location, GLsizei count, const GLint *value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 3 * sizeof(GLint));
   int cmd_size = sizeof(struct marshal_cmd_ProgramUniform3iv) + value_size;
   struct marshal_cmd_ProgramUniform3iv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "ProgramUniform3iv");
      CALL_ProgramUniform3iv(ctx->Dispatch.Current, (program, location, count, value));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ProgramUniform3iv, cmd_size);
   cmd->program = program;
   cmd->location = location;
   cmd->count = count;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, value, value_size);
}


/* ProgramUniform4iv: marshalled asynchronously */
struct marshal_cmd_ProgramUniform4iv
{
   struct marshal_cmd_base cmd_base;
   GLuint program;
   GLint location;
   GLsizei count;
   /* Next safe_mul(count, 4 * sizeof(GLint)) bytes are GLint value[count][4] */
};
uint32_t
_mesa_unmarshal_ProgramUniform4iv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform4iv *restrict cmd)
{
   GLuint program = cmd->program;
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLint *value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLint *) variable_data;
   CALL_ProgramUniform4iv(ctx->Dispatch.Current, (program, location, count, value));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ProgramUniform4iv(GLuint program, GLint location, GLsizei count, const GLint *value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 4 * sizeof(GLint));
   int cmd_size = sizeof(struct marshal_cmd_ProgramUniform4iv) + value_size;
   struct marshal_cmd_ProgramUniform4iv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "ProgramUniform4iv");
      CALL_ProgramUniform4iv(ctx->Dispatch.Current, (program, location, count, value));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ProgramUniform4iv, cmd_size);
   cmd->program = program;
   cmd->location = location;
   cmd->count = count;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, value, value_size);
}


/* ProgramUniform1uiv: marshalled asynchronously */
struct marshal_cmd_ProgramUniform1uiv
{
   struct marshal_cmd_base cmd_base;
   GLuint program;
   GLint location;
   GLsizei count;
   /* Next safe_mul(count, 1 * sizeof(GLuint)) bytes are GLuint value[count] */
};
uint32_t
_mesa_unmarshal_ProgramUniform1uiv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform1uiv *restrict cmd)
{
   GLuint program = cmd->program;
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLuint *value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLuint *) variable_data;
   CALL_ProgramUniform1uiv(ctx->Dispatch.Current, (program, location, count, value));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ProgramUniform1uiv(GLuint program, GLint location, GLsizei count, const GLuint *value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 1 * sizeof(GLuint));
   int cmd_size = sizeof(struct marshal_cmd_ProgramUniform1uiv) + value_size;
   struct marshal_cmd_ProgramUniform1uiv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "ProgramUniform1uiv");
      CALL_ProgramUniform1uiv(ctx->Dispatch.Current, (program, location, count, value));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ProgramUniform1uiv, cmd_size);
   cmd->program = program;
   cmd->location = location;
   cmd->count = count;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, value, value_size);
}


/* ProgramUniform2uiv: marshalled asynchronously */
struct marshal_cmd_ProgramUniform2uiv
{
   struct marshal_cmd_base cmd_base;
   GLuint program;
   GLint location;
   GLsizei count;
   /* Next safe_mul(count, 2 * sizeof(GLuint)) bytes are GLuint value[count][2] */
};
uint32_t
_mesa_unmarshal_ProgramUniform2uiv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform2uiv *restrict cmd)
{
   GLuint program = cmd->program;
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLuint *value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLuint *) variable_data;
   CALL_ProgramUniform2uiv(ctx->Dispatch.Current, (program, location, count, value));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ProgramUniform2uiv(GLuint program, GLint location, GLsizei count, const GLuint *value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 2 * sizeof(GLuint));
   int cmd_size = sizeof(struct marshal_cmd_ProgramUniform2uiv) + value_size;
   struct marshal_cmd_ProgramUniform2uiv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "ProgramUniform2uiv");
      CALL_ProgramUniform2uiv(ctx->Dispatch.Current, (program, location, count, value));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ProgramUniform2uiv, cmd_size);
   cmd->program = program;
   cmd->location = location;
   cmd->count = count;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, value, value_size);
}


/* ProgramUniform3uiv: marshalled asynchronously */
struct marshal_cmd_ProgramUniform3uiv
{
   struct marshal_cmd_base cmd_base;
   GLuint program;
   GLint location;
   GLsizei count;
   /* Next safe_mul(count, 3 * sizeof(GLuint)) bytes are GLuint value[count][3] */
};
uint32_t
_mesa_unmarshal_ProgramUniform3uiv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform3uiv *restrict cmd)
{
   GLuint program = cmd->program;
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLuint *value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLuint *) variable_data;
   CALL_ProgramUniform3uiv(ctx->Dispatch.Current, (program, location, count, value));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ProgramUniform3uiv(GLuint program, GLint location, GLsizei count, const GLuint *value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 3 * sizeof(GLuint));
   int cmd_size = sizeof(struct marshal_cmd_ProgramUniform3uiv) + value_size;
   struct marshal_cmd_ProgramUniform3uiv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "ProgramUniform3uiv");
      CALL_ProgramUniform3uiv(ctx->Dispatch.Current, (program, location, count, value));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ProgramUniform3uiv, cmd_size);
   cmd->program = program;
   cmd->location = location;
   cmd->count = count;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, value, value_size);
}


/* ProgramUniform4uiv: marshalled asynchronously */
struct marshal_cmd_ProgramUniform4uiv
{
   struct marshal_cmd_base cmd_base;
   GLuint program;
   GLint location;
   GLsizei count;
   /* Next safe_mul(count, 4 * sizeof(GLuint)) bytes are GLuint value[count][4] */
};
uint32_t
_mesa_unmarshal_ProgramUniform4uiv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform4uiv *restrict cmd)
{
   GLuint program = cmd->program;
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLuint *value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLuint *) variable_data;
   CALL_ProgramUniform4uiv(ctx->Dispatch.Current, (program, location, count, value));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ProgramUniform4uiv(GLuint program, GLint location, GLsizei count, const GLuint *value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 4 * sizeof(GLuint));
   int cmd_size = sizeof(struct marshal_cmd_ProgramUniform4uiv) + value_size;
   struct marshal_cmd_ProgramUniform4uiv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "ProgramUniform4uiv");
      CALL_ProgramUniform4uiv(ctx->Dispatch.Current, (program, location, count, value));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ProgramUniform4uiv, cmd_size);
   cmd->program = program;
   cmd->location = location;
   cmd->count = count;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, value, value_size);
}


/* ProgramUniform1fv: marshalled asynchronously */
struct marshal_cmd_ProgramUniform1fv
{
   struct marshal_cmd_base cmd_base;
   GLuint program;
   GLint location;
   GLsizei count;
   /* Next safe_mul(count, 1 * sizeof(GLfloat)) bytes are GLfloat value[count] */
};
uint32_t
_mesa_unmarshal_ProgramUniform1fv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform1fv *restrict cmd)
{
   GLuint program = cmd->program;
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLfloat *value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLfloat *) variable_data;
   CALL_ProgramUniform1fv(ctx->Dispatch.Current, (program, location, count, value));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ProgramUniform1fv(GLuint program, GLint location, GLsizei count, const GLfloat *value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 1 * sizeof(GLfloat));
   int cmd_size = sizeof(struct marshal_cmd_ProgramUniform1fv) + value_size;
   struct marshal_cmd_ProgramUniform1fv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "ProgramUniform1fv");
      CALL_ProgramUniform1fv(ctx->Dispatch.Current, (program, location, count, value));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ProgramUniform1fv, cmd_size);
   cmd->program = program;
   cmd->location = location;
   cmd->count = count;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, value, value_size);
}


/* ProgramUniform2fv: marshalled asynchronously */
struct marshal_cmd_ProgramUniform2fv
{
   struct marshal_cmd_base cmd_base;
   GLuint program;
   GLint location;
   GLsizei count;
   /* Next safe_mul(count, 2 * sizeof(GLfloat)) bytes are GLfloat value[count][2] */
};
uint32_t
_mesa_unmarshal_ProgramUniform2fv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform2fv *restrict cmd)
{
   GLuint program = cmd->program;
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLfloat *value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLfloat *) variable_data;
   CALL_ProgramUniform2fv(ctx->Dispatch.Current, (program, location, count, value));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ProgramUniform2fv(GLuint program, GLint location, GLsizei count, const GLfloat *value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 2 * sizeof(GLfloat));
   int cmd_size = sizeof(struct marshal_cmd_ProgramUniform2fv) + value_size;
   struct marshal_cmd_ProgramUniform2fv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "ProgramUniform2fv");
      CALL_ProgramUniform2fv(ctx->Dispatch.Current, (program, location, count, value));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ProgramUniform2fv, cmd_size);
   cmd->program = program;
   cmd->location = location;
   cmd->count = count;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, value, value_size);
}


/* ProgramUniform3fv: marshalled asynchronously */
struct marshal_cmd_ProgramUniform3fv
{
   struct marshal_cmd_base cmd_base;
   GLuint program;
   GLint location;
   GLsizei count;
   /* Next safe_mul(count, 3 * sizeof(GLfloat)) bytes are GLfloat value[count][3] */
};
uint32_t
_mesa_unmarshal_ProgramUniform3fv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform3fv *restrict cmd)
{
   GLuint program = cmd->program;
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLfloat *value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLfloat *) variable_data;
   CALL_ProgramUniform3fv(ctx->Dispatch.Current, (program, location, count, value));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ProgramUniform3fv(GLuint program, GLint location, GLsizei count, const GLfloat *value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 3 * sizeof(GLfloat));
   int cmd_size = sizeof(struct marshal_cmd_ProgramUniform3fv) + value_size;
   struct marshal_cmd_ProgramUniform3fv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "ProgramUniform3fv");
      CALL_ProgramUniform3fv(ctx->Dispatch.Current, (program, location, count, value));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ProgramUniform3fv, cmd_size);
   cmd->program = program;
   cmd->location = location;
   cmd->count = count;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, value, value_size);
}


/* ProgramUniform4fv: marshalled asynchronously */
struct marshal_cmd_ProgramUniform4fv
{
   struct marshal_cmd_base cmd_base;
   GLuint program;
   GLint location;
   GLsizei count;
   /* Next safe_mul(count, 4 * sizeof(GLfloat)) bytes are GLfloat value[count][4] */
};
uint32_t
_mesa_unmarshal_ProgramUniform4fv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform4fv *restrict cmd)
{
   GLuint program = cmd->program;
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLfloat *value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLfloat *) variable_data;
   CALL_ProgramUniform4fv(ctx->Dispatch.Current, (program, location, count, value));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ProgramUniform4fv(GLuint program, GLint location, GLsizei count, const GLfloat *value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 4 * sizeof(GLfloat));
   int cmd_size = sizeof(struct marshal_cmd_ProgramUniform4fv) + value_size;
   struct marshal_cmd_ProgramUniform4fv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "ProgramUniform4fv");
      CALL_ProgramUniform4fv(ctx->Dispatch.Current, (program, location, count, value));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ProgramUniform4fv, cmd_size);
   cmd->program = program;
   cmd->location = location;
   cmd->count = count;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, value, value_size);
}


/* ProgramUniformMatrix2fv: marshalled asynchronously */
struct marshal_cmd_ProgramUniformMatrix2fv
{
   struct marshal_cmd_base cmd_base;
   GLboolean transpose;
   GLuint program;
   GLint location;
   GLsizei count;
   /* Next safe_mul(count, 4 * sizeof(GLfloat)) bytes are GLfloat value[count][4] */
};
uint32_t
_mesa_unmarshal_ProgramUniformMatrix2fv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniformMatrix2fv *restrict cmd)
{
   GLuint program = cmd->program;
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLboolean transpose = cmd->transpose;
   GLfloat *value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLfloat *) variable_data;
   CALL_ProgramUniformMatrix2fv(ctx->Dispatch.Current, (program, location, count, transpose, value));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ProgramUniformMatrix2fv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 4 * sizeof(GLfloat));
   int cmd_size = sizeof(struct marshal_cmd_ProgramUniformMatrix2fv) + value_size;
   struct marshal_cmd_ProgramUniformMatrix2fv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "ProgramUniformMatrix2fv");
      CALL_ProgramUniformMatrix2fv(ctx->Dispatch.Current, (program, location, count, transpose, value));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ProgramUniformMatrix2fv, cmd_size);
   cmd->program = program;
   cmd->location = location;
   cmd->count = count;
   cmd->transpose = transpose;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, value, value_size);
}


/* ProgramUniformMatrix3fv: marshalled asynchronously */
struct marshal_cmd_ProgramUniformMatrix3fv
{
   struct marshal_cmd_base cmd_base;
   GLboolean transpose;
   GLuint program;
   GLint location;
   GLsizei count;
   /* Next safe_mul(count, 9 * sizeof(GLfloat)) bytes are GLfloat value[count][9] */
};
uint32_t
_mesa_unmarshal_ProgramUniformMatrix3fv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniformMatrix3fv *restrict cmd)
{
   GLuint program = cmd->program;
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLboolean transpose = cmd->transpose;
   GLfloat *value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLfloat *) variable_data;
   CALL_ProgramUniformMatrix3fv(ctx->Dispatch.Current, (program, location, count, transpose, value));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ProgramUniformMatrix3fv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 9 * sizeof(GLfloat));
   int cmd_size = sizeof(struct marshal_cmd_ProgramUniformMatrix3fv) + value_size;
   struct marshal_cmd_ProgramUniformMatrix3fv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "ProgramUniformMatrix3fv");
      CALL_ProgramUniformMatrix3fv(ctx->Dispatch.Current, (program, location, count, transpose, value));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ProgramUniformMatrix3fv, cmd_size);
   cmd->program = program;
   cmd->location = location;
   cmd->count = count;
   cmd->transpose = transpose;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, value, value_size);
}


/* ProgramUniformMatrix4fv: marshalled asynchronously */
struct marshal_cmd_ProgramUniformMatrix4fv
{
   struct marshal_cmd_base cmd_base;
   GLboolean transpose;
   GLuint program;
   GLint location;
   GLsizei count;
   /* Next safe_mul(count, 16 * sizeof(GLfloat)) bytes are GLfloat value[count][16] */
};
uint32_t
_mesa_unmarshal_ProgramUniformMatrix4fv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniformMatrix4fv *restrict cmd)
{
   GLuint program = cmd->program;
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLboolean transpose = cmd->transpose;
   GLfloat *value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLfloat *) variable_data;
   CALL_ProgramUniformMatrix4fv(ctx->Dispatch.Current, (program, location, count, transpose, value));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ProgramUniformMatrix4fv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 16 * sizeof(GLfloat));
   int cmd_size = sizeof(struct marshal_cmd_ProgramUniformMatrix4fv) + value_size;
   struct marshal_cmd_ProgramUniformMatrix4fv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "ProgramUniformMatrix4fv");
      CALL_ProgramUniformMatrix4fv(ctx->Dispatch.Current, (program, location, count, transpose, value));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ProgramUniformMatrix4fv, cmd_size);
   cmd->program = program;
   cmd->location = location;
   cmd->count = count;
   cmd->transpose = transpose;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, value, value_size);
}


/* ProgramUniformMatrix2x3fv: marshalled asynchronously */
struct marshal_cmd_ProgramUniformMatrix2x3fv
{
   struct marshal_cmd_base cmd_base;
   GLboolean transpose;
   GLuint program;
   GLint location;
   GLsizei count;
   /* Next safe_mul(count, 6 * sizeof(GLfloat)) bytes are GLfloat value[count][6] */
};
uint32_t
_mesa_unmarshal_ProgramUniformMatrix2x3fv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniformMatrix2x3fv *restrict cmd)
{
   GLuint program = cmd->program;
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLboolean transpose = cmd->transpose;
   GLfloat *value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLfloat *) variable_data;
   CALL_ProgramUniformMatrix2x3fv(ctx->Dispatch.Current, (program, location, count, transpose, value));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ProgramUniformMatrix2x3fv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 6 * sizeof(GLfloat));
   int cmd_size = sizeof(struct marshal_cmd_ProgramUniformMatrix2x3fv) + value_size;
   struct marshal_cmd_ProgramUniformMatrix2x3fv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "ProgramUniformMatrix2x3fv");
      CALL_ProgramUniformMatrix2x3fv(ctx->Dispatch.Current, (program, location, count, transpose, value));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ProgramUniformMatrix2x3fv, cmd_size);
   cmd->program = program;
   cmd->location = location;
   cmd->count = count;
   cmd->transpose = transpose;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, value, value_size);
}


/* ProgramUniformMatrix3x2fv: marshalled asynchronously */
struct marshal_cmd_ProgramUniformMatrix3x2fv
{
   struct marshal_cmd_base cmd_base;
   GLboolean transpose;
   GLuint program;
   GLint location;
   GLsizei count;
   /* Next safe_mul(count, 6 * sizeof(GLfloat)) bytes are GLfloat value[count][6] */
};
uint32_t
_mesa_unmarshal_ProgramUniformMatrix3x2fv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniformMatrix3x2fv *restrict cmd)
{
   GLuint program = cmd->program;
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLboolean transpose = cmd->transpose;
   GLfloat *value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLfloat *) variable_data;
   CALL_ProgramUniformMatrix3x2fv(ctx->Dispatch.Current, (program, location, count, transpose, value));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ProgramUniformMatrix3x2fv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 6 * sizeof(GLfloat));
   int cmd_size = sizeof(struct marshal_cmd_ProgramUniformMatrix3x2fv) + value_size;
   struct marshal_cmd_ProgramUniformMatrix3x2fv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "ProgramUniformMatrix3x2fv");
      CALL_ProgramUniformMatrix3x2fv(ctx->Dispatch.Current, (program, location, count, transpose, value));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ProgramUniformMatrix3x2fv, cmd_size);
   cmd->program = program;
   cmd->location = location;
   cmd->count = count;
   cmd->transpose = transpose;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, value, value_size);
}


/* ProgramUniformMatrix2x4fv: marshalled asynchronously */
struct marshal_cmd_ProgramUniformMatrix2x4fv
{
   struct marshal_cmd_base cmd_base;
   GLboolean transpose;
   GLuint program;
   GLint location;
   GLsizei count;
   /* Next safe_mul(count, 8 * sizeof(GLfloat)) bytes are GLfloat value[count][8] */
};
uint32_t
_mesa_unmarshal_ProgramUniformMatrix2x4fv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniformMatrix2x4fv *restrict cmd)
{
   GLuint program = cmd->program;
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLboolean transpose = cmd->transpose;
   GLfloat *value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLfloat *) variable_data;
   CALL_ProgramUniformMatrix2x4fv(ctx->Dispatch.Current, (program, location, count, transpose, value));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ProgramUniformMatrix2x4fv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 8 * sizeof(GLfloat));
   int cmd_size = sizeof(struct marshal_cmd_ProgramUniformMatrix2x4fv) + value_size;
   struct marshal_cmd_ProgramUniformMatrix2x4fv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "ProgramUniformMatrix2x4fv");
      CALL_ProgramUniformMatrix2x4fv(ctx->Dispatch.Current, (program, location, count, transpose, value));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ProgramUniformMatrix2x4fv, cmd_size);
   cmd->program = program;
   cmd->location = location;
   cmd->count = count;
   cmd->transpose = transpose;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, value, value_size);
}


/* ProgramUniformMatrix4x2fv: marshalled asynchronously */
struct marshal_cmd_ProgramUniformMatrix4x2fv
{
   struct marshal_cmd_base cmd_base;
   GLboolean transpose;
   GLuint program;
   GLint location;
   GLsizei count;
   /* Next safe_mul(count, 8 * sizeof(GLfloat)) bytes are GLfloat value[count][8] */
};
uint32_t
_mesa_unmarshal_ProgramUniformMatrix4x2fv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniformMatrix4x2fv *restrict cmd)
{
   GLuint program = cmd->program;
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLboolean transpose = cmd->transpose;
   GLfloat *value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLfloat *) variable_data;
   CALL_ProgramUniformMatrix4x2fv(ctx->Dispatch.Current, (program, location, count, transpose, value));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ProgramUniformMatrix4x2fv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 8 * sizeof(GLfloat));
   int cmd_size = sizeof(struct marshal_cmd_ProgramUniformMatrix4x2fv) + value_size;
   struct marshal_cmd_ProgramUniformMatrix4x2fv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "ProgramUniformMatrix4x2fv");
      CALL_ProgramUniformMatrix4x2fv(ctx->Dispatch.Current, (program, location, count, transpose, value));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ProgramUniformMatrix4x2fv, cmd_size);
   cmd->program = program;
   cmd->location = location;
   cmd->count = count;
   cmd->transpose = transpose;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, value, value_size);
}


/* ProgramUniformMatrix3x4fv: marshalled asynchronously */
struct marshal_cmd_ProgramUniformMatrix3x4fv
{
   struct marshal_cmd_base cmd_base;
   GLboolean transpose;
   GLuint program;
   GLint location;
   GLsizei count;
   /* Next safe_mul(count, 12 * sizeof(GLfloat)) bytes are GLfloat value[count][12] */
};
uint32_t
_mesa_unmarshal_ProgramUniformMatrix3x4fv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniformMatrix3x4fv *restrict cmd)
{
   GLuint program = cmd->program;
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLboolean transpose = cmd->transpose;
   GLfloat *value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLfloat *) variable_data;
   CALL_ProgramUniformMatrix3x4fv(ctx->Dispatch.Current, (program, location, count, transpose, value));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ProgramUniformMatrix3x4fv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 12 * sizeof(GLfloat));
   int cmd_size = sizeof(struct marshal_cmd_ProgramUniformMatrix3x4fv) + value_size;
   struct marshal_cmd_ProgramUniformMatrix3x4fv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "ProgramUniformMatrix3x4fv");
      CALL_ProgramUniformMatrix3x4fv(ctx->Dispatch.Current, (program, location, count, transpose, value));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ProgramUniformMatrix3x4fv, cmd_size);
   cmd->program = program;
   cmd->location = location;
   cmd->count = count;
   cmd->transpose = transpose;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, value, value_size);
}


/* ProgramUniformMatrix4x3fv: marshalled asynchronously */
struct marshal_cmd_ProgramUniformMatrix4x3fv
{
   struct marshal_cmd_base cmd_base;
   GLboolean transpose;
   GLuint program;
   GLint location;
   GLsizei count;
   /* Next safe_mul(count, 12 * sizeof(GLfloat)) bytes are GLfloat value[count][12] */
};
uint32_t
_mesa_unmarshal_ProgramUniformMatrix4x3fv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniformMatrix4x3fv *restrict cmd)
{
   GLuint program = cmd->program;
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLboolean transpose = cmd->transpose;
   GLfloat *value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLfloat *) variable_data;
   CALL_ProgramUniformMatrix4x3fv(ctx->Dispatch.Current, (program, location, count, transpose, value));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ProgramUniformMatrix4x3fv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 12 * sizeof(GLfloat));
   int cmd_size = sizeof(struct marshal_cmd_ProgramUniformMatrix4x3fv) + value_size;
   struct marshal_cmd_ProgramUniformMatrix4x3fv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "ProgramUniformMatrix4x3fv");
      CALL_ProgramUniformMatrix4x3fv(ctx->Dispatch.Current, (program, location, count, transpose, value));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ProgramUniformMatrix4x3fv, cmd_size);
   cmd->program = program;
   cmd->location = location;
   cmd->count = count;
   cmd->transpose = transpose;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, value, value_size);
}


/* ValidateProgramPipeline: marshalled asynchronously */
struct marshal_cmd_ValidateProgramPipeline
{
   struct marshal_cmd_base cmd_base;
   GLuint pipeline;
};
uint32_t
_mesa_unmarshal_ValidateProgramPipeline(struct gl_context *ctx, const struct marshal_cmd_ValidateProgramPipeline *restrict cmd)
{
   GLuint pipeline = cmd->pipeline;
   CALL_ValidateProgramPipeline(ctx->Dispatch.Current, (pipeline));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_ValidateProgramPipeline), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ValidateProgramPipeline(GLuint pipeline)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ValidateProgramPipeline);
   struct marshal_cmd_ValidateProgramPipeline *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ValidateProgramPipeline, cmd_size);
   cmd->pipeline = pipeline;
}


/* GetProgramPipelineInfoLog: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetProgramPipelineInfoLog(GLuint pipeline, GLsizei bufSize, GLsizei *length, GLchar *infoLog)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetProgramPipelineInfoLog");
   CALL_GetProgramPipelineInfoLog(ctx->Dispatch.Current, (pipeline, bufSize, length, infoLog));
}


/* VertexAttribL1d: marshalled asynchronously */
struct marshal_cmd_VertexAttribL1d
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLdouble x;
};
uint32_t
_mesa_unmarshal_VertexAttribL1d(struct gl_context *ctx, const struct marshal_cmd_VertexAttribL1d *restrict cmd)
{
   GLuint index = cmd->index;
   GLdouble x = cmd->x;
   CALL_VertexAttribL1d(ctx->Dispatch.Current, (index, x));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexAttribL1d), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_VertexAttribL1d(GLuint index, GLdouble x)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttribL1d);
   struct marshal_cmd_VertexAttribL1d *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttribL1d, cmd_size);
   cmd->index = index;
   cmd->x = x;
}


/* VertexAttribL2d: marshalled asynchronously */
struct marshal_cmd_VertexAttribL2d
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLdouble x;
   GLdouble y;
};
uint32_t
_mesa_unmarshal_VertexAttribL2d(struct gl_context *ctx, const struct marshal_cmd_VertexAttribL2d *restrict cmd)
{
   GLuint index = cmd->index;
   GLdouble x = cmd->x;
   GLdouble y = cmd->y;
   CALL_VertexAttribL2d(ctx->Dispatch.Current, (index, x, y));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexAttribL2d), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_VertexAttribL2d(GLuint index, GLdouble x, GLdouble y)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttribL2d);
   struct marshal_cmd_VertexAttribL2d *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttribL2d, cmd_size);
   cmd->index = index;
   cmd->x = x;
   cmd->y = y;
}


/* VertexAttribL3d: marshalled asynchronously */
struct marshal_cmd_VertexAttribL3d
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLdouble x;
   GLdouble y;
   GLdouble z;
};
uint32_t
_mesa_unmarshal_VertexAttribL3d(struct gl_context *ctx, const struct marshal_cmd_VertexAttribL3d *restrict cmd)
{
   GLuint index = cmd->index;
   GLdouble x = cmd->x;
   GLdouble y = cmd->y;
   GLdouble z = cmd->z;
   CALL_VertexAttribL3d(ctx->Dispatch.Current, (index, x, y, z));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexAttribL3d), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_VertexAttribL3d(GLuint index, GLdouble x, GLdouble y, GLdouble z)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttribL3d);
   struct marshal_cmd_VertexAttribL3d *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttribL3d, cmd_size);
   cmd->index = index;
   cmd->x = x;
   cmd->y = y;
   cmd->z = z;
}


void
_mesa_glthread_init_dispatch3(struct gl_context *ctx, struct _glapi_table *table)
{
   if (_mesa_is_desktop_gl(ctx)) {
      SET_BeginQueryIndexed(table, _mesa_marshal_BeginQueryIndexed);
      SET_CompileShaderIncludeARB(table, _mesa_marshal_CompileShaderIncludeARB);
      SET_DeleteNamedStringARB(table, _mesa_marshal_DeleteNamedStringARB);
      SET_DrawTransformFeedbackStream(table, _mesa_marshal_DrawTransformFeedbackStream);
      SET_EndQueryIndexed(table, _mesa_marshal_EndQueryIndexed);
      SET_GetActiveUniformName(table, _mesa_marshal_GetActiveUniformName);
      SET_GetNamedStringARB(table, _mesa_marshal_GetNamedStringARB);
      SET_GetNamedStringivARB(table, _mesa_marshal_GetNamedStringivARB);
      SET_GetQueryIndexediv(table, _mesa_marshal_GetQueryIndexediv);
      SET_IsNamedStringARB(table, _mesa_marshal_IsNamedStringARB);
      SET_NamedStringARB(table, _mesa_marshal_NamedStringARB);
      SET_PatchParameterfv(table, _mesa_marshal_PatchParameterfv);
      SET_ProgramUniform1d(table, _mesa_marshal_ProgramUniform1d);
      SET_ProgramUniform1dv(table, _mesa_marshal_ProgramUniform1dv);
      SET_ProgramUniform2d(table, _mesa_marshal_ProgramUniform2d);
      SET_ProgramUniform2dv(table, _mesa_marshal_ProgramUniform2dv);
      SET_ProgramUniform3d(table, _mesa_marshal_ProgramUniform3d);
      SET_ProgramUniform3dv(table, _mesa_marshal_ProgramUniform3dv);
      SET_ProgramUniform4d(table, _mesa_marshal_ProgramUniform4d);
      SET_ProgramUniform4dv(table, _mesa_marshal_ProgramUniform4dv);
      SET_ProgramUniformMatrix2dv(table, _mesa_marshal_ProgramUniformMatrix2dv);
      SET_ProgramUniformMatrix2x3dv(table, _mesa_marshal_ProgramUniformMatrix2x3dv);
      SET_ProgramUniformMatrix2x4dv(table, _mesa_marshal_ProgramUniformMatrix2x4dv);
      SET_ProgramUniformMatrix3dv(table, _mesa_marshal_ProgramUniformMatrix3dv);
      SET_ProgramUniformMatrix3x2dv(table, _mesa_marshal_ProgramUniformMatrix3x2dv);
      SET_ProgramUniformMatrix3x4dv(table, _mesa_marshal_ProgramUniformMatrix3x4dv);
      SET_ProgramUniformMatrix4dv(table, _mesa_marshal_ProgramUniformMatrix4dv);
      SET_ProgramUniformMatrix4x2dv(table, _mesa_marshal_ProgramUniformMatrix4x2dv);
      SET_ProgramUniformMatrix4x3dv(table, _mesa_marshal_ProgramUniformMatrix4x3dv);
      SET_TexImage2DMultisample(table, _mesa_marshal_TexImage2DMultisample);
      SET_TexImage3DMultisample(table, _mesa_marshal_TexImage3DMultisample);
      SET_VertexArrayVertexAttribDivisorEXT(table, _mesa_marshal_VertexArrayVertexAttribDivisorEXT);
      SET_VertexAttribP1ui(table, _mesa_marshal_VertexAttribP1ui);
      SET_VertexAttribP1uiv(table, _mesa_marshal_VertexAttribP1uiv);
      SET_VertexAttribP2ui(table, _mesa_marshal_VertexAttribP2ui);
      SET_VertexAttribP2uiv(table, _mesa_marshal_VertexAttribP2uiv);
      SET_VertexAttribP3ui(table, _mesa_marshal_VertexAttribP3ui);
      SET_VertexAttribP3uiv(table, _mesa_marshal_VertexAttribP3uiv);
      SET_VertexAttribP4ui(table, _mesa_marshal_VertexAttribP4ui);
      SET_VertexAttribP4uiv(table, _mesa_marshal_VertexAttribP4uiv);
   }
   if (_mesa_is_desktop_gl(ctx) || (_mesa_is_gles2(ctx) && ctx->Version >= 30)) {
      SET_BindFragDataLocationIndexed(table, _mesa_marshal_BindFragDataLocationIndexed);
      SET_BindSampler(table, _mesa_marshal_BindSampler);
      SET_BlendEquationSeparateiARB(table, _mesa_marshal_BlendEquationSeparateiARB);
      SET_BlendEquationiARB(table, _mesa_marshal_BlendEquationiARB);
      SET_BlendFuncSeparateiARB(table, _mesa_marshal_BlendFuncSeparateiARB);
      SET_BlendFunciARB(table, _mesa_marshal_BlendFunciARB);
      SET_BlitFramebuffer(table, _mesa_marshal_BlitFramebuffer);
      SET_ClientWaitSync(table, _mesa_marshal_ClientWaitSync);
      SET_CopyBufferSubData(table, _mesa_marshal_CopyBufferSubData);
      SET_DeleteSamplers(table, _mesa_marshal_DeleteSamplers);
      SET_DeleteSync(table, _mesa_marshal_DeleteSync);
      SET_FenceSync(table, _mesa_marshal_FenceSync);
      SET_GenSamplers(table, _mesa_marshal_GenSamplers);
      SET_GetActiveUniformBlockName(table, _mesa_marshal_GetActiveUniformBlockName);
      SET_GetActiveUniformBlockiv(table, _mesa_marshal_GetActiveUniformBlockiv);
      SET_GetActiveUniformsiv(table, _mesa_marshal_GetActiveUniformsiv);
      SET_GetFragDataIndex(table, _mesa_marshal_GetFragDataIndex);
      SET_GetSamplerParameterIiv(table, _mesa_marshal_GetSamplerParameterIiv);
      SET_GetSamplerParameterIuiv(table, _mesa_marshal_GetSamplerParameterIuiv);
      SET_GetSamplerParameterfv(table, _mesa_marshal_GetSamplerParameterfv);
      SET_GetSamplerParameteriv(table, _mesa_marshal_GetSamplerParameteriv);
      SET_GetSynciv(table, _mesa_marshal_GetSynciv);
      SET_GetUniformBlockIndex(table, _mesa_marshal_GetUniformBlockIndex);
      SET_GetUniformIndices(table, _mesa_marshal_GetUniformIndices);
      SET_IsSampler(table, _mesa_marshal_IsSampler);
      SET_IsSync(table, _mesa_marshal_IsSync);
      SET_MinSampleShading(table, _mesa_marshal_MinSampleShading);
      SET_ProgramUniform1ui(table, _mesa_marshal_ProgramUniform1ui);
      SET_ProgramUniform1uiv(table, _mesa_marshal_ProgramUniform1uiv);
      SET_ProgramUniform2ui(table, _mesa_marshal_ProgramUniform2ui);
      SET_ProgramUniform2uiv(table, _mesa_marshal_ProgramUniform2uiv);
      SET_ProgramUniform3ui(table, _mesa_marshal_ProgramUniform3ui);
      SET_ProgramUniform3uiv(table, _mesa_marshal_ProgramUniform3uiv);
      SET_ProgramUniform4ui(table, _mesa_marshal_ProgramUniform4ui);
      SET_ProgramUniform4uiv(table, _mesa_marshal_ProgramUniform4uiv);
      SET_SamplerParameterIiv(table, _mesa_marshal_SamplerParameterIiv);
      SET_SamplerParameterIuiv(table, _mesa_marshal_SamplerParameterIuiv);
      SET_SamplerParameterf(table, _mesa_marshal_SamplerParameterf);
      SET_SamplerParameterfv(table, _mesa_marshal_SamplerParameterfv);
      SET_SamplerParameteri(table, _mesa_marshal_SamplerParameteri);
      SET_SamplerParameteriv(table, _mesa_marshal_SamplerParameteriv);
      SET_UniformBlockBinding(table, _mesa_marshal_UniformBlockBinding);
      SET_WaitSync(table, _mesa_marshal_WaitSync);
   }
   if (_mesa_is_desktop_gl(ctx) || (_mesa_is_gles2(ctx) && ctx->Version >= 31)) {
      SET_GetMultisamplefv(table, _mesa_marshal_GetMultisamplefv);
      SET_PatchParameteri(table, _mesa_marshal_PatchParameteri);
      SET_SampleMaski(table, _mesa_marshal_SampleMaski);
   }
   if (_mesa_is_desktop_gl(ctx) || _mesa_is_gles1(ctx) || _mesa_is_gles2(ctx)) {
      SET_FlushMappedBufferRange(table, _mesa_marshal_FlushMappedBufferRange);
      SET_FramebufferRenderbuffer(table, _mesa_marshal_FramebufferRenderbuffer);
      SET_GenerateMipmap(table, _mesa_marshal_GenerateMipmap);
      SET_GetFramebufferAttachmentParameteriv(table, _mesa_marshal_GetFramebufferAttachmentParameteriv);
      SET_MapBufferRange(table, _mesa_marshal_MapBufferRange);
   }
   if (_mesa_is_desktop_gl(ctx) || _mesa_is_gles2(ctx)) {
      SET_ActiveShaderProgram(table, _mesa_marshal_ActiveShaderProgram);
      SET_BindProgramPipeline(table, _mesa_marshal_BindProgramPipeline);
      SET_BindVertexArray(table, _mesa_marshal_BindVertexArray);
      SET_CreateShaderProgramv(table, _mesa_marshal_CreateShaderProgramv);
      SET_DeleteProgramPipelines(table, _mesa_marshal_DeleteProgramPipelines);
      SET_DeleteVertexArrays(table, _mesa_marshal_DeleteVertexArrays);
      SET_GenProgramPipelines(table, _mesa_marshal_GenProgramPipelines);
      SET_GenVertexArrays(table, _mesa_marshal_GenVertexArrays);
      SET_GetInteger64v(table, _mesa_marshal_GetInteger64v);
      SET_GetProgramPipelineInfoLog(table, _mesa_marshal_GetProgramPipelineInfoLog);
      SET_GetProgramPipelineiv(table, _mesa_marshal_GetProgramPipelineiv);
      SET_GetQueryObjecti64v(table, _mesa_marshal_GetQueryObjecti64v);
      SET_GetQueryObjectui64v(table, _mesa_marshal_GetQueryObjectui64v);
      SET_IsProgramPipeline(table, _mesa_marshal_IsProgramPipeline);
      SET_IsVertexArray(table, _mesa_marshal_IsVertexArray);
      SET_ProgramUniform1f(table, _mesa_marshal_ProgramUniform1f);
      SET_ProgramUniform1fv(table, _mesa_marshal_ProgramUniform1fv);
      SET_ProgramUniform1i(table, _mesa_marshal_ProgramUniform1i);
      SET_ProgramUniform1iv(table, _mesa_marshal_ProgramUniform1iv);
      SET_ProgramUniform2f(table, _mesa_marshal_ProgramUniform2f);
      SET_ProgramUniform2fv(table, _mesa_marshal_ProgramUniform2fv);
      SET_ProgramUniform2i(table, _mesa_marshal_ProgramUniform2i);
      SET_ProgramUniform2iv(table, _mesa_marshal_ProgramUniform2iv);
      SET_ProgramUniform3f(table, _mesa_marshal_ProgramUniform3f);
      SET_ProgramUniform3fv(table, _mesa_marshal_ProgramUniform3fv);
      SET_ProgramUniform3i(table, _mesa_marshal_ProgramUniform3i);
      SET_ProgramUniform3iv(table, _mesa_marshal_ProgramUniform3iv);
      SET_ProgramUniform4f(table, _mesa_marshal_ProgramUniform4f);
      SET_ProgramUniform4fv(table, _mesa_marshal_ProgramUniform4fv);
      SET_ProgramUniform4i(table, _mesa_marshal_ProgramUniform4i);
      SET_ProgramUniform4iv(table, _mesa_marshal_ProgramUniform4iv);
      SET_ProgramUniformMatrix2fv(table, _mesa_marshal_ProgramUniformMatrix2fv);
      SET_ProgramUniformMatrix2x3fv(table, _mesa_marshal_ProgramUniformMatrix2x3fv);
      SET_ProgramUniformMatrix2x4fv(table, _mesa_marshal_ProgramUniformMatrix2x4fv);
      SET_ProgramUniformMatrix3fv(table, _mesa_marshal_ProgramUniformMatrix3fv);
      SET_ProgramUniformMatrix3x2fv(table, _mesa_marshal_ProgramUniformMatrix3x2fv);
      SET_ProgramUniformMatrix3x4fv(table, _mesa_marshal_ProgramUniformMatrix3x4fv);
      SET_ProgramUniformMatrix4fv(table, _mesa_marshal_ProgramUniformMatrix4fv);
      SET_ProgramUniformMatrix4x2fv(table, _mesa_marshal_ProgramUniformMatrix4x2fv);
      SET_ProgramUniformMatrix4x3fv(table, _mesa_marshal_ProgramUniformMatrix4x3fv);
      SET_QueryCounter(table, _mesa_marshal_QueryCounter);
      SET_UseProgramStages(table, _mesa_marshal_UseProgramStages);
      SET_ValidateProgramPipeline(table, _mesa_marshal_ValidateProgramPipeline);
      SET_VertexAttribDivisor(table, _mesa_marshal_VertexAttribDivisor);
   }
   if (_mesa_is_desktop_gl_compat(ctx)) {
      SET_ColorP3ui(table, _mesa_marshal_ColorP3ui);
      SET_ColorP3uiv(table, _mesa_marshal_ColorP3uiv);
      SET_ColorP4ui(table, _mesa_marshal_ColorP4ui);
      SET_ColorP4uiv(table, _mesa_marshal_ColorP4uiv);
      SET_MultiTexCoordP1ui(table, _mesa_marshal_MultiTexCoordP1ui);
      SET_MultiTexCoordP1uiv(table, _mesa_marshal_MultiTexCoordP1uiv);
      SET_MultiTexCoordP2ui(table, _mesa_marshal_MultiTexCoordP2ui);
      SET_MultiTexCoordP2uiv(table, _mesa_marshal_MultiTexCoordP2uiv);
      SET_MultiTexCoordP3ui(table, _mesa_marshal_MultiTexCoordP3ui);
      SET_MultiTexCoordP3uiv(table, _mesa_marshal_MultiTexCoordP3uiv);
      SET_MultiTexCoordP4ui(table, _mesa_marshal_MultiTexCoordP4ui);
      SET_MultiTexCoordP4uiv(table, _mesa_marshal_MultiTexCoordP4uiv);
      SET_NormalP3ui(table, _mesa_marshal_NormalP3ui);
      SET_NormalP3uiv(table, _mesa_marshal_NormalP3uiv);
      SET_SecondaryColorP3ui(table, _mesa_marshal_SecondaryColorP3ui);
      SET_SecondaryColorP3uiv(table, _mesa_marshal_SecondaryColorP3uiv);
      SET_TexCoordP1ui(table, _mesa_marshal_TexCoordP1ui);
      SET_TexCoordP1uiv(table, _mesa_marshal_TexCoordP1uiv);
      SET_TexCoordP2ui(table, _mesa_marshal_TexCoordP2ui);
      SET_TexCoordP2uiv(table, _mesa_marshal_TexCoordP2uiv);
      SET_TexCoordP3ui(table, _mesa_marshal_TexCoordP3ui);
      SET_TexCoordP3uiv(table, _mesa_marshal_TexCoordP3uiv);
      SET_TexCoordP4ui(table, _mesa_marshal_TexCoordP4ui);
      SET_TexCoordP4uiv(table, _mesa_marshal_TexCoordP4uiv);
      SET_VertexP2ui(table, _mesa_marshal_VertexP2ui);
      SET_VertexP2uiv(table, _mesa_marshal_VertexP2uiv);
      SET_VertexP3ui(table, _mesa_marshal_VertexP3ui);
      SET_VertexP3uiv(table, _mesa_marshal_VertexP3uiv);
      SET_VertexP4ui(table, _mesa_marshal_VertexP4ui);
      SET_VertexP4uiv(table, _mesa_marshal_VertexP4uiv);
   }
   if (_mesa_is_desktop_gl_compat(ctx) || _mesa_is_desktop_gl_core(ctx)) {
      SET_GetActiveSubroutineName(table, _mesa_marshal_GetActiveSubroutineName);
      SET_GetActiveSubroutineUniformName(table, _mesa_marshal_GetActiveSubroutineUniformName);
      SET_GetActiveSubroutineUniformiv(table, _mesa_marshal_GetActiveSubroutineUniformiv);
      SET_GetProgramStageiv(table, _mesa_marshal_GetProgramStageiv);
      SET_GetSubroutineIndex(table, _mesa_marshal_GetSubroutineIndex);
      SET_GetSubroutineUniformLocation(table, _mesa_marshal_GetSubroutineUniformLocation);
      SET_GetUniformSubroutineuiv(table, _mesa_marshal_GetUniformSubroutineuiv);
      SET_GetUniformdv(table, _mesa_marshal_GetUniformdv);
      SET_Uniform1d(table, _mesa_marshal_Uniform1d);
      SET_Uniform1dv(table, _mesa_marshal_Uniform1dv);
      SET_Uniform2d(table, _mesa_marshal_Uniform2d);
      SET_Uniform2dv(table, _mesa_marshal_Uniform2dv);
      SET_Uniform3d(table, _mesa_marshal_Uniform3d);
      SET_Uniform3dv(table, _mesa_marshal_Uniform3dv);
      SET_Uniform4d(table, _mesa_marshal_Uniform4d);
      SET_Uniform4dv(table, _mesa_marshal_Uniform4dv);
      SET_UniformMatrix2dv(table, _mesa_marshal_UniformMatrix2dv);
      SET_UniformMatrix2x3dv(table, _mesa_marshal_UniformMatrix2x3dv);
      SET_UniformMatrix2x4dv(table, _mesa_marshal_UniformMatrix2x4dv);
      SET_UniformMatrix3dv(table, _mesa_marshal_UniformMatrix3dv);
      SET_UniformMatrix3x2dv(table, _mesa_marshal_UniformMatrix3x2dv);
      SET_UniformMatrix3x4dv(table, _mesa_marshal_UniformMatrix3x4dv);
      SET_UniformMatrix4dv(table, _mesa_marshal_UniformMatrix4dv);
      SET_UniformMatrix4x2dv(table, _mesa_marshal_UniformMatrix4x2dv);
      SET_UniformMatrix4x3dv(table, _mesa_marshal_UniformMatrix4x3dv);
      SET_UniformSubroutinesuiv(table, _mesa_marshal_UniformSubroutinesuiv);
      SET_VertexAttribL1d(table, _mesa_marshal_VertexAttribL1d);
      SET_VertexAttribL2d(table, _mesa_marshal_VertexAttribL2d);
      SET_VertexAttribL3d(table, _mesa_marshal_VertexAttribL3d);
   }
   if (_mesa_is_desktop_gl_compat(ctx) || _mesa_is_desktop_gl_core(ctx) || (_mesa_is_gles2(ctx) && ctx->Version >= 31)) {
      SET_TexBuffer(table, _mesa_marshal_TexBuffer);
   }
}
