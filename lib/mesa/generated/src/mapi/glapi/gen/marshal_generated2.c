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

/* PointParameterfv: marshalled asynchronously */
struct marshal_cmd_PointParameterfv
{
   struct marshal_cmd_base cmd_base;
   GLenum16 pname;
   /* Next safe_mul(_mesa_point_param_enum_to_count(pname), 1 * sizeof(GLfloat)) bytes are GLfloat params[None] */
};
uint32_t
_mesa_unmarshal_PointParameterfv(struct gl_context *ctx, const struct marshal_cmd_PointParameterfv *restrict cmd)
{
   GLenum pname = cmd->pname;
   GLfloat *params;
   const char *variable_data = (const char *) (cmd + 1);
   params = (GLfloat *) variable_data;
   CALL_PointParameterfv(ctx->Dispatch.Current, (pname, params));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_PointParameterfv(GLenum pname, const GLfloat *params)
{
   GET_CURRENT_CONTEXT(ctx);
   int params_size = safe_mul(_mesa_point_param_enum_to_count(pname), 1 * sizeof(GLfloat));
   int cmd_size = sizeof(struct marshal_cmd_PointParameterfv) + params_size;
   struct marshal_cmd_PointParameterfv *cmd;
   if (unlikely(params_size < 0 || (params_size > 0 && !params) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "PointParameterfv");
      CALL_PointParameterfv(ctx->Dispatch.Current, (pname, params));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_PointParameterfv, cmd_size);
   cmd->pname = MIN2(pname, 0xffff); /* clamped to 0xffff (invalid enum) */
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, params, params_size);
}


/* PointParameteri: marshalled asynchronously */
struct marshal_cmd_PointParameteri
{
   struct marshal_cmd_base cmd_base;
   GLenum16 pname;
   GLint param;
};
uint32_t
_mesa_unmarshal_PointParameteri(struct gl_context *ctx, const struct marshal_cmd_PointParameteri *restrict cmd)
{
   GLenum pname = cmd->pname;
   GLint param = cmd->param;
   CALL_PointParameteri(ctx->Dispatch.Current, (pname, param));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_PointParameteri), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_PointParameteri(GLenum pname, GLint param)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_PointParameteri);
   struct marshal_cmd_PointParameteri *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_PointParameteri, cmd_size);
   cmd->pname = MIN2(pname, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->param = param;
}


/* PointParameteriv: marshalled asynchronously */
struct marshal_cmd_PointParameteriv
{
   struct marshal_cmd_base cmd_base;
   GLenum16 pname;
   /* Next safe_mul(_mesa_point_param_enum_to_count(pname), 1 * sizeof(GLint)) bytes are GLint params[None] */
};
uint32_t
_mesa_unmarshal_PointParameteriv(struct gl_context *ctx, const struct marshal_cmd_PointParameteriv *restrict cmd)
{
   GLenum pname = cmd->pname;
   GLint *params;
   const char *variable_data = (const char *) (cmd + 1);
   params = (GLint *) variable_data;
   CALL_PointParameteriv(ctx->Dispatch.Current, (pname, params));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_PointParameteriv(GLenum pname, const GLint *params)
{
   GET_CURRENT_CONTEXT(ctx);
   int params_size = safe_mul(_mesa_point_param_enum_to_count(pname), 1 * sizeof(GLint));
   int cmd_size = sizeof(struct marshal_cmd_PointParameteriv) + params_size;
   struct marshal_cmd_PointParameteriv *cmd;
   if (unlikely(params_size < 0 || (params_size > 0 && !params) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "PointParameteriv");
      CALL_PointParameteriv(ctx->Dispatch.Current, (pname, params));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_PointParameteriv, cmd_size);
   cmd->pname = MIN2(pname, 0xffff); /* clamped to 0xffff (invalid enum) */
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
_mesa_unmarshal_SecondaryColor3b(struct gl_context *ctx, const struct marshal_cmd_SecondaryColor3b *restrict cmd)
{
   GLbyte red = cmd->red;
   GLbyte green = cmd->green;
   GLbyte blue = cmd->blue;
   CALL_SecondaryColor3b(ctx->Dispatch.Current, (red, green, blue));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_SecondaryColor3b), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
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
_mesa_unmarshal_SecondaryColor3bv(struct gl_context *ctx, const struct marshal_cmd_SecondaryColor3bv *restrict cmd)
{
   const GLbyte *v = cmd->v;
   CALL_SecondaryColor3bv(ctx->Dispatch.Current, (v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_SecondaryColor3bv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_SecondaryColor3bv(const GLbyte *v)
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
_mesa_unmarshal_SecondaryColor3d(struct gl_context *ctx, const struct marshal_cmd_SecondaryColor3d *restrict cmd)
{
   GLdouble red = cmd->red;
   GLdouble green = cmd->green;
   GLdouble blue = cmd->blue;
   CALL_SecondaryColor3d(ctx->Dispatch.Current, (red, green, blue));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_SecondaryColor3d), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
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
_mesa_unmarshal_SecondaryColor3dv(struct gl_context *ctx, const struct marshal_cmd_SecondaryColor3dv *restrict cmd)
{
   const GLdouble *v = cmd->v;
   CALL_SecondaryColor3dv(ctx->Dispatch.Current, (v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_SecondaryColor3dv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_SecondaryColor3dv(const GLdouble *v)
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
_mesa_unmarshal_SecondaryColor3fEXT(struct gl_context *ctx, const struct marshal_cmd_SecondaryColor3fEXT *restrict cmd)
{
   GLfloat red = cmd->red;
   GLfloat green = cmd->green;
   GLfloat blue = cmd->blue;
   CALL_SecondaryColor3fEXT(ctx->Dispatch.Current, (red, green, blue));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_SecondaryColor3fEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
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
_mesa_unmarshal_SecondaryColor3fvEXT(struct gl_context *ctx, const struct marshal_cmd_SecondaryColor3fvEXT *restrict cmd)
{
   const GLfloat *v = cmd->v;
   CALL_SecondaryColor3fvEXT(ctx->Dispatch.Current, (v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_SecondaryColor3fvEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_SecondaryColor3fvEXT(const GLfloat *v)
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
_mesa_unmarshal_SecondaryColor3i(struct gl_context *ctx, const struct marshal_cmd_SecondaryColor3i *restrict cmd)
{
   GLint red = cmd->red;
   GLint green = cmd->green;
   GLint blue = cmd->blue;
   CALL_SecondaryColor3i(ctx->Dispatch.Current, (red, green, blue));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_SecondaryColor3i), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
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
_mesa_unmarshal_SecondaryColor3iv(struct gl_context *ctx, const struct marshal_cmd_SecondaryColor3iv *restrict cmd)
{
   const GLint *v = cmd->v;
   CALL_SecondaryColor3iv(ctx->Dispatch.Current, (v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_SecondaryColor3iv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_SecondaryColor3iv(const GLint *v)
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
_mesa_unmarshal_SecondaryColor3s(struct gl_context *ctx, const struct marshal_cmd_SecondaryColor3s *restrict cmd)
{
   GLshort red = cmd->red;
   GLshort green = cmd->green;
   GLshort blue = cmd->blue;
   CALL_SecondaryColor3s(ctx->Dispatch.Current, (red, green, blue));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_SecondaryColor3s), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
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
_mesa_unmarshal_SecondaryColor3sv(struct gl_context *ctx, const struct marshal_cmd_SecondaryColor3sv *restrict cmd)
{
   const GLshort *v = cmd->v;
   CALL_SecondaryColor3sv(ctx->Dispatch.Current, (v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_SecondaryColor3sv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_SecondaryColor3sv(const GLshort *v)
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
_mesa_unmarshal_SecondaryColor3ub(struct gl_context *ctx, const struct marshal_cmd_SecondaryColor3ub *restrict cmd)
{
   GLubyte red = cmd->red;
   GLubyte green = cmd->green;
   GLubyte blue = cmd->blue;
   CALL_SecondaryColor3ub(ctx->Dispatch.Current, (red, green, blue));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_SecondaryColor3ub), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
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
_mesa_unmarshal_SecondaryColor3ubv(struct gl_context *ctx, const struct marshal_cmd_SecondaryColor3ubv *restrict cmd)
{
   const GLubyte *v = cmd->v;
   CALL_SecondaryColor3ubv(ctx->Dispatch.Current, (v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_SecondaryColor3ubv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_SecondaryColor3ubv(const GLubyte *v)
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
_mesa_unmarshal_SecondaryColor3ui(struct gl_context *ctx, const struct marshal_cmd_SecondaryColor3ui *restrict cmd)
{
   GLuint red = cmd->red;
   GLuint green = cmd->green;
   GLuint blue = cmd->blue;
   CALL_SecondaryColor3ui(ctx->Dispatch.Current, (red, green, blue));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_SecondaryColor3ui), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
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
_mesa_unmarshal_SecondaryColor3uiv(struct gl_context *ctx, const struct marshal_cmd_SecondaryColor3uiv *restrict cmd)
{
   const GLuint *v = cmd->v;
   CALL_SecondaryColor3uiv(ctx->Dispatch.Current, (v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_SecondaryColor3uiv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_SecondaryColor3uiv(const GLuint *v)
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
_mesa_unmarshal_SecondaryColor3us(struct gl_context *ctx, const struct marshal_cmd_SecondaryColor3us *restrict cmd)
{
   GLushort red = cmd->red;
   GLushort green = cmd->green;
   GLushort blue = cmd->blue;
   CALL_SecondaryColor3us(ctx->Dispatch.Current, (red, green, blue));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_SecondaryColor3us), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
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
_mesa_unmarshal_SecondaryColor3usv(struct gl_context *ctx, const struct marshal_cmd_SecondaryColor3usv *restrict cmd)
{
   const GLushort *v = cmd->v;
   CALL_SecondaryColor3usv(ctx->Dispatch.Current, (v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_SecondaryColor3usv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_SecondaryColor3usv(const GLushort *v)
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
   GLenum16 type;
   GLint size;
   GLsizei stride;
   const GLvoid * pointer;
};
uint32_t
_mesa_unmarshal_SecondaryColorPointer(struct gl_context *ctx, const struct marshal_cmd_SecondaryColorPointer *restrict cmd)
{
   GLint size = cmd->size;
   GLenum type = cmd->type;
   GLsizei stride = cmd->stride;
   const GLvoid * pointer = cmd->pointer;
   CALL_SecondaryColorPointer(ctx->Dispatch.Current, (size, type, stride, pointer));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_SecondaryColorPointer), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_SecondaryColorPointer(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_SecondaryColorPointer);
   struct marshal_cmd_SecondaryColorPointer *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_SecondaryColorPointer, cmd_size);
   cmd->size = size;
   cmd->type = MIN2(type, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->stride = stride;
   cmd->pointer = pointer;
   if (COMPAT) _mesa_glthread_AttribPointer(ctx, VERT_ATTRIB_COLOR1, MESA_PACK_VFORMAT(type, size, 1, 0, 0), stride, pointer);
}


/* WindowPos2d: marshalled asynchronously */
struct marshal_cmd_WindowPos2d
{
   struct marshal_cmd_base cmd_base;
   GLdouble x;
   GLdouble y;
};
uint32_t
_mesa_unmarshal_WindowPos2d(struct gl_context *ctx, const struct marshal_cmd_WindowPos2d *restrict cmd)
{
   GLdouble x = cmd->x;
   GLdouble y = cmd->y;
   CALL_WindowPos2d(ctx->Dispatch.Current, (x, y));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_WindowPos2d), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
_mesa_unmarshal_WindowPos2dv(struct gl_context *ctx, const struct marshal_cmd_WindowPos2dv *restrict cmd)
{
   const GLdouble *v = cmd->v;
   CALL_WindowPos2dv(ctx->Dispatch.Current, (v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_WindowPos2dv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_WindowPos2dv(const GLdouble *v)
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
_mesa_unmarshal_WindowPos2f(struct gl_context *ctx, const struct marshal_cmd_WindowPos2f *restrict cmd)
{
   GLfloat x = cmd->x;
   GLfloat y = cmd->y;
   CALL_WindowPos2f(ctx->Dispatch.Current, (x, y));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_WindowPos2f), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
_mesa_unmarshal_WindowPos2fv(struct gl_context *ctx, const struct marshal_cmd_WindowPos2fv *restrict cmd)
{
   const GLfloat *v = cmd->v;
   CALL_WindowPos2fv(ctx->Dispatch.Current, (v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_WindowPos2fv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_WindowPos2fv(const GLfloat *v)
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
_mesa_unmarshal_WindowPos2i(struct gl_context *ctx, const struct marshal_cmd_WindowPos2i *restrict cmd)
{
   GLint x = cmd->x;
   GLint y = cmd->y;
   CALL_WindowPos2i(ctx->Dispatch.Current, (x, y));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_WindowPos2i), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
_mesa_unmarshal_WindowPos2iv(struct gl_context *ctx, const struct marshal_cmd_WindowPos2iv *restrict cmd)
{
   const GLint *v = cmd->v;
   CALL_WindowPos2iv(ctx->Dispatch.Current, (v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_WindowPos2iv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_WindowPos2iv(const GLint *v)
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
_mesa_unmarshal_WindowPos2s(struct gl_context *ctx, const struct marshal_cmd_WindowPos2s *restrict cmd)
{
   GLshort x = cmd->x;
   GLshort y = cmd->y;
   CALL_WindowPos2s(ctx->Dispatch.Current, (x, y));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_WindowPos2s), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
_mesa_unmarshal_WindowPos2sv(struct gl_context *ctx, const struct marshal_cmd_WindowPos2sv *restrict cmd)
{
   const GLshort *v = cmd->v;
   CALL_WindowPos2sv(ctx->Dispatch.Current, (v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_WindowPos2sv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_WindowPos2sv(const GLshort *v)
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
_mesa_unmarshal_WindowPos3d(struct gl_context *ctx, const struct marshal_cmd_WindowPos3d *restrict cmd)
{
   GLdouble x = cmd->x;
   GLdouble y = cmd->y;
   GLdouble z = cmd->z;
   CALL_WindowPos3d(ctx->Dispatch.Current, (x, y, z));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_WindowPos3d), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
_mesa_unmarshal_WindowPos3dv(struct gl_context *ctx, const struct marshal_cmd_WindowPos3dv *restrict cmd)
{
   const GLdouble *v = cmd->v;
   CALL_WindowPos3dv(ctx->Dispatch.Current, (v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_WindowPos3dv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_WindowPos3dv(const GLdouble *v)
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
_mesa_unmarshal_WindowPos3f(struct gl_context *ctx, const struct marshal_cmd_WindowPos3f *restrict cmd)
{
   GLfloat x = cmd->x;
   GLfloat y = cmd->y;
   GLfloat z = cmd->z;
   CALL_WindowPos3f(ctx->Dispatch.Current, (x, y, z));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_WindowPos3f), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
_mesa_unmarshal_WindowPos3fv(struct gl_context *ctx, const struct marshal_cmd_WindowPos3fv *restrict cmd)
{
   const GLfloat *v = cmd->v;
   CALL_WindowPos3fv(ctx->Dispatch.Current, (v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_WindowPos3fv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_WindowPos3fv(const GLfloat *v)
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
_mesa_unmarshal_WindowPos3i(struct gl_context *ctx, const struct marshal_cmd_WindowPos3i *restrict cmd)
{
   GLint x = cmd->x;
   GLint y = cmd->y;
   GLint z = cmd->z;
   CALL_WindowPos3i(ctx->Dispatch.Current, (x, y, z));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_WindowPos3i), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
_mesa_unmarshal_WindowPos3iv(struct gl_context *ctx, const struct marshal_cmd_WindowPos3iv *restrict cmd)
{
   const GLint *v = cmd->v;
   CALL_WindowPos3iv(ctx->Dispatch.Current, (v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_WindowPos3iv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_WindowPos3iv(const GLint *v)
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
_mesa_unmarshal_WindowPos3s(struct gl_context *ctx, const struct marshal_cmd_WindowPos3s *restrict cmd)
{
   GLshort x = cmd->x;
   GLshort y = cmd->y;
   GLshort z = cmd->z;
   CALL_WindowPos3s(ctx->Dispatch.Current, (x, y, z));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_WindowPos3s), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
_mesa_unmarshal_WindowPos3sv(struct gl_context *ctx, const struct marshal_cmd_WindowPos3sv *restrict cmd)
{
   const GLshort *v = cmd->v;
   CALL_WindowPos3sv(ctx->Dispatch.Current, (v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_WindowPos3sv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_WindowPos3sv(const GLshort *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_WindowPos3sv);
   struct marshal_cmd_WindowPos3sv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_WindowPos3sv, cmd_size);
   memcpy(cmd->v, v, 3 * sizeof(GLshort));
}


/* DeleteBuffers: marshalled asynchronously */
struct marshal_cmd_DeleteBuffers
{
   struct marshal_cmd_base cmd_base;
   GLsizei n;
   /* Next safe_mul(n, 1 * sizeof(GLuint)) bytes are GLuint buffer[n] */
};
uint32_t
_mesa_unmarshal_DeleteBuffers(struct gl_context *ctx, const struct marshal_cmd_DeleteBuffers *restrict cmd)
{
   GLsizei n = cmd->n;
   GLuint *buffer;
   const char *variable_data = (const char *) (cmd + 1);
   buffer = (GLuint *) variable_data;
   CALL_DeleteBuffers(ctx->Dispatch.Current, (n, buffer));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_DeleteBuffers(GLsizei n, const GLuint *buffer)
{
   GET_CURRENT_CONTEXT(ctx);
   int buffer_size = safe_mul(n, 1 * sizeof(GLuint));
   int cmd_size = sizeof(struct marshal_cmd_DeleteBuffers) + buffer_size;
   struct marshal_cmd_DeleteBuffers *cmd;
   if (unlikely(buffer_size < 0 || (buffer_size > 0 && !buffer) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "DeleteBuffers");
      CALL_DeleteBuffers(ctx->Dispatch.Current, (n, buffer));
      _mesa_glthread_DeleteBuffers(ctx, n, buffer);
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_DeleteBuffers, cmd_size);
   cmd->n = n;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, buffer, buffer_size);
   _mesa_glthread_DeleteBuffers(ctx, n, buffer);
}


/* GenBuffers: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GenBuffers(GLsizei n, GLuint *buffer)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GenBuffers");
   CALL_GenBuffers(ctx->Dispatch.Current, (n, buffer));
}


/* GetBufferParameteriv: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetBufferParameteriv(GLenum target, GLenum pname, GLint *params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetBufferParameteriv");
   CALL_GetBufferParameteriv(ctx->Dispatch.Current, (target, pname, params));
}


/* GetBufferPointerv: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetBufferPointerv(GLenum target, GLenum pname, GLvoid **params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetBufferPointerv");
   CALL_GetBufferPointerv(ctx->Dispatch.Current, (target, pname, params));
}


/* GetBufferSubData: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetBufferSubData(GLenum target, GLintptr offset, GLsizeiptr size, GLvoid *data)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetBufferSubData");
   CALL_GetBufferSubData(ctx->Dispatch.Current, (target, offset, size, data));
}


/* IsBuffer: marshalled synchronously */
static GLboolean GLAPIENTRY
_mesa_marshal_IsBuffer(GLuint buffer)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "IsBuffer");
   return CALL_IsBuffer(ctx->Dispatch.Current, (buffer));
}


/* MapBuffer: marshalled synchronously */
static GLvoid * GLAPIENTRY
_mesa_marshal_MapBuffer(GLenum target, GLenum access)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "MapBuffer");
   return CALL_MapBuffer(ctx->Dispatch.Current, (target, access));
}


/* UnmapBuffer: marshalled asynchronously */
struct marshal_cmd_UnmapBuffer
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
};
uint32_t
_mesa_unmarshal_UnmapBuffer(struct gl_context *ctx, const struct marshal_cmd_UnmapBuffer *restrict cmd)
{
   GLenum target = cmd->target;
   CALL_UnmapBuffer(ctx->Dispatch.Current, (target));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_UnmapBuffer), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static GLboolean GLAPIENTRY
_mesa_marshal_UnmapBuffer(GLenum target)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_UnmapBuffer);
   struct marshal_cmd_UnmapBuffer *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_UnmapBuffer, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   return GL_TRUE;
}


/* GenQueries: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GenQueries(GLsizei n, GLuint *ids)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GenQueries");
   CALL_GenQueries(ctx->Dispatch.Current, (n, ids));
}


/* DeleteQueries: marshalled asynchronously */
struct marshal_cmd_DeleteQueries
{
   struct marshal_cmd_base cmd_base;
   GLsizei n;
   /* Next safe_mul(n, 1 * sizeof(GLuint)) bytes are GLuint ids[n] */
};
uint32_t
_mesa_unmarshal_DeleteQueries(struct gl_context *ctx, const struct marshal_cmd_DeleteQueries *restrict cmd)
{
   GLsizei n = cmd->n;
   GLuint *ids;
   const char *variable_data = (const char *) (cmd + 1);
   ids = (GLuint *) variable_data;
   CALL_DeleteQueries(ctx->Dispatch.Current, (n, ids));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_DeleteQueries(GLsizei n, const GLuint *ids)
{
   GET_CURRENT_CONTEXT(ctx);
   int ids_size = safe_mul(n, 1 * sizeof(GLuint));
   int cmd_size = sizeof(struct marshal_cmd_DeleteQueries) + ids_size;
   struct marshal_cmd_DeleteQueries *cmd;
   if (unlikely(ids_size < 0 || (ids_size > 0 && !ids) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "DeleteQueries");
      CALL_DeleteQueries(ctx->Dispatch.Current, (n, ids));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_DeleteQueries, cmd_size);
   cmd->n = n;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, ids, ids_size);
}


/* IsQuery: marshalled synchronously */
static GLboolean GLAPIENTRY
_mesa_marshal_IsQuery(GLuint id)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "IsQuery");
   return CALL_IsQuery(ctx->Dispatch.Current, (id));
}


/* BeginQuery: marshalled asynchronously */
struct marshal_cmd_BeginQuery
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLuint id;
};
uint32_t
_mesa_unmarshal_BeginQuery(struct gl_context *ctx, const struct marshal_cmd_BeginQuery *restrict cmd)
{
   GLenum target = cmd->target;
   GLuint id = cmd->id;
   CALL_BeginQuery(ctx->Dispatch.Current, (target, id));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_BeginQuery), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_BeginQuery(GLenum target, GLuint id)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_BeginQuery);
   struct marshal_cmd_BeginQuery *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_BeginQuery, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->id = id;
}


/* EndQuery: marshalled asynchronously */
struct marshal_cmd_EndQuery
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
};
uint32_t
_mesa_unmarshal_EndQuery(struct gl_context *ctx, const struct marshal_cmd_EndQuery *restrict cmd)
{
   GLenum target = cmd->target;
   CALL_EndQuery(ctx->Dispatch.Current, (target));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_EndQuery), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_EndQuery(GLenum target)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_EndQuery);
   struct marshal_cmd_EndQuery *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_EndQuery, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
}


/* GetQueryiv: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetQueryiv(GLenum target, GLenum pname, GLint *params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetQueryiv");
   CALL_GetQueryiv(ctx->Dispatch.Current, (target, pname, params));
}


/* GetQueryObjectiv: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetQueryObjectiv(GLuint id, GLenum pname, GLint *params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetQueryObjectiv");
   CALL_GetQueryObjectiv(ctx->Dispatch.Current, (id, pname, params));
}


/* GetQueryObjectuiv: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetQueryObjectuiv(GLuint id, GLenum pname, GLuint *params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetQueryObjectuiv");
   CALL_GetQueryObjectuiv(ctx->Dispatch.Current, (id, pname, params));
}


/* BlendEquationSeparate: marshalled asynchronously */
struct marshal_cmd_BlendEquationSeparate
{
   struct marshal_cmd_base cmd_base;
   GLenum16 modeRGB;
   GLenum16 modeA;
};
uint32_t
_mesa_unmarshal_BlendEquationSeparate(struct gl_context *ctx, const struct marshal_cmd_BlendEquationSeparate *restrict cmd)
{
   GLenum modeRGB = cmd->modeRGB;
   GLenum modeA = cmd->modeA;
   CALL_BlendEquationSeparate(ctx->Dispatch.Current, (modeRGB, modeA));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_BlendEquationSeparate), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_BlendEquationSeparate(GLenum modeRGB, GLenum modeA)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_BlendEquationSeparate);
   struct marshal_cmd_BlendEquationSeparate *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_BlendEquationSeparate, cmd_size);
   cmd->modeRGB = MIN2(modeRGB, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->modeA = MIN2(modeA, 0xffff); /* clamped to 0xffff (invalid enum) */
}


/* DrawBuffers: marshalled asynchronously */
struct marshal_cmd_DrawBuffers
{
   struct marshal_cmd_base cmd_base;
   GLsizei n;
   /* Next safe_mul(n, 1 * sizeof(GLenum)) bytes are GLenum bufs[n] */
};
uint32_t
_mesa_unmarshal_DrawBuffers(struct gl_context *ctx, const struct marshal_cmd_DrawBuffers *restrict cmd)
{
   GLsizei n = cmd->n;
   GLenum *bufs;
   const char *variable_data = (const char *) (cmd + 1);
   bufs = (GLenum *) variable_data;
   CALL_DrawBuffers(ctx->Dispatch.Current, (n, bufs));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_DrawBuffers(GLsizei n, const GLenum *bufs)
{
   GET_CURRENT_CONTEXT(ctx);
   int bufs_size = safe_mul(n, 1 * sizeof(GLenum));
   int cmd_size = sizeof(struct marshal_cmd_DrawBuffers) + bufs_size;
   struct marshal_cmd_DrawBuffers *cmd;
   if (unlikely(bufs_size < 0 || (bufs_size > 0 && !bufs) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "DrawBuffers");
      CALL_DrawBuffers(ctx->Dispatch.Current, (n, bufs));
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
   GLenum16 face;
   GLenum16 func;
   GLint ref;
   GLuint mask;
};
uint32_t
_mesa_unmarshal_StencilFuncSeparate(struct gl_context *ctx, const struct marshal_cmd_StencilFuncSeparate *restrict cmd)
{
   GLenum face = cmd->face;
   GLenum func = cmd->func;
   GLint ref = cmd->ref;
   GLuint mask = cmd->mask;
   CALL_StencilFuncSeparate(ctx->Dispatch.Current, (face, func, ref, mask));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_StencilFuncSeparate), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_StencilFuncSeparate(GLenum face, GLenum func, GLint ref, GLuint mask)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_StencilFuncSeparate);
   struct marshal_cmd_StencilFuncSeparate *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_StencilFuncSeparate, cmd_size);
   cmd->face = MIN2(face, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->func = MIN2(func, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->ref = ref;
   cmd->mask = mask;
}


/* StencilOpSeparate: marshalled asynchronously */
struct marshal_cmd_StencilOpSeparate
{
   struct marshal_cmd_base cmd_base;
   GLenum16 face;
   GLenum16 sfail;
   GLenum16 zfail;
   GLenum16 zpass;
};
uint32_t
_mesa_unmarshal_StencilOpSeparate(struct gl_context *ctx, const struct marshal_cmd_StencilOpSeparate *restrict cmd)
{
   GLenum face = cmd->face;
   GLenum sfail = cmd->sfail;
   GLenum zfail = cmd->zfail;
   GLenum zpass = cmd->zpass;
   CALL_StencilOpSeparate(ctx->Dispatch.Current, (face, sfail, zfail, zpass));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_StencilOpSeparate), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_StencilOpSeparate(GLenum face, GLenum sfail, GLenum zfail, GLenum zpass)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_StencilOpSeparate);
   struct marshal_cmd_StencilOpSeparate *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_StencilOpSeparate, cmd_size);
   cmd->face = MIN2(face, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->sfail = MIN2(sfail, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->zfail = MIN2(zfail, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->zpass = MIN2(zpass, 0xffff); /* clamped to 0xffff (invalid enum) */
}


/* StencilMaskSeparate: marshalled asynchronously */
struct marshal_cmd_StencilMaskSeparate
{
   struct marshal_cmd_base cmd_base;
   GLenum16 face;
   GLuint mask;
};
uint32_t
_mesa_unmarshal_StencilMaskSeparate(struct gl_context *ctx, const struct marshal_cmd_StencilMaskSeparate *restrict cmd)
{
   GLenum face = cmd->face;
   GLuint mask = cmd->mask;
   CALL_StencilMaskSeparate(ctx->Dispatch.Current, (face, mask));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_StencilMaskSeparate), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_StencilMaskSeparate(GLenum face, GLuint mask)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_StencilMaskSeparate);
   struct marshal_cmd_StencilMaskSeparate *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_StencilMaskSeparate, cmd_size);
   cmd->face = MIN2(face, 0xffff); /* clamped to 0xffff (invalid enum) */
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
_mesa_unmarshal_AttachShader(struct gl_context *ctx, const struct marshal_cmd_AttachShader *restrict cmd)
{
   GLuint program = cmd->program;
   GLuint shader = cmd->shader;
   CALL_AttachShader(ctx->Dispatch.Current, (program, shader));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_AttachShader), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
_mesa_unmarshal_BindAttribLocation(struct gl_context *ctx, const struct marshal_cmd_BindAttribLocation *restrict cmd)
{
   GLuint program = cmd->program;
   GLuint index = cmd->index;
   GLchar *name;
   const char *variable_data = (const char *) (cmd + 1);
   name = (GLchar *) variable_data;
   CALL_BindAttribLocation(ctx->Dispatch.Current, (program, index, name));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_BindAttribLocation(GLuint program, GLuint index, const GLchar *name)
{
   GET_CURRENT_CONTEXT(ctx);
   int name_size = (strlen(name) + 1);
   int cmd_size = sizeof(struct marshal_cmd_BindAttribLocation) + name_size;
   struct marshal_cmd_BindAttribLocation *cmd;
   if (unlikely(name_size < 0 || (name_size > 0 && !name) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "BindAttribLocation");
      CALL_BindAttribLocation(ctx->Dispatch.Current, (program, index, name));
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
_mesa_unmarshal_CompileShader(struct gl_context *ctx, const struct marshal_cmd_CompileShader *restrict cmd)
{
   GLuint shader = cmd->shader;
   CALL_CompileShader(ctx->Dispatch.Current, (shader));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_CompileShader), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_CompileShader(GLuint shader)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_CompileShader);
   struct marshal_cmd_CompileShader *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_CompileShader, cmd_size);
   cmd->shader = shader;
}


/* CreateProgram: marshalled synchronously */
static GLuint GLAPIENTRY
_mesa_marshal_CreateProgram(void)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "CreateProgram");
   return CALL_CreateProgram(ctx->Dispatch.Current, ());
}


/* CreateShader: marshalled synchronously */
static GLuint GLAPIENTRY
_mesa_marshal_CreateShader(GLenum type)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "CreateShader");
   return CALL_CreateShader(ctx->Dispatch.Current, (type));
}


/* DeleteProgram: marshalled asynchronously */
struct marshal_cmd_DeleteProgram
{
   struct marshal_cmd_base cmd_base;
   GLuint program;
};
uint32_t
_mesa_unmarshal_DeleteProgram(struct gl_context *ctx, const struct marshal_cmd_DeleteProgram *restrict cmd)
{
   GLuint program = cmd->program;
   CALL_DeleteProgram(ctx->Dispatch.Current, (program));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_DeleteProgram), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
_mesa_unmarshal_DeleteShader(struct gl_context *ctx, const struct marshal_cmd_DeleteShader *restrict cmd)
{
   GLuint program = cmd->program;
   CALL_DeleteShader(ctx->Dispatch.Current, (program));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_DeleteShader), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
_mesa_unmarshal_DetachShader(struct gl_context *ctx, const struct marshal_cmd_DetachShader *restrict cmd)
{
   GLuint program = cmd->program;
   GLuint shader = cmd->shader;
   CALL_DetachShader(ctx->Dispatch.Current, (program, shader));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_DetachShader), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
_mesa_unmarshal_DisableVertexAttribArray(struct gl_context *ctx, const struct marshal_cmd_DisableVertexAttribArray *restrict cmd)
{
   GLuint index = cmd->index;
   CALL_DisableVertexAttribArray(ctx->Dispatch.Current, (index));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_DisableVertexAttribArray), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
_mesa_unmarshal_EnableVertexAttribArray(struct gl_context *ctx, const struct marshal_cmd_EnableVertexAttribArray *restrict cmd)
{
   GLuint index = cmd->index;
   CALL_EnableVertexAttribArray(ctx->Dispatch.Current, (index));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_EnableVertexAttribArray), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
static void GLAPIENTRY
_mesa_marshal_GetActiveAttrib(GLuint program, GLuint index, GLsizei  bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetActiveAttrib");
   CALL_GetActiveAttrib(ctx->Dispatch.Current, (program, index, bufSize, length, size, type, name));
}


/* GetAttachedShaders: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetAttachedShaders(GLuint program, GLsizei maxCount, GLsizei *count, GLuint *obj)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetAttachedShaders");
   CALL_GetAttachedShaders(ctx->Dispatch.Current, (program, maxCount, count, obj));
}


/* GetAttribLocation: marshalled synchronously */
static GLint GLAPIENTRY
_mesa_marshal_GetAttribLocation(GLuint program, const GLchar *name)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetAttribLocation");
   return CALL_GetAttribLocation(ctx->Dispatch.Current, (program, name));
}


/* GetProgramiv: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetProgramiv(GLuint program, GLenum pname, GLint *params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetProgramiv");
   CALL_GetProgramiv(ctx->Dispatch.Current, (program, pname, params));
}


/* GetProgramInfoLog: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetProgramInfoLog(GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetProgramInfoLog");
   CALL_GetProgramInfoLog(ctx->Dispatch.Current, (program, bufSize, length, infoLog));
}


/* GetShaderiv: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetShaderiv(GLuint shader, GLenum pname, GLint *params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetShaderiv");
   CALL_GetShaderiv(ctx->Dispatch.Current, (shader, pname, params));
}


/* GetShaderInfoLog: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetShaderInfoLog(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetShaderInfoLog");
   CALL_GetShaderInfoLog(ctx->Dispatch.Current, (shader, bufSize, length, infoLog));
}


/* GetShaderSource: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetShaderSource(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *source)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetShaderSource");
   CALL_GetShaderSource(ctx->Dispatch.Current, (shader, bufSize, length, source));
}


/* GetUniformfv: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetUniformfv(GLuint program, GLint location, GLfloat *params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetUniformfv");
   CALL_GetUniformfv(ctx->Dispatch.Current, (program, location, params));
}


/* GetUniformiv: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetUniformiv(GLuint program, GLint location, GLint *params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetUniformiv");
   CALL_GetUniformiv(ctx->Dispatch.Current, (program, location, params));
}


/* GetVertexAttribdv: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetVertexAttribdv(GLuint index, GLenum pname, GLdouble *params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetVertexAttribdv");
   CALL_GetVertexAttribdv(ctx->Dispatch.Current, (index, pname, params));
}


/* GetVertexAttribfv: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetVertexAttribfv(GLuint index, GLenum pname, GLfloat *params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetVertexAttribfv");
   CALL_GetVertexAttribfv(ctx->Dispatch.Current, (index, pname, params));
}


/* GetVertexAttribiv: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetVertexAttribiv(GLuint index, GLenum pname, GLint *params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetVertexAttribiv");
   CALL_GetVertexAttribiv(ctx->Dispatch.Current, (index, pname, params));
}


/* GetVertexAttribPointerv: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetVertexAttribPointerv(GLuint index, GLenum pname, GLvoid **pointer)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetVertexAttribPointerv");
   CALL_GetVertexAttribPointerv(ctx->Dispatch.Current, (index, pname, pointer));
}


/* IsProgram: marshalled synchronously */
static GLboolean GLAPIENTRY
_mesa_marshal_IsProgram(GLuint program)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "IsProgram");
   return CALL_IsProgram(ctx->Dispatch.Current, (program));
}


/* IsShader: marshalled synchronously */
static GLboolean GLAPIENTRY
_mesa_marshal_IsShader(GLuint shader)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "IsShader");
   return CALL_IsShader(ctx->Dispatch.Current, (shader));
}


/* LinkProgram: marshalled asynchronously */
struct marshal_cmd_LinkProgram
{
   struct marshal_cmd_base cmd_base;
   GLuint program;
};
uint32_t
_mesa_unmarshal_LinkProgram(struct gl_context *ctx, const struct marshal_cmd_LinkProgram *restrict cmd)
{
   GLuint program = cmd->program;
   CALL_LinkProgram(ctx->Dispatch.Current, (program));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_LinkProgram), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_LinkProgram(GLuint program)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_LinkProgram);
   struct marshal_cmd_LinkProgram *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_LinkProgram, cmd_size);
   cmd->program = program;
   _mesa_glthread_ProgramChanged(ctx);
}


/* ShaderSource: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_ShaderSource(GLuint shader, GLsizei count, const GLchar * const *string, const GLint *length)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "ShaderSource");
   CALL_ShaderSource(ctx->Dispatch.Current, (shader, count, string, length));
}


/* UseProgram: marshalled asynchronously */
struct marshal_cmd_UseProgram
{
   struct marshal_cmd_base cmd_base;
   GLuint program;
};
uint32_t
_mesa_unmarshal_UseProgram(struct gl_context *ctx, const struct marshal_cmd_UseProgram *restrict cmd)
{
   GLuint program = cmd->program;
   CALL_UseProgram(ctx->Dispatch.Current, (program));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_UseProgram), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_UseProgram(GLuint program)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_UseProgram);
   struct marshal_cmd_UseProgram *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_UseProgram, cmd_size);
   cmd->program = program;
   ctx->GLThread.CurrentProgram = program;
}


/* Uniform1f: marshalled asynchronously */
struct marshal_cmd_Uniform1f
{
   struct marshal_cmd_base cmd_base;
   GLint location;
   GLfloat v0;
};
uint32_t
_mesa_unmarshal_Uniform1f(struct gl_context *ctx, const struct marshal_cmd_Uniform1f *restrict cmd)
{
   GLint location = cmd->location;
   GLfloat v0 = cmd->v0;
   CALL_Uniform1f(ctx->Dispatch.Current, (location, v0));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Uniform1f), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
_mesa_unmarshal_Uniform2f(struct gl_context *ctx, const struct marshal_cmd_Uniform2f *restrict cmd)
{
   GLint location = cmd->location;
   GLfloat v0 = cmd->v0;
   GLfloat v1 = cmd->v1;
   CALL_Uniform2f(ctx->Dispatch.Current, (location, v0, v1));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Uniform2f), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
_mesa_unmarshal_Uniform3f(struct gl_context *ctx, const struct marshal_cmd_Uniform3f *restrict cmd)
{
   GLint location = cmd->location;
   GLfloat v0 = cmd->v0;
   GLfloat v1 = cmd->v1;
   GLfloat v2 = cmd->v2;
   CALL_Uniform3f(ctx->Dispatch.Current, (location, v0, v1, v2));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Uniform3f), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
_mesa_unmarshal_Uniform4f(struct gl_context *ctx, const struct marshal_cmd_Uniform4f *restrict cmd)
{
   GLint location = cmd->location;
   GLfloat v0 = cmd->v0;
   GLfloat v1 = cmd->v1;
   GLfloat v2 = cmd->v2;
   GLfloat v3 = cmd->v3;
   CALL_Uniform4f(ctx->Dispatch.Current, (location, v0, v1, v2, v3));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Uniform4f), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
_mesa_unmarshal_Uniform1i(struct gl_context *ctx, const struct marshal_cmd_Uniform1i *restrict cmd)
{
   GLint location = cmd->location;
   GLint v0 = cmd->v0;
   CALL_Uniform1i(ctx->Dispatch.Current, (location, v0));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Uniform1i), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
_mesa_unmarshal_Uniform2i(struct gl_context *ctx, const struct marshal_cmd_Uniform2i *restrict cmd)
{
   GLint location = cmd->location;
   GLint v0 = cmd->v0;
   GLint v1 = cmd->v1;
   CALL_Uniform2i(ctx->Dispatch.Current, (location, v0, v1));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Uniform2i), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
_mesa_unmarshal_Uniform3i(struct gl_context *ctx, const struct marshal_cmd_Uniform3i *restrict cmd)
{
   GLint location = cmd->location;
   GLint v0 = cmd->v0;
   GLint v1 = cmd->v1;
   GLint v2 = cmd->v2;
   CALL_Uniform3i(ctx->Dispatch.Current, (location, v0, v1, v2));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Uniform3i), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
_mesa_unmarshal_Uniform4i(struct gl_context *ctx, const struct marshal_cmd_Uniform4i *restrict cmd)
{
   GLint location = cmd->location;
   GLint v0 = cmd->v0;
   GLint v1 = cmd->v1;
   GLint v2 = cmd->v2;
   GLint v3 = cmd->v3;
   CALL_Uniform4i(ctx->Dispatch.Current, (location, v0, v1, v2, v3));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Uniform4i), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
_mesa_unmarshal_Uniform1fv(struct gl_context *ctx, const struct marshal_cmd_Uniform1fv *restrict cmd)
{
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLfloat *value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLfloat *) variable_data;
   CALL_Uniform1fv(ctx->Dispatch.Current, (location, count, value));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_Uniform1fv(GLint location, GLsizei count, const GLfloat *value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 1 * sizeof(GLfloat));
   int cmd_size = sizeof(struct marshal_cmd_Uniform1fv) + value_size;
   struct marshal_cmd_Uniform1fv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "Uniform1fv");
      CALL_Uniform1fv(ctx->Dispatch.Current, (location, count, value));
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
_mesa_unmarshal_Uniform2fv(struct gl_context *ctx, const struct marshal_cmd_Uniform2fv *restrict cmd)
{
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLfloat *value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLfloat *) variable_data;
   CALL_Uniform2fv(ctx->Dispatch.Current, (location, count, value));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_Uniform2fv(GLint location, GLsizei count, const GLfloat *value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 2 * sizeof(GLfloat));
   int cmd_size = sizeof(struct marshal_cmd_Uniform2fv) + value_size;
   struct marshal_cmd_Uniform2fv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "Uniform2fv");
      CALL_Uniform2fv(ctx->Dispatch.Current, (location, count, value));
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
_mesa_unmarshal_Uniform3fv(struct gl_context *ctx, const struct marshal_cmd_Uniform3fv *restrict cmd)
{
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLfloat *value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLfloat *) variable_data;
   CALL_Uniform3fv(ctx->Dispatch.Current, (location, count, value));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_Uniform3fv(GLint location, GLsizei count, const GLfloat *value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 3 * sizeof(GLfloat));
   int cmd_size = sizeof(struct marshal_cmd_Uniform3fv) + value_size;
   struct marshal_cmd_Uniform3fv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "Uniform3fv");
      CALL_Uniform3fv(ctx->Dispatch.Current, (location, count, value));
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
_mesa_unmarshal_Uniform4fv(struct gl_context *ctx, const struct marshal_cmd_Uniform4fv *restrict cmd)
{
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLfloat *value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLfloat *) variable_data;
   CALL_Uniform4fv(ctx->Dispatch.Current, (location, count, value));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_Uniform4fv(GLint location, GLsizei count, const GLfloat *value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 4 * sizeof(GLfloat));
   int cmd_size = sizeof(struct marshal_cmd_Uniform4fv) + value_size;
   struct marshal_cmd_Uniform4fv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "Uniform4fv");
      CALL_Uniform4fv(ctx->Dispatch.Current, (location, count, value));
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
_mesa_unmarshal_Uniform1iv(struct gl_context *ctx, const struct marshal_cmd_Uniform1iv *restrict cmd)
{
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLint *value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLint *) variable_data;
   CALL_Uniform1iv(ctx->Dispatch.Current, (location, count, value));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_Uniform1iv(GLint location, GLsizei count, const GLint *value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 1 * sizeof(GLint));
   int cmd_size = sizeof(struct marshal_cmd_Uniform1iv) + value_size;
   struct marshal_cmd_Uniform1iv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "Uniform1iv");
      CALL_Uniform1iv(ctx->Dispatch.Current, (location, count, value));
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
_mesa_unmarshal_Uniform2iv(struct gl_context *ctx, const struct marshal_cmd_Uniform2iv *restrict cmd)
{
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLint *value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLint *) variable_data;
   CALL_Uniform2iv(ctx->Dispatch.Current, (location, count, value));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_Uniform2iv(GLint location, GLsizei count, const GLint *value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 2 * sizeof(GLint));
   int cmd_size = sizeof(struct marshal_cmd_Uniform2iv) + value_size;
   struct marshal_cmd_Uniform2iv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "Uniform2iv");
      CALL_Uniform2iv(ctx->Dispatch.Current, (location, count, value));
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
_mesa_unmarshal_Uniform3iv(struct gl_context *ctx, const struct marshal_cmd_Uniform3iv *restrict cmd)
{
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLint *value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLint *) variable_data;
   CALL_Uniform3iv(ctx->Dispatch.Current, (location, count, value));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_Uniform3iv(GLint location, GLsizei count, const GLint *value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 3 * sizeof(GLint));
   int cmd_size = sizeof(struct marshal_cmd_Uniform3iv) + value_size;
   struct marshal_cmd_Uniform3iv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "Uniform3iv");
      CALL_Uniform3iv(ctx->Dispatch.Current, (location, count, value));
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
_mesa_unmarshal_Uniform4iv(struct gl_context *ctx, const struct marshal_cmd_Uniform4iv *restrict cmd)
{
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLint *value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLint *) variable_data;
   CALL_Uniform4iv(ctx->Dispatch.Current, (location, count, value));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_Uniform4iv(GLint location, GLsizei count, const GLint *value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 4 * sizeof(GLint));
   int cmd_size = sizeof(struct marshal_cmd_Uniform4iv) + value_size;
   struct marshal_cmd_Uniform4iv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "Uniform4iv");
      CALL_Uniform4iv(ctx->Dispatch.Current, (location, count, value));
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
_mesa_unmarshal_UniformMatrix2fv(struct gl_context *ctx, const struct marshal_cmd_UniformMatrix2fv *restrict cmd)
{
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLboolean transpose = cmd->transpose;
   GLfloat *value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLfloat *) variable_data;
   CALL_UniformMatrix2fv(ctx->Dispatch.Current, (location, count, transpose, value));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_UniformMatrix2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 4 * sizeof(GLfloat));
   int cmd_size = sizeof(struct marshal_cmd_UniformMatrix2fv) + value_size;
   struct marshal_cmd_UniformMatrix2fv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "UniformMatrix2fv");
      CALL_UniformMatrix2fv(ctx->Dispatch.Current, (location, count, transpose, value));
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
_mesa_unmarshal_UniformMatrix3fv(struct gl_context *ctx, const struct marshal_cmd_UniformMatrix3fv *restrict cmd)
{
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLboolean transpose = cmd->transpose;
   GLfloat *value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLfloat *) variable_data;
   CALL_UniformMatrix3fv(ctx->Dispatch.Current, (location, count, transpose, value));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_UniformMatrix3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 9 * sizeof(GLfloat));
   int cmd_size = sizeof(struct marshal_cmd_UniformMatrix3fv) + value_size;
   struct marshal_cmd_UniformMatrix3fv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "UniformMatrix3fv");
      CALL_UniformMatrix3fv(ctx->Dispatch.Current, (location, count, transpose, value));
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
_mesa_unmarshal_UniformMatrix4fv(struct gl_context *ctx, const struct marshal_cmd_UniformMatrix4fv *restrict cmd)
{
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLboolean transpose = cmd->transpose;
   GLfloat *value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLfloat *) variable_data;
   CALL_UniformMatrix4fv(ctx->Dispatch.Current, (location, count, transpose, value));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_UniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 16 * sizeof(GLfloat));
   int cmd_size = sizeof(struct marshal_cmd_UniformMatrix4fv) + value_size;
   struct marshal_cmd_UniformMatrix4fv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "UniformMatrix4fv");
      CALL_UniformMatrix4fv(ctx->Dispatch.Current, (location, count, transpose, value));
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
_mesa_unmarshal_ValidateProgram(struct gl_context *ctx, const struct marshal_cmd_ValidateProgram *restrict cmd)
{
   GLuint program = cmd->program;
   CALL_ValidateProgram(ctx->Dispatch.Current, (program));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_ValidateProgram), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
_mesa_unmarshal_VertexAttrib1d(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib1d *restrict cmd)
{
   GLuint index = cmd->index;
   GLdouble x = cmd->x;
   CALL_VertexAttrib1d(ctx->Dispatch.Current, (index, x));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexAttrib1d), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
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
_mesa_unmarshal_VertexAttrib1dv(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib1dv *restrict cmd)
{
   GLuint index = cmd->index;
   const GLdouble *v = cmd->v;
   CALL_VertexAttrib1dv(ctx->Dispatch.Current, (index, v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexAttrib1dv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_VertexAttrib1dv(GLuint index, const GLdouble *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttrib1dv);
   struct marshal_cmd_VertexAttrib1dv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttrib1dv, cmd_size);
   cmd->index = index;
   memcpy(cmd->v, v, 1 * sizeof(GLdouble));
}


/* VertexAttrib1fARB: marshalled asynchronously */
struct marshal_cmd_VertexAttrib1fARB
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLfloat x;
};
uint32_t
_mesa_unmarshal_VertexAttrib1fARB(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib1fARB *restrict cmd)
{
   GLuint index = cmd->index;
   GLfloat x = cmd->x;
   CALL_VertexAttrib1fARB(ctx->Dispatch.Current, (index, x));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexAttrib1fARB), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_VertexAttrib1fARB(GLuint index, GLfloat x)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttrib1fARB);
   struct marshal_cmd_VertexAttrib1fARB *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttrib1fARB, cmd_size);
   cmd->index = index;
   cmd->x = x;
}


/* VertexAttrib1fvARB: marshalled asynchronously */
struct marshal_cmd_VertexAttrib1fvARB
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLfloat v[1];
};
uint32_t
_mesa_unmarshal_VertexAttrib1fvARB(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib1fvARB *restrict cmd)
{
   GLuint index = cmd->index;
   const GLfloat *v = cmd->v;
   CALL_VertexAttrib1fvARB(ctx->Dispatch.Current, (index, v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexAttrib1fvARB), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_VertexAttrib1fvARB(GLuint index, const GLfloat *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttrib1fvARB);
   struct marshal_cmd_VertexAttrib1fvARB *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttrib1fvARB, cmd_size);
   cmd->index = index;
   memcpy(cmd->v, v, 1 * sizeof(GLfloat));
}


/* VertexAttrib1s: marshalled asynchronously */
struct marshal_cmd_VertexAttrib1s
{
   struct marshal_cmd_base cmd_base;
   GLshort x;
   GLuint index;
};
uint32_t
_mesa_unmarshal_VertexAttrib1s(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib1s *restrict cmd)
{
   GLuint index = cmd->index;
   GLshort x = cmd->x;
   CALL_VertexAttrib1s(ctx->Dispatch.Current, (index, x));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexAttrib1s), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_VertexAttrib1s(GLuint index, GLshort x)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttrib1s);
   struct marshal_cmd_VertexAttrib1s *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttrib1s, cmd_size);
   cmd->index = index;
   cmd->x = x;
}


/* VertexAttrib1sv: marshalled asynchronously */
struct marshal_cmd_VertexAttrib1sv
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLshort v[1];
};
uint32_t
_mesa_unmarshal_VertexAttrib1sv(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib1sv *restrict cmd)
{
   GLuint index = cmd->index;
   const GLshort *v = cmd->v;
   CALL_VertexAttrib1sv(ctx->Dispatch.Current, (index, v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexAttrib1sv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_VertexAttrib1sv(GLuint index, const GLshort *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttrib1sv);
   struct marshal_cmd_VertexAttrib1sv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttrib1sv, cmd_size);
   cmd->index = index;
   memcpy(cmd->v, v, 1 * sizeof(GLshort));
}


/* VertexAttrib2d: marshalled asynchronously */
struct marshal_cmd_VertexAttrib2d
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLdouble x;
   GLdouble y;
};
uint32_t
_mesa_unmarshal_VertexAttrib2d(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib2d *restrict cmd)
{
   GLuint index = cmd->index;
   GLdouble x = cmd->x;
   GLdouble y = cmd->y;
   CALL_VertexAttrib2d(ctx->Dispatch.Current, (index, x, y));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexAttrib2d), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_VertexAttrib2d(GLuint index, GLdouble x, GLdouble y)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttrib2d);
   struct marshal_cmd_VertexAttrib2d *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttrib2d, cmd_size);
   cmd->index = index;
   cmd->x = x;
   cmd->y = y;
}


/* VertexAttrib2dv: marshalled asynchronously */
struct marshal_cmd_VertexAttrib2dv
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLdouble v[2];
};
uint32_t
_mesa_unmarshal_VertexAttrib2dv(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib2dv *restrict cmd)
{
   GLuint index = cmd->index;
   const GLdouble *v = cmd->v;
   CALL_VertexAttrib2dv(ctx->Dispatch.Current, (index, v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexAttrib2dv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_VertexAttrib2dv(GLuint index, const GLdouble *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttrib2dv);
   struct marshal_cmd_VertexAttrib2dv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttrib2dv, cmd_size);
   cmd->index = index;
   memcpy(cmd->v, v, 2 * sizeof(GLdouble));
}


/* VertexAttrib2fARB: marshalled asynchronously */
struct marshal_cmd_VertexAttrib2fARB
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLfloat x;
   GLfloat y;
};
uint32_t
_mesa_unmarshal_VertexAttrib2fARB(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib2fARB *restrict cmd)
{
   GLuint index = cmd->index;
   GLfloat x = cmd->x;
   GLfloat y = cmd->y;
   CALL_VertexAttrib2fARB(ctx->Dispatch.Current, (index, x, y));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexAttrib2fARB), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_VertexAttrib2fARB(GLuint index, GLfloat x, GLfloat y)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttrib2fARB);
   struct marshal_cmd_VertexAttrib2fARB *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttrib2fARB, cmd_size);
   cmd->index = index;
   cmd->x = x;
   cmd->y = y;
}


/* VertexAttrib2fvARB: marshalled asynchronously */
struct marshal_cmd_VertexAttrib2fvARB
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLfloat v[2];
};
uint32_t
_mesa_unmarshal_VertexAttrib2fvARB(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib2fvARB *restrict cmd)
{
   GLuint index = cmd->index;
   const GLfloat *v = cmd->v;
   CALL_VertexAttrib2fvARB(ctx->Dispatch.Current, (index, v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexAttrib2fvARB), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_VertexAttrib2fvARB(GLuint index, const GLfloat *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttrib2fvARB);
   struct marshal_cmd_VertexAttrib2fvARB *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttrib2fvARB, cmd_size);
   cmd->index = index;
   memcpy(cmd->v, v, 2 * sizeof(GLfloat));
}


/* VertexAttrib2s: marshalled asynchronously */
struct marshal_cmd_VertexAttrib2s
{
   struct marshal_cmd_base cmd_base;
   GLshort x;
   GLshort y;
   GLuint index;
};
uint32_t
_mesa_unmarshal_VertexAttrib2s(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib2s *restrict cmd)
{
   GLuint index = cmd->index;
   GLshort x = cmd->x;
   GLshort y = cmd->y;
   CALL_VertexAttrib2s(ctx->Dispatch.Current, (index, x, y));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexAttrib2s), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_VertexAttrib2s(GLuint index, GLshort x, GLshort y)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttrib2s);
   struct marshal_cmd_VertexAttrib2s *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttrib2s, cmd_size);
   cmd->index = index;
   cmd->x = x;
   cmd->y = y;
}


/* VertexAttrib2sv: marshalled asynchronously */
struct marshal_cmd_VertexAttrib2sv
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLshort v[2];
};
uint32_t
_mesa_unmarshal_VertexAttrib2sv(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib2sv *restrict cmd)
{
   GLuint index = cmd->index;
   const GLshort *v = cmd->v;
   CALL_VertexAttrib2sv(ctx->Dispatch.Current, (index, v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexAttrib2sv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_VertexAttrib2sv(GLuint index, const GLshort *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttrib2sv);
   struct marshal_cmd_VertexAttrib2sv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttrib2sv, cmd_size);
   cmd->index = index;
   memcpy(cmd->v, v, 2 * sizeof(GLshort));
}


/* VertexAttrib3d: marshalled asynchronously */
struct marshal_cmd_VertexAttrib3d
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLdouble x;
   GLdouble y;
   GLdouble z;
};
uint32_t
_mesa_unmarshal_VertexAttrib3d(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib3d *restrict cmd)
{
   GLuint index = cmd->index;
   GLdouble x = cmd->x;
   GLdouble y = cmd->y;
   GLdouble z = cmd->z;
   CALL_VertexAttrib3d(ctx->Dispatch.Current, (index, x, y, z));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexAttrib3d), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_VertexAttrib3d(GLuint index, GLdouble x, GLdouble y, GLdouble z)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttrib3d);
   struct marshal_cmd_VertexAttrib3d *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttrib3d, cmd_size);
   cmd->index = index;
   cmd->x = x;
   cmd->y = y;
   cmd->z = z;
}


/* VertexAttrib3dv: marshalled asynchronously */
struct marshal_cmd_VertexAttrib3dv
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLdouble v[3];
};
uint32_t
_mesa_unmarshal_VertexAttrib3dv(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib3dv *restrict cmd)
{
   GLuint index = cmd->index;
   const GLdouble *v = cmd->v;
   CALL_VertexAttrib3dv(ctx->Dispatch.Current, (index, v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexAttrib3dv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_VertexAttrib3dv(GLuint index, const GLdouble *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttrib3dv);
   struct marshal_cmd_VertexAttrib3dv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttrib3dv, cmd_size);
   cmd->index = index;
   memcpy(cmd->v, v, 3 * sizeof(GLdouble));
}


/* VertexAttrib3fARB: marshalled asynchronously */
struct marshal_cmd_VertexAttrib3fARB
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLfloat x;
   GLfloat y;
   GLfloat z;
};
uint32_t
_mesa_unmarshal_VertexAttrib3fARB(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib3fARB *restrict cmd)
{
   GLuint index = cmd->index;
   GLfloat x = cmd->x;
   GLfloat y = cmd->y;
   GLfloat z = cmd->z;
   CALL_VertexAttrib3fARB(ctx->Dispatch.Current, (index, x, y, z));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexAttrib3fARB), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_VertexAttrib3fARB(GLuint index, GLfloat x, GLfloat y, GLfloat z)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttrib3fARB);
   struct marshal_cmd_VertexAttrib3fARB *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttrib3fARB, cmd_size);
   cmd->index = index;
   cmd->x = x;
   cmd->y = y;
   cmd->z = z;
}


/* VertexAttrib3fvARB: marshalled asynchronously */
struct marshal_cmd_VertexAttrib3fvARB
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLfloat v[3];
};
uint32_t
_mesa_unmarshal_VertexAttrib3fvARB(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib3fvARB *restrict cmd)
{
   GLuint index = cmd->index;
   const GLfloat *v = cmd->v;
   CALL_VertexAttrib3fvARB(ctx->Dispatch.Current, (index, v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexAttrib3fvARB), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_VertexAttrib3fvARB(GLuint index, const GLfloat *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttrib3fvARB);
   struct marshal_cmd_VertexAttrib3fvARB *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttrib3fvARB, cmd_size);
   cmd->index = index;
   memcpy(cmd->v, v, 3 * sizeof(GLfloat));
}


/* VertexAttrib3s: marshalled asynchronously */
struct marshal_cmd_VertexAttrib3s
{
   struct marshal_cmd_base cmd_base;
   GLshort x;
   GLshort y;
   GLshort z;
   GLuint index;
};
uint32_t
_mesa_unmarshal_VertexAttrib3s(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib3s *restrict cmd)
{
   GLuint index = cmd->index;
   GLshort x = cmd->x;
   GLshort y = cmd->y;
   GLshort z = cmd->z;
   CALL_VertexAttrib3s(ctx->Dispatch.Current, (index, x, y, z));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexAttrib3s), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_VertexAttrib3s(GLuint index, GLshort x, GLshort y, GLshort z)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttrib3s);
   struct marshal_cmd_VertexAttrib3s *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttrib3s, cmd_size);
   cmd->index = index;
   cmd->x = x;
   cmd->y = y;
   cmd->z = z;
}


/* VertexAttrib3sv: marshalled asynchronously */
struct marshal_cmd_VertexAttrib3sv
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLshort v[3];
};
uint32_t
_mesa_unmarshal_VertexAttrib3sv(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib3sv *restrict cmd)
{
   GLuint index = cmd->index;
   const GLshort *v = cmd->v;
   CALL_VertexAttrib3sv(ctx->Dispatch.Current, (index, v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexAttrib3sv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_VertexAttrib3sv(GLuint index, const GLshort *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttrib3sv);
   struct marshal_cmd_VertexAttrib3sv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttrib3sv, cmd_size);
   cmd->index = index;
   memcpy(cmd->v, v, 3 * sizeof(GLshort));
}


/* VertexAttrib4Nbv: marshalled asynchronously */
struct marshal_cmd_VertexAttrib4Nbv
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLbyte v[4];
};
uint32_t
_mesa_unmarshal_VertexAttrib4Nbv(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4Nbv *restrict cmd)
{
   GLuint index = cmd->index;
   const GLbyte *v = cmd->v;
   CALL_VertexAttrib4Nbv(ctx->Dispatch.Current, (index, v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexAttrib4Nbv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_VertexAttrib4Nbv(GLuint index, const GLbyte *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttrib4Nbv);
   struct marshal_cmd_VertexAttrib4Nbv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttrib4Nbv, cmd_size);
   cmd->index = index;
   memcpy(cmd->v, v, 4 * sizeof(GLbyte));
}


/* VertexAttrib4Niv: marshalled asynchronously */
struct marshal_cmd_VertexAttrib4Niv
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLint v[4];
};
uint32_t
_mesa_unmarshal_VertexAttrib4Niv(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4Niv *restrict cmd)
{
   GLuint index = cmd->index;
   const GLint *v = cmd->v;
   CALL_VertexAttrib4Niv(ctx->Dispatch.Current, (index, v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexAttrib4Niv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_VertexAttrib4Niv(GLuint index, const GLint *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttrib4Niv);
   struct marshal_cmd_VertexAttrib4Niv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttrib4Niv, cmd_size);
   cmd->index = index;
   memcpy(cmd->v, v, 4 * sizeof(GLint));
}


/* VertexAttrib4Nsv: marshalled asynchronously */
struct marshal_cmd_VertexAttrib4Nsv
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLshort v[4];
};
uint32_t
_mesa_unmarshal_VertexAttrib4Nsv(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4Nsv *restrict cmd)
{
   GLuint index = cmd->index;
   const GLshort *v = cmd->v;
   CALL_VertexAttrib4Nsv(ctx->Dispatch.Current, (index, v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexAttrib4Nsv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_VertexAttrib4Nsv(GLuint index, const GLshort *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttrib4Nsv);
   struct marshal_cmd_VertexAttrib4Nsv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttrib4Nsv, cmd_size);
   cmd->index = index;
   memcpy(cmd->v, v, 4 * sizeof(GLshort));
}


/* VertexAttrib4Nub: marshalled asynchronously */
struct marshal_cmd_VertexAttrib4Nub
{
   struct marshal_cmd_base cmd_base;
   GLubyte x;
   GLubyte y;
   GLubyte z;
   GLubyte w;
   GLuint index;
};
uint32_t
_mesa_unmarshal_VertexAttrib4Nub(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4Nub *restrict cmd)
{
   GLuint index = cmd->index;
   GLubyte x = cmd->x;
   GLubyte y = cmd->y;
   GLubyte z = cmd->z;
   GLubyte w = cmd->w;
   CALL_VertexAttrib4Nub(ctx->Dispatch.Current, (index, x, y, z, w));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexAttrib4Nub), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_VertexAttrib4Nub(GLuint index, GLubyte x, GLubyte y, GLubyte z, GLubyte w)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttrib4Nub);
   struct marshal_cmd_VertexAttrib4Nub *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttrib4Nub, cmd_size);
   cmd->index = index;
   cmd->x = x;
   cmd->y = y;
   cmd->z = z;
   cmd->w = w;
}


/* VertexAttrib4Nubv: marshalled asynchronously */
struct marshal_cmd_VertexAttrib4Nubv
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLubyte v[4];
};
uint32_t
_mesa_unmarshal_VertexAttrib4Nubv(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4Nubv *restrict cmd)
{
   GLuint index = cmd->index;
   const GLubyte *v = cmd->v;
   CALL_VertexAttrib4Nubv(ctx->Dispatch.Current, (index, v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexAttrib4Nubv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_VertexAttrib4Nubv(GLuint index, const GLubyte *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttrib4Nubv);
   struct marshal_cmd_VertexAttrib4Nubv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttrib4Nubv, cmd_size);
   cmd->index = index;
   memcpy(cmd->v, v, 4 * sizeof(GLubyte));
}


/* VertexAttrib4Nuiv: marshalled asynchronously */
struct marshal_cmd_VertexAttrib4Nuiv
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLuint v[4];
};
uint32_t
_mesa_unmarshal_VertexAttrib4Nuiv(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4Nuiv *restrict cmd)
{
   GLuint index = cmd->index;
   const GLuint *v = cmd->v;
   CALL_VertexAttrib4Nuiv(ctx->Dispatch.Current, (index, v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexAttrib4Nuiv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_VertexAttrib4Nuiv(GLuint index, const GLuint *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttrib4Nuiv);
   struct marshal_cmd_VertexAttrib4Nuiv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttrib4Nuiv, cmd_size);
   cmd->index = index;
   memcpy(cmd->v, v, 4 * sizeof(GLuint));
}


/* VertexAttrib4Nusv: marshalled asynchronously */
struct marshal_cmd_VertexAttrib4Nusv
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLushort v[4];
};
uint32_t
_mesa_unmarshal_VertexAttrib4Nusv(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4Nusv *restrict cmd)
{
   GLuint index = cmd->index;
   const GLushort *v = cmd->v;
   CALL_VertexAttrib4Nusv(ctx->Dispatch.Current, (index, v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexAttrib4Nusv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_VertexAttrib4Nusv(GLuint index, const GLushort *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttrib4Nusv);
   struct marshal_cmd_VertexAttrib4Nusv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttrib4Nusv, cmd_size);
   cmd->index = index;
   memcpy(cmd->v, v, 4 * sizeof(GLushort));
}


/* VertexAttrib4bv: marshalled asynchronously */
struct marshal_cmd_VertexAttrib4bv
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLbyte v[4];
};
uint32_t
_mesa_unmarshal_VertexAttrib4bv(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4bv *restrict cmd)
{
   GLuint index = cmd->index;
   const GLbyte *v = cmd->v;
   CALL_VertexAttrib4bv(ctx->Dispatch.Current, (index, v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexAttrib4bv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_VertexAttrib4bv(GLuint index, const GLbyte *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttrib4bv);
   struct marshal_cmd_VertexAttrib4bv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttrib4bv, cmd_size);
   cmd->index = index;
   memcpy(cmd->v, v, 4 * sizeof(GLbyte));
}


/* VertexAttrib4d: marshalled asynchronously */
struct marshal_cmd_VertexAttrib4d
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLdouble x;
   GLdouble y;
   GLdouble z;
   GLdouble w;
};
uint32_t
_mesa_unmarshal_VertexAttrib4d(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4d *restrict cmd)
{
   GLuint index = cmd->index;
   GLdouble x = cmd->x;
   GLdouble y = cmd->y;
   GLdouble z = cmd->z;
   GLdouble w = cmd->w;
   CALL_VertexAttrib4d(ctx->Dispatch.Current, (index, x, y, z, w));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexAttrib4d), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_VertexAttrib4d(GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttrib4d);
   struct marshal_cmd_VertexAttrib4d *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttrib4d, cmd_size);
   cmd->index = index;
   cmd->x = x;
   cmd->y = y;
   cmd->z = z;
   cmd->w = w;
}


/* VertexAttrib4dv: marshalled asynchronously */
struct marshal_cmd_VertexAttrib4dv
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLdouble v[4];
};
uint32_t
_mesa_unmarshal_VertexAttrib4dv(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4dv *restrict cmd)
{
   GLuint index = cmd->index;
   const GLdouble *v = cmd->v;
   CALL_VertexAttrib4dv(ctx->Dispatch.Current, (index, v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexAttrib4dv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_VertexAttrib4dv(GLuint index, const GLdouble *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttrib4dv);
   struct marshal_cmd_VertexAttrib4dv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttrib4dv, cmd_size);
   cmd->index = index;
   memcpy(cmd->v, v, 4 * sizeof(GLdouble));
}


/* VertexAttrib4fARB: marshalled asynchronously */
struct marshal_cmd_VertexAttrib4fARB
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLfloat x;
   GLfloat y;
   GLfloat z;
   GLfloat w;
};
uint32_t
_mesa_unmarshal_VertexAttrib4fARB(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4fARB *restrict cmd)
{
   GLuint index = cmd->index;
   GLfloat x = cmd->x;
   GLfloat y = cmd->y;
   GLfloat z = cmd->z;
   GLfloat w = cmd->w;
   CALL_VertexAttrib4fARB(ctx->Dispatch.Current, (index, x, y, z, w));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexAttrib4fARB), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_VertexAttrib4fARB(GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttrib4fARB);
   struct marshal_cmd_VertexAttrib4fARB *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttrib4fARB, cmd_size);
   cmd->index = index;
   cmd->x = x;
   cmd->y = y;
   cmd->z = z;
   cmd->w = w;
}


/* VertexAttrib4fvARB: marshalled asynchronously */
struct marshal_cmd_VertexAttrib4fvARB
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLfloat v[4];
};
uint32_t
_mesa_unmarshal_VertexAttrib4fvARB(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4fvARB *restrict cmd)
{
   GLuint index = cmd->index;
   const GLfloat *v = cmd->v;
   CALL_VertexAttrib4fvARB(ctx->Dispatch.Current, (index, v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexAttrib4fvARB), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_VertexAttrib4fvARB(GLuint index, const GLfloat *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttrib4fvARB);
   struct marshal_cmd_VertexAttrib4fvARB *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttrib4fvARB, cmd_size);
   cmd->index = index;
   memcpy(cmd->v, v, 4 * sizeof(GLfloat));
}


/* VertexAttrib4iv: marshalled asynchronously */
struct marshal_cmd_VertexAttrib4iv
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLint v[4];
};
uint32_t
_mesa_unmarshal_VertexAttrib4iv(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4iv *restrict cmd)
{
   GLuint index = cmd->index;
   const GLint *v = cmd->v;
   CALL_VertexAttrib4iv(ctx->Dispatch.Current, (index, v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexAttrib4iv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_VertexAttrib4iv(GLuint index, const GLint *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttrib4iv);
   struct marshal_cmd_VertexAttrib4iv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttrib4iv, cmd_size);
   cmd->index = index;
   memcpy(cmd->v, v, 4 * sizeof(GLint));
}


/* VertexAttrib4s: marshalled asynchronously */
struct marshal_cmd_VertexAttrib4s
{
   struct marshal_cmd_base cmd_base;
   GLshort x;
   GLshort y;
   GLshort z;
   GLshort w;
   GLuint index;
};
uint32_t
_mesa_unmarshal_VertexAttrib4s(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4s *restrict cmd)
{
   GLuint index = cmd->index;
   GLshort x = cmd->x;
   GLshort y = cmd->y;
   GLshort z = cmd->z;
   GLshort w = cmd->w;
   CALL_VertexAttrib4s(ctx->Dispatch.Current, (index, x, y, z, w));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexAttrib4s), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_VertexAttrib4s(GLuint index, GLshort x, GLshort y, GLshort z, GLshort w)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttrib4s);
   struct marshal_cmd_VertexAttrib4s *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttrib4s, cmd_size);
   cmd->index = index;
   cmd->x = x;
   cmd->y = y;
   cmd->z = z;
   cmd->w = w;
}


/* VertexAttrib4sv: marshalled asynchronously */
struct marshal_cmd_VertexAttrib4sv
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLshort v[4];
};
uint32_t
_mesa_unmarshal_VertexAttrib4sv(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4sv *restrict cmd)
{
   GLuint index = cmd->index;
   const GLshort *v = cmd->v;
   CALL_VertexAttrib4sv(ctx->Dispatch.Current, (index, v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexAttrib4sv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_VertexAttrib4sv(GLuint index, const GLshort *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttrib4sv);
   struct marshal_cmd_VertexAttrib4sv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttrib4sv, cmd_size);
   cmd->index = index;
   memcpy(cmd->v, v, 4 * sizeof(GLshort));
}


/* VertexAttrib4ubv: marshalled asynchronously */
struct marshal_cmd_VertexAttrib4ubv
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLubyte v[4];
};
uint32_t
_mesa_unmarshal_VertexAttrib4ubv(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4ubv *restrict cmd)
{
   GLuint index = cmd->index;
   const GLubyte *v = cmd->v;
   CALL_VertexAttrib4ubv(ctx->Dispatch.Current, (index, v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexAttrib4ubv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_VertexAttrib4ubv(GLuint index, const GLubyte *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttrib4ubv);
   struct marshal_cmd_VertexAttrib4ubv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttrib4ubv, cmd_size);
   cmd->index = index;
   memcpy(cmd->v, v, 4 * sizeof(GLubyte));
}


/* VertexAttrib4uiv: marshalled asynchronously */
struct marshal_cmd_VertexAttrib4uiv
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLuint v[4];
};
uint32_t
_mesa_unmarshal_VertexAttrib4uiv(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4uiv *restrict cmd)
{
   GLuint index = cmd->index;
   const GLuint *v = cmd->v;
   CALL_VertexAttrib4uiv(ctx->Dispatch.Current, (index, v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexAttrib4uiv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_VertexAttrib4uiv(GLuint index, const GLuint *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttrib4uiv);
   struct marshal_cmd_VertexAttrib4uiv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttrib4uiv, cmd_size);
   cmd->index = index;
   memcpy(cmd->v, v, 4 * sizeof(GLuint));
}


/* VertexAttrib4usv: marshalled asynchronously */
struct marshal_cmd_VertexAttrib4usv
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLushort v[4];
};
uint32_t
_mesa_unmarshal_VertexAttrib4usv(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4usv *restrict cmd)
{
   GLuint index = cmd->index;
   const GLushort *v = cmd->v;
   CALL_VertexAttrib4usv(ctx->Dispatch.Current, (index, v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexAttrib4usv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_VertexAttrib4usv(GLuint index, const GLushort *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttrib4usv);
   struct marshal_cmd_VertexAttrib4usv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttrib4usv, cmd_size);
   cmd->index = index;
   memcpy(cmd->v, v, 4 * sizeof(GLushort));
}


/* VertexAttribPointer: marshalled asynchronously */
struct marshal_cmd_VertexAttribPointer
{
   struct marshal_cmd_base cmd_base;
   GLboolean normalized;
   GLenum16 type;
   GLuint index;
   GLint size;
   GLsizei stride;
   const GLvoid * pointer;
};
uint32_t
_mesa_unmarshal_VertexAttribPointer(struct gl_context *ctx, const struct marshal_cmd_VertexAttribPointer *restrict cmd)
{
   GLuint index = cmd->index;
   GLint size = cmd->size;
   GLenum type = cmd->type;
   GLboolean normalized = cmd->normalized;
   GLsizei stride = cmd->stride;
   const GLvoid * pointer = cmd->pointer;
   CALL_VertexAttribPointer(ctx->Dispatch.Current, (index, size, type, normalized, stride, pointer));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexAttribPointer), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_VertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttribPointer);
   struct marshal_cmd_VertexAttribPointer *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttribPointer, cmd_size);
   cmd->index = index;
   cmd->size = size;
   cmd->type = MIN2(type, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->normalized = normalized;
   cmd->stride = stride;
   cmd->pointer = pointer;
   if (COMPAT) _mesa_glthread_AttribPointer(ctx, VERT_ATTRIB_GENERIC(index), MESA_PACK_VFORMAT(type, size, normalized, 0, 0), stride, pointer);
}


/* UniformMatrix2x3fv: marshalled asynchronously */
struct marshal_cmd_UniformMatrix2x3fv
{
   struct marshal_cmd_base cmd_base;
   GLboolean transpose;
   GLint location;
   GLsizei count;
   /* Next safe_mul(count, 6 * sizeof(GLfloat)) bytes are GLfloat value[count][6] */
};
uint32_t
_mesa_unmarshal_UniformMatrix2x3fv(struct gl_context *ctx, const struct marshal_cmd_UniformMatrix2x3fv *restrict cmd)
{
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLboolean transpose = cmd->transpose;
   GLfloat *value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLfloat *) variable_data;
   CALL_UniformMatrix2x3fv(ctx->Dispatch.Current, (location, count, transpose, value));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_UniformMatrix2x3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 6 * sizeof(GLfloat));
   int cmd_size = sizeof(struct marshal_cmd_UniformMatrix2x3fv) + value_size;
   struct marshal_cmd_UniformMatrix2x3fv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "UniformMatrix2x3fv");
      CALL_UniformMatrix2x3fv(ctx->Dispatch.Current, (location, count, transpose, value));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_UniformMatrix2x3fv, cmd_size);
   cmd->location = location;
   cmd->count = count;
   cmd->transpose = transpose;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, value, value_size);
}


/* UniformMatrix3x2fv: marshalled asynchronously */
struct marshal_cmd_UniformMatrix3x2fv
{
   struct marshal_cmd_base cmd_base;
   GLboolean transpose;
   GLint location;
   GLsizei count;
   /* Next safe_mul(count, 6 * sizeof(GLfloat)) bytes are GLfloat value[count][6] */
};
uint32_t
_mesa_unmarshal_UniformMatrix3x2fv(struct gl_context *ctx, const struct marshal_cmd_UniformMatrix3x2fv *restrict cmd)
{
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLboolean transpose = cmd->transpose;
   GLfloat *value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLfloat *) variable_data;
   CALL_UniformMatrix3x2fv(ctx->Dispatch.Current, (location, count, transpose, value));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_UniformMatrix3x2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 6 * sizeof(GLfloat));
   int cmd_size = sizeof(struct marshal_cmd_UniformMatrix3x2fv) + value_size;
   struct marshal_cmd_UniformMatrix3x2fv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "UniformMatrix3x2fv");
      CALL_UniformMatrix3x2fv(ctx->Dispatch.Current, (location, count, transpose, value));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_UniformMatrix3x2fv, cmd_size);
   cmd->location = location;
   cmd->count = count;
   cmd->transpose = transpose;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, value, value_size);
}


/* UniformMatrix2x4fv: marshalled asynchronously */
struct marshal_cmd_UniformMatrix2x4fv
{
   struct marshal_cmd_base cmd_base;
   GLboolean transpose;
   GLint location;
   GLsizei count;
   /* Next safe_mul(count, 8 * sizeof(GLfloat)) bytes are GLfloat value[count][8] */
};
uint32_t
_mesa_unmarshal_UniformMatrix2x4fv(struct gl_context *ctx, const struct marshal_cmd_UniformMatrix2x4fv *restrict cmd)
{
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLboolean transpose = cmd->transpose;
   GLfloat *value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLfloat *) variable_data;
   CALL_UniformMatrix2x4fv(ctx->Dispatch.Current, (location, count, transpose, value));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_UniformMatrix2x4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 8 * sizeof(GLfloat));
   int cmd_size = sizeof(struct marshal_cmd_UniformMatrix2x4fv) + value_size;
   struct marshal_cmd_UniformMatrix2x4fv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "UniformMatrix2x4fv");
      CALL_UniformMatrix2x4fv(ctx->Dispatch.Current, (location, count, transpose, value));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_UniformMatrix2x4fv, cmd_size);
   cmd->location = location;
   cmd->count = count;
   cmd->transpose = transpose;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, value, value_size);
}


/* UniformMatrix4x2fv: marshalled asynchronously */
struct marshal_cmd_UniformMatrix4x2fv
{
   struct marshal_cmd_base cmd_base;
   GLboolean transpose;
   GLint location;
   GLsizei count;
   /* Next safe_mul(count, 8 * sizeof(GLfloat)) bytes are GLfloat value[count][8] */
};
uint32_t
_mesa_unmarshal_UniformMatrix4x2fv(struct gl_context *ctx, const struct marshal_cmd_UniformMatrix4x2fv *restrict cmd)
{
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLboolean transpose = cmd->transpose;
   GLfloat *value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLfloat *) variable_data;
   CALL_UniformMatrix4x2fv(ctx->Dispatch.Current, (location, count, transpose, value));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_UniformMatrix4x2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 8 * sizeof(GLfloat));
   int cmd_size = sizeof(struct marshal_cmd_UniformMatrix4x2fv) + value_size;
   struct marshal_cmd_UniformMatrix4x2fv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "UniformMatrix4x2fv");
      CALL_UniformMatrix4x2fv(ctx->Dispatch.Current, (location, count, transpose, value));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_UniformMatrix4x2fv, cmd_size);
   cmd->location = location;
   cmd->count = count;
   cmd->transpose = transpose;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, value, value_size);
}


/* UniformMatrix3x4fv: marshalled asynchronously */
struct marshal_cmd_UniformMatrix3x4fv
{
   struct marshal_cmd_base cmd_base;
   GLboolean transpose;
   GLint location;
   GLsizei count;
   /* Next safe_mul(count, 12 * sizeof(GLfloat)) bytes are GLfloat value[count][12] */
};
uint32_t
_mesa_unmarshal_UniformMatrix3x4fv(struct gl_context *ctx, const struct marshal_cmd_UniformMatrix3x4fv *restrict cmd)
{
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLboolean transpose = cmd->transpose;
   GLfloat *value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLfloat *) variable_data;
   CALL_UniformMatrix3x4fv(ctx->Dispatch.Current, (location, count, transpose, value));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_UniformMatrix3x4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 12 * sizeof(GLfloat));
   int cmd_size = sizeof(struct marshal_cmd_UniformMatrix3x4fv) + value_size;
   struct marshal_cmd_UniformMatrix3x4fv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "UniformMatrix3x4fv");
      CALL_UniformMatrix3x4fv(ctx->Dispatch.Current, (location, count, transpose, value));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_UniformMatrix3x4fv, cmd_size);
   cmd->location = location;
   cmd->count = count;
   cmd->transpose = transpose;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, value, value_size);
}


/* UniformMatrix4x3fv: marshalled asynchronously */
struct marshal_cmd_UniformMatrix4x3fv
{
   struct marshal_cmd_base cmd_base;
   GLboolean transpose;
   GLint location;
   GLsizei count;
   /* Next safe_mul(count, 12 * sizeof(GLfloat)) bytes are GLfloat value[count][12] */
};
uint32_t
_mesa_unmarshal_UniformMatrix4x3fv(struct gl_context *ctx, const struct marshal_cmd_UniformMatrix4x3fv *restrict cmd)
{
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLboolean transpose = cmd->transpose;
   GLfloat *value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLfloat *) variable_data;
   CALL_UniformMatrix4x3fv(ctx->Dispatch.Current, (location, count, transpose, value));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_UniformMatrix4x3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 12 * sizeof(GLfloat));
   int cmd_size = sizeof(struct marshal_cmd_UniformMatrix4x3fv) + value_size;
   struct marshal_cmd_UniformMatrix4x3fv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "UniformMatrix4x3fv");
      CALL_UniformMatrix4x3fv(ctx->Dispatch.Current, (location, count, transpose, value));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_UniformMatrix4x3fv, cmd_size);
   cmd->location = location;
   cmd->count = count;
   cmd->transpose = transpose;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, value, value_size);
}


/* ProgramStringARB: marshalled asynchronously */
struct marshal_cmd_ProgramStringARB
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLenum16 format;
   GLsizei len;
   /* Next len bytes are GLvoid string[len] */
};
uint32_t
_mesa_unmarshal_ProgramStringARB(struct gl_context *ctx, const struct marshal_cmd_ProgramStringARB *restrict cmd)
{
   GLenum target = cmd->target;
   GLenum format = cmd->format;
   GLsizei len = cmd->len;
   GLvoid *string;
   const char *variable_data = (const char *) (cmd + 1);
   string = (GLvoid *) variable_data;
   CALL_ProgramStringARB(ctx->Dispatch.Current, (target, format, len, string));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ProgramStringARB(GLenum target, GLenum format, GLsizei len, const GLvoid *string)
{
   GET_CURRENT_CONTEXT(ctx);
   int string_size = len;
   int cmd_size = sizeof(struct marshal_cmd_ProgramStringARB) + string_size;
   struct marshal_cmd_ProgramStringARB *cmd;
   if (unlikely(string_size < 0 || (string_size > 0 && !string) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "ProgramStringARB");
      CALL_ProgramStringARB(ctx->Dispatch.Current, (target, format, len, string));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ProgramStringARB, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->format = MIN2(format, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->len = len;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, string, string_size);
}


/* BindProgramARB: marshalled asynchronously */
struct marshal_cmd_BindProgramARB
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLuint program;
};
uint32_t
_mesa_unmarshal_BindProgramARB(struct gl_context *ctx, const struct marshal_cmd_BindProgramARB *restrict cmd)
{
   GLenum target = cmd->target;
   GLuint program = cmd->program;
   CALL_BindProgramARB(ctx->Dispatch.Current, (target, program));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_BindProgramARB), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_BindProgramARB(GLenum target, GLuint program)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_BindProgramARB);
   struct marshal_cmd_BindProgramARB *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_BindProgramARB, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->program = program;
}


/* DeleteProgramsARB: marshalled asynchronously */
struct marshal_cmd_DeleteProgramsARB
{
   struct marshal_cmd_base cmd_base;
   GLsizei n;
   /* Next safe_mul(n, 1 * sizeof(GLuint)) bytes are GLuint programs[n] */
};
uint32_t
_mesa_unmarshal_DeleteProgramsARB(struct gl_context *ctx, const struct marshal_cmd_DeleteProgramsARB *restrict cmd)
{
   GLsizei n = cmd->n;
   GLuint *programs;
   const char *variable_data = (const char *) (cmd + 1);
   programs = (GLuint *) variable_data;
   CALL_DeleteProgramsARB(ctx->Dispatch.Current, (n, programs));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_DeleteProgramsARB(GLsizei n, const GLuint *programs)
{
   GET_CURRENT_CONTEXT(ctx);
   int programs_size = safe_mul(n, 1 * sizeof(GLuint));
   int cmd_size = sizeof(struct marshal_cmd_DeleteProgramsARB) + programs_size;
   struct marshal_cmd_DeleteProgramsARB *cmd;
   if (unlikely(programs_size < 0 || (programs_size > 0 && !programs) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "DeleteProgramsARB");
      CALL_DeleteProgramsARB(ctx->Dispatch.Current, (n, programs));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_DeleteProgramsARB, cmd_size);
   cmd->n = n;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, programs, programs_size);
}


/* GenProgramsARB: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GenProgramsARB(GLsizei n, GLuint *programs)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GenProgramsARB");
   CALL_GenProgramsARB(ctx->Dispatch.Current, (n, programs));
}


/* IsProgramARB: marshalled synchronously */
static GLboolean GLAPIENTRY
_mesa_marshal_IsProgramARB(GLuint program)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "IsProgramARB");
   return CALL_IsProgramARB(ctx->Dispatch.Current, (program));
}


/* ProgramEnvParameter4dARB: marshalled asynchronously */
struct marshal_cmd_ProgramEnvParameter4dARB
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLuint index;
   GLdouble x;
   GLdouble y;
   GLdouble z;
   GLdouble w;
};
uint32_t
_mesa_unmarshal_ProgramEnvParameter4dARB(struct gl_context *ctx, const struct marshal_cmd_ProgramEnvParameter4dARB *restrict cmd)
{
   GLenum target = cmd->target;
   GLuint index = cmd->index;
   GLdouble x = cmd->x;
   GLdouble y = cmd->y;
   GLdouble z = cmd->z;
   GLdouble w = cmd->w;
   CALL_ProgramEnvParameter4dARB(ctx->Dispatch.Current, (target, index, x, y, z, w));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_ProgramEnvParameter4dARB), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ProgramEnvParameter4dARB(GLenum target, GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ProgramEnvParameter4dARB);
   struct marshal_cmd_ProgramEnvParameter4dARB *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ProgramEnvParameter4dARB, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->index = index;
   cmd->x = x;
   cmd->y = y;
   cmd->z = z;
   cmd->w = w;
}


/* ProgramEnvParameter4dvARB: marshalled asynchronously */
struct marshal_cmd_ProgramEnvParameter4dvARB
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLuint index;
   GLdouble params[4];
};
uint32_t
_mesa_unmarshal_ProgramEnvParameter4dvARB(struct gl_context *ctx, const struct marshal_cmd_ProgramEnvParameter4dvARB *restrict cmd)
{
   GLenum target = cmd->target;
   GLuint index = cmd->index;
   const GLdouble *params = cmd->params;
   CALL_ProgramEnvParameter4dvARB(ctx->Dispatch.Current, (target, index, params));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_ProgramEnvParameter4dvARB), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ProgramEnvParameter4dvARB(GLenum target, GLuint index, const GLdouble *params)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ProgramEnvParameter4dvARB);
   struct marshal_cmd_ProgramEnvParameter4dvARB *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ProgramEnvParameter4dvARB, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->index = index;
   memcpy(cmd->params, params, 4 * sizeof(GLdouble));
}


/* ProgramEnvParameter4fARB: marshalled asynchronously */
struct marshal_cmd_ProgramEnvParameter4fARB
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLuint index;
   GLfloat x;
   GLfloat y;
   GLfloat z;
   GLfloat w;
};
uint32_t
_mesa_unmarshal_ProgramEnvParameter4fARB(struct gl_context *ctx, const struct marshal_cmd_ProgramEnvParameter4fARB *restrict cmd)
{
   GLenum target = cmd->target;
   GLuint index = cmd->index;
   GLfloat x = cmd->x;
   GLfloat y = cmd->y;
   GLfloat z = cmd->z;
   GLfloat w = cmd->w;
   CALL_ProgramEnvParameter4fARB(ctx->Dispatch.Current, (target, index, x, y, z, w));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_ProgramEnvParameter4fARB), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ProgramEnvParameter4fARB(GLenum target, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ProgramEnvParameter4fARB);
   struct marshal_cmd_ProgramEnvParameter4fARB *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ProgramEnvParameter4fARB, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->index = index;
   cmd->x = x;
   cmd->y = y;
   cmd->z = z;
   cmd->w = w;
}


/* ProgramEnvParameter4fvARB: marshalled asynchronously */
struct marshal_cmd_ProgramEnvParameter4fvARB
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLuint index;
   GLfloat params[4];
};
uint32_t
_mesa_unmarshal_ProgramEnvParameter4fvARB(struct gl_context *ctx, const struct marshal_cmd_ProgramEnvParameter4fvARB *restrict cmd)
{
   GLenum target = cmd->target;
   GLuint index = cmd->index;
   const GLfloat *params = cmd->params;
   CALL_ProgramEnvParameter4fvARB(ctx->Dispatch.Current, (target, index, params));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_ProgramEnvParameter4fvARB), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ProgramEnvParameter4fvARB(GLenum target, GLuint index, const GLfloat *params)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ProgramEnvParameter4fvARB);
   struct marshal_cmd_ProgramEnvParameter4fvARB *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ProgramEnvParameter4fvARB, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->index = index;
   memcpy(cmd->params, params, 4 * sizeof(GLfloat));
}


/* ProgramLocalParameter4dARB: marshalled asynchronously */
struct marshal_cmd_ProgramLocalParameter4dARB
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLuint index;
   GLdouble x;
   GLdouble y;
   GLdouble z;
   GLdouble w;
};
uint32_t
_mesa_unmarshal_ProgramLocalParameter4dARB(struct gl_context *ctx, const struct marshal_cmd_ProgramLocalParameter4dARB *restrict cmd)
{
   GLenum target = cmd->target;
   GLuint index = cmd->index;
   GLdouble x = cmd->x;
   GLdouble y = cmd->y;
   GLdouble z = cmd->z;
   GLdouble w = cmd->w;
   CALL_ProgramLocalParameter4dARB(ctx->Dispatch.Current, (target, index, x, y, z, w));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_ProgramLocalParameter4dARB), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ProgramLocalParameter4dARB(GLenum target, GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ProgramLocalParameter4dARB);
   struct marshal_cmd_ProgramLocalParameter4dARB *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ProgramLocalParameter4dARB, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->index = index;
   cmd->x = x;
   cmd->y = y;
   cmd->z = z;
   cmd->w = w;
}


/* ProgramLocalParameter4dvARB: marshalled asynchronously */
struct marshal_cmd_ProgramLocalParameter4dvARB
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLuint index;
   GLdouble params[4];
};
uint32_t
_mesa_unmarshal_ProgramLocalParameter4dvARB(struct gl_context *ctx, const struct marshal_cmd_ProgramLocalParameter4dvARB *restrict cmd)
{
   GLenum target = cmd->target;
   GLuint index = cmd->index;
   const GLdouble *params = cmd->params;
   CALL_ProgramLocalParameter4dvARB(ctx->Dispatch.Current, (target, index, params));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_ProgramLocalParameter4dvARB), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ProgramLocalParameter4dvARB(GLenum target, GLuint index, const GLdouble *params)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ProgramLocalParameter4dvARB);
   struct marshal_cmd_ProgramLocalParameter4dvARB *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ProgramLocalParameter4dvARB, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->index = index;
   memcpy(cmd->params, params, 4 * sizeof(GLdouble));
}


/* ProgramLocalParameter4fARB: marshalled asynchronously */
struct marshal_cmd_ProgramLocalParameter4fARB
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLuint index;
   GLfloat x;
   GLfloat y;
   GLfloat z;
   GLfloat w;
};
uint32_t
_mesa_unmarshal_ProgramLocalParameter4fARB(struct gl_context *ctx, const struct marshal_cmd_ProgramLocalParameter4fARB *restrict cmd)
{
   GLenum target = cmd->target;
   GLuint index = cmd->index;
   GLfloat x = cmd->x;
   GLfloat y = cmd->y;
   GLfloat z = cmd->z;
   GLfloat w = cmd->w;
   CALL_ProgramLocalParameter4fARB(ctx->Dispatch.Current, (target, index, x, y, z, w));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_ProgramLocalParameter4fARB), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ProgramLocalParameter4fARB(GLenum target, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ProgramLocalParameter4fARB);
   struct marshal_cmd_ProgramLocalParameter4fARB *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ProgramLocalParameter4fARB, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->index = index;
   cmd->x = x;
   cmd->y = y;
   cmd->z = z;
   cmd->w = w;
}


/* ProgramLocalParameter4fvARB: marshalled asynchronously */
struct marshal_cmd_ProgramLocalParameter4fvARB
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLuint index;
   GLfloat params[4];
};
uint32_t
_mesa_unmarshal_ProgramLocalParameter4fvARB(struct gl_context *ctx, const struct marshal_cmd_ProgramLocalParameter4fvARB *restrict cmd)
{
   GLenum target = cmd->target;
   GLuint index = cmd->index;
   const GLfloat *params = cmd->params;
   CALL_ProgramLocalParameter4fvARB(ctx->Dispatch.Current, (target, index, params));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_ProgramLocalParameter4fvARB), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ProgramLocalParameter4fvARB(GLenum target, GLuint index, const GLfloat *params)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ProgramLocalParameter4fvARB);
   struct marshal_cmd_ProgramLocalParameter4fvARB *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ProgramLocalParameter4fvARB, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->index = index;
   memcpy(cmd->params, params, 4 * sizeof(GLfloat));
}


/* GetProgramEnvParameterdvARB: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetProgramEnvParameterdvARB(GLenum target, GLuint index, GLdouble *params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetProgramEnvParameterdvARB");
   CALL_GetProgramEnvParameterdvARB(ctx->Dispatch.Current, (target, index, params));
}


/* GetProgramEnvParameterfvARB: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetProgramEnvParameterfvARB(GLenum target, GLuint index, GLfloat *params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetProgramEnvParameterfvARB");
   CALL_GetProgramEnvParameterfvARB(ctx->Dispatch.Current, (target, index, params));
}


/* GetProgramLocalParameterdvARB: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetProgramLocalParameterdvARB(GLenum target, GLuint index, GLdouble *params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetProgramLocalParameterdvARB");
   CALL_GetProgramLocalParameterdvARB(ctx->Dispatch.Current, (target, index, params));
}


/* GetProgramLocalParameterfvARB: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetProgramLocalParameterfvARB(GLenum target, GLuint index, GLfloat *params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetProgramLocalParameterfvARB");
   CALL_GetProgramLocalParameterfvARB(ctx->Dispatch.Current, (target, index, params));
}


/* GetProgramivARB: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetProgramivARB(GLenum target, GLenum pname, GLint *params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetProgramivARB");
   CALL_GetProgramivARB(ctx->Dispatch.Current, (target, pname, params));
}


/* GetProgramStringARB: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetProgramStringARB(GLenum target, GLenum pname, GLvoid *string)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetProgramStringARB");
   CALL_GetProgramStringARB(ctx->Dispatch.Current, (target, pname, string));
}


/* DeleteObjectARB: marshalled asynchronously */
struct marshal_cmd_DeleteObjectARB
{
   struct marshal_cmd_base cmd_base;
   GLhandleARB obj;
};
uint32_t
_mesa_unmarshal_DeleteObjectARB(struct gl_context *ctx, const struct marshal_cmd_DeleteObjectARB *restrict cmd)
{
   GLhandleARB obj = cmd->obj;
   CALL_DeleteObjectARB(ctx->Dispatch.Current, (obj));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_DeleteObjectARB), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_DeleteObjectARB(GLhandleARB obj)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_DeleteObjectARB);
   struct marshal_cmd_DeleteObjectARB *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_DeleteObjectARB, cmd_size);
   cmd->obj = obj;
   _mesa_glthread_ProgramChanged(ctx);
}


/* GetHandleARB: marshalled synchronously */
static GLhandleARB GLAPIENTRY
_mesa_marshal_GetHandleARB(GLenum pname)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetHandleARB");
   return CALL_GetHandleARB(ctx->Dispatch.Current, (pname));
}


/* DetachObjectARB: marshalled asynchronously */
struct marshal_cmd_DetachObjectARB
{
   struct marshal_cmd_base cmd_base;
   GLhandleARB containerObj;
   GLhandleARB attachedObj;
};
uint32_t
_mesa_unmarshal_DetachObjectARB(struct gl_context *ctx, const struct marshal_cmd_DetachObjectARB *restrict cmd)
{
   GLhandleARB containerObj = cmd->containerObj;
   GLhandleARB attachedObj = cmd->attachedObj;
   CALL_DetachObjectARB(ctx->Dispatch.Current, (containerObj, attachedObj));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_DetachObjectARB), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_DetachObjectARB(GLhandleARB containerObj, GLhandleARB attachedObj)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_DetachObjectARB);
   struct marshal_cmd_DetachObjectARB *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_DetachObjectARB, cmd_size);
   cmd->containerObj = containerObj;
   cmd->attachedObj = attachedObj;
}


/* CreateShaderObjectARB: marshalled synchronously */
static GLhandleARB GLAPIENTRY
_mesa_marshal_CreateShaderObjectARB(GLenum shaderType)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "CreateShaderObjectARB");
   return CALL_CreateShaderObjectARB(ctx->Dispatch.Current, (shaderType));
}


/* CreateProgramObjectARB: marshalled synchronously */
static GLhandleARB GLAPIENTRY
_mesa_marshal_CreateProgramObjectARB(void)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "CreateProgramObjectARB");
   return CALL_CreateProgramObjectARB(ctx->Dispatch.Current, ());
}


/* AttachObjectARB: marshalled asynchronously */
struct marshal_cmd_AttachObjectARB
{
   struct marshal_cmd_base cmd_base;
   GLhandleARB containerObj;
   GLhandleARB obj;
};
uint32_t
_mesa_unmarshal_AttachObjectARB(struct gl_context *ctx, const struct marshal_cmd_AttachObjectARB *restrict cmd)
{
   GLhandleARB containerObj = cmd->containerObj;
   GLhandleARB obj = cmd->obj;
   CALL_AttachObjectARB(ctx->Dispatch.Current, (containerObj, obj));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_AttachObjectARB), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_AttachObjectARB(GLhandleARB containerObj, GLhandleARB obj)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_AttachObjectARB);
   struct marshal_cmd_AttachObjectARB *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_AttachObjectARB, cmd_size);
   cmd->containerObj = containerObj;
   cmd->obj = obj;
}


/* GetObjectParameterfvARB: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetObjectParameterfvARB(GLhandleARB obj, GLenum pname, GLfloat *params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetObjectParameterfvARB");
   CALL_GetObjectParameterfvARB(ctx->Dispatch.Current, (obj, pname, params));
}


/* GetObjectParameterivARB: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetObjectParameterivARB(GLhandleARB obj, GLenum pname, GLint *params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetObjectParameterivARB");
   CALL_GetObjectParameterivARB(ctx->Dispatch.Current, (obj, pname, params));
}


/* GetInfoLogARB: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetInfoLogARB(GLhandleARB obj, GLsizei maxLength, GLsizei *length, GLcharARB *infoLog)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetInfoLogARB");
   CALL_GetInfoLogARB(ctx->Dispatch.Current, (obj, maxLength, length, infoLog));
}


/* GetAttachedObjectsARB: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetAttachedObjectsARB(GLhandleARB containerObj, GLsizei maxLength, GLsizei *length, GLhandleARB *infoLog)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetAttachedObjectsARB");
   CALL_GetAttachedObjectsARB(ctx->Dispatch.Current, (containerObj, maxLength, length, infoLog));
}


/* ClampColor: marshalled asynchronously */
struct marshal_cmd_ClampColor
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLenum16 clamp;
};
uint32_t
_mesa_unmarshal_ClampColor(struct gl_context *ctx, const struct marshal_cmd_ClampColor *restrict cmd)
{
   GLenum target = cmd->target;
   GLenum clamp = cmd->clamp;
   CALL_ClampColor(ctx->Dispatch.Current, (target, clamp));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_ClampColor), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ClampColor(GLenum target, GLenum clamp)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ClampColor);
   struct marshal_cmd_ClampColor *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ClampColor, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->clamp = MIN2(clamp, 0xffff); /* clamped to 0xffff (invalid enum) */
}


/* IsRenderbuffer: marshalled synchronously */
static GLboolean GLAPIENTRY
_mesa_marshal_IsRenderbuffer(GLuint renderbuffer)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "IsRenderbuffer");
   return CALL_IsRenderbuffer(ctx->Dispatch.Current, (renderbuffer));
}


/* BindRenderbuffer: marshalled asynchronously */
struct marshal_cmd_BindRenderbuffer
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLuint renderbuffer;
};
uint32_t
_mesa_unmarshal_BindRenderbuffer(struct gl_context *ctx, const struct marshal_cmd_BindRenderbuffer *restrict cmd)
{
   GLenum target = cmd->target;
   GLuint renderbuffer = cmd->renderbuffer;
   CALL_BindRenderbuffer(ctx->Dispatch.Current, (target, renderbuffer));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_BindRenderbuffer), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_BindRenderbuffer(GLenum target, GLuint renderbuffer)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_BindRenderbuffer);
   struct marshal_cmd_BindRenderbuffer *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_BindRenderbuffer, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->renderbuffer = renderbuffer;
}


/* DeleteRenderbuffers: marshalled asynchronously */
struct marshal_cmd_DeleteRenderbuffers
{
   struct marshal_cmd_base cmd_base;
   GLsizei n;
   /* Next safe_mul(n, 1 * sizeof(GLuint)) bytes are GLuint renderbuffers[n] */
};
uint32_t
_mesa_unmarshal_DeleteRenderbuffers(struct gl_context *ctx, const struct marshal_cmd_DeleteRenderbuffers *restrict cmd)
{
   GLsizei n = cmd->n;
   GLuint *renderbuffers;
   const char *variable_data = (const char *) (cmd + 1);
   renderbuffers = (GLuint *) variable_data;
   CALL_DeleteRenderbuffers(ctx->Dispatch.Current, (n, renderbuffers));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_DeleteRenderbuffers(GLsizei n, const GLuint *renderbuffers)
{
   GET_CURRENT_CONTEXT(ctx);
   int renderbuffers_size = safe_mul(n, 1 * sizeof(GLuint));
   int cmd_size = sizeof(struct marshal_cmd_DeleteRenderbuffers) + renderbuffers_size;
   struct marshal_cmd_DeleteRenderbuffers *cmd;
   if (unlikely(renderbuffers_size < 0 || (renderbuffers_size > 0 && !renderbuffers) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "DeleteRenderbuffers");
      CALL_DeleteRenderbuffers(ctx->Dispatch.Current, (n, renderbuffers));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_DeleteRenderbuffers, cmd_size);
   cmd->n = n;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, renderbuffers, renderbuffers_size);
}


/* GenRenderbuffers: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GenRenderbuffers(GLsizei n, GLuint *renderbuffers)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GenRenderbuffers");
   CALL_GenRenderbuffers(ctx->Dispatch.Current, (n, renderbuffers));
}


/* RenderbufferStorage: marshalled asynchronously */
struct marshal_cmd_RenderbufferStorage
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLenum16 internalformat;
   GLsizei width;
   GLsizei height;
};
uint32_t
_mesa_unmarshal_RenderbufferStorage(struct gl_context *ctx, const struct marshal_cmd_RenderbufferStorage *restrict cmd)
{
   GLenum target = cmd->target;
   GLenum internalformat = cmd->internalformat;
   GLsizei width = cmd->width;
   GLsizei height = cmd->height;
   CALL_RenderbufferStorage(ctx->Dispatch.Current, (target, internalformat, width, height));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_RenderbufferStorage), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_RenderbufferStorage(GLenum target, GLenum internalformat, GLsizei width, GLsizei height)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_RenderbufferStorage);
   struct marshal_cmd_RenderbufferStorage *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_RenderbufferStorage, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->internalformat = MIN2(internalformat, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->width = width;
   cmd->height = height;
}


/* RenderbufferStorageMultisample: marshalled asynchronously */
struct marshal_cmd_RenderbufferStorageMultisample
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLenum16 internalformat;
   GLsizei samples;
   GLsizei width;
   GLsizei height;
};
uint32_t
_mesa_unmarshal_RenderbufferStorageMultisample(struct gl_context *ctx, const struct marshal_cmd_RenderbufferStorageMultisample *restrict cmd)
{
   GLenum target = cmd->target;
   GLsizei samples = cmd->samples;
   GLenum internalformat = cmd->internalformat;
   GLsizei width = cmd->width;
   GLsizei height = cmd->height;
   CALL_RenderbufferStorageMultisample(ctx->Dispatch.Current, (target, samples, internalformat, width, height));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_RenderbufferStorageMultisample), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_RenderbufferStorageMultisample(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_RenderbufferStorageMultisample);
   struct marshal_cmd_RenderbufferStorageMultisample *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_RenderbufferStorageMultisample, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->samples = samples;
   cmd->internalformat = MIN2(internalformat, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->width = width;
   cmd->height = height;
}


/* GetRenderbufferParameteriv: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetRenderbufferParameteriv(GLenum target, GLenum pname, GLint *params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetRenderbufferParameteriv");
   CALL_GetRenderbufferParameteriv(ctx->Dispatch.Current, (target, pname, params));
}


/* IsFramebuffer: marshalled synchronously */
static GLboolean GLAPIENTRY
_mesa_marshal_IsFramebuffer(GLuint framebuffer)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "IsFramebuffer");
   return CALL_IsFramebuffer(ctx->Dispatch.Current, (framebuffer));
}


/* BindFramebuffer: marshalled asynchronously */
struct marshal_cmd_BindFramebuffer
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLuint framebuffer;
};
uint32_t
_mesa_unmarshal_BindFramebuffer(struct gl_context *ctx, const struct marshal_cmd_BindFramebuffer *restrict cmd)
{
   GLenum target = cmd->target;
   GLuint framebuffer = cmd->framebuffer;
   CALL_BindFramebuffer(ctx->Dispatch.Current, (target, framebuffer));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_BindFramebuffer), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_BindFramebuffer(GLenum target, GLuint framebuffer)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_BindFramebuffer);
   struct marshal_cmd_BindFramebuffer *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_BindFramebuffer, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->framebuffer = framebuffer;
   _mesa_glthread_BindFramebuffer(ctx, target, framebuffer);
}


/* DeleteFramebuffers: marshalled asynchronously */
struct marshal_cmd_DeleteFramebuffers
{
   struct marshal_cmd_base cmd_base;
   GLsizei n;
   /* Next safe_mul(n, 1 * sizeof(GLuint)) bytes are GLuint framebuffers[n] */
};
uint32_t
_mesa_unmarshal_DeleteFramebuffers(struct gl_context *ctx, const struct marshal_cmd_DeleteFramebuffers *restrict cmd)
{
   GLsizei n = cmd->n;
   GLuint *framebuffers;
   const char *variable_data = (const char *) (cmd + 1);
   framebuffers = (GLuint *) variable_data;
   CALL_DeleteFramebuffers(ctx->Dispatch.Current, (n, framebuffers));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_DeleteFramebuffers(GLsizei n, const GLuint *framebuffers)
{
   GET_CURRENT_CONTEXT(ctx);
   int framebuffers_size = safe_mul(n, 1 * sizeof(GLuint));
   int cmd_size = sizeof(struct marshal_cmd_DeleteFramebuffers) + framebuffers_size;
   struct marshal_cmd_DeleteFramebuffers *cmd;
   if (unlikely(framebuffers_size < 0 || (framebuffers_size > 0 && !framebuffers) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "DeleteFramebuffers");
      CALL_DeleteFramebuffers(ctx->Dispatch.Current, (n, framebuffers));
      _mesa_glthread_DeleteFramebuffers(ctx, n, framebuffers);
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_DeleteFramebuffers, cmd_size);
   cmd->n = n;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, framebuffers, framebuffers_size);
   _mesa_glthread_DeleteFramebuffers(ctx, n, framebuffers);
}


/* GenFramebuffers: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GenFramebuffers(GLsizei n, GLuint *framebuffers)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GenFramebuffers");
   CALL_GenFramebuffers(ctx->Dispatch.Current, (n, framebuffers));
}


/* CheckFramebufferStatus: marshalled synchronously */
static GLenum GLAPIENTRY
_mesa_marshal_CheckFramebufferStatus(GLenum target)
{
   GET_CURRENT_CONTEXT(ctx);
   if (ctx->Const.GLThreadNopCheckFramebufferStatus) return GL_FRAMEBUFFER_COMPLETE;
   _mesa_glthread_finish_before(ctx, "CheckFramebufferStatus");
   return CALL_CheckFramebufferStatus(ctx->Dispatch.Current, (target));
}


/* FramebufferTexture1D: marshalled asynchronously */
struct marshal_cmd_FramebufferTexture1D
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLenum16 attachment;
   GLenum16 textarget;
   GLuint texture;
   GLint level;
};
uint32_t
_mesa_unmarshal_FramebufferTexture1D(struct gl_context *ctx, const struct marshal_cmd_FramebufferTexture1D *restrict cmd)
{
   GLenum target = cmd->target;
   GLenum attachment = cmd->attachment;
   GLenum textarget = cmd->textarget;
   GLuint texture = cmd->texture;
   GLint level = cmd->level;
   CALL_FramebufferTexture1D(ctx->Dispatch.Current, (target, attachment, textarget, texture, level));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_FramebufferTexture1D), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_FramebufferTexture1D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_FramebufferTexture1D);
   struct marshal_cmd_FramebufferTexture1D *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_FramebufferTexture1D, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->attachment = MIN2(attachment, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->textarget = MIN2(textarget, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->texture = texture;
   cmd->level = level;
}


/* FramebufferTexture2D: marshalled asynchronously */
struct marshal_cmd_FramebufferTexture2D
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLenum16 attachment;
   GLenum16 textarget;
   GLuint texture;
   GLint level;
};
uint32_t
_mesa_unmarshal_FramebufferTexture2D(struct gl_context *ctx, const struct marshal_cmd_FramebufferTexture2D *restrict cmd)
{
   GLenum target = cmd->target;
   GLenum attachment = cmd->attachment;
   GLenum textarget = cmd->textarget;
   GLuint texture = cmd->texture;
   GLint level = cmd->level;
   CALL_FramebufferTexture2D(ctx->Dispatch.Current, (target, attachment, textarget, texture, level));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_FramebufferTexture2D), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_FramebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_FramebufferTexture2D);
   struct marshal_cmd_FramebufferTexture2D *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_FramebufferTexture2D, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->attachment = MIN2(attachment, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->textarget = MIN2(textarget, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->texture = texture;
   cmd->level = level;
}


/* FramebufferTexture3D: marshalled asynchronously */
struct marshal_cmd_FramebufferTexture3D
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLenum16 attachment;
   GLenum16 textarget;
   GLuint texture;
   GLint level;
   GLint layer;
};
uint32_t
_mesa_unmarshal_FramebufferTexture3D(struct gl_context *ctx, const struct marshal_cmd_FramebufferTexture3D *restrict cmd)
{
   GLenum target = cmd->target;
   GLenum attachment = cmd->attachment;
   GLenum textarget = cmd->textarget;
   GLuint texture = cmd->texture;
   GLint level = cmd->level;
   GLint layer = cmd->layer;
   CALL_FramebufferTexture3D(ctx->Dispatch.Current, (target, attachment, textarget, texture, level, layer));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_FramebufferTexture3D), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_FramebufferTexture3D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint layer)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_FramebufferTexture3D);
   struct marshal_cmd_FramebufferTexture3D *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_FramebufferTexture3D, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->attachment = MIN2(attachment, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->textarget = MIN2(textarget, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->texture = texture;
   cmd->level = level;
   cmd->layer = layer;
}


/* FramebufferTextureLayer: marshalled asynchronously */
struct marshal_cmd_FramebufferTextureLayer
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLenum16 attachment;
   GLuint texture;
   GLint level;
   GLint layer;
};
uint32_t
_mesa_unmarshal_FramebufferTextureLayer(struct gl_context *ctx, const struct marshal_cmd_FramebufferTextureLayer *restrict cmd)
{
   GLenum target = cmd->target;
   GLenum attachment = cmd->attachment;
   GLuint texture = cmd->texture;
   GLint level = cmd->level;
   GLint layer = cmd->layer;
   CALL_FramebufferTextureLayer(ctx->Dispatch.Current, (target, attachment, texture, level, layer));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_FramebufferTextureLayer), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_FramebufferTextureLayer(GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_FramebufferTextureLayer);
   struct marshal_cmd_FramebufferTextureLayer *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_FramebufferTextureLayer, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->attachment = MIN2(attachment, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->texture = texture;
   cmd->level = level;
   cmd->layer = layer;
}


void
_mesa_glthread_init_dispatch2(struct gl_context *ctx, struct _glapi_table *table)
{
   if (_mesa_is_desktop_gl(ctx)) {
      SET_AttachObjectARB(table, _mesa_marshal_AttachObjectARB);
      SET_ClampColor(table, _mesa_marshal_ClampColor);
      SET_CreateProgramObjectARB(table, _mesa_marshal_CreateProgramObjectARB);
      SET_CreateShaderObjectARB(table, _mesa_marshal_CreateShaderObjectARB);
      SET_DeleteObjectARB(table, _mesa_marshal_DeleteObjectARB);
      SET_DetachObjectARB(table, _mesa_marshal_DetachObjectARB);
      SET_FramebufferTexture1D(table, _mesa_marshal_FramebufferTexture1D);
      SET_GetAttachedObjectsARB(table, _mesa_marshal_GetAttachedObjectsARB);
      SET_GetBufferSubData(table, _mesa_marshal_GetBufferSubData);
      SET_GetHandleARB(table, _mesa_marshal_GetHandleARB);
      SET_GetInfoLogARB(table, _mesa_marshal_GetInfoLogARB);
      SET_GetObjectParameterfvARB(table, _mesa_marshal_GetObjectParameterfvARB);
      SET_GetObjectParameterivARB(table, _mesa_marshal_GetObjectParameterivARB);
      SET_GetVertexAttribdv(table, _mesa_marshal_GetVertexAttribdv);
      SET_PointParameteri(table, _mesa_marshal_PointParameteri);
      SET_PointParameteriv(table, _mesa_marshal_PointParameteriv);
      SET_VertexAttrib1d(table, _mesa_marshal_VertexAttrib1d);
      SET_VertexAttrib1dv(table, _mesa_marshal_VertexAttrib1dv);
      SET_VertexAttrib1s(table, _mesa_marshal_VertexAttrib1s);
      SET_VertexAttrib1sv(table, _mesa_marshal_VertexAttrib1sv);
      SET_VertexAttrib2d(table, _mesa_marshal_VertexAttrib2d);
      SET_VertexAttrib2dv(table, _mesa_marshal_VertexAttrib2dv);
      SET_VertexAttrib2s(table, _mesa_marshal_VertexAttrib2s);
      SET_VertexAttrib2sv(table, _mesa_marshal_VertexAttrib2sv);
      SET_VertexAttrib3d(table, _mesa_marshal_VertexAttrib3d);
      SET_VertexAttrib3dv(table, _mesa_marshal_VertexAttrib3dv);
      SET_VertexAttrib3s(table, _mesa_marshal_VertexAttrib3s);
      SET_VertexAttrib3sv(table, _mesa_marshal_VertexAttrib3sv);
      SET_VertexAttrib4Nbv(table, _mesa_marshal_VertexAttrib4Nbv);
      SET_VertexAttrib4Niv(table, _mesa_marshal_VertexAttrib4Niv);
      SET_VertexAttrib4Nsv(table, _mesa_marshal_VertexAttrib4Nsv);
      SET_VertexAttrib4Nub(table, _mesa_marshal_VertexAttrib4Nub);
      SET_VertexAttrib4Nubv(table, _mesa_marshal_VertexAttrib4Nubv);
      SET_VertexAttrib4Nuiv(table, _mesa_marshal_VertexAttrib4Nuiv);
      SET_VertexAttrib4Nusv(table, _mesa_marshal_VertexAttrib4Nusv);
      SET_VertexAttrib4bv(table, _mesa_marshal_VertexAttrib4bv);
      SET_VertexAttrib4d(table, _mesa_marshal_VertexAttrib4d);
      SET_VertexAttrib4dv(table, _mesa_marshal_VertexAttrib4dv);
      SET_VertexAttrib4iv(table, _mesa_marshal_VertexAttrib4iv);
      SET_VertexAttrib4s(table, _mesa_marshal_VertexAttrib4s);
      SET_VertexAttrib4sv(table, _mesa_marshal_VertexAttrib4sv);
      SET_VertexAttrib4ubv(table, _mesa_marshal_VertexAttrib4ubv);
      SET_VertexAttrib4uiv(table, _mesa_marshal_VertexAttrib4uiv);
      SET_VertexAttrib4usv(table, _mesa_marshal_VertexAttrib4usv);
   }
   if (_mesa_is_desktop_gl(ctx) || (_mesa_is_gles2(ctx) && ctx->Version >= 30)) {
      SET_FramebufferTextureLayer(table, _mesa_marshal_FramebufferTextureLayer);
      SET_UniformMatrix2x3fv(table, _mesa_marshal_UniformMatrix2x3fv);
      SET_UniformMatrix2x4fv(table, _mesa_marshal_UniformMatrix2x4fv);
      SET_UniformMatrix3x2fv(table, _mesa_marshal_UniformMatrix3x2fv);
      SET_UniformMatrix3x4fv(table, _mesa_marshal_UniformMatrix3x4fv);
      SET_UniformMatrix4x2fv(table, _mesa_marshal_UniformMatrix4x2fv);
      SET_UniformMatrix4x3fv(table, _mesa_marshal_UniformMatrix4x3fv);
   }
   if (_mesa_is_desktop_gl(ctx) || _mesa_is_gles1(ctx)) {
      SET_PointParameterfv(table, _mesa_marshal_PointParameterfv);
   }
   if (_mesa_is_desktop_gl(ctx) || _mesa_is_gles1(ctx) || _mesa_is_gles2(ctx)) {
      SET_BindFramebuffer(table, _mesa_marshal_BindFramebuffer);
      SET_BindRenderbuffer(table, _mesa_marshal_BindRenderbuffer);
      SET_BlendEquationSeparate(table, _mesa_marshal_BlendEquationSeparate);
      SET_CheckFramebufferStatus(table, _mesa_marshal_CheckFramebufferStatus);
      SET_DeleteBuffers(table, _mesa_marshal_DeleteBuffers);
      SET_DeleteFramebuffers(table, _mesa_marshal_DeleteFramebuffers);
      SET_DeleteRenderbuffers(table, _mesa_marshal_DeleteRenderbuffers);
      SET_FramebufferTexture2D(table, _mesa_marshal_FramebufferTexture2D);
      SET_GenBuffers(table, _mesa_marshal_GenBuffers);
      SET_GenFramebuffers(table, _mesa_marshal_GenFramebuffers);
      SET_GenRenderbuffers(table, _mesa_marshal_GenRenderbuffers);
      SET_GetBufferParameteriv(table, _mesa_marshal_GetBufferParameteriv);
      SET_GetBufferPointerv(table, _mesa_marshal_GetBufferPointerv);
      SET_GetRenderbufferParameteriv(table, _mesa_marshal_GetRenderbufferParameteriv);
      SET_IsBuffer(table, _mesa_marshal_IsBuffer);
      SET_IsFramebuffer(table, _mesa_marshal_IsFramebuffer);
      SET_IsRenderbuffer(table, _mesa_marshal_IsRenderbuffer);
      SET_MapBuffer(table, _mesa_marshal_MapBuffer);
      SET_RenderbufferStorage(table, _mesa_marshal_RenderbufferStorage);
      SET_UnmapBuffer(table, _mesa_marshal_UnmapBuffer);
   }
   if (_mesa_is_desktop_gl(ctx) || _mesa_is_gles2(ctx)) {
      SET_AttachShader(table, _mesa_marshal_AttachShader);
      SET_BeginQuery(table, _mesa_marshal_BeginQuery);
      SET_BindAttribLocation(table, _mesa_marshal_BindAttribLocation);
      SET_CompileShader(table, _mesa_marshal_CompileShader);
      SET_CreateProgram(table, _mesa_marshal_CreateProgram);
      SET_CreateShader(table, _mesa_marshal_CreateShader);
      SET_DeleteProgram(table, _mesa_marshal_DeleteProgram);
      SET_DeleteQueries(table, _mesa_marshal_DeleteQueries);
      SET_DeleteShader(table, _mesa_marshal_DeleteShader);
      SET_DetachShader(table, _mesa_marshal_DetachShader);
      SET_DisableVertexAttribArray(table, _mesa_marshal_DisableVertexAttribArray);
      SET_DrawBuffers(table, _mesa_marshal_DrawBuffers);
      SET_EnableVertexAttribArray(table, _mesa_marshal_EnableVertexAttribArray);
      SET_EndQuery(table, _mesa_marshal_EndQuery);
      SET_FramebufferTexture3D(table, _mesa_marshal_FramebufferTexture3D);
      SET_GenQueries(table, _mesa_marshal_GenQueries);
      SET_GetActiveAttrib(table, _mesa_marshal_GetActiveAttrib);
      SET_GetAttachedShaders(table, _mesa_marshal_GetAttachedShaders);
      SET_GetAttribLocation(table, _mesa_marshal_GetAttribLocation);
      SET_GetProgramInfoLog(table, _mesa_marshal_GetProgramInfoLog);
      SET_GetProgramiv(table, _mesa_marshal_GetProgramiv);
      SET_GetQueryObjectiv(table, _mesa_marshal_GetQueryObjectiv);
      SET_GetQueryObjectuiv(table, _mesa_marshal_GetQueryObjectuiv);
      SET_GetQueryiv(table, _mesa_marshal_GetQueryiv);
      SET_GetShaderInfoLog(table, _mesa_marshal_GetShaderInfoLog);
      SET_GetShaderSource(table, _mesa_marshal_GetShaderSource);
      SET_GetShaderiv(table, _mesa_marshal_GetShaderiv);
      SET_GetUniformfv(table, _mesa_marshal_GetUniformfv);
      SET_GetUniformiv(table, _mesa_marshal_GetUniformiv);
      SET_GetVertexAttribPointerv(table, _mesa_marshal_GetVertexAttribPointerv);
      SET_GetVertexAttribfv(table, _mesa_marshal_GetVertexAttribfv);
      SET_GetVertexAttribiv(table, _mesa_marshal_GetVertexAttribiv);
      SET_IsProgram(table, _mesa_marshal_IsProgram);
      SET_IsQuery(table, _mesa_marshal_IsQuery);
      SET_IsShader(table, _mesa_marshal_IsShader);
      SET_LinkProgram(table, _mesa_marshal_LinkProgram);
      SET_RenderbufferStorageMultisample(table, _mesa_marshal_RenderbufferStorageMultisample);
      SET_ShaderSource(table, _mesa_marshal_ShaderSource);
      SET_StencilFuncSeparate(table, _mesa_marshal_StencilFuncSeparate);
      SET_StencilMaskSeparate(table, _mesa_marshal_StencilMaskSeparate);
      SET_StencilOpSeparate(table, _mesa_marshal_StencilOpSeparate);
      SET_Uniform1f(table, _mesa_marshal_Uniform1f);
      SET_Uniform1fv(table, _mesa_marshal_Uniform1fv);
      SET_Uniform1i(table, _mesa_marshal_Uniform1i);
      SET_Uniform1iv(table, _mesa_marshal_Uniform1iv);
      SET_Uniform2f(table, _mesa_marshal_Uniform2f);
      SET_Uniform2fv(table, _mesa_marshal_Uniform2fv);
      SET_Uniform2i(table, _mesa_marshal_Uniform2i);
      SET_Uniform2iv(table, _mesa_marshal_Uniform2iv);
      SET_Uniform3f(table, _mesa_marshal_Uniform3f);
      SET_Uniform3fv(table, _mesa_marshal_Uniform3fv);
      SET_Uniform3i(table, _mesa_marshal_Uniform3i);
      SET_Uniform3iv(table, _mesa_marshal_Uniform3iv);
      SET_Uniform4f(table, _mesa_marshal_Uniform4f);
      SET_Uniform4fv(table, _mesa_marshal_Uniform4fv);
      SET_Uniform4i(table, _mesa_marshal_Uniform4i);
      SET_Uniform4iv(table, _mesa_marshal_Uniform4iv);
      SET_UniformMatrix2fv(table, _mesa_marshal_UniformMatrix2fv);
      SET_UniformMatrix3fv(table, _mesa_marshal_UniformMatrix3fv);
      SET_UniformMatrix4fv(table, _mesa_marshal_UniformMatrix4fv);
      SET_UseProgram(table, _mesa_marshal_UseProgram);
      SET_ValidateProgram(table, _mesa_marshal_ValidateProgram);
      SET_VertexAttrib1fARB(table, _mesa_marshal_VertexAttrib1fARB);
      SET_VertexAttrib1fvARB(table, _mesa_marshal_VertexAttrib1fvARB);
      SET_VertexAttrib2fARB(table, _mesa_marshal_VertexAttrib2fARB);
      SET_VertexAttrib2fvARB(table, _mesa_marshal_VertexAttrib2fvARB);
      SET_VertexAttrib3fARB(table, _mesa_marshal_VertexAttrib3fARB);
      SET_VertexAttrib3fvARB(table, _mesa_marshal_VertexAttrib3fvARB);
      SET_VertexAttrib4fARB(table, _mesa_marshal_VertexAttrib4fARB);
      SET_VertexAttrib4fvARB(table, _mesa_marshal_VertexAttrib4fvARB);
      SET_VertexAttribPointer(table, _mesa_marshal_VertexAttribPointer);
   }
   if (_mesa_is_desktop_gl_compat(ctx)) {
      SET_BindProgramARB(table, _mesa_marshal_BindProgramARB);
      SET_DeleteProgramsARB(table, _mesa_marshal_DeleteProgramsARB);
      SET_GenProgramsARB(table, _mesa_marshal_GenProgramsARB);
      SET_GetProgramEnvParameterdvARB(table, _mesa_marshal_GetProgramEnvParameterdvARB);
      SET_GetProgramEnvParameterfvARB(table, _mesa_marshal_GetProgramEnvParameterfvARB);
      SET_GetProgramLocalParameterdvARB(table, _mesa_marshal_GetProgramLocalParameterdvARB);
      SET_GetProgramLocalParameterfvARB(table, _mesa_marshal_GetProgramLocalParameterfvARB);
      SET_GetProgramStringARB(table, _mesa_marshal_GetProgramStringARB);
      SET_GetProgramivARB(table, _mesa_marshal_GetProgramivARB);
      SET_IsProgramARB(table, _mesa_marshal_IsProgramARB);
      SET_ProgramEnvParameter4dARB(table, _mesa_marshal_ProgramEnvParameter4dARB);
      SET_ProgramEnvParameter4dvARB(table, _mesa_marshal_ProgramEnvParameter4dvARB);
      SET_ProgramEnvParameter4fARB(table, _mesa_marshal_ProgramEnvParameter4fARB);
      SET_ProgramEnvParameter4fvARB(table, _mesa_marshal_ProgramEnvParameter4fvARB);
      SET_ProgramLocalParameter4dARB(table, _mesa_marshal_ProgramLocalParameter4dARB);
      SET_ProgramLocalParameter4dvARB(table, _mesa_marshal_ProgramLocalParameter4dvARB);
      SET_ProgramLocalParameter4fARB(table, _mesa_marshal_ProgramLocalParameter4fARB);
      SET_ProgramLocalParameter4fvARB(table, _mesa_marshal_ProgramLocalParameter4fvARB);
      SET_ProgramStringARB(table, _mesa_marshal_ProgramStringARB);
      SET_SecondaryColor3b(table, _mesa_marshal_SecondaryColor3b);
      SET_SecondaryColor3bv(table, _mesa_marshal_SecondaryColor3bv);
      SET_SecondaryColor3d(table, _mesa_marshal_SecondaryColor3d);
      SET_SecondaryColor3dv(table, _mesa_marshal_SecondaryColor3dv);
      SET_SecondaryColor3fEXT(table, _mesa_marshal_SecondaryColor3fEXT);
      SET_SecondaryColor3fvEXT(table, _mesa_marshal_SecondaryColor3fvEXT);
      SET_SecondaryColor3i(table, _mesa_marshal_SecondaryColor3i);
      SET_SecondaryColor3iv(table, _mesa_marshal_SecondaryColor3iv);
      SET_SecondaryColor3s(table, _mesa_marshal_SecondaryColor3s);
      SET_SecondaryColor3sv(table, _mesa_marshal_SecondaryColor3sv);
      SET_SecondaryColor3ub(table, _mesa_marshal_SecondaryColor3ub);
      SET_SecondaryColor3ubv(table, _mesa_marshal_SecondaryColor3ubv);
      SET_SecondaryColor3ui(table, _mesa_marshal_SecondaryColor3ui);
      SET_SecondaryColor3uiv(table, _mesa_marshal_SecondaryColor3uiv);
      SET_SecondaryColor3us(table, _mesa_marshal_SecondaryColor3us);
      SET_SecondaryColor3usv(table, _mesa_marshal_SecondaryColor3usv);
      SET_SecondaryColorPointer(table, _mesa_marshal_SecondaryColorPointer);
      SET_WindowPos2d(table, _mesa_marshal_WindowPos2d);
      SET_WindowPos2dv(table, _mesa_marshal_WindowPos2dv);
      SET_WindowPos2f(table, _mesa_marshal_WindowPos2f);
      SET_WindowPos2fv(table, _mesa_marshal_WindowPos2fv);
      SET_WindowPos2i(table, _mesa_marshal_WindowPos2i);
      SET_WindowPos2iv(table, _mesa_marshal_WindowPos2iv);
      SET_WindowPos2s(table, _mesa_marshal_WindowPos2s);
      SET_WindowPos2sv(table, _mesa_marshal_WindowPos2sv);
      SET_WindowPos3d(table, _mesa_marshal_WindowPos3d);
      SET_WindowPos3dv(table, _mesa_marshal_WindowPos3dv);
      SET_WindowPos3f(table, _mesa_marshal_WindowPos3f);
      SET_WindowPos3fv(table, _mesa_marshal_WindowPos3fv);
      SET_WindowPos3i(table, _mesa_marshal_WindowPos3i);
      SET_WindowPos3iv(table, _mesa_marshal_WindowPos3iv);
      SET_WindowPos3s(table, _mesa_marshal_WindowPos3s);
      SET_WindowPos3sv(table, _mesa_marshal_WindowPos3sv);
   }
}
