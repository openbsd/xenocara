
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

/* Driver data structures */
#include "radeon.h"

/* Allocates memory, either by resizing the allocation pointed to by mem_struct,
 * or by freeing mem_struct (if non-NULL) and allocating a new space.  The size
 * is measured in bytes, and the offset from the beginning of card space is
 * returned.
 */
uint32_t
radeon_legacy_allocate_memory(ScrnInfoPtr pScrn,
		       void **mem_struct,
		       int size,
		       int align,
		       int domain)
{
    ScreenPtr pScreen = xf86ScrnToScreen(pScrn);
    RADEONInfoPtr info = RADEONPTR(pScrn);
    uint32_t offset = 0;

#ifdef XF86DRM_MODE
    if (info->cs) {
	struct radeon_bo *video_bo;

	if (*mem_struct)
		radeon_legacy_free_memory(pScrn, *mem_struct);

	video_bo = radeon_bo_open(info->bufmgr, 0, size, align, domain, 0);

	*mem_struct = video_bo;

	if (!video_bo)
	    return 0;

	return (uint32_t)-1;
    }
#endif
#ifdef USE_EXA
    if (info->useEXA) {
	ExaOffscreenArea *area = *mem_struct;

	if (area != NULL) {
	    if (area->size >= size)
		return area->offset;

	    exaOffscreenFree(pScreen, area);
	}

	area = exaOffscreenAlloc(pScreen, size, align, TRUE,
				 NULL, NULL);

	*mem_struct = area;
	if (area == NULL)
	    return 0;
	offset = area->offset;
    }
#endif /* USE_EXA */
#ifdef USE_XAA
    if (!info->useEXA) {
	FBLinearPtr linear = *mem_struct;
	int cpp = info->CurrentLayout.bitsPerPixel / 8;

	/* XAA allocates in units of pixels at the screen bpp, so adjust size
	 * appropriately.
	 */
	size = (size + cpp - 1) / cpp;
	align = (align + cpp - 1) / cpp;

	if (linear) {
	    if(linear->size >= size)
		return linear->offset * cpp;

	    if(xf86ResizeOffscreenLinear(linear, size))
		return linear->offset * cpp;

	    xf86FreeOffscreenLinear(linear);
	}

	linear = xf86AllocateOffscreenLinear(pScreen, size, align,
					     NULL, NULL, NULL);
	*mem_struct = linear;

	if (!linear) {
	    int max_size;

	    xf86QueryLargestOffscreenLinear(pScreen, &max_size, align,
					    PRIORITY_EXTREME);

	    if (max_size < size)
		return 0;

	    xf86PurgeUnlockedOffscreenAreas(pScreen);
	    linear = xf86AllocateOffscreenLinear(pScreen, size, align,
						 NULL, NULL, NULL);
	    *mem_struct = linear;
	    if (!linear)
		return 0;
	}
	offset = linear->offset * cpp;
    }
#endif /* USE_XAA */

    return offset;
}

void
radeon_legacy_free_memory(ScrnInfoPtr pScrn,
		   void *mem_struct)
{
    RADEONInfoPtr info = RADEONPTR(pScrn);

#ifdef XF86DRM_MODE
    if (info->cs) {
        struct radeon_bo *bo = mem_struct;
	radeon_bo_unref(bo);
	return;
    }
#endif
#ifdef USE_EXA
    ScreenPtr pScreen = xf86ScrnToScreen(pScrn);

    if (info->useEXA) {
	ExaOffscreenArea *area = mem_struct;

	if (area != NULL)
	    exaOffscreenFree(pScreen, area);
	area = NULL;
    }
#endif /* USE_EXA */
#ifdef USE_XAA
    if (!info->useEXA) {
	FBLinearPtr linear = mem_struct;

	if (linear != NULL)
	    xf86FreeOffscreenLinear(linear);
	linear = NULL;
    }
#endif /* USE_XAA */
}
