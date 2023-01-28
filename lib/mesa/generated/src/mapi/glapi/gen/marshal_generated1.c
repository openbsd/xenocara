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

/* InitNames: marshalled asynchronously */
struct marshal_cmd_InitNames
{
   struct marshal_cmd_base cmd_base;
};
uint32_t
_mesa_unmarshal_InitNames(struct gl_context *ctx, const struct marshal_cmd_InitNames *cmd)
{
   CALL_InitNames(ctx->CurrentServerDispatch, ());
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_InitNames), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_InitNames(void)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_InitNames);
   struct marshal_cmd_InitNames *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_InitNames, cmd_size);
   (void) cmd;
}


/* LoadName: marshalled asynchronously */
struct marshal_cmd_LoadName
{
   struct marshal_cmd_base cmd_base;
   GLuint name;
};
uint32_t
_mesa_unmarshal_LoadName(struct gl_context *ctx, const struct marshal_cmd_LoadName *cmd)
{
   GLuint name = cmd->name;
   CALL_LoadName(ctx->CurrentServerDispatch, (name));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_LoadName), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_LoadName(GLuint name)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_LoadName);
   struct marshal_cmd_LoadName *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_LoadName, cmd_size);
   cmd->name = name;
}


/* PassThrough: marshalled asynchronously */
struct marshal_cmd_PassThrough
{
   struct marshal_cmd_base cmd_base;
   GLfloat token;
};
uint32_t
_mesa_unmarshal_PassThrough(struct gl_context *ctx, const struct marshal_cmd_PassThrough *cmd)
{
   GLfloat token = cmd->token;
   CALL_PassThrough(ctx->CurrentServerDispatch, (token));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_PassThrough), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_PassThrough(GLfloat token)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_PassThrough);
   struct marshal_cmd_PassThrough *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_PassThrough, cmd_size);
   cmd->token = token;
}


/* PopName: marshalled asynchronously */
struct marshal_cmd_PopName
{
   struct marshal_cmd_base cmd_base;
};
uint32_t
_mesa_unmarshal_PopName(struct gl_context *ctx, const struct marshal_cmd_PopName *cmd)
{
   CALL_PopName(ctx->CurrentServerDispatch, ());
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_PopName), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_PopName(void)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_PopName);
   struct marshal_cmd_PopName *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_PopName, cmd_size);
   (void) cmd;
}


/* PushName: marshalled asynchronously */
struct marshal_cmd_PushName
{
   struct marshal_cmd_base cmd_base;
   GLuint name;
};
uint32_t
_mesa_unmarshal_PushName(struct gl_context *ctx, const struct marshal_cmd_PushName *cmd)
{
   GLuint name = cmd->name;
   CALL_PushName(ctx->CurrentServerDispatch, (name));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_PushName), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_PushName(GLuint name)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_PushName);
   struct marshal_cmd_PushName *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_PushName, cmd_size);
   cmd->name = name;
}


/* DrawBuffer: marshalled asynchronously */
struct marshal_cmd_DrawBuffer
{
   struct marshal_cmd_base cmd_base;
   GLenum16 mode;
};
uint32_t
_mesa_unmarshal_DrawBuffer(struct gl_context *ctx, const struct marshal_cmd_DrawBuffer *cmd)
{
   GLenum mode = cmd->mode;
   CALL_DrawBuffer(ctx->CurrentServerDispatch, (mode));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_DrawBuffer), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_DrawBuffer(GLenum mode)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_DrawBuffer);
   struct marshal_cmd_DrawBuffer *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_DrawBuffer, cmd_size);
   cmd->mode = MIN2(mode, 0xffff); /* clamped to 0xffff (invalid enum) */
}


/* Clear: marshalled asynchronously */
struct marshal_cmd_Clear
{
   struct marshal_cmd_base cmd_base;
   GLbitfield mask;
};
uint32_t
_mesa_unmarshal_Clear(struct gl_context *ctx, const struct marshal_cmd_Clear *cmd)
{
   GLbitfield mask = cmd->mask;
   CALL_Clear(ctx->CurrentServerDispatch, (mask));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Clear), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_Clear(GLbitfield mask)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Clear);
   struct marshal_cmd_Clear *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Clear, cmd_size);
   cmd->mask = mask;
}


/* ClearAccum: marshalled asynchronously */
struct marshal_cmd_ClearAccum
{
   struct marshal_cmd_base cmd_base;
   GLfloat red;
   GLfloat green;
   GLfloat blue;
   GLfloat alpha;
};
uint32_t
_mesa_unmarshal_ClearAccum(struct gl_context *ctx, const struct marshal_cmd_ClearAccum *cmd)
{
   GLfloat red = cmd->red;
   GLfloat green = cmd->green;
   GLfloat blue = cmd->blue;
   GLfloat alpha = cmd->alpha;
   CALL_ClearAccum(ctx->CurrentServerDispatch, (red, green, blue, alpha));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_ClearAccum), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ClearAccum(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ClearAccum);
   struct marshal_cmd_ClearAccum *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ClearAccum, cmd_size);
   cmd->red = red;
   cmd->green = green;
   cmd->blue = blue;
   cmd->alpha = alpha;
}


/* ClearIndex: marshalled asynchronously */
struct marshal_cmd_ClearIndex
{
   struct marshal_cmd_base cmd_base;
   GLfloat c;
};
uint32_t
_mesa_unmarshal_ClearIndex(struct gl_context *ctx, const struct marshal_cmd_ClearIndex *cmd)
{
   GLfloat c = cmd->c;
   CALL_ClearIndex(ctx->CurrentServerDispatch, (c));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_ClearIndex), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ClearIndex(GLfloat c)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ClearIndex);
   struct marshal_cmd_ClearIndex *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ClearIndex, cmd_size);
   cmd->c = c;
}


/* ClearColor: marshalled asynchronously */
struct marshal_cmd_ClearColor
{
   struct marshal_cmd_base cmd_base;
   GLclampf red;
   GLclampf green;
   GLclampf blue;
   GLclampf alpha;
};
uint32_t
_mesa_unmarshal_ClearColor(struct gl_context *ctx, const struct marshal_cmd_ClearColor *cmd)
{
   GLclampf red = cmd->red;
   GLclampf green = cmd->green;
   GLclampf blue = cmd->blue;
   GLclampf alpha = cmd->alpha;
   CALL_ClearColor(ctx->CurrentServerDispatch, (red, green, blue, alpha));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_ClearColor), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ClearColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ClearColor);
   struct marshal_cmd_ClearColor *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ClearColor, cmd_size);
   cmd->red = red;
   cmd->green = green;
   cmd->blue = blue;
   cmd->alpha = alpha;
}


/* ClearStencil: marshalled asynchronously */
struct marshal_cmd_ClearStencil
{
   struct marshal_cmd_base cmd_base;
   GLint s;
};
uint32_t
_mesa_unmarshal_ClearStencil(struct gl_context *ctx, const struct marshal_cmd_ClearStencil *cmd)
{
   GLint s = cmd->s;
   CALL_ClearStencil(ctx->CurrentServerDispatch, (s));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_ClearStencil), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ClearStencil(GLint s)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ClearStencil);
   struct marshal_cmd_ClearStencil *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ClearStencil, cmd_size);
   cmd->s = s;
}


/* ClearDepth: marshalled asynchronously */
struct marshal_cmd_ClearDepth
{
   struct marshal_cmd_base cmd_base;
   GLclampd depth;
};
uint32_t
_mesa_unmarshal_ClearDepth(struct gl_context *ctx, const struct marshal_cmd_ClearDepth *cmd)
{
   GLclampd depth = cmd->depth;
   CALL_ClearDepth(ctx->CurrentServerDispatch, (depth));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_ClearDepth), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ClearDepth(GLclampd depth)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ClearDepth);
   struct marshal_cmd_ClearDepth *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ClearDepth, cmd_size);
   cmd->depth = depth;
}


/* StencilMask: marshalled asynchronously */
struct marshal_cmd_StencilMask
{
   struct marshal_cmd_base cmd_base;
   GLuint mask;
};
uint32_t
_mesa_unmarshal_StencilMask(struct gl_context *ctx, const struct marshal_cmd_StencilMask *cmd)
{
   GLuint mask = cmd->mask;
   CALL_StencilMask(ctx->CurrentServerDispatch, (mask));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_StencilMask), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_StencilMask(GLuint mask)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_StencilMask);
   struct marshal_cmd_StencilMask *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_StencilMask, cmd_size);
   cmd->mask = mask;
}


/* ColorMask: marshalled asynchronously */
struct marshal_cmd_ColorMask
{
   struct marshal_cmd_base cmd_base;
   GLboolean red;
   GLboolean green;
   GLboolean blue;
   GLboolean alpha;
};
uint32_t
_mesa_unmarshal_ColorMask(struct gl_context *ctx, const struct marshal_cmd_ColorMask *cmd)
{
   GLboolean red = cmd->red;
   GLboolean green = cmd->green;
   GLboolean blue = cmd->blue;
   GLboolean alpha = cmd->alpha;
   CALL_ColorMask(ctx->CurrentServerDispatch, (red, green, blue, alpha));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_ColorMask), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ColorMask(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ColorMask);
   struct marshal_cmd_ColorMask *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ColorMask, cmd_size);
   cmd->red = red;
   cmd->green = green;
   cmd->blue = blue;
   cmd->alpha = alpha;
}


/* DepthMask: marshalled asynchronously */
struct marshal_cmd_DepthMask
{
   struct marshal_cmd_base cmd_base;
   GLboolean flag;
};
uint32_t
_mesa_unmarshal_DepthMask(struct gl_context *ctx, const struct marshal_cmd_DepthMask *cmd)
{
   GLboolean flag = cmd->flag;
   CALL_DepthMask(ctx->CurrentServerDispatch, (flag));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_DepthMask), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_DepthMask(GLboolean flag)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_DepthMask);
   struct marshal_cmd_DepthMask *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_DepthMask, cmd_size);
   cmd->flag = flag;
}


/* IndexMask: marshalled asynchronously */
struct marshal_cmd_IndexMask
{
   struct marshal_cmd_base cmd_base;
   GLuint mask;
};
uint32_t
_mesa_unmarshal_IndexMask(struct gl_context *ctx, const struct marshal_cmd_IndexMask *cmd)
{
   GLuint mask = cmd->mask;
   CALL_IndexMask(ctx->CurrentServerDispatch, (mask));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_IndexMask), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_IndexMask(GLuint mask)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_IndexMask);
   struct marshal_cmd_IndexMask *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_IndexMask, cmd_size);
   cmd->mask = mask;
}


/* Accum: marshalled asynchronously */
struct marshal_cmd_Accum
{
   struct marshal_cmd_base cmd_base;
   GLenum16 op;
   GLfloat value;
};
uint32_t
_mesa_unmarshal_Accum(struct gl_context *ctx, const struct marshal_cmd_Accum *cmd)
{
   GLenum op = cmd->op;
   GLfloat value = cmd->value;
   CALL_Accum(ctx->CurrentServerDispatch, (op, value));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Accum), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_Accum(GLenum op, GLfloat value)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Accum);
   struct marshal_cmd_Accum *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Accum, cmd_size);
   cmd->op = MIN2(op, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->value = value;
}


/* Disable: marshalled asynchronously */
struct marshal_cmd_Disable
{
   struct marshal_cmd_base cmd_base;
   GLenum16 cap;
};
uint32_t
_mesa_unmarshal_Disable(struct gl_context *ctx, const struct marshal_cmd_Disable *cmd)
{
   GLenum cap = cmd->cap;
   CALL_Disable(ctx->CurrentServerDispatch, (cap));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Disable), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_Disable(GLenum cap)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Disable);
   struct marshal_cmd_Disable *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Disable, cmd_size);
   cmd->cap = MIN2(cap, 0xffff); /* clamped to 0xffff (invalid enum) */
   _mesa_glthread_Disable(ctx, cap);
}


/* Enable: marshalled asynchronously */
struct marshal_cmd_Enable
{
   struct marshal_cmd_base cmd_base;
   GLenum16 cap;
};
uint32_t
_mesa_unmarshal_Enable(struct gl_context *ctx, const struct marshal_cmd_Enable *cmd)
{
   GLenum cap = cmd->cap;
   CALL_Enable(ctx->CurrentServerDispatch, (cap));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Enable), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_Enable(GLenum cap)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Enable);
   struct marshal_cmd_Enable *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Enable, cmd_size);
   cmd->cap = MIN2(cap, 0xffff); /* clamped to 0xffff (invalid enum) */
   _mesa_glthread_Enable(ctx, cap);
}


/* Finish: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_Finish(void)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "Finish");
   CALL_Finish(ctx->CurrentServerDispatch, ());
}


/* Flush: marshalled asynchronously */
struct marshal_cmd_Flush
{
   struct marshal_cmd_base cmd_base;
};
uint32_t
_mesa_unmarshal_Flush(struct gl_context *ctx, const struct marshal_cmd_Flush *cmd)
{
   CALL_Flush(ctx->CurrentServerDispatch, ());
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Flush), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_Flush(void)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Flush);
   struct marshal_cmd_Flush *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Flush, cmd_size);
   (void) cmd;
   _mesa_glthread_flush_batch(ctx); if (ctx->Shared->HasExternallySharedImages) _mesa_glthread_finish(ctx);
}


/* PopAttrib: marshalled asynchronously */
struct marshal_cmd_PopAttrib
{
   struct marshal_cmd_base cmd_base;
};
uint32_t
_mesa_unmarshal_PopAttrib(struct gl_context *ctx, const struct marshal_cmd_PopAttrib *cmd)
{
   CALL_PopAttrib(ctx->CurrentServerDispatch, ());
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_PopAttrib), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_PopAttrib(void)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_PopAttrib);
   struct marshal_cmd_PopAttrib *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_PopAttrib, cmd_size);
   (void) cmd;
   _mesa_glthread_PopAttrib(ctx);
}


/* PushAttrib: marshalled asynchronously */
struct marshal_cmd_PushAttrib
{
   struct marshal_cmd_base cmd_base;
   GLbitfield mask;
};
uint32_t
_mesa_unmarshal_PushAttrib(struct gl_context *ctx, const struct marshal_cmd_PushAttrib *cmd)
{
   GLbitfield mask = cmd->mask;
   CALL_PushAttrib(ctx->CurrentServerDispatch, (mask));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_PushAttrib), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_PushAttrib(GLbitfield mask)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_PushAttrib);
   struct marshal_cmd_PushAttrib *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_PushAttrib, cmd_size);
   cmd->mask = mask;
   _mesa_glthread_PushAttrib(ctx, mask);
}


/* Map1d: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_Map1d(GLenum target, GLdouble u1, GLdouble u2, GLint stride, GLint order, const GLdouble * points)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "Map1d");
   CALL_Map1d(ctx->CurrentServerDispatch, (target, u1, u2, stride, order, points));
}


/* Map1f: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_Map1f(GLenum target, GLfloat u1, GLfloat u2, GLint stride, GLint order, const GLfloat * points)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "Map1f");
   CALL_Map1f(ctx->CurrentServerDispatch, (target, u1, u2, stride, order, points));
}


/* Map2d: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_Map2d(GLenum target, GLdouble u1, GLdouble u2, GLint ustride, GLint uorder, GLdouble v1, GLdouble v2, GLint vstride, GLint vorder, const GLdouble * points)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "Map2d");
   CALL_Map2d(ctx->CurrentServerDispatch, (target, u1, u2, ustride, uorder, v1, v2, vstride, vorder, points));
}


/* Map2f: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_Map2f(GLenum target, GLfloat u1, GLfloat u2, GLint ustride, GLint uorder, GLfloat v1, GLfloat v2, GLint vstride, GLint vorder, const GLfloat * points)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "Map2f");
   CALL_Map2f(ctx->CurrentServerDispatch, (target, u1, u2, ustride, uorder, v1, v2, vstride, vorder, points));
}


/* MapGrid1d: marshalled asynchronously */
struct marshal_cmd_MapGrid1d
{
   struct marshal_cmd_base cmd_base;
   GLint un;
   GLdouble u1;
   GLdouble u2;
};
uint32_t
_mesa_unmarshal_MapGrid1d(struct gl_context *ctx, const struct marshal_cmd_MapGrid1d *cmd)
{
   GLint un = cmd->un;
   GLdouble u1 = cmd->u1;
   GLdouble u2 = cmd->u2;
   CALL_MapGrid1d(ctx->CurrentServerDispatch, (un, u1, u2));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MapGrid1d), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_MapGrid1d(GLint un, GLdouble u1, GLdouble u2)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MapGrid1d);
   struct marshal_cmd_MapGrid1d *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MapGrid1d, cmd_size);
   cmd->un = un;
   cmd->u1 = u1;
   cmd->u2 = u2;
}


/* MapGrid1f: marshalled asynchronously */
struct marshal_cmd_MapGrid1f
{
   struct marshal_cmd_base cmd_base;
   GLint un;
   GLfloat u1;
   GLfloat u2;
};
uint32_t
_mesa_unmarshal_MapGrid1f(struct gl_context *ctx, const struct marshal_cmd_MapGrid1f *cmd)
{
   GLint un = cmd->un;
   GLfloat u1 = cmd->u1;
   GLfloat u2 = cmd->u2;
   CALL_MapGrid1f(ctx->CurrentServerDispatch, (un, u1, u2));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MapGrid1f), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_MapGrid1f(GLint un, GLfloat u1, GLfloat u2)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MapGrid1f);
   struct marshal_cmd_MapGrid1f *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MapGrid1f, cmd_size);
   cmd->un = un;
   cmd->u1 = u1;
   cmd->u2 = u2;
}


/* MapGrid2d: marshalled asynchronously */
struct marshal_cmd_MapGrid2d
{
   struct marshal_cmd_base cmd_base;
   GLint un;
   GLint vn;
   GLdouble u1;
   GLdouble u2;
   GLdouble v1;
   GLdouble v2;
};
uint32_t
_mesa_unmarshal_MapGrid2d(struct gl_context *ctx, const struct marshal_cmd_MapGrid2d *cmd)
{
   GLint un = cmd->un;
   GLdouble u1 = cmd->u1;
   GLdouble u2 = cmd->u2;
   GLint vn = cmd->vn;
   GLdouble v1 = cmd->v1;
   GLdouble v2 = cmd->v2;
   CALL_MapGrid2d(ctx->CurrentServerDispatch, (un, u1, u2, vn, v1, v2));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MapGrid2d), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_MapGrid2d(GLint un, GLdouble u1, GLdouble u2, GLint vn, GLdouble v1, GLdouble v2)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MapGrid2d);
   struct marshal_cmd_MapGrid2d *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MapGrid2d, cmd_size);
   cmd->un = un;
   cmd->u1 = u1;
   cmd->u2 = u2;
   cmd->vn = vn;
   cmd->v1 = v1;
   cmd->v2 = v2;
}


/* MapGrid2f: marshalled asynchronously */
struct marshal_cmd_MapGrid2f
{
   struct marshal_cmd_base cmd_base;
   GLint un;
   GLfloat u1;
   GLfloat u2;
   GLint vn;
   GLfloat v1;
   GLfloat v2;
};
uint32_t
_mesa_unmarshal_MapGrid2f(struct gl_context *ctx, const struct marshal_cmd_MapGrid2f *cmd)
{
   GLint un = cmd->un;
   GLfloat u1 = cmd->u1;
   GLfloat u2 = cmd->u2;
   GLint vn = cmd->vn;
   GLfloat v1 = cmd->v1;
   GLfloat v2 = cmd->v2;
   CALL_MapGrid2f(ctx->CurrentServerDispatch, (un, u1, u2, vn, v1, v2));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MapGrid2f), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_MapGrid2f(GLint un, GLfloat u1, GLfloat u2, GLint vn, GLfloat v1, GLfloat v2)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MapGrid2f);
   struct marshal_cmd_MapGrid2f *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MapGrid2f, cmd_size);
   cmd->un = un;
   cmd->u1 = u1;
   cmd->u2 = u2;
   cmd->vn = vn;
   cmd->v1 = v1;
   cmd->v2 = v2;
}


/* EvalCoord1d: marshalled asynchronously */
struct marshal_cmd_EvalCoord1d
{
   struct marshal_cmd_base cmd_base;
   GLdouble u;
};
uint32_t
_mesa_unmarshal_EvalCoord1d(struct gl_context *ctx, const struct marshal_cmd_EvalCoord1d *cmd)
{
   GLdouble u = cmd->u;
   CALL_EvalCoord1d(ctx->CurrentServerDispatch, (u));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_EvalCoord1d), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_EvalCoord1d(GLdouble u)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_EvalCoord1d);
   struct marshal_cmd_EvalCoord1d *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_EvalCoord1d, cmd_size);
   cmd->u = u;
}


/* EvalCoord1dv: marshalled asynchronously */
struct marshal_cmd_EvalCoord1dv
{
   struct marshal_cmd_base cmd_base;
   GLdouble u[1];
};
uint32_t
_mesa_unmarshal_EvalCoord1dv(struct gl_context *ctx, const struct marshal_cmd_EvalCoord1dv *cmd)
{
   const GLdouble *u = cmd->u;
   CALL_EvalCoord1dv(ctx->CurrentServerDispatch, (u));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_EvalCoord1dv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_EvalCoord1dv(const GLdouble * u)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_EvalCoord1dv);
   struct marshal_cmd_EvalCoord1dv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_EvalCoord1dv, cmd_size);
   memcpy(cmd->u, u, 1 * sizeof(GLdouble));
}


/* EvalCoord1f: marshalled asynchronously */
struct marshal_cmd_EvalCoord1f
{
   struct marshal_cmd_base cmd_base;
   GLfloat u;
};
uint32_t
_mesa_unmarshal_EvalCoord1f(struct gl_context *ctx, const struct marshal_cmd_EvalCoord1f *cmd)
{
   GLfloat u = cmd->u;
   CALL_EvalCoord1f(ctx->CurrentServerDispatch, (u));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_EvalCoord1f), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_EvalCoord1f(GLfloat u)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_EvalCoord1f);
   struct marshal_cmd_EvalCoord1f *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_EvalCoord1f, cmd_size);
   cmd->u = u;
}


/* EvalCoord1fv: marshalled asynchronously */
struct marshal_cmd_EvalCoord1fv
{
   struct marshal_cmd_base cmd_base;
   GLfloat u[1];
};
uint32_t
_mesa_unmarshal_EvalCoord1fv(struct gl_context *ctx, const struct marshal_cmd_EvalCoord1fv *cmd)
{
   const GLfloat *u = cmd->u;
   CALL_EvalCoord1fv(ctx->CurrentServerDispatch, (u));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_EvalCoord1fv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_EvalCoord1fv(const GLfloat * u)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_EvalCoord1fv);
   struct marshal_cmd_EvalCoord1fv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_EvalCoord1fv, cmd_size);
   memcpy(cmd->u, u, 1 * sizeof(GLfloat));
}


/* EvalCoord2d: marshalled asynchronously */
struct marshal_cmd_EvalCoord2d
{
   struct marshal_cmd_base cmd_base;
   GLdouble u;
   GLdouble v;
};
uint32_t
_mesa_unmarshal_EvalCoord2d(struct gl_context *ctx, const struct marshal_cmd_EvalCoord2d *cmd)
{
   GLdouble u = cmd->u;
   GLdouble v = cmd->v;
   CALL_EvalCoord2d(ctx->CurrentServerDispatch, (u, v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_EvalCoord2d), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_EvalCoord2d(GLdouble u, GLdouble v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_EvalCoord2d);
   struct marshal_cmd_EvalCoord2d *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_EvalCoord2d, cmd_size);
   cmd->u = u;
   cmd->v = v;
}


/* EvalCoord2dv: marshalled asynchronously */
struct marshal_cmd_EvalCoord2dv
{
   struct marshal_cmd_base cmd_base;
   GLdouble u[2];
};
uint32_t
_mesa_unmarshal_EvalCoord2dv(struct gl_context *ctx, const struct marshal_cmd_EvalCoord2dv *cmd)
{
   const GLdouble *u = cmd->u;
   CALL_EvalCoord2dv(ctx->CurrentServerDispatch, (u));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_EvalCoord2dv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_EvalCoord2dv(const GLdouble * u)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_EvalCoord2dv);
   struct marshal_cmd_EvalCoord2dv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_EvalCoord2dv, cmd_size);
   memcpy(cmd->u, u, 2 * sizeof(GLdouble));
}


/* EvalCoord2f: marshalled asynchronously */
struct marshal_cmd_EvalCoord2f
{
   struct marshal_cmd_base cmd_base;
   GLfloat u;
   GLfloat v;
};
uint32_t
_mesa_unmarshal_EvalCoord2f(struct gl_context *ctx, const struct marshal_cmd_EvalCoord2f *cmd)
{
   GLfloat u = cmd->u;
   GLfloat v = cmd->v;
   CALL_EvalCoord2f(ctx->CurrentServerDispatch, (u, v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_EvalCoord2f), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_EvalCoord2f(GLfloat u, GLfloat v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_EvalCoord2f);
   struct marshal_cmd_EvalCoord2f *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_EvalCoord2f, cmd_size);
   cmd->u = u;
   cmd->v = v;
}


/* EvalCoord2fv: marshalled asynchronously */
struct marshal_cmd_EvalCoord2fv
{
   struct marshal_cmd_base cmd_base;
   GLfloat u[2];
};
uint32_t
_mesa_unmarshal_EvalCoord2fv(struct gl_context *ctx, const struct marshal_cmd_EvalCoord2fv *cmd)
{
   const GLfloat *u = cmd->u;
   CALL_EvalCoord2fv(ctx->CurrentServerDispatch, (u));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_EvalCoord2fv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_EvalCoord2fv(const GLfloat * u)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_EvalCoord2fv);
   struct marshal_cmd_EvalCoord2fv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_EvalCoord2fv, cmd_size);
   memcpy(cmd->u, u, 2 * sizeof(GLfloat));
}


/* EvalMesh1: marshalled asynchronously */
struct marshal_cmd_EvalMesh1
{
   struct marshal_cmd_base cmd_base;
   GLenum16 mode;
   GLint i1;
   GLint i2;
};
uint32_t
_mesa_unmarshal_EvalMesh1(struct gl_context *ctx, const struct marshal_cmd_EvalMesh1 *cmd)
{
   GLenum mode = cmd->mode;
   GLint i1 = cmd->i1;
   GLint i2 = cmd->i2;
   CALL_EvalMesh1(ctx->CurrentServerDispatch, (mode, i1, i2));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_EvalMesh1), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_EvalMesh1(GLenum mode, GLint i1, GLint i2)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_EvalMesh1);
   struct marshal_cmd_EvalMesh1 *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_EvalMesh1, cmd_size);
   cmd->mode = MIN2(mode, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->i1 = i1;
   cmd->i2 = i2;
}


/* EvalPoint1: marshalled asynchronously */
struct marshal_cmd_EvalPoint1
{
   struct marshal_cmd_base cmd_base;
   GLint i;
};
uint32_t
_mesa_unmarshal_EvalPoint1(struct gl_context *ctx, const struct marshal_cmd_EvalPoint1 *cmd)
{
   GLint i = cmd->i;
   CALL_EvalPoint1(ctx->CurrentServerDispatch, (i));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_EvalPoint1), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_EvalPoint1(GLint i)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_EvalPoint1);
   struct marshal_cmd_EvalPoint1 *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_EvalPoint1, cmd_size);
   cmd->i = i;
}


/* EvalMesh2: marshalled asynchronously */
struct marshal_cmd_EvalMesh2
{
   struct marshal_cmd_base cmd_base;
   GLenum16 mode;
   GLint i1;
   GLint i2;
   GLint j1;
   GLint j2;
};
uint32_t
_mesa_unmarshal_EvalMesh2(struct gl_context *ctx, const struct marshal_cmd_EvalMesh2 *cmd)
{
   GLenum mode = cmd->mode;
   GLint i1 = cmd->i1;
   GLint i2 = cmd->i2;
   GLint j1 = cmd->j1;
   GLint j2 = cmd->j2;
   CALL_EvalMesh2(ctx->CurrentServerDispatch, (mode, i1, i2, j1, j2));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_EvalMesh2), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_EvalMesh2(GLenum mode, GLint i1, GLint i2, GLint j1, GLint j2)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_EvalMesh2);
   struct marshal_cmd_EvalMesh2 *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_EvalMesh2, cmd_size);
   cmd->mode = MIN2(mode, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->i1 = i1;
   cmd->i2 = i2;
   cmd->j1 = j1;
   cmd->j2 = j2;
}


/* EvalPoint2: marshalled asynchronously */
struct marshal_cmd_EvalPoint2
{
   struct marshal_cmd_base cmd_base;
   GLint i;
   GLint j;
};
uint32_t
_mesa_unmarshal_EvalPoint2(struct gl_context *ctx, const struct marshal_cmd_EvalPoint2 *cmd)
{
   GLint i = cmd->i;
   GLint j = cmd->j;
   CALL_EvalPoint2(ctx->CurrentServerDispatch, (i, j));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_EvalPoint2), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_EvalPoint2(GLint i, GLint j)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_EvalPoint2);
   struct marshal_cmd_EvalPoint2 *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_EvalPoint2, cmd_size);
   cmd->i = i;
   cmd->j = j;
}


/* AlphaFunc: marshalled asynchronously */
struct marshal_cmd_AlphaFunc
{
   struct marshal_cmd_base cmd_base;
   GLenum16 func;
   GLclampf ref;
};
uint32_t
_mesa_unmarshal_AlphaFunc(struct gl_context *ctx, const struct marshal_cmd_AlphaFunc *cmd)
{
   GLenum func = cmd->func;
   GLclampf ref = cmd->ref;
   CALL_AlphaFunc(ctx->CurrentServerDispatch, (func, ref));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_AlphaFunc), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_AlphaFunc(GLenum func, GLclampf ref)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_AlphaFunc);
   struct marshal_cmd_AlphaFunc *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_AlphaFunc, cmd_size);
   cmd->func = MIN2(func, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->ref = ref;
}


/* BlendFunc: marshalled asynchronously */
struct marshal_cmd_BlendFunc
{
   struct marshal_cmd_base cmd_base;
   GLenum16 sfactor;
   GLenum16 dfactor;
};
uint32_t
_mesa_unmarshal_BlendFunc(struct gl_context *ctx, const struct marshal_cmd_BlendFunc *cmd)
{
   GLenum sfactor = cmd->sfactor;
   GLenum dfactor = cmd->dfactor;
   CALL_BlendFunc(ctx->CurrentServerDispatch, (sfactor, dfactor));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_BlendFunc), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_BlendFunc(GLenum sfactor, GLenum dfactor)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_BlendFunc);
   struct marshal_cmd_BlendFunc *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_BlendFunc, cmd_size);
   cmd->sfactor = MIN2(sfactor, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->dfactor = MIN2(dfactor, 0xffff); /* clamped to 0xffff (invalid enum) */
}


/* LogicOp: marshalled asynchronously */
struct marshal_cmd_LogicOp
{
   struct marshal_cmd_base cmd_base;
   GLenum16 opcode;
};
uint32_t
_mesa_unmarshal_LogicOp(struct gl_context *ctx, const struct marshal_cmd_LogicOp *cmd)
{
   GLenum opcode = cmd->opcode;
   CALL_LogicOp(ctx->CurrentServerDispatch, (opcode));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_LogicOp), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_LogicOp(GLenum opcode)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_LogicOp);
   struct marshal_cmd_LogicOp *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_LogicOp, cmd_size);
   cmd->opcode = MIN2(opcode, 0xffff); /* clamped to 0xffff (invalid enum) */
}


/* StencilFunc: marshalled asynchronously */
struct marshal_cmd_StencilFunc
{
   struct marshal_cmd_base cmd_base;
   GLenum16 func;
   GLint ref;
   GLuint mask;
};
uint32_t
_mesa_unmarshal_StencilFunc(struct gl_context *ctx, const struct marshal_cmd_StencilFunc *cmd)
{
   GLenum func = cmd->func;
   GLint ref = cmd->ref;
   GLuint mask = cmd->mask;
   CALL_StencilFunc(ctx->CurrentServerDispatch, (func, ref, mask));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_StencilFunc), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_StencilFunc(GLenum func, GLint ref, GLuint mask)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_StencilFunc);
   struct marshal_cmd_StencilFunc *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_StencilFunc, cmd_size);
   cmd->func = MIN2(func, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->ref = ref;
   cmd->mask = mask;
}


/* StencilOp: marshalled asynchronously */
struct marshal_cmd_StencilOp
{
   struct marshal_cmd_base cmd_base;
   GLenum16 fail;
   GLenum16 zfail;
   GLenum16 zpass;
};
uint32_t
_mesa_unmarshal_StencilOp(struct gl_context *ctx, const struct marshal_cmd_StencilOp *cmd)
{
   GLenum fail = cmd->fail;
   GLenum zfail = cmd->zfail;
   GLenum zpass = cmd->zpass;
   CALL_StencilOp(ctx->CurrentServerDispatch, (fail, zfail, zpass));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_StencilOp), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_StencilOp(GLenum fail, GLenum zfail, GLenum zpass)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_StencilOp);
   struct marshal_cmd_StencilOp *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_StencilOp, cmd_size);
   cmd->fail = MIN2(fail, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->zfail = MIN2(zfail, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->zpass = MIN2(zpass, 0xffff); /* clamped to 0xffff (invalid enum) */
}


/* DepthFunc: marshalled asynchronously */
struct marshal_cmd_DepthFunc
{
   struct marshal_cmd_base cmd_base;
   GLenum16 func;
};
uint32_t
_mesa_unmarshal_DepthFunc(struct gl_context *ctx, const struct marshal_cmd_DepthFunc *cmd)
{
   GLenum func = cmd->func;
   CALL_DepthFunc(ctx->CurrentServerDispatch, (func));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_DepthFunc), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_DepthFunc(GLenum func)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_DepthFunc);
   struct marshal_cmd_DepthFunc *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_DepthFunc, cmd_size);
   cmd->func = MIN2(func, 0xffff); /* clamped to 0xffff (invalid enum) */
}


/* PixelZoom: marshalled asynchronously */
struct marshal_cmd_PixelZoom
{
   struct marshal_cmd_base cmd_base;
   GLfloat xfactor;
   GLfloat yfactor;
};
uint32_t
_mesa_unmarshal_PixelZoom(struct gl_context *ctx, const struct marshal_cmd_PixelZoom *cmd)
{
   GLfloat xfactor = cmd->xfactor;
   GLfloat yfactor = cmd->yfactor;
   CALL_PixelZoom(ctx->CurrentServerDispatch, (xfactor, yfactor));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_PixelZoom), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_PixelZoom(GLfloat xfactor, GLfloat yfactor)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_PixelZoom);
   struct marshal_cmd_PixelZoom *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_PixelZoom, cmd_size);
   cmd->xfactor = xfactor;
   cmd->yfactor = yfactor;
}


/* PixelTransferf: marshalled asynchronously */
struct marshal_cmd_PixelTransferf
{
   struct marshal_cmd_base cmd_base;
   GLenum16 pname;
   GLfloat param;
};
uint32_t
_mesa_unmarshal_PixelTransferf(struct gl_context *ctx, const struct marshal_cmd_PixelTransferf *cmd)
{
   GLenum pname = cmd->pname;
   GLfloat param = cmd->param;
   CALL_PixelTransferf(ctx->CurrentServerDispatch, (pname, param));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_PixelTransferf), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_PixelTransferf(GLenum pname, GLfloat param)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_PixelTransferf);
   struct marshal_cmd_PixelTransferf *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_PixelTransferf, cmd_size);
   cmd->pname = MIN2(pname, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->param = param;
}


/* PixelTransferi: marshalled asynchronously */
struct marshal_cmd_PixelTransferi
{
   struct marshal_cmd_base cmd_base;
   GLenum16 pname;
   GLint param;
};
uint32_t
_mesa_unmarshal_PixelTransferi(struct gl_context *ctx, const struct marshal_cmd_PixelTransferi *cmd)
{
   GLenum pname = cmd->pname;
   GLint param = cmd->param;
   CALL_PixelTransferi(ctx->CurrentServerDispatch, (pname, param));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_PixelTransferi), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_PixelTransferi(GLenum pname, GLint param)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_PixelTransferi);
   struct marshal_cmd_PixelTransferi *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_PixelTransferi, cmd_size);
   cmd->pname = MIN2(pname, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->param = param;
}


/* PixelStoref: marshalled asynchronously */
struct marshal_cmd_PixelStoref
{
   struct marshal_cmd_base cmd_base;
   GLenum16 pname;
   GLfloat param;
};
uint32_t
_mesa_unmarshal_PixelStoref(struct gl_context *ctx, const struct marshal_cmd_PixelStoref *cmd)
{
   GLenum pname = cmd->pname;
   GLfloat param = cmd->param;
   CALL_PixelStoref(ctx->CurrentServerDispatch, (pname, param));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_PixelStoref), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_PixelStoref(GLenum pname, GLfloat param)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_PixelStoref);
   struct marshal_cmd_PixelStoref *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_PixelStoref, cmd_size);
   cmd->pname = MIN2(pname, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->param = param;
}


/* PixelStorei: marshalled asynchronously */
struct marshal_cmd_PixelStorei
{
   struct marshal_cmd_base cmd_base;
   GLenum16 pname;
   GLint param;
};
uint32_t
_mesa_unmarshal_PixelStorei(struct gl_context *ctx, const struct marshal_cmd_PixelStorei *cmd)
{
   GLenum pname = cmd->pname;
   GLint param = cmd->param;
   CALL_PixelStorei(ctx->CurrentServerDispatch, (pname, param));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_PixelStorei), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_PixelStorei(GLenum pname, GLint param)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_PixelStorei);
   struct marshal_cmd_PixelStorei *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_PixelStorei, cmd_size);
   cmd->pname = MIN2(pname, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->param = param;
}


/* PixelMapfv: marshalled asynchronously */
struct marshal_cmd_PixelMapfv
{
   struct marshal_cmd_base cmd_base;
   GLenum16 map;
   GLsizei mapsize;
   const GLfloat * values;
};
uint32_t
_mesa_unmarshal_PixelMapfv(struct gl_context *ctx, const struct marshal_cmd_PixelMapfv *cmd)
{
   GLenum map = cmd->map;
   GLsizei mapsize = cmd->mapsize;
   const GLfloat * values = cmd->values;
   CALL_PixelMapfv(ctx->CurrentServerDispatch, (map, mapsize, values));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_PixelMapfv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_PixelMapfv(GLenum map, GLsizei mapsize, const GLfloat * values)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_PixelMapfv);
   struct marshal_cmd_PixelMapfv *cmd;
   if (_mesa_glthread_has_no_unpack_buffer(ctx)) {
      _mesa_glthread_finish_before(ctx, "PixelMapfv");
      CALL_PixelMapfv(ctx->CurrentServerDispatch, (map, mapsize, values));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_PixelMapfv, cmd_size);
   cmd->map = MIN2(map, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->mapsize = mapsize;
   cmd->values = values;
}


/* PixelMapuiv: marshalled asynchronously */
struct marshal_cmd_PixelMapuiv
{
   struct marshal_cmd_base cmd_base;
   GLenum16 map;
   GLsizei mapsize;
   const GLuint * values;
};
uint32_t
_mesa_unmarshal_PixelMapuiv(struct gl_context *ctx, const struct marshal_cmd_PixelMapuiv *cmd)
{
   GLenum map = cmd->map;
   GLsizei mapsize = cmd->mapsize;
   const GLuint * values = cmd->values;
   CALL_PixelMapuiv(ctx->CurrentServerDispatch, (map, mapsize, values));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_PixelMapuiv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_PixelMapuiv(GLenum map, GLsizei mapsize, const GLuint * values)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_PixelMapuiv);
   struct marshal_cmd_PixelMapuiv *cmd;
   if (_mesa_glthread_has_no_unpack_buffer(ctx)) {
      _mesa_glthread_finish_before(ctx, "PixelMapuiv");
      CALL_PixelMapuiv(ctx->CurrentServerDispatch, (map, mapsize, values));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_PixelMapuiv, cmd_size);
   cmd->map = MIN2(map, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->mapsize = mapsize;
   cmd->values = values;
}


/* PixelMapusv: marshalled asynchronously */
struct marshal_cmd_PixelMapusv
{
   struct marshal_cmd_base cmd_base;
   GLenum16 map;
   GLsizei mapsize;
   const GLushort * values;
};
uint32_t
_mesa_unmarshal_PixelMapusv(struct gl_context *ctx, const struct marshal_cmd_PixelMapusv *cmd)
{
   GLenum map = cmd->map;
   GLsizei mapsize = cmd->mapsize;
   const GLushort * values = cmd->values;
   CALL_PixelMapusv(ctx->CurrentServerDispatch, (map, mapsize, values));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_PixelMapusv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_PixelMapusv(GLenum map, GLsizei mapsize, const GLushort * values)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_PixelMapusv);
   struct marshal_cmd_PixelMapusv *cmd;
   if (_mesa_glthread_has_no_unpack_buffer(ctx)) {
      _mesa_glthread_finish_before(ctx, "PixelMapusv");
      CALL_PixelMapusv(ctx->CurrentServerDispatch, (map, mapsize, values));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_PixelMapusv, cmd_size);
   cmd->map = MIN2(map, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->mapsize = mapsize;
   cmd->values = values;
}


/* ReadBuffer: marshalled asynchronously */
struct marshal_cmd_ReadBuffer
{
   struct marshal_cmd_base cmd_base;
   GLenum16 mode;
};
uint32_t
_mesa_unmarshal_ReadBuffer(struct gl_context *ctx, const struct marshal_cmd_ReadBuffer *cmd)
{
   GLenum mode = cmd->mode;
   CALL_ReadBuffer(ctx->CurrentServerDispatch, (mode));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_ReadBuffer), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ReadBuffer(GLenum mode)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ReadBuffer);
   struct marshal_cmd_ReadBuffer *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ReadBuffer, cmd_size);
   cmd->mode = MIN2(mode, 0xffff); /* clamped to 0xffff (invalid enum) */
}


/* CopyPixels: marshalled asynchronously */
struct marshal_cmd_CopyPixels
{
   struct marshal_cmd_base cmd_base;
   GLenum16 type;
   GLint x;
   GLint y;
   GLsizei width;
   GLsizei height;
};
uint32_t
_mesa_unmarshal_CopyPixels(struct gl_context *ctx, const struct marshal_cmd_CopyPixels *cmd)
{
   GLint x = cmd->x;
   GLint y = cmd->y;
   GLsizei width = cmd->width;
   GLsizei height = cmd->height;
   GLenum type = cmd->type;
   CALL_CopyPixels(ctx->CurrentServerDispatch, (x, y, width, height, type));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_CopyPixels), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_CopyPixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum type)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_CopyPixels);
   struct marshal_cmd_CopyPixels *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_CopyPixels, cmd_size);
   cmd->x = x;
   cmd->y = y;
   cmd->width = width;
   cmd->height = height;
   cmd->type = MIN2(type, 0xffff); /* clamped to 0xffff (invalid enum) */
}


/* ReadPixels: marshalled asynchronously */
struct marshal_cmd_ReadPixels
{
   struct marshal_cmd_base cmd_base;
   GLenum16 format;
   GLenum16 type;
   GLint x;
   GLint y;
   GLsizei width;
   GLsizei height;
   GLvoid * pixels;
};
uint32_t
_mesa_unmarshal_ReadPixels(struct gl_context *ctx, const struct marshal_cmd_ReadPixels *cmd)
{
   GLint x = cmd->x;
   GLint y = cmd->y;
   GLsizei width = cmd->width;
   GLsizei height = cmd->height;
   GLenum format = cmd->format;
   GLenum type = cmd->type;
   GLvoid * pixels = cmd->pixels;
   CALL_ReadPixels(ctx->CurrentServerDispatch, (x, y, width, height, format, type, pixels));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_ReadPixels), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ReadPixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid * pixels)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ReadPixels);
   struct marshal_cmd_ReadPixels *cmd;
   if (_mesa_glthread_has_no_pack_buffer(ctx)) {
      _mesa_glthread_finish_before(ctx, "ReadPixels");
      CALL_ReadPixels(ctx->CurrentServerDispatch, (x, y, width, height, format, type, pixels));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ReadPixels, cmd_size);
   cmd->x = x;
   cmd->y = y;
   cmd->width = width;
   cmd->height = height;
   cmd->format = MIN2(format, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->type = MIN2(type, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->pixels = pixels;
}


/* DrawPixels: marshalled asynchronously */
struct marshal_cmd_DrawPixels
{
   struct marshal_cmd_base cmd_base;
   GLenum16 format;
   GLenum16 type;
   GLsizei width;
   GLsizei height;
   const GLvoid * pixels;
};
uint32_t
_mesa_unmarshal_DrawPixels(struct gl_context *ctx, const struct marshal_cmd_DrawPixels *cmd)
{
   GLsizei width = cmd->width;
   GLsizei height = cmd->height;
   GLenum format = cmd->format;
   GLenum type = cmd->type;
   const GLvoid * pixels = cmd->pixels;
   CALL_DrawPixels(ctx->CurrentServerDispatch, (width, height, format, type, pixels));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_DrawPixels), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_DrawPixels(GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid * pixels)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_DrawPixels);
   struct marshal_cmd_DrawPixels *cmd;
   if (_mesa_glthread_has_no_unpack_buffer(ctx)) {
      _mesa_glthread_finish_before(ctx, "DrawPixels");
      CALL_DrawPixels(ctx->CurrentServerDispatch, (width, height, format, type, pixels));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_DrawPixels, cmd_size);
   cmd->width = width;
   cmd->height = height;
   cmd->format = MIN2(format, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->type = MIN2(type, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->pixels = pixels;
}


/* GetBooleanv: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetBooleanv(GLenum pname, GLboolean * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetBooleanv");
   CALL_GetBooleanv(ctx->CurrentServerDispatch, (pname, params));
}


/* GetClipPlane: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetClipPlane(GLenum plane, GLdouble * equation)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetClipPlane");
   CALL_GetClipPlane(ctx->CurrentServerDispatch, (plane, equation));
}


/* GetDoublev: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetDoublev(GLenum pname, GLdouble * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetDoublev");
   CALL_GetDoublev(ctx->CurrentServerDispatch, (pname, params));
}


/* GetError: marshalled synchronously */
static GLenum GLAPIENTRY
_mesa_marshal_GetError(void)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetError");
   return CALL_GetError(ctx->CurrentServerDispatch, ());
}


/* GetFloatv: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetFloatv(GLenum pname, GLfloat * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetFloatv");
   CALL_GetFloatv(ctx->CurrentServerDispatch, (pname, params));
}


/* GetLightfv: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetLightfv(GLenum light, GLenum pname, GLfloat * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetLightfv");
   CALL_GetLightfv(ctx->CurrentServerDispatch, (light, pname, params));
}


/* GetLightiv: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetLightiv(GLenum light, GLenum pname, GLint * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetLightiv");
   CALL_GetLightiv(ctx->CurrentServerDispatch, (light, pname, params));
}


/* GetMapdv: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetMapdv(GLenum target, GLenum query, GLdouble * v)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetMapdv");
   CALL_GetMapdv(ctx->CurrentServerDispatch, (target, query, v));
}


/* GetMapfv: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetMapfv(GLenum target, GLenum query, GLfloat * v)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetMapfv");
   CALL_GetMapfv(ctx->CurrentServerDispatch, (target, query, v));
}


/* GetMapiv: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetMapiv(GLenum target, GLenum query, GLint * v)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetMapiv");
   CALL_GetMapiv(ctx->CurrentServerDispatch, (target, query, v));
}


/* GetMaterialfv: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetMaterialfv(GLenum face, GLenum pname, GLfloat * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetMaterialfv");
   CALL_GetMaterialfv(ctx->CurrentServerDispatch, (face, pname, params));
}


/* GetMaterialiv: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetMaterialiv(GLenum face, GLenum pname, GLint * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetMaterialiv");
   CALL_GetMaterialiv(ctx->CurrentServerDispatch, (face, pname, params));
}


/* GetPixelMapfv: marshalled asynchronously */
struct marshal_cmd_GetPixelMapfv
{
   struct marshal_cmd_base cmd_base;
   GLenum16 map;
   GLfloat * values;
};
uint32_t
_mesa_unmarshal_GetPixelMapfv(struct gl_context *ctx, const struct marshal_cmd_GetPixelMapfv *cmd)
{
   GLenum map = cmd->map;
   GLfloat * values = cmd->values;
   CALL_GetPixelMapfv(ctx->CurrentServerDispatch, (map, values));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_GetPixelMapfv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
   cmd->map = MIN2(map, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->values = values;
}


/* GetPixelMapuiv: marshalled asynchronously */
struct marshal_cmd_GetPixelMapuiv
{
   struct marshal_cmd_base cmd_base;
   GLenum16 map;
   GLuint * values;
};
uint32_t
_mesa_unmarshal_GetPixelMapuiv(struct gl_context *ctx, const struct marshal_cmd_GetPixelMapuiv *cmd)
{
   GLenum map = cmd->map;
   GLuint * values = cmd->values;
   CALL_GetPixelMapuiv(ctx->CurrentServerDispatch, (map, values));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_GetPixelMapuiv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
   cmd->map = MIN2(map, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->values = values;
}


/* GetPixelMapusv: marshalled asynchronously */
struct marshal_cmd_GetPixelMapusv
{
   struct marshal_cmd_base cmd_base;
   GLenum16 map;
   GLushort * values;
};
uint32_t
_mesa_unmarshal_GetPixelMapusv(struct gl_context *ctx, const struct marshal_cmd_GetPixelMapusv *cmd)
{
   GLenum map = cmd->map;
   GLushort * values = cmd->values;
   CALL_GetPixelMapusv(ctx->CurrentServerDispatch, (map, values));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_GetPixelMapusv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
   cmd->map = MIN2(map, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->values = values;
}


/* GetPolygonStipple: marshalled asynchronously */
struct marshal_cmd_GetPolygonStipple
{
   struct marshal_cmd_base cmd_base;
   GLubyte * mask;
};
uint32_t
_mesa_unmarshal_GetPolygonStipple(struct gl_context *ctx, const struct marshal_cmd_GetPolygonStipple *cmd)
{
   GLubyte * mask = cmd->mask;
   CALL_GetPolygonStipple(ctx->CurrentServerDispatch, (mask));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_GetPolygonStipple), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
static const GLubyte * GLAPIENTRY
_mesa_marshal_GetString(GLenum name)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetString");
   return CALL_GetString(ctx->CurrentServerDispatch, (name));
}


/* GetTexEnvfv: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetTexEnvfv(GLenum target, GLenum pname, GLfloat * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetTexEnvfv");
   CALL_GetTexEnvfv(ctx->CurrentServerDispatch, (target, pname, params));
}


/* GetTexEnviv: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetTexEnviv(GLenum target, GLenum pname, GLint * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetTexEnviv");
   CALL_GetTexEnviv(ctx->CurrentServerDispatch, (target, pname, params));
}


/* GetTexGendv: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetTexGendv(GLenum coord, GLenum pname, GLdouble * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetTexGendv");
   CALL_GetTexGendv(ctx->CurrentServerDispatch, (coord, pname, params));
}


/* GetTexGenfv: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetTexGenfv(GLenum coord, GLenum pname, GLfloat * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetTexGenfv");
   CALL_GetTexGenfv(ctx->CurrentServerDispatch, (coord, pname, params));
}


/* GetTexGeniv: marshalled synchronously */
static void GLAPIENTRY
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
   GLenum16 target;
   GLenum16 format;
   GLenum16 type;
   GLint level;
   GLvoid * pixels;
};
uint32_t
_mesa_unmarshal_GetTexImage(struct gl_context *ctx, const struct marshal_cmd_GetTexImage *cmd)
{
   GLenum target = cmd->target;
   GLint level = cmd->level;
   GLenum format = cmd->format;
   GLenum type = cmd->type;
   GLvoid * pixels = cmd->pixels;
   CALL_GetTexImage(ctx->CurrentServerDispatch, (target, level, format, type, pixels));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_GetTexImage), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->level = level;
   cmd->format = MIN2(format, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->type = MIN2(type, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->pixels = pixels;
}


/* GetTexParameterfv: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetTexParameterfv(GLenum target, GLenum pname, GLfloat * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetTexParameterfv");
   CALL_GetTexParameterfv(ctx->CurrentServerDispatch, (target, pname, params));
}


/* GetTexParameteriv: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetTexParameteriv(GLenum target, GLenum pname, GLint * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetTexParameteriv");
   CALL_GetTexParameteriv(ctx->CurrentServerDispatch, (target, pname, params));
}


/* GetTexLevelParameterfv: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetTexLevelParameterfv(GLenum target, GLint level, GLenum pname, GLfloat * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetTexLevelParameterfv");
   CALL_GetTexLevelParameterfv(ctx->CurrentServerDispatch, (target, level, pname, params));
}


/* GetTexLevelParameteriv: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_GetTexLevelParameteriv(GLenum target, GLint level, GLenum pname, GLint * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetTexLevelParameteriv");
   CALL_GetTexLevelParameteriv(ctx->CurrentServerDispatch, (target, level, pname, params));
}


/* IsEnabled: marshalled synchronously */
static GLboolean GLAPIENTRY
_mesa_marshal_IsEnabled(GLenum cap)
{
   GET_CURRENT_CONTEXT(ctx);
   int result = _mesa_glthread_IsEnabled(ctx, cap); if (result >= 0) return result;
   _mesa_glthread_finish_before(ctx, "IsEnabled");
   return CALL_IsEnabled(ctx->CurrentServerDispatch, (cap));
}


/* IsList: marshalled synchronously */
static GLboolean GLAPIENTRY
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
_mesa_unmarshal_DepthRange(struct gl_context *ctx, const struct marshal_cmd_DepthRange *cmd)
{
   GLclampd zNear = cmd->zNear;
   GLclampd zFar = cmd->zFar;
   CALL_DepthRange(ctx->CurrentServerDispatch, (zNear, zFar));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_DepthRange), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
_mesa_unmarshal_Frustum(struct gl_context *ctx, const struct marshal_cmd_Frustum *cmd)
{
   GLdouble left = cmd->left;
   GLdouble right = cmd->right;
   GLdouble bottom = cmd->bottom;
   GLdouble top = cmd->top;
   GLdouble zNear = cmd->zNear;
   GLdouble zFar = cmd->zFar;
   CALL_Frustum(ctx->CurrentServerDispatch, (left, right, bottom, top, zNear, zFar));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Frustum), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
_mesa_unmarshal_LoadIdentity(struct gl_context *ctx, const struct marshal_cmd_LoadIdentity *cmd)
{
   CALL_LoadIdentity(ctx->CurrentServerDispatch, ());
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_LoadIdentity), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
_mesa_unmarshal_LoadMatrixf(struct gl_context *ctx, const struct marshal_cmd_LoadMatrixf *cmd)
{
   const GLfloat *m = cmd->m;
   CALL_LoadMatrixf(ctx->CurrentServerDispatch, (m));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_LoadMatrixf), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
_mesa_unmarshal_LoadMatrixd(struct gl_context *ctx, const struct marshal_cmd_LoadMatrixd *cmd)
{
   const GLdouble *m = cmd->m;
   CALL_LoadMatrixd(ctx->CurrentServerDispatch, (m));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_LoadMatrixd), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
   GLenum16 mode;
};
uint32_t
_mesa_unmarshal_MatrixMode(struct gl_context *ctx, const struct marshal_cmd_MatrixMode *cmd)
{
   GLenum mode = cmd->mode;
   CALL_MatrixMode(ctx->CurrentServerDispatch, (mode));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MatrixMode), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_MatrixMode(GLenum mode)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MatrixMode);
   struct marshal_cmd_MatrixMode *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MatrixMode, cmd_size);
   cmd->mode = MIN2(mode, 0xffff); /* clamped to 0xffff (invalid enum) */
   _mesa_glthread_MatrixMode(ctx, mode);
}


/* MultMatrixf: marshalled asynchronously */
struct marshal_cmd_MultMatrixf
{
   struct marshal_cmd_base cmd_base;
   GLfloat m[16];
};
uint32_t
_mesa_unmarshal_MultMatrixf(struct gl_context *ctx, const struct marshal_cmd_MultMatrixf *cmd)
{
   const GLfloat *m = cmd->m;
   CALL_MultMatrixf(ctx->CurrentServerDispatch, (m));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MultMatrixf), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_MultMatrixf(const GLfloat * m)
{
   GET_CURRENT_CONTEXT(ctx);
   if (_mesa_matrix_is_identity(m)) return;
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
_mesa_unmarshal_MultMatrixd(struct gl_context *ctx, const struct marshal_cmd_MultMatrixd *cmd)
{
   const GLdouble *m = cmd->m;
   CALL_MultMatrixd(ctx->CurrentServerDispatch, (m));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MultMatrixd), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
_mesa_unmarshal_Ortho(struct gl_context *ctx, const struct marshal_cmd_Ortho *cmd)
{
   GLdouble left = cmd->left;
   GLdouble right = cmd->right;
   GLdouble bottom = cmd->bottom;
   GLdouble top = cmd->top;
   GLdouble zNear = cmd->zNear;
   GLdouble zFar = cmd->zFar;
   CALL_Ortho(ctx->CurrentServerDispatch, (left, right, bottom, top, zNear, zFar));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Ortho), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
_mesa_unmarshal_PopMatrix(struct gl_context *ctx, const struct marshal_cmd_PopMatrix *cmd)
{
   CALL_PopMatrix(ctx->CurrentServerDispatch, ());
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_PopMatrix), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
_mesa_unmarshal_PushMatrix(struct gl_context *ctx, const struct marshal_cmd_PushMatrix *cmd)
{
   CALL_PushMatrix(ctx->CurrentServerDispatch, ());
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_PushMatrix), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
_mesa_unmarshal_Rotated(struct gl_context *ctx, const struct marshal_cmd_Rotated *cmd)
{
   GLdouble angle = cmd->angle;
   GLdouble x = cmd->x;
   GLdouble y = cmd->y;
   GLdouble z = cmd->z;
   CALL_Rotated(ctx->CurrentServerDispatch, (angle, x, y, z));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Rotated), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
_mesa_unmarshal_Rotatef(struct gl_context *ctx, const struct marshal_cmd_Rotatef *cmd)
{
   GLfloat angle = cmd->angle;
   GLfloat x = cmd->x;
   GLfloat y = cmd->y;
   GLfloat z = cmd->z;
   CALL_Rotatef(ctx->CurrentServerDispatch, (angle, x, y, z));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Rotatef), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
_mesa_unmarshal_Scaled(struct gl_context *ctx, const struct marshal_cmd_Scaled *cmd)
{
   GLdouble x = cmd->x;
   GLdouble y = cmd->y;
   GLdouble z = cmd->z;
   CALL_Scaled(ctx->CurrentServerDispatch, (x, y, z));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Scaled), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
_mesa_unmarshal_Scalef(struct gl_context *ctx, const struct marshal_cmd_Scalef *cmd)
{
   GLfloat x = cmd->x;
   GLfloat y = cmd->y;
   GLfloat z = cmd->z;
   CALL_Scalef(ctx->CurrentServerDispatch, (x, y, z));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Scalef), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
_mesa_unmarshal_Translated(struct gl_context *ctx, const struct marshal_cmd_Translated *cmd)
{
   GLdouble x = cmd->x;
   GLdouble y = cmd->y;
   GLdouble z = cmd->z;
   CALL_Translated(ctx->CurrentServerDispatch, (x, y, z));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Translated), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
_mesa_unmarshal_Translatef(struct gl_context *ctx, const struct marshal_cmd_Translatef *cmd)
{
   GLfloat x = cmd->x;
   GLfloat y = cmd->y;
   GLfloat z = cmd->z;
   CALL_Translatef(ctx->CurrentServerDispatch, (x, y, z));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Translatef), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
_mesa_unmarshal_Viewport(struct gl_context *ctx, const struct marshal_cmd_Viewport *cmd)
{
   GLint x = cmd->x;
   GLint y = cmd->y;
   GLsizei width = cmd->width;
   GLsizei height = cmd->height;
   CALL_Viewport(ctx->CurrentServerDispatch, (x, y, width, height));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Viewport), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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


/* ArrayElement: marshalled synchronously */
static void GLAPIENTRY
_mesa_marshal_ArrayElement(GLint i)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "ArrayElement");
   CALL_ArrayElement(ctx->CurrentServerDispatch, (i));
}


/* ColorPointer: marshalled asynchronously */
struct marshal_cmd_ColorPointer
{
   struct marshal_cmd_base cmd_base;
   GLenum16 type;
   GLint size;
   GLsizei stride;
   const GLvoid * pointer;
};
uint32_t
_mesa_unmarshal_ColorPointer(struct gl_context *ctx, const struct marshal_cmd_ColorPointer *cmd)
{
   GLint size = cmd->size;
   GLenum type = cmd->type;
   GLsizei stride = cmd->stride;
   const GLvoid * pointer = cmd->pointer;
   CALL_ColorPointer(ctx->CurrentServerDispatch, (size, type, stride, pointer));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_ColorPointer), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ColorPointer(GLint size, GLenum type, GLsizei stride, const GLvoid * pointer)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ColorPointer);
   struct marshal_cmd_ColorPointer *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ColorPointer, cmd_size);
   cmd->size = size;
   cmd->type = MIN2(type, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->stride = stride;
   cmd->pointer = pointer;
   if (COMPAT) _mesa_glthread_AttribPointer(ctx, VERT_ATTRIB_COLOR0, size, type, stride, pointer);
}


/* DisableClientState: marshalled asynchronously */
struct marshal_cmd_DisableClientState
{
   struct marshal_cmd_base cmd_base;
   GLenum16 array;
};
uint32_t
_mesa_unmarshal_DisableClientState(struct gl_context *ctx, const struct marshal_cmd_DisableClientState *cmd)
{
   GLenum array = cmd->array;
   CALL_DisableClientState(ctx->CurrentServerDispatch, (array));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_DisableClientState), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_DisableClientState(GLenum array)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_DisableClientState);
   struct marshal_cmd_DisableClientState *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_DisableClientState, cmd_size);
   cmd->array = MIN2(array, 0xffff); /* clamped to 0xffff (invalid enum) */
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
_mesa_unmarshal_EdgeFlagPointer(struct gl_context *ctx, const struct marshal_cmd_EdgeFlagPointer *cmd)
{
   GLsizei stride = cmd->stride;
   const GLvoid * pointer = cmd->pointer;
   CALL_EdgeFlagPointer(ctx->CurrentServerDispatch, (stride, pointer));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_EdgeFlagPointer), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
   GLenum16 array;
};
uint32_t
_mesa_unmarshal_EnableClientState(struct gl_context *ctx, const struct marshal_cmd_EnableClientState *cmd)
{
   GLenum array = cmd->array;
   CALL_EnableClientState(ctx->CurrentServerDispatch, (array));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_EnableClientState), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_EnableClientState(GLenum array)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_EnableClientState);
   struct marshal_cmd_EnableClientState *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_EnableClientState, cmd_size);
   cmd->array = MIN2(array, 0xffff); /* clamped to 0xffff (invalid enum) */
   if (COMPAT) _mesa_glthread_ClientState(ctx, NULL, _mesa_array_to_attrib(ctx, array), true);
}


/* GetPointerv: marshalled synchronously */
static void GLAPIENTRY
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
   GLenum16 type;
   GLsizei stride;
   const GLvoid * pointer;
};
uint32_t
_mesa_unmarshal_IndexPointer(struct gl_context *ctx, const struct marshal_cmd_IndexPointer *cmd)
{
   GLenum type = cmd->type;
   GLsizei stride = cmd->stride;
   const GLvoid * pointer = cmd->pointer;
   CALL_IndexPointer(ctx->CurrentServerDispatch, (type, stride, pointer));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_IndexPointer), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_IndexPointer(GLenum type, GLsizei stride, const GLvoid * pointer)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_IndexPointer);
   struct marshal_cmd_IndexPointer *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_IndexPointer, cmd_size);
   cmd->type = MIN2(type, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->stride = stride;
   cmd->pointer = pointer;
   if (COMPAT) _mesa_glthread_AttribPointer(ctx, VERT_ATTRIB_COLOR_INDEX, 1, type, stride, pointer);
}


/* InterleavedArrays: marshalled asynchronously */
struct marshal_cmd_InterleavedArrays
{
   struct marshal_cmd_base cmd_base;
   GLenum16 format;
   GLsizei stride;
   const GLvoid * pointer;
};
uint32_t
_mesa_unmarshal_InterleavedArrays(struct gl_context *ctx, const struct marshal_cmd_InterleavedArrays *cmd)
{
   GLenum format = cmd->format;
   GLsizei stride = cmd->stride;
   const GLvoid * pointer = cmd->pointer;
   CALL_InterleavedArrays(ctx->CurrentServerDispatch, (format, stride, pointer));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_InterleavedArrays), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_InterleavedArrays(GLenum format, GLsizei stride, const GLvoid * pointer)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_InterleavedArrays);
   struct marshal_cmd_InterleavedArrays *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_InterleavedArrays, cmd_size);
   cmd->format = MIN2(format, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->stride = stride;
   cmd->pointer = pointer;
   _mesa_glthread_InterleavedArrays(ctx, format, stride, pointer);
}


/* NormalPointer: marshalled asynchronously */
struct marshal_cmd_NormalPointer
{
   struct marshal_cmd_base cmd_base;
   GLenum16 type;
   GLsizei stride;
   const GLvoid * pointer;
};
uint32_t
_mesa_unmarshal_NormalPointer(struct gl_context *ctx, const struct marshal_cmd_NormalPointer *cmd)
{
   GLenum type = cmd->type;
   GLsizei stride = cmd->stride;
   const GLvoid * pointer = cmd->pointer;
   CALL_NormalPointer(ctx->CurrentServerDispatch, (type, stride, pointer));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_NormalPointer), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_NormalPointer(GLenum type, GLsizei stride, const GLvoid * pointer)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_NormalPointer);
   struct marshal_cmd_NormalPointer *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_NormalPointer, cmd_size);
   cmd->type = MIN2(type, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->stride = stride;
   cmd->pointer = pointer;
   if (COMPAT) _mesa_glthread_AttribPointer(ctx, VERT_ATTRIB_NORMAL, 3, type, stride, pointer);
}


/* TexCoordPointer: marshalled asynchronously */
struct marshal_cmd_TexCoordPointer
{
   struct marshal_cmd_base cmd_base;
   GLenum16 type;
   GLint size;
   GLsizei stride;
   const GLvoid * pointer;
};
uint32_t
_mesa_unmarshal_TexCoordPointer(struct gl_context *ctx, const struct marshal_cmd_TexCoordPointer *cmd)
{
   GLint size = cmd->size;
   GLenum type = cmd->type;
   GLsizei stride = cmd->stride;
   const GLvoid * pointer = cmd->pointer;
   CALL_TexCoordPointer(ctx->CurrentServerDispatch, (size, type, stride, pointer));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_TexCoordPointer), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_TexCoordPointer(GLint size, GLenum type, GLsizei stride, const GLvoid * pointer)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TexCoordPointer);
   struct marshal_cmd_TexCoordPointer *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexCoordPointer, cmd_size);
   cmd->size = size;
   cmd->type = MIN2(type, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->stride = stride;
   cmd->pointer = pointer;
   if (COMPAT) _mesa_glthread_AttribPointer(ctx, VERT_ATTRIB_TEX(ctx->GLThread.ClientActiveTexture), size, type, stride, pointer);
}


/* VertexPointer: marshalled asynchronously */
struct marshal_cmd_VertexPointer
{
   struct marshal_cmd_base cmd_base;
   GLenum16 type;
   GLint size;
   GLsizei stride;
   const GLvoid * pointer;
};
uint32_t
_mesa_unmarshal_VertexPointer(struct gl_context *ctx, const struct marshal_cmd_VertexPointer *cmd)
{
   GLint size = cmd->size;
   GLenum type = cmd->type;
   GLsizei stride = cmd->stride;
   const GLvoid * pointer = cmd->pointer;
   CALL_VertexPointer(ctx->CurrentServerDispatch, (size, type, stride, pointer));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_VertexPointer), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_VertexPointer(GLint size, GLenum type, GLsizei stride, const GLvoid * pointer)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexPointer);
   struct marshal_cmd_VertexPointer *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexPointer, cmd_size);
   cmd->size = size;
   cmd->type = MIN2(type, 0xffff); /* clamped to 0xffff (invalid enum) */
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
_mesa_unmarshal_PolygonOffset(struct gl_context *ctx, const struct marshal_cmd_PolygonOffset *cmd)
{
   GLfloat factor = cmd->factor;
   GLfloat units = cmd->units;
   CALL_PolygonOffset(ctx->CurrentServerDispatch, (factor, units));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_PolygonOffset), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
   GLenum16 target;
   GLenum16 internalformat;
   GLint level;
   GLint x;
   GLint y;
   GLsizei width;
   GLint border;
};
uint32_t
_mesa_unmarshal_CopyTexImage1D(struct gl_context *ctx, const struct marshal_cmd_CopyTexImage1D *cmd)
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
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_CopyTexImage1D(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLint border)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_CopyTexImage1D);
   struct marshal_cmd_CopyTexImage1D *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_CopyTexImage1D, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->level = level;
   cmd->internalformat = MIN2(internalformat, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->x = x;
   cmd->y = y;
   cmd->width = width;
   cmd->border = border;
}


/* CopyTexImage2D: marshalled asynchronously */
struct marshal_cmd_CopyTexImage2D
{
   struct marshal_cmd_base cmd_base;
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
_mesa_unmarshal_CopyTexImage2D(struct gl_context *ctx, const struct marshal_cmd_CopyTexImage2D *cmd)
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
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_CopyTexImage2D(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_CopyTexImage2D);
   struct marshal_cmd_CopyTexImage2D *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_CopyTexImage2D, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->level = level;
   cmd->internalformat = MIN2(internalformat, 0xffff); /* clamped to 0xffff (invalid enum) */
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
   GLenum16 target;
   GLint level;
   GLint xoffset;
   GLint x;
   GLint y;
   GLsizei width;
};
uint32_t
_mesa_unmarshal_CopyTexSubImage1D(struct gl_context *ctx, const struct marshal_cmd_CopyTexSubImage1D *cmd)
{
   GLenum target = cmd->target;
   GLint level = cmd->level;
   GLint xoffset = cmd->xoffset;
   GLint x = cmd->x;
   GLint y = cmd->y;
   GLsizei width = cmd->width;
   CALL_CopyTexSubImage1D(ctx->CurrentServerDispatch, (target, level, xoffset, x, y, width));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_CopyTexSubImage1D), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_CopyTexSubImage1D(GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_CopyTexSubImage1D);
   struct marshal_cmd_CopyTexSubImage1D *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_CopyTexSubImage1D, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
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
_mesa_unmarshal_CopyTexSubImage2D(struct gl_context *ctx, const struct marshal_cmd_CopyTexSubImage2D *cmd)
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
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_CopyTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_CopyTexSubImage2D);
   struct marshal_cmd_CopyTexSubImage2D *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_CopyTexSubImage2D, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
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
   GLenum16 target;
   GLenum16 format;
   GLenum16 type;
   GLint level;
   GLint xoffset;
   GLsizei width;
   const GLvoid * pixels;
};
uint32_t
_mesa_unmarshal_TexSubImage1D(struct gl_context *ctx, const struct marshal_cmd_TexSubImage1D *cmd)
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
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->level = level;
   cmd->xoffset = xoffset;
   cmd->width = width;
   cmd->format = MIN2(format, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->type = MIN2(type, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->pixels = pixels;
}


/* TexSubImage2D: marshalled asynchronously */
struct marshal_cmd_TexSubImage2D
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLenum16 format;
   GLenum16 type;
   GLint level;
   GLint xoffset;
   GLint yoffset;
   GLsizei width;
   GLsizei height;
   const GLvoid * pixels;
};
uint32_t
_mesa_unmarshal_TexSubImage2D(struct gl_context *ctx, const struct marshal_cmd_TexSubImage2D *cmd)
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
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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


/* AreTexturesResident: marshalled synchronously */
static GLboolean GLAPIENTRY
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
   GLenum16 target;
   GLuint texture;
};
uint32_t
_mesa_unmarshal_BindTexture(struct gl_context *ctx, const struct marshal_cmd_BindTexture *cmd)
{
   GLenum target = cmd->target;
   GLuint texture = cmd->texture;
   CALL_BindTexture(ctx->CurrentServerDispatch, (target, texture));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_BindTexture), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_BindTexture(GLenum target, GLuint texture)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_BindTexture);
   struct marshal_cmd_BindTexture *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_BindTexture, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
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
_mesa_unmarshal_DeleteTextures(struct gl_context *ctx, const struct marshal_cmd_DeleteTextures *cmd)
{
   GLsizei n = cmd->n;
   GLuint *textures;
   const char *variable_data = (const char *) (cmd + 1);
   textures = (GLuint *) variable_data;
   CALL_DeleteTextures(ctx->CurrentServerDispatch, (n, textures));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
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
static void GLAPIENTRY
_mesa_marshal_GenTextures(GLsizei n, GLuint * textures)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GenTextures");
   CALL_GenTextures(ctx->CurrentServerDispatch, (n, textures));
}


/* IsTexture: marshalled synchronously */
static GLboolean GLAPIENTRY
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
_mesa_unmarshal_PrioritizeTextures(struct gl_context *ctx, const struct marshal_cmd_PrioritizeTextures *cmd)
{
   GLsizei n = cmd->n;
   GLuint *textures;
   GLclampf *priorities;
   const char *variable_data = (const char *) (cmd + 1);
   textures = (GLuint *) variable_data;
   variable_data += n * 1 * sizeof(GLuint);
   priorities = (GLclampf *) variable_data;
   CALL_PrioritizeTextures(ctx->CurrentServerDispatch, (n, textures, priorities));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
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
_mesa_unmarshal_Indexub(struct gl_context *ctx, const struct marshal_cmd_Indexub *cmd)
{
   GLubyte c = cmd->c;
   CALL_Indexub(ctx->CurrentServerDispatch, (c));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Indexub), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
_mesa_unmarshal_Indexubv(struct gl_context *ctx, const struct marshal_cmd_Indexubv *cmd)
{
   const GLubyte *c = cmd->c;
   CALL_Indexubv(ctx->CurrentServerDispatch, (c));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_Indexubv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
_mesa_unmarshal_PopClientAttrib(struct gl_context *ctx, const struct marshal_cmd_PopClientAttrib *cmd)
{
   CALL_PopClientAttrib(ctx->CurrentServerDispatch, ());
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_PopClientAttrib), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
_mesa_unmarshal_PushClientAttrib(struct gl_context *ctx, const struct marshal_cmd_PushClientAttrib *cmd)
{
   GLbitfield mask = cmd->mask;
   CALL_PushClientAttrib(ctx->CurrentServerDispatch, (mask));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_PushClientAttrib), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
_mesa_unmarshal_BlendColor(struct gl_context *ctx, const struct marshal_cmd_BlendColor *cmd)
{
   GLclampf red = cmd->red;
   GLclampf green = cmd->green;
   GLclampf blue = cmd->blue;
   GLclampf alpha = cmd->alpha;
   CALL_BlendColor(ctx->CurrentServerDispatch, (red, green, blue, alpha));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_BlendColor), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
   GLenum16 mode;
};
uint32_t
_mesa_unmarshal_BlendEquation(struct gl_context *ctx, const struct marshal_cmd_BlendEquation *cmd)
{
   GLenum mode = cmd->mode;
   CALL_BlendEquation(ctx->CurrentServerDispatch, (mode));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_BlendEquation), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_BlendEquation(GLenum mode)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_BlendEquation);
   struct marshal_cmd_BlendEquation *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_BlendEquation, cmd_size);
   cmd->mode = MIN2(mode, 0xffff); /* clamped to 0xffff (invalid enum) */
}


/* TexImage3D: marshalled asynchronously */
struct marshal_cmd_TexImage3D
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLenum16 format;
   GLenum16 type;
   GLint level;
   GLint internalformat;
   GLsizei width;
   GLsizei height;
   GLsizei depth;
   GLint border;
   const GLvoid * pixels;
};
uint32_t
_mesa_unmarshal_TexImage3D(struct gl_context *ctx, const struct marshal_cmd_TexImage3D *cmd)
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
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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


/* TexSubImage3D: marshalled asynchronously */
struct marshal_cmd_TexSubImage3D
{
   struct marshal_cmd_base cmd_base;
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
   const GLvoid * pixels;
};
uint32_t
_mesa_unmarshal_TexSubImage3D(struct gl_context *ctx, const struct marshal_cmd_TexSubImage3D *cmd)
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
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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


/* CopyTexSubImage3D: marshalled asynchronously */
struct marshal_cmd_CopyTexSubImage3D
{
   struct marshal_cmd_base cmd_base;
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
_mesa_unmarshal_CopyTexSubImage3D(struct gl_context *ctx, const struct marshal_cmd_CopyTexSubImage3D *cmd)
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
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_CopyTexSubImage3D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_CopyTexSubImage3D);
   struct marshal_cmd_CopyTexSubImage3D *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_CopyTexSubImage3D, cmd_size);
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


/* ActiveTexture: marshalled asynchronously */
struct marshal_cmd_ActiveTexture
{
   struct marshal_cmd_base cmd_base;
   GLenum16 texture;
};
uint32_t
_mesa_unmarshal_ActiveTexture(struct gl_context *ctx, const struct marshal_cmd_ActiveTexture *cmd)
{
   GLenum texture = cmd->texture;
   CALL_ActiveTexture(ctx->CurrentServerDispatch, (texture));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_ActiveTexture), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ActiveTexture(GLenum texture)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ActiveTexture);
   struct marshal_cmd_ActiveTexture *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ActiveTexture, cmd_size);
   cmd->texture = MIN2(texture, 0xffff); /* clamped to 0xffff (invalid enum) */
   ctx->GLThread.ActiveTexture = texture - GL_TEXTURE0; if (ctx->GLThread.MatrixMode == GL_TEXTURE) ctx->GLThread.MatrixIndex = _mesa_get_matrix_index(ctx, texture);
}


/* ClientActiveTexture: marshalled asynchronously */
struct marshal_cmd_ClientActiveTexture
{
   struct marshal_cmd_base cmd_base;
   GLenum16 texture;
};
uint32_t
_mesa_unmarshal_ClientActiveTexture(struct gl_context *ctx, const struct marshal_cmd_ClientActiveTexture *cmd)
{
   GLenum texture = cmd->texture;
   CALL_ClientActiveTexture(ctx->CurrentServerDispatch, (texture));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_ClientActiveTexture), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_ClientActiveTexture(GLenum texture)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ClientActiveTexture);
   struct marshal_cmd_ClientActiveTexture *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ClientActiveTexture, cmd_size);
   cmd->texture = MIN2(texture, 0xffff); /* clamped to 0xffff (invalid enum) */
   ctx->GLThread.ClientActiveTexture = texture - GL_TEXTURE0;
}


/* MultiTexCoord1d: marshalled asynchronously */
struct marshal_cmd_MultiTexCoord1d
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLdouble s;
};
uint32_t
_mesa_unmarshal_MultiTexCoord1d(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord1d *cmd)
{
   GLenum target = cmd->target;
   GLdouble s = cmd->s;
   CALL_MultiTexCoord1d(ctx->CurrentServerDispatch, (target, s));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MultiTexCoord1d), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_MultiTexCoord1d(GLenum target, GLdouble s)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexCoord1d);
   struct marshal_cmd_MultiTexCoord1d *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexCoord1d, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->s = s;
}


/* MultiTexCoord1dv: marshalled asynchronously */
struct marshal_cmd_MultiTexCoord1dv
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLdouble v[1];
};
uint32_t
_mesa_unmarshal_MultiTexCoord1dv(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord1dv *cmd)
{
   GLenum target = cmd->target;
   const GLdouble *v = cmd->v;
   CALL_MultiTexCoord1dv(ctx->CurrentServerDispatch, (target, v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MultiTexCoord1dv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_MultiTexCoord1dv(GLenum target, const GLdouble * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexCoord1dv);
   struct marshal_cmd_MultiTexCoord1dv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexCoord1dv, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   memcpy(cmd->v, v, 1 * sizeof(GLdouble));
}


/* MultiTexCoord1fARB: marshalled asynchronously */
struct marshal_cmd_MultiTexCoord1fARB
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLfloat s;
};
uint32_t
_mesa_unmarshal_MultiTexCoord1fARB(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord1fARB *cmd)
{
   GLenum target = cmd->target;
   GLfloat s = cmd->s;
   CALL_MultiTexCoord1fARB(ctx->CurrentServerDispatch, (target, s));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MultiTexCoord1fARB), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_MultiTexCoord1fARB(GLenum target, GLfloat s)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexCoord1fARB);
   struct marshal_cmd_MultiTexCoord1fARB *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexCoord1fARB, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->s = s;
}


/* MultiTexCoord1fvARB: marshalled asynchronously */
struct marshal_cmd_MultiTexCoord1fvARB
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLfloat v[1];
};
uint32_t
_mesa_unmarshal_MultiTexCoord1fvARB(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord1fvARB *cmd)
{
   GLenum target = cmd->target;
   const GLfloat *v = cmd->v;
   CALL_MultiTexCoord1fvARB(ctx->CurrentServerDispatch, (target, v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MultiTexCoord1fvARB), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_MultiTexCoord1fvARB(GLenum target, const GLfloat * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexCoord1fvARB);
   struct marshal_cmd_MultiTexCoord1fvARB *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexCoord1fvARB, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   memcpy(cmd->v, v, 1 * sizeof(GLfloat));
}


/* MultiTexCoord1i: marshalled asynchronously */
struct marshal_cmd_MultiTexCoord1i
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLint s;
};
uint32_t
_mesa_unmarshal_MultiTexCoord1i(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord1i *cmd)
{
   GLenum target = cmd->target;
   GLint s = cmd->s;
   CALL_MultiTexCoord1i(ctx->CurrentServerDispatch, (target, s));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MultiTexCoord1i), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_MultiTexCoord1i(GLenum target, GLint s)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexCoord1i);
   struct marshal_cmd_MultiTexCoord1i *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexCoord1i, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->s = s;
}


/* MultiTexCoord1iv: marshalled asynchronously */
struct marshal_cmd_MultiTexCoord1iv
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLint v[1];
};
uint32_t
_mesa_unmarshal_MultiTexCoord1iv(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord1iv *cmd)
{
   GLenum target = cmd->target;
   const GLint *v = cmd->v;
   CALL_MultiTexCoord1iv(ctx->CurrentServerDispatch, (target, v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MultiTexCoord1iv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_MultiTexCoord1iv(GLenum target, const GLint * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexCoord1iv);
   struct marshal_cmd_MultiTexCoord1iv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexCoord1iv, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   memcpy(cmd->v, v, 1 * sizeof(GLint));
}


/* MultiTexCoord1s: marshalled asynchronously */
struct marshal_cmd_MultiTexCoord1s
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLshort s;
};
uint32_t
_mesa_unmarshal_MultiTexCoord1s(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord1s *cmd)
{
   GLenum target = cmd->target;
   GLshort s = cmd->s;
   CALL_MultiTexCoord1s(ctx->CurrentServerDispatch, (target, s));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MultiTexCoord1s), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_MultiTexCoord1s(GLenum target, GLshort s)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexCoord1s);
   struct marshal_cmd_MultiTexCoord1s *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexCoord1s, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->s = s;
}


/* MultiTexCoord1sv: marshalled asynchronously */
struct marshal_cmd_MultiTexCoord1sv
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLshort v[1];
};
uint32_t
_mesa_unmarshal_MultiTexCoord1sv(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord1sv *cmd)
{
   GLenum target = cmd->target;
   const GLshort *v = cmd->v;
   CALL_MultiTexCoord1sv(ctx->CurrentServerDispatch, (target, v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MultiTexCoord1sv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_MultiTexCoord1sv(GLenum target, const GLshort * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexCoord1sv);
   struct marshal_cmd_MultiTexCoord1sv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexCoord1sv, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   memcpy(cmd->v, v, 1 * sizeof(GLshort));
}


/* MultiTexCoord2d: marshalled asynchronously */
struct marshal_cmd_MultiTexCoord2d
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLdouble s;
   GLdouble t;
};
uint32_t
_mesa_unmarshal_MultiTexCoord2d(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord2d *cmd)
{
   GLenum target = cmd->target;
   GLdouble s = cmd->s;
   GLdouble t = cmd->t;
   CALL_MultiTexCoord2d(ctx->CurrentServerDispatch, (target, s, t));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MultiTexCoord2d), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_MultiTexCoord2d(GLenum target, GLdouble s, GLdouble t)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexCoord2d);
   struct marshal_cmd_MultiTexCoord2d *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexCoord2d, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->s = s;
   cmd->t = t;
}


/* MultiTexCoord2dv: marshalled asynchronously */
struct marshal_cmd_MultiTexCoord2dv
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLdouble v[2];
};
uint32_t
_mesa_unmarshal_MultiTexCoord2dv(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord2dv *cmd)
{
   GLenum target = cmd->target;
   const GLdouble *v = cmd->v;
   CALL_MultiTexCoord2dv(ctx->CurrentServerDispatch, (target, v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MultiTexCoord2dv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_MultiTexCoord2dv(GLenum target, const GLdouble * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexCoord2dv);
   struct marshal_cmd_MultiTexCoord2dv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexCoord2dv, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   memcpy(cmd->v, v, 2 * sizeof(GLdouble));
}


/* MultiTexCoord2fARB: marshalled asynchronously */
struct marshal_cmd_MultiTexCoord2fARB
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLfloat s;
   GLfloat t;
};
uint32_t
_mesa_unmarshal_MultiTexCoord2fARB(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord2fARB *cmd)
{
   GLenum target = cmd->target;
   GLfloat s = cmd->s;
   GLfloat t = cmd->t;
   CALL_MultiTexCoord2fARB(ctx->CurrentServerDispatch, (target, s, t));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MultiTexCoord2fARB), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_MultiTexCoord2fARB(GLenum target, GLfloat s, GLfloat t)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexCoord2fARB);
   struct marshal_cmd_MultiTexCoord2fARB *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexCoord2fARB, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->s = s;
   cmd->t = t;
}


/* MultiTexCoord2fvARB: marshalled asynchronously */
struct marshal_cmd_MultiTexCoord2fvARB
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLfloat v[2];
};
uint32_t
_mesa_unmarshal_MultiTexCoord2fvARB(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord2fvARB *cmd)
{
   GLenum target = cmd->target;
   const GLfloat *v = cmd->v;
   CALL_MultiTexCoord2fvARB(ctx->CurrentServerDispatch, (target, v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MultiTexCoord2fvARB), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_MultiTexCoord2fvARB(GLenum target, const GLfloat * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexCoord2fvARB);
   struct marshal_cmd_MultiTexCoord2fvARB *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexCoord2fvARB, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   memcpy(cmd->v, v, 2 * sizeof(GLfloat));
}


/* MultiTexCoord2i: marshalled asynchronously */
struct marshal_cmd_MultiTexCoord2i
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLint s;
   GLint t;
};
uint32_t
_mesa_unmarshal_MultiTexCoord2i(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord2i *cmd)
{
   GLenum target = cmd->target;
   GLint s = cmd->s;
   GLint t = cmd->t;
   CALL_MultiTexCoord2i(ctx->CurrentServerDispatch, (target, s, t));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MultiTexCoord2i), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_MultiTexCoord2i(GLenum target, GLint s, GLint t)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexCoord2i);
   struct marshal_cmd_MultiTexCoord2i *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexCoord2i, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->s = s;
   cmd->t = t;
}


/* MultiTexCoord2iv: marshalled asynchronously */
struct marshal_cmd_MultiTexCoord2iv
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLint v[2];
};
uint32_t
_mesa_unmarshal_MultiTexCoord2iv(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord2iv *cmd)
{
   GLenum target = cmd->target;
   const GLint *v = cmd->v;
   CALL_MultiTexCoord2iv(ctx->CurrentServerDispatch, (target, v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MultiTexCoord2iv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_MultiTexCoord2iv(GLenum target, const GLint * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexCoord2iv);
   struct marshal_cmd_MultiTexCoord2iv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexCoord2iv, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   memcpy(cmd->v, v, 2 * sizeof(GLint));
}


/* MultiTexCoord2s: marshalled asynchronously */
struct marshal_cmd_MultiTexCoord2s
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLshort s;
   GLshort t;
};
uint32_t
_mesa_unmarshal_MultiTexCoord2s(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord2s *cmd)
{
   GLenum target = cmd->target;
   GLshort s = cmd->s;
   GLshort t = cmd->t;
   CALL_MultiTexCoord2s(ctx->CurrentServerDispatch, (target, s, t));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MultiTexCoord2s), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_MultiTexCoord2s(GLenum target, GLshort s, GLshort t)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexCoord2s);
   struct marshal_cmd_MultiTexCoord2s *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexCoord2s, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->s = s;
   cmd->t = t;
}


/* MultiTexCoord2sv: marshalled asynchronously */
struct marshal_cmd_MultiTexCoord2sv
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLshort v[2];
};
uint32_t
_mesa_unmarshal_MultiTexCoord2sv(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord2sv *cmd)
{
   GLenum target = cmd->target;
   const GLshort *v = cmd->v;
   CALL_MultiTexCoord2sv(ctx->CurrentServerDispatch, (target, v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MultiTexCoord2sv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_MultiTexCoord2sv(GLenum target, const GLshort * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexCoord2sv);
   struct marshal_cmd_MultiTexCoord2sv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexCoord2sv, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   memcpy(cmd->v, v, 2 * sizeof(GLshort));
}


/* MultiTexCoord3d: marshalled asynchronously */
struct marshal_cmd_MultiTexCoord3d
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLdouble s;
   GLdouble t;
   GLdouble r;
};
uint32_t
_mesa_unmarshal_MultiTexCoord3d(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord3d *cmd)
{
   GLenum target = cmd->target;
   GLdouble s = cmd->s;
   GLdouble t = cmd->t;
   GLdouble r = cmd->r;
   CALL_MultiTexCoord3d(ctx->CurrentServerDispatch, (target, s, t, r));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MultiTexCoord3d), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_MultiTexCoord3d(GLenum target, GLdouble s, GLdouble t, GLdouble r)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexCoord3d);
   struct marshal_cmd_MultiTexCoord3d *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexCoord3d, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->s = s;
   cmd->t = t;
   cmd->r = r;
}


/* MultiTexCoord3dv: marshalled asynchronously */
struct marshal_cmd_MultiTexCoord3dv
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLdouble v[3];
};
uint32_t
_mesa_unmarshal_MultiTexCoord3dv(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord3dv *cmd)
{
   GLenum target = cmd->target;
   const GLdouble *v = cmd->v;
   CALL_MultiTexCoord3dv(ctx->CurrentServerDispatch, (target, v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MultiTexCoord3dv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_MultiTexCoord3dv(GLenum target, const GLdouble * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexCoord3dv);
   struct marshal_cmd_MultiTexCoord3dv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexCoord3dv, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   memcpy(cmd->v, v, 3 * sizeof(GLdouble));
}


/* MultiTexCoord3fARB: marshalled asynchronously */
struct marshal_cmd_MultiTexCoord3fARB
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLfloat s;
   GLfloat t;
   GLfloat r;
};
uint32_t
_mesa_unmarshal_MultiTexCoord3fARB(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord3fARB *cmd)
{
   GLenum target = cmd->target;
   GLfloat s = cmd->s;
   GLfloat t = cmd->t;
   GLfloat r = cmd->r;
   CALL_MultiTexCoord3fARB(ctx->CurrentServerDispatch, (target, s, t, r));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MultiTexCoord3fARB), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_MultiTexCoord3fARB(GLenum target, GLfloat s, GLfloat t, GLfloat r)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexCoord3fARB);
   struct marshal_cmd_MultiTexCoord3fARB *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexCoord3fARB, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->s = s;
   cmd->t = t;
   cmd->r = r;
}


/* MultiTexCoord3fvARB: marshalled asynchronously */
struct marshal_cmd_MultiTexCoord3fvARB
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLfloat v[3];
};
uint32_t
_mesa_unmarshal_MultiTexCoord3fvARB(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord3fvARB *cmd)
{
   GLenum target = cmd->target;
   const GLfloat *v = cmd->v;
   CALL_MultiTexCoord3fvARB(ctx->CurrentServerDispatch, (target, v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MultiTexCoord3fvARB), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_MultiTexCoord3fvARB(GLenum target, const GLfloat * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexCoord3fvARB);
   struct marshal_cmd_MultiTexCoord3fvARB *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexCoord3fvARB, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   memcpy(cmd->v, v, 3 * sizeof(GLfloat));
}


/* MultiTexCoord3i: marshalled asynchronously */
struct marshal_cmd_MultiTexCoord3i
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLint s;
   GLint t;
   GLint r;
};
uint32_t
_mesa_unmarshal_MultiTexCoord3i(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord3i *cmd)
{
   GLenum target = cmd->target;
   GLint s = cmd->s;
   GLint t = cmd->t;
   GLint r = cmd->r;
   CALL_MultiTexCoord3i(ctx->CurrentServerDispatch, (target, s, t, r));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MultiTexCoord3i), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_MultiTexCoord3i(GLenum target, GLint s, GLint t, GLint r)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexCoord3i);
   struct marshal_cmd_MultiTexCoord3i *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexCoord3i, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->s = s;
   cmd->t = t;
   cmd->r = r;
}


/* MultiTexCoord3iv: marshalled asynchronously */
struct marshal_cmd_MultiTexCoord3iv
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLint v[3];
};
uint32_t
_mesa_unmarshal_MultiTexCoord3iv(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord3iv *cmd)
{
   GLenum target = cmd->target;
   const GLint *v = cmd->v;
   CALL_MultiTexCoord3iv(ctx->CurrentServerDispatch, (target, v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MultiTexCoord3iv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_MultiTexCoord3iv(GLenum target, const GLint * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexCoord3iv);
   struct marshal_cmd_MultiTexCoord3iv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexCoord3iv, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   memcpy(cmd->v, v, 3 * sizeof(GLint));
}


/* MultiTexCoord3s: marshalled asynchronously */
struct marshal_cmd_MultiTexCoord3s
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLshort s;
   GLshort t;
   GLshort r;
};
uint32_t
_mesa_unmarshal_MultiTexCoord3s(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord3s *cmd)
{
   GLenum target = cmd->target;
   GLshort s = cmd->s;
   GLshort t = cmd->t;
   GLshort r = cmd->r;
   CALL_MultiTexCoord3s(ctx->CurrentServerDispatch, (target, s, t, r));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MultiTexCoord3s), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_MultiTexCoord3s(GLenum target, GLshort s, GLshort t, GLshort r)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexCoord3s);
   struct marshal_cmd_MultiTexCoord3s *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexCoord3s, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->s = s;
   cmd->t = t;
   cmd->r = r;
}


/* MultiTexCoord3sv: marshalled asynchronously */
struct marshal_cmd_MultiTexCoord3sv
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLshort v[3];
};
uint32_t
_mesa_unmarshal_MultiTexCoord3sv(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord3sv *cmd)
{
   GLenum target = cmd->target;
   const GLshort *v = cmd->v;
   CALL_MultiTexCoord3sv(ctx->CurrentServerDispatch, (target, v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MultiTexCoord3sv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_MultiTexCoord3sv(GLenum target, const GLshort * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexCoord3sv);
   struct marshal_cmd_MultiTexCoord3sv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexCoord3sv, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   memcpy(cmd->v, v, 3 * sizeof(GLshort));
}


/* MultiTexCoord4d: marshalled asynchronously */
struct marshal_cmd_MultiTexCoord4d
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLdouble s;
   GLdouble t;
   GLdouble r;
   GLdouble q;
};
uint32_t
_mesa_unmarshal_MultiTexCoord4d(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord4d *cmd)
{
   GLenum target = cmd->target;
   GLdouble s = cmd->s;
   GLdouble t = cmd->t;
   GLdouble r = cmd->r;
   GLdouble q = cmd->q;
   CALL_MultiTexCoord4d(ctx->CurrentServerDispatch, (target, s, t, r, q));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MultiTexCoord4d), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_MultiTexCoord4d(GLenum target, GLdouble s, GLdouble t, GLdouble r, GLdouble q)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexCoord4d);
   struct marshal_cmd_MultiTexCoord4d *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexCoord4d, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->s = s;
   cmd->t = t;
   cmd->r = r;
   cmd->q = q;
}


/* MultiTexCoord4dv: marshalled asynchronously */
struct marshal_cmd_MultiTexCoord4dv
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLdouble v[4];
};
uint32_t
_mesa_unmarshal_MultiTexCoord4dv(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord4dv *cmd)
{
   GLenum target = cmd->target;
   const GLdouble *v = cmd->v;
   CALL_MultiTexCoord4dv(ctx->CurrentServerDispatch, (target, v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MultiTexCoord4dv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_MultiTexCoord4dv(GLenum target, const GLdouble * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexCoord4dv);
   struct marshal_cmd_MultiTexCoord4dv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexCoord4dv, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   memcpy(cmd->v, v, 4 * sizeof(GLdouble));
}


/* MultiTexCoord4fARB: marshalled asynchronously */
struct marshal_cmd_MultiTexCoord4fARB
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLfloat s;
   GLfloat t;
   GLfloat r;
   GLfloat q;
};
uint32_t
_mesa_unmarshal_MultiTexCoord4fARB(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord4fARB *cmd)
{
   GLenum target = cmd->target;
   GLfloat s = cmd->s;
   GLfloat t = cmd->t;
   GLfloat r = cmd->r;
   GLfloat q = cmd->q;
   CALL_MultiTexCoord4fARB(ctx->CurrentServerDispatch, (target, s, t, r, q));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MultiTexCoord4fARB), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_MultiTexCoord4fARB(GLenum target, GLfloat s, GLfloat t, GLfloat r, GLfloat q)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexCoord4fARB);
   struct marshal_cmd_MultiTexCoord4fARB *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexCoord4fARB, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->s = s;
   cmd->t = t;
   cmd->r = r;
   cmd->q = q;
}


/* MultiTexCoord4fvARB: marshalled asynchronously */
struct marshal_cmd_MultiTexCoord4fvARB
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLfloat v[4];
};
uint32_t
_mesa_unmarshal_MultiTexCoord4fvARB(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord4fvARB *cmd)
{
   GLenum target = cmd->target;
   const GLfloat *v = cmd->v;
   CALL_MultiTexCoord4fvARB(ctx->CurrentServerDispatch, (target, v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MultiTexCoord4fvARB), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_MultiTexCoord4fvARB(GLenum target, const GLfloat * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexCoord4fvARB);
   struct marshal_cmd_MultiTexCoord4fvARB *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexCoord4fvARB, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   memcpy(cmd->v, v, 4 * sizeof(GLfloat));
}


/* MultiTexCoord4i: marshalled asynchronously */
struct marshal_cmd_MultiTexCoord4i
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLint s;
   GLint t;
   GLint r;
   GLint q;
};
uint32_t
_mesa_unmarshal_MultiTexCoord4i(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord4i *cmd)
{
   GLenum target = cmd->target;
   GLint s = cmd->s;
   GLint t = cmd->t;
   GLint r = cmd->r;
   GLint q = cmd->q;
   CALL_MultiTexCoord4i(ctx->CurrentServerDispatch, (target, s, t, r, q));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MultiTexCoord4i), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_MultiTexCoord4i(GLenum target, GLint s, GLint t, GLint r, GLint q)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexCoord4i);
   struct marshal_cmd_MultiTexCoord4i *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexCoord4i, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->s = s;
   cmd->t = t;
   cmd->r = r;
   cmd->q = q;
}


/* MultiTexCoord4iv: marshalled asynchronously */
struct marshal_cmd_MultiTexCoord4iv
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLint v[4];
};
uint32_t
_mesa_unmarshal_MultiTexCoord4iv(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord4iv *cmd)
{
   GLenum target = cmd->target;
   const GLint *v = cmd->v;
   CALL_MultiTexCoord4iv(ctx->CurrentServerDispatch, (target, v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MultiTexCoord4iv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_MultiTexCoord4iv(GLenum target, const GLint * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexCoord4iv);
   struct marshal_cmd_MultiTexCoord4iv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexCoord4iv, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   memcpy(cmd->v, v, 4 * sizeof(GLint));
}


/* MultiTexCoord4s: marshalled asynchronously */
struct marshal_cmd_MultiTexCoord4s
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLshort s;
   GLshort t;
   GLshort r;
   GLshort q;
};
uint32_t
_mesa_unmarshal_MultiTexCoord4s(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord4s *cmd)
{
   GLenum target = cmd->target;
   GLshort s = cmd->s;
   GLshort t = cmd->t;
   GLshort r = cmd->r;
   GLshort q = cmd->q;
   CALL_MultiTexCoord4s(ctx->CurrentServerDispatch, (target, s, t, r, q));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MultiTexCoord4s), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_MultiTexCoord4s(GLenum target, GLshort s, GLshort t, GLshort r, GLshort q)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexCoord4s);
   struct marshal_cmd_MultiTexCoord4s *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexCoord4s, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->s = s;
   cmd->t = t;
   cmd->r = r;
   cmd->q = q;
}


/* MultiTexCoord4sv: marshalled asynchronously */
struct marshal_cmd_MultiTexCoord4sv
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLshort v[4];
};
uint32_t
_mesa_unmarshal_MultiTexCoord4sv(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord4sv *cmd)
{
   GLenum target = cmd->target;
   const GLshort *v = cmd->v;
   CALL_MultiTexCoord4sv(ctx->CurrentServerDispatch, (target, v));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MultiTexCoord4sv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_MultiTexCoord4sv(GLenum target, const GLshort * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexCoord4sv);
   struct marshal_cmd_MultiTexCoord4sv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexCoord4sv, cmd_size);
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   memcpy(cmd->v, v, 4 * sizeof(GLshort));
}


/* LoadTransposeMatrixf: marshalled asynchronously */
struct marshal_cmd_LoadTransposeMatrixf
{
   struct marshal_cmd_base cmd_base;
   GLfloat m[16];
};
uint32_t
_mesa_unmarshal_LoadTransposeMatrixf(struct gl_context *ctx, const struct marshal_cmd_LoadTransposeMatrixf *cmd)
{
   const GLfloat *m = cmd->m;
   CALL_LoadTransposeMatrixf(ctx->CurrentServerDispatch, (m));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_LoadTransposeMatrixf), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
_mesa_unmarshal_LoadTransposeMatrixd(struct gl_context *ctx, const struct marshal_cmd_LoadTransposeMatrixd *cmd)
{
   const GLdouble *m = cmd->m;
   CALL_LoadTransposeMatrixd(ctx->CurrentServerDispatch, (m));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_LoadTransposeMatrixd), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
_mesa_unmarshal_MultTransposeMatrixf(struct gl_context *ctx, const struct marshal_cmd_MultTransposeMatrixf *cmd)
{
   const GLfloat *m = cmd->m;
   CALL_MultTransposeMatrixf(ctx->CurrentServerDispatch, (m));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MultTransposeMatrixf), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
_mesa_unmarshal_MultTransposeMatrixd(struct gl_context *ctx, const struct marshal_cmd_MultTransposeMatrixd *cmd)
{
   const GLdouble *m = cmd->m;
   CALL_MultTransposeMatrixd(ctx->CurrentServerDispatch, (m));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_MultTransposeMatrixd), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
_mesa_unmarshal_SampleCoverage(struct gl_context *ctx, const struct marshal_cmd_SampleCoverage *cmd)
{
   GLclampf value = cmd->value;
   GLboolean invert = cmd->invert;
   CALL_SampleCoverage(ctx->CurrentServerDispatch, (value, invert));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_SampleCoverage), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
   GLenum16 target;
   GLenum16 internalformat;
   GLint level;
   GLsizei width;
   GLsizei height;
   GLsizei depth;
   GLint border;
   GLsizei imageSize;
   const GLvoid * data;
};
uint32_t
_mesa_unmarshal_CompressedTexImage3D(struct gl_context *ctx, const struct marshal_cmd_CompressedTexImage3D *cmd)
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
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->level = level;
   cmd->internalformat = MIN2(internalformat, 0xffff); /* clamped to 0xffff (invalid enum) */
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
   GLenum16 target;
   GLenum16 internalformat;
   GLint level;
   GLsizei width;
   GLsizei height;
   GLint border;
   GLsizei imageSize;
   const GLvoid * data;
};
uint32_t
_mesa_unmarshal_CompressedTexImage2D(struct gl_context *ctx, const struct marshal_cmd_CompressedTexImage2D *cmd)
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
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->level = level;
   cmd->internalformat = MIN2(internalformat, 0xffff); /* clamped to 0xffff (invalid enum) */
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
   GLenum16 target;
   GLenum16 internalformat;
   GLint level;
   GLsizei width;
   GLint border;
   GLsizei imageSize;
   const GLvoid * data;
};
uint32_t
_mesa_unmarshal_CompressedTexImage1D(struct gl_context *ctx, const struct marshal_cmd_CompressedTexImage1D *cmd)
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
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->level = level;
   cmd->internalformat = MIN2(internalformat, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->width = width;
   cmd->border = border;
   cmd->imageSize = imageSize;
   cmd->data = data;
}


/* CompressedTexSubImage3D: marshalled asynchronously */
struct marshal_cmd_CompressedTexSubImage3D
{
   struct marshal_cmd_base cmd_base;
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
_mesa_unmarshal_CompressedTexSubImage3D(struct gl_context *ctx, const struct marshal_cmd_CompressedTexSubImage3D *cmd)
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
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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


/* CompressedTexSubImage2D: marshalled asynchronously */
struct marshal_cmd_CompressedTexSubImage2D
{
   struct marshal_cmd_base cmd_base;
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
_mesa_unmarshal_CompressedTexSubImage2D(struct gl_context *ctx, const struct marshal_cmd_CompressedTexSubImage2D *cmd)
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
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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


/* CompressedTexSubImage1D: marshalled asynchronously */
struct marshal_cmd_CompressedTexSubImage1D
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLenum16 format;
   GLint level;
   GLint xoffset;
   GLsizei width;
   GLsizei imageSize;
   const GLvoid * data;
};
uint32_t
_mesa_unmarshal_CompressedTexSubImage1D(struct gl_context *ctx, const struct marshal_cmd_CompressedTexSubImage1D *cmd)
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
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->level = level;
   cmd->xoffset = xoffset;
   cmd->width = width;
   cmd->format = MIN2(format, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->imageSize = imageSize;
   cmd->data = data;
}


/* GetCompressedTexImage: marshalled asynchronously */
struct marshal_cmd_GetCompressedTexImage
{
   struct marshal_cmd_base cmd_base;
   GLenum16 target;
   GLint level;
   GLvoid * img;
};
uint32_t
_mesa_unmarshal_GetCompressedTexImage(struct gl_context *ctx, const struct marshal_cmd_GetCompressedTexImage *cmd)
{
   GLenum target = cmd->target;
   GLint level = cmd->level;
   GLvoid * img = cmd->img;
   CALL_GetCompressedTexImage(ctx->CurrentServerDispatch, (target, level, img));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_GetCompressedTexImage), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
   cmd->target = MIN2(target, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->level = level;
   cmd->img = img;
}


/* BlendFuncSeparate: marshalled asynchronously */
struct marshal_cmd_BlendFuncSeparate
{
   struct marshal_cmd_base cmd_base;
   GLenum16 sfactorRGB;
   GLenum16 dfactorRGB;
   GLenum16 sfactorAlpha;
   GLenum16 dfactorAlpha;
};
uint32_t
_mesa_unmarshal_BlendFuncSeparate(struct gl_context *ctx, const struct marshal_cmd_BlendFuncSeparate *cmd)
{
   GLenum sfactorRGB = cmd->sfactorRGB;
   GLenum dfactorRGB = cmd->dfactorRGB;
   GLenum sfactorAlpha = cmd->sfactorAlpha;
   GLenum dfactorAlpha = cmd->dfactorAlpha;
   CALL_BlendFuncSeparate(ctx->CurrentServerDispatch, (sfactorRGB, dfactorRGB, sfactorAlpha, dfactorAlpha));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_BlendFuncSeparate), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_BlendFuncSeparate(GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_BlendFuncSeparate);
   struct marshal_cmd_BlendFuncSeparate *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_BlendFuncSeparate, cmd_size);
   cmd->sfactorRGB = MIN2(sfactorRGB, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->dfactorRGB = MIN2(dfactorRGB, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->sfactorAlpha = MIN2(sfactorAlpha, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->dfactorAlpha = MIN2(dfactorAlpha, 0xffff); /* clamped to 0xffff (invalid enum) */
}


/* FogCoordfEXT: marshalled asynchronously */
struct marshal_cmd_FogCoordfEXT
{
   struct marshal_cmd_base cmd_base;
   GLfloat coord;
};
uint32_t
_mesa_unmarshal_FogCoordfEXT(struct gl_context *ctx, const struct marshal_cmd_FogCoordfEXT *cmd)
{
   GLfloat coord = cmd->coord;
   CALL_FogCoordfEXT(ctx->CurrentServerDispatch, (coord));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_FogCoordfEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
_mesa_unmarshal_FogCoordfvEXT(struct gl_context *ctx, const struct marshal_cmd_FogCoordfvEXT *cmd)
{
   const GLfloat *coord = cmd->coord;
   CALL_FogCoordfvEXT(ctx->CurrentServerDispatch, (coord));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_FogCoordfvEXT), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
_mesa_unmarshal_FogCoordd(struct gl_context *ctx, const struct marshal_cmd_FogCoordd *cmd)
{
   GLdouble coord = cmd->coord;
   CALL_FogCoordd(ctx->CurrentServerDispatch, (coord));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_FogCoordd), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
_mesa_unmarshal_FogCoorddv(struct gl_context *ctx, const struct marshal_cmd_FogCoorddv *cmd)
{
   const GLdouble *coord = cmd->coord;
   CALL_FogCoorddv(ctx->CurrentServerDispatch, (coord));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_FogCoorddv), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
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
   GLenum16 type;
   GLsizei stride;
   const GLvoid * pointer;
};
uint32_t
_mesa_unmarshal_FogCoordPointer(struct gl_context *ctx, const struct marshal_cmd_FogCoordPointer *cmd)
{
   GLenum type = cmd->type;
   GLsizei stride = cmd->stride;
   const GLvoid * pointer = cmd->pointer;
   CALL_FogCoordPointer(ctx->CurrentServerDispatch, (type, stride, pointer));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_FogCoordPointer), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_FogCoordPointer(GLenum type, GLsizei stride, const GLvoid * pointer)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_FogCoordPointer);
   struct marshal_cmd_FogCoordPointer *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_FogCoordPointer, cmd_size);
   cmd->type = MIN2(type, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->stride = stride;
   cmd->pointer = pointer;
   if (COMPAT) _mesa_glthread_AttribPointer(ctx, VERT_ATTRIB_FOG, 1, type, stride, pointer);
}


/* PointParameterf: marshalled asynchronously */
struct marshal_cmd_PointParameterf
{
   struct marshal_cmd_base cmd_base;
   GLenum16 pname;
   GLfloat param;
};
uint32_t
_mesa_unmarshal_PointParameterf(struct gl_context *ctx, const struct marshal_cmd_PointParameterf *cmd)
{
   GLenum pname = cmd->pname;
   GLfloat param = cmd->param;
   CALL_PointParameterf(ctx->CurrentServerDispatch, (pname, param));
   const unsigned cmd_size = (align(sizeof(struct marshal_cmd_PointParameterf), 8) / 8);
   assert(cmd_size == cmd->cmd_base.cmd_size);
   return cmd_size;
}
static void GLAPIENTRY
_mesa_marshal_PointParameterf(GLenum pname, GLfloat param)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_PointParameterf);
   struct marshal_cmd_PointParameterf *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_PointParameterf, cmd_size);
   cmd->pname = MIN2(pname, 0xffff); /* clamped to 0xffff (invalid enum) */
   cmd->param = param;
}


/* PointParameterfv: marshalled asynchronously */
struct marshal_cmd_PointParameterfv
{
   struct marshal_cmd_base cmd_base;
   GLenum16 pname;
   /* Next safe_mul(_mesa_point_param_enum_to_count(pname), 1 * sizeof(GLfloat)) bytes are GLfloat params[None] */
};
uint32_t
_mesa_unmarshal_PointParameterfv(struct gl_context *ctx, const struct marshal_cmd_PointParameterfv *cmd)
{
   GLenum pname = cmd->pname;
   GLfloat *params;
   const char *variable_data = (const char *) (cmd + 1);
   params = (GLfloat *) variable_data;
   CALL_PointParameterfv(ctx->CurrentServerDispatch, (pname, params));
   return cmd->cmd_base.cmd_size;
}
static void GLAPIENTRY
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
_mesa_unmarshal_PointParameteri(struct gl_context *ctx, const struct marshal_cmd_PointParameteri *cmd)
{
   GLenum pname = cmd->pname;
   GLint param = cmd->param;
   CALL_PointParameteri(ctx->CurrentServerDispatch, (pname, param));
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


void
_mesa_glthread_init_dispatch1(struct gl_context *ctx, struct _glapi_table *table)
{
   if (_mesa_is_desktop_gl(ctx)) {
      SET_ClearDepth(table, _mesa_marshal_ClearDepth);
      SET_CompressedTexImage1D(table, _mesa_marshal_CompressedTexImage1D);
      SET_CompressedTexSubImage1D(table, _mesa_marshal_CompressedTexSubImage1D);
      SET_CopyTexImage1D(table, _mesa_marshal_CopyTexImage1D);
      SET_CopyTexSubImage1D(table, _mesa_marshal_CopyTexSubImage1D);
      SET_DepthRange(table, _mesa_marshal_DepthRange);
      SET_DrawBuffer(table, _mesa_marshal_DrawBuffer);
      SET_GetCompressedTexImage(table, _mesa_marshal_GetCompressedTexImage);
      SET_GetDoublev(table, _mesa_marshal_GetDoublev);
      SET_GetTexImage(table, _mesa_marshal_GetTexImage);
      SET_PixelStoref(table, _mesa_marshal_PixelStoref);
      SET_PointParameteri(table, _mesa_marshal_PointParameteri);
      SET_TexSubImage1D(table, _mesa_marshal_TexSubImage1D);
   }
   if (_mesa_is_desktop_gl(ctx) || (ctx->API == API_OPENGLES2 && ctx->Version >= 31)) {
      SET_GetTexLevelParameterfv(table, _mesa_marshal_GetTexLevelParameterfv);
      SET_GetTexLevelParameteriv(table, _mesa_marshal_GetTexLevelParameteriv);
   }
   if (_mesa_is_desktop_gl(ctx) || ctx->API == API_OPENGLES) {
      SET_LogicOp(table, _mesa_marshal_LogicOp);
      SET_PointParameterf(table, _mesa_marshal_PointParameterf);
      SET_PointParameterfv(table, _mesa_marshal_PointParameterfv);
   }
   if (_mesa_is_desktop_gl(ctx) || ctx->API == API_OPENGLES || ctx->API == API_OPENGLES2) {
      SET_ActiveTexture(table, _mesa_marshal_ActiveTexture);
      SET_BindTexture(table, _mesa_marshal_BindTexture);
      SET_BlendEquation(table, _mesa_marshal_BlendEquation);
      SET_BlendFunc(table, _mesa_marshal_BlendFunc);
      SET_BlendFuncSeparate(table, _mesa_marshal_BlendFuncSeparate);
      SET_Clear(table, _mesa_marshal_Clear);
      SET_ClearColor(table, _mesa_marshal_ClearColor);
      SET_ClearStencil(table, _mesa_marshal_ClearStencil);
      SET_ColorMask(table, _mesa_marshal_ColorMask);
      SET_CompressedTexImage2D(table, _mesa_marshal_CompressedTexImage2D);
      SET_CompressedTexSubImage2D(table, _mesa_marshal_CompressedTexSubImage2D);
      SET_CopyTexImage2D(table, _mesa_marshal_CopyTexImage2D);
      SET_CopyTexSubImage2D(table, _mesa_marshal_CopyTexSubImage2D);
      SET_DeleteTextures(table, _mesa_marshal_DeleteTextures);
      SET_DepthFunc(table, _mesa_marshal_DepthFunc);
      SET_DepthMask(table, _mesa_marshal_DepthMask);
      SET_Disable(table, _mesa_marshal_Disable);
      SET_Enable(table, _mesa_marshal_Enable);
      SET_Finish(table, _mesa_marshal_Finish);
      SET_Flush(table, _mesa_marshal_Flush);
      SET_GenTextures(table, _mesa_marshal_GenTextures);
      SET_GetBooleanv(table, _mesa_marshal_GetBooleanv);
      SET_GetError(table, _mesa_marshal_GetError);
      SET_GetFloatv(table, _mesa_marshal_GetFloatv);
      SET_GetPointerv(table, _mesa_marshal_GetPointerv);
      SET_GetString(table, _mesa_marshal_GetString);
      SET_GetTexParameterfv(table, _mesa_marshal_GetTexParameterfv);
      SET_GetTexParameteriv(table, _mesa_marshal_GetTexParameteriv);
      SET_IsEnabled(table, _mesa_marshal_IsEnabled);
      SET_IsTexture(table, _mesa_marshal_IsTexture);
      SET_PixelStorei(table, _mesa_marshal_PixelStorei);
      SET_PolygonOffset(table, _mesa_marshal_PolygonOffset);
      SET_ReadPixels(table, _mesa_marshal_ReadPixels);
      SET_SampleCoverage(table, _mesa_marshal_SampleCoverage);
      SET_StencilFunc(table, _mesa_marshal_StencilFunc);
      SET_StencilMask(table, _mesa_marshal_StencilMask);
      SET_StencilOp(table, _mesa_marshal_StencilOp);
      SET_TexSubImage2D(table, _mesa_marshal_TexSubImage2D);
      SET_Viewport(table, _mesa_marshal_Viewport);
   }
   if (_mesa_is_desktop_gl(ctx) || ctx->API == API_OPENGLES2) {
      SET_BlendColor(table, _mesa_marshal_BlendColor);
      SET_CompressedTexImage3D(table, _mesa_marshal_CompressedTexImage3D);
      SET_CompressedTexSubImage3D(table, _mesa_marshal_CompressedTexSubImage3D);
      SET_CopyTexSubImage3D(table, _mesa_marshal_CopyTexSubImage3D);
      SET_ReadBuffer(table, _mesa_marshal_ReadBuffer);
      SET_TexImage3D(table, _mesa_marshal_TexImage3D);
      SET_TexSubImage3D(table, _mesa_marshal_TexSubImage3D);
   }
   if (ctx->API == API_OPENGL_COMPAT) {
      SET_Accum(table, _mesa_marshal_Accum);
      SET_AreTexturesResident(table, _mesa_marshal_AreTexturesResident);
      SET_ArrayElement(table, _mesa_marshal_ArrayElement);
      SET_ClearAccum(table, _mesa_marshal_ClearAccum);
      SET_ClearIndex(table, _mesa_marshal_ClearIndex);
      SET_CopyPixels(table, _mesa_marshal_CopyPixels);
      SET_DrawPixels(table, _mesa_marshal_DrawPixels);
      SET_EdgeFlagPointer(table, _mesa_marshal_EdgeFlagPointer);
      SET_EvalCoord1d(table, _mesa_marshal_EvalCoord1d);
      SET_EvalCoord1dv(table, _mesa_marshal_EvalCoord1dv);
      SET_EvalCoord1f(table, _mesa_marshal_EvalCoord1f);
      SET_EvalCoord1fv(table, _mesa_marshal_EvalCoord1fv);
      SET_EvalCoord2d(table, _mesa_marshal_EvalCoord2d);
      SET_EvalCoord2dv(table, _mesa_marshal_EvalCoord2dv);
      SET_EvalCoord2f(table, _mesa_marshal_EvalCoord2f);
      SET_EvalCoord2fv(table, _mesa_marshal_EvalCoord2fv);
      SET_EvalMesh1(table, _mesa_marshal_EvalMesh1);
      SET_EvalMesh2(table, _mesa_marshal_EvalMesh2);
      SET_EvalPoint1(table, _mesa_marshal_EvalPoint1);
      SET_EvalPoint2(table, _mesa_marshal_EvalPoint2);
      SET_FogCoordPointer(table, _mesa_marshal_FogCoordPointer);
      SET_FogCoordd(table, _mesa_marshal_FogCoordd);
      SET_FogCoorddv(table, _mesa_marshal_FogCoorddv);
      SET_FogCoordfEXT(table, _mesa_marshal_FogCoordfEXT);
      SET_FogCoordfvEXT(table, _mesa_marshal_FogCoordfvEXT);
      SET_Frustum(table, _mesa_marshal_Frustum);
      SET_GetClipPlane(table, _mesa_marshal_GetClipPlane);
      SET_GetLightiv(table, _mesa_marshal_GetLightiv);
      SET_GetMapdv(table, _mesa_marshal_GetMapdv);
      SET_GetMapfv(table, _mesa_marshal_GetMapfv);
      SET_GetMapiv(table, _mesa_marshal_GetMapiv);
      SET_GetMaterialiv(table, _mesa_marshal_GetMaterialiv);
      SET_GetPixelMapfv(table, _mesa_marshal_GetPixelMapfv);
      SET_GetPixelMapuiv(table, _mesa_marshal_GetPixelMapuiv);
      SET_GetPixelMapusv(table, _mesa_marshal_GetPixelMapusv);
      SET_GetPolygonStipple(table, _mesa_marshal_GetPolygonStipple);
      SET_GetTexGendv(table, _mesa_marshal_GetTexGendv);
      SET_IndexMask(table, _mesa_marshal_IndexMask);
      SET_IndexPointer(table, _mesa_marshal_IndexPointer);
      SET_Indexub(table, _mesa_marshal_Indexub);
      SET_Indexubv(table, _mesa_marshal_Indexubv);
      SET_InitNames(table, _mesa_marshal_InitNames);
      SET_InterleavedArrays(table, _mesa_marshal_InterleavedArrays);
      SET_IsList(table, _mesa_marshal_IsList);
      SET_LoadMatrixd(table, _mesa_marshal_LoadMatrixd);
      SET_LoadName(table, _mesa_marshal_LoadName);
      SET_LoadTransposeMatrixd(table, _mesa_marshal_LoadTransposeMatrixd);
      SET_LoadTransposeMatrixf(table, _mesa_marshal_LoadTransposeMatrixf);
      SET_Map1d(table, _mesa_marshal_Map1d);
      SET_Map1f(table, _mesa_marshal_Map1f);
      SET_Map2d(table, _mesa_marshal_Map2d);
      SET_Map2f(table, _mesa_marshal_Map2f);
      SET_MapGrid1d(table, _mesa_marshal_MapGrid1d);
      SET_MapGrid1f(table, _mesa_marshal_MapGrid1f);
      SET_MapGrid2d(table, _mesa_marshal_MapGrid2d);
      SET_MapGrid2f(table, _mesa_marshal_MapGrid2f);
      SET_MultMatrixd(table, _mesa_marshal_MultMatrixd);
      SET_MultTransposeMatrixd(table, _mesa_marshal_MultTransposeMatrixd);
      SET_MultTransposeMatrixf(table, _mesa_marshal_MultTransposeMatrixf);
      SET_MultiTexCoord1d(table, _mesa_marshal_MultiTexCoord1d);
      SET_MultiTexCoord1dv(table, _mesa_marshal_MultiTexCoord1dv);
      SET_MultiTexCoord1fARB(table, _mesa_marshal_MultiTexCoord1fARB);
      SET_MultiTexCoord1fvARB(table, _mesa_marshal_MultiTexCoord1fvARB);
      SET_MultiTexCoord1i(table, _mesa_marshal_MultiTexCoord1i);
      SET_MultiTexCoord1iv(table, _mesa_marshal_MultiTexCoord1iv);
      SET_MultiTexCoord1s(table, _mesa_marshal_MultiTexCoord1s);
      SET_MultiTexCoord1sv(table, _mesa_marshal_MultiTexCoord1sv);
      SET_MultiTexCoord2d(table, _mesa_marshal_MultiTexCoord2d);
      SET_MultiTexCoord2dv(table, _mesa_marshal_MultiTexCoord2dv);
      SET_MultiTexCoord2fARB(table, _mesa_marshal_MultiTexCoord2fARB);
      SET_MultiTexCoord2fvARB(table, _mesa_marshal_MultiTexCoord2fvARB);
      SET_MultiTexCoord2i(table, _mesa_marshal_MultiTexCoord2i);
      SET_MultiTexCoord2iv(table, _mesa_marshal_MultiTexCoord2iv);
      SET_MultiTexCoord2s(table, _mesa_marshal_MultiTexCoord2s);
      SET_MultiTexCoord2sv(table, _mesa_marshal_MultiTexCoord2sv);
      SET_MultiTexCoord3d(table, _mesa_marshal_MultiTexCoord3d);
      SET_MultiTexCoord3dv(table, _mesa_marshal_MultiTexCoord3dv);
      SET_MultiTexCoord3fARB(table, _mesa_marshal_MultiTexCoord3fARB);
      SET_MultiTexCoord3fvARB(table, _mesa_marshal_MultiTexCoord3fvARB);
      SET_MultiTexCoord3i(table, _mesa_marshal_MultiTexCoord3i);
      SET_MultiTexCoord3iv(table, _mesa_marshal_MultiTexCoord3iv);
      SET_MultiTexCoord3s(table, _mesa_marshal_MultiTexCoord3s);
      SET_MultiTexCoord3sv(table, _mesa_marshal_MultiTexCoord3sv);
      SET_MultiTexCoord4d(table, _mesa_marshal_MultiTexCoord4d);
      SET_MultiTexCoord4dv(table, _mesa_marshal_MultiTexCoord4dv);
      SET_MultiTexCoord4fvARB(table, _mesa_marshal_MultiTexCoord4fvARB);
      SET_MultiTexCoord4i(table, _mesa_marshal_MultiTexCoord4i);
      SET_MultiTexCoord4iv(table, _mesa_marshal_MultiTexCoord4iv);
      SET_MultiTexCoord4s(table, _mesa_marshal_MultiTexCoord4s);
      SET_MultiTexCoord4sv(table, _mesa_marshal_MultiTexCoord4sv);
      SET_Ortho(table, _mesa_marshal_Ortho);
      SET_PassThrough(table, _mesa_marshal_PassThrough);
      SET_PixelMapfv(table, _mesa_marshal_PixelMapfv);
      SET_PixelMapuiv(table, _mesa_marshal_PixelMapuiv);
      SET_PixelMapusv(table, _mesa_marshal_PixelMapusv);
      SET_PixelTransferf(table, _mesa_marshal_PixelTransferf);
      SET_PixelTransferi(table, _mesa_marshal_PixelTransferi);
      SET_PixelZoom(table, _mesa_marshal_PixelZoom);
      SET_PopAttrib(table, _mesa_marshal_PopAttrib);
      SET_PopClientAttrib(table, _mesa_marshal_PopClientAttrib);
      SET_PopName(table, _mesa_marshal_PopName);
      SET_PrioritizeTextures(table, _mesa_marshal_PrioritizeTextures);
      SET_PushAttrib(table, _mesa_marshal_PushAttrib);
      SET_PushClientAttrib(table, _mesa_marshal_PushClientAttrib);
      SET_PushName(table, _mesa_marshal_PushName);
      SET_Rotated(table, _mesa_marshal_Rotated);
      SET_Scaled(table, _mesa_marshal_Scaled);
      SET_Translated(table, _mesa_marshal_Translated);
   }
   if (ctx->API == API_OPENGL_COMPAT || ctx->API == API_OPENGLES) {
      SET_AlphaFunc(table, _mesa_marshal_AlphaFunc);
      SET_ClientActiveTexture(table, _mesa_marshal_ClientActiveTexture);
      SET_ColorPointer(table, _mesa_marshal_ColorPointer);
      SET_DisableClientState(table, _mesa_marshal_DisableClientState);
      SET_EnableClientState(table, _mesa_marshal_EnableClientState);
      SET_GetLightfv(table, _mesa_marshal_GetLightfv);
      SET_GetMaterialfv(table, _mesa_marshal_GetMaterialfv);
      SET_GetTexEnvfv(table, _mesa_marshal_GetTexEnvfv);
      SET_GetTexEnviv(table, _mesa_marshal_GetTexEnviv);
      SET_GetTexGenfv(table, _mesa_marshal_GetTexGenfv);
      SET_GetTexGeniv(table, _mesa_marshal_GetTexGeniv);
      SET_LoadIdentity(table, _mesa_marshal_LoadIdentity);
      SET_LoadMatrixf(table, _mesa_marshal_LoadMatrixf);
      SET_MatrixMode(table, _mesa_marshal_MatrixMode);
      SET_MultMatrixf(table, _mesa_marshal_MultMatrixf);
      SET_MultiTexCoord4fARB(table, _mesa_marshal_MultiTexCoord4fARB);
      SET_NormalPointer(table, _mesa_marshal_NormalPointer);
      SET_PopMatrix(table, _mesa_marshal_PopMatrix);
      SET_PushMatrix(table, _mesa_marshal_PushMatrix);
      SET_Rotatef(table, _mesa_marshal_Rotatef);
      SET_Scalef(table, _mesa_marshal_Scalef);
      SET_TexCoordPointer(table, _mesa_marshal_TexCoordPointer);
      SET_Translatef(table, _mesa_marshal_Translatef);
      SET_VertexPointer(table, _mesa_marshal_VertexPointer);
   }
}
