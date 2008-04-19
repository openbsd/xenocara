/*
 * SiS DRI wrapper
 *
 * Copyright (C) 2001-2005 by Thomas Winischhofer, Vienna, Austria
 *
 * Licensed under the following terms:
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appears in all copies and that both that copyright
 * notice and this permission notice appear in supporting documentation, and
 * and that the name of the copyright holder not be used in advertising
 * or publicity pertaining to distribution of the software without specific,
 * written prior permission. The copyright holder makes no representations
 * about the suitability of this software for any purpose.  It is provided
 * "as is" without expressed or implied warranty.
 *
 * THE COPYRIGHT HOLDER DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO
 * EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
 * DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 *
 * Authors: 	Can-Ru Yeou, SiS Inc.,
 *		Thomas Winischhofer <thomas@winischhofer.net>,
 *		others.
 *
 * Previously taken and modified from tdfx_dri.h
 */

#ifndef _SIS_DRI_H_
#define _SIS_DRI_H_

#include "xf86drm.h"

/* Hack: When the types were changed, the typedefs
 * went into drm.h. This file did not exist earlier.
 */
#ifndef _DRM_H_
#define drm_handle_t drmHandle
#define drm_context_t drmContext
#endif

#define SIS_MAX_DRAWABLES 256
#define SISIOMAPSIZE (64*1024)

typedef struct {
  int CtxOwner;
  int QueueLength;		/* (300: current, 315/etc: total) length of command queue */
  unsigned int AGPCmdBufNext;   /* (rename to AGPVtxBufNext) */
  unsigned int FrameCount;
#ifdef SIS315DRI
  unsigned int  sharedWPoffset;	/* Offset to current queue position (shared with 2D) */
  unsigned int  cmdQueueOffset;	/* Offset of start of command queue in VRAM */
#endif
} SISSAREAPriv, *SISSAREAPrivPtr;

#define AGPVtxBufNext AGPCmdBufNext

#define SIS_FRONT 0
#define SIS_BACK 1
#define SIS_DEPTH 2

typedef struct {
  drm_handle_t handle;
  drmSize size;
#ifndef SISISXORG6899900
  drmAddress map;
#endif
} sisRegion, *sisRegionPtr;

typedef struct {
  sisRegion regs;			/* MMIO registers */
  sisRegion agp;			/* AGP public area */
  int deviceID;				/* = pSiS->Chipset (PCI ID) */
  int width;				/* = pScrn->virtualX */
  int height;				/* = pScrn->virtualY */
  int mem;				/* total video RAM; seems unused */
  int bytesPerPixel;			/* Screen's bpp/8 */
  int priv1;				/* unused */
  int priv2;				/* unused */
  int fbOffset;				/* Front buffer; set up, but unused by DRI driver*/
  int backOffset;			/* unused (handled by the DRI driver) */
  int depthOffset;			/* unused (handled by the DRI driver) */
  int textureOffset;			/* unused (handled by the DRI driver) */
  int textureSize;			/* unused (handled by the DRI driver) */
  unsigned int AGPCmdBufOffset;		/* (rename to AGPVtxBufOffset) */
  unsigned int AGPCmdBufSize;		/* (rename to AGPVtxBufSize)   */
  int irqEnabled;
  unsigned int scrnX;			/* TODO: = width = pScrn->virtualX */
  unsigned int scrnY;			/* TODO: = height = pScrn->virtualY */
#ifdef SIS315DRI
  unsigned char *AGPCmdBufBase;
  unsigned long AGPCmdBufAddr;
  unsigned long AGPCmdBufOffset2;	/* (rename to AGPCmdBufOffset) */
  unsigned int  AGPCmdBufSize2;		/* (rename to AGPCmdBufSize)   */
  int deviceRev;			/* Chip revision */
#endif
} SISDRIRec, *SISDRIPtr;

#define AGPVtxBufOffset AGPCmdBufOffset
#define AGPVtxBufSize AGPCmdBufSize

typedef struct {
  /* Nothing here yet */
  int dummy;
} SISConfigPrivRec, *SISConfigPrivPtr;

typedef struct {
  /* Nothing here yet */
  int dummy;
} SISDRIContextRec, *SISDRIContextPtr;

#include "screenint.h"

Bool SISDRIScreenInit(ScreenPtr pScreen);
void SISDRICloseScreen(ScreenPtr pScreen);
Bool SISDRIFinishScreenInit(ScreenPtr pScreen);

#endif
