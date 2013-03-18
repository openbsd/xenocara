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

#include "intel_options.h"
#include "sna.h"
#include "sna_reg.h"
#include "rop.h"

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

#define FORCE_INPLACE 0
#define FORCE_FALLBACK 0
#define FORCE_FLUSH 0

#define DEFAULT_TILING I915_TILING_X

#define USE_INPLACE 1
#define USE_WIDE_SPANS 0 /* -1 force CPU, 1 force GPU */
#define USE_ZERO_SPANS 1 /* -1 force CPU, 1 force GPU */
#define USE_CPU_BO 1
#define USE_USERPTR_UPLOADS 1
#define USE_USERPTR_DOWNLOADS 1

#define MIGRATE_ALL 0
#define DBG_NO_CPU_UPLOAD 0
#define DBG_NO_CPU_DOWNLOAD 0

#define ACCEL_FILL_SPANS 1
#define ACCEL_SET_SPANS 1
#define ACCEL_PUT_IMAGE 1
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

#define IS_STATIC_PTR(ptr) ((uintptr_t)(ptr) & 1)
#define MAKE_STATIC_PTR(ptr) ((void*)((uintptr_t)(ptr) | 1))

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
	sna_read_boxes(sna,
		       priv->gpu_bo, 0, 0,
		       tmp, 0, 0,
		       &box, 1);

	src = pixmap->devPrivate.ptr;
	dst = tmp->devPrivate.ptr;
	for (i = 0; i < tmp->drawable.height; i++) {
		if (memcmp(src, dst, tmp->drawable.width * tmp->drawable.bitsPerPixel >> 3)) {
			for (j = 0; src[j] == dst[j]; j++)
				;
			ErrorF("mismatch at (%d, %d)\n",
			       8*j / tmp->drawable.bitsPerPixel, i);
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

static inline void region_set(RegionRec *r, const BoxRec *b)
{
	r->extents = *b;
	r->data = NULL;
}

static inline void region_maybe_clip(RegionRec *r, RegionRec *clip)
{
	if (clip->data)
		RegionIntersect(r, r, clip);
}

static inline bool region_is_singular(const RegionRec *r)
{
	return r->data == NULL;
}

typedef struct box32 {
	int32_t x1, y1, x2, y2;
} Box32Rec;

#define PM_IS_SOLID(_draw, _pm) \
	(((_pm) & FbFullMask((_draw)->depth)) == FbFullMask((_draw)->depth))

#ifdef DEBUG_PIXMAP
static void _assert_pixmap_contains_box(PixmapPtr pixmap, const BoxRec *box, const char *function)
{
	if (box->x1 < 0 || box->y1 < 0 ||
	    box->x2 > pixmap->drawable.width ||
	    box->y2 > pixmap->drawable.height)
	{
		ErrorF("%s: damage box is beyond the pixmap: box=(%d, %d), (%d, %d), pixmap=(%d, %d)\n",
		       __FUNCTION__,
		       box->x1, box->y1, box->x2, box->y2,
		       pixmap->drawable.width,
		       pixmap->drawable.height);
		assert(0);
	}
}

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
		ErrorF("%s: damage box is beyond the drawable: box=(%d, %d), (%d, %d), drawable=(%d, %d)x(%d, %d)\n",
		       __FUNCTION__,
		       box->x1, box->y1, box->x2, box->y2,
		       drawable->x, drawable->y,
		       drawable->width, drawable->height);
		assert(0);
	}
}

static void assert_pixmap_damage(PixmapPtr p)
{
	struct sna_pixmap *priv;
	RegionRec reg, cpu, gpu;

	priv = sna_pixmap(p);
	if (priv == NULL)
		return;

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
#define assert_pixmap_damage(p)
#endif

inline static bool
sna_fill_init_blt(struct sna_fill_op *fill,
		  struct sna *sna,
		  PixmapPtr pixmap,
		  struct kgem_bo *bo,
		  uint8_t alu,
		  uint32_t pixel)
{
	return sna->render.fill(sna, alu, pixmap, bo, pixel, fill);
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
	sna_damage_destroy(&priv->gpu_damage);
	priv->clear = false;

	if (priv->gpu_bo && !priv->pinned) {
		kgem_bo_destroy(&sna->kgem, priv->gpu_bo);
		priv->gpu_bo = NULL;
	}

	if (priv->mapped) {
		assert(!priv->shm);
		priv->pixmap->devPrivate.ptr = NULL;
		priv->mapped = false;
	}

	/* and reset the upload counter */
	priv->source_count = SOURCE_BIAS;
}

static bool must_check
sna_pixmap_alloc_cpu(struct sna *sna,
		     PixmapPtr pixmap,
		     struct sna_pixmap *priv,
		     bool from_gpu)
{
	/* Restore after a GTT mapping? */
	assert(!priv->shm);
	if (priv->ptr)
		goto done;

	DBG(("%s: pixmap=%ld\n", __FUNCTION__, pixmap->drawable.serialNumber));
	assert(priv->stride);

	if (priv->create & KGEM_CAN_CREATE_CPU) {
		DBG(("%s: allocating CPU buffer (%dx%d)\n", __FUNCTION__,
		     pixmap->drawable.width, pixmap->drawable.height));

		priv->cpu_bo = kgem_create_cpu_2d(&sna->kgem,
						  pixmap->drawable.width,
						  pixmap->drawable.height,
						  pixmap->drawable.bitsPerPixel,
						  from_gpu ? 0 : CREATE_CPU_MAP | CREATE_INACTIVE | CREATE_NO_THROTTLE);
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

	assert(priv->ptr);
done:
	assert(priv->stride);
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
		if (!priv->cpu_bo->reusable) {
			assert(priv->cpu_bo->flush == true);
			kgem_bo_sync__cpu(&sna->kgem, priv->cpu_bo);
			sna_accel_watch_flush(sna, -1);
		}
		kgem_bo_destroy(&sna->kgem, priv->cpu_bo);
	} else if (!IS_STATIC_PTR(priv->ptr))
		free(priv->ptr);
}

static void sna_pixmap_free_cpu(struct sna *sna, struct sna_pixmap *priv)
{
	assert(priv->cpu_damage == NULL);
	assert(list_is_empty(&priv->list));

	if (IS_STATIC_PTR(priv->ptr))
		return;

	__sna_pixmap_free_cpu(sna, priv);

	priv->cpu_bo = NULL;
	priv->ptr = NULL;

	if (!priv->mapped)
		priv->pixmap->devPrivate.ptr = NULL;
}

static inline uint32_t default_tiling(PixmapPtr pixmap,
				      uint32_t tiling)
{
	struct sna_pixmap *priv = sna_pixmap(pixmap);
	struct sna *sna = to_sna_from_pixmap(pixmap);

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

	if (tiling == I915_TILING_Y &&
	    sna_damage_is_all(&priv->cpu_damage,
			      pixmap->drawable.width,
			      pixmap->drawable.height)) {
		DBG(("%s: entire source is damaged, using Y-tiling\n",
		     __FUNCTION__));
		sna_damage_destroy(&priv->gpu_damage);

		return I915_TILING_Y;
	}

	return tiling;
}

constant static uint32_t sna_pixmap_choose_tiling(PixmapPtr pixmap,
						  uint32_t tiling)
{
	struct sna *sna = to_sna_from_pixmap(pixmap);
	uint32_t bit;

	/* Use tiling by default, but disable per user request */
	if (pixmap->usage_hint == SNA_CREATE_FB) {
		tiling = -I915_TILING_X;
		bit = SNA_TILING_FB;
	} else {
		tiling = default_tiling(pixmap, tiling);
		bit = SNA_TILING_2D;
	}
	if ((sna->tiling && (1 << bit)) == 0)
		tiling = I915_TILING_NONE;

	/* Also adjust tiling if it is not supported or likely to
	 * slow us down,
	 */
	return kgem_choose_tiling(&sna->kgem, tiling,
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

	if (priv->pinned) {
		DBG(("%s: can't convert pinned bo\n", __FUNCTION__));
		return NULL;
	}

	if (wedged(sna)) {
		DBG(("%s: can't convert bo, wedged\n", __FUNCTION__));
		return NULL;
	}

	assert_pixmap_damage(pixmap);

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
				    pixmap, priv->gpu_bo, 0, 0,
				    pixmap, bo, 0, 0,
				    &box, 1, 0)) {
		DBG(("%s: copy failed\n", __FUNCTION__));
		kgem_bo_destroy(&sna->kgem, bo);
		return NULL;
	}

	kgem_bo_destroy(&sna->kgem, priv->gpu_bo);

	if (priv->mapped) {
		assert(!priv->shm);
		pixmap->devPrivate.ptr = NULL;
		priv->mapped = false;
	}

	return priv->gpu_bo = bo;
}

static inline void sna_set_pixmap(PixmapPtr pixmap, struct sna_pixmap *sna)
{
	((void **)dixGetPrivateAddr(&pixmap->devPrivates, &sna_pixmap_key))[1] = sna;
	assert(sna_pixmap(pixmap) == sna);
}

static struct sna_pixmap *
_sna_pixmap_init(struct sna_pixmap *priv, PixmapPtr pixmap)
{
	list_init(&priv->list);
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

bool sna_pixmap_attach_to_bo(PixmapPtr pixmap, struct kgem_bo *bo)
{
	struct sna_pixmap *priv;

	priv = sna_pixmap_attach(pixmap);
	if (!priv)
		return false;

	priv->gpu_bo = kgem_bo_reference(bo);
	assert(priv->gpu_bo->proxy == NULL);
	sna_damage_all(&priv->gpu_damage,
		       pixmap->drawable.width,
		       pixmap->drawable.height);

	return true;
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
	if (base & 15) {
		int adjust = 16 - (base & 15);
		base += adjust;
		datasize += adjust;
	}

	pixmap = AllocatePixmap(screen, datasize);
	if (!pixmap)
		return NullPixmap;

	((void **)dixGetPrivateAddr(&pixmap->devPrivates, &sna_pixmap_key))[0] = sna;
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
	pixmap->devPrivate.ptr =  (char *)pixmap + base;

#ifdef COMPOSITE
	pixmap->screen_x = 0;
	pixmap->screen_y = 0;
#endif

	pixmap->usage_hint = usage_hint;

	DBG(("%s: serial=%ld, usage=%d, %dx%d\n",
	     __FUNCTION__,
	     pixmap->drawable.serialNumber,
	     pixmap->usage_hint,
	     pixmap->drawable.width,
	     pixmap->drawable.height));

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

	if (sna->freed_pixmap) {
		pixmap = sna->freed_pixmap;
		sna->freed_pixmap = pixmap->devPrivate.ptr;

		pixmap->usage_hint = 0;
		pixmap->refcnt = 1;

		pixmap->drawable.width = width;
		pixmap->drawable.height = height;
		pixmap->drawable.depth = depth;
		pixmap->drawable.bitsPerPixel = bpp;
		pixmap->drawable.serialNumber = NEXT_SERIAL_NUMBER;

		DBG(("%s: serial=%ld, %dx%d\n",
		     __FUNCTION__,
		     pixmap->drawable.serialNumber,
		     pixmap->drawable.width,
		     pixmap->drawable.height));

		priv = _sna_pixmap_reset(pixmap);
	} else {
		pixmap = create_pixmap(sna, screen, 0, 0, depth, 0);
		if (pixmap == NullPixmap)
			return NullPixmap;

		pixmap->drawable.width = width;
		pixmap->drawable.height = height;
		pixmap->drawable.depth = depth;
		pixmap->drawable.bitsPerPixel = bpp;

		priv = sna_pixmap_attach(pixmap);
		if (!priv) {
			FreePixmap(pixmap);
			return NullPixmap;
		}
	}

	priv->cpu_bo = kgem_create_map(&sna->kgem, addr, pitch*height, false);
	if (priv->cpu_bo == NULL) {
		priv->header = true;
		sna_pixmap_destroy(pixmap);
		goto fallback;
	}
	priv->cpu_bo->flush = true;
	priv->cpu_bo->pitch = pitch;
	priv->cpu_bo->reusable = false;
	sna_accel_watch_flush(sna, 1);
#ifdef DEBUG_MEMORY
	sna->debug_memory.cpu_bo_allocs++;
	sna->debug_memory.cpu_bo_bytes += kgem_bo_size(priv->cpu_bo);
#endif

	priv->cpu = true;
	priv->shm = true;
	priv->stride = pitch;
	priv->ptr = MAKE_STATIC_PTR(addr);
	sna_damage_all(&priv->cpu_damage, width, height);

	pixmap->devKind = pitch;
	pixmap->devPrivate.ptr = addr;
	return pixmap;
}

PixmapPtr
sna_pixmap_create_unattached(ScreenPtr screen,
			     int width, int height, int depth)
{
	return create_pixmap(to_sna_from_screen(screen),
			     screen, width, height, depth,
			     CREATE_PIXMAP_USAGE_SCRATCH);
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
	if (tiling == I915_TILING_Y && !sna->have_render)
		tiling = I915_TILING_X;

	if (tiling == I915_TILING_Y &&
	    (width > sna->render.max_3d_size ||
	     height > sna->render.max_3d_size))
		tiling = I915_TILING_X;

	tiling = kgem_choose_tiling(&sna->kgem, tiling, width, height, bpp);

	/* you promise never to access this via the cpu... */
	if (sna->freed_pixmap) {
		pixmap = sna->freed_pixmap;
		sna->freed_pixmap = pixmap->devPrivate.ptr;

		pixmap->usage_hint = CREATE_PIXMAP_USAGE_SCRATCH;
		pixmap->refcnt = 1;

		pixmap->drawable.width = width;
		pixmap->drawable.height = height;
		pixmap->drawable.depth = depth;
		pixmap->drawable.bitsPerPixel = bpp;
		pixmap->drawable.serialNumber = NEXT_SERIAL_NUMBER;

		DBG(("%s: serial=%ld, usage=%d, %dx%d\n",
		     __FUNCTION__,
		     pixmap->drawable.serialNumber,
		     pixmap->usage_hint,
		     pixmap->drawable.width,
		     pixmap->drawable.height));

		priv = _sna_pixmap_reset(pixmap);
	} else {
		pixmap = create_pixmap(sna, screen, 0, 0, depth,
				       CREATE_PIXMAP_USAGE_SCRATCH);
		if (pixmap == NullPixmap)
			return NullPixmap;

		pixmap->drawable.width = width;
		pixmap->drawable.height = height;
		pixmap->drawable.depth = depth;
		pixmap->drawable.bitsPerPixel = bpp;

		priv = sna_pixmap_attach(pixmap);
		if (!priv) {
			FreePixmap(pixmap);
			return NullPixmap;
		}
	}

	priv->stride = PixmapBytePad(width, depth);
	pixmap->devPrivate.ptr = NULL;

	priv->gpu_bo = kgem_create_2d(&sna->kgem,
				      width, height, bpp, tiling,
				      CREATE_TEMPORARY);
	if (priv->gpu_bo == NULL) {
		free(priv);
		FreePixmap(pixmap);
		return NullPixmap;
	}

	priv->header = true;
	sna_damage_all(&priv->gpu_damage, width, height);

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

		if (priv->pinned & ~(PIN_DRI | PIN_PRIME)) {
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
					    pixmap, priv->gpu_bo, 0, 0,
					    pixmap, bo, 0, 0,
					    &box, 1, 0)) {
			DBG(("%s: copy failed\n", __FUNCTION__));
			kgem_bo_destroy(&sna->kgem, bo);
			return FALSE;
		}

		kgem_bo_destroy(&sna->kgem, priv->gpu_bo);
		priv->gpu_bo = bo;

		if (priv->mapped) {
			pixmap->devPrivate.ptr = NULL;
			priv->mapped = false;
		}
	}
	assert(priv->gpu_bo->tiling == I915_TILING_NONE);
	assert((priv->gpu_bo->pitch & 255) == 0);

	/* And export the bo->pitch via pixmap->devKind */
	pixmap->devPrivate.ptr = kgem_bo_map__async(&sna->kgem, priv->gpu_bo);
	if (pixmap->devPrivate.ptr == NULL)
		return FALSE;

	pixmap->devKind = priv->gpu_bo->pitch;
	priv->mapped = true;

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

	sna_damage_all(&priv->gpu_damage,
		       pixmap->drawable.width,
		       pixmap->drawable.height);

	bo->pitch = pixmap->devKind;
	priv->stride = pixmap->devKind;

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
	pixmap = create_pixmap(sna, screen, 0, 0, depth, 0);
	if (pixmap == NullPixmap)
		return NullPixmap;

	pixmap->devKind = 0;
	pixmap->devPrivate.ptr = NULL;

	priv = sna_pixmap_attach(pixmap);
	if (priv == NULL) {
		free(pixmap);
		return NullPixmap;
	}

	priv->stride = 0;
	priv->create = 0;

	if (width|height) {
		int bpp = bits_per_pixel(depth);

		priv->gpu_bo = kgem_create_2d(&sna->kgem,
					      width, height, bpp,
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
			free(priv);
			FreePixmap(pixmap);
			return FALSE;
		}

		pixmap->devKind = priv->gpu_bo->pitch;
		pixmap->drawable.width = width;
		pixmap->drawable.height = height;

		priv->stride = priv->gpu_bo->pitch;
		priv->mapped = true;

		sna_damage_all(&priv->gpu_damage, width, height);
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

	if (unlikely(!sna->have_render))
		flags &= ~KGEM_CAN_CREATE_GPU;
	if (wedged(sna))
		flags = 0;

	switch (usage) {
	case CREATE_PIXMAP_USAGE_SCRATCH:
		if (flags & KGEM_CAN_CREATE_GPU)
			return sna_pixmap_create_scratch(screen,
							 width, height, depth,
							 I915_TILING_X);
		else
			goto fallback;

	case SNA_CREATE_GLYPHS:
		if (flags & KGEM_CAN_CREATE_GPU)
			return sna_pixmap_create_scratch(screen,
							 width, height, depth,
							 -I915_TILING_Y);
		else
			goto fallback;

	case SNA_CREATE_SCRATCH:
		if (flags & KGEM_CAN_CREATE_GPU)
			return sna_pixmap_create_scratch(screen,
							 width, height, depth,
							 I915_TILING_Y);
		else
			goto fallback;
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
	} else {
		DBG(("%s: creating GPU pixmap %dx%d, stride=%d, flags=%x\n",
		     __FUNCTION__, width, height, pad, flags));

		pixmap = create_pixmap(sna, screen, 0, 0, depth, usage);
		if (pixmap == NullPixmap)
			return NullPixmap;

		pixmap->drawable.width = width;
		pixmap->drawable.height = height;
		pixmap->devKind = pad;
		pixmap->devPrivate.ptr = NULL;

		ptr = NULL;
	}

	priv = sna_pixmap_attach(pixmap);
	if (priv == NULL) {
		free(pixmap);
		goto fallback;
	}

	priv->stride = pad;
	priv->create = flags;
	priv->ptr = ptr;

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
	list_move(&priv->list, &sna->flush_pixmaps);

	if (bo->exec == NULL) {
		DBG(("%s: new flush bo, flushin before\n", __FUNCTION__));
		kgem_submit(&sna->kgem);
	}
}

static void __sna_free_pixmap(struct sna *sna,
			      PixmapPtr pixmap,
			      struct sna_pixmap *priv)
{
	list_del(&priv->list);

	sna_damage_destroy(&priv->gpu_damage);
	sna_damage_destroy(&priv->cpu_damage);

	__sna_pixmap_free_cpu(sna, priv);

	if (priv->header) {
		assert(!priv->shm);
		pixmap->devPrivate.ptr = sna->freed_pixmap;
		sna->freed_pixmap = pixmap;
	} else {
		free(priv);
		FreePixmap(pixmap);
	}
}

static Bool sna_destroy_pixmap(PixmapPtr pixmap)
{
	struct sna *sna;
	struct sna_pixmap *priv;

	if (--pixmap->refcnt)
		return TRUE;

	priv = sna_pixmap(pixmap);
	DBG(("%s: pixmap=%ld, attached?=%d\n",
	     __FUNCTION__, pixmap->drawable.serialNumber, priv != NULL));
	if (priv == NULL) {
		FreePixmap(pixmap);
		return TRUE;
	}

	assert_pixmap_damage(pixmap);
	sna = to_sna_from_pixmap(pixmap);

	/* Always release the gpu bo back to the lower levels of caching */
	if (priv->gpu_bo) {
		kgem_bo_destroy(&sna->kgem, priv->gpu_bo);
		priv->gpu_bo = NULL;
	}

	if (priv->shm && kgem_bo_is_busy(priv->cpu_bo)) {
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

static inline bool pixmap_inplace(struct sna *sna,
				  PixmapPtr pixmap,
				  struct sna_pixmap *priv,
				  bool write_only)
{
	if (FORCE_INPLACE)
		return FORCE_INPLACE > 0;

	if (wedged(sna) && !priv->pinned)
		return false;

	if (priv->mapped)
		return !IS_CPU_MAP(priv->gpu_bo->map);

	if (!write_only && priv->cpu_damage)
		return false;

	return (pixmap->devKind * pixmap->drawable.height >> 12) >
		sna->kgem.half_cpu_cache_pages;
}

static bool
sna_pixmap_create_mappable_gpu(PixmapPtr pixmap)
{
	struct sna *sna = to_sna_from_pixmap(pixmap);
	struct sna_pixmap *priv = sna_pixmap(pixmap);;

	if (wedged(sna))
		return false;

	if ((priv->create & KGEM_CAN_CREATE_GTT) == 0)
		return false;

	assert_pixmap_damage(pixmap);

	assert(priv->gpu_bo == NULL);
	priv->gpu_bo =
		kgem_create_2d(&sna->kgem,
			       pixmap->drawable.width,
			       pixmap->drawable.height,
			       pixmap->drawable.bitsPerPixel,
			       sna_pixmap_choose_tiling(pixmap, DEFAULT_TILING),
			       CREATE_GTT_MAP | CREATE_INACTIVE);

	return priv->gpu_bo && kgem_bo_is_mappable(&sna->kgem, priv->gpu_bo);
}

static inline bool use_cpu_bo_for_download(struct sna *sna,
					   struct sna_pixmap *priv,
					   const BoxRec *box)
{
	if (DBG_NO_CPU_DOWNLOAD)
		return false;

	if (wedged(sna))
		return false;

	if (priv->cpu_bo == NULL || !sna->kgem.can_blt_cpu)
		return false;

	if (kgem_bo_is_busy(priv->gpu_bo) || kgem_bo_is_busy(priv->cpu_bo)) {
		DBG(("%s: yes, either bo is busy, so use GPU for readback\n",
		     __FUNCTION__));
		return true;
	}

	/* Is it worth detiling? */
	if (kgem_bo_is_mappable(&sna->kgem, priv->gpu_bo) &&
	    (box->y2 - box->y1 - 1) * priv->gpu_bo->pitch < 4096) {
		DBG(("%s: no, tiny transfer, expect to read inplace\n",
		     __FUNCTION__));
		return false;
	}

	DBG(("%s: yes, default action\n", __FUNCTION__));
	return true;
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

static inline bool operate_inplace(struct sna_pixmap *priv, unsigned flags)
{
	if ((flags & MOVE_INPLACE_HINT) == 0) {
		DBG(("%s: no, inplace operation not suitable\n", __FUNCTION__));
		return false;
	}

	assert((flags & MOVE_ASYNC_HINT) == 0);

	if ((priv->create & KGEM_CAN_CREATE_GTT) == 0) {
		DBG(("%s: no, not accessible via GTT\n", __FUNCTION__));
		return false;
	}

	if (priv->cpu_bo && kgem_bo_is_busy(priv->cpu_bo)) {
		DBG(("%s: yes, CPU is busy\n", __FUNCTION__));
		return true;
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

	if (USE_INPLACE && (flags & MOVE_READ) == 0) {
		assert(flags & MOVE_WRITE);
		DBG(("%s: no readbck, discarding gpu damage [%d], pending clear[%d]\n",
		     __FUNCTION__, priv->gpu_damage != NULL, priv->clear));

		if (priv->create & KGEM_CAN_CREATE_GPU &&
		    pixmap_inplace(sna, pixmap, priv, true)) {
			assert(!priv->shm);
			DBG(("%s: write inplace\n", __FUNCTION__));
			if (priv->gpu_bo) {
				if (__kgem_bo_is_busy(&sna->kgem,
						      priv->gpu_bo)) {
					if (priv->pinned)
						goto skip_inplace_map;

					DBG(("%s: discard busy GPU bo\n", __FUNCTION__));
					sna_pixmap_free_gpu(sna, priv);
				}
			}
			if (priv->gpu_bo == NULL &&
			    !sna_pixmap_create_mappable_gpu(pixmap))
				goto skip_inplace_map;

			if (!priv->mapped) {
				pixmap->devPrivate.ptr =
					kgem_bo_map(&sna->kgem, priv->gpu_bo);
				if (pixmap->devPrivate.ptr == NULL)
					goto skip_inplace_map;

				priv->mapped = true;
			}
			pixmap->devKind = priv->gpu_bo->pitch;

			assert(priv->gpu_bo->proxy == NULL);
			sna_damage_all(&priv->gpu_damage,
				       pixmap->drawable.width,
				       pixmap->drawable.height);
			sna_damage_destroy(&priv->cpu_damage);
			priv->clear = false;
			priv->cpu = false;
			list_del(&priv->list);

			assert(!priv->shm);
			assert(priv->cpu_bo == NULL || !priv->cpu_bo->flush);
			sna_pixmap_free_cpu(sna, priv);

			assert_pixmap_damage(pixmap);
			return true;
		}

skip_inplace_map:
		sna_damage_destroy(&priv->gpu_damage);
		priv->clear = false;
		if (priv->cpu_bo && !priv->cpu_bo->flush &&
		    __kgem_bo_is_busy(&sna->kgem, priv->cpu_bo)) {
			DBG(("%s: discarding busy CPU bo\n", __FUNCTION__));
			assert(!priv->shm);
			assert(priv->gpu_bo == NULL || priv->gpu_damage == NULL);

			sna_damage_destroy(&priv->cpu_damage);

			sna_pixmap_free_gpu(sna, priv);
			sna_pixmap_free_cpu(sna, priv);

			if (!sna_pixmap_alloc_cpu(sna, pixmap, priv, false))
				return false;

			sna_damage_all(&priv->cpu_damage,
					pixmap->drawable.width,
					pixmap->drawable.height);
		}
	}

	if (DAMAGE_IS_ALL(priv->cpu_damage)) {
		DBG(("%s: CPU all-damaged\n", __FUNCTION__));
		goto done;
	}

	assert(priv->gpu_bo == NULL || priv->gpu_bo->proxy == NULL);

	if (operate_inplace(priv, flags) &&
	    pixmap_inplace(sna, pixmap, priv, (flags & MOVE_READ) == 0) &&
	    (priv->gpu_bo || sna_pixmap_create_mappable_gpu(pixmap))) {
		kgem_bo_submit(&sna->kgem, priv->gpu_bo);

		DBG(("%s: try to operate inplace (GTT)\n", __FUNCTION__));
		assert(priv->cpu == false);

		pixmap->devPrivate.ptr =
			kgem_bo_map(&sna->kgem, priv->gpu_bo);
		if (pixmap->devPrivate.ptr != NULL) {
			priv->mapped = true;
			pixmap->devKind = priv->gpu_bo->pitch;
			if (flags & MOVE_WRITE) {
				assert(priv->gpu_bo->proxy == NULL);
				sna_damage_all(&priv->gpu_damage,
					       pixmap->drawable.width,
					       pixmap->drawable.height);
				sna_damage_destroy(&priv->cpu_damage);
				sna_pixmap_free_cpu(sna, priv);
				list_del(&priv->list);
				priv->clear = false;
			}

			assert_pixmap_damage(pixmap);
			DBG(("%s: operate inplace (GTT)\n", __FUNCTION__));
			return true;
		}

		priv->mapped = false;
	}

	if (priv->mapped) {
		assert(!priv->shm);
		pixmap->devPrivate.ptr = PTR(priv->ptr);
		pixmap->devKind = priv->stride;
		priv->mapped = false;
	}

	if (priv->gpu_damage &&
	    ((flags & MOVE_ASYNC_HINT) == 0 ||
	     !__kgem_bo_is_busy(&sna->kgem, priv->gpu_bo)) &&
	    priv->gpu_bo->tiling == I915_TILING_NONE &&
	    sna_pixmap_move_to_gpu(pixmap, MOVE_READ)) {
		kgem_bo_submit(&sna->kgem, priv->gpu_bo);

		DBG(("%s: try to operate inplace (CPU)\n", __FUNCTION__));

		pixmap->devPrivate.ptr =
			kgem_bo_map__cpu(&sna->kgem, priv->gpu_bo);
		if (pixmap->devPrivate.ptr != NULL) {
			priv->cpu = true;
			priv->mapped = true;
			pixmap->devKind = priv->gpu_bo->pitch;
			if (flags & MOVE_WRITE) {
				assert(priv->gpu_bo->proxy == NULL);
				sna_damage_all(&priv->gpu_damage,
					       pixmap->drawable.width,
					       pixmap->drawable.height);
				sna_damage_destroy(&priv->cpu_damage);
				sna_pixmap_free_cpu(sna, priv);
				list_del(&priv->list);
				priv->clear = false;
			}

			kgem_bo_sync__cpu_full(&sna->kgem,
					       priv->gpu_bo, flags & MOVE_WRITE);
			assert_pixmap_damage(pixmap);
			DBG(("%s: operate inplace (CPU)\n", __FUNCTION__));
			return true;
		}
	}

	if (((flags & MOVE_READ) == 0 || priv->clear) &&
	    priv->cpu_bo && !priv->cpu_bo->flush &&
	    __kgem_bo_is_busy(&sna->kgem, priv->cpu_bo)) {
		assert(!priv->shm);
		sna_pixmap_free_cpu(sna, priv);
	}

	if (pixmap->devPrivate.ptr == NULL &&
	    !sna_pixmap_alloc_cpu(sna, pixmap, priv,
				  flags & MOVE_READ ? priv->gpu_damage && !priv->clear : 0))
		return false;

	if (priv->clear) {
		DBG(("%s: applying clear [%08x]\n",
		     __FUNCTION__, priv->clear_color));

		if (priv->cpu_bo) {
			DBG(("%s: syncing CPU bo\n", __FUNCTION__));
			kgem_bo_sync__cpu(&sna->kgem, priv->cpu_bo);
		}

		if (priv->clear_color == 0 || pixmap->drawable.bitsPerPixel == 8) {
			memset(pixmap->devPrivate.ptr, priv->clear_color,
			       pixmap->devKind * pixmap->drawable.height);
		} else {
			pixman_fill(pixmap->devPrivate.ptr,
				    pixmap->devKind/sizeof(uint32_t),
				    pixmap->drawable.bitsPerPixel,
				    0, 0,
				    pixmap->drawable.width,
				    pixmap->drawable.height,
				    priv->clear_color);
		}

		sna_damage_all(&priv->cpu_damage,
			       pixmap->drawable.width,
			       pixmap->drawable.height);
		sna_pixmap_free_gpu(sna, priv);
		assert(priv->gpu_damage == NULL);
		priv->clear = false;
	}

	if (priv->gpu_damage) {
		BoxPtr box;
		int n;

		DBG(("%s: flushing GPU damage\n", __FUNCTION__));

		n = sna_damage_get_boxes(priv->gpu_damage, &box);
		if (n) {
			bool ok = false;

			if (use_cpu_bo_for_download(sna, priv, &priv->gpu_damage->extents)) {
				DBG(("%s: using CPU bo for download from GPU\n", __FUNCTION__));
				ok = sna->render.copy_boxes(sna, GXcopy,
							    pixmap, priv->gpu_bo, 0, 0,
							    pixmap, priv->cpu_bo, 0, 0,
							    box, n, COPY_LAST);
			}
			if (!ok)
				sna_read_boxes(sna,
					       priv->gpu_bo, 0, 0,
					       pixmap, 0, 0,
					       box, n);
		}

		__sna_damage_destroy(DAMAGE_PTR(priv->gpu_damage));
		priv->gpu_damage = NULL;
	}

	if (flags & MOVE_WRITE || priv->create & KGEM_CAN_CREATE_LARGE) {
		DBG(("%s: marking as damaged\n", __FUNCTION__));
		sna_damage_all(&priv->cpu_damage,
			       pixmap->drawable.width,
			       pixmap->drawable.height);
		sna_pixmap_free_gpu(sna, priv);

		if (priv->flush) {
			assert(!priv->shm);
			sna_add_flush_pixmap(sna, priv, priv->gpu_bo);
		}
	}

done:
	if (flags & MOVE_WRITE) {
		assert(DAMAGE_IS_ALL(priv->cpu_damage));
		priv->source_count = SOURCE_BIAS;
		assert(priv->gpu_bo == NULL || priv->gpu_bo->proxy == NULL);
		if (priv->gpu_bo && priv->gpu_bo->domain != DOMAIN_GPU) {
			DBG(("%s: discarding inactive GPU bo\n", __FUNCTION__));
			sna_pixmap_free_gpu(sna, priv);
		}
	}

	if (priv->cpu_bo) {
		if ((flags & MOVE_ASYNC_HINT) == 0) {
			DBG(("%s: syncing CPU bo\n", __FUNCTION__));
			kgem_bo_sync__cpu_full(&sna->kgem,
					       priv->cpu_bo, flags & MOVE_WRITE);
		}
		if (flags & MOVE_WRITE) {
			DBG(("%s: discarding GPU bo in favour of CPU bo\n", __FUNCTION__));
			sna_pixmap_free_gpu(sna, priv);
		}
	}
	priv->cpu = (flags & MOVE_ASYNC_HINT) == 0;
	assert(pixmap->devPrivate.ptr);
	assert(pixmap->devKind);
	assert_pixmap_damage(pixmap);
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

#ifndef NDEBUG
static bool
pixmap_contains_damage(PixmapPtr pixmap, struct sna_damage *damage)
{
	if (damage == NULL)
		return true;

	damage = DAMAGE_PTR(damage);
	return (damage->extents.x2 <= pixmap->drawable.width &&
		damage->extents.y2 <= pixmap->drawable.height &&
		damage->extents.x1 >= 0 &&
		damage->extents.y1 >= 0);
}
#endif

static inline bool region_inplace(struct sna *sna,
				  PixmapPtr pixmap,
				  RegionPtr region,
				  struct sna_pixmap *priv,
				  bool write_only)
{
	assert_pixmap_damage(pixmap);

	if (FORCE_INPLACE)
		return FORCE_INPLACE > 0;

	if (wedged(sna) && !priv->pinned)
		return false;

	if ((priv->cpu || !write_only) &&
	    region_overlaps_damage(region, priv->cpu_damage, 0, 0)) {
		DBG(("%s: no, uncovered CPU damage pending\n", __FUNCTION__));
		return false;
	}

	if (priv->flush) {
		DBG(("%s: yes, exported via dri, will flush\n", __FUNCTION__));
		return true;
	}

	if (priv->cpu) {
		DBG(("%s: no, preferring last action of CPU\n", __FUNCTION__));
		return false;
	}

	if (priv->mapped) {
		DBG(("%s: yes, already mapped, continuiung\n", __FUNCTION__));
		return !IS_CPU_MAP(priv->gpu_bo->map);
	}

	if (DAMAGE_IS_ALL(priv->gpu_damage)) {
		DBG(("%s: yes, already wholly damaged on the GPU\n", __FUNCTION__));
		return true;
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

bool
sna_drawable_move_region_to_cpu(DrawablePtr drawable,
				RegionPtr region,
				unsigned flags)
{
	PixmapPtr pixmap = get_drawable_pixmap(drawable);
	struct sna *sna = to_sna_from_pixmap(pixmap);
	struct sna_pixmap *priv;
	int16_t dx, dy;

	DBG(("%s(pixmap=%ld (%dx%d), [(%d, %d), (%d, %d)], flags=%d)\n",
	     __FUNCTION__, pixmap->drawable.serialNumber,
	     pixmap->drawable.width, pixmap->drawable.height,
	     RegionExtents(region)->x1, RegionExtents(region)->y1,
	     RegionExtents(region)->x2, RegionExtents(region)->y2,
	     flags));

	assert_pixmap_damage(pixmap);
	if (flags & MOVE_WRITE) {
		assert_drawable_contains_box(drawable, &region->extents);
	}

	priv = sna_pixmap(pixmap);
	if (priv == NULL) {
		DBG(("%s: not attached to %p\n", __FUNCTION__, pixmap));
		return true;
	}

	if (sna_damage_is_all(&priv->cpu_damage,
			      pixmap->drawable.width,
			      pixmap->drawable.height)) {
		DBG(("%s: pixmap=%ld all damaged on CPU\n",
		     __FUNCTION__, pixmap->drawable.serialNumber));

		sna_damage_destroy(&priv->gpu_damage);

		if (flags & MOVE_WRITE)
			sna_pixmap_free_gpu(sna, priv);

		if (pixmap->devPrivate.ptr == NULL &&
		    !sna_pixmap_alloc_cpu(sna, pixmap, priv, false))
			return false;

		goto out;
	}

	if (USE_INPLACE &&
	    (flags & MOVE_READ) == 0 &&
	    (priv->flush || box_inplace(pixmap, &region->extents))) {
		DBG(("%s: marking for inplace hint (%d, %d)\n",
		     __FUNCTION__, priv->flush, box_inplace(pixmap, &region->extents)));
		flags |= MOVE_INPLACE_HINT;
	}

	if (flags & MOVE_WHOLE_HINT)
		return _sna_pixmap_move_to_cpu(pixmap, flags);

	if (priv->gpu_bo == NULL &&
	    (priv->create & KGEM_CAN_CREATE_GPU) == 0 &&
	    flags & MOVE_WRITE)
		return _sna_pixmap_move_to_cpu(pixmap, flags);

	get_drawable_deltas(drawable, pixmap, &dx, &dy);
	DBG(("%s: delta=(%d, %d)\n", __FUNCTION__, dx, dy));
	if (dx | dy)
		RegionTranslate(region, dx, dy);

	if (region_subsumes_drawable(region, &pixmap->drawable)) {
		DBG(("%s: region subsumes drawable\n", __FUNCTION__));
		if (dx | dy)
			RegionTranslate(region, -dx, -dy);
		return _sna_pixmap_move_to_cpu(pixmap, flags);
	}

	if (operate_inplace(priv, flags) &&
	    region_inplace(sna, pixmap, region, priv, (flags & MOVE_READ) == 0) &&
	    (priv->gpu_bo || sna_pixmap_create_mappable_gpu(pixmap))) {
		kgem_bo_submit(&sna->kgem, priv->gpu_bo);

		DBG(("%s: try to operate inplace\n", __FUNCTION__));

		pixmap->devPrivate.ptr =
			kgem_bo_map(&sna->kgem, priv->gpu_bo);
		if (pixmap->devPrivate.ptr != NULL) {
			priv->mapped = true;
			pixmap->devKind = priv->gpu_bo->pitch;
			if (flags & MOVE_WRITE &&
			    !DAMAGE_IS_ALL(priv->gpu_damage)) {
				sna_damage_add(&priv->gpu_damage, region);
				if (sna_damage_is_all(&priv->gpu_damage,
						      pixmap->drawable.width,
						      pixmap->drawable.height)) {
					DBG(("%s: replaced entire pixmap, destroying CPU shadow\n",
					     __FUNCTION__));
					sna_damage_destroy(&priv->cpu_damage);
					list_del(&priv->list);
				} else
					sna_damage_subtract(&priv->cpu_damage,
							    region);
			}
			assert_pixmap_damage(pixmap);
			priv->clear = false;
			priv->cpu = false;
			if (dx | dy)
				RegionTranslate(region, -dx, -dy);
			DBG(("%s: operate inplace\n", __FUNCTION__));
			return true;
		}

		priv->mapped = false;
	}

	if (priv->clear && flags & MOVE_WRITE) {
		DBG(("%s: pending clear, moving whole pixmap for partial write\n", __FUNCTION__));
		if (dx | dy)
			RegionTranslate(region, -dx, -dy);
		return _sna_pixmap_move_to_cpu(pixmap, flags | MOVE_READ);
	}

	if (priv->mapped) {
		assert(!priv->shm);
		pixmap->devPrivate.ptr = NULL;
		priv->mapped = false;
	}

	if ((priv->clear || (flags & MOVE_READ) == 0) &&
	    priv->cpu_bo && !priv->cpu_bo->flush &&
	    __kgem_bo_is_busy(&sna->kgem, priv->cpu_bo)) {
		sna_damage_subtract(&priv->cpu_damage, region);
		if (sna_pixmap_move_to_gpu(pixmap, MOVE_READ | MOVE_ASYNC_HINT)) {
			sna_damage_all(&priv->gpu_damage,
				       pixmap->drawable.width,
				       pixmap->drawable.height);
			sna_pixmap_free_cpu(sna, priv);
		}
	}

	if (pixmap->devPrivate.ptr == NULL &&
	    !sna_pixmap_alloc_cpu(sna, pixmap, priv,
				  flags & MOVE_READ ? priv->gpu_damage && !priv->clear : 0)) {
		if (dx | dy)
			RegionTranslate(region, -dx, -dy);
		return false;
	}

	if (priv->gpu_bo == NULL) {
		assert(priv->gpu_damage == NULL);
		goto done;
	}

	assert(priv->gpu_bo->proxy == NULL);
	if (priv->clear) {
		int n = REGION_NUM_RECTS(region);
		BoxPtr box = REGION_RECTS(region);

		DBG(("%s: pending clear, doing partial fill\n", __FUNCTION__));
		if (priv->cpu_bo) {
			DBG(("%s: syncing CPU bo\n", __FUNCTION__));
			kgem_bo_sync__cpu(&sna->kgem, priv->cpu_bo);
		}

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

		if (region->extents.x2 - region->extents.x1 > 1 ||
		    region->extents.y2 - region->extents.y1 > 1) {
			sna_damage_subtract(&priv->gpu_damage, region);
			priv->clear = false;
		}
		goto done;
	}

	if ((flags & MOVE_READ) == 0) {
		assert(flags & MOVE_WRITE);
		sna_damage_subtract(&priv->gpu_damage, region);
		goto done;
	}

	if (MIGRATE_ALL && priv->gpu_damage) {
		BoxPtr box;
		int n = sna_damage_get_boxes(priv->gpu_damage, &box);
		if (n) {
			bool ok;

			DBG(("%s: forced migration\n", __FUNCTION__));

			assert(pixmap_contains_damage(pixmap, priv->gpu_damage));

			ok = false;
			if (use_cpu_bo_for_download(sna, priv, &priv->gpu_damage->extents)) {
				DBG(("%s: using CPU bo for download from GPU\n", __FUNCTION__));
				ok = sna->render.copy_boxes(sna, GXcopy,
							    pixmap, priv->gpu_bo, 0, 0,
							    pixmap, priv->cpu_bo, 0, 0,
							    box, n, COPY_LAST);
			}
			if (!ok)
				sna_read_boxes(sna,
					       priv->gpu_bo, 0, 0,
					       pixmap, 0, 0,
					       box, n);
		}
		sna_damage_destroy(&priv->gpu_damage);
	}

	if (priv->gpu_damage &&
	    (DAMAGE_IS_ALL(priv->gpu_damage) ||
	     sna_damage_overlaps_box(priv->gpu_damage, &region->extents))) {
		DBG(("%s: region (%dx%d) overlaps gpu damage\n",
		     __FUNCTION__,
		     region->extents.x2 - region->extents.x1,
		     region->extents.y2 - region->extents.y1));

		if (priv->cpu_damage == NULL) {
			if ((flags & MOVE_WRITE) == 0 &&
			    region->extents.x2 - region->extents.x1 == 1 &&
			    region->extents.y2 - region->extents.y1 == 1) {
				/*  Often associated with synchronisation, KISS */
				DBG(("%s: single pixel read\n", __FUNCTION__));
				sna_read_boxes(sna,
					       priv->gpu_bo, 0, 0,
					       pixmap, 0, 0,
					       &region->extents, 1);
				goto done;
			}
		} else {
			if (sna_damage_contains_box__no_reduce(priv->cpu_damage,
							       &region->extents)) {
				DBG(("%s: region already in CPU damage\n",
				     __FUNCTION__));
				goto done;
			}
		}

		if (sna_damage_contains_box(priv->gpu_damage,
					    &region->extents) != PIXMAN_REGION_OUT) {
			RegionRec want, *r = region;

			DBG(("%s: region (%dx%d) intersects gpu damage\n",
			     __FUNCTION__,
			     region->extents.x2 - region->extents.x1,
			     region->extents.y2 - region->extents.y1));

			if ((flags & MOVE_WRITE) == 0 &&
			    region->extents.x2 - region->extents.x1 == 1 &&
			    region->extents.y2 - region->extents.y1 == 1) {
				sna_read_boxes(sna,
					       priv->gpu_bo, 0, 0,
					       pixmap, 0, 0,
					       &region->extents, 1);
				goto done;
			}

			/* Expand the region to move 32x32 pixel blocks at a
			 * time, as we assume that we will continue writing
			 * afterwards and so aim to coallesce subsequent
			 * reads.
			 */
			if (flags & MOVE_WRITE) {
				int n = REGION_NUM_RECTS(region), i;
				BoxPtr boxes = REGION_RECTS(region);
				BoxPtr blocks = malloc(sizeof(BoxRec) * REGION_NUM_RECTS(region));
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
				BoxPtr box;
				int n;

				DBG(("%s: region wholly contains damage\n",
				     __FUNCTION__));

				n = sna_damage_get_boxes(priv->gpu_damage,
							 &box);
				if (n) {
					bool ok = false;

					if (use_cpu_bo_for_download(sna, priv, &priv->gpu_damage->extents)) {
						DBG(("%s: using CPU bo for download from GPU\n", __FUNCTION__));
						ok = sna->render.copy_boxes(sna, GXcopy,
									    pixmap, priv->gpu_bo, 0, 0,
									    pixmap, priv->cpu_bo, 0, 0,
									    box, n, COPY_LAST);
					}

					if (!ok)
						sna_read_boxes(sna,
							       priv->gpu_bo, 0, 0,
							       pixmap, 0, 0,
							       box, n);
				}

				sna_damage_destroy(&priv->gpu_damage);
			} else if (DAMAGE_IS_ALL(priv->gpu_damage) ||
				   sna_damage_contains_box__no_reduce(priv->gpu_damage,
								      &r->extents)) {
				BoxPtr box = REGION_RECTS(r);
				int n = REGION_NUM_RECTS(r);
				bool ok = false;

				DBG(("%s: region wholly inside damage\n",
				     __FUNCTION__));

				if (use_cpu_bo_for_download(sna, priv, &r->extents)) {
					DBG(("%s: using CPU bo for download from GPU\n", __FUNCTION__));
					ok = sna->render.copy_boxes(sna, GXcopy,
								    pixmap, priv->gpu_bo, 0, 0,
								    pixmap, priv->cpu_bo, 0, 0,
								    box, n, COPY_LAST);
				}
				if (!ok)
					sna_read_boxes(sna,
						       priv->gpu_bo, 0, 0,
						       pixmap, 0, 0,
						       box, n);

				sna_damage_subtract(&priv->gpu_damage, r);
			} else {
				RegionRec need;

				pixman_region_init(&need);
				if (sna_damage_intersect(priv->gpu_damage, r, &need)) {
					BoxPtr box = REGION_RECTS(&need);
					int n = REGION_NUM_RECTS(&need);
					bool ok = false;

					DBG(("%s: region intersects damage\n",
					     __FUNCTION__));

					if (use_cpu_bo_for_download(sna, priv, &need.extents)) {
						DBG(("%s: using CPU bo for download from GPU\n", __FUNCTION__));
						ok = sna->render.copy_boxes(sna, GXcopy,
									    pixmap, priv->gpu_bo, 0, 0,
									    pixmap, priv->cpu_bo, 0, 0,
									    box, n, COPY_LAST);
					}
					if (!ok)
						sna_read_boxes(sna,
							       priv->gpu_bo, 0, 0,
							       pixmap, 0, 0,
							       box, n);

					sna_damage_subtract(&priv->gpu_damage, r);
					RegionUninit(&need);
				}
			}
			if (r == &want)
				pixman_region_fini(&want);
		}
	}

done:
	if (flags & MOVE_WRITE) {
		DBG(("%s: applying cpu damage\n", __FUNCTION__));
		assert(!DAMAGE_IS_ALL(priv->cpu_damage));
		assert_pixmap_contains_box(pixmap, RegionExtents(region));
		sna_damage_add(&priv->cpu_damage, region);
		sna_damage_reduce_all(&priv->cpu_damage,
				      pixmap->drawable.width,
				      pixmap->drawable.height);
		if (DAMAGE_IS_ALL(priv->cpu_damage)) {
			if (priv->gpu_bo) {
				DBG(("%s: replaced entire pixmap\n",
				     __FUNCTION__));
				sna_pixmap_free_gpu(sna, priv);
			}
		}
		if (priv->flush) {
			assert(!priv->shm);
			sna_add_flush_pixmap(sna, priv, priv->gpu_bo);
		}
	}

	if (dx | dy)
		RegionTranslate(region, -dx, -dy);

out:
	if (flags & MOVE_WRITE) {
		priv->source_count = SOURCE_BIAS;
		assert(priv->gpu_bo == NULL || priv->gpu_bo->proxy == NULL);
		assert(!priv->flush || !list_is_empty(&priv->list));
	}
	if ((flags & MOVE_ASYNC_HINT) == 0 && priv->cpu_bo) {
		DBG(("%s: syncing cpu bo\n", __FUNCTION__));
		kgem_bo_sync__cpu_full(&sna->kgem,
				       priv->cpu_bo, flags & MOVE_WRITE);
	}
	priv->cpu = (flags & MOVE_ASYNC_HINT) == 0;
	assert(pixmap->devPrivate.ptr);
	assert(pixmap->devKind);
	assert_pixmap_damage(pixmap);
	return true;
}

static inline bool box_empty(const BoxRec *box)
{
	return box->x2 <= box->x1 || box->y2 <= box->y1;
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

static bool alu_overwrites(uint8_t alu)
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
	return priv;
}

static bool
sna_pixmap_move_area_to_gpu(PixmapPtr pixmap, const BoxRec *box, unsigned int flags)
{
	struct sna *sna = to_sna_from_pixmap(pixmap);
	struct sna_pixmap *priv = sna_pixmap(pixmap);
	RegionRec i, r;

	DBG(("%s: pixmap=%ld box=(%d, %d), (%d, %d), flags=%x\n",
	     __FUNCTION__, pixmap->drawable.serialNumber,
	     box->x1, box->y1, box->x2, box->y2, flags));

	assert(box->x2 > box->x1 && box->y2 > box->y1);
	assert_pixmap_damage(pixmap);
	assert_pixmap_contains_box(pixmap, box);
	assert(!wedged(sna));

	if (sna_damage_is_all(&priv->gpu_damage,
			      pixmap->drawable.width,
			      pixmap->drawable.height)) {
		sna_damage_destroy(&priv->cpu_damage);
		list_del(&priv->list);
		goto done;
	}

	if ((flags & MOVE_READ) == 0)
		sna_damage_subtract_box(&priv->cpu_damage, box);

	sna_damage_reduce(&priv->cpu_damage);
	assert_pixmap_damage(pixmap);

	if (priv->cpu_damage == NULL) {
		list_del(&priv->list);
		return sna_pixmap_move_to_gpu(pixmap, flags);
	}

	if (priv->gpu_bo == NULL) {
		unsigned create, tiling;

		create = CREATE_INACTIVE;
		if (pixmap->usage_hint == SNA_CREATE_FB)
			create |= CREATE_EXACT | CREATE_SCANOUT;

		tiling = (flags & MOVE_SOURCE_HINT) ? I915_TILING_Y : DEFAULT_TILING;
		tiling = sna_pixmap_choose_tiling(pixmap, tiling);

		priv->gpu_bo = kgem_create_2d(&sna->kgem,
					      pixmap->drawable.width,
					      pixmap->drawable.height,
					      pixmap->drawable.bitsPerPixel,
					      tiling, create);
		if (priv->gpu_bo == NULL)
			return false;

		DBG(("%s: created gpu bo\n", __FUNCTION__));
	}
	assert(priv->gpu_bo->proxy == NULL);

	if (priv->mapped) {
		assert(!priv->shm);
		pixmap->devPrivate.ptr = NULL;
		priv->mapped = false;
	}

	region_set(&r, box);
	if (MIGRATE_ALL || region_subsumes_damage(&r, priv->cpu_damage)) {
		int n;

		n = sna_damage_get_boxes(priv->cpu_damage, (BoxPtr *)&box);
		if (n) {
			bool ok = false;

			if (use_cpu_bo_for_upload(sna, priv, 0)) {
				DBG(("%s: using CPU bo for upload to GPU\n", __FUNCTION__));
				ok = sna->render.copy_boxes(sna, GXcopy,
							    pixmap, priv->cpu_bo, 0, 0,
							    pixmap, priv->gpu_bo, 0, 0,
							    box, n, 0);
				if (ok && priv->shm) {
					assert(!priv->flush);
					sna_add_flush_pixmap(sna, priv, priv->cpu_bo);
				}
			}
			if (!ok) {
				if (pixmap->devPrivate.ptr == NULL) {
					assert(priv->ptr);
					pixmap->devPrivate.ptr = PTR(priv->ptr);
					pixmap->devKind = priv->stride;
				}
				assert(!priv->mapped);
				if (n == 1 && !priv->pinned &&
				    box->x1 <= 0 && box->y1 <= 0 &&
				    box->x2 >= pixmap->drawable.width &&
				    box->y2 >= pixmap->drawable.height) {
					ok = sna_replace(sna, pixmap,
							 &priv->gpu_bo,
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
					return false;
			}
		}

		sna_damage_destroy(&priv->cpu_damage);
		list_del(&priv->list);
	} else if (DAMAGE_IS_ALL(priv->cpu_damage) ||
		   sna_damage_contains_box__no_reduce(priv->cpu_damage, box)) {
		bool ok = false;
		if (use_cpu_bo_for_upload(sna, priv, 0)) {
			DBG(("%s: using CPU bo for upload to GPU\n", __FUNCTION__));
			ok = sna->render.copy_boxes(sna, GXcopy,
						    pixmap, priv->cpu_bo, 0, 0,
						    pixmap, priv->gpu_bo, 0, 0,
						    box, 1, 0);
			if (ok && priv->shm) {
				assert(!priv->flush);
				sna_add_flush_pixmap(sna, priv, priv->cpu_bo);
			}
		}
		if (!ok) {
			if (pixmap->devPrivate.ptr == NULL) {
				assert(priv->ptr);
				pixmap->devPrivate.ptr = PTR(priv->ptr);
				pixmap->devKind = priv->stride;
			}
			assert(!priv->mapped);
			ok = sna_write_boxes(sna, pixmap,
					     priv->gpu_bo, 0, 0,
					     pixmap->devPrivate.ptr,
					     pixmap->devKind,
					     0, 0,
					     box, 1);
		}
		if (!ok)
			return false;

		sna_damage_subtract(&priv->cpu_damage, &r);
	} else if (sna_damage_intersect(priv->cpu_damage, &r, &i)) {
		int n = REGION_NUM_RECTS(&i);
		bool ok;

		box = REGION_RECTS(&i);
		ok = false;
		if (use_cpu_bo_for_upload(sna, priv, 0)) {
			DBG(("%s: using CPU bo for upload to GPU, %d boxes\n", __FUNCTION__, n));
			ok = sna->render.copy_boxes(sna, GXcopy,
						    pixmap, priv->cpu_bo, 0, 0,
						    pixmap, priv->gpu_bo, 0, 0,
						    box, n, 0);
			if (ok && priv->shm) {
				assert(!priv->flush);
				sna_add_flush_pixmap(sna, priv, priv->cpu_bo);
			}
		}
		if (!ok) {
			if (pixmap->devPrivate.ptr == NULL) {
				assert(priv->ptr);
				pixmap->devPrivate.ptr = PTR(priv->ptr);
				pixmap->devKind = priv->stride;
			}
			assert(!priv->mapped);
			ok = sna_write_boxes(sna, pixmap,
					     priv->gpu_bo, 0, 0,
					     pixmap->devPrivate.ptr,
					     pixmap->devKind,
					     0, 0,
					     box, n);
		}
		if (!ok)
			return false;

		sna_damage_subtract(&priv->cpu_damage, &r);
		RegionUninit(&i);
	}

	if (priv->shm) {
		assert(!priv->flush);
		sna_add_flush_pixmap(sna, priv, priv->cpu_bo);
	}

done:
	if (flags & MOVE_WRITE) {
		priv->clear = false;
		priv->cpu = false;
		if (priv->cpu_damage == NULL &&
		    box_inplace(pixmap, &r.extents)) {
			DBG(("%s: large operation on undamaged, promoting to full GPU\n",
			     __FUNCTION__));
			assert(priv->gpu_bo->proxy == NULL);
			sna_damage_all(&priv->gpu_damage,
				       pixmap->drawable.width,
				       pixmap->drawable.height);
		}
	}

	assert(!priv->gpu_bo->proxy || (flags & MOVE_WRITE) == 0);
	return sna_pixmap_mark_active(sna, priv) != NULL;
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

	if (priv->gpu_bo && priv->gpu_bo->proxy) {
		DBG(("%s: cached upload proxy, discard and revert to GPU\n",
		     __FUNCTION__));
		assert(priv->gpu_damage == NULL);
		kgem_bo_destroy(&to_sna_from_pixmap(pixmap)->kgem,
				priv->gpu_bo);
		priv->gpu_bo = NULL;
		goto use_cpu_bo;
	}

	if (priv->flush)
		flags |= PREFER_GPU;
	if (priv->shm)
		flags &= ~PREFER_GPU;
	if (priv->cpu && (flags & (FORCE_GPU | IGNORE_CPU)) == 0)
		flags &= ~PREFER_GPU;

	DBG(("%s: flush=%d, shm=%d, cpu=%d => flags=%x\n",
	     __FUNCTION__, priv->flush, priv->shm, priv->cpu, flags));

	if ((flags & PREFER_GPU) == 0 &&
	    (!priv->gpu_damage || !kgem_bo_is_busy(priv->gpu_bo))) {
		DBG(("%s: try cpu as GPU bo is idle\n", __FUNCTION__));
		goto use_cpu_bo;
	}

	if (DAMAGE_IS_ALL(priv->gpu_damage)) {
		DBG(("%s: use GPU fast path (all-damaged)\n", __FUNCTION__));
		assert(priv->cpu_damage == NULL);
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

		if ((flags & IGNORE_CPU) == 0) {
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
			get_drawable_deltas(drawable, pixmap, &dx, &dy);

			region.extents = *box;
			region.extents.x1 += dx;
			region.extents.x2 += dx;
			region.extents.y1 += dy;
			region.extents.y2 += dy;
			region.data = NULL;

			sna_damage_subtract(&priv->cpu_damage, &region);
			if (priv->cpu_damage == NULL) {
				list_del(&priv->list);
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

	get_drawable_deltas(drawable, pixmap, &dx, &dy);

	region.extents = *box;
	region.extents.x1 += dx;
	region.extents.x2 += dx;
	region.extents.y1 += dy;
	region.extents.y2 += dy;

	DBG(("%s extents (%d, %d), (%d, %d)\n", __FUNCTION__,
	     region.extents.x1, region.extents.y1,
	     region.extents.x2, region.extents.y2));

	if (priv->gpu_damage) {
		if (!priv->cpu_damage) {
			if (sna_damage_contains_box__no_reduce(priv->gpu_damage,
							       &region.extents)) {
				DBG(("%s: region wholly contained within GPU damage\n",
				     __FUNCTION__));
				goto use_gpu_bo;
			} else {
				DBG(("%s: partial GPU damage with no CPU damage, continuing to use GPU\n",
				     __FUNCTION__));
				priv->cpu = false;
				goto done;
			}
		}

		ret = sna_damage_contains_box(priv->gpu_damage, &region.extents);
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

	if ((flags & IGNORE_CPU) == 0 && priv->cpu_damage) {
		ret = sna_damage_contains_box(priv->cpu_damage, &region.extents);
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
					 flags & IGNORE_CPU ? MOVE_WRITE : MOVE_READ | MOVE_WRITE)) {
		DBG(("%s: failed to move-to-gpu, fallback\n", __FUNCTION__));
		assert(priv->gpu_bo == NULL);
		goto use_cpu_bo;
	}

done:
	assert(priv->gpu_bo != NULL);
	assert(priv->gpu_bo->refcnt);
	if (sna_damage_is_all(&priv->gpu_damage,
			      pixmap->drawable.width,
			      pixmap->drawable.height)) {
		sna_damage_destroy(&priv->cpu_damage);
		list_del(&priv->list);
		*damage = NULL;
	} else
		*damage = &priv->gpu_damage;

	DBG(("%s: using GPU bo with damage? %d\n",
	     __FUNCTION__, *damage != NULL));
	assert(*damage == NULL || !DAMAGE_IS_ALL(*damage));
	assert(priv->gpu_bo->proxy == NULL);
	assert(priv->clear == false);
	assert(priv->cpu == false);
	return priv->gpu_bo;

use_gpu_bo:
	DBG(("%s: using whole GPU bo\n", __FUNCTION__));
	assert(priv->gpu_bo != NULL);
	assert(priv->gpu_bo->refcnt);
	assert(priv->gpu_bo->proxy == NULL);
	assert(priv->gpu_damage);
	priv->clear = false;
	priv->cpu = false;
	*damage = NULL;
	return priv->gpu_bo;

use_cpu_bo:
	if (!USE_CPU_BO)
		return NULL;

	if (priv->cpu_bo == NULL)
		return NULL;

	assert(priv->cpu_bo->refcnt);

	sna = to_sna_from_pixmap(pixmap);
	if ((flags & FORCE_GPU) == 0 &&
	    !__kgem_bo_is_busy(&sna->kgem, priv->cpu_bo)) {
		DBG(("%s: has CPU bo, but is idle and acceleration not forced\n",
		     __FUNCTION__));
		return NULL;
	}

	get_drawable_deltas(drawable, pixmap, &dx, &dy);

	region.extents = *box;
	region.extents.x1 += dx;
	region.extents.x2 += dx;
	region.extents.y1 += dy;
	region.extents.y2 += dy;
	region.data = NULL;

	/* Both CPU and GPU are busy, prefer to use the GPU */
	if (priv->gpu_bo && kgem_bo_is_busy(priv->gpu_bo))
		goto move_to_gpu;

	assert(priv->gpu_bo == NULL || priv->gpu_bo->proxy == NULL);

	if (flags & RENDER_GPU) {
		if (priv->gpu_bo && priv->gpu_bo->tiling)
			goto move_to_gpu;

		if (priv->cpu_bo->pitch >= 4096)
			goto move_to_gpu;

		if (!sna->kgem.can_blt_cpu)
			goto move_to_gpu;
	}

	if (!sna->kgem.can_blt_cpu)
		return NULL;

	if (!sna_drawable_move_region_to_cpu(&pixmap->drawable, &region,
					     MOVE_READ | MOVE_ASYNC_HINT)) {
		DBG(("%s: failed to move-to-cpu, fallback\n", __FUNCTION__));
		return NULL;
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
		if (priv->cpu_damage &&
		    sna_damage_contains_box__no_reduce(priv->cpu_damage,
						       &region.extents))
			*damage = NULL;
		else
			*damage = &priv->cpu_damage;
	}

	DBG(("%s: using CPU bo with damage? %d\n",
	     __FUNCTION__, *damage != NULL));
	assert(damage == NULL || !DAMAGE_IS_ALL(*damage));
	assert(priv->clear == false);
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
	int bpp = bits_per_pixel(depth);
	void *ptr;

	DBG(("%s(%d, %d, %d, flags=%x)\n", __FUNCTION__,
	     width, height, depth, flags));
	assert(width);
	assert(height);

	if (sna->freed_pixmap) {
		pixmap = sna->freed_pixmap;
		sna->freed_pixmap = pixmap->devPrivate.ptr;

		pixmap->drawable.serialNumber = NEXT_SERIAL_NUMBER;
		pixmap->refcnt = 1;
	} else {
		pixmap = create_pixmap(sna, screen, 0, 0, depth, 0);
		if (!pixmap)
			return NullPixmap;

		priv = malloc(sizeof(*priv));
		if (!priv) {
			FreePixmap(pixmap);
			return NullPixmap;
		}

		sna_set_pixmap(pixmap, priv);
	}

	priv = _sna_pixmap_reset(pixmap);
	priv->header = true;

	priv->gpu_bo = kgem_create_buffer_2d(&sna->kgem,
					     width, height, bpp,
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
	sna_damage_all(&priv->gpu_damage, width, height);
	sna_damage_all(&priv->cpu_damage, width, height);

	pixmap->drawable.width = width;
	pixmap->drawable.height = height;
	pixmap->drawable.depth = depth;
	pixmap->drawable.bitsPerPixel = bpp;
	pixmap->drawable.serialNumber = NEXT_SERIAL_NUMBER;
	pixmap->devKind = priv->gpu_bo->pitch;
	pixmap->devPrivate.ptr = ptr;

	pixmap->usage_hint = 0;
	if (!kgem_buffer_is_inplace(priv->gpu_bo))
		pixmap->usage_hint = 1;

	DBG(("%s: serial=%ld, usage=%d\n",
	     __FUNCTION__,
	     pixmap->drawable.serialNumber,
	     pixmap->usage_hint));

	return pixmap;
}

struct sna_pixmap *
sna_pixmap_move_to_gpu(PixmapPtr pixmap, unsigned flags)
{
	struct sna *sna = to_sna_from_pixmap(pixmap);
	struct sna_pixmap *priv;
	BoxPtr box;
	int n;

	DBG(("%s(pixmap=%ld, usage=%d), flags=%x\n",
	     __FUNCTION__,
	     pixmap->drawable.serialNumber,
	     pixmap->usage_hint,
	     flags));

	if ((flags & __MOVE_FORCE) == 0 && wedged(sna))
		return NULL;

	priv = sna_pixmap(pixmap);
	if (priv == NULL) {
		DBG(("%s: not attached\n", __FUNCTION__));
		if ((flags & __MOVE_DRI) == 0)
			return NULL;

		DBG(("%s: forcing the creation on the GPU\n", __FUNCTION__));

		priv = sna_pixmap_attach(pixmap);
		if (priv == NULL)
			return NULL;

		sna_damage_all(&priv->cpu_damage,
			       pixmap->drawable.width,
			       pixmap->drawable.height);
	}

	if (sna_damage_is_all(&priv->gpu_damage,
			      pixmap->drawable.width,
			      pixmap->drawable.height)) {
		DBG(("%s: already all-damaged\n", __FUNCTION__));
		sna_damage_destroy(&priv->cpu_damage);
		list_del(&priv->list);
		assert(priv->cpu == false || IS_CPU_MAP(priv->gpu_bo->map));
		goto active;
	}

	if (flags & MOVE_WRITE && priv->gpu_bo && priv->gpu_bo->proxy) {
		DBG(("%s: discarding cached upload buffer\n", __FUNCTION__));
		assert(priv->gpu_damage == NULL);
		kgem_bo_destroy(&sna->kgem, priv->gpu_bo);
		priv->gpu_bo = NULL;
	}

	if ((flags & MOVE_READ) == 0)
		sna_damage_destroy(&priv->cpu_damage);

	sna_damage_reduce(&priv->cpu_damage);
	assert_pixmap_damage(pixmap);
	DBG(("%s: CPU damage? %d\n", __FUNCTION__, priv->cpu_damage != NULL));
	if (priv->gpu_bo == NULL) {
		DBG(("%s: creating GPU bo (%dx%d@%d), create=%x\n",
		     __FUNCTION__,
		     pixmap->drawable.width,
		     pixmap->drawable.height,
		     pixmap->drawable.bitsPerPixel,
		     priv->create));
		assert(!priv->mapped);
		if (flags & __MOVE_FORCE || priv->create & KGEM_CAN_CREATE_GPU) {
			unsigned create, tiling;

			assert(pixmap->drawable.width > 0);
			assert(pixmap->drawable.height > 0);
			assert(pixmap->drawable.bitsPerPixel >= 8);

			tiling = (flags & MOVE_SOURCE_HINT) ? I915_TILING_Y : DEFAULT_TILING;
			tiling = sna_pixmap_choose_tiling(pixmap, tiling);

			create = 0;
			if (priv->cpu_damage && priv->cpu_bo == NULL)
				create = CREATE_GTT_MAP | CREATE_INACTIVE;
			if (flags & MOVE_INPLACE_HINT)
				create = CREATE_GTT_MAP | CREATE_INACTIVE;

			priv->gpu_bo =
				kgem_create_2d(&sna->kgem,
					       pixmap->drawable.width,
					       pixmap->drawable.height,
					       pixmap->drawable.bitsPerPixel,
					       tiling, create);
		}
		if (priv->gpu_bo == NULL) {
			DBG(("%s: not creating GPU bo\n", __FUNCTION__));
			assert(list_is_empty(&priv->list));
			return NULL;
		}

		if (flags & MOVE_WRITE && priv->cpu_damage == NULL) {
			/* Presume that we will only ever write to the GPU
			 * bo. Readbacks are expensive but fairly constant
			 * in cost for all sizes i.e. it is the act of
			 * synchronisation that takes the most time. This is
			 * mitigated by avoiding fallbacks in the first place.
			 */
			assert(priv->gpu_bo->proxy == NULL);
			sna_damage_all(&priv->gpu_damage,
				       pixmap->drawable.width,
				       pixmap->drawable.height);
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

	if (priv->mapped) {
		assert(priv->stride);
		pixmap->devPrivate.ptr = PTR(priv->ptr);
		pixmap->devKind = priv->stride;
		priv->mapped = false;
	}

	n = sna_damage_get_boxes(priv->cpu_damage, &box);
	if (n) {
		bool ok;

		assert(pixmap_contains_damage(pixmap, priv->cpu_damage));
		DBG(("%s: uploading %d damage boxes\n", __FUNCTION__, n));

		ok = false;
		if (use_cpu_bo_for_upload(sna, priv, flags)) {
			DBG(("%s: using CPU bo for upload to GPU\n", __FUNCTION__));
			ok = sna->render.copy_boxes(sna, GXcopy,
						    pixmap, priv->cpu_bo, 0, 0,
						    pixmap, priv->gpu_bo, 0, 0,
						    box, n, 0);
		}
		if (!ok) {
			if (pixmap->devPrivate.ptr == NULL) {
				assert(priv->ptr);
				pixmap->devPrivate.ptr = PTR(priv->ptr);
				pixmap->devKind = priv->stride;
			}
			assert(!priv->mapped);
			if (n == 1 && !priv->pinned &&
			    (box->x2 - box->x1) >= pixmap->drawable.width &&
			    (box->y2 - box->y1) >= pixmap->drawable.height) {
				ok = sna_replace(sna, pixmap,
						 &priv->gpu_bo,
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

	if (priv->shm) {
		assert(!priv->flush);
		sna_add_flush_pixmap(sna, priv, priv->cpu_bo);
	}

	/* For large bo, try to keep only a single copy around */
	if (priv->create & KGEM_CAN_CREATE_LARGE ||
	    flags & MOVE_SOURCE_HINT) {
		DBG(("%s: disposing of system copy for large/source\n",
		     __FUNCTION__));
		assert(!priv->shm);
		assert(priv->gpu_bo->proxy == NULL);
		sna_damage_all(&priv->gpu_damage,
			       pixmap->drawable.width,
			       pixmap->drawable.height);
		sna_pixmap_free_cpu(sna, priv);
	}
done:
	list_del(&priv->list);

	sna_damage_reduce_all(&priv->gpu_damage,
			      pixmap->drawable.width,
			      pixmap->drawable.height);
	if (DAMAGE_IS_ALL(priv->gpu_damage))
		sna_pixmap_free_cpu(sna, priv);

active:
	if (flags & MOVE_WRITE)
		priv->clear = false;
	priv->cpu = false;
	assert(!priv->gpu_bo->proxy || (flags & MOVE_WRITE) == 0);
	return sna_pixmap_mark_active(sna, priv);
}

static bool must_check sna_validate_pixmap(DrawablePtr draw, PixmapPtr pixmap)
{
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

	DBG(("%s, changes=%lx\n", __FUNCTION__, changes));

	assert(gc->ops == (GCOps *)&sna_gc_ops);
	gc->ops = (GCOps *)&sna_gc_ops__cpu;

	sgc->old_funcs = gc->funcs;
	gc->funcs = (GCFuncs *)&sna_gc_funcs__cpu;

	sgc->priv = gc->pCompositeClip;
	gc->pCompositeClip = region;

	if (gc->clientClipType == CT_PIXMAP) {
		PixmapPtr clip = gc->clientClip;
		gc->clientClip = BitmapToRegion(gc->pScreen, clip);
		gc->pScreen->DestroyPixmap(clip);
		gc->clientClipType = gc->clientClip ? CT_REGION : CT_NONE;
		changes |= GCClipMask;
	} else
		changes &= ~GCClipMask;

	if (changes || drawable->serialNumber != sgc->serial) {
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

		gc->serialNumber = drawable->serialNumber;
		sgc->serial = drawable->serialNumber;
	}
	sgc->changes = 0;

	switch (gc->fillStyle) {
	case FillTiled:
		return sna_drawable_move_to_cpu(&gc->tile.pixmap->drawable, MOVE_READ);
	case FillStippled:
	case FillOpaqueStippled:
		return sna_drawable_move_to_cpu(&gc->stipple->drawable, MOVE_READ);
	default:
		return true;
	}
}

static void sna_gc_move_to_gpu(GCPtr gc)
{
	assert(gc->ops == (GCOps *)&sna_gc_ops__cpu);
	assert(gc->funcs == (GCFuncs *)&sna_gc_funcs__cpu);

	gc->ops = (GCOps *)&sna_gc_ops;
	gc->funcs = sna_gc(gc)->old_funcs;
	gc->pCompositeClip = sna_gc(gc)->priv;
}

static inline bool clip_box(BoxPtr box, GCPtr gc)
{
	const BoxRec *clip;
	bool clipped;

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
sna_put_zpixmap_blt(DrawablePtr drawable, GCPtr gc, RegionPtr region,
		    int x, int y, int w, int  h, char *bits, int stride)
{
	PixmapPtr pixmap = get_drawable_pixmap(drawable);
	BoxRec *box;
	int16_t dx, dy;
	int n;

	assert_pixmap_contains_box(pixmap, RegionExtents(region));

	if (gc->alu != GXcopy)
		return false;

	if (drawable->depth < 8)
		return false;

	if (!sna_drawable_move_region_to_cpu(&pixmap->drawable,
					     region, MOVE_WRITE))
		return false;

	get_drawable_deltas(drawable, pixmap, &dx, &dy);
	x += dx + drawable->x;
	y += dy + drawable->y;

	DBG(("%s: upload(%d, %d, %d, %d)\n", __FUNCTION__, x, y, w, h));

	/* Region is pre-clipped and translated into pixmap space */
	box = REGION_RECTS(region);
	n = REGION_NUM_RECTS(region);
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

		memcpy_blt(bits, pixmap->devPrivate.ptr,
			   pixmap->drawable.bitsPerPixel,
			   stride, pixmap->devKind,
			   box->x1 - x, box->y1 - y,
			   box->x1, box->y1,
			   box->x2 - box->x1, box->y2 - box->y1);
		box++;
	} while (--n);

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
	BoxRec *box;
	int16_t dx, dy;
	int n;
	uint8_t rop = copy_ROP[gc->alu];

	bo = sna_drawable_use_bo(&pixmap->drawable, PREFER_GPU,
				 &region->extents, &damage);
	if (bo == NULL)
		return false;

	if (bo->tiling == I915_TILING_Y) {
		DBG(("%s: converting bo from Y-tiling\n", __FUNCTION__));
		assert(bo == sna_pixmap_get_bo(pixmap));
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
	box = REGION_RECTS(region);
	n = REGION_NUM_RECTS(region);
	do {
		int bx1 = (box->x1 - x) & ~7;
		int bx2 = (box->x2 - x + 7) & ~7;
		int bw = (bx2 - bx1)/8;
		int bh = box->y2 - box->y1;
		int bstride = ALIGN(bw, 2);
		int src_stride;
		uint8_t *dst, *src;
		uint32_t *b;
		struct kgem_bo *upload;
		void *ptr;

		if (!kgem_check_batch(&sna->kgem, 8) ||
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

		dst = ptr;
		bstride -= bw;

		src_stride = BitmapBytePad(w);
		src = (uint8_t*)bits + (box->y1 - y) * src_stride + bx1/8;
		src_stride -= bw;
		do {
			int i = bw;
			do {
				*dst++ = byte_reverse(*src++);
			} while (--i);
			dst += bstride;
			src += src_stride;
		} while (--bh);

		b = sna->kgem.batch + sna->kgem.nbatch;
		b[0] = XY_MONO_SRC_COPY | 3 << 20;
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
		b[5] = kgem_add_reloc(&sna->kgem, sna->kgem.nbatch + 5,
				      upload,
				      I915_GEM_DOMAIN_RENDER << 16 |
				      KGEM_RELOC_FENCED,
				      0);
		b[6] = gc->bgPixel;
		b[7] = gc->fgPixel;

		sna->kgem.nbatch += 8;
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
		assert(bo == sna_pixmap_get_bo(pixmap));
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
		const BoxRec *box = REGION_RECTS(region);
		int n = REGION_NUM_RECTS(region);

		if ((gc->planemask & i) == 0)
			continue;

		/* Region is pre-clipped and translated into pixmap space */
		do {
			int bx1 = (box->x1 - x) & ~7;
			int bx2 = (box->x2 - x + 7) & ~7;
			int bw = (bx2 - bx1)/8;
			int bh = box->y2 - box->y1;
			int bstride = ALIGN(bw, 2);
			int src_stride;
			uint8_t *dst, *src;
			uint32_t *b;
			struct kgem_bo *upload;
			void *ptr;

			if (!kgem_check_batch(&sna->kgem, 12) ||
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

			dst = ptr;
			bstride -= bw;

			src_stride = BitmapBytePad(w);
			src = (uint8_t*)bits + (box->y1 - y) * src_stride + bx1/8;
			src_stride -= bw;
			do {
				int j = bw;
				do {
					*dst++ = byte_reverse(*src++);
				} while (--j);
				dst += bstride;
				src += src_stride;
			} while (--bh);

			b = sna->kgem.batch + sna->kgem.nbatch;
			b[0] = XY_FULL_MONO_PATTERN_MONO_SRC_BLT | 3 << 20;
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
			b[4] = kgem_add_reloc(&sna->kgem, sna->kgem.nbatch + 4,
					      bo,
					      I915_GEM_DOMAIN_RENDER << 16 |
					      I915_GEM_DOMAIN_RENDER |
					      KGEM_RELOC_FENCED,
					      0);
			b[5] = kgem_add_reloc(&sna->kgem, sna->kgem.nbatch + 5,
					      upload,
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

	get_drawable_deltas(drawable, pixmap, &dx, &dy);

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
		RegionIntersect(&region, &region, gc->pCompositeClip);
		if (RegionNil(&region))
			return;
	}

	if (priv == NULL) {
		DBG(("%s: fallback -- unattached(%d, %d, %d, %d)\n",
		     __FUNCTION__, x, y, w, h));
		goto fallback;
	}

	RegionTranslate(&region, dx, dy);

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
		goto out_gc;

	DBG(("%s: fbPutImage(%d, %d, %d, %d)\n",
	     __FUNCTION__, x, y, w, h));
	fbPutImage(drawable, gc, depth, x, y, w, h, left, format, bits);
	FALLBACK_FLUSH(drawable);
out_gc:
	sna_gc_move_to_gpu(gc);
out:
	RegionUninit(&region);
}

static bool
move_to_gpu(PixmapPtr pixmap, struct sna_pixmap *priv,
	    const BoxRec *box, uint8_t alu)
{
	int w = box->x2 - box->x1;
	int h = box->y2 - box->y1;
	int count;

	if (DAMAGE_IS_ALL(priv->gpu_damage))
		return true;

	if (priv->gpu_bo) {
		if (alu != GXcopy)
			return true;

		if (!priv->cpu)
			return true;

		if (priv->gpu_bo->tiling)
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

		if (sna_pixmap_choose_tiling(pixmap,
					     DEFAULT_TILING) == I915_TILING_NONE)
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

static BoxPtr
reorder_boxes(BoxPtr box, int n, int dx, int dy)
{
	BoxPtr new, base, next, tmp;

	DBG(("%s x %d dx=%d, dy=%d\n", __FUNCTION__, n, dx, dy));

	if (dy <= 0 && dx <= 0) {
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
	BoxPtr box = RegionRects(region);
	int n = RegionNumRects(region);
	int alu = gc ? gc->alu : GXcopy;
	int16_t tx, ty;

	assert(RegionNumRects(region));
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

	get_drawable_deltas(src, pixmap, &tx, &ty);
	dx += tx;
	dy += ty;
	if (dst != src)
		get_drawable_deltas(dst, pixmap, &tx, &ty);

	if (priv == NULL || DAMAGE_IS_ALL(priv->cpu_damage) || priv->shm)
		goto fallback;

	if (priv->gpu_damage) {
		if (alu == GXcopy && priv->clear)
			goto out;

		assert(priv->gpu_bo->proxy == NULL);
		if (!sna_pixmap_move_to_gpu(pixmap, MOVE_WRITE | MOVE_READ | MOVE_ASYNC_HINT)) {
			DBG(("%s: fallback - not a pure copy and failed to move dst to GPU\n",
			     __FUNCTION__));
			goto fallback;
		}

		if (!sna->render.copy_boxes(sna, alu,
					    pixmap, priv->gpu_bo, dx, dy,
					    pixmap, priv->gpu_bo, tx, ty,
					    box, n, 0)) {
			DBG(("%s: fallback - accelerated copy boxes failed\n",
			     __FUNCTION__));
			goto fallback;
		}

		if (!DAMAGE_IS_ALL(priv->gpu_damage)) {
			RegionTranslate(region, tx, ty);
			sna_damage_add(&priv->gpu_damage, region);
		}
		assert_pixmap_damage(pixmap);
	} else {
fallback:
		DBG(("%s: fallback", __FUNCTION__));
		if (!sna_pixmap_move_to_cpu(pixmap, MOVE_READ | MOVE_WRITE))
			goto out;

		if (alu == GXcopy && pixmap->drawable.bitsPerPixel >= 8) {
			FbBits *dst_bits, *src_bits;
			int stride = pixmap->devKind;
			int bpp = pixmap->drawable.bitsPerPixel;
			int i;

			dst_bits = (FbBits *)
				((char *)pixmap->devPrivate.ptr +
				 ty * stride + tx * bpp / 8);
			src_bits = (FbBits *)
				((char *)pixmap->devPrivate.ptr +
				 dy * stride + dx * bpp / 8);

			for (i = 0; i < n; i++)
				memmove_box(src_bits, dst_bits,
					    bpp, stride, box+i,
					    dx, dy);
		} else {
			if (gc && !sna_gc_move_to_cpu(gc, dst, region))
				goto out;

			get_drawable_deltas(src, pixmap, &tx, &ty);
			miCopyRegion(src, dst, gc,
				     region, dx - tx, dy - ty,
				     fbCopyNtoN, 0, NULL);

			if (gc)
				sna_gc_move_to_gpu(gc);
		}
	}

out:
	if (box != RegionRects(region))
		free(box);
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
source_prefer_gpu(struct sna *sna, struct sna_pixmap *priv)
{
	if (priv == NULL) {
		DBG(("%s: source unattached, use cpu\n", __FUNCTION__));
		return 0;
	}

	if (priv->clear) {
		DBG(("%s: source is clear, don't force use of GPU\n", __FUNCTION__));
		return 0;
	}

	if (priv->gpu_damage) {
		DBG(("%s: source has gpu damage, force gpu\n", __FUNCTION__));
		return PREFER_GPU | FORCE_GPU;
	}

	if (priv->cpu_bo && kgem_bo_is_busy(priv->cpu_bo)) {
		DBG(("%s: source has busy CPU bo, force gpu\n", __FUNCTION__));
		return PREFER_GPU | FORCE_GPU;
	}

	if (DAMAGE_IS_ALL(priv->cpu_damage))
		return priv->cpu_bo && kgem_is_idle(&sna->kgem);

	DBG(("%s: source has GPU bo? %d\n",
	     __FUNCTION__, priv->gpu_bo != NULL));
	return priv->gpu_bo != NULL;
}

static bool use_shm_bo(struct sna *sna,
		       struct kgem_bo *bo,
		       struct sna_pixmap *priv,
		       int alu)
{
	if (priv == NULL || priv->cpu_bo == NULL) {
		DBG(("%s: no, not attached\n", __FUNCTION__));
		return false;
	}

	if (!priv->shm) {
		DBG(("%s: yes, ordinary CPU bo\n", __FUNCTION__));
		return true;
	}

	if (alu != GXcopy) {
		DBG(("%s: yes, complex alu=%d\n", __FUNCTION__, alu));
		return true;
	}
	if (bo->tiling) {
		DBG(("%s:, yes, dst tiled=%d\n", __FUNCTION__, bo->tiling));
		return true;
	}

	if (__kgem_bo_is_busy(&sna->kgem, bo)) {
		DBG(("%s: yes, dst is busy\n", __FUNCTION__));
		return true;
	}

	if (__kgem_bo_is_busy(&sna->kgem, priv->cpu_bo)) {
		DBG(("%s: yes, src is busy\n", __FUNCTION__));
		return true;
	}

	return false;
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
	unsigned hint;
	int16_t src_dx, src_dy;
	int16_t dst_dx, dst_dy;
	BoxPtr box = RegionRects(region);
	int n = RegionNumRects(region);
	int alu = gc->alu;
	int stride, bpp;
	char *bits;
	bool replaces;

	assert(RegionNumRects(region));

	if (src_pixmap == dst_pixmap)
		return sna_self_copy_boxes(src, dst, gc,
					   region, dx, dy,
					   bitplane, closure);

	DBG(("%s (boxes=%dx[(%d, %d), (%d, %d)...], src=+(%d, %d), alu=%d, src.size=%dx%d, dst.size=%dx%d)\n",
	     __FUNCTION__, n,
	     box[0].x1, box[0].y1, box[0].x2, box[0].y2,
	     dx, dy, alu,
	     src_pixmap->drawable.width, src_pixmap->drawable.height,
	     dst_pixmap->drawable.width, dst_pixmap->drawable.height));

	assert_pixmap_damage(dst_pixmap);
	assert_pixmap_damage(src_pixmap);

	bpp = dst_pixmap->drawable.bitsPerPixel;

	get_drawable_deltas(dst, dst_pixmap, &dst_dx, &dst_dy);
	RegionTranslate(region, dst_dx, dst_dy);
	get_drawable_deltas(src, src_pixmap, &src_dx, &src_dy);
	src_dx += dx - dst_dx;
	src_dy += dy - dst_dy;

	assert_pixmap_contains_box(dst_pixmap, RegionExtents(region));
	assert_pixmap_contains_box_with_offset(src_pixmap,
					       RegionExtents(region),
					       src_dx, src_dy);

	replaces = n == 1 &&
		box->x1 <= 0 &&
		box->y1 <= 0 &&
		box->x2 >= dst_pixmap->drawable.width &&
		box->y2 >= dst_pixmap->drawable.height;

	DBG(("%s: dst=(priv=%p, gpu_bo=%p, cpu_bo=%p), src=(priv=%p, gpu_bo=%p, cpu_bo=%p), replaces=%d\n",
	     __FUNCTION__,
	     dst_priv,
	     dst_priv ? dst_priv->gpu_bo : NULL,
	     dst_priv ? dst_priv->cpu_bo : NULL,
	     src_priv,
	     src_priv ? src_priv->gpu_bo : NULL,
	     src_priv ? src_priv->cpu_bo : NULL,
	     replaces));

	if (dst_priv == NULL)
		goto fallback;

	hint = source_prefer_gpu(sna, src_priv) ?:
		region_inplace(sna, dst_pixmap, region,
			       dst_priv, alu_overwrites(alu));
	if (dst_priv->cpu_damage && alu_overwrites(alu)) {
		DBG(("%s: overwritting CPU damage\n", __FUNCTION__));
		if (region_subsumes_damage(region, dst_priv->cpu_damage)) {
			DBG(("%s: discarding existing CPU damage\n", __FUNCTION__));
			if (dst_priv->gpu_bo && dst_priv->gpu_bo->proxy) {
				kgem_bo_destroy(&sna->kgem, dst_priv->gpu_bo);
				dst_priv->gpu_bo = NULL;
			}
			sna_damage_destroy(&dst_priv->cpu_damage);
			list_del(&dst_priv->list);
			dst_priv->cpu = false;
		}
		if (region->data == NULL)
			hint |= IGNORE_CPU;
	}
	if (replaces)
		hint |= IGNORE_CPU;

	bo = sna_drawable_use_bo(&dst_pixmap->drawable, hint,
				 &region->extents, &damage);
	if (bo) {
		if (src_priv && src_priv->clear) {
			DBG(("%s: applying src clear[%08x] to dst\n",
			     __FUNCTION__, src_priv->clear_color));
			if (n == 1) {
				if (!sna->render.fill_one(sna,
							  dst_pixmap, bo,
							  src_priv->clear_color,
							  box->x1, box->y1,
							  box->x2, box->y2,
							  alu)) {
					DBG(("%s: unsupported fill\n",
					     __FUNCTION__));
					goto fallback;
				}
			} else {
				struct sna_fill_op fill;

				if (!sna_fill_init_blt(&fill, sna,
						       dst_pixmap, bo,
						       alu, src_priv->clear_color)) {
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
		    move_to_gpu(src_pixmap, src_priv, &region->extents, alu) &&
		    sna_pixmap_move_to_gpu(src_pixmap, MOVE_READ | MOVE_ASYNC_HINT)) {
			DBG(("%s: move whole src_pixmap to GPU and copy\n",
			     __FUNCTION__));
			if (!sna->render.copy_boxes(sna, alu,
						    src_pixmap, src_priv->gpu_bo, src_dx, src_dy,
						    dst_pixmap, bo, 0, 0,
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
							 MOVE_READ | MOVE_ASYNC_HINT))
				goto fallback;

			if (!sna->render.copy_boxes(sna, alu,
						    src_pixmap, src_priv->gpu_bo, src_dx, src_dy,
						    dst_pixmap, bo, 0, 0,
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

		if (use_shm_bo(sna, bo, src_priv, alu)) {
			bool ret;

			DBG(("%s: region overlaps CPU damage, copy from CPU bo (shm? %d)\n",
			     __FUNCTION__, src_priv->shm));

			assert(bo != dst_priv->cpu_bo);

			RegionTranslate(region, src_dx, src_dy);
			ret = sna_drawable_move_region_to_cpu(&src_pixmap->drawable,
							      region,
							      MOVE_READ | MOVE_ASYNC_HINT);
			RegionTranslate(region, -src_dx, -src_dy);
			if (!ret)
				goto fallback;

			if (!sna->render.copy_boxes(sna, alu,
						    src_pixmap, src_priv->cpu_bo, src_dx, src_dy,
						    dst_pixmap, bo, 0, 0,
						    box, n, src_priv->shm ? COPY_LAST : 0)) {
				DBG(("%s: fallback - accelerated copy boxes failed\n",
				     __FUNCTION__));
				goto fallback;
			}

			if (src_priv->shm) {
				assert(!src_priv->flush);
				sna_add_flush_pixmap(sna, src_priv, src_priv->cpu_bo);
			}

			if (damage)
				sna_damage_add(damage, region);
			return;
		}

		if (USE_USERPTR_UPLOADS &&
		    src_priv == NULL &&
		    sna->kgem.has_userptr &&
		    box_inplace(src_pixmap, &region->extents) &&
		    ((sna->kgem.has_llc && bo->tiling && !bo->scanout) ||
		     __kgem_bo_is_busy(&sna->kgem, bo))) {
			struct kgem_bo *src_bo;
			bool ok = false;

			DBG(("%s: upload through a temporary map\n",
			     __FUNCTION__));

			src_bo = kgem_create_map(&sna->kgem,
						 src_pixmap->devPrivate.ptr,
						 src_pixmap->devKind * src_pixmap->drawable.height,
						 true);
			if (src_bo) {
				src_bo->flush = true;
				src_bo->pitch = src_pixmap->devKind;
				src_bo->reusable = false;

				ok = sna->render.copy_boxes(sna, alu,
							    src_pixmap, src_bo, src_dx, src_dy,
							    dst_pixmap, bo, 0, 0,
							    box, n, COPY_LAST);

				kgem_bo_sync__cpu(&sna->kgem, src_bo);
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

			src_bo = sna_pixmap_get_bo(tmp);
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
						    tmp, src_bo, dx, dy,
						    dst_pixmap, bo, 0, 0,
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

			if (src_priv) {
				/* Fixup the shadow pointer as necessary */
				if (src_priv->mapped) {
					assert(!src_priv->shm);
					src_pixmap->devPrivate.ptr = NULL;
					src_priv->mapped = false;
				}
				if (src_pixmap->devPrivate.ptr == NULL) {
					if (!src_priv->ptr) /* uninitialised!*/
						return;
					src_pixmap->devPrivate.ptr = PTR(src_priv->ptr);
					src_pixmap->devKind = src_priv->stride;
				}
			}

			if (!dst_priv->pinned && replaces) {
				stride = src_pixmap->devKind;
				bits = src_pixmap->devPrivate.ptr;
				bits += (src_dy + box->y1) * stride + (src_dx + box->x1) * bpp / 8;

				if (!sna_replace(sna, dst_pixmap,
						 &dst_priv->gpu_bo,
						 bits, stride))
					goto fallback;
			} else {
				assert(!DAMAGE_IS_ALL(dst_priv->cpu_damage));
				if (!sna_write_boxes(sna, dst_pixmap,
						     dst_priv->gpu_bo, 0, 0,
						     src_pixmap->devPrivate.ptr,
						     src_pixmap->devKind,
						     src_dx, src_dy,
						     box, n))
					goto fallback;
			}

			assert(dst_priv->clear == false);
			dst_priv->cpu = false;
			if (damage) {
				assert(dst_priv->gpu_bo->proxy == NULL);
				if (replaces) {
					sna_damage_destroy(&dst_priv->cpu_damage);
					sna_damage_all(&dst_priv->gpu_damage,
						       dst_pixmap->drawable.width,
						       dst_pixmap->drawable.height);
					list_del(&dst_priv->list);
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
	} else {
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
			if (src_priv->cpu_bo == NULL)
				mode |= MOVE_INPLACE_HINT;

			if (!sna_drawable_move_region_to_cpu(&src_pixmap->drawable,
							     region, mode))
				return;

			RegionTranslate(region, -src_dx, -src_dy);
		}

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

			if (!sna_gc_move_to_cpu(gc, dst, region))
				return;

			miCopyRegion(src, dst, gc,
				     region, dx, dy,
				     fbCopyNtoN, 0, NULL);

			sna_gc_move_to_gpu(gc);
		}
	}
}

typedef void (*sna_copy_func)(DrawablePtr src, DrawablePtr dst, GCPtr gc,
			      RegionPtr region, int dx, int dy,
			      Pixel bitPlane, void *closure);

inline static bool
box_intersect(BoxPtr a, const BoxRec *b)
{
	if (a->x1 < b->x1)
		a->x1 = b->x1;
	if (a->x2 > b->x2)
		a->x2 = b->x2;
	if (a->y1 < b->y1)
		a->y1 = b->y1;
	if (a->y2 > b->y2)
		a->y2 = b->y2;

	return a->x1 < a->x2 && a->y1 < a->y2;
}

static RegionPtr
sna_do_copy(DrawablePtr src, DrawablePtr dst, GCPtr gc,
	    int sx, int sy,
	    int width, int height,
	    int dx, int dy,
	    sna_copy_func copy, Pixel bitPlane, void *closure)
{
	RegionPtr clip, free_clip = NULL;
	RegionRec region;
	bool expose;

	DBG(("%s: src=(%d, %d), dst=(%d, %d), size=(%dx%d)\n",
	     __FUNCTION__, sx, sy, dx, dy, width, height));

	/* Short cut for unmapped windows */
	if (dst->type == DRAWABLE_WINDOW && !((WindowPtr)dst)->realized) {
		DBG(("%s: unmapped\n", __FUNCTION__));
		return NULL;
	}

	if (src->pScreen->SourceValidate)
		src->pScreen->SourceValidate(src, sx, sy,
					     width, height,
					     gc->subWindowMode);

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

	DBG(("%s: dst extents (%d, %d), (%d, %d)\n", __FUNCTION__,
	     region.extents.x1, region.extents.y1,
	     region.extents.x2, region.extents.y2));

	if (!box_intersect(&region.extents, &gc->pCompositeClip->extents)) {
		DBG(("%s: dst clipped out\n", __FUNCTION__));
		return NULL;
	}

	region.extents.x1 = clamp(region.extents.x1, sx - dx);
	region.extents.x2 = clamp(region.extents.x2, sx - dx);
	region.extents.y1 = clamp(region.extents.y1, sy - dy);
	region.extents.y2 = clamp(region.extents.y2, sy - dy);

	/* Compute source clip region */
	clip = NULL;
	if (src == dst && gc->clientClipType == CT_NONE) {
		DBG(("%s: using gc clip for src\n", __FUNCTION__));
		clip = gc->pCompositeClip;
	} else if (src->type == DRAWABLE_PIXMAP) {
		DBG(("%s: pixmap -- no source clipping\n", __FUNCTION__));
	} else if (gc->subWindowMode == IncludeInferiors) {
		/*
		 * XFree86 DDX empties the border clip when the
		 * VT is inactive, make sure the region isn't empty
		 */
		if (((WindowPtr)src)->parent ||
		    RegionNil(&((WindowPtr)src)->borderClip)) {
			DBG(("%s: include inferiors\n", __FUNCTION__));
			free_clip = clip = NotClippedByChildren((WindowPtr)src);
		}
	} else {
		DBG(("%s: window clip\n", __FUNCTION__));
		clip = &((WindowPtr)src)->clipList;
	}
	if (clip == NULL) {
		DBG(("%s: fast source clip against extents\n", __FUNCTION__));
		expose = true;
		if (region.extents.x1 < src->x) {
			region.extents.x1 = src->x;
			expose = false;
		}
		if (region.extents.y1 < src->y) {
			region.extents.y1 = src->y;
			expose = false;
		}
		if (region.extents.x2 > src->x + (int) src->width) {
			region.extents.x2 = src->x + (int) src->width;
			expose = false;
		}
		if (region.extents.y2 > src->y + (int) src->height) {
			region.extents.y2 = src->y + (int) src->height;
			expose = false;
		}
		if (box_empty(&region.extents))
			return NULL;
	} else {
		expose = false;
		RegionIntersect(&region, &region, clip);
		if (free_clip)
			RegionDestroy(free_clip);
	}
	DBG(("%s: src extents (%d, %d), (%d, %d) x %d\n", __FUNCTION__,
	     region.extents.x1, region.extents.y1,
	     region.extents.x2, region.extents.y2,
	     RegionNumRects(&region)));
	RegionTranslate(&region, dx-sx, dy-sy);
	if (gc->pCompositeClip->data)
		RegionIntersect(&region, &region, gc->pCompositeClip);
	DBG(("%s: copy region (%d, %d), (%d, %d) x %d\n", __FUNCTION__,
	     region.extents.x1, region.extents.y1,
	     region.extents.x2, region.extents.y2,
	     RegionNumRects(&region)));

	if (RegionNotEmpty(&region))
		copy(src, dst, gc, &region, sx-dx, sy-dy, bitPlane, closure);
	RegionUninit(&region);

	/* Pixmap sources generate a NoExposed (we return NULL to do this) */
	clip = NULL;
	if (!expose && gc->fExpose)
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
	     __FUNCTION__, RegionNumRects(region),
	     region->extents.x1, region->extents.y1,
	     region->extents.x2, region->extents.y2,
	     dx, dy, gc->alu));

	if (!sna_gc_move_to_cpu(gc, dst, region))
		return;

	RegionTranslate(region, dx, dy);
	if (!sna_drawable_move_region_to_cpu(src, region, MOVE_READ))
		goto out_gc;
	RegionTranslate(region, -dx, -dy);

	if (src == dst ||
	    get_drawable_pixmap(src) == get_drawable_pixmap(dst)) {
		DBG(("%s: self-copy\n", __FUNCTION__));
		if (!sna_drawable_move_to_cpu(dst, MOVE_WRITE | MOVE_READ))
			goto out_gc;
	} else {
		if (!sna_drawable_move_region_to_cpu(dst, region,
						     drawable_gc_flags(dst, gc, false)))
			goto out_gc;
	}

	miCopyRegion(src, dst, gc,
		     region, dx, dy,
		     fbCopyNtoN, 0, NULL);
	FALLBACK_FLUSH(dst);
out_gc:
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

	DBG(("%s: src=(%d, %d)x(%d, %d)+(%d, %d) -> dst=(%d, %d)+(%d, %d); alu=%d, pm=%lx\n",
	     __FUNCTION__,
	     src_x, src_y, width, height, src->x, src->y,
	     dst_x, dst_y, dst->x, dst->y,
	     gc->alu, gc->planemask));

	if (FORCE_FALLBACK || !ACCEL_COPY_AREA || wedged(sna) ||
	    !PM_IS_SOLID(dst, gc->planemask))
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

	last.x = drawable->x + data->dx;
	last.y = drawable->y + data->dy;
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

			b->x2 = b->x1 + 1;
			b->y2 = b->y1 + 1;
			b++;
		} while (--nbox);
		op->boxes(data->sna, op, box, b - box);
	}
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
			       data->bo, gc->alu, gc->fgPixel))
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
	struct sna_fill_op *op = data->op;

	if (op->base.u.blt.pixel == gc->fgPixel)
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
	struct sna_fill_op *op = data->op;

	if (op->base.u.blt.pixel == gc->fgPixel)
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
	struct sna_fill_op *op = data->op;

	if (op->base.u.blt.pixel == gc->fgPixel)
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
	struct sna_fill_op *op = data->op;

	if (op->base.u.blt.pixel == gc->fgPixel)
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

	if (!sna_fill_init_blt(&fill, sna, pixmap, bo, gc->alu, pixel))
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
		region_maybe_clip(&clip, gc->pCompositeClip);
		if (RegionNil(&clip))
			return true;

		assert(dx + clip.extents.x1 >= 0);
		assert(dy + clip.extents.y1 >= 0);
		assert(dx + clip.extents.x2 <= pixmap->drawable.width);
		assert(dy + clip.extents.y2 <= pixmap->drawable.height);

		DBG(("%s: clip %d x [(%d, %d), (%d, %d)] x %d [(%d, %d)...]\n",
		     __FUNCTION__,
		     REGION_NUM_RECTS(&clip),
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
		region_maybe_clip(&clip, gc->pCompositeClip);
		if (RegionNil(&clip))
			return true;

		assert(dx + clip.extents.x1 >= 0);
		assert(dy + clip.extents.y1 >= 0);
		assert(dx + clip.extents.x2 <= pixmap->drawable.width);
		assert(dy + clip.extents.y2 <= pixmap->drawable.height);

		DBG(("%s: clip %d x [(%d, %d), (%d, %d)] x %d [(%d, %d)...]\n",
		     __FUNCTION__,
		     REGION_NUM_RECTS(&clip),
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
	region_maybe_clip(&region, gc->pCompositeClip);
	if (RegionNil(&region))
		return;

	if (!sna_gc_move_to_cpu(gc, drawable, &region))
		goto out;
	if (!sna_drawable_move_region_to_cpu(drawable, &region,
					     drawable_gc_flags(drawable, gc, n > 1)))
		goto out_gc;

	DBG(("%s: fbFillSpans\n", __FUNCTION__));
	fbFillSpans(drawable, gc, n, pt, width, sorted);
	FALLBACK_FLUSH(drawable);
out_gc:
	sna_gc_move_to_gpu(gc);
out:
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
	region_maybe_clip(&region, gc->pCompositeClip);
	if (RegionNil(&region))
		return;

	if (!sna_gc_move_to_cpu(gc, drawable, &region))
		goto out;
	if (!sna_drawable_move_region_to_cpu(drawable, &region,
					     drawable_gc_flags(drawable, gc, n > 1)))
		goto out_gc;

	DBG(("%s: fbSetSpans\n", __FUNCTION__));
	fbSetSpans(drawable, gc, src, pt, width, n, sorted);
	FALLBACK_FLUSH(drawable);
out_gc:
	sna_gc_move_to_gpu(gc);
out:
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
	BoxPtr box;
	int n;

	DBG(("%s: plane=%x (%d,%d),(%d,%d)x%d\n",
	     __FUNCTION__, (unsigned)bitplane, RegionNumRects(region),
	     region->extents.x1, region->extents.y1,
	     region->extents.x2, region->extents.y2));

	box = RegionRects(region);
	n = RegionNumRects(region);
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
		if (src_stride <= 128) {
			src_stride = ALIGN(src_stride, 8) / 4;
			if (!kgem_check_batch(&sna->kgem, 7+src_stride) ||
			    !kgem_check_bo_fenced(&sna->kgem, arg->bo) ||
			    !kgem_check_reloc(&sna->kgem, 1)) {
				kgem_submit(&sna->kgem);
				if (!kgem_check_bo_fenced(&sna->kgem, arg->bo))
					return; /* XXX fallback? */
				_kgem_set_mode(&sna->kgem, KGEM_BLT);
			}

			b = sna->kgem.batch + sna->kgem.nbatch;
			b[0] = XY_MONO_SRC_COPY_IMM | (5 + src_stride) | br00;
			b[0] |= ((box->x1 + sx) & 7) << 17;
			b[1] = br13;
			b[2] = (box->y1 + dy) << 16 | (box->x1 + dx);
			b[3] = (box->y2 + dy) << 16 | (box->x2 + dx);
			b[4] = kgem_add_reloc(&sna->kgem, sna->kgem.nbatch + 4,
					      arg->bo,
					      I915_GEM_DOMAIN_RENDER << 16 |
					      I915_GEM_DOMAIN_RENDER |
					      KGEM_RELOC_FENCED,
					      0);
			b[5] = gc->bgPixel;
			b[6] = gc->fgPixel;

			sna->kgem.nbatch += 7 + src_stride;

			dst = (uint8_t *)&b[7];
			src_stride = bitmap->devKind;
			src = bitmap->devPrivate.ptr;
			src += (box->y1 + sy) * src_stride + bx1/8;
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

			if (!kgem_check_batch(&sna->kgem, 8) ||
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

			b = sna->kgem.batch + sna->kgem.nbatch;
			b[0] = XY_MONO_SRC_COPY | br00;
			b[0] |= ((box->x1 + sx) & 7) << 17;
			b[1] = br13;
			b[2] = (box->y1 + dy) << 16 | (box->x1 + dx);
			b[3] = (box->y2 + dy) << 16 | (box->x2 + dx);
			b[4] = kgem_add_reloc(&sna->kgem, sna->kgem.nbatch + 4,
					      arg->bo,
					      I915_GEM_DOMAIN_RENDER << 16 |
					      I915_GEM_DOMAIN_RENDER |
					      KGEM_RELOC_FENCED,
					      0);
			b[5] = kgem_add_reloc(&sna->kgem, sna->kgem.nbatch + 5,
					      upload,
					      I915_GEM_DOMAIN_RENDER << 16 |
					      KGEM_RELOC_FENCED,
					      0);
			b[6] = gc->bgPixel;
			b[7] = gc->fgPixel;

			sna->kgem.nbatch += 8;

			dst = ptr;
			src_stride = bitmap->devKind;
			src = bitmap->devPrivate.ptr;
			src += (box->y1 + sy) * src_stride + bx1/8;
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
	BoxPtr box = RegionRects(region);
	int n = RegionNumRects(region);

	DBG(("%s: plane=%x [%d] x%d\n", __FUNCTION__,
	     (unsigned)bitplane, bit, n));

	if (n == 0)
		return;

	get_drawable_deltas(source, src_pixmap, &dx, &dy);
	sx += dx;
	sy += dy;

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
		uint32_t *b;
		struct kgem_bo *upload;
		void *ptr;

		DBG(("%s: box(%d, %d), (%d, %d), sx=(%d,%d) bx=[%d, %d]\n",
		     __FUNCTION__,
		     box->x1, box->y1,
		     box->x2, box->y2,
		     sx, sy, bx1, bx2));

		if (!kgem_check_batch(&sna->kgem, 8) ||
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

		b = sna->kgem.batch + sna->kgem.nbatch;
		b[0] = br00 | ((box->x1 + sx) & 7) << 17;
		b[1] = br13;
		b[2] = (box->y1 + dy) << 16 | (box->x1 + dx);
		b[3] = (box->y2 + dy) << 16 | (box->x2 + dx);
		b[4] = kgem_add_reloc(&sna->kgem, sna->kgem.nbatch + 4,
				      arg->bo,
				      I915_GEM_DOMAIN_RENDER << 16 |
				      I915_GEM_DOMAIN_RENDER |
				      KGEM_RELOC_FENCED,
				      0);
		b[5] = kgem_add_reloc(&sna->kgem, sna->kgem.nbatch + 5,
				      upload,
				      I915_GEM_DOMAIN_RENDER << 16 |
				      KGEM_RELOC_FENCED,
				      0);
		b[6] = gc->bgPixel;
		b[7] = gc->fgPixel;

		sna->kgem.nbatch += 8;
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
	if (RegionNil(&region))
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
			assert(arg.bo == sna_pixmap_get_bo(pixmap));
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
		goto out_gc;

	DBG(("%s: fbCopyPlane(%d, %d, %d, %d, %d,%d) %x\n",
	     __FUNCTION__, src_x, src_y, w, h, dst_x, dst_y, (unsigned)bit));
	ret = miDoCopy(src, dst, gc,
		       src_x, src_y, w, h, dst_x, dst_y,
		       src->bitsPerPixel > 1 ? fbCopyNto1 : fbCopy1toN,
		       bit, 0);
	FALLBACK_FLUSH(dst);
out_gc:
	sna_gc_move_to_gpu(gc);
out:
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

	if (!sna_fill_init_blt(&fill, sna, pixmap, bo, gc->alu, gc->fgPixel))
		return false;

	get_drawable_deltas(drawable, pixmap, &dx, &dy);

	last.x = drawable->x;
	last.y = drawable->y;

	if (!clipped) {
		last.x += dx;
		last.y += dy;

		assert_pixmap_contains_points(pixmap, pt, n, last.x, last.y);
		sna_damage_add_points(damage, pt, n, last.x, last.y);
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
		while (--n) {
			++pt;
			box_add_pt(&box, pt->x, pt->y);
		}
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
	region_maybe_clip(&region, gc->pCompositeClip);
	if (RegionNil(&region))
		return;

	if (!sna_gc_move_to_cpu(gc, drawable, &region))
		goto out;
	if (!sna_drawable_move_region_to_cpu(drawable, &region,
					     MOVE_READ | MOVE_WRITE))
		goto out_gc;

	DBG(("%s: fbPolyPoint\n", __FUNCTION__));
	fbPolyPoint(drawable, gc, mode, n, pt, flags);
	FALLBACK_FLUSH(drawable);
out_gc:
	sna_gc_move_to_gpu(gc);
out:
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
	if (!sna_fill_init_blt(&fill, sna, pixmap, bo, gc->alu, gc->fgPixel))
		return false;

	get_drawable_deltas(drawable, pixmap, &dx, &dy);

	region_set(&clip, extents);
	if (clipped) {
		region_maybe_clip(&clip, gc->pCompositeClip);
		if (RegionNil(&clip))
			return true;
	}

	jump = _jump[(damage != NULL) | !!(dx|dy) << 1];
	DBG(("%s: [clipped=%x] extents=(%d, %d), (%d, %d), delta=(%d, %d), damage=%p\n",
	     __FUNCTION__, clipped,
	     clip.extents.x1, clip.extents.y1,
	     clip.extents.x2, clip.extents.y2,
	     dx, dy, damage));

	extents = REGION_RECTS(&clip);
	last_extents = extents + REGION_NUM_RECTS(&clip);

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

	DBG(("%s: alu=%d, fg=%08x\n", __FUNCTION__, gc->alu, (unsigned)pixel));

	if (!sna_fill_init_blt(&fill, sna, pixmap, bo, gc->alu, pixel))
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
		region_maybe_clip(&clip, gc->pCompositeClip);
		if (RegionNil(&clip))
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

	DBG(("%s: extents (%d, %d), (%d, %d)\n", __FUNCTION__,
	     data.region.extents.x1, data.region.extents.y1,
	     data.region.extents.x2, data.region.extents.y2));

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

	DBG(("%s: fill=%d [%d], line=%d [%d], width=%d, mask=%lu [%d], rectlinear=%d\n",
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
						       data.bo, gc->alu, color))
					goto fallback;

				data.op = &fill;

				if ((data.flags & 2) == 0) {
					if (data.dx | data.dy)
						sna_gc_ops__tmp.FillSpans = sna_fill_spans__fill_offset;
					else
						sna_gc_ops__tmp.FillSpans = sna_fill_spans__fill;
				} else {
					region_maybe_clip(&data.region,
							  gc->pCompositeClip);
					if (RegionNil(&data.region))
						return;

					if (region_is_singular(&data.region))
						sna_gc_ops__tmp.FillSpans = sna_fill_spans__fill_clip_extents;
					else
						sna_gc_ops__tmp.FillSpans = sna_fill_spans__fill_clip_boxes;
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
				} else {
					region_maybe_clip(&data.region,
							  gc->pCompositeClip);
					if (RegionNil(&data.region))
						return;

					if (region_is_singular(&data.region))
						sna_gc_ops__tmp.FillSpans = sna_fill_spans__dash_clip_extents;
					else
						sna_gc_ops__tmp.FillSpans = sna_fill_spans__dash_clip_boxes;
				}
				assert(gc->miTranslate);

				DBG(("%s: miZeroLine (solid dash)\n", __FUNCTION__));
				if (!sna_fill_init_blt(&fill,
						       data.sna, data.pixmap,
						       data.bo, gc->alu, color))
					goto fallback;

				gc->ops = &sna_gc_ops__tmp;
				miZeroDashLine(drawable, gc, mode, n, pt);
				fill.done(data.sna, &fill);

				if (sna_fill_init_blt(&fill,
						       data.sna, data.pixmap,
						       data.bo, gc->alu,
						       gc->bgPixel)) {
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
	region_maybe_clip(&data.region, gc->pCompositeClip);
	if (RegionNil(&data.region))
		return;

	if (!sna_gc_move_to_cpu(gc, drawable, &data.region))
		goto out;
	if (!sna_drawable_move_region_to_cpu(drawable, &data.region,
					     drawable_gc_flags(drawable, gc,
							       !(data.flags & 4 && n == 2))))
		goto out_gc;

	DBG(("%s: fbPolyLine\n", __FUNCTION__));
	fbPolyLine(drawable, gc, mode, n, pt);
	FALLBACK_FLUSH(drawable);

out_gc:
	sna_gc_move_to_gpu(gc);
out:
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

	if (!sna_fill_init_blt(&fill, sna, pixmap, bo, gc->alu, pixel))
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
		region_maybe_clip(&clip, gc->pCompositeClip);
		if (RegionNil(&clip))
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
	if (!sna_fill_init_blt(&fill, sna, pixmap, bo, gc->alu, gc->fgPixel))
		return false;

	get_drawable_deltas(drawable, pixmap, &dx, &dy);

	region_set(&clip, extents);
	if (clipped) {
		region_maybe_clip(&clip, gc->pCompositeClip);
		if (RegionNil(&clip))
			return true;
	}
	DBG(("%s: [clipped] extents=(%d, %d), (%d, %d), delta=(%d, %d)\n",
	     __FUNCTION__,
	     clip.extents.x1, clip.extents.y1,
	     clip.extents.x2, clip.extents.y2,
	     dx, dy));

	jump = _jump[(damage != NULL) | !!(dx|dy) << 1];

	b = box;
	extents = REGION_RECTS(&clip);
	last_extents = extents + REGION_NUM_RECTS(&clip);
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
					       data.bo, gc->alu, color))
				goto fallback;

			data.op = &fill;

			if ((data.flags & 2) == 0) {
				if (data.dx | data.dy)
					sna_gc_ops__tmp.FillSpans = sna_fill_spans__fill_offset;
				else
					sna_gc_ops__tmp.FillSpans = sna_fill_spans__fill;
			} else {
				region_maybe_clip(&data.region,
						  gc->pCompositeClip);
				if (RegionNil(&data.region))
					return;

				if (region_is_singular(&data.region))
					sna_gc_ops__tmp.FillSpans = sna_fill_spans__fill_clip_extents;
				else
					sna_gc_ops__tmp.FillSpans = sna_fill_spans__fill_clip_boxes;
			}
			assert(gc->miTranslate);
			gc->ops = &sna_gc_ops__tmp;
			for (i = 0; i < n; i++)
				line(drawable, gc, CoordModeOrigin, 2,
				     (DDXPointPtr)&seg[i]);

			fill.done(data.sna, &fill);
		} else {
			sna_gc_ops__tmp.FillSpans = sna_fill_spans__gpu;
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
	region_maybe_clip(&data.region, gc->pCompositeClip);
	if (RegionNil(&data.region))
		return;

	if (!sna_gc_move_to_cpu(gc, drawable, &data.region))
		goto out;
	if (!sna_drawable_move_region_to_cpu(drawable, &data.region,
					     drawable_gc_flags(drawable, gc,
							       !(data.flags & 4 && n == 1))))
		goto out_gc;

	DBG(("%s: fbPolySegment\n", __FUNCTION__));
	fbPolySegment(drawable, gc, n, seg);
	FALLBACK_FLUSH(drawable);

out_gc:
	sna_gc_move_to_gpu(gc);
out:
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

	clipped = box32_trim_and_translate(&box, drawable, gc);
	if (!box32_to_box16(&box, out))
		return 0;

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
	if (!sna_fill_init_blt(&fill, sna, pixmap, bo, gc->alu, gc->fgPixel))
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
		region_maybe_clip(&clip, gc->pCompositeClip);
		if (RegionNil(&clip))
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
		region_maybe_clip(&clip, gc->pCompositeClip);
		DBG(("%s: wide clipped: extents=((%d, %d), (%d, %d))\n",
		     __FUNCTION__,
		     clip.extents.x1, clip.extents.y1,
		     clip.extents.x2, clip.extents.y2));
		if (RegionNil(&clip))
			goto done;

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

	DBG(("%s: fill=_%d [%d], line=%d [%d], join=%d [%d], mask=%lu [%d]\n",
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
	DBG(("%s: fallback\n", __FUNCTION__));

	region.data = NULL;
	region_maybe_clip(&region, gc->pCompositeClip);
	if (RegionNil(&region))
		return;

	if (!sna_gc_move_to_cpu(gc, drawable, &region))
		goto out;
	if (!sna_drawable_move_region_to_cpu(drawable, &region,
					     drawable_gc_flags(drawable, gc, true)))
		goto out_gc;

	DBG(("%s: miPolyRectangle\n", __FUNCTION__));
	miPolyRectangle(drawable, gc, n, r);
	FALLBACK_FLUSH(drawable);
out_gc:
	sna_gc_move_to_gpu(gc);
out:
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
						       data.bo, gc->alu, color))
					goto fallback;

				if ((data.flags & 2) == 0) {
					if (data.dx | data.dy)
						sna_gc_ops__tmp.FillSpans = sna_fill_spans__fill_offset;
					else
						sna_gc_ops__tmp.FillSpans = sna_fill_spans__fill;
					sna_gc_ops__tmp.PolyPoint = sna_poly_point__fill;
				} else {
					region_maybe_clip(&data.region,
							  gc->pCompositeClip);
					if (RegionNil(&data.region))
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
				region_maybe_clip(&data.region,
						  gc->pCompositeClip);
				if (RegionNil(&data.region))
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
	region_maybe_clip(&data.region, gc->pCompositeClip);
	if (RegionNil(&data.region))
		return;

	if (!sna_gc_move_to_cpu(gc, drawable, &data.region))
		goto out;
	if (!sna_drawable_move_region_to_cpu(drawable, &data.region,
					     MOVE_READ | MOVE_WRITE))
		goto out_gc;

	DBG(("%s -- fbPolyArc\n", __FUNCTION__));
	fbPolyArc(drawable, gc, n, arc);
	FALLBACK_FLUSH(drawable);

out_gc:
	sna_gc_move_to_gpu(gc);
out:
	RegionUninit(&data.region);
}

static bool
sna_poly_fill_rect_blt(DrawablePtr drawable,
		       struct kgem_bo *bo,
		       struct sna_damage **damage,
		       GCPtr gc, uint32_t pixel,
		       int n, xRectangle *rect,
		       const BoxRec *extents,
		       bool clipped)
{
	PixmapPtr pixmap = get_drawable_pixmap(drawable);
	struct sna *sna = to_sna_from_pixmap(pixmap);
	struct sna_fill_op fill;
	BoxRec boxes[512], *b = boxes, *const last_box = boxes+ARRAY_SIZE(boxes);
	int16_t dx, dy;

	DBG(("%s x %d [(%d, %d)x(%d, %d)...]+(%d,%d), clipped?=%d\n",
	     __FUNCTION__, n,
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
			get_drawable_deltas(drawable, pixmap, &dx, &dy);
			r.x1 += dx; r.y1 += dy;
			r.x2 += dx; r.y2 += dy;
			if (sna->render.fill_one(sna, pixmap, bo, pixel,
						 r.x1, r.y1, r.x2, r.y2,
						 gc->alu)) {
				if (damage) {
					assert_pixmap_contains_box(pixmap, &r);
					if (r.x2 - r.x1 == pixmap->drawable.width &&
					    r.y2 - r.y1 == pixmap->drawable.height) {
						sna_damage_all(damage,
							       pixmap->drawable.width,
							       pixmap->drawable.height);
					} else
						sna_damage_add_box(damage, &r);
				}
				assert_pixmap_damage(pixmap);

				if ((gc->alu == GXcopy || gc->alu == GXclear) &&
				    r.x2 - r.x1 == pixmap->drawable.width &&
				    r.y2 - r.y1 == pixmap->drawable.height) {
					struct sna_pixmap *priv = sna_pixmap(pixmap);
					if (bo == priv->gpu_bo) {
						assert(priv->gpu_bo->proxy == NULL);
						sna_damage_all(&priv->gpu_damage,
							       pixmap->drawable.width,
							       pixmap->drawable.height);
						sna_damage_destroy(&priv->cpu_damage);
						list_del(&priv->list);
						priv->clear = true;
						priv->clear_color = gc->alu == GXcopy ? pixel : 0;

						DBG(("%s: pixmap=%ld, marking clear [%08x]\n",
						     __FUNCTION__, pixmap->drawable.serialNumber, priv->clear_color));
					}
				}
			} else
				success = false;
		}

		return success;
	}

	if (!sna_fill_init_blt(&fill, sna, pixmap, bo, gc->alu, pixel)) {
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
		region_maybe_clip(&clip, gc->pCompositeClip);
		if (RegionNil(&clip))
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
	DBG(("%s\n", __FUNCTION__));
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
					       data.bo, gc->alu, color))
				goto fallback;

			data.op = &fill;

			if ((data.flags & 2) == 0) {
				if (data.dx | data.dy)
					sna_gc_ops__tmp.FillSpans = sna_fill_spans__fill_offset;
				else
					sna_gc_ops__tmp.FillSpans = sna_fill_spans__fill;
			} else {
				region_maybe_clip(&data.region,
						  gc->pCompositeClip);
				if (RegionNil(&data.region))
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
	region_maybe_clip(&data.region, gc->pCompositeClip);
	if (RegionNil(&data.region)) {
		DBG(("%s: nothing to do, all clipped\n", __FUNCTION__));
		return;
	}

	if (!sna_gc_move_to_cpu(gc, draw, &data.region))
		goto out;
	if (!sna_drawable_move_region_to_cpu(draw, &data.region,
					     drawable_gc_flags(draw, gc, true)))
		goto out_gc;

	DBG(("%s: fallback -- miFillPolygon -> sna_fill_spans__cpu\n",
	     __FUNCTION__));
	miFillPolygon(draw, gc, shape, mode, n, pt);
out_gc:
	sna_gc_move_to_gpu(gc);
out:
	RegionUninit(&data.region);
}

static struct kgem_bo *
sna_pixmap_get_source_bo(PixmapPtr pixmap)
{
	struct sna_pixmap *priv = sna_pixmap(pixmap);

	if (priv == NULL) {
		struct kgem_bo *upload;
		struct sna *sna = to_sna_from_pixmap(pixmap);
		void *ptr;

		upload = kgem_create_buffer_2d(&sna->kgem,
					       pixmap->drawable.width,
					       pixmap->drawable.height,
					       pixmap->drawable.bitsPerPixel,
					       KGEM_BUFFER_WRITE_INPLACE,
					       &ptr);
		if (upload == NULL)
			return NULL;

		memcpy_blt(pixmap->devPrivate.ptr, ptr,
			   pixmap->drawable.bitsPerPixel,
			   pixmap->devKind, upload->pitch,
			   0, 0,
			   0, 0,
			   pixmap->drawable.width,
			   pixmap->drawable.height);

		return upload;
	}

	if (priv->gpu_damage &&
	    !sna_pixmap_move_to_gpu(pixmap, MOVE_READ | MOVE_ASYNC_HINT))
		return NULL;

	if (priv->cpu_damage && priv->cpu_bo)
		return kgem_bo_reference(priv->cpu_bo);

	if (!sna_pixmap_force_to_gpu(pixmap, MOVE_READ))
		return NULL;

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

	DBG(("%s x %d [(%d, %d)x(%d, %d)...], clipped=%x\n",
	     __FUNCTION__, n, r->x, r->y, r->width, r->height, clipped));

	kgem_set_mode(&sna->kgem, KGEM_BLT, bo);
	if (!kgem_check_batch(&sna->kgem, 8+2*3) ||
	    !kgem_check_reloc(&sna->kgem, 2) ||
	    !kgem_check_bo_fenced(&sna->kgem, bo)) {
		kgem_submit(&sna->kgem);
		if (!kgem_check_bo_fenced(&sna->kgem, bo))
			return false;
		_kgem_set_mode(&sna->kgem, KGEM_BLT);
	}

	br00 = XY_SCANLINE_BLT;
	br13 = bo->pitch;
	if (sna->kgem.gen >= 040 && bo->tiling) {
		br00 |= BLT_DST_TILED;
		br13 >>= 2;
	}
	br13 |= blt_depth(drawable->depth) << 24;
	br13 |= fill_ROP[gc->alu] << 16;

	get_drawable_deltas(drawable, pixmap, &dx, &dy);
	assert(extents->x1 + dx >= 0);
	assert(extents->y1 + dy >= 0);
	assert(extents->x2 + dx <= pixmap->drawable.width);
	assert(extents->y2 + dy <= pixmap->drawable.height);

	if (!clipped) {
		dx += drawable->x;
		dy += drawable->y;

		sna_damage_add_rectangles(damage, r, n, dx, dy);
		if (n == 1) {
			tx = (r->x - origin->x) % 8;
			if (tx < 0)
				tx = 8 - tx;
			ty = (r->y - origin->y) % 8;
			if (ty < 0)
				ty = 8 - ty;

			assert(r->x + dx >= 0);
			assert(r->y + dy >= 0);
			assert(r->x + dx + r->width  <= pixmap->drawable.width);
			assert(r->y + dy + r->height <= pixmap->drawable.height);

			b = sna->kgem.batch + sna->kgem.nbatch;
			b[0] = XY_PAT_BLT | tx << 12 | ty << 8 | 3 << 20 | (br00 & BLT_DST_TILED);
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
		} else do {
			int n_this_time;

			b = sna->kgem.batch + sna->kgem.nbatch;
			b[0] = XY_SETUP_BLT | 3 << 20;
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

			n_this_time = n;
			if (3*n_this_time > sna->kgem.surface - sna->kgem.nbatch - KGEM_BATCH_RESERVED)
				n_this_time = (sna->kgem.surface - sna->kgem.nbatch - KGEM_BATCH_RESERVED) / 3;
			assert(n_this_time);
			n -= n_this_time;

			b = sna->kgem.batch + sna->kgem.nbatch;
			sna->kgem.nbatch += 3*n_this_time;
			do {
				assert(r->x + dx >= 0);
				assert(r->y + dy >= 0);
				assert(r->x + dx + r->width  <= pixmap->drawable.width);
				assert(r->y + dy + r->height <= pixmap->drawable.height);

				tx = (r->x - origin->x) % 8;
				if (tx < 0)
					tx = 8 - tx;
				ty = (r->y - origin->y) % 8;
				if (ty < 0)
					ty = 8 - ty;

				b[0] = br00 | tx << 12 | ty << 8;
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

		region_set(&clip, extents);
		region_maybe_clip(&clip, gc->pCompositeClip);
		if (RegionNil(&clip))
			goto done;

		b = sna->kgem.batch + sna->kgem.nbatch;
		b[0] = XY_SETUP_BLT | 3 << 20;
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

		if (clip.data == NULL) {
			const BoxRec *c = &clip.extents;
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
						b = sna->kgem.batch + sna->kgem.nbatch;
						b[0] = XY_SETUP_BLT | 3 << 20;
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

					assert(box.x1 + dx >= 0);
					assert(box.y1 + dy >= 0);
					assert(box.x2 + dx <= pixmap->drawable.width);
					assert(box.y2 + dy <= pixmap->drawable.height);

					ty = (box.y1 - drawable->y - origin->y) % 8;
					if (ty < 0)
						ty = 8 - ty;

					tx = (box.x1 - drawable->x - origin->x) % 8;
					if (tx < 0)
						tx = 8 - tx;

					b = sna->kgem.batch + sna->kgem.nbatch;
					b[0] = br00 | tx << 12 | ty << 8;
					b[1] = (box.y1 + dy) << 16 | (box.x1 + dx);
					b[2] = (box.y2 + dy) << 16 | (box.x2 + dx);
					sna->kgem.nbatch += 3;
				}
			}
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
							b = sna->kgem.batch + sna->kgem.nbatch;
							b[0] = XY_SETUP_BLT | 3 << 20;
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

						assert(bb.x1 + dx >= 0);
						assert(bb.y1 + dy >= 0);
						assert(bb.x2 + dx <= pixmap->drawable.width);
						assert(bb.y2 + dy <= pixmap->drawable.height);

						ty = (bb.y1 - drawable->y - origin->y) % 8;
						if (ty < 0)
							ty = 8 - ty;

						tx = (bb.x1 - drawable->x - origin->x) % 8;
						if (tx < 0)
							tx = 8 - tx;

						b = sna->kgem.batch + sna->kgem.nbatch;
						b[0] = br00 | tx << 12 | ty << 8;
						b[1] = (bb.y1 + dy) << 16 | (bb.x1 + dx);
						b[2] = (bb.y2 + dy) << 16 | (bb.x2 + dx);
						sna->kgem.nbatch += 3;
					}
				}
			} while (--n);
		}
	}
done:
	assert_pixmap_damage(pixmap);
	sna->blt_state.fill_bo = 0;
	return true;
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
	struct kgem_bo *upload;
	int w, h, cpp;
	void *ptr;
	bool ret;

	DBG(("%s: %dx%d\n", __FUNCTION__,
	     tile->drawable.width, tile->drawable.height));

	if (!sna_pixmap_move_to_cpu(tile, MOVE_READ))
		return false;

	upload = kgem_create_buffer(&sna->kgem, 8*tile->drawable.bitsPerPixel,
				    KGEM_BUFFER_WRITE_INPLACE,
				    &ptr);
	if (upload == NULL)
		return false;

	assert(tile->drawable.height && tile->drawable.height <= 8);
	assert(tile->drawable.width && tile->drawable.width <= 8);

	cpp = tile->drawable.bitsPerPixel/8;
	for (h = 0; h < tile->drawable.height; h++) {
		uint8_t *src = (uint8_t *)tile->devPrivate.ptr + tile->devKind*h;
		uint8_t *dst = (uint8_t *)ptr + 8*cpp*h;

		w = tile->drawable.width*cpp;
		memcpy(dst, src, w);
		while (w < 8*cpp) {
			memcpy(dst+w, dst, w);
			w *= 2;
		}
	}
	while (h < 8) {
		memcpy((uint8_t*)ptr + h*w, ptr, h*w);
		h *= 2;
	}

	ret = sna_poly_fill_rect_tiled_8x8_blt(drawable, bo, damage,
					       upload, gc, n, rect,
					       extents, clipped);

	kgem_bo_destroy(&sna->kgem, upload);
	return ret;
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

	DBG(("%s x %d [(%d, %d)x(%d, %d)...]\n",
	     __FUNCTION__, n, rect->x, rect->y, rect->width, rect->height));

	tile_width = tile->drawable.width;
	tile_height = tile->drawable.height;
	if ((tile_width | tile_height) == 1) {
		DBG(("%s: single pixel tile pixmap ,converting to solid fill\n",
		     __FUNCTION__));
		return sna_poly_fill_rect_blt(drawable, bo, damage,
					      gc, get_pixel(tile),
					      n, rect,
					      extents, clipped);
	}

	/* XXX [248]x[238] tiling can be reduced to a pattern fill.
	 * Also we can do the lg2 reduction for BLT and use repeat modes for
	 * RENDER.
	 */

	if ((tile->drawable.width | tile->drawable.height) == 8) {
		bool ret;

		tile_bo = sna_pixmap_get_source_bo(tile);
		ret = sna_poly_fill_rect_tiled_8x8_blt(drawable, bo, damage,
						       tile_bo, gc, n, rect,
						       extents, clipped);
		kgem_bo_destroy(&sna->kgem, tile_bo);

		return ret;
	}

	if ((tile->drawable.width | tile->drawable.height) <= 0xc &&
	    is_power_of_two(tile->drawable.width) &&
	    is_power_of_two(tile->drawable.height))
		return sna_poly_fill_rect_tiled_nxm_blt(drawable, bo, damage,
							gc, n, rect,
							extents, clipped);

	tile_bo = sna_pixmap_get_source_bo(tile);
	if (tile_bo == NULL) {
		DBG(("%s: unable to move tile go GPU, fallback\n",
		     __FUNCTION__));
		return false;
	}

	if (!sna_copy_init_blt(&copy, sna, tile, tile_bo, pixmap, bo, alu)) {
		DBG(("%s: unsupported blt\n", __FUNCTION__));
		kgem_bo_destroy(&sna->kgem, tile_bo);
		return false;
	}

	get_drawable_deltas(drawable, pixmap, &dx, &dy);
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
		region_maybe_clip(&clip, gc->pCompositeClip);
		if (RegionNil(&clip))
			goto done;

		if (clip.data == NULL) {
			const BoxRec *box = &clip.extents;
			while (n--) {
				BoxRec r;

				r.x1 = rect->x + drawable->x;
				r.y1 = rect->y + drawable->y;
				r.x2 = bound(r.x1, rect->width);
				r.y2 = bound(r.y1, rect->height);
				rect++;

				if (box_intersect(&r, box)) {
					int height = r.y2 - r.y1;
					int dst_y = r.y1;
					int tile_y = (r.y1 - drawable->y - origin->y) % tile_height;
					if (tile_y < 0)
						tile_y += tile_height;

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
				BoxRec *box;
				int nbox;

				region.extents.x1 = rect->x + drawable->x;
				region.extents.y1 = rect->y + drawable->y;
				region.extents.x2 = bound(region.extents.x1, rect->width);
				region.extents.y2 = bound(region.extents.y1, rect->height);
				rect++;

				region.data = NULL;
				RegionIntersect(&region, &region, &clip);

				nbox = REGION_NUM_RECTS(&region);
				box = REGION_RECTS(&region);
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
		unsigned px = (0 - gc->patOrg.x - dx) & 7;
		unsigned py = (0 - gc->patOrg.y - dy) & 7;
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
	if (!kgem_check_batch(&sna->kgem, 9 + 2*3) ||
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

			b = sna->kgem.batch + sna->kgem.nbatch;
			b[0] = XY_MONO_PAT | (br00 & (BLT_DST_TILED | 0x7<<12 | 0x7<<8)) | 3<<20;
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
		} else do {
			int n_this_time;

			b = sna->kgem.batch + sna->kgem.nbatch;
			b[0] = XY_SETUP_MONO_PATTERN_SL_BLT | 3 << 20;
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

			n_this_time = n;
			if (3*n_this_time > sna->kgem.surface - sna->kgem.nbatch - KGEM_BATCH_RESERVED)
				n_this_time = (sna->kgem.surface - sna->kgem.nbatch - KGEM_BATCH_RESERVED) / 3;
			assert(n_this_time);
			n -= n_this_time;

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
		region_maybe_clip(&clip, gc->pCompositeClip);
		if (RegionNil(&clip))
			return true;

		b = sna->kgem.batch + sna->kgem.nbatch;
		b[0] = XY_SETUP_MONO_PATTERN_SL_BLT | 3 << 20;
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

						b = sna->kgem.batch + sna->kgem.nbatch;
						b[0] = XY_SETUP_MONO_PATTERN_SL_BLT | 3 << 20;
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

							b = sna->kgem.batch + sna->kgem.nbatch;
							b[0] = XY_SETUP_MONO_PATTERN_SL_BLT | 3 << 20;
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

	DBG(("%s: upload (%d, %d), (%d, %d), origin (%d, %d)\n", __FUNCTION__,
	     extents->x1, extents->y1,
	     extents->x2, extents->y2,
	     origin->x, origin->y));

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
			if (src_stride <= 128) {
				src_stride = ALIGN(src_stride, 8) / 4;
				if (!kgem_check_batch(&sna->kgem, 7+src_stride) ||
				    !kgem_check_bo_fenced(&sna->kgem, bo) ||
				    !kgem_check_reloc(&sna->kgem, 1)) {
					kgem_submit(&sna->kgem);
					if (!kgem_check_bo_fenced(&sna->kgem, bo))
						return false;
					_kgem_set_mode(&sna->kgem, KGEM_BLT);
				}

				b = sna->kgem.batch + sna->kgem.nbatch;
				b[0] = XY_MONO_SRC_COPY_IMM | (5 + src_stride) | br00;
				b[0] |= ((r->x - origin->x) & 7) << 17;
				b[1] = br13;
				b[2] = (r->y + dy) << 16 | (r->x + dx);
				b[3] = (r->y + r->height + dy) << 16 | (r->x + r->width + dx);
				b[4] = kgem_add_reloc(&sna->kgem,
						      sna->kgem.nbatch + 4, bo,
						      I915_GEM_DOMAIN_RENDER << 16 |
						      I915_GEM_DOMAIN_RENDER |
						      KGEM_RELOC_FENCED,
						      0);
				b[5] = gc->bgPixel;
				b[6] = gc->fgPixel;

				sna->kgem.nbatch += 7 + src_stride;

				dst = (uint8_t *)&b[7];
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

				if (!kgem_check_batch(&sna->kgem, 8) ||
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

				dst = ptr;
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
				b = sna->kgem.batch + sna->kgem.nbatch;
				b[0] = XY_MONO_SRC_COPY | br00;
				b[0] |= ((r->x - origin->x) & 7) << 17;
				b[1] = br13;
				b[2] = (r->y + dy) << 16 | (r->x + dx);
				b[3] = (r->y + r->height + dy) << 16 | (r->x + r->width + dx);
				b[4] = kgem_add_reloc(&sna->kgem,
						      sna->kgem.nbatch + 4, bo,
						      I915_GEM_DOMAIN_RENDER << 16 |
						      I915_GEM_DOMAIN_RENDER |
						      KGEM_RELOC_FENCED,
						      0);
				b[5] = kgem_add_reloc(&sna->kgem, sna->kgem.nbatch + 5,
						      upload,
						      I915_GEM_DOMAIN_RENDER << 16 |
						      KGEM_RELOC_FENCED,
						      0);
				b[6] = gc->bgPixel;
				b[7] = gc->fgPixel;

				sna->kgem.nbatch += 8;
				kgem_bo_destroy(&sna->kgem, upload);
			}

			r++;
		} while (--n);
	} else {
		RegionRec clip;
		DDXPointRec pat;

		region_set(&clip, extents);
		region_maybe_clip(&clip, gc->pCompositeClip);
		if (RegionNil(&clip))
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
				box.x2 = bound(r->x, r->width);
				box.y1 = r->y + drawable->y;
				box.y2 = bound(r->y, r->height);
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
				if (src_stride <= 128) {
					src_stride = ALIGN(src_stride, 8) / 4;
					if (!kgem_check_batch(&sna->kgem, 7+src_stride) ||
					    !kgem_check_bo_fenced(&sna->kgem, bo) ||
					    !kgem_check_reloc(&sna->kgem, 1)) {
						kgem_submit(&sna->kgem);
						if (!kgem_check_bo_fenced(&sna->kgem, bo))
							return false;
						_kgem_set_mode(&sna->kgem, KGEM_BLT);
					}

					b = sna->kgem.batch + sna->kgem.nbatch;
					b[0] = XY_MONO_SRC_COPY_IMM | (5 + src_stride) | br00;
					b[0] |= ((box.x1 - pat.x) & 7) << 17;
					b[1] = br13;
					b[2] = (box.y1 + dy) << 16 | (box.x1 + dx);
					b[3] = (box.y2 + dy) << 16 | (box.x2 + dx);
					b[4] = kgem_add_reloc(&sna->kgem,
							      sna->kgem.nbatch + 4, bo,
							      I915_GEM_DOMAIN_RENDER << 16 |
							      I915_GEM_DOMAIN_RENDER |
							      KGEM_RELOC_FENCED,
							      0);
					b[5] = gc->bgPixel;
					b[6] = gc->fgPixel;

					sna->kgem.nbatch += 7 + src_stride;

					dst = (uint8_t *)&b[7];
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
					if (!kgem_check_batch(&sna->kgem, 8) ||
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

					dst = ptr;
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

					b = sna->kgem.batch + sna->kgem.nbatch;
					b[0] = XY_MONO_SRC_COPY | br00;
					b[0] |= ((box.x1 - pat.x) & 7) << 17;
					b[1] = br13;
					b[2] = (box.y1 + dy) << 16 | (box.x1 + dx);
					b[3] = (box.y2 + dy) << 16 | (box.x2 + dx);
					b[4] = kgem_add_reloc(&sna->kgem,
							      sna->kgem.nbatch + 4, bo,
							      I915_GEM_DOMAIN_RENDER << 16 |
							      I915_GEM_DOMAIN_RENDER |
							      KGEM_RELOC_FENCED,
							      0);
					b[5] = kgem_add_reloc(&sna->kgem, sna->kgem.nbatch + 5,
							      upload,
							      I915_GEM_DOMAIN_RENDER << 16 |
							      KGEM_RELOC_FENCED,
							      0);
					b[6] = gc->bgPixel;
					b[7] = gc->fgPixel;

					sna->kgem.nbatch += 8;
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
				unclipped.x2 = bound(r->x, r->width);
				unclipped.y1 = r->y + drawable->y;
				unclipped.y2 = bound(r->y, r->height);
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
					if (src_stride <= 128) {
						src_stride = ALIGN(src_stride, 8) / 4;
						if (!kgem_check_batch(&sna->kgem, 7+src_stride) ||
						    !kgem_check_bo_fenced(&sna->kgem, bo) ||
						    !kgem_check_reloc(&sna->kgem, 1)) {
							kgem_submit(&sna->kgem);
							if (!kgem_check_bo_fenced(&sna->kgem, bo))
								return false;
							_kgem_set_mode(&sna->kgem, KGEM_BLT);
						}

						b = sna->kgem.batch + sna->kgem.nbatch;
						b[0] = XY_MONO_SRC_COPY_IMM | (5 + src_stride) | br00;
						b[0] |= ((box.x1 - pat.x) & 7) << 17;
						b[1] = br13;
						b[2] = (box.y1 + dy) << 16 | (box.x1 + dx);
						b[3] = (box.y2 + dy) << 16 | (box.x2 + dx);
						b[4] = kgem_add_reloc(&sna->kgem,
								      sna->kgem.nbatch + 4, bo,
								      I915_GEM_DOMAIN_RENDER << 16 |
								      I915_GEM_DOMAIN_RENDER |
								      KGEM_RELOC_FENCED,
								      0);
						b[5] = gc->bgPixel;
						b[6] = gc->fgPixel;

						sna->kgem.nbatch += 7 + src_stride;

						dst = (uint8_t *)&b[7];
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
						if (!kgem_check_batch(&sna->kgem, 8) ||
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

						dst = ptr;
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

						b = sna->kgem.batch + sna->kgem.nbatch;
						b[0] = XY_MONO_SRC_COPY | br00;
						b[0] |= ((box.x1 - pat.x) & 7) << 17;
						b[1] = br13;
						b[2] = (box.y1 + dy) << 16 | (box.x1 + dx);
						b[3] = (box.y2 + dy) << 16 | (box.x2 + dx);
						b[4] = kgem_add_reloc(&sna->kgem,
								      sna->kgem.nbatch + 4, bo,
								      I915_GEM_DOMAIN_RENDER << 16 |
								      I915_GEM_DOMAIN_RENDER |
								      KGEM_RELOC_FENCED,
								      0);
						b[5] = kgem_add_reloc(&sna->kgem, sna->kgem.nbatch + 5,
								      upload,
								      I915_GEM_DOMAIN_RENDER << 16 |
								      KGEM_RELOC_FENCED,
								      0);
						b[6] = gc->bgPixel;
						b[7] = gc->fgPixel;

						sna->kgem.nbatch += 8;
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
			if (!kgem_check_batch(&sna->kgem, 7+len) ||
			    !kgem_check_bo_fenced(&sna->kgem, bo) ||
			    !kgem_check_reloc(&sna->kgem, 1)) {
				kgem_submit(&sna->kgem);
				if (!kgem_check_bo_fenced(&sna->kgem, bo))
					return; /* XXX fallback? */
				_kgem_set_mode(&sna->kgem, KGEM_BLT);
			}

			b = sna->kgem.batch + sna->kgem.nbatch;
			b[0] = br00 | (5 + len) | (ox & 7) << 17;
			b[1] = br13;
			b[2] = y1 << 16 | x1;
			b[3] = y2 << 16 | x2;
			b[4] = kgem_add_reloc(&sna->kgem, sna->kgem.nbatch + 4,
					      bo,
					      I915_GEM_DOMAIN_RENDER << 16 |
					      I915_GEM_DOMAIN_RENDER |
					      KGEM_RELOC_FENCED,
					      0);
			b[5] = gc->bgPixel;
			b[6] = gc->fgPixel;

			sna->kgem.nbatch += 7 + len;

			dst = (uint8_t *)&b[7];
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
			if (!kgem_check_batch(&sna->kgem, 7+len) ||
			    !kgem_check_bo_fenced(&sna->kgem, bo) ||
			    !kgem_check_reloc(&sna->kgem, 2)) {
				kgem_submit(&sna->kgem);
				if (!kgem_check_bo_fenced(&sna->kgem, bo))
					return; /* XXX fallback? */
				_kgem_set_mode(&sna->kgem, KGEM_BLT);
			}

			b = sna->kgem.batch + sna->kgem.nbatch;

			if (!use_tile && len <= 128) {
				uint8_t *dst, *src;

				b[0] = XY_MONO_SRC_COPY_IMM;
				b[0] |= (br00 & (BLT_DST_TILED | 3 << 20));
				b[0] |= (ox & 7) << 17;
				b[0] |= (5 + len);
				b[1] = br13;
				b[2] = y1 << 16 | x1;
				b[3] = y2 << 16 | x2;
				b[4] = kgem_add_reloc(&sna->kgem, sna->kgem.nbatch + 4,
						      bo,
						      I915_GEM_DOMAIN_RENDER << 16 |
						      I915_GEM_DOMAIN_RENDER |
						      KGEM_RELOC_FENCED,
						      0);
				b[5] = gc->bgPixel;
				b[6] = gc->fgPixel;

				sna->kgem.nbatch += 7 + len;

				dst = (uint8_t *)&b[7];
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

				b = sna->kgem.batch + sna->kgem.nbatch;
				b[0] = br00 | (ox & 7) << 17;
				b[1] = br13;
				b[2] = y1 << 16 | x1;
				b[3] = y2 << 16 | x2;
				b[4] = kgem_add_reloc(&sna->kgem, sna->kgem.nbatch + 4,
						      bo,
						      I915_GEM_DOMAIN_RENDER << 16 |
						      I915_GEM_DOMAIN_RENDER |
						      KGEM_RELOC_FENCED,
						      0);
				b[5] = kgem_add_reloc(&sna->kgem, sna->kgem.nbatch + 5,
						      upload,
						      I915_GEM_DOMAIN_RENDER << 16 |
						      KGEM_RELOC_FENCED,
						      0);
				b[6] = gc->bgPixel;
				b[7] = gc->fgPixel;

				sna->kgem.nbatch += 8;

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
		region_maybe_clip(&clip, gc->pCompositeClip);
		if (RegionNil(&clip)) {
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
		region_maybe_clip(&clip, gc->pCompositeClip);
		if (RegionNil(&clip)) {
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
		assert(bo == sna_pixmap_get_bo(pixmap));
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

	if (extents->x2 - gc->patOrg.x - drawable->x <= stipple->drawable.width &&
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
		DBG(("%s: fallback -- planemask=%#lx (not-solid)\n",
		     __FUNCTION__, gc->planemask));
		goto fallback;
	}

	/* Clear the cpu damage so that we refresh the GPU status of the
	 * pixmap upon a redraw after a period of inactivity.
	 */
	hint = PREFER_GPU;
	if (n == 1 && gc->fillStyle != FillStippled && alu_overwrites(gc->alu)) {
		region.data = NULL;
		if (priv->cpu_damage &&
		    region_is_singular(gc->pCompositeClip)) {
			if (region_subsumes_damage(&region, priv->cpu_damage)) {
				DBG(("%s: discarding existing CPU damage\n", __FUNCTION__));
				if (priv->gpu_bo && priv->gpu_bo->proxy) {
					kgem_bo_destroy(&sna->kgem, priv->gpu_bo);
					priv->gpu_bo = NULL;
				}
				sna_damage_destroy(&priv->cpu_damage);
				list_del(&priv->list);
			}
			hint |= IGNORE_CPU;
		}
		if (priv->cpu_damage == NULL &&
		    (region_subsumes_drawable(&region, &pixmap->drawable) ||
		     box_inplace(pixmap, &region.extents))) {
			DBG(("%s: promoting to full GPU\n", __FUNCTION__));
			if (priv->gpu_bo) {
				assert(priv->gpu_bo->proxy == NULL);
				sna_damage_all(&priv->gpu_damage,
					       pixmap->drawable.width,
					       pixmap->drawable.height);
			}
		}
		if (priv->cpu_damage == NULL) {
			DBG(("%s: dropping last-cpu hint\n", __FUNCTION__));
			priv->cpu = false;
		}
	}

	/* If the source is already on the GPU, keep the operation on the GPU */
	if (gc->fillStyle == FillTiled) {
		if (!gc->tileIsPixel && sna_pixmap_is_gpu(gc->tile.pixmap)) {
			DBG(("%s: source is already on the gpu\n", __FUNCTION__));
			hint |= PREFER_GPU | FORCE_GPU;
		}
	}

	bo = sna_drawable_use_bo(draw, hint, &region.extents, &damage);
	if (bo == NULL) {
		DBG(("%s: not using GPU, hint=%x\n", __FUNCTION__, hint));
		goto fallback;
	}

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
	region_maybe_clip(&region, gc->pCompositeClip);
	if (RegionNil(&region)) {
		DBG(("%s: nothing to do, all clipped\n", __FUNCTION__));
		return;
	}

	if (!sna_gc_move_to_cpu(gc, draw, &region))
		goto out;
	if (!sna_drawable_move_region_to_cpu(draw, &region,
					     drawable_gc_flags(draw, gc, n > 1)))
		goto out_gc;

	DBG(("%s: fallback - fbPolyFillRect\n", __FUNCTION__));
	fbPolyFillRect(draw, gc, n, rect);
	FALLBACK_FLUSH(draw);
out_gc:
	sna_gc_move_to_gpu(gc);
out:
	RegionUninit(&region);
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
					       data.bo, gc->alu, color))
				goto fallback;

			data.op = &fill;

			if ((data.flags & 2) == 0) {
				if (data.dx | data.dy)
					sna_gc_ops__tmp.FillSpans = sna_fill_spans__fill_offset;
				else
					sna_gc_ops__tmp.FillSpans = sna_fill_spans__fill;
			} else {
				region_maybe_clip(&data.region,
						  gc->pCompositeClip);
				if (RegionNil(&data.region))
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
	region_maybe_clip(&data.region, gc->pCompositeClip);
	if (RegionNil(&data.region)) {
		DBG(("%s: nothing to do, all clipped\n", __FUNCTION__));
		return;
	}

	if (!sna_gc_move_to_cpu(gc, draw, &data.region))
		goto out;
	if (!sna_drawable_move_region_to_cpu(draw, &data.region,
					     drawable_gc_flags(draw, gc, true)))
		goto out_gc;

	DBG(("%s: fallback -- miPolyFillArc -> sna_fill_spans__cpu\n",
	     __FUNCTION__));

	miPolyFillArc(draw, gc, n, arc);
out_gc:
	sna_gc_move_to_gpu(gc);
out:
	RegionUninit(&data.region);
}

struct sna_font {
	CharInfoRec glyphs8[256];
	CharInfoRec *glyphs16[256];
};
#define GLYPH_INVALID (void *)1
#define GLYPH_EMPTY (void *)2
#define GLYPH_CLEAR (void *)3

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

	uint8_t rop = transparent ? copy_ROP[gc->alu] : ROP_S;

	DBG(("%s (%d, %d) x %d, fg=%08x, bg=%08x alu=%02x\n",
	     __FUNCTION__, _x, _y, _n, fg, bg, rop));

	if (wedged(sna)) {
		DBG(("%s: fallback -- wedged\n", __FUNCTION__));
		return false;
	}

	bo = sna_drawable_use_bo(drawable, PREFER_GPU, &clip->extents, &damage);
	if (bo == NULL)
		return false;

	if (bo->tiling == I915_TILING_Y) {
		DBG(("%s: converting bo from Y-tiling\n", __FUNCTION__));
		assert(bo == sna_pixmap_get_bo(pixmap));
		bo = sna_pixmap_change_tiling(pixmap, I915_TILING_X);
		if (bo == NULL) {
			DBG(("%s: fallback -- unable to change tiling\n",
			     __FUNCTION__));
			return false;
		}
	}

	get_drawable_deltas(drawable, pixmap, &dx, &dy);
	_x += drawable->x + dx;
	_y += drawable->y + dy;

	RegionTranslate(clip, dx, dy);
	extents = REGION_RECTS(clip);
	last_extents = extents + REGION_NUM_RECTS(clip);

	if (!transparent) /* emulate miImageGlyphBlt */
		sna_blt_fill_boxes(sna, GXcopy,
				   bo, drawable->bitsPerPixel,
				   bg, extents, REGION_NUM_RECTS(clip));

	kgem_set_mode(&sna->kgem, KGEM_BLT, bo);
	if (!kgem_check_batch(&sna->kgem, 16) ||
	    !kgem_check_bo_fenced(&sna->kgem, bo) ||
	    !kgem_check_reloc(&sna->kgem, 1)) {
		kgem_submit(&sna->kgem);
		if (!kgem_check_bo_fenced(&sna->kgem, bo))
			return false;
		_kgem_set_mode(&sna->kgem, KGEM_BLT);
	}

	DBG(("%s: glyph clip box (%d, %d), (%d, %d)\n",
	     __FUNCTION__,
	     extents->x1, extents->y1,
	     extents->x2, extents->y2));

	b = sna->kgem.batch + sna->kgem.nbatch;
	b[0] = XY_SETUP_BLT | 3 << 20;
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

			if (!transparent && c->bits == GLYPH_CLEAR)
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


			if (!kgem_check_batch(&sna->kgem, 3+len)) {
				_kgem_submit(&sna->kgem);
				_kgem_set_mode(&sna->kgem, KGEM_BLT);

				DBG(("%s: new batch, glyph clip box (%d, %d), (%d, %d)\n",
				     __FUNCTION__,
				     extents->x1, extents->y1,
				     extents->x2, extents->y2));

				b = sna->kgem.batch + sna->kgem.nbatch;
				b[0] = XY_SETUP_BLT | 3 << 20;
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

			b = sna->kgem.batch + sna->kgem.nbatch;
			sna->kgem.nbatch += 3 + len;

			b[0] = br00 | (1 + len);
			b[1] = (uint16_t)y1 << 16 | (uint16_t)x1;
			b[2] = (uint16_t)(y1+h) << 16 | (uint16_t)(x1+w);
			if (c->bits == GLYPH_CLEAR) {
				memset(b+3, 0, len*4);
			} else {
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
		out->bits = GLYPH_CLEAR;
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
	region_maybe_clip(&region, gc->pCompositeClip);
	if (RegionNil(&region))
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
			goto out_gc;

		DBG(("%s: fallback -- fbPolyGlyphBlt\n", __FUNCTION__));
		fbPolyGlyphBlt(drawable, gc, x, y, n,
			       info, FONTGLYPHS(gc->font));
		FALLBACK_FLUSH(drawable);
out_gc:
		sna_gc_move_to_gpu(gc);
	}
out:
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
	region_maybe_clip(&region, gc->pCompositeClip);
	if (RegionNil(&region))
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
			goto out_gc;

		DBG(("%s: fallback -- fbPolyGlyphBlt\n", __FUNCTION__));
		fbPolyGlyphBlt(drawable, gc, x, y, n,
			       info, FONTGLYPHS(gc->font));
		FALLBACK_FLUSH(drawable);
out_gc:
		sna_gc_move_to_gpu(gc);
	}
out:
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
	region_maybe_clip(&region, gc->pCompositeClip);
	if (RegionNil(&region))
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
		if (!sna_drawable_move_region_to_cpu(drawable, &region,
						     MOVE_READ | MOVE_WRITE))
			goto out_gc;

		DBG(("%s: fallback -- fbImageGlyphBlt\n", __FUNCTION__));
		fbImageGlyphBlt(drawable, gc, x, y, n,
				info, FONTGLYPHS(gc->font));
		FALLBACK_FLUSH(drawable);
out_gc:
		sna_gc_move_to_gpu(gc);
	}
out:
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
	region_maybe_clip(&region, gc->pCompositeClip);
	if (RegionNil(&region))
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
		if (!sna_drawable_move_region_to_cpu(drawable, &region,
						     MOVE_READ | MOVE_WRITE))
			goto out_gc;

		DBG(("%s: fallback -- fbImageGlyphBlt\n", __FUNCTION__));
		fbImageGlyphBlt(drawable, gc, x, y, n,
				info, FONTGLYPHS(gc->font));
		FALLBACK_FLUSH(drawable);
out_gc:
		sna_gc_move_to_gpu(gc);
	}
out:
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

	if (bo->tiling == I915_TILING_Y) {
		DBG(("%s: converting bo from Y-tiling\n", __FUNCTION__));
		assert(bo == sna_pixmap_get_bo(pixmap));
		bo = sna_pixmap_change_tiling(pixmap, I915_TILING_X);
		if (bo == NULL) {
			DBG(("%s: fallback -- unable to change tiling\n",
			     __FUNCTION__));
			return false;
		}
	}

	get_drawable_deltas(drawable, pixmap, &dx, &dy);
	_x += drawable->x + dx;
	_y += drawable->y + dy;

	RegionTranslate(clip, dx, dy);
	extents = REGION_RECTS(clip);
	last_extents = extents + REGION_NUM_RECTS(clip);

	if (!transparent) /* emulate miImageGlyphBlt */
		sna_blt_fill_boxes(sna, GXcopy,
				   bo, drawable->bitsPerPixel,
				   bg, extents, REGION_NUM_RECTS(clip));

	kgem_set_mode(&sna->kgem, KGEM_BLT, bo);
	if (!kgem_check_batch(&sna->kgem, 16) ||
	    !kgem_check_bo_fenced(&sna->kgem, bo) ||
	    !kgem_check_reloc(&sna->kgem, 1)) {
		kgem_submit(&sna->kgem);
		if (!kgem_check_bo_fenced(&sna->kgem, bo))
			return false;
		_kgem_set_mode(&sna->kgem, KGEM_BLT);
	}

	DBG(("%s: glyph clip box (%d, %d), (%d, %d)\n",
	     __FUNCTION__,
	     extents->x1, extents->y1,
	     extents->x2, extents->y2));
	b = sna->kgem.batch + sna->kgem.nbatch;
	b[0] = XY_SETUP_BLT | 1 << 20;
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

			if (!transparent) {
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

			if (!kgem_check_batch(&sna->kgem, 3+len)) {
				_kgem_submit(&sna->kgem);
				_kgem_set_mode(&sna->kgem, KGEM_BLT);

				DBG(("%s: new batch, glyph clip box (%d, %d), (%d, %d)\n",
				     __FUNCTION__,
				     extents->x1, extents->y1,
				     extents->x2, extents->y2));

				b = sna->kgem.batch + sna->kgem.nbatch;
				b[0] = XY_SETUP_BLT | 1 << 20;
				b[1] = bo->pitch;
				if (sna->kgem.gen >= 040 && bo->tiling) {
					b[0] |= BLT_DST_TILED;
					b[1] >>= 2;
				}
				b[1] |= 1 << 30 | transparent << 29 | blt_depth(drawable->depth) << 24 | rop << 16;
				b[2] = extents->y1 << 16 | extents->x1;
				b[3] = extents->y2 << 16 | extents->x2;
				b[4] = kgem_add_reloc(&sna->kgem, sna->kgem.nbatch + 4,
						      bo,
						      I915_GEM_DOMAIN_RENDER << 16 |
						      I915_GEM_DOMAIN_RENDER |
						      KGEM_RELOC_FENCED,
						      0);
				b[5] = bg;
				b[6] = fg;
				b[7] = 0;
				sna->kgem.nbatch += 8;
			}

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
	region_maybe_clip(&region, gc->pCompositeClip);
	if (RegionNil(&region))
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

	if ((bo = sna_drawable_use_bo(drawable, PREFER_GPU,
				      &region.extents, &damage)) &&
	    sna_reversed_glyph_blt(drawable, gc, x, y, n, info, base,
				   bo, damage, &region,
				   gc->fgPixel, gc->bgPixel, false))
		goto out;

fallback:
	DBG(("%s: fallback\n", __FUNCTION__));
	if (!sna_gc_move_to_cpu(gc, drawable, &region))
		goto out;
	if (!sna_drawable_move_region_to_cpu(drawable, &region,
					     MOVE_READ | MOVE_WRITE))
		goto out_gc;

	DBG(("%s: fallback -- fbImageGlyphBlt\n", __FUNCTION__));
	fbImageGlyphBlt(drawable, gc, x, y, n, info, base);
	FALLBACK_FLUSH(drawable);

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
	region_maybe_clip(&region, gc->pCompositeClip);
	if (RegionNil(&region))
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
		goto out;
	if (!sna_drawable_move_region_to_cpu(drawable, &region,
					     MOVE_READ | MOVE_WRITE))
		goto out_gc;

	DBG(("%s: fallback -- fbPolyGlyphBlt\n", __FUNCTION__));
	fbPolyGlyphBlt(drawable, gc, x, y, n, info, base);
	FALLBACK_FLUSH(drawable);

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
	BoxRec *box;
	int16_t dx, dy;
	int n;
	uint8_t rop = copy_ROP[gc->alu];

	bo = sna_drawable_use_bo(drawable, PREFER_GPU, &region->extents, &damage);
	if (bo == NULL)
		return false;

	if (bo->tiling == I915_TILING_Y) {
		DBG(("%s: converting bo from Y-tiling\n", __FUNCTION__));
		assert(bo == sna_pixmap_get_bo(pixmap));
		bo = sna_pixmap_change_tiling(pixmap, I915_TILING_X);
		if (bo == NULL) {
			DBG(("%s: fallback -- unable to change tiling\n",
			     __FUNCTION__));
			return false;
		}
	}

	get_drawable_deltas(drawable, pixmap, &dx, &dy);
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
	box = REGION_RECTS(region);
	n = REGION_NUM_RECTS(region);
	do {
		int bx1 = (box->x1 - region->extents.x1) & ~7;
		int bx2 = (box->x2 - region->extents.x1 + 7) & ~7;
		int bw = (bx2 - bx1)/8;
		int bh = box->y2 - box->y1;
		int bstride = ALIGN(bw, 2);
		int src_stride;
		uint8_t *dst, *src;
		uint32_t *b;
		struct kgem_bo *upload;
		void *ptr;

		if (!kgem_check_batch(&sna->kgem, 8) ||
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

		dst = ptr;

		src_stride = bitmap->devKind;
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

		b = sna->kgem.batch + sna->kgem.nbatch;
		b[0] = XY_MONO_SRC_COPY | 3 << 20;
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
		b[5] = kgem_add_reloc(&sna->kgem, sna->kgem.nbatch + 5,
				      upload,
				      I915_GEM_DOMAIN_RENDER << 16 |
				      KGEM_RELOC_FENCED,
				      0);
		b[6] = gc->bgPixel;
		b[7] = gc->fgPixel;

		sna->kgem.nbatch += 8;
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
	region_maybe_clip(&region, gc->pCompositeClip);
	if (RegionNil(&region))
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
		goto out_gc;
	if (!sna_drawable_move_region_to_cpu(drawable, &region,
					     drawable_gc_flags(drawable, gc, false)))
		goto out_gc;

	DBG(("%s: fallback, fbPushPixels(%d, %d, %d %d)\n",
	     __FUNCTION__, w, h, x, y));
	fbPushPixels(gc, bitmap, drawable, w, h, x, y);
	FALLBACK_FLUSH(drawable);
out_gc:
	sna_gc_move_to_gpu(gc);
out:
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
	DBG(("%s changes=%lx\n", __FUNCTION__, changes));

	if (changes & (GCClipMask|GCSubwindowMode) ||
	    drawable->serialNumber != (gc->serialNumber & DRAWABLE_SERIAL_BITS) ||
	    (gc->clientClipType != CT_NONE && (changes & (GCClipXOrigin | GCClipYOrigin))))
		miComputeCompositeClip(gc, drawable);

	sna_gc(gc)->changes |= changes;
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

	fb_gc(gc)->bpp = bits_per_pixel(gc->depth);

	gc->funcs = (GCFuncs *)&sna_gc_funcs;
	gc->ops = (GCOps *)&sna_gc_ops;
	return true;
}

static bool
sna_get_image_blt(DrawablePtr drawable,
		  RegionPtr region,
		  char *dst)
{
	PixmapPtr pixmap = get_drawable_pixmap(drawable);
	struct sna_pixmap *priv = sna_pixmap(pixmap);
	struct sna *sna = to_sna_from_pixmap(pixmap);
	struct kgem_bo *dst_bo;
	bool ok = false;
	int pitch;

	if (!USE_USERPTR_DOWNLOADS)
		return false;

	if (priv == NULL)
		return false;

	if (priv->clear) {
		int w = region->extents.x2 - region->extents.x1;
		int h = region->extents.y2 - region->extents.y1;

		pitch = PixmapBytePad(w, pixmap->drawable.depth);
		if (priv->clear_color == 0 ||
		    pixmap->drawable.bitsPerPixel == 8) {
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

	if (!sna->kgem.has_userptr)
		return false;

	if (!DAMAGE_IS_ALL(priv->gpu_damage) ||
	    !__kgem_bo_is_busy(&sna->kgem, priv->gpu_bo))
		return false;

	DBG(("%s: download through a temporary map\n", __FUNCTION__));

	pitch = PixmapBytePad(region->extents.x2 - region->extents.x1,
			      drawable->depth);
	dst_bo = kgem_create_map(&sna->kgem, dst,
				 pitch * (region->extents.y2 - region->extents.y1),
				 false);
	if (dst_bo) {
		int16_t dx, dy;

		dst_bo->flush = true;
		dst_bo->pitch = pitch;
		dst_bo->reusable = false;

		get_drawable_deltas(drawable, pixmap, &dx, &dy);

		ok = sna->render.copy_boxes(sna, GXcopy,
					    pixmap, priv->gpu_bo, dx, dy,
					    pixmap, dst_bo,
					    -region->extents.x1,
					    -region->extents.y1,
					    &region->extents, 1,
					    COPY_LAST);

		kgem_bo_sync__cpu(&sna->kgem, dst_bo);
		kgem_bo_destroy(&sna->kgem, dst_bo);
	}

	return ok;
}

static void
sna_get_image(DrawablePtr drawable,
	      int x, int y, int w, int h,
	      unsigned int format, unsigned long mask,
	      char *dst)
{
	RegionRec region;
	unsigned int flags;
	bool can_blt;

	if (!fbDrawableEnabled(drawable))
		return;

	DBG(("%s (%d, %d)x(%d, %d)\n", __FUNCTION__, x, y, w, h));

	region.extents.x1 = x + drawable->x;
	region.extents.y1 = y + drawable->y;
	region.extents.x2 = region.extents.x1 + w;
	region.extents.y2 = region.extents.y1 + h;
	region.data = NULL;

	can_blt = format == ZPixmap &&
		drawable->bitsPerPixel >= 8 &&
		PM_IS_SOLID(drawable, mask);

	if (can_blt && sna_get_image_blt(drawable, &region, dst))
		return;

	flags = MOVE_READ;
	if ((w | h) == 1)
		flags |= MOVE_INPLACE_HINT;
	if (w == drawable->width)
		flags |= MOVE_WHOLE_HINT;
	if (!sna_drawable_move_region_to_cpu(drawable, &region, flags))
		return;

	if (can_blt) {
		PixmapPtr pixmap = get_drawable_pixmap(drawable);
		int16_t dx, dy;

		DBG(("%s: copy box (%d, %d), (%d, %d)\n",
		     __FUNCTION__,
		     region.extents.x1, region.extents.y1,
		     region.extents.x2, region.extents.y2));
		get_drawable_deltas(drawable, pixmap, &dx, &dy);
		memcpy_blt(pixmap->devPrivate.ptr, dst, drawable->bitsPerPixel,
			   pixmap->devKind, PixmapBytePad(w, drawable->depth),
			   region.extents.x1 + dx,
			   region.extents.y1 + dy,
			   0, 0, w, h);
	} else
		fbGetImage(drawable, x, y, w, h, format, mask, dst);
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
	if (RegionNil(&dst))
		return;

#ifdef COMPOSITE
	if (pixmap->screen_x | pixmap->screen_y)
		RegionTranslate(&dst, -pixmap->screen_x, -pixmap->screen_y);
#endif

	if (wedged(sna) || FORCE_FALLBACK || !ACCEL_COPY_WINDOW) {
		DBG(("%s: fallback -- wedged\n", __FUNCTION__));
		if (!sna_pixmap_move_to_cpu(pixmap, MOVE_READ | MOVE_WRITE))
			return;

		miCopyRegion(&pixmap->drawable, &pixmap->drawable,
			     0, &dst, dx, dy, fbCopyNtoN, 0, NULL);
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

static void
sna_accel_flush_callback(CallbackListPtr *list,
			 pointer user_data, pointer call_data)
{
	struct sna *sna = user_data;
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
					struct sna_pixmap, list);

		list_del(&priv->list);
		if (priv->shm) {
			DBG(("%s: syncing SHM pixmap=%ld (refcnt=%d)\n",
			     __FUNCTION__,
			     priv->pixmap->drawable.serialNumber,
			     priv->pixmap->refcnt));
			ret = sna_pixmap_move_to_cpu(priv->pixmap,
						     MOVE_READ | MOVE_WRITE);
			assert(!ret || priv->gpu_bo == NULL);
			if (priv->pixmap->refcnt == 0)
				__sna_free_pixmap(sna, priv->pixmap, priv);
		} else {
			DBG(("%s: flushing DRI pixmap=%ld\n", __FUNCTION__,
			     priv->pixmap->drawable.serialNumber));
			ret = sna_pixmap_move_to_gpu(priv->pixmap,
						     MOVE_READ | __MOVE_FORCE);
		}
		(void)ret;
	}

	if (sna->kgem.flush)
		kgem_submit(&sna->kgem);
}

static struct sna_pixmap *sna_accel_scanout(struct sna *sna)
{
	struct sna_pixmap *priv;

	if (sna->vblank_interval == 0)
		return NULL;

	if (sna->front == NULL)
		return NULL;

	priv = sna_pixmap(sna->front);
	return priv && priv->gpu_bo ? priv : NULL;
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
		if (RegionNotEmpty(DamageRegion(dirty->damage)))
			return true;
	}
#endif
	return false;
}

static bool has_shadow(struct sna *sna)
{
	DamagePtr damage = sna->mode.shadow_damage;

	if (!(damage && RegionNotEmpty(DamageRegion(damage))))
		return false;

	DBG(("%s: has pending damage\n", __FUNCTION__));
	if ((sna->flags & SNA_TEAR_FREE) == 0)
		return true;

	DBG(("%s: outstanding flips: %d\n",
	     __FUNCTION__, sna->mode.shadow_flip));
	return !sna->mode.shadow_flip;
}

static bool start_flush(struct sna *sna, struct sna_pixmap *scanout)
{
	DBG(("%s: scanout=%d shadow?=%d, slaves?=%d, (cpu?=%d || gpu?=%d))\n",
	     __FUNCTION__,
	     scanout && scanout->gpu_bo ? scanout->gpu_bo->handle : 0,
	     has_shadow(sna), has_offload_slaves(sna),
	     scanout && scanout->cpu_damage != NULL,
	     scanout && scanout->gpu_bo && scanout->gpu_bo->exec != NULL));

	if (has_offload_slaves(sna))
		return true;

	if (has_shadow(sna))
		return true;

	if (!scanout)
		return false;

	return scanout->cpu_damage || scanout->gpu_bo->exec;
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

static bool sna_accel_do_flush(struct sna *sna)
{
	struct sna_pixmap *priv;
	int interval;

	priv = sna_accel_scanout(sna);
	if (priv == NULL && !sna->mode.shadow_active && !has_offload_slaves(sna)) {
		DBG(("%s -- no scanout attached\n", __FUNCTION__));
		sna_accel_disarm_timer(sna, FLUSH_TIMER);
		return false;
	}

	interval = sna->vblank_interval ?: 20;
	if (sna->timer_active & (1<<(FLUSH_TIMER))) {
		int32_t delta = sna->timer_expire[FLUSH_TIMER] - TIME;
		DBG(("%s: flush timer active: delta=%d\n",
		     __FUNCTION__, delta));
		if (delta <= 3) {
			DBG(("%s (time=%ld), triggered\n", __FUNCTION__, (long)TIME));
			sna->timer_expire[FLUSH_TIMER] = TIME + interval;
			return true;
		}
	} else if (!start_flush(sna, priv)) {
		DBG(("%s -- no pending write to scanout\n", __FUNCTION__));
		if (priv)
			kgem_bo_flush(&sna->kgem, priv->gpu_bo);
	} else
		timer_enable(sna, FLUSH_TIMER, interval/2);

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
		BoxPtr box;
		int n;

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

		DBG(("%s: pushing damage ((%d, %d), (%d, %d))x%d to slave pixmap=%ld, ((%d, %d), (%d, %d))\n", __FUNCTION__,
		     damage->extents.x1, damage->extents.y1,
		     damage->extents.x2, damage->extents.y2,
		     RegionNumRects(damage),
		     dst->drawable.serialNumber,
		     region.extents.x1, region.extents.y1,
		     region.extents.x2, region.extents.y2));

		RegionIntersect(&region, &region, damage);
		if (RegionNil(&region))
			goto skip;

		RegionTranslate(&region, -dirty->x, -dirty->y);
		DamageRegionAppend(&dirty->slave_dst->drawable, &region);

		box = REGION_RECTS(&region);
		n = REGION_NUM_RECTS(&region);
		if (wedged(sna)) {
fallback:
			if (!sna_pixmap_move_to_cpu(src, MOVE_READ))
				goto skip;

			if (!sna_pixmap_move_to_cpu(dst, MOVE_READ | MOVE_WRITE | MOVE_INPLACE_HINT))
				goto skip;

			assert(src->drawable.bitsPerPixel == dst->drawable.bitsPerPixel);
			do {
				DBG(("%s: copy box (%d, %d)->(%d, %d)x(%d, %d)\n",
				     __FUNCTION__,
				     box->x1 + dirty->x, box->y1 + dirty->y,
				     box->x1, box->y1,
				     box->x2 - box->x1, box->y2 - box->y1));

				assert(box->x2 > box->x1);
				assert(box->y2 > box->y1);

				assert(box->x1 + dirty->x >= 0);
				assert(box->y1 + dirty->y >= 0);
				assert(box->x2 + dirty->x <= src->drawable.width);
				assert(box->y2 + dirty->y <= src->drawable.height);

				assert(box->x1 >= 0);
				assert(box->y1 >= 0);
				assert(box->x2 <= src->drawable.width);
				assert(box->y2 <= src->drawable.height);

				memcpy_blt(src->devPrivate.ptr,
					   dst->devPrivate.ptr,
					   src->drawable.bitsPerPixel,
					   src->devKind, dst->devKind,
					   box->x1 + dirty->x,
					   box->y1 + dirty->y,
					   box->x1,
					   box->y1,
					   box->x2 - box->x1,
					   box->y2 - box->y1);
				box++;
			} while (--n);
		} else {
			if (!sna_pixmap_move_to_gpu(src, MOVE_READ | MOVE_ASYNC_HINT | __MOVE_FORCE))
				goto fallback;

			if (!sna_pixmap_move_to_gpu(dst, MOVE_READ | MOVE_WRITE | MOVE_ASYNC_HINT | __MOVE_FORCE))
				goto fallback;

			if (!sna->render.copy_boxes(sna, GXcopy,
						    src, sna_pixmap_get_bo(src), dirty->x, dirty->y,
						    dst, sna_pixmap_get_bo(dst),0, 0,
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

static void sna_accel_flush(struct sna *sna)
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

	if (priv) {
		sna_pixmap_force_to_gpu(priv->pixmap,
					MOVE_READ | MOVE_ASYNC_HINT);
		kgem_bo_flush(&sna->kgem, priv->gpu_bo);
		assert(!priv->cpu);
	}

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

static void sna_accel_expire(struct sna *sna)
{
	DBG(("%s (time=%ld)\n", __FUNCTION__, (long)TIME));

	if (!kgem_expire_cache(&sna->kgem))
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
	ErrorF("Allocated bo: %d, %ld bytes\n",
	       sna->kgem.debug_memory.bo_allocs,
	       (long)sna->kgem.debug_memory.bo_bytes);
	ErrorF("Allocated CPU bo: %d, %ld bytes\n",
	       sna->debug_memory.cpu_bo_allocs,
	       (long)sna->debug_memory.cpu_bo_bytes);
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
	*(PixmapPtr *)dixGetPrivateAddr(&window->devPrivates, &sna_window_key) = pixmap;
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
	sna_dri_destroy_window(win);
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
	ps->Triangles = sna_composite_triangles;
#if PICTURE_SCREEN_VERSION >= 2
	ps->TriStrip = sna_composite_tristrip;
	ps->TriFan = sna_composite_trifan;
#endif

	return true;
}

static bool sna_option_accel_blt(struct sna *sna)
{
	const char *s;

	s = xf86GetOptValString(sna->Options, OPTION_ACCEL_METHOD);
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

	if (sna->kgem.has_userptr)
		ShmRegisterFuncs(screen, &shm_funcs);
	else
		ShmRegisterFbFuncs(screen);

	if (!sna_picture_init(screen))
		return false;

	backend = "no";
	sna->have_render = false;
	no_render_init(sna);

	if (sna_option_accel_blt(sna) || sna->info->gen >= 0100) {
	} else if (sna->info->gen >= 070) {
		if ((sna->have_render = gen7_render_init(sna)))
			backend = "IvyBridge";
	} else if (sna->info->gen >= 060) {
		if ((sna->have_render = gen6_render_init(sna)))
			backend = "SandyBridge";
	} else if (sna->info->gen >= 050) {
		if ((sna->have_render = gen5_render_init(sna)))
			backend = "Ironlake";
	} else if (sna->info->gen >= 040) {
		if ((sna->have_render = gen4_render_init(sna)))
			backend = "Broadwater/Crestline";
	} else if (sna->info->gen >= 030) {
		if ((sna->have_render = gen3_render_init(sna)))
			backend = "gen3";
	} else if (sna->info->gen >= 020) {
		if ((sna->have_render = gen2_render_init(sna)))
			backend = "gen2";
	}
	DBG(("%s(backend=%s, have_render=%d)\n",
	     __FUNCTION__, backend, sna->have_render));

	kgem_reset(&sna->kgem);

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
	sna->have_render = false;
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

void sna_accel_close(struct sna *sna)
{
	DBG(("%s\n", __FUNCTION__));

	sna_composite_close(sna);
	sna_gradients_close(sna);
	sna_glyphs_close(sna);

	while (sna->freed_pixmap) {
		PixmapPtr pixmap = sna->freed_pixmap;
		sna->freed_pixmap = pixmap->devPrivate.ptr;
		assert(pixmap->refcnt == 0);
		free(sna_pixmap(pixmap));
		FreePixmap(pixmap);
	}

	DeleteCallback(&FlushCallback, sna_accel_flush_callback, sna);

	kgem_cleanup_cache(&sna->kgem);
}

void sna_accel_block_handler(struct sna *sna, struct timeval **tv)
{
	if (sna->timer_active)
		UpdateCurrentTimeIf();

	if (sna->kgem.nbatch &&
	    (sna->kgem.scanout_busy ||
	     kgem_ring_is_idle(&sna->kgem, sna->kgem.ring))) {
		DBG(("%s: GPU idle, flushing\n", __FUNCTION__));
		_kgem_submit(&sna->kgem);
	}

	if (sna_accel_do_flush(sna))
		sna_accel_flush(sna);
	assert(sna_accel_scanout(sna) == NULL ||
	       sna_accel_scanout(sna)->gpu_bo->exec == NULL ||
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

		timeout = sna->timer_expire[0] - TIME;
		DBG(("%s: flush timer expires in %d [%d]\n",
		     __FUNCTION__, timeout, sna->timer_expire[0]));

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
}

void sna_accel_wakeup_handler(struct sna *sna)
{
	DBG(("%s\n", __FUNCTION__));

	if (sna->kgem.need_retire)
		kgem_retire(&sna->kgem);
	if (sna->kgem.nbatch && !sna->kgem.need_retire) {
		DBG(("%s: GPU idle, flushing\n", __FUNCTION__));
		_kgem_submit(&sna->kgem);
	}
	if (sna->kgem.need_purge)
		kgem_purge_cache(&sna->kgem);
}

void sna_accel_free(struct sna *sna)
{
	DBG(("%s\n", __FUNCTION__));
}
