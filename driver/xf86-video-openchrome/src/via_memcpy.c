/*
 * Copyright (C) 2004 Thomas Hellström, All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sub license,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial portions
 * of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "via_driver.h"
#include "compiler.h"


#define BSIZ 2048  /* size of /proc/cpuinfo buffer */
#define BSIZW 720  /* typical copy width (YUV420) */
#define BSIZA 736  /* multiple of 32 bytes */
#define BSIZH 576  /* typical copy height */

#define SSE_PREFETCH "  prefetchnta "
#define FENCE __asm__ __volatile__ ("sfence":::"memory");
#define FENCEMMS __asm__ __volatile__ ("\t"		\
				       "sfence\n\t"	\
				       "emms\n\t"	\
				       :::"memory");
#define FEMMS __asm__ __volatile__("femms":::"memory");
#define EMMS __asm__ __volatile__("emms":::"memory");

#define NOW_PREFETCH "  prefetch "


#define PREFETCH1(arch_prefetch,from)			\
    __asm__ __volatile__ (				\
			  "1:  " arch_prefetch "(%0)\n"	\
			  arch_prefetch "32(%0)\n"	\
			  arch_prefetch "64(%0)\n"	\
			  arch_prefetch "96(%0)\n"	\
			  arch_prefetch "128(%0)\n"	\
			  arch_prefetch "160(%0)\n"	\
			  arch_prefetch "192(%0)\n"	\
			  arch_prefetch "256(%0)\n"	\
			  arch_prefetch "288(%0)\n"	\
			  "2:\n"			\
			  : : "r" (from) );

#define PREFETCH2(arch_prefetch,from)			\
    __asm__ __volatile__ (				\
			  arch_prefetch "320(%0)\n"	\
			  : : "r" (from) );
#define PREFETCH3(arch_prefetch,from)			\
    __asm__ __volatile__ (				\
			  arch_prefetch "288(%0)\n"	\
			  : : "r" (from) );


#define small_memcpy(to, from, n)					\
    {									\
	__asm__ __volatile__(						\
			     "movl %2,%%ecx\n\t"			\
			     "sarl $2,%%ecx\n\t"			\
			     "rep ; movsl\n\t"				\
			     "testb $2,%b2\n\t"				\
			     "je 1f\n\t"				\
			     "movsw\n"					\
			     "1:\ttestb $1,%b2\n\t"			\
			     "je 2f\n\t"				\
			     "movsb\n"					\
			     "2:"					\
			     :"=&D" (to), "=&S" (from)			\
			     :"q" (n),"0" ((long) to),"1" ((long) from) \
			     : "%ecx","memory");			\
    }


#define SSE_CPY(prefetch, from, to, dummy, lcnt)			\
    if ((unsigned long) from & 15) {					\
	__asm__ __volatile__ (						\
			      "1:\n"					\
			      prefetch "320(%1)\n"			\
			      "  movups (%1), %%xmm0\n"			\
			      "  movups 16(%1), %%xmm1\n"		\
			      "  movntps %%xmm0, (%0)\n"		\
			      "  movntps %%xmm1, 16(%0)\n"		\
                              prefetch "352(%1)\n"			\
			      "  movups 32(%1), %%xmm2\n"		\
			      "  movups 48(%1), %%xmm3\n"		\
			      "  movntps %%xmm2, 32(%0)\n"		\
			      "  movntps %%xmm3, 48(%0)\n"		\
			      "  addl $64,%0\n"				\
			      "  addl $64,%1\n"				\
			      "  decl %2\n"				\
			      "  jne 1b\n"				\
			      :"=&D"(to), "=&S"(from), "=&r"(dummy)	\
			      :"0" (to), "1" (from), "2" (lcnt): "memory"); \
    } else {								\
	__asm__ __volatile__ (						\
			      "2:\n"					\
			      prefetch "320(%1)\n"			\
			      "  movaps (%1), %%xmm0\n"			\
			      "  movaps 16(%1), %%xmm1\n"		\
			      "  movntps %%xmm0, (%0)\n"		\
			      "  movntps %%xmm1, 16(%0)\n"		\
			      prefetch "352(%1)\n"			\
			      "  movaps 32(%1), %%xmm2\n"		\
			      "  movaps 48(%1), %%xmm3\n"		\
			      "  movntps %%xmm2, 32(%0)\n"		\
			      "  movntps %%xmm3, 48(%0)\n"		\
			      "  addl $64,%0\n"				\
			      "  addl $64,%1\n"				\
			      "  decl %2\n"				\
			      "  jne 2b\n"				\
			      :"=&D"(to), "=&S"(from), "=&r"(dummy)	\
			      :"0" (to), "1" (from), "2" (lcnt): "memory"); \
    }

#define MMX_CPY(prefetch, from, to, dummy, lcnt)			\
    __asm__ __volatile__ (						\
			  "1:\n"					\
			  prefetch "320(%1)\n"				\
			  "2:  movq (%1), %%mm0\n"			\
			  "  movq 8(%1), %%mm1\n"			\
			  "  movq 16(%1), %%mm2\n"			\
			  "  movq 24(%1), %%mm3\n"			\
			  "  movq %%mm0, (%0)\n"			\
			  "  movq %%mm1, 8(%0)\n"			\
			  "  movq %%mm2, 16(%0)\n"			\
			  "  movq %%mm3, 24(%0)\n"			\
			  prefetch "352(%1)\n"				\
			  "  movq 32(%1), %%mm0\n"			\
			  "  movq 40(%1), %%mm1\n"			\
			  "  movq 48(%1), %%mm2\n"			\
			  "  movq 56(%1), %%mm3\n"			\
			  "  movq %%mm0, 32(%0)\n"			\
			  "  movq %%mm1, 40(%0)\n"			\
			  "  movq %%mm2, 48(%0)\n"			\
			  "  movq %%mm3, 56(%0)\n"			\
			  "  addl $64,%0\n"				\
			  "  addl $64,%1\n"				\
			  "  decl %2\n"					\
			  "  jne 1b\n"					\
			  :"=&D"(to), "=&S"(from), "=&r"(dummy)		\
			  :"0" (to), "1" (from), "2" (lcnt) : "memory");

#define MMXEXT_CPY(prefetch, from, to, dummy, lcnt)			\
    __asm__ __volatile__ (						\
			  ".p2align 4,,7\n"				\
			  "1:\n"					\
			  prefetch "320(%1)\n"				\
			  "  movq (%1), %%mm0\n"			\
			  "  movq 8(%1), %%mm1\n"			\
			  "  movq 16(%1), %%mm2\n"			\
			  "  movq 24(%1), %%mm3\n"			\
			  "  movntq %%mm0, (%0)\n"			\
			  "  movntq %%mm1, 8(%0)\n"			\
			  "  movntq %%mm2, 16(%0)\n"			\
			  "  movntq %%mm3, 24(%0)\n"			\
			  prefetch "352(%1)\n"				\
			  "  movq 32(%1), %%mm0\n"			\
			  "  movq 40(%1), %%mm1\n"			\
			  "  movq 48(%1), %%mm2\n"			\
			  "  movq 56(%1), %%mm3\n"			\
			  "  movntq %%mm0, 32(%0)\n"			\
			  "  movntq %%mm1, 40(%0)\n"			\
			  "  movntq %%mm2, 48(%0)\n"			\
			  "  movntq %%mm3, 56(%0)\n"			\
			  "  addl $64,%0\n"				\
			  "  addl $64,%1\n"				\
			  "  decl %2\n"					\
			  "  jne 1b\n"					\
			  :"=&D"(to), "=&S"(from), "=&r"(dummy)		\
			  :"0" (to), "1" (from), "2" (lcnt) : "memory");


#define PREFETCH_FUNC(prefix, itype, ptype, begin, fence)		\
									\
    static void prefix##_YUV42X(unsigned char *to,			\
				const unsigned char *from,		\
				int dstPitch,				\
				int w,					\
				int h,					\
				int yuv422)				\
    {									\
	int dadd, rest, count, hc, lcnt;				\
	register int dummy;						\
	PREFETCH1(ptype##_PREFETCH, from);				\
	begin;								\
	count = 2;							\
									\
	/* If destination pitch equals width, do it all in one go. */	\
									\
	if (yuv422) {							\
	    w <<= 1;							\
	    if (w == dstPitch) {					\
		w *= h;							\
		h = 1;							\
		dstPitch = w;						\
		count = 0;						\
	    } else {							\
		h -= 1;							\
		count = 1;						\
	    }								\
	} else if (w == dstPitch) {					\
	    w = h*(w + (w >> 1));					\
	    count = 0;							\
	    h = 1;							\
	    dstPitch = w;						\
	}								\
									\
	lcnt = w >> 6;							\
	rest = w & 63;							\
	while (count--) {						\
	    hc = h;							\
	    lcnt = w >> 6;						\
	    rest = w & 63;						\
	    dadd = dstPitch - w;					\
	    while (hc--) {						\
		if (lcnt) {						\
		    itype##_CPY(ptype##_PREFETCH, from, to, dummy, lcnt); \
		}							\
		if (rest) {						\
		    PREFETCH2(ptype##_PREFETCH, from);			\
		    small_memcpy(to, from, rest);			\
		    PREFETCH3(ptype##_PREFETCH, from);			\
		}							\
		to += dadd;						\
	    }								\
	    w >>= 1;							\
	    dstPitch >>= 1;						\
	    h -= 1;							\
	}								\
	if (lcnt > 5) {							\
	    lcnt -= 5;							\
	    itype##_CPY(ptype##_PREFETCH, from, to, dummy, lcnt);	\
	    lcnt = 5;							\
	}								\
	if (lcnt) {							\
	    itype##_CPY("#", from, to, dummy, lcnt);			\
	}								\
	if (rest) small_memcpy(to, from, rest);				\
	fence;								\
    }

#define NOPREFETCH_FUNC(prefix, itype, begin, fence)			\
    static void prefix##_YUV42X(unsigned char *to,			\
				const unsigned char *from,		\
				int dstPitch,				\
				int w,					\
				int h,					\
				int yuv422)				\
									\
    {									\
	int dadd, rest, count, hc, lcnt;				\
	register int dummy;						\
	begin;								\
	count = 2;							\
									\
	/* If destination pitch equals width, do it all in one go. */	\
									\
	if (yuv422) {							\
	    w <<= 1;							\
	    count = 1;							\
	    if (w == dstPitch) {					\
		w *= h;							\
		h = 1;							\
		dstPitch = w;						\
	    }								\
	} else if (w == dstPitch) {					\
	    w = h*(w + (w >> 1));					\
	    count = 1;							\
	    h = 1;							\
	    dstPitch = w;						\
	}								\
									\
	lcnt = w >> 6;							\
	rest = w & 63;							\
	while (count--) {						\
	    hc = h;							\
	    dadd = dstPitch - w;					\
	    lcnt = w >> 6;						\
	    rest = w & 63;						\
	    while (hc--) {						\
		if (lcnt) {						\
		    itype##_CPY("#", from, to, dummy, lcnt);		\
		}							\
		if (rest) small_memcpy(to, from, rest);			\
		to += dadd;						\
	    }								\
	    w >>= 1;							\
	    dstPitch >>= 1;						\
	}								\
	fence;								\
    }


static void
libc_YUV42X(unsigned char *dst, const unsigned char *src,
            int dstPitch, int w, int h, int yuv422)
{
    if (yuv422)
        w <<= 1;
    if (dstPitch == w) {
        int size = h * ((yuv422) ? w : (w + (w >> 1)));

        memcpy(dst, src, size);
        return;
    } else {
        int count;

        /* Copy Y component to video memory. */
        count = h;
        while (count--) {
            memcpy(dst, src, w);
            src += w;
            dst += dstPitch;
        }

        /* UV component is 1/2 of Y. */
        if (!yuv422) {
            w >>= 1;
            dstPitch >>= 1;

            /* Copy V(Cr),U(Cb) components to video memory. */
            count = h;
            while (count--) {
                memcpy(dst, src, w);
                src += w;
                dst += dstPitch;
            }
        }
    }
}

#ifdef __i386__

/* Linux kernel __memcpy. */
static __inline void *
__memcpy(void *to, const void *from, size_t n)
{
    int d1, d2, d3;

    __asm__ __volatile__(
                         "rep ; movsl\n\t"
                         "testb $2,%b4\n\t"
                         "je 1f\n\t"
                         "movsw\n"
                         "1:\ttestb $1,%b4\n\t"
                         "je 2f\n\t"
                         "movsb\n"
                         "2:"
                         :"=&c"(d1), "=&D"(d2), "=&S"(d3)
                         :"0"(n >> 2), "q"(n), "1"((long)to), "2"((long)from)
                         :"memory");

    return (to);
}


static void
kernel_YUV42X(unsigned char *dst, const unsigned char *src,
              int dstPitch, int w, int h, int yuv422)
{
    if (yuv422)
        w <<= 1;
    if (dstPitch == w) {
        int size = h * ((yuv422) ? w : (w + (w >> 1)));

        __memcpy(dst, src, size);
        return;
    } else {
        int count;

        /* Copy Y component to video memory. */
        count = h;
        while (count--) {
            __memcpy(dst, src, w);
            src += w;
            dst += dstPitch;
        }

        /* UV component is 1/2 of Y. */
        if (!yuv422) {

            w >>= 1;
            dstPitch >>= 1;

            /* Copy V(Cr),U(Cb) components to video memory. */
            count = h;
            while (count--) {
                __memcpy(dst, src, w);
                src += w;
                dst += dstPitch;
            }
        }
    }
}

PREFETCH_FUNC(sse, SSE, SSE,, FENCE)
PREFETCH_FUNC(mmxext, MMXEXT, SSE, EMMS, FENCEMMS)
PREFETCH_FUNC(now, MMX, NOW, FEMMS, FEMMS)
NOPREFETCH_FUNC(mmx, MMX, EMMS, EMMS)

static void
*kernel_memcpy(void *to, const void *from, size_t len)
{
    return __memcpy(to, from, len);
}

static unsigned
fastrdtsc(void)
{
    unsigned eax;

    __asm__ volatile ("\t"
                      "pushl %%ebx\n\t"
                      "cpuid\n\t"
                      ".byte 0x0f, 0x31\n\t"
                      "popl %%ebx\n"
                      :"=a" (eax)
                      :"0"(0)
                      :"ecx", "edx", "cc");

    return eax;
}


static unsigned
time_function(vidCopyFunc mf, unsigned char *buf1, unsigned char *buf2)
{
    unsigned t, t2;

    t = fastrdtsc();

    (*mf) (buf1, buf2, BSIZA, BSIZW, BSIZH, 0);

    t2 = fastrdtsc();
    return ((t < t2) ? t2 - t : 0xFFFFFFFFU - (t - t2 - 1));
}

enum
{ libc = 0, kernel, sse, mmx, now, mmxext, totNum };

typedef struct
{
    vidCopyFunc mFunc;
    const char *mName, **cpuFlag;
} McFuncData;

static const char *libc_cpuflags[] = { " ", 0 };
static const char *kernel_cpuflags[] = { " ", 0 };
static const char *sse_cpuflags[] = { " sse ", 0 };
static const char *mmx_cpuflags[] = { " mmx ", 0 };
static const char *now_cpuflags[] = { " 3dnow ", 0 };
static const char *mmx2_cpuflags[] = { " mmxext ", " sse ", 0 };

static McFuncData mcFunctions[totNum] = {
{libc_YUV42X, "libc", libc_cpuflags},
{kernel_YUV42X, "kernel", kernel_cpuflags},
{sse_YUV42X, "SSE", sse_cpuflags},
{mmx_YUV42X, "MMX", mmx_cpuflags},
{now_YUV42X, "3DNow!", now_cpuflags},
{mmxext_YUV42X, "MMX2", mmx2_cpuflags}
};


static int
flagValid(const char *cpuinfo, const char *flag)
{
    const char *flagLoc, *nextProc;
    int located = 0;

    while ((cpuinfo = strstr(cpuinfo, "processor\t:"))) {
        located = 1;
        cpuinfo += 11;
        if ((flagLoc = strstr(cpuinfo, flag))) {
            if ((nextProc = strstr(cpuinfo, "processor\t:"))) {
                if (nextProc < flagLoc)
                    return 0;
            }
        } else {
            return 0;
        }
    }
    return located;
}


static int
cpuValid(const char *cpuinfo, const char **flags)
{
    for (; *flags != 0; flags++) {
        if (flagValid(cpuinfo, *flags))
            return 1;
    }
    return 0;
}

/*
 * Benchmark the video copy routines and choose the fastest.
 */
vidCopyFunc
viaVidCopyInit(const char *copyType, ScreenPtr pScreen)
{
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);

    char buf[BSIZ];
    unsigned char *buf1, *buf2, *buf3;
    char *tmpBuf, *endBuf;
    int count, j, bestSoFar;
    unsigned best, tmp, testSize, alignSize, tmp2;
    struct buffer_object *tmpFbBuffer;
    McFuncData *curData;
    FILE *cpuInfoFile;
    double cpuFreq;

    if (NULL == (cpuInfoFile = fopen("/proc/cpuinfo", "r"))) {
        return libc_YUV42X;
    }
    count = fread(buf, 1, BSIZ, cpuInfoFile);
    if (ferror(cpuInfoFile)) {
        fclose(cpuInfoFile);
        return libc_YUV42X;
    }
    fclose(cpuInfoFile);
    if (BSIZ == count) {
        xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
                   "\"/proc/cpuinfo\" file too long. "
                   "Using Linux kernel memcpy.\n");
        return libc_YUV42X;
    }
    buf[count] = 0;

    while (count--)
        if ('\n' == buf[count])
            buf[count] = ' ';

    /* Extract the CPU frequency. */
    cpuFreq = 0.;
    if (NULL != (tmpBuf = strstr(buf, "cpu MHz"))) {
        if (NULL != (tmpBuf = strstr(tmpBuf, ":") + 1)) {
            cpuFreq = strtod(tmpBuf, &endBuf);
            if (endBuf == tmpBuf)
                tmpBuf = NULL;
        }
    }

    alignSize = BSIZH * (BSIZA + (BSIZA >> 1));
    testSize = BSIZH * (BSIZW + (BSIZW >> 1));
    /*
     * Allocate an area of offscreen FB memory, (buf1), a simulated video
     * player buffer (buf2) and a pool of uninitialized "video" data (buf3).
     */
    tmpFbBuffer = drm_bo_alloc(pScrn, alignSize, 32, TTM_PL_VRAM);
    if (!tmpFbBuffer)
        return libc_YUV42X;
    if (NULL == (buf2 = (unsigned char *)malloc(testSize))) {
        drm_bo_free(pScrn, tmpFbBuffer);
        return libc_YUV42X;
    }
    if (NULL == (buf3 = (unsigned char *)malloc(testSize))) {
        free(buf2);
        drm_bo_free(pScrn, tmpFbBuffer);
        return libc_YUV42X;
    }
    buf1 = drm_bo_map(pScrn, tmpFbBuffer);
    bestSoFar = 0;
    best = 0xFFFFFFFFU;

    /* Make probable that buf1 and buf2 are in memory by referencing them. */
    libc_YUV42X(buf1, buf2, BSIZA, BSIZW, BSIZH, 0);

    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
               "Benchmarking %s copy.  Less time is better.\n", copyType);
    for (j = 0; j < totNum; ++j) {
        curData = mcFunctions + j;

        if (cpuValid(buf, curData->cpuFlag)) {

            /* Simulate setup of the video buffer. */
            kernel_memcpy(buf2, buf3, testSize);

            /* Copy the video buffer to frame-buffer memory. */
            tmp = time_function(curData->mFunc, buf1, buf2);

            /* Do it again to avoid context-switch effects. */
            kernel_memcpy(buf2, buf3, testSize);
            tmp2 = time_function(curData->mFunc, buf1, buf2);
            tmp = (tmp2 < tmp) ? tmp2 : tmp;

            if (NULL == tmpBuf) {
                xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
                           "Timed %6s YUV420 copy... %u.\n",
                           curData->mName, tmp);
            } else {
                xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
                           "Timed %6s YUV420 copy... %u. "
                           "Throughput: %.1f MiB/s.\n",
                           curData->mName, tmp,
                           cpuFreq * 1.e6 * (double)testSize /
                           ((double)(tmp) * (double)(0x100000)));
            }
            if (tmp < best) {
                best = tmp;
                bestSoFar = j;
            }
        } else {
            xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
                       "Ditching %6s YUV420 copy. Not supported by CPU.\n",
                       curData->mName);
        }
    }
    free(buf3);
    free(buf2);
    drm_bo_unmap(pScrn, tmpFbBuffer);
    drm_bo_free(pScrn, tmpFbBuffer);
    xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
               "Using %s YUV42X copy for %s.\n",
               mcFunctions[bestSoFar].mName, copyType);
    return mcFunctions[bestSoFar].mFunc;
}

#else

vidCopyFunc
viaVidCopyInit(const char *copyType, ScreenPtr pScreen)
{
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);

    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
               "Using default xfree86 memcpy for video.\n");
    return libc_YUV42X;
}

#endif /* __i386__ */
