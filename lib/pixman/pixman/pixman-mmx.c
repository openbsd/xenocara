/*
 * Copyright © 2004, 2005 Red Hat, Inc.
 * Copyright © 2004 Nicholas Miell
 * Copyright © 2005 Trolltech AS
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of Red Hat not be used in advertising or
 * publicity pertaining to distribution of the software without specific,
 * written prior permission.  Red Hat makes no representations about the
 * suitability of this software for any purpose.  It is provided "as is"
 * without express or implied warranty.
 *
 * THE COPYRIGHT HOLDERS DISCLAIM ALL WARRANTIES WITH REGARD TO THIS
 * SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS, IN NO EVENT SHALL THE COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN
 * AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING
 * OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
 * SOFTWARE.
 *
 * Author:  Søren Sandmann (sandmann@redhat.com)
 * Minor Improvements: Nicholas Miell (nmiell@gmail.com)
 * MMX code paths for fbcompose.c by Lars Knoll (lars@trolltech.com)
 *
 * Based on work by Owen Taylor
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef USE_MMX

#include <mmintrin.h>
#include "pixman-private.h"

#define noVERBOSE

#ifdef VERBOSE
#define CHECKPOINT() ErrorF ("at %s %d\n", __FUNCTION__, __LINE__)
#else
#define CHECKPOINT()
#endif

/* Notes about writing mmx code
 *
 * give memory operands as the second operand. If you give it as the
 * first, gcc will first load it into a register, then use that
 * register
 *
 *   ie. use
 *
 *         _mm_mullo_pi16 (x, mmx_constant);
 *
 *   not
 *
 *         _mm_mullo_pi16 (mmx_constant, x);
 *
 * Also try to minimize dependencies. i.e. when you need a value, try
 * to calculate it from a value that was calculated as early as
 * possible.
 */

/* --------------- MMX primitives ------------------------------------- */

#ifdef __GNUC__
typedef uint64_t mmxdatafield;
#else
typedef __m64 mmxdatafield;
/* If __m64 is defined as a struct or union, define M64_MEMBER to be the
   name of the member used to access the data */
# ifdef _MSC_VER
#  define M64_MEMBER m64_u64
# elif defined(__SUNPRO_C)
#  define M64_MEMBER l_
# endif
#endif

typedef struct
{
    mmxdatafield mmx_4x00ff;
    mmxdatafield mmx_4x0080;
    mmxdatafield mmx_565_rgb;
    mmxdatafield mmx_565_unpack_multiplier;
    mmxdatafield mmx_565_r;
    mmxdatafield mmx_565_g;
    mmxdatafield mmx_565_b;
    mmxdatafield mmx_mask_0;
    mmxdatafield mmx_mask_1;
    mmxdatafield mmx_mask_2;
    mmxdatafield mmx_mask_3;
    mmxdatafield mmx_full_alpha;
    mmxdatafield mmx_ffff0000ffff0000;
    mmxdatafield mmx_0000ffff00000000;
    mmxdatafield mmx_000000000000ffff;
} MMXData;

#if defined(_MSC_VER)
# define MMXDATA_INIT(field, val) { val##UI64 }
#elif defined(M64_MEMBER)	/* __m64 is a struct, not an integral type */
# define MMXDATA_INIT(field, val) field =   { val##ULL }
#else				/* __m64 is an integral type */
# define MMXDATA_INIT(field, val) field =   val##ULL
#endif

static const MMXData c =
{
    MMXDATA_INIT(.mmx_4x00ff,			0x00ff00ff00ff00ff),
    MMXDATA_INIT(.mmx_4x0080,			0x0080008000800080),
    MMXDATA_INIT(.mmx_565_rgb,			0x000001f0003f001f),
    MMXDATA_INIT(.mmx_565_unpack_multiplier,	0x0000008404100840),
    MMXDATA_INIT(.mmx_565_r,			0x000000f800000000),
    MMXDATA_INIT(.mmx_565_g,			0x0000000000fc0000),
    MMXDATA_INIT(.mmx_565_b,			0x00000000000000f8),
    MMXDATA_INIT(.mmx_mask_0,			0xffffffffffff0000),
    MMXDATA_INIT(.mmx_mask_1,			0xffffffff0000ffff),
    MMXDATA_INIT(.mmx_mask_2,			0xffff0000ffffffff),
    MMXDATA_INIT(.mmx_mask_3,			0x0000ffffffffffff),
    MMXDATA_INIT(.mmx_full_alpha,		0x00ff000000000000),
    MMXDATA_INIT(.mmx_ffff0000ffff0000,		0xffff0000ffff0000),
    MMXDATA_INIT(.mmx_0000ffff00000000,		0x0000ffff00000000),
    MMXDATA_INIT(.mmx_000000000000ffff,		0x000000000000ffff),
};

#ifdef __GNUC__
#    ifdef __ICC
#        define MC(x)  M64(c.mmx_##x)
#    else
#        define MC(x) ((__m64)c.mmx_##x)
#    endif
#else
#    define MC(x) c.mmx_##x
#endif

static force_inline __m64
M64 (uint64_t x)
{
#ifdef __ICC
    return _mm_cvtsi64_m64 (x);
#elif defined M64_MEMBER	/* __m64 is a struct, not an integral type */
    __m64 res;

    res.M64_MEMBER = x;
    return res;
#else				/* __m64 is an integral type */
    return (__m64)x;
#endif
}

static force_inline uint64_t
UINT64 (__m64 x)
{
#ifdef __ICC
    return _mm_cvtm64_si64 (x);
#elif defined M64_MEMBER	/* __m64 is a struct, not an integral type */
    uint64_t res = x.M64_MEMBER;
    return res;
#else				/* __m64 is an integral type */
    return (uint64_t)x;
#endif
}

static force_inline __m64
shift (__m64 v, int s)
{
    if (s > 0)
	return _mm_slli_si64 (v, s);
    else if (s < 0)
	return _mm_srli_si64 (v, -s);
    else
	return v;
}

static force_inline __m64
negate (__m64 mask)
{
    return _mm_xor_si64 (mask, MC(4x00ff));
}

static force_inline __m64
pix_multiply (__m64 a, __m64 b)
{
    __m64 res;

    res = _mm_mullo_pi16 (a, b);
    res = _mm_adds_pu16 (res, MC(4x0080));
    res = _mm_adds_pu16 (res, _mm_srli_pi16 (res, 8));
    res = _mm_srli_pi16 (res, 8);

    return res;
}

static force_inline __m64
pix_add (__m64 a, __m64 b)
{
    return  _mm_adds_pu8 (a, b);
}

static force_inline __m64
expand_alpha (__m64 pixel)
{
    __m64 t1, t2;

    t1 = shift (pixel, -48);
    t2 = shift (t1, 16);
    t1 = _mm_or_si64 (t1, t2);
    t2 = shift (t1, 32);
    t1 = _mm_or_si64 (t1, t2);

    return t1;
}

static force_inline __m64
expand_alpha_rev (__m64 pixel)
{
    __m64 t1, t2;

    /* move alpha to low 16 bits and zero the rest */
    t1 = shift (pixel,  48);
    t1 = shift (t1, -48);

    t2 = shift (t1, 16);
    t1 = _mm_or_si64 (t1, t2);
    t2 = shift (t1, 32);
    t1 = _mm_or_si64 (t1, t2);

    return t1;
}

static force_inline __m64
invert_colors (__m64 pixel)
{
    __m64 x, y, z;

    x = y = z = pixel;

    x = _mm_and_si64 (x, MC(ffff0000ffff0000));
    y = _mm_and_si64 (y, MC(000000000000ffff));
    z = _mm_and_si64 (z, MC(0000ffff00000000));

    y = shift (y, 32);
    z = shift (z, -32);

    x = _mm_or_si64 (x, y);
    x = _mm_or_si64 (x, z);

    return x;
}

static force_inline __m64
over (__m64 src, __m64 srca, __m64 dest)
{
    return  _mm_adds_pu8 (src, pix_multiply(dest, negate(srca)));
}

static force_inline __m64
over_rev_non_pre (__m64 src, __m64 dest)
{
    __m64 srca = expand_alpha (src);
    __m64 srcfaaa = _mm_or_si64 (srca, MC(full_alpha));

    return over(pix_multiply(invert_colors(src), srcfaaa), srca, dest);
}

static force_inline __m64
in (__m64 src,
    __m64 mask)
{
    return pix_multiply (src, mask);
}

static force_inline __m64
in_over_full_src_alpha (__m64 src, __m64 mask, __m64 dest)
{
    src = _mm_or_si64 (src, MC(full_alpha));

    return over(in (src, mask), mask, dest);
}

#ifndef _MSC_VER
static force_inline __m64
in_over (__m64 src,
	 __m64 srca,
	 __m64 mask,
	 __m64 dest)
{
    return over(in(src, mask), pix_multiply(srca, mask), dest);
}
#else
#define in_over(src, srca, mask, dest) over(in(src, mask), pix_multiply(srca, mask), dest)
#endif

static force_inline __m64
load8888 (uint32_t v)
{
    return _mm_unpacklo_pi8 (_mm_cvtsi32_si64 (v), _mm_setzero_si64());
}

static force_inline __m64
pack8888 (__m64 lo, __m64 hi)
{
    return _mm_packs_pu16 (lo, hi);
}

static force_inline uint32_t
store8888 (__m64 v)
{
    return _mm_cvtsi64_si32(pack8888(v, _mm_setzero_si64()));
}

/* Expand 16 bits positioned at @pos (0-3) of a mmx register into
 *
 *    00RR00GG00BB
 *
 * --- Expanding 565 in the low word ---
 *
 * m = (m << (32 - 3)) | (m << (16 - 5)) | m;
 * m = m & (01f0003f001f);
 * m = m * (008404100840);
 * m = m >> 8;
 *
 * Note the trick here - the top word is shifted by another nibble to
 * avoid it bumping into the middle word
 */
static force_inline __m64
expand565 (__m64 pixel, int pos)
{
    __m64 p = pixel;
    __m64 t1, t2;

    /* move pixel to low 16 bit and zero the rest */
    p = shift (shift (p, (3 - pos) * 16), -48);

    t1 = shift (p, 36 - 11);
    t2 = shift (p, 16 - 5);

    p = _mm_or_si64 (t1, p);
    p = _mm_or_si64 (t2, p);
    p = _mm_and_si64 (p, MC(565_rgb));

    pixel = _mm_mullo_pi16 (p, MC(565_unpack_multiplier));
    return _mm_srli_pi16 (pixel, 8);
}

static force_inline __m64
expand8888 (__m64 in, int pos)
{
    if (pos == 0)
	return _mm_unpacklo_pi8 (in, _mm_setzero_si64());
    else
	return _mm_unpackhi_pi8 (in, _mm_setzero_si64());
}

static force_inline __m64
expandx888 (__m64 in, int pos)
{
    return _mm_or_si64 (expand8888 (in, pos), MC(full_alpha));
}

static force_inline __m64
pack565 (__m64 pixel, __m64 target, int pos)
{
    __m64 p = pixel;
    __m64 t = target;
    __m64 r, g, b;

    r = _mm_and_si64 (p, MC(565_r));
    g = _mm_and_si64 (p, MC(565_g));
    b = _mm_and_si64 (p, MC(565_b));

    r = shift (r, - (32 - 8) + pos * 16);
    g = shift (g, - (16 - 3) + pos * 16);
    b = shift (b, - (0  + 3) + pos * 16);

    if (pos == 0)
	t = _mm_and_si64 (t, MC(mask_0));
    else if (pos == 1)
	t = _mm_and_si64 (t, MC(mask_1));
    else if (pos == 2)
	t = _mm_and_si64 (t, MC(mask_2));
    else if (pos == 3)
	t = _mm_and_si64 (t, MC(mask_3));

    p = _mm_or_si64 (r, t);
    p = _mm_or_si64 (g, p);

    return _mm_or_si64 (b, p);
}

#ifndef _MSC_VER
static force_inline __m64
pix_add_mul (__m64 x, __m64 a, __m64 y, __m64 b)
{
    x = _mm_mullo_pi16 (x, a);
    y = _mm_mullo_pi16 (y, b);
    x = _mm_adds_pu16 (x, MC(4x0080));
    x = _mm_adds_pu16 (x, y);
    x = _mm_adds_pu16 (x, _mm_srli_pi16 (x, 8));
    x = _mm_srli_pi16 (x, 8);

    return x;
}
#else
#define pix_add_mul(x, a, y, b) \
( x = _mm_mullo_pi16 (x, a), \
  y = _mm_mullo_pi16 (y, b), \
  x = _mm_adds_pu16 (x, MC(4x0080)), \
  x = _mm_adds_pu16 (x, y), \
  x = _mm_adds_pu16 (x, _mm_srli_pi16 (x, 8)), \
  _mm_srli_pi16 (x, 8) )
#endif

/* --------------- MMX code patch for fbcompose.c --------------------- */

static force_inline uint32_t
combine (const uint32_t *src, const uint32_t *mask)
{
    uint32_t ssrc = *src;

    if (mask)
    {
	__m64 m = load8888 (*mask);
	__m64 s = load8888 (ssrc);

	m = expand_alpha (m);
	s = pix_multiply (s, m);

	ssrc = store8888 (s);
    }

    return ssrc;
}

static FASTCALL void
mmxCombineOverU (pixman_implementation_t *imp, pixman_op_t op,
		 uint32_t *dest, const uint32_t *src, const uint32_t *mask, int width)
{
    const uint32_t *end = dest + width;

    while (dest < end) {
	uint32_t ssrc = combine (src, mask);
	uint32_t a = ssrc >> 24;
	if (a == 0xff) {
	    *dest = ssrc;
	} else if (ssrc) {
	    __m64 s, sa;
	    s = load8888(ssrc);
	    sa = expand_alpha(s);
	    *dest = store8888(over(s, sa, load8888(*dest)));
	}
	++dest;
	++src;
	if (mask)
	    ++mask;
    }
    _mm_empty();
}

static FASTCALL void
mmxCombineOverReverseU (pixman_implementation_t *imp, pixman_op_t op,
			uint32_t *dest, const uint32_t *src, const uint32_t *mask, int width)
{
    const uint32_t *end = dest + width;

    while (dest < end) {
	__m64 d, da;
	uint32_t s = combine (src, mask);
	d = load8888(*dest);
	da = expand_alpha(d);
	*dest = store8888(over (d, da, load8888(s)));
        ++dest;
        ++src;
	if (mask)
	    mask++;
    }
    _mm_empty();
}

static FASTCALL void
mmxCombineInU (pixman_implementation_t *imp, pixman_op_t op,
	       uint32_t *dest, const uint32_t *src, const uint32_t *mask, int width)
{
    const uint32_t *end = dest + width;

    while (dest < end) {
        __m64 x, a;
        x = load8888 (combine (src, mask));
        a = load8888(*dest);
        a = expand_alpha(a);
        x = pix_multiply(x, a);
        *dest = store8888(x);
        ++dest;
        ++src;
	if (mask)
	    mask++;
    }
    _mm_empty();
}

static FASTCALL void
mmxCombineInReverseU (pixman_implementation_t *imp, pixman_op_t op,
		      uint32_t *dest, const uint32_t *src, const uint32_t *mask, int width)
{
    const uint32_t *end = dest + width;

    while (dest < end) {
        __m64 x, a;
        x = load8888(*dest);
        a = load8888(combine (src, mask));
        a = expand_alpha(a);
        x = pix_multiply(x, a);
        *dest = store8888(x);
        ++dest;
        ++src;
	if (mask)
	    mask++;
    }
    _mm_empty();
}

static FASTCALL void
mmxCombineOutU (pixman_implementation_t *imp, pixman_op_t op,
		uint32_t *dest, const uint32_t *src, const uint32_t *mask, int width)
{
    const uint32_t *end = dest + width;

    while (dest < end) {
        __m64 x, a;
        x = load8888(combine (src, mask));
        a = load8888(*dest);
        a = expand_alpha(a);
        a = negate(a);
        x = pix_multiply(x, a);
        *dest = store8888(x);
        ++dest;
        ++src;
	if (mask)
	    mask++;
    }
    _mm_empty();
}

static FASTCALL void
mmxCombineOutReverseU (pixman_implementation_t *imp, pixman_op_t op,
		       uint32_t *dest, const uint32_t *src, const uint32_t *mask, int width)
{
    const uint32_t *end = dest + width;

    while (dest < end) {
        __m64 x, a;
        x = load8888(*dest);
        a = load8888(combine (src, mask));
        a = expand_alpha(a);
        a = negate(a);
        x = pix_multiply(x, a);
        *dest = store8888(x);
        ++dest;
        ++src;
	if (mask)
	    mask++;
    }
    _mm_empty();
}

static FASTCALL void
mmxCombineAtopU (pixman_implementation_t *imp, pixman_op_t op,
		 uint32_t *dest, const uint32_t *src, const uint32_t *mask, int width)
{
    const uint32_t *end = dest + width;

    while (dest < end) {
        __m64 s, da, d, sia;
        s = load8888(combine (src, mask));
        d = load8888(*dest);
        sia = expand_alpha(s);
        sia = negate(sia);
        da = expand_alpha(d);
        s = pix_add_mul (s, da, d, sia);
        *dest = store8888(s);
        ++dest;
        ++src;
	if (mask)
	    mask++;
    }
    _mm_empty();
}

static FASTCALL void
mmxCombineAtopReverseU (pixman_implementation_t *imp, pixman_op_t op,
			uint32_t *dest, const uint32_t *src, const uint32_t *mask, int width)
{
    const uint32_t *end;

    end = dest + width;

    while (dest < end) {
        __m64 s, dia, d, sa;
        s = load8888(combine(src, mask));
        d = load8888(*dest);
        sa = expand_alpha(s);
        dia = expand_alpha(d);
        dia = negate(dia);
	s = pix_add_mul (s, dia, d, sa);
        *dest = store8888(s);
        ++dest;
        ++src;
	if (mask)
	    mask++;
    }
    _mm_empty();
}

static FASTCALL void
mmxCombineXorU (pixman_implementation_t *imp, pixman_op_t op,
		uint32_t *dest, const uint32_t *src, const uint32_t *mask, int width)
{
    const uint32_t *end = dest + width;

    while (dest < end) {
        __m64 s, dia, d, sia;
        s = load8888(combine(src, mask));
        d = load8888(*dest);
        sia = expand_alpha(s);
        dia = expand_alpha(d);
        sia = negate(sia);
        dia = negate(dia);
	s = pix_add_mul (s, dia, d, sia);
        *dest = store8888(s);
        ++dest;
        ++src;
	if (mask)
	    mask++;
    }
    _mm_empty();
}

static FASTCALL void
mmxCombineAddU (pixman_implementation_t *imp, pixman_op_t op,
		uint32_t *dest, const uint32_t *src, const uint32_t *mask, int width)
{
    const uint32_t *end = dest + width;
    while (dest < end) {
        __m64 s, d;
        s = load8888(combine(src,mask));
        d = load8888(*dest);
        s = pix_add(s, d);
        *dest = store8888(s);
        ++dest;
        ++src;
	if (mask)
	    mask++;
    }
    _mm_empty();
}

static FASTCALL void
mmxCombineSaturateU (pixman_implementation_t *imp, pixman_op_t op,
		     uint32_t *dest, const uint32_t *src, const uint32_t *mask, int width)
{
    const uint32_t *end = dest + width;
    while (dest < end) {
        uint32_t s = combine(src,mask);
        uint32_t d = *dest;
        __m64 ms = load8888(s);
        __m64 md = load8888(d);
        uint32_t sa = s >> 24;
        uint32_t da = ~d >> 24;

        if (sa > da) {
            __m64 msa = load8888(FbIntDiv(da, sa) << 24);
            msa = expand_alpha(msa);
            ms = pix_multiply(ms, msa);
        }
        md = pix_add(md, ms);
        *dest = store8888(md);
        ++src;
        ++dest;
	if (mask)
	    mask++;
    }
    _mm_empty();
}


static FASTCALL void
mmxCombineSrcC (pixman_implementation_t *imp, pixman_op_t op,
		uint32_t *dest, const uint32_t *src, const uint32_t *mask, int width)
{
    const uint32_t *end = src + width;
    while (src < end) {
        __m64 a = load8888(*mask);
        __m64 s = load8888(*src);
        s = pix_multiply(s, a);
        *dest = store8888(s);
        ++src;
        ++mask;
        ++dest;
    }
    _mm_empty();
}

static FASTCALL void
mmxCombineOverC (pixman_implementation_t *imp, pixman_op_t op,
		 uint32_t *dest, const uint32_t *src, const uint32_t *mask, int width)
{
    const uint32_t *end = src + width;
    while (src < end) {
        __m64 a = load8888(*mask);
        __m64 s = load8888(*src);
        __m64 d = load8888(*dest);
        __m64 sa = expand_alpha(s);

	*dest = store8888(in_over (s, sa, a, d));

        ++src;
        ++dest;
        ++mask;
    }
    _mm_empty();
}

static FASTCALL void
mmxCombineOverReverseC (pixman_implementation_t *imp, pixman_op_t op,
			uint32_t *dest, const uint32_t *src, const uint32_t *mask, int width)
{
    const uint32_t *end = src + width;
    while (src < end) {
        __m64 a = load8888(*mask);
        __m64 s = load8888(*src);
        __m64 d = load8888(*dest);
        __m64 da = expand_alpha(d);

	*dest = store8888(over (d, da, in (s, a)));

        ++src;
        ++dest;
        ++mask;
    }
    _mm_empty();
}


static FASTCALL void
mmxCombineInC (pixman_implementation_t *imp, pixman_op_t op,
	       uint32_t *dest, const uint32_t *src, const uint32_t *mask, int width)
{
    const uint32_t *end = src + width;
    while (src < end) {
        __m64 a = load8888(*mask);
        __m64 s = load8888(*src);
        __m64 d = load8888(*dest);
        __m64 da = expand_alpha(d);
        s = pix_multiply(s, a);
        s = pix_multiply(s, da);
        *dest = store8888(s);
        ++src;
        ++dest;
        ++mask;
    }
    _mm_empty();
}

static FASTCALL void
mmxCombineInReverseC (pixman_implementation_t *imp, pixman_op_t op,
		      uint32_t *dest, const uint32_t *src, const uint32_t *mask, int width)
{
    const uint32_t *end = src + width;
    while (src < end) {
        __m64 a = load8888(*mask);
        __m64 s = load8888(*src);
        __m64 d = load8888(*dest);
        __m64 sa = expand_alpha(s);
        a = pix_multiply(a, sa);
        d = pix_multiply(d, a);
        *dest = store8888(d);
        ++src;
        ++dest;
        ++mask;
    }
    _mm_empty();
}

static FASTCALL void
mmxCombineOutC (pixman_implementation_t *imp, pixman_op_t op,
		uint32_t *dest, const uint32_t *src, const uint32_t *mask, int width)
{
    const uint32_t *end = src + width;
    while (src < end) {
        __m64 a = load8888(*mask);
        __m64 s = load8888(*src);
        __m64 d = load8888(*dest);
        __m64 da = expand_alpha(d);
        da = negate(da);
        s = pix_multiply(s, a);
        s = pix_multiply(s, da);
        *dest = store8888(s);
        ++src;
        ++dest;
        ++mask;
    }
    _mm_empty();
}

static FASTCALL void
mmxCombineOutReverseC (pixman_implementation_t *imp, pixman_op_t op,
		       uint32_t *dest, const uint32_t *src, const uint32_t *mask, int width)
{
    const uint32_t *end = src + width;
    while (src < end) {
        __m64 a = load8888(*mask);
        __m64 s = load8888(*src);
        __m64 d = load8888(*dest);
        __m64 sa = expand_alpha(s);
        a = pix_multiply(a, sa);
        a = negate(a);
        d = pix_multiply(d, a);
        *dest = store8888(d);
        ++src;
        ++dest;
        ++mask;
    }
    _mm_empty();
}

static FASTCALL void
mmxCombineAtopC (pixman_implementation_t *imp, pixman_op_t op,
		 uint32_t *dest, const uint32_t *src, const uint32_t *mask, int width)
{
    const uint32_t *end = src + width;
    while (src < end) {
        __m64 a = load8888(*mask);
        __m64 s = load8888(*src);
        __m64 d = load8888(*dest);
        __m64 da = expand_alpha(d);
        __m64 sa = expand_alpha(s);
        s = pix_multiply(s, a);
        a = pix_multiply(a, sa);
        a = negate(a);
	d = pix_add_mul (d, a, s, da);
        *dest = store8888(d);
        ++src;
        ++dest;
        ++mask;
    }
    _mm_empty();
}

static FASTCALL void
mmxCombineAtopReverseC (pixman_implementation_t *imp, pixman_op_t op,
			uint32_t *dest, const uint32_t *src, const uint32_t *mask, int width)
{
    const uint32_t *end = src + width;
    while (src < end) {
        __m64 a = load8888(*mask);
        __m64 s = load8888(*src);
        __m64 d = load8888(*dest);
        __m64 da = expand_alpha(d);
        __m64 sa = expand_alpha(s);
        s = pix_multiply(s, a);
        a = pix_multiply(a, sa);
        da = negate(da);
	d = pix_add_mul (d, a, s, da);
        *dest = store8888(d);
        ++src;
        ++dest;
        ++mask;
    }
    _mm_empty();
}

static FASTCALL void
mmxCombineXorC (pixman_implementation_t *imp, pixman_op_t op,
		uint32_t *dest, const uint32_t *src, const uint32_t *mask, int width)
{
    const uint32_t *end = src + width;
    while (src < end) {
        __m64 a = load8888(*mask);
        __m64 s = load8888(*src);
        __m64 d = load8888(*dest);
        __m64 da = expand_alpha(d);
        __m64 sa = expand_alpha(s);
        s = pix_multiply(s, a);
        a = pix_multiply(a, sa);
        da = negate(da);
        a = negate(a);
	d = pix_add_mul (d, a, s, da);
        *dest = store8888(d);
        ++src;
        ++dest;
        ++mask;
    }
    _mm_empty();
}

static FASTCALL void
mmxCombineAddC (pixman_implementation_t *imp, pixman_op_t op,
		uint32_t *dest, const uint32_t *src, const uint32_t *mask, int width)
{
    const uint32_t *end = src + width;
    while (src < end) {
        __m64 a = load8888(*mask);
        __m64 s = load8888(*src);
        __m64 d = load8888(*dest);
        s = pix_multiply(s, a);
        d = pix_add(s, d);
        *dest = store8888(d);
        ++src;
        ++dest;
        ++mask;
    }
    _mm_empty();
}

/* ------------------ MMX code paths called from fbpict.c ----------------------- */

static void
fbCompositeSolid_nx8888mmx (pixman_implementation_t *imp,
			    pixman_op_t op,
			    pixman_image_t * pSrc,
			    pixman_image_t * pMask,
			    pixman_image_t * pDst,
			    int32_t	xSrc,
			    int32_t	ySrc,
			    int32_t	xMask,
			    int32_t	yMask,
			    int32_t	xDst,
			    int32_t	yDst,
			    int32_t	width,
			    int32_t	height)
{
    uint32_t	src;
    uint32_t	*dstLine, *dst;
    uint16_t	w;
    int	dstStride;
    __m64	vsrc, vsrca;

    CHECKPOINT();

    fbComposeGetSolid(pSrc, src, pDst->bits.format);

    if (src >> 24 == 0)
	return;

    fbComposeGetStart (pDst, xDst, yDst, uint32_t, dstStride, dstLine, 1);

    vsrc = load8888 (src);
    vsrca = expand_alpha (vsrc);

    while (height--)
    {
	dst = dstLine;
	dstLine += dstStride;
	w = width;

	CHECKPOINT();

	while (w && (unsigned long)dst & 7)
	{
	    *dst = store8888(over(vsrc, vsrca, load8888(*dst)));

	    w--;
	    dst++;
	}

	while (w >= 2)
	{
	    __m64 vdest;
	    __m64 dest0, dest1;

	    vdest = *(__m64 *)dst;

	    dest0 = over(vsrc, vsrca, expand8888(vdest, 0));
	    dest1 = over(vsrc, vsrca, expand8888(vdest, 1));

	    *(__m64 *)dst = pack8888(dest0, dest1);

	    dst += 2;
	    w -= 2;
	}

	CHECKPOINT();

	while (w)
	{
	    *dst = store8888(over(vsrc, vsrca, load8888(*dst)));

	    w--;
	    dst++;
	}
    }

    _mm_empty();
}

static void
fbCompositeSolid_nx0565mmx (pixman_implementation_t *imp,
			    pixman_op_t op,
			    pixman_image_t * pSrc,
			    pixman_image_t * pMask,
			    pixman_image_t * pDst,
			    int32_t	xSrc,
			    int32_t	ySrc,
			    int32_t	xMask,
			    int32_t	yMask,
			    int32_t	xDst,
			    int32_t	yDst,
			    int32_t	width,
			    int32_t	height)
{
    uint32_t	src;
    uint16_t	*dstLine, *dst;
    uint16_t	w;
    int	dstStride;
    __m64	vsrc, vsrca;

    CHECKPOINT();

    fbComposeGetSolid(pSrc, src, pDst->bits.format);

    if (src >> 24 == 0)
	return;

    fbComposeGetStart (pDst, xDst, yDst, uint16_t, dstStride, dstLine, 1);

    vsrc = load8888 (src);
    vsrca = expand_alpha (vsrc);

    while (height--)
    {
	dst = dstLine;
	dstLine += dstStride;
	w = width;

	CHECKPOINT();

	while (w && (unsigned long)dst & 7)
	{
	    uint64_t d = *dst;
	    __m64 vdest = expand565 (M64(d), 0);
	    vdest = pack565(over(vsrc, vsrca, vdest), vdest, 0);
	    *dst = UINT64(vdest);

	    w--;
	    dst++;
	}

	while (w >= 4)
	{
	    __m64 vdest;

	    vdest = *(__m64 *)dst;

	    vdest = pack565 (over(vsrc, vsrca, expand565(vdest, 0)), vdest, 0);
	    vdest = pack565 (over(vsrc, vsrca, expand565(vdest, 1)), vdest, 1);
	    vdest = pack565 (over(vsrc, vsrca, expand565(vdest, 2)), vdest, 2);
	    vdest = pack565 (over(vsrc, vsrca, expand565(vdest, 3)), vdest, 3);

	    *(__m64 *)dst = vdest;

	    dst += 4;
	    w -= 4;
	}

	CHECKPOINT();

	while (w)
	{
	    uint64_t d = *dst;
	    __m64 vdest = expand565 (M64(d), 0);
	    vdest = pack565(over(vsrc, vsrca, vdest), vdest, 0);
	    *dst = UINT64(vdest);

	    w--;
	    dst++;
	}
    }

    _mm_empty();
}

static void
fbCompositeSolidMask_nx8888x8888Cmmx (pixman_implementation_t *imp,
				      pixman_op_t op,
				      pixman_image_t * pSrc,
				      pixman_image_t * pMask,
				      pixman_image_t * pDst,
				      int32_t	xSrc,
				      int32_t	ySrc,
				      int32_t	xMask,
				      int32_t	yMask,
				      int32_t	xDst,
				      int32_t	yDst,
				      int32_t	width,
				      int32_t	height)
{
    uint32_t	src, srca;
    uint32_t	*dstLine;
    uint32_t	*maskLine;
    int	dstStride, maskStride;
    __m64	vsrc, vsrca;

    CHECKPOINT();

    fbComposeGetSolid(pSrc, src, pDst->bits.format);

    srca = src >> 24;
    if (srca == 0)
	return;

    fbComposeGetStart (pDst, xDst, yDst, uint32_t, dstStride, dstLine, 1);
    fbComposeGetStart (pMask, xMask, yMask, uint32_t, maskStride, maskLine, 1);

    vsrc = load8888(src);
    vsrca = expand_alpha(vsrc);

    while (height--)
    {
	int twidth = width;
	uint32_t *p = (uint32_t *)maskLine;
	uint32_t *q = (uint32_t *)dstLine;

	while (twidth && (unsigned long)q & 7)
	{
	    uint32_t m = *(uint32_t *)p;

	    if (m)
	    {
		__m64 vdest = load8888(*q);
		vdest = in_over(vsrc, vsrca, load8888(m), vdest);
		*q = store8888(vdest);
	    }

	    twidth--;
	    p++;
	    q++;
	}

	while (twidth >= 2)
	{
	    uint32_t m0, m1;
	    m0 = *p;
	    m1 = *(p + 1);

	    if (m0 | m1)
	    {
		__m64 dest0, dest1;
		__m64 vdest = *(__m64 *)q;

		dest0 = in_over(vsrc, vsrca, load8888(m0),
				expand8888 (vdest, 0));
		dest1 = in_over(vsrc, vsrca, load8888(m1),
				expand8888 (vdest, 1));

		*(__m64 *)q = pack8888(dest0, dest1);
	    }

	    p += 2;
	    q += 2;
	    twidth -= 2;
	}

	while (twidth)
	{
	    uint32_t m = *(uint32_t *)p;

	    if (m)
	    {
		__m64 vdest = load8888(*q);
		vdest = in_over(vsrc, vsrca, load8888(m), vdest);
		*q = store8888(vdest);
	    }

	    twidth--;
	    p++;
	    q++;
	}

	dstLine += dstStride;
	maskLine += maskStride;
    }

    _mm_empty();
}

static void
fbCompositeSrc_8888x8x8888mmx (pixman_implementation_t *imp,
			       pixman_op_t op,
			       pixman_image_t * pSrc,
			       pixman_image_t * pMask,
			       pixman_image_t * pDst,
			       int32_t	xSrc,
			       int32_t	ySrc,
			       int32_t      xMask,
			       int32_t      yMask,
			       int32_t      xDst,
			       int32_t      yDst,
			       int32_t     width,
			       int32_t     height)
{
    uint32_t	*dstLine, *dst;
    uint32_t	*srcLine, *src;
    uint32_t	mask;
    __m64	vmask;
    int	dstStride, srcStride;
    uint16_t	w;
    __m64  srca;

    CHECKPOINT();

    fbComposeGetStart (pDst, xDst, yDst, uint32_t, dstStride, dstLine, 1);
    fbComposeGetStart (pSrc, xSrc, ySrc, uint32_t, srcStride, srcLine, 1);

    fbComposeGetSolid (pMask, mask, pDst->bits.format);
    mask = mask | mask >> 8 | mask >> 16 | mask >> 24;
    vmask = load8888 (mask);
    srca = MC(4x00ff);

    while (height--)
    {
	dst = dstLine;
	dstLine += dstStride;
	src = srcLine;
	srcLine += srcStride;
	w = width;

	while (w && (unsigned long)dst & 7)
	{
	    __m64 s = load8888 (*src);
	    __m64 d = load8888 (*dst);

	    *dst = store8888 (in_over (s, expand_alpha (s), vmask, d));

	    w--;
	    dst++;
	    src++;
	}

	while (w >= 2)
	{
	    __m64 vs = *(__m64 *)src;
	    __m64 vd = *(__m64 *)dst;
	    __m64 vsrc0 = expand8888 (vs, 0);
	    __m64 vsrc1 = expand8888 (vs, 1);

	    *(__m64 *)dst = pack8888 (
		in_over (vsrc0, expand_alpha (vsrc0), vmask, expand8888 (vd, 0)),
		in_over (vsrc1, expand_alpha (vsrc1), vmask, expand8888 (vd, 1)));

	    w -= 2;
	    dst += 2;
	    src += 2;
	}

	while (w)
	{
	    __m64 s = load8888 (*src);
	    __m64 d = load8888 (*dst);

	    *dst = store8888 (in_over (s, expand_alpha (s), vmask, d));

	    w--;
	    dst++;
	    src++;
	}
    }

    _mm_empty();
}

static void
fbCompositeSrc_x888xnx8888mmx (pixman_implementation_t *imp,
			       pixman_op_t op,
			       pixman_image_t * pSrc,
			       pixman_image_t * pMask,
			       pixman_image_t * pDst,
			       int32_t	xSrc,
			       int32_t	ySrc,
			       int32_t      xMask,
			       int32_t      yMask,
			       int32_t      xDst,
			       int32_t      yDst,
			       int32_t     width,
			       int32_t     height)
{
    uint32_t	*dstLine, *dst;
    uint32_t	*srcLine, *src;
    uint32_t	mask;
    __m64	vmask;
    int	dstStride, srcStride;
    uint16_t	w;
    __m64  srca;

    CHECKPOINT();

    fbComposeGetStart (pDst, xDst, yDst, uint32_t, dstStride, dstLine, 1);
    fbComposeGetStart (pSrc, xSrc, ySrc, uint32_t, srcStride, srcLine, 1);
    fbComposeGetSolid (pMask, mask, pDst->bits.format);

    mask = mask | mask >> 8 | mask >> 16 | mask >> 24;
    vmask = load8888 (mask);
    srca = MC(4x00ff);

    while (height--)
    {
	dst = dstLine;
	dstLine += dstStride;
	src = srcLine;
	srcLine += srcStride;
	w = width;

	while (w && (unsigned long)dst & 7)
	{
	    __m64 s = load8888 (*src | 0xff000000);
	    __m64 d = load8888 (*dst);

	    *dst = store8888 (in_over (s, srca, vmask, d));

	    w--;
	    dst++;
	    src++;
	}

	while (w >= 16)
	{
	    __m64 vd0 = *(__m64 *)(dst + 0);
	    __m64 vd1 = *(__m64 *)(dst + 2);
	    __m64 vd2 = *(__m64 *)(dst + 4);
	    __m64 vd3 = *(__m64 *)(dst + 6);
	    __m64 vd4 = *(__m64 *)(dst + 8);
	    __m64 vd5 = *(__m64 *)(dst + 10);
	    __m64 vd6 = *(__m64 *)(dst + 12);
	    __m64 vd7 = *(__m64 *)(dst + 14);

	    __m64 vs0 = *(__m64 *)(src + 0);
	    __m64 vs1 = *(__m64 *)(src + 2);
	    __m64 vs2 = *(__m64 *)(src + 4);
	    __m64 vs3 = *(__m64 *)(src + 6);
	    __m64 vs4 = *(__m64 *)(src + 8);
	    __m64 vs5 = *(__m64 *)(src + 10);
	    __m64 vs6 = *(__m64 *)(src + 12);
	    __m64 vs7 = *(__m64 *)(src + 14);

	    vd0 = pack8888 (
		in_over (expandx888 (vs0, 0), srca, vmask, expand8888 (vd0, 0)),
		in_over (expandx888 (vs0, 1), srca, vmask, expand8888 (vd0, 1)));

	    vd1 = pack8888 (
		in_over (expandx888 (vs1, 0), srca, vmask, expand8888 (vd1, 0)),
		in_over (expandx888 (vs1, 1), srca, vmask, expand8888 (vd1, 1)));

	    vd2 = pack8888 (
		in_over (expandx888 (vs2, 0), srca, vmask, expand8888 (vd2, 0)),
		in_over (expandx888 (vs2, 1), srca, vmask, expand8888 (vd2, 1)));

	    vd3 = pack8888 (
		in_over (expandx888 (vs3, 0), srca, vmask, expand8888 (vd3, 0)),
		in_over (expandx888 (vs3, 1), srca, vmask, expand8888 (vd3, 1)));

	    vd4 = pack8888 (
		in_over (expandx888 (vs4, 0), srca, vmask, expand8888 (vd4, 0)),
		in_over (expandx888 (vs4, 1), srca, vmask, expand8888 (vd4, 1)));

	    vd5 = pack8888 (
		in_over (expandx888 (vs5, 0), srca, vmask, expand8888 (vd5, 0)),
		in_over (expandx888 (vs5, 1), srca, vmask, expand8888 (vd5, 1)));

            vd6 = pack8888 (
		in_over (expandx888 (vs6, 0), srca, vmask, expand8888 (vd6, 0)),
		in_over (expandx888 (vs6, 1), srca, vmask, expand8888 (vd6, 1)));

	    vd7 = pack8888 (
		in_over (expandx888 (vs7, 0), srca, vmask, expand8888 (vd7, 0)),
		in_over (expandx888 (vs7, 1), srca, vmask, expand8888 (vd7, 1)));

	    *(__m64 *)(dst + 0) = vd0;
	    *(__m64 *)(dst + 2) = vd1;
	    *(__m64 *)(dst + 4) = vd2;
	    *(__m64 *)(dst + 6) = vd3;
	    *(__m64 *)(dst + 8) = vd4;
	    *(__m64 *)(dst + 10) = vd5;
	    *(__m64 *)(dst + 12) = vd6;
	    *(__m64 *)(dst + 14) = vd7;

	    w -= 16;
	    dst += 16;
	    src += 16;
	}

	while (w)
	{
	    __m64 s = load8888 (*src | 0xff000000);
	    __m64 d = load8888 (*dst);

	    *dst = store8888 (in_over (s, srca, vmask, d));

	    w--;
	    dst++;
	    src++;
	}
    }

    _mm_empty();
}

static void
fbCompositeSrc_8888x8888mmx (pixman_implementation_t *imp,
			     pixman_op_t op,
			     pixman_image_t * pSrc,
			     pixman_image_t * pMask,
			     pixman_image_t * pDst,
			     int32_t	xSrc,
			     int32_t	ySrc,
			     int32_t      xMask,
			     int32_t      yMask,
			     int32_t      xDst,
			     int32_t      yDst,
			     int32_t     width,
			     int32_t     height)
{
    uint32_t	*dstLine, *dst;
    uint32_t	*srcLine, *src;
    uint32_t    s;
    int	dstStride, srcStride;
    uint8_t     a;
    uint16_t	w;

    CHECKPOINT();

    fbComposeGetStart (pDst, xDst, yDst, uint32_t, dstStride, dstLine, 1);
    fbComposeGetStart (pSrc, xSrc, ySrc, uint32_t, srcStride, srcLine, 1);

    while (height--)
    {
	dst = dstLine;
	dstLine += dstStride;
	src = srcLine;
	srcLine += srcStride;
	w = width;

	while (w--)
	{
	    s = *src++;
	    a = s >> 24;
	    if (a == 0xff)
		*dst = s;
	    else if (s) {
		__m64 ms, sa;
		ms = load8888(s);
		sa = expand_alpha(ms);
		*dst = store8888(over(ms, sa, load8888(*dst)));
	    }
	    dst++;
	}
    }
    _mm_empty();
}

static void
fbCompositeSrc_8888x0565mmx (pixman_implementation_t *imp,
			     pixman_op_t op,
			     pixman_image_t * pSrc,
			     pixman_image_t * pMask,
			     pixman_image_t * pDst,
			     int32_t      xSrc,
			     int32_t      ySrc,
			     int32_t      xMask,
			     int32_t      yMask,
			     int32_t      xDst,
			     int32_t      yDst,
			     int32_t     width,
			     int32_t     height)
{
    uint16_t	*dstLine, *dst;
    uint32_t	*srcLine, *src;
    int	dstStride, srcStride;
    uint16_t	w;

    CHECKPOINT();

    fbComposeGetStart (pDst, xDst, yDst, uint16_t, dstStride, dstLine, 1);
    fbComposeGetStart (pSrc, xSrc, ySrc, uint32_t, srcStride, srcLine, 1);

#if 0
    /* FIXME */
    assert (pSrc->pDrawable == pMask->pDrawable);
#endif

    while (height--)
    {
	dst = dstLine;
	dstLine += dstStride;
	src = srcLine;
	srcLine += srcStride;
	w = width;

	CHECKPOINT();

	while (w && (unsigned long)dst & 7)
	{
	    __m64 vsrc = load8888 (*src);
	    uint64_t d = *dst;
	    __m64 vdest = expand565 (M64(d), 0);

	    vdest = pack565(over(vsrc, expand_alpha(vsrc), vdest), vdest, 0);

	    *dst = UINT64(vdest);

	    w--;
	    dst++;
	    src++;
	}

	CHECKPOINT();

	while (w >= 4)
	{
	    __m64 vsrc0, vsrc1, vsrc2, vsrc3;
	    __m64 vdest;

	    vsrc0 = load8888(*(src + 0));
	    vsrc1 = load8888(*(src + 1));
	    vsrc2 = load8888(*(src + 2));
	    vsrc3 = load8888(*(src + 3));

	    vdest = *(__m64 *)dst;

	    vdest = pack565(over(vsrc0, expand_alpha(vsrc0), expand565(vdest, 0)), vdest, 0);
	    vdest = pack565(over(vsrc1, expand_alpha(vsrc1), expand565(vdest, 1)), vdest, 1);
	    vdest = pack565(over(vsrc2, expand_alpha(vsrc2), expand565(vdest, 2)), vdest, 2);
	    vdest = pack565(over(vsrc3, expand_alpha(vsrc3), expand565(vdest, 3)), vdest, 3);

	    *(__m64 *)dst = vdest;

	    w -= 4;
	    dst += 4;
	    src += 4;
	}

	CHECKPOINT();

	while (w)
	{
	    __m64 vsrc = load8888 (*src);
	    uint64_t d = *dst;
	    __m64 vdest = expand565 (M64(d), 0);

	    vdest = pack565(over(vsrc, expand_alpha(vsrc), vdest), vdest, 0);

	    *dst = UINT64(vdest);

	    w--;
	    dst++;
	    src++;
	}
    }

    _mm_empty();
}

static void
fbCompositeSolidMask_nx8x8888mmx (pixman_implementation_t *imp,
				  pixman_op_t op,
				  pixman_image_t * pSrc,
				  pixman_image_t * pMask,
				  pixman_image_t * pDst,
				  int32_t      xSrc,
				  int32_t      ySrc,
				  int32_t      xMask,
				  int32_t      yMask,
				  int32_t      xDst,
				  int32_t      yDst,
				  int32_t     width,
				  int32_t     height)
{
    uint32_t	src, srca;
    uint32_t	*dstLine, *dst;
    uint8_t	*maskLine, *mask;
    int	dstStride, maskStride;
    uint16_t	w;
    __m64	vsrc, vsrca;
    uint64_t	srcsrc;

    CHECKPOINT();

    fbComposeGetSolid(pSrc, src, pDst->bits.format);

    srca = src >> 24;
    if (srca == 0)
	return;

    srcsrc = (uint64_t)src << 32 | src;

    fbComposeGetStart (pDst, xDst, yDst, uint32_t, dstStride, dstLine, 1);
    fbComposeGetStart (pMask, xMask, yMask, uint8_t, maskStride, maskLine, 1);

    vsrc = load8888 (src);
    vsrca = expand_alpha (vsrc);

    while (height--)
    {
	dst = dstLine;
	dstLine += dstStride;
	mask = maskLine;
	maskLine += maskStride;
	w = width;

	CHECKPOINT();

	while (w && (unsigned long)dst & 7)
	{
	    uint64_t m = *mask;

	    if (m)
	    {
		__m64 vdest = in_over(vsrc, vsrca, expand_alpha_rev (M64(m)), load8888(*dst));
		*dst = store8888(vdest);
	    }

	    w--;
	    mask++;
	    dst++;
	}

	CHECKPOINT();

	while (w >= 2)
	{
	    uint64_t m0, m1;
	    m0 = *mask;
	    m1 = *(mask + 1);

	    if (srca == 0xff && (m0 & m1) == 0xff)
	    {
		*(uint64_t *)dst = srcsrc;
	    }
	    else if (m0 | m1)
	    {
		__m64 vdest;
		__m64 dest0, dest1;

		vdest = *(__m64 *)dst;

		dest0 = in_over(vsrc, vsrca, expand_alpha_rev (M64(m0)), expand8888(vdest, 0));
		dest1 = in_over(vsrc, vsrca, expand_alpha_rev (M64(m1)), expand8888(vdest, 1));

		*(__m64 *)dst = pack8888(dest0, dest1);
	    }

	    mask += 2;
	    dst += 2;
	    w -= 2;
	}

	CHECKPOINT();

	while (w)
	{
	    uint64_t m = *mask;

	    if (m)
	    {
		__m64 vdest = load8888(*dst);
		vdest = in_over(vsrc, vsrca, expand_alpha_rev (M64(m)), vdest);
		*dst = store8888(vdest);
	    }

	    w--;
	    mask++;
	    dst++;
	}
    }

    _mm_empty();
}

pixman_bool_t
pixman_fill_mmx (uint32_t *bits,
		 int stride,
		 int bpp,
		 int x,
		 int y,
		 int width,
		 int height,
		 uint32_t xor)
{
    uint64_t	fill;
    __m64	vfill;
    uint32_t	byte_width;
    uint8_t	*byte_line;
#ifdef __GNUC__
    __m64	v1, v2, v3, v4, v5, v6, v7;
#endif

    if (bpp != 16 && bpp != 32 && bpp != 8)
	return FALSE;

    if (bpp == 16 && (xor >> 16 != (xor & 0xffff)))
	return FALSE;

    if (bpp == 8 &&
	((xor >> 16 != (xor & 0xffff)) ||
	 (xor >> 24 != (xor & 0x00ff) >> 16)))
    {
	return FALSE;
    }
    
    if (bpp == 8)
    {
	stride = stride * (int) sizeof (uint32_t) / 1;
	byte_line = (uint8_t *)(((uint8_t *)bits) + stride * y + x);
	byte_width = width;
	stride *= 1;
    }
    else if (bpp == 16)
    {
	stride = stride * (int) sizeof (uint32_t) / 2;
	byte_line = (uint8_t *)(((uint16_t *)bits) + stride * y + x);
	byte_width = 2 * width;
	stride *= 2;
    }
    else
    {
	stride = stride * (int) sizeof (uint32_t) / 4;
	byte_line = (uint8_t *)(((uint32_t *)bits) + stride * y + x);
	byte_width = 4 * width;
	stride *= 4;
    }

    fill = ((uint64_t)xor << 32) | xor;
    vfill = M64(fill);

#ifdef __GNUC__
    __asm__ (
	"movq		%7,	%0\n"
	"movq		%7,	%1\n"
	"movq		%7,	%2\n"
	"movq		%7,	%3\n"
	"movq		%7,	%4\n"
	"movq		%7,	%5\n"
	"movq		%7,	%6\n"
	: "=y" (v1), "=y" (v2), "=y" (v3),
	  "=y" (v4), "=y" (v5), "=y" (v6), "=y" (v7)
	: "y" (vfill));
#endif

    while (height--)
    {
	int w;
	uint8_t *d = byte_line;
	byte_line += stride;
	w = byte_width;

	while (w >= 1 && ((unsigned long)d & 1))
	{
	    *(uint8_t *)d = (xor & 0xff);
	    w--;
	    d++;
	}
	
	while (w >= 2 && ((unsigned long)d & 3))
	{
	    *(uint16_t *)d = xor;
	    w -= 2;
	    d += 2;
	}

	while (w >= 4 && ((unsigned long)d & 7))
	{
	    *(uint32_t *)d = xor;

	    w -= 4;
	    d += 4;
	}

	while (w >= 64)
	{
#ifdef __GNUC__
	    __asm__ (
		"movq	%1,	  (%0)\n"
		"movq	%2,	 8(%0)\n"
		"movq	%3,	16(%0)\n"
		"movq	%4,	24(%0)\n"
		"movq	%5,	32(%0)\n"
		"movq	%6,	40(%0)\n"
		"movq	%7,	48(%0)\n"
		"movq	%8,	56(%0)\n"
		:
		: "r" (d),
		  "y" (vfill), "y" (v1), "y" (v2), "y" (v3),
		  "y" (v4), "y" (v5), "y" (v6), "y" (v7)
		: "memory");
#else
	    *(__m64*) (d +  0) = vfill;
	    *(__m64*) (d +  8) = vfill;
	    *(__m64*) (d + 16) = vfill;
	    *(__m64*) (d + 24) = vfill;
	    *(__m64*) (d + 32) = vfill;
	    *(__m64*) (d + 40) = vfill;
	    *(__m64*) (d + 48) = vfill;
	    *(__m64*) (d + 56) = vfill;
#endif
	    w -= 64;
	    d += 64;
	}

	while (w >= 4)
	{
	    *(uint32_t *)d = xor;

	    w -= 4;
	    d += 4;
	}
	while (w >= 2)
	{
	    *(uint16_t *)d = xor;
	    w -= 2;
	    d += 2;
	}
	while (w >= 1)
	{
	    *(uint8_t *)d = (xor & 0xff);
	    w--;
	    d++;
	}
	
    }

    _mm_empty();
    return TRUE;
}

static void
fbCompositeSolidMaskSrc_nx8x8888mmx (pixman_implementation_t *imp,
				     pixman_op_t op,
				     pixman_image_t * pSrc,
				     pixman_image_t * pMask,
				     pixman_image_t * pDst,
				     int32_t      xSrc,
				     int32_t      ySrc,
				     int32_t      xMask,
				     int32_t      yMask,
				     int32_t      xDst,
				     int32_t      yDst,
				     int32_t     width,
				     int32_t     height)
{
    uint32_t	src, srca;
    uint32_t	*dstLine, *dst;
    uint8_t	*maskLine, *mask;
    int	dstStride, maskStride;
    uint16_t	w;
    __m64	vsrc, vsrca;
    uint64_t	srcsrc;

    CHECKPOINT();

    fbComposeGetSolid(pSrc, src, pDst->bits.format);

    srca = src >> 24;
    if (srca == 0)
    {
	pixman_fill_mmx (pDst->bits.bits, pDst->bits.rowstride, PIXMAN_FORMAT_BPP (pDst->bits.format),
			 xDst, yDst, width, height, 0);
	return;
    }

    srcsrc = (uint64_t)src << 32 | src;

    fbComposeGetStart (pDst, xDst, yDst, uint32_t, dstStride, dstLine, 1);
    fbComposeGetStart (pMask, xMask, yMask, uint8_t, maskStride, maskLine, 1);

    vsrc = load8888 (src);
    vsrca = expand_alpha (vsrc);

    while (height--)
    {
	dst = dstLine;
	dstLine += dstStride;
	mask = maskLine;
	maskLine += maskStride;
	w = width;

	CHECKPOINT();

	while (w && (unsigned long)dst & 7)
	{
	    uint64_t m = *mask;

	    if (m)
	    {
		__m64 vdest = in(vsrc, expand_alpha_rev (M64(m)));
		*dst = store8888(vdest);
	    }
	    else
	    {
		*dst = 0;
	    }

	    w--;
	    mask++;
	    dst++;
	}

	CHECKPOINT();

	while (w >= 2)
	{
	    uint64_t m0, m1;
	    m0 = *mask;
	    m1 = *(mask + 1);

	    if (srca == 0xff && (m0 & m1) == 0xff)
	    {
		*(uint64_t *)dst = srcsrc;
	    }
	    else if (m0 | m1)
	    {
		__m64 vdest;
		__m64 dest0, dest1;

		vdest = *(__m64 *)dst;

		dest0 = in(vsrc, expand_alpha_rev (M64(m0)));
		dest1 = in(vsrc, expand_alpha_rev (M64(m1)));

		*(__m64 *)dst = pack8888(dest0, dest1);
	    }
	    else
	    {
		*(uint64_t *)dst = 0;
	    }

	    mask += 2;
	    dst += 2;
	    w -= 2;
	}

	CHECKPOINT();

	while (w)
	{
	    uint64_t m = *mask;

	    if (m)
	    {
		__m64 vdest = load8888(*dst);
		vdest = in(vsrc, expand_alpha_rev (M64(m)));
		*dst = store8888(vdest);
	    }
	    else
	    {
		*dst = 0;
	    }

	    w--;
	    mask++;
	    dst++;
	}
    }

    _mm_empty();
}

static void
fbCompositeSolidMask_nx8x0565mmx (pixman_implementation_t *imp,
				  pixman_op_t op,
				  pixman_image_t * pSrc,
				  pixman_image_t * pMask,
				  pixman_image_t * pDst,
				  int32_t      xSrc,
				  int32_t      ySrc,
				  int32_t      xMask,
				  int32_t      yMask,
				  int32_t      xDst,
				  int32_t      yDst,
				  int32_t     width,
				  int32_t     height)
{
    uint32_t	src, srca;
    uint16_t	*dstLine, *dst;
    uint8_t	*maskLine, *mask;
    int	dstStride, maskStride;
    uint16_t	w;
    __m64	vsrc, vsrca, tmp;
    uint64_t srcsrcsrcsrc, src16;

    CHECKPOINT();

    fbComposeGetSolid(pSrc, src, pDst->bits.format);

    srca = src >> 24;
    if (srca == 0)
	return;

    fbComposeGetStart (pDst, xDst, yDst, uint16_t, dstStride, dstLine, 1);
    fbComposeGetStart (pMask, xMask, yMask, uint8_t, maskStride, maskLine, 1);

    vsrc = load8888 (src);
    vsrca = expand_alpha (vsrc);

    tmp = pack565(vsrc, _mm_setzero_si64(), 0);
    src16 = UINT64(tmp);

    srcsrcsrcsrc = (uint64_t)src16 << 48 | (uint64_t)src16 << 32 |
	(uint64_t)src16 << 16 | (uint64_t)src16;

    while (height--)
    {
	dst = dstLine;
	dstLine += dstStride;
	mask = maskLine;
	maskLine += maskStride;
	w = width;

	CHECKPOINT();

	while (w && (unsigned long)dst & 7)
	{
	    uint64_t m = *mask;

	    if (m)
	    {
		uint64_t d = *dst;
		__m64 vd = M64(d);
		__m64 vdest = in_over(vsrc, vsrca, expand_alpha_rev (M64 (m)), expand565(vd, 0));
		vd = pack565(vdest, _mm_setzero_si64(), 0);
		*dst = UINT64(vd);
	    }

	    w--;
	    mask++;
	    dst++;
	}

	CHECKPOINT();

	while (w >= 4)
	{
	    uint64_t m0, m1, m2, m3;
	    m0 = *mask;
	    m1 = *(mask + 1);
	    m2 = *(mask + 2);
	    m3 = *(mask + 3);

	    if (srca == 0xff && (m0 & m1 & m2 & m3) == 0xff)
	    {
		*(uint64_t *)dst = srcsrcsrcsrc;
	    }
	    else if (m0 | m1 | m2 | m3)
	    {
		__m64 vdest;
		__m64 vm0, vm1, vm2, vm3;

		vdest = *(__m64 *)dst;

		vm0 = M64(m0);
		vdest = pack565(in_over(vsrc, vsrca, expand_alpha_rev(vm0), expand565(vdest, 0)), vdest, 0);
		vm1 = M64(m1);
		vdest = pack565(in_over(vsrc, vsrca, expand_alpha_rev(vm1), expand565(vdest, 1)), vdest, 1);
		vm2 = M64(m2);
		vdest = pack565(in_over(vsrc, vsrca, expand_alpha_rev(vm2), expand565(vdest, 2)), vdest, 2);
		vm3 = M64(m3);
		vdest = pack565(in_over(vsrc, vsrca, expand_alpha_rev(vm3), expand565(vdest, 3)), vdest, 3);

		*(__m64 *)dst = vdest;
	    }

	    w -= 4;
	    mask += 4;
	    dst += 4;
	}

	CHECKPOINT();

	while (w)
	{
	    uint64_t m = *mask;

	    if (m)
	    {
		uint64_t d = *dst;
		__m64 vd = M64(d);
		__m64 vdest = in_over(vsrc, vsrca, expand_alpha_rev (M64(m)), expand565(vd, 0));
		vd = pack565(vdest, _mm_setzero_si64(), 0);
		*dst = UINT64(vd);
	    }

	    w--;
	    mask++;
	    dst++;
	}
    }

    _mm_empty();
}

static void
fbCompositeSrc_8888RevNPx0565mmx (pixman_implementation_t *imp,
				  pixman_op_t op,
				  pixman_image_t * pSrc,
				  pixman_image_t * pMask,
				  pixman_image_t * pDst,
				  int32_t      xSrc,
				  int32_t      ySrc,
				  int32_t      xMask,
				  int32_t      yMask,
				  int32_t      xDst,
				  int32_t      yDst,
				  int32_t     width,
				  int32_t     height)
{
    uint16_t	*dstLine, *dst;
    uint32_t	*srcLine, *src;
    int	dstStride, srcStride;
    uint16_t	w;

    CHECKPOINT();

    fbComposeGetStart (pDst, xDst, yDst, uint16_t, dstStride, dstLine, 1);
    fbComposeGetStart (pSrc, xSrc, ySrc, uint32_t, srcStride, srcLine, 1);

#if 0
    /* FIXME */
    assert (pSrc->pDrawable == pMask->pDrawable);
#endif

    while (height--)
    {
	dst = dstLine;
	dstLine += dstStride;
	src = srcLine;
	srcLine += srcStride;
	w = width;

	CHECKPOINT();

	while (w && (unsigned long)dst & 7)
	{
	    __m64 vsrc = load8888 (*src);
	    uint64_t d = *dst;
	    __m64 vdest = expand565 (M64(d), 0);

	    vdest = pack565(over_rev_non_pre(vsrc, vdest), vdest, 0);

	    *dst = UINT64(vdest);

	    w--;
	    dst++;
	    src++;
	}

	CHECKPOINT();

	while (w >= 4)
	{
	    uint32_t s0, s1, s2, s3;
	    unsigned char a0, a1, a2, a3;

	    s0 = *src;
	    s1 = *(src + 1);
	    s2 = *(src + 2);
	    s3 = *(src + 3);

	    a0 = (s0 >> 24);
	    a1 = (s1 >> 24);
	    a2 = (s2 >> 24);
	    a3 = (s3 >> 24);

	    if ((a0 & a1 & a2 & a3) == 0xFF)
	    {
		__m64 vdest;
		vdest = pack565(invert_colors(load8888(s0)), _mm_setzero_si64(), 0);
		vdest = pack565(invert_colors(load8888(s1)), vdest, 1);
		vdest = pack565(invert_colors(load8888(s2)), vdest, 2);
		vdest = pack565(invert_colors(load8888(s3)), vdest, 3);

		*(__m64 *)dst = vdest;
	    }
	    else if (s0 | s1 | s2 | s3)
	    {
		__m64 vdest = *(__m64 *)dst;

		vdest = pack565(over_rev_non_pre(load8888(s0), expand565(vdest, 0)), vdest, 0);
	        vdest = pack565(over_rev_non_pre(load8888(s1), expand565(vdest, 1)), vdest, 1);
		vdest = pack565(over_rev_non_pre(load8888(s2), expand565(vdest, 2)), vdest, 2);
		vdest = pack565(over_rev_non_pre(load8888(s3), expand565(vdest, 3)), vdest, 3);

		*(__m64 *)dst = vdest;
	    }

	    w -= 4;
	    dst += 4;
	    src += 4;
	}

	CHECKPOINT();

	while (w)
	{
	    __m64 vsrc = load8888 (*src);
	    uint64_t d = *dst;
	    __m64 vdest = expand565 (M64(d), 0);

	    vdest = pack565(over_rev_non_pre(vsrc, vdest), vdest, 0);

	    *dst = UINT64(vdest);

	    w--;
	    dst++;
	    src++;
	}
    }

    _mm_empty();
}

/* "8888RevNP" is GdkPixbuf's format: ABGR, non premultiplied */

static void
fbCompositeSrc_8888RevNPx8888mmx (pixman_implementation_t *imp,
				  pixman_op_t op,
				  pixman_image_t * pSrc,
				  pixman_image_t * pMask,
				  pixman_image_t * pDst,
				  int32_t      xSrc,
				  int32_t      ySrc,
				  int32_t      xMask,
				  int32_t      yMask,
				  int32_t      xDst,
				  int32_t      yDst,
				  int32_t     width,
				  int32_t     height)
{
    uint32_t	*dstLine, *dst;
    uint32_t	*srcLine, *src;
    int	dstStride, srcStride;
    uint16_t	w;

    CHECKPOINT();

    fbComposeGetStart (pDst, xDst, yDst, uint32_t, dstStride, dstLine, 1);
    fbComposeGetStart (pSrc, xSrc, ySrc, uint32_t, srcStride, srcLine, 1);

#if 0
    /* FIXME */
    assert (pSrc->pDrawable == pMask->pDrawable);
#endif

    while (height--)
    {
	dst = dstLine;
	dstLine += dstStride;
	src = srcLine;
	srcLine += srcStride;
	w = width;

	while (w && (unsigned long)dst & 7)
	{
	    __m64 s = load8888 (*src);
	    __m64 d = load8888 (*dst);

	    *dst = store8888 (over_rev_non_pre (s, d));

	    w--;
	    dst++;
	    src++;
	}

	while (w >= 2)
	{
	    uint64_t s0, s1;
	    unsigned char a0, a1;
	    __m64 d0, d1;

	    s0 = *src;
	    s1 = *(src + 1);

	    a0 = (s0 >> 24);
	    a1 = (s1 >> 24);

	    if ((a0 & a1) == 0xFF)
	    {
		d0 = invert_colors(load8888(s0));
		d1 = invert_colors(load8888(s1));

		*(__m64 *)dst = pack8888 (d0, d1);
	    }
	    else if (s0 | s1)
	    {
		__m64 vdest = *(__m64 *)dst;

		d0 = over_rev_non_pre (load8888(s0), expand8888 (vdest, 0));
		d1 = over_rev_non_pre (load8888(s1), expand8888 (vdest, 1));

		*(__m64 *)dst = pack8888 (d0, d1);
	    }

	    w -= 2;
	    dst += 2;
	    src += 2;
	}

	while (w)
	{
	    __m64 s = load8888 (*src);
	    __m64 d = load8888 (*dst);

	    *dst = store8888 (over_rev_non_pre (s, d));

	    w--;
	    dst++;
	    src++;
	}
    }

    _mm_empty();
}

static void
fbCompositeSolidMask_nx8888x0565Cmmx (pixman_implementation_t *imp,
				      pixman_op_t op,
				      pixman_image_t * pSrc,
				      pixman_image_t * pMask,
				      pixman_image_t * pDst,
				      int32_t      xSrc,
				      int32_t      ySrc,
				      int32_t      xMask,
				      int32_t      yMask,
				      int32_t      xDst,
				      int32_t      yDst,
				      int32_t     width,
				      int32_t     height)
{
    uint32_t	src, srca;
    uint16_t	*dstLine;
    uint32_t	*maskLine;
    int	dstStride, maskStride;
    __m64  vsrc, vsrca;

    CHECKPOINT();

    fbComposeGetSolid(pSrc, src, pDst->bits.format);

    srca = src >> 24;
    if (srca == 0)
	return;

    fbComposeGetStart (pDst, xDst, yDst, uint16_t, dstStride, dstLine, 1);
    fbComposeGetStart (pMask, xMask, yMask, uint32_t, maskStride, maskLine, 1);

    vsrc = load8888 (src);
    vsrca = expand_alpha (vsrc);

    while (height--)
    {
	int twidth = width;
	uint32_t *p = (uint32_t *)maskLine;
	uint16_t *q = (uint16_t *)dstLine;

	while (twidth && ((unsigned long)q & 7))
	{
	    uint32_t m = *(uint32_t *)p;

	    if (m)
	    {
		uint64_t d = *q;
		__m64 vdest = expand565 (M64(d), 0);
		vdest = pack565 (in_over (vsrc, vsrca, load8888 (m), vdest), vdest, 0);
		*q = UINT64(vdest);
	    }

	    twidth--;
	    p++;
	    q++;
	}

	while (twidth >= 4)
	{
	    uint32_t m0, m1, m2, m3;

	    m0 = *p;
	    m1 = *(p + 1);
	    m2 = *(p + 2);
	    m3 = *(p + 3);

	    if ((m0 | m1 | m2 | m3))
	    {
		__m64 vdest = *(__m64 *)q;

		vdest = pack565(in_over(vsrc, vsrca, load8888(m0), expand565(vdest, 0)), vdest, 0);
		vdest = pack565(in_over(vsrc, vsrca, load8888(m1), expand565(vdest, 1)), vdest, 1);
		vdest = pack565(in_over(vsrc, vsrca, load8888(m2), expand565(vdest, 2)), vdest, 2);
		vdest = pack565(in_over(vsrc, vsrca, load8888(m3), expand565(vdest, 3)), vdest, 3);

		*(__m64 *)q = vdest;
	    }
	    twidth -= 4;
	    p += 4;
	    q += 4;
	}

	while (twidth)
	{
	    uint32_t m;

	    m = *(uint32_t *)p;
	    if (m)
	    {
		uint64_t d = *q;
		__m64 vdest = expand565(M64(d), 0);
		vdest = pack565 (in_over(vsrc, vsrca, load8888(m), vdest), vdest, 0);
		*q = UINT64(vdest);
	    }

	    twidth--;
	    p++;
	    q++;
	}

	maskLine += maskStride;
	dstLine += dstStride;
    }

    _mm_empty ();
}

static void
fbCompositeIn_nx8x8mmx (pixman_implementation_t *imp,
			pixman_op_t op,
			pixman_image_t * pSrc,
			pixman_image_t * pMask,
			pixman_image_t * pDst,
			int32_t      xSrc,
			int32_t      ySrc,
			int32_t      xMask,
			int32_t      yMask,
			int32_t      xDst,
			int32_t      yDst,
			int32_t     width,
			int32_t     height)
{
    uint8_t	*dstLine, *dst;
    uint8_t	*maskLine, *mask;
    int	dstStride, maskStride;
    uint16_t	w;
    uint32_t	src;
    uint8_t	sa;
    __m64	vsrc, vsrca;

    fbComposeGetStart (pDst, xDst, yDst, uint8_t, dstStride, dstLine, 1);
    fbComposeGetStart (pMask, xMask, yMask, uint8_t, maskStride, maskLine, 1);

    fbComposeGetSolid(pSrc, src, pDst->bits.format);

    sa = src >> 24;
    if (sa == 0)
	return;

    vsrc = load8888(src);
    vsrca = expand_alpha(vsrc);

    while (height--)
    {
	dst = dstLine;
	dstLine += dstStride;
	mask = maskLine;
	maskLine += maskStride;
	w = width;

	if ((((unsigned long)pDst & 3) == 0) &&
	    (((unsigned long)pSrc & 3) == 0))
	{
	    while (w >= 4)
	    {
		uint32_t m;
		__m64 vmask;
		__m64 vdest;

		m = 0;

		vmask = load8888 (*(uint32_t *)mask);
		vdest = load8888 (*(uint32_t *)dst);

		*(uint32_t *)dst = store8888 (in (in (vsrca, vmask), vdest));

		dst += 4;
		mask += 4;
		w -= 4;
	    }
	}

	while (w--)
	{
	    uint16_t	tmp;
	    uint8_t	a;
	    uint32_t	m, d;
	    uint32_t	r;

	    a = *mask++;
	    d = *dst;

	    m = FbInU (sa, 0, a, tmp);
	    r = FbInU (m, 0, d, tmp);

	    *dst++ = r;
	}
    }

    _mm_empty();
}

static void
fbCompositeIn_8x8mmx (pixman_implementation_t *imp,
		      pixman_op_t op,
		      pixman_image_t * pSrc,
		      pixman_image_t * pMask,
		      pixman_image_t * pDst,
		      int32_t      xSrc,
		      int32_t      ySrc,
		      int32_t      xMask,
		      int32_t      yMask,
		      int32_t      xDst,
		      int32_t      yDst,
		      int32_t     width,
		      int32_t     height)
{
    uint8_t	*dstLine, *dst;
    uint8_t	*srcLine, *src;
    int	srcStride, dstStride;
    uint16_t	w;

    fbComposeGetStart (pDst, xDst, yDst, uint8_t, dstStride, dstLine, 1);
    fbComposeGetStart (pSrc, xSrc, ySrc, uint8_t, srcStride, srcLine, 1);

    while (height--)
    {
	dst = dstLine;
	dstLine += dstStride;
	src = srcLine;
	srcLine += srcStride;
	w = width;

	if ((((unsigned long)pDst & 3) == 0) &&
	    (((unsigned long)pSrc & 3) == 0))
	{
	    while (w >= 4)
	    {
		uint32_t *s = (uint32_t *)src;
		uint32_t *d = (uint32_t *)dst;

		*d = store8888 (in (load8888 (*s), load8888 (*d)));

		w -= 4;
		dst += 4;
		src += 4;
	    }
	}

	while (w--)
	{
	    uint8_t s, d;
	    uint16_t tmp;

	    s = *src;
	    d = *dst;

	    *dst = FbInU (s, 0, d, tmp);

	    src++;
	    dst++;
	}
    }

    _mm_empty ();
}

static void
fbCompositeSrcAdd_8888x8x8mmx (pixman_implementation_t *imp,
			       pixman_op_t op,
			       pixman_image_t * pSrc,
			       pixman_image_t * pMask,
			       pixman_image_t * pDst,
			       int32_t      xSrc,
			       int32_t      ySrc,
			       int32_t      xMask,
			       int32_t      yMask,
			       int32_t      xDst,
			       int32_t      yDst,
			       int32_t     width,
			       int32_t     height)
{
    uint8_t	*dstLine, *dst;
    uint8_t	*maskLine, *mask;
    int	dstStride, maskStride;
    uint16_t	w;
    uint32_t	src;
    uint8_t	sa;
    __m64	vsrc, vsrca;

    fbComposeGetStart (pDst, xDst, yDst, uint8_t, dstStride, dstLine, 1);
    fbComposeGetStart (pMask, xMask, yMask, uint8_t, maskStride, maskLine, 1);

    fbComposeGetSolid(pSrc, src, pDst->bits.format);

    sa = src >> 24;
    if (sa == 0)
	return;

    vsrc = load8888(src);
    vsrca = expand_alpha(vsrc);

    while (height--)
    {
	dst = dstLine;
	dstLine += dstStride;
	mask = maskLine;
	maskLine += maskStride;
	w = width;

	if ((((unsigned long)pMask & 3) == 0) &&
	    (((unsigned long)pDst  & 3) == 0))
	{
	    while (w >= 4)
	    {
		__m64 vmask = load8888 (*(uint32_t *)mask);
		__m64 vdest = load8888 (*(uint32_t *)dst);

		*(uint32_t *)dst = store8888 (_mm_adds_pu8 (in (vsrca, vmask), vdest));

		w -= 4;
		dst += 4;
		mask += 4;
	    }
	}

	while (w--)
	{
	    uint16_t	tmp;
	    uint16_t	a;
	    uint32_t	m, d;
	    uint32_t	r;

	    a = *mask++;
	    d = *dst;

	    m = FbInU (sa, 0, a, tmp);
	    r = FbAdd (m, d, 0, tmp);

	    *dst++ = r;
	}
    }

    _mm_empty();
}

static void
fbCompositeSrcAdd_8000x8000mmx (pixman_implementation_t *imp,
				pixman_op_t op,
				pixman_image_t * pSrc,
				pixman_image_t * pMask,
				pixman_image_t * pDst,
				int32_t      xSrc,
				int32_t      ySrc,
				int32_t      xMask,
				int32_t      yMask,
				int32_t      xDst,
				int32_t      yDst,
				int32_t     width,
				int32_t     height)
{
    uint8_t	*dstLine, *dst;
    uint8_t	*srcLine, *src;
    int	dstStride, srcStride;
    uint16_t	w;
    uint8_t	s, d;
    uint16_t	t;

    CHECKPOINT();

    fbComposeGetStart (pSrc, xSrc, ySrc, uint8_t, srcStride, srcLine, 1);
    fbComposeGetStart (pDst, xDst, yDst, uint8_t, dstStride, dstLine, 1);

    while (height--)
    {
	dst = dstLine;
	dstLine += dstStride;
	src = srcLine;
	srcLine += srcStride;
	w = width;

	while (w && (unsigned long)dst & 7)
	{
	    s = *src;
	    d = *dst;
	    t = d + s;
	    s = t | (0 - (t >> 8));
	    *dst = s;

	    dst++;
	    src++;
	    w--;
	}

	while (w >= 8)
	{
	    *(__m64*)dst = _mm_adds_pu8(*(__m64*)src, *(__m64*)dst);
	    dst += 8;
	    src += 8;
	    w -= 8;
	}

	while (w)
	{
	    s = *src;
	    d = *dst;
	    t = d + s;
	    s = t | (0 - (t >> 8));
	    *dst = s;

	    dst++;
	    src++;
	    w--;
	}
    }

    _mm_empty();
}

static void
fbCompositeSrcAdd_8888x8888mmx (pixman_implementation_t *imp,
				pixman_op_t 	op,
				pixman_image_t *	pSrc,
				pixman_image_t *	pMask,
				pixman_image_t *	 pDst,
				int32_t		 xSrc,
				int32_t      ySrc,
				int32_t      xMask,
				int32_t      yMask,
				int32_t      xDst,
				int32_t      yDst,
				int32_t     width,
				int32_t     height)
{
    __m64 dst64;
    uint32_t	*dstLine, *dst;
    uint32_t	*srcLine, *src;
    int	dstStride, srcStride;
    uint16_t	w;

    CHECKPOINT();

    fbComposeGetStart (pSrc, xSrc, ySrc, uint32_t, srcStride, srcLine, 1);
    fbComposeGetStart (pDst, xDst, yDst, uint32_t, dstStride, dstLine, 1);

    while (height--)
    {
	dst = dstLine;
	dstLine += dstStride;
	src = srcLine;
	srcLine += srcStride;
	w = width;

	while (w && (unsigned long)dst & 7)
	{
	    *dst = _mm_cvtsi64_si32(_mm_adds_pu8(_mm_cvtsi32_si64(*src),
						 _mm_cvtsi32_si64(*dst)));
	    dst++;
	    src++;
	    w--;
	}

	while (w >= 2)
	{
	    dst64 = _mm_adds_pu8(*(__m64*)src, *(__m64*)dst);
	    *(uint64_t*)dst = UINT64(dst64);
	    dst += 2;
	    src += 2;
	    w -= 2;
	}

	if (w)
	{
	    *dst = _mm_cvtsi64_si32(_mm_adds_pu8(_mm_cvtsi32_si64(*src),
						 _mm_cvtsi32_si64(*dst)));

	}
    }

    _mm_empty();
}

static pixman_bool_t
pixman_blt_mmx (uint32_t *src_bits,
		uint32_t *dst_bits,
		int src_stride,
		int dst_stride,
		int src_bpp,
		int dst_bpp,
		int src_x, int src_y,
		int dst_x, int dst_y,
		int width, int height)
{
    uint8_t *	src_bytes;
    uint8_t *	dst_bytes;
    int		byte_width;

    if (src_bpp != dst_bpp)
	return FALSE;

    if (src_bpp == 16)
    {
	src_stride = src_stride * (int) sizeof (uint32_t) / 2;
	dst_stride = dst_stride * (int) sizeof (uint32_t) / 2;
	src_bytes = (uint8_t *)(((uint16_t *)src_bits) + src_stride * (src_y) + (src_x));
	dst_bytes = (uint8_t *)(((uint16_t *)dst_bits) + dst_stride * (dst_y) + (dst_x));
	byte_width = 2 * width;
	src_stride *= 2;
	dst_stride *= 2;
    } else if (src_bpp == 32) {
	src_stride = src_stride * (int) sizeof (uint32_t) / 4;
	dst_stride = dst_stride * (int) sizeof (uint32_t) / 4;
	src_bytes = (uint8_t *)(((uint32_t *)src_bits) + src_stride * (src_y) + (src_x));
	dst_bytes = (uint8_t *)(((uint32_t *)dst_bits) + dst_stride * (dst_y) + (dst_x));
	byte_width = 4 * width;
	src_stride *= 4;
	dst_stride *= 4;
    } else {
	return FALSE;
    }

    while (height--)
    {
	int w;
	uint8_t *s = src_bytes;
	uint8_t *d = dst_bytes;
	src_bytes += src_stride;
	dst_bytes += dst_stride;
	w = byte_width;

	while (w >= 2 && ((unsigned long)d & 3))
	{
	    *(uint16_t *)d = *(uint16_t *)s;
	    w -= 2;
	    s += 2;
	    d += 2;
	}

	while (w >= 4 && ((unsigned long)d & 7))
	{
	    *(uint32_t *)d = *(uint32_t *)s;

	    w -= 4;
	    s += 4;
	    d += 4;
	}

	while (w >= 64)
	{
#if defined (__GNUC__) || (defined(__SUNPRO_C) && (__SUNPRO_C >= 0x590))
	    __asm__ (
		"movq	  (%1),	  %%mm0\n"
		"movq	 8(%1),	  %%mm1\n"
		"movq	16(%1),	  %%mm2\n"
		"movq	24(%1),	  %%mm3\n"
		"movq	32(%1),	  %%mm4\n"
		"movq	40(%1),	  %%mm5\n"
		"movq	48(%1),	  %%mm6\n"
		"movq	56(%1),	  %%mm7\n"

		"movq	%%mm0,	  (%0)\n"
		"movq	%%mm1,	 8(%0)\n"
		"movq	%%mm2,	16(%0)\n"
		"movq	%%mm3,	24(%0)\n"
		"movq	%%mm4,	32(%0)\n"
		"movq	%%mm5,	40(%0)\n"
		"movq	%%mm6,	48(%0)\n"
		"movq	%%mm7,	56(%0)\n"
		:
		: "r" (d), "r" (s)
		: "memory",
		  "%mm0", "%mm1", "%mm2", "%mm3",
		  "%mm4", "%mm5", "%mm6", "%mm7");
#else
	    __m64 v0 = *(__m64 *)(s + 0);
	    __m64 v1 = *(__m64 *)(s + 8);
	    __m64 v2 = *(__m64 *)(s + 16);
	    __m64 v3 = *(__m64 *)(s + 24);
	    __m64 v4 = *(__m64 *)(s + 32);
	    __m64 v5 = *(__m64 *)(s + 40);
	    __m64 v6 = *(__m64 *)(s + 48);
	    __m64 v7 = *(__m64 *)(s + 56);
	    *(__m64 *)(d + 0)  = v0;
	    *(__m64 *)(d + 8)  = v1;
	    *(__m64 *)(d + 16) = v2;
	    *(__m64 *)(d + 24) = v3;
	    *(__m64 *)(d + 32) = v4;
	    *(__m64 *)(d + 40) = v5;
	    *(__m64 *)(d + 48) = v6;
	    *(__m64 *)(d + 56) = v7;
#endif

	    w -= 64;
	    s += 64;
	    d += 64;
	}
	while (w >= 4)
	{
	    *(uint32_t *)d = *(uint32_t *)s;

	    w -= 4;
	    s += 4;
	    d += 4;
	}
	if (w >= 2)
	{
	    *(uint16_t *)d = *(uint16_t *)s;
	    w -= 2;
	    s += 2;
	    d += 2;
	}
    }

    _mm_empty();

    return TRUE;
}

static void
fbCompositeCopyAreammx (pixman_implementation_t *imp,
			pixman_op_t       op,
			pixman_image_t *	pSrc,
			pixman_image_t *	pMask,
			pixman_image_t *	pDst,
			int32_t		xSrc,
			int32_t		ySrc,
			int32_t		xMask,
			int32_t		yMask,
			int32_t		xDst,
			int32_t		yDst,
			int32_t		width,
			int32_t		height)
{
    pixman_blt_mmx (pSrc->bits.bits,
		    pDst->bits.bits,
		    pSrc->bits.rowstride,
		    pDst->bits.rowstride,
		    PIXMAN_FORMAT_BPP (pSrc->bits.format),
		    PIXMAN_FORMAT_BPP (pDst->bits.format),
		    xSrc, ySrc, xDst, yDst, width, height);
}

static void
fbCompositeOver_x888x8x8888mmx (pixman_implementation_t *imp,
				pixman_op_t      op,
				pixman_image_t * pSrc,
				pixman_image_t * pMask,
				pixman_image_t * pDst,
				int32_t      xSrc,
				int32_t      ySrc,
				int32_t      xMask,
				int32_t      yMask,
				int32_t      xDst,
				int32_t      yDst,
				int32_t     width,
				int32_t     height)
{
    uint32_t	*src, *srcLine;
    uint32_t    *dst, *dstLine;
    uint8_t	*mask, *maskLine;
    int		 srcStride, maskStride, dstStride;
    uint16_t w;

    fbComposeGetStart (pDst, xDst, yDst, uint32_t, dstStride, dstLine, 1);
    fbComposeGetStart (pMask, xMask, yMask, uint8_t, maskStride, maskLine, 1);
    fbComposeGetStart (pSrc, xSrc, ySrc, uint32_t, srcStride, srcLine, 1);

    while (height--)
    {
	src = srcLine;
	srcLine += srcStride;
	dst = dstLine;
	dstLine += dstStride;
	mask = maskLine;
	maskLine += maskStride;

	w = width;

	while (w--)
	{
	    uint64_t m = *mask;

	    if (m)
	    {
		__m64 s = load8888 (*src | 0xff000000);

		if (m == 0xff)
		    *dst = store8888 (s);
		else
		{
		    __m64 sa = expand_alpha (s);
		    __m64 vm = expand_alpha_rev (M64(m));
		    __m64 vdest = in_over(s, sa, vm, load8888 (*dst));

		    *dst = store8888 (vdest);
		}
	    }

	    mask++;
	    dst++;
	    src++;
	}
    }

    _mm_empty();
}

static const FastPathInfo mmx_fast_paths[] =
{
    { PIXMAN_OP_OVER, PIXMAN_solid,    PIXMAN_a8,       PIXMAN_r5g6b5,   fbCompositeSolidMask_nx8x0565mmx,     0 },
    { PIXMAN_OP_OVER, PIXMAN_solid,    PIXMAN_a8,       PIXMAN_b5g6r5,   fbCompositeSolidMask_nx8x0565mmx,     0 },
    { PIXMAN_OP_OVER, PIXMAN_solid,    PIXMAN_a8,       PIXMAN_a8r8g8b8, fbCompositeSolidMask_nx8x8888mmx,     0 },
    { PIXMAN_OP_OVER, PIXMAN_solid,    PIXMAN_a8,       PIXMAN_x8r8g8b8, fbCompositeSolidMask_nx8x8888mmx,     0 },
    { PIXMAN_OP_OVER, PIXMAN_solid,    PIXMAN_a8,       PIXMAN_a8b8g8r8, fbCompositeSolidMask_nx8x8888mmx,     0 },
    { PIXMAN_OP_OVER, PIXMAN_solid,    PIXMAN_a8,       PIXMAN_x8b8g8r8, fbCompositeSolidMask_nx8x8888mmx,     0 },
    { PIXMAN_OP_OVER, PIXMAN_solid,    PIXMAN_a8r8g8b8, PIXMAN_a8r8g8b8, fbCompositeSolidMask_nx8888x8888Cmmx, NEED_COMPONENT_ALPHA },
    { PIXMAN_OP_OVER, PIXMAN_solid,    PIXMAN_a8r8g8b8, PIXMAN_x8r8g8b8, fbCompositeSolidMask_nx8888x8888Cmmx, NEED_COMPONENT_ALPHA },
    { PIXMAN_OP_OVER, PIXMAN_solid,    PIXMAN_a8r8g8b8, PIXMAN_r5g6b5,   fbCompositeSolidMask_nx8888x0565Cmmx, NEED_COMPONENT_ALPHA },
    { PIXMAN_OP_OVER, PIXMAN_solid,    PIXMAN_a8b8g8r8, PIXMAN_a8b8g8r8, fbCompositeSolidMask_nx8888x8888Cmmx, NEED_COMPONENT_ALPHA },
    { PIXMAN_OP_OVER, PIXMAN_solid,    PIXMAN_a8b8g8r8, PIXMAN_x8b8g8r8, fbCompositeSolidMask_nx8888x8888Cmmx, NEED_COMPONENT_ALPHA },
    { PIXMAN_OP_OVER, PIXMAN_solid,    PIXMAN_a8b8g8r8, PIXMAN_b5g6r5,   fbCompositeSolidMask_nx8888x0565Cmmx, NEED_COMPONENT_ALPHA },
    { PIXMAN_OP_OVER, PIXMAN_x8b8g8r8, PIXMAN_a8r8g8b8, PIXMAN_a8r8g8b8, fbCompositeSrc_8888RevNPx8888mmx, NEED_PIXBUF },
    { PIXMAN_OP_OVER, PIXMAN_x8b8g8r8, PIXMAN_a8b8g8r8, PIXMAN_a8r8g8b8, fbCompositeSrc_8888RevNPx8888mmx, NEED_PIXBUF },
    { PIXMAN_OP_OVER, PIXMAN_x8b8g8r8, PIXMAN_a8r8g8b8, PIXMAN_x8r8g8b8, fbCompositeSrc_8888RevNPx8888mmx, NEED_PIXBUF },
    { PIXMAN_OP_OVER, PIXMAN_x8b8g8r8, PIXMAN_a8b8g8r8, PIXMAN_x8r8g8b8, fbCompositeSrc_8888RevNPx8888mmx, NEED_PIXBUF },
    { PIXMAN_OP_OVER, PIXMAN_x8b8g8r8, PIXMAN_a8r8g8b8, PIXMAN_r5g6b5,   fbCompositeSrc_8888RevNPx0565mmx, NEED_PIXBUF },
    { PIXMAN_OP_OVER, PIXMAN_x8b8g8r8, PIXMAN_a8b8g8r8, PIXMAN_r5g6b5,   fbCompositeSrc_8888RevNPx0565mmx, NEED_PIXBUF },
    { PIXMAN_OP_OVER, PIXMAN_x8r8g8b8, PIXMAN_a8r8g8b8, PIXMAN_a8b8g8r8, fbCompositeSrc_8888RevNPx8888mmx, NEED_PIXBUF },
    { PIXMAN_OP_OVER, PIXMAN_x8r8g8b8, PIXMAN_a8b8g8r8, PIXMAN_a8b8g8r8, fbCompositeSrc_8888RevNPx8888mmx, NEED_PIXBUF },
    { PIXMAN_OP_OVER, PIXMAN_x8r8g8b8, PIXMAN_a8r8g8b8, PIXMAN_x8b8g8r8, fbCompositeSrc_8888RevNPx8888mmx, NEED_PIXBUF },
    { PIXMAN_OP_OVER, PIXMAN_x8r8g8b8, PIXMAN_a8b8g8r8, PIXMAN_x8b8g8r8, fbCompositeSrc_8888RevNPx8888mmx, NEED_PIXBUF },
    { PIXMAN_OP_OVER, PIXMAN_x8r8g8b8, PIXMAN_a8r8g8b8, PIXMAN_b5g6r5,   fbCompositeSrc_8888RevNPx0565mmx, NEED_PIXBUF },
    { PIXMAN_OP_OVER, PIXMAN_x8r8g8b8, PIXMAN_a8b8g8r8, PIXMAN_b5g6r5,   fbCompositeSrc_8888RevNPx0565mmx, NEED_PIXBUF },
    { PIXMAN_OP_OVER, PIXMAN_x8r8g8b8, PIXMAN_a8,       PIXMAN_a8r8g8b8, fbCompositeSrc_x888xnx8888mmx,    NEED_SOLID_MASK },
    { PIXMAN_OP_OVER, PIXMAN_x8r8g8b8, PIXMAN_a8,       PIXMAN_x8r8g8b8, fbCompositeSrc_x888xnx8888mmx,	   NEED_SOLID_MASK },
    { PIXMAN_OP_OVER, PIXMAN_x8b8g8r8, PIXMAN_a8,	PIXMAN_a8b8g8r8, fbCompositeSrc_x888xnx8888mmx,	   NEED_SOLID_MASK },
    { PIXMAN_OP_OVER, PIXMAN_x8b8g8r8, PIXMAN_a8,	PIXMAN_x8b8g8r8, fbCompositeSrc_x888xnx8888mmx,	   NEED_SOLID_MASK },
    { PIXMAN_OP_OVER, PIXMAN_a8r8g8b8, PIXMAN_a8,       PIXMAN_a8r8g8b8, fbCompositeSrc_8888x8x8888mmx,    NEED_SOLID_MASK },
    { PIXMAN_OP_OVER, PIXMAN_a8r8g8b8, PIXMAN_a8,       PIXMAN_x8r8g8b8, fbCompositeSrc_8888x8x8888mmx,	   NEED_SOLID_MASK },
    { PIXMAN_OP_OVER, PIXMAN_a8b8g8r8, PIXMAN_a8,	PIXMAN_a8b8g8r8, fbCompositeSrc_8888x8x8888mmx,	   NEED_SOLID_MASK },
    { PIXMAN_OP_OVER, PIXMAN_a8b8g8r8, PIXMAN_a8,	PIXMAN_x8b8g8r8, fbCompositeSrc_8888x8x8888mmx,	   NEED_SOLID_MASK },
#if 0
    /* FIXME: This code is commented out since it's apparently not actually faster than the generic code. */
    { PIXMAN_OP_OVER, PIXMAN_x8r8g8b8, PIXMAN_a8,	PIXMAN_x8r8g8b8, fbCompositeOver_x888x8x8888mmx,   0 },
    { PIXMAN_OP_OVER, PIXMAN_x8r8g8b8, PIXMAN_a8,	PIXMAN_a8r8g8b8, fbCompositeOver_x888x8x8888mmx,   0 },
    { PIXMAN_OP_OVER, PIXMAN_x8b8r8g8, PIXMAN_a8,	PIXMAN_x8b8g8r8, fbCompositeOver_x888x8x8888mmx,   0 },
    { PIXMAN_OP_OVER, PIXMAN_x8b8r8g8, PIXMAN_a8,	PIXMAN_a8r8g8b8, fbCompositeOver_x888x8x8888mmx,   0 },
#endif
    { PIXMAN_OP_OVER, PIXMAN_solid,    PIXMAN_null,	PIXMAN_a8r8g8b8, fbCompositeSolid_nx8888mmx,       0 },
    { PIXMAN_OP_OVER, PIXMAN_solid,    PIXMAN_null,     PIXMAN_x8r8g8b8, fbCompositeSolid_nx8888mmx,	   0 },
    { PIXMAN_OP_OVER, PIXMAN_solid,    PIXMAN_null,     PIXMAN_r5g6b5,   fbCompositeSolid_nx0565mmx,	   0 },
    { PIXMAN_OP_OVER, PIXMAN_x8r8g8b8, PIXMAN_null,     PIXMAN_x8r8g8b8, fbCompositeCopyAreammx,	   0 },
    { PIXMAN_OP_OVER, PIXMAN_x8b8g8r8, PIXMAN_null,     PIXMAN_x8b8g8r8, fbCompositeCopyAreammx,	   0 },

    { PIXMAN_OP_OVER, PIXMAN_a8r8g8b8, PIXMAN_null,     PIXMAN_a8r8g8b8, fbCompositeSrc_8888x8888mmx,	   0 },
    { PIXMAN_OP_OVER, PIXMAN_a8r8g8b8, PIXMAN_null,	PIXMAN_x8r8g8b8, fbCompositeSrc_8888x8888mmx,	   0 },
    { PIXMAN_OP_OVER, PIXMAN_a8r8g8b8, PIXMAN_null,	PIXMAN_r5g6b5,	 fbCompositeSrc_8888x0565mmx,	   0 },
    { PIXMAN_OP_OVER, PIXMAN_a8b8g8r8, PIXMAN_null,	PIXMAN_a8b8g8r8, fbCompositeSrc_8888x8888mmx,	   0 },
    { PIXMAN_OP_OVER, PIXMAN_a8b8g8r8, PIXMAN_null,	PIXMAN_x8b8g8r8, fbCompositeSrc_8888x8888mmx,	   0 },
    { PIXMAN_OP_OVER, PIXMAN_a8b8g8r8, PIXMAN_null,     PIXMAN_b5g6r5,   fbCompositeSrc_8888x0565mmx,	   0 },

    { PIXMAN_OP_ADD, PIXMAN_a8r8g8b8,  PIXMAN_null,	PIXMAN_a8r8g8b8, fbCompositeSrcAdd_8888x8888mmx,   0 },
    { PIXMAN_OP_ADD, PIXMAN_a8b8g8r8,  PIXMAN_null,	PIXMAN_a8b8g8r8, fbCompositeSrcAdd_8888x8888mmx,   0 },
    { PIXMAN_OP_ADD, PIXMAN_a8,        PIXMAN_null,     PIXMAN_a8,       fbCompositeSrcAdd_8000x8000mmx,   0 },
    { PIXMAN_OP_ADD, PIXMAN_solid,     PIXMAN_a8,       PIXMAN_a8,       fbCompositeSrcAdd_8888x8x8mmx,    0 },

    { PIXMAN_OP_SRC, PIXMAN_solid,     PIXMAN_a8,       PIXMAN_a8r8g8b8, fbCompositeSolidMaskSrc_nx8x8888mmx, 0 },
    { PIXMAN_OP_SRC, PIXMAN_solid,     PIXMAN_a8,       PIXMAN_x8r8g8b8, fbCompositeSolidMaskSrc_nx8x8888mmx, 0 },
    { PIXMAN_OP_SRC, PIXMAN_solid,     PIXMAN_a8,       PIXMAN_a8b8g8r8, fbCompositeSolidMaskSrc_nx8x8888mmx, 0 },
    { PIXMAN_OP_SRC, PIXMAN_solid,     PIXMAN_a8,       PIXMAN_x8b8g8r8, fbCompositeSolidMaskSrc_nx8x8888mmx, 0 },
    { PIXMAN_OP_SRC, PIXMAN_a8r8g8b8,  PIXMAN_null,	PIXMAN_a8r8g8b8, fbCompositeCopyAreammx, 0 },
    { PIXMAN_OP_SRC, PIXMAN_a8b8g8r8,  PIXMAN_null,	PIXMAN_a8b8g8r8, fbCompositeCopyAreammx, 0 },
    { PIXMAN_OP_SRC, PIXMAN_a8r8g8b8,  PIXMAN_null,	PIXMAN_x8r8g8b8, fbCompositeCopyAreammx, 0 },
    { PIXMAN_OP_SRC, PIXMAN_a8b8g8r8,  PIXMAN_null,	PIXMAN_x8b8g8r8, fbCompositeCopyAreammx, 0 },
    { PIXMAN_OP_SRC, PIXMAN_x8r8g8b8,  PIXMAN_null,	PIXMAN_x8r8g8b8, fbCompositeCopyAreammx, 0 },
    { PIXMAN_OP_SRC, PIXMAN_x8b8g8r8,  PIXMAN_null,	PIXMAN_x8b8g8r8, fbCompositeCopyAreammx, 0 },
    { PIXMAN_OP_SRC, PIXMAN_r5g6b5,    PIXMAN_null,     PIXMAN_r5g6b5,   fbCompositeCopyAreammx, 0 },
    { PIXMAN_OP_SRC, PIXMAN_b5g6r5,    PIXMAN_null,     PIXMAN_b5g6r5,   fbCompositeCopyAreammx, 0 },    

    { PIXMAN_OP_IN,  PIXMAN_a8,        PIXMAN_null,     PIXMAN_a8,       fbCompositeIn_8x8mmx,   0 },
    { PIXMAN_OP_IN,  PIXMAN_solid,     PIXMAN_a8,	PIXMAN_a8,	 fbCompositeIn_nx8x8mmx, 0 },

    { PIXMAN_OP_NONE },
};

static void
mmx_composite (pixman_implementation_t *imp,
	       pixman_op_t     op,
	       pixman_image_t *src,
	       pixman_image_t *mask,
	       pixman_image_t *dest,
	       int32_t         src_x,
	       int32_t         src_y,
	       int32_t         mask_x,
	       int32_t         mask_y,
	       int32_t         dest_x,
	       int32_t         dest_y,
	       int32_t         width,
	       int32_t         height)
{
    if (_pixman_run_fast_path (mmx_fast_paths, imp,
			       op, src, mask, dest,
			       src_x, src_y,
			       mask_x, mask_y,
			       dest_x, dest_y,
			       width, height))
	return;

    _pixman_implementation_composite (imp->delegate,
				      op, src, mask, dest, src_x, src_y,
				      mask_x, mask_y, dest_x, dest_y,
				      width, height);
}

static pixman_bool_t
mmx_blt (pixman_implementation_t *imp,
	 uint32_t *src_bits,
	 uint32_t *dst_bits,
	 int src_stride,
	 int dst_stride,
	 int src_bpp,
	 int dst_bpp,
	 int src_x, int src_y,
	 int dst_x, int dst_y,
	 int width, int height)
{
    if (!pixman_blt_mmx (
	    src_bits, dst_bits, src_stride, dst_stride, src_bpp, dst_bpp,
	    src_x, src_y, dst_x, dst_y, width, height))

    {
	return _pixman_implementation_blt (
	    imp->delegate,
	    src_bits, dst_bits, src_stride, dst_stride, src_bpp, dst_bpp,
	    src_x, src_y, dst_x, dst_y, width, height);
    }

    return TRUE;
}

static pixman_bool_t
mmx_fill (pixman_implementation_t *imp,
	  uint32_t *bits,
	  int stride,
	  int bpp,
	  int x,
	  int y,
	  int width,
	  int height,
	  uint32_t xor)
{
    if (!pixman_fill_mmx (bits, stride, bpp, x, y, width, height, xor))
    {
	return _pixman_implementation_fill (
	    imp->delegate, bits, stride, bpp, x, y, width, height, xor);
    }

    return TRUE;
}

pixman_implementation_t *
_pixman_implementation_create_mmx (pixman_implementation_t *toplevel)
{
    pixman_implementation_t *general = _pixman_implementation_create_fast_path (NULL);
    pixman_implementation_t *imp = _pixman_implementation_create (toplevel, general);

    imp->combine_32[PIXMAN_OP_OVER] = mmxCombineOverU;
    imp->combine_32[PIXMAN_OP_OVER_REVERSE] = mmxCombineOverReverseU;
    imp->combine_32[PIXMAN_OP_IN] = mmxCombineInU;
    imp->combine_32[PIXMAN_OP_IN_REVERSE] = mmxCombineInReverseU;
    imp->combine_32[PIXMAN_OP_OUT] = mmxCombineOutU;
    imp->combine_32[PIXMAN_OP_OUT_REVERSE] = mmxCombineOutReverseU;
    imp->combine_32[PIXMAN_OP_ATOP] = mmxCombineAtopU;
    imp->combine_32[PIXMAN_OP_ATOP_REVERSE] = mmxCombineAtopReverseU;
    imp->combine_32[PIXMAN_OP_XOR] = mmxCombineXorU; 
    imp->combine_32[PIXMAN_OP_ADD] = mmxCombineAddU;
    imp->combine_32[PIXMAN_OP_SATURATE] = mmxCombineSaturateU;
    
    imp->combine_32_ca[PIXMAN_OP_SRC] = mmxCombineSrcC;
    imp->combine_32_ca[PIXMAN_OP_OVER] = mmxCombineOverC;
    imp->combine_32_ca[PIXMAN_OP_OVER_REVERSE] = mmxCombineOverReverseC;
    imp->combine_32_ca[PIXMAN_OP_IN] = mmxCombineInC;
    imp->combine_32_ca[PIXMAN_OP_IN_REVERSE] = mmxCombineInReverseC;
    imp->combine_32_ca[PIXMAN_OP_OUT] = mmxCombineOutC;
    imp->combine_32_ca[PIXMAN_OP_OUT_REVERSE] = mmxCombineOutReverseC;
    imp->combine_32_ca[PIXMAN_OP_ATOP] = mmxCombineAtopC;
    imp->combine_32_ca[PIXMAN_OP_ATOP_REVERSE] = mmxCombineAtopReverseC;
    imp->combine_32_ca[PIXMAN_OP_XOR] = mmxCombineXorC;
    imp->combine_32_ca[PIXMAN_OP_ADD] = mmxCombineAddC;

    imp->composite = mmx_composite;
    imp->blt = mmx_blt;
    imp->fill = mmx_fill;
    
    return imp;
}

#endif /* USE_MMX */
