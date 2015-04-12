/*
 * Copyright © 2007 Intel Corporation
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
 *    Zhenyu Wang <zhenyu.z.wang@intel.com>
 *
 */
#include "intel_xvmc_private.h"
#include <xcb/xcb.h>
#include <xcb/xcb_aux.h>
#include <xcb/dri2.h>
#include <X11/Xlib-xcb.h>
#include <X11/extensions/dri2tokens.h>

/* global */
struct _intel_xvmc_driver *xvmc_driver = NULL;

/* Lookup tables to speed common calculations for coded_block_pattern */
/* each block is ((8*8) * sizeof(short)) */
unsigned int mb_bytes_420[] = {
	0,			/* 0 */
	128,			/* 1 */
	128,			/* 10 */
	256,			/* 11 */
	128,			/* 100 */
	256,			/* 101 */
	256,			/* 110 */
	384,			/* 111 */
	128,			/* 1000 */
	256,			/* 1001 */
	256,			/* 1010 */
	384,			/* 1011 */
	256,			/* 1100 */
	384,			/* 1101 */
	384,			/* 1110 */
	512,			/* 1111 */
	128,			/* 10000 */
	256,			/* 10001 */
	256,			/* 10010 */
	384,			/* 10011 */
	256,			/* 10100 */
	384,			/* 10101 */
	384,			/* 10110 */
	512,			/* 10111 */
	256,			/* 11000 */
	384,			/* 11001 */
	384,			/* 11010 */
	512,			/* 11011 */
	384,			/* 11100 */
	512,			/* 11101 */
	512,			/* 11110 */
	640,			/* 11111 */
	128,			/* 100000 */
	256,			/* 100001 */
	256,			/* 100010 */
	384,			/* 100011 */
	256,			/* 100100 */
	384,			/* 100101 */
	384,			/* 100110 */
	512,			/* 100111 */
	256,			/* 101000 */
	384,			/* 101001 */
	384,			/* 101010 */
	512,			/* 101011 */
	384,			/* 101100 */
	512,			/* 101101 */
	512,			/* 101110 */
	640,			/* 101111 */
	256,			/* 110000 */
	384,			/* 110001 */
	384,			/* 110010 */
	512,			/* 110011 */
	384,			/* 110100 */
	512,			/* 110101 */
	512,			/* 110110 */
	640,			/* 110111 */
	384,			/* 111000 */
	512,			/* 111001 */
	512,			/* 111010 */
	640,			/* 111011 */
	512,			/* 111100 */
	640,			/* 111101 */
	640,			/* 111110 */
	768			/* 111111 */
};

static int
dri2_connect(Display *display)
{
	xcb_dri2_query_version_cookie_t query_version_cookie;
	xcb_dri2_query_version_reply_t *query_version_reply;
	xcb_dri2_connect_cookie_t connect_cookie;
	xcb_dri2_connect_reply_t *connect_reply;
	xcb_dri2_authenticate_cookie_t auth_cookie;
	xcb_dri2_authenticate_reply_t *auth_reply;
	xcb_screen_t *root;
	xcb_connection_t *c = XGetXCBConnection(display);
	drm_magic_t magic;
	const xcb_query_extension_reply_t *dri2_reply;
	char *device_name;
	int len;

	root = xcb_aux_get_screen(c, DefaultScreen(display));

	dri2_reply = xcb_get_extension_data(c, &xcb_dri2_id);

	if (!dri2_reply) {
		XVMC_ERR("DRI2 required");
		return BadValue;
	}

	/* Query the extension and make our first use of it at the same time. */
	query_version_cookie = xcb_dri2_query_version(c, 1, 0);
	connect_cookie = xcb_dri2_connect(c, root->root, DRI2DriverDRI);

	query_version_reply =
		xcb_dri2_query_version_reply(c, query_version_cookie, NULL);
	connect_reply = xcb_dri2_connect_reply(c, connect_cookie, NULL);

	if (!query_version_reply) {
		XVMC_ERR("DRI2 required");
		return BadValue;
	}
	free(query_version_reply);

	len = xcb_dri2_connect_device_name_length(connect_reply);
	device_name = malloc(len + 1);
	if (!device_name) {
		XVMC_ERR("malloc failure");
		return BadAlloc;
	}
	strncpy(device_name, xcb_dri2_connect_device_name(connect_reply), len);
	device_name[len] = 0;
	xvmc_driver->fd = open(device_name, O_RDWR);
	free(device_name);
	free(connect_reply);
	if (xvmc_driver->fd < 0) {
		XVMC_ERR("Failed to open drm device: %s\n", strerror(errno));
		return BadValue;
	}

	if (drmGetMagic(xvmc_driver->fd, &magic)) {
		XVMC_ERR("Failed to get magic\n");
		return BadValue;
	}

	auth_cookie = xcb_dri2_authenticate(c, root->root, magic);
	auth_reply = xcb_dri2_authenticate_reply(c, auth_cookie, NULL);
	if (!auth_reply) {
		XVMC_ERR("Failed to authenticate magic %d\n", magic);
		return BadValue;
	}
	free(auth_reply);

	return Success;
}

/*
* Function: XvMCCreateContext
* Description: Create a XvMC context for the given surface parameters.
* Arguments:
*   display - Connection to the X server.
*   port - XvPortID to use as avertised by the X connection.
*   surface_type_id - Unique identifier for the Surface type.
*   width - Width of the surfaces.
*   height - Height of the surfaces.
*   flags - one or more of the following
*      XVMC_DIRECT - A direct rendered context is requested.
*
* Notes: surface_type_id and width/height parameters must match those
*        returned by XvMCListSurfaceTypes.
* Returns: Status
*/
_X_EXPORT Status XvMCCreateContext(Display * display, XvPortID port,
				   int surface_type_id, int width, int height,
				   int flags, XvMCContext * context)
{
	Status ret;
	CARD32 *priv_data = NULL;
	struct intel_xvmc_hw_context *comm;
	int major, minor;
	int error_base;
	int event_base;
	int priv_count;

	/* Verify Obvious things first */
	if (!display || !context)
		return BadValue;

	if (!(flags & XVMC_DIRECT)) {
		XVMC_ERR("Indirect Rendering not supported! Using Direct.");
		return BadValue;
	}

	/*
	   Width, Height, and flags are checked against surface_type_id
	   and port for validity inside the X server, no need to check
	   here.
	 */
	context->surface_type_id = surface_type_id;
	context->width = (unsigned short)((width + 15) & ~15);
	context->height = (unsigned short)((height + 15) & ~15);
	context->flags = flags;
	context->port = port;

	if (!XvMCQueryExtension(display, &event_base, &error_base)) {
		XVMC_ERR("XvMCExtension is not available!");
		return BadValue;
	}

	ret = XvMCQueryVersion(display, &major, &minor);
	if (ret) {
		XVMC_ERR
		    ("XvMCQueryVersion Failed, unable to determine protocol version.");
		return ret;
	}

	/* XXX: major and minor could be checked in future for XvMC
	 * protocol capability (i.e H.264/AVC decode available)
	 */

	/*
	   Pass control to the X server to create a drm_context_t for us and
	   validate the with/height and flags.
	 */
	if ((ret =
	     _xvmc_create_context(display, context, &priv_count, &priv_data))) {
		XVMC_ERR("Unable to create XvMC Context.");
		return ret;
	}

	comm = (struct intel_xvmc_hw_context *)priv_data;

	if (xvmc_driver == NULL || xvmc_driver->type != comm->type) {
		switch (comm->type) {
		case XVMC_I915_MPEG2_MC:
			xvmc_driver = &i915_xvmc_mc_driver;
			break;
		case XVMC_I965_MPEG2_MC:
			xvmc_driver = &i965_xvmc_mc_driver;
			break;
		case XVMC_I965_MPEG2_VLD:
			xvmc_driver = &xvmc_vld_driver;
			break;
		case XVMC_I945_MPEG2_VLD:
		default:
			XVMC_ERR("unimplemented xvmc type %d", comm->type);
			XFree(priv_data);
			priv_data = NULL;
			return BadValue;
		}
	}

	if (xvmc_driver == NULL || xvmc_driver->type != comm->type) {
		XVMC_ERR("fail to load xvmc driver for type %d\n", comm->type);
		return BadValue;
	}

	XVMC_INFO("decoder type is %s", intel_xvmc_decoder_string(comm->type));

	/* check DRI2 */
	ret = Success;
	xvmc_driver->fd = -1;

	ret = dri2_connect(display);
	if (ret != Success) {
		XFree(priv_data);
		context->privData = NULL;
		if (xvmc_driver->fd >= 0)
			close(xvmc_driver->fd);
		xvmc_driver = NULL;
		return ret;
	}

	if ((xvmc_driver->bufmgr =
	     intel_bufmgr_gem_init(xvmc_driver->fd, 1024 * 64)) == NULL) {
		XVMC_ERR("Can't init bufmgr\n");
		return BadAlloc;
	}
	drm_intel_bufmgr_gem_enable_reuse(xvmc_driver->bufmgr);

	if (!intelInitBatchBuffer()) {
		XFree(priv_data);
		context->privData = NULL;

		dri_bufmgr_destroy(xvmc_driver->bufmgr);
		xvmc_driver = NULL;

		return BadAlloc;
	}

	/* call driver hook.
	 * driver hook should free priv_data after return if success.*/
	ret =
	    (xvmc_driver->create_context) (display, context, priv_count,
					   priv_data);
	if (ret) {
		XVMC_ERR("driver create context failed\n");
		intelFiniBatchBuffer();

		XFree(priv_data);
		context->privData = NULL;

		dri_bufmgr_destroy(xvmc_driver->bufmgr);
		xvmc_driver = NULL;
		return ret;
	}

	sigfillset(&xvmc_driver->sa_mask);
	sigdelset(&xvmc_driver->sa_mask, SIGFPE);
	sigdelset(&xvmc_driver->sa_mask, SIGILL);
	sigdelset(&xvmc_driver->sa_mask, SIGSEGV);
	sigdelset(&xvmc_driver->sa_mask, SIGBUS);
	sigdelset(&xvmc_driver->sa_mask, SIGKILL);
	pthread_mutex_init(&xvmc_driver->ctxmutex, NULL);

	intel_xvmc_dump_open();

	return Success;
}

/*
 * Function: XvMCDestroyContext
 * Description: Destorys the specified context.
 *
 * Arguments:
 *   display - Specifies the connection to the server.
 *   context - The context to be destroyed.
 *
 */
_X_EXPORT Status XvMCDestroyContext(Display * display, XvMCContext * context)
{
	Status ret;
	int screen;

	if (!display || !context)
		return XvMCBadContext;
	screen = DefaultScreen(display);
	ret = (xvmc_driver->destroy_context) (display, context);
	if (ret) {
		XVMC_ERR("destroy context fail\n");
		return ret;
	}

	intelFiniBatchBuffer();

	dri_bufmgr_destroy(xvmc_driver->bufmgr);

	ret = _xvmc_destroy_context(display, context);
	if (ret != Success) {
		XVMC_ERR("_xvmc_destroy_context fail\n");
		return ret;
	}

	if (xvmc_driver->num_ctx == 0) {
		pthread_mutex_destroy(&xvmc_driver->ctxmutex);

		if (xvmc_driver->fd >= 0)
			close(xvmc_driver->fd);

		xvmc_driver->fd = -1;
		intel_xvmc_dump_close();
	}
	return Success;
}

/*
 * Function: XvMCCreateSurface
 */
_X_EXPORT Status XvMCCreateSurface(Display * display, XvMCContext * context,
				   XvMCSurface * surface)
{
	Status ret;
	int priv_count;
	CARD32 *priv_data;
	intel_xvmc_surface_ptr intel_surf = NULL;
	struct intel_xvmc_context *intel_ctx;

	if (!display || !context)
		return XvMCBadContext;

	if (!surface)
		return XvMCBadSurface;

	intel_ctx = context->privData;

	if ((ret = _xvmc_create_surface(display, context, surface,
					&priv_count, &priv_data))) {
		XVMC_ERR("Unable to create XvMCSurface.");
		return ret;
	}

	XFree(priv_data);

	surface->privData = calloc(1, sizeof(struct intel_xvmc_surface));

	if (!(intel_surf = surface->privData))
		goto out_xvmc;

	intel_surf->bo = drm_intel_bo_alloc(xvmc_driver->bufmgr,
					      "surface",
					      intel_ctx->surface_bo_size,
					      GTT_PAGE_SIZE);
	if (!intel_surf->bo)
		goto out_surf;

	if (drm_intel_bo_flink(intel_surf->bo, &intel_surf->gem_handle))
		goto out_bo;

	intel_surf = surface->privData;
	intel_surf->context = context;

	intel_surf->image = XvCreateImage(display, context->port,
					  FOURCC_XVMC,
					  (char *) &intel_surf->gem_handle,
					  surface->width, surface->height);
	if (!intel_surf->image) {
		XVMC_ERR("Can't create XvImage for surface\n");
		goto out_bo;
	}

	return Success;

out_bo:
	drm_intel_bo_unreference(intel_surf->bo);
out_surf:
	free(intel_surf);
out_xvmc:
	_xvmc_destroy_surface(display, surface);
	return BadAlloc;
}

/*
 * Function: XvMCDestroySurface
 */
_X_EXPORT Status XvMCDestroySurface(Display * display, XvMCSurface * surface)
{
	intel_xvmc_surface_ptr intel_surf;

	if (!display || !surface)
		return XvMCBadSurface;

	intel_surf = surface->privData;
	if (!intel_surf)
		return XvMCBadSurface;

	XFree(intel_surf->image);
	if (intel_surf->gc_init)
		XFreeGC(display, intel_surf->gc);

	drm_intel_bo_unreference(intel_surf->bo);

	free(intel_surf);

	_xvmc_destroy_surface(display, surface);

	return Success;
}

/*
 * Function: XvMCCreateBlocks
 */
_X_EXPORT Status XvMCCreateBlocks(Display * display, XvMCContext * context,
				  unsigned int num_blocks,
				  XvMCBlockArray * block)
{
	if (!display || !context || !num_blocks || !block)
		return BadValue;

	memset(block, 0, sizeof(XvMCBlockArray));

	if (!
	    (block->blocks =
	     (short *)malloc((num_blocks << 6) * sizeof(short))))
		return BadAlloc;

	block->num_blocks = num_blocks;
	block->context_id = context->context_id;
	block->privData = NULL;

	return Success;
}

/*
 * Function: XvMCDestroyBlocks
 */
_X_EXPORT Status XvMCDestroyBlocks(Display * display, XvMCBlockArray * block)
{
	if (!display || !block)
		return BadValue;

	if (block->blocks)
		free(block->blocks);

	block->context_id = 0;
	block->num_blocks = 0;
	block->blocks = NULL;
	block->privData = NULL;

	return Success;
}

/*
 * Function: XvMCCreateMacroBlocks
 */
_X_EXPORT Status XvMCCreateMacroBlocks(Display * display, XvMCContext * context,
				       unsigned int num_blocks,
				       XvMCMacroBlockArray * blocks)
{
	if (!display || !context || !blocks || !num_blocks)
		return BadValue;

	memset(blocks, 0, sizeof(XvMCMacroBlockArray));
	blocks->macro_blocks =
	    (XvMCMacroBlock *) malloc(num_blocks * sizeof(XvMCMacroBlock));

	if (!blocks->macro_blocks)
		return BadAlloc;

	blocks->num_blocks = num_blocks;
	blocks->context_id = context->context_id;
	blocks->privData = NULL;

	return Success;
}

/*
 * Function: XvMCDestroyMacroBlocks
 */
_X_EXPORT Status XvMCDestroyMacroBlocks(Display * display,
					XvMCMacroBlockArray * block)
{
	if (!display || !block)
		return BadValue;
	if (block->macro_blocks)
		free(block->macro_blocks);

	block->context_id = 0;
	block->num_blocks = 0;
	block->macro_blocks = NULL;
	block->privData = NULL;

	return Success;
}

/*
 * Function: XvMCRenderSurface
 *
 * Description: This function does the actual HWMC. Given a list of
 *  macroblock structures it dispatched the hardware commands to execute
 *  them.
 */
_X_EXPORT Status XvMCRenderSurface(Display * display, XvMCContext * context,
				   unsigned int picture_structure,
				   XvMCSurface * target_surface,
				   XvMCSurface * past_surface,
				   XvMCSurface * future_surface,
				   unsigned int flags,
				   unsigned int num_macroblocks,
				   unsigned int first_macroblock,
				   XvMCMacroBlockArray * macroblock_array,
				   XvMCBlockArray * blocks)
{
	Status ret;

	if (!display || !context) {
		XVMC_ERR("Invalid Display, Context or Target!");
		return XvMCBadContext;
	}
	if (!target_surface)
		return XvMCBadSurface;

	intel_xvmc_dump_render(context, picture_structure, target_surface,
			       past_surface, future_surface, flags,
			       num_macroblocks, first_macroblock,
			       macroblock_array, blocks);

	ret =
	    (xvmc_driver->render_surface) (display, context, picture_structure,
					   target_surface, past_surface,
					   future_surface, flags,
					   num_macroblocks, first_macroblock,
					   macroblock_array, blocks);

	if (ret) {
		XVMC_ERR("render surface fail\n");
		return ret;
	}
	return Success;
}

/*
 * Function: XvMCPutSurface
 *
 * Description:
 * Arguments:
 *  display: Connection to X server
 *  surface: Surface to be displayed
 *  draw: X Drawable on which to display the surface
 *  srcx: X coordinate of the top left corner of the region to be
 *          displayed within the surface.
 *  srcy: Y coordinate of the top left corner of the region to be
 *          displayed within the surface.
 *  srcw: Width of the region to be displayed.
 *  srch: Height of the region to be displayed.
 *  destx: X cordinate of the top left corner of the destination region
 *         in the drawable coordinates.
 *  desty: Y cordinate of the top left corner of the destination region
 *         in the drawable coordinates.
 *  destw: Width of the destination region.
 *  desth: Height of the destination region.
 *  flags: One or more of the following.
 *	XVMC_TOP_FIELD - Display only the Top field of the surface.
 *	XVMC_BOTTOM_FIELD - Display only the Bottom Field of the surface.
 *	XVMC_FRAME_PICTURE - Display both fields or frame.
 */
_X_EXPORT Status XvMCPutSurface(Display * display, XvMCSurface * surface,
				Drawable draw, short srcx, short srcy,
				unsigned short srcw, unsigned short srch,
				short destx, short desty,
				unsigned short destw, unsigned short desth,
				int flags)
{
	XvMCContext *context;
	intel_xvmc_surface_ptr intel_surf;

	if (!display || !surface)
		return XvMCBadSurface;

	intel_surf = surface->privData;
	if (!intel_surf)
		return XvMCBadSurface;

	context = intel_surf->context;
	if (!context)
		return XvMCBadSurface;

	if (intel_surf->gc_init == FALSE) {
		intel_surf->gc = XCreateGC(display, draw, 0, NULL);
		intel_surf->gc_init = TRUE;
	} else if (draw != intel_surf->last_draw) {
		XFreeGC(display, intel_surf->gc);
		intel_surf->gc = XCreateGC(display, draw, 0, NULL);
	}
	intel_surf->last_draw = draw;

	return XvPutImage(display, context->port, draw, intel_surf->gc,
			  intel_surf->image, srcx, srcy, srcw, srch, destx,
			  desty, destw, desth);
}

/*
 * Function: XvMCSyncSurface
 * Arguments:
 *   display - Connection to the X server
 *   surface - The surface to synchronize
 */
_X_EXPORT Status XvMCSyncSurface(Display * display, XvMCSurface * surface)
{
	if (!display || !surface)
		return XvMCBadSurface;

	return Success;
}

/*
 * Function: XvMCFlushSurface
 * Description:
 *   This function commits pending rendering requests to ensure that they
 *   wll be completed in a finite amount of time.
 * Arguments:
 *   display - Connection to X server
 *   surface - Surface to flush
 * Returns: Status
 */
_X_EXPORT Status XvMCFlushSurface(Display * display, XvMCSurface * surface)
{
	if (!display || !surface)
		return XvMCBadSurface;

	return Success;
}

/*
 * Function: XvMCGetSurfaceStatus
 * Description:
 * Arguments:
 *  display: connection to X server
 *  surface: The surface to query
 *  stat: One of the Following
 *    XVMC_RENDERING - The last XvMCRenderSurface command has not
 *                     completed.
 *    XVMC_DISPLAYING - The surface is currently being displayed or a
 *                     display is pending.
 */
_X_EXPORT Status XvMCGetSurfaceStatus(Display * display, XvMCSurface * surface,
				      int *stat)
{
	if (!display || !surface || !stat)
		return XvMCBadSurface;

	*stat = 0;

	return Success;
}

/*
 * Function: XvMCHideSurface
 * Description: Stops the display of a surface.
 * Arguments:
 *   display - Connection to the X server.
 *   surface - surface to be hidden.
 *
 * Returns: Status
 */
_X_EXPORT Status XvMCHideSurface(Display * display, XvMCSurface * surface)
{
	if (!display || !surface)
		return XvMCBadSurface;

	return Success;
}

/*
 * Function: XvMCCreateSubpicture
 * Description: This creates a subpicture by filling out the XvMCSubpicture
 *              structure passed to it and returning Success.
 * Arguments:
 *   display - Connection to the X server.
 *   context - The context to create the subpicture for.
 *   subpicture - Pre-allocated XvMCSubpicture structure to be filled in.
 *   width - of subpicture
 *   height - of subpicture
 *   xvimage_id - The id describing the XvImage format.
 *
 * Returns: Status
 */
_X_EXPORT Status XvMCCreateSubpicture(Display * display, XvMCContext * context,
				      XvMCSubpicture * subpicture,
				      unsigned short width,
				      unsigned short height, int xvimage_id)
{
	XVMC_ERR("XvMCCreateSubpicture not implemented!\n");
	return BadValue;
}

/*
 * Function: XvMCClearSubpicture
 * Description: Clear the area of the given subpicture to "color".
 *              structure passed to it and returning Success.
 * Arguments:
 *   display - Connection to the X server.
 *   subpicture - Subpicture to clear.
 *   x, y, width, height - rectangle in the subpicture to clear.
 *   color - The data to file the rectangle with.
 *
 * Returns: Status
 */
_X_EXPORT Status XvMCClearSubpicture(Display * display,
				     XvMCSubpicture * subpicture, short x,
				     short y, unsigned short width,
				     unsigned short height, unsigned int color)
{
	XVMC_ERR("XvMCClearSubpicture not implemented!");
	return BadValue;
}

/*
 * Function: XvMCCompositeSubpicture
 * Description: Composite the XvImae on the subpicture. This composit uses
 *              non-premultiplied alpha. Destination alpha is utilized
 *              except for with indexed subpictures. Indexed subpictures
 *              use a simple "replace".
 * Arguments:
 *   display - Connection to the X server.
 *   subpicture - Subpicture to clear.
 *   image - the XvImage to be used as the source of the composite.
 *   srcx, srcy, width, height - The rectangle from the image to be used.
 *   dstx, dsty - location in the subpicture to composite the source.
 *
 * Returns: Status
 */
_X_EXPORT Status XvMCCompositeSubpicture(Display * display,
					 XvMCSubpicture * subpicture,
					 XvImage * image, short srcx,
					 short srcy, unsigned short width,
					 unsigned short height, short dstx,
					 short dsty)
{
	XVMC_ERR("XvMCCompositeSubpicture not implemented!");
	return BadValue;
}

/*
 * Function: XvMCDestroySubpicture
 * Description: Destroys the specified subpicture.
 * Arguments:
 *   display - Connection to the X server.
 *   subpicture - Subpicture to be destroyed.
 *
 * Returns: Status
 */
_X_EXPORT Status XvMCDestroySubpicture(Display * display,
				       XvMCSubpicture * subpicture)
{
	XVMC_ERR("XvMCDestroySubpicture not implemented!");
	return BadValue;
}

/*
 * Function: XvMCSetSubpicturePalette
 * Description: Set the subpictures palette
 * Arguments:
 *   display - Connection to the X server.
 *   subpicture - Subpiture to set palette for.
 *   palette - A pointer to an array holding the palette data. The array
 *     is num_palette_entries * entry_bytes in size.
 * Returns: Status
 */
_X_EXPORT Status XvMCSetSubpicturePalette(Display * display,
					  XvMCSubpicture * subpicture,
					  unsigned char *palette)
{
	XVMC_ERR("XvMCSetSubpicturePalette not implemented!");
	return BadValue;
}

/*
 * Function: XvMCBlendSubpicture
 * Description:
 *    The behavior of this function is different depending on whether
 *    or not the XVMC_BACKEND_SUBPICTURE flag is set in the XvMCSurfaceInfo.
 *    i915 only support frontend behavior.
 *
 *    XVMC_BACKEND_SUBPICTURE not set ("frontend" behavior):
 *
 *    XvMCBlendSubpicture is a no-op in this case.
 *
 * Arguments:
 *   display - Connection to the X server.
 *   subpicture - The subpicture to be blended into the video.
 *   target_surface - The surface to be displayed with the blended subpic.
 *   source_surface - Source surface prior to blending.
 *   subx, suby, subw, subh - The rectangle from the subpicture to use.
 *   surfx, surfy, surfw, surfh - The rectangle in the surface to blend
 *      blend the subpicture rectangle into. Scaling can ocure if
 *      XVMC_SUBPICTURE_INDEPENDENT_SCALING is set.
 *
 * Returns: Status
 */
_X_EXPORT Status XvMCBlendSubpicture(Display * display,
				     XvMCSurface * target_surface,
				     XvMCSubpicture * subpicture, short subx,
				     short suby, unsigned short subw,
				     unsigned short subh, short surfx,
				     short surfy, unsigned short surfw,
				     unsigned short surfh)
{
	XVMC_ERR("XvMCBlendSubpicture not implemented!");
	return BadValue;
}

/*
 * Function: XvMCBlendSubpicture2
 * Description:
 *    The behavior of this function is different depending on whether
 *    or not the XVMC_BACKEND_SUBPICTURE flag is set in the XvMCSurfaceInfo.
 *    i915 only supports frontend blending.
 *
 *    XVMC_BACKEND_SUBPICTURE not set ("frontend" behavior):
 *
 *    XvMCBlendSubpicture2 blends the source_surface and subpicture and
 *    puts it in the target_surface.  This does not effect the status of
 *    the source surface but will cause the target_surface to query
 *    XVMC_RENDERING until the blend is completed.
 *
 * Arguments:
 *   display - Connection to the X server.
 *   subpicture - The subpicture to be blended into the video.
 *   target_surface - The surface to be displayed with the blended subpic.
 *   source_surface - Source surface prior to blending.
 *   subx, suby, subw, subh - The rectangle from the subpicture to use.
 *   surfx, surfy, surfw, surfh - The rectangle in the surface to blend
 *      blend the subpicture rectangle into. Scaling can ocure if
 *      XVMC_SUBPICTURE_INDEPENDENT_SCALING is set.
 *
 * Returns: Status
 */
_X_EXPORT Status XvMCBlendSubpicture2(Display * display,
				      XvMCSurface * source_surface,
				      XvMCSurface * target_surface,
				      XvMCSubpicture * subpicture,
				      short subx, short suby,
				      unsigned short subw, unsigned short subh,
				      short surfx, short surfy,
				      unsigned short surfw,
				      unsigned short surfh)
{
	XVMC_ERR("XvMCBlendSubpicture2 not implemented!");
	return BadValue;
}

/*
 * Function: XvMCSyncSubpicture
 * Description: This function blocks until all composite/clear requests on
 *              the subpicture have been complete.
 * Arguments:
 *   display - Connection to the X server.
 *   subpicture - The subpicture to synchronize
 *
 * Returns: Status
 */
_X_EXPORT Status XvMCSyncSubpicture(Display * display,
				    XvMCSubpicture * subpicture)
{
	XVMC_ERR("XvMCSyncSubpicture not implemented!");
	return BadValue;
}

/*
 * Function: XvMCFlushSubpicture
 * Description: This function commits pending composite/clear requests to
 *              ensure that they will be completed in a finite amount of
 *              time.
 * Arguments:
 *   display - Connection to the X server.
 *   subpicture - The subpicture whos compsiting should be flushed
 *
 * Returns: Status
 */
_X_EXPORT Status XvMCFlushSubpicture(Display * display,
				     XvMCSubpicture * subpicture)
{
	XVMC_ERR("XvMCFlushSubpicture not implemented!");
	return BadValue;
}

/*
 * Function: XvMCGetSubpictureStatus
 * Description: This function gets the current status of a subpicture
 *
 * Arguments:
 *   display - Connection to the X server.
 *   subpicture - The subpicture whos status is being queried
 *   stat - The status of the subpicture. It can be any of the following
 *          OR'd together:
 *          XVMC_RENDERING  - Last composite or clear request not completed
 *          XVMC_DISPLAYING - Suppicture currently being displayed.
 *
 * Returns: Status
 */
_X_EXPORT Status XvMCGetSubpictureStatus(Display * display,
					 XvMCSubpicture * subpicture, int *stat)
{
	XVMC_ERR("XvMCGetSubpictureStatus not implemented!");
	return BadValue;
}

/*
 * Function: XvMCQueryAttributes
 * Description: An array of XvAttributes of size "number" is returned by
 *   this function. If there are no attributes, NULL is returned and number
 *   is set to 0. The array may be freed with free().
 *
 * Arguments:
 *   display - Connection to the X server.
 *   context - The context whos attributes we are querying.
 *   number - The returned number of recognized atoms
 *
 * Returns:
 *  An array of XvAttributes.
 */
_X_EXPORT XvAttribute *XvMCQueryAttributes(Display * display,
					   XvMCContext * context, int *number)
{
	/* now XvMC has no extra attribs than Xv */
	*number = 0;
	return NULL;
}

/*
 * Function: XvMCSetAttribute
 * Description: This function sets a context-specific attribute.
 *
 * Arguments:
 *   display - Connection to the X server.
 *   context - The context whos attributes we are querying.
 *   attribute - The X atom of the attribute to be changed.
 *   value - The new value for the attribute.
 *
 * Returns:
 *  Status
 */
_X_EXPORT Status XvMCSetAttribute(Display * display, XvMCContext * context,
				  Atom attribute, int value)
{
	return Success;
}

/*
 * Function: XvMCGetAttribute
 * Description: This function queries a context-specific attribute and
 *   returns the value.
 *
 * Arguments:
 *   display - Connection to the X server.
 *   context - The context whos attributes we are querying.
 *   attribute - The X atom of the attribute to be queried
 *   value - The returned attribute value
 *
 * Returns:
 *  Status
 */
_X_EXPORT Status XvMCGetAttribute(Display * display, XvMCContext * context,
				  Atom attribute, int *value)
{
	return Success;
}

_X_EXPORT Status XvMCBeginSurface(Display * display, XvMCContext * context,
				  XvMCSurface * target,
				  XvMCSurface * past,
				  XvMCSurface * future,
				  const XvMCMpegControl * control)
{
	if (xvmc_driver->begin_surface(display, context,
				       target, past, future, control)) {
		XVMC_ERR("BeginSurface fail\n");
		return BadValue;
	}
	return Success;
}

_X_EXPORT Status XvMCLoadQMatrix(Display * display, XvMCContext * context,
				 const XvMCQMatrix * qmx)
{
	if (xvmc_driver->load_qmatrix(display, context, qmx)) {
		XVMC_ERR("LoadQMatrix fail\n");
		return BadValue;
	}
	return Success;
}

_X_EXPORT Status XvMCPutSlice(Display * display, XvMCContext * context,
			      char *slice, int nbytes)
{
	if (xvmc_driver->put_slice(display, context, (unsigned char *) slice, nbytes)) {
		XVMC_ERR("PutSlice fail\n");
		return BadValue;
	}
	return Success;
}

_X_EXPORT Status XvMCPutSlice2(Display * display, XvMCContext * context,
			       char *slice, int nbytes, int slice_code)
{
	if (xvmc_driver->put_slice2
	    (display, context, (unsigned char *) slice, nbytes, slice_code)) {
		XVMC_ERR("PutSlice2 fail\n");
		return BadValue;
	}
	return Success;
}
