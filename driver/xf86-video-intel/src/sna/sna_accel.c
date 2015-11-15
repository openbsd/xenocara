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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "sna.h"
#include "sna_reg.h"
#include "sna_video.h"
#include "rop.h"

#include "intel_options.h"

#include <X11/fonts/font.h>
#include <X11/fonts/fontstruct.h>

#include <dixfontstr.h>

#include <mi.h>
#include <migc.h>
#include <miline.h>
#include <micmap.h>
#ifdef RENDER
#include <mipict.h>
#endif
#include <shmint.h>

#include <sys/time.h>
#include <sys/mman.h>
#include <unistd.h>

#ifdef HAVE_VALGRIND
#include <valgrind.h>
#include <memcheck.h>
#endif

#define FAULT_INJECTION 0

#define FORCE_INPLACE 0
#define FORCE_FALLBACK 0
#define FORCE_FLUSH 0
#define FORCE_FULL_SYNC 0 /* https://bugs.freedesktop.org/show_bug.cgi?id=61628 */

#define DEFAULT_TILING I915_TILING_X

#define USE_INPLACE 1
#define USE_WIDE_SPANS 0 /* -1 force CPU, 1 force GPU */
#define USE_ZERO_SPANS 1 /* -1 force CPU, 1 force GPU */
#define USE_CPU_BO 1
#define USE_USERPTR_UPLOADS 1
#define USE_USERPTR_DOWNLOADS 1
#define USE_COW 1
#define UNDO 1

#define MIGRATE_ALL 0
#define DBG_NO_PARTIAL_MOVE_TO_CPU 0
#define DBG_NO_CPU_UPLOAD 0
#define DBG_NO_CPU_DOWNLOAD 0

#define ACCEL_FILL_SPANS 1
#define ACCEL_SET_SPANS 1
#define ACCEL_PUT_IMAGE 1
#define ACCEL_GET_IMAGE 1
#define ACCEL_COPY_AREA 1
#define ACCEL_COPY_PLANE 1
#define ACCEL_COPY_WINDOW 1
#define ACCEL_POLY_POINT 1
#define ACCEL_POLY_LINE 1
#define ACCEL_POLY_SEGMENT 1
#define ACCEL_POLY_RECTANGLE 1
#define ACCEL_POLY_ARC 1
#define ACCEL_POLY_FILL_POLYGON 1
#define ACCEL_POLY_FILL_RECT 1
#define ACCEL_POLY_FILL_ARC 1
#define ACCEL_POLY_TEXT8 1
#define ACCEL_POLY_TEXT16 1
#define ACCEL_POLY_GLYPH 1
#define ACCEL_IMAGE_TEXT8 1
#define ACCEL_IMAGE_TEXT16 1
#define ACCEL_IMAGE_GLYPH 1
#define ACCEL_PUSH_PIXELS 1

#define NO_TILE_8x8 0
#define NO_STIPPLE_8x8 0

#define IS_COW_OWNER(ptr) ((uintptr_t)(ptr) & 1)
#define MAKE_COW_OWNER(ptr) ((void*)((uintptr_t)(ptr) | 1))
#define COW(ptr) (void *)((uintptr_t)(ptr) & ~1)

#if 0
static void __sna_fallback_flush(DrawablePtr d)
{
	PixmapPtr pixmap = get_drawable_pixmap(d);
	struct sna *sna = to_sna_from_pixmap(pixmap);
	struct sna_pixmap *priv;
	BoxRec box;
	PixmapPtr tmp;
	int i, j;
	char *src, *dst;

	DBG(("%s: uploading CPU damage...\n", __FUNCTION__));
	priv = sna_pixmap_move_to_gpu(pixmap, MOVE_READ);
	if (priv == NULL)
		return;

	DBG(("%s: downloading GPU damage...\n", __FUNCTION__));
	if (!sna_pixmap_move_to_cpu(pixmap, MOVE_READ))
		return;

	box.x1 = box.y1 = 0;
	box.x2 = pixmap->drawable.width;
	box.y2 = pixmap->drawable.height;

	tmp = sna_pixmap_create_unattached(pixmap->drawable.pScreen,
					   pixmap->drawable.width,
					   pixmap->drawable.height,
					   pixmap->drawable.depth,
					   0);

	DBG(("%s: comparing with direct read...\n", __FUNCTION__));
	sna_read_boxes(sna, tmp, priv->gpu_bo, &box, 1);

	src = pixmap->devPrivate.ptr;
	dst = tmp->devPrivate.ptr;
	for (i = 0; i < tmp->drawable.height; i++) {
		if (memcmp(src, dst, tmp->drawable.width * tmp->drawable.bitsPerPixel >> 3)) {
			for (j = 0; src[j] == dst[j]; j++)
				;
			ERR(("mismatch at (%d, %d)\n",
			     8*j / tmp->drawable.bitsPerPixel, i));
			abort();
		}
		src += pixmap->devKind;
		dst += tmp->devKind;
	}
	tmp->drawable.pScreen->DestroyPixmap(tmp);
}
#define FALLBACK_FLUSH(d) __sna_fallback_flush(d)
#else
#define FALLBACK_FLUSH(d)
#endif

static int sna_font_key;

static const uint8_t copy_ROP[] = {
	ROP_0,		/* GXclear */
	ROP_DSa,	/* GXand */
	ROP_SDna,	/* GXandReverse */
	ROP_S,		/* GXcopy */
	ROP_DSna,	/* GXandInverted */
	ROP_D,		/* GXnoop */
	ROP_DSx,	/* GXxor */
	ROP_DSo,	/* GXor */
	ROP_DSon,	/* GXnor */
	ROP_DSxn,	/* GXequiv */
	ROP_Dn,		/* GXinvert */
	ROP_SDno,	/* GXorReverse */
	ROP_Sn,		/* GXcopyInverted */
	ROP_DSno,	/* GXorInverted */
	ROP_DSan,	/* GXnand */
	ROP_1		/* GXset */
};
static const uint8_t fill_ROP[] = {
	ROP_0,
	ROP_DPa,
	ROP_PDna,
	ROP_P,
	ROP_DPna,
	ROP_D,
	ROP_DPx,
	ROP_DPo,
	ROP_DPon,
	ROP_PDxn,
	ROP_Dn,
	ROP_PDno,
	ROP_Pn,
	ROP_DPno,
	ROP_DPan,
	ROP_1
};

static const GCOps sna_gc_ops;
static const GCOps sna_gc_ops__cpu;
static GCOps sna_gc_ops__tmp;
static const GCFuncs sna_gc_funcs;
static const GCFuncs sna_gc_funcs__cpu;

static void
sna_poly_fill_rect__gpu(DrawablePtr draw, GCPtr gc, int n, xRectangle *rect);

static inline void region_set(RegionRec *r, const BoxRec *b)
{
	r->extents = *b;
	r->data = NULL;
}

static inline bool region_maybe_clip(RegionRec *r, RegionRec *clip)
{
	if (clip->data && !RegionIntersect(r, r, clip))
		return false;

	return !box_empty(&r->extents);
}

static inline bool region_is_singular(const RegionRec *r)
{
	return r->data == NULL;
}

static inline bool region_is_unclipped(const RegionRec *r, int w, int h)
{
	return (region_is_singular(r) &&
		w == r->extents.x2 - r->extents.x1 &&
		h == r->extents.y2 - r->extents.y1);
}

typedef struct box32 {
	int32_t x1, y1, x2, y2;
} Box32Rec;

#define PM_IS_SOLID(_draw, _pm) \
	(((_pm) & FbFullMask((_draw)->depth)) == FbFullMask((_draw)->depth))

#ifndef NDEBUG
static void _assert_pixmap_contains_box(PixmapPtr pixmap, const BoxRec *box, const char *function)
{
	if (box->x1 < 0 || box->y1 < 0 ||
	    box->x2 > pixmap->drawable.width ||
	    box->y2 > pixmap->drawable.height)
	{
		FatalError("%s: damage box [(%d, %d), (%d, %d)] is beyond the pixmap=%ld size=%dx%d\n",
			   function, box->x1, box->y1, box->x2, box->y2,
			   pixmap->drawable.serialNumber,
			   pixmap->drawable.width,
			   pixmap->drawable.height);
	}
}

static void
_assert_pixmap_contains_damage(PixmapPtr pixmap, struct sna_damage *damage, const char *function)
{
	if (damage == NULL)
		return;

	_assert_pixmap_contains_box(pixmap, &DAMAGE_PTR(damage)->extents, function);
}
#define assert_pixmap_contains_damage(p,d) _assert_pixmap_contains_damage(p, d, __FUNCTION__)
#else
#define assert_pixmap_contains_damage(p,d)
#endif

#define __assert_pixmap_damage(p) do { \
	struct sna_pixmap *priv__ = sna_pixmap(p); \
	if (priv__) { \
		assert(priv__->gpu_damage == NULL || priv__->gpu_bo); \
		assert(priv__->gpu_bo == NULL || priv__->gpu_bo->refcnt); \
		assert(priv__->cpu_bo == NULL || priv__->cpu_bo->refcnt); \
		assert_pixmap_contains_damage(p, priv__->gpu_damage); \
		assert_pixmap_contains_damage(p, priv__->cpu_damage); \
		assert_pixmap_map(p, priv__); \
	} \
} while (0)

#ifdef DEBUG_PIXMAP
static void _assert_pixmap_contains_box_with_offset(PixmapPtr pixmap, const BoxRec *box, int dx, int dy, const char *function)
{
	BoxRec b = *box;
	b.x1 += dx; b.x2 += dx;
	b.y1 += dy; b.y2 += dy;
	_assert_pixmap_contains_box(pixmap, &b, function);
}

static void _assert_pixmap_contains_boxes(PixmapPtr pixmap, const BoxRec *box, int n, int dx, int dy, const char *function)
{
	BoxRec extents;

	extents = *box;
	while (--n) {
		++box;

		if (box->x1 < extents.x1)
			extents.x1 = box->x1;
		if (box->x2 > extents.x2)
			extents.x2 = box->x2;

		if (box->y1 < extents.y1)
			extents.y1 = box->y1;
		if (box->y2 > extents.y2)
			extents.y2 = box->y2;
	}
	extents.x1 += dx;
	extents.x2 += dx;
	extents.y1 += dy;
	extents.y2 += dy;
	_assert_pixmap_contains_box(pixmap, &extents, function);
}


static void _assert_pixmap_contains_points(PixmapPtr pixmap, const DDXPointRec *pt, int n, int dx, int dy, const char *function)
{
	BoxRec extents;

	extents.x2 = extents.x1 = pt->x;
	extents.y2 = extents.y1 = pt->y;
	while (--n) {
		++pt;

		if (pt->x < extents.x1)
			extents.x1 = pt->x;
		else if (pt->x > extents.x2)
			extents.x2 = pt->x;

		if (pt->y < extents.y1)
			extents.y1 = pt->y;
		else if (pt->y > extents.y2)
			extents.y2 = pt->y;
	}
	extents.x1 += dx;
	extents.x2 += dx + 1;
	extents.y1 += dy;
	extents.y2 += dy + 1;
	_assert_pixmap_contains_box(pixmap, &extents, function);
}

static void _assert_drawable_contains_box(DrawablePtr drawable, const BoxRec *box, const char *function)
{
	if (box->x1 < drawable->x ||
	    box->y1 < drawable->y ||
	    box->x2 > drawable->x + drawable->width ||
	    box->y2 > drawable->y + drawable->height)
	{
		FatalError("%s: damage box is beyond the drawable: box=(%d, %d), (%d, %d), drawable=(%d, %d)x(%d, %d)\n",
			   function,
			   box->x1, box->y1, box->x2, box->y2,
			   drawable->x, drawable->y,
			   drawable->width, drawable->height);
	}
}

static void assert_pixmap_damage(PixmapPtr p)
{
	struct sna_pixmap *priv;
	RegionRec reg, cpu, gpu;

	priv = sna_pixmap(p);
	if (priv == NULL)
		return;

	__assert_pixmap_damage(p);

	if (priv->clear) {
		assert(DAMAGE_IS_ALL(priv->gpu_damage));
		assert(priv->cpu_damage == NULL);
	}

	if (DAMAGE_IS_ALL(priv->gpu_damage) && DAMAGE_IS_ALL(priv->cpu_damage)) {
		/* special upload buffer */
		assert(priv->gpu_bo && priv->gpu_bo->proxy);
		assert(priv->cpu_bo == NULL);
		return;
	}

	assert(!DAMAGE_IS_ALL(priv->gpu_damage) || priv->cpu_damage == NULL);
	assert(!DAMAGE_IS_ALL(priv->cpu_damage) || priv->gpu_damage == NULL);

	/* Avoid reducing damage to minimise interferrence */
	RegionNull(&reg);
	RegionNull(&gpu);
	RegionNull(&cpu);

	if (priv->gpu_damage)
		_sna_damage_debug_get_region(DAMAGE_PTR(priv->gpu_damage), &gpu);

	if (priv->cpu_damage)
		_sna_damage_debug_get_region(DAMAGE_PTR(priv->cpu_damage), &cpu);

	RegionIntersect(&reg, &cpu, &gpu);
	assert(RegionNil(&reg));

	RegionUninit(&reg);
	RegionUninit(&gpu);
	RegionUninit(&cpu);
}

#define assert_pixmap_contains_box(p, b) _assert_pixmap_contains_box(p, b, __FUNCTION__)
#define assert_pixmap_contains_box_with_offset(p, b, dx, dy) _assert_pixmap_contains_box_with_offset(p, b, dx, dy, __FUNCTION__)
#define assert_drawable_contains_box(d, b) _assert_drawable_contains_box(d, b, __FUNCTION__)
#define assert_pixmap_contains_boxes(p, b, n, x, y) _assert_pixmap_contains_boxes(p, b, n, x, y, __FUNCTION__)
#define assert_pixmap_contains_points(p, pt, n, x, y) _assert_pixmap_contains_points(p, pt, n, x, y, __FUNCTION__)

#else
#define assert_pixmap_contains_box(p, b)
#define assert_pixmap_contains_box_with_offset(p, b, dx, dy)
#define assert_pixmap_contains_boxes(p, b, n, x, y)
#define assert_pixmap_contains_points(p, pt, n, x, y)
#define assert_drawable_contains_box(d, b)
#ifndef NDEBUG
#define assert_pixmap_damage(p) __assert_pixmap_damage(p)
#else
#define assert_pixmap_damage(p)
#endif
#endif

jmp_buf sigjmp[4];
volatile sig_atomic_t sigtrap;

static int sigtrap_handler(int sig)
{
	/* XXX rate-limited squawk? */
	DBG(("%s(sig=%d) sigtrap=%d\n", __FUNCTION__, sig, sigtrap));
	sna_threads_trap(sig);

	if (sigtrap)
		siglongjmp(sigjmp[--sigtrap], sig);

	return -1;
}

static void sigtrap_init(void)
{
#if XORG_VERSION_CURRENT >= XORG_VERSION_NUMERIC(1,6,99,900,0)
	OsRegisterSigWrapper(sigtrap_handler);
#endif
}

inline static bool
sna_fill_init_blt(struct sna_fill_op *fill,
		  struct sna *sna,
		  PixmapPtr pixmap,
		  struct kgem_bo *bo,
		  uint8_t alu,
		  uint32_t pixel,
		  unsigned flags)
{
	return sna->render.fill(sna, alu, pixmap, bo, pixel, flags, fill);
}

static bool
sna_copy_init_blt(struct sna_copy_op *copy,
		  struct sna *sna,
		  PixmapPtr src, struct kgem_bo *src_bo,
		  PixmapPtr dst, struct kgem_bo *dst_bo,
		  uint8_t alu)
{
	memset(copy, 0, sizeof(*copy));
	return sna->render.copy(sna, alu, src, src_bo, dst, dst_bo, copy);
}

static void sna_pixmap_free_gpu(struct sna *sna, struct sna_pixmap *priv)
{
	DBG(("%s: handle=%d (pinned? %d)\n", __FUNCTION__, priv->gpu_bo ? priv->gpu_bo->handle : 0, priv->pinned));
	assert(priv->gpu_damage == NULL || priv->gpu_bo);

	if (priv->cow)
		sna_pixmap_undo_cow(sna, priv, MOVE_WRITE);
	assert(priv->cow == NULL);

	if (priv->move_to_gpu) {
		sna_pixmap_discard_shadow_damage(priv, NULL);
		priv->move_to_gpu(sna, priv, MOVE_WRITE);
	}

	sna_damage_destroy(&priv->gpu_damage);
	priv->clear = false;

	if (priv->gpu_bo) {
		if (!priv->pinned) {
			assert(!priv->flush);
			assert(!priv->move_to_gpu);
			sna_pixmap_unmap(priv->pixmap, priv);
			kgem_bo_destroy(&sna->kgem, priv->gpu_bo);
			priv->gpu_bo = NULL;
		} else
			kgem_bo_undo(&sna->kgem, priv->gpu_bo);
	}

	/* and reset the upload counter */
	priv->source_count = SOURCE_BIAS;
}

static bool must_check
sna_pixmap_alloc_cpu(struct sna *sna,
		     PixmapPtr pixmap,
		     struct sna_pixmap *priv,
		     unsigned flags)
{
	/* Restore after a GTT mapping? */
	assert(priv->gpu_damage == NULL || priv->gpu_bo);
	assert(!priv->shm);
	if (priv->ptr)
		goto done;

	DBG(("%s: pixmap=%ld\n", __FUNCTION__, pixmap->drawable.serialNumber));
	assert(priv->stride);

	if (priv->create & KGEM_CAN_CREATE_CPU) {
		unsigned hint;

		DBG(("%s: allocating CPU buffer (%dx%d)\n", __FUNCTION__,
		     pixmap->drawable.width, pixmap->drawable.height));

		hint = 0;
		if ((flags & MOVE_ASYNC_HINT) == 0 &&
		    ((flags & MOVE_READ) == 0 || (priv->gpu_damage && !priv->clear && !sna->kgem.has_llc)))
			hint = CREATE_CPU_MAP | CREATE_INACTIVE | CREATE_NO_THROTTLE;

		priv->cpu_bo = kgem_create_cpu_2d(&sna->kgem,
						  pixmap->drawable.width,
						  pixmap->drawable.height,
						  pixmap->drawable.bitsPerPixel,
						  hint);
		if (priv->cpu_bo) {
			priv->ptr = kgem_bo_map__cpu(&sna->kgem, priv->cpu_bo);
			if (priv->ptr) {
				DBG(("%s: allocated CPU handle=%d (snooped? %d)\n", __FUNCTION__,
				     priv->cpu_bo->handle, priv->cpu_bo->snoop));
				priv->stride = priv->cpu_bo->pitch;
#ifdef DEBUG_MEMORY
				sna->debug_memory.cpu_bo_allocs++;
				sna->debug_memory.cpu_bo_bytes += kgem_bo_size(priv->cpu_bo);
#endif
			} else {
				kgem_bo_destroy(&sna->kgem, priv->cpu_bo);
				priv->cpu_bo = NULL;
			}
		}
	}

	if (priv->ptr == NULL) {
		DBG(("%s: allocating ordinary memory for shadow pixels [%d bytes]\n",
		     __FUNCTION__, priv->stride * pixmap->drawable.height));
		priv->ptr = malloc(priv->stride * pixmap->drawable.height);
	}

done:
	assert(priv->stride);
	assert(!priv->mapped);
	pixmap->devPrivate.ptr = PTR(priv->ptr);
	pixmap->devKind = priv->stride;
	return priv->ptr != NULL;
}

static void __sna_pixmap_free_cpu(struct sna *sna, struct sna_pixmap *priv)
{
	if (priv->cpu_bo) {
		DBG(("%s: discarding CPU buffer, handle=%d, size=%d\n",
		     __FUNCTION__, priv->cpu_bo->handle, kgem_bo_size(priv->cpu_bo)));
#ifdef DEBUG_MEMORY
		sna->debug_memory.cpu_bo_allocs--;
		sna->debug_memory.cpu_bo_bytes -= kgem_bo_size(priv->cpu_bo);
#endif
		if (priv->cpu_bo->flush) {
			assert(!priv->cpu_bo->reusable);
			kgem_bo_sync__cpu(&sna->kgem, priv->cpu_bo);
			sna_accel_watch_flush(sna, -1);
		}
		kgem_bo_destroy(&sna->kgem, priv->cpu_bo);
	} else if (!IS_STATIC_PTR(priv->ptr))
		free(priv->ptr);
}

static bool sna_pixmap_free_cpu(struct sna *sna, struct sna_pixmap *priv, bool active)
{
	if (active)
		return false;

	if (IS_STATIC_PTR(priv->ptr))
		return false;

	if (priv->ptr == NULL)
		return true;

	__sna_pixmap_free_cpu(sna, priv);

	priv->cpu_bo = NULL;
	priv->ptr = NULL;

	if (priv->mapped == MAPPED_NONE)
		priv->pixmap->devPrivate.ptr = NULL;

	return true;
}

static inline uint32_t default_tiling(struct sna *sna, PixmapPtr pixmap)
{
#if DEFAULT_TILING == I915_TILING_NONE
	return I915_TILING_NONE;
#elif DEFAULT_TILING == I915_TILING_X
	return I915_TILING_X;
#else
	/* Try to avoid hitting the Y-tiling GTT mapping bug on 855GM */
	if (sna->kgem.gen == 021)
		return I915_TILING_X;

	/* Only on later generations was the render pipeline
	 * more flexible than the BLT. So on gen2/3, prefer to
	 * keep large objects accessible through the BLT.
	 */
	if (sna->kgem.gen < 040 &&
	    (pixmap->drawable.width  > sna->render.max_3d_size ||
	     pixmap->drawable.height > sna->render.max_3d_size))
		return I915_TILING_X;

	if (sna_damage_is_all(&sna_pixmap(pixmap)->cpu_damage,
			      pixmap->drawable.width,
			      pixmap->drawable.height)) {
		DBG(("%s: entire source is damaged, using Y-tiling\n",
		     __FUNCTION__));
		sna_damage_destroy(&sna_pixmap(priv)->gpu_damage);
		return I915_TILING_Y;
	}

	return I915_TILING_Y;
#endif
}

pure static uint32_t sna_pixmap_default_tiling(struct sna *sna, PixmapPtr pixmap)
{
	/* Also adjust tiling if it is not supported or likely to
	 * slow us down,
	 */
	return kgem_choose_tiling(&sna->kgem,
				  default_tiling(sna, pixmap),
				  pixmap->drawable.width,
				  pixmap->drawable.height,
				  pixmap->drawable.bitsPerPixel);
}

struct kgem_bo *sna_pixmap_change_tiling(PixmapPtr pixmap, uint32_t tiling)
{
	struct sna_pixmap *priv = sna_pixmap(pixmap);
	struct sna *sna = to_sna_from_pixmap(pixmap);
	struct kgem_bo *bo;
	BoxRec box;

	DBG(("%s: changing tiling %d -> %d for %dx%d pixmap\n",
	     __FUNCTION__, priv->gpu_bo->tiling, tiling,
	     pixmap->drawable.width, pixmap->drawable.height));
	assert(priv->gpu_damage == NULL || priv->gpu_bo);

	if (priv->pinned) {
		DBG(("%s: can't convert pinned bo\n", __FUNCTION__));
		return NULL;
	}

	if (wedged(sna)) {
		DBG(("%s: can't convert bo, wedged\n", __FUNCTION__));
		return NULL;
	}

	assert_pixmap_damage(pixmap);
	assert(!priv->move_to_gpu);

	bo = kgem_create_2d(&sna->kgem,
			    pixmap->drawable.width,
			    pixmap->drawable.height,
			    pixmap->drawable.bitsPerPixel,
			    tiling, 0);
	if (bo == NULL) {
		DBG(("%s: allocation failed\n", __FUNCTION__));
		return NULL;
	}

	box.x1 = box.y1 = 0;
	box.x2 = pixmap->drawable.width;
	box.y2 = pixmap->drawable.height;

	if (!sna->render.copy_boxes(sna, GXcopy,
				    &pixmap->drawable, priv->gpu_bo, 0, 0,
				    &pixmap->drawable, bo, 0, 0,
				    &box, 1, 0)) {
		DBG(("%s: copy failed\n", __FUNCTION__));
		kgem_bo_destroy(&sna->kgem, bo);
		return NULL;
	}

	sna_pixmap_unmap(pixmap, priv);
	kgem_bo_destroy(&sna->kgem, priv->gpu_bo);

	return priv->gpu_bo = bo;
}

static inline void sna_set_pixmap(PixmapPtr pixmap, struct sna_pixmap *sna)
{
	((void **)__get_private(pixmap, sna_pixmap_key))[1] = sna;
	assert(sna_pixmap(pixmap) == sna);
}

static struct sna_pixmap *
_sna_pixmap_init(struct sna_pixmap *priv, PixmapPtr pixmap)
{
	list_init(&priv->flush_list);
	list_init(&priv->cow_list);
	priv->source_count = SOURCE_BIAS;
	priv->pixmap = pixmap;

	return priv;
}

static struct sna_pixmap *
_sna_pixmap_reset(PixmapPtr pixmap)
{
	struct sna_pixmap *priv;

	assert(pixmap->drawable.type == DRAWABLE_PIXMAP);
	assert(pixmap->drawable.class == 0);
	assert(pixmap->drawable.x == 0);
	assert(pixmap->drawable.y == 0);

	priv = sna_pixmap(pixmap);
	assert(priv != NULL);

	memset(priv, 0, sizeof(*priv));
	return _sna_pixmap_init(priv, pixmap);
}

static struct sna_pixmap *sna_pixmap_attach(PixmapPtr pixmap)
{
	struct sna_pixmap *priv;

	priv = calloc(1, sizeof(*priv));
	if (!priv)
		return NULL;

	sna_set_pixmap(pixmap, priv);
	return _sna_pixmap_init(priv, pixmap);
}

struct sna_pixmap *sna_pixmap_attach_to_bo(PixmapPtr pixmap, struct kgem_bo *bo)
{
	struct sna_pixmap *priv;

	assert(bo);
	assert(bo->proxy == NULL);
	assert(bo->unique_id);

	priv = sna_pixmap_attach(pixmap);
	if (!priv)
		return NULL;

	DBG(("%s: attaching %s handle=%d to pixmap=%ld\n",
	     __FUNCTION__, bo->snoop ? "CPU" : "GPU", bo->handle, pixmap->drawable.serialNumber));

	assert(!priv->mapped);
	assert(!priv->move_to_gpu);

	if (bo->snoop) {
		priv->cpu_bo = bo;
		sna_damage_all(&priv->cpu_damage, pixmap);
	} else {
		priv->gpu_bo = bo;
		sna_damage_all(&priv->gpu_damage, pixmap);
	}

	return priv;
}

static int bits_per_pixel(int depth)
{
	switch (depth) {
	case 1: return 1;
	case 4:
	case 8: return 8;
	case 15:
	case 16: return 16;
	case 24:
	case 30:
	case 32: return 32;
	default: return 0;
	}
}
static PixmapPtr
create_pixmap(struct sna *sna, ScreenPtr screen,
	      int width, int height, int depth,
	      unsigned usage_hint)
{
	PixmapPtr pixmap;
	size_t datasize;
	size_t stride;
	int base, bpp;

	bpp = bits_per_pixel(depth);
	if (bpp == 0)
		return NullPixmap;

	stride = ((width * bpp + FB_MASK) >> FB_SHIFT) * sizeof(FbBits);
	if (stride / 4 > 32767 || height > 32767)
		return NullPixmap;

	datasize = height * stride;
	base = screen->totalPixmapSize;
	if (datasize && base & 15) {
		int adjust = 16 - (base & 15);
		base += adjust;
		datasize += adjust;
	}

	DBG(("%s: allocating pixmap %dx%d, depth=%d, size=%ld\n",
	     __FUNCTION__, width, height, depth, (long)datasize));
	pixmap = AllocatePixmap(screen, datasize);
	if (!pixmap)
		return NullPixmap;

	((void **)__get_private(pixmap, sna_pixmap_key))[0] = sna;
	assert(to_sna_from_pixmap(pixmap) == sna);

	pixmap->drawable.type = DRAWABLE_PIXMAP;
	pixmap->drawable.class = 0;
	pixmap->drawable.pScreen = screen;
	pixmap->drawable.depth = depth;
	pixmap->drawable.bitsPerPixel = bpp;
	pixmap->drawable.id = 0;
	pixmap->drawable.serialNumber = NEXT_SERIAL_NUMBER;
	pixmap->drawable.x = 0;
	pixmap->drawable.y = 0;
	pixmap->drawable.width = width;
	pixmap->drawable.height = height;
	pixmap->devKind = stride;
	pixmap->refcnt = 1;
	pixmap->devPrivate.ptr = datasize ? (char *)pixmap + base : NULL;

#ifdef COMPOSITE
	pixmap->screen_x = 0;
	pixmap->screen_y = 0;
#endif

	pixmap->usage_hint = usage_hint;
#if DEBUG_MEMORY
	sna->debug_memory.pixmap_allocs++;
#endif

	DBG(("%s: serial=%ld, usage=%d, %dx%d\n",
	     __FUNCTION__,
	     pixmap->drawable.serialNumber,
	     pixmap->usage_hint,
	     pixmap->drawable.width,
	     pixmap->drawable.height));

	return pixmap;
}

static PixmapPtr
__pop_freed_pixmap(struct sna *sna)
{
	PixmapPtr pixmap;

	assert(sna->freed_pixmap);

	pixmap = sna->freed_pixmap;
	sna->freed_pixmap = pixmap->devPrivate.ptr;

	assert(pixmap->refcnt == 0);
	assert(sna_pixmap(pixmap));
	assert(sna_pixmap(pixmap)->header);

#if DEBUG_MEMORY
	sna->debug_memory.pixmap_cached--;
#endif

	return pixmap;
}

inline static PixmapPtr
create_pixmap_hdr(struct sna *sna, ScreenPtr screen,
		  int width, int height, int depth, int usage,
		  struct sna_pixmap **priv)
{
	PixmapPtr pixmap;

	if (sna->freed_pixmap == NULL) {
		pixmap = create_pixmap(sna, screen, 0, 0, depth, usage);
		if (pixmap == NullPixmap)
			return NullPixmap;

		*priv = sna_pixmap_attach(pixmap);
		if (!*priv) {
			FreePixmap(pixmap);
			return NullPixmap;
		}
	} else {
		pixmap = __pop_freed_pixmap(sna);
		*priv = _sna_pixmap_reset(pixmap);

		assert(pixmap->drawable.type == DRAWABLE_PIXMAP);
		assert(pixmap->drawable.class == 0);
		assert(pixmap->drawable.pScreen == screen);
		assert(pixmap->drawable.x == 0);
		assert(pixmap->drawable.y == 0);

		pixmap->drawable.id = 0;

		pixmap->drawable.depth = depth;
		pixmap->drawable.bitsPerPixel = bits_per_pixel(depth);
		pixmap->drawable.serialNumber = NEXT_SERIAL_NUMBER;

		pixmap->devKind = 0;
		pixmap->devPrivate.ptr = NULL;

#ifdef COMPOSITE
		pixmap->screen_x = 0;
		pixmap->screen_y = 0;
#endif

#if DEBUG_MEMORY
		sna->debug_memory.pixmap_allocs++;
#endif

		pixmap->refcnt = 1;
	}

	pixmap->drawable.width = width;
	pixmap->drawable.height = height;
	pixmap->usage_hint = usage;

	(*priv)->header = true;
	return pixmap;
}

static PixmapPtr
sna_pixmap_create_shm(ScreenPtr screen,
		      int width, int height, int depth,
		      char *addr)
{
	struct sna *sna = to_sna_from_screen(screen);
	int bpp = bits_per_pixel(depth);
	int pitch = PixmapBytePad(width, depth);
	struct sna_pixmap *priv;
	PixmapPtr pixmap;

	DBG(("%s(%dx%d, depth=%d, bpp=%d, pitch=%d)\n",
	     __FUNCTION__, width, height, depth, bpp, pitch));

	if (wedged(sna) || bpp == 0 || pitch*height < 4096) {
fallback:
		pixmap = sna_pixmap_create_unattached(screen, 0, 0, depth);
		if (pixmap == NULL)
			return NULL;

		if (!screen->ModifyPixmapHeader(pixmap, width, height, depth,
						bpp, pitch, addr)) {
			screen->DestroyPixmap(pixmap);
			return NULL;
		}

		return pixmap;
	}

	pixmap = create_pixmap_hdr(sna, screen, width, height, depth, 0, &priv);
	if (pixmap == NullPixmap) {
		DBG(("%s: allocation failed\n", __FUNCTION__));
		goto fallback;
	}

	priv->cpu_bo = kgem_create_map(&sna->kgem, addr, pitch*height, false);
	if (priv->cpu_bo == NULL) {
		DBG(("%s: mapping SHM segment failed\n", __FUNCTION__));
		sna_pixmap_destroy(pixmap);
		goto fallback;
	}
	priv->cpu_bo->pitch = pitch;
	kgem_bo_mark_unreusable(priv->cpu_bo);
	sna_accel_watch_flush(sna, 1);
#ifdef DEBUG_MEMORY
	sna->debug_memory.cpu_bo_allocs++;
	sna->debug_memory.cpu_bo_bytes += kgem_bo_size(priv->cpu_bo);
#endif

	/* Be wary as we cannot cache SHM Pixmap in our freed cache */
	priv->header = false;
	priv->cpu = true;
	priv->shm = true;
	priv->stride = pitch;
	priv->ptr = MAKE_STATIC_PTR(addr);
	sna_damage_all(&priv->cpu_damage, pixmap);

	pixmap->devKind = pitch;
	pixmap->devPrivate.ptr = addr;

	DBG(("%s: serial=%ld, %dx%d, usage=%d\n",
	     __FUNCTION__,
	     pixmap->drawable.serialNumber,
	     pixmap->drawable.width,
	     pixmap->drawable.height,
	     pixmap->usage_hint));
	return pixmap;
}

PixmapPtr
sna_pixmap_create_unattached(ScreenPtr screen,
			     int width, int height, int depth)
{
	return create_pixmap(to_sna_from_screen(screen),
			     screen, width, height, depth,
			     -1);
}

static PixmapPtr
sna_pixmap_create_scratch(ScreenPtr screen,
			  int width, int height, int depth,
			  uint32_t tiling)
{
	struct sna *sna = to_sna_from_screen(screen);
	struct sna_pixmap *priv;
	PixmapPtr pixmap;
	int bpp;

	DBG(("%s(%d, %d, %d, tiling=%d)\n", __FUNCTION__,
	     width, height, depth, tiling));

	bpp = bits_per_pixel(depth);
	if (tiling == I915_TILING_Y &&
	    (sna->render.prefer_gpu & PREFER_GPU_RENDER) == 0)
		tiling = I915_TILING_X;

	if (tiling == I915_TILING_Y &&
	    (width > sna->render.max_3d_size ||
	     height > sna->render.max_3d_size))
		tiling = I915_TILING_X;

	tiling = kgem_choose_tiling(&sna->kgem, tiling, width, height, bpp);

	/* you promise never to access this via the cpu... */
	pixmap = create_pixmap_hdr(sna, screen, width, height, depth, CREATE_PIXMAP_USAGE_SCRATCH, &priv);
	if (pixmap == NullPixmap)
		return NullPixmap;

	priv->stride = PixmapBytePad(width, depth);

	priv->gpu_bo = kgem_create_2d(&sna->kgem,
				      width, height, bpp, tiling,
				      CREATE_TEMPORARY);
	if (priv->gpu_bo == NULL) {
		free(priv);
		FreePixmap(pixmap);
		return NullPixmap;
	}

	sna_damage_all(&priv->gpu_damage, pixmap);

	assert(to_sna_from_pixmap(pixmap) == sna);
	assert(pixmap->drawable.pScreen == screen);
	assert(pixmap->refcnt == 1);

	DBG(("%s: serial=%ld, %dx%d, usage=%d\n",
	     __FUNCTION__,
	     pixmap->drawable.serialNumber,
	     pixmap->drawable.width,
	     pixmap->drawable.height,
	     pixmap->usage_hint));
	return pixmap;
}

#ifdef CREATE_PIXMAP_USAGE_SHARED
static Bool
sna_share_pixmap_backing(PixmapPtr pixmap, ScreenPtr slave, void **fd_handle)
{
	struct sna *sna = to_sna_from_pixmap(pixmap);
	struct sna_pixmap *priv;
	int fd;

	DBG(("%s: pixmap=%ld\n", __FUNCTION__, pixmap->drawable.serialNumber));

	priv = sna_pixmap_move_to_gpu(pixmap,
				      MOVE_READ | MOVE_WRITE | __MOVE_DRI | __MOVE_FORCE);
	if (priv == NULL)
		return FALSE;

	assert(!priv->shm);
	assert(priv->gpu_bo);
	assert(priv->stride);

	/* XXX negotiate format and stride restrictions */
	if (priv->gpu_bo->tiling != I915_TILING_NONE ||
	    priv->gpu_bo->pitch & 255) {
		struct kgem_bo *bo;
		BoxRec box;

		DBG(("%s: removing tiling %d, and aligning pitch  for %dx%d pixmap=%ld\n",
		     __FUNCTION__, priv->gpu_bo->tiling,
		     pixmap->drawable.width, pixmap->drawable.height,
		     pixmap->drawable.serialNumber));

		if (priv->pinned) {
			DBG(("%s: can't convert pinned bo\n", __FUNCTION__));
			return FALSE;
		}

		assert_pixmap_damage(pixmap);

		bo = kgem_create_2d(&sna->kgem,
				    pixmap->drawable.width,
				    pixmap->drawable.height,
				    pixmap->drawable.bitsPerPixel,
				    I915_TILING_NONE,
				    CREATE_GTT_MAP | CREATE_PRIME);
		if (bo == NULL) {
			DBG(("%s: allocation failed\n", __FUNCTION__));
			return FALSE;
		}

		box.x1 = box.y1 = 0;
		box.x2 = pixmap->drawable.width;
		box.y2 = pixmap->drawable.height;

		assert(!wedged(sna)); /* XXX */
		if (!sna->render.copy_boxes(sna, GXcopy,
					    &pixmap->drawable, priv->gpu_bo, 0, 0,
					    &pixmap->drawable, bo, 0, 0,
					    &box, 1, 0)) {
			DBG(("%s: copy failed\n", __FUNCTION__));
			kgem_bo_destroy(&sna->kgem, bo);
			return FALSE;
		}

		sna_pixmap_unmap(pixmap, priv);
		kgem_bo_destroy(&sna->kgem, priv->gpu_bo);
		priv->gpu_bo = bo;
	}
	assert(priv->gpu_bo->tiling == I915_TILING_NONE);
	assert((priv->gpu_bo->pitch & 255) == 0);

	/* And export the bo->pitch via pixmap->devKind */
	if (!priv->mapped) {
		void *ptr;

		ptr = kgem_bo_map__async(&sna->kgem, priv->gpu_bo);
		if (ptr == NULL)
			return FALSE;

		pixmap->devPrivate.ptr = ptr;
		pixmap->devKind = priv->gpu_bo->pitch;
		priv->mapped = ptr == MAP(priv->gpu_bo->map__cpu) ? MAPPED_CPU : MAPPED_GTT;
	}
	assert_pixmap_map(pixmap, priv);

	fd = kgem_bo_export_to_prime(&sna->kgem, priv->gpu_bo);
	if (fd == -1)
		return FALSE;

	priv->pinned |= PIN_PRIME;

	*fd_handle = (void *)(intptr_t)fd;
	return TRUE;
}

static Bool
sna_set_shared_pixmap_backing(PixmapPtr pixmap, void *fd_handle)
{
	struct sna *sna = to_sna_from_pixmap(pixmap);
	struct sna_pixmap *priv;
	struct kgem_bo *bo;

	DBG(("%s: pixmap=%ld, size=%dx%d, depth=%d/%d, stride=%d\n",
	     __FUNCTION__, pixmap->drawable.serialNumber,
	     pixmap->drawable.width, pixmap->drawable.height,
	     pixmap->drawable.depth, pixmap->drawable.bitsPerPixel,
	     pixmap->devKind));

	priv = sna_pixmap(pixmap);
	if (priv == NULL)
		return FALSE;

	assert(!priv->pinned);
	assert(priv->gpu_bo == NULL);
	assert(priv->cpu_bo == NULL);
	assert(priv->cpu_damage == NULL);
	assert(priv->gpu_damage == NULL);

	bo = kgem_create_for_prime(&sna->kgem,
				   (intptr_t)fd_handle,
				   pixmap->devKind * pixmap->drawable.height);
	if (bo == NULL)
		return FALSE;

	sna_damage_all(&priv->gpu_damage, pixmap);

	bo->pitch = pixmap->devKind;
	priv->stride = pixmap->devKind;

	assert(!priv->mapped);
	priv->gpu_bo = bo;
	priv->pinned |= PIN_PRIME;

	close((intptr_t)fd_handle);
	return TRUE;
}

static PixmapPtr
sna_create_pixmap_shared(struct sna *sna, ScreenPtr screen,
			 int width, int height, int depth)
{
	PixmapPtr pixmap;
	struct sna_pixmap *priv;

	DBG(("%s: depth=%d\n", __FUNCTION__, depth));

	/* Create a stub to be attached later */
	pixmap = create_pixmap_hdr(sna, screen,
				   width, height, depth, 0,
				   &priv);
	if (pixmap == NullPixmap)
		return NullPixmap;

	assert(!priv->mapped);
	priv->stride = 0;
	priv->create = 0;

	if (width|height) {
		priv->gpu_bo = kgem_create_2d(&sna->kgem,
					      width, height,
					      pixmap->drawable.bitsPerPixel,
					      I915_TILING_NONE,
					      CREATE_GTT_MAP | CREATE_PRIME);
		if (priv->gpu_bo == NULL) {
			free(priv);
			FreePixmap(pixmap);
			return NullPixmap;
		}

		/* minimal interface for sharing is linear, 256 byte pitch */
		assert(priv->gpu_bo->tiling == I915_TILING_NONE);
		assert((priv->gpu_bo->pitch & 255) == 0);

		pixmap->devPrivate.ptr =
			kgem_bo_map__async(&sna->kgem, priv->gpu_bo);
		if (pixmap->devPrivate.ptr == NULL) {
			kgem_bo_destroy(&sna->kgem, priv->gpu_bo);
			free(priv);
			FreePixmap(pixmap);
			return FALSE;
		}

		pixmap->devKind = priv->gpu_bo->pitch;

		priv->stride = priv->gpu_bo->pitch;
		priv->mapped = pixmap->devPrivate.ptr == MAP(priv->gpu_bo->map__cpu) ? MAPPED_CPU : MAPPED_GTT;
		assert_pixmap_map(pixmap, priv);

		sna_damage_all(&priv->gpu_damage, pixmap);
	}

	return pixmap;
}
#endif

static PixmapPtr sna_create_pixmap(ScreenPtr screen,
				   int width, int height, int depth,
				   unsigned int usage)
{
	struct sna *sna = to_sna_from_screen(screen);
	PixmapPtr pixmap;
	struct sna_pixmap *priv;
	unsigned flags;
	int pad;
	void *ptr;

	DBG(("%s(%d, %d, %d, usage=%x)\n", __FUNCTION__,
	     width, height, depth, usage));

#ifdef CREATE_PIXMAP_USAGE_SHARED
	if (usage == CREATE_PIXMAP_USAGE_SHARED)
		return sna_create_pixmap_shared(sna, screen,
						width, height, depth);
#endif

	if ((width|height) == 0) {
		usage = -1;
		goto fallback;
	}
	assert(width && height);

	flags = kgem_can_create_2d(&sna->kgem, width, height, depth);
	if (flags == 0) {
		DBG(("%s: can not use GPU, just creating shadow\n",
		     __FUNCTION__));
		goto fallback;
	}

	if (unlikely((sna->render.prefer_gpu & PREFER_GPU_RENDER) == 0))
		flags &= ~KGEM_CAN_CREATE_GPU;
	if (wedged(sna))
		flags &= ~KGEM_CAN_CREATE_GTT;

	DBG(("%s: usage=%d, flags=%x\n", __FUNCTION__, usage, flags));
	switch (usage) {
	case CREATE_PIXMAP_USAGE_SCRATCH:
		if (flags & KGEM_CAN_CREATE_GPU)
			return sna_pixmap_create_scratch(screen,
							 width, height, depth,
							 I915_TILING_X);
		else
			goto fallback;

	case SNA_CREATE_SCRATCH:
		if (flags & (KGEM_CAN_CREATE_CPU | KGEM_CAN_CREATE_GPU))
			return sna_pixmap_create_scratch(screen,
							 width, height, depth,
							 I915_TILING_Y);
		else
			return NullPixmap;
	}

	if (usage == CREATE_PIXMAP_USAGE_GLYPH_PICTURE)
		flags &= ~KGEM_CAN_CREATE_GPU;
	if (usage == CREATE_PIXMAP_USAGE_BACKING_PIXMAP)
		usage = 0;

	pad = PixmapBytePad(width, depth);
	if (pad * height < 4096) {
		DBG(("%s: small buffer [%d], attaching to shadow pixmap\n",
		     __FUNCTION__, pad * height));
		pixmap = create_pixmap(sna, screen,
				       width, height, depth, usage);
		if (pixmap == NullPixmap)
			return NullPixmap;

		ptr = MAKE_STATIC_PTR(pixmap->devPrivate.ptr);
		pad = pixmap->devKind;
		flags &= ~(KGEM_CAN_CREATE_GPU | KGEM_CAN_CREATE_CPU);

		priv = sna_pixmap_attach(pixmap);
		if (priv == NULL) {
			free(pixmap);
			goto fallback;
		}
	} else {
		DBG(("%s: creating GPU pixmap %dx%d, stride=%d, flags=%x\n",
		     __FUNCTION__, width, height, pad, flags));

		pixmap = create_pixmap_hdr(sna, screen, width, height, depth, usage, &priv);
		if (pixmap == NullPixmap)
			return NullPixmap;

		ptr = NULL;
	}

	priv->stride = pad;
	priv->create = flags;
	priv->ptr = ptr;

	assert(to_sna_from_pixmap(pixmap) == sna);
	assert(pixmap->drawable.pScreen == screen);
	assert(pixmap->refcnt == 1);

	DBG(("%s: serial=%ld, %dx%d, usage=%d\n",
	     __FUNCTION__,
	     pixmap->drawable.serialNumber,
	     pixmap->drawable.width,
	     pixmap->drawable.height,
	     pixmap->usage_hint));
	return pixmap;

fallback:
	return create_pixmap(sna, screen, width, height, depth, usage);
}

void sna_add_flush_pixmap(struct sna *sna,
			  struct sna_pixmap *priv,
			  struct kgem_bo *bo)
{
	DBG(("%s: marking pixmap=%ld for flushing\n",
	     __FUNCTION__, priv->pixmap->drawable.serialNumber));
	assert(bo);
	assert(bo->flush);
	assert(priv->gpu_damage == NULL || priv->gpu_bo);
	list_move(&priv->flush_list, &sna->flush_pixmaps);

	if (bo->exec == NULL && kgem_is_idle(&sna->kgem)) {
		DBG(("%s: new flush bo, flushin before\n", __FUNCTION__));
		kgem_submit(&sna->kgem);
	}
}

static void __sna_free_pixmap(struct sna *sna,
			      PixmapPtr pixmap,
			      struct sna_pixmap *priv)
{
	DBG(("%s(pixmap=%ld)\n", __FUNCTION__, pixmap->drawable.serialNumber));
	list_del(&priv->flush_list);

	assert(priv->gpu_damage == NULL);
	assert(priv->cpu_damage == NULL);

	__sna_pixmap_free_cpu(sna, priv);

	if (priv->header) {
		assert(pixmap->drawable.pScreen == sna->scrn->pScreen);
		assert(!priv->shm);
		pixmap->devPrivate.ptr = sna->freed_pixmap;
		sna->freed_pixmap = pixmap;
#if DEBUG_MEMORY
		sna->debug_memory.pixmap_cached++;
#endif
	} else {
		free(priv);
		FreePixmap(pixmap);
	}
}

static Bool sna_destroy_pixmap(PixmapPtr pixmap)
{
	struct sna *sna;
	struct sna_pixmap *priv;

	assert(pixmap->refcnt > 0);
	if (--pixmap->refcnt)
		return TRUE;

#if DEBUG_MEMORY
	to_sna_from_pixmap(pixmap)->debug_memory.pixmap_allocs--;
#endif

	priv = sna_pixmap(pixmap);
	DBG(("%s: pixmap=%ld, attached?=%d\n",
	     __FUNCTION__, pixmap->drawable.serialNumber, priv != NULL));
	if (priv == NULL) {
		FreePixmap(pixmap);
		return TRUE;
	}

	assert_pixmap_damage(pixmap);
	sna = to_sna_from_pixmap(pixmap);

	sna_damage_destroy(&priv->gpu_damage);
	sna_damage_destroy(&priv->cpu_damage);

	list_del(&priv->cow_list);
	if (priv->cow) {
		struct sna_cow *cow = COW(priv->cow);
		DBG(("%s: pixmap=%ld discarding cow, refcnt=%d\n",
		     __FUNCTION__, pixmap->drawable.serialNumber, cow->refcnt));
		assert(cow->refcnt);
		if (!--cow->refcnt)
			free(cow);
		priv->cow = NULL;
	} else
		kgem_bo_pair_undo(&sna->kgem, priv->gpu_bo, priv->cpu_bo);

	if (priv->move_to_gpu)
		(void)priv->move_to_gpu(sna, priv, 0);

	/* Always release the gpu bo back to the lower levels of caching */
	if (priv->gpu_bo) {
		sna_pixmap_unmap(pixmap, priv);
		kgem_bo_destroy(&sna->kgem, priv->gpu_bo);
		priv->gpu_bo = NULL;
	}

	if (priv->shm && kgem_bo_is_busy(priv->cpu_bo)) {
		DBG(("%s: deferring release of active SHM pixmap=%ld\n",
		     __FUNCTION__, pixmap->drawable.serialNumber));
		sna_add_flush_pixmap(sna, priv, priv->cpu_bo);
		kgem_bo_submit(&sna->kgem, priv->cpu_bo); /* XXX ShmDetach */
	} else
		__sna_free_pixmap(sna, pixmap, priv);
	return TRUE;
}

void sna_pixmap_destroy(PixmapPtr pixmap)
{
	assert(pixmap->refcnt == 1);
	assert(sna_pixmap(pixmap) == NULL || sna_pixmap(pixmap)->header == true);

	sna_destroy_pixmap(pixmap);
}

static inline bool has_coherent_map(struct sna *sna,
				    struct kgem_bo *bo,
				    unsigned flags)
{
	assert(bo);

	if (kgem_bo_mapped(&sna->kgem, bo))
		return true;

	if (bo->tiling == I915_TILING_Y)
		return false;

	return kgem_bo_can_map__cpu(&sna->kgem, bo, flags & MOVE_WRITE);
}

static inline bool has_coherent_ptr(struct sna *sna, struct sna_pixmap *priv, unsigned flags)
{
	if (priv == NULL)
		return true;

	if (flags & MOVE_ASYNC_HINT) {
		/* Not referencing the pointer itself, so do not care */
		return true;
	}

	if (!priv->mapped) {
		if (!priv->cpu_bo)
			return true;

		assert(!priv->cpu_bo->needs_flush);
		assert(priv->pixmap->devKind == priv->cpu_bo->pitch);
		return priv->pixmap->devPrivate.ptr == MAP(priv->cpu_bo->map__cpu);
	}

	assert(!priv->move_to_gpu || (flags & MOVE_WRITE) == 0);

	assert_pixmap_map(priv->pixmap, priv);
	assert(priv->pixmap->devKind == priv->gpu_bo->pitch);

	if (priv->pixmap->devPrivate.ptr == MAP(priv->gpu_bo->map__cpu)) {
		assert(priv->mapped == MAPPED_CPU);

		if (priv->gpu_bo->tiling != I915_TILING_NONE)
			return false;

		return flags & MOVE_READ || kgem_bo_can_map__cpu(&sna->kgem, priv->gpu_bo, flags & MOVE_WRITE);
	}

	if (priv->pixmap->devPrivate.ptr == MAP(priv->gpu_bo->map__gtt)) {
		assert(priv->mapped == MAPPED_GTT);

		if (priv->gpu_bo->tiling == I915_TILING_Y && sna->kgem.gen == 0x21)
			return false;

		return true;
	}

	return false;
}

static inline bool pixmap_inplace(struct sna *sna,
				  PixmapPtr pixmap,
				  struct sna_pixmap *priv,
				  unsigned flags)
{
	if (FORCE_INPLACE)
		return FORCE_INPLACE > 0;

	if (wedged(sna) && !priv->pinned) {
		DBG(("%s: no, wedged and unpinned; pull pixmap back to CPU\n", __FUNCTION__));
		return false;
	}

	if (priv->move_to_gpu && flags & MOVE_WRITE)
		return false;

	if (priv->gpu_bo && kgem_bo_is_busy(priv->gpu_bo)) {
		if ((flags & (MOVE_WRITE | MOVE_READ)) == (MOVE_WRITE | MOVE_READ)) {
			DBG(("%s: no, GPU bo is busy\n", __FUNCTION__));
			return false;
		}

		if ((flags & MOVE_READ) == 0) {
			DBG(("%s: %s, GPU bo is busy, but not reading\n", __FUNCTION__, priv->pinned ? "no" : "yes"));
			return !priv->pinned;
		}
	}

	if (priv->mapped) {
		DBG(("%s: %s, already mapped\n", __FUNCTION__, has_coherent_map(sna, priv->gpu_bo, flags) ? "yes" : "no"));
		return has_coherent_map(sna, priv->gpu_bo, flags);
	}

	if (priv->cpu_bo && kgem_bo_is_busy(priv->cpu_bo)) {
		DBG(("%s: yes, has CPU bo and is active on CPU\n", __FUNCTION__));
		return true;
	}

	if (priv->cpu_bo && priv->cpu) {
		DBG(("%s: no, has CPU bo and was last active on CPU, presume future CPU activity\n", __FUNCTION__));
		return false;
	}

	if (flags & MOVE_READ &&
	    (priv->cpu || priv->cpu_damage || priv->gpu_damage == NULL)) {
		DBG(("%s:, no, reading and has CPU damage\n", __FUNCTION__));
		return false;
	}

	return (priv->stride * pixmap->drawable.height >> 12) >
		sna->kgem.half_cpu_cache_pages;
}

static bool sna_pixmap_alloc_gpu(struct sna *sna,
				 PixmapPtr pixmap,
				 struct sna_pixmap *priv,
				 unsigned flags)
{
	uint32_t tiling;

	/* Use tiling by default, but disable per user request */
	if (pixmap->usage_hint == SNA_CREATE_FB && (sna->flags & SNA_LINEAR_FB) == 0) {
		flags |= CREATE_SCANOUT;
		tiling = -I915_TILING_X;
	} else
		tiling = sna_pixmap_default_tiling(sna, pixmap);

	DBG(("%s: pixmap=%ld\n", __FUNCTION__, pixmap->drawable.serialNumber));

	priv->gpu_bo = kgem_create_2d(&sna->kgem,
				      pixmap->drawable.width,
				      pixmap->drawable.height,
				      pixmap->drawable.bitsPerPixel,
				      tiling, flags);
	return priv->gpu_bo != NULL;
}

static bool
sna_pixmap_create_mappable_gpu(PixmapPtr pixmap,
			       bool can_replace)
{
	struct sna *sna = to_sna_from_pixmap(pixmap);
	struct sna_pixmap *priv = sna_pixmap(pixmap);

	if (wedged(sna))
		goto out;

	if ((priv->create & KGEM_CAN_CREATE_GTT) == 0)
		goto out;

	assert_pixmap_damage(pixmap);

	if (can_replace && priv->gpu_bo &&
	    (!kgem_bo_can_map(&sna->kgem, priv->gpu_bo) ||
	     __kgem_bo_is_busy(&sna->kgem, priv->gpu_bo))) {
		if (priv->pinned)
			return false;

		DBG(("%s: discard busy GPU bo\n", __FUNCTION__));
		sna_pixmap_free_gpu(sna, priv);
	}

	if (priv->gpu_bo == NULL) {
		assert_pixmap_damage(pixmap);
		assert(priv->gpu_damage == NULL);
		sna_pixmap_alloc_gpu(sna, pixmap, priv, CREATE_GTT_MAP | CREATE_INACTIVE);
	}

out:
	if (priv->gpu_bo == NULL)
		return false;

	return (kgem_bo_can_map(&sna->kgem, priv->gpu_bo) &&
		!kgem_bo_is_busy(priv->gpu_bo));
}

static inline bool gpu_bo_download(struct sna *sna,
				   struct sna_pixmap *priv,
				   int n, const BoxRec *box,
				   bool idle)
{
	char *src;

	if (!USE_INPLACE)
		return false;

	switch (priv->gpu_bo->tiling) {
	case I915_TILING_Y:
		return false;
	case I915_TILING_X:
		if (!sna->kgem.memcpy_from_tiled_x)
			return false;
	default:
		break;
	}

	if (!kgem_bo_can_map__cpu(&sna->kgem, priv->gpu_bo, FORCE_FULL_SYNC))
		return false;

	if (idle) {
		if (__kgem_bo_is_busy(&sna->kgem, priv->gpu_bo))
			return false;

		if (priv->cpu_bo && __kgem_bo_is_busy(&sna->kgem, priv->cpu_bo))
			return false;
	}

	src = kgem_bo_map__cpu(&sna->kgem, priv->gpu_bo);
	if (src == NULL)
		return false;

	kgem_bo_sync__cpu_full(&sna->kgem, priv->gpu_bo, FORCE_FULL_SYNC);

	if (priv->cpu_bo)
		kgem_bo_sync__cpu(&sna->kgem, priv->cpu_bo);
	assert(has_coherent_ptr(sna, priv, MOVE_WRITE));

	if (sigtrap_get())
		return false;

	if (priv->gpu_bo->tiling) {
		int bpp = priv->pixmap->drawable.bitsPerPixel;
		void *dst = priv->pixmap->devPrivate.ptr;
		int dst_pitch = priv->pixmap->devKind;

		DBG(("%s: download through a tiled CPU map\n", __FUNCTION__));
		do {
			DBG(("%s: box (%d, %d), (%d, %d)\n",
			     __FUNCTION__, box->x1, box->y1, box->x2, box->y2));
			memcpy_from_tiled_x(&sna->kgem, src, dst, bpp,
					    priv->gpu_bo->pitch, dst_pitch,
					    box->x1, box->y1,
					    box->x1, box->y1,
					    box->x2 - box->x1, box->y2 - box->y1);
			box++;
		} while (--n);
	} else {
		int bpp = priv->pixmap->drawable.bitsPerPixel;
		void *dst = priv->pixmap->devPrivate.ptr;
		int dst_pitch = priv->pixmap->devKind;

		DBG(("%s: download through a linear CPU map\n", __FUNCTION__));
		do {
			DBG(("%s: box (%d, %d), (%d, %d)\n",
			     __FUNCTION__, box->x1, box->y1, box->x2, box->y2));
			memcpy_blt(src, dst, bpp,
				   priv->gpu_bo->pitch, dst_pitch,
				   box->x1, box->y1,
				   box->x1, box->y1,
				   box->x2 - box->x1, box->y2 - box->y1);
			box++;
		} while (--n);
	}

	sigtrap_put();
	return true;
}

static inline bool cpu_bo_download(struct sna *sna,
				   struct sna_pixmap *priv,
				   int n, const BoxRec *box)
{
	if (DBG_NO_CPU_DOWNLOAD)
		return false;

	if (wedged(sna))
		return false;

	if (priv->cpu_bo == NULL || !sna->kgem.can_blt_cpu)
		return false;

	if (!kgem_bo_is_busy(priv->gpu_bo) && !kgem_bo_is_busy(priv->cpu_bo)) {
		/* Is it worth detiling? */
		assert(box[0].y1 < box[n-1].y2);
		if (kgem_bo_can_map(&sna->kgem, priv->gpu_bo) &&
		    (box[n-1].y2 - box[0].y1 - 1) * priv->gpu_bo->pitch < 4096) {
			DBG(("%s: no, tiny transfer (height=%d, pitch=%d) expect to read inplace\n",
			     __FUNCTION__, box[n-1].y2-box[0].y1, priv->gpu_bo->pitch));
			return false;
		}
	}

	DBG(("%s: using GPU write to CPU bo for download from GPU\n", __FUNCTION__));
	return sna->render.copy_boxes(sna, GXcopy,
				      &priv->pixmap->drawable, priv->gpu_bo, 0, 0,
				      &priv->pixmap->drawable, priv->cpu_bo, 0, 0,
				      box, n, COPY_LAST);
}

static void download_boxes(struct sna *sna,
			   struct sna_pixmap *priv,
			   int n, const BoxRec *box)
{
	bool ok;

	DBG(("%s: nbox=%d\n", __FUNCTION__, n));

	ok = gpu_bo_download(sna, priv, n, box, true);
	if (!ok)
		ok = cpu_bo_download(sna, priv, n, box);
	if (!ok)
		ok = gpu_bo_download(sna, priv, n, box, false);
	if (!ok) {
		if (priv->cpu_bo)
			kgem_bo_sync__cpu(&sna->kgem, priv->cpu_bo);
		assert(priv->mapped == MAPPED_NONE);
		assert(has_coherent_ptr(sna, priv, MOVE_WRITE));
		sna_read_boxes(sna, priv->pixmap, priv->gpu_bo, box, n);
	}
}

static inline bool use_cpu_bo_for_upload(struct sna *sna,
					 struct sna_pixmap *priv,
					 unsigned flags)
{
	if (DBG_NO_CPU_UPLOAD)
		return false;

	if (wedged(sna))
		return false;

	if (priv->cpu_bo == NULL)
		return false;

	DBG(("%s? flags=%x, gpu busy?=%d, cpu busy?=%d\n", __FUNCTION__,
	     flags,
	     kgem_bo_is_busy(priv->gpu_bo),
	     kgem_bo_is_busy(priv->cpu_bo)));

	if (!priv->cpu)
		return true;

	if (flags & (MOVE_WRITE | MOVE_ASYNC_HINT))
		return true;

	if (priv->gpu_bo->tiling)
		return true;

	return kgem_bo_is_busy(priv->gpu_bo) || kgem_bo_is_busy(priv->cpu_bo);
}

bool
sna_pixmap_undo_cow(struct sna *sna, struct sna_pixmap *priv, unsigned flags)
{
	struct sna_cow *cow = COW(priv->cow);

	DBG(("%s: pixmap=%ld, handle=%d [refcnt=%d], cow refcnt=%d, flags=%x\n",
	     __FUNCTION__,
	     priv->pixmap->drawable.serialNumber,
	     priv->gpu_bo->handle,
	     priv->gpu_bo->refcnt,
	     cow->refcnt,
	     flags));

	assert(priv->gpu_bo == cow->bo);
	assert(cow->refcnt);

	if (flags && (flags & MOVE_WRITE) == 0 && IS_COW_OWNER(priv->cow))
		return true;

	if (!IS_COW_OWNER(priv->cow))
		list_del(&priv->cow_list);

	if (!--cow->refcnt) {
		DBG(("%s: freeing cow\n", __FUNCTION__));
		assert(list_is_empty(&cow->list));
		free(cow);
	} else if (IS_COW_OWNER(priv->cow) && priv->pinned) {
		PixmapPtr pixmap = priv->pixmap;
		struct kgem_bo *bo;
		BoxRec box;

		DBG(("%s: copying the Holy cow\n", __FUNCTION__));

		box.x1 = box.y1 = 0;
		box.x2 = pixmap->drawable.width;
		box.y2 = pixmap->drawable.height;

		bo = kgem_create_2d(&sna->kgem,
				    box.x2, box.y2,
				    pixmap->drawable.bitsPerPixel,
				    sna_pixmap_default_tiling(sna, pixmap),
				    0);
		if (bo == NULL) {
			cow->refcnt++;
			DBG(("%s: allocation failed\n", __FUNCTION__));
			return false;
		}

		if (!sna->render.copy_boxes(sna, GXcopy,
					    &pixmap->drawable, priv->gpu_bo, 0, 0,
					    &pixmap->drawable, bo, 0, 0,
					    &box, 1, 0)) {
			DBG(("%s: copy failed\n", __FUNCTION__));
			kgem_bo_destroy(&sna->kgem, bo);
			cow->refcnt++;
			return false;
		}

		assert(!list_is_empty(&cow->list));
		while (!list_is_empty(&cow->list)) {
			struct sna_pixmap *clone;

			clone = list_first_entry(&cow->list,
						 struct sna_pixmap, cow_list);
			list_del(&clone->cow_list);

			assert(clone->gpu_bo == cow->bo);
			sna_pixmap_unmap(clone->pixmap, clone);
			kgem_bo_destroy(&sna->kgem, clone->gpu_bo);
			clone->gpu_bo = kgem_bo_reference(bo);
		}
		cow->bo = bo;
		kgem_bo_destroy(&sna->kgem, bo);
	} else {
		struct kgem_bo *bo = NULL;

		if (flags & MOVE_READ) {
			PixmapPtr pixmap = priv->pixmap;
			BoxRec box;

			DBG(("%s: copying cow\n", __FUNCTION__));

			box.x1 = box.y1 = 0;
			box.x2 = pixmap->drawable.width;
			box.y2 = pixmap->drawable.height;

			bo = kgem_create_2d(&sna->kgem,
					    box.x2, box.y2,
					    pixmap->drawable.bitsPerPixel,
					    sna_pixmap_default_tiling(sna, pixmap),
					    0);
			if (bo == NULL) {
				cow->refcnt++;
				DBG(("%s: allocation failed\n", __FUNCTION__));
				return false;
			}

			if (!sna->render.copy_boxes(sna, GXcopy,
						    &pixmap->drawable, priv->gpu_bo, 0, 0,
						    &pixmap->drawable, bo, 0, 0,
						    &box, 1, 0)) {
				DBG(("%s: copy failed\n", __FUNCTION__));
				kgem_bo_destroy(&sna->kgem, bo);
				cow->refcnt++;
				return false;
			}
		}

		assert(priv->gpu_bo);
		sna_pixmap_unmap(priv->pixmap, priv);
		kgem_bo_destroy(&sna->kgem, priv->gpu_bo);
		priv->gpu_bo = bo;
	}

	priv->cow = NULL;
	return true;
}

static bool
sna_pixmap_make_cow(struct sna *sna,
		    struct sna_pixmap *src_priv,
		    struct sna_pixmap *dst_priv)
{
	struct sna_cow *cow;

	assert(src_priv->gpu_bo);

	if (!USE_COW)
		return false;

	if (src_priv->gpu_bo->proxy)
		return false;

	DBG(("%s: make cow src=%ld, dst=%ld, handle=%d (already cow? src=%d, dst=%d)\n",
	     __FUNCTION__,
	     src_priv->pixmap->drawable.serialNumber,
	     dst_priv->pixmap->drawable.serialNumber,
	     src_priv->gpu_bo->handle,
	     src_priv->cow ? IS_COW_OWNER(src_priv->cow) ? 1 : -1 : 0,
	     dst_priv->cow ? IS_COW_OWNER(dst_priv->cow) ? 1 : -1 : 0));

	if (dst_priv->pinned) {
		DBG(("%s: can't cow, dst_pinned=%x\n",
		     __FUNCTION__, dst_priv->pinned));
		return false;
	}

	assert(dst_priv->move_to_gpu == NULL);
	assert(!dst_priv->flush);
	assert(list_is_empty(&dst_priv->cow_list));

	cow = COW(src_priv->cow);
	if (cow == NULL) {
		cow = malloc(sizeof(*cow));
		if (cow == NULL)
			return false;

		list_init(&cow->list);

		cow->bo = src_priv->gpu_bo;
		cow->refcnt = 1;

		DBG(("%s: moo! attaching source cow to pixmap=%ld, handle=%d\n",
		     __FUNCTION__,
		     src_priv->pixmap->drawable.serialNumber,
		     cow->bo->handle));

		src_priv->cow = MAKE_COW_OWNER(cow);
	}

	if (cow == COW(dst_priv->cow)) {
		assert(dst_priv->gpu_bo == cow->bo);
		return true;
	}

	if (dst_priv->cow)
		sna_pixmap_undo_cow(sna, dst_priv, 0);

	if (dst_priv->gpu_bo) {
		sna_pixmap_unmap(dst_priv->pixmap, dst_priv);
		kgem_bo_destroy(&sna->kgem, dst_priv->gpu_bo);
	}
	assert(!dst_priv->mapped);
	dst_priv->gpu_bo = kgem_bo_reference(cow->bo);
	dst_priv->cow = cow;
	list_add(&dst_priv->cow_list, &cow->list);
	cow->refcnt++;

	DBG(("%s: moo! attaching clone to pixmap=%ld (source=%ld, handle=%d)\n",
	     __FUNCTION__,
	     dst_priv->pixmap->drawable.serialNumber,
	     src_priv->pixmap->drawable.serialNumber,
	     cow->bo->handle));

	return true;
}

static inline bool operate_inplace(struct sna_pixmap *priv, unsigned flags)
{
	if (!USE_INPLACE)
		return false;

	if ((flags & MOVE_INPLACE_HINT) == 0) {
		DBG(("%s: no, inplace operation not suitable\n", __FUNCTION__));
		return false;
	}

	assert((flags & MOVE_ASYNC_HINT) == 0 || (priv->create & KGEM_CAN_CREATE_LARGE));

	if (priv->move_to_gpu && flags & MOVE_WRITE) {
		DBG(("%s: no, has pending move-to-gpu\n", __FUNCTION__));
		return false;
	}

	if (priv->cow && flags & MOVE_WRITE) {
		DBG(("%s: no, has COW\n", __FUNCTION__));
		return false;
	}

	if ((priv->create & KGEM_CAN_CREATE_GTT) == 0) {
		DBG(("%s: no, not accessible via GTT\n", __FUNCTION__));
		return false;
	}

	if ((priv->gpu_damage == NULL || priv->cpu_damage) && flags & MOVE_READ) {
		DBG(("%s: no, has CPU damage and requires readback\n", __FUNCTION__));
		return false;
	}

	if (priv->cpu_bo && kgem_bo_is_busy(priv->cpu_bo)) {
		DBG(("%s: yes, CPU is busy\n", __FUNCTION__));
		return true;
	}

	if (priv->create & KGEM_CAN_CREATE_LARGE) {
		DBG(("%s: large object, has GPU? %d\n",
		     __FUNCTION__, priv->gpu_bo ? priv->gpu_bo->handle : 0));
		return priv->gpu_bo != NULL;
	}

	if (flags & MOVE_WRITE && priv->gpu_bo&&kgem_bo_is_busy(priv->gpu_bo)) {
		DBG(("%s: no, GPU is busy, so stage write\n", __FUNCTION__));
		return false;
	}

	return true;
}

bool
_sna_pixmap_move_to_cpu(PixmapPtr pixmap, unsigned int flags)
{
	struct sna *sna = to_sna_from_pixmap(pixmap);
	struct sna_pixmap *priv;

	DBG(("%s(pixmap=%ld, %dx%d, flags=%x)\n", __FUNCTION__,
	     pixmap->drawable.serialNumber,
	     pixmap->drawable.width,
	     pixmap->drawable.height,
	     flags));

	assert(flags & (MOVE_READ | MOVE_WRITE));
	assert_pixmap_damage(pixmap);

	priv = sna_pixmap(pixmap);
	if (priv == NULL) {
		DBG(("%s: not attached\n", __FUNCTION__));
		return true;
	}

	DBG(("%s: gpu_bo=%d, gpu_damage=%p, cpu_damage=%p, is-clear?=%d\n",
	     __FUNCTION__,
	     priv->gpu_bo ? priv->gpu_bo->handle : 0,
	     priv->gpu_damage, priv->cpu_damage, priv->clear));

	assert(priv->gpu_damage == NULL || priv->gpu_bo);

	if ((flags & MOVE_READ) == 0 && UNDO) {
		kgem_bo_pair_undo(&sna->kgem, priv->gpu_bo, priv->cpu_bo);
		if (priv->move_to_gpu)
			sna_pixmap_discard_shadow_damage(priv, NULL);
	}

	if (kgem_bo_discard_cache(priv->gpu_bo, flags & MOVE_WRITE)) {
		assert(DAMAGE_IS_ALL(priv->cpu_damage));
		if (DAMAGE_IS_ALL(priv->gpu_damage)) {
			DBG(("%s: using magical upload buffer\n", __FUNCTION__));
			goto skip;
		}

		DBG(("%s: discarding cached upload buffer\n", __FUNCTION__));
		assert(priv->gpu_damage == NULL);
		assert(!priv->pinned);
		assert(!priv->mapped);
		kgem_bo_destroy(&sna->kgem, priv->gpu_bo);
		priv->gpu_bo = NULL;
	}

	if (DAMAGE_IS_ALL(priv->cpu_damage)) {
		DBG(("%s: CPU all-damaged\n", __FUNCTION__));
		assert(priv->gpu_damage == NULL || DAMAGE_IS_ALL(priv->gpu_damage));
		assert(priv->gpu_damage == NULL || (flags & MOVE_WRITE) == 0);
		goto done;
	}

	if (USE_INPLACE && (flags & MOVE_READ) == 0 && !(priv->cow || priv->move_to_gpu)) {
		assert(flags & MOVE_WRITE);
		DBG(("%s: no readback, discarding gpu damage [%d], pending clear[%d]\n",
		     __FUNCTION__, priv->gpu_damage != NULL, priv->clear));

		if ((priv->gpu_bo || priv->create & KGEM_CAN_CREATE_GPU) &&
		    pixmap_inplace(sna, pixmap, priv, flags) &&
		    sna_pixmap_create_mappable_gpu(pixmap, true)) {
			void *ptr;

			DBG(("%s: write inplace\n", __FUNCTION__));
			assert(!priv->shm);
			assert(priv->cow == NULL);
			assert(priv->move_to_gpu == NULL);
			assert(priv->gpu_bo->exec == NULL);
			assert((flags & MOVE_READ) == 0 || priv->cpu_damage == NULL);

			ptr = kgem_bo_map(&sna->kgem, priv->gpu_bo);
			if (ptr == NULL)
				goto skip_inplace_map;

			pixmap->devPrivate.ptr = ptr;
			pixmap->devKind = priv->gpu_bo->pitch;
			priv->mapped = ptr == MAP(priv->gpu_bo->map__cpu) ? MAPPED_CPU : MAPPED_GTT;
			assert(has_coherent_ptr(sna, priv, flags));

			assert(priv->gpu_bo->proxy == NULL);
			sna_damage_all(&priv->gpu_damage, pixmap);
			sna_damage_destroy(&priv->cpu_damage);
			priv->clear = false;
			list_del(&priv->flush_list);

			assert(!priv->shm);
			assert(priv->cpu_bo == NULL || !priv->cpu_bo->flush);
			sna_pixmap_free_cpu(sna, priv, priv->cpu);
			priv->cpu &= priv->mapped == MAPPED_CPU;

			assert_pixmap_damage(pixmap);
			return true;
		}

skip_inplace_map:
		sna_damage_destroy(&priv->gpu_damage);
		priv->clear = false;
		if ((flags & MOVE_ASYNC_HINT) == 0 &&
		    priv->cpu_bo && !priv->cpu_bo->flush &&
		    __kgem_bo_is_busy(&sna->kgem, priv->cpu_bo)) {
			DBG(("%s: discarding busy CPU bo\n", __FUNCTION__));
			assert(!priv->shm);
			assert(priv->gpu_bo == NULL || priv->gpu_damage == NULL);

			sna_damage_destroy(&priv->cpu_damage);
			sna_pixmap_free_cpu(sna, priv, false);

			assert(priv->mapped == MAPPED_NONE);
			if (!sna_pixmap_alloc_cpu(sna, pixmap, priv, 0))
				return false;
			assert(priv->mapped == MAPPED_NONE);
			assert(pixmap->devPrivate.ptr == PTR(priv->ptr));

			goto mark_damage;
		}
	}

	assert(priv->gpu_bo == NULL || priv->gpu_bo->proxy == NULL || (flags & MOVE_WRITE) == 0);

	if (operate_inplace(priv, flags) &&
	    pixmap_inplace(sna, pixmap, priv, flags) &&
	    sna_pixmap_create_mappable_gpu(pixmap, (flags & MOVE_READ) == 0)) {
		void *ptr;

		DBG(("%s: try to operate inplace (GTT)\n", __FUNCTION__));
		assert(priv->gpu_bo);
		assert(priv->cow == NULL || (flags & MOVE_WRITE) == 0);
		assert(!priv->move_to_gpu);
		assert(priv->gpu_bo->proxy == NULL || (flags & MOVE_WRITE) == 0);
		assert((flags & MOVE_READ) == 0 || priv->cpu_damage == NULL);
		/* XXX only sync for writes? */
		kgem_bo_submit(&sna->kgem, priv->gpu_bo);
		assert(priv->gpu_bo->exec == NULL);

		ptr = kgem_bo_map(&sna->kgem, priv->gpu_bo);
		if (ptr != NULL) {
			pixmap->devPrivate.ptr = ptr;
			pixmap->devKind = priv->gpu_bo->pitch;
			priv->mapped = ptr == MAP(priv->gpu_bo->map__cpu) ? MAPPED_CPU : MAPPED_GTT;
			assert(has_coherent_ptr(sna, priv, flags));

			if (flags & MOVE_WRITE) {
				assert(priv->gpu_bo->proxy == NULL);
				sna_damage_all(&priv->gpu_damage, pixmap);
				sna_damage_destroy(&priv->cpu_damage);
				sna_pixmap_free_cpu(sna, priv, priv->cpu);
				list_del(&priv->flush_list);
				priv->clear = false;
			}
			priv->cpu &= priv->mapped == MAPPED_CPU;

			assert_pixmap_damage(pixmap);
			DBG(("%s: operate inplace (GTT)\n", __FUNCTION__));
			return true;
		}
	}

	sna_pixmap_unmap(pixmap, priv);

	if (USE_INPLACE &&
	    (flags & MOVE_WRITE ? (void *)priv->gpu_bo : (void *)priv->gpu_damage) && priv->cpu_damage == NULL &&
	    priv->gpu_bo->tiling == I915_TILING_NONE &&
	    (flags & MOVE_READ || kgem_bo_can_map__cpu(&sna->kgem, priv->gpu_bo, flags & MOVE_WRITE)) &&
	    ((flags & (MOVE_WRITE | MOVE_ASYNC_HINT)) == 0 ||
	     (!priv->cow && !priv->move_to_gpu && !__kgem_bo_is_busy(&sna->kgem, priv->gpu_bo)))) {
		void *ptr;

		DBG(("%s: try to operate inplace (CPU)\n", __FUNCTION__));
		assert(priv->gpu_bo);
		assert(priv->cow == NULL || (flags & MOVE_WRITE) == 0);
		assert(priv->move_to_gpu == NULL || (flags & MOVE_WRITE) == 0);
		assert(priv->gpu_bo->proxy == NULL || (flags & MOVE_WRITE) == 0);

		assert(!priv->mapped);
		assert(priv->gpu_bo->tiling == I915_TILING_NONE);

		ptr = kgem_bo_map__cpu(&sna->kgem, priv->gpu_bo);
		if (ptr != NULL) {
			pixmap->devPrivate.ptr = ptr;
			pixmap->devKind = priv->gpu_bo->pitch;
			priv->mapped = MAPPED_CPU;
			assert(has_coherent_ptr(sna, priv, flags));

			if (flags & MOVE_WRITE) {
				assert(priv->gpu_bo->proxy == NULL);
				sna_damage_all(&priv->gpu_damage, pixmap);
				sna_damage_destroy(&priv->cpu_damage);
				sna_pixmap_free_cpu(sna, priv, priv->cpu);
				list_del(&priv->flush_list);
				priv->clear = false;
				priv->cpu = true;
			}

			assert(pixmap->devPrivate.ptr == MAP(priv->gpu_bo->map__cpu));
			kgem_bo_sync__cpu_full(&sna->kgem, priv->gpu_bo,
					       FORCE_FULL_SYNC || flags & MOVE_WRITE);
			assert((flags & MOVE_WRITE) == 0 || !kgem_bo_is_busy(priv->gpu_bo));
			assert_pixmap_damage(pixmap);
			assert(has_coherent_ptr(sna, priv, flags));
			DBG(("%s: operate inplace (CPU)\n", __FUNCTION__));
			return true;
		}
	}

	assert(priv->mapped == MAPPED_NONE);
	if (((flags & MOVE_READ) == 0 || priv->clear) &&
	    priv->cpu_bo && !priv->cpu_bo->flush &&
	    __kgem_bo_is_busy(&sna->kgem, priv->cpu_bo)) {
		assert(!priv->shm);
		sna_pixmap_free_cpu(sna, priv, false);
	}

	assert(priv->mapped == MAPPED_NONE);
	if (pixmap->devPrivate.ptr == NULL &&
	    !sna_pixmap_alloc_cpu(sna, pixmap, priv, flags))
		return false;
	assert(priv->mapped == MAPPED_NONE);
	assert(pixmap->devPrivate.ptr == PTR(priv->ptr));

	if (flags & MOVE_READ) {
		if (priv->clear) {
			DBG(("%s: applying clear [%08x] size=%dx%d, stride=%d (total=%d)\n",
			     __FUNCTION__, priv->clear_color, pixmap->drawable.width, pixmap->drawable.height,
			     pixmap->devKind, pixmap->devKind * pixmap->drawable.height));

			if (priv->cpu_bo) {
				if ((flags & MOVE_ASYNC_HINT || priv->cpu_bo->exec) &&
				    sna->render.fill_one(sna,
							  pixmap, priv->cpu_bo, priv->clear_color,
							  0, 0,
							  pixmap->drawable.width,
							  pixmap->drawable.height,
							  GXcopy))
					goto clear_done;

				DBG(("%s: syncing CPU bo\n", __FUNCTION__));
				kgem_bo_sync__cpu(&sna->kgem, priv->cpu_bo);
				assert(pixmap->devPrivate.ptr == MAP(priv->cpu_bo->map__cpu));
			}

			assert(pixmap->devKind);
			if (priv->clear_color == 0 ||
			    pixmap->drawable.bitsPerPixel == 8 ||
			    priv->clear_color == (1 << pixmap->drawable.depth) - 1) {
				memset(pixmap->devPrivate.ptr, priv->clear_color,
				       (size_t)pixmap->devKind * pixmap->drawable.height);
			} else {
				pixman_fill(pixmap->devPrivate.ptr,
					    pixmap->devKind/sizeof(uint32_t),
					    pixmap->drawable.bitsPerPixel,
					    0, 0,
					    pixmap->drawable.width,
					    pixmap->drawable.height,
					    priv->clear_color);
			}

clear_done:
			sna_damage_all(&priv->cpu_damage, pixmap);
			sna_pixmap_free_gpu(sna, priv);
			assert(priv->gpu_damage == NULL);
			assert(priv->clear == false);
		}

		if (priv->gpu_damage) {
			const BoxRec *box;
			int n;

			DBG(("%s: flushing GPU damage\n", __FUNCTION__));
			assert(priv->gpu_bo);

			n = sna_damage_get_boxes(priv->gpu_damage, &box);
			if (n) {
				if (priv->move_to_gpu && !priv->move_to_gpu(sna, priv, MOVE_READ)) {
					DBG(("%s: move-to-gpu override failed\n", __FUNCTION__));
					return false;
				}

				download_boxes(sna, priv, n, box);
			}

			__sna_damage_destroy(DAMAGE_PTR(priv->gpu_damage));
			priv->gpu_damage = NULL;
		}
	}

	if (flags & MOVE_WRITE || priv->create & KGEM_CAN_CREATE_LARGE) {
mark_damage:
		DBG(("%s: marking as damaged\n", __FUNCTION__));
		sna_damage_all(&priv->cpu_damage, pixmap);
		sna_pixmap_free_gpu(sna, priv);
		assert(priv->gpu_damage == NULL);
		assert(priv->clear == false);

		if (priv->flush) {
			assert(!priv->shm);
			sna_add_flush_pixmap(sna, priv, priv->gpu_bo);
		}
	}

done:
	if (flags & MOVE_WRITE) {
		assert(DAMAGE_IS_ALL(priv->cpu_damage));
		assert(priv->gpu_damage == NULL);
		assert(priv->gpu_bo == NULL || priv->gpu_bo->proxy == NULL);
		if (priv->cow)
			sna_pixmap_undo_cow(sna, priv, 0);
		if (priv->gpu_bo && priv->gpu_bo->rq == NULL) {
			DBG(("%s: discarding idle GPU bo\n", __FUNCTION__));
			sna_pixmap_free_gpu(sna, priv);
		}
		priv->source_count = SOURCE_BIAS;
	}

	if (priv->cpu_bo) {
		if ((flags & MOVE_ASYNC_HINT) == 0) {
			DBG(("%s: syncing CPU bo\n", __FUNCTION__));
			assert(pixmap->devPrivate.ptr == MAP(priv->cpu_bo->map__cpu));
			kgem_bo_sync__cpu_full(&sna->kgem, priv->cpu_bo,
					       FORCE_FULL_SYNC || flags & MOVE_WRITE);
			assert((flags & MOVE_WRITE) == 0 || !kgem_bo_is_busy(priv->cpu_bo));
		}
	}
skip:
	priv->cpu |= (flags & (MOVE_WRITE | MOVE_ASYNC_HINT)) == MOVE_WRITE;
	assert(pixmap->devPrivate.ptr == PTR(priv->ptr));
	assert(pixmap->devKind);
	assert_pixmap_damage(pixmap);
	assert(has_coherent_ptr(sna, sna_pixmap(pixmap), flags));
	return true;
}

static bool
region_overlaps_damage(const RegionRec *region,
		       struct sna_damage *damage,
		       int dx, int dy)
{
	const BoxRec *re, *de;

	DBG(("%s?\n", __FUNCTION__));

	if (damage == NULL)
		return false;

	if (DAMAGE_IS_ALL(damage))
		return true;

	re = &region->extents;
	de = &DAMAGE_PTR(damage)->extents;
	DBG(("%s: region (%d, %d), (%d, %d), damage (%d, %d), (%d, %d)\n",
	     __FUNCTION__,
	     re->x1, re->y1, re->x2, re->y2,
	     de->x1, de->y1, de->x2, de->y2));

	return (re->x1 + dx < de->x2 && re->x2 + dx > de->x1 &&
		re->y1 + dy < de->y2 && re->y2 + dy > de->y1);
}

static inline bool region_inplace(struct sna *sna,
				  PixmapPtr pixmap,
				  RegionPtr region,
				  struct sna_pixmap *priv,
				  unsigned flags)
{
	assert_pixmap_damage(pixmap);

	if (FORCE_INPLACE)
		return FORCE_INPLACE > 0;

	if (wedged(sna) && !priv->pinned)
		return false;

	if (priv->gpu_damage &&
	    (priv->clear || (flags & MOVE_READ) == 0) &&
	    kgem_bo_is_busy(priv->gpu_bo))
		return false;

	if (flags & MOVE_READ &&
	    (priv->cpu ||
	     priv->gpu_damage == NULL ||
	     region_overlaps_damage(region, priv->cpu_damage, 0, 0))) {
		DBG(("%s: no, uncovered CPU damage pending\n", __FUNCTION__));
		return false;
	}

	if (priv->mapped) {
		DBG(("%s: %s, already mapped, continuing\n", __FUNCTION__,
		     has_coherent_map(sna, priv->gpu_bo, flags) ? "yes" : "no"));
		return has_coherent_map(sna, priv->gpu_bo, flags);
	}

	if (priv->flush) {
		DBG(("%s: yes, exported via dri, will flush\n", __FUNCTION__));
		return true;
	}

	if (DAMAGE_IS_ALL(priv->gpu_damage)) {
		DBG(("%s: yes, already wholly damaged on the GPU\n", __FUNCTION__));
		assert(priv->gpu_bo);
		return true;
	}

	if (priv->cpu_bo && priv->cpu) {
		DBG(("%s: no, has CPU bo and was last active on CPU, presume future CPU activity\n", __FUNCTION__));
		return false;
	}

	DBG(("%s: (%dx%d), inplace? %d\n",
	     __FUNCTION__,
	     region->extents.x2 - region->extents.x1,
	     region->extents.y2 - region->extents.y1,
	     ((int)(region->extents.x2 - region->extents.x1) *
	      (int)(region->extents.y2 - region->extents.y1) *
	      pixmap->drawable.bitsPerPixel >> 12)
	     >= sna->kgem.half_cpu_cache_pages));
	return ((int)(region->extents.x2 - region->extents.x1) *
		(int)(region->extents.y2 - region->extents.y1) *
		pixmap->drawable.bitsPerPixel >> 12)
		>= sna->kgem.half_cpu_cache_pages;
}

static bool cpu_clear_boxes(struct sna *sna,
			    PixmapPtr pixmap,
			    struct sna_pixmap *priv,
			    const BoxRec *box, int n)
{
	struct sna_fill_op fill;

	if (!sna_fill_init_blt(&fill, sna,
			       pixmap, priv->cpu_bo,
			       GXcopy, priv->clear_color,
			       FILL_BOXES)) {
		DBG(("%s: unsupported fill\n",
		     __FUNCTION__));
		return false;
	}

	fill.boxes(sna, &fill, box, n);
	fill.done(sna, &fill);
	return true;
}

bool
sna_drawable_move_region_to_cpu(DrawablePtr drawable,
				RegionPtr region,
				unsigned flags)
{
	PixmapPtr pixmap = get_drawable_pixmap(drawable);
	struct sna *sna = to_sna_from_pixmap(pixmap);
	struct sna_pixmap *priv;
	int16_t dx, dy;

	DBG(("%s(pixmap=%ld (%dx%d), [(%d, %d), (%d, %d)], flags=%x)\n",
	     __FUNCTION__, pixmap->drawable.serialNumber,
	     pixmap->drawable.width, pixmap->drawable.height,
	     RegionExtents(region)->x1, RegionExtents(region)->y1,
	     RegionExtents(region)->x2, RegionExtents(region)->y2,
	     flags));

	assert_pixmap_damage(pixmap);
	if (flags & MOVE_WRITE) {
		assert_drawable_contains_box(drawable, &region->extents);
	}
	assert(flags & (MOVE_WRITE | MOVE_READ));

	if (box_empty(&region->extents))
		return true;

	if (MIGRATE_ALL || DBG_NO_PARTIAL_MOVE_TO_CPU) {
		if (!region_subsumes_pixmap(region, pixmap))
			flags |= MOVE_READ;
		return _sna_pixmap_move_to_cpu(pixmap, flags);
	}

	priv = sna_pixmap(pixmap);
	if (priv == NULL) {
		DBG(("%s: not attached to pixmap %ld (depth %d)\n",
		     __FUNCTION__, pixmap->drawable.serialNumber, pixmap->drawable.depth));
		return true;
	}

	assert(priv->gpu_damage == NULL || priv->gpu_bo);

	if (kgem_bo_discard_cache(priv->gpu_bo, flags & MOVE_WRITE)) {
		assert(DAMAGE_IS_ALL(priv->cpu_damage));
		if (DAMAGE_IS_ALL(priv->gpu_damage)) {
			DBG(("%s: using magical upload buffer\n", __FUNCTION__));
			goto skip;
		}

		DBG(("%s: discarding cached upload buffer\n", __FUNCTION__));
		assert(priv->gpu_damage == NULL);
		assert(!priv->pinned);
		assert(!priv->mapped);
		kgem_bo_destroy(&sna->kgem, priv->gpu_bo);
		priv->gpu_bo = NULL;
	}

	if (sna_damage_is_all(&priv->cpu_damage,
			      pixmap->drawable.width,
			      pixmap->drawable.height)) {
		bool discard_gpu = priv->cpu;

		DBG(("%s: pixmap=%ld all damaged on CPU\n",
		     __FUNCTION__, pixmap->drawable.serialNumber));
		assert(!priv->clear);

		sna_damage_destroy(&priv->gpu_damage);

		if ((flags & (MOVE_READ | MOVE_ASYNC_HINT)) == 0 &&
		    priv->cpu_bo && !priv->cpu_bo->flush &&
		    __kgem_bo_is_busy(&sna->kgem, priv->cpu_bo)) {
			DBG(("%s: active CPU bo replacing\n", __FUNCTION__));
			assert(!priv->shm);
			assert(!IS_STATIC_PTR(priv->ptr));

			if (!region_subsumes_pixmap(region, pixmap)) {
				DBG(("%s: partial replacement\n", __FUNCTION__));
				if (get_drawable_deltas(drawable, pixmap, &dx, &dy))
					RegionTranslate(region, dx, dy);

				if (sna->kgem.has_llc && !priv->pinned &&
				    sna_pixmap_default_tiling(sna, pixmap) == I915_TILING_NONE) {
#ifdef DEBUG_MEMORY
					sna->debug_memory.cpu_bo_allocs--;
					sna->debug_memory.cpu_bo_bytes -= kgem_bo_size(priv->cpu_bo);
#endif
					DBG(("%s: promoting CPU bo to GPU bo\n", __FUNCTION__));
					if (priv->gpu_bo)
						sna_pixmap_free_gpu(sna, priv);
					priv->gpu_bo = priv->cpu_bo;
					priv->cpu_bo = NULL;
					priv->ptr = NULL;
					pixmap->devPrivate.ptr = NULL;

					priv->gpu_damage = priv->cpu_damage;
					priv->cpu_damage = NULL;

					sna_damage_subtract(&priv->gpu_damage, region);
					discard_gpu = false;
				} else {
					DBG(("%s: pushing surrounding damage to GPU bo\n", __FUNCTION__));
					sna_damage_subtract(&priv->cpu_damage, region);
					assert(priv->cpu_damage);
					if (sna_pixmap_move_to_gpu(pixmap, MOVE_READ | MOVE_ASYNC_HINT)) {
						sna_pixmap_free_cpu(sna, priv, false);
						if (priv->flush)
							sna_add_flush_pixmap(sna, priv, priv->gpu_bo);

						assert(priv->cpu_damage == NULL);
						sna_damage_all(&priv->gpu_damage, pixmap);
						sna_damage_subtract(&priv->gpu_damage, region);
						discard_gpu = false;
					}
				}
				sna_damage_add(&priv->cpu_damage, region);

				if (dx | dy)
					RegionTranslate(region, -dx, -dy);
			} else
				sna_pixmap_free_cpu(sna, priv, false);
		}

		if (flags & MOVE_WRITE && discard_gpu)
			sna_pixmap_free_gpu(sna, priv);

		sna_pixmap_unmap(pixmap, priv);
		assert(priv->mapped == MAPPED_NONE);
		if (pixmap->devPrivate.ptr == NULL &&
		    !sna_pixmap_alloc_cpu(sna, pixmap, priv, flags))
			return false;
		assert(priv->mapped == MAPPED_NONE);
		assert(pixmap->devPrivate.ptr == PTR(priv->ptr));

		goto out;
	}

	if (USE_INPLACE &&
	    (priv->create & KGEM_CAN_CREATE_LARGE ||
	     ((flags & (MOVE_READ | MOVE_ASYNC_HINT)) == 0 &&
	      (priv->flush ||
	       (flags & MOVE_WHOLE_HINT && whole_pixmap_inplace(pixmap)) ||
	       box_inplace(pixmap, &region->extents))))) {
		DBG(("%s: marking for inplace hint (%d, %d)\n",
		     __FUNCTION__, priv->flush, box_inplace(pixmap, &region->extents)));
		flags |= MOVE_INPLACE_HINT;
	}

	if (region_subsumes_pixmap(region, pixmap)) {
		DBG(("%s: region (%d, %d), (%d, %d) + (%d, %d) subsumes pixmap (%dx%d)\n",
		       __FUNCTION__,
		       region->extents.x1,
		       region->extents.y1,
		       region->extents.x2,
		       region->extents.y2,
		       get_drawable_dx(drawable), get_drawable_dy(drawable),
		       pixmap->drawable.width,
		       pixmap->drawable.height));
		return _sna_pixmap_move_to_cpu(pixmap, flags);
	}

	if (priv->move_to_gpu) {
		DBG(("%s: applying move-to-gpu override\n", __FUNCTION__));
		if ((flags & MOVE_READ) == 0)
			sna_pixmap_discard_shadow_damage(priv, region);
		if (!priv->move_to_gpu(sna, priv, MOVE_READ)) {
			DBG(("%s: move-to-gpu override failed\n", __FUNCTION__));
			return NULL;
		}
	}

	assert(priv->gpu_bo == NULL || priv->gpu_bo->proxy == NULL || (flags & MOVE_WRITE) == 0);

	if (get_drawable_deltas(drawable, pixmap, &dx, &dy)) {
		DBG(("%s: delta=(%d, %d)\n", __FUNCTION__, dx, dy));
		RegionTranslate(region, dx, dy);
	}

	if (operate_inplace(priv, flags) &&
	    region_inplace(sna, pixmap, region, priv, flags) &&
	    sna_pixmap_create_mappable_gpu(pixmap, false)) {
		void *ptr;

		DBG(("%s: try to operate inplace\n", __FUNCTION__));
		assert(priv->gpu_bo);
		assert(priv->cow == NULL || (flags & MOVE_WRITE) == 0);
		assert(priv->move_to_gpu == NULL || (flags & MOVE_WRITE) == 0);
		assert(priv->gpu_bo->proxy == NULL || (flags & MOVE_WRITE) == 0);

		/* XXX only sync for writes? */
		kgem_bo_submit(&sna->kgem, priv->gpu_bo);
		assert(priv->gpu_bo->exec == NULL);

		ptr = kgem_bo_map(&sna->kgem, priv->gpu_bo);
		if (ptr != NULL) {
			pixmap->devPrivate.ptr = ptr;
			pixmap->devKind = priv->gpu_bo->pitch;
			priv->mapped = ptr == MAP(priv->gpu_bo->map__cpu) ? MAPPED_CPU : MAPPED_GTT;
			assert(has_coherent_ptr(sna, priv, flags));

			if (flags & MOVE_WRITE) {
				if (!DAMAGE_IS_ALL(priv->gpu_damage)) {
					assert(!priv->clear);
					sna_damage_add(&priv->gpu_damage, region);
					if (sna_damage_is_all(&priv->gpu_damage,
							      pixmap->drawable.width,
							      pixmap->drawable.height)) {
						DBG(("%s: replaced entire pixmap, destroying CPU shadow\n",
						     __FUNCTION__));
						sna_damage_destroy(&priv->cpu_damage);
						list_del(&priv->flush_list);
					} else
						sna_damage_subtract(&priv->cpu_damage,
								    region);
				}
				priv->clear = false;
			}
			priv->cpu &= priv->mapped == MAPPED_CPU;
			assert_pixmap_damage(pixmap);
			if (dx | dy)
				RegionTranslate(region, -dx, -dy);
			DBG(("%s: operate inplace\n", __FUNCTION__));
			return true;
		}
	}

	if (priv->clear && flags & MOVE_WRITE) {
		DBG(("%s: pending clear, moving whole pixmap for partial write\n", __FUNCTION__));
demote_to_cpu:
		if (dx | dy)
			RegionTranslate(region, -dx, -dy);
		return _sna_pixmap_move_to_cpu(pixmap, flags | MOVE_READ);
	}

	if (flags & MOVE_WHOLE_HINT) {
		DBG(("%s: region (%d, %d), (%d, %d) marked with WHOLE hint, pixmap %dx%d\n",
		       __FUNCTION__,
		       region->extents.x1,
		       region->extents.y1,
		       region->extents.x2,
		       region->extents.y2,
		       pixmap->drawable.width,
		       pixmap->drawable.height));
move_to_cpu:
		if ((flags & MOVE_READ) == 0)
			sna_damage_subtract(&priv->gpu_damage, region);
		goto demote_to_cpu;
	}

	sna_pixmap_unmap(pixmap, priv);

	if (USE_INPLACE &&
	    priv->gpu_damage &&
	    priv->gpu_bo->tiling == I915_TILING_NONE &&
	    ((priv->cow == NULL && priv->move_to_gpu == NULL) || (flags & MOVE_WRITE) == 0) &&
	    (DAMAGE_IS_ALL(priv->gpu_damage) ||
	     sna_damage_contains_box__no_reduce(priv->gpu_damage,
						&region->extents)) &&
	    kgem_bo_can_map__cpu(&sna->kgem, priv->gpu_bo, flags & MOVE_WRITE) &&
	    ((flags & (MOVE_WRITE | MOVE_ASYNC_HINT)) == 0 ||
	     !__kgem_bo_is_busy(&sna->kgem, priv->gpu_bo))) {
		void *ptr;

		DBG(("%s: try to operate inplace (CPU), read? %d, write? %d\n",
		     __FUNCTION__, !!(flags & MOVE_READ), !!(flags & MOVE_WRITE)));
		assert(priv->gpu_bo);
		assert(priv->gpu_bo->proxy == NULL || (flags & MOVE_WRITE) == 0);
		assert(sna_damage_contains_box(&priv->gpu_damage, &region->extents) == PIXMAN_REGION_IN);
		assert(sna_damage_contains_box(&priv->cpu_damage, &region->extents) == PIXMAN_REGION_OUT);

		ptr = kgem_bo_map__cpu(&sna->kgem, priv->gpu_bo);
		if (ptr != NULL) {
			pixmap->devPrivate.ptr = ptr;
			pixmap->devKind = priv->gpu_bo->pitch;
			priv->mapped = MAPPED_CPU;
			assert(has_coherent_ptr(sna, priv, flags));

			if (flags & MOVE_WRITE) {
				if (!DAMAGE_IS_ALL(priv->gpu_damage)) {
					assert(!priv->clear);
					sna_damage_add(&priv->gpu_damage, region);
					if (sna_damage_is_all(&priv->gpu_damage,
							      pixmap->drawable.width,
							      pixmap->drawable.height)) {
						DBG(("%s: replaced entire pixmap, destroying CPU shadow\n",
						     __FUNCTION__));
						sna_damage_destroy(&priv->cpu_damage);
						list_del(&priv->flush_list);
					} else
						sna_damage_subtract(&priv->cpu_damage,
								    region);
				}
				priv->clear = false;
			}
			assert_pixmap_damage(pixmap);

			kgem_bo_sync__cpu_full(&sna->kgem, priv->gpu_bo,
					       FORCE_FULL_SYNC || flags & MOVE_WRITE);
			priv->cpu = true;

			assert_pixmap_map(pixmap, priv);
			assert((flags & MOVE_WRITE) == 0 || !kgem_bo_is_busy(priv->gpu_bo));
			if (dx | dy)
				RegionTranslate(region, -dx, -dy);
			DBG(("%s: operate inplace (CPU)\n", __FUNCTION__));
			return true;
		}
	}

	if ((priv->clear || (flags & MOVE_READ) == 0) &&
	    priv->cpu_bo && !priv->cpu_bo->flush &&
	    __kgem_bo_is_busy(&sna->kgem, priv->cpu_bo)) {
		sna_damage_subtract(&priv->cpu_damage, region);
		if (sna_pixmap_move_to_gpu(pixmap, MOVE_READ | MOVE_ASYNC_HINT)) {
			assert(priv->gpu_bo);
			sna_damage_all(&priv->gpu_damage, pixmap);
			sna_pixmap_free_cpu(sna, priv, false);
		}
	}

	assert(priv->mapped == MAPPED_NONE);
	if (pixmap->devPrivate.ptr == NULL &&
	    !sna_pixmap_alloc_cpu(sna, pixmap, priv, flags)) {
		DBG(("%s: CPU bo allocation failed, trying full move-to-cpu\n", __FUNCTION__));
		goto move_to_cpu;
	}
	assert(priv->mapped == MAPPED_NONE);
	assert(pixmap->devPrivate.ptr == PTR(priv->ptr));

	if (priv->gpu_bo == NULL) {
		assert(priv->gpu_damage == NULL);
		goto done;
	}

	assert(priv->gpu_bo->proxy == NULL);

	if ((flags & MOVE_READ) == 0) {
		assert(flags & MOVE_WRITE);
		sna_damage_subtract(&priv->gpu_damage, region);
		priv->clear = false;
		goto done;
	}

	if (priv->clear) {
		int n = region_num_rects(region);
		const BoxRec *box = region_rects(region);

		assert(DAMAGE_IS_ALL(priv->gpu_damage));
		assert(priv->cpu_damage == NULL);

		DBG(("%s: pending clear, doing partial fill\n", __FUNCTION__));
		if (priv->cpu_bo) {
			if ((flags & MOVE_ASYNC_HINT || priv->cpu_bo->exec) &&
			    cpu_clear_boxes(sna, pixmap, priv, box, n))
				goto clear_done;

			DBG(("%s: syncing CPU bo\n", __FUNCTION__));
			kgem_bo_sync__cpu(&sna->kgem, priv->cpu_bo);
			assert(pixmap->devPrivate.ptr == MAP(priv->cpu_bo->map__cpu));
		}

		assert(pixmap->devKind);
		do {
			pixman_fill(pixmap->devPrivate.ptr,
				    pixmap->devKind/sizeof(uint32_t),
				    pixmap->drawable.bitsPerPixel,
				    box->x1, box->y1,
				    box->x2 - box->x1,
				    box->y2 - box->y1,
				    priv->clear_color);
			box++;
		} while (--n);

clear_done:
		if (flags & MOVE_WRITE ||
		    region->extents.x2 - region->extents.x1 > 1 ||
		    region->extents.y2 - region->extents.y1 > 1) {
			sna_damage_subtract(&priv->gpu_damage, region);
			priv->clear = false;
		}
		goto done;
	}

	if (priv->gpu_damage &&
	    (DAMAGE_IS_ALL(priv->gpu_damage) ||
	     sna_damage_overlaps_box(priv->gpu_damage, &region->extents))) {
		DBG(("%s: region (%dx%d) overlaps gpu damage\n",
		     __FUNCTION__,
		     region->extents.x2 - region->extents.x1,
		     region->extents.y2 - region->extents.y1));
		assert(priv->gpu_bo);

		if (priv->cpu_damage == NULL) {
			if ((flags & MOVE_WRITE) == 0 &&
			    region->extents.x2 - region->extents.x1 == 1 &&
			    region->extents.y2 - region->extents.y1 == 1) {
				/*  Often associated with synchronisation, KISS */
				DBG(("%s: single pixel read\n", __FUNCTION__));
				sna_read_boxes(sna, pixmap, priv->gpu_bo,
					       &region->extents, 1);
				goto done;
			}
		} else {
			if (DAMAGE_IS_ALL(priv->cpu_damage) ||
			    sna_damage_contains_box__no_reduce(priv->cpu_damage,
							       &region->extents)) {
				assert(sna_damage_contains_box(&priv->gpu_damage, &region->extents) == PIXMAN_REGION_OUT);
				assert(sna_damage_contains_box(&priv->cpu_damage, &region->extents) == PIXMAN_REGION_IN);

				DBG(("%s: region already in CPU damage\n",
				     __FUNCTION__));
				goto already_damaged;
			}
		}

		if (sna_damage_contains_box(&priv->gpu_damage,
					    &region->extents) != PIXMAN_REGION_OUT) {
			RegionRec want, *r = region;

			DBG(("%s: region (%dx%d) intersects gpu damage\n",
			     __FUNCTION__,
			     region->extents.x2 - region->extents.x1,
			     region->extents.y2 - region->extents.y1));

			if ((flags & MOVE_WRITE) == 0 &&
			    region->extents.x2 - region->extents.x1 == 1 &&
			    region->extents.y2 - region->extents.y1 == 1) {
				sna_read_boxes(sna, pixmap, priv->gpu_bo,
					       &region->extents, 1);
				goto done;
			}

			/* Expand the region to move 32x32 pixel blocks at a
			 * time, as we assume that we will continue writing
			 * afterwards and so aim to coallesce subsequent
			 * reads.
			 */
			if (flags & MOVE_WRITE) {
				int n = region_num_rects(region), i;
				const BoxRec *boxes = region_rects(region);
				BoxPtr blocks;

				blocks = NULL;
				if (priv->cpu_damage == NULL)
					blocks = malloc(sizeof(BoxRec) * n);
				if (blocks) {
					for (i = 0; i < n; i++) {
						blocks[i].x1 = boxes[i].x1 & ~31;
						if (blocks[i].x1 < 0)
							blocks[i].x1 = 0;

						blocks[i].x2 = (boxes[i].x2 + 31) & ~31;
						if (blocks[i].x2 > pixmap->drawable.width)
							blocks[i].x2 = pixmap->drawable.width;

						blocks[i].y1 = boxes[i].y1 & ~31;
						if (blocks[i].y1 < 0)
							blocks[i].y1 = 0;

						blocks[i].y2 = (boxes[i].y2 + 31) & ~31;
						if (blocks[i].y2 > pixmap->drawable.height)
							blocks[i].y2 = pixmap->drawable.height;
					}
					if (pixman_region_init_rects(&want, blocks, i))
						r = &want;
					free(blocks);
				}
			}

			if (region_subsumes_damage(r, priv->gpu_damage)) {
				const BoxRec *box;
				int n;

				DBG(("%s: region wholly contains damage\n",
				     __FUNCTION__));

				n = sna_damage_get_boxes(priv->gpu_damage, &box);
				if (n)
					download_boxes(sna, priv, n, box);

				sna_damage_destroy(&priv->gpu_damage);
			} else if (DAMAGE_IS_ALL(priv->gpu_damage) ||
				   sna_damage_contains_box__no_reduce(priv->gpu_damage,
								      &r->extents)) {

				DBG(("%s: region wholly inside damage\n",
				     __FUNCTION__));

				assert(sna_damage_contains_box(&priv->gpu_damage, &r->extents) == PIXMAN_REGION_IN);
				assert(sna_damage_contains_box(&priv->cpu_damage, &r->extents) == PIXMAN_REGION_OUT);

				download_boxes(sna, priv,
					       region_num_rects(r),
					       region_rects(r));
				sna_damage_subtract(&priv->gpu_damage, r);
			} else {
				RegionRec need;

				pixman_region_init(&need);
				if (sna_damage_intersect(priv->gpu_damage, r, &need)) {
					DBG(("%s: region intersects damage\n",
					     __FUNCTION__));

					download_boxes(sna, priv,
						       region_num_rects(&need),
						       region_rects(&need));
					sna_damage_subtract(&priv->gpu_damage, r);
					RegionUninit(&need);
				}
			}
			if (r == &want)
				pixman_region_fini(&want);
		}
	}

done:
	if ((flags & (MOVE_WRITE | MOVE_ASYNC_HINT)) == MOVE_WRITE) {
		DBG(("%s: applying cpu damage\n", __FUNCTION__));
		assert(!DAMAGE_IS_ALL(priv->cpu_damage));
		assert_pixmap_contains_box(pixmap, RegionExtents(region));
		sna_damage_add(&priv->cpu_damage, region);
		sna_damage_reduce_all(&priv->cpu_damage, pixmap);
		if (DAMAGE_IS_ALL(priv->cpu_damage)) {
			DBG(("%s: replaced entire pixmap\n", __FUNCTION__));
			sna_pixmap_free_gpu(sna, priv);
		}
		if (priv->flush) {
			assert(!priv->shm);
			sna_add_flush_pixmap(sna, priv, priv->gpu_bo);
		}
	}

already_damaged:
	if (dx | dy)
		RegionTranslate(region, -dx, -dy);

out:
	if (flags & MOVE_WRITE) {
		assert(!DAMAGE_IS_ALL(priv->gpu_damage));
		priv->source_count = SOURCE_BIAS;
		assert(priv->gpu_bo == NULL || priv->gpu_bo->proxy == NULL);
		assert(priv->gpu_bo || priv->gpu_damage == NULL);
		assert(!priv->flush || !list_is_empty(&priv->flush_list));
		assert(!priv->clear);
	}
	if ((flags & MOVE_ASYNC_HINT) == 0 && priv->cpu_bo) {
		DBG(("%s: syncing cpu bo\n", __FUNCTION__));
		assert(pixmap->devPrivate.ptr == MAP(priv->cpu_bo->map__cpu));
		kgem_bo_sync__cpu_full(&sna->kgem, priv->cpu_bo,
				       FORCE_FULL_SYNC || flags & MOVE_WRITE);
		assert((flags & MOVE_WRITE) == 0 || !kgem_bo_is_busy(priv->cpu_bo));
	}
skip:
	priv->cpu |= (flags & (MOVE_WRITE | MOVE_ASYNC_HINT)) == MOVE_WRITE;
	assert(pixmap->devPrivate.ptr == PTR(priv->ptr));
	assert(pixmap->devKind);
	assert_pixmap_damage(pixmap);
	assert(has_coherent_ptr(sna, priv, flags));
	return true;
}

bool
sna_drawable_move_to_cpu(DrawablePtr drawable, unsigned flags)
{
	RegionRec region;
	PixmapPtr pixmap;
	int16_t dx, dy;

	if (drawable->type == DRAWABLE_PIXMAP)
		return sna_pixmap_move_to_cpu((PixmapPtr)drawable, flags);

	pixmap = get_window_pixmap((WindowPtr)drawable);
	get_drawable_deltas(drawable, pixmap, &dx, &dy);

	DBG(("%s: (%d, %d)x(%d, %d) + (%d, %d), flags=%x\n",
	     __FUNCTION__,
	     drawable->x, drawable->y,
	     drawable->width, drawable->height,
	     dx, dy, flags));

	region.extents.x1 = drawable->x + dx;
	region.extents.y1 = drawable->y + dy;
	region.extents.x2 = region.extents.x1 + drawable->width;
	region.extents.y2 = region.extents.y1 + drawable->height;
	region.data = NULL;

	if (region.extents.x1 < 0)
		region.extents.x1 = 0;
	if (region.extents.y1 < 0)
		region.extents.y1 = 0;
	if (region.extents.x2 > pixmap->drawable.width)
		region.extents.x2 = pixmap->drawable.width;
	if (region.extents.y2 > pixmap->drawable.height)
		region.extents.y2 = pixmap->drawable.height;

	if (box_empty(&region.extents))
		return true;

	return sna_drawable_move_region_to_cpu(&pixmap->drawable, &region, flags);
}

pure static bool alu_overwrites(uint8_t alu)
{
	switch (alu) {
	case GXclear:
	case GXcopy:
	case GXcopyInverted:
	case GXset:
		return true;
	default:
		return false;
	}
}

inline static bool drawable_gc_inplace_hint(DrawablePtr draw, GCPtr gc)
{
	if (!alu_overwrites(gc->alu))
		return false;

	if (!PM_IS_SOLID(draw, gc->planemask))
		return false;

	if (gc->fillStyle == FillStippled)
		return false;

	return true;
}

inline static unsigned
drawable_gc_flags(DrawablePtr draw, GCPtr gc, bool partial)
{
	assert(sna_gc(gc)->changes == 0);

	if (gc->fillStyle == FillStippled) {
		DBG(("%s: read due to fill %d\n",
		     __FUNCTION__, gc->fillStyle));
		return MOVE_READ | MOVE_WRITE;
	}

	if (fb_gc(gc)->and | fb_gc(gc)->bgand) {
		DBG(("%s: read due to rrop %d:%x\n",
		     __FUNCTION__, gc->alu, (unsigned)fb_gc(gc)->and));
		return MOVE_READ | MOVE_WRITE;
	}

	DBG(("%s: try operating on drawable inplace [hint? %d]\n",
	     __FUNCTION__, drawable_gc_inplace_hint(draw, gc)));

	return (partial ? MOVE_READ : 0) | MOVE_WRITE | MOVE_INPLACE_HINT;
}

static inline struct sna_pixmap *
sna_pixmap_mark_active(struct sna *sna, struct sna_pixmap *priv)
{
	assert(priv->gpu_bo);
	DBG(("%s: pixmap=%ld, handle=%u\n", __FUNCTION__,
	     priv->pixmap->drawable.serialNumber,
	     priv->gpu_bo->handle));
	return priv;
}

inline static struct sna_pixmap *
__sna_pixmap_for_gpu(struct sna *sna, PixmapPtr pixmap, unsigned flags)
{
	struct sna_pixmap *priv;

	if ((flags & __MOVE_FORCE) == 0 && wedged(sna))
		return NULL;

	priv = sna_pixmap(pixmap);
	if (priv == NULL) {
		DBG(("%s: not attached\n", __FUNCTION__));
		if ((flags & __MOVE_DRI) == 0)
			return NULL;

		if (pixmap->usage_hint == -1) {
			DBG(("%s: not promoting SHM Pixmap for DRI\n", __FUNCTION__));
			return NULL;
		}

		DBG(("%s: forcing the creation on the GPU\n", __FUNCTION__));

		priv = sna_pixmap_attach(pixmap);
		if (priv == NULL)
			return NULL;

		sna_damage_all(&priv->cpu_damage, pixmap);

		assert(priv->gpu_bo == NULL);
		assert(priv->gpu_damage == NULL);
	}

	return priv;
}

struct sna_pixmap *
sna_pixmap_move_area_to_gpu(PixmapPtr pixmap, const BoxRec *box, unsigned int flags)
{
	struct sna *sna = to_sna_from_pixmap(pixmap);
	struct sna_pixmap *priv;
	RegionRec i, r;

	DBG(("%s: pixmap=%ld box=(%d, %d), (%d, %d), flags=%x\n",
	     __FUNCTION__, pixmap->drawable.serialNumber,
	     box->x1, box->y1, box->x2, box->y2, flags));

	priv = __sna_pixmap_for_gpu(sna, pixmap, flags);
	if (priv == NULL)
		return NULL;

	assert(box->x2 > box->x1 && box->y2 > box->y1);
	assert_pixmap_damage(pixmap);
	assert_pixmap_contains_box(pixmap, box);
	assert(priv->gpu_damage == NULL || priv->gpu_bo);

	if ((flags & MOVE_READ) == 0)
		sna_damage_subtract_box(&priv->cpu_damage, box);

	if (priv->move_to_gpu) {
		unsigned int hint;

		DBG(("%s: applying move-to-gpu override\n", __FUNCTION__));
		hint = flags | MOVE_READ;
		if ((flags & MOVE_READ) == 0) {
			RegionRec region;

			region.extents = *box;
			region.data = NULL;
			sna_pixmap_discard_shadow_damage(priv, &region);
			if (region_subsumes_pixmap(&region, pixmap))
				hint &= ~MOVE_READ;
		} else {
			if (priv->cpu_damage)
				hint |= MOVE_WRITE;
		}
		if (!priv->move_to_gpu(sna, priv, hint)) {
			DBG(("%s: move-to-gpu override failed\n", __FUNCTION__));
			return NULL;
		}
	}

	if (priv->cow) {
		unsigned cow = flags & (MOVE_READ | MOVE_WRITE | __MOVE_FORCE);

		if ((flags & MOVE_READ) == 0) {
			if (priv->gpu_damage) {
				r.extents = *box;
				r.data = NULL;
				if (!region_subsumes_damage(&r, priv->gpu_damage))
					cow |= MOVE_READ;
			}
		} else {
			if (priv->cpu_damage) {
				r.extents = *box;
				r.data = NULL;
				if (region_overlaps_damage(&r, priv->cpu_damage, 0, 0))
					cow |= MOVE_WRITE;
			}
		}

		if (cow) {
			if (!sna_pixmap_undo_cow(sna, priv, cow))
				return NULL;

			if (priv->gpu_bo == NULL)
				sna_damage_destroy(&priv->gpu_damage);
		}
	}

	if (sna_damage_is_all(&priv->gpu_damage,
			      pixmap->drawable.width,
			      pixmap->drawable.height)) {
		assert(priv->gpu_bo);
		assert(priv->gpu_bo->proxy == NULL);
		sna_damage_destroy(&priv->cpu_damage);
		list_del(&priv->flush_list);
		goto done;
	}

	if (kgem_bo_discard_cache(priv->gpu_bo, flags & (MOVE_WRITE | __MOVE_FORCE))) {
		DBG(("%s: discarding cached upload buffer\n", __FUNCTION__));
		assert(DAMAGE_IS_ALL(priv->cpu_damage));
		assert(priv->gpu_damage == NULL || DAMAGE_IS_ALL(priv->gpu_damage)); /* magical upload buffer */
		assert(!priv->pinned);
		assert(!priv->mapped);
		sna_damage_destroy(&priv->gpu_damage);
		kgem_bo_destroy(&sna->kgem, priv->gpu_bo);
		priv->gpu_bo = NULL;
	}

	sna_damage_reduce(&priv->cpu_damage);
	assert_pixmap_damage(pixmap);

	if (priv->cpu_damage == NULL) {
		list_del(&priv->flush_list);
		return sna_pixmap_move_to_gpu(pixmap, MOVE_READ | flags);
	}

	if (priv->gpu_bo == NULL) {
		assert(priv->gpu_damage == NULL);

		if (flags & __MOVE_FORCE || priv->create & KGEM_CAN_CREATE_GPU)
			sna_pixmap_alloc_gpu(sna, pixmap, priv, CREATE_INACTIVE);

		if (priv->gpu_bo == NULL)
			return NULL;

		DBG(("%s: created gpu bo\n", __FUNCTION__));
	}

	if (priv->gpu_bo->proxy) {
		DBG(("%s: reusing cached upload\n", __FUNCTION__));
		assert((flags & MOVE_WRITE) == 0);
		assert(priv->gpu_damage == NULL);
		return priv;
	}

	if (priv->shm) {
		assert(!priv->flush);
		sna_add_flush_pixmap(sna, priv, priv->cpu_bo);
	}

	assert(priv->cpu_damage);
	region_set(&r, box);
	if (MIGRATE_ALL || region_subsumes_damage(&r, priv->cpu_damage)) {
		bool ok = false;
		int n;

		n = sna_damage_get_boxes(priv->cpu_damage, &box);
		assert(n);
		if (use_cpu_bo_for_upload(sna, priv, 0)) {
			DBG(("%s: using CPU bo for upload to GPU\n", __FUNCTION__));
			ok = sna->render.copy_boxes(sna, GXcopy,
						    &pixmap->drawable, priv->cpu_bo, 0, 0,
						    &pixmap->drawable, priv->gpu_bo, 0, 0,
						    box, n, 0);
		}
		if (!ok) {
			sna_pixmap_unmap(pixmap, priv);
			if (pixmap->devPrivate.ptr == NULL)
				return NULL;

			assert(pixmap->devKind);
			if (n == 1 && !priv->pinned &&
			    box->x1 <= 0 && box->y1 <= 0 &&
			    box->x2 >= pixmap->drawable.width &&
			    box->y2 >= pixmap->drawable.height) {
				ok = sna_replace(sna, pixmap,
						 pixmap->devPrivate.ptr,
						 pixmap->devKind);
			} else {
				ok = sna_write_boxes(sna, pixmap,
						     priv->gpu_bo, 0, 0,
						     pixmap->devPrivate.ptr,
						     pixmap->devKind,
						     0, 0,
						     box, n);
			}
			if (!ok)
				return NULL;
		}

		sna_damage_destroy(&priv->cpu_damage);
	} else if (DAMAGE_IS_ALL(priv->cpu_damage) ||
		   sna_damage_contains_box__no_reduce(priv->cpu_damage, box)) {
		bool ok = false;

		assert(sna_damage_contains_box(&priv->gpu_damage, box) == PIXMAN_REGION_OUT);
		assert(sna_damage_contains_box(&priv->cpu_damage, box) == PIXMAN_REGION_IN);

		if (use_cpu_bo_for_upload(sna, priv, 0)) {
			DBG(("%s: using CPU bo for upload to GPU\n", __FUNCTION__));
			ok = sna->render.copy_boxes(sna, GXcopy,
						    &pixmap->drawable, priv->cpu_bo, 0, 0,
						    &pixmap->drawable, priv->gpu_bo, 0, 0,
						    box, 1, 0);
		}
		if (!ok) {
			sna_pixmap_unmap(pixmap, priv);
			if (pixmap->devPrivate.ptr != NULL) {
				assert(pixmap->devKind);
				ok = sna_write_boxes(sna, pixmap,
						priv->gpu_bo, 0, 0,
						pixmap->devPrivate.ptr,
						pixmap->devKind,
						0, 0,
						box, 1);
			}
		}
		if (!ok)
			return NULL;

		sna_damage_subtract(&priv->cpu_damage, &r);
	} else if (sna_damage_intersect(priv->cpu_damage, &r, &i)) {
		int n = region_num_rects(&i);
		bool ok;

		box = region_rects(&i);
		ok = false;
		if (use_cpu_bo_for_upload(sna, priv, 0)) {
			DBG(("%s: using CPU bo for upload to GPU, %d boxes\n", __FUNCTION__, n));
			ok = sna->render.copy_boxes(sna, GXcopy,
						    &pixmap->drawable, priv->cpu_bo, 0, 0,
						    &pixmap->drawable, priv->gpu_bo, 0, 0,
						    box, n, 0);
		}
		if (!ok) {
			sna_pixmap_unmap(pixmap, priv);
			if (pixmap->devPrivate.ptr != NULL) {
				assert(pixmap->devKind);
				ok = sna_write_boxes(sna, pixmap,
						priv->gpu_bo, 0, 0,
						pixmap->devPrivate.ptr,
						pixmap->devKind,
						0, 0,
						box, n);
			}
		}
		if (!ok)
			return NULL;

		sna_damage_subtract(&priv->cpu_damage, &r);
		RegionUninit(&i);
	}

done:
	if (priv->cpu_damage == NULL && priv->flush)
		list_del(&priv->flush_list);
	if (flags & MOVE_WRITE) {
		priv->clear = false;
		if (!DAMAGE_IS_ALL(priv->gpu_damage) &&
		    priv->cpu_damage == NULL &&
		    (box_covers_pixmap(pixmap, &r.extents) ||
		     box_inplace(pixmap, &r.extents))) {
			DBG(("%s: large operation on undamaged, promoting to full GPU\n",
			     __FUNCTION__));
			assert(priv->gpu_bo);
			assert(priv->gpu_bo->proxy == NULL);
			if (sna_pixmap_free_cpu(sna, priv, priv->cpu))
				sna_damage_all(&priv->gpu_damage, pixmap);
		}
		if (DAMAGE_IS_ALL(priv->gpu_damage)) {
			sna_pixmap_free_cpu(sna, priv, priv->cpu);
			sna_damage_destroy(&priv->cpu_damage);
			list_del(&priv->flush_list);
		}
		priv->cpu = false;
	}

	assert(!priv->gpu_bo->proxy || (flags & MOVE_WRITE) == 0);
	return sna_pixmap_mark_active(sna, priv);
}

struct kgem_bo *
sna_drawable_use_bo(DrawablePtr drawable, unsigned flags, const BoxRec *box,
		    struct sna_damage ***damage)
{
	PixmapPtr pixmap = get_drawable_pixmap(drawable);
	struct sna_pixmap *priv = sna_pixmap(pixmap);
	struct sna *sna;
	RegionRec region;
	int16_t dx, dy;
	int ret;

	DBG(("%s pixmap=%ld, box=((%d, %d), (%d, %d)), flags=%x...\n",
	     __FUNCTION__,
	     pixmap->drawable.serialNumber,
	     box->x1, box->y1, box->x2, box->y2,
	     flags));

	assert(box->x2 > box->x1 && box->y2 > box->y1);
	assert(pixmap->refcnt);
	assert_pixmap_damage(pixmap);
	assert_drawable_contains_box(drawable, box);

	if (priv == NULL) {
		DBG(("%s: not attached\n", __FUNCTION__));
		return NULL;
	}

	if (priv->cow) {
		unsigned cow = MOVE_WRITE | MOVE_READ;

		if (flags & IGNORE_DAMAGE) {
			if (priv->gpu_damage) {
				region.extents = *box;
				if (get_drawable_deltas(drawable, pixmap, &dx, &dy)) {
					region.extents.x1 += dx;
					region.extents.x2 += dx;
					region.extents.y1 += dy;
					region.extents.y2 += dy;
				}
				region.data = NULL;
				if (region_subsumes_damage(&region,
							   priv->gpu_damage))
					cow &= ~MOVE_READ;
			} else
				cow &= ~MOVE_READ;
		}

		if (!sna_pixmap_undo_cow(to_sna_from_pixmap(pixmap), priv, cow))
			return NULL;

		if (priv->gpu_bo == NULL)
			sna_damage_destroy(&priv->gpu_damage);
	}

	if (kgem_bo_discard_cache(priv->gpu_bo, true)) {
		DBG(("%s: cached upload proxy, discard and revert to GPU\n", __FUNCTION__));
		assert(DAMAGE_IS_ALL(priv->cpu_damage));
		assert(priv->gpu_damage == NULL || DAMAGE_IS_ALL(priv->gpu_damage)); /* magical upload buffer */
		assert(!priv->pinned);
		assert(!priv->mapped);
		sna_damage_destroy(&priv->gpu_damage);
		kgem_bo_destroy(&to_sna_from_pixmap(pixmap)->kgem,
				priv->gpu_bo);
		priv->gpu_bo = NULL;
		goto use_cpu_bo;
	}

	if (priv->flush) {
		DBG(("%s: exported target, set PREFER_GPU\n", __FUNCTION__));
		flags |= PREFER_GPU;
	}
	if (priv->shm) {
		DBG(("%s: shm target, discard PREFER_GPU\n", __FUNCTION__));
		flags &= ~PREFER_GPU;
	}
	if (priv->pinned) {
		DBG(("%s: pinned, never REPLACES\n", __FUNCTION__));
		flags &= ~REPLACES;
	}
	if (priv->cpu && (flags & (FORCE_GPU | IGNORE_DAMAGE)) == 0) {
		DBG(("%s: last on cpu and needs damage, discard PREFER_GPU\n", __FUNCTION__));
		flags &= ~PREFER_GPU;
	}

	if ((flags & (PREFER_GPU | IGNORE_DAMAGE)) == IGNORE_DAMAGE) {
		if (priv->gpu_bo && (box_covers_pixmap(pixmap, box) || box_inplace(pixmap, box))) {
			DBG(("%s: not reading damage and large, set PREFER_GPU\n", __FUNCTION__));
			flags |= PREFER_GPU;
		}
	}

	DBG(("%s: flush=%d, shm=%d, cpu=%d => flags=%x\n",
	     __FUNCTION__, priv->flush, priv->shm, priv->cpu, flags));

	if ((flags & PREFER_GPU) == 0 &&
	    (flags & (REPLACES | IGNORE_DAMAGE) || !priv->gpu_damage || !kgem_bo_is_busy(priv->gpu_bo))) {
		DBG(("%s: try cpu as GPU bo is idle\n", __FUNCTION__));
		goto use_cpu_bo;
	}

	if (DAMAGE_IS_ALL(priv->gpu_damage)) {
		DBG(("%s: use GPU fast path (all-damaged)\n", __FUNCTION__));
		assert(priv->cpu_damage == NULL);
		assert(priv->gpu_bo);
		assert(priv->gpu_bo->proxy == NULL);
		goto use_gpu_bo;
	}

	if (DAMAGE_IS_ALL(priv->cpu_damage)) {
		assert(priv->gpu_damage == NULL);
		if ((flags & FORCE_GPU) == 0 || priv->cpu_bo) {
			DBG(("%s: use CPU fast path (all-damaged), and not forced-gpu\n",
			     __FUNCTION__));
			goto use_cpu_bo;
		}
	}

	DBG(("%s: gpu? %d, damaged? %d; cpu? %d, damaged? %d\n", __FUNCTION__,
	     priv->gpu_bo ? priv->gpu_bo->handle : 0, priv->gpu_damage != NULL,
	     priv->cpu_bo ? priv->cpu_bo->handle : 0, priv->cpu_damage != NULL));
	if (priv->gpu_bo == NULL) {
		unsigned int move;

		if ((flags & FORCE_GPU) == 0 &&
		    (priv->create & KGEM_CAN_CREATE_GPU) == 0) {
			DBG(("%s: untiled, will not force allocation\n",
			     __FUNCTION__));
			goto use_cpu_bo;
		}

		if ((flags & IGNORE_DAMAGE) == 0) {
			if (priv->cpu_bo) {
				if (to_sna_from_pixmap(pixmap)->kgem.can_blt_cpu) {
					if (kgem_bo_is_busy(priv->cpu_bo)) {
						DBG(("%s: already using CPU bo, will not force allocation\n",
						     __FUNCTION__));
						goto use_cpu_bo;
					}

					if ((flags & RENDER_GPU) == 0) {
						DBG(("%s: prefer cpu", __FUNCTION__));
						goto use_cpu_bo;
					}
				} else {
					if (kgem_bo_is_busy(priv->cpu_bo)) {
						DBG(("%s: CPU bo active, must force allocation\n",
						     __FUNCTION__));
						goto create_gpu_bo;
					}
				}
			}

			if ((flags & FORCE_GPU) == 0 && priv->cpu_damage) {
				if ((flags & PREFER_GPU) == 0) {
					DBG(("%s: already damaged and prefer cpu",
					     __FUNCTION__));
					goto use_cpu_bo;
				}

				if (!box_inplace(pixmap, box)) {
					DBG(("%s: damaged with a small operation, will not force allocation\n",
					     __FUNCTION__));
					goto use_cpu_bo;
				}
			}
		} else if (priv->cpu_damage) {
			region.extents = *box;
			if (get_drawable_deltas(drawable, pixmap, &dx, &dy)) {
				region.extents.x1 += dx;
				region.extents.x2 += dx;
				region.extents.y1 += dy;
				region.extents.y2 += dy;
			}
			region.data = NULL;

			sna_damage_subtract(&priv->cpu_damage, &region);
			if (priv->cpu_damage == NULL) {
				list_del(&priv->flush_list);
				priv->cpu = false;
			}
		}

create_gpu_bo:
		move = MOVE_WRITE | MOVE_READ;
		if (flags & FORCE_GPU)
			move |= __MOVE_FORCE;
		if (!sna_pixmap_move_to_gpu(pixmap, move))
			goto use_cpu_bo;

		DBG(("%s: allocated GPU bo for operation\n", __FUNCTION__));
		goto done;
	}


	region.extents = *box;
	if (get_drawable_deltas(drawable, pixmap, &dx, &dy)) {
		region.extents.x1 += dx;
		region.extents.x2 += dx;
		region.extents.y1 += dy;
		region.extents.y2 += dy;
	}
	region.data = NULL;

	DBG(("%s extents (%d, %d), (%d, %d)\n", __FUNCTION__,
	     region.extents.x1, region.extents.y1,
	     region.extents.x2, region.extents.y2));

	if (priv->gpu_damage) {
		assert(priv->gpu_bo);
		if (!priv->cpu_damage || flags & IGNORE_DAMAGE) {
			if (flags & REPLACES || box_covers_pixmap(pixmap, &region.extents)) {
				unsigned int move;

				if (flags & IGNORE_DAMAGE)
					move = MOVE_WRITE;
				else
					move = MOVE_WRITE | MOVE_READ;

				if (sna_pixmap_move_to_gpu(pixmap, move))
					goto use_gpu_bo;
			}

			if (DAMAGE_IS_ALL(priv->gpu_damage) ||
			    sna_damage_contains_box__no_reduce(priv->gpu_damage,
							       &region.extents)) {
				DBG(("%s: region wholly contained within GPU damage\n",
				     __FUNCTION__));
				assert(sna_damage_contains_box(&priv->gpu_damage, &region.extents) == PIXMAN_REGION_IN);
				assert(sna_damage_contains_box(&priv->cpu_damage, &region.extents) == PIXMAN_REGION_OUT);
				goto use_gpu_bo;
			} else {
				DBG(("%s: partial GPU damage with no CPU damage, continuing to use GPU\n",
				     __FUNCTION__));
				goto move_to_gpu;
			}
		}

		ret = sna_damage_contains_box(&priv->gpu_damage, &region.extents);
		if (ret == PIXMAN_REGION_IN) {
			DBG(("%s: region wholly contained within GPU damage\n",
			     __FUNCTION__));
			goto use_gpu_bo;
		}

		if (ret != PIXMAN_REGION_OUT) {
			DBG(("%s: region partially contained within GPU damage\n",
			     __FUNCTION__));
			goto move_to_gpu;
		}
	}

	if ((flags & IGNORE_DAMAGE) == 0 && priv->cpu_damage) {
		ret = sna_damage_contains_box(&priv->cpu_damage, &region.extents);
		if (ret == PIXMAN_REGION_IN) {
			DBG(("%s: region wholly contained within CPU damage\n",
			     __FUNCTION__));
			goto use_cpu_bo;
		}

		if (box_inplace(pixmap, box)) {
			DBG(("%s: forcing inplace\n", __FUNCTION__));
			goto move_to_gpu;
		}

		if (ret != PIXMAN_REGION_OUT) {
			DBG(("%s: region partially contained within CPU damage\n",
			     __FUNCTION__));
			goto use_cpu_bo;
		}
	}

move_to_gpu:
	if (!sna_pixmap_move_area_to_gpu(pixmap, &region.extents,
					 flags & IGNORE_DAMAGE ? MOVE_WRITE : MOVE_READ | MOVE_WRITE)) {
		DBG(("%s: failed to move-to-gpu, fallback\n", __FUNCTION__));
		assert(priv->gpu_bo == NULL);
		goto use_cpu_bo;
	}

done:
	assert(priv->move_to_gpu == NULL);
	assert(priv->gpu_bo != NULL);
	assert(priv->gpu_bo->refcnt);
	if (sna_damage_is_all(&priv->gpu_damage,
			      pixmap->drawable.width,
			      pixmap->drawable.height)) {
		sna_damage_destroy(&priv->cpu_damage);
		list_del(&priv->flush_list);
		*damage = NULL;
	} else
		*damage = &priv->gpu_damage;

	DBG(("%s: using GPU bo with damage? %d\n",
	     __FUNCTION__, *damage != NULL));
	assert(*damage == NULL || !DAMAGE_IS_ALL(*damage));
	assert(priv->gpu_bo->proxy == NULL);
	assert(priv->clear == false);
	assert(priv->cpu == false);
	assert(!priv->shm);
	return priv->gpu_bo;

use_gpu_bo:
	if (priv->move_to_gpu) {
		unsigned hint = MOVE_READ | MOVE_WRITE;

		sna = to_sna_from_pixmap(pixmap);

		DBG(("%s: applying move-to-gpu override\n", __FUNCTION__));
		if (flags & IGNORE_DAMAGE) {
			region.extents = *box;
			region.data = NULL;
			if (get_drawable_deltas(drawable, pixmap, &dx, &dy)) {
				region.extents.x1 += dx;
				region.extents.x2 += dx;
				region.extents.y1 += dy;
				region.extents.y2 += dy;
			}
			sna_pixmap_discard_shadow_damage(priv, &region);
			if (region_subsumes_pixmap(&region, pixmap)) {
				DBG(("%s: discarding move-to-gpu READ for subsumed pixmap\n", __FUNCTION__));
				hint = MOVE_WRITE;
			}
		}

		if (!priv->move_to_gpu(sna, priv, hint)) {
			DBG(("%s: move-to-gpu override failed\n", __FUNCTION__));
			goto use_cpu_bo;
		}
	}

	if (priv->shm) {
		assert(!priv->flush);
		list_move(&priv->flush_list, &sna->flush_pixmaps);
	}

	DBG(("%s: using whole GPU bo\n", __FUNCTION__));
	assert(priv->gpu_bo != NULL);
	assert(priv->gpu_bo->refcnt);
	assert(priv->gpu_bo->proxy == NULL);
	assert(priv->gpu_damage);
	priv->cpu = false;
	priv->clear = false;
	*damage = NULL;
	return priv->gpu_bo;

use_cpu_bo:
	if (!USE_CPU_BO || priv->cpu_bo == NULL) {
		if ((flags & FORCE_GPU) == 0) {
			DBG(("%s: no CPU bo, and GPU not forced\n", __FUNCTION__));
			return NULL;
		}

		flags &= ~FORCE_GPU;

		region.extents = *box;
		if (get_drawable_deltas(drawable, pixmap, &dx, &dy)) {
			region.extents.x1 += dx;
			region.extents.x2 += dx;
			region.extents.y1 += dy;
			region.extents.y2 += dy;
		}
		region.data = NULL;

		if (!sna_drawable_move_region_to_cpu(&pixmap->drawable, &region,
						     (flags & IGNORE_DAMAGE ? 0 : MOVE_READ) | MOVE_WRITE | MOVE_ASYNC_HINT) ||
		    priv->cpu_bo == NULL) {
			DBG(("%s: did not create CPU bo\n", __FUNCTION__));
cpu_fail:
			if (priv->gpu_bo)
				goto move_to_gpu;

			return NULL;
		}
	}

	assert(priv->cpu_bo->refcnt);

	sna = to_sna_from_pixmap(pixmap);
	if ((flags & FORCE_GPU) == 0 &&
	    !__kgem_bo_is_busy(&sna->kgem, priv->cpu_bo)) {
		DBG(("%s: has CPU bo, but is idle and acceleration not forced\n",
		     __FUNCTION__));
		return NULL;
	}

	region.extents = *box;
	if (get_drawable_deltas(drawable, pixmap, &dx, &dy)) {
		region.extents.x1 += dx;
		region.extents.x2 += dx;
		region.extents.y1 += dy;
		region.extents.y2 += dy;
	}
	region.data = NULL;

	if (priv->gpu_bo && kgem_bo_is_busy(priv->gpu_bo)) {
		DBG(("%s: both CPU and GPU are busy, prefer to use the GPU\n",
		     __FUNCTION__));
		goto move_to_gpu;
	}

	assert(priv->gpu_bo == NULL || priv->gpu_bo->proxy == NULL);

	if (flags & RENDER_GPU) {
		flags &= ~RENDER_GPU;

		if ((flags & IGNORE_DAMAGE) == 0 && priv->gpu_damage) {
			DBG(("%s: prefer to use GPU bo for rendering whilst reading from GPU damage\n", __FUNCTION__));

prefer_gpu_bo:
			if (priv->gpu_bo == NULL) {
				if ((flags & FORCE_GPU) == 0) {
					DBG(("%s: untiled, will not force allocation\n",
					     __FUNCTION__));
					return NULL;
				}

				if (flags & IGNORE_DAMAGE) {
					sna_damage_subtract(&priv->cpu_damage, &region);
					if (priv->cpu_damage == NULL) {
						list_del(&priv->flush_list);
						priv->cpu = false;
					}
				}

				if (!sna_pixmap_move_to_gpu(pixmap, MOVE_WRITE | MOVE_READ | __MOVE_FORCE))
					return NULL;

				sna_damage_all(&priv->gpu_damage, pixmap);

				DBG(("%s: allocated GPU bo for operation\n", __FUNCTION__));
				goto done;
			}
			goto move_to_gpu;
		}

		if ((priv->cpu_damage == NULL || flags & IGNORE_DAMAGE)) {
			if (priv->gpu_bo && priv->gpu_bo->tiling) {
				DBG(("%s: prefer to use GPU bo for rendering large pixmaps\n", __FUNCTION__));
				goto prefer_gpu_bo;
			}

			if (priv->cpu_bo->pitch >= 4096) {
				DBG(("%s: prefer to use GPU bo for rendering wide pixmaps\n", __FUNCTION__));
				goto prefer_gpu_bo;
			}
		}

		if ((flags & IGNORE_DAMAGE) == 0 && priv->cpu_bo->snoop) {
			DBG(("%s: prefer to use GPU bo for reading from snooped target bo\n", __FUNCTION__));
			goto prefer_gpu_bo;
		}

		if (!sna->kgem.can_blt_cpu) {
			DBG(("%s: can't render to CPU bo, try to use GPU bo\n", __FUNCTION__));
			goto prefer_gpu_bo;
		}
	}

	if (!sna->kgem.can_blt_cpu)
		goto cpu_fail;

	if (!sna_drawable_move_region_to_cpu(&pixmap->drawable, &region,
					     (flags & IGNORE_DAMAGE ? 0 : MOVE_READ) | MOVE_WRITE | MOVE_ASYNC_HINT)) {
		DBG(("%s: failed to move-to-cpu, fallback\n", __FUNCTION__));
		goto cpu_fail;
	}

	if (priv->shm) {
		assert(!priv->flush);
		sna_add_flush_pixmap(sna, priv, priv->cpu_bo);

		/* As we may have flushed and retired,, recheck for busy bo */
		if ((flags & FORCE_GPU) == 0 && !kgem_bo_is_busy(priv->cpu_bo))
			return NULL;
	}
	if (priv->flush) {
		assert(!priv->shm);
		sna_add_flush_pixmap(sna, priv, priv->gpu_bo);
	}

	if (sna_damage_is_all(&priv->cpu_damage,
			      pixmap->drawable.width,
			      pixmap->drawable.height)) {
		sna_damage_destroy(&priv->gpu_damage);
		*damage = NULL;
	} else {
		assert(!DAMAGE_IS_ALL(priv->cpu_damage));
		if (priv->cpu_damage &&
		    sna_damage_contains_box__no_reduce(priv->cpu_damage,
						       &region.extents)) {
			assert(sna_damage_contains_box(&priv->gpu_damage, &region.extents) == PIXMAN_REGION_OUT);
			assert(sna_damage_contains_box(&priv->cpu_damage, &region.extents) == PIXMAN_REGION_IN);
			*damage = NULL;
		} else
			*damage = &priv->cpu_damage;
	}

	DBG(("%s: using CPU bo with damage? %d\n",
	     __FUNCTION__, *damage != NULL));
	assert(damage == NULL || !DAMAGE_IS_ALL(*damage));
	assert(priv->clear == false);
	priv->cpu = false;
	return priv->cpu_bo;
}

PixmapPtr
sna_pixmap_create_upload(ScreenPtr screen,
			 int width, int height, int depth,
			 unsigned flags)
{
	struct sna *sna = to_sna_from_screen(screen);
	PixmapPtr pixmap;
	struct sna_pixmap *priv;
	void *ptr;

	DBG(("%s(%d, %d, %d, flags=%x)\n", __FUNCTION__,
	     width, height, depth, flags));
	assert(width);
	assert(height);

	if (depth == 1)
		return create_pixmap(sna, screen, width, height, depth,
				     CREATE_PIXMAP_USAGE_SCRATCH);

	pixmap = create_pixmap_hdr(sna, screen,
				   width, height, depth, CREATE_PIXMAP_USAGE_SCRATCH,
				   &priv);
	if (!pixmap)
		return NullPixmap;

	priv->gpu_bo = kgem_create_buffer_2d(&sna->kgem,
					     width, height,
					     pixmap->drawable.bitsPerPixel,
					     flags, &ptr);
	if (!priv->gpu_bo) {
		free(priv);
		FreePixmap(pixmap);
		return NullPixmap;
	}

	/* Marking both the shadow and the GPU bo is a little dubious,
	 * but will work so long as we always check before doing the
	 * transfer.
	 */
	sna_damage_all(&priv->gpu_damage, pixmap);
	sna_damage_all(&priv->cpu_damage, pixmap);

	pixmap->devKind = priv->gpu_bo->pitch;
	pixmap->devPrivate.ptr = ptr;
	priv->ptr = MAKE_STATIC_PTR(ptr);
	priv->stride = priv->gpu_bo->pitch;
	priv->create = 0;

	pixmap->usage_hint = 0;
	if (!kgem_buffer_is_inplace(priv->gpu_bo))
		pixmap->usage_hint = 1;

	DBG(("%s: serial=%ld, %dx%d, usage=%d\n",
	     __FUNCTION__,
	     pixmap->drawable.serialNumber,
	     pixmap->drawable.width,
	     pixmap->drawable.height,
	     pixmap->usage_hint));
	return pixmap;
}

struct sna_pixmap *
sna_pixmap_move_to_gpu(PixmapPtr pixmap, unsigned flags)
{
	struct sna *sna = to_sna_from_pixmap(pixmap);
	struct sna_pixmap *priv;
	const BoxRec *box;
	int n;

	DBG(("%s(pixmap=%ld, usage=%d), flags=%x\n",
	     __FUNCTION__,
	     pixmap->drawable.serialNumber,
	     pixmap->usage_hint,
	     flags));

	priv = __sna_pixmap_for_gpu(sna, pixmap, flags);
	if (priv == NULL)
		return NULL;

	assert_pixmap_damage(pixmap);

	if (priv->move_to_gpu &&
	    !priv->move_to_gpu(sna, priv, flags | ((priv->cpu_damage && (flags & MOVE_READ)) ? MOVE_WRITE : 0))) {
		DBG(("%s: move-to-gpu override failed\n", __FUNCTION__));
		return NULL;
	}

	if ((flags & MOVE_READ) == 0 && UNDO)
		kgem_bo_pair_undo(&sna->kgem, priv->gpu_bo, priv->cpu_bo);

	if (priv->cow) {
		unsigned cow = flags & (MOVE_READ | MOVE_WRITE | __MOVE_FORCE);
		if (flags & MOVE_READ && priv->cpu_damage)
			cow |= MOVE_WRITE;
		if (cow) {
			if (!sna_pixmap_undo_cow(sna, priv, cow))
				return NULL;

			if (priv->gpu_bo == NULL)
				sna_damage_destroy(&priv->gpu_damage);
		}
	}

	if (sna_damage_is_all(&priv->gpu_damage,
			      pixmap->drawable.width,
			      pixmap->drawable.height)) {
		DBG(("%s: already all-damaged\n", __FUNCTION__));
		assert(DAMAGE_IS_ALL(priv->gpu_damage));
		assert(priv->gpu_bo);
		assert(priv->gpu_bo->proxy == NULL);
		assert_pixmap_map(pixmap, priv);
		sna_damage_destroy(&priv->cpu_damage);
		list_del(&priv->flush_list);
		goto active;
	}

	if ((flags & MOVE_READ) == 0)
		sna_damage_destroy(&priv->cpu_damage);

	sna_damage_reduce(&priv->cpu_damage);
	assert_pixmap_damage(pixmap);
	DBG(("%s: CPU damage? %d\n", __FUNCTION__, priv->cpu_damage != NULL));
	if (priv->gpu_bo == NULL ||
	    kgem_bo_discard_cache(priv->gpu_bo, flags & (MOVE_WRITE | __MOVE_FORCE))) {
		struct kgem_bo *proxy;

		proxy = priv->gpu_bo;
		priv->gpu_bo = NULL;

		DBG(("%s: creating GPU bo (%dx%d@%d), create=%x\n",
		     __FUNCTION__,
		     pixmap->drawable.width,
		     pixmap->drawable.height,
		     pixmap->drawable.bitsPerPixel,
		     priv->create));
		assert(!priv->mapped);
		assert(list_is_empty(&priv->flush_list));

		if (flags & __MOVE_FORCE || priv->create & KGEM_CAN_CREATE_GPU) {
			bool is_linear;

			assert(pixmap->drawable.width > 0);
			assert(pixmap->drawable.height > 0);
			assert(pixmap->drawable.bitsPerPixel >= 8);

			is_linear = sna_pixmap_default_tiling(sna, pixmap) == I915_TILING_NONE;
			if (is_linear && flags & __MOVE_TILED) {
				DBG(("%s: not creating linear GPU bo\n", __FUNCTION__));
				return NULL;
			}

			if (is_linear && priv->cpu_bo && !priv->shm &&
			    kgem_bo_convert_to_gpu(&sna->kgem, priv->cpu_bo, flags)) {
				assert(!priv->mapped);
				assert(!IS_STATIC_PTR(priv->ptr));
#ifdef DEBUG_MEMORY
				sna->debug_memory.cpu_bo_allocs--;
				sna->debug_memory.cpu_bo_bytes -= kgem_bo_size(priv->cpu_bo);
#endif
				priv->gpu_bo = priv->cpu_bo;
				priv->cpu_bo = NULL;
				priv->ptr = NULL;
				pixmap->devPrivate.ptr = NULL;
				sna_damage_all(&priv->gpu_damage, pixmap);
				sna_damage_destroy(&priv->cpu_damage);
			} else {
				unsigned create = 0;
				if (flags & MOVE_INPLACE_HINT || (priv->cpu_damage && priv->cpu_bo == NULL))
					create = CREATE_GTT_MAP | CREATE_INACTIVE;

				sna_pixmap_alloc_gpu(sna, pixmap, priv, create);
			}
		}

		if (priv->gpu_bo == NULL) {
			DBG(("%s: not creating GPU bo\n", __FUNCTION__));
			assert(priv->gpu_damage == NULL);
			priv->gpu_bo = proxy;
			if (proxy)
				sna_damage_all(&priv->cpu_damage, pixmap);
			return NULL;
		}

		if (proxy) {
			DBG(("%s: promoting upload proxy handle=%d to GPU\n", __FUNCTION__, proxy->handle));

			if (priv->cpu_damage &&
			    sna->render.copy_boxes(sna, GXcopy,
						   &pixmap->drawable, proxy, 0, 0,
						   &pixmap->drawable, priv->gpu_bo, 0, 0,
						   region_rects(DAMAGE_REGION(priv->cpu_damage)),
						   region_num_rects(DAMAGE_REGION(priv->cpu_damage)),
						   0))
				sna_damage_destroy(&priv->cpu_damage);

			kgem_bo_destroy(&sna->kgem, proxy);
		}

		if (flags & MOVE_WRITE && priv->cpu_damage == NULL) {
			/* Presume that we will only ever write to the GPU
			 * bo. Readbacks are expensive but fairly constant
			 * in cost for all sizes i.e. it is the act of
			 * synchronisation that takes the most time. This is
			 * mitigated by avoiding fallbacks in the first place.
			 */
			assert(priv->gpu_bo);
			assert(priv->gpu_bo->proxy == NULL);
			sna_damage_all(&priv->gpu_damage, pixmap);
			DBG(("%s: marking as all-damaged for GPU\n",
			     __FUNCTION__));
			goto active;
		}
	}

	if (priv->gpu_bo->proxy) {
		DBG(("%s: reusing cached upload\n", __FUNCTION__));
		assert((flags & MOVE_WRITE) == 0);
		assert(priv->gpu_damage == NULL);
		return priv;
	}

	if (priv->cpu_damage == NULL)
		goto done;

	if (DAMAGE_IS_ALL(priv->cpu_damage) && priv->cpu_bo &&
	    !priv->pinned && !priv->shm &&
	    priv->gpu_bo->tiling == I915_TILING_NONE &&
	    kgem_bo_convert_to_gpu(&sna->kgem, priv->cpu_bo, flags)) {
		assert(!priv->mapped);
		assert(!IS_STATIC_PTR(priv->ptr));
#ifdef DEBUG_MEMORY
		sna->debug_memory.cpu_bo_allocs--;
		sna->debug_memory.cpu_bo_bytes -= kgem_bo_size(priv->cpu_bo);
#endif
		sna_pixmap_free_gpu(sna, priv);
		priv->gpu_bo = priv->cpu_bo;
		priv->cpu_bo = NULL;
		priv->ptr = NULL;
		pixmap->devPrivate.ptr = NULL;
		sna_damage_all(&priv->gpu_damage, pixmap);
		sna_damage_destroy(&priv->cpu_damage);
		goto done;
	}

	if (priv->shm) {
		assert(!priv->flush);
		sna_add_flush_pixmap(sna, priv, priv->cpu_bo);
	}

	n = sna_damage_get_boxes(priv->cpu_damage, &box);
	assert(n);
	if (n) {
		bool ok;

		assert_pixmap_contains_damage(pixmap, priv->cpu_damage);
		DBG(("%s: uploading %d damage boxes\n", __FUNCTION__, n));

		ok = false;
		if (use_cpu_bo_for_upload(sna, priv, flags)) {
			DBG(("%s: using CPU bo for upload to GPU\n", __FUNCTION__));
			ok = sna->render.copy_boxes(sna, GXcopy,
						    &pixmap->drawable, priv->cpu_bo, 0, 0,
						    &pixmap->drawable, priv->gpu_bo, 0, 0,
						    box, n, 0);
		}
		if (!ok) {
			sna_pixmap_unmap(pixmap, priv);
			if (pixmap->devPrivate.ptr == NULL)
				return NULL;

			assert(pixmap->devKind);
			if (n == 1 && !priv->pinned &&
			    (box->x2 - box->x1) >= pixmap->drawable.width &&
			    (box->y2 - box->y1) >= pixmap->drawable.height) {
				ok = sna_replace(sna, pixmap,
						 pixmap->devPrivate.ptr,
						 pixmap->devKind);
			} else {
				ok = sna_write_boxes(sna, pixmap,
						     priv->gpu_bo, 0, 0,
						     pixmap->devPrivate.ptr,
						     pixmap->devKind,
						     0, 0,
						     box, n);
			}
			if (!ok)
				return NULL;
		}
	}

	__sna_damage_destroy(DAMAGE_PTR(priv->cpu_damage));
	priv->cpu_damage = NULL;

	/* For large bo, try to keep only a single copy around */
	if (priv->create & KGEM_CAN_CREATE_LARGE || flags & MOVE_SOURCE_HINT) {
		DBG(("%s: disposing of system copy for large/source\n",
		     __FUNCTION__));
		assert(!priv->shm);
		assert(priv->gpu_bo);
		assert(priv->gpu_bo->proxy == NULL);
		sna_damage_all(&priv->gpu_damage, pixmap);
		sna_pixmap_free_cpu(sna, priv,
				    (priv->create & KGEM_CAN_CREATE_LARGE) ? false : priv->cpu);
	}
done:
	list_del(&priv->flush_list);

	sna_damage_reduce_all(&priv->gpu_damage, pixmap);
	if (DAMAGE_IS_ALL(priv->gpu_damage))
		sna_pixmap_free_cpu(sna, priv, priv->cpu);

active:
	if (flags & MOVE_WRITE) {
		priv->clear = false;
		priv->cpu = false;
	}
	assert(!priv->gpu_bo->proxy || (flags & MOVE_WRITE) == 0);
	return sna_pixmap_mark_active(sna, priv);
}

static bool must_check sna_validate_pixmap(DrawablePtr draw, PixmapPtr pixmap)
{
	DBG(("%s: target bpp=%d, source bpp=%d\n",
	     __FUNCTION__, draw->bitsPerPixel, pixmap->drawable.bitsPerPixel));

	if (draw->bitsPerPixel == pixmap->drawable.bitsPerPixel &&
	    FbEvenTile(pixmap->drawable.width *
		       pixmap->drawable.bitsPerPixel)) {
		DBG(("%s: flushing pixmap\n", __FUNCTION__));
		if (!sna_pixmap_move_to_cpu(pixmap, MOVE_READ))
			return false;

		fbPadPixmap(pixmap);
	}

	return true;
}

static bool must_check sna_gc_move_to_cpu(GCPtr gc,
					  DrawablePtr drawable,
					  RegionPtr region)
{
	struct sna_gc *sgc = sna_gc(gc);
	long changes = sgc->changes;

	DBG(("%s(%p) changes=%lx\n", __FUNCTION__, gc, changes));
	assert(drawable);
	assert(region);

	assert(gc->ops == (GCOps *)&sna_gc_ops);
	gc->ops = (GCOps *)&sna_gc_ops__cpu;

	assert(gc->funcs);
	sgc->old_funcs = gc->funcs;
	gc->funcs = (GCFuncs *)&sna_gc_funcs__cpu;

	assert(gc->pCompositeClip);
	sgc->priv = gc->pCompositeClip;
	gc->pCompositeClip = region;

#if XORG_VERSION_CURRENT < XORG_VERSION_NUMERIC(1,16,99,901,0)
	if (gc->clientClipType == CT_PIXMAP) {
		PixmapPtr clip = gc->clientClip;
		gc->clientClip = region_from_bitmap(gc->pScreen, clip);
		gc->pScreen->DestroyPixmap(clip);
		gc->clientClipType = gc->clientClip ? CT_REGION : CT_NONE;
		changes |= GCClipMask;
	} else
		changes &= ~GCClipMask;
#else
	changes &= ~GCClipMask;
#endif

	if (changes || drawable->serialNumber != (sgc->serial & DRAWABLE_SERIAL_BITS)) {
		long tmp = gc->serialNumber;
		gc->serialNumber = sgc->serial;

		if (fb_gc(gc)->bpp != drawable->bitsPerPixel) {
			changes |= GCStipple | GCForeground | GCBackground | GCPlaneMask;
			fb_gc(gc)->bpp = drawable->bitsPerPixel;
		}

		if (changes & GCTile && !gc->tileIsPixel) {
			DBG(("%s: flushing tile pixmap\n", __FUNCTION__));
			if (!sna_validate_pixmap(drawable, gc->tile.pixmap))
				return false;
		}

		if (changes & GCStipple && gc->stipple) {
			DBG(("%s: flushing stipple pixmap\n", __FUNCTION__));
			if (!sna_validate_pixmap(drawable, gc->stipple))
				return false;
		}

		fbValidateGC(gc, changes, drawable);
		gc->serialNumber = tmp;
	}
	sgc->changes = 0;

	switch (gc->fillStyle) {
	case FillTiled:
		DBG(("%s: moving tile to cpu\n", __FUNCTION__));
		return sna_drawable_move_to_cpu(&gc->tile.pixmap->drawable, MOVE_READ);
	case FillStippled:
	case FillOpaqueStippled:
		DBG(("%s: moving stipple to cpu\n", __FUNCTION__));
		return sna_drawable_move_to_cpu(&gc->stipple->drawable, MOVE_READ);
	default:
		return true;
	}
}

static void sna_gc_move_to_gpu(GCPtr gc)
{
	DBG(("%s(%p)\n", __FUNCTION__, gc));

	assert(gc->ops == (GCOps *)&sna_gc_ops__cpu);
	assert(gc->funcs == (GCFuncs *)&sna_gc_funcs__cpu);

	gc->ops = (GCOps *)&sna_gc_ops;
	gc->funcs = (GCFuncs *)sna_gc(gc)->old_funcs;
	assert(gc->funcs);
	gc->pCompositeClip = sna_gc(gc)->priv;
	assert(gc->pCompositeClip);
}

static inline bool clip_box(BoxPtr box, GCPtr gc)
{
	const BoxRec *clip;
	bool clipped;

	assert(gc->pCompositeClip);
	clip = &gc->pCompositeClip->extents;

	clipped = !region_is_singular(gc->pCompositeClip);
	if (box->x1 < clip->x1)
		box->x1 = clip->x1, clipped = true;
	if (box->x2 > clip->x2)
		box->x2 = clip->x2, clipped = true;

	if (box->y1 < clip->y1)
		box->y1 = clip->y1, clipped = true;
	if (box->y2 > clip->y2)
		box->y2 = clip->y2, clipped = true;

	return clipped;
}

static inline void translate_box(BoxPtr box, DrawablePtr d)
{
	box->x1 += d->x;
	box->x2 += d->x;

	box->y1 += d->y;
	box->y2 += d->y;
}

static inline bool trim_and_translate_box(BoxPtr box, DrawablePtr d, GCPtr gc)
{
	translate_box(box, d);
	return clip_box(box, gc);
}

static inline bool box32_clip(Box32Rec *box, GCPtr gc)
{
	bool clipped = !region_is_singular(gc->pCompositeClip);
	const BoxRec *clip = &gc->pCompositeClip->extents;

	if (box->x1 < clip->x1)
		box->x1 = clip->x1, clipped = true;
	if (box->x2 > clip->x2)
		box->x2 = clip->x2, clipped = true;

	if (box->y1 < clip->y1)
		box->y1 = clip->y1, clipped = true;
	if (box->y2 > clip->y2)
		box->y2 = clip->y2, clipped = true;

	return clipped;
}

static inline void box32_translate(Box32Rec *box, DrawablePtr d)
{
	box->x1 += d->x;
	box->x2 += d->x;

	box->y1 += d->y;
	box->y2 += d->y;
}

static inline bool box32_trim_and_translate(Box32Rec *box, DrawablePtr d, GCPtr gc)
{
	box32_translate(box, d);
	return box32_clip(box, gc);
}

static inline void box_add_pt(BoxPtr box, int16_t x, int16_t y)
{
	if (box->x1 > x)
		box->x1 = x;
	else if (box->x2 < x)
		box->x2 = x;

	if (box->y1 > y)
		box->y1 = y;
	else if (box->y2 < y)
		box->y2 = y;
}

static inline bool box32_to_box16(const Box32Rec *b32, BoxRec *b16)
{
	b16->x1 = b32->x1;
	b16->y1 = b32->y1;
	b16->x2 = b32->x2;
	b16->y2 = b32->y2;

	return b16->x2 > b16->x1 && b16->y2 > b16->y1;
}

static inline void box32_add_rect(Box32Rec *box, const xRectangle *r)
{
	int32_t v;

	v = r->x;
	if (box->x1 > v)
		box->x1 = v;
	v += r->width;
	if (box->x2 < v)
		box->x2 = v;

	v = r->y;
	if (box->y1 > v)
		box->y1 = v;
	v += r->height;
	if (box->y2 < v)
		box->y2 = v;
}

static bool
can_create_upload_tiled_x(struct sna *sna,
			  PixmapPtr pixmap,
			  struct sna_pixmap *priv,
			  bool replaces)
{
	if (priv->shm || (priv->cpu && !replaces))
		return false;

	if ((priv->create & KGEM_CAN_CREATE_GPU) == 0)
		return false;

	if (sna->kgem.has_llc)
		return true;

	if (sna_pixmap_default_tiling(sna, pixmap))
		return false;

	return true;
}

static bool
create_upload_tiled_x(struct sna *sna,
		      PixmapPtr pixmap,
		      struct sna_pixmap *priv,
		      bool replaces)
{
	unsigned create;

	if (!can_create_upload_tiled_x(sna, pixmap, priv, replaces))
		return false;

	assert(priv->gpu_bo == NULL);
	assert(priv->gpu_damage == NULL);

	create = CREATE_CPU_MAP | CREATE_INACTIVE;
	if (!sna->kgem.has_llc)
		create |= CREATE_CACHED;

	return sna_pixmap_alloc_gpu(sna, pixmap, priv, create);
}

static bool
try_upload__tiled_x(PixmapPtr pixmap, RegionRec *region,
		    int x, int y, int w, int  h, char *bits, int stride)
{
	struct sna *sna = to_sna_from_pixmap(pixmap);
	struct sna_pixmap *priv = sna_pixmap(pixmap);
	const BoxRec *box;
	uint8_t *dst;
	int n;

	if (!kgem_bo_can_map__cpu(&sna->kgem, priv->gpu_bo, true)) {
		DBG(("%s: no, cannot map through the CPU\n", __FUNCTION__));
		return false;
	}

	if (!sna_pixmap_move_area_to_gpu(pixmap, &region->extents,
					 MOVE_WRITE | (region->data ? MOVE_READ : 0)))
		return false;

	if ((priv->create & KGEM_CAN_CREATE_LARGE) == 0 &&
	    __kgem_bo_is_busy(&sna->kgem, priv->gpu_bo))
		return false;

	dst = kgem_bo_map__cpu(&sna->kgem, priv->gpu_bo);
	if (dst == NULL)
		return false;

	kgem_bo_sync__cpu(&sna->kgem, priv->gpu_bo);

	box = region_rects(region);
	n = region_num_rects(region);

	DBG(("%s: upload(%d, %d, %d, %d) x %d\n", __FUNCTION__, x, y, w, h, n));

	if (sigtrap_get())
		return false;

	if (priv->gpu_bo->tiling) {
		do {
			DBG(("%s: copy tiled box (%d, %d)->(%d, %d)x(%d, %d)\n",
			     __FUNCTION__,
			     box->x1 - x, box->y1 - y,
			     box->x1, box->y1,
			     box->x2 - box->x1, box->y2 - box->y1));

			assert(box->x2 > box->x1);
			assert(box->y2 > box->y1);

			assert(box->x1 >= 0);
			assert(box->y1 >= 0);
			assert(box->x2 <= pixmap->drawable.width);
			assert(box->y2 <= pixmap->drawable.height);

			assert(box->x1 - x >= 0);
			assert(box->y1 - y >= 0);
			assert(box->x2 - x <= w);
			assert(box->y2 - y <= h);

			memcpy_to_tiled_x(&sna->kgem, bits, dst,
					  pixmap->drawable.bitsPerPixel,
					  stride, priv->gpu_bo->pitch,
					  box->x1 - x, box->y1 - y,
					  box->x1, box->y1,
					  box->x2 - box->x1, box->y2 - box->y1);
			box++;
		} while (--n);
	} else {
		do {
			DBG(("%s: copy lined box (%d, %d)->(%d, %d)x(%d, %d)\n",
			     __FUNCTION__,
			     box->x1 - x, box->y1 - y,
			     box->x1, box->y1,
			     box->x2 - box->x1, box->y2 - box->y1));

			assert(box->x2 > box->x1);
			assert(box->y2 > box->y1);

			assert(box->x1 >= 0);
			assert(box->y1 >= 0);
			assert(box->x2 <= pixmap->drawable.width);
			assert(box->y2 <= pixmap->drawable.height);

			assert(box->x1 - x >= 0);
			assert(box->y1 - y >= 0);
			assert(box->x2 - x <= w);
			assert(box->y2 - y <= h);

			memcpy_blt(bits, dst,
				   pixmap->drawable.bitsPerPixel,
				   stride, priv->gpu_bo->pitch,
				   box->x1 - x, box->y1 - y,
				   box->x1, box->y1,
				   box->x2 - box->x1, box->y2 - box->y1);
			box++;
		} while (--n);

		if (!priv->shm) {
			assert(dst == MAP(priv->gpu_bo->map__cpu));
			pixmap->devPrivate.ptr = dst;
			pixmap->devKind = priv->gpu_bo->pitch;
			priv->mapped = MAPPED_CPU;
			assert_pixmap_map(pixmap, priv);
			priv->cpu = true;
		}
	}

	sigtrap_put();
	return true;
}

static bool
try_upload__inplace(PixmapPtr pixmap, RegionRec *region,
		    int x, int y, int w, int  h, char *bits, int stride)
{
	struct sna *sna = to_sna_from_pixmap(pixmap);
	struct sna_pixmap *priv = sna_pixmap(pixmap);
	bool ignore_cpu = false;
	bool replaces;
	const BoxRec *box;
	uint8_t *dst;
	int n;

	if (!USE_INPLACE)
		return false;

	assert(priv);

	if (priv->shm && priv->gpu_damage == NULL)
		return false;

	replaces = region_subsumes_pixmap(region, pixmap);

	DBG(("%s: bo? %d, can map? %d, replaces? %d\n", __FUNCTION__,
	     priv->gpu_bo != NULL,
	     priv->gpu_bo ? kgem_bo_can_map__cpu(&sna->kgem, priv->gpu_bo, true) : 0,
	     replaces));

	if (kgem_bo_discard_cache(priv->gpu_bo, true)) {
		DBG(("%s: discarding cached upload buffer\n", __FUNCTION__));
		assert(DAMAGE_IS_ALL(priv->cpu_damage));
		assert(priv->gpu_damage == NULL || DAMAGE_IS_ALL(priv->gpu_damage)); /* magical upload buffer */
		assert(!priv->pinned);
		assert(!priv->mapped);
		sna_damage_destroy(&priv->gpu_damage);
		kgem_bo_destroy(&sna->kgem, priv->gpu_bo);
		priv->gpu_bo = NULL;
	}

	if (priv->gpu_bo && replaces) {
		if (UNDO) kgem_bo_pair_undo(&sna->kgem, priv->gpu_bo, priv->cpu_bo);
		if (can_create_upload_tiled_x(sna, pixmap, priv, true) &&
		    (priv->cow ||
		     __kgem_bo_is_busy(&sna->kgem, priv->gpu_bo) ||
		     !kgem_bo_can_map__cpu(&sna->kgem, priv->gpu_bo, true))) {
			DBG(("%s: discarding unusable target bo (busy? %d, mappable? %d)\n", __FUNCTION__,
			     kgem_bo_is_busy(priv->gpu_bo),
			     kgem_bo_can_map__cpu(&sna->kgem, priv->gpu_bo, true)));
			sna_pixmap_free_gpu(sna, priv);
			ignore_cpu = true;
		}
	}
	assert(priv->gpu_bo == NULL || priv->gpu_bo->proxy == NULL);

	if (priv->cow ||
	    (priv->move_to_gpu && !sna_pixmap_discard_shadow_damage(priv, replaces ? NULL : region))) {
		DBG(("%s: no, has pending COW? %d or move-to-gpu? %d\n",
		     __FUNCTION__, priv->cow != NULL, priv->move_to_gpu != NULL));
		return false;
	}

	if (priv->gpu_damage &&
	    region_subsumes_damage(region, priv->gpu_damage)) {
		if (UNDO) kgem_bo_undo(&sna->kgem, priv->gpu_bo);
		if (can_create_upload_tiled_x(sna, pixmap, priv, priv->cpu_damage == NULL) &&
		    (__kgem_bo_is_busy(&sna->kgem, priv->gpu_bo) ||
		     !kgem_bo_can_map__cpu(&sna->kgem, priv->gpu_bo, true))) {
			DBG(("%s: discarding unusable partial target bo (busy? %d, mappable? %d)\n", __FUNCTION__,
			     kgem_bo_is_busy(priv->gpu_bo),
			     kgem_bo_can_map__cpu(&sna->kgem, priv->gpu_bo, true)));
			sna_pixmap_free_gpu(sna, priv);
			ignore_cpu = priv->cpu_damage == NULL;
			if (priv->ptr)
				sna_damage_all(&priv->cpu_damage, pixmap);
		}
	}

	if (priv->gpu_bo == NULL &&
	    !create_upload_tiled_x(sna, pixmap, priv, ignore_cpu))
		return false;

	DBG(("%s: tiling=%d\n", __FUNCTION__, priv->gpu_bo->tiling));
	switch (priv->gpu_bo->tiling) {
	case I915_TILING_Y:
		break;
	case I915_TILING_X:
		if (!sna->kgem.memcpy_to_tiled_x)
			break;
	default:
		if (try_upload__tiled_x(pixmap, region, x, y, w, h, bits, stride))
			goto done;
		break;
	}

	if (priv->gpu_damage == NULL && !box_inplace(pixmap, &region->extents)) {
		DBG(("%s: no, too small to bother with using the GTT\n", __FUNCTION__));
		return false;
	}

	if (!kgem_bo_can_map(&sna->kgem, priv->gpu_bo)) {
		DBG(("%s: no, cannot map through the CPU\n", __FUNCTION__));
		return false;
	}

	if (!sna_pixmap_move_area_to_gpu(pixmap, &region->extents,
					 MOVE_WRITE | (region->data ? MOVE_READ : 0)))
		return false;

	if ((priv->create & KGEM_CAN_CREATE_LARGE) == 0 &&
	    __kgem_bo_is_busy(&sna->kgem, priv->gpu_bo))
		return false;

	dst = kgem_bo_map(&sna->kgem, priv->gpu_bo);
	if (dst == NULL)
		return false;

	pixmap->devPrivate.ptr = dst;
	pixmap->devKind = priv->gpu_bo->pitch;
	priv->mapped = dst == MAP(priv->gpu_bo->map__cpu) ? MAPPED_CPU : MAPPED_GTT;
	assert(has_coherent_ptr(sna, priv, MOVE_WRITE));

	box = region_rects(region);
	n = region_num_rects(region);

	DBG(("%s: upload(%d, %d, %d, %d) x %d\n", __FUNCTION__, x, y, w, h, n));

	if (sigtrap_get())
		return false;

	do {
		DBG(("%s: copy lined box (%d, %d)->(%d, %d)x(%d, %d)\n",
		     __FUNCTION__,
		     box->x1 - x, box->y1 - y,
		     box->x1, box->y1,
		     box->x2 - box->x1, box->y2 - box->y1));

		assert(box->x2 > box->x1);
		assert(box->y2 > box->y1);

		assert(box->x1 >= 0);
		assert(box->y1 >= 0);
		assert(box->x2 <= pixmap->drawable.width);
		assert(box->y2 <= pixmap->drawable.height);

		assert(box->x1 - x >= 0);
		assert(box->y1 - y >= 0);
		assert(box->x2 - x <= w);
		assert(box->y2 - y <= h);

		memcpy_blt(bits, dst,
			   pixmap->drawable.bitsPerPixel,
			   stride, priv->gpu_bo->pitch,
			   box->x1 - x, box->y1 - y,
			   box->x1, box->y1,
			   box->x2 - box->x1, box->y2 - box->y1);
		box++;
	} while (--n);

	sigtrap_put();

done:
	if (!DAMAGE_IS_ALL(priv->gpu_damage)) {
		if (replaces) {
			sna_damage_all(&priv->gpu_damage, pixmap);
		} else {
			sna_damage_add(&priv->gpu_damage, region);
			sna_damage_reduce_all(&priv->gpu_damage, pixmap);
		}
		if (DAMAGE_IS_ALL(priv->gpu_damage))
			sna_damage_destroy(&priv->cpu_damage);
		else
			sna_damage_subtract(&priv->cpu_damage, region);

		if (priv->cpu_damage == NULL) {
			list_del(&priv->flush_list);
			sna_damage_all(&priv->gpu_damage, pixmap);
		}

		if (priv->shm)
			sna_add_flush_pixmap(sna, priv, priv->cpu_bo);
	}

	assert(!priv->clear);
	return true;
}

static bool
try_upload__blt(PixmapPtr pixmap, RegionRec *region,
		int x, int y, int w, int  h, char *bits, int stride)
{
	struct sna *sna = to_sna_from_pixmap(pixmap);
	struct sna_pixmap *priv;
	struct kgem_bo *src_bo;
	bool ok;

	if (!sna->kgem.has_userptr || !USE_USERPTR_UPLOADS)
		return false;

	priv = sna_pixmap(pixmap);
	assert(priv);
	assert(priv->gpu_bo);
	assert(priv->gpu_bo->proxy == NULL);

	if (priv->cpu_damage &&
	    (DAMAGE_IS_ALL(priv->cpu_damage) ||
	     sna_damage_contains_box__no_reduce(priv->cpu_damage,
						&region->extents)) &&
	    !box_inplace(pixmap, &region->extents)) {
		DBG(("%s: no, damage on CPU and too small\n", __FUNCTION__));
		return false;
	}

	src_bo = kgem_create_map(&sna->kgem, bits, stride * h, true);
	if (src_bo == NULL)
		return false;

	src_bo->pitch = stride;
	kgem_bo_mark_unreusable(src_bo);

	if (!sna_pixmap_move_area_to_gpu(pixmap, &region->extents,
					 MOVE_WRITE | MOVE_ASYNC_HINT | (region->data ? MOVE_READ : 0))) {
		kgem_bo_destroy(&sna->kgem, src_bo);
		return false;
	}

	DBG(("%s: upload(%d, %d, %d, %d) x %d through a temporary map\n",
	     __FUNCTION__, x, y, w, h, region_num_rects(region)));

	if (sigtrap_get() == 0) {
		ok = sna->render.copy_boxes(sna, GXcopy,
					    &pixmap->drawable, src_bo, -x, -y,
					    &pixmap->drawable, priv->gpu_bo, 0, 0,
					    region_rects(region),
					    region_num_rects(region),
					    COPY_LAST);
		sigtrap_put();
	} else
		ok = false;

	kgem_bo_sync__cpu(&sna->kgem, src_bo);
	assert(src_bo->rq == NULL);
	kgem_bo_destroy(&sna->kgem, src_bo);

	if (!ok) {
		DBG(("%s: copy failed!\n", __FUNCTION__));
		return false;
	}

	if (!DAMAGE_IS_ALL(priv->gpu_damage)) {
		assert(!priv->clear);
		if (region_subsumes_drawable(region, &pixmap->drawable)) {
			sna_damage_all(&priv->gpu_damage, pixmap);
		} else {
			sna_damage_add(&priv->gpu_damage, region);
			sna_damage_reduce_all(&priv->gpu_damage, pixmap);
		}
		if (DAMAGE_IS_ALL(priv->gpu_damage))
			sna_damage_destroy(&priv->cpu_damage);
		else
			sna_damage_subtract(&priv->cpu_damage, region);
		if (priv->cpu_damage == NULL) {
			list_del(&priv->flush_list);
			if (sna_pixmap_free_cpu(sna, priv, priv->cpu))
				sna_damage_all(&priv->gpu_damage, pixmap);
		}
	}
	priv->cpu = false;
	priv->clear = false;

	return true;
}

static bool ignore_cpu_damage(struct sna *sna, struct sna_pixmap *priv, const RegionRec *region)
{
	if (region_subsumes_pixmap(region, priv->pixmap))
		return true;

	if (priv->cpu_damage != NULL) {
		if (DAMAGE_IS_ALL(priv->cpu_damage))
			return false;

		if (!box_inplace(priv->pixmap, &region->extents))
			return false;

		if (sna_damage_contains_box__no_reduce(priv->cpu_damage, &region->extents))
			return false;
	}

	return priv->gpu_bo == NULL || !__kgem_bo_is_busy(&sna->kgem, priv->gpu_bo);

}

static bool
try_upload__fast(PixmapPtr pixmap, RegionRec *region,
		 int x, int y, int w, int  h, char *bits, int stride)
{
	struct sna *sna = to_sna_from_pixmap(pixmap);
	struct sna_pixmap *priv;

	if (wedged(sna))
		return false;

	priv = sna_pixmap(pixmap);
	if (priv == NULL)
		return false;

	if (ignore_cpu_damage(sna, priv, region)) {
		DBG(("%s: ignore existing cpu damage (if any)\n", __FUNCTION__));
		if (try_upload__inplace(pixmap, region, x, y, w, h, bits, stride))
			return true;
	}

	if (DAMAGE_IS_ALL(priv->cpu_damage) || priv->gpu_damage == NULL || priv->cpu) {
		DBG(("%s: no, no gpu damage\n", __FUNCTION__));
		return false;
	}

	assert(priv->gpu_bo);
	assert(priv->gpu_bo->proxy == NULL);

	if (try_upload__blt(pixmap, region, x, y, w, h, bits, stride))
		return true;

	if (try_upload__inplace(pixmap, region, x, y, w, h, bits, stride))
		return true;

	return false;
}

static bool
sna_put_zpixmap_blt(DrawablePtr drawable, GCPtr gc, RegionPtr region,
		    int x, int y, int w, int  h, char *bits, int stride)
{
	PixmapPtr pixmap = get_drawable_pixmap(drawable);
	unsigned int hint;
	const BoxRec *box;
	int16_t dx, dy;
	int n;

	assert_pixmap_contains_box(pixmap, RegionExtents(region));

	if (gc->alu != GXcopy)
		return false;

	if (drawable->depth < 8)
		return false;

	get_drawable_deltas(drawable, pixmap, &dx, &dy);
	x += dx + drawable->x;
	y += dy + drawable->y;
	assert(region->extents.x1 >= x);
	assert(region->extents.y1 >= y);
	assert(region->extents.x2 <= x + w);
	assert(region->extents.y2 <= y + h);

	if (try_upload__fast(pixmap, region, x, y, w, h, bits, stride))
		return true;

	hint = MOVE_WRITE;
	if (region_is_unclipped(region, pixmap->drawable.width, h) &&
	    (h+1)*stride > 65536) {
		DBG(("%s: segmented, unclipped large upload (%d bytes), marking WHOLE_HINT\n",
		     __FUNCTION__, h*stride));
		hint |= MOVE_WHOLE_HINT;
	}

	if (!sna_drawable_move_region_to_cpu(&pixmap->drawable, region, hint))
		return false;

	if (sigtrap_get())
		return false;

	/* Region is pre-clipped and translated into pixmap space */
	box = region_rects(region);
	n = region_num_rects(region);
	DBG(("%s: upload(%d, %d, %d, %d) x %d boxes\n", __FUNCTION__, x, y, w, h, n));
	do {
		DBG(("%s: copy box (%d, %d)->(%d, %d)x(%d, %d)\n",
		     __FUNCTION__,
		     box->x1 - x, box->y1 - y,
		     box->x1, box->y1,
		     box->x2 - box->x1, box->y2 - box->y1));

		assert(box->x2 > box->x1);
		assert(box->y2 > box->y1);

		assert(box->x1 >= 0);
		assert(box->y1 >= 0);
		assert(box->x2 <= pixmap->drawable.width);
		assert(box->y2 <= pixmap->drawable.height);

		assert(box->x1 - x >= 0);
		assert(box->y1 - y >= 0);
		assert(box->x2 - x <= w);
		assert(box->y2 - y <= h);

		assert(has_coherent_ptr(to_sna_from_pixmap(pixmap), sna_pixmap(pixmap), MOVE_WRITE));
		assert(pixmap->devKind);
		memcpy_blt(bits, pixmap->devPrivate.ptr,
			   pixmap->drawable.bitsPerPixel,
			   stride, pixmap->devKind,
			   box->x1 - x, box->y1 - y,
			   box->x1, box->y1,
			   box->x2 - box->x1, box->y2 - box->y1);
		box++;
	} while (--n);

	sigtrap_put();
	assert_pixmap_damage(pixmap);
	return true;
}

static inline uint8_t byte_reverse(uint8_t b)
{
	return ((b * 0x80200802ULL) & 0x0884422110ULL) * 0x0101010101ULL >> 32;
}

static inline uint8_t blt_depth(int depth)
{
	switch (depth) {
	case 8: return 0;
	case 15: return 0x2;
	case 16: return 0x1;
	default: return 0x3;
	}
}

static bool
sna_put_xybitmap_blt(DrawablePtr drawable, GCPtr gc, RegionPtr region,
		     int x, int y, int w, int  h, char *bits)
{
	PixmapPtr pixmap = get_drawable_pixmap(drawable);
	struct sna *sna = to_sna_from_pixmap(pixmap);
	struct sna_damage **damage;
	struct kgem_bo *bo;
	const BoxRec *box;
	int16_t dx, dy;
	int n;
	uint8_t rop = copy_ROP[gc->alu];

	bo = sna_drawable_use_bo(&pixmap->drawable, PREFER_GPU,
				 &region->extents, &damage);
	if (bo == NULL)
		return false;

	if (bo->tiling == I915_TILING_Y) {
		DBG(("%s: converting bo from Y-tiling\n", __FUNCTION__));
		assert(bo == __sna_pixmap_get_bo(pixmap));
		bo = sna_pixmap_change_tiling(pixmap, I915_TILING_X);
		if (bo == NULL) {
			DBG(("%s: fallback -- unable to change tiling\n",
			     __FUNCTION__));
			return false;
		}
	}

	assert_pixmap_contains_box(pixmap, RegionExtents(region));
	if (damage)
		sna_damage_add(damage, region);
	assert_pixmap_damage(pixmap);

	DBG(("%s: upload(%d, %d, %d, %d)\n", __FUNCTION__, x, y, w, h));

	get_drawable_deltas(drawable, pixmap, &dx, &dy);
	x += dx + drawable->x;
	y += dy + drawable->y;

	kgem_set_mode(&sna->kgem, KGEM_BLT, bo);

	/* Region is pre-clipped and translated into pixmap space */
	box = region_rects(region);
	n = region_num_rects(region);
	do {
		int bx1 = (box->x1 - x) & ~7;
		int bx2 = (box->x2 - x + 7) & ~7;
		int bw = (bx2 - bx1)/8;
		int bh = box->y2 - box->y1;
		int bstride = ALIGN(bw, 2);
		struct kgem_bo *upload;
		void *ptr;

		if (!kgem_check_batch(&sna->kgem, 10) ||
		    !kgem_check_bo_fenced(&sna->kgem, bo) ||
		    !kgem_check_reloc_and_exec(&sna->kgem, 2)) {
			kgem_submit(&sna->kgem);
			if (!kgem_check_bo_fenced(&sna->kgem, bo))
				return false;
			_kgem_set_mode(&sna->kgem, KGEM_BLT);
		}

		upload = kgem_create_buffer(&sna->kgem,
					    bstride*bh,
					    KGEM_BUFFER_WRITE_INPLACE,
					    &ptr);
		if (!upload)
			break;


		if (sigtrap_get() == 0) {
			int src_stride = BitmapBytePad(w);
			uint8_t *dst = ptr;
			uint8_t *src = (uint8_t*)bits + (box->y1 - y) * src_stride + bx1/8;
			uint32_t *b;

			bstride -= bw;
			src_stride -= bw;

			do {
				int i = bw;
				assert(src >= (uint8_t *)bits);
				do {
					*dst++ = byte_reverse(*src++);
				} while (--i);
				assert(src <= (uint8_t *)bits + BitmapBytePad(w) * h);
				assert(dst <= (uint8_t *)ptr + kgem_bo_size(upload));
				dst += bstride;
				src += src_stride;
			} while (--bh);

			assert(sna->kgem.mode == KGEM_BLT);
			if (sna->kgem.gen >= 0100) {
				b = sna->kgem.batch + sna->kgem.nbatch;
				b[0] = XY_MONO_SRC_COPY | 3 << 20 | 8;
				b[0] |= ((box->x1 - x) & 7) << 17;
				b[1] = bo->pitch;
				if (bo->tiling) {
					b[0] |= BLT_DST_TILED;
					b[1] >>= 2;
				}
				b[1] |= blt_depth(drawable->depth) << 24;
				b[1] |= rop << 16;
				b[2] = box->y1 << 16 | box->x1;
				b[3] = box->y2 << 16 | box->x2;
				*(uint64_t *)(b+4) =
					kgem_add_reloc64(&sna->kgem, sna->kgem.nbatch + 4, bo,
							I915_GEM_DOMAIN_RENDER << 16 |
							I915_GEM_DOMAIN_RENDER |
							KGEM_RELOC_FENCED,
							0);
				*(uint64_t *)(b+6) =
					kgem_add_reloc64(&sna->kgem, sna->kgem.nbatch + 6, upload,
							I915_GEM_DOMAIN_RENDER << 16 |
							KGEM_RELOC_FENCED,
							0);
				b[8] = gc->bgPixel;
				b[9] = gc->fgPixel;

				sna->kgem.nbatch += 10;
			} else {
				b = sna->kgem.batch + sna->kgem.nbatch;
				b[0] = XY_MONO_SRC_COPY | 3 << 20 | 6;
				b[0] |= ((box->x1 - x) & 7) << 17;
				b[1] = bo->pitch;
				if (sna->kgem.gen >= 040 && bo->tiling) {
					b[0] |= BLT_DST_TILED;
					b[1] >>= 2;
				}
				b[1] |= blt_depth(drawable->depth) << 24;
				b[1] |= rop << 16;
				b[2] = box->y1 << 16 | box->x1;
				b[3] = box->y2 << 16 | box->x2;
				b[4] = kgem_add_reloc(&sna->kgem, sna->kgem.nbatch + 4, bo,
						I915_GEM_DOMAIN_RENDER << 16 |
						I915_GEM_DOMAIN_RENDER |
						KGEM_RELOC_FENCED,
						0);
				b[5] = kgem_add_reloc(&sna->kgem, sna->kgem.nbatch + 5, upload,
						I915_GEM_DOMAIN_RENDER << 16 |
						KGEM_RELOC_FENCED,
						0);
				b[6] = gc->bgPixel;
				b[7] = gc->fgPixel;

				sna->kgem.nbatch += 8;
			}
			sigtrap_put();
		}
		kgem_bo_destroy(&sna->kgem, upload);

		box++;
	} while (--n);

	sna->blt_state.fill_bo = 0;
	return true;
}

static bool
sna_put_xypixmap_blt(DrawablePtr drawable, GCPtr gc, RegionPtr region,
		     int x, int y, int w, int  h, int left,char *bits)
{
	PixmapPtr pixmap = get_drawable_pixmap(drawable);
	struct sna *sna = to_sna_from_pixmap(pixmap);
	struct sna_damage **damage;
	struct kgem_bo *bo;
	int16_t dx, dy;
	unsigned i, skip;

	if (gc->alu != GXcopy)
		return false;

	bo = sna_drawable_use_bo(&pixmap->drawable, PREFER_GPU,
				 &region->extents, &damage);
	if (bo == NULL)
		return false;

	if (bo->tiling == I915_TILING_Y) {
		DBG(("%s: converting bo from Y-tiling\n", __FUNCTION__));
		assert(bo == __sna_pixmap_get_bo(pixmap));
		bo = sna_pixmap_change_tiling(pixmap, I915_TILING_X);
		if (bo == NULL) {
			DBG(("%s: fallback -- unable to change tiling\n",
			     __FUNCTION__));
			return false;
		}
	}

	assert_pixmap_contains_box(pixmap, RegionExtents(region));
	if (damage)
		sna_damage_add(damage, region);
	assert_pixmap_damage(pixmap);

	DBG(("%s: upload(%d, %d, %d, %d)\n", __FUNCTION__, x, y, w, h));

	get_drawable_deltas(drawable, pixmap, &dx, &dy);
	x += dx + drawable->x;
	y += dy + drawable->y;

	kgem_set_mode(&sna->kgem, KGEM_BLT, bo);

	skip = h * BitmapBytePad(w + left);
	for (i = 1 << (gc->depth-1); i; i >>= 1, bits += skip) {
		const BoxRec *box = region_rects(region);
		int n = region_num_rects(region);

		if ((gc->planemask & i) == 0)
			continue;

		/* Region is pre-clipped and translated into pixmap space */
		do {
			int bx1 = (box->x1 - x) & ~7;
			int bx2 = (box->x2 - x + 7) & ~7;
			int bw = (bx2 - bx1)/8;
			int bh = box->y2 - box->y1;
			int bstride = ALIGN(bw, 2);
			struct kgem_bo *upload;
			void *ptr;

			if (!kgem_check_batch(&sna->kgem, 14) ||
			    !kgem_check_bo_fenced(&sna->kgem, bo) ||
			    !kgem_check_reloc_and_exec(&sna->kgem, 2)) {
				kgem_submit(&sna->kgem);
				if (!kgem_check_bo_fenced(&sna->kgem, bo))
					return false;
				_kgem_set_mode(&sna->kgem, KGEM_BLT);
			}

			upload = kgem_create_buffer(&sna->kgem,
						    bstride*bh,
						    KGEM_BUFFER_WRITE_INPLACE,
						    &ptr);
			if (!upload)
				break;

			if (sigtrap_get() == 0) {
				int src_stride = BitmapBytePad(w);
				uint8_t *src = (uint8_t*)bits + (box->y1 - y) * src_stride + bx1/8;
				uint8_t *dst = ptr;
				uint32_t *b;

				bstride -= bw;
				src_stride -= bw;
				do {
					int j = bw;
					assert(src >= (uint8_t *)bits);
					do {
						*dst++ = byte_reverse(*src++);
					} while (--j);
					assert(src <= (uint8_t *)bits + BitmapBytePad(w) * h);
					assert(dst <= (uint8_t *)ptr + kgem_bo_size(upload));
					dst += bstride;
					src += src_stride;
				} while (--bh);

				assert(sna->kgem.mode == KGEM_BLT);
				if (sna->kgem.gen >= 0100) {
					assert(sna->kgem.mode == KGEM_BLT);
					b = sna->kgem.batch + sna->kgem.nbatch;
					b[0] = XY_FULL_MONO_PATTERN_MONO_SRC_BLT | 3 << 20 | 12;
					b[0] |= ((box->x1 - x) & 7) << 17;
					b[1] = bo->pitch;
					if (bo->tiling) {
						b[0] |= BLT_DST_TILED;
						b[1] >>= 2;
					}
					b[1] |= 1 << 31; /* solid pattern */
					b[1] |= blt_depth(drawable->depth) << 24;
					b[1] |= 0xce << 16; /* S or (D and !P) */
					b[2] = box->y1 << 16 | box->x1;
					b[3] = box->y2 << 16 | box->x2;
					*(uint64_t *)(b+4) =
						kgem_add_reloc64(&sna->kgem, sna->kgem.nbatch + 4, bo,
								I915_GEM_DOMAIN_RENDER << 16 |
								I915_GEM_DOMAIN_RENDER |
								KGEM_RELOC_FENCED,
								0);
					*(uint64_t *)(b+6) =
						kgem_add_reloc64(&sna->kgem, sna->kgem.nbatch + 6, upload,
								I915_GEM_DOMAIN_RENDER << 16 |
								KGEM_RELOC_FENCED,
								0);
					b[8] = 0;
					b[9] = i;
					b[10] = i;
					b[11] = i;
					b[12] = -1;
					b[13] = -1;
					sna->kgem.nbatch += 14;
				} else {
					b = sna->kgem.batch + sna->kgem.nbatch;
					b[0] = XY_FULL_MONO_PATTERN_MONO_SRC_BLT | 3 << 20 | 10;
					b[0] |= ((box->x1 - x) & 7) << 17;
					b[1] = bo->pitch;
					if (sna->kgem.gen >= 040 && bo->tiling) {
						b[0] |= BLT_DST_TILED;
						b[1] >>= 2;
					}
					b[1] |= 1 << 31; /* solid pattern */
					b[1] |= blt_depth(drawable->depth) << 24;
					b[1] |= 0xce << 16; /* S or (D and !P) */
					b[2] = box->y1 << 16 | box->x1;
					b[3] = box->y2 << 16 | box->x2;
					b[4] = kgem_add_reloc(&sna->kgem, sna->kgem.nbatch + 4, bo,
							I915_GEM_DOMAIN_RENDER << 16 |
							I915_GEM_DOMAIN_RENDER |
							KGEM_RELOC_FENCED,
							0);
					b[5] = kgem_add_reloc(&sna->kgem, sna->kgem.nbatch + 5, upload,
							I915_GEM_DOMAIN_RENDER << 16 |
							KGEM_RELOC_FENCED,
							0);
					b[6] = 0;
					b[7] = i;
					b[8] = i;
					b[9] = i;
					b[10] = -1;
					b[11] = -1;
					sna->kgem.nbatch += 12;
				}
				sigtrap_put();
			}
			kgem_bo_destroy(&sna->kgem, upload);

			box++;
		} while (--n);
	}

	sna->blt_state.fill_bo = 0;
	return true;
}

static void
sna_put_image(DrawablePtr drawable, GCPtr gc, int depth,
	      int x, int y, int w, int h, int left, int format,
	      char *bits)
{
	PixmapPtr pixmap = get_drawable_pixmap(drawable);
	struct sna *sna = to_sna_from_pixmap(pixmap);
	struct sna_pixmap *priv = sna_pixmap(pixmap);
	RegionRec region;
	int16_t dx, dy;

	DBG(("%s((%d, %d)x(%d, %d), depth=%d, format=%d)\n",
	     __FUNCTION__, x, y, w, h, depth, format));

	if (w == 0 || h == 0)
		return;

	region.extents.x1 = x + drawable->x;
	region.extents.y1 = y + drawable->y;
	region.extents.x2 = region.extents.x1 + w;
	region.extents.y2 = region.extents.y1 + h;
	region.data = NULL;

	if (!region_is_singular(gc->pCompositeClip) ||
	    gc->pCompositeClip->extents.x1 > region.extents.x1 ||
	    gc->pCompositeClip->extents.y1 > region.extents.y1 ||
	    gc->pCompositeClip->extents.x2 < region.extents.x2 ||
	    gc->pCompositeClip->extents.y2 < region.extents.y2) {
		if (!RegionIntersect(&region, &region, gc->pCompositeClip) ||
		    box_empty(&region.extents))
			return;
	}

	if (get_drawable_deltas(drawable, pixmap, &dx, &dy))
		RegionTranslate(&region, dx, dy);

	if (priv == NULL) {
		DBG(("%s: fallback -- unattached(%d, %d, %d, %d)\n",
		     __FUNCTION__, x, y, w, h));
		goto fallback;
	}

	if (FORCE_FALLBACK)
		goto fallback;

	if (wedged(sna))
		goto fallback;

	if (!ACCEL_PUT_IMAGE)
		goto fallback;

	switch (format) {
	case ZPixmap:
		if (!PM_IS_SOLID(drawable, gc->planemask))
			goto fallback;

		if (sna_put_zpixmap_blt(drawable, gc, &region,
					x, y, w, h,
					bits, PixmapBytePad(w, depth)))
			return;
		break;

	case XYBitmap:
		if (!PM_IS_SOLID(drawable, gc->planemask))
			goto fallback;

		if (sna_put_xybitmap_blt(drawable, gc, &region,
					 x, y, w, h,
					 bits))
			return;
		break;

	case XYPixmap:
		if (sna_put_xypixmap_blt(drawable, gc, &region,
					 x, y, w, h, left,
					 bits))
			return;
		break;

	default:
		return;
	}

fallback:
	DBG(("%s: fallback\n", __FUNCTION__));
	RegionTranslate(&region, -dx, -dy);

	if (!sna_gc_move_to_cpu(gc, drawable, &region))
		goto out;
	if (!sna_drawable_move_region_to_cpu(drawable, &region,
					      format == XYPixmap ?
					      MOVE_READ | MOVE_WRITE :
					      drawable_gc_flags(drawable, gc, false)))
		goto out;

	if (sigtrap_get() == 0) {
		DBG(("%s: fbPutImage(%d, %d, %d, %d)\n",
		     __FUNCTION__, x, y, w, h));
		fbPutImage(drawable, gc, depth, x, y, w, h, left, format, bits);
		FALLBACK_FLUSH(drawable);
		sigtrap_put();
	}
out:
	sna_gc_move_to_gpu(gc);
	RegionUninit(&region);
}

static bool
source_contains_region(struct sna_damage *damage,
		       const RegionRec *region, int16_t dx, int16_t dy)
{
	BoxRec box;

	if (DAMAGE_IS_ALL(damage))
		return true;

	if (damage == NULL)
		return false;

	box = region->extents;
	box.x1 += dx;
	box.x2 += dx;
	box.y1 += dy;
	box.y2 += dy;
	return sna_damage_contains_box__no_reduce(damage, &box);
}

static bool
move_to_gpu(PixmapPtr pixmap, struct sna_pixmap *priv,
	    RegionRec *region, int16_t dx, int16_t dy,
	    uint8_t alu, bool dst_is_gpu)
{
	int w = region->extents.x2 - region->extents.x1;
	int h = region->extents.y2 - region->extents.y1;
	int count;

	assert_pixmap_map(pixmap, priv);
	if (DAMAGE_IS_ALL(priv->gpu_damage)) {
		assert(priv->gpu_bo);
		return true;
	}

	if (dst_is_gpu && priv->cpu_bo && priv->cpu_damage) {
		DBG(("%s: can use CPU bo? cpu_damage=%d, gpu_damage=%d, cpu hint=%d\n",
		     __FUNCTION__,
		     priv->cpu_damage ? DAMAGE_IS_ALL(priv->cpu_damage) ? -1 : 1 : 0,
		     priv->gpu_damage ? DAMAGE_IS_ALL(priv->gpu_damage) ? -1 : 1 : 0,
		     priv->cpu));
		if (DAMAGE_IS_ALL(priv->cpu_damage) || priv->gpu_damage == NULL)
			return false;

		if (priv->cpu &&
		    source_contains_region(priv->cpu_damage, region, dx, dy))
			return false;
	}

	if (priv->gpu_bo) {
		DBG(("%s: has gpu bo (cpu damage?=%d, cpu=%d, gpu tiling=%d)\n",
		     __FUNCTION__,
		     priv->cpu_damage ? DAMAGE_IS_ALL(priv->cpu_damage) ? -1 : 1 : 0,
		     priv->cpu, priv->gpu_bo->tiling));

		if (priv->cpu_damage == NULL)
			return true;

		if (alu != GXcopy)
			return true;

		if (!priv->cpu)
			return true;

		if (priv->gpu_bo->tiling)
			return true;

		RegionTranslate(region, dx, dy);
		count = region_subsumes_damage(region, priv->cpu_damage);
		RegionTranslate(region, -dx, -dy);
		if (count)
			return true;
	} else {
		if ((priv->create & KGEM_CAN_CREATE_GPU) == 0)
			return false;
		if (priv->shm)
			return false;
	}

	count = priv->source_count++;
	if (priv->cpu_bo) {
		if (priv->cpu_bo->flush && count > SOURCE_BIAS)
			return true;

		if (sna_pixmap_default_tiling(to_sna_from_pixmap(pixmap), pixmap) == I915_TILING_NONE)
			return false;

		if (priv->cpu)
			return false;

		return count > SOURCE_BIAS;
	} else {
		if (w == pixmap->drawable.width && h == pixmap->drawable.height)
			return count > SOURCE_BIAS;

		return count * w*h >= (SOURCE_BIAS+2) * (int)pixmap->drawable.width * pixmap->drawable.height;
	}
}

static const BoxRec *
reorder_boxes(const BoxRec *box, int n, int dx, int dy)
{
	const BoxRec *next, *base;
	BoxRec *new;

	DBG(("%s x %d dx=%d, dy=%d\n", __FUNCTION__, n, dx, dy));

	if (dy <= 0 && dx <= 0) {
		BoxRec *tmp;

		new = malloc(sizeof(BoxRec) * n);
		if (new == NULL)
			return NULL;

		tmp = new;
		next = box + n;
		do {
			*tmp++ = *--next;
		} while (next != box);
	} else if (dy < 0) {
		new = malloc(sizeof(BoxRec) * n);
		if (new == NULL)
			return NULL;

		base = next = box + n - 1;
		while (base >= box) {
			const BoxRec *tmp;

			while (next >= box && base->y1 == next->y1)
				next--;
			tmp = next + 1;
			while (tmp <= base)
				*new++ = *tmp++;
			base = next;
		}
		new -= n;
	} else {
		new = malloc(sizeof(BoxRec) * n);
		if (!new)
			return NULL;

		base = next = box;
		while (base < box + n) {
			const BoxRec *tmp;

			while (next < box + n && next->y1 == base->y1)
				next++;
			tmp = next;
			while (tmp != base)
				*new++ = *--tmp;
			base = next;
		}
		new -= n;
	}

	return new;
}

static void
sna_self_copy_boxes(DrawablePtr src, DrawablePtr dst, GCPtr gc,
		    RegionPtr region,int dx, int dy,
		    Pixel bitplane, void *closure)
{
	PixmapPtr pixmap = get_drawable_pixmap(src);
	struct sna *sna = to_sna_from_pixmap(pixmap);
	struct sna_pixmap *priv = sna_pixmap(pixmap);
	const BoxRec *box = region_rects(region);
	int n = region_num_rects(region);
	int alu = gc ? gc->alu : GXcopy;
	int16_t tx, ty, sx, sy;

	assert(pixmap == get_drawable_pixmap(dst));

	assert(region_num_rects(region));
	if (((dx | dy) == 0 && alu == GXcopy))
		return;

	if (n > 1 && (dx | dy) < 0) {
		box = reorder_boxes(box, n, dx, dy);
		if (box == NULL)
			return;
	}

	DBG(("%s (boxes=%dx[(%d, %d), (%d, %d)...], src=+(%d, %d), alu=%d, pix.size=%dx%d)\n",
	     __FUNCTION__, n,
	     region->extents.x1, region->extents.y1,
	     region->extents.x2, region->extents.y2,
	     dx, dy, alu,
	     pixmap->drawable.width, pixmap->drawable.height));

	get_drawable_deltas(dst, pixmap, &tx, &ty);
	get_drawable_deltas(src, pixmap, &sx, &sy);
	sx += dx;
	sy += dy;

	if (priv == NULL || DAMAGE_IS_ALL(priv->cpu_damage)) {
		DBG(("%s: unattached, or all damaged on CPU\n", __FUNCTION__));
		goto fallback;
	}

	if (priv->gpu_damage || (priv->cpu_damage == NULL && priv->gpu_bo)) {
		assert(priv->gpu_bo);

		if (alu == GXcopy && priv->clear)
			goto free_boxes;

		assert(priv->gpu_bo->proxy == NULL);
		if (!sna_pixmap_move_to_gpu(pixmap, MOVE_WRITE | MOVE_READ | MOVE_ASYNC_HINT)) {
			DBG(("%s: fallback - not a pure copy and failed to move dst to GPU\n",
			     __FUNCTION__));
			goto fallback;
		}
		assert(priv->cpu_damage == NULL);

		if (!sna->render.copy_boxes(sna, alu,
					    &pixmap->drawable, priv->gpu_bo, sx, sy,
					    &pixmap->drawable, priv->gpu_bo, tx, ty,
					    box, n, 0)) {
			DBG(("%s: fallback - accelerated copy boxes failed\n",
			     __FUNCTION__));
			goto fallback;
		}

		if (!DAMAGE_IS_ALL(priv->gpu_damage)) {
			assert(!priv->clear);
			if (sna_pixmap_free_cpu(sna, priv, false)) {
				sna_damage_all(&priv->gpu_damage, pixmap);
			} else {
				RegionTranslate(region, tx, ty);
				sna_damage_add(&priv->gpu_damage, region);
			}
		}
		assert_pixmap_damage(pixmap);
	} else {
fallback:
		DBG(("%s: fallback\n", __FUNCTION__));
		if (!sna_pixmap_move_to_cpu(pixmap, MOVE_READ | MOVE_WRITE))
			goto free_boxes;

		if (alu == GXcopy && pixmap->drawable.bitsPerPixel >= 8) {
			assert(pixmap->devKind);
			if (sigtrap_get() == 0) {
				FbBits *dst_bits, *src_bits;
				int stride = pixmap->devKind;
				int bpp = pixmap->drawable.bitsPerPixel;
				int i;

				dst_bits = (FbBits *)
					((char *)pixmap->devPrivate.ptr +
					 ty * stride + tx * bpp / 8);
				src_bits = (FbBits *)
					((char *)pixmap->devPrivate.ptr +
					 sy * stride + sx * bpp / 8);

				for (i = 0; i < n; i++)
					memmove_box(src_bits, dst_bits,
						    bpp, stride, box+i,
						    dx, dy);
				sigtrap_put();
			}
		} else {
			if (gc && !sna_gc_move_to_cpu(gc, dst, region))
				goto out;

			if (sigtrap_get() == 0) {
				miCopyRegion(src, dst, gc,
					     region, dx, dy,
					     fbCopyNtoN, 0, NULL);
				sigtrap_put();
			}

			if (gc)
out:
				sna_gc_move_to_gpu(gc);
		}
	}

free_boxes:
	if (box != region_rects(region))
		free((void *)box);
}

static inline bool
sna_pixmap_is_gpu(PixmapPtr pixmap)
{
	struct sna_pixmap *priv = sna_pixmap(pixmap);

	if (priv == NULL || priv->clear)
		return false;

	if (DAMAGE_IS_ALL(priv->gpu_damage) ||
	    (priv->gpu_bo && kgem_bo_is_busy(priv->gpu_bo) && !priv->gpu_bo->proxy))
		return true;

	return priv->cpu_bo && kgem_bo_is_busy(priv->cpu_bo);
}

static int
copy_prefer_gpu(struct sna *sna,
		struct sna_pixmap *dst_priv,
		struct sna_pixmap *src_priv,
		RegionRec *region,
		int16_t dx, int16_t dy)
{
	assert(dst_priv);

	if (wedged(sna) && !dst_priv->pinned)
		return 0;

	if (src_priv == NULL) {
		DBG(("%s: source unattached, use cpu\n", __FUNCTION__));
		return 0;
	}

	if (src_priv->clear) {
		DBG(("%s: source is clear, don't force use of GPU\n", __FUNCTION__));
		return 0;
	}

	if (src_priv->gpu_damage &&
	    !source_contains_region(src_priv->cpu_damage, region, dx, dy)) {
		DBG(("%s: source has gpu damage, force gpu? %d\n",
		     __FUNCTION__, src_priv->cpu_damage == NULL));
		assert(src_priv->gpu_bo);
		return src_priv->cpu_damage ? PREFER_GPU : PREFER_GPU | FORCE_GPU;
	}

	if (src_priv->cpu_bo && kgem_bo_is_busy(src_priv->cpu_bo)) {
		DBG(("%s: source has busy CPU bo, force gpu\n", __FUNCTION__));
		return PREFER_GPU | FORCE_GPU;
	}

	if (source_contains_region(src_priv->cpu_damage, region, dx, dy))
		return src_priv->cpu_bo && kgem_is_idle(&sna->kgem);

	DBG(("%s: source has GPU bo? %d\n",
	     __FUNCTION__, src_priv->gpu_bo != NULL));
	return src_priv->gpu_bo != NULL;
}

static bool use_shm_bo(struct sna *sna,
		       struct kgem_bo *bo,
		       struct sna_pixmap *priv,
		       int alu, bool replaces)
{
	if (priv == NULL || priv->cpu_bo == NULL) {
		DBG(("%s: no, not attached\n", __FUNCTION__));
		return false;
	}

	if (!priv->shm && !priv->cpu) {
		DBG(("%s: yes, ordinary CPU bo\n", __FUNCTION__));
		return true;
	}

	if (alu != GXcopy) {
		DBG(("%s: yes, complex alu=%d\n", __FUNCTION__, alu));
		return true;
	}

	if (!replaces && __kgem_bo_is_busy(&sna->kgem, bo)) {
		DBG(("%s: yes, dst is busy\n", __FUNCTION__));
		return true;
	}

	if (priv->cpu_bo->needs_flush &&
	    __kgem_bo_is_busy(&sna->kgem, priv->cpu_bo)) {
		DBG(("%s: yes, src is busy\n", __FUNCTION__));
		return true;
	}

	return false;
}

static bool
sna_damage_contains_box__no_reduce__offset(struct sna_damage *damage,
					   const BoxRec *extents,
					   int16_t dx, int16_t dy)
{
	BoxRec _extents;

	if (dx | dy) {
		_extents.x1 = extents->x1 + dx;
		_extents.x2 = extents->x2 + dx;
		_extents.y1 = extents->y1 + dy;
		_extents.y2 = extents->y2 + dy;
		extents = &_extents;
	}

	return sna_damage_contains_box__no_reduce(damage, extents);
}

static bool
sna_copy_boxes__inplace(struct sna *sna, RegionPtr region, int alu,
			PixmapPtr src_pixmap, struct sna_pixmap *src_priv,
			int dx, int dy,
			PixmapPtr dst_pixmap, struct sna_pixmap *dst_priv,
			bool replaces)
{
	const BoxRec *box;
	char *ptr;
	int n;

	assert(src_pixmap->drawable.bitsPerPixel == dst_pixmap->drawable.bitsPerPixel);

	if (alu != GXcopy) {
		DBG(("%s - no, complex alu [%d]\n", __FUNCTION__, alu));
		return false;
	}

	if (!USE_INPLACE) {
		DBG(("%s - no, compile time disabled\n", __FUNCTION__));
		return false;
	}

	if (dst_priv == src_priv) {
		DBG(("%s - no, dst == src\n", __FUNCTION__));
		return false;
	}

	if (src_priv == NULL || src_priv->gpu_bo == NULL) {
		if (dst_priv && dst_priv->gpu_bo)
			goto upload_inplace;

		DBG(("%s - no, no src or dst GPU bo\n", __FUNCTION__));
		return false;
	}

	switch (src_priv->gpu_bo->tiling) {
	case I915_TILING_Y:
		DBG(("%s - no, bad src tiling [Y]\n", __FUNCTION__));
		return false;
	case I915_TILING_X:
		if (!sna->kgem.memcpy_from_tiled_x) {
			DBG(("%s - no, bad src tiling [X]\n", __FUNCTION__));
			return false;
		}
	default:
		break;
	}

	if (src_priv->move_to_gpu && !src_priv->move_to_gpu(sna, src_priv, MOVE_READ)) {
		DBG(("%s - no, pending src move-to-gpu failed\n", __FUNCTION__));
		return false;
	}

	if (!kgem_bo_can_map__cpu(&sna->kgem, src_priv->gpu_bo, FORCE_FULL_SYNC)) {
		DBG(("%s - no, cannot map src for reads into the CPU\n", __FUNCTION__));
		return false;
	}

	if (src_priv->gpu_damage == NULL ||
	    !(DAMAGE_IS_ALL(src_priv->gpu_damage) ||
	      sna_damage_contains_box__no_reduce__offset(src_priv->gpu_damage,
							 &region->extents,
							 dx, dy))) {
		DBG(("%s - no, src is not damaged on the GPU\n", __FUNCTION__));
		return false;
	}

	assert(sna_damage_contains_box__offset(&src_priv->gpu_damage, &region->extents, dx, dy) == PIXMAN_REGION_IN);
	assert(sna_damage_contains_box__offset(&src_priv->cpu_damage, &region->extents, dx, dy) == PIXMAN_REGION_OUT);

	ptr = kgem_bo_map__cpu(&sna->kgem, src_priv->gpu_bo);
	if (ptr == NULL) {
		DBG(("%s - no, map failed\n", __FUNCTION__));
		return false;
	}

	if (dst_priv &&
	    !sna_drawable_move_region_to_cpu(&dst_pixmap->drawable,
					     region, MOVE_WRITE | MOVE_INPLACE_HINT)) {
		DBG(("%s - no, dst sync failed\n", __FUNCTION__));
		return false;
	}

	kgem_bo_sync__cpu_full(&sna->kgem, src_priv->gpu_bo, FORCE_FULL_SYNC);

	box = region_rects(region);
	n = region_num_rects(region);
	if (src_priv->gpu_bo->tiling) {
		DBG(("%s: copy from a tiled CPU map\n", __FUNCTION__));
		assert(dst_pixmap->devKind);
		do {
			memcpy_from_tiled_x(&sna->kgem, ptr, dst_pixmap->devPrivate.ptr,
					    src_pixmap->drawable.bitsPerPixel,
					    src_priv->gpu_bo->pitch,
					    dst_pixmap->devKind,
					    box->x1 + dx, box->y1 + dy,
					    box->x1, box->y1,
					    box->x2 - box->x1, box->y2 - box->y1);
			box++;
		} while (--n);
	} else {
		DBG(("%s: copy from a linear CPU map\n", __FUNCTION__));
		assert(dst_pixmap->devKind);
		do {
			memcpy_blt(ptr, dst_pixmap->devPrivate.ptr,
				   src_pixmap->drawable.bitsPerPixel,
				   src_priv->gpu_bo->pitch,
				   dst_pixmap->devKind,
				   box->x1 + dx, box->y1 + dy,
				   box->x1, box->y1,
				   box->x2 - box->x1, box->y2 - box->y1);
			box++;
		} while (--n);

		if (!src_priv->shm) {
			assert(ptr == MAP(src_priv->gpu_bo->map__cpu));
			src_pixmap->devPrivate.ptr = ptr;
			src_pixmap->devKind = src_priv->gpu_bo->pitch;
			src_priv->mapped = MAPPED_CPU;
			assert_pixmap_map(src_pixmap, src_priv);
			src_priv->cpu = true;
		}
	}

	return true;

upload_inplace:
	switch (dst_priv->gpu_bo->tiling) {
	case I915_TILING_Y:
		DBG(("%s - no, bad dst tiling [Y]\n", __FUNCTION__));
		return false;
	case I915_TILING_X:
		if (!sna->kgem.memcpy_to_tiled_x) {
			DBG(("%s - no, bad dst tiling [X]\n", __FUNCTION__));
			return false;
		}
	default:
		break;
	}

	if (dst_priv->move_to_gpu) {
		DBG(("%s - no, pending dst move-to-gpu\n", __FUNCTION__));
		return false;
	}

	if (!kgem_bo_can_map__cpu(&sna->kgem, dst_priv->gpu_bo, true) ||
	    __kgem_bo_is_busy(&sna->kgem, dst_priv->gpu_bo)) {
		if (replaces && !dst_priv->pinned) {
			unsigned create;
			struct kgem_bo *bo;

			create = CREATE_CPU_MAP | CREATE_INACTIVE;
			if (dst_priv->gpu_bo->scanout)
				create |= CREATE_SCANOUT;

			bo = kgem_create_2d(&sna->kgem,
					    dst_pixmap->drawable.width,
					    dst_pixmap->drawable.height,
					    dst_pixmap->drawable.bitsPerPixel,
					    dst_priv->gpu_bo->tiling,
					    create);
			if (bo == NULL)
				return false;

			sna_pixmap_unmap(dst_pixmap, dst_priv);
			kgem_bo_destroy(&sna->kgem, dst_priv->gpu_bo);
			dst_priv->gpu_bo = bo;
		} else {
			DBG(("%s - no, dst is busy\n", __FUNCTION__));
			return false;
		}

		if (!kgem_bo_can_map__cpu(&sna->kgem, dst_priv->gpu_bo, true)) {
			DBG(("%s - no, cannot map dst for reads into the CPU\n", __FUNCTION__));
			return false;
		}
	}

	if (src_priv &&
	    !sna_drawable_move_region_to_cpu(&src_pixmap->drawable,
					     region, MOVE_READ)) {
		DBG(("%s - no, src sync failed\n", __FUNCTION__));
		return false;
	}

	ptr = kgem_bo_map__cpu(&sna->kgem, dst_priv->gpu_bo);
	if (ptr == NULL) {
		DBG(("%s - no, map failed\n", __FUNCTION__));
		return false;
	}

	kgem_bo_sync__cpu(&sna->kgem, dst_priv->gpu_bo);

	if (!DAMAGE_IS_ALL(dst_priv->gpu_damage)) {
		assert(!dst_priv->clear);
		sna_damage_add(&dst_priv->gpu_damage, region);
		if (sna_damage_is_all(&dst_priv->gpu_damage,
				      dst_pixmap->drawable.width,
				      dst_pixmap->drawable.height)) {
			DBG(("%s: replaced entire pixmap, destroying CPU shadow\n",
			     __FUNCTION__));
			sna_damage_destroy(&dst_priv->cpu_damage);
			list_del(&dst_priv->flush_list);
		} else
			sna_damage_subtract(&dst_priv->cpu_damage,
					    region);
	}
	dst_priv->clear = false;

	assert(has_coherent_ptr(sna, src_priv, MOVE_READ));

	box = region_rects(region);
	n = region_num_rects(region);
	if (dst_priv->gpu_bo->tiling) {
		DBG(("%s: copy to a tiled CPU map\n", __FUNCTION__));
		assert(dst_priv->gpu_bo->tiling == I915_TILING_X);
		assert(src_pixmap->devKind);
		do {
			memcpy_to_tiled_x(&sna->kgem, src_pixmap->devPrivate.ptr, ptr,
					  src_pixmap->drawable.bitsPerPixel,
					  src_pixmap->devKind,
					  dst_priv->gpu_bo->pitch,
					  box->x1 + dx, box->y1 + dy,
					  box->x1, box->y1,
					  box->x2 - box->x1, box->y2 - box->y1);
			box++;
		} while (--n);
	} else {
		DBG(("%s: copy to a linear CPU map\n", __FUNCTION__));
		assert(src_pixmap->devKind);
		do {
			memcpy_blt(src_pixmap->devPrivate.ptr, ptr,
				   src_pixmap->drawable.bitsPerPixel,
				   src_pixmap->devKind,
				   dst_priv->gpu_bo->pitch,
				   box->x1 + dx, box->y1 + dy,
				   box->x1, box->y1,
				   box->x2 - box->x1, box->y2 - box->y1);
			box++;
		} while (--n);

		if (!dst_priv->shm) {
			assert(ptr == MAP(dst_priv->gpu_bo->map__cpu));
			dst_pixmap->devPrivate.ptr = ptr;
			dst_pixmap->devKind = dst_priv->gpu_bo->pitch;
			dst_priv->mapped = MAPPED_CPU;
			assert_pixmap_map(dst_pixmap, dst_priv);
			dst_priv->cpu = true;
		}
	}

	return true;
}

static void discard_cpu_damage(struct sna *sna, struct sna_pixmap *priv)
{
	if (priv->cpu_damage == NULL && !priv->shm)
		return;

	DBG(("%s: discarding existing CPU damage\n", __FUNCTION__));

	if (kgem_bo_discard_cache(priv->gpu_bo, true)) {
		DBG(("%s: discarding cached upload buffer\n", __FUNCTION__));
		assert(DAMAGE_IS_ALL(priv->cpu_damage));
		assert(priv->gpu_damage == NULL || DAMAGE_IS_ALL(priv->gpu_damage)); /* magical upload buffer */
		assert(!priv->pinned);
		assert(!priv->mapped);
		sna_damage_destroy(&priv->gpu_damage);
		kgem_bo_destroy(&sna->kgem, priv->gpu_bo);
		priv->gpu_bo = NULL;
	}

	sna_damage_destroy(&priv->cpu_damage);
	list_del(&priv->flush_list);

	if (priv->gpu_bo && sna_pixmap_free_cpu(sna, priv, priv->cpu))
		sna_damage_all(&priv->gpu_damage, priv->pixmap);
	priv->cpu = false;
}

static void
sna_copy_boxes(DrawablePtr src, DrawablePtr dst, GCPtr gc,
	       RegionPtr region, int dx, int dy,
	       Pixel bitplane, void *closure)
{
	PixmapPtr src_pixmap = get_drawable_pixmap(src);
	struct sna_pixmap *src_priv = sna_pixmap(src_pixmap);
	PixmapPtr dst_pixmap = get_drawable_pixmap(dst);
	struct sna_pixmap *dst_priv = sna_pixmap(dst_pixmap);
	struct sna *sna = to_sna_from_pixmap(src_pixmap);
	struct sna_damage **damage;
	struct kgem_bo *bo;
	int16_t src_dx, src_dy;
	int16_t dst_dx, dst_dy;
	const BoxRec *box = region_rects(region);
	int n = region_num_rects(region);
	int alu = gc->alu;
	int stride, bpp;
	char *bits;
	bool replaces;

	assert(region_num_rects(region));

	if (src_pixmap == dst_pixmap)
		return sna_self_copy_boxes(src, dst, gc,
					   region, dx, dy,
					   bitplane, closure);

	DBG(("%s (boxes=%dx[(%d, %d), (%d, %d)...], src pixmap=%ld+(%d, %d), dst pixmap=%ld+(%d, %d), alu=%d, src.size=%dx%d, dst.size=%dx%d)\n",
	     __FUNCTION__, n,
	     box[0].x1, box[0].y1, box[0].x2, box[0].y2,
	     src_pixmap->drawable.serialNumber, dx, dy,
	     dst_pixmap->drawable.serialNumber, get_drawable_dx(dst), get_drawable_dy(dst),
	     alu,
	     src_pixmap->drawable.width, src_pixmap->drawable.height,
	     dst_pixmap->drawable.width, dst_pixmap->drawable.height));

	assert_pixmap_damage(dst_pixmap);
	assert_pixmap_damage(src_pixmap);

	bpp = dst_pixmap->drawable.bitsPerPixel;

	if (get_drawable_deltas(dst, dst_pixmap, &dst_dx, &dst_dy))
		RegionTranslate(region, dst_dx, dst_dy);
	get_drawable_deltas(src, src_pixmap, &src_dx, &src_dy);
	src_dx += dx - dst_dx;
	src_dy += dy - dst_dy;

	assert_pixmap_contains_box(dst_pixmap, RegionExtents(region));
	assert_pixmap_contains_box_with_offset(src_pixmap,
					       RegionExtents(region),
					       src_dx, src_dy);

	replaces = n == 1 &&
		alu_overwrites(alu) &&
		box->x1 <= 0 &&
		box->y1 <= 0 &&
		box->x2 >= dst_pixmap->drawable.width &&
		box->y2 >= dst_pixmap->drawable.height;

	DBG(("%s: dst=(priv=%p, gpu_bo=%d, cpu_bo=%d), src=(priv=%p, gpu_bo=%d, cpu_bo=%d), replaces=%d\n",
	     __FUNCTION__,
	     dst_priv,
	     dst_priv && dst_priv->gpu_bo ? dst_priv->gpu_bo->handle : 0,
	     dst_priv && dst_priv->cpu_bo ? dst_priv->cpu_bo->handle : 0,
	     src_priv,
	     src_priv && src_priv->gpu_bo ? src_priv->gpu_bo->handle : 0,
	     src_priv && src_priv->cpu_bo ? src_priv->cpu_bo->handle : 0,
	     replaces));

	if (dst_priv == NULL) {
		DBG(("%s: unattached dst failed, fallback\n", __FUNCTION__));
		goto fallback;
	}

	if (alu == GXcopy &&
	    src_priv && src_priv->cow &&
	    COW(src_priv->cow) == COW(dst_priv->cow)) {
		if ((dx | dy) == 0) {
			DBG(("%s: ignoring cow for no op\n",
			     __FUNCTION__));
			return;
		} else if (IS_COW_OWNER(dst_priv->cow)) {
			/* XXX hack for firefox -- subsequent uses of src will be corrupt! */
			DBG(("%s: ignoring cow reference for cousin copy\n",
			     __FUNCTION__));
			assert(src_priv->cpu_damage == NULL);
			assert(dst_priv->move_to_gpu == NULL);
			bo = dst_priv->gpu_bo;
			damage = NULL;
		} else
			goto discard_cow;
	} else {
		unsigned hint;
discard_cow:
		hint = copy_prefer_gpu(sna, dst_priv, src_priv, region, src_dx, src_dy);
		if (replaces) {
			discard_cpu_damage(sna, dst_priv);
			hint |= REPLACES | IGNORE_DAMAGE;
		} else if (alu_overwrites(alu)) {
			if (region->data == NULL)
				hint |= IGNORE_DAMAGE;
			if (dst_priv->cpu_damage &&
			    region_subsumes_damage(region,
						   dst_priv->cpu_damage))
				discard_cpu_damage(sna, dst_priv);
		}
		bo = sna_drawable_use_bo(&dst_pixmap->drawable, hint,
					 &region->extents, &damage);
	}
	if (bo) {
		if (alu == GXset || alu == GXclear || (src_priv && src_priv->clear)) {
			uint32_t color;

			if (alu == GXset)
				color = (1 << dst_pixmap->drawable.depth) - 1;
			else if (alu == GXclear)
				color = 0;
			else
				color = src_priv->clear_color;
			DBG(("%s: applying src clear [%08x] to dst\n",
			     __FUNCTION__, src_priv->clear_color));

			if (n == 1) {
				if (replaces && UNDO)
					kgem_bo_pair_undo(&sna->kgem, dst_priv->gpu_bo, dst_priv->cpu_bo);

				if (!sna->render.fill_one(sna,
							  dst_pixmap, bo, color,
							  box->x1, box->y1,
							  box->x2, box->y2,
							  alu)) {
					DBG(("%s: unsupported fill\n",
					     __FUNCTION__));
					goto fallback;
				}

				if (replaces && bo == dst_priv->gpu_bo) {
					DBG(("%s: marking dst handle=%d as all clear [%08x]\n",
					     __FUNCTION__,
					     dst_priv->gpu_bo->handle,
					     src_priv->clear_color));
					dst_priv->clear = true;
					dst_priv->clear_color = color;
					sna_damage_all(&dst_priv->gpu_damage, dst_pixmap);
					sna_damage_destroy(&dst_priv->cpu_damage);
					list_del(&dst_priv->flush_list);
					return;
				}
			} else {
				struct sna_fill_op fill;

				if (!sna_fill_init_blt(&fill, sna,
						       dst_pixmap, bo,
						       alu, color,
						       FILL_BOXES)) {
					DBG(("%s: unsupported fill\n",
					     __FUNCTION__));
					goto fallback;
				}

				fill.boxes(sna, &fill, box, n);
				fill.done(sna, &fill);
			}

			if (damage)
				sna_damage_add(damage, region);
			return;
		}

		if (src_priv &&
		    move_to_gpu(src_pixmap, src_priv, region, src_dx, src_dy, alu, bo == dst_priv->gpu_bo) &&
		    sna_pixmap_move_to_gpu(src_pixmap, MOVE_READ | MOVE_ASYNC_HINT)) {
			DBG(("%s: move whole src_pixmap to GPU and copy\n",
			     __FUNCTION__));
			if (replaces && UNDO)
				kgem_bo_pair_undo(&sna->kgem, dst_priv->gpu_bo, dst_priv->cpu_bo);

			if (replaces &&
			    src_pixmap->drawable.width == dst_pixmap->drawable.width &&
			    src_pixmap->drawable.height == dst_pixmap->drawable.height) {
				assert(src_pixmap->drawable.depth == dst_pixmap->drawable.depth);
				assert(src_pixmap->drawable.bitsPerPixel == dst_pixmap->drawable.bitsPerPixel);
				if (sna_pixmap_make_cow(sna, src_priv, dst_priv)) {
					assert(dst_priv->gpu_bo == src_priv->gpu_bo);
					sna_damage_all(&dst_priv->gpu_damage, dst_pixmap);
					sna_damage_destroy(&dst_priv->cpu_damage);
					list_del(&dst_priv->flush_list);
					if (dst_priv->shm)
						sna_add_flush_pixmap(sna, dst_priv, dst_priv->cpu_bo);
					return;
				}
			}
			if (!sna->render.copy_boxes(sna, alu,
						    &src_pixmap->drawable, src_priv->gpu_bo, src_dx, src_dy,
						    &dst_pixmap->drawable, bo, 0, 0,
						    box, n, 0)) {
				DBG(("%s: fallback - accelerated copy boxes failed\n",
				     __FUNCTION__));
				goto fallback;
			}

			if (damage)
				sna_damage_add(damage, region);
			return;
		}

		if (src_priv &&
		    region_overlaps_damage(region, src_priv->gpu_damage,
					   src_dx, src_dy)) {
			BoxRec area;

			DBG(("%s: region overlaps GPU damage, upload and copy\n",
			     __FUNCTION__));

			area = region->extents;
			area.x1 += src_dx;
			area.x2 += src_dx;
			area.y1 += src_dy;
			area.y2 += src_dy;

			if (!sna_pixmap_move_area_to_gpu(src_pixmap, &area,
							 MOVE_READ | MOVE_ASYNC_HINT)) {
				DBG(("%s: move-to-gpu(src) failed, fallback\n", __FUNCTION__));
				goto fallback;
			}

			if (replaces && UNDO)
				kgem_bo_pair_undo(&sna->kgem, dst_priv->gpu_bo, dst_priv->cpu_bo);

			if (!sna->render.copy_boxes(sna, alu,
						    &src_pixmap->drawable, src_priv->gpu_bo, src_dx, src_dy,
						    &dst_pixmap->drawable, bo, 0, 0,
						    box, n, 0)) {
				DBG(("%s: fallback - accelerated copy boxes failed\n",
				     __FUNCTION__));
				goto fallback;
			}

			if (damage)
				sna_damage_add(damage, region);
			return;
		}

		if (bo != dst_priv->gpu_bo)
			goto fallback;

		if (use_shm_bo(sna, bo, src_priv, alu, replaces && !dst_priv->pinned)) {
			bool ret;

			DBG(("%s: region overlaps CPU damage, copy from CPU bo (shm? %d)\n",
			     __FUNCTION__, src_priv->shm));

			assert(bo != dst_priv->cpu_bo);

			RegionTranslate(region, src_dx, src_dy);
			ret = sna_drawable_move_region_to_cpu(&src_pixmap->drawable,
							      region,
							      MOVE_READ | MOVE_ASYNC_HINT);
			RegionTranslate(region, -src_dx, -src_dy);
			if (!ret) {
				DBG(("%s: move-to-cpu(src) failed, fallback\n", __FUNCTION__));
				goto fallback;
			}

			if (replaces && UNDO)
				kgem_bo_pair_undo(&sna->kgem, dst_priv->gpu_bo, dst_priv->cpu_bo);

			if (src_priv->shm) {
				assert(!src_priv->flush);
				sna_add_flush_pixmap(sna, src_priv, src_priv->cpu_bo);
			}

			if (!sna->render.copy_boxes(sna, alu,
						    &src_pixmap->drawable, src_priv->cpu_bo, src_dx, src_dy,
						    &dst_pixmap->drawable, bo, 0, 0,
						    box, n, src_priv->shm ? COPY_LAST : 0)) {
				DBG(("%s: fallback - accelerated copy boxes failed\n",
				     __FUNCTION__));
				goto fallback;
			}

			if (damage)
				sna_damage_add(damage, region);
			return;
		}

		if (src_priv) {
			bool ret;

			RegionTranslate(region, src_dx, src_dy);
			ret = sna_drawable_move_region_to_cpu(&src_pixmap->drawable,
							      region, MOVE_READ);
			RegionTranslate(region, -src_dx, -src_dy);
			if (!ret) {
				DBG(("%s: move-to-cpu(src) failed, fallback\n", __FUNCTION__));
				goto fallback;
			}

			assert(!src_priv->mapped);
			if (src_pixmap->devPrivate.ptr == NULL)
				/* uninitialised!*/
				return;
		}

		if (USE_USERPTR_UPLOADS &&
		    sna->kgem.has_userptr &&
		    (alu != GXcopy ||
		     (box_inplace(src_pixmap, &region->extents) &&
		      __kgem_bo_is_busy(&sna->kgem, bo)))) {
			struct kgem_bo *src_bo;
			bool ok = false;

			DBG(("%s: upload through a temporary map\n",
			     __FUNCTION__));

			assert(src_pixmap->devKind);
			src_bo = kgem_create_map(&sna->kgem,
						 src_pixmap->devPrivate.ptr,
						 src_pixmap->devKind * src_pixmap->drawable.height,
						 true);
			if (src_bo) {
				src_bo->pitch = src_pixmap->devKind;
				kgem_bo_mark_unreusable(src_bo);

				ok = sna->render.copy_boxes(sna, alu,
							    &src_pixmap->drawable, src_bo, src_dx, src_dy,
							    &dst_pixmap->drawable, bo, 0, 0,
							    box, n, COPY_LAST);

				kgem_bo_sync__cpu(&sna->kgem, src_bo);
				assert(src_bo->rq == NULL);
				kgem_bo_destroy(&sna->kgem, src_bo);
			}

			if (ok) {
				if (damage)
					sna_damage_add(damage, region);
				return;
			}
		}

		if (alu != GXcopy) {
			PixmapPtr tmp;
			struct kgem_bo *src_bo;
			int i;

			assert(src_pixmap->drawable.depth != 1);

			DBG(("%s: creating temporary source upload for non-copy alu [%d]\n",
			     __FUNCTION__, alu));

			tmp = sna_pixmap_create_upload(src->pScreen,
						       region->extents.x2 - region->extents.x1,
						       region->extents.y2 - region->extents.y1,
						       src->depth,
						       KGEM_BUFFER_WRITE_INPLACE);
			if (tmp == NullPixmap)
				return;

			src_bo = __sna_pixmap_get_bo(tmp);
			assert(src_bo != NULL);

			dx = -region->extents.x1;
			dy = -region->extents.y1;
			for (i = 0; i < n; i++) {
				assert(box[i].x1 + src_dx >= 0);
				assert(box[i].y1 + src_dy >= 0);
				assert(box[i].x2 + src_dx <= src_pixmap->drawable.width);
				assert(box[i].y2 + src_dy <= src_pixmap->drawable.height);

				assert(box[i].x1 + dx >= 0);
				assert(box[i].y1 + dy >= 0);
				assert(box[i].x2 + dx <= tmp->drawable.width);
				assert(box[i].y2 + dy <= tmp->drawable.height);

				assert(has_coherent_ptr(sna, sna_pixmap(src_pixmap), MOVE_READ));
				assert(has_coherent_ptr(sna, sna_pixmap(tmp), MOVE_WRITE));
				assert(src_pixmap->devKind);
				assert(tmp->devKind);
				memcpy_blt(src_pixmap->devPrivate.ptr,
					   tmp->devPrivate.ptr,
					   src_pixmap->drawable.bitsPerPixel,
					   src_pixmap->devKind,
					   tmp->devKind,
					   box[i].x1 + src_dx,
					   box[i].y1 + src_dy,
					   box[i].x1 + dx,
					   box[i].y1 + dy,
					   box[i].x2 - box[i].x1,
					   box[i].y2 - box[i].y1);
			}

			if (n == 1 &&
			    tmp->drawable.width == src_pixmap->drawable.width &&
			    tmp->drawable.height == src_pixmap->drawable.height) {
				DBG(("%s: caching upload for src bo\n",
				     __FUNCTION__));
				assert(src_priv->gpu_damage == NULL);
				assert(src_priv->gpu_bo == NULL);
				kgem_proxy_bo_attach(src_bo, &src_priv->gpu_bo);
			}

			if (!sna->render.copy_boxes(sna, alu,
						    &tmp->drawable, src_bo, dx, dy,
						    &dst_pixmap->drawable, bo, 0, 0,
						    box, n, 0)) {
				DBG(("%s: fallback - accelerated copy boxes failed\n",
				     __FUNCTION__));
				tmp->drawable.pScreen->DestroyPixmap(tmp);
				goto fallback;
			}
			tmp->drawable.pScreen->DestroyPixmap(tmp);

			if (damage)
				sna_damage_add(damage, region);
			return;
		} else {
			DBG(("%s: dst is on the GPU, src is on the CPU, uploading into dst\n",
			     __FUNCTION__));

			assert(src_pixmap->devKind);
			if (!dst_priv->pinned && replaces) {
				stride = src_pixmap->devKind;
				bits = src_pixmap->devPrivate.ptr;
				bits += (src_dy + box->y1) * stride + (src_dx + box->x1) * bpp / 8;

				if (!sna_replace(sna, dst_pixmap, bits, stride)) {
					DBG(("%s: replace failed, fallback\n", __FUNCTION__));
					goto fallback;
				}
			} else {
				assert(!DAMAGE_IS_ALL(dst_priv->cpu_damage));
				if (!sna_write_boxes(sna, dst_pixmap,
						     dst_priv->gpu_bo, 0, 0,
						     src_pixmap->devPrivate.ptr,
						     src_pixmap->devKind,
						     src_dx, src_dy,
						     box, n)) {
					DBG(("%s: write failed, fallback\n", __FUNCTION__));
					goto fallback;
				}
			}

			assert(dst_priv->clear == false);
			dst_priv->cpu = false;
			if (damage) {
				assert(!dst_priv->clear);
				assert(dst_priv->gpu_bo);
				assert(dst_priv->gpu_bo->proxy == NULL);
				assert(*damage == dst_priv->gpu_damage);
				if (replaces) {
					sna_damage_destroy(&dst_priv->cpu_damage);
					sna_damage_all(&dst_priv->gpu_damage, dst_pixmap);
					list_del(&dst_priv->flush_list);
				} else
					sna_damage_add(&dst_priv->gpu_damage,
						       region);
				assert_pixmap_damage(dst_pixmap);
			}
		}

		return;
	}

fallback:
	if (alu == GXcopy && src_priv && src_priv->clear) {
		DBG(("%s: copying clear [%08x]\n",
		     __FUNCTION__, src_priv->clear_color));

		if (dst_priv) {
			if (!sna_drawable_move_region_to_cpu(&dst_pixmap->drawable,
							     region,
							     MOVE_WRITE | MOVE_INPLACE_HINT))
				return;
		}

		assert(dst_pixmap->devPrivate.ptr);
		assert(dst_pixmap->devKind);
		do {
			pixman_fill(dst_pixmap->devPrivate.ptr,
				    dst_pixmap->devKind/sizeof(uint32_t),
				    dst_pixmap->drawable.bitsPerPixel,
				    box->x1, box->y1,
				    box->x2 - box->x1,
				    box->y2 - box->y1,
				    src_priv->clear_color);
			box++;
		} while (--n);
	} else if (!sna_copy_boxes__inplace(sna, region, alu,
					    src_pixmap, src_priv,
					    src_dx, src_dy,
					    dst_pixmap, dst_priv,
					    replaces)) {
		FbBits *dst_bits, *src_bits;
		int dst_stride, src_stride;

		DBG(("%s: fallback -- src=(%d, %d), dst=(%d, %d)\n",
		     __FUNCTION__, src_dx, src_dy, dst_dx, dst_dy));
		if (src_priv) {
			unsigned mode;

			RegionTranslate(region, src_dx, src_dy);

			assert_pixmap_contains_box(src_pixmap,
						   RegionExtents(region));

			mode = MOVE_READ;
			if (!sna->kgem.can_blt_cpu ||
			    (src_priv->cpu_bo == NULL &&
			     (src_priv->create & KGEM_CAN_CREATE_CPU) == 0))
				mode |= MOVE_INPLACE_HINT;

			if (!sna_drawable_move_region_to_cpu(&src_pixmap->drawable,
							     region, mode))
				return;

			RegionTranslate(region, -src_dx, -src_dy);
		}
		assert(src_priv == sna_pixmap(src_pixmap));

		if (dst_priv) {
			unsigned mode;

			if (alu_overwrites(alu))
				mode = MOVE_WRITE | MOVE_INPLACE_HINT;
			else
				mode = MOVE_WRITE | MOVE_READ;
			if (!sna_drawable_move_region_to_cpu(&dst_pixmap->drawable,
							     region, mode))
				return;
		}
		assert(dst_priv == sna_pixmap(dst_pixmap));

		assert(dst_pixmap->devKind);
		assert(src_pixmap->devKind);
		dst_stride = dst_pixmap->devKind;
		src_stride = src_pixmap->devKind;

		if (alu == GXcopy && bpp >= 8) {
			dst_bits = (FbBits *)dst_pixmap->devPrivate.ptr;
			src_bits = (FbBits *)
				((char *)src_pixmap->devPrivate.ptr +
				 src_dy * src_stride + src_dx * bpp / 8);

			do {
				DBG(("%s: memcpy_blt(box=(%d, %d), (%d, %d), src=(%d, %d), pitches=(%d, %d))\n",
				     __FUNCTION__,
				     box->x1, box->y1,
				     box->x2 - box->x1,
				     box->y2 - box->y1,
				     src_dx, src_dy,
				     src_stride, dst_stride));

				assert(box->x1 >= 0);
				assert(box->y1 >= 0);
				assert(box->x2 <= dst_pixmap->drawable.width);
				assert(box->y2 <= dst_pixmap->drawable.height);

				assert(box->x1 + src_dx >= 0);
				assert(box->y1 + src_dy >= 0);
				assert(box->x2 + src_dx <= src_pixmap->drawable.width);
				assert(box->y2 + src_dy <= src_pixmap->drawable.height);
				assert(has_coherent_ptr(sna, src_priv, MOVE_READ));
				assert(has_coherent_ptr(sna, dst_priv, MOVE_WRITE));
				assert(src_stride);
				assert(dst_stride);
				memcpy_blt(src_bits, dst_bits, bpp,
					   src_stride, dst_stride,
					   box->x1, box->y1,
					   box->x1, box->y1,
					   box->x2 - box->x1,
					   box->y2 - box->y1);
				box++;
			} while (--n);
		} else {
			DBG(("%s: fallback -- miCopyRegion\n", __FUNCTION__));

			RegionTranslate(region, -dst_dx, -dst_dy);

			if (sna_gc_move_to_cpu(gc, dst, region) &&
			    sigtrap_get() == 0) {
				miCopyRegion(src, dst, gc,
					     region, dx, dy,
					     fbCopyNtoN, 0, NULL);
				sigtrap_put();
			}

			sna_gc_move_to_gpu(gc);
		}
	}
}

typedef void (*sna_copy_func)(DrawablePtr src, DrawablePtr dst, GCPtr gc,
			      RegionPtr region, int dx, int dy,
			      Pixel bitPlane, void *closure);

static inline bool box_equal(const BoxRec *a, const BoxRec *b)
{
	return *(const uint64_t *)a == *(const uint64_t *)b;
}

static inline bool has_clip(GCPtr gc)
{
#if XORG_VERSION_CURRENT < XORG_VERSION_NUMERIC(1,16,99,901,0)
       return gc->clientClipType != CT_NONE;
#else
       return gc->clientClip != NULL;
#endif
}

static RegionPtr
sna_do_copy(DrawablePtr src, DrawablePtr dst, GCPtr gc,
	    int sx, int sy,
	    int width, int height,
	    int dx, int dy,
	    sna_copy_func copy, Pixel bitPlane, void *closure)
{
	RegionPtr clip;
	RegionRec region;
	BoxRec src_extents;
	bool expose;

	DBG(("%s: src=(%d, %d), dst=(%d, %d), size=(%dx%d)\n",
	     __FUNCTION__, sx, sy, dx, dy, width, height));

	/* Short cut for unmapped windows */
	if (dst->type == DRAWABLE_WINDOW && !((WindowPtr)dst)->realized) {
		DBG(("%s: unmapped\n", __FUNCTION__));
		return NULL;
	}

	SourceValidate(src, sx, sy, width, height, gc->subWindowMode);

	sx += src->x;
	sy += src->y;

	dx += dst->x;
	dy += dst->y;

	DBG(("%s: after drawable: src=(%d, %d), dst=(%d, %d), size=(%dx%d)\n",
	     __FUNCTION__, sx, sy, dx, dy, width, height));

	region.extents.x1 = dx;
	region.extents.y1 = dy;
	region.extents.x2 = bound(dx, width);
	region.extents.y2 = bound(dy, height);
	region.data = NULL;

	DBG(("%s: dst extents (%d, %d), (%d, %d), dst clip extents (%d, %d), (%d, %d), dst size=%dx%d\n", __FUNCTION__,
	     region.extents.x1, region.extents.y1,
	     region.extents.x2, region.extents.y2,
	     gc->pCompositeClip->extents.x1, gc->pCompositeClip->extents.y1,
	     gc->pCompositeClip->extents.x2, gc->pCompositeClip->extents.y2,
	     dst->width, dst->height));

	if (!box_intersect(&region.extents, &gc->pCompositeClip->extents)) {
		DBG(("%s: dst clipped out\n", __FUNCTION__));
		return NULL;
	}

	DBG(("%s: clipped dst extents (%d, %d), (%d, %d)\n", __FUNCTION__,
	     region.extents.x1, region.extents.y1,
	     region.extents.x2, region.extents.y2));
	assert_drawable_contains_box(dst, &region.extents);

	region.extents.x1 = clamp(region.extents.x1, sx - dx);
	region.extents.x2 = clamp(region.extents.x2, sx - dx);
	region.extents.y1 = clamp(region.extents.y1, sy - dy);
	region.extents.y2 = clamp(region.extents.y2, sy - dy);

	src_extents = region.extents;
	expose = true;

	DBG(("%s: unclipped src extents (%d, %d), (%d, %d)\n", __FUNCTION__,
	     region.extents.x1, region.extents.y1,
	     region.extents.x2, region.extents.y2));

	if (region.extents.x1 < src->x)
		region.extents.x1 = src->x;
	if (region.extents.y1 < src->y)
		region.extents.y1 = src->y;
	if (region.extents.x2 > src->x + (int) src->width)
		region.extents.x2 = src->x + (int) src->width;
	if (region.extents.y2 > src->y + (int) src->height)
		region.extents.y2 = src->y + (int) src->height;

	DBG(("%s: clipped src extents (%d, %d), (%d, %d)\n", __FUNCTION__,
	     region.extents.x1, region.extents.y1,
	     region.extents.x2, region.extents.y2));
	if (box_empty(&region.extents)) {
		DBG(("%s: src clipped out\n", __FUNCTION__));
		return NULL;
	}

	/* Compute source clip region */
	if (src->type == DRAWABLE_PIXMAP) {
		if (src == dst && !has_clip(gc)) {
			DBG(("%s: pixmap -- using gc clip\n", __FUNCTION__));
			clip = gc->pCompositeClip;
		} else {
			DBG(("%s: pixmap -- no source clipping\n", __FUNCTION__));
			expose = false;
			clip = NULL;
		}
	} else {
		WindowPtr w = (WindowPtr)src;
		if (gc->subWindowMode == IncludeInferiors) {
			DBG(("%s: window -- include inferiors\n", __FUNCTION__));

			if (w->winSize.data)
				RegionIntersect(&region, &region, &w->winSize);
			else
				box_intersect(&region.extents, &w->winSize.extents);
			clip = &w->borderClip;
		} else {
			DBG(("%s: window -- clip by children\n", __FUNCTION__));
			clip = &w->clipList;
		}
	}
	if (clip != NULL) {
		if (clip->data == NULL) {
			box_intersect(&region.extents, &clip->extents);
			if (box_equal(&src_extents, &region.extents))
				expose = false;
		} else
			RegionIntersect(&region, &region, clip);
	}
	DBG(("%s: src extents (%d, %d), (%d, %d) x %d\n", __FUNCTION__,
	     region.extents.x1, region.extents.y1,
	     region.extents.x2, region.extents.y2,
	     region_num_rects(&region)));

	RegionTranslate(&region, dx-sx, dy-sy);
	if (gc->pCompositeClip->data)
		RegionIntersect(&region, &region, gc->pCompositeClip);
	DBG(("%s: copy region (%d, %d), (%d, %d) x %d + (%d, %d)\n",
	     __FUNCTION__,
	     region.extents.x1, region.extents.y1,
	     region.extents.x2, region.extents.y2,
	     region_num_rects(&region),
	     sx-dx, sy-dy));

	if (!box_empty(&region.extents))
		copy(src, dst, gc, &region, sx-dx, sy-dy, bitPlane, closure);
	assert(gc->pCompositeClip != &region);
	RegionUninit(&region);

	/* Pixmap sources generate a NoExposed (we return NULL to do this) */
	clip = NULL;
	if (expose && gc->fExpose)
		clip = miHandleExposures(src, dst, gc,
					 sx - src->x, sy - src->y,
					 width, height,
					 dx - dst->x, dy - dst->y,
					 (unsigned long) bitPlane);
	return clip;
}

static void
sna_fallback_copy_boxes(DrawablePtr src, DrawablePtr dst, GCPtr gc,
			RegionPtr region, int dx, int dy,
			Pixel bitplane, void *closure)
{
	DBG(("%s (boxes=%dx[(%d, %d), (%d, %d)...], src=+(%d, %d), alu=%d\n",
	     __FUNCTION__, region_num_rects(region),
	     region->extents.x1, region->extents.y1,
	     region->extents.x2, region->extents.y2,
	     dx, dy, gc->alu));

	if (!sna_gc_move_to_cpu(gc, dst, region))
		goto out;

	RegionTranslate(region, dx, dy);
	if (!sna_drawable_move_region_to_cpu(src, region, MOVE_READ))
		goto out;
	RegionTranslate(region, -dx, -dy);

	if (src == dst ||
	    get_drawable_pixmap(src) == get_drawable_pixmap(dst)) {
		DBG(("%s: self-copy\n", __FUNCTION__));
		if (!sna_drawable_move_to_cpu(dst, MOVE_WRITE | MOVE_READ))
			goto out;
	} else {
		if (!sna_drawable_move_region_to_cpu(dst, region,
						     drawable_gc_flags(dst, gc, false)))
			goto out;
	}

	if (sigtrap_get() == 0) {
		miCopyRegion(src, dst, gc,
			     region, dx, dy,
			     fbCopyNtoN, 0, NULL);
		FALLBACK_FLUSH(dst);
		sigtrap_put();
	}
out:
	sna_gc_move_to_gpu(gc);
}

static RegionPtr
sna_copy_area(DrawablePtr src, DrawablePtr dst, GCPtr gc,
	      int src_x, int src_y,
	      int width, int height,
	      int dst_x, int dst_y)
{
	struct sna *sna = to_sna_from_drawable(dst);
	sna_copy_func copy;

	if (gc->planemask == 0)
		return NULL;

	DBG(("%s: src=(%d, %d)x(%d, %d)+(%d, %d) -> dst=(%d, %d)+(%d, %d); alu=%d, pm=%lx, depth=%d\n",
	     __FUNCTION__,
	     src_x, src_y, width, height, src->x, src->y,
	     dst_x, dst_y, dst->x, dst->y,
	     gc->alu, gc->planemask, gc->depth));

	if (FORCE_FALLBACK || !ACCEL_COPY_AREA || wedged(sna) ||
	    !PM_IS_SOLID(dst, gc->planemask) || gc->depth < 8)
		copy = sna_fallback_copy_boxes;
	else if (src == dst)
		copy = sna_self_copy_boxes;
	else
		copy = sna_copy_boxes;

	return sna_do_copy(src, dst, gc,
			   src_x, src_y,
			   width, height,
			   dst_x, dst_y,
			   copy, 0, NULL);
}

static const BoxRec *
find_clip_box_for_y(const BoxRec *begin, const BoxRec *end, int16_t y)
{
    const BoxRec *mid;

    if (end == begin)
	return end;

    if (end - begin == 1) {
	if (begin->y2 > y)
	    return begin;
	else
	    return end;
    }

    mid = begin + (end - begin) / 2;
    if (mid->y2 > y)
	/* If no box is found in [begin, mid], the function
	 * will return @mid, which is then known to be the
	 * correct answer.
	 */
	return find_clip_box_for_y(begin, mid, y);
    else
	return find_clip_box_for_y(mid, end, y);
}

struct sna_fill_spans {
	struct sna *sna;
	PixmapPtr pixmap;
	RegionRec region;
	unsigned flags;
	uint32_t phase;
	struct kgem_bo *bo;
	struct sna_damage **damage;
	int16_t dx, dy;
	void *op;
};

static void
sna_poly_point__cpu(DrawablePtr drawable, GCPtr gc,
		    int mode, int n, DDXPointPtr pt)
{
	fbPolyPoint(drawable, gc, mode, n, pt, -1);
}

static void
sna_poly_point__fill(DrawablePtr drawable, GCPtr gc,
		     int mode, int n, DDXPointPtr pt)
{
	struct sna_fill_spans *data = sna_gc(gc)->priv;
	struct sna_fill_op *op = data->op;
	BoxRec box[512];
	DDXPointRec last;

	DBG(("%s: count=%d\n", __FUNCTION__, n));
	if (n == 0)
		return;

	last.x = drawable->x + data->dx;
	last.y = drawable->y + data->dy;
	if (op->points && mode != CoordModePrevious) {
		op->points(data->sna, op, last.x, last.y, pt, n);
	} else do {
		BoxRec *b = box;
		unsigned nbox = n;
		if (nbox > ARRAY_SIZE(box))
			nbox = ARRAY_SIZE(box);
		n -= nbox;
		do {
			*(DDXPointRec *)b = *pt++;

			b->x1 += last.x;
			b->y1 += last.y;
			if (mode == CoordModePrevious)
				last = *(DDXPointRec *)b;

			b->x2 = b->x1 + 1;
			b->y2 = b->y1 + 1;
			b++;
		} while (--nbox);
		op->boxes(data->sna, op, box, b - box);
	} while (n);
}

static void
sna_poly_point__gpu(DrawablePtr drawable, GCPtr gc,
		     int mode, int n, DDXPointPtr pt)
{
	struct sna_fill_spans *data = sna_gc(gc)->priv;
	struct sna_fill_op fill;
	BoxRec box[512];
	DDXPointRec last;

	if (!sna_fill_init_blt(&fill,
			       data->sna, data->pixmap,
			       data->bo, gc->alu, gc->fgPixel,
			       FILL_POINTS))
		return;

	DBG(("%s: count=%d\n", __FUNCTION__, n));

	last.x = drawable->x;
	last.y = drawable->y;
	while (n) {
		BoxRec *b = box;
		unsigned nbox = n;
		if (nbox > ARRAY_SIZE(box))
			nbox = ARRAY_SIZE(box);
		n -= nbox;
		do {
			*(DDXPointRec *)b = *pt++;

			b->x1 += last.x;
			b->y1 += last.y;
			if (mode == CoordModePrevious)
				last = *(DDXPointRec *)b;

			if (RegionContainsPoint(&data->region,
						b->x1, b->y1, NULL)) {
				b->x1 += data->dx;
				b->y1 += data->dy;
				b->x2 = b->x1 + 1;
				b->y2 = b->y1 + 1;
				b++;
			}
		} while (--nbox);
		fill.boxes(data->sna, &fill, box, b - box);
	}
	fill.done(data->sna, &fill);
}

static void
sna_poly_point__fill_clip_extents(DrawablePtr drawable, GCPtr gc,
				  int mode, int n, DDXPointPtr pt)
{
	struct sna_fill_spans *data = sna_gc(gc)->priv;
	struct sna_fill_op *op = data->op;
	const BoxRec *extents = &data->region.extents;
	BoxRec box[512], *b = box;
	const BoxRec *const last_box = b + ARRAY_SIZE(box);
	DDXPointRec last;

	DBG(("%s: count=%d\n", __FUNCTION__, n));

	last.x = drawable->x + data->dx;
	last.y = drawable->y + data->dy;
	while (n--) {
		*(DDXPointRec *)b = *pt++;

		b->x1 += last.x;
		b->y1 += last.y;
		if (mode == CoordModePrevious)
			last = *(DDXPointRec *)b;

		if (b->x1 >= extents->x1 && b->x1 < extents->x2 &&
		    b->y1 >= extents->y1 && b->y1 < extents->y2) {
			b->x2 = b->x1 + 1;
			b->y2 = b->y1 + 1;
			if (++b == last_box) {
				op->boxes(data->sna, op, box, last_box - box);
				b = box;
			}
		}
	}
	if (b != box)
		op->boxes(data->sna, op, box, b - box);
}

static void
sna_poly_point__fill_clip_boxes(DrawablePtr drawable, GCPtr gc,
				int mode, int n, DDXPointPtr pt)
{
	struct sna_fill_spans *data = sna_gc(gc)->priv;
	struct sna_fill_op *op = data->op;
	RegionRec *clip = &data->region;
	BoxRec box[512], *b = box;
	const BoxRec *const last_box = b + ARRAY_SIZE(box);
	DDXPointRec last;

	DBG(("%s: count=%d\n", __FUNCTION__, n));

	last.x = drawable->x + data->dx;
	last.y = drawable->y + data->dy;
	while (n--) {
		*(DDXPointRec *)b = *pt++;

		b->x1 += last.x;
		b->y1 += last.y;
		if (mode == CoordModePrevious)
			last = *(DDXPointRec *)b;

		if (RegionContainsPoint(clip, b->x1, b->y1, NULL)) {
			b->x2 = b->x1 + 1;
			b->y2 = b->y1 + 1;
			if (++b == last_box) {
				op->boxes(data->sna, op, box, last_box - box);
				b = box;
			}
		}
	}
	if (b != box)
		op->boxes(data->sna, op, box, b - box);
}

static void
sna_poly_point__dash(DrawablePtr drawable, GCPtr gc,
		     int mode, int n, DDXPointPtr pt)
{
	struct sna_fill_spans *data = sna_gc(gc)->priv;
	if (data->phase == gc->fgPixel)
		sna_poly_point__fill(drawable, gc, mode, n, pt);
}

static void
sna_poly_point__dash_clip_extents(DrawablePtr drawable, GCPtr gc,
				  int mode, int n, DDXPointPtr pt)
{
	struct sna_fill_spans *data = sna_gc(gc)->priv;
	if (data->phase == gc->fgPixel)
		sna_poly_point__fill_clip_extents(drawable, gc, mode, n, pt);
}

static void
sna_poly_point__dash_clip_boxes(DrawablePtr drawable, GCPtr gc,
				  int mode, int n, DDXPointPtr pt)
{
	struct sna_fill_spans *data = sna_gc(gc)->priv;
	if (data->phase == gc->fgPixel)
		sna_poly_point__fill_clip_boxes(drawable, gc, mode, n, pt);
}

static void
sna_fill_spans__fill(DrawablePtr drawable,
		     GCPtr gc, int n,
		     DDXPointPtr pt, int *width, int sorted)
{
	struct sna_fill_spans *data = sna_gc(gc)->priv;
	struct sna_fill_op *op = data->op;
	BoxRec box[512];

	DBG(("%s: alu=%d, fg=%08lx, count=%d\n",
	     __FUNCTION__, gc->alu, gc->fgPixel, n));

	while (n) {
		BoxRec *b = box;
		int nbox = n;
		if (nbox > ARRAY_SIZE(box))
			nbox = ARRAY_SIZE(box);
		n -= nbox;
		do {
			*(DDXPointRec *)b = *pt++;
			b->x2 = b->x1 + (int)*width++;
			b->y2 = b->y1 + 1;
			DBG(("%s: (%d, %d), (%d, %d)\n",
			     __FUNCTION__, b->x1, b->y1, b->x2, b->y2));
			assert(b->x1 >= drawable->x);
			assert(b->x2 <= drawable->x + drawable->width);
			assert(b->y1 >= drawable->y);
			assert(b->y2 <= drawable->y + drawable->height);
			if (b->x2 > b->x1) {
				if (b != box &&
				    b->y1 == b[-1].y2 &&
				    b->x1 == b[-1].x1 &&
				    b->x2 == b[-1].x2)
					b[-1].y2 = b->y2;
				else
					b++;
			}
		} while (--nbox);
		if (b != box)
			op->boxes(data->sna, op, box, b - box);
	}
}

static void
sna_fill_spans__dash(DrawablePtr drawable,
		     GCPtr gc, int n,
		     DDXPointPtr pt, int *width, int sorted)
{
	struct sna_fill_spans *data = sna_gc(gc)->priv;
	if (data->phase == gc->fgPixel)
		sna_fill_spans__fill(drawable, gc, n, pt, width, sorted);
}

static void
sna_fill_spans__fill_offset(DrawablePtr drawable,
			    GCPtr gc, int n,
			    DDXPointPtr pt, int *width, int sorted)
{
	struct sna_fill_spans *data = sna_gc(gc)->priv;
	struct sna_fill_op *op = data->op;
	BoxRec box[512];

	DBG(("%s: alu=%d, fg=%08lx\n", __FUNCTION__, gc->alu, gc->fgPixel));

	while (n) {
		BoxRec *b = box;
		int nbox = n;
		if (nbox > ARRAY_SIZE(box))
			nbox = ARRAY_SIZE(box);
		n -= nbox;
		do {
			*(DDXPointRec *)b = *pt++;
			b->x1 += data->dx;
			b->y1 += data->dy;
			b->x2 = b->x1 + (int)*width++;
			b->y2 = b->y1 + 1;
			if (b->x2 > b->x1)
				b++;
		} while (--nbox);
		if (b != box)
			op->boxes(data->sna, op, box, b - box);
	}
}

static void
sna_fill_spans__dash_offset(DrawablePtr drawable,
			    GCPtr gc, int n,
			    DDXPointPtr pt, int *width, int sorted)
{
	struct sna_fill_spans *data = sna_gc(gc)->priv;
	if (data->phase == gc->fgPixel)
		sna_fill_spans__fill_offset(drawable, gc, n, pt, width, sorted);
}

static void
sna_fill_spans__fill_clip_extents(DrawablePtr drawable,
				  GCPtr gc, int n,
				  DDXPointPtr pt, int *width, int sorted)
{
	struct sna_fill_spans *data = sna_gc(gc)->priv;
	struct sna_fill_op *op = data->op;
	const BoxRec *extents = &data->region.extents;
	BoxRec box[512], *b = box, *const last_box = box + ARRAY_SIZE(box);

	DBG(("%s: alu=%d, fg=%08lx, count=%d, extents=(%d, %d), (%d, %d)\n",
	     __FUNCTION__, gc->alu, gc->fgPixel, n,
	     extents->x1, extents->y1,
	     extents->x2, extents->y2));

	while (n--) {
		DBG(("%s: [%d] pt=(%d, %d), width=%d\n",
		     __FUNCTION__, n, pt->x, pt->y, *width));
		*(DDXPointRec *)b = *pt++;
		b->x2 = b->x1 + (int)*width++;
		b->y2 = b->y1 + 1;
		if (box_intersect(b, extents)) {
			DBG(("%s: [%d] clipped=(%d, %d), (%d, %d)\n",
			     __FUNCTION__, n, b->x1, b->y1, b->x2, b->y2));
			if (data->dx|data->dy) {
				b->x1 += data->dx; b->x2 += data->dx;
				b->y1 += data->dy; b->y2 += data->dy;
			}
			if (b != box &&
			    b->y1 == b[-1].y2 &&
			    b->x1 == b[-1].x1 &&
			    b->x2 == b[-1].x2) {
				b[-1].y2 = b->y2;
			} else if (++b == last_box) {
				op->boxes(data->sna, op, box, last_box - box);
				b = box;
			}
		}
	}
	if (b != box)
		op->boxes(data->sna, op, box, b - box);
}

static void
sna_fill_spans__dash_clip_extents(DrawablePtr drawable,
				  GCPtr gc, int n,
				  DDXPointPtr pt, int *width, int sorted)
{
	struct sna_fill_spans *data = sna_gc(gc)->priv;
	if (data->phase == gc->fgPixel)
		sna_fill_spans__fill_clip_extents(drawable, gc, n, pt, width, sorted);
}

static void
sna_fill_spans__fill_clip_boxes(DrawablePtr drawable,
				GCPtr gc, int n,
				DDXPointPtr pt, int *width, int sorted)
{
	struct sna_fill_spans *data = sna_gc(gc)->priv;
	struct sna_fill_op *op = data->op;
	BoxRec box[512], *b = box, *const last_box = box + ARRAY_SIZE(box);
	const BoxRec * const clip_start = RegionBoxptr(&data->region);
	const BoxRec * const clip_end = clip_start + data->region.data->numRects;

	DBG(("%s: alu=%d, fg=%08lx, count=%d, extents=(%d, %d), (%d, %d)\n",
	     __FUNCTION__, gc->alu, gc->fgPixel, n,
	     data->region.extents.x1, data->region.extents.y1,
	     data->region.extents.x2, data->region.extents.y2));

	while (n--) {
		int16_t X1 = pt->x;
		int16_t y = pt->y;
		int16_t X2 = X1 + (int)*width;
		const BoxRec *c;

		pt++;
		width++;

		if (y < data->region.extents.y1 || data->region.extents.y2 <= y)
			continue;

		if (X1 < data->region.extents.x1)
			X1 = data->region.extents.x1;

		if (X2 > data->region.extents.x2)
			X2 = data->region.extents.x2;

		if (X1 >= X2)
			continue;

		c = find_clip_box_for_y(clip_start, clip_end, y);
		while (c != clip_end) {
			if (y + 1 <= c->y1 || X2 <= c->x1)
				break;

			if (X1 >= c->x2) {
				c++;
				continue;
			}

			b->x1 = c->x1;
			b->x2 = c->x2;
			c++;

			if (b->x1 < X1)
				b->x1 = X1;
			if (b->x2 > X2)
				b->x2 = X2;
			if (b->x2 <= b->x1)
				continue;

			b->x1 += data->dx;
			b->x2 += data->dx;
			b->y1 = y + data->dy;
			b->y2 = b->y1 + 1;
			if (++b == last_box) {
				op->boxes(data->sna, op, box, last_box - box);
				b = box;
			}
		}
	}
	if (b != box)
		op->boxes(data->sna, op, box, b - box);
}

static void
sna_fill_spans__dash_clip_boxes(DrawablePtr drawable,
				GCPtr gc, int n,
				DDXPointPtr pt, int *width, int sorted)
{
	struct sna_fill_spans *data = sna_gc(gc)->priv;
	if (data->phase == gc->fgPixel)
		sna_fill_spans__fill_clip_boxes(drawable, gc, n, pt, width, sorted);
}

static bool
sna_fill_spans_blt(DrawablePtr drawable,
		   struct kgem_bo *bo, struct sna_damage **damage,
		   GCPtr gc, uint32_t pixel,
		   int n, DDXPointPtr pt, int *width, int sorted,
		   const BoxRec *extents, unsigned clipped)
{
	PixmapPtr pixmap = get_drawable_pixmap(drawable);
	struct sna *sna = to_sna_from_pixmap(pixmap);
	int16_t dx, dy;
	struct sna_fill_op fill;
	BoxRec box[512], *b = box, *const last_box = box + ARRAY_SIZE(box);
	static void * const jump[] = {
		&&no_damage,
		&&damage,
		&&no_damage_clipped,
		&&damage_clipped,
	};
	unsigned v;

	DBG(("%s: alu=%d, fg=%08lx, damge=%p, clipped?=%d\n",
	     __FUNCTION__, gc->alu, gc->fgPixel, damage, clipped));

	if (!sna_fill_init_blt(&fill, sna, pixmap, bo, gc->alu, pixel, FILL_SPANS))
		return false;

	get_drawable_deltas(drawable, pixmap, &dx, &dy);

	v = (damage != NULL) | clipped;
	goto *jump[v];

no_damage:
	if (dx|dy) {
		do {
			int nbox = n;
			if (nbox > last_box - box)
				nbox = last_box - box;
			n -= nbox;
			do {
				*(DDXPointRec *)b = *pt++;
				b->x1 += dx;
				b->y1 += dy;
				b->x2 = b->x1 + (int)*width++;
				b->y2 = b->y1 + 1;
				b++;
			} while (--nbox);
			fill.boxes(sna, &fill, box, b - box);
			b = box;
		} while (n);
	} else {
		do {
			int nbox = n;
			if (nbox > last_box - box)
				nbox = last_box - box;
			n -= nbox;
			do {
				*(DDXPointRec *)b = *pt++;
				b->x2 = b->x1 + (int)*width++;
				b->y2 = b->y1 + 1;
				b++;
			} while (--nbox);
			fill.boxes(sna, &fill, box, b - box);
			b = box;
		} while (n);
	}
	goto done;

damage:
	do {
		*(DDXPointRec *)b = *pt++;
		b->x1 += dx;
		b->y1 += dy;
		b->x2 = b->x1 + (int)*width++;
		b->y2 = b->y1 + 1;

		if (++b == last_box) {
			assert_pixmap_contains_boxes(pixmap, box, last_box-box, 0, 0);
			fill.boxes(sna, &fill, box, last_box - box);
			sna_damage_add_boxes(damage, box, last_box - box, 0, 0);
			b = box;
		}
	} while (--n);
	if (b != box) {
		assert_pixmap_contains_boxes(pixmap, box, b-box, 0, 0);
		fill.boxes(sna, &fill, box, b - box);
		sna_damage_add_boxes(damage, box, b - box, 0, 0);
	}
	goto done;

no_damage_clipped:
	{
		RegionRec clip;

		region_set(&clip, extents);
		if (!region_maybe_clip(&clip, gc->pCompositeClip))
			return true;

		assert(dx + clip.extents.x1 >= 0);
		assert(dy + clip.extents.y1 >= 0);
		assert(dx + clip.extents.x2 <= pixmap->drawable.width);
		assert(dy + clip.extents.y2 <= pixmap->drawable.height);

		DBG(("%s: clip %d x [(%d, %d), (%d, %d)] x %d [(%d, %d)...]\n",
		     __FUNCTION__,
		     region_num_rects(&clip),
		     clip.extents.x1, clip.extents.y1, clip.extents.x2, clip.extents.y2,
		     n, pt->x, pt->y));

		if (clip.data == NULL) {
			do {
				*(DDXPointRec *)b = *pt++;
				b->x2 = b->x1 + (int)*width++;
				b->y2 = b->y1 + 1;

				if (box_intersect(b, &clip.extents)) {
					if (dx|dy) {
						b->x1 += dx; b->x2 += dx;
						b->y1 += dy; b->y2 += dy;
					}
					if (++b == last_box) {
						fill.boxes(sna, &fill, box, last_box - box);
						b = box;
					}
				}
			} while (--n);
		} else {
			const BoxRec * const clip_start = RegionBoxptr(&clip);
			const BoxRec * const clip_end = clip_start + clip.data->numRects;
			do {
				int16_t X1 = pt->x;
				int16_t y = pt->y;
				int16_t X2 = X1 + (int)*width;
				const BoxRec *c;

				pt++;
				width++;

				if (y < extents->y1 || extents->y2 <= y)
					continue;

				if (X1 < extents->x1)
					X1 = extents->x1;

				if (X2 > extents->x2)
					X2 = extents->x2;

				if (X1 >= X2)
					continue;

				c = find_clip_box_for_y(clip_start,
							clip_end,
							y);
				while (c != clip_end) {
					if (y + 1 <= c->y1 || X2 <= c->x1)
						break;

					if (X1 >= c->x2) {
						c++;
						continue;
					}

					b->x1 = c->x1;
					b->x2 = c->x2;
					c++;

					if (b->x1 < X1)
						b->x1 = X1;
					if (b->x2 > X2)
						b->x2 = X2;
					if (b->x2 <= b->x1)
						continue;

					b->x1 += dx;
					b->x2 += dx;
					b->y1 = y + dy;
					b->y2 = b->y1 + 1;
					if (++b == last_box) {
						fill.boxes(sna, &fill, box, last_box - box);
						b = box;
					}
				}
			} while (--n);
			RegionUninit(&clip);
		}
		if (b != box)
			fill.boxes(sna, &fill, box, b - box);
		goto done;
	}

damage_clipped:
	{
		RegionRec clip;

		region_set(&clip, extents);
		if (!region_maybe_clip(&clip, gc->pCompositeClip))
			return true;

		assert(dx + clip.extents.x1 >= 0);
		assert(dy + clip.extents.y1 >= 0);
		assert(dx + clip.extents.x2 <= pixmap->drawable.width);
		assert(dy + clip.extents.y2 <= pixmap->drawable.height);

		DBG(("%s: clip %d x [(%d, %d), (%d, %d)] x %d [(%d, %d)...]\n",
		     __FUNCTION__,
		     region_num_rects(&clip),
		     clip.extents.x1, clip.extents.y1, clip.extents.x2, clip.extents.y2,
		     n, pt->x, pt->y));

		if (clip.data == NULL) {
			do {
				*(DDXPointRec *)b = *pt++;
				b->x2 = b->x1 + (int)*width++;
				b->y2 = b->y1 + 1;

				if (box_intersect(b, &clip.extents)) {
					b->x1 += dx;
					b->x2 += dx;
					b->y1 += dy;
					b->y2 += dy;
					if (++b == last_box) {
						assert_pixmap_contains_boxes(pixmap, box, b-box, 0, 0);
						fill.boxes(sna, &fill, box, last_box - box);
						sna_damage_add_boxes(damage, box, b - box, 0, 0);
						b = box;
					}
				}
			} while (--n);
		} else {
			const BoxRec * const clip_start = RegionBoxptr(&clip);
			const BoxRec * const clip_end = clip_start + clip.data->numRects;
			do {
				int16_t X1 = pt->x;
				int16_t y = pt->y;
				int16_t X2 = X1 + (int)*width;
				const BoxRec *c;

				pt++;
				width++;

				if (y < extents->y1 || extents->y2 <= y)
					continue;

				if (X1 < extents->x1)
					X1 = extents->x1;

				if (X2 > extents->x2)
					X2 = extents->x2;

				if (X1 >= X2)
					continue;

				c = find_clip_box_for_y(clip_start,
							clip_end,
							y);
				while (c != clip_end) {
					if (y + 1 <= c->y1 || X2 <= c->x1)
						break;

					if (X1 >= c->x2) {
						c++;
						continue;
					}

					b->x1 = c->x1;
					b->x2 = c->x2;
					c++;

					if (b->x1 < X1)
						b->x1 = X1;
					if (b->x2 > X2)
						b->x2 = X2;
					if (b->x2 <= b->x1)
						continue;

					b->x1 += dx;
					b->x2 += dx;
					b->y1 = y + dy;
					b->y2 = b->y1 + 1;
					if (++b == last_box) {
						assert_pixmap_contains_boxes(pixmap, box, last_box-box, 0, 0);
						fill.boxes(sna, &fill, box, last_box - box);
						sna_damage_add_boxes(damage, box, last_box - box, 0, 0);
						b = box;
					}
				}
			} while (--n);
			RegionUninit(&clip);
		}
		if (b != box) {
			assert_pixmap_contains_boxes(pixmap, box, b-box, 0, 0);
			fill.boxes(sna, &fill, box, b - box);
			sna_damage_add_boxes(damage, box, b - box, 0, 0);
		}
		goto done;
	}

done:
	fill.done(sna, &fill);
	assert_pixmap_damage(pixmap);
	return true;
}

static bool
sna_poly_fill_rect_tiled_blt(DrawablePtr drawable,
			     struct kgem_bo *bo,
			     struct sna_damage **damage,
			     GCPtr gc, int n, xRectangle *rect,
			     const BoxRec *extents, unsigned clipped);

static bool
sna_poly_fill_rect_stippled_blt(DrawablePtr drawable,
				struct kgem_bo *bo,
				struct sna_damage **damage,
				GCPtr gc, int n, xRectangle *rect,
				const BoxRec *extents, unsigned clipped);

static inline bool
gc_is_solid(GCPtr gc, uint32_t *color)
{
	assert(FbFullMask(gc->depth) == (FbFullMask(gc->depth) & gc->planemask));

	if (gc->alu == GXclear) {
		*color = 0;
		return true;
	}
	if (gc->alu == GXset) {
		*color = (1 << gc->depth) - 1;
		return true;
	}

	if (gc->fillStyle == FillSolid ||
	    (gc->fillStyle == FillTiled && gc->tileIsPixel) ||
	    (gc->fillStyle == FillOpaqueStippled && gc->bgPixel == gc->fgPixel)) {
		*color = gc->fillStyle == FillTiled ? gc->tile.pixel : gc->fgPixel;
		return true;
	}

	return false;
}

static void
sna_fill_spans__gpu(DrawablePtr drawable, GCPtr gc, int n,
		    DDXPointPtr pt, int *width, int sorted)
{
	struct sna_fill_spans *data = sna_gc(gc)->priv;
	uint32_t color;

	DBG(("%s(n=%d, pt[0]=(%d, %d)+%d, sorted=%d\n",
	     __FUNCTION__, n, pt[0].x, pt[0].y, width[0], sorted));

	assert(PM_IS_SOLID(drawable, gc->planemask));
	if (n == 0)
		return;

	/* The mi routines do not attempt to keep the spans it generates
	 * within the clip, so we must run them through the clipper.
	 */

	if (gc_is_solid(gc, &color)) {
		sna_fill_spans_blt(drawable,
				   data->bo, NULL,
				   gc, color, n, pt, width, sorted,
				   &data->region.extents, 2);
	} else {
		/* Try converting these to a set of rectangles instead */
		xRectangle *rect;
		int i;

		DBG(("%s: converting to rectagnles\n", __FUNCTION__));

		rect = malloc (n * sizeof (xRectangle));
		if (rect == NULL)
			return;

		for (i = 0; i < n; i++) {
			rect[i].x = pt[i].x - drawable->x;
			rect[i].width = width[i];
			rect[i].y = pt[i].y - drawable->y;
			rect[i].height = 1;
		}

		if (gc->fillStyle == FillTiled) {
			(void)sna_poly_fill_rect_tiled_blt(drawable,
							   data->bo, NULL,
							   gc, n, rect,
							   &data->region.extents, 2);
		} else {
			(void)sna_poly_fill_rect_stippled_blt(drawable,
							      data->bo, NULL,
							      gc, n, rect,
							      &data->region.extents, 2);
		}
		free (rect);
	}
}

static unsigned
sna_spans_extents(DrawablePtr drawable, GCPtr gc,
		  int n, DDXPointPtr pt, int *width,
		  BoxPtr out)
{
	BoxRec box;
	bool clipped = false;

	if (n == 0)
		return 0;

	box.x1 = pt->x;
	box.x2 = box.x1 + *width;
	box.y2 = box.y1 = pt->y;

	while (--n) {
		pt++;
		width++;
		if (box.x1 > pt->x)
			box.x1 = pt->x;
		if (box.x2 < pt->x + *width)
			box.x2 = pt->x + *width;

		if (box.y1 > pt->y)
			box.y1 = pt->y;
		else if (box.y2 < pt->y)
			box.y2 = pt->y;
	}
	box.y2++;

	if (gc)
		clipped = clip_box(&box, gc);
	if (box_empty(&box))
		return 0;

	*out = box;
	return 1 | clipped << 1;
}

static void
sna_fill_spans(DrawablePtr drawable, GCPtr gc, int n,
	       DDXPointPtr pt, int *width, int sorted)
{
	PixmapPtr pixmap = get_drawable_pixmap(drawable);
	struct sna *sna = to_sna_from_pixmap(pixmap);
	struct sna_damage **damage;
	struct kgem_bo *bo;
	RegionRec region;
	unsigned flags;
	uint32_t color;

	DBG(("%s(n=%d, pt[0]=(%d, %d)+%d, sorted=%d\n",
	     __FUNCTION__, n, pt[0].x, pt[0].y, width[0], sorted));

	flags = sna_spans_extents(drawable, gc, n, pt, width, &region.extents);
	if (flags == 0)
		return;

	DBG(("%s: extents (%d, %d), (%d, %d)\n", __FUNCTION__,
	     region.extents.x1, region.extents.y1,
	     region.extents.x2, region.extents.y2));

	if (FORCE_FALLBACK)
		goto fallback;

	if (!ACCEL_FILL_SPANS)
		goto fallback;

	if (wedged(sna)) {
		DBG(("%s: fallback -- wedged\n", __FUNCTION__));
		goto fallback;
	}

	DBG(("%s: fillStyle=%x [%d], mask=%lx [%d]\n", __FUNCTION__,
	     gc->fillStyle, gc->fillStyle == FillSolid,
	     gc->planemask, PM_IS_SOLID(drawable, gc->planemask)));
	if (!PM_IS_SOLID(drawable, gc->planemask))
		goto fallback;

	bo = sna_drawable_use_bo(drawable, PREFER_GPU,
				 &region.extents, &damage);
	if (bo) {
		if (gc_is_solid(gc, &color)) {
			DBG(("%s: trying solid fill [alu=%d, pixel=%08lx] blt paths\n",
			     __FUNCTION__, gc->alu, gc->fgPixel));

			sna_fill_spans_blt(drawable,
					   bo, damage,
					   gc, color, n, pt, width, sorted,
					   &region.extents, flags & 2);
		} else {
			/* Try converting these to a set of rectangles instead */
			xRectangle *rect;
			int i;

			DBG(("%s: converting to rectagnles\n", __FUNCTION__));

			rect = malloc (n * sizeof (xRectangle));
			if (rect == NULL)
				return;

			for (i = 0; i < n; i++) {
				rect[i].x = pt[i].x - drawable->x;
				rect[i].width = width[i];
				rect[i].y = pt[i].y - drawable->y;
				rect[i].height = 1;
			}

			if (gc->fillStyle == FillTiled) {
				i = sna_poly_fill_rect_tiled_blt(drawable,
								 bo, damage,
								 gc, n, rect,
								 &region.extents, flags & 2);
			} else {
				i = sna_poly_fill_rect_stippled_blt(drawable,
								    bo, damage,
								    gc, n, rect,
								    &region.extents, flags & 2);
			}
			free (rect);

			if (i)
				return;
		}
	}

fallback:
	DBG(("%s: fallback\n", __FUNCTION__));
	region.data = NULL;
	if (!region_maybe_clip(&region, gc->pCompositeClip))
		return;

	if (!sna_gc_move_to_cpu(gc, drawable, &region))
		goto out;
	if (!sna_drawable_move_region_to_cpu(drawable, &region,
					     drawable_gc_flags(drawable, gc, n > 1)))
		goto out;

	if (sigtrap_get() == 0) {
		DBG(("%s: fbFillSpans\n", __FUNCTION__));
		fbFillSpans(drawable, gc, n, pt, width, sorted);
		FALLBACK_FLUSH(drawable);
		sigtrap_put();
	}
out:
	sna_gc_move_to_gpu(gc);
	RegionUninit(&region);
}

static void
sna_set_spans(DrawablePtr drawable, GCPtr gc, char *src,
	      DDXPointPtr pt, int *width, int n, int sorted)
{
	RegionRec region;

	if (sna_spans_extents(drawable, gc, n, pt, width, &region.extents) == 0)
		return;

	DBG(("%s: extents=(%d, %d), (%d, %d)\n", __FUNCTION__,
	     region.extents.x1, region.extents.y1,
	     region.extents.x2, region.extents.y2));

	if (FORCE_FALLBACK)
		goto fallback;

	if (!ACCEL_SET_SPANS)
		goto fallback;

fallback:
	region.data = NULL;
	if (!region_maybe_clip(&region, gc->pCompositeClip))
		return;

	if (!sna_gc_move_to_cpu(gc, drawable, &region))
		goto out;
	if (!sna_drawable_move_region_to_cpu(drawable, &region,
					     drawable_gc_flags(drawable, gc, n > 1)))
		goto out;

	if (sigtrap_get() == 0) {
		DBG(("%s: fbSetSpans\n", __FUNCTION__));
		fbSetSpans(drawable, gc, src, pt, width, n, sorted);
		FALLBACK_FLUSH(drawable);
		sigtrap_put();
	}
out:
	sna_gc_move_to_gpu(gc);
	RegionUninit(&region);
}

struct sna_copy_plane {
	struct sna_damage **damage;
	struct kgem_bo *bo;
};

static void
sna_copy_bitmap_blt(DrawablePtr _bitmap, DrawablePtr drawable, GCPtr gc,
		    RegionRec *region, int sx, int sy,
		    Pixel bitplane, void *closure)
{
	PixmapPtr pixmap = get_drawable_pixmap(drawable);
	struct sna *sna = to_sna_from_pixmap(pixmap);
	struct sna_copy_plane *arg = closure;
	PixmapPtr bitmap = (PixmapPtr)_bitmap;
	uint32_t br00, br13;
	int16_t dx, dy;
	const BoxRec *box;
	int n;

	DBG(("%s: plane=%x (%d,%d),(%d,%d)xld\n",
	     __FUNCTION__, (unsigned)bitplane,
	     region->extents.x1, region->extents.y1,
	     region->extents.x2, region->extents.y2,
	     region_num_rects(region)));

	box = region_rects(region);
	n = region_num_rects(region);
	assert(n);

	get_drawable_deltas(drawable, pixmap, &dx, &dy);
	assert_pixmap_contains_boxes(pixmap, box, n, dx, dy);

	br00 = 3 << 20;
	br13 = arg->bo->pitch;
	if (sna->kgem.gen >= 040 && arg->bo->tiling) {
		br00 |= BLT_DST_TILED;
		br13 >>= 2;
	}
	br13 |= blt_depth(drawable->depth) << 24;
	br13 |= copy_ROP[gc->alu] << 16;

	kgem_set_mode(&sna->kgem, KGEM_BLT, arg->bo);
	do {
		int bx1 = (box->x1 + sx) & ~7;
		int bx2 = (box->x2 + sx + 7) & ~7;
		int bw = (bx2 - bx1)/8;
		int bh = box->y2 - box->y1;
		int bstride = ALIGN(bw, 2);
		int src_stride;
		uint8_t *dst, *src;
		uint32_t *b;

		DBG(("%s: box(%d, %d), (%d, %d), sx=(%d,%d) bx=[%d, %d]\n",
		     __FUNCTION__,
		     box->x1, box->y1,
		     box->x2, box->y2,
		     sx, sy, bx1, bx2));

		src_stride = bstride*bh;
		assert(src_stride > 0);
		if (src_stride <= 128) {
			src_stride = ALIGN(src_stride, 8) / 4;
			assert(src_stride <= 32);
			if (!kgem_check_batch(&sna->kgem, 8+src_stride) ||
			    !kgem_check_bo_fenced(&sna->kgem, arg->bo) ||
			    !kgem_check_reloc(&sna->kgem, 1)) {
				kgem_submit(&sna->kgem);
				if (!kgem_check_bo_fenced(&sna->kgem, arg->bo))
					return; /* XXX fallback? */
				_kgem_set_mode(&sna->kgem, KGEM_BLT);
			}

			assert(sna->kgem.mode == KGEM_BLT);
			if (sna->kgem.gen >= 0100) {
				b = sna->kgem.batch + sna->kgem.nbatch;
				b[0] = XY_MONO_SRC_COPY_IMM | (6 + src_stride) | br00;
				b[0] |= ((box->x1 + sx) & 7) << 17;
				b[1] = br13;
				b[2] = (box->y1 + dy) << 16 | (box->x1 + dx);
				b[3] = (box->y2 + dy) << 16 | (box->x2 + dx);
				*(uint64_t *)(b+4) =
					kgem_add_reloc64(&sna->kgem, sna->kgem.nbatch + 4, arg->bo,
							 I915_GEM_DOMAIN_RENDER << 16 |
							 I915_GEM_DOMAIN_RENDER |
							 KGEM_RELOC_FENCED,
							 0);
				b[5] = gc->bgPixel;
				b[6] = gc->fgPixel;

				dst = (uint8_t *)&b[8];
				sna->kgem.nbatch += 8 + src_stride;
			} else {
				b = sna->kgem.batch + sna->kgem.nbatch;
				b[0] = XY_MONO_SRC_COPY_IMM | (5 + src_stride) | br00;
				b[0] |= ((box->x1 + sx) & 7) << 17;
				b[1] = br13;
				b[2] = (box->y1 + dy) << 16 | (box->x1 + dx);
				b[3] = (box->y2 + dy) << 16 | (box->x2 + dx);
				b[4] = kgem_add_reloc(&sna->kgem, sna->kgem.nbatch + 4, arg->bo,
						      I915_GEM_DOMAIN_RENDER << 16 |
						      I915_GEM_DOMAIN_RENDER |
						      KGEM_RELOC_FENCED,
						      0);
				b[5] = gc->bgPixel;
				b[6] = gc->fgPixel;

				dst = (uint8_t *)&b[7];
				sna->kgem.nbatch += 7 + src_stride;
			}

			assert(bitmap->devKind);
			src_stride = bitmap->devKind;
			src = bitmap->devPrivate.ptr;
			src += (box->y1 + sy) * src_stride + bx1/8;
			src_stride -= bstride;
			do {
				int i = bstride;
				assert(src >= (uint8_t *)bitmap->devPrivate.ptr);
				do {
					*dst++ = byte_reverse(*src++);
					*dst++ = byte_reverse(*src++);
					i -= 2;
				} while (i);
				assert(src <= (uint8_t *)bitmap->devPrivate.ptr + bitmap->devKind * bitmap->drawable.height);
				src += src_stride;
			} while (--bh);
		} else {
			struct kgem_bo *upload;
			void *ptr;

			if (!kgem_check_batch(&sna->kgem, 10) ||
			    !kgem_check_bo_fenced(&sna->kgem, arg->bo) ||
			    !kgem_check_reloc_and_exec(&sna->kgem, 2)) {
				kgem_submit(&sna->kgem);
				if (!kgem_check_bo_fenced(&sna->kgem, arg->bo))
					return; /* XXX fallback? */
				_kgem_set_mode(&sna->kgem, KGEM_BLT);
			}

			upload = kgem_create_buffer(&sna->kgem,
						    bstride*bh,
						    KGEM_BUFFER_WRITE_INPLACE,
						    &ptr);
			if (!upload)
				break;

			if (sigtrap_get() == 0) {
				assert(sna->kgem.mode == KGEM_BLT);
				b = sna->kgem.batch + sna->kgem.nbatch;
				if (sna->kgem.gen >= 0100) {
					b[0] = XY_MONO_SRC_COPY | br00 | 8;
					b[0] |= ((box->x1 + sx) & 7) << 17;
					b[1] = br13;
					b[2] = (box->y1 + dy) << 16 | (box->x1 + dx);
					b[3] = (box->y2 + dy) << 16 | (box->x2 + dx);
					*(uint64_t *)(b+4) =
						kgem_add_reloc64(&sna->kgem, sna->kgem.nbatch + 4, arg->bo,
								I915_GEM_DOMAIN_RENDER << 16 |
								I915_GEM_DOMAIN_RENDER |
								KGEM_RELOC_FENCED,
								0);
					*(uint64_t *)(b+6) =
						kgem_add_reloc64(&sna->kgem, sna->kgem.nbatch + 6, upload,
								I915_GEM_DOMAIN_RENDER << 16 |
								KGEM_RELOC_FENCED,
								0);
					b[8] = gc->bgPixel;
					b[9] = gc->fgPixel;

					sna->kgem.nbatch += 10;
				} else {
					b[0] = XY_MONO_SRC_COPY | br00 | 6;
					b[0] |= ((box->x1 + sx) & 7) << 17;
					b[1] = br13;
					b[2] = (box->y1 + dy) << 16 | (box->x1 + dx);
					b[3] = (box->y2 + dy) << 16 | (box->x2 + dx);
					b[4] = kgem_add_reloc(&sna->kgem, sna->kgem.nbatch + 4, arg->bo,
							I915_GEM_DOMAIN_RENDER << 16 |
							I915_GEM_DOMAIN_RENDER |
							KGEM_RELOC_FENCED,
							0);
					b[5] = kgem_add_reloc(&sna->kgem, sna->kgem.nbatch + 5, upload,
							I915_GEM_DOMAIN_RENDER << 16 |
							KGEM_RELOC_FENCED,
							0);
					b[6] = gc->bgPixel;
					b[7] = gc->fgPixel;

					sna->kgem.nbatch += 8;
				}

				dst = ptr;
				assert(bitmap->devKind);
				src_stride = bitmap->devKind;
				src = bitmap->devPrivate.ptr;
				src += (box->y1 + sy) * src_stride + bx1/8;
				src_stride -= bstride;
				do {
					int i = bstride;
					assert(src >= (uint8_t *)bitmap->devPrivate.ptr);
					do {
						*dst++ = byte_reverse(*src++);
						*dst++ = byte_reverse(*src++);
						i -= 2;
					} while (i);
					assert(src <= (uint8_t *)bitmap->devPrivate.ptr + bitmap->devKind * bitmap->drawable.height);
					assert(dst <= (uint8_t *)ptr + kgem_bo_size(upload));
					src += src_stride;
				} while (--bh);

				sigtrap_put();
			}

			kgem_bo_destroy(&sna->kgem, upload);
		}

		box++;
	} while (--n);

	if (arg->damage) {
		RegionTranslate(region, dx, dy);
		sna_damage_add(arg->damage, region);
	}
	assert_pixmap_damage(pixmap);
	sna->blt_state.fill_bo = 0;
}

static void
sna_copy_plane_blt(DrawablePtr source, DrawablePtr drawable, GCPtr gc,
		   RegionPtr region, int sx, int sy,
		   Pixel bitplane, void *closure)
{
	PixmapPtr dst_pixmap = get_drawable_pixmap(drawable);
	PixmapPtr src_pixmap = get_drawable_pixmap(source);
	struct sna *sna = to_sna_from_pixmap(dst_pixmap);
	struct sna_copy_plane *arg = closure;
	int16_t dx, dy;
	int bit = ffs(bitplane) - 1;
	uint32_t br00, br13;
	const BoxRec *box = region_rects(region);
	int n = region_num_rects(region);

	DBG(("%s: plane=%x [%d] x%d\n", __FUNCTION__,
	     (unsigned)bitplane, bit, n));

	if (n == 0)
		return;

	if (get_drawable_deltas(source, src_pixmap, &dx, &dy))
		sx += dx, sy += dy;

	get_drawable_deltas(drawable, dst_pixmap, &dx, &dy);
	assert_pixmap_contains_boxes(dst_pixmap, box, n, dx, dy);

	br00 = XY_MONO_SRC_COPY | 3 << 20;
	br13 = arg->bo->pitch;
	if (sna->kgem.gen >= 040 && arg->bo->tiling) {
		br00 |= BLT_DST_TILED;
		br13 >>= 2;
	}
	br13 |= blt_depth(drawable->depth) << 24;
	br13 |= copy_ROP[gc->alu] << 16;

	kgem_set_mode(&sna->kgem, KGEM_BLT, arg->bo);
	do {
		int bx1 = (box->x1 + sx) & ~7;
		int bx2 = (box->x2 + sx + 7) & ~7;
		int bw = (bx2 - bx1)/8;
		int bh = box->y2 - box->y1;
		int bstride = ALIGN(bw, 2);
		struct kgem_bo *upload;
		void *ptr;

		DBG(("%s: box(%d, %d), (%d, %d), sx=(%d,%d) bx=[%d, %d]\n",
		     __FUNCTION__,
		     box->x1, box->y1,
		     box->x2, box->y2,
		     sx, sy, bx1, bx2));

		if (!kgem_check_batch(&sna->kgem, 10) ||
		    !kgem_check_bo_fenced(&sna->kgem, arg->bo) ||
		    !kgem_check_reloc_and_exec(&sna->kgem, 2)) {
			kgem_submit(&sna->kgem);
			if (!kgem_check_bo_fenced(&sna->kgem, arg->bo))
				return; /* XXX fallback? */
			_kgem_set_mode(&sna->kgem, KGEM_BLT);
		}

		upload = kgem_create_buffer(&sna->kgem,
					    bstride*bh,
					    KGEM_BUFFER_WRITE_INPLACE,
					    &ptr);
		if (!upload)
			break;

		if (sigtrap_get() == 0) {
			uint32_t *b;

			assert(src_pixmap->devKind);
			switch (source->bitsPerPixel) {
			case 32:
				{
					uint32_t *src = src_pixmap->devPrivate.ptr;
					int src_stride = src_pixmap->devKind/sizeof(uint32_t);
					uint8_t *dst = ptr;

					src += (box->y1 + sy) * src_stride;
					src += bx1;

					src_stride -= bw * 8;
					bstride -= bw;

					do {
						int i = bw;
						do {
							uint8_t v = 0;

							v |= ((*src++ >> bit) & 1) << 7;
							v |= ((*src++ >> bit) & 1) << 6;
							v |= ((*src++ >> bit) & 1) << 5;
							v |= ((*src++ >> bit) & 1) << 4;
							v |= ((*src++ >> bit) & 1) << 3;
							v |= ((*src++ >> bit) & 1) << 2;
							v |= ((*src++ >> bit) & 1) << 1;
							v |= ((*src++ >> bit) & 1) << 0;

							*dst++ = v;
						} while (--i);
						dst += bstride;
						src += src_stride;
					} while (--bh);
					break;
				}
			case 16:
				{
					uint16_t *src = src_pixmap->devPrivate.ptr;
					int src_stride = src_pixmap->devKind/sizeof(uint16_t);
					uint8_t *dst = ptr;

					src += (box->y1 + sy) * src_stride;
					src += bx1;

					src_stride -= bw * 8;
					bstride -= bw;

					do {
						int i = bw;
						do {
							uint8_t v = 0;

							v |= ((*src++ >> bit) & 1) << 7;
							v |= ((*src++ >> bit) & 1) << 6;
							v |= ((*src++ >> bit) & 1) << 5;
							v |= ((*src++ >> bit) & 1) << 4;
							v |= ((*src++ >> bit) & 1) << 3;
							v |= ((*src++ >> bit) & 1) << 2;
							v |= ((*src++ >> bit) & 1) << 1;
							v |= ((*src++ >> bit) & 1) << 0;

							*dst++ = v;
						} while (--i);
						dst += bstride;
						src += src_stride;
					} while (--bh);
					break;
				}
			default:
				assert(0);
			case 8:
				{
					uint8_t *src = src_pixmap->devPrivate.ptr;
					int src_stride = src_pixmap->devKind/sizeof(uint8_t);
					uint8_t *dst = ptr;

					src += (box->y1 + sy) * src_stride;
					src += bx1;

					src_stride -= bw * 8;
					bstride -= bw;

					do {
						int i = bw;
						do {
							uint8_t v = 0;

							v |= ((*src++ >> bit) & 1) << 7;
							v |= ((*src++ >> bit) & 1) << 6;
							v |= ((*src++ >> bit) & 1) << 5;
							v |= ((*src++ >> bit) & 1) << 4;
							v |= ((*src++ >> bit) & 1) << 3;
							v |= ((*src++ >> bit) & 1) << 2;
							v |= ((*src++ >> bit) & 1) << 1;
							v |= ((*src++ >> bit) & 1) << 0;

							*dst++ = v;
						} while (--i);
						dst += bstride;
						src += src_stride;
					} while (--bh);
					break;
				}
			}

			assert(sna->kgem.mode == KGEM_BLT);
			b = sna->kgem.batch + sna->kgem.nbatch;
			if (sna->kgem.gen >= 0100) {
				b[0] = br00 | ((box->x1 + sx) & 7) << 17 | 8;
				b[1] = br13;
				b[2] = (box->y1 + dy) << 16 | (box->x1 + dx);
				b[3] = (box->y2 + dy) << 16 | (box->x2 + dx);
				*(uint64_t *)(b+4) =
					kgem_add_reloc64(&sna->kgem, sna->kgem.nbatch + 4, arg->bo,
							I915_GEM_DOMAIN_RENDER << 16 |
							I915_GEM_DOMAIN_RENDER |
							KGEM_RELOC_FENCED,
							0);
				*(uint64_t *)(b+6) =
					kgem_add_reloc64(&sna->kgem, sna->kgem.nbatch + 6, upload,
							I915_GEM_DOMAIN_RENDER << 16 |
							KGEM_RELOC_FENCED,
							0);
				b[8] = gc->bgPixel;
				b[9] = gc->fgPixel;

				sna->kgem.nbatch += 10;
			} else {
				b[0] = br00 | ((box->x1 + sx) & 7) << 17 | 6;
				b[1] = br13;
				b[2] = (box->y1 + dy) << 16 | (box->x1 + dx);
				b[3] = (box->y2 + dy) << 16 | (box->x2 + dx);
				b[4] = kgem_add_reloc(&sna->kgem, sna->kgem.nbatch + 4, arg->bo,
						I915_GEM_DOMAIN_RENDER << 16 |
						I915_GEM_DOMAIN_RENDER |
						KGEM_RELOC_FENCED,
						0);
				b[5] = kgem_add_reloc(&sna->kgem, sna->kgem.nbatch + 5, upload,
						I915_GEM_DOMAIN_RENDER << 16 |
						KGEM_RELOC_FENCED,
						0);
				b[6] = gc->bgPixel;
				b[7] = gc->fgPixel;

				sna->kgem.nbatch += 8;
			}
			sigtrap_put();
		}
		kgem_bo_destroy(&sna->kgem, upload);

		box++;
	} while (--n);

	if (arg->damage) {
		RegionTranslate(region, dx, dy);
		sna_damage_add(arg->damage, region);
	}
	assert_pixmap_damage(dst_pixmap);
	sna->blt_state.fill_bo = 0;
}

static RegionPtr
sna_copy_plane(DrawablePtr src, DrawablePtr dst, GCPtr gc,
	       int src_x, int src_y,
	       int w, int h,
	       int dst_x, int dst_y,
	       unsigned long bit)
{
	PixmapPtr pixmap = get_drawable_pixmap(dst);
	struct sna *sna = to_sna_from_pixmap(pixmap);
	RegionRec region, *ret = NULL;
	struct sna_copy_plane arg;

	DBG(("%s: src=(%d, %d), dst=(%d, %d), size=%dx%d\n", __FUNCTION__,
	     src_x, src_y, dst_x, dst_y, w, h));

	if (gc->planemask == 0)
		goto empty;

	if (src->bitsPerPixel == 1 && (bit&1) == 0)
		goto empty;

	region.extents.x1 = dst_x + dst->x;
	region.extents.y1 = dst_y + dst->y;
	region.extents.x2 = region.extents.x1 + w;
	region.extents.y2 = region.extents.y1 + h;
	region.data = NULL;
	RegionIntersect(&region, &region, gc->pCompositeClip);

	DBG(("%s: dst extents (%d, %d), (%d, %d)\n",
	     __FUNCTION__,
	     region.extents.x1, region.extents.y1,
	     region.extents.x2, region.extents.y2));

	{
		RegionRec clip;

		clip.extents.x1 = src->x - (src->x + src_x) + (dst->x + dst_x);
		clip.extents.y1 = src->y - (src->y + src_y) + (dst->y + dst_y);
		clip.extents.x2 = clip.extents.x1 + src->width;
		clip.extents.y2 = clip.extents.y1 + src->height;
		clip.data = NULL;

		DBG(("%s: src extents (%d, %d), (%d, %d)\n",
		     __FUNCTION__,
		     clip.extents.x1, clip.extents.y1,
		     clip.extents.x2, clip.extents.y2));

		RegionIntersect(&region, &region, &clip);
	}
	DBG(("%s: dst^src extents (%d, %d), (%d, %d)\n",
	     __FUNCTION__,
	     region.extents.x1, region.extents.y1,
	     region.extents.x2, region.extents.y2));
	if (box_empty(&region.extents))
		goto empty;

	RegionTranslate(&region,
			src_x - dst_x - dst->x + src->x,
			src_y - dst_y - dst->y + src->y);

	if (!sna_drawable_move_region_to_cpu(src, &region, MOVE_READ))
		goto out;

	RegionTranslate(&region,
			-(src_x - dst_x - dst->x + src->x),
			-(src_y - dst_y - dst->y + src->y));

	if (FORCE_FALLBACK)
		goto fallback;

	if (!ACCEL_COPY_PLANE)
		goto fallback;

	if (wedged(sna))
		goto fallback;

	if (!PM_IS_SOLID(dst, gc->planemask))
		goto fallback;

	arg.bo = sna_drawable_use_bo(dst, PREFER_GPU,
				     &region.extents, &arg.damage);
	if (arg.bo) {
		if (arg.bo->tiling == I915_TILING_Y) {
			assert(arg.bo == __sna_pixmap_get_bo(pixmap));
			arg.bo = sna_pixmap_change_tiling(pixmap, I915_TILING_X);
			if (arg.bo == NULL) {
				DBG(("%s: fallback -- unable to change tiling\n",
				     __FUNCTION__));
				goto fallback;
			}
		}
		RegionUninit(&region);
		return sna_do_copy(src, dst, gc,
				   src_x, src_y,
				   w, h,
				   dst_x, dst_y,
				   src->depth == 1 ? sna_copy_bitmap_blt : sna_copy_plane_blt,
				   (Pixel)bit, &arg);
	}

fallback:
	DBG(("%s: fallback\n", __FUNCTION__));
	if (!sna_gc_move_to_cpu(gc, dst, &region))
		goto out;
	if (!sna_drawable_move_region_to_cpu(dst, &region,
					     drawable_gc_flags(dst, gc, false)))
		goto out;

	if (sigtrap_get() == 0) {
		DBG(("%s: fbCopyPlane(%d, %d, %d, %d, %d,%d) %x\n",
		     __FUNCTION__, src_x, src_y, w, h, dst_x, dst_y, (unsigned)bit));
		ret = miDoCopy(src, dst, gc,
			       src_x, src_y, w, h, dst_x, dst_y,
			       src->bitsPerPixel > 1 ? fbCopyNto1 : fbCopy1toN,
			       bit, 0);
		FALLBACK_FLUSH(dst);
		sigtrap_put();
	}
out:
	sna_gc_move_to_gpu(gc);
	RegionUninit(&region);
	return ret;
empty:
	return miHandleExposures(src, dst, gc,
				 src_x, src_y,
				 w, h,
				 dst_x, dst_y, bit);
}

static bool
sna_poly_point_blt(DrawablePtr drawable,
		   struct kgem_bo *bo,
		   struct sna_damage **damage,
		   GCPtr gc, int mode, int n, DDXPointPtr pt,
		   bool clipped)
{
	PixmapPtr pixmap = get_drawable_pixmap(drawable);
	struct sna *sna = to_sna_from_pixmap(pixmap);
	BoxRec box[512], *b = box, * const last_box = box + ARRAY_SIZE(box);
	struct sna_fill_op fill;
	DDXPointRec last;
	int16_t dx, dy;

	DBG(("%s: alu=%d, pixel=%08lx, clipped?=%d\n",
	     __FUNCTION__, gc->alu, gc->fgPixel, clipped));

	if (!sna_fill_init_blt(&fill, sna, pixmap, bo, gc->alu, gc->fgPixel, FILL_POINTS))
		return false;

	get_drawable_deltas(drawable, pixmap, &dx, &dy);

	last.x = drawable->x;
	last.y = drawable->y;

	if (!clipped) {
		last.x += dx;
		last.y += dy;

		assert_pixmap_contains_points(pixmap, pt, n, last.x, last.y);
		sna_damage_add_points(damage, pt, n, last.x, last.y);
		if (fill.points && mode != CoordModePrevious) {
			fill.points(sna, &fill, last.x, last.y, pt, n);
		} else {
			do {
				unsigned nbox = n;
				if (nbox > ARRAY_SIZE(box))
					nbox = ARRAY_SIZE(box);
				n -= nbox;
				do {
					*(DDXPointRec *)b = *pt++;

					b->x1 += last.x;
					b->y1 += last.y;
					if (mode == CoordModePrevious)
						last = *(DDXPointRec *)b;

					b->x2 = b->x1 + 1;
					b->y2 = b->y1 + 1;
					b++;
				} while (--nbox);
				fill.boxes(sna, &fill, box, b - box);
				b = box;
			} while (n);
		}
	} else {
		RegionPtr clip = gc->pCompositeClip;

		while (n--) {
			int x, y;

			x = pt->x;
			y = pt->y;
			pt++;
			if (mode == CoordModePrevious) {
				x += last.x;
				y += last.y;
				last.x = x;
				last.y = y;
			} else {
				x += drawable->x;
				y += drawable->y;
			}

			if (RegionContainsPoint(clip, x, y, NULL)) {
				b->x1 = x + dx;
				b->y1 = y + dy;
				b->x2 = b->x1 + 1;
				b->y2 = b->y1 + 1;
				if (++b == last_box){
					assert_pixmap_contains_boxes(pixmap, box, last_box-box, 0, 0);
					fill.boxes(sna, &fill, box, last_box - box);
					if (damage)
						sna_damage_add_boxes(damage, box, last_box-box, 0, 0);
					b = box;
				}
			}
		}
		if (b != box){
			assert_pixmap_contains_boxes(pixmap, box, b-box, 0, 0);
			fill.boxes(sna, &fill, box, b - box);
			if (damage)
				sna_damage_add_boxes(damage, box, b-box, 0, 0);
		}
	}
	fill.done(sna, &fill);
	assert_pixmap_damage(pixmap);
	return true;
}

static unsigned
sna_poly_point_extents(DrawablePtr drawable, GCPtr gc,
		       int mode, int n, DDXPointPtr pt, BoxPtr out)
{
	BoxRec box;
	bool clipped;

	if (n == 0)
		return 0;

	box.x2 = box.x1 = pt->x;
	box.y2 = box.y1 = pt->y;
	if (mode == CoordModePrevious) {
		DDXPointRec last = *pt++;
		while (--n) {
			last.x += pt->x;
			last.y += pt->y;
			pt++;
			box_add_pt(&box, last.x, last.y);
		}
	} else {
		--n; ++pt;
		while (n >= 8) {
			box_add_pt(&box, pt[0].x, pt[0].y);
			box_add_pt(&box, pt[1].x, pt[1].y);
			box_add_pt(&box, pt[2].x, pt[2].y);
			box_add_pt(&box, pt[3].x, pt[3].y);
			box_add_pt(&box, pt[4].x, pt[4].y);
			box_add_pt(&box, pt[5].x, pt[5].y);
			box_add_pt(&box, pt[6].x, pt[6].y);
			box_add_pt(&box, pt[7].x, pt[7].y);
			pt += 8;
			n -= 8;
		}
		if (n & 4) {
			box_add_pt(&box, pt[0].x, pt[0].y);
			box_add_pt(&box, pt[1].x, pt[1].y);
			box_add_pt(&box, pt[2].x, pt[2].y);
			box_add_pt(&box, pt[3].x, pt[3].y);
			pt += 4;
		}
		if (n & 2) {
			box_add_pt(&box, pt[0].x, pt[0].y);
			box_add_pt(&box, pt[1].x, pt[1].y);
			pt += 2;
		}
		if (n & 1)
			box_add_pt(&box, pt[0].x, pt[0].y);
	}
	box.x2++;
	box.y2++;

	clipped = trim_and_translate_box(&box, drawable, gc);
	if (box_empty(&box))
		return 0;

	*out = box;
	return 1 | clipped << 1;
}

static void
sna_poly_point(DrawablePtr drawable, GCPtr gc,
	       int mode, int n, DDXPointPtr pt)
{
	PixmapPtr pixmap = get_drawable_pixmap(drawable);
	struct sna *sna = to_sna_from_pixmap(pixmap);
	RegionRec region;
	unsigned flags;

	DBG(("%s(mode=%d, n=%d, pt[0]=(%d, %d)\n",
	     __FUNCTION__, mode, n, pt[0].x, pt[0].y));

	flags = sna_poly_point_extents(drawable, gc, mode, n, pt, &region.extents);
	if (flags == 0)
		return;

	DBG(("%s: extents (%d, %d), (%d, %d), flags=%x\n", __FUNCTION__,
	     region.extents.x1, region.extents.y1,
	     region.extents.x2, region.extents.y2,
	     flags));

	if (FORCE_FALLBACK)
		goto fallback;

	if (!ACCEL_POLY_POINT)
		goto fallback;

	if (wedged(sna)) {
		DBG(("%s: fallback -- wedged\n", __FUNCTION__));
		goto fallback;
	}

	if (PM_IS_SOLID(drawable, gc->planemask)) {
		struct sna_damage **damage;
		struct kgem_bo *bo;

		DBG(("%s: trying solid fill [%08lx] blt paths\n",
		     __FUNCTION__, gc->fgPixel));

		if ((bo = sna_drawable_use_bo(drawable, PREFER_GPU,
					      &region.extents, &damage)) &&
		    sna_poly_point_blt(drawable, bo, damage,
				       gc, mode, n, pt, flags & 2))
			return;
	}

fallback:
	DBG(("%s: fallback\n", __FUNCTION__));
	region.data = NULL;
	if (!region_maybe_clip(&region, gc->pCompositeClip))
		return;

	if (!sna_gc_move_to_cpu(gc, drawable, &region))
		goto out;
	if (!sna_drawable_move_region_to_cpu(drawable, &region,
					     MOVE_READ | MOVE_WRITE))
		goto out;

	if (sigtrap_get() == 0) {
		DBG(("%s: fbPolyPoint\n", __FUNCTION__));
		fbPolyPoint(drawable, gc, mode, n, pt, flags);
		FALLBACK_FLUSH(drawable);
		sigtrap_put();
	}
out:
	sna_gc_move_to_gpu(gc);
	RegionUninit(&region);
}

static bool
sna_poly_zero_line_blt(DrawablePtr drawable,
		       struct kgem_bo *bo,
		       struct sna_damage **damage,
		       GCPtr gc, int mode, const int _n, const DDXPointRec * const _pt,
		       const BoxRec *extents, unsigned clipped)
{
	static void * const _jump[] = {
		&&no_damage,
		&&damage,

		&&no_damage_offset,
		&&damage_offset,
	};

	PixmapPtr pixmap = get_drawable_pixmap(drawable);
	struct sna *sna = to_sna_from_pixmap(pixmap);
	int x2, y2, xstart, ystart, oc2;
	unsigned int bias = miGetZeroLineBias(drawable->pScreen);
	bool degenerate = true;
	struct sna_fill_op fill;
	RegionRec clip;
	BoxRec box[512], *b, * const last_box = box + ARRAY_SIZE(box);
	const BoxRec *last_extents;
	int16_t dx, dy;
	void *jump, *ret;

	DBG(("%s: alu=%d, pixel=%lx, n=%d, clipped=%d, damage=%p\n",
	     __FUNCTION__, gc->alu, gc->fgPixel, _n, clipped, damage));
	if (!sna_fill_init_blt(&fill, sna, pixmap, bo, gc->alu, gc->fgPixel, FILL_SPANS))
		return false;

	get_drawable_deltas(drawable, pixmap, &dx, &dy);

	region_set(&clip, extents);
	if (clipped) {
		if (!region_maybe_clip(&clip, gc->pCompositeClip))
			return true;
	}

	jump = _jump[(damage != NULL) | !!(dx|dy) << 1];
	DBG(("%s: [clipped=%x] extents=(%d, %d), (%d, %d), delta=(%d, %d), damage=%p\n",
	     __FUNCTION__, clipped,
	     clip.extents.x1, clip.extents.y1,
	     clip.extents.x2, clip.extents.y2,
	     dx, dy, damage));

	extents = region_rects(&clip);
	last_extents = extents + region_num_rects(&clip);

	b = box;
	do {
		int n = _n;
		const DDXPointRec *pt = _pt;

		xstart = pt->x + drawable->x;
		ystart = pt->y + drawable->y;

		x2 = xstart;
		y2 = ystart;
		oc2 = 0;
		OUTCODES(oc2, x2, y2, extents);

		while (--n) {
			int16_t sdx, sdy;
			int adx, ady, length;
			int e, e1, e2, e3;
			int x1 = x2, x;
			int y1 = y2, y;
			int oc1 = oc2;
			int octant;

			++pt;

			x2 = pt->x;
			y2 = pt->y;
			if (mode == CoordModePrevious) {
				x2 += x1;
				y2 += y1;
			} else {
				x2 += drawable->x;
				y2 += drawable->y;
			}
			DBG(("%s: segment (%d, %d) to (%d, %d)\n",
			     __FUNCTION__, x1, y1, x2, y2));
			if (x2 == x1 && y2 == y1)
				continue;

			degenerate = false;

			oc2 = 0;
			OUTCODES(oc2, x2, y2, extents);
			if (oc1 & oc2)
				continue;

			CalcLineDeltas(x1, y1, x2, y2,
				       adx, ady, sdx, sdy,
				       1, 1, octant);

			DBG(("%s: adx=(%d, %d), sdx=(%d, %d), oc1=%x, oc2=%x\n",
			     __FUNCTION__, adx, ady, sdx, sdy, oc1, oc2));
			if (adx == 0 || ady == 0) {
				if (x1 <= x2) {
					b->x1 = x1;
					b->x2 = x2;
				} else {
					b->x1 = x2;
					b->x2 = x1;
				}
				if (y1 <= y2) {
					b->y1 = y1;
					b->y2 = y2;
				} else {
					b->y1 = y2;
					b->y2 = y1;
				}
				b->x2++;
				b->y2++;
				if (oc1 | oc2) {
					bool intersects;

					intersects = box_intersect(b, extents);
					assert(intersects);
				}
				if (++b == last_box) {
					ret = &&rectangle_continue;
					goto *jump;
rectangle_continue:
					b = box;
				}
			} else if (adx >= ady) {
				int x2_clipped = x2, y2_clipped = y2;
				bool dirty;

				/* X-major segment */
				e1 = ady << 1;
				e2 = e1 - (adx << 1);
				e  = e1 - adx;
				length = adx;

				FIXUP_ERROR(e, octant, bias);

				x = x1;
				y = y1;

				if (oc1 | oc2) {
					int pt1_clipped, pt2_clipped;

					if (miZeroClipLine(extents->x1, extents->y1,
							   extents->x2-1, extents->y2-1,
							   &x, &y, &x2_clipped, &y2_clipped,
							   adx, ady,
							   &pt1_clipped, &pt2_clipped,
							   octant, bias, oc1, oc2) == -1)
						continue;

					length = abs(x2_clipped - x);
					if (length == 0)
						continue;

					if (pt1_clipped) {
						int clipdx = abs(x - x1);
						int clipdy = abs(y - y1);
						e += clipdy * e2 + (clipdx - clipdy) * e1;
					}
				}

				e3 = e2 - e1;
				e  = e - e1;

				b->x1 = x;
				b->y1 = y;
				dirty = false;
				while (length--) {
					e += e1;
					dirty = true;
					if (e >= 0) {
						e += e3;

						if (sdx < 0) {
							b->x2 = b->x1 + 1;
							b->x1 = x;
						} else
							b->x2 = x + 1;
						b->y2 = b->y1 + 1;

						if (++b == last_box) {
							ret = &&X_continue;
							goto *jump;
X_continue:
							b = box;
						}

						b->x1 = x + sdx;
						b->y1 = y += sdy;
						dirty = false;
					}
					x += sdx;
				}
				if (dirty) {
					x -= sdx;
					if (sdx < 0) {
						b->x2 = b->x1 + 1;
						b->x1 = x;
					} else
						b->x2 = x + 1;
					b->y2 = b->y1 + 1;

					if (++b == last_box) {
						ret = &&X2_continue;
						goto *jump;
X2_continue:
						b = box;
					}
				}
			} else {
				int x2_clipped = x2, y2_clipped = y2;
				bool dirty;

				/* Y-major segment */
				e1 = adx << 1;
				e2 = e1 - (ady << 1);
				e  = e1 - ady;
				length  = ady;

				SetYMajorOctant(octant);
				FIXUP_ERROR(e, octant, bias);

				x = x1;
				y = y1;

				if (oc1 | oc2) {
					int pt1_clipped, pt2_clipped;

					if (miZeroClipLine(extents->x1, extents->y1,
							   extents->x2-1, extents->y2-1,
							   &x, &y, &x2_clipped, &y2_clipped,
							   adx, ady,
							   &pt1_clipped, &pt2_clipped,
							   octant, bias, oc1, oc2) == -1)
						continue;

					length = abs(y2_clipped - y);
					if (length == 0)
						continue;

					if (pt1_clipped) {
						int clipdx = abs(x - x1);
						int clipdy = abs(y - y1);
						e += clipdx * e2 + (clipdy - clipdx) * e1;
					}
				}

				e3 = e2 - e1;
				e  = e - e1;

				b->x1 = x;
				b->y1 = y;
				dirty = false;
				while (length--) {
					e += e1;
					dirty = true;
					if (e >= 0) {
						e += e3;

						if (sdy < 0) {
							b->y2 = b->y1 + 1;
							b->y1 = y;
						} else
							b->y2 = y + 1;
						b->x2 = x + 1;

						if (++b == last_box) {
							ret = &&Y_continue;
							goto *jump;
Y_continue:
							b = box;
						}

						b->x1 = x += sdx;
						b->y1 = y + sdy;
						dirty = false;
					}
					y += sdy;
				}

				if (dirty) {
					y -= sdy;
					if (sdy < 0) {
						b->y2 = b->y1 + 1;
						b->y1 = y;
					} else
						b->y2 = y + 1;
					b->x2 = x + 1;

					if (++b == last_box) {
						ret = &&Y2_continue;
						goto *jump;
Y2_continue:
						b = box;
					}
				}
			}
		}

#if 0
		/* Only do the CapNotLast check on the last segment
		 * and only if the endpoint wasn't clipped.  And then, if the last
		 * point is the same as the first point, do not draw it, unless the
		 * line is degenerate
		 */
		if (!pt2_clipped &&
		    gc->capStyle != CapNotLast &&
		    !(xstart == x2 && ystart == y2 && !degenerate))
		{
			b->x2 = x2;
			b->y2 = y2;
			if (b->x2 < b->x1) {
				int16_t t = b->x1;
				b->x1 = b->x2;
				b->x2 = t;
			}
			if (b->y2 < b->y1) {
				int16_t t = b->y1;
				b->y1 = b->y2;
				b->y2 = t;
			}
			b->x2++;
			b->y2++;
			b++;
		}
#endif
	} while (++extents != last_extents);

	if (b != box) {
		ret = &&done;
		goto *jump;
	}

done:
	fill.done(sna, &fill);
	assert_pixmap_damage(pixmap);
	RegionUninit(&clip);
	return true;

damage:
	assert_pixmap_contains_boxes(pixmap, box, b-box, 0, 0);
	sna_damage_add_boxes(damage, box, b-box, 0, 0);
no_damage:
	fill.boxes(sna, &fill, box, b-box);
	goto *ret;

no_damage_offset:
	{
		BoxRec *bb = box;
		do {
			bb->x1 += dx;
			bb->x2 += dx;
			bb->y1 += dy;
			bb->y2 += dy;
		} while (++bb != b);
		assert_pixmap_contains_boxes(pixmap, box, b-box, 0, 0);
		fill.boxes(sna, &fill, box, b - box);
	}
	goto *ret;

damage_offset:
	{
		BoxRec *bb = box;
		do {
			bb->x1 += dx;
			bb->x2 += dx;
			bb->y1 += dy;
			bb->y2 += dy;
		} while (++bb != b);
		assert_pixmap_contains_boxes(pixmap, box, b-box, 0, 0);
		fill.boxes(sna, &fill, box, b - box);
		sna_damage_add_boxes(damage, box, b - box, 0, 0);
	}
	goto *ret;
}

static bool
sna_poly_line_blt(DrawablePtr drawable,
		  struct kgem_bo *bo,
		  struct sna_damage **damage,
		  GCPtr gc, uint32_t pixel,
		  int mode, int n, DDXPointPtr pt,
		  const BoxRec *extents, bool clipped)
{
	PixmapPtr pixmap = get_drawable_pixmap(drawable);
	struct sna *sna = to_sna_from_pixmap(pixmap);
	BoxRec boxes[512], *b = boxes, * const last_box = boxes + ARRAY_SIZE(boxes);
	struct sna_fill_op fill;
	DDXPointRec last;
	int16_t dx, dy;

	DBG(("%s: alu=%d, fg=%08x, clipped=%d\n", __FUNCTION__, gc->alu, (unsigned)pixel, clipped));

	if (!sna_fill_init_blt(&fill, sna, pixmap, bo, gc->alu, pixel, FILL_BOXES))
		return false;

	get_drawable_deltas(drawable, pixmap, &dx, &dy);

	if (!clipped) {
		dx += drawable->x;
		dy += drawable->y;

		last.x = pt->x + dx;
		last.y = pt->y + dy;
		pt++;

		while (--n) {
			DDXPointRec p;

			p = *pt++;
			if (mode == CoordModePrevious) {
				p.x += last.x;
				p.y += last.y;
			} else {
				p.x += dx;
				p.y += dy;
			}
			DBG(("%s: line (%d, %d) -> (%d, %d)\n", __FUNCTION__, last.x, last.y, p.x, p.y));

			if (last.x == p.x) {
				b->x1 = last.x;
				b->x2 = last.x + 1;
			} else if (last.x < p.x) {
				b->x1 = last.x;
				b->x2 = p.x;
			} else {
				b->x1 = p.x;
				b->x2 = last.x;
			}

			if (last.y == p.y) {
				b->y1 = last.y;
				b->y2 = last.y + 1;
			} else if (last.y < p.y) {
				b->y1 = last.y;
				b->y2 = p.y;
			} else {
				b->y1 = p.y;
				b->y2 = last.y;
			}
			b->y2 += last.x == p.x && last.y != p.y;
			b->x2 += last.y == p.y && last.x != p.x;
			DBG(("%s: blt (%d, %d), (%d, %d)\n",
			     __FUNCTION__,
			     b->x1, b->y1, b->x2, b->y2));

			if (++b == last_box) {
				assert_pixmap_contains_boxes(pixmap, boxes, last_box-boxes, 0, 0);
				fill.boxes(sna, &fill, boxes, last_box - boxes);
				if (damage)
					sna_damage_add_boxes(damage, boxes, last_box - boxes, 0, 0);
				b = boxes;
			}

			last = p;
		}
	} else {
		RegionRec clip;

		region_set(&clip, extents);
		if (!region_maybe_clip(&clip, gc->pCompositeClip))
			return true;

		last.x = pt->x + drawable->x;
		last.y = pt->y + drawable->y;
		pt++;

		if (clip.data == NULL) {
			while (--n) {
				DDXPointRec p;

				p = *pt++;
				if (mode == CoordModePrevious) {
					p.x += last.x;
					p.y += last.y;
				} else {
					p.x += drawable->x;
					p.y += drawable->y;
				}
				if (last.x == p.x) {
					b->x1 = last.x;
					b->x2 = last.x + 1;
				} else if (last.x < p.x) {
					b->x1 = last.x;
					b->x2 = p.x;
				} else {
					b->x1 = p.x;
					b->x2 = last.x;
				}
				if (last.y == p.y) {
					b->y1 = last.y;
					b->y2 = last.y + 1;
				} else if (last.y < p.y) {
					b->y1 = last.y;
					b->y2 = p.y;
				} else {
					b->y1 = p.y;
					b->y2 = last.y;
				}
				b->y2 += last.x == p.x && last.y != p.y;
				b->x2 += last.y == p.y && last.x != p.x;
				DBG(("%s: blt (%d, %d), (%d, %d)\n",
				     __FUNCTION__,
				     b->x1, b->y1, b->x2, b->y2));
				if (box_intersect(b, &clip.extents)) {
					b->x1 += dx;
					b->x2 += dx;
					b->y1 += dy;
					b->y2 += dy;
					if (++b == last_box) {
						assert_pixmap_contains_boxes(pixmap, boxes, last_box-boxes, 0, 0);
						fill.boxes(sna, &fill, boxes, last_box - boxes);
						if (damage)
							sna_damage_add_boxes(damage, boxes, last_box - boxes, 0, 0);
						b = boxes;
					}
				}

				last = p;
			}
		} else {
			const BoxRec * const clip_start = RegionBoxptr(&clip);
			const BoxRec * const clip_end = clip_start + clip.data->numRects;
			const BoxRec *c;

			while (--n) {
				DDXPointRec p;
				BoxRec box;

				p = *pt++;
				if (mode == CoordModePrevious) {
					p.x += last.x;
					p.y += last.y;
				} else {
					p.x += drawable->x;
					p.y += drawable->y;
				}
				if (last.x == p.x) {
					box.x1 = last.x;
					box.x2 = last.x + 1;
				} else if (last.x < p.x) {
					box.x1 = last.x;
					box.x2 = p.x;
				} else {
					box.x1 = p.x;
					box.x2 = last.x;
				}
				if (last.y == p.y) {
					box.y1 = last.y;
					box.y2 = last.y + 1;
				} else if (last.y < p.y) {
					box.y1 = last.y;
					box.y2 = p.y;
				} else {
					box.y1 = p.y;
					box.y2 = last.y;
				}
				b->y2 += last.x == p.x && last.y != p.y;
				b->x2 += last.y == p.y && last.x != p.x;
				DBG(("%s: blt (%d, %d), (%d, %d)\n",
				     __FUNCTION__,
				     box.x1, box.y1, box.x2, box.y2));

				c = find_clip_box_for_y(clip_start,
							clip_end,
							box.y1);
				while (c != clip_end) {
					if (box.y2 <= c->y1)
						break;

					*b = box;
					if (box_intersect(b, c++)) {
						b->x1 += dx;
						b->x2 += dx;
						b->y1 += dy;
						b->y2 += dy;
						if (++b == last_box) {
							assert_pixmap_contains_boxes(pixmap, boxes, last_box-boxes, 0, 0);
							fill.boxes(sna, &fill, boxes, last_box-boxes);
							if (damage)
								sna_damage_add_boxes(damage, boxes, last_box-boxes, 0, 0);
							b = boxes;
						}
					}
				}

				last = p;
			}
		}
		RegionUninit(&clip);
	}
	if (b != boxes) {
		assert_pixmap_contains_boxes(pixmap, boxes, b-boxes, 0, 0);
		fill.boxes(sna, &fill, boxes, b - boxes);
		if (damage)
			sna_damage_add_boxes(damage, boxes, b - boxes, 0, 0);
	}
	fill.done(sna, &fill);
	assert_pixmap_damage(pixmap);
	return true;
}

static unsigned
sna_poly_line_extents(DrawablePtr drawable, GCPtr gc,
		      int mode, int n, DDXPointPtr pt,
		      BoxPtr out)
{
	BoxRec box;
	bool clip, blt = true;

	if (n == 0)
		return 0;

	box.x2 = box.x1 = pt->x;
	box.y2 = box.y1 = pt->y;
	if (mode == CoordModePrevious) {
		int x = box.x1;
		int y = box.y1;
		while (--n) {
			pt++;
			x += pt->x;
			y += pt->y;
			if (blt)
				blt &= pt->x == 0 || pt->y == 0;
			box_add_pt(&box, x, y);
		}
	} else {
		int x = box.x1;
		int y = box.y1;
		while (--n) {
			pt++;
			if (blt) {
				blt &= pt->x == x || pt->y == y;
				x = pt->x;
				y = pt->y;
			}
			box_add_pt(&box, pt->x, pt->y);
		}
	}
	box.x2++;
	box.y2++;

	if (gc->lineWidth) {
		int extra = gc->lineWidth >> 1;
		if (n > 1) {
			if (gc->joinStyle == JoinMiter)
				extra = 6 * gc->lineWidth;
			else if (gc->capStyle == CapProjecting)
				extra = gc->lineWidth;
		}
		if (extra) {
			box.x1 -= extra;
			box.x2 += extra;
			box.y1 -= extra;
			box.y2 += extra;
		}
	}

	clip = trim_and_translate_box(&box, drawable, gc);
	if (box_empty(&box))
		return 0;

	*out = box;
	return 1 | blt << 2 | clip << 1;
}

/* Only use our spans code if the destination is busy and we can't perform
 * the operation in place.
 *
 * Currently it looks to be faster to use the GPU for zero spans on all
 * platforms.
 */
inline static int
_use_zero_spans(DrawablePtr drawable, GCPtr gc, const BoxRec *extents)
{
	if (USE_ZERO_SPANS)
		return USE_ZERO_SPANS > 0;

	return !drawable_gc_inplace_hint(drawable, gc);
}

static int
use_zero_spans(DrawablePtr drawable, GCPtr gc, const BoxRec *extents)
{
	bool ret = _use_zero_spans(drawable, gc, extents);
	DBG(("%s? %d\n", __FUNCTION__, ret));
	return ret;
}

/* Only use our spans code if the destination is busy and we can't perform
 * the operation in place.
 *
 * Currently it looks to be faster to use the CPU for wide spans on all
 * platforms, slow MI code. But that does not take into account the true
 * cost of readback?
 */
inline static int
_use_wide_spans(DrawablePtr drawable, GCPtr gc, const BoxRec *extents)
{
	if (USE_WIDE_SPANS)
		return USE_WIDE_SPANS > 0;

	return !drawable_gc_inplace_hint(drawable, gc);
}

static int
use_wide_spans(DrawablePtr drawable, GCPtr gc, const BoxRec *extents)
{
	int ret = _use_wide_spans(drawable, gc, extents);
	DBG(("%s? %d\n", __FUNCTION__, ret));
	return ret;
}

static void
sna_poly_line(DrawablePtr drawable, GCPtr gc,
	      int mode, int n, DDXPointPtr pt)
{
	struct sna_pixmap *priv;
	struct sna_fill_spans data;
	uint32_t color;

	DBG(("%s(mode=%d, n=%d, pt[0]=(%d, %d), lineWidth=%d\n",
	     __FUNCTION__, mode, n, pt[0].x, pt[0].y, gc->lineWidth));

	data.flags = sna_poly_line_extents(drawable, gc, mode, n, pt,
					   &data.region.extents);
	if (data.flags == 0)
		return;

	DBG(("%s: extents (%d, %d), (%d, %d), flags=%x\n", __FUNCTION__,
	     data.region.extents.x1, data.region.extents.y1,
	     data.region.extents.x2, data.region.extents.y2,
	     data.flags));

	data.region.data = NULL;

	if (FORCE_FALLBACK)
		goto fallback;

	if (!ACCEL_POLY_LINE)
		goto fallback;

	data.pixmap = get_drawable_pixmap(drawable);
	data.sna = to_sna_from_pixmap(data.pixmap);
	if (wedged(data.sna)) {
		DBG(("%s: fallback -- wedged\n", __FUNCTION__));
		goto fallback;
	}

	DBG(("%s: fill=%d [%d], line=%d [%d], width=%d, mask=%lx [%d], rectlinear=%d\n",
	     __FUNCTION__,
	     gc->fillStyle, gc->fillStyle == FillSolid,
	     gc->lineStyle, gc->lineStyle == LineSolid,
	     gc->lineWidth,
	     gc->planemask, PM_IS_SOLID(drawable, gc->planemask),
	     data.flags & 4));

	if (!PM_IS_SOLID(drawable, gc->planemask))
		goto fallback;

	priv = sna_pixmap(data.pixmap);
	if (!priv) {
		DBG(("%s: not attached to pixmap %ld\n",
		     __FUNCTION__, data.pixmap->drawable.serialNumber));
		goto fallback;
	}

	if (gc->lineStyle != LineSolid) {
		DBG(("%s: lineStyle, %d, is not solid\n",
		     __FUNCTION__, gc->lineStyle));
		goto spans_fallback;
	}
	if (!(gc->lineWidth == 0 ||
	      (gc->lineWidth == 1 && (n == 1 || gc->alu == GXcopy)))) {
		DBG(("%s: non-zero lineWidth %d\n",
		     __FUNCTION__, gc->lineWidth));
		goto spans_fallback;
	}

	if (gc_is_solid(gc, &color)) {
		DBG(("%s: trying solid fill [%08x]\n",
		     __FUNCTION__, (unsigned)color));

		if (data.flags & 4) {
			data.bo = sna_drawable_use_bo(drawable, PREFER_GPU,
						      &data.region.extents,
						      &data.damage);
			if (data.bo &&
			    sna_poly_line_blt(drawable,
					      data.bo, data.damage,
					      gc, color, mode, n, pt,
					      &data.region.extents,
					      data.flags & 2))
				return;
		} else { /* !rectilinear */
			if ((data.bo = sna_drawable_use_bo(drawable,
							   use_zero_spans(drawable, gc, &data.region.extents),
							   &data.region.extents,
							   &data.damage)) &&
			    sna_poly_zero_line_blt(drawable,
						   data.bo, data.damage,
						   gc, mode, n, pt,
						   &data.region.extents,
						   data.flags & 2))
				return;

		}
	} else if (data.flags & 4) {
		/* Try converting these to a set of rectangles instead */
		data.bo = sna_drawable_use_bo(drawable, PREFER_GPU,
					      &data.region.extents, &data.damage);
		if (data.bo) {
			DDXPointRec p1, p2;
			xRectangle *rect;
			int i;

			DBG(("%s: converting to rectagnles\n", __FUNCTION__));

			rect = malloc (n * sizeof (xRectangle));
			if (rect == NULL)
				return;

			p1 = pt[0];
			for (i = 1; i < n; i++) {
				if (mode == CoordModePrevious) {
					p2.x = p1.x + pt[i].x;
					p2.y = p1.y + pt[i].y;
				} else
					p2 = pt[i];
				if (p1.x < p2.x) {
					rect[i].x = p1.x;
					rect[i].width = p2.x - p1.x + 1;
				} else if (p1.x > p2.x) {
					rect[i].x = p2.x;
					rect[i].width = p1.x - p2.x + 1;
				} else {
					rect[i].x = p1.x;
					rect[i].width = 1;
				}
				if (p1.y < p2.y) {
					rect[i].y = p1.y;
					rect[i].height = p2.y - p1.y + 1;
				} else if (p1.y > p2.y) {
					rect[i].y = p2.y;
					rect[i].height = p1.y - p2.y + 1;
				} else {
					rect[i].y = p1.y;
					rect[i].height = 1;
				}

				/* don't paint last pixel */
				if (gc->capStyle == CapNotLast) {
					if (p1.x == p2.x)
						rect[i].height--;
					else
						rect[i].width--;
				}
				p1 = p2;
			}

			if (gc->fillStyle == FillTiled) {
				i = sna_poly_fill_rect_tiled_blt(drawable,
								 data.bo, data.damage,
								 gc, n - 1, rect + 1,
								 &data.region.extents,
								 data.flags & 2);
			} else {
				i = sna_poly_fill_rect_stippled_blt(drawable,
								    data.bo, data.damage,
								    gc, n - 1, rect + 1,
								    &data.region.extents,
								    data.flags & 2);
			}
			free (rect);

			if (i)
				return;
		}
	}

spans_fallback:
	if ((data.bo = sna_drawable_use_bo(drawable,
					   use_wide_spans(drawable, gc, &data.region.extents),
					   &data.region.extents, &data.damage))) {
		DBG(("%s: converting line into spans\n", __FUNCTION__));
		get_drawable_deltas(drawable, data.pixmap, &data.dx, &data.dy);
		sna_gc(gc)->priv = &data;

		if (gc->lineWidth == 0 && gc_is_solid(gc, &color)) {
			struct sna_fill_op fill;

			if (gc->lineStyle == LineSolid) {
				if (!sna_fill_init_blt(&fill,
						       data.sna, data.pixmap,
						       data.bo, gc->alu, color,
						       FILL_POINTS | FILL_SPANS))
					goto fallback;

				data.op = &fill;

				if ((data.flags & 2) == 0) {
					if (data.dx | data.dy)
						sna_gc_ops__tmp.FillSpans = sna_fill_spans__fill_offset;
					else
						sna_gc_ops__tmp.FillSpans = sna_fill_spans__fill;
					sna_gc_ops__tmp.PolyPoint = sna_poly_point__fill;
				} else {
					if (!region_maybe_clip(&data.region,
							       gc->pCompositeClip))
						return;

					if (region_is_singular(&data.region)) {
						sna_gc_ops__tmp.FillSpans = sna_fill_spans__fill_clip_extents;
						sna_gc_ops__tmp.PolyPoint = sna_poly_point__fill_clip_extents;
					} else {
						sna_gc_ops__tmp.FillSpans = sna_fill_spans__fill_clip_boxes;
						sna_gc_ops__tmp.PolyPoint = sna_poly_point__fill_clip_boxes;
					}
				}
				assert(gc->miTranslate);

				gc->ops = &sna_gc_ops__tmp;
				DBG(("%s: miZeroLine (solid fill)\n", __FUNCTION__));
				miZeroLine(drawable, gc, mode, n, pt);
				fill.done(data.sna, &fill);
			} else {
				data.op = &fill;

				if ((data.flags & 2) == 0) {
					if (data.dx | data.dy)
						sna_gc_ops__tmp.FillSpans = sna_fill_spans__dash_offset;
					else
						sna_gc_ops__tmp.FillSpans = sna_fill_spans__dash;
					sna_gc_ops__tmp.PolyPoint = sna_poly_point__dash;
				} else {
					if (!region_maybe_clip(&data.region,
							       gc->pCompositeClip))
						return;

					if (region_is_singular(&data.region)) {
						sna_gc_ops__tmp.FillSpans = sna_fill_spans__dash_clip_extents;
						sna_gc_ops__tmp.PolyPoint = sna_poly_point__dash_clip_extents;
					} else {
						sna_gc_ops__tmp.FillSpans = sna_fill_spans__dash_clip_boxes;
						sna_gc_ops__tmp.PolyPoint = sna_poly_point__dash_clip_boxes;
					}
				}
				assert(gc->miTranslate);

				DBG(("%s: miZeroLine (solid dash, clipped? %d (complex? %d)), fg pass [%08x]\n",
				     __FUNCTION__,
				     !!(data.flags & 2), data.flags & 2 && !region_is_singular(&data.region),
				     gc->fgPixel));

				if (!sna_fill_init_blt(&fill,
						       data.sna, data.pixmap,
						       data.bo, gc->alu, color,
						       FILL_POINTS | FILL_SPANS))
					goto fallback;

				gc->ops = &sna_gc_ops__tmp;
				data.phase = gc->fgPixel;
				miZeroDashLine(drawable, gc, mode, n, pt);
				fill.done(data.sna, &fill);

				DBG(("%s: miZeroLine (solid dash, clipped? %d (complex? %d)), bg pass [%08x]\n",
				     __FUNCTION__,
				     !!(data.flags & 2), data.flags & 2 && !region_is_singular(&data.region),
				     gc->bgPixel));

				if (sna_fill_init_blt(&fill,
						      data.sna, data.pixmap,
						      data.bo, gc->alu,
						      gc->bgPixel,
						      FILL_POINTS | FILL_SPANS)) {
					data.phase = gc->bgPixel;
					miZeroDashLine(drawable, gc, mode, n, pt);
					fill.done(data.sna, &fill);
				}
			}
		} else {
			/* Note that the WideDash functions alternate
			 * between filling using fgPixel and bgPixel
			 * so we need to reset state between FillSpans and
			 * cannot use the fill fast paths.
			 */
			sna_gc_ops__tmp.FillSpans = sna_fill_spans__gpu;
			sna_gc_ops__tmp.PolyFillRect = sna_poly_fill_rect__gpu;
			sna_gc_ops__tmp.PolyPoint = sna_poly_point__gpu;
			gc->ops = &sna_gc_ops__tmp;

			switch (gc->lineStyle) {
			default:
				assert(0);
			case LineSolid:
				if (gc->lineWidth == 0) {
					DBG(("%s: miZeroLine\n", __FUNCTION__));
					miZeroLine(drawable, gc, mode, n, pt);
				} else {
					DBG(("%s: miWideLine\n", __FUNCTION__));
					miWideLine(drawable, gc, mode, n, pt);
				}
				break;
			case LineOnOffDash:
			case LineDoubleDash:
				if (gc->lineWidth == 0) {
					DBG(("%s: miZeroDashLine\n", __FUNCTION__));
					miZeroDashLine(drawable, gc, mode, n, pt);
				} else {
					DBG(("%s: miWideDash\n", __FUNCTION__));
					miWideDash(drawable, gc, mode, n, pt);
				}
				break;
			}
		}

		gc->ops = (GCOps *)&sna_gc_ops;
		if (data.damage) {
			if (data.dx | data.dy)
				pixman_region_translate(&data.region, data.dx, data.dy);
			assert_pixmap_contains_box(data.pixmap, &data.region.extents);
			sna_damage_add(data.damage, &data.region);
			assert_pixmap_damage(data.pixmap);
		}
		RegionUninit(&data.region);
		return;
	}

fallback:
	DBG(("%s: fallback\n", __FUNCTION__));
	if (!region_maybe_clip(&data.region, gc->pCompositeClip))
		return;

	if (!sna_gc_move_to_cpu(gc, drawable, &data.region))
		goto out;
	if (!sna_drawable_move_region_to_cpu(drawable, &data.region,
					     drawable_gc_flags(drawable, gc,
							       !(data.flags & 4 && n == 2))))
		goto out;

	if (sigtrap_get() == 0) {
		DBG(("%s: fbPolyLine\n", __FUNCTION__));
		fbPolyLine(drawable, gc, mode, n, pt);
		FALLBACK_FLUSH(drawable);
		sigtrap_put();
	}
out:
	sna_gc_move_to_gpu(gc);
	RegionUninit(&data.region);
}

static inline void box_from_seg(BoxPtr b, xSegment *seg, GCPtr gc)
{
	if (seg->x1 == seg->x2) {
		if (seg->y1 > seg->y2) {
			b->y2 = seg->y1 + 1;
			b->y1 = seg->y2 + 1;
			if (gc->capStyle != CapNotLast)
				b->y1--;
		} else {
			b->y1 = seg->y1;
			b->y2 = seg->y2;
			if (gc->capStyle != CapNotLast)
				b->y2++;
		}
		b->x1 = seg->x1;
		b->x2 = seg->x1 + 1;
	} else {
		if (seg->x1 > seg->x2) {
			b->x2 = seg->x1 + 1;
			b->x1 = seg->x2 + 1;
			if (gc->capStyle != CapNotLast)
				b->x1--;
		} else {
			b->x1 = seg->x1;
			b->x2 = seg->x2;
			if (gc->capStyle != CapNotLast)
				b->x2++;
		}
		b->y1 = seg->y1;
		b->y2 = seg->y1 + 1;
	}

	DBG(("%s: seg=(%d,%d),(%d,%d); box=(%d,%d),(%d,%d)\n",
	     __FUNCTION__,
	     seg->x1, seg->y1, seg->x2, seg->y2,
	     b->x1, b->y1, b->x2, b->y2));
}

static bool
sna_poly_segment_blt(DrawablePtr drawable,
		     struct kgem_bo *bo,
		     struct sna_damage **damage,
		     GCPtr gc, uint32_t pixel,
		     int n, xSegment *seg,
		     const BoxRec *extents, unsigned clipped)
{
	PixmapPtr pixmap = get_drawable_pixmap(drawable);
	struct sna *sna = to_sna_from_pixmap(pixmap);
	BoxRec boxes[512], *b = boxes, * const last_box = boxes + ARRAY_SIZE(boxes);
	struct sna_fill_op fill;
	int16_t dx, dy;

	DBG(("%s: n=%d, alu=%d, fg=%08lx, clipped=%d\n",
	     __FUNCTION__, n, gc->alu, gc->fgPixel, clipped));

	if (!sna_fill_init_blt(&fill, sna, pixmap, bo, gc->alu, pixel, FILL_SPANS))
		return false;

	get_drawable_deltas(drawable, pixmap, &dx, &dy);

	if (!clipped) {
		dx += drawable->x;
		dy += drawable->y;
		if (dx|dy) {
			do {
				unsigned nbox = n;
				if (nbox > ARRAY_SIZE(boxes))
					nbox = ARRAY_SIZE(boxes);
				n -= nbox;
				do {
					box_from_seg(b, seg++, gc);
					if (b->y2 > b->y1 && b->x2 > b->x1) {
						b->x1 += dx;
						b->x2 += dx;
						b->y1 += dy;
						b->y2 += dy;
						b++;
					}
				} while (--nbox);

				if (b != boxes) {
					fill.boxes(sna, &fill, boxes, b-boxes);
					if (damage)
						sna_damage_add_boxes(damage, boxes, b-boxes, 0, 0);
					b = boxes;
				}
			} while (n);
		} else {
			do {
				unsigned nbox = n;
				if (nbox > ARRAY_SIZE(boxes))
					nbox = ARRAY_SIZE(boxes);
				n -= nbox;
				do {
					box_from_seg(b++, seg++, gc);
				} while (--nbox);

				if (b != boxes) {
					fill.boxes(sna, &fill, boxes, b-boxes);
					if (damage)
						sna_damage_add_boxes(damage, boxes, b-boxes, 0, 0);
					b = boxes;
				}
			} while (n);
		}
	} else {
		RegionRec clip;

		region_set(&clip, extents);
		if (!region_maybe_clip(&clip, gc->pCompositeClip))
			goto done;

		if (clip.data) {
			const BoxRec * const clip_start = RegionBoxptr(&clip);
			const BoxRec * const clip_end = clip_start + clip.data->numRects;
			const BoxRec *c;
			do {
				BoxRec box;

				box_from_seg(&box, seg++, gc);
				box.x1 += drawable->x;
				box.x2 += drawable->x;
				box.y1 += drawable->y;
				box.y2 += drawable->y;
				c = find_clip_box_for_y(clip_start,
							clip_end,
							box.y1);
				while (c != clip_end) {
					if (box.y2 <= c->y1)
						break;

					*b = box;
					if (box_intersect(b, c++)) {
						b->x1 += dx;
						b->x2 += dx;
						b->y1 += dy;
						b->y2 += dy;
						if (++b == last_box) {
							fill.boxes(sna, &fill, boxes, last_box-boxes);
							if (damage)
								sna_damage_add_boxes(damage, boxes, last_box-boxes, 0, 0);
							b = boxes;
						}
					}
				}
			} while (--n);
		} else {
			do {
				box_from_seg(b, seg++, gc);
				b->x1 += drawable->x;
				b->x2 += drawable->x;
				b->y1 += drawable->y;
				b->y2 += drawable->y;
				if (box_intersect(b, &clip.extents)) {
					b->x1 += dx;
					b->x2 += dx;
					b->y1 += dy;
					b->y2 += dy;
					if (++b == last_box) {
						fill.boxes(sna, &fill, boxes, last_box-boxes);
						if (damage)
							sna_damage_add_boxes(damage, boxes, last_box-boxes, 0, 0);
						b = boxes;
					}
				}
			} while (--n);
		}
		RegionUninit(&clip);
	}
	if (b != boxes) {
		fill.boxes(sna, &fill, boxes, b - boxes);
		if (damage)
			sna_damage_add_boxes(damage, boxes, b - boxes, 0, 0);
	}
done:
	fill.done(sna, &fill);
	assert_pixmap_damage(pixmap);
	return true;
}

static bool
sna_poly_zero_segment_blt(DrawablePtr drawable,
			  struct kgem_bo *bo,
			  struct sna_damage **damage,
			  GCPtr gc, const int _n, const xSegment *_s,
			  const BoxRec *extents, unsigned clipped)
{
	static void * const _jump[] = {
		&&no_damage,
		&&damage,

		&&no_damage_offset,
		&&damage_offset,
	};

	PixmapPtr pixmap = get_drawable_pixmap(drawable);
	struct sna *sna = to_sna_from_pixmap(pixmap);
	unsigned int bias = miGetZeroLineBias(drawable->pScreen);
	struct sna_fill_op fill;
	RegionRec clip;
	const BoxRec *last_extents;
	BoxRec box[512], *b;
	BoxRec *const last_box = box + ARRAY_SIZE(box);
	int16_t dx, dy;
	void *jump, *ret;

	DBG(("%s: alu=%d, pixel=%lx, n=%d, clipped=%d, damage=%p\n",
	     __FUNCTION__, gc->alu, gc->fgPixel, _n, clipped, damage));
	if (!sna_fill_init_blt(&fill, sna, pixmap, bo, gc->alu, gc->fgPixel, FILL_BOXES))
		return false;

	get_drawable_deltas(drawable, pixmap, &dx, &dy);

	region_set(&clip, extents);
	if (clipped) {
		if (!region_maybe_clip(&clip, gc->pCompositeClip))
			return true;
	}
	DBG(("%s: [clipped] extents=(%d, %d), (%d, %d), delta=(%d, %d)\n",
	     __FUNCTION__,
	     clip.extents.x1, clip.extents.y1,
	     clip.extents.x2, clip.extents.y2,
	     dx, dy));

	jump = _jump[(damage != NULL) | !!(dx|dy) << 1];

	b = box;
	extents = region_rects(&clip);
	last_extents = extents + region_num_rects(&clip);
	do {
		int n = _n;
		const xSegment *s = _s;
		do {
			int16_t sdx, sdy;
			int adx, ady, length;
			int e, e1, e2, e3;
			int x1, x2;
			int y1, y2;
			int oc1, oc2;
			int octant;

			x1 = s->x1 + drawable->x;
			y1 = s->y1 + drawable->y;
			x2 = s->x2 + drawable->x;
			y2 = s->y2 + drawable->y;
			s++;

			DBG(("%s: segment (%d, %d) to (%d, %d)\n",
			     __FUNCTION__, x1, y1, x2, y2));
			if (x2 == x1 && y2 == y1)
				continue;

			oc1 = 0;
			OUTCODES(oc1, x1, y1, extents);
			oc2 = 0;
			OUTCODES(oc2, x2, y2, extents);
			if (oc1 & oc2)
				continue;

			CalcLineDeltas(x1, y1, x2, y2,
				       adx, ady, sdx, sdy,
				       1, 1, octant);

			DBG(("%s: adx=(%d, %d), sdx=(%d, %d)\n",
			     __FUNCTION__, adx, ady, sdx, sdy));
			if (adx == 0 || ady == 0) {
				if (x1 <= x2) {
					b->x1 = x1;
					b->x2 = x2;
				} else {
					b->x1 = x2;
					b->x2 = x1;
				}
				if (y1 <= y2) {
					b->y1 = y1;
					b->y2 = y2;
				} else {
					b->y1 = y2;
					b->y2 = y1;
				}
				b->x2++;
				b->y2++;
				if (oc1 | oc2)
					box_intersect(b, extents);
				if (++b == last_box) {
					ret = &&rectangle_continue;
					goto *jump;
rectangle_continue:
					b = box;
				}
			} else if (adx >= ady) {
				bool dirty;

				/* X-major segment */
				e1 = ady << 1;
				e2 = e1 - (adx << 1);
				e  = e1 - adx;
				length = adx;	/* don't draw endpoint in main loop */

				FIXUP_ERROR(e, octant, bias);

				if (oc1 | oc2) {
					int pt1_clipped, pt2_clipped;
					int x = x1, y = y1;

					if (miZeroClipLine(extents->x1, extents->y1,
							   extents->x2-1, extents->y2-1,
							   &x1, &y1, &x2, &y2,
							   adx, ady,
							   &pt1_clipped, &pt2_clipped,
							   octant, bias, oc1, oc2) == -1)
						continue;

					length = abs(x2 - x1);
					if (length == 0)
						continue;

					if (pt1_clipped) {
						int clipdx = abs(x1 - x);
						int clipdy = abs(y1 - y);
						e += clipdy * e2 + (clipdx - clipdy) * e1;
					}
				}
				e3 = e2 - e1;
				e  = e - e1;

				b->x1 = x1;
				b->y1 = y1;
				dirty = false;
				while (length--) {
					dirty = true;
					e += e1;
					if (e >= 0) {
						e += e3;

						if (sdx < 0) {
							b->x2 = b->x1 + 1;
							b->x1 = x1;
						} else
							b->x2 = x1 + 1;
						b->y2 = b->y1 + 1;

						DBG(("%s: horizontal step: (%d, %d), box: (%d, %d), (%d, %d)\n",
						     __FUNCTION__, x1, y1,
						     b->x1, b->y1, b->x2, b->y2));

						if (++b == last_box) {
							ret = &&X_continue;
							goto *jump;
X_continue:
							b = box;
						}

						b->x1 = x1 + sdx;
						b->y1 = y1 += sdy;
						dirty = false;
					}
					x1 += sdx;
				}
				if (dirty) {
					x1 -= sdx;
					DBG(("%s: horizontal tail: (%d, %d)\n",
					     __FUNCTION__, x1, y1));
					if (sdx < 0) {
						b->x2 = b->x1 + 1;
						b->x1 = x1;
					} else
						b->x2 = x1 + 1;
					b->y2 = b->y1 + 1;

					if (++b == last_box) {
						ret = &&X2_continue;
						goto *jump;
X2_continue:
						b = box;
					}
				}
			} else {
				bool dirty;

				/* Y-major segment */
				e1 = adx << 1;
				e2 = e1 - (ady << 1);
				e  = e1 - ady;
				length  = ady;	/* don't draw endpoint in main loop */

				SetYMajorOctant(octant);
				FIXUP_ERROR(e, octant, bias);

				if (oc1 | oc2) {
					int pt1_clipped, pt2_clipped;
					int x = x1, y = y1;

					if (miZeroClipLine(extents->x1, extents->y1,
							   extents->x2-1, extents->y2-1,
							   &x1, &y1, &x2, &y2,
							   adx, ady,
							   &pt1_clipped, &pt2_clipped,
							   octant, bias, oc1, oc2) == -1)
						continue;

					length = abs(y2 - y1);
					if (length == 0)
						continue;

					if (pt1_clipped) {
						int clipdx = abs(x1 - x);
						int clipdy = abs(y1 - y);
						e += clipdx * e2 + (clipdy - clipdx) * e1;
					}
				}

				e3 = e2 - e1;
				e  = e - e1;

				b->x1 = x1;
				b->y1 = y1;
				dirty = false;
				while (length--) {
					e += e1;
					dirty = true;
					if (e >= 0) {
						e += e3;

						if (sdy < 0) {
							b->y2 = b->y1 + 1;
							b->y1 = y1;
						} else
							b->y2 = y1 + 1;
						b->x2 = x1 + 1;

						if (++b == last_box) {
							ret = &&Y_continue;
							goto *jump;
Y_continue:
							b = box;
						}

						b->x1 = x1 += sdx;
						b->y1 = y1 + sdy;
						dirty = false;
					}
					y1 += sdy;
				}

				if (dirty) {
					y1 -= sdy;
					if (sdy < 0) {
						b->y2 = b->y1 + 1;
						b->y1 = y1;
					} else
						b->y2 = y1 + 1;
					b->x2 = x1 + 1;

					if (++b == last_box) {
						ret = &&Y2_continue;
						goto *jump;
Y2_continue:
						b = box;
					}
				}
			}
		} while (--n);
	} while (++extents != last_extents);

	if (b != box) {
		ret = &&done;
		goto *jump;
	}

done:
	fill.done(sna, &fill);
	assert_pixmap_damage(pixmap);
	RegionUninit(&clip);
	return true;

damage:
	sna_damage_add_boxes(damage, box, b-box, 0, 0);
no_damage:
	fill.boxes(sna, &fill, box, b-box);
	goto *ret;

no_damage_offset:
	{
		BoxRec *bb = box;
		do {
			bb->x1 += dx;
			bb->x2 += dx;
			bb->y1 += dy;
			bb->y2 += dy;
		} while (++bb != b);
		fill.boxes(sna, &fill, box, b - box);
	}
	goto *ret;

damage_offset:
	{
		BoxRec *bb = box;
		do {
			bb->x1 += dx;
			bb->x2 += dx;
			bb->y1 += dy;
			bb->y2 += dy;
		} while (++bb != b);
		fill.boxes(sna, &fill, box, b - box);
		sna_damage_add_boxes(damage, box, b - box, 0, 0);
	}
	goto *ret;
}

static unsigned
sna_poly_segment_extents(DrawablePtr drawable, GCPtr gc,
			 int n, xSegment *seg,
			 BoxPtr out)
{
	BoxRec box;
	bool clipped, can_blit;

	if (n == 0)
		return 0;

	if (seg->x2 >= seg->x1) {
		box.x1 = seg->x1;
		box.x2 = seg->x2;
	} else {
		box.x2 = seg->x1;
		box.x1 = seg->x2;
	}

	if (seg->y2 >= seg->y1) {
		box.y1 = seg->y1;
		box.y2 = seg->y2;
	} else {
		box.y2 = seg->y1;
		box.y1 = seg->y2;
	}

	can_blit = seg->x1 == seg->x2 || seg->y1 == seg->y2;
	while (--n) {
		seg++;
		if (seg->x2 > seg->x1) {
			if (seg->x1 < box.x1) box.x1 = seg->x1;
			if (seg->x2 > box.x2) box.x2 = seg->x2;
		} else {
			if (seg->x2 < box.x1) box.x1 = seg->x2;
			if (seg->x1 > box.x2) box.x2 = seg->x1;
		}

		if (seg->y2 > seg->y1) {
			if (seg->y1 < box.y1) box.y1 = seg->y1;
			if (seg->y2 > box.y2) box.y2 = seg->y2;
		} else {
			if (seg->y2 < box.y1) box.y1 = seg->y2;
			if (seg->y1 > box.y2) box.y2 = seg->y1;
		}

		if (can_blit && !(seg->x1 == seg->x2 || seg->y1 == seg->y2))
			can_blit = false;
	}

	box.x2++;
	box.y2++;

	if (gc->lineWidth) {
		int extra = gc->lineWidth;
		if (gc->capStyle != CapProjecting)
			extra >>= 1;
		if (extra) {
			box.x1 -= extra;
			box.x2 += extra;
			box.y1 -= extra;
			box.y2 += extra;
		}
	}

	DBG(("%s: unclipped, untranslated extents (%d, %d), (%d, %d)\n",
	     __FUNCTION__, box.x1, box.y1, box.x2, box.y2));

	clipped = trim_and_translate_box(&box, drawable, gc);
	if (box_empty(&box))
		return 0;

	*out = box;
	return 1 | clipped << 1 | can_blit << 2;
}

static void
sna_poly_segment(DrawablePtr drawable, GCPtr gc, int n, xSegment *seg)
{
	struct sna_pixmap *priv;
	struct sna_fill_spans data;
	uint32_t color;

	DBG(("%s(n=%d, first=((%d, %d), (%d, %d)), lineWidth=%d\n",
	     __FUNCTION__,
	     n, seg->x1, seg->y1, seg->x2, seg->y2,
	     gc->lineWidth));

	data.flags = sna_poly_segment_extents(drawable, gc, n, seg,
					      &data.region.extents);
	if (data.flags == 0)
		return;

	DBG(("%s: extents=(%d, %d), (%d, %d)\n", __FUNCTION__,
	     data.region.extents.x1, data.region.extents.y1,
	     data.region.extents.x2, data.region.extents.y2));

	data.region.data = NULL;

	if (FORCE_FALLBACK)
		goto fallback;

	if (!ACCEL_POLY_SEGMENT)
		goto fallback;

	data.pixmap = get_drawable_pixmap(drawable);
	data.sna = to_sna_from_pixmap(data.pixmap);
	priv = sna_pixmap(data.pixmap);
	if (priv == NULL) {
		DBG(("%s: fallback -- unattached\n", __FUNCTION__));
		goto fallback;
	}

	if (wedged(data.sna)) {
		DBG(("%s: fallback -- wedged\n", __FUNCTION__));
		goto fallback;
	}

	DBG(("%s: fill=%d [%d], line=%d [%d], width=%d, mask=%lu [%d], rectlinear=%d\n",
	     __FUNCTION__,
	     gc->fillStyle, gc->fillStyle == FillSolid,
	     gc->lineStyle, gc->lineStyle == LineSolid,
	     gc->lineWidth,
	     gc->planemask, PM_IS_SOLID(drawable, gc->planemask),
	     data.flags & 4));
	if (!PM_IS_SOLID(drawable, gc->planemask))
		goto fallback;

	if (gc->lineStyle != LineSolid || gc->lineWidth > 1)
		goto spans_fallback;
	if (gc_is_solid(gc, &color)) {
		DBG(("%s: trying blt solid fill [%08x, flags=%x] paths\n",
		     __FUNCTION__, (unsigned)color, data.flags));

		if (data.flags & 4) {
			if ((data.bo = sna_drawable_use_bo(drawable, PREFER_GPU,
							   &data.region.extents,
							   &data.damage)) &&
			     sna_poly_segment_blt(drawable,
						 data.bo, data.damage,
						 gc, color, n, seg,
						 &data.region.extents,
						 data.flags & 2))
				return;
		} else {
			if ((data.bo = sna_drawable_use_bo(drawable,
							   use_zero_spans(drawable, gc, &data.region.extents),
							   &data.region.extents,
							   &data.damage)) &&
			    sna_poly_zero_segment_blt(drawable,
						      data.bo, data.damage,
						      gc, n, seg,
						      &data.region.extents,
						      data.flags & 2))
				return;
		}
	} else if (data.flags & 4) {
		/* Try converting these to a set of rectangles instead */
		xRectangle *rect;
		int i;

		data.bo = sna_drawable_use_bo(drawable, PREFER_GPU,
					      &data.region.extents,
					      &data.damage);
		if (data.bo == NULL)
			goto fallback;

		DBG(("%s: converting to rectagnles\n", __FUNCTION__));

		rect = malloc (n * sizeof (xRectangle));
		if (rect == NULL)
			return;

		for (i = 0; i < n; i++) {
			if (seg[i].x1 < seg[i].x2) {
				rect[i].x = seg[i].x1;
				rect[i].width = seg[i].x2 - seg[i].x1 + 1;
			} else if (seg[i].x1 > seg[i].x2) {
				rect[i].x = seg[i].x2;
				rect[i].width = seg[i].x1 - seg[i].x2 + 1;
			} else {
				rect[i].x = seg[i].x1;
				rect[i].width = 1;
			}
			if (seg[i].y1 < seg[i].y2) {
				rect[i].y = seg[i].y1;
				rect[i].height = seg[i].y2 - seg[i].y1 + 1;
			} else if (seg[i].y1 > seg[i].y2) {
				rect[i].y = seg[i].y2;
				rect[i].height = seg[i].y1 - seg[i].y2 + 1;
			} else {
				rect[i].y = seg[i].y1;
				rect[i].height = 1;
			}

			/* don't paint last pixel */
			if (gc->capStyle == CapNotLast) {
				if (seg[i].x1 == seg[i].x2)
					rect[i].height--;
				else
					rect[i].width--;
			}
		}

		if (gc->fillStyle == FillTiled) {
			i = sna_poly_fill_rect_tiled_blt(drawable,
							 data.bo, data.damage,
							 gc, n, rect,
							 &data.region.extents,
							 data.flags & 2);
		} else {
			i = sna_poly_fill_rect_stippled_blt(drawable,
							    data.bo, data.damage,
							    gc, n, rect,
							    &data.region.extents,
							    data.flags & 2);
		}
		free (rect);

		if (i)
			return;
	}

spans_fallback:
	if ((data.bo = sna_drawable_use_bo(drawable,
					   use_wide_spans(drawable, gc, &data.region.extents),
					   &data.region.extents,
					   &data.damage))) {
		void (*line)(DrawablePtr, GCPtr, int, int, DDXPointPtr);
		int i;

		DBG(("%s: converting segments into spans\n", __FUNCTION__));

		switch (gc->lineStyle) {
		default:
		case LineSolid:
			if (gc->lineWidth == 0)
				line = miZeroLine;
			else
				line = miWideLine;
			break;
		case LineOnOffDash:
		case LineDoubleDash:
			if (gc->lineWidth == 0)
				line = miZeroDashLine;
			else
				line = miWideDash;
			break;
		}

		get_drawable_deltas(drawable, data.pixmap, &data.dx, &data.dy);
		sna_gc(gc)->priv = &data;

		if (gc->lineWidth == 0 &&
		    gc->lineStyle == LineSolid &&
		    gc_is_solid(gc, &color)) {
			struct sna_fill_op fill;

			if (!sna_fill_init_blt(&fill,
					       data.sna, data.pixmap,
					       data.bo, gc->alu, color,
					       FILL_POINTS | FILL_SPANS))
				goto fallback;

			data.op = &fill;

			if ((data.flags & 2) == 0) {
				if (data.dx | data.dy)
					sna_gc_ops__tmp.FillSpans = sna_fill_spans__fill_offset;
				else
					sna_gc_ops__tmp.FillSpans = sna_fill_spans__fill;
				sna_gc_ops__tmp.PolyPoint = sna_poly_point__fill;
			} else {
				if (!region_maybe_clip(&data.region,
						       gc->pCompositeClip))
					return;

				if (region_is_singular(&data.region)) {
					sna_gc_ops__tmp.FillSpans = sna_fill_spans__fill_clip_extents;
					sna_gc_ops__tmp.PolyPoint = sna_poly_point__fill_clip_extents;
				} else {
					sna_gc_ops__tmp.FillSpans = sna_fill_spans__fill_clip_boxes;
					sna_gc_ops__tmp.PolyPoint = sna_poly_point__fill_clip_boxes;
				}
			}
			assert(gc->miTranslate);
			gc->ops = &sna_gc_ops__tmp;
			for (i = 0; i < n; i++)
				line(drawable, gc, CoordModeOrigin, 2,
				     (DDXPointPtr)&seg[i]);

			fill.done(data.sna, &fill);
		} else {
			sna_gc_ops__tmp.FillSpans = sna_fill_spans__gpu;
			sna_gc_ops__tmp.PolyFillRect = sna_poly_fill_rect__gpu;
			sna_gc_ops__tmp.PolyPoint = sna_poly_point__gpu;
			gc->ops = &sna_gc_ops__tmp;

			for (i = 0; i < n; i++)
				line(drawable, gc, CoordModeOrigin, 2,
				     (DDXPointPtr)&seg[i]);
		}

		gc->ops = (GCOps *)&sna_gc_ops;
		if (data.damage) {
			if (data.dx | data.dy)
				pixman_region_translate(&data.region, data.dx, data.dy);
			assert_pixmap_contains_box(data.pixmap, &data.region.extents);
			sna_damage_add(data.damage, &data.region);
		}
		assert_pixmap_damage(data.pixmap);
		RegionUninit(&data.region);
		return;
	}

fallback:
	DBG(("%s: fallback\n", __FUNCTION__));
	if (!region_maybe_clip(&data.region, gc->pCompositeClip))
		return;

	if (!sna_gc_move_to_cpu(gc, drawable, &data.region))
		goto out;
	if (!sna_drawable_move_region_to_cpu(drawable, &data.region,
					     drawable_gc_flags(drawable, gc,
							       !(data.flags & 4 && n == 1))))
		goto out;

	if (sigtrap_get() == 0) {
		DBG(("%s: fbPolySegment\n", __FUNCTION__));
		fbPolySegment(drawable, gc, n, seg);
		FALLBACK_FLUSH(drawable);
		sigtrap_put();
	}
out:
	sna_gc_move_to_gpu(gc);
	RegionUninit(&data.region);
}

static unsigned
sna_poly_rectangle_extents(DrawablePtr drawable, GCPtr gc,
			   int n, xRectangle *r,
			   BoxPtr out)
{
	Box32Rec box;
	int extra = gc->lineWidth >> 1;
	bool clipped;
	bool zero = false;

	if (n == 0)
		return 0;

	box.x1 = r->x;
	box.y1 = r->y;
	box.x2 = box.x1 + r->width;
	box.y2 = box.y1 + r->height;
	zero |= (r->width | r->height) == 0;

	while (--n) {
		r++;
		zero |= (r->width | r->height) == 0;
		box32_add_rect(&box, r);
	}

	box.x2++;
	box.y2++;

	if (extra) {
		box.x1 -= extra;
		box.x2 += extra;
		box.y1 -= extra;
		box.y2 += extra;
		zero = !zero;
	} else
		zero = true;

	DBG(("%s: unclipped original extents: (%d, %d), (%d, %d)\n",
	     __FUNCTION__, box.x1, box.y1, box.x2, box.y2));
	clipped = box32_trim_and_translate(&box, drawable, gc);
	if (!box32_to_box16(&box, out))
		return 0;

	DBG(("%s: extents: (%d, %d), (%d, %d), clipped? %d\n",
	     __FUNCTION__, out->x1, out->y1, out->x2, out->y2, clipped));
	return 1 | clipped << 1 | zero << 2;
}

static bool
sna_poly_rectangle_blt(DrawablePtr drawable,
		       struct kgem_bo *bo,
		       struct sna_damage **damage,
		       GCPtr gc, int n, xRectangle *r,
		       const BoxRec *extents, unsigned clipped)
{
	PixmapPtr pixmap = get_drawable_pixmap(drawable);
	struct sna *sna = to_sna_from_pixmap(pixmap);
	struct sna_fill_op fill;
	BoxRec boxes[512], *b = boxes, *const last_box = boxes+ARRAY_SIZE(boxes);
	int16_t dx, dy;
	static void * const jump[] = {
		&&wide,
		&&zero,
		&&wide_clipped,
		&&zero_clipped,
	};

	DBG(("%s: n=%d, alu=%d, width=%d, fg=%08lx, damge=%p, clipped?=%d\n",
	     __FUNCTION__, n, gc->alu, gc->lineWidth, gc->fgPixel, damage, clipped));
	if (!sna_fill_init_blt(&fill, sna, pixmap, bo, gc->alu, gc->fgPixel, FILL_BOXES))
		return false;

	get_drawable_deltas(drawable, pixmap, &dx, &dy);

	goto *jump[(gc->lineWidth <= 1) | clipped];

zero:
	dx += drawable->x;
	dy += drawable->y;

	do {
		xRectangle rr = *r++;

		if ((rr.width | rr.height) == 0)
			continue; /* XXX -> PolyLine */

		DBG(("%s - zero : r[%d] = (%d, %d) x (%d, %d)\n", __FUNCTION__,
		     n, rr.x, rr.y, rr.width, rr.height));
		rr.x += dx;
		rr.y += dy;

		if (b+4 > last_box) {
			fill.boxes(sna, &fill, boxes, b-boxes);
			if (damage)
				sna_damage_add_boxes(damage, boxes, b-boxes, 0, 0);
			b = boxes;
		}

		if (rr.width <= 1 || rr.height <= 1) {
			b->x1 = rr.x;
			b->y1 = rr.y;
			b->x2 = rr.x + rr.width + (rr.height != 0);
			b->y2 = rr.y + rr.height + (rr.width != 0);
			DBG(("%s: blt (%d, %d), (%d, %d)\n",
			     __FUNCTION__,
			     b->x1, b->y1, b->x2,b->y2));
			b++;
		} else {
			b[0].x1 = rr.x;
			b[0].y1 = rr.y;
			b[0].x2 = rr.x + rr.width + 1;
			b[0].y2 = rr.y + 1;

			b[1] = b[0];
			b[1].y1 += rr.height;
			b[1].y2 += rr.height;

			b[2].y1 = rr.y + 1;
			b[2].y2 = rr.y + rr.height;
			b[2].x1 = rr.x;
			b[2].x2 = rr.x + 1;

			b[3] = b[2];
			b[3].x1 += rr.width;
			b[3].x2 += rr.width;

			b += 4;
		}
	} while (--n);
	goto done;

zero_clipped:
	{
		RegionRec clip;
		BoxRec box[4];
		int count;

		region_set(&clip, extents);
		if (!region_maybe_clip(&clip, gc->pCompositeClip))
			goto done;

		if (clip.data) {
			const BoxRec * const clip_start = RegionBoxptr(&clip);
			const BoxRec * const clip_end = clip_start + clip.data->numRects;
			const BoxRec *c;
			do {
				xRectangle rr = *r++;

				DBG(("%s - zero, clipped complex: r[%d] = (%d, %d) x (%d, %d)\n", __FUNCTION__,
				     n, rr.x, rr.y, rr.width, rr.height));

				if ((rr.width | rr.height) == 0)
					continue; /* XXX -> PolyLine */

				rr.x += drawable->x;
				rr.y += drawable->y;

				if (rr.width <= 1 || rr.height <= 1) {
					box[0].x1 = rr.x;
					box[0].y1 = rr.y;
					box[0].x2 = rr.x + rr.width + (rr.height != 0);
					box[0].y2 = rr.y + rr.height + (rr.width != 0);
					count = 1;
				} else {
					box[0].x1 = rr.x;
					box[0].y1 = rr.y;
					box[0].x2 = rr.x + rr.width + 1;
					box[0].y2 = rr.y + 1;

					box[1] = box[0];
					box[1].y1 += rr.height;
					box[1].y2 += rr.height;

					box[2].y1 = rr.y + 1;
					box[2].y2 = rr.y + rr.height;
					box[2].x1 = rr.x;
					box[2].x2 = rr.x + 1;

					box[3] = box[2];
					box[3].x1 += rr.width;
					box[3].x2 += rr.width;
					count = 4;
				}

				while (count--) {
					c = find_clip_box_for_y(clip_start,
								clip_end,
								box[count].y1);
					while (c != clip_end) {
						if (box[count].y2 <= c->y1)
							break;

						*b = box[count];
						if (box_intersect(b, c++)) {
							b->x1 += dx;
							b->x2 += dx;
							b->y1 += dy;
							b->y2 += dy;
							if (++b == last_box) {
								fill.boxes(sna, &fill, boxes, last_box-boxes);
								if (damage)
									sna_damage_add_boxes(damage, boxes, last_box-boxes, 0, 0);
								b = boxes;
							}
						}

					}
				}
			} while (--n);
		} else {
			do {
				xRectangle rr = *r++;
				DBG(("%s - zero, clip: r[%d] = (%d, %d) x (%d, %d)\n", __FUNCTION__,
				     n, rr.x, rr.y, rr.width, rr.height));

				if ((rr.width | rr.height) == 0)
					continue; /* XXX -> PolyLine */

				rr.x += drawable->x;
				rr.y += drawable->y;

				if (rr.width <= 1 || rr.height <= 1) {
					box[0].x1 = rr.x;
					box[0].y1 = rr.y;
					box[0].x2 = rr.x + rr.width + (rr.height != 0);
					box[0].y2 = rr.y + rr.height + (rr.width != 0);
					count = 1;
				} else {
					box[0].x1 = rr.x;
					box[0].y1 = rr.y;
					box[0].x2 = rr.x + rr.width + 1;
					box[0].y2 = rr.y + 1;

					box[1] = box[0];
					box[1].y1 += rr.height;
					box[1].y2 += rr.height;

					box[2].y1 = rr.y + 1;
					box[2].y2 = rr.y + rr.height;
					box[2].x1 = rr.x;
					box[2].x2 = rr.x + 1;

					box[3] = box[2];
					box[3].x1 += rr.width;
					box[3].x2 += rr.width;
					count = 4;
				}

				while (count--) {
					*b = box[count];
					if (box_intersect(b, &clip.extents)) {
						b->x1 += dx;
						b->x2 += dx;
						b->y1 += dy;
						b->y2 += dy;
						if (++b == last_box) {
							fill.boxes(sna, &fill, boxes, last_box-boxes);
							if (damage)
								sna_damage_add_boxes(damage, boxes, last_box-boxes, 0, 0);
							b = boxes;
						}
					}

				}
			} while (--n);
		}
		RegionUninit(&clip);
	}
	goto done;

wide_clipped:
	{
		RegionRec clip;
		BoxRec box[4];
		int16_t offset2 = gc->lineWidth;
		int16_t offset1 = offset2 >> 1;
		int16_t offset3 = offset2 - offset1;

		region_set(&clip, extents);
		if (!region_maybe_clip(&clip, gc->pCompositeClip))
			goto done;

		DBG(("%s: wide clipped: extents=((%d, %d), (%d, %d))\n",
		     __FUNCTION__,
		     clip.extents.x1, clip.extents.y1,
		     clip.extents.x2, clip.extents.y2));

		if (clip.data) {
			const BoxRec * const clip_start = RegionBoxptr(&clip);
			const BoxRec * const clip_end = clip_start + clip.data->numRects;
			const BoxRec *c;
			do {
				xRectangle rr = *r++;
				int count;

				if ((rr.width | rr.height) == 0)
					continue; /* XXX -> PolyLine */

				rr.x += drawable->x;
				rr.y += drawable->y;

				if (rr.height <= offset2 || rr.width <= offset2) {
					if (rr.height == 0) {
						box[0].x1 = rr.x;
						box[0].x2 = rr.x + rr.width;
					} else {
						box[0].x1 = rr.x - offset1;
						box[0].x2 = rr.x + rr.width + offset3;
					}
					if (rr.width == 0) {
						box[0].y1 = rr.y;
						box[0].y2 = rr.y + rr.height;
					} else {
						box[0].y1 = rr.y - offset1;
						box[0].y2 = rr.y + rr.height + offset3;
					}
					count = 1;
				} else {
					box[0].x1 = rr.x - offset1;
					box[0].x2 = box[0].x1 + rr.width + offset2;
					box[0].y1 = rr.y - offset1;
					box[0].y2 = box[0].y1 + offset2;

					box[1].x1 = rr.x - offset1;
					box[1].x2 = box[1].x1 + offset2;
					box[1].y1 = rr.y + offset3;
					box[1].y2 = rr.y + rr.height - offset1;

					box[2] = box[1];
					box[2].x1 += rr.width;
					box[2].x2 += rr.width;

					box[3] = box[0];
					box[3].y1 += rr.height;
					box[3].y2 += rr.height;
					count = 4;
				}

				while (count--) {
					c = find_clip_box_for_y(clip_start,
								clip_end,
								box[count].y1);
					while (c != clip_end) {
						if (box[count].y2 <= c->y1)
							break;

						*b = box[count];
						if (box_intersect(b, c++)) {
							b->x1 += dx;
							b->x2 += dx;
							b->y1 += dy;
							b->y2 += dy;
							if (++b == last_box) {
								fill.boxes(sna, &fill, boxes, last_box-boxes);
								if (damage)
									sna_damage_add_boxes(damage, boxes, last_box-boxes, 0, 0);
								b = boxes;
							}
						}
					}
				}
			} while (--n);
		} else {
			DBG(("%s: singular clip offset1=%d, offset2=%d, offset3=%d\n",
			     __FUNCTION__, offset1, offset2, offset3));
			do {
				xRectangle rr = *r++;
				int count;
				rr.x += drawable->x;
				rr.y += drawable->y;

				DBG(("%s: r=(%d, %d)x(%d, %d)\n",
				     __FUNCTION__, rr.x, rr.y, rr.width, rr.height));
				if (rr.height <= offset2 || rr.width <= offset2) {
					if (rr.height == 0) {
						box[0].x1 = rr.x;
						box[0].x2 = rr.x + rr.width;
					} else {
						box[0].x1 = rr.x - offset1;
						box[0].x2 = box[0].x1 + rr.width + offset2;
					}
					if (rr.width == 0) {
						box[0].y1 = rr.y;
						box[0].y2 = rr.y + rr.height;
					} else {
						box[0].y1 = rr.y - offset1;
						box[0].y2 = box[0].y1 + rr.height + offset2;
					}
					count = 1;
				} else {
					box[0].x1 = rr.x - offset1;
					box[0].x2 = box[0].x1 + rr.width + offset2;
					box[0].y1 = rr.y - offset1;
					box[0].y2 = box[0].y1 + offset2;
					DBG(("%s: box[0]=(%d, %d), (%d, %d)\n",
					     __FUNCTION__,
					     box[0].x1, box[0].y1,
					     box[0].x2, box[0].y2));

					box[1].x1 = rr.x - offset1;
					box[1].x2 = box[1].x1 + offset2;
					box[1].y1 = rr.y + offset3;
					box[1].y2 = rr.y + rr.height - offset1;
					DBG(("%s: box[1]=(%d, %d), (%d, %d)\n",
					     __FUNCTION__,
					     box[1].x1, box[1].y1,
					     box[1].x2, box[1].y2));

					box[2] = box[1];
					box[2].x1 += rr.width;
					box[2].x2 += rr.width;
					DBG(("%s: box[2]=(%d, %d), (%d, %d)\n",
					     __FUNCTION__,
					     box[2].x1, box[2].y1,
					     box[2].x2, box[2].y2));

					box[3] = box[0];
					box[3].y1 += rr.height;
					box[3].y2 += rr.height;
					DBG(("%s: box[3]=(%d, %d), (%d, %d)\n",
					     __FUNCTION__,
					     box[3].x1, box[3].y1,
					     box[3].x2, box[3].y2));

					count = 4;
				}

				while (count--) {
					*b = box[count];
					if (box_intersect(b, &clip.extents)) {
						b->x1 += dx;
						b->x2 += dx;
						b->y1 += dy;
						b->y2 += dy;
						if (++b == last_box) {
							fill.boxes(sna, &fill, boxes, last_box-boxes);
							if (damage)
								sna_damage_add_boxes(damage, boxes, last_box-boxes, 0, 0);
							b = boxes;
						}
					}
				}
			} while (--n);
		}
		RegionUninit(&clip);
	}
	goto done;

wide:
	{
		int offset2 = gc->lineWidth;
		int offset1 = offset2 >> 1;
		int offset3 = offset2 - offset1;

		dx += drawable->x;
		dy += drawable->y;

		do {
			xRectangle rr = *r++;

			if ((rr.width | rr.height) == 0)
				continue; /* XXX -> PolyLine */

			rr.x += dx;
			rr.y += dy;

			if (b+4 > last_box) {
				fill.boxes(sna, &fill, boxes, last_box-boxes);
				if (damage)
					sna_damage_add_boxes(damage, boxes, last_box-boxes, 0, 0);
				b = boxes;
			}

			if (rr.height <= offset2 || rr.width <= offset2) {
				if (rr.height == 0) {
					b->x1 = rr.x;
					b->x2 = rr.x + rr.width;
				} else {
					b->x1 = rr.x - offset1;
					b->x2 = rr.x + rr.width + offset3;
				}
				if (rr.width == 0) {
					b->y1 = rr.y;
					b->y2 = rr.y + rr.height;
				} else {
					b->y1 = rr.y - offset1;
					b->y2 = rr.y + rr.height + offset3;
				}
				b++;
			} else {
				b[0].x1 = rr.x - offset1;
				b[0].x2 = b[0].x1 + rr.width + offset2;
				b[0].y1 = rr.y - offset1;
				b[0].y2 = b[0].y1 + offset2;

				b[1].x1 = rr.x - offset1;
				b[1].x2 = b[1].x1 + offset2;
				b[1].y1 = rr.y + offset3;
				b[1].y2 = rr.y + rr.height - offset1;

				b[2] = b[1];
				b[2].x1 += rr.width;
				b[2].x2 += rr.width;

				b[3] = b[0];
				b[3].y1 += rr.height;
				b[3].y2 += rr.height;
				b += 4;
			}
		} while (--n);
	}
	goto done;

done:
	if (b != boxes) {
		fill.boxes(sna, &fill, boxes, b-boxes);
		if (damage)
			sna_damage_add_boxes(damage, boxes, b-boxes, 0, 0);
	}
	fill.done(sna, &fill);
	assert_pixmap_damage(pixmap);
	return true;
}

static void
sna_poly_rectangle(DrawablePtr drawable, GCPtr gc, int n, xRectangle *r)
{
	PixmapPtr pixmap = get_drawable_pixmap(drawable);
	struct sna *sna = to_sna_from_pixmap(pixmap);
	struct sna_damage **damage;
	struct kgem_bo *bo;
	RegionRec region;
	unsigned flags;

	DBG(("%s(n=%d, first=((%d, %d)x(%d, %d)), lineWidth=%d\n",
	     __FUNCTION__,
	     n, r->x, r->y, r->width, r->height,
	     gc->lineWidth));

	flags = sna_poly_rectangle_extents(drawable, gc, n, r, &region.extents);
	if (flags == 0)
		return;

	DBG(("%s: extents=(%d, %d), (%d, %d), flags=%x\n", __FUNCTION__,
	     region.extents.x1, region.extents.y1,
	     region.extents.x2, region.extents.y2,
	     flags));

	if (FORCE_FALLBACK)
		goto fallback;

	if (!ACCEL_POLY_RECTANGLE)
		goto fallback;

	if (wedged(sna)) {
		DBG(("%s: fallback -- wedged\n", __FUNCTION__));
		goto fallback;
	}

	DBG(("%s: fill=%d [%d], line=%d [%d], join=%d [%d], mask=%lu [%d]\n",
	     __FUNCTION__,
	     gc->fillStyle, gc->fillStyle == FillSolid,
	     gc->lineStyle, gc->lineStyle == LineSolid,
	     gc->joinStyle, gc->joinStyle == JoinMiter,
	     gc->planemask, PM_IS_SOLID(drawable, gc->planemask)));

	if (!PM_IS_SOLID(drawable, gc->planemask))
		goto fallback;

	if (flags & 4 && gc->fillStyle == FillSolid && gc->lineStyle == LineSolid && gc->joinStyle == JoinMiter) {
		DBG(("%s: trying blt solid fill [%08lx] paths\n",
		     __FUNCTION__, gc->fgPixel));
		if ((bo = sna_drawable_use_bo(drawable, PREFER_GPU,
					      &region.extents, &damage)) &&
		    sna_poly_rectangle_blt(drawable, bo, damage,
					   gc, n, r, &region.extents, flags&2))
			return;
	} else {
		/* Not a trivial outline, but we still maybe able to break it
		 * down into simpler operations that we can accelerate.
		 */
		if (sna_drawable_use_bo(drawable, PREFER_GPU,
					&region.extents, &damage)) {
			miPolyRectangle(drawable, gc, n, r);
			return;
		}
	}

fallback:
	DBG(("%s: fallback, clip=%dx[(%d, %d), (%d, %d)]\n", __FUNCTION__,
	     region_num_rects(gc->pCompositeClip),
	     gc->pCompositeClip->extents.x1, gc->pCompositeClip->extents.y1,
	     gc->pCompositeClip->extents.x2, gc->pCompositeClip->extents.y2));

	region.data = NULL;
	if (!region_maybe_clip(&region, gc->pCompositeClip))
		return;

	DBG(("%s: CPU region=%dx[(%d, %d), (%d, %d)]\n", __FUNCTION__,
	     region_num_rects(&region),
	     region.extents.x1, region.extents.y1,
	     region.extents.x2, region.extents.y2));
	if (!sna_gc_move_to_cpu(gc, drawable, &region))
		goto out;
	if (!sna_drawable_move_region_to_cpu(drawable, &region,
					     drawable_gc_flags(drawable, gc, true)))
		goto out;

	if (sigtrap_get() == 0) {
		DBG(("%s: miPolyRectangle\n", __FUNCTION__));
		miPolyRectangle(drawable, gc, n, r);
		FALLBACK_FLUSH(drawable);
		sigtrap_put();
	}
out:
	sna_gc_move_to_gpu(gc);
	RegionUninit(&region);
}

static unsigned
sna_poly_arc_extents(DrawablePtr drawable, GCPtr gc,
		     int n, xArc *arc,
		     BoxPtr out)
{
	BoxRec box;
	bool clipped;
	int v;

	if (n == 0)
		return 0;

	box.x1 = arc->x;
	box.x2 = bound(box.x1, arc->width);
	box.y1 = arc->y;
	box.y2 = bound(box.y1, arc->height);

	while (--n) {
		arc++;
		if (box.x1 > arc->x)
			box.x1 = arc->x;
		v = bound(arc->x, arc->width);
		if (box.x2 < v)
			box.x2 = v;
		if (box.y1 > arc->y)
			box.y1 = arc->y;
		v = bound(arc->y, arc->height);
		if (box.y2 < v)
			box.y2 = v;
	}

	v = gc->lineWidth >> 1;
	if (v) {
		box.x1 -= v;
		box.x2 += v;
		box.y1 -= v;
		box.y2 += v;
	}

	box.x2++;
	box.y2++;

	clipped = trim_and_translate_box(&box, drawable, gc);
	if (box_empty(&box))
		return 0;

	*out = box;
	return 1 | clipped << 1;
}

static void
sna_poly_arc(DrawablePtr drawable, GCPtr gc, int n, xArc *arc)
{
	struct sna_fill_spans data;
	struct sna_pixmap *priv;

	DBG(("%s(n=%d, lineWidth=%d\n", __FUNCTION__, n, gc->lineWidth));

	data.flags = sna_poly_arc_extents(drawable, gc, n, arc,
					  &data.region.extents);
	if (data.flags == 0)
		return;

	DBG(("%s: extents=(%d, %d), (%d, %d), flags=%x\n", __FUNCTION__,
	     data.region.extents.x1, data.region.extents.y1,
	     data.region.extents.x2, data.region.extents.y2,
	     data.flags));

	data.region.data = NULL;

	if (FORCE_FALLBACK)
		goto fallback;

	if (!ACCEL_POLY_ARC)
		goto fallback;

	data.pixmap = get_drawable_pixmap(drawable);
	data.sna = to_sna_from_pixmap(data.pixmap);
	priv = sna_pixmap(data.pixmap);
	if (priv == NULL) {
		DBG(("%s: fallback -- unattached\n", __FUNCTION__));
		goto fallback;
	}

	if (wedged(data.sna)) {
		DBG(("%s: fallback -- wedged\n", __FUNCTION__));
		goto fallback;
	}

	if (!PM_IS_SOLID(drawable, gc->planemask))
		goto fallback;

	if ((data.bo = sna_drawable_use_bo(drawable,
					   use_wide_spans(drawable, gc, &data.region.extents),
					   &data.region.extents, &data.damage))) {
		uint32_t color;

		DBG(("%s: converting arcs into spans\n", __FUNCTION__));
		get_drawable_deltas(drawable, data.pixmap, &data.dx, &data.dy);

		if (gc_is_solid(gc, &color)) {
			sna_gc(gc)->priv = &data;

			assert(gc->miTranslate);
			if (gc->lineStyle == LineSolid) {
				struct sna_fill_op fill;

				if (!sna_fill_init_blt(&fill,
						       data.sna, data.pixmap,
						       data.bo, gc->alu, color,
						       FILL_POINTS | FILL_SPANS))
					goto fallback;

				if ((data.flags & 2) == 0) {
					if (data.dx | data.dy)
						sna_gc_ops__tmp.FillSpans = sna_fill_spans__fill_offset;
					else
						sna_gc_ops__tmp.FillSpans = sna_fill_spans__fill;
					sna_gc_ops__tmp.PolyPoint = sna_poly_point__fill;
				} else {
					if (!region_maybe_clip(&data.region,
							       gc->pCompositeClip))
						return;

					if (region_is_singular(&data.region)) {
						sna_gc_ops__tmp.FillSpans = sna_fill_spans__fill_clip_extents;
						sna_gc_ops__tmp.PolyPoint = sna_poly_point__fill_clip_extents;
					} else {
						sna_gc_ops__tmp.FillSpans = sna_fill_spans__fill_clip_boxes;
						sna_gc_ops__tmp.PolyPoint = sna_poly_point__fill_clip_boxes;
					}
				}

				data.op = &fill;
				gc->ops = &sna_gc_ops__tmp;
				if (gc->lineWidth == 0)
					miZeroPolyArc(drawable, gc, n, arc);
				else
					miPolyArc(drawable, gc, n, arc);
				gc->ops = (GCOps *)&sna_gc_ops;

				fill.done(data.sna, &fill);
			} else {
				if (!region_maybe_clip(&data.region,
						       gc->pCompositeClip))
					return;

				sna_gc_ops__tmp.FillSpans = sna_fill_spans__gpu;
				sna_gc_ops__tmp.PolyPoint = sna_poly_point__gpu;

				gc->ops = &sna_gc_ops__tmp;
				if (gc->lineWidth == 0)
					miZeroPolyArc(drawable, gc, n, arc);
				else
					miPolyArc(drawable, gc, n, arc);
				gc->ops = (GCOps *)&sna_gc_ops;
			}

			if (data.damage) {
				if (data.dx | data.dy)
					pixman_region_translate(&data.region, data.dx, data.dy);
				assert_pixmap_contains_box(data.pixmap, &data.region.extents);
				sna_damage_add(data.damage, &data.region);
			}
			assert_pixmap_damage(data.pixmap);
			RegionUninit(&data.region);
			return;
		}

		/* XXX still around 10x slower for x11perf -ellipse */
		if (gc->lineWidth == 0)
			miZeroPolyArc(drawable, gc, n, arc);
		else
			miPolyArc(drawable, gc, n, arc);
		return;
	}

fallback:
	DBG(("%s -- fallback\n", __FUNCTION__));
	if (!region_maybe_clip(&data.region, gc->pCompositeClip))
		return;

	if (!sna_gc_move_to_cpu(gc, drawable, &data.region))
		goto out;
	if (!sna_drawable_move_region_to_cpu(drawable, &data.region,
					     drawable_gc_flags(drawable, gc, true)))
		goto out;

	if (sigtrap_get() == 0) {
		DBG(("%s -- fbPolyArc\n", __FUNCTION__));
		fbPolyArc(drawable, gc, n, arc);
		FALLBACK_FLUSH(drawable);
		sigtrap_put();
	}
out:
	sna_gc_move_to_gpu(gc);
	RegionUninit(&data.region);
}

static bool
sna_poly_fill_rect_blt(DrawablePtr drawable,
		       struct kgem_bo *bo,
		       struct sna_damage **damage,
		       GCPtr gc, uint32_t pixel,
		       int n, const xRectangle *rect,
		       const BoxRec *extents,
		       bool clipped)
{
	PixmapPtr pixmap = get_drawable_pixmap(drawable);
	struct sna *sna = to_sna_from_pixmap(pixmap);
	struct sna_fill_op fill;
	BoxRec boxes[512], *b = boxes, *const last_box = boxes+ARRAY_SIZE(boxes);
	int16_t dx, dy;

	DBG(("%s pixmap=%ld x %d [(%d, %d)x(%d, %d)...]+(%d,%d), clipped?=%d\n",
	     __FUNCTION__, pixmap->drawable.serialNumber, n,
	     rect->x, rect->y, rect->width, rect->height,
	     drawable->x, drawable->y,
	     clipped));

	if (n == 1 && region_is_singular(gc->pCompositeClip)) {
		BoxRec r;
		bool success = true;

		r.x1 = rect->x + drawable->x;
		r.y1 = rect->y + drawable->y;
		r.x2 = bound(r.x1, rect->width);
		r.y2 = bound(r.y1, rect->height);
		if (box_intersect(&r, &gc->pCompositeClip->extents)) {
			if (get_drawable_deltas(drawable, pixmap, &dx, &dy)) {
				r.x1 += dx; r.y1 += dy;
				r.x2 += dx; r.y2 += dy;
			}
			DBG(("%s: using fill_one() fast path: (%d, %d), (%d, %d). alu=%d, pixel=%08x\n",
			     __FUNCTION__, r.x1, r.y1, r.x2, r.y2, gc->alu, pixel));

			if (sna->render.fill_one(sna, pixmap, bo, pixel,
						 r.x1, r.y1, r.x2, r.y2,
						 gc->alu)) {
				if (damage) {
					assert_pixmap_contains_box(pixmap, &r);
					if (r.x2 - r.x1 == pixmap->drawable.width &&
					    r.y2 - r.y1 == pixmap->drawable.height)
						sna_damage_all(damage, pixmap);
					else
						sna_damage_add_box(damage, &r);
				}
				assert_pixmap_damage(pixmap);

				if (alu_overwrites(gc->alu) &&
				    r.x2 - r.x1 == pixmap->drawable.width &&
				    r.y2 - r.y1 == pixmap->drawable.height) {
					struct sna_pixmap *priv = sna_pixmap(pixmap);
					if (bo == priv->gpu_bo) {
						assert(priv->gpu_bo->proxy == NULL);
						sna_damage_all(&priv->gpu_damage, pixmap);
						sna_damage_destroy(&priv->cpu_damage);
						list_del(&priv->flush_list);
						priv->clear = true;
						priv->clear_color = gc->alu == GXcopyInverted ? ~pixel & ((1 << gc->depth) - 1) : pixel;

						DBG(("%s: pixmap=%ld, marking clear [%08x]\n",
						     __FUNCTION__, pixmap->drawable.serialNumber, priv->clear_color));
					}
				}
			} else
				success = false;
		}

		return success;
	}

	if (!sna_fill_init_blt(&fill, sna, pixmap, bo, gc->alu, pixel, FILL_BOXES)) {
		DBG(("%s: unsupported blt\n", __FUNCTION__));
		return false;
	}

	get_drawable_deltas(drawable, pixmap, &dx, &dy);
	if (!clipped) {
		dx += drawable->x;
		dy += drawable->y;

		sna_damage_add_rectangles(damage, rect, n, dx, dy);
		if (dx|dy) {
			do {
				unsigned nbox = n;
				if (nbox > ARRAY_SIZE(boxes))
					nbox = ARRAY_SIZE(boxes);
				n -= nbox;
				do {
					b->x1 = rect->x + dx;
					b->y1 = rect->y + dy;
					b->x2 = b->x1 + rect->width;
					b->y2 = b->y1 + rect->height;
					b++;
					rect++;
				} while (--nbox);
				fill.boxes(sna, &fill, boxes, b-boxes);
				b = boxes;
			} while (n);
		} else {
			do {
				unsigned nbox = n;
				if (nbox > ARRAY_SIZE(boxes))
					nbox = ARRAY_SIZE(boxes);
				n -= nbox;
				do {
					b->x1 = rect->x;
					b->y1 = rect->y;
					b->x2 = b->x1 + rect->width;
					b->y2 = b->y1 + rect->height;
					b++;
					rect++;
				} while (--nbox);
				fill.boxes(sna, &fill, boxes, b-boxes);
				b = boxes;
			} while (n);
		}
	} else {
		RegionRec clip;

		region_set(&clip, extents);
		if (!region_maybe_clip(&clip, gc->pCompositeClip))
			goto done;

		if (clip.data == NULL) {
			do {
				b->x1 = rect->x + drawable->x;
				b->y1 = rect->y + drawable->y;
				b->x2 = bound(b->x1, rect->width);
				b->y2 = bound(b->y1, rect->height);
				rect++;

				if (box_intersect(b, &clip.extents)) {
					b->x1 += dx;
					b->x2 += dx;
					b->y1 += dy;
					b->y2 += dy;
					if (++b == last_box) {
						fill.boxes(sna, &fill, boxes, last_box-boxes);
						if (damage)
							sna_damage_add_boxes(damage, boxes, last_box-boxes, 0, 0);
						b = boxes;
					}
				}
			} while (--n);
		} else {
			const BoxRec * const clip_start = RegionBoxptr(&clip);
			const BoxRec * const clip_end = clip_start + clip.data->numRects;
			const BoxRec *c;

			do {
				BoxRec box;

				box.x1 = rect->x + drawable->x;
				box.y1 = rect->y + drawable->y;
				box.x2 = bound(box.x1, rect->width);
				box.y2 = bound(box.y1, rect->height);
				rect++;

				c = find_clip_box_for_y(clip_start,
							clip_end,
							box.y1);
				while (c != clip_end) {
					if (box.y2 <= c->y1)
						break;

					*b = box;
					if (box_intersect(b, c++)) {
						b->x1 += dx;
						b->x2 += dx;
						b->y1 += dy;
						b->y2 += dy;
						if (++b == last_box) {
							fill.boxes(sna, &fill, boxes, last_box-boxes);
							if (damage)
								sna_damage_add_boxes(damage, boxes, last_box-boxes, 0, 0);
							b = boxes;
						}
					}

				}
			} while (--n);
		}

		RegionUninit(&clip);
		if (b != boxes) {
			fill.boxes(sna, &fill, boxes, b-boxes);
			if (damage)
				sna_damage_add_boxes(damage, boxes, b-boxes, 0, 0);
		}
	}
done:
	fill.done(sna, &fill);
	assert_pixmap_damage(pixmap);
	return true;
}

static uint32_t
get_pixel(PixmapPtr pixmap)
{
	DBG(("%s(pixmap=%ld)\n", __FUNCTION__, pixmap->drawable.serialNumber));
	if (!sna_pixmap_move_to_cpu(pixmap, MOVE_READ))
		return 0;

	switch (pixmap->drawable.bitsPerPixel) {
	case 32: return *(uint32_t *)pixmap->devPrivate.ptr;
	case 16: return *(uint16_t *)pixmap->devPrivate.ptr;
	default: return *(uint8_t *)pixmap->devPrivate.ptr;
	}
}

static void
sna_poly_fill_polygon(DrawablePtr draw, GCPtr gc,
		      int shape, int mode,
		      int n, DDXPointPtr pt)
{
	struct sna_fill_spans data;
	struct sna_pixmap *priv;

	DBG(("%s(n=%d, PlaneMask: %lx (solid %d), solid fill: %d [style=%d, tileIsPixel=%d], alu=%d)\n", __FUNCTION__,
	     n, gc->planemask, !!PM_IS_SOLID(draw, gc->planemask),
	     (gc->fillStyle == FillSolid ||
	      (gc->fillStyle == FillTiled && gc->tileIsPixel)),
	     gc->fillStyle, gc->tileIsPixel,
	     gc->alu));
	DBG(("%s: draw=%ld, offset=(%d, %d), size=%dx%d\n",
	     __FUNCTION__, draw->serialNumber,
	     draw->x, draw->y, draw->width, draw->height));

	data.flags = sna_poly_point_extents(draw, gc, mode, n, pt,
					    &data.region.extents);
	if (data.flags == 0) {
		DBG(("%s, nothing to do\n", __FUNCTION__));
		return;
	}

	DBG(("%s: extents(%d, %d), (%d, %d), flags=%x\n", __FUNCTION__,
	     data.region.extents.x1, data.region.extents.y1,
	     data.region.extents.x2, data.region.extents.y2,
	     data.flags));

	data.region.data = NULL;

	if (FORCE_FALLBACK)
		goto fallback;

	if (!ACCEL_POLY_FILL_POLYGON)
		goto fallback;

	data.pixmap = get_drawable_pixmap(draw);
	data.sna = to_sna_from_pixmap(data.pixmap);
	priv = sna_pixmap(data.pixmap);
	if (priv == NULL) {
		DBG(("%s: fallback -- unattached\n", __FUNCTION__));
		goto fallback;
	}

	if (wedged(data.sna)) {
		DBG(("%s: fallback -- wedged\n", __FUNCTION__));
		goto fallback;
	}

	if (!PM_IS_SOLID(draw, gc->planemask))
		goto fallback;

	if ((data.bo = sna_drawable_use_bo(draw,
					   (shape == Convex ? use_zero_spans : use_wide_spans)(draw, gc, &data.region.extents),
					   &data.region.extents,
					   &data.damage))) {
		uint32_t color;

		sna_gc(gc)->priv = &data;
		get_drawable_deltas(draw, data.pixmap, &data.dx, &data.dy);

		if (gc_is_solid(gc, &color)) {
			struct sna_fill_op fill;

			if (!sna_fill_init_blt(&fill,
					       data.sna, data.pixmap,
					       data.bo, gc->alu, color,
					       FILL_SPANS))
				goto fallback;

			data.op = &fill;

			if ((data.flags & 2) == 0) {
				if (data.dx | data.dy)
					sna_gc_ops__tmp.FillSpans = sna_fill_spans__fill_offset;
				else
					sna_gc_ops__tmp.FillSpans = sna_fill_spans__fill;
			} else {
				if (!region_maybe_clip(&data.region,
						       gc->pCompositeClip))
					return;

				if (region_is_singular(&data.region))
					sna_gc_ops__tmp.FillSpans = sna_fill_spans__fill_clip_extents;
				else
					sna_gc_ops__tmp.FillSpans = sna_fill_spans__fill_clip_boxes;
			}
			assert(gc->miTranslate);
			gc->ops = &sna_gc_ops__tmp;

			miFillPolygon(draw, gc, shape, mode, n, pt);
			fill.done(data.sna, &fill);
		} else {
			sna_gc_ops__tmp.FillSpans = sna_fill_spans__gpu;
			gc->ops = &sna_gc_ops__tmp;

			miFillPolygon(draw, gc, shape, mode, n, pt);
		}

		gc->ops = (GCOps *)&sna_gc_ops;
		if (data.damage) {
			if (data.dx | data.dy)
				pixman_region_translate(&data.region, data.dx, data.dy);
			assert_pixmap_contains_box(data.pixmap, &data.region.extents);
			sna_damage_add(data.damage, &data.region);
		}
		assert_pixmap_damage(data.pixmap);
		RegionUninit(&data.region);
		return;
	}

fallback:
	DBG(("%s: fallback (%d, %d), (%d, %d)\n", __FUNCTION__,
	     data.region.extents.x1, data.region.extents.y1,
	     data.region.extents.x2, data.region.extents.y2));
	if (!region_maybe_clip(&data.region, gc->pCompositeClip)) {
		DBG(("%s: nothing to do, all clipped\n", __FUNCTION__));
		return;
	}

	if (!sna_gc_move_to_cpu(gc, draw, &data.region))
		goto out;
	if (!sna_drawable_move_region_to_cpu(draw, &data.region,
					     drawable_gc_flags(draw, gc, true)))
		goto out;

	if (sigtrap_get() == 0) {
		DBG(("%s: fallback -- miFillPolygon -> sna_fill_spans__cpu\n",
		     __FUNCTION__));
		miFillPolygon(draw, gc, shape, mode, n, pt);
		sigtrap_put();
	}
out:
	sna_gc_move_to_gpu(gc);
	RegionUninit(&data.region);
}

static struct kgem_bo *
sna_pixmap_get_source_bo(PixmapPtr pixmap)
{
	struct sna_pixmap *priv = sna_pixmap(pixmap);
	unsigned flags;
	BoxRec box;

	box.x1 = box.y1 = 0;
	box.x2 = pixmap->drawable.width;
	box.y2 = pixmap->drawable.height;

	DBG(("%s(pixmap=%ld, size=%dx%d)\n", __FUNCTION__,
	     pixmap->drawable.serialNumber, pixmap->drawable.width, pixmap->drawable.height));

	if (priv == NULL) {
		DBG(("%s: unattached, uploading data into temporary\n", __FUNCTION__));
		return kgem_upload_source_image(&to_sna_from_pixmap(pixmap)->kgem,
						pixmap->devPrivate.ptr, &box,
						pixmap->devKind,
						pixmap->drawable.bitsPerPixel);
	}

	if (priv->gpu_damage) {
		if (sna_pixmap_move_to_gpu(pixmap, MOVE_READ | MOVE_ASYNC_HINT))
			return kgem_bo_reference(priv->gpu_bo);
	} else if (priv->cpu_damage) {
		if (priv->cpu_bo)
			return kgem_bo_reference(priv->cpu_bo);
	} else {
		if (priv->gpu_bo)
			return kgem_bo_reference(priv->gpu_bo);
		if (priv->cpu_bo)
			return kgem_bo_reference(priv->cpu_bo);
	}

	flags = MOVE_READ | MOVE_ASYNC_HINT;
	if (priv->gpu_bo && priv->gpu_bo->proxy) {
		struct kgem_bo *bo = priv->gpu_bo;
		if (bo->rq == NULL && (bo->snoop || bo->pitch >= 4096))
			flags |= __MOVE_FORCE;
	}
	if (priv->gpu_bo == NULL) {
		if (++priv->source_count > SOURCE_BIAS)
			flags |= __MOVE_FORCE;
	}

	if (!sna_pixmap_move_to_gpu(pixmap, flags)) {
		struct kgem_bo *upload;

		if (!sna_pixmap_move_to_cpu(pixmap, MOVE_READ))
			return NULL;

		upload = kgem_upload_source_image(&to_sna_from_pixmap(pixmap)->kgem,
						  pixmap->devPrivate.ptr, &box,
						  pixmap->devKind,
						  pixmap->drawable.bitsPerPixel);
		if (upload == NULL)
			return NULL;

		if (priv->gpu_bo == NULL) {
			DBG(("%s: adding upload cache to pixmap=%ld\n",
			     __FUNCTION__, pixmap->drawable.serialNumber));
			assert(upload->proxy != NULL);
			kgem_proxy_bo_attach(upload, &priv->gpu_bo);
		}

		return upload;
	}

	return kgem_bo_reference(priv->gpu_bo);
}

/*
static bool
tile(DrawablePtr drawable,
	struct kgem_bo *bo, struct sna_damage **damage,
	PixmapPtr tile, const DDXPointRec * const origin, int alu,
	int n, xRectangle *rect,
	const BoxRec *extents, unsigned clipped)
	*/

static bool
sna_poly_fill_rect_tiled_8x8_blt(DrawablePtr drawable,
				 struct kgem_bo *bo, struct sna_damage **damage,
				 struct kgem_bo *tile_bo, GCPtr gc,
				 int n, const xRectangle *r,
				 const BoxRec *extents, unsigned clipped)
{
	PixmapPtr pixmap = get_drawable_pixmap(drawable);
	struct sna *sna = to_sna_from_pixmap(pixmap);
	const DDXPointRec * const origin = &gc->patOrg;
	uint32_t br00, br13;
	int tx, ty;
	int16_t dx, dy;
	uint32_t *b;

	if (NO_TILE_8x8)
		return false;

	DBG(("%s x %d [(%d, %d)x(%d, %d)...], clipped=%x, origin=(%d, %d)\n",
	     __FUNCTION__, n, r->x, r->y, r->width, r->height, clipped, origin->x, origin->y));

	DBG(("%s: tile_bo tiling=%d, pitch=%d\n", __FUNCTION__, tile_bo->tiling, tile_bo->pitch));
	if (tile_bo->tiling)
		return false;

	assert(tile_bo->pitch == 8 * drawable->bitsPerPixel >> 3);

	kgem_set_mode(&sna->kgem, KGEM_BLT, bo);
	if (!kgem_check_batch(&sna->kgem, 10+2*3) ||
	    !kgem_check_reloc(&sna->kgem, 2) ||
	    !kgem_check_many_bo_fenced(&sna->kgem, bo, tile_bo, NULL)) {
		kgem_submit(&sna->kgem);
		if (!kgem_check_many_bo_fenced(&sna->kgem, bo, tile_bo, NULL))
			return false;
		_kgem_set_mode(&sna->kgem, KGEM_BLT);
	}

	get_drawable_deltas(drawable, pixmap, &dx, &dy);
	assert(extents->x1 + dx >= 0);
	assert(extents->y1 + dy >= 0);
	assert(extents->x2 + dx <= pixmap->drawable.width);
	assert(extents->y2 + dy <= pixmap->drawable.height);

	br00 = XY_SCANLINE_BLT;
	tx = (-drawable->x - dx - origin->x) % 8;
	if (tx < 0)
		tx += 8;
	ty = (-drawable->y - dy - origin->y) % 8;
	if (ty < 0)
		ty += 8;
	br00 |= tx << 12 | ty << 8;

	br13 = bo->pitch;
	if (sna->kgem.gen >= 040 && bo->tiling) {
		br00 |= BLT_DST_TILED;
		br13 >>= 2;
	}
	br13 |= blt_depth(drawable->depth) << 24;
	br13 |= fill_ROP[gc->alu] << 16;

	if (!clipped) {
		dx += drawable->x;
		dy += drawable->y;

		sna_damage_add_rectangles(damage, r, n, dx, dy);
		if (n == 1) {
			DBG(("%s: rect=(%d, %d)x(%d, %d) + (%d, %d), tile=(%d, %d)\n",
			     __FUNCTION__, r->x, r->y, r->width, r->height, dx, dy, tx, ty));

			assert(r->x + dx >= 0);
			assert(r->y + dy >= 0);
			assert(r->x + dx + r->width  <= pixmap->drawable.width);
			assert(r->y + dy + r->height <= pixmap->drawable.height);

			assert(sna->kgem.mode == KGEM_BLT);
			b = sna->kgem.batch + sna->kgem.nbatch;
			if (sna->kgem.gen >= 0100) {
				b[0] = XY_PAT_BLT | 3 << 20 | (br00 & 0x7f00) | 6;
				b[1] = br13;
				b[2] = (r->y + dy) << 16 | (r->x + dx);
				b[3] = (r->y + r->height + dy) << 16 | (r->x + r->width + dx);
				*(uint64_t *)(b+4) =
					kgem_add_reloc64(&sna->kgem, sna->kgem.nbatch + 4, bo,
							 I915_GEM_DOMAIN_RENDER << 16 |
							 I915_GEM_DOMAIN_RENDER |
							 KGEM_RELOC_FENCED,
							 0);
				*(uint64_t *)(b+6) =
					kgem_add_reloc64(&sna->kgem, sna->kgem.nbatch + 6, tile_bo,
							 I915_GEM_DOMAIN_RENDER << 16 |
							 KGEM_RELOC_FENCED,
							 0);
				sna->kgem.nbatch += 8;
			} else {
				b[0] = XY_PAT_BLT | 3 << 20 | (br00 & 0x7f00) | 4;
				b[1] = br13;
				b[2] = (r->y + dy) << 16 | (r->x + dx);
				b[3] = (r->y + r->height + dy) << 16 | (r->x + r->width + dx);
				b[4] = kgem_add_reloc(&sna->kgem, sna->kgem.nbatch + 4, bo,
						      I915_GEM_DOMAIN_RENDER << 16 |
						      I915_GEM_DOMAIN_RENDER |
						      KGEM_RELOC_FENCED,
						      0);
				b[5] = kgem_add_reloc(&sna->kgem, sna->kgem.nbatch + 5, tile_bo,
						      I915_GEM_DOMAIN_RENDER << 16 |
						      KGEM_RELOC_FENCED,
						      0);
				sna->kgem.nbatch += 6;
			}
		} else do {
			int n_this_time;

			assert(sna->kgem.mode == KGEM_BLT);
			b = sna->kgem.batch + sna->kgem.nbatch;
			if (sna->kgem.gen >= 0100) {
				b[0] = XY_SETUP_BLT | 3 << 20 | (br00 & BLT_DST_TILED) | 8;
				b[1] = br13;
				b[2] = 0;
				b[3] = 0;
				*(uint64_t *)(b+4) =
					kgem_add_reloc64(&sna->kgem, sna->kgem.nbatch + 4, bo,
							 I915_GEM_DOMAIN_RENDER << 16 |
							 I915_GEM_DOMAIN_RENDER |
							 KGEM_RELOC_FENCED,
							 0);
				b[6] = gc->bgPixel;
				b[7] = gc->fgPixel;
				*(uint64_t *)(b+8) =
					kgem_add_reloc64(&sna->kgem, sna->kgem.nbatch + 8, tile_bo,
							 I915_GEM_DOMAIN_RENDER << 16 |
							 KGEM_RELOC_FENCED,
							 0);
				sna->kgem.nbatch += 10;
			} else {
				b[0] = XY_SETUP_BLT | 3 << 20 | (br00 & BLT_DST_TILED) | 6;
				b[1] = br13;
				b[2] = 0;
				b[3] = 0;
				b[4] = kgem_add_reloc(&sna->kgem, sna->kgem.nbatch + 4, bo,
						      I915_GEM_DOMAIN_RENDER << 16 |
						      I915_GEM_DOMAIN_RENDER |
						      KGEM_RELOC_FENCED,
						      0);
				b[5] = gc->bgPixel;
				b[6] = gc->fgPixel;
				b[7] = kgem_add_reloc(&sna->kgem, sna->kgem.nbatch + 7, tile_bo,
						      I915_GEM_DOMAIN_RENDER << 16 |
						      KGEM_RELOC_FENCED,
						      0);
				sna->kgem.nbatch += 8;
			}

			n_this_time = n;
			if (3*n_this_time > sna->kgem.surface - sna->kgem.nbatch - KGEM_BATCH_RESERVED)
				n_this_time = (sna->kgem.surface - sna->kgem.nbatch - KGEM_BATCH_RESERVED) / 3;
			assert(n_this_time);
			n -= n_this_time;

			assert(sna->kgem.mode == KGEM_BLT);
			b = sna->kgem.batch + sna->kgem.nbatch;
			sna->kgem.nbatch += 3*n_this_time;
			do {
				assert(r->x + dx >= 0);
				assert(r->y + dy >= 0);
				assert(r->x + dx + r->width  <= pixmap->drawable.width);
				assert(r->y + dy + r->height <= pixmap->drawable.height);

				b[0] = br00;
				b[1] = (r->y + dy) << 16 | (r->x + dx);
				b[2] = (r->y + r->height + dy) << 16 | (r->x + r->width + dx);
				b += 3; r++;
			} while (--n_this_time);

			if (!n)
				break;

			_kgem_submit(&sna->kgem);
			_kgem_set_mode(&sna->kgem, KGEM_BLT);
		} while (1);
	} else {
		RegionRec clip;
		uint16_t unwind_batch, unwind_reloc;

		region_set(&clip, extents);
		if (!region_maybe_clip(&clip, gc->pCompositeClip))
			goto done;

		unwind_batch = sna->kgem.nbatch;
		unwind_reloc = sna->kgem.nreloc;

		assert(sna->kgem.mode == KGEM_BLT);
		b = sna->kgem.batch + sna->kgem.nbatch;
		if (sna->kgem.gen >= 0100) {
			b[0] = XY_SETUP_BLT | 3 << 20 | (br00 & BLT_DST_TILED) | 8;
			b[1] = br13;
			b[2] = 0;
			b[3] = 0;
			*(uint64_t *)(b+4) =
				kgem_add_reloc64(&sna->kgem, sna->kgem.nbatch + 4, bo,
						 I915_GEM_DOMAIN_RENDER << 16 |
						 I915_GEM_DOMAIN_RENDER |
						 KGEM_RELOC_FENCED,
						 0);
			b[6] = gc->bgPixel;
			b[7] = gc->fgPixel;
			*(uint64_t *)(b+8) =
				kgem_add_reloc64(&sna->kgem, sna->kgem.nbatch + 8, tile_bo,
						 I915_GEM_DOMAIN_RENDER << 16 |
						 KGEM_RELOC_FENCED,
						 0);
			sna->kgem.nbatch += 10;
		} else {
			b[0] = XY_SETUP_BLT | 3 << 20 | (br00 & BLT_DST_TILED) | 6;
			b[1] = br13;
			b[2] = 0;
			b[3] = 0;
			b[4] = kgem_add_reloc(&sna->kgem, sna->kgem.nbatch + 4, bo,
					      I915_GEM_DOMAIN_RENDER << 16 |
					      I915_GEM_DOMAIN_RENDER |
					      KGEM_RELOC_FENCED,
					      0);
			b[5] = gc->bgPixel;
			b[6] = gc->fgPixel;
			b[7] = kgem_add_reloc(&sna->kgem, sna->kgem.nbatch + 7, tile_bo,
					      I915_GEM_DOMAIN_RENDER << 16 |
					      KGEM_RELOC_FENCED,
					      0);
			sna->kgem.nbatch += 8;
		}

		if (clip.data == NULL) {
			const BoxRec *c = &clip.extents;
			DBG(("%s: simple clip, %d boxes\n", __FUNCTION__, n));
			while (n--) {
				BoxRec box;

				box.x1 = r->x + drawable->x;
				box.y1 = r->y + drawable->y;
				box.x2 = bound(box.x1, r->width);
				box.y2 = bound(box.y1, r->height);
				r++;

				if (box_intersect(&box, c)) {
					if (!kgem_check_batch(&sna->kgem, 3)) {
						_kgem_submit(&sna->kgem);
						_kgem_set_mode(&sna->kgem, KGEM_BLT);

						unwind_batch = sna->kgem.nbatch;
						unwind_reloc = sna->kgem.nreloc;

						assert(sna->kgem.mode == KGEM_BLT);
						b = sna->kgem.batch + sna->kgem.nbatch;
						if (sna->kgem.gen >= 0100) {
							b[0] = XY_SETUP_BLT | 3 << 20 | (br00 & BLT_DST_TILED) | 8;
							b[1] = br13;
							b[2] = 0;
							b[3] = 0;
							*(uint64_t *)(b+4) =
								kgem_add_reloc64(&sna->kgem, sna->kgem.nbatch + 4, bo,
										 I915_GEM_DOMAIN_RENDER << 16 |
										 I915_GEM_DOMAIN_RENDER |
										 KGEM_RELOC_FENCED,
										 0);
							b[6] = gc->bgPixel;
							b[7] = gc->fgPixel;
							*(uint64_t *)(b+8) =
								kgem_add_reloc64(&sna->kgem, sna->kgem.nbatch + 8, tile_bo,
										 I915_GEM_DOMAIN_RENDER << 16 |
										 KGEM_RELOC_FENCED,
										 0);
							sna->kgem.nbatch += 10;
						} else {
							b[0] = XY_SETUP_BLT | 3 << 20 | (br00 & BLT_DST_TILED) | 6;
							b[1] = br13;
							b[2] = 0;
							b[3] = 0;
							b[4] = kgem_add_reloc(&sna->kgem, sna->kgem.nbatch + 4, bo,
									      I915_GEM_DOMAIN_RENDER << 16 |
									      I915_GEM_DOMAIN_RENDER |
									      KGEM_RELOC_FENCED,
									      0);
							b[5] = gc->bgPixel;
							b[6] = gc->fgPixel;
							b[7] = kgem_add_reloc(&sna->kgem, sna->kgem.nbatch + 7, tile_bo,
									      I915_GEM_DOMAIN_RENDER << 16 |
									      KGEM_RELOC_FENCED,
									      0);
							sna->kgem.nbatch += 8;
						}
					}

					assert(box.x1 + dx >= 0);
					assert(box.y1 + dy >= 0);
					assert(box.x2 + dx <= pixmap->drawable.width);
					assert(box.y2 + dy <= pixmap->drawable.height);

					DBG(("%s: box=(%d, %d),(%d, %d) + (%d, %d), tile=(%d, %d)\n",
					     __FUNCTION__, box.x1, box.y1, box.x2, box.y2, dx, dy, tx, ty));

					assert(sna->kgem.mode == KGEM_BLT);
					b = sna->kgem.batch + sna->kgem.nbatch;
					b[0] = br00;
					b[1] = (box.y1 + dy) << 16 | (box.x1 + dx);
					b[2] = (box.y2 + dy) << 16 | (box.x2 + dx);
					sna->kgem.nbatch += 3;
				}
			}
		} else {
			const BoxRec * const clip_start = RegionBoxptr(&clip);
			const BoxRec * const clip_end = clip_start + clip.data->numRects;
			const BoxRec *c;

			DBG(("%s: complex clip (%ld cliprects), %d boxes\n", __FUNCTION__, (long)clip.data->numRects, n));
			do {
				BoxRec box;

				box.x1 = r->x + drawable->x;
				box.y1 = r->y + drawable->y;
				box.x2 = bound(box.x1, r->width);
				box.y2 = bound(box.y1, r->height);
				DBG(("%s: rect=(%d, %d), (%d, %d), box=(%d, %d), (%d, %d)\n", __FUNCTION__,
				     r->x, r->y, r->width, r->height,
				     box.x1, box.y1, box.x2, box.y2));
				r++;

				c = find_clip_box_for_y(clip_start,
							clip_end,
							box.y1);
				while (c != clip_end) {
					BoxRec bb;

					DBG(("%s: clip=(%d, %d), (%d, %d)\n", __FUNCTION__, c->x1, c->y1, c->x2, c->y2));

					if (box.y2 <= c->y1)
						break;

					bb = box;
					if (box_intersect(&bb, c++)) {
						if (!kgem_check_batch(&sna->kgem, 3)) {
							DBG(("%s: emitting split batch\n", __FUNCTION__));
							_kgem_submit(&sna->kgem);
							_kgem_set_mode(&sna->kgem, KGEM_BLT);

							unwind_batch = sna->kgem.nbatch;
							unwind_reloc = sna->kgem.nreloc;

							assert(sna->kgem.mode == KGEM_BLT);
							b = sna->kgem.batch + sna->kgem.nbatch;
							if (sna->kgem.gen >= 0100) {
								b[0] = XY_SETUP_BLT | 3 << 20 | (br00 & BLT_DST_TILED) | 8;
								b[1] = br13;
								b[2] = 0;
								b[3] = 0;
								*(uint64_t *)(b+4) =
									kgem_add_reloc64(&sna->kgem, sna->kgem.nbatch + 4, bo,
											 I915_GEM_DOMAIN_RENDER << 16 |
											 I915_GEM_DOMAIN_RENDER |
											 KGEM_RELOC_FENCED,
											 0);
								b[6] = gc->bgPixel;
								b[7] = gc->fgPixel;
								*(uint64_t *)(b+8) =
									kgem_add_reloc64(&sna->kgem, sna->kgem.nbatch + 8, tile_bo,
											 I915_GEM_DOMAIN_RENDER << 16 |
											 KGEM_RELOC_FENCED,
											 0);
								sna->kgem.nbatch += 10;
							} else {
								b[0] = XY_SETUP_BLT | 3 << 20 | (br00 & BLT_DST_TILED) | 6;
								b[1] = br13;
								b[2] = 0;
								b[3] = 0;
								b[4] = kgem_add_reloc(&sna->kgem, sna->kgem.nbatch + 4, bo,
										      I915_GEM_DOMAIN_RENDER << 16 |
										      I915_GEM_DOMAIN_RENDER |
										      KGEM_RELOC_FENCED,
										      0);
								b[5] = gc->bgPixel;
								b[6] = gc->fgPixel;
								b[7] = kgem_add_reloc(&sna->kgem, sna->kgem.nbatch + 7, tile_bo,
										      I915_GEM_DOMAIN_RENDER << 16 |
										      KGEM_RELOC_FENCED,
										      0);
								sna->kgem.nbatch += 8;
							}
						}

						assert(bb.x1 + dx >= 0);
						assert(bb.y1 + dy >= 0);
						assert(bb.x2 + dx <= pixmap->drawable.width);
						assert(bb.y2 + dy <= pixmap->drawable.height);

						DBG(("%s: emit box=(%d, %d),(%d, %d) + (%d, %d), tile=(%d, %d) [relative to drawable: (%d, %d)]\n",
						     __FUNCTION__, bb.x1, bb.y1, bb.x2, bb.y2, dx, dy, tx, ty, bb.x1 - drawable->x, bb.y1 - drawable->y));

						assert(sna->kgem.mode == KGEM_BLT);
						b = sna->kgem.batch + sna->kgem.nbatch;
						b[0] = br00;
						b[1] = (bb.y1 + dy) << 16 | (bb.x1 + dx);
						b[2] = (bb.y2 + dy) << 16 | (bb.x2 + dx);
						sna->kgem.nbatch += 3;
					}
				}
			} while (--n);
		}

		if (sna->kgem.nbatch == unwind_batch + (sna->kgem.gen >= 0100 ? 10 : 8)) {
			sna->kgem.nbatch = unwind_batch;
			sna->kgem.nreloc = unwind_reloc;
			if (sna->kgem.nbatch == 0)
				kgem_bo_pair_undo(&sna->kgem, bo, tile_bo);
		}
	}
done:
	assert_pixmap_damage(pixmap);
	sna->blt_state.fill_bo = 0;
	return true;
}

static bool tile8(int x)
{
	switch(x) {
	case 1:
	case 2:
	case 4:
	case 8:
		return true;
	default:
		return false;
	}
}

static int next8(int x, int max)
{
	if (x > 2 && x <= 4)
		x = 4;
	else if (x < 8)
		x = 8;
	return MIN(x, max);
}

static bool
sna_poly_fill_rect_tiled_nxm_blt(DrawablePtr drawable,
				 struct kgem_bo *bo,
				 struct sna_damage **damage,
				 GCPtr gc, int n, const xRectangle *rect,
				 const BoxRec *extents, unsigned clipped)
{
	PixmapPtr pixmap = get_drawable_pixmap(drawable);
	struct sna *sna = to_sna_from_pixmap(pixmap);
	PixmapPtr tile = gc->tile.pixmap;
	int w, h, tx, ty, tw, th, bpp = tile->drawable.bitsPerPixel;
	const DDXPointRec origin = gc->patOrg;
	struct kgem_bo *upload;
	bool ret = false;
	uint8_t *src;
	void *ptr;

	tx = 0, tw = tile->drawable.width;
	if (!tile8(tw) && tw > extents->x2 - extents->x1) {
		tx = (extents->x1 - gc->patOrg.x - drawable->x) % tw;
		if (tx < 0)
			tx += tw;
		tw = next8(extents->x2 - extents->x1, tw);
		gc->patOrg.x = extents->x1 - drawable->x;
	}

	ty = 0, th = tile->drawable.height;
	if (!tile8(th) && th > extents->y2 - extents->y1) {
		ty = (extents->y1 - gc->patOrg.y - drawable->y) % th;
		if (ty < 0)
			ty += th;
		th = next8(extents->y2 - extents->y1, th);
		gc->patOrg.y = extents->y1 - drawable->y;
	}

	DBG(("%s: %dx%d+%d+%d (full tile size %dx%d)\n", __FUNCTION__,
	     tw, th, tx, ty, tile->drawable.width, tile->drawable.height));
	assert(tx < tile->drawable.width && tx >= 0);
	assert(ty < tile->drawable.height && ty >= 0);
	assert(tw && tw <= 8 && tw <= tile->drawable.width);
	assert(is_power_of_two(tw));
	assert(th && th <= 8 && th <= tile->drawable.height);
	assert(is_power_of_two(th));

	if (!sna_pixmap_move_to_cpu(tile, MOVE_READ))
		goto out_gc;

	assert(tile->devKind);
	assert(has_coherent_ptr(sna, sna_pixmap(tile), MOVE_READ));

	src = tile->devPrivate.ptr;
	src += tile->devKind * ty;
	src += tx * bpp/8;

	if ((tw | th) == 1) {
		uint32_t pixel;
		switch (bpp) {
			case 32: pixel = *(uint32_t *)src; break;
			case 16: pixel = *(uint16_t *)src; break;
			default: pixel = *(uint8_t *)src; break;
		}
		return sna_poly_fill_rect_blt(drawable, bo, damage,
					      gc, pixel, n, rect,
					      extents, clipped);
	}

	upload = kgem_create_buffer(&sna->kgem, 8*bpp, KGEM_BUFFER_WRITE, &ptr);
	if (upload == NULL)
		goto out_gc;

	upload->pitch = bpp; /* for sanity checks */

	if (sigtrap_get() == 0) {
		uint8_t *dst = ptr;
		if (tx + tw > tile->drawable.width ||
		    ty + th > tile->drawable.height) {
			int sy = ty;
			src = tile->devPrivate.ptr;
			for (h = 0; h < th; h++) {
				int sx = tx;
				for (w = 0; w < tw; w++) {
					memcpy(dst + w*bpp/8, src + sy * tile->devKind + sx*bpp/8, bpp/8);
					if (++sx == tile->drawable.width)
						sx = 0;
				}
				w *= bpp/8;
				while (w < bpp) {
					memcpy(dst+w, dst, w);
					w *= 2;
				}
				if (++sy == tile->drawable.height)
					sy = 0;
				dst += bpp;
			}
			while (h < 8) {
				memcpy(dst, ptr, bpp*h);
				dst += bpp * h;
				h *= 2;
			}
		} else {
			for (h = 0; h < th; h++) {
				w = tw*bpp/8;
				memcpy(dst, src, w);
				while (w < bpp) {
					memcpy(dst+w, dst, w);
					w *= 2;
				}
				assert(w == bpp);

				src += tile->devKind;
				dst += bpp;
			}
			while (h < 8) {
				memcpy(dst, ptr, bpp*h);
				dst += bpp * h;
				h *= 2;
			}
			assert(h == 8);
		}

		ret = sna_poly_fill_rect_tiled_8x8_blt(drawable, bo, damage,
						       upload, gc, n, rect,
						       extents, clipped);
		sigtrap_put();
	}

	kgem_bo_destroy(&sna->kgem, upload);
out_gc:
	gc->patOrg = origin;
	return ret;
}

inline static bool tile_is_solid(GCPtr gc, uint32_t *pixel)
{
	PixmapPtr tile = gc->tile.pixmap;
	struct sna_pixmap *priv;

	if ((tile->drawable.width | tile->drawable.height) == 1) {
		DBG(("%s: single pixel tile pixmap, converting to solid fill\n", __FUNCTION__));
		*pixel = get_pixel(tile);
		return true;
	}

	priv = sna_pixmap(tile);
	if (priv == NULL || !priv->clear)
		return false;

	DBG(("%s: tile is clear, converting to solid fill\n", __FUNCTION__));
	*pixel = priv->clear_color;
	return true;
}

static bool
sna_poly_fill_rect_tiled_blt(DrawablePtr drawable,
			     struct kgem_bo *bo,
			     struct sna_damage **damage,
			     GCPtr gc, int n, xRectangle *rect,
			     const BoxRec *extents, unsigned clipped)
{
	PixmapPtr pixmap = get_drawable_pixmap(drawable);
	struct sna *sna = to_sna_from_pixmap(pixmap);
	PixmapPtr tile = gc->tile.pixmap;
	struct kgem_bo *tile_bo;
	const DDXPointRec * const origin = &gc->patOrg;
	struct sna_copy_op copy;
	CARD32 alu = gc->alu;
	int tile_width, tile_height;
	int16_t dx, dy;
	uint32_t pixel;

	DBG(("%s pixmap=%ld, x %d [(%d, %d)x(%d, %d)...], clipped? %d\n",
	     __FUNCTION__, pixmap->drawable.serialNumber,
	     n, rect->x, rect->y, rect->width, rect->height,
	     clipped));

	assert(tile->drawable.depth == drawable->depth);
	assert(bo);

	if (tile_is_solid(gc, &pixel))
		return sna_poly_fill_rect_blt(drawable, bo, damage,
					      gc, pixel,
					      n, rect,
					      extents, clipped);

	/* XXX [248]x[238] tiling can be reduced to a pattern fill.
	 * Also we can do the lg2 reduction for BLT and use repeat modes for
	 * RENDER.
	 */

	tile_width = tile->drawable.width;
	tile_height = tile->drawable.height;
	if ((tile_width | tile_height) == 8) {
		bool ret;

		DBG(("%s: have 8x8 tile, using BLT fast path\n", __FUNCTION__));

		tile_bo = sna_pixmap_get_source_bo(tile);
		if (tile_bo == NULL) {
			DBG(("%s: unable to move tile go GPU, fallback\n",
			     __FUNCTION__));
			return false;
		}

		ret = sna_poly_fill_rect_tiled_8x8_blt(drawable, bo, damage,
						       tile_bo, gc, n, rect,
						       extents, clipped);
		if (ret) {
			kgem_bo_destroy(&sna->kgem, tile_bo);
			return true;
		}
	} else {
		int w = tile_width, h = tile_height;
		struct sna_pixmap *priv = sna_pixmap(tile);

		if (priv == NULL || priv->gpu_damage == NULL) {
			w = next8(extents->x2 - extents->x1, w);
			h = next8(extents->y2 - extents->y1, h);
		}

		DBG(("%s: not 8x8, triming size for tile: %dx%d from %dx%d (area %dx%d)\n",
		     __FUNCTION__, w, h, tile_width, tile_height, extents->x2-extents->x1, extents->y2-extents->y1));

		if ((w|h) < 0x10 && is_power_of_two(w) && is_power_of_two(h) &&
		    sna_poly_fill_rect_tiled_nxm_blt(drawable, bo, damage,
						     gc, n, rect,
						     extents, clipped))
			return true;

		tile_bo = sna_pixmap_get_source_bo(tile);
		if (tile_bo == NULL) {
			DBG(("%s: unable to move tile go GPU, fallback\n",
						__FUNCTION__));
			return false;
		}
	}

	if (!sna_copy_init_blt(&copy, sna, tile, tile_bo, pixmap, bo, alu)) {
		DBG(("%s: unsupported blt\n", __FUNCTION__));
		kgem_bo_destroy(&sna->kgem, tile_bo);
		return false;
	}

	get_drawable_deltas(drawable, pixmap, &dx, &dy);
	DBG(("%s: drawable offset into pixmap(%ld) = (%d, %d)\n",
	     __FUNCTION__, pixmap->drawable.serialNumber, dx, dy));
	if (!clipped) {
		dx += drawable->x;
		dy += drawable->y;

		sna_damage_add_rectangles(damage, rect, n, dx, dy);
		do {
			xRectangle r = *rect++;
			int16_t tile_y = (r.y - origin->y) % tile_height;
			if (tile_y < 0)
				tile_y += tile_height;

			assert(r.x + dx >= 0);
			assert(r.y + dy >= 0);
			assert(r.x + dx + r.width  <= pixmap->drawable.width);
			assert(r.y + dy + r.height <= pixmap->drawable.height);

			r.y += dy;
			do {
				int16_t width = r.width;
				int16_t x = r.x + dx, tile_x;
				int16_t h = tile_height - tile_y;
				if (h > r.height)
					h = r.height;
				r.height -= h;

				tile_x = (r.x - origin->x) % tile_width;
				if (tile_x < 0)
					tile_x += tile_width;

				do {
					int16_t w = tile_width - tile_x;
					if (w > width)
						w = width;
					width -= w;

					copy.blt(sna, &copy,
						 tile_x, tile_y,
						 w, h,
						 x, r.y);

					x += w;
					tile_x = 0;
				} while (width);
				r.y += h;
				tile_y = 0;
			} while (r.height);
		} while (--n);
	} else {
		RegionRec clip;

		region_set(&clip, extents);
		if (!region_maybe_clip(&clip, gc->pCompositeClip))
			goto done;

		if (clip.data == NULL) {
			const BoxRec *box = &clip.extents;
			DBG(("%s: single clip box [(%d, %d), (%d, %d)]\n",
			     __FUNCTION__, box->x1, box->y1, box->x2, box->y2));
			while (n--) {
				BoxRec r;

				r.x1 = rect->x + drawable->x;
				r.y1 = rect->y + drawable->y;
				r.x2 = bound(r.x1, rect->width);
				r.y2 = bound(r.y1, rect->height);
				rect++;

				DBG(("%s: rectangle [(%d, %d), (%d, %d)]\n",
				     __FUNCTION__, r.x1, r.y1, r.x2, r.y2));

				if (box_intersect(&r, box)) {
					int height = r.y2 - r.y1;
					int dst_y = r.y1;
					int tile_y = (r.y1 - drawable->y - origin->y) % tile_height;
					if (tile_y < 0)
						tile_y += tile_height;

					assert(r.x1 + dx >= 0);
					assert(r.y1 + dy >= 0);
					assert(r.x2 + dx <= pixmap->drawable.width);
					assert(r.y2 + dy <= pixmap->drawable.height);

					while (height) {
						int width = r.x2 - r.x1;
						int dst_x = r.x1, tile_x;
						int h = tile_height - tile_y;
						if (h > height)
							h = height;
						height -= h;

						tile_x = (r.x1 - drawable->x - origin->x) % tile_width;
						if (tile_x < 0)
							tile_x += tile_width;

						while (width > 0) {
							int w = tile_width - tile_x;
							if (w > width)
								w = width;
							width -= w;

							copy.blt(sna, &copy,
								 tile_x, tile_y,
								 w, h,
								 dst_x + dx, dst_y + dy);
							if (damage) {
								BoxRec b;

								b.x1 = dst_x + dx;
								b.y1 = dst_y + dy;
								b.x2 = b.x1 + w;
								b.y2 = b.y1 + h;

								assert_pixmap_contains_box(pixmap, &b);
								sna_damage_add_box(damage, &b);
							}

							dst_x += w;
							tile_x = 0;
						}
						dst_y += h;
						tile_y = 0;
					}
				}
			}
		} else {
			while (n--) {
				RegionRec region;
				const BoxRec *box;
				int nbox;

				region.extents.x1 = rect->x + drawable->x;
				region.extents.y1 = rect->y + drawable->y;
				region.extents.x2 = bound(region.extents.x1, rect->width);
				region.extents.y2 = bound(region.extents.y1, rect->height);
				rect++;

				DBG(("%s: rectangle [(%d, %d), (%d, %d)]\n",
				     __FUNCTION__,
				     region.extents.x1,
				     region.extents.y1,
				     region.extents.x2,
				     region.extents.y2));

				region.data = NULL;
				RegionIntersect(&region, &region, &clip);

				assert(region.extents.x1 + dx >= 0);
				assert(region.extents.y1 + dy >= 0);
				assert(region.extents.x2 + dx <= pixmap->drawable.width);
				assert(region.extents.y2 + dy <= pixmap->drawable.height);

				nbox = region_num_rects(&region);
				box = region_rects(&region);
				DBG(("%s: split into %d boxes after clipping\n", __FUNCTION__, nbox));
				while (nbox--) {
					int height = box->y2 - box->y1;
					int dst_y = box->y1;
					int tile_y = (box->y1 - drawable->y - origin->y) % tile_height;
					if (tile_y < 0)
						tile_y += tile_height;

					while (height) {
						int width = box->x2 - box->x1;
						int dst_x = box->x1, tile_x;
						int h = tile_height - tile_y;
						if (h > height)
							h = height;
						height -= h;

						tile_x = (box->x1 - drawable->x - origin->x) % tile_width;
						if (tile_x < 0)
							tile_x += tile_width;

						while (width > 0) {
							int w = tile_width - tile_x;
							if (w > width)
								w = width;
							width -= w;

							copy.blt(sna, &copy,
								 tile_x, tile_y,
								 w, h,
								 dst_x + dx, dst_y + dy);
							if (damage) {
								BoxRec b;

								b.x1 = dst_x + dx;
								b.y1 = dst_y + dy;
								b.x2 = b.x1 + w;
								b.y2 = b.y1 + h;

								assert_pixmap_contains_box(pixmap, &b);
								sna_damage_add_box(damage, &b);
							}

							dst_x += w;
							tile_x = 0;
						}
						dst_y += h;
						tile_y = 0;
					}
					box++;
				}

				RegionUninit(&region);
			}
		}

		RegionUninit(&clip);
	}
done:
	copy.done(sna, &copy);
	assert_pixmap_damage(pixmap);
	kgem_bo_destroy(&sna->kgem, tile_bo);
	return true;
}

static bool
sna_poly_fill_rect_stippled_8x8_blt(DrawablePtr drawable,
				    struct kgem_bo *bo,
				    struct sna_damage **damage,
				    GCPtr gc, int n, xRectangle *r,
				    const BoxRec *extents, unsigned clipped)
{
	PixmapPtr pixmap = get_drawable_pixmap(drawable);
	struct sna *sna = to_sna_from_pixmap(pixmap);
	uint32_t pat[2] = {0, 0}, br00, br13;
	int16_t dx, dy;
	uint32_t *b;

	if (NO_STIPPLE_8x8)
		return false;

	DBG(("%s: alu=%d, upload (%d, %d), (%d, %d), origin (%d, %d)\n",
	     __FUNCTION__, gc->alu,
	     extents->x1, extents->y1,
	     extents->x2, extents->y2,
	     gc->patOrg.x, gc->patOrg.y));

	get_drawable_deltas(drawable, pixmap, &dx, &dy);
	{
		int px, py;

		px = (0 - gc->patOrg.x - drawable->x - dx) % 8;
		if (px < 0)
			px += 8;

		py = (0 - gc->patOrg.y - drawable->y - dy) % 8;
		if (py < 0)
			py += 8;
		DBG(("%s: pat offset (%d, %d)\n", __FUNCTION__ ,px, py));

		br00 = XY_SCANLINE_BLT | px << 12 | py << 8 | 3 << 20;
		br13 = bo->pitch;
		if (sna->kgem.gen >= 040 && bo->tiling) {
			br00 |= BLT_DST_TILED;
			br13 >>= 2;
		}
		br13 |= (gc->fillStyle == FillStippled) << 28;
		br13 |= blt_depth(drawable->depth) << 24;
		br13 |= fill_ROP[gc->alu] << 16;
	}

	assert(gc->stipple->devKind);
	{
		uint8_t *dst = (uint8_t *)pat;
		const uint8_t *src = gc->stipple->devPrivate.ptr;
		int stride = gc->stipple->devKind;
		int j = gc->stipple->drawable.height;
		do {
			*dst++ = byte_reverse(*src);
			src += stride;
		} while (--j);
	}

	kgem_set_mode(&sna->kgem, KGEM_BLT, bo);
	if (!kgem_check_batch(&sna->kgem, 10 + 2*3) ||
	    !kgem_check_bo_fenced(&sna->kgem, bo) ||
	    !kgem_check_reloc(&sna->kgem, 1)) {
		kgem_submit(&sna->kgem);
		if (!kgem_check_bo_fenced(&sna->kgem, bo))
			return false;
		_kgem_set_mode(&sna->kgem, KGEM_BLT);
	}

	if (!clipped) {
		dx += drawable->x;
		dy += drawable->y;

		sna_damage_add_rectangles(damage, r, n, dx, dy);
		if (n == 1) {
			DBG(("%s: single unclipped rect (%d, %d)x(%d, %d)\n",
			     __FUNCTION__, r->x + dx, r->y + dy, r->width, r->height));

			assert(sna->kgem.mode == KGEM_BLT);
			b = sna->kgem.batch + sna->kgem.nbatch;
			if (sna->kgem.gen >= 0100) {
				b[0] = XY_MONO_PAT | (br00 & 0x7f00) | 3<<20 | 8;
				b[1] = br13;
				b[2] = (r->y + dy) << 16 | (r->x + dx);
				b[3] = (r->y + r->height + dy) << 16 | (r->x + r->width + dx);
				*(uint64_t *)(b+4) =
					kgem_add_reloc64(&sna->kgem, sna->kgem.nbatch + 4, bo,
							 I915_GEM_DOMAIN_RENDER << 16 |
							 I915_GEM_DOMAIN_RENDER |
							 KGEM_RELOC_FENCED,
							 0);
				b[6] = gc->bgPixel;
				b[7] = gc->fgPixel;
				b[8] = pat[0];
				b[9] = pat[1];
				sna->kgem.nbatch += 10;
			} else {
				b[0] = XY_MONO_PAT | (br00 & 0x7f00) | 3<<20 | 7;
				b[1] = br13;
				b[2] = (r->y + dy) << 16 | (r->x + dx);
				b[3] = (r->y + r->height + dy) << 16 | (r->x + r->width + dx);
				b[4] = kgem_add_reloc(&sna->kgem, sna->kgem.nbatch + 4, bo,
						      I915_GEM_DOMAIN_RENDER << 16 |
						      I915_GEM_DOMAIN_RENDER |
						      KGEM_RELOC_FENCED,
						      0);
				b[5] = gc->bgPixel;
				b[6] = gc->fgPixel;
				b[7] = pat[0];
				b[8] = pat[1];
				sna->kgem.nbatch += 9;
			}
		} else do {
			int n_this_time;

			assert(sna->kgem.mode == KGEM_BLT);
			b = sna->kgem.batch + sna->kgem.nbatch;
			if (sna->kgem.gen >= 0100) {
				b[0] = XY_SETUP_MONO_PATTERN_SL_BLT | 3 << 20 | (br00 & BLT_DST_TILED) | 8;
				b[1] = br13;
				b[2] = 0;
				b[3] = 0;
				*(uint64_t *)(b+4) =
					kgem_add_reloc64(&sna->kgem, sna->kgem.nbatch + 4, bo,
							 I915_GEM_DOMAIN_RENDER << 16 |
							 I915_GEM_DOMAIN_RENDER |
							 KGEM_RELOC_FENCED,
							 0);
				b[6] = gc->bgPixel;
				b[7] = gc->fgPixel;
				b[8] = pat[0];
				b[9] = pat[1];
				sna->kgem.nbatch += 10;
			} else {
				b[0] = XY_SETUP_MONO_PATTERN_SL_BLT | 3 << 20 | (br00 & BLT_DST_TILED) | 7;
				b[1] = br13;
				b[2] = 0;
				b[3] = 0;
				b[4] = kgem_add_reloc(&sna->kgem, sna->kgem.nbatch + 4, bo,
						      I915_GEM_DOMAIN_RENDER << 16 |
						      I915_GEM_DOMAIN_RENDER |
						      KGEM_RELOC_FENCED,
						      0);
				b[5] = gc->bgPixel;
				b[6] = gc->fgPixel;
				b[7] = pat[0];
				b[8] = pat[1];
				sna->kgem.nbatch += 9;
			}

			n_this_time = n;
			if (3*n_this_time > sna->kgem.surface - sna->kgem.nbatch - KGEM_BATCH_RESERVED)
				n_this_time = (sna->kgem.surface - sna->kgem.nbatch - KGEM_BATCH_RESERVED) / 3;
			assert(n_this_time);
			n -= n_this_time;

			assert(sna->kgem.mode == KGEM_BLT);
			b = sna->kgem.batch + sna->kgem.nbatch;
			sna->kgem.nbatch += 3 * n_this_time;
			do {
				DBG(("%s: rect (%d, %d)x(%d, %d)\n",
				     __FUNCTION__, r->x + dx, r->y + dy, r->width, r->height));
				assert(r->x + dx >= 0);
				assert(r->y + dy >= 0);
				assert(r->x + dx + r->width  <= pixmap->drawable.width);
				assert(r->y + dy + r->height <= pixmap->drawable.height);

				b[0] = br00;
				b[1] = (r->y + dy) << 16 | (r->x + dx);
				b[2] = (r->y + r->height + dy) << 16 | (r->x + r->width + dx);

				b += 3; r++;
			} while(--n_this_time);

			if (!n)
				break;

			_kgem_submit(&sna->kgem);
			_kgem_set_mode(&sna->kgem, KGEM_BLT);
		} while (1);
	} else {
		RegionRec clip;

		region_set(&clip, extents);
		if (!region_maybe_clip(&clip, gc->pCompositeClip))
			return true;

		assert(sna->kgem.mode == KGEM_BLT);
		b = sna->kgem.batch + sna->kgem.nbatch;
		if (sna->kgem.gen >= 0100) {
			b[0] = XY_SETUP_MONO_PATTERN_SL_BLT | 3 << 20 | (br00 & BLT_DST_TILED) | 8;
			b[1] = br13;
			b[2] = 0;
			b[3] = 0;
			*(uint64_t *)(b+4) =
				kgem_add_reloc64(&sna->kgem, sna->kgem.nbatch + 4, bo,
						 I915_GEM_DOMAIN_RENDER << 16 |
						 I915_GEM_DOMAIN_RENDER |
						 KGEM_RELOC_FENCED,
						 0);
			b[6] = gc->bgPixel;
			b[7] = gc->fgPixel;
			b[8] = pat[0];
			b[9] = pat[1];
			sna->kgem.nbatch += 10;
		} else {
			b[0] = XY_SETUP_MONO_PATTERN_SL_BLT | 3 << 20 | (br00 & BLT_DST_TILED) | 7;
			b[1] = br13;
			b[2] = 0;
			b[3] = 0;
			b[4] = kgem_add_reloc(&sna->kgem, sna->kgem.nbatch + 4, bo,
					      I915_GEM_DOMAIN_RENDER << 16 |
					      I915_GEM_DOMAIN_RENDER |
					      KGEM_RELOC_FENCED,
					      0);
			b[5] = gc->bgPixel;
			b[6] = gc->fgPixel;
			b[7] = pat[0];
			b[8] = pat[1];
			sna->kgem.nbatch += 9;
		}

		if (clip.data == NULL) {
			do {
				BoxRec box;

				box.x1 = r->x + drawable->x;
				box.y1 = r->y + drawable->y;
				box.x2 = bound(box.x1, r->width);
				box.y2 = bound(box.y1, r->height);
				r++;

				if (box_intersect(&box, &clip.extents)) {
					if (!kgem_check_batch(&sna->kgem, 3)) {
						_kgem_submit(&sna->kgem);
						_kgem_set_mode(&sna->kgem, KGEM_BLT);

						assert(sna->kgem.mode == KGEM_BLT);
						b = sna->kgem.batch + sna->kgem.nbatch;
						if (sna->kgem.gen >= 0100) {
							b[0] = XY_SETUP_MONO_PATTERN_SL_BLT | 3 << 20 | (br00 & BLT_DST_TILED) | 8;
							b[1] = br13;
							b[2] = 0;
							b[3] = 0;
							*(uint64_t *)(b+4) =
								kgem_add_reloc64(&sna->kgem, sna->kgem.nbatch + 4, bo,
										 I915_GEM_DOMAIN_RENDER << 16 |
										 I915_GEM_DOMAIN_RENDER |
										 KGEM_RELOC_FENCED,
										 0);
							b[6] = gc->bgPixel;
							b[7] = gc->fgPixel;
							b[8] = pat[0];
							b[9] = pat[1];
							sna->kgem.nbatch += 10;
						} else {
							b[0] = XY_SETUP_MONO_PATTERN_SL_BLT | 3 << 20 | (br00 & BLT_DST_TILED) | 7;
							b[1] = br13;
							b[2] = 0;
							b[3] = 0;
							b[4] = kgem_add_reloc(&sna->kgem, sna->kgem.nbatch + 4, bo,
									      I915_GEM_DOMAIN_RENDER << 16 |
									      I915_GEM_DOMAIN_RENDER |
									      KGEM_RELOC_FENCED,
									      0);
							b[5] = gc->bgPixel;
							b[6] = gc->fgPixel;
							b[7] = pat[0];
							b[8] = pat[1];
							sna->kgem.nbatch += 9;
						}
					}

					assert(sna->kgem.mode == KGEM_BLT);
					b = sna->kgem.batch + sna->kgem.nbatch;
					sna->kgem.nbatch += 3;
					b[0] = br00;
					b[1] = (box.y1 + dy) << 16 | (box.x1 + dx);
					b[2] = (box.y2 + dy) << 16 | (box.x2 + dx);
				}
			} while (--n);
		} else {
			const BoxRec * const clip_start = RegionBoxptr(&clip);
			const BoxRec * const clip_end = clip_start + clip.data->numRects;
			const BoxRec *c;

			do {
				BoxRec box;

				box.x1 = r->x + drawable->x;
				box.y1 = r->y + drawable->y;
				box.x2 = bound(box.x1, r->width);
				box.y2 = bound(box.y1, r->height);
				r++;

				c = find_clip_box_for_y(clip_start,
							clip_end,
							box.y1);
				while (c != clip_end) {
					BoxRec bb;
					if (box.y2 <= c->y1)
						break;

					bb = box;
					if (box_intersect(&bb, c++)) {
						if (!kgem_check_batch(&sna->kgem, 3)) {
							_kgem_submit(&sna->kgem);
							_kgem_set_mode(&sna->kgem, KGEM_BLT);

							assert(sna->kgem.mode == KGEM_BLT);
							b = sna->kgem.batch + sna->kgem.nbatch;
							if (sna->kgem.gen >= 0100) {
								b[0] = XY_SETUP_MONO_PATTERN_SL_BLT | 3 << 20 | (br00 & BLT_DST_TILED) | 8;
								b[1] = br13;
								b[2] = 0;
								b[3] = 0;
								*(uint64_t *)(b+4) =
									kgem_add_reloc64(&sna->kgem, sna->kgem.nbatch + 4, bo,
											 I915_GEM_DOMAIN_RENDER << 16 |
											 I915_GEM_DOMAIN_RENDER |
											 KGEM_RELOC_FENCED,
											 0);
								b[6] = gc->bgPixel;
								b[7] = gc->fgPixel;
								b[8] = pat[0];
								b[9] = pat[1];
								sna->kgem.nbatch += 10;
							} else {
								b[0] = XY_SETUP_MONO_PATTERN_SL_BLT | 3 << 20 | (br00 & BLT_DST_TILED) | 7;
								b[1] = br13;
								b[2] = 0;
								b[3] = 0;
								b[4] = kgem_add_reloc(&sna->kgem, sna->kgem.nbatch + 4, bo,
										      I915_GEM_DOMAIN_RENDER << 16 |
										      I915_GEM_DOMAIN_RENDER |
										      KGEM_RELOC_FENCED,
										      0);
								b[5] = gc->bgPixel;
								b[6] = gc->fgPixel;
								b[7] = pat[0];
								b[8] = pat[1];
								sna->kgem.nbatch += 9;
							}
						}

						assert(sna->kgem.mode == KGEM_BLT);
						b = sna->kgem.batch + sna->kgem.nbatch;
						sna->kgem.nbatch += 3;
						b[0] = br00;
						b[1] = (bb.y1 + dy) << 16 | (bb.x1 + dx);
						b[2] = (bb.y2 + dy) << 16 | (bb.x2 + dx);
					}
				}
			} while (--n);
		}
	}

	assert_pixmap_damage(pixmap);
	sna->blt_state.fill_bo = 0;
	return true;
}

static bool
sna_poly_fill_rect_stippled_nxm_blt(DrawablePtr drawable,
				    struct kgem_bo *bo,
				    struct sna_damage **damage,
				    GCPtr gc, int n, xRectangle *r,
				    const BoxRec *extents, unsigned clipped)
{
	PixmapPtr scratch, stipple;
	uint8_t bytes[8], *dst = bytes;
	const uint8_t *src, *end;
	int j, stride;
	bool ret;

	DBG(("%s: expanding %dx%d stipple to 8x8\n",
	     __FUNCTION__,
	     gc->stipple->drawable.width,
	     gc->stipple->drawable.height));

	scratch = GetScratchPixmapHeader(drawable->pScreen,
					 8, 8, 1, 1, 1, bytes);
	if (scratch == NullPixmap)
		return false;

	stipple = gc->stipple;
	gc->stipple = scratch;

	assert(stipple->devKind);
	stride = stipple->devKind;
	src = stipple->devPrivate.ptr;
	end = src + stride * stipple->drawable.height;
	for(j = 0; j < 8; j++) {
		switch (stipple->drawable.width) {
		case 1: *dst = (*src & 1) * 0xff; break;
		case 2: *dst = (*src & 3) * 0x55; break;
		case 4: *dst = (*src & 15) * 0x11; break;
		case 8: *dst = *src; break;
		default: assert(0); break;
		}
		dst++;
		src += stride;
		if (src == end)
			src = stipple->devPrivate.ptr;
	}

	ret = sna_poly_fill_rect_stippled_8x8_blt(drawable, bo, damage,
						  gc, n, r, extents, clipped);

	gc->stipple = stipple;
	FreeScratchPixmapHeader(scratch);

	return ret;
}

static bool
sna_poly_fill_rect_stippled_1_blt(DrawablePtr drawable,
				  struct kgem_bo *bo,
				  struct sna_damage **damage,
				  GCPtr gc, int n, xRectangle *r,
				  const BoxRec *extents, unsigned clipped)
{
	PixmapPtr pixmap = get_drawable_pixmap(drawable);
	struct sna *sna = to_sna_from_pixmap(pixmap);
	PixmapPtr stipple = gc->stipple;
	const DDXPointRec *origin = &gc->patOrg;
	int16_t dx, dy;
	uint32_t br00, br13;

	DBG(("%s: upload (%d, %d), (%d, %d), origin (%d, %d), clipped=%x\n", __FUNCTION__,
	     extents->x1, extents->y1,
	     extents->x2, extents->y2,
	     origin->x, origin->y,
	     clipped));

	get_drawable_deltas(drawable, pixmap, &dx, &dy);
	kgem_set_mode(&sna->kgem, KGEM_BLT, bo);

	br00 = 3 << 20;
	br13 = bo->pitch;
	if (sna->kgem.gen >= 040 && bo->tiling) {
		br00 |= BLT_DST_TILED;
		br13 >>= 2;
	}
	br13 |= (gc->fillStyle == FillStippled) << 29;
	br13 |= blt_depth(drawable->depth) << 24;
	br13 |= copy_ROP[gc->alu] << 16;

	if (!clipped) {
		dx += drawable->x;
		dy += drawable->y;

		sna_damage_add_rectangles(damage, r, n, dx, dy);
		do {
			int bx1 = (r->x - origin->x) & ~7;
			int bx2 = (r->x + r->width - origin->x + 7) & ~7;
			int bw = (bx2 - bx1)/8;
			int bh = r->height;
			int bstride = ALIGN(bw, 2);
			int src_stride;
			uint8_t *dst, *src;
			uint32_t *b;

			DBG(("%s: rect (%d, %d)x(%d, %d) stipple [%d,%d]\n",
			     __FUNCTION__,
			     r->x, r->y, r->width, r->height,
			     bx1, bx2));

			src_stride = bstride*bh;
			assert(src_stride > 0);
			if (src_stride <= 128) {
				src_stride = ALIGN(src_stride, 8) / 4;
				assert(src_stride <= 32);
				if (!kgem_check_batch(&sna->kgem, 8+src_stride) ||
				    !kgem_check_bo_fenced(&sna->kgem, bo) ||
				    !kgem_check_reloc(&sna->kgem, 1)) {
					kgem_submit(&sna->kgem);
					if (!kgem_check_bo_fenced(&sna->kgem, bo))
						return false;
					_kgem_set_mode(&sna->kgem, KGEM_BLT);
				}

				assert(sna->kgem.mode == KGEM_BLT);
				b = sna->kgem.batch + sna->kgem.nbatch;
				if (sna->kgem.gen >= 0100) {
					b[0] = XY_MONO_SRC_COPY_IMM | (6 + src_stride) | br00;
					b[0] |= ((r->x - origin->x) & 7) << 17;
					b[1] = br13;
					b[2] = (r->y + dy) << 16 | (r->x + dx);
					b[3] = (r->y + r->height + dy) << 16 | (r->x + r->width + dx);
					*(uint64_t *)(b+4) =
						kgem_add_reloc64(&sna->kgem, sna->kgem.nbatch + 4, bo,
								 I915_GEM_DOMAIN_RENDER << 16 |
								 I915_GEM_DOMAIN_RENDER |
								 KGEM_RELOC_FENCED,
								 0);
					b[6] = gc->bgPixel;
					b[7] = gc->fgPixel;

					dst = (uint8_t *)&b[8];
					sna->kgem.nbatch += 8 + src_stride;
				} else {
					b[0] = XY_MONO_SRC_COPY_IMM | (5 + src_stride) | br00;
					b[0] |= ((r->x - origin->x) & 7) << 17;
					b[1] = br13;
					b[2] = (r->y + dy) << 16 | (r->x + dx);
					b[3] = (r->y + r->height + dy) << 16 | (r->x + r->width + dx);
					b[4] = kgem_add_reloc(&sna->kgem, sna->kgem.nbatch + 4, bo,
							      I915_GEM_DOMAIN_RENDER << 16 |
							      I915_GEM_DOMAIN_RENDER |
							      KGEM_RELOC_FENCED,
							      0);
					b[5] = gc->bgPixel;
					b[6] = gc->fgPixel;

					dst = (uint8_t *)&b[7];
					sna->kgem.nbatch += 7 + src_stride;
				}
				assert(stipple->devKind);
				src_stride = stipple->devKind;
				src = stipple->devPrivate.ptr;
				src += (r->y - origin->y) * src_stride + bx1/8;
				src_stride -= bstride;
				do {
					int i = bstride;
					do {
						*dst++ = byte_reverse(*src++);
						*dst++ = byte_reverse(*src++);
						i -= 2;
					} while (i);
					src += src_stride;
				} while (--bh);
			} else {
				struct kgem_bo *upload;
				void *ptr;

				if (!kgem_check_batch(&sna->kgem, 10) ||
				    !kgem_check_bo_fenced(&sna->kgem, bo) ||
				    !kgem_check_reloc_and_exec(&sna->kgem, 2)) {
					kgem_submit(&sna->kgem);
					if (!kgem_check_bo_fenced(&sna->kgem, bo))
						return false;
					_kgem_set_mode(&sna->kgem, KGEM_BLT);
				}

				upload = kgem_create_buffer(&sna->kgem,
							    bstride*bh,
							    KGEM_BUFFER_WRITE_INPLACE,
							    &ptr);
				if (!upload)
					break;

				if (sigtrap_get() == 0) {
					dst = ptr;
					assert(stipple->devKind);
					src_stride = stipple->devKind;
					src = stipple->devPrivate.ptr;
					src += (r->y - origin->y) * src_stride + bx1/8;
					src_stride -= bstride;
					do {
						int i = bstride;
						do {
							*dst++ = byte_reverse(*src++);
							*dst++ = byte_reverse(*src++);
							i -= 2;
						} while (i);
						src += src_stride;
					} while (--bh);

					assert(sna->kgem.mode == KGEM_BLT);
					b = sna->kgem.batch + sna->kgem.nbatch;
					if (sna->kgem.gen >= 0100) {
						b[0] = XY_MONO_SRC_COPY | br00 | 8;
						b[0] |= ((r->x - origin->x) & 7) << 17;
						b[1] = br13;
						b[2] = (r->y + dy) << 16 | (r->x + dx);
						b[3] = (r->y + r->height + dy) << 16 | (r->x + r->width + dx);
						*(uint64_t *)(b+4) =
							kgem_add_reloc64(&sna->kgem, sna->kgem.nbatch + 4, bo,
									I915_GEM_DOMAIN_RENDER << 16 |
									I915_GEM_DOMAIN_RENDER |
									KGEM_RELOC_FENCED,
									0);
						*(uint64_t *)(b+6) =
							kgem_add_reloc64(&sna->kgem, sna->kgem.nbatch + 6, upload,
									I915_GEM_DOMAIN_RENDER << 16 |
									KGEM_RELOC_FENCED,
									0);
						b[8] = gc->bgPixel;
						b[9] = gc->fgPixel;
						sna->kgem.nbatch += 10;
					} else {
						b[0] = XY_MONO_SRC_COPY | br00 | 6;
						b[0] |= ((r->x - origin->x) & 7) << 17;
						b[1] = br13;
						b[2] = (r->y + dy) << 16 | (r->x + dx);
						b[3] = (r->y + r->height + dy) << 16 | (r->x + r->width + dx);
						b[4] = kgem_add_reloc(&sna->kgem, sna->kgem.nbatch + 4, bo,
								I915_GEM_DOMAIN_RENDER << 16 |
								I915_GEM_DOMAIN_RENDER |
								KGEM_RELOC_FENCED,
								0);
						b[5] = kgem_add_reloc(&sna->kgem, sna->kgem.nbatch + 5, upload,
								I915_GEM_DOMAIN_RENDER << 16 |
								KGEM_RELOC_FENCED,
								0);
						b[6] = gc->bgPixel;
						b[7] = gc->fgPixel;

						sna->kgem.nbatch += 8;
					}
					sigtrap_put();
				}

				kgem_bo_destroy(&sna->kgem, upload);
			}

			r++;
		} while (--n);
	} else {
		RegionRec clip;
		DDXPointRec pat;

		region_set(&clip, extents);
		if (!region_maybe_clip(&clip, gc->pCompositeClip))
			return true;

		pat.x = origin->x + drawable->x;
		pat.y = origin->y + drawable->y;

		if (clip.data == NULL) {
			do {
				BoxRec box;
				int bx1, bx2, bw, bh, bstride;
				int src_stride;
				uint8_t *dst, *src;
				uint32_t *b;
				struct kgem_bo *upload;
				void *ptr;

				box.x1 = r->x + drawable->x;
				box.x2 = bound(box.x1, r->width);
				box.y1 = r->y + drawable->y;
				box.y2 = bound(box.y1, r->height);
				r++;

				if (!box_intersect(&box, &clip.extents))
					continue;

				bx1 = (box.x1 - pat.x) & ~7;
				bx2 = (box.x2 - pat.x + 7) & ~7;
				bw = (bx2 - bx1)/8;
				bh = box.y2 - box.y1;
				bstride = ALIGN(bw, 2);

				DBG(("%s: rect (%d, %d)x(%d, %d), box (%d,%d),(%d,%d) stipple [%d,%d], pitch=%d, stride=%d\n",
				     __FUNCTION__,
				     r->x, r->y, r->width, r->height,
				     box.x1, box.y1, box.x2, box.y2,
				     bx1, bx2, bw, bstride));

				src_stride = bstride*bh;
				assert(src_stride > 0);
				if (src_stride <= 128) {
					src_stride = ALIGN(src_stride, 8) / 4;
					assert(src_stride <= 32);
					if (!kgem_check_batch(&sna->kgem, 8+src_stride) ||
					    !kgem_check_bo_fenced(&sna->kgem, bo) ||
					    !kgem_check_reloc(&sna->kgem, 1)) {
						kgem_submit(&sna->kgem);
						if (!kgem_check_bo_fenced(&sna->kgem, bo))
							return false;
						_kgem_set_mode(&sna->kgem, KGEM_BLT);
					}

					assert(sna->kgem.mode == KGEM_BLT);
					b = sna->kgem.batch + sna->kgem.nbatch;
					if (sna->kgem.gen >= 0100) {
						b[0] = XY_MONO_SRC_COPY_IMM | (6 + src_stride) | br00;
						b[0] |= ((box.x1 - pat.x) & 7) << 17;
						b[1] = br13;
						b[2] = (box.y1 + dy) << 16 | (box.x1 + dx);
						b[3] = (box.y2 + dy) << 16 | (box.x2 + dx);
						*(uint64_t *)(b+4) =
							kgem_add_reloc64(&sna->kgem, sna->kgem.nbatch + 4, bo,
									 I915_GEM_DOMAIN_RENDER << 16 |
									 I915_GEM_DOMAIN_RENDER |
									 KGEM_RELOC_FENCED,
									 0);
						b[6] = gc->bgPixel;
						b[7] = gc->fgPixel;

						dst = (uint8_t *)&b[8];
						sna->kgem.nbatch += 8 + src_stride;
					} else {
						b[0] = XY_MONO_SRC_COPY_IMM | (5 + src_stride) | br00;
						b[0] |= ((box.x1 - pat.x) & 7) << 17;
						b[1] = br13;
						b[2] = (box.y1 + dy) << 16 | (box.x1 + dx);
						b[3] = (box.y2 + dy) << 16 | (box.x2 + dx);
						b[4] = kgem_add_reloc(&sna->kgem, sna->kgem.nbatch + 4, bo,
								      I915_GEM_DOMAIN_RENDER << 16 |
								      I915_GEM_DOMAIN_RENDER |
								      KGEM_RELOC_FENCED,
								      0);
						b[5] = gc->bgPixel;
						b[6] = gc->fgPixel;

						dst = (uint8_t *)&b[7];
						sna->kgem.nbatch += 7 + src_stride;
					}

					assert(stipple->devKind);
					src_stride = stipple->devKind;
					src = stipple->devPrivate.ptr;
					src += (box.y1 - pat.y) * src_stride + bx1/8;
					src_stride -= bstride;
					do {
						int i = bstride;
						do {
							*dst++ = byte_reverse(*src++);
							*dst++ = byte_reverse(*src++);
							i -= 2;
						} while (i);
						src += src_stride;
					} while (--bh);
				} else {
					if (!kgem_check_batch(&sna->kgem, 10) ||
					    !kgem_check_bo_fenced(&sna->kgem, bo) ||
					    !kgem_check_reloc_and_exec(&sna->kgem, 2)) {
						kgem_submit(&sna->kgem);
						if (!kgem_check_bo_fenced(&sna->kgem, bo))
							return false;
						_kgem_set_mode(&sna->kgem, KGEM_BLT);
					}

					upload = kgem_create_buffer(&sna->kgem,
								    bstride*bh,
								    KGEM_BUFFER_WRITE_INPLACE,
								    &ptr);
					if (!upload)
						break;

					if (sigtrap_get() == 0) {
						dst = ptr;
						assert(stipple->devKind);
						src_stride = stipple->devKind;
						src = stipple->devPrivate.ptr;
						src += (box.y1 - pat.y) * src_stride + bx1/8;
						src_stride -= bstride;
						do {
							int i = bstride;
							do {
								*dst++ = byte_reverse(*src++);
								*dst++ = byte_reverse(*src++);
								i -= 2;
							} while (i);
							src += src_stride;
						} while (--bh);

						assert(sna->kgem.mode == KGEM_BLT);
						b = sna->kgem.batch + sna->kgem.nbatch;
						if (sna->kgem.gen >= 0100) {
							b[0] = XY_MONO_SRC_COPY | br00 | 8;
							b[0] |= ((box.x1 - pat.x) & 7) << 17;
							b[1] = br13;
							b[2] = (box.y1 + dy) << 16 | (box.x1 + dx);
							b[3] = (box.y2 + dy) << 16 | (box.x2 + dx);
							*(uint64_t *)(b+4) =
								kgem_add_reloc64(&sna->kgem, sna->kgem.nbatch + 4, bo,
										I915_GEM_DOMAIN_RENDER << 16 |
										I915_GEM_DOMAIN_RENDER |
										KGEM_RELOC_FENCED,
										0);
							*(uint64_t *)(b+5) =
								kgem_add_reloc64(&sna->kgem, sna->kgem.nbatch + 6, upload,
										I915_GEM_DOMAIN_RENDER << 16 |
										KGEM_RELOC_FENCED,
										0);
							b[8] = gc->bgPixel;
							b[9] = gc->fgPixel;
							sna->kgem.nbatch += 10;
						} else {
							b[0] = XY_MONO_SRC_COPY | br00 | 6;
							b[0] |= ((box.x1 - pat.x) & 7) << 17;
							b[1] = br13;
							b[2] = (box.y1 + dy) << 16 | (box.x1 + dx);
							b[3] = (box.y2 + dy) << 16 | (box.x2 + dx);
							b[4] = kgem_add_reloc(&sna->kgem, sna->kgem.nbatch + 4, bo,
									I915_GEM_DOMAIN_RENDER << 16 |
									I915_GEM_DOMAIN_RENDER |
									KGEM_RELOC_FENCED,
									0);
							b[5] = kgem_add_reloc(&sna->kgem, sna->kgem.nbatch + 5, upload,
									I915_GEM_DOMAIN_RENDER << 16 |
									KGEM_RELOC_FENCED,
									0);
							b[6] = gc->bgPixel;
							b[7] = gc->fgPixel;

							sna->kgem.nbatch += 8;
						}
						sigtrap_put();
					}
					kgem_bo_destroy(&sna->kgem, upload);
				}
			} while (--n);
		} else {
			const BoxRec * const clip_start = RegionBoxptr(&clip);
			const BoxRec * const clip_end = clip_start + clip.data->numRects;
			const BoxRec *c;

			do {
				BoxRec unclipped;
				int bx1, bx2, bw, bh, bstride;
				int src_stride;
				uint8_t *dst, *src;
				uint32_t *b;
				struct kgem_bo *upload;
				void *ptr;

				unclipped.x1 = r->x + drawable->x;
				unclipped.x2 = bound(unclipped.x1, r->width);
				unclipped.y1 = r->y + drawable->y;
				unclipped.y2 = bound(unclipped.y1, r->height);
				r++;

				c = find_clip_box_for_y(clip_start,
							clip_end,
							unclipped.y1);
				while (c != clip_end) {
					BoxRec box;

					if (unclipped.y2 <= c->y1)
						break;

					box = unclipped;
					if (!box_intersect(&box, c++))
						continue;

					bx1 = (box.x1 - pat.x) & ~7;
					bx2 = (box.x2 - pat.x + 7) & ~7;
					bw = (bx2 - bx1)/8;
					bh = box.y2 - box.y1;
					bstride = ALIGN(bw, 2);

					DBG(("%s: rect (%d, %d)x(%d, %d), box (%d,%d),(%d,%d) stipple [%d,%d]\n",
					     __FUNCTION__,
					     r->x, r->y, r->width, r->height,
					     box.x1, box.y1, box.x2, box.y2,
					     bx1, bx2));

					src_stride = bstride*bh;
					assert(src_stride > 0);
					if (src_stride <= 128) {
						src_stride = ALIGN(src_stride, 8) / 4;
						assert(src_stride <= 32);
						if (!kgem_check_batch(&sna->kgem, 8+src_stride) ||
						    !kgem_check_bo_fenced(&sna->kgem, bo) ||
						    !kgem_check_reloc(&sna->kgem, 1)) {
							kgem_submit(&sna->kgem);
							if (!kgem_check_bo_fenced(&sna->kgem, bo))
								return false;
							_kgem_set_mode(&sna->kgem, KGEM_BLT);
						}

						assert(sna->kgem.mode == KGEM_BLT);
						b = sna->kgem.batch + sna->kgem.nbatch;
						if (sna->kgem.gen >= 0100) {
							b[0] = XY_MONO_SRC_COPY_IMM | (6 + src_stride) | br00;
							b[0] |= ((box.x1 - pat.x) & 7) << 17;
							b[1] = br13;
							b[2] = (box.y1 + dy) << 16 | (box.x1 + dx);
							b[3] = (box.y2 + dy) << 16 | (box.x2 + dx);
							*(uint64_t *)(b+4) =
								kgem_add_reloc64(&sna->kgem, sna->kgem.nbatch + 4, bo,
										 I915_GEM_DOMAIN_RENDER << 16 |
										 I915_GEM_DOMAIN_RENDER |
										 KGEM_RELOC_FENCED,
										 0);
							b[6] = gc->bgPixel;
							b[7] = gc->fgPixel;

							dst = (uint8_t *)&b[8];
							sna->kgem.nbatch += 8 + src_stride;
						} else {
							b[0] = XY_MONO_SRC_COPY_IMM | (5 + src_stride) | br00;
							b[0] |= ((box.x1 - pat.x) & 7) << 17;
							b[1] = br13;
							b[2] = (box.y1 + dy) << 16 | (box.x1 + dx);
							b[3] = (box.y2 + dy) << 16 | (box.x2 + dx);
							b[4] = kgem_add_reloc(&sna->kgem, sna->kgem.nbatch + 4, bo,
									      I915_GEM_DOMAIN_RENDER << 16 |
									      I915_GEM_DOMAIN_RENDER |
									      KGEM_RELOC_FENCED,
									      0);
							b[5] = gc->bgPixel;
							b[6] = gc->fgPixel;

							dst = (uint8_t *)&b[7];
							sna->kgem.nbatch += 7 + src_stride;
						}
						assert(stipple->devKind);
						src_stride = stipple->devKind;
						src = stipple->devPrivate.ptr;
						src += (box.y1 - pat.y) * src_stride + bx1/8;
						src_stride -= bstride;
						do {
							int i = bstride;
							do {
								*dst++ = byte_reverse(*src++);
								*dst++ = byte_reverse(*src++);
								i -= 2;
							} while (i);
							src += src_stride;
						} while (--bh);
					} else {
						if (!kgem_check_batch(&sna->kgem, 10) ||
						    !kgem_check_bo_fenced(&sna->kgem, bo) ||
						    !kgem_check_reloc_and_exec(&sna->kgem, 2)) {
							kgem_submit(&sna->kgem);
							if (!kgem_check_bo_fenced(&sna->kgem, bo))
								return false;
							_kgem_set_mode(&sna->kgem, KGEM_BLT);
						}

						upload = kgem_create_buffer(&sna->kgem,
									    bstride*bh,
									    KGEM_BUFFER_WRITE_INPLACE,
									    &ptr);
						if (!upload)
							break;

						if (sigtrap_get() == 0) {
							dst = ptr;
							assert(stipple->devKind);
							src_stride = stipple->devKind;
							src = stipple->devPrivate.ptr;
							src += (box.y1 - pat.y) * src_stride + bx1/8;
							src_stride -= bstride;
							do {
								int i = bstride;
								do {
									*dst++ = byte_reverse(*src++);
									*dst++ = byte_reverse(*src++);
									i -= 2;
								} while (i);
								src += src_stride;
							} while (--bh);

							assert(sna->kgem.mode == KGEM_BLT);
							b = sna->kgem.batch + sna->kgem.nbatch;
							if (sna->kgem.gen >= 0100) {
								b[0] = XY_MONO_SRC_COPY | br00 | 8;
								b[0] |= ((box.x1 - pat.x) & 7) << 17;
								b[1] = br13;
								b[2] = (box.y1 + dy) << 16 | (box.x1 + dx);
								b[3] = (box.y2 + dy) << 16 | (box.x2 + dx);
								*(uint64_t *)(b+4) =
									kgem_add_reloc64(&sna->kgem, sna->kgem.nbatch + 4, bo,
											I915_GEM_DOMAIN_RENDER << 16 |
											I915_GEM_DOMAIN_RENDER |
											KGEM_RELOC_FENCED,
											0);
								*(uint64_t *)(b+6) =
									kgem_add_reloc64(&sna->kgem, sna->kgem.nbatch + 6, upload,
											I915_GEM_DOMAIN_RENDER << 16 |
											KGEM_RELOC_FENCED,
											0);
								b[8] = gc->bgPixel;
								b[9] = gc->fgPixel;
								sna->kgem.nbatch += 10;
							} else {
								b[0] = XY_MONO_SRC_COPY | br00 | 6;
								b[0] |= ((box.x1 - pat.x) & 7) << 17;
								b[1] = br13;
								b[2] = (box.y1 + dy) << 16 | (box.x1 + dx);
								b[3] = (box.y2 + dy) << 16 | (box.x2 + dx);
								b[4] = kgem_add_reloc(&sna->kgem, sna->kgem.nbatch + 4, bo,
										I915_GEM_DOMAIN_RENDER << 16 |
										I915_GEM_DOMAIN_RENDER |
										KGEM_RELOC_FENCED,
										0);
								b[5] = kgem_add_reloc(&sna->kgem, sna->kgem.nbatch + 5, upload,
										I915_GEM_DOMAIN_RENDER << 16 |
										KGEM_RELOC_FENCED,
										0);
								b[6] = gc->bgPixel;
								b[7] = gc->fgPixel;

								sna->kgem.nbatch += 8;
							}
							sigtrap_put();
						}
						kgem_bo_destroy(&sna->kgem, upload);
					}
				}
			} while (--n);

		}
	}

	sna->blt_state.fill_bo = 0;
	return true;
}

static void
sna_poly_fill_rect_stippled_n_box__imm(struct sna *sna,
				       struct kgem_bo *bo,
				       uint32_t br00, uint32_t br13,
				       const GC *gc,
				       const BoxRec *box,
				       const DDXPointRec *origin)
{
	int x1, x2, y1, y2;
	uint32_t *b;

	for (y1 = box->y1; y1 < box->y2; y1 = y2) {
		int oy = (y1 - origin->y) % gc->stipple->drawable.height;
		if (oy < 0)
			oy += gc->stipple->drawable.height;

		y2 = box->y2;
		if (y2 - y1 > gc->stipple->drawable.height - oy)
			y2 = y1 + gc->stipple->drawable.height - oy;

		for (x1 = box->x1; x1 < box->x2; x1 = x2) {
			int bx1, bx2, bw, bh, len, ox;
			uint8_t *dst, *src;

			x2 = box->x2;
			ox = (x1 - origin->x) % gc->stipple->drawable.width;
			if (ox < 0)
				ox += gc->stipple->drawable.width;
			bx1 = ox & ~7;
			bx2 = ox + (x2 - x1);
			if (bx2 > gc->stipple->drawable.width) {
				bx2 = gc->stipple->drawable.width;
				x2 = x1 + bx2-ox;
			}
			bw = (bx2 - bx1 + 7)/8;
			bw = ALIGN(bw, 2);
			bh = y2 - y1;

			DBG(("%s: box((%d, %d)x(%d, %d)) origin=(%d, %d), pat=(%d, %d), up=(%d, %d), stipple=%dx%d\n",
			     __FUNCTION__,
			     x1, y1, x2-x1, y2-y1,
			     origin->x, origin->y,
			     ox, oy, bx1, bx2,
			     gc->stipple->drawable.width,
			     gc->stipple->drawable.height));

			len = bw*bh;
			len = ALIGN(len, 8) / 4;
			assert(len > 0);
			assert(len <= 32);
			if (!kgem_check_batch(&sna->kgem, 8+len) ||
			    !kgem_check_bo_fenced(&sna->kgem, bo) ||
			    !kgem_check_reloc(&sna->kgem, 1)) {
				kgem_submit(&sna->kgem);
				if (!kgem_check_bo_fenced(&sna->kgem, bo))
					return; /* XXX fallback? */
				_kgem_set_mode(&sna->kgem, KGEM_BLT);
			}

			assert(sna->kgem.mode == KGEM_BLT);
			b = sna->kgem.batch + sna->kgem.nbatch;
			if (sna->kgem.gen >= 0100) {
				b[0] = br00 | (6 + len) | (ox & 7) << 17;
				b[1] = br13;
				b[2] = y1 << 16 | x1;
				b[3] = y2 << 16 | x2;
				*(uint64_t *)(b+4) =
					kgem_add_reloc64(&sna->kgem, sna->kgem.nbatch + 4, bo,
							 I915_GEM_DOMAIN_RENDER << 16 |
							 I915_GEM_DOMAIN_RENDER |
							 KGEM_RELOC_FENCED,
							 0);
				b[6] = gc->bgPixel;
				b[7] = gc->fgPixel;
				dst = (uint8_t *)&b[8];
				sna->kgem.nbatch += 8 + len;
			} else {
				b[0] = br00 | (5 + len) | (ox & 7) << 17;
				b[1] = br13;
				b[2] = y1 << 16 | x1;
				b[3] = y2 << 16 | x2;
				b[4] = kgem_add_reloc(&sna->kgem, sna->kgem.nbatch + 4, bo,
						      I915_GEM_DOMAIN_RENDER << 16 |
						      I915_GEM_DOMAIN_RENDER |
						      KGEM_RELOC_FENCED,
						      0);
				b[5] = gc->bgPixel;
				b[6] = gc->fgPixel;
				dst = (uint8_t *)&b[7];
				sna->kgem.nbatch += 7 + len;
			}

			assert(gc->stipple->devKind);
			len = gc->stipple->devKind;
			src = gc->stipple->devPrivate.ptr;
			src += oy*len + ox/8;
			len -= bw;
			do {
				int i = bw;
				do {
					*dst++ = byte_reverse(*src++);
					*dst++ = byte_reverse(*src++);
					i -= 2;
				} while (i);
				src += len;
			} while (--bh);
		}
	}
}

static void
sna_poly_fill_rect_stippled_n_box(struct sna *sna,
				  struct kgem_bo *bo,
				  struct kgem_bo **tile,
				  uint32_t br00, uint32_t br13,
				  const GC *gc,
				  const BoxRec *box,
				  const DDXPointRec *origin)
{
	int x1, x2, y1, y2;
	int w = gc->stipple->drawable.width;
	int h = gc->stipple->drawable.height;
	int stride = gc->stipple->devKind;
	uint32_t *b;

	assert(stride);
	if ((((box->y2-box->y1) | (box->x2-box->x1)) & ~31) == 0) {
		br00 = XY_MONO_SRC_COPY_IMM |(br00 & (BLT_DST_TILED | 3 << 20));
		sna_poly_fill_rect_stippled_n_box__imm(sna, bo,
						       br00, br13, gc,
						       box, origin);
		return;
	}

	for (y1 = box->y1; y1 < box->y2; y1 = y2) {
		int row, oy = (y1 - origin->y) % gc->stipple->drawable.height;
		if (oy < 0)
			oy += h;

		y2 = box->y2;
		if (y2 - y1 > h - oy)
			y2 = y1 + h - oy;

		row = oy * stride;
		for (x1 = box->x1; x1 < box->x2; x1 = x2) {
			int bx1, bx2, bw, bh, len, ox;
			bool use_tile;

			x2 = box->x2;
			ox = (x1 - origin->x) % w;
			if (ox < 0)
				ox += w;
			bx1 = ox & ~7;
			bx2 = ox + (x2 - x1);
			if (bx2 > w) {
				bx2 = w;
				x2 = x1 + bx2-ox;
			}

			use_tile = y2-y1 == h && x2-x1 == w;

			DBG(("%s: box((%d, %d)x(%d, %d)) origin=(%d, %d), pat=(%d, %d), up=(%d, %d), stipple=%dx%d, full tile?=%d\n",
			     __FUNCTION__,
			     x1, y1, x2-x1, y2-y1,
			     origin->x, origin->y,
			     ox, oy, bx1, bx2, w, h,
			     use_tile));

			bw = (bx2 - bx1 + 7)/8;
			bw = ALIGN(bw, 2);
			bh = y2 - y1;

			len = bw*bh;
			len = ALIGN(len, 8) / 4;
			assert(len > 0);
			if (!kgem_check_batch(&sna->kgem, 8+len) ||
			    !kgem_check_bo_fenced(&sna->kgem, bo) ||
			    !kgem_check_reloc(&sna->kgem, 2)) {
				kgem_submit(&sna->kgem);
				if (!kgem_check_bo_fenced(&sna->kgem, bo))
					return; /* XXX fallback? */
				_kgem_set_mode(&sna->kgem, KGEM_BLT);
			}

			assert(sna->kgem.mode == KGEM_BLT);
			b = sna->kgem.batch + sna->kgem.nbatch;

			if (!use_tile && len <= 32) {
				uint8_t *dst, *src;

				if (sna->kgem.gen >= 0100) {
					b[0] = XY_MONO_SRC_COPY_IMM;
					b[0] |= (br00 & (BLT_DST_TILED | 3 << 20));
					b[0] |= (ox & 7) << 17;
					b[0] |= (6 + len);
					b[1] = br13;
					b[2] = y1 << 16 | x1;
					b[3] = y2 << 16 | x2;
					*(uint64_t *)(b+4) =
						kgem_add_reloc64(&sna->kgem, sna->kgem.nbatch + 4, bo,
								 I915_GEM_DOMAIN_RENDER << 16 |
								 I915_GEM_DOMAIN_RENDER |
								 KGEM_RELOC_FENCED,
								 0);
					b[6] = gc->bgPixel;
					b[7] = gc->fgPixel;

					dst = (uint8_t *)&b[8];
					sna->kgem.nbatch += 8 + len;
				} else {
					b[0] = XY_MONO_SRC_COPY_IMM;
					b[0] |= (br00 & (BLT_DST_TILED | 3 << 20));
					b[0] |= (ox & 7) << 17;
					b[0] |= (5 + len);
					b[1] = br13;
					b[2] = y1 << 16 | x1;
					b[3] = y2 << 16 | x2;
					b[4] = kgem_add_reloc(&sna->kgem, sna->kgem.nbatch + 4, bo,
							      I915_GEM_DOMAIN_RENDER << 16 |
							      I915_GEM_DOMAIN_RENDER |
							      KGEM_RELOC_FENCED,
							      0);
					b[5] = gc->bgPixel;
					b[6] = gc->fgPixel;

					dst = (uint8_t *)&b[7];
					sna->kgem.nbatch += 7 + len;
				}

				assert(gc->stipple->devKind);
				len = gc->stipple->devKind;
				src = gc->stipple->devPrivate.ptr;
				src += oy*len + ox/8;
				len -= bw;
				do {
					int i = bw;
					do {
						*dst++ = byte_reverse(*src++);
						*dst++ = byte_reverse(*src++);
						i -= 2;
					} while (i);
					src += len;
				} while (--bh);
			} else {
				bool has_tile = use_tile && *tile;
				struct kgem_bo *upload;
				uint8_t *dst, *src;
				void *ptr;

				if (has_tile) {
					upload = kgem_bo_reference(*tile);
				} else {
					upload = kgem_create_buffer(&sna->kgem, bw*bh,
								    KGEM_BUFFER_WRITE_INPLACE,
								    &ptr);
					if (!upload)
						return;
				}

				assert(sna->kgem.mode == KGEM_BLT);
				b = sna->kgem.batch + sna->kgem.nbatch;
				if (sna->kgem.gen >= 0100) {
					b[0] = br00 | (ox & 7) << 17 | 8;
					b[1] = br13;
					b[2] = y1 << 16 | x1;
					b[3] = y2 << 16 | x2;
					*(uint64_t *)(b+4) =
						kgem_add_reloc64(&sna->kgem, sna->kgem.nbatch + 4, bo,
								 I915_GEM_DOMAIN_RENDER << 16 |
								 I915_GEM_DOMAIN_RENDER |
								 KGEM_RELOC_FENCED,
								 0);
					*(uint64_t *)(b+6) =
						kgem_add_reloc64(&sna->kgem, sna->kgem.nbatch + 6, upload,
							       I915_GEM_DOMAIN_RENDER << 16 |
							       KGEM_RELOC_FENCED,
							       0);
					b[8] = gc->bgPixel;
					b[9] = gc->fgPixel;
					sna->kgem.nbatch += 10;
				} else {
					b[0] = br00 | (ox & 7) << 17 | 6;
					b[1] = br13;
					b[2] = y1 << 16 | x1;
					b[3] = y2 << 16 | x2;
					b[4] = kgem_add_reloc(&sna->kgem, sna->kgem.nbatch + 4, bo,
							      I915_GEM_DOMAIN_RENDER << 16 |
							      I915_GEM_DOMAIN_RENDER |
							      KGEM_RELOC_FENCED,
							      0);
					b[5] = kgem_add_reloc(&sna->kgem, sna->kgem.nbatch + 5, upload,
							      I915_GEM_DOMAIN_RENDER << 16 |
							      KGEM_RELOC_FENCED,
							      0);
					b[6] = gc->bgPixel;
					b[7] = gc->fgPixel;
					sna->kgem.nbatch += 8;
				}

				if (!has_tile) {
					dst = ptr;
					len = stride;
					src = gc->stipple->devPrivate.ptr;
					src += row + (ox >> 3);
					len -= bw;
					do {
						int i = bw;
						do {
							*dst++ = byte_reverse(*src++);
							*dst++ = byte_reverse(*src++);
							i -= 2;
						} while (i);
						src += len;
					} while (--bh);
					if (use_tile)
						*tile = kgem_bo_reference(upload);
				}

				kgem_bo_destroy(&sna->kgem, upload);
			}
		}
	}
}

static bool
sna_poly_fill_rect_stippled_n_blt__imm(DrawablePtr drawable,
				       struct kgem_bo *bo,
				       struct sna_damage **damage,
				       GCPtr gc, int n, xRectangle *r,
				       const BoxRec *extents, unsigned clipped)
{
	PixmapPtr pixmap = get_drawable_pixmap(drawable);
	struct sna *sna = to_sna_from_pixmap(pixmap);
	DDXPointRec origin = gc->patOrg;
	int16_t dx, dy;
	uint32_t br00, br13;

	DBG(("%s: upload (%d, %d), (%d, %d), origin (%d, %d), clipped=%d, alu=%d, opaque=%d\n", __FUNCTION__,
	     extents->x1, extents->y1,
	     extents->x2, extents->y2,
	     origin.x, origin.y,
	     clipped, gc->alu, gc->fillStyle == FillOpaqueStippled));

	get_drawable_deltas(drawable, pixmap, &dx, &dy);
	kgem_set_mode(&sna->kgem, KGEM_BLT, bo);

	br00 = XY_MONO_SRC_COPY_IMM | 3 << 20;
	br13 = bo->pitch;
	if (sna->kgem.gen >= 040 && bo->tiling) {
		br00 |= BLT_DST_TILED;
		br13 >>= 2;
	}
	br13 |= (gc->fillStyle == FillStippled) << 29;
	br13 |= blt_depth(drawable->depth) << 24;
	br13 |= copy_ROP[gc->alu] << 16;

	origin.x += dx + drawable->x;
	origin.y += dy + drawable->y;

	if (!clipped) {
		dx += drawable->x;
		dy += drawable->y;

		sna_damage_add_rectangles(damage, r, n, dx, dy);
		do {
			BoxRec box;

			box.x1 = r->x + dx;
			box.y1 = r->y + dy;
			box.x2 = box.x1 + r->width;
			box.y2 = box.y1 + r->height;

			sna_poly_fill_rect_stippled_n_box__imm(sna, bo,
							       br00, br13, gc,
							       &box, &origin);
			r++;
		} while (--n);
	} else {
		RegionRec clip;

		region_set(&clip, extents);
		if (!region_maybe_clip(&clip, gc->pCompositeClip)) {
			DBG(("%s: all clipped\n", __FUNCTION__));
			return true;
		}

		if (clip.data == NULL) {
			DBG(("%s: clipped to extents ((%d, %d), (%d, %d))\n",
			     __FUNCTION__,
			     clip.extents.x1, clip.extents.y1,
			     clip.extents.x2, clip.extents.y2));
			do {
				BoxRec box;

				box.x1 = r->x + drawable->x;
				box.x2 = bound(box.x1, r->width);
				box.y1 = r->y + drawable->y;
				box.y2 = bound(box.y1, r->height);
				r++;

				DBG(("%s: box (%d, %d), (%d, %d)\n",
				     __FUNCTION__,
				     box.x1, box.y1, box.x2, box.y2));
				if (!box_intersect(&box, &clip.extents))
					continue;

				box.x1 += dx; box.x2 += dx;
				box.y1 += dy; box.y2 += dy;

				sna_poly_fill_rect_stippled_n_box__imm(sna, bo,
								       br00, br13, gc,
								       &box, &origin);
			} while (--n);
		} else {
			const BoxRec * const clip_start = RegionBoxptr(&clip);
			const BoxRec * const clip_end = clip_start + clip.data->numRects;
			const BoxRec *c;

			DBG(("%s: clipped to boxes: start((%d, %d), (%d, %d)); end=((%d, %d), (%d, %d))\n", __FUNCTION__,
			     clip_start->x1, clip_start->y1,
			     clip_start->x2, clip_start->y2,
			     clip_end->x1, clip_end->y1,
			     clip_end->x2, clip_end->y2));
			do {
				BoxRec unclipped;

				unclipped.x1 = r->x + drawable->x;
				unclipped.x2 = bound(unclipped.x1, r->width);
				unclipped.y1 = r->y + drawable->y;
				unclipped.y2 = bound(unclipped.y1, r->height);
				r++;

				c = find_clip_box_for_y(clip_start,
							clip_end,
							unclipped.y1);
				while (c != clip_end) {
					BoxRec box;

					if (unclipped.y2 <= c->y1)
						break;

					box = unclipped;
					if (!box_intersect(&box, c++))
						continue;

					box.x1 += dx; box.x2 += dx;
					box.y1 += dy; box.y2 += dy;

					sna_poly_fill_rect_stippled_n_box__imm(sna, bo,
									       br00, br13, gc,
									       &box, &origin);
				}
			} while (--n);
		}
	}

	assert_pixmap_damage(pixmap);
	sna->blt_state.fill_bo = 0;
	return true;
}

static bool
sna_poly_fill_rect_stippled_n_blt(DrawablePtr drawable,
				  struct kgem_bo *bo,
				  struct sna_damage **damage,
				  GCPtr gc, int n, xRectangle *r,
				  const BoxRec *extents, unsigned clipped)
{
	PixmapPtr pixmap = get_drawable_pixmap(drawable);
	struct sna *sna = to_sna_from_pixmap(pixmap);
	DDXPointRec origin = gc->patOrg;
	struct kgem_bo *tile = NULL;
	int16_t dx, dy;
	uint32_t br00, br13;

	DBG(("%s: upload (%d, %d), (%d, %d), origin (%d, %d), clipped=%d, alu=%d, opaque=%d\n", __FUNCTION__,
	     extents->x1, extents->y1,
	     extents->x2, extents->y2,
	     origin.x, origin.y,
	     clipped, gc->alu, gc->fillStyle == FillOpaqueStippled));

	if (((gc->stipple->drawable.width | gc->stipple->drawable.height) & ~31) == 0)
		return sna_poly_fill_rect_stippled_n_blt__imm(drawable,
							      bo, damage,
							      gc, n, r,
							      extents, clipped);

	get_drawable_deltas(drawable, pixmap, &dx, &dy);
	kgem_set_mode(&sna->kgem, KGEM_BLT, bo);

	br00 = XY_MONO_SRC_COPY | 3 << 20;
	br13 = bo->pitch;
	if (sna->kgem.gen >= 040 && bo->tiling) {
		br00 |= BLT_DST_TILED;
		br13 >>= 2;
	}
	br13 |= (gc->fillStyle == FillStippled) << 29;
	br13 |= blt_depth(drawable->depth) << 24;
	br13 |= copy_ROP[gc->alu] << 16;

	origin.x += dx + drawable->x;
	origin.y += dy + drawable->y;

	if (!clipped) {
		dx += drawable->x;
		dy += drawable->y;

		sna_damage_add_rectangles(damage, r, n, dx, dy);
		do {
			BoxRec box;

			box.x1 = r->x + dx;
			box.y1 = r->y + dy;
			box.x2 = box.x1 + r->width;
			box.y2 = box.y1 + r->height;

			sna_poly_fill_rect_stippled_n_box(sna, bo, &tile,
							  br00, br13, gc,
							  &box, &origin);
			r++;
		} while (--n);
	} else {
		RegionRec clip;

		region_set(&clip, extents);
		if (!region_maybe_clip(&clip, gc->pCompositeClip)) {
			DBG(("%s: all clipped\n", __FUNCTION__));
			return true;
		}

		if (clip.data == NULL) {
			DBG(("%s: clipped to extents ((%d, %d), (%d, %d))\n",
			     __FUNCTION__,
			     clip.extents.x1, clip.extents.y1,
			     clip.extents.x2, clip.extents.y2));
			do {
				BoxRec box;

				box.x1 = r->x + drawable->x;
				box.x2 = bound(box.x1, r->width);
				box.y1 = r->y + drawable->y;
				box.y2 = bound(box.y1, r->height);
				r++;

				DBG(("%s: box (%d, %d), (%d, %d)\n",
				     __FUNCTION__,
				     box.x1, box.y1, box.x2, box.y2));
				if (!box_intersect(&box, &clip.extents))
					continue;

				box.x1 += dx; box.x2 += dx;
				box.y1 += dy; box.y2 += dy;

				sna_poly_fill_rect_stippled_n_box(sna, bo, &tile,
								  br00, br13, gc,
								  &box, &origin);
			} while (--n);
		} else {
			const BoxRec * const clip_start = RegionBoxptr(&clip);
			const BoxRec * const clip_end = clip_start + clip.data->numRects;
			const BoxRec *c;

			DBG(("%s: clipped to boxes: start((%d, %d), (%d, %d)); end=((%d, %d), (%d, %d))\n", __FUNCTION__,
			     clip_start->x1, clip_start->y1,
			     clip_start->x2, clip_start->y2,
			     clip_end->x1, clip_end->y1,
			     clip_end->x2, clip_end->y2));
			do {
				BoxRec unclipped;

				unclipped.x1 = r->x + drawable->x;
				unclipped.x2 = bound(unclipped.x1, r->width);
				unclipped.y1 = r->y + drawable->y;
				unclipped.y2 = bound(unclipped.y1, r->height);
				r++;

				c = find_clip_box_for_y(clip_start,
							clip_end,
							unclipped.y1);
				while (c != clip_end) {
					BoxRec box;

					if (unclipped.y2 <= c->y1)
						break;

					box = unclipped;
					if (!box_intersect(&box, c++))
						continue;

					box.x1 += dx; box.x2 += dx;
					box.y1 += dy; box.y2 += dy;

					sna_poly_fill_rect_stippled_n_box(sna, bo, &tile,
									  br00, br13, gc,
									  &box, &origin);
				}
			} while (--n);
		}
	}

	assert_pixmap_damage(pixmap);
	if (tile)
		kgem_bo_destroy(&sna->kgem, tile);
	sna->blt_state.fill_bo = 0;
	return true;
}

static bool
sna_poly_fill_rect_stippled_blt(DrawablePtr drawable,
				struct kgem_bo *bo,
				struct sna_damage **damage,
				GCPtr gc, int n, xRectangle *rect,
				const BoxRec *extents, unsigned clipped)
{

	PixmapPtr stipple = gc->stipple;

	if (bo->tiling == I915_TILING_Y) {
		PixmapPtr pixmap = get_drawable_pixmap(drawable);

		DBG(("%s: converting bo from Y-tiling\n", __FUNCTION__));
		/* This is cheating, but only the gpu_bo can be tiled */
		assert(bo == __sna_pixmap_get_bo(pixmap));
		bo = sna_pixmap_change_tiling(pixmap, I915_TILING_X);
		if (bo == NULL) {
			DBG(("%s: fallback -- unable to change tiling\n",
			     __FUNCTION__));
			return false;
		}
	}

	if (!sna_drawable_move_to_cpu(&stipple->drawable, MOVE_READ))
		return false;

	DBG(("%s: origin (%d, %d), extents (stipple): (%d, %d), stipple size %dx%d\n",
	     __FUNCTION__, gc->patOrg.x, gc->patOrg.y,
	     extents->x2 - gc->patOrg.x - drawable->x,
	     extents->y2 - gc->patOrg.y - drawable->y,
	     stipple->drawable.width, stipple->drawable.height));

	if ((stipple->drawable.width | stipple->drawable.height) == 8)
		return sna_poly_fill_rect_stippled_8x8_blt(drawable, bo, damage,
							   gc, n, rect,
							   extents, clipped);

	if ((stipple->drawable.width | stipple->drawable.height) <= 0xc &&
	    is_power_of_two(stipple->drawable.width) &&
	    is_power_of_two(stipple->drawable.height))
		return sna_poly_fill_rect_stippled_nxm_blt(drawable, bo, damage,
							   gc, n, rect,
							   extents, clipped);

	if (extents->x1 - gc->patOrg.x - drawable->x >= 0 &&
	    extents->x2 - gc->patOrg.x - drawable->x <= stipple->drawable.width &&
	    extents->y1 - gc->patOrg.y - drawable->y >= 0 &&
	    extents->y2 - gc->patOrg.y - drawable->y <= stipple->drawable.height) {
		if (stipple->drawable.width <= 8 && stipple->drawable.height <= 8)
			return sna_poly_fill_rect_stippled_8x8_blt(drawable, bo, damage,
								   gc, n, rect,
								   extents, clipped);
		else
			return sna_poly_fill_rect_stippled_1_blt(drawable, bo, damage,
								 gc, n, rect,
								 extents, clipped);
	} else {
		return sna_poly_fill_rect_stippled_n_blt(drawable, bo, damage,
							 gc, n, rect,
							 extents, clipped);
	}
}

static unsigned
sna_poly_fill_rect_extents(DrawablePtr drawable, GCPtr gc,
			   int *_n, xRectangle **_r,
			   BoxPtr out)
{
	int n;
	xRectangle *r;
	Box32Rec box;
	bool clipped;

	if (*_n == 0)
		return 0;

	DBG(("%s: [0] = (%d, %d)x(%d, %d)\n",
	     __FUNCTION__, (*_r)->x, (*_r)->y, (*_r)->width, (*_r)->height));

	/* Remove any zero-size rectangles from the array */
	while (*_n && ((*_r)->width == 0 || (*_r)->height == 0))
		--*_n, ++*_r;

	if (*_n == 0)
		return 0;

	n = *_n;
	r = *_r;

	box.x1 = r->x;
	box.x2 = box.x1 + r->width;
	box.y1 = r->y;
	box.y2 = box.y1 + r->height;
	r++;

	while (--n) {
		if (r->width == 0 || r->height == 0)
			goto slow;

		box32_add_rect(&box, r++);
	}
	goto done;
slow:
	{
		xRectangle *rr = r;
		do {
			do {
				--*_n, r++;
			} while (--n && (r->width == 0 || r->height == 0));
			while (n && r->width && r->height) {
				box32_add_rect(&box, r);
				*rr++ = *r++;
				n--;
			}
		} while (n);
	}
done:

	clipped = box32_trim_and_translate(&box, drawable, gc);
	if (!box32_to_box16(&box, out))
		return 0;

	return 1 | clipped << 1;
}

static void
sna_poly_fill_rect(DrawablePtr draw, GCPtr gc, int n, xRectangle *rect)
{
	PixmapPtr pixmap = get_drawable_pixmap(draw);
	struct sna *sna = to_sna_from_pixmap(pixmap);
	struct sna_pixmap *priv = sna_pixmap(pixmap);
	struct sna_damage **damage;
	struct kgem_bo *bo;
	RegionRec region;
	unsigned flags, hint;
	uint32_t color;

	DBG(("%s(n=%d, PlaneMask: %lx (solid %d), solid fill: %d [style=%d, tileIsPixel=%d], alu=%d)\n", __FUNCTION__,
	     n, gc->planemask, !!PM_IS_SOLID(draw, gc->planemask),
	     (gc->fillStyle == FillSolid ||
	      (gc->fillStyle == FillTiled && gc->tileIsPixel)),
	     gc->fillStyle, gc->tileIsPixel,
	     gc->alu));

	flags = sna_poly_fill_rect_extents(draw, gc, &n, &rect, &region.extents);
	if (flags == 0) {
		DBG(("%s, nothing to do\n", __FUNCTION__));
		return;
	}

	DBG(("%s: extents(%d, %d), (%d, %d), flags=%x\n", __FUNCTION__,
	     region.extents.x1, region.extents.y1,
	     region.extents.x2, region.extents.y2,
	     flags));

	if (FORCE_FALLBACK || !ACCEL_POLY_FILL_RECT) {
		DBG(("%s: fallback forced\n", __FUNCTION__));
		goto fallback;
	}

	if (priv == NULL) {
		DBG(("%s: fallback -- unattached\n", __FUNCTION__));
		goto fallback;
	}

	if (wedged(sna)) {
		DBG(("%s: fallback -- wedged\n", __FUNCTION__));
		goto fallback;
	}

	if (!PM_IS_SOLID(draw, gc->planemask)) {
		DBG(("%s: fallback -- planemask=0x%lx (not-solid)\n",
		     __FUNCTION__, gc->planemask));
		goto fallback;
	}

	/* Clear the cpu damage so that we refresh the GPU status of the
	 * pixmap upon a redraw after a period of inactivity.
	 */
	hint = PREFER_GPU;
	if (n == 1 && gc->fillStyle != FillStippled && alu_overwrites(gc->alu)) {
		int16_t dx, dy;

		region.data = NULL;

		if (get_drawable_deltas(draw, pixmap, &dx, &dy)) {
			DBG(("%s: delta=(%d, %d)\n", __FUNCTION__, dx, dy));
			RegionTranslate(&region, dx, dy);
		}

		if ((flags & 2) == 0) {
			hint |= IGNORE_DAMAGE;
			if (region_subsumes_drawable(&region, &pixmap->drawable)) {
				discard_cpu_damage(sna, priv);
				hint |= REPLACES;
			} else {
				if (priv->cpu_damage &&
				    region_subsumes_damage(&region, priv->cpu_damage))
					discard_cpu_damage(sna, priv);
			}
		}
		if (priv->cpu_damage == NULL) {
			if (priv->gpu_bo &&
			    (hint & REPLACES ||
			     box_covers_pixmap(pixmap, &region.extents) ||
			     box_inplace(pixmap, &region.extents))) {
				DBG(("%s: promoting to full GPU\n",
				     __FUNCTION__));
				assert(priv->gpu_bo->proxy == NULL);
				sna_damage_all(&priv->gpu_damage, pixmap);
			}
			DBG(("%s: dropping last-cpu hint\n", __FUNCTION__));
			priv->cpu = false;
		}

		if (dx | dy)
			RegionTranslate(&region, -dx, -dy);
	}

	/* If the source is already on the GPU, keep the operation on the GPU */
	if (gc->fillStyle == FillTiled && !gc->tileIsPixel &&
	    sna_pixmap_is_gpu(gc->tile.pixmap)) {
		DBG(("%s: source is already on the gpu\n", __FUNCTION__));
		hint |= FORCE_GPU;
	}

	bo = sna_drawable_use_bo(draw, hint, &region.extents, &damage);
	if (bo == NULL) {
		DBG(("%s: not using GPU, hint=%x\n", __FUNCTION__, hint));
		goto fallback;
	}
	if (hint & REPLACES && UNDO)
		kgem_bo_pair_undo(&sna->kgem, priv->gpu_bo, priv->cpu_bo);

	if (gc_is_solid(gc, &color)) {
		DBG(("%s: solid fill [%08x], testing for blt\n",
		     __FUNCTION__, color));

		if (sna_poly_fill_rect_blt(draw,
					   bo, damage,
					   gc, color, n, rect,
					   &region.extents, flags & 2))
			return;
	} else if (gc->fillStyle == FillTiled) {
		DBG(("%s: tiled fill, testing for blt\n", __FUNCTION__));

		if (sna_poly_fill_rect_tiled_blt(draw, bo, damage,
						 gc, n, rect,
						 &region.extents, flags & 2))
			return;
	} else {
		DBG(("%s: stippled fill, testing for blt\n", __FUNCTION__));

		if (sna_poly_fill_rect_stippled_blt(draw, bo, damage,
						    gc, n, rect,
						    &region.extents, flags & 2))
			return;
	}

fallback:
	DBG(("%s: fallback (%d, %d), (%d, %d)\n", __FUNCTION__,
	     region.extents.x1, region.extents.y1,
	     region.extents.x2, region.extents.y2));
	region.data = NULL;
	if (!region_maybe_clip(&region, gc->pCompositeClip)) {
		DBG(("%s: nothing to do, all clipped\n", __FUNCTION__));
		return;
	}

	if (!sna_gc_move_to_cpu(gc, draw, &region))
		goto out;
	if (!sna_drawable_move_region_to_cpu(draw, &region,
					     drawable_gc_flags(draw, gc, n > 1)))
		goto out;

	if (sigtrap_get() == 0) {
		DBG(("%s: fallback - fbPolyFillRect\n", __FUNCTION__));
		fbPolyFillRect(draw, gc, n, rect);
		FALLBACK_FLUSH(draw);
		sigtrap_put();
	}
out:
	sna_gc_move_to_gpu(gc);
	RegionUninit(&region);
}

static void
sna_poly_fill_rect__gpu(DrawablePtr draw, GCPtr gc, int n, xRectangle *r)
{
	struct sna_fill_spans *data = sna_gc(gc)->priv;
	uint32_t color;

	DBG(("%s(n=%d, PlaneMask: %lx (solid %d), solid fill: %d [style=%d, tileIsPixel=%d], alu=%d)\n", __FUNCTION__,
	     n, gc->planemask, !!PM_IS_SOLID(draw, gc->planemask),
	     (gc->fillStyle == FillSolid ||
	      (gc->fillStyle == FillTiled && gc->tileIsPixel)),
	     gc->fillStyle, gc->tileIsPixel,
	     gc->alu));

	assert(PM_IS_SOLID(draw, gc->planemask));
	if (n == 0)
		return;

	/* The mi routines do not attempt to keep the spans it generates
	 * within the clip, so we must run them through the clipper.
	 */

	if (gc_is_solid(gc, &color)) {
		(void)sna_poly_fill_rect_blt(draw,
					     data->bo, data->damage,
					     gc, color, n, r,
					     &data->region.extents, true);
	} else if (gc->fillStyle == FillTiled) {
		(void)sna_poly_fill_rect_tiled_blt(draw,
						   data->bo, data->damage,
						   gc, n, r,
						   &data->region.extents, true);
	} else {
		(void)sna_poly_fill_rect_stippled_blt(draw,
						    data->bo, data->damage,
						    gc, n, r,
						    &data->region.extents, true);
	}
}

static void
sna_poly_fill_arc(DrawablePtr draw, GCPtr gc, int n, xArc *arc)
{
	struct sna_fill_spans data;
	struct sna_pixmap *priv;

	DBG(("%s(n=%d, PlaneMask: %lx (solid %d), solid fill: %d [style=%d, tileIsPixel=%d], alu=%d)\n", __FUNCTION__,
	     n, gc->planemask, !!PM_IS_SOLID(draw, gc->planemask),
	     (gc->fillStyle == FillSolid ||
	      (gc->fillStyle == FillTiled && gc->tileIsPixel)),
	     gc->fillStyle, gc->tileIsPixel,
	     gc->alu));

	data.flags = sna_poly_arc_extents(draw, gc, n, arc,
					  &data.region.extents);
	if (data.flags == 0)
		return;

	DBG(("%s: extents(%d, %d), (%d, %d), flags=%x\n", __FUNCTION__,
	     data.region.extents.x1, data.region.extents.y1,
	     data.region.extents.x2, data.region.extents.y2,
	     data.flags));

	data.region.data = NULL;

	if (FORCE_FALLBACK)
		goto fallback;

	if (!ACCEL_POLY_FILL_ARC)
		goto fallback;

	data.pixmap = get_drawable_pixmap(draw);
	data.sna = to_sna_from_pixmap(data.pixmap);
	priv = sna_pixmap(data.pixmap);
	if (priv == NULL) {
		DBG(("%s: fallback -- unattached\n", __FUNCTION__));
		goto fallback;
	}

	if (wedged(data.sna)) {
		DBG(("%s: fallback -- wedged\n", __FUNCTION__));
		goto fallback;
	}

	if (!PM_IS_SOLID(draw, gc->planemask))
		goto fallback;

	if ((data.bo = sna_drawable_use_bo(draw, PREFER_GPU,
					   &data.region.extents,
					   &data.damage))) {
		uint32_t color;

		get_drawable_deltas(draw, data.pixmap, &data.dx, &data.dy);
		sna_gc(gc)->priv = &data;

		if (gc_is_solid(gc, &color)) {
			struct sna_fill_op fill;

			if (!sna_fill_init_blt(&fill,
					       data.sna, data.pixmap,
					       data.bo, gc->alu, color,
					       FILL_SPANS))
				goto fallback;

			data.op = &fill;

			if ((data.flags & 2) == 0) {
				if (data.dx | data.dy)
					sna_gc_ops__tmp.FillSpans = sna_fill_spans__fill_offset;
				else
					sna_gc_ops__tmp.FillSpans = sna_fill_spans__fill;
			} else {
				if (!region_maybe_clip(&data.region,
						       gc->pCompositeClip))
					return;

				if (region_is_singular(&data.region))
					sna_gc_ops__tmp.FillSpans = sna_fill_spans__fill_clip_extents;
				else
					sna_gc_ops__tmp.FillSpans = sna_fill_spans__fill_clip_boxes;
			}
			assert(gc->miTranslate);
			gc->ops = &sna_gc_ops__tmp;

			miPolyFillArc(draw, gc, n, arc);
			fill.done(data.sna, &fill);
		} else {
			sna_gc_ops__tmp.FillSpans = sna_fill_spans__gpu;
			gc->ops = &sna_gc_ops__tmp;

			miPolyFillArc(draw, gc, n, arc);
		}

		gc->ops = (GCOps *)&sna_gc_ops;
		if (data.damage) {
			if (data.dx | data.dy)
				pixman_region_translate(&data.region, data.dx, data.dy);
			assert_pixmap_contains_box(data.pixmap, &data.region.extents);
			sna_damage_add(data.damage, &data.region);
		}
		assert_pixmap_damage(data.pixmap);
		RegionUninit(&data.region);
		return;
	}

fallback:
	DBG(("%s: fallback (%d, %d), (%d, %d)\n", __FUNCTION__,
	     data.region.extents.x1, data.region.extents.y1,
	     data.region.extents.x2, data.region.extents.y2));
	if (!region_maybe_clip(&data.region, gc->pCompositeClip)) {
		DBG(("%s: nothing to do, all clipped\n", __FUNCTION__));
		return;
	}

	if (!sna_gc_move_to_cpu(gc, draw, &data.region))
		goto out;
	if (!sna_drawable_move_region_to_cpu(draw, &data.region,
					     drawable_gc_flags(draw, gc, true)))
		goto out;

	if (sigtrap_get() == 0) {
		DBG(("%s: fallback -- miPolyFillArc -> sna_fill_spans__cpu\n",
		     __FUNCTION__));
		miPolyFillArc(draw, gc, n, arc);
		sigtrap_put();
	}
out:
	sna_gc_move_to_gpu(gc);
	RegionUninit(&data.region);
}

struct sna_font {
	CharInfoRec glyphs8[256];
	CharInfoRec *glyphs16[256];
};
#define GLYPH_INVALID (void *)1
#define GLYPH_EMPTY (void *)2

static Bool
sna_realize_font(ScreenPtr screen, FontPtr font)
{
	struct sna_font *priv;

	DBG(("%s (key=%d)\n", __FUNCTION__, sna_font_key));

	priv = calloc(1, sizeof(struct sna_font));
	if (priv == NULL)
		return FALSE;

	if (!FontSetPrivate(font, sna_font_key, priv)) {
		free(priv);
		return FALSE;
	}

	return TRUE;
}

static Bool
sna_unrealize_font(ScreenPtr screen, FontPtr font)
{
	struct sna_font *priv = FontGetPrivate(font, sna_font_key);
	int i, j;

	DBG(("%s (key=%d)\n", __FUNCTION__, sna_font_key));

	if (priv == NULL)
		return TRUE;

	for (i = 0; i < 256; i++) {
		if ((uintptr_t)priv->glyphs8[i].bits & ~3)
			free(priv->glyphs8[i].bits);
	}
	for (j = 0; j < 256; j++) {
		if (priv->glyphs16[j] == NULL)
			continue;

		for (i = 0; i < 256; i++) {
			if ((uintptr_t)priv->glyphs16[j][i].bits & ~3)
				free(priv->glyphs16[j][i].bits);
		}
		free(priv->glyphs16[j]);
	}
	free(priv);

	FontSetPrivate(font, sna_font_key, NULL);
	return TRUE;
}

static bool
sna_glyph_blt(DrawablePtr drawable, GCPtr gc,
	      int _x, int _y, unsigned int _n,
	      CharInfoPtr *_info,
	      RegionRec *clip,
	      uint32_t fg, uint32_t bg,
	      bool transparent)
{
	PixmapPtr pixmap = get_drawable_pixmap(drawable);
	struct sna *sna = to_sna_from_pixmap(pixmap);
	struct kgem_bo *bo;
	struct sna_damage **damage;
	const BoxRec *extents, *last_extents;
	uint32_t *b;
	int16_t dx, dy;
	uint32_t br00;
	uint16_t unwind_batch, unwind_reloc;
	unsigned hint;

	uint8_t rop = transparent ? copy_ROP[gc->alu] : ROP_S;

	DBG(("%s (%d, %d) x %d, fg=%08x, bg=%08x alu=%02x\n",
	     __FUNCTION__, _x, _y, _n, fg, bg, rop));

	if (wedged(sna)) {
		DBG(("%s: fallback -- wedged\n", __FUNCTION__));
		return false;
	}

	if (!transparent && clip->data == NULL)
		hint = PREFER_GPU | IGNORE_DAMAGE;
	else
		hint = PREFER_GPU;

	bo = sna_drawable_use_bo(drawable, hint, &clip->extents, &damage);
	if (bo == NULL)
		return false;

	if (bo->tiling == I915_TILING_Y) {
		DBG(("%s: converting bo from Y-tiling\n", __FUNCTION__));
		assert(bo == __sna_pixmap_get_bo(pixmap));
		bo = sna_pixmap_change_tiling(pixmap, I915_TILING_X);
		if (bo == NULL) {
			DBG(("%s: fallback -- unable to change tiling\n",
			     __FUNCTION__));
			return false;
		}
	}

	if (get_drawable_deltas(drawable, pixmap, &dx, &dy))
		RegionTranslate(clip, dx, dy);
	_x += drawable->x + dx;
	_y += drawable->y + dy;

	extents = region_rects(clip);
	last_extents = extents + region_num_rects(clip);

	if (!transparent) { /* emulate miImageGlyphBlt */
		if (!sna_blt_fill_boxes(sna, GXcopy,
					bo, drawable->bitsPerPixel,
					bg, extents, last_extents - extents)) {
			RegionTranslate(clip, -dx, -dy);
			return false;
		}
	}

	kgem_set_mode(&sna->kgem, KGEM_BLT, bo);
	if (!kgem_check_batch(&sna->kgem, 20) ||
	    !kgem_check_bo_fenced(&sna->kgem, bo) ||
	    !kgem_check_reloc(&sna->kgem, 1)) {
		kgem_submit(&sna->kgem);
		if (!kgem_check_bo_fenced(&sna->kgem, bo)) {
			RegionTranslate(clip, -dx, -dy);
			return false;
		}
		_kgem_set_mode(&sna->kgem, KGEM_BLT);
	}

	DBG(("%s: glyph clip box (%d, %d), (%d, %d)\n",
	     __FUNCTION__,
	     extents->x1, extents->y1,
	     extents->x2, extents->y2));

	unwind_batch = sna->kgem.nbatch;
	unwind_reloc = sna->kgem.nreloc;

	assert(sna->kgem.mode == KGEM_BLT);
	b = sna->kgem.batch + sna->kgem.nbatch;
	if (sna->kgem.gen >= 0100) {
		b[0] = XY_SETUP_BLT | 3 << 20 | 8;
		b[1] = bo->pitch;
		if (sna->kgem.gen >= 040 && bo->tiling) {
			b[0] |= BLT_DST_TILED;
			b[1] >>= 2;
		}
		b[1] |= 1 << 30 | transparent << 29 | blt_depth(drawable->depth) << 24 | rop << 16;
		b[2] = extents->y1 << 16 | extents->x1;
		b[3] = extents->y2 << 16 | extents->x2;
		*(uint64_t *)(b+4) =
			kgem_add_reloc64(&sna->kgem, sna->kgem.nbatch + 4, bo,
					 I915_GEM_DOMAIN_RENDER << 16 |
					 I915_GEM_DOMAIN_RENDER |
					 KGEM_RELOC_FENCED,
					 0);
		b[6] = bg;
		b[7] = fg;
		b[8] = 0;
		b[9] = 0;
		sna->kgem.nbatch += 10;
	} else {
		b[0] = XY_SETUP_BLT | 3 << 20 | 6;
		b[1] = bo->pitch;
		if (sna->kgem.gen >= 040 && bo->tiling) {
			b[0] |= BLT_DST_TILED;
			b[1] >>= 2;
		}
		b[1] |= 1 << 30 | transparent << 29 | blt_depth(drawable->depth) << 24 | rop << 16;
		b[2] = extents->y1 << 16 | extents->x1;
		b[3] = extents->y2 << 16 | extents->x2;
		b[4] = kgem_add_reloc(&sna->kgem, sna->kgem.nbatch + 4, bo,
				      I915_GEM_DOMAIN_RENDER << 16 |
				      I915_GEM_DOMAIN_RENDER |
				      KGEM_RELOC_FENCED,
				      0);
		b[5] = bg;
		b[6] = fg;
		b[7] = 0;
		sna->kgem.nbatch += 8;
	}

	br00 = XY_TEXT_IMMEDIATE_BLT;
	if (bo->tiling && sna->kgem.gen >= 040)
		br00 |= BLT_DST_TILED;

	do {
		CharInfoPtr *info = _info;
		int x = _x, y = _y, n = _n;

		do {
			CharInfoPtr c = *info++;
			int w = GLYPHWIDTHPIXELS(c);
			int h = GLYPHHEIGHTPIXELS(c);
			int w8 = (w + 7) >> 3;
			int x1, y1, len;

			if (c->bits == GLYPH_EMPTY)
				goto skip;

			len = (w8 * h + 7) >> 3 << 1;
			x1 = x + c->metrics.leftSideBearing;
			y1 = y - c->metrics.ascent;

			DBG(("%s glyph: (%d, %d) -> (%d, %d) x (%d[%d], %d), len=%d\n" ,__FUNCTION__,
			     x,y, x1, y1, w, w8, h, len));

			if (x1 >= extents->x2 || y1 >= extents->y2)
				goto skip;
			if (x1 + w <= extents->x1 || y1 + h <= extents->y1)
				goto skip;

			assert(len > 0);
			if (!kgem_check_batch(&sna->kgem, 3+len)) {
				_kgem_submit(&sna->kgem);
				_kgem_set_mode(&sna->kgem, KGEM_BLT);

				DBG(("%s: new batch, glyph clip box (%d, %d), (%d, %d)\n",
				     __FUNCTION__,
				     extents->x1, extents->y1,
				     extents->x2, extents->y2));

				unwind_batch = sna->kgem.nbatch;
				unwind_reloc = sna->kgem.nreloc;

				assert(sna->kgem.mode == KGEM_BLT);
				b = sna->kgem.batch + sna->kgem.nbatch;
				if (sna->kgem.gen >= 0100) {
					b[0] = XY_SETUP_BLT | 3 << 20 | 8;
					b[1] = bo->pitch;
					if (bo->tiling) {
						b[0] |= BLT_DST_TILED;
						b[1] >>= 2;
					}
					b[1] |= 1 << 30 | transparent << 29 | blt_depth(drawable->depth) << 24 | rop << 16;
					b[2] = extents->y1 << 16 | extents->x1;
					b[3] = extents->y2 << 16 | extents->x2;
					*(uint64_t *)(b+4) =
						kgem_add_reloc64(&sna->kgem, sna->kgem.nbatch + 4, bo,
								 I915_GEM_DOMAIN_RENDER << 16 |
								 I915_GEM_DOMAIN_RENDER |
								 KGEM_RELOC_FENCED,
								 0);
					b[6] = bg;
					b[7] = fg;
					b[8] = 0;
					b[9] = 0;
					sna->kgem.nbatch += 10;
				} else {
					b[0] = XY_SETUP_BLT | 3 << 20 | 6;
					b[1] = bo->pitch;
					if (sna->kgem.gen >= 040 && bo->tiling) {
						b[0] |= BLT_DST_TILED;
						b[1] >>= 2;
					}
					b[1] |= 1 << 30 | transparent << 29 | blt_depth(drawable->depth) << 24 | rop << 16;
					b[2] = extents->y1 << 16 | extents->x1;
					b[3] = extents->y2 << 16 | extents->x2;
					b[4] = kgem_add_reloc(&sna->kgem, sna->kgem.nbatch + 4, bo,
							      I915_GEM_DOMAIN_RENDER << 16 |
							      I915_GEM_DOMAIN_RENDER |
							      KGEM_RELOC_FENCED,
							      0);
					b[5] = bg;
					b[6] = fg;
					b[7] = 0;
					sna->kgem.nbatch += 8;
				}
			}

			assert(sna->kgem.mode == KGEM_BLT);
			b = sna->kgem.batch + sna->kgem.nbatch;
			sna->kgem.nbatch += 3 + len;

			b[0] = br00 | (1 + len);
			b[1] = (uint16_t)y1 << 16 | (uint16_t)x1;
			b[2] = (uint16_t)(y1+h) << 16 | (uint16_t)(x1+w);
			{
				uint64_t *src = (uint64_t *)c->bits;
				uint64_t *dst = (uint64_t *)(b + 3);
				do  {
					*dst++ = *src++;
					len -= 2;
				} while (len);
			}

			if (damage) {
				BoxRec r;

				r.x1 = x1;
				r.y1 = y1;
				r.x2 = x1 + w;
				r.y2 = y1 + h;
				if (box_intersect(&r, extents))
					sna_damage_add_box(damage, &r);
			}
skip:
			x += c->metrics.characterWidth;
		} while (--n);

		if (++extents == last_extents)
			break;

		if (kgem_check_batch(&sna->kgem, 3)) {
			assert(sna->kgem.mode == KGEM_BLT);
			b = sna->kgem.batch + sna->kgem.nbatch;
			sna->kgem.nbatch += 3;

			DBG(("%s: glyph clip box (%d, %d), (%d, %d)\n",
			     __FUNCTION__,
			     extents->x1, extents->y1,
			     extents->x2, extents->y2));

			b[0] = XY_SETUP_CLIP;
			b[1] = extents->y1 << 16 | extents->x1;
			b[2] = extents->y2 << 16 | extents->x2;
		}
	} while (1);

	if (sna->kgem.nbatch == unwind_batch + (sna->kgem.gen >= 0100 ? 10 : 8)) {
		sna->kgem.nbatch = unwind_batch;
		sna->kgem.nreloc = unwind_reloc;
		if (sna->kgem.nbatch == 0)
			kgem_bo_undo(&sna->kgem, bo);
	}

	assert_pixmap_damage(pixmap);
	sna->blt_state.fill_bo = 0;
	return true;
}

static void
sna_glyph_extents(FontPtr font,
		  CharInfoPtr *info,
		  unsigned long count,
		  ExtentInfoRec *extents)
{
	extents->drawDirection = font->info.drawDirection;
	extents->fontAscent = font->info.fontAscent;
	extents->fontDescent = font->info.fontDescent;

	extents->overallAscent = info[0]->metrics.ascent;
	extents->overallDescent = info[0]->metrics.descent;
	extents->overallLeft = info[0]->metrics.leftSideBearing;
	extents->overallRight = info[0]->metrics.rightSideBearing;
	extents->overallWidth = info[0]->metrics.characterWidth;

	while (--count) {
		CharInfoPtr p =*++info;
		int v;

		if (p->metrics.ascent > extents->overallAscent)
			extents->overallAscent = p->metrics.ascent;
		if (p->metrics.descent > extents->overallDescent)
			extents->overallDescent = p->metrics.descent;

		v = extents->overallWidth + p->metrics.leftSideBearing;
		if (v < extents->overallLeft)
			extents->overallLeft = v;

		v = extents->overallWidth + p->metrics.rightSideBearing;
		if (v > extents->overallRight)
			extents->overallRight = v;

		extents->overallWidth += p->metrics.characterWidth;
	}
}

static bool sna_set_glyph(CharInfoPtr in, CharInfoPtr out)
{
	int w = GLYPHWIDTHPIXELS(in);
	int h = GLYPHHEIGHTPIXELS(in);
	int stride = GLYPHWIDTHBYTESPADDED(in);
	uint8_t *dst, *src;
	int clear = 1;

	out->metrics = in->metrics;

	/* Skip empty glyphs */
	if (w == 0 || h == 0 || ((w|h) == 1 && (in->bits[0] & 1) == 0)) {
		out->bits = GLYPH_EMPTY;
		return true;
	}

	w = (w + 7) >> 3;

	out->bits = malloc((w*h + 7) & ~7);
	if (out->bits == NULL)
		return false;

	VG(memset(out->bits, 0, (w*h + 7) & ~7));
	src = (uint8_t *)in->bits;
	dst = (uint8_t *)out->bits;
	stride -= w;
	do {
		int i = w;
		do {
			clear &= *src == 0;
			*dst++ = byte_reverse(*src++);
		} while (--i);
		src += stride;
	} while (--h);

	if (clear) {
		free(out->bits);
		out->bits = GLYPH_EMPTY;
	}

	return true;
}

inline static bool sna_get_glyph8(FontPtr font, struct sna_font *priv,
				  uint8_t g, CharInfoPtr *out)
{
	unsigned long n;
	CharInfoPtr p, ret;

	p = &priv->glyphs8[g];
	if (p->bits) {
		*out = p;
		return p->bits != GLYPH_INVALID;
	}

	font->get_glyphs(font, 1, &g, Linear8Bit, &n, &ret);
	if (n == 0) {
		p->bits = GLYPH_INVALID;
		return false;
	}

	return sna_set_glyph(ret, *out = p);
}

inline static bool sna_get_glyph16(FontPtr font, struct sna_font *priv,
				   uint16_t g, CharInfoPtr *out)
{
	unsigned long n;
	CharInfoPtr page, p, ret;

	page = priv->glyphs16[g>>8];
	if (page == NULL)
		page = priv->glyphs16[g>>8] = calloc(256, sizeof(CharInfoRec));

	p = &page[g&0xff];
	if (p->bits) {
		*out = p;
		return p->bits != GLYPH_INVALID;
	}

	font->get_glyphs(font, 1, (unsigned char *)&g,
			 FONTLASTROW(font) ? TwoD16Bit : Linear16Bit,
			 &n, &ret);
	if (n == 0) {
		p->bits = GLYPH_INVALID;
		return false;
	}

	return sna_set_glyph(ret, *out = p);
}

static inline bool sna_font_too_large(FontPtr font)
{
	int top = max(FONTMAXBOUNDS(font, ascent), FONTASCENT(font));
	int bot = max(FONTMAXBOUNDS(font, descent), FONTDESCENT(font));
	int width = max(FONTMAXBOUNDS(font, characterWidth), -FONTMINBOUNDS(font, characterWidth));
	DBG(("%s? (%d + %d) x %d: %d > 124\n", __FUNCTION__,
	     top, bot, width, (top + bot) * (width + 7)/8));
	return (top + bot) * (width + 7)/8 > 124;
}

static int
sna_poly_text8(DrawablePtr drawable, GCPtr gc,
	       int x, int y,
	       int count, char *chars)
{
	struct sna_font *priv = gc->font->devPrivates[sna_font_key];
	CharInfoPtr info[255];
	ExtentInfoRec extents;
	RegionRec region;
	long unsigned i, n;
	uint32_t fg;

	for (i = n = 0; i < count; i++) {
		if (sna_get_glyph8(gc->font, priv, chars[i], &info[n]))
			n++;
	}
	if (n == 0)
		return x;

	sna_glyph_extents(gc->font, info, n, &extents);
	region.extents.x1 = x + extents.overallLeft;
	region.extents.y1 = y - extents.overallAscent;
	region.extents.x2 = x + extents.overallRight;
	region.extents.y2 = y + extents.overallDescent;

	translate_box(&region.extents, drawable);
	clip_box(&region.extents, gc);
	if (box_empty(&region.extents))
		return x + extents.overallRight;

	region.data = NULL;
	if (!region_maybe_clip(&region, gc->pCompositeClip))
		return x + extents.overallRight;

	if (FORCE_FALLBACK)
		goto fallback;

	if (!ACCEL_POLY_TEXT8)
		goto fallback;

	if (sna_font_too_large(gc->font))
		goto fallback;

	if (!PM_IS_SOLID(drawable, gc->planemask))
		goto fallback;

	if (!gc_is_solid(gc, &fg))
		goto fallback;

	if (!sna_glyph_blt(drawable, gc, x, y, n, info, &region, fg, -1, true)) {
fallback:
		DBG(("%s: fallback\n", __FUNCTION__));
		gc->font->get_glyphs(gc->font, count, (unsigned char *)chars,
				     Linear8Bit, &n, info);

		if (!sna_gc_move_to_cpu(gc, drawable, &region))
			goto out;
		if (!sna_drawable_move_region_to_cpu(drawable, &region,
						     MOVE_READ | MOVE_WRITE))
			goto out;

		if (sigtrap_get() == 0) {
			DBG(("%s: fallback -- fbPolyGlyphBlt\n", __FUNCTION__));
			fbPolyGlyphBlt(drawable, gc, x, y, n,
				       info, FONTGLYPHS(gc->font));
			FALLBACK_FLUSH(drawable);
			sigtrap_put();
		}
out:
		sna_gc_move_to_gpu(gc);
	}
	RegionUninit(&region);
	return x + extents.overallRight;
}

static int
sna_poly_text16(DrawablePtr drawable, GCPtr gc,
		int x, int y,
		int count, unsigned short *chars)
{
	struct sna_font *priv = gc->font->devPrivates[sna_font_key];
	CharInfoPtr info[255];
	ExtentInfoRec extents;
	RegionRec region;
	long unsigned i, n;
	uint32_t fg;

	for (i = n = 0; i < count; i++) {
		if (sna_get_glyph16(gc->font, priv, chars[i], &info[n]))
			n++;
	}
	if (n == 0)
		return x;

	sna_glyph_extents(gc->font, info, n, &extents);
	region.extents.x1 = x + extents.overallLeft;
	region.extents.y1 = y - extents.overallAscent;
	region.extents.x2 = x + extents.overallRight;
	region.extents.y2 = y + extents.overallDescent;

	translate_box(&region.extents, drawable);
	clip_box(&region.extents, gc);
	if (box_empty(&region.extents))
		return x + extents.overallRight;

	region.data = NULL;
	if (!region_maybe_clip(&region, gc->pCompositeClip))
		return x + extents.overallRight;

	if (FORCE_FALLBACK)
		goto fallback;

	if (!ACCEL_POLY_TEXT16)
		goto fallback;

	if (sna_font_too_large(gc->font))
		goto fallback;

	if (!PM_IS_SOLID(drawable, gc->planemask))
		goto fallback;

	if (!gc_is_solid(gc, &fg))
		goto fallback;

	if (!sna_glyph_blt(drawable, gc, x, y, n, info, &region, fg, -1, true)) {
fallback:
		DBG(("%s: fallback\n", __FUNCTION__));
		gc->font->get_glyphs(gc->font, count, (unsigned char *)chars,
				     FONTLASTROW(gc->font) ? TwoD16Bit : Linear16Bit,
				     &n, info);

		if (!sna_gc_move_to_cpu(gc, drawable, &region))
			goto out;
		if (!sna_drawable_move_region_to_cpu(drawable, &region,
						     MOVE_READ | MOVE_WRITE))
			goto out;

		if (sigtrap_get() == 0) {
			DBG(("%s: fallback -- fbPolyGlyphBlt\n", __FUNCTION__));
			fbPolyGlyphBlt(drawable, gc, x, y, n,
				       info, FONTGLYPHS(gc->font));
			FALLBACK_FLUSH(drawable);
			sigtrap_put();
		}
out:
		sna_gc_move_to_gpu(gc);
	}
	RegionUninit(&region);
	return x + extents.overallRight;
}

static void
sna_image_text8(DrawablePtr drawable, GCPtr gc,
		int x, int y,
		int count, char *chars)
{
	struct sna_font *priv = gc->font->devPrivates[sna_font_key];
	CharInfoPtr info[255];
	ExtentInfoRec extents;
	RegionRec region;
	long unsigned i, n;

	for (i = n = 0; i < count; i++) {
		if (sna_get_glyph8(gc->font, priv, chars[i], &info[n]))
			n++;
	}
	if (n == 0)
		return;

	sna_glyph_extents(gc->font, info, n, &extents);
	region.extents.x1 = x + MIN(0, extents.overallLeft);
	region.extents.y1 = y - extents.fontAscent;
	region.extents.x2 = x + MAX(extents.overallWidth, extents.overallRight);
	region.extents.y2 = y + extents.fontDescent;

	DBG(("%s: count=%ld/%d, extents=(left=%d, right=%d, width=%d, ascent=%d, descent=%d), box=(%d, %d), (%d, %d)\n",
	     __FUNCTION__, n, count,
	     extents.overallLeft, extents.overallRight, extents.overallWidth,
	     extents.fontAscent, extents.fontDescent,
	     region.extents.x1, region.extents.y1,
	     region.extents.x2, region.extents.y2));

	translate_box(&region.extents, drawable);
	clip_box(&region.extents, gc);
	if (box_empty(&region.extents))
		return;

	region.data = NULL;
	if (!region_maybe_clip(&region, gc->pCompositeClip))
		return;

	DBG(("%s: clipped extents (%d, %d), (%d, %d)\n",
	     __FUNCTION__,
	     region.extents.x1, region.extents.y1,
	     region.extents.x2, region.extents.y2));

	if (FORCE_FALLBACK)
		goto fallback;

	if (!ACCEL_IMAGE_TEXT8)
		goto fallback;

	if (sna_font_too_large(gc->font))
		goto fallback;

	if (!PM_IS_SOLID(drawable, gc->planemask))
		goto fallback;

	if (!sna_glyph_blt(drawable, gc, x, y, n, info, &region,
			   gc->fgPixel, gc->bgPixel, false)) {
fallback:
		DBG(("%s: fallback\n", __FUNCTION__));
		gc->font->get_glyphs(gc->font, count, (unsigned char *)chars,
				     Linear8Bit, &n, info);

		if (!sna_gc_move_to_cpu(gc, drawable, &region))
			goto out;
		if (!sna_drawable_move_region_to_cpu(drawable, &region, MOVE_WRITE))
			goto out;

		if (sigtrap_get() == 0) {
			DBG(("%s: fallback -- fbImageGlyphBlt\n", __FUNCTION__));
			fbImageGlyphBlt(drawable, gc, x, y, n,
					info, FONTGLYPHS(gc->font));
			FALLBACK_FLUSH(drawable);
			sigtrap_put();
		}
out:
		sna_gc_move_to_gpu(gc);
	}
	RegionUninit(&region);
}

static void
sna_image_text16(DrawablePtr drawable, GCPtr gc,
		int x, int y,
		int count, unsigned short *chars)
{
	struct sna_font *priv = gc->font->devPrivates[sna_font_key];
	CharInfoPtr info[255];
	ExtentInfoRec extents;
	RegionRec region;
	long unsigned i, n;

	for (i = n = 0; i < count; i++) {
		if (sna_get_glyph16(gc->font, priv, chars[i], &info[n]))
			n++;
	}
	if (n == 0)
		return;

	sna_glyph_extents(gc->font, info, n, &extents);
	region.extents.x1 = x + MIN(0, extents.overallLeft);
	region.extents.y1 = y - extents.fontAscent;
	region.extents.x2 = x + MAX(extents.overallWidth, extents.overallRight);
	region.extents.y2 = y + extents.fontDescent;

	DBG(("%s: count=%ld/%d, extents=(left=%d, right=%d, width=%d, ascent=%d, descent=%d), box=(%d, %d), (%d, %d)\n",
	     __FUNCTION__, n, count,
	     extents.overallLeft, extents.overallRight, extents.overallWidth,
	     extents.fontAscent, extents.fontDescent,
	     region.extents.x1, region.extents.y1,
	     region.extents.x2, region.extents.y2));

	translate_box(&region.extents, drawable);
	clip_box(&region.extents, gc);
	if (box_empty(&region.extents))
		return;

	region.data = NULL;
	if (!region_maybe_clip(&region, gc->pCompositeClip))
		return;

	DBG(("%s: clipped extents (%d, %d), (%d, %d)\n",
	     __FUNCTION__,
	     region.extents.x1, region.extents.y1,
	     region.extents.x2, region.extents.y2));

	if (FORCE_FALLBACK)
		goto fallback;

	if (!ACCEL_IMAGE_TEXT16)
		goto fallback;

	if (sna_font_too_large(gc->font))
		goto fallback;

	if (!PM_IS_SOLID(drawable, gc->planemask))
		goto fallback;

	if (!sna_glyph_blt(drawable, gc, x, y, n, info, &region,
			   gc->fgPixel, gc->bgPixel, false)) {
fallback:
		DBG(("%s: fallback\n", __FUNCTION__));
		gc->font->get_glyphs(gc->font, count, (unsigned char *)chars,
				     FONTLASTROW(gc->font) ? TwoD16Bit : Linear16Bit,
				     &n, info);

		if (!sna_gc_move_to_cpu(gc, drawable, &region))
			goto out;
		if (!sna_drawable_move_region_to_cpu(drawable, &region, MOVE_WRITE))
			goto out;

		if (sigtrap_get() == 0) {
			DBG(("%s: fallback -- fbImageGlyphBlt\n", __FUNCTION__));
			fbImageGlyphBlt(drawable, gc, x, y, n,
					info, FONTGLYPHS(gc->font));
			FALLBACK_FLUSH(drawable);
			sigtrap_put();
		}
out:
		sna_gc_move_to_gpu(gc);
	}
	RegionUninit(&region);
}

/* XXX Damage bypasses the Text interface and so we lose our custom gluphs */
static bool
sna_reversed_glyph_blt(DrawablePtr drawable, GCPtr gc,
		       int _x, int _y, unsigned int _n,
		       CharInfoPtr *_info, pointer _base,
		       struct kgem_bo *bo,
		       struct sna_damage **damage,
		       RegionPtr clip,
		       uint32_t fg, uint32_t bg,
		       bool transparent)
{
	PixmapPtr pixmap = get_drawable_pixmap(drawable);
	struct sna *sna = to_sna_from_pixmap(pixmap);
	const BoxRec *extents, *last_extents;
	uint32_t *b;
	int16_t dx, dy;
	uint8_t rop = transparent ? copy_ROP[gc->alu] : ROP_S;
	uint16_t unwind_batch, unwind_reloc;

	DBG(("%s: pixmap=%ld, bo=%d, damage=%p, fg=%08x, bg=%08x\n",
	     __FUNCTION__, pixmap->drawable.serialNumber, bo->handle, damage, fg, bg));

	if (bo->tiling == I915_TILING_Y) {
		DBG(("%s: converting bo from Y-tiling\n", __FUNCTION__));
		assert(bo == __sna_pixmap_get_bo(pixmap));
		bo = sna_pixmap_change_tiling(pixmap, I915_TILING_X);
		if (bo == NULL) {
			DBG(("%s: fallback -- unable to change tiling\n",
			     __FUNCTION__));
			return false;
		}
	}

	if (get_drawable_deltas(drawable, pixmap, &dx, &dy))
		RegionTranslate(clip, dx, dy);
	_x += drawable->x + dx;
	_y += drawable->y + dy;

	extents = region_rects(clip);
	last_extents = extents + region_num_rects(clip);

	if (!transparent) { /* emulate miImageGlyphBlt */
		if (!sna_blt_fill_boxes(sna, GXcopy,
					bo, drawable->bitsPerPixel,
					bg, extents, last_extents - extents)) {
			RegionTranslate(clip, -dx, -dy);
			return false;
		}
	}

	kgem_set_mode(&sna->kgem, KGEM_BLT, bo);
	if (!kgem_check_batch(&sna->kgem, 20) ||
	    !kgem_check_bo_fenced(&sna->kgem, bo) ||
	    !kgem_check_reloc(&sna->kgem, 1)) {
		kgem_submit(&sna->kgem);
		if (!kgem_check_bo_fenced(&sna->kgem, bo)) {
			RegionTranslate(clip, -dx, -dy);
			return false;
		}
		_kgem_set_mode(&sna->kgem, KGEM_BLT);
	}

	unwind_batch = sna->kgem.nbatch;
	unwind_reloc = sna->kgem.nreloc;

	DBG(("%s: glyph clip box (%d, %d), (%d, %d)\n",
	     __FUNCTION__,
	     extents->x1, extents->y1,
	     extents->x2, extents->y2));

	assert(sna->kgem.mode == KGEM_BLT);
	b = sna->kgem.batch + sna->kgem.nbatch;
	if (sna->kgem.gen >= 0100) {
		b[0] = XY_SETUP_BLT | 1 << 20 | 8;
		b[1] = bo->pitch;
		if (sna->kgem.gen >= 040 && bo->tiling) {
			b[0] |= BLT_DST_TILED;
			b[1] >>= 2;
		}
		b[1] |= 1 << 30 | transparent << 29 | blt_depth(drawable->depth) << 24 | rop << 16;
		b[2] = extents->y1 << 16 | extents->x1;
		b[3] = extents->y2 << 16 | extents->x2;
		*(uint64_t *)(b+4) =
			kgem_add_reloc64(&sna->kgem, sna->kgem.nbatch + 4, bo,
					 I915_GEM_DOMAIN_RENDER << 16 |
					 I915_GEM_DOMAIN_RENDER |
					 KGEM_RELOC_FENCED,
					 0);
		b[6] = bg;
		b[7] = fg;
		b[8] = 0;
		b[9] = 0;
		sna->kgem.nbatch += 10;
	} else {
		b[0] = XY_SETUP_BLT | 1 << 20 | 6;
		b[1] = bo->pitch;
		if (sna->kgem.gen >= 040 && bo->tiling) {
			b[0] |= BLT_DST_TILED;
			b[1] >>= 2;
		}
		b[1] |= 1 << 30 | transparent << 29 | blt_depth(drawable->depth) << 24 | rop << 16;
		b[2] = extents->y1 << 16 | extents->x1;
		b[3] = extents->y2 << 16 | extents->x2;
		b[4] = kgem_add_reloc(&sna->kgem, sna->kgem.nbatch + 4, bo,
				      I915_GEM_DOMAIN_RENDER << 16 |
				      I915_GEM_DOMAIN_RENDER |
				      KGEM_RELOC_FENCED,
				      0);
		b[5] = bg;
		b[6] = fg;
		b[7] = 0;
		sna->kgem.nbatch += 8;
	}

	do {
		CharInfoPtr *info = _info;
		int x = _x, y = _y, n = _n;

		do {
			CharInfoPtr c = *info++;
			uint8_t *glyph = FONTGLYPHBITS(base, c);
			int w = GLYPHWIDTHPIXELS(c);
			int h = GLYPHHEIGHTPIXELS(c);
			int stride = GLYPHWIDTHBYTESPADDED(c);
			int w8 = (w + 7) >> 3;
			int x1, y1, len, i;
			uint8_t *byte;

			if (w == 0 || h == 0)
				goto skip;

			len = (w8 * h + 7) >> 3 << 1;
			x1 = x + c->metrics.leftSideBearing;
			y1 = y - c->metrics.ascent;

			DBG(("%s glyph: (%d, %d) -> (%d, %d) x (%d[%d], %d), len=%d\n" ,__FUNCTION__,
			     x,y, x1, y1, w, w8, h, len));

			if (x1 >= extents->x2 || y1 >= extents->y2 ||
			    x1 + w <= extents->x1 || y1 + h <= extents->y1) {
				DBG(("%s: glyph is clipped (%d, %d)x(%d,%d) against extents (%d, %d), (%d, %d)\n",
				     __FUNCTION__,
				     x1, y1, w, h,
				     extents->x1, extents->y1,
				     extents->x2, extents->y2));
				goto skip;
			}

			{
				int clear = 1, j = h;
				uint8_t *g = glyph;

				do {
					i = w8;
					do {
						clear = *g++ == 0;
					} while (clear && --i);
					g += stride - w8;
				} while (clear && --j);
				if (clear) {
					DBG(("%s: skipping clear glyph for ImageGlyph\n",
					     __FUNCTION__));
					goto skip;
				}
			}

			assert(len > 0);
			if (!kgem_check_batch(&sna->kgem, 3+len)) {
				_kgem_submit(&sna->kgem);
				_kgem_set_mode(&sna->kgem, KGEM_BLT);

				unwind_batch = sna->kgem.nbatch;
				unwind_reloc = sna->kgem.nreloc;

				DBG(("%s: new batch, glyph clip box (%d, %d), (%d, %d)\n",
				     __FUNCTION__,
				     extents->x1, extents->y1,
				     extents->x2, extents->y2));

				assert(sna->kgem.mode == KGEM_BLT);
				b = sna->kgem.batch + sna->kgem.nbatch;
				if (sna->kgem.gen >= 0100) {
					b[0] = XY_SETUP_BLT | 1 << 20 | 8;
					b[1] = bo->pitch;
					if (bo->tiling) {
						b[0] |= BLT_DST_TILED;
						b[1] >>= 2;
					}
					b[1] |= 1 << 30 | transparent << 29 | blt_depth(drawable->depth) << 24 | rop << 16;
					b[2] = extents->y1 << 16 | extents->x1;
					b[3] = extents->y2 << 16 | extents->x2;
					*(uint64_t *)(b+4) =
						kgem_add_reloc64(&sna->kgem, sna->kgem.nbatch + 4, bo,
								 I915_GEM_DOMAIN_RENDER << 16 |
								 I915_GEM_DOMAIN_RENDER |
								 KGEM_RELOC_FENCED,
								 0);
					b[6] = bg;
					b[7] = fg;
					b[8] = 0;
					b[9] = 0;
					sna->kgem.nbatch += 10;
				} else {
					b[0] = XY_SETUP_BLT | 1 << 20 | 6;
					b[1] = bo->pitch;
					if (sna->kgem.gen >= 040 && bo->tiling) {
						b[0] |= BLT_DST_TILED;
						b[1] >>= 2;
					}
					b[1] |= 1 << 30 | transparent << 29 | blt_depth(drawable->depth) << 24 | rop << 16;
					b[2] = extents->y1 << 16 | extents->x1;
					b[3] = extents->y2 << 16 | extents->x2;
					b[4] = kgem_add_reloc(&sna->kgem, sna->kgem.nbatch + 4, bo,
							      I915_GEM_DOMAIN_RENDER << 16 |
							      I915_GEM_DOMAIN_RENDER |
							      KGEM_RELOC_FENCED,
							      0);
					b[5] = bg;
					b[6] = fg;
					b[7] = 0;
					sna->kgem.nbatch += 8;
				}
			}

			assert(sna->kgem.mode == KGEM_BLT);
			b = sna->kgem.batch + sna->kgem.nbatch;
			sna->kgem.nbatch += 3 + len;

			b[0] = XY_TEXT_IMMEDIATE_BLT | (1 + len);
			if (bo->tiling && sna->kgem.gen >= 040)
				b[0] |= BLT_DST_TILED;
			b[1] = (uint16_t)y1 << 16 | (uint16_t)x1;
			b[2] = (uint16_t)(y1+h) << 16 | (uint16_t)(x1+w);

			byte = (uint8_t *)&b[3];
			stride -= w8;
			do {
				i = w8;
				do {
					*byte++ = byte_reverse(*glyph++);
				} while (--i);
				glyph += stride;
			} while (--h);
			while ((byte - (uint8_t *)&b[3]) & 7)
				*byte++ = 0;
			assert((uint32_t *)byte == sna->kgem.batch + sna->kgem.nbatch);

			if (damage) {
				BoxRec r;

				r.x1 = x1;
				r.y1 = y1;
				r.x2 = x1 + w;
				r.y2 = y1 + h;
				if (box_intersect(&r, extents))
					sna_damage_add_box(damage, &r);
			}
skip:
			x += c->metrics.characterWidth;
		} while (--n);

		if (++extents == last_extents)
			break;

		if (kgem_check_batch(&sna->kgem, 3 + 5)) {
			assert(sna->kgem.mode == KGEM_BLT);
			b = sna->kgem.batch + sna->kgem.nbatch;
			sna->kgem.nbatch += 3;

			DBG(("%s: glyph clip box (%d, %d), (%d, %d)\n",
			     __FUNCTION__,
			     extents->x1, extents->y1,
			     extents->x2, extents->y2));

			b[0] = XY_SETUP_CLIP;
			b[1] = extents->y1 << 16 | extents->x1;
			b[2] = extents->y2 << 16 | extents->x2;
		}
	} while (1);

	if (sna->kgem.nbatch == unwind_batch + (sna->kgem.gen >= 0100 ? 10 : 8)) {
		sna->kgem.nbatch = unwind_batch;
		sna->kgem.nreloc = unwind_reloc;
		if (sna->kgem.nbatch == 0)
			kgem_bo_undo(&sna->kgem, bo);
	}

	assert_pixmap_damage(pixmap);
	sna->blt_state.fill_bo = 0;
	return true;
}

static void
sna_image_glyph(DrawablePtr drawable, GCPtr gc,
		int x, int y, unsigned int n,
		CharInfoPtr *info, pointer base)
{
	PixmapPtr pixmap = get_drawable_pixmap(drawable);
	struct sna *sna = to_sna_from_pixmap(pixmap);
	ExtentInfoRec extents;
	RegionRec region;
	struct sna_damage **damage;
	struct kgem_bo *bo;
	unsigned hint;

	if (n == 0)
		return;

	sna_glyph_extents(gc->font, info, n, &extents);
	region.extents.x1 = x + MIN(0, extents.overallLeft);
	region.extents.y1 = y - extents.fontAscent;
	region.extents.x2 = x + MAX(extents.overallWidth, extents.overallRight);
	region.extents.y2 = y + extents.fontDescent;

	DBG(("%s: count=%d, extents=(left=%d, right=%d, width=%d, ascent=%d, descent=%d), box=(%d, %d), (%d, %d)\n",
	     __FUNCTION__, n,
	     extents.overallLeft, extents.overallRight, extents.overallWidth,
	     extents.fontAscent, extents.fontDescent,
	     region.extents.x1, region.extents.y1,
	     region.extents.x2, region.extents.y2));

	translate_box(&region.extents, drawable);
	clip_box(&region.extents, gc);
	if (box_empty(&region.extents))
		return;

	DBG(("%s: extents(%d, %d), (%d, %d)\n", __FUNCTION__,
	     region.extents.x1, region.extents.y1,
	     region.extents.x2, region.extents.y2));

	region.data = NULL;
	if (!region_maybe_clip(&region, gc->pCompositeClip))
		return;

	if (FORCE_FALLBACK)
		goto fallback;

	if (!ACCEL_IMAGE_GLYPH)
		goto fallback;

	if (wedged(sna)) {
		DBG(("%s: fallback -- wedged\n", __FUNCTION__));
		goto fallback;
	}

	if (!PM_IS_SOLID(drawable, gc->planemask))
		goto fallback;

	if (sna_font_too_large(gc->font))
		goto fallback;

	if (region.data == NULL)
		hint = IGNORE_DAMAGE | PREFER_GPU;
	else
		hint = PREFER_GPU;
	if ((bo = sna_drawable_use_bo(drawable, hint,
				      &region.extents, &damage)) &&
	    sna_reversed_glyph_blt(drawable, gc, x, y, n, info, base,
				   bo, damage, &region,
				   gc->fgPixel, gc->bgPixel, false))
		goto out;

fallback:
	DBG(("%s: fallback\n", __FUNCTION__));
	if (!sna_gc_move_to_cpu(gc, drawable, &region))
		goto out_gc;
	if (!sna_drawable_move_region_to_cpu(drawable, &region, MOVE_WRITE))
		goto out_gc;

	if (sigtrap_get() == 0) {
		DBG(("%s: fallback -- fbImageGlyphBlt\n", __FUNCTION__));
		fbImageGlyphBlt(drawable, gc, x, y, n, info, base);
		FALLBACK_FLUSH(drawable);
		sigtrap_put();
	}
out_gc:
	sna_gc_move_to_gpu(gc);
out:
	RegionUninit(&region);
}

static void
sna_poly_glyph(DrawablePtr drawable, GCPtr gc,
	       int x, int y, unsigned int n,
	       CharInfoPtr *info, pointer base)
{
	PixmapPtr pixmap = get_drawable_pixmap(drawable);
	struct sna *sna = to_sna_from_pixmap(pixmap);
	ExtentInfoRec extents;
	RegionRec region;
	struct sna_damage **damage;
	struct kgem_bo *bo;
	uint32_t fg;

	if (n == 0)
		return;

	sna_glyph_extents(gc->font, info, n, &extents);
	region.extents.x1 = x + extents.overallLeft;
	region.extents.y1 = y - extents.overallAscent;
	region.extents.x2 = x + extents.overallRight;
	region.extents.y2 = y + extents.overallDescent;

	translate_box(&region.extents, drawable);
	clip_box(&region.extents, gc);
	if (box_empty(&region.extents))
		return;

	DBG(("%s: extents(%d, %d), (%d, %d)\n", __FUNCTION__,
	     region.extents.x1, region.extents.y1,
	     region.extents.x2, region.extents.y2));

	region.data = NULL;
	if (!region_maybe_clip(&region, gc->pCompositeClip))
		return;

	if (FORCE_FALLBACK)
		goto fallback;

	if (!ACCEL_POLY_GLYPH)
		goto fallback;

	if (wedged(sna)) {
		DBG(("%s: fallback -- wedged\n", __FUNCTION__));
		goto fallback;
	}

	if (!PM_IS_SOLID(drawable, gc->planemask))
		goto fallback;

	if (!gc_is_solid(gc, &fg))
		goto fallback;

	if (sna_font_too_large(gc->font))
		goto fallback;

	if ((bo = sna_drawable_use_bo(drawable, PREFER_GPU,
				      &region.extents, &damage)) &&
	    sna_reversed_glyph_blt(drawable, gc, x, y, n, info, base,
				   bo, damage, &region, fg, -1, true))
		goto out;

fallback:
	DBG(("%s: fallback\n", __FUNCTION__));
	if (!sna_gc_move_to_cpu(gc, drawable, &region))
		goto out_gc;
	if (!sna_drawable_move_region_to_cpu(drawable, &region,
					     MOVE_READ | MOVE_WRITE))
		goto out_gc;

	if (sigtrap_get() == 0) {
		DBG(("%s: fallback -- fbPolyGlyphBlt\n", __FUNCTION__));
		fbPolyGlyphBlt(drawable, gc, x, y, n, info, base);
		FALLBACK_FLUSH(drawable);
		sigtrap_put();
	}
out_gc:
	sna_gc_move_to_gpu(gc);
out:
	RegionUninit(&region);
}

static bool
sna_push_pixels_solid_blt(GCPtr gc,
			  PixmapPtr bitmap,
			  DrawablePtr drawable,
			  RegionPtr region)
{
	PixmapPtr pixmap = get_drawable_pixmap(drawable);
	struct sna *sna = to_sna_from_pixmap(pixmap);
	struct sna_damage **damage;
	struct kgem_bo *bo;
	const BoxRec *box;
	int16_t dx, dy;
	int n;
	uint8_t rop = copy_ROP[gc->alu];

	bo = sna_drawable_use_bo(drawable, PREFER_GPU, &region->extents, &damage);
	if (bo == NULL)
		return false;

	if (bo->tiling == I915_TILING_Y) {
		DBG(("%s: converting bo from Y-tiling\n", __FUNCTION__));
		assert(bo == __sna_pixmap_get_bo(pixmap));
		bo = sna_pixmap_change_tiling(pixmap, I915_TILING_X);
		if (bo == NULL) {
			DBG(("%s: fallback -- unable to change tiling\n",
			     __FUNCTION__));
			return false;
		}
	}

	if (get_drawable_deltas(drawable, pixmap, &dx, &dy))
		RegionTranslate(region, dx, dy);

	assert_pixmap_contains_box(pixmap, RegionExtents(region));
	if (damage)
		sna_damage_add(damage, region);
	assert_pixmap_damage(pixmap);

	DBG(("%s: upload(%d, %d, %d, %d)\n", __FUNCTION__,
	     region->extents.x1, region->extents.y1,
	     region->extents.x2, region->extents.y2));

	kgem_set_mode(&sna->kgem, KGEM_BLT, bo);

	/* Region is pre-clipped and translated into pixmap space */
	box = region_rects(region);
	n = region_num_rects(region);
	do {
		int bx1 = (box->x1 - region->extents.x1) & ~7;
		int bx2 = (box->x2 - region->extents.x1 + 7) & ~7;
		int bw = (bx2 - bx1)/8;
		int bh = box->y2 - box->y1;
		int bstride = ALIGN(bw, 2);
		struct kgem_bo *upload;
		void *ptr;

		if (!kgem_check_batch(&sna->kgem, 10) ||
		    !kgem_check_bo_fenced(&sna->kgem, bo) ||
		    !kgem_check_reloc_and_exec(&sna->kgem, 2)) {
			kgem_submit(&sna->kgem);
			if (!kgem_check_bo_fenced(&sna->kgem, bo))
				return false;
			_kgem_set_mode(&sna->kgem, KGEM_BLT);
		}

		upload = kgem_create_buffer(&sna->kgem,
					    bstride*bh,
					    KGEM_BUFFER_WRITE_INPLACE,
					    &ptr);
		if (!upload)
			break;

		if (sigtrap_get() == 0) {
			uint8_t *dst = ptr;

			int src_stride = bitmap->devKind;
			uint8_t *src;
			uint32_t *b;

			assert(src_stride);
			src = (uint8_t*)bitmap->devPrivate.ptr;
			src += (box->y1 - region->extents.y1) * src_stride + bx1/8;
			src_stride -= bstride;
			do {
				int i = bstride;
				do {
					*dst++ = byte_reverse(*src++);
					*dst++ = byte_reverse(*src++);
					i -= 2;
				} while (i);
				src += src_stride;
			} while (--bh);

			assert(sna->kgem.mode == KGEM_BLT);
			b = sna->kgem.batch + sna->kgem.nbatch;
			if (sna->kgem.gen >= 0100) {
				b[0] = XY_MONO_SRC_COPY | 3 << 20 | 8;
				b[0] |= ((box->x1 - region->extents.x1) & 7) << 17;
				b[1] = bo->pitch;
				if (sna->kgem.gen >= 040 && bo->tiling) {
					b[0] |= BLT_DST_TILED;
					b[1] >>= 2;
				}
				b[1] |= 1 << 29;
				b[1] |= blt_depth(drawable->depth) << 24;
				b[1] |= rop << 16;
				b[2] = box->y1 << 16 | box->x1;
				b[3] = box->y2 << 16 | box->x2;
				*(uint64_t *)(b+4) =
					kgem_add_reloc64(&sna->kgem, sna->kgem.nbatch + 4, bo,
							I915_GEM_DOMAIN_RENDER << 16 |
							I915_GEM_DOMAIN_RENDER |
							KGEM_RELOC_FENCED,
							0);
				*(uint64_t *)(b+6) =
					kgem_add_reloc64(&sna->kgem, sna->kgem.nbatch + 6, upload,
							I915_GEM_DOMAIN_RENDER << 16 |
							KGEM_RELOC_FENCED,
							0);
				b[8] = gc->bgPixel;
				b[9] = gc->fgPixel;
				sna->kgem.nbatch += 10;
			} else {
				b[0] = XY_MONO_SRC_COPY | 3 << 20 | 6;
				b[0] |= ((box->x1 - region->extents.x1) & 7) << 17;
				b[1] = bo->pitch;
				if (sna->kgem.gen >= 040 && bo->tiling) {
					b[0] |= BLT_DST_TILED;
					b[1] >>= 2;
				}
				b[1] |= 1 << 29;
				b[1] |= blt_depth(drawable->depth) << 24;
				b[1] |= rop << 16;
				b[2] = box->y1 << 16 | box->x1;
				b[3] = box->y2 << 16 | box->x2;
				b[4] = kgem_add_reloc(&sna->kgem, sna->kgem.nbatch + 4, bo,
						I915_GEM_DOMAIN_RENDER << 16 |
						I915_GEM_DOMAIN_RENDER |
						KGEM_RELOC_FENCED,
						0);
				b[5] = kgem_add_reloc(&sna->kgem, sna->kgem.nbatch + 5, upload,
						I915_GEM_DOMAIN_RENDER << 16 |
						KGEM_RELOC_FENCED,
						0);
				b[6] = gc->bgPixel;
				b[7] = gc->fgPixel;

				sna->kgem.nbatch += 8;
			}
			sigtrap_put();
		}

		kgem_bo_destroy(&sna->kgem, upload);

		box++;
	} while (--n);

	sna->blt_state.fill_bo = 0;
	return true;
}

static void
sna_push_pixels(GCPtr gc, PixmapPtr bitmap, DrawablePtr drawable,
		int w, int h,
		int x, int y)
{
	RegionRec region;

	if (w == 0 || h == 0)
		return;

	DBG(("%s (%d, %d)x(%d, %d)\n", __FUNCTION__, x, y, w, h));

	region.extents.x1 = x;
	region.extents.y1 = y;
	region.extents.x2 = region.extents.x1 + w;
	region.extents.y2 = region.extents.y1 + h;

	clip_box(&region.extents, gc);
	if (box_empty(&region.extents))
		return;

	DBG(("%s: extents(%d, %d), (%d, %d)\n", __FUNCTION__,
	     region.extents.x1, region.extents.y1,
	     region.extents.x2, region.extents.y2));

	region.data = NULL;
	if (!region_maybe_clip(&region, gc->pCompositeClip))
		return;

	switch (gc->fillStyle) {
	case FillSolid:
		if (sna_push_pixels_solid_blt(gc, bitmap, drawable, &region))
			return;
		break;
	default:
		break;
	}

	DBG(("%s: fallback\n", __FUNCTION__));
	if (!sna_gc_move_to_cpu(gc, drawable, &region))
		goto out;
	if (!sna_pixmap_move_to_cpu(bitmap, MOVE_READ))
		goto out;
	if (!sna_drawable_move_region_to_cpu(drawable, &region,
					     drawable_gc_flags(drawable, gc, false)))
		goto out;

	if (sigtrap_get() == 0) {
		DBG(("%s: fallback, fbPushPixels(%d, %d, %d %d)\n",
		     __FUNCTION__, w, h, x, y));
		fbPushPixels(gc, bitmap, drawable, w, h, x, y);
		FALLBACK_FLUSH(drawable);
		sigtrap_put();
	}
out:
	sna_gc_move_to_gpu(gc);
	RegionUninit(&region);
}

static const GCOps sna_gc_ops = {
	sna_fill_spans,
	sna_set_spans,
	sna_put_image,
	sna_copy_area,
	sna_copy_plane,
	sna_poly_point,
	sna_poly_line,
	sna_poly_segment,
	sna_poly_rectangle,
	sna_poly_arc,
	sna_poly_fill_polygon,
	sna_poly_fill_rect,
	sna_poly_fill_arc,
	sna_poly_text8,
	sna_poly_text16,
	sna_image_text8,
	sna_image_text16,
	sna_image_glyph,
	sna_poly_glyph,
	sna_push_pixels,
};

static const GCOps sna_gc_ops__cpu = {
	fbFillSpans,
	fbSetSpans,
	fbPutImage,
	fbCopyArea,
	fbCopyPlane,
	sna_poly_point__cpu,
	fbPolyLine,
	fbPolySegment,
	miPolyRectangle,
	fbPolyArc,
	miFillPolygon,
	fbPolyFillRect,
	miPolyFillArc,
	miPolyText8,
	miPolyText16,
	miImageText8,
	miImageText16,
	fbImageGlyphBlt,
	fbPolyGlyphBlt,
	fbPushPixels
};

static GCOps sna_gc_ops__tmp = {
	sna_fill_spans,
	sna_set_spans,
	sna_put_image,
	sna_copy_area,
	sna_copy_plane,
	sna_poly_point,
	sna_poly_line,
	sna_poly_segment,
	sna_poly_rectangle,
	sna_poly_arc,
	sna_poly_fill_polygon,
	sna_poly_fill_rect,
	sna_poly_fill_arc,
	sna_poly_text8,
	sna_poly_text16,
	sna_image_text8,
	sna_image_text16,
	sna_image_glyph,
	sna_poly_glyph,
	sna_push_pixels,
};

static void
sna_validate_gc(GCPtr gc, unsigned long changes, DrawablePtr drawable)
{
	DBG(("%s(%p) changes=%lx, previous serial=%lx, drawable=%lx\n", __FUNCTION__, gc,
	     changes, gc->serialNumber, drawable->serialNumber));

	if (changes & (GCClipMask|GCSubwindowMode) ||
	    drawable->serialNumber != (gc->serialNumber & DRAWABLE_SERIAL_BITS) ||
	    (has_clip(gc) && (changes & (GCClipXOrigin | GCClipYOrigin)))) {
		DBG(("%s: recomputing clip\n", __FUNCTION__));
		miComputeCompositeClip(gc, drawable);
		DBG(("%s: composite clip=%dx[(%d, %d), (%d, %d)] [%p]\n",
		     __FUNCTION__,
		     region_num_rects(gc->pCompositeClip),
		     gc->pCompositeClip->extents.x1,
		     gc->pCompositeClip->extents.y1,
		     gc->pCompositeClip->extents.x2,
		     gc->pCompositeClip->extents.y2,
		     gc->pCompositeClip));
	}

	assert(gc->pCompositeClip);
	assert(RegionNil(gc->pCompositeClip) || gc->pCompositeClip->extents.x1 >= drawable->x);
	assert(RegionNil(gc->pCompositeClip) || gc->pCompositeClip->extents.y1 >= drawable->y);
	assert(RegionNil(gc->pCompositeClip) || gc->pCompositeClip->extents.x2 - drawable->x <= drawable->width);
	assert(RegionNil(gc->pCompositeClip) || gc->pCompositeClip->extents.y2 - drawable->y <= drawable->height);

	sna_gc(gc)->changes |= changes;
	sna_gc(gc)->serial = gc->serialNumber;
}

static const GCFuncs sna_gc_funcs = {
	sna_validate_gc,
	miChangeGC,
	miCopyGC,
	miDestroyGC,
	miChangeClip,
	miDestroyClip,
	miCopyClip
};

static const GCFuncs sna_gc_funcs__cpu = {
	fbValidateGC,
	miChangeGC,
	miCopyGC,
	miDestroyGC,
	miChangeClip,
	miDestroyClip,
	miCopyClip
};

static int sna_create_gc(GCPtr gc)
{
	gc->miTranslate = 1;
	gc->fExpose = 1;

	gc->freeCompClip = 0;
	gc->pCompositeClip = 0;
	gc->pRotatedPixmap = 0;

	fb_gc(gc)->bpp = bits_per_pixel(gc->depth);

	gc->funcs = (GCFuncs *)&sna_gc_funcs;
	gc->ops = (GCOps *)&sna_gc_ops;
	return true;
}

static bool
sna_get_image__inplace(PixmapPtr pixmap,
		       RegionPtr region,
		       char *dst,
		       unsigned flags,
		       bool idle)
{
	struct sna_pixmap *priv = sna_pixmap(pixmap);
	struct sna *sna = to_sna_from_pixmap(pixmap);
	char *src;

	if (!USE_INPLACE)
		return false;

	assert(priv && priv->gpu_bo);

	switch (priv->gpu_bo->tiling) {
	case I915_TILING_Y:
		return false;
	case I915_TILING_X:
		if (!sna->kgem.memcpy_from_tiled_x)
			return false;
	default:
		break;
	}

	if (!kgem_bo_can_map__cpu(&sna->kgem, priv->gpu_bo, FORCE_FULL_SYNC))
		return false;

	if (idle && __kgem_bo_is_busy(&sna->kgem, priv->gpu_bo))
		return false;

	if (priv->move_to_gpu && !priv->move_to_gpu(sna, priv, MOVE_READ))
		return false;

	assert(sna_damage_contains_box(&priv->gpu_damage, &region->extents) == PIXMAN_REGION_IN);
	assert(sna_damage_contains_box(&priv->cpu_damage, &region->extents) == PIXMAN_REGION_OUT);

	src = kgem_bo_map__cpu(&sna->kgem, priv->gpu_bo);
	if (src == NULL)
		return false;

	kgem_bo_sync__cpu_full(&sna->kgem, priv->gpu_bo, FORCE_FULL_SYNC);

	if (sigtrap_get())
		return false;

	if (priv->gpu_bo->tiling) {
		DBG(("%s: download through a tiled CPU map\n", __FUNCTION__));
		memcpy_from_tiled_x(&sna->kgem, src, dst,
				    pixmap->drawable.bitsPerPixel,
				    priv->gpu_bo->pitch,
				    PixmapBytePad(region->extents.x2 - region->extents.x1,
						  pixmap->drawable.depth),
				    region->extents.x1, region->extents.y1,
				    0, 0,
				    region->extents.x2 - region->extents.x1,
				    region->extents.y2 - region->extents.y1);
	} else {
		DBG(("%s: download through a linear CPU map\n", __FUNCTION__));
		memcpy_blt(src, dst,
			   pixmap->drawable.bitsPerPixel,
			   priv->gpu_bo->pitch,
			   PixmapBytePad(region->extents.x2 - region->extents.x1,
					 pixmap->drawable.depth),
			   region->extents.x1, region->extents.y1,
			   0, 0,
			   region->extents.x2 - region->extents.x1,
			   region->extents.y2 - region->extents.y1);
		if (!priv->shm) {
			assert(src == MAP(priv->gpu_bo->map__cpu));
			pixmap->devPrivate.ptr = src;
			pixmap->devKind = priv->gpu_bo->pitch;
			priv->mapped = MAPPED_CPU;
			assert_pixmap_map(pixmap, priv);
			priv->cpu = true;
		}
	}

	sigtrap_put();
	return true;
}

static bool
sna_get_image__blt(PixmapPtr pixmap,
		   RegionPtr region,
		   char *dst,
		   unsigned flags)
{
	struct sna_pixmap *priv = sna_pixmap(pixmap);
	struct sna *sna = to_sna_from_pixmap(pixmap);
	struct kgem_bo *dst_bo;
	bool ok = false;
	int pitch;

	assert(priv && priv->gpu_bo);

	if (!sna->kgem.has_userptr || !USE_USERPTR_DOWNLOADS)
		return false;

	if (!sna->kgem.can_blt_cpu)
		return false;

	if ((priv->create & (KGEM_CAN_CREATE_GTT | KGEM_CAN_CREATE_LARGE)) == KGEM_CAN_CREATE_GTT &&
	    kgem_bo_can_map(&sna->kgem, priv->gpu_bo)) {
		if (flags & (MOVE_WHOLE_HINT | MOVE_INPLACE_HINT))
			return false;

		if (priv->gpu_damage == NULL)
			return false;

		assert(priv->gpu_bo);
		if (!__kgem_bo_is_busy(&sna->kgem, priv->gpu_bo))
			return false;
	} else {
		if (priv->gpu_damage == NULL)
			return false;

		assert(priv->gpu_bo);
	}

	if (priv->move_to_gpu && !priv->move_to_gpu(sna, priv, MOVE_READ))
		return false;

	DBG(("%s: download through a temporary map\n", __FUNCTION__));

	assert(sna_damage_contains_box(&priv->gpu_damage, &region->extents) == PIXMAN_REGION_IN);
	assert(sna_damage_contains_box(&priv->cpu_damage, &region->extents) == PIXMAN_REGION_OUT);

	pitch = PixmapBytePad(region->extents.x2 - region->extents.x1,
			      pixmap->drawable.depth);
	dst_bo = kgem_create_map(&sna->kgem, dst,
				 pitch * (region->extents.y2 - region->extents.y1),
				 false);
	if (dst_bo) {
		dst_bo->pitch = pitch;
		kgem_bo_mark_unreusable(dst_bo);

		ok = sna->render.copy_boxes(sna, GXcopy,
					    &pixmap->drawable, priv->gpu_bo, 0, 0,
					    &pixmap->drawable, dst_bo,
					    -region->extents.x1,
					    -region->extents.y1,
					    &region->extents, 1,
					    COPY_LAST);

		kgem_bo_sync__cpu(&sna->kgem, dst_bo);
		assert(dst_bo->rq == NULL);
		kgem_bo_destroy(&sna->kgem, dst_bo);
	}

	return ok;
}

static bool
sna_get_image__fast(PixmapPtr pixmap,
		   RegionPtr region,
		   char *dst,
		   unsigned flags)
{
	struct sna_pixmap *priv = sna_pixmap(pixmap);

	DBG(("%s: attached?=%d, has gpu damage?=%d\n",
	     __FUNCTION__, priv != NULL,  priv && priv->gpu_damage));
	if (priv == NULL || priv->gpu_damage == NULL)
		return false;

	if (priv->clear) {
		int w = region->extents.x2 - region->extents.x1;
		int h = region->extents.y2 - region->extents.y1;
		int pitch = PixmapBytePad(w, pixmap->drawable.depth);

		DBG(("%s: applying clear [%08x]\n",
		     __FUNCTION__, priv->clear_color));
		assert(DAMAGE_IS_ALL(priv->gpu_damage));
		assert(priv->cpu_damage == NULL);

		if (priv->clear_color == 0 ||
		    pixmap->drawable.bitsPerPixel == 8 ||
		    priv->clear_color == (1U << pixmap->drawable.depth) - 1) {
			DBG(("%s: memset clear [%02x]\n",
			     __FUNCTION__, priv->clear_color & 0xff));
			memset(dst, priv->clear_color, pitch * h);
		} else {
			pixman_fill((uint32_t *)dst,
				    pitch/sizeof(uint32_t),
				    pixmap->drawable.bitsPerPixel,
				    0, 0,
				    w, h,
				    priv->clear_color);
		}

		return true;
	}

	if (!DAMAGE_IS_ALL(priv->gpu_damage) &&
	    !sna_damage_contains_box__no_reduce(priv->gpu_damage,
						&region->extents))
		return false;

	if (sna_get_image__inplace(pixmap, region, dst, flags, true))
		return true;

	if (sna_get_image__blt(pixmap, region, dst, flags))
		return true;

	if (sna_get_image__inplace(pixmap, region, dst, flags, false))
		return true;

	return false;
}

static void
sna_get_image(DrawablePtr drawable,
	      int x, int y, int w, int h,
	      unsigned int format, unsigned long mask,
	      char *dst)
{
	RegionRec region;
	unsigned int flags;

	if (!fbDrawableEnabled(drawable))
		return;

	DBG(("%s: pixmap=%ld (%d, %d)x(%d, %d), format=%d, mask=%lx, depth=%d\n",
	     __FUNCTION__,
	     (long)get_drawable_pixmap(drawable)->drawable.serialNumber,
	     x, y, w, h, format, mask, drawable->depth));

	flags = MOVE_READ;
	if ((w | h) == 1)
		flags |= MOVE_INPLACE_HINT;
	if (w == drawable->width)
		flags |= MOVE_WHOLE_HINT;

	if (ACCEL_GET_IMAGE &&
	    !FORCE_FALLBACK &&
	    format == ZPixmap &&
	    drawable->bitsPerPixel >= 8 &&
	    PM_IS_SOLID(drawable, mask)) {
		PixmapPtr pixmap = get_drawable_pixmap(drawable);
		int16_t dx, dy;

		get_drawable_deltas(drawable, pixmap, &dx, &dy);
		region.extents.x1 = x + drawable->x + dx;
		region.extents.y1 = y + drawable->y + dy;
		region.extents.x2 = region.extents.x1 + w;
		region.extents.y2 = region.extents.y1 + h;
		region.data = NULL;

		if (sna_get_image__fast(pixmap, &region, dst, flags))
			return;

		if (!sna_drawable_move_region_to_cpu(&pixmap->drawable,
						     &region, flags))
			return;

		DBG(("%s: copy box (%d, %d), (%d, %d)\n",
		     __FUNCTION__,
		     region.extents.x1, region.extents.y1,
		     region.extents.x2, region.extents.y2));
		assert(has_coherent_ptr(to_sna_from_pixmap(pixmap), sna_pixmap(pixmap), MOVE_READ));
		if (sigtrap_get() == 0) {
			assert(pixmap->devKind);
			memcpy_blt(pixmap->devPrivate.ptr, dst, drawable->bitsPerPixel,
				   pixmap->devKind, PixmapBytePad(w, drawable->depth),
				   region.extents.x1, region.extents.y1, 0, 0, w, h);
			sigtrap_put();
		}
	} else {
		region.extents.x1 = x + drawable->x;
		region.extents.y1 = y + drawable->y;
		region.extents.x2 = region.extents.x1 + w;
		region.extents.y2 = region.extents.y1 + h;
		region.data = NULL;

		if (sna_drawable_move_region_to_cpu(drawable, &region, flags))
			fbGetImage(drawable, x, y, w, h, format, mask, dst);
	}
}

static void
sna_get_spans(DrawablePtr drawable, int wMax,
	      DDXPointPtr pt, int *width, int n, char *start)
{
	RegionRec region;

	if (!fbDrawableEnabled(drawable))
		return;

	if (sna_spans_extents(drawable, NULL, n, pt, width, &region.extents) == 0)
		return;

	region.data = NULL;
	if (!sna_drawable_move_region_to_cpu(drawable, &region, MOVE_READ))
		return;

	fbGetSpans(drawable, wMax, pt, width, n, start);
}

static void
sna_copy_window(WindowPtr win, DDXPointRec origin, RegionPtr src)
{
	PixmapPtr pixmap = get_window_pixmap(win);
	struct sna *sna = to_sna_from_pixmap(pixmap);
	RegionRec dst;
	int dx, dy;

	DBG(("%s origin=(%d, %d)\n", __FUNCTION__, origin.x, origin.y));
	if (!fbWindowEnabled(win))
		return;

	dx = origin.x - win->drawable.x;
	dy = origin.y - win->drawable.y;
	RegionTranslate(src, -dx, -dy);

	RegionNull(&dst);
	RegionIntersect(&dst, &win->borderClip, src);
	if (box_empty(&dst.extents))
		return;

#ifdef COMPOSITE
	if (pixmap->screen_x | pixmap->screen_y)
		RegionTranslate(&dst, -pixmap->screen_x, -pixmap->screen_y);
#endif

	if (wedged(sna) || FORCE_FALLBACK || !ACCEL_COPY_WINDOW) {
		DBG(("%s: fallback -- wedged\n", __FUNCTION__));
		if (!sna_pixmap_move_to_cpu(pixmap, MOVE_READ | MOVE_WRITE))
			return;

		if (sigtrap_get() == 0) {
			miCopyRegion(&pixmap->drawable, &pixmap->drawable,
				     0, &dst, dx, dy, fbCopyNtoN, 0, NULL);
			sigtrap_put();
		}
	} else {
		sna_self_copy_boxes(&pixmap->drawable, &pixmap->drawable, NULL,
				    &dst, dx, dy, 0, NULL);
	}

	RegionUninit(&dst);
}

static Bool sna_change_window_attributes(WindowPtr win, unsigned long mask)
{
	bool ret = true;

	DBG(("%s\n", __FUNCTION__));

	/* Check if the fb layer wishes to modify the attached pixmaps,
	 * to fix up mismatches between the window and pixmap depths.
	 */
	if (mask & CWBackPixmap && win->backgroundState == BackgroundPixmap) {
		DBG(("%s: flushing background pixmap\n", __FUNCTION__));
		ret &= sna_validate_pixmap(&win->drawable, win->background.pixmap);
	}

	if (mask & CWBorderPixmap && win->borderIsPixel == false) {
		DBG(("%s: flushing border pixmap\n", __FUNCTION__));
		ret &= sna_validate_pixmap(&win->drawable, win->border.pixmap);
	}

	return ret;
}

void sna_accel_flush(struct sna *sna)
{
	struct sna_pixmap *priv;

	/* XXX we should be able to reduce the frequency of flushes further
	 * by checking for outgoing damage events or sync replies. Tricky,
	 * and doesn't appear to mitigate the performance loss.
	 */
	DBG(("%s: flush?=%d, dirty?=%d\n", __FUNCTION__,
	     sna->kgem.flush, !list_is_empty(&sna->flush_pixmaps)));

	/* flush any pending damage from shadow copies to tfp clients */
	while (!list_is_empty(&sna->flush_pixmaps)) {
		bool ret;

		priv = list_first_entry(&sna->flush_pixmaps,
					struct sna_pixmap, flush_list);

		list_del(&priv->flush_list);
		if (priv->shm) {
			DBG(("%s: syncing SHM pixmap=%ld (refcnt=%d)\n",
			     __FUNCTION__,
			     priv->pixmap->drawable.serialNumber,
			     priv->pixmap->refcnt));
			assert(!priv->flush);
			ret = sna_pixmap_move_to_cpu(priv->pixmap,
						     MOVE_READ | MOVE_WRITE);
			assert(!ret || priv->gpu_bo == NULL);
			if (priv->pixmap->refcnt == 0) {
				sna_damage_destroy(&priv->cpu_damage);
				__sna_free_pixmap(sna, priv->pixmap, priv);
			}
		} else {
			DBG(("%s: flushing DRI pixmap=%ld\n", __FUNCTION__,
			     priv->pixmap->drawable.serialNumber));
			assert(priv->flush);
			if (sna_pixmap_move_to_gpu(priv->pixmap,
						   MOVE_READ | __MOVE_FORCE)) {
				if (priv->flush & 2) {
					kgem_bo_unclean(&sna->kgem, priv->gpu_bo);
					sna_damage_all(&priv->gpu_damage, priv->pixmap);
					assert(priv->cpu_damage == NULL);
					priv->clear = false;
					priv->cpu = false;
				}
			}
		}
		(void)ret;
	}

	if (sna->kgem.flush)
		kgem_submit(&sna->kgem);
}

static void
sna_accel_flush_callback(CallbackListPtr *list,
			 pointer user_data, pointer call_data)
{
	sna_accel_flush(user_data);
}

static struct sna_pixmap *sna_accel_scanout(struct sna *sna)
{
	struct sna_pixmap *priv;

	if (sna->mode.front_active == 0)
		return NULL;

	assert(sna->vblank_interval);
	assert(sna->front);

	priv = sna_pixmap(sna->front);
	if (priv->gpu_bo == NULL)
		return NULL;

	return priv;
}

#define TIME currentTime.milliseconds
static void sna_accel_disarm_timer(struct sna *sna, int id)
{
	DBG(("%s[%d] (time=%ld)\n", __FUNCTION__, id, (long)TIME));
	sna->timer_active &= ~(1<<id);
}

static bool has_offload_slaves(struct sna *sna)
{
#if HAS_PIXMAP_SHARING
	ScreenPtr screen = sna->scrn->pScreen;
	PixmapDirtyUpdatePtr dirty;

	xorg_list_for_each_entry(dirty, &screen->pixmap_dirty_list, ent) {
		assert(dirty->src == sna->front);
		if (RegionNotEmpty(DamageRegion(dirty->damage)))
			return true;
	}
#endif
	return false;
}

static bool has_shadow(struct sna *sna)
{
	DamagePtr damage = sna->mode.shadow_damage;

	if (damage == NULL)
		return false;

	DBG(("%s: has pending damage? %d, outstanding flips: %d\n",
	     __FUNCTION__,
	     RegionNotEmpty(DamageRegion(damage)),
	     sna->mode.flip_active));

	return RegionNotEmpty(DamageRegion(damage));
}

static bool start_flush(struct sna *sna)
{
	struct sna_pixmap *scanout;

	if (has_offload_slaves(sna)) {
		DBG(("%s: has offload slaves\n", __FUNCTION__));
		return true;
	}

	if (has_shadow(sna)) {
		DBG(("%s: has dirty shadow\n", __FUNCTION__));
		return true;
	}

	scanout = sna_accel_scanout(sna);
	if (!scanout)
		return false;

	if (sna->flags & SNA_FLUSH_GTT && scanout->gpu_bo->gtt_dirty) {
		scanout->gpu_bo->needs_flush = true;
		return true;
	}

	if (scanout->cpu_damage || scanout->gpu_bo->needs_flush)
		return true;

	kgem_scanout_flush(&sna->kgem, scanout->gpu_bo);
	return false;
}

static bool stop_flush(struct sna *sna, struct sna_pixmap *scanout)
{
	DBG(("%s: scanout=%d shadow?=%d, slaves?=%d, (cpu?=%d || gpu?=%d))\n",
	     __FUNCTION__,
	     scanout && scanout->gpu_bo ? scanout->gpu_bo->handle : 0,
	     has_shadow(sna), has_offload_slaves(sna),
	     scanout && scanout->cpu_damage != NULL,
	     scanout && scanout->gpu_bo && scanout->gpu_bo->rq != NULL));

	if (has_offload_slaves(sna))
		return true;

	if (has_shadow(sna))
		return true;

	if (!scanout)
		return false;

	if (sna->flags & SNA_FLUSH_GTT && scanout->gpu_bo->gtt_dirty) {
		scanout->gpu_bo->needs_flush = true;
		return true;
	}

	return scanout->cpu_damage || scanout->gpu_bo->needs_flush;
}

static void timer_enable(struct sna *sna, int whom, int interval)
{
	if (!sna->timer_active)
		UpdateCurrentTimeIf();
	sna->timer_active |= 1 << whom;
	sna->timer_expire[whom] = TIME + interval;
	DBG(("%s (time=%ld), starting timer %d\n", __FUNCTION__, (long)TIME, whom));
}

static bool sna_scanout_do_flush(struct sna *sna)
{
	int interval = sna->vblank_interval ?: 50;
	if (sna->timer_active & (1<<(FLUSH_TIMER))) {
		int32_t delta = sna->timer_expire[FLUSH_TIMER] - TIME;
		DBG(("%s: flush timer active: delta=%d\n",
		     __FUNCTION__, delta));
		if (delta <= 3) {
			DBG(("%s (time=%ld), triggered\n", __FUNCTION__, (long)TIME));
			sna->timer_expire[FLUSH_TIMER] = TIME + interval;
			return true;
		}
	} else {
		if (start_flush(sna))
			timer_enable(sna, FLUSH_TIMER, interval/2);
	}

	return false;
}

static bool sna_accel_do_throttle(struct sna *sna)
{
	if (sna->timer_active & (1<<(THROTTLE_TIMER))) {
		int32_t delta = sna->timer_expire[THROTTLE_TIMER] - TIME;
		if (delta <= 3) {
			DBG(("%s (time=%ld), triggered\n", __FUNCTION__, (long)TIME));
			sna->timer_expire[THROTTLE_TIMER] = TIME + 20;
			return true;
		}
	} else if (!sna->kgem.need_retire) {
		DBG(("%s -- no pending activity\n", __FUNCTION__));
	} else
		timer_enable(sna, THROTTLE_TIMER, 20);

	return false;
}

static bool sna_accel_do_expire(struct sna *sna)
{
	if (sna->timer_active & (1<<(EXPIRE_TIMER))) {
		int32_t delta = sna->timer_expire[EXPIRE_TIMER] - TIME;
		if (delta <= 3) {
			DBG(("%s (time=%ld), triggered\n", __FUNCTION__, (long)TIME));
			sna->timer_expire[EXPIRE_TIMER] =
				TIME + MAX_INACTIVE_TIME * 1000;
			return true;
		}
	} else if (sna->kgem.need_expire)
		timer_enable(sna, EXPIRE_TIMER, MAX_INACTIVE_TIME * 1000);

	return false;
}

static void sna_accel_post_damage(struct sna *sna)
{
#if HAS_PIXMAP_SHARING
	ScreenPtr screen = sna->scrn->pScreen;
	PixmapDirtyUpdatePtr dirty;
	bool flush = false;

	xorg_list_for_each_entry(dirty, &screen->pixmap_dirty_list, ent) {
		RegionRec region, *damage;
		PixmapPtr src, dst;
		const BoxRec *box;
		int16_t dx, dy;
		int n;

		assert(dirty->src == sna->front);

		damage = DamageRegion(dirty->damage);
		if (RegionNil(damage))
			continue;

		src = dirty->src;
		dst = dirty->slave_dst->master_pixmap;

		region.extents.x1 = dirty->x;
		region.extents.x2 = dirty->x + dst->drawable.width;
		region.extents.y1 = dirty->y;
		region.extents.y2 = dirty->y + dst->drawable.height;
		region.data = NULL;

		DBG(("%s: pushing damage ((%d, %d), (%d, %d))x%d to slave pixmap=%d, ((%d, %d), (%d, %d))\n", __FUNCTION__,
		     damage->extents.x1, damage->extents.y1,
		     damage->extents.x2, damage->extents.y2,
		     region_num_rects(damage),
		     dst->drawable.serialNumber,
		     region.extents.x1, region.extents.y1,
		     region.extents.x2, region.extents.y2));

		RegionIntersect(&region, &region, damage);
		if (RegionNil(&region))
			goto skip;

		dx = -dirty->x;
		dy = -dirty->y;
#if HAS_DIRTYTRACKING2
		dx += dirty->dst_x;
		dy += dirty->dst_y;
#endif
		RegionTranslate(&region, dx, dy);
		DamageRegionAppend(&dirty->slave_dst->drawable, &region);

		DBG(("%s: slave:  ((%d, %d), (%d, %d))x%d\n", __FUNCTION__,
		     region.extents.x1, region.extents.y1,
		     region.extents.x2, region.extents.y2,
		     region_num_rects(&region)));

		box = region_rects(&region);
		n = region_num_rects(&region);
		if (wedged(sna)) {
fallback:
			if (!sna_pixmap_move_to_cpu(src, MOVE_READ))
				goto skip;

			if (!sna_pixmap_move_to_cpu(dst, MOVE_READ | MOVE_WRITE | MOVE_INPLACE_HINT))
				goto skip;

			if (sigtrap_get() == 0) {
				assert(src->drawable.bitsPerPixel == dst->drawable.bitsPerPixel);
				do {
					DBG(("%s: copy box (%d, %d)->(%d, %d)x(%d, %d)\n",
					     __FUNCTION__,
					     box->x1 - dx, box->y1 - dy,
					     box->x1, box->y1,
					     box->x2 - box->x1, box->y2 - box->y1));

					assert(box->x2 > box->x1);
					assert(box->y2 > box->y1);

					assert(box->x1 - dx >= 0);
					assert(box->y1 - dy >= 0);
					assert(box->x2 - dx <= src->drawable.width);
					assert(box->y2 - dy <= src->drawable.height);

					assert(box->x1 >= 0);
					assert(box->y1 >= 0);
					assert(box->x2 <= src->drawable.width);
					assert(box->y2 <= src->drawable.height);

					assert(has_coherent_ptr(sna, sna_pixmap(src), MOVE_READ));
					assert(has_coherent_ptr(sna, sna_pixmap(dst), MOVE_WRITE));
					assert(src->devKind);
					assert(dst->devKind);
					memcpy_blt(src->devPrivate.ptr,
						   dst->devPrivate.ptr,
						   src->drawable.bitsPerPixel,
						   src->devKind, dst->devKind,
						   box->x1 - dx,      box->y1 - dy,
						   box->x1,           box->y1,
						   box->x2 - box->x1, box->y2 - box->y1);
					box++;
				} while (--n);
				sigtrap_put();
			}
		} else {
			if (!sna_pixmap_move_to_gpu(src, MOVE_READ | MOVE_ASYNC_HINT | __MOVE_FORCE))
				goto fallback;

			if (!sna_pixmap_move_to_gpu(dst, MOVE_READ | MOVE_WRITE | MOVE_ASYNC_HINT | __MOVE_FORCE))
				goto fallback;

			if (!sna->render.copy_boxes(sna, GXcopy,
						    &src->drawable, __sna_pixmap_get_bo(src), -dx, -dy,
						    &dst->drawable, __sna_pixmap_get_bo(dst),   0,   0,
						    box, n, COPY_LAST))
				goto fallback;

			flush = true;
		}

		DamageRegionProcessPending(&dirty->slave_dst->drawable);
skip:
		RegionUninit(&region);
		DamageEmpty(dirty->damage);
	}
	if (flush)
		kgem_submit(&sna->kgem);
#endif
}

static void sna_scanout_flush(struct sna *sna)
{
	struct sna_pixmap *priv = sna_accel_scanout(sna);
	bool busy;

	DBG(("%s (time=%ld), cpu damage? %d, exec? %d nbatch=%d, busy? %d\n",
	     __FUNCTION__, (long)TIME,
	     priv && priv->cpu_damage,
	     priv && priv->gpu_bo->exec != NULL,
	     sna->kgem.nbatch,
	     sna->kgem.busy));

	busy = stop_flush(sna, priv);
	if (!sna->kgem.busy && !busy)
		sna_accel_disarm_timer(sna, FLUSH_TIMER);
	sna->kgem.busy = busy;

	if (priv &&
	    sna_pixmap_force_to_gpu(priv->pixmap,
				    MOVE_READ | MOVE_ASYNC_HINT | __MOVE_SCANOUT))
		kgem_scanout_flush(&sna->kgem, priv->gpu_bo);

	sna_mode_redisplay(sna);
	sna_accel_post_damage(sna);
}

static void sna_accel_throttle(struct sna *sna)
{
	DBG(("%s (time=%ld)\n", __FUNCTION__, (long)TIME));

	if (sna->kgem.need_throttle) {
		kgem_submit(&sna->kgem);
		kgem_throttle(&sna->kgem);
	}

	if (!sna->kgem.need_retire)
		sna_accel_disarm_timer(sna, THROTTLE_TIMER);
}

static void sna_pixmap_expire(struct sna *sna)
{
	while (sna->freed_pixmap) {
		PixmapPtr pixmap = __pop_freed_pixmap(sna);
		free(sna_pixmap(pixmap));
		FreePixmap(pixmap);
	}
}

static void sna_accel_expire(struct sna *sna)
{
	DBG(("%s (time=%ld)\n", __FUNCTION__, (long)TIME));

	kgem_expire_cache(&sna->kgem);
	sna_pixmap_expire(sna);

	if (!sna->kgem.need_expire)
		sna_accel_disarm_timer(sna, EXPIRE_TIMER);
}

#ifdef DEBUG_MEMORY
static bool sna_accel_do_debug_memory(struct sna *sna)
{
	int32_t delta = sna->timer_expire[DEBUG_MEMORY_TIMER] - TIME;

	if (delta <= 3) {
		sna->timer_expire[DEBUG_MEMORY_TIMER] = TIME + 10 * 1000;
		return true;
	} else
		return false;
}

static void sna_accel_debug_memory(struct sna *sna)
{
	ErrorF("Allocated pixmaps: %d (cached: %d), bo: %d, %lu bytes (CPU bo: %d, %lu bytes)\n",
	       sna->debug_memory.pixmap_allocs,
	       sna->debug_memory.pixmap_cached,
	       sna->kgem.debug_memory.bo_allocs,
	       (unsigned long)sna->kgem.debug_memory.bo_bytes,
	       sna->debug_memory.cpu_bo_allocs,
	       (unsigned long)sna->debug_memory.cpu_bo_bytes);

#ifdef VALGRIND_DO_ADDED_LEAK_CHECK
	VG(VALGRIND_DO_ADDED_LEAK_CHECK);
#endif
}

#else
#define sna_accel_do_debug_memory(x) 0
static void sna_accel_debug_memory(struct sna *sna) { }
#endif

static ShmFuncs shm_funcs = { sna_pixmap_create_shm, NULL };

static PixmapPtr
sna_get_window_pixmap(WindowPtr window)
{
	return get_window_pixmap(window);
}

static void
sna_set_window_pixmap(WindowPtr window, PixmapPtr pixmap)
{
	*(PixmapPtr *)__get_private(window, sna_window_key) = pixmap;
}

struct sna_visit_set_pixmap_window {
	PixmapPtr old, new;
};

static int
sna_visit_set_window_pixmap(WindowPtr window, pointer data)
{
    struct sna_visit_set_pixmap_window *visit = data;

    if (sna_get_window_pixmap(window) == visit->old) {
	    window->drawable.pScreen->SetWindowPixmap(window, visit->new);
	    return WT_WALKCHILDREN;
    }

    return WT_DONTWALKCHILDREN;
}

static void
migrate_dirty_tracking(PixmapPtr old_front, PixmapPtr new_front)
{
#if HAS_PIXMAP_SHARING
	ScreenPtr screen = old_front->drawable.pScreen;
	PixmapDirtyUpdatePtr dirty, safe;

	xorg_list_for_each_entry_safe(dirty, safe, &screen->pixmap_dirty_list, ent) {
		assert(dirty->src == old_front);
		if (dirty->src != old_front)
			continue;

		DamageUnregister(&dirty->src->drawable, dirty->damage);
		DamageDestroy(dirty->damage);

		dirty->damage = DamageCreate(NULL, NULL,
					     DamageReportNone,
					     TRUE, screen, screen);
		if (!dirty->damage) {
			xorg_list_del(&dirty->ent);
			free(dirty);
			continue;
		}

		DamageRegister(&new_front->drawable, dirty->damage);
		dirty->src = new_front;
	}
#endif
}

static void
sna_set_screen_pixmap(PixmapPtr pixmap)
{
	ScreenPtr screen = pixmap->drawable.pScreen;
	PixmapPtr old_front = screen->devPrivate;
	WindowPtr root;

	DBG(("%s: changing from pixmap=%ld to pixmap=%ld, (sna->front=%ld)\n",
	     __FUNCTION__,
	     old_front ? (long)old_front->drawable.serialNumber : 0,
	     pixmap ? (long)pixmap->drawable.serialNumber : 0,
	     to_sna_from_pixmap(pixmap)->front ? (long)to_sna_from_pixmap(pixmap)->front->drawable.serialNumber : 0));

	assert(to_sna_from_pixmap(pixmap) == to_sna_from_screen(screen));
	assert(to_sna_from_pixmap(pixmap)->front == old_front);

	if (old_front) {
		assert(to_sna_from_pixmap(old_front)->front == old_front);
		migrate_dirty_tracking(old_front, pixmap);
	}

	root = get_root_window(screen);
	if (root) {
		struct sna_visit_set_pixmap_window visit = { old_front, pixmap };
		TraverseTree(root, sna_visit_set_window_pixmap, &visit);
		assert(fbGetWindowPixmap(root) == pixmap);
	}

	to_sna_from_pixmap(pixmap)->front = pixmap;
	screen->devPrivate = pixmap;
	pixmap->refcnt++;

	if (old_front)
		screen->DestroyPixmap(old_front);
}

static Bool
sna_create_window(WindowPtr win)
{
	sna_set_window_pixmap(win, win->drawable.pScreen->devPrivate);
	return TRUE;
}

static Bool
sna_map_window(WindowPtr win)
{
	return TRUE;
}

static Bool
sna_position_window(WindowPtr win, int x, int y)
{
	return TRUE;
}

static Bool
sna_unmap_window(WindowPtr win)
{
	return TRUE;
}

static Bool
sna_destroy_window(WindowPtr win)
{
	sna_video_destroy_window(win);
	sna_dri2_destroy_window(win);
	return TRUE;
}

static void
sna_query_best_size(int class,
		    unsigned short *width, unsigned short *height,
		    ScreenPtr screen)
{
	unsigned short w;

	switch (class) {
	case CursorShape:
		if (*width > screen->width)
			*width = screen->width;
		if (*height > screen->height)
			*height = screen->height;
		break;

	case TileShape:
	case StippleShape:
		w = *width;
		if ((w & (w - 1)) && w < FB_UNIT) {
			for (w = 1; w < *width; w <<= 1)
				;
			*width = w;
		}
		break;
	}
}

static void sna_store_colors(ColormapPtr cmap, int n, xColorItem *def)
{
}

static bool sna_picture_init(ScreenPtr screen)
{
	PictureScreenPtr ps;

	DBG(("%s\n", __FUNCTION__));

	if (!miPictureInit(screen, NULL, 0))
		return false;

	ps = GetPictureScreen(screen);
	assert(ps != NULL);
	assert(ps->CreatePicture != NULL);
	assert(ps->DestroyPicture != NULL);

	ps->Composite = sna_composite;
	ps->CompositeRects = sna_composite_rectangles;
	ps->Glyphs = sna_glyphs;
	if (xf86IsEntityShared(xf86ScreenToScrn(screen)->entityList[0]))
		ps->Glyphs = sna_glyphs__shared;
	ps->UnrealizeGlyph = sna_glyph_unrealize;
	ps->AddTraps = sna_add_traps;
	ps->Trapezoids = sna_composite_trapezoids;
#if HAS_PIXMAN_TRIANGLES
	ps->Triangles = sna_composite_triangles;
#if PICTURE_SCREEN_VERSION >= 2
	ps->TriStrip = sna_composite_tristrip;
	ps->TriFan = sna_composite_trifan;
#endif
#endif

	return true;
}

static bool sna_option_accel_none(struct sna *sna)
{
	const char *s;

	if (xf86ReturnOptValBool(sna->Options, OPTION_ACCEL_DISABLE, FALSE))
		return true;

	s = xf86GetOptValString(sna->Options, OPTION_ACCEL_METHOD);
	if (s == NULL)
		return IS_DEFAULT_ACCEL_METHOD(NOACCEL);

	return strcasecmp(s, "none") == 0;
}

static bool sna_option_accel_blt(struct sna *sna)
{
	const char *s;

	s = xf86GetOptValString(sna->Options, OPTION_ACCEL_METHOD);
	/* XXX avoid render ring since it gets stuck after resume */
	if (s == NULL && sna->info->gen >= 0100)
		return true;
	if (s == NULL)
		return false;

	return strcasecmp(s, "blt") == 0;
}

bool sna_accel_init(ScreenPtr screen, struct sna *sna)
{
	const char *backend;

	DBG(("%s\n", __FUNCTION__));

	sna_font_key = AllocateFontPrivateIndex();

	list_init(&sna->flush_pixmaps);
	list_init(&sna->active_pixmaps);

	AddGeneralSocket(sna->kgem.fd);

#ifdef DEBUG_MEMORY
	sna->timer_expire[DEBUG_MEMORY_TIMER] = GetTimeInMillis()+ 10 * 1000;
#endif

	screen->defColormap = FakeClientID(0);
	/* let CreateDefColormap do whatever it wants for pixels */
	screen->blackPixel = screen->whitePixel = (Pixel) 0;
	screen->QueryBestSize = sna_query_best_size;
	assert(screen->GetImage == NULL);
	screen->GetImage = sna_get_image;
	assert(screen->GetSpans == NULL);
	screen->GetSpans = sna_get_spans;
	assert(screen->CreateWindow == NULL);
	screen->CreateWindow = sna_create_window;
	assert(screen->DestroyWindow == NULL);
	screen->DestroyWindow = sna_destroy_window;
	screen->PositionWindow = sna_position_window;
	screen->ChangeWindowAttributes = sna_change_window_attributes;
	screen->RealizeWindow = sna_map_window;
	screen->UnrealizeWindow = sna_unmap_window;
	screen->CopyWindow = sna_copy_window;
	assert(screen->CreatePixmap == NULL);
	screen->CreatePixmap = sna_create_pixmap;
	assert(screen->DestroyPixmap == NULL);
	screen->DestroyPixmap = sna_destroy_pixmap;
#ifdef CREATE_PIXMAP_USAGE_SHARED
	screen->SharePixmapBacking = sna_share_pixmap_backing;
	screen->SetSharedPixmapBacking = sna_set_shared_pixmap_backing;
#endif
	screen->RealizeFont = sna_realize_font;
	screen->UnrealizeFont = sna_unrealize_font;
	assert(screen->CreateGC == NULL);
	screen->CreateGC = sna_create_gc;
	screen->CreateColormap = miInitializeColormap;
	screen->DestroyColormap = (void (*)(ColormapPtr)) NoopDDA;
	screen->InstallColormap = miInstallColormap;
	screen->UninstallColormap = miUninstallColormap;
	screen->ListInstalledColormaps = miListInstalledColormaps;
	screen->ResolveColor = miResolveColor;
	assert(screen->StoreColors == NULL);
	screen->StoreColors = sna_store_colors;
	screen->BitmapToRegion = fbBitmapToRegion;

#if HAS_PIXMAP_SHARING
	screen->StartPixmapTracking = PixmapStartDirtyTracking;
	screen->StopPixmapTracking = PixmapStopDirtyTracking;
#endif

	assert(screen->GetWindowPixmap == NULL);
	screen->GetWindowPixmap = sna_get_window_pixmap;
	assert(screen->SetWindowPixmap == NULL);
	screen->SetWindowPixmap = sna_set_window_pixmap;

	screen->SetScreenPixmap = sna_set_screen_pixmap;

	if (sna->kgem.has_userptr)
		ShmRegisterFuncs(screen, &shm_funcs);
	else
		ShmRegisterFbFuncs(screen);

	if (!sna_picture_init(screen))
		return false;

	backend = no_render_init(sna);
	if (sna_option_accel_none(sna)) {
		backend = "disabled";
		sna->kgem.wedged = true;
	} else if (sna_option_accel_blt(sna) || sna->info->gen >= 0110)
		(void)backend;
	else if (sna->info->gen >= 0100)
		backend = gen8_render_init(sna, backend);
	else if (sna->info->gen >= 070)
		backend = gen7_render_init(sna, backend);
	else if (sna->info->gen >= 060)
		backend = gen6_render_init(sna, backend);
	else if (sna->info->gen >= 050)
		backend = gen5_render_init(sna, backend);
	else if (sna->info->gen >= 040)
		backend = gen4_render_init(sna, backend);
	else if (sna->info->gen >= 030)
		backend = gen3_render_init(sna, backend);
	else if (sna->info->gen >= 020)
		backend = gen2_render_init(sna, backend);

	DBG(("%s(backend=%s, prefer_gpu=%x)\n",
	     __FUNCTION__, backend, sna->render.prefer_gpu));

	kgem_reset(&sna->kgem);
	sigtrap_init();

	xf86DrvMsg(sna->scrn->scrnIndex, X_INFO,
		   "SNA initialized with %s backend\n",
		   backend);

	return true;
}

void sna_accel_create(struct sna *sna)
{
	DBG(("%s\n", __FUNCTION__));

	if (!sna_glyphs_create(sna))
		goto fail;

	if (!sna_gradients_create(sna))
		goto fail;

	if (!sna_composite_create(sna))
		goto fail;

	return;

fail:
	xf86DrvMsg(sna->scrn->scrnIndex, X_ERROR,
		   "Failed to allocate caches, disabling RENDER acceleration\n");
	no_render_init(sna);
}

void sna_accel_watch_flush(struct sna *sna, int enable)
{
	DBG(("%s: enable=%d\n", __FUNCTION__, enable));
	assert(enable);

	if (sna->watch_flush == 0) {
		DBG(("%s: installing watchers\n", __FUNCTION__));
		assert(enable > 0);
		if (!AddCallback(&FlushCallback, sna_accel_flush_callback, sna)) {
			xf86DrvMsg(sna->scrn->scrnIndex, X_Error,
				   "Failed to attach ourselves to the flush callbacks, expect missing synchronisation with DRI clients (e.g a compositor)\n");
		}
		sna->watch_flush++;
	}

	sna->watch_flush += enable;
}

void sna_accel_leave(struct sna *sna)
{
	DBG(("%s\n", __FUNCTION__));

	/* as root we always have permission to render */
	if (geteuid() == 0)
		return;

	/* as a user, we can only render now if we have a rendernode */
	if (intel_has_render_node(sna->scrn))
		return;

	/* no longer authorized to use our fd */
	DBG(("%s: dropping render privileges\n", __FUNCTION__));

	kgem_submit(&sna->kgem);
	sna->kgem.wedged |= 2;
}

void sna_accel_enter(struct sna *sna)
{
	DBG(("%s\n", __FUNCTION__));
	sna->kgem.wedged &= kgem_is_wedged(&sna->kgem);
	kgem_throttle(&sna->kgem);
}

void sna_accel_close(struct sna *sna)
{
	DBG(("%s\n", __FUNCTION__));

	sna_composite_close(sna);
	sna_gradients_close(sna);
	sna_glyphs_close(sna);

	sna_pixmap_expire(sna);

	DeleteCallback(&FlushCallback, sna_accel_flush_callback, sna);
	RemoveGeneralSocket(sna->kgem.fd);

	kgem_cleanup_cache(&sna->kgem);
}

void sna_accel_block_handler(struct sna *sna, struct timeval **tv)
{
	sigtrap_assert_inactive();

	if (sna->kgem.need_retire)
		kgem_retire(&sna->kgem);
	kgem_retire__buffers(&sna->kgem);

	if (sna->timer_active)
		UpdateCurrentTimeIf();

	if (sna->kgem.nbatch &&
	    (sna->kgem.scanout_busy ||
	     kgem_ring_is_idle(&sna->kgem, sna->kgem.ring))) {
		DBG(("%s: GPU idle, flushing\n", __FUNCTION__));
		_kgem_submit(&sna->kgem);
	}

restart:
	if (sna_scanout_do_flush(sna))
		sna_scanout_flush(sna);
	assert(sna_accel_scanout(sna) == NULL ||
	       !sna_accel_scanout(sna)->gpu_bo->needs_flush ||
	       sna->timer_active & (1<<(FLUSH_TIMER)));

	if (sna_accel_do_throttle(sna))
		sna_accel_throttle(sna);
	assert(!sna->kgem.need_retire ||
	       sna->timer_active & (1<<(THROTTLE_TIMER)));

	if (sna_accel_do_expire(sna))
		sna_accel_expire(sna);
	assert(!sna->kgem.need_expire ||
	       sna->timer_active & (1<<(EXPIRE_TIMER)));

	if (sna_accel_do_debug_memory(sna))
		sna_accel_debug_memory(sna);

	if (sna->watch_flush == 1) {
		DBG(("%s: removing watchers\n", __FUNCTION__));
		DeleteCallback(&FlushCallback, sna_accel_flush_callback, sna);
		sna->watch_flush = 0;
	}

	if (sna->timer_active & 1) {
		int32_t timeout;

		DBG(("%s: evaluating timers, active=%x\n",
		     __FUNCTION__, sna->timer_active));

		timeout = sna->timer_expire[FLUSH_TIMER] - TIME;
		DBG(("%s: flush timer expires in %d [%d]\n",
		     __FUNCTION__, timeout, sna->timer_expire[FLUSH_TIMER]));
		if (timeout < 3)
			goto restart;

		if (*tv == NULL) {
			*tv = &sna->timer_tv;
			goto set_tv;
		}
		if ((*tv)->tv_sec * 1000 + (*tv)->tv_usec / 1000 > timeout) {
set_tv:
			(*tv)->tv_sec = timeout / 1000;
			(*tv)->tv_usec = timeout % 1000 * 1000;
		}
	}

	sna->kgem.scanout_busy = false;

	if (FAULT_INJECTION && (rand() % FAULT_INJECTION) == 0) {
		DBG(("%s hardware acceleration\n",
		     sna->kgem.wedged ? "Re-enabling" : "Disabling"));
		kgem_submit(&sna->kgem);
		sna->kgem.wedged = !sna->kgem.wedged;
	}
}

void sna_accel_wakeup_handler(struct sna *sna)
{
	DBG(("%s: nbatch=%d, need_retire=%d, need_purge=%d\n", __FUNCTION__,
	     sna->kgem.nbatch, sna->kgem.need_retire, sna->kgem.need_purge));

	if (!sna->kgem.nbatch)
		return;

	if (kgem_is_idle(&sna->kgem)) {
		DBG(("%s: GPU idle, flushing\n", __FUNCTION__));
		_kgem_submit(&sna->kgem);
	}

	sigtrap_assert_inactive();
}

void sna_accel_free(struct sna *sna)
{
	DBG(("%s\n", __FUNCTION__));
	sigtrap_assert_inactive();
}
