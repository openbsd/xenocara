
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
/* $XFree86: xc/programs/Xserver/hw/xfree86/drivers/i810/common.h,v 1.9 2003/09/24 02:43:23 dawes Exp $ */

/*
 * Authors:
 *   Keith Whitwell <keith@tungstengraphics.com>
 *   David Dawes <dawes@xfree86.org>
 *
 */

#ifndef _INTEL_COMMON_H_
#define _INTEL_COMMON_H_

/* Provide substitutes for gcc's __FUNCTION__ on other compilers */
#ifndef __GNUC__
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
#define DPRINTF I830DPRINTF_stub
#else /* #ifdef I830DEBUG */
#define MARKER()
/* this is a real ugly hack to get the compiler to optimize the debugging statements into oblivion */
#define DPRINTF if(0) I830DPRINTF_stub
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
extern void I830InitpScrn(ScrnInfoPtr pScrn);

/* Symbol lists shared by the i810 and i830 parts. */
extern int I830EntityIndex;
extern const char *I810vgahwSymbols[];
extern const char *I810ramdacSymbols[];
extern const char *I810int10Symbols[];
extern const char *I810vbeSymbols[];
extern const char *I810ddcSymbols[];
extern const char *I810fbSymbols[];
extern const char *I810xaaSymbols[];
extern const char *I810shadowFBSymbols[];
#ifdef XF86DRI
extern const char *I810driSymbols[];
extern const char *I810drmSymbols[];
#endif
extern const char *I810i2cSymbols[];

extern void I830DPRINTF_stub(const char *filename, int line,
			     const char *function, const char *fmt, ...);

#ifdef _I830_H_
#define PrintErrorState i830_dump_error_state
#define WaitRingFunc I830WaitLpRing
#define RecPtr pI830
#else
#define PrintErrorState I810PrintErrorState
#define WaitRingFunc I810WaitLpRing
#define RecPtr pI810
#endif

/* BIOS debug macro */
#define xf86ExecX86int10_wrapper(pInt, pScrn) do {			\
   ErrorF("Executing (ax == 0x%x) BIOS call at %s:%d\n", pInt->ax, __FILE__, __LINE__);	\
   if (I810_DEBUG & DEBUG_VERBOSE_BIOS) {				\
      ErrorF("Checking Error state before execution\n");		\
      PrintErrorState(pScrn);						\
   }									\
   xf86ExecX86int10(pInt);						\
   if(I810_DEBUG & DEBUG_VERBOSE_BIOS) {				\
      ErrorF("Checking Error state after execution\n");			\
      usleep(50000);							\
      PrintErrorState(pScrn);						\
   }									\
} while (0)

#define OUT_RING(n) do {						\
   if (I810_DEBUG & DEBUG_VERBOSE_RING)					\
      ErrorF( "OUT_RING %lx: %x, (mask %x)\n",				\
		(unsigned long)(outring), (unsigned int)(n), ringmask);	\
   *(volatile unsigned int *)(virt + outring) = n;			\
   outring += 4; ringused += 4;							\
   outring &= ringmask;							\
} while (0)

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

/** Copies a given number of bytes to the ring */
#define OUT_RING_COPY(n, ptr) do {					\
    if (I810_DEBUG & DEBUG_VERBOSE_RING)				\
	ErrorF("OUT_RING_DATA %d bytes\n", n);				\
    memcpy_volatile(virt + outring, ptr, n);				\
    outring += n;							\
    ringused += n;							\
    outring &= ringmask;						\
} while (0)

/** Pads the ring with a given number of zero bytes */
#define OUT_RING_PAD(n) do {						\
    if (I810_DEBUG & DEBUG_VERBOSE_RING)				\
	ErrorF("OUT_RING_PAD %d bytes\n", n);				\
    memset_volatile(virt + outring, 0, n);				\
    outring += n;							\
    ringused += n;							\
    outring &= ringmask;						\
} while (0)

union intfloat {
	float f;
	unsigned int ui;
};

#define OUT_RING_F(x) do {			\
	union intfloat tmp;			\
	tmp.f = (float)(x);			\
	OUT_RING(tmp.ui);			\
} while(0)				

#define ADVANCE_LP_RING() do {						\
   if (ringused > needed)          \
      FatalError("%s: ADVANCE_LP_RING: exceeded allocation %d/%d\n ",	\
	     __FUNCTION__, ringused, needed);   			\
   else if (ringused < needed)						\
      FatalError("%s: ADVANCE_LP_RING: under-used allocation %d/%d\n ",	\
	     __FUNCTION__, ringused, needed);   			\
   RecPtr->LpRing->tail = outring;					\
   RecPtr->LpRing->space -= ringused;					\
   if (outring & 0x07)							\
      FatalError("%s: ADVANCE_LP_RING: "					\
	     "outring (0x%x) isn't on a QWord boundary\n",		\
	     __FUNCTION__, outring);					\
   OUTREG(LP_RING + RING_TAIL, outring);				\
} while (0)

/*
 * XXX Note: the head/tail masks are different for 810 and i830.
 * If the i810 always sets the higher bits to 0, then this shouldn't be
 * a problem.  Check this!
 */
#define DO_RING_IDLE() do {						\
   int _head;								\
   int _tail;								\
   do {									\
      _head = INREG(LP_RING + RING_HEAD) & I830_HEAD_MASK;		\
      _tail = INREG(LP_RING + RING_TAIL) & I830_TAIL_MASK;		\
      DELAY(10);							\
   } while (_head != _tail);						\
} while( 0)


#define BEGIN_LP_RING(n)						\
   unsigned int outring, ringmask, ringused = 0;			\
   volatile unsigned char *virt;					\
   int needed;								\
   if ((n) & 1)								\
      ErrorF("BEGIN_LP_RING called with odd argument: %d\n", n);	\
   if ((n) > 2 && (I810_DEBUG&DEBUG_ALWAYS_SYNC))			\
      DO_RING_IDLE();							\
   needed = (n) * 4;							\
   if (RecPtr->LpRing->space < needed)					\
      WaitRingFunc(pScrn, needed, 0);					\
   outring = RecPtr->LpRing->tail;					\
   ringmask = RecPtr->LpRing->tail_mask;				\
   virt = RecPtr->LpRing->virtual_start;				\
   if (I810_DEBUG & DEBUG_VERBOSE_RING)					\
      ErrorF( "BEGIN_LP_RING %d in %s\n", n, FUNCTION_NAME);



/* Memory mapped register access macros */
#define INREG8(addr)        *(volatile CARD8  *)(RecPtr->MMIOBase + (addr))
#define INREG16(addr)       *(volatile CARD16 *)(RecPtr->MMIOBase + (addr))
#define INREG(addr)         *(volatile CARD32 *)(RecPtr->MMIOBase + (addr))
#define INGTT(addr)         *(volatile CARD32 *)(RecPtr->GTTBase + (addr))
#define POSTING_READ(addr)  (void)INREG(addr)

#define OUTREG8(addr, val) do {						\
   *(volatile CARD8 *)(RecPtr->MMIOBase  + (addr)) = (val);		\
   if (I810_DEBUG&DEBUG_VERBOSE_OUTREG) {				\
      ErrorF("OUTREG8(0x%lx, 0x%lx) in %s\n", (unsigned long)(addr),	\
		(unsigned long)(val), FUNCTION_NAME);			\
   }									\
} while (0)

#define OUTREG16(addr, val) do {					\
   *(volatile CARD16 *)(RecPtr->MMIOBase + (addr)) = (val);		\
   if (I810_DEBUG&DEBUG_VERBOSE_OUTREG) {				\
      ErrorF("OUTREG16(0x%lx, 0x%lx) in %s\n", (unsigned long)(addr),	\
		(unsigned long)(val), FUNCTION_NAME);			\
   }									\
} while (0)

#define OUTREG(addr, val) do {						\
   *(volatile CARD32 *)(RecPtr->MMIOBase + (addr)) = (val);		\
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

#ifndef PCI_CHIP_I965_G_1
#define PCI_CHIP_I965_G_1		0x2982
#define PCI_CHIP_I965_G_1_BRIDGE 	0x2980
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

#if XSERVER_LIBPCIACCESS
#define I810_MEMBASE(p,n) (p)->regions[(n)].base_addr
#define VENDOR_ID(p)      (p)->vendor_id
#define DEVICE_ID(p)      (p)->device_id
#define SUBVENDOR_ID(p)	  (p)->subvendor_id
#define SUBSYS_ID(p)      (p)->subdevice_id
#define CHIP_REVISION(p)  (p)->revision
#else
#define I810_MEMBASE(p,n) (p)->memBase[n]
#define VENDOR_ID(p)      (p)->vendor
#define DEVICE_ID(p)      (p)->chipType
#define SUBVENDOR_ID(p)	  (p)->subsysVendor
#define SUBSYS_ID(p)      (p)->subsysCard
#define CHIP_REVISION(p)  (p)->chipRev
#endif

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
#define IS_I965GM(pI810) (DEVICE_ID(pI810->PciInfo) == PCI_CHIP_I965_GM || DEVICE_ID(pI810->PciInfo) == PCI_CHIP_I965_GME)
#define IS_I965G(pI810) (DEVICE_ID(pI810->PciInfo) == PCI_CHIP_I965_G || DEVICE_ID(pI810->PciInfo) == PCI_CHIP_I965_G_1 || DEVICE_ID(pI810->PciInfo) == PCI_CHIP_I965_Q || DEVICE_ID(pI810->PciInfo) == PCI_CHIP_I946_GZ || DEVICE_ID(pI810->PciInfo) == PCI_CHIP_I965_GM || DEVICE_ID(pI810->PciInfo) == PCI_CHIP_I965_GME)
#define IS_G33CLASS(pI810) (DEVICE_ID(pI810->PciInfo) == PCI_CHIP_G33_G ||\
 			    DEVICE_ID(pI810->PciInfo) == PCI_CHIP_Q35_G ||\
 			    DEVICE_ID(pI810->PciInfo) == PCI_CHIP_Q33_G)
#define IS_I9XX(pI810) (IS_I915G(pI810) || IS_I915GM(pI810) || IS_I945G(pI810) || IS_I945GM(pI810) || IS_I965G(pI810) || IS_G33CLASS(pI810))

#define IS_MOBILE(pI810) (IS_I830(pI810) || IS_I85X(pI810) || IS_I915GM(pI810) || IS_I945GM(pI810) || IS_I965GM(pI810))
/* mark chipsets for using gfx VM offset for overlay */
#define OVERLAY_NOPHYSICAL(pI810) (IS_G33CLASS(pI810))

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

#if XSERVER_LIBPCIACCESS
struct pci_device *
intel_host_bridge (void);
#endif
   
#endif /* _INTEL_COMMON_H_ */
