/* DO NOT EDIT - This file generated automatically by glX_proto_send.py (from Mesa) script */

/*
 * Copyright 1998-1999 Precision Insight, Inc., Cedar Park, Texas.
 * (C) Copyright IBM Corporation 2004
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
 * PRECISION INSIGHT, IBM,
 * AND/OR THEIR SUPPLIERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
 * OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/**
 * \file indirect_init.c
 * Initialize indirect rendering dispatch table.
 *
 * \author Kevin E. Martin <kevin@precisioninsight.com>
 * \author Brian Paul <brian@precisioninsight.com>
 * \author Ian Romanick <idr@us.ibm.com>
 */

#include "indirect_init.h"
#include "indirect.h"
#include "glapi.h"
#include <assert.h>

#if !defined(GLX_USE_APPLEGL) || defined(GLX_USE_APPLE)

/**
 * No-op function used to initialize functions that have no GLX protocol
 * support.
 */
static int NoOp(void)
{
    return 0;
}

/**
 * Create and initialize a new GL dispatch table.  The table is initialized
 * with GLX indirect rendering protocol functions.
 */
struct _glapi_table * __glXNewIndirectAPI( void )
{
    _glapi_proc *table;
    unsigned entries;
    unsigned i;

    entries = _mesa_glapi_get_dispatch_table_size();
    table = malloc(entries * sizeof(_glapi_proc));
    if (table == NULL)
        return NULL;

    /* first, set all entries to point to no-op functions */
    for (i = 0; i < entries; i++) {
       table[i] = (_glapi_proc) NoOp;
    }

    /* now, initialize the entries we understand */

    /* 1.0 */

    table[213] = (_glapi_proc) __indirect_glAccum;
    table[240] = (_glapi_proc) __indirect_glAlphaFunc;
    table[7] = (_glapi_proc) __indirect_glBegin;
    table[8] = (_glapi_proc) __indirect_glBitmap;
    table[241] = (_glapi_proc) __indirect_glBlendFunc;
    table[2] = (_glapi_proc) __indirect_glCallList;
    table[3] = (_glapi_proc) __indirect_glCallLists;
    table[203] = (_glapi_proc) __indirect_glClear;
    table[204] = (_glapi_proc) __indirect_glClearAccum;
    table[206] = (_glapi_proc) __indirect_glClearColor;
    table[208] = (_glapi_proc) __indirect_glClearDepth;
    table[205] = (_glapi_proc) __indirect_glClearIndex;
    table[207] = (_glapi_proc) __indirect_glClearStencil;
    table[150] = (_glapi_proc) __indirect_glClipPlane;
    table[9] = (_glapi_proc) __indirect_glColor3b;
    table[10] = (_glapi_proc) __indirect_glColor3bv;
    table[11] = (_glapi_proc) __indirect_glColor3d;
    table[12] = (_glapi_proc) __indirect_glColor3dv;
    table[13] = (_glapi_proc) __indirect_glColor3f;
    table[14] = (_glapi_proc) __indirect_glColor3fv;
    table[15] = (_glapi_proc) __indirect_glColor3i;
    table[16] = (_glapi_proc) __indirect_glColor3iv;
    table[17] = (_glapi_proc) __indirect_glColor3s;
    table[18] = (_glapi_proc) __indirect_glColor3sv;
    table[19] = (_glapi_proc) __indirect_glColor3ub;
    table[20] = (_glapi_proc) __indirect_glColor3ubv;
    table[21] = (_glapi_proc) __indirect_glColor3ui;
    table[22] = (_glapi_proc) __indirect_glColor3uiv;
    table[23] = (_glapi_proc) __indirect_glColor3us;
    table[24] = (_glapi_proc) __indirect_glColor3usv;
    table[25] = (_glapi_proc) __indirect_glColor4b;
    table[26] = (_glapi_proc) __indirect_glColor4bv;
    table[27] = (_glapi_proc) __indirect_glColor4d;
    table[28] = (_glapi_proc) __indirect_glColor4dv;
    table[29] = (_glapi_proc) __indirect_glColor4f;
    table[30] = (_glapi_proc) __indirect_glColor4fv;
    table[31] = (_glapi_proc) __indirect_glColor4i;
    table[32] = (_glapi_proc) __indirect_glColor4iv;
    table[33] = (_glapi_proc) __indirect_glColor4s;
    table[34] = (_glapi_proc) __indirect_glColor4sv;
    table[35] = (_glapi_proc) __indirect_glColor4ub;
    table[36] = (_glapi_proc) __indirect_glColor4ubv;
    table[37] = (_glapi_proc) __indirect_glColor4ui;
    table[38] = (_glapi_proc) __indirect_glColor4uiv;
    table[39] = (_glapi_proc) __indirect_glColor4us;
    table[40] = (_glapi_proc) __indirect_glColor4usv;
    table[210] = (_glapi_proc) __indirect_glColorMask;
    table[151] = (_glapi_proc) __indirect_glColorMaterial;
    table[255] = (_glapi_proc) __indirect_glCopyPixels;
    table[152] = (_glapi_proc) __indirect_glCullFace;
    table[4] = (_glapi_proc) __indirect_glDeleteLists;
    table[245] = (_glapi_proc) __indirect_glDepthFunc;
    table[211] = (_glapi_proc) __indirect_glDepthMask;
    table[288] = (_glapi_proc) __indirect_glDepthRange;
    table[214] = (_glapi_proc) __indirect_glDisable;
    table[202] = (_glapi_proc) __indirect_glDrawBuffer;
    table[257] = (_glapi_proc) __indirect_glDrawPixels;
    table[41] = (_glapi_proc) __indirect_glEdgeFlag;
    table[42] = (_glapi_proc) __indirect_glEdgeFlagv;
    table[215] = (_glapi_proc) __indirect_glEnable;
    table[43] = (_glapi_proc) __indirect_glEnd;
    table[1] = (_glapi_proc) __indirect_glEndList;
    table[228] = (_glapi_proc) __indirect_glEvalCoord1d;
    table[229] = (_glapi_proc) __indirect_glEvalCoord1dv;
    table[230] = (_glapi_proc) __indirect_glEvalCoord1f;
    table[231] = (_glapi_proc) __indirect_glEvalCoord1fv;
    table[232] = (_glapi_proc) __indirect_glEvalCoord2d;
    table[233] = (_glapi_proc) __indirect_glEvalCoord2dv;
    table[234] = (_glapi_proc) __indirect_glEvalCoord2f;
    table[235] = (_glapi_proc) __indirect_glEvalCoord2fv;
    table[236] = (_glapi_proc) __indirect_glEvalMesh1;
    table[238] = (_glapi_proc) __indirect_glEvalMesh2;
    table[237] = (_glapi_proc) __indirect_glEvalPoint1;
    table[239] = (_glapi_proc) __indirect_glEvalPoint2;
    table[194] = (_glapi_proc) __indirect_glFeedbackBuffer;
    table[216] = (_glapi_proc) __indirect_glFinish;
    table[217] = (_glapi_proc) __indirect_glFlush;
    table[153] = (_glapi_proc) __indirect_glFogf;
    table[154] = (_glapi_proc) __indirect_glFogfv;
    table[155] = (_glapi_proc) __indirect_glFogi;
    table[156] = (_glapi_proc) __indirect_glFogiv;
    table[157] = (_glapi_proc) __indirect_glFrontFace;
    table[289] = (_glapi_proc) __indirect_glFrustum;
    table[5] = (_glapi_proc) __indirect_glGenLists;
    table[258] = (_glapi_proc) __indirect_glGetBooleanv;
    table[259] = (_glapi_proc) __indirect_glGetClipPlane;
    table[260] = (_glapi_proc) __indirect_glGetDoublev;
    table[261] = (_glapi_proc) __indirect_glGetError;
    table[262] = (_glapi_proc) __indirect_glGetFloatv;
    table[263] = (_glapi_proc) __indirect_glGetIntegerv;
    table[264] = (_glapi_proc) __indirect_glGetLightfv;
    table[265] = (_glapi_proc) __indirect_glGetLightiv;
    table[266] = (_glapi_proc) __indirect_glGetMapdv;
    table[267] = (_glapi_proc) __indirect_glGetMapfv;
    table[268] = (_glapi_proc) __indirect_glGetMapiv;
    table[269] = (_glapi_proc) __indirect_glGetMaterialfv;
    table[270] = (_glapi_proc) __indirect_glGetMaterialiv;
    table[271] = (_glapi_proc) __indirect_glGetPixelMapfv;
    table[272] = (_glapi_proc) __indirect_glGetPixelMapuiv;
    table[273] = (_glapi_proc) __indirect_glGetPixelMapusv;
    table[274] = (_glapi_proc) __indirect_glGetPolygonStipple;
    table[275] = (_glapi_proc) __indirect_glGetString;
    table[276] = (_glapi_proc) __indirect_glGetTexEnvfv;
    table[277] = (_glapi_proc) __indirect_glGetTexEnviv;
    table[278] = (_glapi_proc) __indirect_glGetTexGendv;
    table[279] = (_glapi_proc) __indirect_glGetTexGenfv;
    table[280] = (_glapi_proc) __indirect_glGetTexGeniv;
    table[281] = (_glapi_proc) __indirect_glGetTexImage;
    table[284] = (_glapi_proc) __indirect_glGetTexLevelParameterfv;
    table[285] = (_glapi_proc) __indirect_glGetTexLevelParameteriv;
    table[282] = (_glapi_proc) __indirect_glGetTexParameterfv;
    table[283] = (_glapi_proc) __indirect_glGetTexParameteriv;
    table[158] = (_glapi_proc) __indirect_glHint;
    table[212] = (_glapi_proc) __indirect_glIndexMask;
    table[44] = (_glapi_proc) __indirect_glIndexd;
    table[45] = (_glapi_proc) __indirect_glIndexdv;
    table[46] = (_glapi_proc) __indirect_glIndexf;
    table[47] = (_glapi_proc) __indirect_glIndexfv;
    table[48] = (_glapi_proc) __indirect_glIndexi;
    table[49] = (_glapi_proc) __indirect_glIndexiv;
    table[50] = (_glapi_proc) __indirect_glIndexs;
    table[51] = (_glapi_proc) __indirect_glIndexsv;
    table[197] = (_glapi_proc) __indirect_glInitNames;
    table[286] = (_glapi_proc) __indirect_glIsEnabled;
    table[287] = (_glapi_proc) __indirect_glIsList;
    table[163] = (_glapi_proc) __indirect_glLightModelf;
    table[164] = (_glapi_proc) __indirect_glLightModelfv;
    table[165] = (_glapi_proc) __indirect_glLightModeli;
    table[166] = (_glapi_proc) __indirect_glLightModeliv;
    table[159] = (_glapi_proc) __indirect_glLightf;
    table[160] = (_glapi_proc) __indirect_glLightfv;
    table[161] = (_glapi_proc) __indirect_glLighti;
    table[162] = (_glapi_proc) __indirect_glLightiv;
    table[167] = (_glapi_proc) __indirect_glLineStipple;
    table[168] = (_glapi_proc) __indirect_glLineWidth;
    table[6] = (_glapi_proc) __indirect_glListBase;
    table[290] = (_glapi_proc) __indirect_glLoadIdentity;
    table[292] = (_glapi_proc) __indirect_glLoadMatrixd;
    table[291] = (_glapi_proc) __indirect_glLoadMatrixf;
    table[198] = (_glapi_proc) __indirect_glLoadName;
    table[242] = (_glapi_proc) __indirect_glLogicOp;
    table[220] = (_glapi_proc) __indirect_glMap1d;
    table[221] = (_glapi_proc) __indirect_glMap1f;
    table[222] = (_glapi_proc) __indirect_glMap2d;
    table[223] = (_glapi_proc) __indirect_glMap2f;
    table[224] = (_glapi_proc) __indirect_glMapGrid1d;
    table[225] = (_glapi_proc) __indirect_glMapGrid1f;
    table[226] = (_glapi_proc) __indirect_glMapGrid2d;
    table[227] = (_glapi_proc) __indirect_glMapGrid2f;
    table[169] = (_glapi_proc) __indirect_glMaterialf;
    table[170] = (_glapi_proc) __indirect_glMaterialfv;
    table[171] = (_glapi_proc) __indirect_glMateriali;
    table[172] = (_glapi_proc) __indirect_glMaterialiv;
    table[293] = (_glapi_proc) __indirect_glMatrixMode;
    table[295] = (_glapi_proc) __indirect_glMultMatrixd;
    table[294] = (_glapi_proc) __indirect_glMultMatrixf;
    table[0] = (_glapi_proc) __indirect_glNewList;
    table[52] = (_glapi_proc) __indirect_glNormal3b;
    table[53] = (_glapi_proc) __indirect_glNormal3bv;
    table[54] = (_glapi_proc) __indirect_glNormal3d;
    table[55] = (_glapi_proc) __indirect_glNormal3dv;
    table[56] = (_glapi_proc) __indirect_glNormal3f;
    table[57] = (_glapi_proc) __indirect_glNormal3fv;
    table[58] = (_glapi_proc) __indirect_glNormal3i;
    table[59] = (_glapi_proc) __indirect_glNormal3iv;
    table[60] = (_glapi_proc) __indirect_glNormal3s;
    table[61] = (_glapi_proc) __indirect_glNormal3sv;
    table[296] = (_glapi_proc) __indirect_glOrtho;
    table[199] = (_glapi_proc) __indirect_glPassThrough;
    table[251] = (_glapi_proc) __indirect_glPixelMapfv;
    table[252] = (_glapi_proc) __indirect_glPixelMapuiv;
    table[253] = (_glapi_proc) __indirect_glPixelMapusv;
    table[249] = (_glapi_proc) __indirect_glPixelStoref;
    table[250] = (_glapi_proc) __indirect_glPixelStorei;
    table[247] = (_glapi_proc) __indirect_glPixelTransferf;
    table[248] = (_glapi_proc) __indirect_glPixelTransferi;
    table[246] = (_glapi_proc) __indirect_glPixelZoom;
    table[173] = (_glapi_proc) __indirect_glPointSize;
    table[174] = (_glapi_proc) __indirect_glPolygonMode;
    table[175] = (_glapi_proc) __indirect_glPolygonStipple;
    table[218] = (_glapi_proc) __indirect_glPopAttrib;
    table[297] = (_glapi_proc) __indirect_glPopMatrix;
    table[200] = (_glapi_proc) __indirect_glPopName;
    table[219] = (_glapi_proc) __indirect_glPushAttrib;
    table[298] = (_glapi_proc) __indirect_glPushMatrix;
    table[201] = (_glapi_proc) __indirect_glPushName;
    table[62] = (_glapi_proc) __indirect_glRasterPos2d;
    table[63] = (_glapi_proc) __indirect_glRasterPos2dv;
    table[64] = (_glapi_proc) __indirect_glRasterPos2f;
    table[65] = (_glapi_proc) __indirect_glRasterPos2fv;
    table[66] = (_glapi_proc) __indirect_glRasterPos2i;
    table[67] = (_glapi_proc) __indirect_glRasterPos2iv;
    table[68] = (_glapi_proc) __indirect_glRasterPos2s;
    table[69] = (_glapi_proc) __indirect_glRasterPos2sv;
    table[70] = (_glapi_proc) __indirect_glRasterPos3d;
    table[71] = (_glapi_proc) __indirect_glRasterPos3dv;
    table[72] = (_glapi_proc) __indirect_glRasterPos3f;
    table[73] = (_glapi_proc) __indirect_glRasterPos3fv;
    table[74] = (_glapi_proc) __indirect_glRasterPos3i;
    table[75] = (_glapi_proc) __indirect_glRasterPos3iv;
    table[76] = (_glapi_proc) __indirect_glRasterPos3s;
    table[77] = (_glapi_proc) __indirect_glRasterPos3sv;
    table[78] = (_glapi_proc) __indirect_glRasterPos4d;
    table[79] = (_glapi_proc) __indirect_glRasterPos4dv;
    table[80] = (_glapi_proc) __indirect_glRasterPos4f;
    table[81] = (_glapi_proc) __indirect_glRasterPos4fv;
    table[82] = (_glapi_proc) __indirect_glRasterPos4i;
    table[83] = (_glapi_proc) __indirect_glRasterPos4iv;
    table[84] = (_glapi_proc) __indirect_glRasterPos4s;
    table[85] = (_glapi_proc) __indirect_glRasterPos4sv;
    table[254] = (_glapi_proc) __indirect_glReadBuffer;
    table[256] = (_glapi_proc) __indirect_glReadPixels;
    table[86] = (_glapi_proc) __indirect_glRectd;
    table[87] = (_glapi_proc) __indirect_glRectdv;
    table[88] = (_glapi_proc) __indirect_glRectf;
    table[89] = (_glapi_proc) __indirect_glRectfv;
    table[90] = (_glapi_proc) __indirect_glRecti;
    table[91] = (_glapi_proc) __indirect_glRectiv;
    table[92] = (_glapi_proc) __indirect_glRects;
    table[93] = (_glapi_proc) __indirect_glRectsv;
    table[196] = (_glapi_proc) __indirect_glRenderMode;
    table[299] = (_glapi_proc) __indirect_glRotated;
    table[300] = (_glapi_proc) __indirect_glRotatef;
    table[301] = (_glapi_proc) __indirect_glScaled;
    table[302] = (_glapi_proc) __indirect_glScalef;
    table[176] = (_glapi_proc) __indirect_glScissor;
    table[195] = (_glapi_proc) __indirect_glSelectBuffer;
    table[177] = (_glapi_proc) __indirect_glShadeModel;
    table[243] = (_glapi_proc) __indirect_glStencilFunc;
    table[209] = (_glapi_proc) __indirect_glStencilMask;
    table[244] = (_glapi_proc) __indirect_glStencilOp;
    table[94] = (_glapi_proc) __indirect_glTexCoord1d;
    table[95] = (_glapi_proc) __indirect_glTexCoord1dv;
    table[96] = (_glapi_proc) __indirect_glTexCoord1f;
    table[97] = (_glapi_proc) __indirect_glTexCoord1fv;
    table[98] = (_glapi_proc) __indirect_glTexCoord1i;
    table[99] = (_glapi_proc) __indirect_glTexCoord1iv;
    table[100] = (_glapi_proc) __indirect_glTexCoord1s;
    table[101] = (_glapi_proc) __indirect_glTexCoord1sv;
    table[102] = (_glapi_proc) __indirect_glTexCoord2d;
    table[103] = (_glapi_proc) __indirect_glTexCoord2dv;
    table[104] = (_glapi_proc) __indirect_glTexCoord2f;
    table[105] = (_glapi_proc) __indirect_glTexCoord2fv;
    table[106] = (_glapi_proc) __indirect_glTexCoord2i;
    table[107] = (_glapi_proc) __indirect_glTexCoord2iv;
    table[108] = (_glapi_proc) __indirect_glTexCoord2s;
    table[109] = (_glapi_proc) __indirect_glTexCoord2sv;
    table[110] = (_glapi_proc) __indirect_glTexCoord3d;
    table[111] = (_glapi_proc) __indirect_glTexCoord3dv;
    table[112] = (_glapi_proc) __indirect_glTexCoord3f;
    table[113] = (_glapi_proc) __indirect_glTexCoord3fv;
    table[114] = (_glapi_proc) __indirect_glTexCoord3i;
    table[115] = (_glapi_proc) __indirect_glTexCoord3iv;
    table[116] = (_glapi_proc) __indirect_glTexCoord3s;
    table[117] = (_glapi_proc) __indirect_glTexCoord3sv;
    table[118] = (_glapi_proc) __indirect_glTexCoord4d;
    table[119] = (_glapi_proc) __indirect_glTexCoord4dv;
    table[120] = (_glapi_proc) __indirect_glTexCoord4f;
    table[121] = (_glapi_proc) __indirect_glTexCoord4fv;
    table[122] = (_glapi_proc) __indirect_glTexCoord4i;
    table[123] = (_glapi_proc) __indirect_glTexCoord4iv;
    table[124] = (_glapi_proc) __indirect_glTexCoord4s;
    table[125] = (_glapi_proc) __indirect_glTexCoord4sv;
    table[184] = (_glapi_proc) __indirect_glTexEnvf;
    table[185] = (_glapi_proc) __indirect_glTexEnvfv;
    table[186] = (_glapi_proc) __indirect_glTexEnvi;
    table[187] = (_glapi_proc) __indirect_glTexEnviv;
    table[188] = (_glapi_proc) __indirect_glTexGend;
    table[189] = (_glapi_proc) __indirect_glTexGendv;
    table[190] = (_glapi_proc) __indirect_glTexGenf;
    table[191] = (_glapi_proc) __indirect_glTexGenfv;
    table[192] = (_glapi_proc) __indirect_glTexGeni;
    table[193] = (_glapi_proc) __indirect_glTexGeniv;
    table[182] = (_glapi_proc) __indirect_glTexImage1D;
    table[183] = (_glapi_proc) __indirect_glTexImage2D;
    table[178] = (_glapi_proc) __indirect_glTexParameterf;
    table[179] = (_glapi_proc) __indirect_glTexParameterfv;
    table[180] = (_glapi_proc) __indirect_glTexParameteri;
    table[181] = (_glapi_proc) __indirect_glTexParameteriv;
    table[303] = (_glapi_proc) __indirect_glTranslated;
    table[304] = (_glapi_proc) __indirect_glTranslatef;
    table[126] = (_glapi_proc) __indirect_glVertex2d;
    table[127] = (_glapi_proc) __indirect_glVertex2dv;
    table[128] = (_glapi_proc) __indirect_glVertex2f;
    table[129] = (_glapi_proc) __indirect_glVertex2fv;
    table[130] = (_glapi_proc) __indirect_glVertex2i;
    table[131] = (_glapi_proc) __indirect_glVertex2iv;
    table[132] = (_glapi_proc) __indirect_glVertex2s;
    table[133] = (_glapi_proc) __indirect_glVertex2sv;
    table[134] = (_glapi_proc) __indirect_glVertex3d;
    table[135] = (_glapi_proc) __indirect_glVertex3dv;
    table[136] = (_glapi_proc) __indirect_glVertex3f;
    table[137] = (_glapi_proc) __indirect_glVertex3fv;
    table[138] = (_glapi_proc) __indirect_glVertex3i;
    table[139] = (_glapi_proc) __indirect_glVertex3iv;
    table[140] = (_glapi_proc) __indirect_glVertex3s;
    table[141] = (_glapi_proc) __indirect_glVertex3sv;
    table[142] = (_glapi_proc) __indirect_glVertex4d;
    table[143] = (_glapi_proc) __indirect_glVertex4dv;
    table[144] = (_glapi_proc) __indirect_glVertex4f;
    table[145] = (_glapi_proc) __indirect_glVertex4fv;
    table[146] = (_glapi_proc) __indirect_glVertex4i;
    table[147] = (_glapi_proc) __indirect_glVertex4iv;
    table[148] = (_glapi_proc) __indirect_glVertex4s;
    table[149] = (_glapi_proc) __indirect_glVertex4sv;
    table[305] = (_glapi_proc) __indirect_glViewport;

    /* 1.1 */

    table[322] = (_glapi_proc) __indirect_glAreTexturesResident;
    table[306] = (_glapi_proc) __indirect_glArrayElement;
    table[307] = (_glapi_proc) __indirect_glBindTexture;
    table[308] = (_glapi_proc) __indirect_glColorPointer;
    table[323] = (_glapi_proc) __indirect_glCopyTexImage1D;
    table[324] = (_glapi_proc) __indirect_glCopyTexImage2D;
    table[325] = (_glapi_proc) __indirect_glCopyTexSubImage1D;
    table[326] = (_glapi_proc) __indirect_glCopyTexSubImage2D;
    table[327] = (_glapi_proc) __indirect_glDeleteTextures;
    table[309] = (_glapi_proc) __indirect_glDisableClientState;
    table[310] = (_glapi_proc) __indirect_glDrawArrays;
    table[311] = (_glapi_proc) __indirect_glDrawElements;
    table[312] = (_glapi_proc) __indirect_glEdgeFlagPointer;
    table[313] = (_glapi_proc) __indirect_glEnableClientState;
    table[328] = (_glapi_proc) __indirect_glGenTextures;
    table[329] = (_glapi_proc) __indirect_glGetPointerv;
    table[314] = (_glapi_proc) __indirect_glIndexPointer;
    table[315] = (_glapi_proc) __indirect_glIndexub;
    table[316] = (_glapi_proc) __indirect_glIndexubv;
    table[317] = (_glapi_proc) __indirect_glInterleavedArrays;
    table[330] = (_glapi_proc) __indirect_glIsTexture;
    table[318] = (_glapi_proc) __indirect_glNormalPointer;
    table[319] = (_glapi_proc) __indirect_glPolygonOffset;
    table[334] = (_glapi_proc) __indirect_glPopClientAttrib;
    table[331] = (_glapi_proc) __indirect_glPrioritizeTextures;
    table[335] = (_glapi_proc) __indirect_glPushClientAttrib;
    table[320] = (_glapi_proc) __indirect_glTexCoordPointer;
    table[332] = (_glapi_proc) __indirect_glTexSubImage1D;
    table[333] = (_glapi_proc) __indirect_glTexSubImage2D;
    table[321] = (_glapi_proc) __indirect_glVertexPointer;

    /* 1.2 */

    table[336] = (_glapi_proc) __indirect_glBlendColor;
    table[337] = (_glapi_proc) __indirect_glBlendEquation;
    table[346] = (_glapi_proc) __indirect_glColorSubTable;
    table[339] = (_glapi_proc) __indirect_glColorTable;
    table[340] = (_glapi_proc) __indirect_glColorTableParameterfv;
    table[341] = (_glapi_proc) __indirect_glColorTableParameteriv;
    table[348] = (_glapi_proc) __indirect_glConvolutionFilter1D;
    table[349] = (_glapi_proc) __indirect_glConvolutionFilter2D;
    table[350] = (_glapi_proc) __indirect_glConvolutionParameterf;
    table[351] = (_glapi_proc) __indirect_glConvolutionParameterfv;
    table[352] = (_glapi_proc) __indirect_glConvolutionParameteri;
    table[353] = (_glapi_proc) __indirect_glConvolutionParameteriv;
    table[347] = (_glapi_proc) __indirect_glCopyColorSubTable;
    table[342] = (_glapi_proc) __indirect_glCopyColorTable;
    table[354] = (_glapi_proc) __indirect_glCopyConvolutionFilter1D;
    table[355] = (_glapi_proc) __indirect_glCopyConvolutionFilter2D;
    table[373] = (_glapi_proc) __indirect_glCopyTexSubImage3D;
    table[338] = (_glapi_proc) __indirect_glDrawRangeElements;
    table[343] = (_glapi_proc) __indirect_glGetColorTable;
    table[344] = (_glapi_proc) __indirect_glGetColorTableParameterfv;
    table[345] = (_glapi_proc) __indirect_glGetColorTableParameteriv;
    table[356] = (_glapi_proc) __indirect_glGetConvolutionFilter;
    table[357] = (_glapi_proc) __indirect_glGetConvolutionParameterfv;
    table[358] = (_glapi_proc) __indirect_glGetConvolutionParameteriv;
    table[361] = (_glapi_proc) __indirect_glGetHistogram;
    table[362] = (_glapi_proc) __indirect_glGetHistogramParameterfv;
    table[363] = (_glapi_proc) __indirect_glGetHistogramParameteriv;
    table[364] = (_glapi_proc) __indirect_glGetMinmax;
    table[365] = (_glapi_proc) __indirect_glGetMinmaxParameterfv;
    table[366] = (_glapi_proc) __indirect_glGetMinmaxParameteriv;
    table[359] = (_glapi_proc) __indirect_glGetSeparableFilter;
    table[367] = (_glapi_proc) __indirect_glHistogram;
    table[368] = (_glapi_proc) __indirect_glMinmax;
    table[369] = (_glapi_proc) __indirect_glResetHistogram;
    table[370] = (_glapi_proc) __indirect_glResetMinmax;
    table[360] = (_glapi_proc) __indirect_glSeparableFilter2D;
    table[371] = (_glapi_proc) __indirect_glTexImage3D;
    table[372] = (_glapi_proc) __indirect_glTexSubImage3D;

    /* 1.3 */

    table[374] = (_glapi_proc) __indirect_glActiveTexture;
    table[375] = (_glapi_proc) __indirect_glClientActiveTexture;
    table[408] = (_glapi_proc) __indirect_glCompressedTexImage1D;
    table[409] = (_glapi_proc) __indirect_glCompressedTexImage2D;
    table[410] = (_glapi_proc) __indirect_glCompressedTexImage3D;
    table[411] = (_glapi_proc) __indirect_glCompressedTexSubImage1D;
    table[412] = (_glapi_proc) __indirect_glCompressedTexSubImage2D;
    table[413] = (_glapi_proc) __indirect_glCompressedTexSubImage3D;
    table[414] = (_glapi_proc) __indirect_glGetCompressedTexImage;
    table[415] = (_glapi_proc) __indirect_glLoadTransposeMatrixd;
    table[416] = (_glapi_proc) __indirect_glLoadTransposeMatrixf;
    table[417] = (_glapi_proc) __indirect_glMultTransposeMatrixd;
    table[418] = (_glapi_proc) __indirect_glMultTransposeMatrixf;
    table[376] = (_glapi_proc) __indirect_glMultiTexCoord1d;
    table[377] = (_glapi_proc) __indirect_glMultiTexCoord1dv;
    table[380] = (_glapi_proc) __indirect_glMultiTexCoord1i;
    table[381] = (_glapi_proc) __indirect_glMultiTexCoord1iv;
    table[382] = (_glapi_proc) __indirect_glMultiTexCoord1s;
    table[383] = (_glapi_proc) __indirect_glMultiTexCoord1sv;
    table[384] = (_glapi_proc) __indirect_glMultiTexCoord2d;
    table[385] = (_glapi_proc) __indirect_glMultiTexCoord2dv;
    table[388] = (_glapi_proc) __indirect_glMultiTexCoord2i;
    table[389] = (_glapi_proc) __indirect_glMultiTexCoord2iv;
    table[390] = (_glapi_proc) __indirect_glMultiTexCoord2s;
    table[391] = (_glapi_proc) __indirect_glMultiTexCoord2sv;
    table[392] = (_glapi_proc) __indirect_glMultiTexCoord3d;
    table[393] = (_glapi_proc) __indirect_glMultiTexCoord3dv;
    table[396] = (_glapi_proc) __indirect_glMultiTexCoord3i;
    table[397] = (_glapi_proc) __indirect_glMultiTexCoord3iv;
    table[398] = (_glapi_proc) __indirect_glMultiTexCoord3s;
    table[399] = (_glapi_proc) __indirect_glMultiTexCoord3sv;
    table[400] = (_glapi_proc) __indirect_glMultiTexCoord4d;
    table[401] = (_glapi_proc) __indirect_glMultiTexCoord4dv;
    table[404] = (_glapi_proc) __indirect_glMultiTexCoord4i;
    table[405] = (_glapi_proc) __indirect_glMultiTexCoord4iv;
    table[406] = (_glapi_proc) __indirect_glMultiTexCoord4s;
    table[407] = (_glapi_proc) __indirect_glMultiTexCoord4sv;
    table[419] = (_glapi_proc) __indirect_glSampleCoverage;

    /* 1.4 */

    table[420] = (_glapi_proc) __indirect_glBlendFuncSeparate;
    table[421] = (_glapi_proc) __indirect_glFogCoordPointer;
    table[422] = (_glapi_proc) __indirect_glFogCoordd;
    table[423] = (_glapi_proc) __indirect_glFogCoorddv;
    table[424] = (_glapi_proc) __indirect_glMultiDrawArrays;
    table[425] = (_glapi_proc) __indirect_glPointParameterf;
    table[426] = (_glapi_proc) __indirect_glPointParameterfv;
    table[427] = (_glapi_proc) __indirect_glPointParameteri;
    table[428] = (_glapi_proc) __indirect_glPointParameteriv;
    table[429] = (_glapi_proc) __indirect_glSecondaryColor3b;
    table[430] = (_glapi_proc) __indirect_glSecondaryColor3bv;
    table[431] = (_glapi_proc) __indirect_glSecondaryColor3d;
    table[432] = (_glapi_proc) __indirect_glSecondaryColor3dv;
    table[433] = (_glapi_proc) __indirect_glSecondaryColor3i;
    table[434] = (_glapi_proc) __indirect_glSecondaryColor3iv;
    table[435] = (_glapi_proc) __indirect_glSecondaryColor3s;
    table[436] = (_glapi_proc) __indirect_glSecondaryColor3sv;
    table[437] = (_glapi_proc) __indirect_glSecondaryColor3ub;
    table[438] = (_glapi_proc) __indirect_glSecondaryColor3ubv;
    table[439] = (_glapi_proc) __indirect_glSecondaryColor3ui;
    table[440] = (_glapi_proc) __indirect_glSecondaryColor3uiv;
    table[441] = (_glapi_proc) __indirect_glSecondaryColor3us;
    table[442] = (_glapi_proc) __indirect_glSecondaryColor3usv;
    table[443] = (_glapi_proc) __indirect_glSecondaryColorPointer;
    table[444] = (_glapi_proc) __indirect_glWindowPos2d;
    table[445] = (_glapi_proc) __indirect_glWindowPos2dv;
    table[446] = (_glapi_proc) __indirect_glWindowPos2f;
    table[447] = (_glapi_proc) __indirect_glWindowPos2fv;
    table[448] = (_glapi_proc) __indirect_glWindowPos2i;
    table[449] = (_glapi_proc) __indirect_glWindowPos2iv;
    table[450] = (_glapi_proc) __indirect_glWindowPos2s;
    table[451] = (_glapi_proc) __indirect_glWindowPos2sv;
    table[452] = (_glapi_proc) __indirect_glWindowPos3d;
    table[453] = (_glapi_proc) __indirect_glWindowPos3dv;
    table[454] = (_glapi_proc) __indirect_glWindowPos3f;
    table[455] = (_glapi_proc) __indirect_glWindowPos3fv;
    table[456] = (_glapi_proc) __indirect_glWindowPos3i;
    table[457] = (_glapi_proc) __indirect_glWindowPos3iv;
    table[458] = (_glapi_proc) __indirect_glWindowPos3s;
    table[459] = (_glapi_proc) __indirect_glWindowPos3sv;

    /* 1.5 */

    table[460] = (_glapi_proc) __indirect_glBeginQuery;
    table[465] = (_glapi_proc) __indirect_glDeleteQueries;
    table[466] = (_glapi_proc) __indirect_glEndQuery;
    table[468] = (_glapi_proc) __indirect_glGenQueries;
    table[472] = (_glapi_proc) __indirect_glGetQueryObjectiv;
    table[473] = (_glapi_proc) __indirect_glGetQueryObjectuiv;
    table[474] = (_glapi_proc) __indirect_glGetQueryiv;
    table[476] = (_glapi_proc) __indirect_glIsQuery;

    /* 2.0 */

    table[481] = (_glapi_proc) __indirect_glBlendEquationSeparate;
    table[488] = (_glapi_proc) __indirect_glDisableVertexAttribArray;
    table[489] = (_glapi_proc) __indirect_glDrawBuffers;
    table[490] = (_glapi_proc) __indirect_glEnableVertexAttribArray;
    table[503] = (_glapi_proc) __indirect_glGetVertexAttribPointerv;
    table[504] = (_glapi_proc) __indirect_glGetVertexAttribdv;
    table[505] = (_glapi_proc) __indirect_glGetVertexAttribfv;
    table[506] = (_glapi_proc) __indirect_glGetVertexAttribiv;
    table[535] = (_glapi_proc) __indirect_glVertexAttrib1d;
    table[536] = (_glapi_proc) __indirect_glVertexAttrib1dv;
    table[537] = (_glapi_proc) __indirect_glVertexAttrib1s;
    table[538] = (_glapi_proc) __indirect_glVertexAttrib1sv;
    table[539] = (_glapi_proc) __indirect_glVertexAttrib2d;
    table[540] = (_glapi_proc) __indirect_glVertexAttrib2dv;
    table[541] = (_glapi_proc) __indirect_glVertexAttrib2s;
    table[542] = (_glapi_proc) __indirect_glVertexAttrib2sv;
    table[543] = (_glapi_proc) __indirect_glVertexAttrib3d;
    table[544] = (_glapi_proc) __indirect_glVertexAttrib3dv;
    table[545] = (_glapi_proc) __indirect_glVertexAttrib3s;
    table[546] = (_glapi_proc) __indirect_glVertexAttrib3sv;
    table[547] = (_glapi_proc) __indirect_glVertexAttrib4Nbv;
    table[548] = (_glapi_proc) __indirect_glVertexAttrib4Niv;
    table[549] = (_glapi_proc) __indirect_glVertexAttrib4Nsv;
    table[550] = (_glapi_proc) __indirect_glVertexAttrib4Nub;
    table[551] = (_glapi_proc) __indirect_glVertexAttrib4Nubv;
    table[552] = (_glapi_proc) __indirect_glVertexAttrib4Nuiv;
    table[553] = (_glapi_proc) __indirect_glVertexAttrib4Nusv;
    table[554] = (_glapi_proc) __indirect_glVertexAttrib4bv;
    table[555] = (_glapi_proc) __indirect_glVertexAttrib4d;
    table[556] = (_glapi_proc) __indirect_glVertexAttrib4dv;
    table[557] = (_glapi_proc) __indirect_glVertexAttrib4iv;
    table[558] = (_glapi_proc) __indirect_glVertexAttrib4s;
    table[559] = (_glapi_proc) __indirect_glVertexAttrib4sv;
    table[560] = (_glapi_proc) __indirect_glVertexAttrib4ubv;
    table[561] = (_glapi_proc) __indirect_glVertexAttrib4uiv;
    table[562] = (_glapi_proc) __indirect_glVertexAttrib4usv;
    table[563] = (_glapi_proc) __indirect_glVertexAttribPointer;

    /* 3.0 */

    table[575] = (_glapi_proc) __indirect_glClampColor;

    /*   1. GL_ARB_multitexture */

    table[378] = (_glapi_proc) __indirect_glMultiTexCoord1fARB;
    table[379] = (_glapi_proc) __indirect_glMultiTexCoord1fvARB;
    table[386] = (_glapi_proc) __indirect_glMultiTexCoord2fARB;
    table[387] = (_glapi_proc) __indirect_glMultiTexCoord2fvARB;
    table[394] = (_glapi_proc) __indirect_glMultiTexCoord3fARB;
    table[395] = (_glapi_proc) __indirect_glMultiTexCoord3fvARB;
    table[402] = (_glapi_proc) __indirect_glMultiTexCoord4fARB;
    table[403] = (_glapi_proc) __indirect_glMultiTexCoord4fvARB;

    /*  26. GL_ARB_vertex_program */

    table[622] = (_glapi_proc) __indirect_glBindProgramARB;
    table[623] = (_glapi_proc) __indirect_glDeleteProgramsARB;
    table[624] = (_glapi_proc) __indirect_glGenProgramsARB;
    table[625] = (_glapi_proc) __indirect_glGetProgramEnvParameterdvARB;
    table[626] = (_glapi_proc) __indirect_glGetProgramEnvParameterfvARB;
    table[627] = (_glapi_proc) __indirect_glGetProgramLocalParameterdvARB;
    table[628] = (_glapi_proc) __indirect_glGetProgramLocalParameterfvARB;
    table[629] = (_glapi_proc) __indirect_glGetProgramStringARB;
    table[630] = (_glapi_proc) __indirect_glGetProgramivARB;
    table[631] = (_glapi_proc) __indirect_glIsProgramARB;
    table[632] = (_glapi_proc) __indirect_glProgramEnvParameter4dARB;
    table[633] = (_glapi_proc) __indirect_glProgramEnvParameter4dvARB;
    table[634] = (_glapi_proc) __indirect_glProgramEnvParameter4fARB;
    table[635] = (_glapi_proc) __indirect_glProgramEnvParameter4fvARB;
    table[636] = (_glapi_proc) __indirect_glProgramLocalParameter4dARB;
    table[637] = (_glapi_proc) __indirect_glProgramLocalParameter4dvARB;
    table[638] = (_glapi_proc) __indirect_glProgramLocalParameter4fARB;
    table[639] = (_glapi_proc) __indirect_glProgramLocalParameter4fvARB;
    table[640] = (_glapi_proc) __indirect_glProgramStringARB;
    table[641] = (_glapi_proc) __indirect_glVertexAttrib1fARB;
    table[642] = (_glapi_proc) __indirect_glVertexAttrib1fvARB;
    table[643] = (_glapi_proc) __indirect_glVertexAttrib2fARB;
    table[644] = (_glapi_proc) __indirect_glVertexAttrib2fvARB;
    table[645] = (_glapi_proc) __indirect_glVertexAttrib3fARB;
    table[646] = (_glapi_proc) __indirect_glVertexAttrib3fvARB;
    table[647] = (_glapi_proc) __indirect_glVertexAttrib4fARB;
    table[648] = (_glapi_proc) __indirect_glVertexAttrib4fvARB;

    /*  45. GL_ARB_framebuffer_object */

    table[661] = (_glapi_proc) __indirect_glBindFramebuffer;
    table[662] = (_glapi_proc) __indirect_glBindRenderbuffer;
    table[663] = (_glapi_proc) __indirect_glBlitFramebuffer;
    table[664] = (_glapi_proc) __indirect_glCheckFramebufferStatus;
    table[665] = (_glapi_proc) __indirect_glDeleteFramebuffers;
    table[666] = (_glapi_proc) __indirect_glDeleteRenderbuffers;
    table[667] = (_glapi_proc) __indirect_glFramebufferRenderbuffer;
    table[668] = (_glapi_proc) __indirect_glFramebufferTexture1D;
    table[669] = (_glapi_proc) __indirect_glFramebufferTexture2D;
    table[670] = (_glapi_proc) __indirect_glFramebufferTexture3D;
    table[671] = (_glapi_proc) __indirect_glFramebufferTextureLayer;
    table[672] = (_glapi_proc) __indirect_glGenFramebuffers;
    table[673] = (_glapi_proc) __indirect_glGenRenderbuffers;
    table[674] = (_glapi_proc) __indirect_glGenerateMipmap;
    table[675] = (_glapi_proc) __indirect_glGetFramebufferAttachmentParameteriv;
    table[676] = (_glapi_proc) __indirect_glGetRenderbufferParameteriv;
    table[677] = (_glapi_proc) __indirect_glIsFramebuffer;
    table[678] = (_glapi_proc) __indirect_glIsRenderbuffer;
    table[679] = (_glapi_proc) __indirect_glRenderbufferStorage;
    table[680] = (_glapi_proc) __indirect_glRenderbufferStorageMultisample;

    /*  25. GL_SGIS_multisample */

    table[1089] = (_glapi_proc) __indirect_glSampleMaskSGIS;
    table[1090] = (_glapi_proc) __indirect_glSamplePatternSGIS;

    /*  30. GL_EXT_vertex_array */

    table[1091] = (_glapi_proc) __indirect_glColorPointerEXT;
    table[1092] = (_glapi_proc) __indirect_glEdgeFlagPointerEXT;
    table[1093] = (_glapi_proc) __indirect_glIndexPointerEXT;
    table[1094] = (_glapi_proc) __indirect_glNormalPointerEXT;
    table[1095] = (_glapi_proc) __indirect_glTexCoordPointerEXT;
    table[1096] = (_glapi_proc) __indirect_glVertexPointerEXT;

    /* 145. GL_EXT_secondary_color */

    table[1171] = (_glapi_proc) __indirect_glSecondaryColor3fEXT;
    table[1172] = (_glapi_proc) __indirect_glSecondaryColor3fvEXT;

    /* 148. GL_EXT_multi_draw_arrays */

    table[1173] = (_glapi_proc) __indirect_glMultiDrawElements;

    /* 149. GL_EXT_fog_coord */

    table[1174] = (_glapi_proc) __indirect_glFogCoordfEXT;
    table[1175] = (_glapi_proc) __indirect_glFogCoordfvEXT;

    /* 233. GL_NV_vertex_program */

    table[1187] = (_glapi_proc) __indirect_glAreProgramsResidentNV;
    table[1188] = (_glapi_proc) __indirect_glExecuteProgramNV;
    table[1189] = (_glapi_proc) __indirect_glGetProgramParameterdvNV;
    table[1190] = (_glapi_proc) __indirect_glGetProgramParameterfvNV;
    table[1191] = (_glapi_proc) __indirect_glGetProgramStringNV;
    table[1192] = (_glapi_proc) __indirect_glGetProgramivNV;
    table[1193] = (_glapi_proc) __indirect_glGetTrackMatrixivNV;
    table[1194] = (_glapi_proc) __indirect_glGetVertexAttribdvNV;
    table[1195] = (_glapi_proc) __indirect_glGetVertexAttribfvNV;
    table[1196] = (_glapi_proc) __indirect_glGetVertexAttribivNV;
    table[1197] = (_glapi_proc) __indirect_glLoadProgramNV;
    table[1198] = (_glapi_proc) __indirect_glProgramParameters4dvNV;
    table[1199] = (_glapi_proc) __indirect_glProgramParameters4fvNV;
    table[1200] = (_glapi_proc) __indirect_glRequestResidentProgramsNV;
    table[1201] = (_glapi_proc) __indirect_glTrackMatrixNV;
    table[1202] = (_glapi_proc) __indirect_glVertexAttrib1dNV;
    table[1203] = (_glapi_proc) __indirect_glVertexAttrib1dvNV;
    table[1204] = (_glapi_proc) __indirect_glVertexAttrib1fNV;
    table[1205] = (_glapi_proc) __indirect_glVertexAttrib1fvNV;
    table[1206] = (_glapi_proc) __indirect_glVertexAttrib1sNV;
    table[1207] = (_glapi_proc) __indirect_glVertexAttrib1svNV;
    table[1208] = (_glapi_proc) __indirect_glVertexAttrib2dNV;
    table[1209] = (_glapi_proc) __indirect_glVertexAttrib2dvNV;
    table[1210] = (_glapi_proc) __indirect_glVertexAttrib2fNV;
    table[1211] = (_glapi_proc) __indirect_glVertexAttrib2fvNV;
    table[1212] = (_glapi_proc) __indirect_glVertexAttrib2sNV;
    table[1213] = (_glapi_proc) __indirect_glVertexAttrib2svNV;
    table[1214] = (_glapi_proc) __indirect_glVertexAttrib3dNV;
    table[1215] = (_glapi_proc) __indirect_glVertexAttrib3dvNV;
    table[1216] = (_glapi_proc) __indirect_glVertexAttrib3fNV;
    table[1217] = (_glapi_proc) __indirect_glVertexAttrib3fvNV;
    table[1218] = (_glapi_proc) __indirect_glVertexAttrib3sNV;
    table[1219] = (_glapi_proc) __indirect_glVertexAttrib3svNV;
    table[1220] = (_glapi_proc) __indirect_glVertexAttrib4dNV;
    table[1221] = (_glapi_proc) __indirect_glVertexAttrib4dvNV;
    table[1222] = (_glapi_proc) __indirect_glVertexAttrib4fNV;
    table[1223] = (_glapi_proc) __indirect_glVertexAttrib4fvNV;
    table[1224] = (_glapi_proc) __indirect_glVertexAttrib4sNV;
    table[1225] = (_glapi_proc) __indirect_glVertexAttrib4svNV;
    table[1226] = (_glapi_proc) __indirect_glVertexAttrib4ubNV;
    table[1227] = (_glapi_proc) __indirect_glVertexAttrib4ubvNV;
    table[1228] = (_glapi_proc) __indirect_glVertexAttribPointerNV;
    table[1229] = (_glapi_proc) __indirect_glVertexAttribs1dvNV;
    table[1230] = (_glapi_proc) __indirect_glVertexAttribs1fvNV;
    table[1231] = (_glapi_proc) __indirect_glVertexAttribs1svNV;
    table[1232] = (_glapi_proc) __indirect_glVertexAttribs2dvNV;
    table[1233] = (_glapi_proc) __indirect_glVertexAttribs2fvNV;
    table[1234] = (_glapi_proc) __indirect_glVertexAttribs2svNV;
    table[1235] = (_glapi_proc) __indirect_glVertexAttribs3dvNV;
    table[1236] = (_glapi_proc) __indirect_glVertexAttribs3fvNV;
    table[1237] = (_glapi_proc) __indirect_glVertexAttribs3svNV;
    table[1238] = (_glapi_proc) __indirect_glVertexAttribs4dvNV;
    table[1239] = (_glapi_proc) __indirect_glVertexAttribs4fvNV;
    table[1240] = (_glapi_proc) __indirect_glVertexAttribs4svNV;
    table[1241] = (_glapi_proc) __indirect_glVertexAttribs4ubvNV;

    /* 268. GL_EXT_stencil_two_side */

    table[1262] = (_glapi_proc) __indirect_glActiveStencilFaceEXT;

    /* 282. GL_NV_fragment_program */

    table[1263] = (_glapi_proc) __indirect_glGetProgramNamedParameterdvNV;
    table[1264] = (_glapi_proc) __indirect_glGetProgramNamedParameterfvNV;
    table[1265] = (_glapi_proc) __indirect_glProgramNamedParameter4dNV;
    table[1266] = (_glapi_proc) __indirect_glProgramNamedParameter4dvNV;
    table[1267] = (_glapi_proc) __indirect_glProgramNamedParameter4fNV;
    table[1268] = (_glapi_proc) __indirect_glProgramNamedParameter4fvNV;

    /* 310. GL_EXT_framebuffer_object */

    table[1274] = (_glapi_proc) __indirect_glBindFramebufferEXT;
    table[1275] = (_glapi_proc) __indirect_glBindRenderbufferEXT;

    return (struct _glapi_table *) table;
}

#endif

