/*
 * Copyright 1999 through 2004 by Marc Aurele La France (TSI @ UQV), tsi@xfree86.org
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that copyright
 * notice and this permission notice appear in supporting documentation, and
 * that the name of Marc Aurele La France not be used in advertising or
 * publicity pertaining to distribution of the software without specific,
 * written prior permission.  Marc Aurele La France makes no representations
 * about the suitability of this software for any purpose.  It is provided
 * "as-is" without express or implied warranty.
 *
 * MARC AURELE LA FRANCE DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS.  IN NO
 * EVENT SHALL MARC AURELE LA FRANCE BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
 * DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 *
 * DRI support by:
 *    Gareth Hughes <gareth@valinux.com>
 *    José Fonseca <j_r_fonseca@yahoo.co.uk>
 *    Leif Delgass <ldelgass@retinalburn.net>
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <string.h>

#include "ati.h"
#include "atibus.h"
#include "atichip.h"
#include "aticursor.h"
#include "atidac.h"
#include "atidga.h"
#include "atidri.h"
#include "atimach64.h"
#include "atimode.h"
#include "atistruct.h"
#include "atiscreen.h"
#include "atixv.h"
#include "atimach64accel.h"
#include "aticonsole.h"

#ifdef XF86DRI_DEVEL
#include "mach64_dri.h"
#include "mach64_sarea.h"
#endif

#ifdef TV_OUT

#include "atichip.h"

#endif /* TV_OUT */

#include "shadowfb.h"
#include "xf86cmap.h"

#include "fb.h"

#include "micmap.h"
#include "mipointer.h"

/*
 * ATIRefreshArea --
 *
 * This function is called by the shadow frame buffer code to refresh the
 * hardware frame buffer.
 */
static void
ATIRefreshArea
(
    ScrnInfoPtr pScreenInfo,
    int         nBox,
    BoxPtr      pBox
)
{
    ATIPtr  pATI = ATIPTR(pScreenInfo);
    pointer pSrc, pDst;
    int     offset, w, h;

    while (nBox-- > 0)
    {
        w = (pBox->x2 - pBox->x1) * pATI->AdjustDepth;
        h = pBox->y2 - pBox->y1;
        offset = (pBox->y1 * pATI->FBPitch) + (pBox->x1 * pATI->AdjustDepth);
        pSrc = (char *)pATI->pShadow + offset;
        pDst = (char *)pATI->pMemory + offset;

        while (h-- > 0)
        {
            (void)memcpy(pDst, pSrc, w);
            pSrc = (char *)pSrc + pATI->FBPitch;
            pDst = (char *)pDst + pATI->FBPitch;
        }

        pBox++;
    }
}

/*
 * ATIMinBits --
 *
 * Compute log base 2 of val.
 */
static int
ATIMinBits
(
    int val
)
{
    int bits;

    if (!val) return 1;
    for (bits = 0; val; val >>= 1, ++bits);
    return bits;
}

#ifdef USE_XAA
static Bool
ATIMach64SetupMemXAA_NoDRI
(
    ScrnInfoPtr pScreenInfo,
    ScreenPtr pScreen
)
{
    ATIPtr       pATI        = ATIPTR(pScreenInfo);

    int maxScanlines = ATIMach64MaxY;
    int maxPixelArea, PixelArea;

    {
        /*
         * Note:  If PixelArea exceeds the engine's maximum, the excess is
         *        never used, even though it would be useful for such things
         *        as XVideo buffers.
         */
        maxPixelArea = maxScanlines * pScreenInfo->displayWidth;
        PixelArea = pScreenInfo->videoRam * 1024 * 8 / pATI->bitsPerPixel;
        if (PixelArea > maxPixelArea)
            PixelArea = maxPixelArea;
        xf86InitFBManagerArea(pScreen, PixelArea, 2);
    }

    return TRUE;
}

#ifdef XF86DRI_DEVEL
/*
 * Memory layour for XAA with DRI (no local_textures):
 * | front  | pixmaps, xv | back   | depth  | textures | c |
 *
 * 1024x768@16bpp with 8 MB:
 * | 1.5 MB | ~3.5 MB     | 1.5 MB | 1.5 MB | 0        | c |
 *
 * 1024x768@32bpp with 8 MB:
 * | 3.0 MB | ~0.5 MB     | 3.0 MB | 1.5 MB | 0        | c |
 *
 * "c" is the hw cursor which occupies 1KB
 */
static Bool
ATIMach64SetupMemXAA
(
    ScrnInfoPtr pScreenInfo,
    ScreenPtr pScreen
)
{
	ATIPtr       pATI        = ATIPTR(pScreenInfo);

	ATIDRIServerInfoPtr pATIDRIServer = pATI->pDRIServerInfo;
	int cpp = pATI->bitsPerPixel >> 3;
	int widthBytes = pScreenInfo->displayWidth * cpp;
	int zWidthBytes = pScreenInfo->displayWidth * 2; /* always 16-bit z-buffer */
	int fbSize = pScreenInfo->videoRam * 1024;
	int bufferSize = pScreenInfo->virtualY * widthBytes;
	int zBufferSize = pScreenInfo->virtualY * zWidthBytes;
	int offscreenBytes, total, scanlines;

	pATIDRIServer->fbX = 0;
	pATIDRIServer->fbY = 0;
	pATIDRIServer->frontOffset = 0;
	pATIDRIServer->frontPitch = pScreenInfo->displayWidth;

	/* Calculate memory remaining for pixcache and textures after 
	 * front, back, and depth buffers
	 */
	offscreenBytes = fbSize - ( 2 * bufferSize + zBufferSize );

	if ( !pATIDRIServer->IsPCI && !pATI->OptionLocalTextures ) {
	    /* Don't allocate a local texture heap for AGP unless requested */
	    pATIDRIServer->textureSize = 0;
	} else {
	    int l, maxPixcache;

#ifdef XvExtension

	    int xvBytes;

	    /* Try for enough pixmap cache for DVD and a full viewport
	     */
	    xvBytes = 720*480*cpp; /* enough for single-buffered DVD */
	    maxPixcache = xvBytes > bufferSize ? xvBytes : bufferSize;

#else /* XvExtension */

	    /* Try for one viewport */
	    maxPixcache = bufferSize;

#endif /* XvExtension */

	    pATIDRIServer->textureSize = offscreenBytes - maxPixcache;

	    /* If that gives us less than half the offscreen mem available for textures, split 
	     * the available mem between textures and pixmap cache
	     */
	    if (pATIDRIServer->textureSize < (offscreenBytes/2)) {
		pATIDRIServer->textureSize = offscreenBytes/2;
	    }

	    if (pATIDRIServer->textureSize <= 0)
		pATIDRIServer->textureSize = 0;

	    l = ATIMinBits((pATIDRIServer->textureSize-1) / MACH64_NR_TEX_REGIONS);
	    if (l < MACH64_LOG_TEX_GRANULARITY) l = MACH64_LOG_TEX_GRANULARITY;

	    /* Round the texture size up to the nearest whole number of
	     * texture regions.  Again, be greedy about this, don't round
	     * down.
	     */
	    pATIDRIServer->logTextureGranularity = l;
	    pATIDRIServer->textureSize =
		(pATIDRIServer->textureSize >> l) << l;
	}

	total = fbSize - pATIDRIServer->textureSize;
	scanlines = total / widthBytes;
	if (scanlines > ATIMach64MaxY) scanlines = ATIMach64MaxY;

	/* Recalculate the texture offset and size to accommodate any
	 * rounding to a whole number of scanlines.
	 * FIXME: Is this actually needed?
	 */
	pATIDRIServer->textureOffset = scanlines * widthBytes;
	pATIDRIServer->textureSize = fbSize - pATIDRIServer->textureOffset;

	/* Set a minimum usable local texture heap size.  This will fit
	 * two 256x256 textures.  We check this after any rounding of
	 * the texture area.
	 */
	if (pATIDRIServer->textureSize < 256*256 * cpp * 2) {
	    pATIDRIServer->textureOffset = 0;
	    pATIDRIServer->textureSize = 0;
	    scanlines = fbSize / widthBytes;
	    if (scanlines > ATIMach64MaxY) scanlines = ATIMach64MaxY;
	}

	pATIDRIServer->depthOffset = scanlines * widthBytes - zBufferSize;
	pATIDRIServer->depthPitch = pScreenInfo->displayWidth;
	pATIDRIServer->depthY = pATIDRIServer->depthOffset/widthBytes;
	pATIDRIServer->depthX =  (pATIDRIServer->depthOffset - 
				  (pATIDRIServer->depthY * widthBytes)) / cpp;

	pATIDRIServer->backOffset = pATIDRIServer->depthOffset - bufferSize;
	pATIDRIServer->backPitch = pScreenInfo->displayWidth;
	pATIDRIServer->backY = pATIDRIServer->backOffset/widthBytes;
	pATIDRIServer->backX =  (pATIDRIServer->backOffset - 
				  (pATIDRIServer->backY * widthBytes)) / cpp;

	scanlines = fbSize / widthBytes;
	if (scanlines > ATIMach64MaxY) scanlines = ATIMach64MaxY;

	if ( pATIDRIServer->IsPCI && pATIDRIServer->textureSize == 0 ) {
	    xf86DrvMsg(pScreenInfo->scrnIndex, X_WARNING,
		       "Not enough memory for local textures, disabling DRI\n");
	    ATIDRICloseScreen(pScreen);
	    pATI->directRenderingEnabled = FALSE;
	} else {
	    BoxRec ScreenArea;

	    ScreenArea.x1 = 0;
	    ScreenArea.y1 = 0;
	    ScreenArea.x2 = pATI->displayWidth;
	    ScreenArea.y2 = scanlines;

	    if (!xf86InitFBManager(pScreen, &ScreenArea)) {
		xf86DrvMsg(pScreenInfo->scrnIndex, X_ERROR,
			   "Memory manager initialization to (%d,%d) (%d,%d) failed\n",
			   ScreenArea.x1, ScreenArea.y1,
			   ScreenArea.x2, ScreenArea.y2);
		return FALSE;
	    } else {
		int width, height;

		xf86DrvMsg(pScreenInfo->scrnIndex, X_INFO,
			   "Memory manager initialized to (%d,%d) (%d,%d)\n",
			   ScreenArea.x1, ScreenArea.y1, ScreenArea.x2, ScreenArea.y2);

		if (xf86QueryLargestOffscreenArea(pScreen, &width, &height, 0, 0, 0)) {
		    xf86DrvMsg(pScreenInfo->scrnIndex, X_INFO,
			       "Largest offscreen area available: %d x %d\n",
			       width, height);

		    /* lines in offscreen area needed for depth buffer and textures */
		    pATI->depthTexLines = scanlines
			- pATIDRIServer->depthOffset / widthBytes;
		    pATI->backLines     = scanlines
			- pATIDRIServer->backOffset / widthBytes
			- pATI->depthTexLines;
		    pATI->depthTexArea  = NULL;
		    pATI->backArea      = NULL;
		} else {
		    xf86DrvMsg(pScreenInfo->scrnIndex, X_ERROR, 
			       "Unable to determine largest offscreen area available\n");
		    return FALSE;
		}

	    }

	    xf86DrvMsg(pScreenInfo->scrnIndex, X_INFO, "Will use %d kB of offscreen memory for XAA\n", 
		       (offscreenBytes - pATIDRIServer->textureSize)/1024);

	    xf86DrvMsg(pScreenInfo->scrnIndex, X_INFO, "Will use back buffer at offset 0x%x\n",
		       pATIDRIServer->backOffset);

	    xf86DrvMsg(pScreenInfo->scrnIndex, X_INFO, "Will use depth buffer at offset 0x%x\n",
		       pATIDRIServer->depthOffset);

	    if (pATIDRIServer->textureSize > 0) {
		xf86DrvMsg(pScreenInfo->scrnIndex, X_INFO,
			   "Will use %d kB for local textures at offset 0x%x\n",
			   pATIDRIServer->textureSize/1024,
			   pATIDRIServer->textureOffset);
	    }
	}

	return TRUE;
}
#endif /* XF86DRI_DEVEL */
#endif /* USE_XAA */
  	 
/*
 * ATIScreenInit --
 *
 * This function is called by DIX to initialise the screen.
 */
Bool
ATIScreenInit(SCREEN_INIT_ARGS_DECL)
{
    ScrnInfoPtr  pScreenInfo = xf86ScreenToScrn(pScreen);
    ATIPtr       pATI        = ATIPTR(pScreenInfo);
    pointer      pFB;
    int          VisualMask;

    /* Set video hardware state */
    if (!ATIEnterGraphics(pScreen, pScreenInfo, pATI))
        return FALSE;

    /* Re-initialise mi's visual list */
    miClearVisualTypes();

    if ((pATI->depth > 8) && (pATI->DAC == ATI_DAC_INTERNAL))
        VisualMask = TrueColorMask;
    else
        VisualMask = miGetDefaultVisualMask(pATI->depth);

    if (!miSetVisualTypes(pATI->depth, VisualMask, pATI->rgbBits,
                          pScreenInfo->defaultVisual))
        return FALSE;

    if (!miSetPixmapDepths())
        return FALSE;

    pFB = pATI->pMemory;
    pATI->FBPitch = PixmapBytePad(pATI->displayWidth, pATI->depth);
    if (pATI->OptionShadowFB)
    {
        pATI->FBBytesPerPixel = pATI->bitsPerPixel >> 3;
        pATI->FBPitch = PixmapBytePad(pATI->displayWidth, pATI->depth);
        if ((pATI->pShadow = malloc(pATI->FBPitch * pScreenInfo->virtualY)))
        {
            pFB = pATI->pShadow;
        }
        else
        {
            xf86DrvMsg(pScreenInfo->scrnIndex, X_WARNING,
                "Insufficient virtual memory for shadow frame buffer.\n");
            pATI->OptionShadowFB = FALSE;
        }
    }

#ifdef XF86DRI_DEVEL

    /* Setup DRI after visuals have been established, but before
     * fbScreenInit is called.
     */

    /* According to atiregs.h, GTPro (3D Rage Pro) is the first chip type with
     * 3D triangle setup (the VERTEX_* registers)
     */
    if (pATI->Chip < ATI_CHIP_264GTPRO) {
	xf86DrvMsg(pScreenInfo->scrnIndex, X_WARNING,
		   "Direct rendering is not supported for ATI chips earlier than "
		   "the ATI 3D Rage Pro.\n");
	pATI->directRenderingEnabled = FALSE;
    } else {
	/* FIXME: When we move to dynamic allocation of back and depth
	 * buffers, we will want to revisit the following check for 3
	 * times the virtual size (or 2.5 times for 24-bit depth) of the screen below.
	 */
	int cpp = pATI->bitsPerPixel >> 3;
	int maxY = pScreenInfo->videoRam * 1024 / (pATI->displayWidth * cpp);
	int requiredY;

	requiredY = pScreenInfo->virtualY * 2     /* front, back buffers */
	    + (pScreenInfo->virtualY * 2 / cpp);  /* depth buffer (always 16-bit) */

	if (!pATI->OptionAccel) {
	    xf86DrvMsg(pScreenInfo->scrnIndex, X_WARNING,
		       "Acceleration disabled, not initializing the DRI\n");
	    pATI->directRenderingEnabled = FALSE;
	} else if ( maxY > requiredY ) {
	    pATI->directRenderingEnabled = ATIDRIScreenInit(pScreen);
	} else {
	    xf86DrvMsg(pScreenInfo->scrnIndex, X_WARNING,
		       "DRI static buffer allocation failed -- "
		       "need at least %d kB video memory\n",
		       (pScreenInfo->displayWidth * requiredY * cpp ) / 1024);
	    pATI->directRenderingEnabled = FALSE;
	}
    }

#endif /* XF86DRI_DEVEL */

    /* Initialise framebuffer layer */
    switch (pATI->bitsPerPixel)
    {
        case 8:
        case 16:
        case 24:
        case 32:
            pATI->Closeable = fbScreenInit(pScreen, pFB,
                pScreenInfo->virtualX, pScreenInfo->virtualY,
                pScreenInfo->xDpi, pScreenInfo->yDpi, pATI->displayWidth,
                pATI->bitsPerPixel);
            break;

        default:
            return FALSE;
    }

    if (!pATI->Closeable)
        return FALSE;

    /* Fixup RGB ordering */
    if (pATI->depth > 8)
    {
        VisualPtr pVisual = pScreen->visuals + pScreen->numVisuals;

        while (--pVisual >= pScreen->visuals)
        {
            if ((pVisual->class | DynamicClass) != DirectColor)
                continue;

            pVisual->offsetRed = pScreenInfo->offset.red;
            pVisual->offsetGreen = pScreenInfo->offset.green;
            pVisual->offsetBlue = pScreenInfo->offset.blue;

            pVisual->redMask = pScreenInfo->mask.red;
            pVisual->greenMask = pScreenInfo->mask.green;
            pVisual->blueMask = pScreenInfo->mask.blue;
        }
    }

    /* initialise RENDER extension */
    if (!fbPictureInit(pScreen, NULL, 0) && (serverGeneration == 1))
    {
	xf86DrvMsg(pScreenInfo->scrnIndex, X_WARNING,
	    "RENDER extension initialisation failed.\n");
    }

    xf86SetBlackWhitePixels(pScreen);

#ifdef USE_XAA

    if (!pATI->useEXA) {

    /* Memory manager setup */

#ifdef XF86DRI_DEVEL
    if (pATI->directRenderingEnabled)
    {
        if (!ATIMach64SetupMemXAA(pScreenInfo, pScreen))
            return FALSE;
    }
    else
#endif /* XF86DRI_DEVEL */
    {
        if (!ATIMach64SetupMemXAA_NoDRI(pScreenInfo, pScreen))
            return FALSE;
    }

    /* Setup acceleration */

    if (pATI->OptionAccel && !ATIMach64AccelInit(pScreen))
        return FALSE;

    }

#endif /* USE_XAA */

#ifdef USE_EXA

    if (pATI->useEXA) {
        /* EXA setups both memory manager and acceleration here */

        if (pATI->OptionAccel && !ATIMach64ExaInit(pScreen))
            return FALSE;
    }

#endif /* USE_EXA */

#ifndef AVOID_DGA

    /* Initialise DGA support */
    (void)ATIDGAInit(pScreen, pScreenInfo, pATI);

#endif /* AVOID_DGA */

    /* Initialise backing store */
    xf86SetBackingStore(pScreen);

    /* Initialise cursor */
    if (!ATIMach64CursorInit(pScreen))
        return FALSE;

    /* Create default colourmap */
    if (!miCreateDefColormap(pScreen))
        return FALSE;

    if (!xf86HandleColormaps(pScreen, 256, pATI->rgbBits, ATILoadPalette, NULL,
                             CMAP_PALETTED_TRUECOLOR |
                             CMAP_LOAD_EVEN_IF_OFFSCREEN))
            return FALSE;

    /* Initialise shadow framebuffer */
    if (pATI->OptionShadowFB &&
        !ShadowFBInit(pScreen, ATIRefreshArea))
        return FALSE;

    /* Initialise DPMS support */
    (void)xf86DPMSInit(pScreen, ATISetDPMSMode, 0);

    /* Initialise XVideo support */
    (void)ATIInitializeXVideo(pScreen, pScreenInfo, pATI);

    /* Set pScreen->SaveScreen and wrap CloseScreen vector */
    pScreen->SaveScreen = ATISaveScreen;
    pATI->CloseScreen = pScreen->CloseScreen;
    pScreen->CloseScreen = ATICloseScreen;

    if (serverGeneration == 1)
        xf86ShowUnusedOptions(pScreenInfo->scrnIndex, pScreenInfo->options);

#ifdef TV_OUT
    /* Fix-up TV out after ImpacTV probe */
    if (pATI->OptionTvOut && pATI->Chip < ATI_CHIP_264GTPRO)
        ATISwitchMode(SWITCH_MODE_ARGS(pScreenInfo, pScreenInfo->currentMode));
#endif /* TV_OUT */

#ifdef XF86DRI_DEVEL

    /* DRI finalization */
    if (pATI->directRenderingEnabled) {
	/* Now that mi, fb, drm and others have done their thing,
	 * complete the DRI setup.
	 */
	pATI->directRenderingEnabled = ATIDRIFinishScreenInit(pScreen);
    }
    if (pATI->directRenderingEnabled) {
	xf86DrvMsg(pScreenInfo->scrnIndex, X_INFO,
		   "Direct rendering enabled\n");
    } else {
        /* FIXME: Release unused offscreen mem here? */
	xf86DrvMsg(pScreenInfo->scrnIndex, X_INFO,
		   "Direct rendering disabled\n");
    }

#endif /* XF86DRI_DEVEL */

    return TRUE;
}

/*
 * ATICloseScreen --
 *
 * This function is called by DIX to close the screen.
 */
Bool
ATICloseScreen (CLOSE_SCREEN_ARGS_DECL)
{
    ScrnInfoPtr pScreenInfo = xf86ScreenToScrn(pScreen);
    ATIPtr      pATI        = ATIPTR(pScreenInfo);

#ifdef XF86DRI_DEVEL

    /* Disable direct rendering */
    if (pATI->directRenderingEnabled)
    {
	ATIDRICloseScreen(pScreen);
	pATI->directRenderingEnabled = FALSE;
    }

#endif /* XF86DRI_DEVEL */

    ATICloseXVideo(pScreen, pScreenInfo, pATI);

#ifdef USE_EXA
    if (pATI->pExa)
    {
        exaDriverFini(pScreen);
        free(pATI->pExa);
        pATI->pExa = NULL;
    }
#endif
#ifdef USE_XAA
    if (pATI->pXAAInfo)
    {
        XAADestroyInfoRec(pATI->pXAAInfo);
        pATI->pXAAInfo = NULL;
    }
#endif
    if (pATI->pCursorInfo)
    {
        xf86DestroyCursorInfoRec(pATI->pCursorInfo);
        pATI->pCursorInfo = NULL;
    }

    pATI->Closeable = FALSE;
    ATILeaveGraphics(pScreenInfo, pATI);

#ifdef USE_XAA
    if (!pATI->useEXA)
    {
        free(pATI->ExpansionBitmapScanlinePtr[1]);
        pATI->ExpansionBitmapScanlinePtr[0] = NULL;
        pATI->ExpansionBitmapScanlinePtr[1] = NULL;
    }
#endif

    free(pATI->pShadow);
    pATI->pShadow = NULL;
    pScreenInfo->pScreen = NULL;

    pScreen->CloseScreen = pATI->CloseScreen;
    return (*pScreen->CloseScreen)(CLOSE_SCREEN_ARGS);
}
