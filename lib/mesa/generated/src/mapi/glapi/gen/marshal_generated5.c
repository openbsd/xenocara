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

/* BufferPageCommitmentARB: marshalled asynchronously */
struct marshal_cmd_BufferPageCommitmentARB
{
   struct marshal_cmd_base cmd_base;
   GLboolean commit;
   GLenum target;
   GLintptr offset;
   GLsizeiptr size;
};
void
_mesa_unmarshal_BufferPageCommitmentARB(struct gl_context *ctx, const struct marshal_cmd_BufferPageCommitmentARB *cmd)
{
   GLenum target = cmd->target;
   GLintptr offset = cmd->offset;
   GLsizeiptr size = cmd->size;
   GLboolean commit = cmd->commit;
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
   GLuint buffer = cmd->buffer;
   GLintptr offset = cmd->offset;
   GLsizeiptr size = cmd->size;
   GLboolean commit = cmd->commit;
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
   GLuint buffer = cmd->buffer;
   GLintptr offset = cmd->offset;
   GLsizeiptr size = cmd->size;
   GLboolean commit = cmd->commit;
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
   GLfloat minX = cmd->minX;
   GLfloat minY = cmd->minY;
   GLfloat minZ = cmd->minZ;
   GLfloat minW = cmd->minW;
   GLfloat maxX = cmd->maxX;
   GLfloat maxY = cmd->maxY;
   GLfloat maxZ = cmd->maxZ;
   GLfloat maxW = cmd->maxW;
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
   GLint location = cmd->location;
   GLint64 x = cmd->x;
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
   GLint location = cmd->location;
   GLint64 x = cmd->x;
   GLint64 y = cmd->y;
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
   GLint location = cmd->location;
   GLint64 x = cmd->x;
   GLint64 y = cmd->y;
   GLint64 z = cmd->z;
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
   GLint location = cmd->location;
   GLint64 x = cmd->x;
   GLint64 y = cmd->y;
   GLint64 z = cmd->z;
   GLint64 w = cmd->w;
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
   GLint location = cmd->location;
   GLsizei count = cmd->count;
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
   GLint location = cmd->location;
   GLsizei count = cmd->count;
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
   GLint location = cmd->location;
   GLsizei count = cmd->count;
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
   GLint location = cmd->location;
   GLsizei count = cmd->count;
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
   GLint location = cmd->location;
   GLuint64 x = cmd->x;
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
   GLint location = cmd->location;
   GLuint64 x = cmd->x;
   GLuint64 y = cmd->y;
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
   GLint location = cmd->location;
   GLuint64 x = cmd->x;
   GLuint64 y = cmd->y;
   GLuint64 z = cmd->z;
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
   GLint location = cmd->location;
   GLuint64 x = cmd->x;
   GLuint64 y = cmd->y;
   GLuint64 z = cmd->z;
   GLuint64 w = cmd->w;
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
   GLint location = cmd->location;
   GLsizei count = cmd->count;
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
   GLint location = cmd->location;
   GLsizei count = cmd->count;
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
   GLint location = cmd->location;
   GLsizei count = cmd->count;
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
   GLint location = cmd->location;
   GLsizei count = cmd->count;
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
   GLuint program = cmd->program;
   GLint location = cmd->location;
   GLint64 x = cmd->x;
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
   GLuint program = cmd->program;
   GLint location = cmd->location;
   GLint64 x = cmd->x;
   GLint64 y = cmd->y;
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
   GLuint program = cmd->program;
   GLint location = cmd->location;
   GLint64 x = cmd->x;
   GLint64 y = cmd->y;
   GLint64 z = cmd->z;
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
   GLuint program = cmd->program;
   GLint location = cmd->location;
   GLint64 x = cmd->x;
   GLint64 y = cmd->y;
   GLint64 z = cmd->z;
   GLint64 w = cmd->w;
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
   GLuint program = cmd->program;
   GLint location = cmd->location;
   GLsizei count = cmd->count;
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
   GLuint program = cmd->program;
   GLint location = cmd->location;
   GLsizei count = cmd->count;
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
   GLuint program = cmd->program;
   GLint location = cmd->location;
   GLsizei count = cmd->count;
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
   GLuint program = cmd->program;
   GLint location = cmd->location;
   GLsizei count = cmd->count;
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
   GLuint program = cmd->program;
   GLint location = cmd->location;
   GLuint64 x = cmd->x;
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
   GLuint program = cmd->program;
   GLint location = cmd->location;
   GLuint64 x = cmd->x;
   GLuint64 y = cmd->y;
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
   GLuint program = cmd->program;
   GLint location = cmd->location;
   GLuint64 x = cmd->x;
   GLuint64 y = cmd->y;
   GLuint64 z = cmd->z;
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
   GLuint program = cmd->program;
   GLint location = cmd->location;
   GLuint64 x = cmd->x;
   GLuint64 y = cmd->y;
   GLuint64 z = cmd->z;
   GLuint64 w = cmd->w;
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
   GLuint program = cmd->program;
   GLint location = cmd->location;
   GLsizei count = cmd->count;
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
   GLuint program = cmd->program;
   GLint location = cmd->location;
   GLsizei count = cmd->count;
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
   GLuint program = cmd->program;
   GLint location = cmd->location;
   GLsizei count = cmd->count;
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
   GLuint program = cmd->program;
   GLint location = cmd->location;
   GLsizei count = cmd->count;
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
   GLuint count = cmd->count;
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
   GLuint shader = cmd->shader;
   GLuint numSpecializationConstants = cmd->numSpecializationConstants;
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
   GLint size;
   GLenum type;
   GLsizei stride;
   GLsizei count;
   const GLvoid * pointer;
};
void
_mesa_unmarshal_ColorPointerEXT(struct gl_context *ctx, const struct marshal_cmd_ColorPointerEXT *cmd)
{
   GLint size = cmd->size;
   GLenum type = cmd->type;
   GLsizei stride = cmd->stride;
   GLsizei count = cmd->count;
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
   if (COMPAT) _mesa_glthread_AttribPointer(ctx, VERT_ATTRIB_COLOR0, size, type, stride, pointer);
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
   GLsizei stride = cmd->stride;
   GLsizei count = cmd->count;
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
   if (COMPAT) _mesa_glthread_AttribPointer(ctx, VERT_ATTRIB_EDGEFLAG, 1, GL_UNSIGNED_BYTE, stride, pointer);
}


/* IndexPointerEXT: marshalled asynchronously */
struct marshal_cmd_IndexPointerEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum type;
   GLsizei stride;
   GLsizei count;
   const GLvoid * pointer;
};
void
_mesa_unmarshal_IndexPointerEXT(struct gl_context *ctx, const struct marshal_cmd_IndexPointerEXT *cmd)
{
   GLenum type = cmd->type;
   GLsizei stride = cmd->stride;
   GLsizei count = cmd->count;
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
   if (COMPAT) _mesa_glthread_AttribPointer(ctx, VERT_ATTRIB_COLOR_INDEX, 1, type, stride, pointer);
}


/* NormalPointerEXT: marshalled asynchronously */
struct marshal_cmd_NormalPointerEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum type;
   GLsizei stride;
   GLsizei count;
   const GLvoid * pointer;
};
void
_mesa_unmarshal_NormalPointerEXT(struct gl_context *ctx, const struct marshal_cmd_NormalPointerEXT *cmd)
{
   GLenum type = cmd->type;
   GLsizei stride = cmd->stride;
   GLsizei count = cmd->count;
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
   if (COMPAT) _mesa_glthread_AttribPointer(ctx, VERT_ATTRIB_NORMAL, 3, type, stride, pointer);
}


/* TexCoordPointerEXT: marshalled asynchronously */
struct marshal_cmd_TexCoordPointerEXT
{
   struct marshal_cmd_base cmd_base;
   GLint size;
   GLenum type;
   GLsizei stride;
   GLsizei count;
   const GLvoid * pointer;
};
void
_mesa_unmarshal_TexCoordPointerEXT(struct gl_context *ctx, const struct marshal_cmd_TexCoordPointerEXT *cmd)
{
   GLint size = cmd->size;
   GLenum type = cmd->type;
   GLsizei stride = cmd->stride;
   GLsizei count = cmd->count;
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
   if (COMPAT) _mesa_glthread_AttribPointer(ctx, VERT_ATTRIB_TEX(ctx->GLThread.ClientActiveTexture), size, type, stride, pointer);
}


/* VertexPointerEXT: marshalled asynchronously */
struct marshal_cmd_VertexPointerEXT
{
   struct marshal_cmd_base cmd_base;
   GLint size;
   GLenum type;
   GLsizei stride;
   GLsizei count;
   const GLvoid * pointer;
};
void
_mesa_unmarshal_VertexPointerEXT(struct gl_context *ctx, const struct marshal_cmd_VertexPointerEXT *cmd)
{
   GLint size = cmd->size;
   GLenum type = cmd->type;
   GLsizei stride = cmd->stride;
   GLsizei count = cmd->count;
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
   if (COMPAT) _mesa_glthread_AttribPointer(ctx, VERT_ATTRIB_POS, size, type, stride, pointer);
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
   GLint first = cmd->first;
   GLsizei count = cmd->count;
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
   GLuint first = cmd->first;
   GLsizei count = cmd->count;
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
   GLuint index = cmd->index;
   GLfloat x = cmd->x;
   GLfloat y = cmd->y;
   GLfloat w = cmd->w;
   GLfloat h = cmd->h;
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
   GLuint index = cmd->index;
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
   GLuint first = cmd->first;
   GLsizei count = cmd->count;
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
   GLuint index = cmd->index;
   GLint left = cmd->left;
   GLint bottom = cmd->bottom;
   GLsizei width = cmd->width;
   GLsizei height = cmd->height;
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
   GLuint index = cmd->index;
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
   GLuint first = cmd->first;
   GLsizei count = cmd->count;
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
   GLuint index = cmd->index;
   GLclampd n = cmd->n;
   GLclampd f = cmd->f;
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
   GLenum target;
   GLuint start;
   GLsizei count;
   /* Next safe_mul((2 * count), 1 * sizeof(GLfloat)) bytes are GLfloat v[(2 * count)] */
};
void
_mesa_unmarshal_FramebufferSampleLocationsfvARB(struct gl_context *ctx, const struct marshal_cmd_FramebufferSampleLocationsfvARB *cmd)
{
   GLenum target = cmd->target;
   GLuint start = cmd->start;
   GLsizei count = cmd->count;
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
   GLuint framebuffer = cmd->framebuffer;
   GLuint start = cmd->start;
   GLsizei count = cmd->count;
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


/* WindowPos4dMESA: marshalled asynchronously */
struct marshal_cmd_WindowPos4dMESA
{
   struct marshal_cmd_base cmd_base;
   GLdouble x;
   GLdouble y;
   GLdouble z;
   GLdouble w;
};
void
_mesa_unmarshal_WindowPos4dMESA(struct gl_context *ctx, const struct marshal_cmd_WindowPos4dMESA *cmd)
{
   GLdouble x = cmd->x;
   GLdouble y = cmd->y;
   GLdouble z = cmd->z;
   GLdouble w = cmd->w;
   CALL_WindowPos4dMESA(ctx->CurrentServerDispatch, (x, y, z, w));
}
void GLAPIENTRY
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
void
_mesa_unmarshal_WindowPos4dvMESA(struct gl_context *ctx, const struct marshal_cmd_WindowPos4dvMESA *cmd)
{
   const GLdouble * v = cmd->v;
   CALL_WindowPos4dvMESA(ctx->CurrentServerDispatch, (v));
}
void GLAPIENTRY
_mesa_marshal_WindowPos4dvMESA(const GLdouble * v)
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
void
_mesa_unmarshal_WindowPos4fMESA(struct gl_context *ctx, const struct marshal_cmd_WindowPos4fMESA *cmd)
{
   GLfloat x = cmd->x;
   GLfloat y = cmd->y;
   GLfloat z = cmd->z;
   GLfloat w = cmd->w;
   CALL_WindowPos4fMESA(ctx->CurrentServerDispatch, (x, y, z, w));
}
void GLAPIENTRY
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
void
_mesa_unmarshal_WindowPos4fvMESA(struct gl_context *ctx, const struct marshal_cmd_WindowPos4fvMESA *cmd)
{
   const GLfloat * v = cmd->v;
   CALL_WindowPos4fvMESA(ctx->CurrentServerDispatch, (v));
}
void GLAPIENTRY
_mesa_marshal_WindowPos4fvMESA(const GLfloat * v)
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
void
_mesa_unmarshal_WindowPos4iMESA(struct gl_context *ctx, const struct marshal_cmd_WindowPos4iMESA *cmd)
{
   GLint x = cmd->x;
   GLint y = cmd->y;
   GLint z = cmd->z;
   GLint w = cmd->w;
   CALL_WindowPos4iMESA(ctx->CurrentServerDispatch, (x, y, z, w));
}
void GLAPIENTRY
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
void
_mesa_unmarshal_WindowPos4ivMESA(struct gl_context *ctx, const struct marshal_cmd_WindowPos4ivMESA *cmd)
{
   const GLint * v = cmd->v;
   CALL_WindowPos4ivMESA(ctx->CurrentServerDispatch, (v));
}
void GLAPIENTRY
_mesa_marshal_WindowPos4ivMESA(const GLint * v)
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
void
_mesa_unmarshal_WindowPos4sMESA(struct gl_context *ctx, const struct marshal_cmd_WindowPos4sMESA *cmd)
{
   GLshort x = cmd->x;
   GLshort y = cmd->y;
   GLshort z = cmd->z;
   GLshort w = cmd->w;
   CALL_WindowPos4sMESA(ctx->CurrentServerDispatch, (x, y, z, w));
}
void GLAPIENTRY
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
void
_mesa_unmarshal_WindowPos4svMESA(struct gl_context *ctx, const struct marshal_cmd_WindowPos4svMESA *cmd)
{
   const GLshort * v = cmd->v;
   CALL_WindowPos4svMESA(ctx->CurrentServerDispatch, (v));
}
void GLAPIENTRY
_mesa_marshal_WindowPos4svMESA(const GLshort * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_WindowPos4svMESA);
   struct marshal_cmd_WindowPos4svMESA *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_WindowPos4svMESA, cmd_size);
   memcpy(cmd->v, v, 4 * sizeof(GLshort));
}


/* MultiModeDrawArraysIBM: marshalled asynchronously */
struct marshal_cmd_MultiModeDrawArraysIBM
{
   struct marshal_cmd_base cmd_base;
   GLsizei primcount;
   GLint modestride;
   const GLenum * mode;
   const GLint * first;
   const GLsizei * count;
};
void
_mesa_unmarshal_MultiModeDrawArraysIBM(struct gl_context *ctx, const struct marshal_cmd_MultiModeDrawArraysIBM *cmd)
{
   GLsizei primcount = cmd->primcount;
   GLint modestride = cmd->modestride;
   const GLenum * mode = cmd->mode;
   const GLint * first = cmd->first;
   const GLsizei * count = cmd->count;
   CALL_MultiModeDrawArraysIBM(ctx->CurrentServerDispatch, (mode, first, count, primcount, modestride));
}
void GLAPIENTRY
_mesa_marshal_MultiModeDrawArraysIBM(const GLenum * mode, const GLint * first, const GLsizei * count, GLsizei primcount, GLint modestride)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiModeDrawArraysIBM);
   struct marshal_cmd_MultiModeDrawArraysIBM *cmd;
   if (_mesa_glthread_has_non_vbo_vertices(ctx)) {
      _mesa_glthread_finish_before(ctx, "MultiModeDrawArraysIBM");
      CALL_MultiModeDrawArraysIBM(ctx->CurrentServerDispatch, (mode, first, count, primcount, modestride));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiModeDrawArraysIBM, cmd_size);
   cmd->primcount = primcount;
   cmd->modestride = modestride;
   cmd->mode = mode;
   cmd->first = first;
   cmd->count = count;
}


/* MultiModeDrawElementsIBM: marshalled asynchronously */
struct marshal_cmd_MultiModeDrawElementsIBM
{
   struct marshal_cmd_base cmd_base;
   GLenum type;
   GLsizei primcount;
   GLint modestride;
   const GLenum * mode;
   const GLsizei * count;
   const GLvoid * const * indices;
};
void
_mesa_unmarshal_MultiModeDrawElementsIBM(struct gl_context *ctx, const struct marshal_cmd_MultiModeDrawElementsIBM *cmd)
{
   GLenum type = cmd->type;
   GLsizei primcount = cmd->primcount;
   GLint modestride = cmd->modestride;
   const GLenum * mode = cmd->mode;
   const GLsizei * count = cmd->count;
   const GLvoid * const * indices = cmd->indices;
   CALL_MultiModeDrawElementsIBM(ctx->CurrentServerDispatch, (mode, count, type, indices, primcount, modestride));
}
void GLAPIENTRY
_mesa_marshal_MultiModeDrawElementsIBM(const GLenum * mode, const GLsizei * count, GLenum type, const GLvoid * const * indices, GLsizei primcount, GLint modestride)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiModeDrawElementsIBM);
   struct marshal_cmd_MultiModeDrawElementsIBM *cmd;
   if (_mesa_glthread_has_non_vbo_vertices_or_indices(ctx)) {
      _mesa_glthread_finish_before(ctx, "MultiModeDrawElementsIBM");
      CALL_MultiModeDrawElementsIBM(ctx->CurrentServerDispatch, (mode, count, type, indices, primcount, modestride));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiModeDrawElementsIBM, cmd_size);
   cmd->type = type;
   cmd->primcount = primcount;
   cmd->modestride = modestride;
   cmd->mode = mode;
   cmd->count = count;
   cmd->indices = indices;
}


