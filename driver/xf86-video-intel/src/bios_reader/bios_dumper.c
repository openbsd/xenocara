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
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <pciaccess.h>
#include <err.h>

#ifndef DEFFILEMODE
#define DEFFILEMODE (S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH) /* 0666*/
#endif

static void usage(void)
{
    fprintf(stderr, "usage: bios_dumper <filename>\n");
    exit(1);
}

int main(int argc, char **argv)
{
    struct pci_device *dev;
    void *bios;
    int err, fd;

    if (argc != 2)
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

    bios = malloc(dev->rom_size);
    if (bios == NULL)
	errx(1, "Couldn't allocate memory for BIOS data\n");

    err = pci_device_read_rom(dev, bios);
    if (err != 0) {
	fprintf(stderr, "Couldn't read graphics card ROM: %s\n",
		strerror(err));
	exit(1);
    }

    fd = open(argv[1], O_RDWR | O_CREAT | O_TRUNC, DEFFILEMODE);
    if (fd < 0) {
	fprintf(stderr, "Couldn't open output: %s\n", strerror(errno));
	exit(1);
    }

    if (write(fd, bios, dev->rom_size) < dev->rom_size) {
	fprintf(stderr, "Couldn't write BIOS data: %s\n", strerror(errno));
	exit(1);
    }

    close(fd);
    pci_system_cleanup();

    return 0;
}
