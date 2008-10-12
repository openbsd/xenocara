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
#include "intel_xvmc.h"

/* global */
struct _intel_xvmc_driver *xvmc_driver = NULL;

/* Lookup tables to speed common calculations for coded_block_pattern */
/* each block is ((8*8) * sizeof(short)) */
unsigned int mb_bytes_420[] = {
    0, /* 0 */
    128, /* 1 */
    128, /* 10 */
    256, /* 11 */
    128, /* 100 */
    256, /* 101 */
    256, /* 110 */
    384, /* 111 */
    128, /* 1000 */
    256, /* 1001 */
    256, /* 1010 */
    384, /* 1011 */
    256, /* 1100 */
    384, /* 1101 */
    384, /* 1110 */
    512, /* 1111 */
    128, /* 10000 */
    256, /* 10001 */
    256, /* 10010 */
    384, /* 10011 */
    256, /* 10100 */
    384, /* 10101 */
    384, /* 10110 */
    512, /* 10111 */
    256, /* 11000 */
    384, /* 11001 */
    384, /* 11010 */
    512, /* 11011 */
    384, /* 11100 */
    512, /* 11101 */
    512, /* 11110 */
    640, /* 11111 */
    128, /* 100000 */
    256, /* 100001 */
    256, /* 100010 */
    384, /* 100011 */
    256, /* 100100 */
    384, /* 100101 */
    384, /* 100110 */
    512, /* 100111 */
    256, /* 101000 */
    384, /* 101001 */
    384, /* 101010 */
    512, /* 101011 */
    384, /* 101100 */
    512, /* 101101 */
    512, /* 101110 */
    640, /* 101111 */
    256, /* 110000 */
    384, /* 110001 */
    384, /* 110010 */
    512, /* 110011 */
    384, /* 110100 */
    512, /* 110101 */
    512, /* 110110 */
    640, /* 110111 */
    384, /* 111000 */
    512, /* 111001 */
    512, /* 111010 */
    640, /* 111011 */
    512, /* 111100 */
    640, /* 111101 */
    640, /* 111110 */
    768  /* 111111 */
};

int DEBUG;

static int error_base;
static int event_base;

static void intel_xvmc_debug_init(void)
{
    if (getenv("INTEL_XVMC_DEBUG"))
	DEBUG = 1;
}

/* locking */
static void intel_xvmc_try_heavy_lock(drm_context_t ctx)
{
    drmGetLock(xvmc_driver->fd, ctx, 0);
}

void LOCK_HARDWARE(drm_context_t ctx)
{
    char __ret = 0;

    PPTHREAD_MUTEX_LOCK();
    assert(!xvmc_driver->locked);

    DRM_CAS(xvmc_driver->driHwLock, ctx,
            (DRM_LOCK_HELD | ctx), __ret);

    if (__ret)
	intel_xvmc_try_heavy_lock(ctx);

    xvmc_driver->locked = 1;
}

void UNLOCK_HARDWARE(drm_context_t ctx)
{
    xvmc_driver->locked = 0;
    DRM_UNLOCK(xvmc_driver->fd, xvmc_driver->driHwLock, ctx);
    PPTHREAD_MUTEX_UNLOCK();
}

static intel_xvmc_context_ptr intel_xvmc_new_context(Display *dpy)
{
    intel_xvmc_context_ptr ret;

    ret = (intel_xvmc_context_ptr)calloc(1, sizeof(intel_xvmc_context_t));
    if (!ret)
	return NULL;

    if (!xvmc_driver->ctx_list)
	ret->next = NULL;
    else
	ret->next = xvmc_driver->ctx_list;
    xvmc_driver->ctx_list = ret;
    xvmc_driver->num_ctx++;

    return ret;

}

static void intel_xvmc_free_context(XID id)
{
    intel_xvmc_context_ptr p = xvmc_driver->ctx_list;
    intel_xvmc_context_ptr pre = p;

    while(p) {
	if (p->context && p->context->context_id == id) {
	    if (p == xvmc_driver->ctx_list)
		xvmc_driver->ctx_list = p->next;
	    else
		pre->next = p->next;
	    break;
	}
	pre = p;
	p = p->next;
    }

    if (p) {
	free(p);
	xvmc_driver->num_ctx--;
    }
}

intel_xvmc_context_ptr intel_xvmc_find_context(XID id)
{
    intel_xvmc_context_ptr p = xvmc_driver->ctx_list;

    while(p) {
	if (p->context && p->context->context_id == id)
	    return p;
	p = p->next;
    }
    return NULL;
}

static intel_xvmc_surface_ptr intel_xvmc_new_surface(Display *dpy)
{
    intel_xvmc_surface_ptr ret;

    ret = (intel_xvmc_surface_ptr)calloc(1, sizeof(intel_xvmc_surface_t));
    if (!ret)
	return NULL;

    if (!xvmc_driver->surf_list)
	ret->next = NULL;
    else
	ret->next = xvmc_driver->surf_list;
    xvmc_driver->surf_list = ret;
    xvmc_driver->num_surf++;

    ret->image = NULL;
    ret->gc_init = FALSE;

    return ret;

}

static void intel_xvmc_free_surface(XID id)
{
    intel_xvmc_surface_ptr p = xvmc_driver->surf_list;
    intel_xvmc_surface_ptr pre = p;

    while(p) {
	if (p->surface && p->surface->surface_id == id) {
	    if (p == xvmc_driver->surf_list)
		xvmc_driver->surf_list = p->next;
	    else
		pre->next = p->next;
	    break;
	}
	pre = p;
	p = p->next;
    }

    if (p) {
	free(p);
	xvmc_driver->num_surf--;
    }
}

intel_xvmc_surface_ptr intel_xvmc_find_surface(XID id)
{
    intel_xvmc_surface_ptr p = xvmc_driver->surf_list;

    while(p) {
	if (p->surface && p->surface->surface_id == id)
	    return p;
	p = p->next;
    }
    return NULL;
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
Status XvMCCreateContext(Display *display, XvPortID port,
                         int surface_type_id, int width, int height,
                         int flags, XvMCContext *context)
{
    Status ret;
    drm_sarea_t *pSAREA;
    char *curBusID;
    CARD32 *priv_data = NULL;
    struct _intel_xvmc_common *comm;
    drm_magic_t magic;
    int major, minor;
    int priv_count;
    int isCapable;
    int screen = DefaultScreen(display);
    intel_xvmc_context_ptr intel_ctx;
    int fd;

    /* Verify Obvious things first */
    if (!display || !context)
        return BadValue;

    if (!(flags & XVMC_DIRECT)) {
        XVMC_ERR("Indirect Rendering not supported! Using Direct.");
        return BadValue;
    }

    intel_xvmc_debug_init();

    /* Open DRI Device */
    if((fd = drmOpen("i915", NULL)) < 0) {
        XVMC_ERR("DRM Device could not be opened.");
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
        XVMC_ERR("XvMCQueryVersion Failed, unable to determine protocol version.");
	return ret;
    }

    /* XXX: major and minor could be checked in future for XvMC
     * protocol capability (i.e H.264/AVC decode available)
     */

    /*
      Pass control to the X server to create a drm_context_t for us and
      validate the with/height and flags.
    */
    if ((ret = _xvmc_create_context(display, context, &priv_count, &priv_data))) {
        XVMC_ERR("Unable to create XvMC Context.");
        return ret;
    }
    XVMC_DBG("new context %d created\n", (int)context->context_id);

    comm = (struct _intel_xvmc_common *)priv_data;

    if (xvmc_driver == NULL || xvmc_driver->type != comm->type) {
	switch (comm->type) {
	    case XVMC_I915_MPEG2_MC:
		xvmc_driver = &i915_xvmc_mc_driver;
		break;
	    case XVMC_I965_MPEG2_MC:
	    case XVMC_I945_MPEG2_VLD:
	    case XVMC_I965_MPEG2_VLD:
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

    xvmc_driver->fd = fd;

    XVMC_INFO("decoder type is %s", intel_xvmc_decoder_string(comm->type));

    xvmc_driver->sarea_size = comm->sarea_size;
    xvmc_driver->batchbuffer.handle = comm->batchbuffer.handle;
    xvmc_driver->batchbuffer.offset = comm->batchbuffer.offset;
    xvmc_driver->batchbuffer.size = comm->batchbuffer.size;

    /* assign local ctx info */
    intel_ctx = intel_xvmc_new_context(display);
    if (!intel_ctx) {
	XVMC_ERR("Intel XvMC context create fail\n");
	return BadAlloc;
    }
    intel_ctx->context = context;

    ret = uniDRIQueryDirectRenderingCapable(display, screen,
                                            &isCapable);
    if (!ret || !isCapable) {
	XVMC_ERR("Direct Rendering is not available on this system!");
	XFree(priv_data);
        return BadValue;
    }

    if (!uniDRIOpenConnection(display, screen,
                              &xvmc_driver->hsarea, &curBusID)) {
        XVMC_ERR("Could not open DRI connection to X server!");
	XFree(priv_data);
        return BadValue;
    }

    strncpy(xvmc_driver->busID, curBusID, 20);
    xvmc_driver->busID[20] = '\0';
    XFree(curBusID);

    /* Get magic number */
    drmGetMagic(xvmc_driver->fd, &magic);
    // context->flags = (unsigned long)magic;

    if (!uniDRIAuthConnection(display, screen, magic)) {
	XVMC_ERR("[XvMC]: X server did not allow DRI. Check permissions.");
	xvmc_driver = NULL;
	XFree(priv_data);
        return BadAlloc;
    }

    /*
     * Map DRI Sarea. we always want it right?
     */
    if (drmMap(xvmc_driver->fd, xvmc_driver->hsarea,
               xvmc_driver->sarea_size, &xvmc_driver->sarea_address) < 0) {
        XVMC_ERR("Unable to map DRI SAREA.\n");
	xvmc_driver = NULL;
	XFree(priv_data);
        return BadAlloc;
    }
    pSAREA = (drm_sarea_t *)xvmc_driver->sarea_address;
    xvmc_driver->driHwLock = (drmLock *)&pSAREA->lock;
    pthread_mutex_init(&xvmc_driver->ctxmutex, NULL);

    /* context_id is alloc in _xvmc_create_context */
    if (!uniDRICreateContext(display, screen, DefaultVisual(display, screen),
			     context->context_id,
                             &intel_ctx->hw_context)) {
        XVMC_ERR("Could not create DRI context for xvmc ctx %d.",
		 (int)context->context_id);
	XFree(priv_data);
        context->privData = NULL;
        drmUnmap(xvmc_driver->sarea_address, xvmc_driver->sarea_size);
        return BadAlloc;
    }

    /* call driver hook.
     * driver hook should free priv_data after return if success.*/
    ret = (xvmc_driver->create_context)(display, context, priv_count, priv_data);
    if (ret) {
	XVMC_ERR("driver create context failed\n");
	XFree(priv_data);
	drmUnmap(xvmc_driver->sarea_address, xvmc_driver->sarea_size);
	return ret;
    }

    intelInitBatchBuffer();

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
Status XvMCDestroyContext(Display *display, XvMCContext *context)
{
    Status ret;
    int screen;

    if (!display || !context)
        return XvMCBadContext;
    screen = DefaultScreen(display);
    ret = (xvmc_driver->destroy_context)(display, context);
    if (ret) {
	XVMC_ERR("destroy context fail\n");
	return ret;
    }

    uniDRIDestroyContext(display, screen, context->context_id);
    intel_xvmc_free_context(context->context_id);

    ret = _xvmc_destroy_context(display, context);
    if (ret != Success) {
	XVMC_ERR("_xvmc_destroy_context fail\n");
	return ret;
    }

    if (xvmc_driver->num_ctx == 0) {
	uniDRICloseConnection(display, screen);

	pthread_mutex_destroy(&xvmc_driver->ctxmutex);

	drmUnmap(xvmc_driver->sarea_address, xvmc_driver->sarea_size);

	if (xvmc_driver->fd >= 0)
	    drmClose(xvmc_driver->fd);
	xvmc_driver->fd = -1;

	intelFiniBatchBuffer();

	intel_xvmc_dump_close();
    }
    return Success;
}

/*
 * Function: XvMCCreateSurface
 */
Status XvMCCreateSurface(Display *display, XvMCContext *context, XvMCSurface *surface)
{
    Status ret;
    int priv_count;
    CARD32 *priv_data;
    intel_xvmc_surface_ptr intel_surf = NULL;

    if (!display || !context)
        return XvMCBadContext;

    if (!surface)
	return XvMCBadSurface;

    intel_surf = intel_xvmc_new_surface(display);
    if (!intel_surf)
	return BadAlloc;
    intel_surf->surface = surface;

    if ((ret = _xvmc_create_surface(display, context, surface,
                                    &priv_count, &priv_data))) {
        XVMC_ERR("Unable to create XvMCSurface.");
        return ret;
    }

    intel_surf->image = XvCreateImage(display, context->port,
	    FOURCC_XVMC, (char *)&intel_surf->data, surface->width,
	    surface->height);
    if (!intel_surf->image) {
	XVMC_ERR("Can't create XvImage for surface\n");
	_xvmc_destroy_surface(display, surface);
	intel_xvmc_free_surface(surface->surface_id);
	return BadAlloc;
    }
    intel_surf->image->data = (char *)&intel_surf->data;

    ret = (xvmc_driver->create_surface)(display, context, surface, priv_count,
	    priv_data);
    if (ret) {
	XVMC_ERR("create surface failed\n");
	return ret;
    }

    return Success;
}


/*
 * Function: XvMCDestroySurface
 */
Status XvMCDestroySurface(Display *display, XvMCSurface *surface)
{
    intel_xvmc_surface_ptr intel_surf;

    if (!display || !surface)
        return XvMCBadSurface;

    intel_surf = intel_xvmc_find_surface(surface->surface_id);
    if (!intel_surf)
	return XvMCBadSurface;

    XFree(intel_surf->image);
    if (intel_surf->gc_init)
	XFreeGC(display, intel_surf->gc);
    intel_xvmc_free_surface(surface->surface_id);

    (xvmc_driver->destroy_surface)(display, surface);

    _xvmc_destroy_surface(display, surface);

    return Success;
}

/*
 * Function: XvMCCreateBlocks
 */
Status XvMCCreateBlocks(Display *display, XvMCContext *context,
                        unsigned int num_blocks,
                        XvMCBlockArray *block)
{
    if (!display || !context || !num_blocks || !block)
        return BadValue;

    memset(block, 0, sizeof(XvMCBlockArray));

    if (!(block->blocks = (short *)malloc(num_blocks << 6 * sizeof(short))))
        return BadAlloc;

    block->num_blocks = num_blocks;
    block->context_id = context->context_id;
    block->privData = NULL;

    return Success;
}

/*
 * Function: XvMCDestroyBlocks
 */
Status XvMCDestroyBlocks(Display *display, XvMCBlockArray *block)
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
Status XvMCCreateMacroBlocks(Display *display, XvMCContext *context,
                             unsigned int num_blocks,
                             XvMCMacroBlockArray *blocks)
{
    if (!display || !context || !blocks || !num_blocks)
        return BadValue;

    memset(blocks, 0, sizeof(XvMCMacroBlockArray));
    blocks->macro_blocks = (XvMCMacroBlock *)malloc(num_blocks * sizeof(XvMCMacroBlock));

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
Status XvMCDestroyMacroBlocks(Display *display, XvMCMacroBlockArray *block)
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
Status XvMCRenderSurface(Display *display, XvMCContext *context,
                         unsigned int picture_structure,
                         XvMCSurface *target_surface,
                         XvMCSurface *past_surface,
                         XvMCSurface *future_surface,
                         unsigned int flags,
                         unsigned int num_macroblocks,
                         unsigned int first_macroblock,
                         XvMCMacroBlockArray *macroblock_array,
                         XvMCBlockArray *blocks)
{
    Status ret;

    if (!display || !context) {
        XVMC_ERR("Invalid Display, Context or Target!");
        return XvMCBadContext;
    }
    if (!target_surface)
	return XvMCBadSurface;

    intel_xvmc_dump_render(context, picture_structure, target_surface,
	    past_surface, future_surface, flags, num_macroblocks,
	    first_macroblock, macroblock_array, blocks);

    ret = (xvmc_driver->render_surface)(display, context, picture_structure,
	    target_surface, past_surface, future_surface, flags,
	    num_macroblocks, first_macroblock, macroblock_array,
	    blocks);

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
Status XvMCPutSurface(Display *display,XvMCSurface *surface,
                      Drawable draw, short srcx, short srcy,
                      unsigned short srcw, unsigned short srch,
                      short destx, short desty,
                      unsigned short destw, unsigned short desth,
                      int flags)
{
    Status ret = Success;
    XvMCContext *context;
    intel_xvmc_context_ptr intel_ctx;
    intel_xvmc_surface_ptr intel_surf;

    if (!display || !surface)
        return XvMCBadSurface;

    intel_ctx = intel_xvmc_find_context(surface->context_id);
    intel_surf = intel_xvmc_find_surface(surface->surface_id);
    if (!intel_ctx || !intel_surf)
	return XvMCBadSurface;
    context = intel_ctx->context;

    if (intel_surf->gc_init == FALSE) {
	intel_surf->gc = XCreateGC(display, draw, 0, NULL);
	intel_surf->gc_init = TRUE;
    } else if (draw != intel_surf->last_draw) {
	XFreeGC(display, intel_surf->gc);
	intel_surf->gc = XCreateGC(display, draw, 0, NULL);
    }
    intel_surf->last_draw = draw;

    /* fill intel_surf->data */
    ret = (xvmc_driver->put_surface)(display, surface, draw, srcx, srcy,
	    srcw, srch, destx, desty, destw, desth, flags, &intel_surf->data);
    if (ret) {
	XVMC_ERR("put surface fail\n");
	return ret;
    }

    ret = XvPutImage(display, context->port, draw, intel_surf->gc,
	    intel_surf->image, srcx, srcy, srcw, srch, destx, desty,
	    destw, desth);

    return ret;
}

/*
 * Function: XvMCSyncSurface
 * Arguments:
 *   display - Connection to the X server
 *   surface - The surface to synchronize
 */
Status XvMCSyncSurface(Display *display, XvMCSurface *surface)
{
    Status ret;
    int stat = 0;

    if (!display || !surface)
	return XvMCBadSurface;

    do {
        ret = XvMCGetSurfaceStatus(display, surface, &stat);
    } while (!ret && (stat & XVMC_RENDERING));

    return ret;
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
Status XvMCFlushSurface(Display * display, XvMCSurface *surface)
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
Status XvMCGetSurfaceStatus(Display *display, XvMCSurface *surface, int *stat)
{
    Status ret;

    if (!display || !surface || !stat)
        return XvMCBadSurface;

    ret = (xvmc_driver->get_surface_status)(display, surface, stat);
    if (ret) {
	XVMC_ERR("get surface status fail\n");
	return ret;
    }

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
Status XvMCHideSurface(Display *display, XvMCSurface *surface)
{
    int stat = 0;
    Status ret;

    if (!display || !surface)
        return XvMCBadSurface;

    XvMCSyncSurface(display, surface);

    /*
      Get the status of the surface, if it is not currently displayed
      we don't need to worry about it.
    */
    if ((ret = XvMCGetSurfaceStatus(display, surface, &stat)) != Success)
        return ret;

    if (!(stat & XVMC_DISPLAYING))
        return Success;

    /* FIXME: */
    XVMC_ERR("XvMCHideSurface not implemented!\n");
    return BadValue;
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
Status XvMCCreateSubpicture(Display *display, XvMCContext *context,
                            XvMCSubpicture *subpicture,
                            unsigned short width, unsigned short height,
                            int xvimage_id)
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
Status XvMCClearSubpicture(Display *display, XvMCSubpicture *subpicture,
                           short x, short y,
                           unsigned short width, unsigned short height,
                           unsigned int color)
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
Status XvMCCompositeSubpicture(Display *display, XvMCSubpicture *subpicture,
                               XvImage *image,
                               short srcx, short srcy,
                               unsigned short width, unsigned short height,
                               short dstx, short dsty)
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
Status XvMCDestroySubpicture(Display *display, XvMCSubpicture *subpicture)
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
Status XvMCSetSubpicturePalette(Display *display, XvMCSubpicture *subpicture,
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
Status XvMCBlendSubpicture(Display *display, XvMCSurface *target_surface,
                           XvMCSubpicture *subpicture,
                           short subx, short suby,
                           unsigned short subw, unsigned short subh,
                           short surfx, short surfy,
                           unsigned short surfw, unsigned short surfh)
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
Status XvMCBlendSubpicture2(Display *display,
                            XvMCSurface *source_surface,
                            XvMCSurface *target_surface,
                            XvMCSubpicture *subpicture,
                            short subx, short suby,
                            unsigned short subw, unsigned short subh,
                            short surfx, short surfy,
                            unsigned short surfw, unsigned short surfh)
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
Status XvMCSyncSubpicture(Display *display, XvMCSubpicture *subpicture)
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
Status XvMCFlushSubpicture(Display *display, XvMCSubpicture *subpicture)
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
Status XvMCGetSubpictureStatus(Display *display, XvMCSubpicture *subpicture,
                               int *stat)
{
    XVMC_ERR("XvMCGetSubpictureStatus not implemented!");
    return BadValue;
}

/*
 * Function: XvMCQueryAttributes
 * Description: An array of XvAttributes of size "number" is returned by
 *   this function. If there are no attributes, NULL is returned and number
 *   is set to 0. The array may be freed with xfree().
 *
 * Arguments:
 *   display - Connection to the X server.
 *   context - The context whos attributes we are querying.
 *   number - The returned number of recognized atoms
 *
 * Returns:
 *  An array of XvAttributes.
 */
XvAttribute *XvMCQueryAttributes(Display *display, XvMCContext *context,
                                 int *number)
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
Status XvMCSetAttribute(Display *display, XvMCContext *context,
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
Status XvMCGetAttribute(Display *display, XvMCContext *context,
                        Atom attribute, int *value)
{
    return Success;
}
