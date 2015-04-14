/*
 * Copyright 2008 Kristian Høgsberg 
 * Copyright 2008 Jérôme Glisse
 *
 * All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation on the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial
 * portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NON-INFRINGEMENT.  IN NO EVENT SHALL ATI, VA LINUX SYSTEMS AND/OR
 * THEIR SUPPLIERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "radeon.h"
#include "radeon_dri2.h"
#include "radeon_video.h"

#ifdef DRI2

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#include "radeon_version.h"

#if HAVE_LIST_H
#include "list.h"
#if !HAVE_XORG_LIST
#define xorg_list			list
#define xorg_list_init			list_init
#define xorg_list_add			list_add
#define xorg_list_del			list_del
#define xorg_list_for_each_entry	list_for_each_entry
#endif
#endif


#include "radeon_bo_gem.h"

#if DRI2INFOREC_VERSION >= 4 && HAVE_LIST_H
#define USE_DRI2_SCHEDULING
#endif

#if DRI2INFOREC_VERSION >= 9
#define USE_DRI2_PRIME
#endif

#define FALLBACK_SWAP_DELAY 16

#ifdef USE_GLAMOR
#include <glamor.h>
#endif

typedef DRI2BufferPtr BufferPtr;

struct dri2_buffer_priv {
    PixmapPtr   pixmap;
    unsigned int attachment;
    unsigned int refcnt;
};


static PixmapPtr get_drawable_pixmap(DrawablePtr drawable)
{
    if (drawable->type == DRAWABLE_PIXMAP)
	return (PixmapPtr)drawable;
    else
	return (*drawable->pScreen->GetWindowPixmap)((WindowPtr)drawable);
}


static PixmapPtr fixup_glamor(DrawablePtr drawable, PixmapPtr pixmap)
{
	PixmapPtr old = get_drawable_pixmap(drawable);
#ifdef USE_GLAMOR
	ScreenPtr screen = drawable->pScreen;
	struct radeon_pixmap *priv = radeon_get_pixmap_private(pixmap);
	GCPtr gc;

	/* With a glamor pixmap, 2D pixmaps are created in texture
	 * and without a static BO attached to it. To support DRI,
	 * we need to create a new textured-drm pixmap and
	 * need to copy the original content to this new textured-drm
	 * pixmap, and then convert the old pixmap to a coherent
	 * textured-drm pixmap which has a valid BO attached to it
	 * and also has a valid texture, thus both glamor and DRI2
	 * can access it.
	 *
	 */

	/* Copy the current contents of the pixmap to the bo. */
	gc = GetScratchGC(drawable->depth, screen);
	if (gc) {
		ValidateGC(&pixmap->drawable, gc);
		gc->ops->CopyArea(&old->drawable, &pixmap->drawable,
				  gc,
				  0, 0,
				  old->drawable.width,
				  old->drawable.height,
				  0, 0);
		FreeScratchGC(gc);
	}

	radeon_set_pixmap_private(pixmap, NULL);

	/* And redirect the pixmap to the new bo (for 3D). */
	glamor_egl_exchange_buffers(old, pixmap);
	radeon_set_pixmap_private(old, priv);
	screen->DestroyPixmap(pixmap);
	old->refcnt++;

	screen->ModifyPixmapHeader(old,
				   old->drawable.width,
				   old->drawable.height,
				   0, 0,
				   priv->stride,
				   NULL);

#endif /* USE_GLAMOR*/

	return old;
}


static BufferPtr
radeon_dri2_create_buffer2(ScreenPtr pScreen,
			   DrawablePtr drawable,
			   unsigned int attachment,
			   unsigned int format)
{
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
    RADEONInfoPtr info = RADEONPTR(pScrn);
    BufferPtr buffers;
    struct dri2_buffer_priv *privates;
    PixmapPtr pixmap, depth_pixmap;
    struct radeon_bo *bo;
    int flags;
    unsigned front_width;
    uint32_t tiling = 0;
    unsigned aligned_width = drawable->width;
    unsigned height = drawable->height;
    Bool is_glamor_pixmap = FALSE;
    int depth;
    int cpp;

    if (format) {
	depth = format;

	switch (depth) {
	case 15:
	    cpp = 2;
	    break;
	case 24:
	    cpp = 4;
	    break;
	default:
	    cpp = depth / 8;
	}
    } else {
	depth = drawable->depth;
	cpp = drawable->bitsPerPixel / 8;
    }

    pixmap = pScreen->GetScreenPixmap(pScreen);
    front_width = pixmap->drawable.width;

    pixmap = depth_pixmap = NULL;

    if (attachment == DRI2BufferFrontLeft) {
        pixmap = get_drawable_pixmap(drawable);
	if (pScreen != pixmap->drawable.pScreen)
	    pixmap = NULL;
	else if (info->use_glamor && !radeon_get_pixmap_bo(pixmap)) {
	    is_glamor_pixmap = TRUE;
	    aligned_width = pixmap->drawable.width;
	    height = pixmap->drawable.height;
	    pixmap = NULL;
	} else
	    pixmap->refcnt++;
    } else if (attachment == DRI2BufferStencil && depth_pixmap) {
        pixmap = depth_pixmap;
        pixmap->refcnt++;
    }

    if (!pixmap && (is_glamor_pixmap || attachment != DRI2BufferFrontLeft)) {
	/* tile the back buffer */
	switch(attachment) {
	case DRI2BufferDepth:
	    /* macro is the preferred setting, but the 2D detiling for software
	     * fallbacks in mesa still has issues on some configurations
	     */
	    if (info->ChipFamily >= CHIP_FAMILY_R600) {
		if (info->allowColorTiling2D) {
			flags = RADEON_CREATE_PIXMAP_TILING_MACRO;
		} else {
			flags = RADEON_CREATE_PIXMAP_TILING_MICRO;
		}
		if (info->ChipFamily >= CHIP_FAMILY_CEDAR)
		    flags |= RADEON_CREATE_PIXMAP_SZBUFFER;
	    } else if (cpp == 2 && info->ChipFamily >= CHIP_FAMILY_R300)
		flags = RADEON_CREATE_PIXMAP_TILING_MACRO | RADEON_CREATE_PIXMAP_TILING_MICRO_SQUARE;
	    else
		flags = RADEON_CREATE_PIXMAP_TILING_MACRO | RADEON_CREATE_PIXMAP_TILING_MICRO;
	    if (IS_R200_3D || info->ChipFamily == CHIP_FAMILY_RV200 || info->ChipFamily == CHIP_FAMILY_RADEON)
		flags |= RADEON_CREATE_PIXMAP_DEPTH;
	    break;
	case DRI2BufferDepthStencil:
	    /* macro is the preferred setting, but the 2D detiling for software
	     * fallbacks in mesa still has issues on some configurations
	     */
	    if (info->ChipFamily >= CHIP_FAMILY_R600) {
		if (info->allowColorTiling2D) {
			flags = RADEON_CREATE_PIXMAP_TILING_MACRO;
		} else {
			flags = RADEON_CREATE_PIXMAP_TILING_MICRO;
		}
		if (info->ChipFamily >= CHIP_FAMILY_CEDAR)
		    flags |= RADEON_CREATE_PIXMAP_SZBUFFER;
	    } else if (cpp == 2 && info->ChipFamily >= CHIP_FAMILY_R300)
		flags = RADEON_CREATE_PIXMAP_TILING_MACRO | RADEON_CREATE_PIXMAP_TILING_MICRO_SQUARE;
	    else
		flags = RADEON_CREATE_PIXMAP_TILING_MACRO | RADEON_CREATE_PIXMAP_TILING_MICRO;
	    if (IS_R200_3D || info->ChipFamily == CHIP_FAMILY_RV200 || info->ChipFamily == CHIP_FAMILY_RADEON)
		flags |= RADEON_CREATE_PIXMAP_DEPTH;
		
	    break;
	case DRI2BufferBackLeft:
	case DRI2BufferBackRight:
	case DRI2BufferFrontLeft:
	case DRI2BufferFrontRight:
	case DRI2BufferFakeFrontLeft:
	case DRI2BufferFakeFrontRight:
	    if (info->ChipFamily >= CHIP_FAMILY_R600) {
		if (info->allowColorTiling2D) {
			flags = RADEON_CREATE_PIXMAP_TILING_MACRO;
		} else {
			flags = RADEON_CREATE_PIXMAP_TILING_MICRO;
		}
	    } else
		flags = RADEON_CREATE_PIXMAP_TILING_MACRO;
	    break;
	default:
	    flags = 0;
	}

	if (flags & RADEON_CREATE_PIXMAP_TILING_MICRO)
	    tiling |= RADEON_TILING_MICRO;
	if (flags & RADEON_CREATE_PIXMAP_TILING_MICRO_SQUARE)
	    tiling |= RADEON_TILING_MICRO_SQUARE;
	if (flags & RADEON_CREATE_PIXMAP_TILING_MACRO)
	    tiling |= RADEON_TILING_MACRO;

	    if (aligned_width == front_width)
		aligned_width = pScrn->virtualX;

	    pixmap = (*pScreen->CreatePixmap)(pScreen,
					      aligned_width,
					      height,
					      depth,
					      flags | RADEON_CREATE_PIXMAP_DRI2);
    }

    buffers = calloc(1, sizeof *buffers);
    if (buffers == NULL)
        goto error;

    if (attachment == DRI2BufferDepth) {
        depth_pixmap = pixmap;
    }

    if (pixmap) {
	if (!info->use_glamor) {
	    info->exa_force_create = TRUE;
	    exaMoveInPixmap(pixmap);
	    info->exa_force_create = FALSE;
	    if (exaGetPixmapDriverPrivate(pixmap) == NULL) {
		/* this happen if pixmap is non accelerable */
		goto error;
	    }
	}

	if (is_glamor_pixmap)
	    pixmap = fixup_glamor(drawable, pixmap);
	bo = radeon_get_pixmap_bo(pixmap);
	if (!bo || radeon_gem_get_kernel_name(bo, &buffers->name) != 0)
	    goto error;
    }

    privates = calloc(1, sizeof(struct dri2_buffer_priv));
    if (privates == NULL)
        goto error;

    buffers->attachment = attachment;
    if (pixmap) {
	buffers->pitch = pixmap->devKind;
	buffers->cpp = cpp;
    }
    buffers->driverPrivate = privates;
    buffers->format = format;
    buffers->flags = 0; /* not tiled */
    privates->pixmap = pixmap;
    privates->attachment = attachment;
    privates->refcnt = 1;

    return buffers;

error:
    free(buffers);
    if (pixmap)
        (*pScreen->DestroyPixmap)(pixmap);
    return NULL;
}

DRI2BufferPtr
radeon_dri2_create_buffer(DrawablePtr pDraw, unsigned int attachment,
			   unsigned int format)
{
	return radeon_dri2_create_buffer2(pDraw->pScreen, pDraw,
					  attachment, format);
}

static void
radeon_dri2_destroy_buffer2(ScreenPtr pScreen,
			    DrawablePtr drawable, BufferPtr buffers)
{
    if(buffers)
    {
        struct dri2_buffer_priv *private = buffers->driverPrivate;

        /* Trying to free an already freed buffer is unlikely to end well */
        if (private->refcnt == 0) {
            ScrnInfoPtr scrn = xf86ScreenToScrn(pScreen);

            xf86DrvMsg(scrn->scrnIndex, X_WARNING, 
                       "Attempted to destroy previously destroyed buffer.\
 This is a programming error\n");
            return;
        }

        private->refcnt--;
        if (private->refcnt == 0)
        {
	    if (private->pixmap)
                (*pScreen->DestroyPixmap)(private->pixmap);

            free(buffers->driverPrivate);
            free(buffers);
        }
    }
}

void
radeon_dri2_destroy_buffer(DrawablePtr pDraw, DRI2BufferPtr buf)
{
    radeon_dri2_destroy_buffer2(pDraw->pScreen, pDraw, buf);
}


static inline PixmapPtr GetDrawablePixmap(DrawablePtr drawable)
{
    if (drawable->type == DRAWABLE_PIXMAP)
        return (PixmapPtr)drawable;
    else {
        struct _Window *pWin = (struct _Window *)drawable;
        return drawable->pScreen->GetWindowPixmap(pWin);
    }
}
static void
radeon_dri2_copy_region2(ScreenPtr pScreen,
			 DrawablePtr drawable,
			 RegionPtr region,
			 BufferPtr dest_buffer,
			 BufferPtr src_buffer)
{
    struct dri2_buffer_priv *src_private = src_buffer->driverPrivate;
    struct dri2_buffer_priv *dst_private = dest_buffer->driverPrivate;
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
    DrawablePtr src_drawable;
    DrawablePtr dst_drawable;
    RegionPtr copy_clip;
    GCPtr gc;
    RADEONInfoPtr info = RADEONPTR(pScrn);
    Bool vsync;
    Bool translate = FALSE;
    int off_x = 0, off_y = 0;
    PixmapPtr dst_ppix;

    dst_ppix = dst_private->pixmap;
    src_drawable = &src_private->pixmap->drawable;
    dst_drawable = &dst_private->pixmap->drawable;

    if (src_private->attachment == DRI2BufferFrontLeft) {
#ifdef USE_DRI2_PRIME
	if (drawable->pScreen != pScreen) {
	    src_drawable = DRI2UpdatePrime(drawable, src_buffer);
	    if (!src_drawable)
		return;
	} else
#endif
	    src_drawable = drawable;
    }
    if (dst_private->attachment == DRI2BufferFrontLeft) {
#ifdef USE_DRI2_PRIME
	if (drawable->pScreen != pScreen) {
	    dst_drawable = DRI2UpdatePrime(drawable, dest_buffer);
	    if (!dst_drawable)
		return;
	    dst_ppix = (PixmapPtr)dst_drawable;
	    if (dst_drawable != drawable)
		translate = TRUE;
	} else
#endif
	    dst_drawable = drawable;
    }

    if (translate && drawable->type == DRAWABLE_WINDOW) {
	PixmapPtr pPix = GetDrawablePixmap(drawable);

	off_x = drawable->x - pPix->screen_x;
	off_y = drawable->y - pPix->screen_y;
    }
    gc = GetScratchGC(dst_drawable->depth, pScreen);
    copy_clip = REGION_CREATE(pScreen, NULL, 0);
    REGION_COPY(pScreen, copy_clip, region);

    if (translate) {
	REGION_TRANSLATE(pScreen, copy_clip, off_x, off_y);
    }

    (*gc->funcs->ChangeClip) (gc, CT_REGION, copy_clip, 0);
    ValidateGC(dst_drawable, gc);

    /* If this is a full buffer swap or frontbuffer flush, throttle on the
     * previous one
     */
    if (dst_private->attachment == DRI2BufferFrontLeft) {
	if (REGION_NUM_RECTS(region) == 1) {
	    BoxPtr extents = REGION_EXTENTS(pScreen, region);

	    if (extents->x1 == 0 && extents->y1 == 0 &&
		extents->x2 == drawable->width &&
		extents->y2 == drawable->height) {
		struct radeon_bo *bo = radeon_get_pixmap_bo(dst_ppix);

		if (bo)
		    radeon_bo_wait(bo);
	    }
	}
    }

    vsync = info->accel_state->vsync;

    /* Driver option "SwapbuffersWait" defines if we vsync DRI2 copy-swaps. */ 
    info->accel_state->vsync = info->swapBuffersWait;
    info->accel_state->force = TRUE;

    (*gc->ops->CopyArea)(src_drawable, dst_drawable, gc,
                         0, 0, drawable->width, drawable->height, off_x, off_y);

    info->accel_state->force = FALSE;
    info->accel_state->vsync = vsync;

    FreeScratchGC(gc);
}

void
radeon_dri2_copy_region(DrawablePtr pDraw, RegionPtr pRegion,
			 DRI2BufferPtr pDstBuffer, DRI2BufferPtr pSrcBuffer)
{
    return radeon_dri2_copy_region2(pDraw->pScreen, pDraw, pRegion,
				     pDstBuffer, pSrcBuffer);
}

#ifdef USE_DRI2_SCHEDULING

enum DRI2FrameEventType {
    DRI2_SWAP,
    DRI2_FLIP,
    DRI2_WAITMSC,
};

typedef struct _DRI2FrameEvent {
    XID drawable_id;
    ClientPtr client;
    enum DRI2FrameEventType type;
    int frame;
    xf86CrtcPtr crtc;

    /* for swaps & flips only */
    DRI2SwapEventPtr event_complete;
    void *event_data;
    DRI2BufferPtr front;
    DRI2BufferPtr back;

    Bool valid;

    struct xorg_list link;
} DRI2FrameEventRec, *DRI2FrameEventPtr;

typedef struct _DRI2ClientEvents {
    struct xorg_list   reference_list;
} DRI2ClientEventsRec, *DRI2ClientEventsPtr;

#if HAS_DEVPRIVATEKEYREC

static int DRI2InfoCnt;

static DevPrivateKeyRec DRI2ClientEventsPrivateKeyRec;
#define DRI2ClientEventsPrivateKey (&DRI2ClientEventsPrivateKeyRec)

#else

static int DRI2ClientEventsPrivateKeyIndex;
DevPrivateKey DRI2ClientEventsPrivateKey = &DRI2ClientEventsPrivateKeyIndex;

#endif /* HAS_DEVPRIVATEKEYREC */

#define GetDRI2ClientEvents(pClient)	((DRI2ClientEventsPtr) \
    dixLookupPrivate(&(pClient)->devPrivates, DRI2ClientEventsPrivateKey))

static int
ListAddDRI2ClientEvents(ClientPtr client, struct xorg_list *entry)
{
    DRI2ClientEventsPtr pClientPriv;
    pClientPriv = GetDRI2ClientEvents(client);

    if (!pClientPriv) {
        return BadAlloc;
    }

    xorg_list_add(entry, &pClientPriv->reference_list);
    return 0;
}

static void
ListDelDRI2ClientEvents(ClientPtr client, struct xorg_list *entry)
{
    DRI2ClientEventsPtr pClientPriv;
    pClientPriv = GetDRI2ClientEvents(client);

    if (!pClientPriv) {
        return;
    }
    xorg_list_del(entry);
}

static void
radeon_dri2_ref_buffer(BufferPtr buffer)
{
    struct dri2_buffer_priv *private = buffer->driverPrivate;
    private->refcnt++;
}

static void
radeon_dri2_unref_buffer(BufferPtr buffer)
{
    if (buffer) {
        struct dri2_buffer_priv *private = buffer->driverPrivate;
        radeon_dri2_destroy_buffer(&(private->pixmap->drawable), buffer);
    }
}

static void
radeon_dri2_client_state_changed(CallbackListPtr *ClientStateCallback, pointer data, pointer calldata)
{
    DRI2ClientEventsPtr pClientEventsPriv;
    DRI2FrameEventPtr ref;
    NewClientInfoRec *clientinfo = calldata;
    ClientPtr pClient = clientinfo->client;
    pClientEventsPriv = GetDRI2ClientEvents(pClient);

    switch (pClient->clientState) {
    case ClientStateInitial:
        xorg_list_init(&pClientEventsPriv->reference_list);
        break;
    case ClientStateRunning:
        break;

    case ClientStateRetained:
    case ClientStateGone:
        if (pClientEventsPriv) {
            xorg_list_for_each_entry(ref, &pClientEventsPriv->reference_list, link) {
                ref->valid = FALSE;
                radeon_dri2_unref_buffer(ref->front);
                radeon_dri2_unref_buffer(ref->back);
            }
        }
        break;
    default:
        break;
    }
}

static
xf86CrtcPtr radeon_dri2_drawable_crtc(DrawablePtr pDraw, Bool consider_disabled)
{
    ScreenPtr pScreen = pDraw->pScreen;
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
    xf86CrtcPtr crtc;

    crtc = radeon_pick_best_crtc(pScrn, consider_disabled,
				 pDraw->x,
				 pDraw->x + pDraw->width,
				 pDraw->y,
				 pDraw->y + pDraw->height);

    /* Make sure the CRTC is valid and this is the real front buffer */
    if (crtc != NULL && !crtc->rotatedData)
	return crtc;
    else
	return NULL;
}

static Bool
radeon_dri2_schedule_flip(ScrnInfoPtr scrn, ClientPtr client,
			  DrawablePtr draw, DRI2BufferPtr front,
			  DRI2BufferPtr back, DRI2SwapEventPtr func,
			  void *data, unsigned int target_msc)
{
    struct dri2_buffer_priv *back_priv;
    struct radeon_bo *bo;
    DRI2FrameEventPtr flip_info;
    /* Main crtc for this drawable shall finally deliver pageflip event. */
    xf86CrtcPtr crtc = radeon_dri2_drawable_crtc(draw, FALSE);
    int ref_crtc_hw_id = crtc ? drmmode_get_crtc_id(crtc) : -1;

    flip_info = calloc(1, sizeof(DRI2FrameEventRec));
    if (!flip_info)
	return FALSE;

    flip_info->drawable_id = draw->id;
    flip_info->client = client;
    flip_info->type = DRI2_SWAP;
    flip_info->event_complete = func;
    flip_info->event_data = data;
    flip_info->frame = target_msc;
    flip_info->crtc = crtc;

    xf86DrvMsgVerb(scrn->scrnIndex, X_INFO, RADEON_LOGLEVEL_DEBUG,
		   "%s:%d fevent[%p]\n", __func__, __LINE__, flip_info);

    /* Page flip the full screen buffer */
    back_priv = back->driverPrivate;
    bo = radeon_get_pixmap_bo(back_priv->pixmap);

    return radeon_do_pageflip(scrn, bo, flip_info, ref_crtc_hw_id);
}

static Bool
update_front(DrawablePtr draw, DRI2BufferPtr front)
{
    int r;
    PixmapPtr pixmap;
    RADEONInfoPtr info = RADEONPTR(xf86ScreenToScrn(draw->pScreen));
    struct dri2_buffer_priv *priv = front->driverPrivate;
    struct radeon_bo *bo;

    pixmap = get_drawable_pixmap(draw);
    pixmap->refcnt++;

    if (!info->use_glamor)
	exaMoveInPixmap(pixmap);
    bo = radeon_get_pixmap_bo(pixmap);
    r = radeon_gem_get_kernel_name(bo, &front->name);
    if (r) {
	(*draw->pScreen->DestroyPixmap)(pixmap);
	return FALSE;
    }
    (*draw->pScreen->DestroyPixmap)(priv->pixmap);
    front->pitch = pixmap->devKind;
    front->cpp = pixmap->drawable.bitsPerPixel / 8;
    priv->pixmap = pixmap;

    return TRUE;
}

static Bool
can_exchange(ScrnInfoPtr pScrn, DrawablePtr draw,
	     DRI2BufferPtr front, DRI2BufferPtr back)
{
    struct dri2_buffer_priv *front_priv = front->driverPrivate;
    struct dri2_buffer_priv *back_priv = back->driverPrivate;
    PixmapPtr front_pixmap;
    PixmapPtr back_pixmap = back_priv->pixmap;
    xf86CrtcConfigPtr xf86_config = XF86_CRTC_CONFIG_PTR(pScrn);
    int i;

    for (i = 0; i < xf86_config->num_crtc; i++) {
	xf86CrtcPtr crtc = xf86_config->crtc[i];
	if (crtc->enabled && crtc->rotatedData)
	    return FALSE;
    }

    if (!update_front(draw, front))
	return FALSE;

    front_pixmap = front_priv->pixmap;

    if (front_pixmap->drawable.width != back_pixmap->drawable.width)
	return FALSE;

    if (front_pixmap->drawable.height != back_pixmap->drawable.height)
	return FALSE;

    if (front_pixmap->drawable.bitsPerPixel != back_pixmap->drawable.bitsPerPixel)
	return FALSE;

    if (front_pixmap->devKind != back_pixmap->devKind)
	return FALSE;

    return TRUE;
}

static Bool
can_flip(ScrnInfoPtr pScrn, DrawablePtr draw,
	 DRI2BufferPtr front, DRI2BufferPtr back)
{
    return draw->type == DRAWABLE_WINDOW &&
	   RADEONPTR(pScrn)->allowPageFlip &&
	   pScrn->vtSema &&
	   DRI2CanFlip(draw) &&
	   can_exchange(pScrn, draw, front, back);
}

static void
radeon_dri2_exchange_buffers(DrawablePtr draw, DRI2BufferPtr front, DRI2BufferPtr back)
{
    struct dri2_buffer_priv *front_priv = front->driverPrivate;
    struct dri2_buffer_priv *back_priv = back->driverPrivate;
    struct radeon_bo *front_bo, *back_bo;
    ScreenPtr screen;
    RADEONInfoPtr info;
    RegionRec region;
    int tmp;

    region.extents.x1 = region.extents.y1 = 0;
    region.extents.x2 = front_priv->pixmap->drawable.width;
    region.extents.y2 = front_priv->pixmap->drawable.width;
    region.data = NULL;
    DamageRegionAppend(&front_priv->pixmap->drawable, &region);

    /* Swap BO names so DRI works */
    tmp = front->name;
    front->name = back->name;
    back->name = tmp;

    /* Swap pixmap bos */
    front_bo = radeon_get_pixmap_bo(front_priv->pixmap);
    back_bo = radeon_get_pixmap_bo(back_priv->pixmap);
    radeon_set_pixmap_bo(front_priv->pixmap, back_bo);
    radeon_set_pixmap_bo(back_priv->pixmap, front_bo);

    /* Do we need to update the Screen? */
    screen = draw->pScreen;
    info = RADEONPTR(xf86ScreenToScrn(screen));
    if (front_bo == info->front_bo) {
	radeon_bo_ref(back_bo);
	radeon_bo_unref(info->front_bo);
	info->front_bo = back_bo;
	radeon_set_pixmap_bo(screen->GetScreenPixmap(screen), back_bo);
    }

    radeon_glamor_exchange_buffers(front_priv->pixmap, back_priv->pixmap);

    DamageRegionProcessPending(&front_priv->pixmap->drawable);
}

void radeon_dri2_frame_event_handler(unsigned int frame, unsigned int tv_sec,
                                     unsigned int tv_usec, void *event_data)
{
    DRI2FrameEventPtr event = event_data;
    DrawablePtr drawable;
    ScreenPtr screen;
    ScrnInfoPtr scrn;
    int status;
    int swap_type;
    BoxRec box;
    RegionRec region;

    if (!event->valid)
	goto cleanup;

    status = dixLookupDrawable(&drawable, event->drawable_id, serverClient,
                               M_ANY, DixWriteAccess);
    if (status != Success)
        goto cleanup;
    if (!event->crtc)
	goto cleanup;
    frame += radeon_get_interpolated_vblanks(event->crtc);

    screen = drawable->pScreen;
    scrn = xf86ScreenToScrn(screen);

    switch (event->type) {
    case DRI2_FLIP:
	if (can_flip(scrn, drawable, event->front, event->back) &&
	    radeon_dri2_schedule_flip(scrn,
				      event->client,
				      drawable,
				      event->front,
				      event->back,
				      event->event_complete,
				      event->event_data,
				      event->frame)) {
	    radeon_dri2_exchange_buffers(drawable, event->front, event->back);
	    break;
	}
	/* else fall through to exchange/blit */
    case DRI2_SWAP:
	if (DRI2CanExchange(drawable) &&
	    can_exchange(scrn, drawable, event->front, event->back)) {
	    radeon_dri2_exchange_buffers(drawable, event->front, event->back);
	    swap_type = DRI2_EXCHANGE_COMPLETE;
	} else {
	    box.x1 = 0;
	    box.y1 = 0;
	    box.x2 = drawable->width;
	    box.y2 = drawable->height;
	    REGION_INIT(pScreen, &region, &box, 0);
	    radeon_dri2_copy_region(drawable, &region, event->front, event->back);
	    swap_type = DRI2_BLIT_COMPLETE;
	}

        DRI2SwapComplete(event->client, drawable, frame, tv_sec, tv_usec,
                swap_type, event->event_complete, event->event_data);

        break;
    case DRI2_WAITMSC:
        DRI2WaitMSCComplete(event->client, drawable, frame, tv_sec, tv_usec);
        break;
    default:
        /* Unknown type */
        xf86DrvMsg(scrn->scrnIndex, X_WARNING,
                "%s: unknown vblank event received\n", __func__);
        break;
    }

cleanup:
    if (event->valid) {
        radeon_dri2_unref_buffer(event->front);
        radeon_dri2_unref_buffer(event->back);
        ListDelDRI2ClientEvents(event->client, &event->link);
    }
    free(event);
}

drmVBlankSeqType radeon_populate_vbl_request_type(xf86CrtcPtr crtc)
{
    drmVBlankSeqType type = 0;
    int crtc_id = drmmode_get_crtc_id(crtc);

    if (crtc_id == 1)
        type |= DRM_VBLANK_SECONDARY;
    else if (crtc_id > 1)
#ifdef DRM_VBLANK_HIGH_CRTC_SHIFT
	type |= (crtc_id << DRM_VBLANK_HIGH_CRTC_SHIFT) &
		DRM_VBLANK_HIGH_CRTC_MASK;
#else
	ErrorF("radeon driver bug: %s called for CRTC %d > 1, but "
	       "DRM_VBLANK_HIGH_CRTC_MASK not defined at build time\n",
	       __func__, crtc_id);
#endif

    return type; 
}

/*
 * This function should be called on a disabled CRTC only (i.e., CRTC
 * in DPMS-off state). It will calculate the delay necessary to reach
 * target_msc from present time if the CRTC were running.
 */
static
CARD32 radeon_dri2_extrapolate_msc_delay(xf86CrtcPtr crtc, CARD64 *target_msc,
					 CARD64 divisor, CARD64 remainder)
{
    drmmode_crtc_private_ptr drmmode_crtc = crtc->driver_private;
    ScrnInfoPtr pScrn = crtc->scrn;
    RADEONInfoPtr info = RADEONPTR(pScrn);
    int nominal_frame_rate = drmmode_crtc->dpms_last_fps;
    CARD64 last_vblank_ust = drmmode_crtc->dpms_last_ust;
    uint32_t last_vblank_seq = drmmode_crtc->dpms_last_seq;
    int interpolated_vblanks = drmmode_crtc->interpolated_vblanks;
    int target_seq;
    CARD64 now, target_time, delta_t;
    int64_t d, delta_seq;
    int ret;
    CARD32 d_ms;

    if (!last_vblank_ust) {
	*target_msc = 0;
	return FALLBACK_SWAP_DELAY;
    }
    ret = drmmode_get_current_ust(info->dri2.drm_fd, &now);
    if (ret) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		   "%s cannot get current time\n", __func__);
	*target_msc = 0;
	return FALLBACK_SWAP_DELAY;
    }
    target_seq = (int)*target_msc - interpolated_vblanks;
    delta_seq = (int64_t)target_seq - (int64_t)last_vblank_seq;
    delta_seq *= 1000000;
    target_time = last_vblank_ust;
    target_time += delta_seq / nominal_frame_rate;
    d = target_time - now;
    if (d < 0) {
	/* we missed the event, adjust target_msc, do the divisor magic */
	CARD64 current_msc;
	current_msc = last_vblank_seq + interpolated_vblanks;
	delta_t = now - last_vblank_ust;
	delta_seq = delta_t * nominal_frame_rate;
	current_msc += delta_seq / 1000000;
	current_msc &= 0xffffffff;
	if (divisor == 0) {
	    *target_msc = current_msc;
	    d = 0;
	} else {
	    *target_msc = current_msc - (current_msc % divisor) + remainder;
	    if ((current_msc % divisor) >= remainder)
		*target_msc += divisor;
	    *target_msc &= 0xffffffff;
	    target_seq = (int)*target_msc - interpolated_vblanks;
	    delta_seq = (int64_t)target_seq - (int64_t)last_vblank_seq;
	    delta_seq *= 1000000;
	    target_time = last_vblank_ust;
	    target_time += delta_seq / nominal_frame_rate;
	    d = target_time - now;
	}
    }
    /*
     * convert delay to milliseconds and add margin to prevent the client
     * from coming back early (due to timer granularity and rounding
     * errors) and getting the same MSC it just got
     */
    d_ms = (CARD32)d / 1000;
    if ((CARD32)d - d_ms * 1000 > 0)
	d_ms += 2;
    else
	d_ms++;
    return d_ms;
}

/*
 * Get current frame count and frame count timestamp, based on drawable's
 * crtc.
 */
static int radeon_dri2_get_msc(DrawablePtr draw, CARD64 *ust, CARD64 *msc)
{
    ScreenPtr screen = draw->pScreen;
    ScrnInfoPtr scrn = xf86ScreenToScrn(screen);
    RADEONInfoPtr info = RADEONPTR(scrn);
    drmVBlank vbl;
    int ret;
    xf86CrtcPtr crtc = radeon_dri2_drawable_crtc(draw, TRUE);

    /* Drawable not displayed, make up a value */
    if (crtc == NULL) {
        *ust = 0;
        *msc = 0;
        return TRUE;
    }
    if (radeon_crtc_is_enabled(crtc)) {
	/* CRTC is running, read vblank counter and timestamp */
	vbl.request.type = DRM_VBLANK_RELATIVE;
	vbl.request.type |= radeon_populate_vbl_request_type(crtc);
	vbl.request.sequence = 0;

	ret = drmWaitVBlank(info->dri2.drm_fd, &vbl);
	if (ret) {
	    xf86DrvMsg(scrn->scrnIndex, X_WARNING,
		       "get vblank counter failed: %s\n", strerror(errno));
	    return FALSE;
	}

	*ust = ((CARD64)vbl.reply.tval_sec * 1000000) + vbl.reply.tval_usec;
	*msc = vbl.reply.sequence + radeon_get_interpolated_vblanks(crtc);
	*msc &= 0xffffffff;
    } else {
	/* CRTC is not running, extrapolate MSC and timestamp */
	drmmode_crtc_private_ptr drmmode_crtc = crtc->driver_private;
	CARD64 now, delta_t, delta_seq;

	if (!drmmode_crtc->dpms_last_ust)
	    return FALSE;
	ret = drmmode_get_current_ust(info->dri2.drm_fd, &now);
	if (ret) {
	    xf86DrvMsg(scrn->scrnIndex, X_ERROR,
		       "%s cannot get current time\n", __func__);
	    return FALSE;
	}
	delta_t = now - drmmode_crtc->dpms_last_ust;
	delta_seq = delta_t * drmmode_crtc->dpms_last_fps;
	delta_seq /= 1000000;
	*ust = drmmode_crtc->dpms_last_ust;
	delta_t = delta_seq * 1000000;
	delta_t /= drmmode_crtc->dpms_last_fps;
	*ust += delta_t;
	*msc = drmmode_crtc->dpms_last_seq;
	*msc += drmmode_crtc->interpolated_vblanks;
	*msc += delta_seq;
	*msc &= 0xffffffff;
    }
    return TRUE;
}

static
CARD32 radeon_dri2_deferred_event(OsTimerPtr timer, CARD32 now, pointer data)
{
    DRI2FrameEventPtr event_info = (DRI2FrameEventPtr)data;
    DrawablePtr drawable;
    ScreenPtr screen;
    ScrnInfoPtr scrn;
    RADEONInfoPtr info;
    int status;
    CARD64 drm_now;
    int ret;
    unsigned int tv_sec, tv_usec;
    CARD64 delta_t, delta_seq, frame;
    drmmode_crtc_private_ptr drmmode_crtc;
    TimerFree(timer);

    /*
     * This is emulated event, so its time is current time, which we
     * have to get in DRM-compatible form (which is a bit messy given
     * the information that we have at this point). Can't use now argument
     * because DRM event time may come from monotonic clock, while
     * DIX timer facility uses real-time clock.
     */
    if (!event_info->crtc) {
	ErrorF("%s no crtc\n", __func__);
	radeon_dri2_frame_event_handler(0, 0, 0, data);
	return 0;
    }
    status = dixLookupDrawable(&drawable, event_info->drawable_id, serverClient,
			       M_ANY, DixWriteAccess);
    if (status != Success) {
	ErrorF("%s cannot lookup drawable\n", __func__);
	radeon_dri2_frame_event_handler(0, 0, 0, data);
	return 0;
    }
    screen = drawable->pScreen;
    scrn = xf86ScreenToScrn(screen);
    info = RADEONPTR(scrn);
    ret = drmmode_get_current_ust(info->dri2.drm_fd, &drm_now);
    if (ret) {
	xf86DrvMsg(scrn->scrnIndex, X_ERROR,
		   "%s cannot get current time\n", __func__);
	radeon_dri2_frame_event_handler(0, 0, 0, data);
	return 0;
    }
    tv_sec = (unsigned int)(drm_now / 1000000);
    tv_usec = (unsigned int)(drm_now - (CARD64)tv_sec * 1000000);
    /*
     * calculate the frame number from current time
     * that would come from CRTC if it were running
     */
    drmmode_crtc = event_info->crtc->driver_private;
    delta_t = drm_now - (CARD64)drmmode_crtc->dpms_last_ust;
    delta_seq = delta_t * drmmode_crtc->dpms_last_fps;
    delta_seq /= 1000000;
    frame = (CARD64)drmmode_crtc->dpms_last_seq + delta_seq;
    frame &= 0xffffffff;
    radeon_dri2_frame_event_handler((unsigned int)frame, tv_sec, tv_usec, data);
    return 0;
}

static
void radeon_dri2_schedule_event(CARD32 delay, pointer arg)
{
    OsTimerPtr timer;

    timer = TimerSet(NULL, 0, delay, radeon_dri2_deferred_event, arg);
    if (delay == 0) {
	CARD32 now = GetTimeInMillis();
	radeon_dri2_deferred_event(timer, now, arg);
    }
}

/*
 * Request a DRM event when the requested conditions will be satisfied.
 *
 * We need to handle the event and ask the server to wake up the client when
 * we receive it.
 */
static int radeon_dri2_schedule_wait_msc(ClientPtr client, DrawablePtr draw,
                                         CARD64 target_msc, CARD64 divisor,
                                         CARD64 remainder)
{
    ScreenPtr screen = draw->pScreen;
    ScrnInfoPtr scrn = xf86ScreenToScrn(screen);
    RADEONInfoPtr info = RADEONPTR(scrn);
    DRI2FrameEventPtr wait_info = NULL;
    xf86CrtcPtr crtc = radeon_dri2_drawable_crtc(draw, TRUE);
    drmVBlank vbl;
    int ret;
    CARD64 current_msc;

    /* Truncate to match kernel interfaces; means occasional overflow
     * misses, but that's generally not a big deal */
    target_msc &= 0xffffffff;
    divisor &= 0xffffffff;
    remainder &= 0xffffffff;

    /* Drawable not visible, return immediately */
    if (crtc == NULL)
        goto out_complete;

    wait_info = calloc(1, sizeof(DRI2FrameEventRec));
    if (!wait_info)
        goto out_complete;

    wait_info->drawable_id = draw->id;
    wait_info->client = client;
    wait_info->type = DRI2_WAITMSC;
    wait_info->valid = TRUE;
    wait_info->crtc = crtc;

    if (ListAddDRI2ClientEvents(client, &wait_info->link)) {
        xf86DrvMsg(scrn->scrnIndex, X_WARNING,
                "add events to client private failed.\n");
        free(wait_info);
        wait_info = NULL;
        goto out_complete;
    }

    /*
     * CRTC is in DPMS off state, calculate wait time from current time,
     * target_msc and last vblank time/sequence when CRTC was turned off
     */
    if (!radeon_crtc_is_enabled(crtc)) {
	CARD32 delay;
	delay = radeon_dri2_extrapolate_msc_delay(crtc, &target_msc,
						  divisor, remainder);
	wait_info->frame = target_msc;
	radeon_dri2_schedule_event(delay, wait_info);
	DRI2BlockClient(client, draw);
	return TRUE;
    }

    /* Get current count */
    vbl.request.type = DRM_VBLANK_RELATIVE;
    vbl.request.type |= radeon_populate_vbl_request_type(crtc);
    vbl.request.sequence = 0;
    ret = drmWaitVBlank(info->dri2.drm_fd, &vbl);
    if (ret) {
        xf86DrvMsg(scrn->scrnIndex, X_WARNING,
                "get vblank counter failed: %s\n", strerror(errno));
        goto out_complete;
    }

    current_msc = vbl.reply.sequence + radeon_get_interpolated_vblanks(crtc);
    current_msc &= 0xffffffff;

    /*
     * If divisor is zero, or current_msc is smaller than target_msc,
     * we just need to make sure target_msc passes  before waking up the
     * client.
     */
    if (divisor == 0 || current_msc < target_msc) {
        /* If target_msc already reached or passed, set it to
         * current_msc to ensure we return a reasonable value back
         * to the caller. This keeps the client from continually
         * sending us MSC targets from the past by forcibly updating
         * their count on this call.
         */
        if (current_msc >= target_msc)
            target_msc = current_msc;
        vbl.request.type = DRM_VBLANK_ABSOLUTE | DRM_VBLANK_EVENT;
	vbl.request.type |= radeon_populate_vbl_request_type(crtc);
        vbl.request.sequence = target_msc;
	vbl.request.sequence -= radeon_get_interpolated_vblanks(crtc);
        vbl.request.signal = (unsigned long)wait_info;
        ret = drmWaitVBlank(info->dri2.drm_fd, &vbl);
        if (ret) {
            xf86DrvMsg(scrn->scrnIndex, X_WARNING,
                    "get vblank counter failed: %s\n", strerror(errno));
            goto out_complete;
        }

        wait_info->frame = vbl.reply.sequence;
	wait_info->frame += radeon_get_interpolated_vblanks(crtc);
        DRI2BlockClient(client, draw);
        return TRUE;
    }

    /*
     * If we get here, target_msc has already passed or we don't have one,
     * so we queue an event that will satisfy the divisor/remainder equation.
     */
    vbl.request.type = DRM_VBLANK_ABSOLUTE | DRM_VBLANK_EVENT;
    vbl.request.type |= radeon_populate_vbl_request_type(crtc);

    vbl.request.sequence = current_msc - (current_msc % divisor) +
        remainder;

    /*
     * If calculated remainder is larger than requested remainder,
     * it means we've passed the last point where
     * seq % divisor == remainder, so we need to wait for the next time
     * that will happen.
     */
    if ((current_msc % divisor) >= remainder)
        vbl.request.sequence += divisor;
    vbl.request.sequence -= radeon_get_interpolated_vblanks(crtc);

    vbl.request.signal = (unsigned long)wait_info;
    ret = drmWaitVBlank(info->dri2.drm_fd, &vbl);
    if (ret) {
        xf86DrvMsg(scrn->scrnIndex, X_WARNING,
                "get vblank counter failed: %s\n", strerror(errno));
        goto out_complete;
    }

    wait_info->frame = vbl.reply.sequence;
    wait_info->frame += radeon_get_interpolated_vblanks(crtc);
    DRI2BlockClient(client, draw);

    return TRUE;

out_complete:
    if (wait_info) {
        ListDelDRI2ClientEvents(wait_info->client, &wait_info->link);
        free(wait_info);
    }
    DRI2WaitMSCComplete(client, draw, target_msc, 0, 0);
    return TRUE;
}

void radeon_dri2_flip_event_handler(unsigned int frame, unsigned int tv_sec,
				    unsigned int tv_usec, void *event_data)
{
    DRI2FrameEventPtr flip = event_data;
    DrawablePtr drawable;
    ScreenPtr screen;
    ScrnInfoPtr scrn;
    int status;
    PixmapPtr pixmap;

    status = dixLookupDrawable(&drawable, flip->drawable_id, serverClient,
			       M_ANY, DixWriteAccess);
    if (status != Success) {
	free(flip);
	return;
    }
    if (!flip->crtc) {
	free(flip);
	return;
    }
    frame += radeon_get_interpolated_vblanks(flip->crtc);

    screen = drawable->pScreen;
    scrn = xf86ScreenToScrn(screen);

    pixmap = screen->GetScreenPixmap(screen);
    xf86DrvMsgVerb(scrn->scrnIndex, X_INFO, RADEON_LOGLEVEL_DEBUG,
		   "%s:%d fevent[%p] width %d pitch %d (/4 %d)\n",
		   __func__, __LINE__, flip, pixmap->drawable.width, pixmap->devKind, pixmap->devKind/4);

    /* We assume our flips arrive in order, so we don't check the frame */
    switch (flip->type) {
    case DRI2_SWAP:
	/* Check for too small vblank count of pageflip completion, taking wraparound
	 * into account. This usually means some defective kms pageflip completion,
	 * causing wrong (msc, ust) return values and possible visual corruption.
	 */
	if ((frame < flip->frame) && (flip->frame - frame < 5)) {
	    xf86DrvMsg(scrn->scrnIndex, X_WARNING,
		       "%s: Pageflip completion event has impossible msc %d < target_msc %d\n",
		        __func__, frame, flip->frame);
	    /* All-Zero values signal failure of (msc, ust) timestamping to client. */
	    frame = tv_sec = tv_usec = 0;
	}

	DRI2SwapComplete(flip->client, drawable, frame, tv_sec, tv_usec,
			 DRI2_FLIP_COMPLETE, flip->event_complete,
			 flip->event_data);
	break;
    default:
	xf86DrvMsg(scrn->scrnIndex, X_WARNING, "%s: unknown vblank event received\n", __func__);
	/* Unknown type */
	break;
    }

    free(flip);
}

/*
 * ScheduleSwap is responsible for requesting a DRM vblank event for the
 * appropriate frame.
 *
 * In the case of a blit (e.g. for a windowed swap) or buffer exchange,
 * the vblank requested can simply be the last queued swap frame + the swap
 * interval for the drawable.
 *
 * In the case of a page flip, we request an event for the last queued swap
 * frame + swap interval - 1, since we'll need to queue the flip for the frame
 * immediately following the received event.
 *
 * The client will be blocked if it tries to perform further GL commands
 * after queueing a swap, though in the Intel case after queueing a flip, the
 * client is free to queue more commands; they'll block in the kernel if
 * they access buffers busy with the flip.
 *
 * When the swap is complete, the driver should call into the server so it
 * can send any swap complete events that have been requested.
 */
static int radeon_dri2_schedule_swap(ClientPtr client, DrawablePtr draw,
                                     DRI2BufferPtr front, DRI2BufferPtr back,
                                     CARD64 *target_msc, CARD64 divisor,
                                     CARD64 remainder, DRI2SwapEventPtr func,
                                     void *data)
{
    ScreenPtr screen = draw->pScreen;
    ScrnInfoPtr scrn = xf86ScreenToScrn(screen);
    RADEONInfoPtr info = RADEONPTR(scrn);
    xf86CrtcPtr crtc = radeon_dri2_drawable_crtc(draw, TRUE);
    drmVBlank vbl;
    int ret, flip = 0;
    DRI2FrameEventPtr swap_info = NULL;
    enum DRI2FrameEventType swap_type = DRI2_SWAP;
    CARD64 current_msc;
    BoxRec box;
    RegionRec region;

    /* Truncate to match kernel interfaces; means occasional overflow
     * misses, but that's generally not a big deal */
    *target_msc &= 0xffffffff;
    divisor &= 0xffffffff;
    remainder &= 0xffffffff;

    /* radeon_dri2_frame_event_handler will get called some unknown time in the
     * future with these buffers.  Take a reference to ensure that they won't
     * get destroyed before then. 
     */
    radeon_dri2_ref_buffer(front);
    radeon_dri2_ref_buffer(back);

    /* either off-screen or CRTC not usable... just complete the swap */
    if (crtc == NULL)
        goto blit_fallback;

    swap_info = calloc(1, sizeof(DRI2FrameEventRec));
    if (!swap_info)
        goto blit_fallback;

    swap_info->drawable_id = draw->id;
    swap_info->client = client;
    swap_info->event_complete = func;
    swap_info->event_data = data;
    swap_info->front = front;
    swap_info->back = back;
    swap_info->valid = TRUE;
    swap_info->crtc = crtc;
    if (ListAddDRI2ClientEvents(client, &swap_info->link)) {
        xf86DrvMsg(scrn->scrnIndex, X_WARNING,
                "add events to client private failed.\n");
        free(swap_info);
        swap_info = NULL;
        goto blit_fallback;
    }

    /*
     * CRTC is in DPMS off state, fallback to blit, but calculate
     * wait time from current time, target_msc and last vblank
     * time/sequence when CRTC was turned off
     */
    if (!radeon_crtc_is_enabled(crtc)) {
	CARD32 delay;
	delay = radeon_dri2_extrapolate_msc_delay(crtc, target_msc,
						  divisor, remainder);
	swap_info->frame = *target_msc;
	radeon_dri2_schedule_event(delay, swap_info);
	return TRUE;
    }

    /* Get current count */
    vbl.request.type = DRM_VBLANK_RELATIVE;
    vbl.request.type |= radeon_populate_vbl_request_type(crtc);
    vbl.request.sequence = 0;
    ret = drmWaitVBlank(info->dri2.drm_fd, &vbl);
    if (ret) {
        xf86DrvMsg(scrn->scrnIndex, X_WARNING,
                "first get vblank counter failed: %s\n",
                strerror(errno));
	*target_msc = 0;
	radeon_dri2_schedule_event(FALLBACK_SWAP_DELAY, swap_info);
	return TRUE;
    }

    current_msc = vbl.reply.sequence + radeon_get_interpolated_vblanks(crtc);
    current_msc &= 0xffffffff;

    /* Flips need to be submitted one frame before */
    if (can_flip(scrn, draw, front, back)) {
	swap_type = DRI2_FLIP;
	flip = 1;
    }

    swap_info->type = swap_type;

    /* Correct target_msc by 'flip' if swap_type == DRI2_FLIP.
     * Do it early, so handling of different timing constraints
     * for divisor, remainder and msc vs. target_msc works.
     */
    if (*target_msc > 0)
        *target_msc -= flip;

    /*
     * If divisor is zero, or current_msc is smaller than target_msc
     * we just need to make sure target_msc passes before initiating
     * the swap.
     */
    if (divisor == 0 || current_msc < *target_msc) {
        vbl.request.type =  DRM_VBLANK_ABSOLUTE | DRM_VBLANK_EVENT;
        /* If non-pageflipping, but blitting/exchanging, we need to use
         * DRM_VBLANK_NEXTONMISS to avoid unreliable timestamping later
         * on.
         */
        if (flip == 0)
            vbl.request.type |= DRM_VBLANK_NEXTONMISS;
	vbl.request.type |= radeon_populate_vbl_request_type(crtc);

        /* If target_msc already reached or passed, set it to
         * current_msc to ensure we return a reasonable value back
         * to the caller. This makes swap_interval logic more robust.
         */
        if (current_msc >= *target_msc)
            *target_msc = current_msc;

        vbl.request.sequence = *target_msc;
	vbl.request.sequence -= radeon_get_interpolated_vblanks(crtc);
        vbl.request.signal = (unsigned long)swap_info;
        ret = drmWaitVBlank(info->dri2.drm_fd, &vbl);
        if (ret) {
            xf86DrvMsg(scrn->scrnIndex, X_WARNING,
                    "divisor 0 get vblank counter failed: %s\n",
                    strerror(errno));
	    *target_msc = 0;
	    radeon_dri2_schedule_event(FALLBACK_SWAP_DELAY, swap_info);
            return TRUE;
        }

        *target_msc = vbl.reply.sequence + flip;
	*target_msc += radeon_get_interpolated_vblanks(crtc);
        swap_info->frame = *target_msc;

        return TRUE;
    }

    /*
     * If we get here, target_msc has already passed or we don't have one,
     * and we need to queue an event that will satisfy the divisor/remainder
     * equation.
     */
    vbl.request.type = DRM_VBLANK_ABSOLUTE | DRM_VBLANK_EVENT;
    if (flip == 0)
        vbl.request.type |= DRM_VBLANK_NEXTONMISS;
    vbl.request.type |= radeon_populate_vbl_request_type(crtc);

    vbl.request.sequence = current_msc - (current_msc % divisor) +
        remainder;

    /*
     * If the calculated deadline vbl.request.sequence is smaller than
     * or equal to current_msc, it means we've passed the last point
     * when effective onset frame seq could satisfy
     * seq % divisor == remainder, so we need to wait for the next time
     * this will happen.

     * This comparison takes the 1 frame swap delay in pageflipping mode
     * into account, as well as a potential DRM_VBLANK_NEXTONMISS delay
     * if we are blitting/exchanging instead of flipping.
     */
    if (vbl.request.sequence <= current_msc)
        vbl.request.sequence += divisor;
    vbl.request.sequence -= radeon_get_interpolated_vblanks(crtc);

    /* Account for 1 frame extra pageflip delay if flip > 0 */
    vbl.request.sequence -= flip;

    vbl.request.signal = (unsigned long)swap_info;
    ret = drmWaitVBlank(info->dri2.drm_fd, &vbl);
    if (ret) {
        xf86DrvMsg(scrn->scrnIndex, X_WARNING,
                "final get vblank counter failed: %s\n",
                strerror(errno));
	*target_msc = 0;
	radeon_dri2_schedule_event(FALLBACK_SWAP_DELAY, swap_info);
	return TRUE;
    }

    /* Adjust returned value for 1 fame pageflip offset of flip > 0 */
    *target_msc = vbl.reply.sequence + flip;
    *target_msc += radeon_get_interpolated_vblanks(crtc);
    swap_info->frame = *target_msc;

    return TRUE;

blit_fallback:
    box.x1 = 0;
    box.y1 = 0;
    box.x2 = draw->width;
    box.y2 = draw->height;
    REGION_INIT(pScreen, &region, &box, 0);

    radeon_dri2_copy_region(draw, &region, front, back);

    DRI2SwapComplete(client, draw, 0, 0, 0, DRI2_BLIT_COMPLETE, func, data);
    if (swap_info) {
        ListDelDRI2ClientEvents(swap_info->client, &swap_info->link);
        free(swap_info);
    }

    radeon_dri2_unref_buffer(front);
    radeon_dri2_unref_buffer(back);

    *target_msc = 0; /* offscreen, so zero out target vblank count */
    return TRUE;
}

#endif /* USE_DRI2_SCHEDULING */


Bool
radeon_dri2_screen_init(ScreenPtr pScreen)
{
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
    RADEONInfoPtr info = RADEONPTR(pScrn);
    DRI2InfoRec dri2_info = { 0 };
#ifdef USE_DRI2_SCHEDULING
    const char *driverNames[2];
    Bool scheduling_works = TRUE;
#endif

    if (!info->dri2.available)
        return FALSE;

    info->dri2.device_name = drmGetDeviceNameFromFd(info->dri2.drm_fd);

    if ( (info->ChipFamily >= CHIP_FAMILY_TAHITI) ) {
        dri2_info.driverName = SI_DRIVER_NAME;
    } else if ( (info->ChipFamily >= CHIP_FAMILY_R600) ) {
        dri2_info.driverName = R600_DRIVER_NAME;
    } else if ( (info->ChipFamily >= CHIP_FAMILY_R300) ) {
        dri2_info.driverName = R300_DRIVER_NAME;
    } else if ( info->ChipFamily >= CHIP_FAMILY_R200 ) {
        dri2_info.driverName = R200_DRIVER_NAME;
    } else {
        dri2_info.driverName = RADEON_DRIVER_NAME;
    }
    dri2_info.fd = info->dri2.drm_fd;
    dri2_info.deviceName = info->dri2.device_name;
    dri2_info.version = DRI2INFOREC_VERSION;
    dri2_info.CreateBuffer = radeon_dri2_create_buffer;
    dri2_info.DestroyBuffer = radeon_dri2_destroy_buffer;
    dri2_info.CopyRegion = radeon_dri2_copy_region;

#ifdef USE_DRI2_SCHEDULING
    if (info->dri2.pKernelDRMVersion->version_minor < 4) {
	xf86DrvMsg(pScrn->scrnIndex, X_WARNING, "You need a newer kernel for "
		   "sync extension\n");
	scheduling_works = FALSE;
    }

    if (scheduling_works && info->drmmode.mode_res->count_crtcs > 2) {
#ifdef DRM_CAP_VBLANK_HIGH_CRTC
	uint64_t cap_value;

	if (drmGetCap(info->dri2.drm_fd, DRM_CAP_VBLANK_HIGH_CRTC, &cap_value)) {
	    xf86DrvMsg(pScrn->scrnIndex, X_WARNING, "You need a newer kernel "
		       "for VBLANKs on CRTC > 1\n");
	    scheduling_works = FALSE;
	} else if (!cap_value) {
	    xf86DrvMsg(pScrn->scrnIndex, X_WARNING, "Your kernel does not "
		       "handle VBLANKs on CRTC > 1\n");
	    scheduling_works = FALSE;
	}
#else
	xf86DrvMsg(pScrn->scrnIndex, X_WARNING, "You need to rebuild against a "
		   "newer libdrm to handle VBLANKs on CRTC > 1\n");
	scheduling_works = FALSE;
#endif
    }

    if (scheduling_works) {
        dri2_info.version = 4;
        dri2_info.ScheduleSwap = radeon_dri2_schedule_swap;
        dri2_info.GetMSC = radeon_dri2_get_msc;
        dri2_info.ScheduleWaitMSC = radeon_dri2_schedule_wait_msc;
        dri2_info.numDrivers = RADEON_ARRAY_SIZE(driverNames);
        dri2_info.driverNames = driverNames;
        driverNames[0] = driverNames[1] = dri2_info.driverName;

	if (DRI2InfoCnt == 0) {
#if HAS_DIXREGISTERPRIVATEKEY
	    if (!dixRegisterPrivateKey(DRI2ClientEventsPrivateKey,
				       PRIVATE_CLIENT, sizeof(DRI2ClientEventsRec))) {
		xf86DrvMsg(pScrn->scrnIndex, X_WARNING, "DRI2 registering "
			   "private key to client failed\n");
		return FALSE;
	    }
#else
	    if (!dixRequestPrivate(DRI2ClientEventsPrivateKey,
				   sizeof(DRI2ClientEventsRec))) {
		xf86DrvMsg(pScrn->scrnIndex, X_WARNING, "DRI2 requesting "
			   "private key to client failed\n");
		return FALSE;
	    }
#endif

	    AddCallback(&ClientStateCallback, radeon_dri2_client_state_changed, 0);
	}

	DRI2InfoCnt++;
    }
#endif

#if DRI2INFOREC_VERSION >= 9
    dri2_info.version = 9;
    dri2_info.CreateBuffer2 = radeon_dri2_create_buffer2;
    dri2_info.DestroyBuffer2 = radeon_dri2_destroy_buffer2;
    dri2_info.CopyRegion2 = radeon_dri2_copy_region2;
#endif

    info->dri2.enabled = DRI2ScreenInit(pScreen, &dri2_info);
    return info->dri2.enabled;
}

void radeon_dri2_close_screen(ScreenPtr pScreen)
{
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
    RADEONInfoPtr info = RADEONPTR(pScrn);

#ifdef USE_DRI2_SCHEDULING
    if (--DRI2InfoCnt == 0)
    	DeleteCallback(&ClientStateCallback, radeon_dri2_client_state_changed, 0);
#endif

    DRI2CloseScreen(pScreen);
    drmFree(info->dri2.device_name);
}

#endif /* DRI2 */

