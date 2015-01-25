/*
 * Copyright 2011 VMWare, Inc.
 * All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sub license, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial portions
 * of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.
 * IN NO EVENT SHALL TUNGSTEN GRAPHICS AND/OR ITS SUPPLIERS BE LIABLE FOR
 * ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * Author: Thomas Hellstrom <thellstrom@vmware.com>
 */
#ifndef _VMWGFX_SAA_PRIV_H_
#define _VMWGFX_SAA_PRIV_H_

#define VMWGFX_PIX_MALLOC  (1 << 0)
#define VMWGFX_PIX_GMR     (1 << 1)
#define VMWGFX_PIX_SURFACE (1 << 2)

#include <xorg-server.h>
#include <picturestr.h>
#include "vmwgfx_saa.h"

struct vmwgfx_saa {
    struct saa_driver driver;
    struct vmwgfx_dma_ctx *ctx;
    struct xa_tracker *xat;
    struct xa_context *xa_ctx;
    ScreenPtr pScreen;
    int drm_fd;
    struct vmwgfx_saa_pixmap *src_vpix;
    struct vmwgfx_saa_pixmap *dst_vpix;
    Bool present_copy;
    Bool diff_valid;
    int xdiff;
    int ydiff;
    RegionRec present_region;
    uint32_t src_handle;
    Bool can_optimize_dma;
    Bool use_present_opt;
    Bool only_hw_presents;
    Bool rendercheck;
    Bool is_master;
    Bool known_prime_format;
    void (*present_flush) (ScreenPtr pScreen);
    struct _WsbmListHead sync_x_list;
    struct _WsbmListHead pixmaps;
    struct vmwgfx_composite *vcomp;
};

static inline struct vmwgfx_saa *
to_vmwgfx_saa(struct saa_driver *driver) {
    return (struct vmwgfx_saa *) driver;
}

/*
 * In vmwgfx_saa.c
 */

Bool
vmwgfx_hw_kill(struct vmwgfx_saa *vsaa,
	       struct saa_pixmap *spix);
Bool
vmwgfx_create_hw(struct vmwgfx_saa *vsaa,
		 PixmapPtr pixmap);


/*
 * vmwgfx_xa_surface.c
 */

enum xa_formats
vmwgfx_xa_format(enum _PictFormatShort format);
Bool
vmwgfx_hw_validate(PixmapPtr pixmap, RegionPtr region);

Bool
vmwgfx_hw_accel_stage(PixmapPtr pixmap, unsigned int depth,
		      uint32_t add_flags, uint32_t remove_flags);
Bool
vmwgfx_hw_composite_src_stage(PixmapPtr pixmap,
			      enum _PictFormatShort pict_format);
Bool
vmwgfx_hw_composite_dst_stage(PixmapPtr pixmap,
			      enum _PictFormatShort pict_format);
Bool
vmwgfx_hw_commit(PixmapPtr pixmap);

Bool
vmwgfx_xa_surface_redefine(struct vmwgfx_saa_pixmap *vpix,
			   struct xa_surface *srf,
			   int width,
			   int height,
			   int depth,
			   enum xa_surface_type stype,
			   enum xa_formats rgb_format,
			   unsigned int new_flags,
			   int copy_contents);

/*
 * vmwgfx_xa_composite.c
 */

struct vmwgfx_composite;

void
vmwgfx_free_composite(struct vmwgfx_composite *vcomp);
struct vmwgfx_composite *
vmwgfx_alloc_composite(void);

Bool
vmwgfx_xa_update_comp(struct xa_composite *comp,
		      PixmapPtr src_pix,
		      PixmapPtr mask_pix,
		      PixmapPtr dst_pix);

struct xa_composite *
vmwgfx_xa_setup_comp(struct vmwgfx_composite *vcomp,
		     int op,
		     PicturePtr src_pict,
		     PicturePtr mask_pict,
		     PicturePtr dst_pict);


#endif
