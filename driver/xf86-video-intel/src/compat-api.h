/*
 * Copyright 2012 Red Hat, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 * Author: Dave Airlie <airlied@redhat.com>
 */

/* this file provides API compat between server post 1.13 and pre it,
   it should be reused inside as many drivers as possible */
#ifndef COMPAT_API_H
#define COMPAT_API_H

#include <xorg-server.h>
#include <xorgVersion.h>

#include <picturestr.h>
#ifndef GLYPH_HAS_GLYPH_PICTURE_ACCESSOR
#define GetGlyphPicture(g, s) GlyphPicture((g))[(s)->myNum]
#define SetGlyphPicture(g, s, p) GlyphPicture((g))[(s)->myNum] = p
#endif

#ifndef XF86_HAS_SCRN_CONV
#define xf86ScreenToScrn(s) xf86Screens[(s)->myNum]
#define xf86ScrnToScreen(s) screenInfo.screens[(s)->scrnIndex]
#endif

#ifndef XF86_SCRN_INTERFACE

#define SCRN_ARG_TYPE int
#define SCRN_INFO_PTR(arg1) ScrnInfoPtr scrn = xf86Screens[(arg1)]

#define SCREEN_ARG_TYPE int
#define SCREEN_PTR(arg1) ScreenPtr screen = screenInfo.screens[(arg1)]

#define SCREEN_INIT_ARGS_DECL int scrnIndex, ScreenPtr screen, int argc, char **argv

#define BLOCKHANDLER_ARGS_DECL int arg, pointer blockData, pointer timeout, pointer read_mask
#define BLOCKHANDLER_ARGS arg, blockData, timeout, read_mask

#define WAKEUPHANDLER_ARGS_DECL int arg, pointer wakeupData, unsigned long result, pointer read_mask
#define WAKEUPHANDLER_ARGS arg, wakeupData, result, read_mask

#define CLOSE_SCREEN_ARGS_DECL int scrnIndex, ScreenPtr screen
#define CLOSE_SCREEN_ARGS scrnIndex, screen

#define ADJUST_FRAME_ARGS_DECL int arg, int x, int y, int flags
#define ADJUST_FRAME_ARGS(arg, x, y) (arg)->scrnIndex, x, y, 0

#define SWITCH_MODE_ARGS_DECL int arg, DisplayModePtr mode, int flags
#define SWITCH_MODE_ARGS(arg, m) (arg)->scrnIndex, m, 0

#define FREE_SCREEN_ARGS_DECL int arg, int flags

#define VT_FUNC_ARGS_DECL int arg, int flags
#define VT_FUNC_ARGS(flags) scrn->scrnIndex, (flags)

#define XF86_ENABLEDISABLEFB_ARG(x) ((x)->scrnIndex)

#else
#define SCRN_ARG_TYPE ScrnInfoPtr
#define SCRN_INFO_PTR(arg1) ScrnInfoPtr scrn = (arg1)

#define SCREEN_ARG_TYPE ScreenPtr
#define SCREEN_PTR(arg1) ScreenPtr screen = (arg1)

#define SCREEN_INIT_ARGS_DECL ScreenPtr screen, int argc, char **argv

#define BLOCKHANDLER_ARGS_DECL ScreenPtr arg, pointer timeout, pointer read_mask
#define BLOCKHANDLER_ARGS arg, timeout, read_mask

#define WAKEUPHANDLER_ARGS_DECL ScreenPtr arg, unsigned long result, pointer read_mask
#define WAKEUPHANDLER_ARGS arg, result, read_mask

#define CLOSE_SCREEN_ARGS_DECL ScreenPtr screen
#define CLOSE_SCREEN_ARGS screen

#define ADJUST_FRAME_ARGS_DECL ScrnInfoPtr arg, int x, int y
#define ADJUST_FRAME_ARGS(arg, x, y) arg, x, y

#define SWITCH_MODE_ARGS_DECL ScrnInfoPtr arg, DisplayModePtr mode
#define SWITCH_MODE_ARGS(arg, m) arg, m

#define FREE_SCREEN_ARGS_DECL ScrnInfoPtr arg

#define VT_FUNC_ARGS_DECL ScrnInfoPtr arg
#define VT_FUNC_ARGS(flags) scrn

#define XF86_ENABLEDISABLEFB_ARG(x) (x)

#endif

static inline int
region_num_rects(const RegionRec *r)
{
	return r->data ? r->data->numRects : 1;
}

static inline int
region_nil(const RegionRec *r)
{
	return region_num_rects(r) == 0;
}

static inline BoxPtr
region_boxptr(const RegionRec *r)
{
	return (BoxPtr)(r->data + 1);
}

static inline const BoxRec *
region_rects(const RegionRec *r)
{
	return r->data ? (const BoxRec *)(r->data + 1) :  &r->extents;
}

#ifndef INCLUDE_LEGACY_REGION_DEFINES
#define RegionCreate(r, s) REGION_CREATE(NULL, r, s)
#define RegionBreak(r) REGION_BREAK(NULL, r)
#define RegionSizeof REGION_SZOF
#define RegionBoxptr REGION_BOXPTR
#define RegionEnd REGION_END
#define RegionExtents(r) REGION_EXTENTS(NULL, r)
#define RegionRects REGION_RECTS
#define RegionNumRects REGION_NUM_RECTS
#define RegionContainsRect(r, b) RECT_IN_REGION(NULL, r, b)
#define RegionContainsPoint(r, x, y, b) POINT_IN_REGION(NULL, r, x, y, b)
#define RegionCopy(res, r) REGION_COPY(NULL, res, r)
#define RegionIntersect(res, r1, r2) REGION_INTERSECT(NULL, res, r1, r2)
#define RegionUnion(res, r1, r2) REGION_UNION(NULL, res, r1, r2)
#define RegionSubtract(res, r1, r2) REGION_SUBTRACT(NULL, res, r1, r2)
#define RegionTranslate(r, x, y) REGION_TRANSLATE(NULL, r, x, y)
#define RegionUninit(r) REGION_UNINIT(NULL, r)
#define region_from_bitmap BITMAP_TO_REGION
#define RegionNil REGION_NIL
#define RegionNull(r) REGION_NULL(NULL, r)
#define RegionNotEmpty(r) REGION_NOTEMPTY(NULL, r)
#define RegionEmpty(r) REGION_EMPTY(NULL, r)
#define RegionEqual(a, b) REGION_EQUAL(NULL, a, b)
#define RegionDestroy(r) REGION_DESTROY(NULL, r)
#else
#define region_from_bitmap BitmapToRegion
#endif

#ifndef _X_UNUSED
#define _X_UNUSED
#endif

#if HAS_DEVPRIVATEKEYREC
#define __get_private(p, key) dixGetPrivateAddr(&(p)->devPrivates, &(key))
#else
#define __get_private(p, key) dixLookupPrivate(&(p)->devPrivates, &(key))
typedef int DevPrivateKeyRec;
static inline void FreePixmap(PixmapPtr pixmap)
{
	dixFreePrivates(pixmap->devPrivates);
	free(pixmap);
}
#endif

#if !HAS_DIXREGISTERPRIVATEKEY
#define dixPrivateKeyRegistered(key__) (*(key__) != 0)
#endif

#if XORG_VERSION_CURRENT >= XORG_VERSION_NUMERIC(1,9,99,902,0)
#define SourceValidate(d, x, y, w, h, mode) \
	if ((d)->pScreen->SourceValidate) (d)->pScreen->SourceValidate(d, x, y, w, h, mode)
#else
#define SourceValidate(d, x, y, w, h, mode) \
	if ((d)->pScreen->SourceValidate) (d)->pScreen->SourceValidate(d, x, y, w, h)
#endif

#if XORG_VERSION_CURRENT >= XORG_VERSION_NUMERIC(1,14,99,2,0)
#define DamageUnregister(d, dd) DamageUnregister(dd)
#endif

#if XORG_VERSION_CURRENT >= XORG_VERSION_NUMERIC(1,16,99,1,0)

#define XORG_XV_VERSION 2
#define ddStopVideo_ARGS XvPortPtr port, DrawablePtr draw
#define ddSetPortAttribute_ARGS XvPortPtr port, Atom attribute, INT32 value
#define ddGetPortAttribute_ARGS XvPortPtr port, Atom attribute, INT32 *value
#define ddQueryBestSize_ARGS XvPortPtr port, CARD8 motion, CARD16 vid_w, CARD16 vid_h, CARD16 drw_w, CARD16 drw_h, unsigned int *p_w, unsigned int *p_h
#define ddPutImage_ARGS DrawablePtr draw, XvPortPtr port, GCPtr gc, INT16 src_x, INT16 src_y, CARD16 src_w, CARD16 src_h, INT16 drw_x, INT16 drw_y, CARD16 drw_w, CARD16 drw_h, XvImagePtr format, unsigned char *buf, Bool sync, CARD16 width, CARD16 height
#define ddQueryImageAttributes_ARGS XvPortPtr port, XvImagePtr format, unsigned short *w, unsigned short *h, int *pitches, int *offsets

#else

#define XORG_XV_VERSION 1
#define ddStopVideo_ARGS ClientPtr client, XvPortPtr port, DrawablePtr draw
#define ddSetPortAttribute_ARGS ClientPtr client, XvPortPtr port, Atom attribute, INT32 value
#define ddGetPortAttribute_ARGS ClientPtr client, XvPortPtr port, Atom attribute, INT32 *value
#define ddQueryBestSize_ARGS ClientPtr client, XvPortPtr port, CARD8 motion, CARD16 vid_w, CARD16 vid_h, CARD16 drw_w, CARD16 drw_h, unsigned int *p_w, unsigned int *p_h
#define ddPutImage_ARGS ClientPtr client, DrawablePtr draw, XvPortPtr port, GCPtr gc, INT16 src_x, INT16 src_y, CARD16 src_w, CARD16 src_h, INT16 drw_x, INT16 drw_y, CARD16 drw_w, CARD16 drw_h, XvImagePtr format, unsigned char *buf, Bool sync, CARD16 width, CARD16 height
#define ddQueryImageAttributes_ARGS ClientPtr client, XvPortPtr port, XvImagePtr format, unsigned short *w, unsigned short *h, int *pitches, int *offsets

#endif

#if XORG_VERSION_CURRENT >= XORG_VERSION_NUMERIC(1,16,99,1,0)
#include <mi.h>
#define miHandleExposures(pSrcDrawable, pDstDrawable, \
                         pGC, srcx, srcy, width, height, \
                         dstx, dsty, plane) \
       miHandleExposures(pSrcDrawable, pDstDrawable, \
                         pGC, srcx, srcy, width, height, \
                         dstx, dsty)
#endif

#endif
