/*
 * Copyright © 2000 SuSE, Inc.
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of SuSE not be used in advertising or
 * publicity pertaining to distribution of the software without specific,
 * written prior permission.  SuSE makes no representations about the
 * suitability of this software for any purpose.  It is provided "as is"
 * without express or implied warranty.
 *
 * SuSE DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING ALL
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL SuSE
 * BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * Author:  Keith Packard, SuSE, Inc.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdlib.h>

#include "pixman-private.h"

/*
 * Compute the smallest value no less than y which is on a
 * grid row
 */

PIXMAN_EXPORT pixman_fixed_t
pixman_sample_ceil_y (pixman_fixed_t y, int n)
{
    pixman_fixed_t   f = pixman_fixed_frac(y);
    pixman_fixed_t   i = pixman_fixed_floor(y);

    f = ((f + Y_FRAC_FIRST(n)) / STEP_Y_SMALL(n)) * STEP_Y_SMALL(n) + Y_FRAC_FIRST(n);
    if (f > Y_FRAC_LAST(n))
    {
	if (pixman_fixed_to_int(i) == 0x7fff)
	{
	    f = 0xffff; /* saturate */
	} else {
	    f = Y_FRAC_FIRST(n);
	    i += pixman_fixed_1;
	}
    }
    return (i | f);
}

#define _div(a,b)    ((a) >= 0 ? (a) / (b) : -((-(a) + (b) - 1) / (b)))

/*
 * Compute the largest value no greater than y which is on a
 * grid row
 */
PIXMAN_EXPORT pixman_fixed_t
pixman_sample_floor_y (pixman_fixed_t y, int n)
{
    pixman_fixed_t   f = pixman_fixed_frac(y);
    pixman_fixed_t   i = pixman_fixed_floor (y);

    f = _div(f - Y_FRAC_FIRST(n), STEP_Y_SMALL(n)) * STEP_Y_SMALL(n) + Y_FRAC_FIRST(n);
    if (f < Y_FRAC_FIRST(n))
    {
	if (pixman_fixed_to_int(i) == 0x8000)
	{
	    f = 0; /* saturate */
	} else {
	    f = Y_FRAC_LAST(n);
	    i -= pixman_fixed_1;
	}
    }
    return (i | f);
}

/*
 * Step an edge by any amount (including negative values)
 */
PIXMAN_EXPORT void
pixman_edge_step (pixman_edge_t *e, int n)
{
    pixman_fixed_48_16_t	ne;

    e->x += n * e->stepx;

    ne = e->e + n * (pixman_fixed_48_16_t) e->dx;

    if (n >= 0)
    {
	if (ne > 0)
	{
	    int nx = (ne + e->dy - 1) / e->dy;
	    e->e = ne - nx * (pixman_fixed_48_16_t) e->dy;
	    e->x += nx * e->signdx;
	}
    }
    else
    {
	if (ne <= -e->dy)
	{
	    int nx = (-ne) / e->dy;
	    e->e = ne + nx * (pixman_fixed_48_16_t) e->dy;
	    e->x -= nx * e->signdx;
	}
    }
}

/*
 * A private routine to initialize the multi-step
 * elements of an edge structure
 */
static void
_pixman_edge_multi_init (pixman_edge_t *e, int n, pixman_fixed_t *stepx_p, pixman_fixed_t *dx_p)
{
    pixman_fixed_t	stepx;
    pixman_fixed_48_16_t	ne;

    ne = n * (pixman_fixed_48_16_t) e->dx;
    stepx = n * e->stepx;
    if (ne > 0)
    {
	int nx = ne / e->dy;
	ne -= nx * e->dy;
	stepx += nx * e->signdx;
    }
    *dx_p = ne;
    *stepx_p = stepx;
}

/*
 * Initialize one edge structure given the line endpoints and a
 * starting y value
 */
PIXMAN_EXPORT void
pixman_edge_init (pixman_edge_t	*e,
		  int		n,
		  pixman_fixed_t		y_start,
		  pixman_fixed_t		x_top,
		  pixman_fixed_t		y_top,
		  pixman_fixed_t		x_bot,
		  pixman_fixed_t		y_bot)
{
    pixman_fixed_t	dx, dy;

    e->x = x_top;
    e->e = 0;
    dx = x_bot - x_top;
    dy = y_bot - y_top;
    e->dy = dy;
    e->dx = 0;
    if (dy)
    {
	if (dx >= 0)
	{
	    e->signdx = 1;
	    e->stepx = dx / dy;
	    e->dx = dx % dy;
	    e->e = -dy;
	}
	else
	{
	    e->signdx = -1;
	    e->stepx = -(-dx / dy);
	    e->dx = -dx % dy;
	    e->e = 0;
	}

	_pixman_edge_multi_init (e, STEP_Y_SMALL(n), &e->stepx_small, &e->dx_small);
	_pixman_edge_multi_init (e, STEP_Y_BIG(n), &e->stepx_big, &e->dx_big);
    }
    pixman_edge_step (e, y_start - y_top);
}

/*
 * Initialize one edge structure given a line, starting y value
 * and a pixel offset for the line
 */
PIXMAN_EXPORT void
pixman_line_fixed_edge_init (pixman_edge_t *e,
			     int	    n,
			     pixman_fixed_t	    y,
			     const pixman_line_fixed_t *line,
			     int	    x_off,
			     int	    y_off)
{
    pixman_fixed_t	x_off_fixed = pixman_int_to_fixed(x_off);
    pixman_fixed_t	y_off_fixed = pixman_int_to_fixed(y_off);
    const pixman_point_fixed_t *top, *bot;

    if (line->p1.y <= line->p2.y)
    {
	top = &line->p1;
	bot = &line->p2;
    }
    else
    {
	top = &line->p2;
	bot = &line->p1;
    }
    pixman_edge_init (e, n, y,
		    top->x + x_off_fixed,
		    top->y + y_off_fixed,
		    bot->x + x_off_fixed,
		    bot->y + y_off_fixed);
}

pixman_bool_t
pixman_multiply_overflows_int (unsigned int a,
		               unsigned int b)
{
    return a >= INT32_MAX / b;
}

pixman_bool_t
pixman_addition_overflows_int (unsigned int a,
		               unsigned int b)
{
    return a > INT32_MAX - b;
}

void *
pixman_malloc_ab(unsigned int a,
		 unsigned int b)
{
    if (a >= INT32_MAX / b)
	return NULL;

    return malloc (a * b);
}

void *
pixman_malloc_abc (unsigned int a,
		   unsigned int b,
		   unsigned int c)
{
    if (a >= INT32_MAX / b)
	return NULL;
    else if (a * b >= INT32_MAX / c)
	return NULL;
    else
	return malloc (a * b * c);
}


/**
 * pixman_version:
 *
 * Returns the version of the pixman library encoded in a single
 * integer as per %PIXMAN_VERSION_ENCODE. The encoding ensures that
 * later versions compare greater than earlier versions.
 *
 * A run-time comparison to check that pixman's version is greater than
 * or equal to version X.Y.Z could be performed as follows:
 *
 * <informalexample><programlisting>
 * if (pixman_version() >= PIXMAN_VERSION_ENCODE(X,Y,Z)) {...}
 * </programlisting></informalexample>
 *
 * See also pixman_version_string() as well as the compile-time
 * equivalents %PIXMAN_VERSION and %PIXMAN_VERSION_STRING.
 *
 * Return value: the encoded version.
 **/
PIXMAN_EXPORT int
pixman_version (void)
{
    return PIXMAN_VERSION;
}

/**
 * pixman_version_string:
 *
 * Returns the version of the pixman library as a human-readable string
 * of the form "X.Y.Z".
 *
 * See also pixman_version() as well as the compile-time equivalents
 * %PIXMAN_VERSION_STRING and %PIXMAN_VERSION.
 *
 * Return value: a string containing the version.
 **/
PIXMAN_EXPORT const char*
pixman_version_string (void)
{
    return PIXMAN_VERSION_STRING;
}

/**
 * pixman_format_supported_destination:
 * @format: A pixman_format_code_t format
 * 
 * Return value: whether the provided format code is a supported
 * format for a pixman surface used as a destination in
 * rendering.
 *
 * Currently, all pixman_format_code_t values are supported
 * except for the YUV formats.
 **/
PIXMAN_EXPORT pixman_bool_t
pixman_format_supported_destination (pixman_format_code_t format)
{
    switch (format) {
    /* 32 bpp formats */
    case PIXMAN_a2b10g10r10:
    case PIXMAN_x2b10g10r10:
    case PIXMAN_a8r8g8b8:
    case PIXMAN_x8r8g8b8:
    case PIXMAN_a8b8g8r8:
    case PIXMAN_x8b8g8r8:
    case PIXMAN_b8g8r8a8:
    case PIXMAN_b8g8r8x8:
    case PIXMAN_r8g8b8:
    case PIXMAN_b8g8r8:
    case PIXMAN_r5g6b5:
    case PIXMAN_b5g6r5:
    /* 16 bpp formats */
    case PIXMAN_a1r5g5b5:
    case PIXMAN_x1r5g5b5:
    case PIXMAN_a1b5g5r5:
    case PIXMAN_x1b5g5r5:
    case PIXMAN_a4r4g4b4:
    case PIXMAN_x4r4g4b4:
    case PIXMAN_a4b4g4r4:
    case PIXMAN_x4b4g4r4:
    /* 8bpp formats */
    case PIXMAN_a8:
    case PIXMAN_r3g3b2:
    case PIXMAN_b2g3r3:
    case PIXMAN_a2r2g2b2:
    case PIXMAN_a2b2g2r2:
    case PIXMAN_c8:
    case PIXMAN_g8:
    case PIXMAN_x4a4:
    /* Collides with PIXMAN_c8
    case PIXMAN_x4c4:
    */
    /* Collides with PIXMAN_g8
    case PIXMAN_x4g4:
    */
    /* 4bpp formats */
    case PIXMAN_a4:
    case PIXMAN_r1g2b1:
    case PIXMAN_b1g2r1:
    case PIXMAN_a1r1g1b1:
    case PIXMAN_a1b1g1r1:
    case PIXMAN_c4:
    case PIXMAN_g4:
    /* 1bpp formats */
    case PIXMAN_a1:
    case PIXMAN_g1:
	return TRUE;
	
    /* YUV formats */
    case PIXMAN_yuy2:
    case PIXMAN_yv12:
    default:
	return FALSE;
    }
}

/**
 * pixman_format_supported_source:
 * @format: A pixman_format_code_t format
 * 
 * Return value: whether the provided format code is a supported
 * format for a pixman surface used as a source in
 * rendering.
 *
 * Currently, all pixman_format_code_t values are supported.
 **/
PIXMAN_EXPORT pixman_bool_t
pixman_format_supported_source (pixman_format_code_t format)
{
    switch (format) {
    /* 32 bpp formats */
    case PIXMAN_a2b10g10r10:
    case PIXMAN_x2b10g10r10:
    case PIXMAN_a8r8g8b8:
    case PIXMAN_x8r8g8b8:
    case PIXMAN_a8b8g8r8:
    case PIXMAN_x8b8g8r8:
    case PIXMAN_b8g8r8a8:
    case PIXMAN_b8g8r8x8:
    case PIXMAN_r8g8b8:
    case PIXMAN_b8g8r8:
    case PIXMAN_r5g6b5:
    case PIXMAN_b5g6r5:
    /* 16 bpp formats */
    case PIXMAN_a1r5g5b5:
    case PIXMAN_x1r5g5b5:
    case PIXMAN_a1b5g5r5:
    case PIXMAN_x1b5g5r5:
    case PIXMAN_a4r4g4b4:
    case PIXMAN_x4r4g4b4:
    case PIXMAN_a4b4g4r4:
    case PIXMAN_x4b4g4r4:
    /* 8bpp formats */
    case PIXMAN_a8:
    case PIXMAN_r3g3b2:
    case PIXMAN_b2g3r3:
    case PIXMAN_a2r2g2b2:
    case PIXMAN_a2b2g2r2:
    case PIXMAN_c8:
    case PIXMAN_g8:
    case PIXMAN_x4a4:
    /* Collides with PIXMAN_c8
    case PIXMAN_x4c4:
    */
    /* Collides with PIXMAN_g8
    case PIXMAN_x4g4:
    */
    /* 4bpp formats */
    case PIXMAN_a4:
    case PIXMAN_r1g2b1:
    case PIXMAN_b1g2r1:
    case PIXMAN_a1r1g1b1:
    case PIXMAN_a1b1g1r1:
    case PIXMAN_c4:
    case PIXMAN_g4:
    /* 1bpp formats */
    case PIXMAN_a1:
    case PIXMAN_g1:
    /* YUV formats */
    case PIXMAN_yuy2:
    case PIXMAN_yv12:
	return TRUE;

    default:
	return FALSE;
    }
}

void
_pixman_walk_composite_region (pixman_implementation_t *imp,
			      pixman_op_t op,
			      pixman_image_t * pSrc,
			      pixman_image_t * pMask,
			      pixman_image_t * pDst,
			      int16_t xSrc,
			      int16_t ySrc,
			      int16_t xMask,
			      int16_t yMask,
			      int16_t xDst,
			      int16_t yDst,
			      uint16_t width,
			      uint16_t height,
			      pixman_bool_t srcRepeat,
			      pixman_bool_t maskRepeat,
			      pixman_composite_func_t compositeRect)
{
    int		    n;
    const pixman_box32_t *pbox;
    int		    w, h, w_this, h_this;
    int		    x_msk, y_msk, x_src, y_src, x_dst, y_dst;
    pixman_region32_t reg;
    pixman_region32_t *region;

    pixman_region32_init (&reg);
    if (!pixman_compute_composite_region32 (&reg, pSrc, pMask, pDst,
					    xSrc, ySrc, xMask, yMask, xDst, yDst, width, height))
    {
	return;
    }

    region = &reg;

    pbox = pixman_region32_rectangles (region, &n);
    while (n--)
    {
	h = pbox->y2 - pbox->y1;
	y_src = pbox->y1 - yDst + ySrc;
	y_msk = pbox->y1 - yDst + yMask;
	y_dst = pbox->y1;
	while (h)
	{
	    h_this = h;
	    w = pbox->x2 - pbox->x1;
	    x_src = pbox->x1 - xDst + xSrc;
	    x_msk = pbox->x1 - xDst + xMask;
	    x_dst = pbox->x1;
	    if (maskRepeat)
	    {
		y_msk = MOD (y_msk, pMask->bits.height);
		if (h_this > pMask->bits.height - y_msk)
		    h_this = pMask->bits.height - y_msk;
	    }
	    if (srcRepeat)
	    {
		y_src = MOD (y_src, pSrc->bits.height);
		if (h_this > pSrc->bits.height - y_src)
		    h_this = pSrc->bits.height - y_src;
	    }
	    while (w)
	    {
		w_this = w;
		if (maskRepeat)
		{
		    x_msk = MOD (x_msk, pMask->bits.width);
		    if (w_this > pMask->bits.width - x_msk)
			w_this = pMask->bits.width - x_msk;
		}
		if (srcRepeat)
		{
		    x_src = MOD (x_src, pSrc->bits.width);
		    if (w_this > pSrc->bits.width - x_src)
			w_this = pSrc->bits.width - x_src;
		}
		(*compositeRect) (imp,
				  op, pSrc, pMask, pDst,
				  x_src, y_src, x_msk, y_msk, x_dst, y_dst,
				  w_this, h_this);
		w -= w_this;
		x_src += w_this;
		x_msk += w_this;
		x_dst += w_this;
	    }
	    h -= h_this;
	    y_src += h_this;
	    y_msk += h_this;
	    y_dst += h_this;
	}
	pbox++;
    }
    pixman_region32_fini (&reg);
}

static pixman_bool_t
mask_is_solid (pixman_image_t *mask)
{
    if (mask->type == SOLID)
	return TRUE;

    if (mask->type == BITS &&
	mask->common.repeat == PIXMAN_REPEAT_NORMAL &&
	mask->bits.width == 1 &&
	mask->bits.height == 1)
    {
	return TRUE;
    }

    return FALSE;
}

static const FastPathInfo *
get_fast_path (const FastPathInfo *fast_paths,
	       pixman_op_t         op,
	       pixman_image_t     *pSrc,
	       pixman_image_t     *pMask,
	       pixman_image_t     *pDst,
	       pixman_bool_t       is_pixbuf)
{
    const FastPathInfo *info;

    for (info = fast_paths; info->op != PIXMAN_OP_NONE; info++)
    {
	pixman_bool_t valid_src		= FALSE;
	pixman_bool_t valid_mask	= FALSE;

	if (info->op != op)
	    continue;

	if ((info->src_format == PIXMAN_solid && pixman_image_can_get_solid (pSrc))		||
	    (pSrc->type == BITS && info->src_format == pSrc->bits.format))
	{
	    valid_src = TRUE;
	}

	if (!valid_src)
	    continue;

	if ((info->mask_format == PIXMAN_null && !pMask)			||
	    (pMask && pMask->type == BITS && info->mask_format == pMask->bits.format))
	{
	    valid_mask = TRUE;

	    if (info->flags & NEED_SOLID_MASK)
	    {
		if (!pMask || !mask_is_solid (pMask))
		    valid_mask = FALSE;
	    }

	    if (info->flags & NEED_COMPONENT_ALPHA)
	    {
		if (!pMask || !pMask->common.component_alpha)
		    valid_mask = FALSE;
	    }
	}

	if (!valid_mask)
	    continue;
	
	if (info->dest_format != pDst->bits.format)
	    continue;

	if ((info->flags & NEED_PIXBUF) && !is_pixbuf)
	    continue;

	return info;
    }

    return NULL;
}

pixman_bool_t
_pixman_run_fast_path (const FastPathInfo *paths,
		       pixman_implementation_t *imp,
		       pixman_op_t op,
		       pixman_image_t *src,
		       pixman_image_t *mask,
		       pixman_image_t *dest,
		       int32_t src_x,
		       int32_t src_y,
		       int32_t mask_x,
		       int32_t mask_y,
		       int32_t dest_x,
		       int32_t dest_y,
		       int32_t width,
		       int32_t height)
{
    pixman_composite_func_t func = NULL;
    pixman_bool_t src_repeat = src->common.repeat == PIXMAN_REPEAT_NORMAL;
    pixman_bool_t mask_repeat = mask && mask->common.repeat == PIXMAN_REPEAT_NORMAL;
    
    if ((src->type == BITS || pixman_image_can_get_solid (src)) &&
	(!mask || mask->type == BITS)
        && !src->common.transform && !(mask && mask->common.transform)
        && !(mask && mask->common.alpha_map) && !src->common.alpha_map && !dest->common.alpha_map
        && (src->common.filter != PIXMAN_FILTER_CONVOLUTION)
        && (src->common.repeat != PIXMAN_REPEAT_PAD)
        && (src->common.repeat != PIXMAN_REPEAT_REFLECT)
        && (!mask || (mask->common.filter != PIXMAN_FILTER_CONVOLUTION &&
		      mask->common.repeat != PIXMAN_REPEAT_PAD &&
		      mask->common.repeat != PIXMAN_REPEAT_REFLECT))
	&& !src->common.read_func && !src->common.write_func
	&& !(mask && mask->common.read_func)
	&& !(mask && mask->common.write_func)
	&& !dest->common.read_func
	&& !dest->common.write_func)
    {
	const FastPathInfo *info;	
	pixman_bool_t pixbuf;

	pixbuf =
	    src && src->type == BITS		&&
	    mask && mask->type == BITS		&&
	    src->bits.bits == mask->bits.bits	&&
	    src_x == mask_x			&&
	    src_y == mask_y			&&
	    !mask->common.component_alpha	&&
	    !mask_repeat;
	
	info = get_fast_path (paths, op, src, mask, dest, pixbuf);

	if (info)
	{
	    func = info->func;
		
	    if (info->src_format == PIXMAN_solid)
		src_repeat = FALSE;

	    if (info->mask_format == PIXMAN_solid || info->flags & NEED_SOLID_MASK)
		mask_repeat = FALSE;

	    if ((src_repeat			&&
		 src->bits.width == 1		&&
		 src->bits.height == 1)	||
		(mask_repeat			&&
		 mask->bits.width == 1		&&
		 mask->bits.height == 1))
	    {
		/* If src or mask are repeating 1x1 images and src_repeat or
		 * mask_repeat are still TRUE, it means the fast path we
		 * selected does not actually handle repeating images.
		 *
		 * So rather than call the "fast path" with a zillion
		 * 1x1 requests, we just fall back to the general code (which
		 * does do something sensible with 1x1 repeating images).
		 */
		func = NULL;
	    }
	}
    }

    if (func)
    {
	_pixman_walk_composite_region (imp, op,
				       src, mask, dest,
				       src_x, src_y, mask_x, mask_y,
				       dest_x, dest_y,
				       width, height,
				       src_repeat, mask_repeat,
				       func);
	return TRUE;
    }
    
    return FALSE;
}
