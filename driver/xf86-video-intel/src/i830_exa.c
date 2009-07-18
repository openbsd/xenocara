/**************************************************************************

Copyright 1998-1999 Precision Insight, Inc., Cedar Park, Texas.
All Rights Reserved.
Copyright (c) 2005 Jesse Barnes <jbarnes@virtuousgeek.org>
  Based on code from i830_xaa.c.

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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "xf86.h"
#include "xaarop.h"
#include "i830.h"
#include "i810_reg.h"
#include "i915_drm.h"
#include <string.h>
#include <sys/mman.h>

#define ALWAYS_SYNC		0
#define ALWAYS_FLUSH		0

const int I830CopyROP[16] =
{
   ROP_0,               /* GXclear */
   ROP_DSa,             /* GXand */
   ROP_SDna,            /* GXandReverse */
   ROP_S,               /* GXcopy */
   ROP_DSna,            /* GXandInverted */
   ROP_D,               /* GXnoop */
   ROP_DSx,             /* GXxor */
   ROP_DSo,             /* GXor */
   ROP_DSon,            /* GXnor */
   ROP_DSxn,            /* GXequiv */
   ROP_Dn,              /* GXinvert*/
   ROP_SDno,            /* GXorReverse */
   ROP_Sn,              /* GXcopyInverted */
   ROP_DSno,            /* GXorInverted */
   ROP_DSan,            /* GXnand */
   ROP_1                /* GXset */
};

const int I830PatternROP[16] =
{
    ROP_0,
    ROP_DPa,
    ROP_PDna,
    ROP_P,
    ROP_DPna,
    ROP_D,
    ROP_DPx,
    ROP_DPo,
    ROP_DPon,
    ROP_PDxn,
    ROP_Dn,
    ROP_PDno,
    ROP_Pn,
    ROP_DPno,
    ROP_DPan,
    ROP_1
};

#ifdef I830_USE_UXA
static int uxa_pixmap_index;
#endif

#ifndef SERVER_1_5
static inline void *dixLookupPrivate(DevUnion **privates, int *key)
{
    return (*privates)[*key].ptr;
}

static inline void dixSetPrivate(DevUnion **privates, int *key, void *val)
{
    (*privates)[*key].ptr = val;
}
#endif

/**
 * Returns whether a given pixmap is tiled or not.
 *
 * Currently, we only have one pixmap that might be tiled, which is the front
 * buffer.  At the point where we are tiling some pixmaps managed by the
 * general allocator, we should move this to using pixmap privates.
 */
Bool
i830_pixmap_tiled(PixmapPtr pPixmap)
{
    ScreenPtr pScreen = pPixmap->drawable.pScreen;
    ScrnInfoPtr pScrn = xf86Screens[pScreen->myNum];
    I830Ptr pI830 = I830PTR(pScrn);
    unsigned long offset;
    dri_bo *bo;

    bo = i830_get_pixmap_bo(pPixmap);
    if (bo != NULL) {
	uint32_t tiling_mode, swizzle_mode;
	int ret;

	ret = drm_intel_bo_get_tiling(bo, &tiling_mode, &swizzle_mode);
	if (ret != 0) {
	    FatalError("Couldn't get tiling on bo %p: %s\n",
		       bo, strerror(-ret));
	}

	return tiling_mode != I915_TILING_NONE;
    }

    offset = intel_get_pixmap_offset(pPixmap);
    if (offset == pI830->front_buffer->offset &&
	pI830->front_buffer->tiling != TILE_NONE)
    {
	return TRUE;
    }

    return FALSE;
}

Bool
i830_get_aperture_space(ScrnInfoPtr pScrn, drm_intel_bo **bo_table, int num_bos)
{
    I830Ptr pI830 = I830PTR(pScrn);

    if (pI830->batch_bo == NULL)
	I830FALLBACK("VT inactive\n");

    bo_table[0] = pI830->batch_bo;
    if (drm_intel_bufmgr_check_aperture_space(bo_table, num_bos) != 0) {
	intel_batch_flush(pScrn, FALSE);
	bo_table[0] = pI830->batch_bo;
	if (drm_intel_bufmgr_check_aperture_space(bo_table, num_bos) != 0)
	    I830FALLBACK("Couldn't get aperture space for BOs\n");
    }
    return TRUE;
}

static unsigned long
i830_pixmap_pitch(PixmapPtr pixmap)
{
    return pixmap->devKind;
}

static int
i830_pixmap_pitch_is_aligned(PixmapPtr pixmap)
{
    ScrnInfoPtr pScrn = xf86Screens[pixmap->drawable.pScreen->myNum];
    I830Ptr pI830 = I830PTR(pScrn);

    return i830_pixmap_pitch(pixmap) % pI830->accel_pixmap_pitch_alignment == 0;
}

static Bool
i830_exa_pixmap_is_offscreen(PixmapPtr pPixmap)
{
    ScrnInfoPtr pScrn = xf86Screens[pPixmap->drawable.pScreen->myNum];
    I830Ptr pI830 = I830PTR(pScrn);

    if ((void *)pPixmap->devPrivate.ptr >= (void *)pI830->FbBase &&
	(void *)pPixmap->devPrivate.ptr <
	(void *)(pI830->FbBase + pI830->FbMapSize))
    {
	return TRUE;
    } else {
	return FALSE;
    }
}

/**
 * I830EXASync - wait for a command to finish
 * @pScreen: current screen
 * @marker: marker command to wait for
 *
 * Wait for the command specified by @marker to finish, then return.  We don't
 * actually do marker waits, though we might in the future.  For now, just
 * wait for a full idle.
 */
static void
I830EXASync(ScreenPtr pScreen, int marker)
{
    ScrnInfoPtr pScrn = xf86Screens[pScreen->myNum];

    I830Sync(pScrn);
}

/**
 * I830EXAPrepareSolid - prepare for a Solid operation, if possible
 */
static Bool
I830EXAPrepareSolid(PixmapPtr pPixmap, int alu, Pixel planemask, Pixel fg)
{
    ScrnInfoPtr pScrn = xf86Screens[pPixmap->drawable.pScreen->myNum];
    I830Ptr pI830 = I830PTR(pScrn);
    unsigned long pitch;
    drm_intel_bo *bo_table[] = {
	NULL, /* batch_bo */
	i830_get_pixmap_bo(pPixmap),
    };

    if (!EXA_PM_IS_SOLID(&pPixmap->drawable, planemask))
	I830FALLBACK("planemask is not solid");

    if (pPixmap->drawable.bitsPerPixel == 24)
	I830FALLBACK("solid 24bpp unsupported!\n");

    if (pPixmap->drawable.bitsPerPixel < 8)
	I830FALLBACK("under 8bpp pixmaps unsupported\n");

    i830_exa_check_pitch_2d(pPixmap);

    pitch = i830_pixmap_pitch(pPixmap);

    if (!i830_pixmap_pitch_is_aligned(pPixmap))
	I830FALLBACK("pixmap pitch not aligned");

    if (!i830_get_aperture_space(pScrn, bo_table, ARRAY_SIZE(bo_table)))
	return FALSE;

    pI830->BR[13] = (I830PatternROP[alu] & 0xff) << 16 ;
    switch (pPixmap->drawable.bitsPerPixel) {
	case 8:
	    break;
	case 16:
	    /* RGB565 */
	    pI830->BR[13] |= (1 << 24);
	    break;
	case 32:
	    /* RGB8888 */
	    pI830->BR[13] |= ((1 << 24) | (1 << 25));
	    break;
    }
    pI830->BR[16] = fg;
    return TRUE;
}

static void
I830EXASolid(PixmapPtr pPixmap, int x1, int y1, int x2, int y2)
{
    ScrnInfoPtr pScrn = xf86Screens[pPixmap->drawable.pScreen->myNum];
    I830Ptr pI830 = I830PTR(pScrn);
    unsigned long pitch;
    uint32_t cmd;

    pitch = i830_pixmap_pitch(pPixmap);

    {
	BEGIN_BATCH(6);

	cmd = XY_COLOR_BLT_CMD;

	if (pPixmap->drawable.bitsPerPixel == 32)
	    cmd |= XY_COLOR_BLT_WRITE_ALPHA | XY_COLOR_BLT_WRITE_RGB;

	if (IS_I965G(pI830) && i830_pixmap_tiled(pPixmap)) {
	    assert((pitch % 512) == 0);
	    pitch >>= 2;
	    cmd |= XY_COLOR_BLT_TILED;
	}

	OUT_BATCH(cmd);

	OUT_BATCH(pI830->BR[13] | pitch);
	OUT_BATCH((y1 << 16) | (x1 & 0xffff));
	OUT_BATCH((y2 << 16) | (x2 & 0xffff));
	OUT_RELOC_PIXMAP(pPixmap, I915_GEM_DOMAIN_RENDER, I915_GEM_DOMAIN_RENDER, 0);
	OUT_BATCH(pI830->BR[16]);
	ADVANCE_BATCH();
    }
}

static void
I830EXADoneSolid(PixmapPtr pPixmap)
{
#if ALWAYS_SYNC || ALWAYS_FLUSH
    ScrnInfoPtr pScrn = xf86Screens[pPixmap->drawable.pScreen->myNum];

#if ALWAYS_FLUSH
    intel_batch_flush(pScrn, FALSE);
#endif
#if ALWAYS_SYNC
    I830Sync(pScrn);
#endif
#endif
}

/**
 * TODO:
 *   - support planemask using FULL_BLT_CMD?
 */
static Bool
I830EXAPrepareCopy(PixmapPtr pSrcPixmap, PixmapPtr pDstPixmap, int xdir,
		   int ydir, int alu, Pixel planemask)
{
    ScrnInfoPtr pScrn = xf86Screens[pDstPixmap->drawable.pScreen->myNum];
    I830Ptr pI830 = I830PTR(pScrn);
    drm_intel_bo *bo_table[] = {
	NULL, /* batch_bo */
	i830_get_pixmap_bo(pSrcPixmap),
	i830_get_pixmap_bo(pDstPixmap),
    };

    if (!EXA_PM_IS_SOLID(&pSrcPixmap->drawable, planemask))
	I830FALLBACK("planemask is not solid");

    if (pDstPixmap->drawable.bitsPerPixel < 8)
	I830FALLBACK("under 8bpp pixmaps unsupported\n");

    if (!i830_get_aperture_space(pScrn, bo_table, ARRAY_SIZE(bo_table)))
	return FALSE;

    i830_exa_check_pitch_2d(pSrcPixmap);
    i830_exa_check_pitch_2d(pDstPixmap);

    pI830->pSrcPixmap = pSrcPixmap;

    pI830->BR[13] = I830CopyROP[alu] << 16;

    switch (pSrcPixmap->drawable.bitsPerPixel) {
    case 8:
	break;
    case 16:
	pI830->BR[13] |= (1 << 24);
	break;
    case 32:
	pI830->BR[13] |= ((1 << 25) | (1 << 24));
	break;
    }
    return TRUE;
}

static void
I830EXACopy(PixmapPtr pDstPixmap, int src_x1, int src_y1, int dst_x1,
	    int dst_y1, int w, int h)
{
    ScrnInfoPtr pScrn = xf86Screens[pDstPixmap->drawable.pScreen->myNum];
    I830Ptr pI830 = I830PTR(pScrn);
    uint32_t cmd;
    int dst_x2, dst_y2;
    unsigned int dst_pitch, src_pitch;

    dst_x2 = dst_x1 + w;
    dst_y2 = dst_y1 + h;

    dst_pitch = i830_pixmap_pitch(pDstPixmap);
    src_pitch = i830_pixmap_pitch(pI830->pSrcPixmap);

    {
	BEGIN_BATCH(8);

	cmd = XY_SRC_COPY_BLT_CMD;

	if (pDstPixmap->drawable.bitsPerPixel == 32)
	    cmd |= XY_SRC_COPY_BLT_WRITE_ALPHA | XY_SRC_COPY_BLT_WRITE_RGB;

	if (IS_I965G(pI830)) {
	    if (i830_pixmap_tiled(pDstPixmap)) {
		assert((dst_pitch % 512) == 0);
		dst_pitch >>= 2;
		cmd |= XY_SRC_COPY_BLT_DST_TILED;
	    }

	    if (i830_pixmap_tiled(pI830->pSrcPixmap)) {
		assert((src_pitch % 512) == 0);
		src_pitch >>= 2;
		cmd |= XY_SRC_COPY_BLT_SRC_TILED;
	    }
	}

	OUT_BATCH(cmd);

	OUT_BATCH(pI830->BR[13] | dst_pitch);
	OUT_BATCH((dst_y1 << 16) | (dst_x1 & 0xffff));
	OUT_BATCH((dst_y2 << 16) | (dst_x2 & 0xffff));
	OUT_RELOC_PIXMAP(pDstPixmap, I915_GEM_DOMAIN_RENDER, I915_GEM_DOMAIN_RENDER, 0);
	OUT_BATCH((src_y1 << 16) | (src_x1 & 0xffff));
	OUT_BATCH(src_pitch);
	OUT_RELOC_PIXMAP(pI830->pSrcPixmap, I915_GEM_DOMAIN_RENDER, 0, 0);

	ADVANCE_BATCH();
    }
}

static void
I830EXADoneCopy(PixmapPtr pDstPixmap)
{
#if ALWAYS_SYNC || ALWAYS_FLUSH
    ScrnInfoPtr pScrn = xf86Screens[pDstPixmap->drawable.pScreen->myNum];

#if ALWAYS_FLUSH
    intel_batch_flush(pScrn, FALSE);
#endif
#if ALWAYS_SYNC
    I830Sync(pScrn);
#endif
#endif
}


/**
 * Do any cleanup from the Composite operation.
 *
 * This is shared between i830 through i965.
 */
void
i830_done_composite(PixmapPtr pDst)
{
#if ALWAYS_SYNC || ALWAYS_FLUSH
    ScrnInfoPtr pScrn = xf86Screens[pDst->drawable.pScreen->myNum];

#if ALWAYS_FLUSH
    intel_batch_flush(pScrn, FALSE);
#endif
#if ALWAYS_SYNC
    I830Sync(pScrn);
#endif
#endif
}

#define xFixedToFloat(val) \
	((float)xFixedToInt(val) + ((float)xFixedFrac(val) / 65536.0))

static Bool
_i830_transform_point (PictTransformPtr transform,
		       float		x,
		       float		y,
		       float		result[3])
{
    int		    j;

    for (j = 0; j < 3; j++)
    {
	result[j] = (xFixedToFloat (transform->matrix[j][0]) * x +
		     xFixedToFloat (transform->matrix[j][1]) * y +
		     xFixedToFloat (transform->matrix[j][2]));
    }
    if (!result[2])
	return FALSE;
    return TRUE;
}

/**
 * Returns the floating-point coordinates transformed by the given transform.
 *
 * transform may be null.
 */
Bool
i830_get_transformed_coordinates(int x, int y, PictTransformPtr transform,
				 float *x_out, float *y_out)
{
    if (transform == NULL) {
	*x_out = x;
	*y_out = y;
    } else {
	float	result[3];

	if (!_i830_transform_point (transform, (float) x, (float) y, result))
	    return FALSE;
	*x_out = result[0] / result[2];
	*y_out = result[1] / result[2];
    }
    return TRUE;
}

/**
 * Returns the un-normalized floating-point coordinates transformed by the given transform.
 *
 * transform may be null.
 */
Bool
i830_get_transformed_coordinates_3d(int x, int y, PictTransformPtr transform,
				    float *x_out, float *y_out, float *w_out)
{
    if (transform == NULL) {
	*x_out = x;
	*y_out = y;
	*w_out = 1;
    } else {
	float    result[3];

	if (!_i830_transform_point (transform, (float) x, (float) y, result))
	    return FALSE;
	*x_out = result[0];
	*y_out = result[1];
	*w_out = result[2];
    }
    return TRUE;
}

/**
 * Returns whether the provided transform is affine.
 *
 * transform may be null.
 */
Bool
i830_transform_is_affine (PictTransformPtr t)
{
    if (t == NULL)
	return TRUE;
    return t->matrix[2][0] == 0 && t->matrix[2][1] == 0;
}

#ifdef XF86DRM_MODE

static void *
I830EXACreatePixmap(ScreenPtr screen, int size, int align)
{
    ScrnInfoPtr scrn = xf86Screens[screen->myNum];
    I830Ptr i830 = I830PTR(scrn);
    struct i830_exa_pixmap_priv *new_priv;

    new_priv = xcalloc(1, sizeof(struct i830_exa_pixmap_priv));
    if (!new_priv)
        return NULL;

    if (size == 0)
	return new_priv;

    new_priv->bo = dri_bo_alloc(i830->bufmgr, "pixmap", size,
				i830->accel_pixmap_offset_alignment);
    if (!new_priv->bo) {
	xfree(new_priv);
	return NULL;
    }

    return new_priv;
}

static void
I830EXADestroyPixmap(ScreenPtr pScreen, void *driverPriv)
{
    struct i830_exa_pixmap_priv *priv = driverPriv;

    if (priv->bo)
	dri_bo_unreference(priv->bo);
    xfree(priv);
}

static Bool I830EXAPixmapIsOffscreen(PixmapPtr pPix)
{
    struct i830_exa_pixmap_priv *driver_priv = exaGetPixmapDriverPrivate(pPix);

    if (driver_priv && driver_priv->bo)
	return TRUE;

    return FALSE;
}

static Bool I830EXAPrepareAccess(PixmapPtr pPix, int index)
{
    ScreenPtr screen = pPix->drawable.pScreen;
    ScrnInfoPtr scrn = xf86Screens[screen->myNum];
    I830Ptr i830 = I830PTR(scrn);
    struct i830_exa_pixmap_priv *driver_priv = exaGetPixmapDriverPrivate(pPix);

    if (!driver_priv) {
	xf86DrvMsg(scrn->scrnIndex, X_WARNING, "%s: no driver private?\n",
		   __FUNCTION__);
	return FALSE;
    }

    if (!driver_priv->bo) {
	xf86DrvMsg(scrn->scrnIndex, X_WARNING, "%s: no buffer object?\n",
		   __FUNCTION__);
	return TRUE;
    }

    intel_batch_flush(scrn, FALSE);
    if (i830->need_sync) {
	I830Sync(scrn);
	i830->need_sync = FALSE;
    }
    if (drm_intel_gem_bo_map_gtt(driver_priv->bo)) {
	xf86DrvMsg(scrn->scrnIndex, X_WARNING, "%s: bo map failed\n",
		   __FUNCTION__);
	return FALSE;
    }
    pPix->devPrivate.ptr = driver_priv->bo->virtual;

    return TRUE;
}

static void I830EXAFinishAccess(PixmapPtr pPix, int index)
{
    ScreenPtr screen = pPix->drawable.pScreen;
    ScrnInfoPtr scrn = xf86Screens[screen->myNum];
    I830Ptr i830 = I830PTR(scrn);
    struct i830_exa_pixmap_priv *driver_priv = exaGetPixmapDriverPrivate(pPix);

    if (!driver_priv) {
	xf86DrvMsg(scrn->scrnIndex, X_WARNING, "%s: no driver private?\n",
		   __FUNCTION__);
	return;
    }

    if (!driver_priv->bo) {
	xf86DrvMsg(scrn->scrnIndex, X_WARNING, "%s: no buffer object?\n",
		   __FUNCTION__);
	return;
    }

    dri_bo_unmap(driver_priv->bo);
    pPix->devPrivate.ptr = NULL;
    if (driver_priv->bo == i830->front_buffer->bo)
	i830->need_flush = TRUE;
}

static Bool I830EXAModifyPixmapHeader(PixmapPtr pPix, int width, int height,
				      int depth, int bitsPerPixel, int devKind,
				      pointer pPixData)
{
    ScreenPtr	pScreen = pPix->drawable.pScreen;
    ScrnInfoPtr pScrn = xf86Screens[pScreen->myNum];
    I830Ptr pI830 = I830PTR(pScrn);
    struct i830_exa_pixmap_priv *driver_priv = exaGetPixmapDriverPrivate(pPix);

    if (!driver_priv)
	return FALSE;

    if (pI830->use_drm_mode &&
	drmmode_is_rotate_pixmap(pScrn, pPixData, &driver_priv->bo)) {
	/* this is a rotate pixmap */
	dri_bo_unmap(driver_priv->bo);
	dri_bo_reference(driver_priv->bo);
        miModifyPixmapHeader(pPix, width, height, depth,
			     bitsPerPixel, devKind, NULL);
    }

    if (pPixData == pI830->FbBase + pScrn->fbOffset) {
	if (driver_priv->bo)
		dri_bo_unreference(driver_priv->bo);
	driver_priv->bo =
	    intel_bo_gem_create_from_name(pI830->bufmgr, "front",
					  pI830->front_buffer->gem_name);
	if (!driver_priv->bo)
	    return FALSE;

	miModifyPixmapHeader(pPix, width, height, depth,
			     bitsPerPixel, devKind, NULL);

	return TRUE;
    }
    return FALSE;
}

#endif /* XF86DRM_MODE */

Bool
I830EXAInit(ScreenPtr pScreen)
{
    ScrnInfoPtr pScrn = xf86Screens[pScreen->myNum];
    I830Ptr pI830 = I830PTR(pScrn);

    pI830->EXADriverPtr = exaDriverAlloc();
    if (pI830->EXADriverPtr == NULL) {
	pI830->accel = ACCEL_NONE;
	return FALSE;
    }
    memset(pI830->EXADriverPtr, 0, sizeof(*pI830->EXADriverPtr));

    pI830->bufferOffset = 0;
    pI830->EXADriverPtr->exa_major = 2;
    /* If compiled against EXA 2.2, require 2.2 so we can use the
     * PixmapIsOffscreen hook.
     */
#if EXA_VERSION_MINOR >= 2
    pI830->EXADriverPtr->exa_minor = 2;
#else
    pI830->EXADriverPtr->exa_minor = 1;
    xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
	       "EXA compatibility mode.  Output rotation rendering "
	       "performance may suffer\n");
#endif
    if (!pI830->use_drm_mode) {
	pI830->EXADriverPtr->memoryBase = pI830->FbBase;
	if (pI830->exa_offscreen) {
	    pI830->EXADriverPtr->offScreenBase = pI830->exa_offscreen->offset;
	    pI830->EXADriverPtr->memorySize = pI830->exa_offscreen->offset +
		pI830->exa_offscreen->size;
	} else {
	    pI830->EXADriverPtr->offScreenBase = pI830->FbMapSize;
	    pI830->EXADriverPtr->memorySize = pI830->FbMapSize;
	}
	pI830->EXADriverPtr->flags = EXA_OFFSCREEN_PIXMAPS;
    } else {
#ifdef XF86DRM_MODE
	pI830->EXADriverPtr->flags = EXA_OFFSCREEN_PIXMAPS | EXA_HANDLES_PIXMAPS;
	pI830->EXADriverPtr->PrepareAccess = I830EXAPrepareAccess;
	pI830->EXADriverPtr->FinishAccess = I830EXAFinishAccess;
#if EXA_VERSION_MINOR >= 4
	pI830->EXADriverPtr->CreatePixmap = I830EXACreatePixmap;
	pI830->EXADriverPtr->DestroyPixmap = I830EXADestroyPixmap;
	pI830->EXADriverPtr->PixmapIsOffscreen = I830EXAPixmapIsOffscreen;
	pI830->EXADriverPtr->ModifyPixmapHeader = I830EXAModifyPixmapHeader;
#endif
#endif /* XF86DRM_MODE */
    }

    DPRINTF(PFX, "EXA Mem: memoryBase 0x%x, end 0x%x, offscreen base 0x%x, "
	    "memorySize 0x%x\n",
	    pI830->EXADriverPtr->memoryBase,
	    pI830->EXADriverPtr->memoryBase + pI830->EXADriverPtr->memorySize,
	    pI830->EXADriverPtr->offScreenBase,
	    pI830->EXADriverPtr->memorySize);

    pI830->EXADriverPtr->pixmapOffsetAlign = pI830->accel_pixmap_offset_alignment;
    pI830->EXADriverPtr->pixmapPitchAlign = pI830->accel_pixmap_pitch_alignment;
    pI830->EXADriverPtr->maxX = pI830->accel_max_x;
    pI830->EXADriverPtr->maxY = pI830->accel_max_y;

    /* Sync */
    pI830->EXADriverPtr->WaitMarker = I830EXASync;

    /* Solid fill */
    pI830->EXADriverPtr->PrepareSolid = I830EXAPrepareSolid;
    pI830->EXADriverPtr->Solid = I830EXASolid;
    pI830->EXADriverPtr->DoneSolid = I830EXADoneSolid;

    /* Copy */
    pI830->EXADriverPtr->PrepareCopy = I830EXAPrepareCopy;
    pI830->EXADriverPtr->Copy = I830EXACopy;
    pI830->EXADriverPtr->DoneCopy = I830EXADoneCopy;

    /* Composite */
    if (!IS_I9XX(pI830)) {
    	pI830->EXADriverPtr->CheckComposite = i830_check_composite;
    	pI830->EXADriverPtr->PrepareComposite = i830_prepare_composite;
    	pI830->EXADriverPtr->Composite = i830_composite;
    	pI830->EXADriverPtr->DoneComposite = i830_done_composite;
    } else if (IS_I915G(pI830) || IS_I915GM(pI830) ||
	       IS_I945G(pI830) || IS_I945GM(pI830) || IS_G33CLASS(pI830))
    {
	pI830->EXADriverPtr->CheckComposite = i915_check_composite;
   	pI830->EXADriverPtr->PrepareComposite = i915_prepare_composite;
	pI830->EXADriverPtr->Composite = i915_composite;
    	pI830->EXADriverPtr->DoneComposite = i830_done_composite;
    } else {
 	pI830->EXADriverPtr->CheckComposite = i965_check_composite;
 	pI830->EXADriverPtr->PrepareComposite = i965_prepare_composite;
 	pI830->EXADriverPtr->Composite = i965_composite;
 	pI830->EXADriverPtr->DoneComposite = i830_done_composite;
    }
#if EXA_VERSION_MINOR >= 2
    if (!pI830->use_drm_mode)
	pI830->EXADriverPtr->PixmapIsOffscreen = i830_exa_pixmap_is_offscreen;
#endif

    if(!exaDriverInit(pScreen, pI830->EXADriverPtr)) {
	xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		   "EXA initialization failed; trying older version\n");
	pI830->EXADriverPtr->exa_minor = 0;
	if(!exaDriverInit(pScreen, pI830->EXADriverPtr)) {
	    xfree(pI830->EXADriverPtr);
	    pI830->accel = ACCEL_NONE;
	    return FALSE;
	}
    }

    I830SelectBuffer(pScrn, I830_SELECT_FRONT);

    return TRUE;
}

dri_bo *
i830_get_pixmap_bo(PixmapPtr pixmap)
{
    ScreenPtr screen = pixmap->drawable.pScreen;
    ScrnInfoPtr scrn = xf86Screens[screen->myNum];
    I830Ptr i830 = I830PTR(scrn);

#ifdef I830_USE_UXA
    if (i830->accel == ACCEL_UXA) {
	return dixLookupPrivate(&pixmap->devPrivates, &uxa_pixmap_index);
    }
#endif
#ifdef XF86DRM_MODE
    if (i830->accel == ACCEL_EXA) {
	struct i830_exa_pixmap_priv *driver_priv =
	    exaGetPixmapDriverPrivate(pixmap);
	return driver_priv ? driver_priv->bo : NULL;
    }
#endif

    return NULL;
}

void
i830_set_pixmap_bo(PixmapPtr pixmap, dri_bo *bo)
{
    ScrnInfoPtr pScrn = xf86Screens[pixmap->drawable.pScreen->myNum];
    I830Ptr i830 = I830PTR(pScrn);
    dri_bo  *old_bo = i830_get_pixmap_bo (pixmap);

    if (old_bo)
	dri_bo_unreference (old_bo);
#if I830_USE_UXA
    if (i830->accel == ACCEL_UXA) {
	dri_bo_reference(bo);
	dixSetPrivate(&pixmap->devPrivates, &uxa_pixmap_index, bo);
    }
#endif
#ifdef XF86DRM_MODE
    if (i830->accel == ACCEL_EXA) {
	struct i830_exa_pixmap_priv *driver_priv =
	    exaGetPixmapDriverPrivate(pixmap);
	if (driver_priv) {
	    dri_bo_reference(bo);
	    driver_priv->bo = bo;
	}
    }
#endif
}
#if defined(I830_USE_UXA)

static void
i830_uxa_set_pixmap_bo (PixmapPtr pixmap, dri_bo *bo)
{
    dixSetPrivate(&pixmap->devPrivates, &uxa_pixmap_index, bo);
}

static Bool
i830_uxa_prepare_access (PixmapPtr pixmap, uxa_access_t access)
{
    dri_bo *bo = i830_get_pixmap_bo (pixmap);

    if (bo) {
	ScreenPtr screen = pixmap->drawable.pScreen;
	ScrnInfoPtr scrn = xf86Screens[screen->myNum];
	I830Ptr i830 = I830PTR(scrn);
	
	intel_batch_flush(scrn, FALSE);
	/* XXX: dri_bo_map should handle syncing for us, what's the deal? */
	if (i830->need_sync) {
	    I830Sync(scrn);
	    i830->need_sync = FALSE;
	}

	/* No VT sema or GEM?  No GTT mapping. */
	if (!scrn->vtSema || !i830->memory_manager) {
	    if (dri_bo_map(bo, access == UXA_ACCESS_RW) != 0)
		return FALSE;
	    pixmap->devPrivate.ptr = bo->virtual;
	    return TRUE;
	}

	/* Kernel manages fences at GTT map/fault time */
	if (i830->kernel_exec_fencing) {
	    if (drm_intel_gem_bo_map_gtt(bo)) {
		xf86DrvMsg(scrn->scrnIndex, X_WARNING, "%s: bo map failed\n",
			   __FUNCTION__);
		return FALSE;
	    }
	    pixmap->devPrivate.ptr = bo->virtual;
	} else { /* or not... */
	    if (drm_intel_bo_pin(bo, 4096) != 0)
		return FALSE;
	    drm_intel_gem_bo_start_gtt_access(bo, access == UXA_ACCESS_RW);
	    pixmap->devPrivate.ptr = i830->FbBase + bo->offset;
	}
    }
    return TRUE;
}

static void
i830_uxa_finish_access (PixmapPtr pixmap)
{
    dri_bo *bo = i830_get_pixmap_bo (pixmap);

    if (bo) {
	ScreenPtr screen = pixmap->drawable.pScreen;
	ScrnInfoPtr scrn = xf86Screens[screen->myNum];
	I830Ptr i830 = I830PTR(scrn);

	if (bo == i830->front_buffer->bo)
	    i830->need_flush = TRUE;

	if (!scrn->vtSema || !i830->memory_manager) {
	    dri_bo_unmap(bo);
	    pixmap->devPrivate.ptr = NULL;
	    return;
	}

	if (i830->kernel_exec_fencing)
	    drm_intel_gem_bo_unmap_gtt(bo);
	else
	    drm_intel_bo_unpin(bo);
	pixmap->devPrivate.ptr = NULL;
    }
}

void
i830_uxa_block_handler (ScreenPtr screen)
{
    ScrnInfoPtr scrn = xf86Screens[screen->myNum];
    I830Ptr i830 = I830PTR(scrn);

    if (i830->need_flush) {
	dri_bo_wait_rendering (i830->front_buffer->bo);
	i830->need_flush = FALSE;
    }
}

static Bool
i830_uxa_pixmap_is_offscreen(PixmapPtr pixmap)
{
    ScreenPtr screen = pixmap->drawable.pScreen;

    /* The front buffer is always in memory and pinned */
    if (screen->GetScreenPixmap(screen) == pixmap)
	return TRUE;

    return i830_get_pixmap_bo (pixmap) != NULL;
}

static PixmapPtr
i830_uxa_create_pixmap (ScreenPtr screen, int w, int h, int depth, unsigned usage)
{
    ScrnInfoPtr scrn = xf86Screens[screen->myNum];
    I830Ptr i830 = I830PTR(scrn);
    dri_bo *bo;
    int stride;
    PixmapPtr pixmap;
    
    if (w > 32767 || h > 32767)
	return NullPixmap;

#ifdef SERVER_1_5
    pixmap = fbCreatePixmap (screen, 0, 0, depth, usage);
#else
    pixmap = fbCreatePixmap (screen, 0, 0, depth);
#endif

    if (w && h)
    {
	unsigned int size;
	uint32_t tiling = I915_TILING_NONE;

	stride = ROUND_TO((w * pixmap->drawable.bitsPerPixel + 7) / 8,
			  i830->accel_pixmap_pitch_alignment);

	if (usage == INTEL_CREATE_PIXMAP_TILING_X)
	    tiling = I915_TILING_X;
	else if (usage == INTEL_CREATE_PIXMAP_TILING_Y)
	    tiling = I915_TILING_Y;

	if (tiling == I915_TILING_NONE) {
	    size = stride * h;
	} else {
	    stride = i830_get_fence_pitch(i830, stride, tiling);
	    /* Round the object up to the size of the fence it will live in
	     * if necessary.  We could potentially make the kernel allocate
	     * a larger aperture space and just bind the subset of pages in,
	     * but this is easier and also keeps us out of trouble (as much)
	     * with drm_intel_bufmgr_check_aperture().
	     */
	    size = i830_get_fence_size(i830, stride * h);
	}

	bo = drm_intel_bo_alloc_for_render(i830->bufmgr, "pixmap", size, 0);
	if (!bo) {
	    fbDestroyPixmap (pixmap);
	    return NullPixmap;
	}

	if (tiling != I915_TILING_NONE)
	    drm_intel_bo_set_tiling(bo, &tiling, stride);

	screen->ModifyPixmapHeader (pixmap, w, h, 0, 0, stride, NULL);
    
	i830_uxa_set_pixmap_bo (pixmap, bo);
    }

    return pixmap;
}


#ifndef SERVER_1_5
static PixmapPtr
i830_uxa_server_14_create_pixmap (ScreenPtr screen, int w, int h, int depth)
{
    /* For server pre-1.6, we're never allocating DRI2 buffers, so no need for
     * a hint.
     */
    return i830_uxa_create_pixmap(screen, w, h, depth, 0);
}
#endif

static Bool
i830_uxa_destroy_pixmap (PixmapPtr pixmap)
{
    if (pixmap->refcnt == 1) {
	dri_bo  *bo = i830_get_pixmap_bo (pixmap);
    
	if (bo)
	    dri_bo_unreference (bo);
    }
    fbDestroyPixmap (pixmap);
    return TRUE;
}

void i830_uxa_create_screen_resources(ScreenPtr pScreen)
{
    ScrnInfoPtr scrn = xf86Screens[pScreen->myNum];
    I830Ptr i830 = I830PTR(scrn);
    dri_bo *bo = i830->front_buffer->bo;

    if (bo != NULL) {
	PixmapPtr   pixmap = pScreen->GetScreenPixmap(pScreen);
	i830_uxa_set_pixmap_bo (pixmap, bo);
	dri_bo_reference(bo);
    }
}

Bool
i830_uxa_init (ScreenPtr pScreen)
{
    ScrnInfoPtr scrn = xf86Screens[pScreen->myNum];
    I830Ptr i830 = I830PTR(scrn);

#ifdef SERVER_1_5
    if (!dixRequestPrivate(&uxa_pixmap_index, 0))
	return FALSE;
#else
    if (!AllocatePixmapPrivate(pScreen, uxa_pixmap_index, 0))
	return FALSE;
#endif

    i830->uxa_driver = uxa_driver_alloc();
    if (i830->uxa_driver == NULL) {
	i830->accel = ACCEL_NONE;
	return FALSE;
    }
    memset(i830->uxa_driver, 0, sizeof(*i830->uxa_driver));

    i830->bufferOffset = 0;
    i830->uxa_driver->uxa_major = 1;
    i830->uxa_driver->uxa_minor = 0;

    /* Solid fill */
    i830->uxa_driver->prepare_solid = I830EXAPrepareSolid;
    i830->uxa_driver->solid = I830EXASolid;
    i830->uxa_driver->done_solid = I830EXADoneSolid;

    /* Copy */
    i830->uxa_driver->prepare_copy = I830EXAPrepareCopy;
    i830->uxa_driver->copy = I830EXACopy;
    i830->uxa_driver->done_copy = I830EXADoneCopy;

    /* Composite */
    if (!IS_I9XX(i830)) {
    	i830->uxa_driver->check_composite = i830_check_composite;
    	i830->uxa_driver->prepare_composite = i830_prepare_composite;
    	i830->uxa_driver->composite = i830_composite;
    	i830->uxa_driver->done_composite = i830_done_composite;
    } else if (IS_I915G(i830) || IS_I915GM(i830) ||
	       IS_I945G(i830) || IS_I945GM(i830) || IS_G33CLASS(i830))
    {
	i830->uxa_driver->check_composite = i915_check_composite;
   	i830->uxa_driver->prepare_composite = i915_prepare_composite;
	i830->uxa_driver->composite = i915_composite;
    	i830->uxa_driver->done_composite = i830_done_composite;
    } else {
 	i830->uxa_driver->check_composite = i965_check_composite;
 	i830->uxa_driver->prepare_composite = i965_prepare_composite;
 	i830->uxa_driver->composite = i965_composite;
 	i830->uxa_driver->done_composite = i830_done_composite;
    }

    i830->uxa_driver->prepare_access = i830_uxa_prepare_access;
    i830->uxa_driver->finish_access = i830_uxa_finish_access;
    i830->uxa_driver->pixmap_is_offscreen = i830_uxa_pixmap_is_offscreen;

    if(!uxa_driver_init(pScreen, i830->uxa_driver)) {
	xf86DrvMsg(scrn->scrnIndex, X_INFO,
		   "UXA initialization failed\n");
	xfree(i830->uxa_driver);
	i830->accel = ACCEL_NONE;
	return FALSE;
    }

#ifdef SERVER_1_5
    pScreen->CreatePixmap = i830_uxa_create_pixmap;
#else
    pScreen->CreatePixmap = i830_uxa_server_14_create_pixmap;
#endif
    pScreen->DestroyPixmap = i830_uxa_destroy_pixmap;

    I830SelectBuffer(scrn, I830_SELECT_FRONT);

    uxa_set_fallback_debug(pScreen, i830->fallback_debug);

    return TRUE;
}
#endif /* I830_USE_UXA */

#ifdef XF86DRI

#ifndef ExaOffscreenMarkUsed
extern void ExaOffscreenMarkUsed(PixmapPtr);
#endif

unsigned long long
I830TexOffsetStart(PixmapPtr pPix)
{
    exaMoveInPixmap(pPix);
    ExaOffscreenMarkUsed(pPix);

    return exaGetPixmapOffset(pPix);
}
#endif
