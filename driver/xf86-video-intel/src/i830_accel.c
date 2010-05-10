/**************************************************************************

Copyright 1998-1999 Precision Insight, Inc., Cedar Park, Texas.
All Rights Reserved.

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sub license, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice (including the
next paragraph) shall be included in all copies or substantial portions
of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.
IN NO EVENT SHALL PRECISION INSIGHT AND/OR ITS SUPPLIERS BE LIABLE FOR
ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

**************************************************************************/

/*
 * Authors:
 *   Keith Whitwell <keith@tungstengraphics.com>
 *
 */

/*
 * XXX So far, for GXxor this is about 40% of the speed of SW, but CPU
 * utilisation falls from 95% to < 5%.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "xf86.h"
#include "i830.h"
#include "i810_reg.h"
#include "i830_ring.h"
#include "i915_drm.h"

unsigned long intel_get_pixmap_offset(PixmapPtr pixmap)
{
	ScreenPtr pScreen = pixmap->drawable.pScreen;
	ScrnInfoPtr scrn = xf86Screens[pScreen->myNum];
	intel_screen_private *intel = intel_get_screen_private(scrn);

	return (unsigned long)pixmap->devPrivate.ptr -
	    (unsigned long)intel->FbBase;
}

unsigned long intel_get_pixmap_pitch(PixmapPtr pixmap)
{
	return (unsigned long)pixmap->devKind;
}

int
I830WaitLpRing(ScrnInfoPtr scrn, int n, int timeout_millis)
{
   intel_screen_private *intel = intel_get_screen_private(scrn);
   I830RingBuffer *ring = &intel->ring;
   int iters = 0;
   unsigned int start = 0;
   unsigned int now = 0;
   int last_head = 0;
   unsigned int first = 0;

   /* If your system hasn't moved the head pointer in 2 seconds, I'm going to
    * call it crashed.
    */
   if (timeout_millis == 0)
      timeout_millis = 2000;

   if (I810_DEBUG & DEBUG_VERBOSE_ACCEL) {
      ErrorF("I830WaitLpRing %d\n", n);
      first = GetTimeInMillis();
   }

   while (ring->space < n) {
      ring->head = INREG(LP_RING + RING_HEAD) & I830_HEAD_MASK;
      ring->space = ring->head - (ring->tail + 8);

      if (ring->space < 0)
	 ring->space += ring->mem->size;

      iters++;
      now = GetTimeInMillis();
      if (start == 0 || now < start || ring->head != last_head) {
	 if (I810_DEBUG & DEBUG_VERBOSE_ACCEL)
	    if (now > start)
	       ErrorF("space: %d wanted %d\n", ring->space, n);
	 start = now;
	 last_head = ring->head;
      } else if (now - start > timeout_millis) {
	 ErrorF("Error in I830WaitLpRing(), timeout for %d seconds\n",
		timeout_millis/1000);
	 ErrorF("space: %d wanted %d\n", ring->space, n);
	 intel->uxa_driver = NULL;
	 FatalError("lockup\n");
      }

      DELAY(10);
   }

   if (I810_DEBUG & DEBUG_VERBOSE_ACCEL) {
      now = GetTimeInMillis();
      if (now - first) {
	 ErrorF("Elapsed %u ms\n", now - first);
	 ErrorF("space: %d wanted %d\n", ring->space, n);
      }
   }

   return iters;
}

void i830_debug_flush(ScrnInfoPtr scrn)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);

	if (intel->debug_flush & DEBUG_FLUSH_CACHES)
		intel_batch_emit_flush(scrn);

	if (intel->debug_flush & DEBUG_FLUSH_BATCHES)
		intel_batch_submit(scrn);
}

/* The following function sets up the supported acceleration. Call it
 * from the FbInit() function in the SVGA driver, or before ScreenInit
 * in a monolithic server.
 */
Bool I830AccelInit(ScreenPtr screen)
{
	ScrnInfoPtr scrn = xf86Screens[screen->myNum];
	intel_screen_private *intel = intel_get_screen_private(scrn);

	/* Limits are described in the BLT engine chapter under Graphics Data Size
	 * Limitations, and the descriptions of SURFACE_STATE, 3DSTATE_BUFFER_INFO,
	 * 3DSTATE_DRAWING_RECTANGLE, 3DSTATE_MAP_INFO, and 3DSTATE_MAP_INFO.
	 *
	 * i845 through i965 limits 2D rendering to 65536 lines and pitch of 32768.
	 *
	 * i965 limits 3D surface to (2*element size)-aligned offset if un-tiled.
	 * i965 limits 3D surface to 4kB-aligned offset if tiled.
	 * i965 limits 3D surfaces to w,h of ?,8192.
	 * i965 limits 3D surface to pitch of 1B - 128kB.
	 * i965 limits 3D surface pitch alignment to 1 or 2 times the element size.
	 * i965 limits 3D surface pitch alignment to 512B if tiled.
	 * i965 limits 3D destination drawing rect to w,h of 8192,8192.
	 *
	 * i915 limits 3D textures to 4B-aligned offset if un-tiled.
	 * i915 limits 3D textures to ~4kB-aligned offset if tiled.
	 * i915 limits 3D textures to width,height of 2048,2048.
	 * i915 limits 3D textures to pitch of 16B - 8kB, in dwords.
	 * i915 limits 3D destination to ~4kB-aligned offset if tiled.
	 * i915 limits 3D destination to pitch of 16B - 8kB, in dwords, if un-tiled.
	 * i915 limits 3D destination to pitch 64B-aligned if used with depth.
	 * i915 limits 3D destination to pitch of 512B - 8kB, in tiles, if tiled.
	 * i915 limits 3D destination to POT aligned pitch if tiled.
	 * i915 limits 3D destination drawing rect to w,h of 2048,2048.
	 *
	 * i845 limits 3D textures to 4B-aligned offset if un-tiled.
	 * i845 limits 3D textures to ~4kB-aligned offset if tiled.
	 * i845 limits 3D textures to width,height of 2048,2048.
	 * i845 limits 3D textures to pitch of 4B - 8kB, in dwords.
	 * i845 limits 3D destination to 4B-aligned offset if un-tiled.
	 * i845 limits 3D destination to ~4kB-aligned offset if tiled.
	 * i845 limits 3D destination to pitch of 8B - 8kB, in dwords.
	 * i845 limits 3D destination drawing rect to w,h of 2048,2048.
	 *
	 * For the tiled issues, the only tiled buffer we draw to should be
	 * the front, which will have an appropriate pitch/offset already set up,
	 * so UXA doesn't need to worry.
	 */
	if (IS_I965G(intel)) {
		intel->accel_pixmap_offset_alignment = 4 * 2;
		intel->accel_pixmap_pitch_alignment = 64;
		intel->accel_max_x = 8192;
		intel->accel_max_y = 8192;
	} else {
		intel->accel_pixmap_offset_alignment = 4;
		intel->accel_pixmap_pitch_alignment = 64;
		intel->accel_max_x = 2048;
		intel->accel_max_y = 2048;
	}

	return i830_uxa_init(screen);
}
