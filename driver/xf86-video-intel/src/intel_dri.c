/**************************************************************************

Copyright 2001 VA Linux Systems Inc., Fremont, California.
Copyright © 2002 by David Dawes

All Rights Reserved.

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation
on the rights to use, copy, modify, merge, publish, distribute, sub
license, and/or sell copies of the Software, and to permit persons to whom
the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice (including the next
paragraph) shall be included in all copies or substantial portions of the
Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. IN NO EVENT SHALL
ATI, VA LINUX SYSTEMS AND/OR THEIR SUPPLIERS BE LIABLE FOR ANY CLAIM,
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE
USE OR OTHER DEALINGS IN THE SOFTWARE.

**************************************************************************/

/*
 * Authors: Jeff Hartmann <jhartmann@valinux.com>
 *          David Dawes <dawes@xfree86.org>
 *          Keith Whitwell <keith@tungstengraphics.com>
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>
#include <time.h>
#include <errno.h>

#include "xf86.h"
#include "xf86_OSproc.h"
#include "xf86Priv.h"

#include "xf86PciInfo.h"
#include "xf86Pci.h"

#include "windowstr.h"
#include "shadow.h"

#include "xaarop.h"

#include "intel.h"
#include "i830_reg.h"

#include "i915_drm.h"

#include "dri2.h"

#ifdef DRI2
#if DRI2INFOREC_VERSION >= 1
#define USE_DRI2_1_1_0
#endif

extern XF86ModuleData dri2ModuleData;
#endif

typedef struct {
	int refcnt;
	PixmapPtr pixmap;
	unsigned int attachment;
} I830DRI2BufferPrivateRec, *I830DRI2BufferPrivatePtr;

static uint32_t pixmap_flink(PixmapPtr pixmap)
{
	struct intel_pixmap *priv = intel_get_pixmap_private(pixmap);
	uint32_t name;

	if (priv == NULL || priv->bo == NULL)
		return 0;

	if (dri_bo_flink(priv->bo, &name) != 0)
		return 0;

	priv->pinned = 1;
	return name;
}

static PixmapPtr get_front_buffer(DrawablePtr drawable)
{
	ScreenPtr screen = drawable->pScreen;
	ScrnInfoPtr scrn = xf86Screens[screen->myNum];
	intel_screen_private *intel = intel_get_screen_private(scrn);
	PixmapPtr pixmap;

	pixmap = get_drawable_pixmap(drawable);
	if (!intel->use_shadow) {
		pixmap->refcnt++;
	} else if (pixmap_is_scanout(pixmap)) {
		pixmap = fbCreatePixmap(screen, 0, 0, drawable->depth, 0);
		if (pixmap) {
			screen->ModifyPixmapHeader(pixmap,
						   drawable->width,
						   drawable->height,
						   0, 0,
						   intel->front_pitch,
						   intel->front_buffer->virtual);

			intel_set_pixmap_bo(pixmap, intel->front_buffer);
			intel_get_pixmap_private(pixmap)->offscreen = 0;
			if (WindowDrawable(drawable->type))
				screen->SetWindowPixmap((WindowPtr)drawable,
							pixmap);
		}
	} else if (intel_get_pixmap_bo(pixmap)) {
		pixmap->refcnt++;
	} else
		pixmap = NULL;
	return pixmap;
}

static PixmapPtr fixup_shadow(DrawablePtr drawable, PixmapPtr pixmap)
{
	ScreenPtr screen = drawable->pScreen;
	PixmapPtr old = get_drawable_pixmap(drawable);
	struct intel_pixmap *priv = intel_get_pixmap_private(pixmap);
	GCPtr gc;

	/* With an active shadow buffer, 2D pixmaps are created in
	 * system memory and GPU acceleration of 2D render operations
	 * is *disabled*. As DRI is still enabled, we create hardware
	 * buffers for the clients, and need to mix this with the
	 * 2D rendering. So we replace the system pixmap with a GTT
	 * mapping (with the kernel enforcing coherency between
	 * CPU and GPU) for 2D and provide the bo so that clients
	 * can write directly to it (or read from it in the case
	 * of TextureFromPixmap) using the GPU.
	 *
	 * So for a compositor with a GL backend (i.e. compiz) we have
	 * smooth wobbly windows but incur the cost of uncached 2D rendering,
	 * however 3D applications (games and clutter) are still fully
	 * accelerated.
	 */

	if (drm_intel_gem_bo_map_gtt(priv->bo))
		return pixmap;

	screen->ModifyPixmapHeader(pixmap,
				   drawable->width,
				   drawable->height,
				   0, 0,
				   priv->stride,
				   priv->bo->virtual);
	priv->offscreen = 0;

	/* Copy the current contents of the pixmap to the bo. */
	gc = GetScratchGC(drawable->depth, screen);
	if (gc) {
		ValidateGC(&pixmap->drawable, gc);
		gc->ops->CopyArea(drawable, &pixmap->drawable,
				  gc,
				  0, 0,
				  drawable->width,
				  drawable->height,
				  0, 0);
		FreeScratchGC(gc);
	}

	intel_set_pixmap_private(pixmap, NULL);
	screen->DestroyPixmap(pixmap);

	/* Redirect 2D rendering to the uncached GTT map of the bo */
	screen->ModifyPixmapHeader(old,
				   drawable->width,
				   drawable->height,
				   0, 0,
				   priv->stride,
				   priv->bo->virtual);

	/* And redirect the pixmap to the new bo (for 3D). */
	intel_set_pixmap_private(old, priv);
	old->refcnt++;

	intel_get_screen_private(xf86Screens[screen->myNum])->needs_flush = TRUE;
	return old;
}

#if DRI2INFOREC_VERSION < 2
static DRI2BufferPtr
I830DRI2CreateBuffers(DrawablePtr drawable, unsigned int *attachments,
		      int count)
{
	ScreenPtr screen = drawable->pScreen;
	ScrnInfoPtr scrn = xf86Screens[screen->myNum];
	intel_screen_private *intel = intel_get_screen_private(scrn);
	DRI2BufferPtr buffers;
	I830DRI2BufferPrivatePtr privates;
	PixmapPtr pixmap, pDepthPixmap;
	int i;

	buffers = calloc(count, sizeof *buffers);
	if (buffers == NULL)
		return NULL;
	privates = calloc(count, sizeof *privates);
	if (privates == NULL) {
		free(buffers);
		return NULL;
	}

	pDepthPixmap = NULL;
	for (i = 0; i < count; i++) {
		pixmap = NULL;
		if (attachments[i] == DRI2BufferFrontLeft) {
			pixmap = get_front_buffer(drawable);
		} else if (attachments[i] == DRI2BufferStencil && pDepthPixmap) {
			pixmap = pDepthPixmap;
			pixmap->refcnt++;
		}
		if (pixmap == NULL) {
			unsigned int hint = INTEL_CREATE_PIXMAP_DRI2;

			if (intel->tiling & INTEL_TILING_3D) {
				switch (attachments[i]) {
				case DRI2BufferDepth:
					if (SUPPORTS_YTILING(intel))
						hint |= INTEL_CREATE_PIXMAP_TILING_Y;
					else
						hint |= INTEL_CREATE_PIXMAP_TILING_X;
					break;
				case DRI2BufferFakeFrontLeft:
				case DRI2BufferFakeFrontRight:
				case DRI2BufferBackLeft:
				case DRI2BufferBackRight:
					hint |= INTEL_CREATE_PIXMAP_TILING_X;
					break;
				}
			}

			pixmap = screen->CreatePixmap(screen,
						      drawable->width,
						      drawable->height,
						      drawable->depth,
						      hint);
			if (pixmap == NULL ||
			    intel_get_pixmap_bo(pixmap) == NULL)
			{
				if (pixmap)
					screen->DestroyPixmap(pixmap);
				goto unwind;
			}

			if (attachment == DRI2BufferFrontLeft)
				pixmap = fixup_shadow(drawable, pixmap);
		}

		if (attachments[i] == DRI2BufferDepth)
			pDepthPixmap = pixmap;

		buffers[i].attachment = attachments[i];
		buffers[i].pitch = pixmap->devKind;
		buffers[i].cpp = pixmap->drawable.bitsPerPixel / 8;
		buffers[i].driverPrivate = &privates[i];
		buffers[i].flags = 0;	/* not tiled */
		privates[i].refcnt = 1;
		privates[i].pixmap = pixmap;
		privates[i].attachment = attachments[i];

		if ((buffers[i].name = pixmap_flink(pixmap)) == 0) {
			/* failed to name buffer */
			screen->DestroyPixmap(pixmap);
			goto unwind;
		}
	}

	return buffers;

unwind:
	while (i--)
		screen->DestroyPixmap(privates[i].pixmap);
	free(privates);
	free(buffers);
	return NULL;
}

#else

static DRI2Buffer2Ptr
I830DRI2CreateBuffer(DrawablePtr drawable, unsigned int attachment,
		     unsigned int format)
{
	ScreenPtr screen = drawable->pScreen;
	ScrnInfoPtr scrn = xf86Screens[screen->myNum];
	intel_screen_private *intel = intel_get_screen_private(scrn);
	DRI2Buffer2Ptr buffer;
	I830DRI2BufferPrivatePtr privates;
	PixmapPtr pixmap;

	buffer = calloc(1, sizeof *buffer);
	if (buffer == NULL)
		return NULL;
	privates = calloc(1, sizeof *privates);
	if (privates == NULL) {
		free(buffer);
		return NULL;
	}

	pixmap = NULL;
	if (attachment == DRI2BufferFrontLeft)
		pixmap = get_front_buffer(drawable);
	if (pixmap == NULL) {
		unsigned int hint = INTEL_CREATE_PIXMAP_DRI2;

		if (intel->tiling & INTEL_TILING_3D) {
			switch (attachment) {
			case DRI2BufferDepth:
			case DRI2BufferDepthStencil:
				if (SUPPORTS_YTILING(intel)) {
					hint |= INTEL_CREATE_PIXMAP_TILING_Y;
					break;
				}
			case DRI2BufferAccum:
			case DRI2BufferBackLeft:
			case DRI2BufferBackRight:
			case DRI2BufferFakeFrontLeft:
			case DRI2BufferFakeFrontRight:
			case DRI2BufferFrontLeft:
			case DRI2BufferFrontRight:
				hint |= INTEL_CREATE_PIXMAP_TILING_X;
				break;
			default:
				free(privates);
				free(buffer);
				return NULL;
                        }
		}

		pixmap = screen->CreatePixmap(screen,
					      drawable->width,
					      drawable->height,
					      (format != 0) ? format :
							      drawable->depth,
					      hint);
		if (pixmap == NULL || intel_get_pixmap_bo(pixmap) == NULL) {
			if (pixmap)
				screen->DestroyPixmap(pixmap);
			free(privates);
			free(buffer);
			return NULL;
		}

		if (attachment == DRI2BufferFrontLeft)
			pixmap = fixup_shadow(drawable, pixmap);
	}

	buffer->attachment = attachment;
	buffer->pitch = pixmap->devKind;
	buffer->cpp = pixmap->drawable.bitsPerPixel / 8;
	buffer->driverPrivate = privates;
	buffer->format = format;
	buffer->flags = 0;	/* not tiled */
	privates->refcnt = 1;
	privates->pixmap = pixmap;
	privates->attachment = attachment;

	if ((buffer->name = pixmap_flink(pixmap)) == 0) {
		/* failed to name buffer */
		screen->DestroyPixmap(pixmap);
		free(privates);
		free(buffer);
		return NULL;
	}

	return buffer;
}

#endif

#ifndef USE_DRI2_1_1_0

static void
I830DRI2DestroyBuffers(DrawablePtr drawable, DRI2BufferPtr buffers, int count)
{
	ScreenPtr screen = drawable->pScreen;
	I830DRI2BufferPrivatePtr private;
	int i;

	for (i = 0; i < count; i++) {
		private = buffers[i].driverPrivate;
		screen->DestroyPixmap(private->pixmap);
	}

	if (buffers) {
		free(buffers[0].driverPrivate);
		free(buffers);
	}
}

#else

static void I830DRI2DestroyBuffer(DrawablePtr drawable, DRI2Buffer2Ptr buffer)
{
	if (buffer && buffer->driverPrivate) {
		I830DRI2BufferPrivatePtr private = buffer->driverPrivate;
		if (--private->refcnt == 0) {
			ScreenPtr screen = private->pixmap->drawable.pScreen;
			screen->DestroyPixmap(private->pixmap);

			free(private);
			free(buffer);
		}
	} else
		free(buffer);
}

#endif

static void I830DRI2ReferenceBuffer(DRI2Buffer2Ptr buffer)
{
	if (buffer) {
		I830DRI2BufferPrivatePtr private = buffer->driverPrivate;
		private->refcnt++;
	}
}

static void
I830DRI2CopyRegion(DrawablePtr drawable, RegionPtr pRegion,
		   DRI2BufferPtr destBuffer, DRI2BufferPtr sourceBuffer)
{
	I830DRI2BufferPrivatePtr srcPrivate = sourceBuffer->driverPrivate;
	I830DRI2BufferPrivatePtr dstPrivate = destBuffer->driverPrivate;
	ScreenPtr screen = drawable->pScreen;
	ScrnInfoPtr scrn = xf86Screens[screen->myNum];
	intel_screen_private *intel = intel_get_screen_private(scrn);
	DrawablePtr src = (srcPrivate->attachment == DRI2BufferFrontLeft)
		? drawable : &srcPrivate->pixmap->drawable;
	DrawablePtr dst = (dstPrivate->attachment == DRI2BufferFrontLeft)
		? drawable : &dstPrivate->pixmap->drawable;
	RegionPtr pCopyClip;
	GCPtr gc;

	gc = GetScratchGC(dst->depth, screen);
	if (!gc)
		return;

	pCopyClip = REGION_CREATE(screen, NULL, 0);
	REGION_COPY(screen, pCopyClip, pRegion);
	(*gc->funcs->ChangeClip) (gc, CT_REGION, pCopyClip, 0);
	ValidateGC(dst, gc);

	/* Wait for the scanline to be outside the region to be copied */
	if (pixmap_is_scanout(get_drawable_pixmap(dst)) &&
	    intel->swapbuffers_wait &&
	    scrn->currentMode) {
		BoxPtr box;
		BoxRec crtcbox;
		int y1, y2;
		int pipe = -1, event, load_scan_lines_pipe;
		xf86CrtcPtr crtc;
		Bool full_height = FALSE;

		box = REGION_EXTENTS(unused, gc->pCompositeClip);
		crtc = intel_covering_crtc(scrn, box, NULL, &crtcbox);

		/*
		 * Make sure the CRTC is valid and this is the real front
		 * buffer
		 */
		if (crtc != NULL && !crtc->rotatedData) {
			pipe = intel_crtc_to_pipe(crtc);

			/*
			 * Make sure we don't wait for a scanline that will
			 * never occur
			 */
			y1 = (crtcbox.y1 <= box->y1) ? box->y1 - crtcbox.y1 : 0;
			y2 = (box->y2 <= crtcbox.y2) ?
			    box->y2 - crtcbox.y1 : crtcbox.y2 - crtcbox.y1;

			if (y1 == 0 && y2 == (crtcbox.y2 - crtcbox.y1))
			    full_height = TRUE;

			/*
			 * Pre-965 doesn't have SVBLANK, so we need a bit
			 * of extra time for the blitter to start up and
			 * do its job for a full height blit
			 */
			if (full_height && INTEL_INFO(intel)->gen < 40)
			    y2 -= 2;

			if (pipe == 0) {
				event = MI_WAIT_FOR_PIPEA_SCAN_LINE_WINDOW;
				load_scan_lines_pipe =
				    MI_LOAD_SCAN_LINES_DISPLAY_PIPEA;
				if (full_height && INTEL_INFO(intel)->gen >= 40)
				    event = MI_WAIT_FOR_PIPEA_SVBLANK;
			} else {
				event = MI_WAIT_FOR_PIPEB_SCAN_LINE_WINDOW;
				load_scan_lines_pipe =
				    MI_LOAD_SCAN_LINES_DISPLAY_PIPEB;
				if (full_height && INTEL_INFO(intel)->gen >= 40)
				    event = MI_WAIT_FOR_PIPEB_SVBLANK;
			}

			if (crtc->mode.Flags & V_INTERLACE) {
				/* DSL count field lines */
				y1 /= 2;
				y2 /= 2;
			}

			BEGIN_BATCH(5);
			/*
			 * The documentation says that the LOAD_SCAN_LINES
			 * command always comes in pairs. Don't ask me why.
			 */
			OUT_BATCH(MI_LOAD_SCAN_LINES_INCL |
				  load_scan_lines_pipe);
			OUT_BATCH((y1 << 16) | (y2-1));
			OUT_BATCH(MI_LOAD_SCAN_LINES_INCL |
				  load_scan_lines_pipe);
			OUT_BATCH((y1 << 16) | (y2-1));
			OUT_BATCH(MI_WAIT_FOR_EVENT | event);
			ADVANCE_BATCH();
		}
	}

	/* It's important that this copy gets submitted before the
	 * direct rendering client submits rendering for the next
	 * frame, but we don't actually need to submit right now.  The
	 * client will wait for the DRI2CopyRegion reply or the swap
	 * buffer event before rendering, and we'll hit the flush
	 * callback chain before those messages are sent.  We submit
	 * our batch buffers from the flush callback chain so we know
	 * that will happen before the client tries to render
	 * again. */

	/* Re-enable 2D acceleration... */
	if (intel->use_shadow) {
		struct intel_pixmap *src_pixmap, *dst_pixmap;

		src_pixmap = intel_get_pixmap_private(get_drawable_pixmap(src));
		if (src_pixmap) {
			src_pixmap->offscreen = 1;
			src_pixmap->busy = 1;
		}

		dst_pixmap = intel_get_pixmap_private(get_drawable_pixmap(dst));
		if (dst_pixmap) {
			dst_pixmap->offscreen = 1;
			dst_pixmap->busy = 1;
		}

		gc->ops->CopyArea(src, dst, gc,
				  0, 0,
				  drawable->width, drawable->height,
				  0, 0);

		/* and restore 2D/3D coherency */
		if (src_pixmap)
			src_pixmap->offscreen = 0;
		if (dst_pixmap)
			dst_pixmap->offscreen = 0;
	} else {
		gc->ops->CopyArea(src, dst, gc,
				  0, 0,
				  drawable->width, drawable->height,
				  0, 0);
	}
	FreeScratchGC(gc);
}

#if DRI2INFOREC_VERSION >= 4

static int
I830DRI2DrawablePipe(DrawablePtr pDraw)
{
	ScreenPtr pScreen = pDraw->pScreen;
	ScrnInfoPtr pScrn = xf86Screens[pScreen->myNum];
	BoxRec box, crtcbox;
	xf86CrtcPtr crtc;
	int pipe = -1;

	box.x1 = pDraw->x;
	box.y1 = pDraw->y;
	box.x2 = box.x1 + pDraw->width;
	box.y2 = box.y1 + pDraw->height;

	crtc = intel_covering_crtc(pScrn, &box, NULL, &crtcbox);

	/* Make sure the CRTC is valid and this is the real front buffer */
	if (crtc != NULL && !crtc->rotatedData)
		pipe = intel_crtc_to_pipe(crtc);

	return pipe;
}

static void
I830DRI2ExchangeBuffers(DrawablePtr draw, DRI2BufferPtr front,
			DRI2BufferPtr back)
{
	I830DRI2BufferPrivatePtr front_priv, back_priv;
	struct intel_pixmap *front_intel, *back_intel;
	ScreenPtr screen;
	intel_screen_private *intel;
	int tmp;

	front_priv = front->driverPrivate;
	back_priv = back->driverPrivate;

	/* Swap BO names so DRI works */
	tmp = front->name;
	front->name = back->name;
	back->name = tmp;

	/* Swap pixmap bos */
	front_intel = intel_get_pixmap_private(front_priv->pixmap);
	back_intel = intel_get_pixmap_private(back_priv->pixmap);
	intel_set_pixmap_private(front_priv->pixmap, back_intel);
	intel_set_pixmap_private(back_priv->pixmap, front_intel); /* should be screen */

	/* Do we need to update the Screen? */
	screen = draw->pScreen;
	intel = intel_get_screen_private(xf86Screens[screen->myNum]);
	if (front_intel->bo == intel->front_buffer) {
	    dri_bo_unreference (intel->front_buffer);
	    intel->front_buffer = back_intel->bo;
	    dri_bo_reference (intel->front_buffer);
	    intel_set_pixmap_private(screen->GetScreenPixmap(screen),
				     back_intel);
	    back_intel->busy = 1;
	    front_intel->busy = -1;
	}
}

#ifdef notyet
static Bool
can_exchange(DRI2BufferPtr front, DRI2BufferPtr back)
{
	I830DRI2BufferPrivatePtr front_priv = front->driverPrivate;
	I830DRI2BufferPrivatePtr back_priv = back->driverPrivate;
	PixmapPtr front_pixmap = front_priv->pixmap;
	PixmapPtr back_pixmap = back_priv->pixmap;
	struct intel_pixmap *front_intel = intel_get_pixmap_private(front_pixmap);
	struct intel_pixmap *back_intel = intel_get_pixmap_private(back_pixmap);

	if (front_pixmap->drawable.width != back_pixmap->drawable.width)
		return FALSE;

	if (front_pixmap->drawable.height != back_pixmap->drawable.height)
		return FALSE;

	/* XXX should we be checking depth instead of bpp? */
#if 0
	if (front_pixmap->drawable.depth != back_pixmap->drawable.depth)
		return FALSE;
#else
	if (front_pixmap->drawable.bitsPerPixel != back_pixmap->drawable.bitsPerPixel)
		return FALSE;
#endif

	/* prevent an implicit tiling mode change */
	if (front_intel->tiling != back_intel->tiling)
		return FALSE;

	return TRUE;
}

void I830DRI2FrameEventHandler(unsigned int frame, unsigned int tv_sec,
			       unsigned int tv_usec, void *event_data)
{
	DRI2FrameEventPtr event = event_data;
	DrawablePtr drawable;
	ScreenPtr screen;
	ScrnInfoPtr scrn;
	intel_screen_private *intel;
	int status;

	status = dixLookupDrawable(&drawable, event->drawable_id, serverClient,
				   M_ANY, DixWriteAccess);
	if (status != Success) {
		I830DRI2DestroyBuffer(NULL, event->front);
		I830DRI2DestroyBuffer(NULL, event->back);
		free(event);
		return;
	}

	screen = drawable->pScreen;
	scrn = xf86Screens[screen->myNum];
	intel = intel_get_screen_private(scrn);

	switch (event->type) {
	case DRI2_SWAP: {
		int swap_type;

		if (DRI2CanExchange(drawable)) {
			I830DRI2ExchangeBuffers(drawable,
						event->front, event->back);
			swap_type = DRI2_EXCHANGE_COMPLETE;
		} else {
			BoxRec	    box;
			RegionRec	    region;

			box.x1 = 0;
			box.y1 = 0;
			box.x2 = drawable->width;
			box.y2 = drawable->height;
			REGION_INIT(pScreen, &region, &box, 0);

			I830DRI2CopyRegion(drawable,
					   &region, event->front, event->back);
			swap_type = DRI2_BLIT_COMPLETE;
		}
		DRI2SwapComplete(event->client, drawable, frame, tv_sec, tv_usec,
				 swap_type,
				 event->event_complete, event->event_data);
		break;
	}
	case DRI2_WAITMSC:
		DRI2WaitMSCComplete(event->client, drawable,
				    frame, tv_sec, tv_usec);
		break;
	default:
		xf86DrvMsg(scrn->scrnIndex, X_WARNING,
			   "%s: unknown vblank event received\n", __func__);
		/* Unknown type */
		break;
	}

	I830DRI2DestroyBuffer(drawable, event->front);
	I830DRI2DestroyBuffer(drawable, event->back);
	free(event);
}

void I830DRI2FlipEventHandler(unsigned int frame, unsigned int tv_sec,
			      unsigned int tv_usec, void *event_data)
{
	DRI2FrameEventPtr flip = event_data;
	DrawablePtr drawable;
	ScreenPtr screen;
	ScrnInfoPtr scrn;
	int status;

	status = dixLookupDrawable(&drawable, flip->drawable_id, serverClient,
				     M_ANY, DixWriteAccess);
	if (status != Success) {
		free(flip);
		return;
	}

	screen = drawable->pScreen;
	scrn = xf86Screens[screen->myNum];

	/* We assume our flips arrive in order, so we don't check the frame */
	switch (flip->type) {
	case DRI2_SWAP:
		DRI2SwapComplete(flip->client, drawable, frame, tv_sec, tv_usec,
				 DRI2_FLIP_COMPLETE, flip->event_complete,
				 flip->event_data);
	break;
	default:
		xf86DrvMsg(scrn->scrnIndex, X_WARNING,
			   "%s: unknown vblank event received\n", __func__);
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
static int
I830DRI2ScheduleSwap(ClientPtr client, DrawablePtr draw, DRI2BufferPtr front,
		     DRI2BufferPtr back, CARD64 *target_msc, CARD64 divisor,
		     CARD64 remainder, DRI2SwapEventPtr func, void *data)
{
	ScreenPtr screen = draw->pScreen;
	ScrnInfoPtr scrn = xf86Screens[screen->myNum];
	intel_screen_private *intel = intel_get_screen_private(scrn);
	drmVBlank vbl;
	int ret, pipe = I830DRI2DrawablePipe(draw), flip = 0;
	DRI2FrameEventPtr swap_info;
	enum DRI2FrameEventType swap_type = DRI2_SWAP;
	CARD64 current_msc;
	BoxRec box;
	RegionRec region;

	/* Truncate to match kernel interfaces; means occasional overflow
	 * misses, but that's generally not a big deal */
	*target_msc &= 0xffffffff;
	divisor &= 0xffffffff;
	remainder &= 0xffffffff;

	swap_info = calloc(1, sizeof(DRI2FrameEventRec));

	/* Drawable not displayed... just complete the swap */
	if (pipe == -1 || !swap_info)
	    goto blit_fallback;

	swap_info->drawable_id = draw->id;
	swap_info->client = client;
	swap_info->event_complete = func;
	swap_info->event_data = data;
	swap_info->front = front;
	swap_info->back = back;
	I830DRI2ReferenceBuffer(front);
	I830DRI2ReferenceBuffer(back);

	/* Get current count */
	vbl.request.type = DRM_VBLANK_RELATIVE;
	if (pipe > 0)
		vbl.request.type |= DRM_VBLANK_SECONDARY;
	vbl.request.sequence = 0;
	ret = drmWaitVBlank(intel->drmSubFD, &vbl);
	if (ret) {
		xf86DrvMsg(scrn->scrnIndex, X_WARNING,
			   "first get vblank counter failed: %s\n",
			   strerror(errno));
		goto blit_fallback;
	}

	current_msc = vbl.reply.sequence;

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
		if (pipe > 0)
			vbl.request.type |= DRM_VBLANK_SECONDARY;

		/* If non-pageflipping, but blitting/exchanging, we need to use
		 * DRM_VBLANK_NEXTONMISS to avoid unreliable timestamping later
		 * on.
		 */
		if (flip == 0)
			vbl.request.type |= DRM_VBLANK_NEXTONMISS;
		if (pipe > 0)
			vbl.request.type |= DRM_VBLANK_SECONDARY;

		/* If target_msc already reached or passed, set it to
		 * current_msc to ensure we return a reasonable value back
		 * to the caller. This makes swap_interval logic more robust.
		 */
		if (current_msc >= *target_msc)
			*target_msc = current_msc;

		vbl.request.sequence = *target_msc;
		vbl.request.signal = (unsigned long)swap_info;
		ret = drmWaitVBlank(intel->drmSubFD, &vbl);
		if (ret) {
			xf86DrvMsg(scrn->scrnIndex, X_WARNING,
				   "divisor 0 get vblank counter failed: %s\n",
				   strerror(errno));
			goto blit_fallback;
		}

		*target_msc = vbl.reply.sequence + flip;
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
	if (pipe > 0)
		vbl.request.type |= DRM_VBLANK_SECONDARY;

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

	/* Account for 1 frame extra pageflip delay if flip > 0 */
	vbl.request.sequence -= flip;

	vbl.request.signal = (unsigned long)swap_info;
	ret = drmWaitVBlank(intel->drmSubFD, &vbl);
	if (ret) {
		xf86DrvMsg(scrn->scrnIndex, X_WARNING,
			   "final get vblank counter failed: %s\n",
			   strerror(errno));
		goto blit_fallback;
	}

	/* Adjust returned value for 1 fame pageflip offset of flip > 0 */
	*target_msc = vbl.reply.sequence + flip;
	swap_info->frame = *target_msc;

	return TRUE;

blit_fallback:
	box.x1 = 0;
	box.y1 = 0;
	box.x2 = draw->width;
	box.y2 = draw->height;
	REGION_INIT(pScreen, &region, &box, 0);

	I830DRI2CopyRegion(draw, &region, front, back);

	DRI2SwapComplete(client, draw, 0, 0, 0, DRI2_BLIT_COMPLETE, func, data);
	if (swap_info) {
	    I830DRI2DestroyBuffer(draw, swap_info->front);
	    I830DRI2DestroyBuffer(draw, swap_info->back);
	    free(swap_info);
	}
	*target_msc = 0; /* offscreen, so zero out target vblank count */
	return TRUE;
}

/*
 * Get current frame count and frame count timestamp, based on drawable's
 * crtc.
 */
static int
I830DRI2GetMSC(DrawablePtr draw, CARD64 *ust, CARD64 *msc)
{
	ScreenPtr screen = draw->pScreen;
	ScrnInfoPtr scrn = xf86Screens[screen->myNum];
	intel_screen_private *intel = intel_get_screen_private(scrn);
	drmVBlank vbl;
	int ret, pipe = I830DRI2DrawablePipe(draw);

	/* Drawable not displayed, make up a value */
	if (pipe == -1) {
		*ust = 0;
		*msc = 0;
		return TRUE;
	}

	vbl.request.type = DRM_VBLANK_RELATIVE;
	if (pipe > 0)
		vbl.request.type |= DRM_VBLANK_SECONDARY;
	vbl.request.sequence = 0;

	ret = drmWaitVBlank(intel->drmSubFD, &vbl);
	if (ret) {
		static int limit = 5;
		if (limit) {
			xf86DrvMsg(scrn->scrnIndex, X_WARNING,
				   "%s:%d get vblank counter failed: %s\n",
				   __FUNCTION__, __LINE__,
				   strerror(errno));
			limit--;
		}
		return FALSE;
	}

	*ust = ((CARD64)vbl.reply.tval_sec * 1000000) + vbl.reply.tval_usec;
	*msc = vbl.reply.sequence;

	return TRUE;
}

/*
 * Request a DRM event when the requested conditions will be satisfied.
 *
 * We need to handle the event and ask the server to wake up the client when
 * we receive it.
 */
static int
I830DRI2ScheduleWaitMSC(ClientPtr client, DrawablePtr draw, CARD64 target_msc,
			CARD64 divisor, CARD64 remainder)
{
	ScreenPtr screen = draw->pScreen;
	ScrnInfoPtr scrn = xf86Screens[screen->myNum];
	intel_screen_private *intel = intel_get_screen_private(scrn);
	DRI2FrameEventPtr wait_info;
	drmVBlank vbl;
	int ret, pipe = I830DRI2DrawablePipe(draw);
	CARD64 current_msc;

	/* Truncate to match kernel interfaces; means occasional overflow
	 * misses, but that's generally not a big deal */
	target_msc &= 0xffffffff;
	divisor &= 0xffffffff;
	remainder &= 0xffffffff;

	/* Drawable not visible, return immediately */
	if (pipe == -1)
		goto out_complete;

	wait_info = calloc(1, sizeof(DRI2FrameEventRec));
	if (!wait_info)
		goto out_complete;

	wait_info->drawable_id = draw->id;
	wait_info->client = client;
	wait_info->type = DRI2_WAITMSC;

	/* Get current count */
	vbl.request.type = DRM_VBLANK_RELATIVE;
	if (pipe > 0)
		vbl.request.type |= DRM_VBLANK_SECONDARY;
	vbl.request.sequence = 0;
	ret = drmWaitVBlank(intel->drmSubFD, &vbl);
	if (ret) {
		static int limit = 5;
		if (limit) {
			xf86DrvMsg(scrn->scrnIndex, X_WARNING,
				   "%s:%d get vblank counter failed: %s\n",
				   __FUNCTION__, __LINE__,
				   strerror(errno));
			limit--;
		}
		goto out_complete;
	}

	current_msc = vbl.reply.sequence;

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
		if (pipe > 0)
			vbl.request.type |= DRM_VBLANK_SECONDARY;
		vbl.request.sequence = target_msc;
		vbl.request.signal = (unsigned long)wait_info;
		ret = drmWaitVBlank(intel->drmSubFD, &vbl);
		if (ret) {
			static int limit = 5;
			if (limit) {
				xf86DrvMsg(scrn->scrnIndex, X_WARNING,
					   "%s:%d get vblank counter failed: %s\n",
					   __FUNCTION__, __LINE__,
					   strerror(errno));
				limit--;
			}
			goto out_complete;
		}

		wait_info->frame = vbl.reply.sequence;
		DRI2BlockClient(client, draw);
		return TRUE;
	}

	/*
	 * If we get here, target_msc has already passed or we don't have one,
	 * so we queue an event that will satisfy the divisor/remainder equation.
	 */
	vbl.request.type = DRM_VBLANK_ABSOLUTE | DRM_VBLANK_EVENT;
	if (pipe > 0)
		vbl.request.type |= DRM_VBLANK_SECONDARY;

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

	vbl.request.signal = (unsigned long)wait_info;
	ret = drmWaitVBlank(intel->drmSubFD, &vbl);
	if (ret) {
		static int limit = 5;
		if (limit) {
			xf86DrvMsg(scrn->scrnIndex, X_WARNING,
				   "%s:%d get vblank counter failed: %s\n",
				   __FUNCTION__, __LINE__,
				   strerror(errno));
			limit--;
		}
		goto out_complete;
	}

	wait_info->frame = vbl.reply.sequence;
	DRI2BlockClient(client, draw);

	return TRUE;

out_complete:
	DRI2WaitMSCComplete(client, draw, target_msc, 0, 0);
	return TRUE;
}
#endif
#endif

Bool I830DRI2ScreenInit(ScreenPtr screen)
{
	ScrnInfoPtr scrn = xf86Screens[screen->myNum];
	intel_screen_private *intel = intel_get_screen_private(scrn);
	DRI2InfoRec info;
#ifdef USE_DRI2_1_1_0
	int dri2_major = 1;
	int dri2_minor = 0;
#endif
#if DRI2INFOREC_VERSION >= 4
	const char *driverNames[1];
#endif

#ifdef USE_DRI2_1_1_0
	if (xf86LoaderCheckSymbol("DRI2Version")) {
		DRI2Version(&dri2_major, &dri2_minor);
	}

	if (dri2_minor < 1) {
		xf86DrvMsg(scrn->scrnIndex, X_WARNING,
			   "DRI2 requires DRI2 module version 1.1.0 or later\n");
		return FALSE;
	}
#endif

	intel->deviceName = drmGetDeviceNameFromFd(intel->drmSubFD);
	memset(&info, '\0', sizeof(info));
	info.fd = intel->drmSubFD;
	info.driverName = INTEL_INFO(intel)->gen < 40 ? "i915" : "i965";
	info.deviceName = intel->deviceName;

#if DRI2INFOREC_VERSION >= 3
	info.version = 3;
	info.CreateBuffer = I830DRI2CreateBuffer;
	info.DestroyBuffer = I830DRI2DestroyBuffer;
#else
# ifdef USE_DRI2_1_1_0
	info.version = 2;
	info.CreateBuffers = NULL;
	info.DestroyBuffers = NULL;
	info.CreateBuffer = I830DRI2CreateBuffer;
	info.DestroyBuffer = I830DRI2DestroyBuffer;
# else
	info.version = 1;
	info.CreateBuffers = I830DRI2CreateBuffers;
	info.DestroyBuffers = I830DRI2DestroyBuffers;
# endif
#endif

	info.CopyRegion = I830DRI2CopyRegion;
#if DRI2INFOREC_VERSION >= 4
#ifdef notyet
	info.version = 4;
	info.ScheduleSwap = I830DRI2ScheduleSwap;
	info.GetMSC = I830DRI2GetMSC;
	info.ScheduleWaitMSC = I830DRI2ScheduleWaitMSC;
	info.numDrivers = 1;
	info.driverNames = driverNames;
	driverNames[0] = info.driverName;
#endif
#endif

	return DRI2ScreenInit(screen, &info);
}

void I830DRI2CloseScreen(ScreenPtr screen)
{
	ScrnInfoPtr scrn = xf86Screens[screen->myNum];
	intel_screen_private *intel = intel_get_screen_private(scrn);

	DRI2CloseScreen(screen);
	intel->directRenderingType = DRI_NONE;
	drmFree(intel->deviceName);
}
