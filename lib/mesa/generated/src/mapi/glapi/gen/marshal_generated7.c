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

/* Uniform2ui: marshalled asynchronously */
struct marshal_cmd_Uniform2ui
{
   struct marshal_cmd_base cmd_base;
   GLint location;
   GLuint x;
   GLuint y;
};
uint32_t _mesa_unmarshal_Uniform2ui(struct gl_context *ctx, const struct marshal_cmd_Uniform2ui *restrict cmd)
{
   GLint location = cmd->location;
   GLuint x = cmd->x;
   GLuint y = cmd->y;
   CALL_Uniform2ui(ctx->Dispatch.Current, (location, x, y));
   return align(sizeof(struct marshal_cmd_Uniform2ui), 8) / 8;
}
static void GLAPIENTRY
_mesa_marshal_Uniform2ui(GLint location, GLuint x, GLuint y)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Uniform2ui);
   struct marshal_cmd_Uniform2ui *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Uniform2ui, cmd_size);
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
uint32_t _mesa_unmarshal_Uniform3ui(struct gl_context *ctx, const struct marshal_cmd_Uniform3ui *restrict cmd)
{
   GLint location = cmd->location;
   GLuint x = cmd->x;
   GLuint y = cmd->y;
   GLuint z = cmd->z;
   CALL_Uniform3ui(ctx->Dispatch.Current, (location, x, y, z));
   return align(sizeof(struct marshal_cmd_Uniform3ui), 8) / 8;
}
static void GLAPIENTRY
_mesa_marshal_Uniform3ui(GLint location, GLuint x, GLuint y, GLuint z)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Uniform3ui);
   struct marshal_cmd_Uniform3ui *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Uniform3ui, cmd_size);
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
uint32_t _mesa_unmarshal_Uniform4ui(struct gl_context *ctx, const struct marshal_cmd_Uniform4ui *restrict cmd)
{
   GLint location = cmd->location;
   GLuint x = cmd->x;
   GLuint y = cmd->y;
   GLuint z = cmd->z;
   GLuint w = cmd->w;
   CALL_Uniform4ui(ctx->Dispatch.Current, (location, x, y, z, w));
   return align(sizeof(struct marshal_cmd_Uniform4ui), 8) / 8;
}
static void GLAPIENTRY
_mesa_marshal_Uniform4ui(GLint location, GLuint x, GLuint y, GLuint z, GLuint w)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Uniform4ui);
   struct marshal_cmd_Uniform4ui *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Uniform4ui, cmd_size);
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
   uint16_t num_slots;
   GLint location;
   GLsizei count;
   /* Next safe_mul(count, 1 * sizeof(GLuint)) bytes are GLuint value[count] */
};
uint32_t _mesa_unmarshal_Uniform1uiv(struct gl_context *ctx, const struct marshal_cmd_Uniform1uiv *restrict cmd)
{
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLuint *value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLuint *) variable_data;
   CALL_Uniform1uiv(ctx->Dispatch.Current, (location, count, value));
   return cmd->num_slots;
}
static void GLAPIENTRY
_mesa_marshal_Uniform1uiv(GLint location, GLsizei count, const GLuint *value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 1 * sizeof(GLuint));
   int cmd_size = sizeof(struct marshal_cmd_Uniform1uiv) + value_size;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "Uniform1uiv");
      CALL_Uniform1uiv(ctx->Dispatch.Current, (location, count, value));
      return;
   }
   struct marshal_cmd_Uniform1uiv *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Uniform1uiv, cmd_size);
   cmd->num_slots = align(cmd_size, 8) / 8;
   cmd->location = location;
   cmd->count = count;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, value, value_size);
}


/* Uniform2uiv: marshalled asynchronously */
struct marshal_cmd_Uniform2uiv
{
   struct marshal_cmd_base cmd_base;
   uint16_t num_slots;
   GLint location;
   GLsizei count;
   /* Next safe_mul(count, 2 * sizeof(GLuint)) bytes are GLuint value[count][2] */
};
uint32_t _mesa_unmarshal_Uniform2uiv(struct gl_context *ctx, const struct marshal_cmd_Uniform2uiv *restrict cmd)
{
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLuint *value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLuint *) variable_data;
   CALL_Uniform2uiv(ctx->Dispatch.Current, (location, count, value));
   return cmd->num_slots;
}
static void GLAPIENTRY
_mesa_marshal_Uniform2uiv(GLint location, GLsizei count, const GLuint *value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 2 * sizeof(GLuint));
   int cmd_size = sizeof(struct marshal_cmd_Uniform2uiv) + value_size;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "Uniform2uiv");
      CALL_Uniform2uiv(ctx->Dispatch.Current, (location, count, value));
      return;
   }
   struct marshal_cmd_Uniform2uiv *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Uniform2uiv, cmd_size);
   cmd->num_slots = align(cmd_size, 8) / 8;
   cmd->location = location;
   cmd->count = count;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, value, value_size);
}


/* Uniform3uiv: marshalled asynchronously */
struct marshal_cmd_Uniform3uiv
{
   struct marshal_cmd_base cmd_base;
   uint16_t num_slots;
   GLint location;
   GLsizei count;
   /* Next safe_mul(count, 3 * sizeof(GLuint)) bytes are GLuint value[count][3] */
};
uint32_t _mesa_unmarshal_Uniform3uiv(struct gl_context *ctx, const struct marshal_cmd_Uniform3uiv *restrict cmd)
{
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLuint *value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLuint *) variable_data;
   CALL_Uniform3uiv(ctx->Dispatch.Current, (location, count, value));
   return cmd->num_slots;
}
static void GLAPIENTRY
_mesa_marshal_Uniform3uiv(GLint location, GLsizei count, const GLuint *value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 3 * sizeof(GLuint));
   int cmd_size = sizeof(struct marshal_cmd_Uniform3uiv) + value_size;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "Uniform3uiv");
      CALL_Uniform3uiv(ctx->Dispatch.Current, (location, count, value));
      return;
   }
   struct marshal_cmd_Uniform3uiv *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Uniform3uiv, cmd_size);
   cmd->num_slots = align(cmd_size, 8) / 8;
   cmd->location = location;
   cmd->count = count;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, value, value_size);
}


/* Uniform4uiv: marshalled asynchronously */
struct marshal_cmd_Uniform4uiv
{
   struct marshal_cmd_base cmd_base;
   uint16_t num_slots;
   GLint location;
   GLsizei count;
   /* Next safe_mul(count, 4 * sizeof(GLuint)) bytes are GLuint value[count][4] */
};
uint32_t _mesa_unmarshal_Uniform4uiv(struct gl_context *ctx, const struct marshal_cmd_Uniform4uiv *restrict cmd)
{
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLuint *value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLuint *) variable_data;
   CALL_Uniform4uiv(ctx->Dispatch.Current, (location, count, value));
   return cmd->num_slots;
}
static void GLAPIENTRY
_mesa_marshal_Uniform4uiv(GLint location, GLsizei count, const GLuint *value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 4 * sizeof(GLuint));
   int cmd_size = sizeof(struct marshal_cmd_Uniform4uiv) + value_size;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "Uniform4uiv");
      CALL_Uniform4uiv(ctx->Dispatch.Current, (location, count, value));
      return;
   }
   struct marshal_cmd_Uniform4uiv *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Uniform4uiv, cmd_size);
   cmd->num_slots = align(cmd_size, 8) / 8;
   cmd->location = location;
   cmd->count = count;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, value, value_size);
}


/* GetUniformuiv: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetUniformuiv(GLuint program, GLint location, GLuint *params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetUniformuiv");
   CALL_GetUniformuiv(ctx->Dispatch.Current, (program, location, params));
}


/* BindFragDataLocation: marshalled asynchronously */
struct marshal_cmd_BindFragDataLocation
{
   struct marshal_cmd_base cmd_base;
   uint16_t num_slots;
   GLuint program;
   GLuint colorNumber;
   /* Next (strlen(name) + 1) bytes are GLchar name[(strlen(name) + 1)] */
};
uint32_t _mesa_unmarshal_BindFragDataLocation(struct gl_context *ctx, const struct marshal_cmd_BindFragDataLocation *restrict cmd)
{
   GLuint program = cmd->program;
   GLuint colorNumber = cmd->colorNumber;
   GLchar *name;
   const char *variable_data = (const char *) (cmd + 1);
   name = (GLchar *) variable_data;
   CALL_BindFragDataLocation(ctx->Dispatch.Current, (program, colorNumber, name));
   return cmd->num_slots;
}
static void GLAPIENTRY
_mesa_marshal_BindFragDataLocation(GLuint program, GLuint colorNumber, const GLchar *name)
{
   GET_CURRENT_CONTEXT(ctx);
   int name_size = (strlen(name) + 1);
   int cmd_size = sizeof(struct marshal_cmd_BindFragDataLocation) + name_size;
   if (unlikely(name_size < 0 || (name_size > 0 && !name) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "BindFragDataLocation");
      CALL_BindFragDataLocation(ctx->Dispatch.Current, (program, colorNumber, name));
      return;
   }
   struct marshal_cmd_BindFragDataLocation *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_BindFragDataLocation, cmd_size);
   cmd->num_slots = align(cmd_size, 8) / 8;
   cmd->program = program;
   cmd->colorNumber = colorNumber;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, name, name_size);
}


/* GetFragDataLocation: marshalled synchronously */
static GLint GLAPIENTRY
_mesa_marshal_GetFragDataLocation(GLuint program, const GLchar *name)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetFragDataLocation");
   return CALL_GetFragDataLocation(ctx->Dispatch.Current, (program, name));
}


/* ClearBufferiv: marshalled asynchronously */
struct marshal_cmd_ClearBufferiv
{
   struct marshal_cmd_base cmd_base;
   uint16_t num_slots;
   GLenum16 buffer;
   GLint drawbuffer;
   /* Next _mesa_buffer_enum_to_count(buffer) * 1 * sizeof(GLint) bytes are GLint value[None] */
};
uint32_t _mesa_unmarshal_ClearBufferiv(struct gl_context *ctx, const struct marshal_cmd_ClearBufferiv *restrict cmd)
{
   GLenum16 buffer = cmd->buffer;
   GLint drawbuffer = cmd->drawbuffer;
   GLint *value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLint *) variable_data;
   CALL_ClearBufferiv(ctx->Dispatch.Current, (buffer, drawbuffer, value));
   return cmd->num_slots;
}
static void GLAPIENTRY
_mesa_marshal_ClearBufferiv(GLenum buffer, GLint drawbuffer, const GLint *value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = _mesa_buffer_enum_to_count(buffer) * 1 * sizeof(GLint);
   int cmd_size = sizeof(struct marshal_cmd_ClearBufferiv) + value_size;
   assert(cmd_size >= 0 && cmd_size <= MARSHAL_MAX_CMD_SIZE);
   struct marshal_cmd_ClearBufferiv *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ClearBufferiv, cmd_size);
   cmd->num_slots = align(cmd_size, 8) / 8;
   cmd->buffer = MIN2(buffer, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->drawbuffer = drawbuffer;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, value, value_size);
}


/* ClearBufferuiv: marshalled asynchronously */
struct marshal_cmd_ClearBufferuiv
{
   struct marshal_cmd_base cmd_base;
   uint16_t num_slots;
   GLenum16 buffer;
   GLint drawbuffer;
   /* Next _mesa_buffer_enum_to_count(buffer) * 1 * sizeof(GLuint) bytes are GLuint value[None] */
};
uint32_t _mesa_unmarshal_ClearBufferuiv(struct gl_context *ctx, const struct marshal_cmd_ClearBufferuiv *restrict cmd)
{
   GLenum16 buffer = cmd->buffer;
   GLint drawbuffer = cmd->drawbuffer;
   GLuint *value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLuint *) variable_data;
   CALL_ClearBufferuiv(ctx->Dispatch.Current, (buffer, drawbuffer, value));
   return cmd->num_slots;
}
static void GLAPIENTRY
_mesa_marshal_ClearBufferuiv(GLenum buffer, GLint drawbuffer, const GLuint *value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = _mesa_buffer_enum_to_count(buffer) * 1 * sizeof(GLuint);
   int cmd_size = sizeof(struct marshal_cmd_ClearBufferuiv) + value_size;
   assert(cmd_size >= 0 && cmd_size <= MARSHAL_MAX_CMD_SIZE);
   struct marshal_cmd_ClearBufferuiv *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ClearBufferuiv, cmd_size);
   cmd->num_slots = align(cmd_size, 8) / 8;
   cmd->buffer = MIN2(buffer, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->drawbuffer = drawbuffer;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, value, value_size);
}


/* ClearBufferfv: marshalled asynchronously */
struct marshal_cmd_ClearBufferfv
{
   struct marshal_cmd_base cmd_base;
   uint16_t num_slots;
   GLenum16 buffer;
   GLint drawbuffer;
   /* Next _mesa_buffer_enum_to_count(buffer) * 1 * sizeof(GLfloat) bytes are GLfloat value[None] */
};
uint32_t _mesa_unmarshal_ClearBufferfv(struct gl_context *ctx, const struct marshal_cmd_ClearBufferfv *restrict cmd)
{
   GLenum16 buffer = cmd->buffer;
   GLint drawbuffer = cmd->drawbuffer;
   GLfloat *value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLfloat *) variable_data;
   CALL_ClearBufferfv(ctx->Dispatch.Current, (buffer, drawbuffer, value));
   return cmd->num_slots;
}
static void GLAPIENTRY
_mesa_marshal_ClearBufferfv(GLenum buffer, GLint drawbuffer, const GLfloat *value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = _mesa_buffer_enum_to_count(buffer) * 1 * sizeof(GLfloat);
   int cmd_size = sizeof(struct marshal_cmd_ClearBufferfv) + value_size;
   assert(cmd_size >= 0 && cmd_size <= MARSHAL_MAX_CMD_SIZE);
   struct marshal_cmd_ClearBufferfv *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ClearBufferfv, cmd_size);
   cmd->num_slots = align(cmd_size, 8) / 8;
   cmd->buffer = MIN2(buffer, 0xffff); /* clamped to 0xffff (invalid enum) */
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
uint32_t _mesa_unmarshal_ClearBufferfi(struct gl_context *ctx, const struct marshal_cmd_ClearBufferfi *restrict cmd)
{
   GLenum16 buffer = cmd->buffer;
   GLint drawbuffer = cmd->drawbuffer;
   GLfloat depth = cmd->depth;
   GLint stencil = cmd->stencil;
   CALL_ClearBufferfi(ctx->Dispatch.Current, (buffer, drawbuffer, depth, stencil));
   return align(sizeof(struct marshal_cmd_ClearBufferfi), 8) / 8;
}
static void GLAPIENTRY
_mesa_marshal_ClearBufferfi(GLenum buffer, GLint drawbuffer, GLfloat depth, GLint stencil)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ClearBufferfi);
   struct marshal_cmd_ClearBufferfi *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ClearBufferfi, cmd_size);
   cmd->buffer = MIN2(buffer, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->drawbuffer = drawbuffer;
   cmd->depth = depth;
   cmd->stencil = stencil;
}


/* GetStringi: marshalled synchronously */
static const GLubyte * GLAPIENTRY
_mesa_marshal_GetStringi(GLenum name, GLuint index)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetStringi");
   return CALL_GetStringi(ctx->Dispatch.Current, (name, index));
}


/* BeginTransformFeedback: marshalled asynchronously */
struct marshal_cmd_BeginTransformFeedback
{
   struct marshal_cmd_base cmd_base;
   GLenum16 mode;
};
uint32_t _mesa_unmarshal_BeginTransformFeedback(struct gl_context *ctx, const struct marshal_cmd_BeginTransformFeedback *restrict cmd)
{
   GLenum16 mode = cmd->mode;
   CALL_BeginTransformFeedback(ctx->Dispatch.Current, (mode));
   return align(sizeof(struct marshal_cmd_BeginTransformFeedback), 8) / 8;
}
static void GLAPIENTRY
_mesa_marshal_BeginTransformFeedback(GLenum mode)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_BeginTransformFeedback);
   struct marshal_cmd_BeginTransformFeedback *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_BeginTransformFeedback, cmd_size);
   cmd->mode = MIN2(mode, 0xffff); /* clamped to 0xffff (invalid enum) */
}


/* EndTransformFeedback: marshalled asynchronously */
struct marshal_cmd_EndTransformFeedback
{
   struct marshal_cmd_base cmd_base;
};
uint32_t _mesa_unmarshal_EndTransformFeedback(struct gl_context *ctx, const struct marshal_cmd_EndTransformFeedback *restrict cmd)
{
   CALL_EndTransformFeedback(ctx->Dispatch.Current, ());
   return align(sizeof(struct marshal_cmd_EndTransformFeedback), 8) / 8;
}
static void GLAPIENTRY
_mesa_marshal_EndTransformFeedback(void)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_EndTransformFeedback);
   _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_EndTransformFeedback, cmd_size);
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
uint32_t _mesa_unmarshal_BindBufferRange(struct gl_context *ctx, const struct marshal_cmd_BindBufferRange *restrict cmd)
{
   GLenum16 target = cmd->target;
   GLuint index = cmd->index;
   GLuint buffer = cmd->buffer;
   GLintptr offset = cmd->offset;
   GLsizeiptr size = cmd->size;
   CALL_BindBufferRange(ctx->Dispatch.Current, (target, index, buffer, offset, size));
   return align(sizeof(struct marshal_cmd_BindBufferRange), 8) / 8;
}
static void GLAPIENTRY
_mesa_marshal_BindBufferRange(GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_BindBufferRange);
   struct marshal_cmd_BindBufferRange *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_BindBufferRange, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
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
uint32_t _mesa_unmarshal_BindBufferBase(struct gl_context *ctx, const struct marshal_cmd_BindBufferBase *restrict cmd)
{
   GLenum16 target = cmd->target;
   GLuint index = cmd->index;
   GLuint buffer = cmd->buffer;
   CALL_BindBufferBase(ctx->Dispatch.Current, (target, index, buffer));
   return align(sizeof(struct marshal_cmd_BindBufferBase), 8) / 8;
}
static void GLAPIENTRY
_mesa_marshal_BindBufferBase(GLenum target, GLuint index, GLuint buffer)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_BindBufferBase);
   struct marshal_cmd_BindBufferBase *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_BindBufferBase, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->index = index;
   cmd->buffer = buffer;
}


/* TransformFeedbackVaryings: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_TransformFeedbackVaryings(GLuint program, GLsizei count, const GLchar * const *varyings, GLenum bufferMode)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "TransformFeedbackVaryings");
   CALL_TransformFeedbackVaryings(ctx->Dispatch.Current, (program, count, varyings, bufferMode));
}


/* GetTransformFeedbackVarying: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetTransformFeedbackVarying(GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLsizei *size, GLenum *type, GLchar *name)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetTransformFeedbackVarying");
   CALL_GetTransformFeedbackVarying(ctx->Dispatch.Current, (program, index, bufSize, length, size, type, name));
}


/* BeginConditionalRender: marshalled asynchronously */
struct marshal_cmd_BeginConditionalRender
{
   struct marshal_cmd_base cmd_base;
   GLenum16 mode;
   GLuint query;
};
uint32_t _mesa_unmarshal_BeginConditionalRender(struct gl_context *ctx, const struct marshal_cmd_BeginConditionalRender *restrict cmd)
{
   GLenum16 mode = cmd->mode;
   GLuint query = cmd->query;
   CALL_BeginConditionalRender(ctx->Dispatch.Current, (query, mode));
   return align(sizeof(struct marshal_cmd_BeginConditionalRender), 8) / 8;
}
static void GLAPIENTRY
_mesa_marshal_BeginConditionalRender(GLuint query, GLenum mode)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_BeginConditionalRender);
   struct marshal_cmd_BeginConditionalRender *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_BeginConditionalRender, cmd_size);
   cmd->mode = MIN2(mode, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->query = query;
}


/* EndConditionalRender: marshalled asynchronously */
struct marshal_cmd_EndConditionalRender
{
   struct marshal_cmd_base cmd_base;
};
uint32_t _mesa_unmarshal_EndConditionalRender(struct gl_context *ctx, const struct marshal_cmd_EndConditionalRender *restrict cmd)
{
   CALL_EndConditionalRender(ctx->Dispatch.Current, ());
   return align(sizeof(struct marshal_cmd_EndConditionalRender), 8) / 8;
}
static void GLAPIENTRY
_mesa_marshal_EndConditionalRender(void)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_EndConditionalRender);
   _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_EndConditionalRender, cmd_size);
}


/* PrimitiveRestartIndex: marshalled asynchronously */
struct marshal_cmd_PrimitiveRestartIndex
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
};
uint32_t _mesa_unmarshal_PrimitiveRestartIndex(struct gl_context *ctx, const struct marshal_cmd_PrimitiveRestartIndex *restrict cmd)
{
   GLuint index = cmd->index;
   CALL_PrimitiveRestartIndex(ctx->Dispatch.Current, (index));
   return align(sizeof(struct marshal_cmd_PrimitiveRestartIndex), 8) / 8;
}
static void GLAPIENTRY
_mesa_marshal_PrimitiveRestartIndex(GLuint index)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_PrimitiveRestartIndex);
   struct marshal_cmd_PrimitiveRestartIndex *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_PrimitiveRestartIndex, cmd_size);
   cmd->index = index;
   _mesa_glthread_PrimitiveRestartIndex(ctx, index);
}


/* GetInteger64i_v: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetInteger64i_v(GLenum cap, GLuint index, GLint64 *data)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetInteger64i_v");
   CALL_GetInteger64i_v(ctx->Dispatch.Current, (cap, index, data));
}


/* GetBufferParameteri64v: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetBufferParameteri64v(GLenum target, GLenum pname, GLint64 *params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetBufferParameteri64v");
   CALL_GetBufferParameteri64v(ctx->Dispatch.Current, (target, pname, params));
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
uint32_t _mesa_unmarshal_FramebufferTexture(struct gl_context *ctx, const struct marshal_cmd_FramebufferTexture *restrict cmd)
{
   GLenum16 target = cmd->target;
   GLenum16 attachment = cmd->attachment;
   GLuint texture = cmd->texture;
   GLint level = cmd->level;
   CALL_FramebufferTexture(ctx->Dispatch.Current, (target, attachment, texture, level));
   return align(sizeof(struct marshal_cmd_FramebufferTexture), 8) / 8;
}
static void GLAPIENTRY
_mesa_marshal_FramebufferTexture(GLenum target, GLenum attachment, GLuint texture, GLint level)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_FramebufferTexture);
   struct marshal_cmd_FramebufferTexture *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_FramebufferTexture, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->attachment = MIN2(attachment, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->texture = texture;
   cmd->level = level;
}


/* PrimitiveRestartNV: marshalled asynchronously */
struct marshal_cmd_PrimitiveRestartNV
{
   struct marshal_cmd_base cmd_base;
};
uint32_t _mesa_unmarshal_PrimitiveRestartNV(struct gl_context *ctx, const struct marshal_cmd_PrimitiveRestartNV *restrict cmd)
{
   CALL_PrimitiveRestartNV(ctx->Dispatch.Current, ());
   return align(sizeof(struct marshal_cmd_PrimitiveRestartNV), 8) / 8;
}
void GLAPIENTRY
_mesa_marshal_PrimitiveRestartNV(void)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_PrimitiveRestartNV);
   _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_PrimitiveRestartNV, cmd_size);
}


/* BindBufferOffsetEXT: marshalled asynchronously */
struct marshal_cmd_BindBufferOffsetEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum8 index;
   GLenum16 target;
   GLuint buffer;
   GLintptr offset;
};
struct marshal_cmd_BindBufferOffsetEXT_packed
{
   struct marshal_cmd_base cmd_base;
   GLenum8 index;
   GLenum16 target;
   GLuint buffer;
   GLuint offset;
};
uint32_t _mesa_unmarshal_BindBufferOffsetEXT(struct gl_context *ctx, const struct marshal_cmd_BindBufferOffsetEXT *restrict cmd)
{
   GLenum8 index = cmd->index;
   GLenum16 target = cmd->target;
   GLuint buffer = cmd->buffer;
   GLintptr offset = cmd->offset;
   CALL_BindBufferOffsetEXT(ctx->Dispatch.Current, (target, index, buffer, offset));
   return align(sizeof(struct marshal_cmd_BindBufferOffsetEXT), 8) / 8;
}
uint32_t _mesa_unmarshal_BindBufferOffsetEXT_packed(struct gl_context *ctx, const struct marshal_cmd_BindBufferOffsetEXT_packed *restrict cmd)
{
   GLenum8 index = cmd->index;
   GLenum16 target = cmd->target;
   GLuint buffer = cmd->buffer;
   GLintptr offset = (GLintptr)(uintptr_t)cmd->offset;
   CALL_BindBufferOffsetEXT(ctx->Dispatch.Current, (target, index, buffer, offset));
   return align(sizeof(struct marshal_cmd_BindBufferOffsetEXT_packed), 8) / 8;
}
static void GLAPIENTRY
_mesa_marshal_BindBufferOffsetEXT(GLenum target, GLuint index, GLuint buffer, GLintptr offset)
{
   GET_CURRENT_CONTEXT(ctx);
   if (((uintptr_t)offset & 0xffffffff) == (uintptr_t)offset) {
      int cmd_size = sizeof(struct marshal_cmd_BindBufferOffsetEXT_packed);
      struct marshal_cmd_BindBufferOffsetEXT_packed *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_BindBufferOffsetEXT_packed, cmd_size);
      cmd->index = MIN2(index, 0xff); /* clamped to 0xff (invalid enum) */
      cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
      cmd->buffer = buffer;
      cmd->offset = (uintptr_t)offset; /* truncated */
   } else {
      int cmd_size = sizeof(struct marshal_cmd_BindBufferOffsetEXT);
      struct marshal_cmd_BindBufferOffsetEXT *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_BindBufferOffsetEXT, cmd_size);
      cmd->index = MIN2(index, 0xff); /* clamped to 0xff (invalid enum) */
      cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
      cmd->buffer = buffer;
      cmd->offset = offset;
   }
}


/* BindTransformFeedback: marshalled asynchronously */
struct marshal_cmd_BindTransformFeedback
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLuint id;
};
uint32_t _mesa_unmarshal_BindTransformFeedback(struct gl_context *ctx, const struct marshal_cmd_BindTransformFeedback *restrict cmd)
{
   GLenum16 target = cmd->target;
   GLuint id = cmd->id;
   CALL_BindTransformFeedback(ctx->Dispatch.Current, (target, id));
   return align(sizeof(struct marshal_cmd_BindTransformFeedback), 8) / 8;
}
static void GLAPIENTRY
_mesa_marshal_BindTransformFeedback(GLenum target, GLuint id)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_BindTransformFeedback);
   struct marshal_cmd_BindTransformFeedback *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_BindTransformFeedback, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->id = id;
}


/* DeleteTransformFeedbacks: marshalled asynchronously */
struct marshal_cmd_DeleteTransformFeedbacks
{
   struct marshal_cmd_base cmd_base;
   uint16_t num_slots;
   GLsizei n;
   /* Next safe_mul(n, 1 * sizeof(GLuint)) bytes are GLuint ids[n] */
};
uint32_t _mesa_unmarshal_DeleteTransformFeedbacks(struct gl_context *ctx, const struct marshal_cmd_DeleteTransformFeedbacks *restrict cmd)
{
   GLsizei n = cmd->n;
   GLuint *ids;
   const char *variable_data = (const char *) (cmd + 1);
   ids = (GLuint *) variable_data;
   CALL_DeleteTransformFeedbacks(ctx->Dispatch.Current, (n, ids));
   return cmd->num_slots;
}
static void GLAPIENTRY
_mesa_marshal_DeleteTransformFeedbacks(GLsizei n, const GLuint *ids)
{
   GET_CURRENT_CONTEXT(ctx);
   int ids_size = safe_mul(n, 1 * sizeof(GLuint));
   int cmd_size = sizeof(struct marshal_cmd_DeleteTransformFeedbacks) + ids_size;
   if (unlikely(ids_size < 0 || (ids_size > 0 && !ids) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "DeleteTransformFeedbacks");
      CALL_DeleteTransformFeedbacks(ctx->Dispatch.Current, (n, ids));
      return;
   }
   struct marshal_cmd_DeleteTransformFeedbacks *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_DeleteTransformFeedbacks, cmd_size);
   cmd->num_slots = align(cmd_size, 8) / 8;
   cmd->n = n;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, ids, ids_size);
}


/* GenTransformFeedbacks: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GenTransformFeedbacks(GLsizei n, GLuint *ids)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GenTransformFeedbacks");
   CALL_GenTransformFeedbacks(ctx->Dispatch.Current, (n, ids));
}


/* IsTransformFeedback: marshalled synchronously */
static GLboolean GLAPIENTRY
_mesa_marshal_IsTransformFeedback(GLuint id)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "IsTransformFeedback");
   return CALL_IsTransformFeedback(ctx->Dispatch.Current, (id));
}


/* PauseTransformFeedback: marshalled asynchronously */
struct marshal_cmd_PauseTransformFeedback
{
   struct marshal_cmd_base cmd_base;
};
uint32_t _mesa_unmarshal_PauseTransformFeedback(struct gl_context *ctx, const struct marshal_cmd_PauseTransformFeedback *restrict cmd)
{
   CALL_PauseTransformFeedback(ctx->Dispatch.Current, ());
   return align(sizeof(struct marshal_cmd_PauseTransformFeedback), 8) / 8;
}
static void GLAPIENTRY
_mesa_marshal_PauseTransformFeedback(void)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_PauseTransformFeedback);
   _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_PauseTransformFeedback, cmd_size);
}


/* ResumeTransformFeedback: marshalled asynchronously */
struct marshal_cmd_ResumeTransformFeedback
{
   struct marshal_cmd_base cmd_base;
};
uint32_t _mesa_unmarshal_ResumeTransformFeedback(struct gl_context *ctx, const struct marshal_cmd_ResumeTransformFeedback *restrict cmd)
{
   CALL_ResumeTransformFeedback(ctx->Dispatch.Current, ());
   return align(sizeof(struct marshal_cmd_ResumeTransformFeedback), 8) / 8;
}
static void GLAPIENTRY
_mesa_marshal_ResumeTransformFeedback(void)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ResumeTransformFeedback);
   _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ResumeTransformFeedback, cmd_size);
}


/* DrawTransformFeedback: marshalled asynchronously */
struct marshal_cmd_DrawTransformFeedback
{
   struct marshal_cmd_base cmd_base;
   GLenum8 mode;
   GLuint id;
};
uint32_t _mesa_unmarshal_DrawTransformFeedback(struct gl_context *ctx, const struct marshal_cmd_DrawTransformFeedback *restrict cmd)
{
   GLenum8 mode = cmd->mode;
   GLuint id = cmd->id;
   CALL_DrawTransformFeedback(ctx->Dispatch.Current, (mode, id));
   return align(sizeof(struct marshal_cmd_DrawTransformFeedback), 8) / 8;
}
static void GLAPIENTRY
_mesa_marshal_DrawTransformFeedback(GLenum mode, GLuint id)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_DrawTransformFeedback);
   struct marshal_cmd_DrawTransformFeedback *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_DrawTransformFeedback, cmd_size);
   cmd->mode = MIN2(mode, 0xff); /* clamped to 0xff (invalid enum) */
   cmd->id = id;
}


/* VDPAUInitNV: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_VDPAUInitNV(const GLvoid *vdpDevice, const GLvoid *getProcAddress)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "VDPAUInitNV");
   CALL_VDPAUInitNV(ctx->Dispatch.Current, (vdpDevice, getProcAddress));
}


/* VDPAUFiniNV: marshalled asynchronously */
struct marshal_cmd_VDPAUFiniNV
{
   struct marshal_cmd_base cmd_base;
};
uint32_t _mesa_unmarshal_VDPAUFiniNV(struct gl_context *ctx, const struct marshal_cmd_VDPAUFiniNV *restrict cmd)
{
   CALL_VDPAUFiniNV(ctx->Dispatch.Current, ());
   return align(sizeof(struct marshal_cmd_VDPAUFiniNV), 8) / 8;
}
static void GLAPIENTRY
_mesa_marshal_VDPAUFiniNV(void)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VDPAUFiniNV);
   _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VDPAUFiniNV, cmd_size);
}


/* VDPAURegisterVideoSurfaceNV: marshalled synchronously */
static GLintptr GLAPIENTRY
_mesa_marshal_VDPAURegisterVideoSurfaceNV(const GLvoid *vdpSurface, GLenum target, GLsizei numTextureNames, const GLuint *textureNames)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "VDPAURegisterVideoSurfaceNV");
   return CALL_VDPAURegisterVideoSurfaceNV(ctx->Dispatch.Current, (vdpSurface, target, numTextureNames, textureNames));
}


/* VDPAURegisterOutputSurfaceNV: marshalled synchronously */
static GLintptr GLAPIENTRY
_mesa_marshal_VDPAURegisterOutputSurfaceNV(const GLvoid *vdpSurface, GLenum target, GLsizei numTextureNames, const GLuint *textureNames)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "VDPAURegisterOutputSurfaceNV");
   return CALL_VDPAURegisterOutputSurfaceNV(ctx->Dispatch.Current, (vdpSurface, target, numTextureNames, textureNames));
}


/* VDPAUIsSurfaceNV: marshalled synchronously */
static GLboolean GLAPIENTRY
_mesa_marshal_VDPAUIsSurfaceNV(GLintptr surface)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "VDPAUIsSurfaceNV");
   return CALL_VDPAUIsSurfaceNV(ctx->Dispatch.Current, (surface));
}


/* VDPAUUnregisterSurfaceNV: marshalled asynchronously */
struct marshal_cmd_VDPAUUnregisterSurfaceNV
{
   struct marshal_cmd_base cmd_base;
   GLintptr surface;
};
uint32_t _mesa_unmarshal_VDPAUUnregisterSurfaceNV(struct gl_context *ctx, const struct marshal_cmd_VDPAUUnregisterSurfaceNV *restrict cmd)
{
   GLintptr surface = cmd->surface;
   CALL_VDPAUUnregisterSurfaceNV(ctx->Dispatch.Current, (surface));
   return align(sizeof(struct marshal_cmd_VDPAUUnregisterSurfaceNV), 8) / 8;
}
static void GLAPIENTRY
_mesa_marshal_VDPAUUnregisterSurfaceNV(GLintptr surface)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VDPAUUnregisterSurfaceNV);
   struct marshal_cmd_VDPAUUnregisterSurfaceNV *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VDPAUUnregisterSurfaceNV, cmd_size);
   cmd->surface = surface;
}


/* VDPAUGetSurfaceivNV: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_VDPAUGetSurfaceivNV(GLintptr surface, GLenum pname, GLsizei bufSize, GLsizei *length, GLint *values)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "VDPAUGetSurfaceivNV");
   CALL_VDPAUGetSurfaceivNV(ctx->Dispatch.Current, (surface, pname, bufSize, length, values));
}


/* VDPAUSurfaceAccessNV: marshalled asynchronously */
struct marshal_cmd_VDPAUSurfaceAccessNV
{
   struct marshal_cmd_base cmd_base;
   GLenum16 access;
   GLintptr surface;
};
uint32_t _mesa_unmarshal_VDPAUSurfaceAccessNV(struct gl_context *ctx, const struct marshal_cmd_VDPAUSurfaceAccessNV *restrict cmd)
{
   GLenum16 access = cmd->access;
   GLintptr surface = cmd->surface;
   CALL_VDPAUSurfaceAccessNV(ctx->Dispatch.Current, (surface, access));
   return align(sizeof(struct marshal_cmd_VDPAUSurfaceAccessNV), 8) / 8;
}
static void GLAPIENTRY
_mesa_marshal_VDPAUSurfaceAccessNV(GLintptr surface, GLenum access)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VDPAUSurfaceAccessNV);
   struct marshal_cmd_VDPAUSurfaceAccessNV *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VDPAUSurfaceAccessNV, cmd_size);
   cmd->access = MIN2(access, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->surface = surface;
}


/* VDPAUMapSurfacesNV: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_VDPAUMapSurfacesNV(GLsizei numSurfaces, const GLintptr *surfaces)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "VDPAUMapSurfacesNV");
   CALL_VDPAUMapSurfacesNV(ctx->Dispatch.Current, (numSurfaces, surfaces));
}


/* VDPAUUnmapSurfacesNV: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_VDPAUUnmapSurfacesNV(GLsizei numSurfaces, const GLintptr *surfaces)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "VDPAUUnmapSurfacesNV");
   CALL_VDPAUUnmapSurfacesNV(ctx->Dispatch.Current, (numSurfaces, surfaces));
}


/* GetUnsignedBytevEXT: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetUnsignedBytevEXT(GLenum pname, GLubyte *data)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetUnsignedBytevEXT");
   CALL_GetUnsignedBytevEXT(ctx->Dispatch.Current, (pname, data));
}


/* GetUnsignedBytei_vEXT: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetUnsignedBytei_vEXT(GLenum target, GLuint index, GLubyte *data)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetUnsignedBytei_vEXT");
   CALL_GetUnsignedBytei_vEXT(ctx->Dispatch.Current, (target, index, data));
}


/* DeleteMemoryObjectsEXT: marshalled asynchronously */
struct marshal_cmd_DeleteMemoryObjectsEXT
{
   struct marshal_cmd_base cmd_base;
   uint16_t num_slots;
   GLsizei n;
   /* Next safe_mul(n, 1 * sizeof(GLuint)) bytes are GLuint memoryObjects[n] */
};
uint32_t _mesa_unmarshal_DeleteMemoryObjectsEXT(struct gl_context *ctx, const struct marshal_cmd_DeleteMemoryObjectsEXT *restrict cmd)
{
   GLsizei n = cmd->n;
   GLuint *memoryObjects;
   const char *variable_data = (const char *) (cmd + 1);
   memoryObjects = (GLuint *) variable_data;
   CALL_DeleteMemoryObjectsEXT(ctx->Dispatch.Current, (n, memoryObjects));
   return cmd->num_slots;
}
static void GLAPIENTRY
_mesa_marshal_DeleteMemoryObjectsEXT(GLsizei n, const GLuint *memoryObjects)
{
   GET_CURRENT_CONTEXT(ctx);
   int memoryObjects_size = safe_mul(n, 1 * sizeof(GLuint));
   int cmd_size = sizeof(struct marshal_cmd_DeleteMemoryObjectsEXT) + memoryObjects_size;
   if (unlikely(memoryObjects_size < 0 || (memoryObjects_size > 0 && !memoryObjects) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "DeleteMemoryObjectsEXT");
      CALL_DeleteMemoryObjectsEXT(ctx->Dispatch.Current, (n, memoryObjects));
      return;
   }
   struct marshal_cmd_DeleteMemoryObjectsEXT *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_DeleteMemoryObjectsEXT, cmd_size);
   cmd->num_slots = align(cmd_size, 8) / 8;
   cmd->n = n;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, memoryObjects, memoryObjects_size);
}


/* IsMemoryObjectEXT: marshalled synchronously */
static GLboolean GLAPIENTRY
_mesa_marshal_IsMemoryObjectEXT(GLuint memoryObject)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "IsMemoryObjectEXT");
   return CALL_IsMemoryObjectEXT(ctx->Dispatch.Current, (memoryObject));
}


/* CreateMemoryObjectsEXT: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_CreateMemoryObjectsEXT(GLsizei n, GLuint *memoryObjects)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "CreateMemoryObjectsEXT");
   CALL_CreateMemoryObjectsEXT(ctx->Dispatch.Current, (n, memoryObjects));
}


/* MemoryObjectParameterivEXT: marshalled asynchronously */
struct marshal_cmd_MemoryObjectParameterivEXT
{
   struct marshal_cmd_base cmd_base;
   uint16_t num_slots;
   GLenum16 pname;
   GLuint memoryObject;
   /* Next _mesa_memobj_enum_to_count(pname) * 1 * sizeof(GLint) bytes are GLint params[None] */
};
uint32_t _mesa_unmarshal_MemoryObjectParameterivEXT(struct gl_context *ctx, const struct marshal_cmd_MemoryObjectParameterivEXT *restrict cmd)
{
   GLenum16 pname = cmd->pname;
   GLuint memoryObject = cmd->memoryObject;
   GLint *params;
   const char *variable_data = (const char *) (cmd + 1);
   params = (GLint *) variable_data;
   CALL_MemoryObjectParameterivEXT(ctx->Dispatch.Current, (memoryObject, pname, params));
   return cmd->num_slots;
}
static void GLAPIENTRY
_mesa_marshal_MemoryObjectParameterivEXT(GLuint memoryObject, GLenum pname, const GLint *params)
{
   GET_CURRENT_CONTEXT(ctx);
   int params_size = _mesa_memobj_enum_to_count(pname) * 1 * sizeof(GLint);
   int cmd_size = sizeof(struct marshal_cmd_MemoryObjectParameterivEXT) + params_size;
   assert(cmd_size >= 0 && cmd_size <= MARSHAL_MAX_CMD_SIZE);
   struct marshal_cmd_MemoryObjectParameterivEXT *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MemoryObjectParameterivEXT, cmd_size);
   cmd->num_slots = align(cmd_size, 8) / 8;
   cmd->pname = MIN2(pname, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->memoryObject = memoryObject;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, params, params_size);
}


/* GetMemoryObjectParameterivEXT: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetMemoryObjectParameterivEXT(GLuint memoryObject, GLenum pname, GLint *params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetMemoryObjectParameterivEXT");
   CALL_GetMemoryObjectParameterivEXT(ctx->Dispatch.Current, (memoryObject, pname, params));
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
uint32_t _mesa_unmarshal_TexStorageMem2DEXT(struct gl_context *ctx, const struct marshal_cmd_TexStorageMem2DEXT *restrict cmd)
{
   GLenum16 target = cmd->target;
   GLenum16 internalFormat = cmd->internalFormat;
   GLsizei levels = cmd->levels;
   GLsizei width = cmd->width;
   GLsizei height = cmd->height;
   GLuint memory = cmd->memory;
   GLuint64 offset = cmd->offset;
   CALL_TexStorageMem2DEXT(ctx->Dispatch.Current, (target, levels, internalFormat, width, height, memory, offset));
   return align(sizeof(struct marshal_cmd_TexStorageMem2DEXT), 8) / 8;
}
static void GLAPIENTRY
_mesa_marshal_TexStorageMem2DEXT(GLenum target, GLsizei levels, GLenum internalFormat, GLsizei width, GLsizei height, GLuint memory, GLuint64 offset)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TexStorageMem2DEXT);
   struct marshal_cmd_TexStorageMem2DEXT *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexStorageMem2DEXT, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->internalFormat = MIN2(internalFormat, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->levels = levels;
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
uint32_t _mesa_unmarshal_TexStorageMem2DMultisampleEXT(struct gl_context *ctx, const struct marshal_cmd_TexStorageMem2DMultisampleEXT *restrict cmd)
{
   GLboolean fixedSampleLocations = cmd->fixedSampleLocations;
   GLenum16 target = cmd->target;
   GLenum16 internalFormat = cmd->internalFormat;
   GLsizei samples = cmd->samples;
   GLsizei width = cmd->width;
   GLsizei height = cmd->height;
   GLuint memory = cmd->memory;
   GLuint64 offset = cmd->offset;
   CALL_TexStorageMem2DMultisampleEXT(ctx->Dispatch.Current, (target, samples, internalFormat, width, height, fixedSampleLocations, memory, offset));
   return align(sizeof(struct marshal_cmd_TexStorageMem2DMultisampleEXT), 8) / 8;
}
static void GLAPIENTRY
_mesa_marshal_TexStorageMem2DMultisampleEXT(GLenum target, GLsizei samples, GLenum internalFormat, GLsizei width, GLsizei height, GLboolean fixedSampleLocations, GLuint memory, GLuint64 offset)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TexStorageMem2DMultisampleEXT);
   struct marshal_cmd_TexStorageMem2DMultisampleEXT *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexStorageMem2DMultisampleEXT, cmd_size);
   cmd->fixedSampleLocations = fixedSampleLocations;
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->internalFormat = MIN2(internalFormat, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->samples = samples;
   cmd->width = width;
   cmd->height = height;
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
uint32_t _mesa_unmarshal_TexStorageMem3DEXT(struct gl_context *ctx, const struct marshal_cmd_TexStorageMem3DEXT *restrict cmd)
{
   GLenum16 target = cmd->target;
   GLenum16 internalFormat = cmd->internalFormat;
   GLsizei levels = cmd->levels;
   GLsizei width = cmd->width;
   GLsizei height = cmd->height;
   GLsizei depth = cmd->depth;
   GLuint memory = cmd->memory;
   GLuint64 offset = cmd->offset;
   CALL_TexStorageMem3DEXT(ctx->Dispatch.Current, (target, levels, internalFormat, width, height, depth, memory, offset));
   return align(sizeof(struct marshal_cmd_TexStorageMem3DEXT), 8) / 8;
}
static void GLAPIENTRY
_mesa_marshal_TexStorageMem3DEXT(GLenum target, GLsizei levels, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLuint memory, GLuint64 offset)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TexStorageMem3DEXT);
   struct marshal_cmd_TexStorageMem3DEXT *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexStorageMem3DEXT, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->internalFormat = MIN2(internalFormat, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->levels = levels;
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
uint32_t _mesa_unmarshal_TexStorageMem3DMultisampleEXT(struct gl_context *ctx, const struct marshal_cmd_TexStorageMem3DMultisampleEXT *restrict cmd)
{
   GLboolean fixedSampleLocations = cmd->fixedSampleLocations;
   GLenum16 target = cmd->target;
   GLenum16 internalFormat = cmd->internalFormat;
   GLsizei samples = cmd->samples;
   GLsizei width = cmd->width;
   GLsizei height = cmd->height;
   GLsizei depth = cmd->depth;
   GLuint memory = cmd->memory;
   GLuint64 offset = cmd->offset;
   CALL_TexStorageMem3DMultisampleEXT(ctx->Dispatch.Current, (target, samples, internalFormat, width, height, depth, fixedSampleLocations, memory, offset));
   return align(sizeof(struct marshal_cmd_TexStorageMem3DMultisampleEXT), 8) / 8;
}
static void GLAPIENTRY
_mesa_marshal_TexStorageMem3DMultisampleEXT(GLenum target, GLsizei samples, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedSampleLocations, GLuint memory, GLuint64 offset)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TexStorageMem3DMultisampleEXT);
   struct marshal_cmd_TexStorageMem3DMultisampleEXT *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexStorageMem3DMultisampleEXT, cmd_size);
   cmd->fixedSampleLocations = fixedSampleLocations;
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->internalFormat = MIN2(internalFormat, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->samples = samples;
   cmd->width = width;
   cmd->height = height;
   cmd->depth = depth;
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
uint32_t _mesa_unmarshal_BufferStorageMemEXT(struct gl_context *ctx, const struct marshal_cmd_BufferStorageMemEXT *restrict cmd)
{
   GLenum16 target = cmd->target;
   GLuint memory = cmd->memory;
   GLsizeiptr size = cmd->size;
   GLuint64 offset = cmd->offset;
   CALL_BufferStorageMemEXT(ctx->Dispatch.Current, (target, size, memory, offset));
   return align(sizeof(struct marshal_cmd_BufferStorageMemEXT), 8) / 8;
}
static void GLAPIENTRY
_mesa_marshal_BufferStorageMemEXT(GLenum target, GLsizeiptr size, GLuint memory, GLuint64 offset)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_BufferStorageMemEXT);
   struct marshal_cmd_BufferStorageMemEXT *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_BufferStorageMemEXT, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->memory = memory;
   cmd->size = size;
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
uint32_t _mesa_unmarshal_TextureStorageMem2DEXT(struct gl_context *ctx, const struct marshal_cmd_TextureStorageMem2DEXT *restrict cmd)
{
   GLenum16 texture = cmd->texture;
   GLenum16 internalFormat = cmd->internalFormat;
   GLsizei levels = cmd->levels;
   GLsizei width = cmd->width;
   GLsizei height = cmd->height;
   GLuint memory = cmd->memory;
   GLuint64 offset = cmd->offset;
   CALL_TextureStorageMem2DEXT(ctx->Dispatch.Current, (texture, levels, internalFormat, width, height, memory, offset));
   return align(sizeof(struct marshal_cmd_TextureStorageMem2DEXT), 8) / 8;
}
static void GLAPIENTRY
_mesa_marshal_TextureStorageMem2DEXT(GLenum texture, GLsizei levels, GLenum internalFormat, GLsizei width, GLsizei height, GLuint memory, GLuint64 offset)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TextureStorageMem2DEXT);
   struct marshal_cmd_TextureStorageMem2DEXT *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TextureStorageMem2DEXT, cmd_size);
   cmd->texture = MIN2(texture, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->internalFormat = MIN2(internalFormat, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->levels = levels;
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
uint32_t _mesa_unmarshal_TextureStorageMem2DMultisampleEXT(struct gl_context *ctx, const struct marshal_cmd_TextureStorageMem2DMultisampleEXT *restrict cmd)
{
   GLboolean fixedSampleLocations = cmd->fixedSampleLocations;
   GLenum16 internalFormat = cmd->internalFormat;
   GLuint texture = cmd->texture;
   GLsizei samples = cmd->samples;
   GLsizei width = cmd->width;
   GLsizei height = cmd->height;
   GLuint memory = cmd->memory;
   GLuint64 offset = cmd->offset;
   CALL_TextureStorageMem2DMultisampleEXT(ctx->Dispatch.Current, (texture, samples, internalFormat, width, height, fixedSampleLocations, memory, offset));
   return align(sizeof(struct marshal_cmd_TextureStorageMem2DMultisampleEXT), 8) / 8;
}
static void GLAPIENTRY
_mesa_marshal_TextureStorageMem2DMultisampleEXT(GLuint texture, GLsizei samples, GLenum internalFormat, GLsizei width, GLsizei height, GLboolean fixedSampleLocations, GLuint memory, GLuint64 offset)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TextureStorageMem2DMultisampleEXT);
   struct marshal_cmd_TextureStorageMem2DMultisampleEXT *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TextureStorageMem2DMultisampleEXT, cmd_size);
   cmd->fixedSampleLocations = fixedSampleLocations;
   cmd->internalFormat = MIN2(internalFormat, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->texture = texture;
   cmd->samples = samples;
   cmd->width = width;
   cmd->height = height;
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
uint32_t _mesa_unmarshal_TextureStorageMem3DEXT(struct gl_context *ctx, const struct marshal_cmd_TextureStorageMem3DEXT *restrict cmd)
{
   GLenum16 internalFormat = cmd->internalFormat;
   GLuint texture = cmd->texture;
   GLsizei levels = cmd->levels;
   GLsizei width = cmd->width;
   GLsizei height = cmd->height;
   GLsizei depth = cmd->depth;
   GLuint memory = cmd->memory;
   GLuint64 offset = cmd->offset;
   CALL_TextureStorageMem3DEXT(ctx->Dispatch.Current, (texture, levels, internalFormat, width, height, depth, memory, offset));
   return align(sizeof(struct marshal_cmd_TextureStorageMem3DEXT), 8) / 8;
}
static void GLAPIENTRY
_mesa_marshal_TextureStorageMem3DEXT(GLuint texture, GLsizei levels, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLuint memory, GLuint64 offset)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TextureStorageMem3DEXT);
   struct marshal_cmd_TextureStorageMem3DEXT *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TextureStorageMem3DEXT, cmd_size);
   cmd->internalFormat = MIN2(internalFormat, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->texture = texture;
   cmd->levels = levels;
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
uint32_t _mesa_unmarshal_TextureStorageMem3DMultisampleEXT(struct gl_context *ctx, const struct marshal_cmd_TextureStorageMem3DMultisampleEXT *restrict cmd)
{
   GLboolean fixedSampleLocations = cmd->fixedSampleLocations;
   GLenum16 internalFormat = cmd->internalFormat;
   GLuint texture = cmd->texture;
   GLsizei samples = cmd->samples;
   GLsizei width = cmd->width;
   GLsizei height = cmd->height;
   GLsizei depth = cmd->depth;
   GLuint memory = cmd->memory;
   GLuint64 offset = cmd->offset;
   CALL_TextureStorageMem3DMultisampleEXT(ctx->Dispatch.Current, (texture, samples, internalFormat, width, height, depth, fixedSampleLocations, memory, offset));
   return align(sizeof(struct marshal_cmd_TextureStorageMem3DMultisampleEXT), 8) / 8;
}
static void GLAPIENTRY
_mesa_marshal_TextureStorageMem3DMultisampleEXT(GLuint texture, GLsizei samples, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedSampleLocations, GLuint memory, GLuint64 offset)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TextureStorageMem3DMultisampleEXT);
   struct marshal_cmd_TextureStorageMem3DMultisampleEXT *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TextureStorageMem3DMultisampleEXT, cmd_size);
   cmd->fixedSampleLocations = fixedSampleLocations;
   cmd->internalFormat = MIN2(internalFormat, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->texture = texture;
   cmd->samples = samples;
   cmd->width = width;
   cmd->height = height;
   cmd->depth = depth;
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
uint32_t _mesa_unmarshal_NamedBufferStorageMemEXT(struct gl_context *ctx, const struct marshal_cmd_NamedBufferStorageMemEXT *restrict cmd)
{
   GLuint buffer = cmd->buffer;
   GLuint memory = cmd->memory;
   GLsizeiptr size = cmd->size;
   GLuint64 offset = cmd->offset;
   CALL_NamedBufferStorageMemEXT(ctx->Dispatch.Current, (buffer, size, memory, offset));
   return align(sizeof(struct marshal_cmd_NamedBufferStorageMemEXT), 8) / 8;
}
static void GLAPIENTRY
_mesa_marshal_NamedBufferStorageMemEXT(GLuint buffer, GLsizeiptr size, GLuint memory, GLuint64 offset)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_NamedBufferStorageMemEXT);
   struct marshal_cmd_NamedBufferStorageMemEXT *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_NamedBufferStorageMemEXT, cmd_size);
   cmd->buffer = buffer;
   cmd->memory = memory;
   cmd->size = size;
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
uint32_t _mesa_unmarshal_TexStorageMem1DEXT(struct gl_context *ctx, const struct marshal_cmd_TexStorageMem1DEXT *restrict cmd)
{
   GLenum16 target = cmd->target;
   GLenum16 internalFormat = cmd->internalFormat;
   GLsizei levels = cmd->levels;
   GLsizei width = cmd->width;
   GLuint memory = cmd->memory;
   GLuint64 offset = cmd->offset;
   CALL_TexStorageMem1DEXT(ctx->Dispatch.Current, (target, levels, internalFormat, width, memory, offset));
   return align(sizeof(struct marshal_cmd_TexStorageMem1DEXT), 8) / 8;
}
static void GLAPIENTRY
_mesa_marshal_TexStorageMem1DEXT(GLenum target, GLsizei levels, GLenum internalFormat, GLsizei width, GLuint memory, GLuint64 offset)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TexStorageMem1DEXT);
   struct marshal_cmd_TexStorageMem1DEXT *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexStorageMem1DEXT, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->internalFormat = MIN2(internalFormat, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->levels = levels;
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
uint32_t _mesa_unmarshal_TextureStorageMem1DEXT(struct gl_context *ctx, const struct marshal_cmd_TextureStorageMem1DEXT *restrict cmd)
{
   GLenum16 internalFormat = cmd->internalFormat;
   GLuint texture = cmd->texture;
   GLsizei levels = cmd->levels;
   GLsizei width = cmd->width;
   GLuint memory = cmd->memory;
   GLuint64 offset = cmd->offset;
   CALL_TextureStorageMem1DEXT(ctx->Dispatch.Current, (texture, levels, internalFormat, width, memory, offset));
   return align(sizeof(struct marshal_cmd_TextureStorageMem1DEXT), 8) / 8;
}
static void GLAPIENTRY
_mesa_marshal_TextureStorageMem1DEXT(GLuint texture, GLsizei levels, GLenum internalFormat, GLsizei width, GLuint memory, GLuint64 offset)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TextureStorageMem1DEXT);
   struct marshal_cmd_TextureStorageMem1DEXT *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TextureStorageMem1DEXT, cmd_size);
   cmd->internalFormat = MIN2(internalFormat, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->texture = texture;
   cmd->levels = levels;
   cmd->width = width;
   cmd->memory = memory;
   cmd->offset = offset;
}


/* GenSemaphoresEXT: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GenSemaphoresEXT(GLsizei n, GLuint *semaphores)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GenSemaphoresEXT");
   CALL_GenSemaphoresEXT(ctx->Dispatch.Current, (n, semaphores));
}


/* DeleteSemaphoresEXT: marshalled asynchronously */
struct marshal_cmd_DeleteSemaphoresEXT
{
   struct marshal_cmd_base cmd_base;
   uint16_t num_slots;
   GLsizei n;
   /* Next safe_mul(n, 1 * sizeof(GLuint)) bytes are GLuint semaphores[n] */
};
uint32_t _mesa_unmarshal_DeleteSemaphoresEXT(struct gl_context *ctx, const struct marshal_cmd_DeleteSemaphoresEXT *restrict cmd)
{
   GLsizei n = cmd->n;
   GLuint *semaphores;
   const char *variable_data = (const char *) (cmd + 1);
   semaphores = (GLuint *) variable_data;
   CALL_DeleteSemaphoresEXT(ctx->Dispatch.Current, (n, semaphores));
   return cmd->num_slots;
}
static void GLAPIENTRY
_mesa_marshal_DeleteSemaphoresEXT(GLsizei n, const GLuint *semaphores)
{
   GET_CURRENT_CONTEXT(ctx);
   int semaphores_size = safe_mul(n, 1 * sizeof(GLuint));
   int cmd_size = sizeof(struct marshal_cmd_DeleteSemaphoresEXT) + semaphores_size;
   if (unlikely(semaphores_size < 0 || (semaphores_size > 0 && !semaphores) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "DeleteSemaphoresEXT");
      CALL_DeleteSemaphoresEXT(ctx->Dispatch.Current, (n, semaphores));
      return;
   }
   struct marshal_cmd_DeleteSemaphoresEXT *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_DeleteSemaphoresEXT, cmd_size);
   cmd->num_slots = align(cmd_size, 8) / 8;
   cmd->n = n;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, semaphores, semaphores_size);
}


/* IsSemaphoreEXT: marshalled synchronously */
static GLboolean GLAPIENTRY
_mesa_marshal_IsSemaphoreEXT(GLuint semaphore)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "IsSemaphoreEXT");
   return CALL_IsSemaphoreEXT(ctx->Dispatch.Current, (semaphore));
}


/* SemaphoreParameterui64vEXT: marshalled asynchronously */
struct marshal_cmd_SemaphoreParameterui64vEXT
{
   struct marshal_cmd_base cmd_base;
   uint16_t num_slots;
   GLenum16 pname;
   GLuint semaphore;
   /* Next _mesa_semaphore_enum_to_count(pname) * 1 * sizeof(GLuint64) bytes are GLuint64 params[None] */
};
uint32_t _mesa_unmarshal_SemaphoreParameterui64vEXT(struct gl_context *ctx, const struct marshal_cmd_SemaphoreParameterui64vEXT *restrict cmd)
{
   GLenum16 pname = cmd->pname;
   GLuint semaphore = cmd->semaphore;
   GLuint64 *params;
   const char *variable_data = (const char *) (cmd + 1);
   params = (GLuint64 *) variable_data;
   CALL_SemaphoreParameterui64vEXT(ctx->Dispatch.Current, (semaphore, pname, params));
   return cmd->num_slots;
}
static void GLAPIENTRY
_mesa_marshal_SemaphoreParameterui64vEXT(GLuint semaphore, GLenum pname, const GLuint64 *params)
{
   GET_CURRENT_CONTEXT(ctx);
   int params_size = _mesa_semaphore_enum_to_count(pname) * 1 * sizeof(GLuint64);
   int cmd_size = sizeof(struct marshal_cmd_SemaphoreParameterui64vEXT) + params_size;
   assert(cmd_size >= 0 && cmd_size <= MARSHAL_MAX_CMD_SIZE);
   struct marshal_cmd_SemaphoreParameterui64vEXT *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_SemaphoreParameterui64vEXT, cmd_size);
   cmd->num_slots = align(cmd_size, 8) / 8;
   cmd->pname = MIN2(pname, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->semaphore = semaphore;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, params, params_size);
}


/* GetSemaphoreParameterui64vEXT: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetSemaphoreParameterui64vEXT(GLuint semaphore, GLenum pname, GLuint64 *params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetSemaphoreParameterui64vEXT");
   CALL_GetSemaphoreParameterui64vEXT(ctx->Dispatch.Current, (semaphore, pname, params));
}


/* WaitSemaphoreEXT: marshalled asynchronously */
struct marshal_cmd_WaitSemaphoreEXT
{
   struct marshal_cmd_base cmd_base;
   uint16_t num_slots;
   GLuint semaphore;
   GLuint numBufferBarriers;
   GLuint numTextureBarriers;
   /* Next safe_mul(numBufferBarriers, 1 * sizeof(GLuint)) bytes are GLuint buffers[numBufferBarriers] */
   /* Next safe_mul(numTextureBarriers, 1 * sizeof(GLuint)) bytes are GLuint textures[numTextureBarriers] */
   /* Next safe_mul(numTextureBarriers, 1 * sizeof(GLenum)) bytes are GLenum srcLayouts[numTextureBarriers] */
};
uint32_t _mesa_unmarshal_WaitSemaphoreEXT(struct gl_context *ctx, const struct marshal_cmd_WaitSemaphoreEXT *restrict cmd)
{
   GLuint semaphore = cmd->semaphore;
   GLuint numBufferBarriers = cmd->numBufferBarriers;
   GLuint numTextureBarriers = cmd->numTextureBarriers;
   GLuint *buffers;
   GLuint *textures;
   GLenum *srcLayouts;
   const char *variable_data = (const char *) (cmd + 1);
   buffers = (GLuint *) variable_data;
   variable_data += numBufferBarriers * 1 * sizeof(GLuint);
   textures = (GLuint *) variable_data;
   variable_data += numTextureBarriers * 1 * sizeof(GLuint);
   srcLayouts = (GLenum *) variable_data;
   CALL_WaitSemaphoreEXT(ctx->Dispatch.Current, (semaphore, numBufferBarriers, buffers, numTextureBarriers, textures, srcLayouts));
   return cmd->num_slots;
}
static void GLAPIENTRY
_mesa_marshal_WaitSemaphoreEXT(GLuint semaphore, GLuint numBufferBarriers, const GLuint *buffers, GLuint numTextureBarriers, const GLuint *textures, const GLenum *srcLayouts)
{
   GET_CURRENT_CONTEXT(ctx);
   int buffers_size = safe_mul(numBufferBarriers, 1 * sizeof(GLuint));
   int textures_size = safe_mul(numTextureBarriers, 1 * sizeof(GLuint));
   int srcLayouts_size = safe_mul(numTextureBarriers, 1 * sizeof(GLenum));
   int cmd_size = sizeof(struct marshal_cmd_WaitSemaphoreEXT) + buffers_size + textures_size + srcLayouts_size;
   if (unlikely(buffers_size < 0 || (buffers_size > 0 && !buffers) || textures_size < 0 || (textures_size > 0 && !textures) || srcLayouts_size < 0 || (srcLayouts_size > 0 && !srcLayouts) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "WaitSemaphoreEXT");
      CALL_WaitSemaphoreEXT(ctx->Dispatch.Current, (semaphore, numBufferBarriers, buffers, numTextureBarriers, textures, srcLayouts));
      return;
   }
   struct marshal_cmd_WaitSemaphoreEXT *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_WaitSemaphoreEXT, cmd_size);
   cmd->num_slots = align(cmd_size, 8) / 8;
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


/* SignalSemaphoreEXT: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_SignalSemaphoreEXT(GLuint semaphore, GLuint numBufferBarriers, const GLuint *buffers, GLuint numTextureBarriers, const GLuint *textures, const GLenum *dstLayouts)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "SignalSemaphoreEXT");
   CALL_SignalSemaphoreEXT(ctx->Dispatch.Current, (semaphore, numBufferBarriers, buffers, numTextureBarriers, textures, dstLayouts));
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
uint32_t _mesa_unmarshal_ImportMemoryFdEXT(struct gl_context *ctx, const struct marshal_cmd_ImportMemoryFdEXT *restrict cmd)
{
   GLenum16 handleType = cmd->handleType;
   GLuint memory = cmd->memory;
   GLint fd = cmd->fd;
   GLuint64 size = cmd->size;
   CALL_ImportMemoryFdEXT(ctx->Dispatch.Current, (memory, size, handleType, fd));
   return align(sizeof(struct marshal_cmd_ImportMemoryFdEXT), 8) / 8;
}
static void GLAPIENTRY
_mesa_marshal_ImportMemoryFdEXT(GLuint memory, GLuint64 size, GLenum handleType, GLint fd)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ImportMemoryFdEXT);
   struct marshal_cmd_ImportMemoryFdEXT *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ImportMemoryFdEXT, cmd_size);
   cmd->handleType = MIN2(handleType, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->memory = memory;
   cmd->fd = fd;
   cmd->size = size;
}


/* ImportSemaphoreFdEXT: marshalled asynchronously */
struct marshal_cmd_ImportSemaphoreFdEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 handleType;
   GLuint semaphore;
   GLint fd;
};
uint32_t _mesa_unmarshal_ImportSemaphoreFdEXT(struct gl_context *ctx, const struct marshal_cmd_ImportSemaphoreFdEXT *restrict cmd)
{
   GLenum16 handleType = cmd->handleType;
   GLuint semaphore = cmd->semaphore;
   GLint fd = cmd->fd;
   CALL_ImportSemaphoreFdEXT(ctx->Dispatch.Current, (semaphore, handleType, fd));
   return align(sizeof(struct marshal_cmd_ImportSemaphoreFdEXT), 8) / 8;
}
static void GLAPIENTRY
_mesa_marshal_ImportSemaphoreFdEXT(GLuint semaphore, GLenum handleType, GLint fd)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ImportSemaphoreFdEXT);
   struct marshal_cmd_ImportSemaphoreFdEXT *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ImportSemaphoreFdEXT, cmd_size);
   cmd->handleType = MIN2(handleType, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->semaphore = semaphore;
   cmd->fd = fd;
}


/* ImportMemoryWin32HandleEXT: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_ImportMemoryWin32HandleEXT(GLuint memory, GLuint64 size, GLenum handleType, GLvoid *handle)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "ImportMemoryWin32HandleEXT");
   CALL_ImportMemoryWin32HandleEXT(ctx->Dispatch.Current, (memory, size, handleType, handle));
}


/* ImportMemoryWin32NameEXT: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_ImportMemoryWin32NameEXT(GLuint memory, GLuint64 size, GLenum handleType, const GLvoid *name)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "ImportMemoryWin32NameEXT");
   CALL_ImportMemoryWin32NameEXT(ctx->Dispatch.Current, (memory, size, handleType, name));
}


/* ImportSemaphoreWin32HandleEXT: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_ImportSemaphoreWin32HandleEXT(GLuint semaphore, GLenum handleType, GLvoid *handle)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "ImportSemaphoreWin32HandleEXT");
   CALL_ImportSemaphoreWin32HandleEXT(ctx->Dispatch.Current, (semaphore, handleType, handle));
}


/* ImportSemaphoreWin32NameEXT: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_ImportSemaphoreWin32NameEXT(GLuint semaphore, GLenum handleType, const GLvoid *handle)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "ImportSemaphoreWin32NameEXT");
   CALL_ImportSemaphoreWin32NameEXT(ctx->Dispatch.Current, (semaphore, handleType, handle));
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
uint32_t _mesa_unmarshal_ViewportSwizzleNV(struct gl_context *ctx, const struct marshal_cmd_ViewportSwizzleNV *restrict cmd)
{
   GLenum16 swizzlex = cmd->swizzlex;
   GLenum16 swizzley = cmd->swizzley;
   GLenum16 swizzlez = cmd->swizzlez;
   GLenum16 swizzlew = cmd->swizzlew;
   GLuint index = cmd->index;
   CALL_ViewportSwizzleNV(ctx->Dispatch.Current, (index, swizzlex, swizzley, swizzlez, swizzlew));
   return align(sizeof(struct marshal_cmd_ViewportSwizzleNV), 8) / 8;
}
static void GLAPIENTRY
_mesa_marshal_ViewportSwizzleNV(GLuint index, GLenum swizzlex, GLenum swizzley, GLenum swizzlez, GLenum swizzlew)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ViewportSwizzleNV);
   struct marshal_cmd_ViewportSwizzleNV *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ViewportSwizzleNV, cmd_size);
   cmd->swizzlex = MIN2(swizzlex, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->swizzley = MIN2(swizzley, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->swizzlez = MIN2(swizzlez, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->swizzlew = MIN2(swizzlew, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->index = index;
}


/* Vertex2hNV: marshalled asynchronously */
struct marshal_cmd_Vertex2hNV
{
   struct marshal_cmd_base cmd_base;
   GLhalfNV x;
   GLhalfNV y;
};
uint32_t _mesa_unmarshal_Vertex2hNV(struct gl_context *ctx, const struct marshal_cmd_Vertex2hNV *restrict cmd)
{
   GLhalfNV x = cmd->x;
   GLhalfNV y = cmd->y;
   CALL_Vertex2hNV(ctx->Dispatch.Current, (x, y));
   return align(sizeof(struct marshal_cmd_Vertex2hNV), 8) / 8;
}
void GLAPIENTRY
_mesa_marshal_Vertex2hNV(GLhalfNV x, GLhalfNV y)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Vertex2hNV);
   struct marshal_cmd_Vertex2hNV *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Vertex2hNV, cmd_size);
   cmd->x = x;
   cmd->y = y;
}


/* Vertex2hvNV: marshalled asynchronously */
struct marshal_cmd_Vertex2hvNV
{
   struct marshal_cmd_base cmd_base;
   GLhalfNV v[2];
};
uint32_t _mesa_unmarshal_Vertex2hvNV(struct gl_context *ctx, const struct marshal_cmd_Vertex2hvNV *restrict cmd)
{
   const GLhalfNV *v = cmd->v;
   CALL_Vertex2hvNV(ctx->Dispatch.Current, (v));
   return align(sizeof(struct marshal_cmd_Vertex2hvNV), 8) / 8;
}
void GLAPIENTRY
_mesa_marshal_Vertex2hvNV(const GLhalfNV *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Vertex2hvNV);
   struct marshal_cmd_Vertex2hvNV *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Vertex2hvNV, cmd_size);
   memcpy(cmd->v, v, 2 * sizeof(GLhalfNV));
}


/* Vertex3hNV: marshalled asynchronously */
struct marshal_cmd_Vertex3hNV
{
   struct marshal_cmd_base cmd_base;
   GLhalfNV x;
   GLhalfNV y;
   GLhalfNV z;
};
uint32_t _mesa_unmarshal_Vertex3hNV(struct gl_context *ctx, const struct marshal_cmd_Vertex3hNV *restrict cmd)
{
   GLhalfNV x = cmd->x;
   GLhalfNV y = cmd->y;
   GLhalfNV z = cmd->z;
   CALL_Vertex3hNV(ctx->Dispatch.Current, (x, y, z));
   return align(sizeof(struct marshal_cmd_Vertex3hNV), 8) / 8;
}
void GLAPIENTRY
_mesa_marshal_Vertex3hNV(GLhalfNV x, GLhalfNV y, GLhalfNV z)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Vertex3hNV);
   struct marshal_cmd_Vertex3hNV *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Vertex3hNV, cmd_size);
   cmd->x = x;
   cmd->y = y;
   cmd->z = z;
}


/* Vertex3hvNV: marshalled asynchronously */
struct marshal_cmd_Vertex3hvNV
{
   struct marshal_cmd_base cmd_base;
   GLhalfNV v[3];
};
uint32_t _mesa_unmarshal_Vertex3hvNV(struct gl_context *ctx, const struct marshal_cmd_Vertex3hvNV *restrict cmd)
{
   const GLhalfNV *v = cmd->v;
   CALL_Vertex3hvNV(ctx->Dispatch.Current, (v));
   return align(sizeof(struct marshal_cmd_Vertex3hvNV), 8) / 8;
}
void GLAPIENTRY
_mesa_marshal_Vertex3hvNV(const GLhalfNV *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Vertex3hvNV);
   struct marshal_cmd_Vertex3hvNV *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Vertex3hvNV, cmd_size);
   memcpy(cmd->v, v, 3 * sizeof(GLhalfNV));
}


/* Vertex4hNV: marshalled asynchronously */
struct marshal_cmd_Vertex4hNV
{
   struct marshal_cmd_base cmd_base;
   GLhalfNV x;
   GLhalfNV y;
   GLhalfNV z;
   GLhalfNV w;
};
uint32_t _mesa_unmarshal_Vertex4hNV(struct gl_context *ctx, const struct marshal_cmd_Vertex4hNV *restrict cmd)
{
   GLhalfNV x = cmd->x;
   GLhalfNV y = cmd->y;
   GLhalfNV z = cmd->z;
   GLhalfNV w = cmd->w;
   CALL_Vertex4hNV(ctx->Dispatch.Current, (x, y, z, w));
   return align(sizeof(struct marshal_cmd_Vertex4hNV), 8) / 8;
}
void GLAPIENTRY
_mesa_marshal_Vertex4hNV(GLhalfNV x, GLhalfNV y, GLhalfNV z, GLhalfNV w)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Vertex4hNV);
   struct marshal_cmd_Vertex4hNV *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Vertex4hNV, cmd_size);
   cmd->x = x;
   cmd->y = y;
   cmd->z = z;
   cmd->w = w;
}


/* Vertex4hvNV: marshalled asynchronously */
struct marshal_cmd_Vertex4hvNV
{
   struct marshal_cmd_base cmd_base;
   GLhalfNV v[4];
};
uint32_t _mesa_unmarshal_Vertex4hvNV(struct gl_context *ctx, const struct marshal_cmd_Vertex4hvNV *restrict cmd)
{
   const GLhalfNV *v = cmd->v;
   CALL_Vertex4hvNV(ctx->Dispatch.Current, (v));
   return align(sizeof(struct marshal_cmd_Vertex4hvNV), 8) / 8;
}
void GLAPIENTRY
_mesa_marshal_Vertex4hvNV(const GLhalfNV *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Vertex4hvNV);
   struct marshal_cmd_Vertex4hvNV *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Vertex4hvNV, cmd_size);
   memcpy(cmd->v, v, 4 * sizeof(GLhalfNV));
}


/* Normal3hNV: marshalled asynchronously */
struct marshal_cmd_Normal3hNV
{
   struct marshal_cmd_base cmd_base;
   GLhalfNV nx;
   GLhalfNV ny;
   GLhalfNV nz;
};
uint32_t _mesa_unmarshal_Normal3hNV(struct gl_context *ctx, const struct marshal_cmd_Normal3hNV *restrict cmd)
{
   GLhalfNV nx = cmd->nx;
   GLhalfNV ny = cmd->ny;
   GLhalfNV nz = cmd->nz;
   CALL_Normal3hNV(ctx->Dispatch.Current, (nx, ny, nz));
   return align(sizeof(struct marshal_cmd_Normal3hNV), 8) / 8;
}
void GLAPIENTRY
_mesa_marshal_Normal3hNV(GLhalfNV nx, GLhalfNV ny, GLhalfNV nz)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Normal3hNV);
   struct marshal_cmd_Normal3hNV *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Normal3hNV, cmd_size);
   cmd->nx = nx;
   cmd->ny = ny;
   cmd->nz = nz;
}


/* Normal3hvNV: marshalled asynchronously */
struct marshal_cmd_Normal3hvNV
{
   struct marshal_cmd_base cmd_base;
   GLhalfNV v[3];
};
uint32_t _mesa_unmarshal_Normal3hvNV(struct gl_context *ctx, const struct marshal_cmd_Normal3hvNV *restrict cmd)
{
   const GLhalfNV *v = cmd->v;
   CALL_Normal3hvNV(ctx->Dispatch.Current, (v));
   return align(sizeof(struct marshal_cmd_Normal3hvNV), 8) / 8;
}
void GLAPIENTRY
_mesa_marshal_Normal3hvNV(const GLhalfNV *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Normal3hvNV);
   struct marshal_cmd_Normal3hvNV *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Normal3hvNV, cmd_size);
   memcpy(cmd->v, v, 3 * sizeof(GLhalfNV));
}


/* Color3hNV: marshalled asynchronously */
struct marshal_cmd_Color3hNV
{
   struct marshal_cmd_base cmd_base;
   GLhalfNV red;
   GLhalfNV green;
   GLhalfNV blue;
};
uint32_t _mesa_unmarshal_Color3hNV(struct gl_context *ctx, const struct marshal_cmd_Color3hNV *restrict cmd)
{
   GLhalfNV red = cmd->red;
   GLhalfNV green = cmd->green;
   GLhalfNV blue = cmd->blue;
   CALL_Color3hNV(ctx->Dispatch.Current, (red, green, blue));
   return align(sizeof(struct marshal_cmd_Color3hNV), 8) / 8;
}
void GLAPIENTRY
_mesa_marshal_Color3hNV(GLhalfNV red, GLhalfNV green, GLhalfNV blue)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Color3hNV);
   struct marshal_cmd_Color3hNV *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Color3hNV, cmd_size);
   cmd->red = red;
   cmd->green = green;
   cmd->blue = blue;
}


/* Color3hvNV: marshalled asynchronously */
struct marshal_cmd_Color3hvNV
{
   struct marshal_cmd_base cmd_base;
   GLhalfNV v[3];
};
uint32_t _mesa_unmarshal_Color3hvNV(struct gl_context *ctx, const struct marshal_cmd_Color3hvNV *restrict cmd)
{
   const GLhalfNV *v = cmd->v;
   CALL_Color3hvNV(ctx->Dispatch.Current, (v));
   return align(sizeof(struct marshal_cmd_Color3hvNV), 8) / 8;
}
void GLAPIENTRY
_mesa_marshal_Color3hvNV(const GLhalfNV *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Color3hvNV);
   struct marshal_cmd_Color3hvNV *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Color3hvNV, cmd_size);
   memcpy(cmd->v, v, 3 * sizeof(GLhalfNV));
}


/* Color4hNV: marshalled asynchronously */
struct marshal_cmd_Color4hNV
{
   struct marshal_cmd_base cmd_base;
   GLhalfNV red;
   GLhalfNV green;
   GLhalfNV blue;
   GLhalfNV alpha;
};
uint32_t _mesa_unmarshal_Color4hNV(struct gl_context *ctx, const struct marshal_cmd_Color4hNV *restrict cmd)
{
   GLhalfNV red = cmd->red;
   GLhalfNV green = cmd->green;
   GLhalfNV blue = cmd->blue;
   GLhalfNV alpha = cmd->alpha;
   CALL_Color4hNV(ctx->Dispatch.Current, (red, green, blue, alpha));
   return align(sizeof(struct marshal_cmd_Color4hNV), 8) / 8;
}
void GLAPIENTRY
_mesa_marshal_Color4hNV(GLhalfNV red, GLhalfNV green, GLhalfNV blue, GLhalfNV alpha)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Color4hNV);
   struct marshal_cmd_Color4hNV *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Color4hNV, cmd_size);
   cmd->red = red;
   cmd->green = green;
   cmd->blue = blue;
   cmd->alpha = alpha;
}


/* Color4hvNV: marshalled asynchronously */
struct marshal_cmd_Color4hvNV
{
   struct marshal_cmd_base cmd_base;
   GLhalfNV v[4];
};
uint32_t _mesa_unmarshal_Color4hvNV(struct gl_context *ctx, const struct marshal_cmd_Color4hvNV *restrict cmd)
{
   const GLhalfNV *v = cmd->v;
   CALL_Color4hvNV(ctx->Dispatch.Current, (v));
   return align(sizeof(struct marshal_cmd_Color4hvNV), 8) / 8;
}
void GLAPIENTRY
_mesa_marshal_Color4hvNV(const GLhalfNV *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Color4hvNV);
   struct marshal_cmd_Color4hvNV *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Color4hvNV, cmd_size);
   memcpy(cmd->v, v, 4 * sizeof(GLhalfNV));
}


/* TexCoord1hNV: marshalled asynchronously */
struct marshal_cmd_TexCoord1hNV
{
   struct marshal_cmd_base cmd_base;
   GLhalfNV s;
};
uint32_t _mesa_unmarshal_TexCoord1hNV(struct gl_context *ctx, const struct marshal_cmd_TexCoord1hNV *restrict cmd)
{
   GLhalfNV s = cmd->s;
   CALL_TexCoord1hNV(ctx->Dispatch.Current, (s));
   return align(sizeof(struct marshal_cmd_TexCoord1hNV), 8) / 8;
}
void GLAPIENTRY
_mesa_marshal_TexCoord1hNV(GLhalfNV s)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TexCoord1hNV);
   struct marshal_cmd_TexCoord1hNV *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexCoord1hNV, cmd_size);
   cmd->s = s;
}


/* TexCoord1hvNV: marshalled asynchronously */
struct marshal_cmd_TexCoord1hvNV
{
   struct marshal_cmd_base cmd_base;
   GLhalfNV v[1];
};
uint32_t _mesa_unmarshal_TexCoord1hvNV(struct gl_context *ctx, const struct marshal_cmd_TexCoord1hvNV *restrict cmd)
{
   const GLhalfNV *v = cmd->v;
   CALL_TexCoord1hvNV(ctx->Dispatch.Current, (v));
   return align(sizeof(struct marshal_cmd_TexCoord1hvNV), 8) / 8;
}
void GLAPIENTRY
_mesa_marshal_TexCoord1hvNV(const GLhalfNV *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TexCoord1hvNV);
   struct marshal_cmd_TexCoord1hvNV *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexCoord1hvNV, cmd_size);
   memcpy(cmd->v, v, 1 * sizeof(GLhalfNV));
}


/* TexCoord2hNV: marshalled asynchronously */
struct marshal_cmd_TexCoord2hNV
{
   struct marshal_cmd_base cmd_base;
   GLhalfNV s;
   GLhalfNV t;
};
uint32_t _mesa_unmarshal_TexCoord2hNV(struct gl_context *ctx, const struct marshal_cmd_TexCoord2hNV *restrict cmd)
{
   GLhalfNV s = cmd->s;
   GLhalfNV t = cmd->t;
   CALL_TexCoord2hNV(ctx->Dispatch.Current, (s, t));
   return align(sizeof(struct marshal_cmd_TexCoord2hNV), 8) / 8;
}
void GLAPIENTRY
_mesa_marshal_TexCoord2hNV(GLhalfNV s, GLhalfNV t)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TexCoord2hNV);
   struct marshal_cmd_TexCoord2hNV *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexCoord2hNV, cmd_size);
   cmd->s = s;
   cmd->t = t;
}


/* TexCoord2hvNV: marshalled asynchronously */
struct marshal_cmd_TexCoord2hvNV
{
   struct marshal_cmd_base cmd_base;
   GLhalfNV v[2];
};
uint32_t _mesa_unmarshal_TexCoord2hvNV(struct gl_context *ctx, const struct marshal_cmd_TexCoord2hvNV *restrict cmd)
{
   const GLhalfNV *v = cmd->v;
   CALL_TexCoord2hvNV(ctx->Dispatch.Current, (v));
   return align(sizeof(struct marshal_cmd_TexCoord2hvNV), 8) / 8;
}
void GLAPIENTRY
_mesa_marshal_TexCoord2hvNV(const GLhalfNV *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TexCoord2hvNV);
   struct marshal_cmd_TexCoord2hvNV *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexCoord2hvNV, cmd_size);
   memcpy(cmd->v, v, 2 * sizeof(GLhalfNV));
}


/* TexCoord3hNV: marshalled asynchronously */
struct marshal_cmd_TexCoord3hNV
{
   struct marshal_cmd_base cmd_base;
   GLhalfNV s;
   GLhalfNV t;
   GLhalfNV r;
};
uint32_t _mesa_unmarshal_TexCoord3hNV(struct gl_context *ctx, const struct marshal_cmd_TexCoord3hNV *restrict cmd)
{
   GLhalfNV s = cmd->s;
   GLhalfNV t = cmd->t;
   GLhalfNV r = cmd->r;
   CALL_TexCoord3hNV(ctx->Dispatch.Current, (s, t, r));
   return align(sizeof(struct marshal_cmd_TexCoord3hNV), 8) / 8;
}
void GLAPIENTRY
_mesa_marshal_TexCoord3hNV(GLhalfNV s, GLhalfNV t, GLhalfNV r)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TexCoord3hNV);
   struct marshal_cmd_TexCoord3hNV *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexCoord3hNV, cmd_size);
   cmd->s = s;
   cmd->t = t;
   cmd->r = r;
}


/* TexCoord3hvNV: marshalled asynchronously */
struct marshal_cmd_TexCoord3hvNV
{
   struct marshal_cmd_base cmd_base;
   GLhalfNV v[3];
};
uint32_t _mesa_unmarshal_TexCoord3hvNV(struct gl_context *ctx, const struct marshal_cmd_TexCoord3hvNV *restrict cmd)
{
   const GLhalfNV *v = cmd->v;
   CALL_TexCoord3hvNV(ctx->Dispatch.Current, (v));
   return align(sizeof(struct marshal_cmd_TexCoord3hvNV), 8) / 8;
}
void GLAPIENTRY
_mesa_marshal_TexCoord3hvNV(const GLhalfNV *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TexCoord3hvNV);
   struct marshal_cmd_TexCoord3hvNV *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexCoord3hvNV, cmd_size);
   memcpy(cmd->v, v, 3 * sizeof(GLhalfNV));
}


/* TexCoord4hNV: marshalled asynchronously */
struct marshal_cmd_TexCoord4hNV
{
   struct marshal_cmd_base cmd_base;
   GLhalfNV s;
   GLhalfNV t;
   GLhalfNV r;
   GLhalfNV q;
};
uint32_t _mesa_unmarshal_TexCoord4hNV(struct gl_context *ctx, const struct marshal_cmd_TexCoord4hNV *restrict cmd)
{
   GLhalfNV s = cmd->s;
   GLhalfNV t = cmd->t;
   GLhalfNV r = cmd->r;
   GLhalfNV q = cmd->q;
   CALL_TexCoord4hNV(ctx->Dispatch.Current, (s, t, r, q));
   return align(sizeof(struct marshal_cmd_TexCoord4hNV), 8) / 8;
}
void GLAPIENTRY
_mesa_marshal_TexCoord4hNV(GLhalfNV s, GLhalfNV t, GLhalfNV r, GLhalfNV q)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TexCoord4hNV);
   struct marshal_cmd_TexCoord4hNV *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexCoord4hNV, cmd_size);
   cmd->s = s;
   cmd->t = t;
   cmd->r = r;
   cmd->q = q;
}


/* TexCoord4hvNV: marshalled asynchronously */
struct marshal_cmd_TexCoord4hvNV
{
   struct marshal_cmd_base cmd_base;
   GLhalfNV v[4];
};
uint32_t _mesa_unmarshal_TexCoord4hvNV(struct gl_context *ctx, const struct marshal_cmd_TexCoord4hvNV *restrict cmd)
{
   const GLhalfNV *v = cmd->v;
   CALL_TexCoord4hvNV(ctx->Dispatch.Current, (v));
   return align(sizeof(struct marshal_cmd_TexCoord4hvNV), 8) / 8;
}
void GLAPIENTRY
_mesa_marshal_TexCoord4hvNV(const GLhalfNV *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TexCoord4hvNV);
   struct marshal_cmd_TexCoord4hvNV *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexCoord4hvNV, cmd_size);
   memcpy(cmd->v, v, 4 * sizeof(GLhalfNV));
}


/* MultiTexCoord1hNV: marshalled asynchronously */
struct marshal_cmd_MultiTexCoord1hNV
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLhalfNV s;
};
uint32_t _mesa_unmarshal_MultiTexCoord1hNV(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord1hNV *restrict cmd)
{
   GLenum16 target = cmd->target;
   GLhalfNV s = cmd->s;
   CALL_MultiTexCoord1hNV(ctx->Dispatch.Current, (target, s));
   return align(sizeof(struct marshal_cmd_MultiTexCoord1hNV), 8) / 8;
}
void GLAPIENTRY
_mesa_marshal_MultiTexCoord1hNV(GLenum target, GLhalfNV s)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexCoord1hNV);
   struct marshal_cmd_MultiTexCoord1hNV *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexCoord1hNV, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->s = s;
}


/* MultiTexCoord1hvNV: marshalled asynchronously */
struct marshal_cmd_MultiTexCoord1hvNV
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLhalfNV v[1];
};
uint32_t _mesa_unmarshal_MultiTexCoord1hvNV(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord1hvNV *restrict cmd)
{
   GLenum16 target = cmd->target;
   const GLhalfNV *v = cmd->v;
   CALL_MultiTexCoord1hvNV(ctx->Dispatch.Current, (target, v));
   return align(sizeof(struct marshal_cmd_MultiTexCoord1hvNV), 8) / 8;
}
void GLAPIENTRY
_mesa_marshal_MultiTexCoord1hvNV(GLenum target, const GLhalfNV *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexCoord1hvNV);
   struct marshal_cmd_MultiTexCoord1hvNV *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexCoord1hvNV, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   memcpy(cmd->v, v, 1 * sizeof(GLhalfNV));
}


/* MultiTexCoord2hNV: marshalled asynchronously */
struct marshal_cmd_MultiTexCoord2hNV
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLhalfNV s;
   GLhalfNV t;
};
uint32_t _mesa_unmarshal_MultiTexCoord2hNV(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord2hNV *restrict cmd)
{
   GLenum16 target = cmd->target;
   GLhalfNV s = cmd->s;
   GLhalfNV t = cmd->t;
   CALL_MultiTexCoord2hNV(ctx->Dispatch.Current, (target, s, t));
   return align(sizeof(struct marshal_cmd_MultiTexCoord2hNV), 8) / 8;
}
void GLAPIENTRY
_mesa_marshal_MultiTexCoord2hNV(GLenum target, GLhalfNV s, GLhalfNV t)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexCoord2hNV);
   struct marshal_cmd_MultiTexCoord2hNV *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexCoord2hNV, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->s = s;
   cmd->t = t;
}


/* MultiTexCoord2hvNV: marshalled asynchronously */
struct marshal_cmd_MultiTexCoord2hvNV
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLhalfNV v[2];
};
uint32_t _mesa_unmarshal_MultiTexCoord2hvNV(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord2hvNV *restrict cmd)
{
   GLenum16 target = cmd->target;
   const GLhalfNV *v = cmd->v;
   CALL_MultiTexCoord2hvNV(ctx->Dispatch.Current, (target, v));
   return align(sizeof(struct marshal_cmd_MultiTexCoord2hvNV), 8) / 8;
}
void GLAPIENTRY
_mesa_marshal_MultiTexCoord2hvNV(GLenum target, const GLhalfNV *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexCoord2hvNV);
   struct marshal_cmd_MultiTexCoord2hvNV *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexCoord2hvNV, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   memcpy(cmd->v, v, 2 * sizeof(GLhalfNV));
}


/* MultiTexCoord3hNV: marshalled asynchronously */
struct marshal_cmd_MultiTexCoord3hNV
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLhalfNV s;
   GLhalfNV t;
   GLhalfNV r;
};
uint32_t _mesa_unmarshal_MultiTexCoord3hNV(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord3hNV *restrict cmd)
{
   GLenum16 target = cmd->target;
   GLhalfNV s = cmd->s;
   GLhalfNV t = cmd->t;
   GLhalfNV r = cmd->r;
   CALL_MultiTexCoord3hNV(ctx->Dispatch.Current, (target, s, t, r));
   return align(sizeof(struct marshal_cmd_MultiTexCoord3hNV), 8) / 8;
}
void GLAPIENTRY
_mesa_marshal_MultiTexCoord3hNV(GLenum target, GLhalfNV s, GLhalfNV t, GLhalfNV r)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexCoord3hNV);
   struct marshal_cmd_MultiTexCoord3hNV *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexCoord3hNV, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->s = s;
   cmd->t = t;
   cmd->r = r;
}


/* MultiTexCoord3hvNV: marshalled asynchronously */
struct marshal_cmd_MultiTexCoord3hvNV
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLhalfNV v[3];
};
uint32_t _mesa_unmarshal_MultiTexCoord3hvNV(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord3hvNV *restrict cmd)
{
   GLenum16 target = cmd->target;
   const GLhalfNV *v = cmd->v;
   CALL_MultiTexCoord3hvNV(ctx->Dispatch.Current, (target, v));
   return align(sizeof(struct marshal_cmd_MultiTexCoord3hvNV), 8) / 8;
}
void GLAPIENTRY
_mesa_marshal_MultiTexCoord3hvNV(GLenum target, const GLhalfNV *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexCoord3hvNV);
   struct marshal_cmd_MultiTexCoord3hvNV *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexCoord3hvNV, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   memcpy(cmd->v, v, 3 * sizeof(GLhalfNV));
}


/* MultiTexCoord4hNV: marshalled asynchronously */
struct marshal_cmd_MultiTexCoord4hNV
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLhalfNV s;
   GLhalfNV t;
   GLhalfNV r;
   GLhalfNV q;
};
uint32_t _mesa_unmarshal_MultiTexCoord4hNV(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord4hNV *restrict cmd)
{
   GLenum16 target = cmd->target;
   GLhalfNV s = cmd->s;
   GLhalfNV t = cmd->t;
   GLhalfNV r = cmd->r;
   GLhalfNV q = cmd->q;
   CALL_MultiTexCoord4hNV(ctx->Dispatch.Current, (target, s, t, r, q));
   return align(sizeof(struct marshal_cmd_MultiTexCoord4hNV), 8) / 8;
}
void GLAPIENTRY
_mesa_marshal_MultiTexCoord4hNV(GLenum target, GLhalfNV s, GLhalfNV t, GLhalfNV r, GLhalfNV q)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexCoord4hNV);
   struct marshal_cmd_MultiTexCoord4hNV *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexCoord4hNV, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->s = s;
   cmd->t = t;
   cmd->r = r;
   cmd->q = q;
}


/* MultiTexCoord4hvNV: marshalled asynchronously */
struct marshal_cmd_MultiTexCoord4hvNV
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLhalfNV v[4];
};
uint32_t _mesa_unmarshal_MultiTexCoord4hvNV(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord4hvNV *restrict cmd)
{
   GLenum16 target = cmd->target;
   const GLhalfNV *v = cmd->v;
   CALL_MultiTexCoord4hvNV(ctx->Dispatch.Current, (target, v));
   return align(sizeof(struct marshal_cmd_MultiTexCoord4hvNV), 8) / 8;
}
void GLAPIENTRY
_mesa_marshal_MultiTexCoord4hvNV(GLenum target, const GLhalfNV *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexCoord4hvNV);
   struct marshal_cmd_MultiTexCoord4hvNV *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexCoord4hvNV, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   memcpy(cmd->v, v, 4 * sizeof(GLhalfNV));
}


/* VertexAttrib1hNV: marshalled asynchronously */
struct marshal_cmd_VertexAttrib1hNV
{
   struct marshal_cmd_base cmd_base;
   GLhalfNV x;
   GLuint index;
};
uint32_t _mesa_unmarshal_VertexAttrib1hNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib1hNV *restrict cmd)
{
   GLhalfNV x = cmd->x;
   GLuint index = cmd->index;
   CALL_VertexAttrib1hNV(ctx->Dispatch.Current, (index, x));
   return align(sizeof(struct marshal_cmd_VertexAttrib1hNV), 8) / 8;
}
void GLAPIENTRY
_mesa_marshal_VertexAttrib1hNV(GLuint index, GLhalfNV x)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttrib1hNV);
   struct marshal_cmd_VertexAttrib1hNV *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttrib1hNV, cmd_size);
   cmd->x = x;
   cmd->index = index;
}


/* VertexAttrib1hvNV: marshalled asynchronously */
struct marshal_cmd_VertexAttrib1hvNV
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLhalfNV v[1];
};
uint32_t _mesa_unmarshal_VertexAttrib1hvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib1hvNV *restrict cmd)
{
   GLuint index = cmd->index;
   const GLhalfNV *v = cmd->v;
   CALL_VertexAttrib1hvNV(ctx->Dispatch.Current, (index, v));
   return align(sizeof(struct marshal_cmd_VertexAttrib1hvNV), 8) / 8;
}
void GLAPIENTRY
_mesa_marshal_VertexAttrib1hvNV(GLuint index, const GLhalfNV *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttrib1hvNV);
   struct marshal_cmd_VertexAttrib1hvNV *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttrib1hvNV, cmd_size);
   cmd->index = index;
   memcpy(cmd->v, v, 1 * sizeof(GLhalfNV));
}


/* VertexAttrib2hNV: marshalled asynchronously */
struct marshal_cmd_VertexAttrib2hNV
{
   struct marshal_cmd_base cmd_base;
   GLhalfNV x;
   GLhalfNV y;
   GLuint index;
};
uint32_t _mesa_unmarshal_VertexAttrib2hNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib2hNV *restrict cmd)
{
   GLhalfNV x = cmd->x;
   GLhalfNV y = cmd->y;
   GLuint index = cmd->index;
   CALL_VertexAttrib2hNV(ctx->Dispatch.Current, (index, x, y));
   return align(sizeof(struct marshal_cmd_VertexAttrib2hNV), 8) / 8;
}
void GLAPIENTRY
_mesa_marshal_VertexAttrib2hNV(GLuint index, GLhalfNV x, GLhalfNV y)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttrib2hNV);
   struct marshal_cmd_VertexAttrib2hNV *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttrib2hNV, cmd_size);
   cmd->x = x;
   cmd->y = y;
   cmd->index = index;
}


/* VertexAttrib2hvNV: marshalled asynchronously */
struct marshal_cmd_VertexAttrib2hvNV
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLhalfNV v[2];
};
uint32_t _mesa_unmarshal_VertexAttrib2hvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib2hvNV *restrict cmd)
{
   GLuint index = cmd->index;
   const GLhalfNV *v = cmd->v;
   CALL_VertexAttrib2hvNV(ctx->Dispatch.Current, (index, v));
   return align(sizeof(struct marshal_cmd_VertexAttrib2hvNV), 8) / 8;
}
void GLAPIENTRY
_mesa_marshal_VertexAttrib2hvNV(GLuint index, const GLhalfNV *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttrib2hvNV);
   struct marshal_cmd_VertexAttrib2hvNV *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttrib2hvNV, cmd_size);
   cmd->index = index;
   memcpy(cmd->v, v, 2 * sizeof(GLhalfNV));
}


/* VertexAttrib3hNV: marshalled asynchronously */
struct marshal_cmd_VertexAttrib3hNV
{
   struct marshal_cmd_base cmd_base;
   GLhalfNV x;
   GLhalfNV y;
   GLhalfNV z;
   GLuint index;
};
uint32_t _mesa_unmarshal_VertexAttrib3hNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib3hNV *restrict cmd)
{
   GLhalfNV x = cmd->x;
   GLhalfNV y = cmd->y;
   GLhalfNV z = cmd->z;
   GLuint index = cmd->index;
   CALL_VertexAttrib3hNV(ctx->Dispatch.Current, (index, x, y, z));
   return align(sizeof(struct marshal_cmd_VertexAttrib3hNV), 8) / 8;
}
void GLAPIENTRY
_mesa_marshal_VertexAttrib3hNV(GLuint index, GLhalfNV x, GLhalfNV y, GLhalfNV z)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttrib3hNV);
   struct marshal_cmd_VertexAttrib3hNV *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttrib3hNV, cmd_size);
   cmd->x = x;
   cmd->y = y;
   cmd->z = z;
   cmd->index = index;
}


/* VertexAttrib3hvNV: marshalled asynchronously */
struct marshal_cmd_VertexAttrib3hvNV
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLhalfNV v[3];
};
uint32_t _mesa_unmarshal_VertexAttrib3hvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib3hvNV *restrict cmd)
{
   GLuint index = cmd->index;
   const GLhalfNV *v = cmd->v;
   CALL_VertexAttrib3hvNV(ctx->Dispatch.Current, (index, v));
   return align(sizeof(struct marshal_cmd_VertexAttrib3hvNV), 8) / 8;
}
void GLAPIENTRY
_mesa_marshal_VertexAttrib3hvNV(GLuint index, const GLhalfNV *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttrib3hvNV);
   struct marshal_cmd_VertexAttrib3hvNV *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttrib3hvNV, cmd_size);
   cmd->index = index;
   memcpy(cmd->v, v, 3 * sizeof(GLhalfNV));
}


/* VertexAttrib4hNV: marshalled asynchronously */
struct marshal_cmd_VertexAttrib4hNV
{
   struct marshal_cmd_base cmd_base;
   GLhalfNV x;
   GLhalfNV y;
   GLhalfNV z;
   GLhalfNV w;
   GLuint index;
};
uint32_t _mesa_unmarshal_VertexAttrib4hNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4hNV *restrict cmd)
{
   GLhalfNV x = cmd->x;
   GLhalfNV y = cmd->y;
   GLhalfNV z = cmd->z;
   GLhalfNV w = cmd->w;
   GLuint index = cmd->index;
   CALL_VertexAttrib4hNV(ctx->Dispatch.Current, (index, x, y, z, w));
   return align(sizeof(struct marshal_cmd_VertexAttrib4hNV), 8) / 8;
}
void GLAPIENTRY
_mesa_marshal_VertexAttrib4hNV(GLuint index, GLhalfNV x, GLhalfNV y, GLhalfNV z, GLhalfNV w)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttrib4hNV);
   struct marshal_cmd_VertexAttrib4hNV *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttrib4hNV, cmd_size);
   cmd->x = x;
   cmd->y = y;
   cmd->z = z;
   cmd->w = w;
   cmd->index = index;
}


/* VertexAttrib4hvNV: marshalled asynchronously */
struct marshal_cmd_VertexAttrib4hvNV
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLhalfNV v[4];
};
uint32_t _mesa_unmarshal_VertexAttrib4hvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4hvNV *restrict cmd)
{
   GLuint index = cmd->index;
   const GLhalfNV *v = cmd->v;
   CALL_VertexAttrib4hvNV(ctx->Dispatch.Current, (index, v));
   return align(sizeof(struct marshal_cmd_VertexAttrib4hvNV), 8) / 8;
}
void GLAPIENTRY
_mesa_marshal_VertexAttrib4hvNV(GLuint index, const GLhalfNV *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttrib4hvNV);
   struct marshal_cmd_VertexAttrib4hvNV *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttrib4hvNV, cmd_size);
   cmd->index = index;
   memcpy(cmd->v, v, 4 * sizeof(GLhalfNV));
}


/* VertexAttribs1hvNV: marshalled asynchronously */
struct marshal_cmd_VertexAttribs1hvNV
{
   struct marshal_cmd_base cmd_base;
   uint16_t num_slots;
   GLuint index;
   GLsizei n;
   /* Next safe_mul(n, 1 * sizeof(GLhalfNV)) bytes are GLhalfNV v[n] */
};
uint32_t _mesa_unmarshal_VertexAttribs1hvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttribs1hvNV *restrict cmd)
{
   GLuint index = cmd->index;
   GLsizei n = cmd->n;
   GLhalfNV *v;
   const char *variable_data = (const char *) (cmd + 1);
   v = (GLhalfNV *) variable_data;
   CALL_VertexAttribs1hvNV(ctx->Dispatch.Current, (index, n, v));
   return cmd->num_slots;
}
void GLAPIENTRY
_mesa_marshal_VertexAttribs1hvNV(GLuint index, GLsizei n, const GLhalfNV *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int v_size = safe_mul(n, 1 * sizeof(GLhalfNV));
   int cmd_size = sizeof(struct marshal_cmd_VertexAttribs1hvNV) + v_size;
   if (unlikely(v_size < 0 || (v_size > 0 && !v) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "VertexAttribs1hvNV");
      CALL_VertexAttribs1hvNV(ctx->Dispatch.Current, (index, n, v));
      return;
   }
   struct marshal_cmd_VertexAttribs1hvNV *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttribs1hvNV, cmd_size);
   cmd->num_slots = align(cmd_size, 8) / 8;
   cmd->index = index;
   cmd->n = n;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, v, v_size);
}


/* VertexAttribs2hvNV: marshalled asynchronously */
struct marshal_cmd_VertexAttribs2hvNV
{
   struct marshal_cmd_base cmd_base;
   uint16_t num_slots;
   GLuint index;
   GLsizei n;
   /* Next safe_mul(n, 2 * sizeof(GLhalfNV)) bytes are GLhalfNV v[n][2] */
};
uint32_t _mesa_unmarshal_VertexAttribs2hvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttribs2hvNV *restrict cmd)
{
   GLuint index = cmd->index;
   GLsizei n = cmd->n;
   GLhalfNV *v;
   const char *variable_data = (const char *) (cmd + 1);
   v = (GLhalfNV *) variable_data;
   CALL_VertexAttribs2hvNV(ctx->Dispatch.Current, (index, n, v));
   return cmd->num_slots;
}
void GLAPIENTRY
_mesa_marshal_VertexAttribs2hvNV(GLuint index, GLsizei n, const GLhalfNV *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int v_size = safe_mul(n, 2 * sizeof(GLhalfNV));
   int cmd_size = sizeof(struct marshal_cmd_VertexAttribs2hvNV) + v_size;
   if (unlikely(v_size < 0 || (v_size > 0 && !v) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "VertexAttribs2hvNV");
      CALL_VertexAttribs2hvNV(ctx->Dispatch.Current, (index, n, v));
      return;
   }
   struct marshal_cmd_VertexAttribs2hvNV *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttribs2hvNV, cmd_size);
   cmd->num_slots = align(cmd_size, 8) / 8;
   cmd->index = index;
   cmd->n = n;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, v, v_size);
}


/* VertexAttribs3hvNV: marshalled asynchronously */
struct marshal_cmd_VertexAttribs3hvNV
{
   struct marshal_cmd_base cmd_base;
   uint16_t num_slots;
   GLuint index;
   GLsizei n;
   /* Next safe_mul(n, 3 * sizeof(GLhalfNV)) bytes are GLhalfNV v[n][3] */
};
uint32_t _mesa_unmarshal_VertexAttribs3hvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttribs3hvNV *restrict cmd)
{
   GLuint index = cmd->index;
   GLsizei n = cmd->n;
   GLhalfNV *v;
   const char *variable_data = (const char *) (cmd + 1);
   v = (GLhalfNV *) variable_data;
   CALL_VertexAttribs3hvNV(ctx->Dispatch.Current, (index, n, v));
   return cmd->num_slots;
}
void GLAPIENTRY
_mesa_marshal_VertexAttribs3hvNV(GLuint index, GLsizei n, const GLhalfNV *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int v_size = safe_mul(n, 3 * sizeof(GLhalfNV));
   int cmd_size = sizeof(struct marshal_cmd_VertexAttribs3hvNV) + v_size;
   if (unlikely(v_size < 0 || (v_size > 0 && !v) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "VertexAttribs3hvNV");
      CALL_VertexAttribs3hvNV(ctx->Dispatch.Current, (index, n, v));
      return;
   }
   struct marshal_cmd_VertexAttribs3hvNV *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttribs3hvNV, cmd_size);
   cmd->num_slots = align(cmd_size, 8) / 8;
   cmd->index = index;
   cmd->n = n;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, v, v_size);
}


/* VertexAttribs4hvNV: marshalled asynchronously */
struct marshal_cmd_VertexAttribs4hvNV
{
   struct marshal_cmd_base cmd_base;
   uint16_t num_slots;
   GLuint index;
   GLsizei n;
   /* Next safe_mul(n, 4 * sizeof(GLhalfNV)) bytes are GLhalfNV v[n][4] */
};
uint32_t _mesa_unmarshal_VertexAttribs4hvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttribs4hvNV *restrict cmd)
{
   GLuint index = cmd->index;
   GLsizei n = cmd->n;
   GLhalfNV *v;
   const char *variable_data = (const char *) (cmd + 1);
   v = (GLhalfNV *) variable_data;
   CALL_VertexAttribs4hvNV(ctx->Dispatch.Current, (index, n, v));
   return cmd->num_slots;
}
void GLAPIENTRY
_mesa_marshal_VertexAttribs4hvNV(GLuint index, GLsizei n, const GLhalfNV *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int v_size = safe_mul(n, 4 * sizeof(GLhalfNV));
   int cmd_size = sizeof(struct marshal_cmd_VertexAttribs4hvNV) + v_size;
   if (unlikely(v_size < 0 || (v_size > 0 && !v) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "VertexAttribs4hvNV");
      CALL_VertexAttribs4hvNV(ctx->Dispatch.Current, (index, n, v));
      return;
   }
   struct marshal_cmd_VertexAttribs4hvNV *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttribs4hvNV, cmd_size);
   cmd->num_slots = align(cmd_size, 8) / 8;
   cmd->index = index;
   cmd->n = n;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, v, v_size);
}


/* FogCoordhNV: marshalled asynchronously */
struct marshal_cmd_FogCoordhNV
{
   struct marshal_cmd_base cmd_base;
   GLhalfNV x;
};
uint32_t _mesa_unmarshal_FogCoordhNV(struct gl_context *ctx, const struct marshal_cmd_FogCoordhNV *restrict cmd)
{
   GLhalfNV x = cmd->x;
   CALL_FogCoordhNV(ctx->Dispatch.Current, (x));
   return align(sizeof(struct marshal_cmd_FogCoordhNV), 8) / 8;
}
void GLAPIENTRY
_mesa_marshal_FogCoordhNV(GLhalfNV x)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_FogCoordhNV);
   struct marshal_cmd_FogCoordhNV *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_FogCoordhNV, cmd_size);
   cmd->x = x;
}


/* FogCoordhvNV: marshalled asynchronously */
struct marshal_cmd_FogCoordhvNV
{
   struct marshal_cmd_base cmd_base;
   GLhalfNV v[1];
};
uint32_t _mesa_unmarshal_FogCoordhvNV(struct gl_context *ctx, const struct marshal_cmd_FogCoordhvNV *restrict cmd)
{
   const GLhalfNV *v = cmd->v;
   CALL_FogCoordhvNV(ctx->Dispatch.Current, (v));
   return align(sizeof(struct marshal_cmd_FogCoordhvNV), 8) / 8;
}
void GLAPIENTRY
_mesa_marshal_FogCoordhvNV(const GLhalfNV *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_FogCoordhvNV);
   struct marshal_cmd_FogCoordhvNV *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_FogCoordhvNV, cmd_size);
   memcpy(cmd->v, v, 1 * sizeof(GLhalfNV));
}


/* SecondaryColor3hNV: marshalled asynchronously */
struct marshal_cmd_SecondaryColor3hNV
{
   struct marshal_cmd_base cmd_base;
   GLhalfNV red;
   GLhalfNV green;
   GLhalfNV blue;
};
uint32_t _mesa_unmarshal_SecondaryColor3hNV(struct gl_context *ctx, const struct marshal_cmd_SecondaryColor3hNV *restrict cmd)
{
   GLhalfNV red = cmd->red;
   GLhalfNV green = cmd->green;
   GLhalfNV blue = cmd->blue;
   CALL_SecondaryColor3hNV(ctx->Dispatch.Current, (red, green, blue));
   return align(sizeof(struct marshal_cmd_SecondaryColor3hNV), 8) / 8;
}
void GLAPIENTRY
_mesa_marshal_SecondaryColor3hNV(GLhalfNV red, GLhalfNV green, GLhalfNV blue)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_SecondaryColor3hNV);
   struct marshal_cmd_SecondaryColor3hNV *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_SecondaryColor3hNV, cmd_size);
   cmd->red = red;
   cmd->green = green;
   cmd->blue = blue;
}


/* SecondaryColor3hvNV: marshalled asynchronously */
struct marshal_cmd_SecondaryColor3hvNV
{
   struct marshal_cmd_base cmd_base;
   GLhalfNV v[3];
};
uint32_t _mesa_unmarshal_SecondaryColor3hvNV(struct gl_context *ctx, const struct marshal_cmd_SecondaryColor3hvNV *restrict cmd)
{
   const GLhalfNV *v = cmd->v;
   CALL_SecondaryColor3hvNV(ctx->Dispatch.Current, (v));
   return align(sizeof(struct marshal_cmd_SecondaryColor3hvNV), 8) / 8;
}
void GLAPIENTRY
_mesa_marshal_SecondaryColor3hvNV(const GLhalfNV *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_SecondaryColor3hvNV);
   struct marshal_cmd_SecondaryColor3hvNV *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_SecondaryColor3hvNV, cmd_size);
   memcpy(cmd->v, v, 3 * sizeof(GLhalfNV));
}


/* MemoryBarrierByRegion: marshalled asynchronously */
struct marshal_cmd_MemoryBarrierByRegion
{
   struct marshal_cmd_base cmd_base;
   GLbitfield barriers;
};
uint32_t _mesa_unmarshal_MemoryBarrierByRegion(struct gl_context *ctx, const struct marshal_cmd_MemoryBarrierByRegion *restrict cmd)
{
   GLbitfield barriers = cmd->barriers;
   CALL_MemoryBarrierByRegion(ctx->Dispatch.Current, (barriers));
   return align(sizeof(struct marshal_cmd_MemoryBarrierByRegion), 8) / 8;
}
static void GLAPIENTRY
_mesa_marshal_MemoryBarrierByRegion(GLbitfield barriers)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MemoryBarrierByRegion);
   struct marshal_cmd_MemoryBarrierByRegion *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MemoryBarrierByRegion, cmd_size);
   cmd->barriers = barriers;
}


/* AlphaFuncx: marshalled asynchronously */
struct marshal_cmd_AlphaFuncx
{
   struct marshal_cmd_base cmd_base;
   GLenum16 func;
   GLclampx ref;
};
uint32_t _mesa_unmarshal_AlphaFuncx(struct gl_context *ctx, const struct marshal_cmd_AlphaFuncx *restrict cmd)
{
   GLenum16 func = cmd->func;
   GLclampx ref = cmd->ref;
   CALL_AlphaFuncx(ctx->Dispatch.Current, (func, ref));
   return align(sizeof(struct marshal_cmd_AlphaFuncx), 8) / 8;
}
static void GLAPIENTRY
_mesa_marshal_AlphaFuncx(GLenum func, GLclampx ref)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_AlphaFuncx);
   struct marshal_cmd_AlphaFuncx *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_AlphaFuncx, cmd_size);
   cmd->func = MIN2(func, 0xffff); /* clamped to 0xffff (invalid enum) */
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
uint32_t _mesa_unmarshal_ClearColorx(struct gl_context *ctx, const struct marshal_cmd_ClearColorx *restrict cmd)
{
   GLclampx red = cmd->red;
   GLclampx green = cmd->green;
   GLclampx blue = cmd->blue;
   GLclampx alpha = cmd->alpha;
   CALL_ClearColorx(ctx->Dispatch.Current, (red, green, blue, alpha));
   return align(sizeof(struct marshal_cmd_ClearColorx), 8) / 8;
}
static void GLAPIENTRY
_mesa_marshal_ClearColorx(GLclampx red, GLclampx green, GLclampx blue, GLclampx alpha)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ClearColorx);
   struct marshal_cmd_ClearColorx *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ClearColorx, cmd_size);
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
uint32_t _mesa_unmarshal_ClearDepthx(struct gl_context *ctx, const struct marshal_cmd_ClearDepthx *restrict cmd)
{
   GLclampx depth = cmd->depth;
   CALL_ClearDepthx(ctx->Dispatch.Current, (depth));
   return align(sizeof(struct marshal_cmd_ClearDepthx), 8) / 8;
}
static void GLAPIENTRY
_mesa_marshal_ClearDepthx(GLclampx depth)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ClearDepthx);
   struct marshal_cmd_ClearDepthx *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ClearDepthx, cmd_size);
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
uint32_t _mesa_unmarshal_Color4x(struct gl_context *ctx, const struct marshal_cmd_Color4x *restrict cmd)
{
   GLfixed red = cmd->red;
   GLfixed green = cmd->green;
   GLfixed blue = cmd->blue;
   GLfixed alpha = cmd->alpha;
   CALL_Color4x(ctx->Dispatch.Current, (red, green, blue, alpha));
   return align(sizeof(struct marshal_cmd_Color4x), 8) / 8;
}
static void GLAPIENTRY
_mesa_marshal_Color4x(GLfixed red, GLfixed green, GLfixed blue, GLfixed alpha)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Color4x);
   struct marshal_cmd_Color4x *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Color4x, cmd_size);
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
uint32_t _mesa_unmarshal_DepthRangex(struct gl_context *ctx, const struct marshal_cmd_DepthRangex *restrict cmd)
{
   GLclampx zNear = cmd->zNear;
   GLclampx zFar = cmd->zFar;
   CALL_DepthRangex(ctx->Dispatch.Current, (zNear, zFar));
   return align(sizeof(struct marshal_cmd_DepthRangex), 8) / 8;
}
static void GLAPIENTRY
_mesa_marshal_DepthRangex(GLclampx zNear, GLclampx zFar)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_DepthRangex);
   struct marshal_cmd_DepthRangex *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_DepthRangex, cmd_size);
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
uint32_t _mesa_unmarshal_Fogx(struct gl_context *ctx, const struct marshal_cmd_Fogx *restrict cmd)
{
   GLenum16 pname = cmd->pname;
   GLfixed param = cmd->param;
   CALL_Fogx(ctx->Dispatch.Current, (pname, param));
   return align(sizeof(struct marshal_cmd_Fogx), 8) / 8;
}
static void GLAPIENTRY
_mesa_marshal_Fogx(GLenum pname, GLfixed param)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Fogx);
   struct marshal_cmd_Fogx *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Fogx, cmd_size);
   cmd->pname = MIN2(pname, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->param = param;
}


/* Fogxv: marshalled asynchronously */
struct marshal_cmd_Fogxv
{
   struct marshal_cmd_base cmd_base;
   uint16_t num_slots;
   GLenum16 pname;
   /* Next _mesa_fog_enum_to_count(pname) * 1 * sizeof(GLfixed) bytes are GLfixed params[None] */
};
uint32_t _mesa_unmarshal_Fogxv(struct gl_context *ctx, const struct marshal_cmd_Fogxv *restrict cmd)
{
   GLenum16 pname = cmd->pname;
   GLfixed *params;
   const char *variable_data = (const char *) (cmd + 1);
   params = (GLfixed *) variable_data;
   CALL_Fogxv(ctx->Dispatch.Current, (pname, params));
   return cmd->num_slots;
}
static void GLAPIENTRY
_mesa_marshal_Fogxv(GLenum pname, const GLfixed *params)
{
   GET_CURRENT_CONTEXT(ctx);
   int params_size = _mesa_fog_enum_to_count(pname) * 1 * sizeof(GLfixed);
   int cmd_size = sizeof(struct marshal_cmd_Fogxv) + params_size;
   assert(cmd_size >= 0 && cmd_size <= MARSHAL_MAX_CMD_SIZE);
   struct marshal_cmd_Fogxv *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Fogxv, cmd_size);
   cmd->num_slots = align(cmd_size, 8) / 8;
   cmd->pname = MIN2(pname, 0xffff); /* clamped to 0xffff (invalid enum) */
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
uint32_t _mesa_unmarshal_Frustumx(struct gl_context *ctx, const struct marshal_cmd_Frustumx *restrict cmd)
{
   GLfixed left = cmd->left;
   GLfixed right = cmd->right;
   GLfixed bottom = cmd->bottom;
   GLfixed top = cmd->top;
   GLfixed zNear = cmd->zNear;
   GLfixed zFar = cmd->zFar;
   CALL_Frustumx(ctx->Dispatch.Current, (left, right, bottom, top, zNear, zFar));
   return align(sizeof(struct marshal_cmd_Frustumx), 8) / 8;
}
static void GLAPIENTRY
_mesa_marshal_Frustumx(GLfixed left, GLfixed right, GLfixed bottom, GLfixed top, GLfixed zNear, GLfixed zFar)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Frustumx);
   struct marshal_cmd_Frustumx *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Frustumx, cmd_size);
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
uint32_t _mesa_unmarshal_LightModelx(struct gl_context *ctx, const struct marshal_cmd_LightModelx *restrict cmd)
{
   GLenum16 pname = cmd->pname;
   GLfixed param = cmd->param;
   CALL_LightModelx(ctx->Dispatch.Current, (pname, param));
   return align(sizeof(struct marshal_cmd_LightModelx), 8) / 8;
}
static void GLAPIENTRY
_mesa_marshal_LightModelx(GLenum pname, GLfixed param)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_LightModelx);
   struct marshal_cmd_LightModelx *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_LightModelx, cmd_size);
   cmd->pname = MIN2(pname, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->param = param;
}


/* LightModelxv: marshalled asynchronously */
struct marshal_cmd_LightModelxv
{
   struct marshal_cmd_base cmd_base;
   uint16_t num_slots;
   GLenum16 pname;
   /* Next _mesa_light_model_enum_to_count(pname) * 1 * sizeof(GLfixed) bytes are GLfixed params[None] */
};
uint32_t _mesa_unmarshal_LightModelxv(struct gl_context *ctx, const struct marshal_cmd_LightModelxv *restrict cmd)
{
   GLenum16 pname = cmd->pname;
   GLfixed *params;
   const char *variable_data = (const char *) (cmd + 1);
   params = (GLfixed *) variable_data;
   CALL_LightModelxv(ctx->Dispatch.Current, (pname, params));
   return cmd->num_slots;
}
static void GLAPIENTRY
_mesa_marshal_LightModelxv(GLenum pname, const GLfixed *params)
{
   GET_CURRENT_CONTEXT(ctx);
   int params_size = _mesa_light_model_enum_to_count(pname) * 1 * sizeof(GLfixed);
   int cmd_size = sizeof(struct marshal_cmd_LightModelxv) + params_size;
   assert(cmd_size >= 0 && cmd_size <= MARSHAL_MAX_CMD_SIZE);
   struct marshal_cmd_LightModelxv *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_LightModelxv, cmd_size);
   cmd->num_slots = align(cmd_size, 8) / 8;
   cmd->pname = MIN2(pname, 0xffff); /* clamped to 0xffff (invalid enum) */
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
uint32_t _mesa_unmarshal_Lightx(struct gl_context *ctx, const struct marshal_cmd_Lightx *restrict cmd)
{
   GLenum16 light = cmd->light;
   GLenum16 pname = cmd->pname;
   GLfixed param = cmd->param;
   CALL_Lightx(ctx->Dispatch.Current, (light, pname, param));
   return align(sizeof(struct marshal_cmd_Lightx), 8) / 8;
}
static void GLAPIENTRY
_mesa_marshal_Lightx(GLenum light, GLenum pname, GLfixed param)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Lightx);
   struct marshal_cmd_Lightx *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Lightx, cmd_size);
   cmd->light = MIN2(light, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->pname = MIN2(pname, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->param = param;
}


/* Lightxv: marshalled asynchronously */
struct marshal_cmd_Lightxv
{
   struct marshal_cmd_base cmd_base;
   uint16_t num_slots;
   GLenum16 light;
   GLenum16 pname;
   /* Next _mesa_light_enum_to_count(pname) * 1 * sizeof(GLfixed) bytes are GLfixed params[None] */
};
uint32_t _mesa_unmarshal_Lightxv(struct gl_context *ctx, const struct marshal_cmd_Lightxv *restrict cmd)
{
   GLenum16 light = cmd->light;
   GLenum16 pname = cmd->pname;
   GLfixed *params;
   const char *variable_data = (const char *) (cmd + 1);
   params = (GLfixed *) variable_data;
   CALL_Lightxv(ctx->Dispatch.Current, (light, pname, params));
   return cmd->num_slots;
}
static void GLAPIENTRY
_mesa_marshal_Lightxv(GLenum light, GLenum pname, const GLfixed *params)
{
   GET_CURRENT_CONTEXT(ctx);
   int params_size = _mesa_light_enum_to_count(pname) * 1 * sizeof(GLfixed);
   int cmd_size = sizeof(struct marshal_cmd_Lightxv) + params_size;
   assert(cmd_size >= 0 && cmd_size <= MARSHAL_MAX_CMD_SIZE);
   struct marshal_cmd_Lightxv *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Lightxv, cmd_size);
   cmd->num_slots = align(cmd_size, 8) / 8;
   cmd->light = MIN2(light, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->pname = MIN2(pname, 0xffff); /* clamped to 0xffff (invalid enum) */
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, params, params_size);
}


/* LineWidthx: marshalled asynchronously */
struct marshal_cmd_LineWidthx
{
   struct marshal_cmd_base cmd_base;
   GLfixed width;
};
uint32_t _mesa_unmarshal_LineWidthx(struct gl_context *ctx, const struct marshal_cmd_LineWidthx *restrict cmd)
{
   GLfixed width = cmd->width;
   CALL_LineWidthx(ctx->Dispatch.Current, (width));
   return align(sizeof(struct marshal_cmd_LineWidthx), 8) / 8;
}
static void GLAPIENTRY
_mesa_marshal_LineWidthx(GLfixed width)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_LineWidthx);
   struct marshal_cmd_LineWidthx *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_LineWidthx, cmd_size);
   cmd->width = width;
}


/* LoadMatrixx: marshalled asynchronously */
struct marshal_cmd_LoadMatrixx
{
   struct marshal_cmd_base cmd_base;
   GLfixed m[16];
};
uint32_t _mesa_unmarshal_LoadMatrixx(struct gl_context *ctx, const struct marshal_cmd_LoadMatrixx *restrict cmd)
{
   const GLfixed *m = cmd->m;
   CALL_LoadMatrixx(ctx->Dispatch.Current, (m));
   return align(sizeof(struct marshal_cmd_LoadMatrixx), 8) / 8;
}
static void GLAPIENTRY
_mesa_marshal_LoadMatrixx(const GLfixed *m)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_LoadMatrixx);
   struct marshal_cmd_LoadMatrixx *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_LoadMatrixx, cmd_size);
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
uint32_t _mesa_unmarshal_Materialx(struct gl_context *ctx, const struct marshal_cmd_Materialx *restrict cmd)
{
   GLenum16 face = cmd->face;
   GLenum16 pname = cmd->pname;
   GLfixed param = cmd->param;
   CALL_Materialx(ctx->Dispatch.Current, (face, pname, param));
   return align(sizeof(struct marshal_cmd_Materialx), 8) / 8;
}
static void GLAPIENTRY
_mesa_marshal_Materialx(GLenum face, GLenum pname, GLfixed param)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Materialx);
   struct marshal_cmd_Materialx *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Materialx, cmd_size);
   cmd->face = MIN2(face, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->pname = MIN2(pname, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->param = param;
}


/* Materialxv: marshalled asynchronously */
struct marshal_cmd_Materialxv
{
   struct marshal_cmd_base cmd_base;
   uint16_t num_slots;
   GLenum16 face;
   GLenum16 pname;
   /* Next _mesa_material_enum_to_count(pname) * 1 * sizeof(GLfixed) bytes are GLfixed params[None] */
};
uint32_t _mesa_unmarshal_Materialxv(struct gl_context *ctx, const struct marshal_cmd_Materialxv *restrict cmd)
{
   GLenum16 face = cmd->face;
   GLenum16 pname = cmd->pname;
   GLfixed *params;
   const char *variable_data = (const char *) (cmd + 1);
   params = (GLfixed *) variable_data;
   CALL_Materialxv(ctx->Dispatch.Current, (face, pname, params));
   return cmd->num_slots;
}
static void GLAPIENTRY
_mesa_marshal_Materialxv(GLenum face, GLenum pname, const GLfixed *params)
{
   GET_CURRENT_CONTEXT(ctx);
   int params_size = _mesa_material_enum_to_count(pname) * 1 * sizeof(GLfixed);
   int cmd_size = sizeof(struct marshal_cmd_Materialxv) + params_size;
   assert(cmd_size >= 0 && cmd_size <= MARSHAL_MAX_CMD_SIZE);
   struct marshal_cmd_Materialxv *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Materialxv, cmd_size);
   cmd->num_slots = align(cmd_size, 8) / 8;
   cmd->face = MIN2(face, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->pname = MIN2(pname, 0xffff); /* clamped to 0xffff (invalid enum) */
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, params, params_size);
}


/* MultMatrixx: marshalled asynchronously */
struct marshal_cmd_MultMatrixx
{
   struct marshal_cmd_base cmd_base;
   GLfixed m[16];
};
uint32_t _mesa_unmarshal_MultMatrixx(struct gl_context *ctx, const struct marshal_cmd_MultMatrixx *restrict cmd)
{
   const GLfixed *m = cmd->m;
   CALL_MultMatrixx(ctx->Dispatch.Current, (m));
   return align(sizeof(struct marshal_cmd_MultMatrixx), 8) / 8;
}
static void GLAPIENTRY
_mesa_marshal_MultMatrixx(const GLfixed *m)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultMatrixx);
   struct marshal_cmd_MultMatrixx *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultMatrixx, cmd_size);
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
uint32_t _mesa_unmarshal_MultiTexCoord4x(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord4x *restrict cmd)
{
   GLenum16 target = cmd->target;
   GLfixed s = cmd->s;
   GLfixed t = cmd->t;
   GLfixed r = cmd->r;
   GLfixed q = cmd->q;
   CALL_MultiTexCoord4x(ctx->Dispatch.Current, (target, s, t, r, q));
   return align(sizeof(struct marshal_cmd_MultiTexCoord4x), 8) / 8;
}
static void GLAPIENTRY
_mesa_marshal_MultiTexCoord4x(GLenum target, GLfixed s, GLfixed t, GLfixed r, GLfixed q)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexCoord4x);
   struct marshal_cmd_MultiTexCoord4x *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexCoord4x, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
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
uint32_t _mesa_unmarshal_Normal3x(struct gl_context *ctx, const struct marshal_cmd_Normal3x *restrict cmd)
{
   GLfixed nx = cmd->nx;
   GLfixed ny = cmd->ny;
   GLfixed nz = cmd->nz;
   CALL_Normal3x(ctx->Dispatch.Current, (nx, ny, nz));
   return align(sizeof(struct marshal_cmd_Normal3x), 8) / 8;
}
static void GLAPIENTRY
_mesa_marshal_Normal3x(GLfixed nx, GLfixed ny, GLfixed nz)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Normal3x);
   struct marshal_cmd_Normal3x *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Normal3x, cmd_size);
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
uint32_t _mesa_unmarshal_Orthox(struct gl_context *ctx, const struct marshal_cmd_Orthox *restrict cmd)
{
   GLfixed left = cmd->left;
   GLfixed right = cmd->right;
   GLfixed bottom = cmd->bottom;
   GLfixed top = cmd->top;
   GLfixed zNear = cmd->zNear;
   GLfixed zFar = cmd->zFar;
   CALL_Orthox(ctx->Dispatch.Current, (left, right, bottom, top, zNear, zFar));
   return align(sizeof(struct marshal_cmd_Orthox), 8) / 8;
}
static void GLAPIENTRY
_mesa_marshal_Orthox(GLfixed left, GLfixed right, GLfixed bottom, GLfixed top, GLfixed zNear, GLfixed zFar)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Orthox);
   struct marshal_cmd_Orthox *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Orthox, cmd_size);
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
uint32_t _mesa_unmarshal_PointSizex(struct gl_context *ctx, const struct marshal_cmd_PointSizex *restrict cmd)
{
   GLfixed size = cmd->size;
   CALL_PointSizex(ctx->Dispatch.Current, (size));
   return align(sizeof(struct marshal_cmd_PointSizex), 8) / 8;
}
static void GLAPIENTRY
_mesa_marshal_PointSizex(GLfixed size)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_PointSizex);
   struct marshal_cmd_PointSizex *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_PointSizex, cmd_size);
   cmd->size = size;
}


/* PolygonOffsetx: marshalled asynchronously */
struct marshal_cmd_PolygonOffsetx
{
   struct marshal_cmd_base cmd_base;
   GLfixed factor;
   GLfixed units;
};
uint32_t _mesa_unmarshal_PolygonOffsetx(struct gl_context *ctx, const struct marshal_cmd_PolygonOffsetx *restrict cmd)
{
   GLfixed factor = cmd->factor;
   GLfixed units = cmd->units;
   CALL_PolygonOffsetx(ctx->Dispatch.Current, (factor, units));
   return align(sizeof(struct marshal_cmd_PolygonOffsetx), 8) / 8;
}
static void GLAPIENTRY
_mesa_marshal_PolygonOffsetx(GLfixed factor, GLfixed units)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_PolygonOffsetx);
   struct marshal_cmd_PolygonOffsetx *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_PolygonOffsetx, cmd_size);
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
uint32_t _mesa_unmarshal_Rotatex(struct gl_context *ctx, const struct marshal_cmd_Rotatex *restrict cmd)
{
   GLfixed angle = cmd->angle;
   GLfixed x = cmd->x;
   GLfixed y = cmd->y;
   GLfixed z = cmd->z;
   CALL_Rotatex(ctx->Dispatch.Current, (angle, x, y, z));
   return align(sizeof(struct marshal_cmd_Rotatex), 8) / 8;
}
static void GLAPIENTRY
_mesa_marshal_Rotatex(GLfixed angle, GLfixed x, GLfixed y, GLfixed z)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Rotatex);
   struct marshal_cmd_Rotatex *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Rotatex, cmd_size);
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
uint32_t _mesa_unmarshal_SampleCoveragex(struct gl_context *ctx, const struct marshal_cmd_SampleCoveragex *restrict cmd)
{
   GLboolean invert = cmd->invert;
   GLclampx value = cmd->value;
   CALL_SampleCoveragex(ctx->Dispatch.Current, (value, invert));
   return align(sizeof(struct marshal_cmd_SampleCoveragex), 8) / 8;
}
static void GLAPIENTRY
_mesa_marshal_SampleCoveragex(GLclampx value, GLboolean invert)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_SampleCoveragex);
   struct marshal_cmd_SampleCoveragex *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_SampleCoveragex, cmd_size);
   cmd->invert = invert;
   cmd->value = value;
}


/* Scalex: marshalled asynchronously */
struct marshal_cmd_Scalex
{
   struct marshal_cmd_base cmd_base;
   GLfixed x;
   GLfixed y;
   GLfixed z;
};
uint32_t _mesa_unmarshal_Scalex(struct gl_context *ctx, const struct marshal_cmd_Scalex *restrict cmd)
{
   GLfixed x = cmd->x;
   GLfixed y = cmd->y;
   GLfixed z = cmd->z;
   CALL_Scalex(ctx->Dispatch.Current, (x, y, z));
   return align(sizeof(struct marshal_cmd_Scalex), 8) / 8;
}
static void GLAPIENTRY
_mesa_marshal_Scalex(GLfixed x, GLfixed y, GLfixed z)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Scalex);
   struct marshal_cmd_Scalex *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Scalex, cmd_size);
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
uint32_t _mesa_unmarshal_TexEnvx(struct gl_context *ctx, const struct marshal_cmd_TexEnvx *restrict cmd)
{
   GLenum16 target = cmd->target;
   GLenum16 pname = cmd->pname;
   GLfixed param = cmd->param;
   CALL_TexEnvx(ctx->Dispatch.Current, (target, pname, param));
   return align(sizeof(struct marshal_cmd_TexEnvx), 8) / 8;
}
static void GLAPIENTRY
_mesa_marshal_TexEnvx(GLenum target, GLenum pname, GLfixed param)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TexEnvx);
   struct marshal_cmd_TexEnvx *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexEnvx, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->pname = MIN2(pname, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->param = param;
}


/* TexEnvxv: marshalled asynchronously */
struct marshal_cmd_TexEnvxv
{
   struct marshal_cmd_base cmd_base;
   uint16_t num_slots;
   GLenum16 target;
   GLenum16 pname;
   /* Next _mesa_texenv_enum_to_count(pname) * 1 * sizeof(GLfixed) bytes are GLfixed params[None] */
};
uint32_t _mesa_unmarshal_TexEnvxv(struct gl_context *ctx, const struct marshal_cmd_TexEnvxv *restrict cmd)
{
   GLenum16 target = cmd->target;
   GLenum16 pname = cmd->pname;
   GLfixed *params;
   const char *variable_data = (const char *) (cmd + 1);
   params = (GLfixed *) variable_data;
   CALL_TexEnvxv(ctx->Dispatch.Current, (target, pname, params));
   return cmd->num_slots;
}
static void GLAPIENTRY
_mesa_marshal_TexEnvxv(GLenum target, GLenum pname, const GLfixed *params)
{
   GET_CURRENT_CONTEXT(ctx);
   int params_size = _mesa_texenv_enum_to_count(pname) * 1 * sizeof(GLfixed);
   int cmd_size = sizeof(struct marshal_cmd_TexEnvxv) + params_size;
   assert(cmd_size >= 0 && cmd_size <= MARSHAL_MAX_CMD_SIZE);
   struct marshal_cmd_TexEnvxv *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexEnvxv, cmd_size);
   cmd->num_slots = align(cmd_size, 8) / 8;
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->pname = MIN2(pname, 0xffff); /* clamped to 0xffff (invalid enum) */
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
uint32_t _mesa_unmarshal_TexParameterx(struct gl_context *ctx, const struct marshal_cmd_TexParameterx *restrict cmd)
{
   GLenum16 target = cmd->target;
   GLenum16 pname = cmd->pname;
   GLfixed param = cmd->param;
   CALL_TexParameterx(ctx->Dispatch.Current, (target, pname, param));
   return align(sizeof(struct marshal_cmd_TexParameterx), 8) / 8;
}
static void GLAPIENTRY
_mesa_marshal_TexParameterx(GLenum target, GLenum pname, GLfixed param)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TexParameterx);
   struct marshal_cmd_TexParameterx *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexParameterx, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->pname = MIN2(pname, 0xffff); /* clamped to 0xffff (invalid enum) */
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
uint32_t _mesa_unmarshal_Translatex(struct gl_context *ctx, const struct marshal_cmd_Translatex *restrict cmd)
{
   GLfixed x = cmd->x;
   GLfixed y = cmd->y;
   GLfixed z = cmd->z;
   CALL_Translatex(ctx->Dispatch.Current, (x, y, z));
   return align(sizeof(struct marshal_cmd_Translatex), 8) / 8;
}
static void GLAPIENTRY
_mesa_marshal_Translatex(GLfixed x, GLfixed y, GLfixed z)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Translatex);
   struct marshal_cmd_Translatex *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Translatex, cmd_size);
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
uint32_t _mesa_unmarshal_ClipPlanex(struct gl_context *ctx, const struct marshal_cmd_ClipPlanex *restrict cmd)
{
   GLenum16 plane = cmd->plane;
   const GLfixed *equation = cmd->equation;
   CALL_ClipPlanex(ctx->Dispatch.Current, (plane, equation));
   return align(sizeof(struct marshal_cmd_ClipPlanex), 8) / 8;
}
static void GLAPIENTRY
_mesa_marshal_ClipPlanex(GLenum plane, const GLfixed *equation)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ClipPlanex);
   struct marshal_cmd_ClipPlanex *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ClipPlanex, cmd_size);
   cmd->plane = MIN2(plane, 0xffff); /* clamped to 0xffff (invalid enum) */
   memcpy(cmd->equation, equation, 4 * sizeof(GLfixed));
}


/* GetClipPlanex: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetClipPlanex(GLenum plane, GLfixed *equation)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetClipPlanex");
   CALL_GetClipPlanex(ctx->Dispatch.Current, (plane, equation));
}


/* GetFixedv: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetFixedv(GLenum pname, GLfixed *params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetFixedv");
   CALL_GetFixedv(ctx->Dispatch.Current, (pname, params));
}


/* GetLightxv: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetLightxv(GLenum light, GLenum pname, GLfixed *params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetLightxv");
   CALL_GetLightxv(ctx->Dispatch.Current, (light, pname, params));
}


/* GetMaterialxv: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetMaterialxv(GLenum face, GLenum pname, GLfixed *params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetMaterialxv");
   CALL_GetMaterialxv(ctx->Dispatch.Current, (face, pname, params));
}


/* GetTexEnvxv: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetTexEnvxv(GLenum target, GLenum pname, GLfixed *params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetTexEnvxv");
   CALL_GetTexEnvxv(ctx->Dispatch.Current, (target, pname, params));
}


/* GetTexParameterxv: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetTexParameterxv(GLenum target, GLenum pname, GLfixed *params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetTexParameterxv");
   CALL_GetTexParameterxv(ctx->Dispatch.Current, (target, pname, params));
}


/* PointParameterx: marshalled asynchronously */
struct marshal_cmd_PointParameterx
{
   struct marshal_cmd_base cmd_base;
   GLenum16 pname;
   GLfixed param;
};
uint32_t _mesa_unmarshal_PointParameterx(struct gl_context *ctx, const struct marshal_cmd_PointParameterx *restrict cmd)
{
   GLenum16 pname = cmd->pname;
   GLfixed param = cmd->param;
   CALL_PointParameterx(ctx->Dispatch.Current, (pname, param));
   return align(sizeof(struct marshal_cmd_PointParameterx), 8) / 8;
}
static void GLAPIENTRY
_mesa_marshal_PointParameterx(GLenum pname, GLfixed param)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_PointParameterx);
   struct marshal_cmd_PointParameterx *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_PointParameterx, cmd_size);
   cmd->pname = MIN2(pname, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->param = param;
}


/* PointParameterxv: marshalled asynchronously */
struct marshal_cmd_PointParameterxv
{
   struct marshal_cmd_base cmd_base;
   uint16_t num_slots;
   GLenum16 pname;
   /* Next _mesa_point_param_enum_to_count(pname) * 1 * sizeof(GLfixed) bytes are GLfixed params[None] */
};
uint32_t _mesa_unmarshal_PointParameterxv(struct gl_context *ctx, const struct marshal_cmd_PointParameterxv *restrict cmd)
{
   GLenum16 pname = cmd->pname;
   GLfixed *params;
   const char *variable_data = (const char *) (cmd + 1);
   params = (GLfixed *) variable_data;
   CALL_PointParameterxv(ctx->Dispatch.Current, (pname, params));
   return cmd->num_slots;
}
static void GLAPIENTRY
_mesa_marshal_PointParameterxv(GLenum pname, const GLfixed *params)
{
   GET_CURRENT_CONTEXT(ctx);
   int params_size = _mesa_point_param_enum_to_count(pname) * 1 * sizeof(GLfixed);
   int cmd_size = sizeof(struct marshal_cmd_PointParameterxv) + params_size;
   assert(cmd_size >= 0 && cmd_size <= MARSHAL_MAX_CMD_SIZE);
   struct marshal_cmd_PointParameterxv *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_PointParameterxv, cmd_size);
   cmd->num_slots = align(cmd_size, 8) / 8;
   cmd->pname = MIN2(pname, 0xffff); /* clamped to 0xffff (invalid enum) */
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, params, params_size);
}


/* TexParameterxv: marshalled asynchronously */
struct marshal_cmd_TexParameterxv
{
   struct marshal_cmd_base cmd_base;
   uint16_t num_slots;
   GLenum16 target;
   GLenum16 pname;
   /* Next _mesa_tex_param_enum_to_count(pname) * 1 * sizeof(GLfixed) bytes are GLfixed params[None] */
};
uint32_t _mesa_unmarshal_TexParameterxv(struct gl_context *ctx, const struct marshal_cmd_TexParameterxv *restrict cmd)
{
   GLenum16 target = cmd->target;
   GLenum16 pname = cmd->pname;
   GLfixed *params;
   const char *variable_data = (const char *) (cmd + 1);
   params = (GLfixed *) variable_data;
   CALL_TexParameterxv(ctx->Dispatch.Current, (target, pname, params));
   return cmd->num_slots;
}
static void GLAPIENTRY
_mesa_marshal_TexParameterxv(GLenum target, GLenum pname, const GLfixed *params)
{
   GET_CURRENT_CONTEXT(ctx);
   int params_size = _mesa_tex_param_enum_to_count(pname) * 1 * sizeof(GLfixed);
   int cmd_size = sizeof(struct marshal_cmd_TexParameterxv) + params_size;
   assert(cmd_size >= 0 && cmd_size <= MARSHAL_MAX_CMD_SIZE);
   struct marshal_cmd_TexParameterxv *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexParameterxv, cmd_size);
   cmd->num_slots = align(cmd_size, 8) / 8;
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->pname = MIN2(pname, 0xffff); /* clamped to 0xffff (invalid enum) */
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, params, params_size);
}


/* GetTexGenxvOES: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetTexGenxvOES(GLenum coord, GLenum pname, GLfixed *params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetTexGenxvOES");
   CALL_GetTexGenxvOES(ctx->Dispatch.Current, (coord, pname, params));
}


/* TexGenxOES: marshalled asynchronously */
struct marshal_cmd_TexGenxOES
{
   struct marshal_cmd_base cmd_base;
   GLenum16 coord;
   GLenum16 pname;
   GLfixed param;
};
uint32_t _mesa_unmarshal_TexGenxOES(struct gl_context *ctx, const struct marshal_cmd_TexGenxOES *restrict cmd)
{
   GLenum16 coord = cmd->coord;
   GLenum16 pname = cmd->pname;
   GLfixed param = cmd->param;
   CALL_TexGenxOES(ctx->Dispatch.Current, (coord, pname, param));
   return align(sizeof(struct marshal_cmd_TexGenxOES), 8) / 8;
}
static void GLAPIENTRY
_mesa_marshal_TexGenxOES(GLenum coord, GLenum pname, GLfixed param)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TexGenxOES);
   struct marshal_cmd_TexGenxOES *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexGenxOES, cmd_size);
   cmd->coord = MIN2(coord, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->pname = MIN2(pname, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->param = param;
}


/* TexGenxvOES: marshalled asynchronously */
struct marshal_cmd_TexGenxvOES
{
   struct marshal_cmd_base cmd_base;
   uint16_t num_slots;
   GLenum16 coord;
   GLenum16 pname;
   /* Next _mesa_texgen_enum_to_count(pname) * 1 * sizeof(GLfixed) bytes are GLfixed params[None] */
};
uint32_t _mesa_unmarshal_TexGenxvOES(struct gl_context *ctx, const struct marshal_cmd_TexGenxvOES *restrict cmd)
{
   GLenum16 coord = cmd->coord;
   GLenum16 pname = cmd->pname;
   GLfixed *params;
   const char *variable_data = (const char *) (cmd + 1);
   params = (GLfixed *) variable_data;
   CALL_TexGenxvOES(ctx->Dispatch.Current, (coord, pname, params));
   return cmd->num_slots;
}
static void GLAPIENTRY
_mesa_marshal_TexGenxvOES(GLenum coord, GLenum pname, const GLfixed *params)
{
   GET_CURRENT_CONTEXT(ctx);
   int params_size = _mesa_texgen_enum_to_count(pname) * 1 * sizeof(GLfixed);
   int cmd_size = sizeof(struct marshal_cmd_TexGenxvOES) + params_size;
   assert(cmd_size >= 0 && cmd_size <= MARSHAL_MAX_CMD_SIZE);
   struct marshal_cmd_TexGenxvOES *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexGenxvOES, cmd_size);
   cmd->num_slots = align(cmd_size, 8) / 8;
   cmd->coord = MIN2(coord, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->pname = MIN2(pname, 0xffff); /* clamped to 0xffff (invalid enum) */
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
uint32_t _mesa_unmarshal_ClipPlanef(struct gl_context *ctx, const struct marshal_cmd_ClipPlanef *restrict cmd)
{
   GLenum16 plane = cmd->plane;
   const GLfloat *equation = cmd->equation;
   CALL_ClipPlanef(ctx->Dispatch.Current, (plane, equation));
   return align(sizeof(struct marshal_cmd_ClipPlanef), 8) / 8;
}
static void GLAPIENTRY
_mesa_marshal_ClipPlanef(GLenum plane, const GLfloat *equation)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ClipPlanef);
   struct marshal_cmd_ClipPlanef *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ClipPlanef, cmd_size);
   cmd->plane = MIN2(plane, 0xffff); /* clamped to 0xffff (invalid enum) */
   memcpy(cmd->equation, equation, 4 * sizeof(GLfloat));
}


/* GetClipPlanef: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetClipPlanef(GLenum plane, GLfloat *equation)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetClipPlanef");
   CALL_GetClipPlanef(ctx->Dispatch.Current, (plane, equation));
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
uint32_t _mesa_unmarshal_Frustumf(struct gl_context *ctx, const struct marshal_cmd_Frustumf *restrict cmd)
{
   GLfloat left = cmd->left;
   GLfloat right = cmd->right;
   GLfloat bottom = cmd->bottom;
   GLfloat top = cmd->top;
   GLfloat zNear = cmd->zNear;
   GLfloat zFar = cmd->zFar;
   CALL_Frustumf(ctx->Dispatch.Current, (left, right, bottom, top, zNear, zFar));
   return align(sizeof(struct marshal_cmd_Frustumf), 8) / 8;
}
static void GLAPIENTRY
_mesa_marshal_Frustumf(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Frustumf);
   struct marshal_cmd_Frustumf *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Frustumf, cmd_size);
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
uint32_t _mesa_unmarshal_Orthof(struct gl_context *ctx, const struct marshal_cmd_Orthof *restrict cmd)
{
   GLfloat left = cmd->left;
   GLfloat right = cmd->right;
   GLfloat bottom = cmd->bottom;
   GLfloat top = cmd->top;
   GLfloat zNear = cmd->zNear;
   GLfloat zFar = cmd->zFar;
   CALL_Orthof(ctx->Dispatch.Current, (left, right, bottom, top, zNear, zFar));
   return align(sizeof(struct marshal_cmd_Orthof), 8) / 8;
}
static void GLAPIENTRY
_mesa_marshal_Orthof(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Orthof);
   struct marshal_cmd_Orthof *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Orthof, cmd_size);
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
uint32_t _mesa_unmarshal_DrawTexiOES(struct gl_context *ctx, const struct marshal_cmd_DrawTexiOES *restrict cmd)
{
   GLint x = cmd->x;
   GLint y = cmd->y;
   GLint z = cmd->z;
   GLint width = cmd->width;
   GLint height = cmd->height;
   CALL_DrawTexiOES(ctx->Dispatch.Current, (x, y, z, width, height));
   return align(sizeof(struct marshal_cmd_DrawTexiOES), 8) / 8;
}
static void GLAPIENTRY
_mesa_marshal_DrawTexiOES(GLint x, GLint y, GLint z, GLint width, GLint height)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_DrawTexiOES);
   struct marshal_cmd_DrawTexiOES *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_DrawTexiOES, cmd_size);
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
uint32_t _mesa_unmarshal_DrawTexivOES(struct gl_context *ctx, const struct marshal_cmd_DrawTexivOES *restrict cmd)
{
   const GLint *coords = cmd->coords;
   CALL_DrawTexivOES(ctx->Dispatch.Current, (coords));
   return align(sizeof(struct marshal_cmd_DrawTexivOES), 8) / 8;
}
static void GLAPIENTRY
_mesa_marshal_DrawTexivOES(const GLint *coords)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_DrawTexivOES);
   struct marshal_cmd_DrawTexivOES *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_DrawTexivOES, cmd_size);
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
uint32_t _mesa_unmarshal_DrawTexfOES(struct gl_context *ctx, const struct marshal_cmd_DrawTexfOES *restrict cmd)
{
   GLfloat x = cmd->x;
   GLfloat y = cmd->y;
   GLfloat z = cmd->z;
   GLfloat width = cmd->width;
   GLfloat height = cmd->height;
   CALL_DrawTexfOES(ctx->Dispatch.Current, (x, y, z, width, height));
   return align(sizeof(struct marshal_cmd_DrawTexfOES), 8) / 8;
}
static void GLAPIENTRY
_mesa_marshal_DrawTexfOES(GLfloat x, GLfloat y, GLfloat z, GLfloat width, GLfloat height)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_DrawTexfOES);
   struct marshal_cmd_DrawTexfOES *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_DrawTexfOES, cmd_size);
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
uint32_t _mesa_unmarshal_DrawTexfvOES(struct gl_context *ctx, const struct marshal_cmd_DrawTexfvOES *restrict cmd)
{
   const GLfloat *coords = cmd->coords;
   CALL_DrawTexfvOES(ctx->Dispatch.Current, (coords));
   return align(sizeof(struct marshal_cmd_DrawTexfvOES), 8) / 8;
}
static void GLAPIENTRY
_mesa_marshal_DrawTexfvOES(const GLfloat *coords)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_DrawTexfvOES);
   struct marshal_cmd_DrawTexfvOES *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_DrawTexfvOES, cmd_size);
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
uint32_t _mesa_unmarshal_DrawTexsOES(struct gl_context *ctx, const struct marshal_cmd_DrawTexsOES *restrict cmd)
{
   GLshort x = cmd->x;
   GLshort y = cmd->y;
   GLshort z = cmd->z;
   GLshort width = cmd->width;
   GLshort height = cmd->height;
   CALL_DrawTexsOES(ctx->Dispatch.Current, (x, y, z, width, height));
   return align(sizeof(struct marshal_cmd_DrawTexsOES), 8) / 8;
}
static void GLAPIENTRY
_mesa_marshal_DrawTexsOES(GLshort x, GLshort y, GLshort z, GLshort width, GLshort height)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_DrawTexsOES);
   struct marshal_cmd_DrawTexsOES *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_DrawTexsOES, cmd_size);
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
uint32_t _mesa_unmarshal_DrawTexsvOES(struct gl_context *ctx, const struct marshal_cmd_DrawTexsvOES *restrict cmd)
{
   const GLshort *coords = cmd->coords;
   CALL_DrawTexsvOES(ctx->Dispatch.Current, (coords));
   return align(sizeof(struct marshal_cmd_DrawTexsvOES), 8) / 8;
}
static void GLAPIENTRY
_mesa_marshal_DrawTexsvOES(const GLshort *coords)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_DrawTexsvOES);
   struct marshal_cmd_DrawTexsvOES *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_DrawTexsvOES, cmd_size);
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
uint32_t _mesa_unmarshal_DrawTexxOES(struct gl_context *ctx, const struct marshal_cmd_DrawTexxOES *restrict cmd)
{
   GLfixed x = cmd->x;
   GLfixed y = cmd->y;
   GLfixed z = cmd->z;
   GLfixed width = cmd->width;
   GLfixed height = cmd->height;
   CALL_DrawTexxOES(ctx->Dispatch.Current, (x, y, z, width, height));
   return align(sizeof(struct marshal_cmd_DrawTexxOES), 8) / 8;
}
static void GLAPIENTRY
_mesa_marshal_DrawTexxOES(GLfixed x, GLfixed y, GLfixed z, GLfixed width, GLfixed height)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_DrawTexxOES);
   struct marshal_cmd_DrawTexxOES *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_DrawTexxOES, cmd_size);
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
uint32_t _mesa_unmarshal_DrawTexxvOES(struct gl_context *ctx, const struct marshal_cmd_DrawTexxvOES *restrict cmd)
{
   const GLfixed *coords = cmd->coords;
   CALL_DrawTexxvOES(ctx->Dispatch.Current, (coords));
   return align(sizeof(struct marshal_cmd_DrawTexxvOES), 8) / 8;
}
static void GLAPIENTRY
_mesa_marshal_DrawTexxvOES(const GLfixed *coords)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_DrawTexxvOES);
   struct marshal_cmd_DrawTexxvOES *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_DrawTexxvOES, cmd_size);
   memcpy(cmd->coords, coords, 5 * sizeof(GLfixed));
}


/* PointSizePointerOES: marshalled asynchronously */
struct marshal_cmd_PointSizePointerOES
{
   struct marshal_cmd_base cmd_base;
   GLenum16 type;
   GLclamped16i stride;
   const GLvoid * pointer;
};
struct marshal_cmd_PointSizePointerOES_packed
{
   struct marshal_cmd_base cmd_base;
   GLenum16 type;
   GLclamped16i stride;
   GLushort pointer;
};
uint32_t _mesa_unmarshal_PointSizePointerOES(struct gl_context *ctx, const struct marshal_cmd_PointSizePointerOES *restrict cmd)
{
   GLenum16 type = cmd->type;
   GLclamped16i stride = cmd->stride;
   const GLvoid * pointer = cmd->pointer;
   CALL_PointSizePointerOES(ctx->Dispatch.Current, (type, stride, pointer));
   return align(sizeof(struct marshal_cmd_PointSizePointerOES), 8) / 8;
}
uint32_t _mesa_unmarshal_PointSizePointerOES_packed(struct gl_context *ctx, const struct marshal_cmd_PointSizePointerOES_packed *restrict cmd)
{
   GLenum16 type = cmd->type;
   GLclamped16i stride = cmd->stride;
   const GLvoid * pointer = (const GLvoid *)(uintptr_t)cmd->pointer;
   CALL_PointSizePointerOES(ctx->Dispatch.Current, (type, stride, pointer));
   return align(sizeof(struct marshal_cmd_PointSizePointerOES_packed), 8) / 8;
}
static void GLAPIENTRY
_mesa_marshal_PointSizePointerOES(GLenum type, GLsizei stride, const GLvoid *pointer)
{
   GET_CURRENT_CONTEXT(ctx);
   if (((uintptr_t)pointer & 0xffff) == (uintptr_t)pointer) {
      int cmd_size = sizeof(struct marshal_cmd_PointSizePointerOES_packed);
      struct marshal_cmd_PointSizePointerOES_packed *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_PointSizePointerOES_packed, cmd_size);
      cmd->type = MIN2(type, 0xffff); /* clamped to 0xffff (invalid enum) */
      cmd->stride = CLAMP(stride, INT16_MIN, INT16_MAX);
      cmd->pointer = (uintptr_t)pointer; /* truncated */
   } else {
      int cmd_size = sizeof(struct marshal_cmd_PointSizePointerOES);
      struct marshal_cmd_PointSizePointerOES *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_PointSizePointerOES, cmd_size);
      cmd->type = MIN2(type, 0xffff); /* clamped to 0xffff (invalid enum) */
      cmd->stride = CLAMP(stride, INT16_MIN, INT16_MAX);
      cmd->pointer = pointer;
   }
   _mesa_glthread_AttribPointer(ctx, VERT_ATTRIB_POINT_SIZE, MESA_PACK_VFORMAT(type, 1, 0, 0, 0), stride, pointer);
}


/* QueryMatrixxOES: marshalled synchronously */
static GLbitfield GLAPIENTRY
_mesa_marshal_QueryMatrixxOES(GLfixed *mantissa, GLint *exponent)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "QueryMatrixxOES");
   return CALL_QueryMatrixxOES(ctx->Dispatch.Current, (mantissa, exponent));
}


/* DiscardFramebufferEXT: marshalled asynchronously */
struct marshal_cmd_DiscardFramebufferEXT
{
   struct marshal_cmd_base cmd_base;
   uint16_t num_slots;
   GLenum16 target;
   GLsizei numAttachments;
   /* Next safe_mul(numAttachments, 1 * sizeof(GLenum)) bytes are GLenum attachments[numAttachments] */
};
uint32_t _mesa_unmarshal_DiscardFramebufferEXT(struct gl_context *ctx, const struct marshal_cmd_DiscardFramebufferEXT *restrict cmd)
{
   GLenum16 target = cmd->target;
   GLsizei numAttachments = cmd->numAttachments;
   GLenum *attachments;
   const char *variable_data = (const char *) (cmd + 1);
   attachments = (GLenum *) variable_data;
   CALL_DiscardFramebufferEXT(ctx->Dispatch.Current, (target, numAttachments, attachments));
   return cmd->num_slots;
}
static void GLAPIENTRY
_mesa_marshal_DiscardFramebufferEXT(GLenum target, GLsizei numAttachments, const GLenum *attachments)
{
   GET_CURRENT_CONTEXT(ctx);
   int attachments_size = safe_mul(numAttachments, 1 * sizeof(GLenum));
   int cmd_size = sizeof(struct marshal_cmd_DiscardFramebufferEXT) + attachments_size;
   if (unlikely(attachments_size < 0 || (attachments_size > 0 && !attachments) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "DiscardFramebufferEXT");
      CALL_DiscardFramebufferEXT(ctx->Dispatch.Current, (target, numAttachments, attachments));
      return;
   }
   struct marshal_cmd_DiscardFramebufferEXT *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_DiscardFramebufferEXT, cmd_size);
   cmd->num_slots = align(cmd_size, 8) / 8;
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
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
uint32_t _mesa_unmarshal_FramebufferTexture2DMultisampleEXT(struct gl_context *ctx, const struct marshal_cmd_FramebufferTexture2DMultisampleEXT *restrict cmd)
{
   GLenum16 target = cmd->target;
   GLenum16 attachment = cmd->attachment;
   GLenum16 textarget = cmd->textarget;
   GLuint texture = cmd->texture;
   GLint level = cmd->level;
   GLsizei samples = cmd->samples;
   CALL_FramebufferTexture2DMultisampleEXT(ctx->Dispatch.Current, (target, attachment, textarget, texture, level, samples));
   return align(sizeof(struct marshal_cmd_FramebufferTexture2DMultisampleEXT), 8) / 8;
}
static void GLAPIENTRY
_mesa_marshal_FramebufferTexture2DMultisampleEXT(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLsizei samples)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_FramebufferTexture2DMultisampleEXT);
   struct marshal_cmd_FramebufferTexture2DMultisampleEXT *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_FramebufferTexture2DMultisampleEXT, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->attachment = MIN2(attachment, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->textarget = MIN2(textarget, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->texture = texture;
   cmd->level = level;
   cmd->samples = samples;
}


/* DepthRangeArrayfvOES: marshalled asynchronously */
struct marshal_cmd_DepthRangeArrayfvOES
{
   struct marshal_cmd_base cmd_base;
   uint16_t num_slots;
   GLuint first;
   GLsizei count;
   /* Next safe_mul((2 * count), 1 * sizeof(GLfloat)) bytes are GLfloat v[(2 * count)] */
};
uint32_t _mesa_unmarshal_DepthRangeArrayfvOES(struct gl_context *ctx, const struct marshal_cmd_DepthRangeArrayfvOES *restrict cmd)
{
   GLuint first = cmd->first;
   GLsizei count = cmd->count;
   GLfloat *v;
   const char *variable_data = (const char *) (cmd + 1);
   v = (GLfloat *) variable_data;
   CALL_DepthRangeArrayfvOES(ctx->Dispatch.Current, (first, count, v));
   return cmd->num_slots;
}
static void GLAPIENTRY
_mesa_marshal_DepthRangeArrayfvOES(GLuint first, GLsizei count, const GLfloat *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int v_size = safe_mul((2 * count), 1 * sizeof(GLfloat));
   int cmd_size = sizeof(struct marshal_cmd_DepthRangeArrayfvOES) + v_size;
   if (unlikely(v_size < 0 || (v_size > 0 && !v) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "DepthRangeArrayfvOES");
      CALL_DepthRangeArrayfvOES(ctx->Dispatch.Current, (first, count, v));
      return;
   }
   struct marshal_cmd_DepthRangeArrayfvOES *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_DepthRangeArrayfvOES, cmd_size);
   cmd->num_slots = align(cmd_size, 8) / 8;
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
uint32_t _mesa_unmarshal_DepthRangeIndexedfOES(struct gl_context *ctx, const struct marshal_cmd_DepthRangeIndexedfOES *restrict cmd)
{
   GLuint index = cmd->index;
   GLfloat n = cmd->n;
   GLfloat f = cmd->f;
   CALL_DepthRangeIndexedfOES(ctx->Dispatch.Current, (index, n, f));
   return align(sizeof(struct marshal_cmd_DepthRangeIndexedfOES), 8) / 8;
}
static void GLAPIENTRY
_mesa_marshal_DepthRangeIndexedfOES(GLuint index, GLfloat n, GLfloat f)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_DepthRangeIndexedfOES);
   struct marshal_cmd_DepthRangeIndexedfOES *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_DepthRangeIndexedfOES, cmd_size);
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
uint32_t _mesa_unmarshal_FramebufferParameteriMESA(struct gl_context *ctx, const struct marshal_cmd_FramebufferParameteriMESA *restrict cmd)
{
   GLenum16 target = cmd->target;
   GLenum16 pname = cmd->pname;
   GLint param = cmd->param;
   CALL_FramebufferParameteriMESA(ctx->Dispatch.Current, (target, pname, param));
   return align(sizeof(struct marshal_cmd_FramebufferParameteriMESA), 8) / 8;
}
static void GLAPIENTRY
_mesa_marshal_FramebufferParameteriMESA(GLenum target, GLenum pname, GLint param)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_FramebufferParameteriMESA);
   struct marshal_cmd_FramebufferParameteriMESA *cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_FramebufferParameteriMESA, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->pname = MIN2(pname, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->param = param;
}


/* GetFramebufferParameterivMESA: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetFramebufferParameterivMESA(GLenum target, GLenum pname, GLint *params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetFramebufferParameterivMESA");
   CALL_GetFramebufferParameterivMESA(ctx->Dispatch.Current, (target, pname, params));
}


/* TexStorageAttribs2DEXT: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_TexStorageAttribs2DEXT(GLenum target, GLsizei levels, GLenum internalFormat, GLsizei width, GLsizei height, const GLint *attrib_list)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "TexStorageAttribs2DEXT");
   CALL_TexStorageAttribs2DEXT(ctx->Dispatch.Current, (target, levels, internalFormat, width, height, attrib_list));
}


/* TexStorageAttribs3DEXT: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_TexStorageAttribs3DEXT(GLenum target, GLsizei levels, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth, const GLint *attrib_list)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "TexStorageAttribs3DEXT");
   CALL_TexStorageAttribs3DEXT(ctx->Dispatch.Current, (target, levels, internalFormat, width, height, depth, attrib_list));
}


void
_mesa_glthread_init_dispatch7(struct gl_context *ctx, struct _glapi_table *table)
{
   if ((_mesa_is_gles2(ctx) && ctx->Version >= 31)) {
      SET_DepthRangeArrayfvOES(table, _mesa_marshal_DepthRangeArrayfvOES);
      SET_DepthRangeIndexedfOES(table, _mesa_marshal_DepthRangeIndexedfOES);
   }
   if (_mesa_is_desktop_gl(ctx)) {
      SET_BindBufferOffsetEXT(table, _mesa_marshal_BindBufferOffsetEXT);
      SET_DrawTransformFeedback(table, _mesa_marshal_DrawTransformFeedback);
      SET_PrimitiveRestartIndex(table, _mesa_marshal_PrimitiveRestartIndex);
      SET_TexStorageMem1DEXT(table, _mesa_marshal_TexStorageMem1DEXT);
      SET_TextureStorageMem1DEXT(table, _mesa_marshal_TextureStorageMem1DEXT);
      SET_VDPAUFiniNV(table, _mesa_marshal_VDPAUFiniNV);
      SET_VDPAUGetSurfaceivNV(table, _mesa_marshal_VDPAUGetSurfaceivNV);
      SET_VDPAUInitNV(table, _mesa_marshal_VDPAUInitNV);
      SET_VDPAUIsSurfaceNV(table, _mesa_marshal_VDPAUIsSurfaceNV);
      SET_VDPAUMapSurfacesNV(table, _mesa_marshal_VDPAUMapSurfacesNV);
      SET_VDPAURegisterOutputSurfaceNV(table, _mesa_marshal_VDPAURegisterOutputSurfaceNV);
      SET_VDPAURegisterVideoSurfaceNV(table, _mesa_marshal_VDPAURegisterVideoSurfaceNV);
      SET_VDPAUSurfaceAccessNV(table, _mesa_marshal_VDPAUSurfaceAccessNV);
      SET_VDPAUUnmapSurfacesNV(table, _mesa_marshal_VDPAUUnmapSurfacesNV);
      SET_VDPAUUnregisterSurfaceNV(table, _mesa_marshal_VDPAUUnregisterSurfaceNV);
   }
   if (_mesa_is_desktop_gl(ctx) || (_mesa_is_gles2(ctx) && ctx->Version >= 30)) {
      SET_BeginTransformFeedback(table, _mesa_marshal_BeginTransformFeedback);
      SET_BindBufferBase(table, _mesa_marshal_BindBufferBase);
      SET_BindBufferRange(table, _mesa_marshal_BindBufferRange);
      SET_BindFragDataLocation(table, _mesa_marshal_BindFragDataLocation);
      SET_BindTransformFeedback(table, _mesa_marshal_BindTransformFeedback);
      SET_ClearBufferfi(table, _mesa_marshal_ClearBufferfi);
      SET_ClearBufferfv(table, _mesa_marshal_ClearBufferfv);
      SET_ClearBufferiv(table, _mesa_marshal_ClearBufferiv);
      SET_ClearBufferuiv(table, _mesa_marshal_ClearBufferuiv);
      SET_DeleteTransformFeedbacks(table, _mesa_marshal_DeleteTransformFeedbacks);
      SET_EndTransformFeedback(table, _mesa_marshal_EndTransformFeedback);
      SET_GenTransformFeedbacks(table, _mesa_marshal_GenTransformFeedbacks);
      SET_GetBufferParameteri64v(table, _mesa_marshal_GetBufferParameteri64v);
      SET_GetFragDataLocation(table, _mesa_marshal_GetFragDataLocation);
      SET_GetInteger64i_v(table, _mesa_marshal_GetInteger64i_v);
      SET_GetStringi(table, _mesa_marshal_GetStringi);
      SET_GetTransformFeedbackVarying(table, _mesa_marshal_GetTransformFeedbackVarying);
      SET_GetUniformuiv(table, _mesa_marshal_GetUniformuiv);
      SET_IsTransformFeedback(table, _mesa_marshal_IsTransformFeedback);
      SET_PauseTransformFeedback(table, _mesa_marshal_PauseTransformFeedback);
      SET_ResumeTransformFeedback(table, _mesa_marshal_ResumeTransformFeedback);
      SET_TexStorageAttribs2DEXT(table, _mesa_marshal_TexStorageAttribs2DEXT);
      SET_TexStorageAttribs3DEXT(table, _mesa_marshal_TexStorageAttribs3DEXT);
      SET_TransformFeedbackVaryings(table, _mesa_marshal_TransformFeedbackVaryings);
      SET_Uniform1uiv(table, _mesa_marshal_Uniform1uiv);
      SET_Uniform2ui(table, _mesa_marshal_Uniform2ui);
      SET_Uniform2uiv(table, _mesa_marshal_Uniform2uiv);
      SET_Uniform3ui(table, _mesa_marshal_Uniform3ui);
      SET_Uniform3uiv(table, _mesa_marshal_Uniform3uiv);
      SET_Uniform4ui(table, _mesa_marshal_Uniform4ui);
      SET_Uniform4uiv(table, _mesa_marshal_Uniform4uiv);
   }
   if (_mesa_is_desktop_gl(ctx) || (_mesa_is_gles2(ctx) && ctx->Version >= 31)) {
      SET_MemoryBarrierByRegion(table, _mesa_marshal_MemoryBarrierByRegion);
      SET_ViewportSwizzleNV(table, _mesa_marshal_ViewportSwizzleNV);
   }
   if (_mesa_is_desktop_gl(ctx) || (_mesa_is_gles2(ctx) && ctx->Version >= 32)) {
      SET_BufferStorageMemEXT(table, _mesa_marshal_BufferStorageMemEXT);
      SET_CreateMemoryObjectsEXT(table, _mesa_marshal_CreateMemoryObjectsEXT);
      SET_DeleteMemoryObjectsEXT(table, _mesa_marshal_DeleteMemoryObjectsEXT);
      SET_DeleteSemaphoresEXT(table, _mesa_marshal_DeleteSemaphoresEXT);
      SET_GenSemaphoresEXT(table, _mesa_marshal_GenSemaphoresEXT);
      SET_GetMemoryObjectParameterivEXT(table, _mesa_marshal_GetMemoryObjectParameterivEXT);
      SET_GetSemaphoreParameterui64vEXT(table, _mesa_marshal_GetSemaphoreParameterui64vEXT);
      SET_GetUnsignedBytei_vEXT(table, _mesa_marshal_GetUnsignedBytei_vEXT);
      SET_GetUnsignedBytevEXT(table, _mesa_marshal_GetUnsignedBytevEXT);
      SET_ImportMemoryFdEXT(table, _mesa_marshal_ImportMemoryFdEXT);
      SET_ImportMemoryWin32HandleEXT(table, _mesa_marshal_ImportMemoryWin32HandleEXT);
      SET_ImportMemoryWin32NameEXT(table, _mesa_marshal_ImportMemoryWin32NameEXT);
      SET_ImportSemaphoreFdEXT(table, _mesa_marshal_ImportSemaphoreFdEXT);
      SET_ImportSemaphoreWin32HandleEXT(table, _mesa_marshal_ImportSemaphoreWin32HandleEXT);
      SET_ImportSemaphoreWin32NameEXT(table, _mesa_marshal_ImportSemaphoreWin32NameEXT);
      SET_IsMemoryObjectEXT(table, _mesa_marshal_IsMemoryObjectEXT);
      SET_IsSemaphoreEXT(table, _mesa_marshal_IsSemaphoreEXT);
      SET_MemoryObjectParameterivEXT(table, _mesa_marshal_MemoryObjectParameterivEXT);
      SET_NamedBufferStorageMemEXT(table, _mesa_marshal_NamedBufferStorageMemEXT);
      SET_SemaphoreParameterui64vEXT(table, _mesa_marshal_SemaphoreParameterui64vEXT);
      SET_SignalSemaphoreEXT(table, _mesa_marshal_SignalSemaphoreEXT);
      SET_TexStorageMem2DEXT(table, _mesa_marshal_TexStorageMem2DEXT);
      SET_TexStorageMem2DMultisampleEXT(table, _mesa_marshal_TexStorageMem2DMultisampleEXT);
      SET_TexStorageMem3DEXT(table, _mesa_marshal_TexStorageMem3DEXT);
      SET_TexStorageMem3DMultisampleEXT(table, _mesa_marshal_TexStorageMem3DMultisampleEXT);
      SET_TextureStorageMem2DEXT(table, _mesa_marshal_TextureStorageMem2DEXT);
      SET_TextureStorageMem2DMultisampleEXT(table, _mesa_marshal_TextureStorageMem2DMultisampleEXT);
      SET_TextureStorageMem3DEXT(table, _mesa_marshal_TextureStorageMem3DEXT);
      SET_TextureStorageMem3DMultisampleEXT(table, _mesa_marshal_TextureStorageMem3DMultisampleEXT);
      SET_WaitSemaphoreEXT(table, _mesa_marshal_WaitSemaphoreEXT);
   }
   if (_mesa_is_desktop_gl(ctx) || _mesa_is_gles2(ctx)) {
      SET_BeginConditionalRender(table, _mesa_marshal_BeginConditionalRender);
      SET_EndConditionalRender(table, _mesa_marshal_EndConditionalRender);
   }
   if (_mesa_is_desktop_gl_compat(ctx)) {
      SET_Color3hNV(table, _mesa_marshal_Color3hNV);
      SET_Color3hvNV(table, _mesa_marshal_Color3hvNV);
      SET_Color4hNV(table, _mesa_marshal_Color4hNV);
      SET_Color4hvNV(table, _mesa_marshal_Color4hvNV);
      SET_FogCoordhNV(table, _mesa_marshal_FogCoordhNV);
      SET_FogCoordhvNV(table, _mesa_marshal_FogCoordhvNV);
      SET_MultiTexCoord1hNV(table, _mesa_marshal_MultiTexCoord1hNV);
      SET_MultiTexCoord1hvNV(table, _mesa_marshal_MultiTexCoord1hvNV);
      SET_MultiTexCoord2hNV(table, _mesa_marshal_MultiTexCoord2hNV);
      SET_MultiTexCoord2hvNV(table, _mesa_marshal_MultiTexCoord2hvNV);
      SET_MultiTexCoord3hNV(table, _mesa_marshal_MultiTexCoord3hNV);
      SET_MultiTexCoord3hvNV(table, _mesa_marshal_MultiTexCoord3hvNV);
      SET_MultiTexCoord4hNV(table, _mesa_marshal_MultiTexCoord4hNV);
      SET_MultiTexCoord4hvNV(table, _mesa_marshal_MultiTexCoord4hvNV);
      SET_Normal3hNV(table, _mesa_marshal_Normal3hNV);
      SET_Normal3hvNV(table, _mesa_marshal_Normal3hvNV);
      SET_PrimitiveRestartNV(table, _mesa_marshal_PrimitiveRestartNV);
      SET_SecondaryColor3hNV(table, _mesa_marshal_SecondaryColor3hNV);
      SET_SecondaryColor3hvNV(table, _mesa_marshal_SecondaryColor3hvNV);
      SET_TexCoord1hNV(table, _mesa_marshal_TexCoord1hNV);
      SET_TexCoord1hvNV(table, _mesa_marshal_TexCoord1hvNV);
      SET_TexCoord2hNV(table, _mesa_marshal_TexCoord2hNV);
      SET_TexCoord2hvNV(table, _mesa_marshal_TexCoord2hvNV);
      SET_TexCoord3hNV(table, _mesa_marshal_TexCoord3hNV);
      SET_TexCoord3hvNV(table, _mesa_marshal_TexCoord3hvNV);
      SET_TexCoord4hNV(table, _mesa_marshal_TexCoord4hNV);
      SET_TexCoord4hvNV(table, _mesa_marshal_TexCoord4hvNV);
      SET_Vertex2hNV(table, _mesa_marshal_Vertex2hNV);
      SET_Vertex2hvNV(table, _mesa_marshal_Vertex2hvNV);
      SET_Vertex3hNV(table, _mesa_marshal_Vertex3hNV);
      SET_Vertex3hvNV(table, _mesa_marshal_Vertex3hvNV);
      SET_Vertex4hNV(table, _mesa_marshal_Vertex4hNV);
      SET_Vertex4hvNV(table, _mesa_marshal_Vertex4hvNV);
      SET_VertexAttrib1hNV(table, _mesa_marshal_VertexAttrib1hNV);
      SET_VertexAttrib1hvNV(table, _mesa_marshal_VertexAttrib1hvNV);
      SET_VertexAttrib2hNV(table, _mesa_marshal_VertexAttrib2hNV);
      SET_VertexAttrib2hvNV(table, _mesa_marshal_VertexAttrib2hvNV);
      SET_VertexAttrib3hNV(table, _mesa_marshal_VertexAttrib3hNV);
      SET_VertexAttrib3hvNV(table, _mesa_marshal_VertexAttrib3hvNV);
      SET_VertexAttrib4hNV(table, _mesa_marshal_VertexAttrib4hNV);
      SET_VertexAttrib4hvNV(table, _mesa_marshal_VertexAttrib4hvNV);
      SET_VertexAttribs1hvNV(table, _mesa_marshal_VertexAttribs1hvNV);
      SET_VertexAttribs2hvNV(table, _mesa_marshal_VertexAttribs2hvNV);
      SET_VertexAttribs3hvNV(table, _mesa_marshal_VertexAttribs3hvNV);
      SET_VertexAttribs4hvNV(table, _mesa_marshal_VertexAttribs4hvNV);
   }
   if (_mesa_is_desktop_gl_compat(ctx) || _mesa_is_desktop_gl_core(ctx) || (_mesa_is_gles2(ctx) && ctx->Version >= 31)) {
      SET_FramebufferTexture(table, _mesa_marshal_FramebufferTexture);
   }
   if (_mesa_is_desktop_gl_compat(ctx) || _mesa_is_gles1(ctx)) {
      SET_AlphaFuncx(table, _mesa_marshal_AlphaFuncx);
      SET_ClearColorx(table, _mesa_marshal_ClearColorx);
      SET_ClearDepthx(table, _mesa_marshal_ClearDepthx);
      SET_ClipPlanef(table, _mesa_marshal_ClipPlanef);
      SET_ClipPlanex(table, _mesa_marshal_ClipPlanex);
      SET_Color4x(table, _mesa_marshal_Color4x);
      SET_DepthRangex(table, _mesa_marshal_DepthRangex);
      SET_Fogx(table, _mesa_marshal_Fogx);
      SET_Fogxv(table, _mesa_marshal_Fogxv);
      SET_Frustumf(table, _mesa_marshal_Frustumf);
      SET_Frustumx(table, _mesa_marshal_Frustumx);
      SET_GetFixedv(table, _mesa_marshal_GetFixedv);
      SET_GetLightxv(table, _mesa_marshal_GetLightxv);
      SET_GetMaterialxv(table, _mesa_marshal_GetMaterialxv);
      SET_GetTexEnvxv(table, _mesa_marshal_GetTexEnvxv);
      SET_GetTexParameterxv(table, _mesa_marshal_GetTexParameterxv);
      SET_LightModelx(table, _mesa_marshal_LightModelx);
      SET_LightModelxv(table, _mesa_marshal_LightModelxv);
      SET_Lightx(table, _mesa_marshal_Lightx);
      SET_Lightxv(table, _mesa_marshal_Lightxv);
      SET_LineWidthx(table, _mesa_marshal_LineWidthx);
      SET_LoadMatrixx(table, _mesa_marshal_LoadMatrixx);
      SET_Materialx(table, _mesa_marshal_Materialx);
      SET_Materialxv(table, _mesa_marshal_Materialxv);
      SET_MultMatrixx(table, _mesa_marshal_MultMatrixx);
      SET_MultiTexCoord4x(table, _mesa_marshal_MultiTexCoord4x);
      SET_Normal3x(table, _mesa_marshal_Normal3x);
      SET_Orthof(table, _mesa_marshal_Orthof);
      SET_Orthox(table, _mesa_marshal_Orthox);
      SET_PointParameterx(table, _mesa_marshal_PointParameterx);
      SET_PointParameterxv(table, _mesa_marshal_PointParameterxv);
      SET_PointSizex(table, _mesa_marshal_PointSizex);
      SET_PolygonOffsetx(table, _mesa_marshal_PolygonOffsetx);
      SET_Rotatex(table, _mesa_marshal_Rotatex);
      SET_SampleCoveragex(table, _mesa_marshal_SampleCoveragex);
      SET_Scalex(table, _mesa_marshal_Scalex);
      SET_TexEnvx(table, _mesa_marshal_TexEnvx);
      SET_TexEnvxv(table, _mesa_marshal_TexEnvxv);
      SET_TexParameterx(table, _mesa_marshal_TexParameterx);
      SET_TexParameterxv(table, _mesa_marshal_TexParameterxv);
      SET_Translatex(table, _mesa_marshal_Translatex);
   }
   if (_mesa_is_desktop_gl_core(ctx) || (_mesa_is_gles2(ctx) && ctx->Version >= 30)) {
      SET_FramebufferParameteriMESA(table, _mesa_marshal_FramebufferParameteriMESA);
      SET_GetFramebufferParameterivMESA(table, _mesa_marshal_GetFramebufferParameterivMESA);
   }
   if (_mesa_is_gles1(ctx)) {
      SET_DrawTexfOES(table, _mesa_marshal_DrawTexfOES);
      SET_DrawTexfvOES(table, _mesa_marshal_DrawTexfvOES);
      SET_DrawTexiOES(table, _mesa_marshal_DrawTexiOES);
      SET_DrawTexivOES(table, _mesa_marshal_DrawTexivOES);
      SET_DrawTexsOES(table, _mesa_marshal_DrawTexsOES);
      SET_DrawTexsvOES(table, _mesa_marshal_DrawTexsvOES);
      SET_DrawTexxOES(table, _mesa_marshal_DrawTexxOES);
      SET_DrawTexxvOES(table, _mesa_marshal_DrawTexxvOES);
      SET_GetClipPlanef(table, _mesa_marshal_GetClipPlanef);
      SET_GetClipPlanex(table, _mesa_marshal_GetClipPlanex);
      SET_GetTexGenxvOES(table, _mesa_marshal_GetTexGenxvOES);
      SET_PointSizePointerOES(table, _mesa_marshal_PointSizePointerOES);
      SET_QueryMatrixxOES(table, _mesa_marshal_QueryMatrixxOES);
      SET_TexGenxOES(table, _mesa_marshal_TexGenxOES);
      SET_TexGenxvOES(table, _mesa_marshal_TexGenxvOES);
   }
   if (_mesa_is_gles1(ctx) || _mesa_is_gles2(ctx)) {
      SET_DiscardFramebufferEXT(table, _mesa_marshal_DiscardFramebufferEXT);
   }
   if (_mesa_is_gles2(ctx)) {
      SET_FramebufferTexture2DMultisampleEXT(table, _mesa_marshal_FramebufferTexture2DMultisampleEXT);
   }
}
