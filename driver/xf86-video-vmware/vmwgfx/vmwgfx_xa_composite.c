/*
 * Copyright 2009-2011 VMWare, Inc.
 * All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sub license, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial portions
 * of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.
 * IN NO EVENT SHALL TUNGSTEN GRAPHICS AND/OR ITS SUPPLIERS BE LIABLE FOR
 * ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * Author: Thomas Hellstrom <thellstrom@vmware.com>
 * Author: Zack Ruzin <zackr@vmware.com>
 *
 * The code in this file translates XRender PICT composite stuff
 * to fit the libxatracker API.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <pixman.h>
#include <picturestr.h>
#include "xa_composite.h"
#include "vmwgfx_saa.h"
#include "vmwgfx_saa_priv.h"


struct vmwgfx_composite {
    union xa_source_pict *src_spict;
    union xa_source_pict *mask_spict;
    union xa_source_pict *dst_spict;
    struct xa_picture *src_pict;
    struct xa_picture *mask_pict;
    struct xa_picture *dst_pict;
    struct xa_composite *comp;
};

static const enum xa_composite_op vmwgfx_op_map[] = {
    [PictOpClear] = xa_op_clear,
    [PictOpSrc] = xa_op_src,
    [PictOpDst] = xa_op_dst,
    [PictOpOver] = xa_op_over,
    [PictOpOverReverse] = xa_op_over_reverse,
    [PictOpIn] = xa_op_in,
    [PictOpInReverse] = xa_op_in_reverse,
    [PictOpOut] = xa_op_out,
    [PictOpOutReverse] = xa_op_out_reverse,
    [PictOpAtop] = xa_op_atop,
    [PictOpAtopReverse] = xa_op_atop_reverse,
    [PictOpXor] = xa_op_xor,
    [PictOpAdd] = xa_op_add
};

static const unsigned int vmwgfx_op_map_size =
    sizeof(vmwgfx_op_map) / sizeof(enum xa_composite_op);

static Bool
vmwgfx_matrix_from_pict_transform(PictTransform *trans, float *matrix)
{
   if (!trans)
      return FALSE;

   matrix[0] = pixman_fixed_to_double(trans->matrix[0][0]);
   matrix[3] = pixman_fixed_to_double(trans->matrix[0][1]);
   matrix[6] = pixman_fixed_to_double(trans->matrix[0][2]);

   matrix[1] = pixman_fixed_to_double(trans->matrix[1][0]);
   matrix[4] = pixman_fixed_to_double(trans->matrix[1][1]);
   matrix[7] = pixman_fixed_to_double(trans->matrix[1][2]);

   matrix[2] = pixman_fixed_to_double(trans->matrix[2][0]);
   matrix[5] = pixman_fixed_to_double(trans->matrix[2][1]);
   matrix[8] = pixman_fixed_to_double(trans->matrix[2][2]);

   return TRUE;
}

static enum xa_composite_wrap
vmwgfx_xa_setup_wrap(Bool pict_has_repeat, int pict_repeat)
{
    enum xa_composite_wrap wrap = xa_wrap_clamp_to_border;

    if (!pict_has_repeat)
	return wrap;

    switch(pict_repeat) {
    case RepeatNormal:
	wrap = xa_wrap_repeat;
	break;
    case RepeatReflect:
	wrap = xa_wrap_mirror_repeat;
	break;
    case RepeatPad:
	wrap = xa_wrap_clamp_to_edge;
	break;
    default:
	break;
    }
    return wrap;
}

static Bool
vmwgfx_render_filter_to_xa(int xrender_filter,
			   enum xa_composite_filter *out_filter)
{
   switch (xrender_filter) {
   case PictFilterConvolution:
   case PictFilterNearest:
   case PictFilterFast:
       *out_filter = xa_filter_nearest;
      break;
   case PictFilterBest:
   case PictFilterGood:
   case PictFilterBilinear:
       *out_filter = xa_filter_linear;
      break;
   default:
       *out_filter = xa_filter_nearest;
       return FALSE;
   }
   return TRUE;
}

static Bool
vmwgfx_xa_setup_pict(PicturePtr pict,
		     struct xa_picture *xa_pict,
		     union xa_source_pict *src_pict)
{
    if (!pict)
	return FALSE;

    memset(xa_pict, 0, sizeof(*xa_pict));

    xa_pict->pict_format = vmwgfx_xa_format(pict->format);
    if (xa_pict->pict_format == xa_format_unknown)
	return FALSE;

    /*
     * Saa doesn't let drivers accelerate alpha maps.
     */
    xa_pict->alpha_map = NULL;
    xa_pict->component_alpha = pict->componentAlpha;

    xa_pict->has_transform =
	vmwgfx_matrix_from_pict_transform(pict->transform,
					  xa_pict->transform);

    xa_pict->wrap = vmwgfx_xa_setup_wrap(pict->repeat,
					 pict->repeatType);

    (void) vmwgfx_render_filter_to_xa(pict->filter, &xa_pict->filter);

    if (pict->pSourcePict) {
	if (pict->pSourcePict->type != SourcePictTypeSolidFill)
	    return FALSE;

	src_pict->type = xa_src_pict_solid_fill;
	src_pict->solid_fill.color = pict->pSourcePict->solidFill.color;
	xa_pict->src_pict = src_pict;
    }

    return TRUE;
}

struct xa_composite *
vmwgfx_xa_setup_comp(struct vmwgfx_composite *vcomp,
		     int op,
		     PicturePtr src_pict,
		     PicturePtr mask_pict,
		     PicturePtr dst_pict)
{
    struct xa_composite *comp = vcomp->comp;

    if (op >= vmwgfx_op_map_size)
	return NULL;

    comp->op = vmwgfx_op_map[op];
    if (comp->op == xa_op_clear && op != PictOpClear)
	return NULL;

    if (!vmwgfx_xa_setup_pict(dst_pict, vcomp->dst_pict,
			      vcomp->dst_spict))
	return NULL;
    if (!vmwgfx_xa_setup_pict(src_pict, vcomp->src_pict,
			      vcomp->src_spict))
	return NULL;
    if (mask_pict && !vmwgfx_xa_setup_pict(mask_pict,
					   vcomp->mask_pict,
					   vcomp->mask_spict))
	return NULL;

    comp->dst = vcomp->dst_pict;
    comp->src = vcomp->src_pict;
    comp->mask = (mask_pict) ? vcomp->mask_pict : NULL;

    return comp;
}

Bool
vmwgfx_xa_update_comp(struct xa_composite *comp,
		      PixmapPtr src_pix,
		      PixmapPtr mask_pix,
		      PixmapPtr dst_pix)
{
    comp->dst->srf = vmwgfx_saa_pixmap(dst_pix)->hw;
    if (src_pix)
	comp->src->srf = vmwgfx_saa_pixmap(src_pix)->hw;
    if (mask_pix && comp->mask)
	comp->mask->srf = vmwgfx_saa_pixmap(mask_pix)->hw;
    return TRUE;
}


void
vmwgfx_free_composite(struct vmwgfx_composite *vcomp)
{
    if (!vcomp)
	return;

    if (vcomp->src_spict)
	free(vcomp->src_spict);
    if (vcomp->mask_spict)
	free(vcomp->mask_spict);
    if (vcomp->dst_spict)
	free(vcomp->dst_spict);
    if (vcomp->src_pict)
	free(vcomp->src_pict);
    if (vcomp->mask_pict)
	free(vcomp->mask_pict);
    if (vcomp->dst_pict)
	free(vcomp->dst_pict);
    if (vcomp->comp)
	free(vcomp->comp);
    free(vcomp);
}

struct vmwgfx_composite *
vmwgfx_alloc_composite(void)
{
    const struct xa_composite_allocation *a = xa_composite_allocation();
    struct vmwgfx_composite *vcomp = calloc(1, sizeof(*vcomp));

    if (!vcomp)
	return NULL;

    vcomp->src_spict = calloc(1, a->xa_source_pict_size);
    vcomp->mask_spict = calloc(1, a->xa_source_pict_size);
    vcomp->dst_spict = calloc(1, a->xa_source_pict_size);
    vcomp->src_pict = calloc(1, a->xa_picture_size);
    vcomp->mask_pict = calloc(1, a->xa_picture_size);
    vcomp->dst_pict = calloc(1, a->xa_picture_size);
    vcomp->comp = calloc(1, a->xa_composite_size);

    if (!vcomp->src_spict || !vcomp->mask_spict || !vcomp->dst_spict ||
	!vcomp->src_pict || !vcomp->mask_pict || !vcomp->dst_pict ||
	!vcomp->comp) {
	vmwgfx_free_composite(vcomp);
	return NULL;
    }

    return vcomp;
}
