/* $XFree86: xc/programs/Xserver/hw/xfree86/drivers/mga/mga_dri.h,v 1.6 2001/04/10 16:08:01 dawes Exp $ */

/*
 * Copyright 2000 VA Linux Systems Inc., Fremont, California.
 * All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * VA LINUX SYSTEMS AND/OR ITS SUPPLIERS BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Authors:
 *   Keith Whitwell <keith@tungstengraphics.com>
 *   Gareth Hughes <gareth@valinux.com>
 */

#ifndef __MGA_DRI_H__
#define __MGA_DRI_H__

#include <X11/Xfuncproto.h>
#include "xf86drm.h"

#define MGA_DEFAULT_AGP_MODE     1
#define MGA_MAX_AGP_MODE         4

/* Buffer are aligned on 4096 byte boundaries.
 */
#define MGA_BUFFER_ALIGN	0x00000fff

typedef struct {
   unsigned int frontOffset;
   unsigned int frontPitch;

   unsigned int backOffset;
   unsigned int backPitch;

   unsigned int depthOffset;
   unsigned int depthPitch;

   unsigned int textureOffset;
   int textureSize;

   drmRegion agp;

   /* PCI mappings */
   drmRegion fb;
   drmRegion registers;
   drmRegion status;

   /* AGP mappings */
   drmRegion warp;
   drmRegion primary;
   drmRegion buffers;
   drmRegion agpTextures;

   drmBufMapPtr drmBuffers;

   int drm_version_major;
   int drm_version_minor;
} MGADRIServerPrivateRec, *MGADRIServerPrivatePtr;

/**
 * Hardware information sent from server to client-side DRI driver.
 *
 * \todo
 * Several of these fields are no longer used (and will never be used
 * again) on the client-side.  At some point when it is safe to do so
 * (probably for the X.org 6.9 / 7.0 release), these fields should be removed.
 */
#if 1
typedef struct _mgaDrmRegion {
    drm_handle_t     handle;
    unsigned int  offset;
    drmSize       size;
} mgaDrmRegion, *mgaDrmRegionPtr;
#else
#define mgaDrmRegion drmRegion
#endif

typedef struct {
   int chipset;
   int width;
   int height;
   int mem _X_DEPRECATED;           /**< Unused client-side since forever. */
   int cpp;

   int agpMode;

   unsigned int frontOffset;
   unsigned int frontPitch;

   unsigned int backOffset;
   unsigned int backPitch;

   unsigned int depthOffset;
   unsigned int depthPitch;

   unsigned int textureOffset;
   unsigned int textureSize;
   int logTextureGranularity;    /**< Unused client-side since 2003-Aug-06 */

   unsigned int agpBufferOffset _X_DEPRECATED; /**< Unused client-side since forever. */

   unsigned int agpTextureOffset;
   unsigned int agpTextureSize;
   int logAgpTextureGranularity; /**< Unused client-side since 2003-Aug-06 */

   unsigned int mAccess _X_DEPRECATED; /**< Unused client-side since forever. */

   /**
    * \name DRM memory regions.
    */
   /*@{*/
   mgaDrmRegion registers;            /**< MMIO registers. */
   mgaDrmRegion status;               /**< Unused client-side since forever. */
   mgaDrmRegion primary;              /**< Primary DMA region. */
   mgaDrmRegion buffers;              /**< Unused client-side since forever. */
   /*@}*/

   unsigned int sarea_priv_offset;
} MGADRIRec, *MGADRIPtr;

#endif
