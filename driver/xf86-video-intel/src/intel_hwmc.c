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
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#define _INTEL_XVMC_SERVER_
#include "intel.h"
#include "intel_hwmc.h"

#include <X11/extensions/Xv.h>
#include <X11/extensions/XvMC.h>
#include <fourcc.h>

static int create_subpicture(ScrnInfoPtr scrn, XvMCSubpicturePtr subpicture,
			     int *num_priv, CARD32 ** priv)
{
	return Success;
}

static void destroy_subpicture(ScrnInfoPtr scrn, XvMCSubpicturePtr subpicture)
{
}

static int create_surface(ScrnInfoPtr scrn, XvMCSurfacePtr surface,
			  int *num_priv, CARD32 ** priv)
{
	return Success;
}

static void destroy_surface(ScrnInfoPtr scrn, XvMCSurfacePtr surface)
{
}

static int create_context(ScrnInfoPtr scrn, XvMCContextPtr pContext,
				    int *num_priv, CARD32 **priv)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);
	struct intel_xvmc_hw_context *contextRec;

	*priv = calloc(1, sizeof(struct intel_xvmc_hw_context));
	contextRec = (struct intel_xvmc_hw_context *) *priv;
	if (!contextRec) {
		*num_priv = 0;
		return BadAlloc;
	}

	*num_priv = sizeof(struct intel_xvmc_hw_context) >> 2;

	if (IS_GEN3(intel)) {
		contextRec->type = XVMC_I915_MPEG2_MC;
		contextRec->i915.use_phys_addr = 0;
	} else {
		if (INTEL_INFO(intel)->gen >= 045)
			contextRec->type = XVMC_I965_MPEG2_VLD;
		else
			contextRec->type = XVMC_I965_MPEG2_MC;
		contextRec->i965.is_g4x = INTEL_INFO(intel)->gen == 045;
		contextRec->i965.is_965_q = IS_965_Q(intel);
		contextRec->i965.is_igdng = IS_GEN5(intel);
	}

	return Success;
}

static void destroy_context(ScrnInfoPtr scrn, XvMCContextPtr context)
{
}

/* i915 hwmc support */
static XF86MCSurfaceInfoRec i915_YV12_mpg2_surface = {
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

static XF86MCSurfaceInfoRec i915_YV12_mpg1_surface = {
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

static XF86MCSurfaceInfoPtr surface_info_i915[2] = {
	(XF86MCSurfaceInfoPtr) & i915_YV12_mpg2_surface,
	(XF86MCSurfaceInfoPtr) & i915_YV12_mpg1_surface
};

/* i965 and later hwmc support */
#ifndef XVMC_VLD
#define XVMC_VLD  0x00020000
#endif

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

static XF86MCSurfaceInfoRec yv12_mpeg2_i965_surface = {
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

static XF86MCSurfaceInfoRec yv12_mpeg1_i965_surface = {
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

static XF86MCSurfaceInfoPtr surface_info_i965[] = {
	&yv12_mpeg2_i965_surface,
	&yv12_mpeg1_i965_surface
};

static XF86MCSurfaceInfoPtr surface_info_vld[] = {
	&yv12_mpeg2_vld_surface,
	&yv12_mpeg2_i965_surface,
};

/* check chip type and load xvmc driver */
Bool intel_xvmc_adaptor_init(ScreenPtr pScreen)
{
	ScrnInfoPtr scrn = xf86ScreenToScrn(pScreen);
	intel_screen_private *intel = intel_get_screen_private(scrn);
	static XF86MCAdaptorRec *pAdapt;
	char *name;
	char buf[64];

	if (!intel->XvMCEnabled)
		return FALSE;

	/* Needs KMS support. */
	if (IS_I915G(intel) || IS_I915GM(intel))
		return FALSE;

	if (IS_GEN2(intel)) {
		ErrorF("Your chipset doesn't support XvMC.\n");
		return FALSE;
	}

	pAdapt = calloc(1, sizeof(XF86MCAdaptorRec));
	if (!pAdapt) {
		ErrorF("Allocation error.\n");
		return FALSE;
	}

	pAdapt->name = "Intel(R) Textured Video";
	pAdapt->num_subpictures = 0;
	pAdapt->subpictures = NULL;
	pAdapt->CreateContext = create_context;
	pAdapt->DestroyContext = destroy_context;
	pAdapt->CreateSurface = create_surface;
	pAdapt->DestroySurface = destroy_surface;
	pAdapt->CreateSubpicture =  create_subpicture;
	pAdapt->DestroySubpicture = destroy_subpicture;

	if (IS_GEN3(intel)) {
		name = "i915_xvmc",
		pAdapt->num_surfaces = ARRAY_SIZE(surface_info_i915);
		pAdapt->surfaces = surface_info_i915;
	} else if (INTEL_INFO(intel)->gen >= 045) {
		name = "xvmc_vld",
		pAdapt->num_surfaces = ARRAY_SIZE(surface_info_vld);
		pAdapt->surfaces = surface_info_vld;
	} else {
		name = "i965_xvmc",
		pAdapt->num_surfaces = ARRAY_SIZE(surface_info_i965);
		pAdapt->surfaces = surface_info_i965;
	}

	if (xf86XvMCScreenInit(pScreen, 1, &pAdapt)) {
		xf86DrvMsg(scrn->scrnIndex, X_INFO,
			   "[XvMC] %s driver initialized.\n",
			   name);
	} else {
		intel->XvMCEnabled = FALSE;
		xf86DrvMsg(scrn->scrnIndex, X_INFO,
			   "[XvMC] Failed to initialize XvMC.\n");
		return FALSE;
	}

	sprintf(buf, "pci:%04x:%02x:%02x.%d",
		intel->PciInfo->domain,
		intel->PciInfo->bus, intel->PciInfo->dev, intel->PciInfo->func);

	xf86XvMCRegisterDRInfo(pScreen, INTEL_XVMC_LIBNAME,
			       buf,
			       INTEL_XVMC_MAJOR, INTEL_XVMC_MINOR,
			       INTEL_XVMC_PATCHLEVEL);
	return TRUE;
}
