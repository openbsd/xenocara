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

/* VertexAttrib1fARB: marshalled asynchronously */
struct marshal_cmd_VertexAttrib1fARB
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLfloat x;
};
void
_mesa_unmarshal_VertexAttrib1fARB(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib1fARB *cmd)
{
   GLuint index = cmd->index;
   GLfloat x = cmd->x;
   CALL_VertexAttrib1fARB(ctx->CurrentServerDispatch, (index, x));
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
void
_mesa_unmarshal_VertexAttrib1fvARB(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib1fvARB *cmd)
{
   GLuint index = cmd->index;
   const GLfloat * v = cmd->v;
   CALL_VertexAttrib1fvARB(ctx->CurrentServerDispatch, (index, v));
}
void GLAPIENTRY
_mesa_marshal_VertexAttrib1fvARB(GLuint index, const GLfloat * v)
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
void
_mesa_unmarshal_VertexAttrib1s(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib1s *cmd)
{
   GLuint index = cmd->index;
   GLshort x = cmd->x;
   CALL_VertexAttrib1s(ctx->CurrentServerDispatch, (index, x));
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
void
_mesa_unmarshal_VertexAttrib1sv(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib1sv *cmd)
{
   GLuint index = cmd->index;
   const GLshort * v = cmd->v;
   CALL_VertexAttrib1sv(ctx->CurrentServerDispatch, (index, v));
}
void GLAPIENTRY
_mesa_marshal_VertexAttrib1sv(GLuint index, const GLshort * v)
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
void
_mesa_unmarshal_VertexAttrib2d(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib2d *cmd)
{
   GLuint index = cmd->index;
   GLdouble x = cmd->x;
   GLdouble y = cmd->y;
   CALL_VertexAttrib2d(ctx->CurrentServerDispatch, (index, x, y));
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
void
_mesa_unmarshal_VertexAttrib2dv(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib2dv *cmd)
{
   GLuint index = cmd->index;
   const GLdouble * v = cmd->v;
   CALL_VertexAttrib2dv(ctx->CurrentServerDispatch, (index, v));
}
void GLAPIENTRY
_mesa_marshal_VertexAttrib2dv(GLuint index, const GLdouble * v)
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
void
_mesa_unmarshal_VertexAttrib2fARB(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib2fARB *cmd)
{
   GLuint index = cmd->index;
   GLfloat x = cmd->x;
   GLfloat y = cmd->y;
   CALL_VertexAttrib2fARB(ctx->CurrentServerDispatch, (index, x, y));
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
void
_mesa_unmarshal_VertexAttrib2fvARB(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib2fvARB *cmd)
{
   GLuint index = cmd->index;
   const GLfloat * v = cmd->v;
   CALL_VertexAttrib2fvARB(ctx->CurrentServerDispatch, (index, v));
}
void GLAPIENTRY
_mesa_marshal_VertexAttrib2fvARB(GLuint index, const GLfloat * v)
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
void
_mesa_unmarshal_VertexAttrib2s(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib2s *cmd)
{
   GLuint index = cmd->index;
   GLshort x = cmd->x;
   GLshort y = cmd->y;
   CALL_VertexAttrib2s(ctx->CurrentServerDispatch, (index, x, y));
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
void
_mesa_unmarshal_VertexAttrib2sv(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib2sv *cmd)
{
   GLuint index = cmd->index;
   const GLshort * v = cmd->v;
   CALL_VertexAttrib2sv(ctx->CurrentServerDispatch, (index, v));
}
void GLAPIENTRY
_mesa_marshal_VertexAttrib2sv(GLuint index, const GLshort * v)
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
void
_mesa_unmarshal_VertexAttrib3d(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib3d *cmd)
{
   GLuint index = cmd->index;
   GLdouble x = cmd->x;
   GLdouble y = cmd->y;
   GLdouble z = cmd->z;
   CALL_VertexAttrib3d(ctx->CurrentServerDispatch, (index, x, y, z));
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
void
_mesa_unmarshal_VertexAttrib3dv(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib3dv *cmd)
{
   GLuint index = cmd->index;
   const GLdouble * v = cmd->v;
   CALL_VertexAttrib3dv(ctx->CurrentServerDispatch, (index, v));
}
void GLAPIENTRY
_mesa_marshal_VertexAttrib3dv(GLuint index, const GLdouble * v)
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
void
_mesa_unmarshal_VertexAttrib3fARB(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib3fARB *cmd)
{
   GLuint index = cmd->index;
   GLfloat x = cmd->x;
   GLfloat y = cmd->y;
   GLfloat z = cmd->z;
   CALL_VertexAttrib3fARB(ctx->CurrentServerDispatch, (index, x, y, z));
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
void
_mesa_unmarshal_VertexAttrib3fvARB(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib3fvARB *cmd)
{
   GLuint index = cmd->index;
   const GLfloat * v = cmd->v;
   CALL_VertexAttrib3fvARB(ctx->CurrentServerDispatch, (index, v));
}
void GLAPIENTRY
_mesa_marshal_VertexAttrib3fvARB(GLuint index, const GLfloat * v)
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
void
_mesa_unmarshal_VertexAttrib3s(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib3s *cmd)
{
   GLuint index = cmd->index;
   GLshort x = cmd->x;
   GLshort y = cmd->y;
   GLshort z = cmd->z;
   CALL_VertexAttrib3s(ctx->CurrentServerDispatch, (index, x, y, z));
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
void
_mesa_unmarshal_VertexAttrib3sv(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib3sv *cmd)
{
   GLuint index = cmd->index;
   const GLshort * v = cmd->v;
   CALL_VertexAttrib3sv(ctx->CurrentServerDispatch, (index, v));
}
void GLAPIENTRY
_mesa_marshal_VertexAttrib3sv(GLuint index, const GLshort * v)
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
void
_mesa_unmarshal_VertexAttrib4Nbv(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4Nbv *cmd)
{
   GLuint index = cmd->index;
   const GLbyte * v = cmd->v;
   CALL_VertexAttrib4Nbv(ctx->CurrentServerDispatch, (index, v));
}
void GLAPIENTRY
_mesa_marshal_VertexAttrib4Nbv(GLuint index, const GLbyte * v)
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
void
_mesa_unmarshal_VertexAttrib4Niv(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4Niv *cmd)
{
   GLuint index = cmd->index;
   const GLint * v = cmd->v;
   CALL_VertexAttrib4Niv(ctx->CurrentServerDispatch, (index, v));
}
void GLAPIENTRY
_mesa_marshal_VertexAttrib4Niv(GLuint index, const GLint * v)
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
void
_mesa_unmarshal_VertexAttrib4Nsv(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4Nsv *cmd)
{
   GLuint index = cmd->index;
   const GLshort * v = cmd->v;
   CALL_VertexAttrib4Nsv(ctx->CurrentServerDispatch, (index, v));
}
void GLAPIENTRY
_mesa_marshal_VertexAttrib4Nsv(GLuint index, const GLshort * v)
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
void
_mesa_unmarshal_VertexAttrib4Nub(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4Nub *cmd)
{
   GLuint index = cmd->index;
   GLubyte x = cmd->x;
   GLubyte y = cmd->y;
   GLubyte z = cmd->z;
   GLubyte w = cmd->w;
   CALL_VertexAttrib4Nub(ctx->CurrentServerDispatch, (index, x, y, z, w));
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
void
_mesa_unmarshal_VertexAttrib4Nubv(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4Nubv *cmd)
{
   GLuint index = cmd->index;
   const GLubyte * v = cmd->v;
   CALL_VertexAttrib4Nubv(ctx->CurrentServerDispatch, (index, v));
}
void GLAPIENTRY
_mesa_marshal_VertexAttrib4Nubv(GLuint index, const GLubyte * v)
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
void
_mesa_unmarshal_VertexAttrib4Nuiv(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4Nuiv *cmd)
{
   GLuint index = cmd->index;
   const GLuint * v = cmd->v;
   CALL_VertexAttrib4Nuiv(ctx->CurrentServerDispatch, (index, v));
}
void GLAPIENTRY
_mesa_marshal_VertexAttrib4Nuiv(GLuint index, const GLuint * v)
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
void
_mesa_unmarshal_VertexAttrib4Nusv(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4Nusv *cmd)
{
   GLuint index = cmd->index;
   const GLushort * v = cmd->v;
   CALL_VertexAttrib4Nusv(ctx->CurrentServerDispatch, (index, v));
}
void GLAPIENTRY
_mesa_marshal_VertexAttrib4Nusv(GLuint index, const GLushort * v)
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
void
_mesa_unmarshal_VertexAttrib4bv(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4bv *cmd)
{
   GLuint index = cmd->index;
   const GLbyte * v = cmd->v;
   CALL_VertexAttrib4bv(ctx->CurrentServerDispatch, (index, v));
}
void GLAPIENTRY
_mesa_marshal_VertexAttrib4bv(GLuint index, const GLbyte * v)
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
void
_mesa_unmarshal_VertexAttrib4d(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4d *cmd)
{
   GLuint index = cmd->index;
   GLdouble x = cmd->x;
   GLdouble y = cmd->y;
   GLdouble z = cmd->z;
   GLdouble w = cmd->w;
   CALL_VertexAttrib4d(ctx->CurrentServerDispatch, (index, x, y, z, w));
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
void
_mesa_unmarshal_VertexAttrib4dv(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4dv *cmd)
{
   GLuint index = cmd->index;
   const GLdouble * v = cmd->v;
   CALL_VertexAttrib4dv(ctx->CurrentServerDispatch, (index, v));
}
void GLAPIENTRY
_mesa_marshal_VertexAttrib4dv(GLuint index, const GLdouble * v)
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
void
_mesa_unmarshal_VertexAttrib4fARB(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4fARB *cmd)
{
   GLuint index = cmd->index;
   GLfloat x = cmd->x;
   GLfloat y = cmd->y;
   GLfloat z = cmd->z;
   GLfloat w = cmd->w;
   CALL_VertexAttrib4fARB(ctx->CurrentServerDispatch, (index, x, y, z, w));
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
void
_mesa_unmarshal_VertexAttrib4fvARB(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4fvARB *cmd)
{
   GLuint index = cmd->index;
   const GLfloat * v = cmd->v;
   CALL_VertexAttrib4fvARB(ctx->CurrentServerDispatch, (index, v));
}
void GLAPIENTRY
_mesa_marshal_VertexAttrib4fvARB(GLuint index, const GLfloat * v)
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
void
_mesa_unmarshal_VertexAttrib4iv(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4iv *cmd)
{
   GLuint index = cmd->index;
   const GLint * v = cmd->v;
   CALL_VertexAttrib4iv(ctx->CurrentServerDispatch, (index, v));
}
void GLAPIENTRY
_mesa_marshal_VertexAttrib4iv(GLuint index, const GLint * v)
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
void
_mesa_unmarshal_VertexAttrib4s(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4s *cmd)
{
   GLuint index = cmd->index;
   GLshort x = cmd->x;
   GLshort y = cmd->y;
   GLshort z = cmd->z;
   GLshort w = cmd->w;
   CALL_VertexAttrib4s(ctx->CurrentServerDispatch, (index, x, y, z, w));
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
void
_mesa_unmarshal_VertexAttrib4sv(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4sv *cmd)
{
   GLuint index = cmd->index;
   const GLshort * v = cmd->v;
   CALL_VertexAttrib4sv(ctx->CurrentServerDispatch, (index, v));
}
void GLAPIENTRY
_mesa_marshal_VertexAttrib4sv(GLuint index, const GLshort * v)
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
void
_mesa_unmarshal_VertexAttrib4ubv(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4ubv *cmd)
{
   GLuint index = cmd->index;
   const GLubyte * v = cmd->v;
   CALL_VertexAttrib4ubv(ctx->CurrentServerDispatch, (index, v));
}
void GLAPIENTRY
_mesa_marshal_VertexAttrib4ubv(GLuint index, const GLubyte * v)
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
void
_mesa_unmarshal_VertexAttrib4uiv(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4uiv *cmd)
{
   GLuint index = cmd->index;
   const GLuint * v = cmd->v;
   CALL_VertexAttrib4uiv(ctx->CurrentServerDispatch, (index, v));
}
void GLAPIENTRY
_mesa_marshal_VertexAttrib4uiv(GLuint index, const GLuint * v)
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
void
_mesa_unmarshal_VertexAttrib4usv(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4usv *cmd)
{
   GLuint index = cmd->index;
   const GLushort * v = cmd->v;
   CALL_VertexAttrib4usv(ctx->CurrentServerDispatch, (index, v));
}
void GLAPIENTRY
_mesa_marshal_VertexAttrib4usv(GLuint index, const GLushort * v)
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
   GLuint index;
   GLint size;
   GLenum type;
   GLsizei stride;
   const GLvoid * pointer;
};
void
_mesa_unmarshal_VertexAttribPointer(struct gl_context *ctx, const struct marshal_cmd_VertexAttribPointer *cmd)
{
   GLuint index = cmd->index;
   GLint size = cmd->size;
   GLenum type = cmd->type;
   GLboolean normalized = cmd->normalized;
   GLsizei stride = cmd->stride;
   const GLvoid * pointer = cmd->pointer;
   CALL_VertexAttribPointer(ctx->CurrentServerDispatch, (index, size, type, normalized, stride, pointer));
}
void GLAPIENTRY
_mesa_marshal_VertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid * pointer)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttribPointer);
   struct marshal_cmd_VertexAttribPointer *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttribPointer, cmd_size);
   cmd->index = index;
   cmd->size = size;
   cmd->type = type;
   cmd->normalized = normalized;
   cmd->stride = stride;
   cmd->pointer = pointer;
   if (COMPAT) _mesa_glthread_AttribPointer(ctx, VERT_ATTRIB_GENERIC(index), size, type, stride, pointer);
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
void
_mesa_unmarshal_UniformMatrix2x3fv(struct gl_context *ctx, const struct marshal_cmd_UniformMatrix2x3fv *cmd)
{
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLboolean transpose = cmd->transpose;
   GLfloat * value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLfloat *) variable_data;
   CALL_UniformMatrix2x3fv(ctx->CurrentServerDispatch, (location, count, transpose, value));
}
void GLAPIENTRY
_mesa_marshal_UniformMatrix2x3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat * value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 6 * sizeof(GLfloat));
   int cmd_size = sizeof(struct marshal_cmd_UniformMatrix2x3fv) + value_size;
   struct marshal_cmd_UniformMatrix2x3fv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "UniformMatrix2x3fv");
      CALL_UniformMatrix2x3fv(ctx->CurrentServerDispatch, (location, count, transpose, value));
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
void
_mesa_unmarshal_UniformMatrix3x2fv(struct gl_context *ctx, const struct marshal_cmd_UniformMatrix3x2fv *cmd)
{
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLboolean transpose = cmd->transpose;
   GLfloat * value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLfloat *) variable_data;
   CALL_UniformMatrix3x2fv(ctx->CurrentServerDispatch, (location, count, transpose, value));
}
void GLAPIENTRY
_mesa_marshal_UniformMatrix3x2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat * value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 6 * sizeof(GLfloat));
   int cmd_size = sizeof(struct marshal_cmd_UniformMatrix3x2fv) + value_size;
   struct marshal_cmd_UniformMatrix3x2fv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "UniformMatrix3x2fv");
      CALL_UniformMatrix3x2fv(ctx->CurrentServerDispatch, (location, count, transpose, value));
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
void
_mesa_unmarshal_UniformMatrix2x4fv(struct gl_context *ctx, const struct marshal_cmd_UniformMatrix2x4fv *cmd)
{
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLboolean transpose = cmd->transpose;
   GLfloat * value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLfloat *) variable_data;
   CALL_UniformMatrix2x4fv(ctx->CurrentServerDispatch, (location, count, transpose, value));
}
void GLAPIENTRY
_mesa_marshal_UniformMatrix2x4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat * value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 8 * sizeof(GLfloat));
   int cmd_size = sizeof(struct marshal_cmd_UniformMatrix2x4fv) + value_size;
   struct marshal_cmd_UniformMatrix2x4fv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "UniformMatrix2x4fv");
      CALL_UniformMatrix2x4fv(ctx->CurrentServerDispatch, (location, count, transpose, value));
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
void
_mesa_unmarshal_UniformMatrix4x2fv(struct gl_context *ctx, const struct marshal_cmd_UniformMatrix4x2fv *cmd)
{
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLboolean transpose = cmd->transpose;
   GLfloat * value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLfloat *) variable_data;
   CALL_UniformMatrix4x2fv(ctx->CurrentServerDispatch, (location, count, transpose, value));
}
void GLAPIENTRY
_mesa_marshal_UniformMatrix4x2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat * value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 8 * sizeof(GLfloat));
   int cmd_size = sizeof(struct marshal_cmd_UniformMatrix4x2fv) + value_size;
   struct marshal_cmd_UniformMatrix4x2fv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "UniformMatrix4x2fv");
      CALL_UniformMatrix4x2fv(ctx->CurrentServerDispatch, (location, count, transpose, value));
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
void
_mesa_unmarshal_UniformMatrix3x4fv(struct gl_context *ctx, const struct marshal_cmd_UniformMatrix3x4fv *cmd)
{
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLboolean transpose = cmd->transpose;
   GLfloat * value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLfloat *) variable_data;
   CALL_UniformMatrix3x4fv(ctx->CurrentServerDispatch, (location, count, transpose, value));
}
void GLAPIENTRY
_mesa_marshal_UniformMatrix3x4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat * value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 12 * sizeof(GLfloat));
   int cmd_size = sizeof(struct marshal_cmd_UniformMatrix3x4fv) + value_size;
   struct marshal_cmd_UniformMatrix3x4fv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "UniformMatrix3x4fv");
      CALL_UniformMatrix3x4fv(ctx->CurrentServerDispatch, (location, count, transpose, value));
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
void
_mesa_unmarshal_UniformMatrix4x3fv(struct gl_context *ctx, const struct marshal_cmd_UniformMatrix4x3fv *cmd)
{
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLboolean transpose = cmd->transpose;
   GLfloat * value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLfloat *) variable_data;
   CALL_UniformMatrix4x3fv(ctx->CurrentServerDispatch, (location, count, transpose, value));
}
void GLAPIENTRY
_mesa_marshal_UniformMatrix4x3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat * value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 12 * sizeof(GLfloat));
   int cmd_size = sizeof(struct marshal_cmd_UniformMatrix4x3fv) + value_size;
   struct marshal_cmd_UniformMatrix4x3fv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "UniformMatrix4x3fv");
      CALL_UniformMatrix4x3fv(ctx->CurrentServerDispatch, (location, count, transpose, value));
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
   GLenum target;
   GLenum format;
   GLsizei len;
   /* Next len bytes are GLvoid string[len] */
};
void
_mesa_unmarshal_ProgramStringARB(struct gl_context *ctx, const struct marshal_cmd_ProgramStringARB *cmd)
{
   GLenum target = cmd->target;
   GLenum format = cmd->format;
   GLsizei len = cmd->len;
   GLvoid * string;
   const char *variable_data = (const char *) (cmd + 1);
   string = (GLvoid *) variable_data;
   CALL_ProgramStringARB(ctx->CurrentServerDispatch, (target, format, len, string));
}
void GLAPIENTRY
_mesa_marshal_ProgramStringARB(GLenum target, GLenum format, GLsizei len, const GLvoid * string)
{
   GET_CURRENT_CONTEXT(ctx);
   int string_size = len;
   int cmd_size = sizeof(struct marshal_cmd_ProgramStringARB) + string_size;
   struct marshal_cmd_ProgramStringARB *cmd;
   if (unlikely(string_size < 0 || (string_size > 0 && !string) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "ProgramStringARB");
      CALL_ProgramStringARB(ctx->CurrentServerDispatch, (target, format, len, string));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ProgramStringARB, cmd_size);
   cmd->target = target;
   cmd->format = format;
   cmd->len = len;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, string, string_size);
}


/* BindProgramARB: marshalled asynchronously */
struct marshal_cmd_BindProgramARB
{
   struct marshal_cmd_base cmd_base;
   GLenum target;
   GLuint program;
};
void
_mesa_unmarshal_BindProgramARB(struct gl_context *ctx, const struct marshal_cmd_BindProgramARB *cmd)
{
   GLenum target = cmd->target;
   GLuint program = cmd->program;
   CALL_BindProgramARB(ctx->CurrentServerDispatch, (target, program));
}
void GLAPIENTRY
_mesa_marshal_BindProgramARB(GLenum target, GLuint program)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_BindProgramARB);
   struct marshal_cmd_BindProgramARB *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_BindProgramARB, cmd_size);
   cmd->target = target;
   cmd->program = program;
}


/* DeleteProgramsARB: marshalled asynchronously */
struct marshal_cmd_DeleteProgramsARB
{
   struct marshal_cmd_base cmd_base;
   GLsizei n;
   /* Next safe_mul(n, 1 * sizeof(GLuint)) bytes are GLuint programs[n] */
};
void
_mesa_unmarshal_DeleteProgramsARB(struct gl_context *ctx, const struct marshal_cmd_DeleteProgramsARB *cmd)
{
   GLsizei n = cmd->n;
   GLuint * programs;
   const char *variable_data = (const char *) (cmd + 1);
   programs = (GLuint *) variable_data;
   CALL_DeleteProgramsARB(ctx->CurrentServerDispatch, (n, programs));
}
void GLAPIENTRY
_mesa_marshal_DeleteProgramsARB(GLsizei n, const GLuint * programs)
{
   GET_CURRENT_CONTEXT(ctx);
   int programs_size = safe_mul(n, 1 * sizeof(GLuint));
   int cmd_size = sizeof(struct marshal_cmd_DeleteProgramsARB) + programs_size;
   struct marshal_cmd_DeleteProgramsARB *cmd;
   if (unlikely(programs_size < 0 || (programs_size > 0 && !programs) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "DeleteProgramsARB");
      CALL_DeleteProgramsARB(ctx->CurrentServerDispatch, (n, programs));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_DeleteProgramsARB, cmd_size);
   cmd->n = n;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, programs, programs_size);
}


/* GenProgramsARB: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GenProgramsARB(GLsizei n, GLuint * programs)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GenProgramsARB");
   CALL_GenProgramsARB(ctx->CurrentServerDispatch, (n, programs));
}


/* IsProgramARB: marshalled synchronously */
GLboolean GLAPIENTRY
_mesa_marshal_IsProgramARB(GLuint program)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "IsProgramARB");
   return CALL_IsProgramARB(ctx->CurrentServerDispatch, (program));
}


/* ProgramEnvParameter4dARB: marshalled asynchronously */
struct marshal_cmd_ProgramEnvParameter4dARB
{
   struct marshal_cmd_base cmd_base;
   GLenum target;
   GLuint index;
   GLdouble x;
   GLdouble y;
   GLdouble z;
   GLdouble w;
};
void
_mesa_unmarshal_ProgramEnvParameter4dARB(struct gl_context *ctx, const struct marshal_cmd_ProgramEnvParameter4dARB *cmd)
{
   GLenum target = cmd->target;
   GLuint index = cmd->index;
   GLdouble x = cmd->x;
   GLdouble y = cmd->y;
   GLdouble z = cmd->z;
   GLdouble w = cmd->w;
   CALL_ProgramEnvParameter4dARB(ctx->CurrentServerDispatch, (target, index, x, y, z, w));
}
void GLAPIENTRY
_mesa_marshal_ProgramEnvParameter4dARB(GLenum target, GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ProgramEnvParameter4dARB);
   struct marshal_cmd_ProgramEnvParameter4dARB *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ProgramEnvParameter4dARB, cmd_size);
   cmd->target = target;
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
   GLenum target;
   GLuint index;
   GLdouble params[4];
};
void
_mesa_unmarshal_ProgramEnvParameter4dvARB(struct gl_context *ctx, const struct marshal_cmd_ProgramEnvParameter4dvARB *cmd)
{
   GLenum target = cmd->target;
   GLuint index = cmd->index;
   const GLdouble * params = cmd->params;
   CALL_ProgramEnvParameter4dvARB(ctx->CurrentServerDispatch, (target, index, params));
}
void GLAPIENTRY
_mesa_marshal_ProgramEnvParameter4dvARB(GLenum target, GLuint index, const GLdouble * params)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ProgramEnvParameter4dvARB);
   struct marshal_cmd_ProgramEnvParameter4dvARB *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ProgramEnvParameter4dvARB, cmd_size);
   cmd->target = target;
   cmd->index = index;
   memcpy(cmd->params, params, 4 * sizeof(GLdouble));
}


/* ProgramEnvParameter4fARB: marshalled asynchronously */
struct marshal_cmd_ProgramEnvParameter4fARB
{
   struct marshal_cmd_base cmd_base;
   GLenum target;
   GLuint index;
   GLfloat x;
   GLfloat y;
   GLfloat z;
   GLfloat w;
};
void
_mesa_unmarshal_ProgramEnvParameter4fARB(struct gl_context *ctx, const struct marshal_cmd_ProgramEnvParameter4fARB *cmd)
{
   GLenum target = cmd->target;
   GLuint index = cmd->index;
   GLfloat x = cmd->x;
   GLfloat y = cmd->y;
   GLfloat z = cmd->z;
   GLfloat w = cmd->w;
   CALL_ProgramEnvParameter4fARB(ctx->CurrentServerDispatch, (target, index, x, y, z, w));
}
void GLAPIENTRY
_mesa_marshal_ProgramEnvParameter4fARB(GLenum target, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ProgramEnvParameter4fARB);
   struct marshal_cmd_ProgramEnvParameter4fARB *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ProgramEnvParameter4fARB, cmd_size);
   cmd->target = target;
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
   GLenum target;
   GLuint index;
   GLfloat params[4];
};
void
_mesa_unmarshal_ProgramEnvParameter4fvARB(struct gl_context *ctx, const struct marshal_cmd_ProgramEnvParameter4fvARB *cmd)
{
   GLenum target = cmd->target;
   GLuint index = cmd->index;
   const GLfloat * params = cmd->params;
   CALL_ProgramEnvParameter4fvARB(ctx->CurrentServerDispatch, (target, index, params));
}
void GLAPIENTRY
_mesa_marshal_ProgramEnvParameter4fvARB(GLenum target, GLuint index, const GLfloat * params)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ProgramEnvParameter4fvARB);
   struct marshal_cmd_ProgramEnvParameter4fvARB *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ProgramEnvParameter4fvARB, cmd_size);
   cmd->target = target;
   cmd->index = index;
   memcpy(cmd->params, params, 4 * sizeof(GLfloat));
}


/* ProgramLocalParameter4dARB: marshalled asynchronously */
struct marshal_cmd_ProgramLocalParameter4dARB
{
   struct marshal_cmd_base cmd_base;
   GLenum target;
   GLuint index;
   GLdouble x;
   GLdouble y;
   GLdouble z;
   GLdouble w;
};
void
_mesa_unmarshal_ProgramLocalParameter4dARB(struct gl_context *ctx, const struct marshal_cmd_ProgramLocalParameter4dARB *cmd)
{
   GLenum target = cmd->target;
   GLuint index = cmd->index;
   GLdouble x = cmd->x;
   GLdouble y = cmd->y;
   GLdouble z = cmd->z;
   GLdouble w = cmd->w;
   CALL_ProgramLocalParameter4dARB(ctx->CurrentServerDispatch, (target, index, x, y, z, w));
}
void GLAPIENTRY
_mesa_marshal_ProgramLocalParameter4dARB(GLenum target, GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ProgramLocalParameter4dARB);
   struct marshal_cmd_ProgramLocalParameter4dARB *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ProgramLocalParameter4dARB, cmd_size);
   cmd->target = target;
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
   GLenum target;
   GLuint index;
   GLdouble params[4];
};
void
_mesa_unmarshal_ProgramLocalParameter4dvARB(struct gl_context *ctx, const struct marshal_cmd_ProgramLocalParameter4dvARB *cmd)
{
   GLenum target = cmd->target;
   GLuint index = cmd->index;
   const GLdouble * params = cmd->params;
   CALL_ProgramLocalParameter4dvARB(ctx->CurrentServerDispatch, (target, index, params));
}
void GLAPIENTRY
_mesa_marshal_ProgramLocalParameter4dvARB(GLenum target, GLuint index, const GLdouble * params)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ProgramLocalParameter4dvARB);
   struct marshal_cmd_ProgramLocalParameter4dvARB *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ProgramLocalParameter4dvARB, cmd_size);
   cmd->target = target;
   cmd->index = index;
   memcpy(cmd->params, params, 4 * sizeof(GLdouble));
}


/* ProgramLocalParameter4fARB: marshalled asynchronously */
struct marshal_cmd_ProgramLocalParameter4fARB
{
   struct marshal_cmd_base cmd_base;
   GLenum target;
   GLuint index;
   GLfloat x;
   GLfloat y;
   GLfloat z;
   GLfloat w;
};
void
_mesa_unmarshal_ProgramLocalParameter4fARB(struct gl_context *ctx, const struct marshal_cmd_ProgramLocalParameter4fARB *cmd)
{
   GLenum target = cmd->target;
   GLuint index = cmd->index;
   GLfloat x = cmd->x;
   GLfloat y = cmd->y;
   GLfloat z = cmd->z;
   GLfloat w = cmd->w;
   CALL_ProgramLocalParameter4fARB(ctx->CurrentServerDispatch, (target, index, x, y, z, w));
}
void GLAPIENTRY
_mesa_marshal_ProgramLocalParameter4fARB(GLenum target, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ProgramLocalParameter4fARB);
   struct marshal_cmd_ProgramLocalParameter4fARB *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ProgramLocalParameter4fARB, cmd_size);
   cmd->target = target;
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
   GLenum target;
   GLuint index;
   GLfloat params[4];
};
void
_mesa_unmarshal_ProgramLocalParameter4fvARB(struct gl_context *ctx, const struct marshal_cmd_ProgramLocalParameter4fvARB *cmd)
{
   GLenum target = cmd->target;
   GLuint index = cmd->index;
   const GLfloat * params = cmd->params;
   CALL_ProgramLocalParameter4fvARB(ctx->CurrentServerDispatch, (target, index, params));
}
void GLAPIENTRY
_mesa_marshal_ProgramLocalParameter4fvARB(GLenum target, GLuint index, const GLfloat * params)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ProgramLocalParameter4fvARB);
   struct marshal_cmd_ProgramLocalParameter4fvARB *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ProgramLocalParameter4fvARB, cmd_size);
   cmd->target = target;
   cmd->index = index;
   memcpy(cmd->params, params, 4 * sizeof(GLfloat));
}


/* GetProgramEnvParameterdvARB: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetProgramEnvParameterdvARB(GLenum target, GLuint index, GLdouble * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetProgramEnvParameterdvARB");
   CALL_GetProgramEnvParameterdvARB(ctx->CurrentServerDispatch, (target, index, params));
}


/* GetProgramEnvParameterfvARB: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetProgramEnvParameterfvARB(GLenum target, GLuint index, GLfloat * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetProgramEnvParameterfvARB");
   CALL_GetProgramEnvParameterfvARB(ctx->CurrentServerDispatch, (target, index, params));
}


/* GetProgramLocalParameterdvARB: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetProgramLocalParameterdvARB(GLenum target, GLuint index, GLdouble * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetProgramLocalParameterdvARB");
   CALL_GetProgramLocalParameterdvARB(ctx->CurrentServerDispatch, (target, index, params));
}


/* GetProgramLocalParameterfvARB: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetProgramLocalParameterfvARB(GLenum target, GLuint index, GLfloat * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetProgramLocalParameterfvARB");
   CALL_GetProgramLocalParameterfvARB(ctx->CurrentServerDispatch, (target, index, params));
}


/* GetProgramivARB: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetProgramivARB(GLenum target, GLenum pname, GLint * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetProgramivARB");
   CALL_GetProgramivARB(ctx->CurrentServerDispatch, (target, pname, params));
}


/* GetProgramStringARB: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetProgramStringARB(GLenum target, GLenum pname, GLvoid * string)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetProgramStringARB");
   CALL_GetProgramStringARB(ctx->CurrentServerDispatch, (target, pname, string));
}


/* DeleteObjectARB: marshalled asynchronously */
struct marshal_cmd_DeleteObjectARB
{
   struct marshal_cmd_base cmd_base;
   GLhandleARB obj;
};
void
_mesa_unmarshal_DeleteObjectARB(struct gl_context *ctx, const struct marshal_cmd_DeleteObjectARB *cmd)
{
   GLhandleARB obj = cmd->obj;
   CALL_DeleteObjectARB(ctx->CurrentServerDispatch, (obj));
}
void GLAPIENTRY
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
GLhandleARB GLAPIENTRY
_mesa_marshal_GetHandleARB(GLenum pname)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetHandleARB");
   return CALL_GetHandleARB(ctx->CurrentServerDispatch, (pname));
}


/* DetachObjectARB: marshalled asynchronously */
struct marshal_cmd_DetachObjectARB
{
   struct marshal_cmd_base cmd_base;
   GLhandleARB containerObj;
   GLhandleARB attachedObj;
};
void
_mesa_unmarshal_DetachObjectARB(struct gl_context *ctx, const struct marshal_cmd_DetachObjectARB *cmd)
{
   GLhandleARB containerObj = cmd->containerObj;
   GLhandleARB attachedObj = cmd->attachedObj;
   CALL_DetachObjectARB(ctx->CurrentServerDispatch, (containerObj, attachedObj));
}
void GLAPIENTRY
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
GLhandleARB GLAPIENTRY
_mesa_marshal_CreateShaderObjectARB(GLenum shaderType)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "CreateShaderObjectARB");
   return CALL_CreateShaderObjectARB(ctx->CurrentServerDispatch, (shaderType));
}


/* CreateProgramObjectARB: marshalled synchronously */
GLhandleARB GLAPIENTRY
_mesa_marshal_CreateProgramObjectARB(void)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "CreateProgramObjectARB");
   return CALL_CreateProgramObjectARB(ctx->CurrentServerDispatch, ());
}


/* AttachObjectARB: marshalled asynchronously */
struct marshal_cmd_AttachObjectARB
{
   struct marshal_cmd_base cmd_base;
   GLhandleARB containerObj;
   GLhandleARB obj;
};
void
_mesa_unmarshal_AttachObjectARB(struct gl_context *ctx, const struct marshal_cmd_AttachObjectARB *cmd)
{
   GLhandleARB containerObj = cmd->containerObj;
   GLhandleARB obj = cmd->obj;
   CALL_AttachObjectARB(ctx->CurrentServerDispatch, (containerObj, obj));
}
void GLAPIENTRY
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
void GLAPIENTRY
_mesa_marshal_GetObjectParameterfvARB(GLhandleARB obj, GLenum pname, GLfloat * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetObjectParameterfvARB");
   CALL_GetObjectParameterfvARB(ctx->CurrentServerDispatch, (obj, pname, params));
}


/* GetObjectParameterivARB: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetObjectParameterivARB(GLhandleARB obj, GLenum pname, GLint * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetObjectParameterivARB");
   CALL_GetObjectParameterivARB(ctx->CurrentServerDispatch, (obj, pname, params));
}


/* GetInfoLogARB: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetInfoLogARB(GLhandleARB obj, GLsizei maxLength, GLsizei * length, GLcharARB * infoLog)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetInfoLogARB");
   CALL_GetInfoLogARB(ctx->CurrentServerDispatch, (obj, maxLength, length, infoLog));
}


/* GetAttachedObjectsARB: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetAttachedObjectsARB(GLhandleARB containerObj, GLsizei maxLength, GLsizei * length, GLhandleARB * infoLog)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetAttachedObjectsARB");
   CALL_GetAttachedObjectsARB(ctx->CurrentServerDispatch, (containerObj, maxLength, length, infoLog));
}


/* ClampColor: marshalled asynchronously */
struct marshal_cmd_ClampColor
{
   struct marshal_cmd_base cmd_base;
   GLenum target;
   GLenum clamp;
};
void
_mesa_unmarshal_ClampColor(struct gl_context *ctx, const struct marshal_cmd_ClampColor *cmd)
{
   GLenum target = cmd->target;
   GLenum clamp = cmd->clamp;
   CALL_ClampColor(ctx->CurrentServerDispatch, (target, clamp));
}
void GLAPIENTRY
_mesa_marshal_ClampColor(GLenum target, GLenum clamp)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ClampColor);
   struct marshal_cmd_ClampColor *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ClampColor, cmd_size);
   cmd->target = target;
   cmd->clamp = clamp;
}


/* IsRenderbuffer: marshalled synchronously */
GLboolean GLAPIENTRY
_mesa_marshal_IsRenderbuffer(GLuint renderbuffer)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "IsRenderbuffer");
   return CALL_IsRenderbuffer(ctx->CurrentServerDispatch, (renderbuffer));
}


/* BindRenderbuffer: marshalled asynchronously */
struct marshal_cmd_BindRenderbuffer
{
   struct marshal_cmd_base cmd_base;
   GLenum target;
   GLuint renderbuffer;
};
void
_mesa_unmarshal_BindRenderbuffer(struct gl_context *ctx, const struct marshal_cmd_BindRenderbuffer *cmd)
{
   GLenum target = cmd->target;
   GLuint renderbuffer = cmd->renderbuffer;
   CALL_BindRenderbuffer(ctx->CurrentServerDispatch, (target, renderbuffer));
}
void GLAPIENTRY
_mesa_marshal_BindRenderbuffer(GLenum target, GLuint renderbuffer)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_BindRenderbuffer);
   struct marshal_cmd_BindRenderbuffer *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_BindRenderbuffer, cmd_size);
   cmd->target = target;
   cmd->renderbuffer = renderbuffer;
}


/* DeleteRenderbuffers: marshalled asynchronously */
struct marshal_cmd_DeleteRenderbuffers
{
   struct marshal_cmd_base cmd_base;
   GLsizei n;
   /* Next safe_mul(n, 1 * sizeof(GLuint)) bytes are GLuint renderbuffers[n] */
};
void
_mesa_unmarshal_DeleteRenderbuffers(struct gl_context *ctx, const struct marshal_cmd_DeleteRenderbuffers *cmd)
{
   GLsizei n = cmd->n;
   GLuint * renderbuffers;
   const char *variable_data = (const char *) (cmd + 1);
   renderbuffers = (GLuint *) variable_data;
   CALL_DeleteRenderbuffers(ctx->CurrentServerDispatch, (n, renderbuffers));
}
void GLAPIENTRY
_mesa_marshal_DeleteRenderbuffers(GLsizei n, const GLuint * renderbuffers)
{
   GET_CURRENT_CONTEXT(ctx);
   int renderbuffers_size = safe_mul(n, 1 * sizeof(GLuint));
   int cmd_size = sizeof(struct marshal_cmd_DeleteRenderbuffers) + renderbuffers_size;
   struct marshal_cmd_DeleteRenderbuffers *cmd;
   if (unlikely(renderbuffers_size < 0 || (renderbuffers_size > 0 && !renderbuffers) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "DeleteRenderbuffers");
      CALL_DeleteRenderbuffers(ctx->CurrentServerDispatch, (n, renderbuffers));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_DeleteRenderbuffers, cmd_size);
   cmd->n = n;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, renderbuffers, renderbuffers_size);
}


/* GenRenderbuffers: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GenRenderbuffers(GLsizei n, GLuint * renderbuffers)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GenRenderbuffers");
   CALL_GenRenderbuffers(ctx->CurrentServerDispatch, (n, renderbuffers));
}


/* RenderbufferStorage: marshalled asynchronously */
struct marshal_cmd_RenderbufferStorage
{
   struct marshal_cmd_base cmd_base;
   GLenum target;
   GLenum internalformat;
   GLsizei width;
   GLsizei height;
};
void
_mesa_unmarshal_RenderbufferStorage(struct gl_context *ctx, const struct marshal_cmd_RenderbufferStorage *cmd)
{
   GLenum target = cmd->target;
   GLenum internalformat = cmd->internalformat;
   GLsizei width = cmd->width;
   GLsizei height = cmd->height;
   CALL_RenderbufferStorage(ctx->CurrentServerDispatch, (target, internalformat, width, height));
}
void GLAPIENTRY
_mesa_marshal_RenderbufferStorage(GLenum target, GLenum internalformat, GLsizei width, GLsizei height)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_RenderbufferStorage);
   struct marshal_cmd_RenderbufferStorage *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_RenderbufferStorage, cmd_size);
   cmd->target = target;
   cmd->internalformat = internalformat;
   cmd->width = width;
   cmd->height = height;
}


/* RenderbufferStorageMultisample: marshalled asynchronously */
struct marshal_cmd_RenderbufferStorageMultisample
{
   struct marshal_cmd_base cmd_base;
   GLenum target;
   GLsizei samples;
   GLenum internalformat;
   GLsizei width;
   GLsizei height;
};
void
_mesa_unmarshal_RenderbufferStorageMultisample(struct gl_context *ctx, const struct marshal_cmd_RenderbufferStorageMultisample *cmd)
{
   GLenum target = cmd->target;
   GLsizei samples = cmd->samples;
   GLenum internalformat = cmd->internalformat;
   GLsizei width = cmd->width;
   GLsizei height = cmd->height;
   CALL_RenderbufferStorageMultisample(ctx->CurrentServerDispatch, (target, samples, internalformat, width, height));
}
void GLAPIENTRY
_mesa_marshal_RenderbufferStorageMultisample(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_RenderbufferStorageMultisample);
   struct marshal_cmd_RenderbufferStorageMultisample *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_RenderbufferStorageMultisample, cmd_size);
   cmd->target = target;
   cmd->samples = samples;
   cmd->internalformat = internalformat;
   cmd->width = width;
   cmd->height = height;
}


/* GetRenderbufferParameteriv: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetRenderbufferParameteriv(GLenum target, GLenum pname, GLint * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetRenderbufferParameteriv");
   CALL_GetRenderbufferParameteriv(ctx->CurrentServerDispatch, (target, pname, params));
}


/* IsFramebuffer: marshalled synchronously */
GLboolean GLAPIENTRY
_mesa_marshal_IsFramebuffer(GLuint framebuffer)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "IsFramebuffer");
   return CALL_IsFramebuffer(ctx->CurrentServerDispatch, (framebuffer));
}


/* BindFramebuffer: marshalled asynchronously */
struct marshal_cmd_BindFramebuffer
{
   struct marshal_cmd_base cmd_base;
   GLenum target;
   GLuint framebuffer;
};
void
_mesa_unmarshal_BindFramebuffer(struct gl_context *ctx, const struct marshal_cmd_BindFramebuffer *cmd)
{
   GLenum target = cmd->target;
   GLuint framebuffer = cmd->framebuffer;
   CALL_BindFramebuffer(ctx->CurrentServerDispatch, (target, framebuffer));
}
void GLAPIENTRY
_mesa_marshal_BindFramebuffer(GLenum target, GLuint framebuffer)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_BindFramebuffer);
   struct marshal_cmd_BindFramebuffer *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_BindFramebuffer, cmd_size);
   cmd->target = target;
   cmd->framebuffer = framebuffer;
}


/* DeleteFramebuffers: marshalled asynchronously */
struct marshal_cmd_DeleteFramebuffers
{
   struct marshal_cmd_base cmd_base;
   GLsizei n;
   /* Next safe_mul(n, 1 * sizeof(GLuint)) bytes are GLuint framebuffers[n] */
};
void
_mesa_unmarshal_DeleteFramebuffers(struct gl_context *ctx, const struct marshal_cmd_DeleteFramebuffers *cmd)
{
   GLsizei n = cmd->n;
   GLuint * framebuffers;
   const char *variable_data = (const char *) (cmd + 1);
   framebuffers = (GLuint *) variable_data;
   CALL_DeleteFramebuffers(ctx->CurrentServerDispatch, (n, framebuffers));
}
void GLAPIENTRY
_mesa_marshal_DeleteFramebuffers(GLsizei n, const GLuint * framebuffers)
{
   GET_CURRENT_CONTEXT(ctx);
   int framebuffers_size = safe_mul(n, 1 * sizeof(GLuint));
   int cmd_size = sizeof(struct marshal_cmd_DeleteFramebuffers) + framebuffers_size;
   struct marshal_cmd_DeleteFramebuffers *cmd;
   if (unlikely(framebuffers_size < 0 || (framebuffers_size > 0 && !framebuffers) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "DeleteFramebuffers");
      CALL_DeleteFramebuffers(ctx->CurrentServerDispatch, (n, framebuffers));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_DeleteFramebuffers, cmd_size);
   cmd->n = n;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, framebuffers, framebuffers_size);
}


/* GenFramebuffers: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GenFramebuffers(GLsizei n, GLuint * framebuffers)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GenFramebuffers");
   CALL_GenFramebuffers(ctx->CurrentServerDispatch, (n, framebuffers));
}


/* CheckFramebufferStatus: marshalled synchronously */
GLenum GLAPIENTRY
_mesa_marshal_CheckFramebufferStatus(GLenum target)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "CheckFramebufferStatus");
   return CALL_CheckFramebufferStatus(ctx->CurrentServerDispatch, (target));
}


/* FramebufferTexture1D: marshalled asynchronously */
struct marshal_cmd_FramebufferTexture1D
{
   struct marshal_cmd_base cmd_base;
   GLenum target;
   GLenum attachment;
   GLenum textarget;
   GLuint texture;
   GLint level;
};
void
_mesa_unmarshal_FramebufferTexture1D(struct gl_context *ctx, const struct marshal_cmd_FramebufferTexture1D *cmd)
{
   GLenum target = cmd->target;
   GLenum attachment = cmd->attachment;
   GLenum textarget = cmd->textarget;
   GLuint texture = cmd->texture;
   GLint level = cmd->level;
   CALL_FramebufferTexture1D(ctx->CurrentServerDispatch, (target, attachment, textarget, texture, level));
}
void GLAPIENTRY
_mesa_marshal_FramebufferTexture1D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_FramebufferTexture1D);
   struct marshal_cmd_FramebufferTexture1D *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_FramebufferTexture1D, cmd_size);
   cmd->target = target;
   cmd->attachment = attachment;
   cmd->textarget = textarget;
   cmd->texture = texture;
   cmd->level = level;
}


/* FramebufferTexture2D: marshalled asynchronously */
struct marshal_cmd_FramebufferTexture2D
{
   struct marshal_cmd_base cmd_base;
   GLenum target;
   GLenum attachment;
   GLenum textarget;
   GLuint texture;
   GLint level;
};
void
_mesa_unmarshal_FramebufferTexture2D(struct gl_context *ctx, const struct marshal_cmd_FramebufferTexture2D *cmd)
{
   GLenum target = cmd->target;
   GLenum attachment = cmd->attachment;
   GLenum textarget = cmd->textarget;
   GLuint texture = cmd->texture;
   GLint level = cmd->level;
   CALL_FramebufferTexture2D(ctx->CurrentServerDispatch, (target, attachment, textarget, texture, level));
}
void GLAPIENTRY
_mesa_marshal_FramebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_FramebufferTexture2D);
   struct marshal_cmd_FramebufferTexture2D *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_FramebufferTexture2D, cmd_size);
   cmd->target = target;
   cmd->attachment = attachment;
   cmd->textarget = textarget;
   cmd->texture = texture;
   cmd->level = level;
}


/* FramebufferTexture3D: marshalled asynchronously */
struct marshal_cmd_FramebufferTexture3D
{
   struct marshal_cmd_base cmd_base;
   GLenum target;
   GLenum attachment;
   GLenum textarget;
   GLuint texture;
   GLint level;
   GLint layer;
};
void
_mesa_unmarshal_FramebufferTexture3D(struct gl_context *ctx, const struct marshal_cmd_FramebufferTexture3D *cmd)
{
   GLenum target = cmd->target;
   GLenum attachment = cmd->attachment;
   GLenum textarget = cmd->textarget;
   GLuint texture = cmd->texture;
   GLint level = cmd->level;
   GLint layer = cmd->layer;
   CALL_FramebufferTexture3D(ctx->CurrentServerDispatch, (target, attachment, textarget, texture, level, layer));
}
void GLAPIENTRY
_mesa_marshal_FramebufferTexture3D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint layer)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_FramebufferTexture3D);
   struct marshal_cmd_FramebufferTexture3D *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_FramebufferTexture3D, cmd_size);
   cmd->target = target;
   cmd->attachment = attachment;
   cmd->textarget = textarget;
   cmd->texture = texture;
   cmd->level = level;
   cmd->layer = layer;
}


/* FramebufferTextureLayer: marshalled asynchronously */
struct marshal_cmd_FramebufferTextureLayer
{
   struct marshal_cmd_base cmd_base;
   GLenum target;
   GLenum attachment;
   GLuint texture;
   GLint level;
   GLint layer;
};
void
_mesa_unmarshal_FramebufferTextureLayer(struct gl_context *ctx, const struct marshal_cmd_FramebufferTextureLayer *cmd)
{
   GLenum target = cmd->target;
   GLenum attachment = cmd->attachment;
   GLuint texture = cmd->texture;
   GLint level = cmd->level;
   GLint layer = cmd->layer;
   CALL_FramebufferTextureLayer(ctx->CurrentServerDispatch, (target, attachment, texture, level, layer));
}
void GLAPIENTRY
_mesa_marshal_FramebufferTextureLayer(GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_FramebufferTextureLayer);
   struct marshal_cmd_FramebufferTextureLayer *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_FramebufferTextureLayer, cmd_size);
   cmd->target = target;
   cmd->attachment = attachment;
   cmd->texture = texture;
   cmd->level = level;
   cmd->layer = layer;
}


/* FramebufferRenderbuffer: marshalled asynchronously */
struct marshal_cmd_FramebufferRenderbuffer
{
   struct marshal_cmd_base cmd_base;
   GLenum target;
   GLenum attachment;
   GLenum renderbuffertarget;
   GLuint renderbuffer;
};
void
_mesa_unmarshal_FramebufferRenderbuffer(struct gl_context *ctx, const struct marshal_cmd_FramebufferRenderbuffer *cmd)
{
   GLenum target = cmd->target;
   GLenum attachment = cmd->attachment;
   GLenum renderbuffertarget = cmd->renderbuffertarget;
   GLuint renderbuffer = cmd->renderbuffer;
   CALL_FramebufferRenderbuffer(ctx->CurrentServerDispatch, (target, attachment, renderbuffertarget, renderbuffer));
}
void GLAPIENTRY
_mesa_marshal_FramebufferRenderbuffer(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_FramebufferRenderbuffer);
   struct marshal_cmd_FramebufferRenderbuffer *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_FramebufferRenderbuffer, cmd_size);
   cmd->target = target;
   cmd->attachment = attachment;
   cmd->renderbuffertarget = renderbuffertarget;
   cmd->renderbuffer = renderbuffer;
}


/* GetFramebufferAttachmentParameteriv: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetFramebufferAttachmentParameteriv(GLenum target, GLenum attachment, GLenum pname, GLint * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetFramebufferAttachmentParameteriv");
   CALL_GetFramebufferAttachmentParameteriv(ctx->CurrentServerDispatch, (target, attachment, pname, params));
}


/* BlitFramebuffer: marshalled asynchronously */
struct marshal_cmd_BlitFramebuffer
{
   struct marshal_cmd_base cmd_base;
   GLint srcX0;
   GLint srcY0;
   GLint srcX1;
   GLint srcY1;
   GLint dstX0;
   GLint dstY0;
   GLint dstX1;
   GLint dstY1;
   GLbitfield mask;
   GLenum filter;
};
void
_mesa_unmarshal_BlitFramebuffer(struct gl_context *ctx, const struct marshal_cmd_BlitFramebuffer *cmd)
{
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
   CALL_BlitFramebuffer(ctx->CurrentServerDispatch, (srcX0, srcY0, srcX1, srcY1, dstX0, dstY0, dstX1, dstY1, mask, filter));
}
void GLAPIENTRY
_mesa_marshal_BlitFramebuffer(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_BlitFramebuffer);
   struct marshal_cmd_BlitFramebuffer *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_BlitFramebuffer, cmd_size);
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


/* GenerateMipmap: marshalled asynchronously */
struct marshal_cmd_GenerateMipmap
{
   struct marshal_cmd_base cmd_base;
   GLenum target;
};
void
_mesa_unmarshal_GenerateMipmap(struct gl_context *ctx, const struct marshal_cmd_GenerateMipmap *cmd)
{
   GLenum target = cmd->target;
   CALL_GenerateMipmap(ctx->CurrentServerDispatch, (target));
}
void GLAPIENTRY
_mesa_marshal_GenerateMipmap(GLenum target)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_GenerateMipmap);
   struct marshal_cmd_GenerateMipmap *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_GenerateMipmap, cmd_size);
   cmd->target = target;
}


/* VertexAttribDivisor: marshalled asynchronously */
struct marshal_cmd_VertexAttribDivisor
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLuint divisor;
};
void
_mesa_unmarshal_VertexAttribDivisor(struct gl_context *ctx, const struct marshal_cmd_VertexAttribDivisor *cmd)
{
   GLuint index = cmd->index;
   GLuint divisor = cmd->divisor;
   CALL_VertexAttribDivisor(ctx->CurrentServerDispatch, (index, divisor));
}
void GLAPIENTRY
_mesa_marshal_VertexAttribDivisor(GLuint index, GLuint divisor)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttribDivisor);
   struct marshal_cmd_VertexAttribDivisor *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttribDivisor, cmd_size);
   cmd->index = index;
   cmd->divisor = divisor;
   if (COMPAT) _mesa_glthread_AttribDivisor(ctx, NULL, VERT_ATTRIB_GENERIC(index), divisor);
}


/* VertexArrayVertexAttribDivisorEXT: marshalled asynchronously */
struct marshal_cmd_VertexArrayVertexAttribDivisorEXT
{
   struct marshal_cmd_base cmd_base;
   GLuint vaobj;
   GLuint index;
   GLuint divisor;
};
void
_mesa_unmarshal_VertexArrayVertexAttribDivisorEXT(struct gl_context *ctx, const struct marshal_cmd_VertexArrayVertexAttribDivisorEXT *cmd)
{
   GLuint vaobj = cmd->vaobj;
   GLuint index = cmd->index;
   GLuint divisor = cmd->divisor;
   CALL_VertexArrayVertexAttribDivisorEXT(ctx->CurrentServerDispatch, (vaobj, index, divisor));
}
void GLAPIENTRY
_mesa_marshal_VertexArrayVertexAttribDivisorEXT(GLuint vaobj, GLuint index, GLuint divisor)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexArrayVertexAttribDivisorEXT);
   struct marshal_cmd_VertexArrayVertexAttribDivisorEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexArrayVertexAttribDivisorEXT, cmd_size);
   cmd->vaobj = vaobj;
   cmd->index = index;
   cmd->divisor = divisor;
   if (COMPAT) _mesa_glthread_AttribDivisor(ctx, &vaobj, VERT_ATTRIB_GENERIC(index), divisor);
}


/* MapBufferRange: marshalled synchronously */
GLvoid * GLAPIENTRY
_mesa_marshal_MapBufferRange(GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "MapBufferRange");
   return CALL_MapBufferRange(ctx->CurrentServerDispatch, (target, offset, length, access));
}


/* FlushMappedBufferRange: marshalled asynchronously */
struct marshal_cmd_FlushMappedBufferRange
{
   struct marshal_cmd_base cmd_base;
   GLenum target;
   GLintptr offset;
   GLsizeiptr length;
};
void
_mesa_unmarshal_FlushMappedBufferRange(struct gl_context *ctx, const struct marshal_cmd_FlushMappedBufferRange *cmd)
{
   GLenum target = cmd->target;
   GLintptr offset = cmd->offset;
   GLsizeiptr length = cmd->length;
   CALL_FlushMappedBufferRange(ctx->CurrentServerDispatch, (target, offset, length));
}
void GLAPIENTRY
_mesa_marshal_FlushMappedBufferRange(GLenum target, GLintptr offset, GLsizeiptr length)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_FlushMappedBufferRange);
   struct marshal_cmd_FlushMappedBufferRange *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_FlushMappedBufferRange, cmd_size);
   cmd->target = target;
   cmd->offset = offset;
   cmd->length = length;
}


/* TexBuffer: marshalled asynchronously */
struct marshal_cmd_TexBuffer
{
   struct marshal_cmd_base cmd_base;
   GLenum target;
   GLenum internalFormat;
   GLuint buffer;
};
void
_mesa_unmarshal_TexBuffer(struct gl_context *ctx, const struct marshal_cmd_TexBuffer *cmd)
{
   GLenum target = cmd->target;
   GLenum internalFormat = cmd->internalFormat;
   GLuint buffer = cmd->buffer;
   CALL_TexBuffer(ctx->CurrentServerDispatch, (target, internalFormat, buffer));
}
void GLAPIENTRY
_mesa_marshal_TexBuffer(GLenum target, GLenum internalFormat, GLuint buffer)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TexBuffer);
   struct marshal_cmd_TexBuffer *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexBuffer, cmd_size);
   cmd->target = target;
   cmd->internalFormat = internalFormat;
   cmd->buffer = buffer;
}


/* BindVertexArray: marshalled asynchronously */
struct marshal_cmd_BindVertexArray
{
   struct marshal_cmd_base cmd_base;
   GLuint array;
};
void
_mesa_unmarshal_BindVertexArray(struct gl_context *ctx, const struct marshal_cmd_BindVertexArray *cmd)
{
   GLuint array = cmd->array;
   CALL_BindVertexArray(ctx->CurrentServerDispatch, (array));
}
void GLAPIENTRY
_mesa_marshal_BindVertexArray(GLuint array)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_BindVertexArray);
   struct marshal_cmd_BindVertexArray *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_BindVertexArray, cmd_size);
   cmd->array = array;
   if (COMPAT) _mesa_glthread_BindVertexArray(ctx, array);
}


/* DeleteVertexArrays: marshalled asynchronously */
struct marshal_cmd_DeleteVertexArrays
{
   struct marshal_cmd_base cmd_base;
   GLsizei n;
   /* Next safe_mul(n, 1 * sizeof(GLuint)) bytes are GLuint arrays[n] */
};
void
_mesa_unmarshal_DeleteVertexArrays(struct gl_context *ctx, const struct marshal_cmd_DeleteVertexArrays *cmd)
{
   GLsizei n = cmd->n;
   GLuint * arrays;
   const char *variable_data = (const char *) (cmd + 1);
   arrays = (GLuint *) variable_data;
   CALL_DeleteVertexArrays(ctx->CurrentServerDispatch, (n, arrays));
}
void GLAPIENTRY
_mesa_marshal_DeleteVertexArrays(GLsizei n, const GLuint * arrays)
{
   GET_CURRENT_CONTEXT(ctx);
   int arrays_size = safe_mul(n, 1 * sizeof(GLuint));
   int cmd_size = sizeof(struct marshal_cmd_DeleteVertexArrays) + arrays_size;
   struct marshal_cmd_DeleteVertexArrays *cmd;
   if (unlikely(arrays_size < 0 || (arrays_size > 0 && !arrays) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "DeleteVertexArrays");
      CALL_DeleteVertexArrays(ctx->CurrentServerDispatch, (n, arrays));
      if (COMPAT) _mesa_glthread_DeleteVertexArrays(ctx, n, arrays);
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_DeleteVertexArrays, cmd_size);
   cmd->n = n;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, arrays, arrays_size);
   if (COMPAT) _mesa_glthread_DeleteVertexArrays(ctx, n, arrays);
}


/* GenVertexArrays: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GenVertexArrays(GLsizei n, GLuint * arrays)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GenVertexArrays");
   CALL_GenVertexArrays(ctx->CurrentServerDispatch, (n, arrays));
   if (COMPAT) _mesa_glthread_GenVertexArrays(ctx, n, arrays);
}


/* IsVertexArray: marshalled synchronously */
GLboolean GLAPIENTRY
_mesa_marshal_IsVertexArray(GLuint array)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "IsVertexArray");
   return CALL_IsVertexArray(ctx->CurrentServerDispatch, (array));
}


/* GetUniformIndices: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetUniformIndices(GLuint program, GLsizei uniformCount, const GLchar * const * uniformNames, GLuint * uniformIndices)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetUniformIndices");
   CALL_GetUniformIndices(ctx->CurrentServerDispatch, (program, uniformCount, uniformNames, uniformIndices));
}


/* GetActiveUniformsiv: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetActiveUniformsiv(GLuint program, GLsizei uniformCount, const GLuint * uniformIndices, GLenum pname, GLint * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetActiveUniformsiv");
   CALL_GetActiveUniformsiv(ctx->CurrentServerDispatch, (program, uniformCount, uniformIndices, pname, params));
}


/* GetActiveUniformName: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetActiveUniformName(GLuint program, GLuint uniformIndex, GLsizei bufSize, GLsizei * length, GLchar * uniformName)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetActiveUniformName");
   CALL_GetActiveUniformName(ctx->CurrentServerDispatch, (program, uniformIndex, bufSize, length, uniformName));
}


/* GetUniformBlockIndex: marshalled synchronously */
GLuint GLAPIENTRY
_mesa_marshal_GetUniformBlockIndex(GLuint program, const GLchar * uniformBlockName)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetUniformBlockIndex");
   return CALL_GetUniformBlockIndex(ctx->CurrentServerDispatch, (program, uniformBlockName));
}


/* GetActiveUniformBlockiv: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetActiveUniformBlockiv(GLuint program, GLuint uniformBlockIndex, GLenum pname, GLint * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetActiveUniformBlockiv");
   CALL_GetActiveUniformBlockiv(ctx->CurrentServerDispatch, (program, uniformBlockIndex, pname, params));
}


/* GetActiveUniformBlockName: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetActiveUniformBlockName(GLuint program, GLuint uniformBlockIndex, GLsizei bufSize, GLsizei * length, GLchar * uniformBlockName)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetActiveUniformBlockName");
   CALL_GetActiveUniformBlockName(ctx->CurrentServerDispatch, (program, uniformBlockIndex, bufSize, length, uniformBlockName));
}


/* UniformBlockBinding: marshalled asynchronously */
struct marshal_cmd_UniformBlockBinding
{
   struct marshal_cmd_base cmd_base;
   GLuint program;
   GLuint uniformBlockIndex;
   GLuint uniformBlockBinding;
};
void
_mesa_unmarshal_UniformBlockBinding(struct gl_context *ctx, const struct marshal_cmd_UniformBlockBinding *cmd)
{
   GLuint program = cmd->program;
   GLuint uniformBlockIndex = cmd->uniformBlockIndex;
   GLuint uniformBlockBinding = cmd->uniformBlockBinding;
   CALL_UniformBlockBinding(ctx->CurrentServerDispatch, (program, uniformBlockIndex, uniformBlockBinding));
}
void GLAPIENTRY
_mesa_marshal_UniformBlockBinding(GLuint program, GLuint uniformBlockIndex, GLuint uniformBlockBinding)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_UniformBlockBinding);
   struct marshal_cmd_UniformBlockBinding *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_UniformBlockBinding, cmd_size);
   cmd->program = program;
   cmd->uniformBlockIndex = uniformBlockIndex;
   cmd->uniformBlockBinding = uniformBlockBinding;
}


/* CopyBufferSubData: marshalled asynchronously */
struct marshal_cmd_CopyBufferSubData
{
   struct marshal_cmd_base cmd_base;
   GLenum readTarget;
   GLenum writeTarget;
   GLintptr readOffset;
   GLintptr writeOffset;
   GLsizeiptr size;
};
void
_mesa_unmarshal_CopyBufferSubData(struct gl_context *ctx, const struct marshal_cmd_CopyBufferSubData *cmd)
{
   GLenum readTarget = cmd->readTarget;
   GLenum writeTarget = cmd->writeTarget;
   GLintptr readOffset = cmd->readOffset;
   GLintptr writeOffset = cmd->writeOffset;
   GLsizeiptr size = cmd->size;
   CALL_CopyBufferSubData(ctx->CurrentServerDispatch, (readTarget, writeTarget, readOffset, writeOffset, size));
}
void GLAPIENTRY
_mesa_marshal_CopyBufferSubData(GLenum readTarget, GLenum writeTarget, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_CopyBufferSubData);
   struct marshal_cmd_CopyBufferSubData *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_CopyBufferSubData, cmd_size);
   cmd->readTarget = readTarget;
   cmd->writeTarget = writeTarget;
   cmd->readOffset = readOffset;
   cmd->writeOffset = writeOffset;
   cmd->size = size;
}


/* FenceSync: marshalled synchronously */
GLsync GLAPIENTRY
_mesa_marshal_FenceSync(GLenum condition, GLbitfield flags)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "FenceSync");
   return CALL_FenceSync(ctx->CurrentServerDispatch, (condition, flags));
}


/* IsSync: marshalled synchronously */
GLboolean GLAPIENTRY
_mesa_marshal_IsSync(GLsync sync)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "IsSync");
   return CALL_IsSync(ctx->CurrentServerDispatch, (sync));
}


/* DeleteSync: marshalled asynchronously */
struct marshal_cmd_DeleteSync
{
   struct marshal_cmd_base cmd_base;
   GLsync sync;
};
void
_mesa_unmarshal_DeleteSync(struct gl_context *ctx, const struct marshal_cmd_DeleteSync *cmd)
{
   GLsync sync = cmd->sync;
   CALL_DeleteSync(ctx->CurrentServerDispatch, (sync));
}
void GLAPIENTRY
_mesa_marshal_DeleteSync(GLsync sync)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_DeleteSync);
   struct marshal_cmd_DeleteSync *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_DeleteSync, cmd_size);
   cmd->sync = sync;
}


/* ClientWaitSync: marshalled synchronously */
GLenum GLAPIENTRY
_mesa_marshal_ClientWaitSync(GLsync sync, GLbitfield flags, GLuint64 timeout)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "ClientWaitSync");
   return CALL_ClientWaitSync(ctx->CurrentServerDispatch, (sync, flags, timeout));
}


/* WaitSync: marshalled asynchronously */
struct marshal_cmd_WaitSync
{
   struct marshal_cmd_base cmd_base;
   GLbitfield flags;
   GLsync sync;
   GLuint64 timeout;
};
void
_mesa_unmarshal_WaitSync(struct gl_context *ctx, const struct marshal_cmd_WaitSync *cmd)
{
   GLsync sync = cmd->sync;
   GLbitfield flags = cmd->flags;
   GLuint64 timeout = cmd->timeout;
   CALL_WaitSync(ctx->CurrentServerDispatch, (sync, flags, timeout));
}
void GLAPIENTRY
_mesa_marshal_WaitSync(GLsync sync, GLbitfield flags, GLuint64 timeout)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_WaitSync);
   struct marshal_cmd_WaitSync *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_WaitSync, cmd_size);
   cmd->sync = sync;
   cmd->flags = flags;
   cmd->timeout = timeout;
}


/* GetInteger64v: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetInteger64v(GLenum pname, GLint64 * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetInteger64v");
   CALL_GetInteger64v(ctx->CurrentServerDispatch, (pname, params));
}


/* GetSynciv: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetSynciv(GLsync sync, GLenum pname, GLsizei bufSize, GLsizei * length, GLint * values)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetSynciv");
   CALL_GetSynciv(ctx->CurrentServerDispatch, (sync, pname, bufSize, length, values));
}


/* TexImage2DMultisample: marshalled asynchronously */
struct marshal_cmd_TexImage2DMultisample
{
   struct marshal_cmd_base cmd_base;
   GLboolean fixedsamplelocations;
   GLenum target;
   GLsizei samples;
   GLenum internalformat;
   GLsizei width;
   GLsizei height;
};
void
_mesa_unmarshal_TexImage2DMultisample(struct gl_context *ctx, const struct marshal_cmd_TexImage2DMultisample *cmd)
{
   GLenum target = cmd->target;
   GLsizei samples = cmd->samples;
   GLenum internalformat = cmd->internalformat;
   GLsizei width = cmd->width;
   GLsizei height = cmd->height;
   GLboolean fixedsamplelocations = cmd->fixedsamplelocations;
   CALL_TexImage2DMultisample(ctx->CurrentServerDispatch, (target, samples, internalformat, width, height, fixedsamplelocations));
}
void GLAPIENTRY
_mesa_marshal_TexImage2DMultisample(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TexImage2DMultisample);
   struct marshal_cmd_TexImage2DMultisample *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexImage2DMultisample, cmd_size);
   cmd->target = target;
   cmd->samples = samples;
   cmd->internalformat = internalformat;
   cmd->width = width;
   cmd->height = height;
   cmd->fixedsamplelocations = fixedsamplelocations;
}


/* TexImage3DMultisample: marshalled asynchronously */
struct marshal_cmd_TexImage3DMultisample
{
   struct marshal_cmd_base cmd_base;
   GLboolean fixedsamplelocations;
   GLenum target;
   GLsizei samples;
   GLenum internalformat;
   GLsizei width;
   GLsizei height;
   GLsizei depth;
};
void
_mesa_unmarshal_TexImage3DMultisample(struct gl_context *ctx, const struct marshal_cmd_TexImage3DMultisample *cmd)
{
   GLenum target = cmd->target;
   GLsizei samples = cmd->samples;
   GLenum internalformat = cmd->internalformat;
   GLsizei width = cmd->width;
   GLsizei height = cmd->height;
   GLsizei depth = cmd->depth;
   GLboolean fixedsamplelocations = cmd->fixedsamplelocations;
   CALL_TexImage3DMultisample(ctx->CurrentServerDispatch, (target, samples, internalformat, width, height, depth, fixedsamplelocations));
}
void GLAPIENTRY
_mesa_marshal_TexImage3DMultisample(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TexImage3DMultisample);
   struct marshal_cmd_TexImage3DMultisample *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexImage3DMultisample, cmd_size);
   cmd->target = target;
   cmd->samples = samples;
   cmd->internalformat = internalformat;
   cmd->width = width;
   cmd->height = height;
   cmd->depth = depth;
   cmd->fixedsamplelocations = fixedsamplelocations;
}


/* GetMultisamplefv: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetMultisamplefv(GLenum pname, GLuint index, GLfloat * val)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetMultisamplefv");
   CALL_GetMultisamplefv(ctx->CurrentServerDispatch, (pname, index, val));
}


/* SampleMaski: marshalled asynchronously */
struct marshal_cmd_SampleMaski
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLbitfield mask;
};
void
_mesa_unmarshal_SampleMaski(struct gl_context *ctx, const struct marshal_cmd_SampleMaski *cmd)
{
   GLuint index = cmd->index;
   GLbitfield mask = cmd->mask;
   CALL_SampleMaski(ctx->CurrentServerDispatch, (index, mask));
}
void GLAPIENTRY
_mesa_marshal_SampleMaski(GLuint index, GLbitfield mask)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_SampleMaski);
   struct marshal_cmd_SampleMaski *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_SampleMaski, cmd_size);
   cmd->index = index;
   cmd->mask = mask;
}


/* BlendEquationiARB: marshalled asynchronously */
struct marshal_cmd_BlendEquationiARB
{
   struct marshal_cmd_base cmd_base;
   GLuint buf;
   GLenum mode;
};
void
_mesa_unmarshal_BlendEquationiARB(struct gl_context *ctx, const struct marshal_cmd_BlendEquationiARB *cmd)
{
   GLuint buf = cmd->buf;
   GLenum mode = cmd->mode;
   CALL_BlendEquationiARB(ctx->CurrentServerDispatch, (buf, mode));
}
void GLAPIENTRY
_mesa_marshal_BlendEquationiARB(GLuint buf, GLenum mode)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_BlendEquationiARB);
   struct marshal_cmd_BlendEquationiARB *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_BlendEquationiARB, cmd_size);
   cmd->buf = buf;
   cmd->mode = mode;
}


/* BlendEquationSeparateiARB: marshalled asynchronously */
struct marshal_cmd_BlendEquationSeparateiARB
{
   struct marshal_cmd_base cmd_base;
   GLuint buf;
   GLenum modeRGB;
   GLenum modeA;
};
void
_mesa_unmarshal_BlendEquationSeparateiARB(struct gl_context *ctx, const struct marshal_cmd_BlendEquationSeparateiARB *cmd)
{
   GLuint buf = cmd->buf;
   GLenum modeRGB = cmd->modeRGB;
   GLenum modeA = cmd->modeA;
   CALL_BlendEquationSeparateiARB(ctx->CurrentServerDispatch, (buf, modeRGB, modeA));
}
void GLAPIENTRY
_mesa_marshal_BlendEquationSeparateiARB(GLuint buf, GLenum modeRGB, GLenum modeA)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_BlendEquationSeparateiARB);
   struct marshal_cmd_BlendEquationSeparateiARB *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_BlendEquationSeparateiARB, cmd_size);
   cmd->buf = buf;
   cmd->modeRGB = modeRGB;
   cmd->modeA = modeA;
}


/* BlendFunciARB: marshalled asynchronously */
struct marshal_cmd_BlendFunciARB
{
   struct marshal_cmd_base cmd_base;
   GLuint buf;
   GLenum src;
   GLenum dst;
};
void
_mesa_unmarshal_BlendFunciARB(struct gl_context *ctx, const struct marshal_cmd_BlendFunciARB *cmd)
{
   GLuint buf = cmd->buf;
   GLenum src = cmd->src;
   GLenum dst = cmd->dst;
   CALL_BlendFunciARB(ctx->CurrentServerDispatch, (buf, src, dst));
}
void GLAPIENTRY
_mesa_marshal_BlendFunciARB(GLuint buf, GLenum src, GLenum dst)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_BlendFunciARB);
   struct marshal_cmd_BlendFunciARB *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_BlendFunciARB, cmd_size);
   cmd->buf = buf;
   cmd->src = src;
   cmd->dst = dst;
}


/* BlendFuncSeparateiARB: marshalled asynchronously */
struct marshal_cmd_BlendFuncSeparateiARB
{
   struct marshal_cmd_base cmd_base;
   GLuint buf;
   GLenum srcRGB;
   GLenum dstRGB;
   GLenum srcA;
   GLenum dstA;
};
void
_mesa_unmarshal_BlendFuncSeparateiARB(struct gl_context *ctx, const struct marshal_cmd_BlendFuncSeparateiARB *cmd)
{
   GLuint buf = cmd->buf;
   GLenum srcRGB = cmd->srcRGB;
   GLenum dstRGB = cmd->dstRGB;
   GLenum srcA = cmd->srcA;
   GLenum dstA = cmd->dstA;
   CALL_BlendFuncSeparateiARB(ctx->CurrentServerDispatch, (buf, srcRGB, dstRGB, srcA, dstA));
}
void GLAPIENTRY
_mesa_marshal_BlendFuncSeparateiARB(GLuint buf, GLenum srcRGB, GLenum dstRGB, GLenum srcA, GLenum dstA)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_BlendFuncSeparateiARB);
   struct marshal_cmd_BlendFuncSeparateiARB *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_BlendFuncSeparateiARB, cmd_size);
   cmd->buf = buf;
   cmd->srcRGB = srcRGB;
   cmd->dstRGB = dstRGB;
   cmd->srcA = srcA;
   cmd->dstA = dstA;
}


/* MinSampleShading: marshalled asynchronously */
struct marshal_cmd_MinSampleShading
{
   struct marshal_cmd_base cmd_base;
   GLfloat value;
};
void
_mesa_unmarshal_MinSampleShading(struct gl_context *ctx, const struct marshal_cmd_MinSampleShading *cmd)
{
   GLfloat value = cmd->value;
   CALL_MinSampleShading(ctx->CurrentServerDispatch, (value));
}
void GLAPIENTRY
_mesa_marshal_MinSampleShading(GLfloat value)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MinSampleShading);
   struct marshal_cmd_MinSampleShading *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MinSampleShading, cmd_size);
   cmd->value = value;
}


/* NamedStringARB: marshalled asynchronously */
struct marshal_cmd_NamedStringARB
{
   struct marshal_cmd_base cmd_base;
   GLenum type;
   GLint namelen;
   GLint stringlen;
   /* Next namelen bytes are GLchar name[namelen] */
   /* Next stringlen bytes are GLchar string[stringlen] */
};
void
_mesa_unmarshal_NamedStringARB(struct gl_context *ctx, const struct marshal_cmd_NamedStringARB *cmd)
{
   GLenum type = cmd->type;
   GLint namelen = cmd->namelen;
   GLint stringlen = cmd->stringlen;
   GLchar * name;
   GLchar * string;
   const char *variable_data = (const char *) (cmd + 1);
   name = (GLchar *) variable_data;
   variable_data += namelen;
   string = (GLchar *) variable_data;
   CALL_NamedStringARB(ctx->CurrentServerDispatch, (type, namelen, name, stringlen, string));
}
void GLAPIENTRY
_mesa_marshal_NamedStringARB(GLenum type, GLint namelen, const GLchar * name, GLint stringlen, const GLchar * string)
{
   GET_CURRENT_CONTEXT(ctx);
   int name_size = namelen;
   int string_size = stringlen;
   int cmd_size = sizeof(struct marshal_cmd_NamedStringARB) + name_size + string_size;
   struct marshal_cmd_NamedStringARB *cmd;
   if (unlikely(name_size < 0 || (name_size > 0 && !name) || string_size < 0 || (string_size > 0 && !string) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "NamedStringARB");
      CALL_NamedStringARB(ctx->CurrentServerDispatch, (type, namelen, name, stringlen, string));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_NamedStringARB, cmd_size);
   cmd->type = type;
   cmd->namelen = namelen;
   cmd->stringlen = stringlen;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, name, name_size);
   variable_data += name_size;
   memcpy(variable_data, string, string_size);
}


/* DeleteNamedStringARB: marshalled asynchronously */
struct marshal_cmd_DeleteNamedStringARB
{
   struct marshal_cmd_base cmd_base;
   GLint namelen;
   /* Next namelen bytes are GLchar name[namelen] */
};
void
_mesa_unmarshal_DeleteNamedStringARB(struct gl_context *ctx, const struct marshal_cmd_DeleteNamedStringARB *cmd)
{
   GLint namelen = cmd->namelen;
   GLchar * name;
   const char *variable_data = (const char *) (cmd + 1);
   name = (GLchar *) variable_data;
   CALL_DeleteNamedStringARB(ctx->CurrentServerDispatch, (namelen, name));
}
void GLAPIENTRY
_mesa_marshal_DeleteNamedStringARB(GLint namelen, const GLchar * name)
{
   GET_CURRENT_CONTEXT(ctx);
   int name_size = namelen;
   int cmd_size = sizeof(struct marshal_cmd_DeleteNamedStringARB) + name_size;
   struct marshal_cmd_DeleteNamedStringARB *cmd;
   if (unlikely(name_size < 0 || (name_size > 0 && !name) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "DeleteNamedStringARB");
      CALL_DeleteNamedStringARB(ctx->CurrentServerDispatch, (namelen, name));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_DeleteNamedStringARB, cmd_size);
   cmd->namelen = namelen;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, name, name_size);
}


/* CompileShaderIncludeARB: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_CompileShaderIncludeARB(GLuint shader, GLsizei count, const GLchar * const * path, const GLint * length)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "CompileShaderIncludeARB");
   CALL_CompileShaderIncludeARB(ctx->CurrentServerDispatch, (shader, count, path, length));
}


/* IsNamedStringARB: marshalled synchronously */
GLboolean GLAPIENTRY
_mesa_marshal_IsNamedStringARB(GLint namelen, const GLchar * name)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "IsNamedStringARB");
   return CALL_IsNamedStringARB(ctx->CurrentServerDispatch, (namelen, name));
}


/* GetNamedStringARB: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetNamedStringARB(GLint namelen, const GLchar * name, GLsizei bufSize, GLint * stringlen, GLchar * string)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetNamedStringARB");
   CALL_GetNamedStringARB(ctx->CurrentServerDispatch, (namelen, name, bufSize, stringlen, string));
}


/* GetNamedStringivARB: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetNamedStringivARB(GLint namelen, const GLchar * name, GLenum pname, GLint * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetNamedStringivARB");
   CALL_GetNamedStringivARB(ctx->CurrentServerDispatch, (namelen, name, pname, params));
}


/* BindFragDataLocationIndexed: marshalled asynchronously */
struct marshal_cmd_BindFragDataLocationIndexed
{
   struct marshal_cmd_base cmd_base;
   GLuint program;
   GLuint colorNumber;
   GLuint index;
   /* Next (strlen(name) + 1) bytes are GLchar name[(strlen(name) + 1)] */
};
void
_mesa_unmarshal_BindFragDataLocationIndexed(struct gl_context *ctx, const struct marshal_cmd_BindFragDataLocationIndexed *cmd)
{
   GLuint program = cmd->program;
   GLuint colorNumber = cmd->colorNumber;
   GLuint index = cmd->index;
   GLchar * name;
   const char *variable_data = (const char *) (cmd + 1);
   name = (GLchar *) variable_data;
   CALL_BindFragDataLocationIndexed(ctx->CurrentServerDispatch, (program, colorNumber, index, name));
}
void GLAPIENTRY
_mesa_marshal_BindFragDataLocationIndexed(GLuint program, GLuint colorNumber, GLuint index, const GLchar * name)
{
   GET_CURRENT_CONTEXT(ctx);
   int name_size = (strlen(name) + 1);
   int cmd_size = sizeof(struct marshal_cmd_BindFragDataLocationIndexed) + name_size;
   struct marshal_cmd_BindFragDataLocationIndexed *cmd;
   if (unlikely(name_size < 0 || (name_size > 0 && !name) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "BindFragDataLocationIndexed");
      CALL_BindFragDataLocationIndexed(ctx->CurrentServerDispatch, (program, colorNumber, index, name));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_BindFragDataLocationIndexed, cmd_size);
   cmd->program = program;
   cmd->colorNumber = colorNumber;
   cmd->index = index;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, name, name_size);
}


/* GetFragDataIndex: marshalled synchronously */
GLint GLAPIENTRY
_mesa_marshal_GetFragDataIndex(GLuint program, const GLchar * name)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetFragDataIndex");
   return CALL_GetFragDataIndex(ctx->CurrentServerDispatch, (program, name));
}


/* GenSamplers: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GenSamplers(GLsizei count, GLuint * samplers)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GenSamplers");
   CALL_GenSamplers(ctx->CurrentServerDispatch, (count, samplers));
}


/* DeleteSamplers: marshalled asynchronously */
struct marshal_cmd_DeleteSamplers
{
   struct marshal_cmd_base cmd_base;
   GLsizei count;
   /* Next safe_mul(count, 1 * sizeof(GLuint)) bytes are GLuint samplers[count] */
};
void
_mesa_unmarshal_DeleteSamplers(struct gl_context *ctx, const struct marshal_cmd_DeleteSamplers *cmd)
{
   GLsizei count = cmd->count;
   GLuint * samplers;
   const char *variable_data = (const char *) (cmd + 1);
   samplers = (GLuint *) variable_data;
   CALL_DeleteSamplers(ctx->CurrentServerDispatch, (count, samplers));
}
void GLAPIENTRY
_mesa_marshal_DeleteSamplers(GLsizei count, const GLuint * samplers)
{
   GET_CURRENT_CONTEXT(ctx);
   int samplers_size = safe_mul(count, 1 * sizeof(GLuint));
   int cmd_size = sizeof(struct marshal_cmd_DeleteSamplers) + samplers_size;
   struct marshal_cmd_DeleteSamplers *cmd;
   if (unlikely(samplers_size < 0 || (samplers_size > 0 && !samplers) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "DeleteSamplers");
      CALL_DeleteSamplers(ctx->CurrentServerDispatch, (count, samplers));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_DeleteSamplers, cmd_size);
   cmd->count = count;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, samplers, samplers_size);
}


/* IsSampler: marshalled synchronously */
GLboolean GLAPIENTRY
_mesa_marshal_IsSampler(GLuint sampler)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "IsSampler");
   return CALL_IsSampler(ctx->CurrentServerDispatch, (sampler));
}


/* BindSampler: marshalled asynchronously */
struct marshal_cmd_BindSampler
{
   struct marshal_cmd_base cmd_base;
   GLuint unit;
   GLuint sampler;
};
void
_mesa_unmarshal_BindSampler(struct gl_context *ctx, const struct marshal_cmd_BindSampler *cmd)
{
   GLuint unit = cmd->unit;
   GLuint sampler = cmd->sampler;
   CALL_BindSampler(ctx->CurrentServerDispatch, (unit, sampler));
}
void GLAPIENTRY
_mesa_marshal_BindSampler(GLuint unit, GLuint sampler)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_BindSampler);
   struct marshal_cmd_BindSampler *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_BindSampler, cmd_size);
   cmd->unit = unit;
   cmd->sampler = sampler;
}


/* SamplerParameteri: marshalled asynchronously */
struct marshal_cmd_SamplerParameteri
{
   struct marshal_cmd_base cmd_base;
   GLuint sampler;
   GLenum pname;
   GLint param;
};
void
_mesa_unmarshal_SamplerParameteri(struct gl_context *ctx, const struct marshal_cmd_SamplerParameteri *cmd)
{
   GLuint sampler = cmd->sampler;
   GLenum pname = cmd->pname;
   GLint param = cmd->param;
   CALL_SamplerParameteri(ctx->CurrentServerDispatch, (sampler, pname, param));
}
void GLAPIENTRY
_mesa_marshal_SamplerParameteri(GLuint sampler, GLenum pname, GLint param)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_SamplerParameteri);
   struct marshal_cmd_SamplerParameteri *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_SamplerParameteri, cmd_size);
   cmd->sampler = sampler;
   cmd->pname = pname;
   cmd->param = param;
}


/* SamplerParameterf: marshalled asynchronously */
struct marshal_cmd_SamplerParameterf
{
   struct marshal_cmd_base cmd_base;
   GLuint sampler;
   GLenum pname;
   GLfloat param;
};
void
_mesa_unmarshal_SamplerParameterf(struct gl_context *ctx, const struct marshal_cmd_SamplerParameterf *cmd)
{
   GLuint sampler = cmd->sampler;
   GLenum pname = cmd->pname;
   GLfloat param = cmd->param;
   CALL_SamplerParameterf(ctx->CurrentServerDispatch, (sampler, pname, param));
}
void GLAPIENTRY
_mesa_marshal_SamplerParameterf(GLuint sampler, GLenum pname, GLfloat param)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_SamplerParameterf);
   struct marshal_cmd_SamplerParameterf *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_SamplerParameterf, cmd_size);
   cmd->sampler = sampler;
   cmd->pname = pname;
   cmd->param = param;
}


/* SamplerParameteriv: marshalled asynchronously */
struct marshal_cmd_SamplerParameteriv
{
   struct marshal_cmd_base cmd_base;
   GLuint sampler;
   GLenum pname;
   /* Next safe_mul(_mesa_tex_param_enum_to_count(pname), 1 * sizeof(GLint)) bytes are GLint params[_mesa_tex_param_enum_to_count(pname)] */
};
void
_mesa_unmarshal_SamplerParameteriv(struct gl_context *ctx, const struct marshal_cmd_SamplerParameteriv *cmd)
{
   GLuint sampler = cmd->sampler;
   GLenum pname = cmd->pname;
   GLint * params;
   const char *variable_data = (const char *) (cmd + 1);
   params = (GLint *) variable_data;
   CALL_SamplerParameteriv(ctx->CurrentServerDispatch, (sampler, pname, params));
}
void GLAPIENTRY
_mesa_marshal_SamplerParameteriv(GLuint sampler, GLenum pname, const GLint * params)
{
   GET_CURRENT_CONTEXT(ctx);
   int params_size = safe_mul(_mesa_tex_param_enum_to_count(pname), 1 * sizeof(GLint));
   int cmd_size = sizeof(struct marshal_cmd_SamplerParameteriv) + params_size;
   struct marshal_cmd_SamplerParameteriv *cmd;
   if (unlikely(params_size < 0 || (params_size > 0 && !params) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "SamplerParameteriv");
      CALL_SamplerParameteriv(ctx->CurrentServerDispatch, (sampler, pname, params));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_SamplerParameteriv, cmd_size);
   cmd->sampler = sampler;
   cmd->pname = pname;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, params, params_size);
}


/* SamplerParameterfv: marshalled asynchronously */
struct marshal_cmd_SamplerParameterfv
{
   struct marshal_cmd_base cmd_base;
   GLuint sampler;
   GLenum pname;
   /* Next safe_mul(_mesa_tex_param_enum_to_count(pname), 1 * sizeof(GLfloat)) bytes are GLfloat params[_mesa_tex_param_enum_to_count(pname)] */
};
void
_mesa_unmarshal_SamplerParameterfv(struct gl_context *ctx, const struct marshal_cmd_SamplerParameterfv *cmd)
{
   GLuint sampler = cmd->sampler;
   GLenum pname = cmd->pname;
   GLfloat * params;
   const char *variable_data = (const char *) (cmd + 1);
   params = (GLfloat *) variable_data;
   CALL_SamplerParameterfv(ctx->CurrentServerDispatch, (sampler, pname, params));
}
void GLAPIENTRY
_mesa_marshal_SamplerParameterfv(GLuint sampler, GLenum pname, const GLfloat * params)
{
   GET_CURRENT_CONTEXT(ctx);
   int params_size = safe_mul(_mesa_tex_param_enum_to_count(pname), 1 * sizeof(GLfloat));
   int cmd_size = sizeof(struct marshal_cmd_SamplerParameterfv) + params_size;
   struct marshal_cmd_SamplerParameterfv *cmd;
   if (unlikely(params_size < 0 || (params_size > 0 && !params) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "SamplerParameterfv");
      CALL_SamplerParameterfv(ctx->CurrentServerDispatch, (sampler, pname, params));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_SamplerParameterfv, cmd_size);
   cmd->sampler = sampler;
   cmd->pname = pname;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, params, params_size);
}


/* SamplerParameterIiv: marshalled asynchronously */
struct marshal_cmd_SamplerParameterIiv
{
   struct marshal_cmd_base cmd_base;
   GLuint sampler;
   GLenum pname;
   /* Next safe_mul(_mesa_tex_param_enum_to_count(pname), 1 * sizeof(GLint)) bytes are GLint params[_mesa_tex_param_enum_to_count(pname)] */
};
void
_mesa_unmarshal_SamplerParameterIiv(struct gl_context *ctx, const struct marshal_cmd_SamplerParameterIiv *cmd)
{
   GLuint sampler = cmd->sampler;
   GLenum pname = cmd->pname;
   GLint * params;
   const char *variable_data = (const char *) (cmd + 1);
   params = (GLint *) variable_data;
   CALL_SamplerParameterIiv(ctx->CurrentServerDispatch, (sampler, pname, params));
}
void GLAPIENTRY
_mesa_marshal_SamplerParameterIiv(GLuint sampler, GLenum pname, const GLint * params)
{
   GET_CURRENT_CONTEXT(ctx);
   int params_size = safe_mul(_mesa_tex_param_enum_to_count(pname), 1 * sizeof(GLint));
   int cmd_size = sizeof(struct marshal_cmd_SamplerParameterIiv) + params_size;
   struct marshal_cmd_SamplerParameterIiv *cmd;
   if (unlikely(params_size < 0 || (params_size > 0 && !params) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "SamplerParameterIiv");
      CALL_SamplerParameterIiv(ctx->CurrentServerDispatch, (sampler, pname, params));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_SamplerParameterIiv, cmd_size);
   cmd->sampler = sampler;
   cmd->pname = pname;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, params, params_size);
}


/* SamplerParameterIuiv: marshalled asynchronously */
struct marshal_cmd_SamplerParameterIuiv
{
   struct marshal_cmd_base cmd_base;
   GLuint sampler;
   GLenum pname;
   /* Next safe_mul(_mesa_tex_param_enum_to_count(pname), 1 * sizeof(GLuint)) bytes are GLuint params[_mesa_tex_param_enum_to_count(pname)] */
};
void
_mesa_unmarshal_SamplerParameterIuiv(struct gl_context *ctx, const struct marshal_cmd_SamplerParameterIuiv *cmd)
{
   GLuint sampler = cmd->sampler;
   GLenum pname = cmd->pname;
   GLuint * params;
   const char *variable_data = (const char *) (cmd + 1);
   params = (GLuint *) variable_data;
   CALL_SamplerParameterIuiv(ctx->CurrentServerDispatch, (sampler, pname, params));
}
void GLAPIENTRY
_mesa_marshal_SamplerParameterIuiv(GLuint sampler, GLenum pname, const GLuint * params)
{
   GET_CURRENT_CONTEXT(ctx);
   int params_size = safe_mul(_mesa_tex_param_enum_to_count(pname), 1 * sizeof(GLuint));
   int cmd_size = sizeof(struct marshal_cmd_SamplerParameterIuiv) + params_size;
   struct marshal_cmd_SamplerParameterIuiv *cmd;
   if (unlikely(params_size < 0 || (params_size > 0 && !params) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "SamplerParameterIuiv");
      CALL_SamplerParameterIuiv(ctx->CurrentServerDispatch, (sampler, pname, params));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_SamplerParameterIuiv, cmd_size);
   cmd->sampler = sampler;
   cmd->pname = pname;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, params, params_size);
}


/* GetSamplerParameteriv: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetSamplerParameteriv(GLuint sampler, GLenum pname, GLint * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetSamplerParameteriv");
   CALL_GetSamplerParameteriv(ctx->CurrentServerDispatch, (sampler, pname, params));
}


/* GetSamplerParameterfv: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetSamplerParameterfv(GLuint sampler, GLenum pname, GLfloat * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetSamplerParameterfv");
   CALL_GetSamplerParameterfv(ctx->CurrentServerDispatch, (sampler, pname, params));
}


/* GetSamplerParameterIiv: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetSamplerParameterIiv(GLuint sampler, GLenum pname, GLint * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetSamplerParameterIiv");
   CALL_GetSamplerParameterIiv(ctx->CurrentServerDispatch, (sampler, pname, params));
}


/* GetSamplerParameterIuiv: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetSamplerParameterIuiv(GLuint sampler, GLenum pname, GLuint * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetSamplerParameterIuiv");
   CALL_GetSamplerParameterIuiv(ctx->CurrentServerDispatch, (sampler, pname, params));
}


/* GetQueryObjecti64v: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetQueryObjecti64v(GLuint id, GLenum pname, GLint64 * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetQueryObjecti64v");
   CALL_GetQueryObjecti64v(ctx->CurrentServerDispatch, (id, pname, params));
}


/* GetQueryObjectui64v: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetQueryObjectui64v(GLuint id, GLenum pname, GLuint64 * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetQueryObjectui64v");
   CALL_GetQueryObjectui64v(ctx->CurrentServerDispatch, (id, pname, params));
}


/* QueryCounter: marshalled asynchronously */
struct marshal_cmd_QueryCounter
{
   struct marshal_cmd_base cmd_base;
   GLuint id;
   GLenum target;
};
void
_mesa_unmarshal_QueryCounter(struct gl_context *ctx, const struct marshal_cmd_QueryCounter *cmd)
{
   GLuint id = cmd->id;
   GLenum target = cmd->target;
   CALL_QueryCounter(ctx->CurrentServerDispatch, (id, target));
}
void GLAPIENTRY
_mesa_marshal_QueryCounter(GLuint id, GLenum target)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_QueryCounter);
   struct marshal_cmd_QueryCounter *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_QueryCounter, cmd_size);
   cmd->id = id;
   cmd->target = target;
}


/* VertexP2ui: marshalled asynchronously */
struct marshal_cmd_VertexP2ui
{
   struct marshal_cmd_base cmd_base;
   GLenum type;
   GLuint value;
};
void
_mesa_unmarshal_VertexP2ui(struct gl_context *ctx, const struct marshal_cmd_VertexP2ui *cmd)
{
   GLenum type = cmd->type;
   GLuint value = cmd->value;
   CALL_VertexP2ui(ctx->CurrentServerDispatch, (type, value));
}
void GLAPIENTRY
_mesa_marshal_VertexP2ui(GLenum type, GLuint value)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexP2ui);
   struct marshal_cmd_VertexP2ui *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexP2ui, cmd_size);
   cmd->type = type;
   cmd->value = value;
}


/* VertexP3ui: marshalled asynchronously */
struct marshal_cmd_VertexP3ui
{
   struct marshal_cmd_base cmd_base;
   GLenum type;
   GLuint value;
};
void
_mesa_unmarshal_VertexP3ui(struct gl_context *ctx, const struct marshal_cmd_VertexP3ui *cmd)
{
   GLenum type = cmd->type;
   GLuint value = cmd->value;
   CALL_VertexP3ui(ctx->CurrentServerDispatch, (type, value));
}
void GLAPIENTRY
_mesa_marshal_VertexP3ui(GLenum type, GLuint value)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexP3ui);
   struct marshal_cmd_VertexP3ui *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexP3ui, cmd_size);
   cmd->type = type;
   cmd->value = value;
}


/* VertexP4ui: marshalled asynchronously */
struct marshal_cmd_VertexP4ui
{
   struct marshal_cmd_base cmd_base;
   GLenum type;
   GLuint value;
};
void
_mesa_unmarshal_VertexP4ui(struct gl_context *ctx, const struct marshal_cmd_VertexP4ui *cmd)
{
   GLenum type = cmd->type;
   GLuint value = cmd->value;
   CALL_VertexP4ui(ctx->CurrentServerDispatch, (type, value));
}
void GLAPIENTRY
_mesa_marshal_VertexP4ui(GLenum type, GLuint value)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexP4ui);
   struct marshal_cmd_VertexP4ui *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexP4ui, cmd_size);
   cmd->type = type;
   cmd->value = value;
}


/* VertexP2uiv: marshalled asynchronously */
struct marshal_cmd_VertexP2uiv
{
   struct marshal_cmd_base cmd_base;
   GLenum type;
   GLuint value[1];
};
void
_mesa_unmarshal_VertexP2uiv(struct gl_context *ctx, const struct marshal_cmd_VertexP2uiv *cmd)
{
   GLenum type = cmd->type;
   const GLuint * value = cmd->value;
   CALL_VertexP2uiv(ctx->CurrentServerDispatch, (type, value));
}
void GLAPIENTRY
_mesa_marshal_VertexP2uiv(GLenum type, const GLuint * value)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexP2uiv);
   struct marshal_cmd_VertexP2uiv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexP2uiv, cmd_size);
   cmd->type = type;
   memcpy(cmd->value, value, 1 * sizeof(GLuint));
}


/* VertexP3uiv: marshalled asynchronously */
struct marshal_cmd_VertexP3uiv
{
   struct marshal_cmd_base cmd_base;
   GLenum type;
   GLuint value[1];
};
void
_mesa_unmarshal_VertexP3uiv(struct gl_context *ctx, const struct marshal_cmd_VertexP3uiv *cmd)
{
   GLenum type = cmd->type;
   const GLuint * value = cmd->value;
   CALL_VertexP3uiv(ctx->CurrentServerDispatch, (type, value));
}
void GLAPIENTRY
_mesa_marshal_VertexP3uiv(GLenum type, const GLuint * value)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexP3uiv);
   struct marshal_cmd_VertexP3uiv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexP3uiv, cmd_size);
   cmd->type = type;
   memcpy(cmd->value, value, 1 * sizeof(GLuint));
}


/* VertexP4uiv: marshalled asynchronously */
struct marshal_cmd_VertexP4uiv
{
   struct marshal_cmd_base cmd_base;
   GLenum type;
   GLuint value[1];
};
void
_mesa_unmarshal_VertexP4uiv(struct gl_context *ctx, const struct marshal_cmd_VertexP4uiv *cmd)
{
   GLenum type = cmd->type;
   const GLuint * value = cmd->value;
   CALL_VertexP4uiv(ctx->CurrentServerDispatch, (type, value));
}
void GLAPIENTRY
_mesa_marshal_VertexP4uiv(GLenum type, const GLuint * value)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexP4uiv);
   struct marshal_cmd_VertexP4uiv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexP4uiv, cmd_size);
   cmd->type = type;
   memcpy(cmd->value, value, 1 * sizeof(GLuint));
}


/* TexCoordP1ui: marshalled asynchronously */
struct marshal_cmd_TexCoordP1ui
{
   struct marshal_cmd_base cmd_base;
   GLenum type;
   GLuint coords;
};
void
_mesa_unmarshal_TexCoordP1ui(struct gl_context *ctx, const struct marshal_cmd_TexCoordP1ui *cmd)
{
   GLenum type = cmd->type;
   GLuint coords = cmd->coords;
   CALL_TexCoordP1ui(ctx->CurrentServerDispatch, (type, coords));
}
void GLAPIENTRY
_mesa_marshal_TexCoordP1ui(GLenum type, GLuint coords)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TexCoordP1ui);
   struct marshal_cmd_TexCoordP1ui *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexCoordP1ui, cmd_size);
   cmd->type = type;
   cmd->coords = coords;
}


/* TexCoordP2ui: marshalled asynchronously */
struct marshal_cmd_TexCoordP2ui
{
   struct marshal_cmd_base cmd_base;
   GLenum type;
   GLuint coords;
};
void
_mesa_unmarshal_TexCoordP2ui(struct gl_context *ctx, const struct marshal_cmd_TexCoordP2ui *cmd)
{
   GLenum type = cmd->type;
   GLuint coords = cmd->coords;
   CALL_TexCoordP2ui(ctx->CurrentServerDispatch, (type, coords));
}
void GLAPIENTRY
_mesa_marshal_TexCoordP2ui(GLenum type, GLuint coords)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TexCoordP2ui);
   struct marshal_cmd_TexCoordP2ui *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexCoordP2ui, cmd_size);
   cmd->type = type;
   cmd->coords = coords;
}


/* TexCoordP3ui: marshalled asynchronously */
struct marshal_cmd_TexCoordP3ui
{
   struct marshal_cmd_base cmd_base;
   GLenum type;
   GLuint coords;
};
void
_mesa_unmarshal_TexCoordP3ui(struct gl_context *ctx, const struct marshal_cmd_TexCoordP3ui *cmd)
{
   GLenum type = cmd->type;
   GLuint coords = cmd->coords;
   CALL_TexCoordP3ui(ctx->CurrentServerDispatch, (type, coords));
}
void GLAPIENTRY
_mesa_marshal_TexCoordP3ui(GLenum type, GLuint coords)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TexCoordP3ui);
   struct marshal_cmd_TexCoordP3ui *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexCoordP3ui, cmd_size);
   cmd->type = type;
   cmd->coords = coords;
}


/* TexCoordP4ui: marshalled asynchronously */
struct marshal_cmd_TexCoordP4ui
{
   struct marshal_cmd_base cmd_base;
   GLenum type;
   GLuint coords;
};
void
_mesa_unmarshal_TexCoordP4ui(struct gl_context *ctx, const struct marshal_cmd_TexCoordP4ui *cmd)
{
   GLenum type = cmd->type;
   GLuint coords = cmd->coords;
   CALL_TexCoordP4ui(ctx->CurrentServerDispatch, (type, coords));
}
void GLAPIENTRY
_mesa_marshal_TexCoordP4ui(GLenum type, GLuint coords)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TexCoordP4ui);
   struct marshal_cmd_TexCoordP4ui *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexCoordP4ui, cmd_size);
   cmd->type = type;
   cmd->coords = coords;
}


/* TexCoordP1uiv: marshalled asynchronously */
struct marshal_cmd_TexCoordP1uiv
{
   struct marshal_cmd_base cmd_base;
   GLenum type;
   GLuint coords[1];
};
void
_mesa_unmarshal_TexCoordP1uiv(struct gl_context *ctx, const struct marshal_cmd_TexCoordP1uiv *cmd)
{
   GLenum type = cmd->type;
   const GLuint * coords = cmd->coords;
   CALL_TexCoordP1uiv(ctx->CurrentServerDispatch, (type, coords));
}
void GLAPIENTRY
_mesa_marshal_TexCoordP1uiv(GLenum type, const GLuint * coords)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TexCoordP1uiv);
   struct marshal_cmd_TexCoordP1uiv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexCoordP1uiv, cmd_size);
   cmd->type = type;
   memcpy(cmd->coords, coords, 1 * sizeof(GLuint));
}


/* TexCoordP2uiv: marshalled asynchronously */
struct marshal_cmd_TexCoordP2uiv
{
   struct marshal_cmd_base cmd_base;
   GLenum type;
   GLuint coords[1];
};
void
_mesa_unmarshal_TexCoordP2uiv(struct gl_context *ctx, const struct marshal_cmd_TexCoordP2uiv *cmd)
{
   GLenum type = cmd->type;
   const GLuint * coords = cmd->coords;
   CALL_TexCoordP2uiv(ctx->CurrentServerDispatch, (type, coords));
}
void GLAPIENTRY
_mesa_marshal_TexCoordP2uiv(GLenum type, const GLuint * coords)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TexCoordP2uiv);
   struct marshal_cmd_TexCoordP2uiv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexCoordP2uiv, cmd_size);
   cmd->type = type;
   memcpy(cmd->coords, coords, 1 * sizeof(GLuint));
}


/* TexCoordP3uiv: marshalled asynchronously */
struct marshal_cmd_TexCoordP3uiv
{
   struct marshal_cmd_base cmd_base;
   GLenum type;
   GLuint coords[1];
};
void
_mesa_unmarshal_TexCoordP3uiv(struct gl_context *ctx, const struct marshal_cmd_TexCoordP3uiv *cmd)
{
   GLenum type = cmd->type;
   const GLuint * coords = cmd->coords;
   CALL_TexCoordP3uiv(ctx->CurrentServerDispatch, (type, coords));
}
void GLAPIENTRY
_mesa_marshal_TexCoordP3uiv(GLenum type, const GLuint * coords)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TexCoordP3uiv);
   struct marshal_cmd_TexCoordP3uiv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexCoordP3uiv, cmd_size);
   cmd->type = type;
   memcpy(cmd->coords, coords, 1 * sizeof(GLuint));
}


/* TexCoordP4uiv: marshalled asynchronously */
struct marshal_cmd_TexCoordP4uiv
{
   struct marshal_cmd_base cmd_base;
   GLenum type;
   GLuint coords[1];
};
void
_mesa_unmarshal_TexCoordP4uiv(struct gl_context *ctx, const struct marshal_cmd_TexCoordP4uiv *cmd)
{
   GLenum type = cmd->type;
   const GLuint * coords = cmd->coords;
   CALL_TexCoordP4uiv(ctx->CurrentServerDispatch, (type, coords));
}
void GLAPIENTRY
_mesa_marshal_TexCoordP4uiv(GLenum type, const GLuint * coords)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TexCoordP4uiv);
   struct marshal_cmd_TexCoordP4uiv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TexCoordP4uiv, cmd_size);
   cmd->type = type;
   memcpy(cmd->coords, coords, 1 * sizeof(GLuint));
}


/* MultiTexCoordP1ui: marshalled asynchronously */
struct marshal_cmd_MultiTexCoordP1ui
{
   struct marshal_cmd_base cmd_base;
   GLenum texture;
   GLenum type;
   GLuint coords;
};
void
_mesa_unmarshal_MultiTexCoordP1ui(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoordP1ui *cmd)
{
   GLenum texture = cmd->texture;
   GLenum type = cmd->type;
   GLuint coords = cmd->coords;
   CALL_MultiTexCoordP1ui(ctx->CurrentServerDispatch, (texture, type, coords));
}
void GLAPIENTRY
_mesa_marshal_MultiTexCoordP1ui(GLenum texture, GLenum type, GLuint coords)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexCoordP1ui);
   struct marshal_cmd_MultiTexCoordP1ui *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexCoordP1ui, cmd_size);
   cmd->texture = texture;
   cmd->type = type;
   cmd->coords = coords;
}


/* MultiTexCoordP2ui: marshalled asynchronously */
struct marshal_cmd_MultiTexCoordP2ui
{
   struct marshal_cmd_base cmd_base;
   GLenum texture;
   GLenum type;
   GLuint coords;
};
void
_mesa_unmarshal_MultiTexCoordP2ui(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoordP2ui *cmd)
{
   GLenum texture = cmd->texture;
   GLenum type = cmd->type;
   GLuint coords = cmd->coords;
   CALL_MultiTexCoordP2ui(ctx->CurrentServerDispatch, (texture, type, coords));
}
void GLAPIENTRY
_mesa_marshal_MultiTexCoordP2ui(GLenum texture, GLenum type, GLuint coords)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexCoordP2ui);
   struct marshal_cmd_MultiTexCoordP2ui *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexCoordP2ui, cmd_size);
   cmd->texture = texture;
   cmd->type = type;
   cmd->coords = coords;
}


/* MultiTexCoordP3ui: marshalled asynchronously */
struct marshal_cmd_MultiTexCoordP3ui
{
   struct marshal_cmd_base cmd_base;
   GLenum texture;
   GLenum type;
   GLuint coords;
};
void
_mesa_unmarshal_MultiTexCoordP3ui(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoordP3ui *cmd)
{
   GLenum texture = cmd->texture;
   GLenum type = cmd->type;
   GLuint coords = cmd->coords;
   CALL_MultiTexCoordP3ui(ctx->CurrentServerDispatch, (texture, type, coords));
}
void GLAPIENTRY
_mesa_marshal_MultiTexCoordP3ui(GLenum texture, GLenum type, GLuint coords)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexCoordP3ui);
   struct marshal_cmd_MultiTexCoordP3ui *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexCoordP3ui, cmd_size);
   cmd->texture = texture;
   cmd->type = type;
   cmd->coords = coords;
}


/* MultiTexCoordP4ui: marshalled asynchronously */
struct marshal_cmd_MultiTexCoordP4ui
{
   struct marshal_cmd_base cmd_base;
   GLenum texture;
   GLenum type;
   GLuint coords;
};
void
_mesa_unmarshal_MultiTexCoordP4ui(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoordP4ui *cmd)
{
   GLenum texture = cmd->texture;
   GLenum type = cmd->type;
   GLuint coords = cmd->coords;
   CALL_MultiTexCoordP4ui(ctx->CurrentServerDispatch, (texture, type, coords));
}
void GLAPIENTRY
_mesa_marshal_MultiTexCoordP4ui(GLenum texture, GLenum type, GLuint coords)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexCoordP4ui);
   struct marshal_cmd_MultiTexCoordP4ui *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexCoordP4ui, cmd_size);
   cmd->texture = texture;
   cmd->type = type;
   cmd->coords = coords;
}


/* MultiTexCoordP1uiv: marshalled asynchronously */
struct marshal_cmd_MultiTexCoordP1uiv
{
   struct marshal_cmd_base cmd_base;
   GLenum texture;
   GLenum type;
   GLuint coords[1];
};
void
_mesa_unmarshal_MultiTexCoordP1uiv(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoordP1uiv *cmd)
{
   GLenum texture = cmd->texture;
   GLenum type = cmd->type;
   const GLuint * coords = cmd->coords;
   CALL_MultiTexCoordP1uiv(ctx->CurrentServerDispatch, (texture, type, coords));
}
void GLAPIENTRY
_mesa_marshal_MultiTexCoordP1uiv(GLenum texture, GLenum type, const GLuint * coords)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexCoordP1uiv);
   struct marshal_cmd_MultiTexCoordP1uiv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexCoordP1uiv, cmd_size);
   cmd->texture = texture;
   cmd->type = type;
   memcpy(cmd->coords, coords, 1 * sizeof(GLuint));
}


/* MultiTexCoordP2uiv: marshalled asynchronously */
struct marshal_cmd_MultiTexCoordP2uiv
{
   struct marshal_cmd_base cmd_base;
   GLenum texture;
   GLenum type;
   GLuint coords[1];
};
void
_mesa_unmarshal_MultiTexCoordP2uiv(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoordP2uiv *cmd)
{
   GLenum texture = cmd->texture;
   GLenum type = cmd->type;
   const GLuint * coords = cmd->coords;
   CALL_MultiTexCoordP2uiv(ctx->CurrentServerDispatch, (texture, type, coords));
}
void GLAPIENTRY
_mesa_marshal_MultiTexCoordP2uiv(GLenum texture, GLenum type, const GLuint * coords)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexCoordP2uiv);
   struct marshal_cmd_MultiTexCoordP2uiv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexCoordP2uiv, cmd_size);
   cmd->texture = texture;
   cmd->type = type;
   memcpy(cmd->coords, coords, 1 * sizeof(GLuint));
}


/* MultiTexCoordP3uiv: marshalled asynchronously */
struct marshal_cmd_MultiTexCoordP3uiv
{
   struct marshal_cmd_base cmd_base;
   GLenum texture;
   GLenum type;
   GLuint coords[1];
};
void
_mesa_unmarshal_MultiTexCoordP3uiv(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoordP3uiv *cmd)
{
   GLenum texture = cmd->texture;
   GLenum type = cmd->type;
   const GLuint * coords = cmd->coords;
   CALL_MultiTexCoordP3uiv(ctx->CurrentServerDispatch, (texture, type, coords));
}
void GLAPIENTRY
_mesa_marshal_MultiTexCoordP3uiv(GLenum texture, GLenum type, const GLuint * coords)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexCoordP3uiv);
   struct marshal_cmd_MultiTexCoordP3uiv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexCoordP3uiv, cmd_size);
   cmd->texture = texture;
   cmd->type = type;
   memcpy(cmd->coords, coords, 1 * sizeof(GLuint));
}


/* MultiTexCoordP4uiv: marshalled asynchronously */
struct marshal_cmd_MultiTexCoordP4uiv
{
   struct marshal_cmd_base cmd_base;
   GLenum texture;
   GLenum type;
   GLuint coords[1];
};
void
_mesa_unmarshal_MultiTexCoordP4uiv(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoordP4uiv *cmd)
{
   GLenum texture = cmd->texture;
   GLenum type = cmd->type;
   const GLuint * coords = cmd->coords;
   CALL_MultiTexCoordP4uiv(ctx->CurrentServerDispatch, (texture, type, coords));
}
void GLAPIENTRY
_mesa_marshal_MultiTexCoordP4uiv(GLenum texture, GLenum type, const GLuint * coords)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexCoordP4uiv);
   struct marshal_cmd_MultiTexCoordP4uiv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexCoordP4uiv, cmd_size);
   cmd->texture = texture;
   cmd->type = type;
   memcpy(cmd->coords, coords, 1 * sizeof(GLuint));
}


/* NormalP3ui: marshalled asynchronously */
struct marshal_cmd_NormalP3ui
{
   struct marshal_cmd_base cmd_base;
   GLenum type;
   GLuint coords;
};
void
_mesa_unmarshal_NormalP3ui(struct gl_context *ctx, const struct marshal_cmd_NormalP3ui *cmd)
{
   GLenum type = cmd->type;
   GLuint coords = cmd->coords;
   CALL_NormalP3ui(ctx->CurrentServerDispatch, (type, coords));
}
void GLAPIENTRY
_mesa_marshal_NormalP3ui(GLenum type, GLuint coords)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_NormalP3ui);
   struct marshal_cmd_NormalP3ui *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_NormalP3ui, cmd_size);
   cmd->type = type;
   cmd->coords = coords;
}


/* NormalP3uiv: marshalled asynchronously */
struct marshal_cmd_NormalP3uiv
{
   struct marshal_cmd_base cmd_base;
   GLenum type;
   GLuint coords[1];
};
void
_mesa_unmarshal_NormalP3uiv(struct gl_context *ctx, const struct marshal_cmd_NormalP3uiv *cmd)
{
   GLenum type = cmd->type;
   const GLuint * coords = cmd->coords;
   CALL_NormalP3uiv(ctx->CurrentServerDispatch, (type, coords));
}
void GLAPIENTRY
_mesa_marshal_NormalP3uiv(GLenum type, const GLuint * coords)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_NormalP3uiv);
   struct marshal_cmd_NormalP3uiv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_NormalP3uiv, cmd_size);
   cmd->type = type;
   memcpy(cmd->coords, coords, 1 * sizeof(GLuint));
}


/* ColorP3ui: marshalled asynchronously */
struct marshal_cmd_ColorP3ui
{
   struct marshal_cmd_base cmd_base;
   GLenum type;
   GLuint color;
};
void
_mesa_unmarshal_ColorP3ui(struct gl_context *ctx, const struct marshal_cmd_ColorP3ui *cmd)
{
   GLenum type = cmd->type;
   GLuint color = cmd->color;
   CALL_ColorP3ui(ctx->CurrentServerDispatch, (type, color));
}
void GLAPIENTRY
_mesa_marshal_ColorP3ui(GLenum type, GLuint color)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ColorP3ui);
   struct marshal_cmd_ColorP3ui *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ColorP3ui, cmd_size);
   cmd->type = type;
   cmd->color = color;
}


/* ColorP4ui: marshalled asynchronously */
struct marshal_cmd_ColorP4ui
{
   struct marshal_cmd_base cmd_base;
   GLenum type;
   GLuint color;
};
void
_mesa_unmarshal_ColorP4ui(struct gl_context *ctx, const struct marshal_cmd_ColorP4ui *cmd)
{
   GLenum type = cmd->type;
   GLuint color = cmd->color;
   CALL_ColorP4ui(ctx->CurrentServerDispatch, (type, color));
}
void GLAPIENTRY
_mesa_marshal_ColorP4ui(GLenum type, GLuint color)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ColorP4ui);
   struct marshal_cmd_ColorP4ui *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ColorP4ui, cmd_size);
   cmd->type = type;
   cmd->color = color;
}


/* ColorP3uiv: marshalled asynchronously */
struct marshal_cmd_ColorP3uiv
{
   struct marshal_cmd_base cmd_base;
   GLenum type;
   GLuint color[1];
};
void
_mesa_unmarshal_ColorP3uiv(struct gl_context *ctx, const struct marshal_cmd_ColorP3uiv *cmd)
{
   GLenum type = cmd->type;
   const GLuint * color = cmd->color;
   CALL_ColorP3uiv(ctx->CurrentServerDispatch, (type, color));
}
void GLAPIENTRY
_mesa_marshal_ColorP3uiv(GLenum type, const GLuint * color)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ColorP3uiv);
   struct marshal_cmd_ColorP3uiv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ColorP3uiv, cmd_size);
   cmd->type = type;
   memcpy(cmd->color, color, 1 * sizeof(GLuint));
}


/* ColorP4uiv: marshalled asynchronously */
struct marshal_cmd_ColorP4uiv
{
   struct marshal_cmd_base cmd_base;
   GLenum type;
   GLuint color[1];
};
void
_mesa_unmarshal_ColorP4uiv(struct gl_context *ctx, const struct marshal_cmd_ColorP4uiv *cmd)
{
   GLenum type = cmd->type;
   const GLuint * color = cmd->color;
   CALL_ColorP4uiv(ctx->CurrentServerDispatch, (type, color));
}
void GLAPIENTRY
_mesa_marshal_ColorP4uiv(GLenum type, const GLuint * color)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ColorP4uiv);
   struct marshal_cmd_ColorP4uiv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ColorP4uiv, cmd_size);
   cmd->type = type;
   memcpy(cmd->color, color, 1 * sizeof(GLuint));
}


/* SecondaryColorP3ui: marshalled asynchronously */
struct marshal_cmd_SecondaryColorP3ui
{
   struct marshal_cmd_base cmd_base;
   GLenum type;
   GLuint color;
};
void
_mesa_unmarshal_SecondaryColorP3ui(struct gl_context *ctx, const struct marshal_cmd_SecondaryColorP3ui *cmd)
{
   GLenum type = cmd->type;
   GLuint color = cmd->color;
   CALL_SecondaryColorP3ui(ctx->CurrentServerDispatch, (type, color));
}
void GLAPIENTRY
_mesa_marshal_SecondaryColorP3ui(GLenum type, GLuint color)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_SecondaryColorP3ui);
   struct marshal_cmd_SecondaryColorP3ui *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_SecondaryColorP3ui, cmd_size);
   cmd->type = type;
   cmd->color = color;
}


/* SecondaryColorP3uiv: marshalled asynchronously */
struct marshal_cmd_SecondaryColorP3uiv
{
   struct marshal_cmd_base cmd_base;
   GLenum type;
   GLuint color[1];
};
void
_mesa_unmarshal_SecondaryColorP3uiv(struct gl_context *ctx, const struct marshal_cmd_SecondaryColorP3uiv *cmd)
{
   GLenum type = cmd->type;
   const GLuint * color = cmd->color;
   CALL_SecondaryColorP3uiv(ctx->CurrentServerDispatch, (type, color));
}
void GLAPIENTRY
_mesa_marshal_SecondaryColorP3uiv(GLenum type, const GLuint * color)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_SecondaryColorP3uiv);
   struct marshal_cmd_SecondaryColorP3uiv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_SecondaryColorP3uiv, cmd_size);
   cmd->type = type;
   memcpy(cmd->color, color, 1 * sizeof(GLuint));
}


/* VertexAttribP1ui: marshalled asynchronously */
struct marshal_cmd_VertexAttribP1ui
{
   struct marshal_cmd_base cmd_base;
   GLboolean normalized;
   GLuint index;
   GLenum type;
   GLuint value;
};
void
_mesa_unmarshal_VertexAttribP1ui(struct gl_context *ctx, const struct marshal_cmd_VertexAttribP1ui *cmd)
{
   GLuint index = cmd->index;
   GLenum type = cmd->type;
   GLboolean normalized = cmd->normalized;
   GLuint value = cmd->value;
   CALL_VertexAttribP1ui(ctx->CurrentServerDispatch, (index, type, normalized, value));
}
void GLAPIENTRY
_mesa_marshal_VertexAttribP1ui(GLuint index, GLenum type, GLboolean normalized, GLuint value)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttribP1ui);
   struct marshal_cmd_VertexAttribP1ui *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttribP1ui, cmd_size);
   cmd->index = index;
   cmd->type = type;
   cmd->normalized = normalized;
   cmd->value = value;
}


/* VertexAttribP2ui: marshalled asynchronously */
struct marshal_cmd_VertexAttribP2ui
{
   struct marshal_cmd_base cmd_base;
   GLboolean normalized;
   GLuint index;
   GLenum type;
   GLuint value;
};
void
_mesa_unmarshal_VertexAttribP2ui(struct gl_context *ctx, const struct marshal_cmd_VertexAttribP2ui *cmd)
{
   GLuint index = cmd->index;
   GLenum type = cmd->type;
   GLboolean normalized = cmd->normalized;
   GLuint value = cmd->value;
   CALL_VertexAttribP2ui(ctx->CurrentServerDispatch, (index, type, normalized, value));
}
void GLAPIENTRY
_mesa_marshal_VertexAttribP2ui(GLuint index, GLenum type, GLboolean normalized, GLuint value)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttribP2ui);
   struct marshal_cmd_VertexAttribP2ui *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttribP2ui, cmd_size);
   cmd->index = index;
   cmd->type = type;
   cmd->normalized = normalized;
   cmd->value = value;
}


/* VertexAttribP3ui: marshalled asynchronously */
struct marshal_cmd_VertexAttribP3ui
{
   struct marshal_cmd_base cmd_base;
   GLboolean normalized;
   GLuint index;
   GLenum type;
   GLuint value;
};
void
_mesa_unmarshal_VertexAttribP3ui(struct gl_context *ctx, const struct marshal_cmd_VertexAttribP3ui *cmd)
{
   GLuint index = cmd->index;
   GLenum type = cmd->type;
   GLboolean normalized = cmd->normalized;
   GLuint value = cmd->value;
   CALL_VertexAttribP3ui(ctx->CurrentServerDispatch, (index, type, normalized, value));
}
void GLAPIENTRY
_mesa_marshal_VertexAttribP3ui(GLuint index, GLenum type, GLboolean normalized, GLuint value)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttribP3ui);
   struct marshal_cmd_VertexAttribP3ui *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttribP3ui, cmd_size);
   cmd->index = index;
   cmd->type = type;
   cmd->normalized = normalized;
   cmd->value = value;
}


/* VertexAttribP4ui: marshalled asynchronously */
struct marshal_cmd_VertexAttribP4ui
{
   struct marshal_cmd_base cmd_base;
   GLboolean normalized;
   GLuint index;
   GLenum type;
   GLuint value;
};
void
_mesa_unmarshal_VertexAttribP4ui(struct gl_context *ctx, const struct marshal_cmd_VertexAttribP4ui *cmd)
{
   GLuint index = cmd->index;
   GLenum type = cmd->type;
   GLboolean normalized = cmd->normalized;
   GLuint value = cmd->value;
   CALL_VertexAttribP4ui(ctx->CurrentServerDispatch, (index, type, normalized, value));
}
void GLAPIENTRY
_mesa_marshal_VertexAttribP4ui(GLuint index, GLenum type, GLboolean normalized, GLuint value)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttribP4ui);
   struct marshal_cmd_VertexAttribP4ui *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttribP4ui, cmd_size);
   cmd->index = index;
   cmd->type = type;
   cmd->normalized = normalized;
   cmd->value = value;
}


/* VertexAttribP1uiv: marshalled asynchronously */
struct marshal_cmd_VertexAttribP1uiv
{
   struct marshal_cmd_base cmd_base;
   GLboolean normalized;
   GLuint index;
   GLenum type;
   GLuint value[1];
};
void
_mesa_unmarshal_VertexAttribP1uiv(struct gl_context *ctx, const struct marshal_cmd_VertexAttribP1uiv *cmd)
{
   GLuint index = cmd->index;
   GLenum type = cmd->type;
   GLboolean normalized = cmd->normalized;
   const GLuint * value = cmd->value;
   CALL_VertexAttribP1uiv(ctx->CurrentServerDispatch, (index, type, normalized, value));
}
void GLAPIENTRY
_mesa_marshal_VertexAttribP1uiv(GLuint index, GLenum type, GLboolean normalized, const GLuint * value)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttribP1uiv);
   struct marshal_cmd_VertexAttribP1uiv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttribP1uiv, cmd_size);
   cmd->index = index;
   cmd->type = type;
   cmd->normalized = normalized;
   memcpy(cmd->value, value, 1 * sizeof(GLuint));
}


/* VertexAttribP2uiv: marshalled asynchronously */
struct marshal_cmd_VertexAttribP2uiv
{
   struct marshal_cmd_base cmd_base;
   GLboolean normalized;
   GLuint index;
   GLenum type;
   GLuint value[1];
};
void
_mesa_unmarshal_VertexAttribP2uiv(struct gl_context *ctx, const struct marshal_cmd_VertexAttribP2uiv *cmd)
{
   GLuint index = cmd->index;
   GLenum type = cmd->type;
   GLboolean normalized = cmd->normalized;
   const GLuint * value = cmd->value;
   CALL_VertexAttribP2uiv(ctx->CurrentServerDispatch, (index, type, normalized, value));
}
void GLAPIENTRY
_mesa_marshal_VertexAttribP2uiv(GLuint index, GLenum type, GLboolean normalized, const GLuint * value)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttribP2uiv);
   struct marshal_cmd_VertexAttribP2uiv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttribP2uiv, cmd_size);
   cmd->index = index;
   cmd->type = type;
   cmd->normalized = normalized;
   memcpy(cmd->value, value, 1 * sizeof(GLuint));
}


/* VertexAttribP3uiv: marshalled asynchronously */
struct marshal_cmd_VertexAttribP3uiv
{
   struct marshal_cmd_base cmd_base;
   GLboolean normalized;
   GLuint index;
   GLenum type;
   GLuint value[1];
};
void
_mesa_unmarshal_VertexAttribP3uiv(struct gl_context *ctx, const struct marshal_cmd_VertexAttribP3uiv *cmd)
{
   GLuint index = cmd->index;
   GLenum type = cmd->type;
   GLboolean normalized = cmd->normalized;
   const GLuint * value = cmd->value;
   CALL_VertexAttribP3uiv(ctx->CurrentServerDispatch, (index, type, normalized, value));
}
void GLAPIENTRY
_mesa_marshal_VertexAttribP3uiv(GLuint index, GLenum type, GLboolean normalized, const GLuint * value)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttribP3uiv);
   struct marshal_cmd_VertexAttribP3uiv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttribP3uiv, cmd_size);
   cmd->index = index;
   cmd->type = type;
   cmd->normalized = normalized;
   memcpy(cmd->value, value, 1 * sizeof(GLuint));
}


/* VertexAttribP4uiv: marshalled asynchronously */
struct marshal_cmd_VertexAttribP4uiv
{
   struct marshal_cmd_base cmd_base;
   GLboolean normalized;
   GLuint index;
   GLenum type;
   GLuint value[1];
};
void
_mesa_unmarshal_VertexAttribP4uiv(struct gl_context *ctx, const struct marshal_cmd_VertexAttribP4uiv *cmd)
{
   GLuint index = cmd->index;
   GLenum type = cmd->type;
   GLboolean normalized = cmd->normalized;
   const GLuint * value = cmd->value;
   CALL_VertexAttribP4uiv(ctx->CurrentServerDispatch, (index, type, normalized, value));
}
void GLAPIENTRY
_mesa_marshal_VertexAttribP4uiv(GLuint index, GLenum type, GLboolean normalized, const GLuint * value)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttribP4uiv);
   struct marshal_cmd_VertexAttribP4uiv *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttribP4uiv, cmd_size);
   cmd->index = index;
   cmd->type = type;
   cmd->normalized = normalized;
   memcpy(cmd->value, value, 1 * sizeof(GLuint));
}


/* GetSubroutineUniformLocation: marshalled synchronously */
GLint GLAPIENTRY
_mesa_marshal_GetSubroutineUniformLocation(GLuint program, GLenum shadertype, const GLchar * name)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetSubroutineUniformLocation");
   return CALL_GetSubroutineUniformLocation(ctx->CurrentServerDispatch, (program, shadertype, name));
}


/* GetSubroutineIndex: marshalled synchronously */
GLuint GLAPIENTRY
_mesa_marshal_GetSubroutineIndex(GLuint program, GLenum shadertype, const GLchar * name)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetSubroutineIndex");
   return CALL_GetSubroutineIndex(ctx->CurrentServerDispatch, (program, shadertype, name));
}


/* GetActiveSubroutineUniformiv: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetActiveSubroutineUniformiv(GLuint program, GLenum shadertype, GLuint index, GLenum pname, GLint * values)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetActiveSubroutineUniformiv");
   CALL_GetActiveSubroutineUniformiv(ctx->CurrentServerDispatch, (program, shadertype, index, pname, values));
}


/* GetActiveSubroutineUniformName: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetActiveSubroutineUniformName(GLuint program, GLenum shadertype, GLuint index, GLsizei bufsize, GLsizei * length, GLchar * name)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetActiveSubroutineUniformName");
   CALL_GetActiveSubroutineUniformName(ctx->CurrentServerDispatch, (program, shadertype, index, bufsize, length, name));
}


/* GetActiveSubroutineName: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetActiveSubroutineName(GLuint program, GLenum shadertype, GLuint index, GLsizei bufsize, GLsizei * length, GLchar * name)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetActiveSubroutineName");
   CALL_GetActiveSubroutineName(ctx->CurrentServerDispatch, (program, shadertype, index, bufsize, length, name));
}


/* UniformSubroutinesuiv: marshalled asynchronously */
struct marshal_cmd_UniformSubroutinesuiv
{
   struct marshal_cmd_base cmd_base;
   GLenum shadertype;
   GLsizei count;
   /* Next safe_mul(count, 1 * sizeof(GLuint)) bytes are GLuint indices[count] */
};
void
_mesa_unmarshal_UniformSubroutinesuiv(struct gl_context *ctx, const struct marshal_cmd_UniformSubroutinesuiv *cmd)
{
   GLenum shadertype = cmd->shadertype;
   GLsizei count = cmd->count;
   GLuint * indices;
   const char *variable_data = (const char *) (cmd + 1);
   indices = (GLuint *) variable_data;
   CALL_UniformSubroutinesuiv(ctx->CurrentServerDispatch, (shadertype, count, indices));
}
void GLAPIENTRY
_mesa_marshal_UniformSubroutinesuiv(GLenum shadertype, GLsizei count, const GLuint * indices)
{
   GET_CURRENT_CONTEXT(ctx);
   int indices_size = safe_mul(count, 1 * sizeof(GLuint));
   int cmd_size = sizeof(struct marshal_cmd_UniformSubroutinesuiv) + indices_size;
   struct marshal_cmd_UniformSubroutinesuiv *cmd;
   if (unlikely(indices_size < 0 || (indices_size > 0 && !indices) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "UniformSubroutinesuiv");
      CALL_UniformSubroutinesuiv(ctx->CurrentServerDispatch, (shadertype, count, indices));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_UniformSubroutinesuiv, cmd_size);
   cmd->shadertype = shadertype;
   cmd->count = count;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, indices, indices_size);
}


/* GetUniformSubroutineuiv: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetUniformSubroutineuiv(GLenum shadertype, GLint location, GLuint * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetUniformSubroutineuiv");
   CALL_GetUniformSubroutineuiv(ctx->CurrentServerDispatch, (shadertype, location, params));
}


/* GetProgramStageiv: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetProgramStageiv(GLuint program, GLenum shadertype, GLenum pname, GLint * values)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetProgramStageiv");
   CALL_GetProgramStageiv(ctx->CurrentServerDispatch, (program, shadertype, pname, values));
}


/* PatchParameteri: marshalled asynchronously */
struct marshal_cmd_PatchParameteri
{
   struct marshal_cmd_base cmd_base;
   GLenum pname;
   GLint value;
};
void
_mesa_unmarshal_PatchParameteri(struct gl_context *ctx, const struct marshal_cmd_PatchParameteri *cmd)
{
   GLenum pname = cmd->pname;
   GLint value = cmd->value;
   CALL_PatchParameteri(ctx->CurrentServerDispatch, (pname, value));
}
void GLAPIENTRY
_mesa_marshal_PatchParameteri(GLenum pname, GLint value)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_PatchParameteri);
   struct marshal_cmd_PatchParameteri *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_PatchParameteri, cmd_size);
   cmd->pname = pname;
   cmd->value = value;
}


/* PatchParameterfv: marshalled asynchronously */
struct marshal_cmd_PatchParameterfv
{
   struct marshal_cmd_base cmd_base;
   GLenum pname;
   /* Next safe_mul(_mesa_patch_param_enum_to_count(pname), 1 * sizeof(GLfloat)) bytes are GLfloat values[_mesa_patch_param_enum_to_count(pname)] */
};
void
_mesa_unmarshal_PatchParameterfv(struct gl_context *ctx, const struct marshal_cmd_PatchParameterfv *cmd)
{
   GLenum pname = cmd->pname;
   GLfloat * values;
   const char *variable_data = (const char *) (cmd + 1);
   values = (GLfloat *) variable_data;
   CALL_PatchParameterfv(ctx->CurrentServerDispatch, (pname, values));
}
void GLAPIENTRY
_mesa_marshal_PatchParameterfv(GLenum pname, const GLfloat * values)
{
   GET_CURRENT_CONTEXT(ctx);
   int values_size = safe_mul(_mesa_patch_param_enum_to_count(pname), 1 * sizeof(GLfloat));
   int cmd_size = sizeof(struct marshal_cmd_PatchParameterfv) + values_size;
   struct marshal_cmd_PatchParameterfv *cmd;
   if (unlikely(values_size < 0 || (values_size > 0 && !values) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "PatchParameterfv");
      CALL_PatchParameterfv(ctx->CurrentServerDispatch, (pname, values));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_PatchParameterfv, cmd_size);
   cmd->pname = pname;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, values, values_size);
}


/* DrawArraysIndirect: marshalled asynchronously */
struct marshal_cmd_DrawArraysIndirect
{
   struct marshal_cmd_base cmd_base;
   GLenum mode;
   const GLvoid * indirect;
};
void
_mesa_unmarshal_DrawArraysIndirect(struct gl_context *ctx, const struct marshal_cmd_DrawArraysIndirect *cmd)
{
   GLenum mode = cmd->mode;
   const GLvoid * indirect = cmd->indirect;
   CALL_DrawArraysIndirect(ctx->CurrentServerDispatch, (mode, indirect));
}
void GLAPIENTRY
_mesa_marshal_DrawArraysIndirect(GLenum mode, const GLvoid * indirect)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_DrawArraysIndirect);
   struct marshal_cmd_DrawArraysIndirect *cmd;
   if (_mesa_glthread_has_non_vbo_vertices_or_indirect(ctx)) {
      _mesa_glthread_finish_before(ctx, "DrawArraysIndirect");
      CALL_DrawArraysIndirect(ctx->CurrentServerDispatch, (mode, indirect));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_DrawArraysIndirect, cmd_size);
   cmd->mode = mode;
   cmd->indirect = indirect;
}


/* DrawElementsIndirect: marshalled asynchronously */
struct marshal_cmd_DrawElementsIndirect
{
   struct marshal_cmd_base cmd_base;
   GLenum mode;
   GLenum type;
   const GLvoid * indirect;
};
void
_mesa_unmarshal_DrawElementsIndirect(struct gl_context *ctx, const struct marshal_cmd_DrawElementsIndirect *cmd)
{
   GLenum mode = cmd->mode;
   GLenum type = cmd->type;
   const GLvoid * indirect = cmd->indirect;
   CALL_DrawElementsIndirect(ctx->CurrentServerDispatch, (mode, type, indirect));
}
void GLAPIENTRY
_mesa_marshal_DrawElementsIndirect(GLenum mode, GLenum type, const GLvoid * indirect)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_DrawElementsIndirect);
   struct marshal_cmd_DrawElementsIndirect *cmd;
   if (_mesa_glthread_has_non_vbo_vertices_or_indices_or_indirect(ctx)) {
      _mesa_glthread_finish_before(ctx, "DrawElementsIndirect");
      CALL_DrawElementsIndirect(ctx->CurrentServerDispatch, (mode, type, indirect));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_DrawElementsIndirect, cmd_size);
   cmd->mode = mode;
   cmd->type = type;
   cmd->indirect = indirect;
}


/* MultiDrawArraysIndirect: marshalled asynchronously */
struct marshal_cmd_MultiDrawArraysIndirect
{
   struct marshal_cmd_base cmd_base;
   GLenum mode;
   GLsizei primcount;
   GLsizei stride;
   const GLvoid * indirect;
};
void
_mesa_unmarshal_MultiDrawArraysIndirect(struct gl_context *ctx, const struct marshal_cmd_MultiDrawArraysIndirect *cmd)
{
   GLenum mode = cmd->mode;
   const GLvoid * indirect = cmd->indirect;
   GLsizei primcount = cmd->primcount;
   GLsizei stride = cmd->stride;
   CALL_MultiDrawArraysIndirect(ctx->CurrentServerDispatch, (mode, indirect, primcount, stride));
}
void GLAPIENTRY
_mesa_marshal_MultiDrawArraysIndirect(GLenum mode, const GLvoid * indirect, GLsizei primcount, GLsizei stride)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiDrawArraysIndirect);
   struct marshal_cmd_MultiDrawArraysIndirect *cmd;
   if (_mesa_glthread_has_non_vbo_vertices_or_indirect(ctx)) {
      _mesa_glthread_finish_before(ctx, "MultiDrawArraysIndirect");
      CALL_MultiDrawArraysIndirect(ctx->CurrentServerDispatch, (mode, indirect, primcount, stride));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiDrawArraysIndirect, cmd_size);
   cmd->mode = mode;
   cmd->indirect = indirect;
   cmd->primcount = primcount;
   cmd->stride = stride;
}


/* MultiDrawElementsIndirect: marshalled asynchronously */
struct marshal_cmd_MultiDrawElementsIndirect
{
   struct marshal_cmd_base cmd_base;
   GLenum mode;
   GLenum type;
   GLsizei primcount;
   GLsizei stride;
   const GLvoid * indirect;
};
void
_mesa_unmarshal_MultiDrawElementsIndirect(struct gl_context *ctx, const struct marshal_cmd_MultiDrawElementsIndirect *cmd)
{
   GLenum mode = cmd->mode;
   GLenum type = cmd->type;
   const GLvoid * indirect = cmd->indirect;
   GLsizei primcount = cmd->primcount;
   GLsizei stride = cmd->stride;
   CALL_MultiDrawElementsIndirect(ctx->CurrentServerDispatch, (mode, type, indirect, primcount, stride));
}
void GLAPIENTRY
_mesa_marshal_MultiDrawElementsIndirect(GLenum mode, GLenum type, const GLvoid * indirect, GLsizei primcount, GLsizei stride)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiDrawElementsIndirect);
   struct marshal_cmd_MultiDrawElementsIndirect *cmd;
   if (_mesa_glthread_has_non_vbo_vertices_or_indices_or_indirect(ctx)) {
      _mesa_glthread_finish_before(ctx, "MultiDrawElementsIndirect");
      CALL_MultiDrawElementsIndirect(ctx->CurrentServerDispatch, (mode, type, indirect, primcount, stride));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiDrawElementsIndirect, cmd_size);
   cmd->mode = mode;
   cmd->type = type;
   cmd->indirect = indirect;
   cmd->primcount = primcount;
   cmd->stride = stride;
}


/* Uniform1d: marshalled asynchronously */
struct marshal_cmd_Uniform1d
{
   struct marshal_cmd_base cmd_base;
   GLint location;
   GLdouble x;
};
void
_mesa_unmarshal_Uniform1d(struct gl_context *ctx, const struct marshal_cmd_Uniform1d *cmd)
{
   GLint location = cmd->location;
   GLdouble x = cmd->x;
   CALL_Uniform1d(ctx->CurrentServerDispatch, (location, x));
}
void GLAPIENTRY
_mesa_marshal_Uniform1d(GLint location, GLdouble x)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Uniform1d);
   struct marshal_cmd_Uniform1d *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Uniform1d, cmd_size);
   cmd->location = location;
   cmd->x = x;
}


/* Uniform2d: marshalled asynchronously */
struct marshal_cmd_Uniform2d
{
   struct marshal_cmd_base cmd_base;
   GLint location;
   GLdouble x;
   GLdouble y;
};
void
_mesa_unmarshal_Uniform2d(struct gl_context *ctx, const struct marshal_cmd_Uniform2d *cmd)
{
   GLint location = cmd->location;
   GLdouble x = cmd->x;
   GLdouble y = cmd->y;
   CALL_Uniform2d(ctx->CurrentServerDispatch, (location, x, y));
}
void GLAPIENTRY
_mesa_marshal_Uniform2d(GLint location, GLdouble x, GLdouble y)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Uniform2d);
   struct marshal_cmd_Uniform2d *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Uniform2d, cmd_size);
   cmd->location = location;
   cmd->x = x;
   cmd->y = y;
}


/* Uniform3d: marshalled asynchronously */
struct marshal_cmd_Uniform3d
{
   struct marshal_cmd_base cmd_base;
   GLint location;
   GLdouble x;
   GLdouble y;
   GLdouble z;
};
void
_mesa_unmarshal_Uniform3d(struct gl_context *ctx, const struct marshal_cmd_Uniform3d *cmd)
{
   GLint location = cmd->location;
   GLdouble x = cmd->x;
   GLdouble y = cmd->y;
   GLdouble z = cmd->z;
   CALL_Uniform3d(ctx->CurrentServerDispatch, (location, x, y, z));
}
void GLAPIENTRY
_mesa_marshal_Uniform3d(GLint location, GLdouble x, GLdouble y, GLdouble z)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Uniform3d);
   struct marshal_cmd_Uniform3d *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Uniform3d, cmd_size);
   cmd->location = location;
   cmd->x = x;
   cmd->y = y;
   cmd->z = z;
}


/* Uniform4d: marshalled asynchronously */
struct marshal_cmd_Uniform4d
{
   struct marshal_cmd_base cmd_base;
   GLint location;
   GLdouble x;
   GLdouble y;
   GLdouble z;
   GLdouble w;
};
void
_mesa_unmarshal_Uniform4d(struct gl_context *ctx, const struct marshal_cmd_Uniform4d *cmd)
{
   GLint location = cmd->location;
   GLdouble x = cmd->x;
   GLdouble y = cmd->y;
   GLdouble z = cmd->z;
   GLdouble w = cmd->w;
   CALL_Uniform4d(ctx->CurrentServerDispatch, (location, x, y, z, w));
}
void GLAPIENTRY
_mesa_marshal_Uniform4d(GLint location, GLdouble x, GLdouble y, GLdouble z, GLdouble w)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Uniform4d);
   struct marshal_cmd_Uniform4d *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Uniform4d, cmd_size);
   cmd->location = location;
   cmd->x = x;
   cmd->y = y;
   cmd->z = z;
   cmd->w = w;
}


/* Uniform1dv: marshalled asynchronously */
struct marshal_cmd_Uniform1dv
{
   struct marshal_cmd_base cmd_base;
   GLint location;
   GLsizei count;
   /* Next safe_mul(count, 1 * sizeof(GLdouble)) bytes are GLdouble value[count] */
};
void
_mesa_unmarshal_Uniform1dv(struct gl_context *ctx, const struct marshal_cmd_Uniform1dv *cmd)
{
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLdouble * value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLdouble *) variable_data;
   CALL_Uniform1dv(ctx->CurrentServerDispatch, (location, count, value));
}
void GLAPIENTRY
_mesa_marshal_Uniform1dv(GLint location, GLsizei count, const GLdouble * value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 1 * sizeof(GLdouble));
   int cmd_size = sizeof(struct marshal_cmd_Uniform1dv) + value_size;
   struct marshal_cmd_Uniform1dv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "Uniform1dv");
      CALL_Uniform1dv(ctx->CurrentServerDispatch, (location, count, value));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Uniform1dv, cmd_size);
   cmd->location = location;
   cmd->count = count;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, value, value_size);
}


/* Uniform2dv: marshalled asynchronously */
struct marshal_cmd_Uniform2dv
{
   struct marshal_cmd_base cmd_base;
   GLint location;
   GLsizei count;
   /* Next safe_mul(count, 2 * sizeof(GLdouble)) bytes are GLdouble value[count][2] */
};
void
_mesa_unmarshal_Uniform2dv(struct gl_context *ctx, const struct marshal_cmd_Uniform2dv *cmd)
{
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLdouble * value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLdouble *) variable_data;
   CALL_Uniform2dv(ctx->CurrentServerDispatch, (location, count, value));
}
void GLAPIENTRY
_mesa_marshal_Uniform2dv(GLint location, GLsizei count, const GLdouble * value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 2 * sizeof(GLdouble));
   int cmd_size = sizeof(struct marshal_cmd_Uniform2dv) + value_size;
   struct marshal_cmd_Uniform2dv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "Uniform2dv");
      CALL_Uniform2dv(ctx->CurrentServerDispatch, (location, count, value));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Uniform2dv, cmd_size);
   cmd->location = location;
   cmd->count = count;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, value, value_size);
}


/* Uniform3dv: marshalled asynchronously */
struct marshal_cmd_Uniform3dv
{
   struct marshal_cmd_base cmd_base;
   GLint location;
   GLsizei count;
   /* Next safe_mul(count, 3 * sizeof(GLdouble)) bytes are GLdouble value[count][3] */
};
void
_mesa_unmarshal_Uniform3dv(struct gl_context *ctx, const struct marshal_cmd_Uniform3dv *cmd)
{
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLdouble * value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLdouble *) variable_data;
   CALL_Uniform3dv(ctx->CurrentServerDispatch, (location, count, value));
}
void GLAPIENTRY
_mesa_marshal_Uniform3dv(GLint location, GLsizei count, const GLdouble * value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 3 * sizeof(GLdouble));
   int cmd_size = sizeof(struct marshal_cmd_Uniform3dv) + value_size;
   struct marshal_cmd_Uniform3dv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "Uniform3dv");
      CALL_Uniform3dv(ctx->CurrentServerDispatch, (location, count, value));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Uniform3dv, cmd_size);
   cmd->location = location;
   cmd->count = count;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, value, value_size);
}


/* Uniform4dv: marshalled asynchronously */
struct marshal_cmd_Uniform4dv
{
   struct marshal_cmd_base cmd_base;
   GLint location;
   GLsizei count;
   /* Next safe_mul(count, 4 * sizeof(GLdouble)) bytes are GLdouble value[count][4] */
};
void
_mesa_unmarshal_Uniform4dv(struct gl_context *ctx, const struct marshal_cmd_Uniform4dv *cmd)
{
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLdouble * value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLdouble *) variable_data;
   CALL_Uniform4dv(ctx->CurrentServerDispatch, (location, count, value));
}
void GLAPIENTRY
_mesa_marshal_Uniform4dv(GLint location, GLsizei count, const GLdouble * value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 4 * sizeof(GLdouble));
   int cmd_size = sizeof(struct marshal_cmd_Uniform4dv) + value_size;
   struct marshal_cmd_Uniform4dv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "Uniform4dv");
      CALL_Uniform4dv(ctx->CurrentServerDispatch, (location, count, value));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Uniform4dv, cmd_size);
   cmd->location = location;
   cmd->count = count;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, value, value_size);
}


/* UniformMatrix2dv: marshalled asynchronously */
struct marshal_cmd_UniformMatrix2dv
{
   struct marshal_cmd_base cmd_base;
   GLboolean transpose;
   GLint location;
   GLsizei count;
   /* Next safe_mul(count, 4 * sizeof(GLdouble)) bytes are GLdouble value[count][4] */
};
void
_mesa_unmarshal_UniformMatrix2dv(struct gl_context *ctx, const struct marshal_cmd_UniformMatrix2dv *cmd)
{
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLboolean transpose = cmd->transpose;
   GLdouble * value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLdouble *) variable_data;
   CALL_UniformMatrix2dv(ctx->CurrentServerDispatch, (location, count, transpose, value));
}
void GLAPIENTRY
_mesa_marshal_UniformMatrix2dv(GLint location, GLsizei count, GLboolean transpose, const GLdouble * value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 4 * sizeof(GLdouble));
   int cmd_size = sizeof(struct marshal_cmd_UniformMatrix2dv) + value_size;
   struct marshal_cmd_UniformMatrix2dv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "UniformMatrix2dv");
      CALL_UniformMatrix2dv(ctx->CurrentServerDispatch, (location, count, transpose, value));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_UniformMatrix2dv, cmd_size);
   cmd->location = location;
   cmd->count = count;
   cmd->transpose = transpose;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, value, value_size);
}


/* UniformMatrix3dv: marshalled asynchronously */
struct marshal_cmd_UniformMatrix3dv
{
   struct marshal_cmd_base cmd_base;
   GLboolean transpose;
   GLint location;
   GLsizei count;
   /* Next safe_mul(count, 9 * sizeof(GLdouble)) bytes are GLdouble value[count][9] */
};
void
_mesa_unmarshal_UniformMatrix3dv(struct gl_context *ctx, const struct marshal_cmd_UniformMatrix3dv *cmd)
{
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLboolean transpose = cmd->transpose;
   GLdouble * value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLdouble *) variable_data;
   CALL_UniformMatrix3dv(ctx->CurrentServerDispatch, (location, count, transpose, value));
}
void GLAPIENTRY
_mesa_marshal_UniformMatrix3dv(GLint location, GLsizei count, GLboolean transpose, const GLdouble * value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 9 * sizeof(GLdouble));
   int cmd_size = sizeof(struct marshal_cmd_UniformMatrix3dv) + value_size;
   struct marshal_cmd_UniformMatrix3dv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "UniformMatrix3dv");
      CALL_UniformMatrix3dv(ctx->CurrentServerDispatch, (location, count, transpose, value));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_UniformMatrix3dv, cmd_size);
   cmd->location = location;
   cmd->count = count;
   cmd->transpose = transpose;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, value, value_size);
}


/* UniformMatrix4dv: marshalled asynchronously */
struct marshal_cmd_UniformMatrix4dv
{
   struct marshal_cmd_base cmd_base;
   GLboolean transpose;
   GLint location;
   GLsizei count;
   /* Next safe_mul(count, 16 * sizeof(GLdouble)) bytes are GLdouble value[count][16] */
};
void
_mesa_unmarshal_UniformMatrix4dv(struct gl_context *ctx, const struct marshal_cmd_UniformMatrix4dv *cmd)
{
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLboolean transpose = cmd->transpose;
   GLdouble * value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLdouble *) variable_data;
   CALL_UniformMatrix4dv(ctx->CurrentServerDispatch, (location, count, transpose, value));
}
void GLAPIENTRY
_mesa_marshal_UniformMatrix4dv(GLint location, GLsizei count, GLboolean transpose, const GLdouble * value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 16 * sizeof(GLdouble));
   int cmd_size = sizeof(struct marshal_cmd_UniformMatrix4dv) + value_size;
   struct marshal_cmd_UniformMatrix4dv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "UniformMatrix4dv");
      CALL_UniformMatrix4dv(ctx->CurrentServerDispatch, (location, count, transpose, value));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_UniformMatrix4dv, cmd_size);
   cmd->location = location;
   cmd->count = count;
   cmd->transpose = transpose;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, value, value_size);
}


/* UniformMatrix2x3dv: marshalled asynchronously */
struct marshal_cmd_UniformMatrix2x3dv
{
   struct marshal_cmd_base cmd_base;
   GLboolean transpose;
   GLint location;
   GLsizei count;
   /* Next safe_mul(count, 6 * sizeof(GLdouble)) bytes are GLdouble value[count][6] */
};
void
_mesa_unmarshal_UniformMatrix2x3dv(struct gl_context *ctx, const struct marshal_cmd_UniformMatrix2x3dv *cmd)
{
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLboolean transpose = cmd->transpose;
   GLdouble * value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLdouble *) variable_data;
   CALL_UniformMatrix2x3dv(ctx->CurrentServerDispatch, (location, count, transpose, value));
}
void GLAPIENTRY
_mesa_marshal_UniformMatrix2x3dv(GLint location, GLsizei count, GLboolean transpose, const GLdouble * value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 6 * sizeof(GLdouble));
   int cmd_size = sizeof(struct marshal_cmd_UniformMatrix2x3dv) + value_size;
   struct marshal_cmd_UniformMatrix2x3dv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "UniformMatrix2x3dv");
      CALL_UniformMatrix2x3dv(ctx->CurrentServerDispatch, (location, count, transpose, value));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_UniformMatrix2x3dv, cmd_size);
   cmd->location = location;
   cmd->count = count;
   cmd->transpose = transpose;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, value, value_size);
}


/* UniformMatrix2x4dv: marshalled asynchronously */
struct marshal_cmd_UniformMatrix2x4dv
{
   struct marshal_cmd_base cmd_base;
   GLboolean transpose;
   GLint location;
   GLsizei count;
   /* Next safe_mul(count, 8 * sizeof(GLdouble)) bytes are GLdouble value[count][8] */
};
void
_mesa_unmarshal_UniformMatrix2x4dv(struct gl_context *ctx, const struct marshal_cmd_UniformMatrix2x4dv *cmd)
{
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLboolean transpose = cmd->transpose;
   GLdouble * value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLdouble *) variable_data;
   CALL_UniformMatrix2x4dv(ctx->CurrentServerDispatch, (location, count, transpose, value));
}
void GLAPIENTRY
_mesa_marshal_UniformMatrix2x4dv(GLint location, GLsizei count, GLboolean transpose, const GLdouble * value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 8 * sizeof(GLdouble));
   int cmd_size = sizeof(struct marshal_cmd_UniformMatrix2x4dv) + value_size;
   struct marshal_cmd_UniformMatrix2x4dv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "UniformMatrix2x4dv");
      CALL_UniformMatrix2x4dv(ctx->CurrentServerDispatch, (location, count, transpose, value));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_UniformMatrix2x4dv, cmd_size);
   cmd->location = location;
   cmd->count = count;
   cmd->transpose = transpose;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, value, value_size);
}


/* UniformMatrix3x2dv: marshalled asynchronously */
struct marshal_cmd_UniformMatrix3x2dv
{
   struct marshal_cmd_base cmd_base;
   GLboolean transpose;
   GLint location;
   GLsizei count;
   /* Next safe_mul(count, 6 * sizeof(GLdouble)) bytes are GLdouble value[count][6] */
};
void
_mesa_unmarshal_UniformMatrix3x2dv(struct gl_context *ctx, const struct marshal_cmd_UniformMatrix3x2dv *cmd)
{
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLboolean transpose = cmd->transpose;
   GLdouble * value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLdouble *) variable_data;
   CALL_UniformMatrix3x2dv(ctx->CurrentServerDispatch, (location, count, transpose, value));
}
void GLAPIENTRY
_mesa_marshal_UniformMatrix3x2dv(GLint location, GLsizei count, GLboolean transpose, const GLdouble * value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 6 * sizeof(GLdouble));
   int cmd_size = sizeof(struct marshal_cmd_UniformMatrix3x2dv) + value_size;
   struct marshal_cmd_UniformMatrix3x2dv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "UniformMatrix3x2dv");
      CALL_UniformMatrix3x2dv(ctx->CurrentServerDispatch, (location, count, transpose, value));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_UniformMatrix3x2dv, cmd_size);
   cmd->location = location;
   cmd->count = count;
   cmd->transpose = transpose;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, value, value_size);
}


/* UniformMatrix3x4dv: marshalled asynchronously */
struct marshal_cmd_UniformMatrix3x4dv
{
   struct marshal_cmd_base cmd_base;
   GLboolean transpose;
   GLint location;
   GLsizei count;
   /* Next safe_mul(count, 12 * sizeof(GLdouble)) bytes are GLdouble value[count][12] */
};
void
_mesa_unmarshal_UniformMatrix3x4dv(struct gl_context *ctx, const struct marshal_cmd_UniformMatrix3x4dv *cmd)
{
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLboolean transpose = cmd->transpose;
   GLdouble * value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLdouble *) variable_data;
   CALL_UniformMatrix3x4dv(ctx->CurrentServerDispatch, (location, count, transpose, value));
}
void GLAPIENTRY
_mesa_marshal_UniformMatrix3x4dv(GLint location, GLsizei count, GLboolean transpose, const GLdouble * value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 12 * sizeof(GLdouble));
   int cmd_size = sizeof(struct marshal_cmd_UniformMatrix3x4dv) + value_size;
   struct marshal_cmd_UniformMatrix3x4dv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "UniformMatrix3x4dv");
      CALL_UniformMatrix3x4dv(ctx->CurrentServerDispatch, (location, count, transpose, value));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_UniformMatrix3x4dv, cmd_size);
   cmd->location = location;
   cmd->count = count;
   cmd->transpose = transpose;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, value, value_size);
}


/* UniformMatrix4x2dv: marshalled asynchronously */
struct marshal_cmd_UniformMatrix4x2dv
{
   struct marshal_cmd_base cmd_base;
   GLboolean transpose;
   GLint location;
   GLsizei count;
   /* Next safe_mul(count, 8 * sizeof(GLdouble)) bytes are GLdouble value[count][8] */
};
void
_mesa_unmarshal_UniformMatrix4x2dv(struct gl_context *ctx, const struct marshal_cmd_UniformMatrix4x2dv *cmd)
{
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLboolean transpose = cmd->transpose;
   GLdouble * value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLdouble *) variable_data;
   CALL_UniformMatrix4x2dv(ctx->CurrentServerDispatch, (location, count, transpose, value));
}
void GLAPIENTRY
_mesa_marshal_UniformMatrix4x2dv(GLint location, GLsizei count, GLboolean transpose, const GLdouble * value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 8 * sizeof(GLdouble));
   int cmd_size = sizeof(struct marshal_cmd_UniformMatrix4x2dv) + value_size;
   struct marshal_cmd_UniformMatrix4x2dv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "UniformMatrix4x2dv");
      CALL_UniformMatrix4x2dv(ctx->CurrentServerDispatch, (location, count, transpose, value));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_UniformMatrix4x2dv, cmd_size);
   cmd->location = location;
   cmd->count = count;
   cmd->transpose = transpose;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, value, value_size);
}


/* UniformMatrix4x3dv: marshalled asynchronously */
struct marshal_cmd_UniformMatrix4x3dv
{
   struct marshal_cmd_base cmd_base;
   GLboolean transpose;
   GLint location;
   GLsizei count;
   /* Next safe_mul(count, 12 * sizeof(GLdouble)) bytes are GLdouble value[count][12] */
};
void
_mesa_unmarshal_UniformMatrix4x3dv(struct gl_context *ctx, const struct marshal_cmd_UniformMatrix4x3dv *cmd)
{
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLboolean transpose = cmd->transpose;
   GLdouble * value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLdouble *) variable_data;
   CALL_UniformMatrix4x3dv(ctx->CurrentServerDispatch, (location, count, transpose, value));
}
void GLAPIENTRY
_mesa_marshal_UniformMatrix4x3dv(GLint location, GLsizei count, GLboolean transpose, const GLdouble * value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 12 * sizeof(GLdouble));
   int cmd_size = sizeof(struct marshal_cmd_UniformMatrix4x3dv) + value_size;
   struct marshal_cmd_UniformMatrix4x3dv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "UniformMatrix4x3dv");
      CALL_UniformMatrix4x3dv(ctx->CurrentServerDispatch, (location, count, transpose, value));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_UniformMatrix4x3dv, cmd_size);
   cmd->location = location;
   cmd->count = count;
   cmd->transpose = transpose;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, value, value_size);
}


/* GetUniformdv: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetUniformdv(GLuint program, GLint location, GLdouble * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetUniformdv");
   CALL_GetUniformdv(ctx->CurrentServerDispatch, (program, location, params));
}


/* ProgramUniform1d: marshalled asynchronously */
struct marshal_cmd_ProgramUniform1d
{
   struct marshal_cmd_base cmd_base;
   GLuint program;
   GLint location;
   GLdouble x;
};
void
_mesa_unmarshal_ProgramUniform1d(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform1d *cmd)
{
   GLuint program = cmd->program;
   GLint location = cmd->location;
   GLdouble x = cmd->x;
   CALL_ProgramUniform1d(ctx->CurrentServerDispatch, (program, location, x));
}
void GLAPIENTRY
_mesa_marshal_ProgramUniform1d(GLuint program, GLint location, GLdouble x)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ProgramUniform1d);
   struct marshal_cmd_ProgramUniform1d *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ProgramUniform1d, cmd_size);
   cmd->program = program;
   cmd->location = location;
   cmd->x = x;
}


/* ProgramUniform2d: marshalled asynchronously */
struct marshal_cmd_ProgramUniform2d
{
   struct marshal_cmd_base cmd_base;
   GLuint program;
   GLint location;
   GLdouble x;
   GLdouble y;
};
void
_mesa_unmarshal_ProgramUniform2d(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform2d *cmd)
{
   GLuint program = cmd->program;
   GLint location = cmd->location;
   GLdouble x = cmd->x;
   GLdouble y = cmd->y;
   CALL_ProgramUniform2d(ctx->CurrentServerDispatch, (program, location, x, y));
}
void GLAPIENTRY
_mesa_marshal_ProgramUniform2d(GLuint program, GLint location, GLdouble x, GLdouble y)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ProgramUniform2d);
   struct marshal_cmd_ProgramUniform2d *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ProgramUniform2d, cmd_size);
   cmd->program = program;
   cmd->location = location;
   cmd->x = x;
   cmd->y = y;
}


/* ProgramUniform3d: marshalled asynchronously */
struct marshal_cmd_ProgramUniform3d
{
   struct marshal_cmd_base cmd_base;
   GLuint program;
   GLint location;
   GLdouble x;
   GLdouble y;
   GLdouble z;
};
void
_mesa_unmarshal_ProgramUniform3d(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform3d *cmd)
{
   GLuint program = cmd->program;
   GLint location = cmd->location;
   GLdouble x = cmd->x;
   GLdouble y = cmd->y;
   GLdouble z = cmd->z;
   CALL_ProgramUniform3d(ctx->CurrentServerDispatch, (program, location, x, y, z));
}
void GLAPIENTRY
_mesa_marshal_ProgramUniform3d(GLuint program, GLint location, GLdouble x, GLdouble y, GLdouble z)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ProgramUniform3d);
   struct marshal_cmd_ProgramUniform3d *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ProgramUniform3d, cmd_size);
   cmd->program = program;
   cmd->location = location;
   cmd->x = x;
   cmd->y = y;
   cmd->z = z;
}


/* ProgramUniform4d: marshalled asynchronously */
struct marshal_cmd_ProgramUniform4d
{
   struct marshal_cmd_base cmd_base;
   GLuint program;
   GLint location;
   GLdouble x;
   GLdouble y;
   GLdouble z;
   GLdouble w;
};
void
_mesa_unmarshal_ProgramUniform4d(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform4d *cmd)
{
   GLuint program = cmd->program;
   GLint location = cmd->location;
   GLdouble x = cmd->x;
   GLdouble y = cmd->y;
   GLdouble z = cmd->z;
   GLdouble w = cmd->w;
   CALL_ProgramUniform4d(ctx->CurrentServerDispatch, (program, location, x, y, z, w));
}
void GLAPIENTRY
_mesa_marshal_ProgramUniform4d(GLuint program, GLint location, GLdouble x, GLdouble y, GLdouble z, GLdouble w)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ProgramUniform4d);
   struct marshal_cmd_ProgramUniform4d *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ProgramUniform4d, cmd_size);
   cmd->program = program;
   cmd->location = location;
   cmd->x = x;
   cmd->y = y;
   cmd->z = z;
   cmd->w = w;
}


/* ProgramUniform1dv: marshalled asynchronously */
struct marshal_cmd_ProgramUniform1dv
{
   struct marshal_cmd_base cmd_base;
   GLuint program;
   GLint location;
   GLsizei count;
   /* Next safe_mul(count, 1 * sizeof(GLdouble)) bytes are GLdouble value[count] */
};
void
_mesa_unmarshal_ProgramUniform1dv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform1dv *cmd)
{
   GLuint program = cmd->program;
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLdouble * value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLdouble *) variable_data;
   CALL_ProgramUniform1dv(ctx->CurrentServerDispatch, (program, location, count, value));
}
void GLAPIENTRY
_mesa_marshal_ProgramUniform1dv(GLuint program, GLint location, GLsizei count, const GLdouble * value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 1 * sizeof(GLdouble));
   int cmd_size = sizeof(struct marshal_cmd_ProgramUniform1dv) + value_size;
   struct marshal_cmd_ProgramUniform1dv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "ProgramUniform1dv");
      CALL_ProgramUniform1dv(ctx->CurrentServerDispatch, (program, location, count, value));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ProgramUniform1dv, cmd_size);
   cmd->program = program;
   cmd->location = location;
   cmd->count = count;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, value, value_size);
}


/* ProgramUniform2dv: marshalled asynchronously */
struct marshal_cmd_ProgramUniform2dv
{
   struct marshal_cmd_base cmd_base;
   GLuint program;
   GLint location;
   GLsizei count;
   /* Next safe_mul(count, 2 * sizeof(GLdouble)) bytes are GLdouble value[count][2] */
};
void
_mesa_unmarshal_ProgramUniform2dv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform2dv *cmd)
{
   GLuint program = cmd->program;
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLdouble * value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLdouble *) variable_data;
   CALL_ProgramUniform2dv(ctx->CurrentServerDispatch, (program, location, count, value));
}
void GLAPIENTRY
_mesa_marshal_ProgramUniform2dv(GLuint program, GLint location, GLsizei count, const GLdouble * value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 2 * sizeof(GLdouble));
   int cmd_size = sizeof(struct marshal_cmd_ProgramUniform2dv) + value_size;
   struct marshal_cmd_ProgramUniform2dv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "ProgramUniform2dv");
      CALL_ProgramUniform2dv(ctx->CurrentServerDispatch, (program, location, count, value));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ProgramUniform2dv, cmd_size);
   cmd->program = program;
   cmd->location = location;
   cmd->count = count;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, value, value_size);
}


/* ProgramUniform3dv: marshalled asynchronously */
struct marshal_cmd_ProgramUniform3dv
{
   struct marshal_cmd_base cmd_base;
   GLuint program;
   GLint location;
   GLsizei count;
   /* Next safe_mul(count, 3 * sizeof(GLdouble)) bytes are GLdouble value[count][3] */
};
void
_mesa_unmarshal_ProgramUniform3dv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform3dv *cmd)
{
   GLuint program = cmd->program;
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLdouble * value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLdouble *) variable_data;
   CALL_ProgramUniform3dv(ctx->CurrentServerDispatch, (program, location, count, value));
}
void GLAPIENTRY
_mesa_marshal_ProgramUniform3dv(GLuint program, GLint location, GLsizei count, const GLdouble * value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 3 * sizeof(GLdouble));
   int cmd_size = sizeof(struct marshal_cmd_ProgramUniform3dv) + value_size;
   struct marshal_cmd_ProgramUniform3dv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "ProgramUniform3dv");
      CALL_ProgramUniform3dv(ctx->CurrentServerDispatch, (program, location, count, value));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ProgramUniform3dv, cmd_size);
   cmd->program = program;
   cmd->location = location;
   cmd->count = count;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, value, value_size);
}


/* ProgramUniform4dv: marshalled asynchronously */
struct marshal_cmd_ProgramUniform4dv
{
   struct marshal_cmd_base cmd_base;
   GLuint program;
   GLint location;
   GLsizei count;
   /* Next safe_mul(count, 4 * sizeof(GLdouble)) bytes are GLdouble value[count][4] */
};
void
_mesa_unmarshal_ProgramUniform4dv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform4dv *cmd)
{
   GLuint program = cmd->program;
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLdouble * value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLdouble *) variable_data;
   CALL_ProgramUniform4dv(ctx->CurrentServerDispatch, (program, location, count, value));
}
void GLAPIENTRY
_mesa_marshal_ProgramUniform4dv(GLuint program, GLint location, GLsizei count, const GLdouble * value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 4 * sizeof(GLdouble));
   int cmd_size = sizeof(struct marshal_cmd_ProgramUniform4dv) + value_size;
   struct marshal_cmd_ProgramUniform4dv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "ProgramUniform4dv");
      CALL_ProgramUniform4dv(ctx->CurrentServerDispatch, (program, location, count, value));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ProgramUniform4dv, cmd_size);
   cmd->program = program;
   cmd->location = location;
   cmd->count = count;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, value, value_size);
}


/* ProgramUniformMatrix2dv: marshalled asynchronously */
struct marshal_cmd_ProgramUniformMatrix2dv
{
   struct marshal_cmd_base cmd_base;
   GLboolean transpose;
   GLuint program;
   GLint location;
   GLsizei count;
   /* Next safe_mul(count, 4 * sizeof(GLdouble)) bytes are GLdouble value[count][4] */
};
void
_mesa_unmarshal_ProgramUniformMatrix2dv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniformMatrix2dv *cmd)
{
   GLuint program = cmd->program;
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLboolean transpose = cmd->transpose;
   GLdouble * value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLdouble *) variable_data;
   CALL_ProgramUniformMatrix2dv(ctx->CurrentServerDispatch, (program, location, count, transpose, value));
}
void GLAPIENTRY
_mesa_marshal_ProgramUniformMatrix2dv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble * value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 4 * sizeof(GLdouble));
   int cmd_size = sizeof(struct marshal_cmd_ProgramUniformMatrix2dv) + value_size;
   struct marshal_cmd_ProgramUniformMatrix2dv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "ProgramUniformMatrix2dv");
      CALL_ProgramUniformMatrix2dv(ctx->CurrentServerDispatch, (program, location, count, transpose, value));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ProgramUniformMatrix2dv, cmd_size);
   cmd->program = program;
   cmd->location = location;
   cmd->count = count;
   cmd->transpose = transpose;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, value, value_size);
}


/* ProgramUniformMatrix3dv: marshalled asynchronously */
struct marshal_cmd_ProgramUniformMatrix3dv
{
   struct marshal_cmd_base cmd_base;
   GLboolean transpose;
   GLuint program;
   GLint location;
   GLsizei count;
   /* Next safe_mul(count, 9 * sizeof(GLdouble)) bytes are GLdouble value[count][9] */
};
void
_mesa_unmarshal_ProgramUniformMatrix3dv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniformMatrix3dv *cmd)
{
   GLuint program = cmd->program;
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLboolean transpose = cmd->transpose;
   GLdouble * value;
   const char *variable_data = (const char *) (cmd + 1);
   value = (GLdouble *) variable_data;
   CALL_ProgramUniformMatrix3dv(ctx->CurrentServerDispatch, (program, location, count, transpose, value));
}
void GLAPIENTRY
_mesa_marshal_ProgramUniformMatrix3dv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble * value)
{
   GET_CURRENT_CONTEXT(ctx);
   int value_size = safe_mul(count, 9 * sizeof(GLdouble));
   int cmd_size = sizeof(struct marshal_cmd_ProgramUniformMatrix3dv) + value_size;
   struct marshal_cmd_ProgramUniformMatrix3dv *cmd;
   if (unlikely(value_size < 0 || (value_size > 0 && !value) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "ProgramUniformMatrix3dv");
      CALL_ProgramUniformMatrix3dv(ctx->CurrentServerDispatch, (program, location, count, transpose, value));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ProgramUniformMatrix3dv, cmd_size);
   cmd->program = program;
   cmd->location = location;
   cmd->count = count;
   cmd->transpose = transpose;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, value, value_size);
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
   GLuint program = cmd->program;
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLboolean transpose = cmd->transpose;
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
   GLuint program = cmd->program;
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLboolean transpose = cmd->transpose;
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
   GLuint program = cmd->program;
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLboolean transpose = cmd->transpose;
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
   GLuint program = cmd->program;
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLboolean transpose = cmd->transpose;
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
   GLuint program = cmd->program;
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLboolean transpose = cmd->transpose;
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
   GLuint program = cmd->program;
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLboolean transpose = cmd->transpose;
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
   GLuint program = cmd->program;
   GLint location = cmd->location;
   GLsizei count = cmd->count;
   GLboolean transpose = cmd->transpose;
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
   GLenum mode;
   GLuint id;
   GLuint stream;
};
void
_mesa_unmarshal_DrawTransformFeedbackStream(struct gl_context *ctx, const struct marshal_cmd_DrawTransformFeedbackStream *cmd)
{
   GLenum mode = cmd->mode;
   GLuint id = cmd->id;
   GLuint stream = cmd->stream;
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
   GLenum target;
   GLuint index;
   GLuint id;
};
void
_mesa_unmarshal_BeginQueryIndexed(struct gl_context *ctx, const struct marshal_cmd_BeginQueryIndexed *cmd)
{
   GLenum target = cmd->target;
   GLuint index = cmd->index;
   GLuint id = cmd->id;
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
   GLenum target;
   GLuint index;
};
void
_mesa_unmarshal_EndQueryIndexed(struct gl_context *ctx, const struct marshal_cmd_EndQueryIndexed *cmd)
{
   GLenum target = cmd->target;
   GLuint index = cmd->index;
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
   GLuint pipeline = cmd->pipeline;
   GLbitfield stages = cmd->stages;
   GLuint program = cmd->program;
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
   GLuint pipeline = cmd->pipeline;
   GLuint program = cmd->program;
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
   GLuint pipeline = cmd->pipeline;
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
   GLsizei n = cmd->n;
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
   GLuint program = cmd->program;
   GLint location = cmd->location;
   GLint x = cmd->x;
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
   GLuint program = cmd->program;
   GLint location = cmd->location;
   GLint x = cmd->x;
   GLint y = cmd->y;
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


