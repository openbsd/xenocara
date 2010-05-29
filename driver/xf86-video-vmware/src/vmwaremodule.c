/**********************************************************
 * Copyright 2010 VMware, Inc.  All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 **********************************************************/

#include <xorg-server.h>
#include <xf86.h>
#include <xf86drm.h>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

/*
 * Defines and exported module info.
 */

#define VMWARE_DRIVER_NAME    "vmware"
#define VMWGFX_DRIVER_NAME    "vmwgfx"
#define VMWGFX_MODULE_NAME    "vmwgfx"
#define VMWGFX_COMPAT_MAJOR   11
#define VMWGFX_REQUIRED_MAJOR 11
#define VMWGFX_REQUIRED_MINOR 0
#define VMWLEGACY_DRIVER_NAME "vmwlegacy"

#define VMW_STRING_INNER(s) #s
#define VMW_STRING(str) VMW_STRING_INNER(str)

#define VMWARE_DRIVER_VERSION \
   (PACKAGE_VERSION_MAJOR * 65536 + PACKAGE_VERSION_MINOR * 256 + PACKAGE_VERSION_PATCHLEVEL)
#define VMWARE_DRIVER_VERSION_STRING \
    VMW_STRING(PACKAGE_VERSION_MAJOR) "." VMW_STRING(PACKAGE_VERSION_MINOR) \
    "." VMW_STRING(PACKAGE_VERSION_PATCHLEVEL)

/*
 * Standard four digit version string expected by VMware Tools installer.
 * As the driver's version is only  {major, minor, patchlevel}, simply append an
 * extra zero for the fourth digit.
 */
#ifdef __GNUC__
const char vmware_drv_modinfo[] __attribute__((section(".modinfo"),unused)) =
    "version=" VMWARE_DRIVER_VERSION_STRING ".0";
#endif

static XF86ModuleVersionInfo vmware_version;
static MODULESETUPPROTO(vmware_setup);

_X_EXPORT XF86ModuleData vmwareModuleData = {
    &vmware_version,
    vmware_setup,
    NULL
};

extern XF86ModuleData *VMWGFX_MODULE_DATA;

/*
 * Chain loading functions
 */

static Bool
vmware_check_kernel_module()
{
    /* Super simple way of knowing if the kernel driver is loaded */
    int ret = drmOpen(VMWGFX_MODULE_NAME, NULL);
    if (ret < 0) {
	/* This shouldn't go in the log as the original message does not */
	fprintf(stderr, "%s: Please ignore above \"FATAL: Module %s not found."
		"\"\n", VMWARE_DRIVER_NAME, VMWGFX_MODULE_NAME);
	/* This is what goes into the log on the other hand */
	xf86DrvMsg(-1, X_PROBED, "%s: Please ignore above \"[drm] failed to "
		   "load kernel  module \"%s\"\"\n", VMWARE_DRIVER_NAME,
		   VMWGFX_MODULE_NAME);
	return FALSE;
    }

    drmClose(ret);

    return TRUE;
}

static Bool
vmware_check_vmwgfx_driver(int matched, pointer opts)
{
    int major; int minor;
    pointer module;
    CARD32 version;

    if (matched) {
	xf86DrvMsg(-1, X_PROBED, "%s: X configured to use %s X driver assume "
		   "who ever did that knows what they are doing\n",
		   VMWARE_DRIVER_NAME, VMWGFX_DRIVER_NAME);
	/* Also how did they end up here, if the configured X to use vmwgfx and
	 * X should load that driver for that hardware. And since there is only
	 * one SVGA device this driver shouldn't be loaded. Weird...
	 */
	return TRUE;
    }

    module = xf86LoadOneModule(VMWGFX_DRIVER_NAME, opts);
    if (!module) {
	xf86DrvMsg(-1, X_ERROR, "%s: Please ignore the above warnings about "
		   "not being able to to load module/driver %s\n",
		   VMWARE_DRIVER_NAME, VMWGFX_DRIVER_NAME);
	return FALSE;
    }

    version = xf86GetModuleVersion(module);
    major = GET_MODULE_MAJOR_VERSION(version);
    minor = GET_MODULE_MINOR_VERSION(version);

    if (major > VMWGFX_COMPAT_MAJOR ||
	major < VMWGFX_REQUIRED_MAJOR ||
	(major == VMWGFX_REQUIRED_MAJOR && minor < VMWGFX_REQUIRED_MINOR)) {
	xf86DrvMsg(-1, X_PROBED, "%s: The %s X driver failed version "
		   "checking.\n", VMWARE_DRIVER_NAME, VMWGFX_DRIVER_NAME);
	goto err;
    }

    return TRUE;

err:
    /* XXX We should drop the reference on the module here */
    return FALSE;
}

static Bool
vmware_chain_module(pointer opts)
{
    int vmwlegacy_devices;
    int vmwgfx_devices;
    int vmware_devices;
    int matched;
    char *driver_name;
    GDevPtr *gdevs = NULL;
    GDevPtr gdev;
    int i;

    vmware_devices = xf86MatchDevice(VMWARE_DRIVER_NAME, &gdevs);
    vmwgfx_devices = xf86MatchDevice(VMWGFX_DRIVER_NAME, NULL);
    vmwlegacy_devices = xf86MatchDevice(VMWLEGACY_DRIVER_NAME, NULL);

    if (vmware_check_vmwgfx_driver(vmwgfx_devices, opts) &&
	vmware_check_kernel_module()) {
	xf86DrvMsg(-1, X_INFO, "%s: Using %s X driver.\n",
		   VMWARE_DRIVER_NAME, VMWGFX_DRIVER_NAME);
	driver_name = VMWGFX_DRIVER_NAME;
	matched = 1;
    } else {
	xf86DrvMsg(-1, X_INFO, "%s: Using %s driver everything is fine.\n",
		   VMWARE_DRIVER_NAME, VMWLEGACY_DRIVER_NAME);
	driver_name = VMWLEGACY_DRIVER_NAME;
	matched = vmwlegacy_devices;
    }

    if (gdevs != NULL) {
	for (i = 0; i < vmware_devices; i++) {
	    gdev = gdevs[i];
	    gdev->driver = driver_name;
	}

	xfree(gdevs);
    }
    if (!matched)
	xf86LoadOneModule(driver_name, opts);
}


/*
 * Module info
 */

static XF86ModuleVersionInfo vmware_version = {
    VMWARE_DRIVER_NAME,
    MODULEVENDORSTRING,
    MODINFOSTRING1,
    MODINFOSTRING2,
    XORG_VERSION_CURRENT,
    PACKAGE_VERSION_MAJOR, PACKAGE_VERSION_MINOR, PACKAGE_VERSION_PATCHLEVEL,
    ABI_CLASS_VIDEODRV,
    ABI_VIDEODRV_VERSION,
    MOD_CLASS_VIDEODRV,
    {0, 0, 0, 0}
};

static pointer
vmware_setup(pointer module, pointer opts, int *errmaj, int *errmin)
{
    static Bool setupDone = 0;
    int ret;

    /* This module should be loaded only once, but check to be sure. */
    if (!setupDone) {
	setupDone = 1;

	/* Chain load the real driver */
	vmware_chain_module(opts);

	return (pointer) 1;
    } else {
	if (errmaj)
	    *errmaj = LDR_ONCEONLY;
	return NULL;
    }
}
