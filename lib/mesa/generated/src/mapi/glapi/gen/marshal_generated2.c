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

/* GetPixelMapfv: marshalled asynchronously */
struct marshal_cmd_GetPixelMapfv
{
   struct marshal_cmd_base cmd_base;
   GLenum map;
   GLfloat * values;
};
uint32_t
_mesa_unmarshal_GetPixelMapfv(struct gl_context *ctx, const struct marshal_cmd_GetPixelMapfv *cmd, const uint64_t *last)
{
   GLenum map = cmd->map;
   GLfloat * values = cmd->values;
   CALL_GetPixelMapfv(ctx->CurrentServerDispatch, (map, values));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_GetPixelMapfv), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_GetPixelMapfv(GLenum map, GLfloat * values)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_GetPixelMapfv);
   struct marshal_cmd_GetPixelMapfv *cmd;
   if (_mesa_glthread_has_no_pack_buffer(ctx)) {
      _mesa_glthread_finish_before(ctx, "GetPixelMapfv");
      CALL_GetPixelMapfv(ctx->CurrentServerDispatch, (map, values));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_GetPixelMapfv, cmd_size);
   cmd->map = map;
   cmd->values = values;
}


/* GetPixelMapuiv: marshalled asynchronously */
struct marshal_cmd_GetPixelMapuiv
{
   struct marshal_cmd_base cmd_base;
   GLenum map;
   GLuint * values;
};
uint32_t
_mesa_unmarshal_GetPixelMapuiv(struct gl_context *ctx, const struct marshal_cmd_GetPixelMapuiv *cmd, const uint64_t *last)
{
   GLenum map = cmd->map;
   GLuint * values = cmd->values;
   CALL_GetPixelMapuiv(ctx->CurrentServerDispatch, (map, values));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_GetPixelMapuiv), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_GetPixelMapuiv(GLenum map, GLuint * values)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_GetPixelMapuiv);
   struct marshal_cmd_GetPixelMapuiv *cmd;
   if (_mesa_glthread_has_no_pack_buffer(ctx)) {
      _mesa_glthread_finish_before(ctx, "GetPixelMapuiv");
      CALL_GetPixelMapuiv(ctx->CurrentServerDispatch, (map, values));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_GetPixelMapuiv, cmd_size);
   cmd->map = map;
   cmd->values = values;
}


/* GetPixelMapusv: marshalled asynchronously */
struct marshal_cmd_GetPixelMapusv
{
   struct marshal_cmd_base cmd_base;
   GLenum map;
   GLushort * values;
};
uint32_t
_mesa_unmarshal_GetPixelMapusv(struct gl_context *ctx, const struct marshal_cmd_GetPixelMapusv *cmd, const uint64_t *last)
{
   GLenum map = cmd->map;
   GLushort * values = cmd->values;
   CALL_GetPixelMapusv(ctx->CurrentServerDispatch, (map, values));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_GetPixelMapusv), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_GetPixelMapusv(GLenum map, GLushort * values)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_GetPixelMapusv);
   struct marshal_cmd_GetPixelMapusv *cmd;
   if (_mesa_glthread_has_no_pack_buffer(ctx)) {
      _mesa_glthread_finish_before(ctx, "GetPixelMapusv");
      CALL_GetPixelMapusv(ctx->CurrentServerDispatch, (map, values));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_GetPixelMapusv, cmd_size);
   cmd->map = map;
   cmd->values = values;
}


/* GetPolygonStipple: marshalled asynchronously */
struct marshal_cmd_GetPolygonStipple
{
   struct marshal_cmd_base cmd_base;
   GLubyte * mask;
};
uint32_t
_mesa_unmarshal_GetPolygonStipple(struct gl_context *ctx, const struct marshal_cmd_GetPolygonStipple *cmd, const uint64_t *last)
{
   GLubyte * mask = cmd->mask;
   CALL_GetPolygonStipple(ctx->CurrentServerDispatch, (mask));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_GetPolygonStipple), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_GetPolygonStipple(GLubyte * mask)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_GetPolygonStipple);
   struct marshal_cmd_GetPolygonStipple *cmd;
   if (_mesa_glthread_has_no_pack_buffer(ctx)) {
      _mesa_glthread_finish_before(ctx, "GetPolygonStipple");
      CALL_GetPolygonStipple(ctx->CurrentServerDispatch, (mask));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_GetPolygonStipple, cmd_size);
   cmd->mask = mask;
}


/* GetString: marshalled synchronously */
const GLubyte * GLAPIENTRY
_mesa_marshal_GetString(GLenum name)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetString");
   return CALL_GetString(ctx->CurrentServerDispatch, (name));
}


/* GetTexEnvfv: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetTexEnvfv(GLenum target, GLenum pname, GLfloat * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetTexEnvfv");
   CALL_GetTexEnvfv(ctx->CurrentServerDispatch, (target, pname, params));
}


/* GetTexEnviv: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetTexEnviv(GLenum target, GLenum pname, GLint * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetTexEnviv");
   CALL_GetTexEnviv(ctx->CurrentServerDispatch, (target, pname, params));
}


/* GetTexGendv: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetTexGendv(GLenum coord, GLenum pname, GLdouble * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetTexGendv");
   CALL_GetTexGendv(ctx->CurrentServerDispatch, (coord, pname, params));
}


/* GetTexGenfv: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetTexGenfv(GLenum coord, GLenum pname, GLfloat * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetTexGenfv");
   CALL_GetTexGenfv(ctx->CurrentServerDispatch, (coord, pname, params));
}


/* GetTexGeniv: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetTexGeniv(GLenum coord, GLenum pname, GLint * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetTexGeniv");
   CALL_GetTexGeniv(ctx->CurrentServerDispatch, (coord, pname, params));
}


/* GetTexImage: marshalled asynchronously */
struct marshal_cmd_GetTexImage
{
   struct marshal_cmd_base cmd_base;
   GLenum target;
   GLint level;
   GLenum format;
   GLenum type;
   GLvoid * pixels;
};
uint32_t
_mesa_unmarshal_GetTexImage(struct gl_context *ctx, const struct marshal_cmd_GetTexImage *cmd, const uint64_t *last)
{
   GLenum target = cmd->target;
   GLint level = cmd->level;
   GLenum format = cmd->format;
   GLenum type = cmd->type;
   GLvoid * pixels = cmd->pixels;
   CALL_GetTexImage(ctx->CurrentServerDispatch, (target, level, format, type, pixels));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_GetTexImage), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_GetTexImage(GLenum target, GLint level, GLenum format, GLenum type, GLvoid * pixels)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_GetTexImage);
   struct marshal_cmd_GetTexImage *cmd;
   if (_mesa_glthread_has_no_pack_buffer(ctx)) {
      _mesa_glthread_finish_before(ctx, "GetTexImage");
      CALL_GetTexImage(ctx->CurrentServerDispatch, (target, level, format, type, pixels));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_GetTexImage, cmd_size);
   cmd->target = target;
   cmd->level = level;
   cmd->format = format;
   cmd->type = type;
   cmd->pixels = pixels;
}


/* GetTexParameterfv: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetTexParameterfv(GLenum target, GLenum pname, GLfloat * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetTexParameterfv");
   CALL_GetTexParameterfv(ctx->CurrentServerDispatch, (target, pname, params));
}


/* GetTexParameteriv: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetTexParameteriv(GLenum target, GLenum pname, GLint * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetTexParameteriv");
   CALL_GetTexParameteriv(ctx->CurrentServerDispatch, (target, pname, params));
}


/* GetTexLevelParameterfv: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetTexLevelParameterfv(GLenum target, GLint level, GLenum pname, GLfloat * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetTexLevelParameterfv");
   CALL_GetTexLevelParameterfv(ctx->CurrentServerDispatch, (target, level, pname, params));
}


/* GetTexLevelParameteriv: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetTexLevelParameteriv(GLenum target, GLint level, GLenum pname, GLint * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetTexLevelParameteriv");
   CALL_GetTexLevelParameteriv(ctx->CurrentServerDispatch, (target, level, pname, params));
}


/* IsEnabled: marshalled synchronously */
GLboolean GLAPIENTRY
_mesa_marshal_IsEnabled(GLenum cap)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "IsEnabled");
   return CALL_IsEnabled(ctx->CurrentServerDispatch, (cap));
}


/* IsList: marshalled synchronously */
GLboolean GLAPIENTRY
_mesa_marshal_IsList(GLuint list)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "IsList");
   return CALL_IsList(ctx->CurrentServerDispatch, (list));
}


/* DepthRange: marshalled asynchronously */
struct marshal_cmd_DepthRange
{
   struct marshal_cmd_base cmd_base;
   GLclampd zNear;
   GLclampd zFar;
};
uint32_t
_mesa_unmarshal_DepthRange(struct gl_context *ctx, const struct marshal_cmd_DepthRange *cmd, const uint64_t *last)
{
   GLclampd zNear = cmd->zNear;
   GLclampd zFar = cmd->zFar;
   CALL_DepthRange(ctx->CurrentServerDispatch, (zNear, zFar));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_DepthRange), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_DepthRange(GLclampd zNear, GLclampd zFar)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_DepthRange);
   struct marshal_cmd_DepthRange *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_DepthRange, cmd_size);
   cmd->zNear = zNear;
   cmd->zFar = zFar;
}


/* Frustum: marshalled asynchronously */
struct marshal_cmd_Frustum
{
   struct marshal_cmd_base cmd_base;
   GLdouble left;
   GLdouble right;
   GLdouble bottom;
   GLdouble top;
   GLdouble zNear;
   GLdouble zFar;
};
uint32_t
_mesa_unmarshal_Frustum(struct gl_context *ctx, const struct marshal_cmd_Frustum *cmd, const uint64_t *last)
{
   GLdouble left = cmd->left;
   GLdouble right = cmd->right;
   GLdouble bottom = cmd->bottom;
   GLdouble top = cmd->top;
   GLdouble zNear = cmd->zNear;
   GLdouble zFar = cmd->zFar;
   CALL_Frustum(ctx->CurrentServerDispatch, (left, right, bottom, top, zNear, zFar));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Frustum), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_Frustum(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Frustum);
   struct marshal_cmd_Frustum *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Frustum, cmd_size);
   cmd->left = left;
   cmd->right = right;
   cmd->bottom = bottom;
   cmd->top = top;
   cmd->zNear = zNear;
   cmd->zFar = zFar;
}


/* LoadIdentity: marshalled asynchronously */
struct marshal_cmd_LoadIdentity
{
   struct marshal_cmd_base cmd_base;
};
uint32_t
_mesa_unmarshal_LoadIdentity(struct gl_context *ctx, const struct marshal_cmd_LoadIdentity *cmd, const uint64_t *last)
{
   CALL_LoadIdentity(ctx->CurrentServerDispatch, ());
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_LoadIdentity), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_LoadIdentity(void)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_LoadIdentity);
   struct marshal_cmd_LoadIdentity *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_LoadIdentity, cmd_size);
   (void) cmd;
}


/* LoadMatrixf: marshalled asynchronously */
struct marshal_cmd_LoadMatrixf
{
   struct marshal_cmd_base cmd_base;
   GLfloat m[16];
};
uint32_t
_mesa_unmarshal_LoadMatrixf(struct gl_context *ctx, const struct marshal_cmd_LoadMatrixf *cmd, const uint64_t *last)
{
   const GLfloat * m = cmd->m;
   CALL_LoadMatrixf(ctx->CurrentServerDispatch, (m));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_LoadMatrixf), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_LoadMatrixf(const GLfloat * m)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_LoadMatrixf);
   struct marshal_cmd_LoadMatrixf *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_LoadMatrixf, cmd_size);
   memcpy(cmd->m, m, 16 * sizeof(GLfloat));
}


/* LoadMatrixd: marshalled asynchronously */
struct marshal_cmd_LoadMatrixd
{
   struct marshal_cmd_base cmd_base;
   GLdouble m[16];
};
uint32_t
_mesa_unmarshal_LoadMatrixd(struct gl_context *ctx, const struct marshal_cmd_LoadMatrixd *cmd, const uint64_t *last)
{
   const GLdouble * m = cmd->m;
   CALL_LoadMatrixd(ctx->CurrentServerDispatch, (m));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_LoadMatrixd), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_LoadMatrixd(const GLdouble * m)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_LoadMatrixd);
   struct marshal_cmd_LoadMatrixd *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_LoadMatrixd, cmd_size);
   memcpy(cmd->m, m, 16 * sizeof(GLdouble));
}


/* MatrixMode: marshalled asynchronously */
struct marshal_cmd_MatrixMode
{
   struct marshal_cmd_base cmd_base;
   GLenum mode;
};
uint32_t
_mesa_unmarshal_MatrixMode(struct gl_context *ctx, const struct marshal_cmd_MatrixMode *cmd, const uint64_t *last)
{
   GLenum mode = cmd->mode;
   CALL_MatrixMode(ctx->CurrentServerDispatch, (mode));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MatrixMode), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_MatrixMode(GLenum mode)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MatrixMode);
   struct marshal_cmd_MatrixMode *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MatrixMode, cmd_size);
   cmd->mode = mode;
   _mesa_glthread_MatrixMode(ctx, mode);
}


/* MultMatrixf: marshalled asynchronously */
struct marshal_cmd_MultMatrixf
{
   struct marshal_cmd_base cmd_base;
   GLfloat m[16];
};
uint32_t
_mesa_unmarshal_MultMatrixf(struct gl_context *ctx, const struct marshal_cmd_MultMatrixf *cmd, const uint64_t *last)
{
   const GLfloat * m = cmd->m;
   CALL_MultMatrixf(ctx->CurrentServerDispatch, (m));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MultMatrixf), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_MultMatrixf(const GLfloat * m)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultMatrixf);
   struct marshal_cmd_MultMatrixf *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultMatrixf, cmd_size);
   memcpy(cmd->m, m, 16 * sizeof(GLfloat));
}


/* MultMatrixd: marshalled asynchronously */
struct marshal_cmd_MultMatrixd
{
   struct marshal_cmd_base cmd_base;
   GLdouble m[16];
};
uint32_t
_mesa_unmarshal_MultMatrixd(struct gl_context *ctx, const struct marshal_cmd_MultMatrixd *cmd, const uint64_t *last)
{
   const GLdouble * m = cmd->m;
   CALL_MultMatrixd(ctx->CurrentServerDispatch, (m));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MultMatrixd), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_MultMatrixd(const GLdouble * m)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultMatrixd);
   struct marshal_cmd_MultMatrixd *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultMatrixd, cmd_size);
   memcpy(cmd->m, m, 16 * sizeof(GLdouble));
}


/* Ortho: marshalled asynchronously */
struct marshal_cmd_Ortho
{
   struct marshal_cmd_base cmd_base;
   GLdouble left;
   GLdouble right;
   GLdouble bottom;
   GLdouble top;
   GLdouble zNear;
   GLdouble zFar;
};
uint32_t
_mesa_unmarshal_Ortho(struct gl_context *ctx, const struct marshal_cmd_Ortho *cmd, const uint64_t *last)
{
   GLdouble left = cmd->left;
   GLdouble right = cmd->right;
   GLdouble bottom = cmd->bottom;
   GLdouble top = cmd->top;
   GLdouble zNear = cmd->zNear;
   GLdouble zFar = cmd->zFar;
   CALL_Ortho(ctx->CurrentServerDispatch, (left, right, bottom, top, zNear, zFar));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Ortho), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_Ortho(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Ortho);
   struct marshal_cmd_Ortho *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Ortho, cmd_size);
   cmd->left = left;
   cmd->right = right;
   cmd->bottom = bottom;
   cmd->top = top;
   cmd->zNear = zNear;
   cmd->zFar = zFar;
}


/* PopMatrix: marshalled asynchronously */
struct marshal_cmd_PopMatrix
{
   struct marshal_cmd_base cmd_base;
};
uint32_t
_mesa_unmarshal_PopMatrix(struct gl_context *ctx, const struct marshal_cmd_PopMatrix *cmd, const uint64_t *last)
{
   CALL_PopMatrix(ctx->CurrentServerDispatch, ());
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_PopMatrix), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_PopMatrix(void)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_PopMatrix);
   struct marshal_cmd_PopMatrix *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_PopMatrix, cmd_size);
   (void) cmd;
   _mesa_glthread_PopMatrix(ctx);
}


/* PushMatrix: marshalled asynchronously */
struct marshal_cmd_PushMatrix
{
   struct marshal_cmd_base cmd_base;
};
uint32_t
_mesa_unmarshal_PushMatrix(struct gl_context *ctx, const struct marshal_cmd_PushMatrix *cmd, const uint64_t *last)
{
   CALL_PushMatrix(ctx->CurrentServerDispatch, ());
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_PushMatrix), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_PushMatrix(void)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_PushMatrix);
   struct marshal_cmd_PushMatrix *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_PushMatrix, cmd_size);
   (void) cmd;
   _mesa_glthread_PushMatrix(ctx);
}


/* Rotated: marshalled asynchronously */
struct marshal_cmd_Rotated
{
   struct marshal_cmd_base cmd_base;
   GLdouble angle;
   GLdouble x;
   GLdouble y;
   GLdouble z;
};
uint32_t
_mesa_unmarshal_Rotated(struct gl_context *ctx, const struct marshal_cmd_Rotated *cmd, const uint64_t *last)
{
   GLdouble angle = cmd->angle;
   GLdouble x = cmd->x;
   GLdouble y = cmd->y;
   GLdouble z = cmd->z;
   CALL_Rotated(ctx->CurrentServerDispatch, (angle, x, y, z));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Rotated), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_Rotated(GLdouble angle, GLdouble x, GLdouble y, GLdouble z)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Rotated);
   struct marshal_cmd_Rotated *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Rotated, cmd_size);
   cmd->angle = angle;
   cmd->x = x;
   cmd->y = y;
   cmd->z = z;
}


/* Rotatef: marshalled asynchronously */
struct marshal_cmd_Rotatef
{
   struct marshal_cmd_base cmd_base;
   GLfloat angle;
   GLfloat x;
   GLfloat y;
   GLfloat z;
};
uint32_t
_mesa_unmarshal_Rotatef(struct gl_context *ctx, const struct marshal_cmd_Rotatef *cmd, const uint64_t *last)
{
   GLfloat angle = cmd->angle;
   GLfloat x = cmd->x;
   GLfloat y = cmd->y;
   GLfloat z = cmd->z;
   CALL_Rotatef(ctx->CurrentServerDispatch, (angle, x, y, z));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Rotatef), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_Rotatef(GLfloat angle, GLfloat x, GLfloat y, GLfloat z)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Rotatef);
   struct marshal_cmd_Rotatef *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Rotatef, cmd_size);
   cmd->angle = angle;
   cmd->x = x;
   cmd->y = y;
   cmd->z = z;
}


/* Scaled: marshalled asynchronously */
struct marshal_cmd_Scaled
{
   struct marshal_cmd_base cmd_base;
   GLdouble x;
   GLdouble y;
   GLdouble z;
};
uint32_t
_mesa_unmarshal_Scaled(struct gl_context *ctx, const struct marshal_cmd_Scaled *cmd, const uint64_t *last)
{
   GLdouble x = cmd->x;
   GLdouble y = cmd->y;
   GLdouble z = cmd->z;
   CALL_Scaled(ctx->CurrentServerDispatch, (x, y, z));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Scaled), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_Scaled(GLdouble x, GLdouble y, GLdouble z)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Scaled);
   struct marshal_cmd_Scaled *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Scaled, cmd_size);
   cmd->x = x;
   cmd->y = y;
   cmd->z = z;
}


/* Scalef: marshalled asynchronously */
struct marshal_cmd_Scalef
{
   struct marshal_cmd_base cmd_base;
   GLfloat x;
   GLfloat y;
   GLfloat z;
};
uint32_t
_mesa_unmarshal_Scalef(struct gl_context *ctx, const struct marshal_cmd_Scalef *cmd, const uint64_t *last)
{
   GLfloat x = cmd->x;
   GLfloat y = cmd->y;
   GLfloat z = cmd->z;
   CALL_Scalef(ctx->CurrentServerDispatch, (x, y, z));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Scalef), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_Scalef(GLfloat x, GLfloat y, GLfloat z)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Scalef);
   struct marshal_cmd_Scalef *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Scalef, cmd_size);
   cmd->x = x;
   cmd->y = y;
   cmd->z = z;
}


/* Translated: marshalled asynchronously */
struct marshal_cmd_Translated
{
   struct marshal_cmd_base cmd_base;
   GLdouble x;
   GLdouble y;
   GLdouble z;
};
uint32_t
_mesa_unmarshal_Translated(struct gl_context *ctx, const struct marshal_cmd_Translated *cmd, const uint64_t *last)
{
   GLdouble x = cmd->x;
   GLdouble y = cmd->y;
   GLdouble z = cmd->z;
   CALL_Translated(ctx->CurrentServerDispatch, (x, y, z));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Translated), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_Translated(GLdouble x, GLdouble y, GLdouble z)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Translated);
   struct marshal_cmd_Translated *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Translated, cmd_size);
   cmd->x = x;
   cmd->y = y;
   cmd->z = z;
}


/* Translatef: marshalled asynchronously */
struct marshal_cmd_Translatef
{
   struct marshal_cmd_base cmd_base;
   GLfloat x;
   GLfloat y;
   GLfloat z;
};
uint32_t
_mesa_unmarshal_Translatef(struct gl_context *ctx, const struct marshal_cmd_Translatef *cmd, const uint64_t *last)
{
   GLfloat x = cmd->x;
   GLfloat y = cmd->y;
   GLfloat z = cmd->z;
   CALL_Translatef(ctx->CurrentServerDispatch, (x, y, z));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Translatef), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_Translatef(GLfloat x, GLfloat y, GLfloat z)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Translatef);
   struct marshal_cmd_Translatef *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Translatef, cmd_size);
   cmd->x = x;
   cmd->y = y;
   cmd->z = z;
}


/* Viewport: marshalled asynchronously */
struct marshal_cmd_Viewport
{
   struct marshal_cmd_base cmd_base;
   GLint x;
   GLint y;
   GLsizei width;
   GLsizei height;
};
uint32_t
_mesa_unmarshal_Viewport(struct gl_context *ctx, const struct marshal_cmd_Viewport *cmd, const uint64_t *last)
{
   GLint x = cmd->x;
   GLint y = cmd->y;
   GLsizei width = cmd->width;
   GLsizei height = cmd->height;
   CALL_Viewport(ctx->CurrentServerDispatch, (x, y, width, height));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Viewport), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_Viewport(GLint x, GLint y, GLsizei width, GLsizei height)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Viewport);
   struct marshal_cmd_Viewport *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Viewport, cmd_size);
   cmd->x = x;
   cmd->y = y;
   cmd->width = width;
   cmd->height = height;
}


/* ArrayElement: marshalled asynchronously */
struct marshal_cmd_ArrayElement
{
   struct marshal_cmd_base cmd_base;
   GLint i;
};
uint32_t
_mesa_unmarshal_ArrayElement(struct gl_context *ctx, const struct marshal_cmd_ArrayElement *cmd, const uint64_t *last)
{
   GLint i = cmd->i;
   CALL_ArrayElement(ctx->CurrentServerDispatch, (i));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_ArrayElement), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_ArrayElement(GLint i)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ArrayElement);
   struct marshal_cmd_ArrayElement *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ArrayElement, cmd_size);
   cmd->i = i;
}


/* ColorPointer: marshalled asynchronously */
struct marshal_cmd_ColorPointer
{
   struct marshal_cmd_base cmd_base;
   GLint size;
   GLenum type;
   GLsizei stride;
   const GLvoid * pointer;
};
uint32_t
_mesa_unmarshal_ColorPointer(struct gl_context *ctx, const struct marshal_cmd_ColorPointer *cmd, const uint64_t *last)
{
   GLint size = cmd->size;
   GLenum type = cmd->type;
   GLsizei stride = cmd->stride;
   const GLvoid * pointer = cmd->pointer;
   CALL_ColorPointer(ctx->CurrentServerDispatch, (size, type, stride, pointer));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_ColorPointer), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_ColorPointer(GLint size, GLenum type, GLsizei stride, const GLvoid * pointer)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ColorPointer);
   struct marshal_cmd_ColorPointer *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ColorPointer, cmd_size);
   cmd->size = size;
   cmd->type = type;
   cmd->stride = stride;
   cmd->pointer = pointer;
   if (COMPAT) _mesa_glthread_AttribPointer(ctx, VERT_ATTRIB_COLOR0, size, type, stride, pointer);
}


/* DisableClientState: marshalled asynchronously */
struct marshal_cmd_DisableClientState
{
   struct marshal_cmd_base cmd_base;
   GLenum array;
};
uint32_t
_mesa_unmarshal_DisableClientState(struct gl_context *ctx, const struct marshal_cmd_DisableClientState *cmd, const uint64_t *last)
{
   GLenum array = cmd->array;
   CALL_DisableClientState(ctx->CurrentServerDispatch, (array));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_DisableClientState), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_DisableClientState(GLenum array)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_DisableClientState);
   struct marshal_cmd_DisableClientState *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_DisableClientState, cmd_size);
   cmd->array = array;
   if (COMPAT) _mesa_glthread_ClientState(ctx, NULL, _mesa_array_to_attrib(ctx, array), false);
}


/* EdgeFlagPointer: marshalled asynchronously */
struct marshal_cmd_EdgeFlagPointer
{
   struct marshal_cmd_base cmd_base;
   GLsizei stride;
   const GLvoid * pointer;
};
uint32_t
_mesa_unmarshal_EdgeFlagPointer(struct gl_context *ctx, const struct marshal_cmd_EdgeFlagPointer *cmd, const uint64_t *last)
{
   GLsizei stride = cmd->stride;
   const GLvoid * pointer = cmd->pointer;
   CALL_EdgeFlagPointer(ctx->CurrentServerDispatch, (stride, pointer));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_EdgeFlagPointer), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_EdgeFlagPointer(GLsizei stride, const GLvoid * pointer)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_EdgeFlagPointer);
   struct marshal_cmd_EdgeFlagPointer *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_EdgeFlagPointer, cmd_size);
   cmd->stride = stride;
   cmd->pointer = pointer;
   if (COMPAT) _mesa_glthread_AttribPointer(ctx, VERT_ATTRIB_EDGEFLAG, 1, GL_UNSIGNED_BYTE, stride, pointer);
}


/* EnableClientState: marshalled asynchronously */
struct marshal_cmd_EnableClientState
{
   struct marshal_cmd_base cmd_base;
   GLenum array;
};
uint32_t
_mesa_unmarshal_EnableClientState(struct gl_context *ctx, const struct marshal_cmd_EnableClientState *cmd, const uint64_t *last)
{
   GLenum array = cmd->array;
   CALL_EnableClientState(ctx->CurrentServerDispatch, (array));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_EnableClientState), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_EnableClientState(GLenum array)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_EnableClientState);
   struct marshal_cmd_EnableClientState *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_EnableClientState, cmd_size);
   cmd->array = array;
   if (COMPAT) _mesa_glthread_ClientState(ctx, NULL, _mesa_array_to_attrib(ctx, array), true);
}


/* GetPointerv: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetPointerv(GLenum pname, GLvoid ** params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetPointerv");
   CALL_GetPointerv(ctx->CurrentServerDispatch, (pname, params));
}


/* IndexPointer: marshalled asynchronously */
struct marshal_cmd_IndexPointer
{
   struct marshal_cmd_base cmd_base;
   GLenum type;
   GLsizei stride;
   const GLvoid * pointer;
};
uint32_t
_mesa_unmarshal_IndexPointer(struct gl_context *ctx, const struct marshal_cmd_IndexPointer *cmd, const uint64_t *last)
{
   GLenum type = cmd->type;
   GLsizei stride = cmd->stride;
   const GLvoid * pointer = cmd->pointer;
   CALL_IndexPointer(ctx->CurrentServerDispatch, (type, stride, pointer));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_IndexPointer), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_IndexPointer(GLenum type, GLsizei stride, const GLvoid * pointer)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_IndexPointer);
   struct marshal_cmd_IndexPointer *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_IndexPointer, cmd_size);
   cmd->type = type;
   cmd->stride = stride;
   cmd->pointer = pointer;
   if (COMPAT) _mesa_glthread_AttribPointer(ctx, VERT_ATTRIB_COLOR_INDEX, 1, type, stride, pointer);
}


/* InterleavedArrays: marshalled asynchronously */
struct marshal_cmd_InterleavedArrays
{
   struct marshal_cmd_base cmd_base;
   GLenum format;
   GLsizei stride;
   const GLvoid * pointer;
};
uint32_t
_mesa_unmarshal_InterleavedArrays(struct gl_context *ctx, const struct marshal_cmd_InterleavedArrays *cmd, const uint64_t *last)
{
   GLenum format = cmd->format;
   GLsizei stride = cmd->stride;
   const GLvoid * pointer = cmd->pointer;
   CALL_InterleavedArrays(ctx->CurrentServerDispatch, (format, stride, pointer));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_InterleavedArrays), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_InterleavedArrays(GLenum format, GLsizei stride, const GLvoid * pointer)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_InterleavedArrays);
   struct marshal_cmd_InterleavedArrays *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_InterleavedArrays, cmd_size);
   cmd->format = format;
   cmd->stride = stride;
   cmd->pointer = pointer;
   _mesa_glthread_InterleavedArrays(ctx, format, stride, pointer);
}


/* NormalPointer: marshalled asynchronously */
struct marshal_cmd_NormalPointer
{
   struct marshal_cmd_base cmd_base;
   GLenum type;
   GLsizei stride;
   const GLvoid * pointer;
};
uint32_t
_mesa_unmarshal_NormalPointer(struct gl_context *ctx, const struct marshal_cmd_NormalPointer *cmd, const uint64_t *last)
{
   GLenum type = cmd->type;
   GLsizei stride = cmd->stride;
   const GLvoid * pointer = cmd->pointer;
   CALL_NormalPointer(ctx->CurrentServerDispatch, (type, stride, pointer));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_NormalPointer), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_NormalPointer(GLenum type, GLsizei stride, const GLvoid * pointer)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_NormalPointer);
   struct marshal_cmd_NormalPointer *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_NormalPointer, cmd_size);
   cmd->type = type;
   cmd->stride = stride;
   cmd->pointer = pointer;
   if (COMPAT) _mesa_glthread_AttribPointer(ctx, VERT_ATTRIB_NORMAL, 3, type, stride, pointer);
}


/* TexCoordPointer: marshalled asynchronously */
struct marshal_cmd_TexCoordPointer
{
   struct marshal_cmd_base cmd_base;
   GLint size;
   GLenum type;
   GLsizei stride;
   const GLvoid * pointer;
};
uint32_t
_mesa_unmarshal_TexCoordPointer(struct gl_context *ctx, const struct marshal_cmd_TexCoordPointer *cmd, const uint64_t *last)
{
   GLint size = cmd->size;
   GLenum type = cmd->type;
   GLsizei stride = cmd->stride;
   const GLvoid * pointer = cmd->pointer;
   CALL_TexCoordPointer(ctx->CurrentServerDispatch, (size, type, stride, pointer));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_TexCoordPointer), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_TexCoordPointer(GLint size, GLenum type, GLsizei stride, const GLvoid * pointer)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TexCoordPointer);
   struct marshal_cmd_TexCoordPointer *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexCoordPointer, cmd_size);
   cmd->size = size;
   cmd->type = type;
   cmd->stride = stride;
   cmd->pointer = pointer;
   if (COMPAT) _mesa_glthread_AttribPointer(ctx, VERT_ATTRIB_TEX(ctx->GLThread.ClientActiveTexture), size, type, stride, pointer);
}


/* VertexPointer: marshalled asynchronously */
struct marshal_cmd_VertexPointer
{
   struct marshal_cmd_base cmd_base;
   GLint size;
   GLenum type;
   GLsizei stride;
   const GLvoid * pointer;
};
uint32_t
_mesa_unmarshal_VertexPointer(struct gl_context *ctx, const struct marshal_cmd_VertexPointer *cmd, const uint64_t *last)
{
   GLint size = cmd->size;
   GLenum type = cmd->type;
   GLsizei stride = cmd->stride;
   const GLvoid * pointer = cmd->pointer;
   CALL_VertexPointer(ctx->CurrentServerDispatch, (size, type, stride, pointer));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexPointer), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_VertexPointer(GLint size, GLenum type, GLsizei stride, const GLvoid * pointer)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexPointer);
   struct marshal_cmd_VertexPointer *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexPointer, cmd_size);
   cmd->size = size;
   cmd->type = type;
   cmd->stride = stride;
   cmd->pointer = pointer;
   if (COMPAT) _mesa_glthread_AttribPointer(ctx, VERT_ATTRIB_POS, size, type, stride, pointer);
}


/* PolygonOffset: marshalled asynchronously */
struct marshal_cmd_PolygonOffset
{
   struct marshal_cmd_base cmd_base;
   GLfloat factor;
   GLfloat units;
};
uint32_t
_mesa_unmarshal_PolygonOffset(struct gl_context *ctx, const struct marshal_cmd_PolygonOffset *cmd, const uint64_t *last)
{
   GLfloat factor = cmd->factor;
   GLfloat units = cmd->units;
   CALL_PolygonOffset(ctx->CurrentServerDispatch, (factor, units));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_PolygonOffset), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_PolygonOffset(GLfloat factor, GLfloat units)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_PolygonOffset);
   struct marshal_cmd_PolygonOffset *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_PolygonOffset, cmd_size);
   cmd->factor = factor;
   cmd->units = units;
}


/* CopyTexImage1D: marshalled asynchronously */
struct marshal_cmd_CopyTexImage1D
{
   struct marshal_cmd_base cmd_base;
   GLenum target;
   GLint level;
   GLenum internalformat;
   GLint x;
   GLint y;
   GLsizei width;
   GLint border;
};
uint32_t
_mesa_unmarshal_CopyTexImage1D(struct gl_context *ctx, const struct marshal_cmd_CopyTexImage1D *cmd, const uint64_t *last)
{
   GLenum target = cmd->target;
   GLint level = cmd->level;
   GLenum internalformat = cmd->internalformat;
   GLint x = cmd->x;
   GLint y = cmd->y;
   GLsizei width = cmd->width;
   GLint border = cmd->border;
   CALL_CopyTexImage1D(ctx->CurrentServerDispatch, (target, level, internalformat, x, y, width, border));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_CopyTexImage1D), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_CopyTexImage1D(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLint border)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_CopyTexImage1D);
   struct marshal_cmd_CopyTexImage1D *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_CopyTexImage1D, cmd_size);
   cmd->target = target;
   cmd->level = level;
   cmd->internalformat = internalformat;
   cmd->x = x;
   cmd->y = y;
   cmd->width = width;
   cmd->border = border;
}


/* CopyTexImage2D: marshalled asynchronously */
struct marshal_cmd_CopyTexImage2D
{
   struct marshal_cmd_base cmd_base;
   GLenum target;
   GLint level;
   GLenum internalformat;
   GLint x;
   GLint y;
   GLsizei width;
   GLsizei height;
   GLint border;
};
uint32_t
_mesa_unmarshal_CopyTexImage2D(struct gl_context *ctx, const struct marshal_cmd_CopyTexImage2D *cmd, const uint64_t *last)
{
   GLenum target = cmd->target;
   GLint level = cmd->level;
   GLenum internalformat = cmd->internalformat;
   GLint x = cmd->x;
   GLint y = cmd->y;
   GLsizei width = cmd->width;
   GLsizei height = cmd->height;
   GLint border = cmd->border;
   CALL_CopyTexImage2D(ctx->CurrentServerDispatch, (target, level, internalformat, x, y, width, height, border));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_CopyTexImage2D), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_CopyTexImage2D(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_CopyTexImage2D);
   struct marshal_cmd_CopyTexImage2D *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_CopyTexImage2D, cmd_size);
   cmd->target = target;
   cmd->level = level;
   cmd->internalformat = internalformat;
   cmd->x = x;
   cmd->y = y;
   cmd->width = width;
   cmd->height = height;
   cmd->border = border;
}


/* CopyTexSubImage1D: marshalled asynchronously */
struct marshal_cmd_CopyTexSubImage1D
{
   struct marshal_cmd_base cmd_base;
   GLenum target;
   GLint level;
   GLint xoffset;
   GLint x;
   GLint y;
   GLsizei width;
};
uint32_t
_mesa_unmarshal_CopyTexSubImage1D(struct gl_context *ctx, const struct marshal_cmd_CopyTexSubImage1D *cmd, const uint64_t *last)
{
   GLenum target = cmd->target;
   GLint level = cmd->level;
   GLint xoffset = cmd->xoffset;
   GLint x = cmd->x;
   GLint y = cmd->y;
   GLsizei width = cmd->width;
   CALL_CopyTexSubImage1D(ctx->CurrentServerDispatch, (target, level, xoffset, x, y, width));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_CopyTexSubImage1D), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_CopyTexSubImage1D(GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_CopyTexSubImage1D);
   struct marshal_cmd_CopyTexSubImage1D *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_CopyTexSubImage1D, cmd_size);
   cmd->target = target;
   cmd->level = level;
   cmd->xoffset = xoffset;
   cmd->x = x;
   cmd->y = y;
   cmd->width = width;
}


/* CopyTexSubImage2D: marshalled asynchronously */
struct marshal_cmd_CopyTexSubImage2D
{
   struct marshal_cmd_base cmd_base;
   GLenum target;
   GLint level;
   GLint xoffset;
   GLint yoffset;
   GLint x;
   GLint y;
   GLsizei width;
   GLsizei height;
};
uint32_t
_mesa_unmarshal_CopyTexSubImage2D(struct gl_context *ctx, const struct marshal_cmd_CopyTexSubImage2D *cmd, const uint64_t *last)
{
   GLenum target = cmd->target;
   GLint level = cmd->level;
   GLint xoffset = cmd->xoffset;
   GLint yoffset = cmd->yoffset;
   GLint x = cmd->x;
   GLint y = cmd->y;
   GLsizei width = cmd->width;
   GLsizei height = cmd->height;
   CALL_CopyTexSubImage2D(ctx->CurrentServerDispatch, (target, level, xoffset, yoffset, x, y, width, height));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_CopyTexSubImage2D), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_CopyTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_CopyTexSubImage2D);
   struct marshal_cmd_CopyTexSubImage2D *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_CopyTexSubImage2D, cmd_size);
   cmd->target = target;
   cmd->level = level;
   cmd->xoffset = xoffset;
   cmd->yoffset = yoffset;
   cmd->x = x;
   cmd->y = y;
   cmd->width = width;
   cmd->height = height;
}


/* TexSubImage1D: marshalled asynchronously */
struct marshal_cmd_TexSubImage1D
{
   struct marshal_cmd_base cmd_base;
   GLenum target;
   GLint level;
   GLint xoffset;
   GLsizei width;
   GLenum format;
   GLenum type;
   const GLvoid * pixels;
};
uint32_t
_mesa_unmarshal_TexSubImage1D(struct gl_context *ctx, const struct marshal_cmd_TexSubImage1D *cmd, const uint64_t *last)
{
   GLenum target = cmd->target;
   GLint level = cmd->level;
   GLint xoffset = cmd->xoffset;
   GLsizei width = cmd->width;
   GLenum format = cmd->format;
   GLenum type = cmd->type;
   const GLvoid * pixels = cmd->pixels;
   CALL_TexSubImage1D(ctx->CurrentServerDispatch, (target, level, xoffset, width, format, type, pixels));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_TexSubImage1D), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_TexSubImage1D(GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const GLvoid * pixels)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TexSubImage1D);
   struct marshal_cmd_TexSubImage1D *cmd;
   if (_mesa_glthread_has_no_unpack_buffer(ctx)) {
      _mesa_glthread_finish_before(ctx, "TexSubImage1D");
      CALL_TexSubImage1D(ctx->CurrentServerDispatch, (target, level, xoffset, width, format, type, pixels));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexSubImage1D, cmd_size);
   cmd->target = target;
   cmd->level = level;
   cmd->xoffset = xoffset;
   cmd->width = width;
   cmd->format = format;
   cmd->type = type;
   cmd->pixels = pixels;
}


/* TexSubImage2D: marshalled asynchronously */
struct marshal_cmd_TexSubImage2D
{
   struct marshal_cmd_base cmd_base;
   GLenum target;
   GLint level;
   GLint xoffset;
   GLint yoffset;
   GLsizei width;
   GLsizei height;
   GLenum format;
   GLenum type;
   const GLvoid * pixels;
};
uint32_t
_mesa_unmarshal_TexSubImage2D(struct gl_context *ctx, const struct marshal_cmd_TexSubImage2D *cmd, const uint64_t *last)
{
   GLenum target = cmd->target;
   GLint level = cmd->level;
   GLint xoffset = cmd->xoffset;
   GLint yoffset = cmd->yoffset;
   GLsizei width = cmd->width;
   GLsizei height = cmd->height;
   GLenum format = cmd->format;
   GLenum type = cmd->type;
   const GLvoid * pixels = cmd->pixels;
   CALL_TexSubImage2D(ctx->CurrentServerDispatch, (target, level, xoffset, yoffset, width, height, format, type, pixels));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_TexSubImage2D), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_TexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid * pixels)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TexSubImage2D);
   struct marshal_cmd_TexSubImage2D *cmd;
   if (_mesa_glthread_has_no_unpack_buffer(ctx)) {
      _mesa_glthread_finish_before(ctx, "TexSubImage2D");
      CALL_TexSubImage2D(ctx->CurrentServerDispatch, (target, level, xoffset, yoffset, width, height, format, type, pixels));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexSubImage2D, cmd_size);
   cmd->target = target;
   cmd->level = level;
   cmd->xoffset = xoffset;
   cmd->yoffset = yoffset;
   cmd->width = width;
   cmd->height = height;
   cmd->format = format;
   cmd->type = type;
   cmd->pixels = pixels;
}


/* AreTexturesResident: marshalled synchronously */
GLboolean GLAPIENTRY
_mesa_marshal_AreTexturesResident(GLsizei n, const GLuint * textures, GLboolean * residences)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "AreTexturesResident");
   return CALL_AreTexturesResident(ctx->CurrentServerDispatch, (n, textures, residences));
}


/* BindTexture: marshalled asynchronously */
struct marshal_cmd_BindTexture
{
   struct marshal_cmd_base cmd_base;
   GLenum target;
   GLuint texture;
};
uint32_t
_mesa_unmarshal_BindTexture(struct gl_context *ctx, const struct marshal_cmd_BindTexture *cmd, const uint64_t *last)
{
   GLenum target = cmd->target;
   GLuint texture = cmd->texture;
   CALL_BindTexture(ctx->CurrentServerDispatch, (target, texture));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_BindTexture), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_BindTexture(GLenum target, GLuint texture)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_BindTexture);
   struct marshal_cmd_BindTexture *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_BindTexture, cmd_size);
   cmd->target = target;
   cmd->texture = texture;
}


/* DeleteTextures: marshalled asynchronously */
struct marshal_cmd_DeleteTextures
{
   struct marshal_cmd_base cmd_base;
   GLsizei n;
   /* Next safe_mul(n, 1 * sizeof(GLuint)) bytes are GLuint textures[n] */
};
uint32_t
_mesa_unmarshal_DeleteTextures(struct gl_context *ctx, const struct marshal_cmd_DeleteTextures *cmd, const uint64_t *last)
{
   GLsizei n = cmd->n;
   GLuint * textures;
   const char *variable_data = (const char *) (cmd + 1);
   textures = (GLuint *) variable_data;
   CALL_DeleteTextures(ctx->CurrentServerDispatch, (n, textures));
   return cmd->cmd_base.cmd_size;
}
void GLAPIENTRY
_mesa_marshal_DeleteTextures(GLsizei n, const GLuint * textures)
{
   GET_CURRENT_CONTEXT(ctx);
   int textures_size = safe_mul(n, 1 * sizeof(GLuint));
   int cmd_size = sizeof(struct marshal_cmd_DeleteTextures) + textures_size;
   struct marshal_cmd_DeleteTextures *cmd;
   if (unlikely(textures_size < 0 || (textures_size > 0 && !textures) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "DeleteTextures");
      CALL_DeleteTextures(ctx->CurrentServerDispatch, (n, textures));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_DeleteTextures, cmd_size);
   cmd->n = n;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, textures, textures_size);
}


/* GenTextures: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GenTextures(GLsizei n, GLuint * textures)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GenTextures");
   CALL_GenTextures(ctx->CurrentServerDispatch, (n, textures));
}


/* IsTexture: marshalled synchronously */
GLboolean GLAPIENTRY
_mesa_marshal_IsTexture(GLuint texture)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "IsTexture");
   return CALL_IsTexture(ctx->CurrentServerDispatch, (texture));
}


/* PrioritizeTextures: marshalled asynchronously */
struct marshal_cmd_PrioritizeTextures
{
   struct marshal_cmd_base cmd_base;
   GLsizei n;
   /* Next safe_mul(n, 1 * sizeof(GLuint)) bytes are GLuint textures[n] */
   /* Next safe_mul(n, 1 * sizeof(GLclampf)) bytes are GLclampf priorities[n] */
};
uint32_t
_mesa_unmarshal_PrioritizeTextures(struct gl_context *ctx, const struct marshal_cmd_PrioritizeTextures *cmd, const uint64_t *last)
{
   GLsizei n = cmd->n;
   GLuint * textures;
   GLclampf * priorities;
   const char *variable_data = (const char *) (cmd + 1);
   textures = (GLuint *) variable_data;
   variable_data += n * 1 * sizeof(GLuint);
   priorities = (GLclampf *) variable_data;
   CALL_PrioritizeTextures(ctx->CurrentServerDispatch, (n, textures, priorities));
   return cmd->cmd_base.cmd_size;
}
void GLAPIENTRY
_mesa_marshal_PrioritizeTextures(GLsizei n, const GLuint * textures, const GLclampf * priorities)
{
   GET_CURRENT_CONTEXT(ctx);
   int textures_size = safe_mul(n, 1 * sizeof(GLuint));
   int priorities_size = safe_mul(n, 1 * sizeof(GLclampf));
   int cmd_size = sizeof(struct marshal_cmd_PrioritizeTextures) + textures_size + priorities_size;
   struct marshal_cmd_PrioritizeTextures *cmd;
   if (unlikely(textures_size < 0 || (textures_size > 0 && !textures) || priorities_size < 0 || (priorities_size > 0 && !priorities) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "PrioritizeTextures");
      CALL_PrioritizeTextures(ctx->CurrentServerDispatch, (n, textures, priorities));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_PrioritizeTextures, cmd_size);
   cmd->n = n;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, textures, textures_size);
   variable_data += textures_size;
   memcpy(variable_data, priorities, priorities_size);
}


/* Indexub: marshalled asynchronously */
struct marshal_cmd_Indexub
{
   struct marshal_cmd_base cmd_base;
   GLubyte c;
};
uint32_t
_mesa_unmarshal_Indexub(struct gl_context *ctx, const struct marshal_cmd_Indexub *cmd, const uint64_t *last)
{
   GLubyte c = cmd->c;
   CALL_Indexub(ctx->CurrentServerDispatch, (c));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Indexub), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_Indexub(GLubyte c)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Indexub);
   struct marshal_cmd_Indexub *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Indexub, cmd_size);
   cmd->c = c;
}


/* Indexubv: marshalled asynchronously */
struct marshal_cmd_Indexubv
{
   struct marshal_cmd_base cmd_base;
   GLubyte c[1];
};
uint32_t
_mesa_unmarshal_Indexubv(struct gl_context *ctx, const struct marshal_cmd_Indexubv *cmd, const uint64_t *last)
{
   const GLubyte * c = cmd->c;
   CALL_Indexubv(ctx->CurrentServerDispatch, (c));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Indexubv), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_Indexubv(const GLubyte * c)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Indexubv);
   struct marshal_cmd_Indexubv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Indexubv, cmd_size);
   memcpy(cmd->c, c, 1 * sizeof(GLubyte));
}


/* PopClientAttrib: marshalled asynchronously */
struct marshal_cmd_PopClientAttrib
{
   struct marshal_cmd_base cmd_base;
};
uint32_t
_mesa_unmarshal_PopClientAttrib(struct gl_context *ctx, const struct marshal_cmd_PopClientAttrib *cmd, const uint64_t *last)
{
   CALL_PopClientAttrib(ctx->CurrentServerDispatch, ());
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_PopClientAttrib), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_PopClientAttrib(void)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_PopClientAttrib);
   struct marshal_cmd_PopClientAttrib *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_PopClientAttrib, cmd_size);
   (void) cmd;
   _mesa_glthread_PopClientAttrib(ctx);
}


/* PushClientAttrib: marshalled asynchronously */
struct marshal_cmd_PushClientAttrib
{
   struct marshal_cmd_base cmd_base;
   GLbitfield mask;
};
uint32_t
_mesa_unmarshal_PushClientAttrib(struct gl_context *ctx, const struct marshal_cmd_PushClientAttrib *cmd, const uint64_t *last)
{
   GLbitfield mask = cmd->mask;
   CALL_PushClientAttrib(ctx->CurrentServerDispatch, (mask));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_PushClientAttrib), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_PushClientAttrib(GLbitfield mask)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_PushClientAttrib);
   struct marshal_cmd_PushClientAttrib *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_PushClientAttrib, cmd_size);
   cmd->mask = mask;
   _mesa_glthread_PushClientAttrib(ctx, mask, false);
}


/* BlendColor: marshalled asynchronously */
struct marshal_cmd_BlendColor
{
   struct marshal_cmd_base cmd_base;
   GLclampf red;
   GLclampf green;
   GLclampf blue;
   GLclampf alpha;
};
uint32_t
_mesa_unmarshal_BlendColor(struct gl_context *ctx, const struct marshal_cmd_BlendColor *cmd, const uint64_t *last)
{
   GLclampf red = cmd->red;
   GLclampf green = cmd->green;
   GLclampf blue = cmd->blue;
   GLclampf alpha = cmd->alpha;
   CALL_BlendColor(ctx->CurrentServerDispatch, (red, green, blue, alpha));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_BlendColor), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_BlendColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_BlendColor);
   struct marshal_cmd_BlendColor *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_BlendColor, cmd_size);
   cmd->red = red;
   cmd->green = green;
   cmd->blue = blue;
   cmd->alpha = alpha;
}


/* BlendEquation: marshalled asynchronously */
struct marshal_cmd_BlendEquation
{
   struct marshal_cmd_base cmd_base;
   GLenum mode;
};
uint32_t
_mesa_unmarshal_BlendEquation(struct gl_context *ctx, const struct marshal_cmd_BlendEquation *cmd, const uint64_t *last)
{
   GLenum mode = cmd->mode;
   CALL_BlendEquation(ctx->CurrentServerDispatch, (mode));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_BlendEquation), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_BlendEquation(GLenum mode)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_BlendEquation);
   struct marshal_cmd_BlendEquation *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_BlendEquation, cmd_size);
   cmd->mode = mode;
}


/* ColorTable: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_ColorTable(GLenum target, GLenum internalformat, GLsizei width, GLenum format, GLenum type, const GLvoid * table)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "ColorTable");
   CALL_ColorTable(ctx->CurrentServerDispatch, (target, internalformat, width, format, type, table));
}


/* ColorTableParameterfv: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_ColorTableParameterfv(GLenum target, GLenum pname, const GLfloat * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "ColorTableParameterfv");
   CALL_ColorTableParameterfv(ctx->CurrentServerDispatch, (target, pname, params));
}


/* ColorTableParameteriv: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_ColorTableParameteriv(GLenum target, GLenum pname, const GLint * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "ColorTableParameteriv");
   CALL_ColorTableParameteriv(ctx->CurrentServerDispatch, (target, pname, params));
}


/* CopyColorTable: marshalled asynchronously */
struct marshal_cmd_CopyColorTable
{
   struct marshal_cmd_base cmd_base;
   GLenum target;
   GLenum internalformat;
   GLint x;
   GLint y;
   GLsizei width;
};
uint32_t
_mesa_unmarshal_CopyColorTable(struct gl_context *ctx, const struct marshal_cmd_CopyColorTable *cmd, const uint64_t *last)
{
   GLenum target = cmd->target;
   GLenum internalformat = cmd->internalformat;
   GLint x = cmd->x;
   GLint y = cmd->y;
   GLsizei width = cmd->width;
   CALL_CopyColorTable(ctx->CurrentServerDispatch, (target, internalformat, x, y, width));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_CopyColorTable), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_CopyColorTable(GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_CopyColorTable);
   struct marshal_cmd_CopyColorTable *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_CopyColorTable, cmd_size);
   cmd->target = target;
   cmd->internalformat = internalformat;
   cmd->x = x;
   cmd->y = y;
   cmd->width = width;
}


/* GetColorTable: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetColorTable(GLenum target, GLenum format, GLenum type, GLvoid * table)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetColorTable");
   CALL_GetColorTable(ctx->CurrentServerDispatch, (target, format, type, table));
}


/* GetColorTableParameterfv: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetColorTableParameterfv(GLenum target, GLenum pname, GLfloat * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetColorTableParameterfv");
   CALL_GetColorTableParameterfv(ctx->CurrentServerDispatch, (target, pname, params));
}


/* GetColorTableParameteriv: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetColorTableParameteriv(GLenum target, GLenum pname, GLint * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetColorTableParameteriv");
   CALL_GetColorTableParameteriv(ctx->CurrentServerDispatch, (target, pname, params));
}


/* ColorSubTable: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_ColorSubTable(GLenum target, GLsizei start, GLsizei count, GLenum format, GLenum type, const GLvoid * data)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "ColorSubTable");
   CALL_ColorSubTable(ctx->CurrentServerDispatch, (target, start, count, format, type, data));
}


/* CopyColorSubTable: marshalled asynchronously */
struct marshal_cmd_CopyColorSubTable
{
   struct marshal_cmd_base cmd_base;
   GLenum target;
   GLsizei start;
   GLint x;
   GLint y;
   GLsizei width;
};
uint32_t
_mesa_unmarshal_CopyColorSubTable(struct gl_context *ctx, const struct marshal_cmd_CopyColorSubTable *cmd, const uint64_t *last)
{
   GLenum target = cmd->target;
   GLsizei start = cmd->start;
   GLint x = cmd->x;
   GLint y = cmd->y;
   GLsizei width = cmd->width;
   CALL_CopyColorSubTable(ctx->CurrentServerDispatch, (target, start, x, y, width));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_CopyColorSubTable), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_CopyColorSubTable(GLenum target, GLsizei start, GLint x, GLint y, GLsizei width)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_CopyColorSubTable);
   struct marshal_cmd_CopyColorSubTable *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_CopyColorSubTable, cmd_size);
   cmd->target = target;
   cmd->start = start;
   cmd->x = x;
   cmd->y = y;
   cmd->width = width;
}


/* ConvolutionFilter1D: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_ConvolutionFilter1D(GLenum target, GLenum internalformat, GLsizei width, GLenum format, GLenum type, const GLvoid * image)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "ConvolutionFilter1D");
   CALL_ConvolutionFilter1D(ctx->CurrentServerDispatch, (target, internalformat, width, format, type, image));
}


/* ConvolutionFilter2D: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_ConvolutionFilter2D(GLenum target, GLenum internalformat, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid * image)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "ConvolutionFilter2D");
   CALL_ConvolutionFilter2D(ctx->CurrentServerDispatch, (target, internalformat, width, height, format, type, image));
}


/* ConvolutionParameterf: marshalled asynchronously */
struct marshal_cmd_ConvolutionParameterf
{
   struct marshal_cmd_base cmd_base;
   GLenum target;
   GLenum pname;
   GLfloat params;
};
uint32_t
_mesa_unmarshal_ConvolutionParameterf(struct gl_context *ctx, const struct marshal_cmd_ConvolutionParameterf *cmd, const uint64_t *last)
{
   GLenum target = cmd->target;
   GLenum pname = cmd->pname;
   GLfloat params = cmd->params;
   CALL_ConvolutionParameterf(ctx->CurrentServerDispatch, (target, pname, params));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_ConvolutionParameterf), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_ConvolutionParameterf(GLenum target, GLenum pname, GLfloat params)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ConvolutionParameterf);
   struct marshal_cmd_ConvolutionParameterf *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ConvolutionParameterf, cmd_size);
   cmd->target = target;
   cmd->pname = pname;
   cmd->params = params;
}


/* ConvolutionParameterfv: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_ConvolutionParameterfv(GLenum target, GLenum pname, const GLfloat * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "ConvolutionParameterfv");
   CALL_ConvolutionParameterfv(ctx->CurrentServerDispatch, (target, pname, params));
}


/* ConvolutionParameteri: marshalled asynchronously */
struct marshal_cmd_ConvolutionParameteri
{
   struct marshal_cmd_base cmd_base;
   GLenum target;
   GLenum pname;
   GLint params;
};
uint32_t
_mesa_unmarshal_ConvolutionParameteri(struct gl_context *ctx, const struct marshal_cmd_ConvolutionParameteri *cmd, const uint64_t *last)
{
   GLenum target = cmd->target;
   GLenum pname = cmd->pname;
   GLint params = cmd->params;
   CALL_ConvolutionParameteri(ctx->CurrentServerDispatch, (target, pname, params));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_ConvolutionParameteri), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_ConvolutionParameteri(GLenum target, GLenum pname, GLint params)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ConvolutionParameteri);
   struct marshal_cmd_ConvolutionParameteri *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ConvolutionParameteri, cmd_size);
   cmd->target = target;
   cmd->pname = pname;
   cmd->params = params;
}


/* ConvolutionParameteriv: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_ConvolutionParameteriv(GLenum target, GLenum pname, const GLint * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "ConvolutionParameteriv");
   CALL_ConvolutionParameteriv(ctx->CurrentServerDispatch, (target, pname, params));
}


/* CopyConvolutionFilter1D: marshalled asynchronously */
struct marshal_cmd_CopyConvolutionFilter1D
{
   struct marshal_cmd_base cmd_base;
   GLenum target;
   GLenum internalformat;
   GLint x;
   GLint y;
   GLsizei width;
};
uint32_t
_mesa_unmarshal_CopyConvolutionFilter1D(struct gl_context *ctx, const struct marshal_cmd_CopyConvolutionFilter1D *cmd, const uint64_t *last)
{
   GLenum target = cmd->target;
   GLenum internalformat = cmd->internalformat;
   GLint x = cmd->x;
   GLint y = cmd->y;
   GLsizei width = cmd->width;
   CALL_CopyConvolutionFilter1D(ctx->CurrentServerDispatch, (target, internalformat, x, y, width));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_CopyConvolutionFilter1D), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_CopyConvolutionFilter1D(GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_CopyConvolutionFilter1D);
   struct marshal_cmd_CopyConvolutionFilter1D *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_CopyConvolutionFilter1D, cmd_size);
   cmd->target = target;
   cmd->internalformat = internalformat;
   cmd->x = x;
   cmd->y = y;
   cmd->width = width;
}


/* CopyConvolutionFilter2D: marshalled asynchronously */
struct marshal_cmd_CopyConvolutionFilter2D
{
   struct marshal_cmd_base cmd_base;
   GLenum target;
   GLenum internalformat;
   GLint x;
   GLint y;
   GLsizei width;
   GLsizei height;
};
uint32_t
_mesa_unmarshal_CopyConvolutionFilter2D(struct gl_context *ctx, const struct marshal_cmd_CopyConvolutionFilter2D *cmd, const uint64_t *last)
{
   GLenum target = cmd->target;
   GLenum internalformat = cmd->internalformat;
   GLint x = cmd->x;
   GLint y = cmd->y;
   GLsizei width = cmd->width;
   GLsizei height = cmd->height;
   CALL_CopyConvolutionFilter2D(ctx->CurrentServerDispatch, (target, internalformat, x, y, width, height));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_CopyConvolutionFilter2D), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_CopyConvolutionFilter2D(GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_CopyConvolutionFilter2D);
   struct marshal_cmd_CopyConvolutionFilter2D *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_CopyConvolutionFilter2D, cmd_size);
   cmd->target = target;
   cmd->internalformat = internalformat;
   cmd->x = x;
   cmd->y = y;
   cmd->width = width;
   cmd->height = height;
}


/* GetConvolutionFilter: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetConvolutionFilter(GLenum target, GLenum format, GLenum type, GLvoid * image)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetConvolutionFilter");
   CALL_GetConvolutionFilter(ctx->CurrentServerDispatch, (target, format, type, image));
}


/* GetConvolutionParameterfv: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetConvolutionParameterfv(GLenum target, GLenum pname, GLfloat * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetConvolutionParameterfv");
   CALL_GetConvolutionParameterfv(ctx->CurrentServerDispatch, (target, pname, params));
}


/* GetConvolutionParameteriv: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetConvolutionParameteriv(GLenum target, GLenum pname, GLint * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetConvolutionParameteriv");
   CALL_GetConvolutionParameteriv(ctx->CurrentServerDispatch, (target, pname, params));
}


/* GetSeparableFilter: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetSeparableFilter(GLenum target, GLenum format, GLenum type, GLvoid * row, GLvoid * column, GLvoid * span)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetSeparableFilter");
   CALL_GetSeparableFilter(ctx->CurrentServerDispatch, (target, format, type, row, column, span));
}


/* SeparableFilter2D: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_SeparableFilter2D(GLenum target, GLenum internalformat, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid * row, const GLvoid * column)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "SeparableFilter2D");
   CALL_SeparableFilter2D(ctx->CurrentServerDispatch, (target, internalformat, width, height, format, type, row, column));
}


/* GetHistogram: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetHistogram(GLenum target, GLboolean reset, GLenum format, GLenum type, GLvoid * values)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetHistogram");
   CALL_GetHistogram(ctx->CurrentServerDispatch, (target, reset, format, type, values));
}


/* GetHistogramParameterfv: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetHistogramParameterfv(GLenum target, GLenum pname, GLfloat * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetHistogramParameterfv");
   CALL_GetHistogramParameterfv(ctx->CurrentServerDispatch, (target, pname, params));
}


/* GetHistogramParameteriv: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetHistogramParameteriv(GLenum target, GLenum pname, GLint * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetHistogramParameteriv");
   CALL_GetHistogramParameteriv(ctx->CurrentServerDispatch, (target, pname, params));
}


/* GetMinmax: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetMinmax(GLenum target, GLboolean reset, GLenum format, GLenum type, GLvoid * values)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetMinmax");
   CALL_GetMinmax(ctx->CurrentServerDispatch, (target, reset, format, type, values));
}


/* GetMinmaxParameterfv: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetMinmaxParameterfv(GLenum target, GLenum pname, GLfloat * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetMinmaxParameterfv");
   CALL_GetMinmaxParameterfv(ctx->CurrentServerDispatch, (target, pname, params));
}


/* GetMinmaxParameteriv: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetMinmaxParameteriv(GLenum target, GLenum pname, GLint * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetMinmaxParameteriv");
   CALL_GetMinmaxParameteriv(ctx->CurrentServerDispatch, (target, pname, params));
}


/* Histogram: marshalled asynchronously */
struct marshal_cmd_Histogram
{
   struct marshal_cmd_base cmd_base;
   GLboolean sink;
   GLenum target;
   GLsizei width;
   GLenum internalformat;
};
uint32_t
_mesa_unmarshal_Histogram(struct gl_context *ctx, const struct marshal_cmd_Histogram *cmd, const uint64_t *last)
{
   GLenum target = cmd->target;
   GLsizei width = cmd->width;
   GLenum internalformat = cmd->internalformat;
   GLboolean sink = cmd->sink;
   CALL_Histogram(ctx->CurrentServerDispatch, (target, width, internalformat, sink));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Histogram), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_Histogram(GLenum target, GLsizei width, GLenum internalformat, GLboolean sink)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Histogram);
   struct marshal_cmd_Histogram *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Histogram, cmd_size);
   cmd->target = target;
   cmd->width = width;
   cmd->internalformat = internalformat;
   cmd->sink = sink;
}


/* Minmax: marshalled asynchronously */
struct marshal_cmd_Minmax
{
   struct marshal_cmd_base cmd_base;
   GLboolean sink;
   GLenum target;
   GLenum internalformat;
};
uint32_t
_mesa_unmarshal_Minmax(struct gl_context *ctx, const struct marshal_cmd_Minmax *cmd, const uint64_t *last)
{
   GLenum target = cmd->target;
   GLenum internalformat = cmd->internalformat;
   GLboolean sink = cmd->sink;
   CALL_Minmax(ctx->CurrentServerDispatch, (target, internalformat, sink));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Minmax), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_Minmax(GLenum target, GLenum internalformat, GLboolean sink)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Minmax);
   struct marshal_cmd_Minmax *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Minmax, cmd_size);
   cmd->target = target;
   cmd->internalformat = internalformat;
   cmd->sink = sink;
}


/* ResetHistogram: marshalled asynchronously */
struct marshal_cmd_ResetHistogram
{
   struct marshal_cmd_base cmd_base;
   GLenum target;
};
uint32_t
_mesa_unmarshal_ResetHistogram(struct gl_context *ctx, const struct marshal_cmd_ResetHistogram *cmd, const uint64_t *last)
{
   GLenum target = cmd->target;
   CALL_ResetHistogram(ctx->CurrentServerDispatch, (target));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_ResetHistogram), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_ResetHistogram(GLenum target)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ResetHistogram);
   struct marshal_cmd_ResetHistogram *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ResetHistogram, cmd_size);
   cmd->target = target;
}


/* ResetMinmax: marshalled asynchronously */
struct marshal_cmd_ResetMinmax
{
   struct marshal_cmd_base cmd_base;
   GLenum target;
};
uint32_t
_mesa_unmarshal_ResetMinmax(struct gl_context *ctx, const struct marshal_cmd_ResetMinmax *cmd, const uint64_t *last)
{
   GLenum target = cmd->target;
   CALL_ResetMinmax(ctx->CurrentServerDispatch, (target));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_ResetMinmax), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_ResetMinmax(GLenum target)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ResetMinmax);
   struct marshal_cmd_ResetMinmax *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ResetMinmax, cmd_size);
   cmd->target = target;
}


/* TexImage3D: marshalled asynchronously */
struct marshal_cmd_TexImage3D
{
   struct marshal_cmd_base cmd_base;
   GLenum target;
   GLint level;
   GLint internalformat;
   GLsizei width;
   GLsizei height;
   GLsizei depth;
   GLint border;
   GLenum format;
   GLenum type;
   const GLvoid * pixels;
};
uint32_t
_mesa_unmarshal_TexImage3D(struct gl_context *ctx, const struct marshal_cmd_TexImage3D *cmd, const uint64_t *last)
{
   GLenum target = cmd->target;
   GLint level = cmd->level;
   GLint internalformat = cmd->internalformat;
   GLsizei width = cmd->width;
   GLsizei height = cmd->height;
   GLsizei depth = cmd->depth;
   GLint border = cmd->border;
   GLenum format = cmd->format;
   GLenum type = cmd->type;
   const GLvoid * pixels = cmd->pixels;
   CALL_TexImage3D(ctx->CurrentServerDispatch, (target, level, internalformat, width, height, depth, border, format, type, pixels));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_TexImage3D), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_TexImage3D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid * pixels)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TexImage3D);
   struct marshal_cmd_TexImage3D *cmd;
   if (_mesa_glthread_has_no_unpack_buffer(ctx)) {
      _mesa_glthread_finish_before(ctx, "TexImage3D");
      CALL_TexImage3D(ctx->CurrentServerDispatch, (target, level, internalformat, width, height, depth, border, format, type, pixels));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexImage3D, cmd_size);
   cmd->target = target;
   cmd->level = level;
   cmd->internalformat = internalformat;
   cmd->width = width;
   cmd->height = height;
   cmd->depth = depth;
   cmd->border = border;
   cmd->format = format;
   cmd->type = type;
   cmd->pixels = pixels;
}


/* TexSubImage3D: marshalled asynchronously */
struct marshal_cmd_TexSubImage3D
{
   struct marshal_cmd_base cmd_base;
   GLenum target;
   GLint level;
   GLint xoffset;
   GLint yoffset;
   GLint zoffset;
   GLsizei width;
   GLsizei height;
   GLsizei depth;
   GLenum format;
   GLenum type;
   const GLvoid * pixels;
};
uint32_t
_mesa_unmarshal_TexSubImage3D(struct gl_context *ctx, const struct marshal_cmd_TexSubImage3D *cmd, const uint64_t *last)
{
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
   CALL_TexSubImage3D(ctx->CurrentServerDispatch, (target, level, xoffset, yoffset, zoffset, width, height, depth, format, type, pixels));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_TexSubImage3D), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_TexSubImage3D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const GLvoid * pixels)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TexSubImage3D);
   struct marshal_cmd_TexSubImage3D *cmd;
   if (_mesa_glthread_has_no_unpack_buffer(ctx)) {
      _mesa_glthread_finish_before(ctx, "TexSubImage3D");
      CALL_TexSubImage3D(ctx->CurrentServerDispatch, (target, level, xoffset, yoffset, zoffset, width, height, depth, format, type, pixels));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexSubImage3D, cmd_size);
   cmd->target = target;
   cmd->level = level;
   cmd->xoffset = xoffset;
   cmd->yoffset = yoffset;
   cmd->zoffset = zoffset;
   cmd->width = width;
   cmd->height = height;
   cmd->depth = depth;
   cmd->format = format;
   cmd->type = type;
   cmd->pixels = pixels;
}


/* CopyTexSubImage3D: marshalled asynchronously */
struct marshal_cmd_CopyTexSubImage3D
{
   struct marshal_cmd_base cmd_base;
   GLenum target;
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
_mesa_unmarshal_CopyTexSubImage3D(struct gl_context *ctx, const struct marshal_cmd_CopyTexSubImage3D *cmd, const uint64_t *last)
{
   GLenum target = cmd->target;
   GLint level = cmd->level;
   GLint xoffset = cmd->xoffset;
   GLint yoffset = cmd->yoffset;
   GLint zoffset = cmd->zoffset;
   GLint x = cmd->x;
   GLint y = cmd->y;
   GLsizei width = cmd->width;
   GLsizei height = cmd->height;
   CALL_CopyTexSubImage3D(ctx->CurrentServerDispatch, (target, level, xoffset, yoffset, zoffset, x, y, width, height));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_CopyTexSubImage3D), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_CopyTexSubImage3D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_CopyTexSubImage3D);
   struct marshal_cmd_CopyTexSubImage3D *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_CopyTexSubImage3D, cmd_size);
   cmd->target = target;
   cmd->level = level;
   cmd->xoffset = xoffset;
   cmd->yoffset = yoffset;
   cmd->zoffset = zoffset;
   cmd->x = x;
   cmd->y = y;
   cmd->width = width;
   cmd->height = height;
}


/* ActiveTexture: marshalled asynchronously */
struct marshal_cmd_ActiveTexture
{
   struct marshal_cmd_base cmd_base;
   GLenum texture;
};
uint32_t
_mesa_unmarshal_ActiveTexture(struct gl_context *ctx, const struct marshal_cmd_ActiveTexture *cmd, const uint64_t *last)
{
   GLenum texture = cmd->texture;
   CALL_ActiveTexture(ctx->CurrentServerDispatch, (texture));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_ActiveTexture), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_ActiveTexture(GLenum texture)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ActiveTexture);
   struct marshal_cmd_ActiveTexture *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ActiveTexture, cmd_size);
   cmd->texture = texture;
   ctx->GLThread.ActiveTexture = texture - GL_TEXTURE0; if (ctx->GLThread.MatrixMode == GL_TEXTURE) ctx->GLThread.MatrixIndex = _mesa_get_matrix_index(ctx, texture);
}


/* ClientActiveTexture: marshalled asynchronously */
struct marshal_cmd_ClientActiveTexture
{
   struct marshal_cmd_base cmd_base;
   GLenum texture;
};
uint32_t
_mesa_unmarshal_ClientActiveTexture(struct gl_context *ctx, const struct marshal_cmd_ClientActiveTexture *cmd, const uint64_t *last)
{
   GLenum texture = cmd->texture;
   CALL_ClientActiveTexture(ctx->CurrentServerDispatch, (texture));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_ClientActiveTexture), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_ClientActiveTexture(GLenum texture)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ClientActiveTexture);
   struct marshal_cmd_ClientActiveTexture *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ClientActiveTexture, cmd_size);
   cmd->texture = texture;
   ctx->GLThread.ClientActiveTexture = texture - GL_TEXTURE0;
}


/* MultiTexCoord1d: marshalled asynchronously */
struct marshal_cmd_MultiTexCoord1d
{
   struct marshal_cmd_base cmd_base;
   GLenum target;
   GLdouble s;
};
uint32_t
_mesa_unmarshal_MultiTexCoord1d(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord1d *cmd, const uint64_t *last)
{
   GLenum target = cmd->target;
   GLdouble s = cmd->s;
   CALL_MultiTexCoord1d(ctx->CurrentServerDispatch, (target, s));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MultiTexCoord1d), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_MultiTexCoord1d(GLenum target, GLdouble s)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexCoord1d);
   struct marshal_cmd_MultiTexCoord1d *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexCoord1d, cmd_size);
   cmd->target = target;
   cmd->s = s;
}


/* MultiTexCoord1dv: marshalled asynchronously */
struct marshal_cmd_MultiTexCoord1dv
{
   struct marshal_cmd_base cmd_base;
   GLenum target;
   GLdouble v[1];
};
uint32_t
_mesa_unmarshal_MultiTexCoord1dv(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord1dv *cmd, const uint64_t *last)
{
   GLenum target = cmd->target;
   const GLdouble * v = cmd->v;
   CALL_MultiTexCoord1dv(ctx->CurrentServerDispatch, (target, v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MultiTexCoord1dv), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_MultiTexCoord1dv(GLenum target, const GLdouble * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexCoord1dv);
   struct marshal_cmd_MultiTexCoord1dv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexCoord1dv, cmd_size);
   cmd->target = target;
   memcpy(cmd->v, v, 1 * sizeof(GLdouble));
}


/* MultiTexCoord1fARB: marshalled asynchronously */
struct marshal_cmd_MultiTexCoord1fARB
{
   struct marshal_cmd_base cmd_base;
   GLenum target;
   GLfloat s;
};
uint32_t
_mesa_unmarshal_MultiTexCoord1fARB(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord1fARB *cmd, const uint64_t *last)
{
   GLenum target = cmd->target;
   GLfloat s = cmd->s;
   CALL_MultiTexCoord1fARB(ctx->CurrentServerDispatch, (target, s));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MultiTexCoord1fARB), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_MultiTexCoord1fARB(GLenum target, GLfloat s)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexCoord1fARB);
   struct marshal_cmd_MultiTexCoord1fARB *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexCoord1fARB, cmd_size);
   cmd->target = target;
   cmd->s = s;
}


/* MultiTexCoord1fvARB: marshalled asynchronously */
struct marshal_cmd_MultiTexCoord1fvARB
{
   struct marshal_cmd_base cmd_base;
   GLenum target;
   GLfloat v[1];
};
uint32_t
_mesa_unmarshal_MultiTexCoord1fvARB(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord1fvARB *cmd, const uint64_t *last)
{
   GLenum target = cmd->target;
   const GLfloat * v = cmd->v;
   CALL_MultiTexCoord1fvARB(ctx->CurrentServerDispatch, (target, v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MultiTexCoord1fvARB), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_MultiTexCoord1fvARB(GLenum target, const GLfloat * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexCoord1fvARB);
   struct marshal_cmd_MultiTexCoord1fvARB *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexCoord1fvARB, cmd_size);
   cmd->target = target;
   memcpy(cmd->v, v, 1 * sizeof(GLfloat));
}


/* MultiTexCoord1i: marshalled asynchronously */
struct marshal_cmd_MultiTexCoord1i
{
   struct marshal_cmd_base cmd_base;
   GLenum target;
   GLint s;
};
uint32_t
_mesa_unmarshal_MultiTexCoord1i(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord1i *cmd, const uint64_t *last)
{
   GLenum target = cmd->target;
   GLint s = cmd->s;
   CALL_MultiTexCoord1i(ctx->CurrentServerDispatch, (target, s));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MultiTexCoord1i), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_MultiTexCoord1i(GLenum target, GLint s)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexCoord1i);
   struct marshal_cmd_MultiTexCoord1i *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexCoord1i, cmd_size);
   cmd->target = target;
   cmd->s = s;
}


/* MultiTexCoord1iv: marshalled asynchronously */
struct marshal_cmd_MultiTexCoord1iv
{
   struct marshal_cmd_base cmd_base;
   GLenum target;
   GLint v[1];
};
uint32_t
_mesa_unmarshal_MultiTexCoord1iv(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord1iv *cmd, const uint64_t *last)
{
   GLenum target = cmd->target;
   const GLint * v = cmd->v;
   CALL_MultiTexCoord1iv(ctx->CurrentServerDispatch, (target, v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MultiTexCoord1iv), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_MultiTexCoord1iv(GLenum target, const GLint * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexCoord1iv);
   struct marshal_cmd_MultiTexCoord1iv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexCoord1iv, cmd_size);
   cmd->target = target;
   memcpy(cmd->v, v, 1 * sizeof(GLint));
}


/* MultiTexCoord1s: marshalled asynchronously */
struct marshal_cmd_MultiTexCoord1s
{
   struct marshal_cmd_base cmd_base;
   GLshort s;
   GLenum target;
};
uint32_t
_mesa_unmarshal_MultiTexCoord1s(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord1s *cmd, const uint64_t *last)
{
   GLenum target = cmd->target;
   GLshort s = cmd->s;
   CALL_MultiTexCoord1s(ctx->CurrentServerDispatch, (target, s));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MultiTexCoord1s), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_MultiTexCoord1s(GLenum target, GLshort s)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexCoord1s);
   struct marshal_cmd_MultiTexCoord1s *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexCoord1s, cmd_size);
   cmd->target = target;
   cmd->s = s;
}


/* MultiTexCoord1sv: marshalled asynchronously */
struct marshal_cmd_MultiTexCoord1sv
{
   struct marshal_cmd_base cmd_base;
   GLenum target;
   GLshort v[1];
};
uint32_t
_mesa_unmarshal_MultiTexCoord1sv(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord1sv *cmd, const uint64_t *last)
{
   GLenum target = cmd->target;
   const GLshort * v = cmd->v;
   CALL_MultiTexCoord1sv(ctx->CurrentServerDispatch, (target, v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MultiTexCoord1sv), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_MultiTexCoord1sv(GLenum target, const GLshort * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexCoord1sv);
   struct marshal_cmd_MultiTexCoord1sv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexCoord1sv, cmd_size);
   cmd->target = target;
   memcpy(cmd->v, v, 1 * sizeof(GLshort));
}


/* MultiTexCoord2d: marshalled asynchronously */
struct marshal_cmd_MultiTexCoord2d
{
   struct marshal_cmd_base cmd_base;
   GLenum target;
   GLdouble s;
   GLdouble t;
};
uint32_t
_mesa_unmarshal_MultiTexCoord2d(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord2d *cmd, const uint64_t *last)
{
   GLenum target = cmd->target;
   GLdouble s = cmd->s;
   GLdouble t = cmd->t;
   CALL_MultiTexCoord2d(ctx->CurrentServerDispatch, (target, s, t));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MultiTexCoord2d), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_MultiTexCoord2d(GLenum target, GLdouble s, GLdouble t)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexCoord2d);
   struct marshal_cmd_MultiTexCoord2d *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexCoord2d, cmd_size);
   cmd->target = target;
   cmd->s = s;
   cmd->t = t;
}


/* MultiTexCoord2dv: marshalled asynchronously */
struct marshal_cmd_MultiTexCoord2dv
{
   struct marshal_cmd_base cmd_base;
   GLenum target;
   GLdouble v[2];
};
uint32_t
_mesa_unmarshal_MultiTexCoord2dv(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord2dv *cmd, const uint64_t *last)
{
   GLenum target = cmd->target;
   const GLdouble * v = cmd->v;
   CALL_MultiTexCoord2dv(ctx->CurrentServerDispatch, (target, v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MultiTexCoord2dv), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_MultiTexCoord2dv(GLenum target, const GLdouble * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexCoord2dv);
   struct marshal_cmd_MultiTexCoord2dv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexCoord2dv, cmd_size);
   cmd->target = target;
   memcpy(cmd->v, v, 2 * sizeof(GLdouble));
}


/* MultiTexCoord2fARB: marshalled asynchronously */
struct marshal_cmd_MultiTexCoord2fARB
{
   struct marshal_cmd_base cmd_base;
   GLenum target;
   GLfloat s;
   GLfloat t;
};
uint32_t
_mesa_unmarshal_MultiTexCoord2fARB(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord2fARB *cmd, const uint64_t *last)
{
   GLenum target = cmd->target;
   GLfloat s = cmd->s;
   GLfloat t = cmd->t;
   CALL_MultiTexCoord2fARB(ctx->CurrentServerDispatch, (target, s, t));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MultiTexCoord2fARB), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_MultiTexCoord2fARB(GLenum target, GLfloat s, GLfloat t)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexCoord2fARB);
   struct marshal_cmd_MultiTexCoord2fARB *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexCoord2fARB, cmd_size);
   cmd->target = target;
   cmd->s = s;
   cmd->t = t;
}


/* MultiTexCoord2fvARB: marshalled asynchronously */
struct marshal_cmd_MultiTexCoord2fvARB
{
   struct marshal_cmd_base cmd_base;
   GLenum target;
   GLfloat v[2];
};
uint32_t
_mesa_unmarshal_MultiTexCoord2fvARB(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord2fvARB *cmd, const uint64_t *last)
{
   GLenum target = cmd->target;
   const GLfloat * v = cmd->v;
   CALL_MultiTexCoord2fvARB(ctx->CurrentServerDispatch, (target, v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MultiTexCoord2fvARB), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_MultiTexCoord2fvARB(GLenum target, const GLfloat * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexCoord2fvARB);
   struct marshal_cmd_MultiTexCoord2fvARB *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexCoord2fvARB, cmd_size);
   cmd->target = target;
   memcpy(cmd->v, v, 2 * sizeof(GLfloat));
}


/* MultiTexCoord2i: marshalled asynchronously */
struct marshal_cmd_MultiTexCoord2i
{
   struct marshal_cmd_base cmd_base;
   GLenum target;
   GLint s;
   GLint t;
};
uint32_t
_mesa_unmarshal_MultiTexCoord2i(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord2i *cmd, const uint64_t *last)
{
   GLenum target = cmd->target;
   GLint s = cmd->s;
   GLint t = cmd->t;
   CALL_MultiTexCoord2i(ctx->CurrentServerDispatch, (target, s, t));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MultiTexCoord2i), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_MultiTexCoord2i(GLenum target, GLint s, GLint t)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexCoord2i);
   struct marshal_cmd_MultiTexCoord2i *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexCoord2i, cmd_size);
   cmd->target = target;
   cmd->s = s;
   cmd->t = t;
}


/* MultiTexCoord2iv: marshalled asynchronously */
struct marshal_cmd_MultiTexCoord2iv
{
   struct marshal_cmd_base cmd_base;
   GLenum target;
   GLint v[2];
};
uint32_t
_mesa_unmarshal_MultiTexCoord2iv(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord2iv *cmd, const uint64_t *last)
{
   GLenum target = cmd->target;
   const GLint * v = cmd->v;
   CALL_MultiTexCoord2iv(ctx->CurrentServerDispatch, (target, v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MultiTexCoord2iv), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_MultiTexCoord2iv(GLenum target, const GLint * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexCoord2iv);
   struct marshal_cmd_MultiTexCoord2iv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexCoord2iv, cmd_size);
   cmd->target = target;
   memcpy(cmd->v, v, 2 * sizeof(GLint));
}


/* MultiTexCoord2s: marshalled asynchronously */
struct marshal_cmd_MultiTexCoord2s
{
   struct marshal_cmd_base cmd_base;
   GLshort s;
   GLshort t;
   GLenum target;
};
uint32_t
_mesa_unmarshal_MultiTexCoord2s(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord2s *cmd, const uint64_t *last)
{
   GLenum target = cmd->target;
   GLshort s = cmd->s;
   GLshort t = cmd->t;
   CALL_MultiTexCoord2s(ctx->CurrentServerDispatch, (target, s, t));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MultiTexCoord2s), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_MultiTexCoord2s(GLenum target, GLshort s, GLshort t)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexCoord2s);
   struct marshal_cmd_MultiTexCoord2s *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexCoord2s, cmd_size);
   cmd->target = target;
   cmd->s = s;
   cmd->t = t;
}


/* MultiTexCoord2sv: marshalled asynchronously */
struct marshal_cmd_MultiTexCoord2sv
{
   struct marshal_cmd_base cmd_base;
   GLenum target;
   GLshort v[2];
};
uint32_t
_mesa_unmarshal_MultiTexCoord2sv(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord2sv *cmd, const uint64_t *last)
{
   GLenum target = cmd->target;
   const GLshort * v = cmd->v;
   CALL_MultiTexCoord2sv(ctx->CurrentServerDispatch, (target, v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MultiTexCoord2sv), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_MultiTexCoord2sv(GLenum target, const GLshort * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexCoord2sv);
   struct marshal_cmd_MultiTexCoord2sv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexCoord2sv, cmd_size);
   cmd->target = target;
   memcpy(cmd->v, v, 2 * sizeof(GLshort));
}


/* MultiTexCoord3d: marshalled asynchronously */
struct marshal_cmd_MultiTexCoord3d
{
   struct marshal_cmd_base cmd_base;
   GLenum target;
   GLdouble s;
   GLdouble t;
   GLdouble r;
};
uint32_t
_mesa_unmarshal_MultiTexCoord3d(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord3d *cmd, const uint64_t *last)
{
   GLenum target = cmd->target;
   GLdouble s = cmd->s;
   GLdouble t = cmd->t;
   GLdouble r = cmd->r;
   CALL_MultiTexCoord3d(ctx->CurrentServerDispatch, (target, s, t, r));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MultiTexCoord3d), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_MultiTexCoord3d(GLenum target, GLdouble s, GLdouble t, GLdouble r)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexCoord3d);
   struct marshal_cmd_MultiTexCoord3d *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexCoord3d, cmd_size);
   cmd->target = target;
   cmd->s = s;
   cmd->t = t;
   cmd->r = r;
}


/* MultiTexCoord3dv: marshalled asynchronously */
struct marshal_cmd_MultiTexCoord3dv
{
   struct marshal_cmd_base cmd_base;
   GLenum target;
   GLdouble v[3];
};
uint32_t
_mesa_unmarshal_MultiTexCoord3dv(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord3dv *cmd, const uint64_t *last)
{
   GLenum target = cmd->target;
   const GLdouble * v = cmd->v;
   CALL_MultiTexCoord3dv(ctx->CurrentServerDispatch, (target, v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MultiTexCoord3dv), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_MultiTexCoord3dv(GLenum target, const GLdouble * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexCoord3dv);
   struct marshal_cmd_MultiTexCoord3dv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexCoord3dv, cmd_size);
   cmd->target = target;
   memcpy(cmd->v, v, 3 * sizeof(GLdouble));
}


/* MultiTexCoord3fARB: marshalled asynchronously */
struct marshal_cmd_MultiTexCoord3fARB
{
   struct marshal_cmd_base cmd_base;
   GLenum target;
   GLfloat s;
   GLfloat t;
   GLfloat r;
};
uint32_t
_mesa_unmarshal_MultiTexCoord3fARB(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord3fARB *cmd, const uint64_t *last)
{
   GLenum target = cmd->target;
   GLfloat s = cmd->s;
   GLfloat t = cmd->t;
   GLfloat r = cmd->r;
   CALL_MultiTexCoord3fARB(ctx->CurrentServerDispatch, (target, s, t, r));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MultiTexCoord3fARB), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_MultiTexCoord3fARB(GLenum target, GLfloat s, GLfloat t, GLfloat r)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexCoord3fARB);
   struct marshal_cmd_MultiTexCoord3fARB *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexCoord3fARB, cmd_size);
   cmd->target = target;
   cmd->s = s;
   cmd->t = t;
   cmd->r = r;
}


/* MultiTexCoord3fvARB: marshalled asynchronously */
struct marshal_cmd_MultiTexCoord3fvARB
{
   struct marshal_cmd_base cmd_base;
   GLenum target;
   GLfloat v[3];
};
uint32_t
_mesa_unmarshal_MultiTexCoord3fvARB(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord3fvARB *cmd, const uint64_t *last)
{
   GLenum target = cmd->target;
   const GLfloat * v = cmd->v;
   CALL_MultiTexCoord3fvARB(ctx->CurrentServerDispatch, (target, v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MultiTexCoord3fvARB), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_MultiTexCoord3fvARB(GLenum target, const GLfloat * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexCoord3fvARB);
   struct marshal_cmd_MultiTexCoord3fvARB *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexCoord3fvARB, cmd_size);
   cmd->target = target;
   memcpy(cmd->v, v, 3 * sizeof(GLfloat));
}


/* MultiTexCoord3i: marshalled asynchronously */
struct marshal_cmd_MultiTexCoord3i
{
   struct marshal_cmd_base cmd_base;
   GLenum target;
   GLint s;
   GLint t;
   GLint r;
};
uint32_t
_mesa_unmarshal_MultiTexCoord3i(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord3i *cmd, const uint64_t *last)
{
   GLenum target = cmd->target;
   GLint s = cmd->s;
   GLint t = cmd->t;
   GLint r = cmd->r;
   CALL_MultiTexCoord3i(ctx->CurrentServerDispatch, (target, s, t, r));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MultiTexCoord3i), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_MultiTexCoord3i(GLenum target, GLint s, GLint t, GLint r)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexCoord3i);
   struct marshal_cmd_MultiTexCoord3i *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexCoord3i, cmd_size);
   cmd->target = target;
   cmd->s = s;
   cmd->t = t;
   cmd->r = r;
}


/* MultiTexCoord3iv: marshalled asynchronously */
struct marshal_cmd_MultiTexCoord3iv
{
   struct marshal_cmd_base cmd_base;
   GLenum target;
   GLint v[3];
};
uint32_t
_mesa_unmarshal_MultiTexCoord3iv(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord3iv *cmd, const uint64_t *last)
{
   GLenum target = cmd->target;
   const GLint * v = cmd->v;
   CALL_MultiTexCoord3iv(ctx->CurrentServerDispatch, (target, v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MultiTexCoord3iv), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_MultiTexCoord3iv(GLenum target, const GLint * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexCoord3iv);
   struct marshal_cmd_MultiTexCoord3iv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexCoord3iv, cmd_size);
   cmd->target = target;
   memcpy(cmd->v, v, 3 * sizeof(GLint));
}


/* MultiTexCoord3s: marshalled asynchronously */
struct marshal_cmd_MultiTexCoord3s
{
   struct marshal_cmd_base cmd_base;
   GLshort s;
   GLshort t;
   GLshort r;
   GLenum target;
};
uint32_t
_mesa_unmarshal_MultiTexCoord3s(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord3s *cmd, const uint64_t *last)
{
   GLenum target = cmd->target;
   GLshort s = cmd->s;
   GLshort t = cmd->t;
   GLshort r = cmd->r;
   CALL_MultiTexCoord3s(ctx->CurrentServerDispatch, (target, s, t, r));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MultiTexCoord3s), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_MultiTexCoord3s(GLenum target, GLshort s, GLshort t, GLshort r)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexCoord3s);
   struct marshal_cmd_MultiTexCoord3s *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexCoord3s, cmd_size);
   cmd->target = target;
   cmd->s = s;
   cmd->t = t;
   cmd->r = r;
}


/* MultiTexCoord3sv: marshalled asynchronously */
struct marshal_cmd_MultiTexCoord3sv
{
   struct marshal_cmd_base cmd_base;
   GLenum target;
   GLshort v[3];
};
uint32_t
_mesa_unmarshal_MultiTexCoord3sv(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord3sv *cmd, const uint64_t *last)
{
   GLenum target = cmd->target;
   const GLshort * v = cmd->v;
   CALL_MultiTexCoord3sv(ctx->CurrentServerDispatch, (target, v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MultiTexCoord3sv), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_MultiTexCoord3sv(GLenum target, const GLshort * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexCoord3sv);
   struct marshal_cmd_MultiTexCoord3sv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexCoord3sv, cmd_size);
   cmd->target = target;
   memcpy(cmd->v, v, 3 * sizeof(GLshort));
}


/* MultiTexCoord4d: marshalled asynchronously */
struct marshal_cmd_MultiTexCoord4d
{
   struct marshal_cmd_base cmd_base;
   GLenum target;
   GLdouble s;
   GLdouble t;
   GLdouble r;
   GLdouble q;
};
uint32_t
_mesa_unmarshal_MultiTexCoord4d(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord4d *cmd, const uint64_t *last)
{
   GLenum target = cmd->target;
   GLdouble s = cmd->s;
   GLdouble t = cmd->t;
   GLdouble r = cmd->r;
   GLdouble q = cmd->q;
   CALL_MultiTexCoord4d(ctx->CurrentServerDispatch, (target, s, t, r, q));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MultiTexCoord4d), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_MultiTexCoord4d(GLenum target, GLdouble s, GLdouble t, GLdouble r, GLdouble q)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexCoord4d);
   struct marshal_cmd_MultiTexCoord4d *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexCoord4d, cmd_size);
   cmd->target = target;
   cmd->s = s;
   cmd->t = t;
   cmd->r = r;
   cmd->q = q;
}


/* MultiTexCoord4dv: marshalled asynchronously */
struct marshal_cmd_MultiTexCoord4dv
{
   struct marshal_cmd_base cmd_base;
   GLenum target;
   GLdouble v[4];
};
uint32_t
_mesa_unmarshal_MultiTexCoord4dv(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord4dv *cmd, const uint64_t *last)
{
   GLenum target = cmd->target;
   const GLdouble * v = cmd->v;
   CALL_MultiTexCoord4dv(ctx->CurrentServerDispatch, (target, v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MultiTexCoord4dv), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_MultiTexCoord4dv(GLenum target, const GLdouble * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexCoord4dv);
   struct marshal_cmd_MultiTexCoord4dv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexCoord4dv, cmd_size);
   cmd->target = target;
   memcpy(cmd->v, v, 4 * sizeof(GLdouble));
}


/* MultiTexCoord4fARB: marshalled asynchronously */
struct marshal_cmd_MultiTexCoord4fARB
{
   struct marshal_cmd_base cmd_base;
   GLenum target;
   GLfloat s;
   GLfloat t;
   GLfloat r;
   GLfloat q;
};
uint32_t
_mesa_unmarshal_MultiTexCoord4fARB(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord4fARB *cmd, const uint64_t *last)
{
   GLenum target = cmd->target;
   GLfloat s = cmd->s;
   GLfloat t = cmd->t;
   GLfloat r = cmd->r;
   GLfloat q = cmd->q;
   CALL_MultiTexCoord4fARB(ctx->CurrentServerDispatch, (target, s, t, r, q));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MultiTexCoord4fARB), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_MultiTexCoord4fARB(GLenum target, GLfloat s, GLfloat t, GLfloat r, GLfloat q)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexCoord4fARB);
   struct marshal_cmd_MultiTexCoord4fARB *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexCoord4fARB, cmd_size);
   cmd->target = target;
   cmd->s = s;
   cmd->t = t;
   cmd->r = r;
   cmd->q = q;
}


/* MultiTexCoord4fvARB: marshalled asynchronously */
struct marshal_cmd_MultiTexCoord4fvARB
{
   struct marshal_cmd_base cmd_base;
   GLenum target;
   GLfloat v[4];
};
uint32_t
_mesa_unmarshal_MultiTexCoord4fvARB(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord4fvARB *cmd, const uint64_t *last)
{
   GLenum target = cmd->target;
   const GLfloat * v = cmd->v;
   CALL_MultiTexCoord4fvARB(ctx->CurrentServerDispatch, (target, v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MultiTexCoord4fvARB), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_MultiTexCoord4fvARB(GLenum target, const GLfloat * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexCoord4fvARB);
   struct marshal_cmd_MultiTexCoord4fvARB *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexCoord4fvARB, cmd_size);
   cmd->target = target;
   memcpy(cmd->v, v, 4 * sizeof(GLfloat));
}


/* MultiTexCoord4i: marshalled asynchronously */
struct marshal_cmd_MultiTexCoord4i
{
   struct marshal_cmd_base cmd_base;
   GLenum target;
   GLint s;
   GLint t;
   GLint r;
   GLint q;
};
uint32_t
_mesa_unmarshal_MultiTexCoord4i(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord4i *cmd, const uint64_t *last)
{
   GLenum target = cmd->target;
   GLint s = cmd->s;
   GLint t = cmd->t;
   GLint r = cmd->r;
   GLint q = cmd->q;
   CALL_MultiTexCoord4i(ctx->CurrentServerDispatch, (target, s, t, r, q));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MultiTexCoord4i), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_MultiTexCoord4i(GLenum target, GLint s, GLint t, GLint r, GLint q)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexCoord4i);
   struct marshal_cmd_MultiTexCoord4i *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexCoord4i, cmd_size);
   cmd->target = target;
   cmd->s = s;
   cmd->t = t;
   cmd->r = r;
   cmd->q = q;
}


/* MultiTexCoord4iv: marshalled asynchronously */
struct marshal_cmd_MultiTexCoord4iv
{
   struct marshal_cmd_base cmd_base;
   GLenum target;
   GLint v[4];
};
uint32_t
_mesa_unmarshal_MultiTexCoord4iv(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord4iv *cmd, const uint64_t *last)
{
   GLenum target = cmd->target;
   const GLint * v = cmd->v;
   CALL_MultiTexCoord4iv(ctx->CurrentServerDispatch, (target, v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MultiTexCoord4iv), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_MultiTexCoord4iv(GLenum target, const GLint * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexCoord4iv);
   struct marshal_cmd_MultiTexCoord4iv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexCoord4iv, cmd_size);
   cmd->target = target;
   memcpy(cmd->v, v, 4 * sizeof(GLint));
}


/* MultiTexCoord4s: marshalled asynchronously */
struct marshal_cmd_MultiTexCoord4s
{
   struct marshal_cmd_base cmd_base;
   GLshort s;
   GLshort t;
   GLshort r;
   GLshort q;
   GLenum target;
};
uint32_t
_mesa_unmarshal_MultiTexCoord4s(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord4s *cmd, const uint64_t *last)
{
   GLenum target = cmd->target;
   GLshort s = cmd->s;
   GLshort t = cmd->t;
   GLshort r = cmd->r;
   GLshort q = cmd->q;
   CALL_MultiTexCoord4s(ctx->CurrentServerDispatch, (target, s, t, r, q));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MultiTexCoord4s), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_MultiTexCoord4s(GLenum target, GLshort s, GLshort t, GLshort r, GLshort q)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexCoord4s);
   struct marshal_cmd_MultiTexCoord4s *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexCoord4s, cmd_size);
   cmd->target = target;
   cmd->s = s;
   cmd->t = t;
   cmd->r = r;
   cmd->q = q;
}


/* MultiTexCoord4sv: marshalled asynchronously */
struct marshal_cmd_MultiTexCoord4sv
{
   struct marshal_cmd_base cmd_base;
   GLenum target;
   GLshort v[4];
};
uint32_t
_mesa_unmarshal_MultiTexCoord4sv(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord4sv *cmd, const uint64_t *last)
{
   GLenum target = cmd->target;
   const GLshort * v = cmd->v;
   CALL_MultiTexCoord4sv(ctx->CurrentServerDispatch, (target, v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MultiTexCoord4sv), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_MultiTexCoord4sv(GLenum target, const GLshort * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexCoord4sv);
   struct marshal_cmd_MultiTexCoord4sv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexCoord4sv, cmd_size);
   cmd->target = target;
   memcpy(cmd->v, v, 4 * sizeof(GLshort));
}


/* LoadTransposeMatrixf: marshalled asynchronously */
struct marshal_cmd_LoadTransposeMatrixf
{
   struct marshal_cmd_base cmd_base;
   GLfloat m[16];
};
uint32_t
_mesa_unmarshal_LoadTransposeMatrixf(struct gl_context *ctx, const struct marshal_cmd_LoadTransposeMatrixf *cmd, const uint64_t *last)
{
   const GLfloat * m = cmd->m;
   CALL_LoadTransposeMatrixf(ctx->CurrentServerDispatch, (m));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_LoadTransposeMatrixf), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_LoadTransposeMatrixf(const GLfloat * m)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_LoadTransposeMatrixf);
   struct marshal_cmd_LoadTransposeMatrixf *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_LoadTransposeMatrixf, cmd_size);
   memcpy(cmd->m, m, 16 * sizeof(GLfloat));
}


/* LoadTransposeMatrixd: marshalled asynchronously */
struct marshal_cmd_LoadTransposeMatrixd
{
   struct marshal_cmd_base cmd_base;
   GLdouble m[16];
};
uint32_t
_mesa_unmarshal_LoadTransposeMatrixd(struct gl_context *ctx, const struct marshal_cmd_LoadTransposeMatrixd *cmd, const uint64_t *last)
{
   const GLdouble * m = cmd->m;
   CALL_LoadTransposeMatrixd(ctx->CurrentServerDispatch, (m));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_LoadTransposeMatrixd), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_LoadTransposeMatrixd(const GLdouble * m)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_LoadTransposeMatrixd);
   struct marshal_cmd_LoadTransposeMatrixd *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_LoadTransposeMatrixd, cmd_size);
   memcpy(cmd->m, m, 16 * sizeof(GLdouble));
}


/* MultTransposeMatrixf: marshalled asynchronously */
struct marshal_cmd_MultTransposeMatrixf
{
   struct marshal_cmd_base cmd_base;
   GLfloat m[16];
};
uint32_t
_mesa_unmarshal_MultTransposeMatrixf(struct gl_context *ctx, const struct marshal_cmd_MultTransposeMatrixf *cmd, const uint64_t *last)
{
   const GLfloat * m = cmd->m;
   CALL_MultTransposeMatrixf(ctx->CurrentServerDispatch, (m));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MultTransposeMatrixf), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_MultTransposeMatrixf(const GLfloat * m)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultTransposeMatrixf);
   struct marshal_cmd_MultTransposeMatrixf *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultTransposeMatrixf, cmd_size);
   memcpy(cmd->m, m, 16 * sizeof(GLfloat));
}


/* MultTransposeMatrixd: marshalled asynchronously */
struct marshal_cmd_MultTransposeMatrixd
{
   struct marshal_cmd_base cmd_base;
   GLdouble m[16];
};
uint32_t
_mesa_unmarshal_MultTransposeMatrixd(struct gl_context *ctx, const struct marshal_cmd_MultTransposeMatrixd *cmd, const uint64_t *last)
{
   const GLdouble * m = cmd->m;
   CALL_MultTransposeMatrixd(ctx->CurrentServerDispatch, (m));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MultTransposeMatrixd), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_MultTransposeMatrixd(const GLdouble * m)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultTransposeMatrixd);
   struct marshal_cmd_MultTransposeMatrixd *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultTransposeMatrixd, cmd_size);
   memcpy(cmd->m, m, 16 * sizeof(GLdouble));
}


/* SampleCoverage: marshalled asynchronously */
struct marshal_cmd_SampleCoverage
{
   struct marshal_cmd_base cmd_base;
   GLboolean invert;
   GLclampf value;
};
uint32_t
_mesa_unmarshal_SampleCoverage(struct gl_context *ctx, const struct marshal_cmd_SampleCoverage *cmd, const uint64_t *last)
{
   GLclampf value = cmd->value;
   GLboolean invert = cmd->invert;
   CALL_SampleCoverage(ctx->CurrentServerDispatch, (value, invert));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_SampleCoverage), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_SampleCoverage(GLclampf value, GLboolean invert)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_SampleCoverage);
   struct marshal_cmd_SampleCoverage *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_SampleCoverage, cmd_size);
   cmd->value = value;
   cmd->invert = invert;
}


/* CompressedTexImage3D: marshalled asynchronously */
struct marshal_cmd_CompressedTexImage3D
{
   struct marshal_cmd_base cmd_base;
   GLenum target;
   GLint level;
   GLenum internalformat;
   GLsizei width;
   GLsizei height;
   GLsizei depth;
   GLint border;
   GLsizei imageSize;
   const GLvoid * data;
};
uint32_t
_mesa_unmarshal_CompressedTexImage3D(struct gl_context *ctx, const struct marshal_cmd_CompressedTexImage3D *cmd, const uint64_t *last)
{
   GLenum target = cmd->target;
   GLint level = cmd->level;
   GLenum internalformat = cmd->internalformat;
   GLsizei width = cmd->width;
   GLsizei height = cmd->height;
   GLsizei depth = cmd->depth;
   GLint border = cmd->border;
   GLsizei imageSize = cmd->imageSize;
   const GLvoid * data = cmd->data;
   CALL_CompressedTexImage3D(ctx->CurrentServerDispatch, (target, level, internalformat, width, height, depth, border, imageSize, data));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_CompressedTexImage3D), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_CompressedTexImage3D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const GLvoid * data)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_CompressedTexImage3D);
   struct marshal_cmd_CompressedTexImage3D *cmd;
   if (_mesa_glthread_has_no_unpack_buffer(ctx)) {
      _mesa_glthread_finish_before(ctx, "CompressedTexImage3D");
      CALL_CompressedTexImage3D(ctx->CurrentServerDispatch, (target, level, internalformat, width, height, depth, border, imageSize, data));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_CompressedTexImage3D, cmd_size);
   cmd->target = target;
   cmd->level = level;
   cmd->internalformat = internalformat;
   cmd->width = width;
   cmd->height = height;
   cmd->depth = depth;
   cmd->border = border;
   cmd->imageSize = imageSize;
   cmd->data = data;
}


/* CompressedTexImage2D: marshalled asynchronously */
struct marshal_cmd_CompressedTexImage2D
{
   struct marshal_cmd_base cmd_base;
   GLenum target;
   GLint level;
   GLenum internalformat;
   GLsizei width;
   GLsizei height;
   GLint border;
   GLsizei imageSize;
   const GLvoid * data;
};
uint32_t
_mesa_unmarshal_CompressedTexImage2D(struct gl_context *ctx, const struct marshal_cmd_CompressedTexImage2D *cmd, const uint64_t *last)
{
   GLenum target = cmd->target;
   GLint level = cmd->level;
   GLenum internalformat = cmd->internalformat;
   GLsizei width = cmd->width;
   GLsizei height = cmd->height;
   GLint border = cmd->border;
   GLsizei imageSize = cmd->imageSize;
   const GLvoid * data = cmd->data;
   CALL_CompressedTexImage2D(ctx->CurrentServerDispatch, (target, level, internalformat, width, height, border, imageSize, data));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_CompressedTexImage2D), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_CompressedTexImage2D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid * data)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_CompressedTexImage2D);
   struct marshal_cmd_CompressedTexImage2D *cmd;
   if (_mesa_glthread_has_no_unpack_buffer(ctx)) {
      _mesa_glthread_finish_before(ctx, "CompressedTexImage2D");
      CALL_CompressedTexImage2D(ctx->CurrentServerDispatch, (target, level, internalformat, width, height, border, imageSize, data));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_CompressedTexImage2D, cmd_size);
   cmd->target = target;
   cmd->level = level;
   cmd->internalformat = internalformat;
   cmd->width = width;
   cmd->height = height;
   cmd->border = border;
   cmd->imageSize = imageSize;
   cmd->data = data;
}


/* CompressedTexImage1D: marshalled asynchronously */
struct marshal_cmd_CompressedTexImage1D
{
   struct marshal_cmd_base cmd_base;
   GLenum target;
   GLint level;
   GLenum internalformat;
   GLsizei width;
   GLint border;
   GLsizei imageSize;
   const GLvoid * data;
};
uint32_t
_mesa_unmarshal_CompressedTexImage1D(struct gl_context *ctx, const struct marshal_cmd_CompressedTexImage1D *cmd, const uint64_t *last)
{
   GLenum target = cmd->target;
   GLint level = cmd->level;
   GLenum internalformat = cmd->internalformat;
   GLsizei width = cmd->width;
   GLint border = cmd->border;
   GLsizei imageSize = cmd->imageSize;
   const GLvoid * data = cmd->data;
   CALL_CompressedTexImage1D(ctx->CurrentServerDispatch, (target, level, internalformat, width, border, imageSize, data));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_CompressedTexImage1D), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_CompressedTexImage1D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei imageSize, const GLvoid * data)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_CompressedTexImage1D);
   struct marshal_cmd_CompressedTexImage1D *cmd;
   if (_mesa_glthread_has_no_unpack_buffer(ctx)) {
      _mesa_glthread_finish_before(ctx, "CompressedTexImage1D");
      CALL_CompressedTexImage1D(ctx->CurrentServerDispatch, (target, level, internalformat, width, border, imageSize, data));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_CompressedTexImage1D, cmd_size);
   cmd->target = target;
   cmd->level = level;
   cmd->internalformat = internalformat;
   cmd->width = width;
   cmd->border = border;
   cmd->imageSize = imageSize;
   cmd->data = data;
}


/* CompressedTexSubImage3D: marshalled asynchronously */
struct marshal_cmd_CompressedTexSubImage3D
{
   struct marshal_cmd_base cmd_base;
   GLenum target;
   GLint level;
   GLint xoffset;
   GLint yoffset;
   GLint zoffset;
   GLsizei width;
   GLsizei height;
   GLsizei depth;
   GLenum format;
   GLsizei imageSize;
   const GLvoid * data;
};
uint32_t
_mesa_unmarshal_CompressedTexSubImage3D(struct gl_context *ctx, const struct marshal_cmd_CompressedTexSubImage3D *cmd, const uint64_t *last)
{
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
   CALL_CompressedTexSubImage3D(ctx->CurrentServerDispatch, (target, level, xoffset, yoffset, zoffset, width, height, depth, format, imageSize, data));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_CompressedTexSubImage3D), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_CompressedTexSubImage3D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const GLvoid * data)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_CompressedTexSubImage3D);
   struct marshal_cmd_CompressedTexSubImage3D *cmd;
   if (_mesa_glthread_has_no_unpack_buffer(ctx)) {
      _mesa_glthread_finish_before(ctx, "CompressedTexSubImage3D");
      CALL_CompressedTexSubImage3D(ctx->CurrentServerDispatch, (target, level, xoffset, yoffset, zoffset, width, height, depth, format, imageSize, data));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_CompressedTexSubImage3D, cmd_size);
   cmd->target = target;
   cmd->level = level;
   cmd->xoffset = xoffset;
   cmd->yoffset = yoffset;
   cmd->zoffset = zoffset;
   cmd->width = width;
   cmd->height = height;
   cmd->depth = depth;
   cmd->format = format;
   cmd->imageSize = imageSize;
   cmd->data = data;
}


/* CompressedTexSubImage2D: marshalled asynchronously */
struct marshal_cmd_CompressedTexSubImage2D
{
   struct marshal_cmd_base cmd_base;
   GLenum target;
   GLint level;
   GLint xoffset;
   GLint yoffset;
   GLsizei width;
   GLsizei height;
   GLenum format;
   GLsizei imageSize;
   const GLvoid * data;
};
uint32_t
_mesa_unmarshal_CompressedTexSubImage2D(struct gl_context *ctx, const struct marshal_cmd_CompressedTexSubImage2D *cmd, const uint64_t *last)
{
   GLenum target = cmd->target;
   GLint level = cmd->level;
   GLint xoffset = cmd->xoffset;
   GLint yoffset = cmd->yoffset;
   GLsizei width = cmd->width;
   GLsizei height = cmd->height;
   GLenum format = cmd->format;
   GLsizei imageSize = cmd->imageSize;
   const GLvoid * data = cmd->data;
   CALL_CompressedTexSubImage2D(ctx->CurrentServerDispatch, (target, level, xoffset, yoffset, width, height, format, imageSize, data));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_CompressedTexSubImage2D), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_CompressedTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const GLvoid * data)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_CompressedTexSubImage2D);
   struct marshal_cmd_CompressedTexSubImage2D *cmd;
   if (_mesa_glthread_has_no_unpack_buffer(ctx)) {
      _mesa_glthread_finish_before(ctx, "CompressedTexSubImage2D");
      CALL_CompressedTexSubImage2D(ctx->CurrentServerDispatch, (target, level, xoffset, yoffset, width, height, format, imageSize, data));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_CompressedTexSubImage2D, cmd_size);
   cmd->target = target;
   cmd->level = level;
   cmd->xoffset = xoffset;
   cmd->yoffset = yoffset;
   cmd->width = width;
   cmd->height = height;
   cmd->format = format;
   cmd->imageSize = imageSize;
   cmd->data = data;
}


/* CompressedTexSubImage1D: marshalled asynchronously */
struct marshal_cmd_CompressedTexSubImage1D
{
   struct marshal_cmd_base cmd_base;
   GLenum target;
   GLint level;
   GLint xoffset;
   GLsizei width;
   GLenum format;
   GLsizei imageSize;
   const GLvoid * data;
};
uint32_t
_mesa_unmarshal_CompressedTexSubImage1D(struct gl_context *ctx, const struct marshal_cmd_CompressedTexSubImage1D *cmd, const uint64_t *last)
{
   GLenum target = cmd->target;
   GLint level = cmd->level;
   GLint xoffset = cmd->xoffset;
   GLsizei width = cmd->width;
   GLenum format = cmd->format;
   GLsizei imageSize = cmd->imageSize;
   const GLvoid * data = cmd->data;
   CALL_CompressedTexSubImage1D(ctx->CurrentServerDispatch, (target, level, xoffset, width, format, imageSize, data));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_CompressedTexSubImage1D), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_CompressedTexSubImage1D(GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const GLvoid * data)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_CompressedTexSubImage1D);
   struct marshal_cmd_CompressedTexSubImage1D *cmd;
   if (_mesa_glthread_has_no_unpack_buffer(ctx)) {
      _mesa_glthread_finish_before(ctx, "CompressedTexSubImage1D");
      CALL_CompressedTexSubImage1D(ctx->CurrentServerDispatch, (target, level, xoffset, width, format, imageSize, data));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_CompressedTexSubImage1D, cmd_size);
   cmd->target = target;
   cmd->level = level;
   cmd->xoffset = xoffset;
   cmd->width = width;
   cmd->format = format;
   cmd->imageSize = imageSize;
   cmd->data = data;
}


/* GetCompressedTexImage: marshalled asynchronously */
struct marshal_cmd_GetCompressedTexImage
{
   struct marshal_cmd_base cmd_base;
   GLenum target;
   GLint level;
   GLvoid * img;
};
uint32_t
_mesa_unmarshal_GetCompressedTexImage(struct gl_context *ctx, const struct marshal_cmd_GetCompressedTexImage *cmd, const uint64_t *last)
{
   GLenum target = cmd->target;
   GLint level = cmd->level;
   GLvoid * img = cmd->img;
   CALL_GetCompressedTexImage(ctx->CurrentServerDispatch, (target, level, img));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_GetCompressedTexImage), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_GetCompressedTexImage(GLenum target, GLint level, GLvoid * img)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_GetCompressedTexImage);
   struct marshal_cmd_GetCompressedTexImage *cmd;
   if (_mesa_glthread_has_no_pack_buffer(ctx)) {
      _mesa_glthread_finish_before(ctx, "GetCompressedTexImage");
      CALL_GetCompressedTexImage(ctx->CurrentServerDispatch, (target, level, img));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_GetCompressedTexImage, cmd_size);
   cmd->target = target;
   cmd->level = level;
   cmd->img = img;
}


/* BlendFuncSeparate: marshalled asynchronously */
struct marshal_cmd_BlendFuncSeparate
{
   struct marshal_cmd_base cmd_base;
   GLenum sfactorRGB;
   GLenum dfactorRGB;
   GLenum sfactorAlpha;
   GLenum dfactorAlpha;
};
uint32_t
_mesa_unmarshal_BlendFuncSeparate(struct gl_context *ctx, const struct marshal_cmd_BlendFuncSeparate *cmd, const uint64_t *last)
{
   GLenum sfactorRGB = cmd->sfactorRGB;
   GLenum dfactorRGB = cmd->dfactorRGB;
   GLenum sfactorAlpha = cmd->sfactorAlpha;
   GLenum dfactorAlpha = cmd->dfactorAlpha;
   CALL_BlendFuncSeparate(ctx->CurrentServerDispatch, (sfactorRGB, dfactorRGB, sfactorAlpha, dfactorAlpha));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_BlendFuncSeparate), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_BlendFuncSeparate(GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_BlendFuncSeparate);
   struct marshal_cmd_BlendFuncSeparate *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_BlendFuncSeparate, cmd_size);
   cmd->sfactorRGB = sfactorRGB;
   cmd->dfactorRGB = dfactorRGB;
   cmd->sfactorAlpha = sfactorAlpha;
   cmd->dfactorAlpha = dfactorAlpha;
}


/* FogCoordfEXT: marshalled asynchronously */
struct marshal_cmd_FogCoordfEXT
{
   struct marshal_cmd_base cmd_base;
   GLfloat coord;
};
uint32_t
_mesa_unmarshal_FogCoordfEXT(struct gl_context *ctx, const struct marshal_cmd_FogCoordfEXT *cmd, const uint64_t *last)
{
   GLfloat coord = cmd->coord;
   CALL_FogCoordfEXT(ctx->CurrentServerDispatch, (coord));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_FogCoordfEXT), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_FogCoordfEXT(GLfloat coord)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_FogCoordfEXT);
   struct marshal_cmd_FogCoordfEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_FogCoordfEXT, cmd_size);
   cmd->coord = coord;
}


/* FogCoordfvEXT: marshalled asynchronously */
struct marshal_cmd_FogCoordfvEXT
{
   struct marshal_cmd_base cmd_base;
   GLfloat coord[1];
};
uint32_t
_mesa_unmarshal_FogCoordfvEXT(struct gl_context *ctx, const struct marshal_cmd_FogCoordfvEXT *cmd, const uint64_t *last)
{
   const GLfloat * coord = cmd->coord;
   CALL_FogCoordfvEXT(ctx->CurrentServerDispatch, (coord));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_FogCoordfvEXT), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_FogCoordfvEXT(const GLfloat * coord)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_FogCoordfvEXT);
   struct marshal_cmd_FogCoordfvEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_FogCoordfvEXT, cmd_size);
   memcpy(cmd->coord, coord, 1 * sizeof(GLfloat));
}


/* FogCoordd: marshalled asynchronously */
struct marshal_cmd_FogCoordd
{
   struct marshal_cmd_base cmd_base;
   GLdouble coord;
};
uint32_t
_mesa_unmarshal_FogCoordd(struct gl_context *ctx, const struct marshal_cmd_FogCoordd *cmd, const uint64_t *last)
{
   GLdouble coord = cmd->coord;
   CALL_FogCoordd(ctx->CurrentServerDispatch, (coord));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_FogCoordd), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_FogCoordd(GLdouble coord)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_FogCoordd);
   struct marshal_cmd_FogCoordd *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_FogCoordd, cmd_size);
   cmd->coord = coord;
}


/* FogCoorddv: marshalled asynchronously */
struct marshal_cmd_FogCoorddv
{
   struct marshal_cmd_base cmd_base;
   GLdouble coord[1];
};
uint32_t
_mesa_unmarshal_FogCoorddv(struct gl_context *ctx, const struct marshal_cmd_FogCoorddv *cmd, const uint64_t *last)
{
   const GLdouble * coord = cmd->coord;
   CALL_FogCoorddv(ctx->CurrentServerDispatch, (coord));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_FogCoorddv), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_FogCoorddv(const GLdouble * coord)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_FogCoorddv);
   struct marshal_cmd_FogCoorddv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_FogCoorddv, cmd_size);
   memcpy(cmd->coord, coord, 1 * sizeof(GLdouble));
}


/* FogCoordPointer: marshalled asynchronously */
struct marshal_cmd_FogCoordPointer
{
   struct marshal_cmd_base cmd_base;
   GLenum type;
   GLsizei stride;
   const GLvoid * pointer;
};
uint32_t
_mesa_unmarshal_FogCoordPointer(struct gl_context *ctx, const struct marshal_cmd_FogCoordPointer *cmd, const uint64_t *last)
{
   GLenum type = cmd->type;
   GLsizei stride = cmd->stride;
   const GLvoid * pointer = cmd->pointer;
   CALL_FogCoordPointer(ctx->CurrentServerDispatch, (type, stride, pointer));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_FogCoordPointer), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_FogCoordPointer(GLenum type, GLsizei stride, const GLvoid * pointer)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_FogCoordPointer);
   struct marshal_cmd_FogCoordPointer *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_FogCoordPointer, cmd_size);
   cmd->type = type;
   cmd->stride = stride;
   cmd->pointer = pointer;
   if (COMPAT) _mesa_glthread_AttribPointer(ctx, VERT_ATTRIB_FOG, 1, type, stride, pointer);
}


/* PointParameterf: marshalled asynchronously */
struct marshal_cmd_PointParameterf
{
   struct marshal_cmd_base cmd_base;
   GLenum pname;
   GLfloat param;
};
uint32_t
_mesa_unmarshal_PointParameterf(struct gl_context *ctx, const struct marshal_cmd_PointParameterf *cmd, const uint64_t *last)
{
   GLenum pname = cmd->pname;
   GLfloat param = cmd->param;
   CALL_PointParameterf(ctx->CurrentServerDispatch, (pname, param));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_PointParameterf), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_PointParameterf(GLenum pname, GLfloat param)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_PointParameterf);
   struct marshal_cmd_PointParameterf *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_PointParameterf, cmd_size);
   cmd->pname = pname;
   cmd->param = param;
}


/* PointParameterfv: marshalled asynchronously */
struct marshal_cmd_PointParameterfv
{
   struct marshal_cmd_base cmd_base;
   GLenum pname;
   /* Next safe_mul(_mesa_point_param_enum_to_count(pname), 1 * sizeof(GLfloat)) bytes are GLfloat params[None] */
};
uint32_t
_mesa_unmarshal_PointParameterfv(struct gl_context *ctx, const struct marshal_cmd_PointParameterfv *cmd, const uint64_t *last)
{
   GLenum pname = cmd->pname;
   GLfloat * params;
   const char *variable_data = (const char *) (cmd + 1);
   params = (GLfloat *) variable_data;
   CALL_PointParameterfv(ctx->CurrentServerDispatch, (pname, params));
   return cmd->cmd_base.cmd_size;
}
void GLAPIENTRY
_mesa_marshal_PointParameterfv(GLenum pname, const GLfloat * params)
{
   GET_CURRENT_CONTEXT(ctx);
   int params_size = safe_mul(_mesa_point_param_enum_to_count(pname), 1 * sizeof(GLfloat));
   int cmd_size = sizeof(struct marshal_cmd_PointParameterfv) + params_size;
   struct marshal_cmd_PointParameterfv *cmd;
   if (unlikely(params_size < 0 || (params_size > 0 && !params) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "PointParameterfv");
      CALL_PointParameterfv(ctx->CurrentServerDispatch, (pname, params));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_PointParameterfv, cmd_size);
   cmd->pname = pname;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, params, params_size);
}


/* PointParameteri: marshalled asynchronously */
struct marshal_cmd_PointParameteri
{
   struct marshal_cmd_base cmd_base;
   GLenum pname;
   GLint param;
};
uint32_t
_mesa_unmarshal_PointParameteri(struct gl_context *ctx, const struct marshal_cmd_PointParameteri *cmd, const uint64_t *last)
{
   GLenum pname = cmd->pname;
   GLint param = cmd->param;
   CALL_PointParameteri(ctx->CurrentServerDispatch, (pname, param));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_PointParameteri), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_PointParameteri(GLenum pname, GLint param)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_PointParameteri);
   struct marshal_cmd_PointParameteri *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_PointParameteri, cmd_size);
   cmd->pname = pname;
   cmd->param = param;
}


/* PointParameteriv: marshalled asynchronously */
struct marshal_cmd_PointParameteriv
{
   struct marshal_cmd_base cmd_base;
   GLenum pname;
   /* Next safe_mul(_mesa_point_param_enum_to_count(pname), 1 * sizeof(GLint)) bytes are GLint params[None] */
};
uint32_t
_mesa_unmarshal_PointParameteriv(struct gl_context *ctx, const struct marshal_cmd_PointParameteriv *cmd, const uint64_t *last)
{
   GLenum pname = cmd->pname;
   GLint * params;
   const char *variable_data = (const char *) (cmd + 1);
   params = (GLint *) variable_data;
   CALL_PointParameteriv(ctx->CurrentServerDispatch, (pname, params));
   return cmd->cmd_base.cmd_size;
}
void GLAPIENTRY
_mesa_marshal_PointParameteriv(GLenum pname, const GLint * params)
{
   GET_CURRENT_CONTEXT(ctx);
   int params_size = safe_mul(_mesa_point_param_enum_to_count(pname), 1 * sizeof(GLint));
   int cmd_size = sizeof(struct marshal_cmd_PointParameteriv) + params_size;
   struct marshal_cmd_PointParameteriv *cmd;
   if (unlikely(params_size < 0 || (params_size > 0 && !params) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "PointParameteriv");
      CALL_PointParameteriv(ctx->CurrentServerDispatch, (pname, params));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_PointParameteriv, cmd_size);
   cmd->pname = pname;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, params, params_size);
}


/* SecondaryColor3b: marshalled asynchronously */
struct marshal_cmd_SecondaryColor3b
{
   struct marshal_cmd_base cmd_base;
   GLbyte red;
   GLbyte green;
   GLbyte blue;
};
uint32_t
_mesa_unmarshal_SecondaryColor3b(struct gl_context *ctx, const struct marshal_cmd_SecondaryColor3b *cmd, const uint64_t *last)
{
   GLbyte red = cmd->red;
   GLbyte green = cmd->green;
   GLbyte blue = cmd->blue;
   CALL_SecondaryColor3b(ctx->CurrentServerDispatch, (red, green, blue));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_SecondaryColor3b), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_SecondaryColor3b(GLbyte red, GLbyte green, GLbyte blue)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_SecondaryColor3b);
   struct marshal_cmd_SecondaryColor3b *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_SecondaryColor3b, cmd_size);
   cmd->red = red;
   cmd->green = green;
   cmd->blue = blue;
}


/* SecondaryColor3bv: marshalled asynchronously */
struct marshal_cmd_SecondaryColor3bv
{
   struct marshal_cmd_base cmd_base;
   GLbyte v[3];
};
uint32_t
_mesa_unmarshal_SecondaryColor3bv(struct gl_context *ctx, const struct marshal_cmd_SecondaryColor3bv *cmd, const uint64_t *last)
{
   const GLbyte * v = cmd->v;
   CALL_SecondaryColor3bv(ctx->CurrentServerDispatch, (v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_SecondaryColor3bv), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_SecondaryColor3bv(const GLbyte * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_SecondaryColor3bv);
   struct marshal_cmd_SecondaryColor3bv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_SecondaryColor3bv, cmd_size);
   memcpy(cmd->v, v, 3 * sizeof(GLbyte));
}


/* SecondaryColor3d: marshalled asynchronously */
struct marshal_cmd_SecondaryColor3d
{
   struct marshal_cmd_base cmd_base;
   GLdouble red;
   GLdouble green;
   GLdouble blue;
};
uint32_t
_mesa_unmarshal_SecondaryColor3d(struct gl_context *ctx, const struct marshal_cmd_SecondaryColor3d *cmd, const uint64_t *last)
{
   GLdouble red = cmd->red;
   GLdouble green = cmd->green;
   GLdouble blue = cmd->blue;
   CALL_SecondaryColor3d(ctx->CurrentServerDispatch, (red, green, blue));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_SecondaryColor3d), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_SecondaryColor3d(GLdouble red, GLdouble green, GLdouble blue)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_SecondaryColor3d);
   struct marshal_cmd_SecondaryColor3d *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_SecondaryColor3d, cmd_size);
   cmd->red = red;
   cmd->green = green;
   cmd->blue = blue;
}


/* SecondaryColor3dv: marshalled asynchronously */
struct marshal_cmd_SecondaryColor3dv
{
   struct marshal_cmd_base cmd_base;
   GLdouble v[3];
};
uint32_t
_mesa_unmarshal_SecondaryColor3dv(struct gl_context *ctx, const struct marshal_cmd_SecondaryColor3dv *cmd, const uint64_t *last)
{
   const GLdouble * v = cmd->v;
   CALL_SecondaryColor3dv(ctx->CurrentServerDispatch, (v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_SecondaryColor3dv), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_SecondaryColor3dv(const GLdouble * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_SecondaryColor3dv);
   struct marshal_cmd_SecondaryColor3dv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_SecondaryColor3dv, cmd_size);
   memcpy(cmd->v, v, 3 * sizeof(GLdouble));
}


/* SecondaryColor3fEXT: marshalled asynchronously */
struct marshal_cmd_SecondaryColor3fEXT
{
   struct marshal_cmd_base cmd_base;
   GLfloat red;
   GLfloat green;
   GLfloat blue;
};
uint32_t
_mesa_unmarshal_SecondaryColor3fEXT(struct gl_context *ctx, const struct marshal_cmd_SecondaryColor3fEXT *cmd, const uint64_t *last)
{
   GLfloat red = cmd->red;
   GLfloat green = cmd->green;
   GLfloat blue = cmd->blue;
   CALL_SecondaryColor3fEXT(ctx->CurrentServerDispatch, (red, green, blue));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_SecondaryColor3fEXT), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_SecondaryColor3fEXT(GLfloat red, GLfloat green, GLfloat blue)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_SecondaryColor3fEXT);
   struct marshal_cmd_SecondaryColor3fEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_SecondaryColor3fEXT, cmd_size);
   cmd->red = red;
   cmd->green = green;
   cmd->blue = blue;
}


/* SecondaryColor3fvEXT: marshalled asynchronously */
struct marshal_cmd_SecondaryColor3fvEXT
{
   struct marshal_cmd_base cmd_base;
   GLfloat v[3];
};
uint32_t
_mesa_unmarshal_SecondaryColor3fvEXT(struct gl_context *ctx, const struct marshal_cmd_SecondaryColor3fvEXT *cmd, const uint64_t *last)
{
   const GLfloat * v = cmd->v;
   CALL_SecondaryColor3fvEXT(ctx->CurrentServerDispatch, (v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_SecondaryColor3fvEXT), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_SecondaryColor3fvEXT(const GLfloat * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_SecondaryColor3fvEXT);
   struct marshal_cmd_SecondaryColor3fvEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_SecondaryColor3fvEXT, cmd_size);
   memcpy(cmd->v, v, 3 * sizeof(GLfloat));
}


/* SecondaryColor3i: marshalled asynchronously */
struct marshal_cmd_SecondaryColor3i
{
   struct marshal_cmd_base cmd_base;
   GLint red;
   GLint green;
   GLint blue;
};
uint32_t
_mesa_unmarshal_SecondaryColor3i(struct gl_context *ctx, const struct marshal_cmd_SecondaryColor3i *cmd, const uint64_t *last)
{
   GLint red = cmd->red;
   GLint green = cmd->green;
   GLint blue = cmd->blue;
   CALL_SecondaryColor3i(ctx->CurrentServerDispatch, (red, green, blue));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_SecondaryColor3i), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_SecondaryColor3i(GLint red, GLint green, GLint blue)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_SecondaryColor3i);
   struct marshal_cmd_SecondaryColor3i *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_SecondaryColor3i, cmd_size);
   cmd->red = red;
   cmd->green = green;
   cmd->blue = blue;
}


/* SecondaryColor3iv: marshalled asynchronously */
struct marshal_cmd_SecondaryColor3iv
{
   struct marshal_cmd_base cmd_base;
   GLint v[3];
};
uint32_t
_mesa_unmarshal_SecondaryColor3iv(struct gl_context *ctx, const struct marshal_cmd_SecondaryColor3iv *cmd, const uint64_t *last)
{
   const GLint * v = cmd->v;
   CALL_SecondaryColor3iv(ctx->CurrentServerDispatch, (v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_SecondaryColor3iv), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_SecondaryColor3iv(const GLint * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_SecondaryColor3iv);
   struct marshal_cmd_SecondaryColor3iv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_SecondaryColor3iv, cmd_size);
   memcpy(cmd->v, v, 3 * sizeof(GLint));
}


/* SecondaryColor3s: marshalled asynchronously */
struct marshal_cmd_SecondaryColor3s
{
   struct marshal_cmd_base cmd_base;
   GLshort red;
   GLshort green;
   GLshort blue;
};
uint32_t
_mesa_unmarshal_SecondaryColor3s(struct gl_context *ctx, const struct marshal_cmd_SecondaryColor3s *cmd, const uint64_t *last)
{
   GLshort red = cmd->red;
   GLshort green = cmd->green;
   GLshort blue = cmd->blue;
   CALL_SecondaryColor3s(ctx->CurrentServerDispatch, (red, green, blue));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_SecondaryColor3s), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_SecondaryColor3s(GLshort red, GLshort green, GLshort blue)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_SecondaryColor3s);
   struct marshal_cmd_SecondaryColor3s *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_SecondaryColor3s, cmd_size);
   cmd->red = red;
   cmd->green = green;
   cmd->blue = blue;
}


/* SecondaryColor3sv: marshalled asynchronously */
struct marshal_cmd_SecondaryColor3sv
{
   struct marshal_cmd_base cmd_base;
   GLshort v[3];
};
uint32_t
_mesa_unmarshal_SecondaryColor3sv(struct gl_context *ctx, const struct marshal_cmd_SecondaryColor3sv *cmd, const uint64_t *last)
{
   const GLshort * v = cmd->v;
   CALL_SecondaryColor3sv(ctx->CurrentServerDispatch, (v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_SecondaryColor3sv), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_SecondaryColor3sv(const GLshort * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_SecondaryColor3sv);
   struct marshal_cmd_SecondaryColor3sv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_SecondaryColor3sv, cmd_size);
   memcpy(cmd->v, v, 3 * sizeof(GLshort));
}


/* SecondaryColor3ub: marshalled asynchronously */
struct marshal_cmd_SecondaryColor3ub
{
   struct marshal_cmd_base cmd_base;
   GLubyte red;
   GLubyte green;
   GLubyte blue;
};
uint32_t
_mesa_unmarshal_SecondaryColor3ub(struct gl_context *ctx, const struct marshal_cmd_SecondaryColor3ub *cmd, const uint64_t *last)
{
   GLubyte red = cmd->red;
   GLubyte green = cmd->green;
   GLubyte blue = cmd->blue;
   CALL_SecondaryColor3ub(ctx->CurrentServerDispatch, (red, green, blue));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_SecondaryColor3ub), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_SecondaryColor3ub(GLubyte red, GLubyte green, GLubyte blue)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_SecondaryColor3ub);
   struct marshal_cmd_SecondaryColor3ub *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_SecondaryColor3ub, cmd_size);
   cmd->red = red;
   cmd->green = green;
   cmd->blue = blue;
}


/* SecondaryColor3ubv: marshalled asynchronously */
struct marshal_cmd_SecondaryColor3ubv
{
   struct marshal_cmd_base cmd_base;
   GLubyte v[3];
};
uint32_t
_mesa_unmarshal_SecondaryColor3ubv(struct gl_context *ctx, const struct marshal_cmd_SecondaryColor3ubv *cmd, const uint64_t *last)
{
   const GLubyte * v = cmd->v;
   CALL_SecondaryColor3ubv(ctx->CurrentServerDispatch, (v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_SecondaryColor3ubv), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_SecondaryColor3ubv(const GLubyte * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_SecondaryColor3ubv);
   struct marshal_cmd_SecondaryColor3ubv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_SecondaryColor3ubv, cmd_size);
   memcpy(cmd->v, v, 3 * sizeof(GLubyte));
}


/* SecondaryColor3ui: marshalled asynchronously */
struct marshal_cmd_SecondaryColor3ui
{
   struct marshal_cmd_base cmd_base;
   GLuint red;
   GLuint green;
   GLuint blue;
};
uint32_t
_mesa_unmarshal_SecondaryColor3ui(struct gl_context *ctx, const struct marshal_cmd_SecondaryColor3ui *cmd, const uint64_t *last)
{
   GLuint red = cmd->red;
   GLuint green = cmd->green;
   GLuint blue = cmd->blue;
   CALL_SecondaryColor3ui(ctx->CurrentServerDispatch, (red, green, blue));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_SecondaryColor3ui), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_SecondaryColor3ui(GLuint red, GLuint green, GLuint blue)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_SecondaryColor3ui);
   struct marshal_cmd_SecondaryColor3ui *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_SecondaryColor3ui, cmd_size);
   cmd->red = red;
   cmd->green = green;
   cmd->blue = blue;
}


/* SecondaryColor3uiv: marshalled asynchronously */
struct marshal_cmd_SecondaryColor3uiv
{
   struct marshal_cmd_base cmd_base;
   GLuint v[3];
};
uint32_t
_mesa_unmarshal_SecondaryColor3uiv(struct gl_context *ctx, const struct marshal_cmd_SecondaryColor3uiv *cmd, const uint64_t *last)
{
   const GLuint * v = cmd->v;
   CALL_SecondaryColor3uiv(ctx->CurrentServerDispatch, (v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_SecondaryColor3uiv), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_SecondaryColor3uiv(const GLuint * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_SecondaryColor3uiv);
   struct marshal_cmd_SecondaryColor3uiv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_SecondaryColor3uiv, cmd_size);
   memcpy(cmd->v, v, 3 * sizeof(GLuint));
}


/* SecondaryColor3us: marshalled asynchronously */
struct marshal_cmd_SecondaryColor3us
{
   struct marshal_cmd_base cmd_base;
   GLushort red;
   GLushort green;
   GLushort blue;
};
uint32_t
_mesa_unmarshal_SecondaryColor3us(struct gl_context *ctx, const struct marshal_cmd_SecondaryColor3us *cmd, const uint64_t *last)
{
   GLushort red = cmd->red;
   GLushort green = cmd->green;
   GLushort blue = cmd->blue;
   CALL_SecondaryColor3us(ctx->CurrentServerDispatch, (red, green, blue));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_SecondaryColor3us), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_SecondaryColor3us(GLushort red, GLushort green, GLushort blue)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_SecondaryColor3us);
   struct marshal_cmd_SecondaryColor3us *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_SecondaryColor3us, cmd_size);
   cmd->red = red;
   cmd->green = green;
   cmd->blue = blue;
}


/* SecondaryColor3usv: marshalled asynchronously */
struct marshal_cmd_SecondaryColor3usv
{
   struct marshal_cmd_base cmd_base;
   GLushort v[3];
};
uint32_t
_mesa_unmarshal_SecondaryColor3usv(struct gl_context *ctx, const struct marshal_cmd_SecondaryColor3usv *cmd, const uint64_t *last)
{
   const GLushort * v = cmd->v;
   CALL_SecondaryColor3usv(ctx->CurrentServerDispatch, (v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_SecondaryColor3usv), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_SecondaryColor3usv(const GLushort * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_SecondaryColor3usv);
   struct marshal_cmd_SecondaryColor3usv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_SecondaryColor3usv, cmd_size);
   memcpy(cmd->v, v, 3 * sizeof(GLushort));
}


/* SecondaryColorPointer: marshalled asynchronously */
struct marshal_cmd_SecondaryColorPointer
{
   struct marshal_cmd_base cmd_base;
   GLint size;
   GLenum type;
   GLsizei stride;
   const GLvoid * pointer;
};
uint32_t
_mesa_unmarshal_SecondaryColorPointer(struct gl_context *ctx, const struct marshal_cmd_SecondaryColorPointer *cmd, const uint64_t *last)
{
   GLint size = cmd->size;
   GLenum type = cmd->type;
   GLsizei stride = cmd->stride;
   const GLvoid * pointer = cmd->pointer;
   CALL_SecondaryColorPointer(ctx->CurrentServerDispatch, (size, type, stride, pointer));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_SecondaryColorPointer), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_SecondaryColorPointer(GLint size, GLenum type, GLsizei stride, const GLvoid * pointer)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_SecondaryColorPointer);
   struct marshal_cmd_SecondaryColorPointer *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_SecondaryColorPointer, cmd_size);
   cmd->size = size;
   cmd->type = type;
   cmd->stride = stride;
   cmd->pointer = pointer;
   if (COMPAT) _mesa_glthread_AttribPointer(ctx, VERT_ATTRIB_COLOR1, size, type, stride, pointer);
}


/* WindowPos2d: marshalled asynchronously */
struct marshal_cmd_WindowPos2d
{
   struct marshal_cmd_base cmd_base;
   GLdouble x;
   GLdouble y;
};
uint32_t
_mesa_unmarshal_WindowPos2d(struct gl_context *ctx, const struct marshal_cmd_WindowPos2d *cmd, const uint64_t *last)
{
   GLdouble x = cmd->x;
   GLdouble y = cmd->y;
   CALL_WindowPos2d(ctx->CurrentServerDispatch, (x, y));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_WindowPos2d), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_WindowPos2d(GLdouble x, GLdouble y)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_WindowPos2d);
   struct marshal_cmd_WindowPos2d *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_WindowPos2d, cmd_size);
   cmd->x = x;
   cmd->y = y;
}


/* WindowPos2dv: marshalled asynchronously */
struct marshal_cmd_WindowPos2dv
{
   struct marshal_cmd_base cmd_base;
   GLdouble v[2];
};
uint32_t
_mesa_unmarshal_WindowPos2dv(struct gl_context *ctx, const struct marshal_cmd_WindowPos2dv *cmd, const uint64_t *last)
{
   const GLdouble * v = cmd->v;
   CALL_WindowPos2dv(ctx->CurrentServerDispatch, (v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_WindowPos2dv), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_WindowPos2dv(const GLdouble * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_WindowPos2dv);
   struct marshal_cmd_WindowPos2dv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_WindowPos2dv, cmd_size);
   memcpy(cmd->v, v, 2 * sizeof(GLdouble));
}


/* WindowPos2f: marshalled asynchronously */
struct marshal_cmd_WindowPos2f
{
   struct marshal_cmd_base cmd_base;
   GLfloat x;
   GLfloat y;
};
uint32_t
_mesa_unmarshal_WindowPos2f(struct gl_context *ctx, const struct marshal_cmd_WindowPos2f *cmd, const uint64_t *last)
{
   GLfloat x = cmd->x;
   GLfloat y = cmd->y;
   CALL_WindowPos2f(ctx->CurrentServerDispatch, (x, y));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_WindowPos2f), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_WindowPos2f(GLfloat x, GLfloat y)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_WindowPos2f);
   struct marshal_cmd_WindowPos2f *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_WindowPos2f, cmd_size);
   cmd->x = x;
   cmd->y = y;
}


/* WindowPos2fv: marshalled asynchronously */
struct marshal_cmd_WindowPos2fv
{
   struct marshal_cmd_base cmd_base;
   GLfloat v[2];
};
uint32_t
_mesa_unmarshal_WindowPos2fv(struct gl_context *ctx, const struct marshal_cmd_WindowPos2fv *cmd, const uint64_t *last)
{
   const GLfloat * v = cmd->v;
   CALL_WindowPos2fv(ctx->CurrentServerDispatch, (v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_WindowPos2fv), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_WindowPos2fv(const GLfloat * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_WindowPos2fv);
   struct marshal_cmd_WindowPos2fv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_WindowPos2fv, cmd_size);
   memcpy(cmd->v, v, 2 * sizeof(GLfloat));
}


/* WindowPos2i: marshalled asynchronously */
struct marshal_cmd_WindowPos2i
{
   struct marshal_cmd_base cmd_base;
   GLint x;
   GLint y;
};
uint32_t
_mesa_unmarshal_WindowPos2i(struct gl_context *ctx, const struct marshal_cmd_WindowPos2i *cmd, const uint64_t *last)
{
   GLint x = cmd->x;
   GLint y = cmd->y;
   CALL_WindowPos2i(ctx->CurrentServerDispatch, (x, y));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_WindowPos2i), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_WindowPos2i(GLint x, GLint y)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_WindowPos2i);
   struct marshal_cmd_WindowPos2i *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_WindowPos2i, cmd_size);
   cmd->x = x;
   cmd->y = y;
}


/* WindowPos2iv: marshalled asynchronously */
struct marshal_cmd_WindowPos2iv
{
   struct marshal_cmd_base cmd_base;
   GLint v[2];
};
uint32_t
_mesa_unmarshal_WindowPos2iv(struct gl_context *ctx, const struct marshal_cmd_WindowPos2iv *cmd, const uint64_t *last)
{
   const GLint * v = cmd->v;
   CALL_WindowPos2iv(ctx->CurrentServerDispatch, (v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_WindowPos2iv), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_WindowPos2iv(const GLint * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_WindowPos2iv);
   struct marshal_cmd_WindowPos2iv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_WindowPos2iv, cmd_size);
   memcpy(cmd->v, v, 2 * sizeof(GLint));
}


/* WindowPos2s: marshalled asynchronously */
struct marshal_cmd_WindowPos2s
{
   struct marshal_cmd_base cmd_base;
   GLshort x;
   GLshort y;
};
uint32_t
_mesa_unmarshal_WindowPos2s(struct gl_context *ctx, const struct marshal_cmd_WindowPos2s *cmd, const uint64_t *last)
{
   GLshort x = cmd->x;
   GLshort y = cmd->y;
   CALL_WindowPos2s(ctx->CurrentServerDispatch, (x, y));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_WindowPos2s), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_WindowPos2s(GLshort x, GLshort y)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_WindowPos2s);
   struct marshal_cmd_WindowPos2s *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_WindowPos2s, cmd_size);
   cmd->x = x;
   cmd->y = y;
}


/* WindowPos2sv: marshalled asynchronously */
struct marshal_cmd_WindowPos2sv
{
   struct marshal_cmd_base cmd_base;
   GLshort v[2];
};
uint32_t
_mesa_unmarshal_WindowPos2sv(struct gl_context *ctx, const struct marshal_cmd_WindowPos2sv *cmd, const uint64_t *last)
{
   const GLshort * v = cmd->v;
   CALL_WindowPos2sv(ctx->CurrentServerDispatch, (v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_WindowPos2sv), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_WindowPos2sv(const GLshort * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_WindowPos2sv);
   struct marshal_cmd_WindowPos2sv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_WindowPos2sv, cmd_size);
   memcpy(cmd->v, v, 2 * sizeof(GLshort));
}


/* WindowPos3d: marshalled asynchronously */
struct marshal_cmd_WindowPos3d
{
   struct marshal_cmd_base cmd_base;
   GLdouble x;
   GLdouble y;
   GLdouble z;
};
uint32_t
_mesa_unmarshal_WindowPos3d(struct gl_context *ctx, const struct marshal_cmd_WindowPos3d *cmd, const uint64_t *last)
{
   GLdouble x = cmd->x;
   GLdouble y = cmd->y;
   GLdouble z = cmd->z;
   CALL_WindowPos3d(ctx->CurrentServerDispatch, (x, y, z));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_WindowPos3d), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_WindowPos3d(GLdouble x, GLdouble y, GLdouble z)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_WindowPos3d);
   struct marshal_cmd_WindowPos3d *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_WindowPos3d, cmd_size);
   cmd->x = x;
   cmd->y = y;
   cmd->z = z;
}


/* WindowPos3dv: marshalled asynchronously */
struct marshal_cmd_WindowPos3dv
{
   struct marshal_cmd_base cmd_base;
   GLdouble v[3];
};
uint32_t
_mesa_unmarshal_WindowPos3dv(struct gl_context *ctx, const struct marshal_cmd_WindowPos3dv *cmd, const uint64_t *last)
{
   const GLdouble * v = cmd->v;
   CALL_WindowPos3dv(ctx->CurrentServerDispatch, (v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_WindowPos3dv), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_WindowPos3dv(const GLdouble * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_WindowPos3dv);
   struct marshal_cmd_WindowPos3dv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_WindowPos3dv, cmd_size);
   memcpy(cmd->v, v, 3 * sizeof(GLdouble));
}


/* WindowPos3f: marshalled asynchronously */
struct marshal_cmd_WindowPos3f
{
   struct marshal_cmd_base cmd_base;
   GLfloat x;
   GLfloat y;
   GLfloat z;
};
uint32_t
_mesa_unmarshal_WindowPos3f(struct gl_context *ctx, const struct marshal_cmd_WindowPos3f *cmd, const uint64_t *last)
{
   GLfloat x = cmd->x;
   GLfloat y = cmd->y;
   GLfloat z = cmd->z;
   CALL_WindowPos3f(ctx->CurrentServerDispatch, (x, y, z));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_WindowPos3f), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_WindowPos3f(GLfloat x, GLfloat y, GLfloat z)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_WindowPos3f);
   struct marshal_cmd_WindowPos3f *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_WindowPos3f, cmd_size);
   cmd->x = x;
   cmd->y = y;
   cmd->z = z;
}


/* WindowPos3fv: marshalled asynchronously */
struct marshal_cmd_WindowPos3fv
{
   struct marshal_cmd_base cmd_base;
   GLfloat v[3];
};
uint32_t
_mesa_unmarshal_WindowPos3fv(struct gl_context *ctx, const struct marshal_cmd_WindowPos3fv *cmd, const uint64_t *last)
{
   const GLfloat * v = cmd->v;
   CALL_WindowPos3fv(ctx->CurrentServerDispatch, (v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_WindowPos3fv), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_WindowPos3fv(const GLfloat * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_WindowPos3fv);
   struct marshal_cmd_WindowPos3fv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_WindowPos3fv, cmd_size);
   memcpy(cmd->v, v, 3 * sizeof(GLfloat));
}


/* WindowPos3i: marshalled asynchronously */
struct marshal_cmd_WindowPos3i
{
   struct marshal_cmd_base cmd_base;
   GLint x;
   GLint y;
   GLint z;
};
uint32_t
_mesa_unmarshal_WindowPos3i(struct gl_context *ctx, const struct marshal_cmd_WindowPos3i *cmd, const uint64_t *last)
{
   GLint x = cmd->x;
   GLint y = cmd->y;
   GLint z = cmd->z;
   CALL_WindowPos3i(ctx->CurrentServerDispatch, (x, y, z));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_WindowPos3i), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_WindowPos3i(GLint x, GLint y, GLint z)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_WindowPos3i);
   struct marshal_cmd_WindowPos3i *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_WindowPos3i, cmd_size);
   cmd->x = x;
   cmd->y = y;
   cmd->z = z;
}


/* WindowPos3iv: marshalled asynchronously */
struct marshal_cmd_WindowPos3iv
{
   struct marshal_cmd_base cmd_base;
   GLint v[3];
};
uint32_t
_mesa_unmarshal_WindowPos3iv(struct gl_context *ctx, const struct marshal_cmd_WindowPos3iv *cmd, const uint64_t *last)
{
   const GLint * v = cmd->v;
   CALL_WindowPos3iv(ctx->CurrentServerDispatch, (v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_WindowPos3iv), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_WindowPos3iv(const GLint * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_WindowPos3iv);
   struct marshal_cmd_WindowPos3iv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_WindowPos3iv, cmd_size);
   memcpy(cmd->v, v, 3 * sizeof(GLint));
}


/* WindowPos3s: marshalled asynchronously */
struct marshal_cmd_WindowPos3s
{
   struct marshal_cmd_base cmd_base;
   GLshort x;
   GLshort y;
   GLshort z;
};
uint32_t
_mesa_unmarshal_WindowPos3s(struct gl_context *ctx, const struct marshal_cmd_WindowPos3s *cmd, const uint64_t *last)
{
   GLshort x = cmd->x;
   GLshort y = cmd->y;
   GLshort z = cmd->z;
   CALL_WindowPos3s(ctx->CurrentServerDispatch, (x, y, z));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_WindowPos3s), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_WindowPos3s(GLshort x, GLshort y, GLshort z)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_WindowPos3s);
   struct marshal_cmd_WindowPos3s *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_WindowPos3s, cmd_size);
   cmd->x = x;
   cmd->y = y;
   cmd->z = z;
}


/* WindowPos3sv: marshalled asynchronously */
struct marshal_cmd_WindowPos3sv
{
   struct marshal_cmd_base cmd_base;
   GLshort v[3];
};
uint32_t
_mesa_unmarshal_WindowPos3sv(struct gl_context *ctx, const struct marshal_cmd_WindowPos3sv *cmd, const uint64_t *last)
{
   const GLshort * v = cmd->v;
   CALL_WindowPos3sv(ctx->CurrentServerDispatch, (v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_WindowPos3sv), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_WindowPos3sv(const GLshort * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_WindowPos3sv);
   struct marshal_cmd_WindowPos3sv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_WindowPos3sv, cmd_size);
   memcpy(cmd->v, v, 3 * sizeof(GLshort));
}


/* BindBuffer: marshalled asynchronously */
struct marshal_cmd_BindBuffer
{
   struct marshal_cmd_base cmd_base;
   GLenum target;
   GLuint buffer;
};
uint32_t
_mesa_unmarshal_BindBuffer(struct gl_context *ctx, const struct marshal_cmd_BindBuffer *cmd, const uint64_t *last)
{
   GLenum target = cmd->target;
   GLuint buffer = cmd->buffer;
   CALL_BindBuffer(ctx->CurrentServerDispatch, (target, buffer));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_BindBuffer), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_BindBuffer(GLenum target, GLuint buffer)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_BindBuffer);
   struct marshal_cmd_BindBuffer *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_BindBuffer, cmd_size);
   cmd->target = target;
   cmd->buffer = buffer;
   if (COMPAT) _mesa_glthread_BindBuffer(ctx, target, buffer);
}


/* DeleteBuffers: marshalled asynchronously */
struct marshal_cmd_DeleteBuffers
{
   struct marshal_cmd_base cmd_base;
   GLsizei n;
   /* Next safe_mul(n, 1 * sizeof(GLuint)) bytes are GLuint buffer[n] */
};
uint32_t
_mesa_unmarshal_DeleteBuffers(struct gl_context *ctx, const struct marshal_cmd_DeleteBuffers *cmd, const uint64_t *last)
{
   GLsizei n = cmd->n;
   GLuint * buffer;
   const char *variable_data = (const char *) (cmd + 1);
   buffer = (GLuint *) variable_data;
   CALL_DeleteBuffers(ctx->CurrentServerDispatch, (n, buffer));
   return cmd->cmd_base.cmd_size;
}
void GLAPIENTRY
_mesa_marshal_DeleteBuffers(GLsizei n, const GLuint * buffer)
{
   GET_CURRENT_CONTEXT(ctx);
   int buffer_size = safe_mul(n, 1 * sizeof(GLuint));
   int cmd_size = sizeof(struct marshal_cmd_DeleteBuffers) + buffer_size;
   struct marshal_cmd_DeleteBuffers *cmd;
   if (unlikely(buffer_size < 0 || (buffer_size > 0 && !buffer) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "DeleteBuffers");
      CALL_DeleteBuffers(ctx->CurrentServerDispatch, (n, buffer));
      if (COMPAT) _mesa_glthread_DeleteBuffers(ctx, n, buffer);
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_DeleteBuffers, cmd_size);
   cmd->n = n;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, buffer, buffer_size);
   if (COMPAT) _mesa_glthread_DeleteBuffers(ctx, n, buffer);
}


/* GenBuffers: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GenBuffers(GLsizei n, GLuint * buffer)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GenBuffers");
   CALL_GenBuffers(ctx->CurrentServerDispatch, (n, buffer));
}


/* GetBufferParameteriv: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetBufferParameteriv(GLenum target, GLenum pname, GLint * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetBufferParameteriv");
   CALL_GetBufferParameteriv(ctx->CurrentServerDispatch, (target, pname, params));
}


/* GetBufferPointerv: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetBufferPointerv(GLenum target, GLenum pname, GLvoid ** params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetBufferPointerv");
   CALL_GetBufferPointerv(ctx->CurrentServerDispatch, (target, pname, params));
}


/* GetBufferSubData: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetBufferSubData(GLenum target, GLintptr offset, GLsizeiptr size, GLvoid * data)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetBufferSubData");
   CALL_GetBufferSubData(ctx->CurrentServerDispatch, (target, offset, size, data));
}


/* IsBuffer: marshalled synchronously */
GLboolean GLAPIENTRY
_mesa_marshal_IsBuffer(GLuint buffer)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "IsBuffer");
   return CALL_IsBuffer(ctx->CurrentServerDispatch, (buffer));
}


/* MapBuffer: marshalled synchronously */
GLvoid * GLAPIENTRY
_mesa_marshal_MapBuffer(GLenum target, GLenum access)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "MapBuffer");
   return CALL_MapBuffer(ctx->CurrentServerDispatch, (target, access));
}


/* UnmapBuffer: marshalled asynchronously */
struct marshal_cmd_UnmapBuffer
{
   struct marshal_cmd_base cmd_base;
   GLenum target;
};
uint32_t
_mesa_unmarshal_UnmapBuffer(struct gl_context *ctx, const struct marshal_cmd_UnmapBuffer *cmd, const uint64_t *last)
{
   GLenum target = cmd->target;
   CALL_UnmapBuffer(ctx->CurrentServerDispatch, (target));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_UnmapBuffer), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
GLboolean GLAPIENTRY
_mesa_marshal_UnmapBuffer(GLenum target)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_UnmapBuffer);
   struct marshal_cmd_UnmapBuffer *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_UnmapBuffer, cmd_size);
   cmd->target = target;
   return GL_TRUE;
}


/* GenQueries: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GenQueries(GLsizei n, GLuint * ids)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GenQueries");
   CALL_GenQueries(ctx->CurrentServerDispatch, (n, ids));
}


/* DeleteQueries: marshalled asynchronously */
struct marshal_cmd_DeleteQueries
{
   struct marshal_cmd_base cmd_base;
   GLsizei n;
   /* Next safe_mul(n, 1 * sizeof(GLuint)) bytes are GLuint ids[n] */
};
uint32_t
_mesa_unmarshal_DeleteQueries(struct gl_context *ctx, const struct marshal_cmd_DeleteQueries *cmd, const uint64_t *last)
{
   GLsizei n = cmd->n;
   GLuint * ids;
   const char *variable_data = (const char *) (cmd + 1);
   ids = (GLuint *) variable_data;
   CALL_DeleteQueries(ctx->CurrentServerDispatch, (n, ids));
   return cmd->cmd_base.cmd_size;
}
void GLAPIENTRY
_mesa_marshal_DeleteQueries(GLsizei n, const GLuint * ids)
{
   GET_CURRENT_CONTEXT(ctx);
   int ids_size = safe_mul(n, 1 * sizeof(GLuint));
   int cmd_size = sizeof(struct marshal_cmd_DeleteQueries) + ids_size;
   struct marshal_cmd_DeleteQueries *cmd;
   if (unlikely(ids_size < 0 || (ids_size > 0 && !ids) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "DeleteQueries");
      CALL_DeleteQueries(ctx->CurrentServerDispatch, (n, ids));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_DeleteQueries, cmd_size);
   cmd->n = n;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, ids, ids_size);
}


/* IsQuery: marshalled synchronously */
GLboolean GLAPIENTRY
_mesa_marshal_IsQuery(GLuint id)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "IsQuery");
   return CALL_IsQuery(ctx->CurrentServerDispatch, (id));
}


/* BeginQuery: marshalled asynchronously */
struct marshal_cmd_BeginQuery
{
   struct marshal_cmd_base cmd_base;
   GLenum target;
   GLuint id;
};
uint32_t
_mesa_unmarshal_BeginQuery(struct gl_context *ctx, const struct marshal_cmd_BeginQuery *cmd, const uint64_t *last)
{
   GLenum target = cmd->target;
   GLuint id = cmd->id;
   CALL_BeginQuery(ctx->CurrentServerDispatch, (target, id));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_BeginQuery), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_BeginQuery(GLenum target, GLuint id)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_BeginQuery);
   struct marshal_cmd_BeginQuery *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_BeginQuery, cmd_size);
   cmd->target = target;
   cmd->id = id;
}


/* EndQuery: marshalled asynchronously */
struct marshal_cmd_EndQuery
{
   struct marshal_cmd_base cmd_base;
   GLenum target;
};
uint32_t
_mesa_unmarshal_EndQuery(struct gl_context *ctx, const struct marshal_cmd_EndQuery *cmd, const uint64_t *last)
{
   GLenum target = cmd->target;
   CALL_EndQuery(ctx->CurrentServerDispatch, (target));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_EndQuery), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_EndQuery(GLenum target)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_EndQuery);
   struct marshal_cmd_EndQuery *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_EndQuery, cmd_size);
   cmd->target = target;
}


/* GetQueryiv: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetQueryiv(GLenum target, GLenum pname, GLint * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetQueryiv");
   CALL_GetQueryiv(ctx->CurrentServerDispatch, (target, pname, params));
}


/* GetQueryObjectiv: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetQueryObjectiv(GLuint id, GLenum pname, GLint * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetQueryObjectiv");
   CALL_GetQueryObjectiv(ctx->CurrentServerDispatch, (id, pname, params));
}


/* GetQueryObjectuiv: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetQueryObjectuiv(GLuint id, GLenum pname, GLuint * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetQueryObjectuiv");
   CALL_GetQueryObjectuiv(ctx->CurrentServerDispatch, (id, pname, params));
}


/* BlendEquationSeparate: marshalled asynchronously */
struct marshal_cmd_BlendEquationSeparate
{
   struct marshal_cmd_base cmd_base;
   GLenum modeRGB;
   GLenum modeA;
};
uint32_t
_mesa_unmarshal_BlendEquationSeparate(struct gl_context *ctx, const struct marshal_cmd_BlendEquationSeparate *cmd, const uint64_t *last)
{
   GLenum modeRGB = cmd->modeRGB;
   GLenum modeA = cmd->modeA;
   CALL_BlendEquationSeparate(ctx->CurrentServerDispatch, (modeRGB, modeA));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_BlendEquationSeparate), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_BlendEquationSeparate(GLenum modeRGB, GLenum modeA)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_BlendEquationSeparate);
   struct marshal_cmd_BlendEquationSeparate *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_BlendEquationSeparate, cmd_size);
   cmd->modeRGB = modeRGB;
   cmd->modeA = modeA;
}


/* DrawBuffers: marshalled asynchronously */
struct marshal_cmd_DrawBuffers
{
   struct marshal_cmd_base cmd_base;
   GLsizei n;
   /* Next safe_mul(n, 1 * sizeof(GLenum)) bytes are GLenum bufs[n] */
};
uint32_t
_mesa_unmarshal_DrawBuffers(struct gl_context *ctx, const struct marshal_cmd_DrawBuffers *cmd, const uint64_t *last)
{
   GLsizei n = cmd->n;
   GLenum * bufs;
   const char *variable_data = (const char *) (cmd + 1);
   bufs = (GLenum *) variable_data;
   CALL_DrawBuffers(ctx->CurrentServerDispatch, (n, bufs));
   return cmd->cmd_base.cmd_size;
}
void GLAPIENTRY
_mesa_marshal_DrawBuffers(GLsizei n, const GLenum * bufs)
{
   GET_CURRENT_CONTEXT(ctx);
   int bufs_size = safe_mul(n, 1 * sizeof(GLenum));
   int cmd_size = sizeof(struct marshal_cmd_DrawBuffers) + bufs_size;
   struct marshal_cmd_DrawBuffers *cmd;
   if (unlikely(bufs_size < 0 || (bufs_size > 0 && !bufs) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "DrawBuffers");
      CALL_DrawBuffers(ctx->CurrentServerDispatch, (n, bufs));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_DrawBuffers, cmd_size);
   cmd->n = n;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, bufs, bufs_size);
}


/* StencilFuncSeparate: marshalled asynchronously */
struct marshal_cmd_StencilFuncSeparate
{
   struct marshal_cmd_base cmd_base;
   GLenum face;
   GLenum func;
   GLint ref;
   GLuint mask;
};
uint32_t
_mesa_unmarshal_StencilFuncSeparate(struct gl_context *ctx, const struct marshal_cmd_StencilFuncSeparate *cmd, const uint64_t *last)
{
   GLenum face = cmd->face;
   GLenum func = cmd->func;
   GLint ref = cmd->ref;
   GLuint mask = cmd->mask;
   CALL_StencilFuncSeparate(ctx->CurrentServerDispatch, (face, func, ref, mask));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_StencilFuncSeparate), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_StencilFuncSeparate(GLenum face, GLenum func, GLint ref, GLuint mask)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_StencilFuncSeparate);
   struct marshal_cmd_StencilFuncSeparate *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_StencilFuncSeparate, cmd_size);
   cmd->face = face;
   cmd->func = func;
   cmd->ref = ref;
   cmd->mask = mask;
}


/* StencilOpSeparate: marshalled asynchronously */
struct marshal_cmd_StencilOpSeparate
{
   struct marshal_cmd_base cmd_base;
   GLenum face;
   GLenum sfail;
   GLenum zfail;
   GLenum zpass;
};
uint32_t
_mesa_unmarshal_StencilOpSeparate(struct gl_context *ctx, const struct marshal_cmd_StencilOpSeparate *cmd, const uint64_t *last)
{
   GLenum face = cmd->face;
   GLenum sfail = cmd->sfail;
   GLenum zfail = cmd->zfail;
   GLenum zpass = cmd->zpass;
   CALL_StencilOpSeparate(ctx->CurrentServerDispatch, (face, sfail, zfail, zpass));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_StencilOpSeparate), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_StencilOpSeparate(GLenum face, GLenum sfail, GLenum zfail, GLenum zpass)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_StencilOpSeparate);
   struct marshal_cmd_StencilOpSeparate *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_StencilOpSeparate, cmd_size);
   cmd->face = face;
   cmd->sfail = sfail;
   cmd->zfail = zfail;
   cmd->zpass = zpass;
}


/* StencilMaskSeparate: marshalled asynchronously */
struct marshal_cmd_StencilMaskSeparate
{
   struct marshal_cmd_base cmd_base;
   GLenum face;
   GLuint mask;
};
uint32_t
_mesa_unmarshal_StencilMaskSeparate(struct gl_context *ctx, const struct marshal_cmd_StencilMaskSeparate *cmd, const uint64_t *last)
{
   GLenum face = cmd->face;
   GLuint mask = cmd->mask;
   CALL_StencilMaskSeparate(ctx->CurrentServerDispatch, (face, mask));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_StencilMaskSeparate), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_StencilMaskSeparate(GLenum face, GLuint mask)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_StencilMaskSeparate);
   struct marshal_cmd_StencilMaskSeparate *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_StencilMaskSeparate, cmd_size);
   cmd->face = face;
   cmd->mask = mask;
}


/* AttachShader: marshalled asynchronously */
struct marshal_cmd_AttachShader
{
   struct marshal_cmd_base cmd_base;
   GLuint program;
   GLuint shader;
};
uint32_t
_mesa_unmarshal_AttachShader(struct gl_context *ctx, const struct marshal_cmd_AttachShader *cmd, const uint64_t *last)
{
   GLuint program = cmd->program;
   GLuint shader = cmd->shader;
   CALL_AttachShader(ctx->CurrentServerDispatch, (program, shader));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_AttachShader), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_AttachShader(GLuint program, GLuint shader)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_AttachShader);
   struct marshal_cmd_AttachShader *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_AttachShader, cmd_size);
   cmd->program = program;
   cmd->shader = shader;
}


/* BindAttribLocation: marshalled asynchronously */
struct marshal_cmd_BindAttribLocation
{
   struct marshal_cmd_base cmd_base;
   GLuint program;
   GLuint index;
   /* Next (strlen(name) + 1) bytes are GLchar name[(strlen(name) + 1)] */
};
uint32_t
_mesa_unmarshal_BindAttribLocation(struct gl_context *ctx, const struct marshal_cmd_BindAttribLocation *cmd, const uint64_t *last)
{
   GLuint program = cmd->program;
   GLuint index = cmd->index;
   GLchar * name;
   const char *variable_data = (const char *) (cmd + 1);
   name = (GLchar *) variable_data;
   CALL_BindAttribLocation(ctx->CurrentServerDispatch, (program, index, name));
   return cmd->cmd_base.cmd_size;
}
void GLAPIENTRY
_mesa_marshal_BindAttribLocation(GLuint program, GLuint index, const GLchar * name)
{
   GET_CURRENT_CONTEXT(ctx);
   int name_size = (strlen(name) + 1);
   int cmd_size = sizeof(struct marshal_cmd_BindAttribLocation) + name_size;
   struct marshal_cmd_BindAttribLocation *cmd;
   if (unlikely(name_size < 0 || (name_size > 0 && !name) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "BindAttribLocation");
      CALL_BindAttribLocation(ctx->CurrentServerDispatch, (program, index, name));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_BindAttribLocation, cmd_size);
   cmd->program = program;
   cmd->index = index;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, name, name_size);
}


/* CompileShader: marshalled asynchronously */
struct marshal_cmd_CompileShader
{
   struct marshal_cmd_base cmd_base;
   GLuint shader;
};
uint32_t
_mesa_unmarshal_CompileShader(struct gl_context *ctx, const struct marshal_cmd_CompileShader *cmd, const uint64_t *last)
{
   GLuint shader = cmd->shader;
   CALL_CompileShader(ctx->CurrentServerDispatch, (shader));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_CompileShader), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_CompileShader(GLuint shader)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_CompileShader);
   struct marshal_cmd_CompileShader *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_CompileShader, cmd_size);
   cmd->shader = shader;
}


/* CreateProgram: marshalled synchronously */
GLuint GLAPIENTRY
_mesa_marshal_CreateProgram(void)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "CreateProgram");
   return CALL_CreateProgram(ctx->CurrentServerDispatch, ());
}


/* CreateShader: marshalled synchronously */
GLuint GLAPIENTRY
_mesa_marshal_CreateShader(GLenum type)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "CreateShader");
   return CALL_CreateShader(ctx->CurrentServerDispatch, (type));
}


/* DeleteProgram: marshalled asynchronously */
struct marshal_cmd_DeleteProgram
{
   struct marshal_cmd_base cmd_base;
   GLuint program;
};
uint32_t
_mesa_unmarshal_DeleteProgram(struct gl_context *ctx, const struct marshal_cmd_DeleteProgram *cmd, const uint64_t *last)
{
   GLuint program = cmd->program;
   CALL_DeleteProgram(ctx->CurrentServerDispatch, (program));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_DeleteProgram), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_DeleteProgram(GLuint program)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_DeleteProgram);
   struct marshal_cmd_DeleteProgram *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_DeleteProgram, cmd_size);
   cmd->program = program;
   _mesa_glthread_ProgramChanged(ctx);
}


/* DeleteShader: marshalled asynchronously */
struct marshal_cmd_DeleteShader
{
   struct marshal_cmd_base cmd_base;
   GLuint program;
};
uint32_t
_mesa_unmarshal_DeleteShader(struct gl_context *ctx, const struct marshal_cmd_DeleteShader *cmd, const uint64_t *last)
{
   GLuint program = cmd->program;
   CALL_DeleteShader(ctx->CurrentServerDispatch, (program));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_DeleteShader), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_DeleteShader(GLuint program)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_DeleteShader);
   struct marshal_cmd_DeleteShader *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_DeleteShader, cmd_size);
   cmd->program = program;
}


/* DetachShader: marshalled asynchronously */
struct marshal_cmd_DetachShader
{
   struct marshal_cmd_base cmd_base;
   GLuint program;
   GLuint shader;
};
uint32_t
_mesa_unmarshal_DetachShader(struct gl_context *ctx, const struct marshal_cmd_DetachShader *cmd, const uint64_t *last)
{
   GLuint program = cmd->program;
   GLuint shader = cmd->shader;
   CALL_DetachShader(ctx->CurrentServerDispatch, (program, shader));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_DetachShader), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_DetachShader(GLuint program, GLuint shader)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_DetachShader);
   struct marshal_cmd_DetachShader *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_DetachShader, cmd_size);
   cmd->program = program;
   cmd->shader = shader;
}


/* DisableVertexAttribArray: marshalled asynchronously */
struct marshal_cmd_DisableVertexAttribArray
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
};
uint32_t
_mesa_unmarshal_DisableVertexAttribArray(struct gl_context *ctx, const struct marshal_cmd_DisableVertexAttribArray *cmd, const uint64_t *last)
{
   GLuint index = cmd->index;
   CALL_DisableVertexAttribArray(ctx->CurrentServerDispatch, (index));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_DisableVertexAttribArray), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_DisableVertexAttribArray(GLuint index)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_DisableVertexAttribArray);
   struct marshal_cmd_DisableVertexAttribArray *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_DisableVertexAttribArray, cmd_size);
   cmd->index = index;
   if (COMPAT) _mesa_glthread_ClientState(ctx, NULL, VERT_ATTRIB_GENERIC(index), false);
}


/* EnableVertexAttribArray: marshalled asynchronously */
struct marshal_cmd_EnableVertexAttribArray
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
};
uint32_t
_mesa_unmarshal_EnableVertexAttribArray(struct gl_context *ctx, const struct marshal_cmd_EnableVertexAttribArray *cmd, const uint64_t *last)
{
   GLuint index = cmd->index;
   CALL_EnableVertexAttribArray(ctx->CurrentServerDispatch, (index));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_EnableVertexAttribArray), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_EnableVertexAttribArray(GLuint index)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_EnableVertexAttribArray);
   struct marshal_cmd_EnableVertexAttribArray *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_EnableVertexAttribArray, cmd_size);
   cmd->index = index;
   if (COMPAT) _mesa_glthread_ClientState(ctx, NULL, VERT_ATTRIB_GENERIC(index), true);
}


/* GetActiveAttrib: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetActiveAttrib(GLuint program, GLuint index, GLsizei  bufSize, GLsizei * length, GLint * size, GLenum * type, GLchar * name)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetActiveAttrib");
   CALL_GetActiveAttrib(ctx->CurrentServerDispatch, (program, index, bufSize, length, size, type, name));
}


/* GetAttachedShaders: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetAttachedShaders(GLuint program, GLsizei maxCount, GLsizei * count, GLuint * obj)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetAttachedShaders");
   CALL_GetAttachedShaders(ctx->CurrentServerDispatch, (program, maxCount, count, obj));
}


/* GetAttribLocation: marshalled synchronously */
GLint GLAPIENTRY
_mesa_marshal_GetAttribLocation(GLuint program, const GLchar * name)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetAttribLocation");
   return CALL_GetAttribLocation(ctx->CurrentServerDispatch, (program, name));
}


/* GetProgramiv: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetProgramiv(GLuint program, GLenum pname, GLint * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetProgramiv");
   CALL_GetProgramiv(ctx->CurrentServerDispatch, (program, pname, params));
}


/* GetProgramInfoLog: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetProgramInfoLog(GLuint program, GLsizei bufSize, GLsizei * length, GLchar * infoLog)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetProgramInfoLog");
   CALL_GetProgramInfoLog(ctx->CurrentServerDispatch, (program, bufSize, length, infoLog));
}


/* GetShaderiv: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetShaderiv(GLuint shader, GLenum pname, GLint * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetShaderiv");
   CALL_GetShaderiv(ctx->CurrentServerDispatch, (shader, pname, params));
}


/* GetShaderInfoLog: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetShaderInfoLog(GLuint shader, GLsizei bufSize, GLsizei * length, GLchar * infoLog)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetShaderInfoLog");
   CALL_GetShaderInfoLog(ctx->CurrentServerDispatch, (shader, bufSize, length, infoLog));
}


/* GetShaderSource: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetShaderSource(GLuint shader, GLsizei bufSize, GLsizei * length, GLchar * source)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetShaderSource");
   CALL_GetShaderSource(ctx->CurrentServerDispatch, (shader, bufSize, length, source));
}


/* GetUniformfv: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetUniformfv(GLuint program, GLint location, GLfloat * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetUniformfv");
   CALL_GetUniformfv(ctx->CurrentServerDispatch, (program, location, params));
}


/* GetUniformiv: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetUniformiv(GLuint program, GLint location, GLint * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetUniformiv");
   CALL_GetUniformiv(ctx->CurrentServerDispatch, (program, location, params));
}


/* GetVertexAttribdv: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetVertexAttribdv(GLuint index, GLenum pname, GLdouble * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetVertexAttribdv");
   CALL_GetVertexAttribdv(ctx->CurrentServerDispatch, (index, pname, params));
}


/* GetVertexAttribfv: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetVertexAttribfv(GLuint index, GLenum pname, GLfloat * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetVertexAttribfv");
   CALL_GetVertexAttribfv(ctx->CurrentServerDispatch, (index, pname, params));
}


/* GetVertexAttribiv: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetVertexAttribiv(GLuint index, GLenum pname, GLint * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetVertexAttribiv");
   CALL_GetVertexAttribiv(ctx->CurrentServerDispatch, (index, pname, params));
}


/* GetVertexAttribPointerv: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetVertexAttribPointerv(GLuint index, GLenum pname, GLvoid ** pointer)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetVertexAttribPointerv");
   CALL_GetVertexAttribPointerv(ctx->CurrentServerDispatch, (index, pname, pointer));
}


/* IsProgram: marshalled synchronously */
GLboolean GLAPIENTRY
_mesa_marshal_IsProgram(GLuint program)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "IsProgram");
   return CALL_IsProgram(ctx->CurrentServerDispatch, (program));
}


/* IsShader: marshalled synchronously */
GLboolean GLAPIENTRY
_mesa_marshal_IsShader(GLuint shader)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "IsShader");
   return CALL_IsShader(ctx->CurrentServerDispatch, (shader));
}


/* LinkProgram: marshalled asynchronously */
struct marshal_cmd_LinkProgram
{
   struct marshal_cmd_base cmd_base;
   GLuint program;
};
uint32_t
_mesa_unmarshal_LinkProgram(struct gl_context *ctx, const struct marshal_cmd_LinkProgram *cmd, const uint64_t *last)
{
   GLuint program = cmd->program;
   CALL_LinkProgram(ctx->CurrentServerDispatch, (program));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_LinkProgram), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_LinkProgram(GLuint program)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_LinkProgram);
   struct marshal_cmd_LinkProgram *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_LinkProgram, cmd_size);
   cmd->program = program;
   _mesa_glthread_ProgramChanged(ctx);
}


/* UseProgram: marshalled asynchronously */
struct marshal_cmd_UseProgram
{
   struct marshal_cmd_base cmd_base;
   GLuint program;
};
uint32_t
_mesa_unmarshal_UseProgram(struct gl_context *ctx, const struct marshal_cmd_UseProgram *cmd, const uint64_t *last)
{
   GLuint program = cmd->program;
   CALL_UseProgram(ctx->CurrentServerDispatch, (program));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_UseProgram), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_UseProgram(GLuint program)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_UseProgram);
   struct marshal_cmd_UseProgram *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_UseProgram, cmd_size);
   cmd->program = program;
}


/* Uniform1f: marshalled asynchronously */
struct marshal_cmd_Uniform1f
{
   struct marshal_cmd_base cmd_base;
   GLint location;
   GLfloat v0;
};
uint32_t
_mesa_unmarshal_Uniform1f(struct gl_context *ctx, const struct marshal_cmd_Uniform1f *cmd, const uint64_t *last)
{
   GLint location = cmd->location;
   GLfloat v0 = cmd->v0;
   CALL_Uniform1f(ctx->CurrentServerDispatch, (location, v0));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Uniform1f), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_Uniform1f(GLint location, GLfloat v0)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Uniform1f);
   struct marshal_cmd_Uniform1f *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Uniform1f, cmd_size);
   cmd->location = location;
   cmd->v0 = v0;
}


/* Uniform2f: marshalled asynchronously */
struct marshal_cmd_Uniform2f
{
   struct marshal_cmd_base cmd_base;
   GLint location;
   GLfloat v0;
   GLfloat v1;
};
uint32_t
_mesa_unmarshal_Uniform2f(struct gl_context *ctx, const struct marshal_cmd_Uniform2f *cmd, const uint64_t *last)
{
   GLint location = cmd->location;
   GLfloat v0 = cmd->v0;
   GLfloat v1 = cmd->v1;
   CALL_Uniform2f(ctx->CurrentServerDispatch, (location, v0, v1));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Uniform2f), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_Uniform2f(GLint location, GLfloat v0, GLfloat v1)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Uniform2f);
   struct marshal_cmd_Uniform2f *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Uniform2f, cmd_size);
   cmd->location = location;
   cmd->v0 = v0;
   cmd->v1 = v1;
}


/* Uniform3f: marshalled asynchronously */
struct marshal_cmd_Uniform3f
{
   struct marshal_cmd_base cmd_base;
   GLint location;
   GLfloat v0;
   GLfloat v1;
   GLfloat v2;
};
uint32_t
_mesa_unmarshal_Uniform3f(struct gl_context *ctx, const struct marshal_cmd_Uniform3f *cmd, const uint64_t *last)
{
   GLint location = cmd->location;
   GLfloat v0 = cmd->v0;
   GLfloat v1 = cmd->v1;
   GLfloat v2 = cmd->v2;
   CALL_Uniform3f(ctx->CurrentServerDispatch, (location, v0, v1, v2));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Uniform3f), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_Uniform3f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Uniform3f);
   struct marshal_cmd_Uniform3f *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Uniform3f, cmd_size);
   cmd->location = location;
   cmd->v0 = v0;
   cmd->v1 = v1;
   cmd->v2 = v2;
}


/* Uniform4f: marshalled asynchronously */
struct marshal_cmd_Uniform4f
{
   struct marshal_cmd_base cmd_base;
   GLint location;
   GLfloat v0;
   GLfloat v1;
   GLfloat v2;
   GLfloat v3;
};
uint32_t
_mesa_unmarshal_Uniform4f(struct gl_context *ctx, const struct marshal_cmd_Uniform4f *cmd, const uint64_t *last)
{
   GLint location = cmd->location;
   GLfloat v0 = cmd->v0;
   GLfloat v1 = cmd->v1;
   GLfloat v2 = cmd->v2;
   GLfloat v3 = cmd->v3;
   CALL_Uniform4f(ctx->CurrentServerDispatch, (location, v0, v1, v2, v3));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Uniform4f), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_Uniform4f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Uniform4f);
   struct marshal_cmd_Uniform4f *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Uniform4f, cmd_size);
   cmd->location = location;
   cmd->v0 = v0;
   cmd->v1 = v1;
   cmd->v2 = v2;
   cmd->v3 = v3;
}


/* Uniform1i: marshalled asynchronously */
struct marshal_cmd_Uniform1i
{
   struct marshal_cmd_base cmd_base;
   GLint location;
   GLint v0;
};
uint32_t
_mesa_unmarshal_Uniform1i(struct gl_context *ctx, const struct marshal_cmd_Uniform1i *cmd, const uint64_t *last)
{
   GLint location = cmd->location;
   GLint v0 = cmd->v0;
   CALL_Uniform1i(ctx->CurrentServerDispatch, (location, v0));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Uniform1i), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_Uniform1i(GLint location, GLint v0)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Uniform1i);
   struct marshal_cmd_Uniform1i *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Uniform1i, cmd_size);
   cmd->location = location;
   cmd->v0 = v0;
}


/* Uniform2i: marshalled asynchronously */
struct marshal_cmd_Uniform2i
{
   struct marshal_cmd_base cmd_base;
   GLint location;
   GLint v0;
   GLint v1;
};
uint32_t
_mesa_unmarshal_Uniform2i(struct gl_context *ctx, const struct marshal_cmd_Uniform2i *cmd, const uint64_t *last)
{
   GLint location = cmd->location;
   GLint v0 = cmd->v0;
   GLint v1 = cmd->v1;
   CALL_Uniform2i(ctx->CurrentServerDispatch, (location, v0, v1));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Uniform2i), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_Uniform2i(GLint location, GLint v0, GLint v1)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Uniform2i);
   struct marshal_cmd_Uniform2i *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Uniform2i, cmd_size);
   cmd->location = location;
   cmd->v0 = v0;
   cmd->v1 = v1;
}


/* Uniform3i: marshalled asynchronously */
struct marshal_cmd_Uniform3i
{
   struct marshal_cmd_base cmd_base;
   GLint location;
   GLint v0;
   GLint v1;
   GLint v2;
};
uint32_t
_mesa_unmarshal_Uniform3i(struct gl_context *ctx, const struct marshal_cmd_Uniform3i *cmd, const uint64_t *last)
{
   GLint location = cmd->location;
   GLint v0 = cmd->v0;
   GLint v1 = cmd->v1;
   GLint v2 = cmd->v2;
   CALL_Uniform3i(ctx->CurrentServerDispatch, (location, v0, v1, v2));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Uniform3i), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_Uniform3i(GLint location, GLint v0, GLint v1, GLint v2)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Uniform3i);
   struct marshal_cmd_Uniform3i *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Uniform3i, cmd_size);
   cmd->location = location;
   cmd->v0 = v0;
   cmd->v1 = v1;
   cmd->v2 = v2;
}


/* Uniform4i: marshalled asynchronously */
struct marshal_cmd_Uniform4i
{
   struct marshal_cmd_base cmd_base;
   GLint location;
   GLint v0;
   GLint v1;
   GLint v2;
   GLint v3;
};
uint32_t
_mesa_unmarshal_Uniform4i(struct gl_context *ctx, const struct marshal_cmd_Uniform4i *cmd, const uint64_t *last)
{
   GLint location = cmd->location;
   GLint v0 = cmd->v0;
   GLint v1 = cmd->v1;
   GLint v2 = cmd->v2;
   GLint v3 = cmd->v3;
   CALL_Uniform4i(ctx->CurrentServerDispatch, (location, v0, v1, v2, v3));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Uniform4i), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_Uniform4i(GLint location, GLint v0, GLint v1, GLint v2, GLint v3)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Uniform4i);
   struct marshal_cmd_Uniform4i *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Uniform4i, cmd_size);
   cmd->location = location;
   cmd->v0 = v0;
   cmd->v1 = v1;
   cmd->v2 = v2;
   cmd->v3 = v3;
}


/* Uniform1fv: marshalled asynchronously */
struct marshal_cmd_Uniform1fv
{
   struct marshal_cmd_base cmd_base;
   GLint location;
   GLsizei count;
   /* Next safe_mul(count, 1 * sizeof(GLfloat)) bytes are GLfloat value[count] */
};
uint32_t
_mesa_unmarshal_Uniform1fv(struct gl_context *ctx, const struct marshal_cmd_Uniform1fv *cmd, const uint64_t *last)
{
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLfloat * value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLfloat *) variable_data;
   CALL_Uniform1fv(ctx->CurrentServerDispatch, (location, count, value));
   return cmd->cmd_base.cmd_size;
}
void GLAPIENTRY
_mesa_marshal_Uniform1fv(GLint location, GLsizei count, const GLfloat * value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 1 * sizeof(GLfloat));
   int cmd_size = sizeof(struct marshal_cmd_Uniform1fv) + value_size;
   struct marshal_cmd_Uniform1fv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "Uniform1fv");
      CALL_Uniform1fv(ctx->CurrentServerDispatch, (location, count, value));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Uniform1fv, cmd_size);
   cmd->location = location;
   cmd->count = count;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, value, value_size);
}


/* Uniform2fv: marshalled asynchronously */
struct marshal_cmd_Uniform2fv
{
   struct marshal_cmd_base cmd_base;
   GLint location;
   GLsizei count;
   /* Next safe_mul(count, 2 * sizeof(GLfloat)) bytes are GLfloat value[count][2] */
};
uint32_t
_mesa_unmarshal_Uniform2fv(struct gl_context *ctx, const struct marshal_cmd_Uniform2fv *cmd, const uint64_t *last)
{
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLfloat * value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLfloat *) variable_data;
   CALL_Uniform2fv(ctx->CurrentServerDispatch, (location, count, value));
   return cmd->cmd_base.cmd_size;
}
void GLAPIENTRY
_mesa_marshal_Uniform2fv(GLint location, GLsizei count, const GLfloat * value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 2 * sizeof(GLfloat));
   int cmd_size = sizeof(struct marshal_cmd_Uniform2fv) + value_size;
   struct marshal_cmd_Uniform2fv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "Uniform2fv");
      CALL_Uniform2fv(ctx->CurrentServerDispatch, (location, count, value));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Uniform2fv, cmd_size);
   cmd->location = location;
   cmd->count = count;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, value, value_size);
}


/* Uniform3fv: marshalled asynchronously */
struct marshal_cmd_Uniform3fv
{
   struct marshal_cmd_base cmd_base;
   GLint location;
   GLsizei count;
   /* Next safe_mul(count, 3 * sizeof(GLfloat)) bytes are GLfloat value[count][3] */
};
uint32_t
_mesa_unmarshal_Uniform3fv(struct gl_context *ctx, const struct marshal_cmd_Uniform3fv *cmd, const uint64_t *last)
{
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLfloat * value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLfloat *) variable_data;
   CALL_Uniform3fv(ctx->CurrentServerDispatch, (location, count, value));
   return cmd->cmd_base.cmd_size;
}
void GLAPIENTRY
_mesa_marshal_Uniform3fv(GLint location, GLsizei count, const GLfloat * value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 3 * sizeof(GLfloat));
   int cmd_size = sizeof(struct marshal_cmd_Uniform3fv) + value_size;
   struct marshal_cmd_Uniform3fv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "Uniform3fv");
      CALL_Uniform3fv(ctx->CurrentServerDispatch, (location, count, value));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Uniform3fv, cmd_size);
   cmd->location = location;
   cmd->count = count;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, value, value_size);
}


/* Uniform4fv: marshalled asynchronously */
struct marshal_cmd_Uniform4fv
{
   struct marshal_cmd_base cmd_base;
   GLint location;
   GLsizei count;
   /* Next safe_mul(count, 4 * sizeof(GLfloat)) bytes are GLfloat value[count][4] */
};
uint32_t
_mesa_unmarshal_Uniform4fv(struct gl_context *ctx, const struct marshal_cmd_Uniform4fv *cmd, const uint64_t *last)
{
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLfloat * value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLfloat *) variable_data;
   CALL_Uniform4fv(ctx->CurrentServerDispatch, (location, count, value));
   return cmd->cmd_base.cmd_size;
}
void GLAPIENTRY
_mesa_marshal_Uniform4fv(GLint location, GLsizei count, const GLfloat * value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 4 * sizeof(GLfloat));
   int cmd_size = sizeof(struct marshal_cmd_Uniform4fv) + value_size;
   struct marshal_cmd_Uniform4fv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "Uniform4fv");
      CALL_Uniform4fv(ctx->CurrentServerDispatch, (location, count, value));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Uniform4fv, cmd_size);
   cmd->location = location;
   cmd->count = count;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, value, value_size);
}


/* Uniform1iv: marshalled asynchronously */
struct marshal_cmd_Uniform1iv
{
   struct marshal_cmd_base cmd_base;
   GLint location;
   GLsizei count;
   /* Next safe_mul(count, 1 * sizeof(GLint)) bytes are GLint value[count] */
};
uint32_t
_mesa_unmarshal_Uniform1iv(struct gl_context *ctx, const struct marshal_cmd_Uniform1iv *cmd, const uint64_t *last)
{
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLint * value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLint *) variable_data;
   CALL_Uniform1iv(ctx->CurrentServerDispatch, (location, count, value));
   return cmd->cmd_base.cmd_size;
}
void GLAPIENTRY
_mesa_marshal_Uniform1iv(GLint location, GLsizei count, const GLint * value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 1 * sizeof(GLint));
   int cmd_size = sizeof(struct marshal_cmd_Uniform1iv) + value_size;
   struct marshal_cmd_Uniform1iv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "Uniform1iv");
      CALL_Uniform1iv(ctx->CurrentServerDispatch, (location, count, value));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Uniform1iv, cmd_size);
   cmd->location = location;
   cmd->count = count;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, value, value_size);
}


/* Uniform2iv: marshalled asynchronously */
struct marshal_cmd_Uniform2iv
{
   struct marshal_cmd_base cmd_base;
   GLint location;
   GLsizei count;
   /* Next safe_mul(count, 2 * sizeof(GLint)) bytes are GLint value[count][2] */
};
uint32_t
_mesa_unmarshal_Uniform2iv(struct gl_context *ctx, const struct marshal_cmd_Uniform2iv *cmd, const uint64_t *last)
{
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLint * value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLint *) variable_data;
   CALL_Uniform2iv(ctx->CurrentServerDispatch, (location, count, value));
   return cmd->cmd_base.cmd_size;
}
void GLAPIENTRY
_mesa_marshal_Uniform2iv(GLint location, GLsizei count, const GLint * value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 2 * sizeof(GLint));
   int cmd_size = sizeof(struct marshal_cmd_Uniform2iv) + value_size;
   struct marshal_cmd_Uniform2iv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "Uniform2iv");
      CALL_Uniform2iv(ctx->CurrentServerDispatch, (location, count, value));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Uniform2iv, cmd_size);
   cmd->location = location;
   cmd->count = count;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, value, value_size);
}


/* Uniform3iv: marshalled asynchronously */
struct marshal_cmd_Uniform3iv
{
   struct marshal_cmd_base cmd_base;
   GLint location;
   GLsizei count;
   /* Next safe_mul(count, 3 * sizeof(GLint)) bytes are GLint value[count][3] */
};
uint32_t
_mesa_unmarshal_Uniform3iv(struct gl_context *ctx, const struct marshal_cmd_Uniform3iv *cmd, const uint64_t *last)
{
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLint * value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLint *) variable_data;
   CALL_Uniform3iv(ctx->CurrentServerDispatch, (location, count, value));
   return cmd->cmd_base.cmd_size;
}
void GLAPIENTRY
_mesa_marshal_Uniform3iv(GLint location, GLsizei count, const GLint * value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 3 * sizeof(GLint));
   int cmd_size = sizeof(struct marshal_cmd_Uniform3iv) + value_size;
   struct marshal_cmd_Uniform3iv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "Uniform3iv");
      CALL_Uniform3iv(ctx->CurrentServerDispatch, (location, count, value));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Uniform3iv, cmd_size);
   cmd->location = location;
   cmd->count = count;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, value, value_size);
}


/* Uniform4iv: marshalled asynchronously */
struct marshal_cmd_Uniform4iv
{
   struct marshal_cmd_base cmd_base;
   GLint location;
   GLsizei count;
   /* Next safe_mul(count, 4 * sizeof(GLint)) bytes are GLint value[count][4] */
};
uint32_t
_mesa_unmarshal_Uniform4iv(struct gl_context *ctx, const struct marshal_cmd_Uniform4iv *cmd, const uint64_t *last)
{
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLint * value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLint *) variable_data;
   CALL_Uniform4iv(ctx->CurrentServerDispatch, (location, count, value));
   return cmd->cmd_base.cmd_size;
}
void GLAPIENTRY
_mesa_marshal_Uniform4iv(GLint location, GLsizei count, const GLint * value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 4 * sizeof(GLint));
   int cmd_size = sizeof(struct marshal_cmd_Uniform4iv) + value_size;
   struct marshal_cmd_Uniform4iv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "Uniform4iv");
      CALL_Uniform4iv(ctx->CurrentServerDispatch, (location, count, value));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Uniform4iv, cmd_size);
   cmd->location = location;
   cmd->count = count;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, value, value_size);
}


/* UniformMatrix2fv: marshalled asynchronously */
struct marshal_cmd_UniformMatrix2fv
{
   struct marshal_cmd_base cmd_base;
   GLboolean transpose;
   GLint location;
   GLsizei count;
   /* Next safe_mul(count, 4 * sizeof(GLfloat)) bytes are GLfloat value[count][4] */
};
uint32_t
_mesa_unmarshal_UniformMatrix2fv(struct gl_context *ctx, const struct marshal_cmd_UniformMatrix2fv *cmd, const uint64_t *last)
{
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLboolean transpose = cmd->transpose;
   GLfloat * value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLfloat *) variable_data;
   CALL_UniformMatrix2fv(ctx->CurrentServerDispatch, (location, count, transpose, value));
   return cmd->cmd_base.cmd_size;
}
void GLAPIENTRY
_mesa_marshal_UniformMatrix2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat * value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 4 * sizeof(GLfloat));
   int cmd_size = sizeof(struct marshal_cmd_UniformMatrix2fv) + value_size;
   struct marshal_cmd_UniformMatrix2fv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "UniformMatrix2fv");
      CALL_UniformMatrix2fv(ctx->CurrentServerDispatch, (location, count, transpose, value));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_UniformMatrix2fv, cmd_size);
   cmd->location = location;
   cmd->count = count;
   cmd->transpose = transpose;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, value, value_size);
}


/* UniformMatrix3fv: marshalled asynchronously */
struct marshal_cmd_UniformMatrix3fv
{
   struct marshal_cmd_base cmd_base;
   GLboolean transpose;
   GLint location;
   GLsizei count;
   /* Next safe_mul(count, 9 * sizeof(GLfloat)) bytes are GLfloat value[count][9] */
};
uint32_t
_mesa_unmarshal_UniformMatrix3fv(struct gl_context *ctx, const struct marshal_cmd_UniformMatrix3fv *cmd, const uint64_t *last)
{
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLboolean transpose = cmd->transpose;
   GLfloat * value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLfloat *) variable_data;
   CALL_UniformMatrix3fv(ctx->CurrentServerDispatch, (location, count, transpose, value));
   return cmd->cmd_base.cmd_size;
}
void GLAPIENTRY
_mesa_marshal_UniformMatrix3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat * value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 9 * sizeof(GLfloat));
   int cmd_size = sizeof(struct marshal_cmd_UniformMatrix3fv) + value_size;
   struct marshal_cmd_UniformMatrix3fv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "UniformMatrix3fv");
      CALL_UniformMatrix3fv(ctx->CurrentServerDispatch, (location, count, transpose, value));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_UniformMatrix3fv, cmd_size);
   cmd->location = location;
   cmd->count = count;
   cmd->transpose = transpose;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, value, value_size);
}


/* UniformMatrix4fv: marshalled asynchronously */
struct marshal_cmd_UniformMatrix4fv
{
   struct marshal_cmd_base cmd_base;
   GLboolean transpose;
   GLint location;
   GLsizei count;
   /* Next safe_mul(count, 16 * sizeof(GLfloat)) bytes are GLfloat value[count][16] */
};
uint32_t
_mesa_unmarshal_UniformMatrix4fv(struct gl_context *ctx, const struct marshal_cmd_UniformMatrix4fv *cmd, const uint64_t *last)
{
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLboolean transpose = cmd->transpose;
   GLfloat * value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLfloat *) variable_data;
   CALL_UniformMatrix4fv(ctx->CurrentServerDispatch, (location, count, transpose, value));
   return cmd->cmd_base.cmd_size;
}
void GLAPIENTRY
_mesa_marshal_UniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat * value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 16 * sizeof(GLfloat));
   int cmd_size = sizeof(struct marshal_cmd_UniformMatrix4fv) + value_size;
   struct marshal_cmd_UniformMatrix4fv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "UniformMatrix4fv");
      CALL_UniformMatrix4fv(ctx->CurrentServerDispatch, (location, count, transpose, value));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_UniformMatrix4fv, cmd_size);
   cmd->location = location;
   cmd->count = count;
   cmd->transpose = transpose;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, value, value_size);
}


/* ValidateProgram: marshalled asynchronously */
struct marshal_cmd_ValidateProgram
{
   struct marshal_cmd_base cmd_base;
   GLuint program;
};
uint32_t
_mesa_unmarshal_ValidateProgram(struct gl_context *ctx, const struct marshal_cmd_ValidateProgram *cmd, const uint64_t *last)
{
   GLuint program = cmd->program;
   CALL_ValidateProgram(ctx->CurrentServerDispatch, (program));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_ValidateProgram), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_ValidateProgram(GLuint program)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ValidateProgram);
   struct marshal_cmd_ValidateProgram *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ValidateProgram, cmd_size);
   cmd->program = program;
}


/* VertexAttrib1d: marshalled asynchronously */
struct marshal_cmd_VertexAttrib1d
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLdouble x;
};
uint32_t
_mesa_unmarshal_VertexAttrib1d(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib1d *cmd, const uint64_t *last)
{
   GLuint index = cmd->index;
   GLdouble x = cmd->x;
   CALL_VertexAttrib1d(ctx->CurrentServerDispatch, (index, x));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexAttrib1d), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_VertexAttrib1d(GLuint index, GLdouble x)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttrib1d);
   struct marshal_cmd_VertexAttrib1d *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttrib1d, cmd_size);
   cmd->index = index;
   cmd->x = x;
}


/* VertexAttrib1dv: marshalled asynchronously */
struct marshal_cmd_VertexAttrib1dv
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLdouble v[1];
};
uint32_t
_mesa_unmarshal_VertexAttrib1dv(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib1dv *cmd, const uint64_t *last)
{
   GLuint index = cmd->index;
   const GLdouble * v = cmd->v;
   CALL_VertexAttrib1dv(ctx->CurrentServerDispatch, (index, v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexAttrib1dv), 8) / 8);
   assert (cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_VertexAttrib1dv(GLuint index, const GLdouble * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttrib1dv);
   struct marshal_cmd_VertexAttrib1dv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttrib1dv, cmd_size);
   cmd->index = index;
   memcpy(cmd->v, v, 1 * sizeof(GLdouble));
}


