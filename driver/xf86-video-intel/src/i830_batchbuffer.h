/**************************************************************************

Copyright 1998-1999 Precision Insight, Inc., Cedar Park, Texas.
Copyright © 2002 David Dawes

All Rights Reserved.

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sub license, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice (including the
next paragraph) shall be included in all copies or substantial portions
of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.
IN NO EVENT SHALL PRECISION INSIGHT AND/OR ITS SUPPLIERS BE LIABLE FOR
ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

**************************************************************************/

#ifndef _INTEL_BATCHBUFFER_H
#define _INTEL_BATCHBUFFER_H

#define BATCH_RESERVED		16

void intel_batch_init(ScrnInfoPtr pScrn);
void intel_batch_teardown(ScrnInfoPtr pScrn);
void intel_batch_flush(ScrnInfoPtr pScrn, Bool flushed);

static inline int
intel_batch_space(I830Ptr pI830)
{
    return (pI830->batch_bo->size - BATCH_RESERVED) - (pI830->batch_used);
}

static inline void
intel_batch_require_space(ScrnInfoPtr pScrn, I830Ptr pI830, GLuint sz)
{
    assert(sz < pI830->batch_bo->size - 8);
    if (intel_batch_space(pI830) < sz)
	intel_batch_flush(pScrn, FALSE);
}

static inline void
intel_batch_start_atomic(ScrnInfoPtr pScrn, unsigned int sz)
{
    I830Ptr pI830 = I830PTR(pScrn);

    assert(!pI830->in_batch_atomic);
    intel_batch_require_space(pScrn, pI830, sz * 4);

    pI830->in_batch_atomic = TRUE;
    pI830->batch_atomic_limit = pI830->batch_used + sz * 4;
}

static inline void
intel_batch_end_atomic(ScrnInfoPtr pScrn)
{
    I830Ptr pI830 = I830PTR(pScrn);

    assert(pI830->in_batch_atomic);
    assert(pI830->batch_used <= pI830->batch_atomic_limit);
    pI830->in_batch_atomic = FALSE;
}

static inline void
intel_batch_emit_dword(I830Ptr pI830, uint32_t dword)
{
    assert(pI830->batch_ptr != NULL);
    assert(intel_batch_space(pI830) >= 4);
    *(uint32_t *)(pI830->batch_ptr + pI830->batch_used) = dword;
    pI830->batch_used += 4;
}

static inline void
intel_batch_emit_reloc (I830Ptr  pI830,
			dri_bo  *bo,
			uint32_t read_domains,
			uint32_t write_domains,
			uint32_t delta)
{
    assert(intel_batch_space(pI830) >= 4);
    *(uint32_t *)(pI830->batch_ptr + pI830->batch_used) = bo->offset + delta;
    dri_bo_emit_reloc(pI830->batch_bo, read_domains, write_domains, delta,
		      pI830->batch_used, bo);
    pI830->batch_used += 4;
}

static inline void
intel_batch_emit_reloc_pixmap(I830Ptr pI830, PixmapPtr pPixmap,
			      uint32_t read_domains, uint32_t write_domain,
			      uint32_t delta)
{
#if I830_USE_UXA || I830_USE_EXA
    dri_bo *bo = i830_get_pixmap_bo(pPixmap);
#endif
    uint32_t offset;
    assert(pI830->batch_ptr != NULL);
    assert(intel_batch_space(pI830) >= 4);
#if I830_USE_UXA || I830_USE_EXA
    if (bo) {
	intel_batch_emit_reloc(pI830, bo, read_domains, write_domain, delta);
	return;
    }
#endif
    offset = intel_get_pixmap_offset(pPixmap);
    *(uint32_t *)(pI830->batch_ptr + pI830->batch_used) = offset + delta;
    pI830->batch_used += 4;
}

#define OUT_BATCH(dword) intel_batch_emit_dword(pI830, dword)

#define OUT_RELOC(bo, read_domains, write_domains, delta) \
	intel_batch_emit_reloc (pI830, bo, read_domains, write_domains, delta)

#define OUT_RELOC_PIXMAP(pPixmap, reads, write, delta)	\
	intel_batch_emit_reloc_pixmap(pI830, pPixmap, reads, write, delta)

union intfloat {
	float f;
	unsigned int ui;
};

#define OUT_BATCH_F(x) do {			\
	union intfloat tmp;			\
	tmp.f = (float)(x);			\
	OUT_BATCH(tmp.ui);			\
} while(0)

#define BEGIN_BATCH(n)							\
do {									\
    if (pI830->batch_emitting != 0)					\
	FatalError("%s: BEGIN_BATCH called without closing "		\
		   "ADVANCE_BATCH\n", __FUNCTION__);			\
    intel_batch_require_space(pScrn, pI830, (n) * 4);			\
    pI830->batch_emitting = (n) * 4;					\
    pI830->batch_emit_start = pI830->batch_used;			\
} while (0)

#define ADVANCE_BATCH() do {						\
    if (pI830->batch_emitting == 0)					\
	FatalError("%s: ADVANCE_BATCH called with no matching "		\
		   "BEGIN_BATCH\n", __FUNCTION__);			\
    if (pI830->batch_used > pI830->batch_emit_start + pI830->batch_emitting) \
	FatalError("%s: ADVANCE_BATCH: exceeded allocation %d/%d\n ",	\
		   __FUNCTION__,					\
		   pI830->batch_used - pI830->batch_emit_start,		\
		   pI830->batch_emitting);				\
    if (pI830->batch_used < pI830->batch_emit_start + pI830->batch_emitting) \
	FatalError("%s: ADVANCE_BATCH: under-used allocation %d/%d\n ", \
		   __FUNCTION__,					\
		   pI830->batch_used - pI830->batch_emit_start,		\
		   pI830->batch_emitting);				\
    if ((pI830->batch_emitting > 8) && (I810_DEBUG & DEBUG_ALWAYS_SYNC)) { \
	/* Note: not actually syncing, just flushing each batch. */	\
	intel_batch_flush(pScrn, FALSE);					\
    }									\
    pI830->batch_emitting = 0;						\
} while (0)

#endif /* _INTEL_BATCHBUFFER_H */
