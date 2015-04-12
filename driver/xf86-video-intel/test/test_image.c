#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <png.h>

#include "test.h"

#define MAX_DELTA 3

int pixel_difference(uint32_t a, uint32_t b)
{
	int max = 0;
	int i;

	for (i = 0; i < 32; i += 8) {
		uint8_t ac = (a >> i) & 0xff;
		uint8_t bc = (b >> i) & 0xff;
		int d;

		if (ac > bc)
			d = ac - bc;
		else
			d = bc - ac;
		if (d > max)
			max = d;
	}

	return max;
}

static void
show_pixels(char *buf,
	    const XImage *out, const XImage *ref,
	    int x, int y, int w, int h)
{
	int i, j, len = 0;

	for (j = y - 2; j <= y + 2; j++) {
		if (j < 0 || j >= h)
			continue;

		for (i = x - 2; i <= x + 2; i++) {
			if (i < 0 || i >= w)
				continue;

			len += sprintf(buf+len,
				       "%08x ",
				       *(uint32_t*)(out->data +
						    j*out->bytes_per_line +
						    i*out->bits_per_pixel/8));
		}

		len += sprintf(buf+len, "\t");

		for (i = x - 2; i <= x + 2; i++) {
			if (i < 0 || i >= w)
				continue;

			len += sprintf(buf+len,
				       "%08x ",
				       *(uint32_t*)(ref->data +
						    j*out->bytes_per_line +
						    i*out->bits_per_pixel/8));
		}

		len += sprintf(buf+len, "\n");
	}
}

static void test_compare_fallback(struct test *t,
				  Drawable out_draw, XRenderPictFormat *out_format,
				  Drawable ref_draw, XRenderPictFormat *ref_format,
				  int x, int y, int w, int h)
{
	XImage *out_image, *ref_image;
	char *out, *ref;
	char buf[600];
	uint32_t mask;
	int i, j;

	die_unless(out_format->depth == ref_format->depth);

	out_image = XGetImage(t->out.dpy, out_draw,
			       x, y, w, h,
			       AllPlanes, ZPixmap);
	out = out_image->data;

	ref_image = XGetImage(t->ref.dpy, ref_draw,
			      x, y, w, h,
			      AllPlanes, ZPixmap);
	ref = ref_image->data;

	mask = depth_mask(out_image->depth);

	/* Start with an exact comparison. However, one quicky desires
	 * a fuzzy comparator to hide hardware inaccuracies...
	 */
	for (j = 0; j < h; j++) {
		for (i = 0; i < w; i++) {
			uint32_t a = ((uint32_t *)out)[i] & mask;
			uint32_t b = ((uint32_t *)ref)[i] & mask;
			if (a != b && pixel_difference(a, b) > MAX_DELTA) {
				show_pixels(buf,
					    out_image, ref_image,
					    i, j, w, h);
				die("discrepancy found at (%d+%d, %d+%d): found %08x, expected %08x (delta: %d)\n%s",
				    x,i, y,j, a, b, pixel_difference(a, b), buf);
			}
		}
		out += out_image->bytes_per_line;
		ref += ref_image->bytes_per_line;
	}

	XDestroyImage(out_image);
	XDestroyImage(ref_image);
}

static void
unpremultiply_data (png_structp png, png_row_infop row_info, png_bytep data)
{
	unsigned int i;

	for (i = 0; i < row_info->rowbytes; i += 4) {
		uint8_t *b = &data[i];
		uint32_t pixel;
		uint8_t  alpha;

		memcpy (&pixel, b, sizeof (uint32_t));
		alpha = (pixel & 0xff000000) >> 24;
		if (alpha == 0) {
			b[0] = (pixel & 0xff0000) >> 16;
			b[1] = (pixel & 0x00ff00) >>  8;
			b[2] = (pixel & 0x0000ff) >>  0;
			b[3] = 0xff;
		} else {
			b[0] = (((pixel & 0xff0000) >> 16) * 255 + alpha / 2) / alpha;
			b[1] = (((pixel & 0x00ff00) >>  8) * 255 + alpha / 2) / alpha;
			b[2] = (((pixel & 0x0000ff) >>  0) * 255 + alpha / 2) / alpha;
			b[3] = alpha;
		}
	}
}

static void save_image(XImage *image, const char *filename)
{
	FILE *file;
	png_struct *png = NULL;
	png_info *info = NULL;
	png_byte **rows = NULL;
	int i;

	file = fopen(filename, "w");
	if (file == NULL)
		return;

	png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (png == NULL)
		goto out;

	info = png_create_info_struct(png);
	if (info == NULL)
		goto out;

	rows = png_malloc(png, sizeof(png_byte *) * image->height);
	if (rows == NULL)
		goto out;
	for (i = 0; i < image->height; i++)
		rows[i] = (png_byte *)(image->data + image->bytes_per_line * i);

	if (setjmp(png_jmpbuf(png)))
		goto out;

	png_set_IHDR(png, info,
		     image->width, image->height, 8,
		     PNG_COLOR_TYPE_RGB_ALPHA,
		     PNG_INTERLACE_NONE,
		     PNG_COMPRESSION_TYPE_DEFAULT,
		     PNG_FILTER_TYPE_DEFAULT);

	png_init_io(png, file);
	png_write_info(png, info);
	png_set_write_user_transform_fn(png, unpremultiply_data);
	png_write_image(png, rows);
	png_write_end(png, info);

out:
	if (rows)
		png_free(png, rows);
	png_destroy_write_struct(&png, &info);
	fclose(file);
}

void test_compare(struct test *t,
		  Drawable out_draw, XRenderPictFormat *out_format,
		  Drawable ref_draw, XRenderPictFormat *ref_format,
		  int x, int y, int w, int h,
		  const char *info)
{
	XImage out_image, ref_image;
	Pixmap tmp;
	char *out, *ref;
	char buf[600];
	uint32_t mask;
	int i, j;
	XGCValues gcv;
	GC gc;

	if (w * h * 4 > t->out.max_shm_size)
		return test_compare_fallback(t,
					     out_draw, out_format,
					     ref_draw, ref_format,
					     x, y, w, h);

	test_init_image(&out_image, &t->out.shm, out_format, w, h);
	test_init_image(&ref_image, &t->ref.shm, ref_format, w, h);

	gcv.graphics_exposures = 0;

	die_unless(out_image.depth == ref_image.depth);
	die_unless(out_image.bits_per_pixel == ref_image.bits_per_pixel);
	die_unless(out_image.bits_per_pixel == 32);

	mask = depth_mask(out_image.depth);

	tmp = XCreatePixmap(t->out.dpy, out_draw, w, h, out_image.depth);
	gc = XCreateGC(t->out.dpy, tmp, GCGraphicsExposures, &gcv);
	XCopyArea(t->out.dpy, out_draw, tmp, gc, x, y, w, h, 0, 0);
	XShmGetImage(t->out.dpy, tmp, &out_image, 0, 0, AllPlanes);
	XFreeGC(t->out.dpy, gc);
	XFreePixmap(t->out.dpy, tmp);
	out = out_image.data;

	tmp = XCreatePixmap(t->ref.dpy, ref_draw, w, h, ref_image.depth);
	gc = XCreateGC(t->ref.dpy, tmp, GCGraphicsExposures, &gcv);
	XCopyArea(t->ref.dpy, ref_draw, tmp, gc, x, y, w, h, 0, 0);
	XShmGetImage(t->ref.dpy, tmp, &ref_image, 0, 0, AllPlanes);
	XFreeGC(t->ref.dpy, gc);
	XFreePixmap(t->ref.dpy, tmp);
	ref = ref_image.data;

	/* Start with an exact comparison. However, one quicky desires
	 * a fuzzy comparator to hide hardware inaccuracies...
	 */
	for (j = 0; j < h; j++) {
		for (i = 0; i < w; i++) {
			uint32_t a = ((uint32_t *)out)[i] & mask;
			uint32_t b = ((uint32_t *)ref)[i] & mask;
			if (a != b && pixel_difference(a, b) > MAX_DELTA) {
				show_pixels(buf,
					    &out_image, &ref_image,
					    i, j, w, h);
				save_image(&out_image, "out.png");
				save_image(&ref_image,  "ref.png");
				die("discrepancy found at (%d+%d, %d+%d): found %08x, expected %08x (delta: %d)\n%s%s\n",
				    x,i, y,j, a, b, pixel_difference(a, b), buf, info);
			}
		}
		out += out_image.bytes_per_line;
		ref += ref_image.bytes_per_line;
	}
}

static int
_native_byte_order_lsb(void)
{
	int x = 1;
	return *((char *) &x) == 1;
}

void
test_init_image(XImage *ximage,
		XShmSegmentInfo *shm,
		XRenderPictFormat *format,
		int width, int height)
{
	int native_byte_order = _native_byte_order_lsb() ? LSBFirst : MSBFirst;

	ximage->width = width;
	ximage->height = height;
	ximage->format = ZPixmap;
	ximage->data = shm->shmaddr;
	ximage->obdata = (void *)shm;
	ximage->byte_order = native_byte_order;
	ximage->bitmap_unit = 32;
	ximage->bitmap_bit_order = native_byte_order;
	ximage->bitmap_pad = 32;
	ximage->depth = format->depth;
	ximage->bytes_per_line = 4*width;
	ximage->bits_per_pixel = 32;
	ximage->red_mask = 0xff << 16;
	ximage->green_mask = 0xff << 8;
	ximage->blue_mask = 0xff << 0;
	ximage->xoffset = 0;

	XInitImage(ximage);
}
