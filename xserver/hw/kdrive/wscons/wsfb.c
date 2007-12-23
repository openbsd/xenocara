/* $OpenBSD: wsfb.c,v 1.5 2007/12/23 14:28:10 matthieu Exp $ */
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
/* Copyright © 1999 Keith Packard
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of Keith Packard not be used in
 * advertising or publicity pertaining to distribution of the software without
 * specific, written prior permission.  Keith Packard makes no
 * representations about the suitability of this software for any purpose.  It
 * is provided "as is" without express or implied warranty.
 *
 * KEITH PACKARD DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO
 * EVENT SHALL KEITH PACKARD BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
 * DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */

#ifdef HAVE_CONFIG_H
#include <kdrive-config.h>
#endif
#include <dev/wscons/wsconsio.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <errno.h>

#include <X11/X.h>
#include <X11/Xdefs.h>

#include "wsfb.h"

#define DEBUG 1
#define DBG(x) ErrorF x

extern int WsconsConsoleFd;

/* Map the framebuffer's memory */
static void *
wsfbMmap(size_t len, off_t off, int fd)
{
	int pagemask, mapsize;
	caddr_t addr;
	pointer mapaddr;

	pagemask = getpagesize() - 1;
	mapsize = ((int) len + pagemask) & ~pagemask;
	addr = 0;

	mapaddr = (pointer) mmap(addr, mapsize,
				 PROT_READ | PROT_WRITE, MAP_SHARED,
				 fd, off);
	if (mapaddr == (pointer) -1) {
		mapaddr = NULL;
	}
#if DEBUG
	ErrorF("mmap returns: addr %p len 0x%x\n", mapaddr, mapsize);
#endif
	return mapaddr;
}

Bool
wsfbMapFramebuffer(KdScreenInfo *screen)
{
	WsfbScrPriv *scrPriv = screen->driver;
	KdPointerMatrix m;
	WsfbPriv *priv = screen->card->driver;
	size_t len;

	DBG(("wsfbMapFrameBuffer\n"));
	
	if (scrPriv->mapped)
		return TRUE;
	if (scrPriv->randr != RR_Rotate_0)
		scrPriv->shadow = TRUE;
	else
		scrPriv->shadow = FALSE;

	KdComputePointerMatrix(&m, scrPriv->randr, 
	    screen->width, screen->height);
	KdSetPointerMatrix(&m);
	
	DBG(("screen->width %d\n", screen->width));
	DBG(("screen->height %d\n", screen->height));
	len = priv->linebytes * screen->height;
	priv->fb = wsfbMmap(len, 0, WsconsConsoleFd);
	if (priv->fb == NULL)
		FatalError("Can't mmap framebuffer\n");

	screen->memory_base = (CARD8 *)(priv->fb);
	screen->memory_size = len;
	
	if (scrPriv->shadow) {
		if (!KdShadowFbAlloc(screen, 0,
			scrPriv->randr & (RR_Rotate_90|RR_Rotate_270)))
			return FALSE;
		screen->off_screen_base = screen->memory_size;
	} else {
		screen->fb[0].byteStride = priv->linebytes;
		screen->fb[0].pixelStride = priv->linebytes * 8 / priv->bpp;
		screen->fb[0].frameBuffer = (CARD8 *)(priv->fb);
		screen->off_screen_base = 
		    screen->fb[0].byteStride * screen->height;
	}
	scrPriv->mapped = TRUE;
	return TRUE;
}

static Bool
wsfbInitialize(KdCardInfo *card, WsfbPriv *priv)
{

	DBG(("wsfbInitialize\n"));
	if (WsconsConsoleFd == -1) {
		ErrorF("wsfbInitialize: WsconsConsoleFd == -1\n");
		return FALSE;
	}
	if (ioctl(WsconsConsoleFd, WSDISPLAYIO_GTYPE, &priv->wstype) == -1) {
		ErrorF("wsfbInitialize: WSDISPLAY_GTYPE: %s\n",
		    strerror(errno));
		return FALSE;
	}
	if (ioctl(WsconsConsoleFd, WSDISPLAYIO_GETSUPPORTEDDEPTH,
		&priv->supportedDepths) == -1) {
		ErrorF("wsfbInitialize: WSDISPLAYIO_GETSUPPORTEDDEPTH: %s\n",
		    strerror(errno));
		return FALSE;
	}
	
	return TRUE;
}

Bool
wsfbCardInit(KdCardInfo *card)
{
	WsfbPriv *priv;

	DBG(("wsfbCardInit\n"));
	priv = (WsfbPriv *)xalloc(sizeof(WsfbPriv));
	if (priv == NULL)
		return FALSE;
	bzero(priv, sizeof(WsfbPriv));
	if (!wsfbInitialize(card, priv)) {
		xfree(priv);
		return FALSE;
	}
	card->driver = priv;

	return TRUE;
}

static Bool
wsfbScreenInitialize(KdScreenInfo *screen, WsfbScrPriv *scrpriv)
{
	struct wsdisplay_gfx_mode gfxmode;
	WsfbPriv *priv;
	int depth = 24;

	priv = screen->card->driver;

	DBG(("wsfbScreenInitialize\n"));
	DBG(("  screen dimensions %dx%d\n", screen->width, screen->height));
	DBG(("  screen depth %d\n", screen->fb[0].depth));
	DBG(("  randr %d\n", screen->randr));
	if (screen->width == 0 || screen->height == 0) {
		ErrorF("wsfbScreenInitialize: forcing 640x480\n");
		screen->width = 640;
		screen->height = 480;
	}
	if (screen->fb[0].depth == 0) {
		ErrorF("wsfbScreenInitialize: forcing depth 16\n");
		depth = screen->fb[0].depth = 16;
	}
	DBG(("  wstype: %d\n", priv->wstype));
	if (priv->wstype == WSDISPLAY_TYPE_PCIVGA) {
		gfxmode.depth = screen->fb[0].depth;
		gfxmode.width = screen->width;
		gfxmode.height = screen->height;
		if (ioctl(WsconsConsoleFd, WSDISPLAYIO_SETGFXMODE, 
			&gfxmode) == -1) {
			ErrorF("wsfbScreenInitialize: "
			    "WSDISPLAYIO_SETGFXMODE: %s\n", strerror(errno));
			return FALSE;
		}
	}
	if (ioctl(WsconsConsoleFd, WSDISPLAYIO_GINFO, &priv->info) == -1) {
		ErrorF("wsfbInitialize: WSDISPLAY_GINFO: %s\n",
		    strerror(errno));
		return FALSE;
	}
	if (ioctl(WsconsConsoleFd, WSDISPLAYIO_LINEBYTES, 
		&priv->linebytes) == -1) {
		ErrorF("wsfbInitialize: WSDISPLAYIO_LINEBYTES: %s\n",
		    strerror(errno));
		return FALSE;
	}
	switch (depth) {
	case 16:
		screen->fb[0].visuals =  (1 << TrueColor);
		screen->fb[0].redMask = 0x1f << 11;
		screen->fb[0].greenMask = 0x3f << 5;
		screen->fb[0].blueMask = 0x1f;
		break;
	case 24:
		screen->fb[0].visuals =  (1 << TrueColor);
		screen->fb[0].redMask = 0xff0000;
		screen->fb[0].greenMask = 0x00ff00;
		screen->fb[0].blueMask = 0x0000ff;
		break;
	default:
		screen->fb[0].redMask = 0;
		screen->fb[0].greenMask = 0;
		screen->fb[0].blueMask = 0;
		break;
	}
	screen->fb[0].bitsPerPixel = depth;
	priv->bpp = depth;	/* XXX */
	screen->dumb = TRUE;
	screen->rate = 72;
	scrpriv->randr = screen->randr;
	DBG(("wsfbScreenInitialize: done\n"));
	return TRUE;
}
    
Bool
wsfbScreenInit(KdScreenInfo *screen)
{
	WsfbScrPriv *scrPriv;

	DBG(("wsfbScreenInit\n"));
	scrPriv = (WsfbScrPriv *)xalloc(sizeof(WsfbScrPriv));
	if (scrPriv == NULL)
		return FALSE;
	bzero(scrPriv, sizeof(WsfbScrPriv));
	screen->driver = scrPriv;
	if (!wsfbScreenInitialize(screen, scrPriv)) {
		ErrorF("wsfbScreenInitialize: failed to initialize screen\n");
		screen->driver = NULL;
		xfree(scrPriv);
		return FALSE;
	}
	return TRUE;
}

Bool
wsfbInitScreen(ScreenPtr pScreen)
{
	DBG(("wsfbInitScreen\n"));
	pScreen->CreateColormap = wsfbCreateColormap;
	return TRUE;
}


Bool
wsfbFinishInitScreen(ScreenPtr pScreen)
{
	DBG(("wsfbFinishInitScreen\n"));

	if (!shadowSetup(pScreen))
		return FALSE;
#ifdef RANDR
	if (!wsfbRandRInit(pScreen))
		return FALSE;
#endif
	return TRUE;
}

Bool
wsfbCreateResources(ScreenPtr pScreen)
{
	DBG(("wsfbCreateResources\n"));
	return wsfbSetShadow(pScreen);
}

void
wsfbPreserve(KdCardInfo *card)
{
	DBG(("wsfbPreserve\n"));
}


Bool
wsfbEnable(ScreenPtr pScreen)
{
	KdScreenPriv(pScreen);
	KdScreenInfo *screen = pScreenPriv->screen;
	int wsmode = WSDISPLAYIO_MODE_DUMBFB;	

	DBG(("wsfbEnable\n"));
	/* Switch to graphics mode - required before mmap */
	if (ioctl(WsconsConsoleFd, WSDISPLAYIO_SMODE, &wsmode) == -1) {
		ErrorF("ioctl WSDISPLAYIO_SMODE: %s\n", strerror(errno));
		return FALSE;
	}
	if (!wsfbMapFramebuffer(screen)) {
		ErrorF("wsfbEnale: can't map framebuffer\n");
		return FALSE;
	}
	(*pScreen->ModifyPixmapHeader) (fbGetScreenPixmap (pScreen),
					pScreen->width,
					pScreen->height,
					screen->fb[0].depth,
					screen->fb[0].bitsPerPixel,
					screen->fb[0].byteStride,
					screen->fb[0].frameBuffer);
	DBG(("wsfbEnable done.\n"));
	return TRUE;
}

Bool
wsfbDPMS(ScreenPtr pScreen, int mode)
{
	DBG(("wsfb DPMS %d\n", mode));
	return TRUE;
}

void
wsfbDisable(ScreenPtr pScreen)
{
	DBG(("wsfbDisable\n"));
}

void
wsfbRestore(KdCardInfo *card)
{
	int mode = WSDISPLAYIO_MODE_EMUL;

	DBG(("wsfbRestore\n"));

	if (ioctl(WsconsConsoleFd, WSDISPLAYIO_SMODE, &mode) == -1) {
		ErrorF("WSDISPLAYIO_SMODE(EMUL): %s\n", strerror(errno));
	}
}

void
wsfbScreenFini(KdScreenInfo *screen)
{

	DBG(("wsfbScreenFini\n"));
	wsfbUnmapFramebuffer(screen);
}

void
wsfbCardFini(KdCardInfo *card)
{
	WsfbPriv *priv = card->driver;

	DBG(("wsfbCardFini\n"));
	/* unmap framebuffer */
	/* close fd */
	xfree(priv);
}

void
wsfbGetColors(ScreenPtr pScreen, int fb, int n, xColorItem *pdefs)
{
	DBG(("wsfbGetColors %d\n", n));

	while (n--) {
		pdefs->red = 0;
		pdefs->green = 0;
		pdefs->blue = 0;
		pdefs++;
	}
}

void
wsfbPutColors(ScreenPtr pScreen, int fb, int n, xColorItem *pdefs)
{
	DBG(("wsfbPutColors %d\n", n));
}

void *
wsfbWindowLinear(ScreenPtr	pScreen,
		   CARD32	row,
		   CARD32	offset,
		   int		mode,
		   CARD32	*size,
		   void		*closure)
{
	KdScreenPriv(pScreen);
	WsfbPriv *priv = pScreenPriv->card->driver;

	DBG(("wsfbWindowLinear\n"));
	
	if (!pScreenPriv->enabled)
		return NULL;
	*size = priv->linebytes;
	return (CARD8 *)priv->fb + row * priv->linebytes + offset;
}

void
wsfbSetScreenSizes(ScreenPtr pScreen)
{
	KdScreenPriv(pScreen);
	KdScreenInfo	*screen = pScreenPriv->screen;
	WsfbScrPriv	*scrpriv = screen->driver;
	WsfbPriv	*priv = screen->card->driver;

	DBG(("wsfbSetScreenSizes\n"));
	
	if (scrpriv->randr & (RR_Rotate_0|RR_Rotate_180)) {
		pScreen->width = priv->info.width;
		pScreen->height = priv->info.height;
		pScreen->mmWidth = screen->width_mm;
		pScreen->mmHeight = screen->height_mm;
	} else {
		pScreen->width = priv->info.height;
		pScreen->height = priv->info.width;
		pScreen->mmWidth = screen->height_mm;
		pScreen->mmHeight = screen->width_mm;
	}
}

void
wsfbUnmapFramebuffer(KdScreenInfo *screen)
{
	WsfbScrPriv *scrPriv = screen->driver;

	DBG(("wsfbUnmapFramebuffer\n"));
	if (!scrPriv->mapped)
		return;
	KdShadowFbFree(screen, 0);
	return;
}

Bool
wsfbSetShadow(ScreenPtr pScreen)
{
	KdScreenPriv(pScreen);
	KdScreenInfo *screen = pScreenPriv->screen;
	WsfbScrPriv *scrPriv = screen->driver;
	WsfbPriv *priv = screen->card->driver;
	ShadowUpdateProc update;
	ShadowWindowProc window;
	int useXY = 0;

	DBG(("wsfbSetShadow\n"));
	window = wsfbWindowLinear;
	update = NULL;
	if (scrPriv->randr) 
		if (priv->bpp == 16) {
			switch(scrPriv->randr) {
			case RR_Rotate_90:
				if (useXY) 
					update = shadowUpdateRotate16_90YX;
				else
					update = shadowUpdateRotate16_90;
				break;
			case RR_Rotate_180:
				update = shadowUpdateRotate16_180;
				break;
			case RR_Rotate_270:
				if (useXY)
					update = shadowUpdateRotate16_270YX;
				else
					update = shadowUpdateRotate16_270;
				break;
			default:
				update = shadowUpdateRotate16;
				break;
			}
		} else {
			update = shadowUpdateRotatePacked;
		}
	else
		update = shadowUpdateRotatePacked;
	
	return KdShadowSet(pScreen, scrPriv->randr, update, window);
}

Bool
wsfbCreateColormap(ColormapPtr pmap)
{
	DBG(("wsfbCreateColormap\n"));
	return fbInitializeColormap(pmap);
}

#ifdef RANDR
Bool
wsfbRandRGetInfo(ScreenPtr pScreen, Rotation *rotations)
{
	KdScreenPriv(pScreen);
	KdScreenInfo *screen = pScreenPriv->screen;
	WsfbScrPriv *scrPriv = screen->driver;
	RRScreenSizePtr pSize;
	Rotation randr;
	int n;

	DBG(("wsfbRandRGetInfo\n"));
	*rotations = RR_Rotate_All|RR_Reflect_All;
	
	for (n = 0; n < pScreen->numDepths; n++)
		if (pScreen->allowedDepths[n].numVids != 0)
			break;
	if (n == pScreen->numDepths)
		return FALSE;
	pSize = RRRegisterSize(pScreen,
	    screen->width,
	    screen->height,
	    screen->width_mm,
	    screen->height_mm);
	randr = KdSubRotation(scrPriv->randr, screen->randr);
	RRSetCurrentConfig(pScreen, randr, 0, pSize);
	    
	return TRUE;
}

Bool
wsfbRandRSetConfig(ScreenPtr		pScreen,
		     Rotation		randr,
		     int		rate,
		     RRScreenSizePtr	pSize)
{
	KdScreenPriv(pScreen);
	KdScreenInfo	*screen = pScreenPriv->screen;
	WsfbScrPriv	*scrpriv = screen->driver;
	Bool		wasEnabled = pScreenPriv->enabled;
	WsfbScrPriv	oldscr;
	int		oldwidth;
	int		oldheight;
	int		oldmmwidth;
	int		oldmmheight;
	int		newwidth, newheight;

	DBG(("wsfbRandRSetConfig\n"));
	
	if (screen->randr & (RR_Rotate_0|RR_Rotate_180)) {
		newwidth = pSize->width;
		newheight = pSize->height;
	} else {
		newwidth = pSize->height;
		newheight = pSize->width;
	}
	
	if (wasEnabled)
		KdDisableScreen (pScreen);
	
	oldscr = *scrpriv;
	
	oldwidth = screen->width;
	oldheight = screen->height;
	oldmmwidth = pScreen->mmWidth;
	oldmmheight = pScreen->mmHeight;
	
	/*
	 * Set new configuration
	 */
	
	scrpriv->randr = KdAddRotation (screen->randr, randr);
	
	KdOffscreenSwapOut (screen->pScreen);
	
	wsfbUnmapFramebuffer (screen);
	
	if (!wsfbMapFramebuffer (screen))
		goto bail4;
	
	KdShadowUnset (screen->pScreen);
	
	if (!wsfbSetShadow (screen->pScreen))
		goto bail4;
	
	wsfbSetScreenSizes (screen->pScreen);
	
	/*
	 * Set frame buffer mapping
	 */
	(*pScreen->ModifyPixmapHeader) (fbGetScreenPixmap (pScreen),
	    pScreen->width,
	    pScreen->height,
	    screen->fb[0].depth,
	    screen->fb[0].bitsPerPixel,
	    screen->fb[0].byteStride,
	    screen->fb[0].frameBuffer);
	
	/* set the subpixel order */
	
	KdSetSubpixelOrder (pScreen, scrpriv->randr);
	if (wasEnabled)
		KdEnableScreen (pScreen);
	
	return TRUE;
	
bail4:
	wsfbUnmapFramebuffer (screen);
	*scrpriv = oldscr;
	(void) wsfbMapFramebuffer (screen);
	pScreen->width = oldwidth;
	pScreen->height = oldheight;
	pScreen->mmWidth = oldmmwidth;
	pScreen->mmHeight = oldmmheight;
	
	if (wasEnabled)
		KdEnableScreen (pScreen);
	return FALSE;
}

Bool
wsfbRandRInit(ScreenPtr pScreen)
{
	rrScrPrivPtr    pScrPriv;
	
	DBG(("wsfbRandRInit\n"));
    
	if (!RRScreenInit (pScreen))
		return FALSE;
	
	pScrPriv = rrGetScrPriv(pScreen);
	pScrPriv->rrGetInfo = wsfbRandRGetInfo;
	pScrPriv->rrSetConfig = wsfbRandRSetConfig;
	return TRUE;
}
#endif /* RANDR */
