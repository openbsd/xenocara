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
void
_mesa_unmarshal_ProgramUniformMatrix4dv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniformMatrix4dv *cmd)
{
   const GLuint program = cmd->program;
   const GLint location = cmd->location;
   const GLsizei count = cmd->count;
   const GLboolean transpose = cmd->transpose;
   GLdouble * value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLdouble *) variable_data;
   CALL_ProgramUniformMatrix4dv(ctx->CurrentServerDispatch, (program, location, count, transpose, value));
}
void GLAPIENTRY
_mesa_marshal_ProgramUniformMatrix4dv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble * value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 16 * sizeof(GLdouble));
   int cmd_size = sizeof(struct marshal_cmd_ProgramUniformMatrix4dv) + value_size;
   struct marshal_cmd_ProgramUniformMatrix4dv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "ProgramUniformMatrix4dv");
      CALL_ProgramUniformMatrix4dv(ctx->CurrentServerDispatch, (program, location, count, transpose, value));
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
void
_mesa_unmarshal_ProgramUniformMatrix2x3dv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniformMatrix2x3dv *cmd)
{
   const GLuint program = cmd->program;
   const GLint location = cmd->location;
   const GLsizei count = cmd->count;
   const GLboolean transpose = cmd->transpose;
   GLdouble * value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLdouble *) variable_data;
   CALL_ProgramUniformMatrix2x3dv(ctx->CurrentServerDispatch, (program, location, count, transpose, value));
}
void GLAPIENTRY
_mesa_marshal_ProgramUniformMatrix2x3dv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble * value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 6 * sizeof(GLdouble));
   int cmd_size = sizeof(struct marshal_cmd_ProgramUniformMatrix2x3dv) + value_size;
   struct marshal_cmd_ProgramUniformMatrix2x3dv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "ProgramUniformMatrix2x3dv");
      CALL_ProgramUniformMatrix2x3dv(ctx->CurrentServerDispatch, (program, location, count, transpose, value));
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
void
_mesa_unmarshal_ProgramUniformMatrix2x4dv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniformMatrix2x4dv *cmd)
{
   const GLuint program = cmd->program;
   const GLint location = cmd->location;
   const GLsizei count = cmd->count;
   const GLboolean transpose = cmd->transpose;
   GLdouble * value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLdouble *) variable_data;
   CALL_ProgramUniformMatrix2x4dv(ctx->CurrentServerDispatch, (program, location, count, transpose, value));
}
void GLAPIENTRY
_mesa_marshal_ProgramUniformMatrix2x4dv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble * value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 8 * sizeof(GLdouble));
   int cmd_size = sizeof(struct marshal_cmd_ProgramUniformMatrix2x4dv) + value_size;
   struct marshal_cmd_ProgramUniformMatrix2x4dv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "ProgramUniformMatrix2x4dv");
      CALL_ProgramUniformMatrix2x4dv(ctx->CurrentServerDispatch, (program, location, count, transpose, value));
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
void
_mesa_unmarshal_ProgramUniformMatrix3x2dv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniformMatrix3x2dv *cmd)
{
   const GLuint program = cmd->program;
   const GLint location = cmd->location;
   const GLsizei count = cmd->count;
   const GLboolean transpose = cmd->transpose;
   GLdouble * value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLdouble *) variable_data;
   CALL_ProgramUniformMatrix3x2dv(ctx->CurrentServerDispatch, (program, location, count, transpose, value));
}
void GLAPIENTRY
_mesa_marshal_ProgramUniformMatrix3x2dv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble * value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 6 * sizeof(GLdouble));
   int cmd_size = sizeof(struct marshal_cmd_ProgramUniformMatrix3x2dv) + value_size;
   struct marshal_cmd_ProgramUniformMatrix3x2dv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "ProgramUniformMatrix3x2dv");
      CALL_ProgramUniformMatrix3x2dv(ctx->CurrentServerDispatch, (program, location, count, transpose, value));
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
void
_mesa_unmarshal_ProgramUniformMatrix3x4dv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniformMatrix3x4dv *cmd)
{
   const GLuint program = cmd->program;
   const GLint location = cmd->location;
   const GLsizei count = cmd->count;
   const GLboolean transpose = cmd->transpose;
   GLdouble * value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLdouble *) variable_data;
   CALL_ProgramUniformMatrix3x4dv(ctx->CurrentServerDispatch, (program, location, count, transpose, value));
}
void GLAPIENTRY
_mesa_marshal_ProgramUniformMatrix3x4dv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble * value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 12 * sizeof(GLdouble));
   int cmd_size = sizeof(struct marshal_cmd_ProgramUniformMatrix3x4dv) + value_size;
   struct marshal_cmd_ProgramUniformMatrix3x4dv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "ProgramUniformMatrix3x4dv");
      CALL_ProgramUniformMatrix3x4dv(ctx->CurrentServerDispatch, (program, location, count, transpose, value));
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
void
_mesa_unmarshal_ProgramUniformMatrix4x2dv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniformMatrix4x2dv *cmd)
{
   const GLuint program = cmd->program;
   const GLint location = cmd->location;
   const GLsizei count = cmd->count;
   const GLboolean transpose = cmd->transpose;
   GLdouble * value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLdouble *) variable_data;
   CALL_ProgramUniformMatrix4x2dv(ctx->CurrentServerDispatch, (program, location, count, transpose, value));
}
void GLAPIENTRY
_mesa_marshal_ProgramUniformMatrix4x2dv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble * value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 8 * sizeof(GLdouble));
   int cmd_size = sizeof(struct marshal_cmd_ProgramUniformMatrix4x2dv) + value_size;
   struct marshal_cmd_ProgramUniformMatrix4x2dv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "ProgramUniformMatrix4x2dv");
      CALL_ProgramUniformMatrix4x2dv(ctx->CurrentServerDispatch, (program, location, count, transpose, value));
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
void
_mesa_unmarshal_ProgramUniformMatrix4x3dv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniformMatrix4x3dv *cmd)
{
   const GLuint program = cmd->program;
   const GLint location = cmd->location;
   const GLsizei count = cmd->count;
   const GLboolean transpose = cmd->transpose;
   GLdouble * value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLdouble *) variable_data;
   CALL_ProgramUniformMatrix4x3dv(ctx->CurrentServerDispatch, (program, location, count, transpose, value));
}
void GLAPIENTRY
_mesa_marshal_ProgramUniformMatrix4x3dv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble * value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 12 * sizeof(GLdouble));
   int cmd_size = sizeof(struct marshal_cmd_ProgramUniformMatrix4x3dv) + value_size;
   struct marshal_cmd_ProgramUniformMatrix4x3dv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "ProgramUniformMatrix4x3dv");
      CALL_ProgramUniformMatrix4x3dv(ctx->CurrentServerDispatch, (program, location, count, transpose, value));
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
void
_mesa_unmarshal_DrawTransformFeedbackStream(struct gl_context *ctx, const struct marshal_cmd_DrawTransformFeedbackStream *cmd)
{
   const GLenum mode = cmd->mode;
   const GLuint id = cmd->id;
   const GLuint stream = cmd->stream;
   CALL_DrawTransformFeedbackStream(ctx->CurrentServerDispatch, (mode, id, stream));
}
void GLAPIENTRY
_mesa_marshal_DrawTransformFeedbackStream(GLenum mode, GLuint id, GLuint stream)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_DrawTransformFeedbackStream);
   struct marshal_cmd_DrawTransformFeedbackStream *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_DrawTransformFeedbackStream, cmd_size);
   cmd->mode = mode;
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
void
_mesa_unmarshal_BeginQueryIndexed(struct gl_context *ctx, const struct marshal_cmd_BeginQueryIndexed *cmd)
{
   const GLenum target = cmd->target;
   const GLuint index = cmd->index;
   const GLuint id = cmd->id;
   CALL_BeginQueryIndexed(ctx->CurrentServerDispatch, (target, index, id));
}
void GLAPIENTRY
_mesa_marshal_BeginQueryIndexed(GLenum target, GLuint index, GLuint id)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_BeginQueryIndexed);
   struct marshal_cmd_BeginQueryIndexed *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_BeginQueryIndexed, cmd_size);
   cmd->target = target;
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
void
_mesa_unmarshal_EndQueryIndexed(struct gl_context *ctx, const struct marshal_cmd_EndQueryIndexed *cmd)
{
   const GLenum target = cmd->target;
   const GLuint index = cmd->index;
   CALL_EndQueryIndexed(ctx->CurrentServerDispatch, (target, index));
}
void GLAPIENTRY
_mesa_marshal_EndQueryIndexed(GLenum target, GLuint index)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_EndQueryIndexed);
   struct marshal_cmd_EndQueryIndexed *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_EndQueryIndexed, cmd_size);
   cmd->target = target;
   cmd->index = index;
}


/* GetQueryIndexediv: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetQueryIndexediv(GLenum target, GLuint index, GLenum pname, GLint * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetQueryIndexediv");
   CALL_GetQueryIndexediv(ctx->CurrentServerDispatch, (target, index, pname, params));
}


/* UseProgramStages: marshalled asynchronously */
struct marshal_cmd_UseProgramStages
{
   struct marshal_cmd_base cmd_base;
   GLuint pipeline;
   GLbitfield stages;
   GLuint program;
};
void
_mesa_unmarshal_UseProgramStages(struct gl_context *ctx, const struct marshal_cmd_UseProgramStages *cmd)
{
   const GLuint pipeline = cmd->pipeline;
   const GLbitfield stages = cmd->stages;
   const GLuint program = cmd->program;
   CALL_UseProgramStages(ctx->CurrentServerDispatch, (pipeline, stages, program));
}
void GLAPIENTRY
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
void
_mesa_unmarshal_ActiveShaderProgram(struct gl_context *ctx, const struct marshal_cmd_ActiveShaderProgram *cmd)
{
   const GLuint pipeline = cmd->pipeline;
   const GLuint program = cmd->program;
   CALL_ActiveShaderProgram(ctx->CurrentServerDispatch, (pipeline, program));
}
void GLAPIENTRY
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
GLuint GLAPIENTRY
_mesa_marshal_CreateShaderProgramv(GLenum type, GLsizei count, const GLchar * const * strings)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "CreateShaderProgramv");
   return CALL_CreateShaderProgramv(ctx->CurrentServerDispatch, (type, count, strings));
}


/* BindProgramPipeline: marshalled asynchronously */
struct marshal_cmd_BindProgramPipeline
{
   struct marshal_cmd_base cmd_base;
   GLuint pipeline;
};
void
_mesa_unmarshal_BindProgramPipeline(struct gl_context *ctx, const struct marshal_cmd_BindProgramPipeline *cmd)
{
   const GLuint pipeline = cmd->pipeline;
   CALL_BindProgramPipeline(ctx->CurrentServerDispatch, (pipeline));
}
void GLAPIENTRY
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
void
_mesa_unmarshal_DeleteProgramPipelines(struct gl_context *ctx, const struct marshal_cmd_DeleteProgramPipelines *cmd)
{
   const GLsizei n = cmd->n;
   GLuint * pipelines;
   const char *variable_data = (const char *) (cmd + 1);
   pipelines = (GLuint *) variable_data;
   CALL_DeleteProgramPipelines(ctx->CurrentServerDispatch, (n, pipelines));
}
void GLAPIENTRY
_mesa_marshal_DeleteProgramPipelines(GLsizei n, const GLuint * pipelines)
{
   GET_CURRENT_CONTEXT(ctx);
   int pipelines_size = safe_mul(n, 1 * sizeof(GLuint));
   int cmd_size = sizeof(struct marshal_cmd_DeleteProgramPipelines) + pipelines_size;
   struct marshal_cmd_DeleteProgramPipelines *cmd;
   if (unlikely(pipelines_size < 0 || (pipelines_size > 0 && !pipelines) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "DeleteProgramPipelines");
      CALL_DeleteProgramPipelines(ctx->CurrentServerDispatch, (n, pipelines));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_DeleteProgramPipelines, cmd_size);
   cmd->n = n;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, pipelines, pipelines_size);
}


/* GenProgramPipelines: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GenProgramPipelines(GLsizei n, GLuint * pipelines)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GenProgramPipelines");
   CALL_GenProgramPipelines(ctx->CurrentServerDispatch, (n, pipelines));
}


/* IsProgramPipeline: marshalled synchronously */
GLboolean GLAPIENTRY
_mesa_marshal_IsProgramPipeline(GLuint pipeline)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "IsProgramPipeline");
   return CALL_IsProgramPipeline(ctx->CurrentServerDispatch, (pipeline));
}


/* GetProgramPipelineiv: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetProgramPipelineiv(GLuint pipeline, GLenum pname, GLint * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetProgramPipelineiv");
   CALL_GetProgramPipelineiv(ctx->CurrentServerDispatch, (pipeline, pname, params));
}


/* ProgramUniform1i: marshalled asynchronously */
struct marshal_cmd_ProgramUniform1i
{
   struct marshal_cmd_base cmd_base;
   GLuint program;
   GLint location;
   GLint x;
};
void
_mesa_unmarshal_ProgramUniform1i(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform1i *cmd)
{
   const GLuint program = cmd->program;
   const GLint location = cmd->location;
   const GLint x = cmd->x;
   CALL_ProgramUniform1i(ctx->CurrentServerDispatch, (program, location, x));
}
void GLAPIENTRY
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
void
_mesa_unmarshal_ProgramUniform2i(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform2i *cmd)
{
   const GLuint program = cmd->program;
   const GLint location = cmd->location;
   const GLint x = cmd->x;
   const GLint y = cmd->y;
   CALL_ProgramUniform2i(ctx->CurrentServerDispatch, (program, location, x, y));
}
void GLAPIENTRY
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
void
_mesa_unmarshal_ProgramUniform3i(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform3i *cmd)
{
   const GLuint program = cmd->program;
   const GLint location = cmd->location;
   const GLint x = cmd->x;
   const GLint y = cmd->y;
   const GLint z = cmd->z;
   CALL_ProgramUniform3i(ctx->CurrentServerDispatch, (program, location, x, y, z));
}
void GLAPIENTRY
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
void
_mesa_unmarshal_ProgramUniform4i(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform4i *cmd)
{
   const GLuint program = cmd->program;
   const GLint location = cmd->location;
   const GLint x = cmd->x;
   const GLint y = cmd->y;
   const GLint z = cmd->z;
   const GLint w = cmd->w;
   CALL_ProgramUniform4i(ctx->CurrentServerDispatch, (program, location, x, y, z, w));
}
void GLAPIENTRY
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
void
_mesa_unmarshal_ProgramUniform1ui(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform1ui *cmd)
{
   const GLuint program = cmd->program;
   const GLint location = cmd->location;
   const GLuint x = cmd->x;
   CALL_ProgramUniform1ui(ctx->CurrentServerDispatch, (program, location, x));
}
void GLAPIENTRY
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
void
_mesa_unmarshal_ProgramUniform2ui(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform2ui *cmd)
{
   const GLuint program = cmd->program;
   const GLint location = cmd->location;
   const GLuint x = cmd->x;
   const GLuint y = cmd->y;
   CALL_ProgramUniform2ui(ctx->CurrentServerDispatch, (program, location, x, y));
}
void GLAPIENTRY
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
void
_mesa_unmarshal_ProgramUniform3ui(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform3ui *cmd)
{
   const GLuint program = cmd->program;
   const GLint location = cmd->location;
   const GLuint x = cmd->x;
   const GLuint y = cmd->y;
   const GLuint z = cmd->z;
   CALL_ProgramUniform3ui(ctx->CurrentServerDispatch, (program, location, x, y, z));
}
void GLAPIENTRY
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
void
_mesa_unmarshal_ProgramUniform4ui(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform4ui *cmd)
{
   const GLuint program = cmd->program;
   const GLint location = cmd->location;
   const GLuint x = cmd->x;
   const GLuint y = cmd->y;
   const GLuint z = cmd->z;
   const GLuint w = cmd->w;
   CALL_ProgramUniform4ui(ctx->CurrentServerDispatch, (program, location, x, y, z, w));
}
void GLAPIENTRY
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
void
_mesa_unmarshal_ProgramUniform1f(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform1f *cmd)
{
   const GLuint program = cmd->program;
   const GLint location = cmd->location;
   const GLfloat x = cmd->x;
   CALL_ProgramUniform1f(ctx->CurrentServerDispatch, (program, location, x));
}
void GLAPIENTRY
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
void
_mesa_unmarshal_ProgramUniform2f(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform2f *cmd)
{
   const GLuint program = cmd->program;
   const GLint location = cmd->location;
   const GLfloat x = cmd->x;
   const GLfloat y = cmd->y;
   CALL_ProgramUniform2f(ctx->CurrentServerDispatch, (program, location, x, y));
}
void GLAPIENTRY
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
void
_mesa_unmarshal_ProgramUniform3f(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform3f *cmd)
{
   const GLuint program = cmd->program;
   const GLint location = cmd->location;
   const GLfloat x = cmd->x;
   const GLfloat y = cmd->y;
   const GLfloat z = cmd->z;
   CALL_ProgramUniform3f(ctx->CurrentServerDispatch, (program, location, x, y, z));
}
void GLAPIENTRY
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
void
_mesa_unmarshal_ProgramUniform4f(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform4f *cmd)
{
   const GLuint program = cmd->program;
   const GLint location = cmd->location;
   const GLfloat x = cmd->x;
   const GLfloat y = cmd->y;
   const GLfloat z = cmd->z;
   const GLfloat w = cmd->w;
   CALL_ProgramUniform4f(ctx->CurrentServerDispatch, (program, location, x, y, z, w));
}
void GLAPIENTRY
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
void
_mesa_unmarshal_ProgramUniform1iv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform1iv *cmd)
{
   const GLuint program = cmd->program;
   const GLint location = cmd->location;
   const GLsizei count = cmd->count;
   GLint * value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLint *) variable_data;
   CALL_ProgramUniform1iv(ctx->CurrentServerDispatch, (program, location, count, value));
}
void GLAPIENTRY
_mesa_marshal_ProgramUniform1iv(GLuint program, GLint location, GLsizei count, const GLint * value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 1 * sizeof(GLint));
   int cmd_size = sizeof(struct marshal_cmd_ProgramUniform1iv) + value_size;
   struct marshal_cmd_ProgramUniform1iv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "ProgramUniform1iv");
      CALL_ProgramUniform1iv(ctx->CurrentServerDispatch, (program, location, count, value));
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
void
_mesa_unmarshal_ProgramUniform2iv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform2iv *cmd)
{
   const GLuint program = cmd->program;
   const GLint location = cmd->location;
   const GLsizei count = cmd->count;
   GLint * value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLint *) variable_data;
   CALL_ProgramUniform2iv(ctx->CurrentServerDispatch, (program, location, count, value));
}
void GLAPIENTRY
_mesa_marshal_ProgramUniform2iv(GLuint program, GLint location, GLsizei count, const GLint * value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 2 * sizeof(GLint));
   int cmd_size = sizeof(struct marshal_cmd_ProgramUniform2iv) + value_size;
   struct marshal_cmd_ProgramUniform2iv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "ProgramUniform2iv");
      CALL_ProgramUniform2iv(ctx->CurrentServerDispatch, (program, location, count, value));
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
void
_mesa_unmarshal_ProgramUniform3iv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform3iv *cmd)
{
   const GLuint program = cmd->program;
   const GLint location = cmd->location;
   const GLsizei count = cmd->count;
   GLint * value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLint *) variable_data;
   CALL_ProgramUniform3iv(ctx->CurrentServerDispatch, (program, location, count, value));
}
void GLAPIENTRY
_mesa_marshal_ProgramUniform3iv(GLuint program, GLint location, GLsizei count, const GLint * value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 3 * sizeof(GLint));
   int cmd_size = sizeof(struct marshal_cmd_ProgramUniform3iv) + value_size;
   struct marshal_cmd_ProgramUniform3iv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "ProgramUniform3iv");
      CALL_ProgramUniform3iv(ctx->CurrentServerDispatch, (program, location, count, value));
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
void
_mesa_unmarshal_ProgramUniform4iv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform4iv *cmd)
{
   const GLuint program = cmd->program;
   const GLint location = cmd->location;
   const GLsizei count = cmd->count;
   GLint * value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLint *) variable_data;
   CALL_ProgramUniform4iv(ctx->CurrentServerDispatch, (program, location, count, value));
}
void GLAPIENTRY
_mesa_marshal_ProgramUniform4iv(GLuint program, GLint location, GLsizei count, const GLint * value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 4 * sizeof(GLint));
   int cmd_size = sizeof(struct marshal_cmd_ProgramUniform4iv) + value_size;
   struct marshal_cmd_ProgramUniform4iv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "ProgramUniform4iv");
      CALL_ProgramUniform4iv(ctx->CurrentServerDispatch, (program, location, count, value));
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
void
_mesa_unmarshal_ProgramUniform1uiv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform1uiv *cmd)
{
   const GLuint program = cmd->program;
   const GLint location = cmd->location;
   const GLsizei count = cmd->count;
   GLuint * value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLuint *) variable_data;
   CALL_ProgramUniform1uiv(ctx->CurrentServerDispatch, (program, location, count, value));
}
void GLAPIENTRY
_mesa_marshal_ProgramUniform1uiv(GLuint program, GLint location, GLsizei count, const GLuint * value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 1 * sizeof(GLuint));
   int cmd_size = sizeof(struct marshal_cmd_ProgramUniform1uiv) + value_size;
   struct marshal_cmd_ProgramUniform1uiv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "ProgramUniform1uiv");
      CALL_ProgramUniform1uiv(ctx->CurrentServerDispatch, (program, location, count, value));
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
void
_mesa_unmarshal_ProgramUniform2uiv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform2uiv *cmd)
{
   const GLuint program = cmd->program;
   const GLint location = cmd->location;
   const GLsizei count = cmd->count;
   GLuint * value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLuint *) variable_data;
   CALL_ProgramUniform2uiv(ctx->CurrentServerDispatch, (program, location, count, value));
}
void GLAPIENTRY
_mesa_marshal_ProgramUniform2uiv(GLuint program, GLint location, GLsizei count, const GLuint * value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 2 * sizeof(GLuint));
   int cmd_size = sizeof(struct marshal_cmd_ProgramUniform2uiv) + value_size;
   struct marshal_cmd_ProgramUniform2uiv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "ProgramUniform2uiv");
      CALL_ProgramUniform2uiv(ctx->CurrentServerDispatch, (program, location, count, value));
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
void
_mesa_unmarshal_ProgramUniform3uiv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform3uiv *cmd)
{
   const GLuint program = cmd->program;
   const GLint location = cmd->location;
   const GLsizei count = cmd->count;
   GLuint * value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLuint *) variable_data;
   CALL_ProgramUniform3uiv(ctx->CurrentServerDispatch, (program, location, count, value));
}
void GLAPIENTRY
_mesa_marshal_ProgramUniform3uiv(GLuint program, GLint location, GLsizei count, const GLuint * value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 3 * sizeof(GLuint));
   int cmd_size = sizeof(struct marshal_cmd_ProgramUniform3uiv) + value_size;
   struct marshal_cmd_ProgramUniform3uiv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "ProgramUniform3uiv");
      CALL_ProgramUniform3uiv(ctx->CurrentServerDispatch, (program, location, count, value));
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
void
_mesa_unmarshal_ProgramUniform4uiv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform4uiv *cmd)
{
   const GLuint program = cmd->program;
   const GLint location = cmd->location;
   const GLsizei count = cmd->count;
   GLuint * value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLuint *) variable_data;
   CALL_ProgramUniform4uiv(ctx->CurrentServerDispatch, (program, location, count, value));
}
void GLAPIENTRY
_mesa_marshal_ProgramUniform4uiv(GLuint program, GLint location, GLsizei count, const GLuint * value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 4 * sizeof(GLuint));
   int cmd_size = sizeof(struct marshal_cmd_ProgramUniform4uiv) + value_size;
   struct marshal_cmd_ProgramUniform4uiv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "ProgramUniform4uiv");
      CALL_ProgramUniform4uiv(ctx->CurrentServerDispatch, (program, location, count, value));
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
void
_mesa_unmarshal_ProgramUniform1fv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform1fv *cmd)
{
   const GLuint program = cmd->program;
   const GLint location = cmd->location;
   const GLsizei count = cmd->count;
   GLfloat * value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLfloat *) variable_data;
   CALL_ProgramUniform1fv(ctx->CurrentServerDispatch, (program, location, count, value));
}
void GLAPIENTRY
_mesa_marshal_ProgramUniform1fv(GLuint program, GLint location, GLsizei count, const GLfloat * value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 1 * sizeof(GLfloat));
   int cmd_size = sizeof(struct marshal_cmd_ProgramUniform1fv) + value_size;
   struct marshal_cmd_ProgramUniform1fv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "ProgramUniform1fv");
      CALL_ProgramUniform1fv(ctx->CurrentServerDispatch, (program, location, count, value));
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
void
_mesa_unmarshal_ProgramUniform2fv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform2fv *cmd)
{
   const GLuint program = cmd->program;
   const GLint location = cmd->location;
   const GLsizei count = cmd->count;
   GLfloat * value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLfloat *) variable_data;
   CALL_ProgramUniform2fv(ctx->CurrentServerDispatch, (program, location, count, value));
}
void GLAPIENTRY
_mesa_marshal_ProgramUniform2fv(GLuint program, GLint location, GLsizei count, const GLfloat * value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 2 * sizeof(GLfloat));
   int cmd_size = sizeof(struct marshal_cmd_ProgramUniform2fv) + value_size;
   struct marshal_cmd_ProgramUniform2fv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "ProgramUniform2fv");
      CALL_ProgramUniform2fv(ctx->CurrentServerDispatch, (program, location, count, value));
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
void
_mesa_unmarshal_ProgramUniform3fv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform3fv *cmd)
{
   const GLuint program = cmd->program;
   const GLint location = cmd->location;
   const GLsizei count = cmd->count;
   GLfloat * value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLfloat *) variable_data;
   CALL_ProgramUniform3fv(ctx->CurrentServerDispatch, (program, location, count, value));
}
void GLAPIENTRY
_mesa_marshal_ProgramUniform3fv(GLuint program, GLint location, GLsizei count, const GLfloat * value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 3 * sizeof(GLfloat));
   int cmd_size = sizeof(struct marshal_cmd_ProgramUniform3fv) + value_size;
   struct marshal_cmd_ProgramUniform3fv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "ProgramUniform3fv");
      CALL_ProgramUniform3fv(ctx->CurrentServerDispatch, (program, location, count, value));
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
void
_mesa_unmarshal_ProgramUniform4fv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform4fv *cmd)
{
   const GLuint program = cmd->program;
   const GLint location = cmd->location;
   const GLsizei count = cmd->count;
   GLfloat * value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLfloat *) variable_data;
   CALL_ProgramUniform4fv(ctx->CurrentServerDispatch, (program, location, count, value));
}
void GLAPIENTRY
_mesa_marshal_ProgramUniform4fv(GLuint program, GLint location, GLsizei count, const GLfloat * value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 4 * sizeof(GLfloat));
   int cmd_size = sizeof(struct marshal_cmd_ProgramUniform4fv) + value_size;
   struct marshal_cmd_ProgramUniform4fv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "ProgramUniform4fv");
      CALL_ProgramUniform4fv(ctx->CurrentServerDispatch, (program, location, count, value));
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
void
_mesa_unmarshal_ProgramUniformMatrix2fv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniformMatrix2fv *cmd)
{
   const GLuint program = cmd->program;
   const GLint location = cmd->location;
   const GLsizei count = cmd->count;
   const GLboolean transpose = cmd->transpose;
   GLfloat * value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLfloat *) variable_data;
   CALL_ProgramUniformMatrix2fv(ctx->CurrentServerDispatch, (program, location, count, transpose, value));
}
void GLAPIENTRY
_mesa_marshal_ProgramUniformMatrix2fv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat * value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 4 * sizeof(GLfloat));
   int cmd_size = sizeof(struct marshal_cmd_ProgramUniformMatrix2fv) + value_size;
   struct marshal_cmd_ProgramUniformMatrix2fv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "ProgramUniformMatrix2fv");
      CALL_ProgramUniformMatrix2fv(ctx->CurrentServerDispatch, (program, location, count, transpose, value));
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
void
_mesa_unmarshal_ProgramUniformMatrix3fv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniformMatrix3fv *cmd)
{
   const GLuint program = cmd->program;
   const GLint location = cmd->location;
   const GLsizei count = cmd->count;
   const GLboolean transpose = cmd->transpose;
   GLfloat * value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLfloat *) variable_data;
   CALL_ProgramUniformMatrix3fv(ctx->CurrentServerDispatch, (program, location, count, transpose, value));
}
void GLAPIENTRY
_mesa_marshal_ProgramUniformMatrix3fv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat * value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 9 * sizeof(GLfloat));
   int cmd_size = sizeof(struct marshal_cmd_ProgramUniformMatrix3fv) + value_size;
   struct marshal_cmd_ProgramUniformMatrix3fv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "ProgramUniformMatrix3fv");
      CALL_ProgramUniformMatrix3fv(ctx->CurrentServerDispatch, (program, location, count, transpose, value));
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
void
_mesa_unmarshal_ProgramUniformMatrix4fv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniformMatrix4fv *cmd)
{
   const GLuint program = cmd->program;
   const GLint location = cmd->location;
   const GLsizei count = cmd->count;
   const GLboolean transpose = cmd->transpose;
   GLfloat * value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLfloat *) variable_data;
   CALL_ProgramUniformMatrix4fv(ctx->CurrentServerDispatch, (program, location, count, transpose, value));
}
void GLAPIENTRY
_mesa_marshal_ProgramUniformMatrix4fv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat * value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 16 * sizeof(GLfloat));
   int cmd_size = sizeof(struct marshal_cmd_ProgramUniformMatrix4fv) + value_size;
   struct marshal_cmd_ProgramUniformMatrix4fv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "ProgramUniformMatrix4fv");
      CALL_ProgramUniformMatrix4fv(ctx->CurrentServerDispatch, (program, location, count, transpose, value));
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
void
_mesa_unmarshal_ProgramUniformMatrix2x3fv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniformMatrix2x3fv *cmd)
{
   const GLuint program = cmd->program;
   const GLint location = cmd->location;
   const GLsizei count = cmd->count;
   const GLboolean transpose = cmd->transpose;
   GLfloat * value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLfloat *) variable_data;
   CALL_ProgramUniformMatrix2x3fv(ctx->CurrentServerDispatch, (program, location, count, transpose, value));
}
void GLAPIENTRY
_mesa_marshal_ProgramUniformMatrix2x3fv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat * value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 6 * sizeof(GLfloat));
   int cmd_size = sizeof(struct marshal_cmd_ProgramUniformMatrix2x3fv) + value_size;
   struct marshal_cmd_ProgramUniformMatrix2x3fv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "ProgramUniformMatrix2x3fv");
      CALL_ProgramUniformMatrix2x3fv(ctx->CurrentServerDispatch, (program, location, count, transpose, value));
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
void
_mesa_unmarshal_ProgramUniformMatrix3x2fv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniformMatrix3x2fv *cmd)
{
   const GLuint program = cmd->program;
   const GLint location = cmd->location;
   const GLsizei count = cmd->count;
   const GLboolean transpose = cmd->transpose;
   GLfloat * value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLfloat *) variable_data;
   CALL_ProgramUniformMatrix3x2fv(ctx->CurrentServerDispatch, (program, location, count, transpose, value));
}
void GLAPIENTRY
_mesa_marshal_ProgramUniformMatrix3x2fv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat * value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 6 * sizeof(GLfloat));
   int cmd_size = sizeof(struct marshal_cmd_ProgramUniformMatrix3x2fv) + value_size;
   struct marshal_cmd_ProgramUniformMatrix3x2fv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "ProgramUniformMatrix3x2fv");
      CALL_ProgramUniformMatrix3x2fv(ctx->CurrentServerDispatch, (program, location, count, transpose, value));
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
void
_mesa_unmarshal_ProgramUniformMatrix2x4fv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniformMatrix2x4fv *cmd)
{
   const GLuint program = cmd->program;
   const GLint location = cmd->location;
   const GLsizei count = cmd->count;
   const GLboolean transpose = cmd->transpose;
   GLfloat * value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLfloat *) variable_data;
   CALL_ProgramUniformMatrix2x4fv(ctx->CurrentServerDispatch, (program, location, count, transpose, value));
}
void GLAPIENTRY
_mesa_marshal_ProgramUniformMatrix2x4fv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat * value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 8 * sizeof(GLfloat));
   int cmd_size = sizeof(struct marshal_cmd_ProgramUniformMatrix2x4fv) + value_size;
   struct marshal_cmd_ProgramUniformMatrix2x4fv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "ProgramUniformMatrix2x4fv");
      CALL_ProgramUniformMatrix2x4fv(ctx->CurrentServerDispatch, (program, location, count, transpose, value));
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
void
_mesa_unmarshal_ProgramUniformMatrix4x2fv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniformMatrix4x2fv *cmd)
{
   const GLuint program = cmd->program;
   const GLint location = cmd->location;
   const GLsizei count = cmd->count;
   const GLboolean transpose = cmd->transpose;
   GLfloat * value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLfloat *) variable_data;
   CALL_ProgramUniformMatrix4x2fv(ctx->CurrentServerDispatch, (program, location, count, transpose, value));
}
void GLAPIENTRY
_mesa_marshal_ProgramUniformMatrix4x2fv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat * value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 8 * sizeof(GLfloat));
   int cmd_size = sizeof(struct marshal_cmd_ProgramUniformMatrix4x2fv) + value_size;
   struct marshal_cmd_ProgramUniformMatrix4x2fv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "ProgramUniformMatrix4x2fv");
      CALL_ProgramUniformMatrix4x2fv(ctx->CurrentServerDispatch, (program, location, count, transpose, value));
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
void
_mesa_unmarshal_ProgramUniformMatrix3x4fv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniformMatrix3x4fv *cmd)
{
   const GLuint program = cmd->program;
   const GLint location = cmd->location;
   const GLsizei count = cmd->count;
   const GLboolean transpose = cmd->transpose;
   GLfloat * value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLfloat *) variable_data;
   CALL_ProgramUniformMatrix3x4fv(ctx->CurrentServerDispatch, (program, location, count, transpose, value));
}
void GLAPIENTRY
_mesa_marshal_ProgramUniformMatrix3x4fv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat * value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 12 * sizeof(GLfloat));
   int cmd_size = sizeof(struct marshal_cmd_ProgramUniformMatrix3x4fv) + value_size;
   struct marshal_cmd_ProgramUniformMatrix3x4fv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "ProgramUniformMatrix3x4fv");
      CALL_ProgramUniformMatrix3x4fv(ctx->CurrentServerDispatch, (program, location, count, transpose, value));
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
void
_mesa_unmarshal_ProgramUniformMatrix4x3fv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniformMatrix4x3fv *cmd)
{
   const GLuint program = cmd->program;
   const GLint location = cmd->location;
   const GLsizei count = cmd->count;
   const GLboolean transpose = cmd->transpose;
   GLfloat * value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLfloat *) variable_data;
   CALL_ProgramUniformMatrix4x3fv(ctx->CurrentServerDispatch, (program, location, count, transpose, value));
}
void GLAPIENTRY
_mesa_marshal_ProgramUniformMatrix4x3fv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat * value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 12 * sizeof(GLfloat));
   int cmd_size = sizeof(struct marshal_cmd_ProgramUniformMatrix4x3fv) + value_size;
   struct marshal_cmd_ProgramUniformMatrix4x3fv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "ProgramUniformMatrix4x3fv");
      CALL_ProgramUniformMatrix4x3fv(ctx->CurrentServerDispatch, (program, location, count, transpose, value));
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
void
_mesa_unmarshal_ValidateProgramPipeline(struct gl_context *ctx, const struct marshal_cmd_ValidateProgramPipeline *cmd)
{
   const GLuint pipeline = cmd->pipeline;
   CALL_ValidateProgramPipeline(ctx->CurrentServerDispatch, (pipeline));
}
void GLAPIENTRY
_mesa_marshal_ValidateProgramPipeline(GLuint pipeline)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ValidateProgramPipeline);
   struct marshal_cmd_ValidateProgramPipeline *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ValidateProgramPipeline, cmd_size);
   cmd->pipeline = pipeline;
}


/* GetProgramPipelineInfoLog: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetProgramPipelineInfoLog(GLuint pipeline, GLsizei bufSize, GLsizei * length, GLchar * infoLog)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetProgramPipelineInfoLog");
   CALL_GetProgramPipelineInfoLog(ctx->CurrentServerDispatch, (pipeline, bufSize, length, infoLog));
}


/* VertexAttribL1d: marshalled asynchronously */
struct marshal_cmd_VertexAttribL1d
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLdouble x;
};
void
_mesa_unmarshal_VertexAttribL1d(struct gl_context *ctx, const struct marshal_cmd_VertexAttribL1d *cmd)
{
   const GLuint index = cmd->index;
   const GLdouble x = cmd->x;
   CALL_VertexAttribL1d(ctx->CurrentServerDispatch, (index, x));
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
void
_mesa_unmarshal_VertexAttribL2d(struct gl_context *ctx, const struct marshal_cmd_VertexAttribL2d *cmd)
{
   const GLuint index = cmd->index;
   const GLdouble x = cmd->x;
   const GLdouble y = cmd->y;
   CALL_VertexAttribL2d(ctx->CurrentServerDispatch, (index, x, y));
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
void
_mesa_unmarshal_VertexAttribL3d(struct gl_context *ctx, const struct marshal_cmd_VertexAttribL3d *cmd)
{
   const GLuint index = cmd->index;
   const GLdouble x = cmd->x;
   const GLdouble y = cmd->y;
   const GLdouble z = cmd->z;
   CALL_VertexAttribL3d(ctx->CurrentServerDispatch, (index, x, y, z));
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
void
_mesa_unmarshal_VertexAttribL4d(struct gl_context *ctx, const struct marshal_cmd_VertexAttribL4d *cmd)
{
   const GLuint index = cmd->index;
   const GLdouble x = cmd->x;
   const GLdouble y = cmd->y;
   const GLdouble z = cmd->z;
   const GLdouble w = cmd->w;
   CALL_VertexAttribL4d(ctx->CurrentServerDispatch, (index, x, y, z, w));
}
void GLAPIENTRY
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
void
_mesa_unmarshal_VertexAttribL1dv(struct gl_context *ctx, const struct marshal_cmd_VertexAttribL1dv *cmd)
{
   const GLuint index = cmd->index;
   const GLdouble * v = cmd->v;
   CALL_VertexAttribL1dv(ctx->CurrentServerDispatch, (index, v));
}
void GLAPIENTRY
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
void
_mesa_unmarshal_VertexAttribL2dv(struct gl_context *ctx, const struct marshal_cmd_VertexAttribL2dv *cmd)
{
   const GLuint index = cmd->index;
   const GLdouble * v = cmd->v;
   CALL_VertexAttribL2dv(ctx->CurrentServerDispatch, (index, v));
}
void GLAPIENTRY
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
void
_mesa_unmarshal_VertexAttribL3dv(struct gl_context *ctx, const struct marshal_cmd_VertexAttribL3dv *cmd)
{
   const GLuint index = cmd->index;
   const GLdouble * v = cmd->v;
   CALL_VertexAttribL3dv(ctx->CurrentServerDispatch, (index, v));
}
void GLAPIENTRY
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
void
_mesa_unmarshal_VertexAttribL4dv(struct gl_context *ctx, const struct marshal_cmd_VertexAttribL4dv *cmd)
{
   const GLuint index = cmd->index;
   const GLdouble * v = cmd->v;
   CALL_VertexAttribL4dv(ctx->CurrentServerDispatch, (index, v));
}
void GLAPIENTRY
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
void
_mesa_unmarshal_VertexAttribLPointer(struct gl_context *ctx, const struct marshal_cmd_VertexAttribLPointer *cmd)
{
   const GLuint index = cmd->index;
   const GLint size = cmd->size;
   const GLenum type = cmd->type;
   const GLsizei stride = cmd->stride;
   const GLvoid * pointer = cmd->pointer;
   CALL_VertexAttribLPointer(ctx->CurrentServerDispatch, (index, size, type, stride, pointer));
}
void GLAPIENTRY
_mesa_marshal_VertexAttribLPointer(GLuint index, GLint size, GLenum type, GLsizei stride, const GLvoid * pointer)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttribLPointer);
   struct marshal_cmd_VertexAttribLPointer *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttribLPointer, cmd_size);
   cmd->index = index;
   cmd->size = size;
   cmd->type = type;
   cmd->stride = stride;
   cmd->pointer = pointer;
   if (COMPAT) _mesa_glthread_AttribPointer(ctx, VERT_ATTRIB_GENERIC(index));
}


/* GetVertexAttribLdv: marshalled synchronously */
void GLAPIENTRY
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
void
_mesa_unmarshal_VertexArrayVertexAttribLOffsetEXT(struct gl_context *ctx, const struct marshal_cmd_VertexArrayVertexAttribLOffsetEXT *cmd)
{
   const GLuint vaobj = cmd->vaobj;
   const GLuint buffer = cmd->buffer;
   const GLuint index = cmd->index;
   const GLint size = cmd->size;
   const GLenum type = cmd->type;
   const GLsizei stride = cmd->stride;
   const GLintptr offset = cmd->offset;
   CALL_VertexArrayVertexAttribLOffsetEXT(ctx->CurrentServerDispatch, (vaobj, buffer, index, size, type, stride, offset));
}
void GLAPIENTRY
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
   cmd->type = type;
   cmd->stride = stride;
   cmd->offset = offset;
}


/* GetShaderPrecisionFormat: marshalled synchronously */
void GLAPIENTRY
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
void
_mesa_unmarshal_ReleaseShaderCompiler(struct gl_context *ctx, const struct marshal_cmd_ReleaseShaderCompiler *cmd)
{
   CALL_ReleaseShaderCompiler(ctx->CurrentServerDispatch, ());
}
void GLAPIENTRY
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
void
_mesa_unmarshal_ShaderBinary(struct gl_context *ctx, const struct marshal_cmd_ShaderBinary *cmd)
{
   const GLsizei n = cmd->n;
   const GLenum binaryformat = cmd->binaryformat;
   const GLsizei length = cmd->length;
   GLuint * shaders;
   GLvoid * binary;
   const char *variable_data = (const char *) (cmd + 1);
   shaders = (GLuint *) variable_data;
   variable_data += n * 1 * sizeof(GLuint);
   binary = (GLvoid *) variable_data;
   CALL_ShaderBinary(ctx->CurrentServerDispatch, (n, shaders, binaryformat, binary, length));
}
void GLAPIENTRY
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
   cmd->binaryformat = binaryformat;
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
void
_mesa_unmarshal_ClearDepthf(struct gl_context *ctx, const struct marshal_cmd_ClearDepthf *cmd)
{
   const GLclampf depth = cmd->depth;
   CALL_ClearDepthf(ctx->CurrentServerDispatch, (depth));
}
void GLAPIENTRY
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
void
_mesa_unmarshal_DepthRangef(struct gl_context *ctx, const struct marshal_cmd_DepthRangef *cmd)
{
   const GLclampf zNear = cmd->zNear;
   const GLclampf zFar = cmd->zFar;
   CALL_DepthRangef(ctx->CurrentServerDispatch, (zNear, zFar));
}
void GLAPIENTRY
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
void GLAPIENTRY
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
void
_mesa_unmarshal_ProgramBinary(struct gl_context *ctx, const struct marshal_cmd_ProgramBinary *cmd)
{
   const GLuint program = cmd->program;
   const GLenum binaryFormat = cmd->binaryFormat;
   const GLsizei length = cmd->length;
   GLvoid * binary;
   const char *variable_data = (const char *) (cmd + 1);
   binary = (GLvoid *) variable_data;
   CALL_ProgramBinary(ctx->CurrentServerDispatch, (program, binaryFormat, binary, length));
}
void GLAPIENTRY
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
   cmd->binaryFormat = binaryFormat;
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
void
_mesa_unmarshal_ProgramParameteri(struct gl_context *ctx, const struct marshal_cmd_ProgramParameteri *cmd)
{
   const GLuint program = cmd->program;
   const GLenum pname = cmd->pname;
   const GLint value = cmd->value;
   CALL_ProgramParameteri(ctx->CurrentServerDispatch, (program, pname, value));
}
void GLAPIENTRY
_mesa_marshal_ProgramParameteri(GLuint program, GLenum pname, GLint value)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ProgramParameteri);
   struct marshal_cmd_ProgramParameteri *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ProgramParameteri, cmd_size);
   cmd->program = program;
   cmd->pname = pname;
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
void
_mesa_unmarshal_DebugMessageControl(struct gl_context *ctx, const struct marshal_cmd_DebugMessageControl *cmd)
{
   const GLenum source = cmd->source;
   const GLenum type = cmd->type;
   const GLenum severity = cmd->severity;
   const GLsizei count = cmd->count;
   const GLboolean enabled = cmd->enabled;
   GLuint * ids;
   const char *variable_data = (const char *) (cmd + 1);
   ids = (GLuint *) variable_data;
   CALL_DebugMessageControl(ctx->CurrentServerDispatch, (source, type, severity, count, ids, enabled));
}
void GLAPIENTRY
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
   cmd->source = source;
   cmd->type = type;
   cmd->severity = severity;
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
void
_mesa_unmarshal_DebugMessageInsert(struct gl_context *ctx, const struct marshal_cmd_DebugMessageInsert *cmd)
{
   const GLenum source = cmd->source;
   const GLenum type = cmd->type;
   const GLuint id = cmd->id;
   const GLenum severity = cmd->severity;
   const GLsizei length = cmd->length;
   GLchar * buf;
   const char *variable_data = (const char *) (cmd + 1);
   buf = (GLchar *) variable_data;
   CALL_DebugMessageInsert(ctx->CurrentServerDispatch, (source, type, id, severity, length, buf));
}
void GLAPIENTRY
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
   cmd->source = source;
   cmd->type = type;
   cmd->id = id;
   cmd->severity = severity;
   cmd->length = length;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, buf, buf_size);
}


/* DebugMessageCallback: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_DebugMessageCallback(GLDEBUGPROC callback, const GLvoid * userParam)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "DebugMessageCallback");
   CALL_DebugMessageCallback(ctx->CurrentServerDispatch, (callback, userParam));
}


/* GetDebugMessageLog: marshalled synchronously */
GLuint GLAPIENTRY
_mesa_marshal_GetDebugMessageLog(GLuint count, GLsizei bufsize, GLenum * sources, GLenum * types, GLuint * ids, GLenum * severities, GLsizei * lengths, GLchar * messageLog)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetDebugMessageLog");
   return CALL_GetDebugMessageLog(ctx->CurrentServerDispatch, (count, bufsize, sources, types, ids, severities, lengths, messageLog));
}


/* GetGraphicsResetStatusARB: marshalled synchronously */
GLenum GLAPIENTRY
_mesa_marshal_GetGraphicsResetStatusARB(void)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetGraphicsResetStatusARB");
   return CALL_GetGraphicsResetStatusARB(ctx->CurrentServerDispatch, ());
}


/* GetnMapdvARB: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetnMapdvARB(GLenum target, GLenum query, GLsizei bufSize, GLdouble * v)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetnMapdvARB");
   CALL_GetnMapdvARB(ctx->CurrentServerDispatch, (target, query, bufSize, v));
}


/* GetnMapfvARB: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetnMapfvARB(GLenum target, GLenum query, GLsizei bufSize, GLfloat * v)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetnMapfvARB");
   CALL_GetnMapfvARB(ctx->CurrentServerDispatch, (target, query, bufSize, v));
}


/* GetnMapivARB: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetnMapivARB(GLenum target, GLenum query, GLsizei bufSize, GLint * v)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetnMapivARB");
   CALL_GetnMapivARB(ctx->CurrentServerDispatch, (target, query, bufSize, v));
}


/* GetnPixelMapfvARB: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetnPixelMapfvARB(GLenum map, GLsizei bufSize, GLfloat * values)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetnPixelMapfvARB");
   CALL_GetnPixelMapfvARB(ctx->CurrentServerDispatch, (map, bufSize, values));
}


/* GetnPixelMapuivARB: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetnPixelMapuivARB(GLenum map, GLsizei bufSize, GLuint * values)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetnPixelMapuivARB");
   CALL_GetnPixelMapuivARB(ctx->CurrentServerDispatch, (map, bufSize, values));
}


/* GetnPixelMapusvARB: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetnPixelMapusvARB(GLenum map, GLsizei bufSize, GLushort * values)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetnPixelMapusvARB");
   CALL_GetnPixelMapusvARB(ctx->CurrentServerDispatch, (map, bufSize, values));
}


/* GetnPolygonStippleARB: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetnPolygonStippleARB(GLsizei bufSize, GLubyte * pattern)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetnPolygonStippleARB");
   CALL_GetnPolygonStippleARB(ctx->CurrentServerDispatch, (bufSize, pattern));
}


/* GetnTexImageARB: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetnTexImageARB(GLenum target, GLint level, GLenum format, GLenum type, GLsizei bufSize, GLvoid * img)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetnTexImageARB");
   CALL_GetnTexImageARB(ctx->CurrentServerDispatch, (target, level, format, type, bufSize, img));
}


/* ReadnPixelsARB: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_ReadnPixelsARB(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLsizei bufSize, GLvoid * data)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "ReadnPixelsARB");
   CALL_ReadnPixelsARB(ctx->CurrentServerDispatch, (x, y, width, height, format, type, bufSize, data));
}


/* GetnColorTableARB: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetnColorTableARB(GLenum target, GLenum format, GLenum type, GLsizei bufSize, GLvoid * table)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetnColorTableARB");
   CALL_GetnColorTableARB(ctx->CurrentServerDispatch, (target, format, type, bufSize, table));
}


/* GetnConvolutionFilterARB: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetnConvolutionFilterARB(GLenum target, GLenum format, GLenum type, GLsizei bufSize, GLvoid * image)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetnConvolutionFilterARB");
   CALL_GetnConvolutionFilterARB(ctx->CurrentServerDispatch, (target, format, type, bufSize, image));
}


/* GetnSeparableFilterARB: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetnSeparableFilterARB(GLenum target, GLenum format, GLenum type, GLsizei rowBufSize, GLvoid * row, GLsizei columnBufSize, GLvoid * column, GLvoid * span)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetnSeparableFilterARB");
   CALL_GetnSeparableFilterARB(ctx->CurrentServerDispatch, (target, format, type, rowBufSize, row, columnBufSize, column, span));
}


/* GetnHistogramARB: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetnHistogramARB(GLenum target, GLboolean reset, GLenum format, GLenum type, GLsizei bufSize, GLvoid * values)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetnHistogramARB");
   CALL_GetnHistogramARB(ctx->CurrentServerDispatch, (target, reset, format, type, bufSize, values));
}


/* GetnMinmaxARB: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetnMinmaxARB(GLenum target, GLboolean reset, GLenum format, GLenum type, GLsizei bufSize, GLvoid * values)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetnMinmaxARB");
   CALL_GetnMinmaxARB(ctx->CurrentServerDispatch, (target, reset, format, type, bufSize, values));
}


/* GetnCompressedTexImageARB: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetnCompressedTexImageARB(GLenum target, GLint lod, GLsizei bufSize, GLvoid * img)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetnCompressedTexImageARB");
   CALL_GetnCompressedTexImageARB(ctx->CurrentServerDispatch, (target, lod, bufSize, img));
}


/* GetnUniformfvARB: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetnUniformfvARB(GLuint program, GLint location, GLsizei bufSize, GLfloat * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetnUniformfvARB");
   CALL_GetnUniformfvARB(ctx->CurrentServerDispatch, (program, location, bufSize, params));
}


/* GetnUniformivARB: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetnUniformivARB(GLuint program, GLint location, GLsizei bufSize, GLint * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetnUniformivARB");
   CALL_GetnUniformivARB(ctx->CurrentServerDispatch, (program, location, bufSize, params));
}


/* GetnUniformuivARB: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetnUniformuivARB(GLuint program, GLint location, GLsizei bufSize, GLuint * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetnUniformuivARB");
   CALL_GetnUniformuivARB(ctx->CurrentServerDispatch, (program, location, bufSize, params));
}


/* GetnUniformdvARB: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetnUniformdvARB(GLuint program, GLint location, GLsizei bufSize, GLdouble * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetnUniformdvARB");
   CALL_GetnUniformdvARB(ctx->CurrentServerDispatch, (program, location, bufSize, params));
}


/* DrawArraysInstancedBaseInstance: marshalled asynchronously */
struct marshal_cmd_DrawArraysInstancedBaseInstance
{
   struct marshal_cmd_base cmd_base;
   GLenum16 mode;
   GLint first;
   GLsizei count;
   GLsizei primcount;
   GLuint baseinstance;
};
void
_mesa_unmarshal_DrawArraysInstancedBaseInstance(struct gl_context *ctx, const struct marshal_cmd_DrawArraysInstancedBaseInstance *cmd)
{
   const GLenum mode = cmd->mode;
   const GLint first = cmd->first;
   const GLsizei count = cmd->count;
   const GLsizei primcount = cmd->primcount;
   const GLuint baseinstance = cmd->baseinstance;
   CALL_DrawArraysInstancedBaseInstance(ctx->CurrentServerDispatch, (mode, first, count, primcount, baseinstance));
}
void GLAPIENTRY
_mesa_marshal_DrawArraysInstancedBaseInstance(GLenum mode, GLint first, GLsizei count, GLsizei primcount, GLuint baseinstance)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_DrawArraysInstancedBaseInstance);
   struct marshal_cmd_DrawArraysInstancedBaseInstance *cmd;
   if (_mesa_glthread_has_non_vbo_vertices(ctx)) {
      _mesa_glthread_finish_before(ctx, "DrawArraysInstancedBaseInstance");
      CALL_DrawArraysInstancedBaseInstance(ctx->CurrentServerDispatch, (mode, first, count, primcount, baseinstance));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_DrawArraysInstancedBaseInstance, cmd_size);
   cmd->mode = mode;
   cmd->first = first;
   cmd->count = count;
   cmd->primcount = primcount;
   cmd->baseinstance = baseinstance;
}


/* DrawElementsInstancedBaseInstance: marshalled asynchronously */
struct marshal_cmd_DrawElementsInstancedBaseInstance
{
   struct marshal_cmd_base cmd_base;
   GLenum16 mode;
   GLenum16 type;
   GLsizei count;
   GLsizei primcount;
   GLuint baseinstance;
   const GLvoid * indices;
};
void
_mesa_unmarshal_DrawElementsInstancedBaseInstance(struct gl_context *ctx, const struct marshal_cmd_DrawElementsInstancedBaseInstance *cmd)
{
   const GLenum mode = cmd->mode;
   const GLsizei count = cmd->count;
   const GLenum type = cmd->type;
   const GLvoid * indices = cmd->indices;
   const GLsizei primcount = cmd->primcount;
   const GLuint baseinstance = cmd->baseinstance;
   CALL_DrawElementsInstancedBaseInstance(ctx->CurrentServerDispatch, (mode, count, type, indices, primcount, baseinstance));
}
void GLAPIENTRY
_mesa_marshal_DrawElementsInstancedBaseInstance(GLenum mode, GLsizei count, GLenum type, const GLvoid * indices, GLsizei primcount, GLuint baseinstance)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_DrawElementsInstancedBaseInstance);
   struct marshal_cmd_DrawElementsInstancedBaseInstance *cmd;
   if (_mesa_glthread_has_non_vbo_vertices_or_indices(ctx)) {
      _mesa_glthread_finish_before(ctx, "DrawElementsInstancedBaseInstance");
      CALL_DrawElementsInstancedBaseInstance(ctx->CurrentServerDispatch, (mode, count, type, indices, primcount, baseinstance));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_DrawElementsInstancedBaseInstance, cmd_size);
   cmd->mode = mode;
   cmd->count = count;
   cmd->type = type;
   cmd->indices = indices;
   cmd->primcount = primcount;
   cmd->baseinstance = baseinstance;
}


/* DrawElementsInstancedBaseVertexBaseInstance: marshalled asynchronously */
struct marshal_cmd_DrawElementsInstancedBaseVertexBaseInstance
{
   struct marshal_cmd_base cmd_base;
   GLenum16 mode;
   GLenum16 type;
   GLsizei count;
   GLsizei primcount;
   GLint basevertex;
   GLuint baseinstance;
   const GLvoid * indices;
};
void
_mesa_unmarshal_DrawElementsInstancedBaseVertexBaseInstance(struct gl_context *ctx, const struct marshal_cmd_DrawElementsInstancedBaseVertexBaseInstance *cmd)
{
   const GLenum mode = cmd->mode;
   const GLsizei count = cmd->count;
   const GLenum type = cmd->type;
   const GLvoid * indices = cmd->indices;
   const GLsizei primcount = cmd->primcount;
   const GLint basevertex = cmd->basevertex;
   const GLuint baseinstance = cmd->baseinstance;
   CALL_DrawElementsInstancedBaseVertexBaseInstance(ctx->CurrentServerDispatch, (mode, count, type, indices, primcount, basevertex, baseinstance));
}
void GLAPIENTRY
_mesa_marshal_DrawElementsInstancedBaseVertexBaseInstance(GLenum mode, GLsizei count, GLenum type, const GLvoid * indices, GLsizei primcount, GLint basevertex, GLuint baseinstance)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_DrawElementsInstancedBaseVertexBaseInstance);
   struct marshal_cmd_DrawElementsInstancedBaseVertexBaseInstance *cmd;
   if (_mesa_glthread_has_non_vbo_vertices_or_indices(ctx)) {
      _mesa_glthread_finish_before(ctx, "DrawElementsInstancedBaseVertexBaseInstance");
      CALL_DrawElementsInstancedBaseVertexBaseInstance(ctx->CurrentServerDispatch, (mode, count, type, indices, primcount, basevertex, baseinstance));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_DrawElementsInstancedBaseVertexBaseInstance, cmd_size);
   cmd->mode = mode;
   cmd->count = count;
   cmd->type = type;
   cmd->indices = indices;
   cmd->primcount = primcount;
   cmd->basevertex = basevertex;
   cmd->baseinstance = baseinstance;
}


/* DrawTransformFeedbackInstanced: marshalled asynchronously */
struct marshal_cmd_DrawTransformFeedbackInstanced
{
   struct marshal_cmd_base cmd_base;
   GLenum16 mode;
   GLuint id;
   GLsizei primcount;
};
void
_mesa_unmarshal_DrawTransformFeedbackInstanced(struct gl_context *ctx, const struct marshal_cmd_DrawTransformFeedbackInstanced *cmd)
{
   const GLenum mode = cmd->mode;
   const GLuint id = cmd->id;
   const GLsizei primcount = cmd->primcount;
   CALL_DrawTransformFeedbackInstanced(ctx->CurrentServerDispatch, (mode, id, primcount));
}
void GLAPIENTRY
_mesa_marshal_DrawTransformFeedbackInstanced(GLenum mode, GLuint id, GLsizei primcount)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_DrawTransformFeedbackInstanced);
   struct marshal_cmd_DrawTransformFeedbackInstanced *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_DrawTransformFeedbackInstanced, cmd_size);
   cmd->mode = mode;
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
void
_mesa_unmarshal_DrawTransformFeedbackStreamInstanced(struct gl_context *ctx, const struct marshal_cmd_DrawTransformFeedbackStreamInstanced *cmd)
{
   const GLenum mode = cmd->mode;
   const GLuint id = cmd->id;
   const GLuint stream = cmd->stream;
   const GLsizei primcount = cmd->primcount;
   CALL_DrawTransformFeedbackStreamInstanced(ctx->CurrentServerDispatch, (mode, id, stream, primcount));
}
void GLAPIENTRY
_mesa_marshal_DrawTransformFeedbackStreamInstanced(GLenum mode, GLuint id, GLuint stream, GLsizei primcount)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_DrawTransformFeedbackStreamInstanced);
   struct marshal_cmd_DrawTransformFeedbackStreamInstanced *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_DrawTransformFeedbackStreamInstanced, cmd_size);
   cmd->mode = mode;
   cmd->id = id;
   cmd->stream = stream;
   cmd->primcount = primcount;
}


/* GetInternalformativ: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetInternalformativ(GLenum target, GLenum internalformat, GLenum pname, GLsizei bufSize, GLint * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetInternalformativ");
   CALL_GetInternalformativ(ctx->CurrentServerDispatch, (target, internalformat, pname, bufSize, params));
}


/* GetActiveAtomicCounterBufferiv: marshalled synchronously */
void GLAPIENTRY
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
void
_mesa_unmarshal_BindImageTexture(struct gl_context *ctx, const struct marshal_cmd_BindImageTexture *cmd)
{
   const GLuint unit = cmd->unit;
   const GLuint texture = cmd->texture;
   const GLint level = cmd->level;
   const GLboolean layered = cmd->layered;
   const GLint layer = cmd->layer;
   const GLenum access = cmd->access;
   const GLenum format = cmd->format;
   CALL_BindImageTexture(ctx->CurrentServerDispatch, (unit, texture, level, layered, layer, access, format));
}
void GLAPIENTRY
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
   cmd->access = access;
   cmd->format = format;
}


/* MemoryBarrier: marshalled asynchronously */
struct marshal_cmd_MemoryBarrier
{
   struct marshal_cmd_base cmd_base;
   GLbitfield barriers;
};
void
_mesa_unmarshal_MemoryBarrier(struct gl_context *ctx, const struct marshal_cmd_MemoryBarrier *cmd)
{
   const GLbitfield barriers = cmd->barriers;
   CALL_MemoryBarrier(ctx->CurrentServerDispatch, (barriers));
}
void GLAPIENTRY
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
void
_mesa_unmarshal_TexStorage1D(struct gl_context *ctx, const struct marshal_cmd_TexStorage1D *cmd)
{
   const GLenum target = cmd->target;
   const GLsizei levels = cmd->levels;
   const GLenum internalFormat = cmd->internalFormat;
   const GLsizei width = cmd->width;
   CALL_TexStorage1D(ctx->CurrentServerDispatch, (target, levels, internalFormat, width));
}
void GLAPIENTRY
_mesa_marshal_TexStorage1D(GLenum target, GLsizei levels, GLenum internalFormat, GLsizei width)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TexStorage1D);
   struct marshal_cmd_TexStorage1D *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexStorage1D, cmd_size);
   cmd->target = target;
   cmd->levels = levels;
   cmd->internalFormat = internalFormat;
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
void
_mesa_unmarshal_TexStorage2D(struct gl_context *ctx, const struct marshal_cmd_TexStorage2D *cmd)
{
   const GLenum target = cmd->target;
   const GLsizei levels = cmd->levels;
   const GLenum internalFormat = cmd->internalFormat;
   const GLsizei width = cmd->width;
   const GLsizei height = cmd->height;
   CALL_TexStorage2D(ctx->CurrentServerDispatch, (target, levels, internalFormat, width, height));
}
void GLAPIENTRY
_mesa_marshal_TexStorage2D(GLenum target, GLsizei levels, GLenum internalFormat, GLsizei width, GLsizei height)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TexStorage2D);
   struct marshal_cmd_TexStorage2D *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexStorage2D, cmd_size);
   cmd->target = target;
   cmd->levels = levels;
   cmd->internalFormat = internalFormat;
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
void
_mesa_unmarshal_TexStorage3D(struct gl_context *ctx, const struct marshal_cmd_TexStorage3D *cmd)
{
   const GLenum target = cmd->target;
   const GLsizei levels = cmd->levels;
   const GLenum internalFormat = cmd->internalFormat;
   const GLsizei width = cmd->width;
   const GLsizei height = cmd->height;
   const GLsizei depth = cmd->depth;
   CALL_TexStorage3D(ctx->CurrentServerDispatch, (target, levels, internalFormat, width, height, depth));
}
void GLAPIENTRY
_mesa_marshal_TexStorage3D(GLenum target, GLsizei levels, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TexStorage3D);
   struct marshal_cmd_TexStorage3D *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexStorage3D, cmd_size);
   cmd->target = target;
   cmd->levels = levels;
   cmd->internalFormat = internalFormat;
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
void
_mesa_unmarshal_TextureStorage1DEXT(struct gl_context *ctx, const struct marshal_cmd_TextureStorage1DEXT *cmd)
{
   const GLuint texture = cmd->texture;
   const GLenum target = cmd->target;
   const GLsizei levels = cmd->levels;
   const GLenum internalFormat = cmd->internalFormat;
   const GLsizei width = cmd->width;
   CALL_TextureStorage1DEXT(ctx->CurrentServerDispatch, (texture, target, levels, internalFormat, width));
}
void GLAPIENTRY
_mesa_marshal_TextureStorage1DEXT(GLuint texture, GLenum target, GLsizei levels, GLenum internalFormat, GLsizei width)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TextureStorage1DEXT);
   struct marshal_cmd_TextureStorage1DEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TextureStorage1DEXT, cmd_size);
   cmd->texture = texture;
   cmd->target = target;
   cmd->levels = levels;
   cmd->internalFormat = internalFormat;
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
void
_mesa_unmarshal_TextureStorage2DEXT(struct gl_context *ctx, const struct marshal_cmd_TextureStorage2DEXT *cmd)
{
   const GLuint texture = cmd->texture;
   const GLenum target = cmd->target;
   const GLsizei levels = cmd->levels;
   const GLenum internalFormat = cmd->internalFormat;
   const GLsizei width = cmd->width;
   const GLsizei height = cmd->height;
   CALL_TextureStorage2DEXT(ctx->CurrentServerDispatch, (texture, target, levels, internalFormat, width, height));
}
void GLAPIENTRY
_mesa_marshal_TextureStorage2DEXT(GLuint texture, GLenum target, GLsizei levels, GLenum internalFormat, GLsizei width, GLsizei height)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TextureStorage2DEXT);
   struct marshal_cmd_TextureStorage2DEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TextureStorage2DEXT, cmd_size);
   cmd->texture = texture;
   cmd->target = target;
   cmd->levels = levels;
   cmd->internalFormat = internalFormat;
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
void
_mesa_unmarshal_TextureStorage3DEXT(struct gl_context *ctx, const struct marshal_cmd_TextureStorage3DEXT *cmd)
{
   const GLuint texture = cmd->texture;
   const GLenum target = cmd->target;
   const GLsizei levels = cmd->levels;
   const GLenum internalFormat = cmd->internalFormat;
   const GLsizei width = cmd->width;
   const GLsizei height = cmd->height;
   const GLsizei depth = cmd->depth;
   CALL_TextureStorage3DEXT(ctx->CurrentServerDispatch, (texture, target, levels, internalFormat, width, height, depth));
}
void GLAPIENTRY
_mesa_marshal_TextureStorage3DEXT(GLuint texture, GLenum target, GLsizei levels, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TextureStorage3DEXT);
   struct marshal_cmd_TextureStorage3DEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TextureStorage3DEXT, cmd_size);
   cmd->texture = texture;
   cmd->target = target;
   cmd->levels = levels;
   cmd->internalFormat = internalFormat;
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
void
_mesa_unmarshal_PushDebugGroup(struct gl_context *ctx, const struct marshal_cmd_PushDebugGroup *cmd)
{
   const GLenum source = cmd->source;
   const GLuint id = cmd->id;
   const GLsizei length = cmd->length;
   GLchar * message;
   const char *variable_data = (const char *) (cmd + 1);
   message = (GLchar *) variable_data;
   CALL_PushDebugGroup(ctx->CurrentServerDispatch, (source, id, length, message));
}
void GLAPIENTRY
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
   cmd->source = source;
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
void
_mesa_unmarshal_PopDebugGroup(struct gl_context *ctx, const struct marshal_cmd_PopDebugGroup *cmd)
{
   CALL_PopDebugGroup(ctx->CurrentServerDispatch, ());
}
void GLAPIENTRY
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
void
_mesa_unmarshal_ObjectLabel(struct gl_context *ctx, const struct marshal_cmd_ObjectLabel *cmd)
{
   const GLenum identifier = cmd->identifier;
   const GLuint name = cmd->name;
   const GLsizei length = cmd->length;
   GLchar * label;
   const char *variable_data = (const char *) (cmd + 1);
   label = (GLchar *) variable_data;
   CALL_ObjectLabel(ctx->CurrentServerDispatch, (identifier, name, length, label));
}
void GLAPIENTRY
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
   cmd->identifier = identifier;
   cmd->name = name;
   cmd->length = length;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, label, label_size);
}


/* GetObjectLabel: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetObjectLabel(GLenum identifier, GLuint name, GLsizei bufSize, GLsizei * length, GLchar * label)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetObjectLabel");
   CALL_GetObjectLabel(ctx->CurrentServerDispatch, (identifier, name, bufSize, length, label));
}


/* ObjectPtrLabel: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_ObjectPtrLabel(const GLvoid * ptr, GLsizei length, const GLchar * label)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "ObjectPtrLabel");
   CALL_ObjectPtrLabel(ctx->CurrentServerDispatch, (ptr, length, label));
}


/* GetObjectPtrLabel: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetObjectPtrLabel(const GLvoid * ptr, GLsizei bufSize, GLsizei * length, GLchar * label)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetObjectPtrLabel");
   CALL_GetObjectPtrLabel(ctx->CurrentServerDispatch, (ptr, bufSize, length, label));
}


/* ClearBufferData: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_ClearBufferData(GLenum target, GLenum internalformat, GLenum format, GLenum type, const GLvoid * data)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "ClearBufferData");
   CALL_ClearBufferData(ctx->CurrentServerDispatch, (target, internalformat, format, type, data));
}


/* ClearBufferSubData: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_ClearBufferSubData(GLenum target, GLenum internalformat, GLintptr offset, GLsizeiptr size, GLenum format, GLenum type, const GLvoid * data)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "ClearBufferSubData");
   CALL_ClearBufferSubData(ctx->CurrentServerDispatch, (target, internalformat, offset, size, format, type, data));
}


/* ClearNamedBufferDataEXT: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_ClearNamedBufferDataEXT(GLuint buffer, GLenum internalformat, GLenum format, GLenum type, const GLvoid * data)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "ClearNamedBufferDataEXT");
   CALL_ClearNamedBufferDataEXT(ctx->CurrentServerDispatch, (buffer, internalformat, format, type, data));
}


/* ClearNamedBufferSubDataEXT: marshalled synchronously */
void GLAPIENTRY
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
void
_mesa_unmarshal_DispatchCompute(struct gl_context *ctx, const struct marshal_cmd_DispatchCompute *cmd)
{
   const GLuint num_groups_x = cmd->num_groups_x;
   const GLuint num_groups_y = cmd->num_groups_y;
   const GLuint num_groups_z = cmd->num_groups_z;
   CALL_DispatchCompute(ctx->CurrentServerDispatch, (num_groups_x, num_groups_y, num_groups_z));
}
void GLAPIENTRY
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
void
_mesa_unmarshal_DispatchComputeIndirect(struct gl_context *ctx, const struct marshal_cmd_DispatchComputeIndirect *cmd)
{
   const GLintptr indirect = cmd->indirect;
   CALL_DispatchComputeIndirect(ctx->CurrentServerDispatch, (indirect));
}
void GLAPIENTRY
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
void
_mesa_unmarshal_CopyImageSubData(struct gl_context *ctx, const struct marshal_cmd_CopyImageSubData *cmd)
{
   const GLuint srcName = cmd->srcName;
   const GLenum srcTarget = cmd->srcTarget;
   const GLint srcLevel = cmd->srcLevel;
   const GLint srcX = cmd->srcX;
   const GLint srcY = cmd->srcY;
   const GLint srcZ = cmd->srcZ;
   const GLuint dstName = cmd->dstName;
   const GLenum dstTarget = cmd->dstTarget;
   const GLint dstLevel = cmd->dstLevel;
   const GLint dstX = cmd->dstX;
   const GLint dstY = cmd->dstY;
   const GLint dstZ = cmd->dstZ;
   const GLsizei srcWidth = cmd->srcWidth;
   const GLsizei srcHeight = cmd->srcHeight;
   const GLsizei srcDepth = cmd->srcDepth;
   CALL_CopyImageSubData(ctx->CurrentServerDispatch, (srcName, srcTarget, srcLevel, srcX, srcY, srcZ, dstName, dstTarget, dstLevel, dstX, dstY, dstZ, srcWidth, srcHeight, srcDepth));
}
void GLAPIENTRY
_mesa_marshal_CopyImageSubData(GLuint srcName, GLenum srcTarget, GLint srcLevel, GLint srcX, GLint srcY, GLint srcZ, GLuint dstName, GLenum dstTarget, GLint dstLevel, GLint dstX, GLint dstY, GLint dstZ, GLsizei srcWidth, GLsizei srcHeight, GLsizei srcDepth)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_CopyImageSubData);
   struct marshal_cmd_CopyImageSubData *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_CopyImageSubData, cmd_size);
   cmd->srcName = srcName;
   cmd->srcTarget = srcTarget;
   cmd->srcLevel = srcLevel;
   cmd->srcX = srcX;
   cmd->srcY = srcY;
   cmd->srcZ = srcZ;
   cmd->dstName = dstName;
   cmd->dstTarget = dstTarget;
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
void
_mesa_unmarshal_TextureView(struct gl_context *ctx, const struct marshal_cmd_TextureView *cmd)
{
   const GLuint texture = cmd->texture;
   const GLenum target = cmd->target;
   const GLuint origtexture = cmd->origtexture;
   const GLenum internalformat = cmd->internalformat;
   const GLuint minlevel = cmd->minlevel;
   const GLuint numlevels = cmd->numlevels;
   const GLuint minlayer = cmd->minlayer;
   const GLuint numlayers = cmd->numlayers;
   CALL_TextureView(ctx->CurrentServerDispatch, (texture, target, origtexture, internalformat, minlevel, numlevels, minlayer, numlayers));
}
void GLAPIENTRY
_mesa_marshal_TextureView(GLuint texture, GLenum target, GLuint origtexture, GLenum internalformat, GLuint minlevel, GLuint numlevels, GLuint minlayer, GLuint numlayers)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TextureView);
   struct marshal_cmd_TextureView *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TextureView, cmd_size);
   cmd->texture = texture;
   cmd->target = target;
   cmd->origtexture = origtexture;
   cmd->internalformat = internalformat;
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
void
_mesa_unmarshal_BindVertexBuffer(struct gl_context *ctx, const struct marshal_cmd_BindVertexBuffer *cmd)
{
   const GLuint bindingindex = cmd->bindingindex;
   const GLuint buffer = cmd->buffer;
   const GLintptr offset = cmd->offset;
   const GLsizei stride = cmd->stride;
   CALL_BindVertexBuffer(ctx->CurrentServerDispatch, (bindingindex, buffer, offset, stride));
}
void GLAPIENTRY
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
void
_mesa_unmarshal_VertexAttribFormat(struct gl_context *ctx, const struct marshal_cmd_VertexAttribFormat *cmd)
{
   const GLuint attribindex = cmd->attribindex;
   const GLint size = cmd->size;
   const GLenum type = cmd->type;
   const GLboolean normalized = cmd->normalized;
   const GLuint relativeoffset = cmd->relativeoffset;
   CALL_VertexAttribFormat(ctx->CurrentServerDispatch, (attribindex, size, type, normalized, relativeoffset));
}
void GLAPIENTRY
_mesa_marshal_VertexAttribFormat(GLuint attribindex, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttribFormat);
   struct marshal_cmd_VertexAttribFormat *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttribFormat, cmd_size);
   cmd->attribindex = attribindex;
   cmd->size = size;
   cmd->type = type;
   cmd->normalized = normalized;
   cmd->relativeoffset = relativeoffset;
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
void
_mesa_unmarshal_VertexAttribIFormat(struct gl_context *ctx, const struct marshal_cmd_VertexAttribIFormat *cmd)
{
   const GLuint attribindex = cmd->attribindex;
   const GLint size = cmd->size;
   const GLenum type = cmd->type;
   const GLuint relativeoffset = cmd->relativeoffset;
   CALL_VertexAttribIFormat(ctx->CurrentServerDispatch, (attribindex, size, type, relativeoffset));
}
void GLAPIENTRY
_mesa_marshal_VertexAttribIFormat(GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttribIFormat);
   struct marshal_cmd_VertexAttribIFormat *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttribIFormat, cmd_size);
   cmd->attribindex = attribindex;
   cmd->size = size;
   cmd->type = type;
   cmd->relativeoffset = relativeoffset;
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
void
_mesa_unmarshal_VertexAttribLFormat(struct gl_context *ctx, const struct marshal_cmd_VertexAttribLFormat *cmd)
{
   const GLuint attribindex = cmd->attribindex;
   const GLint size = cmd->size;
   const GLenum type = cmd->type;
   const GLuint relativeoffset = cmd->relativeoffset;
   CALL_VertexAttribLFormat(ctx->CurrentServerDispatch, (attribindex, size, type, relativeoffset));
}
void GLAPIENTRY
_mesa_marshal_VertexAttribLFormat(GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttribLFormat);
   struct marshal_cmd_VertexAttribLFormat *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttribLFormat, cmd_size);
   cmd->attribindex = attribindex;
   cmd->size = size;
   cmd->type = type;
   cmd->relativeoffset = relativeoffset;
}


/* VertexAttribBinding: marshalled asynchronously */
struct marshal_cmd_VertexAttribBinding
{
   struct marshal_cmd_base cmd_base;
   GLuint attribindex;
   GLuint bindingindex;
};
void
_mesa_unmarshal_VertexAttribBinding(struct gl_context *ctx, const struct marshal_cmd_VertexAttribBinding *cmd)
{
   const GLuint attribindex = cmd->attribindex;
   const GLuint bindingindex = cmd->bindingindex;
   CALL_VertexAttribBinding(ctx->CurrentServerDispatch, (attribindex, bindingindex));
}
void GLAPIENTRY
_mesa_marshal_VertexAttribBinding(GLuint attribindex, GLuint bindingindex)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttribBinding);
   struct marshal_cmd_VertexAttribBinding *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttribBinding, cmd_size);
   cmd->attribindex = attribindex;
   cmd->bindingindex = bindingindex;
}


/* VertexBindingDivisor: marshalled asynchronously */
struct marshal_cmd_VertexBindingDivisor
{
   struct marshal_cmd_base cmd_base;
   GLuint attribindex;
   GLuint divisor;
};
void
_mesa_unmarshal_VertexBindingDivisor(struct gl_context *ctx, const struct marshal_cmd_VertexBindingDivisor *cmd)
{
   const GLuint attribindex = cmd->attribindex;
   const GLuint divisor = cmd->divisor;
   CALL_VertexBindingDivisor(ctx->CurrentServerDispatch, (attribindex, divisor));
}
void GLAPIENTRY
_mesa_marshal_VertexBindingDivisor(GLuint attribindex, GLuint divisor)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexBindingDivisor);
   struct marshal_cmd_VertexBindingDivisor *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexBindingDivisor, cmd_size);
   cmd->attribindex = attribindex;
   cmd->divisor = divisor;
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
void
_mesa_unmarshal_VertexArrayBindVertexBufferEXT(struct gl_context *ctx, const struct marshal_cmd_VertexArrayBindVertexBufferEXT *cmd)
{
   const GLuint vaobj = cmd->vaobj;
   const GLuint bindingindex = cmd->bindingindex;
   const GLuint buffer = cmd->buffer;
   const GLintptr offset = cmd->offset;
   const GLsizei stride = cmd->stride;
   CALL_VertexArrayBindVertexBufferEXT(ctx->CurrentServerDispatch, (vaobj, bindingindex, buffer, offset, stride));
}
void GLAPIENTRY
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
void
_mesa_unmarshal_VertexArrayVertexAttribFormatEXT(struct gl_context *ctx, const struct marshal_cmd_VertexArrayVertexAttribFormatEXT *cmd)
{
   const GLuint vaobj = cmd->vaobj;
   const GLuint attribindex = cmd->attribindex;
   const GLint size = cmd->size;
   const GLenum type = cmd->type;
   const GLboolean normalized = cmd->normalized;
   const GLuint relativeoffset = cmd->relativeoffset;
   CALL_VertexArrayVertexAttribFormatEXT(ctx->CurrentServerDispatch, (vaobj, attribindex, size, type, normalized, relativeoffset));
}
void GLAPIENTRY
_mesa_marshal_VertexArrayVertexAttribFormatEXT(GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexArrayVertexAttribFormatEXT);
   struct marshal_cmd_VertexArrayVertexAttribFormatEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexArrayVertexAttribFormatEXT, cmd_size);
   cmd->vaobj = vaobj;
   cmd->attribindex = attribindex;
   cmd->size = size;
   cmd->type = type;
   cmd->normalized = normalized;
   cmd->relativeoffset = relativeoffset;
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
void
_mesa_unmarshal_VertexArrayVertexAttribIFormatEXT(struct gl_context *ctx, const struct marshal_cmd_VertexArrayVertexAttribIFormatEXT *cmd)
{
   const GLuint vaobj = cmd->vaobj;
   const GLuint attribindex = cmd->attribindex;
   const GLint size = cmd->size;
   const GLenum type = cmd->type;
   const GLuint relativeoffset = cmd->relativeoffset;
   CALL_VertexArrayVertexAttribIFormatEXT(ctx->CurrentServerDispatch, (vaobj, attribindex, size, type, relativeoffset));
}
void GLAPIENTRY
_mesa_marshal_VertexArrayVertexAttribIFormatEXT(GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexArrayVertexAttribIFormatEXT);
   struct marshal_cmd_VertexArrayVertexAttribIFormatEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexArrayVertexAttribIFormatEXT, cmd_size);
   cmd->vaobj = vaobj;
   cmd->attribindex = attribindex;
   cmd->size = size;
   cmd->type = type;
   cmd->relativeoffset = relativeoffset;
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
void
_mesa_unmarshal_VertexArrayVertexAttribLFormatEXT(struct gl_context *ctx, const struct marshal_cmd_VertexArrayVertexAttribLFormatEXT *cmd)
{
   const GLuint vaobj = cmd->vaobj;
   const GLuint attribindex = cmd->attribindex;
   const GLint size = cmd->size;
   const GLenum type = cmd->type;
   const GLuint relativeoffset = cmd->relativeoffset;
   CALL_VertexArrayVertexAttribLFormatEXT(ctx->CurrentServerDispatch, (vaobj, attribindex, size, type, relativeoffset));
}
void GLAPIENTRY
_mesa_marshal_VertexArrayVertexAttribLFormatEXT(GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexArrayVertexAttribLFormatEXT);
   struct marshal_cmd_VertexArrayVertexAttribLFormatEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexArrayVertexAttribLFormatEXT, cmd_size);
   cmd->vaobj = vaobj;
   cmd->attribindex = attribindex;
   cmd->size = size;
   cmd->type = type;
   cmd->relativeoffset = relativeoffset;
}


/* VertexArrayVertexAttribBindingEXT: marshalled asynchronously */
struct marshal_cmd_VertexArrayVertexAttribBindingEXT
{
   struct marshal_cmd_base cmd_base;
   GLuint vaobj;
   GLuint attribindex;
   GLuint bindingindex;
};
void
_mesa_unmarshal_VertexArrayVertexAttribBindingEXT(struct gl_context *ctx, const struct marshal_cmd_VertexArrayVertexAttribBindingEXT *cmd)
{
   const GLuint vaobj = cmd->vaobj;
   const GLuint attribindex = cmd->attribindex;
   const GLuint bindingindex = cmd->bindingindex;
   CALL_VertexArrayVertexAttribBindingEXT(ctx->CurrentServerDispatch, (vaobj, attribindex, bindingindex));
}
void GLAPIENTRY
_mesa_marshal_VertexArrayVertexAttribBindingEXT(GLuint vaobj, GLuint attribindex, GLuint bindingindex)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexArrayVertexAttribBindingEXT);
   struct marshal_cmd_VertexArrayVertexAttribBindingEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexArrayVertexAttribBindingEXT, cmd_size);
   cmd->vaobj = vaobj;
   cmd->attribindex = attribindex;
   cmd->bindingindex = bindingindex;
}


/* VertexArrayVertexBindingDivisorEXT: marshalled asynchronously */
struct marshal_cmd_VertexArrayVertexBindingDivisorEXT
{
   struct marshal_cmd_base cmd_base;
   GLuint vaobj;
   GLuint attribindex;
   GLuint divisor;
};
void
_mesa_unmarshal_VertexArrayVertexBindingDivisorEXT(struct gl_context *ctx, const struct marshal_cmd_VertexArrayVertexBindingDivisorEXT *cmd)
{
   const GLuint vaobj = cmd->vaobj;
   const GLuint attribindex = cmd->attribindex;
   const GLuint divisor = cmd->divisor;
   CALL_VertexArrayVertexBindingDivisorEXT(ctx->CurrentServerDispatch, (vaobj, attribindex, divisor));
}
void GLAPIENTRY
_mesa_marshal_VertexArrayVertexBindingDivisorEXT(GLuint vaobj, GLuint attribindex, GLuint divisor)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexArrayVertexBindingDivisorEXT);
   struct marshal_cmd_VertexArrayVertexBindingDivisorEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexArrayVertexBindingDivisorEXT, cmd_size);
   cmd->vaobj = vaobj;
   cmd->attribindex = attribindex;
   cmd->divisor = divisor;
}


/* FramebufferParameteri: marshalled asynchronously */
struct marshal_cmd_FramebufferParameteri
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLenum16 pname;
   GLint param;
};
void
_mesa_unmarshal_FramebufferParameteri(struct gl_context *ctx, const struct marshal_cmd_FramebufferParameteri *cmd)
{
   const GLenum target = cmd->target;
   const GLenum pname = cmd->pname;
   const GLint param = cmd->param;
   CALL_FramebufferParameteri(ctx->CurrentServerDispatch, (target, pname, param));
}
void GLAPIENTRY
_mesa_marshal_FramebufferParameteri(GLenum target, GLenum pname, GLint param)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_FramebufferParameteri);
   struct marshal_cmd_FramebufferParameteri *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_FramebufferParameteri, cmd_size);
   cmd->target = target;
   cmd->pname = pname;
   cmd->param = param;
}


/* GetFramebufferParameteriv: marshalled synchronously */
void GLAPIENTRY
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
void
_mesa_unmarshal_NamedFramebufferParameteriEXT(struct gl_context *ctx, const struct marshal_cmd_NamedFramebufferParameteriEXT *cmd)
{
   const GLuint framebuffer = cmd->framebuffer;
   const GLenum pname = cmd->pname;
   const GLint param = cmd->param;
   CALL_NamedFramebufferParameteriEXT(ctx->CurrentServerDispatch, (framebuffer, pname, param));
}
void GLAPIENTRY
_mesa_marshal_NamedFramebufferParameteriEXT(GLuint framebuffer, GLenum pname, GLint param)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_NamedFramebufferParameteriEXT);
   struct marshal_cmd_NamedFramebufferParameteriEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_NamedFramebufferParameteriEXT, cmd_size);
   cmd->framebuffer = framebuffer;
   cmd->pname = pname;
   cmd->param = param;
}


/* GetNamedFramebufferParameterivEXT: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetNamedFramebufferParameterivEXT(GLuint framebuffer, GLenum pname, GLint* params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetNamedFramebufferParameterivEXT");
   CALL_GetNamedFramebufferParameterivEXT(ctx->CurrentServerDispatch, (framebuffer, pname, params));
}


/* GetInternalformati64v: marshalled synchronously */
void GLAPIENTRY
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
void
_mesa_unmarshal_InvalidateTexSubImage(struct gl_context *ctx, const struct marshal_cmd_InvalidateTexSubImage *cmd)
{
   const GLuint texture = cmd->texture;
   const GLint level = cmd->level;
   const GLint xoffset = cmd->xoffset;
   const GLint yoffset = cmd->yoffset;
   const GLint zoffset = cmd->zoffset;
   const GLsizei width = cmd->width;
   const GLsizei height = cmd->height;
   const GLsizei depth = cmd->depth;
   CALL_InvalidateTexSubImage(ctx->CurrentServerDispatch, (texture, level, xoffset, yoffset, zoffset, width, height, depth));
}
void GLAPIENTRY
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
void
_mesa_unmarshal_InvalidateTexImage(struct gl_context *ctx, const struct marshal_cmd_InvalidateTexImage *cmd)
{
   const GLuint texture = cmd->texture;
   const GLint level = cmd->level;
   CALL_InvalidateTexImage(ctx->CurrentServerDispatch, (texture, level));
}
void GLAPIENTRY
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
void
_mesa_unmarshal_InvalidateBufferSubData(struct gl_context *ctx, const struct marshal_cmd_InvalidateBufferSubData *cmd)
{
   const GLuint buffer = cmd->buffer;
   const GLintptr offset = cmd->offset;
   const GLsizeiptr length = cmd->length;
   CALL_InvalidateBufferSubData(ctx->CurrentServerDispatch, (buffer, offset, length));
}
void GLAPIENTRY
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
void
_mesa_unmarshal_InvalidateBufferData(struct gl_context *ctx, const struct marshal_cmd_InvalidateBufferData *cmd)
{
   const GLuint buffer = cmd->buffer;
   CALL_InvalidateBufferData(ctx->CurrentServerDispatch, (buffer));
}
void GLAPIENTRY
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
void
_mesa_unmarshal_InvalidateSubFramebuffer(struct gl_context *ctx, const struct marshal_cmd_InvalidateSubFramebuffer *cmd)
{
   const GLenum target = cmd->target;
   const GLsizei numAttachments = cmd->numAttachments;
   const GLint x = cmd->x;
   const GLint y = cmd->y;
   const GLsizei width = cmd->width;
   const GLsizei height = cmd->height;
   GLenum * attachments;
   const char *variable_data = (const char *) (cmd + 1);
   attachments = (GLenum *) variable_data;
   CALL_InvalidateSubFramebuffer(ctx->CurrentServerDispatch, (target, numAttachments, attachments, x, y, width, height));
}
void GLAPIENTRY
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
   cmd->target = target;
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
void
_mesa_unmarshal_InvalidateFramebuffer(struct gl_context *ctx, const struct marshal_cmd_InvalidateFramebuffer *cmd)
{
   const GLenum target = cmd->target;
   const GLsizei numAttachments = cmd->numAttachments;
   GLenum * attachments;
   const char *variable_data = (const char *) (cmd + 1);
   attachments = (GLenum *) variable_data;
   CALL_InvalidateFramebuffer(ctx->CurrentServerDispatch, (target, numAttachments, attachments));
}
void GLAPIENTRY
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
   cmd->target = target;
   cmd->numAttachments = numAttachments;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, attachments, attachments_size);
}


/* GetProgramInterfaceiv: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetProgramInterfaceiv(GLuint program, GLenum programInterface, GLenum pname, GLint * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetProgramInterfaceiv");
   CALL_GetProgramInterfaceiv(ctx->CurrentServerDispatch, (program, programInterface, pname, params));
}


/* GetProgramResourceIndex: marshalled synchronously */
GLuint GLAPIENTRY
_mesa_marshal_GetProgramResourceIndex(GLuint program, GLenum programInterface, const GLchar * name)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetProgramResourceIndex");
   return CALL_GetProgramResourceIndex(ctx->CurrentServerDispatch, (program, programInterface, name));
}


/* GetProgramResourceName: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetProgramResourceName(GLuint program, GLenum programInterface, GLuint index, GLsizei  bufSize, GLsizei * length, GLchar * name)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetProgramResourceName");
   CALL_GetProgramResourceName(ctx->CurrentServerDispatch, (program, programInterface, index, bufSize, length, name));
}


/* GetProgramResourceiv: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetProgramResourceiv(GLuint program, GLenum programInterface, GLuint index, GLsizei  propCount, const GLenum * props, GLsizei  bufSize, GLsizei * length, GLint * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetProgramResourceiv");
   CALL_GetProgramResourceiv(ctx->CurrentServerDispatch, (program, programInterface, index, propCount, props, bufSize, length, params));
}


/* GetProgramResourceLocation: marshalled synchronously */
GLint GLAPIENTRY
_mesa_marshal_GetProgramResourceLocation(GLuint program, GLenum programInterface, const GLchar * name)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetProgramResourceLocation");
   return CALL_GetProgramResourceLocation(ctx->CurrentServerDispatch, (program, programInterface, name));
}


/* GetProgramResourceLocationIndex: marshalled synchronously */
GLint GLAPIENTRY
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
void
_mesa_unmarshal_ShaderStorageBlockBinding(struct gl_context *ctx, const struct marshal_cmd_ShaderStorageBlockBinding *cmd)
{
   const GLuint program = cmd->program;
   const GLuint shaderStorageBlockIndex = cmd->shaderStorageBlockIndex;
   const GLuint shaderStorageBlockBinding = cmd->shaderStorageBlockBinding;
   CALL_ShaderStorageBlockBinding(ctx->CurrentServerDispatch, (program, shaderStorageBlockIndex, shaderStorageBlockBinding));
}
void GLAPIENTRY
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
void
_mesa_unmarshal_TexBufferRange(struct gl_context *ctx, const struct marshal_cmd_TexBufferRange *cmd)
{
   const GLenum target = cmd->target;
   const GLenum internalformat = cmd->internalformat;
   const GLuint buffer = cmd->buffer;
   const GLintptr offset = cmd->offset;
   const GLsizeiptr size = cmd->size;
   CALL_TexBufferRange(ctx->CurrentServerDispatch, (target, internalformat, buffer, offset, size));
}
void GLAPIENTRY
_mesa_marshal_TexBufferRange(GLenum target, GLenum internalformat, GLuint buffer, GLintptr offset, GLsizeiptr size)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TexBufferRange);
   struct marshal_cmd_TexBufferRange *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexBufferRange, cmd_size);
   cmd->target = target;
   cmd->internalformat = internalformat;
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
void
_mesa_unmarshal_TextureBufferRangeEXT(struct gl_context *ctx, const struct marshal_cmd_TextureBufferRangeEXT *cmd)
{
   const GLuint texture = cmd->texture;
   const GLenum target = cmd->target;
   const GLenum internalformat = cmd->internalformat;
   const GLuint buffer = cmd->buffer;
   const GLintptr offset = cmd->offset;
   const GLsizeiptr size = cmd->size;
   CALL_TextureBufferRangeEXT(ctx->CurrentServerDispatch, (texture, target, internalformat, buffer, offset, size));
}
void GLAPIENTRY
_mesa_marshal_TextureBufferRangeEXT(GLuint texture, GLenum target, GLenum internalformat, GLuint buffer, GLintptr offset, GLsizeiptr size)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TextureBufferRangeEXT);
   struct marshal_cmd_TextureBufferRangeEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TextureBufferRangeEXT, cmd_size);
   cmd->texture = texture;
   cmd->target = target;
   cmd->internalformat = internalformat;
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
void
_mesa_unmarshal_TexStorage2DMultisample(struct gl_context *ctx, const struct marshal_cmd_TexStorage2DMultisample *cmd)
{
   const GLenum target = cmd->target;
   const GLsizei samples = cmd->samples;
   const GLenum internalformat = cmd->internalformat;
   const GLsizei width = cmd->width;
   const GLsizei height = cmd->height;
   const GLboolean fixedsamplelocations = cmd->fixedsamplelocations;
   CALL_TexStorage2DMultisample(ctx->CurrentServerDispatch, (target, samples, internalformat, width, height, fixedsamplelocations));
}
void GLAPIENTRY
_mesa_marshal_TexStorage2DMultisample(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TexStorage2DMultisample);
   struct marshal_cmd_TexStorage2DMultisample *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexStorage2DMultisample, cmd_size);
   cmd->target = target;
   cmd->samples = samples;
   cmd->internalformat = internalformat;
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
void
_mesa_unmarshal_TexStorage3DMultisample(struct gl_context *ctx, const struct marshal_cmd_TexStorage3DMultisample *cmd)
{
   const GLenum target = cmd->target;
   const GLsizei samples = cmd->samples;
   const GLenum internalformat = cmd->internalformat;
   const GLsizei width = cmd->width;
   const GLsizei height = cmd->height;
   const GLsizei depth = cmd->depth;
   const GLboolean fixedsamplelocations = cmd->fixedsamplelocations;
   CALL_TexStorage3DMultisample(ctx->CurrentServerDispatch, (target, samples, internalformat, width, height, depth, fixedsamplelocations));
}
void GLAPIENTRY
_mesa_marshal_TexStorage3DMultisample(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TexStorage3DMultisample);
   struct marshal_cmd_TexStorage3DMultisample *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexStorage3DMultisample, cmd_size);
   cmd->target = target;
   cmd->samples = samples;
   cmd->internalformat = internalformat;
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
void
_mesa_unmarshal_TextureStorage2DMultisampleEXT(struct gl_context *ctx, const struct marshal_cmd_TextureStorage2DMultisampleEXT *cmd)
{
   const GLuint texture = cmd->texture;
   const GLenum target = cmd->target;
   const GLsizei samples = cmd->samples;
   const GLenum internalformat = cmd->internalformat;
   const GLsizei width = cmd->width;
   const GLsizei height = cmd->height;
   const GLboolean fixedsamplelocations = cmd->fixedsamplelocations;
   CALL_TextureStorage2DMultisampleEXT(ctx->CurrentServerDispatch, (texture, target, samples, internalformat, width, height, fixedsamplelocations));
}
void GLAPIENTRY
_mesa_marshal_TextureStorage2DMultisampleEXT(GLuint texture, GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TextureStorage2DMultisampleEXT);
   struct marshal_cmd_TextureStorage2DMultisampleEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TextureStorage2DMultisampleEXT, cmd_size);
   cmd->texture = texture;
   cmd->target = target;
   cmd->samples = samples;
   cmd->internalformat = internalformat;
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
void
_mesa_unmarshal_TextureStorage3DMultisampleEXT(struct gl_context *ctx, const struct marshal_cmd_TextureStorage3DMultisampleEXT *cmd)
{
   const GLuint texture = cmd->texture;
   const GLenum target = cmd->target;
   const GLsizei samples = cmd->samples;
   const GLenum internalformat = cmd->internalformat;
   const GLsizei width = cmd->width;
   const GLsizei height = cmd->height;
   const GLsizei depth = cmd->depth;
   const GLboolean fixedsamplelocations = cmd->fixedsamplelocations;
   CALL_TextureStorage3DMultisampleEXT(ctx->CurrentServerDispatch, (texture, target, samples, internalformat, width, height, depth, fixedsamplelocations));
}
void GLAPIENTRY
_mesa_marshal_TextureStorage3DMultisampleEXT(GLuint texture, GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TextureStorage3DMultisampleEXT);
   struct marshal_cmd_TextureStorage3DMultisampleEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TextureStorage3DMultisampleEXT, cmd_size);
   cmd->texture = texture;
   cmd->target = target;
   cmd->samples = samples;
   cmd->internalformat = internalformat;
   cmd->width = width;
   cmd->height = height;
   cmd->depth = depth;
   cmd->fixedsamplelocations = fixedsamplelocations;
}


/* BufferStorage: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_BufferStorage(GLenum target, GLsizeiptr size, const GLvoid * data, GLbitfield flags)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "BufferStorage");
   CALL_BufferStorage(ctx->CurrentServerDispatch, (target, size, data, flags));
}


/* NamedBufferStorageEXT: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_NamedBufferStorageEXT(GLuint buffer, GLsizeiptr size, const GLvoid * data, GLbitfield flags)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "NamedBufferStorageEXT");
   CALL_NamedBufferStorageEXT(ctx->CurrentServerDispatch, (buffer, size, data, flags));
}


/* ClearTexImage: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_ClearTexImage(GLuint texture, GLint level, GLenum format, GLenum type, const GLvoid * data)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "ClearTexImage");
   CALL_ClearTexImage(ctx->CurrentServerDispatch, (texture, level, format, type, data));
}


/* ClearTexSubImage: marshalled synchronously */
void GLAPIENTRY
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
void
_mesa_unmarshal_BindBuffersBase(struct gl_context *ctx, const struct marshal_cmd_BindBuffersBase *cmd)
{
   const GLenum target = cmd->target;
   const GLuint first = cmd->first;
   const GLsizei count = cmd->count;
   GLuint * buffers;
   const char *variable_data = (const char *) (cmd + 1);
   buffers = (GLuint *) variable_data;
   CALL_BindBuffersBase(ctx->CurrentServerDispatch, (target, first, count, buffers));
}
void GLAPIENTRY
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
   cmd->target = target;
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
void
_mesa_unmarshal_BindBuffersRange(struct gl_context *ctx, const struct marshal_cmd_BindBuffersRange *cmd)
{
   const GLenum target = cmd->target;
   const GLuint first = cmd->first;
   const GLsizei count = cmd->count;
   GLuint * buffers;
   GLintptr * offsets;
   GLsizeiptr * sizes;
   const char *variable_data = (const char *) (cmd + 1);
   buffers = (GLuint *) variable_data;
   variable_data += count * 1 * sizeof(GLuint);
   offsets = (GLintptr *) variable_data;
   variable_data += count * 1 * sizeof(GLintptr);
   sizes = (GLsizeiptr *) variable_data;
   CALL_BindBuffersRange(ctx->CurrentServerDispatch, (target, first, count, buffers, offsets, sizes));
}
void GLAPIENTRY
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
   cmd->target = target;
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
void
_mesa_unmarshal_BindTextures(struct gl_context *ctx, const struct marshal_cmd_BindTextures *cmd)
{
   const GLuint first = cmd->first;
   const GLsizei count = cmd->count;
   GLuint * textures;
   const char *variable_data = (const char *) (cmd + 1);
   textures = (GLuint *) variable_data;
   CALL_BindTextures(ctx->CurrentServerDispatch, (first, count, textures));
}
void GLAPIENTRY
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
void
_mesa_unmarshal_BindSamplers(struct gl_context *ctx, const struct marshal_cmd_BindSamplers *cmd)
{
   const GLuint first = cmd->first;
   const GLsizei count = cmd->count;
   GLuint * samplers;
   const char *variable_data = (const char *) (cmd + 1);
   samplers = (GLuint *) variable_data;
   CALL_BindSamplers(ctx->CurrentServerDispatch, (first, count, samplers));
}
void GLAPIENTRY
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
void
_mesa_unmarshal_BindImageTextures(struct gl_context *ctx, const struct marshal_cmd_BindImageTextures *cmd)
{
   const GLuint first = cmd->first;
   const GLsizei count = cmd->count;
   GLuint * textures;
   const char *variable_data = (const char *) (cmd + 1);
   textures = (GLuint *) variable_data;
   CALL_BindImageTextures(ctx->CurrentServerDispatch, (first, count, textures));
}
void GLAPIENTRY
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
void
_mesa_unmarshal_BindVertexBuffers(struct gl_context *ctx, const struct marshal_cmd_BindVertexBuffers *cmd)
{
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
   CALL_BindVertexBuffers(ctx->CurrentServerDispatch, (first, count, buffers, offsets, strides));
}
void GLAPIENTRY
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
GLuint64 GLAPIENTRY
_mesa_marshal_GetTextureHandleARB(GLuint texture)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetTextureHandleARB");
   return CALL_GetTextureHandleARB(ctx->CurrentServerDispatch, (texture));
}


/* GetTextureSamplerHandleARB: marshalled synchronously */
GLuint64 GLAPIENTRY
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
void
_mesa_unmarshal_MakeTextureHandleResidentARB(struct gl_context *ctx, const struct marshal_cmd_MakeTextureHandleResidentARB *cmd)
{
   const GLuint64 handle = cmd->handle;
   CALL_MakeTextureHandleResidentARB(ctx->CurrentServerDispatch, (handle));
}
void GLAPIENTRY
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
void
_mesa_unmarshal_MakeTextureHandleNonResidentARB(struct gl_context *ctx, const struct marshal_cmd_MakeTextureHandleNonResidentARB *cmd)
{
   const GLuint64 handle = cmd->handle;
   CALL_MakeTextureHandleNonResidentARB(ctx->CurrentServerDispatch, (handle));
}
void GLAPIENTRY
_mesa_marshal_MakeTextureHandleNonResidentARB(GLuint64 handle)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MakeTextureHandleNonResidentARB);
   struct marshal_cmd_MakeTextureHandleNonResidentARB *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MakeTextureHandleNonResidentARB, cmd_size);
   cmd->handle = handle;
}


/* GetImageHandleARB: marshalled synchronously */
GLuint64 GLAPIENTRY
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
void
_mesa_unmarshal_MakeImageHandleResidentARB(struct gl_context *ctx, const struct marshal_cmd_MakeImageHandleResidentARB *cmd)
{
   const GLuint64 handle = cmd->handle;
   const GLenum access = cmd->access;
   CALL_MakeImageHandleResidentARB(ctx->CurrentServerDispatch, (handle, access));
}
void GLAPIENTRY
_mesa_marshal_MakeImageHandleResidentARB(GLuint64 handle, GLenum access)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MakeImageHandleResidentARB);
   struct marshal_cmd_MakeImageHandleResidentARB *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MakeImageHandleResidentARB, cmd_size);
   cmd->handle = handle;
   cmd->access = access;
}


/* MakeImageHandleNonResidentARB: marshalled asynchronously */
struct marshal_cmd_MakeImageHandleNonResidentARB
{
   struct marshal_cmd_base cmd_base;
   GLuint64 handle;
};
void
_mesa_unmarshal_MakeImageHandleNonResidentARB(struct gl_context *ctx, const struct marshal_cmd_MakeImageHandleNonResidentARB *cmd)
{
   const GLuint64 handle = cmd->handle;
   CALL_MakeImageHandleNonResidentARB(ctx->CurrentServerDispatch, (handle));
}
void GLAPIENTRY
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
void
_mesa_unmarshal_UniformHandleui64ARB(struct gl_context *ctx, const struct marshal_cmd_UniformHandleui64ARB *cmd)
{
   const GLint location = cmd->location;
   const GLuint64 value = cmd->value;
   CALL_UniformHandleui64ARB(ctx->CurrentServerDispatch, (location, value));
}
void GLAPIENTRY
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
void
_mesa_unmarshal_UniformHandleui64vARB(struct gl_context *ctx, const struct marshal_cmd_UniformHandleui64vARB *cmd)
{
   const GLint location = cmd->location;
   const GLsizei count = cmd->count;
   GLuint64 * value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLuint64 *) variable_data;
   CALL_UniformHandleui64vARB(ctx->CurrentServerDispatch, (location, count, value));
}
void GLAPIENTRY
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
void
_mesa_unmarshal_ProgramUniformHandleui64ARB(struct gl_context *ctx, const struct marshal_cmd_ProgramUniformHandleui64ARB *cmd)
{
   const GLuint program = cmd->program;
   const GLint location = cmd->location;
   const GLuint64 value = cmd->value;
   CALL_ProgramUniformHandleui64ARB(ctx->CurrentServerDispatch, (program, location, value));
}
void GLAPIENTRY
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
void
_mesa_unmarshal_ProgramUniformHandleui64vARB(struct gl_context *ctx, const struct marshal_cmd_ProgramUniformHandleui64vARB *cmd)
{
   const GLuint program = cmd->program;
   const GLint location = cmd->location;
   const GLsizei count = cmd->count;
   GLuint64 * value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLuint64 *) variable_data;
   CALL_ProgramUniformHandleui64vARB(ctx->CurrentServerDispatch, (program, location, count, value));
}
void GLAPIENTRY
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
GLboolean GLAPIENTRY
_mesa_marshal_IsTextureHandleResidentARB(GLuint64 handle)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "IsTextureHandleResidentARB");
   return CALL_IsTextureHandleResidentARB(ctx->CurrentServerDispatch, (handle));
}


/* IsImageHandleResidentARB: marshalled synchronously */
GLboolean GLAPIENTRY
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
void
_mesa_unmarshal_VertexAttribL1ui64ARB(struct gl_context *ctx, const struct marshal_cmd_VertexAttribL1ui64ARB *cmd)
{
   const GLuint index = cmd->index;
   const GLuint64EXT x = cmd->x;
   CALL_VertexAttribL1ui64ARB(ctx->CurrentServerDispatch, (index, x));
}
void GLAPIENTRY
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
void
_mesa_unmarshal_VertexAttribL1ui64vARB(struct gl_context *ctx, const struct marshal_cmd_VertexAttribL1ui64vARB *cmd)
{
   const GLuint index = cmd->index;
   const GLuint64EXT * v = cmd->v;
   CALL_VertexAttribL1ui64vARB(ctx->CurrentServerDispatch, (index, v));
}
void GLAPIENTRY
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
void GLAPIENTRY
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
void
_mesa_unmarshal_DispatchComputeGroupSizeARB(struct gl_context *ctx, const struct marshal_cmd_DispatchComputeGroupSizeARB *cmd)
{
   const GLuint num_groups_x = cmd->num_groups_x;
   const GLuint num_groups_y = cmd->num_groups_y;
   const GLuint num_groups_z = cmd->num_groups_z;
   const GLuint group_size_x = cmd->group_size_x;
   const GLuint group_size_y = cmd->group_size_y;
   const GLuint group_size_z = cmd->group_size_z;
   CALL_DispatchComputeGroupSizeARB(ctx->CurrentServerDispatch, (num_groups_x, num_groups_y, num_groups_z, group_size_x, group_size_y, group_size_z));
}
void GLAPIENTRY
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
void
_mesa_unmarshal_MultiDrawArraysIndirectCountARB(struct gl_context *ctx, const struct marshal_cmd_MultiDrawArraysIndirectCountARB *cmd)
{
   const GLenum mode = cmd->mode;
   const GLintptr indirect = cmd->indirect;
   const GLintptr drawcount = cmd->drawcount;
   const GLsizei maxdrawcount = cmd->maxdrawcount;
   const GLsizei stride = cmd->stride;
   CALL_MultiDrawArraysIndirectCountARB(ctx->CurrentServerDispatch, (mode, indirect, drawcount, maxdrawcount, stride));
}
void GLAPIENTRY
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
   cmd->mode = mode;
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
void
_mesa_unmarshal_MultiDrawElementsIndirectCountARB(struct gl_context *ctx, const struct marshal_cmd_MultiDrawElementsIndirectCountARB *cmd)
{
   const GLenum mode = cmd->mode;
   const GLenum type = cmd->type;
   const GLintptr indirect = cmd->indirect;
   const GLintptr drawcount = cmd->drawcount;
   const GLsizei maxdrawcount = cmd->maxdrawcount;
   const GLsizei stride = cmd->stride;
   CALL_MultiDrawElementsIndirectCountARB(ctx->CurrentServerDispatch, (mode, type, indirect, drawcount, maxdrawcount, stride));
}
void GLAPIENTRY
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
   cmd->mode = mode;
   cmd->type = type;
   cmd->indirect = indirect;
   cmd->drawcount = drawcount;
   cmd->maxdrawcount = maxdrawcount;
   cmd->stride = stride;
}


/* ClipControl: marshalled asynchronously */
struct marshal_cmd_ClipControl
{
   struct marshal_cmd_base cmd_base;
   GLenum16 origin;
   GLenum16 depth;
};
void
_mesa_unmarshal_ClipControl(struct gl_context *ctx, const struct marshal_cmd_ClipControl *cmd)
{
   const GLenum origin = cmd->origin;
   const GLenum depth = cmd->depth;
   CALL_ClipControl(ctx->CurrentServerDispatch, (origin, depth));
}
void GLAPIENTRY
_mesa_marshal_ClipControl(GLenum origin, GLenum depth)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ClipControl);
   struct marshal_cmd_ClipControl *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ClipControl, cmd_size);
   cmd->origin = origin;
   cmd->depth = depth;
}


/* CreateTransformFeedbacks: marshalled synchronously */
void GLAPIENTRY
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
void
_mesa_unmarshal_TransformFeedbackBufferBase(struct gl_context *ctx, const struct marshal_cmd_TransformFeedbackBufferBase *cmd)
{
   const GLuint xfb = cmd->xfb;
   const GLuint index = cmd->index;
   const GLuint buffer = cmd->buffer;
   CALL_TransformFeedbackBufferBase(ctx->CurrentServerDispatch, (xfb, index, buffer));
}
void GLAPIENTRY
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
void
_mesa_unmarshal_TransformFeedbackBufferRange(struct gl_context *ctx, const struct marshal_cmd_TransformFeedbackBufferRange *cmd)
{
   const GLuint xfb = cmd->xfb;
   const GLuint index = cmd->index;
   const GLuint buffer = cmd->buffer;
   const GLintptr offset = cmd->offset;
   const GLsizeiptr size = cmd->size;
   CALL_TransformFeedbackBufferRange(ctx->CurrentServerDispatch, (xfb, index, buffer, offset, size));
}
void GLAPIENTRY
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
void GLAPIENTRY
_mesa_marshal_GetTransformFeedbackiv(GLuint xfb, GLenum pname, GLint * param)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetTransformFeedbackiv");
   CALL_GetTransformFeedbackiv(ctx->CurrentServerDispatch, (xfb, pname, param));
}


/* GetTransformFeedbacki_v: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetTransformFeedbacki_v(GLuint xfb, GLenum pname, GLuint index, GLint * param)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetTransformFeedbacki_v");
   CALL_GetTransformFeedbacki_v(ctx->CurrentServerDispatch, (xfb, pname, index, param));
}


/* GetTransformFeedbacki64_v: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetTransformFeedbacki64_v(GLuint xfb, GLenum pname, GLuint index, GLint64 * param)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetTransformFeedbacki64_v");
   CALL_GetTransformFeedbacki64_v(ctx->CurrentServerDispatch, (xfb, pname, index, param));
}


/* CreateBuffers: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_CreateBuffers(GLsizei n, GLuint * buffers)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "CreateBuffers");
   CALL_CreateBuffers(ctx->CurrentServerDispatch, (n, buffers));
}


/* NamedBufferStorage: marshalled synchronously */
void GLAPIENTRY
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
void
_mesa_unmarshal_CopyNamedBufferSubData(struct gl_context *ctx, const struct marshal_cmd_CopyNamedBufferSubData *cmd)
{
   const GLuint readBuffer = cmd->readBuffer;
   const GLuint writeBuffer = cmd->writeBuffer;
   const GLintptr readOffset = cmd->readOffset;
   const GLintptr writeOffset = cmd->writeOffset;
   const GLsizeiptr size = cmd->size;
   CALL_CopyNamedBufferSubData(ctx->CurrentServerDispatch, (readBuffer, writeBuffer, readOffset, writeOffset, size));
}
void GLAPIENTRY
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
void GLAPIENTRY
_mesa_marshal_ClearNamedBufferData(GLuint buffer, GLenum internalformat, GLenum format, GLenum type, const GLvoid * data)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "ClearNamedBufferData");
   CALL_ClearNamedBufferData(ctx->CurrentServerDispatch, (buffer, internalformat, format, type, data));
}


/* ClearNamedBufferSubData: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_ClearNamedBufferSubData(GLuint buffer, GLenum internalformat, GLintptr offset, GLsizeiptr size, GLenum format, GLenum type, const GLvoid * data)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "ClearNamedBufferSubData");
   CALL_ClearNamedBufferSubData(ctx->CurrentServerDispatch, (buffer, internalformat, offset, size, format, type, data));
}


/* MapNamedBuffer: marshalled synchronously */
GLvoid * GLAPIENTRY
_mesa_marshal_MapNamedBuffer(GLuint buffer, GLenum access)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "MapNamedBuffer");
   return CALL_MapNamedBuffer(ctx->CurrentServerDispatch, (buffer, access));
}


/* MapNamedBufferRange: marshalled synchronously */
GLvoid * GLAPIENTRY
_mesa_marshal_MapNamedBufferRange(GLuint buffer, GLintptr offset, GLsizeiptr length, GLbitfield access)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "MapNamedBufferRange");
   return CALL_MapNamedBufferRange(ctx->CurrentServerDispatch, (buffer, offset, length, access));
}


/* UnmapNamedBufferEXT: marshalled synchronously */
GLboolean GLAPIENTRY
_mesa_marshal_UnmapNamedBufferEXT(GLuint buffer)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "UnmapNamedBufferEXT");
   return CALL_UnmapNamedBufferEXT(ctx->CurrentServerDispatch, (buffer));
}


/* FlushMappedNamedBufferRange: marshalled asynchronously */
struct marshal_cmd_FlushMappedNamedBufferRange
{
   struct marshal_cmd_base cmd_base;
   GLuint buffer;
   GLintptr offset;
   GLsizeiptr length;
};
void
_mesa_unmarshal_FlushMappedNamedBufferRange(struct gl_context *ctx, const struct marshal_cmd_FlushMappedNamedBufferRange *cmd)
{
   const GLuint buffer = cmd->buffer;
   const GLintptr offset = cmd->offset;
   const GLsizeiptr length = cmd->length;
   CALL_FlushMappedNamedBufferRange(ctx->CurrentServerDispatch, (buffer, offset, length));
}
void GLAPIENTRY
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
void GLAPIENTRY
_mesa_marshal_GetNamedBufferParameteriv(GLuint buffer, GLenum pname, GLint * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetNamedBufferParameteriv");
   CALL_GetNamedBufferParameteriv(ctx->CurrentServerDispatch, (buffer, pname, params));
}


/* GetNamedBufferParameteri64v: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetNamedBufferParameteri64v(GLuint buffer, GLenum pname, GLint64 * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetNamedBufferParameteri64v");
   CALL_GetNamedBufferParameteri64v(ctx->CurrentServerDispatch, (buffer, pname, params));
}


/* GetNamedBufferPointerv: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetNamedBufferPointerv(GLuint buffer, GLenum pname, GLvoid ** params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetNamedBufferPointerv");
   CALL_GetNamedBufferPointerv(ctx->CurrentServerDispatch, (buffer, pname, params));
}


/* GetNamedBufferSubData: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetNamedBufferSubData(GLuint buffer, GLintptr offset, GLsizeiptr size, GLvoid * data)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetNamedBufferSubData");
   CALL_GetNamedBufferSubData(ctx->CurrentServerDispatch, (buffer, offset, size, data));
}


/* CreateFramebuffers: marshalled synchronously */
void GLAPIENTRY
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
void
_mesa_unmarshal_NamedFramebufferRenderbuffer(struct gl_context *ctx, const struct marshal_cmd_NamedFramebufferRenderbuffer *cmd)
{
   const GLuint framebuffer = cmd->framebuffer;
   const GLenum attachment = cmd->attachment;
   const GLenum renderbuffertarget = cmd->renderbuffertarget;
   const GLuint renderbuffer = cmd->renderbuffer;
   CALL_NamedFramebufferRenderbuffer(ctx->CurrentServerDispatch, (framebuffer, attachment, renderbuffertarget, renderbuffer));
}
void GLAPIENTRY
_mesa_marshal_NamedFramebufferRenderbuffer(GLuint framebuffer, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_NamedFramebufferRenderbuffer);
   struct marshal_cmd_NamedFramebufferRenderbuffer *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_NamedFramebufferRenderbuffer, cmd_size);
   cmd->framebuffer = framebuffer;
   cmd->attachment = attachment;
   cmd->renderbuffertarget = renderbuffertarget;
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
void
_mesa_unmarshal_NamedFramebufferParameteri(struct gl_context *ctx, const struct marshal_cmd_NamedFramebufferParameteri *cmd)
{
   const GLuint framebuffer = cmd->framebuffer;
   const GLenum pname = cmd->pname;
   const GLint param = cmd->param;
   CALL_NamedFramebufferParameteri(ctx->CurrentServerDispatch, (framebuffer, pname, param));
}
void GLAPIENTRY
_mesa_marshal_NamedFramebufferParameteri(GLuint framebuffer, GLenum pname, GLint param)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_NamedFramebufferParameteri);
   struct marshal_cmd_NamedFramebufferParameteri *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_NamedFramebufferParameteri, cmd_size);
   cmd->framebuffer = framebuffer;
   cmd->pname = pname;
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
void
_mesa_unmarshal_NamedFramebufferTexture(struct gl_context *ctx, const struct marshal_cmd_NamedFramebufferTexture *cmd)
{
   const GLuint framebuffer = cmd->framebuffer;
   const GLenum attachment = cmd->attachment;
   const GLuint texture = cmd->texture;
   const GLint level = cmd->level;
   CALL_NamedFramebufferTexture(ctx->CurrentServerDispatch, (framebuffer, attachment, texture, level));
}
void GLAPIENTRY
_mesa_marshal_NamedFramebufferTexture(GLuint framebuffer, GLenum attachment, GLuint texture, GLint level)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_NamedFramebufferTexture);
   struct marshal_cmd_NamedFramebufferTexture *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_NamedFramebufferTexture, cmd_size);
   cmd->framebuffer = framebuffer;
   cmd->attachment = attachment;
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
void
_mesa_unmarshal_NamedFramebufferTextureLayer(struct gl_context *ctx, const struct marshal_cmd_NamedFramebufferTextureLayer *cmd)
{
   const GLuint framebuffer = cmd->framebuffer;
   const GLenum attachment = cmd->attachment;
   const GLuint texture = cmd->texture;
   const GLint level = cmd->level;
   const GLint layer = cmd->layer;
   CALL_NamedFramebufferTextureLayer(ctx->CurrentServerDispatch, (framebuffer, attachment, texture, level, layer));
}
void GLAPIENTRY
_mesa_marshal_NamedFramebufferTextureLayer(GLuint framebuffer, GLenum attachment, GLuint texture, GLint level, GLint layer)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_NamedFramebufferTextureLayer);
   struct marshal_cmd_NamedFramebufferTextureLayer *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_NamedFramebufferTextureLayer, cmd_size);
   cmd->framebuffer = framebuffer;
   cmd->attachment = attachment;
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
void
_mesa_unmarshal_NamedFramebufferDrawBuffer(struct gl_context *ctx, const struct marshal_cmd_NamedFramebufferDrawBuffer *cmd)
{
   const GLuint framebuffer = cmd->framebuffer;
   const GLenum buf = cmd->buf;
   CALL_NamedFramebufferDrawBuffer(ctx->CurrentServerDispatch, (framebuffer, buf));
}
void GLAPIENTRY
_mesa_marshal_NamedFramebufferDrawBuffer(GLuint framebuffer, GLenum buf)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_NamedFramebufferDrawBuffer);
   struct marshal_cmd_NamedFramebufferDrawBuffer *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_NamedFramebufferDrawBuffer, cmd_size);
   cmd->framebuffer = framebuffer;
   cmd->buf = buf;
}


/* NamedFramebufferDrawBuffers: marshalled asynchronously */
struct marshal_cmd_NamedFramebufferDrawBuffers
{
   struct marshal_cmd_base cmd_base;
   GLuint framebuffer;
   GLsizei n;
   /* Next safe_mul(n, 1 * sizeof(GLenum)) bytes are GLenum bufs[n] */
};
void
_mesa_unmarshal_NamedFramebufferDrawBuffers(struct gl_context *ctx, const struct marshal_cmd_NamedFramebufferDrawBuffers *cmd)
{
   const GLuint framebuffer = cmd->framebuffer;
   const GLsizei n = cmd->n;
   GLenum * bufs;
   const char *variable_data = (const char *) (cmd + 1);
   bufs = (GLenum *) variable_data;
   CALL_NamedFramebufferDrawBuffers(ctx->CurrentServerDispatch, (framebuffer, n, bufs));
}
void GLAPIENTRY
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
void
_mesa_unmarshal_NamedFramebufferReadBuffer(struct gl_context *ctx, const struct marshal_cmd_NamedFramebufferReadBuffer *cmd)
{
   const GLuint framebuffer = cmd->framebuffer;
   const GLenum buf = cmd->buf;
   CALL_NamedFramebufferReadBuffer(ctx->CurrentServerDispatch, (framebuffer, buf));
}
void GLAPIENTRY
_mesa_marshal_NamedFramebufferReadBuffer(GLuint framebuffer, GLenum buf)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_NamedFramebufferReadBuffer);
   struct marshal_cmd_NamedFramebufferReadBuffer *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_NamedFramebufferReadBuffer, cmd_size);
   cmd->framebuffer = framebuffer;
   cmd->buf = buf;
}


/* InvalidateNamedFramebufferData: marshalled asynchronously */
struct marshal_cmd_InvalidateNamedFramebufferData
{
   struct marshal_cmd_base cmd_base;
   GLuint framebuffer;
   GLsizei numAttachments;
   /* Next safe_mul(numAttachments, 1 * sizeof(GLenum)) bytes are GLenum attachments[numAttachments] */
};
void
_mesa_unmarshal_InvalidateNamedFramebufferData(struct gl_context *ctx, const struct marshal_cmd_InvalidateNamedFramebufferData *cmd)
{
   const GLuint framebuffer = cmd->framebuffer;
   const GLsizei numAttachments = cmd->numAttachments;
   GLenum * attachments;
   const char *variable_data = (const char *) (cmd + 1);
   attachments = (GLenum *) variable_data;
   CALL_InvalidateNamedFramebufferData(ctx->CurrentServerDispatch, (framebuffer, numAttachments, attachments));
}
void GLAPIENTRY
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
void
_mesa_unmarshal_InvalidateNamedFramebufferSubData(struct gl_context *ctx, const struct marshal_cmd_InvalidateNamedFramebufferSubData *cmd)
{
   const GLuint framebuffer = cmd->framebuffer;
   const GLsizei numAttachments = cmd->numAttachments;
   const GLint x = cmd->x;
   const GLint y = cmd->y;
   const GLsizei width = cmd->width;
   const GLsizei height = cmd->height;
   GLenum * attachments;
   const char *variable_data = (const char *) (cmd + 1);
   attachments = (GLenum *) variable_data;
   CALL_InvalidateNamedFramebufferSubData(ctx->CurrentServerDispatch, (framebuffer, numAttachments, attachments, x, y, width, height));
}
void GLAPIENTRY
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
void
_mesa_unmarshal_ClearNamedFramebufferiv(struct gl_context *ctx, const struct marshal_cmd_ClearNamedFramebufferiv *cmd)
{
   const GLuint framebuffer = cmd->framebuffer;
   const GLenum buffer = cmd->buffer;
   const GLint drawbuffer = cmd->drawbuffer;
   GLint * value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLint *) variable_data;
   CALL_ClearNamedFramebufferiv(ctx->CurrentServerDispatch, (framebuffer, buffer, drawbuffer, value));
}
void GLAPIENTRY
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
   cmd->buffer = buffer;
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
void
_mesa_unmarshal_ClearNamedFramebufferuiv(struct gl_context *ctx, const struct marshal_cmd_ClearNamedFramebufferuiv *cmd)
{
   const GLuint framebuffer = cmd->framebuffer;
   const GLenum buffer = cmd->buffer;
   const GLint drawbuffer = cmd->drawbuffer;
   GLuint * value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLuint *) variable_data;
   CALL_ClearNamedFramebufferuiv(ctx->CurrentServerDispatch, (framebuffer, buffer, drawbuffer, value));
}
void GLAPIENTRY
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
   cmd->buffer = buffer;
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
void
_mesa_unmarshal_ClearNamedFramebufferfv(struct gl_context *ctx, const struct marshal_cmd_ClearNamedFramebufferfv *cmd)
{
   const GLuint framebuffer = cmd->framebuffer;
   const GLenum buffer = cmd->buffer;
   const GLint drawbuffer = cmd->drawbuffer;
   GLfloat * value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLfloat *) variable_data;
   CALL_ClearNamedFramebufferfv(ctx->CurrentServerDispatch, (framebuffer, buffer, drawbuffer, value));
}
void GLAPIENTRY
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
   cmd->buffer = buffer;
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
void
_mesa_unmarshal_ClearNamedFramebufferfi(struct gl_context *ctx, const struct marshal_cmd_ClearNamedFramebufferfi *cmd)
{
   const GLuint framebuffer = cmd->framebuffer;
   const GLenum buffer = cmd->buffer;
   const GLint drawbuffer = cmd->drawbuffer;
   const GLfloat depth = cmd->depth;
   const GLint stencil = cmd->stencil;
   CALL_ClearNamedFramebufferfi(ctx->CurrentServerDispatch, (framebuffer, buffer, drawbuffer, depth, stencil));
}
void GLAPIENTRY
_mesa_marshal_ClearNamedFramebufferfi(GLuint framebuffer, GLenum buffer, GLint drawbuffer, GLfloat depth, GLint stencil)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ClearNamedFramebufferfi);
   struct marshal_cmd_ClearNamedFramebufferfi *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ClearNamedFramebufferfi, cmd_size);
   cmd->framebuffer = framebuffer;
   cmd->buffer = buffer;
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
void
_mesa_unmarshal_BlitNamedFramebuffer(struct gl_context *ctx, const struct marshal_cmd_BlitNamedFramebuffer *cmd)
{
   const GLuint readFramebuffer = cmd->readFramebuffer;
   const GLuint drawFramebuffer = cmd->drawFramebuffer;
   const GLint srcX0 = cmd->srcX0;
   const GLint srcY0 = cmd->srcY0;
   const GLint srcX1 = cmd->srcX1;
   const GLint srcY1 = cmd->srcY1;
   const GLint dstX0 = cmd->dstX0;
   const GLint dstY0 = cmd->dstY0;
   const GLint dstX1 = cmd->dstX1;
   const GLint dstY1 = cmd->dstY1;
   const GLbitfield mask = cmd->mask;
   const GLenum filter = cmd->filter;
   CALL_BlitNamedFramebuffer(ctx->CurrentServerDispatch, (readFramebuffer, drawFramebuffer, srcX0, srcY0, srcX1, srcY1, dstX0, dstY0, dstX1, dstY1, mask, filter));
}
void GLAPIENTRY
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
   cmd->filter = filter;
}


/* CheckNamedFramebufferStatus: marshalled synchronously */
GLenum GLAPIENTRY
_mesa_marshal_CheckNamedFramebufferStatus(GLuint framebuffer, GLenum target)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "CheckNamedFramebufferStatus");
   return CALL_CheckNamedFramebufferStatus(ctx->CurrentServerDispatch, (framebuffer, target));
}


/* GetNamedFramebufferParameteriv: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetNamedFramebufferParameteriv(GLuint framebuffer, GLenum pname, GLint * param)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetNamedFramebufferParameteriv");
   CALL_GetNamedFramebufferParameteriv(ctx->CurrentServerDispatch, (framebuffer, pname, param));
}


/* GetNamedFramebufferAttachmentParameteriv: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetNamedFramebufferAttachmentParameteriv(GLuint framebuffer, GLenum attachment, GLenum pname, GLint * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetNamedFramebufferAttachmentParameteriv");
   CALL_GetNamedFramebufferAttachmentParameteriv(ctx->CurrentServerDispatch, (framebuffer, attachment, pname, params));
}


/* CreateRenderbuffers: marshalled synchronously */
void GLAPIENTRY
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
void
_mesa_unmarshal_NamedRenderbufferStorage(struct gl_context *ctx, const struct marshal_cmd_NamedRenderbufferStorage *cmd)
{
   const GLuint renderbuffer = cmd->renderbuffer;
   const GLenum internalformat = cmd->internalformat;
   const GLsizei width = cmd->width;
   const GLsizei height = cmd->height;
   CALL_NamedRenderbufferStorage(ctx->CurrentServerDispatch, (renderbuffer, internalformat, width, height));
}
void GLAPIENTRY
_mesa_marshal_NamedRenderbufferStorage(GLuint renderbuffer, GLenum internalformat, GLsizei width, GLsizei height)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_NamedRenderbufferStorage);
   struct marshal_cmd_NamedRenderbufferStorage *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_NamedRenderbufferStorage, cmd_size);
   cmd->renderbuffer = renderbuffer;
   cmd->internalformat = internalformat;
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
void
_mesa_unmarshal_NamedRenderbufferStorageMultisample(struct gl_context *ctx, const struct marshal_cmd_NamedRenderbufferStorageMultisample *cmd)
{
   const GLuint renderbuffer = cmd->renderbuffer;
   const GLsizei samples = cmd->samples;
   const GLenum internalformat = cmd->internalformat;
   const GLsizei width = cmd->width;
   const GLsizei height = cmd->height;
   CALL_NamedRenderbufferStorageMultisample(ctx->CurrentServerDispatch, (renderbuffer, samples, internalformat, width, height));
}
void GLAPIENTRY
_mesa_marshal_NamedRenderbufferStorageMultisample(GLuint renderbuffer, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_NamedRenderbufferStorageMultisample);
   struct marshal_cmd_NamedRenderbufferStorageMultisample *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_NamedRenderbufferStorageMultisample, cmd_size);
   cmd->renderbuffer = renderbuffer;
   cmd->samples = samples;
   cmd->internalformat = internalformat;
   cmd->width = width;
   cmd->height = height;
}


/* GetNamedRenderbufferParameteriv: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetNamedRenderbufferParameteriv(GLuint renderbuffer, GLenum pname, GLint * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetNamedRenderbufferParameteriv");
   CALL_GetNamedRenderbufferParameteriv(ctx->CurrentServerDispatch, (renderbuffer, pname, params));
}


/* CreateTextures: marshalled synchronously */
void GLAPIENTRY
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
void
_mesa_unmarshal_TextureBuffer(struct gl_context *ctx, const struct marshal_cmd_TextureBuffer *cmd)
{
   const GLuint texture = cmd->texture;
   const GLenum internalformat = cmd->internalformat;
   const GLuint buffer = cmd->buffer;
   CALL_TextureBuffer(ctx->CurrentServerDispatch, (texture, internalformat, buffer));
}
void GLAPIENTRY
_mesa_marshal_TextureBuffer(GLuint texture, GLenum internalformat, GLuint buffer)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TextureBuffer);
   struct marshal_cmd_TextureBuffer *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TextureBuffer, cmd_size);
   cmd->texture = texture;
   cmd->internalformat = internalformat;
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
void
_mesa_unmarshal_TextureBufferRange(struct gl_context *ctx, const struct marshal_cmd_TextureBufferRange *cmd)
{
   const GLuint texture = cmd->texture;
   const GLenum internalformat = cmd->internalformat;
   const GLuint buffer = cmd->buffer;
   const GLintptr offset = cmd->offset;
   const GLsizeiptr size = cmd->size;
   CALL_TextureBufferRange(ctx->CurrentServerDispatch, (texture, internalformat, buffer, offset, size));
}
void GLAPIENTRY
_mesa_marshal_TextureBufferRange(GLuint texture, GLenum internalformat, GLuint buffer, GLintptr offset, GLsizeiptr size)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TextureBufferRange);
   struct marshal_cmd_TextureBufferRange *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TextureBufferRange, cmd_size);
   cmd->texture = texture;
   cmd->internalformat = internalformat;
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
void
_mesa_unmarshal_TextureStorage1D(struct gl_context *ctx, const struct marshal_cmd_TextureStorage1D *cmd)
{
   const GLuint texture = cmd->texture;
   const GLsizei levels = cmd->levels;
   const GLenum internalformat = cmd->internalformat;
   const GLsizei width = cmd->width;
   CALL_TextureStorage1D(ctx->CurrentServerDispatch, (texture, levels, internalformat, width));
}
void GLAPIENTRY
_mesa_marshal_TextureStorage1D(GLuint texture, GLsizei levels, GLenum internalformat, GLsizei width)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TextureStorage1D);
   struct marshal_cmd_TextureStorage1D *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TextureStorage1D, cmd_size);
   cmd->texture = texture;
   cmd->levels = levels;
   cmd->internalformat = internalformat;
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
void
_mesa_unmarshal_TextureStorage2D(struct gl_context *ctx, const struct marshal_cmd_TextureStorage2D *cmd)
{
   const GLuint texture = cmd->texture;
   const GLsizei levels = cmd->levels;
   const GLenum internalformat = cmd->internalformat;
   const GLsizei width = cmd->width;
   const GLsizei height = cmd->height;
   CALL_TextureStorage2D(ctx->CurrentServerDispatch, (texture, levels, internalformat, width, height));
}
void GLAPIENTRY
_mesa_marshal_TextureStorage2D(GLuint texture, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TextureStorage2D);
   struct marshal_cmd_TextureStorage2D *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TextureStorage2D, cmd_size);
   cmd->texture = texture;
   cmd->levels = levels;
   cmd->internalformat = internalformat;
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
void
_mesa_unmarshal_TextureStorage3D(struct gl_context *ctx, const struct marshal_cmd_TextureStorage3D *cmd)
{
   const GLuint texture = cmd->texture;
   const GLsizei levels = cmd->levels;
   const GLenum internalformat = cmd->internalformat;
   const GLsizei width = cmd->width;
   const GLsizei height = cmd->height;
   const GLsizei depth = cmd->depth;
   CALL_TextureStorage3D(ctx->CurrentServerDispatch, (texture, levels, internalformat, width, height, depth));
}
void GLAPIENTRY
_mesa_marshal_TextureStorage3D(GLuint texture, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TextureStorage3D);
   struct marshal_cmd_TextureStorage3D *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TextureStorage3D, cmd_size);
   cmd->texture = texture;
   cmd->levels = levels;
   cmd->internalformat = internalformat;
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
void
_mesa_unmarshal_TextureStorage2DMultisample(struct gl_context *ctx, const struct marshal_cmd_TextureStorage2DMultisample *cmd)
{
   const GLuint texture = cmd->texture;
   const GLsizei samples = cmd->samples;
   const GLenum internalformat = cmd->internalformat;
   const GLsizei width = cmd->width;
   const GLsizei height = cmd->height;
   const GLboolean fixedsamplelocations = cmd->fixedsamplelocations;
   CALL_TextureStorage2DMultisample(ctx->CurrentServerDispatch, (texture, samples, internalformat, width, height, fixedsamplelocations));
}
void GLAPIENTRY
_mesa_marshal_TextureStorage2DMultisample(GLuint texture, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TextureStorage2DMultisample);
   struct marshal_cmd_TextureStorage2DMultisample *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TextureStorage2DMultisample, cmd_size);
   cmd->texture = texture;
   cmd->samples = samples;
   cmd->internalformat = internalformat;
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
void
_mesa_unmarshal_TextureStorage3DMultisample(struct gl_context *ctx, const struct marshal_cmd_TextureStorage3DMultisample *cmd)
{
   const GLuint texture = cmd->texture;
   const GLsizei samples = cmd->samples;
   const GLenum internalformat = cmd->internalformat;
   const GLsizei width = cmd->width;
   const GLsizei height = cmd->height;
   const GLsizei depth = cmd->depth;
   const GLboolean fixedsamplelocations = cmd->fixedsamplelocations;
   CALL_TextureStorage3DMultisample(ctx->CurrentServerDispatch, (texture, samples, internalformat, width, height, depth, fixedsamplelocations));
}
void GLAPIENTRY
_mesa_marshal_TextureStorage3DMultisample(GLuint texture, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TextureStorage3DMultisample);
   struct marshal_cmd_TextureStorage3DMultisample *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TextureStorage3DMultisample, cmd_size);
   cmd->texture = texture;
   cmd->samples = samples;
   cmd->internalformat = internalformat;
   cmd->width = width;
   cmd->height = height;
   cmd->depth = depth;
   cmd->fixedsamplelocations = fixedsamplelocations;
}


/* TextureSubImage1D: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_TextureSubImage1D(GLuint texture, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const GLvoid * pixels)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "TextureSubImage1D");
   CALL_TextureSubImage1D(ctx->CurrentServerDispatch, (texture, level, xoffset, width, format, type, pixels));
}


/* TextureSubImage2D: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_TextureSubImage2D(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid * pixels)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "TextureSubImage2D");
   CALL_TextureSubImage2D(ctx->CurrentServerDispatch, (texture, level, xoffset, yoffset, width, height, format, type, pixels));
}


/* TextureSubImage3D: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_TextureSubImage3D(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const GLvoid * pixels)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "TextureSubImage3D");
   CALL_TextureSubImage3D(ctx->CurrentServerDispatch, (texture, level, xoffset, yoffset, zoffset, width, height, depth, format, type, pixels));
}


/* CompressedTextureSubImage1D: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_CompressedTextureSubImage1D(GLuint texture, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const GLvoid * data)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "CompressedTextureSubImage1D");
   CALL_CompressedTextureSubImage1D(ctx->CurrentServerDispatch, (texture, level, xoffset, width, format, imageSize, data));
}


/* CompressedTextureSubImage2D: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_CompressedTextureSubImage2D(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const GLvoid * data)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "CompressedTextureSubImage2D");
   CALL_CompressedTextureSubImage2D(ctx->CurrentServerDispatch, (texture, level, xoffset, yoffset, width, height, format, imageSize, data));
}


/* CompressedTextureSubImage3D: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_CompressedTextureSubImage3D(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const GLvoid * data)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "CompressedTextureSubImage3D");
   CALL_CompressedTextureSubImage3D(ctx->CurrentServerDispatch, (texture, level, xoffset, yoffset, zoffset, width, height, depth, format, imageSize, data));
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
void
_mesa_unmarshal_CopyTextureSubImage1D(struct gl_context *ctx, const struct marshal_cmd_CopyTextureSubImage1D *cmd)
{
   const GLuint texture = cmd->texture;
   const GLint level = cmd->level;
   const GLint xoffset = cmd->xoffset;
   const GLint x = cmd->x;
   const GLint y = cmd->y;
   const GLsizei width = cmd->width;
   CALL_CopyTextureSubImage1D(ctx->CurrentServerDispatch, (texture, level, xoffset, x, y, width));
}
void GLAPIENTRY
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
void
_mesa_unmarshal_CopyTextureSubImage2D(struct gl_context *ctx, const struct marshal_cmd_CopyTextureSubImage2D *cmd)
{
   const GLuint texture = cmd->texture;
   const GLint level = cmd->level;
   const GLint xoffset = cmd->xoffset;
   const GLint yoffset = cmd->yoffset;
   const GLint x = cmd->x;
   const GLint y = cmd->y;
   const GLsizei width = cmd->width;
   const GLsizei height = cmd->height;
   CALL_CopyTextureSubImage2D(ctx->CurrentServerDispatch, (texture, level, xoffset, yoffset, x, y, width, height));
}
void GLAPIENTRY
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
void
_mesa_unmarshal_CopyTextureSubImage3D(struct gl_context *ctx, const struct marshal_cmd_CopyTextureSubImage3D *cmd)
{
   const GLuint texture = cmd->texture;
   const GLint level = cmd->level;
   const GLint xoffset = cmd->xoffset;
   const GLint yoffset = cmd->yoffset;
   const GLint zoffset = cmd->zoffset;
   const GLint x = cmd->x;
   const GLint y = cmd->y;
   const GLsizei width = cmd->width;
   const GLsizei height = cmd->height;
   CALL_CopyTextureSubImage3D(ctx->CurrentServerDispatch, (texture, level, xoffset, yoffset, zoffset, x, y, width, height));
}
void GLAPIENTRY
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
void
_mesa_unmarshal_TextureParameterf(struct gl_context *ctx, const struct marshal_cmd_TextureParameterf *cmd)
{
   const GLuint texture = cmd->texture;
   const GLenum pname = cmd->pname;
   const GLfloat param = cmd->param;
   CALL_TextureParameterf(ctx->CurrentServerDispatch, (texture, pname, param));
}
void GLAPIENTRY
_mesa_marshal_TextureParameterf(GLuint texture, GLenum pname, GLfloat param)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TextureParameterf);
   struct marshal_cmd_TextureParameterf *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TextureParameterf, cmd_size);
   cmd->texture = texture;
   cmd->pname = pname;
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
void
_mesa_unmarshal_TextureParameterfv(struct gl_context *ctx, const struct marshal_cmd_TextureParameterfv *cmd)
{
   const GLuint texture = cmd->texture;
   const GLenum pname = cmd->pname;
   GLfloat * param;
   const char *variable_data = (const char *) (cmd + 1);
   param = (GLfloat *) variable_data;
   CALL_TextureParameterfv(ctx->CurrentServerDispatch, (texture, pname, param));
}
void GLAPIENTRY
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
   cmd->pname = pname;
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
void
_mesa_unmarshal_TextureParameteri(struct gl_context *ctx, const struct marshal_cmd_TextureParameteri *cmd)
{
   const GLuint texture = cmd->texture;
   const GLenum pname = cmd->pname;
   const GLint param = cmd->param;
   CALL_TextureParameteri(ctx->CurrentServerDispatch, (texture, pname, param));
}
void GLAPIENTRY
_mesa_marshal_TextureParameteri(GLuint texture, GLenum pname, GLint param)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TextureParameteri);
   struct marshal_cmd_TextureParameteri *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TextureParameteri, cmd_size);
   cmd->texture = texture;
   cmd->pname = pname;
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
void
_mesa_unmarshal_TextureParameterIiv(struct gl_context *ctx, const struct marshal_cmd_TextureParameterIiv *cmd)
{
   const GLuint texture = cmd->texture;
   const GLenum pname = cmd->pname;
   GLint * params;
   const char *variable_data = (const char *) (cmd + 1);
   params = (GLint *) variable_data;
   CALL_TextureParameterIiv(ctx->CurrentServerDispatch, (texture, pname, params));
}
void GLAPIENTRY
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
   cmd->pname = pname;
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
void
_mesa_unmarshal_TextureParameterIuiv(struct gl_context *ctx, const struct marshal_cmd_TextureParameterIuiv *cmd)
{
   const GLuint texture = cmd->texture;
   const GLenum pname = cmd->pname;
   GLuint * params;
   const char *variable_data = (const char *) (cmd + 1);
   params = (GLuint *) variable_data;
   CALL_TextureParameterIuiv(ctx->CurrentServerDispatch, (texture, pname, params));
}
void GLAPIENTRY
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
   cmd->pname = pname;
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
void
_mesa_unmarshal_TextureParameteriv(struct gl_context *ctx, const struct marshal_cmd_TextureParameteriv *cmd)
{
   const GLuint texture = cmd->texture;
   const GLenum pname = cmd->pname;
   GLint * param;
   const char *variable_data = (const char *) (cmd + 1);
   param = (GLint *) variable_data;
   CALL_TextureParameteriv(ctx->CurrentServerDispatch, (texture, pname, param));
}
void GLAPIENTRY
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
   cmd->pname = pname;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, param, param_size);
}


/* GenerateTextureMipmap: marshalled asynchronously */
struct marshal_cmd_GenerateTextureMipmap
{
   struct marshal_cmd_base cmd_base;
   GLuint texture;
};
void
_mesa_unmarshal_GenerateTextureMipmap(struct gl_context *ctx, const struct marshal_cmd_GenerateTextureMipmap *cmd)
{
   const GLuint texture = cmd->texture;
   CALL_GenerateTextureMipmap(ctx->CurrentServerDispatch, (texture));
}
void GLAPIENTRY
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
void
_mesa_unmarshal_BindTextureUnit(struct gl_context *ctx, const struct marshal_cmd_BindTextureUnit *cmd)
{
   const GLuint unit = cmd->unit;
   const GLuint texture = cmd->texture;
   CALL_BindTextureUnit(ctx->CurrentServerDispatch, (unit, texture));
}
void GLAPIENTRY
_mesa_marshal_BindTextureUnit(GLuint unit, GLuint texture)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_BindTextureUnit);
   struct marshal_cmd_BindTextureUnit *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_BindTextureUnit, cmd_size);
   cmd->unit = unit;
   cmd->texture = texture;
}


/* GetTextureImage: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetTextureImage(GLuint texture, GLint level, GLenum format, GLenum type, GLsizei bufSize, GLvoid * pixels)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetTextureImage");
   CALL_GetTextureImage(ctx->CurrentServerDispatch, (texture, level, format, type, bufSize, pixels));
}


/* GetCompressedTextureImage: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetCompressedTextureImage(GLuint texture, GLint level, GLsizei bufSize, GLvoid * pixels)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetCompressedTextureImage");
   CALL_GetCompressedTextureImage(ctx->CurrentServerDispatch, (texture, level, bufSize, pixels));
}


/* GetTextureLevelParameterfv: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetTextureLevelParameterfv(GLuint texture, GLint level, GLenum pname, GLfloat * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetTextureLevelParameterfv");
   CALL_GetTextureLevelParameterfv(ctx->CurrentServerDispatch, (texture, level, pname, params));
}


/* GetTextureLevelParameteriv: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetTextureLevelParameteriv(GLuint texture, GLint level, GLenum pname, GLint * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetTextureLevelParameteriv");
   CALL_GetTextureLevelParameteriv(ctx->CurrentServerDispatch, (texture, level, pname, params));
}


