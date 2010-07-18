
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

#ifndef _INTEL_COMMON_H_
#define _INTEL_COMMON_H_

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

#ifdef I830DEBUG
#define MARKER() ErrorF("\n### %s:%d: >>> %s <<< ###\n\n", \
			 __FILE__, __LINE__,__FUNCTION__)
#define DPRINTF I830DPRINTF
#else /* #ifdef I830DEBUG */
#define MARKER()
#define DPRINTF I830DPRINTF_stub
static inline void
I830DPRINTF_stub(const char *filename, int line, const char *function,
		 const char *fmt, ...)
{
}
#endif /* #ifdef I830DEBUG */

#define KB(x) ((x) * 1024)
#define MB(x) ((x) * KB(1024))

/* Using usleep() makes things noticably slow. */
#if 0
#define DELAY(x) usleep(x)
#else
#define DELAY(x) do {;} while (0)
#endif

#ifndef REG_DUMPER
/* I830 hooks for the I810 driver setup/probe. */
extern const OptionInfoRec *I830AvailableOptions(int chipid, int busid);
extern void intel_init_scrn(ScrnInfoPtr scrn);

/* Symbol lists shared by the i810 and i830 parts. */
extern int I830EntityIndex;

extern void I830DPRINTF_stub(const char *filename, int line,
			     const char *function, const char *fmt, ...);

#ifdef _I830_H_
#define PrintErrorState i830_dump_error_state
#define WaitRingFunc I830WaitLpRing
#define RecPtr intel
#else
#define PrintErrorState I810PrintErrorState
#define WaitRingFunc I810WaitLpRing
#define RecPtr pI810
#endif

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
	((volatile char *)dst)[i] = ((volatile char *)src)[i];
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
#endif /* !REG_DUMPER */

/* Size of the mmio region.
 */
#define I810_REG_SIZE 0x80000

#ifndef PCI_CHIP_I810
#define PCI_CHIP_I810              0x7121
#define PCI_CHIP_I810_DC100        0x7123
#define PCI_CHIP_I810_E            0x7125
#define PCI_CHIP_I815              0x1132
#define PCI_CHIP_I810_BRIDGE       0x7120
#define PCI_CHIP_I810_DC100_BRIDGE 0x7122
#define PCI_CHIP_I810_E_BRIDGE     0x7124
#define PCI_CHIP_I815_BRIDGE       0x1130
#endif

#ifndef PCI_CHIP_I830_M
#define PCI_CHIP_I830_M            0x3577
#define PCI_CHIP_I830_M_BRIDGE     0x3575
#endif

#ifndef PCI_CHIP_845_G
#define PCI_CHIP_845_G		   0x2562
#define PCI_CHIP_845_G_BRIDGE	   0x2560
#endif

#ifndef PCI_CHIP_I855_GM
#define PCI_CHIP_I855_GM	   0x3582
#define PCI_CHIP_I855_GM_BRIDGE	   0x3580
#endif

#ifndef PCI_CHIP_I865_G
#define PCI_CHIP_I865_G		   0x2572
#define PCI_CHIP_I865_G_BRIDGE	   0x2570
#endif

#ifndef PCI_CHIP_I915_G
#define PCI_CHIP_I915_G		   0x2582
#define PCI_CHIP_I915_G_BRIDGE	   0x2580
#endif

#ifndef PCI_CHIP_I915_GM
#define PCI_CHIP_I915_GM	   0x2592
#define PCI_CHIP_I915_GM_BRIDGE	   0x2590
#endif

#ifndef PCI_CHIP_E7221_G
#define PCI_CHIP_E7221_G	   0x258A
/* Same as I915_G_BRIDGE */
#define PCI_CHIP_E7221_G_BRIDGE	   0x2580
#endif

#ifndef PCI_CHIP_I945_G
#define PCI_CHIP_I945_G        0x2772
#define PCI_CHIP_I945_G_BRIDGE 0x2770
#endif

#ifndef PCI_CHIP_I945_GM
#define PCI_CHIP_I945_GM        0x27A2
#define PCI_CHIP_I945_GM_BRIDGE 0x27A0
#endif

#ifndef PCI_CHIP_I945_GME
#define PCI_CHIP_I945_GME	 0x27AE
#define PCI_CHIP_I945_GME_BRIDGE 0x27AC
#endif

#ifndef PCI_CHIP_IGD_GM
#define PCI_CHIP_IGD_GM		0xA011
#define PCI_CHIP_IGD_GM_BRIDGE	0xA010
#define PCI_CHIP_IGD_G		0xA001
#define PCI_CHIP_IGD_G_BRIDGE	0xA000
#endif

#ifndef PCI_CHIP_G35_G
#define PCI_CHIP_G35_G		0x2982
#define PCI_CHIP_G35_G_BRIDGE 	0x2980
#endif

#ifndef PCI_CHIP_I965_Q
#define PCI_CHIP_I965_Q		0x2992
#define PCI_CHIP_I965_Q_BRIDGE 	0x2990
#endif

#ifndef PCI_CHIP_I965_G
#define PCI_CHIP_I965_G		0x29A2
#define PCI_CHIP_I965_G_BRIDGE 	0x29A0
#endif

#ifndef PCI_CHIP_I946_GZ
#define PCI_CHIP_I946_GZ		0x2972
#define PCI_CHIP_I946_GZ_BRIDGE 	0x2970
#endif

#ifndef PCI_CHIP_I965_GM
#define PCI_CHIP_I965_GM        0x2A02
#define PCI_CHIP_I965_GM_BRIDGE 0x2A00
#endif

#ifndef PCI_CHIP_I965_GME
#define PCI_CHIP_I965_GME       0x2A12
#define PCI_CHIP_I965_GME_BRIDGE 0x2A10
#endif

#ifndef PCI_CHIP_G33_G
#define PCI_CHIP_G33_G		0x29C2
#define PCI_CHIP_G33_G_BRIDGE 	0x29C0
#endif

#ifndef PCI_CHIP_Q35_G
#define PCI_CHIP_Q35_G		0x29B2
#define PCI_CHIP_Q35_G_BRIDGE 	0x29B0
#endif

#ifndef PCI_CHIP_Q33_G
#define PCI_CHIP_Q33_G		0x29D2
#define PCI_CHIP_Q33_G_BRIDGE 	0x29D0
#endif

#ifndef PCI_CHIP_GM45_GM
#define PCI_CHIP_GM45_GM	0x2A42
#define PCI_CHIP_GM45_BRIDGE    0x2A40
#endif

#ifndef PCI_CHIP_IGD_E_G
#define PCI_CHIP_IGD_E_G	0x2E02
#define PCI_CHIP_IGD_E_G_BRIDGE 0x2E00
#endif

#ifndef PCI_CHIP_G45_G
#define PCI_CHIP_G45_G		0x2E22
#define PCI_CHIP_G45_G_BRIDGE	0x2E20
#endif

#ifndef PCI_CHIP_Q45_G
#define PCI_CHIP_Q45_G		0x2E12
#define PCI_CHIP_Q45_G_BRIDGE	0x2E10
#endif

#ifndef PCI_CHIP_G41_G
#define PCI_CHIP_G41_G		0x2E32
#define PCI_CHIP_G41_G_BRIDGE	0x2E30
#endif

#ifndef PCI_CHIP_B43_G
#define PCI_CHIP_B43_G		0x2E42
#define PCI_CHIP_B43_G_BRIDGE	0x2E40
#endif

#ifndef PCI_CHIP_IGDNG_D_G
#define PCI_CHIP_IGDNG_D_G		0x0042
#define PCI_CHIP_IGDNG_D_G_BRIDGE	0x0040
#endif

#ifndef PCI_CHIP_IGDNG_M_G
#define PCI_CHIP_IGDNG_M_G		0x0046
#define PCI_CHIP_IGDNG_M_G_BRIDGE	0x0044
#endif

#define I810_MEMBASE(p,n) (p)->regions[(n)].base_addr
#define VENDOR_ID(p)      (p)->vendor_id
#define DEVICE_ID(p)      (p)->device_id
#define SUBVENDOR_ID(p)	  (p)->subvendor_id
#define SUBSYS_ID(p)      (p)->subdevice_id
#define CHIP_REVISION(p)  (p)->revision

#define IS_I810(pI810) (DEVICE_ID(pI810->PciInfo) == PCI_CHIP_I810 ||	\
			DEVICE_ID(pI810->PciInfo) == PCI_CHIP_I810_DC100 || \
			DEVICE_ID(pI810->PciInfo) == PCI_CHIP_I810_E)
#define IS_I815(pI810) (DEVICE_ID(pI810->PciInfo) == PCI_CHIP_I815)
#define IS_I830(pI810) (DEVICE_ID(pI810->PciInfo) == PCI_CHIP_I830_M)
#define IS_845G(pI810) (DEVICE_ID(pI810->PciInfo) == PCI_CHIP_845_G)
#define IS_I85X(pI810)  (DEVICE_ID(pI810->PciInfo) == PCI_CHIP_I855_GM)
#define IS_I852(pI810)  (DEVICE_ID(pI810->PciInfo) == PCI_CHIP_I855_GM && (pI810->variant == I852_GM || pI810->variant == I852_GME))
#define IS_I855(pI810)  (DEVICE_ID(pI810->PciInfo) == PCI_CHIP_I855_GM && (pI810->variant == I855_GM || pI810->variant == I855_GME))
#define IS_I865G(pI810) (DEVICE_ID(pI810->PciInfo) == PCI_CHIP_I865_G)

#define IS_I915G(pI810) (DEVICE_ID(pI810->PciInfo) == PCI_CHIP_I915_G || DEVICE_ID(pI810->PciInfo) == PCI_CHIP_E7221_G)
#define IS_I915GM(pI810) (DEVICE_ID(pI810->PciInfo) == PCI_CHIP_I915_GM)
#define IS_I945G(pI810) (DEVICE_ID(pI810->PciInfo) == PCI_CHIP_I945_G)
#define IS_I945GM(pI810) (DEVICE_ID(pI810->PciInfo) == PCI_CHIP_I945_GM || DEVICE_ID(pI810->PciInfo) == PCI_CHIP_I945_GME)
#define IS_IGDGM(pI810) (DEVICE_ID(pI810->PciInfo) == PCI_CHIP_IGD_GM)
#define IS_IGDG(pI810) (DEVICE_ID(pI810->PciInfo) == PCI_CHIP_IGD_G)
#define IS_IGD(pI810) (IS_IGDG(pI810) || IS_IGDGM(pI810))
#define IS_GM45(pI810) (DEVICE_ID(pI810->PciInfo) == PCI_CHIP_GM45_GM)
#define IS_G4X(pI810) (DEVICE_ID(pI810->PciInfo) == PCI_CHIP_IGD_E_G || DEVICE_ID(pI810->PciInfo) == PCI_CHIP_G45_G || DEVICE_ID(pI810->PciInfo) == PCI_CHIP_Q45_G || DEVICE_ID(pI810->PciInfo) == PCI_CHIP_G41_G || DEVICE_ID(pI810->PciInfo) == PCI_CHIP_B43_G || IS_GM45(pI810))
#define IS_I965GM(pI810) (DEVICE_ID(pI810->PciInfo) == PCI_CHIP_I965_GM || DEVICE_ID(pI810->PciInfo) == PCI_CHIP_I965_GME)
#define IS_965_Q(pI810) (DEVICE_ID(pI810->PciInfo) == PCI_CHIP_I965_Q)
#define IS_IGDNG_D(pI810) (DEVICE_ID(pI810->PciInfo) == PCI_CHIP_IGDNG_D_G)
#define IS_IGDNG_M(pI810) (DEVICE_ID(pI810->PciInfo) == PCI_CHIP_IGDNG_M_G)
#define IS_IGDNG(pI810) (IS_IGDNG_D(pI810) || IS_IGDNG_M(pI810))
#define IS_I965G(pI810) (DEVICE_ID(pI810->PciInfo) == PCI_CHIP_I965_G || DEVICE_ID(pI810->PciInfo) == PCI_CHIP_G35_G || DEVICE_ID(pI810->PciInfo) == PCI_CHIP_I965_Q || DEVICE_ID(pI810->PciInfo) == PCI_CHIP_I946_GZ || DEVICE_ID(pI810->PciInfo) == PCI_CHIP_I965_GM || DEVICE_ID(pI810->PciInfo) == PCI_CHIP_I965_GME || IS_G4X(pI810) || IS_IGDNG(pI810))
#define IS_G33CLASS(pI810) (DEVICE_ID(pI810->PciInfo) == PCI_CHIP_G33_G ||\
 			    DEVICE_ID(pI810->PciInfo) == PCI_CHIP_Q35_G ||\
			    DEVICE_ID(pI810->PciInfo) == PCI_CHIP_Q33_G || \
			    IS_IGD(pI810))
#define IS_I9XX(pI810) (IS_I915G(pI810) || IS_I915GM(pI810) || IS_I945G(pI810) || IS_I945GM(pI810) || IS_I965G(pI810) || IS_G33CLASS(pI810))
#define IS_I915(pI810) (IS_I915G(pI810) || IS_I915GM(pI810) || IS_I945G(pI810) || IS_I945GM(pI810) || IS_G33CLASS(pI810))

#define IS_MOBILE(pI810) (IS_I830(pI810) || IS_I85X(pI810) || IS_I915GM(pI810) || IS_I945GM(pI810) || IS_I965GM(pI810) || IS_GM45(pI810) || IS_IGD(pI810) || IS_IGDNG_M(pI810))
/* mark chipsets for using gfx VM offset for overlay */
#define OVERLAY_NOPHYSICAL(pI810) (IS_G33CLASS(pI810) || IS_I965G(pI810))
/* mark chipsets without overlay hw */
#define OVERLAY_NOEXIST(pI810) (IS_G4X(pI810) || IS_IGDNG(pI810))
/* chipsets require graphics mem for hardware status page */
#define HWS_NEED_GFX(pI810) (!pI810->use_drm_mode && \
			     (IS_G33CLASS(pI810) ||\
			      IS_G4X(pI810) || IS_IGDNG(pI810)))
/* chipsets require status page in non stolen memory */
#define HWS_NEED_NONSTOLEN(pI810) (IS_G4X(pI810) || IS_IGDNG(pI810))
#define SUPPORTS_INTEGRATED_HDMI(pI810) (IS_G4X(pI810) || IS_IGDNG(pI810))
/* dsparb controlled by hw only */
#define DSPARB_HWCONTROL(pI810) (IS_G4X(pI810) || IS_IGDNG(pI810))
/* supports Y tiled surfaces (pre-965 Mesa isn't ready yet) */
#define SUPPORTS_YTILING(pI810) (IS_I965G(intel))

#define GTT_PAGE_SIZE			KB(4)
#define ROUND_TO(x, y)			(((x) + (y) - 1) / (y) * (y))
#define ROUND_DOWN_TO(x, y)		((x) / (y) * (y))
#define ROUND_TO_PAGE(x)		ROUND_TO((x), GTT_PAGE_SIZE)
#define ROUND_TO_MB(x)			ROUND_TO((x), MB(1))
#define PRIMARY_RINGBUFFER_SIZE		KB(128)
#define MIN_SCRATCH_BUFFER_SIZE		KB(16)
#define MAX_SCRATCH_BUFFER_SIZE		KB(64)
#define HWCURSOR_SIZE			GTT_PAGE_SIZE
#define HWCURSOR_SIZE_ARGB		GTT_PAGE_SIZE * 4
#define OVERLAY_SIZE			GTT_PAGE_SIZE

/* Use a 64x64 HW cursor */
#define I810_CURSOR_X			64
#define I810_CURSOR_Y			I810_CURSOR_X

#define PIPE_NAME(n)			('A' + (n))

struct pci_device *
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

#endif /* _INTEL_COMMON_H_ */
