/*
Copyright (C) 2006 Dennis De Winter  All Rights Reserved.
Copyright (C) 2007 Alex Deucher  All Rights Reserved.

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

*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "smi.h"

#if SMI501_CLI_DEBUG
# include "smi_501.h"
# undef WRITE_DPR
# define WRITE_DPR(pSmi, dpr, data)					\
    do {								\
	if (pSmi->batch_active)						\
	    BATCH_LOAD_REG((pSmi->DPRBase - pSmi->MapBase) +		\
			   dpr, data);					\
	else								\
	    MMIO_OUT32(pSmi->DPRBase, dpr, data);			\
	DEBUG("DPR%02X = %08X\n", dpr, data);				\
    } while (0)
#endif

static void
SMI_EXASync(ScreenPtr pScreen, int marker);

static Bool
SMI_PrepareCopy(PixmapPtr pSrcPixmap, PixmapPtr pDstPixmap,
		int xdir, int ydir, int alu, Pixel planemask);

static void
SMI_Copy(PixmapPtr pDstPixmap, int srcX, int srcY, int dstX, int dstY, int width, int height);

static void
SMI_DoneCopy(PixmapPtr pDstPixmap);

static Bool
SMI_PrepareSolid(PixmapPtr pPixmap, int alu, Pixel planemask, Pixel fg);

static void
SMI_Solid(PixmapPtr pPixmap, int x1, int y1, int x2, int y2);

static void
SMI_DoneSolid(PixmapPtr pPixmap);

Bool
SMI_UploadToScreen(PixmapPtr pDst, int x, int y, int w, int h, char *src, int src_pitch);

Bool
SMI_DownloadFromScreen(PixmapPtr pSrc, int x, int y, int w, int h, char *dst, int dst_pitch);

static Bool
SMI_CheckComposite(int op, PicturePtr pSrcPicture, PicturePtr pMaskPicture, PicturePtr pDstPicture);
static Bool
SMI_PrepareComposite(int op, PicturePtr pSrcPicture, PicturePtr pMaskPicture, PicturePtr pDstPicture,
                     PixmapPtr pSrc, PixmapPtr pMask, PixmapPtr pDst);
static void
SMI_Composite(PixmapPtr pDst, int srcX, int srcY, int maskX, int maskY,
              int dstX, int dstY, int width, int height);
static void
SMI730_Composite(PixmapPtr pDst, int srcX, int srcY, int maskX, int maskY,
              int dstX, int dstY, int width, int height);
static void
SMI_DoneComposite(PixmapPtr pDst);


#define PIXMAP_FORMAT(pixmap) SMI_DEDataFormat(pixmap->drawable.bitsPerPixel)
#define PIXMAP_OFFSET(pixmap)	IS_MSOC(pSmi) ?				\
    exaGetPixmapOffset(pixmap) : exaGetPixmapOffset(pixmap) >> 3

Bool
SMI_EXAInit(ScreenPtr pScreen)
{
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
    SMIPtr pSmi = SMIPTR(pScrn);
	
    ENTER();

    if (!(pSmi->EXADriverPtr = exaDriverAlloc())) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "Failed to allocate EXADriverRec.\n");
	LEAVE(FALSE);
    }

    /* Require 2.1 semantics:
       Don't uninitialize the memory manager when swapping out */
    pSmi->EXADriverPtr->exa_major = 2;
    pSmi->EXADriverPtr->exa_minor = 1;

    SMI_EngineReset(pScrn);

    /* Memory Manager */
    pSmi->EXADriverPtr->memoryBase = pSmi->FBBase;
    pSmi->EXADriverPtr->memorySize = pSmi->FBReserved;

    /* The framebuffer is allocated as an offscreen area with the
       memory manager (It makes easier further resizing) */
    pSmi->EXADriverPtr->offScreenBase = 0;

    /* Flags */
    pSmi->EXADriverPtr->flags = EXA_TWO_BITBLT_DIRECTIONS;
    if (pSmi->EXADriverPtr->memorySize > pSmi->EXADriverPtr->offScreenBase) {
	/* Offscreen Pixmaps */
	pSmi->EXADriverPtr->flags |= EXA_OFFSCREEN_PIXMAPS;
	xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		   "EXA offscreen memory manager enabled.\n");
    }
    else
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		   "Not enough video RAM for EXA offscreen memory manager.\n");

    /* 12 bit coordinates */
    pSmi->EXADriverPtr->maxX = 4096;
    pSmi->EXADriverPtr->maxY = 4096;

    if (pScrn->bitsPerPixel == 24) {
	pSmi->EXADriverPtr->maxX = 4096 / 3;

	if (pSmi->Chipset == SMI_LYNX) {
	    pSmi->EXADriverPtr->maxY = 4096 / 3;
	}
    }

    pSmi->EXADriverPtr->pixmapPitchAlign  = 16;
    pSmi->EXADriverPtr->pixmapOffsetAlign = 8;

    /* Sync */
    pSmi->EXADriverPtr->WaitMarker = SMI_EXASync;

    /* Copy */
    pSmi->EXADriverPtr->PrepareCopy = SMI_PrepareCopy;
    pSmi->EXADriverPtr->Copy = SMI_Copy;
    pSmi->EXADriverPtr->DoneCopy = SMI_DoneCopy;

    /* Solid */
    pSmi->EXADriverPtr->PrepareSolid = SMI_PrepareSolid;
    pSmi->EXADriverPtr->Solid = SMI_Solid;
    pSmi->EXADriverPtr->DoneSolid = SMI_DoneSolid;

#if 0
    /* DFS & UTS */
    pSmi->EXADriverPtr->UploadToScreen = SMI_UploadToScreen;
    pSmi->EXADriverPtr->DownloadFromScreen = SMI_DownloadFromScreen;
#endif

    /* Composite */
    pSmi->EXADriverPtr->CheckComposite = SMI_CheckComposite;
    pSmi->EXADriverPtr->PrepareComposite = SMI_PrepareComposite;

    if (IS_MSOC(pSmi) || pSmi->Chipset == SMI_COUGAR3DR)
	pSmi->EXADriverPtr->Composite = SMI730_Composite;
    else
	pSmi->EXADriverPtr->Composite = SMI_Composite;

    pSmi->EXADriverPtr->DoneComposite = SMI_DoneComposite;

    if(!exaDriverInit(pScreen, pSmi->EXADriverPtr)) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "exaDriverInit failed.\n");
	LEAVE(FALSE);
    }


    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "EXA Acceleration enabled.\n");

    LEAVE(TRUE);
}

static void
SMI_EXASync(ScreenPtr pScreen, int marker)
{
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);

    ENTER();

    SMI_AccelSync(pScrn);

    LEAVE();
}

/* ----------------------------------------------------- EXA Copy ---------------------------------------------- */

CARD8 SMI_BltRop[16] =	/* table stolen from KAA */
{
    /* GXclear      */      0x00,         /* 0 */
    /* GXand        */      0x88,         /* src AND dst */
    /* GXandReverse */      0x44,         /* src AND NOT dst */
    /* GXcopy       */      0xCC,         /* src */
    /* GXandInverted*/      0x22,         /* NOT src AND dst */
    /* GXnoop       */      0xAA,         /* dst */
    /* GXxor        */      0x66,         /* src XOR dst */
    /* GXor         */      0xEE,         /* src OR dst */
    /* GXnor        */      0x11,         /* NOT src AND NOT dst */
    /* GXequiv      */      0x99,         /* NOT src XOR dst */
    /* GXinvert     */      0x55,         /* NOT dst */
    /* GXorReverse  */      0xDD,         /* src OR NOT dst */
    /* GXcopyInverted*/     0x33,         /* NOT src */
    /* GXorInverted */      0xBB,         /* NOT src OR dst */
    /* GXnand       */      0x77,         /* NOT src OR NOT dst */
    /* GXset        */      0xFF,         /* 1 */
};

static Bool
SMI_PrepareCopy(PixmapPtr pSrcPixmap, PixmapPtr pDstPixmap, int xdir, int ydir,
		int alu, Pixel planemask)
{
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pDstPixmap->drawable.pScreen);
    SMIPtr pSmi = SMIPTR(pScrn);
    int src_pitch, dst_pitch;
    unsigned long src_offset, dst_offset;

    ENTER();
    DEBUG("xdir=%d ydir=%d alu=%02X", xdir, ydir, alu);

    /* Bit Mask not supported > 16 bpp */
    if ((pSrcPixmap->drawable.bitsPerPixel > 16) && 
	(!EXA_PM_IS_SOLID(&pSrcPixmap->drawable, planemask)))
	LEAVE(FALSE);

    /* calculate pitch in pixel unit */
    src_pitch  = exaGetPixmapPitch(pSrcPixmap) / (pSrcPixmap->drawable.bitsPerPixel >> 3);
    dst_pitch  = exaGetPixmapPitch(pDstPixmap) / (pDstPixmap->drawable.bitsPerPixel >> 3);
    /* calculate offset in 8 byte (64 bit) unit */
    src_offset = PIXMAP_OFFSET(pSrcPixmap);
    dst_offset = PIXMAP_OFFSET(pDstPixmap);

    pSmi->AccelCmd = SMI_BltRop[alu]
		   | SMI_BITBLT
		   | SMI_QUICK_START;

    if (xdir < 0 || (ydir < 0)) {
	pSmi->AccelCmd |= SMI_RIGHT_TO_LEFT;
    }

    if (pDstPixmap->drawable.bitsPerPixel == 24) {
	src_pitch *= 3;
	dst_pitch *= 3;
    }

#if SMI501_CLI_DEBUG
    BATCH_BEGIN(7);
#else
    WaitQueue();
#endif
    /* Destination and Source Window Widths */
    WRITE_DPR(pSmi, 0x3C, (dst_pitch << 16) | (src_pitch & 0xFFFF));
    /* Destination and Source Row Pitch */
    WRITE_DPR(pSmi, 0x10, (dst_pitch << 16) | (src_pitch & 0xFFFF));

    /* Bit Mask (planemask) - 16 bit only */
    if (pSrcPixmap->drawable.bitsPerPixel == 16) {
	WRITE_DPR(pSmi, 0x28, planemask | 0xFFFF0000);
    } else {
	WRITE_DPR(pSmi, 0x28, 0xFFFFFFFF);
    }
    /* Drawing engine data format */
    WRITE_DPR(pSmi, 0x1C, PIXMAP_FORMAT(pDstPixmap));
    /* Destination and Source Base Address (offset) */
    WRITE_DPR(pSmi, 0x40, src_offset);
    WRITE_DPR(pSmi, 0x44, dst_offset);

    WRITE_DPR(pSmi, 0x0C, pSmi->AccelCmd);
#if SMI501_CLI_DEBUG
    BATCH_END();
#endif

    LEAVE(TRUE);
}

static void
SMI_Copy(PixmapPtr pDstPixmap, int srcX, int srcY, int dstX,
	 int dstY, int width, int height)
{
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pDstPixmap->drawable.pScreen);
    SMIPtr pSmi = SMIPTR(pScrn);

    ENTER();
    DEBUG("srcX=%d srcY=%d dstX=%d dstY=%d width=%d height=%d\n",
	  srcX, srcY, dstX, dstY, width, height);

    if (pSmi->AccelCmd & SMI_RIGHT_TO_LEFT) {
	srcX += width  - 1;
	srcY += height - 1;
	dstX += width  - 1;
	dstY += height - 1;
    }

    if (pDstPixmap->drawable.bitsPerPixel == 24) {
	srcX  *= 3;
	dstX  *= 3;
	width *= 3;

    	if (pSmi->Chipset == SMI_LYNX) {
	    srcY *= 3;
	    dstY *= 3;
        }

	if (pSmi->AccelCmd & SMI_RIGHT_TO_LEFT) {
	    srcX += 2;
	    dstX += 2;
	}
    }

#if SMI501_CLI_DEBUG
    BATCH_BEGIN(3);
#else
    WaitQueue();
#endif
    WRITE_DPR(pSmi, 0x00, (srcX  << 16) + (srcY & 0xFFFF));
    WRITE_DPR(pSmi, 0x04, (dstX  << 16) + (dstY & 0xFFFF));
    WRITE_DPR(pSmi, 0x08, (width << 16) + (height & 0xFFFF));
#if SMI501_CLI_DEBUG
    BATCH_END();
#endif

    LEAVE();
}

static void
SMI_DoneCopy(PixmapPtr pDstPixmap)
{
    ENTER();

    LEAVE();
}

/* ----------------------------------------------------- EXA Solid --------------------------------------------- */

CARD8 SMI_SolidRop[16] =	/* table stolen from KAA */
{
    /* GXclear      */      0x00,         /* 0 */
    /* GXand        */      0xA0,         /* src AND dst */
    /* GXandReverse */      0x50,         /* src AND NOT dst */
    /* GXcopy       */      0xF0,         /* src */
    /* GXandInverted*/      0x0A,         /* NOT src AND dst */
    /* GXnoop       */      0xAA,         /* dst */
    /* GXxor        */      0x5A,         /* src XOR dst */
    /* GXor         */      0xFA,         /* src OR dst */
    /* GXnor        */      0x05,         /* NOT src AND NOT dst */
    /* GXequiv      */      0xA5,         /* NOT src XOR dst */
    /* GXinvert     */      0x55,         /* NOT dst */
    /* GXorReverse  */      0xF5,         /* src OR NOT dst */
    /* GXcopyInverted*/     0x0F,         /* NOT src */
    /* GXorInverted */      0xAF,         /* NOT src OR dst */
    /* GXnand       */      0x5F,         /* NOT src OR NOT dst */
    /* GXset        */      0xFF,         /* 1 */
};

static Bool
SMI_PrepareSolid(PixmapPtr pPixmap, int alu, Pixel planemask, Pixel fg)
{
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pPixmap->drawable.pScreen);
    SMIPtr pSmi = SMIPTR(pScrn);
    int dst_pitch;
    unsigned long dst_offset;

    ENTER();
    DEBUG("alu=%02X\n", alu);

    /* HW ignores alpha */
    if (pPixmap->drawable.bitsPerPixel == 32)
	LEAVE(FALSE);

    /* Bit Mask not supported > 16 bpp */
    if ((pPixmap->drawable.bitsPerPixel > 16) && 
	(!EXA_PM_IS_SOLID(&pPixmap->drawable, planemask)))
	LEAVE(FALSE);

    /* calculate pitch in pixel unit */
    dst_pitch  = exaGetPixmapPitch(pPixmap) / (pPixmap->drawable.bitsPerPixel >> 3);
    /* calculate offset in 8 byte (64 bit) unit */
    dst_offset = PIXMAP_OFFSET(pPixmap);

    pSmi->AccelCmd = SMI_SolidRop[alu]
		   | SMI_BITBLT
		   | SMI_QUICK_START;

    if (pPixmap->drawable.bitsPerPixel == 24) {
	dst_pitch *= 3;
    }

#if SMI501_CLI_DEBUG
    BATCH_BEGIN(10);
#else
    WaitQueue();
#endif

    /* Destination Window Width */
    WRITE_DPR(pSmi, 0x3C, (dst_pitch << 16) | (dst_pitch & 0xFFFF));
    /* Destination Row Pitch */
    WRITE_DPR(pSmi, 0x10, (dst_pitch << 16) | (dst_pitch & 0xFFFF));

    /* Bit Mask (planemask) - 16 bit only */
    if (pPixmap->drawable.bitsPerPixel == 16) {
	WRITE_DPR(pSmi, 0x28, planemask | 0xFFFF0000);
    } else {
	WRITE_DPR(pSmi, 0x28, 0xFFFFFFFF);
    }

    /* Drawing engine data format */
    WRITE_DPR(pSmi, 0x1C, PIXMAP_FORMAT(pPixmap));
    /* Source and Destination Base Address (offset) */
    WRITE_DPR(pSmi, 0x40, dst_offset);
    WRITE_DPR(pSmi, 0x44, dst_offset);
    /* Foreground Color */
    WRITE_DPR(pSmi, 0x14, fg);
    /* Mono Pattern High and Low */
    WRITE_DPR(pSmi, 0x34, 0xFFFFFFFF);
    WRITE_DPR(pSmi, 0x38, 0xFFFFFFFF);

    WRITE_DPR(pSmi, 0x0C, pSmi->AccelCmd);
#if SMI501_CLI_DEBUG
    BATCH_END();
#endif

    LEAVE(TRUE);
}

static void
SMI_Solid(PixmapPtr pPixmap, int x1, int y1, int x2, int y2)
{
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pPixmap->drawable.pScreen);
    SMIPtr pSmi = SMIPTR(pScrn);
    int w, h;

    ENTER();
    DEBUG("x1=%d y1=%d x2=%d y2=%d\n", x1, y1, x2, y2);

    w = (x2 - x1);
    h = (y2 - y1);

    if (pPixmap->drawable.bitsPerPixel == 24) {
	x1 *= 3;
	w  *= 3;

	if (pSmi->Chipset == SMI_LYNX) {
	    y1 *= 3;
	}
    }

#if SMI501_CLI_DEBUG
    BATCH_BEGIN(2);
#else
    WaitQueue();
#endif
    WRITE_DPR(pSmi, 0x04, (x1 << 16) | (y1 & 0xFFFF));
    WRITE_DPR(pSmi, 0x08, (w  << 16) | (h  & 0xFFFF));
#if SMI501_CLI_DEBUG
    BATCH_END();
#endif

    LEAVE();
}

static void
SMI_DoneSolid(PixmapPtr pPixmap)
{
    ENTER();

    LEAVE();
}

/* --------------------------------------- EXA DFS & UTS ---------------------------------------- */

Bool
SMI_DownloadFromScreen(PixmapPtr pSrc, int x, int y, int w, int h,
		       char *dst, int dst_pitch)
{
    unsigned char *src = pSrc->devPrivate.ptr;
    int src_pitch = exaGetPixmapPitch(pSrc);

    ENTER();
    DEBUG("x=%d y=%d w=%d h=%d dst=%d dst_pitch=%d\n",
	  x, y, w, h, dst, dst_pitch);

    exaWaitSync(pSrc->drawable.pScreen);

    src += (y * src_pitch) + (x * pSrc->drawable.bitsPerPixel/8);
    w   *= pSrc->drawable.bitsPerPixel/8;

    while (h--) {
	memcpy(dst, src, w);
	src += src_pitch;
	dst += dst_pitch;
    }

    LEAVE(TRUE);
}

Bool
SMI_UploadToScreen(PixmapPtr pDst, int x, int y, int w, int h,
		   char *src, int src_pitch)
{
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pDst->drawable.pScreen);
    SMIPtr pSmi = SMIPTR(pScrn);
    int dst_pixelpitch, src_pixelpitch, align, aligned_pitch;
    unsigned long dst_offset;

    ENTER();
    DEBUG("x=%d y=%d w=%d h=%d src=%d src_pitch=%d\n",
	  x, y, w, h, src, src_pitch);

    if (pDst->drawable.bitsPerPixel == 24) {
	align = 16;
    } else {
	align = 128 / pDst->drawable.bitsPerPixel;
    }

    aligned_pitch = ((w*pDst->drawable.bitsPerPixel >> 3) + align - 1) & ~(align - 1);

    /* calculate pitch in pixel unit */
    dst_pixelpitch  = exaGetPixmapPitch(pDst) / (pDst->drawable.bitsPerPixel >> 3);
    src_pixelpitch = src_pitch / (pDst->drawable.bitsPerPixel >> 3);
    /* calculate offset in 8 byte (64 bit) unit */
    dst_offset = PIXMAP_OFFSET(pDst);

    pSmi->AccelCmd = 0xCC /* GXcopy */
		   | SMI_HOSTBLT_WRITE
		   | SMI_QUICK_START;

    /* set clipping */
    SMI_SetClippingRectangle(pScrn, x, y, x+w, y+h);

#if SMI501_CLI_DEBUG
    BATCH_BEGIN(9);
#else
    WaitQueue();
#endif
    /* Destination and Source Window Widths */
    WRITE_DPR(pSmi, 0x3C, (dst_pixelpitch << 16) | (src_pixelpitch & 0xFFFF));

    if (pDst->drawable.bitsPerPixel == 24) {
	x *= 3;
	w *= 3;
	dst_pixelpitch *= 3;
	if (pSmi->Chipset == SMI_LYNX) {
	    y *= 3;
	}
    }

    /* Source and Destination Row Pitch */
    WRITE_DPR(pSmi, 0x10, (dst_pixelpitch << 16) | (src_pixelpitch & 0xFFFF));
    /* Drawing engine data format */
    WRITE_DPR(pSmi, 0x1C,PIXMAP_FORMAT(pDst));
    /* Source and Destination Base Address (offset) */
    WRITE_DPR(pSmi, 0x40, 0);
    WRITE_DPR(pSmi, 0x44, dst_offset);

    WRITE_DPR(pSmi, 0x0C, pSmi->AccelCmd);
    WRITE_DPR(pSmi, 0x00, 0);
    WRITE_DPR(pSmi, 0x04, (x << 16) | (y & 0xFFFF));
    WRITE_DPR(pSmi, 0x08, (w << 16) | (h & 0xFFFF));
#if SMI501_CLI_DEBUG
    BATCH_END();
#endif

    while (h--) {
	memcpy(pSmi->DataPortBase, src, aligned_pitch);
	src += src_pitch;
    }

    /* disable clipping */
    SMI_DisableClipping(pScrn);

    exaWaitSync(pDst->drawable.pScreen);

    LEAVE(TRUE);
}

/* --------------------------------------- EXA Composite ---------------------------------------- */
/* This is a very incomplete Composite implementation that only
   accelerates PictOpSrc with source coordinates transformation by
   using 2D Engine rotate-BITBLTs */

#define SMI_ISROTATION_90(t)                                    \
    (t->matrix[0][0] == 0 && t->matrix[0][1] == xFixed1 &&      \
     t->matrix[1][0] == -xFixed1 && t->matrix[1][1] == 0)

#define SMI_ISROTATION_270(t)                                   \
    (t->matrix[0][0] == 0 && t->matrix[0][1] == -xFixed1 &&     \
     t->matrix[1][0] == xFixed1 && t->matrix[1][1] == 0)

static Bool
SMI_CheckComposite(int op, PicturePtr pSrcPicture, PicturePtr pMaskPicture, PicturePtr pDstPicture)
{
    ENTER();

    if(op!=PictOpSrc || pMaskPicture ||
       pSrcPicture->repeatType || !pSrcPicture->transform)
	LEAVE(FALSE);

    if(!SMI_ISROTATION_90(pSrcPicture->transform) &&
       !SMI_ISROTATION_270(pSrcPicture->transform))
        LEAVE(FALSE);

    if(PICT_FORMAT_BPP(pSrcPicture->format) == 24)
	LEAVE(FALSE);

    LEAVE(TRUE);
}

static Bool
SMI_PrepareComposite(int op, PicturePtr pSrcPicture, PicturePtr pMaskPicture, PicturePtr pDstPicture,
		       PixmapPtr pSrc, PixmapPtr pMask, PixmapPtr pDst)
{
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pDst->drawable.pScreen);
    SMIPtr pSmi = SMIPTR(pScrn);

    if (!pSrc) return FALSE;
    if (!pSrcPicture->pDrawable) return FALSE;

    int src_pitch = exaGetPixmapPitch(pSrc) / (pSrc->drawable.bitsPerPixel >> 3);
    int dst_pitch = exaGetPixmapPitch(pDst) / (pDst->drawable.bitsPerPixel >> 3);

    ENTER();

#if SMI501_CLI_DEBUG
    BATCH_BEGIN(7);
#else
    WaitQueue();
#endif

    /* Destination and Source Window Widths */
    WRITE_DPR(pSmi, 0x3C, (dst_pitch << 16) | (src_pitch & 0xFFFF));

    /* Destination and Source Row Pitch */
    WRITE_DPR(pSmi, 0x10, (dst_pitch << 16) | (src_pitch & 0xFFFF));

    /* Drawing engine data format */
    WRITE_DPR(pSmi, 0x1C, PIXMAP_FORMAT(pDst));

    /* DE Bit Mask */
    WRITE_DPR(pSmi, 0x28, 0xFFFFFFFF);

    /* Destination and Source Base Address (offset) */
    WRITE_DPR(pSmi, 0x40, PIXMAP_OFFSET(pSrc));
    WRITE_DPR(pSmi, 0x44, PIXMAP_OFFSET(pDst));

    /* DE command*/
    if(SMI_ISROTATION_90(pSrcPicture->transform))
        WRITE_DPR(pSmi, 0x0C, 0xCC /*GXCopy*/ | SMI_ROTATE_BLT |
		    SMI_ROTATE_CW | SMI_QUICK_START);
    else
        WRITE_DPR(pSmi, 0x0C, 0xCC /*GXCopy*/ | SMI_ROTATE_BLT |
		    SMI_ROTATE_CCW | SMI_QUICK_START);

#if SMI501_CLI_DEBUG
    BATCH_END();
#endif

    pSmi->renderTransform = pSrcPicture->transform;

    LEAVE(TRUE);
}

static void
SMI_Composite(PixmapPtr pDst, int srcX, int srcY, int maskX, int maskY,
		int dstX, int dstY, int width, int height)
{
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pDst->drawable.pScreen);
    SMIPtr pSmi = SMIPTR(pScrn);
    PictTransformPtr t = pSmi->renderTransform;
    PictVector v;

    ENTER();

    if(SMI_ISROTATION_90(t)){
        srcX=srcX+width;
        dstX=dstX+width-1;
    }else{
        srcY=srcY+height;
        dstY=dstY+height-1;
    }

    v.vector[0] = IntToxFixed(srcX);
    v.vector[1] = IntToxFixed(srcY);
    v.vector[2] = xFixed1;
    PictureTransformPoint(t, &v);

#if SMI501_CLI_DEBUG
    BATCH_BEGIN(3);
#else
    WaitQueue();
#endif

    WRITE_DPR(pSmi, 0x00, (xFixedToInt(v.vector[0]) << 16) + (xFixedToInt(v.vector[1]) & 0xFFFF));
    WRITE_DPR(pSmi, 0x04, (dstX << 16) + (dstY & 0xFFFF));
    WRITE_DPR(pSmi, 0x08, (height << 16) + (width & 0xFFFF));
#if SMI501_CLI_DEBUG
    BATCH_END();
#endif

    LEAVE();
}

static void
SMI730_Composite(PixmapPtr pDst, int srcX, int srcY, int maskX, int maskY,
		int dstX, int dstY, int width, int height)
{
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pDst->drawable.pScreen);
    SMIPtr pSmi = SMIPTR(pScrn);
    int maxPixels;

    ENTER();

    /* Both SM501 and SM731 cannot rotate-blt more than a certain
       number of pixels. */
    if(IS_MSOC(pSmi))
        maxPixels = 128 / pDst->drawable.bitsPerPixel;
    else
        maxPixels = 1280 / pDst->drawable.bitsPerPixel;

    while(height>0){
	SMI_Composite(pDst, srcX, srcY, maskX, maskY, dstX, dstY, width, min(height, maxPixels));

	srcY += maxPixels;
	dstY += maxPixels;
	height -= maxPixels;
    }

    LEAVE();
}

static void
SMI_DoneComposite(PixmapPtr pDst)
{
    ENTER();
    LEAVE();
}
