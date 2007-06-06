/* $OpenBSD: openbsd_pci.c,v 1.1 2007/06/06 21:01:25 matthieu Exp $ */
/*
 * (C) Copyright Eric Anholt 2006
 * (C) Copyright IBM Corporation 2006
 * All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * on the rights to use, copy, modify, merge, publish, distribute, sub
 * license, and/or sell copies of the Software, and to permit persons to whom
 * the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.  IN NO EVENT SHALL
 * IBM AND/OR THEIR SUPPLIERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

/**
 * \file openbsd_pci.c
 *
 * Access the kernel PCI support using /dev/pci's ioctl and mmap interface.
 *
 * \author Eric Anholt <eric@anholt.net>
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/pciio.h>
#include <sys/mman.h>

#include "pciaccess.h"
#include "pciaccess_private.h"

/**
 * OpenBSD private pci_system structure that extends the base pci_system
 * structure.
 *
 * It is initialized once and used as a global, just as pci_system is used.
 */
struct openbsd_pci_system {
	struct pci_system pci_sys; /* must come first */
	int pcidev;		/**< fd for /dev/pci */
} *openbsd_pci_sys;

/**
 * Map a memory region for a device using /dev/mem.
 *
 * \param dev          Device whose memory region is to be mapped.
 * \param region       Region, on the range [0, 5], that is to be mapped.
 * \param write_enable Map for writing (non-zero).
 *
 * \return
 * Zero on success or an \c errno value on failure.
 */
static int
pci_device_openbsd_map(struct pci_device *dev, unsigned int region,
    int write_enable)
{
}

/**
 * Unmap the specified region.
 *
 * \param dev          Device whose memory region is to be unmapped.
 * \param region       Region, on the range [0, 5], that is to be unmapped.
 *
 * \return
 * Zero on success or an \c errno value on failure.
 */
static int
pci_device_openbsd_unmap(struct pci_device *dev, unsigned int region)
{
}

static int
pci_device_openbsd_read(struct pci_device *dev, void *data,
    pciaddr_t offset, pciaddr_t size, pciaddr_t *bytes_read)
{
}

static int
pci_device_openbsd_write(struct pci_device *dev, const void *data,
    pciaddr_t offset, pciaddr_t size, pciaddr_t *bytes_written)
{
}


static int
pci_device_openbsd_probe(struct pci_device *dev)
{
}

static int
pci_device_openbsd_read_rom(struct pci_device *dev, void *buffer)
{
}


static void
pci_system_openbsd_destroy(void)
{
	free(openbsd_pci_sys->pci_sys.devices);
	openbsd_pci_sys = NULL;
}

static const struct pci_system_methods openbsd_pci_methods = {
	.destroy = pci_system_openbsd_destroy,
	.destroy_device = NULL,
	.read_rom = pci_device_openbsd_read_rom, 
	.probe = pci_device_openbsd_probe,
	.map = pci_device_openbsd_map,
	.unmap = pci_device_openbsd_unmap,
	.read = pci_device_openbsd_read,
	.write = pci_device_openbsd_write,
	.fill_capabilities = pci_fill_capabilities_generic,
};

/**
 * Attempt to acces the OpenBSD PCI interface.
 */
int
pci_system_openbsd_create(void)
{
	openbsd_pci_sys = calloc(1, sizeof(struct openbsd_pci_system));
	if (openbsd_pci_sys = NULL)
		return ENOMEM;
	pci_sys = &openbsd_pci_sys->pci_sys;
	pci_sys->methods = &openbsd_pci_methods;
	
	return 0;
}
