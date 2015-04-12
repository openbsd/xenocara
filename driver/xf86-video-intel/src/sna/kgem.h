/*
 * Copyright (c) 2011 Intel Corporation
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
 *    Chris Wilson <chris@chris-wilson.co.uk>
 *
 */

#ifndef KGEM_H
#define KGEM_H

#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>

#include <i915_drm.h>

#include "compiler.h"

#if HAS_DEBUG_FULL
void LogF(const char *f, ...);
#define DBG(x) LogF x
#define ERR(x) ErrorF x
#else
#define DBG(x)
#define ERR(x)
#endif

struct kgem_bo {
	struct kgem_request *rq;
#define RQ(rq) ((struct kgem_request *)((uintptr_t)(rq) & ~3))
#define RQ_RING(rq) ((uintptr_t)(rq) & 3)
#define RQ_IS_BLT(rq) (RQ_RING(rq) == KGEM_BLT)
#define MAKE_REQUEST(rq, ring) ((struct kgem_request *)((uintptr_t)(rq) | (ring)))

	struct drm_i915_gem_exec_object2 *exec;

	struct kgem_bo *proxy;

	struct list list;
	struct list request;
	struct list vma;

	void *map__cpu;
	void *map__gtt;
#define MAP(ptr) ((void*)((uintptr_t)(ptr) & ~3))

	struct kgem_bo_binding {
		struct kgem_bo_binding *next;
		uint32_t format;
		uint16_t offset;
	} binding;

	uint64_t presumed_offset;
	uint32_t unique_id;
	uint32_t refcnt;
	uint32_t handle;
	uint32_t target_handle;
	uint32_t delta;
	uint32_t active_scanout;
	union {
		struct {
			uint32_t count:27;
#define PAGE_SIZE 4096
			uint32_t bucket:5;
#define NUM_CACHE_BUCKETS 16
#define MAX_CACHE_SIZE (1 << (NUM_CACHE_BUCKETS+12))
		} pages;
		uint32_t bytes;
	} size;
	uint32_t pitch : 18; /* max 128k */
	uint32_t tiling : 2;
	uint32_t reusable : 1;
	uint32_t gpu_dirty : 1;
	uint32_t gtt_dirty : 1;
	uint32_t domain : 2;
	uint32_t needs_flush : 1;
	uint32_t snoop : 1;
	uint32_t io : 1;
	uint32_t flush : 1;
	uint32_t scanout : 1;
	uint32_t prime : 1;
	uint32_t purged : 1;
};
#define DOMAIN_NONE 0
#define DOMAIN_CPU 1
#define DOMAIN_GTT 2
#define DOMAIN_GPU 3

struct kgem_request {
	struct list list;
	struct kgem_bo *bo;
	struct list buffers;
	int ring;
};

enum {
	MAP_GTT = 0,
	MAP_CPU,
	NUM_MAP_TYPES,
};

struct kgem {
	int fd;
	unsigned wedged;
	unsigned gen;

	uint32_t unique_id;

	enum kgem_mode {
		/* order matches I915_EXEC_RING ordering */
		KGEM_NONE = 0,
		KGEM_RENDER,
		KGEM_BSD,
		KGEM_BLT,
	} mode, ring;

	struct list flushing;
	struct list large;
	struct list large_inactive;
	struct list active[NUM_CACHE_BUCKETS][3];
	struct list inactive[NUM_CACHE_BUCKETS];
	struct list pinned_batches[2];
	struct list snoop;
	struct list scanout;
	struct list batch_buffers, active_buffers;

	struct list requests[2];
	struct kgem_request *next_request;
	struct kgem_request static_request;

	struct {
		struct list inactive[NUM_CACHE_BUCKETS];
		int16_t count;
	} vma[NUM_MAP_TYPES];

	uint32_t batch_flags;
	uint32_t batch_flags_base;
#define I915_EXEC_SECURE (1<<9)
#define LOCAL_EXEC_OBJECT_WRITE (1<<2)

	uint16_t nbatch;
	uint16_t surface;
	uint16_t nexec;
	uint16_t nreloc;
	uint16_t nreloc__self;
	uint16_t nfence;
	uint16_t batch_size;

	uint32_t flush:1;
	uint32_t need_expire:1;
	uint32_t need_purge:1;
	uint32_t need_retire:1;
	uint32_t need_throttle:1;
	uint32_t needs_semaphore:1;
	uint32_t needs_reservation:1;
	uint32_t scanout_busy:1;
	uint32_t busy:1;

	uint32_t has_create2 :1;
	uint32_t has_userptr :1;
	uint32_t has_blt :1;
	uint32_t has_relaxed_fencing :1;
	uint32_t has_relaxed_delta :1;
	uint32_t has_semaphores :1;
	uint32_t has_secure_batches :1;
	uint32_t has_pinned_batches :1;
	uint32_t has_caching :1;
	uint32_t has_llc :1;
	uint32_t has_wt :1;
	uint32_t has_no_reloc :1;
	uint32_t has_handle_lut :1;

	uint32_t can_blt_cpu :1;
	uint32_t can_render_y :1;

	uint16_t fence_max;
	uint16_t half_cpu_cache_pages;
	uint32_t aperture_total, aperture_high, aperture_low, aperture_mappable, aperture_fenceable;
	uint32_t aperture, aperture_fenced, aperture_max_fence;
	uint32_t max_upload_tile_size, max_copy_tile_size;
	uint32_t max_gpu_size, max_cpu_size;
	uint32_t large_object_size, max_object_size;
	uint32_t buffer_size;

	void (*context_switch)(struct kgem *kgem, int new_mode);
	void (*retire)(struct kgem *kgem);
	void (*expire)(struct kgem *kgem);

	void (*memcpy_to_tiled_x)(const void *src, void *dst, int bpp,
				  int32_t src_stride, int32_t dst_stride,
				  int16_t src_x, int16_t src_y,
				  int16_t dst_x, int16_t dst_y,
				  uint16_t width, uint16_t height);
	void (*memcpy_from_tiled_x)(const void *src, void *dst, int bpp,
				    int32_t src_stride, int32_t dst_stride,
				    int16_t src_x, int16_t src_y,
				    int16_t dst_x, int16_t dst_y,
				    uint16_t width, uint16_t height);

	uint16_t reloc__self[256];
	uint32_t batch[64*1024-8] page_aligned;
	struct drm_i915_gem_exec_object2 exec[384] page_aligned;
	struct drm_i915_gem_relocation_entry reloc[8192] page_aligned;

#ifdef DEBUG_MEMORY
	struct {
		int bo_allocs;
		size_t bo_bytes;
	} debug_memory;
#endif
};

#define KGEM_MAX_DEFERRED_VBO 16

#define KGEM_BATCH_RESERVED 1
#define KGEM_RELOC_RESERVED (KGEM_MAX_DEFERRED_VBO)
#define KGEM_EXEC_RESERVED (1+KGEM_MAX_DEFERRED_VBO)

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(a) (sizeof(a)/sizeof((a)[0]))
#endif

#define KGEM_BATCH_SIZE(K) ((K)->batch_size-KGEM_BATCH_RESERVED)
#define KGEM_EXEC_SIZE(K) (int)(ARRAY_SIZE((K)->exec)-KGEM_EXEC_RESERVED)
#define KGEM_RELOC_SIZE(K) (int)(ARRAY_SIZE((K)->reloc)-KGEM_RELOC_RESERVED)

void kgem_init(struct kgem *kgem, int fd, struct pci_device *dev, unsigned gen);
void kgem_reset(struct kgem *kgem);

struct kgem_bo *kgem_create_map(struct kgem *kgem,
				void *ptr, uint32_t size,
				bool read_only);

struct kgem_bo *kgem_create_for_name(struct kgem *kgem, uint32_t name);
struct kgem_bo *kgem_create_for_prime(struct kgem *kgem, int name, uint32_t size);
int kgem_bo_export_to_prime(struct kgem *kgem, struct kgem_bo *bo);

struct kgem_bo *kgem_create_linear(struct kgem *kgem, int size, unsigned flags);
struct kgem_bo *kgem_create_proxy(struct kgem *kgem,
				  struct kgem_bo *target,
				  int offset, int length);

struct kgem_bo *kgem_upload_source_image(struct kgem *kgem,
					 const void *data,
					 const BoxRec *box,
					 int stride, int bpp);
void kgem_proxy_bo_attach(struct kgem_bo *bo, struct kgem_bo **ptr);

int kgem_choose_tiling(struct kgem *kgem,
		       int tiling, int width, int height, int bpp);
unsigned kgem_can_create_2d(struct kgem *kgem, int width, int height, int depth);
#define KGEM_CAN_CREATE_GPU	0x1
#define KGEM_CAN_CREATE_CPU	0x2
#define KGEM_CAN_CREATE_LARGE	0x4
#define KGEM_CAN_CREATE_GTT	0x8
#define KGEM_CAN_CREATE_TILED	0x10

bool kgem_check_surface_size(struct kgem *kgem,
			     uint32_t width,
			     uint32_t height,
			     uint32_t bpp,
			     uint32_t tiling,
			     uint32_t pitch,
			     uint32_t size);

struct kgem_bo *
kgem_replace_bo(struct kgem *kgem,
		struct kgem_bo *src,
		uint32_t width,
		uint32_t height,
		uint32_t pitch,
		uint32_t bpp);
enum {
	CREATE_EXACT = 0x1,
	CREATE_INACTIVE = 0x2,
	CREATE_CPU_MAP = 0x4,
	CREATE_GTT_MAP = 0x8,
	CREATE_SCANOUT = 0x10,
	CREATE_PRIME = 0x20,
	CREATE_TEMPORARY = 0x40,
	CREATE_CACHED = 0x80,
	CREATE_NO_RETIRE = 0x100,
	CREATE_NO_THROTTLE = 0x200,
};
struct kgem_bo *kgem_create_2d(struct kgem *kgem,
			       int width,
			       int height,
			       int bpp,
			       int tiling,
			       uint32_t flags);
struct kgem_bo *kgem_create_cpu_2d(struct kgem *kgem,
				   int width,
				   int height,
				   int bpp,
				   uint32_t flags);

bool kgem_bo_convert_to_gpu(struct kgem *kgem,
			    struct kgem_bo *bo,
			    unsigned flags);

uint32_t kgem_bo_get_binding(struct kgem_bo *bo, uint32_t format);
void kgem_bo_set_binding(struct kgem_bo *bo, uint32_t format, uint16_t offset);

bool kgem_retire(struct kgem *kgem);
void kgem_retire__buffers(struct kgem *kgem);

static inline bool kgem_bo_discard_cache(struct kgem_bo *bo, bool force)
{
	if (bo == NULL || bo->proxy == NULL)
		return false;

	if (force)
		return true;

	if (bo->proxy->rq)
		return false;

	return bo->snoop;
}

bool __kgem_ring_is_idle(struct kgem *kgem, int ring);
static inline bool kgem_ring_is_idle(struct kgem *kgem, int ring)
{
	ring = ring == KGEM_BLT;

	if (list_is_empty(&kgem->requests[ring]))
		return true;

	return __kgem_ring_is_idle(kgem, ring);
}

static inline bool kgem_is_idle(struct kgem *kgem)
{
	if (!kgem->need_retire)
		return true;

	return kgem_ring_is_idle(kgem, kgem->ring);
}

static inline bool __kgem_ring_empty(struct kgem *kgem)
{
	return list_is_empty(&kgem->requests[kgem->ring == KGEM_BLT]);
}

void _kgem_submit(struct kgem *kgem);
static inline void kgem_submit(struct kgem *kgem)
{
	if (kgem->nbatch)
		_kgem_submit(kgem);
}

static inline void kgem_bo_submit(struct kgem *kgem, struct kgem_bo *bo)
{
	if (bo->exec == NULL)
		return;

	assert(bo->refcnt);
	_kgem_submit(kgem);
}

void kgem_scanout_flush(struct kgem *kgem, struct kgem_bo *bo);

static inline struct kgem_bo *kgem_bo_reference(struct kgem_bo *bo)
{
	assert(bo->refcnt);
	bo->refcnt++;
	return bo;
}

void _kgem_bo_destroy(struct kgem *kgem, struct kgem_bo *bo);
static inline void kgem_bo_destroy(struct kgem *kgem, struct kgem_bo *bo)
{
	assert(bo->refcnt);
	if (--bo->refcnt == 0)
		_kgem_bo_destroy(kgem, bo);
}

void kgem_clear_dirty(struct kgem *kgem);

static inline void kgem_set_mode(struct kgem *kgem,
				 enum kgem_mode mode,
				 struct kgem_bo *bo)
{
	assert(!kgem->wedged);

#if DEBUG_FLUSH_BATCH
	kgem_submit(kgem);
#endif

	if (kgem->nreloc && bo->exec == NULL && kgem_ring_is_idle(kgem, kgem->ring)) {
		DBG(("%s: flushing before new bo\n", __FUNCTION__));
		_kgem_submit(kgem);
	}

	if (kgem->mode == mode)
		return;

	kgem->context_switch(kgem, mode);
	kgem->mode = mode;
}

static inline void _kgem_set_mode(struct kgem *kgem, enum kgem_mode mode)
{
	assert(kgem->mode == KGEM_NONE);
	assert(kgem->nbatch == 0);
	assert(!kgem->wedged);
	kgem->context_switch(kgem, mode);
	kgem->mode = mode;
}

static inline bool kgem_check_batch(struct kgem *kgem, int num_dwords)
{
	assert(num_dwords > 0);
	assert(kgem->nbatch < kgem->surface);
	assert(kgem->surface <= kgem->batch_size);
	return likely(kgem->nbatch + num_dwords + KGEM_BATCH_RESERVED <= kgem->surface);
}

static inline bool kgem_check_reloc(struct kgem *kgem, int n)
{
	assert(kgem->nreloc <= KGEM_RELOC_SIZE(kgem));
	return likely(kgem->nreloc + n <= KGEM_RELOC_SIZE(kgem));
}

static inline bool kgem_check_exec(struct kgem *kgem, int n)
{
	assert(kgem->nexec <= KGEM_EXEC_SIZE(kgem));
	return likely(kgem->nexec + n <= KGEM_EXEC_SIZE(kgem));
}

static inline bool kgem_check_reloc_and_exec(struct kgem *kgem, int n)
{
	return kgem_check_reloc(kgem, n) && kgem_check_exec(kgem, n);
}

static inline bool kgem_check_batch_with_surfaces(struct kgem *kgem,
						  int num_dwords,
						  int num_surfaces)
{
	return (int)(kgem->nbatch + num_dwords + KGEM_BATCH_RESERVED) <= (int)(kgem->surface - num_surfaces*8) &&
		kgem_check_reloc(kgem, num_surfaces) &&
		kgem_check_exec(kgem, num_surfaces);
}

static inline uint32_t *kgem_get_batch(struct kgem *kgem)
{
	if (kgem->nreloc) {
		unsigned mode = kgem->mode;
		_kgem_submit(kgem);
		_kgem_set_mode(kgem, mode);
	}

	return kgem->batch + kgem->nbatch;
}

bool kgem_check_bo(struct kgem *kgem, ...) __attribute__((sentinel(0)));
bool kgem_check_bo_fenced(struct kgem *kgem, struct kgem_bo *bo);
bool kgem_check_many_bo_fenced(struct kgem *kgem, ...) __attribute__((sentinel(0)));

#define KGEM_RELOC_FENCED 0x8000
uint32_t kgem_add_reloc(struct kgem *kgem,
			uint32_t pos,
			struct kgem_bo *bo,
			uint32_t read_write_domains,
			uint32_t delta);
uint64_t kgem_add_reloc64(struct kgem *kgem,
			  uint32_t pos,
			  struct kgem_bo *bo,
			  uint32_t read_write_domains,
			  uint64_t delta);

void *kgem_bo_map(struct kgem *kgem, struct kgem_bo *bo);
void *kgem_bo_map__async(struct kgem *kgem, struct kgem_bo *bo);
void *kgem_bo_map__gtt(struct kgem *kgem, struct kgem_bo *bo);
void kgem_bo_sync__gtt(struct kgem *kgem, struct kgem_bo *bo);
void *kgem_bo_map__debug(struct kgem *kgem, struct kgem_bo *bo);
void *kgem_bo_map__cpu(struct kgem *kgem, struct kgem_bo *bo);
void kgem_bo_sync__cpu(struct kgem *kgem, struct kgem_bo *bo);
void kgem_bo_sync__cpu_full(struct kgem *kgem, struct kgem_bo *bo, bool write);
uint32_t kgem_bo_flink(struct kgem *kgem, struct kgem_bo *bo);

bool kgem_bo_write(struct kgem *kgem, struct kgem_bo *bo,
		   const void *data, int length);

int kgem_bo_fenced_size(struct kgem *kgem, struct kgem_bo *bo);
void kgem_get_tile_size(struct kgem *kgem, int tiling, int pitch,
			int *tile_width, int *tile_height, int *tile_size);

static inline int __kgem_buffer_size(struct kgem_bo *bo)
{
	assert(bo->proxy != NULL);
	return bo->size.bytes;
}

static inline int __kgem_bo_size(struct kgem_bo *bo)
{
	assert(bo->proxy == NULL);
	return PAGE_SIZE * bo->size.pages.count;
}

static inline int __kgem_bo_num_pages(struct kgem_bo *bo)
{
	assert(bo->proxy == NULL);
	return bo->size.pages.count;
}

static inline int kgem_bo_size(struct kgem_bo *bo)
{
	if (bo->proxy)
		return __kgem_buffer_size(bo);
	else
		return __kgem_bo_size(bo);
}

static inline bool kgem_bo_blt_pitch_is_ok(struct kgem *kgem,
					   struct kgem_bo *bo)
{
	int pitch = bo->pitch;
	if (kgem->gen >= 040 && bo->tiling)
		pitch /= 4;
	if (pitch > MAXSHORT) {
		DBG(("%s: can not blt to handle=%d, adjusted pitch=%d\n",
		     __FUNCTION__, bo->handle, pitch));
		return false;
	}

	return true;
}

static inline bool kgem_bo_can_blt(struct kgem *kgem,
				   struct kgem_bo *bo)
{
	assert(bo->refcnt);

	if (bo->tiling == I915_TILING_Y) {
		DBG(("%s: can not blt to handle=%d, tiling=Y\n",
		     __FUNCTION__, bo->handle));
		return false;
	}

	return kgem_bo_blt_pitch_is_ok(kgem, bo);
}

static inline bool kgem_bo_is_snoop(struct kgem_bo *bo)
{
	assert(bo->refcnt);
	while (bo->proxy)
		bo = bo->proxy;
	return bo->snoop;
}

void kgem_bo_undo(struct kgem *kgem, struct kgem_bo *bo);
void kgem_bo_pair_undo(struct kgem *kgem, struct kgem_bo *a, struct kgem_bo *b);

bool __kgem_busy(struct kgem *kgem, int handle);

static inline void kgem_bo_mark_busy(struct kgem *kgem, struct kgem_bo *bo, int ring)
{
	assert(bo->refcnt);
	bo->needs_flush = true;
	if (bo->rq) {
		bo->rq = MAKE_REQUEST(RQ(bo->rq), ring);
	} else {
		bo->rq = MAKE_REQUEST(kgem, ring);
		list_add(&bo->request, &kgem->flushing);
		kgem->need_retire = true;
	}
}

inline static void __kgem_bo_clear_busy(struct kgem_bo *bo)
{
	DBG(("%s: handle=%d\n", __FUNCTION__, bo->handle));
	bo->rq = NULL;
	list_del(&bo->request);

	bo->domain = DOMAIN_NONE;
	bo->needs_flush = false;
	bo->gtt_dirty = false;
}

static inline bool kgem_bo_is_busy(struct kgem_bo *bo)
{
	DBG(("%s: handle=%d, domain: %d exec? %d, rq? %d\n", __FUNCTION__,
	     bo->handle, bo->domain, bo->exec != NULL, bo->rq != NULL));
	assert(bo->refcnt);
	return bo->rq;
}

static inline bool __kgem_bo_is_busy(struct kgem *kgem, struct kgem_bo *bo)
{
	DBG(("%s: handle=%d, domain: %d exec? %d, rq? %d\n", __FUNCTION__,
	     bo->handle, bo->domain, bo->exec != NULL, bo->rq != NULL));
	assert(bo->refcnt);

	if (bo->exec)
		return true;

	if (bo->rq && !__kgem_busy(kgem, bo->handle))
		__kgem_bo_clear_busy(bo);

	return kgem_bo_is_busy(bo);
}

static inline bool kgem_bo_is_render(struct kgem_bo *bo)
{
	DBG(("%s: handle=%d, rq? %d [%d]\n", __FUNCTION__,
	     bo->handle, bo->rq != NULL, (int)RQ_RING(bo->rq)));
	assert(bo->refcnt);
	return bo->rq && RQ_RING(bo->rq) == I915_EXEC_RENDER;
}

static inline void kgem_bo_mark_unreusable(struct kgem_bo *bo)
{
	assert(bo->refcnt);
	while (bo->proxy) {
		bo->flush = true;
		bo = bo->proxy;
		assert(bo->refcnt);
	}
	bo->flush = true;
	bo->reusable = false;
}

static inline bool kgem_bo_is_dirty(struct kgem_bo *bo)
{
	if (bo == NULL)
		return false;

	assert(bo->refcnt);
	return bo->gpu_dirty;
}

static inline void kgem_bo_unclean(struct kgem *kgem, struct kgem_bo *bo)
{
	/* The bo is outside of our control, so presume it is written to */
	bo->needs_flush = true;
	if (bo->rq == NULL)
		bo->rq = (void *)kgem;

	if (bo->domain != DOMAIN_GPU)
		bo->domain = DOMAIN_NONE;
}

static inline void __kgem_bo_mark_dirty(struct kgem_bo *bo)
{
	DBG(("%s: handle=%d (proxy? %d)\n", __FUNCTION__,
	     bo->handle, bo->proxy != NULL));

	assert(bo->refcnt);
	assert(bo->exec);
	assert(bo->rq);

	bo->exec->flags |= LOCAL_EXEC_OBJECT_WRITE;
	bo->needs_flush = bo->gpu_dirty = true;
	list_move(&bo->request, &RQ(bo->rq)->buffers);
}

static inline void kgem_bo_mark_dirty(struct kgem_bo *bo)
{
	assert(bo->refcnt);
	do {
		assert(bo->exec);
		assert(bo->rq);

		if (bo->gpu_dirty)
			return;

		__kgem_bo_mark_dirty(bo);
	} while ((bo = bo->proxy));
}

static inline bool kgem_bo_mapped(struct kgem *kgem, struct kgem_bo *bo)
{
	DBG(("%s: handle=%d, map=%p:%p, tiling=%d, domain=%d\n",
	     __FUNCTION__, bo->handle, bo->map__gtt, bo->map__cpu, bo->tiling, bo->domain));
	assert(bo->proxy == NULL);

	if (bo->tiling == I915_TILING_NONE && (bo->domain == DOMAIN_CPU || kgem->has_llc))
		return bo->map__cpu != NULL;

	return bo->map__gtt != NULL;
}

static inline bool kgem_bo_can_map(struct kgem *kgem, struct kgem_bo *bo)
{
	DBG(("%s: handle=%d, map=%p:%p, tiling=%d, domain=%d, offset=%ld\n",
	     __FUNCTION__, bo->handle, bo->map__gtt, bo->map__cpu, bo->tiling, bo->domain, (long)bo->presumed_offset));

	if (!bo->tiling && (kgem->has_llc || bo->domain == DOMAIN_CPU))
		return true;

	assert(bo->proxy == NULL);

	if (bo->map__gtt != NULL)
		return true;

	if (kgem->gen == 021 && bo->tiling == I915_TILING_Y)
		return false;

	return __kgem_bo_num_pages(bo) <= kgem->aperture_mappable / 4;
}

static inline bool kgem_bo_can_map__cpu(struct kgem *kgem,
					struct kgem_bo *bo,
					bool write)
{
	DBG(("%s: handle=%d\n", __FUNCTION__, bo->handle));
	assert(bo->refcnt);

	if (bo->purged || (bo->scanout && write)) {
		DBG(("%s: no, writing to scanout? %d, or is stolen [inaccessible via CPU]? %d\n",
		     __FUNCTION__, bo->scanout && write, bo->purged));
		return false;
	}

	if (kgem->has_llc) {
		DBG(("%s: yes, has LLC and target is in LLC\n", __FUNCTION__));
		return true;
	}

	DBG(("%s: non-LLC - CPU domain? %d, clean? %d\n",
	     __FUNCTION__, bo->domain == DOMAIN_CPU, !write || bo->exec == NULL));
	if (bo->domain != DOMAIN_CPU)
		return false;

	return !write || bo->exec == NULL;
}

#define KGEM_BUFFER_WRITE	0x1
#define KGEM_BUFFER_INPLACE	0x2
#define KGEM_BUFFER_LAST	0x4

#define KGEM_BUFFER_WRITE_INPLACE (KGEM_BUFFER_WRITE | KGEM_BUFFER_INPLACE)

struct kgem_bo *kgem_create_buffer(struct kgem *kgem,
				   uint32_t size, uint32_t flags,
				   void **ret);
struct kgem_bo *kgem_create_buffer_2d(struct kgem *kgem,
				      int width, int height, int bpp,
				      uint32_t flags,
				      void **ret);
bool kgem_buffer_is_inplace(struct kgem_bo *bo);
void kgem_buffer_read_sync(struct kgem *kgem, struct kgem_bo *bo);

int kgem_is_wedged(struct kgem *kgem);
void kgem_throttle(struct kgem *kgem);
#define MAX_INACTIVE_TIME 10
bool kgem_expire_cache(struct kgem *kgem);
bool kgem_cleanup_cache(struct kgem *kgem);

void kgem_clean_scanout_cache(struct kgem *kgem);
void kgem_clean_large_cache(struct kgem *kgem);

#if HAS_DEBUG_FULL
void __kgem_batch_debug(struct kgem *kgem, uint32_t nbatch);
#else
static inline void __kgem_batch_debug(struct kgem *kgem, uint32_t nbatch)
{
	(void)kgem;
	(void)nbatch;
}
#endif

static inline void
memcpy_to_tiled_x(struct kgem *kgem,
		  const void *src, void *dst, int bpp,
		  int32_t src_stride, int32_t dst_stride,
		  int16_t src_x, int16_t src_y,
		  int16_t dst_x, int16_t dst_y,
		  uint16_t width, uint16_t height)
{
	assert(kgem->memcpy_to_tiled_x);
	assert(src_x >= 0 && src_y >= 0);
	assert(dst_x >= 0 && dst_y >= 0);
	assert(8*src_stride >= (src_x+width) * bpp);
	assert(8*dst_stride >= (dst_x+width) * bpp);
	return kgem->memcpy_to_tiled_x(src, dst, bpp,
				       src_stride, dst_stride,
				       src_x, src_y,
				       dst_x, dst_y,
				       width, height);
}

static inline void
memcpy_from_tiled_x(struct kgem *kgem,
		    const void *src, void *dst, int bpp,
		    int32_t src_stride, int32_t dst_stride,
		    int16_t src_x, int16_t src_y,
		    int16_t dst_x, int16_t dst_y,
		    uint16_t width, uint16_t height)
{
	assert(kgem->memcpy_from_tiled_x);
	assert(src_x >= 0 && src_y >= 0);
	assert(dst_x >= 0 && dst_y >= 0);
	assert(8*src_stride >= (src_x+width) * bpp);
	assert(8*dst_stride >= (dst_x+width) * bpp);
	return kgem->memcpy_from_tiled_x(src, dst, bpp,
					 src_stride, dst_stride,
					 src_x, src_y,
					 dst_x, dst_y,
					 width, height);
}

void choose_memcpy_tiled_x(struct kgem *kgem, int swizzling);

#endif /* KGEM_H */
