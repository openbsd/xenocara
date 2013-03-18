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

#ifndef INTEL_GLAMOR_H
#define INTEL_GLAMOR_H

#ifdef USE_GLAMOR

Bool intel_glamor_pre_init(ScrnInfoPtr scrn);
Bool intel_glamor_init(ScreenPtr screen);
Bool intel_glamor_create_screen_resources(ScreenPtr screen);
Bool intel_glamor_close_screen(ScreenPtr screen);
void intel_glamor_free_screen(int scrnIndex, int flags);

void intel_glamor_flush(intel_screen_private * intel);

Bool intel_glamor_create_textured_pixmap(PixmapPtr pixmap);
void intel_glamor_destroy_pixmap(PixmapPtr pixmap);
PixmapPtr intel_glamor_create_pixmap(ScreenPtr screen, int w, int h,
				     int depth, unsigned int usage);
void intel_glamor_exchange_buffers(struct intel_screen_private *intel, PixmapPtr src, PixmapPtr dst);
#else

static inline Bool intel_glamor_pre_init(ScrnInfoPtr scrn) { return TRUE; }
static inline Bool intel_glamor_init(ScreenPtr screen) { return TRUE; }
static inline Bool intel_glamor_create_screen_resources(ScreenPtr screen) { return TRUE; }
static inline Bool intel_glamor_close_screen(ScreenPtr screen) { return TRUE; }
static inline void intel_glamor_free_screen(int scrnIndex, int flags) { }

static inline void intel_glamor_flush(intel_screen_private * intel) { }

static inline Bool intel_glamor_create_textured_pixmap(PixmapPtr pixmap) { return TRUE; }
static inline void intel_glamor_destroy_pixmap(PixmapPtr pixmap) { }

static inline PixmapPtr intel_glamor_create_pixmap(ScreenPtr screen, int w, int h,
						   int depth, unsigned int usage) { return NULL; }

static inline void intel_glamor_exchange_buffers(struct intel_screen_private *intel, PixmapPtr src, PixmapPtr dst) {}
#endif

#endif /* INTEL_GLAMOR_H */
