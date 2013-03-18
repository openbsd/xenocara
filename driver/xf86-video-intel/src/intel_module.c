/*
 * Copyright 1998-1999 Precision Insight, Inc., Cedar Park, Texas.
 * All Rights Reserved.
 * Copyright © 2010 Intel Corporation
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
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <unistd.h>
#include <xf86_OSproc.h>
#include <xf86Parser.h>
#include <xf86drm.h>
#include <xf86drmMode.h>
#include <i915_drm.h>

#include <xorgVersion.h>

#if XORG_VERSION_CURRENT < XORG_VERSION_NUMERIC(1,6,99,0,0)
#include <xf86Resources.h>
#endif

#include "common.h"
#include "intel_driver.h"
#include "intel_options.h"
#include "legacy/legacy.h"
#include "sna/sna_module.h"

#ifdef XSERVER_PLATFORM_BUS
#include <xf86platformBus.h>
#endif

static const struct intel_device_info intel_generic_info = {
	.gen = -1,
};

static const struct intel_device_info intel_i81x_info = {
	.gen = 010,
};

static const struct intel_device_info intel_i830_info = {
	.gen = 020,
};
static const struct intel_device_info intel_i845_info = {
	.gen = 020,
};
static const struct intel_device_info intel_i855_info = {
	.gen = 021,
};
static const struct intel_device_info intel_i865_info = {
	.gen = 022,
};

static const struct intel_device_info intel_i915_info = {
	.gen = 030,
};
static const struct intel_device_info intel_i945_info = {
	.gen = 031,
};

static const struct intel_device_info intel_g33_info = {
	.gen = 033,
};

static const struct intel_device_info intel_i965_info = {
	.gen = 040,
};

static const struct intel_device_info intel_g4x_info = {
	.gen = 045,
};

static const struct intel_device_info intel_ironlake_info = {
	.gen = 050,
};

static const struct intel_device_info intel_sandybridge_info = {
	.gen = 060,
};

static const struct intel_device_info intel_ivybridge_info = {
	.gen = 070,
};

static const struct intel_device_info intel_valleyview_info = {
	.gen = 070,
};

static const struct intel_device_info intel_haswell_info = {
	.gen = 075,
};

static const SymTabRec intel_chipsets[] = {
	{PCI_CHIP_I810,				"i810"},
	{PCI_CHIP_I810_DC100,			"i810-dc100"},
	{PCI_CHIP_I810_E,			"i810e"},
	{PCI_CHIP_I815,				"i815"},
	{PCI_CHIP_I830_M,			"i830M"},
	{PCI_CHIP_845_G,			"845G"},
	{PCI_CHIP_I854,				"854"},
	{PCI_CHIP_I855_GM,			"852GM/855GM"},
	{PCI_CHIP_I865_G,			"865G"},
	{PCI_CHIP_I915_G,			"915G"},
	{PCI_CHIP_E7221_G,			"E7221 (i915)"},
	{PCI_CHIP_I915_GM,			"915GM"},
	{PCI_CHIP_I945_G,			"945G"},
	{PCI_CHIP_I945_GM,			"945GM"},
	{PCI_CHIP_I945_GME,			"945GME"},
	{PCI_CHIP_PINEVIEW_M,			"Pineview GM"},
	{PCI_CHIP_PINEVIEW_G,			"Pineview G"},
	{PCI_CHIP_I965_G,			"965G"},
	{PCI_CHIP_G35_G,			"G35"},
	{PCI_CHIP_I965_Q,			"965Q"},
	{PCI_CHIP_I946_GZ,			"946GZ"},
	{PCI_CHIP_I965_GM,			"965GM"},
	{PCI_CHIP_I965_GME,			"965GME/GLE"},
	{PCI_CHIP_G33_G,			"G33"},
	{PCI_CHIP_Q35_G,			"Q35"},
	{PCI_CHIP_Q33_G,			"Q33"},
	{PCI_CHIP_GM45_GM,			"GM45"},
	{PCI_CHIP_G45_E_G,			"4 Series"},
	{PCI_CHIP_G45_G,			"G45/G43"},
	{PCI_CHIP_Q45_G,			"Q45/Q43"},
	{PCI_CHIP_G41_G,			"G41"},
	{PCI_CHIP_B43_G,			"B43"},
	{PCI_CHIP_B43_G1,			"B43"},
	{PCI_CHIP_IRONLAKE_D_G,			"Clarkdale"},
	{PCI_CHIP_IRONLAKE_M_G,			"Arrandale"},
	{PCI_CHIP_SANDYBRIDGE_GT1,		"Sandybridge Desktop (GT1)" },
	{PCI_CHIP_SANDYBRIDGE_GT2,		"Sandybridge Desktop (GT2)" },
	{PCI_CHIP_SANDYBRIDGE_GT2_PLUS,		"Sandybridge Desktop (GT2+)" },
	{PCI_CHIP_SANDYBRIDGE_M_GT1,		"Sandybridge Mobile (GT1)" },
	{PCI_CHIP_SANDYBRIDGE_M_GT2,		"Sandybridge Mobile (GT2)" },
	{PCI_CHIP_SANDYBRIDGE_M_GT2_PLUS,	"Sandybridge Mobile (GT2+)" },
	{PCI_CHIP_SANDYBRIDGE_S_GT,		"Sandybridge Server" },
	{PCI_CHIP_IVYBRIDGE_M_GT1,		"Ivybridge Mobile (GT1)" },
	{PCI_CHIP_IVYBRIDGE_M_GT2,		"Ivybridge Mobile (GT2)" },
	{PCI_CHIP_IVYBRIDGE_D_GT1,		"Ivybridge Desktop (GT1)" },
	{PCI_CHIP_IVYBRIDGE_D_GT2,		"Ivybridge Desktop (GT2)" },
	{PCI_CHIP_IVYBRIDGE_S_GT1,		"Ivybridge Server" },
	{PCI_CHIP_IVYBRIDGE_S_GT2,		"Ivybridge Server (GT2)" },
	{PCI_CHIP_HASWELL_D_GT1,		"Haswell Desktop (GT1)" },
	{PCI_CHIP_HASWELL_D_GT2,		"Haswell Desktop (GT2)" },
	{PCI_CHIP_HASWELL_D_GT2_PLUS,		"Haswell Desktop (GT2+)" },
	{PCI_CHIP_HASWELL_M_GT1,		"Haswell Mobile (GT1)" },
	{PCI_CHIP_HASWELL_M_GT2,		"Haswell Mobile (GT2)" },
	{PCI_CHIP_HASWELL_M_GT2_PLUS,		"Haswell Mobile (GT2+)" },
	{PCI_CHIP_HASWELL_S_GT1,		"Haswell Server (GT1)" },
	{PCI_CHIP_HASWELL_S_GT2,		"Haswell Server (GT2)" },
	{PCI_CHIP_HASWELL_S_GT2_PLUS,		"Haswell Server (GT2+)" },
	{PCI_CHIP_HASWELL_SDV_D_GT1,		"Haswell SDV Desktop (GT1)" },
	{PCI_CHIP_HASWELL_SDV_D_GT2,		"Haswell SDV Desktop (GT2)" },
	{PCI_CHIP_HASWELL_SDV_D_GT2_PLUS,	"Haswell SDV Desktop (GT2+)" },
	{PCI_CHIP_HASWELL_SDV_M_GT1,		"Haswell SDV Mobile (GT1)" },
	{PCI_CHIP_HASWELL_SDV_M_GT2,		"Haswell SDV Mobile (GT2)" },
	{PCI_CHIP_HASWELL_SDV_M_GT2_PLUS,	"Haswell SDV Mobile (GT2+)" },
	{PCI_CHIP_HASWELL_SDV_S_GT1,		"Haswell SDV Server (GT1)" },
	{PCI_CHIP_HASWELL_SDV_S_GT2,		"Haswell SDV Server (GT2)" },
	{PCI_CHIP_HASWELL_SDV_S_GT2_PLUS,	"Haswell SDV Server (GT2+)" },
	{PCI_CHIP_HASWELL_ULT_D_GT1,		"Haswell ULT Desktop (GT1)" },
	{PCI_CHIP_HASWELL_ULT_D_GT2,		"Haswell ULT Desktop (GT2)" },
	{PCI_CHIP_HASWELL_ULT_D_GT2_PLUS,	"Haswell ULT Desktop (GT2+)" },
	{PCI_CHIP_HASWELL_ULT_M_GT1,		"Haswell ULT Mobile (GT1)" },
	{PCI_CHIP_HASWELL_ULT_M_GT2,		"Haswell ULT Mobile (GT2)" },
	{PCI_CHIP_HASWELL_ULT_M_GT2_PLUS,	"Haswell ULT Mobile (GT2+)" },
	{PCI_CHIP_HASWELL_ULT_S_GT1,		"Haswell ULT Server (GT1)" },
	{PCI_CHIP_HASWELL_ULT_S_GT2,		"Haswell ULT Server (GT2)" },
	{PCI_CHIP_HASWELL_ULT_S_GT2_PLUS,	"Haswell ULT Server (GT2+)" },
	{PCI_CHIP_HASWELL_CRW_D_GT1,		"Haswell CRW Desktop (GT1)" },
	{PCI_CHIP_HASWELL_CRW_D_GT2,		"Haswell CRW Desktop (GT2)" },
	{PCI_CHIP_HASWELL_CRW_D_GT2_PLUS,	"Haswell CRW Desktop (GT2+)" },
	{PCI_CHIP_HASWELL_CRW_M_GT1,		"Haswell CRW Mobile (GT1)" },
	{PCI_CHIP_HASWELL_CRW_M_GT2,		"Haswell CRW Mobile (GT2)" },
	{PCI_CHIP_HASWELL_CRW_M_GT2_PLUS,	"Haswell CRW Mobile (GT2+)" },
	{PCI_CHIP_HASWELL_CRW_S_GT1,		"Haswell CRW Server (GT1)" },
	{PCI_CHIP_HASWELL_CRW_S_GT2,		"Haswell CRW Server (GT2)" },
	{PCI_CHIP_HASWELL_CRW_S_GT2_PLUS,	"Haswell CRW Server (GT2+)" },
	{PCI_CHIP_VALLEYVIEW_PO,		"ValleyView PO board" },
	{-1,					NULL}
};
#define NUM_CHIPSETS (sizeof(intel_chipsets) / sizeof(intel_chipsets[0]))

#define INTEL_DEVICE_MATCH(d,i) \
    { 0x8086, (d), PCI_MATCH_ANY, PCI_MATCH_ANY, 0x3 << 16, 0xff << 16, (intptr_t)(i) }

static const struct pci_id_match intel_device_match[] = {
#if !KMS_ONLY
	INTEL_DEVICE_MATCH (PCI_CHIP_I810, &intel_i81x_info ),
	INTEL_DEVICE_MATCH (PCI_CHIP_I810_DC100, &intel_i81x_info ),
	INTEL_DEVICE_MATCH (PCI_CHIP_I810_E, &intel_i81x_info ),
	INTEL_DEVICE_MATCH (PCI_CHIP_I815, &intel_i81x_info ),
#endif

#if !UMS_ONLY
	INTEL_DEVICE_MATCH (PCI_CHIP_I830_M, &intel_i830_info ),
	INTEL_DEVICE_MATCH (PCI_CHIP_845_G, &intel_i845_info ),
	INTEL_DEVICE_MATCH (PCI_CHIP_I854, &intel_i855_info ),
	INTEL_DEVICE_MATCH (PCI_CHIP_I855_GM, &intel_i855_info ),
	INTEL_DEVICE_MATCH (PCI_CHIP_I865_G, &intel_i865_info ),

	INTEL_DEVICE_MATCH (PCI_CHIP_I915_G, &intel_i915_info ),
	INTEL_DEVICE_MATCH (PCI_CHIP_E7221_G, &intel_i915_info ),
	INTEL_DEVICE_MATCH (PCI_CHIP_I915_GM, &intel_i915_info ),
	INTEL_DEVICE_MATCH (PCI_CHIP_I945_G, &intel_i945_info ),
	INTEL_DEVICE_MATCH (PCI_CHIP_I945_GM, &intel_i945_info ),
	INTEL_DEVICE_MATCH (PCI_CHIP_I945_GME, &intel_i945_info ),

	INTEL_DEVICE_MATCH (PCI_CHIP_PINEVIEW_M, &intel_g33_info ),
	INTEL_DEVICE_MATCH (PCI_CHIP_PINEVIEW_G, &intel_g33_info ),
	INTEL_DEVICE_MATCH (PCI_CHIP_G33_G, &intel_g33_info ),
	INTEL_DEVICE_MATCH (PCI_CHIP_Q33_G, &intel_g33_info ),
	/* Another marketing win: Q35 is another g33 device not a gen4 part
	 * like its G35 brethren.
	 */
	INTEL_DEVICE_MATCH (PCI_CHIP_Q35_G, &intel_g33_info ),

	INTEL_DEVICE_MATCH (PCI_CHIP_I965_G, &intel_i965_info ),
	INTEL_DEVICE_MATCH (PCI_CHIP_G35_G, &intel_i965_info ),
	INTEL_DEVICE_MATCH (PCI_CHIP_I965_Q, &intel_i965_info ),
	INTEL_DEVICE_MATCH (PCI_CHIP_I946_GZ, &intel_i965_info ),
	INTEL_DEVICE_MATCH (PCI_CHIP_I965_GM, &intel_i965_info ),
	INTEL_DEVICE_MATCH (PCI_CHIP_I965_GME, &intel_i965_info ),

	INTEL_DEVICE_MATCH (PCI_CHIP_GM45_GM, &intel_g4x_info ),
	INTEL_DEVICE_MATCH (PCI_CHIP_G45_E_G, &intel_g4x_info ),
	INTEL_DEVICE_MATCH (PCI_CHIP_G45_G, &intel_g4x_info ),
	INTEL_DEVICE_MATCH (PCI_CHIP_Q45_G, &intel_g4x_info ),
	INTEL_DEVICE_MATCH (PCI_CHIP_G41_G, &intel_g4x_info ),
	INTEL_DEVICE_MATCH (PCI_CHIP_B43_G, &intel_g4x_info ),
	INTEL_DEVICE_MATCH (PCI_CHIP_B43_G1, &intel_g4x_info ),

	INTEL_DEVICE_MATCH (PCI_CHIP_IRONLAKE_D_G, &intel_ironlake_info ),
	INTEL_DEVICE_MATCH (PCI_CHIP_IRONLAKE_M_G, &intel_ironlake_info ),

	INTEL_DEVICE_MATCH (PCI_CHIP_SANDYBRIDGE_GT1, &intel_sandybridge_info ),
	INTEL_DEVICE_MATCH (PCI_CHIP_SANDYBRIDGE_GT2, &intel_sandybridge_info ),
	INTEL_DEVICE_MATCH (PCI_CHIP_SANDYBRIDGE_GT2_PLUS, &intel_sandybridge_info ),
	INTEL_DEVICE_MATCH (PCI_CHIP_SANDYBRIDGE_M_GT1, &intel_sandybridge_info ),
	INTEL_DEVICE_MATCH (PCI_CHIP_SANDYBRIDGE_M_GT2, &intel_sandybridge_info ),
	INTEL_DEVICE_MATCH (PCI_CHIP_SANDYBRIDGE_M_GT2_PLUS, &intel_sandybridge_info ),
	INTEL_DEVICE_MATCH (PCI_CHIP_SANDYBRIDGE_S_GT, &intel_sandybridge_info ),

	INTEL_DEVICE_MATCH (PCI_CHIP_IVYBRIDGE_M_GT1, &intel_ivybridge_info ),
	INTEL_DEVICE_MATCH (PCI_CHIP_IVYBRIDGE_M_GT2, &intel_ivybridge_info ),
	INTEL_DEVICE_MATCH (PCI_CHIP_IVYBRIDGE_D_GT1, &intel_ivybridge_info ),
	INTEL_DEVICE_MATCH (PCI_CHIP_IVYBRIDGE_D_GT2, &intel_ivybridge_info ),
	INTEL_DEVICE_MATCH (PCI_CHIP_IVYBRIDGE_S_GT1, &intel_ivybridge_info ),
	INTEL_DEVICE_MATCH (PCI_CHIP_IVYBRIDGE_S_GT2, &intel_ivybridge_info ),

	INTEL_DEVICE_MATCH (PCI_CHIP_HASWELL_D_GT1, &intel_haswell_info ),
	INTEL_DEVICE_MATCH (PCI_CHIP_HASWELL_D_GT2, &intel_haswell_info ),
	INTEL_DEVICE_MATCH (PCI_CHIP_HASWELL_D_GT2_PLUS, &intel_haswell_info ),
	INTEL_DEVICE_MATCH (PCI_CHIP_HASWELL_M_GT1, &intel_haswell_info ),
	INTEL_DEVICE_MATCH (PCI_CHIP_HASWELL_M_GT2, &intel_haswell_info ),
	INTEL_DEVICE_MATCH (PCI_CHIP_HASWELL_M_GT2_PLUS, &intel_haswell_info ),
	INTEL_DEVICE_MATCH (PCI_CHIP_HASWELL_S_GT1, &intel_haswell_info ),
	INTEL_DEVICE_MATCH (PCI_CHIP_HASWELL_S_GT2, &intel_haswell_info ),
	INTEL_DEVICE_MATCH (PCI_CHIP_HASWELL_S_GT2_PLUS, &intel_haswell_info ),
	INTEL_DEVICE_MATCH (PCI_CHIP_HASWELL_SDV_D_GT1, &intel_haswell_info ),
	INTEL_DEVICE_MATCH (PCI_CHIP_HASWELL_SDV_D_GT2, &intel_haswell_info ),
	INTEL_DEVICE_MATCH (PCI_CHIP_HASWELL_SDV_D_GT2_PLUS, &intel_haswell_info ),
	INTEL_DEVICE_MATCH (PCI_CHIP_HASWELL_SDV_M_GT1, &intel_haswell_info ),
	INTEL_DEVICE_MATCH (PCI_CHIP_HASWELL_SDV_M_GT2, &intel_haswell_info ),
	INTEL_DEVICE_MATCH (PCI_CHIP_HASWELL_SDV_M_GT2_PLUS, &intel_haswell_info ),
	INTEL_DEVICE_MATCH (PCI_CHIP_HASWELL_SDV_S_GT1, &intel_haswell_info ),
	INTEL_DEVICE_MATCH (PCI_CHIP_HASWELL_SDV_S_GT2, &intel_haswell_info ),
	INTEL_DEVICE_MATCH (PCI_CHIP_HASWELL_SDV_S_GT2_PLUS, &intel_haswell_info ),
	INTEL_DEVICE_MATCH (PCI_CHIP_HASWELL_ULT_D_GT1, &intel_haswell_info ),
	INTEL_DEVICE_MATCH (PCI_CHIP_HASWELL_ULT_D_GT2, &intel_haswell_info ),
	INTEL_DEVICE_MATCH (PCI_CHIP_HASWELL_ULT_D_GT2_PLUS, &intel_haswell_info ),
	INTEL_DEVICE_MATCH (PCI_CHIP_HASWELL_ULT_M_GT1, &intel_haswell_info ),
	INTEL_DEVICE_MATCH (PCI_CHIP_HASWELL_ULT_M_GT2, &intel_haswell_info ),
	INTEL_DEVICE_MATCH (PCI_CHIP_HASWELL_ULT_M_GT2_PLUS, &intel_haswell_info ),
	INTEL_DEVICE_MATCH (PCI_CHIP_HASWELL_ULT_S_GT1, &intel_haswell_info ),
	INTEL_DEVICE_MATCH (PCI_CHIP_HASWELL_ULT_S_GT2, &intel_haswell_info ),
	INTEL_DEVICE_MATCH (PCI_CHIP_HASWELL_ULT_S_GT2_PLUS, &intel_haswell_info ),
	INTEL_DEVICE_MATCH (PCI_CHIP_HASWELL_CRW_D_GT1, &intel_haswell_info ),
	INTEL_DEVICE_MATCH (PCI_CHIP_HASWELL_CRW_D_GT2, &intel_haswell_info ),
	INTEL_DEVICE_MATCH (PCI_CHIP_HASWELL_CRW_D_GT2_PLUS, &intel_haswell_info ),
	INTEL_DEVICE_MATCH (PCI_CHIP_HASWELL_CRW_M_GT1, &intel_haswell_info ),
	INTEL_DEVICE_MATCH (PCI_CHIP_HASWELL_CRW_M_GT2, &intel_haswell_info ),
	INTEL_DEVICE_MATCH (PCI_CHIP_HASWELL_CRW_M_GT2_PLUS, &intel_haswell_info ),
	INTEL_DEVICE_MATCH (PCI_CHIP_HASWELL_CRW_S_GT1, &intel_haswell_info ),
	INTEL_DEVICE_MATCH (PCI_CHIP_HASWELL_CRW_S_GT2, &intel_haswell_info ),
	INTEL_DEVICE_MATCH (PCI_CHIP_HASWELL_CRW_S_GT2_PLUS, &intel_haswell_info ),

	INTEL_DEVICE_MATCH (PCI_CHIP_VALLEYVIEW_PO, &intel_valleyview_info ),

	INTEL_DEVICE_MATCH (PCI_MATCH_ANY, &intel_generic_info ),
#endif

	{ 0, 0, 0 },
};

void
intel_detect_chipset(ScrnInfoPtr scrn,
		     EntityInfoPtr ent,
		     struct pci_device *pci)
{
	MessageType from = X_PROBED;
	const char *name = NULL;
	int i;

	if (ent->device->chipID >= 0) {
		xf86DrvMsg(scrn->scrnIndex, from = X_CONFIG,
			   "ChipID override: 0x%04X\n",
			   ent->device->chipID);
		DEVICE_ID(pci) = ent->device->chipID;
	}

	for (i = 0; intel_chipsets[i].name != NULL; i++) {
		if (DEVICE_ID(pci) == intel_chipsets[i].token) {
			name = intel_chipsets[i].name;
			break;
		}
	}
	if (name == NULL) {
		xf86DrvMsg(scrn->scrnIndex, X_WARNING, "unknown chipset\n");
		name = "unknown";
	} else {
		xf86DrvMsg(scrn->scrnIndex, from,
			   "Integrated Graphics Chipset: Intel(R) %s\n",
			   name);
	}

	scrn->chipset = name;
}

/*
 * intel_identify --
 *
 * Returns the string name for the driver based on the chipset.
 *
 */
static void intel_identify(int flags)
{
	xf86PrintChipsets(INTEL_NAME,
			  "Driver for Intel Integrated Graphics Chipsets",
			  intel_chipsets);
}

static Bool intel_driver_func(ScrnInfoPtr pScrn,
			      xorgDriverFuncOp op,
			      pointer ptr)
{
	xorgHWFlags *flag;

	switch (op) {
	case GET_REQUIRED_HW_INTERFACES:
		flag = (CARD32*)ptr;
#ifdef KMS_ONLY
		(*flag) = 0;
#else
		(*flag) = HW_IO | HW_MMIO;
#endif
		return TRUE;
	default:
		/* Unknown or deprecated function */
		return FALSE;
	}
}

static Bool has_kernel_mode_setting(const struct pci_device *dev)
{
	char id[20];
	int ret, fd;

	snprintf(id, sizeof(id),
		 "pci:%04x:%02x:%02x.%d",
		 dev->domain, dev->bus, dev->dev, dev->func);

	ret = drmCheckModesettingSupported(id);
	if (ret) {
		if (xf86LoadKernelModule("i915"))
			ret = drmCheckModesettingSupported(id);
		if (ret)
			return FALSE;
		/* Be nice to the user and load fbcon too */
		(void)xf86LoadKernelModule("fbcon");
	}

	/* Confirm that this is a i915.ko device with GEM/KMS enabled */
	ret = FALSE;
	fd = drmOpen(NULL, id);
	if (fd != -1) {
		drmVersionPtr version = drmGetVersion(fd);
		if (version) {
			ret = strcmp ("i915", version->name) == 0;
			drmFreeVersion(version);
		}
		if (ret) {
			struct drm_i915_getparam gp;
			gp.param = I915_PARAM_HAS_GEM;
			gp.value = &ret;
			if (drmIoctl(fd, DRM_IOCTL_I915_GETPARAM, &gp))
				ret = FALSE;
		}
		close(fd);
	}

	return ret;
}

#if !UMS_ONLY
extern XF86ConfigPtr xf86configptr;

static XF86ConfDevicePtr
_xf86findDriver(const char *ident, XF86ConfDevicePtr p)
{
	while (p) {
		if (p->dev_driver && xf86nameCompare(ident, p->dev_driver) == 0)
			return p;

		p = p->list.next;
	}

	return NULL;
}

static enum accel_method { UXA, SNA } get_accel_method(void)
{
	enum accel_method accel_method = DEFAULT_ACCEL_METHOD;
	XF86ConfDevicePtr dev;

	dev = _xf86findDriver("intel", xf86configptr->conf_device_lst);
	if (dev && dev->dev_option_lst) {
		const char *s;

		s = xf86FindOptionValue(dev->dev_option_lst, "AccelMethod");
		if (s ) {
			if (strcasecmp(s, "sna") == 0)
				accel_method = SNA;
			else if (strcasecmp(s, "uxa") == 0)
				accel_method = UXA;
			else if (strcasecmp(s, "glamor") == 0)
				accel_method = UXA;
		}
	}

	return accel_method;
}
#endif

static Bool
intel_scrn_create(DriverPtr		driver,
		  int			entity_num,
		  intptr_t		match_data,
		  unsigned		flags)
{
	ScrnInfoPtr scrn;

	scrn = xf86AllocateScreen(driver, flags);
	if (scrn == NULL)
		return FALSE;

	scrn->driverVersion = INTEL_VERSION;
	scrn->driverName = INTEL_DRIVER_NAME;
	scrn->name = INTEL_NAME;
	scrn->driverPrivate = (void *)(match_data | 1);
	scrn->Probe = NULL;

	if (xf86IsEntitySharable(entity_num))
		xf86SetEntityShared(entity_num);
	xf86AddEntityToScreen(scrn, entity_num);

#if !KMS_ONLY
	if (((struct intel_device_info *)match_data)->gen < 020)
		return lg_i810_init(scrn);
#endif

#if !UMS_ONLY
	switch (get_accel_method()) {
#if USE_SNA
	case SNA: return sna_init_scrn(scrn, entity_num);
#endif
#if USE_UXA
	case UXA: return intel_init_scrn(scrn);
#endif

	default: break;
	}
#endif

	return FALSE;
}

/*
 * intel_pci_probe --
 *
 * Look through the PCI bus to find cards that are intel boards.
 * Setup the dispatch table for the rest of the driver functions.
 *
 */
static Bool intel_pci_probe(DriverPtr		driver,
			    int			entity_num,
			    struct pci_device	*device,
			    intptr_t		match_data)
{
	if (!has_kernel_mode_setting(device)) {
#if KMS_ONLY
		return FALSE;
#else
		switch (DEVICE_ID(device)) {
		case PCI_CHIP_I810:
		case PCI_CHIP_I810_DC100:
		case PCI_CHIP_I810_E:
		case PCI_CHIP_I815:
			break;
		default:
			return FALSE;
		}
#endif
	}

	return intel_scrn_create(driver, entity_num, match_data, 0);
}

#ifdef XSERVER_PLATFORM_BUS
static Bool
intel_platform_probe(DriverPtr driver,
		     int entity_num, int flags,
		     struct xf86_platform_device *dev,
		     intptr_t match_data)
{
	unsigned scrn_flags = 0;

	if (!dev->pdev)
		return FALSE;

	if (!has_kernel_mode_setting(dev->pdev))
		return FALSE;

	/* Allow ourselves to act as a slaved output if not primary */
	if (flags & PLATFORM_PROBE_GPU_SCREEN) {
		flags &= ~PLATFORM_PROBE_GPU_SCREEN;
		scrn_flags |= XF86_ALLOCATE_GPU_SCREEN;
	}

	/* if we get any flags we don't understand fail to probe for now */
	if (flags)
		return FALSE;

	return intel_scrn_create(driver, entity_num, match_data, scrn_flags);
}
#endif

#ifdef XFree86LOADER

static MODULESETUPPROTO(intel_setup);

static XF86ModuleVersionInfo intel_version = {
	"intel",
	MODULEVENDORSTRING,
	MODINFOSTRING1,
	MODINFOSTRING2,
	XORG_VERSION_CURRENT,
	INTEL_VERSION_MAJOR, INTEL_VERSION_MINOR, INTEL_VERSION_PATCH,
	ABI_CLASS_VIDEODRV,
	ABI_VIDEODRV_VERSION,
	MOD_CLASS_VIDEODRV,
	{0, 0, 0, 0}
};

static const OptionInfoRec *
intel_available_options(int chipid, int busid)
{
	switch (chipid) {
#if !KMS_ONLY
	case PCI_CHIP_I810:
	case PCI_CHIP_I810_DC100:
	case PCI_CHIP_I810_E:
	case PCI_CHIP_I815:
		return lg_i810_available_options(chipid, busid);
#endif

	default:
		return intel_options;
	}
}

static DriverRec intel = {
	INTEL_VERSION,
	INTEL_DRIVER_NAME,
	intel_identify,
	NULL,
	intel_available_options,
	NULL,
	0,
	intel_driver_func,
	intel_device_match,
	intel_pci_probe,
#ifdef XSERVER_PLATFORM_BUS
	intel_platform_probe
#endif
};

static pointer intel_setup(pointer module,
			   pointer opts,
			   int *errmaj,
			   int *errmin)
{
	static Bool setupDone = 0;

	/* This module should be loaded only once, but check to be sure.
	*/
	if (!setupDone) {
		setupDone = 1;
		xf86AddDriver(&intel, module, HaveDriverFuncs);

		/*
		 * The return value must be non-NULL on success even though there
		 * is no TearDownProc.
		 */
		return (pointer) 1;
	} else {
		if (errmaj)
			*errmaj = LDR_ONCEONLY;
		return NULL;
	}
}

_X_EXPORT XF86ModuleData intelModuleData = { &intel_version, intel_setup, NULL };
#endif
