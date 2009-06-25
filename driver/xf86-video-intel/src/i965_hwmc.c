/*
 * Copyright © 2008 Intel Corporation
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
 * Author:
 *    Zou Nan hai <nanhai.zou@intel.com>
 *
 */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <X11/extensions/Xv.h>
#include <X11/extensions/XvMC.h>
#include <fourcc.h>
#include <errno.h>

#include "i830.h"
#include "i830_dri.h"
#define _INTEL_XVMC_SERVER_
#include "i830_hwmc.h"
#include "i965_hwmc.h"

#define STRIDE(w)               (w)
#define SIZE_YUV420(w, h)       (h * (STRIDE(w) + STRIDE(w >> 1)))

static PutImageFuncPtr XvPutImage;

#if 0
static int alloc_drm_memory_tiled(ScrnInfoPtr pScrn, 
	struct drm_memory_block *mem,
	char *name, size_t size, unsigned long pitch, unsigned long alignment)
{
    I830Ptr pI830 = I830PTR(pScrn);
    if ((mem->buffer = i830_allocate_memory(pScrn,
	    name, size, pitch,
	    GTT_PAGE_SIZE, ALIGN_BOTH_ENDS, TILE_XMAJOR)) == NULL) {
	ErrorF("Fail to alloc \n");
	return BadAlloc;
    }

    if (drmAddMap(pI830->drmSubFD,
                  (drm_handle_t)(mem->buffer->offset + pI830->LinearAddr),
                  size, DRM_AGP, 0,
                  (drmAddress)&mem->handle) < 0) {
	ErrorF("Fail to map %d \n", errno);
	i830_free_memory(pScrn, mem->buffer);
	return BadAlloc;
    }

    mem->size = size;
    mem->offset = mem->buffer->offset;
    return Success;
}
#endif

static int alloc_drm_memory(ScrnInfoPtr pScrn, 
	struct drm_memory_block *mem, 
	char *name, size_t size)
{
    I830Ptr pI830 = I830PTR(pScrn);
    if ((mem->buffer = i830_allocate_memory(pScrn, 
	    name, size, PITCH_NONE, GTT_PAGE_SIZE,
	    ALIGN_BOTH_ENDS, TILE_NONE)) == NULL) {
	ErrorF("Fail to alloc \n");
	return BadAlloc;
    }

    if (drmAddMap(pI830->drmSubFD,
                  (drm_handle_t)(mem->buffer->offset + pI830->LinearAddr),
                  size, DRM_AGP, 0,
                  (drmAddress)&mem->handle) < 0) {
	ErrorF("Fail to map %d \n", errno);
	i830_free_memory(pScrn, mem->buffer);
	return BadAlloc;
    }

    mem->size = size;
    mem->offset = mem->buffer->offset;
    return Success;
}

static void free_drm_memory(ScrnInfoPtr pScrn,
		struct drm_memory_block *mem)
{
    I830Ptr pI830 = I830PTR(pScrn);
    drmRmMap(pI830->drmSubFD, mem->handle);
    i830_free_memory(pScrn, mem->buffer);
}

static int create_context(ScrnInfoPtr pScrn, 
	XvMCContextPtr context, int *num_privates, CARD32 **private)
{
        struct i965_xvmc_context *private_context, *context_dup;
	I830Ptr I830 = I830PTR(pScrn);
    	DRIInfoPtr driinfo = I830->pDRIInfo;

	unsigned int blocknum = 
		(((context->width + 15)/16)*((context->height+15)/16));
	unsigned int blocksize = 6*blocknum*64*sizeof(short);
	blocksize = (blocksize + 4095)&(~4095);
	if ((private_context = Xcalloc(sizeof(*private_context))) == NULL) {
		ErrorF("XVMC Can not allocate private context\n");
		return BadAlloc;
	}

	if ((context_dup = Xcalloc(sizeof(*private_context))) == NULL) {
		ErrorF("XVMC Can not allocate private context\n");
		return BadAlloc;
	}

	private_context->is_g4x = IS_G4X(I830);
	private_context->is_965_q = IS_965_Q(I830);
	private_context->comm.type = xvmc_driver->flag;
	private_context->comm.sarea_size = driinfo->SAREASize;
	private_context->comm.batchbuffer.offset = xvmc_driver->batch->offset;
	private_context->comm.batchbuffer.size = xvmc_driver->batch->size;
	private_context->comm.batchbuffer.handle = xvmc_driver->batch_handle;

	if (alloc_drm_memory(pScrn, &private_context->static_buffer,
		"XVMC static buffers", 
		I965_MC_STATIC_BUFFER_SIZE)) {
	    ErrorF("Unable to allocate and map static buffer for XVMC\n");	
	    return BadAlloc;
	}

	if (alloc_drm_memory(pScrn, &private_context->blocks,
		"XVMC blocks", blocksize)) {
	    ErrorF("Unable to allocate and map block buffer for XVMC\n");	
	    return BadAlloc;
	}
	*num_privates = sizeof(*private_context)/sizeof(CARD32);
	*private = (CARD32 *)private_context;
	memcpy(context_dup, private_context, sizeof(*private_context));
	context->driver_priv = context_dup;

	return Success;
}

static void destroy_context(ScrnInfoPtr pScrn, XvMCContextPtr context)
{
    struct i965_xvmc_context *private_context;
    private_context = context->driver_priv;
    free_drm_memory(pScrn, &private_context->static_buffer);
    Xfree(private_context);
}

static int create_surface(ScrnInfoPtr pScrn, XvMCSurfacePtr surface,
  int *num_priv, CARD32 **priv)
{
	XvMCContextPtr ctx = surface->context;
	struct i965_xvmc_surface *priv_surface, *surface_dup;
	struct i965_xvmc_context *priv_ctx = ctx->driver_priv;
	size_t bufsize = SIZE_YUV420(ctx->width, ctx->height);
	int i;
	for (i = 0 ; i < I965_MAX_SURFACES; i++) {
	    if (priv_ctx->surfaces[i] == NULL) {
		priv_surface = Xcalloc(sizeof(*priv_surface));
		if (priv_surface == NULL)
		    return BadAlloc;
		surface_dup = Xcalloc(sizeof(*priv_surface));
		if (surface_dup == NULL)
		    return BadAlloc;
		
		priv_surface->no = i;
		priv_surface->handle = priv_surface;
		priv_ctx->surfaces[i] = surface->driver_priv 
		    = priv_surface;
		if (alloc_drm_memory(pScrn, &priv_surface->buffer,
			    "surface buffer\n", (bufsize+0xFFF)&~(0xFFF))) {
		        ErrorF("Unable to allocate surface buffer\n");
            		return BadAlloc;
        	}
		memcpy(surface_dup, priv_surface, sizeof(*priv_surface));
		*num_priv = sizeof(*priv_surface)/sizeof(CARD32);
		*priv = (CARD32 *)surface_dup;
		break;
	    }
	}

	if (i >= I965_MAX_SURFACES) {
	    ErrorF("I965 XVMC too many surfaces in one context\n");
	    return BadAlloc;
	}
	
	return Success;
}

static void destory_surface(ScrnInfoPtr pScrn, XvMCSurfacePtr surface)
{
	XvMCContextPtr ctx = surface->context;
	struct i965_xvmc_surface *priv_surface = surface->driver_priv; 
	struct i965_xvmc_context *priv_ctx = ctx->driver_priv;
	priv_ctx->surfaces[priv_surface->no] = NULL;
	free_drm_memory(pScrn, &priv_surface->buffer);
	Xfree(priv_surface);
}

static int create_subpicture(ScrnInfoPtr pScrn, XvMCSubpicturePtr subpicture,
  int *num_priv, CARD32 **priv)
{
	return Success;
}

static void destroy_subpicture(ScrnInfoPtr pScrn, XvMCSubpicturePtr subpicture)
{
}
static int put_image(ScrnInfoPtr pScrn,
        short src_x, short src_y,
        short drw_x, short drw_y, short src_w,
        short src_h, short drw_w, short drw_h,
        int id, unsigned char *buf, short width,
        short height, Bool sync, RegionPtr clipBoxes, pointer data,
        DrawablePtr pDraw)
{
	I830Ptr pI830 = I830PTR(pScrn);
	struct intel_xvmc_command *cmd = (struct intel_xvmc_command *)buf;
	if (id == FOURCC_XVMC) {
	    buf = pI830->FbBase + cmd->surf_offset;
	}
	XvPutImage(pScrn, src_x, src_y, drw_x, drw_y, src_w, src_h,
		drw_w, drw_h, id, buf, width, height, sync, clipBoxes,
		data, pDraw);
	return Success;
}

static Bool init(ScrnInfoPtr screen_info, XF86VideoAdaptorPtr adaptor)
{
    if (!intel_xvmc_init_batch(screen_info)) {
	ErrorF("[XvMC] fail to init batch buffer\n");
	return FALSE;
    }
    XvPutImage = adaptor->PutImage;
    adaptor->PutImage = put_image;

    return TRUE;
}

static void fini(ScrnInfoPtr screen_info)
{
}

static XF86MCSurfaceInfoRec yv12_mpeg2_surface =
{
    FOURCC_YV12,
    XVMC_CHROMA_FORMAT_420,
    0,
    1936,
    1096,
    1920,
    1080,
    XVMC_MPEG_2|XVMC_MOCOMP,
    /* XVMC_OVERLAID_SURFACE | XVMC_SUBPICTURE_INDEPENDENT_SCALING,*/
    XVMC_INTRA_UNSIGNED,
    /* &yv12_subpicture_list*/
    NULL
};

static XF86MCSurfaceInfoRec yv12_mpeg1_surface =
{
    FOURCC_YV12,
    XVMC_CHROMA_FORMAT_420,
    0,
    1920,
    1080,
    1920,
    1080,
    XVMC_MPEG_1|XVMC_MOCOMP,
    /*XVMC_OVERLAID_SURFACE | XVMC_SUBPICTURE_INDEPENDENT_SCALING |
    XVMC_INTRA_UNSIGNED,*/
    XVMC_INTRA_UNSIGNED,
    	
    /*&yv12_subpicture_list*/
    NULL
};

static XF86MCSurfaceInfoPtr surface_info[] = {
    &yv12_mpeg2_surface,
    &yv12_mpeg1_surface
};

static XF86MCAdaptorRec adaptor = {
    .name               = "Intel(R) Textured Video",
    .num_surfaces       = sizeof(surface_info)/sizeof(surface_info[0]),
    .surfaces           = surface_info,

    .CreateContext 	= create_context,
    .DestroyContext	= destroy_context,
    .CreateSurface 	= create_surface, 
    .DestroySurface 	= destory_surface,
    .CreateSubpicture   = create_subpicture,
    .DestroySubpicture  = destroy_subpicture
};

struct intel_xvmc_driver i965_xvmc_driver = {
    .name               = "i965_xvmc",
    .adaptor            = &adaptor,
    .flag               = XVMC_I965_MPEG2_MC,
    .init 		= init,
    .fini		= fini
};

