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
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <pciaccess.h>
#include <err.h>
#include <unistd.h>

#include "reg_dumper.h"
#include "../i810_reg.h"

#define INGTT(offset) (*(volatile uint32_t *)(gtt + (offset) / (KB(4) / 4)))

int main(int argc, char **argv)
{
	I830Rec i830;
	I830Ptr pI830 = &i830;
	int start, aper_size;
	unsigned char *gtt;

	intel_i830rec_init(pI830);

	if (!IS_I9XX(pI830)) {
		printf("Unsupported chipset for gtt dumper\n");
		exit(1);
	}

	if (IS_G4X(pI830) || IS_IGDNG(pI830))
		gtt = (unsigned char *)(pI830->mmio + MB(2));
	else if (IS_I965G(pI830))
		gtt = (unsigned char *)(pI830->mmio + KB(512));
	else {
		/* 915/945 chips has GTT range in bar 3 */
		int err = 0;
		err = pci_device_map_range(pI830->PciInfo,
					   pI830->PciInfo->regions[3].base_addr,
					   pI830->PciInfo->regions[3].size,
					   PCI_DEV_MAP_FLAG_WRITABLE,
					   (void **)&gtt);
		if (err != 0) {
			fprintf(stderr, "mapping GTT bar failed\n");
			exit(1);
		}
	}

	aper_size = pI830->PciInfo->regions[2].size;

	for (start = 0; start < aper_size; start += KB(4)) {
		uint32_t start_pte = INGTT(start);
		uint32_t end;
		int constant_length = 0;
		int linear_length = 0;

		/* Check if it's a linear sequence */
		for (end = start + KB(4); end < aper_size; end += KB(4)) {
			uint32_t end_pte = INGTT(end);
			if (end_pte == start_pte + (end - start))
				linear_length++;
			else
				break;
		}
		if (linear_length > 0) {
			printf("0x%08x - 0x%08x: linear from "
			       "0x%08x to 0x%08x\n",
			       start, end - KB(4),
			       start_pte, start_pte + (end - start) - KB(4));
			start = end - KB(4);
			continue;
		}

		/* Check if it's a constant sequence */
		for (end = start + KB(4); end < aper_size; end += KB(4)) {
			uint32_t end_pte = INGTT(end);
			if (end_pte == start_pte)
				constant_length++;
			else
				break;
		}
		if (constant_length > 0) {
			printf("0x%08x - 0x%08x: constant 0x%08x\n",
			       start, end - KB(4), start_pte);
			start = end - KB(4);
			continue;
		}

		printf("0x%08x: 0x%08x\n", start, start_pte);
	}

	return 0;
}
