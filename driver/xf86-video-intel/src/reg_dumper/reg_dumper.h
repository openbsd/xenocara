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

#include <inttypes.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pciaccess.h>
#include "common.h"

/** @file
 * This file defines the typedefs and stub structures necessary for us to
 * use i830_debug.c mostly unmodified.
 */

typedef char Bool;

#define FALSE 0
#define TRUE 1

#define X_INFO 0
#define X_WARNING 1
#define X_ERROR 2

typedef struct _i830 {
	/* Fields in common with the real pI830 */
	struct pci_device *PciInfo;
	Bool use_drm_mode;

	/* Fields used for setting up reg_dumper */
	volatile unsigned char *mmio;
} I830Rec, *I830Ptr;

typedef struct _scrn {
	/* Fields in common with the real pScrn */
	int scrnIndex;

	/* Fields used for setting up reg_dumper */
	I830Ptr pI830;
} ScrnInfoRec, *ScrnInfoPtr;

#define I830PTR(pScrn) (pScrn->pI830)

#define ARRAY_SIZE(x) (sizeof(x) / sizeof(x[0]))

#define INREG8(reg) (*(volatile uint8_t *)((pI830)->mmio + (reg)))
#define INREG16(reg) (*(volatile uint16_t *)((pI830)->mmio + (reg)))
#define INREG(reg) (*(volatile uint32_t *)((pI830)->mmio + (reg)))
#define OUTREG8(reg, val) \
	*(volatile uint8_t *)((pI830)->mmio + (reg)) = (val)
#define OUTREG16(reg, val) \
	*(volatile uint16_t *)((pI830)->mmio + (reg)) = (val)
#define OUTREG(reg, val) \
	*(volatile uint32_t *)((pI830)->mmio + (reg)) = (val)

#define xalloc malloc
#define xfree free
#define ErrorF printf

char *XNFprintf(const char *format, ...);
void xf86DrvMsg(int scrnIndex, int severity, const char *format, ...);
void i830DumpRegs(ScrnInfoPtr pScrn);
void intel_i830rec_init(I830Ptr pI830);
