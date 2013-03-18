/*
 * Copyright © 2001 Keith Packard
 *
 * Partly based on code that is Copyright © The XFree86 Project Inc.
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of Keith Packard not be used in
 * advertising or publicity pertaining to distribution of the software without
 * specific, written prior permission.  Keith Packard makes no
 * representations about the suitability of this software for any purpose.  It
 * is provided "as is" without express or implied warranty.
 *
 * KEITH PACKARD DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO
 * EVENT SHALL KEITH PACKARD BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
 * DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */

#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#include <stdlib.h>

#include "uxa-priv.h"
#include "uxa-glamor.h"
#include <xorgVersion.h>

#ifdef RENDER
#include "mipict.h"

/* Note: when using glamor we can not fail through to the ordinary UXA
 * code paths, as glamor keeps an internal texture which will become
 * inconsistent with the original bo. (The texture is replaced whenever
 * the format changes, e.g. switching between xRGB and ARGB, for which mesa
 * will allocate its own bo.)
 *
 * Ergo it is unsafe to fall through to the original backend operations if
 * glamor is enabled.
 *
 * XXX This has some serious implications for mixing Render, DRI, scanout...
 */

static void uxa_composite_fallback_pict_desc(PicturePtr pict, char *string,
					     int n)
{
	char format[20];
	char size[20];
	char loc;

	if (!pict) {
		snprintf(string, n, "None");
		return;
	}

	if (pict->pDrawable == NULL) {
		snprintf(string, n, "source-only");
		return;
	}

	switch (pict->format) {
	case PICT_a8r8g8b8:
		snprintf(format, 20, "ARGB8888");
		break;
	case PICT_x8r8g8b8:
		snprintf(format, 20, "XRGB8888");
		break;
	case PICT_r5g6b5:
		snprintf(format, 20, "RGB565  ");
		break;
	case PICT_x1r5g5b5:
		snprintf(format, 20, "RGB555  ");
		break;
	case PICT_a8:
		snprintf(format, 20, "A8      ");
		break;
	case PICT_a1:
		snprintf(format, 20, "A1      ");
		break;
	default:
		snprintf(format, 20, "0x%x", (int)pict->format);
		break;
	}

	loc = uxa_drawable_is_offscreen(pict->pDrawable) ? 's' : 'm';

	snprintf(size, 20, "%dx%d%s", pict->pDrawable->width,
		 pict->pDrawable->height, pict->repeat ? " R" : "");

	snprintf(string, n, "%p:%c fmt %s (%s)%s",
		 pict->pDrawable, loc, format, size,
		 pict->alphaMap ? " with alpha map" :"");
}

static const char *
op_to_string(CARD8 op)
{
    switch (op) {
#define C(x) case PictOp##x: return #x
	C(Clear);
	C(Src);
	C(Dst);
	C(Over);
	C(OverReverse);
	C(In);
	C(InReverse);
	C(Out);
	C(OutReverse);
	C(Atop);
	C(AtopReverse);
	C(Xor);
	C(Add);
	C(Saturate);

	/*
	 * Operators only available in version 0.2
	 */
#if RENDER_MAJOR >= 1 || RENDER_MINOR >= 2
	C(DisjointClear);
	C(DisjointSrc);
	C(DisjointDst);
	C(DisjointOver);
	C(DisjointOverReverse);
	C(DisjointIn);
	C(DisjointInReverse);
	C(DisjointOut);
	C(DisjointOutReverse);
	C(DisjointAtop);
	C(DisjointAtopReverse);
	C(DisjointXor);

	C(ConjointClear);
	C(ConjointSrc);
	C(ConjointDst);
	C(ConjointOver);
	C(ConjointOverReverse);
	C(ConjointIn);
	C(ConjointInReverse);
	C(ConjointOut);
	C(ConjointOutReverse);
	C(ConjointAtop);
	C(ConjointAtopReverse);
	C(ConjointXor);
#endif

	/*
	 * Operators only available in version 0.11
	 */
#if RENDER_MAJOR >= 1 || RENDER_MINOR >= 11
	C(Multiply);
	C(Screen);
	C(Overlay);
	C(Darken);
	C(Lighten);
	C(ColorDodge);
	C(ColorBurn);
	C(HardLight);
	C(SoftLight);
	C(Difference);
	C(Exclusion);
	C(HSLHue);
	C(HSLSaturation);
	C(HSLColor);
	C(HSLLuminosity);
#endif
    default: return "garbage";
#undef C
    }
}

static void
uxa_print_composite_fallback(const char *func, CARD8 op,
			     PicturePtr pSrc, PicturePtr pMask, PicturePtr pDst)
{
	uxa_screen_t *uxa_screen = uxa_get_screen(pDst->pDrawable->pScreen);
	char srcdesc[40], maskdesc[40], dstdesc[40];

	if (! uxa_screen->fallback_debug)
		return;

	/* Limit the noise if fallbacks are expected. */
	if (uxa_screen->force_fallback)
		return;

	uxa_composite_fallback_pict_desc(pSrc, srcdesc, 40);
	uxa_composite_fallback_pict_desc(pMask, maskdesc, 40);
	uxa_composite_fallback_pict_desc(pDst, dstdesc, 40);

	ErrorF("Composite fallback at %s:\n"
	       "  op   %s, \n"
	       "  src  %s, \n"
	       "  mask %s, \n"
	       "  dst  %s, \n",
	       func, op_to_string (op), srcdesc, maskdesc, dstdesc);
}

Bool uxa_op_reads_destination(CARD8 op)
{
	/* FALSE (does not read destination) is the list of ops in the protocol
	 * document with "0" in the "Fb" column and no "Ab" in the "Fa" column.
	 * That's just Clear and Src.  ReduceCompositeOp() will already have
	 * converted con/disjoint clear/src to Clear or Src.
	 */
	switch (op) {
	case PictOpClear:
	case PictOpSrc:
		return FALSE;
	default:
		return TRUE;
	}
}

static Bool
uxa_get_pixel_from_rgba(CARD32 * pixel,
			CARD16 red,
			CARD16 green,
			CARD16 blue,
			CARD16 alpha,
			CARD32 format)
{
	int rbits, bbits, gbits, abits;
	int rshift, bshift, gshift, ashift;

	rbits = PICT_FORMAT_R(format);
	gbits = PICT_FORMAT_G(format);
	bbits = PICT_FORMAT_B(format);
	abits = PICT_FORMAT_A(format);
	if (abits == 0)
	    abits = PICT_FORMAT_BPP(format) - (rbits+gbits+bbits);

	if (PICT_FORMAT_TYPE(format) == PICT_TYPE_A) {
		*pixel = alpha >> (16 - abits);
		return TRUE;
	}

	if (!PICT_FORMAT_COLOR(format))
		return FALSE;

	if (PICT_FORMAT_TYPE(format) == PICT_TYPE_ARGB) {
		bshift = 0;
		gshift = bbits;
		rshift = gshift + gbits;
		ashift = rshift + rbits;
	} else if (PICT_FORMAT_TYPE(format) == PICT_TYPE_ABGR) {
		rshift = 0;
		gshift = rbits;
		bshift = gshift + gbits;
		ashift = bshift + bbits;
#if XORG_VERSION_CURRENT >= 10699900
	} else if (PICT_FORMAT_TYPE(format) == PICT_TYPE_BGRA) {
		ashift = 0;
		rshift = abits;
		gshift = rshift + rbits;
		bshift = gshift + gbits;
#endif
	} else {
		return FALSE;
	}

	*pixel = 0;
	*pixel |= (blue  >> (16 - bbits)) << bshift;
	*pixel |= (green >> (16 - gbits)) << gshift;
	*pixel |= (red   >> (16 - rbits)) << rshift;
	*pixel |= (alpha >> (16 - abits)) << ashift;

	return TRUE;
}

Bool
uxa_get_rgba_from_pixel(CARD32 pixel,
			CARD16 * red,
			CARD16 * green,
			CARD16 * blue,
			CARD16 * alpha,
			CARD32 format)
{
	int rbits, bbits, gbits, abits;
	int rshift, bshift, gshift, ashift;

	rbits = PICT_FORMAT_R(format);
	gbits = PICT_FORMAT_G(format);
	bbits = PICT_FORMAT_B(format);
	abits = PICT_FORMAT_A(format);

	if (PICT_FORMAT_TYPE(format) == PICT_TYPE_A) {
		rshift = gshift = bshift = ashift = 0;
        } else if (PICT_FORMAT_TYPE(format) == PICT_TYPE_ARGB) {
		bshift = 0;
		gshift = bbits;
		rshift = gshift + gbits;
		ashift = rshift + rbits;
        } else if (PICT_FORMAT_TYPE(format) == PICT_TYPE_ABGR) {
		rshift = 0;
		gshift = rbits;
		bshift = gshift + gbits;
		ashift = bshift + bbits;
#if XORG_VERSION_CURRENT >= 10699900
        } else if (PICT_FORMAT_TYPE(format) == PICT_TYPE_BGRA) {
		ashift = 0;
		rshift = abits;
		if (abits == 0)
			rshift = PICT_FORMAT_BPP(format) - (rbits+gbits+bbits);
		gshift = rshift + rbits;
		bshift = gshift + gbits;
#endif
	} else {
		return FALSE;
	}

	if (rbits) {
		*red = ((pixel >> rshift) & ((1 << rbits) - 1)) << (16 - rbits);
		while (rbits < 16) {
			*red |= *red >> rbits;
			rbits <<= 1;
		}
	} else
		*red = 0;

	if (gbits) {
		*green = ((pixel >> gshift) & ((1 << gbits) - 1)) << (16 - gbits);
		while (gbits < 16) {
			*green |= *green >> gbits;
			gbits <<= 1;
		}
	} else
		*green = 0;

	if (bbits) {
		*blue = ((pixel >> bshift) & ((1 << bbits) - 1)) << (16 - bbits);
		while (bbits < 16) {
			*blue |= *blue >> bbits;
			bbits <<= 1;
		}
	} else
		*blue = 0;

	if (abits) {
		*alpha =
		    ((pixel >> ashift) & ((1 << abits) - 1)) << (16 - abits);
		while (abits < 16) {
			*alpha |= *alpha >> abits;
			abits <<= 1;
		}
	} else
		*alpha = 0xffff;

	return TRUE;
}

Bool
uxa_get_color_for_pixmap (PixmapPtr	 pixmap,
			  CARD32	 src_format,
			  CARD32	 dst_format,
			  CARD32	*pixel)
{
	CARD16 red, green, blue, alpha;

	*pixel = uxa_get_pixmap_first_pixel(pixmap);

	if (src_format != dst_format) {
	    if (!uxa_get_rgba_from_pixel(*pixel,
					 &red, &green, &blue, &alpha,
					 src_format))
		return FALSE;

	    if (!uxa_get_pixel_from_rgba(pixel,
					 red, green, blue, alpha,
					 dst_format))
		return FALSE;
	}

	return TRUE;
}

static int
uxa_try_driver_solid_fill(PicturePtr pSrc,
			  PicturePtr pDst,
			  INT16 xSrc,
			  INT16 ySrc,
			  INT16 xDst, INT16 yDst, CARD16 width, CARD16 height)
{
	uxa_screen_t *uxa_screen = uxa_get_screen(pDst->pDrawable->pScreen);
	RegionRec region;
	BoxPtr pbox;
	int nbox;
	int dst_off_x, dst_off_y;
	PixmapPtr pSrcPix = NULL, pDstPix;
	CARD32 pixel;

	if (uxa_screen->info->check_solid && !uxa_screen->info->check_solid(pDst->pDrawable, GXcopy, FB_ALLONES))
		return -1;

	pDstPix = uxa_get_offscreen_pixmap(pDst->pDrawable, &dst_off_x, &dst_off_y);
	if (!pDstPix)
		return -1;

	xDst += pDst->pDrawable->x;
	yDst += pDst->pDrawable->y;

	if (pSrc->pDrawable) {
		pSrcPix = uxa_get_drawable_pixmap(pSrc->pDrawable);
		xSrc += pSrc->pDrawable->x;
		ySrc += pSrc->pDrawable->y;
	}

	if (!miComputeCompositeRegion(&region, pSrc, NULL, pDst,
				      xSrc, ySrc, 0, 0, xDst, yDst,
				      width, height))
		return 1;

	if (pSrcPix) {
		if (! uxa_get_color_for_pixmap (pSrcPix, pSrc->format, pDst->format, &pixel)) {
			REGION_UNINIT(pDst->pDrawable->pScreen, &region);
			return -1;
		}
	} else {
		SourcePict *source = pSrc->pSourcePict;
		PictSolidFill *solid = &source->solidFill;

		if (source == NULL || source->type != SourcePictTypeSolidFill) {
			REGION_UNINIT(pDst->pDrawable->pScreen, &region);
			return -1;
		}

		if (pDst->format == PICT_a8r8g8b8) {
			pixel = solid->color;
		} else if (pDst->format == PICT_x8r8g8b8) {
			pixel = solid->color | 0xff000000;
		} else {
			CARD16 red, green, blue, alpha;

			if (!uxa_get_rgba_from_pixel(solid->color,
						     &red, &green, &blue, &alpha,
						     PICT_a8r8g8b8) ||
			    !uxa_get_pixel_from_rgba(&pixel,
						     red, green, blue, alpha,
						     pDst->format)) {
				REGION_UNINIT(pDst->pDrawable->pScreen, &region);
				return -1;
			}
		}
	}

	if (!(*uxa_screen->info->prepare_solid)
	    (pDstPix, GXcopy, FB_ALLONES, pixel)) {
		REGION_UNINIT(pDst->pDrawable->pScreen, &region);
		return -1;
	}

	REGION_TRANSLATE(pScreen, &region, dst_off_x, dst_off_y);

	nbox = REGION_NUM_RECTS(&region);
	pbox = REGION_RECTS(&region);

	while (nbox--) {
		(*uxa_screen->info->solid) (pDstPix, pbox->x1, pbox->y1,
					    pbox->x2, pbox->y2);
		pbox++;
	}

	(*uxa_screen->info->done_solid) (pDstPix);

	REGION_UNINIT(pDst->pDrawable->pScreen, &region);
	return 1;
}

static PicturePtr
uxa_picture_for_pixman_format(ScreenPtr screen,
			      pixman_format_code_t format,
			      int width, int height)
{
	PicturePtr picture;
	PixmapPtr pixmap;
	int error;

	if (format == PIXMAN_a1)
		format = PIXMAN_a8;

	/* fill alpha if unset */
	if (PIXMAN_FORMAT_A(format) == 0)
	    format = PIXMAN_a8r8g8b8;

	pixmap = screen->CreatePixmap(screen, width, height,
					PIXMAN_FORMAT_DEPTH(format),
					UXA_CREATE_PIXMAP_FOR_MAP);
	if (!pixmap)
		return 0;

	if (!uxa_pixmap_is_offscreen(pixmap)) {
		screen->DestroyPixmap(pixmap);
		return 0;
	}

	picture = CreatePicture(0, &pixmap->drawable,
				PictureMatchFormat(screen,
						   PIXMAN_FORMAT_DEPTH(format),
						   format),
				0, 0, serverClient, &error);
	screen->DestroyPixmap(pixmap);
	if (!picture)
		return 0;

	ValidatePicture(picture);

	return picture;
}

static PicturePtr
uxa_picture_from_pixman_image(ScreenPtr screen,
			      pixman_image_t * image,
			      pixman_format_code_t format)
{
	uxa_screen_t *uxa_screen = uxa_get_screen(screen);
	PicturePtr picture;
	PixmapPtr pixmap;
	int width, height;

	width = pixman_image_get_width(image);
	height = pixman_image_get_height(image);

	picture = uxa_picture_for_pixman_format(screen, format,
						width, height);
	if (!picture)
		return 0;

	if (uxa_screen->info->put_image &&
	    ((picture->pDrawable->depth << 24) | picture->format) == format &&
	    uxa_screen->info->put_image((PixmapPtr)picture->pDrawable,
					0, 0,
					width, height,
					(char *)pixman_image_get_data(image),
					pixman_image_get_stride(image)))
		return picture;

	pixmap = GetScratchPixmapHeader(screen, width, height,
					PIXMAN_FORMAT_DEPTH(format),
					PIXMAN_FORMAT_BPP(format),
					pixman_image_get_stride(image),
					pixman_image_get_data(image));
	if (!pixmap) {
		FreePicture(picture, 0);
		return 0;
	}

	if (((picture->pDrawable->depth << 24) | picture->format) == format) {
		GCPtr gc;

		gc = GetScratchGC(PIXMAN_FORMAT_DEPTH(format), screen);
		if (!gc) {
			FreeScratchPixmapHeader(pixmap);
			FreePicture(picture, 0);
			return 0;
		}
		ValidateGC(picture->pDrawable, gc);

		(*gc->ops->CopyArea) (&pixmap->drawable, picture->pDrawable,
				      gc, 0, 0, width, height, 0, 0);

		FreeScratchGC(gc);
	} else {
		PicturePtr src;
		int error;

		src = CreatePicture(0, &pixmap->drawable,
				    PictureMatchFormat(screen,
						       PIXMAN_FORMAT_DEPTH(format),
						       format),
				    0, 0, serverClient, &error);
		if (!src) {
			FreeScratchPixmapHeader(pixmap);
			FreePicture(picture, 0);
			return 0;
		}
		ValidatePicture(src);

		if (uxa_picture_prepare_access(picture, UXA_ACCESS_RW)) {
			fbComposite(PictOpSrc, src, NULL, picture,
				    0, 0, 0, 0, 0, 0, width, height);
			uxa_picture_finish_access(picture, UXA_ACCESS_RW);
		}

		FreePicture(src, 0);
	}
	FreeScratchPixmapHeader(pixmap);

	return picture;
}

static PicturePtr
uxa_create_solid(ScreenPtr screen, uint32_t color)
{
	PixmapPtr pixmap;
	PicturePtr picture;
	XID repeat = RepeatNormal;
	int error = 0;

	pixmap = (*screen->CreatePixmap)(screen, 1, 1, 32,
					 UXA_CREATE_PIXMAP_FOR_MAP);
	if (!pixmap)
		return 0;

	if (!uxa_prepare_access((DrawablePtr)pixmap, UXA_ACCESS_RW)) {
		(*screen->DestroyPixmap)(pixmap);
		return 0;
	}
	*((uint32_t *)pixmap->devPrivate.ptr) = color;
	uxa_finish_access((DrawablePtr)pixmap, UXA_ACCESS_RW);

	picture = CreatePicture(0, &pixmap->drawable,
				PictureMatchFormat(screen, 32, PICT_a8r8g8b8),
				CPRepeat, &repeat, serverClient, &error);
	(*screen->DestroyPixmap)(pixmap);

	return picture;
}

static PicturePtr
uxa_solid_clear(ScreenPtr screen)
{
	uxa_screen_t *uxa_screen = uxa_get_screen(screen);
	PicturePtr picture;

	if (!uxa_screen->solid_clear) {
		uxa_screen->solid_clear = uxa_create_solid(screen, 0);
		if (!uxa_screen->solid_clear)
			return 0;
	}
	picture = uxa_screen->solid_clear;
	return picture;
}

PicturePtr
uxa_acquire_solid(ScreenPtr screen, SourcePict *source)
{
	uxa_screen_t *uxa_screen = uxa_get_screen(screen);
	PictSolidFill *solid = &source->solidFill;
	PicturePtr picture;
	int i;

	if ((solid->color >> 24) == 0) {
		picture = uxa_solid_clear(screen);
		if (!picture)
		    return 0;

		goto DONE;
	} else if (solid->color == 0xff000000) {
		if (!uxa_screen->solid_black) {
			uxa_screen->solid_black = uxa_create_solid(screen, 0xff000000);
			if (!uxa_screen->solid_black)
				return 0;
		}
		picture = uxa_screen->solid_black;
		goto DONE;
	} else if (solid->color == 0xffffffff) {
		if (!uxa_screen->solid_white) {
			uxa_screen->solid_white = uxa_create_solid(screen, 0xffffffff);
			if (!uxa_screen->solid_white)
				return 0;
		}
		picture = uxa_screen->solid_white;
		goto DONE;
	}

	for (i = 0; i < uxa_screen->solid_cache_size; i++) {
		if (uxa_screen->solid_cache[i].color == solid->color) {
			picture = uxa_screen->solid_cache[i].picture;
			goto DONE;
		}
	}

	picture = uxa_create_solid(screen, solid->color);
	if (!picture)
		return 0;

	if (uxa_screen->solid_cache_size == UXA_NUM_SOLID_CACHE) {
		i = rand() % UXA_NUM_SOLID_CACHE;
		FreePicture(uxa_screen->solid_cache[i].picture, 0);
	} else
		uxa_screen->solid_cache_size++;

	uxa_screen->solid_cache[i].picture = picture;
	uxa_screen->solid_cache[i].color = solid->color;

DONE:
	picture->refcnt++;
	return picture;
}

PicturePtr
uxa_acquire_pattern(ScreenPtr pScreen,
		    PicturePtr pSrc,
		    pixman_format_code_t format,
		    INT16 x, INT16 y, CARD16 width, CARD16 height)
{
	PicturePtr pDst;

	if (pSrc->pSourcePict) {
		SourcePict *source = pSrc->pSourcePict;
		if (source->type == SourcePictTypeSolidFill)
			return uxa_acquire_solid (pScreen, source);
	}

	pDst = uxa_picture_for_pixman_format(pScreen, format, width, height);
	if (!pDst)
		return 0;

	if (uxa_picture_prepare_access(pDst, UXA_ACCESS_RW)) {
		fbComposite(PictOpSrc, pSrc, NULL, pDst,
			    x, y, 0, 0, 0, 0, width, height);
		uxa_picture_finish_access(pDst, UXA_ACCESS_RW);
		return pDst;
	} else {
		FreePicture(pDst, 0);
		return 0;
	}
}

static Bool
transform_is_integer_translation(PictTransformPtr t, int *tx, int *ty)
{
	if (t == NULL) {
		*tx = *ty = 0;
		return TRUE;
	}

	if (t->matrix[0][0] != IntToxFixed(1) ||
	    t->matrix[0][1] != 0 ||
	    t->matrix[1][0] != 0 ||
	    t->matrix[1][1] != IntToxFixed(1) ||
	    t->matrix[2][0] != 0 ||
	    t->matrix[2][1] != 0 ||
	    t->matrix[2][2] != IntToxFixed(1))
		return FALSE;

	if (xFixedFrac(t->matrix[0][2]) != 0 ||
	    xFixedFrac(t->matrix[1][2]) != 0)
		return FALSE;

	*tx = xFixedToInt(t->matrix[0][2]);
	*ty = xFixedToInt(t->matrix[1][2]);
	return TRUE;
}

static PicturePtr
uxa_render_picture(ScreenPtr screen,
		   PicturePtr src,
		   pixman_format_code_t format,
		   INT16 x, INT16 y,
		   CARD16 width, CARD16 height)
{
	PicturePtr picture;
	int ret = 0;

	/* XXX we need a mechanism for the card to choose the fallback format */

	/* force alpha channel in case source does not entirely cover the extents */
	if (PIXMAN_FORMAT_A(format) == 0)
		format = PIXMAN_a8r8g8b8; /* available on all hardware */

	picture = uxa_picture_for_pixman_format(screen, format, width, height);
	if (!picture)
		return 0;

	if (uxa_picture_prepare_access(picture, UXA_ACCESS_RW)) {
		if (uxa_picture_prepare_access(src, UXA_ACCESS_RO)) {
			ret = 1;
			fbComposite(PictOpSrc, src, NULL, picture,
				    x, y, 0, 0, 0, 0, width, height);
			uxa_picture_finish_access(src, UXA_ACCESS_RO);
		}
		uxa_picture_finish_access(picture, UXA_ACCESS_RW);
	}

	if (!ret) {
		FreePicture(picture, 0);
		return 0;
	}

	return picture;
}

static int
drawable_contains (DrawablePtr drawable, int x, int y, int w, int h)
{
	if (x < 0 || y < 0)
		return FALSE;

	if (x + w > drawable->width)
		return FALSE;

	if (y + h > drawable->height)
		return FALSE;

	return TRUE;
}

PicturePtr
uxa_acquire_drawable(ScreenPtr pScreen,
		     PicturePtr pSrc,
		     INT16 x, INT16 y,
		     CARD16 width, CARD16 height,
		     INT16 * out_x, INT16 * out_y)
{
	PixmapPtr pPixmap;
	PicturePtr pDst;
	int depth, error;
	int tx, ty;
	GCPtr pGC;

	depth = pSrc->pDrawable->depth;
	if (!transform_is_integer_translation(pSrc->transform, &tx, &ty) ||
	    !drawable_contains(pSrc->pDrawable, x + tx, y + ty, width, height) ||
	    depth == 1 ||
	    pSrc->filter == PictFilterConvolution) {
		/* XXX extract the sample extents and do the transformation on the GPU */
		pDst = uxa_render_picture(pScreen, pSrc,
					  pSrc->format | (BitsPerPixel(pSrc->pDrawable->depth) << 24),
					  x, y, width, height);
		if (!pDst)
			return 0;

		goto done;
	} else {
		if (width == pSrc->pDrawable->width && height == pSrc->pDrawable->height) {
			*out_x = x + pSrc->pDrawable->x;
			*out_y = y + pSrc->pDrawable->y;
			return pSrc;
		}
	}

	pPixmap = pScreen->CreatePixmap(pScreen,
					width, height, depth,
					CREATE_PIXMAP_USAGE_SCRATCH);
	if (!pPixmap)
		return 0;

	/* Skip the copy if the result remains in memory and not a bo */
	if (!uxa_pixmap_is_offscreen(pPixmap)) {
		pScreen->DestroyPixmap(pPixmap);
		return 0;
	}

	pGC = GetScratchGC(depth, pScreen);
	if (!pGC) {
		pScreen->DestroyPixmap(pPixmap);
		return 0;
	}

	ValidateGC(&pPixmap->drawable, pGC);
	pGC->ops->CopyArea(pSrc->pDrawable, &pPixmap->drawable, pGC,
			   x + tx, y + ty, width, height, 0, 0);
	FreeScratchGC(pGC);

	pDst = CreatePicture(0, &pPixmap->drawable,
			     PictureMatchFormat(pScreen, depth, pSrc->format),
			     0, 0, serverClient, &error);
	pScreen->DestroyPixmap(pPixmap);
	if (!pDst)
		return 0;

	ValidatePicture(pDst);
done:
	pDst->componentAlpha = pSrc->componentAlpha;
	*out_x = 0;
	*out_y = 0;
	return pDst;
}

static PicturePtr
uxa_acquire_picture(ScreenPtr screen,
		    PicturePtr src,
		    pixman_format_code_t format,
		    INT16 x, INT16 y,
		    CARD16 width, CARD16 height,
		    INT16 * out_x, INT16 * out_y)
{
	uxa_screen_t *uxa_screen = uxa_get_screen(screen);

	if (uxa_screen->info->check_composite_texture &&
	    uxa_screen->info->check_composite_texture(screen, src)) {
		if (src->pDrawable) {
			*out_x = x + src->pDrawable->x;
			*out_y = y + src->pDrawable->y;
		} else {
			*out_x = x;
			*out_y = y;
		}
		return src;
	}

	if (src->pDrawable) {
		PicturePtr dst;

		dst = uxa_acquire_drawable(screen, src,
					   x, y, width, height,
					   out_x, out_y);
		if (!dst)
			return 0;

		if (uxa_screen->info->check_composite_texture &&
		    !uxa_screen->info->check_composite_texture(screen, dst)) {
			if (dst != src)
				FreePicture(dst, 0);
			return 0;
		}

		return dst;
	}

	*out_x = 0;
	*out_y = 0;
	return uxa_acquire_pattern(screen, src,
				   format, x, y, width, height);
}

static PicturePtr
uxa_acquire_source(ScreenPtr screen,
		   PicturePtr pict,
		   INT16 x, INT16 y,
		   CARD16 width, CARD16 height,
		   INT16 * out_x, INT16 * out_y)
{
	return uxa_acquire_picture (screen, pict,
				    PICT_a8r8g8b8,
				    x, y,
				    width, height,
				    out_x, out_y);
}

static PicturePtr
uxa_acquire_mask(ScreenPtr screen,
		 PicturePtr pict,
		 INT16 x, INT16 y,
		 INT16 width, INT16 height,
		 INT16 * out_x, INT16 * out_y)
{
	return uxa_acquire_picture (screen, pict,
				    PICT_a8,
				    x, y,
				    width, height,
				    out_x, out_y);
}

static int
uxa_try_driver_composite(CARD8 op,
			 PicturePtr pSrc,
			 PicturePtr pMask,
			 PicturePtr pDst,
			 INT16 xSrc, INT16 ySrc,
			 INT16 xMask, INT16 yMask,
			 INT16 xDst, INT16 yDst,
			 CARD16 width, CARD16 height)
{
	ScreenPtr screen = pDst->pDrawable->pScreen;
	uxa_screen_t *uxa_screen = uxa_get_screen(screen);
	RegionRec region;
	BoxPtr pbox;
	int nbox;
	int xDst_copy = 0, yDst_copy = 0;
	int src_off_x, src_off_y, mask_off_x, mask_off_y, dst_off_x, dst_off_y;
	PixmapPtr pSrcPix, pMaskPix = NULL, pDstPix;
	PicturePtr localSrc, localMask = NULL;
	PicturePtr localDst = pDst;

	if (uxa_screen->info->check_composite &&
	    !(*uxa_screen->info->check_composite) (op, pSrc, pMask, pDst, width, height))
		return -1;

	if (uxa_screen->info->check_composite_target &&
	    !uxa_screen->info->check_composite_target(uxa_get_drawable_pixmap(pDst->pDrawable))) {
		int depth = pDst->pDrawable->depth;
		PixmapPtr pixmap;
		int error;
		GCPtr gc;

		pixmap = uxa_get_drawable_pixmap(pDst->pDrawable);
		if (uxa_screen->info->check_copy &&
		    !uxa_screen->info->check_copy(pixmap, pixmap, GXcopy, FB_ALLONES))
			return -1;

		pixmap = screen->CreatePixmap(screen,
					      width, height, depth,
					      CREATE_PIXMAP_USAGE_SCRATCH);
		if (!pixmap)
			return 0;

		gc = GetScratchGC(depth, screen);
		if (!gc) {
			screen->DestroyPixmap(pixmap);
			return 0;
		}

		ValidateGC(&pixmap->drawable, gc);
		gc->ops->CopyArea(pDst->pDrawable, &pixmap->drawable, gc,
				  xDst, yDst, width, height, 0, 0);
		FreeScratchGC(gc);

		xDst_copy = xDst; xDst = 0;
		yDst_copy = yDst; yDst = 0;

		localDst = CreatePicture(0, &pixmap->drawable,
					 PictureMatchFormat(screen, depth, pDst->format),
					 0, 0, serverClient, &error);
		screen->DestroyPixmap(pixmap);

		if (!localDst)
			return 0;

		ValidatePicture(localDst);
	}

	pDstPix =
	    uxa_get_offscreen_pixmap(localDst->pDrawable, &dst_off_x, &dst_off_y);
	if (!pDstPix) {
		if (localDst != pDst)
			FreePicture(localDst, 0);
		return -1;
	}

	xDst += localDst->pDrawable->x;
	yDst += localDst->pDrawable->y;

	localSrc = uxa_acquire_source(screen, pSrc,
				      xSrc, ySrc,
				      width, height,
				      &xSrc, &ySrc);
	if (!localSrc) {
		if (localDst != pDst)
			FreePicture(localDst, 0);
		return 0;
	}

	if (pMask) {
		localMask = uxa_acquire_mask(screen, pMask,
					     xMask, yMask,
					     width, height,
					     &xMask, &yMask);
		if (!localMask) {
			if (localSrc != pSrc)
				FreePicture(localSrc, 0);
			if (localDst != pDst)
				FreePicture(localDst, 0);

			return 0;
		}
	}

	if (!miComputeCompositeRegion(&region, localSrc, localMask, localDst,
				      xSrc, ySrc, xMask, yMask, xDst, yDst,
				      width, height)) {
		if (localSrc != pSrc)
			FreePicture(localSrc, 0);
		if (localMask && localMask != pMask)
			FreePicture(localMask, 0);
		if (localDst != pDst)
			FreePicture(localDst, 0);

		return 1;
	}

	pSrcPix = uxa_get_offscreen_pixmap(localSrc->pDrawable,
					   &src_off_x, &src_off_y);
	if (!pSrcPix) {
		REGION_UNINIT(screen, &region);

		if (localSrc != pSrc)
			FreePicture(localSrc, 0);
		if (localMask && localMask != pMask)
			FreePicture(localMask, 0);
		if (localDst != pDst)
			FreePicture(localDst, 0);

		return 0;
	}

	if (localMask) {
		pMaskPix = uxa_get_offscreen_pixmap(localMask->pDrawable,
						    &mask_off_x, &mask_off_y);
		if (!pMaskPix) {
			REGION_UNINIT(screen, &region);

			if (localSrc != pSrc)
				FreePicture(localSrc, 0);
			if (localMask && localMask != pMask)
				FreePicture(localMask, 0);
			if (localDst != pDst)
				FreePicture(localDst, 0);

			return 0;
		}
	}

	if (!(*uxa_screen->info->prepare_composite)
	    (op, localSrc, localMask, localDst, pSrcPix, pMaskPix, pDstPix)) {
		REGION_UNINIT(screen, &region);

		if (localSrc != pSrc)
			FreePicture(localSrc, 0);
		if (localMask && localMask != pMask)
			FreePicture(localMask, 0);
		if (localDst != pDst)
			FreePicture(localDst, 0);

		return -1;
	}

	if (pMask) {
		xMask = xMask + mask_off_x - xDst;
		yMask = yMask + mask_off_y - yDst;
	}

	xSrc = xSrc + src_off_x - xDst;
	ySrc = ySrc + src_off_y - yDst;

	nbox = REGION_NUM_RECTS(&region);
	pbox = REGION_RECTS(&region);
	while (nbox--) {
		(*uxa_screen->info->composite) (pDstPix,
						pbox->x1 + xSrc,
						pbox->y1 + ySrc,
						pbox->x1 + xMask,
						pbox->y1 + yMask,
						pbox->x1 + dst_off_x,
						pbox->y1 + dst_off_y,
						pbox->x2 - pbox->x1,
						pbox->y2 - pbox->y1);
		pbox++;
	}
	(*uxa_screen->info->done_composite) (pDstPix);

	REGION_UNINIT(screen, &region);

	if (localSrc != pSrc)
		FreePicture(localSrc, 0);
	if (localMask && localMask != pMask)
		FreePicture(localMask, 0);

	if (localDst != pDst) {
		GCPtr gc;

		gc = GetScratchGC(pDst->pDrawable->depth, screen);
		if (gc) {
			ValidateGC(pDst->pDrawable, gc);
			gc->ops->CopyArea(localDst->pDrawable, pDst->pDrawable, gc,
					  0, 0, width, height, xDst_copy, yDst_copy);
			FreeScratchGC(gc);
		}

		FreePicture(localDst, 0);
	}

	return 1;
}

/**
 * uxa_try_magic_two_pass_composite_helper implements PictOpOver using two passes of
 * simpler operations PictOpOutReverse and PictOpAdd. Mainly used for component
 * alpha and limited 1-tmu cards.
 *
 * From http://anholt.livejournal.com/32058.html:
 *
 * The trouble is that component-alpha rendering requires two different sources
 * for blending: one for the source value to the blender, which is the
 * per-channel multiplication of source and mask, and one for the source alpha
 * for multiplying with the destination channels, which is the multiplication
 * of the source channels by the mask alpha. So the equation for Over is:
 *
 * dst.A = src.A * mask.A + (1 - (src.A * mask.A)) * dst.A
 * dst.R = src.R * mask.R + (1 - (src.A * mask.R)) * dst.R
 * dst.G = src.G * mask.G + (1 - (src.A * mask.G)) * dst.G
 * dst.B = src.B * mask.B + (1 - (src.A * mask.B)) * dst.B
 *
 * But we can do some simpler operations, right? How about PictOpOutReverse,
 * which has a source factor of 0 and dest factor of (1 - source alpha). We
 * can get the source alpha value (srca.X = src.A * mask.X) out of the texture
 * blenders pretty easily. So we can do a component-alpha OutReverse, which
 * gets us:
 *
 * dst.A = 0 + (1 - (src.A * mask.A)) * dst.A
 * dst.R = 0 + (1 - (src.A * mask.R)) * dst.R
 * dst.G = 0 + (1 - (src.A * mask.G)) * dst.G
 * dst.B = 0 + (1 - (src.A * mask.B)) * dst.B
 *
 * OK. And if an op doesn't use the source alpha value for the destination
 * factor, then we can do the channel multiplication in the texture blenders
 * to get the source value, and ignore the source alpha that we wouldn't use.
 * We've supported this in the Radeon driver for a long time. An example would
 * be PictOpAdd, which does:
 *
 * dst.A = src.A * mask.A + dst.A
 * dst.R = src.R * mask.R + dst.R
 * dst.G = src.G * mask.G + dst.G
 * dst.B = src.B * mask.B + dst.B
 *
 * Hey, this looks good! If we do a PictOpOutReverse and then a PictOpAdd right
 * after it, we get:
 *
 * dst.A = src.A * mask.A + ((1 - (src.A * mask.A)) * dst.A)
 * dst.R = src.R * mask.R + ((1 - (src.A * mask.R)) * dst.R)
 * dst.G = src.G * mask.G + ((1 - (src.A * mask.G)) * dst.G)
 * dst.B = src.B * mask.B + ((1 - (src.A * mask.B)) * dst.B)
 */

static int
uxa_try_magic_two_pass_composite_helper(CARD8 op,
					PicturePtr pSrc,
					PicturePtr pMask,
					PicturePtr pDst,
					INT16 xSrc, INT16 ySrc,
					INT16 xMask, INT16 yMask,
					INT16 xDst, INT16 yDst,
					CARD16 width, CARD16 height)
{
	ScreenPtr screen = pDst->pDrawable->pScreen;
	uxa_screen_t *uxa_screen = uxa_get_screen(screen);
	PicturePtr localDst = pDst;
	int xDst_copy, yDst_copy;

	assert(op == PictOpOver);

	if (uxa_screen->info->check_composite &&
	    (!(*uxa_screen->info->check_composite) (PictOpOutReverse, pSrc,
						    pMask, pDst, width, height)
	     || !(*uxa_screen->info->check_composite) (PictOpAdd, pSrc, pMask,
						       pDst, width, height))) {
		return -1;
	}

	if (uxa_screen->info->check_composite_target &&
	    !uxa_screen->info->check_composite_target(uxa_get_drawable_pixmap(pDst->pDrawable))) {
		int depth = pDst->pDrawable->depth;
		PixmapPtr pixmap;
		int error;
		GCPtr gc;

		pixmap = uxa_get_drawable_pixmap(pDst->pDrawable);
		if (uxa_screen->info->check_copy &&
		    !uxa_screen->info->check_copy(pixmap, pixmap, GXcopy, FB_ALLONES))
			return -1;

		pixmap = screen->CreatePixmap(screen,
					      width, height, depth,
					      CREATE_PIXMAP_USAGE_SCRATCH);
		if (!pixmap)
			return 0;

		gc = GetScratchGC(depth, screen);
		if (!gc) {
			screen->DestroyPixmap(pixmap);
			return 0;
		}

		ValidateGC(&pixmap->drawable, gc);
		gc->ops->CopyArea(pDst->pDrawable, &pixmap->drawable, gc,
				  xDst, yDst, width, height, 0, 0);
		FreeScratchGC(gc);

		xDst_copy = xDst; xDst = 0;
		yDst_copy = yDst; yDst = 0;

		localDst = CreatePicture(0, &pixmap->drawable,
					 PictureMatchFormat(screen, depth, pDst->format),
					 0, 0, serverClient, &error);
		screen->DestroyPixmap(pixmap);

		if (!localDst)
			return 0;

		ValidatePicture(localDst);
	}

	/* Now, we think we should be able to accelerate this operation. First,
	 * composite the destination to be the destination times the source alpha
	 * factors.
	 */
	uxa_composite(PictOpOutReverse, pSrc, pMask, localDst,
		      xSrc, ySrc,
		      xMask, yMask,
		      xDst, yDst,
		      width, height);

	/* Then, add in the source value times the destination alpha factors (1.0).
	 */
	uxa_composite(PictOpAdd, pSrc, pMask, localDst,
		      xSrc, ySrc,
		      xMask, yMask,
		      xDst, yDst,
		      width, height);

	if (localDst != pDst) {
		GCPtr gc;

		gc = GetScratchGC(pDst->pDrawable->depth, screen);
		if (gc) {
			ValidateGC(pDst->pDrawable, gc);
			gc->ops->CopyArea(localDst->pDrawable, pDst->pDrawable, gc,
					0, 0, width, height, xDst_copy, yDst_copy);
			FreeScratchGC(gc);
		}

		FreePicture(localDst, 0);
	}

	return 1;
}

static int
compatible_formats (CARD8 op, PicturePtr dst, PicturePtr src)
{
	if (op == PictOpSrc) {
		if (src->format == dst->format)
			return 1;

		/* Is the destination an alpha-less version of source? */
		if (dst->format == PICT_FORMAT(PICT_FORMAT_BPP(src->format),
					       PICT_FORMAT_TYPE(src->format),
					       0,
					       PICT_FORMAT_R(src->format),
					       PICT_FORMAT_G(src->format),
					       PICT_FORMAT_B(src->format)))
			return 1;

		/* XXX xrgb is promoted to argb during image upload... */
#if 0
		if (dst->format == PICT_a8r8g8b8 && src->format == PICT_x8r8g8b8)
			return 1;
#endif
	} else if (op == PictOpOver) {
		if (PICT_FORMAT_A(src->format))
			return 0;

		return src->format == dst->format;
	}

	return 0;
}

void
uxa_composite(CARD8 op,
	      PicturePtr pSrc,
	      PicturePtr pMask,
	      PicturePtr pDst,
	      INT16 xSrc, INT16 ySrc,
	      INT16 xMask, INT16 yMask,
	      INT16 xDst, INT16 yDst,
	      CARD16 width, CARD16 height)
{
	uxa_screen_t *uxa_screen = uxa_get_screen(pDst->pDrawable->pScreen);
	int ret = -1;
	Bool saveSrcRepeat = pSrc->repeat;
	Bool saveMaskRepeat = pMask ? pMask->repeat : 0;
	RegionRec region;
	int tx, ty;

	if (uxa_screen->info->flags & UXA_USE_GLAMOR) {
		int ok;

		uxa_picture_prepare_access(pDst, UXA_GLAMOR_ACCESS_RW);
		uxa_picture_prepare_access(pSrc, UXA_GLAMOR_ACCESS_RO);
		if (pMask)
			uxa_picture_prepare_access(pMask, UXA_GLAMOR_ACCESS_RO);

		ok = glamor_composite_nf(op,
					 pSrc, pMask, pDst, xSrc, ySrc,
					 xMask, yMask, xDst, yDst,
					 width, height);

		if (pMask)
			uxa_picture_finish_access(pMask, UXA_GLAMOR_ACCESS_RO);
		uxa_picture_finish_access(pSrc, UXA_GLAMOR_ACCESS_RO);
		uxa_picture_finish_access(pDst, UXA_GLAMOR_ACCESS_RW);

		if (!ok)
			goto fallback;

		return;
	}

	if (uxa_screen->force_fallback)
		goto fallback;

	if (!uxa_drawable_is_offscreen(pDst->pDrawable))
		goto fallback;

	if (pDst->alphaMap || pSrc->alphaMap || (pMask && pMask->alphaMap))
		goto fallback;

	/* Remove repeat in source if useless */
	if (pSrc->pDrawable && pSrc->repeat && pSrc->filter != PictFilterConvolution &&
	    transform_is_integer_translation(pSrc->transform, &tx, &ty) &&
	    (pSrc->pDrawable->width > 1 || pSrc->pDrawable->height > 1) &&
	    drawable_contains(pSrc->pDrawable, xSrc + tx, ySrc + ty, width, height))
		pSrc->repeat = 0;

	if (!pMask) {
		if (op == PictOpClear) {
			PicturePtr clear = uxa_solid_clear(pDst->pDrawable->pScreen);
			if (clear &&
			    uxa_try_driver_solid_fill(clear, pDst,
						      xSrc, ySrc,
						      xDst, yDst,
						      width, height) == 1)
				goto done;
		}

		if (pSrc->pDrawable == NULL) {
			if (pSrc->pSourcePict) {
				SourcePict *source = pSrc->pSourcePict;
				if (source->type == SourcePictTypeSolidFill) {
					if (op == PictOpSrc ||
					    (op == PictOpOver &&
					     (source->solidFill.color & 0xff000000) == 0xff000000)) {
						ret = uxa_try_driver_solid_fill(pSrc, pDst,
										xSrc, ySrc,
										xDst, yDst,
										width, height);
						if (ret == 1)
							goto done;
					}
				}
			}
		} else if (pSrc->pDrawable->width == 1 &&
			   pSrc->pDrawable->height == 1 &&
			   pSrc->repeat &&
			   (op == PictOpSrc || (op == PictOpOver && !PICT_FORMAT_A(pSrc->format)))) {
			ret = uxa_try_driver_solid_fill(pSrc, pDst,
							xSrc, ySrc,
							xDst, yDst,
							width, height);
			if (ret == 1)
				goto done;
		} else if (compatible_formats (op, pDst, pSrc) &&
			   pSrc->filter != PictFilterConvolution &&
			   transform_is_integer_translation(pSrc->transform, &tx, &ty)) {
			if (!pSrc->repeat &&
			    drawable_contains(pSrc->pDrawable,
					     xSrc + tx, ySrc + ty,
					     width, height)) {
				xDst += pDst->pDrawable->x;
				yDst += pDst->pDrawable->y;
				xSrc += pSrc->pDrawable->x + tx;
				ySrc += pSrc->pDrawable->y + ty;

				if (!miComputeCompositeRegion
				    (&region, pSrc, pMask, pDst, xSrc, ySrc,
				     xMask, yMask, xDst, yDst, width, height))
					goto done;

				uxa_copy_n_to_n(pSrc->pDrawable,
						pDst->pDrawable, NULL,
						REGION_RECTS(&region),
						REGION_NUM_RECTS(&region),
						xSrc - xDst, ySrc - yDst, FALSE,
						FALSE, 0, NULL);
				REGION_UNINIT(pDst->pDrawable->pScreen,
					      &region);
				goto done;
			} else if (pSrc->repeat && pSrc->repeatType == RepeatNormal &&
				   pSrc->pDrawable->type == DRAWABLE_PIXMAP) {
				DDXPointRec patOrg;

				/* Let's see if the driver can do the repeat
				 * in one go
				 */
				if (uxa_screen->info->prepare_composite) {
					ret = uxa_try_driver_composite(op, pSrc,
								       pMask, pDst,
								       xSrc, ySrc,
								       xMask, yMask,
								       xDst, yDst,
								       width, height);
					if (ret == 1)
						goto done;
				}

				/* Now see if we can use
				 * uxa_fill_region_tiled()
				 */
				xDst += pDst->pDrawable->x;
				yDst += pDst->pDrawable->y;
				xSrc += pSrc->pDrawable->x + tx;
				ySrc += pSrc->pDrawable->y + ty;

				if (!miComputeCompositeRegion
				    (&region, pSrc, pMask, pDst, xSrc, ySrc,
				     xMask, yMask, xDst, yDst, width, height))
					goto done;

				/* pattern origin is the point in the
				 * destination drawable
				 * corresponding to (0,0) in the source */
				patOrg.x = xDst - xSrc;
				patOrg.y = yDst - ySrc;

				ret = uxa_fill_region_tiled(pDst->pDrawable,
							    &region,
							    (PixmapPtr) pSrc->
							    pDrawable, &patOrg,
							    FB_ALLONES, GXcopy);

				REGION_UNINIT(pDst->pDrawable->pScreen,
					      &region);

				if (ret)
					goto done;
			}
		}
	}

	/* Remove repeat in mask if useless */
	if (pMask && pMask->pDrawable && pMask->repeat &&
	    pMask->filter != PictFilterConvolution &&
	    transform_is_integer_translation(pMask->transform, &tx, &ty) &&
	    (pMask->pDrawable->width > 1 || pMask->pDrawable->height > 1) &&
	    drawable_contains(pMask->pDrawable, xMask + tx, yMask + ty, width, height))
		pMask->repeat = 0;

	if (uxa_screen->info->prepare_composite) {
		Bool isSrcSolid;

		ret =
		    uxa_try_driver_composite(op, pSrc, pMask, pDst, xSrc, ySrc,
					     xMask, yMask, xDst, yDst, width,
					     height);
		if (ret == 1)
			goto done;

		/* For generic masks and solid src pictures, mach64 can do
		 * Over in two passes, similar to the component-alpha case.
		 */

		isSrcSolid =
			pSrc->pDrawable ?
				pSrc->pDrawable->width == 1 &&
				pSrc->pDrawable->height == 1 &&
				pSrc->repeat :
			pSrc->pSourcePict ?
				pSrc->pSourcePict->type == SourcePictTypeSolidFill :
			0;

		/* If we couldn't do the Composite in a single pass, and it
		 * was a component-alpha Over, see if we can do it in two
		 * passes with an OutReverse and then an Add.
		 */
		if (ret == -1 && op == PictOpOver && pMask &&
		    (pMask->componentAlpha || isSrcSolid)) {
			ret =
			    uxa_try_magic_two_pass_composite_helper(op, pSrc,
								    pMask, pDst,
								    xSrc, ySrc,
								    xMask, yMask,
								    xDst, yDst,
								    width, height);
			if (ret == 1)
				goto done;
		}

	}

fallback:
	uxa_print_composite_fallback("uxa_composite",
				     op, pSrc, pMask, pDst);

	uxa_check_composite(op, pSrc, pMask, pDst, xSrc, ySrc,
			    xMask, yMask, xDst, yDst, width, height);

done:
	pSrc->repeat = saveSrcRepeat;
	if (pMask)
		pMask->repeat = saveMaskRepeat;
}
#endif

/**
 * Same as miCreateAlphaPicture, except it uses uxa_check_poly_fill_rect instead
 * of PolyFillRect to initialize the pixmap after creating it, to prevent
 * the pixmap from being migrated.
 *
 * See the comments about uxa_trapezoids and uxa_triangles.
 */
static PicturePtr
uxa_create_alpha_picture(ScreenPtr pScreen,
			 PicturePtr pDst,
			 PictFormatPtr pPictFormat, CARD16 width, CARD16 height)
{
	PixmapPtr pPixmap;
	PicturePtr pPicture;
	int error;

	if (width > 32767 || height > 32767)
		return 0;

	if (!pPictFormat) {
		if (pDst->polyEdge == PolyEdgeSharp)
			pPictFormat = PictureMatchFormat(pScreen, 1, PICT_a1);
		else
			pPictFormat = PictureMatchFormat(pScreen, 8, PICT_a8);
		if (!pPictFormat)
			return 0;
	}

	pPixmap = (*pScreen->CreatePixmap) (pScreen, width, height,
					    pPictFormat->depth,
					    UXA_CREATE_PIXMAP_FOR_MAP);
	if (!pPixmap)
		return 0;
	pPicture = CreatePicture(0, &pPixmap->drawable, pPictFormat,
				 0, 0, serverClient, &error);
	(*pScreen->DestroyPixmap) (pPixmap);
	return pPicture;
}

static void
uxa_check_trapezoids(CARD8 op, PicturePtr src, PicturePtr dst,
		     PictFormatPtr maskFormat, INT16 xSrc, INT16 ySrc,
		     int ntrap, xTrapezoid * traps)
{
	ScreenPtr screen = dst->pDrawable->pScreen;

	if (maskFormat) {
		PixmapPtr scratch = NULL;
		PicturePtr mask;
		INT16 xDst, yDst;
		INT16 xRel, yRel;
		BoxRec bounds;
		int width, height;
		pixman_image_t *image;
		pixman_format_code_t format;
		int error;

		xDst = traps[0].left.p1.x >> 16;
		yDst = traps[0].left.p1.y >> 16;

		miTrapezoidBounds (ntrap, traps, &bounds);
		if (bounds.y1 >= bounds.y2 || bounds.x1 >= bounds.x2)
			return;

		width  = bounds.x2 - bounds.x1;
		height = bounds.y2 - bounds.y1;

		format = maskFormat->format |
			(BitsPerPixel(maskFormat->depth) << 24);
		image =
		    pixman_image_create_bits(format, width, height, NULL, 0);
		if (!image)
			return;

		for (; ntrap; ntrap--, traps++)
			pixman_rasterize_trapezoid(image,
						   (pixman_trapezoid_t *) traps,
						   -bounds.x1, -bounds.y1);


		scratch = GetScratchPixmapHeader(screen, width, height,
						 PIXMAN_FORMAT_DEPTH(format),
						 PIXMAN_FORMAT_BPP(format),
						 pixman_image_get_stride(image),
						 pixman_image_get_data(image));
		if (!scratch) {
			pixman_image_unref(image);
			return;
		}

		mask = CreatePicture(0, &scratch->drawable,
				     PictureMatchFormat(screen,
							PIXMAN_FORMAT_DEPTH(format),
							format),
				     0, 0, serverClient, &error);
		if (!mask) {
			FreeScratchPixmapHeader(scratch);
			pixman_image_unref(image);
			return;
		}

		xRel = bounds.x1 + xSrc - xDst;
		yRel = bounds.y1 + ySrc - yDst;
		CompositePicture(op, src, mask, dst,
				 xRel, yRel,
				 0, 0,
				 bounds.x1, bounds.y1,
				 width, height);
		FreePicture(mask, 0);

		FreeScratchPixmapHeader(scratch);
		pixman_image_unref(image);
	} else {
		if (dst->polyEdge == PolyEdgeSharp)
			maskFormat = PictureMatchFormat(screen, 1, PICT_a1);
		else
			maskFormat = PictureMatchFormat(screen, 8, PICT_a8);

		for (; ntrap; ntrap--, traps++)
			uxa_check_trapezoids(op, src, dst, maskFormat, xSrc, ySrc, 1, traps);
	}
}

/**
 * uxa_trapezoids is essentially a copy of miTrapezoids that uses
 * uxa_create_alpha_picture instead of miCreateAlphaPicture.
 *
 * The problem with miCreateAlphaPicture is that it calls PolyFillRect
 * to initialize the contents after creating the pixmap, which
 * causes the pixmap to be moved in for acceleration. The subsequent
 * call to RasterizeTrapezoid won't be accelerated however, which
 * forces the pixmap to be moved out again.
 *
 * uxa_create_alpha_picture avoids this roundtrip by using
 * uxa_check_poly_fill_rect to initialize the contents.
 */
void
uxa_trapezoids(CARD8 op, PicturePtr src, PicturePtr dst,
	       PictFormatPtr maskFormat, INT16 xSrc, INT16 ySrc,
	       int ntrap, xTrapezoid * traps)
{
	ScreenPtr screen = dst->pDrawable->pScreen;
	uxa_screen_t *uxa_screen = uxa_get_screen(screen);
	BoxRec bounds;
	Bool direct;

	if (uxa_screen->info->flags & UXA_USE_GLAMOR) {
		int ok;

		uxa_picture_prepare_access(dst, UXA_GLAMOR_ACCESS_RW);
		uxa_picture_prepare_access(src, UXA_GLAMOR_ACCESS_RO);
		ok = glamor_trapezoids_nf(op,
					  src, dst, maskFormat, xSrc,
					  ySrc, ntrap, traps);
		uxa_picture_finish_access(src, UXA_GLAMOR_ACCESS_RO);
		uxa_picture_finish_access(dst, UXA_GLAMOR_ACCESS_RW);

		if (!ok)
			goto fallback;

		return;
	}

	if (uxa_screen->force_fallback) {
fallback:
		uxa_check_trapezoids(op, src, dst, maskFormat, xSrc, ySrc, ntrap, traps);
		return;
	}

	direct = op == PictOpAdd && miIsSolidAlpha(src);
	if (maskFormat || direct) {
		miTrapezoidBounds(ntrap, traps, &bounds);

		if (bounds.y1 >= bounds.y2 || bounds.x1 >= bounds.x2)
			return;
	}

	/*
	 * Check for solid alpha add
	 */
	if (direct) {
		DrawablePtr pDraw = dst->pDrawable;
		PixmapPtr pixmap = uxa_get_drawable_pixmap(pDraw);
		int xoff, yoff;

		uxa_get_drawable_deltas(pDraw, pixmap, &xoff, &yoff);

		xoff += pDraw->x;
		yoff += pDraw->y;

		if (uxa_prepare_access(pDraw, UXA_ACCESS_RW)) {
			PictureScreenPtr ps = GetPictureScreen(screen);

			for (; ntrap; ntrap--, traps++)
				(*ps->RasterizeTrapezoid) (dst, traps, 0, 0);
			uxa_finish_access(pDraw, UXA_ACCESS_RW);
		}
	} else if (maskFormat) {
		PixmapPtr scratch = NULL;
		PicturePtr mask;
		INT16 xDst, yDst;
		INT16 xRel, yRel;
		int width, height;
		pixman_image_t *image;
		pixman_format_code_t format;

		xDst = traps[0].left.p1.x >> 16;
		yDst = traps[0].left.p1.y >> 16;

		width  = bounds.x2 - bounds.x1;
		height = bounds.y2 - bounds.y1;

		format = maskFormat->format |
			(BitsPerPixel(maskFormat->depth) << 24);
		image =
		    pixman_image_create_bits(format, width, height, NULL, 0);
		if (!image)
			return;

		for (; ntrap; ntrap--, traps++)
			pixman_rasterize_trapezoid(image,
						   (pixman_trapezoid_t *) traps,
						   -bounds.x1, -bounds.y1);
		if (uxa_drawable_is_offscreen(dst->pDrawable)) {
			mask = uxa_picture_from_pixman_image(screen, image, format);
		} else {
			int error;

			scratch = GetScratchPixmapHeader(screen, width, height,
							PIXMAN_FORMAT_DEPTH(format),
							PIXMAN_FORMAT_BPP(format),
							pixman_image_get_stride(image),
							pixman_image_get_data(image));
			mask = CreatePicture(0, &scratch->drawable,
					     PictureMatchFormat(screen,
								PIXMAN_FORMAT_DEPTH(format),
								format),
					     0, 0, serverClient, &error);
		}
		if (!mask) {
			if (scratch)
				FreeScratchPixmapHeader(scratch);
			pixman_image_unref(image);
			return;
		}

		xRel = bounds.x1 + xSrc - xDst;
		yRel = bounds.y1 + ySrc - yDst;
		CompositePicture(op, src, mask, dst,
				 xRel, yRel,
				 0, 0,
				 bounds.x1, bounds.y1,
				 width, height);
		FreePicture(mask, 0);

		if (scratch)
			FreeScratchPixmapHeader(scratch);
		pixman_image_unref(image);
	} else {
		if (dst->polyEdge == PolyEdgeSharp)
			maskFormat = PictureMatchFormat(screen, 1, PICT_a1);
		else
			maskFormat = PictureMatchFormat(screen, 8, PICT_a8);
		for (; ntrap; ntrap--, traps++)
			uxa_trapezoids(op, src, dst, maskFormat, xSrc, ySrc,
				       1, traps);
	}
}

static void
uxa_check_triangles(CARD8 op, PicturePtr src, PicturePtr dst,
		    PictFormatPtr maskFormat, INT16 xSrc, INT16 ySrc,
		    int ntri, xTriangle *tri)
{
	ScreenPtr screen = dst->pDrawable->pScreen;

	if (maskFormat) {
		PixmapPtr scratch = NULL;
		PicturePtr mask;
		INT16 xDst, yDst;
		INT16 xRel, yRel;
		BoxRec bounds;
		int width, height;
		pixman_image_t *image;
		pixman_format_code_t format;
		int error;

		xDst = pixman_fixed_to_int(tri[0].p1.x);
		yDst = pixman_fixed_to_int(tri[0].p1.y);

		miTriangleBounds (ntri, tri, &bounds);
		if (bounds.y1 >= bounds.y2 || bounds.x1 >= bounds.x2)
			return;

		width  = bounds.x2 - bounds.x1;
		height = bounds.y2 - bounds.y1;

		format = maskFormat->format |
			(BitsPerPixel(maskFormat->depth) << 24);
		image =
		    pixman_image_create_bits(format, width, height, NULL, 0);
		if (!image)
			return;

		pixman_add_triangles(image,
				     -bounds.x1, -bounds.y1,
				     ntri, (pixman_triangle_t *)tri);

		scratch = GetScratchPixmapHeader(screen, width, height,
						 PIXMAN_FORMAT_DEPTH(format),
						 PIXMAN_FORMAT_BPP(format),
						 pixman_image_get_stride(image),
						 pixman_image_get_data(image));
		if (!scratch) {
			pixman_image_unref(image);
			return;
		}

		mask = CreatePicture(0, &scratch->drawable,
				     PictureMatchFormat(screen,
							PIXMAN_FORMAT_DEPTH(format),
							format),
				     0, 0, serverClient, &error);
		if (!mask) {
			FreeScratchPixmapHeader(scratch);
			pixman_image_unref(image);
			return;
		}

		xRel = bounds.x1 + xSrc - xDst;
		yRel = bounds.y1 + ySrc - yDst;
		CompositePicture(op, src, mask, dst,
				 xRel, yRel,
				 0, 0,
				 bounds.x1, bounds.y1,
				 width, height);
		FreePicture(mask, 0);

		FreeScratchPixmapHeader(scratch);
		pixman_image_unref(image);
	} else {
		if (dst->polyEdge == PolyEdgeSharp)
			maskFormat = PictureMatchFormat(screen, 1, PICT_a1);
		else
			maskFormat = PictureMatchFormat(screen, 8, PICT_a8);

		for (; ntri; ntri--, tri++)
			uxa_check_triangles(op, src, dst, maskFormat, xSrc, ySrc, 1, tri);
	}
}

/**
 * uxa_triangles is essentially a copy of miTriangles that uses
 * uxa_create_alpha_picture instead of miCreateAlphaPicture.
 *
 * The problem with miCreateAlphaPicture is that it calls PolyFillRect
 * to initialize the contents after creating the pixmap, which
 * causes the pixmap to be moved in for acceleration. The subsequent
 * call to AddTriangles won't be accelerated however, which forces the pixmap
 * to be moved out again.
 *
 * uxa_create_alpha_picture avoids this roundtrip by using
 * uxa_check_poly_fill_rect to initialize the contents.
 */
void
uxa_triangles(CARD8 op, PicturePtr pSrc, PicturePtr pDst,
	      PictFormatPtr maskFormat, INT16 xSrc, INT16 ySrc,
	      int ntri, xTriangle * tris)
{
	ScreenPtr pScreen = pDst->pDrawable->pScreen;
	uxa_screen_t *uxa_screen = uxa_get_screen(pScreen);
	PictureScreenPtr ps = GetPictureScreen(pScreen);
	BoxRec bounds;
	Bool direct;

	if (uxa_screen->info->flags & UXA_USE_GLAMOR) {
		int ok;

		uxa_picture_prepare_access(pDst, UXA_GLAMOR_ACCESS_RW);
		uxa_picture_prepare_access(pSrc, UXA_GLAMOR_ACCESS_RO);
		ok = glamor_triangles_nf(op,
				        pSrc, pDst, maskFormat, xSrc,
					ySrc, ntri, tris);
		uxa_picture_finish_access(pSrc, UXA_GLAMOR_ACCESS_RO);
		uxa_picture_finish_access(pDst, UXA_GLAMOR_ACCESS_RW);

		if (!ok)
			goto fallback;

		return;
	}

	if (uxa_screen->force_fallback) {
fallback:
		uxa_check_triangles(op, pSrc, pDst, maskFormat,
				    xSrc, ySrc, ntri, tris);
		return;
	}

	direct = op == PictOpAdd && miIsSolidAlpha(pSrc);
	if (maskFormat || direct) {
		miTriangleBounds(ntri, tris, &bounds);

		if (bounds.y1 >= bounds.y2 || bounds.x1 >= bounds.x2)
			return;
	}

	/*
	 * Check for solid alpha add
	 */
	if (direct) {
		DrawablePtr pDraw = pDst->pDrawable;
		if (uxa_prepare_access(pDraw, UXA_ACCESS_RW)) {
			(*ps->AddTriangles) (pDst, 0, 0, ntri, tris);
			uxa_finish_access(pDraw, UXA_ACCESS_RW);
		}
	} else if (maskFormat) {
		PicturePtr pPicture;
		INT16 xDst, yDst;
		INT16 xRel, yRel;
		int width = bounds.x2 - bounds.x1;
		int height = bounds.y2 - bounds.y1;
		GCPtr pGC;
		xRectangle rect;

		xDst = tris[0].p1.x >> 16;
		yDst = tris[0].p1.y >> 16;

		pPicture = uxa_create_alpha_picture(pScreen, pDst, maskFormat,
						    width, height);
		if (!pPicture)
			return;

		/* Clear the alpha picture to 0. */
		pGC = GetScratchGC(pPicture->pDrawable->depth, pScreen);
		if (!pGC) {
			FreePicture(pPicture, 0);
			return;
		}
		ValidateGC(pPicture->pDrawable, pGC);
		rect.x = 0;
		rect.y = 0;
		rect.width = width;
		rect.height = height;
		uxa_check_poly_fill_rect(pPicture->pDrawable, pGC, 1, &rect);
		FreeScratchGC(pGC);

		if (uxa_prepare_access(pPicture->pDrawable, UXA_ACCESS_RW)) {
			(*ps->AddTriangles) (pPicture, -bounds.x1, -bounds.y1,
					     ntri, tris);
			uxa_finish_access(pPicture->pDrawable, UXA_ACCESS_RW);
		}

		xRel = bounds.x1 + xSrc - xDst;
		yRel = bounds.y1 + ySrc - yDst;
		CompositePicture(op, pSrc, pPicture, pDst,
				 xRel, yRel, 0, 0, bounds.x1, bounds.y1,
				 bounds.x2 - bounds.x1, bounds.y2 - bounds.y1);
		FreePicture(pPicture, 0);
	} else {
		if (pDst->polyEdge == PolyEdgeSharp)
			maskFormat = PictureMatchFormat(pScreen, 1, PICT_a1);
		else
			maskFormat = PictureMatchFormat(pScreen, 8, PICT_a8);

		for (; ntri; ntri--, tris++)
			uxa_triangles(op, pSrc, pDst, maskFormat, xSrc, ySrc, 1,
				      tris);
	}
}

void
uxa_add_traps(PicturePtr pPicture,
	      INT16 x_off, INT16 y_off, int ntrap, xTrap * traps)
{
	ScreenPtr pScreen = pPicture->pDrawable->pScreen;
	uxa_screen_t *uxa_screen = uxa_get_screen(pScreen);

	if (uxa_screen->info->flags & UXA_USE_GLAMOR) {
		int ok;

		uxa_picture_prepare_access(pPicture, UXA_GLAMOR_ACCESS_RW);
		ok = glamor_add_traps_nf(pPicture,
					 x_off, y_off, ntrap, traps);
		uxa_picture_finish_access(pPicture, UXA_GLAMOR_ACCESS_RW);

		if (!ok)
			goto fallback;

		return;
	}

fallback:
	uxa_check_add_traps(pPicture, x_off, y_off, ntrap, traps);
}
