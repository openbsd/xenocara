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
#include <err.h>

#include "reg_dumper.h"

int main(int argc, char **argv)
{
	struct pci_device *dev;
	I830Rec i830;
	ScrnInfoRec scrn;
	int err, mmio_bar;
	void *mmio;

	err = pci_system_init();
	if (err != 0) {
		fprintf(stderr, "Couldn't initialize PCI system: %s\n",
			strerror(err));
		exit(1);
	}

	/* Grab the graphics card */
	dev = pci_device_find_by_slot(0, 0, 2, 0);
	if (dev == NULL)
		errx(1, "Couldn't find graphics card");

	err = pci_device_probe(dev);
	if (err != 0) {
		fprintf(stderr, "Couldn't probe graphics card: %s\n",
			strerror(err));
		exit(1);
	}

	if (dev->vendor_id != 0x8086)
		errx(1, "Graphics card is non-intel");

	i830.PciInfo = dev;

	mmio_bar = IS_I9XX((&i830)) ? 0 : 1;

	err = pci_device_map_range(dev,
				   dev->regions[mmio_bar].base_addr,
				   dev->regions[mmio_bar].size,
				   PCI_DEV_MAP_FLAG_WRITABLE, &mmio);

	if (err != 0) {
		fprintf(stderr, "Couldn't map MMIO region: %s\n",
			strerror(err));
		exit(1);
	}
	i830.mmio = mmio;

	scrn.scrnIndex = 0;
	scrn.pI830 = &i830;

	i830DumpRegs(&scrn);

	return 0;
}

void xf86DrvMsg(int scrnIndex, int severity, const char *format, ...)
{
	va_list va;

	switch (severity) {
	case X_INFO:
		printf("(II): ");
		break;
	case X_WARNING:
		printf("(WW): ");
		break;
	case X_ERROR:
		printf("(EE): ");
		break;
	}

	va_start(va, format);
	vprintf(format, va);
	va_end(va);
}
