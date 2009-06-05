/* -*- Mode: c; c-basic-offset: 4; tab-width: 8; indent-tabs-mode: t; -*- */
/*
 * Copyright © 2000 SuSE, Inc.
 * Copyright © 2007 Red Hat, Inc.
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
#include "pixman-private.h"

/*
 * Operator optimizations based on source or destination opacity
 */
typedef struct
{
    pixman_op_t			op;
    pixman_op_t			opSrcDstOpaque;
    pixman_op_t			opSrcOpaque;
    pixman_op_t			opDstOpaque;
} OptimizedOperatorInfo;

static const OptimizedOperatorInfo optimized_operators[] =
{
    /* Input Operator           SRC&DST Opaque          SRC Opaque              DST Opaque      */
    { PIXMAN_OP_OVER,           PIXMAN_OP_SRC,          PIXMAN_OP_SRC,          PIXMAN_OP_OVER },
    { PIXMAN_OP_OVER_REVERSE,   PIXMAN_OP_DST,          PIXMAN_OP_OVER_REVERSE, PIXMAN_OP_DST },
    { PIXMAN_OP_IN,             PIXMAN_OP_SRC,          PIXMAN_OP_IN,           PIXMAN_OP_SRC },
    { PIXMAN_OP_IN_REVERSE,     PIXMAN_OP_DST,          PIXMAN_OP_DST,          PIXMAN_OP_IN_REVERSE },
    { PIXMAN_OP_OUT,            PIXMAN_OP_CLEAR,        PIXMAN_OP_OUT,          PIXMAN_OP_CLEAR },
    { PIXMAN_OP_OUT_REVERSE,    PIXMAN_OP_CLEAR,        PIXMAN_OP_CLEAR,        PIXMAN_OP_OUT_REVERSE },
    { PIXMAN_OP_ATOP,           PIXMAN_OP_SRC,          PIXMAN_OP_IN,           PIXMAN_OP_OVER },
    { PIXMAN_OP_ATOP_REVERSE,   PIXMAN_OP_DST,          PIXMAN_OP_OVER_REVERSE, PIXMAN_OP_IN_REVERSE },
    { PIXMAN_OP_XOR,            PIXMAN_OP_CLEAR,        PIXMAN_OP_OUT,          PIXMAN_OP_OUT_REVERSE },
    { PIXMAN_OP_SATURATE,       PIXMAN_OP_DST,          PIXMAN_OP_OVER_REVERSE, PIXMAN_OP_DST },
    { PIXMAN_OP_NONE }
};

/*
 * Check if the current operator could be optimized
 */
static const OptimizedOperatorInfo*
pixman_operator_can_be_optimized(pixman_op_t op)
{
    const OptimizedOperatorInfo *info;

    for (info = optimized_operators; info->op != PIXMAN_OP_NONE; info++)
    {
        if(info->op == op)
            return info;
    }
    return NULL;
}

/*
 * Optimize the current operator based on opacity of source or destination
 * The output operator should be mathematically equivalent to the source.
 */
static pixman_op_t
pixman_optimize_operator(pixman_op_t op, pixman_image_t *pSrc, pixman_image_t *pMask, pixman_image_t *pDst )
{
    pixman_bool_t is_source_opaque;
    pixman_bool_t is_dest_opaque;
    const OptimizedOperatorInfo *info = pixman_operator_can_be_optimized(op);

    if(!info || pMask)
        return op;

    is_source_opaque = pixman_image_is_opaque(pSrc);
    is_dest_opaque = pixman_image_is_opaque(pDst);

    if(is_source_opaque == FALSE && is_dest_opaque == FALSE)
        return op;

    if(is_source_opaque && is_dest_opaque)
        return info->opSrcDstOpaque;
    else if(is_source_opaque)
        return info->opSrcOpaque;
    else if(is_dest_opaque)
        return info->opDstOpaque;

    return op;

}

static pixman_implementation_t *imp;

PIXMAN_EXPORT void
pixman_image_composite (pixman_op_t      op,
			pixman_image_t * src,
			pixman_image_t * mask,
			pixman_image_t * dest,
			int16_t      src_x,
			int16_t      src_y,
			int16_t      mask_x,
			int16_t      mask_y,
			int16_t      dest_x,
			int16_t      dest_y,
			uint16_t     width,
			uint16_t     height)
{
    /*
     * Check if we can replace our operator by a simpler one if the src or dest are opaque
     * The output operator should be mathematically equivalent to the source.
     */
    op = pixman_optimize_operator(op, src, mask, dest);
    if(op == PIXMAN_OP_DST)
        return;

    if (!imp)
	imp = _pixman_choose_implementation();

    _pixman_implementation_composite (imp, op,
				      src, mask, dest,
				      src_x, src_y,
				      mask_x, mask_y,
				      dest_x, dest_y,
				      width, height);
}

PIXMAN_EXPORT pixman_bool_t
pixman_blt (uint32_t *src_bits,
	    uint32_t *dst_bits,
	    int src_stride,
	    int dst_stride,
	    int src_bpp,
	    int dst_bpp,
	    int src_x, int src_y,
	    int dst_x, int dst_y,
	    int width, int height)
{
    if (!imp)
	imp = _pixman_choose_implementation();
    
    return _pixman_implementation_blt (imp, src_bits, dst_bits, src_stride, dst_stride,
				       src_bpp, dst_bpp,
				       src_x, src_y,
				       dst_x, dst_y,
				       width, height);
}

PIXMAN_EXPORT pixman_bool_t
pixman_fill (uint32_t *bits,
	     int stride,
	     int bpp,
	     int x,
	     int y,
	     int width,
	     int height,
	     uint32_t xor)
{
    if (!imp)
	imp = _pixman_choose_implementation();

    return _pixman_implementation_fill (imp, bits, stride, bpp, x, y, width, height, xor);
}
