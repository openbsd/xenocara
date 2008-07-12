/*
 * Copyright 2000 ATI Technologies Inc., Markham, Ontario, and
 *                VA Linux Systems Inc., Fremont, California.
 *
 * All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation on the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial
 * portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NON-INFRINGEMENT.  IN NO EVENT SHALL ATI, VA LINUX SYSTEMS AND/OR
 * THEIR SUPPLIERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

/*
 * Authors:
 *   Kevin E. Martin <martin@xfree86.org>
 *   Rickard E. Faith <faith@valinux.com>
 *   Alan Hourihane <alanh@fairlite.demon.co.uk>
 *
 * References:
 *
 * !!!! FIXME !!!!
 *   RAGE 128 VR/ RAGE 128 GL Register Reference Manual (Technical
 *   Reference Manual P/N RRG-G04100-C Rev. 0.04), ATI Technologies: April
 *   1999.
 *
 * !!!! FIXME !!!!
 *   RAGE 128 Software Development Manual (Technical Reference Manual P/N
 *   SDK-G04000 Rev. 0.01), ATI Technologies: June 1999.
 *
 */


#ifndef _RADEON_MACROS_H_
#define _RADEON_MACROS_H_

#include "compiler.h"

#if HAVE_BYTESWAP_H
#include <byteswap.h>
#elif defined(USE_SYS_ENDIAN_H)
#include <sys/endian.h>
#else
#define	bswap_16(value)  \
 	((((value) & 0xff) << 8) | ((value) >> 8))

#define	bswap_32(value)	\
 	(((uint32_t)bswap_16((uint16_t)((value) & 0xffff)) << 16) | \
 	(uint32_t)bswap_16((uint16_t)((value) >> 16)))
 
#define	bswap_64(value)	\
 	(((uint64_t)bswap_32((uint32_t)((value) & 0xffffffff)) \
 	    << 32) | \
 	(uint64_t)bswap_32((uint32_t)((value) >> 32)))
#endif

#if X_BYTE_ORDER == X_BIG_ENDIAN
#define le32_to_cpu(x) bswap_32(x)
#define le16_to_cpu(x) bswap_16(x)
#else
#define le32_to_cpu(x) (x)
#define le16_to_cpu(x) (x)
#endif

#define RADEON_BIOS8(v)  (info->VBIOS[v])
#define RADEON_BIOS16(v) (info->VBIOS[v] | \
                          (info->VBIOS[(v) + 1] << 8))
#define RADEON_BIOS32(v) (info->VBIOS[v] | \
                          (info->VBIOS[(v) + 1] << 8) | \
                          (info->VBIOS[(v) + 2] << 16) | \
                          (info->VBIOS[(v) + 3] << 24))

				/* Memory mapped register access macros */
#define INREG8(addr)        MMIO_IN8(RADEONMMIO, addr)
#define INREG16(addr)       MMIO_IN16(RADEONMMIO, addr)
#define INREG(addr)         MMIO_IN32(RADEONMMIO, addr)
#define OUTREG8(addr, val)  MMIO_OUT8(RADEONMMIO, addr, val)
#define OUTREG16(addr, val) MMIO_OUT16(RADEONMMIO, addr, val)
#define OUTREG(addr, val)   MMIO_OUT32(RADEONMMIO, addr, val)

#define ADDRREG(addr)       ((volatile uint32_t *)(pointer)(RADEONMMIO + (addr)))


#define OUTREGP(addr, val, mask)					\
do {									\
    uint32_t tmp = INREG(addr);						\
    tmp &= (mask);							\
    tmp |= ((val) & ~(mask));						\
    OUTREG(addr, tmp);							\
} while (0)

#define INPLL(pScrn, addr) RADEONINPLL(pScrn, addr)

#define OUTPLL(pScrn, addr, val) RADEONOUTPLL(pScrn, addr, val)

#define OUTPLLP(pScrn, addr, val, mask)					\
do {									\
    uint32_t tmp_ = INPLL(pScrn, addr);					\
    tmp_ &= (mask);							\
    tmp_ |= ((val) & ~(mask));						\
    OUTPLL(pScrn, addr, tmp_);						\
} while (0)

#define OUTPAL_START(idx)						\
do {									\
    if (IS_AVIVO_VARIANT) {                                             \
        OUTREG8(AVIVO_DC_LUT_RW_INDEX, (idx));				\
    } else {                                                            \
        OUTREG8(RADEON_PALETTE_INDEX, (idx));				\
    }								        \
} while (0)

#define OUTPAL_NEXT(r, g, b)						\
do {									\
    if (IS_AVIVO_VARIANT) {                                             \
        OUTREG(AVIVO_DC_LUT_30_COLOR, ((r) << 22) | ((g) << 12) | ((b) << 2));	\
    } else {                                                               \
        OUTREG(RADEON_PALETTE_DATA, ((r) << 16) | ((g) << 8) | (b));	\
    }								        \
} while (0)

#define OUTPAL_NEXT_uint32_t(v)						\
do {									\
    OUTREG(RADEON_PALETTE_DATA, (v & 0x00ffffff));			\
} while (0)

#define OUTPAL(idx, r, g, b)						\
do {									\
    OUTPAL_START((idx));						\
    OUTPAL_NEXT((r), (g), (b));						\
} while (0)

#define INPAL_START(idx)						\
do {									\
    if (IS_AVIVO_VARIANT) {                                             \
        OUTREG8(AVIVO_DC_LUT_RW_INDEX, (idx));				\
    } else {                                                            \
        OUTREG(RADEON_PALETTE_INDEX, (idx) << 16);			\
    }								        \
} while (0)

#define INPAL_NEXT()                                                    \
do {									\
    if (IS_AVIVO_VARIANT) {                                             \
        INREG(AVIVO_DC_LUT_30_COLOR);                                   \
    } else {                                                            \
        INREG(RADEON_PALETTE_DATA);                                     \
    }								        \
} while (0)

#define PAL_SELECT(idx)							\
do {									\
    if (IS_AVIVO_VARIANT) {                                             \
        if (!idx) {							\
	    OUTREG(AVIVO_DC_LUT_RW_SELECT, 0);                          \
        } else {						        \
	    OUTREG(AVIVO_DC_LUT_RW_SELECT, 1);                          \
        }								\
    } else {                                                            \
        if (!idx) {							\
	    OUTREG(RADEON_DAC_CNTL2, INREG(RADEON_DAC_CNTL2) &		\
	           (uint32_t)~RADEON_DAC2_PALETTE_ACC_CTL);		\
        } else {							\
	    OUTREG(RADEON_DAC_CNTL2, INREG(RADEON_DAC_CNTL2) |		\
	           RADEON_DAC2_PALETTE_ACC_CTL);			\
        }								\
    }								        \
} while (0)

#define INMC(pScrn, addr) RADEONINMC(pScrn, addr)

#define OUTMC(pScrn, addr, val) RADEONOUTMC(pScrn, addr, val)

#endif
