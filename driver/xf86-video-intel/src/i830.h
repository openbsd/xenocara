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
/* $XFree86: xc/programs/Xserver/hw/xfree86/drivers/i810/i830.h,v 1.12 2004/01/07 03:43:19 dawes Exp $ */

/*
 * Authors:
 *   Keith Whitwell <keith@tungstengraphics.com>
 *   David Dawes <dawes@xfree86.org>
 *
 */

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
#include "xf86PciInfo.h"
#include "xf86Pci.h"
#include "i810_reg.h"
#include "xaa.h"
#include "xf86Cursor.h"
#include "xf86xv.h"
#include "vgaHW.h"
#include "xf86Crtc.h"
#include "xf86RandR12.h"

#include "xorg-server.h"
#ifdef XSERVER_LIBPCIACCESS
#include <pciaccess.h>
#endif

#ifdef XF86DRI
#include "xf86drm.h"
#include "sarea.h"
#define _XF86DRI_SERVER_
#include "dri.h"
#include "GL/glxint.h"
#include "i830_dri.h"
#endif
#include "intel_bufmgr.h"
#include "i915_drm.h"

#ifdef I830_USE_EXA
#include "exa.h"
Bool I830EXAInit(ScreenPtr pScreen);
unsigned long long I830TexOffsetStart(PixmapPtr pPix);
#endif

#ifdef I830_USE_UXA
#include "uxa.h"
Bool i830_uxa_init(ScreenPtr pScreen);
void i830_uxa_create_screen_resources(ScreenPtr pScreen);
void i830_uxa_block_handler (ScreenPtr pScreen);
Bool i830_get_aperture_space(ScrnInfoPtr pScrn, drm_intel_bo **bo_table,
			     int num_bos);
#endif

#if defined(I830_USE_UXA) || defined(I830_USE_EXA)
dri_bo *i830_get_pixmap_bo (PixmapPtr pixmap);
void i830_set_pixmap_bo(PixmapPtr pixmap, dri_bo *bo);
#endif

#ifdef I830_USE_XAA
Bool I830XAAInit(ScreenPtr pScreen);
#endif

typedef struct _I830OutputRec I830OutputRec, *I830OutputPtr;

#include "common.h"
#include "i830_sdvo.h"
#include "i2c_vid.h"

/*
 * The mode handling is based upon the VESA driver written by
 * Paulo César Pereira de Andrade <pcpa@conectiva.com.br>.
 */

#ifdef XF86DRI
#define I830_MM_MINPAGES 512
#define I830_MM_MAXSIZE  (32*1024)
#define I830_KERNEL_MM  (1 << 0) /* Initialize the kernel memory manager*/
#define I830_KERNEL_TEX (1 << 1) /* Allocate texture memory pool */
#endif

#ifdef XvMCExtension
#ifdef ENABLE_XVMC
#define INTEL_XVMC 1
#endif
#endif

typedef struct _I830Rec *I830Ptr;

typedef void (*I830WriteIndexedByteFunc)(I830Ptr pI830, IOADDRESS addr,
                                         uint8_t index, uint8_t value);
typedef uint8_t(*I830ReadIndexedByteFunc)(I830Ptr pI830, IOADDRESS addr,
					  uint8_t index);
typedef void (*I830WriteByteFunc)(I830Ptr pI830, IOADDRESS addr,
				  uint8_t value);
typedef uint8_t(*I830ReadByteFunc)(I830Ptr pI830, IOADDRESS addr);

enum tile_format {
    TILE_NONE,
    TILE_XMAJOR,
    TILE_YMAJOR
};

#define PITCH_NONE 0

/** Record of a linear allocation in the aperture. */
typedef struct _i830_memory i830_memory;
struct _i830_memory {
    /** Offset of the allocation in card VM */
    unsigned long offset;
    /** End of the allocation in card VM */
    unsigned long end;
    /**
     * Requested size of the allocation: doesn't count padding.
     *
     * Any bound memory will cover offset to (offset + size).
     */
    unsigned long size;
    /**
     * Allocated aperture size, taking into account padding to allow for
     * tiling.
     */
    unsigned long allocated_size;
    /**
     * Physical (or more properly, bus) address of the allocation.
     * Only set if requested during allocation.
     */
    uint64_t bus_addr;
    /** AGP memory handle */
    int key;
    /**
     * Whether or not the AGP memory (if any) is currently bound.
     */
    Bool bound;
    /**
     * Offset that the AGP-allocated memory (if any) is to be bound to.
     *
     * This is either @offset or pI830->stolen_size
     */
    unsigned long agp_offset;

    enum tile_format tiling;
    /**
     * Index of the fence register representing the tiled surface, when
     * bound.
     */
    int fence_nr;
    /** Pitch value in bytes for tiled surfaces */
    unsigned int pitch;

    /** Description of the allocation, for logging */
    char *name;

    /** @{
     * Memory allocator linked list pointers
     */
    i830_memory *next;
    i830_memory *prev;
    /** @} */

    dri_bo *bo;
    uint32_t alignment;
    uint32_t gem_name;
    Bool lifetime_fixed_offset;
};

typedef struct {
   int tail_mask;
   i830_memory *mem;
   unsigned char *virtual_start;
   int head;
   int tail;
   int space;
} I830RingBuffer;

/* store information about an Ixxx DVO */
/* The i830->i865 use multiple DVOs with multiple i2cs */
/* the i915, i945 have a single sDVO i2c bus - which is different */
#define MAX_OUTPUTS 6

#define I830_I2C_BUS_DVO 1
#define I830_I2C_BUS_SDVO 2

/* these are outputs from the chip - integrated only 
   external chips are via DVO or SDVO output */
#define I830_OUTPUT_UNUSED 0
#define I830_OUTPUT_ANALOG 1
#define I830_OUTPUT_DVO_TMDS 2
#define I830_OUTPUT_DVO_LVDS 3
#define I830_OUTPUT_DVO_TVOUT 4
#define I830_OUTPUT_SDVO 5
#define I830_OUTPUT_LVDS 6
#define I830_OUTPUT_TVOUT 7
#define I830_OUTPUT_HDMI 8

struct _I830DVODriver {
   int type;
   char *modulename;
   char *fntablename;
   unsigned int dvo_reg;
   uint32_t gpio;
   int address;
   const char **symbols;
   I830I2CVidOutputRec *vid_rec;
   void *dev_priv;
   pointer modhandle;
};

extern const char *i830_output_type_names[];

typedef struct _I830CrtcPrivateRec {
    int			    pipe;
    int			    plane;

    Bool    		    enabled;
    
    int			    dpms_mode;
    
    int			    x, y;

    /* Lookup table values to be set when the CRTC is enabled */
    uint8_t lut_r[256], lut_g[256], lut_b[256];

    i830_memory *rotate_mem;
    /* Card virtual address of the cursor */
    unsigned long cursor_offset;
    unsigned long cursor_argb_offset;
    /* Physical or virtual addresses of the cursor for setting in the cursor
     * registers.
     */
    uint64_t cursor_addr;
    unsigned long cursor_argb_addr;
    Bool	cursor_is_argb;
} I830CrtcPrivateRec, *I830CrtcPrivatePtr;

#define I830CrtcPrivate(c) ((I830CrtcPrivatePtr) (c)->driver_private)

typedef struct _I830OutputPrivateRec {
   int			    type;
   I2CBusPtr		    pI2CBus;
   I2CBusPtr		    pDDCBus;
   struct _I830DVODriver    *i2c_drv;
   Bool			    load_detect_temp;
   Bool			    needs_tv_clock;
   uint32_t		    lvds_bits;
   int                      pipe_mask;
   int			    clone_mask;
   /** Output-private structure.  Should replace i2c_drv */
   void			    *dev_priv;
} I830OutputPrivateRec, *I830OutputPrivatePtr;

#define I830OutputPrivate(o) ((I830OutputPrivatePtr) (o)->driver_private)

/** enumeration of 3d consumers so some can maintain invariant state. */
enum last_3d {
    LAST_3D_OTHER,
    LAST_3D_VIDEO,
    LAST_3D_RENDER,
    LAST_3D_ROTATION
};

/*
 * Backlight control has some unfortunate properties:
 *   - many machines won't give us brightness change notifications
 *     o brightness hotkeys
 *     o events like AC plug/unplug (can be controlled via _DOS setting)
 *     o ambient light sensor triggered changes
 *   - some machines use the so-called "legacy" backlight interface
 *     o resulting brightness is a combo of LBB and PWM values
 *     o LBB sits in config space
 *   - some machines have ACPI methods for changing brightness
 *     o one of the few ways the X server and firmware can stay in sync
 *   - new machines have the IGD OpRegion interface available
 *     o a reliable way of keeping the firmware and X in sync
 *
 * So the real problem is on machines where ACPI or OpRegion methods aren't
 * available.  In that case, problems can occur:
 *   1) the BIOS and X will have different ideas of what the brightness is,
 *      leading to unexpected results when the brightness is increased or
 *      decreased via hotkey or X protocol
 *   2) unless X takes the legacy register into account, machines using it
 *      may prevent X from raising the brightness above 0 if the firmware
 *      set LBB to 0
 * Given these problems, we provide the user with a selection of methods,
 * so they can choose an ideal one for their platform (assuming our quirk
 * code picks the wrong one).
 *
 * Four different backlight control methods are available:
 *   BCM_NATIVE:  only ever touch the native backlight control registers
 *     This method may be susceptible to problem (2) above if the firmware
 *     modifies the legacy registers.
 *   BCM_LEGACY:  only ever touch the legacy backlight control registers
 *     This method may be susceptible to problem (1) above if the firmware
 *     also modifies the legacy registers.
 *   BCM_COMBO:  try to use both sets
 *     In this case, the driver will try to modify both sets of registers
 *     if needed.  To avoid problem (2) above it may set the LBB register
 *     to a non-zero value if the brightness is to be increased.  It's still
 *     susceptible to problem (1), but to a lesser extent than the LEGACY only
 *     method.
 *   BCM_KERNEL:  use kernel methods for controlling the backlight
 *     This is only available on some platforms, but where present this can
 *     provide the best user experience.
 */

enum backlight_control {
    BCM_NATIVE = 0,
    BCM_LEGACY,
    BCM_COMBO,
    BCM_KERNEL,
};

typedef enum accel_method {
    ACCEL_UNINIT = 0,
    ACCEL_NONE,
    ACCEL_XAA,
    ACCEL_EXA,
    ACCEL_UXA
} accel_method_t;

enum dri_type {
    DRI_DISABLED,
    DRI_NONE,
    DRI_XF86DRI,
    DRI_DRI2
};

typedef struct _I830Rec {
   unsigned char *MMIOBase;
   unsigned char *GTTBase;
   unsigned char *FbBase;
   int cpp;

   unsigned int bufferOffset;		/* for I830SelectBuffer */
   BoxRec FbMemBox;
   int CacheLines;

   /* These are set in PreInit and never changed. */
   long FbMapSize;
   long GTTMapSize;

   /**
    * Linked list of video memory allocations.  The head and tail are
    * dummy entries that bound the allocation area.
    */
   i830_memory *memory_list;
   /** Linked list of buffer object memory allocations */
   i830_memory *bo_list;
   long stolen_size;		/**< bytes of pre-bound stolen memory */
   int gtt_acquired;		/**< whether we currently own the AGP */

   i830_memory *front_buffer;
   i830_memory *compressed_front_buffer;
   i830_memory *compressed_ll_buffer;
   /* One big buffer for all cursors for kernels that support this */
   i830_memory *cursor_mem;
   /* separate small buffers for kernels that support this */
   i830_memory *cursor_mem_classic[2];
   i830_memory *cursor_mem_argb[2];
   i830_memory *xaa_scratch;
#ifdef I830_USE_EXA
   i830_memory *exa_offscreen;
#endif
   i830_memory *fake_bufmgr_mem;

   /* Regions allocated either from the above pools, or from agpgart. */
   I830RingBuffer ring;

   /** Number of bytes being emitted in the current BEGIN_LP_RING */
   unsigned int ring_emitting;
   /** Number of bytes that have been emitted in the current BEGIN_LP_RING */
   unsigned int ring_used;
   /** Offset in the ring for the next DWORD emit */
   uint32_t ring_next;

   dri_bufmgr *bufmgr;

   uint8_t *batch_ptr;
   /** Byte offset in batch_ptr for the next dword to be emitted. */
   unsigned int batch_used;
   /** Position in batch_ptr at the start of the current BEGIN_BATCH */
   unsigned int batch_emit_start;
   /** Number of bytes to be emitted in the current BEGIN_BATCH. */
   uint32_t batch_emitting;
   dri_bo *batch_bo;
   /** Whether we're in a section of code that can't tolerate flushing */
   Bool in_batch_atomic;
   /** Ending batch_used that was verified by i830_start_batch_atomic() */
   int batch_atomic_limit;

#ifdef I830_XV
   /* For Xvideo */
   i830_memory *overlay_regs;
#endif
#ifdef INTEL_XVMC
   /* For XvMC */
   Bool XvMCEnabled;
#endif

   XF86ModReqInfo shadowReq; /* to test for later libshadow */
   Rotation rotation;
   void (*PointerMoved)(int, int, int);
   CreateScreenResourcesProcPtr    CreateScreenResources;

   i830_memory *power_context;

#ifdef XF86DRI
   i830_memory *back_buffer;
   i830_memory *depth_buffer;
   i830_memory *textures;		/**< Compatibility texture memory */
   i830_memory *memory_manager;		/**< DRI memory manager aperture */
   i830_memory *hw_status;		/* for G33 hw status page alloc */

   int TexGranularity;
   int drmMinor;
   Bool allocate_classic_textures;

   Bool can_resize;

   Bool want_vblank_interrupts;
#endif

   Bool need_mi_flush;

   Bool NeedRingBufferLow;
   Bool tiling;
   Bool fb_compression;

   int backPitch;

   Bool CursorNeedsPhysical;
 
   DGAModePtr DGAModes;
   int numDGAModes;
   Bool DGAactive;
   int DGAViewportStatus;

   int Chipset;
   unsigned long LinearAddr;
   unsigned long MMIOAddr;
   IOADDRESS ioBase;
   EntityInfoPtr pEnt;
#if XSERVER_LIBPCIACCESS
   struct pci_device *PciInfo;
#else
   pciVideoPtr PciInfo;
   PCITAG PciTag;
#endif
   uint8_t variant;

   unsigned int BR[20];

   unsigned char **ScanlineColorExpandBuffers;
   int NumScanlineColorExpandBuffers;
   int nextColorExpandBuf;

   Bool fence_used[FENCE_NEW_NR];

   accel_method_t accel;
#ifdef I830_USE_XAA
   XAAInfoRecPtr AccelInfoRec;

   /* additional XAA accelerated Composite support */
   CompositeProcPtr saved_composite;
   Bool (*xaa_check_composite)(int op, PicturePtr pSrc, PicturePtr pMask,
			       PicturePtr pDst);
   Bool (*xaa_prepare_composite)(int op, PicturePtr pSrc, PicturePtr pMask,
				 PicturePtr pDst, PixmapPtr pSrcPixmap,
				 PixmapPtr pMaskPixmap, PixmapPtr pDstPixmap);
   void (*xaa_composite)(PixmapPtr pDst, int xSrc, int ySrc,
			 int xMask, int yMask, int xDst, int yDst,
			 int w, int h);
   void (*xaa_done_composite)(PixmapPtr pDst);
#endif
   CloseScreenProcPtr CloseScreen;

   void (*batch_flush_notify)(ScrnInfoPtr pScrn);

#ifdef I830_USE_EXA
   ExaDriverPtr	EXADriverPtr;
#endif
#ifdef I830_USE_UXA
   uxa_driver_t *uxa_driver;
   Bool need_flush;
#endif
   Bool need_sync;
#if defined(I830_USE_EXA) || defined(I830_USE_UXA)
   PixmapPtr pSrcPixmap;
#endif
   int accel_pixmap_pitch_alignment;
   int accel_pixmap_offset_alignment;
   int accel_max_x;
   int accel_max_y;

   I830WriteIndexedByteFunc writeControl;
   I830ReadIndexedByteFunc readControl;
   I830WriteByteFunc writeStandard;
   I830ReadByteFunc readStandard;

   Bool XvDisabled;			/* Xv disabled in PreInit. */
   Bool XvEnabled;			/* Xv enabled for this generation. */
   Bool XvPreferOverlay;

#ifdef I830_XV
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
   } video;
#endif

   /* EXA render state */
   float scale_units[2][2];
  /** Transform pointers for src/mask, or NULL if identity */
   PictTransform *transform[2];
   float coord_adjust;
   /* i915 EXA render state */
   uint32_t mapstate[6];
   uint32_t samplerstate[6];

   struct {
      int op;
      PicturePtr pSrcPicture, pMaskPicture, pDstPicture;
      PixmapPtr pSrc, pMask, pDst;
      uint32_t dst_format;
      Bool is_nearest;
      Bool needs_emit;
   } i915_render_state;

   /* 965 render acceleration state */
   struct gen4_render_state *gen4_render_state;

   enum dri_type directRenderingType;	/* DRI enabled this generation. */

#ifdef XF86DRI
   Bool directRenderingOpen;
   int LockHeld;
   DRIInfoPtr pDRIInfo;
   int drmSubFD;
   int numVisualConfigs;
   __GLXvisualConfig *pVisualConfigs;
   I830ConfigPrivPtr pVisualConfigsPriv;
   drm_handle_t buffer_map;
   drm_handle_t ring_map;
   char deviceName[64];
#endif

   /* Broken-out options. */
   OptionInfoPtr Options;

   Bool lvds_24_bit_mode;
   Bool lvds_use_ssc;
   int lvds_ssc_freq; /* in MHz */
   Bool lvds_dither;
   DisplayModePtr lvds_fixed_mode;
   Bool skip_panel_detect;
   Bool integrated_lvds; /* LVDS config from driver feature BDB */

   Bool tv_present; /* TV connector present (from VBIOS) */

   /* Driver phase/state information */
   Bool preinit;
   Bool starting;
   Bool closing;
   Bool suspended;
   Bool leaving;

   /* fbOffset converted to (x, y). */
   int xoffset;
   int yoffset;

   unsigned int SaveGeneration;

   OsTimerPtr devicesTimer;

   int ddc2;

   enum backlight_control backlight_control_method;

   uint32_t saveDSPARB;
   uint32_t saveDSPACNTR;
   uint32_t saveDSPBCNTR;
   uint32_t savePIPEACONF;
   uint32_t savePIPEBCONF;
   uint32_t savePIPEASRC;
   uint32_t savePIPEBSRC;
   uint32_t saveFPA0;
   uint32_t saveFPA1;
   uint32_t saveDPLL_A;
   uint32_t saveDPLL_A_MD;
   uint32_t saveHTOTAL_A;
   uint32_t saveHBLANK_A;
   uint32_t saveHSYNC_A;
   uint32_t saveVTOTAL_A;
   uint32_t saveVBLANK_A;
   uint32_t saveVSYNC_A;
   uint32_t saveBCLRPAT_A;
   uint32_t saveDSPASTRIDE;
   uint32_t saveDSPASIZE;
   uint32_t saveDSPAPOS;
   uint32_t saveDSPABASE;
   uint32_t saveDSPASURF;
   uint32_t saveDSPATILEOFF;
   uint32_t saveFPB0;
   uint32_t saveFPB1;
   uint32_t saveDPLL_B;
   uint32_t saveDPLL_B_MD;
   uint32_t saveHTOTAL_B;
   uint32_t saveHBLANK_B;
   uint32_t saveHSYNC_B;
   uint32_t saveVTOTAL_B;
   uint32_t saveVBLANK_B;
   uint32_t saveVSYNC_B;
   uint32_t saveBCLRPAT_B;
   uint32_t saveDSPBSTRIDE;
   uint32_t saveDSPBSIZE;
   uint32_t saveDSPBPOS;
   uint32_t saveDSPBBASE;
   uint32_t saveDSPBSURF;
   uint32_t saveDSPBTILEOFF;
   uint32_t saveVCLK_DIVISOR_VGA0;
   uint32_t saveVCLK_DIVISOR_VGA1;
   uint32_t saveVCLK_POST_DIV;
   uint32_t saveVGACNTRL;
   uint32_t saveCURSOR_A_CONTROL;
   uint32_t saveCURSOR_A_BASE;
   uint32_t saveCURSOR_A_POSITION;
   uint32_t saveCURSOR_B_CONTROL;
   uint32_t saveCURSOR_B_BASE;
   uint32_t saveCURSOR_B_POSITION;
   uint32_t saveADPA;
   uint32_t saveLVDS;
   uint32_t saveDVOA;
   uint32_t saveDVOB;
   uint32_t saveDVOC;
   uint32_t savePP_ON;
   uint32_t savePP_OFF;
   uint32_t savePP_CONTROL;
   uint32_t savePP_DIVISOR;
   uint32_t savePFIT_CONTROL;
   uint32_t savePaletteA[256];
   uint32_t savePaletteB[256];
   uint32_t saveSWF[17];
   uint32_t saveBLC_PWM_CTL;
   uint32_t saveBLC_PWM_CTL2;
   uint32_t saveFBC_CFB_BASE;
   uint32_t saveFBC_LL_BASE;
   uint32_t saveFBC_CONTROL2;
   uint32_t saveFBC_CONTROL;
   uint32_t saveFBC_FENCE_OFF;
   uint32_t saveRENCLK_GATE_D1;
   uint32_t saveRENCLK_GATE_D2;
   uint32_t saveDSPCLK_GATE_D;
   uint32_t saveRAMCLK_GATE_D;
   uint32_t savePWRCTXA;

   enum last_3d last_3d;

   Bool use_drm_mode;
   Bool kernel_exec_fencing;

   /** Enables logging of debug output related to mode switching. */
   Bool debug_modes;
   unsigned int quirk_flag;

    /** User option to print acceleration fallback info to the server log. */
   Bool fallback_debug;
} I830Rec;

#define I830PTR(p) ((I830Ptr)((p)->driverPrivate))

#define ARRAY_SIZE(x) (sizeof(x) / sizeof(x[0]))

#define I830_SELECT_FRONT	0
#define I830_SELECT_BACK	1
#define I830_SELECT_DEPTH	2
#define I830_SELECT_THIRD	3

unsigned long intel_get_pixmap_offset(PixmapPtr pPix);
unsigned long intel_get_pixmap_pitch(PixmapPtr pPix);

struct i830_exa_pixmap_priv {
    dri_bo *bo;
};

/* Batchbuffer support macros and functions */
#include "i830_batchbuffer.h"

/* I830 specific functions */
extern int I830WaitLpRing(ScrnInfoPtr pScrn, int n, int timeout_millis);
extern void I830SetPIOAccess(I830Ptr pI830);
extern void I830SetMMIOAccess(I830Ptr pI830);
extern void I830Sync(ScrnInfoPtr pScrn);
extern void I830InitHWCursor(ScrnInfoPtr pScrn);
extern void I830SetPipeCursor (xf86CrtcPtr crtc, Bool force);
extern Bool I830CursorInit(ScreenPtr pScreen);
extern void IntelEmitInvarientState(ScrnInfoPtr pScrn);
extern void I830EmitInvarientState(ScrnInfoPtr pScrn);
extern void I915EmitInvarientState(ScrnInfoPtr pScrn);
extern Bool I830SelectBuffer(ScrnInfoPtr pScrn, int buffer);
void i830_update_cursor_offsets(ScrnInfoPtr pScrn);

/* CRTC-based cursor functions */
void
i830_crtc_load_cursor_image (xf86CrtcPtr crtc, unsigned char *src);

#ifdef ARGB_CURSOR
void
i830_crtc_load_cursor_argb (xf86CrtcPtr crtc, CARD32 *image);
#endif

void
i830_crtc_set_cursor_position (xf86CrtcPtr crtc, int x, int y);

void
i830_crtc_show_cursor (xf86CrtcPtr crtc);

void
i830_crtc_hide_cursor (xf86CrtcPtr crtc);

void
i830_crtc_set_cursor_colors (xf86CrtcPtr crtc, int bg, int fg);

extern void i830_refresh_ring(ScrnInfoPtr pScrn);
extern void I830EmitFlush(ScrnInfoPtr pScrn);

#ifdef I830_XV
extern void I830InitVideo(ScreenPtr pScreen);
extern void i830_crtc_dpms_video(xf86CrtcPtr crtc, Bool on);
#endif

int
i830_crtc_pipe (xf86CrtcPtr crtc);

extern xf86CrtcPtr i830_pipe_to_crtc(ScrnInfoPtr pScrn, int pipe);

Bool
i830_pipe_a_require_activate (ScrnInfoPtr scrn);

void
i830_pipe_a_require_deactivate (ScrnInfoPtr scrn);

#ifdef XF86DRI
extern Bool I830Allocate3DMemory(ScrnInfoPtr pScrn, const int flags);
extern void I830SetupMemoryTiling(ScrnInfoPtr pScrn);
extern Bool I830DRIScreenInit(ScreenPtr pScreen);
extern Bool I830CheckDRIAvailable(ScrnInfoPtr pScrn);
extern Bool I830DRIDoMappings(ScreenPtr pScreen);
extern Bool I830DRIResume(ScreenPtr pScreen);
extern void I830DRICloseScreen(ScreenPtr pScreen);
extern Bool I830DRIFinishScreenInit(ScreenPtr pScreen);
extern void I830DRIUnlock(ScrnInfoPtr pScrn);
extern Bool I830DRILock(ScrnInfoPtr pScrn);
extern Bool I830DRISetVBlankInterrupt (ScrnInfoPtr pScrn, Bool on);
extern Bool i830_update_dri_buffers(ScrnInfoPtr pScrn);
extern Bool I830DRISetHWS(ScrnInfoPtr pScrn);
extern Bool I830DRIInstIrqHandler(ScrnInfoPtr pScrn);
#endif

#ifdef DRI2
Bool I830DRI2ScreenInit(ScreenPtr pScreen);
void I830DRI2CloseScreen(ScreenPtr pScreen);
#endif

#ifdef XF86DRM_MODE
extern Bool drmmode_pre_init(ScrnInfoPtr pScrn, int fd, int cpp);
extern Bool drmmode_is_rotate_pixmap(ScrnInfoPtr pScrn, pointer pPixData,
				     dri_bo **bo);
#endif

extern Bool I830AccelInit(ScreenPtr pScreen);
extern void I830SetupForScreenToScreenCopy(ScrnInfoPtr pScrn, int xdir,
					   int ydir, int rop,
					   unsigned int planemask,
					   int trans_color);
extern void I830SubsequentScreenToScreenCopy(ScrnInfoPtr pScrn, int srcX,
					     int srcY, int dstX, int dstY,
					     int w, int h);
extern void I830SetupForSolidFill(ScrnInfoPtr pScrn, int color, int rop,
				  unsigned int planemask);
extern void I830SubsequentSolidFillRect(ScrnInfoPtr pScrn, int x, int y,
					int w, int h);

Bool i830_allocator_init(ScrnInfoPtr pScrn, unsigned long offset,
			 unsigned long size);
void i830_allocator_fini(ScrnInfoPtr pScrn);
i830_memory * i830_allocate_memory(ScrnInfoPtr pScrn, const char *name,
				   unsigned long size, unsigned long pitch,
				   unsigned long alignment, int flags,
				   enum tile_format tile_format);
void i830_describe_allocations(ScrnInfoPtr pScrn, int verbosity,
			       const char *prefix);
void i830_reset_allocations(ScrnInfoPtr pScrn);
void i830_free_3d_memory(ScrnInfoPtr pScrn);
void i830_free_memory(ScrnInfoPtr pScrn, i830_memory *mem);
extern long I830CheckAvailableMemory(ScrnInfoPtr pScrn);
Bool i830_allocate_2d_memory(ScrnInfoPtr pScrn);
Bool i830_allocate_texture_memory(ScrnInfoPtr pScrn);
Bool i830_allocate_pwrctx(ScrnInfoPtr pScrn);
Bool i830_allocate_3d_memory(ScrnInfoPtr pScrn);
void i830_init_bufmgr(ScrnInfoPtr pScrn);
#ifdef INTEL_XVMC
Bool i830_allocate_xvmc_buffer(ScrnInfoPtr pScrn, const char *name,
                               i830_memory **buffer, unsigned long size, int flags);
#endif
extern void i830_update_front_offset(ScrnInfoPtr pScrn);
extern uint32_t i830_create_new_fb(ScrnInfoPtr pScrn, int width, int height,
				   int *pitch);

Bool
i830_tiled_width(I830Ptr i830, int *width, int cpp);

int
i830_pad_drawable_width(int width, int cpp);


extern Bool I830I2CInit(ScrnInfoPtr pScrn, I2CBusPtr *bus_ptr, int i2c_reg,
			char *name);

/* return a mask of output indices matching outputs against type_mask */
int i830_output_clones (ScrnInfoPtr pScrn, int type_mask);

/* i830_display.c */
Bool
i830PipeHasType (xf86CrtcPtr crtc, int type);

/* i830_crt.c */
void i830_crt_init(ScrnInfoPtr pScrn);

/* i830_dvo.c */
void i830_dvo_init(ScrnInfoPtr pScrn);

/* i830_hdmi.c */
void i830_hdmi_init(ScrnInfoPtr pScrn, int output_reg);

/* i830_lvds.c */
void i830_lvds_init(ScrnInfoPtr pScrn);

extern void i830MarkSync(ScrnInfoPtr pScrn);
extern void i830WaitSync(ScrnInfoPtr pScrn);

/* i830_memory.c */
Bool i830_bind_all_memory(ScrnInfoPtr pScrn);
Bool i830_unbind_all_memory(ScrnInfoPtr pScrn);
unsigned long i830_get_fence_size(I830Ptr pI830, unsigned long size);
unsigned long i830_get_fence_pitch(I830Ptr pI830, unsigned long pitch, int format);
unsigned long i830_get_fence_alignment(I830Ptr pI830, unsigned long size);

Bool I830BindAGPMemory(ScrnInfoPtr pScrn);
Bool I830UnbindAGPMemory(ScrnInfoPtr pScrn);

i830_memory *
i830_allocate_framebuffer(ScrnInfoPtr pScrn);

/* i830_modes.c */
DisplayModePtr i830_ddc_get_modes(xf86OutputPtr output);

/* i830_tv.c */
void i830_tv_init(ScrnInfoPtr pScrn);

/* i830_render.c */
Bool i830_check_composite(int op, PicturePtr pSrc, PicturePtr pMask,
			  PicturePtr pDst);
Bool i830_prepare_composite(int op, PicturePtr pSrc, PicturePtr pMask,
			    PicturePtr pDst, PixmapPtr pSrcPixmap,
			    PixmapPtr pMaskPixmap, PixmapPtr pDstPixmap);
Bool
i830_transform_is_affine (PictTransformPtr t);

void i830_composite(PixmapPtr pDst, int srcX, int srcY,
		    int maskX, int maskY, int dstX, int dstY, int w, int h);
void i830_done_composite(PixmapPtr pDst);
/* i915_render.c */
Bool i915_check_composite(int op, PicturePtr pSrc, PicturePtr pMask,
			  PicturePtr pDst);
Bool i915_prepare_composite(int op, PicturePtr pSrc, PicturePtr pMask,
			    PicturePtr pDst, PixmapPtr pSrcPixmap,
			    PixmapPtr pMaskPixmap, PixmapPtr pDstPixmap);
void i915_composite(PixmapPtr pDst, int srcX, int srcY,
		    int maskX, int maskY, int dstX, int dstY, int w, int h);
void i915_batch_flush_notify(ScrnInfoPtr pScrn);
/* i965_render.c */
unsigned int gen4_render_state_size(ScrnInfoPtr pScrn);
void gen4_render_state_init(ScrnInfoPtr pScrn);
void gen4_render_state_cleanup(ScrnInfoPtr pScrn);
Bool i965_check_composite(int op, PicturePtr pSrc, PicturePtr pMask,
			  PicturePtr pDst);
Bool i965_prepare_composite(int op, PicturePtr pSrc, PicturePtr pMask,
			    PicturePtr pDst, PixmapPtr pSrcPixmap,
			    PixmapPtr pMaskPixmap, PixmapPtr pDstPixmap);
void i965_composite(PixmapPtr pDst, int srcX, int srcY,
		    int maskX, int maskY, int dstX, int dstY, int w, int h);

void
i965_batch_flush_notify(ScrnInfoPtr pScrn);

Bool
i830_get_transformed_coordinates(int x, int y, PictTransformPtr transform,
				 float *x_out, float *y_out);

Bool
i830_get_transformed_coordinates_3d(int x, int y, PictTransformPtr transform,
				    float *x_out, float *y_out, float *z_out);

void i830_enter_render(ScrnInfoPtr);

#ifndef SERVER_1_5
Bool xf86MonitorIsHDMI(xf86MonPtr mon);
#endif

static inline void
i830_wait_ring_idle(ScrnInfoPtr pScrn)
{
   I830Ptr pI830 = I830PTR(pScrn);

   if (pI830->accel != ACCEL_NONE)
       I830WaitLpRing(pScrn, pI830->ring.mem->size - 8, 0);
}

static inline int i830_fb_compression_supported(I830Ptr pI830)
{
    if (!IS_MOBILE(pI830))
	return FALSE;
    if (IS_I810(pI830) || IS_I815(pI830) || IS_I830(pI830))
	return FALSE;
    /* fbc depends on tiled surface. And we don't support tiled
     * front buffer with XAA now.
     */
    if (!pI830->tiling || (IS_I965G(pI830) && pI830->accel <= ACCEL_XAA))
	return FALSE;
    /* We have not gotten FBC to work consistently on 965GM. Our best
     * working theory right now is that FBC simply isn't reliable on
     * that device. See this bug report for more details:
     * https://bugs.freedesktop.org/show_bug.cgi?id=16257
     */
    if (IS_I965GM(pI830))
	return FALSE;
    return TRUE;
}

#define I830FALLBACK(s, arg...)				\
do {							\
    if (I830PTR(pScrn)->fallback_debug) {		\
	xf86DrvMsg(pScrn->scrnIndex, X_INFO,		\
		   "EXA fallback: " s "\n", ##arg);	\
    }							\
    return FALSE;					\
} while(0)

Bool i830_pixmap_tiled(PixmapPtr p);

#define i830_exa_check_pitch_2d(p) do {\
    uint32_t pitch = intel_get_pixmap_pitch(p);\
    if (pitch > KB(32)) I830FALLBACK("pitch exceeds 2d limit 32K\n");\
} while(0)

/* For pre-965 chip only, as they have 8KB limit for 3D */
#define i830_exa_check_pitch_3d(p) do {\
    uint32_t pitch = intel_get_pixmap_pitch(p);\
    if (pitch > KB(8)) I830FALLBACK("pitch exceeds 3d limit 8K\n");\
} while(0)

/**
 * Little wrapper around drm_intel_bo_reloc to return the initial value you
 * should stuff into the relocation entry.
 *
 * If only we'd done this before settling on the library API.
 */
static inline uint32_t
intel_emit_reloc(drm_intel_bo *bo, uint32_t offset,
		 drm_intel_bo *target_bo, uint32_t target_offset,
		 uint32_t read_domains, uint32_t write_domain)
{
    drm_intel_bo_emit_reloc(bo, offset, target_bo, target_offset,
			    read_domains, write_domain);

    return target_bo->offset + target_offset;
}

static inline drm_intel_bo *
intel_bo_alloc_for_data(ScrnInfoPtr scrn, void *data, unsigned int size,
			char *name)
{
    I830Ptr pI830 = I830PTR(scrn);
    drm_intel_bo *bo;

    bo = drm_intel_bo_alloc(pI830->bufmgr, name, size, 4096);
    if (!bo)
	return NULL;
    drm_intel_bo_subdata(bo, 0, size, data);

    return bo;
}

extern const int I830PatternROP[16];
extern const int I830CopyROP[16];

/* Flags for memory allocation function */
#define NEED_PHYSICAL_ADDR		0x00000001
#define ALIGN_BOTH_ENDS			0x00000002
#define NEED_NON_STOLEN			0x00000004
#define NEED_LIFETIME_FIXED		0x00000008
#define ALLOW_SHARING			0x00000010

/* Chipset registers for VIDEO BIOS memory RW access */
#define _855_DRAM_RW_CONTROL 0x58
#define _845_DRAM_RW_CONTROL 0x90
#define DRAM_WRITE    0x33330000

/* quirk flag definition */
#define QUIRK_IGNORE_TV			0x00000001
#define QUIRK_IGNORE_LVDS		0x00000002
#define QUIRK_IGNORE_MACMINI_LVDS 	0x00000004
#define QUIRK_PIPEA_FORCE		0x00000008
#define QUIRK_IVCH_NEED_DVOB		0x00000010
#define QUIRK_RESET_MODES		0x00000020
#define QUIRK_PFIT_SAFE			0x00000040
#define QUIRK_IGNORE_CRT		0x00000080
#define QUIRK_BROKEN_ACPI_LID		0x00000100
extern void i830_fixup_devices(ScrnInfoPtr);

/**
 * Hints to CreatePixmap to tell the driver how the pixmap is going to be
 * used.
 *
 * Compare to CREATE_PIXMAP_USAGE_* in the server.
 */
enum {
    INTEL_CREATE_PIXMAP_TILING_X = 0x10000000,
    INTEL_CREATE_PIXMAP_TILING_Y,
};

static inline PixmapPtr
get_drawable_pixmap(DrawablePtr drawable)
{
    ScreenPtr screen = drawable->pScreen;

    if (drawable->type == DRAWABLE_PIXMAP)
	return (PixmapPtr)drawable;
    else
	return screen->GetWindowPixmap((WindowPtr)drawable);
}

static inline Bool
pixmap_is_scanout(PixmapPtr pixmap)
{
    ScreenPtr screen = pixmap->drawable.pScreen;

    return pixmap == screen->GetScreenPixmap(screen);
}

#endif /* _I830_H_ */
