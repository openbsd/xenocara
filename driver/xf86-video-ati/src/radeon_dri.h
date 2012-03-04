/*
 * Copyright 2000 ATI Technologies Inc., Markham, Ontario,
 *                VA Linux Systems Inc., Fremont, California.
 *
 * All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation on the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial
 * portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NON-INFRINGEMENT.  IN NO EVENT SHALL ATI, VA LINUX SYSTEMS AND/OR
 * THEIR SUPPLIERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

/*
 * Authors:
 *   Kevin E. Martin <martin@xfree86.org>
 *   Rickard E. Faith <faith@valinux.com>
 *
 */

#ifndef _RADEON_DRI_
#define _RADEON_DRI_

#include "xf86drm.h"

/* DRI Driver defaults */
#define RADEON_DEFAULT_GART_SIZE      8 /* MB (must be 2^n and > 4MB) */
#define R300_DEFAULT_GART_SIZE        32 /* MB (for R300 and above) */
#define RADEON_DEFAULT_RING_SIZE      1 /* MB (must be page aligned) */
#define RADEON_DEFAULT_BUFFER_SIZE    2 /* MB (must be page aligned) */
#define RADEON_DEFAULT_GART_TEX_SIZE  1 /* MB (must be page aligned) */

#define RADEON_DEFAULT_CP_TIMEOUT     100000  /* usecs */

#define RADEON_DEFAULT_PCI_APER_SIZE 32 /* in MB */

#define RADEON_CARD_TYPE_RADEON       1

typedef struct {
    /* DRI screen private data */
    int           deviceID;	/* PCI device ID */
    int           width;	/* Width in pixels of display */
    int           height;	/* Height in scanlines of display */
    int           depth;	/* Depth of display (8, 15, 16, 24) */
    int           bpp;		/* Bit depth of display (8, 16, 24, 32) */

    int           IsPCI;	/* Current card is a PCI card */
    int           AGPMode;

    int           frontOffset;  /* Start of front buffer */
    int           frontPitch;
    int           backOffset;   /* Start of shared back buffer */
    int           backPitch;
    int           depthOffset;  /* Start of shared depth buffer */
    int           depthPitch;
    int           textureOffset;/* Start of texture data in frame buffer */
    int           textureSize;
    int           log2TexGran;

    /* MMIO register data */
    drm_handle_t     registerHandle;
    drmSize       registerSize;

    /* CP in-memory status information */
    drm_handle_t     statusHandle;
    drmSize       statusSize;

    /* CP GART Texture data */
    drm_handle_t     gartTexHandle;
    drmSize       gartTexMapSize;
    int           log2GARTTexGran;
    int           gartTexOffset;
    unsigned int  sarea_priv_offset;
} RADEONDRIRec, *RADEONDRIPtr;

#endif
