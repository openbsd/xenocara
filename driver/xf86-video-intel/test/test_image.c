#include <stdint.h>
#include <stdio.h>
#include <string.h>

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
	    const XImage *real, const XImage *ref,
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
				       *(uint32_t*)(real->data +
						    j*real->bytes_per_line +
						    i*real->bits_per_pixel/8));
		}

		len += sprintf(buf+len, "\t");

		for (i = x - 2; i <= x + 2; i++) {
			if (i < 0 || i >= w)
				continue;

			len += sprintf(buf+len,
				       "%08x ",
				       *(uint32_t*)(ref->data +
						    j*real->bytes_per_line +
						    i*real->bits_per_pixel/8));
		}

		len += sprintf(buf+len, "\n");
	}
}

static void test_compare_fallback(struct test *t,
				  Drawable real_draw, XRenderPictFormat *real_format,
				  Drawable ref_draw, XRenderPictFormat *ref_format,
				  int x, int y, int w, int h)
{
	XImage *real_image, *ref_image;
	char *real, *ref;
	char buf[600];
	uint32_t mask;
	int i, j;

	die_unless(real_format->depth == ref_format->depth);

	real_image = XGetImage(t->real.dpy, real_draw,
			       x, y, w, h,
			       AllPlanes, ZPixmap);
	real = real_image->data;

	ref_image = XGetImage(t->ref.dpy, ref_draw,
			      x, y, w, h,
			      AllPlanes, ZPixmap);
	ref = ref_image->data;

	mask = depth_mask(real_image->depth);

	/* Start with an exact comparison. However, one quicky desires
	 * a fuzzy comparator to hide hardware inaccuracies...
	 */
	for (j = 0; j < h; j++) {
		for (i = 0; i < w; i++) {
			uint32_t a = ((uint32_t *)real)[i] & mask;
			uint32_t b = ((uint32_t *)ref)[i] & mask;
			if (a != b && pixel_difference(a, b) > MAX_DELTA) {
				show_pixels(buf,
					    real_image, ref_image,
					    i, j, w, h);
				die("discrepancy found at (%d+%d, %d+%d): found %08x, expected %08x (delta: %d)\n%s",
				    x,i, y,j, a, b, pixel_difference(a, b), buf);
			}
		}
		real += real_image->bytes_per_line;
		ref += ref_image->bytes_per_line;
	}

	XDestroyImage(real_image);
	XDestroyImage(ref_image);
}

void test_compare(struct test *t,
		  Drawable real_draw, XRenderPictFormat *real_format,
		  Drawable ref_draw, XRenderPictFormat *ref_format,
		  int x, int y, int w, int h,
		  const char *info)
{
	XImage real_image, ref_image;
	Pixmap tmp;
	char *real, *ref;
	char buf[600];
	uint32_t mask;
	int i, j;
	XGCValues gcv;
	GC gc;

	if (w * h * 4 > t->real.max_shm_size)
		return test_compare_fallback(t,
					     real_draw, real_format,
					     ref_draw, ref_format,
					     x, y, w, h);

	test_init_image(&real_image, &t->real.shm, real_format, w, h);
	test_init_image(&ref_image, &t->ref.shm, ref_format, w, h);

	gcv.graphics_exposures = 0;

	die_unless(real_image.depth == ref_image.depth);
	die_unless(real_image.bits_per_pixel == ref_image.bits_per_pixel);
	die_unless(real_image.bits_per_pixel == 32);

	mask = depth_mask(real_image.depth);

	tmp = XCreatePixmap(t->real.dpy, real_draw, w, h, real_image.depth);
	gc = XCreateGC(t->real.dpy, tmp, GCGraphicsExposures, &gcv);
	XCopyArea(t->real.dpy, real_draw, tmp, gc, x, y, w, h, 0, 0);
	XShmGetImage(t->real.dpy, tmp, &real_image, 0, 0, AllPlanes);
	XFreeGC(t->real.dpy, gc);
	XFreePixmap(t->real.dpy, tmp);
	real = real_image.data;

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
			uint32_t a = ((uint32_t *)real)[i] & mask;
			uint32_t b = ((uint32_t *)ref)[i] & mask;
			if (a != b && pixel_difference(a, b) > MAX_DELTA) {
				show_pixels(buf,
					    &real_image, &ref_image,
					    i, j, w, h);
				die("discrepancy found at (%d+%d, %d+%d): found %08x, expected %08x (delta: %d)\n%s%s\n",
				    x,i, y,j, a, b, pixel_difference(a, b), buf, info);
			}
		}
		real += real_image.bytes_per_line;
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
