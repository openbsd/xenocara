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

/* VertexAttribI4sv: marshalled asynchronously */
struct marshal_cmd_VertexAttribI4sv
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLshort v[4];
};
uint32_t
_mesa_unmarshal_VertexAttribI4sv(struct gl_context *ctx, const struct marshal_cmd_VertexAttribI4sv *cmd)
{
   GLuint index = cmd->index;
   const GLshort *v = cmd->v;
   CALL_VertexAttribI4sv(ctx->CurrentServerDispatch, (index, v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexAttribI4sv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
uint32_t
_mesa_unmarshal_VertexAttribI4ubv(struct gl_context *ctx, const struct marshal_cmd_VertexAttribI4ubv *cmd)
{
   GLuint index = cmd->index;
   const GLubyte *v = cmd->v;
   CALL_VertexAttribI4ubv(ctx->CurrentServerDispatch, (index, v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexAttribI4ubv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
uint32_t
_mesa_unmarshal_VertexAttribI4usv(struct gl_context *ctx, const struct marshal_cmd_VertexAttribI4usv *cmd)
{
   GLuint index = cmd->index;
   const GLushort *v = cmd->v;
   CALL_VertexAttribI4usv(ctx->CurrentServerDispatch, (index, v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexAttribI4usv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
uint32_t
_mesa_unmarshal_VertexAttribIPointer(struct gl_context *ctx, const struct marshal_cmd_VertexAttribIPointer *cmd)
{
   GLuint index = cmd->index;
   GLint size = cmd->size;
   GLenum type = cmd->type;
   GLsizei stride = cmd->stride;
   const GLvoid * pointer = cmd->pointer;
   CALL_VertexAttribIPointer(ctx->CurrentServerDispatch, (index, size, type, stride, pointer));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexAttribIPointer), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_VertexAttribIPointer(GLuint index, GLint size, GLenum type, GLsizei stride, const GLvoid * pointer)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttribIPointer);
   struct marshal_cmd_VertexAttribIPointer *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttribIPointer, cmd_size);
   cmd->index = index;
   cmd->size = size;
   cmd->type = MIN2(type, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->stride = stride;
   cmd->pointer = pointer;
   if (COMPAT) _mesa_glthread_AttribPointer(ctx, VERT_ATTRIB_GENERIC(index), size, type, stride, pointer);
}


/* GetVertexAttribIiv: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetVertexAttribIiv(GLuint index, GLenum pname, GLint * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetVertexAttribIiv");
   CALL_GetVertexAttribIiv(ctx->CurrentServerDispatch, (index, pname, params));
}


/* GetVertexAttribIuiv: marshalled synchronously */
static void GLAPIENTRY
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
uint32_t
_mesa_unmarshal_Uniform1ui(struct gl_context *ctx, const struct marshal_cmd_Uniform1ui *cmd)
{
   GLint location = cmd->location;
   GLuint x = cmd->x;
   CALL_Uniform1ui(ctx->CurrentServerDispatch, (location, x));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Uniform1ui), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
uint32_t
_mesa_unmarshal_Uniform2ui(struct gl_context *ctx, const struct marshal_cmd_Uniform2ui *cmd)
{
   GLint location = cmd->location;
   GLuint x = cmd->x;
   GLuint y = cmd->y;
   CALL_Uniform2ui(ctx->CurrentServerDispatch, (location, x, y));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Uniform2ui), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
uint32_t
_mesa_unmarshal_Uniform3ui(struct gl_context *ctx, const struct marshal_cmd_Uniform3ui *cmd)
{
   GLint location = cmd->location;
   GLuint x = cmd->x;
   GLuint y = cmd->y;
   GLuint z = cmd->z;
   CALL_Uniform3ui(ctx->CurrentServerDispatch, (location, x, y, z));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Uniform3ui), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
uint32_t
_mesa_unmarshal_Uniform4ui(struct gl_context *ctx, const struct marshal_cmd_Uniform4ui *cmd)
{
   GLint location = cmd->location;
   GLuint x = cmd->x;
   GLuint y = cmd->y;
   GLuint z = cmd->z;
   GLuint w = cmd->w;
   CALL_Uniform4ui(ctx->CurrentServerDispatch, (location, x, y, z, w));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Uniform4ui), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
uint32_t
_mesa_unmarshal_Uniform1uiv(struct gl_context *ctx, const struct marshal_cmd_Uniform1uiv *cmd)
{
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLuint *value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLuint *) variable_data;
   CALL_Uniform1uiv(ctx->CurrentServerDispatch, (location, count, value));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
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
uint32_t
_mesa_unmarshal_Uniform2uiv(struct gl_context *ctx, const struct marshal_cmd_Uniform2uiv *cmd)
{
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLuint *value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLuint *) variable_data;
   CALL_Uniform2uiv(ctx->CurrentServerDispatch, (location, count, value));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
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
uint32_t
_mesa_unmarshal_Uniform3uiv(struct gl_context *ctx, const struct marshal_cmd_Uniform3uiv *cmd)
{
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLuint *value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLuint *) variable_data;
   CALL_Uniform3uiv(ctx->CurrentServerDispatch, (location, count, value));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
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
uint32_t
_mesa_unmarshal_Uniform4uiv(struct gl_context *ctx, const struct marshal_cmd_Uniform4uiv *cmd)
{
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLuint *value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLuint *) variable_data;
   CALL_Uniform4uiv(ctx->CurrentServerDispatch, (location, count, value));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
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
static void GLAPIENTRY
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
uint32_t
_mesa_unmarshal_BindFragDataLocation(struct gl_context *ctx, const struct marshal_cmd_BindFragDataLocation *cmd)
{
   GLuint program = cmd->program;
   GLuint colorNumber = cmd->colorNumber;
   GLchar *name;
   const char *variable_data = (const char *) (cmd + 1);
   name = (GLchar *) variable_data;
   CALL_BindFragDataLocation(ctx->CurrentServerDispatch, (program, colorNumber, name));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
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
static GLint GLAPIENTRY
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
uint32_t
_mesa_unmarshal_ClearBufferiv(struct gl_context *ctx, const struct marshal_cmd_ClearBufferiv *cmd)
{
   GLenum buffer = cmd->buffer;
   GLint drawbuffer = cmd->drawbuffer;
   GLint *value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLint *) variable_data;
   CALL_ClearBufferiv(ctx->CurrentServerDispatch, (buffer, drawbuffer, value));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
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
   cmd->buffer = MIN2(buffer, 0xffff); /* clamped to 0xffff (invalid enum) */
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
uint32_t
_mesa_unmarshal_ClearBufferuiv(struct gl_context *ctx, const struct marshal_cmd_ClearBufferuiv *cmd)
{
   GLenum buffer = cmd->buffer;
   GLint drawbuffer = cmd->drawbuffer;
   GLuint *value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLuint *) variable_data;
   CALL_ClearBufferuiv(ctx->CurrentServerDispatch, (buffer, drawbuffer, value));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
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
   cmd->buffer = MIN2(buffer, 0xffff); /* clamped to 0xffff (invalid enum) */
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
uint32_t
_mesa_unmarshal_ClearBufferfv(struct gl_context *ctx, const struct marshal_cmd_ClearBufferfv *cmd)
{
   GLenum buffer = cmd->buffer;
   GLint drawbuffer = cmd->drawbuffer;
   GLfloat *value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLfloat *) variable_data;
   CALL_ClearBufferfv(ctx->CurrentServerDispatch, (buffer, drawbuffer, value));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
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
uint32_t
_mesa_unmarshal_ClearBufferfi(struct gl_context *ctx, const struct marshal_cmd_ClearBufferfi *cmd)
{
   GLenum buffer = cmd->buffer;
   GLint drawbuffer = cmd->drawbuffer;
   GLfloat depth = cmd->depth;
   GLint stencil = cmd->stencil;
   CALL_ClearBufferfi(ctx->CurrentServerDispatch, (buffer, drawbuffer, depth, stencil));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_ClearBufferfi), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ClearBufferfi(GLenum buffer, GLint drawbuffer, GLfloat depth, GLint stencil)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ClearBufferfi);
   struct marshal_cmd_ClearBufferfi *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ClearBufferfi, cmd_size);
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
   return CALL_GetStringi(ctx->CurrentServerDispatch, (name, index));
}


/* BeginTransformFeedback: marshalled asynchronously */
struct marshal_cmd_BeginTransformFeedback
{
   struct marshal_cmd_base cmd_base;
   GLenum16 mode;
};
uint32_t
_mesa_unmarshal_BeginTransformFeedback(struct gl_context *ctx, const struct marshal_cmd_BeginTransformFeedback *cmd)
{
   GLenum mode = cmd->mode;
   CALL_BeginTransformFeedback(ctx->CurrentServerDispatch, (mode));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_BeginTransformFeedback), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_BeginTransformFeedback(GLenum mode)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_BeginTransformFeedback);
   struct marshal_cmd_BeginTransformFeedback *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_BeginTransformFeedback, cmd_size);
   cmd->mode = MIN2(mode, 0xffff); /* clamped to 0xffff (invalid enum) */
}


/* EndTransformFeedback: marshalled asynchronously */
struct marshal_cmd_EndTransformFeedback
{
   struct marshal_cmd_base cmd_base;
};
uint32_t
_mesa_unmarshal_EndTransformFeedback(struct gl_context *ctx, const struct marshal_cmd_EndTransformFeedback *cmd)
{
   CALL_EndTransformFeedback(ctx->CurrentServerDispatch, ());
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_EndTransformFeedback), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
uint32_t
_mesa_unmarshal_BindBufferRange(struct gl_context *ctx, const struct marshal_cmd_BindBufferRange *cmd)
{
   GLenum target = cmd->target;
   GLuint index = cmd->index;
   GLuint buffer = cmd->buffer;
   GLintptr offset = cmd->offset;
   GLsizeiptr size = cmd->size;
   CALL_BindBufferRange(ctx->CurrentServerDispatch, (target, index, buffer, offset, size));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_BindBufferRange), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_BindBufferRange(GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_BindBufferRange);
   struct marshal_cmd_BindBufferRange *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_BindBufferRange, cmd_size);
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
uint32_t
_mesa_unmarshal_BindBufferBase(struct gl_context *ctx, const struct marshal_cmd_BindBufferBase *cmd)
{
   GLenum target = cmd->target;
   GLuint index = cmd->index;
   GLuint buffer = cmd->buffer;
   CALL_BindBufferBase(ctx->CurrentServerDispatch, (target, index, buffer));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_BindBufferBase), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_BindBufferBase(GLenum target, GLuint index, GLuint buffer)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_BindBufferBase);
   struct marshal_cmd_BindBufferBase *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_BindBufferBase, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->index = index;
   cmd->buffer = buffer;
}


/* TransformFeedbackVaryings: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_TransformFeedbackVaryings(GLuint program, GLsizei count, const GLchar * const * varyings, GLenum bufferMode)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "TransformFeedbackVaryings");
   CALL_TransformFeedbackVaryings(ctx->CurrentServerDispatch, (program, count, varyings, bufferMode));
}


/* GetTransformFeedbackVarying: marshalled synchronously */
static void GLAPIENTRY
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
uint32_t
_mesa_unmarshal_BeginConditionalRender(struct gl_context *ctx, const struct marshal_cmd_BeginConditionalRender *cmd)
{
   GLuint query = cmd->query;
   GLenum mode = cmd->mode;
   CALL_BeginConditionalRender(ctx->CurrentServerDispatch, (query, mode));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_BeginConditionalRender), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_BeginConditionalRender(GLuint query, GLenum mode)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_BeginConditionalRender);
   struct marshal_cmd_BeginConditionalRender *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_BeginConditionalRender, cmd_size);
   cmd->query = query;
   cmd->mode = MIN2(mode, 0xffff); /* clamped to 0xffff (invalid enum) */
}


/* EndConditionalRender: marshalled asynchronously */
struct marshal_cmd_EndConditionalRender
{
   struct marshal_cmd_base cmd_base;
};
uint32_t
_mesa_unmarshal_EndConditionalRender(struct gl_context *ctx, const struct marshal_cmd_EndConditionalRender *cmd)
{
   CALL_EndConditionalRender(ctx->CurrentServerDispatch, ());
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_EndConditionalRender), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
uint32_t
_mesa_unmarshal_PrimitiveRestartIndex(struct gl_context *ctx, const struct marshal_cmd_PrimitiveRestartIndex *cmd)
{
   GLuint index = cmd->index;
   CALL_PrimitiveRestartIndex(ctx->CurrentServerDispatch, (index));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_PrimitiveRestartIndex), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_PrimitiveRestartIndex(GLuint index)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_PrimitiveRestartIndex);
   struct marshal_cmd_PrimitiveRestartIndex *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_PrimitiveRestartIndex, cmd_size);
   cmd->index = index;
   _mesa_glthread_PrimitiveRestartIndex(ctx, index);
}


/* GetInteger64i_v: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetInteger64i_v(GLenum cap, GLuint index, GLint64 * data)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetInteger64i_v");
   CALL_GetInteger64i_v(ctx->CurrentServerDispatch, (cap, index, data));
}


/* GetBufferParameteri64v: marshalled synchronously */
static void GLAPIENTRY
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
uint32_t
_mesa_unmarshal_FramebufferTexture(struct gl_context *ctx, const struct marshal_cmd_FramebufferTexture *cmd)
{
   GLenum target = cmd->target;
   GLenum attachment = cmd->attachment;
   GLuint texture = cmd->texture;
   GLint level = cmd->level;
   CALL_FramebufferTexture(ctx->CurrentServerDispatch, (target, attachment, texture, level));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_FramebufferTexture), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_FramebufferTexture(GLenum target, GLenum attachment, GLuint texture, GLint level)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_FramebufferTexture);
   struct marshal_cmd_FramebufferTexture *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_FramebufferTexture, cmd_size);
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
uint32_t
_mesa_unmarshal_PrimitiveRestartNV(struct gl_context *ctx, const struct marshal_cmd_PrimitiveRestartNV *cmd)
{
   CALL_PrimitiveRestartNV(ctx->CurrentServerDispatch, ());
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_PrimitiveRestartNV), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
uint32_t
_mesa_unmarshal_BindBufferOffsetEXT(struct gl_context *ctx, const struct marshal_cmd_BindBufferOffsetEXT *cmd)
{
   GLenum target = cmd->target;
   GLuint index = cmd->index;
   GLuint buffer = cmd->buffer;
   GLintptr offset = cmd->offset;
   CALL_BindBufferOffsetEXT(ctx->CurrentServerDispatch, (target, index, buffer, offset));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_BindBufferOffsetEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_BindBufferOffsetEXT(GLenum target, GLuint index, GLuint buffer, GLintptr offset)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_BindBufferOffsetEXT);
   struct marshal_cmd_BindBufferOffsetEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_BindBufferOffsetEXT, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
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
uint32_t
_mesa_unmarshal_BindTransformFeedback(struct gl_context *ctx, const struct marshal_cmd_BindTransformFeedback *cmd)
{
   GLenum target = cmd->target;
   GLuint id = cmd->id;
   CALL_BindTransformFeedback(ctx->CurrentServerDispatch, (target, id));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_BindTransformFeedback), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_BindTransformFeedback(GLenum target, GLuint id)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_BindTransformFeedback);
   struct marshal_cmd_BindTransformFeedback *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_BindTransformFeedback, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->id = id;
}


/* DeleteTransformFeedbacks: marshalled asynchronously */
struct marshal_cmd_DeleteTransformFeedbacks
{
   struct marshal_cmd_base cmd_base;
   GLsizei n;
   /* Next safe_mul(n, 1 * sizeof(GLuint)) bytes are GLuint ids[n] */
};
uint32_t
_mesa_unmarshal_DeleteTransformFeedbacks(struct gl_context *ctx, const struct marshal_cmd_DeleteTransformFeedbacks *cmd)
{
   GLsizei n = cmd->n;
   GLuint *ids;
   const char *variable_data = (const char *) (cmd + 1);
   ids = (GLuint *) variable_data;
   CALL_DeleteTransformFeedbacks(ctx->CurrentServerDispatch, (n, ids));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
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
static void GLAPIENTRY
_mesa_marshal_GenTransformFeedbacks(GLsizei n, GLuint * ids)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GenTransformFeedbacks");
   CALL_GenTransformFeedbacks(ctx->CurrentServerDispatch, (n, ids));
}


/* IsTransformFeedback: marshalled synchronously */
static GLboolean GLAPIENTRY
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
uint32_t
_mesa_unmarshal_PauseTransformFeedback(struct gl_context *ctx, const struct marshal_cmd_PauseTransformFeedback *cmd)
{
   CALL_PauseTransformFeedback(ctx->CurrentServerDispatch, ());
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_PauseTransformFeedback), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
uint32_t
_mesa_unmarshal_ResumeTransformFeedback(struct gl_context *ctx, const struct marshal_cmd_ResumeTransformFeedback *cmd)
{
   CALL_ResumeTransformFeedback(ctx->CurrentServerDispatch, ());
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_ResumeTransformFeedback), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
uint32_t
_mesa_unmarshal_DrawTransformFeedback(struct gl_context *ctx, const struct marshal_cmd_DrawTransformFeedback *cmd)
{
   GLenum mode = cmd->mode;
   GLuint id = cmd->id;
   CALL_DrawTransformFeedback(ctx->CurrentServerDispatch, (mode, id));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_DrawTransformFeedback), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_DrawTransformFeedback(GLenum mode, GLuint id)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_DrawTransformFeedback);
   struct marshal_cmd_DrawTransformFeedback *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_DrawTransformFeedback, cmd_size);
   cmd->mode = MIN2(mode, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->id = id;
}


/* VDPAUInitNV: marshalled synchronously */
static void GLAPIENTRY
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
uint32_t
_mesa_unmarshal_VDPAUFiniNV(struct gl_context *ctx, const struct marshal_cmd_VDPAUFiniNV *cmd)
{
   CALL_VDPAUFiniNV(ctx->CurrentServerDispatch, ());
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VDPAUFiniNV), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_VDPAUFiniNV(void)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VDPAUFiniNV);
   struct marshal_cmd_VDPAUFiniNV *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VDPAUFiniNV, cmd_size);
   (void) cmd;
}


/* VDPAURegisterVideoSurfaceNV: marshalled synchronously */
static GLintptr GLAPIENTRY
_mesa_marshal_VDPAURegisterVideoSurfaceNV(const GLvoid * vdpSurface, GLenum target, GLsizei numTextureNames, const GLuint * textureNames)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "VDPAURegisterVideoSurfaceNV");
   return CALL_VDPAURegisterVideoSurfaceNV(ctx->CurrentServerDispatch, (vdpSurface, target, numTextureNames, textureNames));
}


/* VDPAURegisterOutputSurfaceNV: marshalled synchronously */
static GLintptr GLAPIENTRY
_mesa_marshal_VDPAURegisterOutputSurfaceNV(const GLvoid * vdpSurface, GLenum target, GLsizei numTextureNames, const GLuint * textureNames)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "VDPAURegisterOutputSurfaceNV");
   return CALL_VDPAURegisterOutputSurfaceNV(ctx->CurrentServerDispatch, (vdpSurface, target, numTextureNames, textureNames));
}


/* VDPAUIsSurfaceNV: marshalled synchronously */
static GLboolean GLAPIENTRY
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
uint32_t
_mesa_unmarshal_VDPAUUnregisterSurfaceNV(struct gl_context *ctx, const struct marshal_cmd_VDPAUUnregisterSurfaceNV *cmd)
{
   GLintptr surface = cmd->surface;
   CALL_VDPAUUnregisterSurfaceNV(ctx->CurrentServerDispatch, (surface));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VDPAUUnregisterSurfaceNV), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_VDPAUUnregisterSurfaceNV(GLintptr surface)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VDPAUUnregisterSurfaceNV);
   struct marshal_cmd_VDPAUUnregisterSurfaceNV *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VDPAUUnregisterSurfaceNV, cmd_size);
   cmd->surface = surface;
}


/* VDPAUGetSurfaceivNV: marshalled synchronously */
static void GLAPIENTRY
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
uint32_t
_mesa_unmarshal_VDPAUSurfaceAccessNV(struct gl_context *ctx, const struct marshal_cmd_VDPAUSurfaceAccessNV *cmd)
{
   GLintptr surface = cmd->surface;
   GLenum access = cmd->access;
   CALL_VDPAUSurfaceAccessNV(ctx->CurrentServerDispatch, (surface, access));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VDPAUSurfaceAccessNV), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_VDPAUSurfaceAccessNV(GLintptr surface, GLenum access)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VDPAUSurfaceAccessNV);
   struct marshal_cmd_VDPAUSurfaceAccessNV *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VDPAUSurfaceAccessNV, cmd_size);
   cmd->surface = surface;
   cmd->access = MIN2(access, 0xffff); /* clamped to 0xffff (invalid enum) */
}


/* VDPAUMapSurfacesNV: marshalled asynchronously */
struct marshal_cmd_VDPAUMapSurfacesNV
{
   struct marshal_cmd_base cmd_base;
   GLsizei numSurfaces;
   /* Next safe_mul(numSurfaces, 1 * sizeof(GLintptr)) bytes are GLintptr surfaces[numSurfaces] */
};
uint32_t
_mesa_unmarshal_VDPAUMapSurfacesNV(struct gl_context *ctx, const struct marshal_cmd_VDPAUMapSurfacesNV *cmd)
{
   GLsizei numSurfaces = cmd->numSurfaces;
   GLintptr *surfaces;
   const char *variable_data = (const char *) (cmd + 1);
   surfaces = (GLintptr *) variable_data;
   CALL_VDPAUMapSurfacesNV(ctx->CurrentServerDispatch, (numSurfaces, surfaces));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
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
uint32_t
_mesa_unmarshal_VDPAUUnmapSurfacesNV(struct gl_context *ctx, const struct marshal_cmd_VDPAUUnmapSurfacesNV *cmd)
{
   GLsizei numSurfaces = cmd->numSurfaces;
   GLintptr *surfaces;
   const char *variable_data = (const char *) (cmd + 1);
   surfaces = (GLintptr *) variable_data;
   CALL_VDPAUUnmapSurfacesNV(ctx->CurrentServerDispatch, (numSurfaces, surfaces));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
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
static void GLAPIENTRY
_mesa_marshal_GetUnsignedBytevEXT(GLenum pname, GLubyte * data)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetUnsignedBytevEXT");
   CALL_GetUnsignedBytevEXT(ctx->CurrentServerDispatch, (pname, data));
}


/* GetUnsignedBytei_vEXT: marshalled synchronously */
static void GLAPIENTRY
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
uint32_t
_mesa_unmarshal_DeleteMemoryObjectsEXT(struct gl_context *ctx, const struct marshal_cmd_DeleteMemoryObjectsEXT *cmd)
{
   GLsizei n = cmd->n;
   GLuint *memoryObjects;
   const char *variable_data = (const char *) (cmd + 1);
   memoryObjects = (GLuint *) variable_data;
   CALL_DeleteMemoryObjectsEXT(ctx->CurrentServerDispatch, (n, memoryObjects));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
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
static GLboolean GLAPIENTRY
_mesa_marshal_IsMemoryObjectEXT(GLuint memoryObject)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "IsMemoryObjectEXT");
   return CALL_IsMemoryObjectEXT(ctx->CurrentServerDispatch, (memoryObject));
}


/* CreateMemoryObjectsEXT: marshalled synchronously */
static void GLAPIENTRY
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
uint32_t
_mesa_unmarshal_MemoryObjectParameterivEXT(struct gl_context *ctx, const struct marshal_cmd_MemoryObjectParameterivEXT *cmd)
{
   GLuint memoryObject = cmd->memoryObject;
   GLenum pname = cmd->pname;
   GLint *params;
   const char *variable_data = (const char *) (cmd + 1);
   params = (GLint *) variable_data;
   CALL_MemoryObjectParameterivEXT(ctx->CurrentServerDispatch, (memoryObject, pname, params));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
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
   cmd->pname = MIN2(pname, 0xffff); /* clamped to 0xffff (invalid enum) */
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, params, params_size);
}


/* GetMemoryObjectParameterivEXT: marshalled synchronously */
static void GLAPIENTRY
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
uint32_t
_mesa_unmarshal_TexStorageMem2DEXT(struct gl_context *ctx, const struct marshal_cmd_TexStorageMem2DEXT *cmd)
{
   GLenum target = cmd->target;
   GLsizei levels = cmd->levels;
   GLenum internalFormat = cmd->internalFormat;
   GLsizei width = cmd->width;
   GLsizei height = cmd->height;
   GLuint memory = cmd->memory;
   GLuint64 offset = cmd->offset;
   CALL_TexStorageMem2DEXT(ctx->CurrentServerDispatch, (target, levels, internalFormat, width, height, memory, offset));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_TexStorageMem2DEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_TexStorageMem2DEXT(GLenum target, GLsizei levels, GLenum internalFormat, GLsizei width, GLsizei height, GLuint memory, GLuint64 offset)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TexStorageMem2DEXT);
   struct marshal_cmd_TexStorageMem2DEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexStorageMem2DEXT, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->levels = levels;
   cmd->internalFormat = MIN2(internalFormat, 0xffff); /* clamped to 0xffff (invalid enum) */
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
uint32_t
_mesa_unmarshal_TexStorageMem2DMultisampleEXT(struct gl_context *ctx, const struct marshal_cmd_TexStorageMem2DMultisampleEXT *cmd)
{
   GLenum target = cmd->target;
   GLsizei samples = cmd->samples;
   GLenum internalFormat = cmd->internalFormat;
   GLsizei width = cmd->width;
   GLsizei height = cmd->height;
   GLboolean fixedSampleLocations = cmd->fixedSampleLocations;
   GLuint memory = cmd->memory;
   GLuint64 offset = cmd->offset;
   CALL_TexStorageMem2DMultisampleEXT(ctx->CurrentServerDispatch, (target, samples, internalFormat, width, height, fixedSampleLocations, memory, offset));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_TexStorageMem2DMultisampleEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_TexStorageMem2DMultisampleEXT(GLenum target, GLsizei samples, GLenum internalFormat, GLsizei width, GLsizei height, GLboolean fixedSampleLocations, GLuint memory, GLuint64 offset)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TexStorageMem2DMultisampleEXT);
   struct marshal_cmd_TexStorageMem2DMultisampleEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexStorageMem2DMultisampleEXT, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->samples = samples;
   cmd->internalFormat = MIN2(internalFormat, 0xffff); /* clamped to 0xffff (invalid enum) */
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
uint32_t
_mesa_unmarshal_TexStorageMem3DEXT(struct gl_context *ctx, const struct marshal_cmd_TexStorageMem3DEXT *cmd)
{
   GLenum target = cmd->target;
   GLsizei levels = cmd->levels;
   GLenum internalFormat = cmd->internalFormat;
   GLsizei width = cmd->width;
   GLsizei height = cmd->height;
   GLsizei depth = cmd->depth;
   GLuint memory = cmd->memory;
   GLuint64 offset = cmd->offset;
   CALL_TexStorageMem3DEXT(ctx->CurrentServerDispatch, (target, levels, internalFormat, width, height, depth, memory, offset));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_TexStorageMem3DEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_TexStorageMem3DEXT(GLenum target, GLsizei levels, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLuint memory, GLuint64 offset)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TexStorageMem3DEXT);
   struct marshal_cmd_TexStorageMem3DEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexStorageMem3DEXT, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->levels = levels;
   cmd->internalFormat = MIN2(internalFormat, 0xffff); /* clamped to 0xffff (invalid enum) */
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
uint32_t
_mesa_unmarshal_TexStorageMem3DMultisampleEXT(struct gl_context *ctx, const struct marshal_cmd_TexStorageMem3DMultisampleEXT *cmd)
{
   GLenum target = cmd->target;
   GLsizei samples = cmd->samples;
   GLenum internalFormat = cmd->internalFormat;
   GLsizei width = cmd->width;
   GLsizei height = cmd->height;
   GLsizei depth = cmd->depth;
   GLboolean fixedSampleLocations = cmd->fixedSampleLocations;
   GLuint memory = cmd->memory;
   GLuint64 offset = cmd->offset;
   CALL_TexStorageMem3DMultisampleEXT(ctx->CurrentServerDispatch, (target, samples, internalFormat, width, height, depth, fixedSampleLocations, memory, offset));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_TexStorageMem3DMultisampleEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_TexStorageMem3DMultisampleEXT(GLenum target, GLsizei samples, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedSampleLocations, GLuint memory, GLuint64 offset)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TexStorageMem3DMultisampleEXT);
   struct marshal_cmd_TexStorageMem3DMultisampleEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexStorageMem3DMultisampleEXT, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->samples = samples;
   cmd->internalFormat = MIN2(internalFormat, 0xffff); /* clamped to 0xffff (invalid enum) */
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
uint32_t
_mesa_unmarshal_BufferStorageMemEXT(struct gl_context *ctx, const struct marshal_cmd_BufferStorageMemEXT *cmd)
{
   GLenum target = cmd->target;
   GLsizeiptr size = cmd->size;
   GLuint memory = cmd->memory;
   GLuint64 offset = cmd->offset;
   CALL_BufferStorageMemEXT(ctx->CurrentServerDispatch, (target, size, memory, offset));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_BufferStorageMemEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_BufferStorageMemEXT(GLenum target, GLsizeiptr size, GLuint memory, GLuint64 offset)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_BufferStorageMemEXT);
   struct marshal_cmd_BufferStorageMemEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_BufferStorageMemEXT, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
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
uint32_t
_mesa_unmarshal_TextureStorageMem2DEXT(struct gl_context *ctx, const struct marshal_cmd_TextureStorageMem2DEXT *cmd)
{
   GLenum texture = cmd->texture;
   GLsizei levels = cmd->levels;
   GLenum internalFormat = cmd->internalFormat;
   GLsizei width = cmd->width;
   GLsizei height = cmd->height;
   GLuint memory = cmd->memory;
   GLuint64 offset = cmd->offset;
   CALL_TextureStorageMem2DEXT(ctx->CurrentServerDispatch, (texture, levels, internalFormat, width, height, memory, offset));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_TextureStorageMem2DEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_TextureStorageMem2DEXT(GLenum texture, GLsizei levels, GLenum internalFormat, GLsizei width, GLsizei height, GLuint memory, GLuint64 offset)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TextureStorageMem2DEXT);
   struct marshal_cmd_TextureStorageMem2DEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TextureStorageMem2DEXT, cmd_size);
   cmd->texture = MIN2(texture, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->levels = levels;
   cmd->internalFormat = MIN2(internalFormat, 0xffff); /* clamped to 0xffff (invalid enum) */
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
uint32_t
_mesa_unmarshal_TextureStorageMem2DMultisampleEXT(struct gl_context *ctx, const struct marshal_cmd_TextureStorageMem2DMultisampleEXT *cmd)
{
   GLuint texture = cmd->texture;
   GLsizei samples = cmd->samples;
   GLenum internalFormat = cmd->internalFormat;
   GLsizei width = cmd->width;
   GLsizei height = cmd->height;
   GLboolean fixedSampleLocations = cmd->fixedSampleLocations;
   GLuint memory = cmd->memory;
   GLuint64 offset = cmd->offset;
   CALL_TextureStorageMem2DMultisampleEXT(ctx->CurrentServerDispatch, (texture, samples, internalFormat, width, height, fixedSampleLocations, memory, offset));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_TextureStorageMem2DMultisampleEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_TextureStorageMem2DMultisampleEXT(GLuint texture, GLsizei samples, GLenum internalFormat, GLsizei width, GLsizei height, GLboolean fixedSampleLocations, GLuint memory, GLuint64 offset)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TextureStorageMem2DMultisampleEXT);
   struct marshal_cmd_TextureStorageMem2DMultisampleEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TextureStorageMem2DMultisampleEXT, cmd_size);
   cmd->texture = texture;
   cmd->samples = samples;
   cmd->internalFormat = MIN2(internalFormat, 0xffff); /* clamped to 0xffff (invalid enum) */
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
uint32_t
_mesa_unmarshal_TextureStorageMem3DEXT(struct gl_context *ctx, const struct marshal_cmd_TextureStorageMem3DEXT *cmd)
{
   GLuint texture = cmd->texture;
   GLsizei levels = cmd->levels;
   GLenum internalFormat = cmd->internalFormat;
   GLsizei width = cmd->width;
   GLsizei height = cmd->height;
   GLsizei depth = cmd->depth;
   GLuint memory = cmd->memory;
   GLuint64 offset = cmd->offset;
   CALL_TextureStorageMem3DEXT(ctx->CurrentServerDispatch, (texture, levels, internalFormat, width, height, depth, memory, offset));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_TextureStorageMem3DEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_TextureStorageMem3DEXT(GLuint texture, GLsizei levels, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLuint memory, GLuint64 offset)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TextureStorageMem3DEXT);
   struct marshal_cmd_TextureStorageMem3DEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TextureStorageMem3DEXT, cmd_size);
   cmd->texture = texture;
   cmd->levels = levels;
   cmd->internalFormat = MIN2(internalFormat, 0xffff); /* clamped to 0xffff (invalid enum) */
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
uint32_t
_mesa_unmarshal_TextureStorageMem3DMultisampleEXT(struct gl_context *ctx, const struct marshal_cmd_TextureStorageMem3DMultisampleEXT *cmd)
{
   GLuint texture = cmd->texture;
   GLsizei samples = cmd->samples;
   GLenum internalFormat = cmd->internalFormat;
   GLsizei width = cmd->width;
   GLsizei height = cmd->height;
   GLsizei depth = cmd->depth;
   GLboolean fixedSampleLocations = cmd->fixedSampleLocations;
   GLuint memory = cmd->memory;
   GLuint64 offset = cmd->offset;
   CALL_TextureStorageMem3DMultisampleEXT(ctx->CurrentServerDispatch, (texture, samples, internalFormat, width, height, depth, fixedSampleLocations, memory, offset));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_TextureStorageMem3DMultisampleEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_TextureStorageMem3DMultisampleEXT(GLuint texture, GLsizei samples, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedSampleLocations, GLuint memory, GLuint64 offset)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TextureStorageMem3DMultisampleEXT);
   struct marshal_cmd_TextureStorageMem3DMultisampleEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TextureStorageMem3DMultisampleEXT, cmd_size);
   cmd->texture = texture;
   cmd->samples = samples;
   cmd->internalFormat = MIN2(internalFormat, 0xffff); /* clamped to 0xffff (invalid enum) */
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
uint32_t
_mesa_unmarshal_NamedBufferStorageMemEXT(struct gl_context *ctx, const struct marshal_cmd_NamedBufferStorageMemEXT *cmd)
{
   GLuint buffer = cmd->buffer;
   GLsizeiptr size = cmd->size;
   GLuint memory = cmd->memory;
   GLuint64 offset = cmd->offset;
   CALL_NamedBufferStorageMemEXT(ctx->CurrentServerDispatch, (buffer, size, memory, offset));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_NamedBufferStorageMemEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
uint32_t
_mesa_unmarshal_TexStorageMem1DEXT(struct gl_context *ctx, const struct marshal_cmd_TexStorageMem1DEXT *cmd)
{
   GLenum target = cmd->target;
   GLsizei levels = cmd->levels;
   GLenum internalFormat = cmd->internalFormat;
   GLsizei width = cmd->width;
   GLuint memory = cmd->memory;
   GLuint64 offset = cmd->offset;
   CALL_TexStorageMem1DEXT(ctx->CurrentServerDispatch, (target, levels, internalFormat, width, memory, offset));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_TexStorageMem1DEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_TexStorageMem1DEXT(GLenum target, GLsizei levels, GLenum internalFormat, GLsizei width, GLuint memory, GLuint64 offset)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TexStorageMem1DEXT);
   struct marshal_cmd_TexStorageMem1DEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexStorageMem1DEXT, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->levels = levels;
   cmd->internalFormat = MIN2(internalFormat, 0xffff); /* clamped to 0xffff (invalid enum) */
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
uint32_t
_mesa_unmarshal_TextureStorageMem1DEXT(struct gl_context *ctx, const struct marshal_cmd_TextureStorageMem1DEXT *cmd)
{
   GLuint texture = cmd->texture;
   GLsizei levels = cmd->levels;
   GLenum internalFormat = cmd->internalFormat;
   GLsizei width = cmd->width;
   GLuint memory = cmd->memory;
   GLuint64 offset = cmd->offset;
   CALL_TextureStorageMem1DEXT(ctx->CurrentServerDispatch, (texture, levels, internalFormat, width, memory, offset));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_TextureStorageMem1DEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_TextureStorageMem1DEXT(GLuint texture, GLsizei levels, GLenum internalFormat, GLsizei width, GLuint memory, GLuint64 offset)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TextureStorageMem1DEXT);
   struct marshal_cmd_TextureStorageMem1DEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TextureStorageMem1DEXT, cmd_size);
   cmd->texture = texture;
   cmd->levels = levels;
   cmd->internalFormat = MIN2(internalFormat, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->width = width;
   cmd->memory = memory;
   cmd->offset = offset;
}


/* GenSemaphoresEXT: marshalled synchronously */
static void GLAPIENTRY
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
uint32_t
_mesa_unmarshal_DeleteSemaphoresEXT(struct gl_context *ctx, const struct marshal_cmd_DeleteSemaphoresEXT *cmd)
{
   GLsizei n = cmd->n;
   GLuint *semaphores;
   const char *variable_data = (const char *) (cmd + 1);
   semaphores = (GLuint *) variable_data;
   CALL_DeleteSemaphoresEXT(ctx->CurrentServerDispatch, (n, semaphores));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
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
static GLboolean GLAPIENTRY
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
uint32_t
_mesa_unmarshal_SemaphoreParameterui64vEXT(struct gl_context *ctx, const struct marshal_cmd_SemaphoreParameterui64vEXT *cmd)
{
   GLuint semaphore = cmd->semaphore;
   GLenum pname = cmd->pname;
   GLuint64 *params;
   const char *variable_data = (const char *) (cmd + 1);
   params = (GLuint64 *) variable_data;
   CALL_SemaphoreParameterui64vEXT(ctx->CurrentServerDispatch, (semaphore, pname, params));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
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
   cmd->pname = MIN2(pname, 0xffff); /* clamped to 0xffff (invalid enum) */
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, params, params_size);
}


/* GetSemaphoreParameterui64vEXT: marshalled synchronously */
static void GLAPIENTRY
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
uint32_t
_mesa_unmarshal_WaitSemaphoreEXT(struct gl_context *ctx, const struct marshal_cmd_WaitSemaphoreEXT *cmd)
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
   CALL_WaitSemaphoreEXT(ctx->CurrentServerDispatch, (semaphore, numBufferBarriers, buffers, numTextureBarriers, textures, srcLayouts));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
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


/* SignalSemaphoreEXT: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_SignalSemaphoreEXT(GLuint semaphore, GLuint numBufferBarriers, const GLuint * buffers, GLuint numTextureBarriers, const GLuint * textures, const GLenum * dstLayouts)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "SignalSemaphoreEXT");
   CALL_SignalSemaphoreEXT(ctx->CurrentServerDispatch, (semaphore, numBufferBarriers, buffers, numTextureBarriers, textures, dstLayouts));
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
uint32_t
_mesa_unmarshal_ImportMemoryFdEXT(struct gl_context *ctx, const struct marshal_cmd_ImportMemoryFdEXT *cmd)
{
   GLuint memory = cmd->memory;
   GLuint64 size = cmd->size;
   GLenum handleType = cmd->handleType;
   GLint fd = cmd->fd;
   CALL_ImportMemoryFdEXT(ctx->CurrentServerDispatch, (memory, size, handleType, fd));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_ImportMemoryFdEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ImportMemoryFdEXT(GLuint memory, GLuint64 size, GLenum handleType, GLint fd)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ImportMemoryFdEXT);
   struct marshal_cmd_ImportMemoryFdEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ImportMemoryFdEXT, cmd_size);
   cmd->memory = memory;
   cmd->size = size;
   cmd->handleType = MIN2(handleType, 0xffff); /* clamped to 0xffff (invalid enum) */
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
uint32_t
_mesa_unmarshal_ImportSemaphoreFdEXT(struct gl_context *ctx, const struct marshal_cmd_ImportSemaphoreFdEXT *cmd)
{
   GLuint semaphore = cmd->semaphore;
   GLenum handleType = cmd->handleType;
   GLint fd = cmd->fd;
   CALL_ImportSemaphoreFdEXT(ctx->CurrentServerDispatch, (semaphore, handleType, fd));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_ImportSemaphoreFdEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ImportSemaphoreFdEXT(GLuint semaphore, GLenum handleType, GLint fd)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ImportSemaphoreFdEXT);
   struct marshal_cmd_ImportSemaphoreFdEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ImportSemaphoreFdEXT, cmd_size);
   cmd->semaphore = semaphore;
   cmd->handleType = MIN2(handleType, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->fd = fd;
}


/* ImportMemoryWin32HandleEXT: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_ImportMemoryWin32HandleEXT(GLuint memory, GLuint64 size, GLenum handleType, GLvoid * handle)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "ImportMemoryWin32HandleEXT");
   CALL_ImportMemoryWin32HandleEXT(ctx->CurrentServerDispatch, (memory, size, handleType, handle));
}


/* ImportMemoryWin32NameEXT: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_ImportMemoryWin32NameEXT(GLuint memory, GLuint64 size, GLenum handleType, const GLvoid * name)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "ImportMemoryWin32NameEXT");
   CALL_ImportMemoryWin32NameEXT(ctx->CurrentServerDispatch, (memory, size, handleType, name));
}


/* ImportSemaphoreWin32HandleEXT: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_ImportSemaphoreWin32HandleEXT(GLuint semaphore, GLenum handleType, GLvoid * handle)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "ImportSemaphoreWin32HandleEXT");
   CALL_ImportSemaphoreWin32HandleEXT(ctx->CurrentServerDispatch, (semaphore, handleType, handle));
}


/* ImportSemaphoreWin32NameEXT: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_ImportSemaphoreWin32NameEXT(GLuint semaphore, GLenum handleType, const GLvoid * handle)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "ImportSemaphoreWin32NameEXT");
   CALL_ImportSemaphoreWin32NameEXT(ctx->CurrentServerDispatch, (semaphore, handleType, handle));
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
uint32_t
_mesa_unmarshal_ViewportSwizzleNV(struct gl_context *ctx, const struct marshal_cmd_ViewportSwizzleNV *cmd)
{
   GLuint index = cmd->index;
   GLenum swizzlex = cmd->swizzlex;
   GLenum swizzley = cmd->swizzley;
   GLenum swizzlez = cmd->swizzlez;
   GLenum swizzlew = cmd->swizzlew;
   CALL_ViewportSwizzleNV(ctx->CurrentServerDispatch, (index, swizzlex, swizzley, swizzlez, swizzlew));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_ViewportSwizzleNV), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ViewportSwizzleNV(GLuint index, GLenum swizzlex, GLenum swizzley, GLenum swizzlez, GLenum swizzlew)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ViewportSwizzleNV);
   struct marshal_cmd_ViewportSwizzleNV *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ViewportSwizzleNV, cmd_size);
   cmd->index = index;
   cmd->swizzlex = MIN2(swizzlex, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->swizzley = MIN2(swizzley, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->swizzlez = MIN2(swizzlez, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->swizzlew = MIN2(swizzlew, 0xffff); /* clamped to 0xffff (invalid enum) */
}


/* Vertex2hNV: marshalled asynchronously */
struct marshal_cmd_Vertex2hNV
{
   struct marshal_cmd_base cmd_base;
   GLhalfNV x;
   GLhalfNV y;
};
uint32_t
_mesa_unmarshal_Vertex2hNV(struct gl_context *ctx, const struct marshal_cmd_Vertex2hNV *cmd)
{
   GLhalfNV x = cmd->x;
   GLhalfNV y = cmd->y;
   CALL_Vertex2hNV(ctx->CurrentServerDispatch, (x, y));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Vertex2hNV), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_Vertex2hNV(GLhalfNV x, GLhalfNV y)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Vertex2hNV);
   struct marshal_cmd_Vertex2hNV *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Vertex2hNV, cmd_size);
   cmd->x = x;
   cmd->y = y;
}


/* Vertex2hvNV: marshalled asynchronously */
struct marshal_cmd_Vertex2hvNV
{
   struct marshal_cmd_base cmd_base;
   GLhalfNV v[2];
};
uint32_t
_mesa_unmarshal_Vertex2hvNV(struct gl_context *ctx, const struct marshal_cmd_Vertex2hvNV *cmd)
{
   const GLhalfNV *v = cmd->v;
   CALL_Vertex2hvNV(ctx->CurrentServerDispatch, (v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Vertex2hvNV), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_Vertex2hvNV(const GLhalfNV * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Vertex2hvNV);
   struct marshal_cmd_Vertex2hvNV *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Vertex2hvNV, cmd_size);
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
uint32_t
_mesa_unmarshal_Vertex3hNV(struct gl_context *ctx, const struct marshal_cmd_Vertex3hNV *cmd)
{
   GLhalfNV x = cmd->x;
   GLhalfNV y = cmd->y;
   GLhalfNV z = cmd->z;
   CALL_Vertex3hNV(ctx->CurrentServerDispatch, (x, y, z));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Vertex3hNV), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_Vertex3hNV(GLhalfNV x, GLhalfNV y, GLhalfNV z)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Vertex3hNV);
   struct marshal_cmd_Vertex3hNV *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Vertex3hNV, cmd_size);
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
uint32_t
_mesa_unmarshal_Vertex3hvNV(struct gl_context *ctx, const struct marshal_cmd_Vertex3hvNV *cmd)
{
   const GLhalfNV *v = cmd->v;
   CALL_Vertex3hvNV(ctx->CurrentServerDispatch, (v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Vertex3hvNV), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_Vertex3hvNV(const GLhalfNV * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Vertex3hvNV);
   struct marshal_cmd_Vertex3hvNV *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Vertex3hvNV, cmd_size);
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
uint32_t
_mesa_unmarshal_Vertex4hNV(struct gl_context *ctx, const struct marshal_cmd_Vertex4hNV *cmd)
{
   GLhalfNV x = cmd->x;
   GLhalfNV y = cmd->y;
   GLhalfNV z = cmd->z;
   GLhalfNV w = cmd->w;
   CALL_Vertex4hNV(ctx->CurrentServerDispatch, (x, y, z, w));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Vertex4hNV), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_Vertex4hNV(GLhalfNV x, GLhalfNV y, GLhalfNV z, GLhalfNV w)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Vertex4hNV);
   struct marshal_cmd_Vertex4hNV *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Vertex4hNV, cmd_size);
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
uint32_t
_mesa_unmarshal_Vertex4hvNV(struct gl_context *ctx, const struct marshal_cmd_Vertex4hvNV *cmd)
{
   const GLhalfNV *v = cmd->v;
   CALL_Vertex4hvNV(ctx->CurrentServerDispatch, (v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Vertex4hvNV), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_Vertex4hvNV(const GLhalfNV * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Vertex4hvNV);
   struct marshal_cmd_Vertex4hvNV *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Vertex4hvNV, cmd_size);
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
uint32_t
_mesa_unmarshal_Normal3hNV(struct gl_context *ctx, const struct marshal_cmd_Normal3hNV *cmd)
{
   GLhalfNV nx = cmd->nx;
   GLhalfNV ny = cmd->ny;
   GLhalfNV nz = cmd->nz;
   CALL_Normal3hNV(ctx->CurrentServerDispatch, (nx, ny, nz));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Normal3hNV), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_Normal3hNV(GLhalfNV nx, GLhalfNV ny, GLhalfNV nz)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Normal3hNV);
   struct marshal_cmd_Normal3hNV *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Normal3hNV, cmd_size);
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
uint32_t
_mesa_unmarshal_Normal3hvNV(struct gl_context *ctx, const struct marshal_cmd_Normal3hvNV *cmd)
{
   const GLhalfNV *v = cmd->v;
   CALL_Normal3hvNV(ctx->CurrentServerDispatch, (v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Normal3hvNV), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_Normal3hvNV(const GLhalfNV * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Normal3hvNV);
   struct marshal_cmd_Normal3hvNV *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Normal3hvNV, cmd_size);
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
uint32_t
_mesa_unmarshal_Color3hNV(struct gl_context *ctx, const struct marshal_cmd_Color3hNV *cmd)
{
   GLhalfNV red = cmd->red;
   GLhalfNV green = cmd->green;
   GLhalfNV blue = cmd->blue;
   CALL_Color3hNV(ctx->CurrentServerDispatch, (red, green, blue));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Color3hNV), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_Color3hNV(GLhalfNV red, GLhalfNV green, GLhalfNV blue)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Color3hNV);
   struct marshal_cmd_Color3hNV *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Color3hNV, cmd_size);
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
uint32_t
_mesa_unmarshal_Color3hvNV(struct gl_context *ctx, const struct marshal_cmd_Color3hvNV *cmd)
{
   const GLhalfNV *v = cmd->v;
   CALL_Color3hvNV(ctx->CurrentServerDispatch, (v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Color3hvNV), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_Color3hvNV(const GLhalfNV * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Color3hvNV);
   struct marshal_cmd_Color3hvNV *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Color3hvNV, cmd_size);
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
uint32_t
_mesa_unmarshal_Color4hNV(struct gl_context *ctx, const struct marshal_cmd_Color4hNV *cmd)
{
   GLhalfNV red = cmd->red;
   GLhalfNV green = cmd->green;
   GLhalfNV blue = cmd->blue;
   GLhalfNV alpha = cmd->alpha;
   CALL_Color4hNV(ctx->CurrentServerDispatch, (red, green, blue, alpha));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Color4hNV), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_Color4hNV(GLhalfNV red, GLhalfNV green, GLhalfNV blue, GLhalfNV alpha)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Color4hNV);
   struct marshal_cmd_Color4hNV *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Color4hNV, cmd_size);
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
uint32_t
_mesa_unmarshal_Color4hvNV(struct gl_context *ctx, const struct marshal_cmd_Color4hvNV *cmd)
{
   const GLhalfNV *v = cmd->v;
   CALL_Color4hvNV(ctx->CurrentServerDispatch, (v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Color4hvNV), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_Color4hvNV(const GLhalfNV * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Color4hvNV);
   struct marshal_cmd_Color4hvNV *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Color4hvNV, cmd_size);
   memcpy(cmd->v, v, 4 * sizeof(GLhalfNV));
}


/* TexCoord1hNV: marshalled asynchronously */
struct marshal_cmd_TexCoord1hNV
{
   struct marshal_cmd_base cmd_base;
   GLhalfNV s;
};
uint32_t
_mesa_unmarshal_TexCoord1hNV(struct gl_context *ctx, const struct marshal_cmd_TexCoord1hNV *cmd)
{
   GLhalfNV s = cmd->s;
   CALL_TexCoord1hNV(ctx->CurrentServerDispatch, (s));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_TexCoord1hNV), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_TexCoord1hNV(GLhalfNV s)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TexCoord1hNV);
   struct marshal_cmd_TexCoord1hNV *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexCoord1hNV, cmd_size);
   cmd->s = s;
}


/* TexCoord1hvNV: marshalled asynchronously */
struct marshal_cmd_TexCoord1hvNV
{
   struct marshal_cmd_base cmd_base;
   GLhalfNV v[1];
};
uint32_t
_mesa_unmarshal_TexCoord1hvNV(struct gl_context *ctx, const struct marshal_cmd_TexCoord1hvNV *cmd)
{
   const GLhalfNV *v = cmd->v;
   CALL_TexCoord1hvNV(ctx->CurrentServerDispatch, (v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_TexCoord1hvNV), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_TexCoord1hvNV(const GLhalfNV * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TexCoord1hvNV);
   struct marshal_cmd_TexCoord1hvNV *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexCoord1hvNV, cmd_size);
   memcpy(cmd->v, v, 1 * sizeof(GLhalfNV));
}


/* TexCoord2hNV: marshalled asynchronously */
struct marshal_cmd_TexCoord2hNV
{
   struct marshal_cmd_base cmd_base;
   GLhalfNV s;
   GLhalfNV t;
};
uint32_t
_mesa_unmarshal_TexCoord2hNV(struct gl_context *ctx, const struct marshal_cmd_TexCoord2hNV *cmd)
{
   GLhalfNV s = cmd->s;
   GLhalfNV t = cmd->t;
   CALL_TexCoord2hNV(ctx->CurrentServerDispatch, (s, t));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_TexCoord2hNV), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_TexCoord2hNV(GLhalfNV s, GLhalfNV t)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TexCoord2hNV);
   struct marshal_cmd_TexCoord2hNV *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexCoord2hNV, cmd_size);
   cmd->s = s;
   cmd->t = t;
}


/* TexCoord2hvNV: marshalled asynchronously */
struct marshal_cmd_TexCoord2hvNV
{
   struct marshal_cmd_base cmd_base;
   GLhalfNV v[2];
};
uint32_t
_mesa_unmarshal_TexCoord2hvNV(struct gl_context *ctx, const struct marshal_cmd_TexCoord2hvNV *cmd)
{
   const GLhalfNV *v = cmd->v;
   CALL_TexCoord2hvNV(ctx->CurrentServerDispatch, (v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_TexCoord2hvNV), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_TexCoord2hvNV(const GLhalfNV * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TexCoord2hvNV);
   struct marshal_cmd_TexCoord2hvNV *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexCoord2hvNV, cmd_size);
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
uint32_t
_mesa_unmarshal_TexCoord3hNV(struct gl_context *ctx, const struct marshal_cmd_TexCoord3hNV *cmd)
{
   GLhalfNV s = cmd->s;
   GLhalfNV t = cmd->t;
   GLhalfNV r = cmd->r;
   CALL_TexCoord3hNV(ctx->CurrentServerDispatch, (s, t, r));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_TexCoord3hNV), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_TexCoord3hNV(GLhalfNV s, GLhalfNV t, GLhalfNV r)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TexCoord3hNV);
   struct marshal_cmd_TexCoord3hNV *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexCoord3hNV, cmd_size);
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
uint32_t
_mesa_unmarshal_TexCoord3hvNV(struct gl_context *ctx, const struct marshal_cmd_TexCoord3hvNV *cmd)
{
   const GLhalfNV *v = cmd->v;
   CALL_TexCoord3hvNV(ctx->CurrentServerDispatch, (v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_TexCoord3hvNV), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_TexCoord3hvNV(const GLhalfNV * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TexCoord3hvNV);
   struct marshal_cmd_TexCoord3hvNV *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexCoord3hvNV, cmd_size);
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
uint32_t
_mesa_unmarshal_TexCoord4hNV(struct gl_context *ctx, const struct marshal_cmd_TexCoord4hNV *cmd)
{
   GLhalfNV s = cmd->s;
   GLhalfNV t = cmd->t;
   GLhalfNV r = cmd->r;
   GLhalfNV q = cmd->q;
   CALL_TexCoord4hNV(ctx->CurrentServerDispatch, (s, t, r, q));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_TexCoord4hNV), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_TexCoord4hNV(GLhalfNV s, GLhalfNV t, GLhalfNV r, GLhalfNV q)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TexCoord4hNV);
   struct marshal_cmd_TexCoord4hNV *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexCoord4hNV, cmd_size);
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
uint32_t
_mesa_unmarshal_TexCoord4hvNV(struct gl_context *ctx, const struct marshal_cmd_TexCoord4hvNV *cmd)
{
   const GLhalfNV *v = cmd->v;
   CALL_TexCoord4hvNV(ctx->CurrentServerDispatch, (v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_TexCoord4hvNV), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_TexCoord4hvNV(const GLhalfNV * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TexCoord4hvNV);
   struct marshal_cmd_TexCoord4hvNV *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexCoord4hvNV, cmd_size);
   memcpy(cmd->v, v, 4 * sizeof(GLhalfNV));
}


/* MultiTexCoord1hNV: marshalled asynchronously */
struct marshal_cmd_MultiTexCoord1hNV
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLhalfNV s;
};
uint32_t
_mesa_unmarshal_MultiTexCoord1hNV(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord1hNV *cmd)
{
   GLenum target = cmd->target;
   GLhalfNV s = cmd->s;
   CALL_MultiTexCoord1hNV(ctx->CurrentServerDispatch, (target, s));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MultiTexCoord1hNV), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_MultiTexCoord1hNV(GLenum target, GLhalfNV s)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexCoord1hNV);
   struct marshal_cmd_MultiTexCoord1hNV *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexCoord1hNV, cmd_size);
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
uint32_t
_mesa_unmarshal_MultiTexCoord1hvNV(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord1hvNV *cmd)
{
   GLenum target = cmd->target;
   const GLhalfNV *v = cmd->v;
   CALL_MultiTexCoord1hvNV(ctx->CurrentServerDispatch, (target, v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MultiTexCoord1hvNV), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_MultiTexCoord1hvNV(GLenum target, const GLhalfNV * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexCoord1hvNV);
   struct marshal_cmd_MultiTexCoord1hvNV *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexCoord1hvNV, cmd_size);
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
uint32_t
_mesa_unmarshal_MultiTexCoord2hNV(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord2hNV *cmd)
{
   GLenum target = cmd->target;
   GLhalfNV s = cmd->s;
   GLhalfNV t = cmd->t;
   CALL_MultiTexCoord2hNV(ctx->CurrentServerDispatch, (target, s, t));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MultiTexCoord2hNV), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_MultiTexCoord2hNV(GLenum target, GLhalfNV s, GLhalfNV t)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexCoord2hNV);
   struct marshal_cmd_MultiTexCoord2hNV *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexCoord2hNV, cmd_size);
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
uint32_t
_mesa_unmarshal_MultiTexCoord2hvNV(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord2hvNV *cmd)
{
   GLenum target = cmd->target;
   const GLhalfNV *v = cmd->v;
   CALL_MultiTexCoord2hvNV(ctx->CurrentServerDispatch, (target, v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MultiTexCoord2hvNV), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_MultiTexCoord2hvNV(GLenum target, const GLhalfNV * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexCoord2hvNV);
   struct marshal_cmd_MultiTexCoord2hvNV *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexCoord2hvNV, cmd_size);
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
uint32_t
_mesa_unmarshal_MultiTexCoord3hNV(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord3hNV *cmd)
{
   GLenum target = cmd->target;
   GLhalfNV s = cmd->s;
   GLhalfNV t = cmd->t;
   GLhalfNV r = cmd->r;
   CALL_MultiTexCoord3hNV(ctx->CurrentServerDispatch, (target, s, t, r));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MultiTexCoord3hNV), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_MultiTexCoord3hNV(GLenum target, GLhalfNV s, GLhalfNV t, GLhalfNV r)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexCoord3hNV);
   struct marshal_cmd_MultiTexCoord3hNV *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexCoord3hNV, cmd_size);
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
uint32_t
_mesa_unmarshal_MultiTexCoord3hvNV(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord3hvNV *cmd)
{
   GLenum target = cmd->target;
   const GLhalfNV *v = cmd->v;
   CALL_MultiTexCoord3hvNV(ctx->CurrentServerDispatch, (target, v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MultiTexCoord3hvNV), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_MultiTexCoord3hvNV(GLenum target, const GLhalfNV * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexCoord3hvNV);
   struct marshal_cmd_MultiTexCoord3hvNV *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexCoord3hvNV, cmd_size);
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
uint32_t
_mesa_unmarshal_MultiTexCoord4hNV(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord4hNV *cmd)
{
   GLenum target = cmd->target;
   GLhalfNV s = cmd->s;
   GLhalfNV t = cmd->t;
   GLhalfNV r = cmd->r;
   GLhalfNV q = cmd->q;
   CALL_MultiTexCoord4hNV(ctx->CurrentServerDispatch, (target, s, t, r, q));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MultiTexCoord4hNV), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_MultiTexCoord4hNV(GLenum target, GLhalfNV s, GLhalfNV t, GLhalfNV r, GLhalfNV q)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexCoord4hNV);
   struct marshal_cmd_MultiTexCoord4hNV *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexCoord4hNV, cmd_size);
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
uint32_t
_mesa_unmarshal_MultiTexCoord4hvNV(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord4hvNV *cmd)
{
   GLenum target = cmd->target;
   const GLhalfNV *v = cmd->v;
   CALL_MultiTexCoord4hvNV(ctx->CurrentServerDispatch, (target, v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MultiTexCoord4hvNV), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_MultiTexCoord4hvNV(GLenum target, const GLhalfNV * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexCoord4hvNV);
   struct marshal_cmd_MultiTexCoord4hvNV *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexCoord4hvNV, cmd_size);
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
uint32_t
_mesa_unmarshal_VertexAttrib1hNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib1hNV *cmd)
{
   GLuint index = cmd->index;
   GLhalfNV x = cmd->x;
   CALL_VertexAttrib1hNV(ctx->CurrentServerDispatch, (index, x));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexAttrib1hNV), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_VertexAttrib1hNV(GLuint index, GLhalfNV x)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttrib1hNV);
   struct marshal_cmd_VertexAttrib1hNV *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttrib1hNV, cmd_size);
   cmd->index = index;
   cmd->x = x;
}


/* VertexAttrib1hvNV: marshalled asynchronously */
struct marshal_cmd_VertexAttrib1hvNV
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLhalfNV v[1];
};
uint32_t
_mesa_unmarshal_VertexAttrib1hvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib1hvNV *cmd)
{
   GLuint index = cmd->index;
   const GLhalfNV *v = cmd->v;
   CALL_VertexAttrib1hvNV(ctx->CurrentServerDispatch, (index, v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexAttrib1hvNV), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_VertexAttrib1hvNV(GLuint index, const GLhalfNV * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttrib1hvNV);
   struct marshal_cmd_VertexAttrib1hvNV *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttrib1hvNV, cmd_size);
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
uint32_t
_mesa_unmarshal_VertexAttrib2hNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib2hNV *cmd)
{
   GLuint index = cmd->index;
   GLhalfNV x = cmd->x;
   GLhalfNV y = cmd->y;
   CALL_VertexAttrib2hNV(ctx->CurrentServerDispatch, (index, x, y));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexAttrib2hNV), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_VertexAttrib2hNV(GLuint index, GLhalfNV x, GLhalfNV y)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttrib2hNV);
   struct marshal_cmd_VertexAttrib2hNV *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttrib2hNV, cmd_size);
   cmd->index = index;
   cmd->x = x;
   cmd->y = y;
}


/* VertexAttrib2hvNV: marshalled asynchronously */
struct marshal_cmd_VertexAttrib2hvNV
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLhalfNV v[2];
};
uint32_t
_mesa_unmarshal_VertexAttrib2hvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib2hvNV *cmd)
{
   GLuint index = cmd->index;
   const GLhalfNV *v = cmd->v;
   CALL_VertexAttrib2hvNV(ctx->CurrentServerDispatch, (index, v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexAttrib2hvNV), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_VertexAttrib2hvNV(GLuint index, const GLhalfNV * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttrib2hvNV);
   struct marshal_cmd_VertexAttrib2hvNV *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttrib2hvNV, cmd_size);
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
uint32_t
_mesa_unmarshal_VertexAttrib3hNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib3hNV *cmd)
{
   GLuint index = cmd->index;
   GLhalfNV x = cmd->x;
   GLhalfNV y = cmd->y;
   GLhalfNV z = cmd->z;
   CALL_VertexAttrib3hNV(ctx->CurrentServerDispatch, (index, x, y, z));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexAttrib3hNV), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_VertexAttrib3hNV(GLuint index, GLhalfNV x, GLhalfNV y, GLhalfNV z)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttrib3hNV);
   struct marshal_cmd_VertexAttrib3hNV *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttrib3hNV, cmd_size);
   cmd->index = index;
   cmd->x = x;
   cmd->y = y;
   cmd->z = z;
}


/* VertexAttrib3hvNV: marshalled asynchronously */
struct marshal_cmd_VertexAttrib3hvNV
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLhalfNV v[3];
};
uint32_t
_mesa_unmarshal_VertexAttrib3hvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib3hvNV *cmd)
{
   GLuint index = cmd->index;
   const GLhalfNV *v = cmd->v;
   CALL_VertexAttrib3hvNV(ctx->CurrentServerDispatch, (index, v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexAttrib3hvNV), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_VertexAttrib3hvNV(GLuint index, const GLhalfNV * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttrib3hvNV);
   struct marshal_cmd_VertexAttrib3hvNV *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttrib3hvNV, cmd_size);
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
uint32_t
_mesa_unmarshal_VertexAttrib4hNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4hNV *cmd)
{
   GLuint index = cmd->index;
   GLhalfNV x = cmd->x;
   GLhalfNV y = cmd->y;
   GLhalfNV z = cmd->z;
   GLhalfNV w = cmd->w;
   CALL_VertexAttrib4hNV(ctx->CurrentServerDispatch, (index, x, y, z, w));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexAttrib4hNV), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_VertexAttrib4hNV(GLuint index, GLhalfNV x, GLhalfNV y, GLhalfNV z, GLhalfNV w)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttrib4hNV);
   struct marshal_cmd_VertexAttrib4hNV *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttrib4hNV, cmd_size);
   cmd->index = index;
   cmd->x = x;
   cmd->y = y;
   cmd->z = z;
   cmd->w = w;
}


/* VertexAttrib4hvNV: marshalled asynchronously */
struct marshal_cmd_VertexAttrib4hvNV
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLhalfNV v[4];
};
uint32_t
_mesa_unmarshal_VertexAttrib4hvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4hvNV *cmd)
{
   GLuint index = cmd->index;
   const GLhalfNV *v = cmd->v;
   CALL_VertexAttrib4hvNV(ctx->CurrentServerDispatch, (index, v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexAttrib4hvNV), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_VertexAttrib4hvNV(GLuint index, const GLhalfNV * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttrib4hvNV);
   struct marshal_cmd_VertexAttrib4hvNV *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttrib4hvNV, cmd_size);
   cmd->index = index;
   memcpy(cmd->v, v, 4 * sizeof(GLhalfNV));
}


/* VertexAttribs1hvNV: marshalled asynchronously */
struct marshal_cmd_VertexAttribs1hvNV
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLsizei n;
   /* Next safe_mul(n, 1 * sizeof(GLhalfNV)) bytes are GLhalfNV v[n] */
};
uint32_t
_mesa_unmarshal_VertexAttribs1hvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttribs1hvNV *cmd)
{
   GLuint index = cmd->index;
   GLsizei n = cmd->n;
   GLhalfNV *v;
   const char *variable_data = (const char *) (cmd + 1);
   v = (GLhalfNV *) variable_data;
   CALL_VertexAttribs1hvNV(ctx->CurrentServerDispatch, (index, n, v));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_VertexAttribs1hvNV(GLuint index, GLsizei n, const GLhalfNV * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int v_size = safe_mul(n, 1 * sizeof(GLhalfNV));
   int cmd_size = sizeof(struct marshal_cmd_VertexAttribs1hvNV) + v_size;
   struct marshal_cmd_VertexAttribs1hvNV *cmd;
   if (unlikely(v_size < 0 || (v_size > 0 && !v) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "VertexAttribs1hvNV");
      CALL_VertexAttribs1hvNV(ctx->CurrentServerDispatch, (index, n, v));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttribs1hvNV, cmd_size);
   cmd->index = index;
   cmd->n = n;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, v, v_size);
}


/* VertexAttribs2hvNV: marshalled asynchronously */
struct marshal_cmd_VertexAttribs2hvNV
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLsizei n;
   /* Next safe_mul(n, 2 * sizeof(GLhalfNV)) bytes are GLhalfNV v[n][2] */
};
uint32_t
_mesa_unmarshal_VertexAttribs2hvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttribs2hvNV *cmd)
{
   GLuint index = cmd->index;
   GLsizei n = cmd->n;
   GLhalfNV *v;
   const char *variable_data = (const char *) (cmd + 1);
   v = (GLhalfNV *) variable_data;
   CALL_VertexAttribs2hvNV(ctx->CurrentServerDispatch, (index, n, v));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_VertexAttribs2hvNV(GLuint index, GLsizei n, const GLhalfNV * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int v_size = safe_mul(n, 2 * sizeof(GLhalfNV));
   int cmd_size = sizeof(struct marshal_cmd_VertexAttribs2hvNV) + v_size;
   struct marshal_cmd_VertexAttribs2hvNV *cmd;
   if (unlikely(v_size < 0 || (v_size > 0 && !v) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "VertexAttribs2hvNV");
      CALL_VertexAttribs2hvNV(ctx->CurrentServerDispatch, (index, n, v));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttribs2hvNV, cmd_size);
   cmd->index = index;
   cmd->n = n;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, v, v_size);
}


/* VertexAttribs3hvNV: marshalled asynchronously */
struct marshal_cmd_VertexAttribs3hvNV
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLsizei n;
   /* Next safe_mul(n, 3 * sizeof(GLhalfNV)) bytes are GLhalfNV v[n][3] */
};
uint32_t
_mesa_unmarshal_VertexAttribs3hvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttribs3hvNV *cmd)
{
   GLuint index = cmd->index;
   GLsizei n = cmd->n;
   GLhalfNV *v;
   const char *variable_data = (const char *) (cmd + 1);
   v = (GLhalfNV *) variable_data;
   CALL_VertexAttribs3hvNV(ctx->CurrentServerDispatch, (index, n, v));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_VertexAttribs3hvNV(GLuint index, GLsizei n, const GLhalfNV * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int v_size = safe_mul(n, 3 * sizeof(GLhalfNV));
   int cmd_size = sizeof(struct marshal_cmd_VertexAttribs3hvNV) + v_size;
   struct marshal_cmd_VertexAttribs3hvNV *cmd;
   if (unlikely(v_size < 0 || (v_size > 0 && !v) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "VertexAttribs3hvNV");
      CALL_VertexAttribs3hvNV(ctx->CurrentServerDispatch, (index, n, v));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttribs3hvNV, cmd_size);
   cmd->index = index;
   cmd->n = n;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, v, v_size);
}


/* VertexAttribs4hvNV: marshalled asynchronously */
struct marshal_cmd_VertexAttribs4hvNV
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLsizei n;
   /* Next safe_mul(n, 4 * sizeof(GLhalfNV)) bytes are GLhalfNV v[n][4] */
};
uint32_t
_mesa_unmarshal_VertexAttribs4hvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttribs4hvNV *cmd)
{
   GLuint index = cmd->index;
   GLsizei n = cmd->n;
   GLhalfNV *v;
   const char *variable_data = (const char *) (cmd + 1);
   v = (GLhalfNV *) variable_data;
   CALL_VertexAttribs4hvNV(ctx->CurrentServerDispatch, (index, n, v));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_VertexAttribs4hvNV(GLuint index, GLsizei n, const GLhalfNV * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int v_size = safe_mul(n, 4 * sizeof(GLhalfNV));
   int cmd_size = sizeof(struct marshal_cmd_VertexAttribs4hvNV) + v_size;
   struct marshal_cmd_VertexAttribs4hvNV *cmd;
   if (unlikely(v_size < 0 || (v_size > 0 && !v) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "VertexAttribs4hvNV");
      CALL_VertexAttribs4hvNV(ctx->CurrentServerDispatch, (index, n, v));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttribs4hvNV, cmd_size);
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
uint32_t
_mesa_unmarshal_FogCoordhNV(struct gl_context *ctx, const struct marshal_cmd_FogCoordhNV *cmd)
{
   GLhalfNV x = cmd->x;
   CALL_FogCoordhNV(ctx->CurrentServerDispatch, (x));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_FogCoordhNV), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_FogCoordhNV(GLhalfNV x)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_FogCoordhNV);
   struct marshal_cmd_FogCoordhNV *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_FogCoordhNV, cmd_size);
   cmd->x = x;
}


/* FogCoordhvNV: marshalled asynchronously */
struct marshal_cmd_FogCoordhvNV
{
   struct marshal_cmd_base cmd_base;
   GLhalfNV v[1];
};
uint32_t
_mesa_unmarshal_FogCoordhvNV(struct gl_context *ctx, const struct marshal_cmd_FogCoordhvNV *cmd)
{
   const GLhalfNV *v = cmd->v;
   CALL_FogCoordhvNV(ctx->CurrentServerDispatch, (v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_FogCoordhvNV), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_FogCoordhvNV(const GLhalfNV * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_FogCoordhvNV);
   struct marshal_cmd_FogCoordhvNV *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_FogCoordhvNV, cmd_size);
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
uint32_t
_mesa_unmarshal_SecondaryColor3hNV(struct gl_context *ctx, const struct marshal_cmd_SecondaryColor3hNV *cmd)
{
   GLhalfNV red = cmd->red;
   GLhalfNV green = cmd->green;
   GLhalfNV blue = cmd->blue;
   CALL_SecondaryColor3hNV(ctx->CurrentServerDispatch, (red, green, blue));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_SecondaryColor3hNV), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_SecondaryColor3hNV(GLhalfNV red, GLhalfNV green, GLhalfNV blue)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_SecondaryColor3hNV);
   struct marshal_cmd_SecondaryColor3hNV *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_SecondaryColor3hNV, cmd_size);
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
uint32_t
_mesa_unmarshal_SecondaryColor3hvNV(struct gl_context *ctx, const struct marshal_cmd_SecondaryColor3hvNV *cmd)
{
   const GLhalfNV *v = cmd->v;
   CALL_SecondaryColor3hvNV(ctx->CurrentServerDispatch, (v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_SecondaryColor3hvNV), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_SecondaryColor3hvNV(const GLhalfNV * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_SecondaryColor3hvNV);
   struct marshal_cmd_SecondaryColor3hvNV *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_SecondaryColor3hvNV, cmd_size);
   memcpy(cmd->v, v, 3 * sizeof(GLhalfNV));
}


/* MemoryBarrierByRegion: marshalled asynchronously */
struct marshal_cmd_MemoryBarrierByRegion
{
   struct marshal_cmd_base cmd_base;
   GLbitfield barriers;
};
uint32_t
_mesa_unmarshal_MemoryBarrierByRegion(struct gl_context *ctx, const struct marshal_cmd_MemoryBarrierByRegion *cmd)
{
   GLbitfield barriers = cmd->barriers;
   CALL_MemoryBarrierByRegion(ctx->CurrentServerDispatch, (barriers));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MemoryBarrierByRegion), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
uint32_t
_mesa_unmarshal_AlphaFuncx(struct gl_context *ctx, const struct marshal_cmd_AlphaFuncx *cmd)
{
   GLenum func = cmd->func;
   GLclampx ref = cmd->ref;
   CALL_AlphaFuncx(ctx->CurrentServerDispatch, (func, ref));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_AlphaFuncx), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_AlphaFuncx(GLenum func, GLclampx ref)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_AlphaFuncx);
   struct marshal_cmd_AlphaFuncx *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_AlphaFuncx, cmd_size);
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
uint32_t
_mesa_unmarshal_ClearColorx(struct gl_context *ctx, const struct marshal_cmd_ClearColorx *cmd)
{
   GLclampx red = cmd->red;
   GLclampx green = cmd->green;
   GLclampx blue = cmd->blue;
   GLclampx alpha = cmd->alpha;
   CALL_ClearColorx(ctx->CurrentServerDispatch, (red, green, blue, alpha));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_ClearColorx), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
uint32_t
_mesa_unmarshal_ClearDepthx(struct gl_context *ctx, const struct marshal_cmd_ClearDepthx *cmd)
{
   GLclampx depth = cmd->depth;
   CALL_ClearDepthx(ctx->CurrentServerDispatch, (depth));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_ClearDepthx), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
uint32_t
_mesa_unmarshal_Color4x(struct gl_context *ctx, const struct marshal_cmd_Color4x *cmd)
{
   GLfixed red = cmd->red;
   GLfixed green = cmd->green;
   GLfixed blue = cmd->blue;
   GLfixed alpha = cmd->alpha;
   CALL_Color4x(ctx->CurrentServerDispatch, (red, green, blue, alpha));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Color4x), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
uint32_t
_mesa_unmarshal_DepthRangex(struct gl_context *ctx, const struct marshal_cmd_DepthRangex *cmd)
{
   GLclampx zNear = cmd->zNear;
   GLclampx zFar = cmd->zFar;
   CALL_DepthRangex(ctx->CurrentServerDispatch, (zNear, zFar));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_DepthRangex), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
uint32_t
_mesa_unmarshal_Fogx(struct gl_context *ctx, const struct marshal_cmd_Fogx *cmd)
{
   GLenum pname = cmd->pname;
   GLfixed param = cmd->param;
   CALL_Fogx(ctx->CurrentServerDispatch, (pname, param));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Fogx), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_Fogx(GLenum pname, GLfixed param)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Fogx);
   struct marshal_cmd_Fogx *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Fogx, cmd_size);
   cmd->pname = MIN2(pname, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->param = param;
}


/* Fogxv: marshalled asynchronously */
struct marshal_cmd_Fogxv
{
   struct marshal_cmd_base cmd_base;
   GLenum16 pname;
   /* Next safe_mul(_mesa_fog_enum_to_count(pname), 1 * sizeof(GLfixed)) bytes are GLfixed params[None] */
};
uint32_t
_mesa_unmarshal_Fogxv(struct gl_context *ctx, const struct marshal_cmd_Fogxv *cmd)
{
   GLenum pname = cmd->pname;
   GLfixed *params;
   const char *variable_data = (const char *) (cmd + 1);
   params = (GLfixed *) variable_data;
   CALL_Fogxv(ctx->CurrentServerDispatch, (pname, params));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
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
uint32_t
_mesa_unmarshal_Frustumx(struct gl_context *ctx, const struct marshal_cmd_Frustumx *cmd)
{
   GLfixed left = cmd->left;
   GLfixed right = cmd->right;
   GLfixed bottom = cmd->bottom;
   GLfixed top = cmd->top;
   GLfixed zNear = cmd->zNear;
   GLfixed zFar = cmd->zFar;
   CALL_Frustumx(ctx->CurrentServerDispatch, (left, right, bottom, top, zNear, zFar));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Frustumx), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
uint32_t
_mesa_unmarshal_LightModelx(struct gl_context *ctx, const struct marshal_cmd_LightModelx *cmd)
{
   GLenum pname = cmd->pname;
   GLfixed param = cmd->param;
   CALL_LightModelx(ctx->CurrentServerDispatch, (pname, param));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_LightModelx), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_LightModelx(GLenum pname, GLfixed param)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_LightModelx);
   struct marshal_cmd_LightModelx *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_LightModelx, cmd_size);
   cmd->pname = MIN2(pname, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->param = param;
}


/* LightModelxv: marshalled asynchronously */
struct marshal_cmd_LightModelxv
{
   struct marshal_cmd_base cmd_base;
   GLenum16 pname;
   /* Next safe_mul(_mesa_light_model_enum_to_count(pname), 1 * sizeof(GLfixed)) bytes are GLfixed params[None] */
};
uint32_t
_mesa_unmarshal_LightModelxv(struct gl_context *ctx, const struct marshal_cmd_LightModelxv *cmd)
{
   GLenum pname = cmd->pname;
   GLfixed *params;
   const char *variable_data = (const char *) (cmd + 1);
   params = (GLfixed *) variable_data;
   CALL_LightModelxv(ctx->CurrentServerDispatch, (pname, params));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
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
uint32_t
_mesa_unmarshal_Lightx(struct gl_context *ctx, const struct marshal_cmd_Lightx *cmd)
{
   GLenum light = cmd->light;
   GLenum pname = cmd->pname;
   GLfixed param = cmd->param;
   CALL_Lightx(ctx->CurrentServerDispatch, (light, pname, param));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Lightx), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_Lightx(GLenum light, GLenum pname, GLfixed param)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Lightx);
   struct marshal_cmd_Lightx *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Lightx, cmd_size);
   cmd->light = MIN2(light, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->pname = MIN2(pname, 0xffff); /* clamped to 0xffff (invalid enum) */
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
uint32_t
_mesa_unmarshal_Lightxv(struct gl_context *ctx, const struct marshal_cmd_Lightxv *cmd)
{
   GLenum light = cmd->light;
   GLenum pname = cmd->pname;
   GLfixed *params;
   const char *variable_data = (const char *) (cmd + 1);
   params = (GLfixed *) variable_data;
   CALL_Lightxv(ctx->CurrentServerDispatch, (light, pname, params));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
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
uint32_t
_mesa_unmarshal_LineWidthx(struct gl_context *ctx, const struct marshal_cmd_LineWidthx *cmd)
{
   GLfixed width = cmd->width;
   CALL_LineWidthx(ctx->CurrentServerDispatch, (width));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_LineWidthx), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
uint32_t
_mesa_unmarshal_LoadMatrixx(struct gl_context *ctx, const struct marshal_cmd_LoadMatrixx *cmd)
{
   const GLfixed *m = cmd->m;
   CALL_LoadMatrixx(ctx->CurrentServerDispatch, (m));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_LoadMatrixx), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
uint32_t
_mesa_unmarshal_Materialx(struct gl_context *ctx, const struct marshal_cmd_Materialx *cmd)
{
   GLenum face = cmd->face;
   GLenum pname = cmd->pname;
   GLfixed param = cmd->param;
   CALL_Materialx(ctx->CurrentServerDispatch, (face, pname, param));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Materialx), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_Materialx(GLenum face, GLenum pname, GLfixed param)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Materialx);
   struct marshal_cmd_Materialx *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Materialx, cmd_size);
   cmd->face = MIN2(face, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->pname = MIN2(pname, 0xffff); /* clamped to 0xffff (invalid enum) */
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
uint32_t
_mesa_unmarshal_Materialxv(struct gl_context *ctx, const struct marshal_cmd_Materialxv *cmd)
{
   GLenum face = cmd->face;
   GLenum pname = cmd->pname;
   GLfixed *params;
   const char *variable_data = (const char *) (cmd + 1);
   params = (GLfixed *) variable_data;
   CALL_Materialxv(ctx->CurrentServerDispatch, (face, pname, params));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
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
uint32_t
_mesa_unmarshal_MultMatrixx(struct gl_context *ctx, const struct marshal_cmd_MultMatrixx *cmd)
{
   const GLfixed *m = cmd->m;
   CALL_MultMatrixx(ctx->CurrentServerDispatch, (m));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MultMatrixx), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
uint32_t
_mesa_unmarshal_MultiTexCoord4x(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord4x *cmd)
{
   GLenum target = cmd->target;
   GLfixed s = cmd->s;
   GLfixed t = cmd->t;
   GLfixed r = cmd->r;
   GLfixed q = cmd->q;
   CALL_MultiTexCoord4x(ctx->CurrentServerDispatch, (target, s, t, r, q));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MultiTexCoord4x), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_MultiTexCoord4x(GLenum target, GLfixed s, GLfixed t, GLfixed r, GLfixed q)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexCoord4x);
   struct marshal_cmd_MultiTexCoord4x *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexCoord4x, cmd_size);
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
uint32_t
_mesa_unmarshal_Normal3x(struct gl_context *ctx, const struct marshal_cmd_Normal3x *cmd)
{
   GLfixed nx = cmd->nx;
   GLfixed ny = cmd->ny;
   GLfixed nz = cmd->nz;
   CALL_Normal3x(ctx->CurrentServerDispatch, (nx, ny, nz));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Normal3x), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
uint32_t
_mesa_unmarshal_Orthox(struct gl_context *ctx, const struct marshal_cmd_Orthox *cmd)
{
   GLfixed left = cmd->left;
   GLfixed right = cmd->right;
   GLfixed bottom = cmd->bottom;
   GLfixed top = cmd->top;
   GLfixed zNear = cmd->zNear;
   GLfixed zFar = cmd->zFar;
   CALL_Orthox(ctx->CurrentServerDispatch, (left, right, bottom, top, zNear, zFar));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Orthox), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
uint32_t
_mesa_unmarshal_PointSizex(struct gl_context *ctx, const struct marshal_cmd_PointSizex *cmd)
{
   GLfixed size = cmd->size;
   CALL_PointSizex(ctx->CurrentServerDispatch, (size));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_PointSizex), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
uint32_t
_mesa_unmarshal_PolygonOffsetx(struct gl_context *ctx, const struct marshal_cmd_PolygonOffsetx *cmd)
{
   GLfixed factor = cmd->factor;
   GLfixed units = cmd->units;
   CALL_PolygonOffsetx(ctx->CurrentServerDispatch, (factor, units));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_PolygonOffsetx), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
uint32_t
_mesa_unmarshal_Rotatex(struct gl_context *ctx, const struct marshal_cmd_Rotatex *cmd)
{
   GLfixed angle = cmd->angle;
   GLfixed x = cmd->x;
   GLfixed y = cmd->y;
   GLfixed z = cmd->z;
   CALL_Rotatex(ctx->CurrentServerDispatch, (angle, x, y, z));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Rotatex), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
uint32_t
_mesa_unmarshal_SampleCoveragex(struct gl_context *ctx, const struct marshal_cmd_SampleCoveragex *cmd)
{
   GLclampx value = cmd->value;
   GLboolean invert = cmd->invert;
   CALL_SampleCoveragex(ctx->CurrentServerDispatch, (value, invert));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_SampleCoveragex), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
uint32_t
_mesa_unmarshal_Scalex(struct gl_context *ctx, const struct marshal_cmd_Scalex *cmd)
{
   GLfixed x = cmd->x;
   GLfixed y = cmd->y;
   GLfixed z = cmd->z;
   CALL_Scalex(ctx->CurrentServerDispatch, (x, y, z));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Scalex), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
uint32_t
_mesa_unmarshal_TexEnvx(struct gl_context *ctx, const struct marshal_cmd_TexEnvx *cmd)
{
   GLenum target = cmd->target;
   GLenum pname = cmd->pname;
   GLfixed param = cmd->param;
   CALL_TexEnvx(ctx->CurrentServerDispatch, (target, pname, param));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_TexEnvx), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_TexEnvx(GLenum target, GLenum pname, GLfixed param)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TexEnvx);
   struct marshal_cmd_TexEnvx *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexEnvx, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->pname = MIN2(pname, 0xffff); /* clamped to 0xffff (invalid enum) */
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
uint32_t
_mesa_unmarshal_TexEnvxv(struct gl_context *ctx, const struct marshal_cmd_TexEnvxv *cmd)
{
   GLenum target = cmd->target;
   GLenum pname = cmd->pname;
   GLfixed *params;
   const char *variable_data = (const char *) (cmd + 1);
   params = (GLfixed *) variable_data;
   CALL_TexEnvxv(ctx->CurrentServerDispatch, (target, pname, params));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
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
uint32_t
_mesa_unmarshal_TexParameterx(struct gl_context *ctx, const struct marshal_cmd_TexParameterx *cmd)
{
   GLenum target = cmd->target;
   GLenum pname = cmd->pname;
   GLfixed param = cmd->param;
   CALL_TexParameterx(ctx->CurrentServerDispatch, (target, pname, param));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_TexParameterx), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_TexParameterx(GLenum target, GLenum pname, GLfixed param)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TexParameterx);
   struct marshal_cmd_TexParameterx *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexParameterx, cmd_size);
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
uint32_t
_mesa_unmarshal_Translatex(struct gl_context *ctx, const struct marshal_cmd_Translatex *cmd)
{
   GLfixed x = cmd->x;
   GLfixed y = cmd->y;
   GLfixed z = cmd->z;
   CALL_Translatex(ctx->CurrentServerDispatch, (x, y, z));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Translatex), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
uint32_t
_mesa_unmarshal_ClipPlanex(struct gl_context *ctx, const struct marshal_cmd_ClipPlanex *cmd)
{
   GLenum plane = cmd->plane;
   const GLfixed *equation = cmd->equation;
   CALL_ClipPlanex(ctx->CurrentServerDispatch, (plane, equation));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_ClipPlanex), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ClipPlanex(GLenum plane, const GLfixed * equation)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ClipPlanex);
   struct marshal_cmd_ClipPlanex *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ClipPlanex, cmd_size);
   cmd->plane = MIN2(plane, 0xffff); /* clamped to 0xffff (invalid enum) */
   memcpy(cmd->equation, equation, 4 * sizeof(GLfixed));
}


/* GetClipPlanex: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetClipPlanex(GLenum plane, GLfixed * equation)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetClipPlanex");
   CALL_GetClipPlanex(ctx->CurrentServerDispatch, (plane, equation));
}


/* GetFixedv: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetFixedv(GLenum pname, GLfixed * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetFixedv");
   CALL_GetFixedv(ctx->CurrentServerDispatch, (pname, params));
}


/* GetLightxv: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetLightxv(GLenum light, GLenum pname, GLfixed * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetLightxv");
   CALL_GetLightxv(ctx->CurrentServerDispatch, (light, pname, params));
}


/* GetMaterialxv: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetMaterialxv(GLenum face, GLenum pname, GLfixed * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetMaterialxv");
   CALL_GetMaterialxv(ctx->CurrentServerDispatch, (face, pname, params));
}


/* GetTexEnvxv: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetTexEnvxv(GLenum target, GLenum pname, GLfixed * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetTexEnvxv");
   CALL_GetTexEnvxv(ctx->CurrentServerDispatch, (target, pname, params));
}


/* GetTexParameterxv: marshalled synchronously */
static void GLAPIENTRY
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
uint32_t
_mesa_unmarshal_PointParameterx(struct gl_context *ctx, const struct marshal_cmd_PointParameterx *cmd)
{
   GLenum pname = cmd->pname;
   GLfixed param = cmd->param;
   CALL_PointParameterx(ctx->CurrentServerDispatch, (pname, param));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_PointParameterx), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_PointParameterx(GLenum pname, GLfixed param)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_PointParameterx);
   struct marshal_cmd_PointParameterx *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_PointParameterx, cmd_size);
   cmd->pname = MIN2(pname, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->param = param;
}


/* PointParameterxv: marshalled asynchronously */
struct marshal_cmd_PointParameterxv
{
   struct marshal_cmd_base cmd_base;
   GLenum16 pname;
   /* Next safe_mul(_mesa_point_param_enum_to_count(pname), 1 * sizeof(GLfixed)) bytes are GLfixed params[_mesa_point_param_enum_to_count(pname)] */
};
uint32_t
_mesa_unmarshal_PointParameterxv(struct gl_context *ctx, const struct marshal_cmd_PointParameterxv *cmd)
{
   GLenum pname = cmd->pname;
   GLfixed *params;
   const char *variable_data = (const char *) (cmd + 1);
   params = (GLfixed *) variable_data;
   CALL_PointParameterxv(ctx->CurrentServerDispatch, (pname, params));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
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
   cmd->pname = MIN2(pname, 0xffff); /* clamped to 0xffff (invalid enum) */
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
uint32_t
_mesa_unmarshal_TexParameterxv(struct gl_context *ctx, const struct marshal_cmd_TexParameterxv *cmd)
{
   GLenum target = cmd->target;
   GLenum pname = cmd->pname;
   GLfixed *params;
   const char *variable_data = (const char *) (cmd + 1);
   params = (GLfixed *) variable_data;
   CALL_TexParameterxv(ctx->CurrentServerDispatch, (target, pname, params));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
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
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->pname = MIN2(pname, 0xffff); /* clamped to 0xffff (invalid enum) */
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, params, params_size);
}


/* GetTexGenxvOES: marshalled synchronously */
static void GLAPIENTRY
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
uint32_t
_mesa_unmarshal_TexGenxOES(struct gl_context *ctx, const struct marshal_cmd_TexGenxOES *cmd)
{
   GLenum coord = cmd->coord;
   GLenum pname = cmd->pname;
   GLint param = cmd->param;
   CALL_TexGenxOES(ctx->CurrentServerDispatch, (coord, pname, param));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_TexGenxOES), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_TexGenxOES(GLenum coord, GLenum pname, GLint param)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TexGenxOES);
   struct marshal_cmd_TexGenxOES *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexGenxOES, cmd_size);
   cmd->coord = MIN2(coord, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->pname = MIN2(pname, 0xffff); /* clamped to 0xffff (invalid enum) */
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
uint32_t
_mesa_unmarshal_TexGenxvOES(struct gl_context *ctx, const struct marshal_cmd_TexGenxvOES *cmd)
{
   GLenum coord = cmd->coord;
   GLenum pname = cmd->pname;
   GLfixed *params;
   const char *variable_data = (const char *) (cmd + 1);
   params = (GLfixed *) variable_data;
   CALL_TexGenxvOES(ctx->CurrentServerDispatch, (coord, pname, params));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
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
uint32_t
_mesa_unmarshal_ClipPlanef(struct gl_context *ctx, const struct marshal_cmd_ClipPlanef *cmd)
{
   GLenum plane = cmd->plane;
   const GLfloat *equation = cmd->equation;
   CALL_ClipPlanef(ctx->CurrentServerDispatch, (plane, equation));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_ClipPlanef), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ClipPlanef(GLenum plane, const GLfloat * equation)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ClipPlanef);
   struct marshal_cmd_ClipPlanef *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ClipPlanef, cmd_size);
   cmd->plane = MIN2(plane, 0xffff); /* clamped to 0xffff (invalid enum) */
   memcpy(cmd->equation, equation, 4 * sizeof(GLfloat));
}


/* GetClipPlanef: marshalled synchronously */
static void GLAPIENTRY
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
uint32_t
_mesa_unmarshal_Frustumf(struct gl_context *ctx, const struct marshal_cmd_Frustumf *cmd)
{
   GLfloat left = cmd->left;
   GLfloat right = cmd->right;
   GLfloat bottom = cmd->bottom;
   GLfloat top = cmd->top;
   GLfloat zNear = cmd->zNear;
   GLfloat zFar = cmd->zFar;
   CALL_Frustumf(ctx->CurrentServerDispatch, (left, right, bottom, top, zNear, zFar));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Frustumf), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
uint32_t
_mesa_unmarshal_Orthof(struct gl_context *ctx, const struct marshal_cmd_Orthof *cmd)
{
   GLfloat left = cmd->left;
   GLfloat right = cmd->right;
   GLfloat bottom = cmd->bottom;
   GLfloat top = cmd->top;
   GLfloat zNear = cmd->zNear;
   GLfloat zFar = cmd->zFar;
   CALL_Orthof(ctx->CurrentServerDispatch, (left, right, bottom, top, zNear, zFar));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Orthof), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
uint32_t
_mesa_unmarshal_DrawTexiOES(struct gl_context *ctx, const struct marshal_cmd_DrawTexiOES *cmd)
{
   GLint x = cmd->x;
   GLint y = cmd->y;
   GLint z = cmd->z;
   GLint width = cmd->width;
   GLint height = cmd->height;
   CALL_DrawTexiOES(ctx->CurrentServerDispatch, (x, y, z, width, height));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_DrawTexiOES), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
uint32_t
_mesa_unmarshal_DrawTexivOES(struct gl_context *ctx, const struct marshal_cmd_DrawTexivOES *cmd)
{
   const GLint *coords = cmd->coords;
   CALL_DrawTexivOES(ctx->CurrentServerDispatch, (coords));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_DrawTexivOES), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
uint32_t
_mesa_unmarshal_DrawTexfOES(struct gl_context *ctx, const struct marshal_cmd_DrawTexfOES *cmd)
{
   GLfloat x = cmd->x;
   GLfloat y = cmd->y;
   GLfloat z = cmd->z;
   GLfloat width = cmd->width;
   GLfloat height = cmd->height;
   CALL_DrawTexfOES(ctx->CurrentServerDispatch, (x, y, z, width, height));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_DrawTexfOES), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
uint32_t
_mesa_unmarshal_DrawTexfvOES(struct gl_context *ctx, const struct marshal_cmd_DrawTexfvOES *cmd)
{
   const GLfloat *coords = cmd->coords;
   CALL_DrawTexfvOES(ctx->CurrentServerDispatch, (coords));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_DrawTexfvOES), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
uint32_t
_mesa_unmarshal_DrawTexsOES(struct gl_context *ctx, const struct marshal_cmd_DrawTexsOES *cmd)
{
   GLshort x = cmd->x;
   GLshort y = cmd->y;
   GLshort z = cmd->z;
   GLshort width = cmd->width;
   GLshort height = cmd->height;
   CALL_DrawTexsOES(ctx->CurrentServerDispatch, (x, y, z, width, height));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_DrawTexsOES), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
uint32_t
_mesa_unmarshal_DrawTexsvOES(struct gl_context *ctx, const struct marshal_cmd_DrawTexsvOES *cmd)
{
   const GLshort *coords = cmd->coords;
   CALL_DrawTexsvOES(ctx->CurrentServerDispatch, (coords));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_DrawTexsvOES), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
uint32_t
_mesa_unmarshal_DrawTexxOES(struct gl_context *ctx, const struct marshal_cmd_DrawTexxOES *cmd)
{
   GLfixed x = cmd->x;
   GLfixed y = cmd->y;
   GLfixed z = cmd->z;
   GLfixed width = cmd->width;
   GLfixed height = cmd->height;
   CALL_DrawTexxOES(ctx->CurrentServerDispatch, (x, y, z, width, height));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_DrawTexxOES), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
uint32_t
_mesa_unmarshal_DrawTexxvOES(struct gl_context *ctx, const struct marshal_cmd_DrawTexxvOES *cmd)
{
   const GLfixed *coords = cmd->coords;
   CALL_DrawTexxvOES(ctx->CurrentServerDispatch, (coords));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_DrawTexxvOES), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
uint32_t
_mesa_unmarshal_PointSizePointerOES(struct gl_context *ctx, const struct marshal_cmd_PointSizePointerOES *cmd)
{
   GLenum type = cmd->type;
   GLsizei stride = cmd->stride;
   const GLvoid * pointer = cmd->pointer;
   CALL_PointSizePointerOES(ctx->CurrentServerDispatch, (type, stride, pointer));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_PointSizePointerOES), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_PointSizePointerOES(GLenum type, GLsizei stride, const GLvoid * pointer)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_PointSizePointerOES);
   struct marshal_cmd_PointSizePointerOES *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_PointSizePointerOES, cmd_size);
   cmd->type = MIN2(type, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->stride = stride;
   cmd->pointer = pointer;
   if (COMPAT) _mesa_glthread_AttribPointer(ctx, VERT_ATTRIB_POINT_SIZE, 1, type, stride, pointer);
}


/* QueryMatrixxOES: marshalled synchronously */
static GLbitfield GLAPIENTRY
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
uint32_t
_mesa_unmarshal_DiscardFramebufferEXT(struct gl_context *ctx, const struct marshal_cmd_DiscardFramebufferEXT *cmd)
{
   GLenum target = cmd->target;
   GLsizei numAttachments = cmd->numAttachments;
   GLenum *attachments;
   const char *variable_data = (const char *) (cmd + 1);
   attachments = (GLenum *) variable_data;
   CALL_DiscardFramebufferEXT(ctx->CurrentServerDispatch, (target, numAttachments, attachments));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
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
uint32_t
_mesa_unmarshal_FramebufferTexture2DMultisampleEXT(struct gl_context *ctx, const struct marshal_cmd_FramebufferTexture2DMultisampleEXT *cmd)
{
   GLenum target = cmd->target;
   GLenum attachment = cmd->attachment;
   GLenum textarget = cmd->textarget;
   GLuint texture = cmd->texture;
   GLint level = cmd->level;
   GLsizei samples = cmd->samples;
   CALL_FramebufferTexture2DMultisampleEXT(ctx->CurrentServerDispatch, (target, attachment, textarget, texture, level, samples));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_FramebufferTexture2DMultisampleEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_FramebufferTexture2DMultisampleEXT(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLsizei samples)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_FramebufferTexture2DMultisampleEXT);
   struct marshal_cmd_FramebufferTexture2DMultisampleEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_FramebufferTexture2DMultisampleEXT, cmd_size);
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
   GLuint first;
   GLsizei count;
   /* Next safe_mul((2 * count), 1 * sizeof(GLfloat)) bytes are GLfloat v[(2 * count)] */
};
uint32_t
_mesa_unmarshal_DepthRangeArrayfvOES(struct gl_context *ctx, const struct marshal_cmd_DepthRangeArrayfvOES *cmd)
{
   GLuint first = cmd->first;
   GLsizei count = cmd->count;
   GLfloat *v;
   const char *variable_data = (const char *) (cmd + 1);
   v = (GLfloat *) variable_data;
   CALL_DepthRangeArrayfvOES(ctx->CurrentServerDispatch, (first, count, v));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
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
uint32_t
_mesa_unmarshal_DepthRangeIndexedfOES(struct gl_context *ctx, const struct marshal_cmd_DepthRangeIndexedfOES *cmd)
{
   GLuint index = cmd->index;
   GLfloat n = cmd->n;
   GLfloat f = cmd->f;
   CALL_DepthRangeIndexedfOES(ctx->CurrentServerDispatch, (index, n, f));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_DepthRangeIndexedfOES), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
uint32_t
_mesa_unmarshal_FramebufferParameteriMESA(struct gl_context *ctx, const struct marshal_cmd_FramebufferParameteriMESA *cmd)
{
   GLenum target = cmd->target;
   GLenum pname = cmd->pname;
   GLint param = cmd->param;
   CALL_FramebufferParameteriMESA(ctx->CurrentServerDispatch, (target, pname, param));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_FramebufferParameteriMESA), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_FramebufferParameteriMESA(GLenum target, GLenum pname, GLint param)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_FramebufferParameteriMESA);
   struct marshal_cmd_FramebufferParameteriMESA *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_FramebufferParameteriMESA, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->pname = MIN2(pname, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->param = param;
}


/* GetFramebufferParameterivMESA: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetFramebufferParameterivMESA(GLenum target, GLenum pname, GLint * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetFramebufferParameterivMESA");
   CALL_GetFramebufferParameterivMESA(ctx->CurrentServerDispatch, (target, pname, params));
}


void
_mesa_glthread_init_dispatch7(struct gl_context *ctx, struct _glapi_table *table)
{
   if ((ctx->API == API_OPENGLES2 && ctx->Version >= 31)) {
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
      SET_VertexAttribI4sv(table, _mesa_marshal_VertexAttribI4sv);
      SET_VertexAttribI4ubv(table, _mesa_marshal_VertexAttribI4ubv);
      SET_VertexAttribI4usv(table, _mesa_marshal_VertexAttribI4usv);
   }
   if (_mesa_is_desktop_gl(ctx) || (ctx->API == API_OPENGLES2 && ctx->Version >= 30)) {
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
      SET_GetVertexAttribIiv(table, _mesa_marshal_GetVertexAttribIiv);
      SET_GetVertexAttribIuiv(table, _mesa_marshal_GetVertexAttribIuiv);
      SET_IsTransformFeedback(table, _mesa_marshal_IsTransformFeedback);
      SET_PauseTransformFeedback(table, _mesa_marshal_PauseTransformFeedback);
      SET_ResumeTransformFeedback(table, _mesa_marshal_ResumeTransformFeedback);
      SET_TransformFeedbackVaryings(table, _mesa_marshal_TransformFeedbackVaryings);
      SET_Uniform1ui(table, _mesa_marshal_Uniform1ui);
      SET_Uniform1uiv(table, _mesa_marshal_Uniform1uiv);
      SET_Uniform2ui(table, _mesa_marshal_Uniform2ui);
      SET_Uniform2uiv(table, _mesa_marshal_Uniform2uiv);
      SET_Uniform3ui(table, _mesa_marshal_Uniform3ui);
      SET_Uniform3uiv(table, _mesa_marshal_Uniform3uiv);
      SET_Uniform4ui(table, _mesa_marshal_Uniform4ui);
      SET_Uniform4uiv(table, _mesa_marshal_Uniform4uiv);
      SET_VertexAttribIPointer(table, _mesa_marshal_VertexAttribIPointer);
   }
   if (_mesa_is_desktop_gl(ctx) || (ctx->API == API_OPENGLES2 && ctx->Version >= 31)) {
      SET_MemoryBarrierByRegion(table, _mesa_marshal_MemoryBarrierByRegion);
      SET_ViewportSwizzleNV(table, _mesa_marshal_ViewportSwizzleNV);
   }
   if (_mesa_is_desktop_gl(ctx) || (ctx->API == API_OPENGLES2 && ctx->Version >= 32)) {
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
   if (_mesa_is_desktop_gl(ctx) || ctx->API == API_OPENGLES2) {
      SET_BeginConditionalRender(table, _mesa_marshal_BeginConditionalRender);
      SET_EndConditionalRender(table, _mesa_marshal_EndConditionalRender);
   }
   if (ctx->API == API_OPENGLES) {
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
   if (ctx->API == API_OPENGLES || ctx->API == API_OPENGLES2) {
      SET_DiscardFramebufferEXT(table, _mesa_marshal_DiscardFramebufferEXT);
   }
   if (ctx->API == API_OPENGLES2) {
      SET_FramebufferTexture2DMultisampleEXT(table, _mesa_marshal_FramebufferTexture2DMultisampleEXT);
   }
   if (ctx->API == API_OPENGL_COMPAT) {
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
   if (ctx->API == API_OPENGL_COMPAT || ctx->API == API_OPENGLES) {
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
   if (ctx->API == API_OPENGL_COMPAT || ctx->API == API_OPENGL_CORE || (ctx->API == API_OPENGLES2 && ctx->Version >= 31)) {
      SET_FramebufferTexture(table, _mesa_marshal_FramebufferTexture);
   }
   if (ctx->API == API_OPENGL_CORE || (ctx->API == API_OPENGLES2 && ctx->Version >= 30)) {
      SET_FramebufferParameteriMESA(table, _mesa_marshal_FramebufferParameteriMESA);
      SET_GetFramebufferParameterivMESA(table, _mesa_marshal_GetFramebufferParameterivMESA);
   }
}
