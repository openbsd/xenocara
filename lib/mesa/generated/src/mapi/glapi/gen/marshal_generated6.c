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

/* WindowPos4dMESA: marshalled asynchronously */
struct marshal_cmd_WindowPos4dMESA
{
   struct marshal_cmd_base cmd_base;
   GLdouble x;
   GLdouble y;
   GLdouble z;
   GLdouble w;
};
void
_mesa_unmarshal_WindowPos4dMESA(struct gl_context *ctx, const struct marshal_cmd_WindowPos4dMESA *cmd)
{
   const GLdouble x = cmd->x;
   const GLdouble y = cmd->y;
   const GLdouble z = cmd->z;
   const GLdouble w = cmd->w;
   CALL_WindowPos4dMESA(ctx->CurrentServerDispatch, (x, y, z, w));
}
void GLAPIENTRY
_mesa_marshal_WindowPos4dMESA(GLdouble x, GLdouble y, GLdouble z, GLdouble w)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_WindowPos4dMESA);
   struct marshal_cmd_WindowPos4dMESA *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_WindowPos4dMESA, cmd_size);
   cmd->x = x;
   cmd->y = y;
   cmd->z = z;
   cmd->w = w;
}


/* WindowPos4dvMESA: marshalled asynchronously */
struct marshal_cmd_WindowPos4dvMESA
{
   struct marshal_cmd_base cmd_base;
   GLdouble v[4];
};
void
_mesa_unmarshal_WindowPos4dvMESA(struct gl_context *ctx, const struct marshal_cmd_WindowPos4dvMESA *cmd)
{
   const GLdouble * v = cmd->v;
   CALL_WindowPos4dvMESA(ctx->CurrentServerDispatch, (v));
}
void GLAPIENTRY
_mesa_marshal_WindowPos4dvMESA(const GLdouble * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_WindowPos4dvMESA);
   struct marshal_cmd_WindowPos4dvMESA *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_WindowPos4dvMESA, cmd_size);
   memcpy(cmd->v, v, 4 * sizeof(GLdouble));
}


/* WindowPos4fMESA: marshalled asynchronously */
struct marshal_cmd_WindowPos4fMESA
{
   struct marshal_cmd_base cmd_base;
   GLfloat x;
   GLfloat y;
   GLfloat z;
   GLfloat w;
};
void
_mesa_unmarshal_WindowPos4fMESA(struct gl_context *ctx, const struct marshal_cmd_WindowPos4fMESA *cmd)
{
   const GLfloat x = cmd->x;
   const GLfloat y = cmd->y;
   const GLfloat z = cmd->z;
   const GLfloat w = cmd->w;
   CALL_WindowPos4fMESA(ctx->CurrentServerDispatch, (x, y, z, w));
}
void GLAPIENTRY
_mesa_marshal_WindowPos4fMESA(GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_WindowPos4fMESA);
   struct marshal_cmd_WindowPos4fMESA *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_WindowPos4fMESA, cmd_size);
   cmd->x = x;
   cmd->y = y;
   cmd->z = z;
   cmd->w = w;
}


/* WindowPos4fvMESA: marshalled asynchronously */
struct marshal_cmd_WindowPos4fvMESA
{
   struct marshal_cmd_base cmd_base;
   GLfloat v[4];
};
void
_mesa_unmarshal_WindowPos4fvMESA(struct gl_context *ctx, const struct marshal_cmd_WindowPos4fvMESA *cmd)
{
   const GLfloat * v = cmd->v;
   CALL_WindowPos4fvMESA(ctx->CurrentServerDispatch, (v));
}
void GLAPIENTRY
_mesa_marshal_WindowPos4fvMESA(const GLfloat * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_WindowPos4fvMESA);
   struct marshal_cmd_WindowPos4fvMESA *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_WindowPos4fvMESA, cmd_size);
   memcpy(cmd->v, v, 4 * sizeof(GLfloat));
}


/* WindowPos4iMESA: marshalled asynchronously */
struct marshal_cmd_WindowPos4iMESA
{
   struct marshal_cmd_base cmd_base;
   GLint x;
   GLint y;
   GLint z;
   GLint w;
};
void
_mesa_unmarshal_WindowPos4iMESA(struct gl_context *ctx, const struct marshal_cmd_WindowPos4iMESA *cmd)
{
   const GLint x = cmd->x;
   const GLint y = cmd->y;
   const GLint z = cmd->z;
   const GLint w = cmd->w;
   CALL_WindowPos4iMESA(ctx->CurrentServerDispatch, (x, y, z, w));
}
void GLAPIENTRY
_mesa_marshal_WindowPos4iMESA(GLint x, GLint y, GLint z, GLint w)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_WindowPos4iMESA);
   struct marshal_cmd_WindowPos4iMESA *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_WindowPos4iMESA, cmd_size);
   cmd->x = x;
   cmd->y = y;
   cmd->z = z;
   cmd->w = w;
}


/* WindowPos4ivMESA: marshalled asynchronously */
struct marshal_cmd_WindowPos4ivMESA
{
   struct marshal_cmd_base cmd_base;
   GLint v[4];
};
void
_mesa_unmarshal_WindowPos4ivMESA(struct gl_context *ctx, const struct marshal_cmd_WindowPos4ivMESA *cmd)
{
   const GLint * v = cmd->v;
   CALL_WindowPos4ivMESA(ctx->CurrentServerDispatch, (v));
}
void GLAPIENTRY
_mesa_marshal_WindowPos4ivMESA(const GLint * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_WindowPos4ivMESA);
   struct marshal_cmd_WindowPos4ivMESA *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_WindowPos4ivMESA, cmd_size);
   memcpy(cmd->v, v, 4 * sizeof(GLint));
}


/* WindowPos4sMESA: marshalled asynchronously */
struct marshal_cmd_WindowPos4sMESA
{
   struct marshal_cmd_base cmd_base;
   GLshort x;
   GLshort y;
   GLshort z;
   GLshort w;
};
void
_mesa_unmarshal_WindowPos4sMESA(struct gl_context *ctx, const struct marshal_cmd_WindowPos4sMESA *cmd)
{
   const GLshort x = cmd->x;
   const GLshort y = cmd->y;
   const GLshort z = cmd->z;
   const GLshort w = cmd->w;
   CALL_WindowPos4sMESA(ctx->CurrentServerDispatch, (x, y, z, w));
}
void GLAPIENTRY
_mesa_marshal_WindowPos4sMESA(GLshort x, GLshort y, GLshort z, GLshort w)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_WindowPos4sMESA);
   struct marshal_cmd_WindowPos4sMESA *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_WindowPos4sMESA, cmd_size);
   cmd->x = x;
   cmd->y = y;
   cmd->z = z;
   cmd->w = w;
}


/* WindowPos4svMESA: marshalled asynchronously */
struct marshal_cmd_WindowPos4svMESA
{
   struct marshal_cmd_base cmd_base;
   GLshort v[4];
};
void
_mesa_unmarshal_WindowPos4svMESA(struct gl_context *ctx, const struct marshal_cmd_WindowPos4svMESA *cmd)
{
   const GLshort * v = cmd->v;
   CALL_WindowPos4svMESA(ctx->CurrentServerDispatch, (v));
}
void GLAPIENTRY
_mesa_marshal_WindowPos4svMESA(const GLshort * v)
{
   GET_CURRENT_CONTEXT(ctx);
   int cmd_size = sizeof(struct marshal_cmd_WindowPos4svMESA);
   struct marshal_cmd_WindowPos4svMESA *cmd;
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_WindowPos4svMESA, cmd_size);
   memcpy(cmd->v, v, 4 * sizeof(GLshort));
}


/* MultiModeDrawArraysIBM: marshalled asynchronously */
struct marshal_cmd_MultiModeDrawArraysIBM
{
   struct marshal_cmd_base cmd_base;
   GLsizei primcount;
   GLint modestride;
   /* Next safe_mul(primcount, 1 * sizeof(GLenum)) bytes are GLenum mode[primcount] */
   /* Next safe_mul(primcount, 1 * sizeof(GLint)) bytes are GLint first[primcount] */
   /* Next safe_mul(primcount, 1 * sizeof(GLsizei)) bytes are GLsizei count[primcount] */
};
void
_mesa_unmarshal_MultiModeDrawArraysIBM(struct gl_context *ctx, const struct marshal_cmd_MultiModeDrawArraysIBM *cmd)
{
   const GLsizei primcount = cmd->primcount;
   const GLint modestride = cmd->modestride;
   GLenum * mode;
   GLint * first;
   GLsizei * count;
   const char *variable_data = (const char *) (cmd + 1);
   mode = (GLenum *) variable_data;
   variable_data += primcount * 1 * sizeof(GLenum);
   first = (GLint *) variable_data;
   variable_data += primcount * 1 * sizeof(GLint);
   count = (GLsizei *) variable_data;
   CALL_MultiModeDrawArraysIBM(ctx->CurrentServerDispatch, (mode, first, count, primcount, modestride));
}
void GLAPIENTRY
_mesa_marshal_MultiModeDrawArraysIBM(const GLenum * mode, const GLint * first, const GLsizei * count, GLsizei primcount, GLint modestride)
{
   GET_CURRENT_CONTEXT(ctx);
   int mode_size = safe_mul(primcount, 1 * sizeof(GLenum));
   int first_size = safe_mul(primcount, 1 * sizeof(GLint));
   int count_size = safe_mul(primcount, 1 * sizeof(GLsizei));
   int cmd_size = sizeof(struct marshal_cmd_MultiModeDrawArraysIBM) + mode_size + first_size + count_size;
   struct marshal_cmd_MultiModeDrawArraysIBM *cmd;
   if (unlikely(mode_size < 0 || (mode_size > 0 && !mode) || first_size < 0 || (first_size > 0 && !first) || count_size < 0 || (count_size > 0 && !count) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "MultiModeDrawArraysIBM");
      CALL_MultiModeDrawArraysIBM(ctx->CurrentServerDispatch, (mode, first, count, primcount, modestride));
      return;
   }
   if (_mesa_glthread_has_non_vbo_vertices(ctx)) {
      _mesa_glthread_finish_before(ctx, "MultiModeDrawArraysIBM");
      CALL_MultiModeDrawArraysIBM(ctx->CurrentServerDispatch, (mode, first, count, primcount, modestride));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiModeDrawArraysIBM, cmd_size);
   cmd->primcount = primcount;
   cmd->modestride = modestride;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, mode, mode_size);
   variable_data += mode_size;
   memcpy(variable_data, first, first_size);
   variable_data += first_size;
   memcpy(variable_data, count, count_size);
}


/* MultiModeDrawElementsIBM: marshalled asynchronously */
struct marshal_cmd_MultiModeDrawElementsIBM
{
   struct marshal_cmd_base cmd_base;
   GLenum16 type;
   GLsizei primcount;
   GLint modestride;
   /* Next safe_mul(primcount, 1 * sizeof(GLenum)) bytes are GLenum mode[primcount] */
   /* Next safe_mul(primcount, 1 * sizeof(GLsizei)) bytes are GLsizei count[primcount] */
   /* Next (sizeof(GLvoid *) * primcount) bytes are GLvoid indices[(sizeof(GLvoid *) * primcount)] */
};
void
_mesa_unmarshal_MultiModeDrawElementsIBM(struct gl_context *ctx, const struct marshal_cmd_MultiModeDrawElementsIBM *cmd)
{
   const GLenum type = cmd->type;
   const GLsizei primcount = cmd->primcount;
   const GLint modestride = cmd->modestride;
   GLenum * mode;
   GLsizei * count;
   GLvoid * indices;
   const char *variable_data = (const char *) (cmd + 1);
   mode = (GLenum *) variable_data;
   variable_data += primcount * 1 * sizeof(GLenum);
   count = (GLsizei *) variable_data;
   variable_data += primcount * 1 * sizeof(GLsizei);
   indices = (GLvoid *) variable_data;
   CALL_MultiModeDrawElementsIBM(ctx->CurrentServerDispatch, (mode, count, type, indices, primcount, modestride));
}
void GLAPIENTRY
_mesa_marshal_MultiModeDrawElementsIBM(const GLenum * mode, const GLsizei * count, GLenum type, const GLvoid * const * indices, GLsizei primcount, GLint modestride)
{
   GET_CURRENT_CONTEXT(ctx);
   int mode_size = safe_mul(primcount, 1 * sizeof(GLenum));
   int count_size = safe_mul(primcount, 1 * sizeof(GLsizei));
   int indices_size = (sizeof(GLvoid *) * primcount);
   int cmd_size = sizeof(struct marshal_cmd_MultiModeDrawElementsIBM) + mode_size + count_size + indices_size;
   struct marshal_cmd_MultiModeDrawElementsIBM *cmd;
   if (unlikely(mode_size < 0 || (mode_size > 0 && !mode) || count_size < 0 || (count_size > 0 && !count) || indices_size < 0 || (indices_size > 0 && !indices) || (unsigned)cmd_size > MARSHAL_MAX_CMD_SIZE)) {
      _mesa_glthread_finish_before(ctx, "MultiModeDrawElementsIBM");
      CALL_MultiModeDrawElementsIBM(ctx->CurrentServerDispatch, (mode, count, type, indices, primcount, modestride));
      return;
   }
   if (_mesa_glthread_has_non_vbo_vertices_or_indices(ctx)) {
      _mesa_glthread_finish_before(ctx, "MultiModeDrawElementsIBM");
      CALL_MultiModeDrawElementsIBM(ctx->CurrentServerDispatch, (mode, count, type, indices, primcount, modestride));
      return;
   }
   cmd = _mesa_glthread_allocate_command(ctx, DISPATCH_CMD_MultiModeDrawElementsIBM, cmd_size);
   cmd->type = type;
   cmd->primcount = primcount;
   cmd->modestride = modestride;
   char *variable_data = (char *) (cmd + 1);
   memcpy(variable_data, mode, mode_size);
   variable_data += mode_size;
   memcpy(variable_data, count, count_size);
   variable_data += count_size;
   memcpy(variable_data, indices, indices_size);
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
   const GLuint index = cmd->index;
   const GLshort x = cmd->x;
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
   const GLuint index = cmd->index;
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
   const GLuint index = cmd->index;
   const GLshort x = cmd->x;
   const GLshort y = cmd->y;
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
   const GLuint index = cmd->index;
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
   const GLuint index = cmd->index;
   const GLshort x = cmd->x;
   const GLshort y = cmd->y;
   const GLshort z = cmd->z;
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
   const GLuint index = cmd->index;
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
   const GLuint index = cmd->index;
   const GLshort x = cmd->x;
   const GLshort y = cmd->y;
   const GLshort z = cmd->z;
   const GLshort w = cmd->w;
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
   const GLuint index = cmd->index;
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
   const GLuint index = cmd->index;
   const GLfloat x = cmd->x;
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
   const GLuint index = cmd->index;
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
   const GLuint index = cmd->index;
   const GLfloat x = cmd->x;
   const GLfloat y = cmd->y;
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
   const GLuint index = cmd->index;
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
   const GLuint index = cmd->index;
   const GLfloat x = cmd->x;
   const GLfloat y = cmd->y;
   const GLfloat z = cmd->z;
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
   const GLuint index = cmd->index;
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
   const GLuint index = cmd->index;
   const GLfloat x = cmd->x;
   const GLfloat y = cmd->y;
   const GLfloat z = cmd->z;
   const GLfloat w = cmd->w;
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
   const GLuint index = cmd->index;
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
   const GLuint index = cmd->index;
   const GLdouble x = cmd->x;
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
   const GLuint index = cmd->index;
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
   const GLuint index = cmd->index;
   const GLdouble x = cmd->x;
   const GLdouble y = cmd->y;
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
   const GLuint index = cmd->index;
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
   const GLuint index = cmd->index;
   const GLdouble x = cmd->x;
   const GLdouble y = cmd->y;
   const GLdouble z = cmd->z;
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
   const GLuint index = cmd->index;
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
   const GLuint index = cmd->index;
   const GLdouble x = cmd->x;
   const GLdouble y = cmd->y;
   const GLdouble z = cmd->z;
   const GLdouble w = cmd->w;
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
   const GLuint index = cmd->index;
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
   const GLuint index = cmd->index;
   const GLubyte x = cmd->x;
   const GLubyte y = cmd->y;
   const GLubyte z = cmd->z;
   const GLubyte w = cmd->w;
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
   const GLuint index = cmd->index;
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
   const GLuint index = cmd->index;
   const GLsizei n = cmd->n;
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
   const GLuint index = cmd->index;
   const GLsizei n = cmd->n;
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
   const GLuint index = cmd->index;
   const GLsizei n = cmd->n;
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
   const GLuint index = cmd->index;
   const GLsizei n = cmd->n;
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
   const GLuint index = cmd->index;
   const GLsizei n = cmd->n;
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
   const GLuint index = cmd->index;
   const GLsizei n = cmd->n;
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
   const GLuint index = cmd->index;
   const GLsizei n = cmd->n;
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
   const GLuint index = cmd->index;
   const GLsizei n = cmd->n;
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
   const GLuint index = cmd->index;
   const GLsizei n = cmd->n;
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
   const GLuint index = cmd->index;
   const GLsizei n = cmd->n;
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
   const GLuint index = cmd->index;
   const GLsizei n = cmd->n;
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
   const GLuint index = cmd->index;
   const GLsizei n = cmd->n;
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
   const GLuint index = cmd->index;
   const GLsizei n = cmd->n;
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
   const GLuint id = cmd->id;
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
   const GLuint id = cmd->id;
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
   GLenum16 swizzle;
   GLuint dst;
   GLuint coord;
};
void
_mesa_unmarshal_PassTexCoordATI(struct gl_context *ctx, const struct marshal_cmd_PassTexCoordATI *cmd)
{
   const GLuint dst = cmd->dst;
   const GLuint coord = cmd->coord;
   const GLenum swizzle = cmd->swizzle;
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
   GLenum16 swizzle;
   GLuint dst;
   GLuint interp;
};
void
_mesa_unmarshal_SampleMapATI(struct gl_context *ctx, const struct marshal_cmd_SampleMapATI *cmd)
{
   const GLuint dst = cmd->dst;
   const GLuint interp = cmd->interp;
   const GLenum swizzle = cmd->swizzle;
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
   GLenum16 op;
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
   const GLenum op = cmd->op;
   const GLuint dst = cmd->dst;
   const GLuint dstMask = cmd->dstMask;
   const GLuint dstMod = cmd->dstMod;
   const GLuint arg1 = cmd->arg1;
   const GLuint arg1Rep = cmd->arg1Rep;
   const GLuint arg1Mod = cmd->arg1Mod;
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
   GLenum16 op;
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
   const GLenum op = cmd->op;
   const GLuint dst = cmd->dst;
   const GLuint dstMask = cmd->dstMask;
   const GLuint dstMod = cmd->dstMod;
   const GLuint arg1 = cmd->arg1;
   const GLuint arg1Rep = cmd->arg1Rep;
   const GLuint arg1Mod = cmd->arg1Mod;
   const GLuint arg2 = cmd->arg2;
   const GLuint arg2Rep = cmd->arg2Rep;
   const GLuint arg2Mod = cmd->arg2Mod;
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
   GLenum16 op;
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
   const GLenum op = cmd->op;
   const GLuint dst = cmd->dst;
   const GLuint dstMask = cmd->dstMask;
   const GLuint dstMod = cmd->dstMod;
   const GLuint arg1 = cmd->arg1;
   const GLuint arg1Rep = cmd->arg1Rep;
   const GLuint arg1Mod = cmd->arg1Mod;
   const GLuint arg2 = cmd->arg2;
   const GLuint arg2Rep = cmd->arg2Rep;
   const GLuint arg2Mod = cmd->arg2Mod;
   const GLuint arg3 = cmd->arg3;
   const GLuint arg3Rep = cmd->arg3Rep;
   const GLuint arg3Mod = cmd->arg3Mod;
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
   GLenum16 op;
   GLuint dst;
   GLuint dstMod;
   GLuint arg1;
   GLuint arg1Rep;
   GLuint arg1Mod;
};
void
_mesa_unmarshal_AlphaFragmentOp1ATI(struct gl_context *ctx, const struct marshal_cmd_AlphaFragmentOp1ATI *cmd)
{
   const GLenum op = cmd->op;
   const GLuint dst = cmd->dst;
   const GLuint dstMod = cmd->dstMod;
   const GLuint arg1 = cmd->arg1;
   const GLuint arg1Rep = cmd->arg1Rep;
   const GLuint arg1Mod = cmd->arg1Mod;
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
   GLenum16 op;
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
   const GLenum op = cmd->op;
   const GLuint dst = cmd->dst;
   const GLuint dstMod = cmd->dstMod;
   const GLuint arg1 = cmd->arg1;
   const GLuint arg1Rep = cmd->arg1Rep;
   const GLuint arg1Mod = cmd->arg1Mod;
   const GLuint arg2 = cmd->arg2;
   const GLuint arg2Rep = cmd->arg2Rep;
   const GLuint arg2Mod = cmd->arg2Mod;
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
   GLenum16 op;
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
   const GLenum op = cmd->op;
   const GLuint dst = cmd->dst;
   const GLuint dstMod = cmd->dstMod;
   const GLuint arg1 = cmd->arg1;
   const GLuint arg1Rep = cmd->arg1Rep;
   const GLuint arg1Mod = cmd->arg1Mod;
   const GLuint arg2 = cmd->arg2;
   const GLuint arg2Rep = cmd->arg2Rep;
   const GLuint arg2Mod = cmd->arg2Mod;
   const GLuint arg3 = cmd->arg3;
   const GLuint arg3Rep = cmd->arg3Rep;
   const GLuint arg3Mod = cmd->arg3Mod;
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
   const GLuint dst = cmd->dst;
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
   GLenum16 face;
};
void
_mesa_unmarshal_ActiveStencilFaceEXT(struct gl_context *ctx, const struct marshal_cmd_ActiveStencilFaceEXT *cmd)
{
   const GLenum face = cmd->face;
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
   const GLclampd zmin = cmd->zmin;
   const GLclampd zmax = cmd->zmax;
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
   GLenum16 target;
   GLuint renderbuffer;
};
void
_mesa_unmarshal_BindRenderbufferEXT(struct gl_context *ctx, const struct marshal_cmd_BindRenderbufferEXT *cmd)
{
   const GLenum target = cmd->target;
   const GLuint renderbuffer = cmd->renderbuffer;
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
   GLenum16 target;
   GLuint framebuffer;
};
void
_mesa_unmarshal_BindFramebufferEXT(struct gl_context *ctx, const struct marshal_cmd_BindFramebufferEXT *cmd)
{
   const GLenum target = cmd->target;
   const GLuint framebuffer = cmd->framebuffer;
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
   GLenum16 mode;
};
void
_mesa_unmarshal_ProvokingVertex(struct gl_context *ctx, const struct marshal_cmd_ProvokingVertex *cmd)
{
   const GLenum mode = cmd->mode;
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
   const GLuint buf = cmd->buf;
   const GLboolean r = cmd->r;
   const GLboolean g = cmd->g;
   const GLboolean b = cmd->b;
   const GLboolean a = cmd->a;
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
   GLenum16 target;
   GLuint index;
};
void
_mesa_unmarshal_Enablei(struct gl_context *ctx, const struct marshal_cmd_Enablei *cmd)
{
   const GLenum target = cmd->target;
   const GLuint index = cmd->index;
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
   GLenum16 target;
   GLuint index;
};
void
_mesa_unmarshal_Disablei(struct gl_context *ctx, const struct marshal_cmd_Disablei *cmd)
{
   const GLenum target = cmd->target;
   const GLuint index = cmd->index;
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
   const GLsizei n = cmd->n;
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
   const GLuint monitor = cmd->monitor;
   const GLboolean enable = cmd->enable;
   const GLuint group = cmd->group;
   const GLint numCounters = cmd->numCounters;
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
   const GLuint monitor = cmd->monitor;
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
   const GLuint monitor = cmd->monitor;
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
   GLsizei width;
   GLsizei height;
   GLsizei depth;
};
void
_mesa_unmarshal_CopyImageSubDataNV(struct gl_context *ctx, const struct marshal_cmd_CopyImageSubDataNV *cmd)
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
   const GLsizei width = cmd->width;
   const GLsizei height = cmd->height;
   const GLsizei depth = cmd->depth;
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
   GLenum16 matrixMode;
   GLfloat m[16];
};
void
_mesa_unmarshal_MatrixLoadfEXT(struct gl_context *ctx, const struct marshal_cmd_MatrixLoadfEXT *cmd)
{
   const GLenum matrixMode = cmd->matrixMode;
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
   GLenum16 matrixMode;
   GLdouble m[16];
};
void
_mesa_unmarshal_MatrixLoaddEXT(struct gl_context *ctx, const struct marshal_cmd_MatrixLoaddEXT *cmd)
{
   const GLenum matrixMode = cmd->matrixMode;
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
   GLenum16 matrixMode;
   GLfloat m[16];
};
void
_mesa_unmarshal_MatrixMultfEXT(struct gl_context *ctx, const struct marshal_cmd_MatrixMultfEXT *cmd)
{
   const GLenum matrixMode = cmd->matrixMode;
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
   GLenum16 matrixMode;
   GLdouble m[16];
};
void
_mesa_unmarshal_MatrixMultdEXT(struct gl_context *ctx, const struct marshal_cmd_MatrixMultdEXT *cmd)
{
   const GLenum matrixMode = cmd->matrixMode;
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
   GLenum16 matrixMode;
};
void
_mesa_unmarshal_MatrixLoadIdentityEXT(struct gl_context *ctx, const struct marshal_cmd_MatrixLoadIdentityEXT *cmd)
{
   const GLenum matrixMode = cmd->matrixMode;
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
   GLenum16 matrixMode;
   GLfloat angle;
   GLfloat x;
   GLfloat y;
   GLfloat z;
};
void
_mesa_unmarshal_MatrixRotatefEXT(struct gl_context *ctx, const struct marshal_cmd_MatrixRotatefEXT *cmd)
{
   const GLenum matrixMode = cmd->matrixMode;
   const GLfloat angle = cmd->angle;
   const GLfloat x = cmd->x;
   const GLfloat y = cmd->y;
   const GLfloat z = cmd->z;
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
   GLenum16 matrixMode;
   GLdouble angle;
   GLdouble x;
   GLdouble y;
   GLdouble z;
};
void
_mesa_unmarshal_MatrixRotatedEXT(struct gl_context *ctx, const struct marshal_cmd_MatrixRotatedEXT *cmd)
{
   const GLenum matrixMode = cmd->matrixMode;
   const GLdouble angle = cmd->angle;
   const GLdouble x = cmd->x;
   const GLdouble y = cmd->y;
   const GLdouble z = cmd->z;
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
   GLenum16 matrixMode;
   GLfloat x;
   GLfloat y;
   GLfloat z;
};
void
_mesa_unmarshal_MatrixScalefEXT(struct gl_context *ctx, const struct marshal_cmd_MatrixScalefEXT *cmd)
{
   const GLenum matrixMode = cmd->matrixMode;
   const GLfloat x = cmd->x;
   const GLfloat y = cmd->y;
   const GLfloat z = cmd->z;
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
   GLenum16 matrixMode;
   GLdouble x;
   GLdouble y;
   GLdouble z;
};
void
_mesa_unmarshal_MatrixScaledEXT(struct gl_context *ctx, const struct marshal_cmd_MatrixScaledEXT *cmd)
{
   const GLenum matrixMode = cmd->matrixMode;
   const GLdouble x = cmd->x;
   const GLdouble y = cmd->y;
   const GLdouble z = cmd->z;
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
   GLenum16 matrixMode;
   GLfloat x;
   GLfloat y;
   GLfloat z;
};
void
_mesa_unmarshal_MatrixTranslatefEXT(struct gl_context *ctx, const struct marshal_cmd_MatrixTranslatefEXT *cmd)
{
   const GLenum matrixMode = cmd->matrixMode;
   const GLfloat x = cmd->x;
   const GLfloat y = cmd->y;
   const GLfloat z = cmd->z;
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
   GLenum16 matrixMode;
   GLdouble x;
   GLdouble y;
   GLdouble z;
};
void
_mesa_unmarshal_MatrixTranslatedEXT(struct gl_context *ctx, const struct marshal_cmd_MatrixTranslatedEXT *cmd)
{
   const GLenum matrixMode = cmd->matrixMode;
   const GLdouble x = cmd->x;
   const GLdouble y = cmd->y;
   const GLdouble z = cmd->z;
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
   GLenum16 matrixMode;
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
   const GLenum matrixMode = cmd->matrixMode;
   const GLdouble l = cmd->l;
   const GLdouble r = cmd->r;
   const GLdouble b = cmd->b;
   const GLdouble t = cmd->t;
   const GLdouble n = cmd->n;
   const GLdouble f = cmd->f;
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
   GLenum16 matrixMode;
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
   const GLenum matrixMode = cmd->matrixMode;
   const GLdouble l = cmd->l;
   const GLdouble r = cmd->r;
   const GLdouble b = cmd->b;
   const GLdouble t = cmd->t;
   const GLdouble n = cmd->n;
   const GLdouble f = cmd->f;
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
   GLenum16 matrixMode;
};
void
_mesa_unmarshal_MatrixPushEXT(struct gl_context *ctx, const struct marshal_cmd_MatrixPushEXT *cmd)
{
   const GLenum matrixMode = cmd->matrixMode;
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
}


/* MatrixPopEXT: marshalled asynchronously */
struct marshal_cmd_MatrixPopEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 matrixMode;
};
void
_mesa_unmarshal_MatrixPopEXT(struct gl_context *ctx, const struct marshal_cmd_MatrixPopEXT *cmd)
{
   const GLenum matrixMode = cmd->matrixMode;
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
   const GLbitfield mask = cmd->mask;
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
   const GLbitfield mask = cmd->mask;
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
   GLenum16 target;
   GLenum16 pname;
   GLuint texture;
   int param;
};
void
_mesa_unmarshal_TextureParameteriEXT(struct gl_context *ctx, const struct marshal_cmd_TextureParameteriEXT *cmd)
{
   const GLuint texture = cmd->texture;
   const GLenum target = cmd->target;
   const GLenum pname = cmd->pname;
   const int param = cmd->param;
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
   GLenum16 target;
   GLenum16 pname;
   GLuint texture;
   /* Next safe_mul(_mesa_tex_param_enum_to_count(pname), 1 * sizeof(GLint)) bytes are GLint params[_mesa_tex_param_enum_to_count(pname)] */
};
void
_mesa_unmarshal_TextureParameterivEXT(struct gl_context *ctx, const struct marshal_cmd_TextureParameterivEXT *cmd)
{
   const GLuint texture = cmd->texture;
   const GLenum target = cmd->target;
   const GLenum pname = cmd->pname;
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
   GLenum16 target;
   GLenum16 pname;
   GLuint texture;
   float param;
};
void
_mesa_unmarshal_TextureParameterfEXT(struct gl_context *ctx, const struct marshal_cmd_TextureParameterfEXT *cmd)
{
   const GLuint texture = cmd->texture;
   const GLenum target = cmd->target;
   const GLenum pname = cmd->pname;
   const float param = cmd->param;
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
   GLenum16 target;
   GLenum16 pname;
   GLuint texture;
   /* Next safe_mul(_mesa_tex_param_enum_to_count(pname), 1 * sizeof(float)) bytes are float params[_mesa_tex_param_enum_to_count(pname)] */
};
void
_mesa_unmarshal_TextureParameterfvEXT(struct gl_context *ctx, const struct marshal_cmd_TextureParameterfvEXT *cmd)
{
   const GLuint texture = cmd->texture;
   const GLenum target = cmd->target;
   const GLenum pname = cmd->pname;
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


/* TextureImage1DEXT: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_TextureImage1DEXT(GLuint texture, GLenum target, GLint level, GLint internalFormat, GLsizei width, GLint border, GLenum format, GLenum type, const GLvoid * pixels)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "TextureImage1DEXT");
   CALL_TextureImage1DEXT(ctx->CurrentServerDispatch, (texture, target, level, internalFormat, width, border, format, type, pixels));
}


/* TextureImage2DEXT: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_TextureImage2DEXT(GLuint texture, GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid * pixels)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "TextureImage2DEXT");
   CALL_TextureImage2DEXT(ctx->CurrentServerDispatch, (texture, target, level, internalFormat, width, height, border, format, type, pixels));
}


/* TextureImage3DEXT: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_TextureImage3DEXT(GLuint texture, GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid * pixels)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "TextureImage3DEXT");
   CALL_TextureImage3DEXT(ctx->CurrentServerDispatch, (texture, target, level, internalFormat, width, height, depth, border, format, type, pixels));
}


/* TextureSubImage1DEXT: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_TextureSubImage1DEXT(GLuint texture, GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const GLvoid * pixels)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "TextureSubImage1DEXT");
   CALL_TextureSubImage1DEXT(ctx->CurrentServerDispatch, (texture, target, level, xoffset, width, format, type, pixels));
}


/* TextureSubImage2DEXT: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_TextureSubImage2DEXT(GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid * pixels)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "TextureSubImage2DEXT");
   CALL_TextureSubImage2DEXT(ctx->CurrentServerDispatch, (texture, target, level, xoffset, yoffset, width, height, format, type, pixels));
}


/* TextureSubImage3DEXT: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_TextureSubImage3DEXT(GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const GLvoid * pixels)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "TextureSubImage3DEXT");
   CALL_TextureSubImage3DEXT(ctx->CurrentServerDispatch, (texture, target, level, xoffset, yoffset, zoffset, width, height, depth, format, type, pixels));
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
void
_mesa_unmarshal_CopyTextureImage1DEXT(struct gl_context *ctx, const struct marshal_cmd_CopyTextureImage1DEXT *cmd)
{
   const GLuint texture = cmd->texture;
   const GLenum target = cmd->target;
   const GLint level = cmd->level;
   const GLenum internalFormat = cmd->internalFormat;
   const GLint x = cmd->x;
   const GLint y = cmd->y;
   const GLsizei width = cmd->width;
   const int border = cmd->border;
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
void
_mesa_unmarshal_CopyTextureImage2DEXT(struct gl_context *ctx, const struct marshal_cmd_CopyTextureImage2DEXT *cmd)
{
   const GLuint texture = cmd->texture;
   const GLenum target = cmd->target;
   const GLint level = cmd->level;
   const GLenum internalFormat = cmd->internalFormat;
   const GLint x = cmd->x;
   const GLint y = cmd->y;
   const GLsizei width = cmd->width;
   const GLsizei height = cmd->height;
   const int border = cmd->border;
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
   GLenum16 target;
   GLuint texture;
   GLint level;
   GLint xoffset;
   GLint x;
   GLint y;
   GLsizei width;
};
void
_mesa_unmarshal_CopyTextureSubImage1DEXT(struct gl_context *ctx, const struct marshal_cmd_CopyTextureSubImage1DEXT *cmd)
{
   const GLuint texture = cmd->texture;
   const GLenum target = cmd->target;
   const GLint level = cmd->level;
   const GLint xoffset = cmd->xoffset;
   const GLint x = cmd->x;
   const GLint y = cmd->y;
   const GLsizei width = cmd->width;
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
void
_mesa_unmarshal_CopyTextureSubImage2DEXT(struct gl_context *ctx, const struct marshal_cmd_CopyTextureSubImage2DEXT *cmd)
{
   const GLuint texture = cmd->texture;
   const GLenum target = cmd->target;
   const GLint level = cmd->level;
   const GLint xoffset = cmd->xoffset;
   const GLint yoffset = cmd->yoffset;
   const GLint x = cmd->x;
   const GLint y = cmd->y;
   const GLsizei width = cmd->width;
   const GLsizei height = cmd->height;
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
void
_mesa_unmarshal_CopyTextureSubImage3DEXT(struct gl_context *ctx, const struct marshal_cmd_CopyTextureSubImage3DEXT *cmd)
{
   const GLuint texture = cmd->texture;
   const GLenum target = cmd->target;
   const GLint level = cmd->level;
   const GLint xoffset = cmd->xoffset;
   const GLint yoffset = cmd->yoffset;
   const GLint zoffset = cmd->zoffset;
   const GLint x = cmd->x;
   const GLint y = cmd->y;
   const GLsizei width = cmd->width;
   const GLsizei height = cmd->height;
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


/* GetTextureImageEXT: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetTextureImageEXT(GLuint texture, GLenum target, GLint level, GLenum format, GLenum type, GLvoid * pixels)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetTextureImageEXT");
   CALL_GetTextureImageEXT(ctx->CurrentServerDispatch, (texture, target, level, format, type, pixels));
}


/* BindMultiTextureEXT: marshalled asynchronously */
struct marshal_cmd_BindMultiTextureEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 texunit;
   GLenum16 target;
   GLuint texture;
};
void
_mesa_unmarshal_BindMultiTextureEXT(struct gl_context *ctx, const struct marshal_cmd_BindMultiTextureEXT *cmd)
{
   const GLenum texunit = cmd->texunit;
   const GLenum target = cmd->target;
   const GLuint texture = cmd->texture;
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
   GLenum16 array;
   GLuint index;
};
void
_mesa_unmarshal_EnableClientStateiEXT(struct gl_context *ctx, const struct marshal_cmd_EnableClientStateiEXT *cmd)
{
   const GLenum array = cmd->array;
   const GLuint index = cmd->index;
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
   GLenum16 array;
   GLuint index;
};
void
_mesa_unmarshal_DisableClientStateiEXT(struct gl_context *ctx, const struct marshal_cmd_DisableClientStateiEXT *cmd)
{
   const GLenum array = cmd->array;
   const GLuint index = cmd->index;
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
   GLenum16 texunit;
   GLenum16 target;
   GLenum16 pname;
   GLint param;
};
void
_mesa_unmarshal_MultiTexEnviEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexEnviEXT *cmd)
{
   const GLenum texunit = cmd->texunit;
   const GLenum target = cmd->target;
   const GLenum pname = cmd->pname;
   const GLint param = cmd->param;
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
   GLenum16 texunit;
   GLenum16 target;
   GLenum16 pname;
   /* Next safe_mul(_mesa_texenv_enum_to_count(pname), 1 * sizeof(GLint)) bytes are GLint param[_mesa_texenv_enum_to_count(pname)] */
};
void
_mesa_unmarshal_MultiTexEnvivEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexEnvivEXT *cmd)
{
   const GLenum texunit = cmd->texunit;
   const GLenum target = cmd->target;
   const GLenum pname = cmd->pname;
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
   GLenum16 texunit;
   GLenum16 target;
   GLenum16 pname;
   GLfloat param;
};
void
_mesa_unmarshal_MultiTexEnvfEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexEnvfEXT *cmd)
{
   const GLenum texunit = cmd->texunit;
   const GLenum target = cmd->target;
   const GLenum pname = cmd->pname;
   const GLfloat param = cmd->param;
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
   GLenum16 texunit;
   GLenum16 target;
   GLenum16 pname;
   /* Next safe_mul(_mesa_texenv_enum_to_count(pname), 1 * sizeof(GLfloat)) bytes are GLfloat param[_mesa_texenv_enum_to_count(pname)] */
};
void
_mesa_unmarshal_MultiTexEnvfvEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexEnvfvEXT *cmd)
{
   const GLenum texunit = cmd->texunit;
   const GLenum target = cmd->target;
   const GLenum pname = cmd->pname;
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
   GLenum16 texunit;
   GLenum16 target;
   GLenum16 pname;
   GLint param;
};
void
_mesa_unmarshal_MultiTexParameteriEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexParameteriEXT *cmd)
{
   const GLenum texunit = cmd->texunit;
   const GLenum target = cmd->target;
   const GLenum pname = cmd->pname;
   const GLint param = cmd->param;
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
   GLenum16 texunit;
   GLenum16 target;
   GLenum16 pname;
   /* Next safe_mul(_mesa_tex_param_enum_to_count(pname), 1 * sizeof(GLint)) bytes are GLint param[_mesa_tex_param_enum_to_count(pname)] */
};
void
_mesa_unmarshal_MultiTexParameterivEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexParameterivEXT *cmd)
{
   const GLenum texunit = cmd->texunit;
   const GLenum target = cmd->target;
   const GLenum pname = cmd->pname;
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
   GLenum16 texunit;
   GLenum16 target;
   GLenum16 pname;
   GLfloat param;
};
void
_mesa_unmarshal_MultiTexParameterfEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexParameterfEXT *cmd)
{
   const GLenum texunit = cmd->texunit;
   const GLenum target = cmd->target;
   const GLenum pname = cmd->pname;
   const GLfloat param = cmd->param;
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
   GLenum16 texunit;
   GLenum16 target;
   GLenum16 pname;
   /* Next safe_mul(_mesa_tex_param_enum_to_count(pname), 1 * sizeof(GLfloat)) bytes are GLfloat param[_mesa_tex_param_enum_to_count(pname)] */
};
void
_mesa_unmarshal_MultiTexParameterfvEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexParameterfvEXT *cmd)
{
   const GLenum texunit = cmd->texunit;
   const GLenum target = cmd->target;
   const GLenum pname = cmd->pname;
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


/* GetMultiTexImageEXT: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetMultiTexImageEXT(GLenum texunit, GLenum target, GLint level, GLenum format, GLenum type, GLvoid* pixels)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetMultiTexImageEXT");
   CALL_GetMultiTexImageEXT(ctx->CurrentServerDispatch, (texunit, target, level, format, type, pixels));
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


/* MultiTexImage1DEXT: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_MultiTexImage1DEXT(GLenum texunit, GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, const GLvoid* pixels)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "MultiTexImage1DEXT");
   CALL_MultiTexImage1DEXT(ctx->CurrentServerDispatch, (texunit, target, level, internalformat, width, border, format, type, pixels));
}


/* MultiTexImage2DEXT: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_MultiTexImage2DEXT(GLenum texunit, GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid* pixels)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "MultiTexImage2DEXT");
   CALL_MultiTexImage2DEXT(ctx->CurrentServerDispatch, (texunit, target, level, internalformat, width, height, border, format, type, pixels));
}


/* MultiTexImage3DEXT: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_MultiTexImage3DEXT(GLenum texunit, GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid* pixels)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "MultiTexImage3DEXT");
   CALL_MultiTexImage3DEXT(ctx->CurrentServerDispatch, (texunit, target, level, internalformat, width, height, depth, border, format, type, pixels));
}


/* MultiTexSubImage1DEXT: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_MultiTexSubImage1DEXT(GLenum texunit, GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const GLvoid* pixels)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "MultiTexSubImage1DEXT");
   CALL_MultiTexSubImage1DEXT(ctx->CurrentServerDispatch, (texunit, target, level, xoffset, width, format, type, pixels));
}


/* MultiTexSubImage2DEXT: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_MultiTexSubImage2DEXT(GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid* pixels)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "MultiTexSubImage2DEXT");
   CALL_MultiTexSubImage2DEXT(ctx->CurrentServerDispatch, (texunit, target, level, xoffset, yoffset, width, height, format, type, pixels));
}


/* MultiTexSubImage3DEXT: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_MultiTexSubImage3DEXT(GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const GLvoid* pixels)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "MultiTexSubImage3DEXT");
   CALL_MultiTexSubImage3DEXT(ctx->CurrentServerDispatch, (texunit, target, level, xoffset, yoffset, zoffset, width, height, depth, format, type, pixels));
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
void
_mesa_unmarshal_CopyMultiTexImage1DEXT(struct gl_context *ctx, const struct marshal_cmd_CopyMultiTexImage1DEXT *cmd)
{
   const GLenum texunit = cmd->texunit;
   const GLenum target = cmd->target;
   const GLint level = cmd->level;
   const GLenum internalformat = cmd->internalformat;
   const GLint x = cmd->x;
   const GLint y = cmd->y;
   const GLsizei width = cmd->width;
   const GLint border = cmd->border;
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
void
_mesa_unmarshal_CopyMultiTexImage2DEXT(struct gl_context *ctx, const struct marshal_cmd_CopyMultiTexImage2DEXT *cmd)
{
   const GLenum texunit = cmd->texunit;
   const GLenum target = cmd->target;
   const GLint level = cmd->level;
   const GLenum internalformat = cmd->internalformat;
   const GLint x = cmd->x;
   const GLint y = cmd->y;
   const GLsizei width = cmd->width;
   const GLsizei height = cmd->height;
   const GLint border = cmd->border;
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
   GLenum16 texunit;
   GLenum16 target;
   GLint level;
   GLint xoffset;
   GLint x;
   GLint y;
   GLsizei width;
};
void
_mesa_unmarshal_CopyMultiTexSubImage1DEXT(struct gl_context *ctx, const struct marshal_cmd_CopyMultiTexSubImage1DEXT *cmd)
{
   const GLenum texunit = cmd->texunit;
   const GLenum target = cmd->target;
   const GLint level = cmd->level;
   const GLint xoffset = cmd->xoffset;
   const GLint x = cmd->x;
   const GLint y = cmd->y;
   const GLsizei width = cmd->width;
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
void
_mesa_unmarshal_CopyMultiTexSubImage2DEXT(struct gl_context *ctx, const struct marshal_cmd_CopyMultiTexSubImage2DEXT *cmd)
{
   const GLenum texunit = cmd->texunit;
   const GLenum target = cmd->target;
   const GLint level = cmd->level;
   const GLint xoffset = cmd->xoffset;
   const GLint yoffset = cmd->yoffset;
   const GLint x = cmd->x;
   const GLint y = cmd->y;
   const GLsizei width = cmd->width;
   const GLsizei height = cmd->height;
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
void
_mesa_unmarshal_CopyMultiTexSubImage3DEXT(struct gl_context *ctx, const struct marshal_cmd_CopyMultiTexSubImage3DEXT *cmd)
{
   const GLenum texunit = cmd->texunit;
   const GLenum target = cmd->target;
   const GLint level = cmd->level;
   const GLint xoffset = cmd->xoffset;
   const GLint yoffset = cmd->yoffset;
   const GLint zoffset = cmd->zoffset;
   const GLint x = cmd->x;
   const GLint y = cmd->y;
   const GLsizei width = cmd->width;
   const GLsizei height = cmd->height;
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
   GLenum16 texunit;
   GLenum16 coord;
   GLenum16 pname;
   GLdouble param;
};
void
_mesa_unmarshal_MultiTexGendEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexGendEXT *cmd)
{
   const GLenum texunit = cmd->texunit;
   const GLenum coord = cmd->coord;
   const GLenum pname = cmd->pname;
   const GLdouble param = cmd->param;
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
   GLenum16 texunit;
   GLenum16 coord;
   GLenum16 pname;
   /* Next safe_mul(_mesa_texgen_enum_to_count(pname), 1 * sizeof(GLdouble)) bytes are GLdouble param[_mesa_texgen_enum_to_count(pname)] */
};
void
_mesa_unmarshal_MultiTexGendvEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexGendvEXT *cmd)
{
   const GLenum texunit = cmd->texunit;
   const GLenum coord = cmd->coord;
   const GLenum pname = cmd->pname;
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
   GLenum16 texunit;
   GLenum16 coord;
   GLenum16 pname;
   GLfloat param;
};
void
_mesa_unmarshal_MultiTexGenfEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexGenfEXT *cmd)
{
   const GLenum texunit = cmd->texunit;
   const GLenum coord = cmd->coord;
   const GLenum pname = cmd->pname;
   const GLfloat param = cmd->param;
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
   GLenum16 texunit;
   GLenum16 coord;
   GLenum16 pname;
   /* Next safe_mul(_mesa_texgen_enum_to_count(pname), 1 * sizeof(GLfloat)) bytes are GLfloat param[_mesa_texgen_enum_to_count(pname)] */
};
void
_mesa_unmarshal_MultiTexGenfvEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexGenfvEXT *cmd)
{
   const GLenum texunit = cmd->texunit;
   const GLenum coord = cmd->coord;
   const GLenum pname = cmd->pname;
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
   GLenum16 texunit;
   GLenum16 coord;
   GLenum16 pname;
   GLint param;
};
void
_mesa_unmarshal_MultiTexGeniEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexGeniEXT *cmd)
{
   const GLenum texunit = cmd->texunit;
   const GLenum coord = cmd->coord;
   const GLenum pname = cmd->pname;
   const GLint param = cmd->param;
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
   GLenum16 texunit;
   GLenum16 coord;
   GLenum16 pname;
   /* Next safe_mul(_mesa_texgen_enum_to_count(pname), 1 * sizeof(GLint)) bytes are GLint param[_mesa_texgen_enum_to_count(pname)] */
};
void
_mesa_unmarshal_MultiTexGenivEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexGenivEXT *cmd)
{
   const GLenum texunit = cmd->texunit;
   const GLenum coord = cmd->coord;
   const GLenum pname = cmd->pname;
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
   GLenum16 texunit;
   GLenum16 type;
   GLint size;
   GLsizei stride;
   const GLvoid * pointer;
};
void
_mesa_unmarshal_MultiTexCoordPointerEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoordPointerEXT *cmd)
{
   const GLenum texunit = cmd->texunit;
   const GLint size = cmd->size;
   const GLenum type = cmd->type;
   const GLsizei stride = cmd->stride;
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
   if (COMPAT) _mesa_glthread_AttribPointer(ctx, VERT_ATTRIB_TEX(texunit - GL_TEXTURE0));
}


/* MatrixLoadTransposefEXT: marshalled asynchronously */
struct marshal_cmd_MatrixLoadTransposefEXT
{
   struct marshal_cmd_base cmd_base;
   GLenum16 matrixMode;
   GLfloat m[16];
};
void
_mesa_unmarshal_MatrixLoadTransposefEXT(struct gl_context *ctx, const struct marshal_cmd_MatrixLoadTransposefEXT *cmd)
{
   const GLenum matrixMode = cmd->matrixMode;
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
   GLenum16 matrixMode;
   GLdouble m[16];
};
void
_mesa_unmarshal_MatrixLoadTransposedEXT(struct gl_context *ctx, const struct marshal_cmd_MatrixLoadTransposedEXT *cmd)
{
   const GLenum matrixMode = cmd->matrixMode;
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
   GLenum16 matrixMode;
   GLfloat m[16];
};
void
_mesa_unmarshal_MatrixMultTransposefEXT(struct gl_context *ctx, const struct marshal_cmd_MatrixMultTransposefEXT *cmd)
{
   const GLenum matrixMode = cmd->matrixMode;
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
   GLenum16 matrixMode;
   GLdouble m[16];
};
void
_mesa_unmarshal_MatrixMultTransposedEXT(struct gl_context *ctx, const struct marshal_cmd_MatrixMultTransposedEXT *cmd)
{
   const GLenum matrixMode = cmd->matrixMode;
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


/* CompressedTextureImage1DEXT: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_CompressedTextureImage1DEXT(GLuint texture, GLenum target, GLint level, GLenum internalFormat, GLsizei width, GLsizei border, GLsizei imageSize, const GLvoid * data)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "CompressedTextureImage1DEXT");
   CALL_CompressedTextureImage1DEXT(ctx->CurrentServerDispatch, (texture, target, level, internalFormat, width, border, imageSize, data));
}


/* CompressedTextureImage2DEXT: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_CompressedTextureImage2DEXT(GLuint texture, GLenum target, GLint level, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei border, GLsizei imageSize, const GLvoid * data)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "CompressedTextureImage2DEXT");
   CALL_CompressedTextureImage2DEXT(ctx->CurrentServerDispatch, (texture, target, level, internalFormat, width, height, border, imageSize, data));
}


/* CompressedTextureImage3DEXT: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_CompressedTextureImage3DEXT(GLuint texture, GLenum target, GLint level, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLsizei border, GLsizei imageSize, const GLvoid * data)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "CompressedTextureImage3DEXT");
   CALL_CompressedTextureImage3DEXT(ctx->CurrentServerDispatch, (texture, target, level, internalFormat, width, height, depth, border, imageSize, data));
}


/* CompressedTextureSubImage1DEXT: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_CompressedTextureSubImage1DEXT(GLuint texture, GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const GLvoid * data)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "CompressedTextureSubImage1DEXT");
   CALL_CompressedTextureSubImage1DEXT(ctx->CurrentServerDispatch, (texture, target, level, xoffset, width, format, imageSize, data));
}


/* CompressedTextureSubImage2DEXT: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_CompressedTextureSubImage2DEXT(GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const GLvoid * data)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "CompressedTextureSubImage2DEXT");
   CALL_CompressedTextureSubImage2DEXT(ctx->CurrentServerDispatch, (texture, target, level, xoffset, yoffset, width, height, format, imageSize, data));
}


/* CompressedTextureSubImage3DEXT: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_CompressedTextureSubImage3DEXT(GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const GLvoid * data)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "CompressedTextureSubImage3DEXT");
   CALL_CompressedTextureSubImage3DEXT(ctx->CurrentServerDispatch, (texture, target, level, xoffset, yoffset, zoffset, width, height, depth, format, imageSize, data));
}


/* GetCompressedTextureImageEXT: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetCompressedTextureImageEXT(GLuint texture, GLenum target, GLint level, GLvoid * img)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetCompressedTextureImageEXT");
   CALL_GetCompressedTextureImageEXT(ctx->CurrentServerDispatch, (texture, target, level, img));
}


/* CompressedMultiTexImage1DEXT: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_CompressedMultiTexImage1DEXT(GLenum texunit, GLenum target, GLint level, GLenum internalFormat, GLsizei width, GLsizei border, GLsizei imageSize, const GLvoid * data)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "CompressedMultiTexImage1DEXT");
   CALL_CompressedMultiTexImage1DEXT(ctx->CurrentServerDispatch, (texunit, target, level, internalFormat, width, border, imageSize, data));
}


/* CompressedMultiTexImage2DEXT: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_CompressedMultiTexImage2DEXT(GLenum texunit, GLenum target, GLint level, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei border, GLsizei imageSize, const GLvoid * data)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "CompressedMultiTexImage2DEXT");
   CALL_CompressedMultiTexImage2DEXT(ctx->CurrentServerDispatch, (texunit, target, level, internalFormat, width, height, border, imageSize, data));
}


/* CompressedMultiTexImage3DEXT: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_CompressedMultiTexImage3DEXT(GLenum texunit, GLenum target, GLint level, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLsizei border, GLsizei imageSize, const GLvoid * data)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "CompressedMultiTexImage3DEXT");
   CALL_CompressedMultiTexImage3DEXT(ctx->CurrentServerDispatch, (texunit, target, level, internalFormat, width, height, depth, border, imageSize, data));
}


/* CompressedMultiTexSubImage1DEXT: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_CompressedMultiTexSubImage1DEXT(GLenum texunit, GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const GLvoid * data)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "CompressedMultiTexSubImage1DEXT");
   CALL_CompressedMultiTexSubImage1DEXT(ctx->CurrentServerDispatch, (texunit, target, level, xoffset, width, format, imageSize, data));
}


/* CompressedMultiTexSubImage2DEXT: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_CompressedMultiTexSubImage2DEXT(GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const GLvoid * data)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "CompressedMultiTexSubImage2DEXT");
   CALL_CompressedMultiTexSubImage2DEXT(ctx->CurrentServerDispatch, (texunit, target, level, xoffset, yoffset, width, height, format, imageSize, data));
}


/* CompressedMultiTexSubImage3DEXT: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_CompressedMultiTexSubImage3DEXT(GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const GLvoid * data)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "CompressedMultiTexSubImage3DEXT");
   CALL_CompressedMultiTexSubImage3DEXT(ctx->CurrentServerDispatch, (texunit, target, level, xoffset, yoffset, zoffset, width, height, depth, format, imageSize, data));
}


/* GetCompressedMultiTexImageEXT: marshalled synchronously */
void GLAPIENTRY
_mesa_marshal_GetCompressedMultiTexImageEXT(GLenum texunit, GLenum target, GLint level, GLvoid * img)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_glthread_finish_before(ctx, "GetCompressedMultiTexImageEXT");
   CALL_GetCompressedMultiTexImageEXT(ctx->CurrentServerDispatch, (texunit, target, level, img));
}


