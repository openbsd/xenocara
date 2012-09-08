
/*
Copyright (C) 1994-2000 The XFree86 Project, Inc.  All Rights Reserved.

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FIT-
NESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
XFREE86 PROJECT BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Except as contained in this notice, the name of the XFree86 Project shall not
be used in advertising or otherwise to promote the sale, use or other dealings
in this Software without prior written authorization from the XFree86 Project.
*/

/*
Copyright (C) 1994-2000 The XFree86 Project, Inc.  All Rights Reserved.

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FIT-
NESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
XFREE86 PROJECT BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Except as contained in this notice, the name of the XFree86 Project shall not
be used in advertising or otherwise to promote the sale, use or other dealings
in this Software without prior written authorization from the XFree86 Project.
*/

/*
 * file: s3v_dga.c
 * ported from mga
 *
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "xf86.h"
#include "xf86_OSproc.h"
#include "xf86Pci.h"
#include "s3v.h"

#include "dgaproc.h"


static Bool S3V_OpenFramebuffer(ScrnInfoPtr, char **, unsigned char **, 
					int *, int *, int *);
static Bool S3V_SetMode(ScrnInfoPtr, DGAModePtr);
static int  S3V_GetViewport(ScrnInfoPtr);
static void S3V_SetViewport(ScrnInfoPtr, int, int, int);
#ifdef HAVE_XAA_H
static void S3V_FillRect(ScrnInfoPtr, int, int, int, int, unsigned long);
static void S3V_BlitRect(ScrnInfoPtr, int, int, int, int, int, int);
#endif
/* dummy... */


static
DGAFunctionRec S3V_DGAFuncs = {
   S3V_OpenFramebuffer,
   NULL,
   S3V_SetMode,
   S3V_SetViewport,
   S3V_GetViewport,
   S3VAccelSync,
#ifdef HAVE_XAA_H
   S3V_FillRect,
   S3V_BlitRect,
#else
   NULL, NULL,
#endif
   NULL
   /* dummy... MGA_BlitTransRect */
};


Bool
S3VDGAInit(ScreenPtr pScreen)
{   
   ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
   S3VPtr ps3v = S3VPTR(pScrn);
   DGAModePtr modes = NULL, newmodes = NULL, currentMode;
   DisplayModePtr pMode, firstMode;
   int Bpp = pScrn->bitsPerPixel >> 3;
   int num = 0;
   Bool oneMore;

   PVERB5("	S3VDGAInit\n");
    
   pMode = firstMode = pScrn->modes;

   while(pMode) {
	/* The MGA driver wasn't designed with switching depths in
	   mind.  Subsequently, large chunks of it will probably need
	   to be rewritten to accommodate depth changes in DGA mode */

	if(0 /*pScrn->displayWidth != pMode->HDisplay*/) {
	    newmodes = realloc(modes, (num + 2) * sizeof(DGAModeRec));
	    oneMore = TRUE;
	} else {
	    newmodes = realloc(modes, (num + 1) * sizeof(DGAModeRec));
	    oneMore = FALSE;
	}

	if(!newmodes) {
	   free(modes);
	   return FALSE;
	}
	modes = newmodes;

SECOND_PASS:

	currentMode = modes + num;
	num++;

	currentMode->mode = pMode;
	currentMode->flags = DGA_CONCURRENT_ACCESS | DGA_PIXMAP_AVAILABLE;
#ifdef HAVE_XAA_H
	if(!ps3v->NoAccel)
	   currentMode->flags |= DGA_FILL_RECT | DGA_BLIT_RECT;
#endif
	if(pMode->Flags & V_DBLSCAN)
	   currentMode->flags |= DGA_DOUBLESCAN;
	if(pMode->Flags & V_INTERLACE)
	   currentMode->flags |= DGA_INTERLACED;
	currentMode->byteOrder = pScrn->imageByteOrder;
	currentMode->depth = pScrn->depth;
	currentMode->bitsPerPixel = pScrn->bitsPerPixel;
	currentMode->red_mask = pScrn->mask.red;
	currentMode->green_mask = pScrn->mask.green;
	currentMode->blue_mask = pScrn->mask.blue;
	currentMode->visualClass = (Bpp == 1) ? PseudoColor : TrueColor;
	currentMode->viewportWidth = pMode->HDisplay;
	currentMode->viewportHeight = pMode->VDisplay;
	/* currentMode->xViewportStep = (3 - ps3v->BppShift); */
				/* always 1 on ViRGE ? */
	currentMode->xViewportStep = 1;
	currentMode->yViewportStep = 1;
	currentMode->viewportFlags = DGA_FLIP_RETRACE;
	/* currentMode->offset = ps3v->YDstOrg * (pScrn->bitsPerPixel / 8);
	 * MGA, 0 for ViRGE */
	currentMode->offset = 0;
	/* currentMode->address = pMga->FbStart;   MGA */
	currentMode->address = ps3v->FBBase;
/*cep*/
  xf86ErrorFVerb(VERBLEV, 
	"	S3VDGAInit firstone vpWid=%d, vpHgt=%d, Bpp=%d, mdbitsPP=%d\n",
		currentMode->viewportWidth,
		currentMode->viewportHeight,
		Bpp,
		currentMode->bitsPerPixel
		 );
		

	if(oneMore) { /* first one is narrow width */
	    currentMode->bytesPerScanline = ((pMode->HDisplay * Bpp) + 3) & ~3L;
	    currentMode->imageWidth = pMode->HDisplay;
	    /* currentMode->imageHeight =  pMga->FbUsableSize /
					currentMode->bytesPerScanline; 
					MGA above */
	    currentMode->imageHeight =  pMode->VDisplay;
	    currentMode->pixmapWidth = currentMode->imageWidth;
	    currentMode->pixmapHeight = currentMode->imageHeight;
	    currentMode->maxViewportX = currentMode->imageWidth - 
					currentMode->viewportWidth;
	    /* this might need to get clamped to some maximum */
	    currentMode->maxViewportY = currentMode->imageHeight -
					currentMode->viewportHeight;
	    oneMore = FALSE;

/*cep*/
  xf86ErrorFVerb(VERBLEV, 
	"	S3VDGAInit imgHgt=%d, ram=%d, bytesPerScanl=%d\n",
		currentMode->imageHeight,
		ps3v->videoRambytes,
		currentMode->bytesPerScanline );
		
	    goto SECOND_PASS;
	} else {
	    currentMode->bytesPerScanline = 
			((pScrn->displayWidth * Bpp) + 3) & ~3L;
	    currentMode->imageWidth = pScrn->displayWidth;
	    /* currentMode->imageHeight =  pMga->FbUsableSize /
					currentMode->bytesPerScanline; 
					*/
	    currentMode->imageHeight =  ps3v->videoRambytes /
	    			currentMode->bytesPerScanline;
	    currentMode->pixmapWidth = currentMode->imageWidth;
	    currentMode->pixmapHeight = currentMode->imageHeight;
	    currentMode->maxViewportX = currentMode->imageWidth - 
					currentMode->viewportWidth;
	    /* this might need to get clamped to some maximum */
	    currentMode->maxViewportY = currentMode->imageHeight -
					currentMode->viewportHeight;
	}	    

	pMode = pMode->next;
	if(pMode == firstMode)
	   break;
   }

   ps3v->numDGAModes = num;
   ps3v->DGAModes = modes;

   return DGAInit(pScreen, &S3V_DGAFuncs, modes, num);  
}


static Bool
S3V_SetMode(
   ScrnInfoPtr pScrn,
   DGAModePtr pMode
){
   static int OldDisplayWidth[MAXSCREENS];
   int index = pScrn->pScreen->myNum;

   S3VPtr ps3v = S3VPTR(pScrn);

   if(!pMode) { /* restore the original mode */
	/* put the ScreenParameters back */
	
	pScrn->displayWidth = OldDisplayWidth[index];
	
        S3VSwitchMode(SWITCH_MODE_ARGS(pScrn, pScrn->currentMode));
	ps3v->DGAactive = FALSE;
   } else {
	if(!ps3v->DGAactive) {  /* save the old parameters */
	    OldDisplayWidth[index] = pScrn->displayWidth;

	    ps3v->DGAactive = TRUE;
	}

	pScrn->displayWidth = pMode->bytesPerScanline / 
			      (pMode->bitsPerPixel >> 3);

        S3VSwitchMode(SWITCH_MODE_ARGS(pScrn, pMode->mode));
   }
   
   return TRUE;
}



static int  
S3V_GetViewport(
  ScrnInfoPtr pScrn
){
    S3VPtr ps3v = S3VPTR(pScrn);

    return ps3v->DGAViewportStatus;
}

static void 
S3V_SetViewport(
   ScrnInfoPtr pScrn, 
   int x, int y, 
   int flags
){
   S3VPtr ps3v = S3VPTR(pScrn);

   S3VAdjustFrame(ADJUST_FRAME_ARGS(pScrn, x, y));
   ps3v->DGAViewportStatus = 0;  /* MGAAdjustFrame loops until finished */
}

#ifdef HAVE_XAA_H
static void 
S3V_FillRect (
   ScrnInfoPtr pScrn, 
   int x, int y, int w, int h, 
   unsigned long color
){
    S3VPtr ps3v = S3VPTR(pScrn);

    if(ps3v->AccelInfoRec) {
	(*ps3v->AccelInfoRec->SetupForSolidFill)(pScrn, color, GXcopy, ~0);
	(*ps3v->AccelInfoRec->SubsequentSolidFillRect)(pScrn, x, y, w, h);
	SET_SYNC_FLAG(ps3v->AccelInfoRec);
    }
}

static void 
S3V_BlitRect(
   ScrnInfoPtr pScrn, 
   int srcx, int srcy, 
   int w, int h, 
   int dstx, int dsty
){
    S3VPtr ps3v = S3VPTR(pScrn);

    if(ps3v->AccelInfoRec) {
	int xdir = ((srcx < dstx) && (srcy == dsty)) ? -1 : 1;
	int ydir = (srcy < dsty) ? -1 : 1;

	(*ps3v->AccelInfoRec->SetupForScreenToScreenCopy)(
		pScrn, xdir, ydir, GXcopy, ~0, -1);
	(*ps3v->AccelInfoRec->SubsequentScreenToScreenCopy)(
		pScrn, srcx, srcy, dstx, dsty, w, h);
	SET_SYNC_FLAG(ps3v->AccelInfoRec);
    }
}
#endif


static Bool 
S3V_OpenFramebuffer(
   ScrnInfoPtr pScrn, 
   char **name,
   unsigned char **mem,
   int *size,
   int *offset,
   int *flags
){
    S3VPtr ps3v = S3VPTR(pScrn);

    *name = NULL; 		/* no special device */
    *mem = (unsigned char*)PCI_REGION_BASE(ps3v->PciInfo, 0, REGION_MEM);
    *size = ps3v->videoRambytes;
    *offset = 0;
    *flags = DGA_NEED_ROOT;

    return TRUE;
}
