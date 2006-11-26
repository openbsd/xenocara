/* savage_sarea.h -- Public header for the savage driver (SAREA)
 *
 * Copyright 2004  Felix Kuehling
 * All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sub license,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial portions
 * of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. IN NO EVENT SHALL
 * VIA, S3 GRAPHICS, AND/OR ITS SUPPLIERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#ifndef __SAVAGE_SAREA_H__
#define __SAVAGE_SAREA_H__

#ifndef __SAVAGE_SAREA_DEFINES__
#define __SAVAGE_SAREA_DEFINES__

/* 2 heaps (1 for card, 1 for agp), each divided into upto 128
 * regions, subject to a minimum region size of (1<<16) == 64k.
 *
 * Clients may subdivide regions internally, but when sharing between
 * clients, the region size is the minimum granularity.
 */

#define SAVAGE_CARD_HEAP		0
#define SAVAGE_AGP_HEAP			1
#define SAVAGE_NR_TEX_HEAPS		2
#define SAVAGE_NR_TEX_REGIONS		16
#define SAVAGE_LOG_MIN_TEX_REGION_SIZE	16

#endif /* __SAVAGE_SAREA_DEFINES__ */

typedef struct {
	/* LRU lists for texture memory in agp space and on the card.
	 */
	drm_tex_region_t texList[SAVAGE_NR_TEX_HEAPS][SAVAGE_NR_TEX_REGIONS+1];
	unsigned int texAge[SAVAGE_NR_TEX_HEAPS];

	/* Mechanism to validate card state.
	 */
	int ctxOwner;
}  SAVAGESAREAPrivRec, *SAVAGESAREAPrivPtr;

#endif /* __SAVAGE_SAREA_H__ */
