/*
 * Copyright 2011 VMWare, Inc.
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
 */
#ifdef _HAVE_CONFIG_H_
#include "config.h"
#endif

#include <xorg-server.h>
#include "vmwgfx_saa_priv.h"


static const enum xa_surface_type vmwgfx_stype_map[] = {
  [PICT_TYPE_OTHER] = xa_type_other,
  [PICT_TYPE_A] = xa_type_a,
  [PICT_TYPE_ARGB] = xa_type_argb,
  [PICT_TYPE_ABGR] = xa_type_abgr,
  [PICT_TYPE_BGRA] = xa_type_bgra
};

static const unsigned int vmwgfx_stype_map_size =
    sizeof(vmwgfx_stype_map) / sizeof(enum xa_surface_type);

/**
 * vmwgfx_xa_surface_redefine - wrapper around xa_surface_redefine
 *
 * @vpix: Pointer to the struct vmwgfx_saa_pixmap the surface is attached to.
 * @srf: The surface.
 * @width: New width.
 * @height: New height.
 * @depth: New pixel depth.
 * @stype: New surface type.
 * @rgb_format: New rgb format.
 * @new_flags: New surface flags.
 * @copy_contents: Copy contents if new backing store is allocated.
 *
 * This is a wrapper that prints out an error message if the backing store
 * of an active scanout surface is changed.
 */
Bool
vmwgfx_xa_surface_redefine(struct vmwgfx_saa_pixmap *vpix,
			   struct xa_surface *srf,
			   int width,
			   int height,
			   int depth,
			   enum xa_surface_type stype,
			   enum xa_formats rgb_format,
			   unsigned int new_flags,
			   int copy_contents)
{
    uint32_t handle, new_handle, dummy;
    Bool have_handle = FALSE;

    if (!WSBMLISTEMPTY(&vpix->scanout_list))
	have_handle = (_xa_surface_handle(srf, &handle, &dummy) == XA_ERR_NONE);

    if (xa_surface_redefine(srf, width, height, depth, stype, rgb_format,
			    new_flags, copy_contents) != XA_ERR_NONE)
	return FALSE;

    if (!WSBMLISTEMPTY(&vpix->scanout_list) && have_handle &&
	_xa_surface_handle(srf, &new_handle, &dummy) == XA_ERR_NONE &&
	new_handle != handle) {
	LogMessage(X_ERROR, "Changed active scanout surface handle.\n");
    }

    return TRUE;
}


/*
 * Create an xa format from a PICT format.
 */
enum xa_formats
vmwgfx_xa_format(enum _PictFormatShort format)
{
    uint32_t ptype = PICT_FORMAT_TYPE(format);

    if (ptype >= vmwgfx_stype_map_size ||
	vmwgfx_stype_map[ptype] == 0 ||
	vmwgfx_stype_map[ptype] == xa_type_other)
	return xa_format_unknown;

    return xa_format(PICT_FORMAT_BPP(format),
		     vmwgfx_stype_map[ptype],
		     PICT_FORMAT_A(format),
		     PICT_FORMAT_R(format),
		     PICT_FORMAT_G(format),
		     PICT_FORMAT_B(format));
}

/*
 * Choose formats and flags for a dri2 surface.
 */
static Bool
vmwgfx_hw_dri2_stage(PixmapPtr pixmap, unsigned int depth)
{
    struct vmwgfx_saa_pixmap *vpix = vmwgfx_saa_pixmap(pixmap);
    enum xa_formats format;

    if (depth == 0)
	depth = pixmap->drawable.depth;

    switch(depth) {
    case 32:
	format = xa_format_a8r8g8b8;
	break;
    case 24:
        format = xa_format_x8r8g8b8;
	break;
    case 16:
	format = xa_format_r5g6b5;
	break;
    case 15:
	format = xa_format_x1r5g5b5;
	break;
    default:
	return FALSE;
    }

    vpix->staging_format = format;
    vpix->staging_remove_flags = 0;
    vpix->staging_add_flags = XA_FLAG_RENDER_TARGET | XA_FLAG_SHARED;

    return TRUE;
}

/*
 * Is composite old format compatible? Only difference is that old format
 * has more alpha bits?
 */
static inline Bool
vmwgfx_old_format_compatible(enum xa_formats format,
			     enum xa_formats old_format)
{
    return (format == old_format ||
	    (xa_format_type(format) == xa_format_type(old_format) &&
	     xa_format_a(format) <= xa_format_a(old_format) &&
	     xa_format_r(format) == xa_format_r(old_format) &&
	     xa_format_g(format) == xa_format_g(old_format) &&
	     xa_format_b(format) == xa_format_b(old_format)));
}


/*
 * Choose format and flags for a composite dst surface.
 */
Bool
vmwgfx_hw_composite_dst_stage(PixmapPtr pixmap,
			      enum _PictFormatShort pict_format)
{
    struct vmwgfx_saa *vsaa =
	to_vmwgfx_saa(saa_get_driver(pixmap->drawable.pScreen));
    struct vmwgfx_saa_pixmap *vpix = vmwgfx_saa_pixmap(pixmap);
    enum xa_formats format = vmwgfx_xa_format(pict_format);

    /*
     * Check if we can reuse old hardware format.
     */
    if (vpix->hw) {
	enum xa_formats old_format = xa_surface_format(vpix->hw);

	if (vmwgfx_old_format_compatible(format, old_format))
	    format = old_format;
    }

    if (xa_format_check_supported(vsaa->xat, format,
				  vpix->xa_flags | XA_FLAG_RENDER_TARGET) !=
	XA_ERR_NONE) {
	return FALSE;
    }

    vpix->staging_format = format;
    vpix->staging_remove_flags = 0;
    vpix->staging_add_flags = XA_FLAG_RENDER_TARGET | XA_FLAG_SHARED;

    return TRUE;
}

/*
 * Choose format and flags for a composite src surface.
 */
Bool
vmwgfx_hw_composite_src_stage(PixmapPtr pixmap,
			      enum _PictFormatShort pict_format)
{
    struct vmwgfx_saa *vsaa =
	to_vmwgfx_saa(saa_get_driver(pixmap->drawable.pScreen));
    struct vmwgfx_saa_pixmap *vpix = vmwgfx_saa_pixmap(pixmap);
    enum xa_formats format = vmwgfx_xa_format(pict_format);
    enum xa_formats swizzle_format = xa_format_unknown;
    enum xa_surface_type ftype;

    if (format == xa_format_unknown)
	return FALSE;

    ftype = xa_format_type(format);
    if (ftype == xa_type_abgr) {

	swizzle_format = xa_format(xa_format_bpp(format),
				   xa_type_argb,
				   xa_format_a(format),
				   xa_format_r(format),
				   xa_format_g(format),
				   xa_format_b(format));
    }

    /*
     * Check if we can reuse old format.
     */

    if (vpix->hw) {
	enum xa_formats old_format = xa_surface_format(vpix->hw);

	if (vmwgfx_old_format_compatible(format, old_format) ||
	    (swizzle_format != xa_format_unknown &&
	     vmwgfx_old_format_compatible(swizzle_format, old_format))) {
	    format = old_format;
	    goto have_format;
	}
    }

    if (swizzle_format != xa_format_unknown &&
	xa_format_check_supported(vsaa->xat, swizzle_format, vpix->xa_flags) ==
	XA_ERR_NONE) {
	format = swizzle_format;
	goto have_format;
    }

    if (xa_format_check_supported(vsaa->xat, format, vpix->xa_flags) ==
	XA_ERR_NONE) {
	goto have_format;
    }

    return FALSE;
  have_format:
    vpix->staging_format = format;
    vpix->staging_remove_flags = 0;
    vpix->staging_add_flags = 0;

    return TRUE;
}

/*
 * Choose accel format given depth.
 */
static enum xa_formats
vmwgfx_choose_accel_format(unsigned int depth)
{
    switch(depth) {
    case 32:
	return xa_format_a8r8g8b8;
    case 24:
	return xa_format_x8r8g8b8;
    case 16:
	return xa_format_r5g6b5;
    case 15:
	return xa_format_x1r5g5b5;
    case 8:
	return xa_format_a8;
    default:
	break;
    }
    return xa_format_unknown;
}


/*
 * Determine xa format and flags for an ordinary accel surface.
 */
Bool
vmwgfx_hw_accel_stage(PixmapPtr pixmap, unsigned int depth,
		      uint32_t add_flags, uint32_t remove_flags)
{
    struct vmwgfx_saa_pixmap *vpix = vmwgfx_saa_pixmap(pixmap);
    enum xa_formats format = xa_format_unknown;

    if (depth == 0)
	depth = pixmap->drawable.depth;

    if (vpix->hw) {
	enum xa_formats old_format = xa_surface_format(vpix->hw);
	enum xa_surface_type ftype = xa_format_type(old_format);

	if (ftype != xa_type_argb &&
	    ftype != xa_type_a) {
	    LogMessage(X_ERROR,
		       "Acceleration fallback due to strange hw format.\n");
	    return FALSE;
	}

	if (xa_format_depth(old_format) == depth ||
	    (xa_format_depth(old_format) == 32 &&
	     depth == 24))
	    format = old_format;
    }

    if (format == xa_format_unknown)
	format = vmwgfx_choose_accel_format(depth);

    if (format == xa_format_unknown)
	return FALSE;

    vpix->staging_add_flags = add_flags;
    vpix->staging_remove_flags = remove_flags;
    vpix->staging_format = format;

    return TRUE;
}

/*
 * Create a surface with a format and flags determined by one of
 * the staging functions.
 */
Bool
vmwgfx_hw_commit(PixmapPtr pixmap)
{
    struct vmwgfx_saa *vsaa =
	to_vmwgfx_saa(saa_get_driver(pixmap->drawable.pScreen));
    struct saa_pixmap *spix = saa_get_saa_pixmap(pixmap);
    struct vmwgfx_saa_pixmap *vpix = to_vmwgfx_saa_pixmap(spix);
    enum xa_formats format = vpix->staging_format;

    if (vpix->hw) {
	enum xa_formats old_format = xa_surface_format(vpix->hw);

	if (vpix->staging_format != old_format) {
	    if (xa_format_type(format) != xa_format_type(old_format) ||
		xa_format_r(format) != xa_format_r(old_format) ||
		xa_format_g(format) != xa_format_g(old_format) ||
		xa_format_b(format) != xa_format_b(old_format)) {

		LogMessage(X_INFO, "Killing old hw surface.\n");

		if (!vmwgfx_hw_kill(vsaa, spix))
		    return FALSE;
	    }
	}
    }

    if (vpix->hw) {
	uint32_t new_flags;

	new_flags = (vpix->xa_flags & ~vpix->staging_remove_flags) |
	    vpix->staging_add_flags | XA_FLAG_SHARED;

	if (vpix->staging_format != xa_surface_format(vpix->hw))
	    LogMessage(X_INFO, "Changing hardware format.\n");

	if (!vmwgfx_xa_surface_redefine(vpix,
					vpix->hw,
					pixmap->drawable.width,
					pixmap->drawable.height,
					0,
					xa_type_other,
					vpix->staging_format,
					new_flags, 1) != XA_ERR_NONE)
	    return FALSE;
	vpix->xa_flags = new_flags;
    } else if (!vmwgfx_create_hw(vsaa, pixmap))
	return FALSE;

    return TRUE;
}

/*
 * Create an accel surface if there is none, and make sure the region
 * given by @region is valid. If @region is NULL, the whole surface
 * will be valid. This is a utility convenience function only.
 */
Bool
vmwgfx_hw_accel_validate(PixmapPtr pixmap, unsigned int depth,
			 uint32_t add_flags, uint32_t remove_flags,
			 RegionPtr region)
{
    return (vmwgfx_hw_accel_stage(pixmap, depth, add_flags, remove_flags) &&
	    vmwgfx_hw_commit(pixmap) &&
	    vmwgfx_hw_validate(pixmap, region));
}


/*
 * Create a dri2 surface if there is none,
 * and make sure the whole surfade is valid.
 * This is a utility convenience function only.
 */
Bool
vmwgfx_hw_dri2_validate(PixmapPtr pixmap, unsigned int depth)
{
    struct vmwgfx_saa *vsaa =
	to_vmwgfx_saa(saa_get_driver(pixmap->drawable.pScreen));

    if (!vsaa->is_master)
	    return FALSE;

    return (vmwgfx_hw_dri2_stage(pixmap, depth) &&
	    vmwgfx_hw_commit(pixmap) &&
	    vmwgfx_hw_validate(pixmap, NULL));
}
