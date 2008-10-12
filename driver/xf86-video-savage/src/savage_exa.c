/*
 * The exa accel file for the Savage driver.  
 * 
 * Created 2005-2006 by Alex Deucher
 * Revision: 
 *
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "savage_driver.h"
#include "savage_regs.h"
#include "savage_bci.h"
#include "savage_streams.h"

#ifdef XF86DRI
#define _XF86DRI_SERVER_
#include "savage_dri.h"
#endif

static void
SavageEXASync(ScreenPtr pScreen, int marker);

static Bool
SavagePrepareSolid(PixmapPtr pPixmap, int alu, Pixel planemask, Pixel fg);

static void
SavageSolid(PixmapPtr pPixmap, int x1, int y1, int x2, int y2);

static void
SavageDoneSolid(PixmapPtr pPixmap);

static Bool
SavagePrepareCopy(PixmapPtr pSrcPixmap, PixmapPtr pDstPixmap, int xdir, int ydir,
					int alu, Pixel planemask);

static void
SavageCopy(PixmapPtr pDstPixmap, int srcX, int srcY, int dstX, int dstY, int width, int height);

static void
SavageDoneCopy(PixmapPtr pDstPixmap);

Bool
SavageUploadToScreen(PixmapPtr pDst, int x, int y, int w, int h, char *src, int src_pitch);

#if 1
Bool
SavageDownloadFromScreen(PixmapPtr pSrc, int x, int y, int w, int h, char *dst, int dst_pitch);
#endif

#if 0
#define	GXclear	0x00
#define GXand	0x88
#define	GXandReverse	0x44
#define	GXcopy	0xCC
#define	GXandInverted	0x22
#define	GXnoop	0xAA
#define	GXxor	0x66
#define	GXor	0xEE
#define	GXnor	0x11
#define	GXequiv	0x99
#define	GXinvert	0x55
#define	GXorReverse	0xDD
#define	GXcopyInverted	0x33
#define	GXorInverted	0xBB
#define	GXnand	0x77
#define	GXset	0xFF
#endif

static int SavageGetCopyROP(int rop) {

    int ALUCopyROP[16] =
    {
       0x00, /*ROP_0 GXclear */
       0x88, /*ROP_DSa GXand */
       0x44, /*ROP_SDna GXandReverse */
       0xCC, /*ROP_S GXcopy */
       0x22, /*ROP_DSna GXandInverted */
       0xAA, /*ROP_D GXnoop */
       0x66, /*ROP_DSx GXxor */
       0xEE, /*ROP_DSo GXor */
       0x11, /*ROP_DSon GXnor */
       0x99, /*ROP_DSxn GXequiv */
       0x55, /*ROP_Dn GXinvert*/
       0xDD, /*ROP_SDno GXorReverse */
       0x33, /*ROP_Sn GXcopyInverted */
       0xBB, /*ROP_DSno GXorInverted */
       0x77, /*ROP_DSan GXnand */
       0xFF, /*ROP_1 GXset */
    };

    return (ALUCopyROP[rop]);

}

static int SavageGetSolidROP(int rop) {

    int ALUSolidROP[16] =
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

    return (ALUSolidROP[rop]);

}

Bool 
SavageEXAInit(ScreenPtr pScreen)
{
    ScrnInfoPtr pScrn = xf86Screens[pScreen->myNum];
    SavagePtr psav = SAVPTR(pScrn);

    if (!(psav->EXADriverPtr = exaDriverAlloc())) {
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		"Failed to allocate EXADriverRec.\n");
	return FALSE;
    }

    /*ErrorF("in SavageEXAinit\n");*/

    psav->EXADriverPtr->exa_major = 2;
    psav->EXADriverPtr->exa_minor = 0;
    
    /* use the linear aperture */
    psav->EXADriverPtr->memoryBase = psav->FBBase + pScrn->fbOffset;

    psav->EXADriverPtr->memorySize = psav->EXAendfb;


    if (psav->bTiled) {
        if (pScrn->bitsPerPixel == 16) {
            psav->EXADriverPtr->offScreenBase = 
		((pScrn->virtualX+63)/64)*((pScrn->virtualY+15)/16) * 2048;
        } else {
            psav->EXADriverPtr->offScreenBase = 
		((pScrn->virtualX+31)/32)*((pScrn->virtualY+15)/16) * 2048;
	}
    } else {
        psav->EXADriverPtr->offScreenBase = pScrn->virtualY * psav->lDelta;
    }

    if (psav->EXADriverPtr->memorySize > psav->EXADriverPtr->offScreenBase) {
        psav->EXADriverPtr->flags = EXA_OFFSCREEN_PIXMAPS;
    } else {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		"Not enough video RAM for EXA offscreen memory manager.\n");
    }

    if (psav->bTiled) {
	psav->EXADriverPtr->pixmapPitchAlign = 128; /* ~127 */
    } else {
	psav->EXADriverPtr->pixmapPitchAlign = 32; /* ~31 */
    }

    if (psav->Chipset == S3_SAVAGE2000 ||
	psav->Chipset == S3_SUPERSAVAGE) {
	psav->EXADriverPtr->pixmapOffsetAlign = 16; /* octword */
    } else {
	psav->EXADriverPtr->pixmapOffsetAlign = 8; /* quadword */
    }

    /* engine has 12 bit coordinates */
    psav->EXADriverPtr->maxX = 4096;
    psav->EXADriverPtr->maxY = 4096;

    /* Sync */
    psav->EXADriverPtr->WaitMarker = SavageEXASync;
#if 1
    /* Solid fill */
    psav->EXADriverPtr->PrepareSolid = SavagePrepareSolid;
    psav->EXADriverPtr->Solid = SavageSolid;
    psav->EXADriverPtr->DoneSolid = SavageDoneSolid;

    /* Copy */
    psav->EXADriverPtr->PrepareCopy = SavagePrepareCopy;
    psav->EXADriverPtr->Copy = SavageCopy;
    psav->EXADriverPtr->DoneCopy = SavageDoneCopy;
#endif
    /* Composite not implemented yet */
    /* savage3d series only have one tmu */

#if 1
    /* host data blit */
    psav->EXADriverPtr->UploadToScreen = SavageUploadToScreen;
#endif
#if 0
    psav->EXADriverPtr->DownloadFromScreen = SavageDownloadFromScreen;
#endif

    if(!exaDriverInit(pScreen, psav->EXADriverPtr)) {
        xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                   "exaDriverinit failed.\n");
	return FALSE;
    } else {
        xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                   "Savage EXA Acceleration enabled.\n");
	return TRUE;
    }

}
static void
SavageEXASync(ScreenPtr pScreen, int marker)
{
    SavagePtr psav = SAVPTR(xf86Screens[pScreen->myNum]);
    psav->WaitIdleEmpty(psav);
}

static unsigned int
SavageSetBD(SavagePtr psav, PixmapPtr pPixmap)
{
    unsigned int pixpitch, bd = 0;
    unsigned int tile16, tile32;

    /* front buffer can be tiled */
    if (psav->bTiled && exaGetPixmapOffset(pPixmap) == 0) {
        switch( psav->Chipset ) {
    	    case S3_SAVAGE3D:
    	    case S3_SAVAGE_MX:
    	    case S3_SAVAGE4:
    	        tile16 = BCI_BD_TILE_16;
    	        tile32 = BCI_BD_TILE_32;
		break;
    	    case S3_TWISTER:
    	    case S3_PROSAVAGE:
    	    case S3_PROSAVAGEDDR:
    	    case S3_SUPERSAVAGE:
    	    case S3_SAVAGE2000:
    	    default:
    	        tile16 = BCI_BD_TILE_DESTINATION;
    	        tile32 = BCI_BD_TILE_DESTINATION;
		break;
	}
    } else {
	tile16 = BCI_BD_TILE_NONE;
    	tile32 = BCI_BD_TILE_NONE;
    }

    /* HW uses width */
    pixpitch = exaGetPixmapPitch(pPixmap) / (pPixmap->drawable.bitsPerPixel >> 3);

    BCI_BD_SET_BPP(bd, pPixmap->drawable.bitsPerPixel);
    BCI_BD_SET_STRIDE(bd, pixpitch);

    if (pPixmap->drawable.bitsPerPixel == 32)
	bd |= BCI_BD_BW_DISABLE | tile32;
    else
	bd |= BCI_BD_BW_DISABLE | tile16;

    return bd;

}

static Bool
SavagePrepareSolid(PixmapPtr pPixmap, int alu, Pixel planemask, Pixel fg)
{
    ScrnInfoPtr pScrn = xf86Screens[pPixmap->drawable.pScreen->myNum];
    SavagePtr psav = SAVPTR(pScrn);
    int cmd, rop;
    BCI_GET_PTR;

    /* HW seems to ignore alpha */
    if (pPixmap->drawable.bitsPerPixel == 32)
	return FALSE;

    cmd = BCI_CMD_RECT
        | BCI_CMD_RECT_XP | BCI_CMD_RECT_YP
        | BCI_CMD_DEST_PBD /*BCI_CMD_DEST_PBD_NEW*/
	| BCI_CMD_SRC_SOLID;

#if 0
    if (alu == 3 /*GXcopy*/) {
	if (fg == 0)
	    alu = 0 /*GXclear*/;
	else if (fg == planemask)
	    alu = 15 /*GXset*/;
    }

    if (EXA_PM_IS_SOLID(&pPixmap->drawable, planemask)) {
	if (!((alu == 5 /*GXnoop*/) || (alu == 15 /*GXset*/) || (alu == 0 /*GXclear*/) || (alu == 10 /*GXinvert*/))) 
	   cmd |= BCI_CMD_SEND_COLOR;
	rop = SavageGetCopyROP(alu);
    } else {	
	switch(alu) {
	case 5  /*GXnoop*/:
	    break;
	case 15 /*GXset*/:
	case 0  /*GXclear*/:
	case 10 /*GXinvert*/:
	    cmd |= BCI_CMD_SEND_COLOR;
	    fg = planemask;
	    break;
	default:
	    cmd |= BCI_CMD_SEND_COLOR;
	    break;
	}
	rop = SavageGetSolidROP(alu);
    }
#else
    cmd |= BCI_CMD_SEND_COLOR;
    rop = SavageGetSolidROP(alu);
#endif

    BCI_CMD_SET_ROP( cmd, rop );

    psav->pbd_offset = exaGetPixmapOffset(pPixmap);
    psav->pbd_high = SavageSetBD(psav, pPixmap);

    psav->SavedBciCmd = cmd;
    psav->SavedFgColor = fg;

    psav->WaitQueue(psav,5);

    BCI_SEND(BCI_SET_REGISTER
	     | BCI_SET_REGISTER_COUNT(1)
	     | BCI_BITPLANE_WRITE_MASK);
    BCI_SEND(planemask);

    BCI_SEND(BCI_SET_REGISTER
	     | BCI_SET_REGISTER_COUNT(2)
	     | BCI_PBD_1);
    BCI_SEND(psav->pbd_offset);
    BCI_SEND(psav->pbd_high);

    return TRUE;
}

static void
SavageSolid(PixmapPtr pPixmap, int x1, int y1, int x2, int y2)
{
    ScrnInfoPtr pScrn = xf86Screens[pPixmap->drawable.pScreen->myNum];
    SavagePtr psav = SAVPTR(pScrn);
    int w = x2 - x1;
    int h = y2 - y1;
    BCI_GET_PTR;

    /* yes, it has to be done this way... */
    psav->WaitQueue(psav,4);
    BCI_SEND(psav->SavedBciCmd);
    /*BCI_SEND(psav->pbd_offset);
    BCI_SEND(psav->pbd_high);*/
#if 0
    if ( psav->SavedBciCmd & BCI_CMD_SEND_COLOR )
	BCI_SEND(psav->SavedFgColor);
#endif
    BCI_SEND(psav->SavedFgColor);
    BCI_SEND(BCI_X_Y(x1, y1));
    BCI_SEND(BCI_W_H(w, h));

}

static void
SavageDoneSolid(PixmapPtr pPixmap)
{
}

static Bool
SavagePrepareCopy(PixmapPtr pSrcPixmap, PixmapPtr pDstPixmap, int xdir, int ydir,
					int alu, Pixel planemask)
{
    ScrnInfoPtr pScrn = xf86Screens[pDstPixmap->drawable.pScreen->myNum];
    SavagePtr psav = SAVPTR(pScrn);
    int cmd;
    BCI_GET_PTR;

    /*ErrorF("in preparecopy\n");*/

    cmd = BCI_CMD_RECT | BCI_CMD_DEST_PBD_NEW | BCI_CMD_SRC_SBD_COLOR_NEW;

    BCI_CMD_SET_ROP( cmd, SavageGetCopyROP(alu) );

    if (xdir > 0 ) cmd |= BCI_CMD_RECT_XP;
    if (ydir > 0 ) cmd |= BCI_CMD_RECT_YP;

    psav->sbd_offset = exaGetPixmapOffset(pSrcPixmap);
    psav->pbd_offset = exaGetPixmapOffset(pDstPixmap);

    psav->sbd_high = SavageSetBD(psav, pSrcPixmap);
    psav->pbd_high = SavageSetBD(psav, pDstPixmap);

    psav->SavedBciCmd = cmd;

    psav->WaitQueue(psav,7);

    BCI_SEND(BCI_SET_REGISTER
	     | BCI_SET_REGISTER_COUNT(1)
	     | BCI_BITPLANE_WRITE_MASK);
    BCI_SEND(planemask);

    BCI_SEND(psav->SavedBciCmd);
    /* src */
    BCI_SEND(psav->sbd_offset);
    BCI_SEND(psav->sbd_high);
    /* dst */
    BCI_SEND(psav->pbd_offset);
    BCI_SEND(psav->pbd_high);

    return TRUE;
}

static void
SavageCopy(PixmapPtr pDstPixmap, int srcX, int srcY, int dstX, int dstY, int width, int height)
{
    ScrnInfoPtr pScrn = xf86Screens[pDstPixmap->drawable.pScreen->myNum];
    SavagePtr psav = SAVPTR(pScrn);
    BCI_GET_PTR;

    if (!width || !height) return;

    if (!(psav->SavedBciCmd & BCI_CMD_RECT_XP)) {
        width --;
        srcX += width;
        dstX += width;
        width ++;
    }
    if (!(psav->SavedBciCmd & BCI_CMD_RECT_YP)) {
        height --;
        srcY += height;
        dstY += height;
        height ++;
    }

    psav->WaitQueue(psav,4);
    BCI_SEND(BCI_X_Y(srcX, srcY));
    BCI_SEND(BCI_X_Y(dstX, dstY));
    BCI_SEND(BCI_W_H(width, height));

}

static void
SavageDoneCopy(PixmapPtr pDstPixmap)
{
}

Bool
SavageUploadToScreen(PixmapPtr pDst, int x, int y, int w, int h, char *src, int src_pitch)
{
    ScrnInfoPtr pScrn = xf86Screens[pDst->drawable.pScreen->myNum];
    SavagePtr psav = SAVPTR(pScrn);
    BCI_GET_PTR;
    int i, j, dwords, queue, Bpp;
    unsigned int cmd;
    CARD32 * srcp; 
    
    Bpp = pDst->drawable.bitsPerPixel / 8;

    psav->sbd_offset = exaGetPixmapOffset(pDst);
    psav->sbd_high = SavageSetBD(psav, pDst);

    cmd = BCI_CMD_RECT
	| BCI_CMD_RECT_XP | BCI_CMD_RECT_YP
        | BCI_CMD_CLIP_LR
        | BCI_CMD_DEST_SBD_NEW
        | BCI_CMD_SRC_COLOR; /* host color data */

    BCI_CMD_SET_ROP( cmd, 0xCC ); /* GXcopy */

    /*ErrorF("in UTS\n");*/

    psav->WaitQueue(psav, 6);
    BCI_SEND(cmd);

    /* dst */
    BCI_SEND(psav->sbd_offset);
    BCI_SEND(psav->sbd_high);

    BCI_SEND(BCI_CLIP_LR(x, x+w-1));
    BCI_SEND(BCI_X_Y(x, y));
    BCI_SEND(BCI_W_H(w, h));
    
    queue = 120 * 1024;
    dwords = (((w * Bpp) + 3) >> 2);
    for (i = 0; i < h; i++) {
	srcp = (CARD32 *)src;
	for (j = 0; j < dwords; j++) {
	    if (queue < 4) {
		BCI_RESET;
		queue = 120 * 1024;
	    }
	    BCI_SEND(*srcp++);
	    queue -= 4;
	}
	src += src_pitch;
    }

    /*exaWaitSync(pDst->drawable.pScreen);*/

    return TRUE;
}

#if 1
Bool
SavageDownloadFromScreen(PixmapPtr pSrc, int x, int y, int w, int h, char *dst, int dst_pitch)
{
    ScrnInfoPtr pScrn = xf86Screens[pSrc->drawable.pScreen->myNum];
    unsigned char *src = pSrc->devPrivate.ptr;
    int	src_pitch = exaGetPixmapPitch(pSrc);
    int	bpp = pSrc->drawable.bitsPerPixel;

    exaWaitSync(pSrc->drawable.pScreen);

    /* do the copy */
    src += (x * bpp / 8) + (y * src_pitch);
    w *= bpp / 8;

    while (h--) {
	memcpy(dst, src, w);
	src += src_pitch;
	dst += dst_pitch;
    }
    return TRUE;
}

#endif
