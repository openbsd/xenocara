/* i810_common.h -- common header definitions for I810 2D/3D/DRM suite
 *
 * Copyright 2002 Tungsten Graphics, Inc., Cedar Park, Texas.
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
 * PRECISION INSIGHT AND/OR ITS SUPPLIERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 * Converted to common header format:
 *   Jens Owen <jens@tungstengraphics.com>
 *
 *
 */

/* WARNING: If you change any of these defines, make sure to change
 * the kernel include file as well (i810_drm.h)
 */

#ifndef _I810_COMMON_H_
#define _I810_COMMON_H_

/* Provide substitutes for gcc's __FUNCTION__ on other compilers */
#if !defined(__GNUC__) && !defined(__FUNCTION__)
# if defined(__STDC__) && (__STDC_VERSION__>=199901L) /* C99 */
#  define __FUNCTION__ __func__
# else
#  define __FUNCTION__ ""
# endif
#endif

#define PFX __FILE__,__LINE__,__FUNCTION__
#define FUNCTION_NAME __FUNCTION__

#define KB(x) ((x) * 1024)
#define MB(x) ((x) * KB(1024))

#define ALIGN(i,m) (((i) + (m) - 1) & ~((m) - 1))

/* Using usleep() makes things noticably slow. */
#if 0
#define DELAY(x) usleep(x)
#else
#define DELAY(x) do {;} while (0)
#endif

#define PrintErrorState I810PrintErrorState
#define WaitRingFunc I810WaitLpRing
#define RecPtr pI810

static inline void memset_volatile(volatile void *b, int c, size_t len)
{
    int i;
    
    for (i = 0; i < len; i++)
	((volatile char *)b)[i] = c;
}

static inline void memcpy_volatile(volatile void *dst, const void *src,
				   size_t len)
{
    int i;
    
    for (i = 0; i < len; i++)
	((volatile char *)dst)[i] = ((const volatile char *)src)[i];
}

/* Memory mapped register access macros */
#define INREG8(addr)        *(volatile uint8_t *)(RecPtr->MMIOBase + (addr))
#define INREG16(addr)       *(volatile uint16_t *)(RecPtr->MMIOBase + (addr))
#define INREG(addr)         *(volatile uint32_t *)(RecPtr->MMIOBase + (addr))
#define INGTT(addr)         *(volatile uint32_t *)(RecPtr->GTTBase + (addr))
#define POSTING_READ(addr)  (void)INREG(addr)

#define OUTREG8(addr, val) do {						\
   *(volatile uint8_t *)(RecPtr->MMIOBase  + (addr)) = (val);		\
   if (I810_DEBUG&DEBUG_VERBOSE_OUTREG) {				\
      ErrorF("OUTREG8(0x%lx, 0x%lx) in %s\n", (unsigned long)(addr),	\
		(unsigned long)(val), FUNCTION_NAME);			\
   }									\
} while (0)

#define OUTREG16(addr, val) do {					\
   *(volatile uint16_t *)(RecPtr->MMIOBase + (addr)) = (val);		\
   if (I810_DEBUG&DEBUG_VERBOSE_OUTREG) {				\
      ErrorF("OUTREG16(0x%lx, 0x%lx) in %s\n", (unsigned long)(addr),	\
		(unsigned long)(val), FUNCTION_NAME);			\
   }									\
} while (0)

#define OUTREG(addr, val) do {						\
   *(volatile uint32_t *)(RecPtr->MMIOBase + (addr)) = (val);		\
   if (I810_DEBUG&DEBUG_VERBOSE_OUTREG) {				\
      ErrorF("OUTREG(0x%lx, 0x%lx) in %s\n", (unsigned long)(addr),	\
		(unsigned long)(val), FUNCTION_NAME);			\
   }									\
} while (0)

/* To remove all debugging, make sure I810_DEBUG is defined as a
 * preprocessor symbol, and equal to zero.
 */
#if 1
#define I810_DEBUG 0
#endif
#ifndef I810_DEBUG
#warning "Debugging enabled - expect reduced performance"
extern int I810_DEBUG;
#endif

#define DEBUG_VERBOSE_ACCEL  0x1
#define DEBUG_VERBOSE_SYNC   0x2
#define DEBUG_VERBOSE_VGA    0x4
#define DEBUG_VERBOSE_RING   0x8
#define DEBUG_VERBOSE_OUTREG 0x10
#define DEBUG_VERBOSE_MEMORY 0x20
#define DEBUG_VERBOSE_CURSOR 0x40
#define DEBUG_ALWAYS_SYNC    0x80
#define DEBUG_VERBOSE_DRI    0x100
#define DEBUG_VERBOSE_BIOS   0x200

/* Size of the mmio region.
 */
#define I810_REG_SIZE 0x80000

#define GTT_PAGE_SIZE			KB(4)
#define PRIMARY_RINGBUFFER_SIZE		KB(128)
#define MIN_SCRATCH_BUFFER_SIZE		KB(16)
#define MAX_SCRATCH_BUFFER_SIZE		KB(64)
#define HWCURSOR_SIZE			GTT_PAGE_SIZE
#define HWCURSOR_SIZE_ARGB		GTT_PAGE_SIZE * 4

/* Use a 64x64 HW cursor */
#define I810_CURSOR_X			64
#define I810_CURSOR_Y			I810_CURSOR_X

#define PIPE_NAME(n)			('A' + (n))

extern struct pci_device *
intel_host_bridge (void);

/**
 * Hints to CreatePixmap to tell the driver how the pixmap is going to be
 * used.
 *
 * Compare to CREATE_PIXMAP_USAGE_* in the server.
 */
enum {
	INTEL_CREATE_PIXMAP_TILING_X = 0x10000000,
	INTEL_CREATE_PIXMAP_TILING_Y,
	INTEL_CREATE_PIXMAP_TILING_NONE,
};

#ifndef _I810_DEFINES_
#define _I810_DEFINES_
#define I810_USE_BATCH 1

#define I810_DMA_BUF_ORDER     12
#define I810_DMA_BUF_SZ        (1<<I810_DMA_BUF_ORDER)
#define I810_DMA_BUF_NR        256

#define I810_NR_SAREA_CLIPRECTS 8

/* Each region is a minimum of 64k, and there are at most 64 of them.
 */
#define I810_NR_TEX_REGIONS 64
#define I810_LOG_MIN_TEX_REGION_SIZE 16

/* Destbuffer state
 *    - backbuffer linear offset and pitch -- invarient in the current dri
 *    - zbuffer linear offset and pitch -- also invarient
 *    - drawing origin in back and depth buffers.
 *
 * Keep the depth/back buffer state here to acommodate private buffers
 * in the future.
 */
#define I810_DESTREG_DI0  0		/* CMD_OP_DESTBUFFER_INFO (2 dwords) */
#define I810_DESTREG_DI1  1
#define I810_DESTREG_DV0  2		/* GFX_OP_DESTBUFFER_VARS (2 dwords) */
#define I810_DESTREG_DV1  3
#define I810_DESTREG_DR0  4		/* GFX_OP_DRAWRECT_INFO (4 dwords) */
#define I810_DESTREG_DR1  5
#define I810_DESTREG_DR2  6
#define I810_DESTREG_DR3  7
#define I810_DESTREG_DR4  8
#define I810_DEST_SETUP_SIZE 10

/* Context state
 */
#define I810_CTXREG_CF0   0		/* GFX_OP_COLOR_FACTOR */
#define I810_CTXREG_CF1   1
#define I810_CTXREG_ST0   2		/* GFX_OP_STIPPLE */
#define I810_CTXREG_ST1   3
#define I810_CTXREG_VF    4		/* GFX_OP_VERTEX_FMT */
#define I810_CTXREG_MT    5		/* GFX_OP_MAP_TEXELS */
#define I810_CTXREG_MC0   6		/* GFX_OP_MAP_COLOR_STAGES - stage 0 */
#define I810_CTXREG_MC1   7		/* GFX_OP_MAP_COLOR_STAGES - stage 1 */
#define I810_CTXREG_MC2   8		/* GFX_OP_MAP_COLOR_STAGES - stage 2 */
#define I810_CTXREG_MA0   9		/* GFX_OP_MAP_ALPHA_STAGES - stage 0 */
#define I810_CTXREG_MA1   10		/* GFX_OP_MAP_ALPHA_STAGES - stage 1 */
#define I810_CTXREG_MA2   11		/* GFX_OP_MAP_ALPHA_STAGES - stage 2 */
#define I810_CTXREG_SDM   12		/* GFX_OP_SRC_DEST_MONO */
#define I810_CTXREG_FOG   13		/* GFX_OP_FOG_COLOR */
#define I810_CTXREG_B1    14		/* GFX_OP_BOOL_1 */
#define I810_CTXREG_B2    15		/* GFX_OP_BOOL_2 */
#define I810_CTXREG_LCS   16		/* GFX_OP_LINEWIDTH_CULL_SHADE_MODE */
#define I810_CTXREG_PV    17		/* GFX_OP_PV_RULE -- Invarient! */
#define I810_CTXREG_ZA    18		/* GFX_OP_ZBIAS_ALPHAFUNC */
#define I810_CTXREG_AA    19		/* GFX_OP_ANTIALIAS */
#define I810_CTX_SETUP_SIZE 20

/* Texture state (per tex unit)
 */
#define I810_TEXREG_MI0  0		/* GFX_OP_MAP_INFO (4 dwords) */
#define I810_TEXREG_MI1  1
#define I810_TEXREG_MI2  2
#define I810_TEXREG_MI3  3
#define I810_TEXREG_MF   4		/* GFX_OP_MAP_FILTER */
#define I810_TEXREG_MLC  5		/* GFX_OP_MAP_LOD_CTL */
#define I810_TEXREG_MLL  6		/* GFX_OP_MAP_LOD_LIMITS */
#define I810_TEXREG_MCS  7		/* GFX_OP_MAP_COORD_SETS ??? */
#define I810_TEX_SETUP_SIZE 8

/* Driver specific DRM command indices
 * NOTE: these are not OS specific, but they are driver specific
 */
#define DRM_I810_INIT                     0x00
#define DRM_I810_VERTEX                   0x01
#define DRM_I810_CLEAR                    0x02
#define DRM_I810_FLUSH                    0x03
#define DRM_I810_GETAGE                   0x04
#define DRM_I810_GETBUF                   0x05
#define DRM_I810_SWAP                     0x06
#define DRM_I810_COPY                     0x07
#define DRM_I810_DOCOPY                   0x08
#define DRM_I810_OV0INFO                  0x09
#define DRM_I810_FSTATUS                  0x0a
#define DRM_I810_OV0FLIP                  0x0b
#define DRM_I810_MC                       0x0c
#define DRM_I810_RSTATUS                  0x0d
#define DRM_I810_FLIP                     0x0e

#endif

typedef enum _drmI810Initfunc {
	I810_INIT_DMA = 0x01,
	I810_CLEANUP_DMA = 0x02,
	I810_INIT_DMA_1_4 = 0x03
} drmI810Initfunc;

typedef struct {
   drmI810Initfunc func;
   unsigned int mmio_offset;
   unsigned int buffers_offset;
   int sarea_priv_offset;
   unsigned int ring_start;
   unsigned int ring_end;
   unsigned int ring_size;
   unsigned int front_offset;
   unsigned int back_offset;
   unsigned int depth_offset;
   unsigned int overlay_offset;
   unsigned int overlay_physical;
   unsigned int w;
   unsigned int h;
   unsigned int pitch;
   unsigned int pitch_bits;
} drmI810Init;

typedef struct {
   void *virtual;
   int request_idx;
   int request_size;
   int granted;
} drmI810DMA;

/* Flags for clear ioctl
 */
#define I810_FRONT   0x1
#define I810_BACK    0x2
#define I810_DEPTH   0x4

typedef struct {
   int clear_color;
   int clear_depth;
   int flags;
} drmI810Clear;

typedef struct {
   int idx;				/* buffer index */
   int used;				/* nr bytes in use */
   int discard;				/* client is finished with the buffer? */
} drmI810Vertex;

/* Flags for vertex ioctl
 */
#define PR_TRIANGLES         (0x0<<18)
#define PR_TRISTRIP_0        (0x1<<18)
#define PR_TRISTRIP_1        (0x2<<18)
#define PR_TRIFAN            (0x3<<18)
#define PR_POLYGON           (0x4<<18)
#define PR_LINES             (0x5<<18)
#define PR_LINESTRIP         (0x6<<18)
#define PR_RECTS             (0x7<<18)
#define PR_MASK              (0x7<<18)

#endif
