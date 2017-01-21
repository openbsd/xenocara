/*
 * Copyright 2014 by VMware, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE COPYRIGHT HOLDER(S) OR AUTHOR(S) BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the name of the copyright holder(s)
 * and author(s) shall not be used in advertising or otherwise to promote
 * the sale, use or other dealings in this Software without prior written
 * authorization from the copyright holder(s) and author(s).
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef HAVE_LIBUDEV
#include <libudev.h>
#include <stdlib.h>
#include <string.h>

#define KERNEL_DEVNAME "VirtualPS/2 VMware VMMouse"

/**
 * vmmouse_uses_kernel_driver - Check whether there's an active
 *    vmmouse driver in the kernel.
 *
 * Returns 0 if there was no kernel driver found.
 * Returns non-zero on error or if there was an active driver found.
 *
 * Scans the input subsystem for devices matching KERNEL_DEVNAME. These
 * devices are assumed to be active vmmouse drivers.
 */
int vmmouse_uses_kernel_driver(void)
{
    struct udev *udev;
    struct udev_enumerate *enumerate;
    struct udev_list_entry *devices, *dev_list_entry;
    struct udev_device *dev;

    udev = udev_new();
    if (!udev)
	return 1;

    /*
     * Udev error return codes that are not caught immediately are
     * typically caught in the input argument check in the udev
     * function calls following the failing call!
     */
    enumerate = udev_enumerate_new(udev);
    if (udev_enumerate_add_match_subsystem(enumerate, "input"))
	goto out_err;
    if (udev_enumerate_scan_devices(enumerate))
	goto out_err;

    devices = udev_enumerate_get_list_entry(enumerate);
    udev_list_entry_foreach(dev_list_entry, devices) {
	const char *path, *name;

	path = udev_list_entry_get_name(dev_list_entry);
	dev = udev_device_new_from_syspath(udev, path);
	if (!dev)
	    goto out_err;
	name = udev_device_get_sysattr_value(dev, "name");
	if (name && !strcasecmp(name, KERNEL_DEVNAME))
	    goto out_found;

	udev_device_unref(dev);
    }

    udev_enumerate_unref(enumerate);
    udev_unref(udev);

    return 0;

  out_found:
    udev_device_unref(dev);
  out_err:
    udev_enumerate_unref(enumerate);
    udev_unref(udev);

    return 1;
}
#else
int vmmouse_uses_kernel_driver(void)
{
   return 0;
}
#endif
