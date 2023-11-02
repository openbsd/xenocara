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

/* NewList: marshalled asynchronously */
struct marshal_cmd_NewList
{
   struct marshal_cmd_base cmd_base;
   GLenum16 mode;
   GLuint list;
};
uint32_t
_mesa_unmarshal_NewList(struct gl_context *ctx, const struct marshal_cmd_NewList *restrict cmd)
{
   GLuint list = cmd->list;
   GLenum mode = cmd->mode;
   CALL_NewList(ctx->Dispatch.Current, (list, mode));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_NewList), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_NewList(GLuint list, GLenum mode)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_NewList);
   struct marshal_cmd_NewList *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_NewList, cmd_size);
   cmd->list = list;
   cmd->mode = MIN2(mode, 0xffff); /* clamped to 0xffff (invalid enum) */
   _mesa_glthread_NewList(ctx, list, mode);
}


/* EndList: marshalled asynchronously */
struct marshal_cmd_EndList
{
   struct marshal_cmd_base cmd_base;
};
uint32_t
_mesa_unmarshal_EndList(struct gl_context *ctx, const struct marshal_cmd_EndList *restrict cmd)
{
   CALL_EndList(ctx->Dispatch.Current, ());
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_EndList), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_EndList(void)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_EndList);
   struct marshal_cmd_EndList *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_EndList, cmd_size);
   (void) cmd;
   _mesa_glthread_EndList(ctx);
}


/* CallLists: marshalled asynchronously */
struct marshal_cmd_CallLists
{
   struct marshal_cmd_base cmd_base;
   GLenum16 type;
   GLsizei n;
   /* Next (n * _mesa_calllists_enum_to_count(type)) bytes are GLvoid lists[n] */
};
uint32_t
_mesa_unmarshal_CallLists(struct gl_context *ctx, const struct marshal_cmd_CallLists *restrict cmd)
{
   GLsizei n = cmd->n;
   GLenum type = cmd->type;
   GLvoid *lists;
   const char *variable_data = (const char *) (cmd + 1);
   lists = (GLvoid *) variable_data;
   CALL_CallLists(ctx->Dispatch.Current, (n, type, lists));
   return cmd->cmd_base.cmd_size;
}
void GLAPIENTRY
_mesa_marshal_CallLists(GLsizei n, GLenum type, const GLvoid *lists)
{
   GET_CURRENT_CONTEXT(ctx);
   int lists_size = (n * _mesa_calllists_enum_to_count(type));
   int cmd_size = sizeof(struct marshal_cmd_CallLists) + lists_size;
   struct marshal_cmd_CallLists *cmd;
   if (unlikely(lists_size < 0 || (lists_size > 0 && !lists) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "CallLists");
      CALL_CallLists(ctx->Dispatch.Current, (n, type, lists));
      _mesa_glthread_CallLists(ctx, n, type, lists);
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_CallLists, cmd_size);
   cmd->n = n;
   cmd->type = MIN2(type, 0xffff); /* clamped to 0xffff (invalid enum) */
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, lists, lists_size);
   _mesa_glthread_CallLists(ctx, n, type, lists);
}


/* DeleteLists: marshalled asynchronously */
struct marshal_cmd_DeleteLists
{
   struct marshal_cmd_base cmd_base;
   GLuint list;
   GLsizei range;
};
uint32_t
_mesa_unmarshal_DeleteLists(struct gl_context *ctx, const struct marshal_cmd_DeleteLists *restrict cmd)
{
   GLuint list = cmd->list;
   GLsizei range = cmd->range;
   CALL_DeleteLists(ctx->Dispatch.Current, (list, range));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_DeleteLists), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_DeleteLists(GLuint list, GLsizei range)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_DeleteLists);
   struct marshal_cmd_DeleteLists *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_DeleteLists, cmd_size);
   cmd->list = list;
   cmd->range = range;
   _mesa_glthread_DeleteLists(ctx, range);
}


/* GenLists: marshalled synchronously */
static GLuint GLAPIENTRY
_mesa_marshal_GenLists(GLsizei range)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GenLists");
   return CALL_GenLists(ctx->Dispatch.Current, (range));
}


/* ListBase: marshalled asynchronously */
struct marshal_cmd_ListBase
{
   struct marshal_cmd_base cmd_base;
   GLuint base;
};
uint32_t
_mesa_unmarshal_ListBase(struct gl_context *ctx, const struct marshal_cmd_ListBase *restrict cmd)
{
   GLuint base = cmd->base;
   CALL_ListBase(ctx->Dispatch.Current, (base));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_ListBase), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ListBase(GLuint base)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ListBase);
   struct marshal_cmd_ListBase *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ListBase, cmd_size);
   cmd->base = base;
   _mesa_glthread_ListBase(ctx, base);
}


/* Begin: marshalled asynchronously */
struct marshal_cmd_Begin
{
   struct marshal_cmd_base cmd_base;
   GLenum16 mode;
};
uint32_t
_mesa_unmarshal_Begin(struct gl_context *ctx, const struct marshal_cmd_Begin *restrict cmd)
{
   GLenum mode = cmd->mode;
   CALL_Begin(ctx->Dispatch.Current, (mode));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Begin), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_Begin(GLenum mode)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Begin);
   struct marshal_cmd_Begin *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Begin, cmd_size);
   cmd->mode = MIN2(mode, 0xffff); /* clamped to 0xffff (invalid enum) */
   ctx->GLThread.inside_begin_end = true;
}


/* Bitmap: marshalled asynchronously */
struct marshal_cmd_Bitmap
{
   struct marshal_cmd_base cmd_base;
   GLsizei width;
   GLsizei height;
   GLfloat xorig;
   GLfloat yorig;
   GLfloat xmove;
   GLfloat ymove;
   const GLubyte * bitmap;
};
uint32_t
_mesa_unmarshal_Bitmap(struct gl_context *ctx, const struct marshal_cmd_Bitmap *restrict cmd)
{
   GLsizei width = cmd->width;
   GLsizei height = cmd->height;
   GLfloat xorig = cmd->xorig;
   GLfloat yorig = cmd->yorig;
   GLfloat xmove = cmd->xmove;
   GLfloat ymove = cmd->ymove;
   const GLubyte * bitmap = cmd->bitmap;
   CALL_Bitmap(ctx->Dispatch.Current, (width, height, xorig, yorig, xmove, ymove, bitmap));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Bitmap), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_Bitmap(GLsizei width, GLsizei height, GLfloat xorig, GLfloat yorig, GLfloat xmove, GLfloat ymove, const GLubyte *bitmap)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Bitmap);
   struct marshal_cmd_Bitmap *cmd;
   if (_mesa_glthread_has_no_unpack_buffer(ctx)) {
      _mesa_glthread_finish_before(ctx, "Bitmap");
      CALL_Bitmap(ctx->Dispatch.Current, (width, height, xorig, yorig, xmove, ymove, bitmap));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Bitmap, cmd_size);
   cmd->width = width;
   cmd->height = height;
   cmd->xorig = xorig;
   cmd->yorig = yorig;
   cmd->xmove = xmove;
   cmd->ymove = ymove;
   cmd->bitmap = bitmap;
}


/* Color3b: marshalled asynchronously */
struct marshal_cmd_Color3b
{
   struct marshal_cmd_base cmd_base;
   GLbyte red;
   GLbyte green;
   GLbyte blue;
};
uint32_t
_mesa_unmarshal_Color3b(struct gl_context *ctx, const struct marshal_cmd_Color3b *restrict cmd)
{
   GLbyte red = cmd->red;
   GLbyte green = cmd->green;
   GLbyte blue = cmd->blue;
   CALL_Color3b(ctx->Dispatch.Current, (red, green, blue));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Color3b), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_Color3b(GLbyte red, GLbyte green, GLbyte blue)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Color3b);
   struct marshal_cmd_Color3b *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Color3b, cmd_size);
   cmd->red = red;
   cmd->green = green;
   cmd->blue = blue;
}


/* Color3bv: marshalled asynchronously */
struct marshal_cmd_Color3bv
{
   struct marshal_cmd_base cmd_base;
   GLbyte v[3];
};
uint32_t
_mesa_unmarshal_Color3bv(struct gl_context *ctx, const struct marshal_cmd_Color3bv *restrict cmd)
{
   const GLbyte *v = cmd->v;
   CALL_Color3bv(ctx->Dispatch.Current, (v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Color3bv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_Color3bv(const GLbyte *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Color3bv);
   struct marshal_cmd_Color3bv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Color3bv, cmd_size);
   memcpy(cmd->v, v, 3 * sizeof(GLbyte));
}


/* Color3d: marshalled asynchronously */
struct marshal_cmd_Color3d
{
   struct marshal_cmd_base cmd_base;
   GLdouble red;
   GLdouble green;
   GLdouble blue;
};
uint32_t
_mesa_unmarshal_Color3d(struct gl_context *ctx, const struct marshal_cmd_Color3d *restrict cmd)
{
   GLdouble red = cmd->red;
   GLdouble green = cmd->green;
   GLdouble blue = cmd->blue;
   CALL_Color3d(ctx->Dispatch.Current, (red, green, blue));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Color3d), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_Color3d(GLdouble red, GLdouble green, GLdouble blue)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Color3d);
   struct marshal_cmd_Color3d *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Color3d, cmd_size);
   cmd->red = red;
   cmd->green = green;
   cmd->blue = blue;
}


/* Color3dv: marshalled asynchronously */
struct marshal_cmd_Color3dv
{
   struct marshal_cmd_base cmd_base;
   GLdouble v[3];
};
uint32_t
_mesa_unmarshal_Color3dv(struct gl_context *ctx, const struct marshal_cmd_Color3dv *restrict cmd)
{
   const GLdouble *v = cmd->v;
   CALL_Color3dv(ctx->Dispatch.Current, (v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Color3dv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_Color3dv(const GLdouble *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Color3dv);
   struct marshal_cmd_Color3dv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Color3dv, cmd_size);
   memcpy(cmd->v, v, 3 * sizeof(GLdouble));
}


/* Color3f: marshalled asynchronously */
struct marshal_cmd_Color3f
{
   struct marshal_cmd_base cmd_base;
   GLfloat red;
   GLfloat green;
   GLfloat blue;
};
uint32_t
_mesa_unmarshal_Color3f(struct gl_context *ctx, const struct marshal_cmd_Color3f *restrict cmd)
{
   GLfloat red = cmd->red;
   GLfloat green = cmd->green;
   GLfloat blue = cmd->blue;
   CALL_Color3f(ctx->Dispatch.Current, (red, green, blue));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Color3f), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_Color3f(GLfloat red, GLfloat green, GLfloat blue)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Color3f);
   struct marshal_cmd_Color3f *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Color3f, cmd_size);
   cmd->red = red;
   cmd->green = green;
   cmd->blue = blue;
}


/* Color3fv: marshalled asynchronously */
struct marshal_cmd_Color3fv
{
   struct marshal_cmd_base cmd_base;
   GLfloat v[3];
};
uint32_t
_mesa_unmarshal_Color3fv(struct gl_context *ctx, const struct marshal_cmd_Color3fv *restrict cmd)
{
   const GLfloat *v = cmd->v;
   CALL_Color3fv(ctx->Dispatch.Current, (v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Color3fv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_Color3fv(const GLfloat *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Color3fv);
   struct marshal_cmd_Color3fv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Color3fv, cmd_size);
   memcpy(cmd->v, v, 3 * sizeof(GLfloat));
}


/* Color3i: marshalled asynchronously */
struct marshal_cmd_Color3i
{
   struct marshal_cmd_base cmd_base;
   GLint red;
   GLint green;
   GLint blue;
};
uint32_t
_mesa_unmarshal_Color3i(struct gl_context *ctx, const struct marshal_cmd_Color3i *restrict cmd)
{
   GLint red = cmd->red;
   GLint green = cmd->green;
   GLint blue = cmd->blue;
   CALL_Color3i(ctx->Dispatch.Current, (red, green, blue));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Color3i), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_Color3i(GLint red, GLint green, GLint blue)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Color3i);
   struct marshal_cmd_Color3i *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Color3i, cmd_size);
   cmd->red = red;
   cmd->green = green;
   cmd->blue = blue;
}


/* Color3iv: marshalled asynchronously */
struct marshal_cmd_Color3iv
{
   struct marshal_cmd_base cmd_base;
   GLint v[3];
};
uint32_t
_mesa_unmarshal_Color3iv(struct gl_context *ctx, const struct marshal_cmd_Color3iv *restrict cmd)
{
   const GLint *v = cmd->v;
   CALL_Color3iv(ctx->Dispatch.Current, (v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Color3iv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_Color3iv(const GLint *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Color3iv);
   struct marshal_cmd_Color3iv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Color3iv, cmd_size);
   memcpy(cmd->v, v, 3 * sizeof(GLint));
}


/* Color3s: marshalled asynchronously */
struct marshal_cmd_Color3s
{
   struct marshal_cmd_base cmd_base;
   GLshort red;
   GLshort green;
   GLshort blue;
};
uint32_t
_mesa_unmarshal_Color3s(struct gl_context *ctx, const struct marshal_cmd_Color3s *restrict cmd)
{
   GLshort red = cmd->red;
   GLshort green = cmd->green;
   GLshort blue = cmd->blue;
   CALL_Color3s(ctx->Dispatch.Current, (red, green, blue));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Color3s), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_Color3s(GLshort red, GLshort green, GLshort blue)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Color3s);
   struct marshal_cmd_Color3s *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Color3s, cmd_size);
   cmd->red = red;
   cmd->green = green;
   cmd->blue = blue;
}


/* Color3sv: marshalled asynchronously */
struct marshal_cmd_Color3sv
{
   struct marshal_cmd_base cmd_base;
   GLshort v[3];
};
uint32_t
_mesa_unmarshal_Color3sv(struct gl_context *ctx, const struct marshal_cmd_Color3sv *restrict cmd)
{
   const GLshort *v = cmd->v;
   CALL_Color3sv(ctx->Dispatch.Current, (v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Color3sv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_Color3sv(const GLshort *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Color3sv);
   struct marshal_cmd_Color3sv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Color3sv, cmd_size);
   memcpy(cmd->v, v, 3 * sizeof(GLshort));
}


/* Color3ub: marshalled asynchronously */
struct marshal_cmd_Color3ub
{
   struct marshal_cmd_base cmd_base;
   GLubyte red;
   GLubyte green;
   GLubyte blue;
};
uint32_t
_mesa_unmarshal_Color3ub(struct gl_context *ctx, const struct marshal_cmd_Color3ub *restrict cmd)
{
   GLubyte red = cmd->red;
   GLubyte green = cmd->green;
   GLubyte blue = cmd->blue;
   CALL_Color3ub(ctx->Dispatch.Current, (red, green, blue));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Color3ub), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_Color3ub(GLubyte red, GLubyte green, GLubyte blue)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Color3ub);
   struct marshal_cmd_Color3ub *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Color3ub, cmd_size);
   cmd->red = red;
   cmd->green = green;
   cmd->blue = blue;
}


/* Color3ubv: marshalled asynchronously */
struct marshal_cmd_Color3ubv
{
   struct marshal_cmd_base cmd_base;
   GLubyte v[3];
};
uint32_t
_mesa_unmarshal_Color3ubv(struct gl_context *ctx, const struct marshal_cmd_Color3ubv *restrict cmd)
{
   const GLubyte *v = cmd->v;
   CALL_Color3ubv(ctx->Dispatch.Current, (v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Color3ubv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_Color3ubv(const GLubyte *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Color3ubv);
   struct marshal_cmd_Color3ubv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Color3ubv, cmd_size);
   memcpy(cmd->v, v, 3 * sizeof(GLubyte));
}


/* Color3ui: marshalled asynchronously */
struct marshal_cmd_Color3ui
{
   struct marshal_cmd_base cmd_base;
   GLuint red;
   GLuint green;
   GLuint blue;
};
uint32_t
_mesa_unmarshal_Color3ui(struct gl_context *ctx, const struct marshal_cmd_Color3ui *restrict cmd)
{
   GLuint red = cmd->red;
   GLuint green = cmd->green;
   GLuint blue = cmd->blue;
   CALL_Color3ui(ctx->Dispatch.Current, (red, green, blue));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Color3ui), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_Color3ui(GLuint red, GLuint green, GLuint blue)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Color3ui);
   struct marshal_cmd_Color3ui *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Color3ui, cmd_size);
   cmd->red = red;
   cmd->green = green;
   cmd->blue = blue;
}


/* Color3uiv: marshalled asynchronously */
struct marshal_cmd_Color3uiv
{
   struct marshal_cmd_base cmd_base;
   GLuint v[3];
};
uint32_t
_mesa_unmarshal_Color3uiv(struct gl_context *ctx, const struct marshal_cmd_Color3uiv *restrict cmd)
{
   const GLuint *v = cmd->v;
   CALL_Color3uiv(ctx->Dispatch.Current, (v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Color3uiv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_Color3uiv(const GLuint *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Color3uiv);
   struct marshal_cmd_Color3uiv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Color3uiv, cmd_size);
   memcpy(cmd->v, v, 3 * sizeof(GLuint));
}


/* Color3us: marshalled asynchronously */
struct marshal_cmd_Color3us
{
   struct marshal_cmd_base cmd_base;
   GLushort red;
   GLushort green;
   GLushort blue;
};
uint32_t
_mesa_unmarshal_Color3us(struct gl_context *ctx, const struct marshal_cmd_Color3us *restrict cmd)
{
   GLushort red = cmd->red;
   GLushort green = cmd->green;
   GLushort blue = cmd->blue;
   CALL_Color3us(ctx->Dispatch.Current, (red, green, blue));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Color3us), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_Color3us(GLushort red, GLushort green, GLushort blue)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Color3us);
   struct marshal_cmd_Color3us *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Color3us, cmd_size);
   cmd->red = red;
   cmd->green = green;
   cmd->blue = blue;
}


/* Color3usv: marshalled asynchronously */
struct marshal_cmd_Color3usv
{
   struct marshal_cmd_base cmd_base;
   GLushort v[3];
};
uint32_t
_mesa_unmarshal_Color3usv(struct gl_context *ctx, const struct marshal_cmd_Color3usv *restrict cmd)
{
   const GLushort *v = cmd->v;
   CALL_Color3usv(ctx->Dispatch.Current, (v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Color3usv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_Color3usv(const GLushort *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Color3usv);
   struct marshal_cmd_Color3usv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Color3usv, cmd_size);
   memcpy(cmd->v, v, 3 * sizeof(GLushort));
}


/* Color4b: marshalled asynchronously */
struct marshal_cmd_Color4b
{
   struct marshal_cmd_base cmd_base;
   GLbyte red;
   GLbyte green;
   GLbyte blue;
   GLbyte alpha;
};
uint32_t
_mesa_unmarshal_Color4b(struct gl_context *ctx, const struct marshal_cmd_Color4b *restrict cmd)
{
   GLbyte red = cmd->red;
   GLbyte green = cmd->green;
   GLbyte blue = cmd->blue;
   GLbyte alpha = cmd->alpha;
   CALL_Color4b(ctx->Dispatch.Current, (red, green, blue, alpha));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Color4b), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_Color4b(GLbyte red, GLbyte green, GLbyte blue, GLbyte alpha)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Color4b);
   struct marshal_cmd_Color4b *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Color4b, cmd_size);
   cmd->red = red;
   cmd->green = green;
   cmd->blue = blue;
   cmd->alpha = alpha;
}


/* Color4bv: marshalled asynchronously */
struct marshal_cmd_Color4bv
{
   struct marshal_cmd_base cmd_base;
   GLbyte v[4];
};
uint32_t
_mesa_unmarshal_Color4bv(struct gl_context *ctx, const struct marshal_cmd_Color4bv *restrict cmd)
{
   const GLbyte *v = cmd->v;
   CALL_Color4bv(ctx->Dispatch.Current, (v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Color4bv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_Color4bv(const GLbyte *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Color4bv);
   struct marshal_cmd_Color4bv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Color4bv, cmd_size);
   memcpy(cmd->v, v, 4 * sizeof(GLbyte));
}


/* Color4d: marshalled asynchronously */
struct marshal_cmd_Color4d
{
   struct marshal_cmd_base cmd_base;
   GLdouble red;
   GLdouble green;
   GLdouble blue;
   GLdouble alpha;
};
uint32_t
_mesa_unmarshal_Color4d(struct gl_context *ctx, const struct marshal_cmd_Color4d *restrict cmd)
{
   GLdouble red = cmd->red;
   GLdouble green = cmd->green;
   GLdouble blue = cmd->blue;
   GLdouble alpha = cmd->alpha;
   CALL_Color4d(ctx->Dispatch.Current, (red, green, blue, alpha));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Color4d), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_Color4d(GLdouble red, GLdouble green, GLdouble blue, GLdouble alpha)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Color4d);
   struct marshal_cmd_Color4d *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Color4d, cmd_size);
   cmd->red = red;
   cmd->green = green;
   cmd->blue = blue;
   cmd->alpha = alpha;
}


/* Color4dv: marshalled asynchronously */
struct marshal_cmd_Color4dv
{
   struct marshal_cmd_base cmd_base;
   GLdouble v[4];
};
uint32_t
_mesa_unmarshal_Color4dv(struct gl_context *ctx, const struct marshal_cmd_Color4dv *restrict cmd)
{
   const GLdouble *v = cmd->v;
   CALL_Color4dv(ctx->Dispatch.Current, (v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Color4dv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_Color4dv(const GLdouble *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Color4dv);
   struct marshal_cmd_Color4dv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Color4dv, cmd_size);
   memcpy(cmd->v, v, 4 * sizeof(GLdouble));
}


/* Color4f: marshalled asynchronously */
struct marshal_cmd_Color4f
{
   struct marshal_cmd_base cmd_base;
   GLfloat red;
   GLfloat green;
   GLfloat blue;
   GLfloat alpha;
};
uint32_t
_mesa_unmarshal_Color4f(struct gl_context *ctx, const struct marshal_cmd_Color4f *restrict cmd)
{
   GLfloat red = cmd->red;
   GLfloat green = cmd->green;
   GLfloat blue = cmd->blue;
   GLfloat alpha = cmd->alpha;
   CALL_Color4f(ctx->Dispatch.Current, (red, green, blue, alpha));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Color4f), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_Color4f(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Color4f);
   struct marshal_cmd_Color4f *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Color4f, cmd_size);
   cmd->red = red;
   cmd->green = green;
   cmd->blue = blue;
   cmd->alpha = alpha;
}


/* Color4fv: marshalled asynchronously */
struct marshal_cmd_Color4fv
{
   struct marshal_cmd_base cmd_base;
   GLfloat v[4];
};
uint32_t
_mesa_unmarshal_Color4fv(struct gl_context *ctx, const struct marshal_cmd_Color4fv *restrict cmd)
{
   const GLfloat *v = cmd->v;
   CALL_Color4fv(ctx->Dispatch.Current, (v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Color4fv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_Color4fv(const GLfloat *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Color4fv);
   struct marshal_cmd_Color4fv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Color4fv, cmd_size);
   memcpy(cmd->v, v, 4 * sizeof(GLfloat));
}


/* Color4i: marshalled asynchronously */
struct marshal_cmd_Color4i
{
   struct marshal_cmd_base cmd_base;
   GLint red;
   GLint green;
   GLint blue;
   GLint alpha;
};
uint32_t
_mesa_unmarshal_Color4i(struct gl_context *ctx, const struct marshal_cmd_Color4i *restrict cmd)
{
   GLint red = cmd->red;
   GLint green = cmd->green;
   GLint blue = cmd->blue;
   GLint alpha = cmd->alpha;
   CALL_Color4i(ctx->Dispatch.Current, (red, green, blue, alpha));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Color4i), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_Color4i(GLint red, GLint green, GLint blue, GLint alpha)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Color4i);
   struct marshal_cmd_Color4i *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Color4i, cmd_size);
   cmd->red = red;
   cmd->green = green;
   cmd->blue = blue;
   cmd->alpha = alpha;
}


/* Color4iv: marshalled asynchronously */
struct marshal_cmd_Color4iv
{
   struct marshal_cmd_base cmd_base;
   GLint v[4];
};
uint32_t
_mesa_unmarshal_Color4iv(struct gl_context *ctx, const struct marshal_cmd_Color4iv *restrict cmd)
{
   const GLint *v = cmd->v;
   CALL_Color4iv(ctx->Dispatch.Current, (v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Color4iv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_Color4iv(const GLint *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Color4iv);
   struct marshal_cmd_Color4iv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Color4iv, cmd_size);
   memcpy(cmd->v, v, 4 * sizeof(GLint));
}


/* Color4s: marshalled asynchronously */
struct marshal_cmd_Color4s
{
   struct marshal_cmd_base cmd_base;
   GLshort red;
   GLshort green;
   GLshort blue;
   GLshort alpha;
};
uint32_t
_mesa_unmarshal_Color4s(struct gl_context *ctx, const struct marshal_cmd_Color4s *restrict cmd)
{
   GLshort red = cmd->red;
   GLshort green = cmd->green;
   GLshort blue = cmd->blue;
   GLshort alpha = cmd->alpha;
   CALL_Color4s(ctx->Dispatch.Current, (red, green, blue, alpha));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Color4s), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_Color4s(GLshort red, GLshort green, GLshort blue, GLshort alpha)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Color4s);
   struct marshal_cmd_Color4s *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Color4s, cmd_size);
   cmd->red = red;
   cmd->green = green;
   cmd->blue = blue;
   cmd->alpha = alpha;
}


/* Color4sv: marshalled asynchronously */
struct marshal_cmd_Color4sv
{
   struct marshal_cmd_base cmd_base;
   GLshort v[4];
};
uint32_t
_mesa_unmarshal_Color4sv(struct gl_context *ctx, const struct marshal_cmd_Color4sv *restrict cmd)
{
   const GLshort *v = cmd->v;
   CALL_Color4sv(ctx->Dispatch.Current, (v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Color4sv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_Color4sv(const GLshort *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Color4sv);
   struct marshal_cmd_Color4sv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Color4sv, cmd_size);
   memcpy(cmd->v, v, 4 * sizeof(GLshort));
}


/* Color4ub: marshalled asynchronously */
struct marshal_cmd_Color4ub
{
   struct marshal_cmd_base cmd_base;
   GLubyte red;
   GLubyte green;
   GLubyte blue;
   GLubyte alpha;
};
uint32_t
_mesa_unmarshal_Color4ub(struct gl_context *ctx, const struct marshal_cmd_Color4ub *restrict cmd)
{
   GLubyte red = cmd->red;
   GLubyte green = cmd->green;
   GLubyte blue = cmd->blue;
   GLubyte alpha = cmd->alpha;
   CALL_Color4ub(ctx->Dispatch.Current, (red, green, blue, alpha));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Color4ub), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_Color4ub(GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Color4ub);
   struct marshal_cmd_Color4ub *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Color4ub, cmd_size);
   cmd->red = red;
   cmd->green = green;
   cmd->blue = blue;
   cmd->alpha = alpha;
}


/* Color4ubv: marshalled asynchronously */
struct marshal_cmd_Color4ubv
{
   struct marshal_cmd_base cmd_base;
   GLubyte v[4];
};
uint32_t
_mesa_unmarshal_Color4ubv(struct gl_context *ctx, const struct marshal_cmd_Color4ubv *restrict cmd)
{
   const GLubyte *v = cmd->v;
   CALL_Color4ubv(ctx->Dispatch.Current, (v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Color4ubv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_Color4ubv(const GLubyte *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Color4ubv);
   struct marshal_cmd_Color4ubv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Color4ubv, cmd_size);
   memcpy(cmd->v, v, 4 * sizeof(GLubyte));
}


/* Color4ui: marshalled asynchronously */
struct marshal_cmd_Color4ui
{
   struct marshal_cmd_base cmd_base;
   GLuint red;
   GLuint green;
   GLuint blue;
   GLuint alpha;
};
uint32_t
_mesa_unmarshal_Color4ui(struct gl_context *ctx, const struct marshal_cmd_Color4ui *restrict cmd)
{
   GLuint red = cmd->red;
   GLuint green = cmd->green;
   GLuint blue = cmd->blue;
   GLuint alpha = cmd->alpha;
   CALL_Color4ui(ctx->Dispatch.Current, (red, green, blue, alpha));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Color4ui), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_Color4ui(GLuint red, GLuint green, GLuint blue, GLuint alpha)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Color4ui);
   struct marshal_cmd_Color4ui *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Color4ui, cmd_size);
   cmd->red = red;
   cmd->green = green;
   cmd->blue = blue;
   cmd->alpha = alpha;
}


/* Color4uiv: marshalled asynchronously */
struct marshal_cmd_Color4uiv
{
   struct marshal_cmd_base cmd_base;
   GLuint v[4];
};
uint32_t
_mesa_unmarshal_Color4uiv(struct gl_context *ctx, const struct marshal_cmd_Color4uiv *restrict cmd)
{
   const GLuint *v = cmd->v;
   CALL_Color4uiv(ctx->Dispatch.Current, (v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Color4uiv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_Color4uiv(const GLuint *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Color4uiv);
   struct marshal_cmd_Color4uiv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Color4uiv, cmd_size);
   memcpy(cmd->v, v, 4 * sizeof(GLuint));
}


/* Color4us: marshalled asynchronously */
struct marshal_cmd_Color4us
{
   struct marshal_cmd_base cmd_base;
   GLushort red;
   GLushort green;
   GLushort blue;
   GLushort alpha;
};
uint32_t
_mesa_unmarshal_Color4us(struct gl_context *ctx, const struct marshal_cmd_Color4us *restrict cmd)
{
   GLushort red = cmd->red;
   GLushort green = cmd->green;
   GLushort blue = cmd->blue;
   GLushort alpha = cmd->alpha;
   CALL_Color4us(ctx->Dispatch.Current, (red, green, blue, alpha));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Color4us), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_Color4us(GLushort red, GLushort green, GLushort blue, GLushort alpha)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Color4us);
   struct marshal_cmd_Color4us *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Color4us, cmd_size);
   cmd->red = red;
   cmd->green = green;
   cmd->blue = blue;
   cmd->alpha = alpha;
}


/* Color4usv: marshalled asynchronously */
struct marshal_cmd_Color4usv
{
   struct marshal_cmd_base cmd_base;
   GLushort v[4];
};
uint32_t
_mesa_unmarshal_Color4usv(struct gl_context *ctx, const struct marshal_cmd_Color4usv *restrict cmd)
{
   const GLushort *v = cmd->v;
   CALL_Color4usv(ctx->Dispatch.Current, (v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Color4usv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_Color4usv(const GLushort *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Color4usv);
   struct marshal_cmd_Color4usv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Color4usv, cmd_size);
   memcpy(cmd->v, v, 4 * sizeof(GLushort));
}


/* EdgeFlag: marshalled asynchronously */
struct marshal_cmd_EdgeFlag
{
   struct marshal_cmd_base cmd_base;
   GLboolean flag;
};
uint32_t
_mesa_unmarshal_EdgeFlag(struct gl_context *ctx, const struct marshal_cmd_EdgeFlag *restrict cmd)
{
   GLboolean flag = cmd->flag;
   CALL_EdgeFlag(ctx->Dispatch.Current, (flag));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_EdgeFlag), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_EdgeFlag(GLboolean flag)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_EdgeFlag);
   struct marshal_cmd_EdgeFlag *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_EdgeFlag, cmd_size);
   cmd->flag = flag;
}


/* EdgeFlagv: marshalled asynchronously */
struct marshal_cmd_EdgeFlagv
{
   struct marshal_cmd_base cmd_base;
   GLboolean flag[1];
};
uint32_t
_mesa_unmarshal_EdgeFlagv(struct gl_context *ctx, const struct marshal_cmd_EdgeFlagv *restrict cmd)
{
   const GLboolean *flag = cmd->flag;
   CALL_EdgeFlagv(ctx->Dispatch.Current, (flag));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_EdgeFlagv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_EdgeFlagv(const GLboolean *flag)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_EdgeFlagv);
   struct marshal_cmd_EdgeFlagv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_EdgeFlagv, cmd_size);
   memcpy(cmd->flag, flag, 1 * sizeof(GLboolean));
}


/* End: marshalled asynchronously */
struct marshal_cmd_End
{
   struct marshal_cmd_base cmd_base;
};
uint32_t
_mesa_unmarshal_End(struct gl_context *ctx, const struct marshal_cmd_End *restrict cmd)
{
   CALL_End(ctx->Dispatch.Current, ());
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_End), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_End(void)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_End);
   struct marshal_cmd_End *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_End, cmd_size);
   (void) cmd;
   ctx->GLThread.inside_begin_end = false;
}


/* Indexd: marshalled asynchronously */
struct marshal_cmd_Indexd
{
   struct marshal_cmd_base cmd_base;
   GLdouble c;
};
uint32_t
_mesa_unmarshal_Indexd(struct gl_context *ctx, const struct marshal_cmd_Indexd *restrict cmd)
{
   GLdouble c = cmd->c;
   CALL_Indexd(ctx->Dispatch.Current, (c));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Indexd), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_Indexd(GLdouble c)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Indexd);
   struct marshal_cmd_Indexd *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Indexd, cmd_size);
   cmd->c = c;
}


/* Indexdv: marshalled asynchronously */
struct marshal_cmd_Indexdv
{
   struct marshal_cmd_base cmd_base;
   GLdouble c[1];
};
uint32_t
_mesa_unmarshal_Indexdv(struct gl_context *ctx, const struct marshal_cmd_Indexdv *restrict cmd)
{
   const GLdouble *c = cmd->c;
   CALL_Indexdv(ctx->Dispatch.Current, (c));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Indexdv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_Indexdv(const GLdouble *c)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Indexdv);
   struct marshal_cmd_Indexdv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Indexdv, cmd_size);
   memcpy(cmd->c, c, 1 * sizeof(GLdouble));
}


/* Indexf: marshalled asynchronously */
struct marshal_cmd_Indexf
{
   struct marshal_cmd_base cmd_base;
   GLfloat c;
};
uint32_t
_mesa_unmarshal_Indexf(struct gl_context *ctx, const struct marshal_cmd_Indexf *restrict cmd)
{
   GLfloat c = cmd->c;
   CALL_Indexf(ctx->Dispatch.Current, (c));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Indexf), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_Indexf(GLfloat c)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Indexf);
   struct marshal_cmd_Indexf *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Indexf, cmd_size);
   cmd->c = c;
}


/* Indexfv: marshalled asynchronously */
struct marshal_cmd_Indexfv
{
   struct marshal_cmd_base cmd_base;
   GLfloat c[1];
};
uint32_t
_mesa_unmarshal_Indexfv(struct gl_context *ctx, const struct marshal_cmd_Indexfv *restrict cmd)
{
   const GLfloat *c = cmd->c;
   CALL_Indexfv(ctx->Dispatch.Current, (c));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Indexfv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_Indexfv(const GLfloat *c)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Indexfv);
   struct marshal_cmd_Indexfv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Indexfv, cmd_size);
   memcpy(cmd->c, c, 1 * sizeof(GLfloat));
}


/* Indexi: marshalled asynchronously */
struct marshal_cmd_Indexi
{
   struct marshal_cmd_base cmd_base;
   GLint c;
};
uint32_t
_mesa_unmarshal_Indexi(struct gl_context *ctx, const struct marshal_cmd_Indexi *restrict cmd)
{
   GLint c = cmd->c;
   CALL_Indexi(ctx->Dispatch.Current, (c));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Indexi), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_Indexi(GLint c)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Indexi);
   struct marshal_cmd_Indexi *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Indexi, cmd_size);
   cmd->c = c;
}


/* Indexiv: marshalled asynchronously */
struct marshal_cmd_Indexiv
{
   struct marshal_cmd_base cmd_base;
   GLint c[1];
};
uint32_t
_mesa_unmarshal_Indexiv(struct gl_context *ctx, const struct marshal_cmd_Indexiv *restrict cmd)
{
   const GLint *c = cmd->c;
   CALL_Indexiv(ctx->Dispatch.Current, (c));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Indexiv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_Indexiv(const GLint *c)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Indexiv);
   struct marshal_cmd_Indexiv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Indexiv, cmd_size);
   memcpy(cmd->c, c, 1 * sizeof(GLint));
}


/* Indexs: marshalled asynchronously */
struct marshal_cmd_Indexs
{
   struct marshal_cmd_base cmd_base;
   GLshort c;
};
uint32_t
_mesa_unmarshal_Indexs(struct gl_context *ctx, const struct marshal_cmd_Indexs *restrict cmd)
{
   GLshort c = cmd->c;
   CALL_Indexs(ctx->Dispatch.Current, (c));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Indexs), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_Indexs(GLshort c)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Indexs);
   struct marshal_cmd_Indexs *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Indexs, cmd_size);
   cmd->c = c;
}


/* Indexsv: marshalled asynchronously */
struct marshal_cmd_Indexsv
{
   struct marshal_cmd_base cmd_base;
   GLshort c[1];
};
uint32_t
_mesa_unmarshal_Indexsv(struct gl_context *ctx, const struct marshal_cmd_Indexsv *restrict cmd)
{
   const GLshort *c = cmd->c;
   CALL_Indexsv(ctx->Dispatch.Current, (c));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Indexsv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_Indexsv(const GLshort *c)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Indexsv);
   struct marshal_cmd_Indexsv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Indexsv, cmd_size);
   memcpy(cmd->c, c, 1 * sizeof(GLshort));
}


/* Normal3b: marshalled asynchronously */
struct marshal_cmd_Normal3b
{
   struct marshal_cmd_base cmd_base;
   GLbyte nx;
   GLbyte ny;
   GLbyte nz;
};
uint32_t
_mesa_unmarshal_Normal3b(struct gl_context *ctx, const struct marshal_cmd_Normal3b *restrict cmd)
{
   GLbyte nx = cmd->nx;
   GLbyte ny = cmd->ny;
   GLbyte nz = cmd->nz;
   CALL_Normal3b(ctx->Dispatch.Current, (nx, ny, nz));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Normal3b), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_Normal3b(GLbyte nx, GLbyte ny, GLbyte nz)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Normal3b);
   struct marshal_cmd_Normal3b *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Normal3b, cmd_size);
   cmd->nx = nx;
   cmd->ny = ny;
   cmd->nz = nz;
}


/* Normal3bv: marshalled asynchronously */
struct marshal_cmd_Normal3bv
{
   struct marshal_cmd_base cmd_base;
   GLbyte v[3];
};
uint32_t
_mesa_unmarshal_Normal3bv(struct gl_context *ctx, const struct marshal_cmd_Normal3bv *restrict cmd)
{
   const GLbyte *v = cmd->v;
   CALL_Normal3bv(ctx->Dispatch.Current, (v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Normal3bv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_Normal3bv(const GLbyte *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Normal3bv);
   struct marshal_cmd_Normal3bv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Normal3bv, cmd_size);
   memcpy(cmd->v, v, 3 * sizeof(GLbyte));
}


/* Normal3d: marshalled asynchronously */
struct marshal_cmd_Normal3d
{
   struct marshal_cmd_base cmd_base;
   GLdouble nx;
   GLdouble ny;
   GLdouble nz;
};
uint32_t
_mesa_unmarshal_Normal3d(struct gl_context *ctx, const struct marshal_cmd_Normal3d *restrict cmd)
{
   GLdouble nx = cmd->nx;
   GLdouble ny = cmd->ny;
   GLdouble nz = cmd->nz;
   CALL_Normal3d(ctx->Dispatch.Current, (nx, ny, nz));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Normal3d), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_Normal3d(GLdouble nx, GLdouble ny, GLdouble nz)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Normal3d);
   struct marshal_cmd_Normal3d *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Normal3d, cmd_size);
   cmd->nx = nx;
   cmd->ny = ny;
   cmd->nz = nz;
}


/* Normal3dv: marshalled asynchronously */
struct marshal_cmd_Normal3dv
{
   struct marshal_cmd_base cmd_base;
   GLdouble v[3];
};
uint32_t
_mesa_unmarshal_Normal3dv(struct gl_context *ctx, const struct marshal_cmd_Normal3dv *restrict cmd)
{
   const GLdouble *v = cmd->v;
   CALL_Normal3dv(ctx->Dispatch.Current, (v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Normal3dv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_Normal3dv(const GLdouble *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Normal3dv);
   struct marshal_cmd_Normal3dv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Normal3dv, cmd_size);
   memcpy(cmd->v, v, 3 * sizeof(GLdouble));
}


/* Normal3f: marshalled asynchronously */
struct marshal_cmd_Normal3f
{
   struct marshal_cmd_base cmd_base;
   GLfloat nx;
   GLfloat ny;
   GLfloat nz;
};
uint32_t
_mesa_unmarshal_Normal3f(struct gl_context *ctx, const struct marshal_cmd_Normal3f *restrict cmd)
{
   GLfloat nx = cmd->nx;
   GLfloat ny = cmd->ny;
   GLfloat nz = cmd->nz;
   CALL_Normal3f(ctx->Dispatch.Current, (nx, ny, nz));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Normal3f), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_Normal3f(GLfloat nx, GLfloat ny, GLfloat nz)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Normal3f);
   struct marshal_cmd_Normal3f *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Normal3f, cmd_size);
   cmd->nx = nx;
   cmd->ny = ny;
   cmd->nz = nz;
}


/* Normal3fv: marshalled asynchronously */
struct marshal_cmd_Normal3fv
{
   struct marshal_cmd_base cmd_base;
   GLfloat v[3];
};
uint32_t
_mesa_unmarshal_Normal3fv(struct gl_context *ctx, const struct marshal_cmd_Normal3fv *restrict cmd)
{
   const GLfloat *v = cmd->v;
   CALL_Normal3fv(ctx->Dispatch.Current, (v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Normal3fv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_Normal3fv(const GLfloat *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Normal3fv);
   struct marshal_cmd_Normal3fv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Normal3fv, cmd_size);
   memcpy(cmd->v, v, 3 * sizeof(GLfloat));
}


/* Normal3i: marshalled asynchronously */
struct marshal_cmd_Normal3i
{
   struct marshal_cmd_base cmd_base;
   GLint nx;
   GLint ny;
   GLint nz;
};
uint32_t
_mesa_unmarshal_Normal3i(struct gl_context *ctx, const struct marshal_cmd_Normal3i *restrict cmd)
{
   GLint nx = cmd->nx;
   GLint ny = cmd->ny;
   GLint nz = cmd->nz;
   CALL_Normal3i(ctx->Dispatch.Current, (nx, ny, nz));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Normal3i), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_Normal3i(GLint nx, GLint ny, GLint nz)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Normal3i);
   struct marshal_cmd_Normal3i *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Normal3i, cmd_size);
   cmd->nx = nx;
   cmd->ny = ny;
   cmd->nz = nz;
}


/* Normal3iv: marshalled asynchronously */
struct marshal_cmd_Normal3iv
{
   struct marshal_cmd_base cmd_base;
   GLint v[3];
};
uint32_t
_mesa_unmarshal_Normal3iv(struct gl_context *ctx, const struct marshal_cmd_Normal3iv *restrict cmd)
{
   const GLint *v = cmd->v;
   CALL_Normal3iv(ctx->Dispatch.Current, (v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Normal3iv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_Normal3iv(const GLint *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Normal3iv);
   struct marshal_cmd_Normal3iv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Normal3iv, cmd_size);
   memcpy(cmd->v, v, 3 * sizeof(GLint));
}


/* Normal3s: marshalled asynchronously */
struct marshal_cmd_Normal3s
{
   struct marshal_cmd_base cmd_base;
   GLshort nx;
   GLshort ny;
   GLshort nz;
};
uint32_t
_mesa_unmarshal_Normal3s(struct gl_context *ctx, const struct marshal_cmd_Normal3s *restrict cmd)
{
   GLshort nx = cmd->nx;
   GLshort ny = cmd->ny;
   GLshort nz = cmd->nz;
   CALL_Normal3s(ctx->Dispatch.Current, (nx, ny, nz));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Normal3s), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_Normal3s(GLshort nx, GLshort ny, GLshort nz)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Normal3s);
   struct marshal_cmd_Normal3s *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Normal3s, cmd_size);
   cmd->nx = nx;
   cmd->ny = ny;
   cmd->nz = nz;
}


/* Normal3sv: marshalled asynchronously */
struct marshal_cmd_Normal3sv
{
   struct marshal_cmd_base cmd_base;
   GLshort v[3];
};
uint32_t
_mesa_unmarshal_Normal3sv(struct gl_context *ctx, const struct marshal_cmd_Normal3sv *restrict cmd)
{
   const GLshort *v = cmd->v;
   CALL_Normal3sv(ctx->Dispatch.Current, (v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Normal3sv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_Normal3sv(const GLshort *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Normal3sv);
   struct marshal_cmd_Normal3sv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Normal3sv, cmd_size);
   memcpy(cmd->v, v, 3 * sizeof(GLshort));
}


/* RasterPos2d: marshalled asynchronously */
struct marshal_cmd_RasterPos2d
{
   struct marshal_cmd_base cmd_base;
   GLdouble x;
   GLdouble y;
};
uint32_t
_mesa_unmarshal_RasterPos2d(struct gl_context *ctx, const struct marshal_cmd_RasterPos2d *restrict cmd)
{
   GLdouble x = cmd->x;
   GLdouble y = cmd->y;
   CALL_RasterPos2d(ctx->Dispatch.Current, (x, y));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_RasterPos2d), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_RasterPos2d(GLdouble x, GLdouble y)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_RasterPos2d);
   struct marshal_cmd_RasterPos2d *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_RasterPos2d, cmd_size);
   cmd->x = x;
   cmd->y = y;
}


/* RasterPos2dv: marshalled asynchronously */
struct marshal_cmd_RasterPos2dv
{
   struct marshal_cmd_base cmd_base;
   GLdouble v[2];
};
uint32_t
_mesa_unmarshal_RasterPos2dv(struct gl_context *ctx, const struct marshal_cmd_RasterPos2dv *restrict cmd)
{
   const GLdouble *v = cmd->v;
   CALL_RasterPos2dv(ctx->Dispatch.Current, (v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_RasterPos2dv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_RasterPos2dv(const GLdouble *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_RasterPos2dv);
   struct marshal_cmd_RasterPos2dv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_RasterPos2dv, cmd_size);
   memcpy(cmd->v, v, 2 * sizeof(GLdouble));
}


/* RasterPos2f: marshalled asynchronously */
struct marshal_cmd_RasterPos2f
{
   struct marshal_cmd_base cmd_base;
   GLfloat x;
   GLfloat y;
};
uint32_t
_mesa_unmarshal_RasterPos2f(struct gl_context *ctx, const struct marshal_cmd_RasterPos2f *restrict cmd)
{
   GLfloat x = cmd->x;
   GLfloat y = cmd->y;
   CALL_RasterPos2f(ctx->Dispatch.Current, (x, y));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_RasterPos2f), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_RasterPos2f(GLfloat x, GLfloat y)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_RasterPos2f);
   struct marshal_cmd_RasterPos2f *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_RasterPos2f, cmd_size);
   cmd->x = x;
   cmd->y = y;
}


/* RasterPos2fv: marshalled asynchronously */
struct marshal_cmd_RasterPos2fv
{
   struct marshal_cmd_base cmd_base;
   GLfloat v[2];
};
uint32_t
_mesa_unmarshal_RasterPos2fv(struct gl_context *ctx, const struct marshal_cmd_RasterPos2fv *restrict cmd)
{
   const GLfloat *v = cmd->v;
   CALL_RasterPos2fv(ctx->Dispatch.Current, (v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_RasterPos2fv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_RasterPos2fv(const GLfloat *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_RasterPos2fv);
   struct marshal_cmd_RasterPos2fv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_RasterPos2fv, cmd_size);
   memcpy(cmd->v, v, 2 * sizeof(GLfloat));
}


/* RasterPos2i: marshalled asynchronously */
struct marshal_cmd_RasterPos2i
{
   struct marshal_cmd_base cmd_base;
   GLint x;
   GLint y;
};
uint32_t
_mesa_unmarshal_RasterPos2i(struct gl_context *ctx, const struct marshal_cmd_RasterPos2i *restrict cmd)
{
   GLint x = cmd->x;
   GLint y = cmd->y;
   CALL_RasterPos2i(ctx->Dispatch.Current, (x, y));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_RasterPos2i), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_RasterPos2i(GLint x, GLint y)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_RasterPos2i);
   struct marshal_cmd_RasterPos2i *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_RasterPos2i, cmd_size);
   cmd->x = x;
   cmd->y = y;
}


/* RasterPos2iv: marshalled asynchronously */
struct marshal_cmd_RasterPos2iv
{
   struct marshal_cmd_base cmd_base;
   GLint v[2];
};
uint32_t
_mesa_unmarshal_RasterPos2iv(struct gl_context *ctx, const struct marshal_cmd_RasterPos2iv *restrict cmd)
{
   const GLint *v = cmd->v;
   CALL_RasterPos2iv(ctx->Dispatch.Current, (v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_RasterPos2iv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_RasterPos2iv(const GLint *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_RasterPos2iv);
   struct marshal_cmd_RasterPos2iv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_RasterPos2iv, cmd_size);
   memcpy(cmd->v, v, 2 * sizeof(GLint));
}


/* RasterPos2s: marshalled asynchronously */
struct marshal_cmd_RasterPos2s
{
   struct marshal_cmd_base cmd_base;
   GLshort x;
   GLshort y;
};
uint32_t
_mesa_unmarshal_RasterPos2s(struct gl_context *ctx, const struct marshal_cmd_RasterPos2s *restrict cmd)
{
   GLshort x = cmd->x;
   GLshort y = cmd->y;
   CALL_RasterPos2s(ctx->Dispatch.Current, (x, y));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_RasterPos2s), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_RasterPos2s(GLshort x, GLshort y)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_RasterPos2s);
   struct marshal_cmd_RasterPos2s *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_RasterPos2s, cmd_size);
   cmd->x = x;
   cmd->y = y;
}


/* RasterPos2sv: marshalled asynchronously */
struct marshal_cmd_RasterPos2sv
{
   struct marshal_cmd_base cmd_base;
   GLshort v[2];
};
uint32_t
_mesa_unmarshal_RasterPos2sv(struct gl_context *ctx, const struct marshal_cmd_RasterPos2sv *restrict cmd)
{
   const GLshort *v = cmd->v;
   CALL_RasterPos2sv(ctx->Dispatch.Current, (v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_RasterPos2sv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_RasterPos2sv(const GLshort *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_RasterPos2sv);
   struct marshal_cmd_RasterPos2sv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_RasterPos2sv, cmd_size);
   memcpy(cmd->v, v, 2 * sizeof(GLshort));
}


/* RasterPos3d: marshalled asynchronously */
struct marshal_cmd_RasterPos3d
{
   struct marshal_cmd_base cmd_base;
   GLdouble x;
   GLdouble y;
   GLdouble z;
};
uint32_t
_mesa_unmarshal_RasterPos3d(struct gl_context *ctx, const struct marshal_cmd_RasterPos3d *restrict cmd)
{
   GLdouble x = cmd->x;
   GLdouble y = cmd->y;
   GLdouble z = cmd->z;
   CALL_RasterPos3d(ctx->Dispatch.Current, (x, y, z));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_RasterPos3d), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_RasterPos3d(GLdouble x, GLdouble y, GLdouble z)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_RasterPos3d);
   struct marshal_cmd_RasterPos3d *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_RasterPos3d, cmd_size);
   cmd->x = x;
   cmd->y = y;
   cmd->z = z;
}


/* RasterPos3dv: marshalled asynchronously */
struct marshal_cmd_RasterPos3dv
{
   struct marshal_cmd_base cmd_base;
   GLdouble v[3];
};
uint32_t
_mesa_unmarshal_RasterPos3dv(struct gl_context *ctx, const struct marshal_cmd_RasterPos3dv *restrict cmd)
{
   const GLdouble *v = cmd->v;
   CALL_RasterPos3dv(ctx->Dispatch.Current, (v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_RasterPos3dv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_RasterPos3dv(const GLdouble *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_RasterPos3dv);
   struct marshal_cmd_RasterPos3dv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_RasterPos3dv, cmd_size);
   memcpy(cmd->v, v, 3 * sizeof(GLdouble));
}


/* RasterPos3f: marshalled asynchronously */
struct marshal_cmd_RasterPos3f
{
   struct marshal_cmd_base cmd_base;
   GLfloat x;
   GLfloat y;
   GLfloat z;
};
uint32_t
_mesa_unmarshal_RasterPos3f(struct gl_context *ctx, const struct marshal_cmd_RasterPos3f *restrict cmd)
{
   GLfloat x = cmd->x;
   GLfloat y = cmd->y;
   GLfloat z = cmd->z;
   CALL_RasterPos3f(ctx->Dispatch.Current, (x, y, z));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_RasterPos3f), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_RasterPos3f(GLfloat x, GLfloat y, GLfloat z)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_RasterPos3f);
   struct marshal_cmd_RasterPos3f *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_RasterPos3f, cmd_size);
   cmd->x = x;
   cmd->y = y;
   cmd->z = z;
}


/* RasterPos3fv: marshalled asynchronously */
struct marshal_cmd_RasterPos3fv
{
   struct marshal_cmd_base cmd_base;
   GLfloat v[3];
};
uint32_t
_mesa_unmarshal_RasterPos3fv(struct gl_context *ctx, const struct marshal_cmd_RasterPos3fv *restrict cmd)
{
   const GLfloat *v = cmd->v;
   CALL_RasterPos3fv(ctx->Dispatch.Current, (v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_RasterPos3fv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_RasterPos3fv(const GLfloat *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_RasterPos3fv);
   struct marshal_cmd_RasterPos3fv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_RasterPos3fv, cmd_size);
   memcpy(cmd->v, v, 3 * sizeof(GLfloat));
}


/* RasterPos3i: marshalled asynchronously */
struct marshal_cmd_RasterPos3i
{
   struct marshal_cmd_base cmd_base;
   GLint x;
   GLint y;
   GLint z;
};
uint32_t
_mesa_unmarshal_RasterPos3i(struct gl_context *ctx, const struct marshal_cmd_RasterPos3i *restrict cmd)
{
   GLint x = cmd->x;
   GLint y = cmd->y;
   GLint z = cmd->z;
   CALL_RasterPos3i(ctx->Dispatch.Current, (x, y, z));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_RasterPos3i), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_RasterPos3i(GLint x, GLint y, GLint z)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_RasterPos3i);
   struct marshal_cmd_RasterPos3i *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_RasterPos3i, cmd_size);
   cmd->x = x;
   cmd->y = y;
   cmd->z = z;
}


/* RasterPos3iv: marshalled asynchronously */
struct marshal_cmd_RasterPos3iv
{
   struct marshal_cmd_base cmd_base;
   GLint v[3];
};
uint32_t
_mesa_unmarshal_RasterPos3iv(struct gl_context *ctx, const struct marshal_cmd_RasterPos3iv *restrict cmd)
{
   const GLint *v = cmd->v;
   CALL_RasterPos3iv(ctx->Dispatch.Current, (v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_RasterPos3iv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_RasterPos3iv(const GLint *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_RasterPos3iv);
   struct marshal_cmd_RasterPos3iv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_RasterPos3iv, cmd_size);
   memcpy(cmd->v, v, 3 * sizeof(GLint));
}


/* RasterPos3s: marshalled asynchronously */
struct marshal_cmd_RasterPos3s
{
   struct marshal_cmd_base cmd_base;
   GLshort x;
   GLshort y;
   GLshort z;
};
uint32_t
_mesa_unmarshal_RasterPos3s(struct gl_context *ctx, const struct marshal_cmd_RasterPos3s *restrict cmd)
{
   GLshort x = cmd->x;
   GLshort y = cmd->y;
   GLshort z = cmd->z;
   CALL_RasterPos3s(ctx->Dispatch.Current, (x, y, z));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_RasterPos3s), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_RasterPos3s(GLshort x, GLshort y, GLshort z)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_RasterPos3s);
   struct marshal_cmd_RasterPos3s *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_RasterPos3s, cmd_size);
   cmd->x = x;
   cmd->y = y;
   cmd->z = z;
}


/* RasterPos3sv: marshalled asynchronously */
struct marshal_cmd_RasterPos3sv
{
   struct marshal_cmd_base cmd_base;
   GLshort v[3];
};
uint32_t
_mesa_unmarshal_RasterPos3sv(struct gl_context *ctx, const struct marshal_cmd_RasterPos3sv *restrict cmd)
{
   const GLshort *v = cmd->v;
   CALL_RasterPos3sv(ctx->Dispatch.Current, (v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_RasterPos3sv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_RasterPos3sv(const GLshort *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_RasterPos3sv);
   struct marshal_cmd_RasterPos3sv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_RasterPos3sv, cmd_size);
   memcpy(cmd->v, v, 3 * sizeof(GLshort));
}


/* RasterPos4d: marshalled asynchronously */
struct marshal_cmd_RasterPos4d
{
   struct marshal_cmd_base cmd_base;
   GLdouble x;
   GLdouble y;
   GLdouble z;
   GLdouble w;
};
uint32_t
_mesa_unmarshal_RasterPos4d(struct gl_context *ctx, const struct marshal_cmd_RasterPos4d *restrict cmd)
{
   GLdouble x = cmd->x;
   GLdouble y = cmd->y;
   GLdouble z = cmd->z;
   GLdouble w = cmd->w;
   CALL_RasterPos4d(ctx->Dispatch.Current, (x, y, z, w));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_RasterPos4d), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_RasterPos4d(GLdouble x, GLdouble y, GLdouble z, GLdouble w)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_RasterPos4d);
   struct marshal_cmd_RasterPos4d *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_RasterPos4d, cmd_size);
   cmd->x = x;
   cmd->y = y;
   cmd->z = z;
   cmd->w = w;
}


/* RasterPos4dv: marshalled asynchronously */
struct marshal_cmd_RasterPos4dv
{
   struct marshal_cmd_base cmd_base;
   GLdouble v[4];
};
uint32_t
_mesa_unmarshal_RasterPos4dv(struct gl_context *ctx, const struct marshal_cmd_RasterPos4dv *restrict cmd)
{
   const GLdouble *v = cmd->v;
   CALL_RasterPos4dv(ctx->Dispatch.Current, (v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_RasterPos4dv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_RasterPos4dv(const GLdouble *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_RasterPos4dv);
   struct marshal_cmd_RasterPos4dv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_RasterPos4dv, cmd_size);
   memcpy(cmd->v, v, 4 * sizeof(GLdouble));
}


/* RasterPos4f: marshalled asynchronously */
struct marshal_cmd_RasterPos4f
{
   struct marshal_cmd_base cmd_base;
   GLfloat x;
   GLfloat y;
   GLfloat z;
   GLfloat w;
};
uint32_t
_mesa_unmarshal_RasterPos4f(struct gl_context *ctx, const struct marshal_cmd_RasterPos4f *restrict cmd)
{
   GLfloat x = cmd->x;
   GLfloat y = cmd->y;
   GLfloat z = cmd->z;
   GLfloat w = cmd->w;
   CALL_RasterPos4f(ctx->Dispatch.Current, (x, y, z, w));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_RasterPos4f), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_RasterPos4f(GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_RasterPos4f);
   struct marshal_cmd_RasterPos4f *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_RasterPos4f, cmd_size);
   cmd->x = x;
   cmd->y = y;
   cmd->z = z;
   cmd->w = w;
}


/* RasterPos4fv: marshalled asynchronously */
struct marshal_cmd_RasterPos4fv
{
   struct marshal_cmd_base cmd_base;
   GLfloat v[4];
};
uint32_t
_mesa_unmarshal_RasterPos4fv(struct gl_context *ctx, const struct marshal_cmd_RasterPos4fv *restrict cmd)
{
   const GLfloat *v = cmd->v;
   CALL_RasterPos4fv(ctx->Dispatch.Current, (v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_RasterPos4fv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_RasterPos4fv(const GLfloat *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_RasterPos4fv);
   struct marshal_cmd_RasterPos4fv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_RasterPos4fv, cmd_size);
   memcpy(cmd->v, v, 4 * sizeof(GLfloat));
}


/* RasterPos4i: marshalled asynchronously */
struct marshal_cmd_RasterPos4i
{
   struct marshal_cmd_base cmd_base;
   GLint x;
   GLint y;
   GLint z;
   GLint w;
};
uint32_t
_mesa_unmarshal_RasterPos4i(struct gl_context *ctx, const struct marshal_cmd_RasterPos4i *restrict cmd)
{
   GLint x = cmd->x;
   GLint y = cmd->y;
   GLint z = cmd->z;
   GLint w = cmd->w;
   CALL_RasterPos4i(ctx->Dispatch.Current, (x, y, z, w));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_RasterPos4i), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_RasterPos4i(GLint x, GLint y, GLint z, GLint w)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_RasterPos4i);
   struct marshal_cmd_RasterPos4i *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_RasterPos4i, cmd_size);
   cmd->x = x;
   cmd->y = y;
   cmd->z = z;
   cmd->w = w;
}


/* RasterPos4iv: marshalled asynchronously */
struct marshal_cmd_RasterPos4iv
{
   struct marshal_cmd_base cmd_base;
   GLint v[4];
};
uint32_t
_mesa_unmarshal_RasterPos4iv(struct gl_context *ctx, const struct marshal_cmd_RasterPos4iv *restrict cmd)
{
   const GLint *v = cmd->v;
   CALL_RasterPos4iv(ctx->Dispatch.Current, (v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_RasterPos4iv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_RasterPos4iv(const GLint *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_RasterPos4iv);
   struct marshal_cmd_RasterPos4iv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_RasterPos4iv, cmd_size);
   memcpy(cmd->v, v, 4 * sizeof(GLint));
}


/* RasterPos4s: marshalled asynchronously */
struct marshal_cmd_RasterPos4s
{
   struct marshal_cmd_base cmd_base;
   GLshort x;
   GLshort y;
   GLshort z;
   GLshort w;
};
uint32_t
_mesa_unmarshal_RasterPos4s(struct gl_context *ctx, const struct marshal_cmd_RasterPos4s *restrict cmd)
{
   GLshort x = cmd->x;
   GLshort y = cmd->y;
   GLshort z = cmd->z;
   GLshort w = cmd->w;
   CALL_RasterPos4s(ctx->Dispatch.Current, (x, y, z, w));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_RasterPos4s), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_RasterPos4s(GLshort x, GLshort y, GLshort z, GLshort w)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_RasterPos4s);
   struct marshal_cmd_RasterPos4s *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_RasterPos4s, cmd_size);
   cmd->x = x;
   cmd->y = y;
   cmd->z = z;
   cmd->w = w;
}


/* RasterPos4sv: marshalled asynchronously */
struct marshal_cmd_RasterPos4sv
{
   struct marshal_cmd_base cmd_base;
   GLshort v[4];
};
uint32_t
_mesa_unmarshal_RasterPos4sv(struct gl_context *ctx, const struct marshal_cmd_RasterPos4sv *restrict cmd)
{
   const GLshort *v = cmd->v;
   CALL_RasterPos4sv(ctx->Dispatch.Current, (v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_RasterPos4sv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_RasterPos4sv(const GLshort *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_RasterPos4sv);
   struct marshal_cmd_RasterPos4sv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_RasterPos4sv, cmd_size);
   memcpy(cmd->v, v, 4 * sizeof(GLshort));
}


/* Rectd: marshalled asynchronously */
struct marshal_cmd_Rectd
{
   struct marshal_cmd_base cmd_base;
   GLdouble x1;
   GLdouble y1;
   GLdouble x2;
   GLdouble y2;
};
uint32_t
_mesa_unmarshal_Rectd(struct gl_context *ctx, const struct marshal_cmd_Rectd *restrict cmd)
{
   GLdouble x1 = cmd->x1;
   GLdouble y1 = cmd->y1;
   GLdouble x2 = cmd->x2;
   GLdouble y2 = cmd->y2;
   CALL_Rectd(ctx->Dispatch.Current, (x1, y1, x2, y2));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Rectd), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_Rectd(GLdouble x1, GLdouble y1, GLdouble x2, GLdouble y2)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Rectd);
   struct marshal_cmd_Rectd *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Rectd, cmd_size);
   cmd->x1 = x1;
   cmd->y1 = y1;
   cmd->x2 = x2;
   cmd->y2 = y2;
}


/* Rectdv: marshalled asynchronously */
struct marshal_cmd_Rectdv
{
   struct marshal_cmd_base cmd_base;
   GLdouble v1[2];
   GLdouble v2[2];
};
uint32_t
_mesa_unmarshal_Rectdv(struct gl_context *ctx, const struct marshal_cmd_Rectdv *restrict cmd)
{
   const GLdouble *v1 = cmd->v1;
   const GLdouble *v2 = cmd->v2;
   CALL_Rectdv(ctx->Dispatch.Current, (v1, v2));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Rectdv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_Rectdv(const GLdouble *v1, const GLdouble *v2)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Rectdv);
   struct marshal_cmd_Rectdv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Rectdv, cmd_size);
   memcpy(cmd->v1, v1, 2 * sizeof(GLdouble));
   memcpy(cmd->v2, v2, 2 * sizeof(GLdouble));
}


/* Rectf: marshalled asynchronously */
struct marshal_cmd_Rectf
{
   struct marshal_cmd_base cmd_base;
   GLfloat x1;
   GLfloat y1;
   GLfloat x2;
   GLfloat y2;
};
uint32_t
_mesa_unmarshal_Rectf(struct gl_context *ctx, const struct marshal_cmd_Rectf *restrict cmd)
{
   GLfloat x1 = cmd->x1;
   GLfloat y1 = cmd->y1;
   GLfloat x2 = cmd->x2;
   GLfloat y2 = cmd->y2;
   CALL_Rectf(ctx->Dispatch.Current, (x1, y1, x2, y2));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Rectf), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_Rectf(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Rectf);
   struct marshal_cmd_Rectf *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Rectf, cmd_size);
   cmd->x1 = x1;
   cmd->y1 = y1;
   cmd->x2 = x2;
   cmd->y2 = y2;
}


/* Rectfv: marshalled asynchronously */
struct marshal_cmd_Rectfv
{
   struct marshal_cmd_base cmd_base;
   GLfloat v1[2];
   GLfloat v2[2];
};
uint32_t
_mesa_unmarshal_Rectfv(struct gl_context *ctx, const struct marshal_cmd_Rectfv *restrict cmd)
{
   const GLfloat *v1 = cmd->v1;
   const GLfloat *v2 = cmd->v2;
   CALL_Rectfv(ctx->Dispatch.Current, (v1, v2));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Rectfv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_Rectfv(const GLfloat *v1, const GLfloat *v2)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Rectfv);
   struct marshal_cmd_Rectfv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Rectfv, cmd_size);
   memcpy(cmd->v1, v1, 2 * sizeof(GLfloat));
   memcpy(cmd->v2, v2, 2 * sizeof(GLfloat));
}


/* Recti: marshalled asynchronously */
struct marshal_cmd_Recti
{
   struct marshal_cmd_base cmd_base;
   GLint x1;
   GLint y1;
   GLint x2;
   GLint y2;
};
uint32_t
_mesa_unmarshal_Recti(struct gl_context *ctx, const struct marshal_cmd_Recti *restrict cmd)
{
   GLint x1 = cmd->x1;
   GLint y1 = cmd->y1;
   GLint x2 = cmd->x2;
   GLint y2 = cmd->y2;
   CALL_Recti(ctx->Dispatch.Current, (x1, y1, x2, y2));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Recti), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_Recti(GLint x1, GLint y1, GLint x2, GLint y2)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Recti);
   struct marshal_cmd_Recti *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Recti, cmd_size);
   cmd->x1 = x1;
   cmd->y1 = y1;
   cmd->x2 = x2;
   cmd->y2 = y2;
}


/* Rectiv: marshalled asynchronously */
struct marshal_cmd_Rectiv
{
   struct marshal_cmd_base cmd_base;
   GLint v1[2];
   GLint v2[2];
};
uint32_t
_mesa_unmarshal_Rectiv(struct gl_context *ctx, const struct marshal_cmd_Rectiv *restrict cmd)
{
   const GLint *v1 = cmd->v1;
   const GLint *v2 = cmd->v2;
   CALL_Rectiv(ctx->Dispatch.Current, (v1, v2));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Rectiv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_Rectiv(const GLint *v1, const GLint *v2)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Rectiv);
   struct marshal_cmd_Rectiv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Rectiv, cmd_size);
   memcpy(cmd->v1, v1, 2 * sizeof(GLint));
   memcpy(cmd->v2, v2, 2 * sizeof(GLint));
}


/* Rects: marshalled asynchronously */
struct marshal_cmd_Rects
{
   struct marshal_cmd_base cmd_base;
   GLshort x1;
   GLshort y1;
   GLshort x2;
   GLshort y2;
};
uint32_t
_mesa_unmarshal_Rects(struct gl_context *ctx, const struct marshal_cmd_Rects *restrict cmd)
{
   GLshort x1 = cmd->x1;
   GLshort y1 = cmd->y1;
   GLshort x2 = cmd->x2;
   GLshort y2 = cmd->y2;
   CALL_Rects(ctx->Dispatch.Current, (x1, y1, x2, y2));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Rects), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_Rects(GLshort x1, GLshort y1, GLshort x2, GLshort y2)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Rects);
   struct marshal_cmd_Rects *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Rects, cmd_size);
   cmd->x1 = x1;
   cmd->y1 = y1;
   cmd->x2 = x2;
   cmd->y2 = y2;
}


/* Rectsv: marshalled asynchronously */
struct marshal_cmd_Rectsv
{
   struct marshal_cmd_base cmd_base;
   GLshort v1[2];
   GLshort v2[2];
};
uint32_t
_mesa_unmarshal_Rectsv(struct gl_context *ctx, const struct marshal_cmd_Rectsv *restrict cmd)
{
   const GLshort *v1 = cmd->v1;
   const GLshort *v2 = cmd->v2;
   CALL_Rectsv(ctx->Dispatch.Current, (v1, v2));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Rectsv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_Rectsv(const GLshort *v1, const GLshort *v2)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Rectsv);
   struct marshal_cmd_Rectsv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Rectsv, cmd_size);
   memcpy(cmd->v1, v1, 2 * sizeof(GLshort));
   memcpy(cmd->v2, v2, 2 * sizeof(GLshort));
}


/* TexCoord1d: marshalled asynchronously */
struct marshal_cmd_TexCoord1d
{
   struct marshal_cmd_base cmd_base;
   GLdouble s;
};
uint32_t
_mesa_unmarshal_TexCoord1d(struct gl_context *ctx, const struct marshal_cmd_TexCoord1d *restrict cmd)
{
   GLdouble s = cmd->s;
   CALL_TexCoord1d(ctx->Dispatch.Current, (s));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_TexCoord1d), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_TexCoord1d(GLdouble s)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TexCoord1d);
   struct marshal_cmd_TexCoord1d *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexCoord1d, cmd_size);
   cmd->s = s;
}


/* TexCoord1dv: marshalled asynchronously */
struct marshal_cmd_TexCoord1dv
{
   struct marshal_cmd_base cmd_base;
   GLdouble v[1];
};
uint32_t
_mesa_unmarshal_TexCoord1dv(struct gl_context *ctx, const struct marshal_cmd_TexCoord1dv *restrict cmd)
{
   const GLdouble *v = cmd->v;
   CALL_TexCoord1dv(ctx->Dispatch.Current, (v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_TexCoord1dv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_TexCoord1dv(const GLdouble *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TexCoord1dv);
   struct marshal_cmd_TexCoord1dv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexCoord1dv, cmd_size);
   memcpy(cmd->v, v, 1 * sizeof(GLdouble));
}


/* TexCoord1f: marshalled asynchronously */
struct marshal_cmd_TexCoord1f
{
   struct marshal_cmd_base cmd_base;
   GLfloat s;
};
uint32_t
_mesa_unmarshal_TexCoord1f(struct gl_context *ctx, const struct marshal_cmd_TexCoord1f *restrict cmd)
{
   GLfloat s = cmd->s;
   CALL_TexCoord1f(ctx->Dispatch.Current, (s));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_TexCoord1f), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_TexCoord1f(GLfloat s)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TexCoord1f);
   struct marshal_cmd_TexCoord1f *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexCoord1f, cmd_size);
   cmd->s = s;
}


/* TexCoord1fv: marshalled asynchronously */
struct marshal_cmd_TexCoord1fv
{
   struct marshal_cmd_base cmd_base;
   GLfloat v[1];
};
uint32_t
_mesa_unmarshal_TexCoord1fv(struct gl_context *ctx, const struct marshal_cmd_TexCoord1fv *restrict cmd)
{
   const GLfloat *v = cmd->v;
   CALL_TexCoord1fv(ctx->Dispatch.Current, (v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_TexCoord1fv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_TexCoord1fv(const GLfloat *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TexCoord1fv);
   struct marshal_cmd_TexCoord1fv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexCoord1fv, cmd_size);
   memcpy(cmd->v, v, 1 * sizeof(GLfloat));
}


/* TexCoord1i: marshalled asynchronously */
struct marshal_cmd_TexCoord1i
{
   struct marshal_cmd_base cmd_base;
   GLint s;
};
uint32_t
_mesa_unmarshal_TexCoord1i(struct gl_context *ctx, const struct marshal_cmd_TexCoord1i *restrict cmd)
{
   GLint s = cmd->s;
   CALL_TexCoord1i(ctx->Dispatch.Current, (s));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_TexCoord1i), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_TexCoord1i(GLint s)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TexCoord1i);
   struct marshal_cmd_TexCoord1i *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexCoord1i, cmd_size);
   cmd->s = s;
}


/* TexCoord1iv: marshalled asynchronously */
struct marshal_cmd_TexCoord1iv
{
   struct marshal_cmd_base cmd_base;
   GLint v[1];
};
uint32_t
_mesa_unmarshal_TexCoord1iv(struct gl_context *ctx, const struct marshal_cmd_TexCoord1iv *restrict cmd)
{
   const GLint *v = cmd->v;
   CALL_TexCoord1iv(ctx->Dispatch.Current, (v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_TexCoord1iv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_TexCoord1iv(const GLint *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TexCoord1iv);
   struct marshal_cmd_TexCoord1iv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexCoord1iv, cmd_size);
   memcpy(cmd->v, v, 1 * sizeof(GLint));
}


/* TexCoord1s: marshalled asynchronously */
struct marshal_cmd_TexCoord1s
{
   struct marshal_cmd_base cmd_base;
   GLshort s;
};
uint32_t
_mesa_unmarshal_TexCoord1s(struct gl_context *ctx, const struct marshal_cmd_TexCoord1s *restrict cmd)
{
   GLshort s = cmd->s;
   CALL_TexCoord1s(ctx->Dispatch.Current, (s));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_TexCoord1s), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_TexCoord1s(GLshort s)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TexCoord1s);
   struct marshal_cmd_TexCoord1s *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexCoord1s, cmd_size);
   cmd->s = s;
}


/* TexCoord1sv: marshalled asynchronously */
struct marshal_cmd_TexCoord1sv
{
   struct marshal_cmd_base cmd_base;
   GLshort v[1];
};
uint32_t
_mesa_unmarshal_TexCoord1sv(struct gl_context *ctx, const struct marshal_cmd_TexCoord1sv *restrict cmd)
{
   const GLshort *v = cmd->v;
   CALL_TexCoord1sv(ctx->Dispatch.Current, (v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_TexCoord1sv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_TexCoord1sv(const GLshort *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TexCoord1sv);
   struct marshal_cmd_TexCoord1sv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexCoord1sv, cmd_size);
   memcpy(cmd->v, v, 1 * sizeof(GLshort));
}


/* TexCoord2d: marshalled asynchronously */
struct marshal_cmd_TexCoord2d
{
   struct marshal_cmd_base cmd_base;
   GLdouble s;
   GLdouble t;
};
uint32_t
_mesa_unmarshal_TexCoord2d(struct gl_context *ctx, const struct marshal_cmd_TexCoord2d *restrict cmd)
{
   GLdouble s = cmd->s;
   GLdouble t = cmd->t;
   CALL_TexCoord2d(ctx->Dispatch.Current, (s, t));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_TexCoord2d), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_TexCoord2d(GLdouble s, GLdouble t)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TexCoord2d);
   struct marshal_cmd_TexCoord2d *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexCoord2d, cmd_size);
   cmd->s = s;
   cmd->t = t;
}


/* TexCoord2dv: marshalled asynchronously */
struct marshal_cmd_TexCoord2dv
{
   struct marshal_cmd_base cmd_base;
   GLdouble v[2];
};
uint32_t
_mesa_unmarshal_TexCoord2dv(struct gl_context *ctx, const struct marshal_cmd_TexCoord2dv *restrict cmd)
{
   const GLdouble *v = cmd->v;
   CALL_TexCoord2dv(ctx->Dispatch.Current, (v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_TexCoord2dv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_TexCoord2dv(const GLdouble *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TexCoord2dv);
   struct marshal_cmd_TexCoord2dv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexCoord2dv, cmd_size);
   memcpy(cmd->v, v, 2 * sizeof(GLdouble));
}


/* TexCoord2f: marshalled asynchronously */
struct marshal_cmd_TexCoord2f
{
   struct marshal_cmd_base cmd_base;
   GLfloat s;
   GLfloat t;
};
uint32_t
_mesa_unmarshal_TexCoord2f(struct gl_context *ctx, const struct marshal_cmd_TexCoord2f *restrict cmd)
{
   GLfloat s = cmd->s;
   GLfloat t = cmd->t;
   CALL_TexCoord2f(ctx->Dispatch.Current, (s, t));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_TexCoord2f), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_TexCoord2f(GLfloat s, GLfloat t)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TexCoord2f);
   struct marshal_cmd_TexCoord2f *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexCoord2f, cmd_size);
   cmd->s = s;
   cmd->t = t;
}


/* TexCoord2fv: marshalled asynchronously */
struct marshal_cmd_TexCoord2fv
{
   struct marshal_cmd_base cmd_base;
   GLfloat v[2];
};
uint32_t
_mesa_unmarshal_TexCoord2fv(struct gl_context *ctx, const struct marshal_cmd_TexCoord2fv *restrict cmd)
{
   const GLfloat *v = cmd->v;
   CALL_TexCoord2fv(ctx->Dispatch.Current, (v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_TexCoord2fv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_TexCoord2fv(const GLfloat *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TexCoord2fv);
   struct marshal_cmd_TexCoord2fv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexCoord2fv, cmd_size);
   memcpy(cmd->v, v, 2 * sizeof(GLfloat));
}


/* TexCoord2i: marshalled asynchronously */
struct marshal_cmd_TexCoord2i
{
   struct marshal_cmd_base cmd_base;
   GLint s;
   GLint t;
};
uint32_t
_mesa_unmarshal_TexCoord2i(struct gl_context *ctx, const struct marshal_cmd_TexCoord2i *restrict cmd)
{
   GLint s = cmd->s;
   GLint t = cmd->t;
   CALL_TexCoord2i(ctx->Dispatch.Current, (s, t));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_TexCoord2i), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_TexCoord2i(GLint s, GLint t)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TexCoord2i);
   struct marshal_cmd_TexCoord2i *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexCoord2i, cmd_size);
   cmd->s = s;
   cmd->t = t;
}


/* TexCoord2iv: marshalled asynchronously */
struct marshal_cmd_TexCoord2iv
{
   struct marshal_cmd_base cmd_base;
   GLint v[2];
};
uint32_t
_mesa_unmarshal_TexCoord2iv(struct gl_context *ctx, const struct marshal_cmd_TexCoord2iv *restrict cmd)
{
   const GLint *v = cmd->v;
   CALL_TexCoord2iv(ctx->Dispatch.Current, (v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_TexCoord2iv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_TexCoord2iv(const GLint *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TexCoord2iv);
   struct marshal_cmd_TexCoord2iv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexCoord2iv, cmd_size);
   memcpy(cmd->v, v, 2 * sizeof(GLint));
}


/* TexCoord2s: marshalled asynchronously */
struct marshal_cmd_TexCoord2s
{
   struct marshal_cmd_base cmd_base;
   GLshort s;
   GLshort t;
};
uint32_t
_mesa_unmarshal_TexCoord2s(struct gl_context *ctx, const struct marshal_cmd_TexCoord2s *restrict cmd)
{
   GLshort s = cmd->s;
   GLshort t = cmd->t;
   CALL_TexCoord2s(ctx->Dispatch.Current, (s, t));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_TexCoord2s), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_TexCoord2s(GLshort s, GLshort t)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TexCoord2s);
   struct marshal_cmd_TexCoord2s *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexCoord2s, cmd_size);
   cmd->s = s;
   cmd->t = t;
}


/* TexCoord2sv: marshalled asynchronously */
struct marshal_cmd_TexCoord2sv
{
   struct marshal_cmd_base cmd_base;
   GLshort v[2];
};
uint32_t
_mesa_unmarshal_TexCoord2sv(struct gl_context *ctx, const struct marshal_cmd_TexCoord2sv *restrict cmd)
{
   const GLshort *v = cmd->v;
   CALL_TexCoord2sv(ctx->Dispatch.Current, (v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_TexCoord2sv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_TexCoord2sv(const GLshort *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TexCoord2sv);
   struct marshal_cmd_TexCoord2sv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexCoord2sv, cmd_size);
   memcpy(cmd->v, v, 2 * sizeof(GLshort));
}


/* TexCoord3d: marshalled asynchronously */
struct marshal_cmd_TexCoord3d
{
   struct marshal_cmd_base cmd_base;
   GLdouble s;
   GLdouble t;
   GLdouble r;
};
uint32_t
_mesa_unmarshal_TexCoord3d(struct gl_context *ctx, const struct marshal_cmd_TexCoord3d *restrict cmd)
{
   GLdouble s = cmd->s;
   GLdouble t = cmd->t;
   GLdouble r = cmd->r;
   CALL_TexCoord3d(ctx->Dispatch.Current, (s, t, r));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_TexCoord3d), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_TexCoord3d(GLdouble s, GLdouble t, GLdouble r)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TexCoord3d);
   struct marshal_cmd_TexCoord3d *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexCoord3d, cmd_size);
   cmd->s = s;
   cmd->t = t;
   cmd->r = r;
}


/* TexCoord3dv: marshalled asynchronously */
struct marshal_cmd_TexCoord3dv
{
   struct marshal_cmd_base cmd_base;
   GLdouble v[3];
};
uint32_t
_mesa_unmarshal_TexCoord3dv(struct gl_context *ctx, const struct marshal_cmd_TexCoord3dv *restrict cmd)
{
   const GLdouble *v = cmd->v;
   CALL_TexCoord3dv(ctx->Dispatch.Current, (v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_TexCoord3dv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_TexCoord3dv(const GLdouble *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TexCoord3dv);
   struct marshal_cmd_TexCoord3dv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexCoord3dv, cmd_size);
   memcpy(cmd->v, v, 3 * sizeof(GLdouble));
}


/* TexCoord3f: marshalled asynchronously */
struct marshal_cmd_TexCoord3f
{
   struct marshal_cmd_base cmd_base;
   GLfloat s;
   GLfloat t;
   GLfloat r;
};
uint32_t
_mesa_unmarshal_TexCoord3f(struct gl_context *ctx, const struct marshal_cmd_TexCoord3f *restrict cmd)
{
   GLfloat s = cmd->s;
   GLfloat t = cmd->t;
   GLfloat r = cmd->r;
   CALL_TexCoord3f(ctx->Dispatch.Current, (s, t, r));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_TexCoord3f), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_TexCoord3f(GLfloat s, GLfloat t, GLfloat r)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TexCoord3f);
   struct marshal_cmd_TexCoord3f *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexCoord3f, cmd_size);
   cmd->s = s;
   cmd->t = t;
   cmd->r = r;
}


/* TexCoord3fv: marshalled asynchronously */
struct marshal_cmd_TexCoord3fv
{
   struct marshal_cmd_base cmd_base;
   GLfloat v[3];
};
uint32_t
_mesa_unmarshal_TexCoord3fv(struct gl_context *ctx, const struct marshal_cmd_TexCoord3fv *restrict cmd)
{
   const GLfloat *v = cmd->v;
   CALL_TexCoord3fv(ctx->Dispatch.Current, (v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_TexCoord3fv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_TexCoord3fv(const GLfloat *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TexCoord3fv);
   struct marshal_cmd_TexCoord3fv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexCoord3fv, cmd_size);
   memcpy(cmd->v, v, 3 * sizeof(GLfloat));
}


/* TexCoord3i: marshalled asynchronously */
struct marshal_cmd_TexCoord3i
{
   struct marshal_cmd_base cmd_base;
   GLint s;
   GLint t;
   GLint r;
};
uint32_t
_mesa_unmarshal_TexCoord3i(struct gl_context *ctx, const struct marshal_cmd_TexCoord3i *restrict cmd)
{
   GLint s = cmd->s;
   GLint t = cmd->t;
   GLint r = cmd->r;
   CALL_TexCoord3i(ctx->Dispatch.Current, (s, t, r));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_TexCoord3i), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_TexCoord3i(GLint s, GLint t, GLint r)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TexCoord3i);
   struct marshal_cmd_TexCoord3i *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexCoord3i, cmd_size);
   cmd->s = s;
   cmd->t = t;
   cmd->r = r;
}


/* TexCoord3iv: marshalled asynchronously */
struct marshal_cmd_TexCoord3iv
{
   struct marshal_cmd_base cmd_base;
   GLint v[3];
};
uint32_t
_mesa_unmarshal_TexCoord3iv(struct gl_context *ctx, const struct marshal_cmd_TexCoord3iv *restrict cmd)
{
   const GLint *v = cmd->v;
   CALL_TexCoord3iv(ctx->Dispatch.Current, (v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_TexCoord3iv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_TexCoord3iv(const GLint *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TexCoord3iv);
   struct marshal_cmd_TexCoord3iv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexCoord3iv, cmd_size);
   memcpy(cmd->v, v, 3 * sizeof(GLint));
}


/* TexCoord3s: marshalled asynchronously */
struct marshal_cmd_TexCoord3s
{
   struct marshal_cmd_base cmd_base;
   GLshort s;
   GLshort t;
   GLshort r;
};
uint32_t
_mesa_unmarshal_TexCoord3s(struct gl_context *ctx, const struct marshal_cmd_TexCoord3s *restrict cmd)
{
   GLshort s = cmd->s;
   GLshort t = cmd->t;
   GLshort r = cmd->r;
   CALL_TexCoord3s(ctx->Dispatch.Current, (s, t, r));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_TexCoord3s), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_TexCoord3s(GLshort s, GLshort t, GLshort r)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TexCoord3s);
   struct marshal_cmd_TexCoord3s *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexCoord3s, cmd_size);
   cmd->s = s;
   cmd->t = t;
   cmd->r = r;
}


/* TexCoord3sv: marshalled asynchronously */
struct marshal_cmd_TexCoord3sv
{
   struct marshal_cmd_base cmd_base;
   GLshort v[3];
};
uint32_t
_mesa_unmarshal_TexCoord3sv(struct gl_context *ctx, const struct marshal_cmd_TexCoord3sv *restrict cmd)
{
   const GLshort *v = cmd->v;
   CALL_TexCoord3sv(ctx->Dispatch.Current, (v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_TexCoord3sv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_TexCoord3sv(const GLshort *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TexCoord3sv);
   struct marshal_cmd_TexCoord3sv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexCoord3sv, cmd_size);
   memcpy(cmd->v, v, 3 * sizeof(GLshort));
}


/* TexCoord4d: marshalled asynchronously */
struct marshal_cmd_TexCoord4d
{
   struct marshal_cmd_base cmd_base;
   GLdouble s;
   GLdouble t;
   GLdouble r;
   GLdouble q;
};
uint32_t
_mesa_unmarshal_TexCoord4d(struct gl_context *ctx, const struct marshal_cmd_TexCoord4d *restrict cmd)
{
   GLdouble s = cmd->s;
   GLdouble t = cmd->t;
   GLdouble r = cmd->r;
   GLdouble q = cmd->q;
   CALL_TexCoord4d(ctx->Dispatch.Current, (s, t, r, q));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_TexCoord4d), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_TexCoord4d(GLdouble s, GLdouble t, GLdouble r, GLdouble q)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TexCoord4d);
   struct marshal_cmd_TexCoord4d *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexCoord4d, cmd_size);
   cmd->s = s;
   cmd->t = t;
   cmd->r = r;
   cmd->q = q;
}


/* TexCoord4dv: marshalled asynchronously */
struct marshal_cmd_TexCoord4dv
{
   struct marshal_cmd_base cmd_base;
   GLdouble v[4];
};
uint32_t
_mesa_unmarshal_TexCoord4dv(struct gl_context *ctx, const struct marshal_cmd_TexCoord4dv *restrict cmd)
{
   const GLdouble *v = cmd->v;
   CALL_TexCoord4dv(ctx->Dispatch.Current, (v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_TexCoord4dv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_TexCoord4dv(const GLdouble *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TexCoord4dv);
   struct marshal_cmd_TexCoord4dv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexCoord4dv, cmd_size);
   memcpy(cmd->v, v, 4 * sizeof(GLdouble));
}


/* TexCoord4f: marshalled asynchronously */
struct marshal_cmd_TexCoord4f
{
   struct marshal_cmd_base cmd_base;
   GLfloat s;
   GLfloat t;
   GLfloat r;
   GLfloat q;
};
uint32_t
_mesa_unmarshal_TexCoord4f(struct gl_context *ctx, const struct marshal_cmd_TexCoord4f *restrict cmd)
{
   GLfloat s = cmd->s;
   GLfloat t = cmd->t;
   GLfloat r = cmd->r;
   GLfloat q = cmd->q;
   CALL_TexCoord4f(ctx->Dispatch.Current, (s, t, r, q));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_TexCoord4f), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_TexCoord4f(GLfloat s, GLfloat t, GLfloat r, GLfloat q)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TexCoord4f);
   struct marshal_cmd_TexCoord4f *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexCoord4f, cmd_size);
   cmd->s = s;
   cmd->t = t;
   cmd->r = r;
   cmd->q = q;
}


/* TexCoord4fv: marshalled asynchronously */
struct marshal_cmd_TexCoord4fv
{
   struct marshal_cmd_base cmd_base;
   GLfloat v[4];
};
uint32_t
_mesa_unmarshal_TexCoord4fv(struct gl_context *ctx, const struct marshal_cmd_TexCoord4fv *restrict cmd)
{
   const GLfloat *v = cmd->v;
   CALL_TexCoord4fv(ctx->Dispatch.Current, (v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_TexCoord4fv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_TexCoord4fv(const GLfloat *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TexCoord4fv);
   struct marshal_cmd_TexCoord4fv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexCoord4fv, cmd_size);
   memcpy(cmd->v, v, 4 * sizeof(GLfloat));
}


/* TexCoord4i: marshalled asynchronously */
struct marshal_cmd_TexCoord4i
{
   struct marshal_cmd_base cmd_base;
   GLint s;
   GLint t;
   GLint r;
   GLint q;
};
uint32_t
_mesa_unmarshal_TexCoord4i(struct gl_context *ctx, const struct marshal_cmd_TexCoord4i *restrict cmd)
{
   GLint s = cmd->s;
   GLint t = cmd->t;
   GLint r = cmd->r;
   GLint q = cmd->q;
   CALL_TexCoord4i(ctx->Dispatch.Current, (s, t, r, q));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_TexCoord4i), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_TexCoord4i(GLint s, GLint t, GLint r, GLint q)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TexCoord4i);
   struct marshal_cmd_TexCoord4i *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexCoord4i, cmd_size);
   cmd->s = s;
   cmd->t = t;
   cmd->r = r;
   cmd->q = q;
}


/* TexCoord4iv: marshalled asynchronously */
struct marshal_cmd_TexCoord4iv
{
   struct marshal_cmd_base cmd_base;
   GLint v[4];
};
uint32_t
_mesa_unmarshal_TexCoord4iv(struct gl_context *ctx, const struct marshal_cmd_TexCoord4iv *restrict cmd)
{
   const GLint *v = cmd->v;
   CALL_TexCoord4iv(ctx->Dispatch.Current, (v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_TexCoord4iv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_TexCoord4iv(const GLint *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TexCoord4iv);
   struct marshal_cmd_TexCoord4iv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexCoord4iv, cmd_size);
   memcpy(cmd->v, v, 4 * sizeof(GLint));
}


/* TexCoord4s: marshalled asynchronously */
struct marshal_cmd_TexCoord4s
{
   struct marshal_cmd_base cmd_base;
   GLshort s;
   GLshort t;
   GLshort r;
   GLshort q;
};
uint32_t
_mesa_unmarshal_TexCoord4s(struct gl_context *ctx, const struct marshal_cmd_TexCoord4s *restrict cmd)
{
   GLshort s = cmd->s;
   GLshort t = cmd->t;
   GLshort r = cmd->r;
   GLshort q = cmd->q;
   CALL_TexCoord4s(ctx->Dispatch.Current, (s, t, r, q));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_TexCoord4s), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_TexCoord4s(GLshort s, GLshort t, GLshort r, GLshort q)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TexCoord4s);
   struct marshal_cmd_TexCoord4s *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexCoord4s, cmd_size);
   cmd->s = s;
   cmd->t = t;
   cmd->r = r;
   cmd->q = q;
}


/* TexCoord4sv: marshalled asynchronously */
struct marshal_cmd_TexCoord4sv
{
   struct marshal_cmd_base cmd_base;
   GLshort v[4];
};
uint32_t
_mesa_unmarshal_TexCoord4sv(struct gl_context *ctx, const struct marshal_cmd_TexCoord4sv *restrict cmd)
{
   const GLshort *v = cmd->v;
   CALL_TexCoord4sv(ctx->Dispatch.Current, (v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_TexCoord4sv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_TexCoord4sv(const GLshort *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TexCoord4sv);
   struct marshal_cmd_TexCoord4sv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexCoord4sv, cmd_size);
   memcpy(cmd->v, v, 4 * sizeof(GLshort));
}


/* Vertex2d: marshalled asynchronously */
struct marshal_cmd_Vertex2d
{
   struct marshal_cmd_base cmd_base;
   GLdouble x;
   GLdouble y;
};
uint32_t
_mesa_unmarshal_Vertex2d(struct gl_context *ctx, const struct marshal_cmd_Vertex2d *restrict cmd)
{
   GLdouble x = cmd->x;
   GLdouble y = cmd->y;
   CALL_Vertex2d(ctx->Dispatch.Current, (x, y));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Vertex2d), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_Vertex2d(GLdouble x, GLdouble y)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Vertex2d);
   struct marshal_cmd_Vertex2d *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Vertex2d, cmd_size);
   cmd->x = x;
   cmd->y = y;
}


/* Vertex2dv: marshalled asynchronously */
struct marshal_cmd_Vertex2dv
{
   struct marshal_cmd_base cmd_base;
   GLdouble v[2];
};
uint32_t
_mesa_unmarshal_Vertex2dv(struct gl_context *ctx, const struct marshal_cmd_Vertex2dv *restrict cmd)
{
   const GLdouble *v = cmd->v;
   CALL_Vertex2dv(ctx->Dispatch.Current, (v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Vertex2dv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_Vertex2dv(const GLdouble *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Vertex2dv);
   struct marshal_cmd_Vertex2dv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Vertex2dv, cmd_size);
   memcpy(cmd->v, v, 2 * sizeof(GLdouble));
}


/* Vertex2f: marshalled asynchronously */
struct marshal_cmd_Vertex2f
{
   struct marshal_cmd_base cmd_base;
   GLfloat x;
   GLfloat y;
};
uint32_t
_mesa_unmarshal_Vertex2f(struct gl_context *ctx, const struct marshal_cmd_Vertex2f *restrict cmd)
{
   GLfloat x = cmd->x;
   GLfloat y = cmd->y;
   CALL_Vertex2f(ctx->Dispatch.Current, (x, y));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Vertex2f), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_Vertex2f(GLfloat x, GLfloat y)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Vertex2f);
   struct marshal_cmd_Vertex2f *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Vertex2f, cmd_size);
   cmd->x = x;
   cmd->y = y;
}


/* Vertex2fv: marshalled asynchronously */
struct marshal_cmd_Vertex2fv
{
   struct marshal_cmd_base cmd_base;
   GLfloat v[2];
};
uint32_t
_mesa_unmarshal_Vertex2fv(struct gl_context *ctx, const struct marshal_cmd_Vertex2fv *restrict cmd)
{
   const GLfloat *v = cmd->v;
   CALL_Vertex2fv(ctx->Dispatch.Current, (v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Vertex2fv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_Vertex2fv(const GLfloat *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Vertex2fv);
   struct marshal_cmd_Vertex2fv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Vertex2fv, cmd_size);
   memcpy(cmd->v, v, 2 * sizeof(GLfloat));
}


/* Vertex2i: marshalled asynchronously */
struct marshal_cmd_Vertex2i
{
   struct marshal_cmd_base cmd_base;
   GLint x;
   GLint y;
};
uint32_t
_mesa_unmarshal_Vertex2i(struct gl_context *ctx, const struct marshal_cmd_Vertex2i *restrict cmd)
{
   GLint x = cmd->x;
   GLint y = cmd->y;
   CALL_Vertex2i(ctx->Dispatch.Current, (x, y));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Vertex2i), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_Vertex2i(GLint x, GLint y)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Vertex2i);
   struct marshal_cmd_Vertex2i *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Vertex2i, cmd_size);
   cmd->x = x;
   cmd->y = y;
}


/* Vertex2iv: marshalled asynchronously */
struct marshal_cmd_Vertex2iv
{
   struct marshal_cmd_base cmd_base;
   GLint v[2];
};
uint32_t
_mesa_unmarshal_Vertex2iv(struct gl_context *ctx, const struct marshal_cmd_Vertex2iv *restrict cmd)
{
   const GLint *v = cmd->v;
   CALL_Vertex2iv(ctx->Dispatch.Current, (v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Vertex2iv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_Vertex2iv(const GLint *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Vertex2iv);
   struct marshal_cmd_Vertex2iv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Vertex2iv, cmd_size);
   memcpy(cmd->v, v, 2 * sizeof(GLint));
}


/* Vertex2s: marshalled asynchronously */
struct marshal_cmd_Vertex2s
{
   struct marshal_cmd_base cmd_base;
   GLshort x;
   GLshort y;
};
uint32_t
_mesa_unmarshal_Vertex2s(struct gl_context *ctx, const struct marshal_cmd_Vertex2s *restrict cmd)
{
   GLshort x = cmd->x;
   GLshort y = cmd->y;
   CALL_Vertex2s(ctx->Dispatch.Current, (x, y));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Vertex2s), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_Vertex2s(GLshort x, GLshort y)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Vertex2s);
   struct marshal_cmd_Vertex2s *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Vertex2s, cmd_size);
   cmd->x = x;
   cmd->y = y;
}


/* Vertex2sv: marshalled asynchronously */
struct marshal_cmd_Vertex2sv
{
   struct marshal_cmd_base cmd_base;
   GLshort v[2];
};
uint32_t
_mesa_unmarshal_Vertex2sv(struct gl_context *ctx, const struct marshal_cmd_Vertex2sv *restrict cmd)
{
   const GLshort *v = cmd->v;
   CALL_Vertex2sv(ctx->Dispatch.Current, (v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Vertex2sv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_Vertex2sv(const GLshort *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Vertex2sv);
   struct marshal_cmd_Vertex2sv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Vertex2sv, cmd_size);
   memcpy(cmd->v, v, 2 * sizeof(GLshort));
}


/* Vertex3d: marshalled asynchronously */
struct marshal_cmd_Vertex3d
{
   struct marshal_cmd_base cmd_base;
   GLdouble x;
   GLdouble y;
   GLdouble z;
};
uint32_t
_mesa_unmarshal_Vertex3d(struct gl_context *ctx, const struct marshal_cmd_Vertex3d *restrict cmd)
{
   GLdouble x = cmd->x;
   GLdouble y = cmd->y;
   GLdouble z = cmd->z;
   CALL_Vertex3d(ctx->Dispatch.Current, (x, y, z));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Vertex3d), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_Vertex3d(GLdouble x, GLdouble y, GLdouble z)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Vertex3d);
   struct marshal_cmd_Vertex3d *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Vertex3d, cmd_size);
   cmd->x = x;
   cmd->y = y;
   cmd->z = z;
}


/* Vertex3dv: marshalled asynchronously */
struct marshal_cmd_Vertex3dv
{
   struct marshal_cmd_base cmd_base;
   GLdouble v[3];
};
uint32_t
_mesa_unmarshal_Vertex3dv(struct gl_context *ctx, const struct marshal_cmd_Vertex3dv *restrict cmd)
{
   const GLdouble *v = cmd->v;
   CALL_Vertex3dv(ctx->Dispatch.Current, (v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Vertex3dv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_Vertex3dv(const GLdouble *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Vertex3dv);
   struct marshal_cmd_Vertex3dv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Vertex3dv, cmd_size);
   memcpy(cmd->v, v, 3 * sizeof(GLdouble));
}


/* Vertex3f: marshalled asynchronously */
struct marshal_cmd_Vertex3f
{
   struct marshal_cmd_base cmd_base;
   GLfloat x;
   GLfloat y;
   GLfloat z;
};
uint32_t
_mesa_unmarshal_Vertex3f(struct gl_context *ctx, const struct marshal_cmd_Vertex3f *restrict cmd)
{
   GLfloat x = cmd->x;
   GLfloat y = cmd->y;
   GLfloat z = cmd->z;
   CALL_Vertex3f(ctx->Dispatch.Current, (x, y, z));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Vertex3f), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_Vertex3f(GLfloat x, GLfloat y, GLfloat z)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Vertex3f);
   struct marshal_cmd_Vertex3f *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Vertex3f, cmd_size);
   cmd->x = x;
   cmd->y = y;
   cmd->z = z;
}


/* Vertex3fv: marshalled asynchronously */
struct marshal_cmd_Vertex3fv
{
   struct marshal_cmd_base cmd_base;
   GLfloat v[3];
};
uint32_t
_mesa_unmarshal_Vertex3fv(struct gl_context *ctx, const struct marshal_cmd_Vertex3fv *restrict cmd)
{
   const GLfloat *v = cmd->v;
   CALL_Vertex3fv(ctx->Dispatch.Current, (v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Vertex3fv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_Vertex3fv(const GLfloat *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Vertex3fv);
   struct marshal_cmd_Vertex3fv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Vertex3fv, cmd_size);
   memcpy(cmd->v, v, 3 * sizeof(GLfloat));
}


/* Vertex3i: marshalled asynchronously */
struct marshal_cmd_Vertex3i
{
   struct marshal_cmd_base cmd_base;
   GLint x;
   GLint y;
   GLint z;
};
uint32_t
_mesa_unmarshal_Vertex3i(struct gl_context *ctx, const struct marshal_cmd_Vertex3i *restrict cmd)
{
   GLint x = cmd->x;
   GLint y = cmd->y;
   GLint z = cmd->z;
   CALL_Vertex3i(ctx->Dispatch.Current, (x, y, z));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Vertex3i), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_Vertex3i(GLint x, GLint y, GLint z)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Vertex3i);
   struct marshal_cmd_Vertex3i *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Vertex3i, cmd_size);
   cmd->x = x;
   cmd->y = y;
   cmd->z = z;
}


/* Vertex3iv: marshalled asynchronously */
struct marshal_cmd_Vertex3iv
{
   struct marshal_cmd_base cmd_base;
   GLint v[3];
};
uint32_t
_mesa_unmarshal_Vertex3iv(struct gl_context *ctx, const struct marshal_cmd_Vertex3iv *restrict cmd)
{
   const GLint *v = cmd->v;
   CALL_Vertex3iv(ctx->Dispatch.Current, (v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Vertex3iv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_Vertex3iv(const GLint *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Vertex3iv);
   struct marshal_cmd_Vertex3iv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Vertex3iv, cmd_size);
   memcpy(cmd->v, v, 3 * sizeof(GLint));
}


/* Vertex3s: marshalled asynchronously */
struct marshal_cmd_Vertex3s
{
   struct marshal_cmd_base cmd_base;
   GLshort x;
   GLshort y;
   GLshort z;
};
uint32_t
_mesa_unmarshal_Vertex3s(struct gl_context *ctx, const struct marshal_cmd_Vertex3s *restrict cmd)
{
   GLshort x = cmd->x;
   GLshort y = cmd->y;
   GLshort z = cmd->z;
   CALL_Vertex3s(ctx->Dispatch.Current, (x, y, z));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Vertex3s), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_Vertex3s(GLshort x, GLshort y, GLshort z)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Vertex3s);
   struct marshal_cmd_Vertex3s *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Vertex3s, cmd_size);
   cmd->x = x;
   cmd->y = y;
   cmd->z = z;
}


/* Vertex3sv: marshalled asynchronously */
struct marshal_cmd_Vertex3sv
{
   struct marshal_cmd_base cmd_base;
   GLshort v[3];
};
uint32_t
_mesa_unmarshal_Vertex3sv(struct gl_context *ctx, const struct marshal_cmd_Vertex3sv *restrict cmd)
{
   const GLshort *v = cmd->v;
   CALL_Vertex3sv(ctx->Dispatch.Current, (v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Vertex3sv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_Vertex3sv(const GLshort *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Vertex3sv);
   struct marshal_cmd_Vertex3sv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Vertex3sv, cmd_size);
   memcpy(cmd->v, v, 3 * sizeof(GLshort));
}


/* Vertex4d: marshalled asynchronously */
struct marshal_cmd_Vertex4d
{
   struct marshal_cmd_base cmd_base;
   GLdouble x;
   GLdouble y;
   GLdouble z;
   GLdouble w;
};
uint32_t
_mesa_unmarshal_Vertex4d(struct gl_context *ctx, const struct marshal_cmd_Vertex4d *restrict cmd)
{
   GLdouble x = cmd->x;
   GLdouble y = cmd->y;
   GLdouble z = cmd->z;
   GLdouble w = cmd->w;
   CALL_Vertex4d(ctx->Dispatch.Current, (x, y, z, w));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Vertex4d), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_Vertex4d(GLdouble x, GLdouble y, GLdouble z, GLdouble w)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Vertex4d);
   struct marshal_cmd_Vertex4d *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Vertex4d, cmd_size);
   cmd->x = x;
   cmd->y = y;
   cmd->z = z;
   cmd->w = w;
}


/* Vertex4dv: marshalled asynchronously */
struct marshal_cmd_Vertex4dv
{
   struct marshal_cmd_base cmd_base;
   GLdouble v[4];
};
uint32_t
_mesa_unmarshal_Vertex4dv(struct gl_context *ctx, const struct marshal_cmd_Vertex4dv *restrict cmd)
{
   const GLdouble *v = cmd->v;
   CALL_Vertex4dv(ctx->Dispatch.Current, (v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Vertex4dv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_Vertex4dv(const GLdouble *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Vertex4dv);
   struct marshal_cmd_Vertex4dv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Vertex4dv, cmd_size);
   memcpy(cmd->v, v, 4 * sizeof(GLdouble));
}


/* Vertex4f: marshalled asynchronously */
struct marshal_cmd_Vertex4f
{
   struct marshal_cmd_base cmd_base;
   GLfloat x;
   GLfloat y;
   GLfloat z;
   GLfloat w;
};
uint32_t
_mesa_unmarshal_Vertex4f(struct gl_context *ctx, const struct marshal_cmd_Vertex4f *restrict cmd)
{
   GLfloat x = cmd->x;
   GLfloat y = cmd->y;
   GLfloat z = cmd->z;
   GLfloat w = cmd->w;
   CALL_Vertex4f(ctx->Dispatch.Current, (x, y, z, w));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Vertex4f), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_Vertex4f(GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Vertex4f);
   struct marshal_cmd_Vertex4f *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Vertex4f, cmd_size);
   cmd->x = x;
   cmd->y = y;
   cmd->z = z;
   cmd->w = w;
}


/* Vertex4fv: marshalled asynchronously */
struct marshal_cmd_Vertex4fv
{
   struct marshal_cmd_base cmd_base;
   GLfloat v[4];
};
uint32_t
_mesa_unmarshal_Vertex4fv(struct gl_context *ctx, const struct marshal_cmd_Vertex4fv *restrict cmd)
{
   const GLfloat *v = cmd->v;
   CALL_Vertex4fv(ctx->Dispatch.Current, (v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Vertex4fv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_Vertex4fv(const GLfloat *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Vertex4fv);
   struct marshal_cmd_Vertex4fv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Vertex4fv, cmd_size);
   memcpy(cmd->v, v, 4 * sizeof(GLfloat));
}


/* Vertex4i: marshalled asynchronously */
struct marshal_cmd_Vertex4i
{
   struct marshal_cmd_base cmd_base;
   GLint x;
   GLint y;
   GLint z;
   GLint w;
};
uint32_t
_mesa_unmarshal_Vertex4i(struct gl_context *ctx, const struct marshal_cmd_Vertex4i *restrict cmd)
{
   GLint x = cmd->x;
   GLint y = cmd->y;
   GLint z = cmd->z;
   GLint w = cmd->w;
   CALL_Vertex4i(ctx->Dispatch.Current, (x, y, z, w));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Vertex4i), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_Vertex4i(GLint x, GLint y, GLint z, GLint w)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Vertex4i);
   struct marshal_cmd_Vertex4i *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Vertex4i, cmd_size);
   cmd->x = x;
   cmd->y = y;
   cmd->z = z;
   cmd->w = w;
}


/* Vertex4iv: marshalled asynchronously */
struct marshal_cmd_Vertex4iv
{
   struct marshal_cmd_base cmd_base;
   GLint v[4];
};
uint32_t
_mesa_unmarshal_Vertex4iv(struct gl_context *ctx, const struct marshal_cmd_Vertex4iv *restrict cmd)
{
   const GLint *v = cmd->v;
   CALL_Vertex4iv(ctx->Dispatch.Current, (v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Vertex4iv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_Vertex4iv(const GLint *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Vertex4iv);
   struct marshal_cmd_Vertex4iv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Vertex4iv, cmd_size);
   memcpy(cmd->v, v, 4 * sizeof(GLint));
}


/* Vertex4s: marshalled asynchronously */
struct marshal_cmd_Vertex4s
{
   struct marshal_cmd_base cmd_base;
   GLshort x;
   GLshort y;
   GLshort z;
   GLshort w;
};
uint32_t
_mesa_unmarshal_Vertex4s(struct gl_context *ctx, const struct marshal_cmd_Vertex4s *restrict cmd)
{
   GLshort x = cmd->x;
   GLshort y = cmd->y;
   GLshort z = cmd->z;
   GLshort w = cmd->w;
   CALL_Vertex4s(ctx->Dispatch.Current, (x, y, z, w));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Vertex4s), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_Vertex4s(GLshort x, GLshort y, GLshort z, GLshort w)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Vertex4s);
   struct marshal_cmd_Vertex4s *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Vertex4s, cmd_size);
   cmd->x = x;
   cmd->y = y;
   cmd->z = z;
   cmd->w = w;
}


/* Vertex4sv: marshalled asynchronously */
struct marshal_cmd_Vertex4sv
{
   struct marshal_cmd_base cmd_base;
   GLshort v[4];
};
uint32_t
_mesa_unmarshal_Vertex4sv(struct gl_context *ctx, const struct marshal_cmd_Vertex4sv *restrict cmd)
{
   const GLshort *v = cmd->v;
   CALL_Vertex4sv(ctx->Dispatch.Current, (v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Vertex4sv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_Vertex4sv(const GLshort *v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Vertex4sv);
   struct marshal_cmd_Vertex4sv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Vertex4sv, cmd_size);
   memcpy(cmd->v, v, 4 * sizeof(GLshort));
}


/* ClipPlane: marshalled asynchronously */
struct marshal_cmd_ClipPlane
{
   struct marshal_cmd_base cmd_base;
   GLenum16 plane;
   GLdouble equation[4];
};
uint32_t
_mesa_unmarshal_ClipPlane(struct gl_context *ctx, const struct marshal_cmd_ClipPlane *restrict cmd)
{
   GLenum plane = cmd->plane;
   const GLdouble *equation = cmd->equation;
   CALL_ClipPlane(ctx->Dispatch.Current, (plane, equation));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_ClipPlane), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ClipPlane(GLenum plane, const GLdouble *equation)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ClipPlane);
   struct marshal_cmd_ClipPlane *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ClipPlane, cmd_size);
   cmd->plane = MIN2(plane, 0xffff); /* clamped to 0xffff (invalid enum) */
   memcpy(cmd->equation, equation, 4 * sizeof(GLdouble));
}


/* ColorMaterial: marshalled asynchronously */
struct marshal_cmd_ColorMaterial
{
   struct marshal_cmd_base cmd_base;
   GLenum16 face;
   GLenum16 mode;
};
uint32_t
_mesa_unmarshal_ColorMaterial(struct gl_context *ctx, const struct marshal_cmd_ColorMaterial *restrict cmd)
{
   GLenum face = cmd->face;
   GLenum mode = cmd->mode;
   CALL_ColorMaterial(ctx->Dispatch.Current, (face, mode));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_ColorMaterial), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ColorMaterial(GLenum face, GLenum mode)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ColorMaterial);
   struct marshal_cmd_ColorMaterial *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ColorMaterial, cmd_size);
   cmd->face = MIN2(face, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->mode = MIN2(mode, 0xffff); /* clamped to 0xffff (invalid enum) */
}


/* CullFace: marshalled asynchronously */
struct marshal_cmd_CullFace
{
   struct marshal_cmd_base cmd_base;
   GLenum16 mode;
};
uint32_t
_mesa_unmarshal_CullFace(struct gl_context *ctx, const struct marshal_cmd_CullFace *restrict cmd)
{
   GLenum mode = cmd->mode;
   CALL_CullFace(ctx->Dispatch.Current, (mode));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_CullFace), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_CullFace(GLenum mode)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_CullFace);
   struct marshal_cmd_CullFace *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_CullFace, cmd_size);
   cmd->mode = MIN2(mode, 0xffff); /* clamped to 0xffff (invalid enum) */
}


/* Fogf: marshalled asynchronously */
struct marshal_cmd_Fogf
{
   struct marshal_cmd_base cmd_base;
   GLenum16 pname;
   GLfloat param;
};
uint32_t
_mesa_unmarshal_Fogf(struct gl_context *ctx, const struct marshal_cmd_Fogf *restrict cmd)
{
   GLenum pname = cmd->pname;
   GLfloat param = cmd->param;
   CALL_Fogf(ctx->Dispatch.Current, (pname, param));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Fogf), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_Fogf(GLenum pname, GLfloat param)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Fogf);
   struct marshal_cmd_Fogf *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Fogf, cmd_size);
   cmd->pname = MIN2(pname, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->param = param;
}


/* Fogfv: marshalled asynchronously */
struct marshal_cmd_Fogfv
{
   struct marshal_cmd_base cmd_base;
   GLenum16 pname;
   /* Next safe_mul(_mesa_fog_enum_to_count(pname), 1 * sizeof(GLfloat)) bytes are GLfloat params[None] */
};
uint32_t
_mesa_unmarshal_Fogfv(struct gl_context *ctx, const struct marshal_cmd_Fogfv *restrict cmd)
{
   GLenum pname = cmd->pname;
   GLfloat *params;
   const char *variable_data = (const char *) (cmd + 1);
   params = (GLfloat *) variable_data;
   CALL_Fogfv(ctx->Dispatch.Current, (pname, params));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_Fogfv(GLenum pname, const GLfloat *params)
{
   GET_CURRENT_CONTEXT(ctx);
   int params_size = safe_mul(_mesa_fog_enum_to_count(pname), 1 * sizeof(GLfloat));
   int cmd_size = sizeof(struct marshal_cmd_Fogfv) + params_size;
   struct marshal_cmd_Fogfv *cmd;
   if (unlikely(params_size < 0 || (params_size > 0 && !params) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "Fogfv");
      CALL_Fogfv(ctx->Dispatch.Current, (pname, params));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Fogfv, cmd_size);
   cmd->pname = MIN2(pname, 0xffff); /* clamped to 0xffff (invalid enum) */
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, params, params_size);
}


/* Fogi: marshalled asynchronously */
struct marshal_cmd_Fogi
{
   struct marshal_cmd_base cmd_base;
   GLenum16 pname;
   GLint param;
};
uint32_t
_mesa_unmarshal_Fogi(struct gl_context *ctx, const struct marshal_cmd_Fogi *restrict cmd)
{
   GLenum pname = cmd->pname;
   GLint param = cmd->param;
   CALL_Fogi(ctx->Dispatch.Current, (pname, param));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Fogi), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_Fogi(GLenum pname, GLint param)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Fogi);
   struct marshal_cmd_Fogi *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Fogi, cmd_size);
   cmd->pname = MIN2(pname, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->param = param;
}


/* Fogiv: marshalled asynchronously */
struct marshal_cmd_Fogiv
{
   struct marshal_cmd_base cmd_base;
   GLenum16 pname;
   /* Next safe_mul(_mesa_fog_enum_to_count(pname), 1 * sizeof(GLint)) bytes are GLint params[None] */
};
uint32_t
_mesa_unmarshal_Fogiv(struct gl_context *ctx, const struct marshal_cmd_Fogiv *restrict cmd)
{
   GLenum pname = cmd->pname;
   GLint *params;
   const char *variable_data = (const char *) (cmd + 1);
   params = (GLint *) variable_data;
   CALL_Fogiv(ctx->Dispatch.Current, (pname, params));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_Fogiv(GLenum pname, const GLint *params)
{
   GET_CURRENT_CONTEXT(ctx);
   int params_size = safe_mul(_mesa_fog_enum_to_count(pname), 1 * sizeof(GLint));
   int cmd_size = sizeof(struct marshal_cmd_Fogiv) + params_size;
   struct marshal_cmd_Fogiv *cmd;
   if (unlikely(params_size < 0 || (params_size > 0 && !params) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "Fogiv");
      CALL_Fogiv(ctx->Dispatch.Current, (pname, params));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Fogiv, cmd_size);
   cmd->pname = MIN2(pname, 0xffff); /* clamped to 0xffff (invalid enum) */
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, params, params_size);
}


/* FrontFace: marshalled asynchronously */
struct marshal_cmd_FrontFace
{
   struct marshal_cmd_base cmd_base;
   GLenum16 mode;
};
uint32_t
_mesa_unmarshal_FrontFace(struct gl_context *ctx, const struct marshal_cmd_FrontFace *restrict cmd)
{
   GLenum mode = cmd->mode;
   CALL_FrontFace(ctx->Dispatch.Current, (mode));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_FrontFace), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_FrontFace(GLenum mode)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_FrontFace);
   struct marshal_cmd_FrontFace *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_FrontFace, cmd_size);
   cmd->mode = MIN2(mode, 0xffff); /* clamped to 0xffff (invalid enum) */
}


/* Hint: marshalled asynchronously */
struct marshal_cmd_Hint
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLenum16 mode;
};
uint32_t
_mesa_unmarshal_Hint(struct gl_context *ctx, const struct marshal_cmd_Hint *restrict cmd)
{
   GLenum target = cmd->target;
   GLenum mode = cmd->mode;
   CALL_Hint(ctx->Dispatch.Current, (target, mode));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Hint), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_Hint(GLenum target, GLenum mode)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Hint);
   struct marshal_cmd_Hint *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Hint, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->mode = MIN2(mode, 0xffff); /* clamped to 0xffff (invalid enum) */
}


/* Lightf: marshalled asynchronously */
struct marshal_cmd_Lightf
{
   struct marshal_cmd_base cmd_base;
   GLenum16 light;
   GLenum16 pname;
   GLfloat param;
};
uint32_t
_mesa_unmarshal_Lightf(struct gl_context *ctx, const struct marshal_cmd_Lightf *restrict cmd)
{
   GLenum light = cmd->light;
   GLenum pname = cmd->pname;
   GLfloat param = cmd->param;
   CALL_Lightf(ctx->Dispatch.Current, (light, pname, param));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Lightf), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_Lightf(GLenum light, GLenum pname, GLfloat param)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Lightf);
   struct marshal_cmd_Lightf *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Lightf, cmd_size);
   cmd->light = MIN2(light, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->pname = MIN2(pname, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->param = param;
}


/* Lightfv: marshalled asynchronously */
struct marshal_cmd_Lightfv
{
   struct marshal_cmd_base cmd_base;
   GLenum16 light;
   GLenum16 pname;
   /* Next safe_mul(_mesa_light_enum_to_count(pname), 1 * sizeof(GLfloat)) bytes are GLfloat params[None] */
};
uint32_t
_mesa_unmarshal_Lightfv(struct gl_context *ctx, const struct marshal_cmd_Lightfv *restrict cmd)
{
   GLenum light = cmd->light;
   GLenum pname = cmd->pname;
   GLfloat *params;
   const char *variable_data = (const char *) (cmd + 1);
   params = (GLfloat *) variable_data;
   CALL_Lightfv(ctx->Dispatch.Current, (light, pname, params));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_Lightfv(GLenum light, GLenum pname, const GLfloat *params)
{
   GET_CURRENT_CONTEXT(ctx);
   int params_size = safe_mul(_mesa_light_enum_to_count(pname), 1 * sizeof(GLfloat));
   int cmd_size = sizeof(struct marshal_cmd_Lightfv) + params_size;
   struct marshal_cmd_Lightfv *cmd;
   if (unlikely(params_size < 0 || (params_size > 0 && !params) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "Lightfv");
      CALL_Lightfv(ctx->Dispatch.Current, (light, pname, params));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Lightfv, cmd_size);
   cmd->light = MIN2(light, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->pname = MIN2(pname, 0xffff); /* clamped to 0xffff (invalid enum) */
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, params, params_size);
}


/* Lighti: marshalled asynchronously */
struct marshal_cmd_Lighti
{
   struct marshal_cmd_base cmd_base;
   GLenum16 light;
   GLenum16 pname;
   GLint param;
};
uint32_t
_mesa_unmarshal_Lighti(struct gl_context *ctx, const struct marshal_cmd_Lighti *restrict cmd)
{
   GLenum light = cmd->light;
   GLenum pname = cmd->pname;
   GLint param = cmd->param;
   CALL_Lighti(ctx->Dispatch.Current, (light, pname, param));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Lighti), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_Lighti(GLenum light, GLenum pname, GLint param)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Lighti);
   struct marshal_cmd_Lighti *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Lighti, cmd_size);
   cmd->light = MIN2(light, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->pname = MIN2(pname, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->param = param;
}


/* Lightiv: marshalled asynchronously */
struct marshal_cmd_Lightiv
{
   struct marshal_cmd_base cmd_base;
   GLenum16 light;
   GLenum16 pname;
   /* Next safe_mul(_mesa_light_enum_to_count(pname), 1 * sizeof(GLint)) bytes are GLint params[None] */
};
uint32_t
_mesa_unmarshal_Lightiv(struct gl_context *ctx, const struct marshal_cmd_Lightiv *restrict cmd)
{
   GLenum light = cmd->light;
   GLenum pname = cmd->pname;
   GLint *params;
   const char *variable_data = (const char *) (cmd + 1);
   params = (GLint *) variable_data;
   CALL_Lightiv(ctx->Dispatch.Current, (light, pname, params));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_Lightiv(GLenum light, GLenum pname, const GLint *params)
{
   GET_CURRENT_CONTEXT(ctx);
   int params_size = safe_mul(_mesa_light_enum_to_count(pname), 1 * sizeof(GLint));
   int cmd_size = sizeof(struct marshal_cmd_Lightiv) + params_size;
   struct marshal_cmd_Lightiv *cmd;
   if (unlikely(params_size < 0 || (params_size > 0 && !params) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "Lightiv");
      CALL_Lightiv(ctx->Dispatch.Current, (light, pname, params));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Lightiv, cmd_size);
   cmd->light = MIN2(light, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->pname = MIN2(pname, 0xffff); /* clamped to 0xffff (invalid enum) */
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, params, params_size);
}


/* LightModelf: marshalled asynchronously */
struct marshal_cmd_LightModelf
{
   struct marshal_cmd_base cmd_base;
   GLenum16 pname;
   GLfloat param;
};
uint32_t
_mesa_unmarshal_LightModelf(struct gl_context *ctx, const struct marshal_cmd_LightModelf *restrict cmd)
{
   GLenum pname = cmd->pname;
   GLfloat param = cmd->param;
   CALL_LightModelf(ctx->Dispatch.Current, (pname, param));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_LightModelf), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_LightModelf(GLenum pname, GLfloat param)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_LightModelf);
   struct marshal_cmd_LightModelf *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_LightModelf, cmd_size);
   cmd->pname = MIN2(pname, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->param = param;
}


/* LightModelfv: marshalled asynchronously */
struct marshal_cmd_LightModelfv
{
   struct marshal_cmd_base cmd_base;
   GLenum16 pname;
   /* Next safe_mul(_mesa_light_model_enum_to_count(pname), 1 * sizeof(GLfloat)) bytes are GLfloat params[None] */
};
uint32_t
_mesa_unmarshal_LightModelfv(struct gl_context *ctx, const struct marshal_cmd_LightModelfv *restrict cmd)
{
   GLenum pname = cmd->pname;
   GLfloat *params;
   const char *variable_data = (const char *) (cmd + 1);
   params = (GLfloat *) variable_data;
   CALL_LightModelfv(ctx->Dispatch.Current, (pname, params));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_LightModelfv(GLenum pname, const GLfloat *params)
{
   GET_CURRENT_CONTEXT(ctx);
   int params_size = safe_mul(_mesa_light_model_enum_to_count(pname), 1 * sizeof(GLfloat));
   int cmd_size = sizeof(struct marshal_cmd_LightModelfv) + params_size;
   struct marshal_cmd_LightModelfv *cmd;
   if (unlikely(params_size < 0 || (params_size > 0 && !params) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "LightModelfv");
      CALL_LightModelfv(ctx->Dispatch.Current, (pname, params));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_LightModelfv, cmd_size);
   cmd->pname = MIN2(pname, 0xffff); /* clamped to 0xffff (invalid enum) */
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, params, params_size);
}


/* LightModeli: marshalled asynchronously */
struct marshal_cmd_LightModeli
{
   struct marshal_cmd_base cmd_base;
   GLenum16 pname;
   GLint param;
};
uint32_t
_mesa_unmarshal_LightModeli(struct gl_context *ctx, const struct marshal_cmd_LightModeli *restrict cmd)
{
   GLenum pname = cmd->pname;
   GLint param = cmd->param;
   CALL_LightModeli(ctx->Dispatch.Current, (pname, param));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_LightModeli), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_LightModeli(GLenum pname, GLint param)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_LightModeli);
   struct marshal_cmd_LightModeli *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_LightModeli, cmd_size);
   cmd->pname = MIN2(pname, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->param = param;
}


/* LightModeliv: marshalled asynchronously */
struct marshal_cmd_LightModeliv
{
   struct marshal_cmd_base cmd_base;
   GLenum16 pname;
   /* Next safe_mul(_mesa_light_model_enum_to_count(pname), 1 * sizeof(GLint)) bytes are GLint params[None] */
};
uint32_t
_mesa_unmarshal_LightModeliv(struct gl_context *ctx, const struct marshal_cmd_LightModeliv *restrict cmd)
{
   GLenum pname = cmd->pname;
   GLint *params;
   const char *variable_data = (const char *) (cmd + 1);
   params = (GLint *) variable_data;
   CALL_LightModeliv(ctx->Dispatch.Current, (pname, params));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_LightModeliv(GLenum pname, const GLint *params)
{
   GET_CURRENT_CONTEXT(ctx);
   int params_size = safe_mul(_mesa_light_model_enum_to_count(pname), 1 * sizeof(GLint));
   int cmd_size = sizeof(struct marshal_cmd_LightModeliv) + params_size;
   struct marshal_cmd_LightModeliv *cmd;
   if (unlikely(params_size < 0 || (params_size > 0 && !params) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "LightModeliv");
      CALL_LightModeliv(ctx->Dispatch.Current, (pname, params));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_LightModeliv, cmd_size);
   cmd->pname = MIN2(pname, 0xffff); /* clamped to 0xffff (invalid enum) */
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, params, params_size);
}


/* LineStipple: marshalled asynchronously */
struct marshal_cmd_LineStipple
{
   struct marshal_cmd_base cmd_base;
   GLushort pattern;
   GLint factor;
};
uint32_t
_mesa_unmarshal_LineStipple(struct gl_context *ctx, const struct marshal_cmd_LineStipple *restrict cmd)
{
   GLint factor = cmd->factor;
   GLushort pattern = cmd->pattern;
   CALL_LineStipple(ctx->Dispatch.Current, (factor, pattern));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_LineStipple), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_LineStipple(GLint factor, GLushort pattern)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_LineStipple);
   struct marshal_cmd_LineStipple *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_LineStipple, cmd_size);
   cmd->factor = factor;
   cmd->pattern = pattern;
}


/* LineWidth: marshalled asynchronously */
struct marshal_cmd_LineWidth
{
   struct marshal_cmd_base cmd_base;
   GLfloat width;
};
uint32_t
_mesa_unmarshal_LineWidth(struct gl_context *ctx, const struct marshal_cmd_LineWidth *restrict cmd)
{
   GLfloat width = cmd->width;
   CALL_LineWidth(ctx->Dispatch.Current, (width));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_LineWidth), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_LineWidth(GLfloat width)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_LineWidth);
   struct marshal_cmd_LineWidth *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_LineWidth, cmd_size);
   cmd->width = width;
}


/* Materialf: marshalled asynchronously */
struct marshal_cmd_Materialf
{
   struct marshal_cmd_base cmd_base;
   GLenum16 face;
   GLenum16 pname;
   GLfloat param;
};
uint32_t
_mesa_unmarshal_Materialf(struct gl_context *ctx, const struct marshal_cmd_Materialf *restrict cmd)
{
   GLenum face = cmd->face;
   GLenum pname = cmd->pname;
   GLfloat param = cmd->param;
   CALL_Materialf(ctx->Dispatch.Current, (face, pname, param));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Materialf), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_Materialf(GLenum face, GLenum pname, GLfloat param)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Materialf);
   struct marshal_cmd_Materialf *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Materialf, cmd_size);
   cmd->face = MIN2(face, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->pname = MIN2(pname, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->param = param;
}


/* Materialfv: marshalled asynchronously */
struct marshal_cmd_Materialfv
{
   struct marshal_cmd_base cmd_base;
   GLenum16 face;
   GLenum16 pname;
   /* Next safe_mul(_mesa_material_enum_to_count(pname), 1 * sizeof(GLfloat)) bytes are GLfloat params[None] */
};
uint32_t
_mesa_unmarshal_Materialfv(struct gl_context *ctx, const struct marshal_cmd_Materialfv *restrict cmd)
{
   GLenum face = cmd->face;
   GLenum pname = cmd->pname;
   GLfloat *params;
   const char *variable_data = (const char *) (cmd + 1);
   params = (GLfloat *) variable_data;
   CALL_Materialfv(ctx->Dispatch.Current, (face, pname, params));
   return cmd->cmd_base.cmd_size;
}
void GLAPIENTRY
_mesa_marshal_Materialfv(GLenum face, GLenum pname, const GLfloat *params)
{
   GET_CURRENT_CONTEXT(ctx);
   int params_size = safe_mul(_mesa_material_enum_to_count(pname), 1 * sizeof(GLfloat));
   int cmd_size = sizeof(struct marshal_cmd_Materialfv) + params_size;
   struct marshal_cmd_Materialfv *cmd;
   if (unlikely(params_size < 0 || (params_size > 0 && !params) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "Materialfv");
      CALL_Materialfv(ctx->Dispatch.Current, (face, pname, params));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Materialfv, cmd_size);
   cmd->face = MIN2(face, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->pname = MIN2(pname, 0xffff); /* clamped to 0xffff (invalid enum) */
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, params, params_size);
}


/* Materiali: marshalled asynchronously */
struct marshal_cmd_Materiali
{
   struct marshal_cmd_base cmd_base;
   GLenum16 face;
   GLenum16 pname;
   GLint param;
};
uint32_t
_mesa_unmarshal_Materiali(struct gl_context *ctx, const struct marshal_cmd_Materiali *restrict cmd)
{
   GLenum face = cmd->face;
   GLenum pname = cmd->pname;
   GLint param = cmd->param;
   CALL_Materiali(ctx->Dispatch.Current, (face, pname, param));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Materiali), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
void GLAPIENTRY
_mesa_marshal_Materiali(GLenum face, GLenum pname, GLint param)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Materiali);
   struct marshal_cmd_Materiali *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Materiali, cmd_size);
   cmd->face = MIN2(face, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->pname = MIN2(pname, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->param = param;
}


/* Materialiv: marshalled asynchronously */
struct marshal_cmd_Materialiv
{
   struct marshal_cmd_base cmd_base;
   GLenum16 face;
   GLenum16 pname;
   /* Next safe_mul(_mesa_material_enum_to_count(pname), 1 * sizeof(GLint)) bytes are GLint params[None] */
};
uint32_t
_mesa_unmarshal_Materialiv(struct gl_context *ctx, const struct marshal_cmd_Materialiv *restrict cmd)
{
   GLenum face = cmd->face;
   GLenum pname = cmd->pname;
   GLint *params;
   const char *variable_data = (const char *) (cmd + 1);
   params = (GLint *) variable_data;
   CALL_Materialiv(ctx->Dispatch.Current, (face, pname, params));
   return cmd->cmd_base.cmd_size;
}
void GLAPIENTRY
_mesa_marshal_Materialiv(GLenum face, GLenum pname, const GLint *params)
{
   GET_CURRENT_CONTEXT(ctx);
   int params_size = safe_mul(_mesa_material_enum_to_count(pname), 1 * sizeof(GLint));
   int cmd_size = sizeof(struct marshal_cmd_Materialiv) + params_size;
   struct marshal_cmd_Materialiv *cmd;
   if (unlikely(params_size < 0 || (params_size > 0 && !params) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "Materialiv");
      CALL_Materialiv(ctx->Dispatch.Current, (face, pname, params));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Materialiv, cmd_size);
   cmd->face = MIN2(face, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->pname = MIN2(pname, 0xffff); /* clamped to 0xffff (invalid enum) */
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, params, params_size);
}


/* PointSize: marshalled asynchronously */
struct marshal_cmd_PointSize
{
   struct marshal_cmd_base cmd_base;
   GLfloat size;
};
uint32_t
_mesa_unmarshal_PointSize(struct gl_context *ctx, const struct marshal_cmd_PointSize *restrict cmd)
{
   GLfloat size = cmd->size;
   CALL_PointSize(ctx->Dispatch.Current, (size));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_PointSize), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_PointSize(GLfloat size)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_PointSize);
   struct marshal_cmd_PointSize *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_PointSize, cmd_size);
   cmd->size = size;
}


/* PolygonMode: marshalled asynchronously */
struct marshal_cmd_PolygonMode
{
   struct marshal_cmd_base cmd_base;
   GLenum16 face;
   GLenum16 mode;
};
uint32_t
_mesa_unmarshal_PolygonMode(struct gl_context *ctx, const struct marshal_cmd_PolygonMode *restrict cmd)
{
   GLenum face = cmd->face;
   GLenum mode = cmd->mode;
   CALL_PolygonMode(ctx->Dispatch.Current, (face, mode));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_PolygonMode), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_PolygonMode(GLenum face, GLenum mode)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_PolygonMode);
   struct marshal_cmd_PolygonMode *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_PolygonMode, cmd_size);
   cmd->face = MIN2(face, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->mode = MIN2(mode, 0xffff); /* clamped to 0xffff (invalid enum) */
}


/* PolygonStipple: marshalled asynchronously */
struct marshal_cmd_PolygonStipple
{
   struct marshal_cmd_base cmd_base;
   const GLubyte * mask;
};
uint32_t
_mesa_unmarshal_PolygonStipple(struct gl_context *ctx, const struct marshal_cmd_PolygonStipple *restrict cmd)
{
   const GLubyte * mask = cmd->mask;
   CALL_PolygonStipple(ctx->Dispatch.Current, (mask));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_PolygonStipple), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_PolygonStipple(const GLubyte *mask)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_PolygonStipple);
   struct marshal_cmd_PolygonStipple *cmd;
   if (_mesa_glthread_has_no_unpack_buffer(ctx)) {
      _mesa_glthread_finish_before(ctx, "PolygonStipple");
      CALL_PolygonStipple(ctx->Dispatch.Current, (mask));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_PolygonStipple, cmd_size);
   cmd->mask = mask;
}


/* Scissor: marshalled asynchronously */
struct marshal_cmd_Scissor
{
   struct marshal_cmd_base cmd_base;
   GLint x;
   GLint y;
   GLsizei width;
   GLsizei height;
};
uint32_t
_mesa_unmarshal_Scissor(struct gl_context *ctx, const struct marshal_cmd_Scissor *restrict cmd)
{
   GLint x = cmd->x;
   GLint y = cmd->y;
   GLsizei width = cmd->width;
   GLsizei height = cmd->height;
   CALL_Scissor(ctx->Dispatch.Current, (x, y, width, height));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Scissor), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_Scissor(GLint x, GLint y, GLsizei width, GLsizei height)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Scissor);
   struct marshal_cmd_Scissor *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Scissor, cmd_size);
   cmd->x = x;
   cmd->y = y;
   cmd->width = width;
   cmd->height = height;
}


/* ShadeModel: marshalled asynchronously */
struct marshal_cmd_ShadeModel
{
   struct marshal_cmd_base cmd_base;
   GLenum16 mode;
};
uint32_t
_mesa_unmarshal_ShadeModel(struct gl_context *ctx, const struct marshal_cmd_ShadeModel *restrict cmd)
{
   GLenum mode = cmd->mode;
   CALL_ShadeModel(ctx->Dispatch.Current, (mode));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_ShadeModel), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ShadeModel(GLenum mode)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ShadeModel);
   struct marshal_cmd_ShadeModel *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ShadeModel, cmd_size);
   cmd->mode = MIN2(mode, 0xffff); /* clamped to 0xffff (invalid enum) */
}


/* TexParameterf: marshalled asynchronously */
struct marshal_cmd_TexParameterf
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLenum16 pname;
   GLfloat param;
};
uint32_t
_mesa_unmarshal_TexParameterf(struct gl_context *ctx, const struct marshal_cmd_TexParameterf *restrict cmd)
{
   GLenum target = cmd->target;
   GLenum pname = cmd->pname;
   GLfloat param = cmd->param;
   CALL_TexParameterf(ctx->Dispatch.Current, (target, pname, param));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_TexParameterf), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_TexParameterf(GLenum target, GLenum pname, GLfloat param)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TexParameterf);
   struct marshal_cmd_TexParameterf *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexParameterf, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->pname = MIN2(pname, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->param = param;
}


/* TexParameterfv: marshalled asynchronously */
struct marshal_cmd_TexParameterfv
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLenum16 pname;
   /* Next safe_mul(_mesa_tex_param_enum_to_count(pname), 1 * sizeof(GLfloat)) bytes are GLfloat params[None] */
};
uint32_t
_mesa_unmarshal_TexParameterfv(struct gl_context *ctx, const struct marshal_cmd_TexParameterfv *restrict cmd)
{
   GLenum target = cmd->target;
   GLenum pname = cmd->pname;
   GLfloat *params;
   const char *variable_data = (const char *) (cmd + 1);
   params = (GLfloat *) variable_data;
   CALL_TexParameterfv(ctx->Dispatch.Current, (target, pname, params));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_TexParameterfv(GLenum target, GLenum pname, const GLfloat *params)
{
   GET_CURRENT_CONTEXT(ctx);
   int params_size = safe_mul(_mesa_tex_param_enum_to_count(pname), 1 * sizeof(GLfloat));
   int cmd_size = sizeof(struct marshal_cmd_TexParameterfv) + params_size;
   struct marshal_cmd_TexParameterfv *cmd;
   if (unlikely(params_size < 0 || (params_size > 0 && !params) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "TexParameterfv");
      CALL_TexParameterfv(ctx->Dispatch.Current, (target, pname, params));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexParameterfv, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->pname = MIN2(pname, 0xffff); /* clamped to 0xffff (invalid enum) */
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, params, params_size);
}


/* TexParameteri: marshalled asynchronously */
struct marshal_cmd_TexParameteri
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLenum16 pname;
   GLint param;
};
uint32_t
_mesa_unmarshal_TexParameteri(struct gl_context *ctx, const struct marshal_cmd_TexParameteri *restrict cmd)
{
   GLenum target = cmd->target;
   GLenum pname = cmd->pname;
   GLint param = cmd->param;
   CALL_TexParameteri(ctx->Dispatch.Current, (target, pname, param));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_TexParameteri), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_TexParameteri(GLenum target, GLenum pname, GLint param)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TexParameteri);
   struct marshal_cmd_TexParameteri *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexParameteri, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->pname = MIN2(pname, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->param = param;
}


/* TexParameteriv: marshalled asynchronously */
struct marshal_cmd_TexParameteriv
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLenum16 pname;
   /* Next safe_mul(_mesa_tex_param_enum_to_count(pname), 1 * sizeof(GLint)) bytes are GLint params[None] */
};
uint32_t
_mesa_unmarshal_TexParameteriv(struct gl_context *ctx, const struct marshal_cmd_TexParameteriv *restrict cmd)
{
   GLenum target = cmd->target;
   GLenum pname = cmd->pname;
   GLint *params;
   const char *variable_data = (const char *) (cmd + 1);
   params = (GLint *) variable_data;
   CALL_TexParameteriv(ctx->Dispatch.Current, (target, pname, params));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_TexParameteriv(GLenum target, GLenum pname, const GLint *params)
{
   GET_CURRENT_CONTEXT(ctx);
   int params_size = safe_mul(_mesa_tex_param_enum_to_count(pname), 1 * sizeof(GLint));
   int cmd_size = sizeof(struct marshal_cmd_TexParameteriv) + params_size;
   struct marshal_cmd_TexParameteriv *cmd;
   if (unlikely(params_size < 0 || (params_size > 0 && !params) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "TexParameteriv");
      CALL_TexParameteriv(ctx->Dispatch.Current, (target, pname, params));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexParameteriv, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->pname = MIN2(pname, 0xffff); /* clamped to 0xffff (invalid enum) */
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, params, params_size);
}


/* TexImage1D: marshalled asynchronously */
struct marshal_cmd_TexImage1D
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLenum16 format;
   GLenum16 type;
   GLint level;
   GLint internalformat;
   GLsizei width;
   GLint border;
   const GLvoid * pixels;
};
uint32_t
_mesa_unmarshal_TexImage1D(struct gl_context *ctx, const struct marshal_cmd_TexImage1D *restrict cmd)
{
   GLenum target = cmd->target;
   GLint level = cmd->level;
   GLint internalformat = cmd->internalformat;
   GLsizei width = cmd->width;
   GLint border = cmd->border;
   GLenum format = cmd->format;
   GLenum type = cmd->type;
   const GLvoid * pixels = cmd->pixels;
   CALL_TexImage1D(ctx->Dispatch.Current, (target, level, internalformat, width, border, format, type, pixels));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_TexImage1D), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_TexImage1D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, const GLvoid *pixels)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TexImage1D);
   struct marshal_cmd_TexImage1D *cmd;
   if (_mesa_glthread_has_no_unpack_buffer(ctx)) {
      _mesa_glthread_finish_before(ctx, "TexImage1D");
      CALL_TexImage1D(ctx->Dispatch.Current, (target, level, internalformat, width, border, format, type, pixels));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexImage1D, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->level = level;
   cmd->internalformat = internalformat;
   cmd->width = width;
   cmd->border = border;
   cmd->format = MIN2(format, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->type = MIN2(type, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->pixels = pixels;
}


/* TexImage2D: marshalled asynchronously */
struct marshal_cmd_TexImage2D
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLenum16 format;
   GLenum16 type;
   GLint level;
   GLint internalformat;
   GLsizei width;
   GLsizei height;
   GLint border;
   const GLvoid * pixels;
};
uint32_t
_mesa_unmarshal_TexImage2D(struct gl_context *ctx, const struct marshal_cmd_TexImage2D *restrict cmd)
{
   GLenum target = cmd->target;
   GLint level = cmd->level;
   GLint internalformat = cmd->internalformat;
   GLsizei width = cmd->width;
   GLsizei height = cmd->height;
   GLint border = cmd->border;
   GLenum format = cmd->format;
   GLenum type = cmd->type;
   const GLvoid * pixels = cmd->pixels;
   CALL_TexImage2D(ctx->Dispatch.Current, (target, level, internalformat, width, height, border, format, type, pixels));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_TexImage2D), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_TexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TexImage2D);
   struct marshal_cmd_TexImage2D *cmd;
   if (_mesa_glthread_has_no_unpack_buffer(ctx)) {
      _mesa_glthread_finish_before(ctx, "TexImage2D");
      CALL_TexImage2D(ctx->Dispatch.Current, (target, level, internalformat, width, height, border, format, type, pixels));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexImage2D, cmd_size);
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


/* TexEnvf: marshalled asynchronously */
struct marshal_cmd_TexEnvf
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLenum16 pname;
   GLfloat param;
};
uint32_t
_mesa_unmarshal_TexEnvf(struct gl_context *ctx, const struct marshal_cmd_TexEnvf *restrict cmd)
{
   GLenum target = cmd->target;
   GLenum pname = cmd->pname;
   GLfloat param = cmd->param;
   CALL_TexEnvf(ctx->Dispatch.Current, (target, pname, param));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_TexEnvf), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_TexEnvf(GLenum target, GLenum pname, GLfloat param)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TexEnvf);
   struct marshal_cmd_TexEnvf *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexEnvf, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->pname = MIN2(pname, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->param = param;
}


/* TexEnvfv: marshalled asynchronously */
struct marshal_cmd_TexEnvfv
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLenum16 pname;
   /* Next safe_mul(_mesa_texenv_enum_to_count(pname), 1 * sizeof(GLfloat)) bytes are GLfloat params[None] */
};
uint32_t
_mesa_unmarshal_TexEnvfv(struct gl_context *ctx, const struct marshal_cmd_TexEnvfv *restrict cmd)
{
   GLenum target = cmd->target;
   GLenum pname = cmd->pname;
   GLfloat *params;
   const char *variable_data = (const char *) (cmd + 1);
   params = (GLfloat *) variable_data;
   CALL_TexEnvfv(ctx->Dispatch.Current, (target, pname, params));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_TexEnvfv(GLenum target, GLenum pname, const GLfloat *params)
{
   GET_CURRENT_CONTEXT(ctx);
   int params_size = safe_mul(_mesa_texenv_enum_to_count(pname), 1 * sizeof(GLfloat));
   int cmd_size = sizeof(struct marshal_cmd_TexEnvfv) + params_size;
   struct marshal_cmd_TexEnvfv *cmd;
   if (unlikely(params_size < 0 || (params_size > 0 && !params) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "TexEnvfv");
      CALL_TexEnvfv(ctx->Dispatch.Current, (target, pname, params));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexEnvfv, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->pname = MIN2(pname, 0xffff); /* clamped to 0xffff (invalid enum) */
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, params, params_size);
}


/* TexEnvi: marshalled asynchronously */
struct marshal_cmd_TexEnvi
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLenum16 pname;
   GLint param;
};
uint32_t
_mesa_unmarshal_TexEnvi(struct gl_context *ctx, const struct marshal_cmd_TexEnvi *restrict cmd)
{
   GLenum target = cmd->target;
   GLenum pname = cmd->pname;
   GLint param = cmd->param;
   CALL_TexEnvi(ctx->Dispatch.Current, (target, pname, param));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_TexEnvi), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_TexEnvi(GLenum target, GLenum pname, GLint param)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TexEnvi);
   struct marshal_cmd_TexEnvi *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexEnvi, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->pname = MIN2(pname, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->param = param;
}


/* TexEnviv: marshalled asynchronously */
struct marshal_cmd_TexEnviv
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLenum16 pname;
   /* Next safe_mul(_mesa_texenv_enum_to_count(pname), 1 * sizeof(GLint)) bytes are GLint params[None] */
};
uint32_t
_mesa_unmarshal_TexEnviv(struct gl_context *ctx, const struct marshal_cmd_TexEnviv *restrict cmd)
{
   GLenum target = cmd->target;
   GLenum pname = cmd->pname;
   GLint *params;
   const char *variable_data = (const char *) (cmd + 1);
   params = (GLint *) variable_data;
   CALL_TexEnviv(ctx->Dispatch.Current, (target, pname, params));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_TexEnviv(GLenum target, GLenum pname, const GLint *params)
{
   GET_CURRENT_CONTEXT(ctx);
   int params_size = safe_mul(_mesa_texenv_enum_to_count(pname), 1 * sizeof(GLint));
   int cmd_size = sizeof(struct marshal_cmd_TexEnviv) + params_size;
   struct marshal_cmd_TexEnviv *cmd;
   if (unlikely(params_size < 0 || (params_size > 0 && !params) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "TexEnviv");
      CALL_TexEnviv(ctx->Dispatch.Current, (target, pname, params));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexEnviv, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->pname = MIN2(pname, 0xffff); /* clamped to 0xffff (invalid enum) */
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, params, params_size);
}


/* TexGend: marshalled asynchronously */
struct marshal_cmd_TexGend
{
   struct marshal_cmd_base cmd_base;
   GLenum16 coord;
   GLenum16 pname;
   GLdouble param;
};
uint32_t
_mesa_unmarshal_TexGend(struct gl_context *ctx, const struct marshal_cmd_TexGend *restrict cmd)
{
   GLenum coord = cmd->coord;
   GLenum pname = cmd->pname;
   GLdouble param = cmd->param;
   CALL_TexGend(ctx->Dispatch.Current, (coord, pname, param));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_TexGend), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_TexGend(GLenum coord, GLenum pname, GLdouble param)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TexGend);
   struct marshal_cmd_TexGend *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexGend, cmd_size);
   cmd->coord = MIN2(coord, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->pname = MIN2(pname, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->param = param;
}


/* TexGendv: marshalled asynchronously */
struct marshal_cmd_TexGendv
{
   struct marshal_cmd_base cmd_base;
   GLenum16 coord;
   GLenum16 pname;
   /* Next safe_mul(_mesa_texgen_enum_to_count(pname), 1 * sizeof(GLdouble)) bytes are GLdouble params[None] */
};
uint32_t
_mesa_unmarshal_TexGendv(struct gl_context *ctx, const struct marshal_cmd_TexGendv *restrict cmd)
{
   GLenum coord = cmd->coord;
   GLenum pname = cmd->pname;
   GLdouble *params;
   const char *variable_data = (const char *) (cmd + 1);
   params = (GLdouble *) variable_data;
   CALL_TexGendv(ctx->Dispatch.Current, (coord, pname, params));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_TexGendv(GLenum coord, GLenum pname, const GLdouble *params)
{
   GET_CURRENT_CONTEXT(ctx);
   int params_size = safe_mul(_mesa_texgen_enum_to_count(pname), 1 * sizeof(GLdouble));
   int cmd_size = sizeof(struct marshal_cmd_TexGendv) + params_size;
   struct marshal_cmd_TexGendv *cmd;
   if (unlikely(params_size < 0 || (params_size > 0 && !params) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "TexGendv");
      CALL_TexGendv(ctx->Dispatch.Current, (coord, pname, params));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexGendv, cmd_size);
   cmd->coord = MIN2(coord, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->pname = MIN2(pname, 0xffff); /* clamped to 0xffff (invalid enum) */
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, params, params_size);
}


/* TexGenf: marshalled asynchronously */
struct marshal_cmd_TexGenf
{
   struct marshal_cmd_base cmd_base;
   GLenum16 coord;
   GLenum16 pname;
   GLfloat param;
};
uint32_t
_mesa_unmarshal_TexGenf(struct gl_context *ctx, const struct marshal_cmd_TexGenf *restrict cmd)
{
   GLenum coord = cmd->coord;
   GLenum pname = cmd->pname;
   GLfloat param = cmd->param;
   CALL_TexGenf(ctx->Dispatch.Current, (coord, pname, param));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_TexGenf), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_TexGenf(GLenum coord, GLenum pname, GLfloat param)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TexGenf);
   struct marshal_cmd_TexGenf *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexGenf, cmd_size);
   cmd->coord = MIN2(coord, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->pname = MIN2(pname, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->param = param;
}


/* TexGenfv: marshalled asynchronously */
struct marshal_cmd_TexGenfv
{
   struct marshal_cmd_base cmd_base;
   GLenum16 coord;
   GLenum16 pname;
   /* Next safe_mul(_mesa_texgen_enum_to_count(pname), 1 * sizeof(GLfloat)) bytes are GLfloat params[None] */
};
uint32_t
_mesa_unmarshal_TexGenfv(struct gl_context *ctx, const struct marshal_cmd_TexGenfv *restrict cmd)
{
   GLenum coord = cmd->coord;
   GLenum pname = cmd->pname;
   GLfloat *params;
   const char *variable_data = (const char *) (cmd + 1);
   params = (GLfloat *) variable_data;
   CALL_TexGenfv(ctx->Dispatch.Current, (coord, pname, params));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_TexGenfv(GLenum coord, GLenum pname, const GLfloat *params)
{
   GET_CURRENT_CONTEXT(ctx);
   int params_size = safe_mul(_mesa_texgen_enum_to_count(pname), 1 * sizeof(GLfloat));
   int cmd_size = sizeof(struct marshal_cmd_TexGenfv) + params_size;
   struct marshal_cmd_TexGenfv *cmd;
   if (unlikely(params_size < 0 || (params_size > 0 && !params) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "TexGenfv");
      CALL_TexGenfv(ctx->Dispatch.Current, (coord, pname, params));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexGenfv, cmd_size);
   cmd->coord = MIN2(coord, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->pname = MIN2(pname, 0xffff); /* clamped to 0xffff (invalid enum) */
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, params, params_size);
}


/* TexGeni: marshalled asynchronously */
struct marshal_cmd_TexGeni
{
   struct marshal_cmd_base cmd_base;
   GLenum16 coord;
   GLenum16 pname;
   GLint param;
};
uint32_t
_mesa_unmarshal_TexGeni(struct gl_context *ctx, const struct marshal_cmd_TexGeni *restrict cmd)
{
   GLenum coord = cmd->coord;
   GLenum pname = cmd->pname;
   GLint param = cmd->param;
   CALL_TexGeni(ctx->Dispatch.Current, (coord, pname, param));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_TexGeni), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_TexGeni(GLenum coord, GLenum pname, GLint param)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TexGeni);
   struct marshal_cmd_TexGeni *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexGeni, cmd_size);
   cmd->coord = MIN2(coord, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->pname = MIN2(pname, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->param = param;
}


/* TexGeniv: marshalled asynchronously */
struct marshal_cmd_TexGeniv
{
   struct marshal_cmd_base cmd_base;
   GLenum16 coord;
   GLenum16 pname;
   /* Next safe_mul(_mesa_texgen_enum_to_count(pname), 1 * sizeof(GLint)) bytes are GLint params[None] */
};
uint32_t
_mesa_unmarshal_TexGeniv(struct gl_context *ctx, const struct marshal_cmd_TexGeniv *restrict cmd)
{
   GLenum coord = cmd->coord;
   GLenum pname = cmd->pname;
   GLint *params;
   const char *variable_data = (const char *) (cmd + 1);
   params = (GLint *) variable_data;
   CALL_TexGeniv(ctx->Dispatch.Current, (coord, pname, params));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_TexGeniv(GLenum coord, GLenum pname, const GLint *params)
{
   GET_CURRENT_CONTEXT(ctx);
   int params_size = safe_mul(_mesa_texgen_enum_to_count(pname), 1 * sizeof(GLint));
   int cmd_size = sizeof(struct marshal_cmd_TexGeniv) + params_size;
   struct marshal_cmd_TexGeniv *cmd;
   if (unlikely(params_size < 0 || (params_size > 0 && !params) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "TexGeniv");
      CALL_TexGeniv(ctx->Dispatch.Current, (coord, pname, params));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexGeniv, cmd_size);
   cmd->coord = MIN2(coord, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->pname = MIN2(pname, 0xffff); /* clamped to 0xffff (invalid enum) */
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, params, params_size);
}


/* FeedbackBuffer: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_FeedbackBuffer(GLsizei size, GLenum type, GLfloat *buffer)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "FeedbackBuffer");
   CALL_FeedbackBuffer(ctx->Dispatch.Current, (size, type, buffer));
}


/* SelectBuffer: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_SelectBuffer(GLsizei size, GLuint *buffer)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "SelectBuffer");
   CALL_SelectBuffer(ctx->Dispatch.Current, (size, buffer));
}


void
_mesa_glthread_init_dispatch0(struct gl_context *ctx, struct _glapi_table *table)
{
   if (_mesa_is_desktop_gl(ctx)) {
      SET_MultiDrawArraysIndirect(table, _mesa_marshal_MultiDrawArraysIndirect);
      SET_MultiDrawArraysIndirectCountARB(table, _mesa_marshal_MultiDrawArraysIndirectCountARB);
      SET_MultiDrawElementsIndirect(table, _mesa_marshal_MultiDrawElementsIndirect);
      SET_MultiDrawElementsIndirectCountARB(table, _mesa_marshal_MultiDrawElementsIndirectCountARB);
      SET_MultiModeDrawArraysIBM(table, _mesa_marshal_MultiModeDrawArraysIBM);
      SET_MultiModeDrawElementsIBM(table, _mesa_marshal_MultiModeDrawElementsIBM);
      SET_NamedBufferDataEXT(table, _mesa_marshal_NamedBufferDataEXT);
      SET_NamedBufferSubDataEXT(table, _mesa_marshal_NamedBufferSubDataEXT);
      SET_PolygonMode(table, _mesa_marshal_PolygonMode);
      SET_TexImage1D(table, _mesa_marshal_TexImage1D);
   }
   if (_mesa_is_desktop_gl(ctx) || (_mesa_is_gles2(ctx) && ctx->Version >= 30)) {
      SET_DrawElementsInstancedBaseInstance(table, _mesa_marshal_DrawElementsInstancedBaseInstance);
      SET_DrawRangeElements(table, _mesa_marshal_DrawRangeElements);
   }
   if (_mesa_is_desktop_gl(ctx) || (_mesa_is_gles2(ctx) && ctx->Version >= 31)) {
      SET_DrawArraysIndirect(table, _mesa_marshal_DrawArraysIndirect);
      SET_DrawElementsIndirect(table, _mesa_marshal_DrawElementsIndirect);
   }
   if (_mesa_is_desktop_gl(ctx) || _mesa_is_gles1(ctx)) {
      SET_PointSize(table, _mesa_marshal_PointSize);
   }
   if (_mesa_is_desktop_gl(ctx) || _mesa_is_gles1(ctx) || (_mesa_is_gles2(ctx) && ctx->Version >= 30)) {
      SET_DrawArraysInstancedBaseInstance(table, _mesa_marshal_DrawArraysInstancedBaseInstance);
      SET_DrawElementsInstancedBaseVertexBaseInstance(table, _mesa_marshal_DrawElementsInstancedBaseVertexBaseInstance);
      SET_DrawRangeElementsBaseVertex(table, _mesa_marshal_DrawRangeElementsBaseVertex);
   }
   if (_mesa_is_desktop_gl(ctx) || _mesa_is_gles1(ctx) || _mesa_is_gles2(ctx)) {
      SET_BindBuffer(table, _mesa_marshal_BindBuffer);
      SET_BufferData(table, _mesa_marshal_BufferData);
      SET_BufferSubData(table, _mesa_marshal_BufferSubData);
      SET_CullFace(table, _mesa_marshal_CullFace);
      SET_DrawArrays(table, _mesa_marshal_DrawArrays);
      SET_DrawArraysInstancedBaseInstanceDrawID(table, _mesa_marshal_DrawArraysInstancedBaseInstanceDrawID);
      SET_DrawArraysUserBuf(table, _mesa_marshal_DrawArraysUserBuf);
      SET_DrawElements(table, _mesa_marshal_DrawElements);
      SET_DrawElementsInstancedBaseVertexBaseInstanceDrawID(table, _mesa_marshal_DrawElementsInstancedBaseVertexBaseInstanceDrawID);
      SET_DrawElementsUserBuf(table, _mesa_marshal_DrawElementsUserBuf);
      SET_FrontFace(table, _mesa_marshal_FrontFace);
      SET_GetIntegerv(table, _mesa_marshal_GetIntegerv);
      SET_Hint(table, _mesa_marshal_Hint);
      SET_LineWidth(table, _mesa_marshal_LineWidth);
      SET_MultiDrawArrays(table, _mesa_marshal_MultiDrawArrays);
      SET_MultiDrawArraysUserBuf(table, _mesa_marshal_MultiDrawArraysUserBuf);
      SET_MultiDrawElements(table, _mesa_marshal_MultiDrawElements);
      SET_MultiDrawElementsBaseVertex(table, _mesa_marshal_MultiDrawElementsBaseVertex);
      SET_MultiDrawElementsUserBuf(table, _mesa_marshal_MultiDrawElementsUserBuf);
      SET_Scissor(table, _mesa_marshal_Scissor);
      SET_TexImage2D(table, _mesa_marshal_TexImage2D);
      SET_TexParameterf(table, _mesa_marshal_TexParameterf);
      SET_TexParameterfv(table, _mesa_marshal_TexParameterfv);
      SET_TexParameteri(table, _mesa_marshal_TexParameteri);
      SET_TexParameteriv(table, _mesa_marshal_TexParameteriv);
   }
   if (_mesa_is_desktop_gl(ctx) || _mesa_is_gles2(ctx)) {
      SET_DrawArraysInstanced(table, _mesa_marshal_DrawArraysInstanced);
      SET_DrawElementsBaseVertex(table, _mesa_marshal_DrawElementsBaseVertex);
      SET_DrawElementsInstanced(table, _mesa_marshal_DrawElementsInstanced);
      SET_DrawElementsInstancedBaseVertex(table, _mesa_marshal_DrawElementsInstancedBaseVertex);
      SET_GetActiveUniform(table, _mesa_marshal_GetActiveUniform);
      SET_GetUniformLocation(table, _mesa_marshal_GetUniformLocation);
   }
   if (_mesa_is_desktop_gl_compat(ctx)) {
      SET_Begin(table, _mesa_marshal_Begin);
      SET_Bitmap(table, _mesa_marshal_Bitmap);
      SET_CallList(table, _mesa_marshal_CallList);
      SET_CallLists(table, _mesa_marshal_CallLists);
      SET_ClipPlane(table, _mesa_marshal_ClipPlane);
      SET_Color3b(table, _mesa_marshal_Color3b);
      SET_Color3bv(table, _mesa_marshal_Color3bv);
      SET_Color3d(table, _mesa_marshal_Color3d);
      SET_Color3dv(table, _mesa_marshal_Color3dv);
      SET_Color3f(table, _mesa_marshal_Color3f);
      SET_Color3fv(table, _mesa_marshal_Color3fv);
      SET_Color3i(table, _mesa_marshal_Color3i);
      SET_Color3iv(table, _mesa_marshal_Color3iv);
      SET_Color3s(table, _mesa_marshal_Color3s);
      SET_Color3sv(table, _mesa_marshal_Color3sv);
      SET_Color3ub(table, _mesa_marshal_Color3ub);
      SET_Color3ubv(table, _mesa_marshal_Color3ubv);
      SET_Color3ui(table, _mesa_marshal_Color3ui);
      SET_Color3uiv(table, _mesa_marshal_Color3uiv);
      SET_Color3us(table, _mesa_marshal_Color3us);
      SET_Color3usv(table, _mesa_marshal_Color3usv);
      SET_Color4b(table, _mesa_marshal_Color4b);
      SET_Color4bv(table, _mesa_marshal_Color4bv);
      SET_Color4d(table, _mesa_marshal_Color4d);
      SET_Color4dv(table, _mesa_marshal_Color4dv);
      SET_Color4fv(table, _mesa_marshal_Color4fv);
      SET_Color4i(table, _mesa_marshal_Color4i);
      SET_Color4iv(table, _mesa_marshal_Color4iv);
      SET_Color4s(table, _mesa_marshal_Color4s);
      SET_Color4sv(table, _mesa_marshal_Color4sv);
      SET_Color4ubv(table, _mesa_marshal_Color4ubv);
      SET_Color4ui(table, _mesa_marshal_Color4ui);
      SET_Color4uiv(table, _mesa_marshal_Color4uiv);
      SET_Color4us(table, _mesa_marshal_Color4us);
      SET_Color4usv(table, _mesa_marshal_Color4usv);
      SET_ColorMaterial(table, _mesa_marshal_ColorMaterial);
      SET_DeleteLists(table, _mesa_marshal_DeleteLists);
      SET_EdgeFlag(table, _mesa_marshal_EdgeFlag);
      SET_EdgeFlagv(table, _mesa_marshal_EdgeFlagv);
      SET_End(table, _mesa_marshal_End);
      SET_EndList(table, _mesa_marshal_EndList);
      SET_FeedbackBuffer(table, _mesa_marshal_FeedbackBuffer);
      SET_Fogi(table, _mesa_marshal_Fogi);
      SET_Fogiv(table, _mesa_marshal_Fogiv);
      SET_GenLists(table, _mesa_marshal_GenLists);
      SET_Indexd(table, _mesa_marshal_Indexd);
      SET_Indexdv(table, _mesa_marshal_Indexdv);
      SET_Indexf(table, _mesa_marshal_Indexf);
      SET_Indexfv(table, _mesa_marshal_Indexfv);
      SET_Indexi(table, _mesa_marshal_Indexi);
      SET_Indexiv(table, _mesa_marshal_Indexiv);
      SET_Indexs(table, _mesa_marshal_Indexs);
      SET_Indexsv(table, _mesa_marshal_Indexsv);
      SET_LightModeli(table, _mesa_marshal_LightModeli);
      SET_LightModeliv(table, _mesa_marshal_LightModeliv);
      SET_Lighti(table, _mesa_marshal_Lighti);
      SET_Lightiv(table, _mesa_marshal_Lightiv);
      SET_LineStipple(table, _mesa_marshal_LineStipple);
      SET_ListBase(table, _mesa_marshal_ListBase);
      SET_Materiali(table, _mesa_marshal_Materiali);
      SET_Materialiv(table, _mesa_marshal_Materialiv);
      SET_NewList(table, _mesa_marshal_NewList);
      SET_Normal3b(table, _mesa_marshal_Normal3b);
      SET_Normal3bv(table, _mesa_marshal_Normal3bv);
      SET_Normal3d(table, _mesa_marshal_Normal3d);
      SET_Normal3dv(table, _mesa_marshal_Normal3dv);
      SET_Normal3fv(table, _mesa_marshal_Normal3fv);
      SET_Normal3i(table, _mesa_marshal_Normal3i);
      SET_Normal3iv(table, _mesa_marshal_Normal3iv);
      SET_Normal3s(table, _mesa_marshal_Normal3s);
      SET_Normal3sv(table, _mesa_marshal_Normal3sv);
      SET_PolygonStipple(table, _mesa_marshal_PolygonStipple);
      SET_RasterPos2d(table, _mesa_marshal_RasterPos2d);
      SET_RasterPos2dv(table, _mesa_marshal_RasterPos2dv);
      SET_RasterPos2f(table, _mesa_marshal_RasterPos2f);
      SET_RasterPos2fv(table, _mesa_marshal_RasterPos2fv);
      SET_RasterPos2i(table, _mesa_marshal_RasterPos2i);
      SET_RasterPos2iv(table, _mesa_marshal_RasterPos2iv);
      SET_RasterPos2s(table, _mesa_marshal_RasterPos2s);
      SET_RasterPos2sv(table, _mesa_marshal_RasterPos2sv);
      SET_RasterPos3d(table, _mesa_marshal_RasterPos3d);
      SET_RasterPos3dv(table, _mesa_marshal_RasterPos3dv);
      SET_RasterPos3f(table, _mesa_marshal_RasterPos3f);
      SET_RasterPos3fv(table, _mesa_marshal_RasterPos3fv);
      SET_RasterPos3i(table, _mesa_marshal_RasterPos3i);
      SET_RasterPos3iv(table, _mesa_marshal_RasterPos3iv);
      SET_RasterPos3s(table, _mesa_marshal_RasterPos3s);
      SET_RasterPos3sv(table, _mesa_marshal_RasterPos3sv);
      SET_RasterPos4d(table, _mesa_marshal_RasterPos4d);
      SET_RasterPos4dv(table, _mesa_marshal_RasterPos4dv);
      SET_RasterPos4f(table, _mesa_marshal_RasterPos4f);
      SET_RasterPos4fv(table, _mesa_marshal_RasterPos4fv);
      SET_RasterPos4i(table, _mesa_marshal_RasterPos4i);
      SET_RasterPos4iv(table, _mesa_marshal_RasterPos4iv);
      SET_RasterPos4s(table, _mesa_marshal_RasterPos4s);
      SET_RasterPos4sv(table, _mesa_marshal_RasterPos4sv);
      SET_Rectd(table, _mesa_marshal_Rectd);
      SET_Rectdv(table, _mesa_marshal_Rectdv);
      SET_Rectf(table, _mesa_marshal_Rectf);
      SET_Rectfv(table, _mesa_marshal_Rectfv);
      SET_Recti(table, _mesa_marshal_Recti);
      SET_Rectiv(table, _mesa_marshal_Rectiv);
      SET_Rects(table, _mesa_marshal_Rects);
      SET_Rectsv(table, _mesa_marshal_Rectsv);
      SET_SelectBuffer(table, _mesa_marshal_SelectBuffer);
      SET_TexCoord1d(table, _mesa_marshal_TexCoord1d);
      SET_TexCoord1dv(table, _mesa_marshal_TexCoord1dv);
      SET_TexCoord1f(table, _mesa_marshal_TexCoord1f);
      SET_TexCoord1fv(table, _mesa_marshal_TexCoord1fv);
      SET_TexCoord1i(table, _mesa_marshal_TexCoord1i);
      SET_TexCoord1iv(table, _mesa_marshal_TexCoord1iv);
      SET_TexCoord1s(table, _mesa_marshal_TexCoord1s);
      SET_TexCoord1sv(table, _mesa_marshal_TexCoord1sv);
      SET_TexCoord2d(table, _mesa_marshal_TexCoord2d);
      SET_TexCoord2dv(table, _mesa_marshal_TexCoord2dv);
      SET_TexCoord2f(table, _mesa_marshal_TexCoord2f);
      SET_TexCoord2fv(table, _mesa_marshal_TexCoord2fv);
      SET_TexCoord2i(table, _mesa_marshal_TexCoord2i);
      SET_TexCoord2iv(table, _mesa_marshal_TexCoord2iv);
      SET_TexCoord2s(table, _mesa_marshal_TexCoord2s);
      SET_TexCoord2sv(table, _mesa_marshal_TexCoord2sv);
      SET_TexCoord3d(table, _mesa_marshal_TexCoord3d);
      SET_TexCoord3dv(table, _mesa_marshal_TexCoord3dv);
      SET_TexCoord3f(table, _mesa_marshal_TexCoord3f);
      SET_TexCoord3fv(table, _mesa_marshal_TexCoord3fv);
      SET_TexCoord3i(table, _mesa_marshal_TexCoord3i);
      SET_TexCoord3iv(table, _mesa_marshal_TexCoord3iv);
      SET_TexCoord3s(table, _mesa_marshal_TexCoord3s);
      SET_TexCoord3sv(table, _mesa_marshal_TexCoord3sv);
      SET_TexCoord4d(table, _mesa_marshal_TexCoord4d);
      SET_TexCoord4dv(table, _mesa_marshal_TexCoord4dv);
      SET_TexCoord4f(table, _mesa_marshal_TexCoord4f);
      SET_TexCoord4fv(table, _mesa_marshal_TexCoord4fv);
      SET_TexCoord4i(table, _mesa_marshal_TexCoord4i);
      SET_TexCoord4iv(table, _mesa_marshal_TexCoord4iv);
      SET_TexCoord4s(table, _mesa_marshal_TexCoord4s);
      SET_TexCoord4sv(table, _mesa_marshal_TexCoord4sv);
      SET_TexGend(table, _mesa_marshal_TexGend);
      SET_TexGendv(table, _mesa_marshal_TexGendv);
      SET_Vertex2d(table, _mesa_marshal_Vertex2d);
      SET_Vertex2dv(table, _mesa_marshal_Vertex2dv);
      SET_Vertex2f(table, _mesa_marshal_Vertex2f);
      SET_Vertex2fv(table, _mesa_marshal_Vertex2fv);
      SET_Vertex2i(table, _mesa_marshal_Vertex2i);
      SET_Vertex2iv(table, _mesa_marshal_Vertex2iv);
      SET_Vertex2s(table, _mesa_marshal_Vertex2s);
      SET_Vertex2sv(table, _mesa_marshal_Vertex2sv);
      SET_Vertex3d(table, _mesa_marshal_Vertex3d);
      SET_Vertex3dv(table, _mesa_marshal_Vertex3dv);
      SET_Vertex3f(table, _mesa_marshal_Vertex3f);
      SET_Vertex3fv(table, _mesa_marshal_Vertex3fv);
      SET_Vertex3i(table, _mesa_marshal_Vertex3i);
      SET_Vertex3iv(table, _mesa_marshal_Vertex3iv);
      SET_Vertex3s(table, _mesa_marshal_Vertex3s);
      SET_Vertex3sv(table, _mesa_marshal_Vertex3sv);
      SET_Vertex4d(table, _mesa_marshal_Vertex4d);
      SET_Vertex4dv(table, _mesa_marshal_Vertex4dv);
      SET_Vertex4f(table, _mesa_marshal_Vertex4f);
      SET_Vertex4fv(table, _mesa_marshal_Vertex4fv);
      SET_Vertex4i(table, _mesa_marshal_Vertex4i);
      SET_Vertex4iv(table, _mesa_marshal_Vertex4iv);
      SET_Vertex4s(table, _mesa_marshal_Vertex4s);
      SET_Vertex4sv(table, _mesa_marshal_Vertex4sv);
   }
   if (_mesa_is_desktop_gl_compat(ctx) || _mesa_is_desktop_gl_core(ctx)) {
      SET_NamedBufferData(table, _mesa_marshal_NamedBufferData);
      SET_NamedBufferSubData(table, _mesa_marshal_NamedBufferSubData);
   }
   if (_mesa_is_desktop_gl_compat(ctx) || _mesa_is_gles1(ctx)) {
      SET_Color4f(table, _mesa_marshal_Color4f);
      SET_Color4ub(table, _mesa_marshal_Color4ub);
      SET_Fogf(table, _mesa_marshal_Fogf);
      SET_Fogfv(table, _mesa_marshal_Fogfv);
      SET_LightModelf(table, _mesa_marshal_LightModelf);
      SET_LightModelfv(table, _mesa_marshal_LightModelfv);
      SET_Lightf(table, _mesa_marshal_Lightf);
      SET_Lightfv(table, _mesa_marshal_Lightfv);
      SET_Materialf(table, _mesa_marshal_Materialf);
      SET_Materialfv(table, _mesa_marshal_Materialfv);
      SET_Normal3f(table, _mesa_marshal_Normal3f);
      SET_ShadeModel(table, _mesa_marshal_ShadeModel);
      SET_TexEnvf(table, _mesa_marshal_TexEnvf);
      SET_TexEnvfv(table, _mesa_marshal_TexEnvfv);
      SET_TexEnvi(table, _mesa_marshal_TexEnvi);
      SET_TexEnviv(table, _mesa_marshal_TexEnviv);
      SET_TexGenf(table, _mesa_marshal_TexGenf);
      SET_TexGenfv(table, _mesa_marshal_TexGenfv);
      SET_TexGeni(table, _mesa_marshal_TexGeni);
      SET_TexGeniv(table, _mesa_marshal_TexGeniv);
   }
}
