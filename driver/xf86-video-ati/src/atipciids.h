/*
 * Copyright (c) 1995-2003 by The XFree86 Project, Inc.
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

/*
 * This file is a replacement for xf86PciInfo.h moving ATI related PCI IDs
 * locally to the driver module
 */

#ifndef _ATIPCIIDS_H
#define _ATIPCIIDS_H

/* PCI Vendor */
#define PCI_VENDOR_ASUS			0x1043
#define PCI_VENDOR_ATI			0x1002
#define PCI_VENDOR_IBM			0x1014
#define PCI_VENDOR_AMD			0x1022
#define PCI_VENDOR_DELL			0x1028
#define PCI_VENDOR_VIA			0x1106
#define PCI_VENDOR_INTEL		0x8086
#ifndef PCI_VENDOR_HP
#define PCI_VENDOR_HP			0x103c
#endif
#define PCI_VENDOR_SONY			0x104D

#include "ati_pciids_gen.h"

/* Misc */
#define PCI_CHIP_AMD761			0x700E

#endif /* _ATIPCIIDS_H */
