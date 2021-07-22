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
   GLuint program;
   GLenum target;
   GLuint index;
   GLfloat x;
   GLfloat y;
   GLfloat z;
   GLfloat w;
};
void
_mesa_unmarshal_NamedProgramLocalParameter4fEXT(struct gl_context *ctx, const struct marshal_cmd_NamedProgramLocalParameter4fEXT *cmd)
{
   GLuint program = cmd->program;
   GLenum target = cmd->target;
   GLuint index = cmd->index;
   GLfloat x = cmd->x;
   GLfloat y = cmd->y;
   GLfloat z = cmd->z;
   GLfloat w = cmd->w;
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
   GLuint program;
   GLenum target;
   GLuint index;
   GLfloat params[4];
};
void
_mesa_unmarshal_NamedProgramLocalParameter4fvEXT(struct gl_context *ctx, const struct marshal_cmd_NamedProgramLocalParameter4fvEXT *cmd)
{
   GLuint program = cmd->program;
   GLenum target = cmd->target;
   GLuint index = cmd->index;
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
   GLuint program;
   GLenum target;
   GLuint index;
   GLdouble x;
   GLdouble y;
   GLdouble z;
   GLdouble w;
};
void
_mesa_unmarshal_NamedProgramLocalParameter4dEXT(struct gl_context *ctx, const struct marshal_cmd_NamedProgramLocalParameter4dEXT *cmd)
{
   GLuint program = cmd->program;
   GLenum target = cmd->target;
   GLuint index = cmd->index;
   GLdouble x = cmd->x;
   GLdouble y = cmd->y;
   GLdouble z = cmd->z;
   GLdouble w = cmd->w;
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
   GLuint program;
   GLenum target;
   GLuint index;
   GLdouble params[4];
};
void
_mesa_unmarshal_NamedProgramLocalParameter4dvEXT(struct gl_context *ctx, const struct marshal_cmd_NamedProgramLocalParameter4dvEXT *cmd)
{
   GLuint program = cmd->program;
   GLenum target = cmd->target;
   GLuint index = cmd->index;
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
   GLuint texture;
   GLenum target;
   GLenum internalformat;
   GLuint buffer;
};
void
_mesa_unmarshal_TextureBufferEXT(struct gl_context *ctx, const struct marshal_cmd_TextureBufferEXT *cmd)
{
   GLuint texture = cmd->texture;
   GLenum target = cmd->target;
   GLenum internalformat = cmd->internalformat;
   GLuint buffer = cmd->buffer;
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
   GLenum texunit;
   GLenum target;
   GLenum internalformat;
   GLuint buffer;
};
void
_mesa_unmarshal_MultiTexBufferEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexBufferEXT *cmd)
{
   GLenum texunit = cmd->texunit;
   GLenum target = cmd->target;
   GLenum internalformat = cmd->internalformat;
   GLuint buffer = cmd->buffer;
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
   GLuint texture;
   GLenum target;
   GLenum pname;
   /* Next safe_mul(_mesa_tex_param_enum_to_count(pname), 1 * sizeof(GLint)) bytes are GLint params[_mesa_tex_param_enum_to_count(pname)] */
};
void
_mesa_unmarshal_TextureParameterIivEXT(struct gl_context *ctx, const struct marshal_cmd_TextureParameterIivEXT *cmd)
{
   GLuint texture = cmd->texture;
   GLenum target = cmd->target;
   GLenum pname = cmd->pname;
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
   GLuint texture;
   GLenum target;
   GLenum pname;
   /* Next safe_mul(_mesa_tex_param_enum_to_count(pname), 1 * sizeof(GLuint)) bytes are GLuint params[_mesa_tex_param_enum_to_count(pname)] */
};
void
_mesa_unmarshal_TextureParameterIuivEXT(struct gl_context *ctx, const struct marshal_cmd_TextureParameterIuivEXT *cmd)
{
   GLuint texture = cmd->texture;
   GLenum target = cmd->target;
   GLenum pname = cmd->pname;
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
   GLenum texunit;
   GLenum target;
   GLenum pname;
   /* Next safe_mul(_mesa_tex_param_enum_to_count(pname), 1 * sizeof(GLint)) bytes are GLint params[_mesa_tex_param_enum_to_count(pname)] */
};
void
_mesa_unmarshal_MultiTexParameterIivEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexParameterIivEXT *cmd)
{
   GLenum texunit = cmd->texunit;
   GLenum target = cmd->target;
   GLenum pname = cmd->pname;
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
   GLenum texunit;
   GLenum target;
   GLenum pname;
   /* Next safe_mul(_mesa_tex_param_enum_to_count(pname), 1 * sizeof(GLuint)) bytes are GLuint params[_mesa_tex_param_enum_to_count(pname)] */
};
void
_mesa_unmarshal_MultiTexParameterIuivEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexParameterIuivEXT *cmd)
{
   GLenum texunit = cmd->texunit;
   GLenum target = cmd->target;
   GLenum pname = cmd->pname;
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
   GLuint program;
   GLenum target;
   GLuint index;
   GLsizei count;
   GLfloat params[4];
};
void
_mesa_unmarshal_NamedProgramLocalParameters4fvEXT(struct gl_context *ctx, const struct marshal_cmd_NamedProgramLocalParameters4fvEXT *cmd)
{
   GLuint program = cmd->program;
   GLenum target = cmd->target;
   GLuint index = cmd->index;
   GLsizei count = cmd->count;
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
   GLuint index;
   GLuint texture;
   GLint level;
   GLint layer;
   GLenum access;
   GLint format;
};
void
_mesa_unmarshal_BindImageTextureEXT(struct gl_context *ctx, const struct marshal_cmd_BindImageTextureEXT *cmd)
{
   GLuint index = cmd->index;
   GLuint texture = cmd->texture;
   GLint level = cmd->level;
   GLboolean layered = cmd->layered;
   GLint layer = cmd->layer;
   GLenum access = cmd->access;
   GLint format = cmd->format;
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
   GLuint xbits = cmd->xbits;
   GLuint ybits = cmd->ybits;
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
   GLenum pname;
   GLfloat param;
};
void
_mesa_unmarshal_ConservativeRasterParameterfNV(struct gl_context *ctx, const struct marshal_cmd_ConservativeRasterParameterfNV *cmd)
{
   GLenum pname = cmd->pname;
   GLfloat param = cmd->param;
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
   GLenum pname;
   GLint param;
};
void
_mesa_unmarshal_ConservativeRasterParameteriNV(struct gl_context *ctx, const struct marshal_cmd_ConservativeRasterParameteriNV *cmd)
{
   GLenum pname = cmd->pname;
   GLint param = cmd->param;
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
   GLuint queryHandle = cmd->queryHandle;
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
   GLuint queryHandle = cmd->queryHandle;
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
   GLuint queryHandle = cmd->queryHandle;
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
   GLenum mode;
};
void
_mesa_unmarshal_AlphaToCoverageDitherControlNV(struct gl_context *ctx, const struct marshal_cmd_AlphaToCoverageDitherControlNV *cmd)
{
   GLenum mode = cmd->mode;
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
   GLfloat factor = cmd->factor;
   GLfloat units = cmd->units;
   GLfloat clamp = cmd->clamp;
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
   GLenum mode;
   GLsizei count;
   /* Next safe_mul((4 * count), 1 * sizeof(GLint)) bytes are GLint box[(4 * count)] */
};
void
_mesa_unmarshal_WindowRectanglesEXT(struct gl_context *ctx, const struct marshal_cmd_WindowRectanglesEXT *cmd)
{
   GLenum mode = cmd->mode;
   GLsizei count = cmd->count;
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
   GLenum target;
   GLsizei samples;
   GLsizei storageSamples;
   GLenum internalformat;
   GLsizei width;
   GLsizei height;
};
void
_mesa_unmarshal_RenderbufferStorageMultisampleAdvancedAMD(struct gl_context *ctx, const struct marshal_cmd_RenderbufferStorageMultisampleAdvancedAMD *cmd)
{
   GLenum target = cmd->target;
   GLsizei samples = cmd->samples;
   GLsizei storageSamples = cmd->storageSamples;
   GLenum internalformat = cmd->internalformat;
   GLsizei width = cmd->width;
   GLsizei height = cmd->height;
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
   GLuint renderbuffer;
   GLsizei samples;
   GLsizei storageSamples;
   GLenum internalformat;
   GLsizei width;
   GLsizei height;
};
void
_mesa_unmarshal_NamedRenderbufferStorageMultisampleAdvancedAMD(struct gl_context *ctx, const struct marshal_cmd_NamedRenderbufferStorageMultisampleAdvancedAMD *cmd)
{
   GLuint renderbuffer = cmd->renderbuffer;
   GLsizei samples = cmd->samples;
   GLsizei storageSamples = cmd->storageSamples;
   GLenum internalformat = cmd->internalformat;
   GLsizei width = cmd->width;
   GLsizei height = cmd->height;
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
   GLenum frontfunc;
   GLenum backfunc;
   GLint ref;
   GLuint mask;
};
void
_mesa_unmarshal_StencilFuncSeparateATI(struct gl_context *ctx, const struct marshal_cmd_StencilFuncSeparateATI *cmd)
{
   GLenum frontfunc = cmd->frontfunc;
   GLenum backfunc = cmd->backfunc;
   GLint ref = cmd->ref;
   GLuint mask = cmd->mask;
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
   GLenum target;
   GLuint index;
   GLsizei count;
   GLfloat params[4];
};
void
_mesa_unmarshal_ProgramEnvParameters4fvEXT(struct gl_context *ctx, const struct marshal_cmd_ProgramEnvParameters4fvEXT *cmd)
{
   GLenum target = cmd->target;
   GLuint index = cmd->index;
   GLsizei count = cmd->count;
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
   GLenum target;
   GLuint index;
   GLsizei count;
   GLfloat params[4];
};
void
_mesa_unmarshal_ProgramLocalParameters4fvEXT(struct gl_context *ctx, const struct marshal_cmd_ProgramLocalParameters4fvEXT *cmd)
{
   GLenum target = cmd->target;
   GLuint index = cmd->index;
   GLsizei count = cmd->count;
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


/* InternalBufferSubDataCopyMESA: marshalled asynchronously */
struct marshal_cmd_InternalBufferSubDataCopyMESA
{
   struct marshal_cmd_base cmd_base;
   GLboolean named;
   GLboolean ext_dsa;
   GLuint srcOffset;
   GLuint dstTargetOrName;
   GLintptr srcBuffer;
   GLintptr dstOffset;
   GLsizeiptr size;
};
void
_mesa_unmarshal_InternalBufferSubDataCopyMESA(struct gl_context *ctx, const struct marshal_cmd_InternalBufferSubDataCopyMESA *cmd)
{
   GLintptr srcBuffer = cmd->srcBuffer;
   GLuint srcOffset = cmd->srcOffset;
   GLuint dstTargetOrName = cmd->dstTargetOrName;
   GLintptr dstOffset = cmd->dstOffset;
   GLsizeiptr size = cmd->size;
   GLboolean named = cmd->named;
   GLboolean ext_dsa = cmd->ext_dsa;
   CALL_InternalBufferSubDataCopyMESA(ctx->CurrentServerDispatch, (srcBuffer, srcOffset, dstTargetOrName, dstOffset, size, named, ext_dsa));
}
void GLAPIENTRY
_mesa_marshal_InternalBufferSubDataCopyMESA(GLintptr srcBuffer, GLuint srcOffset, GLuint dstTargetOrName, GLintptr dstOffset, GLsizeiptr size, GLboolean named, GLboolean ext_dsa)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_InternalBufferSubDataCopyMESA);
   struct marshal_cmd_InternalBufferSubDataCopyMESA *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_InternalBufferSubDataCopyMESA, cmd_size);
   cmd->srcBuffer = srcBuffer;
   cmd->srcOffset = srcOffset;
   cmd->dstTargetOrName = dstTargetOrName;
   cmd->dstOffset = dstOffset;
   cmd->size = size;
   cmd->named = named;
   cmd->ext_dsa = ext_dsa;
}


/* InternalSetError: marshalled asynchronously */
struct marshal_cmd_InternalSetError
{
   struct marshal_cmd_base cmd_base;
   GLenum error;
};
void
_mesa_unmarshal_InternalSetError(struct gl_context *ctx, const struct marshal_cmd_InternalSetError *cmd)
{
   GLenum error = cmd->error;
   CALL_InternalSetError(ctx->CurrentServerDispatch, (error));
}
void GLAPIENTRY
_mesa_marshal_InternalSetError(GLenum error)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_InternalSetError);
   struct marshal_cmd_InternalSetError *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_InternalSetError, cmd_size);
   cmd->error = error;
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
   GLint r = cmd->r;
   GLint g = cmd->g;
   GLint b = cmd->b;
   GLint a = cmd->a;
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
   GLuint r = cmd->r;
   GLuint g = cmd->g;
   GLuint b = cmd->b;
   GLuint a = cmd->a;
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
   GLenum target;
   GLenum pname;
   /* Next safe_mul(_mesa_tex_param_enum_to_count(pname), 1 * sizeof(GLint)) bytes are GLint params[_mesa_tex_param_enum_to_count(pname)] */
};
void
_mesa_unmarshal_TexParameterIiv(struct gl_context *ctx, const struct marshal_cmd_TexParameterIiv *cmd)
{
   GLenum target = cmd->target;
   GLenum pname = cmd->pname;
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
   GLenum target;
   GLenum pname;
   /* Next safe_mul(_mesa_tex_param_enum_to_count(pname), 1 * sizeof(GLuint)) bytes are GLuint params[_mesa_tex_param_enum_to_count(pname)] */
};
void
_mesa_unmarshal_TexParameterIuiv(struct gl_context *ctx, const struct marshal_cmd_TexParameterIuiv *cmd)
{
   GLenum target = cmd->target;
   GLenum pname = cmd->pname;
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
   GLuint index = cmd->index;
   GLint x = cmd->x;
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
   GLuint index = cmd->index;
   GLint x = cmd->x;
   GLint y = cmd->y;
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
   GLuint index = cmd->index;
   GLint x = cmd->x;
   GLint y = cmd->y;
   GLint z = cmd->z;
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
   GLuint index = cmd->index;
   GLint x = cmd->x;
   GLint y = cmd->y;
   GLint z = cmd->z;
   GLint w = cmd->w;
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
   GLuint index = cmd->index;
   GLuint x = cmd->x;
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
   GLuint index = cmd->index;
   GLuint x = cmd->x;
   GLuint y = cmd->y;
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
   GLuint index = cmd->index;
   GLuint x = cmd->x;
   GLuint y = cmd->y;
   GLuint z = cmd->z;
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
   GLuint index = cmd->index;
   GLuint x = cmd->x;
   GLuint y = cmd->y;
   GLuint z = cmd->z;
   GLuint w = cmd->w;
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
   GLuint index = cmd->index;
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
   GLuint index = cmd->index;
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
   GLuint index = cmd->index;
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
   GLuint index = cmd->index;
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
   GLuint index = cmd->index;
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
   GLuint index = cmd->index;
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
   GLuint index = cmd->index;
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
   GLuint index = cmd->index;
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
   GLuint index = cmd->index;
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
   GLuint index = cmd->index;
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
   GLuint index = cmd->index;
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
   GLuint index = cmd->index;
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
   GLuint index;
   GLint size;
   GLenum type;
   GLsizei stride;
   const GLvoid * pointer;
};
void
_mesa_unmarshal_VertexAttribIPointer(struct gl_context *ctx, const struct marshal_cmd_VertexAttribIPointer *cmd)
{
   GLuint index = cmd->index;
   GLint size = cmd->size;
   GLenum type = cmd->type;
   GLsizei stride = cmd->stride;
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
   if (COMPAT) _mesa_glthread_AttribPointer(ctx, VERT_ATTRIB_GENERIC(index), size, type, stride, pointer);
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
   GLint location = cmd->location;
   GLuint x = cmd->x;
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
   GLint location = cmd->location;
   GLuint x = cmd->x;
   GLuint y = cmd->y;
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
   GLint location = cmd->location;
   GLuint x = cmd->x;
   GLuint y = cmd->y;
   GLuint z = cmd->z;
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
   GLint location = cmd->location;
   GLuint x = cmd->x;
   GLuint y = cmd->y;
   GLuint z = cmd->z;
   GLuint w = cmd->w;
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
   GLint location = cmd->location;
   GLsizei count = cmd->count;
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
   GLint location = cmd->location;
   GLsizei count = cmd->count;
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
   GLint location = cmd->location;
   GLsizei count = cmd->count;
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
   GLint location = cmd->location;
   GLsizei count = cmd->count;
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
   GLuint program = cmd->program;
   GLuint colorNumber = cmd->colorNumber;
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
   GLenum buffer;
   GLint drawbuffer;
   /* Next safe_mul(_mesa_buffer_enum_to_count(buffer), 1 * sizeof(GLint)) bytes are GLint value[_mesa_buffer_enum_to_count(buffer)] */
};
void
_mesa_unmarshal_ClearBufferiv(struct gl_context *ctx, const struct marshal_cmd_ClearBufferiv *cmd)
{
   GLenum buffer = cmd->buffer;
   GLint drawbuffer = cmd->drawbuffer;
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
   GLenum buffer;
   GLint drawbuffer;
   /* Next safe_mul(_mesa_buffer_enum_to_count(buffer), 1 * sizeof(GLuint)) bytes are GLuint value[_mesa_buffer_enum_to_count(buffer)] */
};
void
_mesa_unmarshal_ClearBufferuiv(struct gl_context *ctx, const struct marshal_cmd_ClearBufferuiv *cmd)
{
   GLenum buffer = cmd->buffer;
   GLint drawbuffer = cmd->drawbuffer;
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
   GLenum buffer;
   GLint drawbuffer;
   /* Next safe_mul(_mesa_buffer_enum_to_count(buffer), 1 * sizeof(GLfloat)) bytes are GLfloat value[_mesa_buffer_enum_to_count(buffer)] */
};
void
_mesa_unmarshal_ClearBufferfv(struct gl_context *ctx, const struct marshal_cmd_ClearBufferfv *cmd)
{
   GLenum buffer = cmd->buffer;
   GLint drawbuffer = cmd->drawbuffer;
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
   GLenum buffer;
   GLint drawbuffer;
   GLfloat depth;
   GLint stencil;
};
void
_mesa_unmarshal_ClearBufferfi(struct gl_context *ctx, const struct marshal_cmd_ClearBufferfi *cmd)
{
   GLenum buffer = cmd->buffer;
   GLint drawbuffer = cmd->drawbuffer;
   GLfloat depth = cmd->depth;
   GLint stencil = cmd->stencil;
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
   GLenum mode;
};
void
_mesa_unmarshal_BeginTransformFeedback(struct gl_context *ctx, const struct marshal_cmd_BeginTransformFeedback *cmd)
{
   GLenum mode = cmd->mode;
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
   GLenum target;
   GLuint index;
   GLuint buffer;
   GLintptr offset;
   GLsizeiptr size;
};
void
_mesa_unmarshal_BindBufferRange(struct gl_context *ctx, const struct marshal_cmd_BindBufferRange *cmd)
{
   GLenum target = cmd->target;
   GLuint index = cmd->index;
   GLuint buffer = cmd->buffer;
   GLintptr offset = cmd->offset;
   GLsizeiptr size = cmd->size;
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
   GLenum target;
   GLuint index;
   GLuint buffer;
};
void
_mesa_unmarshal_BindBufferBase(struct gl_context *ctx, const struct marshal_cmd_BindBufferBase *cmd)
{
   GLenum target = cmd->target;
   GLuint index = cmd->index;
   GLuint buffer = cmd->buffer;
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
   GLuint query;
   GLenum mode;
};
void
_mesa_unmarshal_BeginConditionalRender(struct gl_context *ctx, const struct marshal_cmd_BeginConditionalRender *cmd)
{
   GLuint query = cmd->query;
   GLenum mode = cmd->mode;
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
   GLuint index = cmd->index;
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
   _mesa_glthread_PrimitiveRestartIndex(ctx, index);
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
   GLenum target;
   GLenum attachment;
   GLuint texture;
   GLint level;
};
void
_mesa_unmarshal_FramebufferTexture(struct gl_context *ctx, const struct marshal_cmd_FramebufferTexture *cmd)
{
   GLenum target = cmd->target;
   GLenum attachment = cmd->attachment;
   GLuint texture = cmd->texture;
   GLint level = cmd->level;
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
   GLenum target;
   GLuint index;
   GLuint buffer;
   GLintptr offset;
};
void
_mesa_unmarshal_BindBufferOffsetEXT(struct gl_context *ctx, const struct marshal_cmd_BindBufferOffsetEXT *cmd)
{
   GLenum target = cmd->target;
   GLuint index = cmd->index;
   GLuint buffer = cmd->buffer;
   GLintptr offset = cmd->offset;
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
   GLenum target;
   GLuint id;
};
void
_mesa_unmarshal_BindTransformFeedback(struct gl_context *ctx, const struct marshal_cmd_BindTransformFeedback *cmd)
{
   GLenum target = cmd->target;
   GLuint id = cmd->id;
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
   GLsizei n = cmd->n;
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
   GLenum mode;
   GLuint id;
};
void
_mesa_unmarshal_DrawTransformFeedback(struct gl_context *ctx, const struct marshal_cmd_DrawTransformFeedback *cmd)
{
   GLenum mode = cmd->mode;
   GLuint id = cmd->id;
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
   GLintptr surface = cmd->surface;
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
   GLenum access;
   GLintptr surface;
};
void
_mesa_unmarshal_VDPAUSurfaceAccessNV(struct gl_context *ctx, const struct marshal_cmd_VDPAUSurfaceAccessNV *cmd)
{
   GLintptr surface = cmd->surface;
   GLenum access = cmd->access;
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
   GLsizei numSurfaces = cmd->numSurfaces;
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
   GLsizei numSurfaces = cmd->numSurfaces;
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
   GLsizei n = cmd->n;
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
   GLuint memoryObject;
   GLenum pname;
   /* Next safe_mul(_mesa_memobj_enum_to_count(pname), 1 * sizeof(GLint)) bytes are GLint params[_mesa_memobj_enum_to_count(pname)] */
};
void
_mesa_unmarshal_MemoryObjectParameterivEXT(struct gl_context *ctx, const struct marshal_cmd_MemoryObjectParameterivEXT *cmd)
{
   GLuint memoryObject = cmd->memoryObject;
   GLenum pname = cmd->pname;
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
   GLenum target;
   GLsizei levels;
   GLenum internalFormat;
   GLsizei width;
   GLsizei height;
   GLuint memory;
   GLuint64 offset;
};
void
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
   GLenum target;
   GLsizei samples;
   GLenum internalFormat;
   GLsizei width;
   GLsizei height;
   GLuint memory;
   GLuint64 offset;
};
void
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
   GLenum target;
   GLsizei levels;
   GLenum internalFormat;
   GLsizei width;
   GLsizei height;
   GLsizei depth;
   GLuint memory;
   GLuint64 offset;
};
void
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
   GLenum target;
   GLsizei samples;
   GLenum internalFormat;
   GLsizei width;
   GLsizei height;
   GLsizei depth;
   GLuint memory;
   GLuint64 offset;
};
void
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
   GLenum target;
   GLuint memory;
   GLsizeiptr size;
   GLuint64 offset;
};
void
_mesa_unmarshal_BufferStorageMemEXT(struct gl_context *ctx, const struct marshal_cmd_BufferStorageMemEXT *cmd)
{
   GLenum target = cmd->target;
   GLsizeiptr size = cmd->size;
   GLuint memory = cmd->memory;
   GLuint64 offset = cmd->offset;
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
   GLenum texture;
   GLsizei levels;
   GLenum internalFormat;
   GLsizei width;
   GLsizei height;
   GLuint memory;
   GLuint64 offset;
};
void
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
   GLuint texture;
   GLsizei samples;
   GLenum internalFormat;
   GLsizei width;
   GLsizei height;
   GLuint memory;
   GLuint64 offset;
};
void
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
   GLuint texture;
   GLsizei levels;
   GLenum internalFormat;
   GLsizei width;
   GLsizei height;
   GLsizei depth;
   GLuint memory;
   GLuint64 offset;
};
void
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
   GLuint texture;
   GLsizei samples;
   GLenum internalFormat;
   GLsizei width;
   GLsizei height;
   GLsizei depth;
   GLuint memory;
   GLuint64 offset;
};
void
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
   GLuint buffer = cmd->buffer;
   GLsizeiptr size = cmd->size;
   GLuint memory = cmd->memory;
   GLuint64 offset = cmd->offset;
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
   GLenum target;
   GLsizei levels;
   GLenum internalFormat;
   GLsizei width;
   GLuint memory;
   GLuint64 offset;
};
void
_mesa_unmarshal_TexStorageMem1DEXT(struct gl_context *ctx, const struct marshal_cmd_TexStorageMem1DEXT *cmd)
{
   GLenum target = cmd->target;
   GLsizei levels = cmd->levels;
   GLenum internalFormat = cmd->internalFormat;
   GLsizei width = cmd->width;
   GLuint memory = cmd->memory;
   GLuint64 offset = cmd->offset;
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
   GLuint texture;
   GLsizei levels;
   GLenum internalFormat;
   GLsizei width;
   GLuint memory;
   GLuint64 offset;
};
void
_mesa_unmarshal_TextureStorageMem1DEXT(struct gl_context *ctx, const struct marshal_cmd_TextureStorageMem1DEXT *cmd)
{
   GLuint texture = cmd->texture;
   GLsizei levels = cmd->levels;
   GLenum internalFormat = cmd->internalFormat;
   GLsizei width = cmd->width;
   GLuint memory = cmd->memory;
   GLuint64 offset = cmd->offset;
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
   GLsizei n = cmd->n;
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
   GLuint semaphore;
   GLenum pname;
   /* Next safe_mul(_mesa_semaphore_enum_to_count(pname), 1 * sizeof(GLuint64)) bytes are GLuint64 params[_mesa_semaphore_enum_to_count(pname)] */
};
void
_mesa_unmarshal_SemaphoreParameterui64vEXT(struct gl_context *ctx, const struct marshal_cmd_SemaphoreParameterui64vEXT *cmd)
{
   GLuint semaphore = cmd->semaphore;
   GLenum pname = cmd->pname;
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
   GLuint semaphore = cmd->semaphore;
   GLuint numBufferBarriers = cmd->numBufferBarriers;
   GLuint numTextureBarriers = cmd->numTextureBarriers;
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
   GLuint semaphore = cmd->semaphore;
   GLuint numBufferBarriers = cmd->numBufferBarriers;
   GLuint numTextureBarriers = cmd->numTextureBarriers;
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
   GLuint memory;
   GLenum handleType;
   GLint fd;
   GLuint64 size;
};
void
_mesa_unmarshal_ImportMemoryFdEXT(struct gl_context *ctx, const struct marshal_cmd_ImportMemoryFdEXT *cmd)
{
   GLuint memory = cmd->memory;
   GLuint64 size = cmd->size;
   GLenum handleType = cmd->handleType;
   GLint fd = cmd->fd;
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
   GLuint semaphore;
   GLenum handleType;
   GLint fd;
};
void
_mesa_unmarshal_ImportSemaphoreFdEXT(struct gl_context *ctx, const struct marshal_cmd_ImportSemaphoreFdEXT *cmd)
{
   GLuint semaphore = cmd->semaphore;
   GLenum handleType = cmd->handleType;
   GLint fd = cmd->fd;
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
   GLuint index;
   GLenum swizzlex;
   GLenum swizzley;
   GLenum swizzlez;
   GLenum swizzlew;
};
void
_mesa_unmarshal_ViewportSwizzleNV(struct gl_context *ctx, const struct marshal_cmd_ViewportSwizzleNV *cmd)
{
   GLuint index = cmd->index;
   GLenum swizzlex = cmd->swizzlex;
   GLenum swizzley = cmd->swizzley;
   GLenum swizzlez = cmd->swizzlez;
   GLenum swizzlew = cmd->swizzlew;
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


/* Vertex2hNV: marshalled asynchronously */
struct marshal_cmd_Vertex2hNV
{
   struct marshal_cmd_base cmd_base;
   GLhalfNV x;
   GLhalfNV y;
};
void
_mesa_unmarshal_Vertex2hNV(struct gl_context *ctx, const struct marshal_cmd_Vertex2hNV *cmd)
{
   GLhalfNV x = cmd->x;
   GLhalfNV y = cmd->y;
   CALL_Vertex2hNV(ctx->CurrentServerDispatch, (x, y));
}
void GLAPIENTRY
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
void
_mesa_unmarshal_Vertex2hvNV(struct gl_context *ctx, const struct marshal_cmd_Vertex2hvNV *cmd)
{
   const GLhalfNV * v = cmd->v;
   CALL_Vertex2hvNV(ctx->CurrentServerDispatch, (v));
}
void GLAPIENTRY
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
void
_mesa_unmarshal_Vertex3hNV(struct gl_context *ctx, const struct marshal_cmd_Vertex3hNV *cmd)
{
   GLhalfNV x = cmd->x;
   GLhalfNV y = cmd->y;
   GLhalfNV z = cmd->z;
   CALL_Vertex3hNV(ctx->CurrentServerDispatch, (x, y, z));
}
void GLAPIENTRY
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
void
_mesa_unmarshal_Vertex3hvNV(struct gl_context *ctx, const struct marshal_cmd_Vertex3hvNV *cmd)
{
   const GLhalfNV * v = cmd->v;
   CALL_Vertex3hvNV(ctx->CurrentServerDispatch, (v));
}
void GLAPIENTRY
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
void
_mesa_unmarshal_Vertex4hNV(struct gl_context *ctx, const struct marshal_cmd_Vertex4hNV *cmd)
{
   GLhalfNV x = cmd->x;
   GLhalfNV y = cmd->y;
   GLhalfNV z = cmd->z;
   GLhalfNV w = cmd->w;
   CALL_Vertex4hNV(ctx->CurrentServerDispatch, (x, y, z, w));
}
void GLAPIENTRY
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
void
_mesa_unmarshal_Vertex4hvNV(struct gl_context *ctx, const struct marshal_cmd_Vertex4hvNV *cmd)
{
   const GLhalfNV * v = cmd->v;
   CALL_Vertex4hvNV(ctx->CurrentServerDispatch, (v));
}
void GLAPIENTRY
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
void
_mesa_unmarshal_Normal3hNV(struct gl_context *ctx, const struct marshal_cmd_Normal3hNV *cmd)
{
   GLhalfNV nx = cmd->nx;
   GLhalfNV ny = cmd->ny;
   GLhalfNV nz = cmd->nz;
   CALL_Normal3hNV(ctx->CurrentServerDispatch, (nx, ny, nz));
}
void GLAPIENTRY
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
void
_mesa_unmarshal_Normal3hvNV(struct gl_context *ctx, const struct marshal_cmd_Normal3hvNV *cmd)
{
   const GLhalfNV * v = cmd->v;
   CALL_Normal3hvNV(ctx->CurrentServerDispatch, (v));
}
void GLAPIENTRY
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
void
_mesa_unmarshal_Color3hNV(struct gl_context *ctx, const struct marshal_cmd_Color3hNV *cmd)
{
   GLhalfNV red = cmd->red;
   GLhalfNV green = cmd->green;
   GLhalfNV blue = cmd->blue;
   CALL_Color3hNV(ctx->CurrentServerDispatch, (red, green, blue));
}
void GLAPIENTRY
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
void
_mesa_unmarshal_Color3hvNV(struct gl_context *ctx, const struct marshal_cmd_Color3hvNV *cmd)
{
   const GLhalfNV * v = cmd->v;
   CALL_Color3hvNV(ctx->CurrentServerDispatch, (v));
}
void GLAPIENTRY
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
void
_mesa_unmarshal_Color4hNV(struct gl_context *ctx, const struct marshal_cmd_Color4hNV *cmd)
{
   GLhalfNV red = cmd->red;
   GLhalfNV green = cmd->green;
   GLhalfNV blue = cmd->blue;
   GLhalfNV alpha = cmd->alpha;
   CALL_Color4hNV(ctx->CurrentServerDispatch, (red, green, blue, alpha));
}
void GLAPIENTRY
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
void
_mesa_unmarshal_Color4hvNV(struct gl_context *ctx, const struct marshal_cmd_Color4hvNV *cmd)
{
   const GLhalfNV * v = cmd->v;
   CALL_Color4hvNV(ctx->CurrentServerDispatch, (v));
}
void GLAPIENTRY
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
void
_mesa_unmarshal_TexCoord1hNV(struct gl_context *ctx, const struct marshal_cmd_TexCoord1hNV *cmd)
{
   GLhalfNV s = cmd->s;
   CALL_TexCoord1hNV(ctx->CurrentServerDispatch, (s));
}
void GLAPIENTRY
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
void
_mesa_unmarshal_TexCoord1hvNV(struct gl_context *ctx, const struct marshal_cmd_TexCoord1hvNV *cmd)
{
   const GLhalfNV * v = cmd->v;
   CALL_TexCoord1hvNV(ctx->CurrentServerDispatch, (v));
}
void GLAPIENTRY
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
void
_mesa_unmarshal_TexCoord2hNV(struct gl_context *ctx, const struct marshal_cmd_TexCoord2hNV *cmd)
{
   GLhalfNV s = cmd->s;
   GLhalfNV t = cmd->t;
   CALL_TexCoord2hNV(ctx->CurrentServerDispatch, (s, t));
}
void GLAPIENTRY
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
void
_mesa_unmarshal_TexCoord2hvNV(struct gl_context *ctx, const struct marshal_cmd_TexCoord2hvNV *cmd)
{
   const GLhalfNV * v = cmd->v;
   CALL_TexCoord2hvNV(ctx->CurrentServerDispatch, (v));
}
void GLAPIENTRY
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
void
_mesa_unmarshal_TexCoord3hNV(struct gl_context *ctx, const struct marshal_cmd_TexCoord3hNV *cmd)
{
   GLhalfNV s = cmd->s;
   GLhalfNV t = cmd->t;
   GLhalfNV r = cmd->r;
   CALL_TexCoord3hNV(ctx->CurrentServerDispatch, (s, t, r));
}
void GLAPIENTRY
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
void
_mesa_unmarshal_TexCoord3hvNV(struct gl_context *ctx, const struct marshal_cmd_TexCoord3hvNV *cmd)
{
   const GLhalfNV * v = cmd->v;
   CALL_TexCoord3hvNV(ctx->CurrentServerDispatch, (v));
}
void GLAPIENTRY
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
void
_mesa_unmarshal_TexCoord4hNV(struct gl_context *ctx, const struct marshal_cmd_TexCoord4hNV *cmd)
{
   GLhalfNV s = cmd->s;
   GLhalfNV t = cmd->t;
   GLhalfNV r = cmd->r;
   GLhalfNV q = cmd->q;
   CALL_TexCoord4hNV(ctx->CurrentServerDispatch, (s, t, r, q));
}
void GLAPIENTRY
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
void
_mesa_unmarshal_TexCoord4hvNV(struct gl_context *ctx, const struct marshal_cmd_TexCoord4hvNV *cmd)
{
   const GLhalfNV * v = cmd->v;
   CALL_TexCoord4hvNV(ctx->CurrentServerDispatch, (v));
}
void GLAPIENTRY
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
   GLhalfNV s;
   GLenum target;
};
void
_mesa_unmarshal_MultiTexCoord1hNV(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord1hNV *cmd)
{
   GLenum target = cmd->target;
   GLhalfNV s = cmd->s;
   CALL_MultiTexCoord1hNV(ctx->CurrentServerDispatch, (target, s));
}
void GLAPIENTRY
_mesa_marshal_MultiTexCoord1hNV(GLenum target, GLhalfNV s)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexCoord1hNV);
   struct marshal_cmd_MultiTexCoord1hNV *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexCoord1hNV, cmd_size);
   cmd->target = target;
   cmd->s = s;
}


/* MultiTexCoord1hvNV: marshalled asynchronously */
struct marshal_cmd_MultiTexCoord1hvNV
{
   struct marshal_cmd_base cmd_base;
   GLenum target;
   GLhalfNV v[1];
};
void
_mesa_unmarshal_MultiTexCoord1hvNV(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord1hvNV *cmd)
{
   GLenum target = cmd->target;
   const GLhalfNV * v = cmd->v;
   CALL_MultiTexCoord1hvNV(ctx->CurrentServerDispatch, (target, v));
}
void GLAPIENTRY
_mesa_marshal_MultiTexCoord1hvNV(GLenum target, const GLhalfNV * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexCoord1hvNV);
   struct marshal_cmd_MultiTexCoord1hvNV *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexCoord1hvNV, cmd_size);
   cmd->target = target;
   memcpy(cmd->v, v, 1 * sizeof(GLhalfNV));
}


/* MultiTexCoord2hNV: marshalled asynchronously */
struct marshal_cmd_MultiTexCoord2hNV
{
   struct marshal_cmd_base cmd_base;
   GLhalfNV s;
   GLhalfNV t;
   GLenum target;
};
void
_mesa_unmarshal_MultiTexCoord2hNV(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord2hNV *cmd)
{
   GLenum target = cmd->target;
   GLhalfNV s = cmd->s;
   GLhalfNV t = cmd->t;
   CALL_MultiTexCoord2hNV(ctx->CurrentServerDispatch, (target, s, t));
}
void GLAPIENTRY
_mesa_marshal_MultiTexCoord2hNV(GLenum target, GLhalfNV s, GLhalfNV t)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexCoord2hNV);
   struct marshal_cmd_MultiTexCoord2hNV *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexCoord2hNV, cmd_size);
   cmd->target = target;
   cmd->s = s;
   cmd->t = t;
}


/* MultiTexCoord2hvNV: marshalled asynchronously */
struct marshal_cmd_MultiTexCoord2hvNV
{
   struct marshal_cmd_base cmd_base;
   GLenum target;
   GLhalfNV v[2];
};
void
_mesa_unmarshal_MultiTexCoord2hvNV(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord2hvNV *cmd)
{
   GLenum target = cmd->target;
   const GLhalfNV * v = cmd->v;
   CALL_MultiTexCoord2hvNV(ctx->CurrentServerDispatch, (target, v));
}
void GLAPIENTRY
_mesa_marshal_MultiTexCoord2hvNV(GLenum target, const GLhalfNV * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexCoord2hvNV);
   struct marshal_cmd_MultiTexCoord2hvNV *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexCoord2hvNV, cmd_size);
   cmd->target = target;
   memcpy(cmd->v, v, 2 * sizeof(GLhalfNV));
}


/* MultiTexCoord3hNV: marshalled asynchronously */
struct marshal_cmd_MultiTexCoord3hNV
{
   struct marshal_cmd_base cmd_base;
   GLhalfNV s;
   GLhalfNV t;
   GLhalfNV r;
   GLenum target;
};
void
_mesa_unmarshal_MultiTexCoord3hNV(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord3hNV *cmd)
{
   GLenum target = cmd->target;
   GLhalfNV s = cmd->s;
   GLhalfNV t = cmd->t;
   GLhalfNV r = cmd->r;
   CALL_MultiTexCoord3hNV(ctx->CurrentServerDispatch, (target, s, t, r));
}
void GLAPIENTRY
_mesa_marshal_MultiTexCoord3hNV(GLenum target, GLhalfNV s, GLhalfNV t, GLhalfNV r)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexCoord3hNV);
   struct marshal_cmd_MultiTexCoord3hNV *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexCoord3hNV, cmd_size);
   cmd->target = target;
   cmd->s = s;
   cmd->t = t;
   cmd->r = r;
}


/* MultiTexCoord3hvNV: marshalled asynchronously */
struct marshal_cmd_MultiTexCoord3hvNV
{
   struct marshal_cmd_base cmd_base;
   GLenum target;
   GLhalfNV v[3];
};
void
_mesa_unmarshal_MultiTexCoord3hvNV(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord3hvNV *cmd)
{
   GLenum target = cmd->target;
   const GLhalfNV * v = cmd->v;
   CALL_MultiTexCoord3hvNV(ctx->CurrentServerDispatch, (target, v));
}
void GLAPIENTRY
_mesa_marshal_MultiTexCoord3hvNV(GLenum target, const GLhalfNV * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexCoord3hvNV);
   struct marshal_cmd_MultiTexCoord3hvNV *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexCoord3hvNV, cmd_size);
   cmd->target = target;
   memcpy(cmd->v, v, 3 * sizeof(GLhalfNV));
}


/* MultiTexCoord4hNV: marshalled asynchronously */
struct marshal_cmd_MultiTexCoord4hNV
{
   struct marshal_cmd_base cmd_base;
   GLhalfNV s;
   GLhalfNV t;
   GLhalfNV r;
   GLhalfNV q;
   GLenum target;
};
void
_mesa_unmarshal_MultiTexCoord4hNV(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord4hNV *cmd)
{
   GLenum target = cmd->target;
   GLhalfNV s = cmd->s;
   GLhalfNV t = cmd->t;
   GLhalfNV r = cmd->r;
   GLhalfNV q = cmd->q;
   CALL_MultiTexCoord4hNV(ctx->CurrentServerDispatch, (target, s, t, r, q));
}
void GLAPIENTRY
_mesa_marshal_MultiTexCoord4hNV(GLenum target, GLhalfNV s, GLhalfNV t, GLhalfNV r, GLhalfNV q)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexCoord4hNV);
   struct marshal_cmd_MultiTexCoord4hNV *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexCoord4hNV, cmd_size);
   cmd->target = target;
   cmd->s = s;
   cmd->t = t;
   cmd->r = r;
   cmd->q = q;
}


/* MultiTexCoord4hvNV: marshalled asynchronously */
struct marshal_cmd_MultiTexCoord4hvNV
{
   struct marshal_cmd_base cmd_base;
   GLenum target;
   GLhalfNV v[4];
};
void
_mesa_unmarshal_MultiTexCoord4hvNV(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord4hvNV *cmd)
{
   GLenum target = cmd->target;
   const GLhalfNV * v = cmd->v;
   CALL_MultiTexCoord4hvNV(ctx->CurrentServerDispatch, (target, v));
}
void GLAPIENTRY
_mesa_marshal_MultiTexCoord4hvNV(GLenum target, const GLhalfNV * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexCoord4hvNV);
   struct marshal_cmd_MultiTexCoord4hvNV *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexCoord4hvNV, cmd_size);
   cmd->target = target;
   memcpy(cmd->v, v, 4 * sizeof(GLhalfNV));
}


/* VertexAttrib1hNV: marshalled asynchronously */
struct marshal_cmd_VertexAttrib1hNV
{
   struct marshal_cmd_base cmd_base;
   GLhalfNV x;
   GLuint index;
};
void
_mesa_unmarshal_VertexAttrib1hNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib1hNV *cmd)
{
   GLuint index = cmd->index;
   GLhalfNV x = cmd->x;
   CALL_VertexAttrib1hNV(ctx->CurrentServerDispatch, (index, x));
}
void GLAPIENTRY
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
void
_mesa_unmarshal_VertexAttrib1hvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib1hvNV *cmd)
{
   GLuint index = cmd->index;
   const GLhalfNV * v = cmd->v;
   CALL_VertexAttrib1hvNV(ctx->CurrentServerDispatch, (index, v));
}
void GLAPIENTRY
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
void
_mesa_unmarshal_VertexAttrib2hNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib2hNV *cmd)
{
   GLuint index = cmd->index;
   GLhalfNV x = cmd->x;
   GLhalfNV y = cmd->y;
   CALL_VertexAttrib2hNV(ctx->CurrentServerDispatch, (index, x, y));
}
void GLAPIENTRY
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
void
_mesa_unmarshal_VertexAttrib2hvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib2hvNV *cmd)
{
   GLuint index = cmd->index;
   const GLhalfNV * v = cmd->v;
   CALL_VertexAttrib2hvNV(ctx->CurrentServerDispatch, (index, v));
}
void GLAPIENTRY
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
void
_mesa_unmarshal_VertexAttrib3hNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib3hNV *cmd)
{
   GLuint index = cmd->index;
   GLhalfNV x = cmd->x;
   GLhalfNV y = cmd->y;
   GLhalfNV z = cmd->z;
   CALL_VertexAttrib3hNV(ctx->CurrentServerDispatch, (index, x, y, z));
}
void GLAPIENTRY
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
void
_mesa_unmarshal_VertexAttrib3hvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib3hvNV *cmd)
{
   GLuint index = cmd->index;
   const GLhalfNV * v = cmd->v;
   CALL_VertexAttrib3hvNV(ctx->CurrentServerDispatch, (index, v));
}
void GLAPIENTRY
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
void
_mesa_unmarshal_VertexAttrib4hNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4hNV *cmd)
{
   GLuint index = cmd->index;
   GLhalfNV x = cmd->x;
   GLhalfNV y = cmd->y;
   GLhalfNV z = cmd->z;
   GLhalfNV w = cmd->w;
   CALL_VertexAttrib4hNV(ctx->CurrentServerDispatch, (index, x, y, z, w));
}
void GLAPIENTRY
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
void
_mesa_unmarshal_VertexAttrib4hvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4hvNV *cmd)
{
   GLuint index = cmd->index;
   const GLhalfNV * v = cmd->v;
   CALL_VertexAttrib4hvNV(ctx->CurrentServerDispatch, (index, v));
}
void GLAPIENTRY
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
void
_mesa_unmarshal_VertexAttribs1hvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttribs1hvNV *cmd)
{
   GLuint index = cmd->index;
   GLsizei n = cmd->n;
   GLhalfNV * v;
   const char *variable_data = (const char *) (cmd + 1);
   v = (GLhalfNV *) variable_data;
   CALL_VertexAttribs1hvNV(ctx->CurrentServerDispatch, (index, n, v));
}
void GLAPIENTRY
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
void
_mesa_unmarshal_VertexAttribs2hvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttribs2hvNV *cmd)
{
   GLuint index = cmd->index;
   GLsizei n = cmd->n;
   GLhalfNV * v;
   const char *variable_data = (const char *) (cmd + 1);
   v = (GLhalfNV *) variable_data;
   CALL_VertexAttribs2hvNV(ctx->CurrentServerDispatch, (index, n, v));
}
void GLAPIENTRY
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
void
_mesa_unmarshal_VertexAttribs3hvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttribs3hvNV *cmd)
{
   GLuint index = cmd->index;
   GLsizei n = cmd->n;
   GLhalfNV * v;
   const char *variable_data = (const char *) (cmd + 1);
   v = (GLhalfNV *) variable_data;
   CALL_VertexAttribs3hvNV(ctx->CurrentServerDispatch, (index, n, v));
}
void GLAPIENTRY
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
void
_mesa_unmarshal_VertexAttribs4hvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttribs4hvNV *cmd)
{
   GLuint index = cmd->index;
   GLsizei n = cmd->n;
   GLhalfNV * v;
   const char *variable_data = (const char *) (cmd + 1);
   v = (GLhalfNV *) variable_data;
   CALL_VertexAttribs4hvNV(ctx->CurrentServerDispatch, (index, n, v));
}
void GLAPIENTRY
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
void
_mesa_unmarshal_FogCoordhNV(struct gl_context *ctx, const struct marshal_cmd_FogCoordhNV *cmd)
{
   GLhalfNV x = cmd->x;
   CALL_FogCoordhNV(ctx->CurrentServerDispatch, (x));
}
void GLAPIENTRY
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
void
_mesa_unmarshal_FogCoordhvNV(struct gl_context *ctx, const struct marshal_cmd_FogCoordhvNV *cmd)
{
   const GLhalfNV * v = cmd->v;
   CALL_FogCoordhvNV(ctx->CurrentServerDispatch, (v));
}
void GLAPIENTRY
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
void
_mesa_unmarshal_SecondaryColor3hNV(struct gl_context *ctx, const struct marshal_cmd_SecondaryColor3hNV *cmd)
{
   GLhalfNV red = cmd->red;
   GLhalfNV green = cmd->green;
   GLhalfNV blue = cmd->blue;
   CALL_SecondaryColor3hNV(ctx->CurrentServerDispatch, (red, green, blue));
}
void GLAPIENTRY
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
void
_mesa_unmarshal_SecondaryColor3hvNV(struct gl_context *ctx, const struct marshal_cmd_SecondaryColor3hvNV *cmd)
{
   const GLhalfNV * v = cmd->v;
   CALL_SecondaryColor3hvNV(ctx->CurrentServerDispatch, (v));
}
void GLAPIENTRY
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
void
_mesa_unmarshal_MemoryBarrierByRegion(struct gl_context *ctx, const struct marshal_cmd_MemoryBarrierByRegion *cmd)
{
   GLbitfield barriers = cmd->barriers;
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
   GLenum func;
   GLclampx ref;
};
void
_mesa_unmarshal_AlphaFuncx(struct gl_context *ctx, const struct marshal_cmd_AlphaFuncx *cmd)
{
   GLenum func = cmd->func;
   GLclampx ref = cmd->ref;
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
   GLclampx red = cmd->red;
   GLclampx green = cmd->green;
   GLclampx blue = cmd->blue;
   GLclampx alpha = cmd->alpha;
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
   GLclampx depth = cmd->depth;
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
   GLfixed red = cmd->red;
   GLfixed green = cmd->green;
   GLfixed blue = cmd->blue;
   GLfixed alpha = cmd->alpha;
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
   GLclampx zNear = cmd->zNear;
   GLclampx zFar = cmd->zFar;
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
   GLenum pname;
   GLfixed param;
};
void
_mesa_unmarshal_Fogx(struct gl_context *ctx, const struct marshal_cmd_Fogx *cmd)
{
   GLenum pname = cmd->pname;
   GLfixed param = cmd->param;
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
   GLenum pname;
   /* Next safe_mul(_mesa_fog_enum_to_count(pname), 1 * sizeof(GLfixed)) bytes are GLfixed params[None] */
};
void
_mesa_unmarshal_Fogxv(struct gl_context *ctx, const struct marshal_cmd_Fogxv *cmd)
{
   GLenum pname = cmd->pname;
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
   GLfixed left = cmd->left;
   GLfixed right = cmd->right;
   GLfixed bottom = cmd->bottom;
   GLfixed top = cmd->top;
   GLfixed zNear = cmd->zNear;
   GLfixed zFar = cmd->zFar;
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
   GLenum pname;
   GLfixed param;
};
void
_mesa_unmarshal_LightModelx(struct gl_context *ctx, const struct marshal_cmd_LightModelx *cmd)
{
   GLenum pname = cmd->pname;
   GLfixed param = cmd->param;
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
   GLenum pname;
   /* Next safe_mul(_mesa_light_model_enum_to_count(pname), 1 * sizeof(GLfixed)) bytes are GLfixed params[None] */
};
void
_mesa_unmarshal_LightModelxv(struct gl_context *ctx, const struct marshal_cmd_LightModelxv *cmd)
{
   GLenum pname = cmd->pname;
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
   GLenum light;
   GLenum pname;
   GLfixed param;
};
void
_mesa_unmarshal_Lightx(struct gl_context *ctx, const struct marshal_cmd_Lightx *cmd)
{
   GLenum light = cmd->light;
   GLenum pname = cmd->pname;
   GLfixed param = cmd->param;
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
   GLenum light;
   GLenum pname;
   /* Next safe_mul(_mesa_light_enum_to_count(pname), 1 * sizeof(GLfixed)) bytes are GLfixed params[None] */
};
void
_mesa_unmarshal_Lightxv(struct gl_context *ctx, const struct marshal_cmd_Lightxv *cmd)
{
   GLenum light = cmd->light;
   GLenum pname = cmd->pname;
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
   GLfixed width = cmd->width;
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
   GLenum face;
   GLenum pname;
   GLfixed param;
};
void
_mesa_unmarshal_Materialx(struct gl_context *ctx, const struct marshal_cmd_Materialx *cmd)
{
   GLenum face = cmd->face;
   GLenum pname = cmd->pname;
   GLfixed param = cmd->param;
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
   GLenum face;
   GLenum pname;
   /* Next safe_mul(_mesa_material_enum_to_count(pname), 1 * sizeof(GLfixed)) bytes are GLfixed params[None] */
};
void
_mesa_unmarshal_Materialxv(struct gl_context *ctx, const struct marshal_cmd_Materialxv *cmd)
{
   GLenum face = cmd->face;
   GLenum pname = cmd->pname;
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
   GLenum target;
   GLfixed s;
   GLfixed t;
   GLfixed r;
   GLfixed q;
};
void
_mesa_unmarshal_MultiTexCoord4x(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord4x *cmd)
{
   GLenum target = cmd->target;
   GLfixed s = cmd->s;
   GLfixed t = cmd->t;
   GLfixed r = cmd->r;
   GLfixed q = cmd->q;
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
   GLfixed nx = cmd->nx;
   GLfixed ny = cmd->ny;
   GLfixed nz = cmd->nz;
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
   GLfixed left = cmd->left;
   GLfixed right = cmd->right;
   GLfixed bottom = cmd->bottom;
   GLfixed top = cmd->top;
   GLfixed zNear = cmd->zNear;
   GLfixed zFar = cmd->zFar;
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
   GLfixed size = cmd->size;
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
   GLfixed factor = cmd->factor;
   GLfixed units = cmd->units;
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
   GLfixed angle = cmd->angle;
   GLfixed x = cmd->x;
   GLfixed y = cmd->y;
   GLfixed z = cmd->z;
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
   GLclampx value = cmd->value;
   GLboolean invert = cmd->invert;
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
   GLfixed x = cmd->x;
   GLfixed y = cmd->y;
   GLfixed z = cmd->z;
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
   GLenum target;
   GLenum pname;
   GLfixed param;
};
void
_mesa_unmarshal_TexEnvx(struct gl_context *ctx, const struct marshal_cmd_TexEnvx *cmd)
{
   GLenum target = cmd->target;
   GLenum pname = cmd->pname;
   GLfixed param = cmd->param;
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
   GLenum target;
   GLenum pname;
   /* Next safe_mul(_mesa_texenv_enum_to_count(pname), 1 * sizeof(GLfixed)) bytes are GLfixed params[None] */
};
void
_mesa_unmarshal_TexEnvxv(struct gl_context *ctx, const struct marshal_cmd_TexEnvxv *cmd)
{
   GLenum target = cmd->target;
   GLenum pname = cmd->pname;
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
   GLenum target;
   GLenum pname;
   GLfixed param;
};
void
_mesa_unmarshal_TexParameterx(struct gl_context *ctx, const struct marshal_cmd_TexParameterx *cmd)
{
   GLenum target = cmd->target;
   GLenum pname = cmd->pname;
   GLfixed param = cmd->param;
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
   GLfixed x = cmd->x;
   GLfixed y = cmd->y;
   GLfixed z = cmd->z;
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
   GLenum plane;
   GLfixed equation[4];
};
void
_mesa_unmarshal_ClipPlanex(struct gl_context *ctx, const struct marshal_cmd_ClipPlanex *cmd)
{
   GLenum plane = cmd->plane;
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
   GLenum pname;
   GLfixed param;
};
void
_mesa_unmarshal_PointParameterx(struct gl_context *ctx, const struct marshal_cmd_PointParameterx *cmd)
{
   GLenum pname = cmd->pname;
   GLfixed param = cmd->param;
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
   GLenum pname;
   /* Next safe_mul(_mesa_point_param_enum_to_count(pname), 1 * sizeof(GLfixed)) bytes are GLfixed params[_mesa_point_param_enum_to_count(pname)] */
};
void
_mesa_unmarshal_PointParameterxv(struct gl_context *ctx, const struct marshal_cmd_PointParameterxv *cmd)
{
   GLenum pname = cmd->pname;
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
   GLenum target;
   GLenum pname;
   /* Next safe_mul(_mesa_tex_param_enum_to_count(pname), 1 * sizeof(GLfixed)) bytes are GLfixed params[None] */
};
void
_mesa_unmarshal_TexParameterxv(struct gl_context *ctx, const struct marshal_cmd_TexParameterxv *cmd)
{
   GLenum target = cmd->target;
   GLenum pname = cmd->pname;
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
   GLenum coord;
   GLenum pname;
   GLint param;
};
void
_mesa_unmarshal_TexGenxOES(struct gl_context *ctx, const struct marshal_cmd_TexGenxOES *cmd)
{
   GLenum coord = cmd->coord;
   GLenum pname = cmd->pname;
   GLint param = cmd->param;
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
   GLenum coord;
   GLenum pname;
   /* Next safe_mul(_mesa_texgen_enum_to_count(pname), 1 * sizeof(GLfixed)) bytes are GLfixed params[None] */
};
void
_mesa_unmarshal_TexGenxvOES(struct gl_context *ctx, const struct marshal_cmd_TexGenxvOES *cmd)
{
   GLenum coord = cmd->coord;
   GLenum pname = cmd->pname;
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
   GLenum plane;
   GLfloat equation[4];
};
void
_mesa_unmarshal_ClipPlanef(struct gl_context *ctx, const struct marshal_cmd_ClipPlanef *cmd)
{
   GLenum plane = cmd->plane;
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
   GLfloat left = cmd->left;
   GLfloat right = cmd->right;
   GLfloat bottom = cmd->bottom;
   GLfloat top = cmd->top;
   GLfloat zNear = cmd->zNear;
   GLfloat zFar = cmd->zFar;
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
   GLfloat left = cmd->left;
   GLfloat right = cmd->right;
   GLfloat bottom = cmd->bottom;
   GLfloat top = cmd->top;
   GLfloat zNear = cmd->zNear;
   GLfloat zFar = cmd->zFar;
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
   GLint x = cmd->x;
   GLint y = cmd->y;
   GLint z = cmd->z;
   GLint width = cmd->width;
   GLint height = cmd->height;
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
   GLfloat x = cmd->x;
   GLfloat y = cmd->y;
   GLfloat z = cmd->z;
   GLfloat width = cmd->width;
   GLfloat height = cmd->height;
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
   GLshort x = cmd->x;
   GLshort y = cmd->y;
   GLshort z = cmd->z;
   GLshort width = cmd->width;
   GLshort height = cmd->height;
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
   GLfixed x = cmd->x;
   GLfixed y = cmd->y;
   GLfixed z = cmd->z;
   GLfixed width = cmd->width;
   GLfixed height = cmd->height;
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
   GLenum type;
   GLsizei stride;
   const GLvoid * pointer;
};
void
_mesa_unmarshal_PointSizePointerOES(struct gl_context *ctx, const struct marshal_cmd_PointSizePointerOES *cmd)
{
   GLenum type = cmd->type;
   GLsizei stride = cmd->stride;
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
   if (COMPAT) _mesa_glthread_AttribPointer(ctx, VERT_ATTRIB_POINT_SIZE, 1, type, stride, pointer);
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
   GLenum target;
   GLsizei numAttachments;
   /* Next safe_mul(numAttachments, 1 * sizeof(GLenum)) bytes are GLenum attachments[numAttachments] */
};
void
_mesa_unmarshal_DiscardFramebufferEXT(struct gl_context *ctx, const struct marshal_cmd_DiscardFramebufferEXT *cmd)
{
   GLenum target = cmd->target;
   GLsizei numAttachments = cmd->numAttachments;
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
   GLenum target;
   GLenum attachment;
   GLenum textarget;
   GLuint texture;
   GLint level;
   GLsizei samples;
};
void
_mesa_unmarshal_FramebufferTexture2DMultisampleEXT(struct gl_context *ctx, const struct marshal_cmd_FramebufferTexture2DMultisampleEXT *cmd)
{
   GLenum target = cmd->target;
   GLenum attachment = cmd->attachment;
   GLenum textarget = cmd->textarget;
   GLuint texture = cmd->texture;
   GLint level = cmd->level;
   GLsizei samples = cmd->samples;
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
   GLuint first = cmd->first;
   GLsizei count = cmd->count;
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
   GLuint index = cmd->index;
   GLfloat n = cmd->n;
   GLfloat f = cmd->f;
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
   GLenum target;
   GLenum pname;
   GLint param;
};
void
_mesa_unmarshal_FramebufferParameteriMESA(struct gl_context *ctx, const struct marshal_cmd_FramebufferParameteriMESA *cmd)
{
   GLenum target = cmd->target;
   GLenum pname = cmd->pname;
   GLint param = cmd->param;
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


