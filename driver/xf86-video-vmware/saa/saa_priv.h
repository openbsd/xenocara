/*
 *
 * Copyright (C) 2000 Keith Packard, member of The XFree86 Project, Inc.
 *               2005 Zack Rusin, Trolltech
 * Copyright 2011 VMWare, inc. All rights reserved.
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of Keith Packard not be used in
 * advertising or publicity pertaining to distribution of the software without
 * specific, written prior permission.  Keith Packard makes no
 * representations about the suitability of this software for any purpose.  It
 * is provided "as is" without express or implied warranty.
 *
 * THE COPYRIGHT HOLDERS DISCLAIM ALL WARRANTIES WITH REGARD TO THIS
 * SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS, IN NO EVENT SHALL THE COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN
 * AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING
 * OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
 * SOFTWARE.
 *
 * Authors: Based on exa_priv.h
 * Authors: Thomas Hellstrom <thellstrom@vmware.com>
 */

#ifndef _SAA_PRIV_H
#define _SAA_PRIV_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#else
#include <xorg-server.h>
#endif
#include "xf86.h"

#include "saa.h"

#include <X11/X.h>
#include <X11/Xproto.h>
#include "scrnintstr.h"
#include "pixmapstr.h"
#include "windowstr.h"
#include "servermd.h"
#include "colormapst.h"
#include "gcstruct.h"
#include "input.h"
#include "mipointer.h"
#include "mi.h"
#include "dix.h"
#include "fb.h"
#ifdef RENDER
#include "glyphstr.h"
#endif
#include "damage.h"
#include "../src/common_compat.h"

#define SAA_INVALID_ADDRESS \
  ((void *) ((unsigned long) 0xFFFFFFFF - 1024*1024))

struct saa_gc_priv {
    /* GC values from the layer below. */
    CONST_ABI_18_0 GCOps *saved_ops;
    CONST_ABI_18_0 GCFuncs *saved_funcs;
};

struct saa_screen_priv {
    struct saa_driver *driver;
    CreateGCProcPtr saved_CreateGC;
    CloseScreenProcPtr saved_CloseScreen;
    GetImageProcPtr saved_GetImage;
    GetSpansProcPtr saved_GetSpans;
    CreatePixmapProcPtr saved_CreatePixmap;
    DestroyPixmapProcPtr saved_DestroyPixmap;
    CopyWindowProcPtr saved_CopyWindow;
    ChangeWindowAttributesProcPtr saved_ChangeWindowAttributes;
    BitmapToRegionProcPtr saved_BitmapToRegion;
    ModifyPixmapHeaderProcPtr saved_ModifyPixmapHeader;
#ifdef RENDER
    CompositeProcPtr saved_Composite;
    CompositeRectsProcPtr saved_CompositeRects;
    TrianglesProcPtr saved_Triangles;
    GlyphsProcPtr saved_Glyphs;
    TrapezoidsProcPtr saved_Trapezoids;
    AddTrapsProcPtr saved_AddTraps;
    UnrealizeGlyphProcPtr saved_UnrealizeGlyph;
    SourceValidateProcPtr saved_SourceValidate;
#endif
    Bool fallback_debug;

    unsigned int fallback_count;

    RegionRec srcReg;
    RegionRec maskReg;
    DrawablePtr srcDraw;
};

extern GCOps saa_gc_ops;

#if DEBUG_TRACE_FALL
#define SAA_FALLBACK(x)						\
do {								\
	ErrorF("SAA fallback at %s: ", __FUNCTION__);		\
	ErrorF x;						\
} while (0)

#define saa_drawable_location() ("u")
#else
#define SAA_FALLBACK(x)
#endif

/*
 * Some macros to deal with function wrapping.
 */
#define saa_wrap(priv, real, mem, func) {\
	(priv)->saved_##mem = (real)->mem;	\
	(real)->mem = func;			\
}

#define saa_unwrap(priv, real, mem) {\
	(real)->mem = (priv)->saved_##mem;	\
}

#define saa_swap(priv, real, mem) {\
	CONST_ABI_18_0 void *tmp = (priv)->saved_##mem;		\
	(priv)->saved_##mem = (real)->mem;	\
	(real)->mem = tmp;			\
}

#if (GET_ABI_MAJOR(ABI_VIDEODRV_VERSION) >= 8)
#define SAA_DEVPRIVATEKEYREC 1

extern DevPrivateKeyRec saa_screen_index;
extern DevPrivateKeyRec saa_pixmap_index;
extern DevPrivateKeyRec saa_gc_index;

static inline struct saa_screen_priv *
saa_screen(ScreenPtr screen)
{
    return (struct saa_screen_priv *)dixGetPrivate(&screen->devPrivates,
						   &saa_screen_index);
}

static inline struct saa_gc_priv *
saa_gc(GCPtr gc)
{
    return (struct saa_gc_priv *)dixGetPrivateAddr(&gc->devPrivates,
						   &saa_gc_index);
}

static inline struct saa_pixmap *
saa_pixmap(PixmapPtr pix)
{
    return (struct saa_pixmap *)dixGetPrivateAddr(&pix->devPrivates,
						  &saa_pixmap_index);
}
#else
#undef SAA_DEVPRIVATEKEYREC
extern int saa_screen_index;
extern int saa_pixmap_index;
extern int saa_gc_index;

static inline struct saa_screen_priv *
saa_screen(ScreenPtr screen)
{
    return (struct saa_screen_priv *)dixLookupPrivate(&screen->devPrivates,
						      &saa_screen_index);
}

static inline struct saa_gc_priv *
saa_gc(GCPtr gc)
{
    return (struct saa_gc_priv *)dixLookupPrivateAddr(&gc->devPrivates,
						      &saa_gc_index);
}

static inline struct saa_pixmap *
saa_pixmap(PixmapPtr pix)
{
    return (struct saa_pixmap *)dixLookupPrivateAddr(&pix->devPrivates,
							  &saa_pixmap_index);
}

#endif

extern void
saa_check_fill_spans(DrawablePtr pDrawable, GCPtr pGC, int nspans,
		     DDXPointPtr ppt, int *pwidth, int fSorted);
extern void
saa_check_poly_fill_rect(DrawablePtr pDrawable, GCPtr pGC,
			 int nrect, xRectangle * prect);
extern RegionPtr
saa_check_copy_area(DrawablePtr pSrc, DrawablePtr pDst, GCPtr pGC,
		    int srcx, int srcy, int w, int h, int dstx, int dsty);
extern void
saa_check_copy_nton(DrawablePtr pSrc, DrawablePtr pDst, GCPtr pGC,
		    BoxPtr pbox, int nbox, int dx, int dy, Bool reverse,
		    Bool upsidedown, Pixel bitplane, void *closure);

extern void
saa_unaccel_setup(ScreenPtr pScreen);

extern void
saa_unaccel_takedown(ScreenPtr pScreen);

extern RegionPtr
saa_copy_area(DrawablePtr pSrcDrawable, DrawablePtr pDstDrawable, GCPtr pGC,
	      int srcx, int srcy, int width, int height, int dstx, int dsty);

extern Bool
saa_hw_copy_nton(DrawablePtr pSrcDrawable,
		 DrawablePtr pDstDrawable,
		 GCPtr pGC,
		 BoxPtr pbox,
		 int nbox, int dx, int dy, Bool reverse, Bool upsidedown);

#ifdef RENDER
extern void
saa_render_setup(ScreenPtr pScreen);

extern void
saa_render_takedown(ScreenPtr pScreen);


extern void
saa_check_composite(CARD8 op,
		    PicturePtr pSrc,
		    PicturePtr pMask,
		    PicturePtr pDst,
		    INT16 xSrc,
		    INT16 ySrc,
		    INT16 xMask,
		    INT16 yMask,
		    INT16 xDst, INT16 yDst, CARD16 width, CARD16 height,
		    RegionPtr src_region,
		    RegionPtr mask_region,
		    RegionPtr dst_region);
#endif

extern Bool
saa_modify_pixmap_header(PixmapPtr pPixmap, int width, int height, int depth,
			 int bitsPerPixel, int devKind, pointer pPixData);

extern PixmapPtr
saa_create_pixmap(ScreenPtr pScreen, int w, int h, int depth,
		  unsigned usage_hint);

extern Bool
saa_destroy_pixmap(PixmapPtr pPixmap);

static inline RegionPtr
saa_pix_damage_pending(struct saa_pixmap *spix)
{
    return (spix->damage ? DamagePendingRegion(spix->damage) : NULL);
}

extern RegionPtr
saa_boxes_to_region(ScreenPtr pScreen, int nbox, BoxPtr pbox, int ordering);


Bool
saa_compute_composite_regions(ScreenPtr pScreen,
			      PicturePtr pSrc,
			      PicturePtr pMask,
			      PicturePtr pDst,
			      INT16 xSrc, INT16 ySrc, INT16 xMask,
			      INT16 yMask, INT16 xDst,
			      INT16 yDst, INT16 width, INT16 height,
			      RegionPtr dst_reg,
			      RegionPtr *src_reg,
			      RegionPtr *mask_reg);

#endif
