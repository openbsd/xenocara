/*
 * Copyright © 2009 Red Hat, Inc.
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
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * Authors:
 *    Dave Airlie <airlied@redhat.com>
 *
 */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <errno.h>
#include <sys/ioctl.h>
/* Driver data structures */
#include "radeon.h"
#include "radeon_bo_helper.h"
#include "radeon_drm_queue.h"
#include "radeon_glamor.h"
#include "radeon_reg.h"
#include "radeon_probe.h"
#include "micmap.h"
#include "mipointrst.h"

#include "radeon_version.h"
#include "shadow.h"
#include <xf86Priv.h>

#include "atipciids.h"

#if HAVE_PRESENT_H
#include <present.h>
#endif

/* DPMS */
#ifdef HAVE_XEXTPROTO_71
#include <X11/extensions/dpmsconst.h>
#else
#define DPMS_SERVER
#include <X11/extensions/dpms.h>
#endif

#include <X11/extensions/damageproto.h>

#include "radeon_chipinfo_gen.h"

#include "radeon_bo_gem.h"
#include "radeon_cs_gem.h"
#include "radeon_vbo.h"

static DevScreenPrivateKeyRec radeon_client_private_key;
DevScreenPrivateKeyRec radeon_device_private_key;

extern SymTabRec RADEONChipsets[];
static Bool radeon_setup_kernel_mem(ScreenPtr pScreen);

const OptionInfoRec RADEONOptions_KMS[] = {
    { OPTION_ACCEL,          "Accel",            OPTV_BOOLEAN, {0}, FALSE },
    { OPTION_SW_CURSOR,      "SWcursor",         OPTV_BOOLEAN, {0}, FALSE },
    { OPTION_PAGE_FLIP,      "EnablePageFlip",   OPTV_BOOLEAN, {0}, FALSE },
    { OPTION_COLOR_TILING,   "ColorTiling",      OPTV_BOOLEAN, {0}, FALSE },
    { OPTION_COLOR_TILING_2D,"ColorTiling2D",    OPTV_BOOLEAN, {0}, FALSE },
    { OPTION_RENDER_ACCEL,   "RenderAccel",      OPTV_BOOLEAN, {0}, FALSE },
    { OPTION_SUBPIXEL_ORDER, "SubPixelOrder",    OPTV_ANYSTR,  {0}, FALSE },
#ifdef USE_GLAMOR
    { OPTION_ACCELMETHOD,    "AccelMethod",      OPTV_STRING,  {0}, FALSE },
    { OPTION_SHADOW_PRIMARY, "ShadowPrimary",    OPTV_BOOLEAN, {0}, FALSE },
#endif
    { OPTION_EXA_VSYNC,      "EXAVSync",         OPTV_BOOLEAN, {0}, FALSE },
    { OPTION_EXA_PIXMAPS,    "EXAPixmaps",	 OPTV_BOOLEAN,   {0}, FALSE },
    { OPTION_ZAPHOD_HEADS,   "ZaphodHeads",      OPTV_STRING,  {0}, FALSE },
    { OPTION_SWAPBUFFERS_WAIT,"SwapbuffersWait", OPTV_BOOLEAN, {0}, FALSE },
    { OPTION_DELETE_DP12,    "DeleteUnusedDP12Displays", OPTV_BOOLEAN, {0}, FALSE},
    { OPTION_DRI3,           "DRI3",             OPTV_BOOLEAN, {0}, FALSE },
    { OPTION_DRI,            "DRI",              OPTV_INTEGER, {0}, FALSE },
    { OPTION_TEAR_FREE,      "TearFree",         OPTV_BOOLEAN, {0}, FALSE },
    { -1,                    NULL,               OPTV_NONE,    {0}, FALSE }
};

const OptionInfoRec *RADEONOptionsWeak(void) { return RADEONOptions_KMS; }

void radeon_cs_flush_indirect(ScrnInfoPtr pScrn)
{
    RADEONInfoPtr  info = RADEONPTR(pScrn);
    struct radeon_accel_state *accel_state;
    int ret;

    info->gpu_flushed++;

#ifdef USE_GLAMOR
    if (info->use_glamor) {
	glamor_block_handler(pScrn->pScreen);
	return;
    }
#endif

    if (!info->cs->cdw)
	return;

    accel_state = info->accel_state;

    /* release the current VBO so we don't block on mapping it later */
    if (info->accel_state->vbo.vb_offset && info->accel_state->vbo.vb_bo) {
        radeon_vbo_put(pScrn, &info->accel_state->vbo);
        info->accel_state->vbo.vb_start_op = -1;
    }

    /* release the current VBO so we don't block on mapping it later */
    if (info->accel_state->cbuf.vb_bo) {
        radeon_vbo_put(pScrn, &info->accel_state->cbuf);
        info->accel_state->cbuf.vb_start_op = -1;
    }

    radeon_cs_emit(info->cs);
    radeon_cs_erase(info->cs);

    if (accel_state->use_vbos)
        radeon_vbo_flush_bos(pScrn);

    ret = radeon_cs_space_check_with_bo(info->cs,
					accel_state->vbo.vb_bo,
					RADEON_GEM_DOMAIN_GTT, 0);
    if (ret)
      ErrorF("space check failed in flush\n");

    if (info->reemit_current2d && info->state_2d.op)
        info->reemit_current2d(pScrn, info->state_2d.op);

    if (info->dri2.enabled) {
        info->accel_state->XInited3D = FALSE;
        info->accel_state->engineMode = EXA_ENGINEMODE_UNKNOWN;
    }

}

void radeon_ddx_cs_start(ScrnInfoPtr pScrn,
			 int n, const char *file,
			 const char *func, int line)
{
    RADEONInfoPtr  info = RADEONPTR(pScrn);

    if (info->cs->cdw + n > info->cs->ndw) {
	radeon_cs_flush_indirect(pScrn);

    }
    radeon_cs_begin(info->cs, n, file, func, line);
}


extern _X_EXPORT int gRADEONEntityIndex;

static int getRADEONEntityIndex(void)
{
    return gRADEONEntityIndex;
}


RADEONEntPtr RADEONEntPriv(ScrnInfoPtr pScrn)
{
    DevUnion     *pPriv;
    RADEONInfoPtr  info   = RADEONPTR(pScrn);
    pPriv = xf86GetEntityPrivate(info->pEnt->index,
                                 getRADEONEntityIndex());
    return pPriv->ptr;
}

/* Allocate our private RADEONInfoRec */
static Bool RADEONGetRec(ScrnInfoPtr pScrn)
{
    if (pScrn->driverPrivate) return TRUE;

    pScrn->driverPrivate = xnfcalloc(sizeof(RADEONInfoRec), 1);
    return TRUE;
}

/* Free our private RADEONInfoRec */
static void RADEONFreeRec(ScrnInfoPtr pScrn)
{
    DevUnion *pPriv;
    RADEONEntPtr pRADEONEnt;
    RADEONInfoPtr  info;
    EntityInfoPtr pEnt;

    if (!pScrn)
	return;

    pEnt = xf86GetEntityInfo(pScrn->entityList[pScrn->numEntities - 1]);
    pPriv = xf86GetEntityPrivate(pEnt->index, gRADEONEntityIndex);
    pRADEONEnt = pPriv->ptr;

    info = RADEONPTR(pScrn);
    if (info) {
	if (info->fbcon_pixmap)
	    pScrn->pScreen->DestroyPixmap(info->fbcon_pixmap);

	if (info->accel_state) {
	    free(info->accel_state);
	    info->accel_state = NULL;
	}

#ifdef USE_GLAMOR
	if (info->gbm)
	    gbm_device_destroy(info->gbm);
#endif

	pRADEONEnt->scrn[info->instance_id] = NULL;
	pRADEONEnt->num_scrns--;
	free(pScrn->driverPrivate);
	pScrn->driverPrivate = NULL;
    }

    if (pRADEONEnt->fd > 0) {
        DevUnion *pPriv;
        RADEONEntPtr pRADEONEnt;
        pPriv = xf86GetEntityPrivate(pScrn->entityList[0],
				     getRADEONEntityIndex());

        pRADEONEnt = pPriv->ptr;
        pRADEONEnt->fd_ref--;
        if (!pRADEONEnt->fd_ref) {
#ifdef XF86_PDEV_SERVER_FD
            if (!(pRADEONEnt->platform_dev &&
                    pRADEONEnt->platform_dev->flags & XF86_PDEV_SERVER_FD))
#endif
                drmClose(pRADEONEnt->fd);
            free(pPriv->ptr);
            pPriv->ptr = NULL;
        }
    }

    free(pEnt);
}

static void *
radeonShadowWindow(ScreenPtr screen, CARD32 row, CARD32 offset, int mode,
		   CARD32 *size, void *closure)
{
    ScrnInfoPtr pScrn = xf86ScreenToScrn(screen);
    RADEONInfoPtr  info   = RADEONPTR(pScrn);
    int stride;

    stride = (pScrn->displayWidth * pScrn->bitsPerPixel) / 8;
    *size = stride;

    return ((uint8_t *)info->front_buffer->bo.radeon->ptr + row * stride + offset);
}

static void
radeonUpdatePacked(ScreenPtr pScreen, shadowBufPtr pBuf)
{
    shadowUpdatePacked(pScreen, pBuf);
}

static Bool
callback_needs_flush(RADEONInfoPtr info, struct radeon_client_priv *client_priv)
{
    return (int)(client_priv->needs_flush - info->gpu_flushed) > 0;
}

static void
radeon_event_callback(CallbackListPtr *list,
		      pointer user_data, pointer call_data)
{
    EventInfoRec *eventinfo = call_data;
    ScrnInfoPtr pScrn = user_data;
    ScreenPtr pScreen = pScrn->pScreen;
    struct radeon_client_priv *client_priv =
	dixLookupScreenPrivate(&eventinfo->client->devPrivates,
			       &radeon_client_private_key, pScreen);
    struct radeon_client_priv *server_priv =
	dixLookupScreenPrivate(&serverClient->devPrivates,
			       &radeon_client_private_key, pScreen);
    RADEONInfoPtr info = RADEONPTR(pScrn);
    int i;

    if (callback_needs_flush(info, client_priv) ||
	callback_needs_flush(info, server_priv))
	return;

    /* Don't let gpu_flushed get too far ahead of needs_flush, in order
     * to prevent false positives in callback_needs_flush()
     */
    client_priv->needs_flush = info->gpu_flushed;
    server_priv->needs_flush = info->gpu_flushed;
    
    for (i = 0; i < eventinfo->count; i++) {
	if (eventinfo->events[i].u.u.type == info->callback_event_type) {
	    client_priv->needs_flush++;
	    server_priv->needs_flush++;
	    return;
	}
    }
}

static void
radeon_flush_callback(CallbackListPtr *list,
		      pointer user_data, pointer call_data)
{
    ScrnInfoPtr pScrn = user_data;
    ScreenPtr pScreen = pScrn->pScreen;
    ClientPtr client = call_data ? call_data : serverClient;
    struct radeon_client_priv *client_priv =
	dixLookupScreenPrivate(&client->devPrivates,
			       &radeon_client_private_key, pScreen);
    RADEONInfoPtr info = RADEONPTR(pScrn);

    if (pScrn->vtSema && callback_needs_flush(info, client_priv))
        radeon_cs_flush_indirect(pScrn);
}

static Bool RADEONCreateScreenResources_KMS(ScreenPtr pScreen)
{
    ExtensionEntry *damage_ext;
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
    RADEONInfoPtr  info   = RADEONPTR(pScrn);
    PixmapPtr pixmap;

    pScreen->CreateScreenResources = info->CreateScreenResources;
    if (!(*pScreen->CreateScreenResources)(pScreen))
	return FALSE;
    pScreen->CreateScreenResources = RADEONCreateScreenResources_KMS;

    /* Set the RandR primary output if Xorg hasn't */
    if (dixPrivateKeyRegistered(rrPrivKey)) {
	rrScrPrivPtr rrScrPriv = rrGetScrPriv(pScreen);

	if (!pScreen->isGPU && !rrScrPriv->primaryOutput) {
	    xf86CrtcConfigPtr xf86_config = XF86_CRTC_CONFIG_PTR(pScrn);

	    rrScrPriv->primaryOutput = xf86_config->output[0]->randr_output;
	    RROutputChanged(rrScrPriv->primaryOutput, FALSE);
	    rrScrPriv->layoutChanged = TRUE;
	}

	drmmode_uevent_init(pScrn, &info->drmmode);
    }

    if (!drmmode_set_desired_modes(pScrn, &info->drmmode, pScreen->isGPU))
	return FALSE;

    if (info->r600_shadow_fb) {
	pixmap = pScreen->GetScreenPixmap(pScreen);

	if (!shadowAdd(pScreen, pixmap, radeonUpdatePacked,
		       radeonShadowWindow, 0, NULL))
	    return FALSE;
    }

    if (info->dri2.enabled || info->use_glamor) {
	if (info->front_buffer) {
	    PixmapPtr pPix = pScreen->GetScreenPixmap(pScreen);
	    if (!radeon_set_pixmap_bo(pPix, info->front_buffer))
		return FALSE;

	    if (info->surf_man && !info->use_glamor)
		*radeon_get_pixmap_surface(pPix) = info->front_surface;
	}
    }

    if (info->use_glamor)
	radeon_glamor_create_screen_resources(pScreen);

    info->callback_event_type = -1;
    if (!pScreen->isGPU && (damage_ext = CheckExtension("DAMAGE"))) {
	info->callback_event_type = damage_ext->eventBase + XDamageNotify;

	if (!AddCallback(&FlushCallback, radeon_flush_callback, pScrn))
	    return FALSE;

	if (!AddCallback(&EventCallback, radeon_event_callback, pScrn)) {
	    DeleteCallback(&FlushCallback, radeon_flush_callback, pScrn);
	    return FALSE;
	}

	if (!dixRegisterScreenPrivateKey(&radeon_client_private_key, pScreen,
					 PRIVATE_CLIENT, sizeof(struct radeon_client_priv))) {
	    DeleteCallback(&FlushCallback, radeon_flush_callback, pScrn);
	    DeleteCallback(&EventCallback, radeon_event_callback, pScrn);
	    return FALSE;
	}
    }

    return TRUE;
}

static Bool
radeon_scanout_extents_intersect(xf86CrtcPtr xf86_crtc, BoxPtr extents)
{
    if (xf86_crtc->scrn->is_gpu) {
	extents->x1 -= xf86_crtc->x;
	extents->y1 -= xf86_crtc->y;
	extents->x2 -= xf86_crtc->x;
	extents->y2 -= xf86_crtc->y;
    } else {
	extents->x1 -= xf86_crtc->filter_width >> 1;
	extents->x2 += xf86_crtc->filter_width >> 1;
	extents->y1 -= xf86_crtc->filter_height >> 1;
	extents->y2 += xf86_crtc->filter_height >> 1;
	pixman_f_transform_bounds(&xf86_crtc->f_framebuffer_to_crtc, extents);
    }

    extents->x1 = max(extents->x1, 0);
    extents->y1 = max(extents->y1, 0);
    extents->x2 = min(extents->x2, xf86_crtc->mode.HDisplay);
    extents->y2 = min(extents->y2, xf86_crtc->mode.VDisplay);

    return (extents->x1 < extents->x2 && extents->y1 < extents->y2);
}

static RegionPtr
transform_region(RegionPtr region, struct pict_f_transform *transform,
		 int w, int h)
{
	BoxPtr boxes = RegionRects(region);
	int nboxes = RegionNumRects(region);
	xRectanglePtr rects = malloc(nboxes * sizeof(*rects));
	RegionPtr transformed;
	int nrects = 0;
	BoxRec box;
	int i;

	for (i = 0; i < nboxes; i++) {
		box.x1 = boxes[i].x1;
		box.x2 = boxes[i].x2;
		box.y1 = boxes[i].y1;
		box.y2 = boxes[i].y2;
		pixman_f_transform_bounds(transform, &box);

		box.x1 = max(box.x1, 0);
		box.y1 = max(box.y1, 0);
		box.x2 = min(box.x2, w);
		box.y2 = min(box.y2, h);
		if (box.x1 >= box.x2 || box.y1 >= box.y2)
			continue;

		rects[nrects].x = box.x1;
		rects[nrects].y = box.y1;
		rects[nrects].width = box.x2 - box.x1;
		rects[nrects].height = box.y2 - box.y1;
		nrects++;
	}

	transformed = RegionFromRects(nrects, rects, CT_UNSORTED);
	free(rects);
	return transformed;
}

static void
radeon_sync_scanout_pixmaps(xf86CrtcPtr xf86_crtc, RegionPtr new_region,
			    int scanout_id)
{
    drmmode_crtc_private_ptr drmmode_crtc = xf86_crtc->driver_private;
    DrawablePtr dst = &drmmode_crtc->scanout[scanout_id].pixmap->drawable;
    DrawablePtr src = &drmmode_crtc->scanout[scanout_id ^ 1].pixmap->drawable;
    RegionPtr last_region = &drmmode_crtc->scanout_last_region;
    ScrnInfoPtr scrn = xf86_crtc->scrn;
    ScreenPtr pScreen = scrn->pScreen;
    RADEONInfoPtr info = RADEONPTR(scrn);
    RegionRec remaining;
    RegionPtr sync_region = NULL;
    BoxRec extents;
    Bool force;
    GCPtr gc;

    if (RegionNil(last_region))
	return;

    RegionNull(&remaining);
    RegionSubtract(&remaining, last_region, new_region);
    if (RegionNil(&remaining))
	goto uninit;

    extents = *RegionExtents(&remaining);
    if (!radeon_scanout_extents_intersect(xf86_crtc, &extents))
	goto uninit;

    if (xf86_crtc->driverIsPerformingTransform) {
	sync_region = transform_region(&remaining,
				       &xf86_crtc->f_framebuffer_to_crtc,
				       dst->width, dst->height);
    } else {
	sync_region = RegionDuplicate(&remaining);
	RegionTranslate(sync_region, -xf86_crtc->x, -xf86_crtc->y);
    }

    force = info->accel_state->force;
    info->accel_state->force = TRUE;

    gc = GetScratchGC(dst->depth, pScreen);
    if (gc) {
	gc->funcs->ChangeClip(gc, CT_REGION, sync_region, 0);
	ValidateGC(dst, gc);
	sync_region = NULL;
	gc->ops->CopyArea(src, dst, gc, 0, 0, dst->width, dst->height, 0, 0);
	FreeScratchGC(gc);
    }

    info->accel_state->force = force;

 uninit:
    if (sync_region)
	RegionDestroy(sync_region);
    RegionUninit(&remaining);
}

static void
radeon_scanout_flip_abort(xf86CrtcPtr crtc, void *event_data)
{
    RADEONEntPtr pRADEONEnt = RADEONEntPriv(crtc->scrn);
    drmmode_crtc_private_ptr drmmode_crtc = crtc->driver_private;
    struct drmmode_fb *fb = event_data;

    drmmode_crtc->scanout_update_pending = 0;

    if (drmmode_crtc->flip_pending == fb) {
	drmmode_fb_reference(pRADEONEnt->fd, &drmmode_crtc->flip_pending,
			     NULL);
    }
}

static void
radeon_scanout_flip_handler(xf86CrtcPtr crtc, uint32_t msc, uint64_t usec,
				  void *event_data)
{
    RADEONEntPtr pRADEONEnt = RADEONEntPriv(crtc->scrn);
    drmmode_crtc_private_ptr drmmode_crtc = crtc->driver_private;
    struct drmmode_fb *fb = event_data;

    drmmode_fb_reference(pRADEONEnt->fd, &drmmode_crtc->fb, fb);
    radeon_scanout_flip_abort(crtc, event_data);
}


static RegionPtr
dirty_region(PixmapDirtyUpdatePtr dirty)
{
	RegionPtr damageregion = DamageRegion(dirty->damage);
	RegionPtr dstregion;

#ifdef HAS_DIRTYTRACKING_ROTATION
	if (dirty->rotation != RR_Rotate_0) {
		dstregion = transform_region(damageregion,
					     &dirty->f_inverse,
					     dirty->secondary_dst->drawable.width,
					     dirty->secondary_dst->drawable.height);
	} else
#endif
	{
	    RegionRec pixregion;

	    dstregion = RegionDuplicate(damageregion);
	    RegionTranslate(dstregion, -dirty->x, -dirty->y);
	    PixmapRegionInit(&pixregion, dirty->secondary_dst);
	    RegionIntersect(dstregion, dstregion, &pixregion);
	    RegionUninit(&pixregion);
	}

	return dstregion;
}

static void
redisplay_dirty(PixmapDirtyUpdatePtr dirty, RegionPtr region)
{
	ScrnInfoPtr src_scrn =
		xf86ScreenToScrn(radeon_dirty_src_drawable(dirty)->pScreen);

	if (RegionNil(region))
		goto out;

	if (dirty->secondary_dst->primary_pixmap)
	    DamageRegionAppend(&dirty->secondary_dst->drawable, region);

#ifdef HAS_DIRTYTRACKING_ROTATION
	PixmapSyncDirtyHelper(dirty);
#else
	PixmapSyncDirtyHelper(dirty, region);
#endif

	radeon_cs_flush_indirect(src_scrn);
	if (dirty->secondary_dst->primary_pixmap)
	    DamageRegionProcessPending(&dirty->secondary_dst->drawable);

out:
	DamageEmpty(dirty->damage);
}

static void
radeon_prime_scanout_update_abort(xf86CrtcPtr crtc, void *event_data)
{
    drmmode_crtc_private_ptr drmmode_crtc = crtc->driver_private;

    drmmode_crtc->scanout_update_pending = 0;
}

void
radeon_sync_shared_pixmap(PixmapDirtyUpdatePtr dirty)
{
    ScreenPtr primary_screen = radeon_dirty_primary(dirty);
    PixmapDirtyUpdatePtr ent;
    RegionPtr region;

    xorg_list_for_each_entry(ent, &primary_screen->pixmap_dirty_list, ent) {
	if (!radeon_dirty_src_equals(dirty, ent->secondary_dst))
	    continue;

	region = dirty_region(ent);
	redisplay_dirty(ent, region);
	RegionDestroy(region);
    }
}


#if HAS_SYNC_SHARED_PIXMAP

static Bool
primary_has_sync_shared_pixmap(ScrnInfoPtr scrn, PixmapDirtyUpdatePtr dirty)
{
    ScreenPtr primary_screen = radeon_dirty_primary(dirty);

    return !!primary_screen->SyncSharedPixmap;
}

static Bool
secondary_has_sync_shared_pixmap(ScrnInfoPtr scrn, PixmapDirtyUpdatePtr dirty)
{
    ScreenPtr secondary_screen = dirty->secondary_dst->drawable.pScreen;

    return !!secondary_screen->SyncSharedPixmap;
}

static void
call_sync_shared_pixmap(PixmapDirtyUpdatePtr dirty)
{
    ScreenPtr primary_screen = radeon_dirty_primary(dirty);

    primary_screen->SyncSharedPixmap(dirty);
}

#else /* !HAS_SYNC_SHARED_PIXMAP */

static Bool
primary_has_sync_shared_pixmap(ScrnInfoPtr scrn, PixmapDirtyUpdatePtr dirty)
{
    ScrnInfoPtr primary_scrn = xf86ScreenToScrn(radeon_dirty_primary(dirty));

    return primary_scrn->driverName == scrn->driverName;
}

static Bool
secondary_has_sync_shared_pixmap(ScrnInfoPtr scrn, PixmapDirtyUpdatePtr dirty)
{
    ScrnInfoPtr secondary_scrn = xf86ScreenToScrn(dirty->secondary_dst->drawable.pScreen);

    return secondary_scrn->driverName == scrn->driverName;
}

static void
call_sync_shared_pixmap(PixmapDirtyUpdatePtr dirty)
{
    radeon_sync_shared_pixmap(dirty);
}

#endif /* HAS_SYNC_SHARED_PIXMAPS */


static xf86CrtcPtr
radeon_prime_dirty_to_crtc(PixmapDirtyUpdatePtr dirty)
{
    ScreenPtr screen = dirty->secondary_dst->drawable.pScreen;
    ScrnInfoPtr scrn = xf86ScreenToScrn(screen);
    xf86CrtcConfigPtr xf86_config = XF86_CRTC_CONFIG_PTR(scrn);
    int c;

    /* Find the CRTC which is scanning out from this secondary pixmap */
    for (c = 0; c < xf86_config->num_crtc; c++) {
	xf86CrtcPtr xf86_crtc = xf86_config->crtc[c];
	drmmode_crtc_private_ptr drmmode_crtc = xf86_crtc->driver_private;

	if (radeon_dirty_src_equals(dirty, drmmode_crtc->prime_scanout_pixmap))
	    return xf86_crtc;
    }

    return NULL;
}

static Bool
radeon_prime_scanout_do_update(xf86CrtcPtr crtc, unsigned scanout_id)
{
    ScrnInfoPtr scrn = crtc->scrn;
    ScreenPtr screen = scrn->pScreen;
    drmmode_crtc_private_ptr drmmode_crtc = crtc->driver_private;
    PixmapDirtyUpdatePtr dirty;
    Bool ret = FALSE;

    xorg_list_for_each_entry(dirty, &screen->pixmap_dirty_list, ent) {
	if (radeon_dirty_src_equals(dirty, drmmode_crtc->prime_scanout_pixmap)) {
	    RegionPtr region;

	    if (primary_has_sync_shared_pixmap(scrn, dirty))
		call_sync_shared_pixmap(dirty);

	    region = dirty_region(dirty);
	    if (RegionNil(region))
		goto destroy;

	    if (drmmode_crtc->tear_free) {
		RegionTranslate(region, crtc->x, crtc->y);
		radeon_sync_scanout_pixmaps(crtc, region, scanout_id);
		radeon_cs_flush_indirect(scrn);
		RegionCopy(&drmmode_crtc->scanout_last_region, region);
		RegionTranslate(region, -crtc->x, -crtc->y);
		dirty->secondary_dst = drmmode_crtc->scanout[scanout_id].pixmap;
	    }

	    redisplay_dirty(dirty, region);
	    ret = TRUE;
	destroy:
	    RegionDestroy(region);
	    break;
	}
    }

    return ret;
}

static void
radeon_prime_scanout_update_handler(xf86CrtcPtr crtc, uint32_t frame, uint64_t usec,
				    void *event_data)
{
    drmmode_crtc_private_ptr drmmode_crtc = crtc->driver_private;

    radeon_prime_scanout_do_update(crtc, 0);
    drmmode_crtc->scanout_update_pending = 0;
}

static void
radeon_prime_scanout_update(PixmapDirtyUpdatePtr dirty)
{
    ScreenPtr screen = dirty->secondary_dst->drawable.pScreen;
    ScrnInfoPtr scrn = xf86ScreenToScrn(screen);
    RADEONEntPtr pRADEONEnt = RADEONEntPriv(scrn);
    xf86CrtcPtr xf86_crtc = radeon_prime_dirty_to_crtc(dirty);
    drmmode_crtc_private_ptr drmmode_crtc;
    uintptr_t drm_queue_seq;

    if (!xf86_crtc || !xf86_crtc->enabled)
	return;

    drmmode_crtc = xf86_crtc->driver_private;
    if (drmmode_crtc->scanout_update_pending ||
	!drmmode_crtc->scanout[drmmode_crtc->scanout_id].pixmap ||
	drmmode_crtc->dpms_mode != DPMSModeOn)
	return;

    drm_queue_seq = radeon_drm_queue_alloc(xf86_crtc,
					   RADEON_DRM_QUEUE_CLIENT_DEFAULT,
					   RADEON_DRM_QUEUE_ID_DEFAULT, NULL,
					   radeon_prime_scanout_update_handler,
					   radeon_prime_scanout_update_abort,
					   FALSE);
    if (drm_queue_seq == RADEON_DRM_QUEUE_ERROR) {
	xf86DrvMsg(scrn->scrnIndex, X_WARNING,
		   "radeon_drm_queue_alloc failed for PRIME update\n");
	radeon_prime_scanout_update_handler(xf86_crtc, 0, 0, NULL);
	return;
    }

    drmmode_crtc->scanout_update_pending = drm_queue_seq;

    if (!drmmode_wait_vblank(xf86_crtc, DRM_VBLANK_RELATIVE | DRM_VBLANK_EVENT,
			     1, drm_queue_seq, NULL, NULL)) {
	if (!(drmmode_crtc->scanout_status & DRMMODE_SCANOUT_VBLANK_FAILED)) {
	    xf86DrvMsg(scrn->scrnIndex, X_WARNING,
		       "drmmode_wait_vblank failed for PRIME update: %s\n",
		       strerror(errno));
	    drmmode_crtc->scanout_status |= DRMMODE_SCANOUT_VBLANK_FAILED;
	}

	drmmode_crtc->drmmode->event_context.vblank_handler(pRADEONEnt->fd,
							    0, 0, 0,
							    (void*)drm_queue_seq);
	drmmode_crtc->wait_flip_nesting_level++;
	radeon_drm_queue_handle_deferred(xf86_crtc);
	return;
    }

    if (drmmode_crtc->scanout_status ==
	(DRMMODE_SCANOUT_FLIP_FAILED | DRMMODE_SCANOUT_VBLANK_FAILED)) {
	/* The page flip and vblank ioctls failed before, but the vblank
	 * ioctl is working again, so we can try re-enabling TearFree
	 */
	xf86_crtc->funcs->set_mode_major(xf86_crtc, &xf86_crtc->mode,
					 xf86_crtc->rotation,
					 xf86_crtc->x, xf86_crtc->y);
    }

    drmmode_crtc->scanout_status &= ~DRMMODE_SCANOUT_VBLANK_FAILED;
}

static void
radeon_prime_scanout_flip(PixmapDirtyUpdatePtr ent)
{
    ScreenPtr screen = ent->secondary_dst->drawable.pScreen;
    ScrnInfoPtr scrn = xf86ScreenToScrn(screen);
    RADEONEntPtr pRADEONEnt = RADEONEntPriv(scrn);
    xf86CrtcPtr crtc = radeon_prime_dirty_to_crtc(ent);
    drmmode_crtc_private_ptr drmmode_crtc;
    uintptr_t drm_queue_seq;
    unsigned scanout_id;
    struct drmmode_fb *fb;

    if (!crtc || !crtc->enabled)
	return;

    drmmode_crtc = crtc->driver_private;
    scanout_id = drmmode_crtc->scanout_id ^ 1;
    if (drmmode_crtc->scanout_update_pending ||
	!drmmode_crtc->scanout[scanout_id].pixmap ||
	drmmode_crtc->dpms_mode != DPMSModeOn)
	return;

    if (!radeon_prime_scanout_do_update(crtc, scanout_id))
	return;

    fb = radeon_pixmap_get_fb(drmmode_crtc->scanout[scanout_id].pixmap);
    if (!fb) {
	xf86DrvMsg(scrn->scrnIndex, X_WARNING,
		   "Failed to get FB for PRIME flip.\n");
	return;
    }
	
    drm_queue_seq = radeon_drm_queue_alloc(crtc,
					   RADEON_DRM_QUEUE_CLIENT_DEFAULT,
					   RADEON_DRM_QUEUE_ID_DEFAULT, fb,
					   radeon_scanout_flip_handler,
					   radeon_scanout_flip_abort, TRUE);
    if (drm_queue_seq == RADEON_DRM_QUEUE_ERROR) {
	xf86DrvMsg(scrn->scrnIndex, X_WARNING,
		   "Allocating DRM event queue entry failed for PRIME flip.\n");
	return;
    }

    if (drmmode_page_flip_target_relative(pRADEONEnt, drmmode_crtc,
					  fb->handle, 0, drm_queue_seq, 1)
	!= 0) {
	if (!(drmmode_crtc->scanout_status & DRMMODE_SCANOUT_FLIP_FAILED)) {
	    xf86DrvMsg(scrn->scrnIndex, X_WARNING,
		       "flip queue failed in %s: %s, TearFree inactive\n",
		       __func__, strerror(errno));
	    drmmode_crtc->scanout_status |= DRMMODE_SCANOUT_FLIP_FAILED;
	}

	radeon_drm_abort_entry(drm_queue_seq);
	return;
    }

    if (drmmode_crtc->scanout_status & DRMMODE_SCANOUT_FLIP_FAILED) {
	xf86DrvMsg(scrn->scrnIndex, X_INFO, "TearFree active again\n");
	drmmode_crtc->scanout_status &= ~DRMMODE_SCANOUT_FLIP_FAILED;
    }

    drmmode_crtc->scanout_id = scanout_id;
    drmmode_crtc->scanout_update_pending = drm_queue_seq;
    drmmode_fb_reference(pRADEONEnt->fd, &drmmode_crtc->flip_pending, fb);
}

static void
radeon_dirty_update(ScrnInfoPtr scrn)
{
	ScreenPtr screen = scrn->pScreen;
	PixmapDirtyUpdatePtr ent;
	RegionPtr region;

	xorg_list_for_each_entry(ent, &screen->pixmap_dirty_list, ent) {
		if (screen->isGPU) {
			PixmapDirtyUpdatePtr region_ent = ent;

			if (primary_has_sync_shared_pixmap(scrn, ent)) {
				ScreenPtr primary_screen = radeon_dirty_primary(ent);

				xorg_list_for_each_entry(region_ent, &primary_screen->pixmap_dirty_list, ent) {
					if (radeon_dirty_src_equals(ent, region_ent->secondary_dst))
						break;
				}
			}

			region = dirty_region(region_ent);

			if (RegionNotEmpty(region)) {
				xf86CrtcPtr crtc = radeon_prime_dirty_to_crtc(ent);
				drmmode_crtc_private_ptr drmmode_crtc = NULL;

				if (crtc)
				    drmmode_crtc = crtc->driver_private;

				if (drmmode_crtc && drmmode_crtc->tear_free)
					radeon_prime_scanout_flip(ent);
				else
					radeon_prime_scanout_update(ent);
			} else {
				DamageEmpty(region_ent->damage);
			}

			RegionDestroy(region);
		} else {
			if (secondary_has_sync_shared_pixmap(scrn, ent))
				continue;

			region = dirty_region(ent);
			redisplay_dirty(ent, region);
			RegionDestroy(region);
		}
	}
}

static void
radeonSourceValidate(DrawablePtr draw, int x, int y, int w, int h,
		     unsigned int subWindowMode)
{
}



Bool
radeon_scanout_do_update(xf86CrtcPtr xf86_crtc, int scanout_id,
			 PixmapPtr src_pix, BoxRec extents)
{
    drmmode_crtc_private_ptr drmmode_crtc = xf86_crtc->driver_private;
    RegionRec region = { .extents = extents, .data = NULL };
    ScrnInfoPtr scrn = xf86_crtc->scrn;
    ScreenPtr pScreen = scrn->pScreen;
    RADEONInfoPtr info = RADEONPTR(scrn);
    DrawablePtr pDraw;
    Bool force;

    if (!xf86_crtc->enabled ||
	!drmmode_crtc->scanout[scanout_id].pixmap ||
	extents.x1 >= extents.x2 || extents.y1 >= extents.y2)
	return FALSE;

    pDraw = &drmmode_crtc->scanout[scanout_id].pixmap->drawable;
    if (!radeon_scanout_extents_intersect(xf86_crtc, &extents))
	return FALSE;

    if (drmmode_crtc->tear_free) {
	radeon_sync_scanout_pixmaps(xf86_crtc, &region, scanout_id);
	RegionCopy(&drmmode_crtc->scanout_last_region, &region);
    }

    force = info->accel_state->force;
    info->accel_state->force = TRUE;

    if (xf86_crtc->driverIsPerformingTransform) {
	SourceValidateProcPtr SourceValidate = pScreen->SourceValidate;
	PictFormatPtr format = PictureWindowFormat(pScreen->root);
	int error;
	PicturePtr src, dst;

	src = CreatePicture(None, &src_pix->drawable, format, 0L, NULL,
			    serverClient, &error);
	if (!src) {
	    ErrorF("Failed to create source picture for transformed scanout "
		   "update\n");
	    goto out;
	}

	dst = CreatePicture(None, pDraw, format, 0L, NULL, serverClient, &error);
	if (!dst) {
	    ErrorF("Failed to create destination picture for transformed scanout "
		   "update\n");
	    goto free_src;
	}

	error = SetPictureTransform(src, &xf86_crtc->crtc_to_framebuffer);
	if (error) {
	    ErrorF("SetPictureTransform failed for transformed scanout "
		   "update\n");
	    goto free_dst;
	}

	if (xf86_crtc->filter)
	    SetPicturePictFilter(src, xf86_crtc->filter, xf86_crtc->params,
				 xf86_crtc->nparams);

	pScreen->SourceValidate = radeonSourceValidate;
	CompositePicture(PictOpSrc,
			 src, NULL, dst,
			 extents.x1, extents.y1, 0, 0, extents.x1,
			 extents.y1, extents.x2 - extents.x1,
			 extents.y2 - extents.y1);
	pScreen->SourceValidate = SourceValidate;

 free_dst:
	FreePicture(dst, None);
 free_src:
	FreePicture(src, None);
    } else
 out:
    {
	GCPtr gc = GetScratchGC(pDraw->depth, pScreen);

	ValidateGC(pDraw, gc);
	(*gc->ops->CopyArea)(&src_pix->drawable, pDraw, gc,
			     xf86_crtc->x + extents.x1, xf86_crtc->y + extents.y1,
			     extents.x2 - extents.x1, extents.y2 - extents.y1,
			     extents.x1, extents.y1);
	FreeScratchGC(gc);
    }

    info->accel_state->force = force;

    return TRUE;
}

static void
radeon_scanout_update_abort(xf86CrtcPtr crtc, void *event_data)
{
    drmmode_crtc_private_ptr drmmode_crtc = event_data;

    drmmode_crtc->scanout_update_pending = 0;
}

static void
radeon_scanout_update_handler(xf86CrtcPtr crtc, uint32_t frame, uint64_t usec,
			      void *event_data)
{
    drmmode_crtc_private_ptr drmmode_crtc = event_data;
    ScreenPtr screen = crtc->scrn->pScreen;
    RegionPtr region = DamageRegion(drmmode_crtc->scanout_damage);

    if (crtc->enabled &&
	!drmmode_crtc->flip_pending &&
	drmmode_crtc->dpms_mode == DPMSModeOn) {
	if (radeon_scanout_do_update(crtc, drmmode_crtc->scanout_id,
				     screen->GetWindowPixmap(screen->root),
				     region->extents)) {
	    radeon_cs_flush_indirect(crtc->scrn);
	    RegionEmpty(region);
	}
    }

    radeon_scanout_update_abort(crtc, event_data);
}

static void
radeon_scanout_update(xf86CrtcPtr xf86_crtc)
{
    drmmode_crtc_private_ptr drmmode_crtc = xf86_crtc->driver_private;
    ScrnInfoPtr scrn = xf86_crtc->scrn;
    RADEONEntPtr pRADEONEnt = RADEONEntPriv(scrn);
    uintptr_t drm_queue_seq;
    DamagePtr pDamage;
    RegionPtr pRegion;
    BoxRec extents;

    if (!xf86_crtc->enabled ||
	drmmode_crtc->scanout_update_pending ||
	drmmode_crtc->flip_pending ||
	drmmode_crtc->dpms_mode != DPMSModeOn)
	return;

    pDamage = drmmode_crtc->scanout_damage;
    if (!pDamage)
	return;

    pRegion = DamageRegion(pDamage);
    if (!RegionNotEmpty(pRegion))
	return;

    extents = *RegionExtents(pRegion);
    if (!radeon_scanout_extents_intersect(xf86_crtc, &extents)) {
	RegionEmpty(pRegion);
	return;
    }

    drm_queue_seq = radeon_drm_queue_alloc(xf86_crtc,
					   RADEON_DRM_QUEUE_CLIENT_DEFAULT,
					   RADEON_DRM_QUEUE_ID_DEFAULT,
					   drmmode_crtc,
					   radeon_scanout_update_handler,
					   radeon_scanout_update_abort,
					   FALSE);
    if (drm_queue_seq == RADEON_DRM_QUEUE_ERROR) {
	xf86DrvMsg(scrn->scrnIndex, X_WARNING,
		   "radeon_drm_queue_alloc failed for scanout update\n");
	radeon_scanout_update_handler(xf86_crtc, 0, 0, drmmode_crtc);
	return;
    }

    drmmode_crtc->scanout_update_pending = drm_queue_seq;

    if (!drmmode_wait_vblank(xf86_crtc, DRM_VBLANK_RELATIVE | DRM_VBLANK_EVENT,
			     1, drm_queue_seq, NULL, NULL)) {
	if (!(drmmode_crtc->scanout_status & DRMMODE_SCANOUT_VBLANK_FAILED)) {
	    xf86DrvMsg(scrn->scrnIndex, X_WARNING,
		       "drmmode_wait_vblank failed for scanout update: %s\n",
		       strerror(errno));
	    drmmode_crtc->scanout_status |= DRMMODE_SCANOUT_VBLANK_FAILED;
	}

	drmmode_crtc->drmmode->event_context.vblank_handler(pRADEONEnt->fd,
							    0, 0, 0,
							    (void*)drm_queue_seq);
	drmmode_crtc->wait_flip_nesting_level++;
	radeon_drm_queue_handle_deferred(xf86_crtc);
	return;
    }

    if (drmmode_crtc->scanout_status ==
	(DRMMODE_SCANOUT_FLIP_FAILED | DRMMODE_SCANOUT_VBLANK_FAILED)) {
	/* The page flip and vblank ioctls failed before, but the vblank
	 * ioctl is working again, so we can try re-enabling TearFree
	 */
	xf86_crtc->funcs->set_mode_major(xf86_crtc, &xf86_crtc->mode,
					 xf86_crtc->rotation,
					 xf86_crtc->x, xf86_crtc->y);
    }

    drmmode_crtc->scanout_status &= ~DRMMODE_SCANOUT_VBLANK_FAILED;
}

static void
radeon_scanout_flip(ScreenPtr pScreen, RADEONInfoPtr info,
		    xf86CrtcPtr xf86_crtc)
{
    drmmode_crtc_private_ptr drmmode_crtc = xf86_crtc->driver_private;
    RegionPtr region = DamageRegion(drmmode_crtc->scanout_damage);
    ScrnInfoPtr scrn = xf86_crtc->scrn;
    RADEONEntPtr pRADEONEnt = RADEONEntPriv(scrn);
    uintptr_t drm_queue_seq;
    unsigned scanout_id;
    struct drmmode_fb *fb;

    if (drmmode_crtc->scanout_update_pending ||
	drmmode_crtc->flip_pending ||
	drmmode_crtc->dpms_mode != DPMSModeOn)
	return;

    scanout_id = drmmode_crtc->scanout_id ^ 1;
    if (!radeon_scanout_do_update(xf86_crtc, scanout_id,
				  pScreen->GetWindowPixmap(pScreen->root),
				  region->extents))
	return;

    radeon_cs_flush_indirect(scrn);
    RegionEmpty(region);

    fb = radeon_pixmap_get_fb(drmmode_crtc->scanout[scanout_id].pixmap);
    if (!fb) {
	xf86DrvMsg(scrn->scrnIndex, X_WARNING,
	       "Failed to get FB for scanout flip.\n");
	return;
    }

    drm_queue_seq = radeon_drm_queue_alloc(xf86_crtc,
					   RADEON_DRM_QUEUE_CLIENT_DEFAULT,
					   RADEON_DRM_QUEUE_ID_DEFAULT, fb,
					   radeon_scanout_flip_handler,
					   radeon_scanout_flip_abort, TRUE);
    if (drm_queue_seq == RADEON_DRM_QUEUE_ERROR) {
	xf86DrvMsg(scrn->scrnIndex, X_WARNING,
		   "Allocating DRM event queue entry failed.\n");
	return;
    }

    if (drmmode_page_flip_target_relative(pRADEONEnt, drmmode_crtc,
					  fb->handle, 0, drm_queue_seq, 1)
	!= 0) {
	if (!(drmmode_crtc->scanout_status & DRMMODE_SCANOUT_FLIP_FAILED)) {
	    xf86DrvMsg(scrn->scrnIndex, X_WARNING,
		       "flip queue failed in %s: %s, TearFree inactive\n",
		       __func__, strerror(errno));
	    drmmode_crtc->scanout_status |= DRMMODE_SCANOUT_FLIP_FAILED;
	}

	radeon_drm_abort_entry(drm_queue_seq);
	RegionCopy(DamageRegion(drmmode_crtc->scanout_damage),
		   &drmmode_crtc->scanout_last_region);
	RegionEmpty(&drmmode_crtc->scanout_last_region);
	radeon_scanout_update(xf86_crtc);
	drmmode_crtc_scanout_destroy(drmmode_crtc->drmmode,
				     &drmmode_crtc->scanout[scanout_id]);
	drmmode_crtc->tear_free = FALSE;
	return;
    }

    if (drmmode_crtc->scanout_status & DRMMODE_SCANOUT_FLIP_FAILED) {
	xf86DrvMsg(scrn->scrnIndex, X_INFO, "TearFree active again\n");
	drmmode_crtc->scanout_status &= ~DRMMODE_SCANOUT_FLIP_FAILED;
    }

    drmmode_crtc->scanout_id = scanout_id;
    drmmode_crtc->scanout_update_pending = drm_queue_seq;
    drmmode_fb_reference(pRADEONEnt->fd, &drmmode_crtc->flip_pending, fb);
}

static void RADEONBlockHandler_KMS(BLOCKHANDLER_ARGS_DECL)
{
    ScrnInfoPtr    pScrn   = xf86ScreenToScrn(pScreen);
    RADEONInfoPtr  info    = RADEONPTR(pScrn);
    xf86CrtcConfigPtr xf86_config = XF86_CRTC_CONFIG_PTR(pScrn);
    int c;

    pScreen->BlockHandler = info->BlockHandler;
    (*pScreen->BlockHandler) (BLOCKHANDLER_ARGS);
    pScreen->BlockHandler = RADEONBlockHandler_KMS;

    if (!xf86ScreenToScrn(radeon_primary_screen(pScreen))->vtSema)
	return;

    if (!pScreen->isGPU)
    {
	for (c = 0; c < xf86_config->num_crtc; c++) {
	    xf86CrtcPtr crtc = xf86_config->crtc[c];
	    drmmode_crtc_private_ptr drmmode_crtc = crtc->driver_private;

	    if (drmmode_crtc->rotate.pixmap)
		continue;

	    if (drmmode_crtc->tear_free)
		radeon_scanout_flip(pScreen, info, crtc);
	    else if (drmmode_crtc->scanout[drmmode_crtc->scanout_id].pixmap)
		radeon_scanout_update(crtc);
	}
    }

    radeon_cs_flush_indirect(pScrn);

    radeon_dirty_update(pScrn);
}

static Bool RADEONIsFastFBWorking(ScrnInfoPtr pScrn)
{
    RADEONEntPtr pRADEONEnt = RADEONEntPriv(pScrn);
    struct drm_radeon_info ginfo;
    int r;
    uint32_t tmp = 0;

    memset(&ginfo, 0, sizeof(ginfo));
    ginfo.request = RADEON_INFO_FASTFB_WORKING;
    ginfo.value = (uintptr_t)&tmp;
    r = drmCommandWriteRead(pRADEONEnt->fd, DRM_RADEON_INFO, &ginfo, sizeof(ginfo));
    if (r) {
	return FALSE;
    }
    if (tmp == 1)
	return TRUE;
    return FALSE;
}

static Bool RADEONIsFusionGARTWorking(ScrnInfoPtr pScrn)
{
    RADEONEntPtr pRADEONEnt = RADEONEntPriv(pScrn);
    struct drm_radeon_info ginfo;
    int r;
    uint32_t tmp;

    memset(&ginfo, 0, sizeof(ginfo));
    ginfo.request = RADEON_INFO_FUSION_GART_WORKING;
    ginfo.value = (uintptr_t)&tmp;
    r = drmCommandWriteRead(pRADEONEnt->fd, DRM_RADEON_INFO, &ginfo, sizeof(ginfo));
    if (r) {
	return FALSE;
    }
    if (tmp == 1)
	return TRUE;
    return FALSE;
}

static Bool RADEONIsAccelWorking(ScrnInfoPtr pScrn)
{
    RADEONEntPtr pRADEONEnt = RADEONEntPriv(pScrn);
    RADEONInfoPtr info = RADEONPTR(pScrn);
    struct drm_radeon_info ginfo;
    int r;
    uint32_t tmp;

    memset(&ginfo, 0, sizeof(ginfo));
    if (info->dri2.pKernelDRMVersion->version_minor >= 5)
	ginfo.request = RADEON_INFO_ACCEL_WORKING2;
    else
	ginfo.request = RADEON_INFO_ACCEL_WORKING;
    ginfo.value = (uintptr_t)&tmp;
    r = drmCommandWriteRead(pRADEONEnt->fd, DRM_RADEON_INFO, &ginfo, sizeof(ginfo));
    if (r) {
        /* If kernel is too old before 2.6.32 than assume accel is working */
        if (r == -EINVAL) {
            xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Kernel too old missing accel "
                       "information, assuming accel is working\n");
            return TRUE;
        }
        return FALSE;
    }
    if (info->ChipFamily == CHIP_FAMILY_HAWAII) {
        if (tmp == 2 || tmp == 3)
            return TRUE;
    } else if (tmp) {
        return TRUE;
    }
    return FALSE;
}

/* This is called by RADEONPreInit to set up the default visual */
static Bool RADEONPreInitVisual(ScrnInfoPtr pScrn)
{
    RADEONInfoPtr  info = RADEONPTR(pScrn);

    if (!xf86SetDepthBpp(pScrn, 0, 0, 0, Support32bppFb))
	return FALSE;

    switch (pScrn->depth) {
    case 8:
    case 15:
    case 16:
    case 24:
    case 30:
	break;

    default:
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		   "Given depth (%d) is not supported by %s driver\n",
		   pScrn->depth, RADEON_DRIVER_NAME);
	return FALSE;
    }

    xf86PrintDepthBpp(pScrn);

    info->pix24bpp                   = xf86GetBppFromDepth(pScrn,
							   pScrn->depth);
    info->pixel_bytes  = pScrn->bitsPerPixel / 8;

    if (info->pix24bpp == 24) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		   "Radeon does NOT support 24bpp\n");
	return FALSE;
    }

    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
	       "Pixel depth = %d bits stored in %d byte%s (%d bpp pixmaps)\n",
	       pScrn->depth,
	       info->pixel_bytes,
	       info->pixel_bytes > 1 ? "s" : "",
	       info->pix24bpp);

    if (!xf86SetDefaultVisual(pScrn, -1)) return FALSE;

    if (pScrn->depth > 8 && pScrn->defaultVisual != TrueColor) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		   "Default visual (%s) is not supported at depth %d\n",
		   xf86GetVisualName(pScrn->defaultVisual), pScrn->depth);
	return FALSE;
    }
    return TRUE;
}

/* This is called by RADEONPreInit to handle all color weight issues */
static Bool RADEONPreInitWeight(ScrnInfoPtr pScrn)
{
    RADEONInfoPtr  info = RADEONPTR(pScrn);

				/* Save flag for 6 bit DAC to use for
				   setting CRTC registers.  Otherwise use
				   an 8 bit DAC, even if xf86SetWeight sets
				   pScrn->rgbBits to some value other than
				   8. */
    info->dac6bits = FALSE;

    if (pScrn->depth > 8) {
	rgb  defaultWeight = { 0, 0, 0 };

	if (!xf86SetWeight(pScrn, defaultWeight, defaultWeight)) return FALSE;
    } else {
	pScrn->rgbBits = 8;
    }

    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
	       "Using %d bits per RGB (%d bit DAC)\n",
	       pScrn->rgbBits, info->dac6bits ? 6 : 8);

    return TRUE;
}

static Bool RADEONPreInitAccel_KMS(ScrnInfoPtr pScrn)
{
    RADEONInfoPtr  info = RADEONPTR(pScrn);

    if (!(info->accel_state = calloc(1, sizeof(struct radeon_accel_state)))) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "Unable to allocate accel_state rec!\n");
	return FALSE;
    }

    /* Check whether direct mapping is used for fast fb access*/
    if (RADEONIsFastFBWorking(pScrn)) {
	xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Direct mapping of fb aperture is enabled for fast fb access.\n");
	info->is_fast_fb = TRUE;
    }

    if (!xf86ReturnOptValBool(info->Options, OPTION_ACCEL, TRUE) ||
	(!RADEONIsAccelWorking(pScrn))) {
	xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		   "GPU accel disabled or not working, using shadowfb for KMS\n");
shadowfb:
	if (!xf86LoadSubModule(pScrn, "shadow"))
	    return FALSE;

	info->r600_shadow_fb = TRUE;
	return TRUE;
    }

#ifdef DRI2
    info->dri2.available = !!xf86LoadSubModule(pScrn, "dri2");
#endif

    if (radeon_glamor_pre_init(pScrn))
	return TRUE;

    if (info->ChipFamily >= CHIP_FAMILY_TAHITI) {
	goto shadowfb;
    } else if (info->ChipFamily == CHIP_FAMILY_PALM) {
	info->accel_state->allowHWDFS = RADEONIsFusionGARTWorking(pScrn);
    } else
	info->accel_state->allowHWDFS = TRUE;

    if ((info->ChipFamily == CHIP_FAMILY_RS100) ||
	(info->ChipFamily == CHIP_FAMILY_RS200) ||
	(info->ChipFamily == CHIP_FAMILY_RS300) ||
	(info->ChipFamily == CHIP_FAMILY_RS400) ||
	(info->ChipFamily == CHIP_FAMILY_RS480) ||
	(info->ChipFamily == CHIP_FAMILY_RS600) ||
	(info->ChipFamily == CHIP_FAMILY_RS690) ||
	(info->ChipFamily == CHIP_FAMILY_RS740))
	info->accel_state->has_tcl = FALSE;
    else {
	info->accel_state->has_tcl = TRUE;
    }

    {
	int errmaj = 0, errmin = 0;
	info->exaReq.majorversion = EXA_VERSION_MAJOR;
	info->exaReq.minorversion = EXA_VERSION_MINOR;
	if (!LoadSubModule(pScrn->module, "exa", NULL, NULL, NULL,
			   &info->exaReq, &errmaj, &errmin)) {
	    LoaderErrorMsg(NULL, "exa", errmaj, errmin);
	    return FALSE;
	}
    }

    return TRUE;
}

static Bool RADEONPreInitChipType_KMS(ScrnInfoPtr pScrn)
{
    RADEONInfoPtr  info   = RADEONPTR(pScrn);
    int i;

    info->Chipset = PCI_DEV_DEVICE_ID(info->PciInfo);
    pScrn->chipset = (char *)xf86TokenToString(RADEONChipsets, info->Chipset);
    if (!pScrn->chipset) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		   "ChipID 0x%04x is not recognized\n", info->Chipset);
	return FALSE;
    }

    if (info->Chipset < 0) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		   "Chipset \"%s\" is not recognized\n", pScrn->chipset);
	return FALSE;
    }
    xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
	       "Chipset: \"%s\" (ChipID = 0x%04x)\n",
	       pScrn->chipset,
	       info->Chipset);

    for (i = 0; i < sizeof(RADEONCards) / sizeof(RADEONCardInfo); i++) {
	if (info->Chipset == RADEONCards[i].pci_device_id) {
	    RADEONCardInfo *card = &RADEONCards[i];
	    info->ChipFamily = card->chip_family;
	    break;
	}
    }

#ifdef RENDER
    info->RenderAccel = xf86ReturnOptValBool(info->Options, OPTION_RENDER_ACCEL,
					     info->Chipset != PCI_CHIP_RN50_515E &&
					     info->Chipset != PCI_CHIP_RN50_5969);
#endif
    return TRUE;
}

static int radeon_get_drm_master_fd(ScrnInfoPtr pScrn)
{
    RADEONInfoPtr  info   = RADEONPTR(pScrn);
#ifdef XF86_PDEV_SERVER_FD
    RADEONEntPtr pRADEONEnt = RADEONEntPriv(pScrn);
#endif
    struct pci_device *dev = info->PciInfo;
    char *busid;
    int fd;

#ifdef XF86_PDEV_SERVER_FD
    if (pRADEONEnt->platform_dev) {
        fd = xf86_get_platform_device_int_attrib(pRADEONEnt->platform_dev,
                                                 ODEV_ATTRIB_FD, -1);
        if (fd != -1)
            return fd;
    }
#endif

    XNFasprintf(&busid, "pci:%04x:%02x:%02x.%d",
                dev->domain, dev->bus, dev->dev, dev->func);

    fd = drmOpen(NULL, busid);
    if (fd == -1)
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		   "[drm] Failed to open DRM device for %s: %s\n",
		   busid, strerror(errno));

    free(busid);
    return fd;
}

static Bool radeon_open_drm_master(ScrnInfoPtr pScrn)
{
    RADEONEntPtr pRADEONEnt = RADEONEntPriv(pScrn);
    drmSetVersion sv;
    int err;

    if (pRADEONEnt->fd) {
	xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		   " reusing fd for second head\n");
	pRADEONEnt->fd_ref++;
        return TRUE;
    }

    pRADEONEnt->fd = radeon_get_drm_master_fd(pScrn);
    if (pRADEONEnt->fd == -1)
	return FALSE;

    /* Check that what we opened was a master or a master-capable FD,
     * by setting the version of the interface we'll use to talk to it.
     * (see DRIOpenDRMMaster() in DRI1)
     */
    sv.drm_di_major = 1;
    sv.drm_di_minor = 1;
    sv.drm_dd_major = -1;
    sv.drm_dd_minor = -1;
    err = drmSetInterfaceVersion(pRADEONEnt->fd, &sv);
    if (err != 0) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		   "[drm] failed to set drm interface version.\n");
	drmClose(pRADEONEnt->fd);
	pRADEONEnt->fd = -1;

	return FALSE;
    }

    pRADEONEnt->fd_ref = 1;
    return TRUE;
}

static Bool r600_get_tile_config(ScrnInfoPtr pScrn)
{
    RADEONEntPtr pRADEONEnt = RADEONEntPriv(pScrn);
    RADEONInfoPtr  info   = RADEONPTR(pScrn);
    struct drm_radeon_info ginfo;
    int r;
    uint32_t tmp;

    if (info->ChipFamily < CHIP_FAMILY_R600)
	return FALSE;

    memset(&ginfo, 0, sizeof(ginfo));
    ginfo.request = RADEON_INFO_TILING_CONFIG;
    ginfo.value = (uintptr_t)&tmp;
    r = drmCommandWriteRead(pRADEONEnt->fd, DRM_RADEON_INFO, &ginfo, sizeof(ginfo));
    if (r)
	return FALSE;

    info->tile_config = tmp;
    info->r7xx_bank_op = 0;
    if (info->ChipFamily >= CHIP_FAMILY_CEDAR) {
	if (info->dri2.pKernelDRMVersion->version_minor >= 7) {
	    switch (info->tile_config & 0xf) {
	    case 0:
                info->num_channels = 1;
                break;
	    case 1:
                info->num_channels = 2;
                break;
	    case 2:
                info->num_channels = 4;
                break;
	    case 3:
                info->num_channels = 8;
                break;
	    default:
                return FALSE;
	    }

	    switch((info->tile_config & 0xf0) >> 4) {
	    case 0:
		info->num_banks = 4;
		break;
	    case 1:
		info->num_banks = 8;
		break;
	    case 2:
		info->num_banks = 16;
		break;
	    default:
		return FALSE;
	    }

	    switch ((info->tile_config & 0xf00) >> 8) {
	    case 0:
                info->group_bytes = 256;
                break;
	    case 1:
                info->group_bytes = 512;
                break;
	    default:
                return FALSE;
	    }
	} else
	    return FALSE;
    } else {
	switch((info->tile_config & 0xe) >> 1) {
	case 0:
	    info->num_channels = 1;
	    break;
	case 1:
	    info->num_channels = 2;
	    break;
	case 2:
	    info->num_channels = 4;
	    break;
	case 3:
	    info->num_channels = 8;
	    break;
	default:
	    return FALSE;
	}
	switch((info->tile_config & 0x30) >> 4) {
	case 0:
	    info->num_banks = 4;
	    break;
	case 1:
	    info->num_banks = 8;
	    break;
	default:
	    return FALSE;
	}
	switch((info->tile_config & 0xc0) >> 6) {
	case 0:
	    info->group_bytes = 256;
	    break;
	case 1:
	    info->group_bytes = 512;
	    break;
	default:
	    return FALSE;
	}
    }

    info->have_tiling_info = TRUE;
    return TRUE;
}

static void RADEONSetupCapabilities(ScrnInfoPtr pScrn)
{
    RADEONEntPtr pRADEONEnt = RADEONEntPriv(pScrn);
    RADEONInfoPtr  info = RADEONPTR(pScrn);
    uint64_t value;
    int ret;

    pScrn->capabilities = 0;

    /* PRIME offloading requires acceleration */
    if (info->r600_shadow_fb)
	return;

    ret = drmGetCap(pRADEONEnt->fd, DRM_CAP_PRIME, &value);
    if (ret == 0) {
	if (value & DRM_PRIME_CAP_EXPORT)
	    pScrn->capabilities |= RR_Capability_SourceOutput | RR_Capability_SourceOffload;
	if (value & DRM_PRIME_CAP_IMPORT) {
	    pScrn->capabilities |= RR_Capability_SinkOffload;
	    if (info->drmmode.count_crtcs)
		pScrn->capabilities |= RR_Capability_SinkOutput;
	}
    }
}

/* When the root window is created, initialize the screen contents from
 * console if -background none was specified on the command line
 */
static Bool RADEONCreateWindow_oneshot(WindowPtr pWin)
{
    ScreenPtr pScreen = pWin->drawable.pScreen;
    ScrnInfoPtr pScrn;
    RADEONInfoPtr info;
    Bool ret;

    if (pWin != pScreen->root)
	ErrorF("%s called for non-root window %p\n", __func__, pWin);

    pScrn = xf86ScreenToScrn(pScreen);
    info = RADEONPTR(pScrn);
    pScreen->CreateWindow = info->CreateWindow;
    ret = pScreen->CreateWindow(pWin);

    if (ret)
	drmmode_copy_fb(pScrn, &info->drmmode);

    return ret;
}

/* When the root window is mapped, set the initial modes */
void RADEONWindowExposures_oneshot(WindowPtr pWin, RegionPtr pRegion
#if XORG_VERSION_CURRENT < XORG_VERSION_NUMERIC(1,16,99,901,0)
				   , RegionPtr pBSRegion
#endif
				   )
{
    ScreenPtr pScreen = pWin->drawable.pScreen;
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
    RADEONInfoPtr info = RADEONPTR(pScrn);

    if (pWin != pScreen->root)
	ErrorF("%s called for non-root window %p\n", __func__, pWin);

    pScreen->WindowExposures = info->WindowExposures;
#if XORG_VERSION_CURRENT < XORG_VERSION_NUMERIC(1,16,99,901,0)
    pScreen->WindowExposures(pWin, pRegion, pBSRegion);
#else
    pScreen->WindowExposures(pWin, pRegion);
#endif

    radeon_finish(pScrn, info->front_buffer);
    drmmode_set_desired_modes(pScrn, &info->drmmode, TRUE);
}

Bool RADEONPreInit_KMS(ScrnInfoPtr pScrn, int flags)
{
    RADEONInfoPtr     info;
    RADEONEntPtr pRADEONEnt;
    MessageType from;
    Gamma  zeros = { 0.0, 0.0, 0.0 };
    uint32_t tiling = 0;
    int cpp;

    if (flags & PROBE_DETECT)
        return TRUE;

    xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, RADEON_LOGLEVEL_DEBUG,
		   "RADEONPreInit_KMS\n");
    if (pScrn->numEntities != 1) return FALSE;

    pRADEONEnt = xf86GetEntityPrivate(pScrn->entityList[0],
				      getRADEONEntityIndex())->ptr;
    if (pRADEONEnt->num_scrns == ARRAY_SIZE(pRADEONEnt->scrn)) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		   "Only up to %u Zaphod instances supported\n",
		   (unsigned)ARRAY_SIZE(pRADEONEnt->scrn));
	return FALSE;
    }
    
    if (!RADEONGetRec(pScrn)) return FALSE;

    info               = RADEONPTR(pScrn);

    info->instance_id = pRADEONEnt->num_scrns++;
    pRADEONEnt->scrn[info->instance_id] = pScrn;

    info->pEnt         = xf86GetEntityInfo(pScrn->entityList[pScrn->numEntities - 1]);
    if (info->pEnt->location.type != BUS_PCI
#ifdef XSERVER_PLATFORM_BUS
        && info->pEnt->location.type != BUS_PLATFORM
#endif
        )
        return FALSE;

    if (xf86IsEntityShared(pScrn->entityList[0]) &&
	info->instance_id == 0) {
	xf86SetPrimInitDone(pScrn->entityList[0]);
    }

    info->PciInfo = xf86GetPciInfoForEntity(info->pEnt->index);
    pScrn->monitor     = pScrn->confScreen->monitor;

    if (!RADEONPreInitVisual(pScrn))
	return FALSE;

    xf86CollectOptions(pScrn, NULL);
    if (!(info->Options = malloc(sizeof(RADEONOptions_KMS))))
	return FALSE;

    memcpy(info->Options, RADEONOptions_KMS, sizeof(RADEONOptions_KMS));
    xf86ProcessOptions(pScrn->scrnIndex, pScrn->options, info->Options);

    if (!RADEONPreInitWeight(pScrn))
	return FALSE;

    if (!RADEONPreInitChipType_KMS(pScrn))
        return FALSE;

    if (radeon_open_drm_master(pScrn) == FALSE) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "Kernel modesetting setup failed\n");
	return FALSE;
    }

    info->dri2.available = FALSE;
    info->dri2.enabled = FALSE;
    info->dri2.pKernelDRMVersion = drmGetVersion(pRADEONEnt->fd);
    if (!info->dri2.pKernelDRMVersion) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		   "RADEONDRIGetVersion failed to get the DRM version\n");
	return FALSE;
    }

    /* Get ScreenInit function */
    if (!xf86LoadSubModule(pScrn, "fb"))
	return FALSE;

    if (!RADEONPreInitAccel_KMS(pScrn))
	return FALSE;

    /* Depth 30 only supported since Linux 3.16 / kms driver minor version 39 */
    if (pScrn->depth == 30 && info->dri2.pKernelDRMVersion->version_minor < 39) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		   "Depth 30 is not supported. Kernel too old. Needs Linux 3.16+\n");
	return FALSE;
    }

    radeon_drm_queue_init(pScrn);

    info->allowColorTiling2D = FALSE;

    /* don't enable tiling if accel is not enabled */
    if (!info->r600_shadow_fb) {
	Bool colorTilingDefault =
	    info->ChipFamily >= CHIP_FAMILY_R300 &&
	    /* this check could be removed sometime after a big mesa release
	     * with proper bit, in the meantime you need to set tiling option in
	     * xorg configuration files
	     */
	    info->ChipFamily <= CHIP_FAMILY_MULLINS &&
	    !info->is_fast_fb;

	/* 2D color tiling */
	if (info->ChipFamily >= CHIP_FAMILY_R600) {
		info->allowColorTiling2D = xf86ReturnOptValBool(info->Options, OPTION_COLOR_TILING_2D,
                                                                info->ChipFamily <= CHIP_FAMILY_MULLINS);
	}

	if (info->ChipFamily >= CHIP_FAMILY_R600) {
	    /* set default group bytes, overridden by kernel info below */
	    info->group_bytes = 256;
	    info->have_tiling_info = FALSE;
	    if (info->dri2.pKernelDRMVersion->version_minor >= 6) {
		if (r600_get_tile_config(pScrn)) {
		    info->allowColorTiling = xf86ReturnOptValBool(info->Options,
								  OPTION_COLOR_TILING, colorTilingDefault);
		    if (!info->use_glamor) {
			/* need working DFS for tiling */
			if (info->ChipFamily == CHIP_FAMILY_PALM &&
			    !info->accel_state->allowHWDFS)
			    info->allowColorTiling = FALSE;
		    }
		} else
		    info->allowColorTiling = FALSE;
	    } else
		xf86DrvMsg(pScrn->scrnIndex, X_INFO,
			   "R6xx+ KMS Color Tiling requires radeon drm 2.6.0 or newer\n");
	} else
	    info->allowColorTiling = xf86ReturnOptValBool(info->Options,
							  OPTION_COLOR_TILING, colorTilingDefault);
    } else
	info->allowColorTiling = FALSE;

    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
	 "KMS Color Tiling: %sabled\n", info->allowColorTiling ? "en" : "dis");
    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
	 "KMS Color Tiling 2D: %sabled\n", info->allowColorTiling2D ? "en" : "dis");

#if USE_GLAMOR
    if (info->use_glamor) {
	info->shadow_primary = xf86ReturnOptValBool(info->Options,
						   OPTION_SHADOW_PRIMARY, FALSE);

	if (info->shadow_primary)
	    xf86DrvMsg(pScrn->scrnIndex, X_CONFIG, "ShadowPrimary enabled\n");
    }
#endif

    if (!info->r600_shadow_fb) {
	from = X_DEFAULT;

	info->tear_free = 2;
	if (xf86GetOptValBool(info->Options, OPTION_TEAR_FREE,
			      &info->tear_free))
	    from = X_CONFIG;
	xf86DrvMsg(pScrn->scrnIndex, from, "TearFree property default: %s\n",
		   info->tear_free == 2 ? "auto" : (info->tear_free ? "on" : "off"));
    }

    if (!pScrn->is_gpu) {
	if (info->dri2.pKernelDRMVersion->version_minor >= 8) {
	    info->allowPageFlip = xf86ReturnOptValBool(info->Options,
						       OPTION_PAGE_FLIP, TRUE);

	    if (info->shadow_primary) {
		xf86DrvMsg(pScrn->scrnIndex,
			   info->allowPageFlip ? X_WARNING : X_DEFAULT,
			   "KMS Pageflipping: disabled%s\n",
			   info->allowPageFlip ?
			   " because of ShadowPrimary" : "");
		info->allowPageFlip = FALSE;
	    } else {
		xf86DrvMsg(pScrn->scrnIndex, X_INFO,
			   "KMS Pageflipping: %sabled\n",
			   info->allowPageFlip ? "en" : "dis");
	    }
	}

	if (!info->use_glamor) {
	    info->swapBuffersWait =
		xf86ReturnOptValBool(info->Options, OPTION_SWAPBUFFERS_WAIT, TRUE);
	    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		       "SwapBuffers wait for vsync: %sabled\n",
		       info->swapBuffersWait ? "en" : "dis");
	}
    }

    if (xf86ReturnOptValBool(info->Options, OPTION_DELETE_DP12, FALSE)) {
        info->drmmode.delete_dp_12_displays = TRUE;
    }

    if (drmmode_pre_init(pScrn, &info->drmmode, pScrn->bitsPerPixel / 8) == FALSE) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "Kernel modesetting setup failed\n");
	return FALSE;
    }

    RADEONSetupCapabilities(pScrn);

    if (info->drmmode.count_crtcs == 1)
        pRADEONEnt->HasCRTC2 = FALSE;
    else
        pRADEONEnt->HasCRTC2 = TRUE;


    /* fix up cloning on rn50 cards
     * since they only have one crtc sometimes the xserver doesn't assign
     * a crtc to one of the outputs even though both outputs have common modes
     * which results in only one monitor being enabled.  Assign a crtc here so
     * that both outputs light up.
     */
    if (info->ChipFamily == CHIP_FAMILY_RV100 && !pRADEONEnt->HasCRTC2) {
	xf86CrtcConfigPtr xf86_config = XF86_CRTC_CONFIG_PTR(pScrn);
	int i;

	for (i = 0; i < xf86_config->num_output; i++) {
	    xf86OutputPtr output = xf86_config->output[i];

	    /* XXX: double check crtc mode */
	    if (output->probed_modes && !output->crtc)
		output->crtc = xf86_config->crtc[0];
	}
    }

    /* set cursor size */
    if (info->ChipFamily >= CHIP_FAMILY_BONAIRE) {
	info->cursor_w = CURSOR_WIDTH_CIK;
	info->cursor_h = CURSOR_HEIGHT_CIK;
    } else {
	info->cursor_w = CURSOR_WIDTH;
	info->cursor_h = CURSOR_HEIGHT;
    }

    {
	struct drm_radeon_gem_info mminfo;

	if (!drmCommandWriteRead(pRADEONEnt->fd, DRM_RADEON_GEM_INFO, &mminfo, sizeof(mminfo)))
	{
	    info->vram_size = mminfo.vram_visible;
	    info->gart_size = mminfo.gart_size;
	    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		       "mem size init: gart size :%llx vram size: s:%llx visible:%llx\n",
		       (unsigned long long)mminfo.gart_size,
		       (unsigned long long)mminfo.vram_size,
		       (unsigned long long)mminfo.vram_visible);
	}
    }

    if (!info->use_glamor) {
	info->exa_pixmaps = xf86ReturnOptValBool(info->Options,
						 OPTION_EXA_PIXMAPS,
						 (info->vram_size > (32 * 1024 * 1024) &&
						 info->RenderAccel &&
                                                 !info->is_fast_fb));
	if (info->exa_pixmaps)
	    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		       "EXA: Driver will allow EXA pixmaps in VRAM\n");
	else
	    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		       "EXA: Driver will not allow EXA pixmaps in VRAM\n");
    }

    /* no tiled scanout on r6xx+ yet */
    if (info->allowColorTiling) {
	if (info->ChipFamily >= CHIP_FAMILY_R600)
	    tiling |= RADEON_TILING_MICRO;
	else
	    tiling |= RADEON_TILING_MACRO;
    }
    cpp = pScrn->bitsPerPixel / 8;
    pScrn->displayWidth =
	RADEON_ALIGN(pScrn->virtualX, drmmode_get_pitch_align(pScrn, cpp, tiling));

    /* Set display resolution */
    xf86SetDpi(pScrn, 0, 0);

    if (!xf86SetGamma(pScrn, zeros)) return FALSE;

    if (!xf86ReturnOptValBool(info->Options, OPTION_SW_CURSOR, FALSE)) {
	if (!xf86LoadSubModule(pScrn, "ramdac")) return FALSE;
    }

    if (!pScrn->modes
#ifdef XSERVER_PLATFORM_BUS
        && !pScrn->is_gpu
#endif
        ) {
      xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "No modes.\n");
      return FALSE;
    }

    return TRUE;
}

static Bool RADEONCursorInit_KMS(ScreenPtr pScreen)
{
    ScrnInfoPtr    pScrn = xf86ScreenToScrn(pScreen);
    RADEONInfoPtr  info  = RADEONPTR(pScrn);

    xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, RADEON_LOGLEVEL_DEBUG,
		   "Initializing Cursor\n");

    /* Set Silken Mouse */
    xf86SetSilkenMouse(pScreen);

    /* Cursor setup */
    miDCInitialize(pScreen, xf86GetPointerScreenFuncs());

    if (info->allowPageFlip) {
	miPointerScreenPtr PointPriv =
	    dixLookupPrivate(&pScreen->devPrivates, miPointerScreenKey);

	if (!dixRegisterScreenPrivateKey(&radeon_device_private_key, pScreen,
					 PRIVATE_DEVICE,
					 sizeof(struct radeon_device_priv))) {
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "dixRegisterScreenPrivateKey failed\n");
	    return FALSE;
	}

	info->SpriteFuncs = PointPriv->spriteFuncs;
	PointPriv->spriteFuncs = &drmmode_sprite_funcs;
    }

    if (xf86ReturnOptValBool(info->Options, OPTION_SW_CURSOR, FALSE))
	return TRUE;

    if (!xf86_cursors_init(pScreen, info->cursor_w, info->cursor_h,
			   HARDWARE_CURSOR_TRUECOLOR_AT_8BPP |
			   HARDWARE_CURSOR_AND_SOURCE_WITH_MASK |
			   HARDWARE_CURSOR_SOURCE_MASK_INTERLEAVE_1 |
			   HARDWARE_CURSOR_UPDATE_UNHIDDEN |
			   HARDWARE_CURSOR_ARGB)) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "xf86_cursors_init failed\n");
	return FALSE;
    }

    return TRUE;
}

void
RADEONBlank(ScrnInfoPtr pScrn)
{
    xf86CrtcConfigPtr   xf86_config = XF86_CRTC_CONFIG_PTR(pScrn);
    xf86OutputPtr output;
    xf86CrtcPtr crtc;
    int o, c;

    for (c = 0; c < xf86_config->num_crtc; c++) {
       crtc = xf86_config->crtc[c];
       for (o = 0; o < xf86_config->num_output; o++) {
           output = xf86_config->output[o];
           if (output->crtc != crtc)
               continue;

           output->funcs->dpms(output, DPMSModeOff);
       }
      crtc->funcs->dpms(crtc, DPMSModeOff);
    }
}

void
RADEONUnblank(ScrnInfoPtr pScrn)
{
    xf86CrtcConfigPtr   xf86_config = XF86_CRTC_CONFIG_PTR(pScrn);
    xf86OutputPtr output;
    xf86CrtcPtr crtc;
    int o, c;
    for (c = 0; c < xf86_config->num_crtc; c++) {
       crtc = xf86_config->crtc[c];
       if(!crtc->enabled)
              continue;
       crtc->funcs->dpms(crtc, DPMSModeOn);
       for (o = 0; o < xf86_config->num_output; o++) {
           output = xf86_config->output[o];
           if (output->crtc != crtc)
               continue;
           output->funcs->dpms(output, DPMSModeOn);
       }
    }
}


static Bool RADEONSaveScreen_KMS(ScreenPtr pScreen, int mode)
{
    ScrnInfoPtr  pScrn = xf86ScreenToScrn(pScreen);
    Bool         unblank;

    xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, RADEON_LOGLEVEL_DEBUG,
		   "RADEONSaveScreen(%d)\n", mode);

    unblank = xf86IsUnblank(mode);
    if (unblank) SetTimeSinceLastInputEvent();

    if (pScrn && pScrn->vtSema) {
	if (unblank)
	    RADEONUnblank(pScrn);
	else
	    RADEONBlank(pScrn);
    }
    return TRUE;
}

static Bool radeon_set_drm_master(ScrnInfoPtr pScrn)
{
    RADEONEntPtr pRADEONEnt = RADEONEntPriv(pScrn);
    int err;

#ifdef XF86_PDEV_SERVER_FD
    if (pRADEONEnt->platform_dev &&
            (pRADEONEnt->platform_dev->flags & XF86_PDEV_SERVER_FD))
        return TRUE;
#endif

    err = drmSetMaster(pRADEONEnt->fd);
    if (err)
        ErrorF("Unable to retrieve master\n");

    return err == 0;
}

static void radeon_drop_drm_master(ScrnInfoPtr pScrn)
{
    RADEONEntPtr pRADEONEnt = RADEONEntPriv(pScrn);

#ifdef XF86_PDEV_SERVER_FD
    if (pRADEONEnt->platform_dev &&
            (pRADEONEnt->platform_dev->flags & XF86_PDEV_SERVER_FD))
        return;
#endif

    drmDropMaster(pRADEONEnt->fd);
}

/* Called at the end of each server generation.  Restore the original
 * text mode, unmap video memory, and unwrap and call the saved
 * CloseScreen function.
 */
static Bool RADEONCloseScreen_KMS(ScreenPtr pScreen)
{
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
    RADEONInfoPtr  info  = RADEONPTR(pScrn);
    RADEONEntPtr pRADEONEnt = RADEONEntPriv(pScrn);

    xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, RADEON_LOGLEVEL_DEBUG,
		   "RADEONCloseScreen\n");

    /* Clear mask of assigned crtc's in this generation */
    pRADEONEnt->assigned_crtcs = 0;

    drmmode_uevent_fini(pScrn, &info->drmmode);
    radeon_drm_queue_close(pScrn);
    radeon_cs_flush_indirect(pScrn);

    if (info->callback_event_type != -1) {
	DeleteCallback(&EventCallback, radeon_event_callback, pScrn);
	DeleteCallback(&FlushCallback, radeon_flush_callback, pScrn);
    }

    if (info->accel_state->exa) {
	exaDriverFini(pScreen);
	free(info->accel_state->exa);
	info->accel_state->exa = NULL;
    }

    radeon_sync_close(pScreen);

    if (info->accel_state->use_vbos)
        radeon_vbo_free_lists(pScrn);

    radeon_drop_drm_master(pScrn);

    drmmode_fini(pScrn, &info->drmmode);
    if (info->dri2.enabled)
	radeon_dri2_close_screen(pScreen);

    radeon_glamor_fini(pScreen);

    pScrn->vtSema = FALSE;
    xf86ClearPrimInitDone(info->pEnt->index);

    if (info->allowPageFlip) {
	miPointerScreenPtr PointPriv =
	    dixLookupPrivate(&pScreen->devPrivates, miPointerScreenKey);

	if (PointPriv->spriteFuncs == &drmmode_sprite_funcs)
	    PointPriv->spriteFuncs = info->SpriteFuncs;
    }

    pScreen->BlockHandler = info->BlockHandler;
    pScreen->CloseScreen = info->CloseScreen;
    return pScreen->CloseScreen(pScreen);
}


void RADEONFreeScreen_KMS(ScrnInfoPtr pScrn)
{
    xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, RADEON_LOGLEVEL_DEBUG,
		   "RADEONFreeScreen\n");

    RADEONFreeRec(pScrn);
}

Bool RADEONScreenInit_KMS(ScreenPtr pScreen, int argc, char **argv)
{
    ScrnInfoPtr    pScrn = xf86ScreenToScrn(pScreen);
    RADEONEntPtr pRADEONEnt = RADEONEntPriv(pScrn);
    RADEONInfoPtr  info  = RADEONPTR(pScrn);
    int            subPixelOrder = SubPixelUnknown;
    MessageType from;
    Bool value;
    int driLevel;
    const char *s;
    void *front_ptr;

    pScrn->fbOffset = 0;

    miClearVisualTypes();
    if (!miSetVisualTypes(pScrn->depth,
			  miGetDefaultVisualMask(pScrn->depth),
			  pScrn->rgbBits,
			  pScrn->defaultVisual)) return FALSE;
    miSetPixmapDepths ();

    if (!radeon_set_drm_master(pScrn))
        return FALSE;

    info->directRenderingEnabled = FALSE;
    if (info->r600_shadow_fb == FALSE)
        info->directRenderingEnabled = radeon_dri2_screen_init(pScreen);

    if (info->ChipFamily >= CHIP_FAMILY_R600) {
	info->surf_man = radeon_surface_manager_new(pRADEONEnt->fd);

	if (!info->surf_man) {
	    xf86DrvMsg(pScreen->myNum, X_ERROR,
		       "Failed to initialize surface manager\n");
	    return FALSE;
	}
    }

    if (!info->bufmgr)
        info->bufmgr = radeon_bo_manager_gem_ctor(pRADEONEnt->fd);
    if (!info->bufmgr) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		   "failed to initialise GEM buffer manager");
	return FALSE;
    }
    drmmode_set_bufmgr(pScrn, &info->drmmode, info->bufmgr);

    if (!info->csm)
        info->csm = radeon_cs_manager_gem_ctor(pRADEONEnt->fd);
    if (!info->csm) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		   "failed to initialise command submission manager");
	return FALSE;
    }

    if (!info->cs)
        info->cs = radeon_cs_create(info->csm, RADEON_BUFFER_SIZE/4);
    if (!info->cs) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		   "failed to initialise command submission buffer");
	return FALSE;
    }

    radeon_cs_set_limit(info->cs, RADEON_GEM_DOMAIN_GTT, info->gart_size);
    radeon_cs_space_set_flush(info->cs, (void(*)(void *))radeon_cs_flush_indirect, pScrn); 

    if (!radeon_setup_kernel_mem(pScreen)) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "radeon_setup_kernel_mem failed\n");
	return FALSE;
    }

    if (!(info->front_buffer->flags & RADEON_BO_FLAGS_GBM))
	front_ptr = info->front_buffer->bo.radeon->ptr;
    else
	front_ptr = NULL;

    if (info->r600_shadow_fb) {
	info->fb_shadow = calloc(1,
				 pScrn->displayWidth * pScrn->virtualY *
				 ((pScrn->bitsPerPixel + 7) >> 3));
	if (!info->fb_shadow) {
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                       "Failed to allocate shadow framebuffer\n");
	    return FALSE;
	} else {
	    if (!fbScreenInit(pScreen, info->fb_shadow,
			      pScrn->virtualX, pScrn->virtualY,
			      pScrn->xDpi, pScrn->yDpi, pScrn->displayWidth,
			      pScrn->bitsPerPixel))
		return FALSE;
	}
    }

    if (info->r600_shadow_fb == FALSE) {
	/* Init fb layer */
	if (!fbScreenInit(pScreen, front_ptr,
			  pScrn->virtualX, pScrn->virtualY,
			  pScrn->xDpi, pScrn->yDpi, pScrn->displayWidth,
			  pScrn->bitsPerPixel))
	    return FALSE;
    }

    xf86SetBlackWhitePixels(pScreen);

    if (pScrn->bitsPerPixel > 8) {
	VisualPtr  visual;

	visual = pScreen->visuals + pScreen->numVisuals;
	while (--visual >= pScreen->visuals) {
	    if ((visual->class | DynamicClass) == DirectColor) {
		visual->offsetRed   = pScrn->offset.red;
		visual->offsetGreen = pScrn->offset.green;
		visual->offsetBlue  = pScrn->offset.blue;
		visual->redMask     = pScrn->mask.red;
		visual->greenMask   = pScrn->mask.green;
		visual->blueMask    = pScrn->mask.blue;
	    }
	}
    }

    /* Must be after RGB order fixed */
    fbPictureInit (pScreen, 0, 0);

#ifdef RENDER
    if ((s = xf86GetOptValString(info->Options, OPTION_SUBPIXEL_ORDER))) {
	if (strcmp(s, "RGB") == 0) subPixelOrder = SubPixelHorizontalRGB;
	else if (strcmp(s, "BGR") == 0) subPixelOrder = SubPixelHorizontalBGR;
	else if (strcmp(s, "NONE") == 0) subPixelOrder = SubPixelNone;
	PictureSetSubpixelOrder (pScreen, subPixelOrder);
    }
#endif

    if (!pScreen->isGPU) {
	if (xorgGetVersion() >= XORG_VERSION_NUMERIC(1,18,3,0,0))
	    value = info->use_glamor;
	else
	    value = FALSE;
	from = X_DEFAULT;

	if (!info->r600_shadow_fb) {
	    if (xf86GetOptValBool(info->Options, OPTION_DRI3, &value))
		from = X_CONFIG;

	    if (xf86GetOptValInteger(info->Options, OPTION_DRI, &driLevel) &&
		(driLevel == 2 || driLevel == 3)) {
		from = X_CONFIG;
		value = driLevel == 3;
	    }
	}

	if (value) {
	    value = radeon_sync_init(pScreen) &&
		radeon_present_screen_init(pScreen) &&
		radeon_dri3_screen_init(pScreen);

	    if (!value)
		from = X_WARNING;
	}

	xf86DrvMsg(pScrn->scrnIndex, from, "DRI3 %sabled\n", value ? "en" : "dis");
    }

    pScrn->vtSema = TRUE;
    xf86SetBackingStore(pScreen);

    if (info->directRenderingEnabled) {
	xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Direct rendering enabled\n");
    } else {
	xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
		   "Direct rendering disabled\n");
    }

    if (info->r600_shadow_fb) {
	xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Acceleration disabled\n");
	info->accelOn = FALSE;
    } else {
	xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, RADEON_LOGLEVEL_DEBUG,
		       "Initializing Acceleration\n");
	if (RADEONAccelInit(pScreen)) {
	    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Acceleration enabled\n");
	    info->accelOn = TRUE;
	} else {
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		       "Acceleration initialization failed\n");
	    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Acceleration disabled\n");
	    info->accelOn = FALSE;
	}
    }

    /* Init DPMS */
    xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, RADEON_LOGLEVEL_DEBUG,
		   "Initializing DPMS\n");
    xf86DPMSInit(pScreen, xf86DPMSSet, 0);

    if (!RADEONCursorInit_KMS(pScreen))
	return FALSE;

    /* DGA setup */
#ifdef XFreeXDGA
    /* DGA is dangerous on kms as the base and framebuffer location may change:
     * http://lists.freedesktop.org/archives/xorg-devel/2009-September/002113.html
     */
    /* xf86DiDGAInit(pScreen, info->LinearAddr + pScrn->fbOffset); */
#endif
    if (info->r600_shadow_fb == FALSE && !pScreen->isGPU) {
        /* Init Xv */
        xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, RADEON_LOGLEVEL_DEBUG,
                       "Initializing Xv\n");
        RADEONInitVideo(pScreen);
    }

    if (info->r600_shadow_fb == TRUE) {
        if (!shadowSetup(pScreen)) {
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		       "Shadowfb initialization failed\n");
            return FALSE;
        }
    }
    pScrn->pScreen = pScreen;

    if (!pScreen->isGPU) {
	if (serverGeneration == 1 && bgNoneRoot && info->accelOn) {
	    info->CreateWindow = pScreen->CreateWindow;
	    pScreen->CreateWindow = RADEONCreateWindow_oneshot;
	}
	info->WindowExposures = pScreen->WindowExposures;
	pScreen->WindowExposures = RADEONWindowExposures_oneshot;
    }

    /* Provide SaveScreen & wrap BlockHandler and CloseScreen */
    /* Wrap CloseScreen */
    info->CloseScreen    = pScreen->CloseScreen;
    pScreen->CloseScreen = RADEONCloseScreen_KMS;
    pScreen->SaveScreen  = RADEONSaveScreen_KMS;
    info->BlockHandler = pScreen->BlockHandler;
    pScreen->BlockHandler = RADEONBlockHandler_KMS;

    info->CreateScreenResources = pScreen->CreateScreenResources;
    pScreen->CreateScreenResources = RADEONCreateScreenResources_KMS;

    pScreen->StartPixmapTracking = PixmapStartDirtyTracking;
    pScreen->StopPixmapTracking = PixmapStopDirtyTracking;
#if HAS_SYNC_SHARED_PIXMAP
    pScreen->SyncSharedPixmap = radeon_sync_shared_pixmap;
#endif

   if (!xf86CrtcScreenInit (pScreen))
       return FALSE;

   /* Wrap pointer motion to flip touch screen around */
//    info->PointerMoved = pScrn->PointerMoved;
//    pScrn->PointerMoved = RADEONPointerMoved;

    if (!drmmode_setup_colormap(pScreen, pScrn))
	return FALSE;

   /* Note unused options */
    if (serverGeneration == 1)
	xf86ShowUnusedOptions(pScrn->scrnIndex, pScrn->options);

    drmmode_init(pScrn, &info->drmmode);

    xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, RADEON_LOGLEVEL_DEBUG,
		   "RADEONScreenInit finished\n");

    info->accel_state->XInited3D = FALSE;
    info->accel_state->engineMode = EXA_ENGINEMODE_UNKNOWN;

    return TRUE;
}

Bool RADEONEnterVT_KMS(ScrnInfoPtr pScrn)
{
    RADEONInfoPtr  info  = RADEONPTR(pScrn);

    xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, RADEON_LOGLEVEL_DEBUG,
		   "RADEONEnterVT_KMS\n");

    radeon_set_drm_master(pScrn);

    if (info->r600_shadow_fb) {
	int base_align = drmmode_get_base_align(pScrn, info->pixel_bytes, 0);
	struct radeon_bo *front_bo = radeon_bo_open(info->bufmgr, 0,
						    pScrn->displayWidth *
						    info->pixel_bytes *
						    pScrn->virtualY,
						    base_align,
						    RADEON_GEM_DOMAIN_VRAM, 0);

	if (front_bo) {
	    if (radeon_bo_map(front_bo, 1) == 0) {
		memset(front_bo->ptr, 0, front_bo->size);
		radeon_bo_unref(info->front_buffer->bo.radeon);
		info->front_buffer->bo.radeon = front_bo;
	    } else {
		radeon_bo_unref(front_bo);
		front_bo = NULL;
	    }
	}

	if (!front_bo) {
	    xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
		       "Failed to allocate new scanout BO after VT switch, "
		       "other DRM masters may see screen contents\n");
	}
    }

    info->accel_state->XInited3D = FALSE;
    info->accel_state->engineMode = EXA_ENGINEMODE_UNKNOWN;

    pScrn->vtSema = TRUE;

    if (!drmmode_set_desired_modes(pScrn, &info->drmmode, TRUE))
	return FALSE;

    return TRUE;
}

static
CARD32 cleanup_black_fb(OsTimerPtr timer, CARD32 now, pointer data)
{
    ScreenPtr screen = data;
    ScrnInfoPtr scrn = xf86ScreenToScrn(screen);
    RADEONEntPtr pRADEONEnt = RADEONEntPriv(scrn);
    xf86CrtcConfigPtr xf86_config = XF86_CRTC_CONFIG_PTR(scrn);
    int c;

    if (xf86ScreenToScrn(radeon_primary_screen(screen))->vtSema)
	return 0;

    /* Unreference the all-black FB created by RADEONLeaveVT_KMS. After
     * this, there should be no FB left created by this driver.
     */
    for (c = 0; c < xf86_config->num_crtc; c++) {
	drmmode_crtc_private_ptr drmmode_crtc =
	    xf86_config->crtc[c]->driver_private;

	drmmode_fb_reference(pRADEONEnt->fd, &drmmode_crtc->fb, NULL);
    }

    TimerFree(timer);
    return 0;
}

static void
pixmap_unref_fb(PixmapPtr pixmap)
{
    ScrnInfoPtr scrn = xf86ScreenToScrn(pixmap->drawable.pScreen);
    struct drmmode_fb **fb_ptr = radeon_pixmap_get_fb_ptr(pixmap);
    RADEONEntPtr pRADEONEnt = RADEONEntPriv(scrn);

    if (fb_ptr)
	drmmode_fb_reference(pRADEONEnt->fd, fb_ptr, NULL);
}

static void
client_pixmap_unref_fb(void *value, XID id, void *pScreen)
{
    PixmapPtr pixmap = value;

    if (pixmap->drawable.pScreen == pScreen)
	pixmap_unref_fb(pixmap);
}

void RADEONLeaveVT_KMS(ScrnInfoPtr pScrn)
{
    RADEONInfoPtr  info  = RADEONPTR(pScrn);
    ScreenPtr pScreen = pScrn->pScreen;

    xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, RADEON_LOGLEVEL_DEBUG,
		   "RADEONLeaveVT_KMS\n");

    if (!info->r600_shadow_fb) {
	RADEONEntPtr pRADEONEnt = RADEONEntPriv(pScrn);
	xf86CrtcConfigPtr xf86_config = XF86_CRTC_CONFIG_PTR(pScrn);
	struct drmmode_scanout black_scanout = { .pixmap = NULL, .bo = NULL };
	xf86CrtcPtr crtc;
	drmmode_crtc_private_ptr drmmode_crtc;
	unsigned w = 0, h = 0;
	int i;

	/* If we're called from CloseScreen, trying to clear the black
	 * scanout BO will likely crash and burn
	 */
	if (!pScreen->GCperDepth[0])
	    goto hide_cursors;

	/* Compute maximum scanout dimensions of active CRTCs */
	for (i = 0; i < xf86_config->num_crtc; i++) {
	    crtc = xf86_config->crtc[i];
	    drmmode_crtc = crtc->driver_private;

	    if (!drmmode_crtc->fb)
		continue;

	    w = max(w, crtc->mode.HDisplay);
	    h = max(h, crtc->mode.VDisplay);
	}

	/* Make all active CRTCs scan out from an all-black framebuffer */
	if (w > 0 && h > 0) {
	    if (drmmode_crtc_scanout_create(crtc, &black_scanout, w, h)) {
		struct drmmode_fb *black_fb =
		    radeon_pixmap_get_fb(black_scanout.pixmap);

		radeon_pixmap_clear(black_scanout.pixmap);
		radeon_finish(pScrn, black_scanout.bo);

		for (i = 0; i < xf86_config->num_crtc; i++) {
		    crtc = xf86_config->crtc[i];
		    drmmode_crtc = crtc->driver_private;

		    if (drmmode_crtc->fb) {
			if (black_fb) {
			    drmmode_set_mode(crtc, black_fb, &crtc->mode, 0, 0);
			} else {
			    drmModeSetCrtc(pRADEONEnt->fd,
					   drmmode_crtc->mode_crtc->crtc_id, 0,
					   0, 0, NULL, 0, NULL);
			    drmmode_fb_reference(pRADEONEnt->fd,
						 &drmmode_crtc->fb, NULL);
			}

			if (pScrn->is_gpu) {
			    if (drmmode_crtc->scanout[0].pixmap)
				pixmap_unref_fb(drmmode_crtc->scanout[0].pixmap);
			    if (drmmode_crtc->scanout[1].pixmap)
				pixmap_unref_fb(drmmode_crtc->scanout[1].pixmap);
			} else {
			    drmmode_crtc_scanout_free(crtc);
			}
		    }
		}
	    }
	}

	xf86RotateFreeShadow(pScrn);
	drmmode_crtc_scanout_destroy(&info->drmmode, &black_scanout);

	/* Unreference FBs of all pixmaps. After this, the only FB remaining
	 * should be the all-black one being scanned out by active CRTCs
	 */
	for (i = 0; i < currentMaxClients; i++) {
	    if (i > 0 &&
		(!clients[i] || clients[i]->clientState != ClientStateRunning))
		continue;

	    FindClientResourcesByType(clients[i], RT_PIXMAP,
				      client_pixmap_unref_fb, pScreen);
	}

	pixmap_unref_fb(pScreen->GetScreenPixmap(pScreen));
    } else {
	memset(info->front_buffer->bo.radeon->ptr, 0,
	       pScrn->displayWidth * info->pixel_bytes * pScrn->virtualY);
    }

    if (pScreen->GCperDepth[0])
	TimerSet(NULL, 0, 1000, cleanup_black_fb, pScreen);

 hide_cursors:
    xf86_hide_cursors (pScrn);

    radeon_drop_drm_master(pScrn);

    info->accel_state->XInited3D = FALSE;
    info->accel_state->engineMode = EXA_ENGINEMODE_UNKNOWN;

    xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, RADEON_LOGLEVEL_DEBUG,
		   "Ok, leaving now...\n");
}


Bool RADEONSwitchMode_KMS(ScrnInfoPtr pScrn, DisplayModePtr mode)
{
    Bool ret;
    ret = xf86SetSingleMode (pScrn, mode, RR_Rotate_0);
    return ret;

}

void RADEONAdjustFrame_KMS(ScrnInfoPtr pScrn, int x, int y)
{
    RADEONInfoPtr  info        = RADEONPTR(pScrn);
    drmmode_adjust_frame(pScrn, &info->drmmode, x, y);
    return;
}

static Bool radeon_setup_kernel_mem(ScreenPtr pScreen)
{
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
    RADEONInfoPtr info = RADEONPTR(pScrn);
    xf86CrtcConfigPtr   xf86_config = XF86_CRTC_CONFIG_PTR(pScrn);
    int cpp = info->pixel_bytes;
    int pitch;
    uint32_t tiling_flags = 0;

    if (info->accel_state->exa) {
	xf86DrvMsg(pScreen->myNum, X_ERROR, "Memory map already initialized\n");
	return FALSE;
    }
    if (!info->use_glamor && info->r600_shadow_fb == FALSE) {
        info->accel_state->exa = exaDriverAlloc();
        if (!info->accel_state->exa) {
	    xf86DrvMsg(pScreen->myNum, X_ERROR, "exaDriverAlloc failed\n");
	    return FALSE;
	}
    }

    {
	int cursor_size;
	int c, i;

	cursor_size = info->cursor_w * info->cursor_h * 4;
	cursor_size = RADEON_ALIGN(cursor_size, RADEON_GPU_PAGE_SIZE);
	for (c = 0; c < xf86_config->num_crtc; c++) {
	    drmmode_crtc_private_ptr drmmode_crtc = xf86_config->crtc[c]->driver_private;

	    for (i = 0; i < 2; i++) {
		if (!drmmode_crtc->cursor_bo[i]) {
		    drmmode_crtc->cursor_bo[i] =
			radeon_bo_open(info->bufmgr, 0, cursor_size, 0,
				       RADEON_GEM_DOMAIN_VRAM, 0);

		    if (!(drmmode_crtc->cursor_bo[i])) {
			ErrorF("Failed to allocate cursor buffer memory\n");
			return FALSE;
		    }

		    if (radeon_bo_map(drmmode_crtc->cursor_bo[i], 1))
			ErrorF("Failed to map cursor buffer memory\n");
		}
	    }
	}
    }

    if (!info->front_buffer) {
	int usage = CREATE_PIXMAP_USAGE_BACKING_PIXMAP;

	if (info->allowColorTiling && !info->shadow_primary) {
	    if (info->ChipFamily < CHIP_FAMILY_R600 || info->allowColorTiling2D)
		usage |= RADEON_CREATE_PIXMAP_TILING_MACRO;
	    else
		usage |= RADEON_CREATE_PIXMAP_TILING_MICRO;
	}

        info->front_buffer = radeon_alloc_pixmap_bo(pScrn, pScrn->virtualX,
						    pScrn->virtualY,
						    pScrn->depth,
						    usage,
						    pScrn->bitsPerPixel,
						    &pitch,
						    &info->front_surface,
						    &tiling_flags);

        if (info->r600_shadow_fb == TRUE) {
            if (radeon_bo_map(info->front_buffer->bo.radeon, 1)) {
                ErrorF("Failed to map cursor buffer memory\n");
            }
        }

	if (!info->use_glamor) {
#if X_BYTE_ORDER == X_BIG_ENDIAN
	    switch (cpp) {
	    case 4:
		tiling_flags |= RADEON_TILING_SWAP_32BIT;
		break;
	    case 2:
		tiling_flags |= RADEON_TILING_SWAP_16BIT;
		break;
	    }
	    if (info->ChipFamily < CHIP_FAMILY_R600 &&
		info->r600_shadow_fb && tiling_flags)
		tiling_flags |= RADEON_TILING_SURFACE;
#endif
	    if (tiling_flags)
		radeon_bo_set_tiling(info->front_buffer->bo.radeon, tiling_flags, pitch);
	}

	pScrn->displayWidth = pitch / cpp;
    }

    pitch = pScrn->displayWidth * cpp;
    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Front buffer size: %dK\n",
	       pitch * pScrn->virtualY / 1024);
    radeon_kms_update_vram_limit(pScrn, pitch * pScrn->virtualY);
    return TRUE;
}

void radeon_kms_update_vram_limit(ScrnInfoPtr pScrn, uint32_t new_fb_size)
{
    xf86CrtcConfigPtr   xf86_config = XF86_CRTC_CONFIG_PTR(pScrn);
    RADEONInfoPtr info = RADEONPTR(pScrn);
    uint64_t remain_size_bytes;
    int c;

    for (c = 0; c < xf86_config->num_crtc; c++) {
	drmmode_crtc_private_ptr drmmode_crtc = xf86_config->crtc[c]->driver_private;

	if (drmmode_crtc->cursor_bo[0])
	    new_fb_size += (64 * 4 * 64);
    }

    remain_size_bytes = info->vram_size - new_fb_size;
    remain_size_bytes = (remain_size_bytes / 10) * 9;
    if (remain_size_bytes > 0xffffffff)
	remain_size_bytes = 0xffffffff;
    radeon_cs_set_limit(info->cs, RADEON_GEM_DOMAIN_VRAM,
			(uint32_t)remain_size_bytes);

    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "VRAM usage limit set to %uK\n",
	       (uint32_t)remain_size_bytes / 1024);
}

/* Used to disallow modes that are not supported by the hardware */
ModeStatus RADEONValidMode(ScrnInfoPtr pScrn, DisplayModePtr mode,
                           Bool verbose, int flag)
{
    RADEONInfoPtr info = RADEONPTR(pScrn);
    RADEONEntPtr pRADEONEnt = RADEONEntPriv(pScrn);

    /*
     * RN50 has effective maximum mode bandwidth of about 300MiB/s.
     * XXX should really do this for all chips by properly computing
     * memory bandwidth and an overhead factor.
    */
    if (info->ChipFamily == CHIP_FAMILY_RV100 && !pRADEONEnt->HasCRTC2) {
       if (xf86ModeBandwidth(mode, pScrn->bitsPerPixel) > 300)
          return MODE_BANDWIDTH;
    }
    /* There are problems with double scan mode at high clocks
     * They're likely related PLL and display buffer settings.
     * Disable these modes for now.
     */
    if (mode->Flags & V_DBLSCAN) {
       if ((mode->CrtcHDisplay >= 1024) || (mode->CrtcVDisplay >= 768))
           return MODE_CLOCK_RANGE;
   }
    return MODE_OK;
}
