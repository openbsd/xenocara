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

/* VertexAttrib1sNV: marshalled asynchronously */
struct marshal_cmd_VertexAttrib1sNV
{
   struct marshal_cmd_base cmd_base;
   GLshort x;
   GLuint index;
};
void
_mesa_unmarshal_VertexAttrib1sNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib1sNV *cmd)
{
   GLuint index = cmd->index;
   GLshort x = cmd->x;
   CALL_VertexAttrib1sNV(ctx->CurrentServerDispatch, (index, x));
}
void GLAPIENTRY
_mesa_marshal_VertexAttrib1sNV(GLuint index, GLshort x)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttrib1sNV);
   struct marshal_cmd_VertexAttrib1sNV *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttrib1sNV, cmd_size);
   cmd->index = index;
   cmd->x = x;
}


/* VertexAttrib1svNV: marshalled asynchronously */
struct marshal_cmd_VertexAttrib1svNV
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLshort v[1];
};
void
_mesa_unmarshal_VertexAttrib1svNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib1svNV *cmd)
{
   GLuint index = cmd->index;
   const GLshort * v = cmd->v;
   CALL_VertexAttrib1svNV(ctx->CurrentServerDispatch, (index, v));
}
void GLAPIENTRY
_mesa_marshal_VertexAttrib1svNV(GLuint index, const GLshort * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttrib1svNV);
   struct marshal_cmd_VertexAttrib1svNV *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttrib1svNV, cmd_size);
   cmd->index = index;
   memcpy(cmd->v, v, 1 * sizeof(GLshort));
}


/* VertexAttrib2sNV: marshalled asynchronously */
struct marshal_cmd_VertexAttrib2sNV
{
   struct marshal_cmd_base cmd_base;
   GLshort x;
   GLshort y;
   GLuint index;
};
void
_mesa_unmarshal_VertexAttrib2sNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib2sNV *cmd)
{
   GLuint index = cmd->index;
   GLshort x = cmd->x;
   GLshort y = cmd->y;
   CALL_VertexAttrib2sNV(ctx->CurrentServerDispatch, (index, x, y));
}
void GLAPIENTRY
_mesa_marshal_VertexAttrib2sNV(GLuint index, GLshort x, GLshort y)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttrib2sNV);
   struct marshal_cmd_VertexAttrib2sNV *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttrib2sNV, cmd_size);
   cmd->index = index;
   cmd->x = x;
   cmd->y = y;
}


/* VertexAttrib2svNV: marshalled asynchronously */
struct marshal_cmd_VertexAttrib2svNV
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLshort v[2];
};
void
_mesa_unmarshal_VertexAttrib2svNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib2svNV *cmd)
{
   GLuint index = cmd->index;
   const GLshort * v = cmd->v;
   CALL_VertexAttrib2svNV(ctx->CurrentServerDispatch, (index, v));
}
void GLAPIENTRY
_mesa_marshal_VertexAttrib2svNV(GLuint index, const GLshort * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttrib2svNV);
   struct marshal_cmd_VertexAttrib2svNV *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttrib2svNV, cmd_size);
   cmd->index = index;
   memcpy(cmd->v, v, 2 * sizeof(GLshort));
}


/* VertexAttrib3sNV: marshalled asynchronously */
struct marshal_cmd_VertexAttrib3sNV
{
   struct marshal_cmd_base cmd_base;
   GLshort x;
   GLshort y;
   GLshort z;
   GLuint index;
};
void
_mesa_unmarshal_VertexAttrib3sNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib3sNV *cmd)
{
   GLuint index = cmd->index;
   GLshort x = cmd->x;
   GLshort y = cmd->y;
   GLshort z = cmd->z;
   CALL_VertexAttrib3sNV(ctx->CurrentServerDispatch, (index, x, y, z));
}
void GLAPIENTRY
_mesa_marshal_VertexAttrib3sNV(GLuint index, GLshort x, GLshort y, GLshort z)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttrib3sNV);
   struct marshal_cmd_VertexAttrib3sNV *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttrib3sNV, cmd_size);
   cmd->index = index;
   cmd->x = x;
   cmd->y = y;
   cmd->z = z;
}


/* VertexAttrib3svNV: marshalled asynchronously */
struct marshal_cmd_VertexAttrib3svNV
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLshort v[3];
};
void
_mesa_unmarshal_VertexAttrib3svNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib3svNV *cmd)
{
   GLuint index = cmd->index;
   const GLshort * v = cmd->v;
   CALL_VertexAttrib3svNV(ctx->CurrentServerDispatch, (index, v));
}
void GLAPIENTRY
_mesa_marshal_VertexAttrib3svNV(GLuint index, const GLshort * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttrib3svNV);
   struct marshal_cmd_VertexAttrib3svNV *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttrib3svNV, cmd_size);
   cmd->index = index;
   memcpy(cmd->v, v, 3 * sizeof(GLshort));
}


/* VertexAttrib4sNV: marshalled asynchronously */
struct marshal_cmd_VertexAttrib4sNV
{
   struct marshal_cmd_base cmd_base;
   GLshort x;
   GLshort y;
   GLshort z;
   GLshort w;
   GLuint index;
};
void
_mesa_unmarshal_VertexAttrib4sNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4sNV *cmd)
{
   GLuint index = cmd->index;
   GLshort x = cmd->x;
   GLshort y = cmd->y;
   GLshort z = cmd->z;
   GLshort w = cmd->w;
   CALL_VertexAttrib4sNV(ctx->CurrentServerDispatch, (index, x, y, z, w));
}
void GLAPIENTRY
_mesa_marshal_VertexAttrib4sNV(GLuint index, GLshort x, GLshort y, GLshort z, GLshort w)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttrib4sNV);
   struct marshal_cmd_VertexAttrib4sNV *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttrib4sNV, cmd_size);
   cmd->index = index;
   cmd->x = x;
   cmd->y = y;
   cmd->z = z;
   cmd->w = w;
}


/* VertexAttrib4svNV: marshalled asynchronously */
struct marshal_cmd_VertexAttrib4svNV
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLshort v[4];
};
void
_mesa_unmarshal_VertexAttrib4svNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4svNV *cmd)
{
   GLuint index = cmd->index;
   const GLshort * v = cmd->v;
   CALL_VertexAttrib4svNV(ctx->CurrentServerDispatch, (index, v));
}
void GLAPIENTRY
_mesa_marshal_VertexAttrib4svNV(GLuint index, const GLshort * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttrib4svNV);
   struct marshal_cmd_VertexAttrib4svNV *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttrib4svNV, cmd_size);
   cmd->index = index;
   memcpy(cmd->v, v, 4 * sizeof(GLshort));
}


/* VertexAttrib1fNV: marshalled asynchronously */
struct marshal_cmd_VertexAttrib1fNV
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLfloat x;
};
void
_mesa_unmarshal_VertexAttrib1fNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib1fNV *cmd)
{
   GLuint index = cmd->index;
   GLfloat x = cmd->x;
   CALL_VertexAttrib1fNV(ctx->CurrentServerDispatch, (index, x));
}
void GLAPIENTRY
_mesa_marshal_VertexAttrib1fNV(GLuint index, GLfloat x)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttrib1fNV);
   struct marshal_cmd_VertexAttrib1fNV *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttrib1fNV, cmd_size);
   cmd->index = index;
   cmd->x = x;
}


/* VertexAttrib1fvNV: marshalled asynchronously */
struct marshal_cmd_VertexAttrib1fvNV
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLfloat v[1];
};
void
_mesa_unmarshal_VertexAttrib1fvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib1fvNV *cmd)
{
   GLuint index = cmd->index;
   const GLfloat * v = cmd->v;
   CALL_VertexAttrib1fvNV(ctx->CurrentServerDispatch, (index, v));
}
void GLAPIENTRY
_mesa_marshal_VertexAttrib1fvNV(GLuint index, const GLfloat * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttrib1fvNV);
   struct marshal_cmd_VertexAttrib1fvNV *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttrib1fvNV, cmd_size);
   cmd->index = index;
   memcpy(cmd->v, v, 1 * sizeof(GLfloat));
}


/* VertexAttrib2fNV: marshalled asynchronously */
struct marshal_cmd_VertexAttrib2fNV
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLfloat x;
   GLfloat y;
};
void
_mesa_unmarshal_VertexAttrib2fNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib2fNV *cmd)
{
   GLuint index = cmd->index;
   GLfloat x = cmd->x;
   GLfloat y = cmd->y;
   CALL_VertexAttrib2fNV(ctx->CurrentServerDispatch, (index, x, y));
}
void GLAPIENTRY
_mesa_marshal_VertexAttrib2fNV(GLuint index, GLfloat x, GLfloat y)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttrib2fNV);
   struct marshal_cmd_VertexAttrib2fNV *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttrib2fNV, cmd_size);
   cmd->index = index;
   cmd->x = x;
   cmd->y = y;
}


/* VertexAttrib2fvNV: marshalled asynchronously */
struct marshal_cmd_VertexAttrib2fvNV
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLfloat v[2];
};
void
_mesa_unmarshal_VertexAttrib2fvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib2fvNV *cmd)
{
   GLuint index = cmd->index;
   const GLfloat * v = cmd->v;
   CALL_VertexAttrib2fvNV(ctx->CurrentServerDispatch, (index, v));
}
void GLAPIENTRY
_mesa_marshal_VertexAttrib2fvNV(GLuint index, const GLfloat * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttrib2fvNV);
   struct marshal_cmd_VertexAttrib2fvNV *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttrib2fvNV, cmd_size);
   cmd->index = index;
   memcpy(cmd->v, v, 2 * sizeof(GLfloat));
}


/* VertexAttrib3fNV: marshalled asynchronously */
struct marshal_cmd_VertexAttrib3fNV
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLfloat x;
   GLfloat y;
   GLfloat z;
};
void
_mesa_unmarshal_VertexAttrib3fNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib3fNV *cmd)
{
   GLuint index = cmd->index;
   GLfloat x = cmd->x;
   GLfloat y = cmd->y;
   GLfloat z = cmd->z;
   CALL_VertexAttrib3fNV(ctx->CurrentServerDispatch, (index, x, y, z));
}
void GLAPIENTRY
_mesa_marshal_VertexAttrib3fNV(GLuint index, GLfloat x, GLfloat y, GLfloat z)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttrib3fNV);
   struct marshal_cmd_VertexAttrib3fNV *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttrib3fNV, cmd_size);
   cmd->index = index;
   cmd->x = x;
   cmd->y = y;
   cmd->z = z;
}


/* VertexAttrib3fvNV: marshalled asynchronously */
struct marshal_cmd_VertexAttrib3fvNV
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLfloat v[3];
};
void
_mesa_unmarshal_VertexAttrib3fvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib3fvNV *cmd)
{
   GLuint index = cmd->index;
   const GLfloat * v = cmd->v;
   CALL_VertexAttrib3fvNV(ctx->CurrentServerDispatch, (index, v));
}
void GLAPIENTRY
_mesa_marshal_VertexAttrib3fvNV(GLuint index, const GLfloat * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttrib3fvNV);
   struct marshal_cmd_VertexAttrib3fvNV *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttrib3fvNV, cmd_size);
   cmd->index = index;
   memcpy(cmd->v, v, 3 * sizeof(GLfloat));
}


/* VertexAttrib4fNV: marshalled asynchronously */
struct marshal_cmd_VertexAttrib4fNV
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLfloat x;
   GLfloat y;
   GLfloat z;
   GLfloat w;
};
void
_mesa_unmarshal_VertexAttrib4fNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4fNV *cmd)
{
   GLuint index = cmd->index;
   GLfloat x = cmd->x;
   GLfloat y = cmd->y;
   GLfloat z = cmd->z;
   GLfloat w = cmd->w;
   CALL_VertexAttrib4fNV(ctx->CurrentServerDispatch, (index, x, y, z, w));
}
void GLAPIENTRY
_mesa_marshal_VertexAttrib4fNV(GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttrib4fNV);
   struct marshal_cmd_VertexAttrib4fNV *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttrib4fNV, cmd_size);
   cmd->index = index;
   cmd->x = x;
   cmd->y = y;
   cmd->z = z;
   cmd->w = w;
}


/* VertexAttrib4fvNV: marshalled asynchronously */
struct marshal_cmd_VertexAttrib4fvNV
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLfloat v[4];
};
void
_mesa_unmarshal_VertexAttrib4fvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4fvNV *cmd)
{
   GLuint index = cmd->index;
   const GLfloat * v = cmd->v;
   CALL_VertexAttrib4fvNV(ctx->CurrentServerDispatch, (index, v));
}
void GLAPIENTRY
_mesa_marshal_VertexAttrib4fvNV(GLuint index, const GLfloat * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttrib4fvNV);
   struct marshal_cmd_VertexAttrib4fvNV *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttrib4fvNV, cmd_size);
   cmd->index = index;
   memcpy(cmd->v, v, 4 * sizeof(GLfloat));
}


/* VertexAttrib1dNV: marshalled asynchronously */
struct marshal_cmd_VertexAttrib1dNV
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLdouble x;
};
void
_mesa_unmarshal_VertexAttrib1dNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib1dNV *cmd)
{
   GLuint index = cmd->index;
   GLdouble x = cmd->x;
   CALL_VertexAttrib1dNV(ctx->CurrentServerDispatch, (index, x));
}
void GLAPIENTRY
_mesa_marshal_VertexAttrib1dNV(GLuint index, GLdouble x)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttrib1dNV);
   struct marshal_cmd_VertexAttrib1dNV *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttrib1dNV, cmd_size);
   cmd->index = index;
   cmd->x = x;
}


/* VertexAttrib1dvNV: marshalled asynchronously */
struct marshal_cmd_VertexAttrib1dvNV
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLdouble v[1];
};
void
_mesa_unmarshal_VertexAttrib1dvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib1dvNV *cmd)
{
   GLuint index = cmd->index;
   const GLdouble * v = cmd->v;
   CALL_VertexAttrib1dvNV(ctx->CurrentServerDispatch, (index, v));
}
void GLAPIENTRY
_mesa_marshal_VertexAttrib1dvNV(GLuint index, const GLdouble * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttrib1dvNV);
   struct marshal_cmd_VertexAttrib1dvNV *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttrib1dvNV, cmd_size);
   cmd->index = index;
   memcpy(cmd->v, v, 1 * sizeof(GLdouble));
}


/* VertexAttrib2dNV: marshalled asynchronously */
struct marshal_cmd_VertexAttrib2dNV
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLdouble x;
   GLdouble y;
};
void
_mesa_unmarshal_VertexAttrib2dNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib2dNV *cmd)
{
   GLuint index = cmd->index;
   GLdouble x = cmd->x;
   GLdouble y = cmd->y;
   CALL_VertexAttrib2dNV(ctx->CurrentServerDispatch, (index, x, y));
}
void GLAPIENTRY
_mesa_marshal_VertexAttrib2dNV(GLuint index, GLdouble x, GLdouble y)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttrib2dNV);
   struct marshal_cmd_VertexAttrib2dNV *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttrib2dNV, cmd_size);
   cmd->index = index;
   cmd->x = x;
   cmd->y = y;
}


/* VertexAttrib2dvNV: marshalled asynchronously */
struct marshal_cmd_VertexAttrib2dvNV
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLdouble v[2];
};
void
_mesa_unmarshal_VertexAttrib2dvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib2dvNV *cmd)
{
   GLuint index = cmd->index;
   const GLdouble * v = cmd->v;
   CALL_VertexAttrib2dvNV(ctx->CurrentServerDispatch, (index, v));
}
void GLAPIENTRY
_mesa_marshal_VertexAttrib2dvNV(GLuint index, const GLdouble * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttrib2dvNV);
   struct marshal_cmd_VertexAttrib2dvNV *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttrib2dvNV, cmd_size);
   cmd->index = index;
   memcpy(cmd->v, v, 2 * sizeof(GLdouble));
}


/* VertexAttrib3dNV: marshalled asynchronously */
struct marshal_cmd_VertexAttrib3dNV
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLdouble x;
   GLdouble y;
   GLdouble z;
};
void
_mesa_unmarshal_VertexAttrib3dNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib3dNV *cmd)
{
   GLuint index = cmd->index;
   GLdouble x = cmd->x;
   GLdouble y = cmd->y;
   GLdouble z = cmd->z;
   CALL_VertexAttrib3dNV(ctx->CurrentServerDispatch, (index, x, y, z));
}
void GLAPIENTRY
_mesa_marshal_VertexAttrib3dNV(GLuint index, GLdouble x, GLdouble y, GLdouble z)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttrib3dNV);
   struct marshal_cmd_VertexAttrib3dNV *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttrib3dNV, cmd_size);
   cmd->index = index;
   cmd->x = x;
   cmd->y = y;
   cmd->z = z;
}


/* VertexAttrib3dvNV: marshalled asynchronously */
struct marshal_cmd_VertexAttrib3dvNV
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLdouble v[3];
};
void
_mesa_unmarshal_VertexAttrib3dvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib3dvNV *cmd)
{
   GLuint index = cmd->index;
   const GLdouble * v = cmd->v;
   CALL_VertexAttrib3dvNV(ctx->CurrentServerDispatch, (index, v));
}
void GLAPIENTRY
_mesa_marshal_VertexAttrib3dvNV(GLuint index, const GLdouble * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttrib3dvNV);
   struct marshal_cmd_VertexAttrib3dvNV *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttrib3dvNV, cmd_size);
   cmd->index = index;
   memcpy(cmd->v, v, 3 * sizeof(GLdouble));
}


/* VertexAttrib4dNV: marshalled asynchronously */
struct marshal_cmd_VertexAttrib4dNV
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLdouble x;
   GLdouble y;
   GLdouble z;
   GLdouble w;
};
void
_mesa_unmarshal_VertexAttrib4dNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4dNV *cmd)
{
   GLuint index = cmd->index;
   GLdouble x = cmd->x;
   GLdouble y = cmd->y;
   GLdouble z = cmd->z;
   GLdouble w = cmd->w;
   CALL_VertexAttrib4dNV(ctx->CurrentServerDispatch, (index, x, y, z, w));
}
void GLAPIENTRY
_mesa_marshal_VertexAttrib4dNV(GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttrib4dNV);
   struct marshal_cmd_VertexAttrib4dNV *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttrib4dNV, cmd_size);
   cmd->index = index;
   cmd->x = x;
   cmd->y = y;
   cmd->z = z;
   cmd->w = w;
}


/* VertexAttrib4dvNV: marshalled asynchronously */
struct marshal_cmd_VertexAttrib4dvNV
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLdouble v[4];
};
void
_mesa_unmarshal_VertexAttrib4dvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4dvNV *cmd)
{
   GLuint index = cmd->index;
   const GLdouble * v = cmd->v;
   CALL_VertexAttrib4dvNV(ctx->CurrentServerDispatch, (index, v));
}
void GLAPIENTRY
_mesa_marshal_VertexAttrib4dvNV(GLuint index, const GLdouble * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttrib4dvNV);
   struct marshal_cmd_VertexAttrib4dvNV *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttrib4dvNV, cmd_size);
   cmd->index = index;
   memcpy(cmd->v, v, 4 * sizeof(GLdouble));
}


/* VertexAttrib4ubNV: marshalled asynchronously */
struct marshal_cmd_VertexAttrib4ubNV
{
   struct marshal_cmd_base cmd_base;
   GLubyte x;
   GLubyte y;
   GLubyte z;
   GLubyte w;
   GLuint index;
};
void
_mesa_unmarshal_VertexAttrib4ubNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4ubNV *cmd)
{
   GLuint index = cmd->index;
   GLubyte x = cmd->x;
   GLubyte y = cmd->y;
   GLubyte z = cmd->z;
   GLubyte w = cmd->w;
   CALL_VertexAttrib4ubNV(ctx->CurrentServerDispatch, (index, x, y, z, w));
}
void GLAPIENTRY
_mesa_marshal_VertexAttrib4ubNV(GLuint index, GLubyte x, GLubyte y, GLubyte z, GLubyte w)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttrib4ubNV);
   struct marshal_cmd_VertexAttrib4ubNV *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttrib4ubNV, cmd_size);
   cmd->index = index;
   cmd->x = x;
   cmd->y = y;
   cmd->z = z;
   cmd->w = w;
}


/* VertexAttrib4ubvNV: marshalled asynchronously */
struct marshal_cmd_VertexAttrib4ubvNV
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLubyte v[4];
};
void
_mesa_unmarshal_VertexAttrib4ubvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4ubvNV *cmd)
{
   GLuint index = cmd->index;
   const GLubyte * v = cmd->v;
   CALL_VertexAttrib4ubvNV(ctx->CurrentServerDispatch, (index, v));
}
void GLAPIENTRY
_mesa_marshal_VertexAttrib4ubvNV(GLuint index, const GLubyte * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexAttrib4ubvNV);
   struct marshal_cmd_VertexAttrib4ubvNV *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttrib4ubvNV, cmd_size);
   cmd->index = index;
   memcpy(cmd->v, v, 4 * sizeof(GLubyte));
}


/* VertexAttribs1svNV: marshalled asynchronously */
struct marshal_cmd_VertexAttribs1svNV
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLsizei n;
   /* Next safe_mul(n, 1 * sizeof(GLshort)) bytes are GLshort v[n] */
};
void
_mesa_unmarshal_VertexAttribs1svNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttribs1svNV *cmd)
{
   GLuint index = cmd->index;
   GLsizei n = cmd->n;
   GLshort * v;
   const char *variable_data = (const char *) (cmd + 1);
   v = (GLshort *) variable_data;
   CALL_VertexAttribs1svNV(ctx->CurrentServerDispatch, (index, n, v));
}
void GLAPIENTRY
_mesa_marshal_VertexAttribs1svNV(GLuint index, GLsizei n, const GLshort * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int v_size = safe_mul(n, 1 * sizeof(GLshort));
   int cmd_size = sizeof(struct marshal_cmd_VertexAttribs1svNV) + v_size;
   struct marshal_cmd_VertexAttribs1svNV *cmd;
   if (unlikely(v_size < 0 || (v_size > 0 && !v) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "VertexAttribs1svNV");
      CALL_VertexAttribs1svNV(ctx->CurrentServerDispatch, (index, n, v));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttribs1svNV, cmd_size);
   cmd->index = index;
   cmd->n = n;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, v, v_size);
}


/* VertexAttribs2svNV: marshalled asynchronously */
struct marshal_cmd_VertexAttribs2svNV
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLsizei n;
   /* Next safe_mul(n, 2 * sizeof(GLshort)) bytes are GLshort v[n][2] */
};
void
_mesa_unmarshal_VertexAttribs2svNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttribs2svNV *cmd)
{
   GLuint index = cmd->index;
   GLsizei n = cmd->n;
   GLshort * v;
   const char *variable_data = (const char *) (cmd + 1);
   v = (GLshort *) variable_data;
   CALL_VertexAttribs2svNV(ctx->CurrentServerDispatch, (index, n, v));
}
void GLAPIENTRY
_mesa_marshal_VertexAttribs2svNV(GLuint index, GLsizei n, const GLshort * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int v_size = safe_mul(n, 2 * sizeof(GLshort));
   int cmd_size = sizeof(struct marshal_cmd_VertexAttribs2svNV) + v_size;
   struct marshal_cmd_VertexAttribs2svNV *cmd;
   if (unlikely(v_size < 0 || (v_size > 0 && !v) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "VertexAttribs2svNV");
      CALL_VertexAttribs2svNV(ctx->CurrentServerDispatch, (index, n, v));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttribs2svNV, cmd_size);
   cmd->index = index;
   cmd->n = n;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, v, v_size);
}


/* VertexAttribs3svNV: marshalled asynchronously */
struct marshal_cmd_VertexAttribs3svNV
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLsizei n;
   /* Next safe_mul(n, 3 * sizeof(GLshort)) bytes are GLshort v[n][3] */
};
void
_mesa_unmarshal_VertexAttribs3svNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttribs3svNV *cmd)
{
   GLuint index = cmd->index;
   GLsizei n = cmd->n;
   GLshort * v;
   const char *variable_data = (const char *) (cmd + 1);
   v = (GLshort *) variable_data;
   CALL_VertexAttribs3svNV(ctx->CurrentServerDispatch, (index, n, v));
}
void GLAPIENTRY
_mesa_marshal_VertexAttribs3svNV(GLuint index, GLsizei n, const GLshort * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int v_size = safe_mul(n, 3 * sizeof(GLshort));
   int cmd_size = sizeof(struct marshal_cmd_VertexAttribs3svNV) + v_size;
   struct marshal_cmd_VertexAttribs3svNV *cmd;
   if (unlikely(v_size < 0 || (v_size > 0 && !v) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "VertexAttribs3svNV");
      CALL_VertexAttribs3svNV(ctx->CurrentServerDispatch, (index, n, v));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttribs3svNV, cmd_size);
   cmd->index = index;
   cmd->n = n;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, v, v_size);
}


/* VertexAttribs4svNV: marshalled asynchronously */
struct marshal_cmd_VertexAttribs4svNV
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLsizei n;
   /* Next safe_mul(n, 4 * sizeof(GLshort)) bytes are GLshort v[n][4] */
};
void
_mesa_unmarshal_VertexAttribs4svNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttribs4svNV *cmd)
{
   GLuint index = cmd->index;
   GLsizei n = cmd->n;
   GLshort * v;
   const char *variable_data = (const char *) (cmd + 1);
   v = (GLshort *) variable_data;
   CALL_VertexAttribs4svNV(ctx->CurrentServerDispatch, (index, n, v));
}
void GLAPIENTRY
_mesa_marshal_VertexAttribs4svNV(GLuint index, GLsizei n, const GLshort * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int v_size = safe_mul(n, 4 * sizeof(GLshort));
   int cmd_size = sizeof(struct marshal_cmd_VertexAttribs4svNV) + v_size;
   struct marshal_cmd_VertexAttribs4svNV *cmd;
   if (unlikely(v_size < 0 || (v_size > 0 && !v) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "VertexAttribs4svNV");
      CALL_VertexAttribs4svNV(ctx->CurrentServerDispatch, (index, n, v));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttribs4svNV, cmd_size);
   cmd->index = index;
   cmd->n = n;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, v, v_size);
}


/* VertexAttribs1fvNV: marshalled asynchronously */
struct marshal_cmd_VertexAttribs1fvNV
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLsizei n;
   /* Next safe_mul(n, 1 * sizeof(GLfloat)) bytes are GLfloat v[n] */
};
void
_mesa_unmarshal_VertexAttribs1fvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttribs1fvNV *cmd)
{
   GLuint index = cmd->index;
   GLsizei n = cmd->n;
   GLfloat * v;
   const char *variable_data = (const char *) (cmd + 1);
   v = (GLfloat *) variable_data;
   CALL_VertexAttribs1fvNV(ctx->CurrentServerDispatch, (index, n, v));
}
void GLAPIENTRY
_mesa_marshal_VertexAttribs1fvNV(GLuint index, GLsizei n, const GLfloat * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int v_size = safe_mul(n, 1 * sizeof(GLfloat));
   int cmd_size = sizeof(struct marshal_cmd_VertexAttribs1fvNV) + v_size;
   struct marshal_cmd_VertexAttribs1fvNV *cmd;
   if (unlikely(v_size < 0 || (v_size > 0 && !v) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "VertexAttribs1fvNV");
      CALL_VertexAttribs1fvNV(ctx->CurrentServerDispatch, (index, n, v));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttribs1fvNV, cmd_size);
   cmd->index = index;
   cmd->n = n;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, v, v_size);
}


/* VertexAttribs2fvNV: marshalled asynchronously */
struct marshal_cmd_VertexAttribs2fvNV
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLsizei n;
   /* Next safe_mul(n, 2 * sizeof(GLfloat)) bytes are GLfloat v[n][2] */
};
void
_mesa_unmarshal_VertexAttribs2fvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttribs2fvNV *cmd)
{
   GLuint index = cmd->index;
   GLsizei n = cmd->n;
   GLfloat * v;
   const char *variable_data = (const char *) (cmd + 1);
   v = (GLfloat *) variable_data;
   CALL_VertexAttribs2fvNV(ctx->CurrentServerDispatch, (index, n, v));
}
void GLAPIENTRY
_mesa_marshal_VertexAttribs2fvNV(GLuint index, GLsizei n, const GLfloat * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int v_size = safe_mul(n, 2 * sizeof(GLfloat));
   int cmd_size = sizeof(struct marshal_cmd_VertexAttribs2fvNV) + v_size;
   struct marshal_cmd_VertexAttribs2fvNV *cmd;
   if (unlikely(v_size < 0 || (v_size > 0 && !v) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "VertexAttribs2fvNV");
      CALL_VertexAttribs2fvNV(ctx->CurrentServerDispatch, (index, n, v));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttribs2fvNV, cmd_size);
   cmd->index = index;
   cmd->n = n;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, v, v_size);
}


/* VertexAttribs3fvNV: marshalled asynchronously */
struct marshal_cmd_VertexAttribs3fvNV
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLsizei n;
   /* Next safe_mul(n, 3 * sizeof(GLfloat)) bytes are GLfloat v[n][3] */
};
void
_mesa_unmarshal_VertexAttribs3fvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttribs3fvNV *cmd)
{
   GLuint index = cmd->index;
   GLsizei n = cmd->n;
   GLfloat * v;
   const char *variable_data = (const char *) (cmd + 1);
   v = (GLfloat *) variable_data;
   CALL_VertexAttribs3fvNV(ctx->CurrentServerDispatch, (index, n, v));
}
void GLAPIENTRY
_mesa_marshal_VertexAttribs3fvNV(GLuint index, GLsizei n, const GLfloat * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int v_size = safe_mul(n, 3 * sizeof(GLfloat));
   int cmd_size = sizeof(struct marshal_cmd_VertexAttribs3fvNV) + v_size;
   struct marshal_cmd_VertexAttribs3fvNV *cmd;
   if (unlikely(v_size < 0 || (v_size > 0 && !v) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "VertexAttribs3fvNV");
      CALL_VertexAttribs3fvNV(ctx->CurrentServerDispatch, (index, n, v));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttribs3fvNV, cmd_size);
   cmd->index = index;
   cmd->n = n;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, v, v_size);
}


/* VertexAttribs4fvNV: marshalled asynchronously */
struct marshal_cmd_VertexAttribs4fvNV
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLsizei n;
   /* Next safe_mul(n, 4 * sizeof(GLfloat)) bytes are GLfloat v[n][4] */
};
void
_mesa_unmarshal_VertexAttribs4fvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttribs4fvNV *cmd)
{
   GLuint index = cmd->index;
   GLsizei n = cmd->n;
   GLfloat * v;
   const char *variable_data = (const char *) (cmd + 1);
   v = (GLfloat *) variable_data;
   CALL_VertexAttribs4fvNV(ctx->CurrentServerDispatch, (index, n, v));
}
void GLAPIENTRY
_mesa_marshal_VertexAttribs4fvNV(GLuint index, GLsizei n, const GLfloat * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int v_size = safe_mul(n, 4 * sizeof(GLfloat));
   int cmd_size = sizeof(struct marshal_cmd_VertexAttribs4fvNV) + v_size;
   struct marshal_cmd_VertexAttribs4fvNV *cmd;
   if (unlikely(v_size < 0 || (v_size > 0 && !v) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "VertexAttribs4fvNV");
      CALL_VertexAttribs4fvNV(ctx->CurrentServerDispatch, (index, n, v));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttribs4fvNV, cmd_size);
   cmd->index = index;
   cmd->n = n;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, v, v_size);
}


/* VertexAttribs1dvNV: marshalled asynchronously */
struct marshal_cmd_VertexAttribs1dvNV
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLsizei n;
   /* Next safe_mul(n, 1 * sizeof(GLdouble)) bytes are GLdouble v[n] */
};
void
_mesa_unmarshal_VertexAttribs1dvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttribs1dvNV *cmd)
{
   GLuint index = cmd->index;
   GLsizei n = cmd->n;
   GLdouble * v;
   const char *variable_data = (const char *) (cmd + 1);
   v = (GLdouble *) variable_data;
   CALL_VertexAttribs1dvNV(ctx->CurrentServerDispatch, (index, n, v));
}
void GLAPIENTRY
_mesa_marshal_VertexAttribs1dvNV(GLuint index, GLsizei n, const GLdouble * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int v_size = safe_mul(n, 1 * sizeof(GLdouble));
   int cmd_size = sizeof(struct marshal_cmd_VertexAttribs1dvNV) + v_size;
   struct marshal_cmd_VertexAttribs1dvNV *cmd;
   if (unlikely(v_size < 0 || (v_size > 0 && !v) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "VertexAttribs1dvNV");
      CALL_VertexAttribs1dvNV(ctx->CurrentServerDispatch, (index, n, v));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttribs1dvNV, cmd_size);
   cmd->index = index;
   cmd->n = n;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, v, v_size);
}


/* VertexAttribs2dvNV: marshalled asynchronously */
struct marshal_cmd_VertexAttribs2dvNV
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLsizei n;
   /* Next safe_mul(n, 2 * sizeof(GLdouble)) bytes are GLdouble v[n][2] */
};
void
_mesa_unmarshal_VertexAttribs2dvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttribs2dvNV *cmd)
{
   GLuint index = cmd->index;
   GLsizei n = cmd->n;
   GLdouble * v;
   const char *variable_data = (const char *) (cmd + 1);
   v = (GLdouble *) variable_data;
   CALL_VertexAttribs2dvNV(ctx->CurrentServerDispatch, (index, n, v));
}
void GLAPIENTRY
_mesa_marshal_VertexAttribs2dvNV(GLuint index, GLsizei n, const GLdouble * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int v_size = safe_mul(n, 2 * sizeof(GLdouble));
   int cmd_size = sizeof(struct marshal_cmd_VertexAttribs2dvNV) + v_size;
   struct marshal_cmd_VertexAttribs2dvNV *cmd;
   if (unlikely(v_size < 0 || (v_size > 0 && !v) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "VertexAttribs2dvNV");
      CALL_VertexAttribs2dvNV(ctx->CurrentServerDispatch, (index, n, v));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttribs2dvNV, cmd_size);
   cmd->index = index;
   cmd->n = n;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, v, v_size);
}


/* VertexAttribs3dvNV: marshalled asynchronously */
struct marshal_cmd_VertexAttribs3dvNV
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLsizei n;
   /* Next safe_mul(n, 3 * sizeof(GLdouble)) bytes are GLdouble v[n][3] */
};
void
_mesa_unmarshal_VertexAttribs3dvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttribs3dvNV *cmd)
{
   GLuint index = cmd->index;
   GLsizei n = cmd->n;
   GLdouble * v;
   const char *variable_data = (const char *) (cmd + 1);
   v = (GLdouble *) variable_data;
   CALL_VertexAttribs3dvNV(ctx->CurrentServerDispatch, (index, n, v));
}
void GLAPIENTRY
_mesa_marshal_VertexAttribs3dvNV(GLuint index, GLsizei n, const GLdouble * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int v_size = safe_mul(n, 3 * sizeof(GLdouble));
   int cmd_size = sizeof(struct marshal_cmd_VertexAttribs3dvNV) + v_size;
   struct marshal_cmd_VertexAttribs3dvNV *cmd;
   if (unlikely(v_size < 0 || (v_size > 0 && !v) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "VertexAttribs3dvNV");
      CALL_VertexAttribs3dvNV(ctx->CurrentServerDispatch, (index, n, v));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttribs3dvNV, cmd_size);
   cmd->index = index;
   cmd->n = n;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, v, v_size);
}


/* VertexAttribs4dvNV: marshalled asynchronously */
struct marshal_cmd_VertexAttribs4dvNV
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLsizei n;
   /* Next safe_mul(n, 4 * sizeof(GLdouble)) bytes are GLdouble v[n][4] */
};
void
_mesa_unmarshal_VertexAttribs4dvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttribs4dvNV *cmd)
{
   GLuint index = cmd->index;
   GLsizei n = cmd->n;
   GLdouble * v;
   const char *variable_data = (const char *) (cmd + 1);
   v = (GLdouble *) variable_data;
   CALL_VertexAttribs4dvNV(ctx->CurrentServerDispatch, (index, n, v));
}
void GLAPIENTRY
_mesa_marshal_VertexAttribs4dvNV(GLuint index, GLsizei n, const GLdouble * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int v_size = safe_mul(n, 4 * sizeof(GLdouble));
   int cmd_size = sizeof(struct marshal_cmd_VertexAttribs4dvNV) + v_size;
   struct marshal_cmd_VertexAttribs4dvNV *cmd;
   if (unlikely(v_size < 0 || (v_size > 0 && !v) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "VertexAttribs4dvNV");
      CALL_VertexAttribs4dvNV(ctx->CurrentServerDispatch, (index, n, v));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttribs4dvNV, cmd_size);
   cmd->index = index;
   cmd->n = n;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, v, v_size);
}


/* VertexAttribs4ubvNV: marshalled asynchronously */
struct marshal_cmd_VertexAttribs4ubvNV
{
   struct marshal_cmd_base cmd_base;
   GLuint index;
   GLsizei n;
   /* Next safe_mul(n, 4 * sizeof(GLubyte)) bytes are GLubyte v[n][4] */
};
void
_mesa_unmarshal_VertexAttribs4ubvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttribs4ubvNV *cmd)
{
   GLuint index = cmd->index;
   GLsizei n = cmd->n;
   GLubyte * v;
   const char *variable_data = (const char *) (cmd + 1);
   v = (GLubyte *) variable_data;
   CALL_VertexAttribs4ubvNV(ctx->CurrentServerDispatch, (index, n, v));
}
void GLAPIENTRY
_mesa_marshal_VertexAttribs4ubvNV(GLuint index, GLsizei n, const GLubyte * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int v_size = safe_mul(n, 4 * sizeof(GLubyte));
   int cmd_size = sizeof(struct marshal_cmd_VertexAttribs4ubvNV) + v_size;
   struct marshal_cmd_VertexAttribs4ubvNV *cmd;
   if (unlikely(v_size < 0 || (v_size > 0 && !v) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "VertexAttribs4ubvNV");
      CALL_VertexAttribs4ubvNV(ctx->CurrentServerDispatch, (index, n, v));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexAttribs4ubvNV, cmd_size);
   cmd->index = index;
   cmd->n = n;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, v, v_size);
}


/* GenFragmentShadersATI: marshalled synchronously */
GLuint GLAPIENTRY
_mesa_marshal_GenFragmentShadersATI(GLuint range)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GenFragmentShadersATI");
   return CALL_GenFragmentShadersATI(ctx->CurrentServerDispatch, (range));
}


/* BindFragmentShaderATI: marshalled asynchronously */
struct marshal_cmd_BindFragmentShaderATI
{
   struct marshal_cmd_base cmd_base;
   GLuint id;
};
void
_mesa_unmarshal_BindFragmentShaderATI(struct gl_context *ctx, const struct marshal_cmd_BindFragmentShaderATI *cmd)
{
   GLuint id = cmd->id;
   CALL_BindFragmentShaderATI(ctx->CurrentServerDispatch, (id));
}
void GLAPIENTRY
_mesa_marshal_BindFragmentShaderATI(GLuint id)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_BindFragmentShaderATI);
   struct marshal_cmd_BindFragmentShaderATI *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_BindFragmentShaderATI, cmd_size);
   cmd->id = id;
}


/* DeleteFragmentShaderATI: marshalled asynchronously */
struct marshal_cmd_DeleteFragmentShaderATI
{
   struct marshal_cmd_base cmd_base;
   GLuint id;
};
void
_mesa_unmarshal_DeleteFragmentShaderATI(struct gl_context *ctx, const struct marshal_cmd_DeleteFragmentShaderATI *cmd)
{
   GLuint id = cmd->id;
   CALL_DeleteFragmentShaderATI(ctx->CurrentServerDispatch, (id));
}
void GLAPIENTRY
_mesa_marshal_DeleteFragmentShaderATI(GLuint id)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_DeleteFragmentShaderATI);
   struct marshal_cmd_DeleteFragmentShaderATI *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_DeleteFragmentShaderATI, cmd_size);
   cmd->id = id;
}


/* BeginFragmentShaderATI: marshalled asynchronously */
struct marshal_cmd_BeginFragmentShaderATI
{
   struct marshal_cmd_base cmd_base;
};
void
_mesa_unmarshal_BeginFragmentShaderATI(struct gl_context *ctx, const struct marshal_cmd_BeginFragmentShaderATI *cmd)
{
   CALL_BeginFragmentShaderATI(ctx->CurrentServerDispatch, ());
}
void GLAPIENTRY
_mesa_marshal_BeginFragmentShaderATI(void)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_BeginFragmentShaderATI);
   struct marshal_cmd_BeginFragmentShaderATI *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_BeginFragmentShaderATI, cmd_size);
   (void) cmd;
}


/* EndFragmentShaderATI: marshalled asynchronously */
struct marshal_cmd_EndFragmentShaderATI
{
   struct marshal_cmd_base cmd_base;
};
void
_mesa_unmarshal_EndFragmentShaderATI(struct gl_context *ctx, const struct marshal_cmd_EndFragmentShaderATI *cmd)
{
   CALL_EndFragmentShaderATI(ctx->CurrentServerDispatch, ());
}
void GLAPIENTRY
_mesa_marshal_EndFragmentShaderATI(void)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_EndFragmentShaderATI);
   struct marshal_cmd_EndFragmentShaderATI *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_EndFragmentShaderATI, cmd_size);
   (void) cmd;
}


/* PassTexCoordATI: marshalled asynchronously */
struct marshal_cmd_PassTexCoordATI
{
   struct marshal_cmd_base cmd_base;
   GLuint dst;
   GLuint coord;
   GLenum swizzle;
};
void
_mesa_unmarshal_PassTexCoordATI(struct gl_context *ctx, const struct marshal_cmd_PassTexCoordATI *cmd)
{
   GLuint dst = cmd->dst;
   GLuint coord = cmd->coord;
   GLenum swizzle = cmd->swizzle;
   CALL_PassTexCoordATI(ctx->CurrentServerDispatch, (dst, coord, swizzle));
}
void GLAPIENTRY
_mesa_marshal_PassTexCoordATI(GLuint dst, GLuint coord, GLenum swizzle)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_PassTexCoordATI);
   struct marshal_cmd_PassTexCoordATI *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_PassTexCoordATI, cmd_size);
   cmd->dst = dst;
   cmd->coord = coord;
   cmd->swizzle = swizzle;
}


/* SampleMapATI: marshalled asynchronously */
struct marshal_cmd_SampleMapATI
{
   struct marshal_cmd_base cmd_base;
   GLuint dst;
   GLuint interp;
   GLenum swizzle;
};
void
_mesa_unmarshal_SampleMapATI(struct gl_context *ctx, const struct marshal_cmd_SampleMapATI *cmd)
{
   GLuint dst = cmd->dst;
   GLuint interp = cmd->interp;
   GLenum swizzle = cmd->swizzle;
   CALL_SampleMapATI(ctx->CurrentServerDispatch, (dst, interp, swizzle));
}
void GLAPIENTRY
_mesa_marshal_SampleMapATI(GLuint dst, GLuint interp, GLenum swizzle)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_SampleMapATI);
   struct marshal_cmd_SampleMapATI *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_SampleMapATI, cmd_size);
   cmd->dst = dst;
   cmd->interp = interp;
   cmd->swizzle = swizzle;
}


/* ColorFragmentOp1ATI: marshalled asynchronously */
struct marshal_cmd_ColorFragmentOp1ATI
{
   struct marshal_cmd_base cmd_base;
   GLenum op;
   GLuint dst;
   GLuint dstMask;
   GLuint dstMod;
   GLuint arg1;
   GLuint arg1Rep;
   GLuint arg1Mod;
};
void
_mesa_unmarshal_ColorFragmentOp1ATI(struct gl_context *ctx, const struct marshal_cmd_ColorFragmentOp1ATI *cmd)
{
   GLenum op = cmd->op;
   GLuint dst = cmd->dst;
   GLuint dstMask = cmd->dstMask;
   GLuint dstMod = cmd->dstMod;
   GLuint arg1 = cmd->arg1;
   GLuint arg1Rep = cmd->arg1Rep;
   GLuint arg1Mod = cmd->arg1Mod;
   CALL_ColorFragmentOp1ATI(ctx->CurrentServerDispatch, (op, dst, dstMask, dstMod, arg1, arg1Rep, arg1Mod));
}
void GLAPIENTRY
_mesa_marshal_ColorFragmentOp1ATI(GLenum op, GLuint dst, GLuint dstMask, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ColorFragmentOp1ATI);
   struct marshal_cmd_ColorFragmentOp1ATI *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ColorFragmentOp1ATI, cmd_size);
   cmd->op = op;
   cmd->dst = dst;
   cmd->dstMask = dstMask;
   cmd->dstMod = dstMod;
   cmd->arg1 = arg1;
   cmd->arg1Rep = arg1Rep;
   cmd->arg1Mod = arg1Mod;
}


/* ColorFragmentOp2ATI: marshalled asynchronously */
struct marshal_cmd_ColorFragmentOp2ATI
{
   struct marshal_cmd_base cmd_base;
   GLenum op;
   GLuint dst;
   GLuint dstMask;
   GLuint dstMod;
   GLuint arg1;
   GLuint arg1Rep;
   GLuint arg1Mod;
   GLuint arg2;
   GLuint arg2Rep;
   GLuint arg2Mod;
};
void
_mesa_unmarshal_ColorFragmentOp2ATI(struct gl_context *ctx, const struct marshal_cmd_ColorFragmentOp2ATI *cmd)
{
   GLenum op = cmd->op;
   GLuint dst = cmd->dst;
   GLuint dstMask = cmd->dstMask;
   GLuint dstMod = cmd->dstMod;
   GLuint arg1 = cmd->arg1;
   GLuint arg1Rep = cmd->arg1Rep;
   GLuint arg1Mod = cmd->arg1Mod;
   GLuint arg2 = cmd->arg2;
   GLuint arg2Rep = cmd->arg2Rep;
   GLuint arg2Mod = cmd->arg2Mod;
   CALL_ColorFragmentOp2ATI(ctx->CurrentServerDispatch, (op, dst, dstMask, dstMod, arg1, arg1Rep, arg1Mod, arg2, arg2Rep, arg2Mod));
}
void GLAPIENTRY
_mesa_marshal_ColorFragmentOp2ATI(GLenum op, GLuint dst, GLuint dstMask, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod, GLuint arg2, GLuint arg2Rep, GLuint arg2Mod)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ColorFragmentOp2ATI);
   struct marshal_cmd_ColorFragmentOp2ATI *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ColorFragmentOp2ATI, cmd_size);
   cmd->op = op;
   cmd->dst = dst;
   cmd->dstMask = dstMask;
   cmd->dstMod = dstMod;
   cmd->arg1 = arg1;
   cmd->arg1Rep = arg1Rep;
   cmd->arg1Mod = arg1Mod;
   cmd->arg2 = arg2;
   cmd->arg2Rep = arg2Rep;
   cmd->arg2Mod = arg2Mod;
}


/* ColorFragmentOp3ATI: marshalled asynchronously */
struct marshal_cmd_ColorFragmentOp3ATI
{
   struct marshal_cmd_base cmd_base;
   GLenum op;
   GLuint dst;
   GLuint dstMask;
   GLuint dstMod;
   GLuint arg1;
   GLuint arg1Rep;
   GLuint arg1Mod;
   GLuint arg2;
   GLuint arg2Rep;
   GLuint arg2Mod;
   GLuint arg3;
   GLuint arg3Rep;
   GLuint arg3Mod;
};
void
_mesa_unmarshal_ColorFragmentOp3ATI(struct gl_context *ctx, const struct marshal_cmd_ColorFragmentOp3ATI *cmd)
{
   GLenum op = cmd->op;
   GLuint dst = cmd->dst;
   GLuint dstMask = cmd->dstMask;
   GLuint dstMod = cmd->dstMod;
   GLuint arg1 = cmd->arg1;
   GLuint arg1Rep = cmd->arg1Rep;
   GLuint arg1Mod = cmd->arg1Mod;
   GLuint arg2 = cmd->arg2;
   GLuint arg2Rep = cmd->arg2Rep;
   GLuint arg2Mod = cmd->arg2Mod;
   GLuint arg3 = cmd->arg3;
   GLuint arg3Rep = cmd->arg3Rep;
   GLuint arg3Mod = cmd->arg3Mod;
   CALL_ColorFragmentOp3ATI(ctx->CurrentServerDispatch, (op, dst, dstMask, dstMod, arg1, arg1Rep, arg1Mod, arg2, arg2Rep, arg2Mod, arg3, arg3Rep, arg3Mod));
}
void GLAPIENTRY
_mesa_marshal_ColorFragmentOp3ATI(GLenum op, GLuint dst, GLuint dstMask, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod, GLuint arg2, GLuint arg2Rep, GLuint arg2Mod, GLuint arg3, GLuint arg3Rep, GLuint arg3Mod)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ColorFragmentOp3ATI);
   struct marshal_cmd_ColorFragmentOp3ATI *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ColorFragmentOp3ATI, cmd_size);
   cmd->op = op;
   cmd->dst = dst;
   cmd->dstMask = dstMask;
   cmd->dstMod = dstMod;
   cmd->arg1 = arg1;
   cmd->arg1Rep = arg1Rep;
   cmd->arg1Mod = arg1Mod;
   cmd->arg2 = arg2;
   cmd->arg2Rep = arg2Rep;
   cmd->arg2Mod = arg2Mod;
   cmd->arg3 = arg3;
   cmd->arg3Rep = arg3Rep;
   cmd->arg3Mod = arg3Mod;
}


/* AlphaFragmentOp1ATI: marshalled asynchronously */
struct marshal_cmd_AlphaFragmentOp1ATI
{
   struct marshal_cmd_base cmd_base;
   GLenum op;
   GLuint dst;
   GLuint dstMod;
   GLuint arg1;
   GLuint arg1Rep;
   GLuint arg1Mod;
};
void
_mesa_unmarshal_AlphaFragmentOp1ATI(struct gl_context *ctx, const struct marshal_cmd_AlphaFragmentOp1ATI *cmd)
{
   GLenum op = cmd->op;
   GLuint dst = cmd->dst;
   GLuint dstMod = cmd->dstMod;
   GLuint arg1 = cmd->arg1;
   GLuint arg1Rep = cmd->arg1Rep;
   GLuint arg1Mod = cmd->arg1Mod;
   CALL_AlphaFragmentOp1ATI(ctx->CurrentServerDispatch, (op, dst, dstMod, arg1, arg1Rep, arg1Mod));
}
void GLAPIENTRY
_mesa_marshal_AlphaFragmentOp1ATI(GLenum op, GLuint dst, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_AlphaFragmentOp1ATI);
   struct marshal_cmd_AlphaFragmentOp1ATI *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_AlphaFragmentOp1ATI, cmd_size);
   cmd->op = op;
   cmd->dst = dst;
   cmd->dstMod = dstMod;
   cmd->arg1 = arg1;
   cmd->arg1Rep = arg1Rep;
   cmd->arg1Mod = arg1Mod;
}


/* AlphaFragmentOp2ATI: marshalled asynchronously */
struct marshal_cmd_AlphaFragmentOp2ATI
{
   struct marshal_cmd_base cmd_base;
   GLenum op;
   GLuint dst;
   GLuint dstMod;
   GLuint arg1;
   GLuint arg1Rep;
   GLuint arg1Mod;
   GLuint arg2;
   GLuint arg2Rep;
   GLuint arg2Mod;
};
void
_mesa_unmarshal_AlphaFragmentOp2ATI(struct gl_context *ctx, const struct marshal_cmd_AlphaFragmentOp2ATI *cmd)
{
   GLenum op = cmd->op;
   GLuint dst = cmd->dst;
   GLuint dstMod = cmd->dstMod;
   GLuint arg1 = cmd->arg1;
   GLuint arg1Rep = cmd->arg1Rep;
   GLuint arg1Mod = cmd->arg1Mod;
   GLuint arg2 = cmd->arg2;
   GLuint arg2Rep = cmd->arg2Rep;
   GLuint arg2Mod = cmd->arg2Mod;
   CALL_AlphaFragmentOp2ATI(ctx->CurrentServerDispatch, (op, dst, dstMod, arg1, arg1Rep, arg1Mod, arg2, arg2Rep, arg2Mod));
}
void GLAPIENTRY
_mesa_marshal_AlphaFragmentOp2ATI(GLenum op, GLuint dst, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod, GLuint arg2, GLuint arg2Rep, GLuint arg2Mod)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_AlphaFragmentOp2ATI);
   struct marshal_cmd_AlphaFragmentOp2ATI *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_AlphaFragmentOp2ATI, cmd_size);
   cmd->op = op;
   cmd->dst = dst;
   cmd->dstMod = dstMod;
   cmd->arg1 = arg1;
   cmd->arg1Rep = arg1Rep;
   cmd->arg1Mod = arg1Mod;
   cmd->arg2 = arg2;
   cmd->arg2Rep = arg2Rep;
   cmd->arg2Mod = arg2Mod;
}


/* AlphaFragmentOp3ATI: marshalled asynchronously */
struct marshal_cmd_AlphaFragmentOp3ATI
{
   struct marshal_cmd_base cmd_base;
   GLenum op;
   GLuint dst;
   GLuint dstMod;
   GLuint arg1;
   GLuint arg1Rep;
   GLuint arg1Mod;
   GLuint arg2;
   GLuint arg2Rep;
   GLuint arg2Mod;
   GLuint arg3;
   GLuint arg3Rep;
   GLuint arg3Mod;
};
void
_mesa_unmarshal_AlphaFragmentOp3ATI(struct gl_context *ctx, const struct marshal_cmd_AlphaFragmentOp3ATI *cmd)
{
   GLenum op = cmd->op;
   GLuint dst = cmd->dst;
   GLuint dstMod = cmd->dstMod;
   GLuint arg1 = cmd->arg1;
   GLuint arg1Rep = cmd->arg1Rep;
   GLuint arg1Mod = cmd->arg1Mod;
   GLuint arg2 = cmd->arg2;
   GLuint arg2Rep = cmd->arg2Rep;
   GLuint arg2Mod = cmd->arg2Mod;
   GLuint arg3 = cmd->arg3;
   GLuint arg3Rep = cmd->arg3Rep;
   GLuint arg3Mod = cmd->arg3Mod;
   CALL_AlphaFragmentOp3ATI(ctx->CurrentServerDispatch, (op, dst, dstMod, arg1, arg1Rep, arg1Mod, arg2, arg2Rep, arg2Mod, arg3, arg3Rep, arg3Mod));
}
void GLAPIENTRY
_mesa_marshal_AlphaFragmentOp3ATI(GLenum op, GLuint dst, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod, GLuint arg2, GLuint arg2Rep, GLuint arg2Mod, GLuint arg3, GLuint arg3Rep, GLuint arg3Mod)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_AlphaFragmentOp3ATI);
   struct marshal_cmd_AlphaFragmentOp3ATI *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_AlphaFragmentOp3ATI, cmd_size);
   cmd->op = op;
   cmd->dst = dst;
   cmd->dstMod = dstMod;
   cmd->arg1 = arg1;
   cmd->arg1Rep = arg1Rep;
   cmd->arg1Mod = arg1Mod;
   cmd->arg2 = arg2;
   cmd->arg2Rep = arg2Rep;
   cmd->arg2Mod = arg2Mod;
   cmd->arg3 = arg3;
   cmd->arg3Rep = arg3Rep;
   cmd->arg3Mod = arg3Mod;
}


/* SetFragmentShaderConstantATI: marshalled asynchronously */
struct marshal_cmd_SetFragmentShaderConstantATI
{
   struct marshal_cmd_base cmd_base;
   GLuint dst;
   GLfloat value[4];
};
void
_mesa_unmarshal_SetFragmentShaderConstantATI(struct gl_context *ctx, const struct marshal_cmd_SetFragmentShaderConstantATI *cmd)
{
   GLuint dst = cmd->dst;
   const GLfloat * value = cmd->value;
   CALL_SetFragmentShaderConstantATI(ctx->CurrentServerDispatch, (dst, value));
}
void GLAPIENTRY
_mesa_marshal_SetFragmentShaderConstantATI(GLuint dst, const GLfloat * value)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_SetFragmentShaderConstantATI);
   struct marshal_cmd_SetFragmentShaderConstantATI *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_SetFragmentShaderConstantATI, cmd_size);
   cmd->dst = dst;
   memcpy(cmd->value, value, 4 * sizeof(GLfloat));
}


/* ActiveStencilFaceEXT: marshalled asynchronously */
struct marshal_cmd_ActiveStencilFaceEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum face;
};
void
_mesa_unmarshal_ActiveStencilFaceEXT(struct gl_context *ctx, const struct marshal_cmd_ActiveStencilFaceEXT *cmd)
{
   GLenum face = cmd->face;
   CALL_ActiveStencilFaceEXT(ctx->CurrentServerDispatch, (face));
}
void GLAPIENTRY
_mesa_marshal_ActiveStencilFaceEXT(GLenum face)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ActiveStencilFaceEXT);
   struct marshal_cmd_ActiveStencilFaceEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ActiveStencilFaceEXT, cmd_size);
   cmd->face = face;
}


/* ObjectPurgeableAPPLE: marshalled synchronously */
GLenum GLAPIENTRY
_mesa_marshal_ObjectPurgeableAPPLE(GLenum objectType, GLuint name, GLenum option)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "ObjectPurgeableAPPLE");
   return CALL_ObjectPurgeableAPPLE(ctx->CurrentServerDispatch, (objectType, name, option));
}


/* ObjectUnpurgeableAPPLE: marshalled synchronously */
GLenum GLAPIENTRY
_mesa_marshal_ObjectUnpurgeableAPPLE(GLenum objectType, GLuint name, GLenum option)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "ObjectUnpurgeableAPPLE");
   return CALL_ObjectUnpurgeableAPPLE(ctx->CurrentServerDispatch, (objectType, name, option));
}


/* GetObjectParameterivAPPLE: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetObjectParameterivAPPLE(GLenum objectType, GLuint name, GLenum pname, GLint * value)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetObjectParameterivAPPLE");
   CALL_GetObjectParameterivAPPLE(ctx->CurrentServerDispatch, (objectType, name, pname, value));
}


/* DepthBoundsEXT: marshalled asynchronously */
struct marshal_cmd_DepthBoundsEXT
{
   struct marshal_cmd_base cmd_base;
   GLclampd zmin;
   GLclampd zmax;
};
void
_mesa_unmarshal_DepthBoundsEXT(struct gl_context *ctx, const struct marshal_cmd_DepthBoundsEXT *cmd)
{
   GLclampd zmin = cmd->zmin;
   GLclampd zmax = cmd->zmax;
   CALL_DepthBoundsEXT(ctx->CurrentServerDispatch, (zmin, zmax));
}
void GLAPIENTRY
_mesa_marshal_DepthBoundsEXT(GLclampd zmin, GLclampd zmax)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_DepthBoundsEXT);
   struct marshal_cmd_DepthBoundsEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_DepthBoundsEXT, cmd_size);
   cmd->zmin = zmin;
   cmd->zmax = zmax;
}


/* BindRenderbufferEXT: marshalled asynchronously */
struct marshal_cmd_BindRenderbufferEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum target;
   GLuint renderbuffer;
};
void
_mesa_unmarshal_BindRenderbufferEXT(struct gl_context *ctx, const struct marshal_cmd_BindRenderbufferEXT *cmd)
{
   GLenum target = cmd->target;
   GLuint renderbuffer = cmd->renderbuffer;
   CALL_BindRenderbufferEXT(ctx->CurrentServerDispatch, (target, renderbuffer));
}
void GLAPIENTRY
_mesa_marshal_BindRenderbufferEXT(GLenum target, GLuint renderbuffer)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_BindRenderbufferEXT);
   struct marshal_cmd_BindRenderbufferEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_BindRenderbufferEXT, cmd_size);
   cmd->target = target;
   cmd->renderbuffer = renderbuffer;
}


/* BindFramebufferEXT: marshalled asynchronously */
struct marshal_cmd_BindFramebufferEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum target;
   GLuint framebuffer;
};
void
_mesa_unmarshal_BindFramebufferEXT(struct gl_context *ctx, const struct marshal_cmd_BindFramebufferEXT *cmd)
{
   GLenum target = cmd->target;
   GLuint framebuffer = cmd->framebuffer;
   CALL_BindFramebufferEXT(ctx->CurrentServerDispatch, (target, framebuffer));
}
void GLAPIENTRY
_mesa_marshal_BindFramebufferEXT(GLenum target, GLuint framebuffer)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_BindFramebufferEXT);
   struct marshal_cmd_BindFramebufferEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_BindFramebufferEXT, cmd_size);
   cmd->target = target;
   cmd->framebuffer = framebuffer;
}


/* StringMarkerGREMEDY: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_StringMarkerGREMEDY(GLsizei len, const GLvoid * string)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "StringMarkerGREMEDY");
   CALL_StringMarkerGREMEDY(ctx->CurrentServerDispatch, (len, string));
}


/* ProvokingVertex: marshalled asynchronously */
struct marshal_cmd_ProvokingVertex
{
   struct marshal_cmd_base cmd_base;
   GLenum mode;
};
void
_mesa_unmarshal_ProvokingVertex(struct gl_context *ctx, const struct marshal_cmd_ProvokingVertex *cmd)
{
   GLenum mode = cmd->mode;
   CALL_ProvokingVertex(ctx->CurrentServerDispatch, (mode));
}
void GLAPIENTRY
_mesa_marshal_ProvokingVertex(GLenum mode)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ProvokingVertex);
   struct marshal_cmd_ProvokingVertex *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ProvokingVertex, cmd_size);
   cmd->mode = mode;
}


/* ColorMaski: marshalled asynchronously */
struct marshal_cmd_ColorMaski
{
   struct marshal_cmd_base cmd_base;
   GLboolean r;
   GLboolean g;
   GLboolean b;
   GLboolean a;
   GLuint buf;
};
void
_mesa_unmarshal_ColorMaski(struct gl_context *ctx, const struct marshal_cmd_ColorMaski *cmd)
{
   GLuint buf = cmd->buf;
   GLboolean r = cmd->r;
   GLboolean g = cmd->g;
   GLboolean b = cmd->b;
   GLboolean a = cmd->a;
   CALL_ColorMaski(ctx->CurrentServerDispatch, (buf, r, g, b, a));
}
void GLAPIENTRY
_mesa_marshal_ColorMaski(GLuint buf, GLboolean r, GLboolean g, GLboolean b, GLboolean a)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_ColorMaski);
   struct marshal_cmd_ColorMaski *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_ColorMaski, cmd_size);
   cmd->buf = buf;
   cmd->r = r;
   cmd->g = g;
   cmd->b = b;
   cmd->a = a;
}


/* GetBooleani_v: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetBooleani_v(GLenum value, GLuint index, GLboolean * data)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetBooleani_v");
   CALL_GetBooleani_v(ctx->CurrentServerDispatch, (value, index, data));
}


/* GetIntegeri_v: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetIntegeri_v(GLenum value, GLuint index, GLint * data)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetIntegeri_v");
   CALL_GetIntegeri_v(ctx->CurrentServerDispatch, (value, index, data));
}


/* Enablei: marshalled asynchronously */
struct marshal_cmd_Enablei
{
   struct marshal_cmd_base cmd_base;
   GLenum target;
   GLuint index;
};
void
_mesa_unmarshal_Enablei(struct gl_context *ctx, const struct marshal_cmd_Enablei *cmd)
{
   GLenum target = cmd->target;
   GLuint index = cmd->index;
   CALL_Enablei(ctx->CurrentServerDispatch, (target, index));
}
void GLAPIENTRY
_mesa_marshal_Enablei(GLenum target, GLuint index)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Enablei);
   struct marshal_cmd_Enablei *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Enablei, cmd_size);
   cmd->target = target;
   cmd->index = index;
}


/* Disablei: marshalled asynchronously */
struct marshal_cmd_Disablei
{
   struct marshal_cmd_base cmd_base;
   GLenum target;
   GLuint index;
};
void
_mesa_unmarshal_Disablei(struct gl_context *ctx, const struct marshal_cmd_Disablei *cmd)
{
   GLenum target = cmd->target;
   GLuint index = cmd->index;
   CALL_Disablei(ctx->CurrentServerDispatch, (target, index));
}
void GLAPIENTRY
_mesa_marshal_Disablei(GLenum target, GLuint index)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_Disablei);
   struct marshal_cmd_Disablei *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_Disablei, cmd_size);
   cmd->target = target;
   cmd->index = index;
}


/* IsEnabledi: marshalled synchronously */
GLboolean GLAPIENTRY
_mesa_marshal_IsEnabledi(GLenum target, GLuint index)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "IsEnabledi");
   return CALL_IsEnabledi(ctx->CurrentServerDispatch, (target, index));
}


/* GetPerfMonitorGroupsAMD: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetPerfMonitorGroupsAMD(GLint * numGroups, GLsizei groupsSize, GLuint * groups)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetPerfMonitorGroupsAMD");
   CALL_GetPerfMonitorGroupsAMD(ctx->CurrentServerDispatch, (numGroups, groupsSize, groups));
}


/* GetPerfMonitorCountersAMD: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetPerfMonitorCountersAMD(GLuint group, GLint * numCounters, GLint * maxActiveCounters, GLsizei countersSize, GLuint * counters)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetPerfMonitorCountersAMD");
   CALL_GetPerfMonitorCountersAMD(ctx->CurrentServerDispatch, (group, numCounters, maxActiveCounters, countersSize, counters));
}


/* GetPerfMonitorGroupStringAMD: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetPerfMonitorGroupStringAMD(GLuint group, GLsizei bufSize, GLsizei * length, GLchar * groupString)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetPerfMonitorGroupStringAMD");
   CALL_GetPerfMonitorGroupStringAMD(ctx->CurrentServerDispatch, (group, bufSize, length, groupString));
}


/* GetPerfMonitorCounterStringAMD: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetPerfMonitorCounterStringAMD(GLuint group, GLuint counter, GLsizei bufSize, GLsizei * length, GLchar * counterString)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetPerfMonitorCounterStringAMD");
   CALL_GetPerfMonitorCounterStringAMD(ctx->CurrentServerDispatch, (group, counter, bufSize, length, counterString));
}


/* GetPerfMonitorCounterInfoAMD: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetPerfMonitorCounterInfoAMD(GLuint group, GLuint counter, GLenum pname, GLvoid * data)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetPerfMonitorCounterInfoAMD");
   CALL_GetPerfMonitorCounterInfoAMD(ctx->CurrentServerDispatch, (group, counter, pname, data));
}


/* GenPerfMonitorsAMD: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GenPerfMonitorsAMD(GLsizei n, GLuint * monitors)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GenPerfMonitorsAMD");
   CALL_GenPerfMonitorsAMD(ctx->CurrentServerDispatch, (n, monitors));
}


/* DeletePerfMonitorsAMD: marshalled asynchronously */
struct marshal_cmd_DeletePerfMonitorsAMD
{
   struct marshal_cmd_base cmd_base;
   GLsizei n;
   /* Next safe_mul(n, 1 * sizeof(GLuint)) bytes are GLuint monitors[n] */
};
void
_mesa_unmarshal_DeletePerfMonitorsAMD(struct gl_context *ctx, const struct marshal_cmd_DeletePerfMonitorsAMD *cmd)
{
   GLsizei n = cmd->n;
   GLuint * monitors;
   const char *variable_data = (const char *) (cmd + 1);
   monitors = (GLuint *) variable_data;
   CALL_DeletePerfMonitorsAMD(ctx->CurrentServerDispatch, (n, monitors));
}
void GLAPIENTRY
_mesa_marshal_DeletePerfMonitorsAMD(GLsizei n, GLuint * monitors)
{
   GET_CURRENT_CONTEXT(ctx);
   int monitors_size = safe_mul(n, 1 * sizeof(GLuint));
   int cmd_size = sizeof(struct marshal_cmd_DeletePerfMonitorsAMD) + monitors_size;
   struct marshal_cmd_DeletePerfMonitorsAMD *cmd;
   if (unlikely(monitors_size < 0 || (monitors_size > 0 && !monitors) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "DeletePerfMonitorsAMD");
      CALL_DeletePerfMonitorsAMD(ctx->CurrentServerDispatch, (n, monitors));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_DeletePerfMonitorsAMD, cmd_size);
   cmd->n = n;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, monitors, monitors_size);
}


/* SelectPerfMonitorCountersAMD: marshalled asynchronously */
struct marshal_cmd_SelectPerfMonitorCountersAMD
{
   struct marshal_cmd_base cmd_base;
   GLboolean enable;
   GLuint monitor;
   GLuint group;
   GLint numCounters;
   /* Next safe_mul(numCounters, 1 * sizeof(GLuint)) bytes are GLuint counterList[numCounters] */
};
void
_mesa_unmarshal_SelectPerfMonitorCountersAMD(struct gl_context *ctx, const struct marshal_cmd_SelectPerfMonitorCountersAMD *cmd)
{
   GLuint monitor = cmd->monitor;
   GLboolean enable = cmd->enable;
   GLuint group = cmd->group;
   GLint numCounters = cmd->numCounters;
   GLuint * counterList;
   const char *variable_data = (const char *) (cmd + 1);
   counterList = (GLuint *) variable_data;
   CALL_SelectPerfMonitorCountersAMD(ctx->CurrentServerDispatch, (monitor, enable, group, numCounters, counterList));
}
void GLAPIENTRY
_mesa_marshal_SelectPerfMonitorCountersAMD(GLuint monitor, GLboolean enable, GLuint group, GLint numCounters, GLuint * counterList)
{
   GET_CURRENT_CONTEXT(ctx);
   int counterList_size = safe_mul(numCounters, 1 * sizeof(GLuint));
   int cmd_size = sizeof(struct marshal_cmd_SelectPerfMonitorCountersAMD) + counterList_size;
   struct marshal_cmd_SelectPerfMonitorCountersAMD *cmd;
   if (unlikely(counterList_size < 0 || (counterList_size > 0 && !counterList) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "SelectPerfMonitorCountersAMD");
      CALL_SelectPerfMonitorCountersAMD(ctx->CurrentServerDispatch, (monitor, enable, group, numCounters, counterList));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_SelectPerfMonitorCountersAMD, cmd_size);
   cmd->monitor = monitor;
   cmd->enable = enable;
   cmd->group = group;
   cmd->numCounters = numCounters;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, counterList, counterList_size);
}


/* BeginPerfMonitorAMD: marshalled asynchronously */
struct marshal_cmd_BeginPerfMonitorAMD
{
   struct marshal_cmd_base cmd_base;
   GLuint monitor;
};
void
_mesa_unmarshal_BeginPerfMonitorAMD(struct gl_context *ctx, const struct marshal_cmd_BeginPerfMonitorAMD *cmd)
{
   GLuint monitor = cmd->monitor;
   CALL_BeginPerfMonitorAMD(ctx->CurrentServerDispatch, (monitor));
}
void GLAPIENTRY
_mesa_marshal_BeginPerfMonitorAMD(GLuint monitor)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_BeginPerfMonitorAMD);
   struct marshal_cmd_BeginPerfMonitorAMD *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_BeginPerfMonitorAMD, cmd_size);
   cmd->monitor = monitor;
}


/* EndPerfMonitorAMD: marshalled asynchronously */
struct marshal_cmd_EndPerfMonitorAMD
{
   struct marshal_cmd_base cmd_base;
   GLuint monitor;
};
void
_mesa_unmarshal_EndPerfMonitorAMD(struct gl_context *ctx, const struct marshal_cmd_EndPerfMonitorAMD *cmd)
{
   GLuint monitor = cmd->monitor;
   CALL_EndPerfMonitorAMD(ctx->CurrentServerDispatch, (monitor));
}
void GLAPIENTRY
_mesa_marshal_EndPerfMonitorAMD(GLuint monitor)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_EndPerfMonitorAMD);
   struct marshal_cmd_EndPerfMonitorAMD *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_EndPerfMonitorAMD, cmd_size);
   cmd->monitor = monitor;
}


/* GetPerfMonitorCounterDataAMD: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetPerfMonitorCounterDataAMD(GLuint monitor, GLenum pname, GLsizei dataSize, GLuint * data, GLint * bytesWritten)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetPerfMonitorCounterDataAMD");
   CALL_GetPerfMonitorCounterDataAMD(ctx->CurrentServerDispatch, (monitor, pname, dataSize, data, bytesWritten));
}


/* CopyImageSubDataNV: marshalled asynchronously */
struct marshal_cmd_CopyImageSubDataNV
{
   struct marshal_cmd_base cmd_base;
   GLuint srcName;
   GLenum srcTarget;
   GLint srcLevel;
   GLint srcX;
   GLint srcY;
   GLint srcZ;
   GLuint dstName;
   GLenum dstTarget;
   GLint dstLevel;
   GLint dstX;
   GLint dstY;
   GLint dstZ;
   GLsizei width;
   GLsizei height;
   GLsizei depth;
};
void
_mesa_unmarshal_CopyImageSubDataNV(struct gl_context *ctx, const struct marshal_cmd_CopyImageSubDataNV *cmd)
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
   GLsizei width = cmd->width;
   GLsizei height = cmd->height;
   GLsizei depth = cmd->depth;
   CALL_CopyImageSubDataNV(ctx->CurrentServerDispatch, (srcName, srcTarget, srcLevel, srcX, srcY, srcZ, dstName, dstTarget, dstLevel, dstX, dstY, dstZ, width, height, depth));
}
void GLAPIENTRY
_mesa_marshal_CopyImageSubDataNV(GLuint srcName, GLenum srcTarget, GLint srcLevel, GLint srcX, GLint srcY, GLint srcZ, GLuint dstName, GLenum dstTarget, GLint dstLevel, GLint dstX, GLint dstY, GLint dstZ, GLsizei width, GLsizei height, GLsizei depth)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_CopyImageSubDataNV);
   struct marshal_cmd_CopyImageSubDataNV *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_CopyImageSubDataNV, cmd_size);
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
   cmd->width = width;
   cmd->height = height;
   cmd->depth = depth;
}


/* MatrixLoadfEXT: marshalled asynchronously */
struct marshal_cmd_MatrixLoadfEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum matrixMode;
   GLfloat m[16];
};
void
_mesa_unmarshal_MatrixLoadfEXT(struct gl_context *ctx, const struct marshal_cmd_MatrixLoadfEXT *cmd)
{
   GLenum matrixMode = cmd->matrixMode;
   const GLfloat * m = cmd->m;
   CALL_MatrixLoadfEXT(ctx->CurrentServerDispatch, (matrixMode, m));
}
void GLAPIENTRY
_mesa_marshal_MatrixLoadfEXT(GLenum matrixMode, const GLfloat * m)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MatrixLoadfEXT);
   struct marshal_cmd_MatrixLoadfEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MatrixLoadfEXT, cmd_size);
   cmd->matrixMode = matrixMode;
   memcpy(cmd->m, m, 16 * sizeof(GLfloat));
}


/* MatrixLoaddEXT: marshalled asynchronously */
struct marshal_cmd_MatrixLoaddEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum matrixMode;
   GLdouble m[16];
};
void
_mesa_unmarshal_MatrixLoaddEXT(struct gl_context *ctx, const struct marshal_cmd_MatrixLoaddEXT *cmd)
{
   GLenum matrixMode = cmd->matrixMode;
   const GLdouble * m = cmd->m;
   CALL_MatrixLoaddEXT(ctx->CurrentServerDispatch, (matrixMode, m));
}
void GLAPIENTRY
_mesa_marshal_MatrixLoaddEXT(GLenum matrixMode, const GLdouble * m)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MatrixLoaddEXT);
   struct marshal_cmd_MatrixLoaddEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MatrixLoaddEXT, cmd_size);
   cmd->matrixMode = matrixMode;
   memcpy(cmd->m, m, 16 * sizeof(GLdouble));
}


/* MatrixMultfEXT: marshalled asynchronously */
struct marshal_cmd_MatrixMultfEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum matrixMode;
   GLfloat m[16];
};
void
_mesa_unmarshal_MatrixMultfEXT(struct gl_context *ctx, const struct marshal_cmd_MatrixMultfEXT *cmd)
{
   GLenum matrixMode = cmd->matrixMode;
   const GLfloat * m = cmd->m;
   CALL_MatrixMultfEXT(ctx->CurrentServerDispatch, (matrixMode, m));
}
void GLAPIENTRY
_mesa_marshal_MatrixMultfEXT(GLenum matrixMode, const GLfloat * m)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MatrixMultfEXT);
   struct marshal_cmd_MatrixMultfEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MatrixMultfEXT, cmd_size);
   cmd->matrixMode = matrixMode;
   memcpy(cmd->m, m, 16 * sizeof(GLfloat));
}


/* MatrixMultdEXT: marshalled asynchronously */
struct marshal_cmd_MatrixMultdEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum matrixMode;
   GLdouble m[16];
};
void
_mesa_unmarshal_MatrixMultdEXT(struct gl_context *ctx, const struct marshal_cmd_MatrixMultdEXT *cmd)
{
   GLenum matrixMode = cmd->matrixMode;
   const GLdouble * m = cmd->m;
   CALL_MatrixMultdEXT(ctx->CurrentServerDispatch, (matrixMode, m));
}
void GLAPIENTRY
_mesa_marshal_MatrixMultdEXT(GLenum matrixMode, const GLdouble * m)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MatrixMultdEXT);
   struct marshal_cmd_MatrixMultdEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MatrixMultdEXT, cmd_size);
   cmd->matrixMode = matrixMode;
   memcpy(cmd->m, m, 16 * sizeof(GLdouble));
}


/* MatrixLoadIdentityEXT: marshalled asynchronously */
struct marshal_cmd_MatrixLoadIdentityEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum matrixMode;
};
void
_mesa_unmarshal_MatrixLoadIdentityEXT(struct gl_context *ctx, const struct marshal_cmd_MatrixLoadIdentityEXT *cmd)
{
   GLenum matrixMode = cmd->matrixMode;
   CALL_MatrixLoadIdentityEXT(ctx->CurrentServerDispatch, (matrixMode));
}
void GLAPIENTRY
_mesa_marshal_MatrixLoadIdentityEXT(GLenum matrixMode)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MatrixLoadIdentityEXT);
   struct marshal_cmd_MatrixLoadIdentityEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MatrixLoadIdentityEXT, cmd_size);
   cmd->matrixMode = matrixMode;
}


/* MatrixRotatefEXT: marshalled asynchronously */
struct marshal_cmd_MatrixRotatefEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum matrixMode;
   GLfloat angle;
   GLfloat x;
   GLfloat y;
   GLfloat z;
};
void
_mesa_unmarshal_MatrixRotatefEXT(struct gl_context *ctx, const struct marshal_cmd_MatrixRotatefEXT *cmd)
{
   GLenum matrixMode = cmd->matrixMode;
   GLfloat angle = cmd->angle;
   GLfloat x = cmd->x;
   GLfloat y = cmd->y;
   GLfloat z = cmd->z;
   CALL_MatrixRotatefEXT(ctx->CurrentServerDispatch, (matrixMode, angle, x, y, z));
}
void GLAPIENTRY
_mesa_marshal_MatrixRotatefEXT(GLenum matrixMode, GLfloat angle, GLfloat x, GLfloat y, GLfloat z)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MatrixRotatefEXT);
   struct marshal_cmd_MatrixRotatefEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MatrixRotatefEXT, cmd_size);
   cmd->matrixMode = matrixMode;
   cmd->angle = angle;
   cmd->x = x;
   cmd->y = y;
   cmd->z = z;
}


/* MatrixRotatedEXT: marshalled asynchronously */
struct marshal_cmd_MatrixRotatedEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum matrixMode;
   GLdouble angle;
   GLdouble x;
   GLdouble y;
   GLdouble z;
};
void
_mesa_unmarshal_MatrixRotatedEXT(struct gl_context *ctx, const struct marshal_cmd_MatrixRotatedEXT *cmd)
{
   GLenum matrixMode = cmd->matrixMode;
   GLdouble angle = cmd->angle;
   GLdouble x = cmd->x;
   GLdouble y = cmd->y;
   GLdouble z = cmd->z;
   CALL_MatrixRotatedEXT(ctx->CurrentServerDispatch, (matrixMode, angle, x, y, z));
}
void GLAPIENTRY
_mesa_marshal_MatrixRotatedEXT(GLenum matrixMode, GLdouble angle, GLdouble x, GLdouble y, GLdouble z)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MatrixRotatedEXT);
   struct marshal_cmd_MatrixRotatedEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MatrixRotatedEXT, cmd_size);
   cmd->matrixMode = matrixMode;
   cmd->angle = angle;
   cmd->x = x;
   cmd->y = y;
   cmd->z = z;
}


/* MatrixScalefEXT: marshalled asynchronously */
struct marshal_cmd_MatrixScalefEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum matrixMode;
   GLfloat x;
   GLfloat y;
   GLfloat z;
};
void
_mesa_unmarshal_MatrixScalefEXT(struct gl_context *ctx, const struct marshal_cmd_MatrixScalefEXT *cmd)
{
   GLenum matrixMode = cmd->matrixMode;
   GLfloat x = cmd->x;
   GLfloat y = cmd->y;
   GLfloat z = cmd->z;
   CALL_MatrixScalefEXT(ctx->CurrentServerDispatch, (matrixMode, x, y, z));
}
void GLAPIENTRY
_mesa_marshal_MatrixScalefEXT(GLenum matrixMode, GLfloat x, GLfloat y, GLfloat z)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MatrixScalefEXT);
   struct marshal_cmd_MatrixScalefEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MatrixScalefEXT, cmd_size);
   cmd->matrixMode = matrixMode;
   cmd->x = x;
   cmd->y = y;
   cmd->z = z;
}


/* MatrixScaledEXT: marshalled asynchronously */
struct marshal_cmd_MatrixScaledEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum matrixMode;
   GLdouble x;
   GLdouble y;
   GLdouble z;
};
void
_mesa_unmarshal_MatrixScaledEXT(struct gl_context *ctx, const struct marshal_cmd_MatrixScaledEXT *cmd)
{
   GLenum matrixMode = cmd->matrixMode;
   GLdouble x = cmd->x;
   GLdouble y = cmd->y;
   GLdouble z = cmd->z;
   CALL_MatrixScaledEXT(ctx->CurrentServerDispatch, (matrixMode, x, y, z));
}
void GLAPIENTRY
_mesa_marshal_MatrixScaledEXT(GLenum matrixMode, GLdouble x, GLdouble y, GLdouble z)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MatrixScaledEXT);
   struct marshal_cmd_MatrixScaledEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MatrixScaledEXT, cmd_size);
   cmd->matrixMode = matrixMode;
   cmd->x = x;
   cmd->y = y;
   cmd->z = z;
}


/* MatrixTranslatefEXT: marshalled asynchronously */
struct marshal_cmd_MatrixTranslatefEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum matrixMode;
   GLfloat x;
   GLfloat y;
   GLfloat z;
};
void
_mesa_unmarshal_MatrixTranslatefEXT(struct gl_context *ctx, const struct marshal_cmd_MatrixTranslatefEXT *cmd)
{
   GLenum matrixMode = cmd->matrixMode;
   GLfloat x = cmd->x;
   GLfloat y = cmd->y;
   GLfloat z = cmd->z;
   CALL_MatrixTranslatefEXT(ctx->CurrentServerDispatch, (matrixMode, x, y, z));
}
void GLAPIENTRY
_mesa_marshal_MatrixTranslatefEXT(GLenum matrixMode, GLfloat x, GLfloat y, GLfloat z)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MatrixTranslatefEXT);
   struct marshal_cmd_MatrixTranslatefEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MatrixTranslatefEXT, cmd_size);
   cmd->matrixMode = matrixMode;
   cmd->x = x;
   cmd->y = y;
   cmd->z = z;
}


/* MatrixTranslatedEXT: marshalled asynchronously */
struct marshal_cmd_MatrixTranslatedEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum matrixMode;
   GLdouble x;
   GLdouble y;
   GLdouble z;
};
void
_mesa_unmarshal_MatrixTranslatedEXT(struct gl_context *ctx, const struct marshal_cmd_MatrixTranslatedEXT *cmd)
{
   GLenum matrixMode = cmd->matrixMode;
   GLdouble x = cmd->x;
   GLdouble y = cmd->y;
   GLdouble z = cmd->z;
   CALL_MatrixTranslatedEXT(ctx->CurrentServerDispatch, (matrixMode, x, y, z));
}
void GLAPIENTRY
_mesa_marshal_MatrixTranslatedEXT(GLenum matrixMode, GLdouble x, GLdouble y, GLdouble z)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MatrixTranslatedEXT);
   struct marshal_cmd_MatrixTranslatedEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MatrixTranslatedEXT, cmd_size);
   cmd->matrixMode = matrixMode;
   cmd->x = x;
   cmd->y = y;
   cmd->z = z;
}


/* MatrixOrthoEXT: marshalled asynchronously */
struct marshal_cmd_MatrixOrthoEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum matrixMode;
   GLdouble l;
   GLdouble r;
   GLdouble b;
   GLdouble t;
   GLdouble n;
   GLdouble f;
};
void
_mesa_unmarshal_MatrixOrthoEXT(struct gl_context *ctx, const struct marshal_cmd_MatrixOrthoEXT *cmd)
{
   GLenum matrixMode = cmd->matrixMode;
   GLdouble l = cmd->l;
   GLdouble r = cmd->r;
   GLdouble b = cmd->b;
   GLdouble t = cmd->t;
   GLdouble n = cmd->n;
   GLdouble f = cmd->f;
   CALL_MatrixOrthoEXT(ctx->CurrentServerDispatch, (matrixMode, l, r, b, t, n, f));
}
void GLAPIENTRY
_mesa_marshal_MatrixOrthoEXT(GLenum matrixMode, GLdouble l, GLdouble r, GLdouble b, GLdouble t, GLdouble n, GLdouble f)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MatrixOrthoEXT);
   struct marshal_cmd_MatrixOrthoEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MatrixOrthoEXT, cmd_size);
   cmd->matrixMode = matrixMode;
   cmd->l = l;
   cmd->r = r;
   cmd->b = b;
   cmd->t = t;
   cmd->n = n;
   cmd->f = f;
}


/* MatrixFrustumEXT: marshalled asynchronously */
struct marshal_cmd_MatrixFrustumEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum matrixMode;
   GLdouble l;
   GLdouble r;
   GLdouble b;
   GLdouble t;
   GLdouble n;
   GLdouble f;
};
void
_mesa_unmarshal_MatrixFrustumEXT(struct gl_context *ctx, const struct marshal_cmd_MatrixFrustumEXT *cmd)
{
   GLenum matrixMode = cmd->matrixMode;
   GLdouble l = cmd->l;
   GLdouble r = cmd->r;
   GLdouble b = cmd->b;
   GLdouble t = cmd->t;
   GLdouble n = cmd->n;
   GLdouble f = cmd->f;
   CALL_MatrixFrustumEXT(ctx->CurrentServerDispatch, (matrixMode, l, r, b, t, n, f));
}
void GLAPIENTRY
_mesa_marshal_MatrixFrustumEXT(GLenum matrixMode, GLdouble l, GLdouble r, GLdouble b, GLdouble t, GLdouble n, GLdouble f)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MatrixFrustumEXT);
   struct marshal_cmd_MatrixFrustumEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MatrixFrustumEXT, cmd_size);
   cmd->matrixMode = matrixMode;
   cmd->l = l;
   cmd->r = r;
   cmd->b = b;
   cmd->t = t;
   cmd->n = n;
   cmd->f = f;
}


/* MatrixPushEXT: marshalled asynchronously */
struct marshal_cmd_MatrixPushEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum matrixMode;
};
void
_mesa_unmarshal_MatrixPushEXT(struct gl_context *ctx, const struct marshal_cmd_MatrixPushEXT *cmd)
{
   GLenum matrixMode = cmd->matrixMode;
   CALL_MatrixPushEXT(ctx->CurrentServerDispatch, (matrixMode));
}
void GLAPIENTRY
_mesa_marshal_MatrixPushEXT(GLenum matrixMode)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MatrixPushEXT);
   struct marshal_cmd_MatrixPushEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MatrixPushEXT, cmd_size);
   cmd->matrixMode = matrixMode;
   _mesa_glthread_MatrixPushEXT(ctx, matrixMode);
}


/* MatrixPopEXT: marshalled asynchronously */
struct marshal_cmd_MatrixPopEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum matrixMode;
};
void
_mesa_unmarshal_MatrixPopEXT(struct gl_context *ctx, const struct marshal_cmd_MatrixPopEXT *cmd)
{
   GLenum matrixMode = cmd->matrixMode;
   CALL_MatrixPopEXT(ctx->CurrentServerDispatch, (matrixMode));
}
void GLAPIENTRY
_mesa_marshal_MatrixPopEXT(GLenum matrixMode)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MatrixPopEXT);
   struct marshal_cmd_MatrixPopEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MatrixPopEXT, cmd_size);
   cmd->matrixMode = matrixMode;
   _mesa_glthread_MatrixPopEXT(ctx, matrixMode);
}


/* ClientAttribDefaultEXT: marshalled asynchronously */
struct marshal_cmd_ClientAttribDefaultEXT
{
   struct marshal_cmd_base cmd_base;
   GLbitfield mask;
};
void
_mesa_unmarshal_ClientAttribDefaultEXT(struct gl_context *ctx, const struct marshal_cmd_ClientAttribDefaultEXT *cmd)
{
   GLbitfield mask = cmd->mask;
   CALL_ClientAttribDefaultEXT(ctx->CurrentServerDispatch, (mask));
}
void GLAPIENTRY
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
void
_mesa_unmarshal_PushClientAttribDefaultEXT(struct gl_context *ctx, const struct marshal_cmd_PushClientAttribDefaultEXT *cmd)
{
   GLbitfield mask = cmd->mask;
   CALL_PushClientAttribDefaultEXT(ctx->CurrentServerDispatch, (mask));
}
void GLAPIENTRY
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
void GLAPIENTRY
_mesa_marshal_GetTextureParameterivEXT(GLuint texture, GLenum target, GLenum pname, GLint * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetTextureParameterivEXT");
   CALL_GetTextureParameterivEXT(ctx->CurrentServerDispatch, (texture, target, pname, params));
}


/* GetTextureParameterfvEXT: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetTextureParameterfvEXT(GLuint texture, GLenum target, GLenum pname, float * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetTextureParameterfvEXT");
   CALL_GetTextureParameterfvEXT(ctx->CurrentServerDispatch, (texture, target, pname, params));
}


/* GetTextureLevelParameterivEXT: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetTextureLevelParameterivEXT(GLuint texture, GLenum target, GLint level, GLenum pname, GLint * params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetTextureLevelParameterivEXT");
   CALL_GetTextureLevelParameterivEXT(ctx->CurrentServerDispatch, (texture, target, level, pname, params));
}


/* GetTextureLevelParameterfvEXT: marshalled synchronously */
void GLAPIENTRY
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
   GLuint texture;
   GLenum target;
   GLenum pname;
   int param;
};
void
_mesa_unmarshal_TextureParameteriEXT(struct gl_context *ctx, const struct marshal_cmd_TextureParameteriEXT *cmd)
{
   GLuint texture = cmd->texture;
   GLenum target = cmd->target;
   GLenum pname = cmd->pname;
   int param = cmd->param;
   CALL_TextureParameteriEXT(ctx->CurrentServerDispatch, (texture, target, pname, param));
}
void GLAPIENTRY
_mesa_marshal_TextureParameteriEXT(GLuint texture, GLenum target, GLenum pname, int param)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TextureParameteriEXT);
   struct marshal_cmd_TextureParameteriEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TextureParameteriEXT, cmd_size);
   cmd->texture = texture;
   cmd->target = target;
   cmd->pname = pname;
   cmd->param = param;
}


/* TextureParameterivEXT: marshalled asynchronously */
struct marshal_cmd_TextureParameterivEXT
{
   struct marshal_cmd_base cmd_base;
   GLuint texture;
   GLenum target;
   GLenum pname;
   /* Next safe_mul(_mesa_tex_param_enum_to_count(pname), 1 * sizeof(GLint)) bytes are GLint params[_mesa_tex_param_enum_to_count(pname)] */
};
void
_mesa_unmarshal_TextureParameterivEXT(struct gl_context *ctx, const struct marshal_cmd_TextureParameterivEXT *cmd)
{
   GLuint texture = cmd->texture;
   GLenum target = cmd->target;
   GLenum pname = cmd->pname;
   GLint * params;
   const char *variable_data = (const char *) (cmd + 1);
   params = (GLint *) variable_data;
   CALL_TextureParameterivEXT(ctx->CurrentServerDispatch, (texture, target, pname, params));
}
void GLAPIENTRY
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
   cmd->target = target;
   cmd->pname = pname;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, params, params_size);
}


/* TextureParameterfEXT: marshalled asynchronously */
struct marshal_cmd_TextureParameterfEXT
{
   struct marshal_cmd_base cmd_base;
   GLuint texture;
   GLenum target;
   GLenum pname;
   float param;
};
void
_mesa_unmarshal_TextureParameterfEXT(struct gl_context *ctx, const struct marshal_cmd_TextureParameterfEXT *cmd)
{
   GLuint texture = cmd->texture;
   GLenum target = cmd->target;
   GLenum pname = cmd->pname;
   float param = cmd->param;
   CALL_TextureParameterfEXT(ctx->CurrentServerDispatch, (texture, target, pname, param));
}
void GLAPIENTRY
_mesa_marshal_TextureParameterfEXT(GLuint texture, GLenum target, GLenum pname, float param)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_TextureParameterfEXT);
   struct marshal_cmd_TextureParameterfEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_TextureParameterfEXT, cmd_size);
   cmd->texture = texture;
   cmd->target = target;
   cmd->pname = pname;
   cmd->param = param;
}


/* TextureParameterfvEXT: marshalled asynchronously */
struct marshal_cmd_TextureParameterfvEXT
{
   struct marshal_cmd_base cmd_base;
   GLuint texture;
   GLenum target;
   GLenum pname;
   /* Next safe_mul(_mesa_tex_param_enum_to_count(pname), 1 * sizeof(float)) bytes are float params[_mesa_tex_param_enum_to_count(pname)] */
};
void
_mesa_unmarshal_TextureParameterfvEXT(struct gl_context *ctx, const struct marshal_cmd_TextureParameterfvEXT *cmd)
{
   GLuint texture = cmd->texture;
   GLenum target = cmd->target;
   GLenum pname = cmd->pname;
   float * params;
   const char *variable_data = (const char *) (cmd + 1);
   params = (float *) variable_data;
   CALL_TextureParameterfvEXT(ctx->CurrentServerDispatch, (texture, target, pname, params));
}
void GLAPIENTRY
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
   cmd->target = target;
   cmd->pname = pname;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, params, params_size);
}


/* TextureImage1DEXT: marshalled asynchronously */
struct marshal_cmd_TextureImage1DEXT
{
   struct marshal_cmd_base cmd_base;
   GLuint texture;
   GLenum target;
   GLint level;
   GLint internalFormat;
   GLsizei width;
   GLint border;
   GLenum format;
   GLenum type;
   const GLvoid * pixels;
};
void
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
}
void GLAPIENTRY
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
   cmd->target = target;
   cmd->level = level;
   cmd->internalFormat = internalFormat;
   cmd->width = width;
   cmd->border = border;
   cmd->format = format;
   cmd->type = type;
   cmd->pixels = pixels;
}


/* TextureImage2DEXT: marshalled asynchronously */
struct marshal_cmd_TextureImage2DEXT
{
   struct marshal_cmd_base cmd_base;
   GLuint texture;
   GLenum target;
   GLint level;
   GLint internalFormat;
   GLsizei width;
   GLsizei height;
   GLint border;
   GLenum format;
   GLenum type;
   const GLvoid * pixels;
};
void
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
}
void GLAPIENTRY
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
   cmd->target = target;
   cmd->level = level;
   cmd->internalFormat = internalFormat;
   cmd->width = width;
   cmd->height = height;
   cmd->border = border;
   cmd->format = format;
   cmd->type = type;
   cmd->pixels = pixels;
}


/* TextureImage3DEXT: marshalled asynchronously */
struct marshal_cmd_TextureImage3DEXT
{
   struct marshal_cmd_base cmd_base;
   GLuint texture;
   GLenum target;
   GLint level;
   GLint internalFormat;
   GLsizei width;
   GLsizei height;
   GLsizei depth;
   GLint border;
   GLenum format;
   GLenum type;
   const GLvoid * pixels;
};
void
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
}
void GLAPIENTRY
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
   cmd->target = target;
   cmd->level = level;
   cmd->internalFormat = internalFormat;
   cmd->width = width;
   cmd->height = height;
   cmd->depth = depth;
   cmd->border = border;
   cmd->format = format;
   cmd->type = type;
   cmd->pixels = pixels;
}


/* TextureSubImage1DEXT: marshalled asynchronously */
struct marshal_cmd_TextureSubImage1DEXT
{
   struct marshal_cmd_base cmd_base;
   GLuint texture;
   GLenum target;
   GLint level;
   GLint xoffset;
   GLsizei width;
   GLenum format;
   GLenum type;
   const GLvoid * pixels;
};
void
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
}
void GLAPIENTRY
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
   cmd->target = target;
   cmd->level = level;
   cmd->xoffset = xoffset;
   cmd->width = width;
   cmd->format = format;
   cmd->type = type;
   cmd->pixels = pixels;
}


/* TextureSubImage2DEXT: marshalled asynchronously */
struct marshal_cmd_TextureSubImage2DEXT
{
   struct marshal_cmd_base cmd_base;
   GLuint texture;
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
void
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
}
void GLAPIENTRY
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


/* TextureSubImage3DEXT: marshalled asynchronously */
struct marshal_cmd_TextureSubImage3DEXT
{
   struct marshal_cmd_base cmd_base;
   GLuint texture;
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
void
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
}
void GLAPIENTRY
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


/* CopyTextureImage1DEXT: marshalled asynchronously */
struct marshal_cmd_CopyTextureImage1DEXT
{
   struct marshal_cmd_base cmd_base;
   GLuint texture;
   GLenum target;
   GLint level;
   GLenum internalFormat;
   GLint x;
   GLint y;
   GLsizei width;
   int border;
};
void
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
}
void GLAPIENTRY
_mesa_marshal_CopyTextureImage1DEXT(GLuint texture, GLenum target, GLint level, GLenum internalFormat, GLint x, GLint y, GLsizei width, int border)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_CopyTextureImage1DEXT);
   struct marshal_cmd_CopyTextureImage1DEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_CopyTextureImage1DEXT, cmd_size);
   cmd->texture = texture;
   cmd->target = target;
   cmd->level = level;
   cmd->internalFormat = internalFormat;
   cmd->x = x;
   cmd->y = y;
   cmd->width = width;
   cmd->border = border;
}


/* CopyTextureImage2DEXT: marshalled asynchronously */
struct marshal_cmd_CopyTextureImage2DEXT
{
   struct marshal_cmd_base cmd_base;
   GLuint texture;
   GLenum target;
   GLint level;
   GLenum internalFormat;
   GLint x;
   GLint y;
   GLsizei width;
   GLsizei height;
   int border;
};
void
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
}
void GLAPIENTRY
_mesa_marshal_CopyTextureImage2DEXT(GLuint texture, GLenum target, GLint level, GLenum internalFormat, GLint x, GLint y, GLsizei width, GLsizei height, int border)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_CopyTextureImage2DEXT);
   struct marshal_cmd_CopyTextureImage2DEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_CopyTextureImage2DEXT, cmd_size);
   cmd->texture = texture;
   cmd->target = target;
   cmd->level = level;
   cmd->internalFormat = internalFormat;
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
   GLuint texture;
   GLenum target;
   GLint level;
   GLint xoffset;
   GLint x;
   GLint y;
   GLsizei width;
};
void
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
}
void GLAPIENTRY
_mesa_marshal_CopyTextureSubImage1DEXT(GLuint texture, GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_CopyTextureSubImage1DEXT);
   struct marshal_cmd_CopyTextureSubImage1DEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_CopyTextureSubImage1DEXT, cmd_size);
   cmd->texture = texture;
   cmd->target = target;
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
   GLuint texture;
   GLenum target;
   GLint level;
   GLint xoffset;
   GLint yoffset;
   GLint x;
   GLint y;
   GLsizei width;
   GLsizei height;
};
void
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
}
void GLAPIENTRY
_mesa_marshal_CopyTextureSubImage2DEXT(GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_CopyTextureSubImage2DEXT);
   struct marshal_cmd_CopyTextureSubImage2DEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_CopyTextureSubImage2DEXT, cmd_size);
   cmd->texture = texture;
   cmd->target = target;
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
   GLuint texture;
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
void
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
}
void GLAPIENTRY
_mesa_marshal_CopyTextureSubImage3DEXT(GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_CopyTextureSubImage3DEXT);
   struct marshal_cmd_CopyTextureSubImage3DEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_CopyTextureSubImage3DEXT, cmd_size);
   cmd->texture = texture;
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


/* GetTextureImageEXT: marshalled asynchronously */
struct marshal_cmd_GetTextureImageEXT
{
   struct marshal_cmd_base cmd_base;
   GLuint texture;
   GLenum target;
   GLint level;
   GLenum format;
   GLenum type;
   GLvoid * pixels;
};
void
_mesa_unmarshal_GetTextureImageEXT(struct gl_context *ctx, const struct marshal_cmd_GetTextureImageEXT *cmd)
{
   GLuint texture = cmd->texture;
   GLenum target = cmd->target;
   GLint level = cmd->level;
   GLenum format = cmd->format;
   GLenum type = cmd->type;
   GLvoid * pixels = cmd->pixels;
   CALL_GetTextureImageEXT(ctx->CurrentServerDispatch, (texture, target, level, format, type, pixels));
}
void GLAPIENTRY
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
   cmd->target = target;
   cmd->level = level;
   cmd->format = format;
   cmd->type = type;
   cmd->pixels = pixels;
}


/* BindMultiTextureEXT: marshalled asynchronously */
struct marshal_cmd_BindMultiTextureEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum texunit;
   GLenum target;
   GLuint texture;
};
void
_mesa_unmarshal_BindMultiTextureEXT(struct gl_context *ctx, const struct marshal_cmd_BindMultiTextureEXT *cmd)
{
   GLenum texunit = cmd->texunit;
   GLenum target = cmd->target;
   GLuint texture = cmd->texture;
   CALL_BindMultiTextureEXT(ctx->CurrentServerDispatch, (texunit, target, texture));
}
void GLAPIENTRY
_mesa_marshal_BindMultiTextureEXT(GLenum texunit, GLenum target, GLuint texture)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_BindMultiTextureEXT);
   struct marshal_cmd_BindMultiTextureEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_BindMultiTextureEXT, cmd_size);
   cmd->texunit = texunit;
   cmd->target = target;
   cmd->texture = texture;
}


/* EnableClientStateiEXT: marshalled asynchronously */
struct marshal_cmd_EnableClientStateiEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum array;
   GLuint index;
};
void
_mesa_unmarshal_EnableClientStateiEXT(struct gl_context *ctx, const struct marshal_cmd_EnableClientStateiEXT *cmd)
{
   GLenum array = cmd->array;
   GLuint index = cmd->index;
   CALL_EnableClientStateiEXT(ctx->CurrentServerDispatch, (array, index));
}
void GLAPIENTRY
_mesa_marshal_EnableClientStateiEXT(GLenum array, GLuint index)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_EnableClientStateiEXT);
   struct marshal_cmd_EnableClientStateiEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_EnableClientStateiEXT, cmd_size);
   cmd->array = array;
   cmd->index = index;
   if (COMPAT) _mesa_glthread_ClientState(ctx, NULL, VERT_ATTRIB_TEX(index), true);
}


/* DisableClientStateiEXT: marshalled asynchronously */
struct marshal_cmd_DisableClientStateiEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum array;
   GLuint index;
};
void
_mesa_unmarshal_DisableClientStateiEXT(struct gl_context *ctx, const struct marshal_cmd_DisableClientStateiEXT *cmd)
{
   GLenum array = cmd->array;
   GLuint index = cmd->index;
   CALL_DisableClientStateiEXT(ctx->CurrentServerDispatch, (array, index));
}
void GLAPIENTRY
_mesa_marshal_DisableClientStateiEXT(GLenum array, GLuint index)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_DisableClientStateiEXT);
   struct marshal_cmd_DisableClientStateiEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_DisableClientStateiEXT, cmd_size);
   cmd->array = array;
   cmd->index = index;
   if (COMPAT) _mesa_glthread_ClientState(ctx, NULL, VERT_ATTRIB_TEX(index), false);
}


/* GetPointerIndexedvEXT: marshalled synchronously */
void GLAPIENTRY
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
   GLenum texunit;
   GLenum target;
   GLenum pname;
   GLint param;
};
void
_mesa_unmarshal_MultiTexEnviEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexEnviEXT *cmd)
{
   GLenum texunit = cmd->texunit;
   GLenum target = cmd->target;
   GLenum pname = cmd->pname;
   GLint param = cmd->param;
   CALL_MultiTexEnviEXT(ctx->CurrentServerDispatch, (texunit, target, pname, param));
}
void GLAPIENTRY
_mesa_marshal_MultiTexEnviEXT(GLenum texunit, GLenum target, GLenum pname, GLint param)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexEnviEXT);
   struct marshal_cmd_MultiTexEnviEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexEnviEXT, cmd_size);
   cmd->texunit = texunit;
   cmd->target = target;
   cmd->pname = pname;
   cmd->param = param;
}


/* MultiTexEnvivEXT: marshalled asynchronously */
struct marshal_cmd_MultiTexEnvivEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum texunit;
   GLenum target;
   GLenum pname;
   /* Next safe_mul(_mesa_texenv_enum_to_count(pname), 1 * sizeof(GLint)) bytes are GLint param[_mesa_texenv_enum_to_count(pname)] */
};
void
_mesa_unmarshal_MultiTexEnvivEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexEnvivEXT *cmd)
{
   GLenum texunit = cmd->texunit;
   GLenum target = cmd->target;
   GLenum pname = cmd->pname;
   GLint * param;
   const char *variable_data = (const char *) (cmd + 1);
   param = (GLint *) variable_data;
   CALL_MultiTexEnvivEXT(ctx->CurrentServerDispatch, (texunit, target, pname, param));
}
void GLAPIENTRY
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
   cmd->texunit = texunit;
   cmd->target = target;
   cmd->pname = pname;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, param, param_size);
}


/* MultiTexEnvfEXT: marshalled asynchronously */
struct marshal_cmd_MultiTexEnvfEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum texunit;
   GLenum target;
   GLenum pname;
   GLfloat param;
};
void
_mesa_unmarshal_MultiTexEnvfEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexEnvfEXT *cmd)
{
   GLenum texunit = cmd->texunit;
   GLenum target = cmd->target;
   GLenum pname = cmd->pname;
   GLfloat param = cmd->param;
   CALL_MultiTexEnvfEXT(ctx->CurrentServerDispatch, (texunit, target, pname, param));
}
void GLAPIENTRY
_mesa_marshal_MultiTexEnvfEXT(GLenum texunit, GLenum target, GLenum pname, GLfloat param)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexEnvfEXT);
   struct marshal_cmd_MultiTexEnvfEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexEnvfEXT, cmd_size);
   cmd->texunit = texunit;
   cmd->target = target;
   cmd->pname = pname;
   cmd->param = param;
}


/* MultiTexEnvfvEXT: marshalled asynchronously */
struct marshal_cmd_MultiTexEnvfvEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum texunit;
   GLenum target;
   GLenum pname;
   /* Next safe_mul(_mesa_texenv_enum_to_count(pname), 1 * sizeof(GLfloat)) bytes are GLfloat param[_mesa_texenv_enum_to_count(pname)] */
};
void
_mesa_unmarshal_MultiTexEnvfvEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexEnvfvEXT *cmd)
{
   GLenum texunit = cmd->texunit;
   GLenum target = cmd->target;
   GLenum pname = cmd->pname;
   GLfloat * param;
   const char *variable_data = (const char *) (cmd + 1);
   param = (GLfloat *) variable_data;
   CALL_MultiTexEnvfvEXT(ctx->CurrentServerDispatch, (texunit, target, pname, param));
}
void GLAPIENTRY
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
   cmd->texunit = texunit;
   cmd->target = target;
   cmd->pname = pname;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, param, param_size);
}


/* GetMultiTexEnvivEXT: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetMultiTexEnvivEXT(GLenum texunit, GLenum target, GLenum pname, GLint * param)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetMultiTexEnvivEXT");
   CALL_GetMultiTexEnvivEXT(ctx->CurrentServerDispatch, (texunit, target, pname, param));
}


/* GetMultiTexEnvfvEXT: marshalled synchronously */
void GLAPIENTRY
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
   GLenum texunit;
   GLenum target;
   GLenum pname;
   GLint param;
};
void
_mesa_unmarshal_MultiTexParameteriEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexParameteriEXT *cmd)
{
   GLenum texunit = cmd->texunit;
   GLenum target = cmd->target;
   GLenum pname = cmd->pname;
   GLint param = cmd->param;
   CALL_MultiTexParameteriEXT(ctx->CurrentServerDispatch, (texunit, target, pname, param));
}
void GLAPIENTRY
_mesa_marshal_MultiTexParameteriEXT(GLenum texunit, GLenum target, GLenum pname, GLint param)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexParameteriEXT);
   struct marshal_cmd_MultiTexParameteriEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexParameteriEXT, cmd_size);
   cmd->texunit = texunit;
   cmd->target = target;
   cmd->pname = pname;
   cmd->param = param;
}


/* MultiTexParameterivEXT: marshalled asynchronously */
struct marshal_cmd_MultiTexParameterivEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum texunit;
   GLenum target;
   GLenum pname;
   /* Next safe_mul(_mesa_tex_param_enum_to_count(pname), 1 * sizeof(GLint)) bytes are GLint param[_mesa_tex_param_enum_to_count(pname)] */
};
void
_mesa_unmarshal_MultiTexParameterivEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexParameterivEXT *cmd)
{
   GLenum texunit = cmd->texunit;
   GLenum target = cmd->target;
   GLenum pname = cmd->pname;
   GLint * param;
   const char *variable_data = (const char *) (cmd + 1);
   param = (GLint *) variable_data;
   CALL_MultiTexParameterivEXT(ctx->CurrentServerDispatch, (texunit, target, pname, param));
}
void GLAPIENTRY
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
   cmd->texunit = texunit;
   cmd->target = target;
   cmd->pname = pname;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, param, param_size);
}


/* MultiTexParameterfEXT: marshalled asynchronously */
struct marshal_cmd_MultiTexParameterfEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum texunit;
   GLenum target;
   GLenum pname;
   GLfloat param;
};
void
_mesa_unmarshal_MultiTexParameterfEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexParameterfEXT *cmd)
{
   GLenum texunit = cmd->texunit;
   GLenum target = cmd->target;
   GLenum pname = cmd->pname;
   GLfloat param = cmd->param;
   CALL_MultiTexParameterfEXT(ctx->CurrentServerDispatch, (texunit, target, pname, param));
}
void GLAPIENTRY
_mesa_marshal_MultiTexParameterfEXT(GLenum texunit, GLenum target, GLenum pname, GLfloat param)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexParameterfEXT);
   struct marshal_cmd_MultiTexParameterfEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexParameterfEXT, cmd_size);
   cmd->texunit = texunit;
   cmd->target = target;
   cmd->pname = pname;
   cmd->param = param;
}


/* MultiTexParameterfvEXT: marshalled asynchronously */
struct marshal_cmd_MultiTexParameterfvEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum texunit;
   GLenum target;
   GLenum pname;
   /* Next safe_mul(_mesa_tex_param_enum_to_count(pname), 1 * sizeof(GLfloat)) bytes are GLfloat param[_mesa_tex_param_enum_to_count(pname)] */
};
void
_mesa_unmarshal_MultiTexParameterfvEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexParameterfvEXT *cmd)
{
   GLenum texunit = cmd->texunit;
   GLenum target = cmd->target;
   GLenum pname = cmd->pname;
   GLfloat * param;
   const char *variable_data = (const char *) (cmd + 1);
   param = (GLfloat *) variable_data;
   CALL_MultiTexParameterfvEXT(ctx->CurrentServerDispatch, (texunit, target, pname, param));
}
void GLAPIENTRY
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
   cmd->texunit = texunit;
   cmd->target = target;
   cmd->pname = pname;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, param, param_size);
}


/* GetMultiTexParameterivEXT: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetMultiTexParameterivEXT(GLenum texunit, GLenum target, GLenum pname, GLint* params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetMultiTexParameterivEXT");
   CALL_GetMultiTexParameterivEXT(ctx->CurrentServerDispatch, (texunit, target, pname, params));
}


/* GetMultiTexParameterfvEXT: marshalled synchronously */
void GLAPIENTRY
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
   GLenum texunit;
   GLenum target;
   GLint level;
   GLenum format;
   GLenum type;
   GLvoid* pixels;
};
void
_mesa_unmarshal_GetMultiTexImageEXT(struct gl_context *ctx, const struct marshal_cmd_GetMultiTexImageEXT *cmd)
{
   GLenum texunit = cmd->texunit;
   GLenum target = cmd->target;
   GLint level = cmd->level;
   GLenum format = cmd->format;
   GLenum type = cmd->type;
   GLvoid* pixels = cmd->pixels;
   CALL_GetMultiTexImageEXT(ctx->CurrentServerDispatch, (texunit, target, level, format, type, pixels));
}
void GLAPIENTRY
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
   cmd->texunit = texunit;
   cmd->target = target;
   cmd->level = level;
   cmd->format = format;
   cmd->type = type;
   cmd->pixels = pixels;
}


/* GetMultiTexLevelParameterivEXT: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetMultiTexLevelParameterivEXT(GLenum texunit, GLenum target, GLint level, GLenum pname, GLint* params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetMultiTexLevelParameterivEXT");
   CALL_GetMultiTexLevelParameterivEXT(ctx->CurrentServerDispatch, (texunit, target, level, pname, params));
}


/* GetMultiTexLevelParameterfvEXT: marshalled synchronously */
void GLAPIENTRY
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
   GLenum texunit;
   GLenum target;
   GLint level;
   GLint internalformat;
   GLsizei width;
   GLint border;
   GLenum format;
   GLenum type;
   const GLvoid* pixels;
};
void
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
}
void GLAPIENTRY
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
   cmd->texunit = texunit;
   cmd->target = target;
   cmd->level = level;
   cmd->internalformat = internalformat;
   cmd->width = width;
   cmd->border = border;
   cmd->format = format;
   cmd->type = type;
   cmd->pixels = pixels;
}


/* MultiTexImage2DEXT: marshalled asynchronously */
struct marshal_cmd_MultiTexImage2DEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum texunit;
   GLenum target;
   GLint level;
   GLint internalformat;
   GLsizei width;
   GLsizei height;
   GLint border;
   GLenum format;
   GLenum type;
   const GLvoid* pixels;
};
void
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
}
void GLAPIENTRY
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
   cmd->texunit = texunit;
   cmd->target = target;
   cmd->level = level;
   cmd->internalformat = internalformat;
   cmd->width = width;
   cmd->height = height;
   cmd->border = border;
   cmd->format = format;
   cmd->type = type;
   cmd->pixels = pixels;
}


/* MultiTexImage3DEXT: marshalled asynchronously */
struct marshal_cmd_MultiTexImage3DEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum texunit;
   GLenum target;
   GLint level;
   GLint internalformat;
   GLsizei width;
   GLsizei height;
   GLsizei depth;
   GLint border;
   GLenum format;
   GLenum type;
   const GLvoid* pixels;
};
void
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
}
void GLAPIENTRY
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
   cmd->texunit = texunit;
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


/* MultiTexSubImage1DEXT: marshalled asynchronously */
struct marshal_cmd_MultiTexSubImage1DEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum texunit;
   GLenum target;
   GLint level;
   GLint xoffset;
   GLsizei width;
   GLenum format;
   GLenum type;
   const GLvoid* pixels;
};
void
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
}
void GLAPIENTRY
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
   cmd->texunit = texunit;
   cmd->target = target;
   cmd->level = level;
   cmd->xoffset = xoffset;
   cmd->width = width;
   cmd->format = format;
   cmd->type = type;
   cmd->pixels = pixels;
}


/* MultiTexSubImage2DEXT: marshalled asynchronously */
struct marshal_cmd_MultiTexSubImage2DEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum texunit;
   GLenum target;
   GLint level;
   GLint xoffset;
   GLint yoffset;
   GLsizei width;
   GLsizei height;
   GLenum format;
   GLenum type;
   const GLvoid* pixels;
};
void
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
}
void GLAPIENTRY
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
   cmd->texunit = texunit;
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


/* MultiTexSubImage3DEXT: marshalled asynchronously */
struct marshal_cmd_MultiTexSubImage3DEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum texunit;
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
   const GLvoid* pixels;
};
void
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
}
void GLAPIENTRY
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
   cmd->texunit = texunit;
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


/* CopyMultiTexImage1DEXT: marshalled asynchronously */
struct marshal_cmd_CopyMultiTexImage1DEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum texunit;
   GLenum target;
   GLint level;
   GLenum internalformat;
   GLint x;
   GLint y;
   GLsizei width;
   GLint border;
};
void
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
}
void GLAPIENTRY
_mesa_marshal_CopyMultiTexImage1DEXT(GLenum texunit, GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLint border)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_CopyMultiTexImage1DEXT);
   struct marshal_cmd_CopyMultiTexImage1DEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_CopyMultiTexImage1DEXT, cmd_size);
   cmd->texunit = texunit;
   cmd->target = target;
   cmd->level = level;
   cmd->internalformat = internalformat;
   cmd->x = x;
   cmd->y = y;
   cmd->width = width;
   cmd->border = border;
}


/* CopyMultiTexImage2DEXT: marshalled asynchronously */
struct marshal_cmd_CopyMultiTexImage2DEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum texunit;
   GLenum target;
   GLint level;
   GLenum internalformat;
   GLint x;
   GLint y;
   GLsizei width;
   GLsizei height;
   GLint border;
};
void
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
}
void GLAPIENTRY
_mesa_marshal_CopyMultiTexImage2DEXT(GLenum texunit, GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_CopyMultiTexImage2DEXT);
   struct marshal_cmd_CopyMultiTexImage2DEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_CopyMultiTexImage2DEXT, cmd_size);
   cmd->texunit = texunit;
   cmd->target = target;
   cmd->level = level;
   cmd->internalformat = internalformat;
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
   GLenum texunit;
   GLenum target;
   GLint level;
   GLint xoffset;
   GLint x;
   GLint y;
   GLsizei width;
};
void
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
}
void GLAPIENTRY
_mesa_marshal_CopyMultiTexSubImage1DEXT(GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_CopyMultiTexSubImage1DEXT);
   struct marshal_cmd_CopyMultiTexSubImage1DEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_CopyMultiTexSubImage1DEXT, cmd_size);
   cmd->texunit = texunit;
   cmd->target = target;
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
   GLenum texunit;
   GLenum target;
   GLint level;
   GLint xoffset;
   GLint yoffset;
   GLint x;
   GLint y;
   GLsizei width;
   GLsizei height;
};
void
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
}
void GLAPIENTRY
_mesa_marshal_CopyMultiTexSubImage2DEXT(GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_CopyMultiTexSubImage2DEXT);
   struct marshal_cmd_CopyMultiTexSubImage2DEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_CopyMultiTexSubImage2DEXT, cmd_size);
   cmd->texunit = texunit;
   cmd->target = target;
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
   GLenum texunit;
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
void
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
}
void GLAPIENTRY
_mesa_marshal_CopyMultiTexSubImage3DEXT(GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_CopyMultiTexSubImage3DEXT);
   struct marshal_cmd_CopyMultiTexSubImage3DEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_CopyMultiTexSubImage3DEXT, cmd_size);
   cmd->texunit = texunit;
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


/* MultiTexGendEXT: marshalled asynchronously */
struct marshal_cmd_MultiTexGendEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum texunit;
   GLenum coord;
   GLenum pname;
   GLdouble param;
};
void
_mesa_unmarshal_MultiTexGendEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexGendEXT *cmd)
{
   GLenum texunit = cmd->texunit;
   GLenum coord = cmd->coord;
   GLenum pname = cmd->pname;
   GLdouble param = cmd->param;
   CALL_MultiTexGendEXT(ctx->CurrentServerDispatch, (texunit, coord, pname, param));
}
void GLAPIENTRY
_mesa_marshal_MultiTexGendEXT(GLenum texunit, GLenum coord, GLenum pname, GLdouble param)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexGendEXT);
   struct marshal_cmd_MultiTexGendEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexGendEXT, cmd_size);
   cmd->texunit = texunit;
   cmd->coord = coord;
   cmd->pname = pname;
   cmd->param = param;
}


/* MultiTexGendvEXT: marshalled asynchronously */
struct marshal_cmd_MultiTexGendvEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum texunit;
   GLenum coord;
   GLenum pname;
   /* Next safe_mul(_mesa_texgen_enum_to_count(pname), 1 * sizeof(GLdouble)) bytes are GLdouble param[_mesa_texgen_enum_to_count(pname)] */
};
void
_mesa_unmarshal_MultiTexGendvEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexGendvEXT *cmd)
{
   GLenum texunit = cmd->texunit;
   GLenum coord = cmd->coord;
   GLenum pname = cmd->pname;
   GLdouble * param;
   const char *variable_data = (const char *) (cmd + 1);
   param = (GLdouble *) variable_data;
   CALL_MultiTexGendvEXT(ctx->CurrentServerDispatch, (texunit, coord, pname, param));
}
void GLAPIENTRY
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
   cmd->texunit = texunit;
   cmd->coord = coord;
   cmd->pname = pname;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, param, param_size);
}


/* MultiTexGenfEXT: marshalled asynchronously */
struct marshal_cmd_MultiTexGenfEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum texunit;
   GLenum coord;
   GLenum pname;
   GLfloat param;
};
void
_mesa_unmarshal_MultiTexGenfEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexGenfEXT *cmd)
{
   GLenum texunit = cmd->texunit;
   GLenum coord = cmd->coord;
   GLenum pname = cmd->pname;
   GLfloat param = cmd->param;
   CALL_MultiTexGenfEXT(ctx->CurrentServerDispatch, (texunit, coord, pname, param));
}
void GLAPIENTRY
_mesa_marshal_MultiTexGenfEXT(GLenum texunit, GLenum coord, GLenum pname, GLfloat param)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexGenfEXT);
   struct marshal_cmd_MultiTexGenfEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexGenfEXT, cmd_size);
   cmd->texunit = texunit;
   cmd->coord = coord;
   cmd->pname = pname;
   cmd->param = param;
}


/* MultiTexGenfvEXT: marshalled asynchronously */
struct marshal_cmd_MultiTexGenfvEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum texunit;
   GLenum coord;
   GLenum pname;
   /* Next safe_mul(_mesa_texgen_enum_to_count(pname), 1 * sizeof(GLfloat)) bytes are GLfloat param[_mesa_texgen_enum_to_count(pname)] */
};
void
_mesa_unmarshal_MultiTexGenfvEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexGenfvEXT *cmd)
{
   GLenum texunit = cmd->texunit;
   GLenum coord = cmd->coord;
   GLenum pname = cmd->pname;
   GLfloat * param;
   const char *variable_data = (const char *) (cmd + 1);
   param = (GLfloat *) variable_data;
   CALL_MultiTexGenfvEXT(ctx->CurrentServerDispatch, (texunit, coord, pname, param));
}
void GLAPIENTRY
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
   cmd->texunit = texunit;
   cmd->coord = coord;
   cmd->pname = pname;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, param, param_size);
}


/* MultiTexGeniEXT: marshalled asynchronously */
struct marshal_cmd_MultiTexGeniEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum texunit;
   GLenum coord;
   GLenum pname;
   GLint param;
};
void
_mesa_unmarshal_MultiTexGeniEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexGeniEXT *cmd)
{
   GLenum texunit = cmd->texunit;
   GLenum coord = cmd->coord;
   GLenum pname = cmd->pname;
   GLint param = cmd->param;
   CALL_MultiTexGeniEXT(ctx->CurrentServerDispatch, (texunit, coord, pname, param));
}
void GLAPIENTRY
_mesa_marshal_MultiTexGeniEXT(GLenum texunit, GLenum coord, GLenum pname, GLint param)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexGeniEXT);
   struct marshal_cmd_MultiTexGeniEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexGeniEXT, cmd_size);
   cmd->texunit = texunit;
   cmd->coord = coord;
   cmd->pname = pname;
   cmd->param = param;
}


/* MultiTexGenivEXT: marshalled asynchronously */
struct marshal_cmd_MultiTexGenivEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum texunit;
   GLenum coord;
   GLenum pname;
   /* Next safe_mul(_mesa_texgen_enum_to_count(pname), 1 * sizeof(GLint)) bytes are GLint param[_mesa_texgen_enum_to_count(pname)] */
};
void
_mesa_unmarshal_MultiTexGenivEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexGenivEXT *cmd)
{
   GLenum texunit = cmd->texunit;
   GLenum coord = cmd->coord;
   GLenum pname = cmd->pname;
   GLint * param;
   const char *variable_data = (const char *) (cmd + 1);
   param = (GLint *) variable_data;
   CALL_MultiTexGenivEXT(ctx->CurrentServerDispatch, (texunit, coord, pname, param));
}
void GLAPIENTRY
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
   cmd->texunit = texunit;
   cmd->coord = coord;
   cmd->pname = pname;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, param, param_size);
}


/* GetMultiTexGendvEXT: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetMultiTexGendvEXT(GLenum texunit, GLenum coord, GLenum pname, GLdouble * param)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetMultiTexGendvEXT");
   CALL_GetMultiTexGendvEXT(ctx->CurrentServerDispatch, (texunit, coord, pname, param));
}


/* GetMultiTexGenfvEXT: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetMultiTexGenfvEXT(GLenum texunit, GLenum coord, GLenum pname, GLfloat * param)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetMultiTexGenfvEXT");
   CALL_GetMultiTexGenfvEXT(ctx->CurrentServerDispatch, (texunit, coord, pname, param));
}


/* GetMultiTexGenivEXT: marshalled synchronously */
void GLAPIENTRY
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
   GLenum texunit;
   GLint size;
   GLenum type;
   GLsizei stride;
   const GLvoid * pointer;
};
void
_mesa_unmarshal_MultiTexCoordPointerEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoordPointerEXT *cmd)
{
   GLenum texunit = cmd->texunit;
   GLint size = cmd->size;
   GLenum type = cmd->type;
   GLsizei stride = cmd->stride;
   const GLvoid * pointer = cmd->pointer;
   CALL_MultiTexCoordPointerEXT(ctx->CurrentServerDispatch, (texunit, size, type, stride, pointer));
}
void GLAPIENTRY
_mesa_marshal_MultiTexCoordPointerEXT(GLenum texunit, GLint size, GLenum type, GLsizei stride, const GLvoid * pointer)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MultiTexCoordPointerEXT);
   struct marshal_cmd_MultiTexCoordPointerEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiTexCoordPointerEXT, cmd_size);
   cmd->texunit = texunit;
   cmd->size = size;
   cmd->type = type;
   cmd->stride = stride;
   cmd->pointer = pointer;
   if (COMPAT) _mesa_glthread_AttribPointer(ctx, VERT_ATTRIB_TEX(texunit - GL_TEXTURE0), size, type, stride,  pointer);
}


/* MatrixLoadTransposefEXT: marshalled asynchronously */
struct marshal_cmd_MatrixLoadTransposefEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum matrixMode;
   GLfloat m[16];
};
void
_mesa_unmarshal_MatrixLoadTransposefEXT(struct gl_context *ctx, const struct marshal_cmd_MatrixLoadTransposefEXT *cmd)
{
   GLenum matrixMode = cmd->matrixMode;
   const GLfloat * m = cmd->m;
   CALL_MatrixLoadTransposefEXT(ctx->CurrentServerDispatch, (matrixMode, m));
}
void GLAPIENTRY
_mesa_marshal_MatrixLoadTransposefEXT(GLenum matrixMode, const GLfloat * m)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MatrixLoadTransposefEXT);
   struct marshal_cmd_MatrixLoadTransposefEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MatrixLoadTransposefEXT, cmd_size);
   cmd->matrixMode = matrixMode;
   memcpy(cmd->m, m, 16 * sizeof(GLfloat));
}


/* MatrixLoadTransposedEXT: marshalled asynchronously */
struct marshal_cmd_MatrixLoadTransposedEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum matrixMode;
   GLdouble m[16];
};
void
_mesa_unmarshal_MatrixLoadTransposedEXT(struct gl_context *ctx, const struct marshal_cmd_MatrixLoadTransposedEXT *cmd)
{
   GLenum matrixMode = cmd->matrixMode;
   const GLdouble * m = cmd->m;
   CALL_MatrixLoadTransposedEXT(ctx->CurrentServerDispatch, (matrixMode, m));
}
void GLAPIENTRY
_mesa_marshal_MatrixLoadTransposedEXT(GLenum matrixMode, const GLdouble * m)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MatrixLoadTransposedEXT);
   struct marshal_cmd_MatrixLoadTransposedEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MatrixLoadTransposedEXT, cmd_size);
   cmd->matrixMode = matrixMode;
   memcpy(cmd->m, m, 16 * sizeof(GLdouble));
}


/* MatrixMultTransposefEXT: marshalled asynchronously */
struct marshal_cmd_MatrixMultTransposefEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum matrixMode;
   GLfloat m[16];
};
void
_mesa_unmarshal_MatrixMultTransposefEXT(struct gl_context *ctx, const struct marshal_cmd_MatrixMultTransposefEXT *cmd)
{
   GLenum matrixMode = cmd->matrixMode;
   const GLfloat * m = cmd->m;
   CALL_MatrixMultTransposefEXT(ctx->CurrentServerDispatch, (matrixMode, m));
}
void GLAPIENTRY
_mesa_marshal_MatrixMultTransposefEXT(GLenum matrixMode, const GLfloat * m)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MatrixMultTransposefEXT);
   struct marshal_cmd_MatrixMultTransposefEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MatrixMultTransposefEXT, cmd_size);
   cmd->matrixMode = matrixMode;
   memcpy(cmd->m, m, 16 * sizeof(GLfloat));
}


/* MatrixMultTransposedEXT: marshalled asynchronously */
struct marshal_cmd_MatrixMultTransposedEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum matrixMode;
   GLdouble m[16];
};
void
_mesa_unmarshal_MatrixMultTransposedEXT(struct gl_context *ctx, const struct marshal_cmd_MatrixMultTransposedEXT *cmd)
{
   GLenum matrixMode = cmd->matrixMode;
   const GLdouble * m = cmd->m;
   CALL_MatrixMultTransposedEXT(ctx->CurrentServerDispatch, (matrixMode, m));
}
void GLAPIENTRY
_mesa_marshal_MatrixMultTransposedEXT(GLenum matrixMode, const GLdouble * m)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_MatrixMultTransposedEXT);
   struct marshal_cmd_MatrixMultTransposedEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MatrixMultTransposedEXT, cmd_size);
   cmd->matrixMode = matrixMode;
   memcpy(cmd->m, m, 16 * sizeof(GLdouble));
}


/* CompressedTextureImage1DEXT: marshalled asynchronously */
struct marshal_cmd_CompressedTextureImage1DEXT
{
   struct marshal_cmd_base cmd_base;
   GLuint texture;
   GLenum target;
   GLint level;
   GLenum internalFormat;
   GLsizei width;
   GLsizei border;
   GLsizei imageSize;
   const GLvoid * data;
};
void
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
}
void GLAPIENTRY
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
   cmd->target = target;
   cmd->level = level;
   cmd->internalFormat = internalFormat;
   cmd->width = width;
   cmd->border = border;
   cmd->imageSize = imageSize;
   cmd->data = data;
}


/* CompressedTextureImage2DEXT: marshalled asynchronously */
struct marshal_cmd_CompressedTextureImage2DEXT
{
   struct marshal_cmd_base cmd_base;
   GLuint texture;
   GLenum target;
   GLint level;
   GLenum internalFormat;
   GLsizei width;
   GLsizei height;
   GLsizei border;
   GLsizei imageSize;
   const GLvoid * data;
};
void
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
}
void GLAPIENTRY
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
   cmd->target = target;
   cmd->level = level;
   cmd->internalFormat = internalFormat;
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
   GLuint texture;
   GLenum target;
   GLint level;
   GLenum internalFormat;
   GLsizei width;
   GLsizei height;
   GLsizei depth;
   GLsizei border;
   GLsizei imageSize;
   const GLvoid * data;
};
void
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
}
void GLAPIENTRY
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
   cmd->target = target;
   cmd->level = level;
   cmd->internalFormat = internalFormat;
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
   GLuint texture;
   GLenum target;
   GLint level;
   GLint xoffset;
   GLsizei width;
   GLenum format;
   GLsizei imageSize;
   const GLvoid * data;
};
void
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
}
void GLAPIENTRY
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
   cmd->target = target;
   cmd->level = level;
   cmd->xoffset = xoffset;
   cmd->width = width;
   cmd->format = format;
   cmd->imageSize = imageSize;
   cmd->data = data;
}


/* CompressedTextureSubImage2DEXT: marshalled asynchronously */
struct marshal_cmd_CompressedTextureSubImage2DEXT
{
   struct marshal_cmd_base cmd_base;
   GLuint texture;
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
void
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
}
void GLAPIENTRY
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


/* CompressedTextureSubImage3DEXT: marshalled asynchronously */
struct marshal_cmd_CompressedTextureSubImage3DEXT
{
   struct marshal_cmd_base cmd_base;
   GLuint texture;
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
void
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
}
void GLAPIENTRY
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


/* GetCompressedTextureImageEXT: marshalled asynchronously */
struct marshal_cmd_GetCompressedTextureImageEXT
{
   struct marshal_cmd_base cmd_base;
   GLuint texture;
   GLenum target;
   GLint level;
   GLvoid * img;
};
void
_mesa_unmarshal_GetCompressedTextureImageEXT(struct gl_context *ctx, const struct marshal_cmd_GetCompressedTextureImageEXT *cmd)
{
   GLuint texture = cmd->texture;
   GLenum target = cmd->target;
   GLint level = cmd->level;
   GLvoid * img = cmd->img;
   CALL_GetCompressedTextureImageEXT(ctx->CurrentServerDispatch, (texture, target, level, img));
}
void GLAPIENTRY
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
   cmd->target = target;
   cmd->level = level;
   cmd->img = img;
}


/* CompressedMultiTexImage1DEXT: marshalled asynchronously */
struct marshal_cmd_CompressedMultiTexImage1DEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum texunit;
   GLenum target;
   GLint level;
   GLenum internalFormat;
   GLsizei width;
   GLsizei border;
   GLsizei imageSize;
   const GLvoid * data;
};
void
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
}
void GLAPIENTRY
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
   cmd->texunit = texunit;
   cmd->target = target;
   cmd->level = level;
   cmd->internalFormat = internalFormat;
   cmd->width = width;
   cmd->border = border;
   cmd->imageSize = imageSize;
   cmd->data = data;
}


/* CompressedMultiTexImage2DEXT: marshalled asynchronously */
struct marshal_cmd_CompressedMultiTexImage2DEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum texunit;
   GLenum target;
   GLint level;
   GLenum internalFormat;
   GLsizei width;
   GLsizei height;
   GLsizei border;
   GLsizei imageSize;
   const GLvoid * data;
};
void
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
}
void GLAPIENTRY
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
   cmd->texunit = texunit;
   cmd->target = target;
   cmd->level = level;
   cmd->internalFormat = internalFormat;
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
   GLenum texunit;
   GLenum target;
   GLint level;
   GLenum internalFormat;
   GLsizei width;
   GLsizei height;
   GLsizei depth;
   GLsizei border;
   GLsizei imageSize;
   const GLvoid * data;
};
void
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
}
void GLAPIENTRY
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
   cmd->texunit = texunit;
   cmd->target = target;
   cmd->level = level;
   cmd->internalFormat = internalFormat;
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
   GLenum texunit;
   GLenum target;
   GLint level;
   GLint xoffset;
   GLsizei width;
   GLenum format;
   GLsizei imageSize;
   const GLvoid * data;
};
void
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
}
void GLAPIENTRY
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
   cmd->texunit = texunit;
   cmd->target = target;
   cmd->level = level;
   cmd->xoffset = xoffset;
   cmd->width = width;
   cmd->format = format;
   cmd->imageSize = imageSize;
   cmd->data = data;
}


/* CompressedMultiTexSubImage2DEXT: marshalled asynchronously */
struct marshal_cmd_CompressedMultiTexSubImage2DEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum texunit;
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
void
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
}
void GLAPIENTRY
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
   cmd->texunit = texunit;
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


/* CompressedMultiTexSubImage3DEXT: marshalled asynchronously */
struct marshal_cmd_CompressedMultiTexSubImage3DEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum texunit;
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
void
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
}
void GLAPIENTRY
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
   cmd->texunit = texunit;
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


/* GetCompressedMultiTexImageEXT: marshalled asynchronously */
struct marshal_cmd_GetCompressedMultiTexImageEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum texunit;
   GLenum target;
   GLint level;
   GLvoid * img;
};
void
_mesa_unmarshal_GetCompressedMultiTexImageEXT(struct gl_context *ctx, const struct marshal_cmd_GetCompressedMultiTexImageEXT *cmd)
{
   GLenum texunit = cmd->texunit;
   GLenum target = cmd->target;
   GLint level = cmd->level;
   GLvoid * img = cmd->img;
   CALL_GetCompressedMultiTexImageEXT(ctx->CurrentServerDispatch, (texunit, target, level, img));
}
void GLAPIENTRY
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
   cmd->texunit = texunit;
   cmd->target = target;
   cmd->level = level;
   cmd->img = img;
}


/* MapNamedBufferEXT: marshalled synchronously */
GLvoid * GLAPIENTRY
_mesa_marshal_MapNamedBufferEXT(GLuint buffer, GLenum access)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "MapNamedBufferEXT");
   return CALL_MapNamedBufferEXT(ctx->CurrentServerDispatch, (buffer, access));
}


/* GetNamedBufferSubDataEXT: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetNamedBufferSubDataEXT(GLuint buffer, GLintptr offset, GLsizeiptr size, GLvoid * data)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetNamedBufferSubDataEXT");
   CALL_GetNamedBufferSubDataEXT(ctx->CurrentServerDispatch, (buffer, offset, size, data));
}


/* GetNamedBufferPointervEXT: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetNamedBufferPointervEXT(GLuint buffer, GLenum pname, GLvoid ** params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetNamedBufferPointervEXT");
   CALL_GetNamedBufferPointervEXT(ctx->CurrentServerDispatch, (buffer, pname, params));
}


/* GetNamedBufferParameterivEXT: marshalled synchronously */
void GLAPIENTRY
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
void
_mesa_unmarshal_FlushMappedNamedBufferRangeEXT(struct gl_context *ctx, const struct marshal_cmd_FlushMappedNamedBufferRangeEXT *cmd)
{
   GLuint buffer = cmd->buffer;
   GLintptr offset = cmd->offset;
   GLsizeiptr length = cmd->length;
   CALL_FlushMappedNamedBufferRangeEXT(ctx->CurrentServerDispatch, (buffer, offset, length));
}
void GLAPIENTRY
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
GLvoid * GLAPIENTRY
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
   GLuint framebuffer;
   GLenum mode;
};
void
_mesa_unmarshal_FramebufferDrawBufferEXT(struct gl_context *ctx, const struct marshal_cmd_FramebufferDrawBufferEXT *cmd)
{
   GLuint framebuffer = cmd->framebuffer;
   GLenum mode = cmd->mode;
   CALL_FramebufferDrawBufferEXT(ctx->CurrentServerDispatch, (framebuffer, mode));
}
void GLAPIENTRY
_mesa_marshal_FramebufferDrawBufferEXT(GLuint framebuffer, GLenum mode)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_FramebufferDrawBufferEXT);
   struct marshal_cmd_FramebufferDrawBufferEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_FramebufferDrawBufferEXT, cmd_size);
   cmd->framebuffer = framebuffer;
   cmd->mode = mode;
}


/* FramebufferDrawBuffersEXT: marshalled asynchronously */
struct marshal_cmd_FramebufferDrawBuffersEXT
{
   struct marshal_cmd_base cmd_base;
   GLuint framebuffer;
   GLsizei n;
   /* Next safe_mul(n, 1 * sizeof(GLenum)) bytes are GLenum bufs[n] */
};
void
_mesa_unmarshal_FramebufferDrawBuffersEXT(struct gl_context *ctx, const struct marshal_cmd_FramebufferDrawBuffersEXT *cmd)
{
   GLuint framebuffer = cmd->framebuffer;
   GLsizei n = cmd->n;
   GLenum * bufs;
   const char *variable_data = (const char *) (cmd + 1);
   bufs = (GLenum *) variable_data;
   CALL_FramebufferDrawBuffersEXT(ctx->CurrentServerDispatch, (framebuffer, n, bufs));
}
void GLAPIENTRY
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
   GLuint framebuffer;
   GLenum mode;
};
void
_mesa_unmarshal_FramebufferReadBufferEXT(struct gl_context *ctx, const struct marshal_cmd_FramebufferReadBufferEXT *cmd)
{
   GLuint framebuffer = cmd->framebuffer;
   GLenum mode = cmd->mode;
   CALL_FramebufferReadBufferEXT(ctx->CurrentServerDispatch, (framebuffer, mode));
}
void GLAPIENTRY
_mesa_marshal_FramebufferReadBufferEXT(GLuint framebuffer, GLenum mode)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_FramebufferReadBufferEXT);
   struct marshal_cmd_FramebufferReadBufferEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_FramebufferReadBufferEXT, cmd_size);
   cmd->framebuffer = framebuffer;
   cmd->mode = mode;
}


/* GetFramebufferParameterivEXT: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetFramebufferParameterivEXT(GLuint framebuffer, GLenum pname, GLint * param)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetFramebufferParameterivEXT");
   CALL_GetFramebufferParameterivEXT(ctx->CurrentServerDispatch, (framebuffer, pname, param));
}


/* CheckNamedFramebufferStatusEXT: marshalled synchronously */
GLenum GLAPIENTRY
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
   GLuint framebuffer;
   GLenum attachment;
   GLenum textarget;
   GLuint texture;
   GLint level;
};
void
_mesa_unmarshal_NamedFramebufferTexture1DEXT(struct gl_context *ctx, const struct marshal_cmd_NamedFramebufferTexture1DEXT *cmd)
{
   GLuint framebuffer = cmd->framebuffer;
   GLenum attachment = cmd->attachment;
   GLenum textarget = cmd->textarget;
   GLuint texture = cmd->texture;
   GLint level = cmd->level;
   CALL_NamedFramebufferTexture1DEXT(ctx->CurrentServerDispatch, (framebuffer, attachment, textarget, texture, level));
}
void GLAPIENTRY
_mesa_marshal_NamedFramebufferTexture1DEXT(GLuint framebuffer, GLenum attachment, GLenum textarget, GLuint texture, GLint level)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_NamedFramebufferTexture1DEXT);
   struct marshal_cmd_NamedFramebufferTexture1DEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_NamedFramebufferTexture1DEXT, cmd_size);
   cmd->framebuffer = framebuffer;
   cmd->attachment = attachment;
   cmd->textarget = textarget;
   cmd->texture = texture;
   cmd->level = level;
}


/* NamedFramebufferTexture2DEXT: marshalled asynchronously */
struct marshal_cmd_NamedFramebufferTexture2DEXT
{
   struct marshal_cmd_base cmd_base;
   GLuint framebuffer;
   GLenum attachment;
   GLenum textarget;
   GLuint texture;
   GLint level;
};
void
_mesa_unmarshal_NamedFramebufferTexture2DEXT(struct gl_context *ctx, const struct marshal_cmd_NamedFramebufferTexture2DEXT *cmd)
{
   GLuint framebuffer = cmd->framebuffer;
   GLenum attachment = cmd->attachment;
   GLenum textarget = cmd->textarget;
   GLuint texture = cmd->texture;
   GLint level = cmd->level;
   CALL_NamedFramebufferTexture2DEXT(ctx->CurrentServerDispatch, (framebuffer, attachment, textarget, texture, level));
}
void GLAPIENTRY
_mesa_marshal_NamedFramebufferTexture2DEXT(GLuint framebuffer, GLenum attachment, GLenum textarget, GLuint texture, GLint level)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_NamedFramebufferTexture2DEXT);
   struct marshal_cmd_NamedFramebufferTexture2DEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_NamedFramebufferTexture2DEXT, cmd_size);
   cmd->framebuffer = framebuffer;
   cmd->attachment = attachment;
   cmd->textarget = textarget;
   cmd->texture = texture;
   cmd->level = level;
}


/* NamedFramebufferTexture3DEXT: marshalled asynchronously */
struct marshal_cmd_NamedFramebufferTexture3DEXT
{
   struct marshal_cmd_base cmd_base;
   GLuint framebuffer;
   GLenum attachment;
   GLenum textarget;
   GLuint texture;
   GLint level;
   GLint zoffset;
};
void
_mesa_unmarshal_NamedFramebufferTexture3DEXT(struct gl_context *ctx, const struct marshal_cmd_NamedFramebufferTexture3DEXT *cmd)
{
   GLuint framebuffer = cmd->framebuffer;
   GLenum attachment = cmd->attachment;
   GLenum textarget = cmd->textarget;
   GLuint texture = cmd->texture;
   GLint level = cmd->level;
   GLint zoffset = cmd->zoffset;
   CALL_NamedFramebufferTexture3DEXT(ctx->CurrentServerDispatch, (framebuffer, attachment, textarget, texture, level, zoffset));
}
void GLAPIENTRY
_mesa_marshal_NamedFramebufferTexture3DEXT(GLuint framebuffer, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_NamedFramebufferTexture3DEXT);
   struct marshal_cmd_NamedFramebufferTexture3DEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_NamedFramebufferTexture3DEXT, cmd_size);
   cmd->framebuffer = framebuffer;
   cmd->attachment = attachment;
   cmd->textarget = textarget;
   cmd->texture = texture;
   cmd->level = level;
   cmd->zoffset = zoffset;
}


/* NamedFramebufferRenderbufferEXT: marshalled asynchronously */
struct marshal_cmd_NamedFramebufferRenderbufferEXT
{
   struct marshal_cmd_base cmd_base;
   GLuint framebuffer;
   GLenum attachment;
   GLenum renderbuffertarget;
   GLuint renderbuffer;
};
void
_mesa_unmarshal_NamedFramebufferRenderbufferEXT(struct gl_context *ctx, const struct marshal_cmd_NamedFramebufferRenderbufferEXT *cmd)
{
   GLuint framebuffer = cmd->framebuffer;
   GLenum attachment = cmd->attachment;
   GLenum renderbuffertarget = cmd->renderbuffertarget;
   GLuint renderbuffer = cmd->renderbuffer;
   CALL_NamedFramebufferRenderbufferEXT(ctx->CurrentServerDispatch, (framebuffer, attachment, renderbuffertarget, renderbuffer));
}
void GLAPIENTRY
_mesa_marshal_NamedFramebufferRenderbufferEXT(GLuint framebuffer, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_NamedFramebufferRenderbufferEXT);
   struct marshal_cmd_NamedFramebufferRenderbufferEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_NamedFramebufferRenderbufferEXT, cmd_size);
   cmd->framebuffer = framebuffer;
   cmd->attachment = attachment;
   cmd->renderbuffertarget = renderbuffertarget;
   cmd->renderbuffer = renderbuffer;
}


/* GetNamedFramebufferAttachmentParameterivEXT: marshalled synchronously */
void GLAPIENTRY
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
   GLuint renderbuffer;
   GLenum internalformat;
   GLsizei width;
   GLsizei height;
};
void
_mesa_unmarshal_NamedRenderbufferStorageEXT(struct gl_context *ctx, const struct marshal_cmd_NamedRenderbufferStorageEXT *cmd)
{
   GLuint renderbuffer = cmd->renderbuffer;
   GLenum internalformat = cmd->internalformat;
   GLsizei width = cmd->width;
   GLsizei height = cmd->height;
   CALL_NamedRenderbufferStorageEXT(ctx->CurrentServerDispatch, (renderbuffer, internalformat, width, height));
}
void GLAPIENTRY
_mesa_marshal_NamedRenderbufferStorageEXT(GLuint renderbuffer, GLenum internalformat, GLsizei width, GLsizei height)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_NamedRenderbufferStorageEXT);
   struct marshal_cmd_NamedRenderbufferStorageEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_NamedRenderbufferStorageEXT, cmd_size);
   cmd->renderbuffer = renderbuffer;
   cmd->internalformat = internalformat;
   cmd->width = width;
   cmd->height = height;
}


/* GetNamedRenderbufferParameterivEXT: marshalled synchronously */
void GLAPIENTRY
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
   GLuint texture;
   GLenum target;
};
void
_mesa_unmarshal_GenerateTextureMipmapEXT(struct gl_context *ctx, const struct marshal_cmd_GenerateTextureMipmapEXT *cmd)
{
   GLuint texture = cmd->texture;
   GLenum target = cmd->target;
   CALL_GenerateTextureMipmapEXT(ctx->CurrentServerDispatch, (texture, target));
}
void GLAPIENTRY
_mesa_marshal_GenerateTextureMipmapEXT(GLuint texture, GLenum target)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_GenerateTextureMipmapEXT);
   struct marshal_cmd_GenerateTextureMipmapEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_GenerateTextureMipmapEXT, cmd_size);
   cmd->texture = texture;
   cmd->target = target;
}


/* GenerateMultiTexMipmapEXT: marshalled asynchronously */
struct marshal_cmd_GenerateMultiTexMipmapEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum texunit;
   GLenum target;
};
void
_mesa_unmarshal_GenerateMultiTexMipmapEXT(struct gl_context *ctx, const struct marshal_cmd_GenerateMultiTexMipmapEXT *cmd)
{
   GLenum texunit = cmd->texunit;
   GLenum target = cmd->target;
   CALL_GenerateMultiTexMipmapEXT(ctx->CurrentServerDispatch, (texunit, target));
}
void GLAPIENTRY
_mesa_marshal_GenerateMultiTexMipmapEXT(GLenum texunit, GLenum target)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_GenerateMultiTexMipmapEXT);
   struct marshal_cmd_GenerateMultiTexMipmapEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_GenerateMultiTexMipmapEXT, cmd_size);
   cmd->texunit = texunit;
   cmd->target = target;
}


/* NamedRenderbufferStorageMultisampleEXT: marshalled asynchronously */
struct marshal_cmd_NamedRenderbufferStorageMultisampleEXT
{
   struct marshal_cmd_base cmd_base;
   GLuint renderbuffer;
   GLsizei samples;
   GLenum internalformat;
   GLsizei width;
   GLsizei height;
};
void
_mesa_unmarshal_NamedRenderbufferStorageMultisampleEXT(struct gl_context *ctx, const struct marshal_cmd_NamedRenderbufferStorageMultisampleEXT *cmd)
{
   GLuint renderbuffer = cmd->renderbuffer;
   GLsizei samples = cmd->samples;
   GLenum internalformat = cmd->internalformat;
   GLsizei width = cmd->width;
   GLsizei height = cmd->height;
   CALL_NamedRenderbufferStorageMultisampleEXT(ctx->CurrentServerDispatch, (renderbuffer, samples, internalformat, width, height));
}
void GLAPIENTRY
_mesa_marshal_NamedRenderbufferStorageMultisampleEXT(GLuint renderbuffer, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_NamedRenderbufferStorageMultisampleEXT);
   struct marshal_cmd_NamedRenderbufferStorageMultisampleEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_NamedRenderbufferStorageMultisampleEXT, cmd_size);
   cmd->renderbuffer = renderbuffer;
   cmd->samples = samples;
   cmd->internalformat = internalformat;
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
void
_mesa_unmarshal_NamedCopyBufferSubDataEXT(struct gl_context *ctx, const struct marshal_cmd_NamedCopyBufferSubDataEXT *cmd)
{
   GLuint readBuffer = cmd->readBuffer;
   GLuint writeBuffer = cmd->writeBuffer;
   GLintptr readOffset = cmd->readOffset;
   GLintptr writeOffset = cmd->writeOffset;
   GLsizeiptr size = cmd->size;
   CALL_NamedCopyBufferSubDataEXT(ctx->CurrentServerDispatch, (readBuffer, writeBuffer, readOffset, writeOffset, size));
}
void GLAPIENTRY
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
   GLuint vaobj;
   GLuint buffer;
   GLint size;
   GLenum type;
   GLsizei stride;
   GLintptr offset;
};
void
_mesa_unmarshal_VertexArrayVertexOffsetEXT(struct gl_context *ctx, const struct marshal_cmd_VertexArrayVertexOffsetEXT *cmd)
{
   GLuint vaobj = cmd->vaobj;
   GLuint buffer = cmd->buffer;
   GLint size = cmd->size;
   GLenum type = cmd->type;
   GLsizei stride = cmd->stride;
   GLintptr offset = cmd->offset;
   CALL_VertexArrayVertexOffsetEXT(ctx->CurrentServerDispatch, (vaobj, buffer, size, type, stride, offset));
}
void GLAPIENTRY
_mesa_marshal_VertexArrayVertexOffsetEXT(GLuint vaobj, GLuint buffer, GLint size, GLenum type, GLsizei stride, GLintptr offset)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexArrayVertexOffsetEXT);
   struct marshal_cmd_VertexArrayVertexOffsetEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexArrayVertexOffsetEXT, cmd_size);
   cmd->vaobj = vaobj;
   cmd->buffer = buffer;
   cmd->size = size;
   cmd->type = type;
   cmd->stride = stride;
   cmd->offset = offset;
   if (COMPAT) _mesa_glthread_DSAAttribPointer(ctx, vaobj, buffer, VERT_ATTRIB_POS, size, type, stride, offset);
}


/* VertexArrayColorOffsetEXT: marshalled asynchronously */
struct marshal_cmd_VertexArrayColorOffsetEXT
{
   struct marshal_cmd_base cmd_base;
   GLuint vaobj;
   GLuint buffer;
   GLint size;
   GLenum type;
   GLsizei stride;
   GLintptr offset;
};
void
_mesa_unmarshal_VertexArrayColorOffsetEXT(struct gl_context *ctx, const struct marshal_cmd_VertexArrayColorOffsetEXT *cmd)
{
   GLuint vaobj = cmd->vaobj;
   GLuint buffer = cmd->buffer;
   GLint size = cmd->size;
   GLenum type = cmd->type;
   GLsizei stride = cmd->stride;
   GLintptr offset = cmd->offset;
   CALL_VertexArrayColorOffsetEXT(ctx->CurrentServerDispatch, (vaobj, buffer, size, type, stride, offset));
}
void GLAPIENTRY
_mesa_marshal_VertexArrayColorOffsetEXT(GLuint vaobj, GLuint buffer, GLint size, GLenum type, GLsizei stride, GLintptr offset)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexArrayColorOffsetEXT);
   struct marshal_cmd_VertexArrayColorOffsetEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexArrayColorOffsetEXT, cmd_size);
   cmd->vaobj = vaobj;
   cmd->buffer = buffer;
   cmd->size = size;
   cmd->type = type;
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
void
_mesa_unmarshal_VertexArrayEdgeFlagOffsetEXT(struct gl_context *ctx, const struct marshal_cmd_VertexArrayEdgeFlagOffsetEXT *cmd)
{
   GLuint vaobj = cmd->vaobj;
   GLuint buffer = cmd->buffer;
   GLsizei stride = cmd->stride;
   GLintptr offset = cmd->offset;
   CALL_VertexArrayEdgeFlagOffsetEXT(ctx->CurrentServerDispatch, (vaobj, buffer, stride, offset));
}
void GLAPIENTRY
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
   GLuint vaobj;
   GLuint buffer;
   GLenum type;
   GLsizei stride;
   GLintptr offset;
};
void
_mesa_unmarshal_VertexArrayIndexOffsetEXT(struct gl_context *ctx, const struct marshal_cmd_VertexArrayIndexOffsetEXT *cmd)
{
   GLuint vaobj = cmd->vaobj;
   GLuint buffer = cmd->buffer;
   GLenum type = cmd->type;
   GLsizei stride = cmd->stride;
   GLintptr offset = cmd->offset;
   CALL_VertexArrayIndexOffsetEXT(ctx->CurrentServerDispatch, (vaobj, buffer, type, stride, offset));
}
void GLAPIENTRY
_mesa_marshal_VertexArrayIndexOffsetEXT(GLuint vaobj, GLuint buffer, GLenum type, GLsizei stride, GLintptr offset)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexArrayIndexOffsetEXT);
   struct marshal_cmd_VertexArrayIndexOffsetEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexArrayIndexOffsetEXT, cmd_size);
   cmd->vaobj = vaobj;
   cmd->buffer = buffer;
   cmd->type = type;
   cmd->stride = stride;
   cmd->offset = offset;
   if (COMPAT) _mesa_glthread_DSAAttribPointer(ctx, vaobj, buffer, VERT_ATTRIB_COLOR_INDEX, 1, type, stride, offset);
}


/* VertexArrayNormalOffsetEXT: marshalled asynchronously */
struct marshal_cmd_VertexArrayNormalOffsetEXT
{
   struct marshal_cmd_base cmd_base;
   GLuint vaobj;
   GLuint buffer;
   GLenum type;
   GLsizei stride;
   GLintptr offset;
};
void
_mesa_unmarshal_VertexArrayNormalOffsetEXT(struct gl_context *ctx, const struct marshal_cmd_VertexArrayNormalOffsetEXT *cmd)
{
   GLuint vaobj = cmd->vaobj;
   GLuint buffer = cmd->buffer;
   GLenum type = cmd->type;
   GLsizei stride = cmd->stride;
   GLintptr offset = cmd->offset;
   CALL_VertexArrayNormalOffsetEXT(ctx->CurrentServerDispatch, (vaobj, buffer, type, stride, offset));
}
void GLAPIENTRY
_mesa_marshal_VertexArrayNormalOffsetEXT(GLuint vaobj, GLuint buffer, GLenum type, GLsizei stride, GLintptr offset)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexArrayNormalOffsetEXT);
   struct marshal_cmd_VertexArrayNormalOffsetEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexArrayNormalOffsetEXT, cmd_size);
   cmd->vaobj = vaobj;
   cmd->buffer = buffer;
   cmd->type = type;
   cmd->stride = stride;
   cmd->offset = offset;
   if (COMPAT) _mesa_glthread_DSAAttribPointer(ctx, vaobj, buffer, VERT_ATTRIB_NORMAL, 3, type, stride, offset);
}


/* VertexArrayTexCoordOffsetEXT: marshalled asynchronously */
struct marshal_cmd_VertexArrayTexCoordOffsetEXT
{
   struct marshal_cmd_base cmd_base;
   GLuint vaobj;
   GLuint buffer;
   GLint size;
   GLenum type;
   GLsizei stride;
   GLintptr offset;
};
void
_mesa_unmarshal_VertexArrayTexCoordOffsetEXT(struct gl_context *ctx, const struct marshal_cmd_VertexArrayTexCoordOffsetEXT *cmd)
{
   GLuint vaobj = cmd->vaobj;
   GLuint buffer = cmd->buffer;
   GLint size = cmd->size;
   GLenum type = cmd->type;
   GLsizei stride = cmd->stride;
   GLintptr offset = cmd->offset;
   CALL_VertexArrayTexCoordOffsetEXT(ctx->CurrentServerDispatch, (vaobj, buffer, size, type, stride, offset));
}
void GLAPIENTRY
_mesa_marshal_VertexArrayTexCoordOffsetEXT(GLuint vaobj, GLuint buffer, GLint size, GLenum type, GLsizei stride, GLintptr offset)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexArrayTexCoordOffsetEXT);
   struct marshal_cmd_VertexArrayTexCoordOffsetEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexArrayTexCoordOffsetEXT, cmd_size);
   cmd->vaobj = vaobj;
   cmd->buffer = buffer;
   cmd->size = size;
   cmd->type = type;
   cmd->stride = stride;
   cmd->offset = offset;
   if (COMPAT) _mesa_glthread_DSAAttribPointer(ctx, vaobj, buffer, VERT_ATTRIB_TEX(ctx->GLThread.ClientActiveTexture), size, type, stride, offset);
}


/* VertexArrayMultiTexCoordOffsetEXT: marshalled asynchronously */
struct marshal_cmd_VertexArrayMultiTexCoordOffsetEXT
{
   struct marshal_cmd_base cmd_base;
   GLuint vaobj;
   GLuint buffer;
   GLenum texunit;
   GLint size;
   GLenum type;
   GLsizei stride;
   GLintptr offset;
};
void
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
}
void GLAPIENTRY
_mesa_marshal_VertexArrayMultiTexCoordOffsetEXT(GLuint vaobj, GLuint buffer, GLenum texunit, GLint size, GLenum type, GLsizei stride, GLintptr offset)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexArrayMultiTexCoordOffsetEXT);
   struct marshal_cmd_VertexArrayMultiTexCoordOffsetEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexArrayMultiTexCoordOffsetEXT, cmd_size);
   cmd->vaobj = vaobj;
   cmd->buffer = buffer;
   cmd->texunit = texunit;
   cmd->size = size;
   cmd->type = type;
   cmd->stride = stride;
   cmd->offset = offset;
   if (COMPAT) _mesa_glthread_DSAAttribPointer(ctx, vaobj, buffer, VERT_ATTRIB_TEX(texunit - GL_TEXTURE0), size, type, stride, offset);
}


/* VertexArrayFogCoordOffsetEXT: marshalled asynchronously */
struct marshal_cmd_VertexArrayFogCoordOffsetEXT
{
   struct marshal_cmd_base cmd_base;
   GLuint vaobj;
   GLuint buffer;
   GLenum type;
   GLsizei stride;
   GLintptr offset;
};
void
_mesa_unmarshal_VertexArrayFogCoordOffsetEXT(struct gl_context *ctx, const struct marshal_cmd_VertexArrayFogCoordOffsetEXT *cmd)
{
   GLuint vaobj = cmd->vaobj;
   GLuint buffer = cmd->buffer;
   GLenum type = cmd->type;
   GLsizei stride = cmd->stride;
   GLintptr offset = cmd->offset;
   CALL_VertexArrayFogCoordOffsetEXT(ctx->CurrentServerDispatch, (vaobj, buffer, type, stride, offset));
}
void GLAPIENTRY
_mesa_marshal_VertexArrayFogCoordOffsetEXT(GLuint vaobj, GLuint buffer, GLenum type, GLsizei stride, GLintptr offset)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexArrayFogCoordOffsetEXT);
   struct marshal_cmd_VertexArrayFogCoordOffsetEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexArrayFogCoordOffsetEXT, cmd_size);
   cmd->vaobj = vaobj;
   cmd->buffer = buffer;
   cmd->type = type;
   cmd->stride = stride;
   cmd->offset = offset;
   if (COMPAT) _mesa_glthread_DSAAttribPointer(ctx, vaobj, buffer, VERT_ATTRIB_FOG, 1, type, stride, offset);
}


/* VertexArraySecondaryColorOffsetEXT: marshalled asynchronously */
struct marshal_cmd_VertexArraySecondaryColorOffsetEXT
{
   struct marshal_cmd_base cmd_base;
   GLuint vaobj;
   GLuint buffer;
   GLint size;
   GLenum type;
   GLsizei stride;
   GLintptr offset;
};
void
_mesa_unmarshal_VertexArraySecondaryColorOffsetEXT(struct gl_context *ctx, const struct marshal_cmd_VertexArraySecondaryColorOffsetEXT *cmd)
{
   GLuint vaobj = cmd->vaobj;
   GLuint buffer = cmd->buffer;
   GLint size = cmd->size;
   GLenum type = cmd->type;
   GLsizei stride = cmd->stride;
   GLintptr offset = cmd->offset;
   CALL_VertexArraySecondaryColorOffsetEXT(ctx->CurrentServerDispatch, (vaobj, buffer, size, type, stride, offset));
}
void GLAPIENTRY
_mesa_marshal_VertexArraySecondaryColorOffsetEXT(GLuint vaobj, GLuint buffer, GLint size, GLenum type, GLsizei stride, GLintptr offset)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_VertexArraySecondaryColorOffsetEXT);
   struct marshal_cmd_VertexArraySecondaryColorOffsetEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_VertexArraySecondaryColorOffsetEXT, cmd_size);
   cmd->vaobj = vaobj;
   cmd->buffer = buffer;
   cmd->size = size;
   cmd->type = type;
   cmd->stride = stride;
   cmd->offset = offset;
   if (COMPAT) _mesa_glthread_DSAAttribPointer(ctx, vaobj, buffer, VERT_ATTRIB_COLOR1, size, type, stride, offset);
}


/* VertexArrayVertexAttribOffsetEXT: marshalled asynchronously */
struct marshal_cmd_VertexArrayVertexAttribOffsetEXT
{
   struct marshal_cmd_base cmd_base;
   GLboolean normalized;
   GLuint vaobj;
   GLuint buffer;
   GLuint index;
   GLint size;
   GLenum type;
   GLsizei stride;
   GLintptr offset;
};
void
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
}
void GLAPIENTRY
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
   cmd->type = type;
   cmd->normalized = normalized;
   cmd->stride = stride;
   cmd->offset = offset;
   if (COMPAT) _mesa_glthread_DSAAttribPointer(ctx, vaobj, buffer, VERT_ATTRIB_GENERIC(index), size, type, stride, offset);
}


/* VertexArrayVertexAttribIOffsetEXT: marshalled asynchronously */
struct marshal_cmd_VertexArrayVertexAttribIOffsetEXT
{
   struct marshal_cmd_base cmd_base;
   GLuint vaobj;
   GLuint buffer;
   GLuint index;
   GLint size;
   GLenum type;
   GLsizei stride;
   GLintptr offset;
};
void
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
}
void GLAPIENTRY
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
   cmd->type = type;
   cmd->stride = stride;
   cmd->offset = offset;
   if (COMPAT) _mesa_glthread_DSAAttribPointer(ctx, vaobj, buffer, VERT_ATTRIB_GENERIC(index), size, type, stride, offset);
}


/* EnableVertexArrayEXT: marshalled asynchronously */
struct marshal_cmd_EnableVertexArrayEXT
{
   struct marshal_cmd_base cmd_base;
   GLuint vaobj;
   GLenum array;
};
void
_mesa_unmarshal_EnableVertexArrayEXT(struct gl_context *ctx, const struct marshal_cmd_EnableVertexArrayEXT *cmd)
{
   GLuint vaobj = cmd->vaobj;
   GLenum array = cmd->array;
   CALL_EnableVertexArrayEXT(ctx->CurrentServerDispatch, (vaobj, array));
}
void GLAPIENTRY
_mesa_marshal_EnableVertexArrayEXT(GLuint vaobj, GLenum array)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_EnableVertexArrayEXT);
   struct marshal_cmd_EnableVertexArrayEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_EnableVertexArrayEXT, cmd_size);
   cmd->vaobj = vaobj;
   cmd->array = array;
   if (COMPAT) _mesa_glthread_ClientState(ctx, &vaobj, _mesa_array_to_attrib(ctx, array), true);
}


/* DisableVertexArrayEXT: marshalled asynchronously */
struct marshal_cmd_DisableVertexArrayEXT
{
   struct marshal_cmd_base cmd_base;
   GLuint vaobj;
   GLenum array;
};
void
_mesa_unmarshal_DisableVertexArrayEXT(struct gl_context *ctx, const struct marshal_cmd_DisableVertexArrayEXT *cmd)
{
   GLuint vaobj = cmd->vaobj;
   GLenum array = cmd->array;
   CALL_DisableVertexArrayEXT(ctx->CurrentServerDispatch, (vaobj, array));
}
void GLAPIENTRY
_mesa_marshal_DisableVertexArrayEXT(GLuint vaobj, GLenum array)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_DisableVertexArrayEXT);
   struct marshal_cmd_DisableVertexArrayEXT *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_DisableVertexArrayEXT, cmd_size);
   cmd->vaobj = vaobj;
   cmd->array = array;
   if (COMPAT) _mesa_glthread_ClientState(ctx, &vaobj, _mesa_array_to_attrib(ctx, array), false);
}


/* EnableVertexArrayAttribEXT: marshalled asynchronously */
struct marshal_cmd_EnableVertexArrayAttribEXT
{
   struct marshal_cmd_base cmd_base;
   GLuint vaobj;
   GLuint index;
};
void
_mesa_unmarshal_EnableVertexArrayAttribEXT(struct gl_context *ctx, const struct marshal_cmd_EnableVertexArrayAttribEXT *cmd)
{
   GLuint vaobj = cmd->vaobj;
   GLuint index = cmd->index;
   CALL_EnableVertexArrayAttribEXT(ctx->CurrentServerDispatch, (vaobj, index));
}
void GLAPIENTRY
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
void
_mesa_unmarshal_DisableVertexArrayAttribEXT(struct gl_context *ctx, const struct marshal_cmd_DisableVertexArrayAttribEXT *cmd)
{
   GLuint vaobj = cmd->vaobj;
   GLuint index = cmd->index;
   CALL_DisableVertexArrayAttribEXT(ctx->CurrentServerDispatch, (vaobj, index));
}
void GLAPIENTRY
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
void GLAPIENTRY
_mesa_marshal_GetVertexArrayIntegervEXT(GLuint vaobj, GLenum pname, GLint* param)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetVertexArrayIntegervEXT");
   CALL_GetVertexArrayIntegervEXT(ctx->CurrentServerDispatch, (vaobj, pname, param));
}


/* GetVertexArrayPointervEXT: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetVertexArrayPointervEXT(GLuint vaobj, GLenum pname, GLvoid** param)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetVertexArrayPointervEXT");
   CALL_GetVertexArrayPointervEXT(ctx->CurrentServerDispatch, (vaobj, pname, param));
}


/* GetVertexArrayIntegeri_vEXT: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetVertexArrayIntegeri_vEXT(GLuint vaobj, GLuint index, GLenum pname, GLint* param)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetVertexArrayIntegeri_vEXT");
   CALL_GetVertexArrayIntegeri_vEXT(ctx->CurrentServerDispatch, (vaobj, index, pname, param));
}


/* GetVertexArrayPointeri_vEXT: marshalled synchronously */
void GLAPIENTRY
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
   GLuint program;
   GLenum target;
   GLenum format;
   GLsizei len;
   /* Next len bytes are GLvoid string[len] */
};
void
_mesa_unmarshal_NamedProgramStringEXT(struct gl_context *ctx, const struct marshal_cmd_NamedProgramStringEXT *cmd)
{
   GLuint program = cmd->program;
   GLenum target = cmd->target;
   GLenum format = cmd->format;
   GLsizei len = cmd->len;
   GLvoid * string;
   const char *variable_data = (const char *) (cmd + 1);
   string = (GLvoid *) variable_data;
   CALL_NamedProgramStringEXT(ctx->CurrentServerDispatch, (program, target, format, len, string));
}
void GLAPIENTRY
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
   cmd->target = target;
   cmd->format = format;
   cmd->len = len;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, string, string_size);
}


