/* $XFree86: xc/lib/GL/dri/xf86dristr.h,v 1.10 2002/10/30 12:51:25 alanh Exp $ */
/**************************************************************************

Copyright 1998-1999 Precision Insight, Inc., Cedar Park, Texas.
Copyright 2000 VA Linux Systems, Inc.
All Rights Reserved.

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sub license, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice (including the
next paragraph) shall be included in all copies or substantial portions
of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.
IN NO EVENT SHALL PRECISION INSIGHT AND/OR ITS SUPPLIERS BE LIABLE FOR
ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

**************************************************************************/

/*
 * Authors:
 *   Kevin E. Martin <martin@valinux.com>
 *   Jens Owen <jens@tungstengraphics.com>
 *   Rickard E. (Rik) Fiath <faith@valinux.com>
 *
 */

#ifndef _OPENCHROMEDRISTR_H_
#define _OPENCHROMEDRISTR_H_

#include "xf86dri.h"

#define OPENCHROMEDRINAME "XFree86-DRI"

/* The DRI version number.  This was originally set to be the same of the
 * XFree86 version number.  However, this version is really indepedent of
 * the XFree86 version.
 *
 * Version History:
 *    4.0.0: Original
 *    4.0.1: Patch to bump clipstamp when windows are destroyed, 28 May 02
 *    4.1.0: Add transition from single to multi in DRMInfo rec, 24 Jun 02
 */
#define OPENCHROMEDRI_MAJOR_VERSION	4
#define OPENCHROMEDRI_MINOR_VERSION	1
#define OPENCHROMEDRI_PATCH_VERSION	0

typedef struct _OPENCHROMEDRIQueryVersion
{
    CARD8 reqType;		       /* always DRIReqCode */
    CARD8 driReqType;		       /* always X_DRIQueryVersion */
    CARD16 length B16;
} xOPENCHROMEDRIQueryVersionReq;

#define sz_xOPENCHROMEDRIQueryVersionReq	4

typedef struct
{
    BYTE type;			       /* X_Reply */
    BOOL pad1;
    CARD16 sequenceNumber B16;
    CARD32 length B32;
    CARD16 majorVersion B16;	       /* major version of DRI protocol */
    CARD16 minorVersion B16;	       /* minor version of DRI protocol */
    CARD32 patchVersion B32;	       /* patch version of DRI protocol */
    CARD32 pad3 B32;
    CARD32 pad4 B32;
    CARD32 pad5 B32;
    CARD32 pad6 B32;
} xOPENCHROMEDRIQueryVersionReply;

#define sz_xOPENCHROMEDRIQueryVersionReply	32

typedef struct _OPENCHROMEDRIQueryDirectRenderingCapable
{
    CARD8 reqType;		       /* always DRIReqCode */
    CARD8 driReqType;		       /* X_DRIQueryDirectRenderingCapable */
    CARD16 length B16;
    CARD32 screen B32;
} xOPENCHROMEDRIQueryDirectRenderingCapableReq;

#define sz_xOPENCHROMEDRIQueryDirectRenderingCapableReq	8

typedef struct
{
    BYTE type;			       /* X_Reply */
    BOOL pad1;
    CARD16 sequenceNumber B16;
    CARD32 length B32;
    BOOL isCapable;
    BOOL pad2;
    BOOL pad3;
    BOOL pad4;
    CARD32 pad5 B32;
    CARD32 pad6 B32;
    CARD32 pad7 B32;
    CARD32 pad8 B32;
    CARD32 pad9 B32;
} xOPENCHROMEDRIQueryDirectRenderingCapableReply;

#define sz_xOPENCHROMEDRIQueryDirectRenderingCapableReply	32

typedef struct _OPENCHROMEDRIOpenConnection
{
    CARD8 reqType;		       /* always DRIReqCode */
    CARD8 driReqType;		       /* always X_DRIOpenConnection */
    CARD16 length B16;
    CARD32 screen B32;
} xOPENCHROMEDRIOpenConnectionReq;

#define sz_xOPENCHROMEDRIOpenConnectionReq	8

typedef struct
{
    BYTE type;			       /* X_Reply */
    BOOL pad1;
    CARD16 sequenceNumber B16;
    CARD32 length B32;
    CARD32 hSAREALow B32;
    CARD32 hSAREAHigh B32;
    CARD32 busIdStringLength B32;
    CARD32 pad6 B32;
    CARD32 pad7 B32;
    CARD32 pad8 B32;
} xOPENCHROMEDRIOpenConnectionReply;

#define sz_xOPENCHROMEDRIOpenConnectionReply	32

typedef struct _OPENCHROMEDRIAuthConnection
{
    CARD8 reqType;		       /* always DRIReqCode */
    CARD8 driReqType;		       /* always X_DRICloseConnection */
    CARD16 length B16;
    CARD32 screen B32;
    CARD32 magic B32;
} xOPENCHROMEDRIAuthConnectionReq;

#define sz_xOPENCHROMEDRIAuthConnectionReq	12

typedef struct
{
    BYTE type;
    BOOL pad1;
    CARD16 sequenceNumber B16;
    CARD32 length B32;
    CARD32 authenticated B32;
    CARD32 pad2 B32;
    CARD32 pad3 B32;
    CARD32 pad4 B32;
    CARD32 pad5 B32;
    CARD32 pad6 B32;
} xOPENCHROMEDRIAuthConnectionReply;

#define zx_xOPENCHROMEDRIAuthConnectionReply  32

typedef struct _OPENCHROMEDRICloseConnection
{
    CARD8 reqType;		       /* always DRIReqCode */
    CARD8 driReqType;		       /* always X_DRICloseConnection */
    CARD16 length B16;
    CARD32 screen B32;
} xOPENCHROMEDRICloseConnectionReq;

#define sz_xOPENCHROMEDRICloseConnectionReq	8

typedef struct _OPENCHROMEDRIGetClientDriverName
{
    CARD8 reqType;		       /* always DRIReqCode */
    CARD8 driReqType;		       /* always X_DRIGetClientDriverName */
    CARD16 length B16;
    CARD32 screen B32;
} xOPENCHROMEDRIGetClientDriverNameReq;

#define sz_xOPENCHROMEDRIGetClientDriverNameReq	8

typedef struct
{
    BYTE type;			       /* X_Reply */
    BOOL pad1;
    CARD16 sequenceNumber B16;
    CARD32 length B32;
    CARD32 ddxDriverMajorVersion B32;
    CARD32 ddxDriverMinorVersion B32;
    CARD32 ddxDriverPatchVersion B32;
    CARD32 clientDriverNameLength B32;
    CARD32 pad5 B32;
    CARD32 pad6 B32;
} xOPENCHROMEDRIGetClientDriverNameReply;

#define sz_xOPENCHROMEDRIGetClientDriverNameReply	32

typedef struct _OPENCHROMEDRICreateContext
{
    CARD8 reqType;		       /* always DRIReqCode */
    CARD8 driReqType;		       /* always X_DRICreateContext */
    CARD16 length B16;
    CARD32 screen B32;
    CARD32 visual B32;
    CARD32 context B32;
} xOPENCHROMEDRICreateContextReq;

#define sz_xOPENCHROMEDRICreateContextReq	16

typedef struct
{
    BYTE type;			       /* X_Reply */
    BOOL pad1;
    CARD16 sequenceNumber B16;
    CARD32 length B32;
    CARD32 hHWContext B32;
    CARD32 pad2 B32;
    CARD32 pad3 B32;
    CARD32 pad4 B32;
    CARD32 pad5 B32;
    CARD32 pad6 B32;
} xOPENCHROMEDRICreateContextReply;

#define sz_xOPENCHROMEDRICreateContextReply	32

typedef struct _OPENCHROMEDRIDestroyContext
{
    CARD8 reqType;		       /* always DRIReqCode */
    CARD8 driReqType;		       /* always X_DRIDestroyContext */
    CARD16 length B16;
    CARD32 screen B32;
    CARD32 context B32;
} xOPENCHROMEDRIDestroyContextReq;

#define sz_xOPENCHROMEDRIDestroyContextReq	12

typedef struct _OPENCHROMEDRICreateDrawable
{
    CARD8 reqType;		       /* always DRIReqCode */
    CARD8 driReqType;		       /* always X_DRICreateDrawable */
    CARD16 length B16;
    CARD32 screen B32;
    CARD32 drawable B32;
} xOPENCHROMEDRICreateDrawableReq;

#define sz_xOPENCHROMEDRICreateDrawableReq	12

typedef struct
{
    BYTE type;			       /* X_Reply */
    BOOL pad1;
    CARD16 sequenceNumber B16;
    CARD32 length B32;
    CARD32 hHWDrawable B32;
    CARD32 pad2 B32;
    CARD32 pad3 B32;
    CARD32 pad4 B32;
    CARD32 pad5 B32;
    CARD32 pad6 B32;
} xOPENCHROMEDRICreateDrawableReply;

#define sz_xOPENCHROMEDRICreateDrawableReply	32

typedef struct _OPENCHROMEDRIDestroyDrawable
{
    CARD8 reqType;		       /* always DRIReqCode */
    CARD8 driReqType;		       /* always X_DRIDestroyDrawable */
    CARD16 length B16;
    CARD32 screen B32;
    CARD32 drawable B32;
} xOPENCHROMEDRIDestroyDrawableReq;

#define sz_xOPENCHROMEDRIDestroyDrawableReq	12

typedef struct _OPENCHROMEDRIGetDrawableInfo
{
    CARD8 reqType;		       /* always DRIReqCode */
    CARD8 driReqType;		       /* always X_DRIGetDrawableInfo */
    CARD16 length B16;
    CARD32 screen B32;
    CARD32 drawable B32;
} xOPENCHROMEDRIGetDrawableInfoReq;

#define sz_xOPENCHROMEDRIGetDrawableInfoReq	12

typedef struct
{
    BYTE type;			       /* X_Reply */
    BOOL pad1;
    CARD16 sequenceNumber B16;
    CARD32 length B32;
    CARD32 drawableTableIndex B32;
    CARD32 drawableTableStamp B32;
    INT16 drawableX B16;
    INT16 drawableY B16;
    INT16 drawableWidth B16;
    INT16 drawableHeight B16;
    CARD32 numClipRects B32;
    INT16 backX B16;
    INT16 backY B16;
    CARD32 numBackClipRects B32;
} xOPENCHROMEDRIGetDrawableInfoReply;

#define sz_xOPENCHROMEDRIGetDrawableInfoReply	36

typedef struct _OPENCHROMEDRIGetDeviceInfo
{
    CARD8 reqType;		       /* always DRIReqCode */
    CARD8 driReqType;		       /* always X_DRIGetDeviceInfo */
    CARD16 length B16;
    CARD32 screen B32;
} xOPENCHROMEDRIGetDeviceInfoReq;

#define sz_xOPENCHROMEDRIGetDeviceInfoReq	8

typedef struct
{
    BYTE type;			       /* X_Reply */
    BOOL pad1;
    CARD16 sequenceNumber B16;
    CARD32 length B32;
    CARD32 hFrameBufferLow B32;
    CARD32 hFrameBufferHigh B32;
    CARD32 framebufferOrigin B32;
    CARD32 framebufferSize B32;
    CARD32 framebufferStride B32;
    CARD32 devPrivateSize B32;
} xOPENCHROMEDRIGetDeviceInfoReply;

#define sz_xOPENCHROMEDRIGetDeviceInfoReply	32

typedef struct _OPENCHROMEDRIOpenFullScreen
{
    CARD8 reqType;		       /* always DRIReqCode */
    CARD8 driReqType;		       /* always X_DRIOpenFullScreen */
    CARD16 length B16;
    CARD32 screen B32;
    CARD32 drawable B32;
} xOPENCHROMEDRIOpenFullScreenReq;

#define sz_xOPENCHROMEDRIOpenFullScreenReq    12

typedef struct
{
    BYTE type;
    BOOL pad1;
    CARD16 sequenceNumber B16;
    CARD32 length B32;
    CARD32 isFullScreen B32;
    CARD32 pad2 B32;
    CARD32 pad3 B32;
    CARD32 pad4 B32;
    CARD32 pad5 B32;
    CARD32 pad6 B32;
} xOPENCHROMEDRIOpenFullScreenReply;

#define sz_xOPENCHROMEDRIOpenFullScreenReply  32

typedef struct _OPENCHROMEDRICloseFullScreen
{
    CARD8 reqType;		       /* always DRIReqCode */
    CARD8 driReqType;		       /* always X_DRICloseFullScreen */
    CARD16 length B16;
    CARD32 screen B32;
    CARD32 drawable B32;
} xOPENCHROMEDRICloseFullScreenReq;

#define sz_xOPENCHROMEDRICloseFullScreenReq   12

typedef struct
{
    BYTE type;
    BOOL pad1;
    CARD16 sequenceNumber B16;
    CARD32 length B32;
    CARD32 pad2 B32;
    CARD32 pad3 B32;
    CARD32 pad4 B32;
    CARD32 pad5 B32;
    CARD32 pad6 B32;
    CARD32 pad7 B32;
} xOPENCHROMEDRICloseFullScreenReply;

#define sz_xOPENCHROMEDRICloseFullScreenReply  32

#endif /* _OPENCHROMEDRISTR_H_ */
