/* $XFree86$ */
/*
 * Copyright 2003 Alex Deucher.
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
 * NON-INFRINGEMENT.  IN NO EVENT SHALL ALEX DEUCHER, OR ANY OTHER
 * CONTRIBUTORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

/*
 * Authors:
 *   Alex Deucher <agd5f@yahoo.com>
 *
 */

#ifndef _RADEON_MERGEDFB_H_
#define _RADEON_MERGEDFB_H_

#include "xf86.h"

#include "radeon.h"

#if 0
		/* Psuedo Xinerama support */
#define NEED_REPLIES  		/* ? */
#define EXTENSION_PROC_ARGS void *
#include "extnsionst.h"  	/* required */
#include <X11/extensions/panoramiXproto.h>  	/* required */
#define RADEON_XINERAMA_MAJOR_VERSION  1
#define RADEON_XINERAMA_MINOR_VERSION  1

/* needed for pseudo-xinerama by radeon_driver.c */
Bool 		RADEONnoPanoramiXExtension = TRUE;

/* Relative merge position */
typedef enum {
   radeonLeftOf,
   radeonRightOf,
   radeonAbove,
   radeonBelow,
   radeonClone
} RADEONScrn2Rel;
#endif

#define SDMPTR(x) ((RADEONMergedDisplayModePtr)(x->currentMode->Private))
#define CDMPTR    ((RADEONMergedDisplayModePtr)(info->CurrentLayout.mode->Private))

#define BOUND(test,low,hi) { \
    if(test < low) test = low; \
    if(test > hi) test = hi; }

#define REBOUND(low,hi,test) { \
    if(test < low) { \
        hi += test-low; \
        low = test; } \
    if(test > hi) { \
        low += test-hi; \
        hi = test; } }

typedef struct _MergedDisplayModeRec {
    DisplayModePtr CRT1;
    DisplayModePtr CRT2;
    RADEONScrn2Rel    CRT2Position;
} RADEONMergedDisplayModeRec, *RADEONMergedDisplayModePtr;

typedef struct _RADEONXineramaData {
    int x;
    int y;
    int width;
    int height;
} RADEONXineramaData;

/* needed by radeon_driver.c */
extern void
RADEONAdjustFrameMerged(int scrnIndex, int x, int y, int flags);
extern void
RADEONMergePointerMoved(int scrnIndex, int x, int y);
extern DisplayModePtr
RADEONGenerateModeList(ScrnInfoPtr pScrn, char* str,
		    DisplayModePtr i, DisplayModePtr j,
		    RADEONScrn2Rel srel);
extern int
RADEONStrToRanges(range *r, char *s, int max);
extern void
RADEONXineramaExtensionInit(ScrnInfoPtr pScrn);
extern void
RADEONUpdateXineramaScreenInfo(ScrnInfoPtr pScrn1);
extern void
RADEONMergedFBSetDpi(ScrnInfoPtr pScrn1, ScrnInfoPtr pScrn2, RADEONScrn2Rel srel);
extern void
RADEONMergedFBResetDpi(ScrnInfoPtr pScrn, Bool force);
extern void
RADEONRecalcDefaultVirtualSize(ScrnInfoPtr pScrn);

/* needed by radeon_cursor.c */
extern void
RADEONSetCursorPositionMerged(ScrnInfoPtr pScrn, int x, int y);

/* needed by radeon_video.c */
extern void
RADEONChooseOverlayCRTC(ScrnInfoPtr, BoxPtr);

#endif /* _RADEON_MERGEDFB_H_ */
