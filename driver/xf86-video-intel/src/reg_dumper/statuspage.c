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
#include <sys/types.h>
#include <sys/mman.h>
#include <pciaccess.h>
#include <err.h>
#include <unistd.h>
#include <fcntl.h>

#include "reg_dumper.h"
#include "../i810_reg.h"

int main(int argc, char **argv)
{
    I830Rec i830;
    I830Ptr pI830 = &i830;
    int devmem;
    uint32_t hws_offset;
    volatile uint32_t *hws;

    intel_i830rec_init(pI830);

    if (HWS_NEED_GFX(pI830))
	errx(1, "status page in graphics virtual unsupported.\n");

    hws_offset = INREG(HWS_PGA);

    devmem = open("/dev/mem", O_RDWR, 0);
    if (devmem == -1)
	err(1, "Couldn't open /dev/mem");

    hws = mmap(NULL, 4096, PROT_READ, MAP_SHARED, devmem, hws_offset);
    if (hws == MAP_FAILED)
	err(1, "Couldn't map /dev/mem at 0x%08x", hws_offset);

    close(devmem);

    for (;;) {
	int i;

	printf("\n");

	for (i = 0; i < 64; i += 4) {
	    printf("0x%04x: 0x%08x 0x%08x 0x%08x 0x%08x\n", i * 4,
		   hws[i], hws[i + 1], hws[i + 2], hws[i + 3]);
	}

	sleep(1);
    }

    return 0;
}
