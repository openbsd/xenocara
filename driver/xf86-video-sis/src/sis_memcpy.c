/*
 * SiS memcpy() routines (assembly)
 *
 * Copyright (C) 2004-2005 Thomas Winischhofer
 *
 * Idea and some code bits from via_memcpy.c which is
 * Copyright (C) 2004 Thomas Hellstroem, All Rights Reserved.
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
 * THE CODE SUPPLIER(S) BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "sis.h"

#if 0			/* Debug */
#define SISDGBMC
#endif

extern unsigned int SISAllocateFBMemory(ScrnInfoPtr pScrn, void **handle, int bytesize);
extern void	    SISFreeFBMemory(ScrnInfoPtr pScrn, void **handle);

#define CPUBUFFERSIZE 2048       /* Size of /proc/cpuinfo buffer */
#define BUFFERSIZE (576 * 1152)  /* Matches 720x576 YUV420 */

/************************************************************************/
/*                   arch specific memcpy() routines                    */
/************************************************************************/

/* i386, AMD64 */

#define FENCE 			\
     __asm__ __volatile__( 	\
		  " sfence\n" 	\
		  :		\
		  :		\
		  : "memory");

#define FENCEMMS 		\
     __asm__ __volatile__ (	\
		  " sfence\n"	\
		  " emms\n"	\
		  :		\
		  :		\
		  : "memory");

#define FEMMS 			\
     __asm__ __volatile__(	\
		  " femms\n"	\
		  :		\
		  :		\
		  : "memory");

#define EMMS 			\
     __asm__ __volatile__(	\
		  " emms\n"	\
		  :		\
		  :		\
		  : "memory");

#define SSE_PREFETCH " prefetchnta "
#define NOW_PREFETCH " prefetch "

#define PREFETCH1(arch_prefetch,from)		\
    __asm__ __volatile__ (			\
		  arch_prefetch "(%0)\n"	\
		  arch_prefetch "32(%0)\n"	\
		  arch_prefetch "64(%0)\n"	\
		  arch_prefetch "96(%0)\n"	\
		  arch_prefetch "128(%0)\n"	\
		  arch_prefetch "160(%0)\n"	\
		  arch_prefetch "192(%0)\n"	\
		  arch_prefetch "256(%0)\n"	\
		  arch_prefetch "288(%0)\n"	\
		  : 				\
		  : "r" (from) );

#define PREFETCH2(arch_prefetch,from)		\
    __asm__ __volatile__ (			\
		  arch_prefetch "320(%0)\n"	\
		  : 				\
		  : "r" (from) );

#define PREFETCH3(arch_prefetch,from)		\
    __asm__ __volatile__ (			\
		  arch_prefetch "288(%0)\n"	\
		  : 				\
		  : "r" (from) );

#define small_memcpy_i386(to,from,n)					\
    {									\
	__asm__ __volatile__(						\
		  " cld\n"						\
		  " shrl $1, %%ecx\n"					\
		  " jnc 1f\n"						\
		  " movsb\n"						\
		"1: shrl $1, %%ecx\n"					\
		  " jnc 2f\n"						\
		  " movsw\n"						\
		"2: rep ; movsl"					\
		  : "=&D" (to), "=&S" (from)				\
		  : "c" (n), "0" ((long) to), "1" ((long) from) 	\
		  : "memory", "cc");					\
    }

#define small_memcpy_amd64(to,from,n)					\
    {									\
	__asm__ __volatile__(						\
		  " cld\n"						\
		  " shrq $1, %%rcx\n"					\
		  " jnc 1f\n"						\
		  " movsb\n"						\
		"1: shrq $1, %%rcx\n"					\
		  " jnc 2f\n"						\
		  " movsw\n"						\
		"2: shrq $1, %%rcx\n"					\
		  " jnc 3f\n"						\
		  " movsl\n"						\
		"3: rep ; movsq"					\
		  : "=&D" (to), "=&S" (from)				\
		  : "c" (n), "0" ((long) to), "1" ((long) from) 	\
		  : "memory", "cc");					\
    }

#define MMX_CPY(prefetch,from,to,dummy,lcnt)				\
    __asm__ __volatile__ (						\
	        "1:\n"							\
		    prefetch "320(%1)\n"				\
	          " movq (%1), %%mm0\n"					\
		  " movq 8(%1), %%mm1\n"				\
		  " movq 16(%1), %%mm2\n"				\
		  " movq 24(%1), %%mm3\n"				\
		  " movq %%mm0, (%0)\n"					\
		  " movq %%mm1, 8(%0)\n"				\
		  " movq %%mm2, 16(%0)\n"				\
		  " movq %%mm3, 24(%0)\n"				\
		    prefetch "352(%1)\n"				\
		  " movq 32(%1), %%mm0\n"				\
		  " movq 40(%1), %%mm1\n"				\
		  " movq 48(%1), %%mm2\n"				\
		  " movq 56(%1), %%mm3\n"				\
		  " leal 64(%1),%1\n"					\
		  " movq %%mm0, 32(%0)\n"				\
		  " movq %%mm1, 40(%0)\n"				\
		  " movq %%mm2, 48(%0)\n"				\
		  " movq %%mm3, 56(%0)\n"				\
		  " decl %2\n"						\
		  " leal 64(%0),%0\n"					\
		  " jne 1b\n"						\
		  : "=&D"(to), "=&S"(from), "=&r"(dummy)		\
		  : "0" (to), "1" (from), "2" (lcnt) 			\
		  : "memory", "cc");

#define SSE_CPY(prefetch,from,to,dummy,lcnt)				\
    if((ULong) from & 15) {						\
	__asm__ __volatile__ (						\
		"1:\n"							\
		    prefetch "320(%1)\n"				\
		  " movups (%1), %%xmm0\n"				\
		  " movups 16(%1), %%xmm1\n"				\
		  " movntps %%xmm0, (%0)\n"				\
		  " movntps %%xmm1, 16(%0)\n"				\
		    prefetch "352(%1)\n"				\
		  " movups 32(%1), %%xmm2\n"				\
		  " movups 48(%1), %%xmm3\n"				\
		  " leal 64(%1),%1\n"					\
		  " movntps %%xmm2, 32(%0)\n"				\
		  " movntps %%xmm3, 48(%0)\n"				\
		  " decl %2\n"						\
		  " leal 64(%0),%0\n"					\
		  " jne 1b\n"						\
		  : "=&D"(to), "=&S"(from), "=&r"(dummy)		\
		  : "0" (to), "1" (from), "2" (lcnt)			\
		  : "memory", "cc"); 					\
    } else {								\
	__asm__ __volatile__ (						\
		"2:\n"							\
		    prefetch "320(%1)\n"				\
		  " movaps (%1), %%xmm0\n"				\
		  " movaps 16(%1), %%xmm1\n"				\
		  " movntps %%xmm0, (%0)\n"				\
		  " movntps %%xmm1, 16(%0)\n"				\
        	    prefetch "352(%1)\n"				\
		  " movaps 32(%1), %%xmm2\n"				\
		  " movaps 48(%1), %%xmm3\n"				\
		  " leal 64(%1),%1\n"					\
		  " movntps %%xmm2, 32(%0)\n"				\
		  " movntps %%xmm3, 48(%0)\n"				\
		  " decl %2\n"						\
		  " leal 64(%0),%0\n"					\
		  " jne 2b\n"						\
		  : "=&D"(to), "=&S"(from), "=&r"(dummy)		\
		  : "0" (to), "1" (from), "2" (lcnt)			\
		  : "memory", "cc");					\
    }

#define SSE64_CPY(prefetch,from,to,dummy,lcnt)				\
    if((ULong) from & 15) {						\
	__asm__ __volatile__ (						\
		"1:\n"							\
		    prefetch "320(%1)\n"				\
		  " movups (%1), %%xmm0\n"				\
		  " movups 16(%1), %%xmm1\n"				\
		  " movntps %%xmm0, (%0)\n"				\
		  " movntps %%xmm1, 16(%0)\n"				\
		    prefetch "352(%1)\n"				\
		  " movups 32(%1), %%xmm2\n"				\
		  " movups 48(%1), %%xmm3\n"				\
		  " leaq 64(%1),%1\n"					\
		  " movntps %%xmm2, 32(%0)\n"				\
		  " movntps %%xmm3, 48(%0)\n"				\
		  " decl %2\n"						\
		  " leaq 64(%0),%0\n"					\
		  " jne 1b\n"						\
		  : "=&D"(to), "=&S"(from), "=&r"(dummy)		\
		  : "0" (to), "1" (from), "2" (lcnt)			\
		  : "memory", "cc"); 					\
    } else {								\
	__asm__ __volatile__ (						\
		"2:\n"							\
		    prefetch "320(%1)\n"				\
		  " movaps (%1), %%xmm0\n"				\
		  " movaps 16(%1), %%xmm1\n"				\
		  " movntps %%xmm0, (%0)\n"				\
		  " movntps %%xmm1, 16(%0)\n"				\
        	    prefetch "352(%1)\n"				\
		  " movaps 32(%1), %%xmm2\n"				\
		  " movaps 48(%1), %%xmm3\n"				\
		  " leaq 64(%1),%1\n"					\
		  " movntps %%xmm2, 32(%0)\n"				\
		  " movntps %%xmm3, 48(%0)\n"				\
		  " decl %2\n"						\
		  " leaq 64(%0),%0\n"					\
		  " jne 2b\n"						\
		  : "=&D"(to), "=&S"(from), "=&r"(dummy)		\
		  : "0" (to), "1" (from), "2" (lcnt)			\
		  : "memory", "cc");					\
    }

#define MMXEXT_CPY(prefetch,from,to,dummy,lcnt)				\
    __asm__ __volatile__ (						\
		  ".p2align 4,,7\n"					\
		 "1:\n"							\
		    prefetch "320(%1)\n"				\
		  " movq (%1), %%mm0\n"					\
		  " movq 8(%1), %%mm1\n"				\
		  " movq 16(%1), %%mm2\n"				\
		  " movq 24(%1), %%mm3\n"				\
		  " movntq %%mm0, (%0)\n"				\
		  " movntq %%mm1, 8(%0)\n"				\
		  " movntq %%mm2, 16(%0)\n"				\
		  " movntq %%mm3, 24(%0)\n"				\
		    prefetch "352(%1)\n"				\
		  " movq 32(%1), %%mm0\n"				\
		  " movq 40(%1), %%mm1\n"				\
		  " movq 48(%1), %%mm2\n"				\
		  " movq 56(%1), %%mm3\n"				\
		  " leal 64(%1),%1\n"					\
		  " movntq %%mm0, 32(%0)\n"				\
		  " movntq %%mm1, 40(%0)\n"				\
		  " movntq %%mm2, 48(%0)\n"				\
		  " movntq %%mm3, 56(%0)\n"				\
		  " decl %2\n"						\
		  " leal 64(%0),%0\n"					\
		  " jne 1b\n"						\
		  : "=&D"(to), "=&S"(from), "=&r"(dummy)		\
		  : "0" (to), "1" (from), "2" (lcnt) 			\
		  : "memory", "cc");


#define PREFETCH_FUNC(prefix,itype,ptype,begin,fence,small)		\
									\
    static void prefix##_memcpy(UChar *to,				\
				const UChar *from,			\
				int size)				\
    {									\
	int lcnt = size >> 6;						\
	int rest = size & 63;						\
	register int dummy;						\
									\
	PREFETCH1(ptype##_PREFETCH,from);				\
									\
	begin;								\
	if(lcnt) {							\
	   itype##_CPY(ptype##_PREFETCH,from,to,dummy,lcnt);		\
	}								\
	if(rest) {							\
	   PREFETCH2(ptype##_PREFETCH,from);				\
	   small(to, from, rest);					\
	   PREFETCH3(ptype##_PREFETCH,from);				\
	}								\
	fence;								\
    }

#define NOPREFETCH_FUNC(prefix,itype,begin,fence,small)			\
									\
    static void prefix##_memcpy(UChar *to,				\
				const UChar *from,			\
				int size)				\
    {									\
	int lcnt = size >> 6;						\
	int rest = size & 63;						\
	register int dummy;						\
									\
	begin;								\
	if(lcnt) {							\
	   itype##_CPY("#",from,to,dummy,lcnt);				\
	}								\
	if(rest) {							\
	   small(to, from, rest);					\
	}								\
	fence;								\
    }

/* Other archs */

/* ... */


/* Type for table for benchmark list */

typedef struct {
    vidCopyFunc  mFunc;
    char         *mName;
    unsigned int mycpuflag;
    int          grade;
    int 	 gradefrom;
    Bool         reqAlignment;
} SISMCFuncData;

/************************************************************************/
/*                   libc memcpy() wrapper - generic                    */
/************************************************************************/

static void SiS_libc_memcpy(UChar *dst, const UChar *src, int size)
{
    memcpy(dst, src, size);
}

/************************************************************************/
/* We only do all that stuff under gcc; no idea what other compilers 	*/
/* would do with our asm code.  					*/
/************************************************************************/

#ifndef __GNUC__

unsigned int SiSGetCPUFlags(ScrnInfoPtr pScrn)
{
    return 0;
}

vidCopyFunc SiSVidCopyInit(ScreenPtr pScreen, vidCopyFunc *UMemCpy, Bool from)
{
    *UMemCpy = SiS_libc_memcpy;
    return SiS_libc_memcpy;
}

vidCopyFunc SiSVidCopyGetDefault(void)
{
    return SiS_libc_memcpy;
}

#else /* ! Everything below is gcc specific ! */

/************************************************************************/
/*                    Definitions for archs and OSes                    */
/************************************************************************/

#undef SiS_checkosforsse
#undef SiS_canBenchmark
#undef SiS_haveProc
#undef SiS_haveBuiltInMC

#if defined(__i386__) /* ***************************************** i386 */

#define SiS_checkosforsse 	/* Does this cpu support sse and do we need to check os? */
#define SiS_canBenchmark	/* Can we perform a benchmark? */
#ifdef SIS_LINUX
#define SiS_haveProc		/* Do we have /proc/cpuinfo or similar? */
#endif
#define SiS_haveBuiltInMC	/* Is there a built-in memcpy for this arch? */

/* Built-in memcpy for i386 */
static __inline void * builtin_memcpy(void * to, const void * from, size_t n)
{
    int d1,d2,d3;

    __asm__ __volatile__(
		  " cld\n"
		  " shrl $1, %%ecx\n"
		  " jnc 1f\n"
		  " movsb\n"
		"1: shrl $1, %%ecx\n"
		  " jnc 2f\n"
		  " movsw\n"
		"2: rep ; movsl\n"
		  : "=&c" (d1), "=&D" (d2), "=&S" (d3)
		  : "0" (n), "1" ((long) to), "2" ((long) from)
		  : "memory", "cc");

    return(to);
}

/* Alternative for 586: Unroll loop, copy 32 bytes at a time */
static void SiS_builtin_memcp2(UChar *to, const UChar *from, int n)
{
    int d1,d2,d3;

    __asm__ __volatile__(
		  " movl %%edi, %%eax\n"
		  " cmpl $32, %%ecx\n"
		  " cld\n"
		  " jbe 3f\n"
		  " negl %%eax\n"		/* Align dest */
		  " andl $3, %%eax\n"
		  " subl %%eax, %%ecx\n"
		  " xchgl %%eax, %%ecx\n"
		  " rep ; movsb\n"
		  " movl %%eax, %%ecx\n"
		  " subl $32, %%ecx\n"
		  " js 2f\n"
		  " movl (%%edi), %%eax\n"
		"1: movl 28(%%edi), %%edx\n"   	/* Trick: Read-ahead */
		  " subl $32, %%ecx\n"
		  " movl (%%esi), %%eax\n"
		  " movl 4(%%esi), %%edx\n"
		  " movl %%eax, (%%edi)\n"
		  " movl %%edx, 4(%%edi)\n"
		  " movl 8(%%esi), %%eax\n"
		  " movl 12(%%esi), %%edx\n"
		  " movl %%eax, 8(%%edi)\n"
		  " movl %%edx, 12(%%edi)\n"
		  " movl 16(%%esi), %%eax\n"
		  " movl 20(%%esi), %%edx\n"
		  " movl %%eax, 16(%%edi)\n"
		  " movl %%edx, 20(%%edi)\n"
		  " movl 24(%%esi), %%eax\n"
		  " movl 28(%%esi), %%edx\n"
		  " movl %%eax, 24(%%edi)\n"
		  " movl %%edx, 28(%%edi)\n"
		  " leal 32(%%esi), %%esi\n"
		  " leal 32(%%edi), %%edi\n"
		  " jns 1b\n"
		"2: addl $32, %%ecx\n"
		"3: rep ; movsb"
		  : "=&c" (d1), "=&D" (d2), "=&S" (d3)
		  : "0" (n), "1" ((long) to), "2" ((long) from)
		  : "eax", "edx", "memory", "cc");

}

static unsigned int taketime(void)	/* get current time (for benchmarking) */
{
    unsigned int eax;

    __asm__ volatile (
		" pushl %%ebx\n"
		" cpuid\n"
		" rdtsc\n"
		" popl %%ebx\n"
		: "=a" (eax)
		: "0" (0)
		: "ecx", "edx", "cc");

    return(eax);
}

#elif defined(__AMD64__) || defined(__amd64__) || defined(__x86_64__) /***************** AMD64 */

#define SiS_checkosforsse	/* Does this cpu support sse and do we need to check os? */
#define SiS_canBenchmark	/* Can we perform a benchmark? */
#ifdef SIS_LINUX
#define SiS_haveProc		/* Do we have /proc/cpuinfo or similar? */
#endif
#define SiS_haveBuiltInMC	/* Is there a built-in memcpy for this arch? */

/* Built-in memcpy for AMD64 */
static __inline void * builtin_memcpy(void * to, const void * from, int n)
{
    long d1, d2, d3;

    __asm__ __volatile__ (
		" cld\n"
		" rep ; movsq\n"
		" movq %4, %%rcx\n"
		" rep ; movsb"
		: "=%c" (d1), "=&D" (d2), "=&S" (d3)
		: "0" ((ULong)(n >> 3)), "q" ((ULong)(n & 7)),
		  "1" ((long) to), "2" ((long) from)
		: "memory");

    return(to);
}

/* Alternative: Unroll loop, copy 32 bytes at a time */
static void SiS_builtin_memcp2(UChar *to, const UChar *from, int n)
{
    long d1,d2,d3;

    __asm__ __volatile__(
		  " movq %%rdi, %%rax\n"
		  " cmpq $32, %%rcx\n"
		  " cld\n"			/* Pipeline; no other flags but DF */
		  " jbe 1f\n"
		  " negq %%rax\n"		/* Align dest */
		  " andq $7, %%rax\n"
		  " subq %%rax, %%rcx\n"
		  " xchgq %%rax, %%rcx\n"
		  " rep ; movsb\n"
		  " movq %%rax, %%rcx\n"
		  " subq $32, %%rcx\n"
		  " js 2f\n"
		  ".p2align 4\n"
		"3: subq $32, %%rcx\n"
		  " movq (%%rsi), %%rax\n"
		  " movq 8(%%rsi), %%rdx\n"
		  " movq 16(%%rsi), %%r8\n"
		  " movq 24(%%rsi), %%r9\n"
		  " movq %%rax, (%%rdi)\n"
		  " movq %%rdx, 8(%%rdi)\n"
		  " movq %%r8, 16(%%rdi)\n"
		  " movq %%r9, 24(%%rdi)\n"
		  " leaq 32(%%rsi), %%rsi\n"
		  " leaq 32(%%rdi), %%rdi\n"
		  " jns 3b\n"
		"2: addq $32, %%rcx\n"
		"1: rep ; movsb"
		  : "=&c" (d1), "=&D" (d2), "=&S" (d3)
		  :"0" ((ULong) n), "1" ((long) to), "2" ((long) from)
		  : "rax", "rdx", "r8", "r9", "memory", "cc");

}

static unsigned int taketime(void)	/* get current time (for benchmarking) */
{
    unsigned int eax;

    __asm__ volatile (
		" pushq %%rbx\n"
		" cpuid\n"
		" rdtsc\n"
		" popq %%rbx\n"
		: "=a" (eax)
		: "0" (0)
		: "rcx", "rdx", "cc");

    return(eax);
}

#else		/* **************************************** Other archs */

/* 1. Can we do a benchmark?		*/
/* #define SiS_canBenchmark		*/

/* 2. Do we have /proc filesystem or similar for CPU information? */
/* #define SiS_haveproc			*/

/* 3. Optional: build-in memcpy()	*/
/* #define SiS_haveBuiltInMC		*/
/* static __inline void * builtin_memcpy(void * to, const void * from, int n)
   {
   }
*/

/* 4. Function for getting current time (for benchmarking)  */
/* static unsigned int taketime(void)
   {
   }
*/

#endif

/************************************************************************/
/*                   Generic built-in memcpy wrapper                    */
/************************************************************************/

#ifdef SiS_haveBuiltInMC
static void SiS_builtin_memcpy(UChar *dst, const UChar *src, int size)
{
    builtin_memcpy(dst, src, size);
}
#endif

/************************************************************************/
/* Generic routines if Benchmark can be performed (all archs, all OSes) */
/************************************************************************/

#ifdef SiS_canBenchmark

/* Get time (unsigned int) */
static unsigned int time_function(vidCopyFunc mf, UChar *buf1, UChar *buf2, int size)
{
    unsigned int t1, t2;

    t1 = taketime();

    (*mf)(buf1, buf2, size);

    t2 = taketime();

    return((t1 <  t2) ? t2 - t1 : 0xFFFFFFFFU - (t1 - t2 - 1));
}

/* Allocate an area of offscreen FB memory (buf1), a simulated video
 * player buffer (buf2) and a pool of uninitialized "video" data (buf3).
 */
static void *
SiS_AllocBuffers(ScrnInfoPtr pScrn, UChar **buf1, UChar **buf2, UChar **buf3)
{
    SISPtr pSiS = SISPTR(pScrn);
    unsigned int offset;
    void *handle = NULL;

    if(!(offset = SISAllocateFBMemory(pScrn, &handle, BUFFERSIZE + 31))) {
       return NULL;
    }
    (*buf1) = (UChar *)pSiS->FbBase + offset;
    (*buf1) = (UChar *)(((ULong)(*buf1) + 31) & ~31);

    if(!((*buf2) = (UChar *)xalloc(BUFFERSIZE + 15))) {
       SISFreeFBMemory(pScrn, &handle);
       return NULL;
    }

    if(!((*buf3) = (UChar *)xalloc(BUFFERSIZE + 15))) {
       xfree((*buf2));
       SISFreeFBMemory(pScrn, &handle);
       return NULL;
    }

    return handle;
}

/* Perform Benchmark */
static int SiS_BenchmarkMemcpy(ScrnInfoPtr pScrn, SISMCFuncData *MCFunctions,
                               unsigned int myCPUflags, UChar *buf1, UChar *buf2,
			       UChar *buf3, char *frqBuf, double cpuFreq,
			       vidCopyFunc *UMemCpy, int *best2, Bool from)
{
    SISMCFuncData *curData;
    int j = 0, bestSoFar = 0;
    unsigned int tmp1, tmp2, best = 0xFFFFFFFFU, sbest = 0xFFFFFFFFU;

    (*best2) = 0;

    /* Make probable buf1 and buf2 are not paged out by referencing them */
    SiS_libc_memcpy(buf1, buf2, BUFFERSIZE);

    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
	       "Benchmarking %s RAM to %s RAM memory transfer methods:\n",
	       from ? "video" : "system",
	       from ? "system" : "video");

#ifdef TWDEBUG
    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Benchmark: CPUFlags %x\n", myCPUflags);
#endif

    j = 0;
    while(MCFunctions[j].mFunc) {

	curData = MCFunctions + j;

	if(myCPUflags & curData->mycpuflag) {

	   /* Simulate setup of the video buffer and copy result to framebuffer */
	   /* Do this 4 times to verify results */
	   if(!from) {
	      SiS_builtin_memcpy(buf2, buf3, BUFFERSIZE);
	      tmp1 = time_function(curData->mFunc, buf1, buf2, BUFFERSIZE);
	      SiS_builtin_memcpy(buf2, buf3, BUFFERSIZE);
	      tmp2 = time_function(curData->mFunc, buf1, buf2, BUFFERSIZE);
	      tmp1 = (tmp2 < tmp1) ? tmp2 : tmp1;
	      SiS_builtin_memcpy(buf2, buf3, BUFFERSIZE);
	      tmp2 = time_function(curData->mFunc, buf1, buf2, BUFFERSIZE);
	      tmp1 = (tmp2 < tmp1) ? tmp2 : tmp1;
	      SiS_builtin_memcpy(buf2, buf3, BUFFERSIZE);
	      tmp2 = time_function(curData->mFunc, buf1, buf2, BUFFERSIZE);
	      tmp1 = (tmp2 < tmp1) ? tmp2 : tmp1;
	   } else {
	      SiS_builtin_memcpy(buf3, buf2, BUFFERSIZE);
	      tmp1 = time_function(curData->mFunc, buf2, buf1, BUFFERSIZE);
	      SiS_builtin_memcpy(buf3, buf2, BUFFERSIZE);
	      tmp2 = time_function(curData->mFunc, buf2, buf1, BUFFERSIZE);
	      tmp1 = (tmp2 < tmp1) ? tmp2 : tmp1;
	      SiS_builtin_memcpy(buf3, buf2, BUFFERSIZE);
	      tmp2 = time_function(curData->mFunc, buf2, buf1, BUFFERSIZE);
	      tmp1 = (tmp2 < tmp1) ? tmp2 : tmp1;
	      SiS_builtin_memcpy(buf3, buf2, BUFFERSIZE);
	      tmp2 = time_function(curData->mFunc, buf2, buf1, BUFFERSIZE);
	      tmp1 = (tmp2 < tmp1) ? tmp2 : tmp1;
	   }

	   if((!frqBuf) || (tmp1 == 0)) {
	      xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
			   "\tChecked %s memcpy()... \t%u\n",curData->mName, tmp1);
	   } else {
	      xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
			   "\tChecked %s memcpy()... \t%.1f MiB/s\n",
			   curData->mName,
			   cpuFreq * 1.e6 * (double)BUFFERSIZE / ((double)(tmp1) * (double)(0x100000)));
	   }

	   if(tmp1 < best) {
	      best = tmp1;
	      bestSoFar = j;
	   }

	   if(!curData->reqAlignment) {
	      if(tmp1 < sbest) {
	         sbest = tmp1;
	         (*best2) = j;
	      }
	   }

	}

	j++;
    }

    return bestSoFar;
}

static vidCopyFunc SiS_GetBestByGrade(ScrnInfoPtr pScrn, SISMCFuncData *MCFunctions,
			unsigned int myCPUflags, vidCopyFunc *UMemCpy, Bool from)
{
    int j = 0, best = -1, secondbest = -1, bestSoFar = 10, best2SoFar = 10;
    int grade;

    *UMemCpy = SiS_libc_memcpy;

    while(MCFunctions[j].mFunc) {
	if(myCPUflags & MCFunctions[j].mycpuflag) {
	   grade = from ? MCFunctions[j].gradefrom : MCFunctions[j].grade;
	   if(grade < bestSoFar) {
	      best = j;
	      bestSoFar = grade;
	   }
	   if(grade < best2SoFar) {
	      if(!MCFunctions[j].reqAlignment) {
	         secondbest = j;
		 best2SoFar = grade;
	      }
	   }
	}
	j++;
    }
    if(best >= 0) {
       xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		"Chose %s method for aligned data transfers %s video RAM\n",
		MCFunctions[best].mName,
		from ? "from" : "to");
       if(secondbest >= 0) {
          xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		"Chose %s method for unaligned data transfers %s video RAM\n",
		   MCFunctions[secondbest].mName,
		   from ? "from" : "to");
          *UMemCpy = MCFunctions[secondbest].mFunc;
       }
       return MCFunctions[best].mFunc;
    }

    return SiS_libc_memcpy;
}
#endif /* canBenchmark */

/**********************************************************************/
/*      Generic routines if /proc filesystem is available (Linux)     */
/**********************************************************************/

#ifdef SiS_haveProc
/* Linux: Read file (/proc/cpuinfo) into buffer */
static int SiS_ReadProc(char *buf, char *filename)
{
    FILE *cpuInfoFile;
    int count;

    if((cpuInfoFile = fopen(filename, "r")) == NULL) {
       return 0;
    }

    count = fread(buf, 1, CPUBUFFERSIZE, cpuInfoFile);
    if(ferror(cpuInfoFile)) {
       fclose(cpuInfoFile);
       return 0;
    }

    fclose(cpuInfoFile);

    if(count >= CPUBUFFERSIZE - 2) {
       return 0;
    }

    buf[count] = 0;

    return count;
}

/* Linux: Extract CPU speed from /proc/cpuinfo */
static char *SiS_GetCPUFreq(ScrnInfoPtr pScrn, char *buf, double *cpuFreq)
{
    char *frqBuf, *endBuf;

    (*cpuFreq) = 0.0;

    if((frqBuf = strstr(buf,"cpu MHz\t\t:"))) {
       frqBuf += 11;
       (*cpuFreq) = strtod(frqBuf, &endBuf);
       if(endBuf == frqBuf) frqBuf = NULL;
       if((*cpuFreq) < 10.0) frqBuf = NULL; /* sanity check */
       if(frqBuf) {
          xf86DrvMsg(pScrn->scrnIndex, X_PROBED, "CPU frequency %.2fMhz\n", (*cpuFreq));
       }
    }

    return frqBuf;
}
#endif /* haveProc */

/**********************************************************************/
/*                      Arch-specific routines                        */
/**********************************************************************/

#ifdef SiS_checkosforsse   /* Common i386, AMD64  */

#ifdef SISCHECKOSSSE

#ifndef XFree86LOADER
#include <setjmp.h>
#endif

static jmp_buf sigill_return;

static void sigill_handler(void)
{
    longjmp(sigill_return, 1);
}
#endif

static Bool CheckOSforSSE(ScrnInfoPtr pScrn)
{
#ifdef SISCHECKOSSSE  /* Check OS for SSE possible: */
    int signo = -1;

#ifdef SISDGBMC
    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Checking OS SSE support\n");
#endif

    xf86InterceptSigIll(&sigill_handler);

    if(setjmp(sigill_return)) {
       signo = 4;
    } else {
       __asm__ __volatile__ (" xorps %xmm0, %xmm0\n");
       /* __asm__ __volatile__ (" .byte 0xff\n"); */  /* For test */
    }

    xf86InterceptSigIll(NULL);

#ifdef SISDGBMC
    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "OS SSE support signal %d\n", signo);
#endif

    if(signo != -1) {
       xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
		"OS does not support SSE instructions\n");
    }

    return (signo >= 0) ? FALSE : TRUE;

#else  /* no check for SSE possible: */

    SISPtr pSiS = SISPTR(pScrn);

    xf86DrvMsg(pScrn->scrnIndex, pSiS->XvSSEMemcpy ? X_WARNING : X_INFO,
	"Checking OS for SSE support is not supported in this version of " SISMYSERVERNAME "\n");

    if(pSiS->XvSSEMemcpy) {
       xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
	"If you get a signal 4 here, set the option \"UseSSE\" to \"off\".\n");
       return TRUE;
    } else {
       xf86DrvMsg(pScrn->scrnIndex, X_INFO,
	"If your OS supports SSE, set the option \"UseSSE\" to \"on\".\n");
       return FALSE;
    }
#endif
}

#endif /* SiS_checkosforsse */

#ifdef __i386__   /* i386 specific *************************************/

PREFETCH_FUNC(SiS_sse,SSE,SSE,,FENCE,small_memcpy_i386)
PREFETCH_FUNC(SiS_mmxext,MMXEXT,SSE,EMMS,FENCEMMS,small_memcpy_i386)
PREFETCH_FUNC(SiS_now,MMX,NOW,FEMMS,FEMMS,small_memcpy_i386)
NOPREFETCH_FUNC(SiS_mmx,MMX,EMMS,EMMS,small_memcpy_i386)

static SISMCFuncData MCFunctions_i386[] = {
    {SiS_libc_memcpy,   "libc",      SIS_CPUFL_LIBC,  4,  4, FALSE},
    {SiS_builtin_memcpy,"built-in-1",SIS_CPUFL_BI,    5,  5, FALSE},
    {SiS_builtin_memcp2,"built-in-2",SIS_CPUFL_BI2,   6,  6, FALSE},
    {SiS_mmx_memcpy,    "MMX",       SIS_CPUFL_MMX,   3,  3, FALSE},
    {SiS_sse_memcpy,    "SSE",       SIS_CPUFL_SSE,   1,  0, TRUE},
    {SiS_now_memcpy,    "3DNow!",    SIS_CPUFL_3DNOW, 2,  2, FALSE},
    {SiS_mmxext_memcpy, "MMX2",      SIS_CPUFL_MMX2,  0,  1, FALSE},
    {NULL,              "",          0,              10, 10, FALSE}
};

#define Def_FL  (SIS_CPUFL_LIBC | SIS_CPUFL_BI | SIS_CPUFL_BI2)  /* Default methods */

#define cpuid(op, eax, ebx, ecx, edx) 		\
    __asm__ __volatile__ (			\
		" pushl %%ebx\n"		\
		" cpuid\n"			\
		" movl %%ebx, %1\n"		\
		" popl %%ebx\n"			\
		: "=a" (eax), "=r" (ebx), 	\
		  "=c" (ecx), "=d" (edx)	\
		: "a" (op)			\
		: "cc")

static Bool cpuIDSupported(ScrnInfoPtr pScrn)
{
    int eax, ebx, ecx, edx;

    /* Check for cpuid instruction */
    __asm__ __volatile__ (
		" pushf\n"
		" popl %0\n"
		" movl %0, %1\n"
		" xorl $0x200000, %0\n"
		" push %0\n"
		" popf\n"
		" pushf\n"
		" popl %0\n"
		: "=a" (eax), "=c" (ecx)
		:
		: "cc");

    if(eax == ecx) {
       xf86DrvMsg(pScrn->scrnIndex, X_PROBED, "CPU does not support CPUID instruction\n");
       return FALSE;
    }

    /* Check for cpuid level */
    cpuid(0x00000000, eax, ebx, ecx, edx);
    if(!eax) {
       return FALSE;
    }

    /* Check for RDTSC */
    cpuid(0x00000001, eax, ebx, ecx, edx);

    if(!(edx & 0x10)) {
       xf86DrvMsg(pScrn->scrnIndex, X_PROBED, "CPU does not support RDTSC instruction\n");
       return FALSE;
    }

    return TRUE;
}

static unsigned int SiS_GetCpuFeatures(ScrnInfoPtr pScrn)
{
    unsigned int flags = 0, eax, ebx, ecx, edx;
    Bool IsAMD;

    /* Check if cpuid and rdtsc instructions are supported */
    if(!cpuIDSupported(pScrn)) {
       return 0;
    }

    cpuid(0x00000000, eax, ebx, ecx, edx);

    IsAMD = (ebx == 0x68747541) && (edx == 0x69746e65) && (ecx == 0x444d4163);

    cpuid(0x00000001, eax, ebx, ecx, edx);
    /* MMX */
    if(edx & 0x00800000) flags |= SIS_CPUFL_MMX;
    /* SSE, MMXEXT */
    if(edx & 0x02000000) flags |= (SIS_CPUFL_SSE | SIS_CPUFL_MMX2);
    /* SSE2 - don't need this one directly, set SSE instead */
    if(edx & 0x04000000) flags |= (SIS_CPUFL_SSE | SIS_CPUFL_SSE2);

    cpuid(0x80000000, eax, ebx, ecx, edx);
    if(eax >= 0x80000001) {
       cpuid(0x80000001, eax, ebx, ecx, edx);
       /* 3DNow! */
       if(edx & 0x80000000) flags |= SIS_CPUFL_3DNOW;
       /* AMD MMXEXT */
       if(IsAMD && (edx & 0x00400000)) flags |= SIS_CPUFL_MMX2;
    }

    return flags;
}

#elif defined(__AMD64__) || defined(__amd64__) || defined(__x86_64__) /* AMD64 specific ***** */

PREFETCH_FUNC(SiS_sse,SSE64,SSE,,FENCE,small_memcpy_amd64)

static SISMCFuncData MCFunctions_AMD64[] = {
    {SiS_libc_memcpy,   "libc",      SIS_CPUFL_LIBC, 2,  2, FALSE},
    {SiS_builtin_memcpy,"built-in-1",SIS_CPUFL_BI,   1,  1, FALSE},
    {SiS_builtin_memcp2,"built-in-2",SIS_CPUFL_BI2,  3,  3, FALSE},
    {SiS_sse_memcpy,    "SSE",       SIS_CPUFL_SSE,  0,  0, TRUE},
    {NULL,              "",          0,             10, 10, FALSE}
};

#define Def_FL  (SIS_CPUFL_LIBC | SIS_CPUFL_BI | SIS_CPUFL_BI2)

static unsigned int SiS_GetCpuFeatures(ScrnInfoPtr pScrn)
{
    return((unsigned int)(SIS_CPUFL_SSE|SIS_CPUFL_SSE2));
}

#else  /* Specific for other archs ******************************** */

/* Fill in here */

#define Def_FL  (SIS_CPUFL_LIBC)

static unsigned int SiS_GetCpuFeatures(ScrnInfoPtr pScrn)
{
    return((unsigned int)(0));
}

#endif

/**********************************************************************/
/*     Benchmark the video copy routines and choose the fastest       */
/**********************************************************************/

#ifdef SiS_canBenchmark
static vidCopyFunc
SiSVidCopyInitGen(ScreenPtr pScreen, SISMCFuncData *MCFunctions, vidCopyFunc *UMemCpy, Bool from)
{
    ScrnInfoPtr pScrn = xf86Screens[pScreen->myNum];
    SISPtr pSiS = SISPTR(pScrn);
    void *fbhandle = NULL;
    char  *frqBuf = NULL;
    UChar *buf1, *buf2, *buf3;
    double cpuFreq = 0.0;
    unsigned int myCPUflags = pSiS->CPUFlags | Def_FL;
    int best, secondbest;
#ifdef SiS_haveProc
    char buf[CPUBUFFERSIZE];
#endif

    *UMemCpy = SiS_libc_memcpy;

    /* Bail out if user disabled benchmarking */
    if(!pSiS->BenchMemCpy) {
       return SiS_libc_memcpy;
    }

#ifdef SiS_haveProc
    /* Read /proc/cpuinfo into buf */
    if(SiS_ReadProc(buf, "/proc/cpuinfo")) {

       /* Extract CPU frequency */
       frqBuf = SiS_GetCPUFreq(pScrn, buf, &cpuFreq);

    }
#endif

    /* Allocate buffers */
    if(!(fbhandle = SiS_AllocBuffers(pScrn, &buf1, &buf2, &buf3))) {
       xf86DrvMsg(pScrn->scrnIndex, X_INFO,
       		"Failed to allocate video RAM for video data transfer benchmark\n");
       return SiS_GetBestByGrade(pScrn, MCFunctions, myCPUflags, UMemCpy, from);
    }

    /* Perform Benchmark */
    best = SiS_BenchmarkMemcpy(pScrn, MCFunctions, myCPUflags, buf1,
    				(UChar *)(((unsigned long)buf2 + 15) & ~15),
				(UChar *)(((unsigned long)buf3 + 15) & ~15),
				frqBuf, cpuFreq, UMemCpy, &secondbest, from);

    /* Free buffers */
    SISFreeFBMemory(pScrn, &fbhandle);
    xfree(buf2);
    xfree(buf3);

    xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
	       "Using %s method for aligned data transfers %s video RAM\n",
	       MCFunctions[best].mName,
	       from ? "from" : "to");

    xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
	       "Using %s method for unaligned data transfers %s video RAM\n",
	       MCFunctions[secondbest].mName,
	       from ? "from" : "to");

    return MCFunctions[best].mFunc;
}
#endif /* canBenchmark */

/**********************************************************************/
/* 		       main(): Get CPU capabilities		      */
/* 			    (called externally)			      */
/**********************************************************************/

unsigned int
SiSGetCPUFlags(ScrnInfoPtr pScrn)
{
    unsigned int myCPUflags = SiS_GetCpuFeatures(pScrn);

#ifdef SiS_checkosforsse
    if(myCPUflags & (SIS_CPUFL_SSE | SIS_CPUFL_SSE2)) {

       /* Check if OS supports usage of SSE instructions */
       if(!(CheckOSforSSE(pScrn))) {
          myCPUflags &= ~(SIS_CPUFL_SSE | SIS_CPUFL_SSE2);
       }

    }
#endif

    return myCPUflags;
}

/**********************************************************************/
/*                       main(): SiSVidCopyInit()                     */
/*			    (called externally)			      */
/*		(SiSGetCPUFlags must be called before this one)       */
/**********************************************************************/

vidCopyFunc SiSVidCopyInit(ScreenPtr pScreen, vidCopyFunc *UMemCpy, Bool from)
{
#if defined(__i386__) && defined(SiS_canBenchmark)
    return(SiSVidCopyInitGen(pScreen, MCFunctions_i386, UMemCpy, from));
#elif (defined(__AMD64__) || defined(__amd64__) || defined(__x86_64__)) && defined(SiS_canBenchmark)
    return(SiSVidCopyInitGen(pScreen, MCFunctions_AMD64, UMemCpy, from));
#else /* Other cases: Use libc memcpy() */
    *UMemCpy = SiS_libc_memcpy;
    return SiS_libc_memcpy;
#endif
}

vidCopyFunc SiSVidCopyGetDefault(void)
{
    return SiS_libc_memcpy;
}

#endif /* GNU C */


