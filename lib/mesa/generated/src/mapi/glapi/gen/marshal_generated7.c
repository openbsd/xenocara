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

/* MapNamedBufferEXT: marshalled synchronously */
GLvoid * GLAPIENTRY
_mesa_marshal_MapNamedBufferEXT(GLuint buffer, GLenum access)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "MapNamedBufferEXT");
   return CALL_MapNamedBufferEXT(ctx->CurrentServerDispatch, (buffer, access));
}


/* GetNamedBufferSubDataEXT: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetNamedBufferSubDataEXT(GLuint buffer, GLintptr offset, GLsizeiptr size, GLvoid * data)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetNamedBufferSubDataEXT");
   CALL_GetNamedBufferSubDataEXT(ctx->CurrentServerDispatch, (buffer, offset, size, data));
}


/* GetNamedBufferPointervEXT: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetNamedBufferPointervEXT(GLuint buffer, GLenum pname, GLvoid ** params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetNamedBufferPointervEXT");
   CALL_GetNamedBufferPointervEXT(ctx->CurrentServerDispatch, (buffer, pname, params));
}


/* GetNamedBufferParameterivEXT: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetNamedBufferParameterivEXT(GLuint buffer, GLenum pname, GLint * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetNamedBufferParameterivEXT");
   CALL_GetNamedBufferParameterivEXT(ctx->CurrentServerDispatch, (buffer, pname, params));
}


/* FlushMappedNamedBufferRangeEXT: marshalled asynchronously */
struct marshal_cmd_FlushMappedNamedBufferRangeEXT
{
   struct marshal_cmd_base cmd_base;
   GLuint buffer;
   GLintptr offset;
   GLsizeiptr length;
};
void
_mesa_unmarshal_FlushMappedNamedBufferRangeEXT(struct gl_context *ctx, const struct marshal_cmd_FlushMappedNamedBufferRangeEXT *cmd)
{
   const GLuint buffer = cmd->buffer;
   const GLintptr offset = cmd->offset;
   const GLsizeiptr length = cmd->length;
   CALL_FlushMappedNamedBufferRangeEXT(ctx->CurrentServerDispatch, (buffer, offset, length));
}
void GLAPIENTRY
_mesa_marshal_FlushMappedNamedBufferRangeEXT(GLuint buffer, GLintptr offset, GLsizeiptr length)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_FlushMappedNamedBufferRangeEXT);
   struct marshal_cmd_FlushMappedNamedBufferRangeEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_FlushMappedNamedBufferRangeEXT, cmd_size);
   cmd->buffer = buffer;
   cmd->offset = offset;
   cmd->length = length;
}


/* MapNamedBufferRangeEXT: marshalled synchronously */
GLvoid * GLAPIENTRY
_mesa_marshal_MapNamedBufferRangeEXT(GLuint buffer, GLintptr offset, GLsizeiptr length, GLbitfield access)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "MapNamedBufferRangeEXT");
   return CALL_MapNamedBufferRangeEXT(ctx->CurrentServerDispatch, (buffer, offset, length, access));
}


/* FramebufferDrawBufferEXT: marshalled asynchronously */
struct marshal_cmd_FramebufferDrawBufferEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 mode;
   GLuint framebuffer;
};
void
_mesa_unmarshal_FramebufferDrawBufferEXT(struct gl_context *ctx, const struct marshal_cmd_FramebufferDrawBufferEXT *cmd)
{
   const GLuint framebuffer = cmd->framebuffer;
   const GLenum mode = cmd->mode;
   CALL_FramebufferDrawBufferEXT(ctx->CurrentServerDispatch, (framebuffer, mode));
}
void GLAPIENTRY
_mesa_marshal_FramebufferDrawBufferEXT(GLuint framebuffer, GLenum mode)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_FramebufferDrawBufferEXT);
   struct marshal_cmd_FramebufferDrawBufferEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_FramebufferDrawBufferEXT, cmd_size);
   cmd->framebuffer = framebuffer;
   cmd->mode = mode;
}


/* FramebufferDrawBuffersEXT: marshalled asynchronously */
struct marshal_cmd_FramebufferDrawBuffersEXT
{
   struct marshal_cmd_base cmd_base;
   GLuint framebuffer;
   GLsizei n;
   /* Next safe_mul(n, 1 * sizeof(GLenum)) bytes are GLenum bufs[n] */
};
void
_mesa_unmarshal_FramebufferDrawBuffersEXT(struct gl_context *ctx, const struct marshal_cmd_FramebufferDrawBuffersEXT *cmd)
{
   const GLuint framebuffer = cmd->framebuffer;
   const GLsizei n = cmd->n;
   GLenum * bufs;
   const char *variable_data = (const char *) (cmd + 1);
   bufs = (GLenum *) variable_data;
   CALL_FramebufferDrawBuffersEXT(ctx->CurrentServerDispatch, (framebuffer, n, bufs));
}
void GLAPIENTRY
_mesa_marshal_FramebufferDrawBuffersEXT(GLuint framebuffer, GLsizei n, const GLenum * bufs)
{
   GET_CURRENT_CONTEXT(ctx);
   int bufs_size = safe_mul(n, 1 * sizeof(GLenum));
   int cmd_size = sizeof(struct marshal_cmd_FramebufferDrawBuffersEXT) + bufs_size;
   struct marshal_cmd_FramebufferDrawBuffersEXT *cmd;
   if (unlikely(bufs_size < 0 || (bufs_size > 0 && !bufs) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "FramebufferDrawBuffersEXT");
      CALL_FramebufferDrawBuffersEXT(ctx->CurrentServerDispatch, (framebuffer, n, bufs));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_FramebufferDrawBuffersEXT, cmd_size);
   cmd->framebuffer = framebuffer;
   cmd->n = n;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, bufs, bufs_size);
}


/* FramebufferReadBufferEXT: marshalled asynchronously */
struct marshal_cmd_FramebufferReadBufferEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 mode;
   GLuint framebuffer;
};
void
_mesa_unmarshal_FramebufferReadBufferEXT(struct gl_context *ctx, const struct marshal_cmd_FramebufferReadBufferEXT *cmd)
{
   const GLuint framebuffer = cmd->framebuffer;
   const GLenum mode = cmd->mode;
   CALL_FramebufferReadBufferEXT(ctx->CurrentServerDispatch, (framebuffer, mode));
}
void GLAPIENTRY
_mesa_marshal_FramebufferReadBufferEXT(GLuint framebuffer, GLenum mode)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_FramebufferReadBufferEXT);
   struct marshal_cmd_FramebufferReadBufferEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_FramebufferReadBufferEXT, cmd_size);
   cmd->framebuffer = framebuffer;
   cmd->mode = mode;
}


/* GetFramebufferParameterivEXT: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetFramebufferParameterivEXT(GLuint framebuffer, GLenum pname, GLint * param)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetFramebufferParameterivEXT");
   CALL_GetFramebufferParameterivEXT(ctx->CurrentServerDispatch, (framebuffer, pname, param));
}


/* CheckNamedFramebufferStatusEXT: marshalled synchronously */
GLenum GLAPIENTRY
_mesa_marshal_CheckNamedFramebufferStatusEXT(GLuint framebuffer, GLenum target)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "CheckNamedFramebufferStatusEXT");
   return CALL_CheckNamedFramebufferStatusEXT(ctx->CurrentServerDispatch, (framebuffer, target));
}


/* NamedFramebufferTexture1DEXT: marshalled asynchronously */
struct marshal_cmd_NamedFramebufferTexture1DEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 attachment;
   GLenum16 textarget;
   GLuint framebuffer;
   GLuint texture;
   GLint level;
};
void
_mesa_unmarshal_NamedFramebufferTexture1DEXT(struct gl_context *ctx, const struct marshal_cmd_NamedFramebufferTexture1DEXT *cmd)
{
   const GLuint framebuffer = cmd->framebuffer;
   const GLenum attachment = cmd->attachment;
   const GLenum textarget = cmd->textarget;
   const GLuint texture = cmd->texture;
   const GLint level = cmd->level;
   CALL_NamedFramebufferTexture1DEXT(ctx->CurrentServerDispatch, (framebuffer, attachment, textarget, texture, level));
}
void GLAPIENTRY
_mesa_marshal_NamedFramebufferTexture1DEXT(GLuint framebuffer, GLenum attachment, GLenum textarget, GLuint texture, GLint level)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_NamedFramebufferTexture1DEXT);
   struct marshal_cmd_NamedFramebufferTexture1DEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_NamedFramebufferTexture1DEXT, cmd_size);
   cmd->framebuffer = framebuffer;
   cmd->attachment = attachment;
   cmd->textarget = textarget;
   cmd->texture = texture;
   cmd->level = level;
}


/* NamedFramebufferTexture2DEXT: marshalled asynchronously */
struct marshal_cmd_NamedFramebufferTexture2DEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 attachment;
   GLenum16 textarget;
   GLuint framebuffer;
   GLuint texture;
   GLint level;
};
void
_mesa_unmarshal_NamedFramebufferTexture2DEXT(struct gl_context *ctx, const struct marshal_cmd_NamedFramebufferTexture2DEXT *cmd)
{
   const GLuint framebuffer = cmd->framebuffer;
   const GLenum attachment = cmd->attachment;
   const GLenum textarget = cmd->textarget;
   const GLuint texture = cmd->texture;
   const GLint level = cmd->level;
   CALL_NamedFramebufferTexture2DEXT(ctx->CurrentServerDispatch, (framebuffer, attachment, textarget, texture, level));
}
void GLAPIENTRY
_mesa_marshal_NamedFramebufferTexture2DEXT(GLuint framebuffer, GLenum attachment, GLenum textarget, GLuint texture, GLint level)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_NamedFramebufferTexture2DEXT);
   struct marshal_cmd_NamedFramebufferTexture2DEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_NamedFramebufferTexture2DEXT, cmd_size);
   cmd->framebuffer = framebuffer;
   cmd->attachment = attachment;
   cmd->textarget = textarget;
   cmd->texture = texture;
   cmd->level = level;
}


/* NamedFramebufferTexture3DEXT: marshalled asynchronously */
struct marshal_cmd_NamedFramebufferTexture3DEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 attachment;
   GLenum16 textarget;
   GLuint framebuffer;
   GLuint texture;
   GLint level;
   GLint zoffset;
};
void
_mesa_unmarshal_NamedFramebufferTexture3DEXT(struct gl_context *ctx, const struct marshal_cmd_NamedFramebufferTexture3DEXT *cmd)
{
   const GLuint framebuffer = cmd->framebuffer;
   const GLenum attachment = cmd->attachment;
   const GLenum textarget = cmd->textarget;
   const GLuint texture = cmd->texture;
   const GLint level = cmd->level;
   const GLint zoffset = cmd->zoffset;
   CALL_NamedFramebufferTexture3DEXT(ctx->CurrentServerDispatch, (framebuffer, attachment, textarget, texture, level, zoffset));
}
void GLAPIENTRY
_mesa_marshal_NamedFramebufferTexture3DEXT(GLuint framebuffer, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_NamedFramebufferTexture3DEXT);
   struct marshal_cmd_NamedFramebufferTexture3DEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_NamedFramebufferTexture3DEXT, cmd_size);
   cmd->framebuffer = framebuffer;
   cmd->attachment = attachment;
   cmd->textarget = textarget;
   cmd->texture = texture;
   cmd->level = level;
   cmd->zoffset = zoffset;
}


/* NamedFramebufferRenderbufferEXT: marshalled asynchronously */
struct marshal_cmd_NamedFramebufferRenderbufferEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 attachment;
   GLenum16 renderbuffertarget;
   GLuint framebuffer;
   GLuint renderbuffer;
};
void
_mesa_unmarshal_NamedFramebufferRenderbufferEXT(struct gl_context *ctx, const struct marshal_cmd_NamedFramebufferRenderbufferEXT *cmd)
{
   const GLuint framebuffer = cmd->framebuffer;
   const GLenum attachment = cmd->attachment;
   const GLenum renderbuffertarget = cmd->renderbuffertarget;
   const GLuint renderbuffer = cmd->renderbuffer;
   CALL_NamedFramebufferRenderbufferEXT(ctx->CurrentServerDispatch, (framebuffer, attachment, renderbuffertarget, renderbuffer));
}
void GLAPIENTRY
_mesa_marshal_NamedFramebufferRenderbufferEXT(GLuint framebuffer, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_NamedFramebufferRenderbufferEXT);
   struct marshal_cmd_NamedFramebufferRenderbufferEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_NamedFramebufferRenderbufferEXT, cmd_size);
   cmd->framebuffer = framebuffer;
   cmd->attachment = attachment;
   cmd->renderbuffertarget = renderbuffertarget;
   cmd->renderbuffer = renderbuffer;
}


/* GetNamedFramebufferAttachmentParameterivEXT: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetNamedFramebufferAttachmentParameterivEXT(GLuint framebuffer, GLenum attachment, GLenum pname, GLint * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetNamedFramebufferAttachmentParameterivEXT");
   CALL_GetNamedFramebufferAttachmentParameterivEXT(ctx->CurrentServerDispatch, (framebuffer, attachment, pname, params));
}


/* NamedRenderbufferStorageEXT: marshalled asynchronously */
struct marshal_cmd_NamedRenderbufferStorageEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 internalformat;
   GLuint renderbuffer;
   GLsizei width;
   GLsizei height;
};
void
_mesa_unmarshal_NamedRenderbufferStorageEXT(struct gl_context *ctx, const struct marshal_cmd_NamedRenderbufferStorageEXT *cmd)
{
   const GLuint renderbuffer = cmd->renderbuffer;
   const GLenum internalformat = cmd->internalformat;
   const GLsizei width = cmd->width;
   const GLsizei height = cmd->height;
   CALL_NamedRenderbufferStorageEXT(ctx->CurrentServerDispatch, (renderbuffer, internalformat, width, height));
}
void GLAPIENTRY
_mesa_marshal_NamedRenderbufferStorageEXT(GLuint renderbuffer, GLenum internalformat, GLsizei width, GLsizei height)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_NamedRenderbufferStorageEXT);
   struct marshal_cmd_NamedRenderbufferStorageEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_NamedRenderbufferStorageEXT, cmd_size);
   cmd->renderbuffer = renderbuffer;
   cmd->internalformat = internalformat;
   cmd->width = width;
   cmd->height = height;
}


/* GetNamedRenderbufferParameterivEXT: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetNamedRenderbufferParameterivEXT(GLuint renderbuffer, GLenum pname, GLint * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetNamedRenderbufferParameterivEXT");
   CALL_GetNamedRenderbufferParameterivEXT(ctx->CurrentServerDispatch, (renderbuffer, pname, params));
}


/* GenerateTextureMipmapEXT: marshalled asynchronously */
struct marshal_cmd_GenerateTextureMipmapEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLuint texture;
};
void
_mesa_unmarshal_GenerateTextureMipmapEXT(struct gl_context *ctx, const struct marshal_cmd_GenerateTextureMipmapEXT *cmd)
{
   const GLuint texture = cmd->texture;
   const GLenum target = cmd->target;
   CALL_GenerateTextureMipmapEXT(ctx->CurrentServerDispatch, (texture, target));
}
void GLAPIENTRY
_mesa_marshal_GenerateTextureMipmapEXT(GLuint texture, GLenum target)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_GenerateTextureMipmapEXT);
   struct marshal_cmd_GenerateTextureMipmapEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_GenerateTextureMipmapEXT, cmd_size);
   cmd->texture = texture;
   cmd->target = target;
}


/* GenerateMultiTexMipmapEXT: marshalled asynchronously */
struct marshal_cmd_GenerateMultiTexMipmapEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 texunit;
   GLenum16 target;
};
void
_mesa_unmarshal_GenerateMultiTexMipmapEXT(struct gl_context *ctx, const struct marshal_cmd_GenerateMultiTexMipmapEXT *cmd)
{
   const GLenum texunit = cmd->texunit;
   const GLenum target = cmd->target;
   CALL_GenerateMultiTexMipmapEXT(ctx->CurrentServerDispatch, (texunit, target));
}
void GLAPIENTRY
_mesa_marshal_GenerateMultiTexMipmapEXT(GLenum texunit, GLenum target)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_GenerateMultiTexMipmapEXT);
   struct marshal_cmd_GenerateMultiTexMipmapEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_GenerateMultiTexMipmapEXT, cmd_size);
   cmd->texunit = texunit;
   cmd->target = target;
}


/* NamedRenderbufferStorageMultisampleEXT: marshalled asynchronously */
struct marshal_cmd_NamedRenderbufferStorageMultisampleEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 internalformat;
   GLuint renderbuffer;
   GLsizei samples;
   GLsizei width;
   GLsizei height;
};
void
_mesa_unmarshal_NamedRenderbufferStorageMultisampleEXT(struct gl_context *ctx, const struct marshal_cmd_NamedRenderbufferStorageMultisampleEXT *cmd)
{
   const GLuint renderbuffer = cmd->renderbuffer;
   const GLsizei samples = cmd->samples;
   const GLenum internalformat = cmd->internalformat;
   const GLsizei width = cmd->width;
   const GLsizei height = cmd->height;
   CALL_NamedRenderbufferStorageMultisampleEXT(ctx->CurrentServerDispatch, (renderbuffer, samples, internalformat, width, height));
}
void GLAPIENTRY
_mesa_marshal_NamedRenderbufferStorageMultisampleEXT(GLuint renderbuffer, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_NamedRenderbufferStorageMultisampleEXT);
   struct marshal_cmd_NamedRenderbufferStorageMultisampleEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_NamedRenderbufferStorageMultisampleEXT, cmd_size);
   cmd->renderbuffer = renderbuffer;
   cmd->samples = samples;
   cmd->internalformat = internalformat;
   cmd->width = width;
   cmd->height = height;
}


/* NamedCopyBufferSubDataEXT: marshalled asynchronously */
struct marshal_cmd_NamedCopyBufferSubDataEXT
{
   struct marshal_cmd_base cmd_base;
   GLuint readBuffer;
   GLuint writeBuffer;
   GLintptr readOffset;
   GLintptr writeOffset;
   GLsizeiptr size;
};
void
_mesa_unmarshal_NamedCopyBufferSubDataEXT(struct gl_context *ctx, const struct marshal_cmd_NamedCopyBufferSubDataEXT *cmd)
{
   const GLuint readBuffer = cmd->readBuffer;
   const GLuint writeBuffer = cmd->writeBuffer;
   const GLintptr readOffset = cmd->readOffset;
   const GLintptr writeOffset = cmd->writeOffset;
   const GLsizeiptr size = cmd->size;
   CALL_NamedCopyBufferSubDataEXT(ctx->CurrentServerDispatch, (readBuffer, writeBuffer, readOffset, writeOffset, size));
}
void GLAPIENTRY
_mesa_marshal_NamedCopyBufferSubDataEXT(GLuint readBuffer, GLuint writeBuffer, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_NamedCopyBufferSubDataEXT);
   struct marshal_cmd_NamedCopyBufferSubDataEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_NamedCopyBufferSubDataEXT, cmd_size);
   cmd->readBuffer = readBuffer;
   cmd->writeBuffer = writeBuffer;
   cmd->readOffset = readOffset;
   cmd->writeOffset = writeOffset;
   cmd->size = size;
}


/* VertexArrayVertexOffsetEXT: marshalled asynchronously */
struct marshal_cmd_VertexArrayVertexOffsetEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 type;
   GLuint vaobj;
   GLuint buffer;
   GLint size;
   GLsizei stride;
   GLintptr offset;
};
void
_mesa_unmarshal_VertexArrayVertexOffsetEXT(struct gl_context *ctx, const struct marshal_cmd_VertexArrayVertexOffsetEXT *cmd)
{
   const GLuint vaobj = cmd->vaobj;
   const GLuint buffer = cmd->buffer;
   const GLint size = cmd->size;
   const GLenum type = cmd->type;
   const GLsizei stride = cmd->stride;
   const GLintptr offset = cmd->offset;
   CALL_VertexArrayVertexOffsetEXT(ctx->CurrentServerDispatch, (vaobj, buffer, size, type, stride, offset));
}
void GLAPIENTRY
_mesa_marshal_VertexArrayVertexOffsetEXT(GLuint vaobj, GLuint buffer, GLint size, GLenum type, GLsizei stride, GLintptr offset)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexArrayVertexOffsetEXT);
   struct marshal_cmd_VertexArrayVertexOffsetEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexArrayVertexOffsetEXT, cmd_size);
   cmd->vaobj = vaobj;
   cmd->buffer = buffer;
   cmd->size = size;
   cmd->type = type;
   cmd->stride = stride;
   cmd->offset = offset;
}


/* VertexArrayColorOffsetEXT: marshalled asynchronously */
struct marshal_cmd_VertexArrayColorOffsetEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 type;
   GLuint vaobj;
   GLuint buffer;
   GLint size;
   GLsizei stride;
   GLintptr offset;
};
void
_mesa_unmarshal_VertexArrayColorOffsetEXT(struct gl_context *ctx, const struct marshal_cmd_VertexArrayColorOffsetEXT *cmd)
{
   const GLuint vaobj = cmd->vaobj;
   const GLuint buffer = cmd->buffer;
   const GLint size = cmd->size;
   const GLenum type = cmd->type;
   const GLsizei stride = cmd->stride;
   const GLintptr offset = cmd->offset;
   CALL_VertexArrayColorOffsetEXT(ctx->CurrentServerDispatch, (vaobj, buffer, size, type, stride, offset));
}
void GLAPIENTRY
_mesa_marshal_VertexArrayColorOffsetEXT(GLuint vaobj, GLuint buffer, GLint size, GLenum type, GLsizei stride, GLintptr offset)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexArrayColorOffsetEXT);
   struct marshal_cmd_VertexArrayColorOffsetEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexArrayColorOffsetEXT, cmd_size);
   cmd->vaobj = vaobj;
   cmd->buffer = buffer;
   cmd->size = size;
   cmd->type = type;
   cmd->stride = stride;
   cmd->offset = offset;
}


/* VertexArrayEdgeFlagOffsetEXT: marshalled asynchronously */
struct marshal_cmd_VertexArrayEdgeFlagOffsetEXT
{
   struct marshal_cmd_base cmd_base;
   GLuint vaobj;
   GLuint buffer;
   GLsizei stride;
   GLintptr offset;
};
void
_mesa_unmarshal_VertexArrayEdgeFlagOffsetEXT(struct gl_context *ctx, const struct marshal_cmd_VertexArrayEdgeFlagOffsetEXT *cmd)
{
   const GLuint vaobj = cmd->vaobj;
   const GLuint buffer = cmd->buffer;
   const GLsizei stride = cmd->stride;
   const GLintptr offset = cmd->offset;
   CALL_VertexArrayEdgeFlagOffsetEXT(ctx->CurrentServerDispatch, (vaobj, buffer, stride, offset));
}
void GLAPIENTRY
_mesa_marshal_VertexArrayEdgeFlagOffsetEXT(GLuint vaobj, GLuint buffer, GLsizei stride, GLintptr offset)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexArrayEdgeFlagOffsetEXT);
   struct marshal_cmd_VertexArrayEdgeFlagOffsetEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexArrayEdgeFlagOffsetEXT, cmd_size);
   cmd->vaobj = vaobj;
   cmd->buffer = buffer;
   cmd->stride = stride;
   cmd->offset = offset;
}


/* VertexArrayIndexOffsetEXT: marshalled asynchronously */
struct marshal_cmd_VertexArrayIndexOffsetEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 type;
   GLuint vaobj;
   GLuint buffer;
   GLsizei stride;
   GLintptr offset;
};
void
_mesa_unmarshal_VertexArrayIndexOffsetEXT(struct gl_context *ctx, const struct marshal_cmd_VertexArrayIndexOffsetEXT *cmd)
{
   const GLuint vaobj = cmd->vaobj;
   const GLuint buffer = cmd->buffer;
   const GLenum type = cmd->type;
   const GLsizei stride = cmd->stride;
   const GLintptr offset = cmd->offset;
   CALL_VertexArrayIndexOffsetEXT(ctx->CurrentServerDispatch, (vaobj, buffer, type, stride, offset));
}
void GLAPIENTRY
_mesa_marshal_VertexArrayIndexOffsetEXT(GLuint vaobj, GLuint buffer, GLenum type, GLsizei stride, GLintptr offset)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexArrayIndexOffsetEXT);
   struct marshal_cmd_VertexArrayIndexOffsetEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexArrayIndexOffsetEXT, cmd_size);
   cmd->vaobj = vaobj;
   cmd->buffer = buffer;
   cmd->type = type;
   cmd->stride = stride;
   cmd->offset = offset;
}


/* VertexArrayNormalOffsetEXT: marshalled asynchronously */
struct marshal_cmd_VertexArrayNormalOffsetEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 type;
   GLuint vaobj;
   GLuint buffer;
   GLsizei stride;
   GLintptr offset;
};
void
_mesa_unmarshal_VertexArrayNormalOffsetEXT(struct gl_context *ctx, const struct marshal_cmd_VertexArrayNormalOffsetEXT *cmd)
{
   const GLuint vaobj = cmd->vaobj;
   const GLuint buffer = cmd->buffer;
   const GLenum type = cmd->type;
   const GLsizei stride = cmd->stride;
   const GLintptr offset = cmd->offset;
   CALL_VertexArrayNormalOffsetEXT(ctx->CurrentServerDispatch, (vaobj, buffer, type, stride, offset));
}
void GLAPIENTRY
_mesa_marshal_VertexArrayNormalOffsetEXT(GLuint vaobj, GLuint buffer, GLenum type, GLsizei stride, GLintptr offset)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexArrayNormalOffsetEXT);
   struct marshal_cmd_VertexArrayNormalOffsetEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexArrayNormalOffsetEXT, cmd_size);
   cmd->vaobj = vaobj;
   cmd->buffer = buffer;
   cmd->type = type;
   cmd->stride = stride;
   cmd->offset = offset;
}


/* VertexArrayTexCoordOffsetEXT: marshalled asynchronously */
struct marshal_cmd_VertexArrayTexCoordOffsetEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 type;
   GLuint vaobj;
   GLuint buffer;
   GLint size;
   GLsizei stride;
   GLintptr offset;
};
void
_mesa_unmarshal_VertexArrayTexCoordOffsetEXT(struct gl_context *ctx, const struct marshal_cmd_VertexArrayTexCoordOffsetEXT *cmd)
{
   const GLuint vaobj = cmd->vaobj;
   const GLuint buffer = cmd->buffer;
   const GLint size = cmd->size;
   const GLenum type = cmd->type;
   const GLsizei stride = cmd->stride;
   const GLintptr offset = cmd->offset;
   CALL_VertexArrayTexCoordOffsetEXT(ctx->CurrentServerDispatch, (vaobj, buffer, size, type, stride, offset));
}
void GLAPIENTRY
_mesa_marshal_VertexArrayTexCoordOffsetEXT(GLuint vaobj, GLuint buffer, GLint size, GLenum type, GLsizei stride, GLintptr offset)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexArrayTexCoordOffsetEXT);
   struct marshal_cmd_VertexArrayTexCoordOffsetEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexArrayTexCoordOffsetEXT, cmd_size);
   cmd->vaobj = vaobj;
   cmd->buffer = buffer;
   cmd->size = size;
   cmd->type = type;
   cmd->stride = stride;
   cmd->offset = offset;
}


/* VertexArrayMultiTexCoordOffsetEXT: marshalled asynchronously */
struct marshal_cmd_VertexArrayMultiTexCoordOffsetEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 texunit;
   GLenum16 type;
   GLuint vaobj;
   GLuint buffer;
   GLint size;
   GLsizei stride;
   GLintptr offset;
};
void
_mesa_unmarshal_VertexArrayMultiTexCoordOffsetEXT(struct gl_context *ctx, const struct marshal_cmd_VertexArrayMultiTexCoordOffsetEXT *cmd)
{
   const GLuint vaobj = cmd->vaobj;
   const GLuint buffer = cmd->buffer;
   const GLenum texunit = cmd->texunit;
   const GLint size = cmd->size;
   const GLenum type = cmd->type;
   const GLsizei stride = cmd->stride;
   const GLintptr offset = cmd->offset;
   CALL_VertexArrayMultiTexCoordOffsetEXT(ctx->CurrentServerDispatch, (vaobj, buffer, texunit, size, type, stride, offset));
}
void GLAPIENTRY
_mesa_marshal_VertexArrayMultiTexCoordOffsetEXT(GLuint vaobj, GLuint buffer, GLenum texunit, GLint size, GLenum type, GLsizei stride, GLintptr offset)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexArrayMultiTexCoordOffsetEXT);
   struct marshal_cmd_VertexArrayMultiTexCoordOffsetEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexArrayMultiTexCoordOffsetEXT, cmd_size);
   cmd->vaobj = vaobj;
   cmd->buffer = buffer;
   cmd->texunit = texunit;
   cmd->size = size;
   cmd->type = type;
   cmd->stride = stride;
   cmd->offset = offset;
}


/* VertexArrayFogCoordOffsetEXT: marshalled asynchronously */
struct marshal_cmd_VertexArrayFogCoordOffsetEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 type;
   GLuint vaobj;
   GLuint buffer;
   GLsizei stride;
   GLintptr offset;
};
void
_mesa_unmarshal_VertexArrayFogCoordOffsetEXT(struct gl_context *ctx, const struct marshal_cmd_VertexArrayFogCoordOffsetEXT *cmd)
{
   const GLuint vaobj = cmd->vaobj;
   const GLuint buffer = cmd->buffer;
   const GLenum type = cmd->type;
   const GLsizei stride = cmd->stride;
   const GLintptr offset = cmd->offset;
   CALL_VertexArrayFogCoordOffsetEXT(ctx->CurrentServerDispatch, (vaobj, buffer, type, stride, offset));
}
void GLAPIENTRY
_mesa_marshal_VertexArrayFogCoordOffsetEXT(GLuint vaobj, GLuint buffer, GLenum type, GLsizei stride, GLintptr offset)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexArrayFogCoordOffsetEXT);
   struct marshal_cmd_VertexArrayFogCoordOffsetEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexArrayFogCoordOffsetEXT, cmd_size);
   cmd->vaobj = vaobj;
   cmd->buffer = buffer;
   cmd->type = type;
   cmd->stride = stride;
   cmd->offset = offset;
}


/* VertexArraySecondaryColorOffsetEXT: marshalled asynchronously */
struct marshal_cmd_VertexArraySecondaryColorOffsetEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 type;
   GLuint vaobj;
   GLuint buffer;
   GLint size;
   GLsizei stride;
   GLintptr offset;
};
void
_mesa_unmarshal_VertexArraySecondaryColorOffsetEXT(struct gl_context *ctx, const struct marshal_cmd_VertexArraySecondaryColorOffsetEXT *cmd)
{
   const GLuint vaobj = cmd->vaobj;
   const GLuint buffer = cmd->buffer;
   const GLint size = cmd->size;
   const GLenum type = cmd->type;
   const GLsizei stride = cmd->stride;
   const GLintptr offset = cmd->offset;
   CALL_VertexArraySecondaryColorOffsetEXT(ctx->CurrentServerDispatch, (vaobj, buffer, size, type, stride, offset));
}
void GLAPIENTRY
_mesa_marshal_VertexArraySecondaryColorOffsetEXT(GLuint vaobj, GLuint buffer, GLint size, GLenum type, GLsizei stride, GLintptr offset)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexArraySecondaryColorOffsetEXT);
   struct marshal_cmd_VertexArraySecondaryColorOffsetEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexArraySecondaryColorOffsetEXT, cmd_size);
   cmd->vaobj = vaobj;
   cmd->buffer = buffer;
   cmd->size = size;
   cmd->type = type;
   cmd->stride = stride;
   cmd->offset = offset;
}


/* VertexArrayVertexAttribOffsetEXT: marshalled asynchronously */
struct marshal_cmd_VertexArrayVertexAttribOffsetEXT
{
   struct marshal_cmd_base cmd_base;
   GLboolean normalized;
   GLenum16 type;
   GLuint vaobj;
   GLuint buffer;
   GLuint index;
   GLint size;
   GLsizei stride;
   GLintptr offset;
};
void
_mesa_unmarshal_VertexArrayVertexAttribOffsetEXT(struct gl_context *ctx, const struct marshal_cmd_VertexArrayVertexAttribOffsetEXT *cmd)
{
   const GLuint vaobj = cmd->vaobj;
   const GLuint buffer = cmd->buffer;
   const GLuint index = cmd->index;
   const GLint size = cmd->size;
   const GLenum type = cmd->type;
   const GLboolean normalized = cmd->normalized;
   const GLsizei stride = cmd->stride;
   const GLintptr offset = cmd->offset;
   CALL_VertexArrayVertexAttribOffsetEXT(ctx->CurrentServerDispatch, (vaobj, buffer, index, size, type, normalized, stride, offset));
}
void GLAPIENTRY
_mesa_marshal_VertexArrayVertexAttribOffsetEXT(GLuint vaobj, GLuint buffer, GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, GLintptr offset)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexArrayVertexAttribOffsetEXT);
   struct marshal_cmd_VertexArrayVertexAttribOffsetEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexArrayVertexAttribOffsetEXT, cmd_size);
   cmd->vaobj = vaobj;
   cmd->buffer = buffer;
   cmd->index = index;
   cmd->size = size;
   cmd->type = type;
   cmd->normalized = normalized;
   cmd->stride = stride;
   cmd->offset = offset;
}


/* VertexArrayVertexAttribIOffsetEXT: marshalled asynchronously */
struct marshal_cmd_VertexArrayVertexAttribIOffsetEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 type;
   GLuint vaobj;
   GLuint buffer;
   GLuint index;
   GLint size;
   GLsizei stride;
   GLintptr offset;
};
void
_mesa_unmarshal_VertexArrayVertexAttribIOffsetEXT(struct gl_context *ctx, const struct marshal_cmd_VertexArrayVertexAttribIOffsetEXT *cmd)
{
   const GLuint vaobj = cmd->vaobj;
   const GLuint buffer = cmd->buffer;
   const GLuint index = cmd->index;
   const GLint size = cmd->size;
   const GLenum type = cmd->type;
   const GLsizei stride = cmd->stride;
   const GLintptr offset = cmd->offset;
   CALL_VertexArrayVertexAttribIOffsetEXT(ctx->CurrentServerDispatch, (vaobj, buffer, index, size, type, stride, offset));
}
void GLAPIENTRY
_mesa_marshal_VertexArrayVertexAttribIOffsetEXT(GLuint vaobj, GLuint buffer, GLuint index, GLint size, GLenum type, GLsizei stride, GLintptr offset)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexArrayVertexAttribIOffsetEXT);
   struct marshal_cmd_VertexArrayVertexAttribIOffsetEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexArrayVertexAttribIOffsetEXT, cmd_size);
   cmd->vaobj = vaobj;
   cmd->buffer = buffer;
   cmd->index = index;
   cmd->size = size;
   cmd->type = type;
   cmd->stride = stride;
   cmd->offset = offset;
}


/* EnableVertexArrayEXT: marshalled asynchronously */
struct marshal_cmd_EnableVertexArrayEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 array;
   GLuint vaobj;
};
void
_mesa_unmarshal_EnableVertexArrayEXT(struct gl_context *ctx, const struct marshal_cmd_EnableVertexArrayEXT *cmd)
{
   const GLuint vaobj = cmd->vaobj;
   const GLenum array = cmd->array;
   CALL_EnableVertexArrayEXT(ctx->CurrentServerDispatch, (vaobj, array));
}
void GLAPIENTRY
_mesa_marshal_EnableVertexArrayEXT(GLuint vaobj, GLenum array)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_EnableVertexArrayEXT);
   struct marshal_cmd_EnableVertexArrayEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_EnableVertexArrayEXT, cmd_size);
   cmd->vaobj = vaobj;
   cmd->array = array;
   if (COMPAT) _mesa_glthread_ClientState(ctx, &vaobj, _mesa_array_to_attrib(ctx, array), true);
}


/* DisableVertexArrayEXT: marshalled asynchronously */
struct marshal_cmd_DisableVertexArrayEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 array;
   GLuint vaobj;
};
void
_mesa_unmarshal_DisableVertexArrayEXT(struct gl_context *ctx, const struct marshal_cmd_DisableVertexArrayEXT *cmd)
{
   const GLuint vaobj = cmd->vaobj;
   const GLenum array = cmd->array;
   CALL_DisableVertexArrayEXT(ctx->CurrentServerDispatch, (vaobj, array));
}
void GLAPIENTRY
_mesa_marshal_DisableVertexArrayEXT(GLuint vaobj, GLenum array)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_DisableVertexArrayEXT);
   struct marshal_cmd_DisableVertexArrayEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_DisableVertexArrayEXT, cmd_size);
   cmd->vaobj = vaobj;
   cmd->array = array;
   if (COMPAT) _mesa_glthread_ClientState(ctx, &vaobj, _mesa_array_to_attrib(ctx, array), false);
}


/* EnableVertexArrayAttribEXT: marshalled asynchronously */
struct marshal_cmd_EnableVertexArrayAttribEXT
{
   struct marshal_cmd_base cmd_base;
   GLuint vaobj;
   GLuint index;
};
void
_mesa_unmarshal_EnableVertexArrayAttribEXT(struct gl_context *ctx, const struct marshal_cmd_EnableVertexArrayAttribEXT *cmd)
{
   const GLuint vaobj = cmd->vaobj;
   const GLuint index = cmd->index;
   CALL_EnableVertexArrayAttribEXT(ctx->CurrentServerDispatch, (vaobj, index));
}
void GLAPIENTRY
_mesa_marshal_EnableVertexArrayAttribEXT(GLuint vaobj, GLuint index)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_EnableVertexArrayAttribEXT);
   struct marshal_cmd_EnableVertexArrayAttribEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_EnableVertexArrayAttribEXT, cmd_size);
   cmd->vaobj = vaobj;
   cmd->index = index;
   if (COMPAT) _mesa_glthread_ClientState(ctx, &vaobj, VERT_ATTRIB_GENERIC(index), true);
}


/* DisableVertexArrayAttribEXT: marshalled asynchronously */
struct marshal_cmd_DisableVertexArrayAttribEXT
{
   struct marshal_cmd_base cmd_base;
   GLuint vaobj;
   GLuint index;
};
void
_mesa_unmarshal_DisableVertexArrayAttribEXT(struct gl_context *ctx, const struct marshal_cmd_DisableVertexArrayAttribEXT *cmd)
{
   const GLuint vaobj = cmd->vaobj;
   const GLuint index = cmd->index;
   CALL_DisableVertexArrayAttribEXT(ctx->CurrentServerDispatch, (vaobj, index));
}
void GLAPIENTRY
_mesa_marshal_DisableVertexArrayAttribEXT(GLuint vaobj, GLuint index)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_DisableVertexArrayAttribEXT);
   struct marshal_cmd_DisableVertexArrayAttribEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_DisableVertexArrayAttribEXT, cmd_size);
   cmd->vaobj = vaobj;
   cmd->index = index;
   if (COMPAT) _mesa_glthread_ClientState(ctx, &vaobj, VERT_ATTRIB_GENERIC(index), false);
}


/* GetVertexArrayIntegervEXT: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetVertexArrayIntegervEXT(GLuint vaobj, GLenum pname, GLint* param)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetVertexArrayIntegervEXT");
   CALL_GetVertexArrayIntegervEXT(ctx->CurrentServerDispatch, (vaobj, pname, param));
}


/* GetVertexArrayPointervEXT: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetVertexArrayPointervEXT(GLuint vaobj, GLenum pname, GLvoid** param)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetVertexArrayPointervEXT");
   CALL_GetVertexArrayPointervEXT(ctx->CurrentServerDispatch, (vaobj, pname, param));
}


/* GetVertexArrayIntegeri_vEXT: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetVertexArrayIntegeri_vEXT(GLuint vaobj, GLuint index, GLenum pname, GLint* param)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetVertexArrayIntegeri_vEXT");
   CALL_GetVertexArrayIntegeri_vEXT(ctx->CurrentServerDispatch, (vaobj, index, pname, param));
}


/* GetVertexArrayPointeri_vEXT: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetVertexArrayPointeri_vEXT(GLuint vaobj, GLuint index, GLenum pname, GLvoid** param)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetVertexArrayPointeri_vEXT");
   CALL_GetVertexArrayPointeri_vEXT(ctx->CurrentServerDispatch, (vaobj, index, pname, param));
}


/* NamedProgramStringEXT: marshalled asynchronously */
struct marshal_cmd_NamedProgramStringEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLenum16 format;
   GLuint program;
   GLsizei len;
   /* Next len bytes are GLvoid string[len] */
};
void
_mesa_unmarshal_NamedProgramStringEXT(struct gl_context *ctx, const struct marshal_cmd_NamedProgramStringEXT *cmd)
{
   const GLuint program = cmd->program;
   const GLenum target = cmd->target;
   const GLenum format = cmd->format;
   const GLsizei len = cmd->len;
   GLvoid * string;
   const char *variable_data = (const char *) (cmd + 1);
   string = (GLvoid *) variable_data;
   CALL_NamedProgramStringEXT(ctx->CurrentServerDispatch, (program, target, format, len, string));
}
void GLAPIENTRY
_mesa_marshal_NamedProgramStringEXT(GLuint program, GLenum target, GLenum format, GLsizei len, const GLvoid* string)
{
   GET_CURRENT_CONTEXT(ctx);
   int string_size = len;
   int cmd_size = sizeof(struct marshal_cmd_NamedProgramStringEXT) + string_size;
   struct marshal_cmd_NamedProgramStringEXT *cmd;
   if (unlikely(string_size < 0 || (string_size > 0 && !string) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "NamedProgramStringEXT");
      CALL_NamedProgramStringEXT(ctx->CurrentServerDispatch, (program, target, format, len, string));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_NamedProgramStringEXT, cmd_size);
   cmd->program = program;
   cmd->target = target;
   cmd->format = format;
   cmd->len = len;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, string, string_size);
}


/* GetNamedProgramStringEXT: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetNamedProgramStringEXT(GLuint program, GLenum target, GLenum pname, GLvoid* string)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetNamedProgramStringEXT");
   CALL_GetNamedProgramStringEXT(ctx->CurrentServerDispatch, (program, target, pname, string));
}


/* NamedProgramLocalParameter4fEXT: marshalled asynchronously */
struct marshal_cmd_NamedProgramLocalParameter4fEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLuint program;
   GLuint index;
   GLfloat x;
   GLfloat y;
   GLfloat z;
   GLfloat w;
};
void
_mesa_unmarshal_NamedProgramLocalParameter4fEXT(struct gl_context *ctx, const struct marshal_cmd_NamedProgramLocalParameter4fEXT *cmd)
{
   const GLuint program = cmd->program;
   const GLenum target = cmd->target;
   const GLuint index = cmd->index;
   const GLfloat x = cmd->x;
   const GLfloat y = cmd->y;
   const GLfloat z = cmd->z;
   const GLfloat w = cmd->w;
   CALL_NamedProgramLocalParameter4fEXT(ctx->CurrentServerDispatch, (program, target, index, x, y, z, w));
}
void GLAPIENTRY
_mesa_marshal_NamedProgramLocalParameter4fEXT(GLuint program, GLenum target, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_NamedProgramLocalParameter4fEXT);
   struct marshal_cmd_NamedProgramLocalParameter4fEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_NamedProgramLocalParameter4fEXT, cmd_size);
   cmd->program = program;
   cmd->target = target;
   cmd->index = index;
   cmd->x = x;
   cmd->y = y;
   cmd->z = z;
   cmd->w = w;
}


/* NamedProgramLocalParameter4fvEXT: marshalled asynchronously */
struct marshal_cmd_NamedProgramLocalParameter4fvEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLuint program;
   GLuint index;
   GLfloat params[4];
};
void
_mesa_unmarshal_NamedProgramLocalParameter4fvEXT(struct gl_context *ctx, const struct marshal_cmd_NamedProgramLocalParameter4fvEXT *cmd)
{
   const GLuint program = cmd->program;
   const GLenum target = cmd->target;
   const GLuint index = cmd->index;
   const GLfloat * params = cmd->params;
   CALL_NamedProgramLocalParameter4fvEXT(ctx->CurrentServerDispatch, (program, target, index, params));
}
void GLAPIENTRY
_mesa_marshal_NamedProgramLocalParameter4fvEXT(GLuint program, GLenum target, GLuint index, const GLfloat* params)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_NamedProgramLocalParameter4fvEXT);
   struct marshal_cmd_NamedProgramLocalParameter4fvEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_NamedProgramLocalParameter4fvEXT, cmd_size);
   cmd->program = program;
   cmd->target = target;
   cmd->index = index;
   memcpy(cmd->params, params, 4 * sizeof(GLfloat));
}


/* GetNamedProgramLocalParameterfvEXT: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetNamedProgramLocalParameterfvEXT(GLuint program, GLenum target, GLuint index, GLfloat* params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetNamedProgramLocalParameterfvEXT");
   CALL_GetNamedProgramLocalParameterfvEXT(ctx->CurrentServerDispatch, (program, target, index, params));
}


/* NamedProgramLocalParameter4dEXT: marshalled asynchronously */
struct marshal_cmd_NamedProgramLocalParameter4dEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLuint program;
   GLuint index;
   GLdouble x;
   GLdouble y;
   GLdouble z;
   GLdouble w;
};
void
_mesa_unmarshal_NamedProgramLocalParameter4dEXT(struct gl_context *ctx, const struct marshal_cmd_NamedProgramLocalParameter4dEXT *cmd)
{
   const GLuint program = cmd->program;
   const GLenum target = cmd->target;
   const GLuint index = cmd->index;
   const GLdouble x = cmd->x;
   const GLdouble y = cmd->y;
   const GLdouble z = cmd->z;
   const GLdouble w = cmd->w;
   CALL_NamedProgramLocalParameter4dEXT(ctx->CurrentServerDispatch, (program, target, index, x, y, z, w));
}
void GLAPIENTRY
_mesa_marshal_NamedProgramLocalParameter4dEXT(GLuint program, GLenum target, GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_NamedProgramLocalParameter4dEXT);
   struct marshal_cmd_NamedProgramLocalParameter4dEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_NamedProgramLocalParameter4dEXT, cmd_size);
   cmd->program = program;
   cmd->target = target;
   cmd->index = index;
   cmd->x = x;
   cmd->y = y;
   cmd->z = z;
   cmd->w = w;
}


/* NamedProgramLocalParameter4dvEXT: marshalled asynchronously */
struct marshal_cmd_NamedProgramLocalParameter4dvEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLuint program;
   GLuint index;
   GLdouble params[4];
};
void
_mesa_unmarshal_NamedProgramLocalParameter4dvEXT(struct gl_context *ctx, const struct marshal_cmd_NamedProgramLocalParameter4dvEXT *cmd)
{
   const GLuint program = cmd->program;
   const GLenum target = cmd->target;
   const GLuint index = cmd->index;
   const GLdouble * params = cmd->params;
   CALL_NamedProgramLocalParameter4dvEXT(ctx->CurrentServerDispatch, (program, target, index, params));
}
void GLAPIENTRY
_mesa_marshal_NamedProgramLocalParameter4dvEXT(GLuint program, GLenum target, GLuint index, const GLdouble* params)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_NamedProgramLocalParameter4dvEXT);
   struct marshal_cmd_NamedProgramLocalParameter4dvEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_NamedProgramLocalParameter4dvEXT, cmd_size);
   cmd->program = program;
   cmd->target = target;
   cmd->index = index;
   memcpy(cmd->params, params, 4 * sizeof(GLdouble));
}


/* GetNamedProgramLocalParameterdvEXT: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetNamedProgramLocalParameterdvEXT(GLuint program, GLenum target, GLuint index, GLdouble* params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetNamedProgramLocalParameterdvEXT");
   CALL_GetNamedProgramLocalParameterdvEXT(ctx->CurrentServerDispatch, (program, target, index, params));
}


/* GetNamedProgramivEXT: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetNamedProgramivEXT(GLuint program, GLenum target, GLenum pname, GLint* params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetNamedProgramivEXT");
   CALL_GetNamedProgramivEXT(ctx->CurrentServerDispatch, (program, target, pname, params));
}


/* TextureBufferEXT: marshalled asynchronously */
struct marshal_cmd_TextureBufferEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLenum16 internalformat;
   GLuint texture;
   GLuint buffer;
};
void
_mesa_unmarshal_TextureBufferEXT(struct gl_context *ctx, const struct marshal_cmd_TextureBufferEXT *cmd)
{
   const GLuint texture = cmd->texture;
   const GLenum target = cmd->target;
   const GLenum internalformat = cmd->internalformat;
   const GLuint buffer = cmd->buffer;
   CALL_TextureBufferEXT(ctx->CurrentServerDispatch, (texture, target, internalformat, buffer));
}
void GLAPIENTRY
_mesa_marshal_TextureBufferEXT(GLuint texture, GLenum target, GLenum internalformat, GLuint buffer)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TextureBufferEXT);
   struct marshal_cmd_TextureBufferEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TextureBufferEXT, cmd_size);
   cmd->texture = texture;
   cmd->target = target;
   cmd->internalformat = internalformat;
   cmd->buffer = buffer;
}


/* MultiTexBufferEXT: marshalled asynchronously */
struct marshal_cmd_MultiTexBufferEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 texunit;
   GLenum16 target;
   GLenum16 internalformat;
   GLuint buffer;
};
void
_mesa_unmarshal_MultiTexBufferEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexBufferEXT *cmd)
{
   const GLenum texunit = cmd->texunit;
   const GLenum target = cmd->target;
   const GLenum internalformat = cmd->internalformat;
   const GLuint buffer = cmd->buffer;
   CALL_MultiTexBufferEXT(ctx->CurrentServerDispatch, (texunit, target, internalformat, buffer));
}
void GLAPIENTRY
_mesa_marshal_MultiTexBufferEXT(GLenum texunit, GLenum target, GLenum internalformat, GLuint buffer)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexBufferEXT);
   struct marshal_cmd_MultiTexBufferEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexBufferEXT, cmd_size);
   cmd->texunit = texunit;
   cmd->target = target;
   cmd->internalformat = internalformat;
   cmd->buffer = buffer;
}


/* TextureParameterIivEXT: marshalled asynchronously */
struct marshal_cmd_TextureParameterIivEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLenum16 pname;
   GLuint texture;
   /* Next safe_mul(_mesa_tex_param_enum_to_count(pname), 1 * sizeof(GLint)) bytes are GLint params[_mesa_tex_param_enum_to_count(pname)] */
};
void
_mesa_unmarshal_TextureParameterIivEXT(struct gl_context *ctx, const struct marshal_cmd_TextureParameterIivEXT *cmd)
{
   const GLuint texture = cmd->texture;
   const GLenum target = cmd->target;
   const GLenum pname = cmd->pname;
   GLint * params;
   const char *variable_data = (const char *) (cmd + 1);
   params = (GLint *) variable_data;
   CALL_TextureParameterIivEXT(ctx->CurrentServerDispatch, (texture, target, pname, params));
}
void GLAPIENTRY
_mesa_marshal_TextureParameterIivEXT(GLuint texture, GLenum target, GLenum pname, const GLint* params)
{
   GET_CURRENT_CONTEXT(ctx);
   int params_size = safe_mul(_mesa_tex_param_enum_to_count(pname), 1 * sizeof(GLint));
   int cmd_size = sizeof(struct marshal_cmd_TextureParameterIivEXT) + params_size;
   struct marshal_cmd_TextureParameterIivEXT *cmd;
   if (unlikely(params_size < 0 || (params_size > 0 && !params) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "TextureParameterIivEXT");
      CALL_TextureParameterIivEXT(ctx->CurrentServerDispatch, (texture, target, pname, params));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TextureParameterIivEXT, cmd_size);
   cmd->texture = texture;
   cmd->target = target;
   cmd->pname = pname;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, params, params_size);
}


/* TextureParameterIuivEXT: marshalled asynchronously */
struct marshal_cmd_TextureParameterIuivEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLenum16 pname;
   GLuint texture;
   /* Next safe_mul(_mesa_tex_param_enum_to_count(pname), 1 * sizeof(GLuint)) bytes are GLuint params[_mesa_tex_param_enum_to_count(pname)] */
};
void
_mesa_unmarshal_TextureParameterIuivEXT(struct gl_context *ctx, const struct marshal_cmd_TextureParameterIuivEXT *cmd)
{
   const GLuint texture = cmd->texture;
   const GLenum target = cmd->target;
   const GLenum pname = cmd->pname;
   GLuint * params;
   const char *variable_data = (const char *) (cmd + 1);
   params = (GLuint *) variable_data;
   CALL_TextureParameterIuivEXT(ctx->CurrentServerDispatch, (texture, target, pname, params));
}
void GLAPIENTRY
_mesa_marshal_TextureParameterIuivEXT(GLuint texture, GLenum target, GLenum pname, const GLuint* params)
{
   GET_CURRENT_CONTEXT(ctx);
   int params_size = safe_mul(_mesa_tex_param_enum_to_count(pname), 1 * sizeof(GLuint));
   int cmd_size = sizeof(struct marshal_cmd_TextureParameterIuivEXT) + params_size;
   struct marshal_cmd_TextureParameterIuivEXT *cmd;
   if (unlikely(params_size < 0 || (params_size > 0 && !params) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "TextureParameterIuivEXT");
      CALL_TextureParameterIuivEXT(ctx->CurrentServerDispatch, (texture, target, pname, params));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TextureParameterIuivEXT, cmd_size);
   cmd->texture = texture;
   cmd->target = target;
   cmd->pname = pname;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, params, params_size);
}


/* GetTextureParameterIivEXT: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetTextureParameterIivEXT(GLuint texture, GLenum target, GLenum pname, GLint* params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetTextureParameterIivEXT");
   CALL_GetTextureParameterIivEXT(ctx->CurrentServerDispatch, (texture, target, pname, params));
}


/* GetTextureParameterIuivEXT: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetTextureParameterIuivEXT(GLuint texture, GLenum target, GLenum pname, GLuint* params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetTextureParameterIuivEXT");
   CALL_GetTextureParameterIuivEXT(ctx->CurrentServerDispatch, (texture, target, pname, params));
}


/* MultiTexParameterIivEXT: marshalled asynchronously */
struct marshal_cmd_MultiTexParameterIivEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 texunit;
   GLenum16 target;
   GLenum16 pname;
   /* Next safe_mul(_mesa_tex_param_enum_to_count(pname), 1 * sizeof(GLint)) bytes are GLint params[_mesa_tex_param_enum_to_count(pname)] */
};
void
_mesa_unmarshal_MultiTexParameterIivEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexParameterIivEXT *cmd)
{
   const GLenum texunit = cmd->texunit;
   const GLenum target = cmd->target;
   const GLenum pname = cmd->pname;
   GLint * params;
   const char *variable_data = (const char *) (cmd + 1);
   params = (GLint *) variable_data;
   CALL_MultiTexParameterIivEXT(ctx->CurrentServerDispatch, (texunit, target, pname, params));
}
void GLAPIENTRY
_mesa_marshal_MultiTexParameterIivEXT(GLenum texunit, GLenum target, GLenum pname, const GLint* params)
{
   GET_CURRENT_CONTEXT(ctx);
   int params_size = safe_mul(_mesa_tex_param_enum_to_count(pname), 1 * sizeof(GLint));
   int cmd_size = sizeof(struct marshal_cmd_MultiTexParameterIivEXT) + params_size;
   struct marshal_cmd_MultiTexParameterIivEXT *cmd;
   if (unlikely(params_size < 0 || (params_size > 0 && !params) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "MultiTexParameterIivEXT");
      CALL_MultiTexParameterIivEXT(ctx->CurrentServerDispatch, (texunit, target, pname, params));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexParameterIivEXT, cmd_size);
   cmd->texunit = texunit;
   cmd->target = target;
   cmd->pname = pname;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, params, params_size);
}


/* MultiTexParameterIuivEXT: marshalled asynchronously */
struct marshal_cmd_MultiTexParameterIuivEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 texunit;
   GLenum16 target;
   GLenum16 pname;
   /* Next safe_mul(_mesa_tex_param_enum_to_count(pname), 1 * sizeof(GLuint)) bytes are GLuint params[_mesa_tex_param_enum_to_count(pname)] */
};
void
_mesa_unmarshal_MultiTexParameterIuivEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexParameterIuivEXT *cmd)
{
   const GLenum texunit = cmd->texunit;
   const GLenum target = cmd->target;
   const GLenum pname = cmd->pname;
   GLuint * params;
   const char *variable_data = (const char *) (cmd + 1);
   params = (GLuint *) variable_data;
   CALL_MultiTexParameterIuivEXT(ctx->CurrentServerDispatch, (texunit, target, pname, params));
}
void GLAPIENTRY
_mesa_marshal_MultiTexParameterIuivEXT(GLenum texunit, GLenum target, GLenum pname, const GLuint* params)
{
   GET_CURRENT_CONTEXT(ctx);
   int params_size = safe_mul(_mesa_tex_param_enum_to_count(pname), 1 * sizeof(GLuint));
   int cmd_size = sizeof(struct marshal_cmd_MultiTexParameterIuivEXT) + params_size;
   struct marshal_cmd_MultiTexParameterIuivEXT *cmd;
   if (unlikely(params_size < 0 || (params_size > 0 && !params) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "MultiTexParameterIuivEXT");
      CALL_MultiTexParameterIuivEXT(ctx->CurrentServerDispatch, (texunit, target, pname, params));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexParameterIuivEXT, cmd_size);
   cmd->texunit = texunit;
   cmd->target = target;
   cmd->pname = pname;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, params, params_size);
}


/* GetMultiTexParameterIivEXT: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetMultiTexParameterIivEXT(GLenum texunit, GLenum target, GLenum pname, GLint* params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetMultiTexParameterIivEXT");
   CALL_GetMultiTexParameterIivEXT(ctx->CurrentServerDispatch, (texunit, target, pname, params));
}


/* GetMultiTexParameterIuivEXT: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetMultiTexParameterIuivEXT(GLenum texunit, GLenum target, GLenum pname, GLuint* params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetMultiTexParameterIuivEXT");
   CALL_GetMultiTexParameterIuivEXT(ctx->CurrentServerDispatch, (texunit, target, pname, params));
}


/* NamedProgramLocalParameters4fvEXT: marshalled asynchronously */
struct marshal_cmd_NamedProgramLocalParameters4fvEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLuint program;
   GLuint index;
   GLsizei count;
   GLfloat params[4];
};
void
_mesa_unmarshal_NamedProgramLocalParameters4fvEXT(struct gl_context *ctx, const struct marshal_cmd_NamedProgramLocalParameters4fvEXT *cmd)
{
   const GLuint program = cmd->program;
   const GLenum target = cmd->target;
   const GLuint index = cmd->index;
   const GLsizei count = cmd->count;
   const GLfloat * params = cmd->params;
   CALL_NamedProgramLocalParameters4fvEXT(ctx->CurrentServerDispatch, (program, target, index, count, params));
}
void GLAPIENTRY
_mesa_marshal_NamedProgramLocalParameters4fvEXT(GLuint program, GLenum target, GLuint index, GLsizei count, const GLfloat* params)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_NamedProgramLocalParameters4fvEXT);
   struct marshal_cmd_NamedProgramLocalParameters4fvEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_NamedProgramLocalParameters4fvEXT, cmd_size);
   cmd->program = program;
   cmd->target = target;
   cmd->index = index;
   cmd->count = count;
   memcpy(cmd->params, params, 4 * sizeof(GLfloat));
}


/* BindImageTextureEXT: marshalled asynchronously */
struct marshal_cmd_BindImageTextureEXT
{
   struct marshal_cmd_base cmd_base;
   GLboolean layered;
   GLenum16 access;
   GLuint index;
   GLuint texture;
   GLint level;
   GLint layer;
   GLint format;
};
void
_mesa_unmarshal_BindImageTextureEXT(struct gl_context *ctx, const struct marshal_cmd_BindImageTextureEXT *cmd)
{
   const GLuint index = cmd->index;
   const GLuint texture = cmd->texture;
   const GLint level = cmd->level;
   const GLboolean layered = cmd->layered;
   const GLint layer = cmd->layer;
   const GLenum access = cmd->access;
   const GLint format = cmd->format;
   CALL_BindImageTextureEXT(ctx->CurrentServerDispatch, (index, texture, level, layered, layer, access, format));
}
void GLAPIENTRY
_mesa_marshal_BindImageTextureEXT(GLuint index, GLuint texture, GLint level, GLboolean layered, GLint layer, GLenum access, GLint format)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_BindImageTextureEXT);
   struct marshal_cmd_BindImageTextureEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_BindImageTextureEXT, cmd_size);
   cmd->index = index;
   cmd->texture = texture;
   cmd->level = level;
   cmd->layered = layered;
   cmd->layer = layer;
   cmd->access = access;
   cmd->format = format;
}


/* SubpixelPrecisionBiasNV: marshalled asynchronously */
struct marshal_cmd_SubpixelPrecisionBiasNV
{
   struct marshal_cmd_base cmd_base;
   GLuint xbits;
   GLuint ybits;
};
void
_mesa_unmarshal_SubpixelPrecisionBiasNV(struct gl_context *ctx, const struct marshal_cmd_SubpixelPrecisionBiasNV *cmd)
{
   const GLuint xbits = cmd->xbits;
   const GLuint ybits = cmd->ybits;
   CALL_SubpixelPrecisionBiasNV(ctx->CurrentServerDispatch, (xbits, ybits));
}
void GLAPIENTRY
_mesa_marshal_SubpixelPrecisionBiasNV(GLuint xbits, GLuint ybits)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_SubpixelPrecisionBiasNV);
   struct marshal_cmd_SubpixelPrecisionBiasNV *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_SubpixelPrecisionBiasNV, cmd_size);
   cmd->xbits = xbits;
   cmd->ybits = ybits;
}


/* ConservativeRasterParameterfNV: marshalled asynchronously */
struct marshal_cmd_ConservativeRasterParameterfNV
{
   struct marshal_cmd_base cmd_base;
   GLenum16 pname;
   GLfloat param;
};
void
_mesa_unmarshal_ConservativeRasterParameterfNV(struct gl_context *ctx, const struct marshal_cmd_ConservativeRasterParameterfNV *cmd)
{
   const GLenum pname = cmd->pname;
   const GLfloat param = cmd->param;
   CALL_ConservativeRasterParameterfNV(ctx->CurrentServerDispatch, (pname, param));
}
void GLAPIENTRY
_mesa_marshal_ConservativeRasterParameterfNV(GLenum pname, GLfloat param)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ConservativeRasterParameterfNV);
   struct marshal_cmd_ConservativeRasterParameterfNV *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ConservativeRasterParameterfNV, cmd_size);
   cmd->pname = pname;
   cmd->param = param;
}


/* ConservativeRasterParameteriNV: marshalled asynchronously */
struct marshal_cmd_ConservativeRasterParameteriNV
{
   struct marshal_cmd_base cmd_base;
   GLenum16 pname;
   GLint param;
};
void
_mesa_unmarshal_ConservativeRasterParameteriNV(struct gl_context *ctx, const struct marshal_cmd_ConservativeRasterParameteriNV *cmd)
{
   const GLenum pname = cmd->pname;
   const GLint param = cmd->param;
   CALL_ConservativeRasterParameteriNV(ctx->CurrentServerDispatch, (pname, param));
}
void GLAPIENTRY
_mesa_marshal_ConservativeRasterParameteriNV(GLenum pname, GLint param)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ConservativeRasterParameteriNV);
   struct marshal_cmd_ConservativeRasterParameteriNV *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ConservativeRasterParameteriNV, cmd_size);
   cmd->pname = pname;
   cmd->param = param;
}


/* GetFirstPerfQueryIdINTEL: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetFirstPerfQueryIdINTEL(GLuint * queryId)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetFirstPerfQueryIdINTEL");
   CALL_GetFirstPerfQueryIdINTEL(ctx->CurrentServerDispatch, (queryId));
}


/* GetNextPerfQueryIdINTEL: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetNextPerfQueryIdINTEL(GLuint queryId, GLuint * nextQueryId)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetNextPerfQueryIdINTEL");
   CALL_GetNextPerfQueryIdINTEL(ctx->CurrentServerDispatch, (queryId, nextQueryId));
}


/* GetPerfQueryIdByNameINTEL: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetPerfQueryIdByNameINTEL(GLchar * queryName, GLuint * queryId)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetPerfQueryIdByNameINTEL");
   CALL_GetPerfQueryIdByNameINTEL(ctx->CurrentServerDispatch, (queryName, queryId));
}


/* GetPerfQueryInfoINTEL: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetPerfQueryInfoINTEL(GLuint queryId, GLuint queryNameLength, GLchar * queryName, GLuint * dataSize, GLuint * noCounters, GLuint * noInstances, GLuint * capsMask)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetPerfQueryInfoINTEL");
   CALL_GetPerfQueryInfoINTEL(ctx->CurrentServerDispatch, (queryId, queryNameLength, queryName, dataSize, noCounters, noInstances, capsMask));
}


/* GetPerfCounterInfoINTEL: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetPerfCounterInfoINTEL(GLuint queryId, GLuint counterId, GLuint counterNameLength, GLchar * counterName, GLuint counterDescLength, GLchar * counterDesc, GLuint * counterOffset, GLuint * counterDataSize, GLuint * counterTypeEnum, GLuint * counterDataTypeEnum, GLuint64 * rawCounterMaxValue)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetPerfCounterInfoINTEL");
   CALL_GetPerfCounterInfoINTEL(ctx->CurrentServerDispatch, (queryId, counterId, counterNameLength, counterName, counterDescLength, counterDesc, counterOffset, counterDataSize, counterTypeEnum, counterDataTypeEnum, rawCounterMaxValue));
}


/* CreatePerfQueryINTEL: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_CreatePerfQueryINTEL(GLuint queryId, GLuint * queryHandle)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "CreatePerfQueryINTEL");
   CALL_CreatePerfQueryINTEL(ctx->CurrentServerDispatch, (queryId, queryHandle));
}


/* DeletePerfQueryINTEL: marshalled asynchronously */
struct marshal_cmd_DeletePerfQueryINTEL
{
   struct marshal_cmd_base cmd_base;
   GLuint queryHandle;
};
void
_mesa_unmarshal_DeletePerfQueryINTEL(struct gl_context *ctx, const struct marshal_cmd_DeletePerfQueryINTEL *cmd)
{
   const GLuint queryHandle = cmd->queryHandle;
   CALL_DeletePerfQueryINTEL(ctx->CurrentServerDispatch, (queryHandle));
}
void GLAPIENTRY
_mesa_marshal_DeletePerfQueryINTEL(GLuint queryHandle)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_DeletePerfQueryINTEL);
   struct marshal_cmd_DeletePerfQueryINTEL *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_DeletePerfQueryINTEL, cmd_size);
   cmd->queryHandle = queryHandle;
}


/* BeginPerfQueryINTEL: marshalled asynchronously */
struct marshal_cmd_BeginPerfQueryINTEL
{
   struct marshal_cmd_base cmd_base;
   GLuint queryHandle;
};
void
_mesa_unmarshal_BeginPerfQueryINTEL(struct gl_context *ctx, const struct marshal_cmd_BeginPerfQueryINTEL *cmd)
{
   const GLuint queryHandle = cmd->queryHandle;
   CALL_BeginPerfQueryINTEL(ctx->CurrentServerDispatch, (queryHandle));
}
void GLAPIENTRY
_mesa_marshal_BeginPerfQueryINTEL(GLuint queryHandle)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_BeginPerfQueryINTEL);
   struct marshal_cmd_BeginPerfQueryINTEL *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_BeginPerfQueryINTEL, cmd_size);
   cmd->queryHandle = queryHandle;
}


/* EndPerfQueryINTEL: marshalled asynchronously */
struct marshal_cmd_EndPerfQueryINTEL
{
   struct marshal_cmd_base cmd_base;
   GLuint queryHandle;
};
void
_mesa_unmarshal_EndPerfQueryINTEL(struct gl_context *ctx, const struct marshal_cmd_EndPerfQueryINTEL *cmd)
{
   const GLuint queryHandle = cmd->queryHandle;
   CALL_EndPerfQueryINTEL(ctx->CurrentServerDispatch, (queryHandle));
}
void GLAPIENTRY
_mesa_marshal_EndPerfQueryINTEL(GLuint queryHandle)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_EndPerfQueryINTEL);
   struct marshal_cmd_EndPerfQueryINTEL *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_EndPerfQueryINTEL, cmd_size);
   cmd->queryHandle = queryHandle;
}


/* GetPerfQueryDataINTEL: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetPerfQueryDataINTEL(GLuint queryHandle, GLuint flags, GLsizei dataSize, GLvoid * data, GLuint * bytesWritten)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetPerfQueryDataINTEL");
   CALL_GetPerfQueryDataINTEL(ctx->CurrentServerDispatch, (queryHandle, flags, dataSize, data, bytesWritten));
}


/* AlphaToCoverageDitherControlNV: marshalled asynchronously */
struct marshal_cmd_AlphaToCoverageDitherControlNV
{
   struct marshal_cmd_base cmd_base;
   GLenum16 mode;
};
void
_mesa_unmarshal_AlphaToCoverageDitherControlNV(struct gl_context *ctx, const struct marshal_cmd_AlphaToCoverageDitherControlNV *cmd)
{
   const GLenum mode = cmd->mode;
   CALL_AlphaToCoverageDitherControlNV(ctx->CurrentServerDispatch, (mode));
}
void GLAPIENTRY
_mesa_marshal_AlphaToCoverageDitherControlNV(GLenum mode)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_AlphaToCoverageDitherControlNV);
   struct marshal_cmd_AlphaToCoverageDitherControlNV *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_AlphaToCoverageDitherControlNV, cmd_size);
   cmd->mode = mode;
}


/* PolygonOffsetClampEXT: marshalled asynchronously */
struct marshal_cmd_PolygonOffsetClampEXT
{
   struct marshal_cmd_base cmd_base;
   GLfloat factor;
   GLfloat units;
   GLfloat clamp;
};
void
_mesa_unmarshal_PolygonOffsetClampEXT(struct gl_context *ctx, const struct marshal_cmd_PolygonOffsetClampEXT *cmd)
{
   const GLfloat factor = cmd->factor;
   const GLfloat units = cmd->units;
   const GLfloat clamp = cmd->clamp;
   CALL_PolygonOffsetClampEXT(ctx->CurrentServerDispatch, (factor, units, clamp));
}
void GLAPIENTRY
_mesa_marshal_PolygonOffsetClampEXT(GLfloat factor, GLfloat units, GLfloat clamp)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_PolygonOffsetClampEXT);
   struct marshal_cmd_PolygonOffsetClampEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_PolygonOffsetClampEXT, cmd_size);
   cmd->factor = factor;
   cmd->units = units;
   cmd->clamp = clamp;
}


/* WindowRectanglesEXT: marshalled asynchronously */
struct marshal_cmd_WindowRectanglesEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 mode;
   GLsizei count;
   /* Next safe_mul((4 * count), 1 * sizeof(GLint)) bytes are GLint box[(4 * count)] */
};
void
_mesa_unmarshal_WindowRectanglesEXT(struct gl_context *ctx, const struct marshal_cmd_WindowRectanglesEXT *cmd)
{
   const GLenum mode = cmd->mode;
   const GLsizei count = cmd->count;
   GLint * box;
   const char *variable_data = (const char *) (cmd + 1);
   box = (GLint *) variable_data;
   CALL_WindowRectanglesEXT(ctx->CurrentServerDispatch, (mode, count, box));
}
void GLAPIENTRY
_mesa_marshal_WindowRectanglesEXT(GLenum mode, GLsizei count, const GLint * box)
{
   GET_CURRENT_CONTEXT(ctx);
   int box_size = safe_mul((4 * count), 1 * sizeof(GLint));
   int cmd_size = sizeof(struct marshal_cmd_WindowRectanglesEXT) + box_size;
   struct marshal_cmd_WindowRectanglesEXT *cmd;
   if (unlikely(box_size < 0 || (box_size > 0 && !box) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "WindowRectanglesEXT");
      CALL_WindowRectanglesEXT(ctx->CurrentServerDispatch, (mode, count, box));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_WindowRectanglesEXT, cmd_size);
   cmd->mode = mode;
   cmd->count = count;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, box, box_size);
}


/* FramebufferFetchBarrierEXT: marshalled asynchronously */
struct marshal_cmd_FramebufferFetchBarrierEXT
{
   struct marshal_cmd_base cmd_base;
};
void
_mesa_unmarshal_FramebufferFetchBarrierEXT(struct gl_context *ctx, const struct marshal_cmd_FramebufferFetchBarrierEXT *cmd)
{
   CALL_FramebufferFetchBarrierEXT(ctx->CurrentServerDispatch, ());
}
void GLAPIENTRY
_mesa_marshal_FramebufferFetchBarrierEXT(void)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_FramebufferFetchBarrierEXT);
   struct marshal_cmd_FramebufferFetchBarrierEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_FramebufferFetchBarrierEXT, cmd_size);
   (void) cmd;
}


/* RenderbufferStorageMultisampleAdvancedAMD: marshalled asynchronously */
struct marshal_cmd_RenderbufferStorageMultisampleAdvancedAMD
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLenum16 internalformat;
   GLsizei samples;
   GLsizei storageSamples;
   GLsizei width;
   GLsizei height;
};
void
_mesa_unmarshal_RenderbufferStorageMultisampleAdvancedAMD(struct gl_context *ctx, const struct marshal_cmd_RenderbufferStorageMultisampleAdvancedAMD *cmd)
{
   const GLenum target = cmd->target;
   const GLsizei samples = cmd->samples;
   const GLsizei storageSamples = cmd->storageSamples;
   const GLenum internalformat = cmd->internalformat;
   const GLsizei width = cmd->width;
   const GLsizei height = cmd->height;
   CALL_RenderbufferStorageMultisampleAdvancedAMD(ctx->CurrentServerDispatch, (target, samples, storageSamples, internalformat, width, height));
}
void GLAPIENTRY
_mesa_marshal_RenderbufferStorageMultisampleAdvancedAMD(GLenum target, GLsizei samples, GLsizei storageSamples, GLenum internalformat, GLsizei width, GLsizei height)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_RenderbufferStorageMultisampleAdvancedAMD);
   struct marshal_cmd_RenderbufferStorageMultisampleAdvancedAMD *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_RenderbufferStorageMultisampleAdvancedAMD, cmd_size);
   cmd->target = target;
   cmd->samples = samples;
   cmd->storageSamples = storageSamples;
   cmd->internalformat = internalformat;
   cmd->width = width;
   cmd->height = height;
}


/* NamedRenderbufferStorageMultisampleAdvancedAMD: marshalled asynchronously */
struct marshal_cmd_NamedRenderbufferStorageMultisampleAdvancedAMD
{
   struct marshal_cmd_base cmd_base;
   GLenum16 internalformat;
   GLuint renderbuffer;
   GLsizei samples;
   GLsizei storageSamples;
   GLsizei width;
   GLsizei height;
};
void
_mesa_unmarshal_NamedRenderbufferStorageMultisampleAdvancedAMD(struct gl_context *ctx, const struct marshal_cmd_NamedRenderbufferStorageMultisampleAdvancedAMD *cmd)
{
   const GLuint renderbuffer = cmd->renderbuffer;
   const GLsizei samples = cmd->samples;
   const GLsizei storageSamples = cmd->storageSamples;
   const GLenum internalformat = cmd->internalformat;
   const GLsizei width = cmd->width;
   const GLsizei height = cmd->height;
   CALL_NamedRenderbufferStorageMultisampleAdvancedAMD(ctx->CurrentServerDispatch, (renderbuffer, samples, storageSamples, internalformat, width, height));
}
void GLAPIENTRY
_mesa_marshal_NamedRenderbufferStorageMultisampleAdvancedAMD(GLuint renderbuffer, GLsizei samples, GLsizei storageSamples, GLenum internalformat, GLsizei width, GLsizei height)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_NamedRenderbufferStorageMultisampleAdvancedAMD);
   struct marshal_cmd_NamedRenderbufferStorageMultisampleAdvancedAMD *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_NamedRenderbufferStorageMultisampleAdvancedAMD, cmd_size);
   cmd->renderbuffer = renderbuffer;
   cmd->samples = samples;
   cmd->storageSamples = storageSamples;
   cmd->internalformat = internalformat;
   cmd->width = width;
   cmd->height = height;
}


/* StencilFuncSeparateATI: marshalled asynchronously */
struct marshal_cmd_StencilFuncSeparateATI
{
   struct marshal_cmd_base cmd_base;
   GLenum16 frontfunc;
   GLenum16 backfunc;
   GLint ref;
   GLuint mask;
};
void
_mesa_unmarshal_StencilFuncSeparateATI(struct gl_context *ctx, const struct marshal_cmd_StencilFuncSeparateATI *cmd)
{
   const GLenum frontfunc = cmd->frontfunc;
   const GLenum backfunc = cmd->backfunc;
   const GLint ref = cmd->ref;
   const GLuint mask = cmd->mask;
   CALL_StencilFuncSeparateATI(ctx->CurrentServerDispatch, (frontfunc, backfunc, ref, mask));
}
void GLAPIENTRY
_mesa_marshal_StencilFuncSeparateATI(GLenum frontfunc, GLenum backfunc, GLint ref, GLuint mask)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_StencilFuncSeparateATI);
   struct marshal_cmd_StencilFuncSeparateATI *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_StencilFuncSeparateATI, cmd_size);
   cmd->frontfunc = frontfunc;
   cmd->backfunc = backfunc;
   cmd->ref = ref;
   cmd->mask = mask;
}


/* ProgramEnvParameters4fvEXT: marshalled asynchronously */
struct marshal_cmd_ProgramEnvParameters4fvEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLuint index;
   GLsizei count;
   GLfloat params[4];
};
void
_mesa_unmarshal_ProgramEnvParameters4fvEXT(struct gl_context *ctx, const struct marshal_cmd_ProgramEnvParameters4fvEXT *cmd)
{
   const GLenum target = cmd->target;
   const GLuint index = cmd->index;
   const GLsizei count = cmd->count;
   const GLfloat * params = cmd->params;
   CALL_ProgramEnvParameters4fvEXT(ctx->CurrentServerDispatch, (target, index, count, params));
}
void GLAPIENTRY
_mesa_marshal_ProgramEnvParameters4fvEXT(GLenum target, GLuint index, GLsizei count, const GLfloat * params)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ProgramEnvParameters4fvEXT);
   struct marshal_cmd_ProgramEnvParameters4fvEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ProgramEnvParameters4fvEXT, cmd_size);
   cmd->target = target;
   cmd->index = index;
   cmd->count = count;
   memcpy(cmd->params, params, 4 * sizeof(GLfloat));
}


/* ProgramLocalParameters4fvEXT: marshalled asynchronously */
struct marshal_cmd_ProgramLocalParameters4fvEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLuint index;
   GLsizei count;
   GLfloat params[4];
};
void
_mesa_unmarshal_ProgramLocalParameters4fvEXT(struct gl_context *ctx, const struct marshal_cmd_ProgramLocalParameters4fvEXT *cmd)
{
   const GLenum target = cmd->target;
   const GLuint index = cmd->index;
   const GLsizei count = cmd->count;
   const GLfloat * params = cmd->params;
   CALL_ProgramLocalParameters4fvEXT(ctx->CurrentServerDispatch, (target, index, count, params));
}
void GLAPIENTRY
_mesa_marshal_ProgramLocalParameters4fvEXT(GLenum target, GLuint index, GLsizei count, const GLfloat * params)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ProgramLocalParameters4fvEXT);
   struct marshal_cmd_ProgramLocalParameters4fvEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ProgramLocalParameters4fvEXT, cmd_size);
   cmd->target = target;
   cmd->index = index;
   cmd->count = count;
   memcpy(cmd->params, params, 4 * sizeof(GLfloat));
}


/* EGLImageTargetTexture2DOES: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_EGLImageTargetTexture2DOES(GLenum target, GLvoid * writeOffset)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "EGLImageTargetTexture2DOES");
   CALL_EGLImageTargetTexture2DOES(ctx->CurrentServerDispatch, (target, writeOffset));
}


/* EGLImageTargetRenderbufferStorageOES: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_EGLImageTargetRenderbufferStorageOES(GLenum target, GLvoid * writeOffset)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "EGLImageTargetRenderbufferStorageOES");
   CALL_EGLImageTargetRenderbufferStorageOES(ctx->CurrentServerDispatch, (target, writeOffset));
}


/* EGLImageTargetTexStorageEXT: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_EGLImageTargetTexStorageEXT(GLenum target, GLvoid * image, const GLint * attrib_list)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "EGLImageTargetTexStorageEXT");
   CALL_EGLImageTargetTexStorageEXT(ctx->CurrentServerDispatch, (target, image, attrib_list));
}


/* EGLImageTargetTextureStorageEXT: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_EGLImageTargetTextureStorageEXT(GLuint texture, GLvoid * image, const GLint * attrib_list)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "EGLImageTargetTextureStorageEXT");
   CALL_EGLImageTargetTextureStorageEXT(ctx->CurrentServerDispatch, (texture, image, attrib_list));
}


/* ClearColorIiEXT: marshalled asynchronously */
struct marshal_cmd_ClearColorIiEXT
{
   struct marshal_cmd_base cmd_base;
   GLint r;
   GLint g;
   GLint b;
   GLint a;
};
void
_mesa_unmarshal_ClearColorIiEXT(struct gl_context *ctx, const struct marshal_cmd_ClearColorIiEXT *cmd)
{
   const GLint r = cmd->r;
   const GLint g = cmd->g;
   const GLint b = cmd->b;
   const GLint a = cmd->a;
   CALL_ClearColorIiEXT(ctx->CurrentServerDispatch, (r, g, b, a));
}
void GLAPIENTRY
_mesa_marshal_ClearColorIiEXT(GLint r, GLint g, GLint b, GLint a)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ClearColorIiEXT);
   struct marshal_cmd_ClearColorIiEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ClearColorIiEXT, cmd_size);
   cmd->r = r;
   cmd->g = g;
   cmd->b = b;
   cmd->a = a;
}


/* ClearColorIuiEXT: marshalled asynchronously */
struct marshal_cmd_ClearColorIuiEXT
{
   struct marshal_cmd_base cmd_base;
   GLuint r;
   GLuint g;
   GLuint b;
   GLuint a;
};
void
_mesa_unmarshal_ClearColorIuiEXT(struct gl_context *ctx, const struct marshal_cmd_ClearColorIuiEXT *cmd)
{
   const GLuint r = cmd->r;
   const GLuint g = cmd->g;
   const GLuint b = cmd->b;
   const GLuint a = cmd->a;
   CALL_ClearColorIuiEXT(ctx->CurrentServerDispatch, (r, g, b, a));
}
void GLAPIENTRY
_mesa_marshal_ClearColorIuiEXT(GLuint r, GLuint g, GLuint b, GLuint a)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ClearColorIuiEXT);
   struct marshal_cmd_ClearColorIuiEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ClearColorIuiEXT, cmd_size);
   cmd->r = r;
   cmd->g = g;
   cmd->b = b;
   cmd->a = a;
}


/* TexParameterIiv: marshalled asynchronously */
struct marshal_cmd_TexParameterIiv
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLenum16 pname;
   /* Next safe_mul(_mesa_tex_param_enum_to_count(pname), 1 * sizeof(GLint)) bytes are GLint params[_mesa_tex_param_enum_to_count(pname)] */
};
void
_mesa_unmarshal_TexParameterIiv(struct gl_context *ctx, const struct marshal_cmd_TexParameterIiv *cmd)
{
   const GLenum target = cmd->target;
   const GLenum pname = cmd->pname;
   GLint * params;
   const char *variable_data = (const char *) (cmd + 1);
   params = (GLint *) variable_data;
   CALL_TexParameterIiv(ctx->CurrentServerDispatch, (target, pname, params));
}
void GLAPIENTRY
_mesa_marshal_TexParameterIiv(GLenum target, GLenum pname, const GLint * params)
{
   GET_CURRENT_CONTEXT(ctx);
   int params_size = safe_mul(_mesa_tex_param_enum_to_count(pname), 1 * sizeof(GLint));
   int cmd_size = sizeof(struct marshal_cmd_TexParameterIiv) + params_size;
   struct marshal_cmd_TexParameterIiv *cmd;
   if (unlikely(params_size < 0 || (params_size > 0 && !params) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "TexParameterIiv");
      CALL_TexParameterIiv(ctx->CurrentServerDispatch, (target, pname, params));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexParameterIiv, cmd_size);
   cmd->target = target;
   cmd->pname = pname;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, params, params_size);
}


/* TexParameterIuiv: marshalled asynchronously */
struct marshal_cmd_TexParameterIuiv
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLenum16 pname;
   /* Next safe_mul(_mesa_tex_param_enum_to_count(pname), 1 * sizeof(GLuint)) bytes are GLuint params[_mesa_tex_param_enum_to_count(pname)] */
};
void
_mesa_unmarshal_TexParameterIuiv(struct gl_context *ctx, const struct marshal_cmd_TexParameterIuiv *cmd)
{
   const GLenum target = cmd->target;
   const GLenum pname = cmd->pname;
   GLuint * params;
   const char *variable_data = (const char *) (cmd + 1);
   params = (GLuint *) variable_data;
   CALL_TexParameterIuiv(ctx->CurrentServerDispatch, (target, pname, params));
}
void GLAPIENTRY
_mesa_marshal_TexParameterIuiv(GLenum target, GLenum pname, const GLuint * params)
{
   GET_CURRENT_CONTEXT(ctx);
   int params_size = safe_mul(_mesa_tex_param_enum_to_count(pname), 1 * sizeof(GLuint));
   int cmd_size = sizeof(struct marshal_cmd_TexParameterIuiv) + params_size;
   struct marshal_cmd_TexParameterIuiv *cmd;
   if (unlikely(params_size < 0 || (params_size > 0 && !params) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "TexParameterIuiv");
      CALL_TexParameterIuiv(ctx->CurrentServerDispatch, (target, pname, params));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexParameterIuiv, cmd_size);
   cmd->target = target;
   cmd->pname = pname;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, params, params_size);
}


/* GetTexParameterIiv: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetTexParameterIiv(GLenum target, GLenum pname, GLint * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetTexParameterIiv");
   CALL_GetTexParameterIiv(ctx->CurrentServerDispatch, (target, pname, params));
}


/* GetTexParameterIuiv: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetTexParameterIuiv(GLenum target, GLenum pname, GLuint * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetTexParameterIuiv");
   CALL_GetTexParameterIuiv(ctx->CurrentServerDispatch, (target, pname, params));
}


/* VertexAttribI1iEXT: marshalled asynchronously */
struct marshal_cmd_VertexAttribI1iEXT
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLint x;
};
void
_mesa_unmarshal_VertexAttribI1iEXT(struct gl_context *ctx, const struct marshal_cmd_VertexAttribI1iEXT *cmd)
{
   const GLuint index = cmd->index;
   const GLint x = cmd->x;
   CALL_VertexAttribI1iEXT(ctx->CurrentServerDispatch, (index, x));
}
void GLAPIENTRY
_mesa_marshal_VertexAttribI1iEXT(GLuint index, GLint x)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttribI1iEXT);
   struct marshal_cmd_VertexAttribI1iEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttribI1iEXT, cmd_size);
   cmd->index = index;
   cmd->x = x;
}


/* VertexAttribI2iEXT: marshalled asynchronously */
struct marshal_cmd_VertexAttribI2iEXT
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLint x;
   GLint y;
};
void
_mesa_unmarshal_VertexAttribI2iEXT(struct gl_context *ctx, const struct marshal_cmd_VertexAttribI2iEXT *cmd)
{
   const GLuint index = cmd->index;
   const GLint x = cmd->x;
   const GLint y = cmd->y;
   CALL_VertexAttribI2iEXT(ctx->CurrentServerDispatch, (index, x, y));
}
void GLAPIENTRY
_mesa_marshal_VertexAttribI2iEXT(GLuint index, GLint x, GLint y)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttribI2iEXT);
   struct marshal_cmd_VertexAttribI2iEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttribI2iEXT, cmd_size);
   cmd->index = index;
   cmd->x = x;
   cmd->y = y;
}


/* VertexAttribI3iEXT: marshalled asynchronously */
struct marshal_cmd_VertexAttribI3iEXT
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLint x;
   GLint y;
   GLint z;
};
void
_mesa_unmarshal_VertexAttribI3iEXT(struct gl_context *ctx, const struct marshal_cmd_VertexAttribI3iEXT *cmd)
{
   const GLuint index = cmd->index;
   const GLint x = cmd->x;
   const GLint y = cmd->y;
   const GLint z = cmd->z;
   CALL_VertexAttribI3iEXT(ctx->CurrentServerDispatch, (index, x, y, z));
}
void GLAPIENTRY
_mesa_marshal_VertexAttribI3iEXT(GLuint index, GLint x, GLint y, GLint z)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttribI3iEXT);
   struct marshal_cmd_VertexAttribI3iEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttribI3iEXT, cmd_size);
   cmd->index = index;
   cmd->x = x;
   cmd->y = y;
   cmd->z = z;
}


/* VertexAttribI4iEXT: marshalled asynchronously */
struct marshal_cmd_VertexAttribI4iEXT
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLint x;
   GLint y;
   GLint z;
   GLint w;
};
void
_mesa_unmarshal_VertexAttribI4iEXT(struct gl_context *ctx, const struct marshal_cmd_VertexAttribI4iEXT *cmd)
{
   const GLuint index = cmd->index;
   const GLint x = cmd->x;
   const GLint y = cmd->y;
   const GLint z = cmd->z;
   const GLint w = cmd->w;
   CALL_VertexAttribI4iEXT(ctx->CurrentServerDispatch, (index, x, y, z, w));
}
void GLAPIENTRY
_mesa_marshal_VertexAttribI4iEXT(GLuint index, GLint x, GLint y, GLint z, GLint w)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttribI4iEXT);
   struct marshal_cmd_VertexAttribI4iEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttribI4iEXT, cmd_size);
   cmd->index = index;
   cmd->x = x;
   cmd->y = y;
   cmd->z = z;
   cmd->w = w;
}


/* VertexAttribI1uiEXT: marshalled asynchronously */
struct marshal_cmd_VertexAttribI1uiEXT
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLuint x;
};
void
_mesa_unmarshal_VertexAttribI1uiEXT(struct gl_context *ctx, const struct marshal_cmd_VertexAttribI1uiEXT *cmd)
{
   const GLuint index = cmd->index;
   const GLuint x = cmd->x;
   CALL_VertexAttribI1uiEXT(ctx->CurrentServerDispatch, (index, x));
}
void GLAPIENTRY
_mesa_marshal_VertexAttribI1uiEXT(GLuint index, GLuint x)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttribI1uiEXT);
   struct marshal_cmd_VertexAttribI1uiEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttribI1uiEXT, cmd_size);
   cmd->index = index;
   cmd->x = x;
}


/* VertexAttribI2uiEXT: marshalled asynchronously */
struct marshal_cmd_VertexAttribI2uiEXT
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLuint x;
   GLuint y;
};
void
_mesa_unmarshal_VertexAttribI2uiEXT(struct gl_context *ctx, const struct marshal_cmd_VertexAttribI2uiEXT *cmd)
{
   const GLuint index = cmd->index;
   const GLuint x = cmd->x;
   const GLuint y = cmd->y;
   CALL_VertexAttribI2uiEXT(ctx->CurrentServerDispatch, (index, x, y));
}
void GLAPIENTRY
_mesa_marshal_VertexAttribI2uiEXT(GLuint index, GLuint x, GLuint y)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttribI2uiEXT);
   struct marshal_cmd_VertexAttribI2uiEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttribI2uiEXT, cmd_size);
   cmd->index = index;
   cmd->x = x;
   cmd->y = y;
}


/* VertexAttribI3uiEXT: marshalled asynchronously */
struct marshal_cmd_VertexAttribI3uiEXT
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLuint x;
   GLuint y;
   GLuint z;
};
void
_mesa_unmarshal_VertexAttribI3uiEXT(struct gl_context *ctx, const struct marshal_cmd_VertexAttribI3uiEXT *cmd)
{
   const GLuint index = cmd->index;
   const GLuint x = cmd->x;
   const GLuint y = cmd->y;
   const GLuint z = cmd->z;
   CALL_VertexAttribI3uiEXT(ctx->CurrentServerDispatch, (index, x, y, z));
}
void GLAPIENTRY
_mesa_marshal_VertexAttribI3uiEXT(GLuint index, GLuint x, GLuint y, GLuint z)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttribI3uiEXT);
   struct marshal_cmd_VertexAttribI3uiEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttribI3uiEXT, cmd_size);
   cmd->index = index;
   cmd->x = x;
   cmd->y = y;
   cmd->z = z;
}


/* VertexAttribI4uiEXT: marshalled asynchronously */
struct marshal_cmd_VertexAttribI4uiEXT
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLuint x;
   GLuint y;
   GLuint z;
   GLuint w;
};
void
_mesa_unmarshal_VertexAttribI4uiEXT(struct gl_context *ctx, const struct marshal_cmd_VertexAttribI4uiEXT *cmd)
{
   const GLuint index = cmd->index;
   const GLuint x = cmd->x;
   const GLuint y = cmd->y;
   const GLuint z = cmd->z;
   const GLuint w = cmd->w;
   CALL_VertexAttribI4uiEXT(ctx->CurrentServerDispatch, (index, x, y, z, w));
}
void GLAPIENTRY
_mesa_marshal_VertexAttribI4uiEXT(GLuint index, GLuint x, GLuint y, GLuint z, GLuint w)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttribI4uiEXT);
   struct marshal_cmd_VertexAttribI4uiEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttribI4uiEXT, cmd_size);
   cmd->index = index;
   cmd->x = x;
   cmd->y = y;
   cmd->z = z;
   cmd->w = w;
}


/* VertexAttribI1iv: marshalled asynchronously */
struct marshal_cmd_VertexAttribI1iv
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLint v[1];
};
void
_mesa_unmarshal_VertexAttribI1iv(struct gl_context *ctx, const struct marshal_cmd_VertexAttribI1iv *cmd)
{
   const GLuint index = cmd->index;
   const GLint * v = cmd->v;
   CALL_VertexAttribI1iv(ctx->CurrentServerDispatch, (index, v));
}
void GLAPIENTRY
_mesa_marshal_VertexAttribI1iv(GLuint index, const GLint * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttribI1iv);
   struct marshal_cmd_VertexAttribI1iv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttribI1iv, cmd_size);
   cmd->index = index;
   memcpy(cmd->v, v, 1 * sizeof(GLint));
}


/* VertexAttribI2ivEXT: marshalled asynchronously */
struct marshal_cmd_VertexAttribI2ivEXT
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLint v[2];
};
void
_mesa_unmarshal_VertexAttribI2ivEXT(struct gl_context *ctx, const struct marshal_cmd_VertexAttribI2ivEXT *cmd)
{
   const GLuint index = cmd->index;
   const GLint * v = cmd->v;
   CALL_VertexAttribI2ivEXT(ctx->CurrentServerDispatch, (index, v));
}
void GLAPIENTRY
_mesa_marshal_VertexAttribI2ivEXT(GLuint index, const GLint * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttribI2ivEXT);
   struct marshal_cmd_VertexAttribI2ivEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttribI2ivEXT, cmd_size);
   cmd->index = index;
   memcpy(cmd->v, v, 2 * sizeof(GLint));
}


/* VertexAttribI3ivEXT: marshalled asynchronously */
struct marshal_cmd_VertexAttribI3ivEXT
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLint v[3];
};
void
_mesa_unmarshal_VertexAttribI3ivEXT(struct gl_context *ctx, const struct marshal_cmd_VertexAttribI3ivEXT *cmd)
{
   const GLuint index = cmd->index;
   const GLint * v = cmd->v;
   CALL_VertexAttribI3ivEXT(ctx->CurrentServerDispatch, (index, v));
}
void GLAPIENTRY
_mesa_marshal_VertexAttribI3ivEXT(GLuint index, const GLint * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttribI3ivEXT);
   struct marshal_cmd_VertexAttribI3ivEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttribI3ivEXT, cmd_size);
   cmd->index = index;
   memcpy(cmd->v, v, 3 * sizeof(GLint));
}


/* VertexAttribI4ivEXT: marshalled asynchronously */
struct marshal_cmd_VertexAttribI4ivEXT
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLint v[4];
};
void
_mesa_unmarshal_VertexAttribI4ivEXT(struct gl_context *ctx, const struct marshal_cmd_VertexAttribI4ivEXT *cmd)
{
   const GLuint index = cmd->index;
   const GLint * v = cmd->v;
   CALL_VertexAttribI4ivEXT(ctx->CurrentServerDispatch, (index, v));
}
void GLAPIENTRY
_mesa_marshal_VertexAttribI4ivEXT(GLuint index, const GLint * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttribI4ivEXT);
   struct marshal_cmd_VertexAttribI4ivEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttribI4ivEXT, cmd_size);
   cmd->index = index;
   memcpy(cmd->v, v, 4 * sizeof(GLint));
}


/* VertexAttribI1uiv: marshalled asynchronously */
struct marshal_cmd_VertexAttribI1uiv
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLuint v[1];
};
void
_mesa_unmarshal_VertexAttribI1uiv(struct gl_context *ctx, const struct marshal_cmd_VertexAttribI1uiv *cmd)
{
   const GLuint index = cmd->index;
   const GLuint * v = cmd->v;
   CALL_VertexAttribI1uiv(ctx->CurrentServerDispatch, (index, v));
}
void GLAPIENTRY
_mesa_marshal_VertexAttribI1uiv(GLuint index, const GLuint * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttribI1uiv);
   struct marshal_cmd_VertexAttribI1uiv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttribI1uiv, cmd_size);
   cmd->index = index;
   memcpy(cmd->v, v, 1 * sizeof(GLuint));
}


/* VertexAttribI2uivEXT: marshalled asynchronously */
struct marshal_cmd_VertexAttribI2uivEXT
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLuint v[2];
};
void
_mesa_unmarshal_VertexAttribI2uivEXT(struct gl_context *ctx, const struct marshal_cmd_VertexAttribI2uivEXT *cmd)
{
   const GLuint index = cmd->index;
   const GLuint * v = cmd->v;
   CALL_VertexAttribI2uivEXT(ctx->CurrentServerDispatch, (index, v));
}
void GLAPIENTRY
_mesa_marshal_VertexAttribI2uivEXT(GLuint index, const GLuint * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttribI2uivEXT);
   struct marshal_cmd_VertexAttribI2uivEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttribI2uivEXT, cmd_size);
   cmd->index = index;
   memcpy(cmd->v, v, 2 * sizeof(GLuint));
}


/* VertexAttribI3uivEXT: marshalled asynchronously */
struct marshal_cmd_VertexAttribI3uivEXT
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLuint v[3];
};
void
_mesa_unmarshal_VertexAttribI3uivEXT(struct gl_context *ctx, const struct marshal_cmd_VertexAttribI3uivEXT *cmd)
{
   const GLuint index = cmd->index;
   const GLuint * v = cmd->v;
   CALL_VertexAttribI3uivEXT(ctx->CurrentServerDispatch, (index, v));
}
void GLAPIENTRY
_mesa_marshal_VertexAttribI3uivEXT(GLuint index, const GLuint * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttribI3uivEXT);
   struct marshal_cmd_VertexAttribI3uivEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttribI3uivEXT, cmd_size);
   cmd->index = index;
   memcpy(cmd->v, v, 3 * sizeof(GLuint));
}


/* VertexAttribI4uivEXT: marshalled asynchronously */
struct marshal_cmd_VertexAttribI4uivEXT
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLuint v[4];
};
void
_mesa_unmarshal_VertexAttribI4uivEXT(struct gl_context *ctx, const struct marshal_cmd_VertexAttribI4uivEXT *cmd)
{
   const GLuint index = cmd->index;
   const GLuint * v = cmd->v;
   CALL_VertexAttribI4uivEXT(ctx->CurrentServerDispatch, (index, v));
}
void GLAPIENTRY
_mesa_marshal_VertexAttribI4uivEXT(GLuint index, const GLuint * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttribI4uivEXT);
   struct marshal_cmd_VertexAttribI4uivEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttribI4uivEXT, cmd_size);
   cmd->index = index;
   memcpy(cmd->v, v, 4 * sizeof(GLuint));
}


/* VertexAttribI4bv: marshalled asynchronously */
struct marshal_cmd_VertexAttribI4bv
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLbyte v[4];
};
void
_mesa_unmarshal_VertexAttribI4bv(struct gl_context *ctx, const struct marshal_cmd_VertexAttribI4bv *cmd)
{
   const GLuint index = cmd->index;
   const GLbyte * v = cmd->v;
   CALL_VertexAttribI4bv(ctx->CurrentServerDispatch, (index, v));
}
void GLAPIENTRY
_mesa_marshal_VertexAttribI4bv(GLuint index, const GLbyte * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttribI4bv);
   struct marshal_cmd_VertexAttribI4bv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttribI4bv, cmd_size);
   cmd->index = index;
   memcpy(cmd->v, v, 4 * sizeof(GLbyte));
}


/* VertexAttribI4sv: marshalled asynchronously */
struct marshal_cmd_VertexAttribI4sv
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLshort v[4];
};
void
_mesa_unmarshal_VertexAttribI4sv(struct gl_context *ctx, const struct marshal_cmd_VertexAttribI4sv *cmd)
{
   const GLuint index = cmd->index;
   const GLshort * v = cmd->v;
   CALL_VertexAttribI4sv(ctx->CurrentServerDispatch, (index, v));
}
void GLAPIENTRY
_mesa_marshal_VertexAttribI4sv(GLuint index, const GLshort * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttribI4sv);
   struct marshal_cmd_VertexAttribI4sv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttribI4sv, cmd_size);
   cmd->index = index;
   memcpy(cmd->v, v, 4 * sizeof(GLshort));
}


/* VertexAttribI4ubv: marshalled asynchronously */
struct marshal_cmd_VertexAttribI4ubv
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLubyte v[4];
};
void
_mesa_unmarshal_VertexAttribI4ubv(struct gl_context *ctx, const struct marshal_cmd_VertexAttribI4ubv *cmd)
{
   const GLuint index = cmd->index;
   const GLubyte * v = cmd->v;
   CALL_VertexAttribI4ubv(ctx->CurrentServerDispatch, (index, v));
}
void GLAPIENTRY
_mesa_marshal_VertexAttribI4ubv(GLuint index, const GLubyte * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttribI4ubv);
   struct marshal_cmd_VertexAttribI4ubv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttribI4ubv, cmd_size);
   cmd->index = index;
   memcpy(cmd->v, v, 4 * sizeof(GLubyte));
}


/* VertexAttribI4usv: marshalled asynchronously */
struct marshal_cmd_VertexAttribI4usv
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLushort v[4];
};
void
_mesa_unmarshal_VertexAttribI4usv(struct gl_context *ctx, const struct marshal_cmd_VertexAttribI4usv *cmd)
{
   const GLuint index = cmd->index;
   const GLushort * v = cmd->v;
   CALL_VertexAttribI4usv(ctx->CurrentServerDispatch, (index, v));
}
void GLAPIENTRY
_mesa_marshal_VertexAttribI4usv(GLuint index, const GLushort * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttribI4usv);
   struct marshal_cmd_VertexAttribI4usv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttribI4usv, cmd_size);
   cmd->index = index;
   memcpy(cmd->v, v, 4 * sizeof(GLushort));
}


/* VertexAttribIPointer: marshalled asynchronously */
struct marshal_cmd_VertexAttribIPointer
{
   struct marshal_cmd_base cmd_base;
   GLenum16 type;
   GLuint index;
   GLint size;
   GLsizei stride;
   const GLvoid * pointer;
};
void
_mesa_unmarshal_VertexAttribIPointer(struct gl_context *ctx, const struct marshal_cmd_VertexAttribIPointer *cmd)
{
   const GLuint index = cmd->index;
   const GLint size = cmd->size;
   const GLenum type = cmd->type;
   const GLsizei stride = cmd->stride;
   const GLvoid * pointer = cmd->pointer;
   CALL_VertexAttribIPointer(ctx->CurrentServerDispatch, (index, size, type, stride, pointer));
}
void GLAPIENTRY
_mesa_marshal_VertexAttribIPointer(GLuint index, GLint size, GLenum type, GLsizei stride, const GLvoid * pointer)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttribIPointer);
   struct marshal_cmd_VertexAttribIPointer *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttribIPointer, cmd_size);
   cmd->index = index;
   cmd->size = size;
   cmd->type = type;
   cmd->stride = stride;
   cmd->pointer = pointer;
   if (COMPAT) _mesa_glthread_AttribPointer(ctx, VERT_ATTRIB_GENERIC(index));
}


/* GetVertexAttribIiv: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetVertexAttribIiv(GLuint index, GLenum pname, GLint * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetVertexAttribIiv");
   CALL_GetVertexAttribIiv(ctx->CurrentServerDispatch, (index, pname, params));
}


/* GetVertexAttribIuiv: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetVertexAttribIuiv(GLuint index, GLenum pname, GLuint * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetVertexAttribIuiv");
   CALL_GetVertexAttribIuiv(ctx->CurrentServerDispatch, (index, pname, params));
}


/* Uniform1ui: marshalled asynchronously */
struct marshal_cmd_Uniform1ui
{
   struct marshal_cmd_base cmd_base;
   GLint location;
   GLuint x;
};
void
_mesa_unmarshal_Uniform1ui(struct gl_context *ctx, const struct marshal_cmd_Uniform1ui *cmd)
{
   const GLint location = cmd->location;
   const GLuint x = cmd->x;
   CALL_Uniform1ui(ctx->CurrentServerDispatch, (location, x));
}
void GLAPIENTRY
_mesa_marshal_Uniform1ui(GLint location, GLuint x)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Uniform1ui);
   struct marshal_cmd_Uniform1ui *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Uniform1ui, cmd_size);
   cmd->location = location;
   cmd->x = x;
}


/* Uniform2ui: marshalled asynchronously */
struct marshal_cmd_Uniform2ui
{
   struct marshal_cmd_base cmd_base;
   GLint location;
   GLuint x;
   GLuint y;
};
void
_mesa_unmarshal_Uniform2ui(struct gl_context *ctx, const struct marshal_cmd_Uniform2ui *cmd)
{
   const GLint location = cmd->location;
   const GLuint x = cmd->x;
   const GLuint y = cmd->y;
   CALL_Uniform2ui(ctx->CurrentServerDispatch, (location, x, y));
}
void GLAPIENTRY
_mesa_marshal_Uniform2ui(GLint location, GLuint x, GLuint y)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Uniform2ui);
   struct marshal_cmd_Uniform2ui *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Uniform2ui, cmd_size);
   cmd->location = location;
   cmd->x = x;
   cmd->y = y;
}


/* Uniform3ui: marshalled asynchronously */
struct marshal_cmd_Uniform3ui
{
   struct marshal_cmd_base cmd_base;
   GLint location;
   GLuint x;
   GLuint y;
   GLuint z;
};
void
_mesa_unmarshal_Uniform3ui(struct gl_context *ctx, const struct marshal_cmd_Uniform3ui *cmd)
{
   const GLint location = cmd->location;
   const GLuint x = cmd->x;
   const GLuint y = cmd->y;
   const GLuint z = cmd->z;
   CALL_Uniform3ui(ctx->CurrentServerDispatch, (location, x, y, z));
}
void GLAPIENTRY
_mesa_marshal_Uniform3ui(GLint location, GLuint x, GLuint y, GLuint z)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Uniform3ui);
   struct marshal_cmd_Uniform3ui *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Uniform3ui, cmd_size);
   cmd->location = location;
   cmd->x = x;
   cmd->y = y;
   cmd->z = z;
}


/* Uniform4ui: marshalled asynchronously */
struct marshal_cmd_Uniform4ui
{
   struct marshal_cmd_base cmd_base;
   GLint location;
   GLuint x;
   GLuint y;
   GLuint z;
   GLuint w;
};
void
_mesa_unmarshal_Uniform4ui(struct gl_context *ctx, const struct marshal_cmd_Uniform4ui *cmd)
{
   const GLint location = cmd->location;
   const GLuint x = cmd->x;
   const GLuint y = cmd->y;
   const GLuint z = cmd->z;
   const GLuint w = cmd->w;
   CALL_Uniform4ui(ctx->CurrentServerDispatch, (location, x, y, z, w));
}
void GLAPIENTRY
_mesa_marshal_Uniform4ui(GLint location, GLuint x, GLuint y, GLuint z, GLuint w)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Uniform4ui);
   struct marshal_cmd_Uniform4ui *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Uniform4ui, cmd_size);
   cmd->location = location;
   cmd->x = x;
   cmd->y = y;
   cmd->z = z;
   cmd->w = w;
}


/* Uniform1uiv: marshalled asynchronously */
struct marshal_cmd_Uniform1uiv
{
   struct marshal_cmd_base cmd_base;
   GLint location;
   GLsizei count;
   /* Next safe_mul(count, 1 * sizeof(GLuint)) bytes are GLuint value[count] */
};
void
_mesa_unmarshal_Uniform1uiv(struct gl_context *ctx, const struct marshal_cmd_Uniform1uiv *cmd)
{
   const GLint location = cmd->location;
   const GLsizei count = cmd->count;
   GLuint * value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLuint *) variable_data;
   CALL_Uniform1uiv(ctx->CurrentServerDispatch, (location, count, value));
}
void GLAPIENTRY
_mesa_marshal_Uniform1uiv(GLint location, GLsizei count, const GLuint * value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 1 * sizeof(GLuint));
   int cmd_size = sizeof(struct marshal_cmd_Uniform1uiv) + value_size;
   struct marshal_cmd_Uniform1uiv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "Uniform1uiv");
      CALL_Uniform1uiv(ctx->CurrentServerDispatch, (location, count, value));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Uniform1uiv, cmd_size);
   cmd->location = location;
   cmd->count = count;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, value, value_size);
}


/* Uniform2uiv: marshalled asynchronously */
struct marshal_cmd_Uniform2uiv
{
   struct marshal_cmd_base cmd_base;
   GLint location;
   GLsizei count;
   /* Next safe_mul(count, 2 * sizeof(GLuint)) bytes are GLuint value[count][2] */
};
void
_mesa_unmarshal_Uniform2uiv(struct gl_context *ctx, const struct marshal_cmd_Uniform2uiv *cmd)
{
   const GLint location = cmd->location;
   const GLsizei count = cmd->count;
   GLuint * value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLuint *) variable_data;
   CALL_Uniform2uiv(ctx->CurrentServerDispatch, (location, count, value));
}
void GLAPIENTRY
_mesa_marshal_Uniform2uiv(GLint location, GLsizei count, const GLuint * value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 2 * sizeof(GLuint));
   int cmd_size = sizeof(struct marshal_cmd_Uniform2uiv) + value_size;
   struct marshal_cmd_Uniform2uiv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "Uniform2uiv");
      CALL_Uniform2uiv(ctx->CurrentServerDispatch, (location, count, value));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Uniform2uiv, cmd_size);
   cmd->location = location;
   cmd->count = count;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, value, value_size);
}


/* Uniform3uiv: marshalled asynchronously */
struct marshal_cmd_Uniform3uiv
{
   struct marshal_cmd_base cmd_base;
   GLint location;
   GLsizei count;
   /* Next safe_mul(count, 3 * sizeof(GLuint)) bytes are GLuint value[count][3] */
};
void
_mesa_unmarshal_Uniform3uiv(struct gl_context *ctx, const struct marshal_cmd_Uniform3uiv *cmd)
{
   const GLint location = cmd->location;
   const GLsizei count = cmd->count;
   GLuint * value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLuint *) variable_data;
   CALL_Uniform3uiv(ctx->CurrentServerDispatch, (location, count, value));
}
void GLAPIENTRY
_mesa_marshal_Uniform3uiv(GLint location, GLsizei count, const GLuint * value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 3 * sizeof(GLuint));
   int cmd_size = sizeof(struct marshal_cmd_Uniform3uiv) + value_size;
   struct marshal_cmd_Uniform3uiv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "Uniform3uiv");
      CALL_Uniform3uiv(ctx->CurrentServerDispatch, (location, count, value));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Uniform3uiv, cmd_size);
   cmd->location = location;
   cmd->count = count;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, value, value_size);
}


/* Uniform4uiv: marshalled asynchronously */
struct marshal_cmd_Uniform4uiv
{
   struct marshal_cmd_base cmd_base;
   GLint location;
   GLsizei count;
   /* Next safe_mul(count, 4 * sizeof(GLuint)) bytes are GLuint value[count][4] */
};
void
_mesa_unmarshal_Uniform4uiv(struct gl_context *ctx, const struct marshal_cmd_Uniform4uiv *cmd)
{
   const GLint location = cmd->location;
   const GLsizei count = cmd->count;
   GLuint * value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLuint *) variable_data;
   CALL_Uniform4uiv(ctx->CurrentServerDispatch, (location, count, value));
}
void GLAPIENTRY
_mesa_marshal_Uniform4uiv(GLint location, GLsizei count, const GLuint * value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 4 * sizeof(GLuint));
   int cmd_size = sizeof(struct marshal_cmd_Uniform4uiv) + value_size;
   struct marshal_cmd_Uniform4uiv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "Uniform4uiv");
      CALL_Uniform4uiv(ctx->CurrentServerDispatch, (location, count, value));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Uniform4uiv, cmd_size);
   cmd->location = location;
   cmd->count = count;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, value, value_size);
}


/* GetUniformuiv: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetUniformuiv(GLuint program, GLint location, GLuint * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetUniformuiv");
   CALL_GetUniformuiv(ctx->CurrentServerDispatch, (program, location, params));
}


/* BindFragDataLocation: marshalled asynchronously */
struct marshal_cmd_BindFragDataLocation
{
   struct marshal_cmd_base cmd_base;
   GLuint program;
   GLuint colorNumber;
   /* Next (strlen(name) + 1) bytes are GLchar name[(strlen(name) + 1)] */
};
void
_mesa_unmarshal_BindFragDataLocation(struct gl_context *ctx, const struct marshal_cmd_BindFragDataLocation *cmd)
{
   const GLuint program = cmd->program;
   const GLuint colorNumber = cmd->colorNumber;
   GLchar * name;
   const char *variable_data = (const char *) (cmd + 1);
   name = (GLchar *) variable_data;
   CALL_BindFragDataLocation(ctx->CurrentServerDispatch, (program, colorNumber, name));
}
void GLAPIENTRY
_mesa_marshal_BindFragDataLocation(GLuint program, GLuint colorNumber, const GLchar * name)
{
   GET_CURRENT_CONTEXT(ctx);
   int name_size = (strlen(name) + 1);
   int cmd_size = sizeof(struct marshal_cmd_BindFragDataLocation) + name_size;
   struct marshal_cmd_BindFragDataLocation *cmd;
   if (unlikely(name_size < 0 || (name_size > 0 && !name) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "BindFragDataLocation");
      CALL_BindFragDataLocation(ctx->CurrentServerDispatch, (program, colorNumber, name));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_BindFragDataLocation, cmd_size);
   cmd->program = program;
   cmd->colorNumber = colorNumber;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, name, name_size);
}


/* GetFragDataLocation: marshalled synchronously */
GLint GLAPIENTRY
_mesa_marshal_GetFragDataLocation(GLuint program, const GLchar * name)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetFragDataLocation");
   return CALL_GetFragDataLocation(ctx->CurrentServerDispatch, (program, name));
}


/* ClearBufferiv: marshalled asynchronously */
struct marshal_cmd_ClearBufferiv
{
   struct marshal_cmd_base cmd_base;
   GLenum16 buffer;
   GLint drawbuffer;
   /* Next safe_mul(_mesa_buffer_enum_to_count(buffer), 1 * sizeof(GLint)) bytes are GLint value[_mesa_buffer_enum_to_count(buffer)] */
};
void
_mesa_unmarshal_ClearBufferiv(struct gl_context *ctx, const struct marshal_cmd_ClearBufferiv *cmd)
{
   const GLenum buffer = cmd->buffer;
   const GLint drawbuffer = cmd->drawbuffer;
   GLint * value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLint *) variable_data;
   CALL_ClearBufferiv(ctx->CurrentServerDispatch, (buffer, drawbuffer, value));
}
void GLAPIENTRY
_mesa_marshal_ClearBufferiv(GLenum buffer, GLint drawbuffer, const GLint * value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(_mesa_buffer_enum_to_count(buffer), 1 * sizeof(GLint));
   int cmd_size = sizeof(struct marshal_cmd_ClearBufferiv) + value_size;
   struct marshal_cmd_ClearBufferiv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "ClearBufferiv");
      CALL_ClearBufferiv(ctx->CurrentServerDispatch, (buffer, drawbuffer, value));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ClearBufferiv, cmd_size);
   cmd->buffer = buffer;
   cmd->drawbuffer = drawbuffer;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, value, value_size);
}


/* ClearBufferuiv: marshalled asynchronously */
struct marshal_cmd_ClearBufferuiv
{
   struct marshal_cmd_base cmd_base;
   GLenum16 buffer;
   GLint drawbuffer;
   /* Next safe_mul(_mesa_buffer_enum_to_count(buffer), 1 * sizeof(GLuint)) bytes are GLuint value[_mesa_buffer_enum_to_count(buffer)] */
};
void
_mesa_unmarshal_ClearBufferuiv(struct gl_context *ctx, const struct marshal_cmd_ClearBufferuiv *cmd)
{
   const GLenum buffer = cmd->buffer;
   const GLint drawbuffer = cmd->drawbuffer;
   GLuint * value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLuint *) variable_data;
   CALL_ClearBufferuiv(ctx->CurrentServerDispatch, (buffer, drawbuffer, value));
}
void GLAPIENTRY
_mesa_marshal_ClearBufferuiv(GLenum buffer, GLint drawbuffer, const GLuint * value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(_mesa_buffer_enum_to_count(buffer), 1 * sizeof(GLuint));
   int cmd_size = sizeof(struct marshal_cmd_ClearBufferuiv) + value_size;
   struct marshal_cmd_ClearBufferuiv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "ClearBufferuiv");
      CALL_ClearBufferuiv(ctx->CurrentServerDispatch, (buffer, drawbuffer, value));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ClearBufferuiv, cmd_size);
   cmd->buffer = buffer;
   cmd->drawbuffer = drawbuffer;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, value, value_size);
}


/* ClearBufferfv: marshalled asynchronously */
struct marshal_cmd_ClearBufferfv
{
   struct marshal_cmd_base cmd_base;
   GLenum16 buffer;
   GLint drawbuffer;
   /* Next safe_mul(_mesa_buffer_enum_to_count(buffer), 1 * sizeof(GLfloat)) bytes are GLfloat value[_mesa_buffer_enum_to_count(buffer)] */
};
void
_mesa_unmarshal_ClearBufferfv(struct gl_context *ctx, const struct marshal_cmd_ClearBufferfv *cmd)
{
   const GLenum buffer = cmd->buffer;
   const GLint drawbuffer = cmd->drawbuffer;
   GLfloat * value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLfloat *) variable_data;
   CALL_ClearBufferfv(ctx->CurrentServerDispatch, (buffer, drawbuffer, value));
}
void GLAPIENTRY
_mesa_marshal_ClearBufferfv(GLenum buffer, GLint drawbuffer, const GLfloat * value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(_mesa_buffer_enum_to_count(buffer), 1 * sizeof(GLfloat));
   int cmd_size = sizeof(struct marshal_cmd_ClearBufferfv) + value_size;
   struct marshal_cmd_ClearBufferfv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "ClearBufferfv");
      CALL_ClearBufferfv(ctx->CurrentServerDispatch, (buffer, drawbuffer, value));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ClearBufferfv, cmd_size);
   cmd->buffer = buffer;
   cmd->drawbuffer = drawbuffer;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, value, value_size);
}


/* ClearBufferfi: marshalled asynchronously */
struct marshal_cmd_ClearBufferfi
{
   struct marshal_cmd_base cmd_base;
   GLenum16 buffer;
   GLint drawbuffer;
   GLfloat depth;
   GLint stencil;
};
void
_mesa_unmarshal_ClearBufferfi(struct gl_context *ctx, const struct marshal_cmd_ClearBufferfi *cmd)
{
   const GLenum buffer = cmd->buffer;
   const GLint drawbuffer = cmd->drawbuffer;
   const GLfloat depth = cmd->depth;
   const GLint stencil = cmd->stencil;
   CALL_ClearBufferfi(ctx->CurrentServerDispatch, (buffer, drawbuffer, depth, stencil));
}
void GLAPIENTRY
_mesa_marshal_ClearBufferfi(GLenum buffer, GLint drawbuffer, GLfloat depth, GLint stencil)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ClearBufferfi);
   struct marshal_cmd_ClearBufferfi *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ClearBufferfi, cmd_size);
   cmd->buffer = buffer;
   cmd->drawbuffer = drawbuffer;
   cmd->depth = depth;
   cmd->stencil = stencil;
}


/* GetStringi: marshalled synchronously */
const GLubyte * GLAPIENTRY
_mesa_marshal_GetStringi(GLenum name, GLuint index)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetStringi");
   return CALL_GetStringi(ctx->CurrentServerDispatch, (name, index));
}


/* BeginTransformFeedback: marshalled asynchronously */
struct marshal_cmd_BeginTransformFeedback
{
   struct marshal_cmd_base cmd_base;
   GLenum16 mode;
};
void
_mesa_unmarshal_BeginTransformFeedback(struct gl_context *ctx, const struct marshal_cmd_BeginTransformFeedback *cmd)
{
   const GLenum mode = cmd->mode;
   CALL_BeginTransformFeedback(ctx->CurrentServerDispatch, (mode));
}
void GLAPIENTRY
_mesa_marshal_BeginTransformFeedback(GLenum mode)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_BeginTransformFeedback);
   struct marshal_cmd_BeginTransformFeedback *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_BeginTransformFeedback, cmd_size);
   cmd->mode = mode;
}


/* EndTransformFeedback: marshalled asynchronously */
struct marshal_cmd_EndTransformFeedback
{
   struct marshal_cmd_base cmd_base;
};
void
_mesa_unmarshal_EndTransformFeedback(struct gl_context *ctx, const struct marshal_cmd_EndTransformFeedback *cmd)
{
   CALL_EndTransformFeedback(ctx->CurrentServerDispatch, ());
}
void GLAPIENTRY
_mesa_marshal_EndTransformFeedback(void)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_EndTransformFeedback);
   struct marshal_cmd_EndTransformFeedback *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_EndTransformFeedback, cmd_size);
   (void) cmd;
}


/* BindBufferRange: marshalled asynchronously */
struct marshal_cmd_BindBufferRange
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLuint index;
   GLuint buffer;
   GLintptr offset;
   GLsizeiptr size;
};
void
_mesa_unmarshal_BindBufferRange(struct gl_context *ctx, const struct marshal_cmd_BindBufferRange *cmd)
{
   const GLenum target = cmd->target;
   const GLuint index = cmd->index;
   const GLuint buffer = cmd->buffer;
   const GLintptr offset = cmd->offset;
   const GLsizeiptr size = cmd->size;
   CALL_BindBufferRange(ctx->CurrentServerDispatch, (target, index, buffer, offset, size));
}
void GLAPIENTRY
_mesa_marshal_BindBufferRange(GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_BindBufferRange);
   struct marshal_cmd_BindBufferRange *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_BindBufferRange, cmd_size);
   cmd->target = target;
   cmd->index = index;
   cmd->buffer = buffer;
   cmd->offset = offset;
   cmd->size = size;
}


/* BindBufferBase: marshalled asynchronously */
struct marshal_cmd_BindBufferBase
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLuint index;
   GLuint buffer;
};
void
_mesa_unmarshal_BindBufferBase(struct gl_context *ctx, const struct marshal_cmd_BindBufferBase *cmd)
{
   const GLenum target = cmd->target;
   const GLuint index = cmd->index;
   const GLuint buffer = cmd->buffer;
   CALL_BindBufferBase(ctx->CurrentServerDispatch, (target, index, buffer));
}
void GLAPIENTRY
_mesa_marshal_BindBufferBase(GLenum target, GLuint index, GLuint buffer)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_BindBufferBase);
   struct marshal_cmd_BindBufferBase *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_BindBufferBase, cmd_size);
   cmd->target = target;
   cmd->index = index;
   cmd->buffer = buffer;
}


/* TransformFeedbackVaryings: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_TransformFeedbackVaryings(GLuint program, GLsizei count, const GLchar * const * varyings, GLenum bufferMode)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "TransformFeedbackVaryings");
   CALL_TransformFeedbackVaryings(ctx->CurrentServerDispatch, (program, count, varyings, bufferMode));
}


/* GetTransformFeedbackVarying: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetTransformFeedbackVarying(GLuint program, GLuint index, GLsizei bufSize, GLsizei * length, GLsizei * size, GLenum * type, GLchar * name)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetTransformFeedbackVarying");
   CALL_GetTransformFeedbackVarying(ctx->CurrentServerDispatch, (program, index, bufSize, length, size, type, name));
}


/* BeginConditionalRender: marshalled asynchronously */
struct marshal_cmd_BeginConditionalRender
{
   struct marshal_cmd_base cmd_base;
   GLenum16 mode;
   GLuint query;
};
void
_mesa_unmarshal_BeginConditionalRender(struct gl_context *ctx, const struct marshal_cmd_BeginConditionalRender *cmd)
{
   const GLuint query = cmd->query;
   const GLenum mode = cmd->mode;
   CALL_BeginConditionalRender(ctx->CurrentServerDispatch, (query, mode));
}
void GLAPIENTRY
_mesa_marshal_BeginConditionalRender(GLuint query, GLenum mode)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_BeginConditionalRender);
   struct marshal_cmd_BeginConditionalRender *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_BeginConditionalRender, cmd_size);
   cmd->query = query;
   cmd->mode = mode;
}


/* EndConditionalRender: marshalled asynchronously */
struct marshal_cmd_EndConditionalRender
{
   struct marshal_cmd_base cmd_base;
};
void
_mesa_unmarshal_EndConditionalRender(struct gl_context *ctx, const struct marshal_cmd_EndConditionalRender *cmd)
{
   CALL_EndConditionalRender(ctx->CurrentServerDispatch, ());
}
void GLAPIENTRY
_mesa_marshal_EndConditionalRender(void)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_EndConditionalRender);
   struct marshal_cmd_EndConditionalRender *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_EndConditionalRender, cmd_size);
   (void) cmd;
}


/* PrimitiveRestartIndex: marshalled asynchronously */
struct marshal_cmd_PrimitiveRestartIndex
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
};
void
_mesa_unmarshal_PrimitiveRestartIndex(struct gl_context *ctx, const struct marshal_cmd_PrimitiveRestartIndex *cmd)
{
   const GLuint index = cmd->index;
   CALL_PrimitiveRestartIndex(ctx->CurrentServerDispatch, (index));
}
void GLAPIENTRY
_mesa_marshal_PrimitiveRestartIndex(GLuint index)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_PrimitiveRestartIndex);
   struct marshal_cmd_PrimitiveRestartIndex *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_PrimitiveRestartIndex, cmd_size);
   cmd->index = index;
}


/* GetInteger64i_v: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetInteger64i_v(GLenum cap, GLuint index, GLint64 * data)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetInteger64i_v");
   CALL_GetInteger64i_v(ctx->CurrentServerDispatch, (cap, index, data));
}


/* GetBufferParameteri64v: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetBufferParameteri64v(GLenum target, GLenum pname, GLint64 * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetBufferParameteri64v");
   CALL_GetBufferParameteri64v(ctx->CurrentServerDispatch, (target, pname, params));
}


/* FramebufferTexture: marshalled asynchronously */
struct marshal_cmd_FramebufferTexture
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLenum16 attachment;
   GLuint texture;
   GLint level;
};
void
_mesa_unmarshal_FramebufferTexture(struct gl_context *ctx, const struct marshal_cmd_FramebufferTexture *cmd)
{
   const GLenum target = cmd->target;
   const GLenum attachment = cmd->attachment;
   const GLuint texture = cmd->texture;
   const GLint level = cmd->level;
   CALL_FramebufferTexture(ctx->CurrentServerDispatch, (target, attachment, texture, level));
}
void GLAPIENTRY
_mesa_marshal_FramebufferTexture(GLenum target, GLenum attachment, GLuint texture, GLint level)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_FramebufferTexture);
   struct marshal_cmd_FramebufferTexture *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_FramebufferTexture, cmd_size);
   cmd->target = target;
   cmd->attachment = attachment;
   cmd->texture = texture;
   cmd->level = level;
}


/* PrimitiveRestartNV: marshalled asynchronously */
struct marshal_cmd_PrimitiveRestartNV
{
   struct marshal_cmd_base cmd_base;
};
void
_mesa_unmarshal_PrimitiveRestartNV(struct gl_context *ctx, const struct marshal_cmd_PrimitiveRestartNV *cmd)
{
   CALL_PrimitiveRestartNV(ctx->CurrentServerDispatch, ());
}
void GLAPIENTRY
_mesa_marshal_PrimitiveRestartNV(void)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_PrimitiveRestartNV);
   struct marshal_cmd_PrimitiveRestartNV *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_PrimitiveRestartNV, cmd_size);
   (void) cmd;
}


/* BindBufferOffsetEXT: marshalled asynchronously */
struct marshal_cmd_BindBufferOffsetEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLuint index;
   GLuint buffer;
   GLintptr offset;
};
void
_mesa_unmarshal_BindBufferOffsetEXT(struct gl_context *ctx, const struct marshal_cmd_BindBufferOffsetEXT *cmd)
{
   const GLenum target = cmd->target;
   const GLuint index = cmd->index;
   const GLuint buffer = cmd->buffer;
   const GLintptr offset = cmd->offset;
   CALL_BindBufferOffsetEXT(ctx->CurrentServerDispatch, (target, index, buffer, offset));
}
void GLAPIENTRY
_mesa_marshal_BindBufferOffsetEXT(GLenum target, GLuint index, GLuint buffer, GLintptr offset)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_BindBufferOffsetEXT);
   struct marshal_cmd_BindBufferOffsetEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_BindBufferOffsetEXT, cmd_size);
   cmd->target = target;
   cmd->index = index;
   cmd->buffer = buffer;
   cmd->offset = offset;
}


/* BindTransformFeedback: marshalled asynchronously */
struct marshal_cmd_BindTransformFeedback
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLuint id;
};
void
_mesa_unmarshal_BindTransformFeedback(struct gl_context *ctx, const struct marshal_cmd_BindTransformFeedback *cmd)
{
   const GLenum target = cmd->target;
   const GLuint id = cmd->id;
   CALL_BindTransformFeedback(ctx->CurrentServerDispatch, (target, id));
}
void GLAPIENTRY
_mesa_marshal_BindTransformFeedback(GLenum target, GLuint id)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_BindTransformFeedback);
   struct marshal_cmd_BindTransformFeedback *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_BindTransformFeedback, cmd_size);
   cmd->target = target;
   cmd->id = id;
}


/* DeleteTransformFeedbacks: marshalled asynchronously */
struct marshal_cmd_DeleteTransformFeedbacks
{
   struct marshal_cmd_base cmd_base;
   GLsizei n;
   /* Next safe_mul(n, 1 * sizeof(GLuint)) bytes are GLuint ids[n] */
};
void
_mesa_unmarshal_DeleteTransformFeedbacks(struct gl_context *ctx, const struct marshal_cmd_DeleteTransformFeedbacks *cmd)
{
   const GLsizei n = cmd->n;
   GLuint * ids;
   const char *variable_data = (const char *) (cmd + 1);
   ids = (GLuint *) variable_data;
   CALL_DeleteTransformFeedbacks(ctx->CurrentServerDispatch, (n, ids));
}
void GLAPIENTRY
_mesa_marshal_DeleteTransformFeedbacks(GLsizei n, const GLuint * ids)
{
   GET_CURRENT_CONTEXT(ctx);
   int ids_size = safe_mul(n, 1 * sizeof(GLuint));
   int cmd_size = sizeof(struct marshal_cmd_DeleteTransformFeedbacks) + ids_size;
   struct marshal_cmd_DeleteTransformFeedbacks *cmd;
   if (unlikely(ids_size < 0 || (ids_size > 0 && !ids) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "DeleteTransformFeedbacks");
      CALL_DeleteTransformFeedbacks(ctx->CurrentServerDispatch, (n, ids));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_DeleteTransformFeedbacks, cmd_size);
   cmd->n = n;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, ids, ids_size);
}


/* GenTransformFeedbacks: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GenTransformFeedbacks(GLsizei n, GLuint * ids)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GenTransformFeedbacks");
   CALL_GenTransformFeedbacks(ctx->CurrentServerDispatch, (n, ids));
}


/* IsTransformFeedback: marshalled synchronously */
GLboolean GLAPIENTRY
_mesa_marshal_IsTransformFeedback(GLuint id)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "IsTransformFeedback");
   return CALL_IsTransformFeedback(ctx->CurrentServerDispatch, (id));
}


/* PauseTransformFeedback: marshalled asynchronously */
struct marshal_cmd_PauseTransformFeedback
{
   struct marshal_cmd_base cmd_base;
};
void
_mesa_unmarshal_PauseTransformFeedback(struct gl_context *ctx, const struct marshal_cmd_PauseTransformFeedback *cmd)
{
   CALL_PauseTransformFeedback(ctx->CurrentServerDispatch, ());
}
void GLAPIENTRY
_mesa_marshal_PauseTransformFeedback(void)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_PauseTransformFeedback);
   struct marshal_cmd_PauseTransformFeedback *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_PauseTransformFeedback, cmd_size);
   (void) cmd;
}


/* ResumeTransformFeedback: marshalled asynchronously */
struct marshal_cmd_ResumeTransformFeedback
{
   struct marshal_cmd_base cmd_base;
};
void
_mesa_unmarshal_ResumeTransformFeedback(struct gl_context *ctx, const struct marshal_cmd_ResumeTransformFeedback *cmd)
{
   CALL_ResumeTransformFeedback(ctx->CurrentServerDispatch, ());
}
void GLAPIENTRY
_mesa_marshal_ResumeTransformFeedback(void)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ResumeTransformFeedback);
   struct marshal_cmd_ResumeTransformFeedback *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ResumeTransformFeedback, cmd_size);
   (void) cmd;
}


/* DrawTransformFeedback: marshalled asynchronously */
struct marshal_cmd_DrawTransformFeedback
{
   struct marshal_cmd_base cmd_base;
   GLenum16 mode;
   GLuint id;
};
void
_mesa_unmarshal_DrawTransformFeedback(struct gl_context *ctx, const struct marshal_cmd_DrawTransformFeedback *cmd)
{
   const GLenum mode = cmd->mode;
   const GLuint id = cmd->id;
   CALL_DrawTransformFeedback(ctx->CurrentServerDispatch, (mode, id));
}
void GLAPIENTRY
_mesa_marshal_DrawTransformFeedback(GLenum mode, GLuint id)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_DrawTransformFeedback);
   struct marshal_cmd_DrawTransformFeedback *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_DrawTransformFeedback, cmd_size);
   cmd->mode = mode;
   cmd->id = id;
}


/* VDPAUInitNV: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_VDPAUInitNV(const GLvoid * vdpDevice, const GLvoid * getProcAddress)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "VDPAUInitNV");
   CALL_VDPAUInitNV(ctx->CurrentServerDispatch, (vdpDevice, getProcAddress));
}


/* VDPAUFiniNV: marshalled asynchronously */
struct marshal_cmd_VDPAUFiniNV
{
   struct marshal_cmd_base cmd_base;
};
void
_mesa_unmarshal_VDPAUFiniNV(struct gl_context *ctx, const struct marshal_cmd_VDPAUFiniNV *cmd)
{
   CALL_VDPAUFiniNV(ctx->CurrentServerDispatch, ());
}
void GLAPIENTRY
_mesa_marshal_VDPAUFiniNV(void)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VDPAUFiniNV);
   struct marshal_cmd_VDPAUFiniNV *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VDPAUFiniNV, cmd_size);
   (void) cmd;
}


/* VDPAURegisterVideoSurfaceNV: marshalled synchronously */
GLintptr GLAPIENTRY
_mesa_marshal_VDPAURegisterVideoSurfaceNV(const GLvoid * vdpSurface, GLenum target, GLsizei numTextureNames, const GLuint * textureNames)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "VDPAURegisterVideoSurfaceNV");
   return CALL_VDPAURegisterVideoSurfaceNV(ctx->CurrentServerDispatch, (vdpSurface, target, numTextureNames, textureNames));
}


/* VDPAURegisterOutputSurfaceNV: marshalled synchronously */
GLintptr GLAPIENTRY
_mesa_marshal_VDPAURegisterOutputSurfaceNV(const GLvoid * vdpSurface, GLenum target, GLsizei numTextureNames, const GLuint * textureNames)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "VDPAURegisterOutputSurfaceNV");
   return CALL_VDPAURegisterOutputSurfaceNV(ctx->CurrentServerDispatch, (vdpSurface, target, numTextureNames, textureNames));
}


/* VDPAUIsSurfaceNV: marshalled synchronously */
GLboolean GLAPIENTRY
_mesa_marshal_VDPAUIsSurfaceNV(GLintptr surface)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "VDPAUIsSurfaceNV");
   return CALL_VDPAUIsSurfaceNV(ctx->CurrentServerDispatch, (surface));
}


/* VDPAUUnregisterSurfaceNV: marshalled asynchronously */
struct marshal_cmd_VDPAUUnregisterSurfaceNV
{
   struct marshal_cmd_base cmd_base;
   GLintptr surface;
};
void
_mesa_unmarshal_VDPAUUnregisterSurfaceNV(struct gl_context *ctx, const struct marshal_cmd_VDPAUUnregisterSurfaceNV *cmd)
{
   const GLintptr surface = cmd->surface;
   CALL_VDPAUUnregisterSurfaceNV(ctx->CurrentServerDispatch, (surface));
}
void GLAPIENTRY
_mesa_marshal_VDPAUUnregisterSurfaceNV(GLintptr surface)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VDPAUUnregisterSurfaceNV);
   struct marshal_cmd_VDPAUUnregisterSurfaceNV *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VDPAUUnregisterSurfaceNV, cmd_size);
   cmd->surface = surface;
}


/* VDPAUGetSurfaceivNV: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_VDPAUGetSurfaceivNV(GLintptr surface, GLenum pname, GLsizei bufSize, GLsizei * length, GLint * values)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "VDPAUGetSurfaceivNV");
   CALL_VDPAUGetSurfaceivNV(ctx->CurrentServerDispatch, (surface, pname, bufSize, length, values));
}


/* VDPAUSurfaceAccessNV: marshalled asynchronously */
struct marshal_cmd_VDPAUSurfaceAccessNV
{
   struct marshal_cmd_base cmd_base;
   GLenum16 access;
   GLintptr surface;
};
void
_mesa_unmarshal_VDPAUSurfaceAccessNV(struct gl_context *ctx, const struct marshal_cmd_VDPAUSurfaceAccessNV *cmd)
{
   const GLintptr surface = cmd->surface;
   const GLenum access = cmd->access;
   CALL_VDPAUSurfaceAccessNV(ctx->CurrentServerDispatch, (surface, access));
}
void GLAPIENTRY
_mesa_marshal_VDPAUSurfaceAccessNV(GLintptr surface, GLenum access)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VDPAUSurfaceAccessNV);
   struct marshal_cmd_VDPAUSurfaceAccessNV *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VDPAUSurfaceAccessNV, cmd_size);
   cmd->surface = surface;
   cmd->access = access;
}


/* VDPAUMapSurfacesNV: marshalled asynchronously */
struct marshal_cmd_VDPAUMapSurfacesNV
{
   struct marshal_cmd_base cmd_base;
   GLsizei numSurfaces;
   /* Next safe_mul(numSurfaces, 1 * sizeof(GLintptr)) bytes are GLintptr surfaces[numSurfaces] */
};
void
_mesa_unmarshal_VDPAUMapSurfacesNV(struct gl_context *ctx, const struct marshal_cmd_VDPAUMapSurfacesNV *cmd)
{
   const GLsizei numSurfaces = cmd->numSurfaces;
   GLintptr * surfaces;
   const char *variable_data = (const char *) (cmd + 1);
   surfaces = (GLintptr *) variable_data;
   CALL_VDPAUMapSurfacesNV(ctx->CurrentServerDispatch, (numSurfaces, surfaces));
}
void GLAPIENTRY
_mesa_marshal_VDPAUMapSurfacesNV(GLsizei numSurfaces, const GLintptr * surfaces)
{
   GET_CURRENT_CONTEXT(ctx);
   int surfaces_size = safe_mul(numSurfaces, 1 * sizeof(GLintptr));
   int cmd_size = sizeof(struct marshal_cmd_VDPAUMapSurfacesNV) + surfaces_size;
   struct marshal_cmd_VDPAUMapSurfacesNV *cmd;
   if (unlikely(surfaces_size < 0 || (surfaces_size > 0 && !surfaces) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "VDPAUMapSurfacesNV");
      CALL_VDPAUMapSurfacesNV(ctx->CurrentServerDispatch, (numSurfaces, surfaces));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VDPAUMapSurfacesNV, cmd_size);
   cmd->numSurfaces = numSurfaces;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, surfaces, surfaces_size);
}


/* VDPAUUnmapSurfacesNV: marshalled asynchronously */
struct marshal_cmd_VDPAUUnmapSurfacesNV
{
   struct marshal_cmd_base cmd_base;
   GLsizei numSurfaces;
   /* Next safe_mul(numSurfaces, 1 * sizeof(GLintptr)) bytes are GLintptr surfaces[numSurfaces] */
};
void
_mesa_unmarshal_VDPAUUnmapSurfacesNV(struct gl_context *ctx, const struct marshal_cmd_VDPAUUnmapSurfacesNV *cmd)
{
   const GLsizei numSurfaces = cmd->numSurfaces;
   GLintptr * surfaces;
   const char *variable_data = (const char *) (cmd + 1);
   surfaces = (GLintptr *) variable_data;
   CALL_VDPAUUnmapSurfacesNV(ctx->CurrentServerDispatch, (numSurfaces, surfaces));
}
void GLAPIENTRY
_mesa_marshal_VDPAUUnmapSurfacesNV(GLsizei numSurfaces, const GLintptr * surfaces)
{
   GET_CURRENT_CONTEXT(ctx);
   int surfaces_size = safe_mul(numSurfaces, 1 * sizeof(GLintptr));
   int cmd_size = sizeof(struct marshal_cmd_VDPAUUnmapSurfacesNV) + surfaces_size;
   struct marshal_cmd_VDPAUUnmapSurfacesNV *cmd;
   if (unlikely(surfaces_size < 0 || (surfaces_size > 0 && !surfaces) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "VDPAUUnmapSurfacesNV");
      CALL_VDPAUUnmapSurfacesNV(ctx->CurrentServerDispatch, (numSurfaces, surfaces));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VDPAUUnmapSurfacesNV, cmd_size);
   cmd->numSurfaces = numSurfaces;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, surfaces, surfaces_size);
}


/* GetUnsignedBytevEXT: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetUnsignedBytevEXT(GLenum pname, GLubyte * data)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetUnsignedBytevEXT");
   CALL_GetUnsignedBytevEXT(ctx->CurrentServerDispatch, (pname, data));
}


/* GetUnsignedBytei_vEXT: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetUnsignedBytei_vEXT(GLenum target, GLuint index, GLubyte * data)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetUnsignedBytei_vEXT");
   CALL_GetUnsignedBytei_vEXT(ctx->CurrentServerDispatch, (target, index, data));
}


/* DeleteMemoryObjectsEXT: marshalled asynchronously */
struct marshal_cmd_DeleteMemoryObjectsEXT
{
   struct marshal_cmd_base cmd_base;
   GLsizei n;
   /* Next safe_mul(n, 1 * sizeof(GLuint)) bytes are GLuint memoryObjects[n] */
};
void
_mesa_unmarshal_DeleteMemoryObjectsEXT(struct gl_context *ctx, const struct marshal_cmd_DeleteMemoryObjectsEXT *cmd)
{
   const GLsizei n = cmd->n;
   GLuint * memoryObjects;
   const char *variable_data = (const char *) (cmd + 1);
   memoryObjects = (GLuint *) variable_data;
   CALL_DeleteMemoryObjectsEXT(ctx->CurrentServerDispatch, (n, memoryObjects));
}
void GLAPIENTRY
_mesa_marshal_DeleteMemoryObjectsEXT(GLsizei n, const GLuint * memoryObjects)
{
   GET_CURRENT_CONTEXT(ctx);
   int memoryObjects_size = safe_mul(n, 1 * sizeof(GLuint));
   int cmd_size = sizeof(struct marshal_cmd_DeleteMemoryObjectsEXT) + memoryObjects_size;
   struct marshal_cmd_DeleteMemoryObjectsEXT *cmd;
   if (unlikely(memoryObjects_size < 0 || (memoryObjects_size > 0 && !memoryObjects) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "DeleteMemoryObjectsEXT");
      CALL_DeleteMemoryObjectsEXT(ctx->CurrentServerDispatch, (n, memoryObjects));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_DeleteMemoryObjectsEXT, cmd_size);
   cmd->n = n;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, memoryObjects, memoryObjects_size);
}


/* IsMemoryObjectEXT: marshalled synchronously */
GLboolean GLAPIENTRY
_mesa_marshal_IsMemoryObjectEXT(GLuint memoryObject)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "IsMemoryObjectEXT");
   return CALL_IsMemoryObjectEXT(ctx->CurrentServerDispatch, (memoryObject));
}


/* CreateMemoryObjectsEXT: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_CreateMemoryObjectsEXT(GLsizei n, GLuint * memoryObjects)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "CreateMemoryObjectsEXT");
   CALL_CreateMemoryObjectsEXT(ctx->CurrentServerDispatch, (n, memoryObjects));
}


/* MemoryObjectParameterivEXT: marshalled asynchronously */
struct marshal_cmd_MemoryObjectParameterivEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 pname;
   GLuint memoryObject;
   /* Next safe_mul(_mesa_memobj_enum_to_count(pname), 1 * sizeof(GLint)) bytes are GLint params[_mesa_memobj_enum_to_count(pname)] */
};
void
_mesa_unmarshal_MemoryObjectParameterivEXT(struct gl_context *ctx, const struct marshal_cmd_MemoryObjectParameterivEXT *cmd)
{
   const GLuint memoryObject = cmd->memoryObject;
   const GLenum pname = cmd->pname;
   GLint * params;
   const char *variable_data = (const char *) (cmd + 1);
   params = (GLint *) variable_data;
   CALL_MemoryObjectParameterivEXT(ctx->CurrentServerDispatch, (memoryObject, pname, params));
}
void GLAPIENTRY
_mesa_marshal_MemoryObjectParameterivEXT(GLuint memoryObject, GLenum pname, const GLint * params)
{
   GET_CURRENT_CONTEXT(ctx);
   int params_size = safe_mul(_mesa_memobj_enum_to_count(pname), 1 * sizeof(GLint));
   int cmd_size = sizeof(struct marshal_cmd_MemoryObjectParameterivEXT) + params_size;
   struct marshal_cmd_MemoryObjectParameterivEXT *cmd;
   if (unlikely(params_size < 0 || (params_size > 0 && !params) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "MemoryObjectParameterivEXT");
      CALL_MemoryObjectParameterivEXT(ctx->CurrentServerDispatch, (memoryObject, pname, params));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MemoryObjectParameterivEXT, cmd_size);
   cmd->memoryObject = memoryObject;
   cmd->pname = pname;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, params, params_size);
}


/* GetMemoryObjectParameterivEXT: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetMemoryObjectParameterivEXT(GLuint memoryObject, GLenum pname, GLint * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetMemoryObjectParameterivEXT");
   CALL_GetMemoryObjectParameterivEXT(ctx->CurrentServerDispatch, (memoryObject, pname, params));
}


/* TexStorageMem2DEXT: marshalled asynchronously */
struct marshal_cmd_TexStorageMem2DEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLenum16 internalFormat;
   GLsizei levels;
   GLsizei width;
   GLsizei height;
   GLuint memory;
   GLuint64 offset;
};
void
_mesa_unmarshal_TexStorageMem2DEXT(struct gl_context *ctx, const struct marshal_cmd_TexStorageMem2DEXT *cmd)
{
   const GLenum target = cmd->target;
   const GLsizei levels = cmd->levels;
   const GLenum internalFormat = cmd->internalFormat;
   const GLsizei width = cmd->width;
   const GLsizei height = cmd->height;
   const GLuint memory = cmd->memory;
   const GLuint64 offset = cmd->offset;
   CALL_TexStorageMem2DEXT(ctx->CurrentServerDispatch, (target, levels, internalFormat, width, height, memory, offset));
}
void GLAPIENTRY
_mesa_marshal_TexStorageMem2DEXT(GLenum target, GLsizei levels, GLenum internalFormat, GLsizei width, GLsizei height, GLuint memory, GLuint64 offset)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TexStorageMem2DEXT);
   struct marshal_cmd_TexStorageMem2DEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexStorageMem2DEXT, cmd_size);
   cmd->target = target;
   cmd->levels = levels;
   cmd->internalFormat = internalFormat;
   cmd->width = width;
   cmd->height = height;
   cmd->memory = memory;
   cmd->offset = offset;
}


/* TexStorageMem2DMultisampleEXT: marshalled asynchronously */
struct marshal_cmd_TexStorageMem2DMultisampleEXT
{
   struct marshal_cmd_base cmd_base;
   GLboolean fixedSampleLocations;
   GLenum16 target;
   GLenum16 internalFormat;
   GLsizei samples;
   GLsizei width;
   GLsizei height;
   GLuint memory;
   GLuint64 offset;
};
void
_mesa_unmarshal_TexStorageMem2DMultisampleEXT(struct gl_context *ctx, const struct marshal_cmd_TexStorageMem2DMultisampleEXT *cmd)
{
   const GLenum target = cmd->target;
   const GLsizei samples = cmd->samples;
   const GLenum internalFormat = cmd->internalFormat;
   const GLsizei width = cmd->width;
   const GLsizei height = cmd->height;
   const GLboolean fixedSampleLocations = cmd->fixedSampleLocations;
   const GLuint memory = cmd->memory;
   const GLuint64 offset = cmd->offset;
   CALL_TexStorageMem2DMultisampleEXT(ctx->CurrentServerDispatch, (target, samples, internalFormat, width, height, fixedSampleLocations, memory, offset));
}
void GLAPIENTRY
_mesa_marshal_TexStorageMem2DMultisampleEXT(GLenum target, GLsizei samples, GLenum internalFormat, GLsizei width, GLsizei height, GLboolean fixedSampleLocations, GLuint memory, GLuint64 offset)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TexStorageMem2DMultisampleEXT);
   struct marshal_cmd_TexStorageMem2DMultisampleEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexStorageMem2DMultisampleEXT, cmd_size);
   cmd->target = target;
   cmd->samples = samples;
   cmd->internalFormat = internalFormat;
   cmd->width = width;
   cmd->height = height;
   cmd->fixedSampleLocations = fixedSampleLocations;
   cmd->memory = memory;
   cmd->offset = offset;
}


/* TexStorageMem3DEXT: marshalled asynchronously */
struct marshal_cmd_TexStorageMem3DEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLenum16 internalFormat;
   GLsizei levels;
   GLsizei width;
   GLsizei height;
   GLsizei depth;
   GLuint memory;
   GLuint64 offset;
};
void
_mesa_unmarshal_TexStorageMem3DEXT(struct gl_context *ctx, const struct marshal_cmd_TexStorageMem3DEXT *cmd)
{
   const GLenum target = cmd->target;
   const GLsizei levels = cmd->levels;
   const GLenum internalFormat = cmd->internalFormat;
   const GLsizei width = cmd->width;
   const GLsizei height = cmd->height;
   const GLsizei depth = cmd->depth;
   const GLuint memory = cmd->memory;
   const GLuint64 offset = cmd->offset;
   CALL_TexStorageMem3DEXT(ctx->CurrentServerDispatch, (target, levels, internalFormat, width, height, depth, memory, offset));
}
void GLAPIENTRY
_mesa_marshal_TexStorageMem3DEXT(GLenum target, GLsizei levels, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLuint memory, GLuint64 offset)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TexStorageMem3DEXT);
   struct marshal_cmd_TexStorageMem3DEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexStorageMem3DEXT, cmd_size);
   cmd->target = target;
   cmd->levels = levels;
   cmd->internalFormat = internalFormat;
   cmd->width = width;
   cmd->height = height;
   cmd->depth = depth;
   cmd->memory = memory;
   cmd->offset = offset;
}


/* TexStorageMem3DMultisampleEXT: marshalled asynchronously */
struct marshal_cmd_TexStorageMem3DMultisampleEXT
{
   struct marshal_cmd_base cmd_base;
   GLboolean fixedSampleLocations;
   GLenum16 target;
   GLenum16 internalFormat;
   GLsizei samples;
   GLsizei width;
   GLsizei height;
   GLsizei depth;
   GLuint memory;
   GLuint64 offset;
};
void
_mesa_unmarshal_TexStorageMem3DMultisampleEXT(struct gl_context *ctx, const struct marshal_cmd_TexStorageMem3DMultisampleEXT *cmd)
{
   const GLenum target = cmd->target;
   const GLsizei samples = cmd->samples;
   const GLenum internalFormat = cmd->internalFormat;
   const GLsizei width = cmd->width;
   const GLsizei height = cmd->height;
   const GLsizei depth = cmd->depth;
   const GLboolean fixedSampleLocations = cmd->fixedSampleLocations;
   const GLuint memory = cmd->memory;
   const GLuint64 offset = cmd->offset;
   CALL_TexStorageMem3DMultisampleEXT(ctx->CurrentServerDispatch, (target, samples, internalFormat, width, height, depth, fixedSampleLocations, memory, offset));
}
void GLAPIENTRY
_mesa_marshal_TexStorageMem3DMultisampleEXT(GLenum target, GLsizei samples, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedSampleLocations, GLuint memory, GLuint64 offset)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TexStorageMem3DMultisampleEXT);
   struct marshal_cmd_TexStorageMem3DMultisampleEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexStorageMem3DMultisampleEXT, cmd_size);
   cmd->target = target;
   cmd->samples = samples;
   cmd->internalFormat = internalFormat;
   cmd->width = width;
   cmd->height = height;
   cmd->depth = depth;
   cmd->fixedSampleLocations = fixedSampleLocations;
   cmd->memory = memory;
   cmd->offset = offset;
}


/* BufferStorageMemEXT: marshalled asynchronously */
struct marshal_cmd_BufferStorageMemEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLuint memory;
   GLsizeiptr size;
   GLuint64 offset;
};
void
_mesa_unmarshal_BufferStorageMemEXT(struct gl_context *ctx, const struct marshal_cmd_BufferStorageMemEXT *cmd)
{
   const GLenum target = cmd->target;
   const GLsizeiptr size = cmd->size;
   const GLuint memory = cmd->memory;
   const GLuint64 offset = cmd->offset;
   CALL_BufferStorageMemEXT(ctx->CurrentServerDispatch, (target, size, memory, offset));
}
void GLAPIENTRY
_mesa_marshal_BufferStorageMemEXT(GLenum target, GLsizeiptr size, GLuint memory, GLuint64 offset)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_BufferStorageMemEXT);
   struct marshal_cmd_BufferStorageMemEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_BufferStorageMemEXT, cmd_size);
   cmd->target = target;
   cmd->size = size;
   cmd->memory = memory;
   cmd->offset = offset;
}


/* TextureStorageMem2DEXT: marshalled asynchronously */
struct marshal_cmd_TextureStorageMem2DEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 texture;
   GLenum16 internalFormat;
   GLsizei levels;
   GLsizei width;
   GLsizei height;
   GLuint memory;
   GLuint64 offset;
};
void
_mesa_unmarshal_TextureStorageMem2DEXT(struct gl_context *ctx, const struct marshal_cmd_TextureStorageMem2DEXT *cmd)
{
   const GLenum texture = cmd->texture;
   const GLsizei levels = cmd->levels;
   const GLenum internalFormat = cmd->internalFormat;
   const GLsizei width = cmd->width;
   const GLsizei height = cmd->height;
   const GLuint memory = cmd->memory;
   const GLuint64 offset = cmd->offset;
   CALL_TextureStorageMem2DEXT(ctx->CurrentServerDispatch, (texture, levels, internalFormat, width, height, memory, offset));
}
void GLAPIENTRY
_mesa_marshal_TextureStorageMem2DEXT(GLenum texture, GLsizei levels, GLenum internalFormat, GLsizei width, GLsizei height, GLuint memory, GLuint64 offset)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TextureStorageMem2DEXT);
   struct marshal_cmd_TextureStorageMem2DEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TextureStorageMem2DEXT, cmd_size);
   cmd->texture = texture;
   cmd->levels = levels;
   cmd->internalFormat = internalFormat;
   cmd->width = width;
   cmd->height = height;
   cmd->memory = memory;
   cmd->offset = offset;
}


/* TextureStorageMem2DMultisampleEXT: marshalled asynchronously */
struct marshal_cmd_TextureStorageMem2DMultisampleEXT
{
   struct marshal_cmd_base cmd_base;
   GLboolean fixedSampleLocations;
   GLenum16 internalFormat;
   GLuint texture;
   GLsizei samples;
   GLsizei width;
   GLsizei height;
   GLuint memory;
   GLuint64 offset;
};
void
_mesa_unmarshal_TextureStorageMem2DMultisampleEXT(struct gl_context *ctx, const struct marshal_cmd_TextureStorageMem2DMultisampleEXT *cmd)
{
   const GLuint texture = cmd->texture;
   const GLsizei samples = cmd->samples;
   const GLenum internalFormat = cmd->internalFormat;
   const GLsizei width = cmd->width;
   const GLsizei height = cmd->height;
   const GLboolean fixedSampleLocations = cmd->fixedSampleLocations;
   const GLuint memory = cmd->memory;
   const GLuint64 offset = cmd->offset;
   CALL_TextureStorageMem2DMultisampleEXT(ctx->CurrentServerDispatch, (texture, samples, internalFormat, width, height, fixedSampleLocations, memory, offset));
}
void GLAPIENTRY
_mesa_marshal_TextureStorageMem2DMultisampleEXT(GLuint texture, GLsizei samples, GLenum internalFormat, GLsizei width, GLsizei height, GLboolean fixedSampleLocations, GLuint memory, GLuint64 offset)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TextureStorageMem2DMultisampleEXT);
   struct marshal_cmd_TextureStorageMem2DMultisampleEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TextureStorageMem2DMultisampleEXT, cmd_size);
   cmd->texture = texture;
   cmd->samples = samples;
   cmd->internalFormat = internalFormat;
   cmd->width = width;
   cmd->height = height;
   cmd->fixedSampleLocations = fixedSampleLocations;
   cmd->memory = memory;
   cmd->offset = offset;
}


/* TextureStorageMem3DEXT: marshalled asynchronously */
struct marshal_cmd_TextureStorageMem3DEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 internalFormat;
   GLuint texture;
   GLsizei levels;
   GLsizei width;
   GLsizei height;
   GLsizei depth;
   GLuint memory;
   GLuint64 offset;
};
void
_mesa_unmarshal_TextureStorageMem3DEXT(struct gl_context *ctx, const struct marshal_cmd_TextureStorageMem3DEXT *cmd)
{
   const GLuint texture = cmd->texture;
   const GLsizei levels = cmd->levels;
   const GLenum internalFormat = cmd->internalFormat;
   const GLsizei width = cmd->width;
   const GLsizei height = cmd->height;
   const GLsizei depth = cmd->depth;
   const GLuint memory = cmd->memory;
   const GLuint64 offset = cmd->offset;
   CALL_TextureStorageMem3DEXT(ctx->CurrentServerDispatch, (texture, levels, internalFormat, width, height, depth, memory, offset));
}
void GLAPIENTRY
_mesa_marshal_TextureStorageMem3DEXT(GLuint texture, GLsizei levels, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLuint memory, GLuint64 offset)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TextureStorageMem3DEXT);
   struct marshal_cmd_TextureStorageMem3DEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TextureStorageMem3DEXT, cmd_size);
   cmd->texture = texture;
   cmd->levels = levels;
   cmd->internalFormat = internalFormat;
   cmd->width = width;
   cmd->height = height;
   cmd->depth = depth;
   cmd->memory = memory;
   cmd->offset = offset;
}


/* TextureStorageMem3DMultisampleEXT: marshalled asynchronously */
struct marshal_cmd_TextureStorageMem3DMultisampleEXT
{
   struct marshal_cmd_base cmd_base;
   GLboolean fixedSampleLocations;
   GLenum16 internalFormat;
   GLuint texture;
   GLsizei samples;
   GLsizei width;
   GLsizei height;
   GLsizei depth;
   GLuint memory;
   GLuint64 offset;
};
void
_mesa_unmarshal_TextureStorageMem3DMultisampleEXT(struct gl_context *ctx, const struct marshal_cmd_TextureStorageMem3DMultisampleEXT *cmd)
{
   const GLuint texture = cmd->texture;
   const GLsizei samples = cmd->samples;
   const GLenum internalFormat = cmd->internalFormat;
   const GLsizei width = cmd->width;
   const GLsizei height = cmd->height;
   const GLsizei depth = cmd->depth;
   const GLboolean fixedSampleLocations = cmd->fixedSampleLocations;
   const GLuint memory = cmd->memory;
   const GLuint64 offset = cmd->offset;
   CALL_TextureStorageMem3DMultisampleEXT(ctx->CurrentServerDispatch, (texture, samples, internalFormat, width, height, depth, fixedSampleLocations, memory, offset));
}
void GLAPIENTRY
_mesa_marshal_TextureStorageMem3DMultisampleEXT(GLuint texture, GLsizei samples, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedSampleLocations, GLuint memory, GLuint64 offset)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TextureStorageMem3DMultisampleEXT);
   struct marshal_cmd_TextureStorageMem3DMultisampleEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TextureStorageMem3DMultisampleEXT, cmd_size);
   cmd->texture = texture;
   cmd->samples = samples;
   cmd->internalFormat = internalFormat;
   cmd->width = width;
   cmd->height = height;
   cmd->depth = depth;
   cmd->fixedSampleLocations = fixedSampleLocations;
   cmd->memory = memory;
   cmd->offset = offset;
}


/* NamedBufferStorageMemEXT: marshalled asynchronously */
struct marshal_cmd_NamedBufferStorageMemEXT
{
   struct marshal_cmd_base cmd_base;
   GLuint buffer;
   GLuint memory;
   GLsizeiptr size;
   GLuint64 offset;
};
void
_mesa_unmarshal_NamedBufferStorageMemEXT(struct gl_context *ctx, const struct marshal_cmd_NamedBufferStorageMemEXT *cmd)
{
   const GLuint buffer = cmd->buffer;
   const GLsizeiptr size = cmd->size;
   const GLuint memory = cmd->memory;
   const GLuint64 offset = cmd->offset;
   CALL_NamedBufferStorageMemEXT(ctx->CurrentServerDispatch, (buffer, size, memory, offset));
}
void GLAPIENTRY
_mesa_marshal_NamedBufferStorageMemEXT(GLuint buffer, GLsizeiptr size, GLuint memory, GLuint64 offset)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_NamedBufferStorageMemEXT);
   struct marshal_cmd_NamedBufferStorageMemEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_NamedBufferStorageMemEXT, cmd_size);
   cmd->buffer = buffer;
   cmd->size = size;
   cmd->memory = memory;
   cmd->offset = offset;
}


/* TexStorageMem1DEXT: marshalled asynchronously */
struct marshal_cmd_TexStorageMem1DEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLenum16 internalFormat;
   GLsizei levels;
   GLsizei width;
   GLuint memory;
   GLuint64 offset;
};
void
_mesa_unmarshal_TexStorageMem1DEXT(struct gl_context *ctx, const struct marshal_cmd_TexStorageMem1DEXT *cmd)
{
   const GLenum target = cmd->target;
   const GLsizei levels = cmd->levels;
   const GLenum internalFormat = cmd->internalFormat;
   const GLsizei width = cmd->width;
   const GLuint memory = cmd->memory;
   const GLuint64 offset = cmd->offset;
   CALL_TexStorageMem1DEXT(ctx->CurrentServerDispatch, (target, levels, internalFormat, width, memory, offset));
}
void GLAPIENTRY
_mesa_marshal_TexStorageMem1DEXT(GLenum target, GLsizei levels, GLenum internalFormat, GLsizei width, GLuint memory, GLuint64 offset)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TexStorageMem1DEXT);
   struct marshal_cmd_TexStorageMem1DEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexStorageMem1DEXT, cmd_size);
   cmd->target = target;
   cmd->levels = levels;
   cmd->internalFormat = internalFormat;
   cmd->width = width;
   cmd->memory = memory;
   cmd->offset = offset;
}


/* TextureStorageMem1DEXT: marshalled asynchronously */
struct marshal_cmd_TextureStorageMem1DEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 internalFormat;
   GLuint texture;
   GLsizei levels;
   GLsizei width;
   GLuint memory;
   GLuint64 offset;
};
void
_mesa_unmarshal_TextureStorageMem1DEXT(struct gl_context *ctx, const struct marshal_cmd_TextureStorageMem1DEXT *cmd)
{
   const GLuint texture = cmd->texture;
   const GLsizei levels = cmd->levels;
   const GLenum internalFormat = cmd->internalFormat;
   const GLsizei width = cmd->width;
   const GLuint memory = cmd->memory;
   const GLuint64 offset = cmd->offset;
   CALL_TextureStorageMem1DEXT(ctx->CurrentServerDispatch, (texture, levels, internalFormat, width, memory, offset));
}
void GLAPIENTRY
_mesa_marshal_TextureStorageMem1DEXT(GLuint texture, GLsizei levels, GLenum internalFormat, GLsizei width, GLuint memory, GLuint64 offset)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TextureStorageMem1DEXT);
   struct marshal_cmd_TextureStorageMem1DEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TextureStorageMem1DEXT, cmd_size);
   cmd->texture = texture;
   cmd->levels = levels;
   cmd->internalFormat = internalFormat;
   cmd->width = width;
   cmd->memory = memory;
   cmd->offset = offset;
}


/* GenSemaphoresEXT: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GenSemaphoresEXT(GLsizei n, GLuint * semaphores)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GenSemaphoresEXT");
   CALL_GenSemaphoresEXT(ctx->CurrentServerDispatch, (n, semaphores));
}


/* DeleteSemaphoresEXT: marshalled asynchronously */
struct marshal_cmd_DeleteSemaphoresEXT
{
   struct marshal_cmd_base cmd_base;
   GLsizei n;
   /* Next safe_mul(n, 1 * sizeof(GLuint)) bytes are GLuint semaphores[n] */
};
void
_mesa_unmarshal_DeleteSemaphoresEXT(struct gl_context *ctx, const struct marshal_cmd_DeleteSemaphoresEXT *cmd)
{
   const GLsizei n = cmd->n;
   GLuint * semaphores;
   const char *variable_data = (const char *) (cmd + 1);
   semaphores = (GLuint *) variable_data;
   CALL_DeleteSemaphoresEXT(ctx->CurrentServerDispatch, (n, semaphores));
}
void GLAPIENTRY
_mesa_marshal_DeleteSemaphoresEXT(GLsizei n, const GLuint * semaphores)
{
   GET_CURRENT_CONTEXT(ctx);
   int semaphores_size = safe_mul(n, 1 * sizeof(GLuint));
   int cmd_size = sizeof(struct marshal_cmd_DeleteSemaphoresEXT) + semaphores_size;
   struct marshal_cmd_DeleteSemaphoresEXT *cmd;
   if (unlikely(semaphores_size < 0 || (semaphores_size > 0 && !semaphores) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "DeleteSemaphoresEXT");
      CALL_DeleteSemaphoresEXT(ctx->CurrentServerDispatch, (n, semaphores));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_DeleteSemaphoresEXT, cmd_size);
   cmd->n = n;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, semaphores, semaphores_size);
}


/* IsSemaphoreEXT: marshalled synchronously */
GLboolean GLAPIENTRY
_mesa_marshal_IsSemaphoreEXT(GLuint semaphore)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "IsSemaphoreEXT");
   return CALL_IsSemaphoreEXT(ctx->CurrentServerDispatch, (semaphore));
}


/* SemaphoreParameterui64vEXT: marshalled asynchronously */
struct marshal_cmd_SemaphoreParameterui64vEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 pname;
   GLuint semaphore;
   /* Next safe_mul(_mesa_semaphore_enum_to_count(pname), 1 * sizeof(GLuint64)) bytes are GLuint64 params[_mesa_semaphore_enum_to_count(pname)] */
};
void
_mesa_unmarshal_SemaphoreParameterui64vEXT(struct gl_context *ctx, const struct marshal_cmd_SemaphoreParameterui64vEXT *cmd)
{
   const GLuint semaphore = cmd->semaphore;
   const GLenum pname = cmd->pname;
   GLuint64 * params;
   const char *variable_data = (const char *) (cmd + 1);
   params = (GLuint64 *) variable_data;
   CALL_SemaphoreParameterui64vEXT(ctx->CurrentServerDispatch, (semaphore, pname, params));
}
void GLAPIENTRY
_mesa_marshal_SemaphoreParameterui64vEXT(GLuint semaphore, GLenum pname, const GLuint64 * params)
{
   GET_CURRENT_CONTEXT(ctx);
   int params_size = safe_mul(_mesa_semaphore_enum_to_count(pname), 1 * sizeof(GLuint64));
   int cmd_size = sizeof(struct marshal_cmd_SemaphoreParameterui64vEXT) + params_size;
   struct marshal_cmd_SemaphoreParameterui64vEXT *cmd;
   if (unlikely(params_size < 0 || (params_size > 0 && !params) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "SemaphoreParameterui64vEXT");
      CALL_SemaphoreParameterui64vEXT(ctx->CurrentServerDispatch, (semaphore, pname, params));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_SemaphoreParameterui64vEXT, cmd_size);
   cmd->semaphore = semaphore;
   cmd->pname = pname;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, params, params_size);
}


/* GetSemaphoreParameterui64vEXT: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetSemaphoreParameterui64vEXT(GLuint semaphore, GLenum pname, GLuint64 * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetSemaphoreParameterui64vEXT");
   CALL_GetSemaphoreParameterui64vEXT(ctx->CurrentServerDispatch, (semaphore, pname, params));
}


/* WaitSemaphoreEXT: marshalled asynchronously */
struct marshal_cmd_WaitSemaphoreEXT
{
   struct marshal_cmd_base cmd_base;
   GLuint semaphore;
   GLuint numBufferBarriers;
   GLuint numTextureBarriers;
   /* Next safe_mul(numBufferBarriers, 1 * sizeof(GLuint)) bytes are GLuint buffers[numBufferBarriers] */
   /* Next safe_mul(numTextureBarriers, 1 * sizeof(GLuint)) bytes are GLuint textures[numTextureBarriers] */
   /* Next safe_mul(numTextureBarriers, 1 * sizeof(GLenum)) bytes are GLenum srcLayouts[numTextureBarriers] */
};
void
_mesa_unmarshal_WaitSemaphoreEXT(struct gl_context *ctx, const struct marshal_cmd_WaitSemaphoreEXT *cmd)
{
   const GLuint semaphore = cmd->semaphore;
   const GLuint numBufferBarriers = cmd->numBufferBarriers;
   const GLuint numTextureBarriers = cmd->numTextureBarriers;
   GLuint * buffers;
   GLuint * textures;
   GLenum * srcLayouts;
   const char *variable_data = (const char *) (cmd + 1);
   buffers = (GLuint *) variable_data;
   variable_data += numBufferBarriers * 1 * sizeof(GLuint);
   textures = (GLuint *) variable_data;
   variable_data += numTextureBarriers * 1 * sizeof(GLuint);
   srcLayouts = (GLenum *) variable_data;
   CALL_WaitSemaphoreEXT(ctx->CurrentServerDispatch, (semaphore, numBufferBarriers, buffers, numTextureBarriers, textures, srcLayouts));
}
void GLAPIENTRY
_mesa_marshal_WaitSemaphoreEXT(GLuint semaphore, GLuint numBufferBarriers, const GLuint * buffers, GLuint numTextureBarriers, const GLuint * textures, const GLenum * srcLayouts)
{
   GET_CURRENT_CONTEXT(ctx);
   int buffers_size = safe_mul(numBufferBarriers, 1 * sizeof(GLuint));
   int textures_size = safe_mul(numTextureBarriers, 1 * sizeof(GLuint));
   int srcLayouts_size = safe_mul(numTextureBarriers, 1 * sizeof(GLenum));
   int cmd_size = sizeof(struct marshal_cmd_WaitSemaphoreEXT) + buffers_size + textures_size + srcLayouts_size;
   struct marshal_cmd_WaitSemaphoreEXT *cmd;
   if (unlikely(buffers_size < 0 || (buffers_size > 0 && !buffers) || textures_size < 0 || (textures_size > 0 && !textures) || srcLayouts_size < 0 || (srcLayouts_size > 0 && !srcLayouts) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "WaitSemaphoreEXT");
      CALL_WaitSemaphoreEXT(ctx->CurrentServerDispatch, (semaphore, numBufferBarriers, buffers, numTextureBarriers, textures, srcLayouts));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_WaitSemaphoreEXT, cmd_size);
   cmd->semaphore = semaphore;
   cmd->numBufferBarriers = numBufferBarriers;
   cmd->numTextureBarriers = numTextureBarriers;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, buffers, buffers_size);
   variable_data += buffers_size;
   memcpy(variable_data, textures, textures_size);
   variable_data += textures_size;
   memcpy(variable_data, srcLayouts, srcLayouts_size);
}


/* SignalSemaphoreEXT: marshalled asynchronously */
struct marshal_cmd_SignalSemaphoreEXT
{
   struct marshal_cmd_base cmd_base;
   GLuint semaphore;
   GLuint numBufferBarriers;
   GLuint numTextureBarriers;
   /* Next safe_mul(numBufferBarriers, 1 * sizeof(GLuint)) bytes are GLuint buffers[numBufferBarriers] */
   /* Next safe_mul(numTextureBarriers, 1 * sizeof(GLuint)) bytes are GLuint textures[numTextureBarriers] */
   /* Next safe_mul(numTextureBarriers, 1 * sizeof(GLenum)) bytes are GLenum dstLayouts[numTextureBarriers] */
};
void
_mesa_unmarshal_SignalSemaphoreEXT(struct gl_context *ctx, const struct marshal_cmd_SignalSemaphoreEXT *cmd)
{
   const GLuint semaphore = cmd->semaphore;
   const GLuint numBufferBarriers = cmd->numBufferBarriers;
   const GLuint numTextureBarriers = cmd->numTextureBarriers;
   GLuint * buffers;
   GLuint * textures;
   GLenum * dstLayouts;
   const char *variable_data = (const char *) (cmd + 1);
   buffers = (GLuint *) variable_data;
   variable_data += numBufferBarriers * 1 * sizeof(GLuint);
   textures = (GLuint *) variable_data;
   variable_data += numTextureBarriers * 1 * sizeof(GLuint);
   dstLayouts = (GLenum *) variable_data;
   CALL_SignalSemaphoreEXT(ctx->CurrentServerDispatch, (semaphore, numBufferBarriers, buffers, numTextureBarriers, textures, dstLayouts));
}
void GLAPIENTRY
_mesa_marshal_SignalSemaphoreEXT(GLuint semaphore, GLuint numBufferBarriers, const GLuint * buffers, GLuint numTextureBarriers, const GLuint * textures, const GLenum * dstLayouts)
{
   GET_CURRENT_CONTEXT(ctx);
   int buffers_size = safe_mul(numBufferBarriers, 1 * sizeof(GLuint));
   int textures_size = safe_mul(numTextureBarriers, 1 * sizeof(GLuint));
   int dstLayouts_size = safe_mul(numTextureBarriers, 1 * sizeof(GLenum));
   int cmd_size = sizeof(struct marshal_cmd_SignalSemaphoreEXT) + buffers_size + textures_size + dstLayouts_size;
   struct marshal_cmd_SignalSemaphoreEXT *cmd;
   if (unlikely(buffers_size < 0 || (buffers_size > 0 && !buffers) || textures_size < 0 || (textures_size > 0 && !textures) || dstLayouts_size < 0 || (dstLayouts_size > 0 && !dstLayouts) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "SignalSemaphoreEXT");
      CALL_SignalSemaphoreEXT(ctx->CurrentServerDispatch, (semaphore, numBufferBarriers, buffers, numTextureBarriers, textures, dstLayouts));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_SignalSemaphoreEXT, cmd_size);
   cmd->semaphore = semaphore;
   cmd->numBufferBarriers = numBufferBarriers;
   cmd->numTextureBarriers = numTextureBarriers;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, buffers, buffers_size);
   variable_data += buffers_size;
   memcpy(variable_data, textures, textures_size);
   variable_data += textures_size;
   memcpy(variable_data, dstLayouts, dstLayouts_size);
}


/* ImportMemoryFdEXT: marshalled asynchronously */
struct marshal_cmd_ImportMemoryFdEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 handleType;
   GLuint memory;
   GLint fd;
   GLuint64 size;
};
void
_mesa_unmarshal_ImportMemoryFdEXT(struct gl_context *ctx, const struct marshal_cmd_ImportMemoryFdEXT *cmd)
{
   const GLuint memory = cmd->memory;
   const GLuint64 size = cmd->size;
   const GLenum handleType = cmd->handleType;
   const GLint fd = cmd->fd;
   CALL_ImportMemoryFdEXT(ctx->CurrentServerDispatch, (memory, size, handleType, fd));
}
void GLAPIENTRY
_mesa_marshal_ImportMemoryFdEXT(GLuint memory, GLuint64 size, GLenum handleType, GLint fd)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ImportMemoryFdEXT);
   struct marshal_cmd_ImportMemoryFdEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ImportMemoryFdEXT, cmd_size);
   cmd->memory = memory;
   cmd->size = size;
   cmd->handleType = handleType;
   cmd->fd = fd;
}


/* ImportSemaphoreFdEXT: marshalled asynchronously */
struct marshal_cmd_ImportSemaphoreFdEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 handleType;
   GLuint semaphore;
   GLint fd;
};
void
_mesa_unmarshal_ImportSemaphoreFdEXT(struct gl_context *ctx, const struct marshal_cmd_ImportSemaphoreFdEXT *cmd)
{
   const GLuint semaphore = cmd->semaphore;
   const GLenum handleType = cmd->handleType;
   const GLint fd = cmd->fd;
   CALL_ImportSemaphoreFdEXT(ctx->CurrentServerDispatch, (semaphore, handleType, fd));
}
void GLAPIENTRY
_mesa_marshal_ImportSemaphoreFdEXT(GLuint semaphore, GLenum handleType, GLint fd)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ImportSemaphoreFdEXT);
   struct marshal_cmd_ImportSemaphoreFdEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ImportSemaphoreFdEXT, cmd_size);
   cmd->semaphore = semaphore;
   cmd->handleType = handleType;
   cmd->fd = fd;
}


/* ViewportSwizzleNV: marshalled asynchronously */
struct marshal_cmd_ViewportSwizzleNV
{
   struct marshal_cmd_base cmd_base;
   GLenum16 swizzlex;
   GLenum16 swizzley;
   GLenum16 swizzlez;
   GLenum16 swizzlew;
   GLuint index;
};
void
_mesa_unmarshal_ViewportSwizzleNV(struct gl_context *ctx, const struct marshal_cmd_ViewportSwizzleNV *cmd)
{
   const GLuint index = cmd->index;
   const GLenum swizzlex = cmd->swizzlex;
   const GLenum swizzley = cmd->swizzley;
   const GLenum swizzlez = cmd->swizzlez;
   const GLenum swizzlew = cmd->swizzlew;
   CALL_ViewportSwizzleNV(ctx->CurrentServerDispatch, (index, swizzlex, swizzley, swizzlez, swizzlew));
}
void GLAPIENTRY
_mesa_marshal_ViewportSwizzleNV(GLuint index, GLenum swizzlex, GLenum swizzley, GLenum swizzlez, GLenum swizzlew)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ViewportSwizzleNV);
   struct marshal_cmd_ViewportSwizzleNV *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ViewportSwizzleNV, cmd_size);
   cmd->index = index;
   cmd->swizzlex = swizzlex;
   cmd->swizzley = swizzley;
   cmd->swizzlez = swizzlez;
   cmd->swizzlew = swizzlew;
}


/* MemoryBarrierByRegion: marshalled asynchronously */
struct marshal_cmd_MemoryBarrierByRegion
{
   struct marshal_cmd_base cmd_base;
   GLbitfield barriers;
};
void
_mesa_unmarshal_MemoryBarrierByRegion(struct gl_context *ctx, const struct marshal_cmd_MemoryBarrierByRegion *cmd)
{
   const GLbitfield barriers = cmd->barriers;
   CALL_MemoryBarrierByRegion(ctx->CurrentServerDispatch, (barriers));
}
void GLAPIENTRY
_mesa_marshal_MemoryBarrierByRegion(GLbitfield barriers)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MemoryBarrierByRegion);
   struct marshal_cmd_MemoryBarrierByRegion *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MemoryBarrierByRegion, cmd_size);
   cmd->barriers = barriers;
}


/* AlphaFuncx: marshalled asynchronously */
struct marshal_cmd_AlphaFuncx
{
   struct marshal_cmd_base cmd_base;
   GLenum16 func;
   GLclampx ref;
};
void
_mesa_unmarshal_AlphaFuncx(struct gl_context *ctx, const struct marshal_cmd_AlphaFuncx *cmd)
{
   const GLenum func = cmd->func;
   const GLclampx ref = cmd->ref;
   CALL_AlphaFuncx(ctx->CurrentServerDispatch, (func, ref));
}
void GLAPIENTRY
_mesa_marshal_AlphaFuncx(GLenum func, GLclampx ref)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_AlphaFuncx);
   struct marshal_cmd_AlphaFuncx *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_AlphaFuncx, cmd_size);
   cmd->func = func;
   cmd->ref = ref;
}


/* ClearColorx: marshalled asynchronously */
struct marshal_cmd_ClearColorx
{
   struct marshal_cmd_base cmd_base;
   GLclampx red;
   GLclampx green;
   GLclampx blue;
   GLclampx alpha;
};
void
_mesa_unmarshal_ClearColorx(struct gl_context *ctx, const struct marshal_cmd_ClearColorx *cmd)
{
   const GLclampx red = cmd->red;
   const GLclampx green = cmd->green;
   const GLclampx blue = cmd->blue;
   const GLclampx alpha = cmd->alpha;
   CALL_ClearColorx(ctx->CurrentServerDispatch, (red, green, blue, alpha));
}
void GLAPIENTRY
_mesa_marshal_ClearColorx(GLclampx red, GLclampx green, GLclampx blue, GLclampx alpha)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ClearColorx);
   struct marshal_cmd_ClearColorx *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ClearColorx, cmd_size);
   cmd->red = red;
   cmd->green = green;
   cmd->blue = blue;
   cmd->alpha = alpha;
}


/* ClearDepthx: marshalled asynchronously */
struct marshal_cmd_ClearDepthx
{
   struct marshal_cmd_base cmd_base;
   GLclampx depth;
};
void
_mesa_unmarshal_ClearDepthx(struct gl_context *ctx, const struct marshal_cmd_ClearDepthx *cmd)
{
   const GLclampx depth = cmd->depth;
   CALL_ClearDepthx(ctx->CurrentServerDispatch, (depth));
}
void GLAPIENTRY
_mesa_marshal_ClearDepthx(GLclampx depth)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ClearDepthx);
   struct marshal_cmd_ClearDepthx *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ClearDepthx, cmd_size);
   cmd->depth = depth;
}


/* Color4x: marshalled asynchronously */
struct marshal_cmd_Color4x
{
   struct marshal_cmd_base cmd_base;
   GLfixed red;
   GLfixed green;
   GLfixed blue;
   GLfixed alpha;
};
void
_mesa_unmarshal_Color4x(struct gl_context *ctx, const struct marshal_cmd_Color4x *cmd)
{
   const GLfixed red = cmd->red;
   const GLfixed green = cmd->green;
   const GLfixed blue = cmd->blue;
   const GLfixed alpha = cmd->alpha;
   CALL_Color4x(ctx->CurrentServerDispatch, (red, green, blue, alpha));
}
void GLAPIENTRY
_mesa_marshal_Color4x(GLfixed red, GLfixed green, GLfixed blue, GLfixed alpha)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Color4x);
   struct marshal_cmd_Color4x *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Color4x, cmd_size);
   cmd->red = red;
   cmd->green = green;
   cmd->blue = blue;
   cmd->alpha = alpha;
}


/* DepthRangex: marshalled asynchronously */
struct marshal_cmd_DepthRangex
{
   struct marshal_cmd_base cmd_base;
   GLclampx zNear;
   GLclampx zFar;
};
void
_mesa_unmarshal_DepthRangex(struct gl_context *ctx, const struct marshal_cmd_DepthRangex *cmd)
{
   const GLclampx zNear = cmd->zNear;
   const GLclampx zFar = cmd->zFar;
   CALL_DepthRangex(ctx->CurrentServerDispatch, (zNear, zFar));
}
void GLAPIENTRY
_mesa_marshal_DepthRangex(GLclampx zNear, GLclampx zFar)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_DepthRangex);
   struct marshal_cmd_DepthRangex *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_DepthRangex, cmd_size);
   cmd->zNear = zNear;
   cmd->zFar = zFar;
}


/* Fogx: marshalled asynchronously */
struct marshal_cmd_Fogx
{
   struct marshal_cmd_base cmd_base;
   GLenum16 pname;
   GLfixed param;
};
void
_mesa_unmarshal_Fogx(struct gl_context *ctx, const struct marshal_cmd_Fogx *cmd)
{
   const GLenum pname = cmd->pname;
   const GLfixed param = cmd->param;
   CALL_Fogx(ctx->CurrentServerDispatch, (pname, param));
}
void GLAPIENTRY
_mesa_marshal_Fogx(GLenum pname, GLfixed param)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Fogx);
   struct marshal_cmd_Fogx *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Fogx, cmd_size);
   cmd->pname = pname;
   cmd->param = param;
}


/* Fogxv: marshalled asynchronously */
struct marshal_cmd_Fogxv
{
   struct marshal_cmd_base cmd_base;
   GLenum16 pname;
   /* Next safe_mul(_mesa_fog_enum_to_count(pname), 1 * sizeof(GLfixed)) bytes are GLfixed params[None] */
};
void
_mesa_unmarshal_Fogxv(struct gl_context *ctx, const struct marshal_cmd_Fogxv *cmd)
{
   const GLenum pname = cmd->pname;
   GLfixed * params;
   const char *variable_data = (const char *) (cmd + 1);
   params = (GLfixed *) variable_data;
   CALL_Fogxv(ctx->CurrentServerDispatch, (pname, params));
}
void GLAPIENTRY
_mesa_marshal_Fogxv(GLenum pname, const GLfixed * params)
{
   GET_CURRENT_CONTEXT(ctx);
   int params_size = safe_mul(_mesa_fog_enum_to_count(pname), 1 * sizeof(GLfixed));
   int cmd_size = sizeof(struct marshal_cmd_Fogxv) + params_size;
   struct marshal_cmd_Fogxv *cmd;
   if (unlikely(params_size < 0 || (params_size > 0 && !params) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "Fogxv");
      CALL_Fogxv(ctx->CurrentServerDispatch, (pname, params));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Fogxv, cmd_size);
   cmd->pname = pname;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, params, params_size);
}


/* Frustumx: marshalled asynchronously */
struct marshal_cmd_Frustumx
{
   struct marshal_cmd_base cmd_base;
   GLfixed left;
   GLfixed right;
   GLfixed bottom;
   GLfixed top;
   GLfixed zNear;
   GLfixed zFar;
};
void
_mesa_unmarshal_Frustumx(struct gl_context *ctx, const struct marshal_cmd_Frustumx *cmd)
{
   const GLfixed left = cmd->left;
   const GLfixed right = cmd->right;
   const GLfixed bottom = cmd->bottom;
   const GLfixed top = cmd->top;
   const GLfixed zNear = cmd->zNear;
   const GLfixed zFar = cmd->zFar;
   CALL_Frustumx(ctx->CurrentServerDispatch, (left, right, bottom, top, zNear, zFar));
}
void GLAPIENTRY
_mesa_marshal_Frustumx(GLfixed left, GLfixed right, GLfixed bottom, GLfixed top, GLfixed zNear, GLfixed zFar)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Frustumx);
   struct marshal_cmd_Frustumx *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Frustumx, cmd_size);
   cmd->left = left;
   cmd->right = right;
   cmd->bottom = bottom;
   cmd->top = top;
   cmd->zNear = zNear;
   cmd->zFar = zFar;
}


/* LightModelx: marshalled asynchronously */
struct marshal_cmd_LightModelx
{
   struct marshal_cmd_base cmd_base;
   GLenum16 pname;
   GLfixed param;
};
void
_mesa_unmarshal_LightModelx(struct gl_context *ctx, const struct marshal_cmd_LightModelx *cmd)
{
   const GLenum pname = cmd->pname;
   const GLfixed param = cmd->param;
   CALL_LightModelx(ctx->CurrentServerDispatch, (pname, param));
}
void GLAPIENTRY
_mesa_marshal_LightModelx(GLenum pname, GLfixed param)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_LightModelx);
   struct marshal_cmd_LightModelx *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_LightModelx, cmd_size);
   cmd->pname = pname;
   cmd->param = param;
}


/* LightModelxv: marshalled asynchronously */
struct marshal_cmd_LightModelxv
{
   struct marshal_cmd_base cmd_base;
   GLenum16 pname;
   /* Next safe_mul(_mesa_light_model_enum_to_count(pname), 1 * sizeof(GLfixed)) bytes are GLfixed params[None] */
};
void
_mesa_unmarshal_LightModelxv(struct gl_context *ctx, const struct marshal_cmd_LightModelxv *cmd)
{
   const GLenum pname = cmd->pname;
   GLfixed * params;
   const char *variable_data = (const char *) (cmd + 1);
   params = (GLfixed *) variable_data;
   CALL_LightModelxv(ctx->CurrentServerDispatch, (pname, params));
}
void GLAPIENTRY
_mesa_marshal_LightModelxv(GLenum pname, const GLfixed * params)
{
   GET_CURRENT_CONTEXT(ctx);
   int params_size = safe_mul(_mesa_light_model_enum_to_count(pname), 1 * sizeof(GLfixed));
   int cmd_size = sizeof(struct marshal_cmd_LightModelxv) + params_size;
   struct marshal_cmd_LightModelxv *cmd;
   if (unlikely(params_size < 0 || (params_size > 0 && !params) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "LightModelxv");
      CALL_LightModelxv(ctx->CurrentServerDispatch, (pname, params));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_LightModelxv, cmd_size);
   cmd->pname = pname;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, params, params_size);
}


/* Lightx: marshalled asynchronously */
struct marshal_cmd_Lightx
{
   struct marshal_cmd_base cmd_base;
   GLenum16 light;
   GLenum16 pname;
   GLfixed param;
};
void
_mesa_unmarshal_Lightx(struct gl_context *ctx, const struct marshal_cmd_Lightx *cmd)
{
   const GLenum light = cmd->light;
   const GLenum pname = cmd->pname;
   const GLfixed param = cmd->param;
   CALL_Lightx(ctx->CurrentServerDispatch, (light, pname, param));
}
void GLAPIENTRY
_mesa_marshal_Lightx(GLenum light, GLenum pname, GLfixed param)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Lightx);
   struct marshal_cmd_Lightx *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Lightx, cmd_size);
   cmd->light = light;
   cmd->pname = pname;
   cmd->param = param;
}


/* Lightxv: marshalled asynchronously */
struct marshal_cmd_Lightxv
{
   struct marshal_cmd_base cmd_base;
   GLenum16 light;
   GLenum16 pname;
   /* Next safe_mul(_mesa_light_enum_to_count(pname), 1 * sizeof(GLfixed)) bytes are GLfixed params[None] */
};
void
_mesa_unmarshal_Lightxv(struct gl_context *ctx, const struct marshal_cmd_Lightxv *cmd)
{
   const GLenum light = cmd->light;
   const GLenum pname = cmd->pname;
   GLfixed * params;
   const char *variable_data = (const char *) (cmd + 1);
   params = (GLfixed *) variable_data;
   CALL_Lightxv(ctx->CurrentServerDispatch, (light, pname, params));
}
void GLAPIENTRY
_mesa_marshal_Lightxv(GLenum light, GLenum pname, const GLfixed * params)
{
   GET_CURRENT_CONTEXT(ctx);
   int params_size = safe_mul(_mesa_light_enum_to_count(pname), 1 * sizeof(GLfixed));
   int cmd_size = sizeof(struct marshal_cmd_Lightxv) + params_size;
   struct marshal_cmd_Lightxv *cmd;
   if (unlikely(params_size < 0 || (params_size > 0 && !params) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "Lightxv");
      CALL_Lightxv(ctx->CurrentServerDispatch, (light, pname, params));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Lightxv, cmd_size);
   cmd->light = light;
   cmd->pname = pname;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, params, params_size);
}


/* LineWidthx: marshalled asynchronously */
struct marshal_cmd_LineWidthx
{
   struct marshal_cmd_base cmd_base;
   GLfixed width;
};
void
_mesa_unmarshal_LineWidthx(struct gl_context *ctx, const struct marshal_cmd_LineWidthx *cmd)
{
   const GLfixed width = cmd->width;
   CALL_LineWidthx(ctx->CurrentServerDispatch, (width));
}
void GLAPIENTRY
_mesa_marshal_LineWidthx(GLfixed width)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_LineWidthx);
   struct marshal_cmd_LineWidthx *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_LineWidthx, cmd_size);
   cmd->width = width;
}


/* LoadMatrixx: marshalled asynchronously */
struct marshal_cmd_LoadMatrixx
{
   struct marshal_cmd_base cmd_base;
   GLfixed m[16];
};
void
_mesa_unmarshal_LoadMatrixx(struct gl_context *ctx, const struct marshal_cmd_LoadMatrixx *cmd)
{
   const GLfixed * m = cmd->m;
   CALL_LoadMatrixx(ctx->CurrentServerDispatch, (m));
}
void GLAPIENTRY
_mesa_marshal_LoadMatrixx(const GLfixed * m)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_LoadMatrixx);
   struct marshal_cmd_LoadMatrixx *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_LoadMatrixx, cmd_size);
   memcpy(cmd->m, m, 16 * sizeof(GLfixed));
}


/* Materialx: marshalled asynchronously */
struct marshal_cmd_Materialx
{
   struct marshal_cmd_base cmd_base;
   GLenum16 face;
   GLenum16 pname;
   GLfixed param;
};
void
_mesa_unmarshal_Materialx(struct gl_context *ctx, const struct marshal_cmd_Materialx *cmd)
{
   const GLenum face = cmd->face;
   const GLenum pname = cmd->pname;
   const GLfixed param = cmd->param;
   CALL_Materialx(ctx->CurrentServerDispatch, (face, pname, param));
}
void GLAPIENTRY
_mesa_marshal_Materialx(GLenum face, GLenum pname, GLfixed param)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Materialx);
   struct marshal_cmd_Materialx *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Materialx, cmd_size);
   cmd->face = face;
   cmd->pname = pname;
   cmd->param = param;
}


/* Materialxv: marshalled asynchronously */
struct marshal_cmd_Materialxv
{
   struct marshal_cmd_base cmd_base;
   GLenum16 face;
   GLenum16 pname;
   /* Next safe_mul(_mesa_material_enum_to_count(pname), 1 * sizeof(GLfixed)) bytes are GLfixed params[None] */
};
void
_mesa_unmarshal_Materialxv(struct gl_context *ctx, const struct marshal_cmd_Materialxv *cmd)
{
   const GLenum face = cmd->face;
   const GLenum pname = cmd->pname;
   GLfixed * params;
   const char *variable_data = (const char *) (cmd + 1);
   params = (GLfixed *) variable_data;
   CALL_Materialxv(ctx->CurrentServerDispatch, (face, pname, params));
}
void GLAPIENTRY
_mesa_marshal_Materialxv(GLenum face, GLenum pname, const GLfixed * params)
{
   GET_CURRENT_CONTEXT(ctx);
   int params_size = safe_mul(_mesa_material_enum_to_count(pname), 1 * sizeof(GLfixed));
   int cmd_size = sizeof(struct marshal_cmd_Materialxv) + params_size;
   struct marshal_cmd_Materialxv *cmd;
   if (unlikely(params_size < 0 || (params_size > 0 && !params) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "Materialxv");
      CALL_Materialxv(ctx->CurrentServerDispatch, (face, pname, params));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Materialxv, cmd_size);
   cmd->face = face;
   cmd->pname = pname;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, params, params_size);
}


/* MultMatrixx: marshalled asynchronously */
struct marshal_cmd_MultMatrixx
{
   struct marshal_cmd_base cmd_base;
   GLfixed m[16];
};
void
_mesa_unmarshal_MultMatrixx(struct gl_context *ctx, const struct marshal_cmd_MultMatrixx *cmd)
{
   const GLfixed * m = cmd->m;
   CALL_MultMatrixx(ctx->CurrentServerDispatch, (m));
}
void GLAPIENTRY
_mesa_marshal_MultMatrixx(const GLfixed * m)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultMatrixx);
   struct marshal_cmd_MultMatrixx *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultMatrixx, cmd_size);
   memcpy(cmd->m, m, 16 * sizeof(GLfixed));
}


/* MultiTexCoord4x: marshalled asynchronously */
struct marshal_cmd_MultiTexCoord4x
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLfixed s;
   GLfixed t;
   GLfixed r;
   GLfixed q;
};
void
_mesa_unmarshal_MultiTexCoord4x(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord4x *cmd)
{
   const GLenum target = cmd->target;
   const GLfixed s = cmd->s;
   const GLfixed t = cmd->t;
   const GLfixed r = cmd->r;
   const GLfixed q = cmd->q;
   CALL_MultiTexCoord4x(ctx->CurrentServerDispatch, (target, s, t, r, q));
}
void GLAPIENTRY
_mesa_marshal_MultiTexCoord4x(GLenum target, GLfixed s, GLfixed t, GLfixed r, GLfixed q)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexCoord4x);
   struct marshal_cmd_MultiTexCoord4x *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexCoord4x, cmd_size);
   cmd->target = target;
   cmd->s = s;
   cmd->t = t;
   cmd->r = r;
   cmd->q = q;
}


/* Normal3x: marshalled asynchronously */
struct marshal_cmd_Normal3x
{
   struct marshal_cmd_base cmd_base;
   GLfixed nx;
   GLfixed ny;
   GLfixed nz;
};
void
_mesa_unmarshal_Normal3x(struct gl_context *ctx, const struct marshal_cmd_Normal3x *cmd)
{
   const GLfixed nx = cmd->nx;
   const GLfixed ny = cmd->ny;
   const GLfixed nz = cmd->nz;
   CALL_Normal3x(ctx->CurrentServerDispatch, (nx, ny, nz));
}
void GLAPIENTRY
_mesa_marshal_Normal3x(GLfixed nx, GLfixed ny, GLfixed nz)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Normal3x);
   struct marshal_cmd_Normal3x *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Normal3x, cmd_size);
   cmd->nx = nx;
   cmd->ny = ny;
   cmd->nz = nz;
}


/* Orthox: marshalled asynchronously */
struct marshal_cmd_Orthox
{
   struct marshal_cmd_base cmd_base;
   GLfixed left;
   GLfixed right;
   GLfixed bottom;
   GLfixed top;
   GLfixed zNear;
   GLfixed zFar;
};
void
_mesa_unmarshal_Orthox(struct gl_context *ctx, const struct marshal_cmd_Orthox *cmd)
{
   const GLfixed left = cmd->left;
   const GLfixed right = cmd->right;
   const GLfixed bottom = cmd->bottom;
   const GLfixed top = cmd->top;
   const GLfixed zNear = cmd->zNear;
   const GLfixed zFar = cmd->zFar;
   CALL_Orthox(ctx->CurrentServerDispatch, (left, right, bottom, top, zNear, zFar));
}
void GLAPIENTRY
_mesa_marshal_Orthox(GLfixed left, GLfixed right, GLfixed bottom, GLfixed top, GLfixed zNear, GLfixed zFar)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Orthox);
   struct marshal_cmd_Orthox *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Orthox, cmd_size);
   cmd->left = left;
   cmd->right = right;
   cmd->bottom = bottom;
   cmd->top = top;
   cmd->zNear = zNear;
   cmd->zFar = zFar;
}


/* PointSizex: marshalled asynchronously */
struct marshal_cmd_PointSizex
{
   struct marshal_cmd_base cmd_base;
   GLfixed size;
};
void
_mesa_unmarshal_PointSizex(struct gl_context *ctx, const struct marshal_cmd_PointSizex *cmd)
{
   const GLfixed size = cmd->size;
   CALL_PointSizex(ctx->CurrentServerDispatch, (size));
}
void GLAPIENTRY
_mesa_marshal_PointSizex(GLfixed size)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_PointSizex);
   struct marshal_cmd_PointSizex *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_PointSizex, cmd_size);
   cmd->size = size;
}


/* PolygonOffsetx: marshalled asynchronously */
struct marshal_cmd_PolygonOffsetx
{
   struct marshal_cmd_base cmd_base;
   GLfixed factor;
   GLfixed units;
};
void
_mesa_unmarshal_PolygonOffsetx(struct gl_context *ctx, const struct marshal_cmd_PolygonOffsetx *cmd)
{
   const GLfixed factor = cmd->factor;
   const GLfixed units = cmd->units;
   CALL_PolygonOffsetx(ctx->CurrentServerDispatch, (factor, units));
}
void GLAPIENTRY
_mesa_marshal_PolygonOffsetx(GLfixed factor, GLfixed units)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_PolygonOffsetx);
   struct marshal_cmd_PolygonOffsetx *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_PolygonOffsetx, cmd_size);
   cmd->factor = factor;
   cmd->units = units;
}


/* Rotatex: marshalled asynchronously */
struct marshal_cmd_Rotatex
{
   struct marshal_cmd_base cmd_base;
   GLfixed angle;
   GLfixed x;
   GLfixed y;
   GLfixed z;
};
void
_mesa_unmarshal_Rotatex(struct gl_context *ctx, const struct marshal_cmd_Rotatex *cmd)
{
   const GLfixed angle = cmd->angle;
   const GLfixed x = cmd->x;
   const GLfixed y = cmd->y;
   const GLfixed z = cmd->z;
   CALL_Rotatex(ctx->CurrentServerDispatch, (angle, x, y, z));
}
void GLAPIENTRY
_mesa_marshal_Rotatex(GLfixed angle, GLfixed x, GLfixed y, GLfixed z)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Rotatex);
   struct marshal_cmd_Rotatex *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Rotatex, cmd_size);
   cmd->angle = angle;
   cmd->x = x;
   cmd->y = y;
   cmd->z = z;
}


/* SampleCoveragex: marshalled asynchronously */
struct marshal_cmd_SampleCoveragex
{
   struct marshal_cmd_base cmd_base;
   GLboolean invert;
   GLclampx value;
};
void
_mesa_unmarshal_SampleCoveragex(struct gl_context *ctx, const struct marshal_cmd_SampleCoveragex *cmd)
{
   const GLclampx value = cmd->value;
   const GLboolean invert = cmd->invert;
   CALL_SampleCoveragex(ctx->CurrentServerDispatch, (value, invert));
}
void GLAPIENTRY
_mesa_marshal_SampleCoveragex(GLclampx value, GLboolean invert)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_SampleCoveragex);
   struct marshal_cmd_SampleCoveragex *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_SampleCoveragex, cmd_size);
   cmd->value = value;
   cmd->invert = invert;
}


/* Scalex: marshalled asynchronously */
struct marshal_cmd_Scalex
{
   struct marshal_cmd_base cmd_base;
   GLfixed x;
   GLfixed y;
   GLfixed z;
};
void
_mesa_unmarshal_Scalex(struct gl_context *ctx, const struct marshal_cmd_Scalex *cmd)
{
   const GLfixed x = cmd->x;
   const GLfixed y = cmd->y;
   const GLfixed z = cmd->z;
   CALL_Scalex(ctx->CurrentServerDispatch, (x, y, z));
}
void GLAPIENTRY
_mesa_marshal_Scalex(GLfixed x, GLfixed y, GLfixed z)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Scalex);
   struct marshal_cmd_Scalex *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Scalex, cmd_size);
   cmd->x = x;
   cmd->y = y;
   cmd->z = z;
}


/* TexEnvx: marshalled asynchronously */
struct marshal_cmd_TexEnvx
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLenum16 pname;
   GLfixed param;
};
void
_mesa_unmarshal_TexEnvx(struct gl_context *ctx, const struct marshal_cmd_TexEnvx *cmd)
{
   const GLenum target = cmd->target;
   const GLenum pname = cmd->pname;
   const GLfixed param = cmd->param;
   CALL_TexEnvx(ctx->CurrentServerDispatch, (target, pname, param));
}
void GLAPIENTRY
_mesa_marshal_TexEnvx(GLenum target, GLenum pname, GLfixed param)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TexEnvx);
   struct marshal_cmd_TexEnvx *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexEnvx, cmd_size);
   cmd->target = target;
   cmd->pname = pname;
   cmd->param = param;
}


/* TexEnvxv: marshalled asynchronously */
struct marshal_cmd_TexEnvxv
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLenum16 pname;
   /* Next safe_mul(_mesa_texenv_enum_to_count(pname), 1 * sizeof(GLfixed)) bytes are GLfixed params[None] */
};
void
_mesa_unmarshal_TexEnvxv(struct gl_context *ctx, const struct marshal_cmd_TexEnvxv *cmd)
{
   const GLenum target = cmd->target;
   const GLenum pname = cmd->pname;
   GLfixed * params;
   const char *variable_data = (const char *) (cmd + 1);
   params = (GLfixed *) variable_data;
   CALL_TexEnvxv(ctx->CurrentServerDispatch, (target, pname, params));
}
void GLAPIENTRY
_mesa_marshal_TexEnvxv(GLenum target, GLenum pname, const GLfixed * params)
{
   GET_CURRENT_CONTEXT(ctx);
   int params_size = safe_mul(_mesa_texenv_enum_to_count(pname), 1 * sizeof(GLfixed));
   int cmd_size = sizeof(struct marshal_cmd_TexEnvxv) + params_size;
   struct marshal_cmd_TexEnvxv *cmd;
   if (unlikely(params_size < 0 || (params_size > 0 && !params) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "TexEnvxv");
      CALL_TexEnvxv(ctx->CurrentServerDispatch, (target, pname, params));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexEnvxv, cmd_size);
   cmd->target = target;
   cmd->pname = pname;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, params, params_size);
}


/* TexParameterx: marshalled asynchronously */
struct marshal_cmd_TexParameterx
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLenum16 pname;
   GLfixed param;
};
void
_mesa_unmarshal_TexParameterx(struct gl_context *ctx, const struct marshal_cmd_TexParameterx *cmd)
{
   const GLenum target = cmd->target;
   const GLenum pname = cmd->pname;
   const GLfixed param = cmd->param;
   CALL_TexParameterx(ctx->CurrentServerDispatch, (target, pname, param));
}
void GLAPIENTRY
_mesa_marshal_TexParameterx(GLenum target, GLenum pname, GLfixed param)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TexParameterx);
   struct marshal_cmd_TexParameterx *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexParameterx, cmd_size);
   cmd->target = target;
   cmd->pname = pname;
   cmd->param = param;
}


/* Translatex: marshalled asynchronously */
struct marshal_cmd_Translatex
{
   struct marshal_cmd_base cmd_base;
   GLfixed x;
   GLfixed y;
   GLfixed z;
};
void
_mesa_unmarshal_Translatex(struct gl_context *ctx, const struct marshal_cmd_Translatex *cmd)
{
   const GLfixed x = cmd->x;
   const GLfixed y = cmd->y;
   const GLfixed z = cmd->z;
   CALL_Translatex(ctx->CurrentServerDispatch, (x, y, z));
}
void GLAPIENTRY
_mesa_marshal_Translatex(GLfixed x, GLfixed y, GLfixed z)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Translatex);
   struct marshal_cmd_Translatex *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Translatex, cmd_size);
   cmd->x = x;
   cmd->y = y;
   cmd->z = z;
}


/* ClipPlanex: marshalled asynchronously */
struct marshal_cmd_ClipPlanex
{
   struct marshal_cmd_base cmd_base;
   GLenum16 plane;
   GLfixed equation[4];
};
void
_mesa_unmarshal_ClipPlanex(struct gl_context *ctx, const struct marshal_cmd_ClipPlanex *cmd)
{
   const GLenum plane = cmd->plane;
   const GLfixed * equation = cmd->equation;
   CALL_ClipPlanex(ctx->CurrentServerDispatch, (plane, equation));
}
void GLAPIENTRY
_mesa_marshal_ClipPlanex(GLenum plane, const GLfixed * equation)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ClipPlanex);
   struct marshal_cmd_ClipPlanex *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ClipPlanex, cmd_size);
   cmd->plane = plane;
   memcpy(cmd->equation, equation, 4 * sizeof(GLfixed));
}


/* GetClipPlanex: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetClipPlanex(GLenum plane, GLfixed * equation)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetClipPlanex");
   CALL_GetClipPlanex(ctx->CurrentServerDispatch, (plane, equation));
}


/* GetFixedv: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetFixedv(GLenum pname, GLfixed * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetFixedv");
   CALL_GetFixedv(ctx->CurrentServerDispatch, (pname, params));
}


/* GetLightxv: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetLightxv(GLenum light, GLenum pname, GLfixed * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetLightxv");
   CALL_GetLightxv(ctx->CurrentServerDispatch, (light, pname, params));
}


/* GetMaterialxv: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetMaterialxv(GLenum face, GLenum pname, GLfixed * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetMaterialxv");
   CALL_GetMaterialxv(ctx->CurrentServerDispatch, (face, pname, params));
}


/* GetTexEnvxv: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetTexEnvxv(GLenum target, GLenum pname, GLfixed * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetTexEnvxv");
   CALL_GetTexEnvxv(ctx->CurrentServerDispatch, (target, pname, params));
}


/* GetTexParameterxv: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetTexParameterxv(GLenum target, GLenum pname, GLfixed * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetTexParameterxv");
   CALL_GetTexParameterxv(ctx->CurrentServerDispatch, (target, pname, params));
}


/* PointParameterx: marshalled asynchronously */
struct marshal_cmd_PointParameterx
{
   struct marshal_cmd_base cmd_base;
   GLenum16 pname;
   GLfixed param;
};
void
_mesa_unmarshal_PointParameterx(struct gl_context *ctx, const struct marshal_cmd_PointParameterx *cmd)
{
   const GLenum pname = cmd->pname;
   const GLfixed param = cmd->param;
   CALL_PointParameterx(ctx->CurrentServerDispatch, (pname, param));
}
void GLAPIENTRY
_mesa_marshal_PointParameterx(GLenum pname, GLfixed param)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_PointParameterx);
   struct marshal_cmd_PointParameterx *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_PointParameterx, cmd_size);
   cmd->pname = pname;
   cmd->param = param;
}


/* PointParameterxv: marshalled asynchronously */
struct marshal_cmd_PointParameterxv
{
   struct marshal_cmd_base cmd_base;
   GLenum16 pname;
   /* Next safe_mul(_mesa_point_param_enum_to_count(pname), 1 * sizeof(GLfixed)) bytes are GLfixed params[_mesa_point_param_enum_to_count(pname)] */
};
void
_mesa_unmarshal_PointParameterxv(struct gl_context *ctx, const struct marshal_cmd_PointParameterxv *cmd)
{
   const GLenum pname = cmd->pname;
   GLfixed * params;
   const char *variable_data = (const char *) (cmd + 1);
   params = (GLfixed *) variable_data;
   CALL_PointParameterxv(ctx->CurrentServerDispatch, (pname, params));
}
void GLAPIENTRY
_mesa_marshal_PointParameterxv(GLenum pname, const GLfixed * params)
{
   GET_CURRENT_CONTEXT(ctx);
   int params_size = safe_mul(_mesa_point_param_enum_to_count(pname), 1 * sizeof(GLfixed));
   int cmd_size = sizeof(struct marshal_cmd_PointParameterxv) + params_size;
   struct marshal_cmd_PointParameterxv *cmd;
   if (unlikely(params_size < 0 || (params_size > 0 && !params) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "PointParameterxv");
      CALL_PointParameterxv(ctx->CurrentServerDispatch, (pname, params));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_PointParameterxv, cmd_size);
   cmd->pname = pname;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, params, params_size);
}


/* TexParameterxv: marshalled asynchronously */
struct marshal_cmd_TexParameterxv
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLenum16 pname;
   /* Next safe_mul(_mesa_tex_param_enum_to_count(pname), 1 * sizeof(GLfixed)) bytes are GLfixed params[None] */
};
void
_mesa_unmarshal_TexParameterxv(struct gl_context *ctx, const struct marshal_cmd_TexParameterxv *cmd)
{
   const GLenum target = cmd->target;
   const GLenum pname = cmd->pname;
   GLfixed * params;
   const char *variable_data = (const char *) (cmd + 1);
   params = (GLfixed *) variable_data;
   CALL_TexParameterxv(ctx->CurrentServerDispatch, (target, pname, params));
}
void GLAPIENTRY
_mesa_marshal_TexParameterxv(GLenum target, GLenum pname, const GLfixed * params)
{
   GET_CURRENT_CONTEXT(ctx);
   int params_size = safe_mul(_mesa_tex_param_enum_to_count(pname), 1 * sizeof(GLfixed));
   int cmd_size = sizeof(struct marshal_cmd_TexParameterxv) + params_size;
   struct marshal_cmd_TexParameterxv *cmd;
   if (unlikely(params_size < 0 || (params_size > 0 && !params) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "TexParameterxv");
      CALL_TexParameterxv(ctx->CurrentServerDispatch, (target, pname, params));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexParameterxv, cmd_size);
   cmd->target = target;
   cmd->pname = pname;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, params, params_size);
}


/* GetTexGenxvOES: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetTexGenxvOES(GLenum coord, GLenum pname, GLfixed * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetTexGenxvOES");
   CALL_GetTexGenxvOES(ctx->CurrentServerDispatch, (coord, pname, params));
}


/* TexGenxOES: marshalled asynchronously */
struct marshal_cmd_TexGenxOES
{
   struct marshal_cmd_base cmd_base;
   GLenum16 coord;
   GLenum16 pname;
   GLint param;
};
void
_mesa_unmarshal_TexGenxOES(struct gl_context *ctx, const struct marshal_cmd_TexGenxOES *cmd)
{
   const GLenum coord = cmd->coord;
   const GLenum pname = cmd->pname;
   const GLint param = cmd->param;
   CALL_TexGenxOES(ctx->CurrentServerDispatch, (coord, pname, param));
}
void GLAPIENTRY
_mesa_marshal_TexGenxOES(GLenum coord, GLenum pname, GLint param)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TexGenxOES);
   struct marshal_cmd_TexGenxOES *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexGenxOES, cmd_size);
   cmd->coord = coord;
   cmd->pname = pname;
   cmd->param = param;
}


/* TexGenxvOES: marshalled asynchronously */
struct marshal_cmd_TexGenxvOES
{
   struct marshal_cmd_base cmd_base;
   GLenum16 coord;
   GLenum16 pname;
   /* Next safe_mul(_mesa_texgen_enum_to_count(pname), 1 * sizeof(GLfixed)) bytes are GLfixed params[None] */
};
void
_mesa_unmarshal_TexGenxvOES(struct gl_context *ctx, const struct marshal_cmd_TexGenxvOES *cmd)
{
   const GLenum coord = cmd->coord;
   const GLenum pname = cmd->pname;
   GLfixed * params;
   const char *variable_data = (const char *) (cmd + 1);
   params = (GLfixed *) variable_data;
   CALL_TexGenxvOES(ctx->CurrentServerDispatch, (coord, pname, params));
}
void GLAPIENTRY
_mesa_marshal_TexGenxvOES(GLenum coord, GLenum pname, const GLfixed * params)
{
   GET_CURRENT_CONTEXT(ctx);
   int params_size = safe_mul(_mesa_texgen_enum_to_count(pname), 1 * sizeof(GLfixed));
   int cmd_size = sizeof(struct marshal_cmd_TexGenxvOES) + params_size;
   struct marshal_cmd_TexGenxvOES *cmd;
   if (unlikely(params_size < 0 || (params_size > 0 && !params) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "TexGenxvOES");
      CALL_TexGenxvOES(ctx->CurrentServerDispatch, (coord, pname, params));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexGenxvOES, cmd_size);
   cmd->coord = coord;
   cmd->pname = pname;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, params, params_size);
}


/* ClipPlanef: marshalled asynchronously */
struct marshal_cmd_ClipPlanef
{
   struct marshal_cmd_base cmd_base;
   GLenum16 plane;
   GLfloat equation[4];
};
void
_mesa_unmarshal_ClipPlanef(struct gl_context *ctx, const struct marshal_cmd_ClipPlanef *cmd)
{
   const GLenum plane = cmd->plane;
   const GLfloat * equation = cmd->equation;
   CALL_ClipPlanef(ctx->CurrentServerDispatch, (plane, equation));
}
void GLAPIENTRY
_mesa_marshal_ClipPlanef(GLenum plane, const GLfloat * equation)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ClipPlanef);
   struct marshal_cmd_ClipPlanef *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ClipPlanef, cmd_size);
   cmd->plane = plane;
   memcpy(cmd->equation, equation, 4 * sizeof(GLfloat));
}


/* GetClipPlanef: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetClipPlanef(GLenum plane, GLfloat * equation)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetClipPlanef");
   CALL_GetClipPlanef(ctx->CurrentServerDispatch, (plane, equation));
}


/* Frustumf: marshalled asynchronously */
struct marshal_cmd_Frustumf
{
   struct marshal_cmd_base cmd_base;
   GLfloat left;
   GLfloat right;
   GLfloat bottom;
   GLfloat top;
   GLfloat zNear;
   GLfloat zFar;
};
void
_mesa_unmarshal_Frustumf(struct gl_context *ctx, const struct marshal_cmd_Frustumf *cmd)
{
   const GLfloat left = cmd->left;
   const GLfloat right = cmd->right;
   const GLfloat bottom = cmd->bottom;
   const GLfloat top = cmd->top;
   const GLfloat zNear = cmd->zNear;
   const GLfloat zFar = cmd->zFar;
   CALL_Frustumf(ctx->CurrentServerDispatch, (left, right, bottom, top, zNear, zFar));
}
void GLAPIENTRY
_mesa_marshal_Frustumf(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Frustumf);
   struct marshal_cmd_Frustumf *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Frustumf, cmd_size);
   cmd->left = left;
   cmd->right = right;
   cmd->bottom = bottom;
   cmd->top = top;
   cmd->zNear = zNear;
   cmd->zFar = zFar;
}


/* Orthof: marshalled asynchronously */
struct marshal_cmd_Orthof
{
   struct marshal_cmd_base cmd_base;
   GLfloat left;
   GLfloat right;
   GLfloat bottom;
   GLfloat top;
   GLfloat zNear;
   GLfloat zFar;
};
void
_mesa_unmarshal_Orthof(struct gl_context *ctx, const struct marshal_cmd_Orthof *cmd)
{
   const GLfloat left = cmd->left;
   const GLfloat right = cmd->right;
   const GLfloat bottom = cmd->bottom;
   const GLfloat top = cmd->top;
   const GLfloat zNear = cmd->zNear;
   const GLfloat zFar = cmd->zFar;
   CALL_Orthof(ctx->CurrentServerDispatch, (left, right, bottom, top, zNear, zFar));
}
void GLAPIENTRY
_mesa_marshal_Orthof(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Orthof);
   struct marshal_cmd_Orthof *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Orthof, cmd_size);
   cmd->left = left;
   cmd->right = right;
   cmd->bottom = bottom;
   cmd->top = top;
   cmd->zNear = zNear;
   cmd->zFar = zFar;
}


/* DrawTexiOES: marshalled asynchronously */
struct marshal_cmd_DrawTexiOES
{
   struct marshal_cmd_base cmd_base;
   GLint x;
   GLint y;
   GLint z;
   GLint width;
   GLint height;
};
void
_mesa_unmarshal_DrawTexiOES(struct gl_context *ctx, const struct marshal_cmd_DrawTexiOES *cmd)
{
   const GLint x = cmd->x;
   const GLint y = cmd->y;
   const GLint z = cmd->z;
   const GLint width = cmd->width;
   const GLint height = cmd->height;
   CALL_DrawTexiOES(ctx->CurrentServerDispatch, (x, y, z, width, height));
}
void GLAPIENTRY
_mesa_marshal_DrawTexiOES(GLint x, GLint y, GLint z, GLint width, GLint height)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_DrawTexiOES);
   struct marshal_cmd_DrawTexiOES *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_DrawTexiOES, cmd_size);
   cmd->x = x;
   cmd->y = y;
   cmd->z = z;
   cmd->width = width;
   cmd->height = height;
}


/* DrawTexivOES: marshalled asynchronously */
struct marshal_cmd_DrawTexivOES
{
   struct marshal_cmd_base cmd_base;
   GLint coords[5];
};
void
_mesa_unmarshal_DrawTexivOES(struct gl_context *ctx, const struct marshal_cmd_DrawTexivOES *cmd)
{
   const GLint * coords = cmd->coords;
   CALL_DrawTexivOES(ctx->CurrentServerDispatch, (coords));
}
void GLAPIENTRY
_mesa_marshal_DrawTexivOES(const GLint * coords)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_DrawTexivOES);
   struct marshal_cmd_DrawTexivOES *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_DrawTexivOES, cmd_size);
   memcpy(cmd->coords, coords, 5 * sizeof(GLint));
}


/* DrawTexfOES: marshalled asynchronously */
struct marshal_cmd_DrawTexfOES
{
   struct marshal_cmd_base cmd_base;
   GLfloat x;
   GLfloat y;
   GLfloat z;
   GLfloat width;
   GLfloat height;
};
void
_mesa_unmarshal_DrawTexfOES(struct gl_context *ctx, const struct marshal_cmd_DrawTexfOES *cmd)
{
   const GLfloat x = cmd->x;
   const GLfloat y = cmd->y;
   const GLfloat z = cmd->z;
   const GLfloat width = cmd->width;
   const GLfloat height = cmd->height;
   CALL_DrawTexfOES(ctx->CurrentServerDispatch, (x, y, z, width, height));
}
void GLAPIENTRY
_mesa_marshal_DrawTexfOES(GLfloat x, GLfloat y, GLfloat z, GLfloat width, GLfloat height)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_DrawTexfOES);
   struct marshal_cmd_DrawTexfOES *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_DrawTexfOES, cmd_size);
   cmd->x = x;
   cmd->y = y;
   cmd->z = z;
   cmd->width = width;
   cmd->height = height;
}


/* DrawTexfvOES: marshalled asynchronously */
struct marshal_cmd_DrawTexfvOES
{
   struct marshal_cmd_base cmd_base;
   GLfloat coords[5];
};
void
_mesa_unmarshal_DrawTexfvOES(struct gl_context *ctx, const struct marshal_cmd_DrawTexfvOES *cmd)
{
   const GLfloat * coords = cmd->coords;
   CALL_DrawTexfvOES(ctx->CurrentServerDispatch, (coords));
}
void GLAPIENTRY
_mesa_marshal_DrawTexfvOES(const GLfloat * coords)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_DrawTexfvOES);
   struct marshal_cmd_DrawTexfvOES *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_DrawTexfvOES, cmd_size);
   memcpy(cmd->coords, coords, 5 * sizeof(GLfloat));
}


/* DrawTexsOES: marshalled asynchronously */
struct marshal_cmd_DrawTexsOES
{
   struct marshal_cmd_base cmd_base;
   GLshort x;
   GLshort y;
   GLshort z;
   GLshort width;
   GLshort height;
};
void
_mesa_unmarshal_DrawTexsOES(struct gl_context *ctx, const struct marshal_cmd_DrawTexsOES *cmd)
{
   const GLshort x = cmd->x;
   const GLshort y = cmd->y;
   const GLshort z = cmd->z;
   const GLshort width = cmd->width;
   const GLshort height = cmd->height;
   CALL_DrawTexsOES(ctx->CurrentServerDispatch, (x, y, z, width, height));
}
void GLAPIENTRY
_mesa_marshal_DrawTexsOES(GLshort x, GLshort y, GLshort z, GLshort width, GLshort height)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_DrawTexsOES);
   struct marshal_cmd_DrawTexsOES *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_DrawTexsOES, cmd_size);
   cmd->x = x;
   cmd->y = y;
   cmd->z = z;
   cmd->width = width;
   cmd->height = height;
}


/* DrawTexsvOES: marshalled asynchronously */
struct marshal_cmd_DrawTexsvOES
{
   struct marshal_cmd_base cmd_base;
   GLshort coords[5];
};
void
_mesa_unmarshal_DrawTexsvOES(struct gl_context *ctx, const struct marshal_cmd_DrawTexsvOES *cmd)
{
   const GLshort * coords = cmd->coords;
   CALL_DrawTexsvOES(ctx->CurrentServerDispatch, (coords));
}
void GLAPIENTRY
_mesa_marshal_DrawTexsvOES(const GLshort * coords)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_DrawTexsvOES);
   struct marshal_cmd_DrawTexsvOES *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_DrawTexsvOES, cmd_size);
   memcpy(cmd->coords, coords, 5 * sizeof(GLshort));
}


/* DrawTexxOES: marshalled asynchronously */
struct marshal_cmd_DrawTexxOES
{
   struct marshal_cmd_base cmd_base;
   GLfixed x;
   GLfixed y;
   GLfixed z;
   GLfixed width;
   GLfixed height;
};
void
_mesa_unmarshal_DrawTexxOES(struct gl_context *ctx, const struct marshal_cmd_DrawTexxOES *cmd)
{
   const GLfixed x = cmd->x;
   const GLfixed y = cmd->y;
   const GLfixed z = cmd->z;
   const GLfixed width = cmd->width;
   const GLfixed height = cmd->height;
   CALL_DrawTexxOES(ctx->CurrentServerDispatch, (x, y, z, width, height));
}
void GLAPIENTRY
_mesa_marshal_DrawTexxOES(GLfixed x, GLfixed y, GLfixed z, GLfixed width, GLfixed height)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_DrawTexxOES);
   struct marshal_cmd_DrawTexxOES *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_DrawTexxOES, cmd_size);
   cmd->x = x;
   cmd->y = y;
   cmd->z = z;
   cmd->width = width;
   cmd->height = height;
}


/* DrawTexxvOES: marshalled asynchronously */
struct marshal_cmd_DrawTexxvOES
{
   struct marshal_cmd_base cmd_base;
   GLfixed coords[5];
};
void
_mesa_unmarshal_DrawTexxvOES(struct gl_context *ctx, const struct marshal_cmd_DrawTexxvOES *cmd)
{
   const GLfixed * coords = cmd->coords;
   CALL_DrawTexxvOES(ctx->CurrentServerDispatch, (coords));
}
void GLAPIENTRY
_mesa_marshal_DrawTexxvOES(const GLfixed * coords)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_DrawTexxvOES);
   struct marshal_cmd_DrawTexxvOES *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_DrawTexxvOES, cmd_size);
   memcpy(cmd->coords, coords, 5 * sizeof(GLfixed));
}


/* PointSizePointerOES: marshalled asynchronously */
struct marshal_cmd_PointSizePointerOES
{
   struct marshal_cmd_base cmd_base;
   GLenum16 type;
   GLsizei stride;
   const GLvoid * pointer;
};
void
_mesa_unmarshal_PointSizePointerOES(struct gl_context *ctx, const struct marshal_cmd_PointSizePointerOES *cmd)
{
   const GLenum type = cmd->type;
   const GLsizei stride = cmd->stride;
   const GLvoid * pointer = cmd->pointer;
   CALL_PointSizePointerOES(ctx->CurrentServerDispatch, (type, stride, pointer));
}
void GLAPIENTRY
_mesa_marshal_PointSizePointerOES(GLenum type, GLsizei stride, const GLvoid * pointer)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_PointSizePointerOES);
   struct marshal_cmd_PointSizePointerOES *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_PointSizePointerOES, cmd_size);
   cmd->type = type;
   cmd->stride = stride;
   cmd->pointer = pointer;
   if (COMPAT) _mesa_glthread_AttribPointer(ctx, VERT_ATTRIB_POINT_SIZE);
}


/* QueryMatrixxOES: marshalled synchronously */
GLbitfield GLAPIENTRY
_mesa_marshal_QueryMatrixxOES(GLfixed * mantissa, GLint * exponent)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "QueryMatrixxOES");
   return CALL_QueryMatrixxOES(ctx->CurrentServerDispatch, (mantissa, exponent));
}


/* DiscardFramebufferEXT: marshalled asynchronously */
struct marshal_cmd_DiscardFramebufferEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLsizei numAttachments;
   /* Next safe_mul(numAttachments, 1 * sizeof(GLenum)) bytes are GLenum attachments[numAttachments] */
};
void
_mesa_unmarshal_DiscardFramebufferEXT(struct gl_context *ctx, const struct marshal_cmd_DiscardFramebufferEXT *cmd)
{
   const GLenum target = cmd->target;
   const GLsizei numAttachments = cmd->numAttachments;
   GLenum * attachments;
   const char *variable_data = (const char *) (cmd + 1);
   attachments = (GLenum *) variable_data;
   CALL_DiscardFramebufferEXT(ctx->CurrentServerDispatch, (target, numAttachments, attachments));
}
void GLAPIENTRY
_mesa_marshal_DiscardFramebufferEXT(GLenum target, GLsizei numAttachments, const GLenum * attachments)
{
   GET_CURRENT_CONTEXT(ctx);
   int attachments_size = safe_mul(numAttachments, 1 * sizeof(GLenum));
   int cmd_size = sizeof(struct marshal_cmd_DiscardFramebufferEXT) + attachments_size;
   struct marshal_cmd_DiscardFramebufferEXT *cmd;
   if (unlikely(attachments_size < 0 || (attachments_size > 0 && !attachments) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "DiscardFramebufferEXT");
      CALL_DiscardFramebufferEXT(ctx->CurrentServerDispatch, (target, numAttachments, attachments));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_DiscardFramebufferEXT, cmd_size);
   cmd->target = target;
   cmd->numAttachments = numAttachments;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, attachments, attachments_size);
}


/* FramebufferTexture2DMultisampleEXT: marshalled asynchronously */
struct marshal_cmd_FramebufferTexture2DMultisampleEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLenum16 attachment;
   GLenum16 textarget;
   GLuint texture;
   GLint level;
   GLsizei samples;
};
void
_mesa_unmarshal_FramebufferTexture2DMultisampleEXT(struct gl_context *ctx, const struct marshal_cmd_FramebufferTexture2DMultisampleEXT *cmd)
{
   const GLenum target = cmd->target;
   const GLenum attachment = cmd->attachment;
   const GLenum textarget = cmd->textarget;
   const GLuint texture = cmd->texture;
   const GLint level = cmd->level;
   const GLsizei samples = cmd->samples;
   CALL_FramebufferTexture2DMultisampleEXT(ctx->CurrentServerDispatch, (target, attachment, textarget, texture, level, samples));
}
void GLAPIENTRY
_mesa_marshal_FramebufferTexture2DMultisampleEXT(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLsizei samples)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_FramebufferTexture2DMultisampleEXT);
   struct marshal_cmd_FramebufferTexture2DMultisampleEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_FramebufferTexture2DMultisampleEXT, cmd_size);
   cmd->target = target;
   cmd->attachment = attachment;
   cmd->textarget = textarget;
   cmd->texture = texture;
   cmd->level = level;
   cmd->samples = samples;
}


/* DepthRangeArrayfvOES: marshalled asynchronously */
struct marshal_cmd_DepthRangeArrayfvOES
{
   struct marshal_cmd_base cmd_base;
   GLuint first;
   GLsizei count;
   /* Next safe_mul((2 * count), 1 * sizeof(GLfloat)) bytes are GLfloat v[(2 * count)] */
};
void
_mesa_unmarshal_DepthRangeArrayfvOES(struct gl_context *ctx, const struct marshal_cmd_DepthRangeArrayfvOES *cmd)
{
   const GLuint first = cmd->first;
   const GLsizei count = cmd->count;
   GLfloat * v;
   const char *variable_data = (const char *) (cmd + 1);
   v = (GLfloat *) variable_data;
   CALL_DepthRangeArrayfvOES(ctx->CurrentServerDispatch, (first, count, v));
}
void GLAPIENTRY
_mesa_marshal_DepthRangeArrayfvOES(GLuint first, GLsizei count, const GLfloat * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int v_size = safe_mul((2 * count), 1 * sizeof(GLfloat));
   int cmd_size = sizeof(struct marshal_cmd_DepthRangeArrayfvOES) + v_size;
   struct marshal_cmd_DepthRangeArrayfvOES *cmd;
   if (unlikely(v_size < 0 || (v_size > 0 && !v) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "DepthRangeArrayfvOES");
      CALL_DepthRangeArrayfvOES(ctx->CurrentServerDispatch, (first, count, v));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_DepthRangeArrayfvOES, cmd_size);
   cmd->first = first;
   cmd->count = count;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, v, v_size);
}


/* DepthRangeIndexedfOES: marshalled asynchronously */
struct marshal_cmd_DepthRangeIndexedfOES
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLfloat n;
   GLfloat f;
};
void
_mesa_unmarshal_DepthRangeIndexedfOES(struct gl_context *ctx, const struct marshal_cmd_DepthRangeIndexedfOES *cmd)
{
   const GLuint index = cmd->index;
   const GLfloat n = cmd->n;
   const GLfloat f = cmd->f;
   CALL_DepthRangeIndexedfOES(ctx->CurrentServerDispatch, (index, n, f));
}
void GLAPIENTRY
_mesa_marshal_DepthRangeIndexedfOES(GLuint index, GLfloat n, GLfloat f)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_DepthRangeIndexedfOES);
   struct marshal_cmd_DepthRangeIndexedfOES *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_DepthRangeIndexedfOES, cmd_size);
   cmd->index = index;
   cmd->n = n;
   cmd->f = f;
}


/* FramebufferParameteriMESA: marshalled asynchronously */
struct marshal_cmd_FramebufferParameteriMESA
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLenum16 pname;
   GLint param;
};
void
_mesa_unmarshal_FramebufferParameteriMESA(struct gl_context *ctx, const struct marshal_cmd_FramebufferParameteriMESA *cmd)
{
   const GLenum target = cmd->target;
   const GLenum pname = cmd->pname;
   const GLint param = cmd->param;
   CALL_FramebufferParameteriMESA(ctx->CurrentServerDispatch, (target, pname, param));
}
void GLAPIENTRY
_mesa_marshal_FramebufferParameteriMESA(GLenum target, GLenum pname, GLint param)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_FramebufferParameteriMESA);
   struct marshal_cmd_FramebufferParameteriMESA *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_FramebufferParameteriMESA, cmd_size);
   cmd->target = target;
   cmd->pname = pname;
   cmd->param = param;
}


/* GetFramebufferParameterivMESA: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetFramebufferParameterivMESA(GLenum target, GLenum pname, GLint * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetFramebufferParameterivMESA");
   CALL_GetFramebufferParameterivMESA(ctx->CurrentServerDispatch, (target, pname, params));
}


