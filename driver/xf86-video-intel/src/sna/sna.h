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

/*
 * Authors:
 *   Keith Whitwell <keith@tungstengraphics.com>
 *   David Dawes <dawes@xfree86.org>
 *
 */

#ifndef _SNA_H_
#define _SNA_H_

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdint.h>
#include "compiler.h"

#include <xorg-server.h>

#include <xf86Crtc.h>
#if XF86_CRTC_VERSION >= 5
#define HAS_PIXMAP_SHARING 1
#endif

#include <xf86str.h>
#include <windowstr.h>
#include <glyphstr.h>
#include <picturestr.h>
#include <gcstruct.h>

#include <pciaccess.h>

#include <xf86drmMode.h>

#include "../compat-api.h"
#include <drm.h>
#include <i915_drm.h>

#ifdef HAVE_DRI2_H
#include <dri2.h>
#endif

#if HAVE_UDEV
#include <libudev.h>
#endif

#if HAS_DEBUG_FULL
#define DBG(x) ErrorF x
#else
#define DBG(x)
#endif

#define DEBUG_NO_BLT 0

#define DEBUG_FLUSH_BATCH 0

#define TEST_ALL 0
#define TEST_ACCEL (TEST_ALL || 0)
#define TEST_BATCH (TEST_ALL || 0)
#define TEST_BLT (TEST_ALL || 0)
#define TEST_COMPOSITE (TEST_ALL || 0)
#define TEST_DAMAGE (TEST_ALL || 0)
#define TEST_GRADIENT (TEST_ALL || 0)
#define TEST_GLYPHS (TEST_ALL || 0)
#define TEST_IO (TEST_ALL || 0)
#define TEST_KGEM (TEST_ALL || 0)
#define TEST_RENDER (TEST_ALL || 0)

#include "intel_driver.h"
#include "intel_list.h"
#include "kgem.h"
#include "sna_damage.h"
#include "sna_render.h"
#include "fb/fb.h"

#define SNA_CURSOR_X			64
#define SNA_CURSOR_Y			SNA_CURSOR_X

struct sna_pixmap {
	PixmapPtr pixmap;
	struct kgem_bo *gpu_bo, *cpu_bo;
	struct sna_damage *gpu_damage, *cpu_damage;
	void *ptr;
#define PTR(ptr) ((void*)((uintptr_t)(ptr) & ~1))

	struct list list;

	uint32_t stride;
	uint32_t clear_color;

	uint32_t flush;

#define SOURCE_BIAS 4
	uint16_t source_count;
	uint8_t pinned :3;
#define PIN_SCANOUT 0x1
#define PIN_DRI 0x2
#define PIN_PRIME 0x4
	uint8_t create :4;
	uint8_t mapped :1;
	uint8_t shm :1;
	uint8_t clear :1;
	uint8_t header :1;
	uint8_t cpu :1;
};

struct sna_glyph {
	PicturePtr atlas;
	pixman_image_t *image;
	struct sna_coordinate coordinate;
	uint16_t size, pos;
};

static inline PixmapPtr get_window_pixmap(WindowPtr window)
{
	return fbGetWindowPixmap(window);
}

static inline PixmapPtr get_drawable_pixmap(DrawablePtr drawable)
{
	if (drawable->type == DRAWABLE_PIXMAP)
		return (PixmapPtr)drawable;
	else
		return get_window_pixmap((WindowPtr)drawable);
}

extern DevPrivateKeyRec sna_pixmap_key;

constant static inline struct sna_pixmap *sna_pixmap(PixmapPtr pixmap)
{
	return ((void **)dixGetPrivateAddr(&pixmap->devPrivates, &sna_pixmap_key))[1];
}

static inline struct sna_pixmap *sna_pixmap_from_drawable(DrawablePtr drawable)
{
	return sna_pixmap(get_drawable_pixmap(drawable));
}

struct sna_gc {
	long changes;
	long serial;

	GCFuncs *old_funcs;
	void *priv;
};

static inline struct sna_gc *sna_gc(GCPtr gc)
{
	return dixGetPrivateAddr(&gc->devPrivates, &sna_gc_key);
}

enum {
	FLUSH_TIMER = 0,
	THROTTLE_TIMER,
	EXPIRE_TIMER,
#if DEBUG_MEMORY
	DEBUG_MEMORY_TIMER,
#endif
	NUM_TIMERS
};

struct sna {
	ScrnInfoPtr scrn;

	unsigned flags;
#define SNA_NO_WAIT		0x1
#define SNA_NO_FLIP		0x2
#define SNA_TRIPLE_BUFFER	0x4
#define SNA_TEAR_FREE		0x10
#define SNA_FORCE_SHADOW	0x20

	unsigned watch_flush;

	struct timeval timer_tv;
	uint32_t timer_expire[NUM_TIMERS];
	uint16_t timer_active;

	int vblank_interval;

	struct list flush_pixmaps;
	struct list active_pixmaps;

	PixmapPtr front;
	PixmapPtr freed_pixmap;

	struct sna_mode {
		drmModeResPtr kmode;

		int shadow_active;
		DamagePtr shadow_damage;
		struct kgem_bo *shadow;
		int shadow_flip;

		struct list outputs;
		struct list crtcs;
	} mode;

	struct sna_dri {
		void *flip_pending;
	} dri;

	unsigned int tiling;
#define SNA_TILING_FB		0x1
#define SNA_TILING_2D		0x2
#define SNA_TILING_ALL (~0)

	EntityInfoPtr pEnt;
	struct pci_device *PciInfo;
	const struct intel_device_info *info;

	ScreenBlockHandlerProcPtr BlockHandler;
	ScreenWakeupHandlerProcPtr WakeupHandler;
	CloseScreenProcPtr CloseScreen;

	PicturePtr clear;
	struct {
		uint32_t fill_bo;
		uint32_t fill_pixel;
		uint32_t fill_alu;
	} blt_state;
	union {
		struct gen2_render_state gen2;
		struct gen3_render_state gen3;
		struct gen4_render_state gen4;
		struct gen5_render_state gen5;
		struct gen6_render_state gen6;
		struct gen7_render_state gen7;
	} render_state;
	uint32_t have_render;

	bool dri_available;
	bool dri_open;
	char *deviceName;

	/* Broken-out options. */
	OptionInfoPtr Options;

	/* Driver phase/state information */
	bool suspended;

#if HAVE_UDEV
	struct udev_monitor *uevent_monitor;
	InputHandlerProc uevent_handler;
#endif

	struct kgem kgem;
	struct sna_render render;

#if DEBUG_MEMORY
	struct {
	       int shadow_pixels_allocs;
	       int cpu_bo_allocs;
	       size_t shadow_pixels_bytes;
	       size_t cpu_bo_bytes;
	} debug_memory;
#endif
};

bool sna_mode_pre_init(ScrnInfoPtr scrn, struct sna *sna);
void sna_mode_adjust_frame(struct sna *sna, int x, int y);
extern void sna_mode_update(struct sna *sna);
extern void sna_mode_disable_unused(struct sna *sna);
extern void sna_mode_wakeup(struct sna *sna);
extern void sna_mode_redisplay(struct sna *sna);
extern void sna_mode_close(struct sna *sna);
extern void sna_mode_fini(struct sna *sna);

extern int sna_page_flip(struct sna *sna,
			 struct kgem_bo *bo,
			 void *data,
			 int ref_crtc_hw_id);

constant static inline struct sna *
to_sna(ScrnInfoPtr scrn)
{
	return (struct sna *)(scrn->driverPrivate);
}

constant static inline struct sna *
to_sna_from_screen(ScreenPtr screen)
{
	return to_sna(xf86ScreenToScrn(screen));
}

constant static inline struct sna *
to_sna_from_pixmap(PixmapPtr pixmap)
{
	return ((void **)dixGetPrivateAddr(&pixmap->devPrivates, &sna_pixmap_key))[0];
}

constant static inline struct sna *
to_sna_from_drawable(DrawablePtr drawable)
{
	return to_sna_from_screen(drawable->pScreen);
}

static inline struct sna *
to_sna_from_kgem(struct kgem *kgem)
{
	return container_of(kgem, struct sna, kgem);
}

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(x) (sizeof(x) / sizeof(x[0]))
#endif

#ifndef ALIGN
#define ALIGN(i,m)	(((i) + (m) - 1) & ~((m) - 1))
#endif

#ifndef MIN
#define MIN(a,b)	((a) <= (b) ? (a) : (b))
#endif

#ifndef MAX
#define MAX(a,b)	((a) >= (b) ? (a) : (b))
#endif

extern xf86CrtcPtr sna_covering_crtc(ScrnInfoPtr scrn,
				     const BoxRec *box,
				     xf86CrtcPtr desired);

extern bool sna_wait_for_scanline(struct sna *sna, PixmapPtr pixmap,
				  xf86CrtcPtr crtc, const BoxRec *clip);

#if HAVE_DRI2_H
bool sna_dri_open(struct sna *sna, ScreenPtr pScreen);
void sna_dri_page_flip_handler(struct sna *sna, struct drm_event_vblank *event);
void sna_dri_vblank_handler(struct sna *sna, struct drm_event_vblank *event);
void sna_dri_destroy_window(WindowPtr win);
void sna_dri_close(struct sna *sna, ScreenPtr pScreen);
#else
static inline bool sna_dri_open(struct sna *sna, ScreenPtr pScreen) { return false; }
static inline void sna_dri_page_flip_handler(struct sna *sna, struct drm_event_vblank *event) { }
static inline void sna_dri_vblank_handler(struct sna *sna, struct drm_event_vblank *event) { }
static inline void sna_dri_destroy_window(WindowPtr win) { }
static inline void sna_dri_close(struct sna *sna, ScreenPtr pScreen) { }
#endif
void sna_dri_pixmap_update_bo(struct sna *sna, PixmapPtr pixmap);

extern int sna_crtc_to_pipe(xf86CrtcPtr crtc);
extern uint32_t sna_crtc_to_plane(xf86CrtcPtr crtc);
extern uint32_t sna_crtc_id(xf86CrtcPtr crtc);

CARD32 sna_format_for_depth(int depth);
CARD32 sna_render_format_for_depth(int depth);

void sna_debug_flush(struct sna *sna);

static inline void
get_drawable_deltas(DrawablePtr drawable, PixmapPtr pixmap, int16_t *x, int16_t *y)
{
#ifdef COMPOSITE
	if (drawable->type == DRAWABLE_WINDOW) {
		*x = -pixmap->screen_x;
		*y = -pixmap->screen_y;
		return;
	}
#endif
	*x = *y = 0;
}

static inline int
get_drawable_dx(DrawablePtr drawable)
{
#ifdef COMPOSITE
	if (drawable->type == DRAWABLE_WINDOW)
		return -get_drawable_pixmap(drawable)->screen_x;
#endif
	return 0;
}

static inline int
get_drawable_dy(DrawablePtr drawable)
{
#ifdef COMPOSITE
	if (drawable->type == DRAWABLE_WINDOW)
		return -get_drawable_pixmap(drawable)->screen_y;
#endif
	return 0;
}

bool sna_pixmap_attach_to_bo(PixmapPtr pixmap, struct kgem_bo *bo);
static inline bool sna_pixmap_is_scanout(struct sna *sna, PixmapPtr pixmap)
{
	return (pixmap == sna->front &&
		!sna->mode.shadow_active &&
		(sna->flags & SNA_NO_WAIT) == 0);
}

PixmapPtr sna_pixmap_create_upload(ScreenPtr screen,
				   int width, int height, int depth,
				   unsigned flags);
PixmapPtr sna_pixmap_create_unattached(ScreenPtr screen,
				       int width, int height, int depth);
void sna_pixmap_destroy(PixmapPtr pixmap);

#define MOVE_WRITE 0x1
#define MOVE_READ 0x2
#define MOVE_INPLACE_HINT 0x4
#define MOVE_ASYNC_HINT 0x8
#define MOVE_SOURCE_HINT 0x10
#define MOVE_WHOLE_HINT 0x20
#define __MOVE_FORCE 0x40
#define __MOVE_DRI 0x80

struct sna_pixmap *sna_pixmap_move_to_gpu(PixmapPtr pixmap, unsigned flags);
static inline struct sna_pixmap *
sna_pixmap_force_to_gpu(PixmapPtr pixmap, unsigned flags)
{
	/* Unlike move-to-gpu, we ignore wedged and always create the GPU bo */
	DBG(("%s(pixmap=%p, flags=%x)\n", __FUNCTION__, pixmap, flags));
	return sna_pixmap_move_to_gpu(pixmap, flags | __MOVE_FORCE);
}
bool must_check _sna_pixmap_move_to_cpu(PixmapPtr pixmap, unsigned flags);
static inline bool must_check sna_pixmap_move_to_cpu(PixmapPtr pixmap, unsigned flags)
{
	if (flags == MOVE_READ) {
		struct sna_pixmap *priv = sna_pixmap(pixmap);
		if (priv == NULL)
			return true;
	}

	return _sna_pixmap_move_to_cpu(pixmap, flags);
}
bool must_check sna_drawable_move_region_to_cpu(DrawablePtr drawable,
						RegionPtr region,
						unsigned flags);

bool must_check sna_drawable_move_to_cpu(DrawablePtr drawable, unsigned flags);

static inline bool must_check
sna_drawable_move_to_gpu(DrawablePtr drawable, unsigned flags)
{
	return sna_pixmap_move_to_gpu(get_drawable_pixmap(drawable), flags) != NULL;
}

void sna_add_flush_pixmap(struct sna *sna,
			  struct sna_pixmap *priv,
			  struct kgem_bo *bo);

struct kgem_bo *sna_pixmap_change_tiling(PixmapPtr pixmap, uint32_t tiling);

#define PREFER_GPU	0x1
#define FORCE_GPU	0x2
#define RENDER_GPU	0x4
#define IGNORE_CPU	0x8
struct kgem_bo *
sna_drawable_use_bo(DrawablePtr drawable, unsigned flags, const BoxRec *box,
		    struct sna_damage ***damage);

inline static int16_t bound(int16_t a, uint16_t b)
{
	int v = (int)a + (int)b;
	if (v > MAXSHORT)
		return MAXSHORT;
	return v;
}

inline static int16_t clamp(int16_t a, int16_t b)
{
	int v = (int)a + (int)b;
	if (v > MAXSHORT)
		return MAXSHORT;
	if (v < MINSHORT)
		return MINSHORT;
	return v;
}

static inline bool
box_inplace(PixmapPtr pixmap, const BoxRec *box)
{
	struct sna *sna = to_sna_from_pixmap(pixmap);
	return ((int)(box->x2 - box->x1) * (int)(box->y2 - box->y1) * pixmap->drawable.bitsPerPixel >> 12) >= sna->kgem.half_cpu_cache_pages;
}

static inline bool
region_subsumes_drawable(RegionPtr region, DrawablePtr drawable)
{
	const BoxRec *extents;

	if (region->data)
		return false;

	extents = RegionExtents(region);
	return  extents->x1 <= 0 && extents->y1 <= 0 &&
		extents->x2 >= drawable->width &&
		extents->y2 >= drawable->height;
}

static inline bool
region_subsumes_damage(const RegionRec *region, struct sna_damage *damage)
{
	const BoxRec *re, *de;

	DBG(("%s?\n", __FUNCTION__));
	assert(damage);

	re = &region->extents;
	de = &DAMAGE_PTR(damage)->extents;
	DBG(("%s: region (%d, %d), (%d, %d), damage (%d, %d), (%d, %d)\n",
	     __FUNCTION__,
	     re->x1, re->y1, re->x2, re->y2,
	     de->x1, de->y1, de->x2, de->y2));

	if (re->x2 < de->x2 || re->x1 > de->x1 ||
	    re->y2 < de->y2 || re->y1 > de->y1) {
		DBG(("%s: not contained\n", __FUNCTION__));
		return false;
	}

	if (region->data == NULL) {
		DBG(("%s: singular region contains damage\n", __FUNCTION__));
		return true;
	}

	return pixman_region_contains_rectangle((RegionPtr)region,
						(BoxPtr)de) == PIXMAN_REGION_IN;
}


static inline bool
sna_drawable_is_clear(DrawablePtr d)
{
	struct sna_pixmap *priv = sna_pixmap(get_drawable_pixmap(d));
	return priv && priv->clear && priv->clear_color == 0;
}

static inline struct kgem_bo *sna_pixmap_get_bo(PixmapPtr pixmap)
{
	return sna_pixmap(pixmap)->gpu_bo;
}

static inline struct kgem_bo *sna_pixmap_pin(PixmapPtr pixmap, unsigned flags)
{
	struct sna_pixmap *priv;

	priv = sna_pixmap_force_to_gpu(pixmap, MOVE_READ | MOVE_WRITE);
	if (!priv)
		return NULL;

	priv->pinned |= flags;
	return priv->gpu_bo;
}


static inline bool
_sna_transform_point(const PictTransform *transform,
		     int64_t x, int64_t y, int64_t result[3])
{
	int j;

	for (j = 0; j < 3; j++)
		result[j] = (transform->matrix[j][0] * x +
			     transform->matrix[j][1] * y +
			     transform->matrix[j][2]);

	return result[2] != 0;
}

static inline void
_sna_get_transformed_coordinates(int x, int y,
				 const PictTransform *transform,
				 float *x_out, float *y_out)
{

	int64_t result[3];

	_sna_transform_point(transform, x, y, result);
	*x_out = result[0] / (double)result[2];
	*y_out = result[1] / (double)result[2];
}

static inline void
_sna_get_transformed_scaled(int x, int y,
			    const PictTransform *transform, const float *sf,
			    float *x_out, float *y_out)
{
	*x_out = sf[0] * (transform->matrix[0][0] * x +
			  transform->matrix[0][1] * y +
			  transform->matrix[0][2]);

	*y_out = sf[1] * (transform->matrix[1][0] * x +
			  transform->matrix[1][1] * y +
			  transform->matrix[1][2]);
}

void
sna_get_transformed_coordinates(int x, int y,
				const PictTransform *transform,
				float *x_out, float *y_out);

void
sna_get_transformed_coordinates_3d(int x, int y,
				   const PictTransform *transform,
				   float *x_out, float *y_out, float *z_out);

bool sna_transform_is_affine(const PictTransform *t);
bool sna_transform_is_integer_translation(const PictTransform *t,
					  int16_t *tx, int16_t *ty);
bool sna_transform_is_translation(const PictTransform *t,
				  pixman_fixed_t *tx, pixman_fixed_t *ty);
static inline bool
sna_affine_transform_is_rotation(const PictTransform *t)
{
	assert(sna_transform_is_affine(t));
	return t->matrix[0][1] | t->matrix[1][0];
}

static inline bool
sna_transform_equal(const PictTransform *a, const PictTransform *b)
{
	if (a == b)
		return true;

	if (a == NULL || b == NULL)
		return false;

	return memcmp(a, b, sizeof(*a)) == 0;
}

static inline bool
sna_picture_alphamap_equal(PicturePtr a, PicturePtr b)
{
	if (a->alphaMap != b->alphaMap)
		return false;

	if (a->alphaMap)
		return false;

	return (a->alphaOrigin.x == b->alphaOrigin.x &&
		a->alphaOrigin.y == b->alphaOrigin.y);
}

static inline bool wedged(struct sna *sna)
{
	return unlikely(sna->kgem.wedged);
}

static inline bool can_render(struct sna *sna)
{
	return likely(!sna->kgem.wedged && sna->have_render);
}

static inline uint32_t pixmap_size(PixmapPtr pixmap)
{
	return (pixmap->drawable.height - 1) * pixmap->devKind +
		pixmap->drawable.width * pixmap->drawable.bitsPerPixel/8;
}

bool sna_accel_init(ScreenPtr sreen, struct sna *sna);
void sna_accel_create(struct sna *sna);
void sna_accel_block_handler(struct sna *sna, struct timeval **tv);
void sna_accel_wakeup_handler(struct sna *sna);
void sna_accel_watch_flush(struct sna *sna, int enable);
void sna_accel_close(struct sna *sna);
void sna_accel_free(struct sna *sna);

void sna_copy_fbcon(struct sna *sna);

bool sna_composite_create(struct sna *sna);
void sna_composite_close(struct sna *sna);

void sna_composite(CARD8 op,
		   PicturePtr src,
		   PicturePtr mask,
		   PicturePtr dst,
		   INT16 src_x,  INT16 src_y,
		   INT16 mask_x, INT16 mask_y,
		   INT16 dst_x,  INT16 dst_y,
		   CARD16 width, CARD16 height);
void sna_composite_rectangles(CARD8		 op,
			      PicturePtr		 dst,
			      xRenderColor	*color,
			      int			 num_rects,
			      xRectangle		*rects);
void sna_composite_trapezoids(CARD8 op,
			      PicturePtr src,
			      PicturePtr dst,
			      PictFormatPtr maskFormat,
			      INT16 xSrc, INT16 ySrc,
			      int ntrap, xTrapezoid *traps);
void sna_add_traps(PicturePtr picture, INT16 x, INT16 y, int n, xTrap *t);

void sna_composite_triangles(CARD8 op,
			     PicturePtr src,
			     PicturePtr dst,
			     PictFormatPtr maskFormat,
			     INT16 xSrc, INT16 ySrc,
			     int ntri, xTriangle *tri);

void sna_composite_tristrip(CARD8 op,
			    PicturePtr src,
			    PicturePtr dst,
			    PictFormatPtr maskFormat,
			    INT16 xSrc, INT16 ySrc,
			    int npoints, xPointFixed *points);

void sna_composite_trifan(CARD8 op,
			  PicturePtr src,
			  PicturePtr dst,
			  PictFormatPtr maskFormat,
			  INT16 xSrc, INT16 ySrc,
			  int npoints, xPointFixed *points);

bool sna_gradients_create(struct sna *sna);
void sna_gradients_close(struct sna *sna);

bool sna_glyphs_create(struct sna *sna);
void sna_glyphs(CARD8 op,
		PicturePtr src,
		PicturePtr dst,
		PictFormatPtr mask,
		INT16 xSrc, INT16 ySrc,
		int nlist,
		GlyphListPtr list,
		GlyphPtr *glyphs);
void sna_glyphs__shared(CARD8 op,
			PicturePtr src,
			PicturePtr dst,
			PictFormatPtr mask,
			INT16 src_x, INT16 src_y,
			int nlist, GlyphListPtr list, GlyphPtr *glyphs);
void sna_glyph_unrealize(ScreenPtr screen, GlyphPtr glyph);
void sna_glyphs_close(struct sna *sna);

void sna_read_boxes(struct sna *sna,
		    struct kgem_bo *src_bo, int16_t src_dx, int16_t src_dy,
		    PixmapPtr dst, int16_t dst_dx, int16_t dst_dy,
		    const BoxRec *box, int n);
bool sna_write_boxes(struct sna *sna, PixmapPtr dst,
		     struct kgem_bo *dst_bo, int16_t dst_dx, int16_t dst_dy,
		     const void *src, int stride, int16_t src_dx, int16_t src_dy,
		     const BoxRec *box, int n);
void sna_write_boxes__xor(struct sna *sna, PixmapPtr dst,
			  struct kgem_bo *dst_bo, int16_t dst_dx, int16_t dst_dy,
			  const void *src, int stride, int16_t src_dx, int16_t src_dy,
			  const BoxRec *box, int nbox,
			  uint32_t and, uint32_t or);

bool sna_replace(struct sna *sna,
		 PixmapPtr pixmap,
		 struct kgem_bo **bo,
		 const void *src, int stride);
struct kgem_bo *sna_replace__xor(struct sna *sna,
				 PixmapPtr pixmap,
				 struct kgem_bo *bo,
				 const void *src, int stride,
				 uint32_t and, uint32_t or);

bool
sna_compute_composite_extents(BoxPtr extents,
			      PicturePtr src, PicturePtr mask, PicturePtr dst,
			      INT16 src_x,  INT16 src_y,
			      INT16 mask_x, INT16 mask_y,
			      INT16 dst_x,  INT16 dst_y,
			      CARD16 width, CARD16 height);
bool
sna_compute_composite_region(RegionPtr region,
			     PicturePtr src, PicturePtr mask, PicturePtr dst,
			     INT16 src_x,  INT16 src_y,
			     INT16 mask_x, INT16 mask_y,
			     INT16 dst_x,  INT16 dst_y,
			     CARD16 width, CARD16 height);

void
memcpy_blt(const void *src, void *dst, int bpp,
	   int32_t src_stride, int32_t dst_stride,
	   int16_t src_x, int16_t src_y,
	   int16_t dst_x, int16_t dst_y,
	   uint16_t width, uint16_t height);
void
memcpy_to_tiled_x(const void *src, void *dst, int bpp, int swizzling,
		  int32_t src_stride, int32_t dst_stride,
		  int16_t src_x, int16_t src_y,
		  int16_t dst_x, int16_t dst_y,
		  uint16_t width, uint16_t height);
void
memmove_box(const void *src, void *dst,
	    int bpp, int32_t stride,
	    const BoxRec *box,
	    int dx, int dy);

void
memcpy_xor(const void *src, void *dst, int bpp,
	   int32_t src_stride, int32_t dst_stride,
	   int16_t src_x, int16_t src_y,
	   int16_t dst_x, int16_t dst_y,
	   uint16_t width, uint16_t height,
	   uint32_t and, uint32_t or);

#define SNA_CREATE_FB 0x10
#define SNA_CREATE_SCRATCH 0x11
#define SNA_CREATE_GLYPHS 0x12

inline static bool is_power_of_two(unsigned x)
{
	return (x & (x-1)) == 0;
}

inline static bool is_clipped(const RegionRec *r,
			      const DrawableRec *d)
{
	return (r->data ||
		r->extents.x2 - r->extents.x1 != d->width ||
		r->extents.y2 - r->extents.y1 != d->height);
}

#endif /* _SNA_H */
