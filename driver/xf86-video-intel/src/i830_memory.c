/**************************************************************************

Copyright 1998-1999 Precision Insight, Inc., Cedar Park, Texas.
Copyright © 2002 by David Dawes.

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
IN NO EVENT SHALL THE COPYRIGHT HOLDERS AND/OR THEIR SUPPLIERS BE LIABLE FOR
ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

**************************************************************************/

/*
 * Authors:
 *   Keith Whitwell <keith@tungstengraphics.com>
 *   David Dawes <dawes@xfree86.org>
 *
 * Updated for Dual Head capabilities:
 *   Alan Hourihane <alanh@tungstengraphics.com>
 */

/**
 * @file i830_memory.c
 *
 * This is the video memory allocator.  Our memory allocation is different from
 * other graphics chips, where you have a fixed amount of graphics memory
 * available that you want to put to the best use.  Instead, we have almost no
 * memory pre-allocated, and we have to choose an appropriate amount of sytem
 * memory to use.
 *
 * The allocations we might do:
 *
 * - Ring buffer
 * - HW cursor block (either one block or four)
 * - Overlay registers
 * - Front buffer (screen 1)
 * - Front buffer (screen 2, only in zaphod mode)
 * - Back/depth buffer (3D only)
 * - Compatibility texture pool (optional, more is always better)
 * - New texture pool (optional, more is always better.  aperture allocation
 *     only)
 *
 * The user may request a specific amount of memory to be used
 * (intel->pEnt->videoRam != 0), in which case allocations have to fit within
 * that much aperture.  If not, the individual allocations will be
 * automatically sized, and will be fit within the maximum aperture size.
 * Only the actual memory used (not alignment padding) will get actual AGP
 * memory allocated.
 *
 * Given that the allocations listed are generally a page or more than a page,
 * our allocator will only return page-aligned offsets, simplifying the memory
 * binding process.  For smaller allocations, the acceleration architecture's
 * linear allocator is preferred.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <assert.h>
#include <inttypes.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <errno.h>

#include "xf86.h"
#include "xf86_OSproc.h"

#include "i830.h"
#include "i810_reg.h"
#include "i915_drm.h"

/* Our hardware status area is just a single page */
#define HWSTATUS_PAGE_SIZE GTT_PAGE_SIZE
#define PWRCTX_SIZE GTT_PAGE_SIZE

static i830_memory *
i830_allocate_aperture(ScrnInfoPtr scrn, const char *name,
		       unsigned long size, unsigned long pitch,
		       unsigned long alignment, int flags,
		       enum tile_format tile_format);

static int i830_set_tiling(ScrnInfoPtr scrn, unsigned int offset,
			   unsigned int pitch, unsigned int size,
			   enum tile_format tile_format);

static void i830_clear_tiling(ScrnInfoPtr scrn, unsigned int fence_nr);

/**
 * Returns the fence size for a tiled area of the given size.
 */
unsigned long i830_get_fence_size(intel_screen_private *intel, unsigned long size)
{
	unsigned long i;
	unsigned long start;

	if (IS_I965G(intel)) {
		/* The 965 can have fences at any page boundary. */
		return ALIGN(size, GTT_PAGE_SIZE);
	} else {
		/* Align the size to a power of two greater than the smallest fence
		 * size.
		 */
		if (IS_I9XX(intel))
			start = MB(1);
		else
			start = KB(512);

		for (i = start; i < size; i <<= 1) ;

		return i;
	}
}

/**
 * On some chips, pitch width has to be a power of two tile width, so
 * calculate that here.
 */
unsigned long
i830_get_fence_pitch(intel_screen_private *intel, unsigned long pitch, int format)
{
	unsigned long i;
	unsigned long tile_width = (format == I915_TILING_Y) ? 128 : 512;

	if (format == TILE_NONE)
		return pitch;

	/* 965 is flexible */
	if (IS_I965G(intel))
		return ROUND_TO(pitch, tile_width);

	/* Pre-965 needs power of two tile width */
	for (i = tile_width; i < pitch; i <<= 1) ;

	return i;
}

/**
 * On some chips, pitch width has to be a power of two tile width, so
 * calculate that here.
 */
static unsigned long i830_get_fence_alignment(intel_screen_private *intel, unsigned long size)
{
	if (IS_I965G(intel))
		return 4096;
	else
		return i830_get_fence_size(intel, size);
}

static Bool
i830_check_display_stride(ScrnInfoPtr scrn, int stride, Bool tiling)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);
	int limit = KB(32);

	/* 8xx spec has always 8K limit, but tests show larger limit in
	   non-tiling mode, which makes large monitor work. */
	if ((IS_845G(intel) || IS_I85X(intel)) && tiling)
		limit = KB(8);

	if (IS_I915(intel) && tiling)
		limit = KB(8);

	if (IS_I965G(intel) && tiling)
		limit = KB(16);

	if (IS_IGDNG(intel) && tiling)
		limit = KB(32);

	if (stride <= limit)
		return TRUE;
	else
		return FALSE;
}

static Bool i830_bind_memory(ScrnInfoPtr scrn, i830_memory *mem)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);

	if (mem == NULL || mem->bound || intel->use_drm_mode)
		return TRUE;

	if (intel->have_gem && mem->bo != NULL) {

		if (dri_bo_pin(mem->bo, mem->alignment) != 0) {
			xf86DrvMsg(scrn->scrnIndex, X_ERROR,
			    "Failed to pin %s: %s\n",
			    mem->name, strerror(errno));
			return FALSE;
		}

		mem->bound = TRUE;
		mem->offset = mem->bo->offset;
		mem->end = mem->offset + mem->size;
	} else {
		if (!intel->gtt_acquired)
			return TRUE;

		if (mem->key != -1 &&
			!xf86BindGARTMemory(scrn->scrnIndex, mem->key,
			    mem->agp_offset)) {
			return FALSE;
		}

		mem->bound = TRUE;
	}

	if (mem->tiling != TILE_NONE && !intel->kernel_exec_fencing) {
		mem->fence_nr = i830_set_tiling(scrn, mem->offset, mem->pitch,
		    mem->allocated_size, mem->tiling);
	}

	return TRUE;
}

static Bool i830_unbind_memory(ScrnInfoPtr scrn, i830_memory *mem)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);

	if (mem == NULL || !mem->bound)
		return TRUE;

	if (mem->tiling != TILE_NONE && !intel->use_drm_mode &&
	    !intel->kernel_exec_fencing)
		i830_clear_tiling(scrn, mem->fence_nr);

	if (mem->bo != NULL && !intel->use_drm_mode) {
		if (dri_bo_unpin(mem->bo) == 0) {
			mem->bound = FALSE;
			/* Give buffer obviously wrong offset/end until
			 * it's re-pinned.
			 */
			mem->offset = -1;
			mem->end = -1;
			return TRUE;
		} else {
			return FALSE;
		}
	}

	if (mem->key == -1 || xf86UnbindGARTMemory(scrn->scrnIndex, mem->key)) {
		mem->bound = FALSE;
		return TRUE;
	} else {
		return FALSE;
	}
}

void i830_free_memory(ScrnInfoPtr scrn, i830_memory * mem)
{
	if (mem == NULL)
		return;

	/* Free any AGP memory. */
	i830_unbind_memory(scrn, mem);

	if (mem->bo != NULL) {
		intel_screen_private *intel = intel_get_screen_private(scrn);
		dri_bo_unreference(mem->bo);
		if (intel->bo_list == mem) {
			intel->bo_list = mem->next;
			if (mem->next)
				mem->next->prev = NULL;
		} else {
			if (mem->prev)
				mem->prev->next = mem->next;
			if (mem->next)
				mem->next->prev = mem->prev;
		}
		xfree(mem->name);
		xfree(mem);
		return;
	}
	/* Disconnect from the list of allocations */
	if (mem->prev != NULL)
		mem->prev->next = mem->next;
	if (mem->next != NULL)
		mem->next->prev = mem->prev;

	if (mem->key != -1) {
		xf86DeallocateGARTMemory(scrn->scrnIndex, mem->key);
		mem->key = -1;
	}

	xfree(mem->name);
	xfree(mem);
}

/* Resets the state of the aperture allocator, freeing all memory that had
 * been allocated.
 */
void i830_reset_allocations(ScrnInfoPtr scrn)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);
	int p;

	/* While there is any memory between the start and end markers, free it. */
	while (intel->memory_list->next->next != NULL) {
		i830_memory *mem = intel->memory_list->next;

		/* Don't reset BO allocator, which we set up at init. */
		if (intel->memory_manager == mem) {
			mem = mem->next;
			if (mem->next == NULL)
				break;
		}

		i830_free_memory(scrn, mem);
	}

	/* Free any allocations in buffer objects */
	while (intel->bo_list != NULL)
		i830_free_memory(scrn, intel->bo_list);

	/* Null out the pointers for all the allocations we just freed.  This is
	 * kind of gross, but at least it's just one place now.
	 */
	intel->cursor_mem = NULL;
	for (p = 0; p < 2; p++) {
		intel->cursor_mem_classic[p] = NULL;
		intel->cursor_mem_argb[p] = NULL;
	}
	intel->front_buffer = NULL;
	intel->overlay_regs = NULL;
	intel->power_context = NULL;
	intel->ring.mem = NULL;
	intel->fake_bufmgr_mem = NULL;
}

/**
 * Initialize's the driver's video memory allocator to allocate in the
 * given range.
 *
 * This sets up the kernel memory manager to manage as much of the memory
 * as we think it can, while leaving enough to us to fulfill our non-GEM
 * static allocations.  Some of these exist because of the need for physical
 * addresses to reference.
 */
Bool i830_allocator_init(ScrnInfoPtr scrn, unsigned long size)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);
	i830_memory *start, *end;
	struct drm_i915_setparam sp;

	start = xcalloc(1, sizeof(*start));
	if (start == NULL)
		return FALSE;
	start->name = xstrdup("start marker");
	if (start->name == NULL) {
		xfree(start);
		return FALSE;
	}
	end = xcalloc(1, sizeof(*end));
	if (end == NULL) {
		xfree(start->name);
		xfree(start);
		return FALSE;
	}
	end->name = xstrdup("end marker");
	if (end->name == NULL) {
		xfree(start->name);
		xfree(start);
		xfree(end);
		return FALSE;
	}

	start->key = -1;
	start->offset = 0;
	start->end = start->offset;
	start->size = 0;
	start->next = end;
	end->key = -1;
	end->offset = size;
	end->end = end->offset;
	end->size = 0;
	end->prev = start;

	intel->memory_list = start;

	/* Now that we have our manager set up, give the kernel a piece of the
	 * aperture for GEM buffer object mapping. This is only needed for UXA
	 * and/or DRI2 when the kernel hasn't already managed this itself under
	 * KMS.  We need libdri interface5.4 or newer so we can rely on the lock
	 * being held after DRIScreenInit, rather than after DRIFinishScreenInit.
	 */

	if (!intel->use_drm_mode) {
		int mmsize;

		/* Take over all of the graphics aperture minus enough to for
		 * physical-address allocations of cursor/overlay registers.
		 */
		mmsize = size;

		/* Overlay and cursors, if physical, need to be allocated
		 * outside of the kernel memory manager.
		 */
		if (!OVERLAY_NOPHYSICAL(intel) && !OVERLAY_NOEXIST(intel)) {
			mmsize -= ROUND_TO(OVERLAY_SIZE, GTT_PAGE_SIZE);
		}
		if (intel->CursorNeedsPhysical) {
			mmsize -= 2 * (ROUND_TO(HWCURSOR_SIZE, GTT_PAGE_SIZE) +
			ROUND_TO(HWCURSOR_SIZE_ARGB, GTT_PAGE_SIZE));
		}
		if (intel->fb_compression)
			mmsize -= MB(6) + ROUND_TO_PAGE(FBC_LL_SIZE +
			    FBC_LL_PAD);

		/* Can't do GEM on stolen memory */
		mmsize -= intel->stolen_size;

		/* Create the aperture allocation */
		intel->memory_manager =
		   i830_allocate_aperture(scrn, "DRI memory manager",
	           mmsize, 0, GTT_PAGE_SIZE, ALIGN_BOTH_ENDS | NEED_NON_STOLEN,
	           TILE_NONE);

		if (intel->memory_manager != NULL) {
			struct drm_i915_gem_init init;
			int ret;

			sp.param = I915_SETPARAM_NUM_USED_FENCES;
			sp.value = 0; /* kernel gets them all */

			ret = drmCommandWrite(intel->drmSubFD,
			    DRM_I915_SETPARAM, &sp, sizeof(sp));
			if (ret == 0)
				intel->kernel_exec_fencing = TRUE;
			init.gtt_start = intel->memory_manager->offset;
			init.gtt_end = intel->memory_manager->offset +
			    intel->memory_manager->size;

			/* Tell the kernel to manage it */
			ret = ioctl(intel->drmSubFD, DRM_IOCTL_I915_GEM_INIT,
			    &init);
			if (ret == 0) {
				intel->have_gem = TRUE;
				i830_init_bufmgr(scrn);
			} else {
				xf86DrvMsg(scrn->scrnIndex, X_ERROR,
				    "Failed to initialize kernel memory manager\n");
				i830_free_memory(scrn, intel->memory_manager);
				intel->memory_manager = NULL;
			}
		} else {
			xf86DrvMsg(scrn->scrnIndex, X_ERROR,
			    "Failed to allocate space for kernel memory manager\n");
		}
	}

	return TRUE;
}

void i830_allocator_fini(ScrnInfoPtr scrn)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);

	/* The memory manager is more special */ if
	(intel->memory_manager) {
		i830_free_memory(scrn, intel->memory_manager);
		intel->memory_manager = NULL;
	}

	/* Free the start/end markers */
	free(intel->memory_list->next);
	free(intel->memory_list);
	intel->memory_list = NULL;
}

/**
 * Reads a GTT entry for the memory at the given offset and returns the
 * physical address.
 *
 * \return physical address if successful.
 * \return (uint64_t)-1 if unsuccessful.
 */
static uint64_t i830_get_gtt_physical(ScrnInfoPtr scrn, unsigned long offset)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);
	uint32_t gttentry;

	/* We don't have GTTBase set up on i830 yet. */
	if (intel->GTTBase == NULL)
		return -1;

	gttentry = INGTT(offset / 1024);

	/* Mask out these reserved bits on this hardware. */
	if (!IS_I9XX(intel) || IS_I915G(intel) || IS_I915GM(intel) ||
	    IS_I945G(intel) || IS_I945GM(intel)) {
		gttentry &= ~PTE_ADDRESS_MASK_HIGH;
	}

	/* If it's not a mapping type we know, then bail. */
	if ((gttentry & PTE_MAPPING_TYPE_MASK) != PTE_MAPPING_TYPE_UNCACHED &&
	    (gttentry & PTE_MAPPING_TYPE_MASK) != PTE_MAPPING_TYPE_CACHED) {
		xf86DrvMsg(scrn->scrnIndex, X_ERROR,
		    "Unusable physical mapping type 0x%08x\n",
		    (unsigned int)(gttentry & PTE_MAPPING_TYPE_MASK));
		return -1;
	}
	assert((gttentry & PTE_VALID) != 0);

	return (gttentry & PTE_ADDRESS_MASK) |
	    ((uint64_t)(gttentry & PTE_ADDRESS_MASK_HIGH) << (32 - 4));
}

/**
 * Reads the GTT entries for stolen memory at the given offset, returning the
 * physical address.
 *
 * \return physical address if successful.
 * \return (uint64_t)-1 if unsuccessful.
 */
static uint64_t i830_get_stolen_physical(ScrnInfoPtr scrn,
    unsigned long offset, unsigned long size)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);
	uint64_t physical;
	unsigned long scan;

	/* Check that the requested region is within stolen memory. */
	if (offset + size >= intel->stolen_size)
		return -1;

	physical = i830_get_gtt_physical(scrn, offset);
	if (physical == -1)
		return -1;

	/* Check that the following pages in our allocation follow the first
	 * page contiguously.
	 */
	for (scan = offset + 4096; scan < offset + size; scan += 4096) {
		uint64_t scan_physical = i830_get_gtt_physical(scrn, scan);

		if ((scan - offset) != (scan_physical - physical)) {
			xf86DrvMsg(scrn->scrnIndex, X_ERROR,
			    "Non-contiguous GTT entries: (%ld,0x16%"
			    PRIx64 ") vs  (%ld,0x%" PRIx64 ")\n",
			    scan, scan_physical, offset, physical);
			return -1;
		}
	}

    return physical;
}

/* Allocate aperture space for the given size and alignment, and returns the
 * memory allocation.
 *
 * Allocations are a minimum of a page, and will be at least page-aligned.
 */
static i830_memory *
i830_allocate_aperture(ScrnInfoPtr scrn, const char *name, unsigned long size,
    unsigned long pitch, unsigned long alignment, int flags,
    enum tile_format tile_format)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);
	i830_memory *mem, *scan;

	mem = xcalloc(1, sizeof(*mem));
	if (mem == NULL)
		return NULL;

	/* No memory allocated to back the region */
	mem->key = -1;

	mem->name = xstrdup(name);
	if (mem->name == NULL) {
		xfree(mem);
		return NULL;
	}
	/* Only allocate page-sized increments. */
	size = ALIGN(size, GTT_PAGE_SIZE);
	mem->size = size;
	mem->allocated_size = size;
	mem->alignment = alignment;
	mem->tiling = tile_format;
	mem->pitch = pitch;
	mem->fence_nr = -1;

	if (alignment < GTT_PAGE_SIZE)
		alignment = GTT_PAGE_SIZE;

	for (scan = intel->memory_list; scan->next != NULL; scan = scan->next) {
		mem->offset = ROUND_TO(scan->end, alignment);
		if ((flags & NEED_PHYSICAL_ADDR) &&
		    mem->offset < intel->stolen_size) {
			/* If the allocation is entirely within stolen memory,
			 * and we're able to get the physical addresses out of
			 * the GTT and check that it's contiguous (it ought to
			 * be), then we can do our physical allocations there
			 * and not bother the kernel about it.  This helps
			 * avoid aperture fragmentation from our
			 * physical allocations.
			 */
			mem->bus_addr = i830_get_stolen_physical(scrn,
			    mem->offset, mem->size);

			if (mem->bus_addr == ((uint64_t)-1)) {
				/* Move the start of the allocation to just
				 * past the end of stolen memory.
				 */
				mem->offset = ROUND_TO(intel->stolen_size,
				    alignment);
			}
		}
		if ((flags & NEED_NON_STOLEN) &&
		    mem->offset < intel->stolen_size) {
			mem->offset = ROUND_TO(intel->stolen_size, alignment);
		}

		mem->end = mem->offset + size;
		if (flags & ALIGN_BOTH_ENDS)
		    mem->end = ROUND_TO(mem->end, alignment);
		if (mem->end <= scan->next->offset)
		    break;
	    }
	if (scan->next == NULL) {
		/* Reached the end of the list, and didn't find space */
		xfree(mem->name);
		xfree(mem);
		return NULL;
	}
	/* Insert new allocation into the list */
	mem->prev = scan;
	mem->next = scan->next;
	scan->next = mem;
	mem->next->prev = mem;

	return mem;
}

/**
 * Allocates the AGP memory necessary for the part of a memory allocation not
 * already covered by the stolen memory.
 *
 * The memory is automatically bound if we have the VT.
 */
static Bool
i830_allocate_agp_memory(ScrnInfoPtr scrn, i830_memory *mem, int flags)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);
	unsigned long size;

	if (mem->key != -1)
		return TRUE;

	if (mem->offset + mem->size <= intel->stolen_size)
		return TRUE;

	if (mem->offset < intel->stolen_size)
		mem->agp_offset = intel->stolen_size;
	else
		mem->agp_offset = mem->offset;

	size = mem->size - (mem->agp_offset - mem->offset);

	if (flags & NEED_PHYSICAL_ADDR) {
		unsigned long agp_bus_addr;

		mem->key = xf86AllocateGARTMemory(scrn->scrnIndex, size, 2,
		    &agp_bus_addr);
		mem->bus_addr = agp_bus_addr;
	} else {
		mem->key = xf86AllocateGARTMemory(scrn->scrnIndex,
		    size, 0, NULL);
	}
	if (mem->key == -1 || ((flags & NEED_PHYSICAL_ADDR) &&
	    mem->bus_addr == 0)) {
		return FALSE;
	}

	return TRUE;
}

static i830_memory *i830_allocate_memory_bo(ScrnInfoPtr scrn, const char *name,
					    unsigned long size,
					    unsigned long pitch,
					    unsigned long align, int flags,
					    enum tile_format tile_format)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);
	i830_memory *mem;
	uint32_t bo_tiling_mode = I915_TILING_NONE;
	int ret;

	assert((flags & NEED_PHYSICAL_ADDR) == 0);

	/* Only allocate page-sized increments. */
	size = ALIGN(size, GTT_PAGE_SIZE);
	align = i830_get_fence_alignment(intel, size);

	mem = xcalloc(1, sizeof(*mem));
	if (mem == NULL)
		return NULL;

	mem->name = xstrdup(name);
	if (mem->name == NULL) {
		xfree(mem);
		return NULL;
	}

	mem->bo = dri_bo_alloc(intel->bufmgr, name, size, align);

	if (!mem->bo) {
		xfree(mem->name);
		xfree(mem);
		return NULL;
	}

	/* Give buffer obviously wrong offset/end until it's pinned. */
	mem->offset = -1;
	mem->end = -1;
	mem->size = size;
	mem->allocated_size = size;
	mem->alignment = align;
	mem->tiling = tile_format;
	mem->pitch = pitch;
	mem->fence_nr = -1;

	/* Give buffer obviously wrong offset/end until it's pinned. */
	if (flags & NEED_LIFETIME_FIXED)
		mem->lifetime_fixed_offset = TRUE;

	switch (tile_format) {
	case TILE_XMAJOR:
		bo_tiling_mode = I915_TILING_X;
		break;
	case TILE_YMAJOR:
		bo_tiling_mode = I915_TILING_Y;
		break;
	case TILE_NONE:
	default:
		bo_tiling_mode = I915_TILING_NONE;
		break;
	}

	ret = drm_intel_bo_set_tiling(mem->bo, &bo_tiling_mode, pitch);
	if (ret != 0
	    || (bo_tiling_mode == I915_TILING_NONE
		&& tile_format != TILE_NONE)) {
		xf86DrvMsg(scrn->scrnIndex, X_ERROR,
			   "Failed to set tiling on %s: %s\n", mem->name,
			   ret == 0 ? "rejected by kernel" : strerror(-ret));
	}

	/* Bind it if we currently control the VT */
	if (scrn->vtSema || intel->use_drm_mode) {
		if (!i830_bind_memory(scrn, mem)) {
			dri_bo_unreference (mem->bo);
			xfree(mem->name);
			xfree(mem);
			return NULL;
		}
	}


	if (flags & DISABLE_REUSE)
		drm_intel_bo_disable_reuse(mem->bo);

	/* Insert new allocation into the list */
	mem->prev = NULL;
	mem->next = intel->bo_list;
	if (intel->bo_list != NULL)
		intel->bo_list->prev = mem;
	intel->bo_list = mem;

	return mem;
}

/* Allocates video memory at the given size, pitch, alignment and tile format.
 *
 * The memory will be bound automatically when the driver is in control of the
 * VT.  When the kernel memory manager is available and compatible with flags
 * (that is, flags doesn't say that the allocation must include a physical
 * address), that will be used for the allocation.
 *
 * flags:
 * - NEED_PHYSICAL_ADDR: Allocates the memory physically contiguous, and return
 *   the bus address for that memory.
 * - ALIGN_BOTH_ENDS: after choosing the alignment, align the end offset to
 *   @alignment as well.
 * - NEED_NON-STOLEN: don't allow any part of the memory allocation to lie
 *   within stolen memory
 * - NEED_LIFETIME_FIXED: don't allow the buffer object to move throughout
 *   the entire Screen lifetime.  This means not using buffer objects, which
 *   get their offsets chosen at each EnterVT time.
 */
i830_memory *i830_allocate_memory(ScrnInfoPtr scrn, const char *name,
				  unsigned long size, unsigned long pitch,
				  unsigned long alignment, int flags,
				  enum tile_format tile_format)
{
	i830_memory *mem;
	intel_screen_private *intel = intel_get_screen_private(scrn);

	/* Manage tile alignment and size constraints */
	if (tile_format != TILE_NONE) {
		/* Only allocate page-sized increments. */
		size = ALIGN(size, GTT_PAGE_SIZE);

		/* Check for maximum tiled region size */
		if (IS_I9XX(intel)) {
			if (size > MB(128))
				return NULL;
		} else {
			if (size > MB(64))
				return NULL;
		}

		/* round to size necessary for the fence register to work */
		size = i830_get_fence_size(intel, size);
		alignment = i830_get_fence_alignment(intel, size);
	}

	/*
	 * Create a kernel buffer object when suitable.
	 * Under KMS, all graphics memory must be managed by the
	 * kernel. Under UMS, we separately reserve space for
	 *  a few objects (overlays, power context, cursors, etc).
	 */
	if (intel->have_gem && (intel->use_drm_mode ||
	    !(flags & (NEED_PHYSICAL_ADDR|NEED_LIFETIME_FIXED)))) {
		return i830_allocate_memory_bo(scrn, name, size, pitch,
		    alignment, flags, tile_format);
	} else {
		mem = i830_allocate_aperture(scrn, name, size, pitch,
		    alignment, flags, tile_format);
		if (mem == NULL)
			return NULL;

		if (!i830_allocate_agp_memory(scrn, mem, flags)) {
			i830_free_memory(scrn, mem);
			return NULL;
		}

		if (!i830_bind_memory(scrn, mem)) {
			i830_free_memory(scrn, mem);
			return NULL;
		}
	}

	return mem;
}

void
i830_describe_allocations(ScrnInfoPtr scrn, int verbosity, const char *prefix)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);
	i830_memory *mem;

	if (intel->memory_list == NULL) {
		xf86DrvMsgVerb(scrn->scrnIndex, X_INFO, verbosity,
			       "%sMemory allocator not initialized\n", prefix);
		return;
	}

	if (intel->memory_list->next->next == NULL) {
		xf86DrvMsgVerb(scrn->scrnIndex, X_INFO, verbosity,
			       "%sNo memory allocations\n", prefix);
		return;
	}

	xf86DrvMsgVerb(scrn->scrnIndex, X_INFO, verbosity,
		       "%sFixed memory allocation layout:\n", prefix);

	for (mem = intel->memory_list->next; mem->next != NULL; mem = mem->next) {
		char phys_suffix[32] = "";
		char *tile_suffix = "";

		if (mem->offset >= intel->stolen_size &&
		    mem->prev->offset < intel->stolen_size) {
			xf86DrvMsgVerb(scrn->scrnIndex, X_INFO, verbosity,
			    "%s0x%08lx:            end of stolen memory\n",
			    prefix, intel->stolen_size);
		}

		if (mem->bus_addr != 0)
			snprintf(phys_suffix, sizeof(phys_suffix),
			    ", 0x%016" PRIx64 " physical\n", mem->bus_addr);
		if (mem->tiling == TILE_XMAJOR)
			tile_suffix = " X tiled";
		else if (mem->tiling == TILE_YMAJOR)
			tile_suffix = " Y tiled";

		xf86DrvMsgVerb(scrn->scrnIndex, X_INFO, verbosity,
			       "%s0x%08lx-0x%08lx: %s (%ld kB%s)%s\n", prefix,
			       mem->offset, mem->end - 1, mem->name,
			       mem->size / 1024, phys_suffix, tile_suffix);
	}
	xf86DrvMsgVerb(scrn->scrnIndex, X_INFO, verbosity,
	    "%s0x%08lx:            end of aperture\n", prefix, intel->FbMapSize);

	xf86DrvMsgVerb(scrn->scrnIndex, X_INFO, verbosity,
	    "%sBO memory allocation layout:\n", prefix);
	if (intel->memory_manager) {
		xf86DrvMsgVerb(scrn->scrnIndex, X_INFO, verbosity,
		    "%s0x%08lx:            start of memory manager\n",
		    prefix, intel->memory_manager->offset);
	}
	for (mem = intel->bo_list; mem != NULL; mem = mem->next) {
		char *tile_suffix = "";

		if (mem->tiling == TILE_XMAJOR)
			tile_suffix = " X tiled";
		else if (mem->tiling == TILE_YMAJOR)
			tile_suffix = " Y tiled";

		if (mem->bound) {
			xf86DrvMsgVerb(scrn->scrnIndex, X_INFO, verbosity,
			   "%s0x%08lx-0x%08lx: %s (%ld kB)%s\n", prefix,
			   mem->offset, mem->end - 1, mem->name,
			   mem->size / 1024, tile_suffix);
		} else {
			xf86DrvMsgVerb(scrn->scrnIndex, X_INFO, verbosity,
			    "%sunpinned          : %s (%ld kB)%s\n", prefix,
			    mem->name, mem->size / 1024, tile_suffix);
		}
	}
	if (intel->memory_manager) {
		xf86DrvMsgVerb(scrn->scrnIndex, X_INFO, verbosity,
		    "%s0x%08lx:            end of memory manager\n",
		    prefix, intel->memory_manager->end);
	}
}

static Bool i830_allocate_ringbuffer(ScrnInfoPtr scrn)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);

	if (intel->have_gem || intel->ring.mem != NULL)
		return TRUE;

	/* We don't have any mechanism in the DRM yet to alert it that we've
	 * moved the ringbuffer since init time, so allocate it fixed for its
	 * lifetime.
	 */
	intel->ring.mem = i830_allocate_memory(scrn, "ring buffer",
	    PRIMARY_RINGBUFFER_SIZE, PITCH_NONE, GTT_PAGE_SIZE,
	    NEED_LIFETIME_FIXED, TILE_NONE);
	if (intel->ring.mem == NULL) {
		xf86DrvMsg(scrn->scrnIndex, X_ERROR,
		    "Failed to allocate Ring Buffer space\n");
		return FALSE;
	}

	intel->ring.tail_mask = intel->ring.mem->size - 1;
	intel->ring.virtual_start = intel->FbBase + intel->ring.mem->offset;
	return TRUE;
}

/**
 * Allocate space for overlay registers.
 */
static Bool i830_allocate_overlay(ScrnInfoPtr scrn)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);
	int flags = 0;

	/* Only allocate if overlay is going to be enabled. */
	if (!intel->XvEnabled)
		return TRUE;

	if (OVERLAY_NOEXIST(intel))
		return TRUE;

	if (!OVERLAY_NOPHYSICAL(intel)) {
		if (intel->use_drm_mode)
			return TRUE;
		flags |= NEED_PHYSICAL_ADDR;
	}

	intel->overlay_regs = i830_allocate_memory(scrn, "overlay registers",
	    OVERLAY_SIZE, PITCH_NONE, GTT_PAGE_SIZE, flags, TILE_NONE);
	if (intel->overlay_regs == NULL) {
		xf86DrvMsg(scrn->scrnIndex, X_WARNING,
		    "Failed to allocate Overlay register space.\n");
		/* This failure isn't fatal. */
	}

	if (flags & NEED_PHYSICAL_ADDR)
		if (intel->use_drm_mode)
			; /* need physical addr */

	return TRUE;
}

static Bool IsTileable(ScrnInfoPtr scrn, int pitch)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);

	if (IS_I965G(intel)) {
		if (pitch / 512 * 512 == pitch && pitch <= KB(128))
			return TRUE;
		else
			return FALSE;
	}

	/*
	 * Allow tiling for pitches that are a power of 2 multiple of 128 bytes,
	 * up to 64 * 128 (= 8192) bytes.
	 */
	switch (pitch) {
	case 128:
	case 256:
		if (IS_I945G(intel) || IS_I945GM(intel) || IS_G33CLASS(intel))
			return TRUE;
		else
			return FALSE;
	case 512:
	case KB(1):
	case KB(2):
	case KB(4):
	case KB(8):
		return TRUE;
	default:
		return FALSE;
	}
}

/**
 * Allocates a framebuffer for a screen.
 *
 * Used once for each X screen, so once with RandR 1.2 and twice with classic
 * dualhead.
 */
i830_memory *i830_allocate_framebuffer(ScrnInfoPtr scrn)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);
	unsigned int pitch = scrn->displayWidth * intel->cpp;
	unsigned long minspace;
	int align;
	long size, fb_height;
	int flags;
	i830_memory *front_buffer = NULL;
	enum tile_format tile_format = TILE_NONE;

	flags = ALLOW_SHARING | DISABLE_REUSE;

	/* We'll allocate the fb such that the root window will fit regardless of
	 * rotation.
	 */
	fb_height = scrn->virtualY;

	/* Calculate how much framebuffer memory to allocate.  For the
	 * initial allocation, calculate a reasonable minimum.  This is
	 * enough for the virtual screen size.
	 */
	minspace = pitch * scrn->virtualY;

	size = ROUND_TO_PAGE(pitch * fb_height);

	if (intel->tiling)
		tile_format = TILE_XMAJOR;

	if (!IsTileable(scrn, pitch))
		tile_format = TILE_NONE;

	if (!i830_check_display_stride(scrn, pitch, tile_format != TILE_NONE)) {
		xf86DrvMsg(scrn->scrnIndex, X_ERROR,
			   "Front buffer stride %d kB "
			   "exceed display limit\n", pitch / 1024);
		return NULL;
	}

	/* Attempt to allocate it tiled first if we have page flipping on. */
	if (tile_format != TILE_NONE) {
		/* XXX: probably not the case on 965 */
		if (IS_I9XX(intel))
			align = MB(1);
		else
			align = KB(512);
	} else
		align = KB(64);
	front_buffer = i830_allocate_memory(scrn, "front buffer", size,
					    pitch, align, flags, tile_format);

	if (front_buffer == NULL) {
		xf86DrvMsg(scrn->scrnIndex, X_ERROR,
			   "Failed to allocate framebuffer.\n");
		return NULL;
	}

	if (intel->FbBase && front_buffer->bound)
		memset (intel->FbBase + front_buffer->offset, 0, size);
	i830_set_gem_max_sizes(scrn);

	return front_buffer;
}

static Bool i830_allocate_cursor_buffers(ScrnInfoPtr scrn)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);
	xf86CrtcConfigPtr xf86_config = XF86_CRTC_CONFIG_PTR(scrn);
	int flags, i;
	long size;

	if (intel->use_drm_mode)
		intel->CursorNeedsPhysical = FALSE;

	flags = intel->CursorNeedsPhysical ? NEED_PHYSICAL_ADDR : 0;

	flags |= DISABLE_REUSE;


	if (!intel->use_drm_mode) {
		/* Try to allocate one big blob for our cursor memory.
		 * This works around a limitation in the FreeBSD AGP driver
		 * that allows only one physical allocation larger than a page,
		 * and could allow us to pack the cursors smaller.
		 */
		size = xf86_config->num_crtc * (HWCURSOR_SIZE +
		    HWCURSOR_SIZE_ARGB);

		intel->cursor_mem = i830_allocate_memory(scrn, "HW cursors",
		    size, PITCH_NONE, GTT_PAGE_SIZE, flags, TILE_NONE);
		if (intel->cursor_mem != NULL)
			return TRUE;
	}

	/*
	 * Allocate four separate buffers when the kernel doesn't support
	 * large allocations as on Linux. If any of these fail, just
	 * bail back to software cursors everywhere
	 */
	for (i = 0; i < xf86_config->num_crtc; i++) {
		if (!intel->use_drm_mode) {
			intel->cursor_mem_classic[i] =
			    i830_allocate_memory(scrn, "Core cursor",
			        HWCURSOR_SIZE, PITCH_NONE, GTT_PAGE_SIZE,
			        flags, TILE_NONE);
		if (!intel->cursor_mem_classic[i])
			return FALSE;
		}
		intel->cursor_mem_argb[i] =
		    i830_allocate_memory(scrn, "ARGB cursor",
					 HWCURSOR_SIZE_ARGB, PITCH_NONE,
					 GTT_PAGE_SIZE, flags, TILE_NONE);
		if (!intel->cursor_mem_argb[i])
			return FALSE;

	}
	return TRUE;
}

static void i830_setup_fb_compression(ScrnInfoPtr scrn)
{
    intel_screen_private *intel = intel_get_screen_private(scrn);
    unsigned long compressed_size;
    unsigned long fb_height;

    if (scrn->virtualX > scrn->virtualY)
	fb_height = scrn->virtualX;
    else
	fb_height = scrn->virtualY;

    /* Only mobile chips since 845 support this feature */
    if (!IS_MOBILE(intel)) {
	intel->fb_compression = FALSE;
	goto out;
    }

    if (IS_GM45(intel)) {
	/* Update i830_display.c too if compression ratio changes */
	compressed_size = fb_height * (scrn->displayWidth / 4);
    } else {
	compressed_size = MB(6);
    }

    /*
     * Compressed framebuffer limitations:
     *   - contiguous, physical, uncached memory
     *   - ideally as large as the front buffer(s), smaller sizes cache less
     *   - uncompressed buffer must be tiled w/pitch 2k-16k
     *   - uncompressed fb is <= 2048 in width, 0 mod 8
     *   - uncompressed fb is <= 1536 in height, 0 mod 2
     *   - compressed fb stride is <= uncompressed stride
     *   - SR display watermarks must be equal between 16bpp and 32bpp?
     *   - both compressed and line buffers must be in stolen memory
     */
    intel->compressed_front_buffer =
	i830_allocate_memory(scrn, "compressed frame buffer",
			     compressed_size, PITCH_NONE,
			     KB(4), NEED_PHYSICAL_ADDR,
			     TILE_NONE);

    if (!intel->compressed_front_buffer) {
	intel->fb_compression = FALSE;
	goto out;
    }

    if (!IS_GM45(intel)) {
	intel->compressed_ll_buffer =
	    i830_allocate_memory(scrn, "compressed ll buffer",
				 FBC_LL_SIZE + FBC_LL_PAD,
				 PITCH_NONE, KB(4),
				 NEED_PHYSICAL_ADDR,
				 TILE_NONE);
	if (!intel->compressed_ll_buffer) {
	    i830_free_memory(scrn, intel->compressed_front_buffer);
	    intel->fb_compression = FALSE;
	    goto out;
	}
    }

out:
    if (!intel->fb_compression)
	xf86DrvMsg(scrn->scrnIndex, X_WARNING, "Allocation error, framebuffer"
		   " compression disabled\n");
	
    return;
}
/*
 * Allocate memory for 2D operation.  This includes the (front) framebuffer,
 * ring buffer, scratch memory, HW cursor.
 */
Bool i830_allocate_2d_memory(ScrnInfoPtr scrn)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);

	if (!intel->use_drm_mode) {
		if (!xf86AgpGARTSupported() ||
		    !xf86AcquireGART(scrn->scrnIndex)) {
			xf86DrvMsg(scrn->scrnIndex, X_ERROR,
			    "AGP GART support is either not available or "
			    "cannot be used.\n"
			    "\tMake sure your kernel has agpgart support or has\n"
			    "\tthe agpgart module loaded.\n");
			return FALSE;
		}

		/* Allocate the ring buffer first, so it ends up in stolen mem. */
		if (!i830_allocate_ringbuffer(scrn))
			return FALSE;
	}

	if (intel->fb_compression)
		i830_setup_fb_compression(scrn);

	/* Next, allocate other fixed-size allocations we have. */
	if (!i830_allocate_cursor_buffers(scrn)) {
		xf86DrvMsg(scrn->scrnIndex, X_ERROR,
			   "Failed to allocate HW cursor space.\n");
		return FALSE;
	}

	if (!intel->have_gem) {
		intel->fake_bufmgr_mem = i830_allocate_memory(scrn,
		    "fake bufmgr", MB(8), PITCH_NONE, GTT_PAGE_SIZE, 0,
		    TILE_NONE);
		if (intel->fake_bufmgr_mem == NULL) {
			xf86DrvMsg(scrn->scrnIndex, X_WARNING,
			    "Failed to allocate fake bufmgr space.\n");
			return FALSE;
		}
		i830_init_bufmgr(scrn);
	    }

	if (!intel->use_drm_mode)
		i830_allocate_overlay(scrn);

	intel->front_buffer = i830_allocate_framebuffer(scrn);
	if (intel->front_buffer == NULL)
		return FALSE;

	return TRUE;
}

Bool
i830_allocate_pwrctx(ScrnInfoPtr scrn)
{
    intel_screen_private *intel = intel_get_screen_private(scrn);

    if (intel->use_drm_mode)
	return TRUE;

    intel->power_context = i830_allocate_memory(scrn, "power context",
						PWRCTX_SIZE, PITCH_NONE,
						GTT_PAGE_SIZE,
						NEED_LIFETIME_FIXED,
						TILE_NONE);
    if (!intel->power_context) {
	xf86DrvMsg(scrn->scrnIndex, X_WARNING,
		"Failed to allocate power context.\n");
	return FALSE;
    }
    return TRUE;
}

/**
 * Sets up tiled surface registers ("fences") for the hardware.
 *
 * The fences control automatic tiled address swizzling for CPU access of the
 * framebuffer, and may be used in many rendering operations instead of
 * manually supplying tiling enables per surface.
 */
static int
i830_set_tiling(ScrnInfoPtr scrn, unsigned int offset,
		unsigned int pitch, unsigned int size,
		enum tile_format tile_format)
{
    intel_screen_private *intel = intel_get_screen_private(scrn);
    uint32_t val;
    uint32_t fence_mask = 0;
    unsigned int fence_pitch;
    unsigned int max_fence;
    unsigned int fence_nr;

    DPRINTF(PFX, "i830_set_tiling(): 0x%08x, %d, %d kByte\n",
	    offset, pitch, size / 1024);

    assert(tile_format != TILE_NONE);

    intel_sync(scrn);

    if (IS_I965G(intel))
	max_fence = FENCE_NEW_NR;
    else
	max_fence = FENCE_NR;

    for (fence_nr = 0; fence_nr < max_fence; fence_nr++) {
	if (!intel->fence_used[fence_nr])
	    break;
    }
    if (fence_nr == max_fence)
	FatalError("Ran out of fence registers at %d\n", fence_nr);

    intel->fence_used[fence_nr] = TRUE;

    if (IS_I965G(intel)) {
	uint32_t fence_start, fence_end;

	switch (tile_format) {
	case TILE_XMAJOR:
	    fence_start = (((pitch / 128) - 1) << 2) | offset | 1;
	    fence_start |= I965_FENCE_X_MAJOR;
            break;
	case TILE_YMAJOR:
            /* YMajor can be 128B aligned but the current code dictates
             * otherwise. This isn't a problem apart from memory waste.
             * FIXME */
	    fence_start = (((pitch / 128) - 1) << 2) | offset | 1;
	    fence_start |= I965_FENCE_Y_MAJOR;
            break;
	default:
	    return -1;
	}

	/* The end marker is the address of the last page in the allocation. */
	fence_end = offset + size - 4096;

	OUTREG(FENCE_NEW + fence_nr * 8, fence_start);
	OUTREG(FENCE_NEW + fence_nr * 8 + 4, fence_end);
    } else {
	if (IS_I9XX(intel))
	    fence_mask = ~I915G_FENCE_START_MASK;
	else
	    fence_mask = ~I830_FENCE_START_MASK;

	if (offset & fence_mask) {
	    FatalError("i830_set_tiling(): %d: offset (0x%08x) is not %s "
		       "aligned\n",
		       fence_nr, offset, (IS_I9XX(intel)) ? "1MB" : "512k");
	}

	if (offset % size) {
	    FatalError("i830_set_tiling(): %d: offset (0x%08x) is not "
		       "size (%dk) aligned\n",
		       fence_nr, offset, size / 1024);
	}

	if (pitch & 127) {
	    FatalError("i830_set_tiling(): %d: pitch (%d) not a multiple of "
		       "128 bytes\n",
		       fence_nr, pitch);
	}

	val = offset | FENCE_VALID;

	switch (tile_format) {
	case TILE_XMAJOR:
	    val |= FENCE_X_MAJOR;
	    break;
	case TILE_YMAJOR:
	    val |= FENCE_Y_MAJOR;
	    break;
	case TILE_NONE:
	    break;
	}

	if (IS_I9XX(intel)) {
	    switch (size) {
	    case MB(1):
		val |= I915G_FENCE_SIZE_1M;
		break;
	    case MB(2):
		val |= I915G_FENCE_SIZE_2M;
		break;
	    case MB(4):
		val |= I915G_FENCE_SIZE_4M;
		break;
	    case MB(8):
		val |= I915G_FENCE_SIZE_8M;
		break;
	    case MB(16):
		val |= I915G_FENCE_SIZE_16M;
		break;
	    case MB(32):
		val |= I915G_FENCE_SIZE_32M;
		break;
	    case MB(64):
		val |= I915G_FENCE_SIZE_64M;
		break;
	    default:
		FatalError("i830_set_tiling(): %d: illegal size (%d kByte)\n",
			   fence_nr, size / 1024);
	    }
	} else {
	    switch (size) {
	    case KB(512):
		val |= FENCE_SIZE_512K;
		break;
	    case MB(1):
		val |= FENCE_SIZE_1M;
		break;
	    case MB(2):
		val |= FENCE_SIZE_2M;
		break;
	    case MB(4):
		val |= FENCE_SIZE_4M;
		break;
	    case MB(8):
		val |= FENCE_SIZE_8M;
		break;
	    case MB(16):
		val |= FENCE_SIZE_16M;
		break;
	    case MB(32):
		val |= FENCE_SIZE_32M;
		break;
	    case MB(64):
		val |= FENCE_SIZE_64M;
		break;
	    default:
		FatalError("i830_set_tiling(): %d: illegal size (%d kByte)\n",
			   fence_nr, size / 1024);
	    }
	}

	if ((IS_I945G(intel) || IS_I945GM(intel) || IS_G33CLASS(intel)) &&
	    tile_format == TILE_YMAJOR)
	    fence_pitch = pitch / 128;
	else if (IS_I9XX(intel))
	    fence_pitch = pitch / 512;
	else
	    fence_pitch = pitch / 128;

	switch (fence_pitch) {
	case 1:
	    val |= FENCE_PITCH_1;
	    break;
	case 2:
	    val |= FENCE_PITCH_2;
	    break;
	case 4:
	    val |= FENCE_PITCH_4;
	    break;
	case 8:
	    val |= FENCE_PITCH_8;
	    break;
	case 16:
	    val |= FENCE_PITCH_16;
	    break;
	case 32:
	    val |= FENCE_PITCH_32;
	    break;
	case 64:
	    val |= FENCE_PITCH_64;
	    break;
	default:
	    FatalError("i830_set_tiling(): %d: illegal pitch (%d)\n",
		       fence_nr, pitch);
	}

	OUTREG(FENCE + fence_nr * 4, val);
    }

    return fence_nr;
}

static void
i830_clear_tiling(ScrnInfoPtr scrn, unsigned int fence_nr)
{
    intel_screen_private *intel = intel_get_screen_private(scrn);

    if (IS_I965G(intel)) {
	OUTREG(FENCE_NEW + fence_nr * 8, 0);
	OUTREG(FENCE_NEW + fence_nr * 8 + 4, 0);
    } else {
	OUTREG(FENCE + fence_nr * 4, 0);
    }
    intel->fence_used[fence_nr] = FALSE;
}

/**
 * Called at EnterVT to grab the AGP GART and bind our allocations.
 *
 * In zaphod mode, this will walk the list trying to bind twice, since each
 * intel points to the same allocation list, but the bind_memory will just
 * no-op then.
 */
Bool i830_bind_all_memory(ScrnInfoPtr scrn)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);

	if (intel->memory_list == NULL)
		return TRUE;

	if (intel->use_drm_mode || (xf86AgpGARTSupported() &&
	    !intel->gtt_acquired)) {
		i830_memory *mem;

		if (!intel->use_drm_mode) {
			if (!xf86AcquireGART(scrn->scrnIndex))
				return FALSE;
			intel->gtt_acquired = TRUE;
		}

		for (mem = intel->memory_list->next; mem->next != NULL;
		    mem = mem->next) {
			if (!mem->bound && !i830_bind_memory(scrn, mem)) {
				/* This shouldn't happen */
				FatalError("Couldn't bind memory for %s\n",
				    mem->name);
			}
		}
		for (mem = intel->bo_list; mem != NULL; mem = mem->next) {
			if (mem->bound)
				continue;
			if (!mem->lifetime_fixed_offset &&
			    !i830_bind_memory(scrn, mem))
			FatalError("Couldn't bind memory for BO %s\n",
			    mem->name);
		}
	}
	if (intel->use_drm_mode) {
		int	i;
		xf86CrtcConfigPtr xf86_config = XF86_CRTC_CONFIG_PTR(scrn);
		for (i = 0; i < xf86_config->num_crtc; i++)
			drmmode_crtc_set_cursor_bo(xf86_config->crtc[i],
			    intel->cursor_mem_argb[i]->bo);
	} else
		i830_update_cursor_offsets(scrn);
	i830_set_gem_max_sizes(scrn);

	if (intel->front_buffer)
		scrn->fbOffset = intel->front_buffer->offset;

	return TRUE;
}

/** Called at LeaveVT, to unbind all of our AGP allocations. */
Bool
i830_unbind_all_memory(ScrnInfoPtr scrn)
{
    intel_screen_private *intel = intel_get_screen_private(scrn);

    if (intel->use_drm_mode || (xf86AgpGARTSupported() &&
				intel->gtt_acquired)) {
	i830_memory *mem;

	for (mem = intel->memory_list->next; mem->next != NULL;
	     mem = mem->next)
	{
	    i830_unbind_memory(scrn, mem);
	}
	for (mem = intel->bo_list; mem != NULL; mem = mem->next) {
	    /* Don't unpin objects which require that their offsets never
	     * change.
	     */
	    if (!mem->lifetime_fixed_offset)
		i830_unbind_memory(scrn, mem);
	}

	if (!intel->use_drm_mode) {
	    intel->gtt_acquired = FALSE;

	    if (!xf86ReleaseGART(scrn->scrnIndex))
		return FALSE;
	}
    }

    return TRUE;
}

/**
 * Returns the amount of system memory that could potentially be allocated
 * from AGP, in kB.
 */
long
I830CheckAvailableMemory(ScrnInfoPtr scrn)
{
    AgpInfoPtr agpinf;
    int maxPages;

    if (!xf86AgpGARTSupported() ||
	!xf86AcquireGART(scrn->scrnIndex) ||
	(agpinf = xf86GetAGPInfo(scrn->scrnIndex)) == NULL ||
	!xf86ReleaseGART(scrn->scrnIndex))
	return -1;

    maxPages = agpinf->totalPages - agpinf->usedPages;
    xf86DrvMsgVerb(scrn->scrnIndex, X_INFO, 2, "%s: %d kB available\n",
		   "I830CheckAvailableMemory", maxPages * 4);

    return maxPages * 4;
}

#ifdef INTEL_XVMC
/*
 * Allocate memory for MC compensation
 */
Bool i830_allocate_xvmc_buffer(ScrnInfoPtr scrn, const char *name,
			       i830_memory ** buffer, unsigned long size,
			       int flags)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);

	*buffer = i830_allocate_memory(scrn, name, size, PITCH_NONE,
				       GTT_PAGE_SIZE, flags, TILE_NONE);

	if (!*buffer) {
		xf86DrvMsg(scrn->scrnIndex, X_ERROR,
			   "Failed to allocate memory for %s.\n", name);
		return FALSE;
	}

	if (intel->use_drm_mode && (*buffer)->bo) {
		if (drm_intel_bo_pin((*buffer)->bo, GTT_PAGE_SIZE)) {
			i830_free_memory(scrn, *buffer);
			xf86DrvMsg(scrn->scrnIndex, X_ERROR,
				   "Failed to bind XvMC buffer bo!\n");
			return FALSE;
		}

		(*buffer)->offset = (*buffer)->bo->offset;
	}

	return TRUE;
}

void i830_free_xvmc_buffer(ScrnInfoPtr scrn, i830_memory * buffer)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);

	if (intel->use_drm_mode && buffer->bo)
		drm_intel_bo_unpin(buffer->bo);

	i830_free_memory(scrn, buffer);
}

#endif

static void i830_set_max_gtt_map_size(ScrnInfoPtr scrn)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);
	struct drm_i915_gem_get_aperture aperture;
	int ret;

	/* Default low value in case it gets used during server init. */
	intel->max_gtt_map_size = 16 * 1024 * 1024;

	if (!intel->have_gem)
		return;

	ret =
	    ioctl(intel->drmSubFD, DRM_IOCTL_I915_GEM_GET_APERTURE, &aperture);
	if (ret == 0) {
		/* Let objects up get bound up to the size where only 2 would
		 * fit in the aperture, but then leave slop to account for
		 * alignment like libdrm does.
		 */
		intel->max_gtt_map_size =
		    aperture.aper_available_size * 3 / 4 / 2;
	}
}

static void i830_set_max_tiling_size(ScrnInfoPtr scrn)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);
	struct drm_i915_gem_get_aperture aperture;
	int ret;

	/* Default low value in case it gets used during server init. */
	intel->max_tiling_size = 4 * 1024 * 1024;

	ret =
	    ioctl(intel->drmSubFD, DRM_IOCTL_I915_GEM_GET_APERTURE, &aperture);
	if (ret == 0) {
		/* Let objects be tiled up to the size where only 4 would fit in
		 * the aperture, presuming worst case alignment.
		 */
		intel->max_tiling_size = aperture.aper_available_size / 4;
		if (!IS_I965G(intel))
			intel->max_tiling_size /= 2;
	}
}

void i830_set_gem_max_sizes(ScrnInfoPtr scrn)
{
	i830_set_max_gtt_map_size(scrn);
	i830_set_max_tiling_size(scrn);
}
