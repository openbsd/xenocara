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

#include <config.h>
#include <stdlib.h>
#include "pixman-private.h"


#define READ_ACCESS(f) ((image->common.read_func)? f##_accessors : f)
#define WRITE_ACCESS(f) ((image->common.write_func)? f##_accessors : f)

static void
fbFetchSolid(bits_image_t * image,
	     int x, int y, int width,
	     uint32_t *buffer,
	     uint32_t *mask, uint32_t maskBits)
{
    uint32_t color;
    uint32_t *end;
    fetchPixelProc32 fetch =
	READ_ACCESS(pixman_fetchPixelProcForPicture32)(image);
    
    color = fetch(image, 0, 0);
    
    end = buffer + width;
    while (buffer < end)
	*(buffer++) = color;
}

static void
fbFetchSolid64(bits_image_t * image,
	       int x, int y, int width,
	       uint64_t *buffer, void *unused, uint32_t unused2)
{
    uint64_t color;
    uint64_t *end;
    fetchPixelProc64 fetch =
	READ_ACCESS(pixman_fetchPixelProcForPicture64)(image);
    
    color = fetch(image, 0, 0);
    
    end = buffer + width;
    while (buffer < end)
	*(buffer++) = color;
}

static void
fbFetch(bits_image_t * image,
	int x, int y, int width,
	uint32_t *buffer, uint32_t *mask, uint32_t maskBits)
{
    fetchProc32 fetch = READ_ACCESS(pixman_fetchProcForPicture32)(image);
    
    fetch(image, x, y, width, buffer);
}

static void
fbFetch64(bits_image_t * image,
	  int x, int y, int width,
	  uint64_t *buffer, void *unused, uint32_t unused2)
{
    fetchProc64 fetch = READ_ACCESS(pixman_fetchProcForPicture64)(image);
    
    fetch(image, x, y, width, buffer);
}

static void
fbStore(bits_image_t * image, int x, int y, int width, uint32_t *buffer)
{
    uint32_t *bits;
    int32_t stride;
    storeProc32 store = WRITE_ACCESS(pixman_storeProcForPicture32)(image);
    const pixman_indexed_t * indexed = image->indexed;

    bits = image->bits;
    stride = image->rowstride;
    bits += y*stride;
    store((pixman_image_t *)image, bits, buffer, x, width, indexed);
}

static void
fbStore64 (bits_image_t * image, int x, int y, int width, uint64_t *buffer)
{
    uint32_t *bits;
    int32_t stride;
    storeProc64 store = WRITE_ACCESS(pixman_storeProcForPicture64)(image);
    const pixman_indexed_t * indexed = image->indexed;

    bits = image->bits;
    stride = image->rowstride;
    bits += y*stride;
    store((pixman_image_t *)image, bits, buffer, x, width, indexed);
}

static void
fbStoreExternalAlpha (bits_image_t * image, int x, int y, int width,
		      uint32_t *buffer)
{
    uint32_t *bits, *alpha_bits;
    int32_t stride, astride;
    int ax, ay;
    storeProc32 store;
    storeProc32 astore;
    const pixman_indexed_t * indexed = image->indexed;
    const pixman_indexed_t * aindexed;

    if (!image->common.alpha_map) {
        // XXX[AGP]: This should never happen!
        // fbStore(image, x, y, width, buffer);
        abort();
	return;
    }

    store = WRITE_ACCESS(pixman_storeProcForPicture32)(image);
    astore = WRITE_ACCESS(pixman_storeProcForPicture32)(image->common.alpha_map);
    aindexed = image->common.alpha_map->indexed;

    ax = x;
    ay = y;

    bits = image->bits;
    stride = image->rowstride;

    alpha_bits = image->common.alpha_map->bits;
    astride = image->common.alpha_map->rowstride;

    bits       += y*stride;
    alpha_bits += (ay - image->common.alpha_origin.y)*astride;


    store((pixman_image_t *)image, bits, buffer, x, width, indexed);
    astore((pixman_image_t *)image->common.alpha_map,
	   alpha_bits, buffer, ax - image->common.alpha_origin.x, width, aindexed);
}

static void
fbStoreExternalAlpha64 (bits_image_t * image, int x, int y, int width,
			uint64_t *buffer)
{
    uint32_t *bits, *alpha_bits;
    int32_t stride, astride;
    int ax, ay;
    storeProc64 store;
    storeProc64 astore;
    const pixman_indexed_t * indexed = image->indexed;
    const pixman_indexed_t * aindexed;

    store = ACCESS(pixman_storeProcForPicture64)(image);
    astore = ACCESS(pixman_storeProcForPicture64)(image->common.alpha_map);
    aindexed = image->common.alpha_map->indexed;

    ax = x;
    ay = y;

    bits = image->bits;
    stride = image->rowstride;

    alpha_bits = image->common.alpha_map->bits;
    astride = image->common.alpha_map->rowstride;

    bits       += y*stride;
    alpha_bits += (ay - image->common.alpha_origin.y)*astride;


    store((pixman_image_t *)image, bits, buffer, x, width, indexed);
    astore((pixman_image_t *)image->common.alpha_map,
	   alpha_bits, buffer, ax - image->common.alpha_origin.x, width, aindexed);
}

static void
bits_image_property_changed (pixman_image_t *image)
{
    bits_image_t *bits = (bits_image_t *)image;
    
    if (bits->common.alpha_map)
    {
	image->common.get_scanline_64 =
	    (scanFetchProc)_pixman_image_get_scanline_64_generic;
	image->common.get_scanline_32 =
	    (scanFetchProc)READ_ACCESS(fbFetchExternalAlpha);
    }
    else if ((bits->common.repeat != PIXMAN_REPEAT_NONE) &&
	    bits->width == 1 &&
	    bits->height == 1)
    {
	image->common.get_scanline_64 = (scanFetchProc)fbFetchSolid64;
	image->common.get_scanline_32 = (scanFetchProc)fbFetchSolid;
    }
    else if (!bits->common.transform &&
	     bits->common.filter != PIXMAN_FILTER_CONVOLUTION &&
	     bits->common.repeat != PIXMAN_REPEAT_PAD &&
	     bits->common.repeat != PIXMAN_REPEAT_REFLECT)
    {
	image->common.get_scanline_64 = (scanFetchProc)fbFetch64;
	image->common.get_scanline_32 = (scanFetchProc)fbFetch;
    }
    else
    {
	image->common.get_scanline_64 =
	    (scanFetchProc)_pixman_image_get_scanline_64_generic;
	image->common.get_scanline_32 =
	    (scanFetchProc)READ_ACCESS(fbFetchTransformed);
    }
    
    if (bits->common.alpha_map)
    {
	bits->store_scanline_64 = (scanStoreProc)fbStoreExternalAlpha64;
	bits->store_scanline_32 = fbStoreExternalAlpha;
    }
    else
    {
	bits->store_scanline_64 = (scanStoreProc)fbStore64;
	bits->store_scanline_32 = fbStore;
    }
}

void
_pixman_image_store_scanline_32 (bits_image_t *image, int x, int y, int width,
				 uint32_t *buffer)
{
    image->store_scanline_32 (image, x, y, width, buffer);
}

void
_pixman_image_store_scanline_64 (bits_image_t *image, int x, int y, int width,
				 uint32_t *buffer)
{
    image->store_scanline_64 (image, x, y, width, buffer);
}

static uint32_t *
create_bits (pixman_format_code_t format,
	     int		  width,
	     int		  height,
	     int		 *rowstride_bytes)
{
    int stride;
    int buf_size;
    int bpp;
    
    /* what follows is a long-winded way, avoiding any possibility of integer
     * overflows, of saying:
     * stride = ((width * bpp + FB_MASK) >> FB_SHIFT) * sizeof (uint32_t);
     */
    
    bpp = PIXMAN_FORMAT_BPP (format);
    if (pixman_multiply_overflows_int (width, bpp))
	return NULL;
    
    stride = width * bpp;
    if (pixman_addition_overflows_int (stride, FB_MASK))
	return NULL;
    
    stride += FB_MASK;
    stride >>= FB_SHIFT;
    
#if FB_SHIFT < 2
    if (pixman_multiply_overflows_int (stride, sizeof (uint32_t)))
	return NULL;
#endif
    stride *= sizeof (uint32_t);
    
    if (pixman_multiply_overflows_int (height, stride))
	return NULL;
    
    buf_size = height * stride;
    
    if (rowstride_bytes)
	*rowstride_bytes = stride;
    
    return calloc (buf_size, 1);
}

PIXMAN_EXPORT pixman_image_t *
pixman_image_create_bits (pixman_format_code_t  format,
			  int                   width,
			  int                   height,
			  uint32_t	       *bits,
			  int			rowstride_bytes)
{
    pixman_image_t *image;
    uint32_t *free_me = NULL;
    
    /* must be a whole number of uint32_t's
     */
    return_val_if_fail (bits == NULL ||
			(rowstride_bytes % sizeof (uint32_t)) == 0, NULL);
    
    if (!bits && width && height)
    {
	free_me = bits = create_bits (format, width, height, &rowstride_bytes);
	if (!bits)
	    return NULL;
    }
    
    image = _pixman_image_allocate();
    
    if (!image) {
	if (free_me)
	    free (free_me);
	return NULL;
    }
    
    image->type = BITS;
    image->bits.format = format;
    image->bits.width = width;
    image->bits.height = height;
    image->bits.bits = bits;
    image->bits.free_me = free_me;
    
    image->bits.rowstride = rowstride_bytes / (int) sizeof (uint32_t); /* we store it in number
									* of uint32_t's
									*/
    image->bits.indexed = NULL;
    
    pixman_region32_fini (&image->common.full_region);
    pixman_region32_init_rect (&image->common.full_region, 0, 0,
			       image->bits.width, image->bits.height);
    
    image->common.property_changed = bits_image_property_changed;
    
    bits_image_property_changed (image);
    
    _pixman_image_reset_clip_region (image);
    
    return image;
}
