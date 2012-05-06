/*
 * Copyright 1997-2003 by Alan Hourihane, North Wales, UK.
 * Copyright (c) 2006, Jesse Barnes <jbarnes@virtuousgeek.org>
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of the authors not be used in
 * advertising or publicity pertaining to distribution of the software without
 * specific, written prior permission.  The authors make no representations
 * about the suitability of this software for any purpose.  It is provided
 * "as is" without express or implied warranty.
 *
 * THE AUTHORS DISCLAIM ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO
 * EVENT SHALL THE AUTHORS BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
 * DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 *
 * Authors:  Alan Hourihane, <alanh@fairlite.demon.co.uk>
 *           Jesse Barnes <jbarnes@virtuousgeek.org>
 *
 * Trident Blade3D EXA support.
 * TODO:
 *   Composite hooks (some ops/arg. combos may not be supported)
 *   Upload/Download from screen (is this even possible with this chip?)
 *   Fast mixed directoion Blts
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "xf86.h"
#include "xf86_OSproc.h"

#include "xf86PciInfo.h"
#include "xf86Pci.h"

#include "exa.h"

#include "trident.h"
#include "trident_regs.h"

#include "xaarop.h"

#undef REPLICATE
#define REPLICATE(r, bpp)					\
{								\
	if (bpp == 16) {					\
		r = ((r & 0xFFFF) << 16) | (r & 0xFFFF);	\
	} else							\
	if (bpp == 8) { 					\
		r &= 0xFF;					\
		r |= (r<<8);					\
		r |= (r<<16);					\
	}							\
}

static int rop_table[16] =
{
   ROP_0,               /* GXclear */
   ROP_DSa,             /* GXand */
   ROP_SDna,            /* GXandReverse */
   ROP_S,               /* GXcopy */
   ROP_DSna,            /* GXandInverted */
   ROP_D,               /* GXnoop */
   ROP_DSx,             /* GXxor */
   ROP_DSo,             /* GXor */
   ROP_DSon,            /* GXnor */
   ROP_DSxn,            /* GXequiv */
   ROP_Dn,              /* GXinvert*/
   ROP_SDno,            /* GXorReverse */
   ROP_Sn,              /* GXcopyInverted */
   ROP_DSno,            /* GXorInverted */
   ROP_DSan,            /* GXnand */
   ROP_1                /* GXset */
};

static int GetCopyROP(int rop)
{
    return rop_table[rop];
}

static unsigned long GetDepth(int depth)
{
    unsigned long ret;

    switch (depth) {
    case 8:
	ret = 0;
	break;
    case 15:
	ret = 5UL << 29; /* 555 */
    case 16:
	ret = 1UL << 29; /* 565 */
	break;
    case 32:
	ret = 2UL << 29;
	break;
    default:
	ret = 0;
	break;
    }
    return ret;
}

static Bool PrepareSolid(PixmapPtr pPixmap, int rop, Pixel planemask,
			 Pixel color)
{
    TRIDENTPtr pTrident =
	TRIDENTPTR(xf86Screens[pPixmap->drawable.pScreen->myNum]);

    REPLICATE(color, pPixmap->drawable.bitsPerPixel);
    BLADE_OUT(GER_FGCOLOR, color);
    BLADE_OUT(GER_ROP, GetCopyROP(rop));
    pTrident->BltScanDirection = 0;

    return TRUE;
}

static void Solid(PixmapPtr pPixmap, int x, int y, int x2, int y2)
{
    TRIDENTPtr pTrident =
	TRIDENTPTR(xf86Screens[pPixmap->drawable.pScreen->myNum]);
    int dst_stride = (pPixmap->drawable.width + 7) / 8;
    int dst_off = exaGetPixmapOffset(pPixmap) / 8;

    BLADE_OUT(GER_DSTBASE0, GetDepth(pPixmap->drawable.bitsPerPixel) |
	      dst_stride << 20 | dst_off);

    BLADE_OUT(GER_DRAW_CMD, GER_OP_LINE | pTrident->BltScanDirection |
	      GER_DRAW_SRC_COLOR | GER_ROP_ENABLE | GER_SRC_CONST);

    BLADE_OUT(GER_DST1, y << 16 | x);
    BLADE_OUT(GER_DST2, ((y2 - 1) & 0xfff) << 16 | ((x2 - 1) & 0xfff));
}

static void DoneSolid(PixmapPtr pPixmap)
{
}

static Bool PrepareCopy(PixmapPtr pSrcPixmap, PixmapPtr pDstPixmap,
			int xdir, int ydir, int alu, Pixel planemask)
{
    TRIDENTPtr pTrident =
	TRIDENTPTR(xf86Screens[pSrcPixmap->drawable.pScreen->myNum]);
    int src_stride = (pSrcPixmap->drawable.width + 7) / 8;
    int src_off = exaGetPixmapOffset(pSrcPixmap) / 8;
    int dst_stride = (pDstPixmap->drawable.width + 7) / 8;
    int dst_off = exaGetPixmapOffset(pDstPixmap) / 8;

    pTrident->BltScanDirection = 0;

    REPLICATE(planemask, pSrcPixmap->drawable.bitsPerPixel);
    if (planemask != (unsigned int)-1) {
	BLADE_OUT(GER_BITMASK, ~planemask);
	pTrident->BltScanDirection |= 1 << 5;
    }

    BLADE_OUT(GER_SRCBASE0, GetDepth(pSrcPixmap->drawable.bitsPerPixel) |
	      src_stride << 20 | src_off);

    BLADE_OUT(GER_DSTBASE0, GetDepth(pDstPixmap->drawable.bitsPerPixel) |
	      dst_stride << 20 | dst_off);

    if ((xdir < 0) || (ydir < 0))
	pTrident->BltScanDirection |= 1 << 1;

    BLADE_OUT(GER_ROP, GetCopyROP(alu));

    return TRUE;
}

static void Copy(PixmapPtr pDstPixmap, int x1, int y1, int x2,
		 int y2, int w, int h)
{
    TRIDENTPtr pTrident =
	TRIDENTPTR(xf86Screens[pDstPixmap->drawable.pScreen->myNum]);

    BLADE_OUT(GER_DRAW_CMD, GER_OP_BLT_HOST | GER_DRAW_SRC_COLOR |
	      GER_ROP_ENABLE | GER_BLT_SRC_FB | pTrident->BltScanDirection);

    if (pTrident->BltScanDirection) {
	BLADE_OUT(GER_SRC1, (y1 + h - 1) << 16 | (x1 + w - 1));
	BLADE_OUT(GER_SRC2, y1 << 16 | x1);
	BLADE_OUT(GER_DST1, (y2 + h - 1) << 16 | (x2 + w - 1));
	BLADE_OUT(GER_DST2, (y2 & 0xfff) << 16 | (x2 & 0xfff));
    } else {
	BLADE_OUT(GER_SRC1, y1 << 16 | x1);
	BLADE_OUT(GER_SRC2, (y1 + h - 1) << 16 | (x1 + w - 1));
	BLADE_OUT(GER_DST1, y2 << 16 | x2);
	BLADE_OUT(GER_DST2, (((y2 + h - 1) & 0xfff) << 16 |
			     ((x2 +w - 1) & 0xfff)));
    }
}

static void DoneCopy(PixmapPtr pDstPixmap)
{
}

/* Composite comes later (if at all) */
static Bool CheckComposite(int op, PicturePtr pSrcPicture,
			   PicturePtr pMaskPicture, PicturePtr pDstPicture)
{
    return 0;
}

static Bool PrepareComposite(int op, PicturePtr pSrcPicture,
			     PicturePtr pMaskPicture, PicturePtr pDstPicture,
			     PixmapPtr pSrc, PixmapPtr pMask, PixmapPtr pDst)
{
    return 0;
}

static void Composite(PixmapPtr pDst, int srcX, int srcY, int maskX,
		      int maskY, int dstX, int dstY, int width,
		      int height)
{
}

static void DoneComposite(PixmapPtr pDst)
{
}

static int MarkSync(ScreenPtr pScreen)
{
    return 0;
}

static void WaitMarker(ScreenPtr pScreen, int marker)
{
    TRIDENTPtr pTrident = TRIDENTPTR(xf86Screens[pScreen->myNum]);
    int busy;
    int cnt = 10000000;

    BLADE_OUT(GER_PATSTYLE, 0); /* Clear pattern & style first? */

    BLADEBUSY(busy);
    while (busy != 0) {
	if (--cnt < 0) {
	    ErrorF("GE timeout\n");
	    BLADE_OUT(GER_CONTROL, GER_CTL_RESET);
	    BLADE_OUT(GER_CONTROL, GER_CTL_RESUME);
	    break;
	}
    	BLADEBUSY(busy);
    }
}

static void BladeInitializeAccelerator(ScrnInfoPtr pScrn)
{
    TRIDENTPtr pTrident = TRIDENTPTR(pScrn);

    BLADE_OUT(GER_DSTBASE0, 0);
    BLADE_OUT(GER_DSTBASE1, 0);
    BLADE_OUT(GER_DSTBASE2, 0);
    BLADE_OUT(GER_DSTBASE3, 0);
    BLADE_OUT(GER_SRCBASE0, 0);
    BLADE_OUT(GER_SRCBASE1, 0);
    BLADE_OUT(GER_SRCBASE2, 0);
    BLADE_OUT(GER_SRCBASE3, 0);
    BLADE_OUT(GER_PATSTYLE, 0);
}

Bool BladeExaInit(ScreenPtr pScreen)
{
    ScrnInfoPtr pScrn = xf86Screens[pScreen->myNum];
    TRIDENTPtr pTrident = TRIDENTPTR(pScrn);
    ExaDriverPtr ExaDriver;

    if (pTrident->NoAccel)
	return FALSE;

    if (!(ExaDriver = exaDriverAlloc())) {
        pTrident->NoAccel = TRUE;
        return FALSE;
    }

    ExaDriver->exa_major = 2;
    ExaDriver->exa_minor = 0;

    pTrident->EXADriverPtr = ExaDriver;

    pTrident->InitializeAccelerator = BladeInitializeAccelerator;
    BladeInitializeAccelerator(pScrn);

    ExaDriver->memoryBase = pTrident->FbBase;
    ExaDriver->memorySize = pScrn->videoRam * 1024;

    ExaDriver->offScreenBase = pScrn->displayWidth * pScrn->virtualY *
	((pScrn->bitsPerPixel + 7) / 8);

    if(ExaDriver->memorySize > ExaDriver->offScreenBase)
	ExaDriver->flags |= EXA_OFFSCREEN_PIXMAPS;
    else {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "Not enough video RAM for "
		   "offscreen memory manager. Xv disabled\n");
	/* disable Xv here... */
    }

    ExaDriver->pixmapOffsetAlign = 32;
    ExaDriver->pixmapPitchAlign = 32;
    ExaDriver->maxX = 2047;
    ExaDriver->maxY = 2047;

    ExaDriver->flags |= EXA_TWO_BITBLT_DIRECTIONS;

    ExaDriver->MarkSync = MarkSync;
    ExaDriver->WaitMarker = WaitMarker;

    /* Solid fill & copy, the bare minimum */
    ExaDriver->PrepareSolid = PrepareSolid;
    ExaDriver->Solid = Solid;
    ExaDriver->DoneSolid = DoneSolid;
    ExaDriver->PrepareCopy = PrepareCopy;
    ExaDriver->Copy = Copy;
    ExaDriver->DoneCopy = DoneCopy;

    /* Composite not done yet */

    return exaDriverInit(pScreen, ExaDriver);
}
