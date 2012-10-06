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

#include "xf86.h"
#include "xf86_OSproc.h"
#include "xf86cmap.h"

#include "common.h"
#include "intel.h"
#include "intel_driver.h"
#include "legacy/legacy.h"


static struct intel_device_info *chipset_info;

static const struct intel_device_info intel_i81x_info = {
	.gen = 10,
};

static const struct intel_device_info intel_i8xx_info = {
	.gen = 20,
};

static const struct intel_device_info intel_i915_info = {
	.gen = 30,
};

static const struct intel_device_info intel_g33_info = {
	.gen = 33,
};

static const struct intel_device_info intel_i965_info = {
	.gen = 40,
};

static const struct intel_device_info intel_g4x_info = {
	.gen = 45,
};

static const struct intel_device_info intel_ironlake_info = {
	.gen = 50,
};

static const struct intel_device_info intel_sandybridge_info = {
	.gen = 60,
};

static const struct intel_device_info intel_ivybridge_info = {
	.gen = 70,
};

static const SymTabRec _intel_chipsets[] = {
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
	{PCI_CHIP_IVYBRIDGE_S_GT1,		"Ivybridge Server (GT1)" },
	{PCI_CHIP_IVYBRIDGE_S_GT2,		"Ivybridge Server (GT2)" },
	{-1,					NULL}
};
SymTabRec *intel_chipsets = (SymTabRec *) _intel_chipsets;

#define INTEL_DEVICE_MATCH(d,i) \
    { 0x8086, (d), PCI_MATCH_ANY, PCI_MATCH_ANY, 0, 0, (intptr_t)(i) }

static const struct pci_id_match intel_device_match[] = {
	INTEL_DEVICE_MATCH (PCI_CHIP_I810, &intel_i81x_info ),
	INTEL_DEVICE_MATCH (PCI_CHIP_I810_DC100, &intel_i81x_info ),
	INTEL_DEVICE_MATCH (PCI_CHIP_I810_E, &intel_i81x_info ),
	INTEL_DEVICE_MATCH (PCI_CHIP_I815, &intel_i81x_info ),

	INTEL_DEVICE_MATCH (PCI_CHIP_I830_M, &intel_i8xx_info ),
	INTEL_DEVICE_MATCH (PCI_CHIP_845_G, &intel_i8xx_info ),
	INTEL_DEVICE_MATCH (PCI_CHIP_I854, &intel_i8xx_info ),
	INTEL_DEVICE_MATCH (PCI_CHIP_I855_GM, &intel_i8xx_info ),
	INTEL_DEVICE_MATCH (PCI_CHIP_I865_G, &intel_i8xx_info ),

	INTEL_DEVICE_MATCH (PCI_CHIP_I915_G, &intel_i915_info ),
	INTEL_DEVICE_MATCH (PCI_CHIP_E7221_G, &intel_i915_info ),
	INTEL_DEVICE_MATCH (PCI_CHIP_I915_GM, &intel_i915_info ),
	INTEL_DEVICE_MATCH (PCI_CHIP_I945_G, &intel_i915_info ),
	INTEL_DEVICE_MATCH (PCI_CHIP_I945_GM, &intel_i915_info ),
	INTEL_DEVICE_MATCH (PCI_CHIP_I945_GME, &intel_i915_info ),

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

	{ 0, 0, 0 },
};

void intel_detect_chipset(ScrnInfoPtr scrn,
			  struct pci_device *pci,
			  struct intel_chipset *chipset)
{
	int i;

	chipset->info = chipset_info;

	for (i = 0; intel_chipsets[i].name != NULL; i++) {
		if (DEVICE_ID(pci) == intel_chipsets[i].token) {
			chipset->name = intel_chipsets[i].name;
			break;
		}
	}
	if (intel_chipsets[i].name == NULL) {
		chipset->name = "unknown chipset";
	}

	xf86DrvMsg(scrn->scrnIndex, X_INFO,
		   "Integrated Graphics Chipset: Intel(R) %s\n", chipset->name);
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
	ScrnInfoPtr scrn;
	PciChipsets intel_pci_chipsets[ARRAY_SIZE(intel_chipsets)];
	int i;

	chipset_info = (void *)match_data;


	for (i = 0; i < ARRAY_SIZE(intel_chipsets); i++) {
		intel_pci_chipsets[i].numChipset = intel_chipsets[i].token;
		intel_pci_chipsets[i].PCIid = intel_chipsets[i].token;
		intel_pci_chipsets[i].dummy = NULL;
	}

	scrn = xf86ConfigPciEntity(NULL, 0, entity_num, intel_pci_chipsets,
				   NULL, NULL, NULL, NULL, NULL);
	if (scrn != NULL) {
		scrn->driverVersion = INTEL_VERSION;
		scrn->driverName = INTEL_DRIVER_NAME;
		scrn->name = INTEL_NAME;
		scrn->Probe = NULL;

#if KMS_ONLY
		intel_init_scrn(scrn);
#else
		switch (DEVICE_ID(device)) {
		case PCI_CHIP_I810:
		case PCI_CHIP_I810_DC100:
		case PCI_CHIP_I810_E:
		case PCI_CHIP_I815:
			lg_i810_init(scrn);
			break;

		default:
			intel_init_scrn(scrn);
			break;
		}
#endif
	}
	return scrn != NULL;
}

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
#if KMS_ONLY
	return intel_uxa_available_options(chipid, busid);
#else
	switch (chipid) {
	case PCI_CHIP_I810:
	case PCI_CHIP_I810_DC100:
	case PCI_CHIP_I810_E:
	case PCI_CHIP_I815:
		return lg_i810_available_options(chipid, busid);

	default:
		return intel_uxa_available_options(chipid, busid);
	}
#endif
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
	intel_pci_probe
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
