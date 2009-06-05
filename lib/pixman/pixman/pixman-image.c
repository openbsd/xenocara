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
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "pixman-private.h"

#define Alpha(x) ((x) >> 24)

pixman_bool_t
_pixman_init_gradient (gradient_t     *gradient,
		       const pixman_gradient_stop_t *stops,
		       int	       n_stops)
{
    return_val_if_fail (n_stops > 0, FALSE);

    gradient->stops = pixman_malloc_ab (n_stops, sizeof (pixman_gradient_stop_t));
    if (!gradient->stops)
	return FALSE;

    memcpy (gradient->stops, stops, n_stops * sizeof (pixman_gradient_stop_t));

    gradient->n_stops = n_stops;

    gradient->stop_range = 0xffff;
    gradient->color_table = NULL;
    gradient->color_table_size = 0;
    gradient->common.class = SOURCE_IMAGE_CLASS_UNKNOWN;

    return TRUE;
}

/*
 * By default, just evaluate the image at 32bpp and expand.  Individual image
 * types can plug in a better scanline getter if they want to. For example
 * we  could produce smoother gradients by evaluating them at higher color depth, but
 * that's a project for the future.
 */
void
_pixman_image_get_scanline_64_generic (pixman_image_t * pict, int x, int y, int width,
				       uint64_t *buffer, uint64_t *mask, uint32_t maskBits)
{
    uint32_t *mask8 = NULL;

    // Contract the mask image, if one exists, so that the 32-bit fetch function
    // can use it.
    if (mask) {
        mask8 = pixman_malloc_ab(width, sizeof(uint32_t));
	if (!mask8)
	    return;
	
        pixman_contract(mask8, mask, width);
    }

    // Fetch the source image into the first half of buffer.
    _pixman_image_get_scanline_32 (pict, x, y, width, (uint32_t*)buffer, mask8,
				   maskBits);

    // Expand from 32bpp to 64bpp in place.
    pixman_expand(buffer, (uint32_t*)buffer, PIXMAN_a8r8g8b8, width);

    free(mask8);
}

pixman_image_t *
_pixman_image_allocate (void)
{
    pixman_image_t *image = malloc (sizeof (pixman_image_t));

    if (image)
    {
	image_common_t *common = &image->common;

	pixman_region32_init (&common->full_region);
	pixman_region32_init (&common->clip_region);
	common->src_clip = &common->full_region;
	common->has_client_clip = FALSE;
	common->transform = NULL;
	common->repeat = PIXMAN_REPEAT_NONE;
	common->filter = PIXMAN_FILTER_NEAREST;
	common->filter_params = NULL;
	common->n_filter_params = 0;
	common->alpha_map = NULL;
	common->component_alpha = FALSE;
	common->ref_count = 1;
	common->read_func = NULL;
	common->write_func = NULL;
	common->classify = NULL;
    }

    return image;
}

source_pict_class_t
_pixman_image_classify (pixman_image_t *image,
			int             x,
			int             y,
			int             width,
			int             height)
{
    if (image->common.classify)
	return image->common.classify (image, x, y, width, height);
    else
	return SOURCE_IMAGE_CLASS_UNKNOWN;
}

void
_pixman_image_get_scanline_32 (pixman_image_t *image, int x, int y, int width,
			       uint32_t *buffer, uint32_t *mask, uint32_t mask_bits)
{
    image->common.get_scanline_32 (image, x, y, width, buffer, mask, mask_bits);
}

void
_pixman_image_get_scanline_64 (pixman_image_t *image, int x, int y, int width,
			       uint32_t *buffer, uint32_t *unused, uint32_t unused2)
{
    image->common.get_scanline_64 (image, x, y, width, buffer, unused, unused2);
}

/* Even thought the type of buffer is uint32_t *, the function actually expects
 * a uint64_t *buffer.
 */

scanFetchProc
_pixman_image_get_fetcher (pixman_image_t *image,
			   int             wide)
{
    assert (image->common.get_scanline_64);
    assert (image->common.get_scanline_32);
    
    if (wide)
	return image->common.get_scanline_64;
    else
	return image->common.get_scanline_32;
}

#define WRITE_ACCESS(f) ((image->common.write_func)? f##_accessors : f)

static void
image_property_changed (pixman_image_t *image)
{
    
    
    image->common.property_changed (image);
}

/* Ref Counting */
PIXMAN_EXPORT pixman_image_t *
pixman_image_ref (pixman_image_t *image)
{
    image->common.ref_count++;

    return image;
}

/* returns TRUE when the image is freed */
PIXMAN_EXPORT pixman_bool_t
pixman_image_unref (pixman_image_t *image)
{
    image_common_t *common = (image_common_t *)image;

    common->ref_count--;

    if (common->ref_count == 0)
    {
	pixman_region32_fini (&common->clip_region);
	pixman_region32_fini (&common->full_region);

	if (common->transform)
	    free (common->transform);

	if (common->filter_params)
	    free (common->filter_params);

	if (common->alpha_map)
	    pixman_image_unref ((pixman_image_t *)common->alpha_map);

#if 0
	if (image->type == BITS && image->bits.indexed)
	    free (image->bits.indexed);
#endif

#if 0
	memset (image, 0xaa, sizeof (pixman_image_t));
#endif
	if (image->type == LINEAR || image->type == RADIAL || image->type == CONICAL)
	{
	    if (image->gradient.stops)
		free (image->gradient.stops);
	}


	if (image->type == BITS && image->bits.free_me)
	    free (image->bits.free_me);

	free (image);

	return TRUE;
    }

    return FALSE;
}

/* Constructors */

void
_pixman_image_reset_clip_region (pixman_image_t *image)
{
    pixman_region32_fini (&image->common.clip_region);

    if (image->type == BITS)
    {
	pixman_region32_init_rect (&image->common.clip_region, 0, 0,
				   image->bits.width, image->bits.height);
    }
    else
    {
	pixman_region32_init (&image->common.clip_region);
    }
}

PIXMAN_EXPORT pixman_bool_t
pixman_image_set_clip_region32 (pixman_image_t *image,
				pixman_region32_t *region)
{
    image_common_t *common = (image_common_t *)image;
    pixman_bool_t result;

    if (region)
    {
	result = pixman_region32_copy (&common->clip_region, region);
    }
    else
    {
	_pixman_image_reset_clip_region (image);

	result = TRUE;
    }

    image_property_changed (image);

    return result;
}


PIXMAN_EXPORT pixman_bool_t
pixman_image_set_clip_region (pixman_image_t    *image,
			      pixman_region16_t *region)
{
    image_common_t *common = (image_common_t *)image;
    pixman_bool_t result;

    if (region)
    {
	result = pixman_region32_copy_from_region16 (&common->clip_region, region);
    }
    else
    {
	_pixman_image_reset_clip_region (image);

	result = TRUE;
    }

    image_property_changed (image);

    return result;
}

/* Sets whether the clip region includes a clip region set by the client
 */
PIXMAN_EXPORT void
pixman_image_set_has_client_clip (pixman_image_t *image,
				  pixman_bool_t	  client_clip)
{
    image->common.has_client_clip = client_clip;

    image_property_changed (image);
}

PIXMAN_EXPORT pixman_bool_t
pixman_image_set_transform (pixman_image_t           *image,
			    const pixman_transform_t *transform)
{
    static const pixman_transform_t id =
    {
	{ { pixman_fixed_1, 0, 0 },
	  { 0, pixman_fixed_1, 0 },
	  { 0, 0, pixman_fixed_1 }
	}
    };

    image_common_t *common = (image_common_t *)image;
    pixman_bool_t result;

    if (common->transform == transform)
	return TRUE;

    if (memcmp (&id, transform, sizeof (pixman_transform_t)) == 0)
    {
	free(common->transform);
	common->transform = NULL;
	result = TRUE;
	goto out;
    }

    if (common->transform == NULL)
	common->transform = malloc (sizeof (pixman_transform_t));

    if (common->transform == NULL)
    {
	result = FALSE;
	goto out;
    }

    memcpy(common->transform, transform, sizeof(pixman_transform_t));

out:
    image_property_changed (image);
    
    return TRUE;
}

PIXMAN_EXPORT void
pixman_image_set_repeat (pixman_image_t  *image,
			 pixman_repeat_t  repeat)
{
    image->common.repeat = repeat;

    image_property_changed (image);
}

PIXMAN_EXPORT pixman_bool_t
pixman_image_set_filter (pixman_image_t       *image,
			 pixman_filter_t       filter,
			 const pixman_fixed_t *params,
			 int		       n_params)
{
    image_common_t *common = (image_common_t *)image;
    pixman_fixed_t *new_params;

    if (params == common->filter_params && filter == common->filter)
	return TRUE;

    new_params = NULL;
    if (params)
    {
	new_params = pixman_malloc_ab (n_params, sizeof (pixman_fixed_t));
	if (!new_params)
	    return FALSE;

	memcpy (new_params,
		params, n_params * sizeof (pixman_fixed_t));
    }

    common->filter = filter;

    if (common->filter_params)
	free (common->filter_params);

    common->filter_params = new_params;
    common->n_filter_params = n_params;

    image_property_changed (image);
    return TRUE;
}

PIXMAN_EXPORT void
pixman_image_set_source_clipping (pixman_image_t  *image,
				  pixman_bool_t    source_clipping)
{
    image_common_t *common = &image->common;

    if (source_clipping)
	common->src_clip = &common->clip_region;
    else
	common->src_clip = &common->full_region;

    image_property_changed (image);
}

/* Unlike all the other property setters, this function does not
 * copy the content of indexed. Doing this copying is simply
 * way, way too expensive.
 */
PIXMAN_EXPORT void
pixman_image_set_indexed (pixman_image_t	 *image,
			  const pixman_indexed_t *indexed)
{
    bits_image_t *bits = (bits_image_t *)image;

    bits->indexed = indexed;

    image_property_changed (image);
}

PIXMAN_EXPORT void
pixman_image_set_alpha_map (pixman_image_t *image,
			    pixman_image_t *alpha_map,
			    int16_t         x,
			    int16_t         y)
{
    image_common_t *common = (image_common_t *)image;

    return_if_fail (!alpha_map || alpha_map->type == BITS);

    if (common->alpha_map != (bits_image_t *)alpha_map)
    {
	if (common->alpha_map)
	    pixman_image_unref ((pixman_image_t *)common->alpha_map);

	if (alpha_map)
	    common->alpha_map = (bits_image_t *)pixman_image_ref (alpha_map);
	else
	    common->alpha_map = NULL;
    }

    common->alpha_origin.x = x;
    common->alpha_origin.y = y;

    image_property_changed (image);
}

PIXMAN_EXPORT void
pixman_image_set_component_alpha   (pixman_image_t       *image,
				    pixman_bool_t         component_alpha)
{
    image->common.component_alpha = component_alpha;

    image_property_changed (image);
}


PIXMAN_EXPORT void
pixman_image_set_accessors (pixman_image_t             *image,
			    pixman_read_memory_func_t	read_func,
			    pixman_write_memory_func_t	write_func)
{
    return_if_fail (image != NULL);

    image->common.read_func = read_func;
    image->common.write_func = write_func;

    image_property_changed (image);
}

PIXMAN_EXPORT uint32_t *
pixman_image_get_data (pixman_image_t *image)
{
    if (image->type == BITS)
	return image->bits.bits;

    return NULL;
}

PIXMAN_EXPORT int
pixman_image_get_width (pixman_image_t *image)
{
    if (image->type == BITS)
	return image->bits.width;

    return 0;
}

PIXMAN_EXPORT int
pixman_image_get_height (pixman_image_t *image)
{
    if (image->type == BITS)
	return image->bits.height;

    return 0;
}

PIXMAN_EXPORT int
pixman_image_get_stride (pixman_image_t *image)
{
    if (image->type == BITS)
	return image->bits.rowstride * (int) sizeof (uint32_t);

    return 0;
}

PIXMAN_EXPORT int
pixman_image_get_depth (pixman_image_t *image)
{
    if (image->type == BITS)
	return PIXMAN_FORMAT_DEPTH (image->bits.format);

    return 0;
}

static uint32_t
color_to_uint32 (const pixman_color_t *color)
{
    return
	(color->alpha >> 8 << 24) |
	(color->red >> 8 << 16) |
        (color->green & 0xff00) |
	(color->blue >> 8);
}

static pixman_bool_t
color_to_pixel (pixman_color_t *color,
		uint32_t       *pixel,
		pixman_format_code_t format)
{
    uint32_t c = color_to_uint32 (color);

    if (!(format == PIXMAN_a8r8g8b8	||
	  format == PIXMAN_x8r8g8b8	||
	  format == PIXMAN_a8b8g8r8	||
	  format == PIXMAN_x8b8g8r8	||
	  format == PIXMAN_b8g8r8a8	||
	  format == PIXMAN_b8g8r8x8	||
	  format == PIXMAN_r5g6b5	||
	  format == PIXMAN_b5g6r5	||
	  format == PIXMAN_a8))
    {
	return FALSE;
    }

    if (PIXMAN_FORMAT_TYPE (format) == PIXMAN_TYPE_ABGR)
    {
	c = ((c & 0xff000000) >>  0) |
	    ((c & 0x00ff0000) >> 16) |
	    ((c & 0x0000ff00) >>  0) |
	    ((c & 0x000000ff) << 16);
    }
    if (PIXMAN_FORMAT_TYPE (format) == PIXMAN_TYPE_BGRA)
    {
	c = ((c & 0xff000000) >> 24) |
	    ((c & 0x00ff0000) >>  8) |
	    ((c & 0x0000ff00) <<  8) |
	    ((c & 0x000000ff) << 24);
    }

    if (format == PIXMAN_a8)
	c = c >> 24;
    else if (format == PIXMAN_r5g6b5 ||
	     format == PIXMAN_b5g6r5)
	c = cvt8888to0565 (c);

#if 0
    printf ("color: %x %x %x %x\n", color->alpha, color->red, color->green, color->blue);
    printf ("pixel: %x\n", c);
#endif

    *pixel = c;
    return TRUE;
}

PIXMAN_EXPORT pixman_bool_t
pixman_image_fill_rectangles (pixman_op_t		    op,
			      pixman_image_t		   *dest,
			      pixman_color_t		   *color,
			      int			    n_rects,
			      const pixman_rectangle16_t   *rects)
{
    pixman_image_t *solid;
    pixman_color_t c;
    int i;

    if (color->alpha == 0xffff)
    {
	if (op == PIXMAN_OP_OVER)
	    op = PIXMAN_OP_SRC;
    }

    if (op == PIXMAN_OP_CLEAR)
    {
	c.red = 0;
	c.green = 0;
	c.blue = 0;
	c.alpha = 0;

	color = &c;

	op = PIXMAN_OP_SRC;
    }

    if (op == PIXMAN_OP_SRC)
    {
	uint32_t pixel;

	if (color_to_pixel (color, &pixel, dest->bits.format))
	{
	    for (i = 0; i < n_rects; ++i)
	    {
		pixman_region32_t fill_region;
		int n_boxes, j;
		pixman_box32_t *boxes;

		pixman_region32_init_rect (&fill_region, rects[i].x, rects[i].y, rects[i].width, rects[i].height);
		if (!pixman_region32_intersect (&fill_region,
						&fill_region,
						&dest->common.clip_region))
		    return FALSE;


		boxes = pixman_region32_rectangles (&fill_region, &n_boxes);
		for (j = 0; j < n_boxes; ++j)
		{
		    const pixman_box32_t *box = &(boxes[j]);
		    pixman_fill (dest->bits.bits, dest->bits.rowstride, PIXMAN_FORMAT_BPP (dest->bits.format),
				 box->x1, box->y1, box->x2 - box->x1, box->y2 - box->y1,
				 pixel);
		}

		pixman_region32_fini (&fill_region);
	    }
	    return TRUE;
	}
    }

    solid = pixman_image_create_solid_fill (color);
    if (!solid)
	return FALSE;

    for (i = 0; i < n_rects; ++i)
    {
	const pixman_rectangle16_t *rect = &(rects[i]);

	pixman_image_composite (op, solid, NULL, dest,
				0, 0, 0, 0,
				rect->x, rect->y,
				rect->width, rect->height);
    }

    pixman_image_unref (solid);

    return TRUE;
}

pixman_bool_t
pixman_image_can_get_solid (pixman_image_t *image)
{
    if (image->type == SOLID)
	return TRUE;

    if (image->type != BITS	||
	image->bits.width != 1	||
	image->bits.height != 1)
    {
	return FALSE;
    }

    if (image->common.repeat != PIXMAN_REPEAT_NORMAL)
	return FALSE;

    switch (image->bits.format)
    {
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
	return TRUE;
    default:
	return FALSE;
    }
}

pixman_bool_t
pixman_image_is_opaque(pixman_image_t *image)
{
    int i = 0;
    int gradientNumberOfColors = 0;

    if(image->common.alpha_map)
        return FALSE;

    switch(image->type)
    {
    case BITS:
        if(PIXMAN_FORMAT_A(image->bits.format))
            return FALSE;
        break;

    case LINEAR:
    case CONICAL:
    case RADIAL:
        gradientNumberOfColors = image->gradient.n_stops;
        i=0;
        while(i<gradientNumberOfColors)
        {
            if(image->gradient.stops[i].color.alpha != 0xffff)
                return FALSE;
            i++;
        }
        break;

    case SOLID:
         if(Alpha(image->solid.color) != 0xff)
            return FALSE;
        break;
    }

    /* Convolution filters can introduce translucency if the sum of the weights
       is lower than 1. */
    if (image->common.filter == PIXMAN_FILTER_CONVOLUTION)
         return FALSE;

    if (image->common.repeat == PIXMAN_REPEAT_NONE)
    {
        if (image->common.filter != PIXMAN_FILTER_NEAREST)
            return FALSE;

        if (image->common.transform)
            return FALSE;

	/* Gradients do not necessarily cover the entire compositing area */
	if (image->type == LINEAR || image->type == CONICAL || image->type == RADIAL)
	    return FALSE;
    }

     return TRUE;
}
