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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#if 0
#define I830DEBUG
#endif

#include <stdint.h>

#ifndef REMAP_RESERVED
#define REMAP_RESERVED 0
#endif

#ifndef _I830_H_
#define _I830_H_

#include "xf86_OSproc.h"
#include "compiler.h"
#include "xf86Pci.h"
#include "xf86Cursor.h"
#include "xf86xv.h"
#include "xf86Crtc.h"
#include "xf86RandR12.h"

#include "xorg-server.h"
#include <pciaccess.h>

#define _XF86DRI_SERVER_
#include "dri2.h"
#include "intel_bufmgr.h"
#include "i915_drm.h"

#include "intel_driver.h"
#include "intel_options.h"
#include "intel_list.h"
#include "compat-api.h"

#if HAVE_UDEV
#include <libudev.h>
#endif

/* remain compatible to xorg-server 1.6 */
#ifndef MONITOR_EDID_COMPLETE_RAWDATA
#define MONITOR_EDID_COMPLETE_RAWDATA EDID_COMPLETE_RAWDATA
#endif

#if XF86_CRTC_VERSION >= 5
#define INTEL_PIXMAP_SHARING 1
#endif

struct intel_pixmap {
	dri_bo *bo;

	struct list batch;

	uint16_t stride;
	uint8_t tiling;
	int8_t busy :2;
	uint8_t dirty :1;
	uint8_t offscreen :1;
	uint8_t pinned :3;
#define PIN_SCANOUT 0x1
#define PIN_DRI 0x2
#define PIN_GLAMOR 0x4
};

#if HAS_DEVPRIVATEKEYREC
extern DevPrivateKeyRec uxa_pixmap_index;
#else
extern int uxa_pixmap_index;
#endif

static inline struct intel_pixmap *intel_get_pixmap_private(PixmapPtr pixmap)
{
#if HAS_DEVPRIVATEKEYREC
	return dixGetPrivate(&pixmap->devPrivates, &uxa_pixmap_index);
#else
	return dixLookupPrivate(&pixmap->devPrivates, &uxa_pixmap_index);
#endif
}

static inline Bool intel_pixmap_is_busy(struct intel_pixmap *priv)
{
	if (priv->busy == -1)
		priv->busy = drm_intel_bo_busy(priv->bo);
	return priv->busy;
}

static inline void intel_set_pixmap_private(PixmapPtr pixmap, struct intel_pixmap *intel)
{
	dixSetPrivate(&pixmap->devPrivates, &uxa_pixmap_index, intel);
}

static inline Bool intel_pixmap_is_dirty(PixmapPtr pixmap)
{
	return pixmap && intel_get_pixmap_private(pixmap)->dirty;
}

static inline Bool intel_pixmap_tiled(PixmapPtr pixmap)
{
	return intel_get_pixmap_private(pixmap)->tiling != I915_TILING_NONE;
}

dri_bo *intel_get_pixmap_bo(PixmapPtr pixmap);
void intel_set_pixmap_bo(PixmapPtr pixmap, dri_bo * bo);

#include "common.h"

#define PITCH_NONE 0

/** enumeration of 3d consumers so some can maintain invariant state. */
enum last_3d {
	LAST_3D_OTHER,
	LAST_3D_VIDEO,
	LAST_3D_RENDER,
	LAST_3D_ROTATION
};

enum dri_type {
	DRI_DISABLED,
	DRI_NONE,
	DRI_DRI2
};

typedef struct intel_screen_private {
	ScrnInfoPtr scrn;
	int cpp;

#define RENDER_BATCH			I915_EXEC_RENDER
#define BLT_BATCH			I915_EXEC_BLT
	unsigned int current_batch;

	void *modes;
	drm_intel_bo *front_buffer, *back_buffer;
	PixmapPtr back_pixmap;
	unsigned int back_name;
	long front_pitch, front_tiling;

	dri_bufmgr *bufmgr;

	uint32_t batch_ptr[4096];
	/** Byte offset in batch_ptr for the next dword to be emitted. */
	unsigned int batch_used;
	/** Position in batch_ptr at the start of the current BEGIN_BATCH */
	unsigned int batch_emit_start;
	/** Number of bytes to be emitted in the current BEGIN_BATCH. */
	uint32_t batch_emitting;
	dri_bo *batch_bo, *last_batch_bo[2];
	/** Whether we're in a section of code that can't tolerate flushing */
	Bool in_batch_atomic;
	/** Ending batch_used that was verified by intel_start_batch_atomic() */
	int batch_atomic_limit;
	struct list batch_pixmaps;
	drm_intel_bo *wa_scratch_bo;
	OsTimerPtr cache_expire;

	/* For Xvideo */
	Bool use_overlay;
#ifdef INTEL_XVMC
	/* For XvMC */
	Bool XvMCEnabled;
#endif

	CreateScreenResourcesProcPtr CreateScreenResources;

	Bool shadow_present;

	unsigned int tiling;
#define INTEL_TILING_FB		0x1
#define INTEL_TILING_2D		0x2
#define INTEL_TILING_3D		0x4
#define INTEL_TILING_ALL (~0)

	Bool swapbuffers_wait;
	Bool has_relaxed_fencing;

	int Chipset;
	EntityInfoPtr pEnt;
	struct pci_device *PciInfo;
	const struct intel_device_info *info;

	unsigned int BR[20];

	CloseScreenProcPtr CloseScreen;

	void (*context_switch) (struct intel_screen_private *intel,
				int new_mode);
	void (*vertex_flush) (struct intel_screen_private *intel);
	void (*batch_flush) (struct intel_screen_private *intel);
	void (*batch_commit_notify) (struct intel_screen_private *intel);

	struct _UxaDriver *uxa_driver;
	int uxa_flags;
	Bool need_sync;
	int accel_pixmap_offset_alignment;
	int accel_max_x;
	int accel_max_y;
	int max_bo_size;
	int max_gtt_map_size;
	int max_tiling_size;

	Bool XvDisabled;	/* Xv disabled in PreInit. */
	Bool XvEnabled;		/* Xv enabled for this generation. */
	Bool XvPreferOverlay;

	int colorKey;
	XF86VideoAdaptorPtr adaptor;
	ScreenBlockHandlerProcPtr BlockHandler;
	Bool overlayOn;

	struct {
		drm_intel_bo *gen4_vs_bo;
		drm_intel_bo *gen4_sf_bo;
		drm_intel_bo *gen4_wm_packed_bo;
		drm_intel_bo *gen4_wm_planar_bo;
		drm_intel_bo *gen4_cc_bo;
		drm_intel_bo *gen4_cc_vp_bo;
		drm_intel_bo *gen4_sampler_bo;
		drm_intel_bo *gen4_sip_kernel_bo;
		drm_intel_bo *wm_prog_packed_bo;
		drm_intel_bo *wm_prog_planar_bo;
		drm_intel_bo *gen6_blend_bo;
		drm_intel_bo *gen6_depth_stencil_bo;
	} video;

	/* Render accel state */
	float scale_units[2][2];
	/** Transform pointers for src/mask, or NULL if identity */
	PictTransform *transform[2];

	PixmapPtr render_source, render_mask, render_dest;
	PicturePtr render_source_picture, render_mask_picture, render_dest_picture;
	Bool needs_3d_invariant;
	Bool needs_render_state_emit;
	Bool needs_render_vertex_emit;

	/* i830 render accel state */
	uint32_t render_dest_format;
	uint32_t cblend, ablend, s8_blendctl;

	/* i915 render accel state */
	PixmapPtr texture[2];
	uint32_t mapstate[6];
	uint32_t samplerstate[6];

	struct {
		int op;
		uint32_t dst_format;
	} i915_render_state;

	struct {
		int num_sf_outputs;
		int drawrect;
		uint32_t blend;
		dri_bo *samplers;
		dri_bo *kernel;
	} gen6_render_state;

	uint32_t prim_offset;
	void (*prim_emit)(struct intel_screen_private *intel,
			  int srcX, int srcY,
			  int maskX, int maskY,
			  int dstX, int dstY,
			  int w, int h);
	int floats_per_vertex;
	int last_floats_per_vertex;
	uint16_t vertex_offset;
	uint16_t vertex_count;
	uint16_t vertex_index;
	uint16_t vertex_used;
	uint32_t vertex_id;
	float vertex_ptr[4*1024];
	dri_bo *vertex_bo;

	uint8_t surface_data[16*1024];
	uint16_t surface_used;
	uint16_t surface_table;
	uint32_t surface_reloc;
	dri_bo *surface_bo;

	/* 965 render acceleration state */
	struct gen4_render_state *gen4_render_state;

	enum dri_type directRenderingType;	/* DRI enabled this generation. */

	Bool directRenderingOpen;
	int drmSubFD;
	char *deviceName;

	Bool use_pageflipping;
	Bool use_triple_buffer;
	Bool force_fallback;
	Bool has_kernel_flush;
	Bool needs_flush;

	struct _DRI2FrameEvent *pending_flip[2];

	/* Broken-out options. */
	OptionInfoPtr Options;

	/* Driver phase/state information */
	Bool suspended;

	enum last_3d last_3d;

	/**
	 * User option to print acceleration fallback info to the server log.
	 */
	Bool fallback_debug;
	unsigned debug_flush;
#if HAVE_UDEV
	struct udev_monitor *uevent_monitor;
	InputHandlerProc uevent_handler;
#endif
	Bool has_prime_vmap_flush;
} intel_screen_private;

#ifndef I915_PARAM_HAS_PRIME_VMAP_FLUSH
#define I915_PARAM_HAS_PRIME_VMAP_FLUSH 21
#endif

enum {
	DEBUG_FLUSH_BATCHES = 0x1,
	DEBUG_FLUSH_CACHES = 0x2,
	DEBUG_FLUSH_WAIT = 0x4,
};

extern Bool intel_mode_pre_init(ScrnInfoPtr pScrn, int fd, int cpp);
extern void intel_mode_init(struct intel_screen_private *intel);
extern void intel_mode_disable_unused_functions(ScrnInfoPtr scrn);
extern void intel_mode_remove_fb(intel_screen_private *intel);
extern void intel_mode_close(intel_screen_private *intel);
extern void intel_mode_fini(intel_screen_private *intel);

extern int intel_get_pipe_from_crtc_id(drm_intel_bufmgr *bufmgr, xf86CrtcPtr crtc);
extern int intel_crtc_id(xf86CrtcPtr crtc);
extern int intel_output_dpms_status(xf86OutputPtr output);
extern void intel_copy_fb(ScrnInfoPtr scrn);

enum DRI2FrameEventType {
	DRI2_SWAP,
	DRI2_SWAP_CHAIN,
	DRI2_FLIP,
	DRI2_WAITMSC,
};

#if XORG_VERSION_CURRENT <= XORG_VERSION_NUMERIC(1,7,99,3,0)
typedef void (*DRI2SwapEventPtr)(ClientPtr client, void *data, int type,
				 CARD64 ust, CARD64 msc, CARD64 sbc);
#endif

typedef struct _DRI2FrameEvent {
	struct intel_screen_private *intel;

	XID drawable_id;
	ClientPtr client;
	enum DRI2FrameEventType type;
	int frame;
	int pipe;

	struct list drawable_resource, client_resource;

	/* for swaps & flips only */
	DRI2SwapEventPtr event_complete;
	void *event_data;
	DRI2BufferPtr front;
	DRI2BufferPtr back;

	struct _DRI2FrameEvent *chain;
} DRI2FrameEventRec, *DRI2FrameEventPtr;

extern Bool intel_do_pageflip(intel_screen_private *intel,
			      dri_bo *new_front,
			      DRI2FrameEventPtr flip_info, int ref_crtc_hw_id);

static inline intel_screen_private *
intel_get_screen_private(ScrnInfoPtr scrn)
{
	return (intel_screen_private *)(scrn->driverPrivate);
}

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(x) (sizeof(x) / sizeof(x[0]))
#endif

#ifndef ALIGN
#define ALIGN(i,m)	(((i) + (m) - 1) & ~((m) - 1))
#endif

#ifndef MIN
#define MIN(a,b)	((a) < (b) ? (a) : (b))
#endif

static inline unsigned long intel_pixmap_pitch(PixmapPtr pixmap)
{
	return (unsigned long)pixmap->devKind;
}

/* Batchbuffer support macros and functions */
#include "intel_batchbuffer.h"

/* I830 specific functions */
extern void IntelEmitInvarientState(ScrnInfoPtr scrn);
extern void I830EmitInvarientState(ScrnInfoPtr scrn);
extern void I915EmitInvarientState(ScrnInfoPtr scrn);

extern void I830EmitFlush(ScrnInfoPtr scrn);

extern void I830InitVideo(ScreenPtr pScreen);
extern xf86CrtcPtr intel_covering_crtc(ScrnInfoPtr scrn, BoxPtr box,
				      xf86CrtcPtr desired, BoxPtr crtc_box_ret);

Bool I830DRI2ScreenInit(ScreenPtr pScreen);
void I830DRI2CloseScreen(ScreenPtr pScreen);
void I830DRI2FrameEventHandler(unsigned int frame, unsigned int tv_sec,
			       unsigned int tv_usec, DRI2FrameEventPtr flip_info);
void I830DRI2FlipEventHandler(unsigned int frame, unsigned int tv_sec,
			      unsigned int tv_usec, DRI2FrameEventPtr flip_info);

extern Bool intel_crtc_on(xf86CrtcPtr crtc);
int intel_crtc_to_pipe(xf86CrtcPtr crtc);

/* intel_memory.c */
unsigned long intel_get_fence_size(intel_screen_private *intel, unsigned long size);
unsigned long intel_get_fence_pitch(intel_screen_private *intel, unsigned long pitch,
				   uint32_t tiling_mode);

drm_intel_bo *intel_allocate_framebuffer(ScrnInfoPtr scrn,
					int w, int h, int cpp,
					unsigned long *pitch,
					uint32_t *tiling);

/* i830_render.c */
Bool i830_check_composite(int op,
			  PicturePtr sourcec, PicturePtr mask, PicturePtr dest,
			  int width, int height);
Bool i830_check_composite_target(PixmapPtr pixmap);
Bool i830_check_composite_texture(ScreenPtr screen, PicturePtr picture);
Bool i830_prepare_composite(int op, PicturePtr sourcec, PicturePtr mask,
			    PicturePtr dest, PixmapPtr sourcecPixmap,
			    PixmapPtr maskPixmap, PixmapPtr destPixmap);
void i830_composite(PixmapPtr dest, int srcX, int srcY,
		    int maskX, int maskY, int dstX, int dstY, int w, int h);
void i830_vertex_flush(intel_screen_private *intel);

/* i915_render.c */
Bool i915_check_composite(int op,
			  PicturePtr sourcec, PicturePtr mask, PicturePtr dest,
			  int width, int height);
Bool i915_check_composite_target(PixmapPtr pixmap);
Bool i915_check_composite_texture(ScreenPtr screen, PicturePtr picture);
Bool i915_prepare_composite(int op, PicturePtr sourcec, PicturePtr mask,
			    PicturePtr dest, PixmapPtr sourcecPixmap,
			    PixmapPtr maskPixmap, PixmapPtr destPixmap);
void i915_composite(PixmapPtr dest, int srcX, int srcY,
		    int maskX, int maskY, int dstX, int dstY, int w, int h);
void i915_vertex_flush(intel_screen_private *intel);
void i915_batch_commit_notify(intel_screen_private *intel);
void i830_batch_commit_notify(intel_screen_private *intel);
/* i965_render.c */
unsigned int gen4_render_state_size(ScrnInfoPtr scrn);
void gen4_render_state_init(ScrnInfoPtr scrn);
void gen4_render_state_cleanup(ScrnInfoPtr scrn);
Bool i965_check_composite(int op,
			  PicturePtr sourcec, PicturePtr mask, PicturePtr dest,
			  int width, int height);
Bool i965_check_composite_texture(ScreenPtr screen, PicturePtr picture);
Bool i965_prepare_composite(int op, PicturePtr sourcec, PicturePtr mask,
			    PicturePtr dest, PixmapPtr sourcecPixmap,
			    PixmapPtr maskPixmap, PixmapPtr destPixmap);
void i965_composite(PixmapPtr dest, int srcX, int srcY,
		    int maskX, int maskY, int dstX, int dstY, int w, int h);

void i965_vertex_flush(intel_screen_private *intel);
void i965_batch_flush(intel_screen_private *intel);
void i965_batch_commit_notify(intel_screen_private *intel);

/* i965_3d.c */
void gen6_upload_invariant_states(intel_screen_private *intel);
void gen6_upload_viewport_state_pointers(intel_screen_private *intel,
					 drm_intel_bo *cc_vp_bo);
void gen7_upload_viewport_state_pointers(intel_screen_private *intel,
					 drm_intel_bo *cc_vp_bo);
void gen6_upload_urb(intel_screen_private *intel);
void gen7_upload_urb(intel_screen_private *intel);
void gen6_upload_cc_state_pointers(intel_screen_private *intel,
				   drm_intel_bo *blend_bo, drm_intel_bo *cc_bo,
				   drm_intel_bo *depth_stencil_bo,
				   uint32_t blend_offset);
void gen7_upload_cc_state_pointers(intel_screen_private *intel,
				   drm_intel_bo *blend_bo, drm_intel_bo *cc_bo,
				   drm_intel_bo *depth_stencil_bo,
				   uint32_t blend_offset);
void gen6_upload_sampler_state_pointers(intel_screen_private *intel,
					drm_intel_bo *sampler_bo);
void gen7_upload_sampler_state_pointers(intel_screen_private *intel,
					drm_intel_bo *sampler_bo);
void gen7_upload_bypass_states(intel_screen_private *intel);
void gen6_upload_gs_state(intel_screen_private *intel);
void gen6_upload_vs_state(intel_screen_private *intel);
void gen6_upload_clip_state(intel_screen_private *intel);
void gen6_upload_sf_state(intel_screen_private *intel, int num_sf_outputs, int read_offset);
void gen7_upload_sf_state(intel_screen_private *intel, int num_sf_outputs, int read_offset);
void gen6_upload_binding_table(intel_screen_private *intel, uint32_t ps_binding_table_offset);
void gen7_upload_binding_table(intel_screen_private *intel, uint32_t ps_binding_table_offset);
void gen6_upload_depth_buffer_state(intel_screen_private *intel);
void gen7_upload_depth_buffer_state(intel_screen_private *intel);

Bool intel_transform_is_affine(PictTransformPtr t);
Bool
intel_get_transformed_coordinates(int x, int y, PictTransformPtr transform,
				 float *x_out, float *y_out);

Bool
intel_get_transformed_coordinates_3d(int x, int y, PictTransformPtr transform,
				    float *x_out, float *y_out, float *z_out);

static inline void
intel_debug_fallback(ScrnInfoPtr scrn, const char *format, ...) _X_ATTRIBUTE_PRINTF(2, 3);

static inline void
intel_debug_fallback(ScrnInfoPtr scrn, const char *format, ...)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);
	va_list ap;

	va_start(ap, format);
	if (intel->fallback_debug) {
		xf86DrvMsg(scrn->scrnIndex, X_INFO, "fallback: ");
		LogVMessageVerb(X_INFO, 1, format, ap);
	}
	va_end(ap);
}

static inline Bool
intel_check_pitch_2d(PixmapPtr pixmap)
{
	uint32_t pitch = intel_pixmap_pitch(pixmap);
	if (pitch > KB(32)) {
		ScrnInfoPtr scrn = xf86ScreenToScrn(pixmap->drawable.pScreen);
		intel_debug_fallback(scrn, "pitch exceeds 2d limit 32K\n");
		return FALSE;
	}
	return TRUE;
}

/* For pre-965 chip only, as they have 8KB limit for 3D */
static inline Bool
intel_check_pitch_3d(PixmapPtr pixmap)
{
	uint32_t pitch = intel_pixmap_pitch(pixmap);
	if (pitch > KB(8)) {
		ScrnInfoPtr scrn = xf86ScreenToScrn(pixmap->drawable.pScreen);
		intel_debug_fallback(scrn, "pitch exceeds 3d limit 8K\n");
		return FALSE;
	}
	return TRUE;
}

/**
 * Little wrapper around drm_intel_bo_reloc to return the initial value you
 * should stuff into the relocation entry.
 *
 * If only we'd done this before settling on the library API.
 */
static inline uint32_t
intel_emit_reloc(drm_intel_bo * bo, uint32_t offset,
		 drm_intel_bo * target_bo, uint32_t target_offset,
		 uint32_t read_domains, uint32_t write_domain)
{
	drm_intel_bo_emit_reloc(bo, offset, target_bo, target_offset,
				read_domains, write_domain);

	return target_bo->offset + target_offset;
}

static inline drm_intel_bo *intel_bo_alloc_for_data(intel_screen_private *intel,
						    const void *data,
						    unsigned int size,
						    const char *name)
{
	drm_intel_bo *bo;
	int ret;

	bo = drm_intel_bo_alloc(intel->bufmgr, name, size, 4096);
	assert(bo);

	ret = drm_intel_bo_subdata(bo, 0, size, data);
	assert(ret == 0);

	return bo;
	(void)ret;
}

void intel_debug_flush(ScrnInfoPtr scrn);

static inline PixmapPtr get_drawable_pixmap(DrawablePtr drawable)
{
	ScreenPtr screen = drawable->pScreen;

	if (drawable->type == DRAWABLE_PIXMAP)
		return (PixmapPtr) drawable;
	else
		return screen->GetWindowPixmap((WindowPtr) drawable);
}

static inline Bool pixmap_is_scanout(PixmapPtr pixmap)
{
	ScreenPtr screen = pixmap->drawable.pScreen;

	return pixmap == screen->GetScreenPixmap(screen);
}

Bool intel_uxa_init(ScreenPtr pScreen);
Bool intel_uxa_create_screen_resources(ScreenPtr pScreen);
void intel_uxa_block_handler(intel_screen_private *intel);
Bool intel_get_aperture_space(ScrnInfoPtr scrn, drm_intel_bo ** bo_table,
			      int num_bos);

static inline Bool intel_pixmap_is_offscreen(PixmapPtr pixmap)
{
	struct intel_pixmap *priv = intel_get_pixmap_private(pixmap);
	return priv && priv->offscreen;
}

#endif /* _I830_H_ */
