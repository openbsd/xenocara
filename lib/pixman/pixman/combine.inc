#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <string.h>

#include "pixman-private.h"

#include "pixman-combine.h"

/*
 * There are two ways of handling alpha -- either as a single unified value or
 * a separate value for each component, hence each macro must have two
 * versions.  The unified alpha version has a 'U' at the end of the name,
 * the component version has a 'C'.  Similarly, functions which deal with
 * this difference will have two versions using the same convention.
 */


/*
 * All of the composing functions
 */

static force_inline comp4_t
combineMask (const comp4_t *src, const comp4_t *mask, int i)
{
    comp4_t s, m;

    if (mask)
    {
	m = *(mask + i) >> A_SHIFT;

	if (!m)
	    return 0;
    }

    s = *(src + i);

    if (mask)
	FbByteMul (s, m);

    return s;
}

FASTCALL static void
fbCombineClear (pixman_implementation_t *imp, pixman_op_t op,
		comp4_t *dest, const comp4_t *src, const comp4_t *mask, int width)
{
    memset(dest, 0, width*sizeof(comp4_t));
}

FASTCALL static void
fbCombineSrcU (pixman_implementation_t *imp, pixman_op_t op,
	       comp4_t *dest, const comp4_t *src, const comp4_t *mask, int width)
{
    int i;

    if (!mask)
	memcpy (dest, src, width * sizeof (comp4_t));
    else
    {
	for (i = 0; i < width; ++i)
	{
	    comp4_t s = combineMask (src, mask, i);
	    
	    *(dest + i) = s;
	}
    }
}

/* if the Src is opaque, call fbCombineSrcU */
FASTCALL static void
fbCombineOverU (pixman_implementation_t *imp, pixman_op_t op,
		comp4_t *dest, const comp4_t *src, const comp4_t *mask, int width)
{
    int i;
    for (i = 0; i < width; ++i) {
        comp4_t s = combineMask (src, mask, i);
        comp4_t d = *(dest + i);
        comp4_t ia = Alpha(~s);

        FbByteMulAdd(d, ia, s);
	*(dest + i) = d;
    }
}

/* if the Dst is opaque, this is a noop */
FASTCALL static void
fbCombineOverReverseU (pixman_implementation_t *imp, pixman_op_t op,
		       comp4_t *dest, const comp4_t *src, const comp4_t *mask, int width)
{
    int i;
    for (i = 0; i < width; ++i) {
        comp4_t s = combineMask (src, mask, i);
        comp4_t d = *(dest + i);
        comp4_t ia = Alpha(~*(dest + i));
        FbByteMulAdd(s, ia, d);
	*(dest + i) = s;
    }
}

/* if the Dst is opaque, call fbCombineSrcU */
FASTCALL static void
fbCombineInU (pixman_implementation_t *imp, pixman_op_t op,
	      comp4_t *dest, const comp4_t *src, const comp4_t *mask, int width)
{
    int i;
    for (i = 0; i < width; ++i) {
        comp4_t s = combineMask (src, mask, i);
        comp4_t a = Alpha(*(dest + i));
        FbByteMul(s, a);
	*(dest + i) = s;
    }
}

/* if the Src is opaque, this is a noop */
FASTCALL static void
fbCombineInReverseU (pixman_implementation_t *imp, pixman_op_t op,
		     comp4_t *dest, const comp4_t *src, const comp4_t *mask, int width)
{
    int i;
    for (i = 0; i < width; ++i) {
	comp4_t s = combineMask (src, mask, i);
	comp4_t d = *(dest + i);
        comp4_t a = Alpha(s);
        FbByteMul(d, a);
	*(dest + i) = d;
    }
}

/* if the Dst is opaque, call fbCombineClear */
FASTCALL static void
fbCombineOutU (pixman_implementation_t *imp, pixman_op_t op,
	       comp4_t *dest, const comp4_t *src, const comp4_t *mask, int width)
{
    int i;
    for (i = 0; i < width; ++i) {
        comp4_t s = combineMask (src, mask, i);
        comp4_t a = Alpha(~*(dest + i));
        FbByteMul(s, a);
	*(dest + i) = s;
    }
}

/* if the Src is opaque, call fbCombineClear */
FASTCALL static void
fbCombineOutReverseU (pixman_implementation_t *imp, pixman_op_t op,
		      comp4_t *dest, const comp4_t *src, const comp4_t *mask, int width)
{
    int i;
    for (i = 0; i < width; ++i) {
	comp4_t s = combineMask (src, mask, i);
        comp4_t d = *(dest + i);
        comp4_t a = Alpha(~s);
        FbByteMul(d, a);
	*(dest + i) = d;
    }
}

/* if the Src is opaque, call fbCombineInU */
/* if the Dst is opaque, call fbCombineOverU */
/* if both the Src and Dst are opaque, call fbCombineSrcU */
FASTCALL static void
fbCombineAtopU (pixman_implementation_t *imp, pixman_op_t op,
		comp4_t *dest, const comp4_t *src, const comp4_t *mask, int width)
{
    int i;
    for (i = 0; i < width; ++i) {
        comp4_t s = combineMask (src, mask, i);
        comp4_t d = *(dest + i);
        comp4_t dest_a = Alpha(d);
        comp4_t src_ia = Alpha(~s);

        FbByteAddMul(s, dest_a, d, src_ia);
	*(dest + i) = s;
    }
}

/* if the Src is opaque, call fbCombineOverReverseU */
/* if the Dst is opaque, call fbCombineInReverseU */
/* if both the Src and Dst are opaque, call fbCombineDstU */
FASTCALL static void
fbCombineAtopReverseU (pixman_implementation_t *imp, pixman_op_t op,
		       comp4_t *dest, const comp4_t *src, const comp4_t *mask, int width)
{
    int i;
    for (i = 0; i < width; ++i) {
        comp4_t s = combineMask (src, mask, i);
        comp4_t d = *(dest + i);
        comp4_t src_a = Alpha(s);
        comp4_t dest_ia = Alpha(~d);

        FbByteAddMul(s, dest_ia, d, src_a);
	*(dest + i) = s;
    }
}

/* if the Src is opaque, call fbCombineOverU */
/* if the Dst is opaque, call fbCombineOverReverseU */
/* if both the Src and Dst are opaque, call fbCombineClear */
FASTCALL static void
fbCombineXorU (pixman_implementation_t *imp, pixman_op_t op,
	       comp4_t *dest, const comp4_t *src, const comp4_t *mask, int width)
{
    int i;
    for (i = 0; i < width; ++i) {
        comp4_t s = combineMask (src, mask, i);
        comp4_t d = *(dest + i);
        comp4_t src_ia = Alpha(~s);
        comp4_t dest_ia = Alpha(~d);

        FbByteAddMul(s, dest_ia, d, src_ia);
	*(dest + i) = s;
    }
}

FASTCALL static void
fbCombineAddU (pixman_implementation_t *imp, pixman_op_t op,
	       comp4_t *dest, const comp4_t *src, const comp4_t *mask, int width)
{
    int i;
    for (i = 0; i < width; ++i) {
        comp4_t s = combineMask (src, mask, i);
        comp4_t d = *(dest + i);
        FbByteAdd(d, s);
	*(dest + i) = d;
    }
}

/* if the Src is opaque, call fbCombineAddU */
/* if the Dst is opaque, call fbCombineAddU */
/* if both the Src and Dst are opaque, call fbCombineAddU */
FASTCALL static void
fbCombineSaturateU (pixman_implementation_t *imp, pixman_op_t op,
		    comp4_t *dest, const comp4_t *src, const comp4_t *mask, int width)
{
    int i;
    for (i = 0; i < width; ++i) {
        comp4_t s = combineMask (src, mask, i);
        comp4_t d = *(dest + i);
        comp2_t sa, da;

        sa = s >> A_SHIFT;
        da = ~d >> A_SHIFT;
        if (sa > da)
        {
            sa = IntDiv(da, sa);
            FbByteMul(s, sa);
        };
        FbByteAdd(d, s);
	*(dest + i) = d;
    }
}


/*
 * All of the disjoint composing functions

 The four entries in the first column indicate what source contributions
 come from each of the four areas of the picture -- areas covered by neither
 A nor B, areas covered only by A, areas covered only by B and finally
 areas covered by both A and B.

 Disjoint			Conjoint
 Fa		Fb		Fa		Fb
 (0,0,0,0)	0		0		0		0
 (0,A,0,A)	1		0		1		0
 (0,0,B,B)	0		1		0		1
 (0,A,B,A)	1		min((1-a)/b,1)	1		max(1-a/b,0)
 (0,A,B,B)	min((1-b)/a,1)	1		max(1-b/a,0)	1
 (0,0,0,A)	max(1-(1-b)/a,0) 0		min(1,b/a)	0
 (0,0,0,B)	0		max(1-(1-a)/b,0) 0		min(a/b,1)
 (0,A,0,0)	min(1,(1-b)/a)	0		max(1-b/a,0)	0
 (0,0,B,0)	0		min(1,(1-a)/b)	0		max(1-a/b,0)
 (0,0,B,A)	max(1-(1-b)/a,0) min(1,(1-a)/b)	 min(1,b/a)	max(1-a/b,0)
 (0,A,0,B)	min(1,(1-b)/a)	max(1-(1-a)/b,0) max(1-b/a,0)	min(1,a/b)
 (0,A,B,0)	min(1,(1-b)/a)	min(1,(1-a)/b)	max(1-b/a,0)	max(1-a/b,0)

*/

#define CombineAOut 1
#define CombineAIn  2
#define CombineBOut 4
#define CombineBIn  8

#define CombineClear	0
#define CombineA	(CombineAOut|CombineAIn)
#define CombineB	(CombineBOut|CombineBIn)
#define CombineAOver	(CombineAOut|CombineBOut|CombineAIn)
#define CombineBOver	(CombineAOut|CombineBOut|CombineBIn)
#define CombineAAtop	(CombineBOut|CombineAIn)
#define CombineBAtop	(CombineAOut|CombineBIn)
#define CombineXor	(CombineAOut|CombineBOut)

/* portion covered by a but not b */
FASTCALL static comp1_t
fbCombineDisjointOutPart (comp1_t a, comp1_t b)
{
    /* min (1, (1-b) / a) */

    b = ~b;		    /* 1 - b */
    if (b >= a)		    /* 1 - b >= a -> (1-b)/a >= 1 */
	return MASK;	    /* 1 */
    return IntDiv(b,a);     /* (1-b) / a */
}

/* portion covered by both a and b */
FASTCALL static comp1_t
fbCombineDisjointInPart (comp1_t a, comp1_t b)
{
    /* max (1-(1-b)/a,0) */
    /*  = - min ((1-b)/a - 1, 0) */
    /*  = 1 - min (1, (1-b)/a) */

    b = ~b;		    /* 1 - b */
    if (b >= a)		    /* 1 - b >= a -> (1-b)/a >= 1 */
	return 0;	    /* 1 - 1 */
    return ~IntDiv(b,a);    /* 1 - (1-b) / a */
}

/* portion covered by a but not b */
FASTCALL static comp1_t
fbCombineConjointOutPart (comp1_t a, comp1_t b)
{
    /* max (1-b/a,0) */
    /* = 1-min(b/a,1) */

    /* min (1, (1-b) / a) */

    if (b >= a)		    /* b >= a -> b/a >= 1 */
	return 0x00;	    /* 0 */
    return ~IntDiv(b,a);    /* 1 - b/a */
}

/* portion covered by both a and b */
FASTCALL static comp1_t
fbCombineConjointInPart (comp1_t a, comp1_t b)
{
    /* min (1,b/a) */

    if (b >= a)		    /* b >= a -> b/a >= 1 */
	return MASK;	    /* 1 */
    return IntDiv(b,a);     /* b/a */
}

FASTCALL static void
fbCombineDisjointGeneralU (comp4_t *dest, const comp4_t *src, const comp4_t *mask, int width, comp1_t combine)
{
    int i;
    for (i = 0; i < width; ++i) {
        comp4_t s = combineMask (src, mask, i);
        comp4_t d = *(dest + i);
        comp4_t m,n,o,p;
        comp2_t Fa, Fb, t, u, v;
        comp1_t sa = s >> A_SHIFT;
        comp1_t da = d >> A_SHIFT;

        switch (combine & CombineA) {
        default:
            Fa = 0;
            break;
        case CombineAOut:
            Fa = fbCombineDisjointOutPart (sa, da);
            break;
        case CombineAIn:
            Fa = fbCombineDisjointInPart (sa, da);
            break;
        case CombineA:
            Fa = MASK;
            break;
        }

        switch (combine & CombineB) {
        default:
            Fb = 0;
            break;
        case CombineBOut:
            Fb = fbCombineDisjointOutPart (da, sa);
            break;
        case CombineBIn:
            Fb = fbCombineDisjointInPart (da, sa);
            break;
        case CombineB:
            Fb = MASK;
            break;
        }
        m = FbGen (s,d,0,Fa,Fb,t, u, v);
        n = FbGen (s,d,G_SHIFT,Fa,Fb,t, u, v);
        o = FbGen (s,d,R_SHIFT,Fa,Fb,t, u, v);
        p = FbGen (s,d,A_SHIFT,Fa,Fb,t, u, v);
        s = m|n|o|p;
	*(dest + i) = s;
    }
}

FASTCALL static void
fbCombineDisjointOverU (pixman_implementation_t *imp, pixman_op_t op,
			comp4_t *dest, const comp4_t *src, const comp4_t *mask, int width)
{
    int i;
    for (i = 0; i < width; ++i) {
        comp4_t s = combineMask (src, mask, i);
        comp2_t a = s >> A_SHIFT;

        if (a != 0x00)
        {
            if (a != MASK)
            {
                comp4_t d = *(dest + i);
                a = fbCombineDisjointOutPart (d >> A_SHIFT, a);
                FbByteMulAdd(d, a, s);
                s = d;
            }
	    *(dest + i) = s;
        }
    }
}

FASTCALL static void
fbCombineDisjointInU (pixman_implementation_t *imp, pixman_op_t op,
		      comp4_t *dest, const comp4_t *src, const comp4_t *mask, int width)
{
    fbCombineDisjointGeneralU (dest, src, mask, width, CombineAIn);
}

FASTCALL static void
fbCombineDisjointInReverseU (pixman_implementation_t *imp, pixman_op_t op,
			     comp4_t *dest, const comp4_t *src, const comp4_t *mask, int width)
{
    fbCombineDisjointGeneralU (dest, src, mask, width, CombineBIn);
}

FASTCALL static void
fbCombineDisjointOutU (pixman_implementation_t *imp, pixman_op_t op,
		       comp4_t *dest, const comp4_t *src, const comp4_t *mask, int width)
{
    fbCombineDisjointGeneralU (dest, src, mask, width, CombineAOut);
}

FASTCALL static void
fbCombineDisjointOutReverseU (pixman_implementation_t *imp, pixman_op_t op,
			      comp4_t *dest, const comp4_t *src, const comp4_t *mask, int width)
{
    fbCombineDisjointGeneralU (dest, src, mask, width, CombineBOut);
}

FASTCALL static void
fbCombineDisjointAtopU (pixman_implementation_t *imp, pixman_op_t op,
			comp4_t *dest, const comp4_t *src, const comp4_t *mask, int width)
{
    fbCombineDisjointGeneralU (dest, src, mask, width, CombineAAtop);
}

FASTCALL static void
fbCombineDisjointAtopReverseU (pixman_implementation_t *imp, pixman_op_t op,
			       comp4_t *dest, const comp4_t *src, const comp4_t *mask, int width)
{
    fbCombineDisjointGeneralU (dest, src, mask, width, CombineBAtop);
}

FASTCALL static void
fbCombineDisjointXorU (pixman_implementation_t *imp, pixman_op_t op,
		       comp4_t *dest, const comp4_t *src, const comp4_t *mask, int width)
{
    fbCombineDisjointGeneralU (dest, src, mask, width, CombineXor);
}

FASTCALL static void
fbCombineConjointGeneralU (comp4_t *dest, const comp4_t *src, const comp4_t *mask, int width, comp1_t combine)
{
    int i;
    for (i = 0; i < width; ++i) {
        comp4_t s = combineMask (src, mask, i);
        comp4_t d = *(dest + i);
        comp4_t m,n,o,p;
        comp2_t Fa, Fb, t, u, v;
        comp1_t sa = s >> A_SHIFT;
        comp1_t da = d >> A_SHIFT;

        switch (combine & CombineA) {
        default:
            Fa = 0;
            break;
        case CombineAOut:
            Fa = fbCombineConjointOutPart (sa, da);
            break;
        case CombineAIn:
            Fa = fbCombineConjointInPart (sa, da);
            break;
        case CombineA:
            Fa = MASK;
            break;
        }

        switch (combine & CombineB) {
        default:
            Fb = 0;
            break;
        case CombineBOut:
            Fb = fbCombineConjointOutPart (da, sa);
            break;
        case CombineBIn:
            Fb = fbCombineConjointInPart (da, sa);
            break;
        case CombineB:
            Fb = MASK;
            break;
        }
        m = FbGen (s,d,0,Fa,Fb,t, u, v);
        n = FbGen (s,d,G_SHIFT,Fa,Fb,t, u, v);
        o = FbGen (s,d,R_SHIFT,Fa,Fb,t, u, v);
        p = FbGen (s,d,A_SHIFT,Fa,Fb,t, u, v);
        s = m|n|o|p;
	*(dest + i) = s;
    }
}

FASTCALL static void
fbCombineConjointOverU (pixman_implementation_t *imp, pixman_op_t op,
			comp4_t *dest, const comp4_t *src, const comp4_t *mask, int width)
{
    fbCombineConjointGeneralU (dest, src, mask, width, CombineAOver);
}


FASTCALL static void
fbCombineConjointOverReverseU (pixman_implementation_t *imp, pixman_op_t op,
			       comp4_t *dest, const comp4_t *src, const comp4_t *mask, int width)
{
    fbCombineConjointGeneralU (dest, src, mask, width, CombineBOver);
}


FASTCALL static void
fbCombineConjointInU (pixman_implementation_t *imp, pixman_op_t op,
		      comp4_t *dest, const comp4_t *src, const comp4_t *mask, int width)
{
    fbCombineConjointGeneralU (dest, src, mask, width, CombineAIn);
}


FASTCALL static void
fbCombineConjointInReverseU (pixman_implementation_t *imp, pixman_op_t op,
			     comp4_t *dest, const comp4_t *src, const comp4_t *mask, int width)
{
    fbCombineConjointGeneralU (dest, src, mask, width, CombineBIn);
}

FASTCALL static void
fbCombineConjointOutU (pixman_implementation_t *imp, pixman_op_t op,
		       comp4_t *dest, const comp4_t *src, const comp4_t *mask, int width)
{
    fbCombineConjointGeneralU (dest, src, mask, width, CombineAOut);
}

FASTCALL static void
fbCombineConjointOutReverseU (pixman_implementation_t *imp, pixman_op_t op,
			      comp4_t *dest, const comp4_t *src, const comp4_t *mask, int width)
{
    fbCombineConjointGeneralU (dest, src, mask, width, CombineBOut);
}

FASTCALL static void
fbCombineConjointAtopU (pixman_implementation_t *imp, pixman_op_t op,
			comp4_t *dest, const comp4_t *src, const comp4_t *mask, int width)
{
    fbCombineConjointGeneralU (dest, src, mask, width, CombineAAtop);
}

FASTCALL static void
fbCombineConjointAtopReverseU (pixman_implementation_t *imp, pixman_op_t op,
			       comp4_t *dest, const comp4_t *src, const comp4_t *mask, int width)
{
    fbCombineConjointGeneralU (dest, src, mask, width, CombineBAtop);
}

FASTCALL static void
fbCombineConjointXorU (pixman_implementation_t *imp, pixman_op_t op,
		       comp4_t *dest, const comp4_t *src, const comp4_t *mask, int width)
{
    fbCombineConjointGeneralU (dest, src, mask, width, CombineXor);
}

/********************************************************************************/
/*************************** Per Channel functions ******************************/
/********************************************************************************/

FASTCALL static void
fbCombineMaskC (comp4_t *src, comp4_t *mask)
{
    comp4_t a = *mask;

    comp4_t	x;
    comp2_t	xa;

    if (!a)
    {
	*(src) = 0;
	return;
    }

    x = *(src);
    if (a == ~0)
    {
	x = x >> A_SHIFT;
	x |= x << G_SHIFT;
	x |= x << R_SHIFT;
	*(mask) = x;
	return;
    }

    xa = x >> A_SHIFT;
    FbByteMulC(x, a);
    *(src) = x;
    FbByteMul(a, xa);
    *(mask) = a;
}

FASTCALL static void
fbCombineMaskValueC (comp4_t *src, const comp4_t *mask)
{
    comp4_t a = *mask;
    comp4_t	x;

    if (!a)
    {
	*(src) = 0;
	return;
    }

    if (a == ~0)
	return;

    x = *(src);
    FbByteMulC(x, a);
    *(src) =x;
}

FASTCALL static void
fbCombineMaskAlphaC (const comp4_t *src, comp4_t *mask)
{
    comp4_t a = *(mask);
    comp4_t	x;

    if (!a)
	return;

    x = *(src) >> A_SHIFT;
    if (x == MASK)
	return;
    if (a == ~0)
    {
	x = x >> A_SHIFT;
	x |= x << G_SHIFT;
	x |= x << R_SHIFT;
	*(mask) = x;
	return;
    }

    FbByteMul(a, x);
    *(mask) = a;
}

FASTCALL static void
fbCombineClearC (pixman_implementation_t *imp, pixman_op_t op,
		 comp4_t *dest, const comp4_t *src, const comp4_t *mask, int width)
{
    memset(dest, 0, width*sizeof(comp4_t));
}

FASTCALL static void
fbCombineSrcC (pixman_implementation_t *imp, pixman_op_t op,
	       comp4_t *dest, const comp4_t *src, const comp4_t *mask, int width)
{
    int i;

    for (i = 0; i < width; ++i) {
	comp4_t s = *(src + i);
	comp4_t m = *(mask + i);

	fbCombineMaskValueC (&s, &m);

	*(dest) = s;
    }
}

FASTCALL static void
fbCombineOverC (pixman_implementation_t *imp, pixman_op_t op,
		comp4_t *dest, const comp4_t *src, const comp4_t *mask, int width)
{
    int i;

    for (i = 0; i < width; ++i) {
	comp4_t s = *(src + i);
	comp4_t m = *(mask + i);
	comp4_t a;

	fbCombineMaskC (&s, &m);

	a = ~m;
        if (a != ~0)
        {
            if (a)
            {
                comp4_t d = *(dest + i);
                FbByteMulAddC(d, a, s);
                s = d;
            }
	    *(dest + i) = s;
        }
    }
}

FASTCALL static void
fbCombineOverReverseC (pixman_implementation_t *imp, pixman_op_t op,
		       comp4_t *dest, const comp4_t *src, const comp4_t *mask, int width)
{
    int i;

    for (i = 0; i < width; ++i) {
        comp4_t d = *(dest + i);
        comp4_t a = ~d >> A_SHIFT;

        if (a)
        {
            comp4_t s = *(src + i);
	    comp4_t m = *(mask + i);

	    fbCombineMaskValueC (&s, &m);

            if (a != MASK)
            {
                FbByteMulAdd(s, a, d);
            }
	    *(dest + i) = s;
        }
    }
}

FASTCALL static void
fbCombineInC (pixman_implementation_t *imp, pixman_op_t op,
	      comp4_t *dest, const comp4_t *src, const comp4_t *mask, int width)
{
    int i;

    for (i = 0; i < width; ++i) {
        comp4_t d = *(dest + i);
        comp2_t a = d >> A_SHIFT;
        comp4_t s = 0;
        if (a)
        {
	    comp4_t m = *(mask + i);

	    s = *(src + i);
	    fbCombineMaskValueC (&s, &m);
            if (a != MASK)
            {
                FbByteMul(s, a);
            }
        }
	*(dest + i) = s;
    }
}

FASTCALL static void
fbCombineInReverseC (pixman_implementation_t *imp, pixman_op_t op,
		     comp4_t *dest, const comp4_t *src, const comp4_t *mask, int width)
{
    int i;

    for (i = 0; i < width; ++i) {
        comp4_t s = *(src + i);
        comp4_t m = *(mask + i);
        comp4_t a;

	fbCombineMaskAlphaC (&s, &m);

	a = m;
        if (a != ~0)
        {
            comp4_t d = 0;
            if (a)
            {
                d = *(dest + i);
                FbByteMulC(d, a);
            }
	    *(dest + i) = d;
        }
    }
}

FASTCALL static void
fbCombineOutC (pixman_implementation_t *imp, pixman_op_t op,
	       comp4_t *dest, const comp4_t *src, const comp4_t *mask, int width)
{
    int i;

    for (i = 0; i < width; ++i) {
        comp4_t d = *(dest + i);
        comp2_t a = ~d >> A_SHIFT;
        comp4_t s = 0;
        if (a)
        {
	    comp4_t m = *(mask + i);

	    s = *(src + i);
	    fbCombineMaskValueC (&s, &m);

            if (a != MASK)
            {
                FbByteMul(s, a);
            }
        }
	*(dest + i) = s;
    }
}

FASTCALL static void
fbCombineOutReverseC (pixman_implementation_t *imp, pixman_op_t op,
		      comp4_t *dest, const comp4_t *src, const comp4_t *mask, int width)
{
    int i;

    for (i = 0; i < width; ++i) {
	comp4_t s = *(src + i);
	comp4_t m = *(mask + i);
	comp4_t a;

	fbCombineMaskAlphaC (&s, &m);

        a = ~m;
        if (a != ~0)
        {
            comp4_t d = 0;
            if (a)
            {
                d = *(dest + i);
                FbByteMulC(d, a);
            }
	    *(dest + i) = d;
        }
    }
}

FASTCALL static void
fbCombineAtopC (pixman_implementation_t *imp, pixman_op_t op,
		comp4_t *dest, const comp4_t *src, const comp4_t *mask, int width)
{
    int i;

    for (i = 0; i < width; ++i) {
        comp4_t d = *(dest + i);
        comp4_t s = *(src + i);
        comp4_t m = *(mask + i);
        comp4_t ad;
        comp2_t as = d >> A_SHIFT;

	fbCombineMaskC (&s, &m);

        ad = ~m;

        FbByteAddMulC(d, ad, s, as);
	*(dest + i) = d;
    }
}

FASTCALL static void
fbCombineAtopReverseC (pixman_implementation_t *imp, pixman_op_t op,
		       comp4_t *dest, const comp4_t *src, const comp4_t *mask, int width)
{
    int i;

    for (i = 0; i < width; ++i) {

        comp4_t d = *(dest + i);
        comp4_t s = *(src + i);
        comp4_t m = *(mask + i);
        comp4_t ad;
        comp2_t as = ~d >> A_SHIFT;

	fbCombineMaskC (&s, &m);

	ad = m;

        FbByteAddMulC(d, ad, s, as);
	*(dest + i) = d;
    }
}

FASTCALL static void
fbCombineXorC (pixman_implementation_t *imp, pixman_op_t op,
	       comp4_t *dest, const comp4_t *src, const comp4_t *mask, int width)
{
    int i;

    for (i = 0; i < width; ++i) {
        comp4_t d = *(dest + i);
        comp4_t s = *(src + i);
        comp4_t m = *(mask + i);
        comp4_t ad;
        comp2_t as = ~d >> A_SHIFT;

	fbCombineMaskC (&s, &m);

	ad = ~m;

        FbByteAddMulC(d, ad, s, as);
	*(dest + i) = d;
    }
}

FASTCALL static void
fbCombineAddC (pixman_implementation_t *imp, pixman_op_t op,
	       comp4_t *dest, const comp4_t *src, const comp4_t *mask, int width)
{
    int i;

    for (i = 0; i < width; ++i) {
        comp4_t s = *(src + i);
        comp4_t m = *(mask + i);
        comp4_t d = *(dest + i);

	fbCombineMaskValueC (&s, &m);

        FbByteAdd(d, s);
	*(dest + i) = d;
    }
}

FASTCALL static void
fbCombineSaturateC (pixman_implementation_t *imp, pixman_op_t op,
		    comp4_t *dest, const comp4_t *src, const comp4_t *mask, int width)
{
    int i;

    for (i = 0; i < width; ++i) {
        comp4_t s, d;
        comp2_t sa, sr, sg, sb, da;
        comp2_t t, u, v;
        comp4_t m,n,o,p;

        d = *(dest + i);
        s = *(src + i);
	m = *(mask + i);

	fbCombineMaskC (&s, &m);

        sa = (m >> A_SHIFT);
        sr = (m >> R_SHIFT) & MASK;
        sg = (m >> G_SHIFT) & MASK;
        sb =  m             & MASK;
        da = ~d >> A_SHIFT;

        if (sb <= da)
            m = Add(s,d,0,t);
        else
            m = FbGen (s, d, 0, (da << G_SHIFT) / sb, MASK, t, u, v);

        if (sg <= da)
            n = Add(s,d,G_SHIFT,t);
        else
            n = FbGen (s, d, G_SHIFT, (da << G_SHIFT) / sg, MASK, t, u, v);

        if (sr <= da)
            o = Add(s,d,R_SHIFT,t);
        else
            o = FbGen (s, d, R_SHIFT, (da << G_SHIFT) / sr, MASK, t, u, v);

        if (sa <= da)
            p = Add(s,d,A_SHIFT,t);
        else
            p = FbGen (s, d, A_SHIFT, (da << G_SHIFT) / sa, MASK, t, u, v);

	*(dest + i) = m|n|o|p;
    }
}

FASTCALL static void
fbCombineDisjointGeneralC (comp4_t *dest, const comp4_t *src, const comp4_t *mask, int width, comp1_t combine)
{
    int i;

    for (i = 0; i < width; ++i) {
        comp4_t s, d;
        comp4_t m,n,o,p;
        comp4_t Fa, Fb;
        comp2_t t, u, v;
        comp4_t sa;
        comp1_t da;

        s = *(src + i);
        m = *(mask + i);
        d = *(dest + i);
        da = d >> A_SHIFT;

	fbCombineMaskC (&s, &m);

	sa = m;

        switch (combine & CombineA) {
        default:
            Fa = 0;
            break;
        case CombineAOut:
            m = (comp4_t)fbCombineDisjointOutPart ((comp1_t) (sa >> 0), da);
            n = (comp4_t)fbCombineDisjointOutPart ((comp1_t) (sa >> G_SHIFT), da) << G_SHIFT;
            o = (comp4_t)fbCombineDisjointOutPart ((comp1_t) (sa >> R_SHIFT), da) << R_SHIFT;
            p = (comp4_t)fbCombineDisjointOutPart ((comp1_t) (sa >> A_SHIFT), da) << A_SHIFT;
            Fa = m|n|o|p;
            break;
        case CombineAIn:
            m = (comp4_t)fbCombineDisjointInPart ((comp1_t) (sa >> 0), da);
            n = (comp4_t)fbCombineDisjointInPart ((comp1_t) (sa >> G_SHIFT), da) << G_SHIFT;
            o = (comp4_t)fbCombineDisjointInPart ((comp1_t) (sa >> R_SHIFT), da) << R_SHIFT;
            p = (comp4_t)fbCombineDisjointInPart ((comp1_t) (sa >> A_SHIFT), da) << A_SHIFT;
            Fa = m|n|o|p;
            break;
        case CombineA:
            Fa = ~0;
            break;
        }

        switch (combine & CombineB) {
        default:
            Fb = 0;
            break;
        case CombineBOut:
            m = (comp4_t)fbCombineDisjointOutPart (da, (comp1_t) (sa >> 0));
            n = (comp4_t)fbCombineDisjointOutPart (da, (comp1_t) (sa >> G_SHIFT)) << G_SHIFT;
            o = (comp4_t)fbCombineDisjointOutPart (da, (comp1_t) (sa >> R_SHIFT)) << R_SHIFT;
            p = (comp4_t)fbCombineDisjointOutPart (da, (comp1_t) (sa >> A_SHIFT)) << A_SHIFT;
            Fb = m|n|o|p;
            break;
        case CombineBIn:
            m = (comp4_t)fbCombineDisjointInPart (da, (comp1_t) (sa >> 0));
            n = (comp4_t)fbCombineDisjointInPart (da, (comp1_t) (sa >> G_SHIFT)) << G_SHIFT;
            o = (comp4_t)fbCombineDisjointInPart (da, (comp1_t) (sa >> R_SHIFT)) << R_SHIFT;
            p = (comp4_t)fbCombineDisjointInPart (da, (comp1_t) (sa >> A_SHIFT)) << A_SHIFT;
            Fb = m|n|o|p;
            break;
        case CombineB:
            Fb = ~0;
            break;
        }
        m = FbGen (s,d,0,GetComp(Fa,0),GetComp(Fb,0),t, u, v);
        n = FbGen (s,d,G_SHIFT,GetComp(Fa,G_SHIFT),GetComp(Fb,G_SHIFT),t, u, v);
        o = FbGen (s,d,R_SHIFT,GetComp(Fa,R_SHIFT),GetComp(Fb,R_SHIFT),t, u, v);
        p = FbGen (s,d,A_SHIFT,GetComp(Fa,A_SHIFT),GetComp(Fb,A_SHIFT),t, u, v);
        s = m|n|o|p;
	*(dest + i) = s;
    }
}

FASTCALL static void
fbCombineDisjointOverC (pixman_implementation_t *imp, pixman_op_t op,
			comp4_t *dest, const comp4_t *src, const comp4_t *mask, int width)
{
    fbCombineDisjointGeneralC (dest, src, mask, width, CombineAOver);
}

FASTCALL static void
fbCombineDisjointInC (pixman_implementation_t *imp, pixman_op_t op,
		      comp4_t *dest, const comp4_t *src, const comp4_t *mask, int width)
{
    fbCombineDisjointGeneralC (dest, src, mask, width, CombineAIn);
}

FASTCALL static void
fbCombineDisjointInReverseC (pixman_implementation_t *imp, pixman_op_t op,
			     comp4_t *dest, const comp4_t *src, const comp4_t *mask, int width)
{
    fbCombineDisjointGeneralC (dest, src, mask, width, CombineBIn);
}

FASTCALL static void
fbCombineDisjointOutC (pixman_implementation_t *imp, pixman_op_t op,
		       comp4_t *dest, const comp4_t *src, const comp4_t *mask, int width)
{
    fbCombineDisjointGeneralC (dest, src, mask, width, CombineAOut);
}

FASTCALL static void
fbCombineDisjointOutReverseC (pixman_implementation_t *imp, pixman_op_t op,
			      comp4_t *dest, const comp4_t *src, const comp4_t *mask, int width)
{
    fbCombineDisjointGeneralC (dest, src, mask, width, CombineBOut);
}

FASTCALL static void
fbCombineDisjointAtopC (pixman_implementation_t *imp, pixman_op_t op,
			comp4_t *dest, const comp4_t *src, const comp4_t *mask, int width)
{
    fbCombineDisjointGeneralC (dest, src, mask, width, CombineAAtop);
}

FASTCALL static void
fbCombineDisjointAtopReverseC (pixman_implementation_t *imp, pixman_op_t op,
			       comp4_t *dest, const comp4_t *src, const comp4_t *mask, int width)
{
    fbCombineDisjointGeneralC (dest, src, mask, width, CombineBAtop);
}

FASTCALL static void
fbCombineDisjointXorC (pixman_implementation_t *imp, pixman_op_t op,
		       comp4_t *dest, const comp4_t *src, const comp4_t *mask, int width)
{
    fbCombineDisjointGeneralC (dest, src, mask, width, CombineXor);
}

FASTCALL static void
fbCombineConjointGeneralC (comp4_t *dest, const comp4_t *src, const comp4_t *mask, int width, comp1_t combine)
{
    int i;

    for (i = 0; i < width; ++i) {
        comp4_t s, d;
        comp4_t m,n,o,p;
        comp4_t Fa, Fb;
        comp2_t t, u, v;
        comp4_t sa;
        comp1_t da;

        s = *(src + i);
        m = *(mask + i);
        d = *(dest + i);
        da = d >> A_SHIFT;

	fbCombineMaskC (&s, &m);

        sa = m;

        switch (combine & CombineA) {
        default:
            Fa = 0;
            break;
        case CombineAOut:
            m = (comp4_t)fbCombineConjointOutPart ((comp1_t) (sa >> 0), da);
            n = (comp4_t)fbCombineConjointOutPart ((comp1_t) (sa >> G_SHIFT), da) << G_SHIFT;
            o = (comp4_t)fbCombineConjointOutPart ((comp1_t) (sa >> R_SHIFT), da) << R_SHIFT;
            p = (comp4_t)fbCombineConjointOutPart ((comp1_t) (sa >> A_SHIFT), da) << A_SHIFT;
            Fa = m|n|o|p;
            break;
        case CombineAIn:
            m = (comp4_t)fbCombineConjointInPart ((comp1_t) (sa >> 0), da);
            n = (comp4_t)fbCombineConjointInPart ((comp1_t) (sa >> G_SHIFT), da) << G_SHIFT;
            o = (comp4_t)fbCombineConjointInPart ((comp1_t) (sa >> R_SHIFT), da) << R_SHIFT;
            p = (comp4_t)fbCombineConjointInPart ((comp1_t) (sa >> A_SHIFT), da) << A_SHIFT;
            Fa = m|n|o|p;
            break;
        case CombineA:
            Fa = ~0;
            break;
        }

        switch (combine & CombineB) {
        default:
            Fb = 0;
            break;
        case CombineBOut:
            m = (comp4_t)fbCombineConjointOutPart (da, (comp1_t) (sa >> 0));
            n = (comp4_t)fbCombineConjointOutPart (da, (comp1_t) (sa >> G_SHIFT)) << G_SHIFT;
            o = (comp4_t)fbCombineConjointOutPart (da, (comp1_t) (sa >> R_SHIFT)) << R_SHIFT;
            p = (comp4_t)fbCombineConjointOutPart (da, (comp1_t) (sa >> A_SHIFT)) << A_SHIFT;
            Fb = m|n|o|p;
            break;
        case CombineBIn:
            m = (comp4_t)fbCombineConjointInPart (da, (comp1_t) (sa >> 0));
            n = (comp4_t)fbCombineConjointInPart (da, (comp1_t) (sa >> G_SHIFT)) << G_SHIFT;
            o = (comp4_t)fbCombineConjointInPart (da, (comp1_t) (sa >> R_SHIFT)) << R_SHIFT;
            p = (comp4_t)fbCombineConjointInPart (da, (comp1_t) (sa >> A_SHIFT)) << A_SHIFT;
            Fb = m|n|o|p;
            break;
        case CombineB:
            Fb = ~0;
            break;
        }
        m = FbGen (s,d,0,GetComp(Fa,0),GetComp(Fb,0),t, u, v);
        n = FbGen (s,d,G_SHIFT,GetComp(Fa,G_SHIFT),GetComp(Fb,G_SHIFT),t, u, v);
        o = FbGen (s,d,R_SHIFT,GetComp(Fa,R_SHIFT),GetComp(Fb,R_SHIFT),t, u, v);
        p = FbGen (s,d,A_SHIFT,GetComp(Fa,A_SHIFT),GetComp(Fb,A_SHIFT),t, u, v);
        s = m|n|o|p;
	*(dest + i) = s;
    }
}

FASTCALL static void
fbCombineConjointOverC (pixman_implementation_t *imp, pixman_op_t op,
			comp4_t *dest, const comp4_t *src, const comp4_t *mask, int width)
{
    fbCombineConjointGeneralC (dest, src, mask, width, CombineAOver);
}

FASTCALL static void
fbCombineConjointOverReverseC (pixman_implementation_t *imp, pixman_op_t op,
			       comp4_t *dest, const comp4_t *src, const comp4_t *mask, int width)
{
    fbCombineConjointGeneralC (dest, src, mask, width, CombineBOver);
}

FASTCALL static void
fbCombineConjointInC (pixman_implementation_t *imp, pixman_op_t op,
		      comp4_t *dest, const comp4_t *src, const comp4_t *mask, int width)
{
    fbCombineConjointGeneralC (dest, src, mask, width, CombineAIn);
}

FASTCALL static void
fbCombineConjointInReverseC (pixman_implementation_t *imp, pixman_op_t op,
			     comp4_t *dest, const comp4_t *src, const comp4_t *mask, int width)
{
    fbCombineConjointGeneralC (dest, src, mask, width, CombineBIn);
}

FASTCALL static void
fbCombineConjointOutC (pixman_implementation_t *imp, pixman_op_t op,
		       comp4_t *dest, const comp4_t *src, const comp4_t *mask, int width)
{
    fbCombineConjointGeneralC (dest, src, mask, width, CombineAOut);
}

FASTCALL static void
fbCombineConjointOutReverseC (pixman_implementation_t *imp, pixman_op_t op,
			      comp4_t *dest, const comp4_t *src, const comp4_t *mask, int width)
{
    fbCombineConjointGeneralC (dest, src, mask, width, CombineBOut);
}

FASTCALL static void
fbCombineConjointAtopC (pixman_implementation_t *imp, pixman_op_t op,
			comp4_t *dest, const comp4_t *src, const comp4_t *mask, int width)
{
    fbCombineConjointGeneralC (dest, src, mask, width, CombineAAtop);
}

FASTCALL static void
fbCombineConjointAtopReverseC (pixman_implementation_t *imp, pixman_op_t op,
			       comp4_t *dest, const comp4_t *src, const comp4_t *mask, int width)
{
    fbCombineConjointGeneralC (dest, src, mask, width, CombineBAtop);
}

FASTCALL static void
fbCombineConjointXorC (pixman_implementation_t *imp, pixman_op_t op,
		       comp4_t *dest, const comp4_t *src, const comp4_t *mask, int width)
{
    fbCombineConjointGeneralC (dest, src, mask, width, CombineXor);
}

void
_pixman_setup_combiner_functions_width (pixman_implementation_t *imp)
{
    /* Unified alpha */
    imp->combine_width[PIXMAN_OP_CLEAR] = fbCombineClear;
    imp->combine_width[PIXMAN_OP_SRC] = fbCombineSrcU;
    /* dest */
    imp->combine_width[PIXMAN_OP_OVER] = fbCombineOverU;
    imp->combine_width[PIXMAN_OP_OVER_REVERSE] = fbCombineOverReverseU;
    imp->combine_width[PIXMAN_OP_IN] = fbCombineInU;
    imp->combine_width[PIXMAN_OP_IN_REVERSE] = fbCombineInReverseU;
    imp->combine_width[PIXMAN_OP_OUT] = fbCombineOutU;
    imp->combine_width[PIXMAN_OP_OUT_REVERSE] = fbCombineOutReverseU;
    imp->combine_width[PIXMAN_OP_ATOP] = fbCombineAtopU;
    imp->combine_width[PIXMAN_OP_ATOP_REVERSE] = fbCombineAtopReverseU;
    imp->combine_width[PIXMAN_OP_XOR] = fbCombineXorU;
    imp->combine_width[PIXMAN_OP_ADD] = fbCombineAddU;
    imp->combine_width[PIXMAN_OP_SATURATE] = fbCombineSaturateU;

    /* Disjoint, unified */
    imp->combine_width[PIXMAN_OP_DISJOINT_CLEAR] = fbCombineClear;
    imp->combine_width[PIXMAN_OP_DISJOINT_SRC] = fbCombineSrcU;
    /* dest */
    imp->combine_width[PIXMAN_OP_DISJOINT_OVER] = fbCombineDisjointOverU;
    imp->combine_width[PIXMAN_OP_DISJOINT_OVER_REVERSE] = fbCombineSaturateU;
    imp->combine_width[PIXMAN_OP_DISJOINT_IN] = fbCombineDisjointInU;
    imp->combine_width[PIXMAN_OP_DISJOINT_IN_REVERSE] = fbCombineDisjointInReverseU;
    imp->combine_width[PIXMAN_OP_DISJOINT_OUT] = fbCombineDisjointOutU;
    imp->combine_width[PIXMAN_OP_DISJOINT_OUT_REVERSE] = fbCombineDisjointOutReverseU;
    imp->combine_width[PIXMAN_OP_DISJOINT_ATOP] = fbCombineDisjointAtopU;
    imp->combine_width[PIXMAN_OP_DISJOINT_ATOP_REVERSE] = fbCombineDisjointAtopReverseU;
    imp->combine_width[PIXMAN_OP_DISJOINT_XOR] = fbCombineDisjointXorU;

    /* Conjoint, unified */
    imp->combine_width[PIXMAN_OP_CONJOINT_CLEAR] = fbCombineClear;
    imp->combine_width[PIXMAN_OP_CONJOINT_SRC] = fbCombineSrcU;
    /* dest */
    imp->combine_width[PIXMAN_OP_CONJOINT_OVER] = fbCombineConjointOverU;
    imp->combine_width[PIXMAN_OP_CONJOINT_OVER_REVERSE] = fbCombineConjointOverReverseU;
    imp->combine_width[PIXMAN_OP_CONJOINT_IN] = fbCombineConjointInU;
    imp->combine_width[PIXMAN_OP_CONJOINT_IN_REVERSE] = fbCombineConjointInReverseU;
    imp->combine_width[PIXMAN_OP_CONJOINT_OUT] = fbCombineConjointOutU;
    imp->combine_width[PIXMAN_OP_CONJOINT_OUT_REVERSE] = fbCombineConjointOutReverseU;
    imp->combine_width[PIXMAN_OP_CONJOINT_ATOP] = fbCombineConjointAtopU;
    imp->combine_width[PIXMAN_OP_CONJOINT_ATOP_REVERSE] = fbCombineConjointAtopReverseU;
    imp->combine_width[PIXMAN_OP_CONJOINT_XOR] = fbCombineConjointXorU;

    /* Component alpha combiners */
    imp->combine_width_ca[PIXMAN_OP_CLEAR] = fbCombineClearC;
    imp->combine_width_ca[PIXMAN_OP_SRC] = fbCombineSrcC;
    /* dest */
    imp->combine_width_ca[PIXMAN_OP_OVER] = fbCombineOverC;
    imp->combine_width_ca[PIXMAN_OP_OVER_REVERSE] = fbCombineOverReverseC;
    imp->combine_width_ca[PIXMAN_OP_IN] = fbCombineInC;
    imp->combine_width_ca[PIXMAN_OP_IN_REVERSE] = fbCombineInReverseC;
    imp->combine_width_ca[PIXMAN_OP_OUT] = fbCombineOutC;
    imp->combine_width_ca[PIXMAN_OP_OUT_REVERSE] = fbCombineOutReverseC;
    imp->combine_width_ca[PIXMAN_OP_ATOP] = fbCombineAtopC;
    imp->combine_width_ca[PIXMAN_OP_ATOP_REVERSE] = fbCombineAtopReverseC;
    imp->combine_width_ca[PIXMAN_OP_XOR] = fbCombineXorC;
    imp->combine_width_ca[PIXMAN_OP_ADD] = fbCombineAddC;
    imp->combine_width_ca[PIXMAN_OP_SATURATE] = fbCombineSaturateC;

    /* Disjoint CA */
    imp->combine_width_ca[PIXMAN_OP_DISJOINT_CLEAR] = fbCombineClearC;
    imp->combine_width_ca[PIXMAN_OP_DISJOINT_SRC] = fbCombineSrcC;
    /* dest */
    imp->combine_width_ca[PIXMAN_OP_DISJOINT_OVER] = fbCombineDisjointOverC;
    imp->combine_width_ca[PIXMAN_OP_DISJOINT_OVER_REVERSE] = fbCombineSaturateC,
    imp->combine_width_ca[PIXMAN_OP_DISJOINT_IN] = fbCombineDisjointInC;
    imp->combine_width_ca[PIXMAN_OP_DISJOINT_IN_REVERSE] = fbCombineDisjointInReverseC;
    imp->combine_width_ca[PIXMAN_OP_DISJOINT_OUT] = fbCombineDisjointOutC;
    imp->combine_width_ca[PIXMAN_OP_DISJOINT_OUT_REVERSE] = fbCombineDisjointOutReverseC;
    imp->combine_width_ca[PIXMAN_OP_DISJOINT_ATOP] = fbCombineDisjointAtopC;
    imp->combine_width_ca[PIXMAN_OP_DISJOINT_ATOP_REVERSE] = fbCombineDisjointAtopReverseC;
    imp->combine_width_ca[PIXMAN_OP_DISJOINT_XOR] = fbCombineDisjointXorC;

    /* Conjoint CA */
    imp->combine_width_ca[PIXMAN_OP_CONJOINT_CLEAR] = fbCombineClearC;
    imp->combine_width_ca[PIXMAN_OP_CONJOINT_SRC] = fbCombineSrcC;
    /* dest */
    imp->combine_width_ca[PIXMAN_OP_CONJOINT_OVER] = fbCombineConjointOverC;
    imp->combine_width_ca[PIXMAN_OP_CONJOINT_OVER_REVERSE] = fbCombineConjointOverReverseC;
    imp->combine_width_ca[PIXMAN_OP_CONJOINT_IN] = fbCombineConjointInC;
    imp->combine_width_ca[PIXMAN_OP_CONJOINT_IN_REVERSE] = fbCombineConjointInReverseC;
    imp->combine_width_ca[PIXMAN_OP_CONJOINT_OUT] = fbCombineConjointOutC;
    imp->combine_width_ca[PIXMAN_OP_CONJOINT_OUT_REVERSE] = fbCombineConjointOutReverseC;
    imp->combine_width_ca[PIXMAN_OP_CONJOINT_ATOP] = fbCombineConjointAtopC;
    imp->combine_width_ca[PIXMAN_OP_CONJOINT_ATOP_REVERSE] = fbCombineConjointAtopReverseC;
    imp->combine_width_ca[PIXMAN_OP_CONJOINT_XOR] = fbCombineConjointXorC;
}
