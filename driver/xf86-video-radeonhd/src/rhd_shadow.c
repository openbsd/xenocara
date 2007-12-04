/*
 * Copyright 2007  Luc Verhaegen <lverhaegen@novell.com>
 * Copyright 2007  Matthias Hopf <mhopf@novell.com>
 * Copyright 2007  Egbert Eich   <eich@novell.com>
 * Copyright 2007  Advanced Micro Devices, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE COPYRIGHT HOLDER(S) OR AUTHOR(S) BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "scrnintstr.h"
#include "shadow.h"
#include "fb.h"
#include "xf86.h"
#include "rhd.h"

typedef struct _rhdShadowRec
{
    unsigned char *shadow;
    CreateScreenResourcesProcPtr CreateScreenResources;
} rhdShadowRec;

Bool
RHDShadowPreInit(ScrnInfoPtr pScrn)
{
    RHDPtr rhdPtr = RHDPTR(pScrn);

    RHDFUNC(pScrn);

    if (!rhdPtr->shadowFB.val.bool)
	return FALSE;

    if (!xf86LoadSubModule(pScrn, "shadow")) {
	return FALSE;
    }
    if (!(rhdPtr->shadowPtr = (rhdShadowPtr)xalloc(sizeof(rhdShadowRec))))
	return FALSE;

    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Using ShadowFB\n");

    return TRUE;
}

static void *
rhdShadowWindow(ScreenPtr pScreen, CARD32 row, CARD32 offset, int mode,
	  CARD32 *size, void *closure)
{
    ScrnInfoPtr pScrn = xf86Screens[pScreen->myNum];
    RHDPtr rhdPtr = RHDPTR(pScrn);

    DEBUGP(RHDDebugVerb(pScrn->scrnIndex, 1, "FUNCTION: %s row: %i\n",__func__,row));

    *size = pScrn->displayWidth * (pScrn->bitsPerPixel >> 3);
    return ((CARD8 *)rhdPtr->FbBase + rhdPtr->FbFreeStart
	    + row * (*size) + offset);
}

static Bool
rhdShadowCreateScreenResources(ScreenPtr pScreen)
{
    ScrnInfoPtr pScrn = xf86Screens[pScreen->myNum];
    RHDPtr rhdPtr = RHDPTR(pScrn);
    rhdShadowPtr shadowPtr = rhdPtr->shadowPtr;
    Bool ret;
    shadowUpdateProc update;

    RHDFUNC(pScrn);

    update = shadowUpdatePackedWeak();
    pScreen->CreateScreenResources = shadowPtr->CreateScreenResources;
    ret = pScreen->CreateScreenResources(pScreen);
    pScreen->CreateScreenResources = rhdShadowCreateScreenResources;

    shadowAdd(pScreen, pScreen->GetScreenPixmap(pScreen),
	      update,
	      rhdShadowWindow, 0, 0);

    return ret;
}

Bool
RHDShadowScreenInit(ScreenPtr pScreen)
{
    ScrnInfoPtr pScrn = xf86Screens[pScreen->myNum];
    RHDPtr rhdPtr = RHDPTR(pScrn);
    rhdShadowPtr shadowPtr = rhdPtr->shadowPtr;
    Bool ret;

    RHDFUNC(pScrn);

    if (shadowPtr) {
	if (!(shadowPtr->shadow
	      = xcalloc(1, pScrn->displayWidth * pScrn->virtualY
			* ((pScrn->bitsPerPixel + 7) >> 3)))) {
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		       "Failed to allocate shadow FB buffer\n");
	    return FALSE;
	}
	ret =  fbScreenInit(pScreen,
			    (CARD8 *) shadowPtr->shadow,
			    pScrn->virtualX, pScrn->virtualY,
			    pScrn->xDpi, pScrn->yDpi,
			    pScrn->displayWidth, pScrn->bitsPerPixel);
	if (!ret) {
	    xfree(shadowPtr->shadow);
	    shadowPtr->shadow = NULL;
	}
	return ret;
    }
    return FALSE;
}

Bool
RHDShadowSetup(ScreenPtr pScreen)
{
    ScrnInfoPtr pScrn = xf86Screens[pScreen->myNum];
    RHDPtr rhdPtr = RHDPTR(pScrn);
    rhdShadowPtr shadowPtr = rhdPtr->shadowPtr;

    RHDFUNC(pScrn);

    if (!shadowPtr || !shadowPtr->shadow)
	return TRUE;

    if (!shadowSetup(pScreen))
	return FALSE;

    shadowPtr->CreateScreenResources = pScreen->CreateScreenResources;
    pScreen->CreateScreenResources = rhdShadowCreateScreenResources;

    return TRUE;
}

Bool
RHDShadowCloseScreen(ScreenPtr pScreen)
{
    ScrnInfoPtr pScrn = xf86Screens[pScreen->myNum];
    RHDPtr rhdPtr = RHDPTR(pScrn);
    rhdShadowPtr shadowPtr = rhdPtr->shadowPtr;

    RHDFUNC(pScrn);

    if (!shadowPtr)
	return TRUE;

    xfree(shadowPtr->shadow);
    shadowPtr->shadow = NULL;

    return TRUE;
}

void
RHDShadowDestroy(RHDPtr rhdPtr)
{
    RHDFUNC(rhdPtr);

    if (rhdPtr->shadowPtr) {
	xfree(rhdPtr->shadowPtr);
	rhdPtr->shadowPtr = NULL;
    }
}
