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

/* MatrixPopEXT: marshalled asynchronously */
struct marshal_cmd_MatrixPopEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 matrixMode;
};
uint32_t
_mesa_unmarshal_MatrixPopEXT(struct gl_context *ctx, const struct marshal_cmd_MatrixPopEXT *cmd)
{
   GLenum matrixMode = cmd->matrixMode;
   CALL_MatrixPopEXT(ctx->CurrentServerDispatch, (matrixMode));
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
_mesa_unmarshal_ClientAttribDefaultEXT(struct gl_context *ctx, const struct marshal_cmd_ClientAttribDefaultEXT *cmd)
{
   GLbitfield mask = cmd->mask;
   CALL_ClientAttribDefaultEXT(ctx->CurrentServerDispatch, (mask));
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


/* PushClientAttribDefaultEXT: marshalled asynchronously */
struct marshal_cmd_PushClientAttribDefaultEXT
{
   struct marshal_cmd_base cmd_base;
   GLbitfield mask;
};
uint32_t
_mesa_unmarshal_PushClientAttribDefaultEXT(struct gl_context *ctx, const struct marshal_cmd_PushClientAttribDefaultEXT *cmd)
{
   GLbitfield mask = cmd->mask;
   CALL_PushClientAttribDefaultEXT(ctx->CurrentServerDispatch, (mask));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_PushClientAttribDefaultEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_PushClientAttribDefaultEXT(GLbitfield mask)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_PushClientAttribDefaultEXT);
   struct marshal_cmd_PushClientAttribDefaultEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_PushClientAttribDefaultEXT, cmd_size);
   cmd->mask = mask;
   if (COMPAT) _mesa_glthread_PushClientAttrib(ctx, mask, true);
}


/* GetTextureParameterivEXT: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetTextureParameterivEXT(GLuint texture, GLenum target, GLenum pname, GLint * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetTextureParameterivEXT");
   CALL_GetTextureParameterivEXT(ctx->CurrentServerDispatch, (texture, target, pname, params));
}


/* GetTextureParameterfvEXT: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetTextureParameterfvEXT(GLuint texture, GLenum target, GLenum pname, float * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetTextureParameterfvEXT");
   CALL_GetTextureParameterfvEXT(ctx->CurrentServerDispatch, (texture, target, pname, params));
}


/* GetTextureLevelParameterivEXT: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetTextureLevelParameterivEXT(GLuint texture, GLenum target, GLint level, GLenum pname, GLint * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetTextureLevelParameterivEXT");
   CALL_GetTextureLevelParameterivEXT(ctx->CurrentServerDispatch, (texture, target, level, pname, params));
}


/* GetTextureLevelParameterfvEXT: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetTextureLevelParameterfvEXT(GLuint texture, GLenum target, GLint level, GLenum pname, float * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetTextureLevelParameterfvEXT");
   CALL_GetTextureLevelParameterfvEXT(ctx->CurrentServerDispatch, (texture, target, level, pname, params));
}


/* TextureParameteriEXT: marshalled asynchronously */
struct marshal_cmd_TextureParameteriEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLenum16 pname;
   GLuint texture;
   int param;
};
uint32_t
_mesa_unmarshal_TextureParameteriEXT(struct gl_context *ctx, const struct marshal_cmd_TextureParameteriEXT *cmd)
{
   GLuint texture = cmd->texture;
   GLenum target = cmd->target;
   GLenum pname = cmd->pname;
   int param = cmd->param;
   CALL_TextureParameteriEXT(ctx->CurrentServerDispatch, (texture, target, pname, param));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_TextureParameteriEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_TextureParameteriEXT(GLuint texture, GLenum target, GLenum pname, int param)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TextureParameteriEXT);
   struct marshal_cmd_TextureParameteriEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TextureParameteriEXT, cmd_size);
   cmd->texture = texture;
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->pname = MIN2(pname, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->param = param;
}


/* TextureParameterivEXT: marshalled asynchronously */
struct marshal_cmd_TextureParameterivEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLenum16 pname;
   GLuint texture;
   /* Next safe_mul(_mesa_tex_param_enum_to_count(pname), 1 * sizeof(GLint)) bytes are GLint params[_mesa_tex_param_enum_to_count(pname)] */
};
uint32_t
_mesa_unmarshal_TextureParameterivEXT(struct gl_context *ctx, const struct marshal_cmd_TextureParameterivEXT *cmd)
{
   GLuint texture = cmd->texture;
   GLenum target = cmd->target;
   GLenum pname = cmd->pname;
   GLint *params;
   const char *variable_data = (const char *) (cmd + 1);
   params = (GLint *) variable_data;
   CALL_TextureParameterivEXT(ctx->CurrentServerDispatch, (texture, target, pname, params));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_TextureParameterivEXT(GLuint texture, GLenum target, GLenum pname, const GLint * params)
{
   GET_CURRENT_CONTEXT(ctx);
   int params_size = safe_mul(_mesa_tex_param_enum_to_count(pname), 1 * sizeof(GLint));
   int cmd_size = sizeof(struct marshal_cmd_TextureParameterivEXT) + params_size;
   struct marshal_cmd_TextureParameterivEXT *cmd;
   if (unlikely(params_size < 0 || (params_size > 0 && !params) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "TextureParameterivEXT");
      CALL_TextureParameterivEXT(ctx->CurrentServerDispatch, (texture, target, pname, params));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TextureParameterivEXT, cmd_size);
   cmd->texture = texture;
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->pname = MIN2(pname, 0xffff); /* clamped to 0xffff (invalid enum) */
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, params, params_size);
}


/* TextureParameterfEXT: marshalled asynchronously */
struct marshal_cmd_TextureParameterfEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLenum16 pname;
   GLuint texture;
   float param;
};
uint32_t
_mesa_unmarshal_TextureParameterfEXT(struct gl_context *ctx, const struct marshal_cmd_TextureParameterfEXT *cmd)
{
   GLuint texture = cmd->texture;
   GLenum target = cmd->target;
   GLenum pname = cmd->pname;
   float param = cmd->param;
   CALL_TextureParameterfEXT(ctx->CurrentServerDispatch, (texture, target, pname, param));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_TextureParameterfEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_TextureParameterfEXT(GLuint texture, GLenum target, GLenum pname, float param)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TextureParameterfEXT);
   struct marshal_cmd_TextureParameterfEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TextureParameterfEXT, cmd_size);
   cmd->texture = texture;
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->pname = MIN2(pname, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->param = param;
}


/* TextureParameterfvEXT: marshalled asynchronously */
struct marshal_cmd_TextureParameterfvEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLenum16 pname;
   GLuint texture;
   /* Next safe_mul(_mesa_tex_param_enum_to_count(pname), 1 * sizeof(float)) bytes are float params[_mesa_tex_param_enum_to_count(pname)] */
};
uint32_t
_mesa_unmarshal_TextureParameterfvEXT(struct gl_context *ctx, const struct marshal_cmd_TextureParameterfvEXT *cmd)
{
   GLuint texture = cmd->texture;
   GLenum target = cmd->target;
   GLenum pname = cmd->pname;
   float *params;
   const char *variable_data = (const char *) (cmd + 1);
   params = (float *) variable_data;
   CALL_TextureParameterfvEXT(ctx->CurrentServerDispatch, (texture, target, pname, params));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_TextureParameterfvEXT(GLuint texture, GLenum target, GLenum pname, const float * params)
{
   GET_CURRENT_CONTEXT(ctx);
   int params_size = safe_mul(_mesa_tex_param_enum_to_count(pname), 1 * sizeof(float));
   int cmd_size = sizeof(struct marshal_cmd_TextureParameterfvEXT) + params_size;
   struct marshal_cmd_TextureParameterfvEXT *cmd;
   if (unlikely(params_size < 0 || (params_size > 0 && !params) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "TextureParameterfvEXT");
      CALL_TextureParameterfvEXT(ctx->CurrentServerDispatch, (texture, target, pname, params));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TextureParameterfvEXT, cmd_size);
   cmd->texture = texture;
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->pname = MIN2(pname, 0xffff); /* clamped to 0xffff (invalid enum) */
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, params, params_size);
}


/* TextureImage1DEXT: marshalled asynchronously */
struct marshal_cmd_TextureImage1DEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLenum16 format;
   GLenum16 type;
   GLuint texture;
   GLint level;
   GLint internalFormat;
   GLsizei width;
   GLint border;
   const GLvoid * pixels;
};
uint32_t
_mesa_unmarshal_TextureImage1DEXT(struct gl_context *ctx, const struct marshal_cmd_TextureImage1DEXT *cmd)
{
   GLuint texture = cmd->texture;
   GLenum target = cmd->target;
   GLint level = cmd->level;
   GLint internalFormat = cmd->internalFormat;
   GLsizei width = cmd->width;
   GLint border = cmd->border;
   GLenum format = cmd->format;
   GLenum type = cmd->type;
   const GLvoid * pixels = cmd->pixels;
   CALL_TextureImage1DEXT(ctx->CurrentServerDispatch, (texture, target, level, internalFormat, width, border, format, type, pixels));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_TextureImage1DEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_TextureImage1DEXT(GLuint texture, GLenum target, GLint level, GLint internalFormat, GLsizei width, GLint border, GLenum format, GLenum type, const GLvoid * pixels)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TextureImage1DEXT);
   struct marshal_cmd_TextureImage1DEXT *cmd;
   if (_mesa_glthread_has_no_unpack_buffer(ctx)) {
      _mesa_glthread_finish_before(ctx, "TextureImage1DEXT");
      CALL_TextureImage1DEXT(ctx->CurrentServerDispatch, (texture, target, level, internalFormat, width, border, format, type, pixels));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TextureImage1DEXT, cmd_size);
   cmd->texture = texture;
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->level = level;
   cmd->internalFormat = internalFormat;
   cmd->width = width;
   cmd->border = border;
   cmd->format = MIN2(format, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->type = MIN2(type, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->pixels = pixels;
}


/* TextureImage2DEXT: marshalled asynchronously */
struct marshal_cmd_TextureImage2DEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLenum16 format;
   GLenum16 type;
   GLuint texture;
   GLint level;
   GLint internalFormat;
   GLsizei width;
   GLsizei height;
   GLint border;
   const GLvoid * pixels;
};
uint32_t
_mesa_unmarshal_TextureImage2DEXT(struct gl_context *ctx, const struct marshal_cmd_TextureImage2DEXT *cmd)
{
   GLuint texture = cmd->texture;
   GLenum target = cmd->target;
   GLint level = cmd->level;
   GLint internalFormat = cmd->internalFormat;
   GLsizei width = cmd->width;
   GLsizei height = cmd->height;
   GLint border = cmd->border;
   GLenum format = cmd->format;
   GLenum type = cmd->type;
   const GLvoid * pixels = cmd->pixels;
   CALL_TextureImage2DEXT(ctx->CurrentServerDispatch, (texture, target, level, internalFormat, width, height, border, format, type, pixels));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_TextureImage2DEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_TextureImage2DEXT(GLuint texture, GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid * pixels)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TextureImage2DEXT);
   struct marshal_cmd_TextureImage2DEXT *cmd;
   if (_mesa_glthread_has_no_unpack_buffer(ctx)) {
      _mesa_glthread_finish_before(ctx, "TextureImage2DEXT");
      CALL_TextureImage2DEXT(ctx->CurrentServerDispatch, (texture, target, level, internalFormat, width, height, border, format, type, pixels));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TextureImage2DEXT, cmd_size);
   cmd->texture = texture;
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->level = level;
   cmd->internalFormat = internalFormat;
   cmd->width = width;
   cmd->height = height;
   cmd->border = border;
   cmd->format = MIN2(format, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->type = MIN2(type, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->pixels = pixels;
}


/* TextureImage3DEXT: marshalled asynchronously */
struct marshal_cmd_TextureImage3DEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLenum16 format;
   GLenum16 type;
   GLuint texture;
   GLint level;
   GLint internalFormat;
   GLsizei width;
   GLsizei height;
   GLsizei depth;
   GLint border;
   const GLvoid * pixels;
};
uint32_t
_mesa_unmarshal_TextureImage3DEXT(struct gl_context *ctx, const struct marshal_cmd_TextureImage3DEXT *cmd)
{
   GLuint texture = cmd->texture;
   GLenum target = cmd->target;
   GLint level = cmd->level;
   GLint internalFormat = cmd->internalFormat;
   GLsizei width = cmd->width;
   GLsizei height = cmd->height;
   GLsizei depth = cmd->depth;
   GLint border = cmd->border;
   GLenum format = cmd->format;
   GLenum type = cmd->type;
   const GLvoid * pixels = cmd->pixels;
   CALL_TextureImage3DEXT(ctx->CurrentServerDispatch, (texture, target, level, internalFormat, width, height, depth, border, format, type, pixels));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_TextureImage3DEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_TextureImage3DEXT(GLuint texture, GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid * pixels)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TextureImage3DEXT);
   struct marshal_cmd_TextureImage3DEXT *cmd;
   if (_mesa_glthread_has_no_unpack_buffer(ctx)) {
      _mesa_glthread_finish_before(ctx, "TextureImage3DEXT");
      CALL_TextureImage3DEXT(ctx->CurrentServerDispatch, (texture, target, level, internalFormat, width, height, depth, border, format, type, pixels));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TextureImage3DEXT, cmd_size);
   cmd->texture = texture;
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->level = level;
   cmd->internalFormat = internalFormat;
   cmd->width = width;
   cmd->height = height;
   cmd->depth = depth;
   cmd->border = border;
   cmd->format = MIN2(format, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->type = MIN2(type, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->pixels = pixels;
}


/* TextureSubImage1DEXT: marshalled asynchronously */
struct marshal_cmd_TextureSubImage1DEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLenum16 format;
   GLenum16 type;
   GLuint texture;
   GLint level;
   GLint xoffset;
   GLsizei width;
   const GLvoid * pixels;
};
uint32_t
_mesa_unmarshal_TextureSubImage1DEXT(struct gl_context *ctx, const struct marshal_cmd_TextureSubImage1DEXT *cmd)
{
   GLuint texture = cmd->texture;
   GLenum target = cmd->target;
   GLint level = cmd->level;
   GLint xoffset = cmd->xoffset;
   GLsizei width = cmd->width;
   GLenum format = cmd->format;
   GLenum type = cmd->type;
   const GLvoid * pixels = cmd->pixels;
   CALL_TextureSubImage1DEXT(ctx->CurrentServerDispatch, (texture, target, level, xoffset, width, format, type, pixels));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_TextureSubImage1DEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_TextureSubImage1DEXT(GLuint texture, GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const GLvoid * pixels)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TextureSubImage1DEXT);
   struct marshal_cmd_TextureSubImage1DEXT *cmd;
   if (_mesa_glthread_has_no_unpack_buffer(ctx)) {
      _mesa_glthread_finish_before(ctx, "TextureSubImage1DEXT");
      CALL_TextureSubImage1DEXT(ctx->CurrentServerDispatch, (texture, target, level, xoffset, width, format, type, pixels));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TextureSubImage1DEXT, cmd_size);
   cmd->texture = texture;
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->level = level;
   cmd->xoffset = xoffset;
   cmd->width = width;
   cmd->format = MIN2(format, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->type = MIN2(type, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->pixels = pixels;
}


/* TextureSubImage2DEXT: marshalled asynchronously */
struct marshal_cmd_TextureSubImage2DEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
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
_mesa_unmarshal_TextureSubImage2DEXT(struct gl_context *ctx, const struct marshal_cmd_TextureSubImage2DEXT *cmd)
{
   GLuint texture = cmd->texture;
   GLenum target = cmd->target;
   GLint level = cmd->level;
   GLint xoffset = cmd->xoffset;
   GLint yoffset = cmd->yoffset;
   GLsizei width = cmd->width;
   GLsizei height = cmd->height;
   GLenum format = cmd->format;
   GLenum type = cmd->type;
   const GLvoid * pixels = cmd->pixels;
   CALL_TextureSubImage2DEXT(ctx->CurrentServerDispatch, (texture, target, level, xoffset, yoffset, width, height, format, type, pixels));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_TextureSubImage2DEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_TextureSubImage2DEXT(GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid * pixels)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TextureSubImage2DEXT);
   struct marshal_cmd_TextureSubImage2DEXT *cmd;
   if (_mesa_glthread_has_no_unpack_buffer(ctx)) {
      _mesa_glthread_finish_before(ctx, "TextureSubImage2DEXT");
      CALL_TextureSubImage2DEXT(ctx->CurrentServerDispatch, (texture, target, level, xoffset, yoffset, width, height, format, type, pixels));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TextureSubImage2DEXT, cmd_size);
   cmd->texture = texture;
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->level = level;
   cmd->xoffset = xoffset;
   cmd->yoffset = yoffset;
   cmd->width = width;
   cmd->height = height;
   cmd->format = MIN2(format, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->type = MIN2(type, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->pixels = pixels;
}


/* TextureSubImage3DEXT: marshalled asynchronously */
struct marshal_cmd_TextureSubImage3DEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
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
_mesa_unmarshal_TextureSubImage3DEXT(struct gl_context *ctx, const struct marshal_cmd_TextureSubImage3DEXT *cmd)
{
   GLuint texture = cmd->texture;
   GLenum target = cmd->target;
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
   CALL_TextureSubImage3DEXT(ctx->CurrentServerDispatch, (texture, target, level, xoffset, yoffset, zoffset, width, height, depth, format, type, pixels));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_TextureSubImage3DEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_TextureSubImage3DEXT(GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const GLvoid * pixels)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TextureSubImage3DEXT);
   struct marshal_cmd_TextureSubImage3DEXT *cmd;
   if (_mesa_glthread_has_no_unpack_buffer(ctx)) {
      _mesa_glthread_finish_before(ctx, "TextureSubImage3DEXT");
      CALL_TextureSubImage3DEXT(ctx->CurrentServerDispatch, (texture, target, level, xoffset, yoffset, zoffset, width, height, depth, format, type, pixels));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TextureSubImage3DEXT, cmd_size);
   cmd->texture = texture;
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
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


/* CopyTextureImage1DEXT: marshalled asynchronously */
struct marshal_cmd_CopyTextureImage1DEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLenum16 internalFormat;
   GLuint texture;
   GLint level;
   GLint x;
   GLint y;
   GLsizei width;
   int border;
};
uint32_t
_mesa_unmarshal_CopyTextureImage1DEXT(struct gl_context *ctx, const struct marshal_cmd_CopyTextureImage1DEXT *cmd)
{
   GLuint texture = cmd->texture;
   GLenum target = cmd->target;
   GLint level = cmd->level;
   GLenum internalFormat = cmd->internalFormat;
   GLint x = cmd->x;
   GLint y = cmd->y;
   GLsizei width = cmd->width;
   int border = cmd->border;
   CALL_CopyTextureImage1DEXT(ctx->CurrentServerDispatch, (texture, target, level, internalFormat, x, y, width, border));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_CopyTextureImage1DEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_CopyTextureImage1DEXT(GLuint texture, GLenum target, GLint level, GLenum internalFormat, GLint x, GLint y, GLsizei width, int border)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_CopyTextureImage1DEXT);
   struct marshal_cmd_CopyTextureImage1DEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_CopyTextureImage1DEXT, cmd_size);
   cmd->texture = texture;
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->level = level;
   cmd->internalFormat = MIN2(internalFormat, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->x = x;
   cmd->y = y;
   cmd->width = width;
   cmd->border = border;
}


/* CopyTextureImage2DEXT: marshalled asynchronously */
struct marshal_cmd_CopyTextureImage2DEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLenum16 internalFormat;
   GLuint texture;
   GLint level;
   GLint x;
   GLint y;
   GLsizei width;
   GLsizei height;
   int border;
};
uint32_t
_mesa_unmarshal_CopyTextureImage2DEXT(struct gl_context *ctx, const struct marshal_cmd_CopyTextureImage2DEXT *cmd)
{
   GLuint texture = cmd->texture;
   GLenum target = cmd->target;
   GLint level = cmd->level;
   GLenum internalFormat = cmd->internalFormat;
   GLint x = cmd->x;
   GLint y = cmd->y;
   GLsizei width = cmd->width;
   GLsizei height = cmd->height;
   int border = cmd->border;
   CALL_CopyTextureImage2DEXT(ctx->CurrentServerDispatch, (texture, target, level, internalFormat, x, y, width, height, border));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_CopyTextureImage2DEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_CopyTextureImage2DEXT(GLuint texture, GLenum target, GLint level, GLenum internalFormat, GLint x, GLint y, GLsizei width, GLsizei height, int border)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_CopyTextureImage2DEXT);
   struct marshal_cmd_CopyTextureImage2DEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_CopyTextureImage2DEXT, cmd_size);
   cmd->texture = texture;
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->level = level;
   cmd->internalFormat = MIN2(internalFormat, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->x = x;
   cmd->y = y;
   cmd->width = width;
   cmd->height = height;
   cmd->border = border;
}


/* CopyTextureSubImage1DEXT: marshalled asynchronously */
struct marshal_cmd_CopyTextureSubImage1DEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLuint texture;
   GLint level;
   GLint xoffset;
   GLint x;
   GLint y;
   GLsizei width;
};
uint32_t
_mesa_unmarshal_CopyTextureSubImage1DEXT(struct gl_context *ctx, const struct marshal_cmd_CopyTextureSubImage1DEXT *cmd)
{
   GLuint texture = cmd->texture;
   GLenum target = cmd->target;
   GLint level = cmd->level;
   GLint xoffset = cmd->xoffset;
   GLint x = cmd->x;
   GLint y = cmd->y;
   GLsizei width = cmd->width;
   CALL_CopyTextureSubImage1DEXT(ctx->CurrentServerDispatch, (texture, target, level, xoffset, x, y, width));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_CopyTextureSubImage1DEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_CopyTextureSubImage1DEXT(GLuint texture, GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_CopyTextureSubImage1DEXT);
   struct marshal_cmd_CopyTextureSubImage1DEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_CopyTextureSubImage1DEXT, cmd_size);
   cmd->texture = texture;
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->level = level;
   cmd->xoffset = xoffset;
   cmd->x = x;
   cmd->y = y;
   cmd->width = width;
}


/* CopyTextureSubImage2DEXT: marshalled asynchronously */
struct marshal_cmd_CopyTextureSubImage2DEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
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
_mesa_unmarshal_CopyTextureSubImage2DEXT(struct gl_context *ctx, const struct marshal_cmd_CopyTextureSubImage2DEXT *cmd)
{
   GLuint texture = cmd->texture;
   GLenum target = cmd->target;
   GLint level = cmd->level;
   GLint xoffset = cmd->xoffset;
   GLint yoffset = cmd->yoffset;
   GLint x = cmd->x;
   GLint y = cmd->y;
   GLsizei width = cmd->width;
   GLsizei height = cmd->height;
   CALL_CopyTextureSubImage2DEXT(ctx->CurrentServerDispatch, (texture, target, level, xoffset, yoffset, x, y, width, height));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_CopyTextureSubImage2DEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_CopyTextureSubImage2DEXT(GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_CopyTextureSubImage2DEXT);
   struct marshal_cmd_CopyTextureSubImage2DEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_CopyTextureSubImage2DEXT, cmd_size);
   cmd->texture = texture;
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->level = level;
   cmd->xoffset = xoffset;
   cmd->yoffset = yoffset;
   cmd->x = x;
   cmd->y = y;
   cmd->width = width;
   cmd->height = height;
}


/* CopyTextureSubImage3DEXT: marshalled asynchronously */
struct marshal_cmd_CopyTextureSubImage3DEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
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
_mesa_unmarshal_CopyTextureSubImage3DEXT(struct gl_context *ctx, const struct marshal_cmd_CopyTextureSubImage3DEXT *cmd)
{
   GLuint texture = cmd->texture;
   GLenum target = cmd->target;
   GLint level = cmd->level;
   GLint xoffset = cmd->xoffset;
   GLint yoffset = cmd->yoffset;
   GLint zoffset = cmd->zoffset;
   GLint x = cmd->x;
   GLint y = cmd->y;
   GLsizei width = cmd->width;
   GLsizei height = cmd->height;
   CALL_CopyTextureSubImage3DEXT(ctx->CurrentServerDispatch, (texture, target, level, xoffset, yoffset, zoffset, x, y, width, height));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_CopyTextureSubImage3DEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_CopyTextureSubImage3DEXT(GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_CopyTextureSubImage3DEXT);
   struct marshal_cmd_CopyTextureSubImage3DEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_CopyTextureSubImage3DEXT, cmd_size);
   cmd->texture = texture;
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->level = level;
   cmd->xoffset = xoffset;
   cmd->yoffset = yoffset;
   cmd->zoffset = zoffset;
   cmd->x = x;
   cmd->y = y;
   cmd->width = width;
   cmd->height = height;
}


/* GetTextureImageEXT: marshalled asynchronously */
struct marshal_cmd_GetTextureImageEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLenum16 format;
   GLenum16 type;
   GLuint texture;
   GLint level;
   GLvoid * pixels;
};
uint32_t
_mesa_unmarshal_GetTextureImageEXT(struct gl_context *ctx, const struct marshal_cmd_GetTextureImageEXT *cmd)
{
   GLuint texture = cmd->texture;
   GLenum target = cmd->target;
   GLint level = cmd->level;
   GLenum format = cmd->format;
   GLenum type = cmd->type;
   GLvoid * pixels = cmd->pixels;
   CALL_GetTextureImageEXT(ctx->CurrentServerDispatch, (texture, target, level, format, type, pixels));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_GetTextureImageEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_GetTextureImageEXT(GLuint texture, GLenum target, GLint level, GLenum format, GLenum type, GLvoid * pixels)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_GetTextureImageEXT);
   struct marshal_cmd_GetTextureImageEXT *cmd;
   if (_mesa_glthread_has_no_pack_buffer(ctx)) {
      _mesa_glthread_finish_before(ctx, "GetTextureImageEXT");
      CALL_GetTextureImageEXT(ctx->CurrentServerDispatch, (texture, target, level, format, type, pixels));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_GetTextureImageEXT, cmd_size);
   cmd->texture = texture;
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->level = level;
   cmd->format = MIN2(format, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->type = MIN2(type, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->pixels = pixels;
}


/* BindMultiTextureEXT: marshalled asynchronously */
struct marshal_cmd_BindMultiTextureEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 texunit;
   GLenum16 target;
   GLuint texture;
};
uint32_t
_mesa_unmarshal_BindMultiTextureEXT(struct gl_context *ctx, const struct marshal_cmd_BindMultiTextureEXT *cmd)
{
   GLenum texunit = cmd->texunit;
   GLenum target = cmd->target;
   GLuint texture = cmd->texture;
   CALL_BindMultiTextureEXT(ctx->CurrentServerDispatch, (texunit, target, texture));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_BindMultiTextureEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_BindMultiTextureEXT(GLenum texunit, GLenum target, GLuint texture)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_BindMultiTextureEXT);
   struct marshal_cmd_BindMultiTextureEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_BindMultiTextureEXT, cmd_size);
   cmd->texunit = MIN2(texunit, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->texture = texture;
}


/* EnableClientStateiEXT: marshalled asynchronously */
struct marshal_cmd_EnableClientStateiEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 array;
   GLuint index;
};
uint32_t
_mesa_unmarshal_EnableClientStateiEXT(struct gl_context *ctx, const struct marshal_cmd_EnableClientStateiEXT *cmd)
{
   GLenum array = cmd->array;
   GLuint index = cmd->index;
   CALL_EnableClientStateiEXT(ctx->CurrentServerDispatch, (array, index));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_EnableClientStateiEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_EnableClientStateiEXT(GLenum array, GLuint index)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_EnableClientStateiEXT);
   struct marshal_cmd_EnableClientStateiEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_EnableClientStateiEXT, cmd_size);
   cmd->array = MIN2(array, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->index = index;
   if (COMPAT) _mesa_glthread_ClientState(ctx, NULL, VERT_ATTRIB_TEX(index), true);
}


/* DisableClientStateiEXT: marshalled asynchronously */
struct marshal_cmd_DisableClientStateiEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 array;
   GLuint index;
};
uint32_t
_mesa_unmarshal_DisableClientStateiEXT(struct gl_context *ctx, const struct marshal_cmd_DisableClientStateiEXT *cmd)
{
   GLenum array = cmd->array;
   GLuint index = cmd->index;
   CALL_DisableClientStateiEXT(ctx->CurrentServerDispatch, (array, index));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_DisableClientStateiEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_DisableClientStateiEXT(GLenum array, GLuint index)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_DisableClientStateiEXT);
   struct marshal_cmd_DisableClientStateiEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_DisableClientStateiEXT, cmd_size);
   cmd->array = MIN2(array, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->index = index;
   if (COMPAT) _mesa_glthread_ClientState(ctx, NULL, VERT_ATTRIB_TEX(index), false);
}


/* GetPointerIndexedvEXT: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetPointerIndexedvEXT(GLenum target, GLuint index, GLvoid** params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetPointerIndexedvEXT");
   CALL_GetPointerIndexedvEXT(ctx->CurrentServerDispatch, (target, index, params));
}


/* MultiTexEnviEXT: marshalled asynchronously */
struct marshal_cmd_MultiTexEnviEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 texunit;
   GLenum16 target;
   GLenum16 pname;
   GLint param;
};
uint32_t
_mesa_unmarshal_MultiTexEnviEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexEnviEXT *cmd)
{
   GLenum texunit = cmd->texunit;
   GLenum target = cmd->target;
   GLenum pname = cmd->pname;
   GLint param = cmd->param;
   CALL_MultiTexEnviEXT(ctx->CurrentServerDispatch, (texunit, target, pname, param));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MultiTexEnviEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_MultiTexEnviEXT(GLenum texunit, GLenum target, GLenum pname, GLint param)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexEnviEXT);
   struct marshal_cmd_MultiTexEnviEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexEnviEXT, cmd_size);
   cmd->texunit = MIN2(texunit, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->pname = MIN2(pname, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->param = param;
}


/* MultiTexEnvivEXT: marshalled asynchronously */
struct marshal_cmd_MultiTexEnvivEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 texunit;
   GLenum16 target;
   GLenum16 pname;
   /* Next safe_mul(_mesa_texenv_enum_to_count(pname), 1 * sizeof(GLint)) bytes are GLint param[_mesa_texenv_enum_to_count(pname)] */
};
uint32_t
_mesa_unmarshal_MultiTexEnvivEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexEnvivEXT *cmd)
{
   GLenum texunit = cmd->texunit;
   GLenum target = cmd->target;
   GLenum pname = cmd->pname;
   GLint *param;
   const char *variable_data = (const char *) (cmd + 1);
   param = (GLint *) variable_data;
   CALL_MultiTexEnvivEXT(ctx->CurrentServerDispatch, (texunit, target, pname, param));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_MultiTexEnvivEXT(GLenum texunit, GLenum target, GLenum pname, const GLint * param)
{
   GET_CURRENT_CONTEXT(ctx);
   int param_size = safe_mul(_mesa_texenv_enum_to_count(pname), 1 * sizeof(GLint));
   int cmd_size = sizeof(struct marshal_cmd_MultiTexEnvivEXT) + param_size;
   struct marshal_cmd_MultiTexEnvivEXT *cmd;
   if (unlikely(param_size < 0 || (param_size > 0 && !param) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "MultiTexEnvivEXT");
      CALL_MultiTexEnvivEXT(ctx->CurrentServerDispatch, (texunit, target, pname, param));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexEnvivEXT, cmd_size);
   cmd->texunit = MIN2(texunit, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->pname = MIN2(pname, 0xffff); /* clamped to 0xffff (invalid enum) */
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, param, param_size);
}


/* MultiTexEnvfEXT: marshalled asynchronously */
struct marshal_cmd_MultiTexEnvfEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 texunit;
   GLenum16 target;
   GLenum16 pname;
   GLfloat param;
};
uint32_t
_mesa_unmarshal_MultiTexEnvfEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexEnvfEXT *cmd)
{
   GLenum texunit = cmd->texunit;
   GLenum target = cmd->target;
   GLenum pname = cmd->pname;
   GLfloat param = cmd->param;
   CALL_MultiTexEnvfEXT(ctx->CurrentServerDispatch, (texunit, target, pname, param));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MultiTexEnvfEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_MultiTexEnvfEXT(GLenum texunit, GLenum target, GLenum pname, GLfloat param)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexEnvfEXT);
   struct marshal_cmd_MultiTexEnvfEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexEnvfEXT, cmd_size);
   cmd->texunit = MIN2(texunit, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->pname = MIN2(pname, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->param = param;
}


/* MultiTexEnvfvEXT: marshalled asynchronously */
struct marshal_cmd_MultiTexEnvfvEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 texunit;
   GLenum16 target;
   GLenum16 pname;
   /* Next safe_mul(_mesa_texenv_enum_to_count(pname), 1 * sizeof(GLfloat)) bytes are GLfloat param[_mesa_texenv_enum_to_count(pname)] */
};
uint32_t
_mesa_unmarshal_MultiTexEnvfvEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexEnvfvEXT *cmd)
{
   GLenum texunit = cmd->texunit;
   GLenum target = cmd->target;
   GLenum pname = cmd->pname;
   GLfloat *param;
   const char *variable_data = (const char *) (cmd + 1);
   param = (GLfloat *) variable_data;
   CALL_MultiTexEnvfvEXT(ctx->CurrentServerDispatch, (texunit, target, pname, param));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_MultiTexEnvfvEXT(GLenum texunit, GLenum target, GLenum pname, const GLfloat * param)
{
   GET_CURRENT_CONTEXT(ctx);
   int param_size = safe_mul(_mesa_texenv_enum_to_count(pname), 1 * sizeof(GLfloat));
   int cmd_size = sizeof(struct marshal_cmd_MultiTexEnvfvEXT) + param_size;
   struct marshal_cmd_MultiTexEnvfvEXT *cmd;
   if (unlikely(param_size < 0 || (param_size > 0 && !param) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "MultiTexEnvfvEXT");
      CALL_MultiTexEnvfvEXT(ctx->CurrentServerDispatch, (texunit, target, pname, param));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexEnvfvEXT, cmd_size);
   cmd->texunit = MIN2(texunit, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->pname = MIN2(pname, 0xffff); /* clamped to 0xffff (invalid enum) */
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, param, param_size);
}


/* GetMultiTexEnvivEXT: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetMultiTexEnvivEXT(GLenum texunit, GLenum target, GLenum pname, GLint * param)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetMultiTexEnvivEXT");
   CALL_GetMultiTexEnvivEXT(ctx->CurrentServerDispatch, (texunit, target, pname, param));
}


/* GetMultiTexEnvfvEXT: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetMultiTexEnvfvEXT(GLenum texunit, GLenum target, GLenum pname, GLfloat * param)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetMultiTexEnvfvEXT");
   CALL_GetMultiTexEnvfvEXT(ctx->CurrentServerDispatch, (texunit, target, pname, param));
}


/* MultiTexParameteriEXT: marshalled asynchronously */
struct marshal_cmd_MultiTexParameteriEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 texunit;
   GLenum16 target;
   GLenum16 pname;
   GLint param;
};
uint32_t
_mesa_unmarshal_MultiTexParameteriEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexParameteriEXT *cmd)
{
   GLenum texunit = cmd->texunit;
   GLenum target = cmd->target;
   GLenum pname = cmd->pname;
   GLint param = cmd->param;
   CALL_MultiTexParameteriEXT(ctx->CurrentServerDispatch, (texunit, target, pname, param));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MultiTexParameteriEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_MultiTexParameteriEXT(GLenum texunit, GLenum target, GLenum pname, GLint param)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexParameteriEXT);
   struct marshal_cmd_MultiTexParameteriEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexParameteriEXT, cmd_size);
   cmd->texunit = MIN2(texunit, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->pname = MIN2(pname, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->param = param;
}


/* MultiTexParameterivEXT: marshalled asynchronously */
struct marshal_cmd_MultiTexParameterivEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 texunit;
   GLenum16 target;
   GLenum16 pname;
   /* Next safe_mul(_mesa_tex_param_enum_to_count(pname), 1 * sizeof(GLint)) bytes are GLint param[_mesa_tex_param_enum_to_count(pname)] */
};
uint32_t
_mesa_unmarshal_MultiTexParameterivEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexParameterivEXT *cmd)
{
   GLenum texunit = cmd->texunit;
   GLenum target = cmd->target;
   GLenum pname = cmd->pname;
   GLint *param;
   const char *variable_data = (const char *) (cmd + 1);
   param = (GLint *) variable_data;
   CALL_MultiTexParameterivEXT(ctx->CurrentServerDispatch, (texunit, target, pname, param));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_MultiTexParameterivEXT(GLenum texunit, GLenum target, GLenum pname, const GLint* param)
{
   GET_CURRENT_CONTEXT(ctx);
   int param_size = safe_mul(_mesa_tex_param_enum_to_count(pname), 1 * sizeof(GLint));
   int cmd_size = sizeof(struct marshal_cmd_MultiTexParameterivEXT) + param_size;
   struct marshal_cmd_MultiTexParameterivEXT *cmd;
   if (unlikely(param_size < 0 || (param_size > 0 && !param) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "MultiTexParameterivEXT");
      CALL_MultiTexParameterivEXT(ctx->CurrentServerDispatch, (texunit, target, pname, param));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexParameterivEXT, cmd_size);
   cmd->texunit = MIN2(texunit, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->pname = MIN2(pname, 0xffff); /* clamped to 0xffff (invalid enum) */
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, param, param_size);
}


/* MultiTexParameterfEXT: marshalled asynchronously */
struct marshal_cmd_MultiTexParameterfEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 texunit;
   GLenum16 target;
   GLenum16 pname;
   GLfloat param;
};
uint32_t
_mesa_unmarshal_MultiTexParameterfEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexParameterfEXT *cmd)
{
   GLenum texunit = cmd->texunit;
   GLenum target = cmd->target;
   GLenum pname = cmd->pname;
   GLfloat param = cmd->param;
   CALL_MultiTexParameterfEXT(ctx->CurrentServerDispatch, (texunit, target, pname, param));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MultiTexParameterfEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_MultiTexParameterfEXT(GLenum texunit, GLenum target, GLenum pname, GLfloat param)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexParameterfEXT);
   struct marshal_cmd_MultiTexParameterfEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexParameterfEXT, cmd_size);
   cmd->texunit = MIN2(texunit, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->pname = MIN2(pname, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->param = param;
}


/* MultiTexParameterfvEXT: marshalled asynchronously */
struct marshal_cmd_MultiTexParameterfvEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 texunit;
   GLenum16 target;
   GLenum16 pname;
   /* Next safe_mul(_mesa_tex_param_enum_to_count(pname), 1 * sizeof(GLfloat)) bytes are GLfloat param[_mesa_tex_param_enum_to_count(pname)] */
};
uint32_t
_mesa_unmarshal_MultiTexParameterfvEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexParameterfvEXT *cmd)
{
   GLenum texunit = cmd->texunit;
   GLenum target = cmd->target;
   GLenum pname = cmd->pname;
   GLfloat *param;
   const char *variable_data = (const char *) (cmd + 1);
   param = (GLfloat *) variable_data;
   CALL_MultiTexParameterfvEXT(ctx->CurrentServerDispatch, (texunit, target, pname, param));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_MultiTexParameterfvEXT(GLenum texunit, GLenum target, GLenum pname, const GLfloat* param)
{
   GET_CURRENT_CONTEXT(ctx);
   int param_size = safe_mul(_mesa_tex_param_enum_to_count(pname), 1 * sizeof(GLfloat));
   int cmd_size = sizeof(struct marshal_cmd_MultiTexParameterfvEXT) + param_size;
   struct marshal_cmd_MultiTexParameterfvEXT *cmd;
   if (unlikely(param_size < 0 || (param_size > 0 && !param) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "MultiTexParameterfvEXT");
      CALL_MultiTexParameterfvEXT(ctx->CurrentServerDispatch, (texunit, target, pname, param));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexParameterfvEXT, cmd_size);
   cmd->texunit = MIN2(texunit, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->pname = MIN2(pname, 0xffff); /* clamped to 0xffff (invalid enum) */
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, param, param_size);
}


/* GetMultiTexParameterivEXT: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetMultiTexParameterivEXT(GLenum texunit, GLenum target, GLenum pname, GLint* params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetMultiTexParameterivEXT");
   CALL_GetMultiTexParameterivEXT(ctx->CurrentServerDispatch, (texunit, target, pname, params));
}


/* GetMultiTexParameterfvEXT: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetMultiTexParameterfvEXT(GLenum texunit, GLenum target, GLenum pname, GLfloat* params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetMultiTexParameterfvEXT");
   CALL_GetMultiTexParameterfvEXT(ctx->CurrentServerDispatch, (texunit, target, pname, params));
}


/* GetMultiTexImageEXT: marshalled asynchronously */
struct marshal_cmd_GetMultiTexImageEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 texunit;
   GLenum16 target;
   GLenum16 format;
   GLenum16 type;
   GLint level;
   GLvoid* pixels;
};
uint32_t
_mesa_unmarshal_GetMultiTexImageEXT(struct gl_context *ctx, const struct marshal_cmd_GetMultiTexImageEXT *cmd)
{
   GLenum texunit = cmd->texunit;
   GLenum target = cmd->target;
   GLint level = cmd->level;
   GLenum format = cmd->format;
   GLenum type = cmd->type;
   GLvoid* pixels = cmd->pixels;
   CALL_GetMultiTexImageEXT(ctx->CurrentServerDispatch, (texunit, target, level, format, type, pixels));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_GetMultiTexImageEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_GetMultiTexImageEXT(GLenum texunit, GLenum target, GLint level, GLenum format, GLenum type, GLvoid* pixels)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_GetMultiTexImageEXT);
   struct marshal_cmd_GetMultiTexImageEXT *cmd;
   if (_mesa_glthread_has_no_pack_buffer(ctx)) {
      _mesa_glthread_finish_before(ctx, "GetMultiTexImageEXT");
      CALL_GetMultiTexImageEXT(ctx->CurrentServerDispatch, (texunit, target, level, format, type, pixels));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_GetMultiTexImageEXT, cmd_size);
   cmd->texunit = MIN2(texunit, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->level = level;
   cmd->format = MIN2(format, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->type = MIN2(type, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->pixels = pixels;
}


/* GetMultiTexLevelParameterivEXT: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetMultiTexLevelParameterivEXT(GLenum texunit, GLenum target, GLint level, GLenum pname, GLint* params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetMultiTexLevelParameterivEXT");
   CALL_GetMultiTexLevelParameterivEXT(ctx->CurrentServerDispatch, (texunit, target, level, pname, params));
}


/* GetMultiTexLevelParameterfvEXT: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetMultiTexLevelParameterfvEXT(GLenum texunit, GLenum target, GLint level, GLenum pname, GLfloat* params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetMultiTexLevelParameterfvEXT");
   CALL_GetMultiTexLevelParameterfvEXT(ctx->CurrentServerDispatch, (texunit, target, level, pname, params));
}


/* MultiTexImage1DEXT: marshalled asynchronously */
struct marshal_cmd_MultiTexImage1DEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 texunit;
   GLenum16 target;
   GLenum16 format;
   GLenum16 type;
   GLint level;
   GLint internalformat;
   GLsizei width;
   GLint border;
   const GLvoid* pixels;
};
uint32_t
_mesa_unmarshal_MultiTexImage1DEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexImage1DEXT *cmd)
{
   GLenum texunit = cmd->texunit;
   GLenum target = cmd->target;
   GLint level = cmd->level;
   GLint internalformat = cmd->internalformat;
   GLsizei width = cmd->width;
   GLint border = cmd->border;
   GLenum format = cmd->format;
   GLenum type = cmd->type;
   const GLvoid* pixels = cmd->pixels;
   CALL_MultiTexImage1DEXT(ctx->CurrentServerDispatch, (texunit, target, level, internalformat, width, border, format, type, pixels));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MultiTexImage1DEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_MultiTexImage1DEXT(GLenum texunit, GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, const GLvoid* pixels)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexImage1DEXT);
   struct marshal_cmd_MultiTexImage1DEXT *cmd;
   if (_mesa_glthread_has_no_unpack_buffer(ctx)) {
      _mesa_glthread_finish_before(ctx, "MultiTexImage1DEXT");
      CALL_MultiTexImage1DEXT(ctx->CurrentServerDispatch, (texunit, target, level, internalformat, width, border, format, type, pixels));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexImage1DEXT, cmd_size);
   cmd->texunit = MIN2(texunit, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->level = level;
   cmd->internalformat = internalformat;
   cmd->width = width;
   cmd->border = border;
   cmd->format = MIN2(format, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->type = MIN2(type, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->pixels = pixels;
}


/* MultiTexImage2DEXT: marshalled asynchronously */
struct marshal_cmd_MultiTexImage2DEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 texunit;
   GLenum16 target;
   GLenum16 format;
   GLenum16 type;
   GLint level;
   GLint internalformat;
   GLsizei width;
   GLsizei height;
   GLint border;
   const GLvoid* pixels;
};
uint32_t
_mesa_unmarshal_MultiTexImage2DEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexImage2DEXT *cmd)
{
   GLenum texunit = cmd->texunit;
   GLenum target = cmd->target;
   GLint level = cmd->level;
   GLint internalformat = cmd->internalformat;
   GLsizei width = cmd->width;
   GLsizei height = cmd->height;
   GLint border = cmd->border;
   GLenum format = cmd->format;
   GLenum type = cmd->type;
   const GLvoid* pixels = cmd->pixels;
   CALL_MultiTexImage2DEXT(ctx->CurrentServerDispatch, (texunit, target, level, internalformat, width, height, border, format, type, pixels));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MultiTexImage2DEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_MultiTexImage2DEXT(GLenum texunit, GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid* pixels)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexImage2DEXT);
   struct marshal_cmd_MultiTexImage2DEXT *cmd;
   if (_mesa_glthread_has_no_unpack_buffer(ctx)) {
      _mesa_glthread_finish_before(ctx, "MultiTexImage2DEXT");
      CALL_MultiTexImage2DEXT(ctx->CurrentServerDispatch, (texunit, target, level, internalformat, width, height, border, format, type, pixels));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexImage2DEXT, cmd_size);
   cmd->texunit = MIN2(texunit, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->level = level;
   cmd->internalformat = internalformat;
   cmd->width = width;
   cmd->height = height;
   cmd->border = border;
   cmd->format = MIN2(format, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->type = MIN2(type, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->pixels = pixels;
}


/* MultiTexImage3DEXT: marshalled asynchronously */
struct marshal_cmd_MultiTexImage3DEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 texunit;
   GLenum16 target;
   GLenum16 format;
   GLenum16 type;
   GLint level;
   GLint internalformat;
   GLsizei width;
   GLsizei height;
   GLsizei depth;
   GLint border;
   const GLvoid* pixels;
};
uint32_t
_mesa_unmarshal_MultiTexImage3DEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexImage3DEXT *cmd)
{
   GLenum texunit = cmd->texunit;
   GLenum target = cmd->target;
   GLint level = cmd->level;
   GLint internalformat = cmd->internalformat;
   GLsizei width = cmd->width;
   GLsizei height = cmd->height;
   GLsizei depth = cmd->depth;
   GLint border = cmd->border;
   GLenum format = cmd->format;
   GLenum type = cmd->type;
   const GLvoid* pixels = cmd->pixels;
   CALL_MultiTexImage3DEXT(ctx->CurrentServerDispatch, (texunit, target, level, internalformat, width, height, depth, border, format, type, pixels));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MultiTexImage3DEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_MultiTexImage3DEXT(GLenum texunit, GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid* pixels)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexImage3DEXT);
   struct marshal_cmd_MultiTexImage3DEXT *cmd;
   if (_mesa_glthread_has_no_unpack_buffer(ctx)) {
      _mesa_glthread_finish_before(ctx, "MultiTexImage3DEXT");
      CALL_MultiTexImage3DEXT(ctx->CurrentServerDispatch, (texunit, target, level, internalformat, width, height, depth, border, format, type, pixels));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexImage3DEXT, cmd_size);
   cmd->texunit = MIN2(texunit, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->level = level;
   cmd->internalformat = internalformat;
   cmd->width = width;
   cmd->height = height;
   cmd->depth = depth;
   cmd->border = border;
   cmd->format = MIN2(format, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->type = MIN2(type, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->pixels = pixels;
}


/* MultiTexSubImage1DEXT: marshalled asynchronously */
struct marshal_cmd_MultiTexSubImage1DEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 texunit;
   GLenum16 target;
   GLenum16 format;
   GLenum16 type;
   GLint level;
   GLint xoffset;
   GLsizei width;
   const GLvoid* pixels;
};
uint32_t
_mesa_unmarshal_MultiTexSubImage1DEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexSubImage1DEXT *cmd)
{
   GLenum texunit = cmd->texunit;
   GLenum target = cmd->target;
   GLint level = cmd->level;
   GLint xoffset = cmd->xoffset;
   GLsizei width = cmd->width;
   GLenum format = cmd->format;
   GLenum type = cmd->type;
   const GLvoid* pixels = cmd->pixels;
   CALL_MultiTexSubImage1DEXT(ctx->CurrentServerDispatch, (texunit, target, level, xoffset, width, format, type, pixels));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MultiTexSubImage1DEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_MultiTexSubImage1DEXT(GLenum texunit, GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const GLvoid* pixels)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexSubImage1DEXT);
   struct marshal_cmd_MultiTexSubImage1DEXT *cmd;
   if (_mesa_glthread_has_no_unpack_buffer(ctx)) {
      _mesa_glthread_finish_before(ctx, "MultiTexSubImage1DEXT");
      CALL_MultiTexSubImage1DEXT(ctx->CurrentServerDispatch, (texunit, target, level, xoffset, width, format, type, pixels));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexSubImage1DEXT, cmd_size);
   cmd->texunit = MIN2(texunit, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->level = level;
   cmd->xoffset = xoffset;
   cmd->width = width;
   cmd->format = MIN2(format, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->type = MIN2(type, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->pixels = pixels;
}


/* MultiTexSubImage2DEXT: marshalled asynchronously */
struct marshal_cmd_MultiTexSubImage2DEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 texunit;
   GLenum16 target;
   GLenum16 format;
   GLenum16 type;
   GLint level;
   GLint xoffset;
   GLint yoffset;
   GLsizei width;
   GLsizei height;
   const GLvoid* pixels;
};
uint32_t
_mesa_unmarshal_MultiTexSubImage2DEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexSubImage2DEXT *cmd)
{
   GLenum texunit = cmd->texunit;
   GLenum target = cmd->target;
   GLint level = cmd->level;
   GLint xoffset = cmd->xoffset;
   GLint yoffset = cmd->yoffset;
   GLsizei width = cmd->width;
   GLsizei height = cmd->height;
   GLenum format = cmd->format;
   GLenum type = cmd->type;
   const GLvoid* pixels = cmd->pixels;
   CALL_MultiTexSubImage2DEXT(ctx->CurrentServerDispatch, (texunit, target, level, xoffset, yoffset, width, height, format, type, pixels));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MultiTexSubImage2DEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_MultiTexSubImage2DEXT(GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid* pixels)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexSubImage2DEXT);
   struct marshal_cmd_MultiTexSubImage2DEXT *cmd;
   if (_mesa_glthread_has_no_unpack_buffer(ctx)) {
      _mesa_glthread_finish_before(ctx, "MultiTexSubImage2DEXT");
      CALL_MultiTexSubImage2DEXT(ctx->CurrentServerDispatch, (texunit, target, level, xoffset, yoffset, width, height, format, type, pixels));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexSubImage2DEXT, cmd_size);
   cmd->texunit = MIN2(texunit, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->level = level;
   cmd->xoffset = xoffset;
   cmd->yoffset = yoffset;
   cmd->width = width;
   cmd->height = height;
   cmd->format = MIN2(format, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->type = MIN2(type, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->pixels = pixels;
}


/* MultiTexSubImage3DEXT: marshalled asynchronously */
struct marshal_cmd_MultiTexSubImage3DEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 texunit;
   GLenum16 target;
   GLenum16 format;
   GLenum16 type;
   GLint level;
   GLint xoffset;
   GLint yoffset;
   GLint zoffset;
   GLsizei width;
   GLsizei height;
   GLsizei depth;
   const GLvoid* pixels;
};
uint32_t
_mesa_unmarshal_MultiTexSubImage3DEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexSubImage3DEXT *cmd)
{
   GLenum texunit = cmd->texunit;
   GLenum target = cmd->target;
   GLint level = cmd->level;
   GLint xoffset = cmd->xoffset;
   GLint yoffset = cmd->yoffset;
   GLint zoffset = cmd->zoffset;
   GLsizei width = cmd->width;
   GLsizei height = cmd->height;
   GLsizei depth = cmd->depth;
   GLenum format = cmd->format;
   GLenum type = cmd->type;
   const GLvoid* pixels = cmd->pixels;
   CALL_MultiTexSubImage3DEXT(ctx->CurrentServerDispatch, (texunit, target, level, xoffset, yoffset, zoffset, width, height, depth, format, type, pixels));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MultiTexSubImage3DEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_MultiTexSubImage3DEXT(GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const GLvoid* pixels)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexSubImage3DEXT);
   struct marshal_cmd_MultiTexSubImage3DEXT *cmd;
   if (_mesa_glthread_has_no_unpack_buffer(ctx)) {
      _mesa_glthread_finish_before(ctx, "MultiTexSubImage3DEXT");
      CALL_MultiTexSubImage3DEXT(ctx->CurrentServerDispatch, (texunit, target, level, xoffset, yoffset, zoffset, width, height, depth, format, type, pixels));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexSubImage3DEXT, cmd_size);
   cmd->texunit = MIN2(texunit, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
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


/* CopyMultiTexImage1DEXT: marshalled asynchronously */
struct marshal_cmd_CopyMultiTexImage1DEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 texunit;
   GLenum16 target;
   GLenum16 internalformat;
   GLint level;
   GLint x;
   GLint y;
   GLsizei width;
   GLint border;
};
uint32_t
_mesa_unmarshal_CopyMultiTexImage1DEXT(struct gl_context *ctx, const struct marshal_cmd_CopyMultiTexImage1DEXT *cmd)
{
   GLenum texunit = cmd->texunit;
   GLenum target = cmd->target;
   GLint level = cmd->level;
   GLenum internalformat = cmd->internalformat;
   GLint x = cmd->x;
   GLint y = cmd->y;
   GLsizei width = cmd->width;
   GLint border = cmd->border;
   CALL_CopyMultiTexImage1DEXT(ctx->CurrentServerDispatch, (texunit, target, level, internalformat, x, y, width, border));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_CopyMultiTexImage1DEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_CopyMultiTexImage1DEXT(GLenum texunit, GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLint border)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_CopyMultiTexImage1DEXT);
   struct marshal_cmd_CopyMultiTexImage1DEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_CopyMultiTexImage1DEXT, cmd_size);
   cmd->texunit = MIN2(texunit, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->level = level;
   cmd->internalformat = MIN2(internalformat, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->x = x;
   cmd->y = y;
   cmd->width = width;
   cmd->border = border;
}


/* CopyMultiTexImage2DEXT: marshalled asynchronously */
struct marshal_cmd_CopyMultiTexImage2DEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 texunit;
   GLenum16 target;
   GLenum16 internalformat;
   GLint level;
   GLint x;
   GLint y;
   GLsizei width;
   GLsizei height;
   GLint border;
};
uint32_t
_mesa_unmarshal_CopyMultiTexImage2DEXT(struct gl_context *ctx, const struct marshal_cmd_CopyMultiTexImage2DEXT *cmd)
{
   GLenum texunit = cmd->texunit;
   GLenum target = cmd->target;
   GLint level = cmd->level;
   GLenum internalformat = cmd->internalformat;
   GLint x = cmd->x;
   GLint y = cmd->y;
   GLsizei width = cmd->width;
   GLsizei height = cmd->height;
   GLint border = cmd->border;
   CALL_CopyMultiTexImage2DEXT(ctx->CurrentServerDispatch, (texunit, target, level, internalformat, x, y, width, height, border));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_CopyMultiTexImage2DEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_CopyMultiTexImage2DEXT(GLenum texunit, GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_CopyMultiTexImage2DEXT);
   struct marshal_cmd_CopyMultiTexImage2DEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_CopyMultiTexImage2DEXT, cmd_size);
   cmd->texunit = MIN2(texunit, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->level = level;
   cmd->internalformat = MIN2(internalformat, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->x = x;
   cmd->y = y;
   cmd->width = width;
   cmd->height = height;
   cmd->border = border;
}


/* CopyMultiTexSubImage1DEXT: marshalled asynchronously */
struct marshal_cmd_CopyMultiTexSubImage1DEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 texunit;
   GLenum16 target;
   GLint level;
   GLint xoffset;
   GLint x;
   GLint y;
   GLsizei width;
};
uint32_t
_mesa_unmarshal_CopyMultiTexSubImage1DEXT(struct gl_context *ctx, const struct marshal_cmd_CopyMultiTexSubImage1DEXT *cmd)
{
   GLenum texunit = cmd->texunit;
   GLenum target = cmd->target;
   GLint level = cmd->level;
   GLint xoffset = cmd->xoffset;
   GLint x = cmd->x;
   GLint y = cmd->y;
   GLsizei width = cmd->width;
   CALL_CopyMultiTexSubImage1DEXT(ctx->CurrentServerDispatch, (texunit, target, level, xoffset, x, y, width));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_CopyMultiTexSubImage1DEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_CopyMultiTexSubImage1DEXT(GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_CopyMultiTexSubImage1DEXT);
   struct marshal_cmd_CopyMultiTexSubImage1DEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_CopyMultiTexSubImage1DEXT, cmd_size);
   cmd->texunit = MIN2(texunit, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->level = level;
   cmd->xoffset = xoffset;
   cmd->x = x;
   cmd->y = y;
   cmd->width = width;
}


/* CopyMultiTexSubImage2DEXT: marshalled asynchronously */
struct marshal_cmd_CopyMultiTexSubImage2DEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 texunit;
   GLenum16 target;
   GLint level;
   GLint xoffset;
   GLint yoffset;
   GLint x;
   GLint y;
   GLsizei width;
   GLsizei height;
};
uint32_t
_mesa_unmarshal_CopyMultiTexSubImage2DEXT(struct gl_context *ctx, const struct marshal_cmd_CopyMultiTexSubImage2DEXT *cmd)
{
   GLenum texunit = cmd->texunit;
   GLenum target = cmd->target;
   GLint level = cmd->level;
   GLint xoffset = cmd->xoffset;
   GLint yoffset = cmd->yoffset;
   GLint x = cmd->x;
   GLint y = cmd->y;
   GLsizei width = cmd->width;
   GLsizei height = cmd->height;
   CALL_CopyMultiTexSubImage2DEXT(ctx->CurrentServerDispatch, (texunit, target, level, xoffset, yoffset, x, y, width, height));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_CopyMultiTexSubImage2DEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_CopyMultiTexSubImage2DEXT(GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_CopyMultiTexSubImage2DEXT);
   struct marshal_cmd_CopyMultiTexSubImage2DEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_CopyMultiTexSubImage2DEXT, cmd_size);
   cmd->texunit = MIN2(texunit, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->level = level;
   cmd->xoffset = xoffset;
   cmd->yoffset = yoffset;
   cmd->x = x;
   cmd->y = y;
   cmd->width = width;
   cmd->height = height;
}


/* CopyMultiTexSubImage3DEXT: marshalled asynchronously */
struct marshal_cmd_CopyMultiTexSubImage3DEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 texunit;
   GLenum16 target;
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
_mesa_unmarshal_CopyMultiTexSubImage3DEXT(struct gl_context *ctx, const struct marshal_cmd_CopyMultiTexSubImage3DEXT *cmd)
{
   GLenum texunit = cmd->texunit;
   GLenum target = cmd->target;
   GLint level = cmd->level;
   GLint xoffset = cmd->xoffset;
   GLint yoffset = cmd->yoffset;
   GLint zoffset = cmd->zoffset;
   GLint x = cmd->x;
   GLint y = cmd->y;
   GLsizei width = cmd->width;
   GLsizei height = cmd->height;
   CALL_CopyMultiTexSubImage3DEXT(ctx->CurrentServerDispatch, (texunit, target, level, xoffset, yoffset, zoffset, x, y, width, height));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_CopyMultiTexSubImage3DEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_CopyMultiTexSubImage3DEXT(GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_CopyMultiTexSubImage3DEXT);
   struct marshal_cmd_CopyMultiTexSubImage3DEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_CopyMultiTexSubImage3DEXT, cmd_size);
   cmd->texunit = MIN2(texunit, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->level = level;
   cmd->xoffset = xoffset;
   cmd->yoffset = yoffset;
   cmd->zoffset = zoffset;
   cmd->x = x;
   cmd->y = y;
   cmd->width = width;
   cmd->height = height;
}


/* MultiTexGendEXT: marshalled asynchronously */
struct marshal_cmd_MultiTexGendEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 texunit;
   GLenum16 coord;
   GLenum16 pname;
   GLdouble param;
};
uint32_t
_mesa_unmarshal_MultiTexGendEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexGendEXT *cmd)
{
   GLenum texunit = cmd->texunit;
   GLenum coord = cmd->coord;
   GLenum pname = cmd->pname;
   GLdouble param = cmd->param;
   CALL_MultiTexGendEXT(ctx->CurrentServerDispatch, (texunit, coord, pname, param));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MultiTexGendEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_MultiTexGendEXT(GLenum texunit, GLenum coord, GLenum pname, GLdouble param)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexGendEXT);
   struct marshal_cmd_MultiTexGendEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexGendEXT, cmd_size);
   cmd->texunit = MIN2(texunit, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->coord = MIN2(coord, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->pname = MIN2(pname, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->param = param;
}


/* MultiTexGendvEXT: marshalled asynchronously */
struct marshal_cmd_MultiTexGendvEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 texunit;
   GLenum16 coord;
   GLenum16 pname;
   /* Next safe_mul(_mesa_texgen_enum_to_count(pname), 1 * sizeof(GLdouble)) bytes are GLdouble param[_mesa_texgen_enum_to_count(pname)] */
};
uint32_t
_mesa_unmarshal_MultiTexGendvEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexGendvEXT *cmd)
{
   GLenum texunit = cmd->texunit;
   GLenum coord = cmd->coord;
   GLenum pname = cmd->pname;
   GLdouble *param;
   const char *variable_data = (const char *) (cmd + 1);
   param = (GLdouble *) variable_data;
   CALL_MultiTexGendvEXT(ctx->CurrentServerDispatch, (texunit, coord, pname, param));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_MultiTexGendvEXT(GLenum texunit, GLenum coord, GLenum pname, const GLdouble* param)
{
   GET_CURRENT_CONTEXT(ctx);
   int param_size = safe_mul(_mesa_texgen_enum_to_count(pname), 1 * sizeof(GLdouble));
   int cmd_size = sizeof(struct marshal_cmd_MultiTexGendvEXT) + param_size;
   struct marshal_cmd_MultiTexGendvEXT *cmd;
   if (unlikely(param_size < 0 || (param_size > 0 && !param) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "MultiTexGendvEXT");
      CALL_MultiTexGendvEXT(ctx->CurrentServerDispatch, (texunit, coord, pname, param));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexGendvEXT, cmd_size);
   cmd->texunit = MIN2(texunit, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->coord = MIN2(coord, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->pname = MIN2(pname, 0xffff); /* clamped to 0xffff (invalid enum) */
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, param, param_size);
}


/* MultiTexGenfEXT: marshalled asynchronously */
struct marshal_cmd_MultiTexGenfEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 texunit;
   GLenum16 coord;
   GLenum16 pname;
   GLfloat param;
};
uint32_t
_mesa_unmarshal_MultiTexGenfEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexGenfEXT *cmd)
{
   GLenum texunit = cmd->texunit;
   GLenum coord = cmd->coord;
   GLenum pname = cmd->pname;
   GLfloat param = cmd->param;
   CALL_MultiTexGenfEXT(ctx->CurrentServerDispatch, (texunit, coord, pname, param));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MultiTexGenfEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_MultiTexGenfEXT(GLenum texunit, GLenum coord, GLenum pname, GLfloat param)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexGenfEXT);
   struct marshal_cmd_MultiTexGenfEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexGenfEXT, cmd_size);
   cmd->texunit = MIN2(texunit, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->coord = MIN2(coord, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->pname = MIN2(pname, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->param = param;
}


/* MultiTexGenfvEXT: marshalled asynchronously */
struct marshal_cmd_MultiTexGenfvEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 texunit;
   GLenum16 coord;
   GLenum16 pname;
   /* Next safe_mul(_mesa_texgen_enum_to_count(pname), 1 * sizeof(GLfloat)) bytes are GLfloat param[_mesa_texgen_enum_to_count(pname)] */
};
uint32_t
_mesa_unmarshal_MultiTexGenfvEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexGenfvEXT *cmd)
{
   GLenum texunit = cmd->texunit;
   GLenum coord = cmd->coord;
   GLenum pname = cmd->pname;
   GLfloat *param;
   const char *variable_data = (const char *) (cmd + 1);
   param = (GLfloat *) variable_data;
   CALL_MultiTexGenfvEXT(ctx->CurrentServerDispatch, (texunit, coord, pname, param));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_MultiTexGenfvEXT(GLenum texunit, GLenum coord, GLenum pname, const GLfloat * param)
{
   GET_CURRENT_CONTEXT(ctx);
   int param_size = safe_mul(_mesa_texgen_enum_to_count(pname), 1 * sizeof(GLfloat));
   int cmd_size = sizeof(struct marshal_cmd_MultiTexGenfvEXT) + param_size;
   struct marshal_cmd_MultiTexGenfvEXT *cmd;
   if (unlikely(param_size < 0 || (param_size > 0 && !param) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "MultiTexGenfvEXT");
      CALL_MultiTexGenfvEXT(ctx->CurrentServerDispatch, (texunit, coord, pname, param));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexGenfvEXT, cmd_size);
   cmd->texunit = MIN2(texunit, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->coord = MIN2(coord, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->pname = MIN2(pname, 0xffff); /* clamped to 0xffff (invalid enum) */
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, param, param_size);
}


/* MultiTexGeniEXT: marshalled asynchronously */
struct marshal_cmd_MultiTexGeniEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 texunit;
   GLenum16 coord;
   GLenum16 pname;
   GLint param;
};
uint32_t
_mesa_unmarshal_MultiTexGeniEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexGeniEXT *cmd)
{
   GLenum texunit = cmd->texunit;
   GLenum coord = cmd->coord;
   GLenum pname = cmd->pname;
   GLint param = cmd->param;
   CALL_MultiTexGeniEXT(ctx->CurrentServerDispatch, (texunit, coord, pname, param));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MultiTexGeniEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_MultiTexGeniEXT(GLenum texunit, GLenum coord, GLenum pname, GLint param)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexGeniEXT);
   struct marshal_cmd_MultiTexGeniEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexGeniEXT, cmd_size);
   cmd->texunit = MIN2(texunit, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->coord = MIN2(coord, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->pname = MIN2(pname, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->param = param;
}


/* MultiTexGenivEXT: marshalled asynchronously */
struct marshal_cmd_MultiTexGenivEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 texunit;
   GLenum16 coord;
   GLenum16 pname;
   /* Next safe_mul(_mesa_texgen_enum_to_count(pname), 1 * sizeof(GLint)) bytes are GLint param[_mesa_texgen_enum_to_count(pname)] */
};
uint32_t
_mesa_unmarshal_MultiTexGenivEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexGenivEXT *cmd)
{
   GLenum texunit = cmd->texunit;
   GLenum coord = cmd->coord;
   GLenum pname = cmd->pname;
   GLint *param;
   const char *variable_data = (const char *) (cmd + 1);
   param = (GLint *) variable_data;
   CALL_MultiTexGenivEXT(ctx->CurrentServerDispatch, (texunit, coord, pname, param));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_MultiTexGenivEXT(GLenum texunit, GLenum coord, GLenum pname, const GLint * param)
{
   GET_CURRENT_CONTEXT(ctx);
   int param_size = safe_mul(_mesa_texgen_enum_to_count(pname), 1 * sizeof(GLint));
   int cmd_size = sizeof(struct marshal_cmd_MultiTexGenivEXT) + param_size;
   struct marshal_cmd_MultiTexGenivEXT *cmd;
   if (unlikely(param_size < 0 || (param_size > 0 && !param) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "MultiTexGenivEXT");
      CALL_MultiTexGenivEXT(ctx->CurrentServerDispatch, (texunit, coord, pname, param));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexGenivEXT, cmd_size);
   cmd->texunit = MIN2(texunit, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->coord = MIN2(coord, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->pname = MIN2(pname, 0xffff); /* clamped to 0xffff (invalid enum) */
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, param, param_size);
}


/* GetMultiTexGendvEXT: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetMultiTexGendvEXT(GLenum texunit, GLenum coord, GLenum pname, GLdouble * param)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetMultiTexGendvEXT");
   CALL_GetMultiTexGendvEXT(ctx->CurrentServerDispatch, (texunit, coord, pname, param));
}


/* GetMultiTexGenfvEXT: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetMultiTexGenfvEXT(GLenum texunit, GLenum coord, GLenum pname, GLfloat * param)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetMultiTexGenfvEXT");
   CALL_GetMultiTexGenfvEXT(ctx->CurrentServerDispatch, (texunit, coord, pname, param));
}


/* GetMultiTexGenivEXT: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetMultiTexGenivEXT(GLenum texunit, GLenum coord, GLenum pname, GLint * param)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetMultiTexGenivEXT");
   CALL_GetMultiTexGenivEXT(ctx->CurrentServerDispatch, (texunit, coord, pname, param));
}


/* MultiTexCoordPointerEXT: marshalled asynchronously */
struct marshal_cmd_MultiTexCoordPointerEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 texunit;
   GLenum16 type;
   GLint size;
   GLsizei stride;
   const GLvoid * pointer;
};
uint32_t
_mesa_unmarshal_MultiTexCoordPointerEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoordPointerEXT *cmd)
{
   GLenum texunit = cmd->texunit;
   GLint size = cmd->size;
   GLenum type = cmd->type;
   GLsizei stride = cmd->stride;
   const GLvoid * pointer = cmd->pointer;
   CALL_MultiTexCoordPointerEXT(ctx->CurrentServerDispatch, (texunit, size, type, stride, pointer));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MultiTexCoordPointerEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_MultiTexCoordPointerEXT(GLenum texunit, GLint size, GLenum type, GLsizei stride, const GLvoid * pointer)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexCoordPointerEXT);
   struct marshal_cmd_MultiTexCoordPointerEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexCoordPointerEXT, cmd_size);
   cmd->texunit = MIN2(texunit, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->size = size;
   cmd->type = MIN2(type, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->stride = stride;
   cmd->pointer = pointer;
   if (COMPAT) _mesa_glthread_AttribPointer(ctx, VERT_ATTRIB_TEX(texunit - GL_TEXTURE0), size, type, stride,  pointer);
}


/* MatrixLoadTransposefEXT: marshalled asynchronously */
struct marshal_cmd_MatrixLoadTransposefEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 matrixMode;
   GLfloat m[16];
};
uint32_t
_mesa_unmarshal_MatrixLoadTransposefEXT(struct gl_context *ctx, const struct marshal_cmd_MatrixLoadTransposefEXT *cmd)
{
   GLenum matrixMode = cmd->matrixMode;
   const GLfloat *m = cmd->m;
   CALL_MatrixLoadTransposefEXT(ctx->CurrentServerDispatch, (matrixMode, m));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MatrixLoadTransposefEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_MatrixLoadTransposefEXT(GLenum matrixMode, const GLfloat * m)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MatrixLoadTransposefEXT);
   struct marshal_cmd_MatrixLoadTransposefEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MatrixLoadTransposefEXT, cmd_size);
   cmd->matrixMode = MIN2(matrixMode, 0xffff); /* clamped to 0xffff (invalid enum) */
   memcpy(cmd->m, m, 16 * sizeof(GLfloat));
}


/* MatrixLoadTransposedEXT: marshalled asynchronously */
struct marshal_cmd_MatrixLoadTransposedEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 matrixMode;
   GLdouble m[16];
};
uint32_t
_mesa_unmarshal_MatrixLoadTransposedEXT(struct gl_context *ctx, const struct marshal_cmd_MatrixLoadTransposedEXT *cmd)
{
   GLenum matrixMode = cmd->matrixMode;
   const GLdouble *m = cmd->m;
   CALL_MatrixLoadTransposedEXT(ctx->CurrentServerDispatch, (matrixMode, m));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MatrixLoadTransposedEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_MatrixLoadTransposedEXT(GLenum matrixMode, const GLdouble * m)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MatrixLoadTransposedEXT);
   struct marshal_cmd_MatrixLoadTransposedEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MatrixLoadTransposedEXT, cmd_size);
   cmd->matrixMode = MIN2(matrixMode, 0xffff); /* clamped to 0xffff (invalid enum) */
   memcpy(cmd->m, m, 16 * sizeof(GLdouble));
}


/* MatrixMultTransposefEXT: marshalled asynchronously */
struct marshal_cmd_MatrixMultTransposefEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 matrixMode;
   GLfloat m[16];
};
uint32_t
_mesa_unmarshal_MatrixMultTransposefEXT(struct gl_context *ctx, const struct marshal_cmd_MatrixMultTransposefEXT *cmd)
{
   GLenum matrixMode = cmd->matrixMode;
   const GLfloat *m = cmd->m;
   CALL_MatrixMultTransposefEXT(ctx->CurrentServerDispatch, (matrixMode, m));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MatrixMultTransposefEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_MatrixMultTransposefEXT(GLenum matrixMode, const GLfloat * m)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MatrixMultTransposefEXT);
   struct marshal_cmd_MatrixMultTransposefEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MatrixMultTransposefEXT, cmd_size);
   cmd->matrixMode = MIN2(matrixMode, 0xffff); /* clamped to 0xffff (invalid enum) */
   memcpy(cmd->m, m, 16 * sizeof(GLfloat));
}


/* MatrixMultTransposedEXT: marshalled asynchronously */
struct marshal_cmd_MatrixMultTransposedEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 matrixMode;
   GLdouble m[16];
};
uint32_t
_mesa_unmarshal_MatrixMultTransposedEXT(struct gl_context *ctx, const struct marshal_cmd_MatrixMultTransposedEXT *cmd)
{
   GLenum matrixMode = cmd->matrixMode;
   const GLdouble *m = cmd->m;
   CALL_MatrixMultTransposedEXT(ctx->CurrentServerDispatch, (matrixMode, m));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MatrixMultTransposedEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_MatrixMultTransposedEXT(GLenum matrixMode, const GLdouble * m)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MatrixMultTransposedEXT);
   struct marshal_cmd_MatrixMultTransposedEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MatrixMultTransposedEXT, cmd_size);
   cmd->matrixMode = MIN2(matrixMode, 0xffff); /* clamped to 0xffff (invalid enum) */
   memcpy(cmd->m, m, 16 * sizeof(GLdouble));
}


/* CompressedTextureImage1DEXT: marshalled asynchronously */
struct marshal_cmd_CompressedTextureImage1DEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLenum16 internalFormat;
   GLuint texture;
   GLint level;
   GLsizei width;
   GLsizei border;
   GLsizei imageSize;
   const GLvoid * data;
};
uint32_t
_mesa_unmarshal_CompressedTextureImage1DEXT(struct gl_context *ctx, const struct marshal_cmd_CompressedTextureImage1DEXT *cmd)
{
   GLuint texture = cmd->texture;
   GLenum target = cmd->target;
   GLint level = cmd->level;
   GLenum internalFormat = cmd->internalFormat;
   GLsizei width = cmd->width;
   GLsizei border = cmd->border;
   GLsizei imageSize = cmd->imageSize;
   const GLvoid * data = cmd->data;
   CALL_CompressedTextureImage1DEXT(ctx->CurrentServerDispatch, (texture, target, level, internalFormat, width, border, imageSize, data));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_CompressedTextureImage1DEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_CompressedTextureImage1DEXT(GLuint texture, GLenum target, GLint level, GLenum internalFormat, GLsizei width, GLsizei border, GLsizei imageSize, const GLvoid * data)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_CompressedTextureImage1DEXT);
   struct marshal_cmd_CompressedTextureImage1DEXT *cmd;
   if (_mesa_glthread_has_no_unpack_buffer(ctx)) {
      _mesa_glthread_finish_before(ctx, "CompressedTextureImage1DEXT");
      CALL_CompressedTextureImage1DEXT(ctx->CurrentServerDispatch, (texture, target, level, internalFormat, width, border, imageSize, data));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_CompressedTextureImage1DEXT, cmd_size);
   cmd->texture = texture;
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->level = level;
   cmd->internalFormat = MIN2(internalFormat, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->width = width;
   cmd->border = border;
   cmd->imageSize = imageSize;
   cmd->data = data;
}


/* CompressedTextureImage2DEXT: marshalled asynchronously */
struct marshal_cmd_CompressedTextureImage2DEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLenum16 internalFormat;
   GLuint texture;
   GLint level;
   GLsizei width;
   GLsizei height;
   GLsizei border;
   GLsizei imageSize;
   const GLvoid * data;
};
uint32_t
_mesa_unmarshal_CompressedTextureImage2DEXT(struct gl_context *ctx, const struct marshal_cmd_CompressedTextureImage2DEXT *cmd)
{
   GLuint texture = cmd->texture;
   GLenum target = cmd->target;
   GLint level = cmd->level;
   GLenum internalFormat = cmd->internalFormat;
   GLsizei width = cmd->width;
   GLsizei height = cmd->height;
   GLsizei border = cmd->border;
   GLsizei imageSize = cmd->imageSize;
   const GLvoid * data = cmd->data;
   CALL_CompressedTextureImage2DEXT(ctx->CurrentServerDispatch, (texture, target, level, internalFormat, width, height, border, imageSize, data));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_CompressedTextureImage2DEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_CompressedTextureImage2DEXT(GLuint texture, GLenum target, GLint level, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei border, GLsizei imageSize, const GLvoid * data)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_CompressedTextureImage2DEXT);
   struct marshal_cmd_CompressedTextureImage2DEXT *cmd;
   if (_mesa_glthread_has_no_unpack_buffer(ctx)) {
      _mesa_glthread_finish_before(ctx, "CompressedTextureImage2DEXT");
      CALL_CompressedTextureImage2DEXT(ctx->CurrentServerDispatch, (texture, target, level, internalFormat, width, height, border, imageSize, data));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_CompressedTextureImage2DEXT, cmd_size);
   cmd->texture = texture;
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->level = level;
   cmd->internalFormat = MIN2(internalFormat, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->width = width;
   cmd->height = height;
   cmd->border = border;
   cmd->imageSize = imageSize;
   cmd->data = data;
}


/* CompressedTextureImage3DEXT: marshalled asynchronously */
struct marshal_cmd_CompressedTextureImage3DEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLenum16 internalFormat;
   GLuint texture;
   GLint level;
   GLsizei width;
   GLsizei height;
   GLsizei depth;
   GLsizei border;
   GLsizei imageSize;
   const GLvoid * data;
};
uint32_t
_mesa_unmarshal_CompressedTextureImage3DEXT(struct gl_context *ctx, const struct marshal_cmd_CompressedTextureImage3DEXT *cmd)
{
   GLuint texture = cmd->texture;
   GLenum target = cmd->target;
   GLint level = cmd->level;
   GLenum internalFormat = cmd->internalFormat;
   GLsizei width = cmd->width;
   GLsizei height = cmd->height;
   GLsizei depth = cmd->depth;
   GLsizei border = cmd->border;
   GLsizei imageSize = cmd->imageSize;
   const GLvoid * data = cmd->data;
   CALL_CompressedTextureImage3DEXT(ctx->CurrentServerDispatch, (texture, target, level, internalFormat, width, height, depth, border, imageSize, data));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_CompressedTextureImage3DEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_CompressedTextureImage3DEXT(GLuint texture, GLenum target, GLint level, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLsizei border, GLsizei imageSize, const GLvoid * data)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_CompressedTextureImage3DEXT);
   struct marshal_cmd_CompressedTextureImage3DEXT *cmd;
   if (_mesa_glthread_has_no_unpack_buffer(ctx)) {
      _mesa_glthread_finish_before(ctx, "CompressedTextureImage3DEXT");
      CALL_CompressedTextureImage3DEXT(ctx->CurrentServerDispatch, (texture, target, level, internalFormat, width, height, depth, border, imageSize, data));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_CompressedTextureImage3DEXT, cmd_size);
   cmd->texture = texture;
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->level = level;
   cmd->internalFormat = MIN2(internalFormat, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->width = width;
   cmd->height = height;
   cmd->depth = depth;
   cmd->border = border;
   cmd->imageSize = imageSize;
   cmd->data = data;
}


/* CompressedTextureSubImage1DEXT: marshalled asynchronously */
struct marshal_cmd_CompressedTextureSubImage1DEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLenum16 format;
   GLuint texture;
   GLint level;
   GLint xoffset;
   GLsizei width;
   GLsizei imageSize;
   const GLvoid * data;
};
uint32_t
_mesa_unmarshal_CompressedTextureSubImage1DEXT(struct gl_context *ctx, const struct marshal_cmd_CompressedTextureSubImage1DEXT *cmd)
{
   GLuint texture = cmd->texture;
   GLenum target = cmd->target;
   GLint level = cmd->level;
   GLint xoffset = cmd->xoffset;
   GLsizei width = cmd->width;
   GLenum format = cmd->format;
   GLsizei imageSize = cmd->imageSize;
   const GLvoid * data = cmd->data;
   CALL_CompressedTextureSubImage1DEXT(ctx->CurrentServerDispatch, (texture, target, level, xoffset, width, format, imageSize, data));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_CompressedTextureSubImage1DEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_CompressedTextureSubImage1DEXT(GLuint texture, GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const GLvoid * data)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_CompressedTextureSubImage1DEXT);
   struct marshal_cmd_CompressedTextureSubImage1DEXT *cmd;
   if (_mesa_glthread_has_no_unpack_buffer(ctx)) {
      _mesa_glthread_finish_before(ctx, "CompressedTextureSubImage1DEXT");
      CALL_CompressedTextureSubImage1DEXT(ctx->CurrentServerDispatch, (texture, target, level, xoffset, width, format, imageSize, data));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_CompressedTextureSubImage1DEXT, cmd_size);
   cmd->texture = texture;
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->level = level;
   cmd->xoffset = xoffset;
   cmd->width = width;
   cmd->format = MIN2(format, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->imageSize = imageSize;
   cmd->data = data;
}


/* CompressedTextureSubImage2DEXT: marshalled asynchronously */
struct marshal_cmd_CompressedTextureSubImage2DEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
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
_mesa_unmarshal_CompressedTextureSubImage2DEXT(struct gl_context *ctx, const struct marshal_cmd_CompressedTextureSubImage2DEXT *cmd)
{
   GLuint texture = cmd->texture;
   GLenum target = cmd->target;
   GLint level = cmd->level;
   GLint xoffset = cmd->xoffset;
   GLint yoffset = cmd->yoffset;
   GLsizei width = cmd->width;
   GLsizei height = cmd->height;
   GLenum format = cmd->format;
   GLsizei imageSize = cmd->imageSize;
   const GLvoid * data = cmd->data;
   CALL_CompressedTextureSubImage2DEXT(ctx->CurrentServerDispatch, (texture, target, level, xoffset, yoffset, width, height, format, imageSize, data));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_CompressedTextureSubImage2DEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_CompressedTextureSubImage2DEXT(GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const GLvoid * data)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_CompressedTextureSubImage2DEXT);
   struct marshal_cmd_CompressedTextureSubImage2DEXT *cmd;
   if (_mesa_glthread_has_no_unpack_buffer(ctx)) {
      _mesa_glthread_finish_before(ctx, "CompressedTextureSubImage2DEXT");
      CALL_CompressedTextureSubImage2DEXT(ctx->CurrentServerDispatch, (texture, target, level, xoffset, yoffset, width, height, format, imageSize, data));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_CompressedTextureSubImage2DEXT, cmd_size);
   cmd->texture = texture;
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->level = level;
   cmd->xoffset = xoffset;
   cmd->yoffset = yoffset;
   cmd->width = width;
   cmd->height = height;
   cmd->format = MIN2(format, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->imageSize = imageSize;
   cmd->data = data;
}


/* CompressedTextureSubImage3DEXT: marshalled asynchronously */
struct marshal_cmd_CompressedTextureSubImage3DEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
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
_mesa_unmarshal_CompressedTextureSubImage3DEXT(struct gl_context *ctx, const struct marshal_cmd_CompressedTextureSubImage3DEXT *cmd)
{
   GLuint texture = cmd->texture;
   GLenum target = cmd->target;
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
   CALL_CompressedTextureSubImage3DEXT(ctx->CurrentServerDispatch, (texture, target, level, xoffset, yoffset, zoffset, width, height, depth, format, imageSize, data));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_CompressedTextureSubImage3DEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_CompressedTextureSubImage3DEXT(GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const GLvoid * data)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_CompressedTextureSubImage3DEXT);
   struct marshal_cmd_CompressedTextureSubImage3DEXT *cmd;
   if (_mesa_glthread_has_no_unpack_buffer(ctx)) {
      _mesa_glthread_finish_before(ctx, "CompressedTextureSubImage3DEXT");
      CALL_CompressedTextureSubImage3DEXT(ctx->CurrentServerDispatch, (texture, target, level, xoffset, yoffset, zoffset, width, height, depth, format, imageSize, data));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_CompressedTextureSubImage3DEXT, cmd_size);
   cmd->texture = texture;
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
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


/* GetCompressedTextureImageEXT: marshalled asynchronously */
struct marshal_cmd_GetCompressedTextureImageEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLuint texture;
   GLint level;
   GLvoid * img;
};
uint32_t
_mesa_unmarshal_GetCompressedTextureImageEXT(struct gl_context *ctx, const struct marshal_cmd_GetCompressedTextureImageEXT *cmd)
{
   GLuint texture = cmd->texture;
   GLenum target = cmd->target;
   GLint level = cmd->level;
   GLvoid * img = cmd->img;
   CALL_GetCompressedTextureImageEXT(ctx->CurrentServerDispatch, (texture, target, level, img));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_GetCompressedTextureImageEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_GetCompressedTextureImageEXT(GLuint texture, GLenum target, GLint level, GLvoid * img)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_GetCompressedTextureImageEXT);
   struct marshal_cmd_GetCompressedTextureImageEXT *cmd;
   if (_mesa_glthread_has_no_pack_buffer(ctx)) {
      _mesa_glthread_finish_before(ctx, "GetCompressedTextureImageEXT");
      CALL_GetCompressedTextureImageEXT(ctx->CurrentServerDispatch, (texture, target, level, img));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_GetCompressedTextureImageEXT, cmd_size);
   cmd->texture = texture;
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->level = level;
   cmd->img = img;
}


/* CompressedMultiTexImage1DEXT: marshalled asynchronously */
struct marshal_cmd_CompressedMultiTexImage1DEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 texunit;
   GLenum16 target;
   GLenum16 internalFormat;
   GLint level;
   GLsizei width;
   GLsizei border;
   GLsizei imageSize;
   const GLvoid * data;
};
uint32_t
_mesa_unmarshal_CompressedMultiTexImage1DEXT(struct gl_context *ctx, const struct marshal_cmd_CompressedMultiTexImage1DEXT *cmd)
{
   GLenum texunit = cmd->texunit;
   GLenum target = cmd->target;
   GLint level = cmd->level;
   GLenum internalFormat = cmd->internalFormat;
   GLsizei width = cmd->width;
   GLsizei border = cmd->border;
   GLsizei imageSize = cmd->imageSize;
   const GLvoid * data = cmd->data;
   CALL_CompressedMultiTexImage1DEXT(ctx->CurrentServerDispatch, (texunit, target, level, internalFormat, width, border, imageSize, data));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_CompressedMultiTexImage1DEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_CompressedMultiTexImage1DEXT(GLenum texunit, GLenum target, GLint level, GLenum internalFormat, GLsizei width, GLsizei border, GLsizei imageSize, const GLvoid * data)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_CompressedMultiTexImage1DEXT);
   struct marshal_cmd_CompressedMultiTexImage1DEXT *cmd;
   if (_mesa_glthread_has_no_unpack_buffer(ctx)) {
      _mesa_glthread_finish_before(ctx, "CompressedMultiTexImage1DEXT");
      CALL_CompressedMultiTexImage1DEXT(ctx->CurrentServerDispatch, (texunit, target, level, internalFormat, width, border, imageSize, data));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_CompressedMultiTexImage1DEXT, cmd_size);
   cmd->texunit = MIN2(texunit, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->level = level;
   cmd->internalFormat = MIN2(internalFormat, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->width = width;
   cmd->border = border;
   cmd->imageSize = imageSize;
   cmd->data = data;
}


/* CompressedMultiTexImage2DEXT: marshalled asynchronously */
struct marshal_cmd_CompressedMultiTexImage2DEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 texunit;
   GLenum16 target;
   GLenum16 internalFormat;
   GLint level;
   GLsizei width;
   GLsizei height;
   GLsizei border;
   GLsizei imageSize;
   const GLvoid * data;
};
uint32_t
_mesa_unmarshal_CompressedMultiTexImage2DEXT(struct gl_context *ctx, const struct marshal_cmd_CompressedMultiTexImage2DEXT *cmd)
{
   GLenum texunit = cmd->texunit;
   GLenum target = cmd->target;
   GLint level = cmd->level;
   GLenum internalFormat = cmd->internalFormat;
   GLsizei width = cmd->width;
   GLsizei height = cmd->height;
   GLsizei border = cmd->border;
   GLsizei imageSize = cmd->imageSize;
   const GLvoid * data = cmd->data;
   CALL_CompressedMultiTexImage2DEXT(ctx->CurrentServerDispatch, (texunit, target, level, internalFormat, width, height, border, imageSize, data));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_CompressedMultiTexImage2DEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_CompressedMultiTexImage2DEXT(GLenum texunit, GLenum target, GLint level, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei border, GLsizei imageSize, const GLvoid * data)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_CompressedMultiTexImage2DEXT);
   struct marshal_cmd_CompressedMultiTexImage2DEXT *cmd;
   if (_mesa_glthread_has_no_unpack_buffer(ctx)) {
      _mesa_glthread_finish_before(ctx, "CompressedMultiTexImage2DEXT");
      CALL_CompressedMultiTexImage2DEXT(ctx->CurrentServerDispatch, (texunit, target, level, internalFormat, width, height, border, imageSize, data));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_CompressedMultiTexImage2DEXT, cmd_size);
   cmd->texunit = MIN2(texunit, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->level = level;
   cmd->internalFormat = MIN2(internalFormat, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->width = width;
   cmd->height = height;
   cmd->border = border;
   cmd->imageSize = imageSize;
   cmd->data = data;
}


/* CompressedMultiTexImage3DEXT: marshalled asynchronously */
struct marshal_cmd_CompressedMultiTexImage3DEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 texunit;
   GLenum16 target;
   GLenum16 internalFormat;
   GLint level;
   GLsizei width;
   GLsizei height;
   GLsizei depth;
   GLsizei border;
   GLsizei imageSize;
   const GLvoid * data;
};
uint32_t
_mesa_unmarshal_CompressedMultiTexImage3DEXT(struct gl_context *ctx, const struct marshal_cmd_CompressedMultiTexImage3DEXT *cmd)
{
   GLenum texunit = cmd->texunit;
   GLenum target = cmd->target;
   GLint level = cmd->level;
   GLenum internalFormat = cmd->internalFormat;
   GLsizei width = cmd->width;
   GLsizei height = cmd->height;
   GLsizei depth = cmd->depth;
   GLsizei border = cmd->border;
   GLsizei imageSize = cmd->imageSize;
   const GLvoid * data = cmd->data;
   CALL_CompressedMultiTexImage3DEXT(ctx->CurrentServerDispatch, (texunit, target, level, internalFormat, width, height, depth, border, imageSize, data));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_CompressedMultiTexImage3DEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_CompressedMultiTexImage3DEXT(GLenum texunit, GLenum target, GLint level, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLsizei border, GLsizei imageSize, const GLvoid * data)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_CompressedMultiTexImage3DEXT);
   struct marshal_cmd_CompressedMultiTexImage3DEXT *cmd;
   if (_mesa_glthread_has_no_unpack_buffer(ctx)) {
      _mesa_glthread_finish_before(ctx, "CompressedMultiTexImage3DEXT");
      CALL_CompressedMultiTexImage3DEXT(ctx->CurrentServerDispatch, (texunit, target, level, internalFormat, width, height, depth, border, imageSize, data));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_CompressedMultiTexImage3DEXT, cmd_size);
   cmd->texunit = MIN2(texunit, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->level = level;
   cmd->internalFormat = MIN2(internalFormat, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->width = width;
   cmd->height = height;
   cmd->depth = depth;
   cmd->border = border;
   cmd->imageSize = imageSize;
   cmd->data = data;
}


/* CompressedMultiTexSubImage1DEXT: marshalled asynchronously */
struct marshal_cmd_CompressedMultiTexSubImage1DEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 texunit;
   GLenum16 target;
   GLenum16 format;
   GLint level;
   GLint xoffset;
   GLsizei width;
   GLsizei imageSize;
   const GLvoid * data;
};
uint32_t
_mesa_unmarshal_CompressedMultiTexSubImage1DEXT(struct gl_context *ctx, const struct marshal_cmd_CompressedMultiTexSubImage1DEXT *cmd)
{
   GLenum texunit = cmd->texunit;
   GLenum target = cmd->target;
   GLint level = cmd->level;
   GLint xoffset = cmd->xoffset;
   GLsizei width = cmd->width;
   GLenum format = cmd->format;
   GLsizei imageSize = cmd->imageSize;
   const GLvoid * data = cmd->data;
   CALL_CompressedMultiTexSubImage1DEXT(ctx->CurrentServerDispatch, (texunit, target, level, xoffset, width, format, imageSize, data));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_CompressedMultiTexSubImage1DEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_CompressedMultiTexSubImage1DEXT(GLenum texunit, GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const GLvoid * data)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_CompressedMultiTexSubImage1DEXT);
   struct marshal_cmd_CompressedMultiTexSubImage1DEXT *cmd;
   if (_mesa_glthread_has_no_unpack_buffer(ctx)) {
      _mesa_glthread_finish_before(ctx, "CompressedMultiTexSubImage1DEXT");
      CALL_CompressedMultiTexSubImage1DEXT(ctx->CurrentServerDispatch, (texunit, target, level, xoffset, width, format, imageSize, data));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_CompressedMultiTexSubImage1DEXT, cmd_size);
   cmd->texunit = MIN2(texunit, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->level = level;
   cmd->xoffset = xoffset;
   cmd->width = width;
   cmd->format = MIN2(format, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->imageSize = imageSize;
   cmd->data = data;
}


/* CompressedMultiTexSubImage2DEXT: marshalled asynchronously */
struct marshal_cmd_CompressedMultiTexSubImage2DEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 texunit;
   GLenum16 target;
   GLenum16 format;
   GLint level;
   GLint xoffset;
   GLint yoffset;
   GLsizei width;
   GLsizei height;
   GLsizei imageSize;
   const GLvoid * data;
};
uint32_t
_mesa_unmarshal_CompressedMultiTexSubImage2DEXT(struct gl_context *ctx, const struct marshal_cmd_CompressedMultiTexSubImage2DEXT *cmd)
{
   GLenum texunit = cmd->texunit;
   GLenum target = cmd->target;
   GLint level = cmd->level;
   GLint xoffset = cmd->xoffset;
   GLint yoffset = cmd->yoffset;
   GLsizei width = cmd->width;
   GLsizei height = cmd->height;
   GLenum format = cmd->format;
   GLsizei imageSize = cmd->imageSize;
   const GLvoid * data = cmd->data;
   CALL_CompressedMultiTexSubImage2DEXT(ctx->CurrentServerDispatch, (texunit, target, level, xoffset, yoffset, width, height, format, imageSize, data));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_CompressedMultiTexSubImage2DEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_CompressedMultiTexSubImage2DEXT(GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const GLvoid * data)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_CompressedMultiTexSubImage2DEXT);
   struct marshal_cmd_CompressedMultiTexSubImage2DEXT *cmd;
   if (_mesa_glthread_has_no_unpack_buffer(ctx)) {
      _mesa_glthread_finish_before(ctx, "CompressedMultiTexSubImage2DEXT");
      CALL_CompressedMultiTexSubImage2DEXT(ctx->CurrentServerDispatch, (texunit, target, level, xoffset, yoffset, width, height, format, imageSize, data));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_CompressedMultiTexSubImage2DEXT, cmd_size);
   cmd->texunit = MIN2(texunit, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->level = level;
   cmd->xoffset = xoffset;
   cmd->yoffset = yoffset;
   cmd->width = width;
   cmd->height = height;
   cmd->format = MIN2(format, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->imageSize = imageSize;
   cmd->data = data;
}


/* CompressedMultiTexSubImage3DEXT: marshalled asynchronously */
struct marshal_cmd_CompressedMultiTexSubImage3DEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 texunit;
   GLenum16 target;
   GLenum16 format;
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
_mesa_unmarshal_CompressedMultiTexSubImage3DEXT(struct gl_context *ctx, const struct marshal_cmd_CompressedMultiTexSubImage3DEXT *cmd)
{
   GLenum texunit = cmd->texunit;
   GLenum target = cmd->target;
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
   CALL_CompressedMultiTexSubImage3DEXT(ctx->CurrentServerDispatch, (texunit, target, level, xoffset, yoffset, zoffset, width, height, depth, format, imageSize, data));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_CompressedMultiTexSubImage3DEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_CompressedMultiTexSubImage3DEXT(GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const GLvoid * data)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_CompressedMultiTexSubImage3DEXT);
   struct marshal_cmd_CompressedMultiTexSubImage3DEXT *cmd;
   if (_mesa_glthread_has_no_unpack_buffer(ctx)) {
      _mesa_glthread_finish_before(ctx, "CompressedMultiTexSubImage3DEXT");
      CALL_CompressedMultiTexSubImage3DEXT(ctx->CurrentServerDispatch, (texunit, target, level, xoffset, yoffset, zoffset, width, height, depth, format, imageSize, data));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_CompressedMultiTexSubImage3DEXT, cmd_size);
   cmd->texunit = MIN2(texunit, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
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


/* GetCompressedMultiTexImageEXT: marshalled asynchronously */
struct marshal_cmd_GetCompressedMultiTexImageEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 texunit;
   GLenum16 target;
   GLint level;
   GLvoid * img;
};
uint32_t
_mesa_unmarshal_GetCompressedMultiTexImageEXT(struct gl_context *ctx, const struct marshal_cmd_GetCompressedMultiTexImageEXT *cmd)
{
   GLenum texunit = cmd->texunit;
   GLenum target = cmd->target;
   GLint level = cmd->level;
   GLvoid * img = cmd->img;
   CALL_GetCompressedMultiTexImageEXT(ctx->CurrentServerDispatch, (texunit, target, level, img));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_GetCompressedMultiTexImageEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_GetCompressedMultiTexImageEXT(GLenum texunit, GLenum target, GLint level, GLvoid * img)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_GetCompressedMultiTexImageEXT);
   struct marshal_cmd_GetCompressedMultiTexImageEXT *cmd;
   if (_mesa_glthread_has_no_pack_buffer(ctx)) {
      _mesa_glthread_finish_before(ctx, "GetCompressedMultiTexImageEXT");
      CALL_GetCompressedMultiTexImageEXT(ctx->CurrentServerDispatch, (texunit, target, level, img));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_GetCompressedMultiTexImageEXT, cmd_size);
   cmd->texunit = MIN2(texunit, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->level = level;
   cmd->img = img;
}


/* MapNamedBufferEXT: marshalled synchronously */
static GLvoid * GLAPIENTRY
_mesa_marshal_MapNamedBufferEXT(GLuint buffer, GLenum access)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "MapNamedBufferEXT");
   return CALL_MapNamedBufferEXT(ctx->CurrentServerDispatch, (buffer, access));
}


/* GetNamedBufferSubDataEXT: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetNamedBufferSubDataEXT(GLuint buffer, GLintptr offset, GLsizeiptr size, GLvoid * data)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetNamedBufferSubDataEXT");
   CALL_GetNamedBufferSubDataEXT(ctx->CurrentServerDispatch, (buffer, offset, size, data));
}


/* GetNamedBufferPointervEXT: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetNamedBufferPointervEXT(GLuint buffer, GLenum pname, GLvoid ** params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetNamedBufferPointervEXT");
   CALL_GetNamedBufferPointervEXT(ctx->CurrentServerDispatch, (buffer, pname, params));
}


/* GetNamedBufferParameterivEXT: marshalled synchronously */
static void GLAPIENTRY
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
uint32_t
_mesa_unmarshal_FlushMappedNamedBufferRangeEXT(struct gl_context *ctx, const struct marshal_cmd_FlushMappedNamedBufferRangeEXT *cmd)
{
   GLuint buffer = cmd->buffer;
   GLintptr offset = cmd->offset;
   GLsizeiptr length = cmd->length;
   CALL_FlushMappedNamedBufferRangeEXT(ctx->CurrentServerDispatch, (buffer, offset, length));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_FlushMappedNamedBufferRangeEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
static GLvoid * GLAPIENTRY
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
uint32_t
_mesa_unmarshal_FramebufferDrawBufferEXT(struct gl_context *ctx, const struct marshal_cmd_FramebufferDrawBufferEXT *cmd)
{
   GLuint framebuffer = cmd->framebuffer;
   GLenum mode = cmd->mode;
   CALL_FramebufferDrawBufferEXT(ctx->CurrentServerDispatch, (framebuffer, mode));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_FramebufferDrawBufferEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_FramebufferDrawBufferEXT(GLuint framebuffer, GLenum mode)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_FramebufferDrawBufferEXT);
   struct marshal_cmd_FramebufferDrawBufferEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_FramebufferDrawBufferEXT, cmd_size);
   cmd->framebuffer = framebuffer;
   cmd->mode = MIN2(mode, 0xffff); /* clamped to 0xffff (invalid enum) */
}


/* FramebufferDrawBuffersEXT: marshalled asynchronously */
struct marshal_cmd_FramebufferDrawBuffersEXT
{
   struct marshal_cmd_base cmd_base;
   GLuint framebuffer;
   GLsizei n;
   /* Next safe_mul(n, 1 * sizeof(GLenum)) bytes are GLenum bufs[n] */
};
uint32_t
_mesa_unmarshal_FramebufferDrawBuffersEXT(struct gl_context *ctx, const struct marshal_cmd_FramebufferDrawBuffersEXT *cmd)
{
   GLuint framebuffer = cmd->framebuffer;
   GLsizei n = cmd->n;
   GLenum *bufs;
   const char *variable_data = (const char *) (cmd + 1);
   bufs = (GLenum *) variable_data;
   CALL_FramebufferDrawBuffersEXT(ctx->CurrentServerDispatch, (framebuffer, n, bufs));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
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
uint32_t
_mesa_unmarshal_FramebufferReadBufferEXT(struct gl_context *ctx, const struct marshal_cmd_FramebufferReadBufferEXT *cmd)
{
   GLuint framebuffer = cmd->framebuffer;
   GLenum mode = cmd->mode;
   CALL_FramebufferReadBufferEXT(ctx->CurrentServerDispatch, (framebuffer, mode));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_FramebufferReadBufferEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_FramebufferReadBufferEXT(GLuint framebuffer, GLenum mode)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_FramebufferReadBufferEXT);
   struct marshal_cmd_FramebufferReadBufferEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_FramebufferReadBufferEXT, cmd_size);
   cmd->framebuffer = framebuffer;
   cmd->mode = MIN2(mode, 0xffff); /* clamped to 0xffff (invalid enum) */
}


/* GetFramebufferParameterivEXT: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetFramebufferParameterivEXT(GLuint framebuffer, GLenum pname, GLint * param)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetFramebufferParameterivEXT");
   CALL_GetFramebufferParameterivEXT(ctx->CurrentServerDispatch, (framebuffer, pname, param));
}


/* CheckNamedFramebufferStatusEXT: marshalled synchronously */
static GLenum GLAPIENTRY
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
uint32_t
_mesa_unmarshal_NamedFramebufferTexture1DEXT(struct gl_context *ctx, const struct marshal_cmd_NamedFramebufferTexture1DEXT *cmd)
{
   GLuint framebuffer = cmd->framebuffer;
   GLenum attachment = cmd->attachment;
   GLenum textarget = cmd->textarget;
   GLuint texture = cmd->texture;
   GLint level = cmd->level;
   CALL_NamedFramebufferTexture1DEXT(ctx->CurrentServerDispatch, (framebuffer, attachment, textarget, texture, level));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_NamedFramebufferTexture1DEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_NamedFramebufferTexture1DEXT(GLuint framebuffer, GLenum attachment, GLenum textarget, GLuint texture, GLint level)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_NamedFramebufferTexture1DEXT);
   struct marshal_cmd_NamedFramebufferTexture1DEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_NamedFramebufferTexture1DEXT, cmd_size);
   cmd->framebuffer = framebuffer;
   cmd->attachment = MIN2(attachment, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->textarget = MIN2(textarget, 0xffff); /* clamped to 0xffff (invalid enum) */
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
uint32_t
_mesa_unmarshal_NamedFramebufferTexture2DEXT(struct gl_context *ctx, const struct marshal_cmd_NamedFramebufferTexture2DEXT *cmd)
{
   GLuint framebuffer = cmd->framebuffer;
   GLenum attachment = cmd->attachment;
   GLenum textarget = cmd->textarget;
   GLuint texture = cmd->texture;
   GLint level = cmd->level;
   CALL_NamedFramebufferTexture2DEXT(ctx->CurrentServerDispatch, (framebuffer, attachment, textarget, texture, level));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_NamedFramebufferTexture2DEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_NamedFramebufferTexture2DEXT(GLuint framebuffer, GLenum attachment, GLenum textarget, GLuint texture, GLint level)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_NamedFramebufferTexture2DEXT);
   struct marshal_cmd_NamedFramebufferTexture2DEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_NamedFramebufferTexture2DEXT, cmd_size);
   cmd->framebuffer = framebuffer;
   cmd->attachment = MIN2(attachment, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->textarget = MIN2(textarget, 0xffff); /* clamped to 0xffff (invalid enum) */
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
uint32_t
_mesa_unmarshal_NamedFramebufferTexture3DEXT(struct gl_context *ctx, const struct marshal_cmd_NamedFramebufferTexture3DEXT *cmd)
{
   GLuint framebuffer = cmd->framebuffer;
   GLenum attachment = cmd->attachment;
   GLenum textarget = cmd->textarget;
   GLuint texture = cmd->texture;
   GLint level = cmd->level;
   GLint zoffset = cmd->zoffset;
   CALL_NamedFramebufferTexture3DEXT(ctx->CurrentServerDispatch, (framebuffer, attachment, textarget, texture, level, zoffset));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_NamedFramebufferTexture3DEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_NamedFramebufferTexture3DEXT(GLuint framebuffer, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_NamedFramebufferTexture3DEXT);
   struct marshal_cmd_NamedFramebufferTexture3DEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_NamedFramebufferTexture3DEXT, cmd_size);
   cmd->framebuffer = framebuffer;
   cmd->attachment = MIN2(attachment, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->textarget = MIN2(textarget, 0xffff); /* clamped to 0xffff (invalid enum) */
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
uint32_t
_mesa_unmarshal_NamedFramebufferRenderbufferEXT(struct gl_context *ctx, const struct marshal_cmd_NamedFramebufferRenderbufferEXT *cmd)
{
   GLuint framebuffer = cmd->framebuffer;
   GLenum attachment = cmd->attachment;
   GLenum renderbuffertarget = cmd->renderbuffertarget;
   GLuint renderbuffer = cmd->renderbuffer;
   CALL_NamedFramebufferRenderbufferEXT(ctx->CurrentServerDispatch, (framebuffer, attachment, renderbuffertarget, renderbuffer));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_NamedFramebufferRenderbufferEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_NamedFramebufferRenderbufferEXT(GLuint framebuffer, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_NamedFramebufferRenderbufferEXT);
   struct marshal_cmd_NamedFramebufferRenderbufferEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_NamedFramebufferRenderbufferEXT, cmd_size);
   cmd->framebuffer = framebuffer;
   cmd->attachment = MIN2(attachment, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->renderbuffertarget = MIN2(renderbuffertarget, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->renderbuffer = renderbuffer;
}


/* GetNamedFramebufferAttachmentParameterivEXT: marshalled synchronously */
static void GLAPIENTRY
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
uint32_t
_mesa_unmarshal_NamedRenderbufferStorageEXT(struct gl_context *ctx, const struct marshal_cmd_NamedRenderbufferStorageEXT *cmd)
{
   GLuint renderbuffer = cmd->renderbuffer;
   GLenum internalformat = cmd->internalformat;
   GLsizei width = cmd->width;
   GLsizei height = cmd->height;
   CALL_NamedRenderbufferStorageEXT(ctx->CurrentServerDispatch, (renderbuffer, internalformat, width, height));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_NamedRenderbufferStorageEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_NamedRenderbufferStorageEXT(GLuint renderbuffer, GLenum internalformat, GLsizei width, GLsizei height)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_NamedRenderbufferStorageEXT);
   struct marshal_cmd_NamedRenderbufferStorageEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_NamedRenderbufferStorageEXT, cmd_size);
   cmd->renderbuffer = renderbuffer;
   cmd->internalformat = MIN2(internalformat, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->width = width;
   cmd->height = height;
}


/* GetNamedRenderbufferParameterivEXT: marshalled synchronously */
static void GLAPIENTRY
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
uint32_t
_mesa_unmarshal_GenerateTextureMipmapEXT(struct gl_context *ctx, const struct marshal_cmd_GenerateTextureMipmapEXT *cmd)
{
   GLuint texture = cmd->texture;
   GLenum target = cmd->target;
   CALL_GenerateTextureMipmapEXT(ctx->CurrentServerDispatch, (texture, target));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_GenerateTextureMipmapEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_GenerateTextureMipmapEXT(GLuint texture, GLenum target)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_GenerateTextureMipmapEXT);
   struct marshal_cmd_GenerateTextureMipmapEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_GenerateTextureMipmapEXT, cmd_size);
   cmd->texture = texture;
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
}


/* GenerateMultiTexMipmapEXT: marshalled asynchronously */
struct marshal_cmd_GenerateMultiTexMipmapEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 texunit;
   GLenum16 target;
};
uint32_t
_mesa_unmarshal_GenerateMultiTexMipmapEXT(struct gl_context *ctx, const struct marshal_cmd_GenerateMultiTexMipmapEXT *cmd)
{
   GLenum texunit = cmd->texunit;
   GLenum target = cmd->target;
   CALL_GenerateMultiTexMipmapEXT(ctx->CurrentServerDispatch, (texunit, target));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_GenerateMultiTexMipmapEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_GenerateMultiTexMipmapEXT(GLenum texunit, GLenum target)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_GenerateMultiTexMipmapEXT);
   struct marshal_cmd_GenerateMultiTexMipmapEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_GenerateMultiTexMipmapEXT, cmd_size);
   cmd->texunit = MIN2(texunit, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
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
uint32_t
_mesa_unmarshal_NamedRenderbufferStorageMultisampleEXT(struct gl_context *ctx, const struct marshal_cmd_NamedRenderbufferStorageMultisampleEXT *cmd)
{
   GLuint renderbuffer = cmd->renderbuffer;
   GLsizei samples = cmd->samples;
   GLenum internalformat = cmd->internalformat;
   GLsizei width = cmd->width;
   GLsizei height = cmd->height;
   CALL_NamedRenderbufferStorageMultisampleEXT(ctx->CurrentServerDispatch, (renderbuffer, samples, internalformat, width, height));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_NamedRenderbufferStorageMultisampleEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_NamedRenderbufferStorageMultisampleEXT(GLuint renderbuffer, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_NamedRenderbufferStorageMultisampleEXT);
   struct marshal_cmd_NamedRenderbufferStorageMultisampleEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_NamedRenderbufferStorageMultisampleEXT, cmd_size);
   cmd->renderbuffer = renderbuffer;
   cmd->samples = samples;
   cmd->internalformat = MIN2(internalformat, 0xffff); /* clamped to 0xffff (invalid enum) */
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
uint32_t
_mesa_unmarshal_NamedCopyBufferSubDataEXT(struct gl_context *ctx, const struct marshal_cmd_NamedCopyBufferSubDataEXT *cmd)
{
   GLuint readBuffer = cmd->readBuffer;
   GLuint writeBuffer = cmd->writeBuffer;
   GLintptr readOffset = cmd->readOffset;
   GLintptr writeOffset = cmd->writeOffset;
   GLsizeiptr size = cmd->size;
   CALL_NamedCopyBufferSubDataEXT(ctx->CurrentServerDispatch, (readBuffer, writeBuffer, readOffset, writeOffset, size));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_NamedCopyBufferSubDataEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
uint32_t
_mesa_unmarshal_VertexArrayVertexOffsetEXT(struct gl_context *ctx, const struct marshal_cmd_VertexArrayVertexOffsetEXT *cmd)
{
   GLuint vaobj = cmd->vaobj;
   GLuint buffer = cmd->buffer;
   GLint size = cmd->size;
   GLenum type = cmd->type;
   GLsizei stride = cmd->stride;
   GLintptr offset = cmd->offset;
   CALL_VertexArrayVertexOffsetEXT(ctx->CurrentServerDispatch, (vaobj, buffer, size, type, stride, offset));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexArrayVertexOffsetEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_VertexArrayVertexOffsetEXT(GLuint vaobj, GLuint buffer, GLint size, GLenum type, GLsizei stride, GLintptr offset)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexArrayVertexOffsetEXT);
   struct marshal_cmd_VertexArrayVertexOffsetEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexArrayVertexOffsetEXT, cmd_size);
   cmd->vaobj = vaobj;
   cmd->buffer = buffer;
   cmd->size = size;
   cmd->type = MIN2(type, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->stride = stride;
   cmd->offset = offset;
   if (COMPAT) _mesa_glthread_DSAAttribPointer(ctx, vaobj, buffer, VERT_ATTRIB_POS, size, type, stride, offset);
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
uint32_t
_mesa_unmarshal_VertexArrayColorOffsetEXT(struct gl_context *ctx, const struct marshal_cmd_VertexArrayColorOffsetEXT *cmd)
{
   GLuint vaobj = cmd->vaobj;
   GLuint buffer = cmd->buffer;
   GLint size = cmd->size;
   GLenum type = cmd->type;
   GLsizei stride = cmd->stride;
   GLintptr offset = cmd->offset;
   CALL_VertexArrayColorOffsetEXT(ctx->CurrentServerDispatch, (vaobj, buffer, size, type, stride, offset));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexArrayColorOffsetEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_VertexArrayColorOffsetEXT(GLuint vaobj, GLuint buffer, GLint size, GLenum type, GLsizei stride, GLintptr offset)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexArrayColorOffsetEXT);
   struct marshal_cmd_VertexArrayColorOffsetEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexArrayColorOffsetEXT, cmd_size);
   cmd->vaobj = vaobj;
   cmd->buffer = buffer;
   cmd->size = size;
   cmd->type = MIN2(type, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->stride = stride;
   cmd->offset = offset;
   if (COMPAT) _mesa_glthread_DSAAttribPointer(ctx, vaobj, buffer, VERT_ATTRIB_COLOR0, size, type, stride, offset);
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
uint32_t
_mesa_unmarshal_VertexArrayEdgeFlagOffsetEXT(struct gl_context *ctx, const struct marshal_cmd_VertexArrayEdgeFlagOffsetEXT *cmd)
{
   GLuint vaobj = cmd->vaobj;
   GLuint buffer = cmd->buffer;
   GLsizei stride = cmd->stride;
   GLintptr offset = cmd->offset;
   CALL_VertexArrayEdgeFlagOffsetEXT(ctx->CurrentServerDispatch, (vaobj, buffer, stride, offset));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexArrayEdgeFlagOffsetEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
   if (COMPAT) _mesa_glthread_DSAAttribPointer(ctx, vaobj, buffer, VERT_ATTRIB_EDGEFLAG, 1, GL_UNSIGNED_BYTE, stride, offset);
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
uint32_t
_mesa_unmarshal_VertexArrayIndexOffsetEXT(struct gl_context *ctx, const struct marshal_cmd_VertexArrayIndexOffsetEXT *cmd)
{
   GLuint vaobj = cmd->vaobj;
   GLuint buffer = cmd->buffer;
   GLenum type = cmd->type;
   GLsizei stride = cmd->stride;
   GLintptr offset = cmd->offset;
   CALL_VertexArrayIndexOffsetEXT(ctx->CurrentServerDispatch, (vaobj, buffer, type, stride, offset));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexArrayIndexOffsetEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_VertexArrayIndexOffsetEXT(GLuint vaobj, GLuint buffer, GLenum type, GLsizei stride, GLintptr offset)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexArrayIndexOffsetEXT);
   struct marshal_cmd_VertexArrayIndexOffsetEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexArrayIndexOffsetEXT, cmd_size);
   cmd->vaobj = vaobj;
   cmd->buffer = buffer;
   cmd->type = MIN2(type, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->stride = stride;
   cmd->offset = offset;
   if (COMPAT) _mesa_glthread_DSAAttribPointer(ctx, vaobj, buffer, VERT_ATTRIB_COLOR_INDEX, 1, type, stride, offset);
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
uint32_t
_mesa_unmarshal_VertexArrayNormalOffsetEXT(struct gl_context *ctx, const struct marshal_cmd_VertexArrayNormalOffsetEXT *cmd)
{
   GLuint vaobj = cmd->vaobj;
   GLuint buffer = cmd->buffer;
   GLenum type = cmd->type;
   GLsizei stride = cmd->stride;
   GLintptr offset = cmd->offset;
   CALL_VertexArrayNormalOffsetEXT(ctx->CurrentServerDispatch, (vaobj, buffer, type, stride, offset));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexArrayNormalOffsetEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_VertexArrayNormalOffsetEXT(GLuint vaobj, GLuint buffer, GLenum type, GLsizei stride, GLintptr offset)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexArrayNormalOffsetEXT);
   struct marshal_cmd_VertexArrayNormalOffsetEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexArrayNormalOffsetEXT, cmd_size);
   cmd->vaobj = vaobj;
   cmd->buffer = buffer;
   cmd->type = MIN2(type, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->stride = stride;
   cmd->offset = offset;
   if (COMPAT) _mesa_glthread_DSAAttribPointer(ctx, vaobj, buffer, VERT_ATTRIB_NORMAL, 3, type, stride, offset);
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
uint32_t
_mesa_unmarshal_VertexArrayTexCoordOffsetEXT(struct gl_context *ctx, const struct marshal_cmd_VertexArrayTexCoordOffsetEXT *cmd)
{
   GLuint vaobj = cmd->vaobj;
   GLuint buffer = cmd->buffer;
   GLint size = cmd->size;
   GLenum type = cmd->type;
   GLsizei stride = cmd->stride;
   GLintptr offset = cmd->offset;
   CALL_VertexArrayTexCoordOffsetEXT(ctx->CurrentServerDispatch, (vaobj, buffer, size, type, stride, offset));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexArrayTexCoordOffsetEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_VertexArrayTexCoordOffsetEXT(GLuint vaobj, GLuint buffer, GLint size, GLenum type, GLsizei stride, GLintptr offset)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexArrayTexCoordOffsetEXT);
   struct marshal_cmd_VertexArrayTexCoordOffsetEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexArrayTexCoordOffsetEXT, cmd_size);
   cmd->vaobj = vaobj;
   cmd->buffer = buffer;
   cmd->size = size;
   cmd->type = MIN2(type, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->stride = stride;
   cmd->offset = offset;
   if (COMPAT) _mesa_glthread_DSAAttribPointer(ctx, vaobj, buffer, VERT_ATTRIB_TEX(ctx->GLThread.ClientActiveTexture), size, type, stride, offset);
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
uint32_t
_mesa_unmarshal_VertexArrayMultiTexCoordOffsetEXT(struct gl_context *ctx, const struct marshal_cmd_VertexArrayMultiTexCoordOffsetEXT *cmd)
{
   GLuint vaobj = cmd->vaobj;
   GLuint buffer = cmd->buffer;
   GLenum texunit = cmd->texunit;
   GLint size = cmd->size;
   GLenum type = cmd->type;
   GLsizei stride = cmd->stride;
   GLintptr offset = cmd->offset;
   CALL_VertexArrayMultiTexCoordOffsetEXT(ctx->CurrentServerDispatch, (vaobj, buffer, texunit, size, type, stride, offset));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexArrayMultiTexCoordOffsetEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_VertexArrayMultiTexCoordOffsetEXT(GLuint vaobj, GLuint buffer, GLenum texunit, GLint size, GLenum type, GLsizei stride, GLintptr offset)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexArrayMultiTexCoordOffsetEXT);
   struct marshal_cmd_VertexArrayMultiTexCoordOffsetEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexArrayMultiTexCoordOffsetEXT, cmd_size);
   cmd->vaobj = vaobj;
   cmd->buffer = buffer;
   cmd->texunit = MIN2(texunit, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->size = size;
   cmd->type = MIN2(type, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->stride = stride;
   cmd->offset = offset;
   if (COMPAT) _mesa_glthread_DSAAttribPointer(ctx, vaobj, buffer, VERT_ATTRIB_TEX(texunit - GL_TEXTURE0), size, type, stride, offset);
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
uint32_t
_mesa_unmarshal_VertexArrayFogCoordOffsetEXT(struct gl_context *ctx, const struct marshal_cmd_VertexArrayFogCoordOffsetEXT *cmd)
{
   GLuint vaobj = cmd->vaobj;
   GLuint buffer = cmd->buffer;
   GLenum type = cmd->type;
   GLsizei stride = cmd->stride;
   GLintptr offset = cmd->offset;
   CALL_VertexArrayFogCoordOffsetEXT(ctx->CurrentServerDispatch, (vaobj, buffer, type, stride, offset));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexArrayFogCoordOffsetEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_VertexArrayFogCoordOffsetEXT(GLuint vaobj, GLuint buffer, GLenum type, GLsizei stride, GLintptr offset)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexArrayFogCoordOffsetEXT);
   struct marshal_cmd_VertexArrayFogCoordOffsetEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexArrayFogCoordOffsetEXT, cmd_size);
   cmd->vaobj = vaobj;
   cmd->buffer = buffer;
   cmd->type = MIN2(type, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->stride = stride;
   cmd->offset = offset;
   if (COMPAT) _mesa_glthread_DSAAttribPointer(ctx, vaobj, buffer, VERT_ATTRIB_FOG, 1, type, stride, offset);
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
uint32_t
_mesa_unmarshal_VertexArraySecondaryColorOffsetEXT(struct gl_context *ctx, const struct marshal_cmd_VertexArraySecondaryColorOffsetEXT *cmd)
{
   GLuint vaobj = cmd->vaobj;
   GLuint buffer = cmd->buffer;
   GLint size = cmd->size;
   GLenum type = cmd->type;
   GLsizei stride = cmd->stride;
   GLintptr offset = cmd->offset;
   CALL_VertexArraySecondaryColorOffsetEXT(ctx->CurrentServerDispatch, (vaobj, buffer, size, type, stride, offset));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexArraySecondaryColorOffsetEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_VertexArraySecondaryColorOffsetEXT(GLuint vaobj, GLuint buffer, GLint size, GLenum type, GLsizei stride, GLintptr offset)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexArraySecondaryColorOffsetEXT);
   struct marshal_cmd_VertexArraySecondaryColorOffsetEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexArraySecondaryColorOffsetEXT, cmd_size);
   cmd->vaobj = vaobj;
   cmd->buffer = buffer;
   cmd->size = size;
   cmd->type = MIN2(type, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->stride = stride;
   cmd->offset = offset;
   if (COMPAT) _mesa_glthread_DSAAttribPointer(ctx, vaobj, buffer, VERT_ATTRIB_COLOR1, size, type, stride, offset);
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
uint32_t
_mesa_unmarshal_VertexArrayVertexAttribOffsetEXT(struct gl_context *ctx, const struct marshal_cmd_VertexArrayVertexAttribOffsetEXT *cmd)
{
   GLuint vaobj = cmd->vaobj;
   GLuint buffer = cmd->buffer;
   GLuint index = cmd->index;
   GLint size = cmd->size;
   GLenum type = cmd->type;
   GLboolean normalized = cmd->normalized;
   GLsizei stride = cmd->stride;
   GLintptr offset = cmd->offset;
   CALL_VertexArrayVertexAttribOffsetEXT(ctx->CurrentServerDispatch, (vaobj, buffer, index, size, type, normalized, stride, offset));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexArrayVertexAttribOffsetEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
   cmd->type = MIN2(type, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->normalized = normalized;
   cmd->stride = stride;
   cmd->offset = offset;
   if (COMPAT) _mesa_glthread_DSAAttribPointer(ctx, vaobj, buffer, VERT_ATTRIB_GENERIC(index), size, type, stride, offset);
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
uint32_t
_mesa_unmarshal_VertexArrayVertexAttribIOffsetEXT(struct gl_context *ctx, const struct marshal_cmd_VertexArrayVertexAttribIOffsetEXT *cmd)
{
   GLuint vaobj = cmd->vaobj;
   GLuint buffer = cmd->buffer;
   GLuint index = cmd->index;
   GLint size = cmd->size;
   GLenum type = cmd->type;
   GLsizei stride = cmd->stride;
   GLintptr offset = cmd->offset;
   CALL_VertexArrayVertexAttribIOffsetEXT(ctx->CurrentServerDispatch, (vaobj, buffer, index, size, type, stride, offset));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexArrayVertexAttribIOffsetEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
   cmd->type = MIN2(type, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->stride = stride;
   cmd->offset = offset;
   if (COMPAT) _mesa_glthread_DSAAttribPointer(ctx, vaobj, buffer, VERT_ATTRIB_GENERIC(index), size, type, stride, offset);
}


/* EnableVertexArrayEXT: marshalled asynchronously */
struct marshal_cmd_EnableVertexArrayEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 array;
   GLuint vaobj;
};
uint32_t
_mesa_unmarshal_EnableVertexArrayEXT(struct gl_context *ctx, const struct marshal_cmd_EnableVertexArrayEXT *cmd)
{
   GLuint vaobj = cmd->vaobj;
   GLenum array = cmd->array;
   CALL_EnableVertexArrayEXT(ctx->CurrentServerDispatch, (vaobj, array));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_EnableVertexArrayEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_EnableVertexArrayEXT(GLuint vaobj, GLenum array)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_EnableVertexArrayEXT);
   struct marshal_cmd_EnableVertexArrayEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_EnableVertexArrayEXT, cmd_size);
   cmd->vaobj = vaobj;
   cmd->array = MIN2(array, 0xffff); /* clamped to 0xffff (invalid enum) */
   if (COMPAT) _mesa_glthread_ClientState(ctx, &vaobj, _mesa_array_to_attrib(ctx, array), true);
}


/* DisableVertexArrayEXT: marshalled asynchronously */
struct marshal_cmd_DisableVertexArrayEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 array;
   GLuint vaobj;
};
uint32_t
_mesa_unmarshal_DisableVertexArrayEXT(struct gl_context *ctx, const struct marshal_cmd_DisableVertexArrayEXT *cmd)
{
   GLuint vaobj = cmd->vaobj;
   GLenum array = cmd->array;
   CALL_DisableVertexArrayEXT(ctx->CurrentServerDispatch, (vaobj, array));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_DisableVertexArrayEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_DisableVertexArrayEXT(GLuint vaobj, GLenum array)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_DisableVertexArrayEXT);
   struct marshal_cmd_DisableVertexArrayEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_DisableVertexArrayEXT, cmd_size);
   cmd->vaobj = vaobj;
   cmd->array = MIN2(array, 0xffff); /* clamped to 0xffff (invalid enum) */
   if (COMPAT) _mesa_glthread_ClientState(ctx, &vaobj, _mesa_array_to_attrib(ctx, array), false);
}


/* EnableVertexArrayAttribEXT: marshalled asynchronously */
struct marshal_cmd_EnableVertexArrayAttribEXT
{
   struct marshal_cmd_base cmd_base;
   GLuint vaobj;
   GLuint index;
};
uint32_t
_mesa_unmarshal_EnableVertexArrayAttribEXT(struct gl_context *ctx, const struct marshal_cmd_EnableVertexArrayAttribEXT *cmd)
{
   GLuint vaobj = cmd->vaobj;
   GLuint index = cmd->index;
   CALL_EnableVertexArrayAttribEXT(ctx->CurrentServerDispatch, (vaobj, index));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_EnableVertexArrayAttribEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
uint32_t
_mesa_unmarshal_DisableVertexArrayAttribEXT(struct gl_context *ctx, const struct marshal_cmd_DisableVertexArrayAttribEXT *cmd)
{
   GLuint vaobj = cmd->vaobj;
   GLuint index = cmd->index;
   CALL_DisableVertexArrayAttribEXT(ctx->CurrentServerDispatch, (vaobj, index));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_DisableVertexArrayAttribEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
static void GLAPIENTRY
_mesa_marshal_GetVertexArrayIntegervEXT(GLuint vaobj, GLenum pname, GLint* param)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetVertexArrayIntegervEXT");
   CALL_GetVertexArrayIntegervEXT(ctx->CurrentServerDispatch, (vaobj, pname, param));
}


/* GetVertexArrayPointervEXT: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetVertexArrayPointervEXT(GLuint vaobj, GLenum pname, GLvoid** param)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetVertexArrayPointervEXT");
   CALL_GetVertexArrayPointervEXT(ctx->CurrentServerDispatch, (vaobj, pname, param));
}


/* GetVertexArrayIntegeri_vEXT: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetVertexArrayIntegeri_vEXT(GLuint vaobj, GLuint index, GLenum pname, GLint* param)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetVertexArrayIntegeri_vEXT");
   CALL_GetVertexArrayIntegeri_vEXT(ctx->CurrentServerDispatch, (vaobj, index, pname, param));
}


/* GetVertexArrayPointeri_vEXT: marshalled synchronously */
static void GLAPIENTRY
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
uint32_t
_mesa_unmarshal_NamedProgramStringEXT(struct gl_context *ctx, const struct marshal_cmd_NamedProgramStringEXT *cmd)
{
   GLuint program = cmd->program;
   GLenum target = cmd->target;
   GLenum format = cmd->format;
   GLsizei len = cmd->len;
   GLvoid *string;
   const char *variable_data = (const char *) (cmd + 1);
   string = (GLvoid *) variable_data;
   CALL_NamedProgramStringEXT(ctx->CurrentServerDispatch, (program, target, format, len, string));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
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
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->format = MIN2(format, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->len = len;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, string, string_size);
}


/* GetNamedProgramStringEXT: marshalled synchronously */
static void GLAPIENTRY
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
uint32_t
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
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_NamedProgramLocalParameter4fEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_NamedProgramLocalParameter4fEXT(GLuint program, GLenum target, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_NamedProgramLocalParameter4fEXT);
   struct marshal_cmd_NamedProgramLocalParameter4fEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_NamedProgramLocalParameter4fEXT, cmd_size);
   cmd->program = program;
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
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
uint32_t
_mesa_unmarshal_NamedProgramLocalParameter4fvEXT(struct gl_context *ctx, const struct marshal_cmd_NamedProgramLocalParameter4fvEXT *cmd)
{
   GLuint program = cmd->program;
   GLenum target = cmd->target;
   GLuint index = cmd->index;
   const GLfloat *params = cmd->params;
   CALL_NamedProgramLocalParameter4fvEXT(ctx->CurrentServerDispatch, (program, target, index, params));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_NamedProgramLocalParameter4fvEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_NamedProgramLocalParameter4fvEXT(GLuint program, GLenum target, GLuint index, const GLfloat* params)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_NamedProgramLocalParameter4fvEXT);
   struct marshal_cmd_NamedProgramLocalParameter4fvEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_NamedProgramLocalParameter4fvEXT, cmd_size);
   cmd->program = program;
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->index = index;
   memcpy(cmd->params, params, 4 * sizeof(GLfloat));
}


/* GetNamedProgramLocalParameterfvEXT: marshalled synchronously */
static void GLAPIENTRY
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
uint32_t
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
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_NamedProgramLocalParameter4dEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_NamedProgramLocalParameter4dEXT(GLuint program, GLenum target, GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_NamedProgramLocalParameter4dEXT);
   struct marshal_cmd_NamedProgramLocalParameter4dEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_NamedProgramLocalParameter4dEXT, cmd_size);
   cmd->program = program;
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
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
uint32_t
_mesa_unmarshal_NamedProgramLocalParameter4dvEXT(struct gl_context *ctx, const struct marshal_cmd_NamedProgramLocalParameter4dvEXT *cmd)
{
   GLuint program = cmd->program;
   GLenum target = cmd->target;
   GLuint index = cmd->index;
   const GLdouble *params = cmd->params;
   CALL_NamedProgramLocalParameter4dvEXT(ctx->CurrentServerDispatch, (program, target, index, params));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_NamedProgramLocalParameter4dvEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_NamedProgramLocalParameter4dvEXT(GLuint program, GLenum target, GLuint index, const GLdouble* params)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_NamedProgramLocalParameter4dvEXT);
   struct marshal_cmd_NamedProgramLocalParameter4dvEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_NamedProgramLocalParameter4dvEXT, cmd_size);
   cmd->program = program;
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->index = index;
   memcpy(cmd->params, params, 4 * sizeof(GLdouble));
}


/* GetNamedProgramLocalParameterdvEXT: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetNamedProgramLocalParameterdvEXT(GLuint program, GLenum target, GLuint index, GLdouble* params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetNamedProgramLocalParameterdvEXT");
   CALL_GetNamedProgramLocalParameterdvEXT(ctx->CurrentServerDispatch, (program, target, index, params));
}


/* GetNamedProgramivEXT: marshalled synchronously */
static void GLAPIENTRY
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
uint32_t
_mesa_unmarshal_TextureBufferEXT(struct gl_context *ctx, const struct marshal_cmd_TextureBufferEXT *cmd)
{
   GLuint texture = cmd->texture;
   GLenum target = cmd->target;
   GLenum internalformat = cmd->internalformat;
   GLuint buffer = cmd->buffer;
   CALL_TextureBufferEXT(ctx->CurrentServerDispatch, (texture, target, internalformat, buffer));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_TextureBufferEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_TextureBufferEXT(GLuint texture, GLenum target, GLenum internalformat, GLuint buffer)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TextureBufferEXT);
   struct marshal_cmd_TextureBufferEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TextureBufferEXT, cmd_size);
   cmd->texture = texture;
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->internalformat = MIN2(internalformat, 0xffff); /* clamped to 0xffff (invalid enum) */
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
uint32_t
_mesa_unmarshal_MultiTexBufferEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexBufferEXT *cmd)
{
   GLenum texunit = cmd->texunit;
   GLenum target = cmd->target;
   GLenum internalformat = cmd->internalformat;
   GLuint buffer = cmd->buffer;
   CALL_MultiTexBufferEXT(ctx->CurrentServerDispatch, (texunit, target, internalformat, buffer));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MultiTexBufferEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_MultiTexBufferEXT(GLenum texunit, GLenum target, GLenum internalformat, GLuint buffer)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexBufferEXT);
   struct marshal_cmd_MultiTexBufferEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexBufferEXT, cmd_size);
   cmd->texunit = MIN2(texunit, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->internalformat = MIN2(internalformat, 0xffff); /* clamped to 0xffff (invalid enum) */
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
uint32_t
_mesa_unmarshal_TextureParameterIivEXT(struct gl_context *ctx, const struct marshal_cmd_TextureParameterIivEXT *cmd)
{
   GLuint texture = cmd->texture;
   GLenum target = cmd->target;
   GLenum pname = cmd->pname;
   GLint *params;
   const char *variable_data = (const char *) (cmd + 1);
   params = (GLint *) variable_data;
   CALL_TextureParameterIivEXT(ctx->CurrentServerDispatch, (texture, target, pname, params));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
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
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->pname = MIN2(pname, 0xffff); /* clamped to 0xffff (invalid enum) */
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
uint32_t
_mesa_unmarshal_TextureParameterIuivEXT(struct gl_context *ctx, const struct marshal_cmd_TextureParameterIuivEXT *cmd)
{
   GLuint texture = cmd->texture;
   GLenum target = cmd->target;
   GLenum pname = cmd->pname;
   GLuint *params;
   const char *variable_data = (const char *) (cmd + 1);
   params = (GLuint *) variable_data;
   CALL_TextureParameterIuivEXT(ctx->CurrentServerDispatch, (texture, target, pname, params));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
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
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->pname = MIN2(pname, 0xffff); /* clamped to 0xffff (invalid enum) */
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, params, params_size);
}


/* GetTextureParameterIivEXT: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetTextureParameterIivEXT(GLuint texture, GLenum target, GLenum pname, GLint* params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetTextureParameterIivEXT");
   CALL_GetTextureParameterIivEXT(ctx->CurrentServerDispatch, (texture, target, pname, params));
}


/* GetTextureParameterIuivEXT: marshalled synchronously */
static void GLAPIENTRY
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
uint32_t
_mesa_unmarshal_MultiTexParameterIivEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexParameterIivEXT *cmd)
{
   GLenum texunit = cmd->texunit;
   GLenum target = cmd->target;
   GLenum pname = cmd->pname;
   GLint *params;
   const char *variable_data = (const char *) (cmd + 1);
   params = (GLint *) variable_data;
   CALL_MultiTexParameterIivEXT(ctx->CurrentServerDispatch, (texunit, target, pname, params));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
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
   cmd->texunit = MIN2(texunit, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->pname = MIN2(pname, 0xffff); /* clamped to 0xffff (invalid enum) */
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
uint32_t
_mesa_unmarshal_MultiTexParameterIuivEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexParameterIuivEXT *cmd)
{
   GLenum texunit = cmd->texunit;
   GLenum target = cmd->target;
   GLenum pname = cmd->pname;
   GLuint *params;
   const char *variable_data = (const char *) (cmd + 1);
   params = (GLuint *) variable_data;
   CALL_MultiTexParameterIuivEXT(ctx->CurrentServerDispatch, (texunit, target, pname, params));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
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
   cmd->texunit = MIN2(texunit, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->pname = MIN2(pname, 0xffff); /* clamped to 0xffff (invalid enum) */
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, params, params_size);
}


/* GetMultiTexParameterIivEXT: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetMultiTexParameterIivEXT(GLenum texunit, GLenum target, GLenum pname, GLint* params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetMultiTexParameterIivEXT");
   CALL_GetMultiTexParameterIivEXT(ctx->CurrentServerDispatch, (texunit, target, pname, params));
}


/* GetMultiTexParameterIuivEXT: marshalled synchronously */
static void GLAPIENTRY
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
   /* Next safe_mul(count, 4 * sizeof(GLfloat)) bytes are GLfloat params[count][4] */
};
uint32_t
_mesa_unmarshal_NamedProgramLocalParameters4fvEXT(struct gl_context *ctx, const struct marshal_cmd_NamedProgramLocalParameters4fvEXT *cmd)
{
   GLuint program = cmd->program;
   GLenum target = cmd->target;
   GLuint index = cmd->index;
   GLsizei count = cmd->count;
   GLfloat *params;
   const char *variable_data = (const char *) (cmd + 1);
   params = (GLfloat *) variable_data;
   CALL_NamedProgramLocalParameters4fvEXT(ctx->CurrentServerDispatch, (program, target, index, count, params));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_NamedProgramLocalParameters4fvEXT(GLuint program, GLenum target, GLuint index, GLsizei count, const GLfloat* params)
{
   GET_CURRENT_CONTEXT(ctx);
   int params_size = safe_mul(count, 4 * sizeof(GLfloat));
   int cmd_size = sizeof(struct marshal_cmd_NamedProgramLocalParameters4fvEXT) + params_size;
   struct marshal_cmd_NamedProgramLocalParameters4fvEXT *cmd;
   if (unlikely(params_size < 0 || (params_size > 0 && !params) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "NamedProgramLocalParameters4fvEXT");
      CALL_NamedProgramLocalParameters4fvEXT(ctx->CurrentServerDispatch, (program, target, index, count, params));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_NamedProgramLocalParameters4fvEXT, cmd_size);
   cmd->program = program;
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->index = index;
   cmd->count = count;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, params, params_size);
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
uint32_t
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
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_BindImageTextureEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
   cmd->access = MIN2(access, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->format = format;
}


/* LabelObjectEXT: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_LabelObjectEXT(GLenum type, GLuint object, GLsizei length, const GLchar * label)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "LabelObjectEXT");
   CALL_LabelObjectEXT(ctx->CurrentServerDispatch, (type, object, length, label));
}


/* GetObjectLabelEXT: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetObjectLabelEXT(GLenum type, GLuint object, GLsizei bufSize, GLsizei * length, GLchar * label)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetObjectLabelEXT");
   CALL_GetObjectLabelEXT(ctx->CurrentServerDispatch, (type, object, bufSize, length, label));
}


/* SubpixelPrecisionBiasNV: marshalled asynchronously */
struct marshal_cmd_SubpixelPrecisionBiasNV
{
   struct marshal_cmd_base cmd_base;
   GLuint xbits;
   GLuint ybits;
};
uint32_t
_mesa_unmarshal_SubpixelPrecisionBiasNV(struct gl_context *ctx, const struct marshal_cmd_SubpixelPrecisionBiasNV *cmd)
{
   GLuint xbits = cmd->xbits;
   GLuint ybits = cmd->ybits;
   CALL_SubpixelPrecisionBiasNV(ctx->CurrentServerDispatch, (xbits, ybits));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_SubpixelPrecisionBiasNV), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
uint32_t
_mesa_unmarshal_ConservativeRasterParameterfNV(struct gl_context *ctx, const struct marshal_cmd_ConservativeRasterParameterfNV *cmd)
{
   GLenum pname = cmd->pname;
   GLfloat param = cmd->param;
   CALL_ConservativeRasterParameterfNV(ctx->CurrentServerDispatch, (pname, param));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_ConservativeRasterParameterfNV), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ConservativeRasterParameterfNV(GLenum pname, GLfloat param)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ConservativeRasterParameterfNV);
   struct marshal_cmd_ConservativeRasterParameterfNV *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ConservativeRasterParameterfNV, cmd_size);
   cmd->pname = MIN2(pname, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->param = param;
}


/* ConservativeRasterParameteriNV: marshalled asynchronously */
struct marshal_cmd_ConservativeRasterParameteriNV
{
   struct marshal_cmd_base cmd_base;
   GLenum16 pname;
   GLint param;
};
uint32_t
_mesa_unmarshal_ConservativeRasterParameteriNV(struct gl_context *ctx, const struct marshal_cmd_ConservativeRasterParameteriNV *cmd)
{
   GLenum pname = cmd->pname;
   GLint param = cmd->param;
   CALL_ConservativeRasterParameteriNV(ctx->CurrentServerDispatch, (pname, param));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_ConservativeRasterParameteriNV), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ConservativeRasterParameteriNV(GLenum pname, GLint param)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ConservativeRasterParameteriNV);
   struct marshal_cmd_ConservativeRasterParameteriNV *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ConservativeRasterParameteriNV, cmd_size);
   cmd->pname = MIN2(pname, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->param = param;
}


/* GetFirstPerfQueryIdINTEL: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetFirstPerfQueryIdINTEL(GLuint * queryId)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetFirstPerfQueryIdINTEL");
   CALL_GetFirstPerfQueryIdINTEL(ctx->CurrentServerDispatch, (queryId));
}


/* GetNextPerfQueryIdINTEL: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetNextPerfQueryIdINTEL(GLuint queryId, GLuint * nextQueryId)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetNextPerfQueryIdINTEL");
   CALL_GetNextPerfQueryIdINTEL(ctx->CurrentServerDispatch, (queryId, nextQueryId));
}


/* GetPerfQueryIdByNameINTEL: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetPerfQueryIdByNameINTEL(GLchar * queryName, GLuint * queryId)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetPerfQueryIdByNameINTEL");
   CALL_GetPerfQueryIdByNameINTEL(ctx->CurrentServerDispatch, (queryName, queryId));
}


/* GetPerfQueryInfoINTEL: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetPerfQueryInfoINTEL(GLuint queryId, GLuint queryNameLength, GLchar * queryName, GLuint * dataSize, GLuint * noCounters, GLuint * noInstances, GLuint * capsMask)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetPerfQueryInfoINTEL");
   CALL_GetPerfQueryInfoINTEL(ctx->CurrentServerDispatch, (queryId, queryNameLength, queryName, dataSize, noCounters, noInstances, capsMask));
}


/* GetPerfCounterInfoINTEL: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetPerfCounterInfoINTEL(GLuint queryId, GLuint counterId, GLuint counterNameLength, GLchar * counterName, GLuint counterDescLength, GLchar * counterDesc, GLuint * counterOffset, GLuint * counterDataSize, GLuint * counterTypeEnum, GLuint * counterDataTypeEnum, GLuint64 * rawCounterMaxValue)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetPerfCounterInfoINTEL");
   CALL_GetPerfCounterInfoINTEL(ctx->CurrentServerDispatch, (queryId, counterId, counterNameLength, counterName, counterDescLength, counterDesc, counterOffset, counterDataSize, counterTypeEnum, counterDataTypeEnum, rawCounterMaxValue));
}


/* CreatePerfQueryINTEL: marshalled synchronously */
static void GLAPIENTRY
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
uint32_t
_mesa_unmarshal_DeletePerfQueryINTEL(struct gl_context *ctx, const struct marshal_cmd_DeletePerfQueryINTEL *cmd)
{
   GLuint queryHandle = cmd->queryHandle;
   CALL_DeletePerfQueryINTEL(ctx->CurrentServerDispatch, (queryHandle));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_DeletePerfQueryINTEL), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
uint32_t
_mesa_unmarshal_BeginPerfQueryINTEL(struct gl_context *ctx, const struct marshal_cmd_BeginPerfQueryINTEL *cmd)
{
   GLuint queryHandle = cmd->queryHandle;
   CALL_BeginPerfQueryINTEL(ctx->CurrentServerDispatch, (queryHandle));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_BeginPerfQueryINTEL), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
uint32_t
_mesa_unmarshal_EndPerfQueryINTEL(struct gl_context *ctx, const struct marshal_cmd_EndPerfQueryINTEL *cmd)
{
   GLuint queryHandle = cmd->queryHandle;
   CALL_EndPerfQueryINTEL(ctx->CurrentServerDispatch, (queryHandle));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_EndPerfQueryINTEL), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_EndPerfQueryINTEL(GLuint queryHandle)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_EndPerfQueryINTEL);
   struct marshal_cmd_EndPerfQueryINTEL *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_EndPerfQueryINTEL, cmd_size);
   cmd->queryHandle = queryHandle;
}


/* GetPerfQueryDataINTEL: marshalled synchronously */
static void GLAPIENTRY
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
uint32_t
_mesa_unmarshal_AlphaToCoverageDitherControlNV(struct gl_context *ctx, const struct marshal_cmd_AlphaToCoverageDitherControlNV *cmd)
{
   GLenum mode = cmd->mode;
   CALL_AlphaToCoverageDitherControlNV(ctx->CurrentServerDispatch, (mode));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_AlphaToCoverageDitherControlNV), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_AlphaToCoverageDitherControlNV(GLenum mode)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_AlphaToCoverageDitherControlNV);
   struct marshal_cmd_AlphaToCoverageDitherControlNV *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_AlphaToCoverageDitherControlNV, cmd_size);
   cmd->mode = MIN2(mode, 0xffff); /* clamped to 0xffff (invalid enum) */
}


/* PolygonOffsetClampEXT: marshalled asynchronously */
struct marshal_cmd_PolygonOffsetClampEXT
{
   struct marshal_cmd_base cmd_base;
   GLfloat factor;
   GLfloat units;
   GLfloat clamp;
};
uint32_t
_mesa_unmarshal_PolygonOffsetClampEXT(struct gl_context *ctx, const struct marshal_cmd_PolygonOffsetClampEXT *cmd)
{
   GLfloat factor = cmd->factor;
   GLfloat units = cmd->units;
   GLfloat clamp = cmd->clamp;
   CALL_PolygonOffsetClampEXT(ctx->CurrentServerDispatch, (factor, units, clamp));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_PolygonOffsetClampEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
uint32_t
_mesa_unmarshal_WindowRectanglesEXT(struct gl_context *ctx, const struct marshal_cmd_WindowRectanglesEXT *cmd)
{
   GLenum mode = cmd->mode;
   GLsizei count = cmd->count;
   GLint *box;
   const char *variable_data = (const char *) (cmd + 1);
   box = (GLint *) variable_data;
   CALL_WindowRectanglesEXT(ctx->CurrentServerDispatch, (mode, count, box));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
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
   cmd->mode = MIN2(mode, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->count = count;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, box, box_size);
}


/* FramebufferFetchBarrierEXT: marshalled asynchronously */
struct marshal_cmd_FramebufferFetchBarrierEXT
{
   struct marshal_cmd_base cmd_base;
};
uint32_t
_mesa_unmarshal_FramebufferFetchBarrierEXT(struct gl_context *ctx, const struct marshal_cmd_FramebufferFetchBarrierEXT *cmd)
{
   CALL_FramebufferFetchBarrierEXT(ctx->CurrentServerDispatch, ());
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_FramebufferFetchBarrierEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
uint32_t
_mesa_unmarshal_RenderbufferStorageMultisampleAdvancedAMD(struct gl_context *ctx, const struct marshal_cmd_RenderbufferStorageMultisampleAdvancedAMD *cmd)
{
   GLenum target = cmd->target;
   GLsizei samples = cmd->samples;
   GLsizei storageSamples = cmd->storageSamples;
   GLenum internalformat = cmd->internalformat;
   GLsizei width = cmd->width;
   GLsizei height = cmd->height;
   CALL_RenderbufferStorageMultisampleAdvancedAMD(ctx->CurrentServerDispatch, (target, samples, storageSamples, internalformat, width, height));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_RenderbufferStorageMultisampleAdvancedAMD), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_RenderbufferStorageMultisampleAdvancedAMD(GLenum target, GLsizei samples, GLsizei storageSamples, GLenum internalformat, GLsizei width, GLsizei height)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_RenderbufferStorageMultisampleAdvancedAMD);
   struct marshal_cmd_RenderbufferStorageMultisampleAdvancedAMD *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_RenderbufferStorageMultisampleAdvancedAMD, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->samples = samples;
   cmd->storageSamples = storageSamples;
   cmd->internalformat = MIN2(internalformat, 0xffff); /* clamped to 0xffff (invalid enum) */
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
uint32_t
_mesa_unmarshal_NamedRenderbufferStorageMultisampleAdvancedAMD(struct gl_context *ctx, const struct marshal_cmd_NamedRenderbufferStorageMultisampleAdvancedAMD *cmd)
{
   GLuint renderbuffer = cmd->renderbuffer;
   GLsizei samples = cmd->samples;
   GLsizei storageSamples = cmd->storageSamples;
   GLenum internalformat = cmd->internalformat;
   GLsizei width = cmd->width;
   GLsizei height = cmd->height;
   CALL_NamedRenderbufferStorageMultisampleAdvancedAMD(ctx->CurrentServerDispatch, (renderbuffer, samples, storageSamples, internalformat, width, height));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_NamedRenderbufferStorageMultisampleAdvancedAMD), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_NamedRenderbufferStorageMultisampleAdvancedAMD(GLuint renderbuffer, GLsizei samples, GLsizei storageSamples, GLenum internalformat, GLsizei width, GLsizei height)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_NamedRenderbufferStorageMultisampleAdvancedAMD);
   struct marshal_cmd_NamedRenderbufferStorageMultisampleAdvancedAMD *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_NamedRenderbufferStorageMultisampleAdvancedAMD, cmd_size);
   cmd->renderbuffer = renderbuffer;
   cmd->samples = samples;
   cmd->storageSamples = storageSamples;
   cmd->internalformat = MIN2(internalformat, 0xffff); /* clamped to 0xffff (invalid enum) */
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
uint32_t
_mesa_unmarshal_StencilFuncSeparateATI(struct gl_context *ctx, const struct marshal_cmd_StencilFuncSeparateATI *cmd)
{
   GLenum frontfunc = cmd->frontfunc;
   GLenum backfunc = cmd->backfunc;
   GLint ref = cmd->ref;
   GLuint mask = cmd->mask;
   CALL_StencilFuncSeparateATI(ctx->CurrentServerDispatch, (frontfunc, backfunc, ref, mask));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_StencilFuncSeparateATI), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_StencilFuncSeparateATI(GLenum frontfunc, GLenum backfunc, GLint ref, GLuint mask)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_StencilFuncSeparateATI);
   struct marshal_cmd_StencilFuncSeparateATI *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_StencilFuncSeparateATI, cmd_size);
   cmd->frontfunc = MIN2(frontfunc, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->backfunc = MIN2(backfunc, 0xffff); /* clamped to 0xffff (invalid enum) */
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
   /* Next safe_mul(count, 4 * sizeof(GLfloat)) bytes are GLfloat params[count][4] */
};
uint32_t
_mesa_unmarshal_ProgramEnvParameters4fvEXT(struct gl_context *ctx, const struct marshal_cmd_ProgramEnvParameters4fvEXT *cmd)
{
   GLenum target = cmd->target;
   GLuint index = cmd->index;
   GLsizei count = cmd->count;
   GLfloat *params;
   const char *variable_data = (const char *) (cmd + 1);
   params = (GLfloat *) variable_data;
   CALL_ProgramEnvParameters4fvEXT(ctx->CurrentServerDispatch, (target, index, count, params));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ProgramEnvParameters4fvEXT(GLenum target, GLuint index, GLsizei count, const GLfloat * params)
{
   GET_CURRENT_CONTEXT(ctx);
   int params_size = safe_mul(count, 4 * sizeof(GLfloat));
   int cmd_size = sizeof(struct marshal_cmd_ProgramEnvParameters4fvEXT) + params_size;
   struct marshal_cmd_ProgramEnvParameters4fvEXT *cmd;
   if (unlikely(params_size < 0 || (params_size > 0 && !params) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "ProgramEnvParameters4fvEXT");
      CALL_ProgramEnvParameters4fvEXT(ctx->CurrentServerDispatch, (target, index, count, params));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ProgramEnvParameters4fvEXT, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->index = index;
   cmd->count = count;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, params, params_size);
}


/* ProgramLocalParameters4fvEXT: marshalled asynchronously */
struct marshal_cmd_ProgramLocalParameters4fvEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLuint index;
   GLsizei count;
   /* Next safe_mul(count, 4 * sizeof(GLfloat)) bytes are GLfloat params[count][4] */
};
uint32_t
_mesa_unmarshal_ProgramLocalParameters4fvEXT(struct gl_context *ctx, const struct marshal_cmd_ProgramLocalParameters4fvEXT *cmd)
{
   GLenum target = cmd->target;
   GLuint index = cmd->index;
   GLsizei count = cmd->count;
   GLfloat *params;
   const char *variable_data = (const char *) (cmd + 1);
   params = (GLfloat *) variable_data;
   CALL_ProgramLocalParameters4fvEXT(ctx->CurrentServerDispatch, (target, index, count, params));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ProgramLocalParameters4fvEXT(GLenum target, GLuint index, GLsizei count, const GLfloat * params)
{
   GET_CURRENT_CONTEXT(ctx);
   int params_size = safe_mul(count, 4 * sizeof(GLfloat));
   int cmd_size = sizeof(struct marshal_cmd_ProgramLocalParameters4fvEXT) + params_size;
   struct marshal_cmd_ProgramLocalParameters4fvEXT *cmd;
   if (unlikely(params_size < 0 || (params_size > 0 && !params) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "ProgramLocalParameters4fvEXT");
      CALL_ProgramLocalParameters4fvEXT(ctx->CurrentServerDispatch, (target, index, count, params));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ProgramLocalParameters4fvEXT, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->index = index;
   cmd->count = count;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, params, params_size);
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
uint32_t
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
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_InternalBufferSubDataCopyMESA), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
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
   GLenum16 error;
};
uint32_t
_mesa_unmarshal_InternalSetError(struct gl_context *ctx, const struct marshal_cmd_InternalSetError *cmd)
{
   GLenum error = cmd->error;
   CALL_InternalSetError(ctx->CurrentServerDispatch, (error));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_InternalSetError), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_InternalSetError(GLenum error)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_InternalSetError);
   struct marshal_cmd_InternalSetError *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_InternalSetError, cmd_size);
   cmd->error = MIN2(error, 0xffff); /* clamped to 0xffff (invalid enum) */
}


/* EGLImageTargetTexture2DOES: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_EGLImageTargetTexture2DOES(GLenum target, GLvoid * writeOffset)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "EGLImageTargetTexture2DOES");
   CALL_EGLImageTargetTexture2DOES(ctx->CurrentServerDispatch, (target, writeOffset));
}


/* EGLImageTargetRenderbufferStorageOES: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_EGLImageTargetRenderbufferStorageOES(GLenum target, GLvoid * writeOffset)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "EGLImageTargetRenderbufferStorageOES");
   CALL_EGLImageTargetRenderbufferStorageOES(ctx->CurrentServerDispatch, (target, writeOffset));
}


/* EGLImageTargetTexStorageEXT: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_EGLImageTargetTexStorageEXT(GLenum target, GLvoid * image, const GLint * attrib_list)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "EGLImageTargetTexStorageEXT");
   CALL_EGLImageTargetTexStorageEXT(ctx->CurrentServerDispatch, (target, image, attrib_list));
}


/* EGLImageTargetTextureStorageEXT: marshalled synchronously */
static void GLAPIENTRY
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
uint32_t
_mesa_unmarshal_ClearColorIiEXT(struct gl_context *ctx, const struct marshal_cmd_ClearColorIiEXT *cmd)
{
   GLint r = cmd->r;
   GLint g = cmd->g;
   GLint b = cmd->b;
   GLint a = cmd->a;
   CALL_ClearColorIiEXT(ctx->CurrentServerDispatch, (r, g, b, a));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_ClearColorIiEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
uint32_t
_mesa_unmarshal_ClearColorIuiEXT(struct gl_context *ctx, const struct marshal_cmd_ClearColorIuiEXT *cmd)
{
   GLuint r = cmd->r;
   GLuint g = cmd->g;
   GLuint b = cmd->b;
   GLuint a = cmd->a;
   CALL_ClearColorIuiEXT(ctx->CurrentServerDispatch, (r, g, b, a));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_ClearColorIuiEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
uint32_t
_mesa_unmarshal_TexParameterIiv(struct gl_context *ctx, const struct marshal_cmd_TexParameterIiv *cmd)
{
   GLenum target = cmd->target;
   GLenum pname = cmd->pname;
   GLint *params;
   const char *variable_data = (const char *) (cmd + 1);
   params = (GLint *) variable_data;
   CALL_TexParameterIiv(ctx->CurrentServerDispatch, (target, pname, params));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
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
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->pname = MIN2(pname, 0xffff); /* clamped to 0xffff (invalid enum) */
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
uint32_t
_mesa_unmarshal_TexParameterIuiv(struct gl_context *ctx, const struct marshal_cmd_TexParameterIuiv *cmd)
{
   GLenum target = cmd->target;
   GLenum pname = cmd->pname;
   GLuint *params;
   const char *variable_data = (const char *) (cmd + 1);
   params = (GLuint *) variable_data;
   CALL_TexParameterIuiv(ctx->CurrentServerDispatch, (target, pname, params));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
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
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->pname = MIN2(pname, 0xffff); /* clamped to 0xffff (invalid enum) */
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, params, params_size);
}


/* GetTexParameterIiv: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetTexParameterIiv(GLenum target, GLenum pname, GLint * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetTexParameterIiv");
   CALL_GetTexParameterIiv(ctx->CurrentServerDispatch, (target, pname, params));
}


/* GetTexParameterIuiv: marshalled synchronously */
static void GLAPIENTRY
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
uint32_t
_mesa_unmarshal_VertexAttribI1iEXT(struct gl_context *ctx, const struct marshal_cmd_VertexAttribI1iEXT *cmd)
{
   GLuint index = cmd->index;
   GLint x = cmd->x;
   CALL_VertexAttribI1iEXT(ctx->CurrentServerDispatch, (index, x));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexAttribI1iEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
uint32_t
_mesa_unmarshal_VertexAttribI2iEXT(struct gl_context *ctx, const struct marshal_cmd_VertexAttribI2iEXT *cmd)
{
   GLuint index = cmd->index;
   GLint x = cmd->x;
   GLint y = cmd->y;
   CALL_VertexAttribI2iEXT(ctx->CurrentServerDispatch, (index, x, y));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexAttribI2iEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
uint32_t
_mesa_unmarshal_VertexAttribI3iEXT(struct gl_context *ctx, const struct marshal_cmd_VertexAttribI3iEXT *cmd)
{
   GLuint index = cmd->index;
   GLint x = cmd->x;
   GLint y = cmd->y;
   GLint z = cmd->z;
   CALL_VertexAttribI3iEXT(ctx->CurrentServerDispatch, (index, x, y, z));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexAttribI3iEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
uint32_t
_mesa_unmarshal_VertexAttribI4iEXT(struct gl_context *ctx, const struct marshal_cmd_VertexAttribI4iEXT *cmd)
{
   GLuint index = cmd->index;
   GLint x = cmd->x;
   GLint y = cmd->y;
   GLint z = cmd->z;
   GLint w = cmd->w;
   CALL_VertexAttribI4iEXT(ctx->CurrentServerDispatch, (index, x, y, z, w));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexAttribI4iEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
uint32_t
_mesa_unmarshal_VertexAttribI1uiEXT(struct gl_context *ctx, const struct marshal_cmd_VertexAttribI1uiEXT *cmd)
{
   GLuint index = cmd->index;
   GLuint x = cmd->x;
   CALL_VertexAttribI1uiEXT(ctx->CurrentServerDispatch, (index, x));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexAttribI1uiEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
uint32_t
_mesa_unmarshal_VertexAttribI2uiEXT(struct gl_context *ctx, const struct marshal_cmd_VertexAttribI2uiEXT *cmd)
{
   GLuint index = cmd->index;
   GLuint x = cmd->x;
   GLuint y = cmd->y;
   CALL_VertexAttribI2uiEXT(ctx->CurrentServerDispatch, (index, x, y));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexAttribI2uiEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
uint32_t
_mesa_unmarshal_VertexAttribI3uiEXT(struct gl_context *ctx, const struct marshal_cmd_VertexAttribI3uiEXT *cmd)
{
   GLuint index = cmd->index;
   GLuint x = cmd->x;
   GLuint y = cmd->y;
   GLuint z = cmd->z;
   CALL_VertexAttribI3uiEXT(ctx->CurrentServerDispatch, (index, x, y, z));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexAttribI3uiEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
uint32_t
_mesa_unmarshal_VertexAttribI4uiEXT(struct gl_context *ctx, const struct marshal_cmd_VertexAttribI4uiEXT *cmd)
{
   GLuint index = cmd->index;
   GLuint x = cmd->x;
   GLuint y = cmd->y;
   GLuint z = cmd->z;
   GLuint w = cmd->w;
   CALL_VertexAttribI4uiEXT(ctx->CurrentServerDispatch, (index, x, y, z, w));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexAttribI4uiEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
uint32_t
_mesa_unmarshal_VertexAttribI1iv(struct gl_context *ctx, const struct marshal_cmd_VertexAttribI1iv *cmd)
{
   GLuint index = cmd->index;
   const GLint *v = cmd->v;
   CALL_VertexAttribI1iv(ctx->CurrentServerDispatch, (index, v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexAttribI1iv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
uint32_t
_mesa_unmarshal_VertexAttribI2ivEXT(struct gl_context *ctx, const struct marshal_cmd_VertexAttribI2ivEXT *cmd)
{
   GLuint index = cmd->index;
   const GLint *v = cmd->v;
   CALL_VertexAttribI2ivEXT(ctx->CurrentServerDispatch, (index, v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexAttribI2ivEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
uint32_t
_mesa_unmarshal_VertexAttribI3ivEXT(struct gl_context *ctx, const struct marshal_cmd_VertexAttribI3ivEXT *cmd)
{
   GLuint index = cmd->index;
   const GLint *v = cmd->v;
   CALL_VertexAttribI3ivEXT(ctx->CurrentServerDispatch, (index, v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexAttribI3ivEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
uint32_t
_mesa_unmarshal_VertexAttribI4ivEXT(struct gl_context *ctx, const struct marshal_cmd_VertexAttribI4ivEXT *cmd)
{
   GLuint index = cmd->index;
   const GLint *v = cmd->v;
   CALL_VertexAttribI4ivEXT(ctx->CurrentServerDispatch, (index, v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexAttribI4ivEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
uint32_t
_mesa_unmarshal_VertexAttribI1uiv(struct gl_context *ctx, const struct marshal_cmd_VertexAttribI1uiv *cmd)
{
   GLuint index = cmd->index;
   const GLuint *v = cmd->v;
   CALL_VertexAttribI1uiv(ctx->CurrentServerDispatch, (index, v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexAttribI1uiv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
uint32_t
_mesa_unmarshal_VertexAttribI2uivEXT(struct gl_context *ctx, const struct marshal_cmd_VertexAttribI2uivEXT *cmd)
{
   GLuint index = cmd->index;
   const GLuint *v = cmd->v;
   CALL_VertexAttribI2uivEXT(ctx->CurrentServerDispatch, (index, v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexAttribI2uivEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
uint32_t
_mesa_unmarshal_VertexAttribI3uivEXT(struct gl_context *ctx, const struct marshal_cmd_VertexAttribI3uivEXT *cmd)
{
   GLuint index = cmd->index;
   const GLuint *v = cmd->v;
   CALL_VertexAttribI3uivEXT(ctx->CurrentServerDispatch, (index, v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexAttribI3uivEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
uint32_t
_mesa_unmarshal_VertexAttribI4uivEXT(struct gl_context *ctx, const struct marshal_cmd_VertexAttribI4uivEXT *cmd)
{
   GLuint index = cmd->index;
   const GLuint *v = cmd->v;
   CALL_VertexAttribI4uivEXT(ctx->CurrentServerDispatch, (index, v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexAttribI4uivEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
uint32_t
_mesa_unmarshal_VertexAttribI4bv(struct gl_context *ctx, const struct marshal_cmd_VertexAttribI4bv *cmd)
{
   GLuint index = cmd->index;
   const GLbyte *v = cmd->v;
   CALL_VertexAttribI4bv(ctx->CurrentServerDispatch, (index, v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexAttribI4bv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_VertexAttribI4bv(GLuint index, const GLbyte * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttribI4bv);
   struct marshal_cmd_VertexAttribI4bv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttribI4bv, cmd_size);
   cmd->index = index;
   memcpy(cmd->v, v, 4 * sizeof(GLbyte));
}


void
_mesa_glthread_init_dispatch6(struct gl_context *ctx, struct _glapi_table *table)
{
   if (_mesa_is_desktop_gl(ctx)) {
      SET_AlphaToCoverageDitherControlNV(table, _mesa_marshal_AlphaToCoverageDitherControlNV);
      SET_BindImageTextureEXT(table, _mesa_marshal_BindImageTextureEXT);
      SET_BindMultiTextureEXT(table, _mesa_marshal_BindMultiTextureEXT);
      SET_CheckNamedFramebufferStatusEXT(table, _mesa_marshal_CheckNamedFramebufferStatusEXT);
      SET_ClearColorIiEXT(table, _mesa_marshal_ClearColorIiEXT);
      SET_ClearColorIuiEXT(table, _mesa_marshal_ClearColorIuiEXT);
      SET_ClientAttribDefaultEXT(table, _mesa_marshal_ClientAttribDefaultEXT);
      SET_CompressedMultiTexImage1DEXT(table, _mesa_marshal_CompressedMultiTexImage1DEXT);
      SET_CompressedMultiTexImage2DEXT(table, _mesa_marshal_CompressedMultiTexImage2DEXT);
      SET_CompressedMultiTexImage3DEXT(table, _mesa_marshal_CompressedMultiTexImage3DEXT);
      SET_CompressedMultiTexSubImage1DEXT(table, _mesa_marshal_CompressedMultiTexSubImage1DEXT);
      SET_CompressedMultiTexSubImage2DEXT(table, _mesa_marshal_CompressedMultiTexSubImage2DEXT);
      SET_CompressedMultiTexSubImage3DEXT(table, _mesa_marshal_CompressedMultiTexSubImage3DEXT);
      SET_CompressedTextureImage1DEXT(table, _mesa_marshal_CompressedTextureImage1DEXT);
      SET_CompressedTextureImage2DEXT(table, _mesa_marshal_CompressedTextureImage2DEXT);
      SET_CompressedTextureImage3DEXT(table, _mesa_marshal_CompressedTextureImage3DEXT);
      SET_CompressedTextureSubImage1DEXT(table, _mesa_marshal_CompressedTextureSubImage1DEXT);
      SET_CompressedTextureSubImage2DEXT(table, _mesa_marshal_CompressedTextureSubImage2DEXT);
      SET_CompressedTextureSubImage3DEXT(table, _mesa_marshal_CompressedTextureSubImage3DEXT);
      SET_CopyMultiTexImage1DEXT(table, _mesa_marshal_CopyMultiTexImage1DEXT);
      SET_CopyMultiTexImage2DEXT(table, _mesa_marshal_CopyMultiTexImage2DEXT);
      SET_CopyMultiTexSubImage1DEXT(table, _mesa_marshal_CopyMultiTexSubImage1DEXT);
      SET_CopyMultiTexSubImage2DEXT(table, _mesa_marshal_CopyMultiTexSubImage2DEXT);
      SET_CopyMultiTexSubImage3DEXT(table, _mesa_marshal_CopyMultiTexSubImage3DEXT);
      SET_CopyTextureImage1DEXT(table, _mesa_marshal_CopyTextureImage1DEXT);
      SET_CopyTextureImage2DEXT(table, _mesa_marshal_CopyTextureImage2DEXT);
      SET_CopyTextureSubImage1DEXT(table, _mesa_marshal_CopyTextureSubImage1DEXT);
      SET_CopyTextureSubImage2DEXT(table, _mesa_marshal_CopyTextureSubImage2DEXT);
      SET_CopyTextureSubImage3DEXT(table, _mesa_marshal_CopyTextureSubImage3DEXT);
      SET_DisableClientStateiEXT(table, _mesa_marshal_DisableClientStateiEXT);
      SET_DisableVertexArrayAttribEXT(table, _mesa_marshal_DisableVertexArrayAttribEXT);
      SET_DisableVertexArrayEXT(table, _mesa_marshal_DisableVertexArrayEXT);
      SET_EGLImageTargetTextureStorageEXT(table, _mesa_marshal_EGLImageTargetTextureStorageEXT);
      SET_EnableClientStateiEXT(table, _mesa_marshal_EnableClientStateiEXT);
      SET_EnableVertexArrayAttribEXT(table, _mesa_marshal_EnableVertexArrayAttribEXT);
      SET_EnableVertexArrayEXT(table, _mesa_marshal_EnableVertexArrayEXT);
      SET_FlushMappedNamedBufferRangeEXT(table, _mesa_marshal_FlushMappedNamedBufferRangeEXT);
      SET_FramebufferDrawBufferEXT(table, _mesa_marshal_FramebufferDrawBufferEXT);
      SET_FramebufferDrawBuffersEXT(table, _mesa_marshal_FramebufferDrawBuffersEXT);
      SET_FramebufferReadBufferEXT(table, _mesa_marshal_FramebufferReadBufferEXT);
      SET_GenerateMultiTexMipmapEXT(table, _mesa_marshal_GenerateMultiTexMipmapEXT);
      SET_GenerateTextureMipmapEXT(table, _mesa_marshal_GenerateTextureMipmapEXT);
      SET_GetCompressedMultiTexImageEXT(table, _mesa_marshal_GetCompressedMultiTexImageEXT);
      SET_GetCompressedTextureImageEXT(table, _mesa_marshal_GetCompressedTextureImageEXT);
      SET_GetFramebufferParameterivEXT(table, _mesa_marshal_GetFramebufferParameterivEXT);
      SET_GetMultiTexEnvfvEXT(table, _mesa_marshal_GetMultiTexEnvfvEXT);
      SET_GetMultiTexEnvivEXT(table, _mesa_marshal_GetMultiTexEnvivEXT);
      SET_GetMultiTexGendvEXT(table, _mesa_marshal_GetMultiTexGendvEXT);
      SET_GetMultiTexGenfvEXT(table, _mesa_marshal_GetMultiTexGenfvEXT);
      SET_GetMultiTexGenivEXT(table, _mesa_marshal_GetMultiTexGenivEXT);
      SET_GetMultiTexImageEXT(table, _mesa_marshal_GetMultiTexImageEXT);
      SET_GetMultiTexLevelParameterfvEXT(table, _mesa_marshal_GetMultiTexLevelParameterfvEXT);
      SET_GetMultiTexLevelParameterivEXT(table, _mesa_marshal_GetMultiTexLevelParameterivEXT);
      SET_GetMultiTexParameterIivEXT(table, _mesa_marshal_GetMultiTexParameterIivEXT);
      SET_GetMultiTexParameterIuivEXT(table, _mesa_marshal_GetMultiTexParameterIuivEXT);
      SET_GetMultiTexParameterfvEXT(table, _mesa_marshal_GetMultiTexParameterfvEXT);
      SET_GetMultiTexParameterivEXT(table, _mesa_marshal_GetMultiTexParameterivEXT);
      SET_GetNamedBufferParameterivEXT(table, _mesa_marshal_GetNamedBufferParameterivEXT);
      SET_GetNamedBufferPointervEXT(table, _mesa_marshal_GetNamedBufferPointervEXT);
      SET_GetNamedBufferSubDataEXT(table, _mesa_marshal_GetNamedBufferSubDataEXT);
      SET_GetNamedFramebufferAttachmentParameterivEXT(table, _mesa_marshal_GetNamedFramebufferAttachmentParameterivEXT);
      SET_GetNamedProgramLocalParameterdvEXT(table, _mesa_marshal_GetNamedProgramLocalParameterdvEXT);
      SET_GetNamedProgramLocalParameterfvEXT(table, _mesa_marshal_GetNamedProgramLocalParameterfvEXT);
      SET_GetNamedProgramStringEXT(table, _mesa_marshal_GetNamedProgramStringEXT);
      SET_GetNamedProgramivEXT(table, _mesa_marshal_GetNamedProgramivEXT);
      SET_GetNamedRenderbufferParameterivEXT(table, _mesa_marshal_GetNamedRenderbufferParameterivEXT);
      SET_GetPointerIndexedvEXT(table, _mesa_marshal_GetPointerIndexedvEXT);
      SET_GetTextureImageEXT(table, _mesa_marshal_GetTextureImageEXT);
      SET_GetTextureLevelParameterfvEXT(table, _mesa_marshal_GetTextureLevelParameterfvEXT);
      SET_GetTextureLevelParameterivEXT(table, _mesa_marshal_GetTextureLevelParameterivEXT);
      SET_GetTextureParameterIivEXT(table, _mesa_marshal_GetTextureParameterIivEXT);
      SET_GetTextureParameterIuivEXT(table, _mesa_marshal_GetTextureParameterIuivEXT);
      SET_GetTextureParameterfvEXT(table, _mesa_marshal_GetTextureParameterfvEXT);
      SET_GetTextureParameterivEXT(table, _mesa_marshal_GetTextureParameterivEXT);
      SET_GetVertexArrayIntegeri_vEXT(table, _mesa_marshal_GetVertexArrayIntegeri_vEXT);
      SET_GetVertexArrayIntegervEXT(table, _mesa_marshal_GetVertexArrayIntegervEXT);
      SET_GetVertexArrayPointeri_vEXT(table, _mesa_marshal_GetVertexArrayPointeri_vEXT);
      SET_GetVertexArrayPointervEXT(table, _mesa_marshal_GetVertexArrayPointervEXT);
      SET_MapNamedBufferEXT(table, _mesa_marshal_MapNamedBufferEXT);
      SET_MapNamedBufferRangeEXT(table, _mesa_marshal_MapNamedBufferRangeEXT);
      SET_MatrixLoadTransposedEXT(table, _mesa_marshal_MatrixLoadTransposedEXT);
      SET_MatrixLoadTransposefEXT(table, _mesa_marshal_MatrixLoadTransposefEXT);
      SET_MatrixMultTransposedEXT(table, _mesa_marshal_MatrixMultTransposedEXT);
      SET_MatrixMultTransposefEXT(table, _mesa_marshal_MatrixMultTransposefEXT);
      SET_MatrixPopEXT(table, _mesa_marshal_MatrixPopEXT);
      SET_MultiTexBufferEXT(table, _mesa_marshal_MultiTexBufferEXT);
      SET_MultiTexCoordPointerEXT(table, _mesa_marshal_MultiTexCoordPointerEXT);
      SET_MultiTexEnvfEXT(table, _mesa_marshal_MultiTexEnvfEXT);
      SET_MultiTexEnvfvEXT(table, _mesa_marshal_MultiTexEnvfvEXT);
      SET_MultiTexEnviEXT(table, _mesa_marshal_MultiTexEnviEXT);
      SET_MultiTexEnvivEXT(table, _mesa_marshal_MultiTexEnvivEXT);
      SET_MultiTexGendEXT(table, _mesa_marshal_MultiTexGendEXT);
      SET_MultiTexGendvEXT(table, _mesa_marshal_MultiTexGendvEXT);
      SET_MultiTexGenfEXT(table, _mesa_marshal_MultiTexGenfEXT);
      SET_MultiTexGenfvEXT(table, _mesa_marshal_MultiTexGenfvEXT);
      SET_MultiTexGeniEXT(table, _mesa_marshal_MultiTexGeniEXT);
      SET_MultiTexGenivEXT(table, _mesa_marshal_MultiTexGenivEXT);
      SET_MultiTexImage1DEXT(table, _mesa_marshal_MultiTexImage1DEXT);
      SET_MultiTexImage2DEXT(table, _mesa_marshal_MultiTexImage2DEXT);
      SET_MultiTexImage3DEXT(table, _mesa_marshal_MultiTexImage3DEXT);
      SET_MultiTexParameterIivEXT(table, _mesa_marshal_MultiTexParameterIivEXT);
      SET_MultiTexParameterIuivEXT(table, _mesa_marshal_MultiTexParameterIuivEXT);
      SET_MultiTexParameterfEXT(table, _mesa_marshal_MultiTexParameterfEXT);
      SET_MultiTexParameterfvEXT(table, _mesa_marshal_MultiTexParameterfvEXT);
      SET_MultiTexParameteriEXT(table, _mesa_marshal_MultiTexParameteriEXT);
      SET_MultiTexParameterivEXT(table, _mesa_marshal_MultiTexParameterivEXT);
      SET_MultiTexSubImage1DEXT(table, _mesa_marshal_MultiTexSubImage1DEXT);
      SET_MultiTexSubImage2DEXT(table, _mesa_marshal_MultiTexSubImage2DEXT);
      SET_MultiTexSubImage3DEXT(table, _mesa_marshal_MultiTexSubImage3DEXT);
      SET_NamedCopyBufferSubDataEXT(table, _mesa_marshal_NamedCopyBufferSubDataEXT);
      SET_NamedFramebufferRenderbufferEXT(table, _mesa_marshal_NamedFramebufferRenderbufferEXT);
      SET_NamedFramebufferTexture1DEXT(table, _mesa_marshal_NamedFramebufferTexture1DEXT);
      SET_NamedFramebufferTexture2DEXT(table, _mesa_marshal_NamedFramebufferTexture2DEXT);
      SET_NamedFramebufferTexture3DEXT(table, _mesa_marshal_NamedFramebufferTexture3DEXT);
      SET_NamedProgramLocalParameter4dEXT(table, _mesa_marshal_NamedProgramLocalParameter4dEXT);
      SET_NamedProgramLocalParameter4dvEXT(table, _mesa_marshal_NamedProgramLocalParameter4dvEXT);
      SET_NamedProgramLocalParameter4fEXT(table, _mesa_marshal_NamedProgramLocalParameter4fEXT);
      SET_NamedProgramLocalParameter4fvEXT(table, _mesa_marshal_NamedProgramLocalParameter4fvEXT);
      SET_NamedProgramLocalParameters4fvEXT(table, _mesa_marshal_NamedProgramLocalParameters4fvEXT);
      SET_NamedProgramStringEXT(table, _mesa_marshal_NamedProgramStringEXT);
      SET_NamedRenderbufferStorageEXT(table, _mesa_marshal_NamedRenderbufferStorageEXT);
      SET_NamedRenderbufferStorageMultisampleEXT(table, _mesa_marshal_NamedRenderbufferStorageMultisampleEXT);
      SET_PushClientAttribDefaultEXT(table, _mesa_marshal_PushClientAttribDefaultEXT);
      SET_TextureBufferEXT(table, _mesa_marshal_TextureBufferEXT);
      SET_TextureImage1DEXT(table, _mesa_marshal_TextureImage1DEXT);
      SET_TextureImage2DEXT(table, _mesa_marshal_TextureImage2DEXT);
      SET_TextureImage3DEXT(table, _mesa_marshal_TextureImage3DEXT);
      SET_TextureParameterIivEXT(table, _mesa_marshal_TextureParameterIivEXT);
      SET_TextureParameterIuivEXT(table, _mesa_marshal_TextureParameterIuivEXT);
      SET_TextureParameterfEXT(table, _mesa_marshal_TextureParameterfEXT);
      SET_TextureParameterfvEXT(table, _mesa_marshal_TextureParameterfvEXT);
      SET_TextureParameteriEXT(table, _mesa_marshal_TextureParameteriEXT);
      SET_TextureParameterivEXT(table, _mesa_marshal_TextureParameterivEXT);
      SET_TextureSubImage1DEXT(table, _mesa_marshal_TextureSubImage1DEXT);
      SET_TextureSubImage2DEXT(table, _mesa_marshal_TextureSubImage2DEXT);
      SET_TextureSubImage3DEXT(table, _mesa_marshal_TextureSubImage3DEXT);
      SET_VertexArrayColorOffsetEXT(table, _mesa_marshal_VertexArrayColorOffsetEXT);
      SET_VertexArrayEdgeFlagOffsetEXT(table, _mesa_marshal_VertexArrayEdgeFlagOffsetEXT);
      SET_VertexArrayFogCoordOffsetEXT(table, _mesa_marshal_VertexArrayFogCoordOffsetEXT);
      SET_VertexArrayIndexOffsetEXT(table, _mesa_marshal_VertexArrayIndexOffsetEXT);
      SET_VertexArrayMultiTexCoordOffsetEXT(table, _mesa_marshal_VertexArrayMultiTexCoordOffsetEXT);
      SET_VertexArrayNormalOffsetEXT(table, _mesa_marshal_VertexArrayNormalOffsetEXT);
      SET_VertexArraySecondaryColorOffsetEXT(table, _mesa_marshal_VertexArraySecondaryColorOffsetEXT);
      SET_VertexArrayTexCoordOffsetEXT(table, _mesa_marshal_VertexArrayTexCoordOffsetEXT);
      SET_VertexArrayVertexAttribIOffsetEXT(table, _mesa_marshal_VertexArrayVertexAttribIOffsetEXT);
      SET_VertexArrayVertexAttribOffsetEXT(table, _mesa_marshal_VertexArrayVertexAttribOffsetEXT);
      SET_VertexArrayVertexOffsetEXT(table, _mesa_marshal_VertexArrayVertexOffsetEXT);
      SET_VertexAttribI1iEXT(table, _mesa_marshal_VertexAttribI1iEXT);
      SET_VertexAttribI1iv(table, _mesa_marshal_VertexAttribI1iv);
      SET_VertexAttribI1uiEXT(table, _mesa_marshal_VertexAttribI1uiEXT);
      SET_VertexAttribI1uiv(table, _mesa_marshal_VertexAttribI1uiv);
      SET_VertexAttribI2iEXT(table, _mesa_marshal_VertexAttribI2iEXT);
      SET_VertexAttribI2ivEXT(table, _mesa_marshal_VertexAttribI2ivEXT);
      SET_VertexAttribI2uiEXT(table, _mesa_marshal_VertexAttribI2uiEXT);
      SET_VertexAttribI2uivEXT(table, _mesa_marshal_VertexAttribI2uivEXT);
      SET_VertexAttribI3iEXT(table, _mesa_marshal_VertexAttribI3iEXT);
      SET_VertexAttribI3ivEXT(table, _mesa_marshal_VertexAttribI3ivEXT);
      SET_VertexAttribI3uiEXT(table, _mesa_marshal_VertexAttribI3uiEXT);
      SET_VertexAttribI3uivEXT(table, _mesa_marshal_VertexAttribI3uivEXT);
      SET_VertexAttribI4bv(table, _mesa_marshal_VertexAttribI4bv);
   }
   if (_mesa_is_desktop_gl(ctx) || (ctx->API == API_OPENGLES2 && ctx->Version >= 30)) {
      SET_EGLImageTargetTexStorageEXT(table, _mesa_marshal_EGLImageTargetTexStorageEXT);
      SET_GetTexParameterIiv(table, _mesa_marshal_GetTexParameterIiv);
      SET_GetTexParameterIuiv(table, _mesa_marshal_GetTexParameterIuiv);
      SET_NamedRenderbufferStorageMultisampleAdvancedAMD(table, _mesa_marshal_NamedRenderbufferStorageMultisampleAdvancedAMD);
      SET_RenderbufferStorageMultisampleAdvancedAMD(table, _mesa_marshal_RenderbufferStorageMultisampleAdvancedAMD);
      SET_TexParameterIiv(table, _mesa_marshal_TexParameterIiv);
      SET_TexParameterIuiv(table, _mesa_marshal_TexParameterIuiv);
      SET_VertexAttribI4iEXT(table, _mesa_marshal_VertexAttribI4iEXT);
      SET_VertexAttribI4ivEXT(table, _mesa_marshal_VertexAttribI4ivEXT);
      SET_VertexAttribI4uiEXT(table, _mesa_marshal_VertexAttribI4uiEXT);
      SET_VertexAttribI4uivEXT(table, _mesa_marshal_VertexAttribI4uivEXT);
      SET_WindowRectanglesEXT(table, _mesa_marshal_WindowRectanglesEXT);
   }
   if (_mesa_is_desktop_gl(ctx) || ctx->API == API_OPENGLES || ctx->API == API_OPENGLES2) {
      SET_ConservativeRasterParameterfNV(table, _mesa_marshal_ConservativeRasterParameterfNV);
      SET_ConservativeRasterParameteriNV(table, _mesa_marshal_ConservativeRasterParameteriNV);
      SET_EGLImageTargetRenderbufferStorageOES(table, _mesa_marshal_EGLImageTargetRenderbufferStorageOES);
      SET_EGLImageTargetTexture2DOES(table, _mesa_marshal_EGLImageTargetTexture2DOES);
      SET_GetObjectLabelEXT(table, _mesa_marshal_GetObjectLabelEXT);
      SET_InternalSetError(table, _mesa_marshal_InternalSetError);
      SET_LabelObjectEXT(table, _mesa_marshal_LabelObjectEXT);
      SET_PolygonOffsetClampEXT(table, _mesa_marshal_PolygonOffsetClampEXT);
      SET_SubpixelPrecisionBiasNV(table, _mesa_marshal_SubpixelPrecisionBiasNV);
   }
   if (_mesa_is_desktop_gl(ctx) || ctx->API == API_OPENGLES2) {
      SET_BeginPerfQueryINTEL(table, _mesa_marshal_BeginPerfQueryINTEL);
      SET_CreatePerfQueryINTEL(table, _mesa_marshal_CreatePerfQueryINTEL);
      SET_DeletePerfQueryINTEL(table, _mesa_marshal_DeletePerfQueryINTEL);
      SET_EndPerfQueryINTEL(table, _mesa_marshal_EndPerfQueryINTEL);
      SET_FramebufferFetchBarrierEXT(table, _mesa_marshal_FramebufferFetchBarrierEXT);
      SET_GetFirstPerfQueryIdINTEL(table, _mesa_marshal_GetFirstPerfQueryIdINTEL);
      SET_GetNextPerfQueryIdINTEL(table, _mesa_marshal_GetNextPerfQueryIdINTEL);
      SET_GetPerfCounterInfoINTEL(table, _mesa_marshal_GetPerfCounterInfoINTEL);
      SET_GetPerfQueryDataINTEL(table, _mesa_marshal_GetPerfQueryDataINTEL);
      SET_GetPerfQueryIdByNameINTEL(table, _mesa_marshal_GetPerfQueryIdByNameINTEL);
      SET_GetPerfQueryInfoINTEL(table, _mesa_marshal_GetPerfQueryInfoINTEL);
      SET_InternalBufferSubDataCopyMESA(table, _mesa_marshal_InternalBufferSubDataCopyMESA);
   }
   if (ctx->API == API_OPENGL_COMPAT) {
      SET_ProgramEnvParameters4fvEXT(table, _mesa_marshal_ProgramEnvParameters4fvEXT);
      SET_ProgramLocalParameters4fvEXT(table, _mesa_marshal_ProgramLocalParameters4fvEXT);
      SET_StencilFuncSeparateATI(table, _mesa_marshal_StencilFuncSeparateATI);
   }
}
