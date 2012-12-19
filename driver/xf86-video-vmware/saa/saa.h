/*
 *
 * Copyright (C) 2000 Keith Packard
 *               2004 Eric Anholt
 *               2005 Zack Rusin
 *
 * Copyright 2011 VMWare, Inc. All rights reserved.
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of copyright holders not be used in
 * advertising or publicity pertaining to distribution of the software without
 * specific, written prior permission. Copyright holders make no
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
 * Author: Based on "exa.h"
 * Author: Thomas Hellstrom <thellstrom@vmware.com>
 */

#ifndef _SAA_H_
#define _SAA_H_

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#else
#include <xorg-server.h>
#endif
#include <xf86.h>
#include <damage.h>
#include <picturestr.h>

#include "../src/compat-api.h"

#define SAA_VERSION_MAJOR 0
#define SAA_VERSION_MINOR 1

#define SAA_ACCESS_R (1 << 0)
#define SAA_ACCESS_W (1 << 1)
#define SAA_ACCESS_RW (SAA_ACCESS_R | SAA_ACCESS_W)

#define SAA_PIXMAP_HINT_CREATE_HW (1 << 25)
#define SAA_PIXMAP_PREFER_SHADOW  (1 << 0)

typedef unsigned int saa_access_t;

enum saa_pixmap_loc {
    saa_loc_driver,
    saa_loc_override,
};

struct saa_pixmap {
    PixmapPtr pixmap;
    int read_access;
    int write_access;
    unsigned int mapped_access;
    Bool fallback_created;
    RegionRec dirty_shadow;
    RegionRec dirty_hw;
    RegionRec shadow_damage;
    DamagePtr damage;
    void *addr;
    void *override;
    enum saa_pixmap_loc auth_loc;
    PictFormatShort src_format;
    PictFormatShort dst_format;
    uint32_t pad[16];
};

struct saa_driver {
    unsigned int saa_major;
    unsigned int saa_minor;
    size_t pixmap_size;
     Bool(*damage) (struct saa_driver * driver, PixmapPtr pixmap,
		    Bool hw, RegionPtr damage);
    void (*operation_complete) (struct saa_driver * driver, PixmapPtr pixmap);
     Bool(*download_from_hw) (struct saa_driver * driver, PixmapPtr pixmap,
			      RegionPtr readback);
    void (*release_from_cpu) (struct saa_driver * driver, PixmapPtr pixmap,
			      saa_access_t access);
    void *(*sync_for_cpu) (struct saa_driver * driver, PixmapPtr pixmap,
			   saa_access_t access);
    void *(*map) (struct saa_driver * driver, PixmapPtr pixmap,
		  saa_access_t access);
    void (*unmap) (struct saa_driver * driver, PixmapPtr pixmap,
		   saa_access_t access);
     Bool(*create_pixmap) (struct saa_driver * driver, struct saa_pixmap * spix,
			   int w, int h, int depth, unsigned int usage_hint,
			   int bpp, int *new_pitch);
    void (*destroy_pixmap) (struct saa_driver * driver, PixmapPtr pixmap);
    Bool (*modify_pixmap_header) (PixmapPtr pixmap, int w, int h, int depth,
				  int bpp, int devkind, void *pPixData);

    Bool (*copy_prepare) (struct saa_driver * driver, PixmapPtr src_pixmap,
			 PixmapPtr dst_pixmap, int dx, int dy, int alu,
			 RegionPtr scr_reg, uint32_t plane_mask);
    void (*copy) (struct saa_driver * driver, int src_x, int src_y, int dst_x,
		  int dst_y, int w, int h);
    void (*copy_done) (struct saa_driver * driver);
    Bool (*composite_prepare) (struct saa_driver *driver, CARD8 op,
			       PicturePtr src_pict, PicturePtr mask_pict,
			       PicturePtr dst_pict,
			       PixmapPtr src_pix, PixmapPtr mask_pix,
			       PixmapPtr dst_pix,
			       RegionPtr src_region,
			       RegionPtr mask_region,
			       RegionPtr dst_region);
    void (*composite) (struct saa_driver *driver,
		       int src_x, int src_y, int mask_x, int mask_y,
		       int dst_x, int dst_y,
		       int width, int height);
    void (*composite_done) (struct saa_driver *driver);

    void (*takedown) (struct saa_driver * driver);
    uint32_t pad[16];
};

extern _X_EXPORT PixmapPtr
saa_get_drawable_pixmap(DrawablePtr pDrawable);

extern _X_EXPORT void
saa_get_drawable_deltas(DrawablePtr pDrawable, PixmapPtr pPixmap,
			int *xp, int *yp);

extern _X_EXPORT PixmapPtr
saa_get_pixmap(DrawablePtr drawable, int *xp, int *yp);

extern _X_EXPORT Bool
saa_prepare_access_pixmap(PixmapPtr pix, saa_access_t access,
			  RegionPtr read_reg);

extern _X_EXPORT Bool
saa_pad_read(DrawablePtr draw);

Bool
saa_pad_read_box(DrawablePtr draw, int x, int y, int w, int h);

extern _X_EXPORT Bool
saa_pad_write(DrawablePtr draw, GCPtr pGC, Bool check_read,
	      saa_access_t * access);

extern _X_EXPORT void
saa_finish_access_pixmap(PixmapPtr pix, saa_access_t access);

extern _X_EXPORT void
saa_fad_read(DrawablePtr draw);

extern _X_EXPORT void
saa_fad_write(DrawablePtr draw, saa_access_t access);

extern _X_EXPORT Bool
saa_resources_init(ScreenPtr screen);

extern _X_EXPORT void
saa_driver_fini(ScreenPtr pScreen);

extern _X_EXPORT int
saa_create_gc(GCPtr pGC);

extern _X_EXPORT RegionPtr
saa_bitmap_to_region(PixmapPtr pPix);

extern _X_EXPORT Bool
saa_close_screen(CLOSE_SCREEN_ARGS_DECL);

extern _X_EXPORT Bool
saa_gc_reads_destination(DrawablePtr pDrawable, GCPtr pGC);

extern _X_EXPORT Bool
saa_op_reads_destination(CARD8 op);

extern _X_EXPORT void
saa_set_fallback_debug(ScreenPtr screen, Bool enable);

extern _X_EXPORT
struct saa_pixmap *saa_get_saa_pixmap(PixmapPtr pPixmap);

extern _X_EXPORT Bool
saa_add_damage(PixmapPtr pixmap);

extern _X_EXPORT struct saa_driver *
saa_get_driver(ScreenPtr pScreen);

extern _X_EXPORT Bool
saa_driver_init(ScreenPtr screen, struct saa_driver *saa_driver);

extern _X_EXPORT void
saa_pixmap_dirty(PixmapPtr pixmap, Bool hw, RegionPtr reg);

extern _X_EXPORT void
saa_drawable_dirty(DrawablePtr draw, Bool hw, RegionPtr reg);

#define SAA_PM_IS_SOLID(_pDrawable, _pm) \
  (((_pm) & FbFullMask((_pDrawable)->depth)) == \
   FbFullMask((_pDrawable)->depth))

#endif
