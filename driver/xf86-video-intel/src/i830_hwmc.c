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
#include "i830.h"
#include "i830_hwmc.h"

struct intel_xvmc_driver *xvmc_driver;

/* set global current driver for xvmc */
static Bool intel_xvmc_set_driver(struct intel_xvmc_driver *d)
{
	if (xvmc_driver) {
		ErrorF("XvMC driver already set!\n");
		return FALSE;
	} else
		xvmc_driver = d;
	return TRUE;
}

/* check chip type and load xvmc driver */
/* This must be first called! */
Bool intel_xvmc_probe(ScrnInfoPtr scrn)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);
	Bool ret = FALSE;

	if (!intel->XvMCEnabled)
		return FALSE;

	/* Needs KMS support. */
	if (intel->use_drm_mode && (IS_I915G(intel) || IS_I915GM(intel)))
		return FALSE;

	if (IS_I9XX(intel)) {
		if (IS_I915(intel))
			ret = intel_xvmc_set_driver(&i915_xvmc_driver);
		else if (IS_G4X(intel) || IS_IGDNG(intel))
			ret = intel_xvmc_set_driver(&vld_xvmc_driver);
		else
			ret = intel_xvmc_set_driver(&i965_xvmc_driver);
	} else {
		ErrorF("Your chipset doesn't support XvMC.\n");
		return FALSE;
	}
	return TRUE;
}

void intel_xvmc_finish(ScrnInfoPtr scrn)
{
	if (!xvmc_driver)
		return;
	(*xvmc_driver->fini) (scrn);
}

Bool intel_xvmc_driver_init(ScreenPtr pScreen, XF86VideoAdaptorPtr xv_adaptor)
{
	ScrnInfoPtr scrn = xf86Screens[pScreen->myNum];
	intel_screen_private *intel = intel_get_screen_private(scrn);
	struct drm_i915_setparam sp;
	int ret;

	if (!xvmc_driver) {
		ErrorF("Failed to probe XvMC driver.\n");
		return FALSE;
	}

	if (!(*xvmc_driver->init) (scrn, xv_adaptor)) {
		ErrorF("XvMC driver initialize failed.\n");
		return FALSE;
	}

	/* Currently XvMC uses batchbuffer */
	sp.param = I915_SETPARAM_ALLOW_BATCHBUFFER;
	sp.value = 1;
	ret = drmCommandWrite(intel->drmSubFD, DRM_I915_SETPARAM,
			      &sp, sizeof(sp));
	if (ret == 0)
		return TRUE;

	return FALSE;
}

Bool intel_xvmc_screen_init(ScreenPtr pScreen)
{
	ScrnInfoPtr scrn = xf86Screens[pScreen->myNum];
	intel_screen_private *intel = intel_get_screen_private(scrn);
	char buf[64];

	if (!xvmc_driver)
		return FALSE;

	if (xf86XvMCScreenInit(pScreen, 1, &xvmc_driver->adaptor)) {
		xf86DrvMsg(scrn->scrnIndex, X_INFO,
			   "[XvMC] %s driver initialized.\n",
			   xvmc_driver->name);
	} else {
		intel_xvmc_finish(scrn);
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

Bool intel_xvmc_init_batch(ScrnInfoPtr scrn)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);
	int size = KB(64);

	if (!i830_allocate_xvmc_buffer(scrn, "[XvMC] batch buffer",
				       &(xvmc_driver->batch), size,
				       ALIGN_BOTH_ENDS))
		return FALSE;

	if (drmAddMap(intel->drmSubFD,
		      (drm_handle_t) (xvmc_driver->batch->offset +
				      intel->LinearAddr),
		      xvmc_driver->batch->size, DRM_AGP, 0,
		      &xvmc_driver->batch_handle) < 0) {
		xf86DrvMsg(scrn->scrnIndex, X_ERROR,
			   "[drm] drmAddMap(batchbuffer_handle) failed!\n");
		return FALSE;
	}
	return TRUE;
}

void intel_xvmc_fini_batch(ScrnInfoPtr scrn)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);

	if (xvmc_driver->batch_handle) {
		drmRmMap(intel->drmSubFD, xvmc_driver->batch_handle);
		xvmc_driver->batch_handle = 0;
	}
	if (xvmc_driver->batch) {
		i830_free_xvmc_buffer(scrn, xvmc_driver->batch);
		xvmc_driver->batch = NULL;
	}
}
