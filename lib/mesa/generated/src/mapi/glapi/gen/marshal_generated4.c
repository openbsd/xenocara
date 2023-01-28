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

/* VertexAttribL4d: marshalled asynchronously */
struct marshal_cmd_VertexAttribL4d
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLdouble x;
   GLdouble y;
   GLdouble z;
   GLdouble w;
};
uint32_t
_mesa_unmarshal_VertexAttribL4d(struct gl_context *ctx, const struct marshal_cmd_VertexAttribL4d *cmd)
{
   GLuint index = cmd->index;
   GLdouble x = cmd->x;
   GLdouble y = cmd->y;
   GLdouble z = cmd->z;
   GLdouble w = cmd->w;
   CALL_VertexAttribL4d(ctx->CurrentServerDispatch, (index, x, y, z, w));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexAttribL4d), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_VertexAttribL4d(GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttribL4d);
   struct marshal_cmd_VertexAttribL4d *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttribL4d, cmd_size);
   cmd->index = index;
   cmd->x = x;
   cmd->y = y;
   cmd->z = z;
   cmd->w = w;
}


/* VertexAttribL1dv: marshalled asynchronously */
struct marshal_cmd_VertexAttribL1dv
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLdouble v[1];
};
uint32_t
_mesa_unmarshal_VertexAttribL1dv(struct gl_context *ctx, const struct marshal_cmd_VertexAttribL1dv *cmd)
{
   GLuint index = cmd->index;
   const GLdouble *v = cmd->v;
   CALL_VertexAttribL1dv(ctx->CurrentServerDispatch, (index, v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexAttribL1dv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_VertexAttribL1dv(GLuint index, const GLdouble * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttribL1dv);
   struct marshal_cmd_VertexAttribL1dv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttribL1dv, cmd_size);
   cmd->index = index;
   memcpy(cmd->v, v, 1 * sizeof(GLdouble));
}


/* VertexAttribL2dv: marshalled asynchronously */
struct marshal_cmd_VertexAttribL2dv
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLdouble v[2];
};
uint32_t
_mesa_unmarshal_VertexAttribL2dv(struct gl_context *ctx, const struct marshal_cmd_VertexAttribL2dv *cmd)
{
   GLuint index = cmd->index;
   const GLdouble *v = cmd->v;
   CALL_VertexAttribL2dv(ctx->CurrentServerDispatch, (index, v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexAttribL2dv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_VertexAttribL2dv(GLuint index, const GLdouble * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttribL2dv);
   struct marshal_cmd_VertexAttribL2dv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttribL2dv, cmd_size);
   cmd->index = index;
   memcpy(cmd->v, v, 2 * sizeof(GLdouble));
}


/* VertexAttribL3dv: marshalled asynchronously */
struct marshal_cmd_VertexAttribL3dv
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLdouble v[3];
};
uint32_t
_mesa_unmarshal_VertexAttribL3dv(struct gl_context *ctx, const struct marshal_cmd_VertexAttribL3dv *cmd)
{
   GLuint index = cmd->index;
   const GLdouble *v = cmd->v;
   CALL_VertexAttribL3dv(ctx->CurrentServerDispatch, (index, v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexAttribL3dv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_VertexAttribL3dv(GLuint index, const GLdouble * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttribL3dv);
   struct marshal_cmd_VertexAttribL3dv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttribL3dv, cmd_size);
   cmd->index = index;
   memcpy(cmd->v, v, 3 * sizeof(GLdouble));
}


/* VertexAttribL4dv: marshalled asynchronously */
struct marshal_cmd_VertexAttribL4dv
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLdouble v[4];
};
uint32_t
_mesa_unmarshal_VertexAttribL4dv(struct gl_context *ctx, const struct marshal_cmd_VertexAttribL4dv *cmd)
{
   GLuint index = cmd->index;
   const GLdouble *v = cmd->v;
   CALL_VertexAttribL4dv(ctx->CurrentServerDispatch, (index, v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexAttribL4dv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_VertexAttribL4dv(GLuint index, const GLdouble * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttribL4dv);
   struct marshal_cmd_VertexAttribL4dv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttribL4dv, cmd_size);
   cmd->index = index;
   memcpy(cmd->v, v, 4 * sizeof(GLdouble));
}


/* VertexAttribLPointer: marshalled asynchronously */
struct marshal_cmd_VertexAttribLPointer
{
   struct marshal_cmd_base cmd_base;
   GLenum16 type;
   GLuint index;
   GLint size;
   GLsizei stride;
   const GLvoid * pointer;
};
uint32_t
_mesa_unmarshal_VertexAttribLPointer(struct gl_context *ctx, const struct marshal_cmd_VertexAttribLPointer *cmd)
{
   GLuint index = cmd->index;
   GLint size = cmd->size;
   GLenum type = cmd->type;
   GLsizei stride = cmd->stride;
   const GLvoid * pointer = cmd->pointer;
   CALL_VertexAttribLPointer(ctx->CurrentServerDispatch, (index, size, type, stride, pointer));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexAttribLPointer), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_VertexAttribLPointer(GLuint index, GLint size, GLenum type, GLsizei stride, const GLvoid * pointer)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttribLPointer);
   struct marshal_cmd_VertexAttribLPointer *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttribLPointer, cmd_size);
   cmd->index = index;
   cmd->size = size;
   cmd->type = MIN2(type, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->stride = stride;
   cmd->pointer = pointer;
   if (COMPAT) _mesa_glthread_AttribPointer(ctx, VERT_ATTRIB_GENERIC(index), size, type, stride, pointer);
}


/* GetVertexAttribLdv: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetVertexAttribLdv(GLuint index, GLenum pname, GLdouble * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetVertexAttribLdv");
   CALL_GetVertexAttribLdv(ctx->CurrentServerDispatch, (index, pname, params));
}


/* VertexArrayVertexAttribLOffsetEXT: marshalled asynchronously */
struct marshal_cmd_VertexArrayVertexAttribLOffsetEXT
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
uint32_t
_mesa_unmarshal_VertexArrayVertexAttribLOffsetEXT(struct gl_context *ctx, const struct marshal_cmd_VertexArrayVertexAttribLOffsetEXT *cmd)
{
   GLuint vaobj = cmd->vaobj;
   GLuint buffer = cmd->buffer;
   GLuint index = cmd->index;
   GLint size = cmd->size;
   GLenum type = cmd->type;
   GLsizei stride = cmd->stride;
   GLintptr offset = cmd->offset;
   CALL_VertexArrayVertexAttribLOffsetEXT(ctx->CurrentServerDispatch, (vaobj, buffer, index, size, type, stride, offset));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexArrayVertexAttribLOffsetEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_VertexArrayVertexAttribLOffsetEXT(GLuint vaobj, GLuint buffer, GLuint index, GLint size, GLenum type, GLsizei stride, GLintptr offset)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexArrayVertexAttribLOffsetEXT);
   struct marshal_cmd_VertexArrayVertexAttribLOffsetEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexArrayVertexAttribLOffsetEXT, cmd_size);
   cmd->vaobj = vaobj;
   cmd->buffer = buffer;
   cmd->index = index;
   cmd->size = size;
   cmd->type = MIN2(type, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->stride = stride;
   cmd->offset = offset;
   if (COMPAT) _mesa_glthread_DSAAttribPointer(ctx, vaobj, buffer, VERT_ATTRIB_GENERIC(index), size, type, stride, offset);
}


/* GetShaderPrecisionFormat: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetShaderPrecisionFormat(GLenum shadertype, GLenum precisiontype, GLint * range, GLint * precision)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetShaderPrecisionFormat");
   CALL_GetShaderPrecisionFormat(ctx->CurrentServerDispatch, (shadertype, precisiontype, range, precision));
}


/* ReleaseShaderCompiler: marshalled asynchronously */
struct marshal_cmd_ReleaseShaderCompiler
{
   struct marshal_cmd_base cmd_base;
};
uint32_t
_mesa_unmarshal_ReleaseShaderCompiler(struct gl_context *ctx, const struct marshal_cmd_ReleaseShaderCompiler *cmd)
{
   CALL_ReleaseShaderCompiler(ctx->CurrentServerDispatch, ());
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_ReleaseShaderCompiler), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ReleaseShaderCompiler(void)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ReleaseShaderCompiler);
   struct marshal_cmd_ReleaseShaderCompiler *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ReleaseShaderCompiler, cmd_size);
   (void) cmd;
}


/* ShaderBinary: marshalled asynchronously */
struct marshal_cmd_ShaderBinary
{
   struct marshal_cmd_base cmd_base;
   GLenum16 binaryformat;
   GLsizei n;
   GLsizei length;
   /* Next safe_mul(n, 1 * sizeof(GLuint)) bytes are GLuint shaders[n] */
   /* Next length bytes are GLvoid binary[length] */
};
uint32_t
_mesa_unmarshal_ShaderBinary(struct gl_context *ctx, const struct marshal_cmd_ShaderBinary *cmd)
{
   GLsizei n = cmd->n;
   GLenum binaryformat = cmd->binaryformat;
   GLsizei length = cmd->length;
   GLuint *shaders;
   GLvoid *binary;
   const char *variable_data = (const char *) (cmd + 1);
   shaders = (GLuint *) variable_data;
   variable_data += n * 1 * sizeof(GLuint);
   binary = (GLvoid *) variable_data;
   CALL_ShaderBinary(ctx->CurrentServerDispatch, (n, shaders, binaryformat, binary, length));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ShaderBinary(GLsizei n, const GLuint * shaders, GLenum binaryformat, const GLvoid * binary, GLsizei length)
{
   GET_CURRENT_CONTEXT(ctx);
   int shaders_size = safe_mul(n, 1 * sizeof(GLuint));
   int binary_size = length;
   int cmd_size = sizeof(struct marshal_cmd_ShaderBinary) + shaders_size + binary_size;
   struct marshal_cmd_ShaderBinary *cmd;
   if (unlikely(shaders_size < 0 || (shaders_size > 0 && !shaders) || binary_size < 0 || (binary_size > 0 && !binary) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "ShaderBinary");
      CALL_ShaderBinary(ctx->CurrentServerDispatch, (n, shaders, binaryformat, binary, length));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ShaderBinary, cmd_size);
   cmd->n = n;
   cmd->binaryformat = MIN2(binaryformat, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->length = length;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, shaders, shaders_size);
   variable_data += shaders_size;
   memcpy(variable_data, binary, binary_size);
}


/* ClearDepthf: marshalled asynchronously */
struct marshal_cmd_ClearDepthf
{
   struct marshal_cmd_base cmd_base;
   GLclampf depth;
};
uint32_t
_mesa_unmarshal_ClearDepthf(struct gl_context *ctx, const struct marshal_cmd_ClearDepthf *cmd)
{
   GLclampf depth = cmd->depth;
   CALL_ClearDepthf(ctx->CurrentServerDispatch, (depth));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_ClearDepthf), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ClearDepthf(GLclampf depth)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ClearDepthf);
   struct marshal_cmd_ClearDepthf *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ClearDepthf, cmd_size);
   cmd->depth = depth;
}


/* DepthRangef: marshalled asynchronously */
struct marshal_cmd_DepthRangef
{
   struct marshal_cmd_base cmd_base;
   GLclampf zNear;
   GLclampf zFar;
};
uint32_t
_mesa_unmarshal_DepthRangef(struct gl_context *ctx, const struct marshal_cmd_DepthRangef *cmd)
{
   GLclampf zNear = cmd->zNear;
   GLclampf zFar = cmd->zFar;
   CALL_DepthRangef(ctx->CurrentServerDispatch, (zNear, zFar));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_DepthRangef), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_DepthRangef(GLclampf zNear, GLclampf zFar)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_DepthRangef);
   struct marshal_cmd_DepthRangef *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_DepthRangef, cmd_size);
   cmd->zNear = zNear;
   cmd->zFar = zFar;
}


/* GetProgramBinary: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetProgramBinary(GLuint program, GLsizei bufSize, GLsizei * length, GLenum * binaryFormat, GLvoid * binary)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetProgramBinary");
   CALL_GetProgramBinary(ctx->CurrentServerDispatch, (program, bufSize, length, binaryFormat, binary));
}


/* ProgramBinary: marshalled asynchronously */
struct marshal_cmd_ProgramBinary
{
   struct marshal_cmd_base cmd_base;
   GLenum16 binaryFormat;
   GLuint program;
   GLsizei length;
   /* Next length bytes are GLvoid binary[length] */
};
uint32_t
_mesa_unmarshal_ProgramBinary(struct gl_context *ctx, const struct marshal_cmd_ProgramBinary *cmd)
{
   GLuint program = cmd->program;
   GLenum binaryFormat = cmd->binaryFormat;
   GLsizei length = cmd->length;
   GLvoid *binary;
   const char *variable_data = (const char *) (cmd + 1);
   binary = (GLvoid *) variable_data;
   CALL_ProgramBinary(ctx->CurrentServerDispatch, (program, binaryFormat, binary, length));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ProgramBinary(GLuint program, GLenum binaryFormat, const GLvoid * binary, GLsizei length)
{
   GET_CURRENT_CONTEXT(ctx);
   int binary_size = length;
   int cmd_size = sizeof(struct marshal_cmd_ProgramBinary) + binary_size;
   struct marshal_cmd_ProgramBinary *cmd;
   if (unlikely(binary_size < 0 || (binary_size > 0 && !binary) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "ProgramBinary");
      CALL_ProgramBinary(ctx->CurrentServerDispatch, (program, binaryFormat, binary, length));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ProgramBinary, cmd_size);
   cmd->program = program;
   cmd->binaryFormat = MIN2(binaryFormat, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->length = length;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, binary, binary_size);
}


/* ProgramParameteri: marshalled asynchronously */
struct marshal_cmd_ProgramParameteri
{
   struct marshal_cmd_base cmd_base;
   GLenum16 pname;
   GLuint program;
   GLint value;
};
uint32_t
_mesa_unmarshal_ProgramParameteri(struct gl_context *ctx, const struct marshal_cmd_ProgramParameteri *cmd)
{
   GLuint program = cmd->program;
   GLenum pname = cmd->pname;
   GLint value = cmd->value;
   CALL_ProgramParameteri(ctx->CurrentServerDispatch, (program, pname, value));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_ProgramParameteri), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ProgramParameteri(GLuint program, GLenum pname, GLint value)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ProgramParameteri);
   struct marshal_cmd_ProgramParameteri *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ProgramParameteri, cmd_size);
   cmd->program = program;
   cmd->pname = MIN2(pname, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->value = value;
}


/* DebugMessageControl: marshalled asynchronously */
struct marshal_cmd_DebugMessageControl
{
   struct marshal_cmd_base cmd_base;
   GLboolean enabled;
   GLenum16 source;
   GLenum16 type;
   GLenum16 severity;
   GLsizei count;
   /* Next safe_mul(count, 1 * sizeof(GLuint)) bytes are GLuint ids[count] */
};
uint32_t
_mesa_unmarshal_DebugMessageControl(struct gl_context *ctx, const struct marshal_cmd_DebugMessageControl *cmd)
{
   GLenum source = cmd->source;
   GLenum type = cmd->type;
   GLenum severity = cmd->severity;
   GLsizei count = cmd->count;
   GLboolean enabled = cmd->enabled;
   GLuint *ids;
   const char *variable_data = (const char *) (cmd + 1);
   ids = (GLuint *) variable_data;
   CALL_DebugMessageControl(ctx->CurrentServerDispatch, (source, type, severity, count, ids, enabled));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_DebugMessageControl(GLenum source, GLenum type, GLenum severity, GLsizei count, const GLuint * ids, GLboolean enabled)
{
   GET_CURRENT_CONTEXT(ctx);
   int ids_size = safe_mul(count, 1 * sizeof(GLuint));
   int cmd_size = sizeof(struct marshal_cmd_DebugMessageControl) + ids_size;
   struct marshal_cmd_DebugMessageControl *cmd;
   if (unlikely(ids_size < 0 || (ids_size > 0 && !ids) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "DebugMessageControl");
      CALL_DebugMessageControl(ctx->CurrentServerDispatch, (source, type, severity, count, ids, enabled));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_DebugMessageControl, cmd_size);
   cmd->source = MIN2(source, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->type = MIN2(type, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->severity = MIN2(severity, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->count = count;
   cmd->enabled = enabled;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, ids, ids_size);
}


/* DebugMessageInsert: marshalled asynchronously */
struct marshal_cmd_DebugMessageInsert
{
   struct marshal_cmd_base cmd_base;
   GLenum16 source;
   GLenum16 type;
   GLenum16 severity;
   GLuint id;
   GLsizei length;
   /* Next length bytes are GLchar buf[length] */
};
uint32_t
_mesa_unmarshal_DebugMessageInsert(struct gl_context *ctx, const struct marshal_cmd_DebugMessageInsert *cmd)
{
   GLenum source = cmd->source;
   GLenum type = cmd->type;
   GLuint id = cmd->id;
   GLenum severity = cmd->severity;
   GLsizei length = cmd->length;
   GLchar *buf;
   const char *variable_data = (const char *) (cmd + 1);
   buf = (GLchar *) variable_data;
   CALL_DebugMessageInsert(ctx->CurrentServerDispatch, (source, type, id, severity, length, buf));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_DebugMessageInsert(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar * buf)
{
   GET_CURRENT_CONTEXT(ctx);
   int buf_size = length;
   int cmd_size = sizeof(struct marshal_cmd_DebugMessageInsert) + buf_size;
   struct marshal_cmd_DebugMessageInsert *cmd;
   if (unlikely(buf_size < 0 || (buf_size > 0 && !buf) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "DebugMessageInsert");
      CALL_DebugMessageInsert(ctx->CurrentServerDispatch, (source, type, id, severity, length, buf));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_DebugMessageInsert, cmd_size);
   cmd->source = MIN2(source, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->type = MIN2(type, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->id = id;
   cmd->severity = MIN2(severity, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->length = length;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, buf, buf_size);
}


/* DebugMessageCallback: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_DebugMessageCallback(GLDEBUGPROC callback, const GLvoid * userParam)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "DebugMessageCallback");
   CALL_DebugMessageCallback(ctx->CurrentServerDispatch, (callback, userParam));
}


/* GetDebugMessageLog: marshalled synchronously */
static GLuint GLAPIENTRY
_mesa_marshal_GetDebugMessageLog(GLuint count, GLsizei bufsize, GLenum * sources, GLenum * types, GLuint * ids, GLenum * severities, GLsizei * lengths, GLchar * messageLog)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetDebugMessageLog");
   return CALL_GetDebugMessageLog(ctx->CurrentServerDispatch, (count, bufsize, sources, types, ids, severities, lengths, messageLog));
}


/* GetGraphicsResetStatusARB: marshalled synchronously */
static GLenum GLAPIENTRY
_mesa_marshal_GetGraphicsResetStatusARB(void)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetGraphicsResetStatusARB");
   return CALL_GetGraphicsResetStatusARB(ctx->CurrentServerDispatch, ());
}


/* GetnMapdvARB: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetnMapdvARB(GLenum target, GLenum query, GLsizei bufSize, GLdouble * v)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetnMapdvARB");
   CALL_GetnMapdvARB(ctx->CurrentServerDispatch, (target, query, bufSize, v));
}


/* GetnMapfvARB: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetnMapfvARB(GLenum target, GLenum query, GLsizei bufSize, GLfloat * v)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetnMapfvARB");
   CALL_GetnMapfvARB(ctx->CurrentServerDispatch, (target, query, bufSize, v));
}


/* GetnMapivARB: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetnMapivARB(GLenum target, GLenum query, GLsizei bufSize, GLint * v)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetnMapivARB");
   CALL_GetnMapivARB(ctx->CurrentServerDispatch, (target, query, bufSize, v));
}


/* GetnPixelMapfvARB: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetnPixelMapfvARB(GLenum map, GLsizei bufSize, GLfloat * values)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetnPixelMapfvARB");
   CALL_GetnPixelMapfvARB(ctx->CurrentServerDispatch, (map, bufSize, values));
}


/* GetnPixelMapuivARB: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetnPixelMapuivARB(GLenum map, GLsizei bufSize, GLuint * values)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetnPixelMapuivARB");
   CALL_GetnPixelMapuivARB(ctx->CurrentServerDispatch, (map, bufSize, values));
}


/* GetnPixelMapusvARB: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetnPixelMapusvARB(GLenum map, GLsizei bufSize, GLushort * values)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetnPixelMapusvARB");
   CALL_GetnPixelMapusvARB(ctx->CurrentServerDispatch, (map, bufSize, values));
}


/* GetnPolygonStippleARB: marshalled asynchronously */
struct marshal_cmd_GetnPolygonStippleARB
{
   struct marshal_cmd_base cmd_base;
   GLsizei bufSize;
   GLubyte * pattern;
};
uint32_t
_mesa_unmarshal_GetnPolygonStippleARB(struct gl_context *ctx, const struct marshal_cmd_GetnPolygonStippleARB *cmd)
{
   GLsizei bufSize = cmd->bufSize;
   GLubyte * pattern = cmd->pattern;
   CALL_GetnPolygonStippleARB(ctx->CurrentServerDispatch, (bufSize, pattern));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_GetnPolygonStippleARB), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_GetnPolygonStippleARB(GLsizei bufSize, GLubyte * pattern)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_GetnPolygonStippleARB);
   struct marshal_cmd_GetnPolygonStippleARB *cmd;
   if (_mesa_glthread_has_no_pack_buffer(ctx)) {
      _mesa_glthread_finish_before(ctx, "GetnPolygonStippleARB");
      CALL_GetnPolygonStippleARB(ctx->CurrentServerDispatch, (bufSize, pattern));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_GetnPolygonStippleARB, cmd_size);
   cmd->bufSize = bufSize;
   cmd->pattern = pattern;
}


/* GetnTexImageARB: marshalled asynchronously */
struct marshal_cmd_GetnTexImageARB
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLenum16 format;
   GLenum16 type;
   GLint level;
   GLsizei bufSize;
   GLvoid * img;
};
uint32_t
_mesa_unmarshal_GetnTexImageARB(struct gl_context *ctx, const struct marshal_cmd_GetnTexImageARB *cmd)
{
   GLenum target = cmd->target;
   GLint level = cmd->level;
   GLenum format = cmd->format;
   GLenum type = cmd->type;
   GLsizei bufSize = cmd->bufSize;
   GLvoid * img = cmd->img;
   CALL_GetnTexImageARB(ctx->CurrentServerDispatch, (target, level, format, type, bufSize, img));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_GetnTexImageARB), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_GetnTexImageARB(GLenum target, GLint level, GLenum format, GLenum type, GLsizei bufSize, GLvoid * img)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_GetnTexImageARB);
   struct marshal_cmd_GetnTexImageARB *cmd;
   if (_mesa_glthread_has_no_pack_buffer(ctx)) {
      _mesa_glthread_finish_before(ctx, "GetnTexImageARB");
      CALL_GetnTexImageARB(ctx->CurrentServerDispatch, (target, level, format, type, bufSize, img));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_GetnTexImageARB, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->level = level;
   cmd->format = MIN2(format, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->type = MIN2(type, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->bufSize = bufSize;
   cmd->img = img;
}


/* ReadnPixelsARB: marshalled asynchronously */
struct marshal_cmd_ReadnPixelsARB
{
   struct marshal_cmd_base cmd_base;
   GLenum16 format;
   GLenum16 type;
   GLint x;
   GLint y;
   GLsizei width;
   GLsizei height;
   GLsizei bufSize;
   GLvoid * data;
};
uint32_t
_mesa_unmarshal_ReadnPixelsARB(struct gl_context *ctx, const struct marshal_cmd_ReadnPixelsARB *cmd)
{
   GLint x = cmd->x;
   GLint y = cmd->y;
   GLsizei width = cmd->width;
   GLsizei height = cmd->height;
   GLenum format = cmd->format;
   GLenum type = cmd->type;
   GLsizei bufSize = cmd->bufSize;
   GLvoid * data = cmd->data;
   CALL_ReadnPixelsARB(ctx->CurrentServerDispatch, (x, y, width, height, format, type, bufSize, data));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_ReadnPixelsARB), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ReadnPixelsARB(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLsizei bufSize, GLvoid * data)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ReadnPixelsARB);
   struct marshal_cmd_ReadnPixelsARB *cmd;
   if (_mesa_glthread_has_no_pack_buffer(ctx)) {
      _mesa_glthread_finish_before(ctx, "ReadnPixelsARB");
      CALL_ReadnPixelsARB(ctx->CurrentServerDispatch, (x, y, width, height, format, type, bufSize, data));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ReadnPixelsARB, cmd_size);
   cmd->x = x;
   cmd->y = y;
   cmd->width = width;
   cmd->height = height;
   cmd->format = MIN2(format, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->type = MIN2(type, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->bufSize = bufSize;
   cmd->data = data;
}


/* GetnCompressedTexImageARB: marshalled asynchronously */
struct marshal_cmd_GetnCompressedTexImageARB
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLint lod;
   GLsizei bufSize;
   GLvoid * img;
};
uint32_t
_mesa_unmarshal_GetnCompressedTexImageARB(struct gl_context *ctx, const struct marshal_cmd_GetnCompressedTexImageARB *cmd)
{
   GLenum target = cmd->target;
   GLint lod = cmd->lod;
   GLsizei bufSize = cmd->bufSize;
   GLvoid * img = cmd->img;
   CALL_GetnCompressedTexImageARB(ctx->CurrentServerDispatch, (target, lod, bufSize, img));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_GetnCompressedTexImageARB), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_GetnCompressedTexImageARB(GLenum target, GLint lod, GLsizei bufSize, GLvoid * img)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_GetnCompressedTexImageARB);
   struct marshal_cmd_GetnCompressedTexImageARB *cmd;
   if (_mesa_glthread_has_no_pack_buffer(ctx)) {
      _mesa_glthread_finish_before(ctx, "GetnCompressedTexImageARB");
      CALL_GetnCompressedTexImageARB(ctx->CurrentServerDispatch, (target, lod, bufSize, img));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_GetnCompressedTexImageARB, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->lod = lod;
   cmd->bufSize = bufSize;
   cmd->img = img;
}


/* GetnUniformfvARB: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetnUniformfvARB(GLuint program, GLint location, GLsizei bufSize, GLfloat * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetnUniformfvARB");
   CALL_GetnUniformfvARB(ctx->CurrentServerDispatch, (program, location, bufSize, params));
}


/* GetnUniformivARB: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetnUniformivARB(GLuint program, GLint location, GLsizei bufSize, GLint * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetnUniformivARB");
   CALL_GetnUniformivARB(ctx->CurrentServerDispatch, (program, location, bufSize, params));
}


/* GetnUniformuivARB: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetnUniformuivARB(GLuint program, GLint location, GLsizei bufSize, GLuint * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetnUniformuivARB");
   CALL_GetnUniformuivARB(ctx->CurrentServerDispatch, (program, location, bufSize, params));
}


/* GetnUniformdvARB: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetnUniformdvARB(GLuint program, GLint location, GLsizei bufSize, GLdouble * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetnUniformdvARB");
   CALL_GetnUniformdvARB(ctx->CurrentServerDispatch, (program, location, bufSize, params));
}


/* DrawTransformFeedbackInstanced: marshalled asynchronously */
struct marshal_cmd_DrawTransformFeedbackInstanced
{
   struct marshal_cmd_base cmd_base;
   GLenum16 mode;
   GLuint id;
   GLsizei primcount;
};
uint32_t
_mesa_unmarshal_DrawTransformFeedbackInstanced(struct gl_context *ctx, const struct marshal_cmd_DrawTransformFeedbackInstanced *cmd)
{
   GLenum mode = cmd->mode;
   GLuint id = cmd->id;
   GLsizei primcount = cmd->primcount;
   CALL_DrawTransformFeedbackInstanced(ctx->CurrentServerDispatch, (mode, id, primcount));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_DrawTransformFeedbackInstanced), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_DrawTransformFeedbackInstanced(GLenum mode, GLuint id, GLsizei primcount)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_DrawTransformFeedbackInstanced);
   struct marshal_cmd_DrawTransformFeedbackInstanced *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_DrawTransformFeedbackInstanced, cmd_size);
   cmd->mode = MIN2(mode, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->id = id;
   cmd->primcount = primcount;
}


/* DrawTransformFeedbackStreamInstanced: marshalled asynchronously */
struct marshal_cmd_DrawTransformFeedbackStreamInstanced
{
   struct marshal_cmd_base cmd_base;
   GLenum16 mode;
   GLuint id;
   GLuint stream;
   GLsizei primcount;
};
uint32_t
_mesa_unmarshal_DrawTransformFeedbackStreamInstanced(struct gl_context *ctx, const struct marshal_cmd_DrawTransformFeedbackStreamInstanced *cmd)
{
   GLenum mode = cmd->mode;
   GLuint id = cmd->id;
   GLuint stream = cmd->stream;
   GLsizei primcount = cmd->primcount;
   CALL_DrawTransformFeedbackStreamInstanced(ctx->CurrentServerDispatch, (mode, id, stream, primcount));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_DrawTransformFeedbackStreamInstanced), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_DrawTransformFeedbackStreamInstanced(GLenum mode, GLuint id, GLuint stream, GLsizei primcount)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_DrawTransformFeedbackStreamInstanced);
   struct marshal_cmd_DrawTransformFeedbackStreamInstanced *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_DrawTransformFeedbackStreamInstanced, cmd_size);
   cmd->mode = MIN2(mode, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->id = id;
   cmd->stream = stream;
   cmd->primcount = primcount;
}


/* GetInternalformativ: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetInternalformativ(GLenum target, GLenum internalformat, GLenum pname, GLsizei bufSize, GLint * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetInternalformativ");
   CALL_GetInternalformativ(ctx->CurrentServerDispatch, (target, internalformat, pname, bufSize, params));
}


/* GetActiveAtomicCounterBufferiv: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetActiveAtomicCounterBufferiv(GLuint program, GLuint bufferIndex, GLenum pname, GLint * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetActiveAtomicCounterBufferiv");
   CALL_GetActiveAtomicCounterBufferiv(ctx->CurrentServerDispatch, (program, bufferIndex, pname, params));
}


/* BindImageTexture: marshalled asynchronously */
struct marshal_cmd_BindImageTexture
{
   struct marshal_cmd_base cmd_base;
   GLboolean layered;
   GLenum16 access;
   GLenum16 format;
   GLuint unit;
   GLuint texture;
   GLint level;
   GLint layer;
};
uint32_t
_mesa_unmarshal_BindImageTexture(struct gl_context *ctx, const struct marshal_cmd_BindImageTexture *cmd)
{
   GLuint unit = cmd->unit;
   GLuint texture = cmd->texture;
   GLint level = cmd->level;
   GLboolean layered = cmd->layered;
   GLint layer = cmd->layer;
   GLenum access = cmd->access;
   GLenum format = cmd->format;
   CALL_BindImageTexture(ctx->CurrentServerDispatch, (unit, texture, level, layered, layer, access, format));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_BindImageTexture), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_BindImageTexture(GLuint unit, GLuint texture, GLint level, GLboolean layered, GLint layer, GLenum access, GLenum format)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_BindImageTexture);
   struct marshal_cmd_BindImageTexture *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_BindImageTexture, cmd_size);
   cmd->unit = unit;
   cmd->texture = texture;
   cmd->level = level;
   cmd->layered = layered;
   cmd->layer = layer;
   cmd->access = MIN2(access, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->format = MIN2(format, 0xffff); /* clamped to 0xffff (invalid enum) */
}


/* MemoryBarrier: marshalled asynchronously */
struct marshal_cmd_MemoryBarrier
{
   struct marshal_cmd_base cmd_base;
   GLbitfield barriers;
};
uint32_t
_mesa_unmarshal_MemoryBarrier(struct gl_context *ctx, const struct marshal_cmd_MemoryBarrier *cmd)
{
   GLbitfield barriers = cmd->barriers;
   CALL_MemoryBarrier(ctx->CurrentServerDispatch, (barriers));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MemoryBarrier), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_MemoryBarrier(GLbitfield barriers)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MemoryBarrier);
   struct marshal_cmd_MemoryBarrier *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MemoryBarrier, cmd_size);
   cmd->barriers = barriers;
}


/* TexStorage1D: marshalled asynchronously */
struct marshal_cmd_TexStorage1D
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLenum16 internalFormat;
   GLsizei levels;
   GLsizei width;
};
uint32_t
_mesa_unmarshal_TexStorage1D(struct gl_context *ctx, const struct marshal_cmd_TexStorage1D *cmd)
{
   GLenum target = cmd->target;
   GLsizei levels = cmd->levels;
   GLenum internalFormat = cmd->internalFormat;
   GLsizei width = cmd->width;
   CALL_TexStorage1D(ctx->CurrentServerDispatch, (target, levels, internalFormat, width));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_TexStorage1D), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_TexStorage1D(GLenum target, GLsizei levels, GLenum internalFormat, GLsizei width)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TexStorage1D);
   struct marshal_cmd_TexStorage1D *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexStorage1D, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->levels = levels;
   cmd->internalFormat = MIN2(internalFormat, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->width = width;
}


/* TexStorage2D: marshalled asynchronously */
struct marshal_cmd_TexStorage2D
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLenum16 internalFormat;
   GLsizei levels;
   GLsizei width;
   GLsizei height;
};
uint32_t
_mesa_unmarshal_TexStorage2D(struct gl_context *ctx, const struct marshal_cmd_TexStorage2D *cmd)
{
   GLenum target = cmd->target;
   GLsizei levels = cmd->levels;
   GLenum internalFormat = cmd->internalFormat;
   GLsizei width = cmd->width;
   GLsizei height = cmd->height;
   CALL_TexStorage2D(ctx->CurrentServerDispatch, (target, levels, internalFormat, width, height));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_TexStorage2D), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_TexStorage2D(GLenum target, GLsizei levels, GLenum internalFormat, GLsizei width, GLsizei height)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TexStorage2D);
   struct marshal_cmd_TexStorage2D *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexStorage2D, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->levels = levels;
   cmd->internalFormat = MIN2(internalFormat, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->width = width;
   cmd->height = height;
}


/* TexStorage3D: marshalled asynchronously */
struct marshal_cmd_TexStorage3D
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLenum16 internalFormat;
   GLsizei levels;
   GLsizei width;
   GLsizei height;
   GLsizei depth;
};
uint32_t
_mesa_unmarshal_TexStorage3D(struct gl_context *ctx, const struct marshal_cmd_TexStorage3D *cmd)
{
   GLenum target = cmd->target;
   GLsizei levels = cmd->levels;
   GLenum internalFormat = cmd->internalFormat;
   GLsizei width = cmd->width;
   GLsizei height = cmd->height;
   GLsizei depth = cmd->depth;
   CALL_TexStorage3D(ctx->CurrentServerDispatch, (target, levels, internalFormat, width, height, depth));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_TexStorage3D), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_TexStorage3D(GLenum target, GLsizei levels, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TexStorage3D);
   struct marshal_cmd_TexStorage3D *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexStorage3D, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->levels = levels;
   cmd->internalFormat = MIN2(internalFormat, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->width = width;
   cmd->height = height;
   cmd->depth = depth;
}


/* TextureStorage1DEXT: marshalled asynchronously */
struct marshal_cmd_TextureStorage1DEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLenum16 internalFormat;
   GLuint texture;
   GLsizei levels;
   GLsizei width;
};
uint32_t
_mesa_unmarshal_TextureStorage1DEXT(struct gl_context *ctx, const struct marshal_cmd_TextureStorage1DEXT *cmd)
{
   GLuint texture = cmd->texture;
   GLenum target = cmd->target;
   GLsizei levels = cmd->levels;
   GLenum internalFormat = cmd->internalFormat;
   GLsizei width = cmd->width;
   CALL_TextureStorage1DEXT(ctx->CurrentServerDispatch, (texture, target, levels, internalFormat, width));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_TextureStorage1DEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_TextureStorage1DEXT(GLuint texture, GLenum target, GLsizei levels, GLenum internalFormat, GLsizei width)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TextureStorage1DEXT);
   struct marshal_cmd_TextureStorage1DEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TextureStorage1DEXT, cmd_size);
   cmd->texture = texture;
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->levels = levels;
   cmd->internalFormat = MIN2(internalFormat, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->width = width;
}


/* TextureStorage2DEXT: marshalled asynchronously */
struct marshal_cmd_TextureStorage2DEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLenum16 internalFormat;
   GLuint texture;
   GLsizei levels;
   GLsizei width;
   GLsizei height;
};
uint32_t
_mesa_unmarshal_TextureStorage2DEXT(struct gl_context *ctx, const struct marshal_cmd_TextureStorage2DEXT *cmd)
{
   GLuint texture = cmd->texture;
   GLenum target = cmd->target;
   GLsizei levels = cmd->levels;
   GLenum internalFormat = cmd->internalFormat;
   GLsizei width = cmd->width;
   GLsizei height = cmd->height;
   CALL_TextureStorage2DEXT(ctx->CurrentServerDispatch, (texture, target, levels, internalFormat, width, height));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_TextureStorage2DEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_TextureStorage2DEXT(GLuint texture, GLenum target, GLsizei levels, GLenum internalFormat, GLsizei width, GLsizei height)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TextureStorage2DEXT);
   struct marshal_cmd_TextureStorage2DEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TextureStorage2DEXT, cmd_size);
   cmd->texture = texture;
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->levels = levels;
   cmd->internalFormat = MIN2(internalFormat, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->width = width;
   cmd->height = height;
}


/* TextureStorage3DEXT: marshalled asynchronously */
struct marshal_cmd_TextureStorage3DEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLenum16 internalFormat;
   GLuint texture;
   GLsizei levels;
   GLsizei width;
   GLsizei height;
   GLsizei depth;
};
uint32_t
_mesa_unmarshal_TextureStorage3DEXT(struct gl_context *ctx, const struct marshal_cmd_TextureStorage3DEXT *cmd)
{
   GLuint texture = cmd->texture;
   GLenum target = cmd->target;
   GLsizei levels = cmd->levels;
   GLenum internalFormat = cmd->internalFormat;
   GLsizei width = cmd->width;
   GLsizei height = cmd->height;
   GLsizei depth = cmd->depth;
   CALL_TextureStorage3DEXT(ctx->CurrentServerDispatch, (texture, target, levels, internalFormat, width, height, depth));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_TextureStorage3DEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_TextureStorage3DEXT(GLuint texture, GLenum target, GLsizei levels, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TextureStorage3DEXT);
   struct marshal_cmd_TextureStorage3DEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TextureStorage3DEXT, cmd_size);
   cmd->texture = texture;
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->levels = levels;
   cmd->internalFormat = MIN2(internalFormat, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->width = width;
   cmd->height = height;
   cmd->depth = depth;
}


/* PushDebugGroup: marshalled asynchronously */
struct marshal_cmd_PushDebugGroup
{
   struct marshal_cmd_base cmd_base;
   GLenum16 source;
   GLuint id;
   GLsizei length;
   /* Next length bytes are GLchar message[length] */
};
uint32_t
_mesa_unmarshal_PushDebugGroup(struct gl_context *ctx, const struct marshal_cmd_PushDebugGroup *cmd)
{
   GLenum source = cmd->source;
   GLuint id = cmd->id;
   GLsizei length = cmd->length;
   GLchar *message;
   const char *variable_data = (const char *) (cmd + 1);
   message = (GLchar *) variable_data;
   CALL_PushDebugGroup(ctx->CurrentServerDispatch, (source, id, length, message));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_PushDebugGroup(GLenum source, GLuint id, GLsizei length, const GLchar * message)
{
   GET_CURRENT_CONTEXT(ctx);
   int message_size = length;
   int cmd_size = sizeof(struct marshal_cmd_PushDebugGroup) + message_size;
   struct marshal_cmd_PushDebugGroup *cmd;
   if (unlikely(message_size < 0 || (message_size > 0 && !message) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "PushDebugGroup");
      CALL_PushDebugGroup(ctx->CurrentServerDispatch, (source, id, length, message));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_PushDebugGroup, cmd_size);
   cmd->source = MIN2(source, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->id = id;
   cmd->length = length;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, message, message_size);
}


/* PopDebugGroup: marshalled asynchronously */
struct marshal_cmd_PopDebugGroup
{
   struct marshal_cmd_base cmd_base;
};
uint32_t
_mesa_unmarshal_PopDebugGroup(struct gl_context *ctx, const struct marshal_cmd_PopDebugGroup *cmd)
{
   CALL_PopDebugGroup(ctx->CurrentServerDispatch, ());
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_PopDebugGroup), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_PopDebugGroup(void)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_PopDebugGroup);
   struct marshal_cmd_PopDebugGroup *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_PopDebugGroup, cmd_size);
   (void) cmd;
}


/* ObjectLabel: marshalled asynchronously */
struct marshal_cmd_ObjectLabel
{
   struct marshal_cmd_base cmd_base;
   GLenum16 identifier;
   GLuint name;
   GLsizei length;
   /* Next length bytes are GLchar label[length] */
};
uint32_t
_mesa_unmarshal_ObjectLabel(struct gl_context *ctx, const struct marshal_cmd_ObjectLabel *cmd)
{
   GLenum identifier = cmd->identifier;
   GLuint name = cmd->name;
   GLsizei length = cmd->length;
   GLchar *label;
   const char *variable_data = (const char *) (cmd + 1);
   label = (GLchar *) variable_data;
   CALL_ObjectLabel(ctx->CurrentServerDispatch, (identifier, name, length, label));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ObjectLabel(GLenum identifier, GLuint name, GLsizei length, const GLchar * label)
{
   GET_CURRENT_CONTEXT(ctx);
   int label_size = length;
   int cmd_size = sizeof(struct marshal_cmd_ObjectLabel) + label_size;
   struct marshal_cmd_ObjectLabel *cmd;
   if (unlikely(label_size < 0 || (label_size > 0 && !label) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "ObjectLabel");
      CALL_ObjectLabel(ctx->CurrentServerDispatch, (identifier, name, length, label));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ObjectLabel, cmd_size);
   cmd->identifier = MIN2(identifier, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->name = name;
   cmd->length = length;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, label, label_size);
}


/* GetObjectLabel: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetObjectLabel(GLenum identifier, GLuint name, GLsizei bufSize, GLsizei * length, GLchar * label)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetObjectLabel");
   CALL_GetObjectLabel(ctx->CurrentServerDispatch, (identifier, name, bufSize, length, label));
}


/* ObjectPtrLabel: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_ObjectPtrLabel(const GLvoid * ptr, GLsizei length, const GLchar * label)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "ObjectPtrLabel");
   CALL_ObjectPtrLabel(ctx->CurrentServerDispatch, (ptr, length, label));
}


/* GetObjectPtrLabel: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetObjectPtrLabel(const GLvoid * ptr, GLsizei bufSize, GLsizei * length, GLchar * label)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetObjectPtrLabel");
   CALL_GetObjectPtrLabel(ctx->CurrentServerDispatch, (ptr, bufSize, length, label));
}


/* ClearBufferData: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_ClearBufferData(GLenum target, GLenum internalformat, GLenum format, GLenum type, const GLvoid * data)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "ClearBufferData");
   CALL_ClearBufferData(ctx->CurrentServerDispatch, (target, internalformat, format, type, data));
}


/* ClearBufferSubData: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_ClearBufferSubData(GLenum target, GLenum internalformat, GLintptr offset, GLsizeiptr size, GLenum format, GLenum type, const GLvoid * data)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "ClearBufferSubData");
   CALL_ClearBufferSubData(ctx->CurrentServerDispatch, (target, internalformat, offset, size, format, type, data));
}


/* ClearNamedBufferDataEXT: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_ClearNamedBufferDataEXT(GLuint buffer, GLenum internalformat, GLenum format, GLenum type, const GLvoid * data)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "ClearNamedBufferDataEXT");
   CALL_ClearNamedBufferDataEXT(ctx->CurrentServerDispatch, (buffer, internalformat, format, type, data));
}


/* ClearNamedBufferSubDataEXT: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_ClearNamedBufferSubDataEXT(GLuint buffer, GLenum internalformat, GLintptr offset, GLsizeiptr size, GLenum format, GLenum type, const GLvoid * data)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "ClearNamedBufferSubDataEXT");
   CALL_ClearNamedBufferSubDataEXT(ctx->CurrentServerDispatch, (buffer, internalformat, offset, size, format, type, data));
}


/* DispatchCompute: marshalled asynchronously */
struct marshal_cmd_DispatchCompute
{
   struct marshal_cmd_base cmd_base;
   GLuint num_groups_x;
   GLuint num_groups_y;
   GLuint num_groups_z;
};
uint32_t
_mesa_unmarshal_DispatchCompute(struct gl_context *ctx, const struct marshal_cmd_DispatchCompute *cmd)
{
   GLuint num_groups_x = cmd->num_groups_x;
   GLuint num_groups_y = cmd->num_groups_y;
   GLuint num_groups_z = cmd->num_groups_z;
   CALL_DispatchCompute(ctx->CurrentServerDispatch, (num_groups_x, num_groups_y, num_groups_z));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_DispatchCompute), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_DispatchCompute(GLuint num_groups_x, GLuint num_groups_y, GLuint num_groups_z)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_DispatchCompute);
   struct marshal_cmd_DispatchCompute *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_DispatchCompute, cmd_size);
   cmd->num_groups_x = num_groups_x;
   cmd->num_groups_y = num_groups_y;
   cmd->num_groups_z = num_groups_z;
}


/* DispatchComputeIndirect: marshalled asynchronously */
struct marshal_cmd_DispatchComputeIndirect
{
   struct marshal_cmd_base cmd_base;
   GLintptr indirect;
};
uint32_t
_mesa_unmarshal_DispatchComputeIndirect(struct gl_context *ctx, const struct marshal_cmd_DispatchComputeIndirect *cmd)
{
   GLintptr indirect = cmd->indirect;
   CALL_DispatchComputeIndirect(ctx->CurrentServerDispatch, (indirect));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_DispatchComputeIndirect), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_DispatchComputeIndirect(GLintptr indirect)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_DispatchComputeIndirect);
   struct marshal_cmd_DispatchComputeIndirect *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_DispatchComputeIndirect, cmd_size);
   cmd->indirect = indirect;
}


/* CopyImageSubData: marshalled asynchronously */
struct marshal_cmd_CopyImageSubData
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
   GLsizei srcWidth;
   GLsizei srcHeight;
   GLsizei srcDepth;
};
uint32_t
_mesa_unmarshal_CopyImageSubData(struct gl_context *ctx, const struct marshal_cmd_CopyImageSubData *cmd)
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
   GLsizei srcWidth = cmd->srcWidth;
   GLsizei srcHeight = cmd->srcHeight;
   GLsizei srcDepth = cmd->srcDepth;
   CALL_CopyImageSubData(ctx->CurrentServerDispatch, (srcName, srcTarget, srcLevel, srcX, srcY, srcZ, dstName, dstTarget, dstLevel, dstX, dstY, dstZ, srcWidth, srcHeight, srcDepth));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_CopyImageSubData), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_CopyImageSubData(GLuint srcName, GLenum srcTarget, GLint srcLevel, GLint srcX, GLint srcY, GLint srcZ, GLuint dstName, GLenum dstTarget, GLint dstLevel, GLint dstX, GLint dstY, GLint dstZ, GLsizei srcWidth, GLsizei srcHeight, GLsizei srcDepth)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_CopyImageSubData);
   struct marshal_cmd_CopyImageSubData *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_CopyImageSubData, cmd_size);
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
   cmd->srcWidth = srcWidth;
   cmd->srcHeight = srcHeight;
   cmd->srcDepth = srcDepth;
}


/* TextureView: marshalled asynchronously */
struct marshal_cmd_TextureView
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLenum16 internalformat;
   GLuint texture;
   GLuint origtexture;
   GLuint minlevel;
   GLuint numlevels;
   GLuint minlayer;
   GLuint numlayers;
};
uint32_t
_mesa_unmarshal_TextureView(struct gl_context *ctx, const struct marshal_cmd_TextureView *cmd)
{
   GLuint texture = cmd->texture;
   GLenum target = cmd->target;
   GLuint origtexture = cmd->origtexture;
   GLenum internalformat = cmd->internalformat;
   GLuint minlevel = cmd->minlevel;
   GLuint numlevels = cmd->numlevels;
   GLuint minlayer = cmd->minlayer;
   GLuint numlayers = cmd->numlayers;
   CALL_TextureView(ctx->CurrentServerDispatch, (texture, target, origtexture, internalformat, minlevel, numlevels, minlayer, numlayers));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_TextureView), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_TextureView(GLuint texture, GLenum target, GLuint origtexture, GLenum internalformat, GLuint minlevel, GLuint numlevels, GLuint minlayer, GLuint numlayers)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TextureView);
   struct marshal_cmd_TextureView *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TextureView, cmd_size);
   cmd->texture = texture;
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->origtexture = origtexture;
   cmd->internalformat = MIN2(internalformat, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->minlevel = minlevel;
   cmd->numlevels = numlevels;
   cmd->minlayer = minlayer;
   cmd->numlayers = numlayers;
}


/* BindVertexBuffer: marshalled asynchronously */
struct marshal_cmd_BindVertexBuffer
{
   struct marshal_cmd_base cmd_base;
   GLuint bindingindex;
   GLuint buffer;
   GLsizei stride;
   GLintptr offset;
};
uint32_t
_mesa_unmarshal_BindVertexBuffer(struct gl_context *ctx, const struct marshal_cmd_BindVertexBuffer *cmd)
{
   GLuint bindingindex = cmd->bindingindex;
   GLuint buffer = cmd->buffer;
   GLintptr offset = cmd->offset;
   GLsizei stride = cmd->stride;
   CALL_BindVertexBuffer(ctx->CurrentServerDispatch, (bindingindex, buffer, offset, stride));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_BindVertexBuffer), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_BindVertexBuffer(GLuint bindingindex, GLuint buffer, GLintptr offset, GLsizei stride)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_BindVertexBuffer);
   struct marshal_cmd_BindVertexBuffer *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_BindVertexBuffer, cmd_size);
   cmd->bindingindex = bindingindex;
   cmd->buffer = buffer;
   cmd->offset = offset;
   cmd->stride = stride;
   if (COMPAT) _mesa_glthread_VertexBuffer(ctx, bindingindex, buffer, offset, stride);
}


/* VertexAttribFormat: marshalled asynchronously */
struct marshal_cmd_VertexAttribFormat
{
   struct marshal_cmd_base cmd_base;
   GLboolean normalized;
   GLenum16 type;
   GLuint attribindex;
   GLint size;
   GLuint relativeoffset;
};
uint32_t
_mesa_unmarshal_VertexAttribFormat(struct gl_context *ctx, const struct marshal_cmd_VertexAttribFormat *cmd)
{
   GLuint attribindex = cmd->attribindex;
   GLint size = cmd->size;
   GLenum type = cmd->type;
   GLboolean normalized = cmd->normalized;
   GLuint relativeoffset = cmd->relativeoffset;
   CALL_VertexAttribFormat(ctx->CurrentServerDispatch, (attribindex, size, type, normalized, relativeoffset));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexAttribFormat), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_VertexAttribFormat(GLuint attribindex, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttribFormat);
   struct marshal_cmd_VertexAttribFormat *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttribFormat, cmd_size);
   cmd->attribindex = attribindex;
   cmd->size = size;
   cmd->type = MIN2(type, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->normalized = normalized;
   cmd->relativeoffset = relativeoffset;
   if (COMPAT) _mesa_glthread_AttribFormat(ctx, attribindex, size, type, relativeoffset);
}


/* VertexAttribIFormat: marshalled asynchronously */
struct marshal_cmd_VertexAttribIFormat
{
   struct marshal_cmd_base cmd_base;
   GLenum16 type;
   GLuint attribindex;
   GLint size;
   GLuint relativeoffset;
};
uint32_t
_mesa_unmarshal_VertexAttribIFormat(struct gl_context *ctx, const struct marshal_cmd_VertexAttribIFormat *cmd)
{
   GLuint attribindex = cmd->attribindex;
   GLint size = cmd->size;
   GLenum type = cmd->type;
   GLuint relativeoffset = cmd->relativeoffset;
   CALL_VertexAttribIFormat(ctx->CurrentServerDispatch, (attribindex, size, type, relativeoffset));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexAttribIFormat), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_VertexAttribIFormat(GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttribIFormat);
   struct marshal_cmd_VertexAttribIFormat *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttribIFormat, cmd_size);
   cmd->attribindex = attribindex;
   cmd->size = size;
   cmd->type = MIN2(type, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->relativeoffset = relativeoffset;
   if (COMPAT) _mesa_glthread_AttribFormat(ctx, attribindex, size, type, relativeoffset);
}


/* VertexAttribLFormat: marshalled asynchronously */
struct marshal_cmd_VertexAttribLFormat
{
   struct marshal_cmd_base cmd_base;
   GLenum16 type;
   GLuint attribindex;
   GLint size;
   GLuint relativeoffset;
};
uint32_t
_mesa_unmarshal_VertexAttribLFormat(struct gl_context *ctx, const struct marshal_cmd_VertexAttribLFormat *cmd)
{
   GLuint attribindex = cmd->attribindex;
   GLint size = cmd->size;
   GLenum type = cmd->type;
   GLuint relativeoffset = cmd->relativeoffset;
   CALL_VertexAttribLFormat(ctx->CurrentServerDispatch, (attribindex, size, type, relativeoffset));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexAttribLFormat), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_VertexAttribLFormat(GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttribLFormat);
   struct marshal_cmd_VertexAttribLFormat *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttribLFormat, cmd_size);
   cmd->attribindex = attribindex;
   cmd->size = size;
   cmd->type = MIN2(type, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->relativeoffset = relativeoffset;
   if (COMPAT) _mesa_glthread_AttribFormat(ctx, attribindex, size, type, relativeoffset);
}


/* VertexAttribBinding: marshalled asynchronously */
struct marshal_cmd_VertexAttribBinding
{
   struct marshal_cmd_base cmd_base;
   GLuint attribindex;
   GLuint bindingindex;
};
uint32_t
_mesa_unmarshal_VertexAttribBinding(struct gl_context *ctx, const struct marshal_cmd_VertexAttribBinding *cmd)
{
   GLuint attribindex = cmd->attribindex;
   GLuint bindingindex = cmd->bindingindex;
   CALL_VertexAttribBinding(ctx->CurrentServerDispatch, (attribindex, bindingindex));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexAttribBinding), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_VertexAttribBinding(GLuint attribindex, GLuint bindingindex)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttribBinding);
   struct marshal_cmd_VertexAttribBinding *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttribBinding, cmd_size);
   cmd->attribindex = attribindex;
   cmd->bindingindex = bindingindex;
   if (COMPAT) _mesa_glthread_AttribBinding(ctx, attribindex, bindingindex);
}


/* VertexBindingDivisor: marshalled asynchronously */
struct marshal_cmd_VertexBindingDivisor
{
   struct marshal_cmd_base cmd_base;
   GLuint bindingindex;
   GLuint divisor;
};
uint32_t
_mesa_unmarshal_VertexBindingDivisor(struct gl_context *ctx, const struct marshal_cmd_VertexBindingDivisor *cmd)
{
   GLuint bindingindex = cmd->bindingindex;
   GLuint divisor = cmd->divisor;
   CALL_VertexBindingDivisor(ctx->CurrentServerDispatch, (bindingindex, divisor));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexBindingDivisor), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_VertexBindingDivisor(GLuint bindingindex, GLuint divisor)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexBindingDivisor);
   struct marshal_cmd_VertexBindingDivisor *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexBindingDivisor, cmd_size);
   cmd->bindingindex = bindingindex;
   cmd->divisor = divisor;
   if (COMPAT) _mesa_glthread_BindingDivisor(ctx, bindingindex, divisor);
}


/* VertexArrayBindVertexBufferEXT: marshalled asynchronously */
struct marshal_cmd_VertexArrayBindVertexBufferEXT
{
   struct marshal_cmd_base cmd_base;
   GLuint vaobj;
   GLuint bindingindex;
   GLuint buffer;
   GLsizei stride;
   GLintptr offset;
};
uint32_t
_mesa_unmarshal_VertexArrayBindVertexBufferEXT(struct gl_context *ctx, const struct marshal_cmd_VertexArrayBindVertexBufferEXT *cmd)
{
   GLuint vaobj = cmd->vaobj;
   GLuint bindingindex = cmd->bindingindex;
   GLuint buffer = cmd->buffer;
   GLintptr offset = cmd->offset;
   GLsizei stride = cmd->stride;
   CALL_VertexArrayBindVertexBufferEXT(ctx->CurrentServerDispatch, (vaobj, bindingindex, buffer, offset, stride));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexArrayBindVertexBufferEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_VertexArrayBindVertexBufferEXT(GLuint vaobj, GLuint bindingindex, GLuint buffer, GLintptr offset, GLsizei stride)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexArrayBindVertexBufferEXT);
   struct marshal_cmd_VertexArrayBindVertexBufferEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexArrayBindVertexBufferEXT, cmd_size);
   cmd->vaobj = vaobj;
   cmd->bindingindex = bindingindex;
   cmd->buffer = buffer;
   cmd->offset = offset;
   cmd->stride = stride;
   if (COMPAT) _mesa_glthread_DSAVertexBuffer(ctx, vaobj, bindingindex, buffer, offset, stride);
}


/* VertexArrayVertexAttribFormatEXT: marshalled asynchronously */
struct marshal_cmd_VertexArrayVertexAttribFormatEXT
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
_mesa_unmarshal_VertexArrayVertexAttribFormatEXT(struct gl_context *ctx, const struct marshal_cmd_VertexArrayVertexAttribFormatEXT *cmd)
{
   GLuint vaobj = cmd->vaobj;
   GLuint attribindex = cmd->attribindex;
   GLint size = cmd->size;
   GLenum type = cmd->type;
   GLboolean normalized = cmd->normalized;
   GLuint relativeoffset = cmd->relativeoffset;
   CALL_VertexArrayVertexAttribFormatEXT(ctx->CurrentServerDispatch, (vaobj, attribindex, size, type, normalized, relativeoffset));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexArrayVertexAttribFormatEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_VertexArrayVertexAttribFormatEXT(GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexArrayVertexAttribFormatEXT);
   struct marshal_cmd_VertexArrayVertexAttribFormatEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexArrayVertexAttribFormatEXT, cmd_size);
   cmd->vaobj = vaobj;
   cmd->attribindex = attribindex;
   cmd->size = size;
   cmd->type = MIN2(type, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->normalized = normalized;
   cmd->relativeoffset = relativeoffset;
   if (COMPAT) _mesa_glthread_DSAAttribFormat(ctx, vaobj, attribindex, size, type, relativeoffset);
}


/* VertexArrayVertexAttribIFormatEXT: marshalled asynchronously */
struct marshal_cmd_VertexArrayVertexAttribIFormatEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 type;
   GLuint vaobj;
   GLuint attribindex;
   GLint size;
   GLuint relativeoffset;
};
uint32_t
_mesa_unmarshal_VertexArrayVertexAttribIFormatEXT(struct gl_context *ctx, const struct marshal_cmd_VertexArrayVertexAttribIFormatEXT *cmd)
{
   GLuint vaobj = cmd->vaobj;
   GLuint attribindex = cmd->attribindex;
   GLint size = cmd->size;
   GLenum type = cmd->type;
   GLuint relativeoffset = cmd->relativeoffset;
   CALL_VertexArrayVertexAttribIFormatEXT(ctx->CurrentServerDispatch, (vaobj, attribindex, size, type, relativeoffset));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexArrayVertexAttribIFormatEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_VertexArrayVertexAttribIFormatEXT(GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexArrayVertexAttribIFormatEXT);
   struct marshal_cmd_VertexArrayVertexAttribIFormatEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexArrayVertexAttribIFormatEXT, cmd_size);
   cmd->vaobj = vaobj;
   cmd->attribindex = attribindex;
   cmd->size = size;
   cmd->type = MIN2(type, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->relativeoffset = relativeoffset;
   if (COMPAT) _mesa_glthread_DSAAttribFormat(ctx, vaobj, attribindex, size, type, relativeoffset);
}


/* VertexArrayVertexAttribLFormatEXT: marshalled asynchronously */
struct marshal_cmd_VertexArrayVertexAttribLFormatEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 type;
   GLuint vaobj;
   GLuint attribindex;
   GLint size;
   GLuint relativeoffset;
};
uint32_t
_mesa_unmarshal_VertexArrayVertexAttribLFormatEXT(struct gl_context *ctx, const struct marshal_cmd_VertexArrayVertexAttribLFormatEXT *cmd)
{
   GLuint vaobj = cmd->vaobj;
   GLuint attribindex = cmd->attribindex;
   GLint size = cmd->size;
   GLenum type = cmd->type;
   GLuint relativeoffset = cmd->relativeoffset;
   CALL_VertexArrayVertexAttribLFormatEXT(ctx->CurrentServerDispatch, (vaobj, attribindex, size, type, relativeoffset));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexArrayVertexAttribLFormatEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_VertexArrayVertexAttribLFormatEXT(GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexArrayVertexAttribLFormatEXT);
   struct marshal_cmd_VertexArrayVertexAttribLFormatEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexArrayVertexAttribLFormatEXT, cmd_size);
   cmd->vaobj = vaobj;
   cmd->attribindex = attribindex;
   cmd->size = size;
   cmd->type = MIN2(type, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->relativeoffset = relativeoffset;
   if (COMPAT) _mesa_glthread_DSAAttribFormat(ctx, vaobj, attribindex, size, type, relativeoffset);
}


/* VertexArrayVertexAttribBindingEXT: marshalled asynchronously */
struct marshal_cmd_VertexArrayVertexAttribBindingEXT
{
   struct marshal_cmd_base cmd_base;
   GLuint vaobj;
   GLuint attribindex;
   GLuint bindingindex;
};
uint32_t
_mesa_unmarshal_VertexArrayVertexAttribBindingEXT(struct gl_context *ctx, const struct marshal_cmd_VertexArrayVertexAttribBindingEXT *cmd)
{
   GLuint vaobj = cmd->vaobj;
   GLuint attribindex = cmd->attribindex;
   GLuint bindingindex = cmd->bindingindex;
   CALL_VertexArrayVertexAttribBindingEXT(ctx->CurrentServerDispatch, (vaobj, attribindex, bindingindex));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexArrayVertexAttribBindingEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_VertexArrayVertexAttribBindingEXT(GLuint vaobj, GLuint attribindex, GLuint bindingindex)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexArrayVertexAttribBindingEXT);
   struct marshal_cmd_VertexArrayVertexAttribBindingEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexArrayVertexAttribBindingEXT, cmd_size);
   cmd->vaobj = vaobj;
   cmd->attribindex = attribindex;
   cmd->bindingindex = bindingindex;
   if (COMPAT) _mesa_glthread_DSAAttribBinding(ctx, vaobj, attribindex, bindingindex);
}


/* VertexArrayVertexBindingDivisorEXT: marshalled asynchronously */
struct marshal_cmd_VertexArrayVertexBindingDivisorEXT
{
   struct marshal_cmd_base cmd_base;
   GLuint vaobj;
   GLuint bindingindex;
   GLuint divisor;
};
uint32_t
_mesa_unmarshal_VertexArrayVertexBindingDivisorEXT(struct gl_context *ctx, const struct marshal_cmd_VertexArrayVertexBindingDivisorEXT *cmd)
{
   GLuint vaobj = cmd->vaobj;
   GLuint bindingindex = cmd->bindingindex;
   GLuint divisor = cmd->divisor;
   CALL_VertexArrayVertexBindingDivisorEXT(ctx->CurrentServerDispatch, (vaobj, bindingindex, divisor));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexArrayVertexBindingDivisorEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_VertexArrayVertexBindingDivisorEXT(GLuint vaobj, GLuint bindingindex, GLuint divisor)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexArrayVertexBindingDivisorEXT);
   struct marshal_cmd_VertexArrayVertexBindingDivisorEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexArrayVertexBindingDivisorEXT, cmd_size);
   cmd->vaobj = vaobj;
   cmd->bindingindex = bindingindex;
   cmd->divisor = divisor;
   if (COMPAT) _mesa_glthread_DSABindingDivisor(ctx, vaobj, bindingindex, divisor);
}


/* FramebufferParameteri: marshalled asynchronously */
struct marshal_cmd_FramebufferParameteri
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLenum16 pname;
   GLint param;
};
uint32_t
_mesa_unmarshal_FramebufferParameteri(struct gl_context *ctx, const struct marshal_cmd_FramebufferParameteri *cmd)
{
   GLenum target = cmd->target;
   GLenum pname = cmd->pname;
   GLint param = cmd->param;
   CALL_FramebufferParameteri(ctx->CurrentServerDispatch, (target, pname, param));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_FramebufferParameteri), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_FramebufferParameteri(GLenum target, GLenum pname, GLint param)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_FramebufferParameteri);
   struct marshal_cmd_FramebufferParameteri *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_FramebufferParameteri, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->pname = MIN2(pname, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->param = param;
}


/* GetFramebufferParameteriv: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetFramebufferParameteriv(GLenum target, GLenum pname, GLint * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetFramebufferParameteriv");
   CALL_GetFramebufferParameteriv(ctx->CurrentServerDispatch, (target, pname, params));
}


/* NamedFramebufferParameteriEXT: marshalled asynchronously */
struct marshal_cmd_NamedFramebufferParameteriEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 pname;
   GLuint framebuffer;
   GLint param;
};
uint32_t
_mesa_unmarshal_NamedFramebufferParameteriEXT(struct gl_context *ctx, const struct marshal_cmd_NamedFramebufferParameteriEXT *cmd)
{
   GLuint framebuffer = cmd->framebuffer;
   GLenum pname = cmd->pname;
   GLint param = cmd->param;
   CALL_NamedFramebufferParameteriEXT(ctx->CurrentServerDispatch, (framebuffer, pname, param));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_NamedFramebufferParameteriEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_NamedFramebufferParameteriEXT(GLuint framebuffer, GLenum pname, GLint param)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_NamedFramebufferParameteriEXT);
   struct marshal_cmd_NamedFramebufferParameteriEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_NamedFramebufferParameteriEXT, cmd_size);
   cmd->framebuffer = framebuffer;
   cmd->pname = MIN2(pname, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->param = param;
}


/* GetNamedFramebufferParameterivEXT: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetNamedFramebufferParameterivEXT(GLuint framebuffer, GLenum pname, GLint* params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetNamedFramebufferParameterivEXT");
   CALL_GetNamedFramebufferParameterivEXT(ctx->CurrentServerDispatch, (framebuffer, pname, params));
}


/* GetInternalformati64v: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetInternalformati64v(GLenum target, GLenum internalformat, GLenum pname, GLsizei bufSize, GLint64 * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetInternalformati64v");
   CALL_GetInternalformati64v(ctx->CurrentServerDispatch, (target, internalformat, pname, bufSize, params));
}


/* InvalidateTexSubImage: marshalled asynchronously */
struct marshal_cmd_InvalidateTexSubImage
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
};
uint32_t
_mesa_unmarshal_InvalidateTexSubImage(struct gl_context *ctx, const struct marshal_cmd_InvalidateTexSubImage *cmd)
{
   GLuint texture = cmd->texture;
   GLint level = cmd->level;
   GLint xoffset = cmd->xoffset;
   GLint yoffset = cmd->yoffset;
   GLint zoffset = cmd->zoffset;
   GLsizei width = cmd->width;
   GLsizei height = cmd->height;
   GLsizei depth = cmd->depth;
   CALL_InvalidateTexSubImage(ctx->CurrentServerDispatch, (texture, level, xoffset, yoffset, zoffset, width, height, depth));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_InvalidateTexSubImage), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_InvalidateTexSubImage(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_InvalidateTexSubImage);
   struct marshal_cmd_InvalidateTexSubImage *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_InvalidateTexSubImage, cmd_size);
   cmd->texture = texture;
   cmd->level = level;
   cmd->xoffset = xoffset;
   cmd->yoffset = yoffset;
   cmd->zoffset = zoffset;
   cmd->width = width;
   cmd->height = height;
   cmd->depth = depth;
}


/* InvalidateTexImage: marshalled asynchronously */
struct marshal_cmd_InvalidateTexImage
{
   struct marshal_cmd_base cmd_base;
   GLuint texture;
   GLint level;
};
uint32_t
_mesa_unmarshal_InvalidateTexImage(struct gl_context *ctx, const struct marshal_cmd_InvalidateTexImage *cmd)
{
   GLuint texture = cmd->texture;
   GLint level = cmd->level;
   CALL_InvalidateTexImage(ctx->CurrentServerDispatch, (texture, level));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_InvalidateTexImage), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_InvalidateTexImage(GLuint texture, GLint level)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_InvalidateTexImage);
   struct marshal_cmd_InvalidateTexImage *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_InvalidateTexImage, cmd_size);
   cmd->texture = texture;
   cmd->level = level;
}


/* InvalidateBufferSubData: marshalled asynchronously */
struct marshal_cmd_InvalidateBufferSubData
{
   struct marshal_cmd_base cmd_base;
   GLuint buffer;
   GLintptr offset;
   GLsizeiptr length;
};
uint32_t
_mesa_unmarshal_InvalidateBufferSubData(struct gl_context *ctx, const struct marshal_cmd_InvalidateBufferSubData *cmd)
{
   GLuint buffer = cmd->buffer;
   GLintptr offset = cmd->offset;
   GLsizeiptr length = cmd->length;
   CALL_InvalidateBufferSubData(ctx->CurrentServerDispatch, (buffer, offset, length));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_InvalidateBufferSubData), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_InvalidateBufferSubData(GLuint buffer, GLintptr offset, GLsizeiptr length)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_InvalidateBufferSubData);
   struct marshal_cmd_InvalidateBufferSubData *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_InvalidateBufferSubData, cmd_size);
   cmd->buffer = buffer;
   cmd->offset = offset;
   cmd->length = length;
}


/* InvalidateBufferData: marshalled asynchronously */
struct marshal_cmd_InvalidateBufferData
{
   struct marshal_cmd_base cmd_base;
   GLuint buffer;
};
uint32_t
_mesa_unmarshal_InvalidateBufferData(struct gl_context *ctx, const struct marshal_cmd_InvalidateBufferData *cmd)
{
   GLuint buffer = cmd->buffer;
   CALL_InvalidateBufferData(ctx->CurrentServerDispatch, (buffer));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_InvalidateBufferData), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_InvalidateBufferData(GLuint buffer)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_InvalidateBufferData);
   struct marshal_cmd_InvalidateBufferData *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_InvalidateBufferData, cmd_size);
   cmd->buffer = buffer;
}


/* InvalidateSubFramebuffer: marshalled asynchronously */
struct marshal_cmd_InvalidateSubFramebuffer
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLsizei numAttachments;
   GLint x;
   GLint y;
   GLsizei width;
   GLsizei height;
   /* Next safe_mul(numAttachments, 1 * sizeof(GLenum)) bytes are GLenum attachments[numAttachments] */
};
uint32_t
_mesa_unmarshal_InvalidateSubFramebuffer(struct gl_context *ctx, const struct marshal_cmd_InvalidateSubFramebuffer *cmd)
{
   GLenum target = cmd->target;
   GLsizei numAttachments = cmd->numAttachments;
   GLint x = cmd->x;
   GLint y = cmd->y;
   GLsizei width = cmd->width;
   GLsizei height = cmd->height;
   GLenum *attachments;
   const char *variable_data = (const char *) (cmd + 1);
   attachments = (GLenum *) variable_data;
   CALL_InvalidateSubFramebuffer(ctx->CurrentServerDispatch, (target, numAttachments, attachments, x, y, width, height));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_InvalidateSubFramebuffer(GLenum target, GLsizei numAttachments, const GLenum * attachments, GLint x, GLint y, GLsizei width, GLsizei height)
{
   GET_CURRENT_CONTEXT(ctx);
   int attachments_size = safe_mul(numAttachments, 1 * sizeof(GLenum));
   int cmd_size = sizeof(struct marshal_cmd_InvalidateSubFramebuffer) + attachments_size;
   struct marshal_cmd_InvalidateSubFramebuffer *cmd;
   if (unlikely(attachments_size < 0 || (attachments_size > 0 && !attachments) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "InvalidateSubFramebuffer");
      CALL_InvalidateSubFramebuffer(ctx->CurrentServerDispatch, (target, numAttachments, attachments, x, y, width, height));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_InvalidateSubFramebuffer, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->numAttachments = numAttachments;
   cmd->x = x;
   cmd->y = y;
   cmd->width = width;
   cmd->height = height;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, attachments, attachments_size);
}


/* InvalidateFramebuffer: marshalled asynchronously */
struct marshal_cmd_InvalidateFramebuffer
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLsizei numAttachments;
   /* Next safe_mul(numAttachments, 1 * sizeof(GLenum)) bytes are GLenum attachments[numAttachments] */
};
uint32_t
_mesa_unmarshal_InvalidateFramebuffer(struct gl_context *ctx, const struct marshal_cmd_InvalidateFramebuffer *cmd)
{
   GLenum target = cmd->target;
   GLsizei numAttachments = cmd->numAttachments;
   GLenum *attachments;
   const char *variable_data = (const char *) (cmd + 1);
   attachments = (GLenum *) variable_data;
   CALL_InvalidateFramebuffer(ctx->CurrentServerDispatch, (target, numAttachments, attachments));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_InvalidateFramebuffer(GLenum target, GLsizei numAttachments, const GLenum * attachments)
{
   GET_CURRENT_CONTEXT(ctx);
   int attachments_size = safe_mul(numAttachments, 1 * sizeof(GLenum));
   int cmd_size = sizeof(struct marshal_cmd_InvalidateFramebuffer) + attachments_size;
   struct marshal_cmd_InvalidateFramebuffer *cmd;
   if (unlikely(attachments_size < 0 || (attachments_size > 0 && !attachments) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "InvalidateFramebuffer");
      CALL_InvalidateFramebuffer(ctx->CurrentServerDispatch, (target, numAttachments, attachments));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_InvalidateFramebuffer, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->numAttachments = numAttachments;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, attachments, attachments_size);
}


/* GetProgramInterfaceiv: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetProgramInterfaceiv(GLuint program, GLenum programInterface, GLenum pname, GLint * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetProgramInterfaceiv");
   CALL_GetProgramInterfaceiv(ctx->CurrentServerDispatch, (program, programInterface, pname, params));
}


/* GetProgramResourceIndex: marshalled synchronously */
static GLuint GLAPIENTRY
_mesa_marshal_GetProgramResourceIndex(GLuint program, GLenum programInterface, const GLchar * name)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetProgramResourceIndex");
   return CALL_GetProgramResourceIndex(ctx->CurrentServerDispatch, (program, programInterface, name));
}


/* GetProgramResourceName: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetProgramResourceName(GLuint program, GLenum programInterface, GLuint index, GLsizei  bufSize, GLsizei * length, GLchar * name)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetProgramResourceName");
   CALL_GetProgramResourceName(ctx->CurrentServerDispatch, (program, programInterface, index, bufSize, length, name));
}


/* GetProgramResourceiv: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetProgramResourceiv(GLuint program, GLenum programInterface, GLuint index, GLsizei  propCount, const GLenum * props, GLsizei  bufSize, GLsizei * length, GLint * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetProgramResourceiv");
   CALL_GetProgramResourceiv(ctx->CurrentServerDispatch, (program, programInterface, index, propCount, props, bufSize, length, params));
}


/* GetProgramResourceLocation: marshalled synchronously */
static GLint GLAPIENTRY
_mesa_marshal_GetProgramResourceLocation(GLuint program, GLenum programInterface, const GLchar * name)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetProgramResourceLocation");
   return CALL_GetProgramResourceLocation(ctx->CurrentServerDispatch, (program, programInterface, name));
}


/* GetProgramResourceLocationIndex: marshalled synchronously */
static GLint GLAPIENTRY
_mesa_marshal_GetProgramResourceLocationIndex(GLuint program, GLenum programInterface, const GLchar * name)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetProgramResourceLocationIndex");
   return CALL_GetProgramResourceLocationIndex(ctx->CurrentServerDispatch, (program, programInterface, name));
}


/* ShaderStorageBlockBinding: marshalled asynchronously */
struct marshal_cmd_ShaderStorageBlockBinding
{
   struct marshal_cmd_base cmd_base;
   GLuint program;
   GLuint shaderStorageBlockIndex;
   GLuint shaderStorageBlockBinding;
};
uint32_t
_mesa_unmarshal_ShaderStorageBlockBinding(struct gl_context *ctx, const struct marshal_cmd_ShaderStorageBlockBinding *cmd)
{
   GLuint program = cmd->program;
   GLuint shaderStorageBlockIndex = cmd->shaderStorageBlockIndex;
   GLuint shaderStorageBlockBinding = cmd->shaderStorageBlockBinding;
   CALL_ShaderStorageBlockBinding(ctx->CurrentServerDispatch, (program, shaderStorageBlockIndex, shaderStorageBlockBinding));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_ShaderStorageBlockBinding), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ShaderStorageBlockBinding(GLuint program, GLuint shaderStorageBlockIndex, GLuint shaderStorageBlockBinding)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ShaderStorageBlockBinding);
   struct marshal_cmd_ShaderStorageBlockBinding *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ShaderStorageBlockBinding, cmd_size);
   cmd->program = program;
   cmd->shaderStorageBlockIndex = shaderStorageBlockIndex;
   cmd->shaderStorageBlockBinding = shaderStorageBlockBinding;
}


/* TexBufferRange: marshalled asynchronously */
struct marshal_cmd_TexBufferRange
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLenum16 internalformat;
   GLuint buffer;
   GLintptr offset;
   GLsizeiptr size;
};
uint32_t
_mesa_unmarshal_TexBufferRange(struct gl_context *ctx, const struct marshal_cmd_TexBufferRange *cmd)
{
   GLenum target = cmd->target;
   GLenum internalformat = cmd->internalformat;
   GLuint buffer = cmd->buffer;
   GLintptr offset = cmd->offset;
   GLsizeiptr size = cmd->size;
   CALL_TexBufferRange(ctx->CurrentServerDispatch, (target, internalformat, buffer, offset, size));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_TexBufferRange), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_TexBufferRange(GLenum target, GLenum internalformat, GLuint buffer, GLintptr offset, GLsizeiptr size)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TexBufferRange);
   struct marshal_cmd_TexBufferRange *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexBufferRange, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->internalformat = MIN2(internalformat, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->buffer = buffer;
   cmd->offset = offset;
   cmd->size = size;
}


/* TextureBufferRangeEXT: marshalled asynchronously */
struct marshal_cmd_TextureBufferRangeEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLenum16 internalformat;
   GLuint texture;
   GLuint buffer;
   GLintptr offset;
   GLsizeiptr size;
};
uint32_t
_mesa_unmarshal_TextureBufferRangeEXT(struct gl_context *ctx, const struct marshal_cmd_TextureBufferRangeEXT *cmd)
{
   GLuint texture = cmd->texture;
   GLenum target = cmd->target;
   GLenum internalformat = cmd->internalformat;
   GLuint buffer = cmd->buffer;
   GLintptr offset = cmd->offset;
   GLsizeiptr size = cmd->size;
   CALL_TextureBufferRangeEXT(ctx->CurrentServerDispatch, (texture, target, internalformat, buffer, offset, size));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_TextureBufferRangeEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_TextureBufferRangeEXT(GLuint texture, GLenum target, GLenum internalformat, GLuint buffer, GLintptr offset, GLsizeiptr size)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TextureBufferRangeEXT);
   struct marshal_cmd_TextureBufferRangeEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TextureBufferRangeEXT, cmd_size);
   cmd->texture = texture;
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->internalformat = MIN2(internalformat, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->buffer = buffer;
   cmd->offset = offset;
   cmd->size = size;
}


/* TexStorage2DMultisample: marshalled asynchronously */
struct marshal_cmd_TexStorage2DMultisample
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
_mesa_unmarshal_TexStorage2DMultisample(struct gl_context *ctx, const struct marshal_cmd_TexStorage2DMultisample *cmd)
{
   GLenum target = cmd->target;
   GLsizei samples = cmd->samples;
   GLenum internalformat = cmd->internalformat;
   GLsizei width = cmd->width;
   GLsizei height = cmd->height;
   GLboolean fixedsamplelocations = cmd->fixedsamplelocations;
   CALL_TexStorage2DMultisample(ctx->CurrentServerDispatch, (target, samples, internalformat, width, height, fixedsamplelocations));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_TexStorage2DMultisample), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_TexStorage2DMultisample(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TexStorage2DMultisample);
   struct marshal_cmd_TexStorage2DMultisample *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexStorage2DMultisample, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->samples = samples;
   cmd->internalformat = MIN2(internalformat, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->width = width;
   cmd->height = height;
   cmd->fixedsamplelocations = fixedsamplelocations;
}


/* TexStorage3DMultisample: marshalled asynchronously */
struct marshal_cmd_TexStorage3DMultisample
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
_mesa_unmarshal_TexStorage3DMultisample(struct gl_context *ctx, const struct marshal_cmd_TexStorage3DMultisample *cmd)
{
   GLenum target = cmd->target;
   GLsizei samples = cmd->samples;
   GLenum internalformat = cmd->internalformat;
   GLsizei width = cmd->width;
   GLsizei height = cmd->height;
   GLsizei depth = cmd->depth;
   GLboolean fixedsamplelocations = cmd->fixedsamplelocations;
   CALL_TexStorage3DMultisample(ctx->CurrentServerDispatch, (target, samples, internalformat, width, height, depth, fixedsamplelocations));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_TexStorage3DMultisample), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_TexStorage3DMultisample(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TexStorage3DMultisample);
   struct marshal_cmd_TexStorage3DMultisample *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexStorage3DMultisample, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->samples = samples;
   cmd->internalformat = MIN2(internalformat, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->width = width;
   cmd->height = height;
   cmd->depth = depth;
   cmd->fixedsamplelocations = fixedsamplelocations;
}


/* TextureStorage2DMultisampleEXT: marshalled asynchronously */
struct marshal_cmd_TextureStorage2DMultisampleEXT
{
   struct marshal_cmd_base cmd_base;
   GLboolean fixedsamplelocations;
   GLenum16 target;
   GLenum16 internalformat;
   GLuint texture;
   GLsizei samples;
   GLsizei width;
   GLsizei height;
};
uint32_t
_mesa_unmarshal_TextureStorage2DMultisampleEXT(struct gl_context *ctx, const struct marshal_cmd_TextureStorage2DMultisampleEXT *cmd)
{
   GLuint texture = cmd->texture;
   GLenum target = cmd->target;
   GLsizei samples = cmd->samples;
   GLenum internalformat = cmd->internalformat;
   GLsizei width = cmd->width;
   GLsizei height = cmd->height;
   GLboolean fixedsamplelocations = cmd->fixedsamplelocations;
   CALL_TextureStorage2DMultisampleEXT(ctx->CurrentServerDispatch, (texture, target, samples, internalformat, width, height, fixedsamplelocations));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_TextureStorage2DMultisampleEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_TextureStorage2DMultisampleEXT(GLuint texture, GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TextureStorage2DMultisampleEXT);
   struct marshal_cmd_TextureStorage2DMultisampleEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TextureStorage2DMultisampleEXT, cmd_size);
   cmd->texture = texture;
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->samples = samples;
   cmd->internalformat = MIN2(internalformat, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->width = width;
   cmd->height = height;
   cmd->fixedsamplelocations = fixedsamplelocations;
}


/* TextureStorage3DMultisampleEXT: marshalled asynchronously */
struct marshal_cmd_TextureStorage3DMultisampleEXT
{
   struct marshal_cmd_base cmd_base;
   GLboolean fixedsamplelocations;
   GLenum16 target;
   GLenum16 internalformat;
   GLuint texture;
   GLsizei samples;
   GLsizei width;
   GLsizei height;
   GLsizei depth;
};
uint32_t
_mesa_unmarshal_TextureStorage3DMultisampleEXT(struct gl_context *ctx, const struct marshal_cmd_TextureStorage3DMultisampleEXT *cmd)
{
   GLuint texture = cmd->texture;
   GLenum target = cmd->target;
   GLsizei samples = cmd->samples;
   GLenum internalformat = cmd->internalformat;
   GLsizei width = cmd->width;
   GLsizei height = cmd->height;
   GLsizei depth = cmd->depth;
   GLboolean fixedsamplelocations = cmd->fixedsamplelocations;
   CALL_TextureStorage3DMultisampleEXT(ctx->CurrentServerDispatch, (texture, target, samples, internalformat, width, height, depth, fixedsamplelocations));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_TextureStorage3DMultisampleEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_TextureStorage3DMultisampleEXT(GLuint texture, GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TextureStorage3DMultisampleEXT);
   struct marshal_cmd_TextureStorage3DMultisampleEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TextureStorage3DMultisampleEXT, cmd_size);
   cmd->texture = texture;
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->samples = samples;
   cmd->internalformat = MIN2(internalformat, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->width = width;
   cmd->height = height;
   cmd->depth = depth;
   cmd->fixedsamplelocations = fixedsamplelocations;
}


/* BufferStorage: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_BufferStorage(GLenum target, GLsizeiptr size, const GLvoid * data, GLbitfield flags)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "BufferStorage");
   CALL_BufferStorage(ctx->CurrentServerDispatch, (target, size, data, flags));
}


/* NamedBufferStorageEXT: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_NamedBufferStorageEXT(GLuint buffer, GLsizeiptr size, const GLvoid * data, GLbitfield flags)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "NamedBufferStorageEXT");
   CALL_NamedBufferStorageEXT(ctx->CurrentServerDispatch, (buffer, size, data, flags));
}


/* ClearTexImage: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_ClearTexImage(GLuint texture, GLint level, GLenum format, GLenum type, const GLvoid * data)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "ClearTexImage");
   CALL_ClearTexImage(ctx->CurrentServerDispatch, (texture, level, format, type, data));
}


/* ClearTexSubImage: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_ClearTexSubImage(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const GLvoid * data)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "ClearTexSubImage");
   CALL_ClearTexSubImage(ctx->CurrentServerDispatch, (texture, level, xoffset, yoffset, zoffset, width, height, depth, format, type, data));
}


/* BindBuffersBase: marshalled asynchronously */
struct marshal_cmd_BindBuffersBase
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLuint first;
   GLsizei count;
   /* Next safe_mul(count, 1 * sizeof(GLuint)) bytes are GLuint buffers[count] */
};
uint32_t
_mesa_unmarshal_BindBuffersBase(struct gl_context *ctx, const struct marshal_cmd_BindBuffersBase *cmd)
{
   GLenum target = cmd->target;
   GLuint first = cmd->first;
   GLsizei count = cmd->count;
   GLuint *buffers;
   const char *variable_data = (const char *) (cmd + 1);
   buffers = (GLuint *) variable_data;
   CALL_BindBuffersBase(ctx->CurrentServerDispatch, (target, first, count, buffers));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_BindBuffersBase(GLenum target, GLuint first, GLsizei count, const GLuint * buffers)
{
   GET_CURRENT_CONTEXT(ctx);
   int buffers_size = safe_mul(count, 1 * sizeof(GLuint));
   int cmd_size = sizeof(struct marshal_cmd_BindBuffersBase) + buffers_size;
   struct marshal_cmd_BindBuffersBase *cmd;
   if (unlikely(buffers_size < 0 || (buffers_size > 0 && !buffers) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "BindBuffersBase");
      CALL_BindBuffersBase(ctx->CurrentServerDispatch, (target, first, count, buffers));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_BindBuffersBase, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->first = first;
   cmd->count = count;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, buffers, buffers_size);
}


/* BindBuffersRange: marshalled asynchronously */
struct marshal_cmd_BindBuffersRange
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLuint first;
   GLsizei count;
   /* Next safe_mul(count, 1 * sizeof(GLuint)) bytes are GLuint buffers[count] */
   /* Next safe_mul(count, 1 * sizeof(GLintptr)) bytes are GLintptr offsets[count] */
   /* Next safe_mul(count, 1 * sizeof(GLsizeiptr)) bytes are GLsizeiptr sizes[count] */
};
uint32_t
_mesa_unmarshal_BindBuffersRange(struct gl_context *ctx, const struct marshal_cmd_BindBuffersRange *cmd)
{
   GLenum target = cmd->target;
   GLuint first = cmd->first;
   GLsizei count = cmd->count;
   GLuint *buffers;
   GLintptr *offsets;
   GLsizeiptr *sizes;
   const char *variable_data = (const char *) (cmd + 1);
   buffers = (GLuint *) variable_data;
   variable_data += count * 1 * sizeof(GLuint);
   offsets = (GLintptr *) variable_data;
   variable_data += count * 1 * sizeof(GLintptr);
   sizes = (GLsizeiptr *) variable_data;
   CALL_BindBuffersRange(ctx->CurrentServerDispatch, (target, first, count, buffers, offsets, sizes));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_BindBuffersRange(GLenum target, GLuint first, GLsizei count, const GLuint * buffers, const GLintptr * offsets, const GLsizeiptr * sizes)
{
   GET_CURRENT_CONTEXT(ctx);
   int buffers_size = safe_mul(count, 1 * sizeof(GLuint));
   int offsets_size = safe_mul(count, 1 * sizeof(GLintptr));
   int sizes_size = safe_mul(count, 1 * sizeof(GLsizeiptr));
   int cmd_size = sizeof(struct marshal_cmd_BindBuffersRange) + buffers_size + offsets_size + sizes_size;
   struct marshal_cmd_BindBuffersRange *cmd;
   if (unlikely(buffers_size < 0 || (buffers_size > 0 && !buffers) || offsets_size < 0 || (offsets_size > 0 && !offsets) || sizes_size < 0 || (sizes_size > 0 && !sizes) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "BindBuffersRange");
      CALL_BindBuffersRange(ctx->CurrentServerDispatch, (target, first, count, buffers, offsets, sizes));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_BindBuffersRange, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->first = first;
   cmd->count = count;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, buffers, buffers_size);
   variable_data += buffers_size;
   memcpy(variable_data, offsets, offsets_size);
   variable_data += offsets_size;
   memcpy(variable_data, sizes, sizes_size);
}


/* BindTextures: marshalled asynchronously */
struct marshal_cmd_BindTextures
{
   struct marshal_cmd_base cmd_base;
   GLuint first;
   GLsizei count;
   /* Next safe_mul(count, 1 * sizeof(GLuint)) bytes are GLuint textures[count] */
};
uint32_t
_mesa_unmarshal_BindTextures(struct gl_context *ctx, const struct marshal_cmd_BindTextures *cmd)
{
   GLuint first = cmd->first;
   GLsizei count = cmd->count;
   GLuint *textures;
   const char *variable_data = (const char *) (cmd + 1);
   textures = (GLuint *) variable_data;
   CALL_BindTextures(ctx->CurrentServerDispatch, (first, count, textures));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_BindTextures(GLuint first, GLsizei count, const GLuint * textures)
{
   GET_CURRENT_CONTEXT(ctx);
   int textures_size = safe_mul(count, 1 * sizeof(GLuint));
   int cmd_size = sizeof(struct marshal_cmd_BindTextures) + textures_size;
   struct marshal_cmd_BindTextures *cmd;
   if (unlikely(textures_size < 0 || (textures_size > 0 && !textures) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "BindTextures");
      CALL_BindTextures(ctx->CurrentServerDispatch, (first, count, textures));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_BindTextures, cmd_size);
   cmd->first = first;
   cmd->count = count;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, textures, textures_size);
}


/* BindSamplers: marshalled asynchronously */
struct marshal_cmd_BindSamplers
{
   struct marshal_cmd_base cmd_base;
   GLuint first;
   GLsizei count;
   /* Next safe_mul(count, 1 * sizeof(GLuint)) bytes are GLuint samplers[count] */
};
uint32_t
_mesa_unmarshal_BindSamplers(struct gl_context *ctx, const struct marshal_cmd_BindSamplers *cmd)
{
   GLuint first = cmd->first;
   GLsizei count = cmd->count;
   GLuint *samplers;
   const char *variable_data = (const char *) (cmd + 1);
   samplers = (GLuint *) variable_data;
   CALL_BindSamplers(ctx->CurrentServerDispatch, (first, count, samplers));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_BindSamplers(GLuint first, GLsizei count, const GLuint * samplers)
{
   GET_CURRENT_CONTEXT(ctx);
   int samplers_size = safe_mul(count, 1 * sizeof(GLuint));
   int cmd_size = sizeof(struct marshal_cmd_BindSamplers) + samplers_size;
   struct marshal_cmd_BindSamplers *cmd;
   if (unlikely(samplers_size < 0 || (samplers_size > 0 && !samplers) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "BindSamplers");
      CALL_BindSamplers(ctx->CurrentServerDispatch, (first, count, samplers));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_BindSamplers, cmd_size);
   cmd->first = first;
   cmd->count = count;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, samplers, samplers_size);
}


/* BindImageTextures: marshalled asynchronously */
struct marshal_cmd_BindImageTextures
{
   struct marshal_cmd_base cmd_base;
   GLuint first;
   GLsizei count;
   /* Next safe_mul(count, 1 * sizeof(GLuint)) bytes are GLuint textures[count] */
};
uint32_t
_mesa_unmarshal_BindImageTextures(struct gl_context *ctx, const struct marshal_cmd_BindImageTextures *cmd)
{
   GLuint first = cmd->first;
   GLsizei count = cmd->count;
   GLuint *textures;
   const char *variable_data = (const char *) (cmd + 1);
   textures = (GLuint *) variable_data;
   CALL_BindImageTextures(ctx->CurrentServerDispatch, (first, count, textures));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_BindImageTextures(GLuint first, GLsizei count, const GLuint * textures)
{
   GET_CURRENT_CONTEXT(ctx);
   int textures_size = safe_mul(count, 1 * sizeof(GLuint));
   int cmd_size = sizeof(struct marshal_cmd_BindImageTextures) + textures_size;
   struct marshal_cmd_BindImageTextures *cmd;
   if (unlikely(textures_size < 0 || (textures_size > 0 && !textures) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "BindImageTextures");
      CALL_BindImageTextures(ctx->CurrentServerDispatch, (first, count, textures));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_BindImageTextures, cmd_size);
   cmd->first = first;
   cmd->count = count;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, textures, textures_size);
}


/* BindVertexBuffers: marshalled asynchronously */
struct marshal_cmd_BindVertexBuffers
{
   struct marshal_cmd_base cmd_base;
   GLuint first;
   GLsizei count;
   /* Next safe_mul(count, 1 * sizeof(GLuint)) bytes are GLuint buffers[count] */
   /* Next safe_mul(count, 1 * sizeof(GLintptr)) bytes are GLintptr offsets[count] */
   /* Next safe_mul(count, 1 * sizeof(GLsizei)) bytes are GLsizei strides[count] */
};
uint32_t
_mesa_unmarshal_BindVertexBuffers(struct gl_context *ctx, const struct marshal_cmd_BindVertexBuffers *cmd)
{
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
   CALL_BindVertexBuffers(ctx->CurrentServerDispatch, (first, count, buffers, offsets, strides));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_BindVertexBuffers(GLuint first, GLsizei count, const GLuint * buffers, const GLintptr * offsets, const GLsizei * strides)
{
   GET_CURRENT_CONTEXT(ctx);
   int buffers_size = safe_mul(count, 1 * sizeof(GLuint));
   int offsets_size = safe_mul(count, 1 * sizeof(GLintptr));
   int strides_size = safe_mul(count, 1 * sizeof(GLsizei));
   int cmd_size = sizeof(struct marshal_cmd_BindVertexBuffers) + buffers_size + offsets_size + strides_size;
   struct marshal_cmd_BindVertexBuffers *cmd;
   if (unlikely(buffers_size < 0 || (buffers_size > 0 && !buffers) || offsets_size < 0 || (offsets_size > 0 && !offsets) || strides_size < 0 || (strides_size > 0 && !strides) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "BindVertexBuffers");
      CALL_BindVertexBuffers(ctx->CurrentServerDispatch, (first, count, buffers, offsets, strides));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_BindVertexBuffers, cmd_size);
   cmd->first = first;
   cmd->count = count;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, buffers, buffers_size);
   variable_data += buffers_size;
   memcpy(variable_data, offsets, offsets_size);
   variable_data += offsets_size;
   memcpy(variable_data, strides, strides_size);
}


/* GetTextureHandleARB: marshalled synchronously */
static GLuint64 GLAPIENTRY
_mesa_marshal_GetTextureHandleARB(GLuint texture)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetTextureHandleARB");
   return CALL_GetTextureHandleARB(ctx->CurrentServerDispatch, (texture));
}


/* GetTextureSamplerHandleARB: marshalled synchronously */
static GLuint64 GLAPIENTRY
_mesa_marshal_GetTextureSamplerHandleARB(GLuint texture, GLuint sampler)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetTextureSamplerHandleARB");
   return CALL_GetTextureSamplerHandleARB(ctx->CurrentServerDispatch, (texture, sampler));
}


/* MakeTextureHandleResidentARB: marshalled asynchronously */
struct marshal_cmd_MakeTextureHandleResidentARB
{
   struct marshal_cmd_base cmd_base;
   GLuint64 handle;
};
uint32_t
_mesa_unmarshal_MakeTextureHandleResidentARB(struct gl_context *ctx, const struct marshal_cmd_MakeTextureHandleResidentARB *cmd)
{
   GLuint64 handle = cmd->handle;
   CALL_MakeTextureHandleResidentARB(ctx->CurrentServerDispatch, (handle));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MakeTextureHandleResidentARB), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_MakeTextureHandleResidentARB(GLuint64 handle)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MakeTextureHandleResidentARB);
   struct marshal_cmd_MakeTextureHandleResidentARB *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MakeTextureHandleResidentARB, cmd_size);
   cmd->handle = handle;
}


/* MakeTextureHandleNonResidentARB: marshalled asynchronously */
struct marshal_cmd_MakeTextureHandleNonResidentARB
{
   struct marshal_cmd_base cmd_base;
   GLuint64 handle;
};
uint32_t
_mesa_unmarshal_MakeTextureHandleNonResidentARB(struct gl_context *ctx, const struct marshal_cmd_MakeTextureHandleNonResidentARB *cmd)
{
   GLuint64 handle = cmd->handle;
   CALL_MakeTextureHandleNonResidentARB(ctx->CurrentServerDispatch, (handle));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MakeTextureHandleNonResidentARB), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_MakeTextureHandleNonResidentARB(GLuint64 handle)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MakeTextureHandleNonResidentARB);
   struct marshal_cmd_MakeTextureHandleNonResidentARB *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MakeTextureHandleNonResidentARB, cmd_size);
   cmd->handle = handle;
}


/* GetImageHandleARB: marshalled synchronously */
static GLuint64 GLAPIENTRY
_mesa_marshal_GetImageHandleARB(GLuint texture, GLint level, GLboolean layered, GLint layer, GLenum format)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetImageHandleARB");
   return CALL_GetImageHandleARB(ctx->CurrentServerDispatch, (texture, level, layered, layer, format));
}


/* MakeImageHandleResidentARB: marshalled asynchronously */
struct marshal_cmd_MakeImageHandleResidentARB
{
   struct marshal_cmd_base cmd_base;
   GLenum16 access;
   GLuint64 handle;
};
uint32_t
_mesa_unmarshal_MakeImageHandleResidentARB(struct gl_context *ctx, const struct marshal_cmd_MakeImageHandleResidentARB *cmd)
{
   GLuint64 handle = cmd->handle;
   GLenum access = cmd->access;
   CALL_MakeImageHandleResidentARB(ctx->CurrentServerDispatch, (handle, access));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MakeImageHandleResidentARB), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_MakeImageHandleResidentARB(GLuint64 handle, GLenum access)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MakeImageHandleResidentARB);
   struct marshal_cmd_MakeImageHandleResidentARB *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MakeImageHandleResidentARB, cmd_size);
   cmd->handle = handle;
   cmd->access = MIN2(access, 0xffff); /* clamped to 0xffff (invalid enum) */
}


/* MakeImageHandleNonResidentARB: marshalled asynchronously */
struct marshal_cmd_MakeImageHandleNonResidentARB
{
   struct marshal_cmd_base cmd_base;
   GLuint64 handle;
};
uint32_t
_mesa_unmarshal_MakeImageHandleNonResidentARB(struct gl_context *ctx, const struct marshal_cmd_MakeImageHandleNonResidentARB *cmd)
{
   GLuint64 handle = cmd->handle;
   CALL_MakeImageHandleNonResidentARB(ctx->CurrentServerDispatch, (handle));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MakeImageHandleNonResidentARB), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_MakeImageHandleNonResidentARB(GLuint64 handle)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MakeImageHandleNonResidentARB);
   struct marshal_cmd_MakeImageHandleNonResidentARB *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MakeImageHandleNonResidentARB, cmd_size);
   cmd->handle = handle;
}


/* UniformHandleui64ARB: marshalled asynchronously */
struct marshal_cmd_UniformHandleui64ARB
{
   struct marshal_cmd_base cmd_base;
   GLint location;
   GLuint64 value;
};
uint32_t
_mesa_unmarshal_UniformHandleui64ARB(struct gl_context *ctx, const struct marshal_cmd_UniformHandleui64ARB *cmd)
{
   GLint location = cmd->location;
   GLuint64 value = cmd->value;
   CALL_UniformHandleui64ARB(ctx->CurrentServerDispatch, (location, value));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_UniformHandleui64ARB), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_UniformHandleui64ARB(GLint location, GLuint64 value)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_UniformHandleui64ARB);
   struct marshal_cmd_UniformHandleui64ARB *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_UniformHandleui64ARB, cmd_size);
   cmd->location = location;
   cmd->value = value;
}


/* UniformHandleui64vARB: marshalled asynchronously */
struct marshal_cmd_UniformHandleui64vARB
{
   struct marshal_cmd_base cmd_base;
   GLint location;
   GLsizei count;
   /* Next safe_mul(count, 1 * sizeof(GLuint64)) bytes are GLuint64 value[count] */
};
uint32_t
_mesa_unmarshal_UniformHandleui64vARB(struct gl_context *ctx, const struct marshal_cmd_UniformHandleui64vARB *cmd)
{
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLuint64 *value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLuint64 *) variable_data;
   CALL_UniformHandleui64vARB(ctx->CurrentServerDispatch, (location, count, value));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_UniformHandleui64vARB(GLint location, GLsizei count, const GLuint64 * value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 1 * sizeof(GLuint64));
   int cmd_size = sizeof(struct marshal_cmd_UniformHandleui64vARB) + value_size;
   struct marshal_cmd_UniformHandleui64vARB *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "UniformHandleui64vARB");
      CALL_UniformHandleui64vARB(ctx->CurrentServerDispatch, (location, count, value));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_UniformHandleui64vARB, cmd_size);
   cmd->location = location;
   cmd->count = count;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, value, value_size);
}


/* ProgramUniformHandleui64ARB: marshalled asynchronously */
struct marshal_cmd_ProgramUniformHandleui64ARB
{
   struct marshal_cmd_base cmd_base;
   GLuint program;
   GLint location;
   GLuint64 value;
};
uint32_t
_mesa_unmarshal_ProgramUniformHandleui64ARB(struct gl_context *ctx, const struct marshal_cmd_ProgramUniformHandleui64ARB *cmd)
{
   GLuint program = cmd->program;
   GLint location = cmd->location;
   GLuint64 value = cmd->value;
   CALL_ProgramUniformHandleui64ARB(ctx->CurrentServerDispatch, (program, location, value));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_ProgramUniformHandleui64ARB), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ProgramUniformHandleui64ARB(GLuint program, GLint location, GLuint64 value)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ProgramUniformHandleui64ARB);
   struct marshal_cmd_ProgramUniformHandleui64ARB *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ProgramUniformHandleui64ARB, cmd_size);
   cmd->program = program;
   cmd->location = location;
   cmd->value = value;
}


/* ProgramUniformHandleui64vARB: marshalled asynchronously */
struct marshal_cmd_ProgramUniformHandleui64vARB
{
   struct marshal_cmd_base cmd_base;
   GLuint program;
   GLint location;
   GLsizei count;
   /* Next safe_mul(count, 1 * sizeof(GLuint64)) bytes are GLuint64 value[count] */
};
uint32_t
_mesa_unmarshal_ProgramUniformHandleui64vARB(struct gl_context *ctx, const struct marshal_cmd_ProgramUniformHandleui64vARB *cmd)
{
   GLuint program = cmd->program;
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLuint64 *value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLuint64 *) variable_data;
   CALL_ProgramUniformHandleui64vARB(ctx->CurrentServerDispatch, (program, location, count, value));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ProgramUniformHandleui64vARB(GLuint program, GLint location, GLsizei count, const GLuint64 * value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 1 * sizeof(GLuint64));
   int cmd_size = sizeof(struct marshal_cmd_ProgramUniformHandleui64vARB) + value_size;
   struct marshal_cmd_ProgramUniformHandleui64vARB *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "ProgramUniformHandleui64vARB");
      CALL_ProgramUniformHandleui64vARB(ctx->CurrentServerDispatch, (program, location, count, value));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ProgramUniformHandleui64vARB, cmd_size);
   cmd->program = program;
   cmd->location = location;
   cmd->count = count;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, value, value_size);
}


/* IsTextureHandleResidentARB: marshalled synchronously */
static GLboolean GLAPIENTRY
_mesa_marshal_IsTextureHandleResidentARB(GLuint64 handle)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "IsTextureHandleResidentARB");
   return CALL_IsTextureHandleResidentARB(ctx->CurrentServerDispatch, (handle));
}


/* IsImageHandleResidentARB: marshalled synchronously */
static GLboolean GLAPIENTRY
_mesa_marshal_IsImageHandleResidentARB(GLuint64 handle)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "IsImageHandleResidentARB");
   return CALL_IsImageHandleResidentARB(ctx->CurrentServerDispatch, (handle));
}


/* VertexAttribL1ui64ARB: marshalled asynchronously */
struct marshal_cmd_VertexAttribL1ui64ARB
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLuint64EXT x;
};
uint32_t
_mesa_unmarshal_VertexAttribL1ui64ARB(struct gl_context *ctx, const struct marshal_cmd_VertexAttribL1ui64ARB *cmd)
{
   GLuint index = cmd->index;
   GLuint64EXT x = cmd->x;
   CALL_VertexAttribL1ui64ARB(ctx->CurrentServerDispatch, (index, x));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexAttribL1ui64ARB), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_VertexAttribL1ui64ARB(GLuint index, GLuint64EXT x)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttribL1ui64ARB);
   struct marshal_cmd_VertexAttribL1ui64ARB *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttribL1ui64ARB, cmd_size);
   cmd->index = index;
   cmd->x = x;
}


/* VertexAttribL1ui64vARB: marshalled asynchronously */
struct marshal_cmd_VertexAttribL1ui64vARB
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLuint64EXT v[1];
};
uint32_t
_mesa_unmarshal_VertexAttribL1ui64vARB(struct gl_context *ctx, const struct marshal_cmd_VertexAttribL1ui64vARB *cmd)
{
   GLuint index = cmd->index;
   const GLuint64EXT *v = cmd->v;
   CALL_VertexAttribL1ui64vARB(ctx->CurrentServerDispatch, (index, v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexAttribL1ui64vARB), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_VertexAttribL1ui64vARB(GLuint index, const GLuint64EXT * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttribL1ui64vARB);
   struct marshal_cmd_VertexAttribL1ui64vARB *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttribL1ui64vARB, cmd_size);
   cmd->index = index;
   memcpy(cmd->v, v, 1 * sizeof(GLuint64EXT));
}


/* GetVertexAttribLui64vARB: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetVertexAttribLui64vARB(GLuint index, GLenum pname, GLuint64EXT * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetVertexAttribLui64vARB");
   CALL_GetVertexAttribLui64vARB(ctx->CurrentServerDispatch, (index, pname, params));
}


/* DispatchComputeGroupSizeARB: marshalled asynchronously */
struct marshal_cmd_DispatchComputeGroupSizeARB
{
   struct marshal_cmd_base cmd_base;
   GLuint num_groups_x;
   GLuint num_groups_y;
   GLuint num_groups_z;
   GLuint group_size_x;
   GLuint group_size_y;
   GLuint group_size_z;
};
uint32_t
_mesa_unmarshal_DispatchComputeGroupSizeARB(struct gl_context *ctx, const struct marshal_cmd_DispatchComputeGroupSizeARB *cmd)
{
   GLuint num_groups_x = cmd->num_groups_x;
   GLuint num_groups_y = cmd->num_groups_y;
   GLuint num_groups_z = cmd->num_groups_z;
   GLuint group_size_x = cmd->group_size_x;
   GLuint group_size_y = cmd->group_size_y;
   GLuint group_size_z = cmd->group_size_z;
   CALL_DispatchComputeGroupSizeARB(ctx->CurrentServerDispatch, (num_groups_x, num_groups_y, num_groups_z, group_size_x, group_size_y, group_size_z));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_DispatchComputeGroupSizeARB), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_DispatchComputeGroupSizeARB(GLuint num_groups_x, GLuint num_groups_y, GLuint num_groups_z, GLuint group_size_x, GLuint group_size_y, GLuint group_size_z)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_DispatchComputeGroupSizeARB);
   struct marshal_cmd_DispatchComputeGroupSizeARB *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_DispatchComputeGroupSizeARB, cmd_size);
   cmd->num_groups_x = num_groups_x;
   cmd->num_groups_y = num_groups_y;
   cmd->num_groups_z = num_groups_z;
   cmd->group_size_x = group_size_x;
   cmd->group_size_y = group_size_y;
   cmd->group_size_z = group_size_z;
}


/* MultiDrawArraysIndirectCountARB: marshalled asynchronously */
struct marshal_cmd_MultiDrawArraysIndirectCountARB
{
   struct marshal_cmd_base cmd_base;
   GLenum16 mode;
   GLsizei maxdrawcount;
   GLsizei stride;
   GLintptr indirect;
   GLintptr drawcount;
};
uint32_t
_mesa_unmarshal_MultiDrawArraysIndirectCountARB(struct gl_context *ctx, const struct marshal_cmd_MultiDrawArraysIndirectCountARB *cmd)
{
   GLenum mode = cmd->mode;
   GLintptr indirect = cmd->indirect;
   GLintptr drawcount = cmd->drawcount;
   GLsizei maxdrawcount = cmd->maxdrawcount;
   GLsizei stride = cmd->stride;
   CALL_MultiDrawArraysIndirectCountARB(ctx->CurrentServerDispatch, (mode, indirect, drawcount, maxdrawcount, stride));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MultiDrawArraysIndirectCountARB), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_MultiDrawArraysIndirectCountARB(GLenum mode, GLintptr indirect, GLintptr drawcount, GLsizei maxdrawcount, GLsizei stride)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiDrawArraysIndirectCountARB);
   struct marshal_cmd_MultiDrawArraysIndirectCountARB *cmd;
   if (_mesa_glthread_has_non_vbo_vertices(ctx)) {
      _mesa_glthread_finish_before(ctx, "MultiDrawArraysIndirectCountARB");
      CALL_MultiDrawArraysIndirectCountARB(ctx->CurrentServerDispatch, (mode, indirect, drawcount, maxdrawcount, stride));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiDrawArraysIndirectCountARB, cmd_size);
   cmd->mode = MIN2(mode, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->indirect = indirect;
   cmd->drawcount = drawcount;
   cmd->maxdrawcount = maxdrawcount;
   cmd->stride = stride;
}


/* MultiDrawElementsIndirectCountARB: marshalled asynchronously */
struct marshal_cmd_MultiDrawElementsIndirectCountARB
{
   struct marshal_cmd_base cmd_base;
   GLenum16 mode;
   GLenum16 type;
   GLsizei maxdrawcount;
   GLsizei stride;
   GLintptr indirect;
   GLintptr drawcount;
};
uint32_t
_mesa_unmarshal_MultiDrawElementsIndirectCountARB(struct gl_context *ctx, const struct marshal_cmd_MultiDrawElementsIndirectCountARB *cmd)
{
   GLenum mode = cmd->mode;
   GLenum type = cmd->type;
   GLintptr indirect = cmd->indirect;
   GLintptr drawcount = cmd->drawcount;
   GLsizei maxdrawcount = cmd->maxdrawcount;
   GLsizei stride = cmd->stride;
   CALL_MultiDrawElementsIndirectCountARB(ctx->CurrentServerDispatch, (mode, type, indirect, drawcount, maxdrawcount, stride));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MultiDrawElementsIndirectCountARB), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_MultiDrawElementsIndirectCountARB(GLenum mode, GLenum type, GLintptr indirect, GLintptr drawcount, GLsizei maxdrawcount, GLsizei stride)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiDrawElementsIndirectCountARB);
   struct marshal_cmd_MultiDrawElementsIndirectCountARB *cmd;
   if (_mesa_glthread_has_non_vbo_vertices(ctx)) {
      _mesa_glthread_finish_before(ctx, "MultiDrawElementsIndirectCountARB");
      CALL_MultiDrawElementsIndirectCountARB(ctx->CurrentServerDispatch, (mode, type, indirect, drawcount, maxdrawcount, stride));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiDrawElementsIndirectCountARB, cmd_size);
   cmd->mode = MIN2(mode, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->type = MIN2(type, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->indirect = indirect;
   cmd->drawcount = drawcount;
   cmd->maxdrawcount = maxdrawcount;
   cmd->stride = stride;
}


/* TexPageCommitmentARB: marshalled asynchronously */
struct marshal_cmd_TexPageCommitmentARB
{
   struct marshal_cmd_base cmd_base;
   GLboolean commit;
   GLenum16 target;
   GLint level;
   GLint xoffset;
   GLint yoffset;
   GLint zoffset;
   GLsizei width;
   GLsizei height;
   GLsizei depth;
};
uint32_t
_mesa_unmarshal_TexPageCommitmentARB(struct gl_context *ctx, const struct marshal_cmd_TexPageCommitmentARB *cmd)
{
   GLenum target = cmd->target;
   GLint level = cmd->level;
   GLint xoffset = cmd->xoffset;
   GLint yoffset = cmd->yoffset;
   GLint zoffset = cmd->zoffset;
   GLsizei width = cmd->width;
   GLsizei height = cmd->height;
   GLsizei depth = cmd->depth;
   GLboolean commit = cmd->commit;
   CALL_TexPageCommitmentARB(ctx->CurrentServerDispatch, (target, level, xoffset, yoffset, zoffset, width, height, depth, commit));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_TexPageCommitmentARB), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_TexPageCommitmentARB(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLboolean commit)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TexPageCommitmentARB);
   struct marshal_cmd_TexPageCommitmentARB *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexPageCommitmentARB, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->level = level;
   cmd->xoffset = xoffset;
   cmd->yoffset = yoffset;
   cmd->zoffset = zoffset;
   cmd->width = width;
   cmd->height = height;
   cmd->depth = depth;
   cmd->commit = commit;
}


/* TexturePageCommitmentEXT: marshalled asynchronously */
struct marshal_cmd_TexturePageCommitmentEXT
{
   struct marshal_cmd_base cmd_base;
   GLboolean commit;
   GLuint texture;
   GLint level;
   GLint xoffset;
   GLint yoffset;
   GLint zoffset;
   GLsizei width;
   GLsizei height;
   GLsizei depth;
};
uint32_t
_mesa_unmarshal_TexturePageCommitmentEXT(struct gl_context *ctx, const struct marshal_cmd_TexturePageCommitmentEXT *cmd)
{
   GLuint texture = cmd->texture;
   GLint level = cmd->level;
   GLint xoffset = cmd->xoffset;
   GLint yoffset = cmd->yoffset;
   GLint zoffset = cmd->zoffset;
   GLsizei width = cmd->width;
   GLsizei height = cmd->height;
   GLsizei depth = cmd->depth;
   GLboolean commit = cmd->commit;
   CALL_TexturePageCommitmentEXT(ctx->CurrentServerDispatch, (texture, level, xoffset, yoffset, zoffset, width, height, depth, commit));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_TexturePageCommitmentEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_TexturePageCommitmentEXT(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLboolean commit)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TexturePageCommitmentEXT);
   struct marshal_cmd_TexturePageCommitmentEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexturePageCommitmentEXT, cmd_size);
   cmd->texture = texture;
   cmd->level = level;
   cmd->xoffset = xoffset;
   cmd->yoffset = yoffset;
   cmd->zoffset = zoffset;
   cmd->width = width;
   cmd->height = height;
   cmd->depth = depth;
   cmd->commit = commit;
}


/* ClipControl: marshalled asynchronously */
struct marshal_cmd_ClipControl
{
   struct marshal_cmd_base cmd_base;
   GLenum16 origin;
   GLenum16 depth;
};
uint32_t
_mesa_unmarshal_ClipControl(struct gl_context *ctx, const struct marshal_cmd_ClipControl *cmd)
{
   GLenum origin = cmd->origin;
   GLenum depth = cmd->depth;
   CALL_ClipControl(ctx->CurrentServerDispatch, (origin, depth));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_ClipControl), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ClipControl(GLenum origin, GLenum depth)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ClipControl);
   struct marshal_cmd_ClipControl *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ClipControl, cmd_size);
   cmd->origin = MIN2(origin, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->depth = MIN2(depth, 0xffff); /* clamped to 0xffff (invalid enum) */
}


/* CreateTransformFeedbacks: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_CreateTransformFeedbacks(GLsizei n, GLuint * ids)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "CreateTransformFeedbacks");
   CALL_CreateTransformFeedbacks(ctx->CurrentServerDispatch, (n, ids));
}


/* TransformFeedbackBufferBase: marshalled asynchronously */
struct marshal_cmd_TransformFeedbackBufferBase
{
   struct marshal_cmd_base cmd_base;
   GLuint xfb;
   GLuint index;
   GLuint buffer;
};
uint32_t
_mesa_unmarshal_TransformFeedbackBufferBase(struct gl_context *ctx, const struct marshal_cmd_TransformFeedbackBufferBase *cmd)
{
   GLuint xfb = cmd->xfb;
   GLuint index = cmd->index;
   GLuint buffer = cmd->buffer;
   CALL_TransformFeedbackBufferBase(ctx->CurrentServerDispatch, (xfb, index, buffer));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_TransformFeedbackBufferBase), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_TransformFeedbackBufferBase(GLuint xfb, GLuint index, GLuint buffer)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TransformFeedbackBufferBase);
   struct marshal_cmd_TransformFeedbackBufferBase *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TransformFeedbackBufferBase, cmd_size);
   cmd->xfb = xfb;
   cmd->index = index;
   cmd->buffer = buffer;
}


/* TransformFeedbackBufferRange: marshalled asynchronously */
struct marshal_cmd_TransformFeedbackBufferRange
{
   struct marshal_cmd_base cmd_base;
   GLuint xfb;
   GLuint index;
   GLuint buffer;
   GLintptr offset;
   GLsizeiptr size;
};
uint32_t
_mesa_unmarshal_TransformFeedbackBufferRange(struct gl_context *ctx, const struct marshal_cmd_TransformFeedbackBufferRange *cmd)
{
   GLuint xfb = cmd->xfb;
   GLuint index = cmd->index;
   GLuint buffer = cmd->buffer;
   GLintptr offset = cmd->offset;
   GLsizeiptr size = cmd->size;
   CALL_TransformFeedbackBufferRange(ctx->CurrentServerDispatch, (xfb, index, buffer, offset, size));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_TransformFeedbackBufferRange), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_TransformFeedbackBufferRange(GLuint xfb, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TransformFeedbackBufferRange);
   struct marshal_cmd_TransformFeedbackBufferRange *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TransformFeedbackBufferRange, cmd_size);
   cmd->xfb = xfb;
   cmd->index = index;
   cmd->buffer = buffer;
   cmd->offset = offset;
   cmd->size = size;
}


/* GetTransformFeedbackiv: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetTransformFeedbackiv(GLuint xfb, GLenum pname, GLint * param)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetTransformFeedbackiv");
   CALL_GetTransformFeedbackiv(ctx->CurrentServerDispatch, (xfb, pname, param));
}


/* GetTransformFeedbacki_v: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetTransformFeedbacki_v(GLuint xfb, GLenum pname, GLuint index, GLint * param)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetTransformFeedbacki_v");
   CALL_GetTransformFeedbacki_v(ctx->CurrentServerDispatch, (xfb, pname, index, param));
}


/* GetTransformFeedbacki64_v: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetTransformFeedbacki64_v(GLuint xfb, GLenum pname, GLuint index, GLint64 * param)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetTransformFeedbacki64_v");
   CALL_GetTransformFeedbacki64_v(ctx->CurrentServerDispatch, (xfb, pname, index, param));
}


/* CreateBuffers: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_CreateBuffers(GLsizei n, GLuint * buffers)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "CreateBuffers");
   CALL_CreateBuffers(ctx->CurrentServerDispatch, (n, buffers));
}


/* NamedBufferStorage: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_NamedBufferStorage(GLuint buffer, GLsizeiptr size, const GLvoid * data, GLbitfield flags)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "NamedBufferStorage");
   CALL_NamedBufferStorage(ctx->CurrentServerDispatch, (buffer, size, data, flags));
}


/* CopyNamedBufferSubData: marshalled asynchronously */
struct marshal_cmd_CopyNamedBufferSubData
{
   struct marshal_cmd_base cmd_base;
   GLuint readBuffer;
   GLuint writeBuffer;
   GLintptr readOffset;
   GLintptr writeOffset;
   GLsizeiptr size;
};
uint32_t
_mesa_unmarshal_CopyNamedBufferSubData(struct gl_context *ctx, const struct marshal_cmd_CopyNamedBufferSubData *cmd)
{
   GLuint readBuffer = cmd->readBuffer;
   GLuint writeBuffer = cmd->writeBuffer;
   GLintptr readOffset = cmd->readOffset;
   GLintptr writeOffset = cmd->writeOffset;
   GLsizeiptr size = cmd->size;
   CALL_CopyNamedBufferSubData(ctx->CurrentServerDispatch, (readBuffer, writeBuffer, readOffset, writeOffset, size));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_CopyNamedBufferSubData), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_CopyNamedBufferSubData(GLuint readBuffer, GLuint writeBuffer, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_CopyNamedBufferSubData);
   struct marshal_cmd_CopyNamedBufferSubData *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_CopyNamedBufferSubData, cmd_size);
   cmd->readBuffer = readBuffer;
   cmd->writeBuffer = writeBuffer;
   cmd->readOffset = readOffset;
   cmd->writeOffset = writeOffset;
   cmd->size = size;
}


/* ClearNamedBufferData: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_ClearNamedBufferData(GLuint buffer, GLenum internalformat, GLenum format, GLenum type, const GLvoid * data)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "ClearNamedBufferData");
   CALL_ClearNamedBufferData(ctx->CurrentServerDispatch, (buffer, internalformat, format, type, data));
}


/* ClearNamedBufferSubData: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_ClearNamedBufferSubData(GLuint buffer, GLenum internalformat, GLintptr offset, GLsizeiptr size, GLenum format, GLenum type, const GLvoid * data)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "ClearNamedBufferSubData");
   CALL_ClearNamedBufferSubData(ctx->CurrentServerDispatch, (buffer, internalformat, offset, size, format, type, data));
}


/* MapNamedBuffer: marshalled synchronously */
static GLvoid * GLAPIENTRY
_mesa_marshal_MapNamedBuffer(GLuint buffer, GLenum access)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "MapNamedBuffer");
   return CALL_MapNamedBuffer(ctx->CurrentServerDispatch, (buffer, access));
}


/* MapNamedBufferRange: marshalled synchronously */
static GLvoid * GLAPIENTRY
_mesa_marshal_MapNamedBufferRange(GLuint buffer, GLintptr offset, GLsizeiptr length, GLbitfield access)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "MapNamedBufferRange");
   return CALL_MapNamedBufferRange(ctx->CurrentServerDispatch, (buffer, offset, length, access));
}


/* UnmapNamedBufferEXT: marshalled asynchronously */
struct marshal_cmd_UnmapNamedBufferEXT
{
   struct marshal_cmd_base cmd_base;
   GLuint buffer;
};
uint32_t
_mesa_unmarshal_UnmapNamedBufferEXT(struct gl_context *ctx, const struct marshal_cmd_UnmapNamedBufferEXT *cmd)
{
   GLuint buffer = cmd->buffer;
   CALL_UnmapNamedBufferEXT(ctx->CurrentServerDispatch, (buffer));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_UnmapNamedBufferEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static GLboolean GLAPIENTRY
_mesa_marshal_UnmapNamedBufferEXT(GLuint buffer)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_UnmapNamedBufferEXT);
   struct marshal_cmd_UnmapNamedBufferEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_UnmapNamedBufferEXT, cmd_size);
   cmd->buffer = buffer;
   return GL_TRUE;
}


/* FlushMappedNamedBufferRange: marshalled asynchronously */
struct marshal_cmd_FlushMappedNamedBufferRange
{
   struct marshal_cmd_base cmd_base;
   GLuint buffer;
   GLintptr offset;
   GLsizeiptr length;
};
uint32_t
_mesa_unmarshal_FlushMappedNamedBufferRange(struct gl_context *ctx, const struct marshal_cmd_FlushMappedNamedBufferRange *cmd)
{
   GLuint buffer = cmd->buffer;
   GLintptr offset = cmd->offset;
   GLsizeiptr length = cmd->length;
   CALL_FlushMappedNamedBufferRange(ctx->CurrentServerDispatch, (buffer, offset, length));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_FlushMappedNamedBufferRange), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_FlushMappedNamedBufferRange(GLuint buffer, GLintptr offset, GLsizeiptr length)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_FlushMappedNamedBufferRange);
   struct marshal_cmd_FlushMappedNamedBufferRange *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_FlushMappedNamedBufferRange, cmd_size);
   cmd->buffer = buffer;
   cmd->offset = offset;
   cmd->length = length;
}


/* GetNamedBufferParameteriv: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetNamedBufferParameteriv(GLuint buffer, GLenum pname, GLint * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetNamedBufferParameteriv");
   CALL_GetNamedBufferParameteriv(ctx->CurrentServerDispatch, (buffer, pname, params));
}


/* GetNamedBufferParameteri64v: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetNamedBufferParameteri64v(GLuint buffer, GLenum pname, GLint64 * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetNamedBufferParameteri64v");
   CALL_GetNamedBufferParameteri64v(ctx->CurrentServerDispatch, (buffer, pname, params));
}


/* GetNamedBufferPointerv: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetNamedBufferPointerv(GLuint buffer, GLenum pname, GLvoid ** params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetNamedBufferPointerv");
   CALL_GetNamedBufferPointerv(ctx->CurrentServerDispatch, (buffer, pname, params));
}


/* GetNamedBufferSubData: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetNamedBufferSubData(GLuint buffer, GLintptr offset, GLsizeiptr size, GLvoid * data)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetNamedBufferSubData");
   CALL_GetNamedBufferSubData(ctx->CurrentServerDispatch, (buffer, offset, size, data));
}


/* CreateFramebuffers: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_CreateFramebuffers(GLsizei n, GLuint * framebuffers)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "CreateFramebuffers");
   CALL_CreateFramebuffers(ctx->CurrentServerDispatch, (n, framebuffers));
}


/* NamedFramebufferRenderbuffer: marshalled asynchronously */
struct marshal_cmd_NamedFramebufferRenderbuffer
{
   struct marshal_cmd_base cmd_base;
   GLenum16 attachment;
   GLenum16 renderbuffertarget;
   GLuint framebuffer;
   GLuint renderbuffer;
};
uint32_t
_mesa_unmarshal_NamedFramebufferRenderbuffer(struct gl_context *ctx, const struct marshal_cmd_NamedFramebufferRenderbuffer *cmd)
{
   GLuint framebuffer = cmd->framebuffer;
   GLenum attachment = cmd->attachment;
   GLenum renderbuffertarget = cmd->renderbuffertarget;
   GLuint renderbuffer = cmd->renderbuffer;
   CALL_NamedFramebufferRenderbuffer(ctx->CurrentServerDispatch, (framebuffer, attachment, renderbuffertarget, renderbuffer));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_NamedFramebufferRenderbuffer), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_NamedFramebufferRenderbuffer(GLuint framebuffer, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_NamedFramebufferRenderbuffer);
   struct marshal_cmd_NamedFramebufferRenderbuffer *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_NamedFramebufferRenderbuffer, cmd_size);
   cmd->framebuffer = framebuffer;
   cmd->attachment = MIN2(attachment, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->renderbuffertarget = MIN2(renderbuffertarget, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->renderbuffer = renderbuffer;
}


/* NamedFramebufferParameteri: marshalled asynchronously */
struct marshal_cmd_NamedFramebufferParameteri
{
   struct marshal_cmd_base cmd_base;
   GLenum16 pname;
   GLuint framebuffer;
   GLint param;
};
uint32_t
_mesa_unmarshal_NamedFramebufferParameteri(struct gl_context *ctx, const struct marshal_cmd_NamedFramebufferParameteri *cmd)
{
   GLuint framebuffer = cmd->framebuffer;
   GLenum pname = cmd->pname;
   GLint param = cmd->param;
   CALL_NamedFramebufferParameteri(ctx->CurrentServerDispatch, (framebuffer, pname, param));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_NamedFramebufferParameteri), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_NamedFramebufferParameteri(GLuint framebuffer, GLenum pname, GLint param)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_NamedFramebufferParameteri);
   struct marshal_cmd_NamedFramebufferParameteri *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_NamedFramebufferParameteri, cmd_size);
   cmd->framebuffer = framebuffer;
   cmd->pname = MIN2(pname, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->param = param;
}


/* NamedFramebufferTexture: marshalled asynchronously */
struct marshal_cmd_NamedFramebufferTexture
{
   struct marshal_cmd_base cmd_base;
   GLenum16 attachment;
   GLuint framebuffer;
   GLuint texture;
   GLint level;
};
uint32_t
_mesa_unmarshal_NamedFramebufferTexture(struct gl_context *ctx, const struct marshal_cmd_NamedFramebufferTexture *cmd)
{
   GLuint framebuffer = cmd->framebuffer;
   GLenum attachment = cmd->attachment;
   GLuint texture = cmd->texture;
   GLint level = cmd->level;
   CALL_NamedFramebufferTexture(ctx->CurrentServerDispatch, (framebuffer, attachment, texture, level));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_NamedFramebufferTexture), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_NamedFramebufferTexture(GLuint framebuffer, GLenum attachment, GLuint texture, GLint level)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_NamedFramebufferTexture);
   struct marshal_cmd_NamedFramebufferTexture *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_NamedFramebufferTexture, cmd_size);
   cmd->framebuffer = framebuffer;
   cmd->attachment = MIN2(attachment, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->texture = texture;
   cmd->level = level;
}


/* NamedFramebufferTextureLayer: marshalled asynchronously */
struct marshal_cmd_NamedFramebufferTextureLayer
{
   struct marshal_cmd_base cmd_base;
   GLenum16 attachment;
   GLuint framebuffer;
   GLuint texture;
   GLint level;
   GLint layer;
};
uint32_t
_mesa_unmarshal_NamedFramebufferTextureLayer(struct gl_context *ctx, const struct marshal_cmd_NamedFramebufferTextureLayer *cmd)
{
   GLuint framebuffer = cmd->framebuffer;
   GLenum attachment = cmd->attachment;
   GLuint texture = cmd->texture;
   GLint level = cmd->level;
   GLint layer = cmd->layer;
   CALL_NamedFramebufferTextureLayer(ctx->CurrentServerDispatch, (framebuffer, attachment, texture, level, layer));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_NamedFramebufferTextureLayer), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_NamedFramebufferTextureLayer(GLuint framebuffer, GLenum attachment, GLuint texture, GLint level, GLint layer)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_NamedFramebufferTextureLayer);
   struct marshal_cmd_NamedFramebufferTextureLayer *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_NamedFramebufferTextureLayer, cmd_size);
   cmd->framebuffer = framebuffer;
   cmd->attachment = MIN2(attachment, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->texture = texture;
   cmd->level = level;
   cmd->layer = layer;
}


/* NamedFramebufferDrawBuffer: marshalled asynchronously */
struct marshal_cmd_NamedFramebufferDrawBuffer
{
   struct marshal_cmd_base cmd_base;
   GLenum16 buf;
   GLuint framebuffer;
};
uint32_t
_mesa_unmarshal_NamedFramebufferDrawBuffer(struct gl_context *ctx, const struct marshal_cmd_NamedFramebufferDrawBuffer *cmd)
{
   GLuint framebuffer = cmd->framebuffer;
   GLenum buf = cmd->buf;
   CALL_NamedFramebufferDrawBuffer(ctx->CurrentServerDispatch, (framebuffer, buf));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_NamedFramebufferDrawBuffer), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_NamedFramebufferDrawBuffer(GLuint framebuffer, GLenum buf)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_NamedFramebufferDrawBuffer);
   struct marshal_cmd_NamedFramebufferDrawBuffer *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_NamedFramebufferDrawBuffer, cmd_size);
   cmd->framebuffer = framebuffer;
   cmd->buf = MIN2(buf, 0xffff); /* clamped to 0xffff (invalid enum) */
}


/* NamedFramebufferDrawBuffers: marshalled asynchronously */
struct marshal_cmd_NamedFramebufferDrawBuffers
{
   struct marshal_cmd_base cmd_base;
   GLuint framebuffer;
   GLsizei n;
   /* Next safe_mul(n, 1 * sizeof(GLenum)) bytes are GLenum bufs[n] */
};
uint32_t
_mesa_unmarshal_NamedFramebufferDrawBuffers(struct gl_context *ctx, const struct marshal_cmd_NamedFramebufferDrawBuffers *cmd)
{
   GLuint framebuffer = cmd->framebuffer;
   GLsizei n = cmd->n;
   GLenum *bufs;
   const char *variable_data = (const char *) (cmd + 1);
   bufs = (GLenum *) variable_data;
   CALL_NamedFramebufferDrawBuffers(ctx->CurrentServerDispatch, (framebuffer, n, bufs));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_NamedFramebufferDrawBuffers(GLuint framebuffer, GLsizei n, const GLenum * bufs)
{
   GET_CURRENT_CONTEXT(ctx);
   int bufs_size = safe_mul(n, 1 * sizeof(GLenum));
   int cmd_size = sizeof(struct marshal_cmd_NamedFramebufferDrawBuffers) + bufs_size;
   struct marshal_cmd_NamedFramebufferDrawBuffers *cmd;
   if (unlikely(bufs_size < 0 || (bufs_size > 0 && !bufs) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "NamedFramebufferDrawBuffers");
      CALL_NamedFramebufferDrawBuffers(ctx->CurrentServerDispatch, (framebuffer, n, bufs));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_NamedFramebufferDrawBuffers, cmd_size);
   cmd->framebuffer = framebuffer;
   cmd->n = n;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, bufs, bufs_size);
}


/* NamedFramebufferReadBuffer: marshalled asynchronously */
struct marshal_cmd_NamedFramebufferReadBuffer
{
   struct marshal_cmd_base cmd_base;
   GLenum16 buf;
   GLuint framebuffer;
};
uint32_t
_mesa_unmarshal_NamedFramebufferReadBuffer(struct gl_context *ctx, const struct marshal_cmd_NamedFramebufferReadBuffer *cmd)
{
   GLuint framebuffer = cmd->framebuffer;
   GLenum buf = cmd->buf;
   CALL_NamedFramebufferReadBuffer(ctx->CurrentServerDispatch, (framebuffer, buf));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_NamedFramebufferReadBuffer), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_NamedFramebufferReadBuffer(GLuint framebuffer, GLenum buf)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_NamedFramebufferReadBuffer);
   struct marshal_cmd_NamedFramebufferReadBuffer *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_NamedFramebufferReadBuffer, cmd_size);
   cmd->framebuffer = framebuffer;
   cmd->buf = MIN2(buf, 0xffff); /* clamped to 0xffff (invalid enum) */
}


/* InvalidateNamedFramebufferData: marshalled asynchronously */
struct marshal_cmd_InvalidateNamedFramebufferData
{
   struct marshal_cmd_base cmd_base;
   GLuint framebuffer;
   GLsizei numAttachments;
   /* Next safe_mul(numAttachments, 1 * sizeof(GLenum)) bytes are GLenum attachments[numAttachments] */
};
uint32_t
_mesa_unmarshal_InvalidateNamedFramebufferData(struct gl_context *ctx, const struct marshal_cmd_InvalidateNamedFramebufferData *cmd)
{
   GLuint framebuffer = cmd->framebuffer;
   GLsizei numAttachments = cmd->numAttachments;
   GLenum *attachments;
   const char *variable_data = (const char *) (cmd + 1);
   attachments = (GLenum *) variable_data;
   CALL_InvalidateNamedFramebufferData(ctx->CurrentServerDispatch, (framebuffer, numAttachments, attachments));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_InvalidateNamedFramebufferData(GLuint framebuffer, GLsizei numAttachments, const GLenum * attachments)
{
   GET_CURRENT_CONTEXT(ctx);
   int attachments_size = safe_mul(numAttachments, 1 * sizeof(GLenum));
   int cmd_size = sizeof(struct marshal_cmd_InvalidateNamedFramebufferData) + attachments_size;
   struct marshal_cmd_InvalidateNamedFramebufferData *cmd;
   if (unlikely(attachments_size < 0 || (attachments_size > 0 && !attachments) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "InvalidateNamedFramebufferData");
      CALL_InvalidateNamedFramebufferData(ctx->CurrentServerDispatch, (framebuffer, numAttachments, attachments));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_InvalidateNamedFramebufferData, cmd_size);
   cmd->framebuffer = framebuffer;
   cmd->numAttachments = numAttachments;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, attachments, attachments_size);
}


/* InvalidateNamedFramebufferSubData: marshalled asynchronously */
struct marshal_cmd_InvalidateNamedFramebufferSubData
{
   struct marshal_cmd_base cmd_base;
   GLuint framebuffer;
   GLsizei numAttachments;
   GLint x;
   GLint y;
   GLsizei width;
   GLsizei height;
   /* Next safe_mul(numAttachments, 1 * sizeof(GLenum)) bytes are GLenum attachments[numAttachments] */
};
uint32_t
_mesa_unmarshal_InvalidateNamedFramebufferSubData(struct gl_context *ctx, const struct marshal_cmd_InvalidateNamedFramebufferSubData *cmd)
{
   GLuint framebuffer = cmd->framebuffer;
   GLsizei numAttachments = cmd->numAttachments;
   GLint x = cmd->x;
   GLint y = cmd->y;
   GLsizei width = cmd->width;
   GLsizei height = cmd->height;
   GLenum *attachments;
   const char *variable_data = (const char *) (cmd + 1);
   attachments = (GLenum *) variable_data;
   CALL_InvalidateNamedFramebufferSubData(ctx->CurrentServerDispatch, (framebuffer, numAttachments, attachments, x, y, width, height));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_InvalidateNamedFramebufferSubData(GLuint framebuffer, GLsizei numAttachments, const GLenum * attachments, GLint x, GLint y, GLsizei width, GLsizei height)
{
   GET_CURRENT_CONTEXT(ctx);
   int attachments_size = safe_mul(numAttachments, 1 * sizeof(GLenum));
   int cmd_size = sizeof(struct marshal_cmd_InvalidateNamedFramebufferSubData) + attachments_size;
   struct marshal_cmd_InvalidateNamedFramebufferSubData *cmd;
   if (unlikely(attachments_size < 0 || (attachments_size > 0 && !attachments) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "InvalidateNamedFramebufferSubData");
      CALL_InvalidateNamedFramebufferSubData(ctx->CurrentServerDispatch, (framebuffer, numAttachments, attachments, x, y, width, height));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_InvalidateNamedFramebufferSubData, cmd_size);
   cmd->framebuffer = framebuffer;
   cmd->numAttachments = numAttachments;
   cmd->x = x;
   cmd->y = y;
   cmd->width = width;
   cmd->height = height;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, attachments, attachments_size);
}


/* ClearNamedFramebufferiv: marshalled asynchronously */
struct marshal_cmd_ClearNamedFramebufferiv
{
   struct marshal_cmd_base cmd_base;
   GLenum16 buffer;
   GLuint framebuffer;
   GLint drawbuffer;
   /* Next safe_mul(_mesa_buffer_enum_to_count(buffer), 1 * sizeof(GLint)) bytes are GLint value[_mesa_buffer_enum_to_count(buffer)] */
};
uint32_t
_mesa_unmarshal_ClearNamedFramebufferiv(struct gl_context *ctx, const struct marshal_cmd_ClearNamedFramebufferiv *cmd)
{
   GLuint framebuffer = cmd->framebuffer;
   GLenum buffer = cmd->buffer;
   GLint drawbuffer = cmd->drawbuffer;
   GLint *value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLint *) variable_data;
   CALL_ClearNamedFramebufferiv(ctx->CurrentServerDispatch, (framebuffer, buffer, drawbuffer, value));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ClearNamedFramebufferiv(GLuint framebuffer, GLenum buffer, GLint drawbuffer, const GLint * value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(_mesa_buffer_enum_to_count(buffer), 1 * sizeof(GLint));
   int cmd_size = sizeof(struct marshal_cmd_ClearNamedFramebufferiv) + value_size;
   struct marshal_cmd_ClearNamedFramebufferiv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "ClearNamedFramebufferiv");
      CALL_ClearNamedFramebufferiv(ctx->CurrentServerDispatch, (framebuffer, buffer, drawbuffer, value));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ClearNamedFramebufferiv, cmd_size);
   cmd->framebuffer = framebuffer;
   cmd->buffer = MIN2(buffer, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->drawbuffer = drawbuffer;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, value, value_size);
}


/* ClearNamedFramebufferuiv: marshalled asynchronously */
struct marshal_cmd_ClearNamedFramebufferuiv
{
   struct marshal_cmd_base cmd_base;
   GLenum16 buffer;
   GLuint framebuffer;
   GLint drawbuffer;
   /* Next safe_mul(_mesa_buffer_enum_to_count(buffer), 1 * sizeof(GLuint)) bytes are GLuint value[_mesa_buffer_enum_to_count(buffer)] */
};
uint32_t
_mesa_unmarshal_ClearNamedFramebufferuiv(struct gl_context *ctx, const struct marshal_cmd_ClearNamedFramebufferuiv *cmd)
{
   GLuint framebuffer = cmd->framebuffer;
   GLenum buffer = cmd->buffer;
   GLint drawbuffer = cmd->drawbuffer;
   GLuint *value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLuint *) variable_data;
   CALL_ClearNamedFramebufferuiv(ctx->CurrentServerDispatch, (framebuffer, buffer, drawbuffer, value));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ClearNamedFramebufferuiv(GLuint framebuffer, GLenum buffer, GLint drawbuffer, const GLuint * value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(_mesa_buffer_enum_to_count(buffer), 1 * sizeof(GLuint));
   int cmd_size = sizeof(struct marshal_cmd_ClearNamedFramebufferuiv) + value_size;
   struct marshal_cmd_ClearNamedFramebufferuiv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "ClearNamedFramebufferuiv");
      CALL_ClearNamedFramebufferuiv(ctx->CurrentServerDispatch, (framebuffer, buffer, drawbuffer, value));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ClearNamedFramebufferuiv, cmd_size);
   cmd->framebuffer = framebuffer;
   cmd->buffer = MIN2(buffer, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->drawbuffer = drawbuffer;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, value, value_size);
}


/* ClearNamedFramebufferfv: marshalled asynchronously */
struct marshal_cmd_ClearNamedFramebufferfv
{
   struct marshal_cmd_base cmd_base;
   GLenum16 buffer;
   GLuint framebuffer;
   GLint drawbuffer;
   /* Next safe_mul(_mesa_buffer_enum_to_count(buffer), 1 * sizeof(GLfloat)) bytes are GLfloat value[_mesa_buffer_enum_to_count(buffer)] */
};
uint32_t
_mesa_unmarshal_ClearNamedFramebufferfv(struct gl_context *ctx, const struct marshal_cmd_ClearNamedFramebufferfv *cmd)
{
   GLuint framebuffer = cmd->framebuffer;
   GLenum buffer = cmd->buffer;
   GLint drawbuffer = cmd->drawbuffer;
   GLfloat *value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLfloat *) variable_data;
   CALL_ClearNamedFramebufferfv(ctx->CurrentServerDispatch, (framebuffer, buffer, drawbuffer, value));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ClearNamedFramebufferfv(GLuint framebuffer, GLenum buffer, GLint drawbuffer, const GLfloat * value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(_mesa_buffer_enum_to_count(buffer), 1 * sizeof(GLfloat));
   int cmd_size = sizeof(struct marshal_cmd_ClearNamedFramebufferfv) + value_size;
   struct marshal_cmd_ClearNamedFramebufferfv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "ClearNamedFramebufferfv");
      CALL_ClearNamedFramebufferfv(ctx->CurrentServerDispatch, (framebuffer, buffer, drawbuffer, value));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ClearNamedFramebufferfv, cmd_size);
   cmd->framebuffer = framebuffer;
   cmd->buffer = MIN2(buffer, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->drawbuffer = drawbuffer;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, value, value_size);
}


/* ClearNamedFramebufferfi: marshalled asynchronously */
struct marshal_cmd_ClearNamedFramebufferfi
{
   struct marshal_cmd_base cmd_base;
   GLenum16 buffer;
   GLuint framebuffer;
   GLint drawbuffer;
   GLfloat depth;
   GLint stencil;
};
uint32_t
_mesa_unmarshal_ClearNamedFramebufferfi(struct gl_context *ctx, const struct marshal_cmd_ClearNamedFramebufferfi *cmd)
{
   GLuint framebuffer = cmd->framebuffer;
   GLenum buffer = cmd->buffer;
   GLint drawbuffer = cmd->drawbuffer;
   GLfloat depth = cmd->depth;
   GLint stencil = cmd->stencil;
   CALL_ClearNamedFramebufferfi(ctx->CurrentServerDispatch, (framebuffer, buffer, drawbuffer, depth, stencil));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_ClearNamedFramebufferfi), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ClearNamedFramebufferfi(GLuint framebuffer, GLenum buffer, GLint drawbuffer, GLfloat depth, GLint stencil)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ClearNamedFramebufferfi);
   struct marshal_cmd_ClearNamedFramebufferfi *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ClearNamedFramebufferfi, cmd_size);
   cmd->framebuffer = framebuffer;
   cmd->buffer = MIN2(buffer, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->drawbuffer = drawbuffer;
   cmd->depth = depth;
   cmd->stencil = stencil;
}


/* BlitNamedFramebuffer: marshalled asynchronously */
struct marshal_cmd_BlitNamedFramebuffer
{
   struct marshal_cmd_base cmd_base;
   GLenum16 filter;
   GLuint readFramebuffer;
   GLuint drawFramebuffer;
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
_mesa_unmarshal_BlitNamedFramebuffer(struct gl_context *ctx, const struct marshal_cmd_BlitNamedFramebuffer *cmd)
{
   GLuint readFramebuffer = cmd->readFramebuffer;
   GLuint drawFramebuffer = cmd->drawFramebuffer;
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
   CALL_BlitNamedFramebuffer(ctx->CurrentServerDispatch, (readFramebuffer, drawFramebuffer, srcX0, srcY0, srcX1, srcY1, dstX0, dstY0, dstX1, dstY1, mask, filter));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_BlitNamedFramebuffer), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_BlitNamedFramebuffer(GLuint readFramebuffer, GLuint drawFramebuffer, GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_BlitNamedFramebuffer);
   struct marshal_cmd_BlitNamedFramebuffer *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_BlitNamedFramebuffer, cmd_size);
   cmd->readFramebuffer = readFramebuffer;
   cmd->drawFramebuffer = drawFramebuffer;
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


/* CheckNamedFramebufferStatus: marshalled synchronously */
static GLenum GLAPIENTRY
_mesa_marshal_CheckNamedFramebufferStatus(GLuint framebuffer, GLenum target)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "CheckNamedFramebufferStatus");
   return CALL_CheckNamedFramebufferStatus(ctx->CurrentServerDispatch, (framebuffer, target));
}


/* GetNamedFramebufferParameteriv: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetNamedFramebufferParameteriv(GLuint framebuffer, GLenum pname, GLint * param)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetNamedFramebufferParameteriv");
   CALL_GetNamedFramebufferParameteriv(ctx->CurrentServerDispatch, (framebuffer, pname, param));
}


/* GetNamedFramebufferAttachmentParameteriv: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetNamedFramebufferAttachmentParameteriv(GLuint framebuffer, GLenum attachment, GLenum pname, GLint * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetNamedFramebufferAttachmentParameteriv");
   CALL_GetNamedFramebufferAttachmentParameteriv(ctx->CurrentServerDispatch, (framebuffer, attachment, pname, params));
}


/* CreateRenderbuffers: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_CreateRenderbuffers(GLsizei n, GLuint * renderbuffers)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "CreateRenderbuffers");
   CALL_CreateRenderbuffers(ctx->CurrentServerDispatch, (n, renderbuffers));
}


/* NamedRenderbufferStorage: marshalled asynchronously */
struct marshal_cmd_NamedRenderbufferStorage
{
   struct marshal_cmd_base cmd_base;
   GLenum16 internalformat;
   GLuint renderbuffer;
   GLsizei width;
   GLsizei height;
};
uint32_t
_mesa_unmarshal_NamedRenderbufferStorage(struct gl_context *ctx, const struct marshal_cmd_NamedRenderbufferStorage *cmd)
{
   GLuint renderbuffer = cmd->renderbuffer;
   GLenum internalformat = cmd->internalformat;
   GLsizei width = cmd->width;
   GLsizei height = cmd->height;
   CALL_NamedRenderbufferStorage(ctx->CurrentServerDispatch, (renderbuffer, internalformat, width, height));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_NamedRenderbufferStorage), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_NamedRenderbufferStorage(GLuint renderbuffer, GLenum internalformat, GLsizei width, GLsizei height)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_NamedRenderbufferStorage);
   struct marshal_cmd_NamedRenderbufferStorage *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_NamedRenderbufferStorage, cmd_size);
   cmd->renderbuffer = renderbuffer;
   cmd->internalformat = MIN2(internalformat, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->width = width;
   cmd->height = height;
}


/* NamedRenderbufferStorageMultisample: marshalled asynchronously */
struct marshal_cmd_NamedRenderbufferStorageMultisample
{
   struct marshal_cmd_base cmd_base;
   GLenum16 internalformat;
   GLuint renderbuffer;
   GLsizei samples;
   GLsizei width;
   GLsizei height;
};
uint32_t
_mesa_unmarshal_NamedRenderbufferStorageMultisample(struct gl_context *ctx, const struct marshal_cmd_NamedRenderbufferStorageMultisample *cmd)
{
   GLuint renderbuffer = cmd->renderbuffer;
   GLsizei samples = cmd->samples;
   GLenum internalformat = cmd->internalformat;
   GLsizei width = cmd->width;
   GLsizei height = cmd->height;
   CALL_NamedRenderbufferStorageMultisample(ctx->CurrentServerDispatch, (renderbuffer, samples, internalformat, width, height));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_NamedRenderbufferStorageMultisample), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_NamedRenderbufferStorageMultisample(GLuint renderbuffer, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_NamedRenderbufferStorageMultisample);
   struct marshal_cmd_NamedRenderbufferStorageMultisample *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_NamedRenderbufferStorageMultisample, cmd_size);
   cmd->renderbuffer = renderbuffer;
   cmd->samples = samples;
   cmd->internalformat = MIN2(internalformat, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->width = width;
   cmd->height = height;
}


/* GetNamedRenderbufferParameteriv: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetNamedRenderbufferParameteriv(GLuint renderbuffer, GLenum pname, GLint * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetNamedRenderbufferParameteriv");
   CALL_GetNamedRenderbufferParameteriv(ctx->CurrentServerDispatch, (renderbuffer, pname, params));
}


/* CreateTextures: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_CreateTextures(GLenum target, GLsizei n, GLuint * textures)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "CreateTextures");
   CALL_CreateTextures(ctx->CurrentServerDispatch, (target, n, textures));
}


/* TextureBuffer: marshalled asynchronously */
struct marshal_cmd_TextureBuffer
{
   struct marshal_cmd_base cmd_base;
   GLenum16 internalformat;
   GLuint texture;
   GLuint buffer;
};
uint32_t
_mesa_unmarshal_TextureBuffer(struct gl_context *ctx, const struct marshal_cmd_TextureBuffer *cmd)
{
   GLuint texture = cmd->texture;
   GLenum internalformat = cmd->internalformat;
   GLuint buffer = cmd->buffer;
   CALL_TextureBuffer(ctx->CurrentServerDispatch, (texture, internalformat, buffer));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_TextureBuffer), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_TextureBuffer(GLuint texture, GLenum internalformat, GLuint buffer)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TextureBuffer);
   struct marshal_cmd_TextureBuffer *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TextureBuffer, cmd_size);
   cmd->texture = texture;
   cmd->internalformat = MIN2(internalformat, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->buffer = buffer;
}


/* TextureBufferRange: marshalled asynchronously */
struct marshal_cmd_TextureBufferRange
{
   struct marshal_cmd_base cmd_base;
   GLenum16 internalformat;
   GLuint texture;
   GLuint buffer;
   GLintptr offset;
   GLsizeiptr size;
};
uint32_t
_mesa_unmarshal_TextureBufferRange(struct gl_context *ctx, const struct marshal_cmd_TextureBufferRange *cmd)
{
   GLuint texture = cmd->texture;
   GLenum internalformat = cmd->internalformat;
   GLuint buffer = cmd->buffer;
   GLintptr offset = cmd->offset;
   GLsizeiptr size = cmd->size;
   CALL_TextureBufferRange(ctx->CurrentServerDispatch, (texture, internalformat, buffer, offset, size));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_TextureBufferRange), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_TextureBufferRange(GLuint texture, GLenum internalformat, GLuint buffer, GLintptr offset, GLsizeiptr size)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TextureBufferRange);
   struct marshal_cmd_TextureBufferRange *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TextureBufferRange, cmd_size);
   cmd->texture = texture;
   cmd->internalformat = MIN2(internalformat, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->buffer = buffer;
   cmd->offset = offset;
   cmd->size = size;
}


/* TextureStorage1D: marshalled asynchronously */
struct marshal_cmd_TextureStorage1D
{
   struct marshal_cmd_base cmd_base;
   GLenum16 internalformat;
   GLuint texture;
   GLsizei levels;
   GLsizei width;
};
uint32_t
_mesa_unmarshal_TextureStorage1D(struct gl_context *ctx, const struct marshal_cmd_TextureStorage1D *cmd)
{
   GLuint texture = cmd->texture;
   GLsizei levels = cmd->levels;
   GLenum internalformat = cmd->internalformat;
   GLsizei width = cmd->width;
   CALL_TextureStorage1D(ctx->CurrentServerDispatch, (texture, levels, internalformat, width));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_TextureStorage1D), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_TextureStorage1D(GLuint texture, GLsizei levels, GLenum internalformat, GLsizei width)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TextureStorage1D);
   struct marshal_cmd_TextureStorage1D *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TextureStorage1D, cmd_size);
   cmd->texture = texture;
   cmd->levels = levels;
   cmd->internalformat = MIN2(internalformat, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->width = width;
}


/* TextureStorage2D: marshalled asynchronously */
struct marshal_cmd_TextureStorage2D
{
   struct marshal_cmd_base cmd_base;
   GLenum16 internalformat;
   GLuint texture;
   GLsizei levels;
   GLsizei width;
   GLsizei height;
};
uint32_t
_mesa_unmarshal_TextureStorage2D(struct gl_context *ctx, const struct marshal_cmd_TextureStorage2D *cmd)
{
   GLuint texture = cmd->texture;
   GLsizei levels = cmd->levels;
   GLenum internalformat = cmd->internalformat;
   GLsizei width = cmd->width;
   GLsizei height = cmd->height;
   CALL_TextureStorage2D(ctx->CurrentServerDispatch, (texture, levels, internalformat, width, height));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_TextureStorage2D), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_TextureStorage2D(GLuint texture, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TextureStorage2D);
   struct marshal_cmd_TextureStorage2D *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TextureStorage2D, cmd_size);
   cmd->texture = texture;
   cmd->levels = levels;
   cmd->internalformat = MIN2(internalformat, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->width = width;
   cmd->height = height;
}


/* TextureStorage3D: marshalled asynchronously */
struct marshal_cmd_TextureStorage3D
{
   struct marshal_cmd_base cmd_base;
   GLenum16 internalformat;
   GLuint texture;
   GLsizei levels;
   GLsizei width;
   GLsizei height;
   GLsizei depth;
};
uint32_t
_mesa_unmarshal_TextureStorage3D(struct gl_context *ctx, const struct marshal_cmd_TextureStorage3D *cmd)
{
   GLuint texture = cmd->texture;
   GLsizei levels = cmd->levels;
   GLenum internalformat = cmd->internalformat;
   GLsizei width = cmd->width;
   GLsizei height = cmd->height;
   GLsizei depth = cmd->depth;
   CALL_TextureStorage3D(ctx->CurrentServerDispatch, (texture, levels, internalformat, width, height, depth));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_TextureStorage3D), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_TextureStorage3D(GLuint texture, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TextureStorage3D);
   struct marshal_cmd_TextureStorage3D *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TextureStorage3D, cmd_size);
   cmd->texture = texture;
   cmd->levels = levels;
   cmd->internalformat = MIN2(internalformat, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->width = width;
   cmd->height = height;
   cmd->depth = depth;
}


/* TextureStorage2DMultisample: marshalled asynchronously */
struct marshal_cmd_TextureStorage2DMultisample
{
   struct marshal_cmd_base cmd_base;
   GLboolean fixedsamplelocations;
   GLenum16 internalformat;
   GLuint texture;
   GLsizei samples;
   GLsizei width;
   GLsizei height;
};
uint32_t
_mesa_unmarshal_TextureStorage2DMultisample(struct gl_context *ctx, const struct marshal_cmd_TextureStorage2DMultisample *cmd)
{
   GLuint texture = cmd->texture;
   GLsizei samples = cmd->samples;
   GLenum internalformat = cmd->internalformat;
   GLsizei width = cmd->width;
   GLsizei height = cmd->height;
   GLboolean fixedsamplelocations = cmd->fixedsamplelocations;
   CALL_TextureStorage2DMultisample(ctx->CurrentServerDispatch, (texture, samples, internalformat, width, height, fixedsamplelocations));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_TextureStorage2DMultisample), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_TextureStorage2DMultisample(GLuint texture, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TextureStorage2DMultisample);
   struct marshal_cmd_TextureStorage2DMultisample *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TextureStorage2DMultisample, cmd_size);
   cmd->texture = texture;
   cmd->samples = samples;
   cmd->internalformat = MIN2(internalformat, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->width = width;
   cmd->height = height;
   cmd->fixedsamplelocations = fixedsamplelocations;
}


/* TextureStorage3DMultisample: marshalled asynchronously */
struct marshal_cmd_TextureStorage3DMultisample
{
   struct marshal_cmd_base cmd_base;
   GLboolean fixedsamplelocations;
   GLenum16 internalformat;
   GLuint texture;
   GLsizei samples;
   GLsizei width;
   GLsizei height;
   GLsizei depth;
};
uint32_t
_mesa_unmarshal_TextureStorage3DMultisample(struct gl_context *ctx, const struct marshal_cmd_TextureStorage3DMultisample *cmd)
{
   GLuint texture = cmd->texture;
   GLsizei samples = cmd->samples;
   GLenum internalformat = cmd->internalformat;
   GLsizei width = cmd->width;
   GLsizei height = cmd->height;
   GLsizei depth = cmd->depth;
   GLboolean fixedsamplelocations = cmd->fixedsamplelocations;
   CALL_TextureStorage3DMultisample(ctx->CurrentServerDispatch, (texture, samples, internalformat, width, height, depth, fixedsamplelocations));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_TextureStorage3DMultisample), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_TextureStorage3DMultisample(GLuint texture, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TextureStorage3DMultisample);
   struct marshal_cmd_TextureStorage3DMultisample *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TextureStorage3DMultisample, cmd_size);
   cmd->texture = texture;
   cmd->samples = samples;
   cmd->internalformat = MIN2(internalformat, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->width = width;
   cmd->height = height;
   cmd->depth = depth;
   cmd->fixedsamplelocations = fixedsamplelocations;
}


/* TextureSubImage1D: marshalled asynchronously */
struct marshal_cmd_TextureSubImage1D
{
   struct marshal_cmd_base cmd_base;
   GLenum16 format;
   GLenum16 type;
   GLuint texture;
   GLint level;
   GLint xoffset;
   GLsizei width;
   const GLvoid * pixels;
};
uint32_t
_mesa_unmarshal_TextureSubImage1D(struct gl_context *ctx, const struct marshal_cmd_TextureSubImage1D *cmd)
{
   GLuint texture = cmd->texture;
   GLint level = cmd->level;
   GLint xoffset = cmd->xoffset;
   GLsizei width = cmd->width;
   GLenum format = cmd->format;
   GLenum type = cmd->type;
   const GLvoid * pixels = cmd->pixels;
   CALL_TextureSubImage1D(ctx->CurrentServerDispatch, (texture, level, xoffset, width, format, type, pixels));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_TextureSubImage1D), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_TextureSubImage1D(GLuint texture, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const GLvoid * pixels)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TextureSubImage1D);
   struct marshal_cmd_TextureSubImage1D *cmd;
   if (_mesa_glthread_has_no_unpack_buffer(ctx)) {
      _mesa_glthread_finish_before(ctx, "TextureSubImage1D");
      CALL_TextureSubImage1D(ctx->CurrentServerDispatch, (texture, level, xoffset, width, format, type, pixels));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TextureSubImage1D, cmd_size);
   cmd->texture = texture;
   cmd->level = level;
   cmd->xoffset = xoffset;
   cmd->width = width;
   cmd->format = MIN2(format, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->type = MIN2(type, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->pixels = pixels;
}


/* TextureSubImage2D: marshalled asynchronously */
struct marshal_cmd_TextureSubImage2D
{
   struct marshal_cmd_base cmd_base;
   GLenum16 format;
   GLenum16 type;
   GLuint texture;
   GLint level;
   GLint xoffset;
   GLint yoffset;
   GLsizei width;
   GLsizei height;
   const GLvoid * pixels;
};
uint32_t
_mesa_unmarshal_TextureSubImage2D(struct gl_context *ctx, const struct marshal_cmd_TextureSubImage2D *cmd)
{
   GLuint texture = cmd->texture;
   GLint level = cmd->level;
   GLint xoffset = cmd->xoffset;
   GLint yoffset = cmd->yoffset;
   GLsizei width = cmd->width;
   GLsizei height = cmd->height;
   GLenum format = cmd->format;
   GLenum type = cmd->type;
   const GLvoid * pixels = cmd->pixels;
   CALL_TextureSubImage2D(ctx->CurrentServerDispatch, (texture, level, xoffset, yoffset, width, height, format, type, pixels));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_TextureSubImage2D), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_TextureSubImage2D(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid * pixels)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TextureSubImage2D);
   struct marshal_cmd_TextureSubImage2D *cmd;
   if (_mesa_glthread_has_no_unpack_buffer(ctx)) {
      _mesa_glthread_finish_before(ctx, "TextureSubImage2D");
      CALL_TextureSubImage2D(ctx->CurrentServerDispatch, (texture, level, xoffset, yoffset, width, height, format, type, pixels));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TextureSubImage2D, cmd_size);
   cmd->texture = texture;
   cmd->level = level;
   cmd->xoffset = xoffset;
   cmd->yoffset = yoffset;
   cmd->width = width;
   cmd->height = height;
   cmd->format = MIN2(format, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->type = MIN2(type, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->pixels = pixels;
}


/* TextureSubImage3D: marshalled asynchronously */
struct marshal_cmd_TextureSubImage3D
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
   const GLvoid * pixels;
};
uint32_t
_mesa_unmarshal_TextureSubImage3D(struct gl_context *ctx, const struct marshal_cmd_TextureSubImage3D *cmd)
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
   const GLvoid * pixels = cmd->pixels;
   CALL_TextureSubImage3D(ctx->CurrentServerDispatch, (texture, level, xoffset, yoffset, zoffset, width, height, depth, format, type, pixels));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_TextureSubImage3D), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_TextureSubImage3D(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const GLvoid * pixels)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TextureSubImage3D);
   struct marshal_cmd_TextureSubImage3D *cmd;
   if (_mesa_glthread_has_no_unpack_buffer(ctx)) {
      _mesa_glthread_finish_before(ctx, "TextureSubImage3D");
      CALL_TextureSubImage3D(ctx->CurrentServerDispatch, (texture, level, xoffset, yoffset, zoffset, width, height, depth, format, type, pixels));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TextureSubImage3D, cmd_size);
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
   cmd->pixels = pixels;
}


/* CompressedTextureSubImage1D: marshalled asynchronously */
struct marshal_cmd_CompressedTextureSubImage1D
{
   struct marshal_cmd_base cmd_base;
   GLenum16 format;
   GLuint texture;
   GLint level;
   GLint xoffset;
   GLsizei width;
   GLsizei imageSize;
   const GLvoid * data;
};
uint32_t
_mesa_unmarshal_CompressedTextureSubImage1D(struct gl_context *ctx, const struct marshal_cmd_CompressedTextureSubImage1D *cmd)
{
   GLuint texture = cmd->texture;
   GLint level = cmd->level;
   GLint xoffset = cmd->xoffset;
   GLsizei width = cmd->width;
   GLenum format = cmd->format;
   GLsizei imageSize = cmd->imageSize;
   const GLvoid * data = cmd->data;
   CALL_CompressedTextureSubImage1D(ctx->CurrentServerDispatch, (texture, level, xoffset, width, format, imageSize, data));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_CompressedTextureSubImage1D), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_CompressedTextureSubImage1D(GLuint texture, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const GLvoid * data)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_CompressedTextureSubImage1D);
   struct marshal_cmd_CompressedTextureSubImage1D *cmd;
   if (_mesa_glthread_has_no_unpack_buffer(ctx)) {
      _mesa_glthread_finish_before(ctx, "CompressedTextureSubImage1D");
      CALL_CompressedTextureSubImage1D(ctx->CurrentServerDispatch, (texture, level, xoffset, width, format, imageSize, data));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_CompressedTextureSubImage1D, cmd_size);
   cmd->texture = texture;
   cmd->level = level;
   cmd->xoffset = xoffset;
   cmd->width = width;
   cmd->format = MIN2(format, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->imageSize = imageSize;
   cmd->data = data;
}


/* CompressedTextureSubImage2D: marshalled asynchronously */
struct marshal_cmd_CompressedTextureSubImage2D
{
   struct marshal_cmd_base cmd_base;
   GLenum16 format;
   GLuint texture;
   GLint level;
   GLint xoffset;
   GLint yoffset;
   GLsizei width;
   GLsizei height;
   GLsizei imageSize;
   const GLvoid * data;
};
uint32_t
_mesa_unmarshal_CompressedTextureSubImage2D(struct gl_context *ctx, const struct marshal_cmd_CompressedTextureSubImage2D *cmd)
{
   GLuint texture = cmd->texture;
   GLint level = cmd->level;
   GLint xoffset = cmd->xoffset;
   GLint yoffset = cmd->yoffset;
   GLsizei width = cmd->width;
   GLsizei height = cmd->height;
   GLenum format = cmd->format;
   GLsizei imageSize = cmd->imageSize;
   const GLvoid * data = cmd->data;
   CALL_CompressedTextureSubImage2D(ctx->CurrentServerDispatch, (texture, level, xoffset, yoffset, width, height, format, imageSize, data));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_CompressedTextureSubImage2D), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_CompressedTextureSubImage2D(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const GLvoid * data)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_CompressedTextureSubImage2D);
   struct marshal_cmd_CompressedTextureSubImage2D *cmd;
   if (_mesa_glthread_has_no_unpack_buffer(ctx)) {
      _mesa_glthread_finish_before(ctx, "CompressedTextureSubImage2D");
      CALL_CompressedTextureSubImage2D(ctx->CurrentServerDispatch, (texture, level, xoffset, yoffset, width, height, format, imageSize, data));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_CompressedTextureSubImage2D, cmd_size);
   cmd->texture = texture;
   cmd->level = level;
   cmd->xoffset = xoffset;
   cmd->yoffset = yoffset;
   cmd->width = width;
   cmd->height = height;
   cmd->format = MIN2(format, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->imageSize = imageSize;
   cmd->data = data;
}


/* CompressedTextureSubImage3D: marshalled asynchronously */
struct marshal_cmd_CompressedTextureSubImage3D
{
   struct marshal_cmd_base cmd_base;
   GLenum16 format;
   GLuint texture;
   GLint level;
   GLint xoffset;
   GLint yoffset;
   GLint zoffset;
   GLsizei width;
   GLsizei height;
   GLsizei depth;
   GLsizei imageSize;
   const GLvoid * data;
};
uint32_t
_mesa_unmarshal_CompressedTextureSubImage3D(struct gl_context *ctx, const struct marshal_cmd_CompressedTextureSubImage3D *cmd)
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
   GLsizei imageSize = cmd->imageSize;
   const GLvoid * data = cmd->data;
   CALL_CompressedTextureSubImage3D(ctx->CurrentServerDispatch, (texture, level, xoffset, yoffset, zoffset, width, height, depth, format, imageSize, data));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_CompressedTextureSubImage3D), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_CompressedTextureSubImage3D(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const GLvoid * data)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_CompressedTextureSubImage3D);
   struct marshal_cmd_CompressedTextureSubImage3D *cmd;
   if (_mesa_glthread_has_no_unpack_buffer(ctx)) {
      _mesa_glthread_finish_before(ctx, "CompressedTextureSubImage3D");
      CALL_CompressedTextureSubImage3D(ctx->CurrentServerDispatch, (texture, level, xoffset, yoffset, zoffset, width, height, depth, format, imageSize, data));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_CompressedTextureSubImage3D, cmd_size);
   cmd->texture = texture;
   cmd->level = level;
   cmd->xoffset = xoffset;
   cmd->yoffset = yoffset;
   cmd->zoffset = zoffset;
   cmd->width = width;
   cmd->height = height;
   cmd->depth = depth;
   cmd->format = MIN2(format, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->imageSize = imageSize;
   cmd->data = data;
}


/* CopyTextureSubImage1D: marshalled asynchronously */
struct marshal_cmd_CopyTextureSubImage1D
{
   struct marshal_cmd_base cmd_base;
   GLuint texture;
   GLint level;
   GLint xoffset;
   GLint x;
   GLint y;
   GLsizei width;
};
uint32_t
_mesa_unmarshal_CopyTextureSubImage1D(struct gl_context *ctx, const struct marshal_cmd_CopyTextureSubImage1D *cmd)
{
   GLuint texture = cmd->texture;
   GLint level = cmd->level;
   GLint xoffset = cmd->xoffset;
   GLint x = cmd->x;
   GLint y = cmd->y;
   GLsizei width = cmd->width;
   CALL_CopyTextureSubImage1D(ctx->CurrentServerDispatch, (texture, level, xoffset, x, y, width));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_CopyTextureSubImage1D), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_CopyTextureSubImage1D(GLuint texture, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_CopyTextureSubImage1D);
   struct marshal_cmd_CopyTextureSubImage1D *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_CopyTextureSubImage1D, cmd_size);
   cmd->texture = texture;
   cmd->level = level;
   cmd->xoffset = xoffset;
   cmd->x = x;
   cmd->y = y;
   cmd->width = width;
}


/* CopyTextureSubImage2D: marshalled asynchronously */
struct marshal_cmd_CopyTextureSubImage2D
{
   struct marshal_cmd_base cmd_base;
   GLuint texture;
   GLint level;
   GLint xoffset;
   GLint yoffset;
   GLint x;
   GLint y;
   GLsizei width;
   GLsizei height;
};
uint32_t
_mesa_unmarshal_CopyTextureSubImage2D(struct gl_context *ctx, const struct marshal_cmd_CopyTextureSubImage2D *cmd)
{
   GLuint texture = cmd->texture;
   GLint level = cmd->level;
   GLint xoffset = cmd->xoffset;
   GLint yoffset = cmd->yoffset;
   GLint x = cmd->x;
   GLint y = cmd->y;
   GLsizei width = cmd->width;
   GLsizei height = cmd->height;
   CALL_CopyTextureSubImage2D(ctx->CurrentServerDispatch, (texture, level, xoffset, yoffset, x, y, width, height));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_CopyTextureSubImage2D), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_CopyTextureSubImage2D(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_CopyTextureSubImage2D);
   struct marshal_cmd_CopyTextureSubImage2D *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_CopyTextureSubImage2D, cmd_size);
   cmd->texture = texture;
   cmd->level = level;
   cmd->xoffset = xoffset;
   cmd->yoffset = yoffset;
   cmd->x = x;
   cmd->y = y;
   cmd->width = width;
   cmd->height = height;
}


/* CopyTextureSubImage3D: marshalled asynchronously */
struct marshal_cmd_CopyTextureSubImage3D
{
   struct marshal_cmd_base cmd_base;
   GLuint texture;
   GLint level;
   GLint xoffset;
   GLint yoffset;
   GLint zoffset;
   GLint x;
   GLint y;
   GLsizei width;
   GLsizei height;
};
uint32_t
_mesa_unmarshal_CopyTextureSubImage3D(struct gl_context *ctx, const struct marshal_cmd_CopyTextureSubImage3D *cmd)
{
   GLuint texture = cmd->texture;
   GLint level = cmd->level;
   GLint xoffset = cmd->xoffset;
   GLint yoffset = cmd->yoffset;
   GLint zoffset = cmd->zoffset;
   GLint x = cmd->x;
   GLint y = cmd->y;
   GLsizei width = cmd->width;
   GLsizei height = cmd->height;
   CALL_CopyTextureSubImage3D(ctx->CurrentServerDispatch, (texture, level, xoffset, yoffset, zoffset, x, y, width, height));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_CopyTextureSubImage3D), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_CopyTextureSubImage3D(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_CopyTextureSubImage3D);
   struct marshal_cmd_CopyTextureSubImage3D *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_CopyTextureSubImage3D, cmd_size);
   cmd->texture = texture;
   cmd->level = level;
   cmd->xoffset = xoffset;
   cmd->yoffset = yoffset;
   cmd->zoffset = zoffset;
   cmd->x = x;
   cmd->y = y;
   cmd->width = width;
   cmd->height = height;
}


/* TextureParameterf: marshalled asynchronously */
struct marshal_cmd_TextureParameterf
{
   struct marshal_cmd_base cmd_base;
   GLenum16 pname;
   GLuint texture;
   GLfloat param;
};
uint32_t
_mesa_unmarshal_TextureParameterf(struct gl_context *ctx, const struct marshal_cmd_TextureParameterf *cmd)
{
   GLuint texture = cmd->texture;
   GLenum pname = cmd->pname;
   GLfloat param = cmd->param;
   CALL_TextureParameterf(ctx->CurrentServerDispatch, (texture, pname, param));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_TextureParameterf), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_TextureParameterf(GLuint texture, GLenum pname, GLfloat param)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TextureParameterf);
   struct marshal_cmd_TextureParameterf *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TextureParameterf, cmd_size);
   cmd->texture = texture;
   cmd->pname = MIN2(pname, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->param = param;
}


/* TextureParameterfv: marshalled asynchronously */
struct marshal_cmd_TextureParameterfv
{
   struct marshal_cmd_base cmd_base;
   GLenum16 pname;
   GLuint texture;
   /* Next safe_mul(_mesa_tex_param_enum_to_count(pname), 1 * sizeof(GLfloat)) bytes are GLfloat param[_mesa_tex_param_enum_to_count(pname)] */
};
uint32_t
_mesa_unmarshal_TextureParameterfv(struct gl_context *ctx, const struct marshal_cmd_TextureParameterfv *cmd)
{
   GLuint texture = cmd->texture;
   GLenum pname = cmd->pname;
   GLfloat *param;
   const char *variable_data = (const char *) (cmd + 1);
   param = (GLfloat *) variable_data;
   CALL_TextureParameterfv(ctx->CurrentServerDispatch, (texture, pname, param));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_TextureParameterfv(GLuint texture, GLenum pname, const GLfloat * param)
{
   GET_CURRENT_CONTEXT(ctx);
   int param_size = safe_mul(_mesa_tex_param_enum_to_count(pname), 1 * sizeof(GLfloat));
   int cmd_size = sizeof(struct marshal_cmd_TextureParameterfv) + param_size;
   struct marshal_cmd_TextureParameterfv *cmd;
   if (unlikely(param_size < 0 || (param_size > 0 && !param) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "TextureParameterfv");
      CALL_TextureParameterfv(ctx->CurrentServerDispatch, (texture, pname, param));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TextureParameterfv, cmd_size);
   cmd->texture = texture;
   cmd->pname = MIN2(pname, 0xffff); /* clamped to 0xffff (invalid enum) */
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, param, param_size);
}


/* TextureParameteri: marshalled asynchronously */
struct marshal_cmd_TextureParameteri
{
   struct marshal_cmd_base cmd_base;
   GLenum16 pname;
   GLuint texture;
   GLint param;
};
uint32_t
_mesa_unmarshal_TextureParameteri(struct gl_context *ctx, const struct marshal_cmd_TextureParameteri *cmd)
{
   GLuint texture = cmd->texture;
   GLenum pname = cmd->pname;
   GLint param = cmd->param;
   CALL_TextureParameteri(ctx->CurrentServerDispatch, (texture, pname, param));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_TextureParameteri), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_TextureParameteri(GLuint texture, GLenum pname, GLint param)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TextureParameteri);
   struct marshal_cmd_TextureParameteri *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TextureParameteri, cmd_size);
   cmd->texture = texture;
   cmd->pname = MIN2(pname, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->param = param;
}


/* TextureParameterIiv: marshalled asynchronously */
struct marshal_cmd_TextureParameterIiv
{
   struct marshal_cmd_base cmd_base;
   GLenum16 pname;
   GLuint texture;
   /* Next safe_mul(_mesa_tex_param_enum_to_count(pname), 1 * sizeof(GLint)) bytes are GLint params[_mesa_tex_param_enum_to_count(pname)] */
};
uint32_t
_mesa_unmarshal_TextureParameterIiv(struct gl_context *ctx, const struct marshal_cmd_TextureParameterIiv *cmd)
{
   GLuint texture = cmd->texture;
   GLenum pname = cmd->pname;
   GLint *params;
   const char *variable_data = (const char *) (cmd + 1);
   params = (GLint *) variable_data;
   CALL_TextureParameterIiv(ctx->CurrentServerDispatch, (texture, pname, params));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_TextureParameterIiv(GLuint texture, GLenum pname, const GLint * params)
{
   GET_CURRENT_CONTEXT(ctx);
   int params_size = safe_mul(_mesa_tex_param_enum_to_count(pname), 1 * sizeof(GLint));
   int cmd_size = sizeof(struct marshal_cmd_TextureParameterIiv) + params_size;
   struct marshal_cmd_TextureParameterIiv *cmd;
   if (unlikely(params_size < 0 || (params_size > 0 && !params) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "TextureParameterIiv");
      CALL_TextureParameterIiv(ctx->CurrentServerDispatch, (texture, pname, params));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TextureParameterIiv, cmd_size);
   cmd->texture = texture;
   cmd->pname = MIN2(pname, 0xffff); /* clamped to 0xffff (invalid enum) */
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, params, params_size);
}


/* TextureParameterIuiv: marshalled asynchronously */
struct marshal_cmd_TextureParameterIuiv
{
   struct marshal_cmd_base cmd_base;
   GLenum16 pname;
   GLuint texture;
   /* Next safe_mul(_mesa_tex_param_enum_to_count(pname), 1 * sizeof(GLuint)) bytes are GLuint params[_mesa_tex_param_enum_to_count(pname)] */
};
uint32_t
_mesa_unmarshal_TextureParameterIuiv(struct gl_context *ctx, const struct marshal_cmd_TextureParameterIuiv *cmd)
{
   GLuint texture = cmd->texture;
   GLenum pname = cmd->pname;
   GLuint *params;
   const char *variable_data = (const char *) (cmd + 1);
   params = (GLuint *) variable_data;
   CALL_TextureParameterIuiv(ctx->CurrentServerDispatch, (texture, pname, params));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_TextureParameterIuiv(GLuint texture, GLenum pname, const GLuint * params)
{
   GET_CURRENT_CONTEXT(ctx);
   int params_size = safe_mul(_mesa_tex_param_enum_to_count(pname), 1 * sizeof(GLuint));
   int cmd_size = sizeof(struct marshal_cmd_TextureParameterIuiv) + params_size;
   struct marshal_cmd_TextureParameterIuiv *cmd;
   if (unlikely(params_size < 0 || (params_size > 0 && !params) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "TextureParameterIuiv");
      CALL_TextureParameterIuiv(ctx->CurrentServerDispatch, (texture, pname, params));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TextureParameterIuiv, cmd_size);
   cmd->texture = texture;
   cmd->pname = MIN2(pname, 0xffff); /* clamped to 0xffff (invalid enum) */
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, params, params_size);
}


/* TextureParameteriv: marshalled asynchronously */
struct marshal_cmd_TextureParameteriv
{
   struct marshal_cmd_base cmd_base;
   GLenum16 pname;
   GLuint texture;
   /* Next safe_mul(_mesa_tex_param_enum_to_count(pname), 1 * sizeof(GLint)) bytes are GLint param[_mesa_tex_param_enum_to_count(pname)] */
};
uint32_t
_mesa_unmarshal_TextureParameteriv(struct gl_context *ctx, const struct marshal_cmd_TextureParameteriv *cmd)
{
   GLuint texture = cmd->texture;
   GLenum pname = cmd->pname;
   GLint *param;
   const char *variable_data = (const char *) (cmd + 1);
   param = (GLint *) variable_data;
   CALL_TextureParameteriv(ctx->CurrentServerDispatch, (texture, pname, param));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_TextureParameteriv(GLuint texture, GLenum pname, const GLint * param)
{
   GET_CURRENT_CONTEXT(ctx);
   int param_size = safe_mul(_mesa_tex_param_enum_to_count(pname), 1 * sizeof(GLint));
   int cmd_size = sizeof(struct marshal_cmd_TextureParameteriv) + param_size;
   struct marshal_cmd_TextureParameteriv *cmd;
   if (unlikely(param_size < 0 || (param_size > 0 && !param) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "TextureParameteriv");
      CALL_TextureParameteriv(ctx->CurrentServerDispatch, (texture, pname, param));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TextureParameteriv, cmd_size);
   cmd->texture = texture;
   cmd->pname = MIN2(pname, 0xffff); /* clamped to 0xffff (invalid enum) */
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, param, param_size);
}


/* GenerateTextureMipmap: marshalled asynchronously */
struct marshal_cmd_GenerateTextureMipmap
{
   struct marshal_cmd_base cmd_base;
   GLuint texture;
};
uint32_t
_mesa_unmarshal_GenerateTextureMipmap(struct gl_context *ctx, const struct marshal_cmd_GenerateTextureMipmap *cmd)
{
   GLuint texture = cmd->texture;
   CALL_GenerateTextureMipmap(ctx->CurrentServerDispatch, (texture));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_GenerateTextureMipmap), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_GenerateTextureMipmap(GLuint texture)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_GenerateTextureMipmap);
   struct marshal_cmd_GenerateTextureMipmap *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_GenerateTextureMipmap, cmd_size);
   cmd->texture = texture;
}


/* BindTextureUnit: marshalled asynchronously */
struct marshal_cmd_BindTextureUnit
{
   struct marshal_cmd_base cmd_base;
   GLuint unit;
   GLuint texture;
};
uint32_t
_mesa_unmarshal_BindTextureUnit(struct gl_context *ctx, const struct marshal_cmd_BindTextureUnit *cmd)
{
   GLuint unit = cmd->unit;
   GLuint texture = cmd->texture;
   CALL_BindTextureUnit(ctx->CurrentServerDispatch, (unit, texture));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_BindTextureUnit), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_BindTextureUnit(GLuint unit, GLuint texture)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_BindTextureUnit);
   struct marshal_cmd_BindTextureUnit *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_BindTextureUnit, cmd_size);
   cmd->unit = unit;
   cmd->texture = texture;
}


/* GetTextureImage: marshalled asynchronously */
struct marshal_cmd_GetTextureImage
{
   struct marshal_cmd_base cmd_base;
   GLenum16 format;
   GLenum16 type;
   GLuint texture;
   GLint level;
   GLsizei bufSize;
   GLvoid * pixels;
};
uint32_t
_mesa_unmarshal_GetTextureImage(struct gl_context *ctx, const struct marshal_cmd_GetTextureImage *cmd)
{
   GLuint texture = cmd->texture;
   GLint level = cmd->level;
   GLenum format = cmd->format;
   GLenum type = cmd->type;
   GLsizei bufSize = cmd->bufSize;
   GLvoid * pixels = cmd->pixels;
   CALL_GetTextureImage(ctx->CurrentServerDispatch, (texture, level, format, type, bufSize, pixels));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_GetTextureImage), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_GetTextureImage(GLuint texture, GLint level, GLenum format, GLenum type, GLsizei bufSize, GLvoid * pixels)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_GetTextureImage);
   struct marshal_cmd_GetTextureImage *cmd;
   if (_mesa_glthread_has_no_pack_buffer(ctx)) {
      _mesa_glthread_finish_before(ctx, "GetTextureImage");
      CALL_GetTextureImage(ctx->CurrentServerDispatch, (texture, level, format, type, bufSize, pixels));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_GetTextureImage, cmd_size);
   cmd->texture = texture;
   cmd->level = level;
   cmd->format = MIN2(format, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->type = MIN2(type, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->bufSize = bufSize;
   cmd->pixels = pixels;
}


void
_mesa_glthread_init_dispatch4(struct gl_context *ctx, struct _glapi_table *table)
{
   if (_mesa_is_desktop_gl(ctx)) {
      SET_BindBuffersBase(table, _mesa_marshal_BindBuffersBase);
      SET_BindBuffersRange(table, _mesa_marshal_BindBuffersRange);
      SET_BindImageTextures(table, _mesa_marshal_BindImageTextures);
      SET_BindSamplers(table, _mesa_marshal_BindSamplers);
      SET_BindTextures(table, _mesa_marshal_BindTextures);
      SET_BindVertexBuffers(table, _mesa_marshal_BindVertexBuffers);
      SET_ClearBufferData(table, _mesa_marshal_ClearBufferData);
      SET_ClearBufferSubData(table, _mesa_marshal_ClearBufferSubData);
      SET_ClearNamedBufferDataEXT(table, _mesa_marshal_ClearNamedBufferDataEXT);
      SET_ClearNamedBufferSubDataEXT(table, _mesa_marshal_ClearNamedBufferSubDataEXT);
      SET_DispatchComputeGroupSizeARB(table, _mesa_marshal_DispatchComputeGroupSizeARB);
      SET_DrawTransformFeedbackInstanced(table, _mesa_marshal_DrawTransformFeedbackInstanced);
      SET_DrawTransformFeedbackStreamInstanced(table, _mesa_marshal_DrawTransformFeedbackStreamInstanced);
      SET_GetActiveAtomicCounterBufferiv(table, _mesa_marshal_GetActiveAtomicCounterBufferiv);
      SET_GetImageHandleARB(table, _mesa_marshal_GetImageHandleARB);
      SET_GetNamedFramebufferParameterivEXT(table, _mesa_marshal_GetNamedFramebufferParameterivEXT);
      SET_GetTextureHandleARB(table, _mesa_marshal_GetTextureHandleARB);
      SET_GetTextureSamplerHandleARB(table, _mesa_marshal_GetTextureSamplerHandleARB);
      SET_GetnCompressedTexImageARB(table, _mesa_marshal_GetnCompressedTexImageARB);
      SET_GetnPolygonStippleARB(table, _mesa_marshal_GetnPolygonStippleARB);
      SET_GetnTexImageARB(table, _mesa_marshal_GetnTexImageARB);
      SET_GetnUniformdvARB(table, _mesa_marshal_GetnUniformdvARB);
      SET_InvalidateBufferData(table, _mesa_marshal_InvalidateBufferData);
      SET_InvalidateBufferSubData(table, _mesa_marshal_InvalidateBufferSubData);
      SET_InvalidateTexImage(table, _mesa_marshal_InvalidateTexImage);
      SET_InvalidateTexSubImage(table, _mesa_marshal_InvalidateTexSubImage);
      SET_IsImageHandleResidentARB(table, _mesa_marshal_IsImageHandleResidentARB);
      SET_IsTextureHandleResidentARB(table, _mesa_marshal_IsTextureHandleResidentARB);
      SET_MakeImageHandleNonResidentARB(table, _mesa_marshal_MakeImageHandleNonResidentARB);
      SET_MakeImageHandleResidentARB(table, _mesa_marshal_MakeImageHandleResidentARB);
      SET_MakeTextureHandleNonResidentARB(table, _mesa_marshal_MakeTextureHandleNonResidentARB);
      SET_MakeTextureHandleResidentARB(table, _mesa_marshal_MakeTextureHandleResidentARB);
      SET_MultiDrawArraysIndirectCountARB(table, _mesa_marshal_MultiDrawArraysIndirectCountARB);
      SET_MultiDrawElementsIndirectCountARB(table, _mesa_marshal_MultiDrawElementsIndirectCountARB);
      SET_NamedBufferStorageEXT(table, _mesa_marshal_NamedBufferStorageEXT);
      SET_NamedFramebufferParameteriEXT(table, _mesa_marshal_NamedFramebufferParameteriEXT);
      SET_ProgramUniformHandleui64ARB(table, _mesa_marshal_ProgramUniformHandleui64ARB);
      SET_ProgramUniformHandleui64vARB(table, _mesa_marshal_ProgramUniformHandleui64vARB);
      SET_ShaderStorageBlockBinding(table, _mesa_marshal_ShaderStorageBlockBinding);
      SET_TexPageCommitmentARB(table, _mesa_marshal_TexPageCommitmentARB);
      SET_TexStorage1D(table, _mesa_marshal_TexStorage1D);
      SET_TextureBufferRangeEXT(table, _mesa_marshal_TextureBufferRangeEXT);
      SET_TexturePageCommitmentEXT(table, _mesa_marshal_TexturePageCommitmentEXT);
      SET_TextureStorage1DEXT(table, _mesa_marshal_TextureStorage1DEXT);
      SET_TextureStorage2DEXT(table, _mesa_marshal_TextureStorage2DEXT);
      SET_TextureStorage2DMultisampleEXT(table, _mesa_marshal_TextureStorage2DMultisampleEXT);
      SET_TextureStorage3DEXT(table, _mesa_marshal_TextureStorage3DEXT);
      SET_TextureStorage3DMultisampleEXT(table, _mesa_marshal_TextureStorage3DMultisampleEXT);
      SET_UniformHandleui64ARB(table, _mesa_marshal_UniformHandleui64ARB);
      SET_UniformHandleui64vARB(table, _mesa_marshal_UniformHandleui64vARB);
      SET_UnmapNamedBufferEXT(table, _mesa_marshal_UnmapNamedBufferEXT);
      SET_VertexArrayBindVertexBufferEXT(table, _mesa_marshal_VertexArrayBindVertexBufferEXT);
      SET_VertexArrayVertexAttribBindingEXT(table, _mesa_marshal_VertexArrayVertexAttribBindingEXT);
      SET_VertexArrayVertexAttribFormatEXT(table, _mesa_marshal_VertexArrayVertexAttribFormatEXT);
      SET_VertexArrayVertexAttribIFormatEXT(table, _mesa_marshal_VertexArrayVertexAttribIFormatEXT);
      SET_VertexArrayVertexAttribLFormatEXT(table, _mesa_marshal_VertexArrayVertexAttribLFormatEXT);
      SET_VertexArrayVertexAttribLOffsetEXT(table, _mesa_marshal_VertexArrayVertexAttribLOffsetEXT);
      SET_VertexArrayVertexBindingDivisorEXT(table, _mesa_marshal_VertexArrayVertexBindingDivisorEXT);
      SET_VertexAttribL1ui64ARB(table, _mesa_marshal_VertexAttribL1ui64ARB);
      SET_VertexAttribL1ui64vARB(table, _mesa_marshal_VertexAttribL1ui64vARB);
      SET_VertexAttribLFormat(table, _mesa_marshal_VertexAttribLFormat);
   }
   if (_mesa_is_desktop_gl(ctx) || (ctx->API == API_OPENGLES2 && ctx->Version >= 30)) {
      SET_CopyImageSubData(table, _mesa_marshal_CopyImageSubData);
      SET_GetInternalformati64v(table, _mesa_marshal_GetInternalformati64v);
      SET_GetInternalformativ(table, _mesa_marshal_GetInternalformativ);
      SET_InvalidateFramebuffer(table, _mesa_marshal_InvalidateFramebuffer);
      SET_InvalidateSubFramebuffer(table, _mesa_marshal_InvalidateSubFramebuffer);
      SET_TexStorage2D(table, _mesa_marshal_TexStorage2D);
      SET_TexStorage3D(table, _mesa_marshal_TexStorage3D);
   }
   if (_mesa_is_desktop_gl(ctx) || (ctx->API == API_OPENGLES2 && ctx->Version >= 31)) {
      SET_BindImageTexture(table, _mesa_marshal_BindImageTexture);
      SET_BindVertexBuffer(table, _mesa_marshal_BindVertexBuffer);
      SET_BufferStorage(table, _mesa_marshal_BufferStorage);
      SET_ClearTexImage(table, _mesa_marshal_ClearTexImage);
      SET_ClearTexSubImage(table, _mesa_marshal_ClearTexSubImage);
      SET_DispatchCompute(table, _mesa_marshal_DispatchCompute);
      SET_DispatchComputeIndirect(table, _mesa_marshal_DispatchComputeIndirect);
      SET_GetProgramInterfaceiv(table, _mesa_marshal_GetProgramInterfaceiv);
      SET_GetProgramResourceIndex(table, _mesa_marshal_GetProgramResourceIndex);
      SET_GetProgramResourceLocation(table, _mesa_marshal_GetProgramResourceLocation);
      SET_GetProgramResourceLocationIndex(table, _mesa_marshal_GetProgramResourceLocationIndex);
      SET_GetProgramResourceName(table, _mesa_marshal_GetProgramResourceName);
      SET_GetProgramResourceiv(table, _mesa_marshal_GetProgramResourceiv);
      SET_MemoryBarrier(table, _mesa_marshal_MemoryBarrier);
      SET_TexStorage2DMultisample(table, _mesa_marshal_TexStorage2DMultisample);
      SET_TexStorage3DMultisample(table, _mesa_marshal_TexStorage3DMultisample);
      SET_TextureView(table, _mesa_marshal_TextureView);
      SET_VertexAttribBinding(table, _mesa_marshal_VertexAttribBinding);
      SET_VertexAttribFormat(table, _mesa_marshal_VertexAttribFormat);
      SET_VertexAttribIFormat(table, _mesa_marshal_VertexAttribIFormat);
      SET_VertexBindingDivisor(table, _mesa_marshal_VertexBindingDivisor);
   }
   if (_mesa_is_desktop_gl(ctx) || ctx->API == API_OPENGLES || ctx->API == API_OPENGLES2) {
      SET_ClearDepthf(table, _mesa_marshal_ClearDepthf);
      SET_DebugMessageCallback(table, _mesa_marshal_DebugMessageCallback);
      SET_DebugMessageControl(table, _mesa_marshal_DebugMessageControl);
      SET_DebugMessageInsert(table, _mesa_marshal_DebugMessageInsert);
      SET_DepthRangef(table, _mesa_marshal_DepthRangef);
      SET_GetDebugMessageLog(table, _mesa_marshal_GetDebugMessageLog);
      SET_GetObjectLabel(table, _mesa_marshal_GetObjectLabel);
      SET_GetObjectPtrLabel(table, _mesa_marshal_GetObjectPtrLabel);
      SET_ObjectLabel(table, _mesa_marshal_ObjectLabel);
      SET_ObjectPtrLabel(table, _mesa_marshal_ObjectPtrLabel);
      SET_PopDebugGroup(table, _mesa_marshal_PopDebugGroup);
      SET_PushDebugGroup(table, _mesa_marshal_PushDebugGroup);
   }
   if (_mesa_is_desktop_gl(ctx) || ctx->API == API_OPENGLES2) {
      SET_ClipControl(table, _mesa_marshal_ClipControl);
      SET_GetGraphicsResetStatusARB(table, _mesa_marshal_GetGraphicsResetStatusARB);
      SET_GetProgramBinary(table, _mesa_marshal_GetProgramBinary);
      SET_GetShaderPrecisionFormat(table, _mesa_marshal_GetShaderPrecisionFormat);
      SET_GetnUniformfvARB(table, _mesa_marshal_GetnUniformfvARB);
      SET_GetnUniformivARB(table, _mesa_marshal_GetnUniformivARB);
      SET_GetnUniformuivARB(table, _mesa_marshal_GetnUniformuivARB);
      SET_ProgramBinary(table, _mesa_marshal_ProgramBinary);
      SET_ProgramParameteri(table, _mesa_marshal_ProgramParameteri);
      SET_ReadnPixelsARB(table, _mesa_marshal_ReadnPixelsARB);
      SET_ReleaseShaderCompiler(table, _mesa_marshal_ReleaseShaderCompiler);
      SET_ShaderBinary(table, _mesa_marshal_ShaderBinary);
   }
   if (ctx->API == API_OPENGL_COMPAT) {
      SET_GetnMapdvARB(table, _mesa_marshal_GetnMapdvARB);
      SET_GetnMapfvARB(table, _mesa_marshal_GetnMapfvARB);
      SET_GetnMapivARB(table, _mesa_marshal_GetnMapivARB);
      SET_GetnPixelMapfvARB(table, _mesa_marshal_GetnPixelMapfvARB);
      SET_GetnPixelMapuivARB(table, _mesa_marshal_GetnPixelMapuivARB);
      SET_GetnPixelMapusvARB(table, _mesa_marshal_GetnPixelMapusvARB);
   }
   if (ctx->API == API_OPENGL_COMPAT || ctx->API == API_OPENGL_CORE) {
      SET_BindTextureUnit(table, _mesa_marshal_BindTextureUnit);
      SET_BlitNamedFramebuffer(table, _mesa_marshal_BlitNamedFramebuffer);
      SET_CheckNamedFramebufferStatus(table, _mesa_marshal_CheckNamedFramebufferStatus);
      SET_ClearNamedBufferData(table, _mesa_marshal_ClearNamedBufferData);
      SET_ClearNamedBufferSubData(table, _mesa_marshal_ClearNamedBufferSubData);
      SET_ClearNamedFramebufferfi(table, _mesa_marshal_ClearNamedFramebufferfi);
      SET_ClearNamedFramebufferfv(table, _mesa_marshal_ClearNamedFramebufferfv);
      SET_ClearNamedFramebufferiv(table, _mesa_marshal_ClearNamedFramebufferiv);
      SET_ClearNamedFramebufferuiv(table, _mesa_marshal_ClearNamedFramebufferuiv);
      SET_CompressedTextureSubImage1D(table, _mesa_marshal_CompressedTextureSubImage1D);
      SET_CompressedTextureSubImage2D(table, _mesa_marshal_CompressedTextureSubImage2D);
      SET_CompressedTextureSubImage3D(table, _mesa_marshal_CompressedTextureSubImage3D);
      SET_CopyNamedBufferSubData(table, _mesa_marshal_CopyNamedBufferSubData);
      SET_CopyTextureSubImage1D(table, _mesa_marshal_CopyTextureSubImage1D);
      SET_CopyTextureSubImage2D(table, _mesa_marshal_CopyTextureSubImage2D);
      SET_CopyTextureSubImage3D(table, _mesa_marshal_CopyTextureSubImage3D);
      SET_CreateBuffers(table, _mesa_marshal_CreateBuffers);
      SET_CreateFramebuffers(table, _mesa_marshal_CreateFramebuffers);
      SET_CreateRenderbuffers(table, _mesa_marshal_CreateRenderbuffers);
      SET_CreateTextures(table, _mesa_marshal_CreateTextures);
      SET_CreateTransformFeedbacks(table, _mesa_marshal_CreateTransformFeedbacks);
      SET_FlushMappedNamedBufferRange(table, _mesa_marshal_FlushMappedNamedBufferRange);
      SET_GenerateTextureMipmap(table, _mesa_marshal_GenerateTextureMipmap);
      SET_GetNamedBufferParameteri64v(table, _mesa_marshal_GetNamedBufferParameteri64v);
      SET_GetNamedBufferParameteriv(table, _mesa_marshal_GetNamedBufferParameteriv);
      SET_GetNamedBufferPointerv(table, _mesa_marshal_GetNamedBufferPointerv);
      SET_GetNamedBufferSubData(table, _mesa_marshal_GetNamedBufferSubData);
      SET_GetNamedFramebufferAttachmentParameteriv(table, _mesa_marshal_GetNamedFramebufferAttachmentParameteriv);
      SET_GetNamedFramebufferParameteriv(table, _mesa_marshal_GetNamedFramebufferParameteriv);
      SET_GetNamedRenderbufferParameteriv(table, _mesa_marshal_GetNamedRenderbufferParameteriv);
      SET_GetTextureImage(table, _mesa_marshal_GetTextureImage);
      SET_GetTransformFeedbacki64_v(table, _mesa_marshal_GetTransformFeedbacki64_v);
      SET_GetTransformFeedbacki_v(table, _mesa_marshal_GetTransformFeedbacki_v);
      SET_GetTransformFeedbackiv(table, _mesa_marshal_GetTransformFeedbackiv);
      SET_GetVertexAttribLdv(table, _mesa_marshal_GetVertexAttribLdv);
      SET_GetVertexAttribLui64vARB(table, _mesa_marshal_GetVertexAttribLui64vARB);
      SET_InvalidateNamedFramebufferData(table, _mesa_marshal_InvalidateNamedFramebufferData);
      SET_InvalidateNamedFramebufferSubData(table, _mesa_marshal_InvalidateNamedFramebufferSubData);
      SET_MapNamedBuffer(table, _mesa_marshal_MapNamedBuffer);
      SET_MapNamedBufferRange(table, _mesa_marshal_MapNamedBufferRange);
      SET_NamedBufferStorage(table, _mesa_marshal_NamedBufferStorage);
      SET_NamedFramebufferDrawBuffer(table, _mesa_marshal_NamedFramebufferDrawBuffer);
      SET_NamedFramebufferDrawBuffers(table, _mesa_marshal_NamedFramebufferDrawBuffers);
      SET_NamedFramebufferParameteri(table, _mesa_marshal_NamedFramebufferParameteri);
      SET_NamedFramebufferReadBuffer(table, _mesa_marshal_NamedFramebufferReadBuffer);
      SET_NamedFramebufferRenderbuffer(table, _mesa_marshal_NamedFramebufferRenderbuffer);
      SET_NamedFramebufferTexture(table, _mesa_marshal_NamedFramebufferTexture);
      SET_NamedFramebufferTextureLayer(table, _mesa_marshal_NamedFramebufferTextureLayer);
      SET_NamedRenderbufferStorage(table, _mesa_marshal_NamedRenderbufferStorage);
      SET_NamedRenderbufferStorageMultisample(table, _mesa_marshal_NamedRenderbufferStorageMultisample);
      SET_TextureBuffer(table, _mesa_marshal_TextureBuffer);
      SET_TextureBufferRange(table, _mesa_marshal_TextureBufferRange);
      SET_TextureParameterIiv(table, _mesa_marshal_TextureParameterIiv);
      SET_TextureParameterIuiv(table, _mesa_marshal_TextureParameterIuiv);
      SET_TextureParameterf(table, _mesa_marshal_TextureParameterf);
      SET_TextureParameterfv(table, _mesa_marshal_TextureParameterfv);
      SET_TextureParameteri(table, _mesa_marshal_TextureParameteri);
      SET_TextureParameteriv(table, _mesa_marshal_TextureParameteriv);
      SET_TextureStorage1D(table, _mesa_marshal_TextureStorage1D);
      SET_TextureStorage2D(table, _mesa_marshal_TextureStorage2D);
      SET_TextureStorage2DMultisample(table, _mesa_marshal_TextureStorage2DMultisample);
      SET_TextureStorage3D(table, _mesa_marshal_TextureStorage3D);
      SET_TextureStorage3DMultisample(table, _mesa_marshal_TextureStorage3DMultisample);
      SET_TextureSubImage1D(table, _mesa_marshal_TextureSubImage1D);
      SET_TextureSubImage2D(table, _mesa_marshal_TextureSubImage2D);
      SET_TextureSubImage3D(table, _mesa_marshal_TextureSubImage3D);
      SET_TransformFeedbackBufferBase(table, _mesa_marshal_TransformFeedbackBufferBase);
      SET_TransformFeedbackBufferRange(table, _mesa_marshal_TransformFeedbackBufferRange);
      SET_VertexAttribL1dv(table, _mesa_marshal_VertexAttribL1dv);
      SET_VertexAttribL2dv(table, _mesa_marshal_VertexAttribL2dv);
      SET_VertexAttribL3dv(table, _mesa_marshal_VertexAttribL3dv);
      SET_VertexAttribL4d(table, _mesa_marshal_VertexAttribL4d);
      SET_VertexAttribL4dv(table, _mesa_marshal_VertexAttribL4dv);
      SET_VertexAttribLPointer(table, _mesa_marshal_VertexAttribLPointer);
   }
   if (ctx->API == API_OPENGL_COMPAT || ctx->API == API_OPENGL_CORE || (ctx->API == API_OPENGLES2 && ctx->Version >= 31)) {
      SET_FramebufferParameteri(table, _mesa_marshal_FramebufferParameteri);
      SET_GetFramebufferParameteriv(table, _mesa_marshal_GetFramebufferParameteriv);
      SET_TexBufferRange(table, _mesa_marshal_TexBufferRange);
   }
}
