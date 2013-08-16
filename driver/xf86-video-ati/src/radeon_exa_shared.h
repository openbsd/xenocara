/*
 * Copyright 2005 Eric Anholt
 * Copyright 2005 Benjamin Herrenschmidt
 * Copyright 2008 Advanced Micro Devices, Inc.
 * All Rights Reserved.
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
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * Authors:
 *    Eric Anholt <anholt@FreeBSD.org>
 *    Zack Rusin <zrusin@trolltech.com>
 *    Benjamin Herrenschmidt <benh@kernel.crashing.org>
 *    Alex Deucher <alexander.deucher@amd.com>
 *    Matthias Hopf <mhopf@suse.de>
 */
#ifndef RADEON_EXA_SHARED_H

#define RADEON_EXA_SHARED_H

extern PixmapPtr RADEONGetDrawablePixmap(DrawablePtr pDrawable);

extern void RADEONVlineHelperClear(ScrnInfoPtr pScrn);
extern void RADEONVlineHelperSet(ScrnInfoPtr pScrn, int x1, int y1, int x2, int y2);
extern Bool RADEONValidPM(uint32_t pm, int bpp);
extern Bool RADEONCheckBPP(int bpp);
extern PixmapPtr RADEONSolidPixmap(ScreenPtr pScreen, uint32_t solid);

#define RADEON_TRACE_FALL 0
#define RADEON_TRACE_DRAW 0

#if RADEON_TRACE_FALL
#define RADEON_FALLBACK(x)     		\
do {					\
	ErrorF("%s: ", __FUNCTION__);	\
	ErrorF x;			\
	return FALSE;			\
} while (0)
#else
#define RADEON_FALLBACK(x) return FALSE
#endif

#if RADEON_TRACE_DRAW
#define TRACE do { ErrorF("TRACE: %s\n", __FUNCTION__); } while(0)
#else
#define TRACE
#endif

static inline void radeon_add_pixmap(struct radeon_cs *cs, PixmapPtr pPix, int read_domains, int write_domain)
{
    struct radeon_exa_pixmap_priv *driver_priv = exaGetPixmapDriverPrivate(pPix);

    radeon_cs_space_add_persistent_bo(cs, driver_priv->bo, read_domains, write_domain);
}

extern void radeon_ib_discard(ScrnInfoPtr pScrn);

extern int radeon_cp_start(ScrnInfoPtr pScrn);
extern void radeon_vb_no_space(ScrnInfoPtr pScrn, struct radeon_vbo_object *vbo, int vert_size);
extern void radeon_vbo_done_composite(PixmapPtr pDst);

#endif
