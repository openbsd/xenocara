/*
 * Copyright © 2011 Intel Corporation.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including
 * the next paragraph) shall be included in all copies or substantial
 * portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT.  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 * Authors:
 *    Zhigang Gong <zhigang.gong@linux.intel.com>
 *
 */

#ifndef UXA_GLAMOR_H
#define UXA_GLAMOR_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef USE_GLAMOR
#include "glamor.h"
#else
#define glamor_fill_spans_nf(...)	FALSE
#define glamor_poly_fill_rect_nf(...)	FALSE
#define glamor_put_image_nf(...)	FALSE
#define glamor_copy_n_to_n_nf(...)	FALSE
#define glamor_get_spans_nf(...)	FALSE
#define glamor_set_spans_nf(...)	FALSE
#define glamor_get_image_nf(...)	FALSE
#define glamor_glyphs_nf(...)		FALSE
#define glamor_glyph_unrealize(...)	do { } while(0)
#define glamor_composite_nf(...)	FALSE
#define glamor_composite_rects_nf(...)	FALSE
#define glamor_trapezoids_nf(...)	FALSE
#define glamor_triangles_nf(...)	FALSE
#define glamor_add_traps_nf(...)	FALSE
#define glamor_create_gc(...)		FALSE
#define glamor_validate_gc(...)		do { } while(0)
#define glamor_poly_point_nf(...)	FALSE
#define glamor_poly_segment_nf(...)	FALSE
#define glamor_poly_lines_nf(...)	FALSE
#define glamor_push_pixels_nf(...)	FALSE
#define glamor_copy_plane_nf(...)	FALSE
#define glamor_image_glyph_blt_nf(...)	FALSE
#define glamor_poly_glyph_blt_nf(...)	FALSE
#endif

#endif /* UXA_GLAMOR_H */
