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

#ifndef _VMWGFX_SAA_H_
#define _VMWGFX_SAA_H_

#include "saa.h"
#include <xa_composite.h>
#include "vmwgfx_drmi.h"
#include "wsbm_util.h"


#define VMWGFX_FLAG_FORCE_GMR     (1 << 0) /* Create with GMR as backing store */
#define VMWGFX_FLAG_FORCE_SURFACE (1 << 1) /* Create with surface as backing store */
#define VMWGFX_FLAG_AVOID_HWACCEL (1 << 2) /* Avoid Hardware acceleration on this pixmap */
#define VMWGFX_FLAG_USE_PRESENT   (1 << 3) /* Use presents when copying to this pixmap */

struct vmwgfx_saa_pixmap {
    struct saa_pixmap base;
    RegionPtr dirty_present;
    RegionPtr present_damage;
    RegionPtr pending_update;
    RegionPtr pending_present;
    uint32_t usage_flags;
    uint32_t backing;
    void *malloc;
    struct vmwgfx_dmabuf *gmr;
    struct xa_surface *hw;
    uint32_t fb_id;
    int hw_is_dri2_fronts;
    Bool hw_is_hosted;
    struct _WsbmListHead sync_x_head;
    struct _WsbmListHead scanout_list;
    struct _WsbmListHead pixmap_list;

    uint32_t xa_flags;
    uint32_t staging_add_flags;
    uint32_t staging_remove_flags;
    enum xa_formats staging_format;
};

struct vmwgfx_screen_entry {
    struct _WsbmListHead scanout_head;
    PixmapPtr pixmap;
};

static inline struct vmwgfx_saa_pixmap *
to_vmwgfx_saa_pixmap(struct saa_pixmap *spix)
{
    return (struct vmwgfx_saa_pixmap *) spix;
}

static inline struct vmwgfx_saa_pixmap*
vmwgfx_saa_pixmap(PixmapPtr pix)
{
    return to_vmwgfx_saa_pixmap(saa_get_saa_pixmap(pix));
}

extern Bool
vmwgfx_saa_init(ScreenPtr pScreen, int drm_fd, struct xa_tracker *xat,
		void (*present_flush)(ScreenPtr pScreen),
		Bool direct_presents,
		Bool only_hw_presents,
		Bool rendercheck);

extern uint32_t
vmwgfx_scanout_ref(struct vmwgfx_screen_entry *box);

extern void
vmwgfx_scanout_unref(struct vmwgfx_screen_entry *box);

extern void
vmwgfx_scanout_refresh(PixmapPtr pixmap);

extern void
vmwgfx_remove_dri2_list(struct vmwgfx_saa_pixmap *vpix);

extern void
vmwgfx_flush_dri2(ScreenPtr pScreen);

extern Bool
vmwgfx_hw_dri2_validate(PixmapPtr pixmap, unsigned int depth);

Bool
vmwgfx_hw_accel_validate(PixmapPtr pixmap, unsigned int depth,
			 uint32_t add_flags, uint32_t remove_flags,
			 RegionPtr region);

void
vmwgfx_saa_set_master(ScreenPtr pScreen);

void
vmwgfx_saa_drop_master(ScreenPtr pScreen);

#if (XA_TRACKER_VERSION_MAJOR >= 2) && defined(HAVE_LIBDRM_2_4_38)
Bool
vmwgfx_saa_copy_to_surface(DrawablePtr pDraw, uint32_t surface_fd,
			   const BoxRec *dst_box, RegionPtr region);
#endif /* (XA_TRACKER_VERSION_MAJOR >= 2) && defined(HAVE_LIBDRM_2_4_38) */

#if (XA_TRACKER_VERSION_MAJOR <= 1) && !defined(HAVE_XA_2)

#define _xa_surface_handle(_a, _b, _c) xa_surface_handle(_a, _b, _c)
#define xa_context_flush(_a)

#else

#define xa_surface_destroy(_a) xa_surface_unref(_a)
#define _xa_surface_handle(_a, _b, _c)		\
    xa_surface_handle(_a, xa_handle_type_shared, _b, _c)

#endif /*  (XA_TRACKER_VERSION_MAJOR <= 1) */
#endif
