/* $OpenBSD: wsfb.h,v 1.2 2007/05/27 00:56:29 matthieu Exp $ */
/*
 * Copyright (c) 2007 Matthieu Herrb <matthieu@openbsd.org>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */
#ifndef _WSFB_H
#define _WSFB_H

#include "kdrive.h"
#ifdef RANDR
#include "randrstr.h"
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

typedef struct _wsfbPriv {
	int wstype;
	struct wsdisplay_fbinfo info;
	int linebytes;
	int supportedDepths;
	int bpp;
	char *fb;
	char *fb_base;
} WsfbPriv;

typedef struct _wsfbScrPriv {
	Rotation randr;
	Bool shadow;
	PixmapPtr pShadow;
} WsfbScrPriv;

extern KdCardFuncs wsfbFuncs;

Bool wsfbCardInit(KdCardInfo *);
Bool wsfbScreenInit(KdScreenInfo *);
Bool wsfbInitScreen(ScreenPtr);
Bool wsfbFinishInitScreen(ScreenPtr);
Bool wsfbCreateResources(ScreenPtr);
void wsfbPreserve(KdCardInfo *);
Bool wsfbEnable(ScreenPtr);
Bool wsfbDPMS(ScreenPtr, int);
void wsfbDisable(ScreenPtr);
void wsfbRestore(KdCardInfo *);
void wsfbScreenFini(KdScreenInfo *);
void wsfbCardFini(KdCardInfo *);
void wsfbGetColors(ScreenPtr, int, int, xColorItem *);
void wsfbPutColors(ScreenPtr, int, int, xColorItem *);

Bool wsfbMapFramebuffer(KdScreenInfo *);
void *wsfbWindowLinear(ScreenPtr, CARD32, CARD32, int, CARD32 *, void *);
void wsfbSetScreenSizes(ScreenPtr);
Bool wsfbUnmapFramebuffer(KdScreenInfo *);
Bool wsfbSetShadow(ScreenPtr);
Bool wsfbCreateColormap(ColormapPtr);

#ifdef RANDR
Bool wsfbRandRGetInfo(ScreenPtr, Rotation *);
Bool wsfbRandRSetConfig(ScreenPtr, Rotation, int, RRScreenSizePtr);
Bool wsfbRandRInit(ScreenPtr);
#endif

#endif /* _WSFB_H */
