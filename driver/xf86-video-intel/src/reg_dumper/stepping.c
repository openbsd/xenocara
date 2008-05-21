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
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 * Authors:
 *    Eric Anholt <eric@anholt.net>
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <pciaccess.h>
#include <err.h>
#include "common.h"

int main(int argc, char **argv)
{
    struct pci_device *dev, *bridge;
    int err;
    uint8_t stepping;
    char *step_desc = "??";

    err = pci_system_init();
    if (err != 0) {
	fprintf(stderr, "Couldn't initialize PCI system: %s\n", strerror(err));
	exit(1);
    }

    /* Grab the graphics card */
    dev = pci_device_find_by_slot(0, 0, 2, 0);
    if (dev == NULL)
	errx(1, "Couldn't find graphics card");

    err = pci_device_probe(dev);
    if (err != 0) {
	fprintf(stderr, "Couldn't probe graphics card: %s\n", strerror(err));
	exit(1);
    }

    if (dev->vendor_id != 0x8086)
	errx(1, "Graphics card is non-intel");

    bridge = pci_device_find_by_slot(0, 0, 0, 0);
    if (dev == NULL)
	errx(1, "Couldn't bridge");

    err = pci_device_cfg_read_u8(bridge, &stepping, 8);
    if (err != 0) {
	fprintf(stderr, "Couldn't read revision ID: %s\n", strerror(err));
	exit(1);
    }

    switch (dev->device_id) {
    case PCI_CHIP_I915_G:
	if (stepping < 0x04)
	    step_desc = "<B1";
	else if (stepping == 0x04)
	    step_desc = "B1";
	else if (stepping == 0x0e)
	    step_desc = "C2";
	else if (stepping > 0x0e)
	    step_desc = ">C2";
	else
	    step_desc = ">B1 <C2";
	break;
    case PCI_CHIP_I915_GM:
	if (stepping < 0x03)
	    step_desc = "<B1";
	else if (stepping == 0x03)
	    step_desc = "B1/C0";
	else if (stepping == 0x04)
	    step_desc = "C1/C2";
	else
	    step_desc = ">C2";
	break;
    case PCI_CHIP_I945_GM:
	if (stepping < 0x03)
	    step_desc = "<A3";
	else if (stepping == 0x03)
	    step_desc = "A3";
	else
	    step_desc = ">A3";
	break;
    case PCI_CHIP_I965_G:
    case PCI_CHIP_I965_Q:
	if (stepping < 0x02)
	    step_desc = "<C1";
	else if (stepping == 0x02)
	    step_desc = "C1/C2";
	else
	    step_desc = ">C2";
	break;
    case PCI_CHIP_I965_GM:
	if (stepping < 0x03)
	    step_desc = "<C0";
	else if (stepping == 0x03)
	    step_desc = "C0";
	else
	    step_desc = ">C0";
	break;
    case PCI_CHIP_G35_G:
	if (stepping < 0x03)
	    step_desc = "<E0";
	else if (stepping == 0x03)
	    step_desc = "E0";
	else
	    step_desc = ">E0";
	break;
    }

    printf("Vendor: 0x%04x, Device: 0x%04x, Revision: 0x%02x (%s)\n",
	   dev->vendor_id,
	   dev->device_id,
	   stepping,
	   step_desc);
    return 0;
}
