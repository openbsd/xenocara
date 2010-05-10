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

#include "i830.h"
#include "i830_dri.h"
#define _INTEL_XVMC_SERVER_
#include "i830_hwmc.h"
#include "i965_hwmc.h"
#include "intel_bufmgr.h"

#define STRIDE(w)               (w)
#define SIZE_YUV420(w, h)       (h * (STRIDE(w) + STRIDE(w >> 1)))
#define VLD_MAX_SLICE_LEN	(32*1024)

#ifndef XVMC_VLD
#define XVMC_VLD  0x00020000
#endif

static PutImageFuncPtr savedXvPutImage;

static int create_context(ScrnInfoPtr scrn,
			  XvMCContextPtr context, int *num_privates,
			  CARD32 ** private)
{
	struct i965_xvmc_context *private_context, *context_dup;
	intel_screen_private *intel = intel_get_screen_private(scrn);

	unsigned int blocknum =
	    (((context->width + 15) / 16) * ((context->height + 15) / 16));
	unsigned int blocksize = 6 * blocknum * 64 * sizeof(short);
	blocksize = (blocksize + 4095) & (~4095);
	if ((private_context = Xcalloc(sizeof(*private_context))) == NULL) {
		ErrorF("XVMC Can not allocate private context\n");
		return BadAlloc;
	}

	if ((context_dup = Xcalloc(sizeof(*private_context))) == NULL) {
		ErrorF("XVMC Can not allocate private context\n");
		return BadAlloc;
	}

	private_context->is_g4x = IS_G4X(intel);
	private_context->is_965_q = IS_965_Q(intel);
	private_context->is_igdng = IS_IGDNG(intel);
	private_context->comm.kernel_exec_fencing = 1;
	private_context->comm.type = xvmc_driver->flag;

	*num_privates = sizeof(*private_context) / sizeof(CARD32);
	*private = (CARD32 *) private_context;
	memcpy(context_dup, private_context, sizeof(*private_context));
	context->driver_priv = context_dup;

	return Success;
}

static void destroy_context(ScrnInfoPtr scrn, XvMCContextPtr context)
{
	struct i965_xvmc_context *private_context;
	private_context = context->driver_priv;
	Xfree(private_context);
}

static int create_surface(ScrnInfoPtr scrn, XvMCSurfacePtr surface,
			  int *num_priv, CARD32 ** priv)
{
	XvMCContextPtr ctx = surface->context;

	struct i965_xvmc_surface *priv_surface, *surface_dup;
	struct i965_xvmc_context *priv_ctx = ctx->driver_priv;
	int i;
	for (i = 0; i < I965_MAX_SURFACES; i++) {
		if (priv_ctx->surfaces[i] == NULL) {
			priv_surface = Xcalloc(sizeof(*priv_surface));
			if (priv_surface == NULL)
				return BadAlloc;
			surface_dup = Xcalloc(sizeof(*priv_surface));
			if (surface_dup == NULL)
				return BadAlloc;

			priv_surface->no = i;
			priv_surface->handle = priv_surface;
			priv_surface->w = ctx->width;
			priv_surface->h = ctx->height;
			priv_ctx->surfaces[i] = surface->driver_priv
			    = priv_surface;
			memcpy(surface_dup, priv_surface,
			       sizeof(*priv_surface));
			*num_priv = sizeof(*priv_surface) / sizeof(CARD32);
			*priv = (CARD32 *) surface_dup;
			break;
		}
	}

	if (i >= I965_MAX_SURFACES) {
		ErrorF("I965 XVMC too many surfaces in one context\n");
		return BadAlloc;
	}

	return Success;
}

static void destory_surface(ScrnInfoPtr scrn, XvMCSurfacePtr surface)
{
	XvMCContextPtr ctx = surface->context;
	struct i965_xvmc_surface *priv_surface = surface->driver_priv;
	struct i965_xvmc_context *priv_ctx = ctx->driver_priv;
	priv_ctx->surfaces[priv_surface->no] = NULL;
	Xfree(priv_surface);
}

static int create_subpicture(ScrnInfoPtr scrn, XvMCSubpicturePtr subpicture,
			     int *num_priv, CARD32 ** priv)
{
	return Success;
}

static void destroy_subpicture(ScrnInfoPtr scrn, XvMCSubpicturePtr subpicture)
{
}

static int put_image(ScrnInfoPtr scrn,
		     short src_x, short src_y,
		     short drw_x, short drw_y, short src_w,
		     short src_h, short drw_w, short drw_h,
		     int id, unsigned char *buf, short width,
		     short height, Bool sync, RegionPtr clipBoxes, pointer data,
		     DrawablePtr drawable)
{
	struct intel_xvmc_command *cmd = (struct intel_xvmc_command *)buf;

	if (id == FOURCC_XVMC) {
		/* Pass the GEM object name through the pointer arg. */
		buf = (void *)(uintptr_t)cmd->handle;
	}

	savedXvPutImage(scrn, src_x, src_y, drw_x, drw_y, src_w, src_h,
			drw_w, drw_h, id, buf,
			width, height, sync, clipBoxes,
			data, drawable);

	return Success;
}

static Bool init(ScrnInfoPtr screen_info, XF86VideoAdaptorPtr adaptor)
{
	savedXvPutImage = adaptor->PutImage;
	adaptor->PutImage = put_image;

	return TRUE;
}

static void fini(ScrnInfoPtr screen_info)
{
}

static XF86MCSurfaceInfoRec yv12_mpeg2_vld_surface = {
	FOURCC_YV12,
	XVMC_CHROMA_FORMAT_420,
	0,
	1936,
	1096,
	1920,
	1080,
	XVMC_MPEG_2 | XVMC_VLD,
	XVMC_INTRA_UNSIGNED,
	NULL
};

static XF86MCSurfaceInfoRec yv12_mpeg2_surface = {
	FOURCC_YV12,
	XVMC_CHROMA_FORMAT_420,
	0,
	1936,
	1096,
	1920,
	1080,
	XVMC_MPEG_2 | XVMC_MOCOMP,
	/* XVMC_OVERLAID_SURFACE | XVMC_SUBPICTURE_INDEPENDENT_SCALING, */
	XVMC_INTRA_UNSIGNED,
	/* &yv12_subpicture_list */
	NULL
};

static XF86MCSurfaceInfoRec yv12_mpeg1_surface = {
	FOURCC_YV12,
	XVMC_CHROMA_FORMAT_420,
	0,
	1920,
	1080,
	1920,
	1080,
	XVMC_MPEG_1 | XVMC_MOCOMP,
	/*XVMC_OVERLAID_SURFACE | XVMC_SUBPICTURE_INDEPENDENT_SCALING |
	   XVMC_INTRA_UNSIGNED, */
	XVMC_INTRA_UNSIGNED,

	/*&yv12_subpicture_list */
	NULL
};

static XF86MCSurfaceInfoPtr surface_info[] = {
	&yv12_mpeg2_surface,
	&yv12_mpeg1_surface
};

static XF86MCSurfaceInfoPtr surface_info_vld[] = {
	&yv12_mpeg2_vld_surface,
	&yv12_mpeg2_surface,
};

static XF86MCAdaptorRec adaptor_vld = {
	.name = "Intel(R) Textured Video",
	.num_surfaces = sizeof(surface_info_vld) / sizeof(surface_info_vld[0]),
	.surfaces = surface_info_vld,

	.CreateContext = create_context,
	.DestroyContext = destroy_context,
	.CreateSurface = create_surface,
	.DestroySurface = destory_surface,
	.CreateSubpicture = create_subpicture,
	.DestroySubpicture = destroy_subpicture
};

static XF86MCAdaptorRec adaptor = {
	.name = "Intel(R) Textured Video",
	.num_surfaces = sizeof(surface_info) / sizeof(surface_info[0]),
	.surfaces = surface_info,

	.CreateContext = create_context,
	.DestroyContext = destroy_context,
	.CreateSurface = create_surface,
	.DestroySurface = destory_surface,
	.CreateSubpicture = create_subpicture,
	.DestroySubpicture = destroy_subpicture
};

struct intel_xvmc_driver i965_xvmc_driver = {
	.name = "i965_xvmc",
	.adaptor = &adaptor,
	.flag = XVMC_I965_MPEG2_MC,
	.init = init,
	.fini = fini
};

struct intel_xvmc_driver vld_xvmc_driver = {
	.name = "xvmc_vld",
	.adaptor = &adaptor_vld,
	.flag = XVMC_I965_MPEG2_VLD,
	.init = init,
	.fini = fini
};
