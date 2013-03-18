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
 *    Zhenyu Wang <zhenyu.z.wang@sna.com>
 *
 */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#define _SNA_XVMC_SERVER_
#include "sna.h"
#include "sna_video_hwmc.h"

#include <X11/extensions/Xv.h>
#include <X11/extensions/XvMC.h>
#include <fourcc.h>

extern DevPrivateKey XF86XvScreenKey;

static int create_subpicture(XvMCSubpicturePtr sub, int *size, CARD32 **priv)
{
	return Success;
}

static void destroy_subpicture(XvMCSubpicturePtr sub)
{
}

static int create_surface(XvMCSurfacePtr surface, int *size, CARD32 **priv)
{
	return Success;
}

static void destroy_surface(XvMCSurfacePtr surface)
{
}

static int create_context(XvPortPtr port, XvMCContextPtr ctx,
			  int *size, CARD32 **out)
{
	struct sna *sna = to_sna_from_screen(ctx->pScreen);
	struct intel_xvmc_hw_context {
		unsigned int type;
		union {
			struct {
				unsigned int use_phys_addr : 1;
			} i915;
			struct {
				unsigned int is_g4x:1;
				unsigned int is_965_q:1;
				unsigned int is_igdng:1;
			} i965;
		};
	} *priv;

	ctx->port_priv = port->devPriv.ptr;

	priv = calloc(1, sizeof(*priv));
	if (priv == NULL)
		return BadAlloc;

	if (sna->kgem.gen >= 040) {
		if (sna->kgem.gen >= 045)
			priv->type = XVMC_I965_MPEG2_VLD;
		else
			priv->type = XVMC_I965_MPEG2_MC;
		priv->i965.is_g4x = sna->kgem.gen == 045;
		priv->i965.is_965_q = IS_965_Q(sna);
		priv->i965.is_igdng = sna->kgem.gen == 050;
	} else
		priv->type = XVMC_I915_MPEG2_MC;

	*size = sizeof(*priv) >> 2;
	*out = priv;
	return Success;
}

static void destroy_context(XvMCContextPtr ctx)
{
}

/* i915 hwmc support */
static XvMCSurfaceInfoRec i915_YV12_mpg2_surface = {
	FOURCC_YV12,
	XVMC_CHROMA_FORMAT_420,
	0,
	720,
	576,
	720,
	576,
	XVMC_MPEG_2,
	/* XVMC_OVERLAID_SURFACE | XVMC_SUBPICTURE_INDEPENDENT_SCALING, */
	0,
	/* &yv12_subpicture_list */
	NULL,
};

static XvMCSurfaceInfoRec i915_YV12_mpg1_surface = {
	FOURCC_YV12,
	XVMC_CHROMA_FORMAT_420,
	0,
	720,
	576,
	720,
	576,
	XVMC_MPEG_1,
	/* XVMC_OVERLAID_SURFACE | XVMC_SUBPICTURE_INDEPENDENT_SCALING, */
	0,
	NULL,
};

static XvMCSurfaceInfoPtr surface_info_i915[2] = {
	&i915_YV12_mpg2_surface,
	&i915_YV12_mpg1_surface
};

/* i965 and later hwmc support */
#ifndef XVMC_VLD
#define XVMC_VLD  0x00020000
#endif

static XvMCSurfaceInfoRec yv12_mpeg2_vld_surface = {
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

static XvMCSurfaceInfoRec yv12_mpeg2_i965_surface = {
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

static XvMCSurfaceInfoRec yv12_mpeg1_i965_surface = {
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

static XvMCSurfaceInfoPtr surface_info_i965[] = {
	&yv12_mpeg2_i965_surface,
	&yv12_mpeg1_i965_surface
};

static XvMCSurfaceInfoPtr surface_info_vld[] = {
	&yv12_mpeg2_vld_surface,
	&yv12_mpeg2_i965_surface,
};

/* check chip type and load xvmc driver */
Bool sna_video_xvmc_setup(struct sna *sna,
			  ScreenPtr screen,
			  XF86VideoAdaptorPtr target)
{
	XvMCAdaptorRec *adaptors;
	XvScreenPtr xv;
	const char *name;
	char bus[64];
	int i;

	if (!xf86LoaderCheckSymbol("XvMCScreenInit"))
		return FALSE;

	/* Needs KMS support. */
	if (sna->kgem.gen < 031)
		return FALSE;

	/* Not implemented */
	if (sna->kgem.gen >= 060)
		return FALSE;

	adaptors = calloc(1, sizeof(XvMCAdaptorRec));
	if (adaptors == NULL)
		return FALSE;

	xv = dixLookupPrivate(&screen->devPrivates, XF86XvScreenKey);
	for (i = 0; i< xv->nAdaptors;i++) {
		if (strcmp(xv->pAdaptors[i].name, target->name) == 0) {
			adaptors->xv_adaptor = &xv->pAdaptors[i];
			break;
		}
	}
	assert(adaptors->xv_adaptor);

	adaptors->num_subpictures = 0;
	adaptors->subpictures = NULL;
	adaptors->CreateContext = create_context;
	adaptors->DestroyContext = destroy_context;
	adaptors->CreateSurface = create_surface;
	adaptors->DestroySurface = destroy_surface;
	adaptors->CreateSubpicture =  create_subpicture;
	adaptors->DestroySubpicture = destroy_subpicture;

	if (sna->kgem.gen >= 045) {
		name = "xvmc_vld",
		adaptors->num_surfaces = ARRAY_SIZE(surface_info_vld);
		adaptors->surfaces = surface_info_vld;
	} else if (sna->kgem.gen >= 040) {
		name = "i965_xvmc",
		adaptors->num_surfaces = ARRAY_SIZE(surface_info_i965);
		adaptors->surfaces = surface_info_i965;
	} else {
		name = "i915_xvmc",
		adaptors->num_surfaces = ARRAY_SIZE(surface_info_i915);
		adaptors->surfaces = surface_info_i915;
	}

	if (XvMCScreenInit(screen, 1, adaptors) != Success) {
		xf86DrvMsg(sna->scrn->scrnIndex, X_INFO,
			   "[XvMC] Failed to initialize XvMC.\n");
		free(adaptors);
		return FALSE;
	}

	sprintf(bus, "pci:%04x:%02x:%02x.%d",
		sna->PciInfo->domain,
		sna->PciInfo->bus, sna->PciInfo->dev, sna->PciInfo->func);

	xf86XvMCRegisterDRInfo(screen, SNA_XVMC_LIBNAME, bus,
			       SNA_XVMC_MAJOR, SNA_XVMC_MINOR,
			       SNA_XVMC_PATCHLEVEL);

	xf86DrvMsg(sna->scrn->scrnIndex, X_INFO,
		   "[XvMC] %s driver initialized.\n",
		   name);
	return TRUE;
}
