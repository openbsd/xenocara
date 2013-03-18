/*
 * Copyright © 2000, 2008 Keith Packard
 *             2004 Eric Anholt
 *             2005 Zack Rusin
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of copyright holders not be used in
 * advertising or publicity pertaining to distribution of the software without
 * specific, written prior permission. Copyright holders make no
 * representations about the suitability of this software for any purpose.  It
 * is provided "as is" without express or implied warranty.
 *
 * THE COPYRIGHT HOLDERS DISCLAIM ALL WARRANTIES WITH REGARD TO THIS
 * SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS, IN NO EVENT SHALL THE COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN
 * AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING
 * OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
 * SOFTWARE.
 */

/** @file
 * UXA - the unified memory acceleration architecture.
 *
 * This is the header containing the public API of UXA for uxa drivers.
 */

#ifndef UXA_H
#define UXA_H

#include "scrnintstr.h"
#include "pixmapstr.h"
#include "windowstr.h"
#include "gcstruct.h"
#include "picturestr.h"
#include "fb.h"

#define UXA_VERSION_MAJOR   1
#define UXA_VERSION_MINOR   0
#define UXA_VERSION_RELEASE 0

typedef enum {
	UXA_ACCESS_RO,
	UXA_ACCESS_RW,
	UXA_GLAMOR_ACCESS_RO,
	UXA_GLAMOR_ACCESS_RW
} uxa_access_t;

/**
 * The UxaDriver structure is allocated through uxa_driver_alloc(), and then
 * fllled in by drivers.
 */
typedef struct _UxaDriver {
	/**
	 * uxa_major and uxa_minor should be set by the driver to the version of
	 * UXA which the driver was compiled for (or configures itself at
	 * runtime to support).  This allows UXA to extend the structure for
	 * new features without breaking ABI for drivers compiled against
	 * older versions.
	 */
	int uxa_major, uxa_minor;

	/**
	 * The flags field is bitfield of boolean values controlling UXA's
	 * behavior.
	 *
	 * The flags include UXA_TWO_BITBLT_DIRECTIONS.
	 */
	int flags;

	/** @name solid
	 * @{
	 */
	/**
	 * check_solid() checks whether the driver can do a solid fill to this drawable.
	 * @param pDrawable Destination drawable
	 * @param alu raster operation
	 * @param planemask write mask for the fill
	 *
	 * The check_solid() call is recommended if prepare_solid() is
	 * implemented, but is not required.
	 */
	Bool(*check_solid) (DrawablePtr pDrawable, int alu, Pixel planemask);

	/**
	 * prepare_solid() sets up the driver for doing a solid fill.
	 * @param pPixmap Destination pixmap
	 * @param alu raster operation
	 * @param planemask write mask for the fill
	 * @param fg "foreground" color for the fill
	 *
	 * This call should set up the driver for doing a series of solid fills
	 * through the solid() call.  The alu raster op is one of the GX*
	 * graphics functions listed in X.h, and typically maps to a similar
	 * single-byte "ROP" setting in all hardware.  The planemask controls
	 * which bits of the destination should be affected, and will only
	 * represent the bits up to the depth of pPixmap.  The fg is the pixel
	 * value of the foreground color referred to in ROP descriptions.
	 *
	 * Note that many drivers will need to store some of the data in the
	 * driver private record, for sending to the hardware with each
	 * drawing command.
	 *
	 * The prepare_solid() call is required of all drivers, but it may fail
	 * for any reason.  Failure results in a fallback to software rendering.
	 */
	Bool(*prepare_solid) (PixmapPtr pPixmap,
			      int alu, Pixel planemask, Pixel fg);

	/**
	 * solid() performs a solid fill set up in the last prepare_solid()
	 * call.
	 *
	 * @param pPixmap destination pixmap
	 * @param x1 left coordinate
	 * @param y1 top coordinate
	 * @param x2 right coordinate
	 * @param y2 bottom coordinate
	 *
	 * Performs the fill set up by the last prepare_solid() call,
	 * covering the area from (x1,y1) to (x2,y2) in pPixmap.  Note that
	 * the coordinates are in the coordinate space of the destination
	 * pixmap, so the driver will need to set up the hardware's offset
	 * and pitch for the destination coordinates according to the pixmap's
	 * offset and pitch within framebuffer.
	 *
	 * This call is required if prepare_solid() ever succeeds.
	 */
	void (*solid) (PixmapPtr pPixmap, int x1, int y1, int x2, int y2);

	/**
	 * done_solid() finishes a set of solid fills.
	 *
	 * @param pPixmap destination pixmap.
	 *
	 * The done_solid() call is called at the end of a series of consecutive
	 * solid() calls following a successful prepare_solid().  This allows
	 * drivers to finish up emitting drawing commands that were buffered, or
	 * clean up state from prepare_solid().
	 *
	 * This call is required if prepare_solid() ever succeeds.
	 */
	void (*done_solid) (PixmapPtr pPixmap);
	/** @} */

	/** @name copy
	 * @{
	 */
	/**
	 * check_copy() checks whether the driver can blit between the two Pictures
	 */
	Bool(*check_copy) (PixmapPtr pSrc, PixmapPtr pDst, int alu, Pixel planemask);
	/**
	 * prepare_copy() sets up the driver for doing a copy within video
	 * memory.
	 -     *
	 * @param pSrcPixmap source pixmap
	 * @param pDstPixmap destination pixmap
	 * @param dx X copy direction
	 * @param dy Y copy direction
	 * @param alu raster operation
	 * @param planemask write mask for the fill
	 *
	 * This call should set up the driver for doing a series of copies
	 * from the pSrcPixmap to the pDstPixmap.  The dx flag will be
	 * positive if the
	 * hardware should do the copy from the left to the right, and dy will
	 * be positive if the copy should be done from the top to the bottom.
	 * This is to deal with self-overlapping copies when
	 * pSrcPixmap == pDstPixmap.
	 *
	 * If your hardware can only support blits that are (left to right,
	 * top to bottom) or (right to left, bottom to top), then you should
	 * set #UXA_TWO_BITBLT_DIRECTIONS, and UXA will break down copy
	 * operations to ones that meet those requirements.  The alu raster
	 * op is one of the GX* graphics functions listed in X.h, and
	 * typically maps to a similar single-byte "ROP" setting in all
	 * hardware.  The planemask controls which bits of the destination
	 * should be affected, and will only represent the bits up to the
	 * depth of pPixmap.
	 *
	 * Note that many drivers will need to store some of the data in the
	 * driver private record, for sending to the hardware with each
	 * drawing command.
	 *
	 * The prepare_copy() call is required of all drivers, but it may fail
	 * for any reason.  Failure results in a fallback to software rendering.
	 */
	Bool(*prepare_copy) (PixmapPtr pSrcPixmap,
			     PixmapPtr pDstPixmap,
			     int dx, int dy, int alu, Pixel planemask);

	/**
	 * copy() performs a copy set up in the last prepare_copy call.
	 *
	 * @param pDstPixmap destination pixmap
	 * @param srcX source X coordinate
	 * @param srcY source Y coordinate
	 * @param dstX destination X coordinate
	 * @param dstY destination Y coordinate
	 * @param width width of the rectangle to be copied
	 * @param height height of the rectangle to be copied.
	 *
	 * Performs the copy set up by the last prepare_copy() call, copying the
	 * rectangle from (srcX, srcY) to (srcX + width, srcY + width) in the
	 * source pixmap to the same-sized rectangle at (dstX, dstY) in the
	 * destination pixmap.  Those rectangles may overlap in memory, if
	 * pSrcPixmap == pDstPixmap.  Note that this call does not receive the
	 * pSrcPixmap as an argument -- if it's needed in this function, it
	 * should be stored in the driver private during prepare_copy().  As
	 * with solid(), the coordinates are in the coordinate space of each
	 * pixmap, so the driver will need to set up source and destination
	 * pitches and offsets from those pixmaps, probably using
	 * uxaGetPixmapOffset() and uxa_get_pixmap_pitch().
	 *
	 * This call is required if prepare_copy ever succeeds.
	 */
	void (*copy) (PixmapPtr pDstPixmap,
		      int srcX,
		      int srcY, int dstX, int dstY, int width, int height);

	/**
	 * done_copy() finishes a set of copies.
	 *
	 * @param pPixmap destination pixmap.
	 *
	 * The done_copy() call is called at the end of a series of consecutive
	 * copy() calls following a successful prepare_copy().  This allows
	 * drivers to finish up emitting drawing commands that were buffered,
	 * or clean up state from prepare_copy().
	 *
	 * This call is required if prepare_copy() ever succeeds.
	 */
	void (*done_copy) (PixmapPtr pDstPixmap);
	/** @} */

	/** @name composite
	 * @{
	 */
	/**
	 * check_composite() checks to see if a composite operation could be
	 * accelerated.
	 *
	 * @param op Render operation
	 * @param pSrcPicture source Picture
	 * @param pMaskPicture mask picture
	 * @param pDstPicture destination Picture
	 * @param width The width of the composite operation
	 * @param height The height of the composite operation
	 *
	 * The check_composite() call checks if the driver could handle
	 * acceleration of op with the given source, mask, and destination
	 * pictures.  This allows drivers to check source and destination
	 * formats, supported operations, transformations, and component
	 * alpha state, and send operations it can't support to software
	 * rendering early on.
	 *
	 * See prepare_composite() for more details on likely issues that
	 * drivers will have in accelerating composite operations.
	 *
	 * The check_composite() call is recommended if prepare_composite() is
	 * implemented, but is not required.
	 */
	Bool(*check_composite) (int op,
				PicturePtr pSrcPicture,
				PicturePtr pMaskPicture,
				PicturePtr pDstPicture,
				int width, int height);

	/**
	 * check_composite_target() checks to see if the destination of the composite
	 * operation can be used without midification.
	 *
	 * @param pixmap Destination Pixmap
	 *
	 * The check_composite_target() call is recommended if prepare_composite() is
	 * implemented, but is not required.
	 */
	Bool(*check_composite_target) (PixmapPtr pixmap);

	/**
	 * check_composite_texture() checks to see if a source to the composite
	 * operation can be used without midification.
	 *
	 * @param pScreen Screen
	 * @param pPicture Picture
	 *
	 * The check_composite_texture() call is recommended if prepare_composite() is
	 * implemented, but is not required.
	 */
	Bool(*check_composite_texture) (ScreenPtr pScreen,
					PicturePtr pPicture);

	/**
	 * prepare_composite() sets up the driver for doing a composite
	 * operation described in the Render extension protocol spec.
	 *
	 * @param op Render operation
	 * @param pSrcPicture source Picture
	 * @param pMaskPicture mask picture
	 * @param pDstPicture destination Picture
	 * @param pSrc source pixmap
	 * @param pMask mask pixmap
	 * @param pDst destination pixmap
	 *
	 * This call should set up the driver for doing a series of composite
	 * operations, as described in the Render protocol spec, with the given
	 * pSrcPicture, pMaskPicture, and pDstPicture.  The pSrc, pMask, and
	 * pDst are the pixmaps containing the pixel data, and should be used
	 * for setting the offset and pitch used for the coordinate spaces for
	 * each of the Pictures.
	 *
	 * Notes on interpreting Picture structures:
	 * - The Picture structures will always have a valid pDrawable.
	 * - The Picture structures will never have alphaMap set.
	 * - The mask Picture (and therefore pMask) may be NULL, in which case
	 *   the operation is simply src OP dst instead of src IN mask OP dst,
	 *   and mask coordinates should be ignored.
	 * - pMarkPicture may have componentAlpha set, which greatly changes
	 *   the behavior of the composite operation.  componentAlpha has no
	 *   effect when set on pSrcPicture or pDstPicture.
	 * - The source and mask Pictures may have a transformation set
	 *   (Picture->transform != NULL), which means that the source
	 *   coordinates should be transformed by that transformation,
	 *   resulting in scaling, rotation, etc.  The PictureTransformPoint()
	 *   call can transform coordinates for you.  Transforms have no
	 *   effect on Pictures when used as a destination.
	 * - The source and mask pictures may have a filter set.
	 *   PictFilterNearest and PictFilterBilinear are defined in the
	 *   Render protocol, but others may be encountered, and must be
	 *   handled correctly (usually by prepare_composite failing, and
	 *   falling back to software).  Filters have
	 *   no effect on Pictures when used as a destination.
	 * - The source and mask Pictures may have repeating set, which must be
	 *   respected.  Many chipsets will be unable to support repeating on
	 *   pixmaps that have a width or height that is not a power of two.
	 *
	 * If your hardware can't support source pictures (textures) with
	 * non-power-of-two pitches, you should set #UXA_OFFSCREEN_ALIGN_POT.
	 *
	 * Note that many drivers will need to store some of the data in the
	 * driver private record, for sending to the hardware with each
	 * drawing command.
	 *
	 * The prepare_composite() call is not required.  However, it is highly
	 * recommended for performance of antialiased font rendering and
	 * performance of cairo applications.  Failure results in a fallback
	 * to software rendering.
	 */
	Bool(*prepare_composite) (int op,
				  PicturePtr pSrcPicture,
				  PicturePtr pMaskPicture,
				  PicturePtr pDstPicture,
				  PixmapPtr pSrc,
				  PixmapPtr pMask, PixmapPtr pDst);

	/**
	 * composite() performs a composite operation set up in the last
	 * prepare_composite() call.
	 *
	 * @param pDstPixmap destination pixmap
	 * @param srcX source X coordinate
	 * @param srcY source Y coordinate
	 * @param maskX source X coordinate
	 * @param maskY source Y coordinate
	 * @param dstX destination X coordinate
	 * @param dstY destination Y coordinate
	 * @param width destination rectangle width
	 * @param height destination rectangle height
	 *
	 * Performs the composite operation set up by the last
	 * prepare_composite() call, to the rectangle from (dstX, dstY) to
	 * (dstX + width, dstY + height) in the destination Pixmap.  Note that
	 * if a transformation was set on the source or mask Pictures, the
	 * source rectangles may not be the same size as the destination
	 * rectangles and filtering.  Getting the coordinate transformation
	 * right at the subpixel level can be tricky, and rendercheck
	 * can test this for you.
	 *
	 * This call is required if prepare_composite() ever succeeds.
	 */
	void (*composite) (PixmapPtr pDst,
			   int srcX,
			   int srcY,
			   int maskX,
			   int maskY,
			   int dstX, int dstY, int width, int height);

	/**
	 * done_composite() finishes a set of composite operations.
	 *
	 * @param pPixmap destination pixmap.
	 *
	 * The done_composite() call is called at the end of a series of
	 * consecutive composite() calls following a successful
	 * prepare_composite().  This allows drivers to finish up emitting
	 * drawing commands that were buffered, or clean up state from
	 * prepare_composite().
	 *
	 * This call is required if prepare_composite() ever succeeds.
	 */
	void (*done_composite) (PixmapPtr pDst);
	/** @} */

	/**
	 * put_image() loads a rectangle of data from src into pDst.
	 *
	 * @param pDst destination pixmap
	 * @param x destination X coordinate.
	 * @param y destination Y coordinate
	 * @param width width of the rectangle to be copied
	 * @param height height of the rectangle to be copied
	 * @param src pointer to the beginning of the source data
	 * @param src_pitch pitch (in bytes) of the lines of source data.
	 *
	 * put_image() copies data in system memory beginning at src (with
	 * pitch src_pitch) into the destination pixmap from (x, y) to
	 * (x + width, y + height).  This is typically done with hostdata
	 * uploads, where the CPU sets up a blit command on the hardware with
	 * instructions that the blit data will be fed through some sort of
	 * aperture on the card.
	 *
	 * put_image() is most important for the performance of uxa_glyphs()
	 * (antialiased font drawing) by allowing pipelining of data uploads,
	 * avoiding a sync of the card after each glyph.
	 *
	 * @return TRUE if the driver successfully uploaded the data.  FALSE
	 * indicates that UXA should fall back to doing the upload in software.
	 *
	 * put_image() is not required, but is recommended if composite
	 * acceleration is supported.
	 */
	Bool(*put_image) (PixmapPtr pDst,
			  int x,
			  int y, int w, int h, char *src, int src_pitch);

	/**
	 * get_image() loads a rectangle of data from pSrc into dst
	 *
	 * @param pSrc source pixmap
	 * @param x source X coordinate.
	 * @param y source Y coordinate
	 * @param width width of the rectangle to be copied
	 * @param height height of the rectangle to be copied
	 * @param dst pointer to the beginning of the destination data
	 * @param dst_pitch pitch (in bytes) of the lines of destination data.
	 *
	 * get_image() copies data from offscreen memory in pSrc from
	 * (x, y) to (x + width, y + height), to system memory starting at
	 * dst (with pitch dst_pitch).  This would usually be done
	 * using scatter-gather DMA, supported by a DRM call, or by blitting
	 * to AGP and then synchronously reading from AGP.
	 *
	 * @return TRUE if the driver successfully downloaded the data.  FALSE
	 * indicates that UXA should fall back to doing the download in
	 * software.
	 *
	 * get_image() is not required, but is highly recommended.
	 */
	Bool(*get_image) (PixmapPtr pSrc,
			  int x, int y,
			  int w, int h, char *dst, int dst_pitch);

	/** @{ */
	/**
	 * prepare_access() is called before CPU access to an offscreen pixmap.
	 *
	 * @param pPix the pixmap being accessed
	 * @param index the index of the pixmap being accessed.
	 *
	 * prepare_access() will be called before CPU access to an offscreen
	 * pixmap.
	 *
	 * This can be used to set up hardware surfaces for byteswapping or
	 * untiling, or to adjust the pixmap's devPrivate.ptr for the purpose of
	 * making CPU access use a different aperture.
	 *
	 * The index is one of #UXA_PREPARE_DEST, #UXA_PREPARE_SRC, or
	 * #UXA_PREPARE_MASK, indicating which pixmap is in question.  Since
	 * only up to three pixmaps will have prepare_access() called on them
	 * per operation, drivers can have a small, statically-allocated space
	 * to maintain state for prepare_access() and finish_access() in.
	 * Note that the same pixmap may have prepare_access() called on it
	 * more than once, for uxample when doing a copy within the same
	 * pixmap (so it gets prepare_access as
	 * #UXA_PREPARE_DEST and then as #UXA_PREPARE_SRC).
	 *
	 * prepare_access() may fail.  An example might be the case of
	 * hardware that can set up 1 or 2 surfaces for CPU access, but not
	 * 3.  If prepare_access()
	 * fails, UXA will migrate the pixmap to system memory.
	 * get_image() must be implemented and must not fail if a driver
	 * wishes to fail in prepare_access().  prepare_access() must not
	 * fail when pPix is the visible screen, because the visible screen
	 * can not be migrated.
	 *
	 * @return TRUE if prepare_access() successfully prepared the pixmap
	 * for CPU drawing.
	 * @return FALSE if prepare_access() is unsuccessful and UXA should use
	 * get_image() to migate the pixmap out.
	 */
	Bool(*prepare_access) (PixmapPtr pPix, uxa_access_t access);

	/**
	 * finish_access() is called after CPU access to an offscreen pixmap.
	 *
	 * @param pPix the pixmap being accessed
	 * @param index the index of the pixmap being accessed.
	 *
	 * finish_access() will be called after finishing CPU access of an
	 * offscreen pixmap set up by prepare_access().  Note that the
	 * finish_access() will not be called if prepare_access() failed.
	 */
	void (*finish_access) (PixmapPtr pPix, uxa_access_t access);

	/**
	 * PixmapIsOffscreen() is an optional driver replacement to
	 * uxa_pixmap_is_offscreen(). Set to NULL if you want the standard
	 * behaviour of uxa_pixmap_is_offscreen().
	 *
	 * @param pPix the pixmap
	 * @return TRUE if the given drawable is in framebuffer memory.
	 *
	 * uxa_pixmap_is_offscreen() is used to determine if a pixmap is in
	 * offscreen memory, meaning that acceleration could probably be done
	 * to it, and that it will need to be wrapped by
	 * prepare_access()/finish_access() when accessing it with the CPU.
	 */
	Bool(*pixmap_is_offscreen) (PixmapPtr pPix);

	/** @} */
} uxa_driver_t;

/** @name UXA driver flags
 * @{
 */
/**
 * UXA_TWO_BITBLT_DIRECTIONS indicates to UXA that the driver can only
 * support copies that are (left-to-right, top-to-bottom) or
 * (right-to-left, bottom-to-top).
 */
#define UXA_TWO_BITBLT_DIRECTIONS	(1 << 2)

/**
 * UXA_USE_GLAMOR indicates to use glamor acceleration to perform rendering.
 * And if glamor fail to accelerate the rendering, then goto fallback to
 * use CPU to do the rendering. This flag will be set only when glamor get
 * initialized successfully.
 * Note, in ddx close screen, this bit need to be cleared.
 */
#define UXA_USE_GLAMOR			(1 << 3)

/* UXA_GLAMOR_EGL_INITIALIZED indicates glamor egl layer get initialized
 * successfully. UXA layer does not use this flag, before call to
 * glamor_init, ddx need to check this flag. */
#define UXA_GLAMOR_EGL_INITIALIZED	(1 << 4)

/** @} */
/** @name UXA CreatePixmap hint flags
 * @{
 */
/**
 * Flag to hint that the first operation on the pixmap will be a
 * prepare_access.
 */
#define UXA_CREATE_PIXMAP_FOR_MAP	0x20000000
/** @} */

uxa_driver_t *uxa_driver_alloc(void);

Bool uxa_driver_init(ScreenPtr screen, uxa_driver_t * uxa_driver);
Bool uxa_resources_init(ScreenPtr screen);

void uxa_driver_fini(ScreenPtr pScreen);

CARD32 uxa_get_pixmap_first_pixel(PixmapPtr pPixmap);

Bool
uxa_get_color_for_pixmap (PixmapPtr	 pixmap,
			  CARD32	 src_format,
			  CARD32	 dst_format,
			  CARD32	*pixel);

void uxa_set_fallback_debug(ScreenPtr screen, Bool enable);
void uxa_set_force_fallback(ScreenPtr screen, Bool enable);

/**
 * Returns TRUE if the given planemask covers all the significant bits in the
 * pixel values for pDrawable.
 */
#define UXA_PM_IS_SOLID(_pDrawable, _pm) \
	(((_pm) & FbFullMask((_pDrawable)->depth)) == \
	 FbFullMask((_pDrawable)->depth))

#endif /* UXA_H */
