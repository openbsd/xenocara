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
#include <unistd.h>

#include "reg_dumper.h"
#include "../i810_reg.h"

struct idle_flags {
    uint32_t instdone_flag;
    char *name;
    unsigned int count;
};

struct idle_flags i965_idle_flags[] = {
    {I965_SF_DONE, "SF"},
    {I965_SE_DONE, "SE"},
    {I965_WM_DONE, "WM"},
    {I965_TEXTURE_FETCH_DONE, "texture fetch"},
    {I965_SAMPLER_CACHE_DONE, "sampler cache"},
    {I965_FILTER_DONE, "filter"},
    {I965_PS_DONE, "PS"},
    {I965_CC_DONE, "CC"},
    {I965_MAP_FILTER_DONE, "map filter"},
    {I965_MAP_L2_IDLE, "map L2"},
    {I965_CP_DONE, "CP"},
    {0, "total"},
    {0, "other"},
};

/* Fills in the "other" and "total" fields' idle flags */
static void
setup_other_flags(struct idle_flags *idle_flags, int idle_flag_count)
{
    uint32_t other_idle_flags, total_idle_flags = 0;
    int i;

    other_idle_flags = ~(I965_RING_0_ENABLE);
    for (i = 0; i < idle_flag_count - 2; i++) {
	other_idle_flags &= ~idle_flags[i].instdone_flag;
	total_idle_flags |= idle_flags[i].instdone_flag;
    }
    idle_flags[i - 1].instdone_flag = total_idle_flags;
    idle_flags[i].instdone_flag = other_idle_flags;
}

int main(int argc, char **argv)
{
    struct pci_device *dev;
    I830Rec i830;
    I830Ptr pI830 = &i830;
    ScrnInfoRec scrn;
    int err, mmio_bar;
    void *mmio;
    struct idle_flags *idle_flags;
    int idle_flag_count;

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

    i830.PciInfo = &i830.pci_info_rec;
    i830.PciInfo->chipType = dev->device_id;

    i830.pci_dev = dev;

    mmio_bar = IS_I9XX((&i830)) ? 0 : 1;

    err = pci_device_map_range (dev,
				dev->regions[mmio_bar].base_addr,
				dev->regions[mmio_bar].size, 
				PCI_DEV_MAP_FLAG_WRITABLE,
				&mmio);

    if (err != 0) {
	fprintf(stderr, "Couldn't map MMIO region: %s\n", strerror(err));
	exit(1);
    }
    i830.mmio = mmio;

    scrn.scrnIndex = 0;
    scrn.pI830 = &i830;

    /* if (IS_I965) { */
    idle_flags = i965_idle_flags;
    idle_flag_count = sizeof(i965_idle_flags) / sizeof(i965_idle_flags[0]);

    setup_other_flags(idle_flags, idle_flag_count);

    for (;;) {
	int i, j;

	for (i = 0; i < 100; i++) {
	    uint32_t instdone = INREG(INST_DONE_I965);

	    for (j = 0; j < idle_flag_count; j++) {
		if ((instdone & idle_flags[j].instdone_flag) !=
		    idle_flags[j].instdone_flag)
		    idle_flags[j].count++;
	    }

	    usleep (10000);
	}

	for (j = 0; j < idle_flag_count; j++) {
	    printf("%15s: %3d\n", idle_flags[j].name, idle_flags[j].count);
	    idle_flags[j].count = 0;
	}
	printf("\n");
    }

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
