
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
} while (0)

#define OUTREG16(addr, val) do {					\
   *(volatile uint16_t *)(RecPtr->MMIOBase + (addr)) = (val);		\
} while (0)

#define OUTREG(addr, val) do {						\
   *(volatile uint32_t *)(RecPtr->MMIOBase + (addr)) = (val);		\
} while (0)


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

#define GTT_PAGE_SIZE			KB(4)
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
	INTEL_CREATE_PIXMAP_TILING_X	= 0x10000000,
	INTEL_CREATE_PIXMAP_TILING_Y	= 0x20000000,
	INTEL_CREATE_PIXMAP_TILING_NONE	= 0x40000000,
	INTEL_CREATE_PIXMAP_DRI2	= 0x80000000,
};

#endif /* _INTEL_COMMON_H_ */
