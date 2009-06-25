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
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 * Authors:
 *    Eric Anholt <eric@anholt.net>
 *    Jesse Barnes <jesse.barnes@intel.com>
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <pciaccess.h>
#include <err.h>

#include "../i810_reg.h"
#include "../i830_bios.h"

#ifndef DEFFILEMODE
#define DEFFILEMODE (S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH) /* 0666*/
#endif

static uint32_t read32(void *base, int reg)
{
    uint32_t *addr = (uint32_t *)((unsigned char *)(base) + reg);

    return *addr;
}

#if 0
static void write32(void *base, int reg, uint32_t val)
{
    uint32_t *addr = (uint32_t *)((unsigned char *)(base) + reg);
    *addr = val;
}
#endif

static void usage(void)
{
    fprintf(stderr, "usage: swf_dumper\n");
    exit(1);
}

int main(int argc, char **argv)
{
    struct pci_device *dev;
    int err;
    void *addr;

    if (argc != 1)
	usage();

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

    err = pci_device_map_range(dev, dev->regions[0].base_addr,
			       dev->regions[0].size,
			       PCI_DEV_MAP_FLAG_WRITABLE, &addr);
    if (err) {
	fprintf(stderr, "Couldn't map MMIO space: %s\n", strerror(err));
	exit(1);
    }

    printf("SWF14: 0x%08x\n", read32(addr, SWF14));

    pci_system_cleanup();

    return 0;
}
