/* $OpenBSD: wsfb.c,v 1.1 2007/05/25 15:33:32 matthieu Exp $ */
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

#ifdef HAVE_CONFIG_H
#include <kdrive-config.h>
#endif
#include <dev/wscons/wsconsio.h>

#include <X11/X.h>
#include <X11/Xdefs.h>

#include "wsfb.h"

Bool
wsfbInitialize(KdCardInfo *card, WsfbPriv *priv)
{
	return TRUE;
}

Bool
wsfbCardInit(KdCardInfo *card)
{
	WsfbPriv *priv;

	priv = (WsfbPriv *)xalloc(sizeof(WsfbPriv));
	if (priv == NULL)
		return FALSE;
	if (!wsfbInitialize(card, priv)) {
		xfree(priv);
		return FALSE;
	}
	card->driver = priv;

	return TRUE;
}

Bool
wsfbScreenInitialize(KdScreenInfo *screen, WsfbScrPriv *scrpriv)
{
	return TRUE;
}
    
Bool
wsfbScreenInit(KdScreenInfo *screen)
{
	WsfbScrPriv *scrPriv;

	scrPriv = (WsfbScrPriv *)xalloc(sizeof(WsfbScrPriv));
	if (scrPriv == NULL)
		return FALSE;
	bzero(scrPriv, sizeof(WsfbScrPriv));
	screen->driver = scrPriv;
	if (!wsfbScreenInitialize(screen, scrPriv)) {
		screen->driver = NULL;
		xfree(scrPriv);
		return FALSE;
	}
	return TRUE;
}

Bool
wsfbInitScreen(ScreenPtr pScreen)
{
	pScreen->CreateColormap = wsfbCreateColormap;
	return TRUE;
}


Bool
wsfbFinishInitScreen(ScreenPtr pScreen)
{
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
	return wsfbSetShadow(pScreen);
}

void
wsfbPreserve(KdCardInfo *card)
{
}


Bool
wsfbEnable(ScreenPtr pScreen)
{
	return TRUE;
}

Bool
wsfbDPMS(ScreenPtr pScreen, int mode)
{
	return TRUE;
}

void
wsfbDisable(ScreenPtr pScreen)
{
}

void
wsfbRestore(KdCardInfo *card)
{
}

void
wsfbScreenFini(KdScreenInfo *screen)
{
}

void
wsfbCardFini(KdCardInfo *card)
{
	WsfbPriv *priv = card->driver;

	/* unmap framebuffer */
	/* close fd */
	xfree(priv);
}

void
wsfbGetColors(ScreenPtr pScreen, int fb, int n, xColorItem *pdefs)
{
}

void
wsfbPutColors(ScreenPtr pScreen, int fb, int n, xColorItem *pdefs)
{
}

Bool
wsfbMapFramebuffer(KdScreenInfo *screen)
{
	WsfbScrPriv *scrPriv = screen->driver;
	KdMouseMatrix m;
	WsfbPriv *priv = screen->card->driver;

	if (scrPriv->randr != RR_Rotate_0)
		scrPriv->shadow = TRUE;
	else
		scrPriv->shadow = FALSE;

	KdComputeMouseMatrix(&m, scrPriv->randr, 
	    screen->width, screen->height);
	KdSetMouseMatrix(&m);
	
	/* ?? */
	/* screen->width = priv->var.xres;
	   screen->height = priv->var.yres; */
	screen->memory_base = (CARD8 *)(priv->fb);
	/* screen->memory_size = priv->fix.smem_len; */
	
	if (scrPriv->shadow) {
		if (!KdShadowFbAlloc(screen, 0,
			scrPriv->randr & (RR_Rotate_90|RR_Rotate_270)))
			return FALSE;
		screen->off_screen_base = screen->memory_size;
	} else {
		/* screen->fb[0].byteStride = priv->fix.line_length;
		   screen->fb[0].pixelStride = (prif->fix.line_length * 8 /
		   priv->var.bits_per_pixel); */
		screen->fb[0].frameBuffer = (CARD8 *)(priv->fb);
		screen->off_screen_base = 
		    screen->fb[0].byteStride * screen->height;
	}
	return TRUE;
}

void *
wsfbWindowLinear(ScreenPtr	pScreen,
		   CARD32	row,
		   CARD32	offset,
		   int		mode,
		   CARD32	*size,
		   void		*closure)
{
	return NULL;
}

void
wsfbSetScreenSizes(ScreenPtr pScreen)
{
}

Bool
wsfbUnmapFramebuffer(KdScreenInfo *screen)
{
	KdShadowFbFree(screen, 0);
	return TRUE;
}

Bool
wsfbSetShadow(ScreenPtr pScreen)
{
	return TRUE;
}

Bool
wsfbCreateColormap(ColormapPtr pmap)
{
	return TRUE;
}

#ifdef RANDR
Bool
wsfbRandRGetInfo(ScreenPtr pScreen, Rotation *rotations)
{
	return TRUE;
}

Bool
wsfbRandRSetConfig(ScreenPtr		pScreen,
		     Rotation		randr,
		     int		rate,
		     RRScreenSizePtr	pSize)
{
	return TRUE;
}

Bool
wsfbRandRInit(ScreenPtr pScreen)
{
	return TRUE;
}
#endif /* RANDR */
