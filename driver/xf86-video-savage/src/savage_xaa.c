/*
 * The accel file for the Savage driver.  
 * 
 * Created 20/03/97 by Sebastien Marineau for 3.3.6
 * Modified 17-Nov-2000 by Tim Roberts for 4.0.1
 * Modified Feb-2004 by Alex Deucher - integrating DRI support
 * Modified 2005-2006 by Alex Deucher - adding exa support
 * Revision: 
 *
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <X11/Xarch.h>
#include "savage_driver.h"
#ifdef HAVE_XAA_H
#include "xaalocal.h"
#include "xaarop.h"

#include "miline.h"

#include "savage_bci.h"

extern int gSavageEntityIndex;

static void SavageSetupForScreenToScreenCopy(
    ScrnInfoPtr pScrn,
    int xdir, 
    int ydir,
    int rop,
    unsigned planemask,
    int transparency_color);

static void SavageSubsequentScreenToScreenCopy(
    ScrnInfoPtr pScrn,
    int x1,
    int y1,
    int x2,
    int y2,
    int w,
    int h);
 
static void SavageSetupForSolidFill(
    ScrnInfoPtr pScrn,
    int color, 
    int rop,
    unsigned int planemask);

static void SavageSubsequentSolidFillRect(
    ScrnInfoPtr pScrn,
    int x,
    int y,
    int w,
    int h);

static void SavageSubsequentSolidBresenhamLine(
    ScrnInfoPtr pScrn,
    int x1,
    int y1,
    int e1,
    int e2,
    int err,
    int length,
    int octant);

static void SavageSetupForCPUToScreenColorExpandFill(
    ScrnInfoPtr pScrn,
    int fg,
    int bg,
    int rop,
    unsigned int planemask);

static void SavageSubsequentScanlineCPUToScreenColorExpandFill(
    ScrnInfoPtr pScrn,
    int x,
    int y,
    int w,
    int h,
    int skipleft);

static void SavageSubsequentColorExpandScanline(
    ScrnInfoPtr pScrn,
    int buffer_no);

static void SavageSetupForMono8x8PatternFill(
    ScrnInfoPtr pScrn,
    int patternx,
    int patterny,
    int fg, 
    int bg,
    int rop,
    unsigned int planemask);

static void SavageSubsequentMono8x8PatternFillRect(
    ScrnInfoPtr pScrn,
    int pattern0,
    int pattern1,
    int x,
    int y,
    int w,
    int h);

static void SavageSetClippingRectangle(
    ScrnInfoPtr pScrn,
    int x1,
    int y1,
    int x2,
    int y2);

static void SavageDisableClipping( ScrnInfoPtr );

/* from savage_image.c: */

void SavageSetupForImageWrite(
    ScrnInfoPtr pScrn,
    int rop,
    unsigned int planemask,
    int transparency_color,
    int bpp,
    int depth);

void SavageSubsequentImageWriteRect(
    ScrnInfoPtr pScrn,
    int x,
    int y,
    int w,
    int h,
    int skipleft);

void SavageWriteBitmapCPUToScreenColorExpand (
    ScrnInfoPtr pScrn,
    int x, int y, int w, int h,
    unsigned char * src,
    int srcwidth,
    int skipleft,
    int fg, int bg,
    int rop,
    unsigned int planemask
);

static 	 
void SavageRestoreAccelState(ScrnInfoPtr pScrn) 	 
{ 	 
    SavagePtr psav = SAVPTR(pScrn); 	  

    psav->WaitIdleEmpty(psav);

    return;
}
#endif

Bool 
SavageXAAInit(ScreenPtr pScreen)
{
#ifdef HAVE_XAA_H
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
    SavagePtr psav = SAVPTR(pScrn);
    XAAInfoRecPtr xaaptr;
    BoxRec AvailFBArea;
    int tmp;

    /* Set-up our GE command primitive */
    
    if (pScrn->depth == 8) {
	psav->PlaneMask = 0xFF;
    }
    else if (pScrn->depth == 15) {
	psav->PlaneMask = 0x7FFF;
    }
    else if (pScrn->depth == 16) {
	psav->PlaneMask = 0xFFFF;
    }
    else if (pScrn->depth == 24) {
	psav->PlaneMask = 0xFFFFFF;
    }

    /* General acceleration flags */

    if (!(xaaptr = psav->AccelInfoRec = XAACreateInfoRec())) {
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		"Failed to allocate XAAInfoRec.\n");
	return FALSE;
    }

    xaaptr->Flags = 0
    	| PIXMAP_CACHE
	| OFFSCREEN_PIXMAPS
	| LINEAR_FRAMEBUFFER
	;


    if(xf86IsEntityShared(pScrn->entityList[0]))
    {
        DevUnion* pPriv;
        SavageEntPtr pEnt;
        pPriv = xf86GetEntityPrivate(pScrn->entityList[0],
                gSavageEntityIndex);
        pEnt = pPriv->ptr;
        
        /*if there are more than one devices sharing this entity, we
          have to assign this call back, otherwise the XAA will be
          disabled */
        if(pEnt->HasSecondary)
           xaaptr->RestoreAccelState           = SavageRestoreAccelState;
    }

    /* Clipping */

    xaaptr->SetClippingRectangle = SavageSetClippingRectangle;
    xaaptr->DisableClipping = SavageDisableClipping;
    xaaptr->ClippingFlags = 0
#if 0
	| HARDWARE_CLIP_SOLID_FILL 
	| HARDWARE_CLIP_SOLID_LINE
	| HARDWARE_CLIP_DASHED_LINE
#endif
	| HARDWARE_CLIP_SCREEN_TO_SCREEN_COPY
	| HARDWARE_CLIP_MONO_8x8_FILL
	| HARDWARE_CLIP_COLOR_8x8_FILL
	;

    xaaptr->Sync = SavageAccelSync;

    /* ScreenToScreen copies */

#if 1

    xaaptr->SetupForScreenToScreenCopy = SavageSetupForScreenToScreenCopy;
    xaaptr->SubsequentScreenToScreenCopy = SavageSubsequentScreenToScreenCopy;
    xaaptr->ScreenToScreenCopyFlags = 0
	| NO_TRANSPARENCY
	| NO_PLANEMASK
	| ROP_NEEDS_SOURCE;

#endif


    /* Solid filled rectangles */

#if 1
    xaaptr->SetupForSolidFill = SavageSetupForSolidFill;
    xaaptr->SubsequentSolidFillRect = SavageSubsequentSolidFillRect;
    xaaptr->SolidFillFlags = NO_PLANEMASK | ROP_NEEDS_SOURCE;
#endif

    /* Mono 8x8 pattern fills */

#if 1
    xaaptr->SetupForMono8x8PatternFill = SavageSetupForMono8x8PatternFill;
    xaaptr->SubsequentMono8x8PatternFillRect 
    	= SavageSubsequentMono8x8PatternFillRect;
    xaaptr->Mono8x8PatternFillFlags = 0
	| HARDWARE_PATTERN_PROGRAMMED_BITS 
	| HARDWARE_PATTERN_SCREEN_ORIGIN
	| BIT_ORDER_IN_BYTE_MSBFIRST
	| ROP_NEEDS_SOURCE
	;
    if( psav->Chipset == S3_SAVAGE4 )
	xaaptr->Mono8x8PatternFillFlags |= NO_TRANSPARENCY;
#endif

    /* Solid lines */

#if 1
    xaaptr->SolidLineFlags = NO_PLANEMASK;
    xaaptr->SetupForSolidLine = SavageSetupForSolidFill;
    xaaptr->SubsequentSolidBresenhamLine = SavageSubsequentSolidBresenhamLine;
#if 0
    xaaptr->SubsequentSolidFillTrap = SavageSubsequentSolidFillTrap; 
#endif

    xaaptr->SolidBresenhamLineErrorTermBits = 13;
#endif

    /* ImageWrite */

    xaaptr->ImageWriteFlags = 0
	| NO_PLANEMASK
	| CPU_TRANSFER_PAD_DWORD
	| SCANLINE_PAD_DWORD
	| BIT_ORDER_IN_BYTE_MSBFIRST
	| LEFT_EDGE_CLIPPING
	;
    xaaptr->SetupForImageWrite = SavageSetupForImageWrite;
    xaaptr->SubsequentImageWriteRect = SavageSubsequentImageWriteRect;
    xaaptr->NumScanlineImageWriteBuffers = 1;
    xaaptr->ImageWriteBase = psav->BciMem;
    xaaptr->ImageWriteRange = 120 * 1024;


    /* CPU to Screen color expansion */

    xaaptr->ScanlineCPUToScreenColorExpandFillFlags = 0
	| NO_PLANEMASK
	| CPU_TRANSFER_PAD_DWORD
	| SCANLINE_PAD_DWORD
	| BIT_ORDER_IN_BYTE_MSBFIRST
	| LEFT_EDGE_CLIPPING
	| ROP_NEEDS_SOURCE
	;

    xaaptr->SetupForScanlineCPUToScreenColorExpandFill =
            SavageSetupForCPUToScreenColorExpandFill;
    xaaptr->SubsequentScanlineCPUToScreenColorExpandFill =
            SavageSubsequentScanlineCPUToScreenColorExpandFill;
    xaaptr->SubsequentColorExpandScanline =
	    SavageSubsequentColorExpandScanline;
    xaaptr->ColorExpandBase = psav->BciMem;
    xaaptr->ScanlineColorExpandBuffers = &xaaptr->ColorExpandBase;
    xaaptr->NumScanlineColorExpandBuffers = 1;

    /* Set up screen parameters. */

    psav->Bpp = pScrn->bitsPerPixel / 8;
    psav->Bpl = pScrn->displayWidth * psav->Bpp;
    psav->ScissB = (psav->CursorKByte << 10) / psav->Bpl;
    if (psav->ScissB > 2047)
        psav->ScissB = 2047;

    /*
     * Finally, we set up the video memory space available to the pixmap
     * cache. In this case, all memory from the end of the virtual screen
     * to the end of the command overflow buffer can be used. If you haven't
     * enabled the PIXMAP_CACHE flag, then these lines can be omitted.
     */
            
    AvailFBArea.x1 = 0;
    AvailFBArea.y1 = 0;
    AvailFBArea.x2 = psav->cxMemory;
    AvailFBArea.y2 = psav->cyMemory;
    xf86InitFBManager(pScreen, &AvailFBArea);
    /*
     * because the alignment requirement,the on-screen need more memory
     * than (0,0,virtualX,virtualY), but xf86InitFBManager only subtract
     * (pScrn->virtualX * pScrn->virtualY from (0,0,cxMemory,cyMemory),so
     * here,we should reserver some memory for on-screen
     */
    tmp = ((psav->cxMemory * pScrn->virtualY - pScrn->virtualX * pScrn->virtualY)
               + psav->cxMemory -1) / (psav->cxMemory);
    if (tmp)
        xf86AllocateOffscreenArea(pScreen, psav->cxMemory,tmp, 0, NULL, NULL, NULL);
        
    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                   "Using %d lines for offscreen memory.\n",
                   psav->cyMemory - pScrn->virtualY );


    return XAAInit(pScreen, xaaptr);
#else
    return FALSE;
#endif
}

/* The sync function for the GE */
void
SavageAccelSync(ScrnInfoPtr pScrn)
{
    SavagePtr psav = SAVPTR(pScrn);
    psav->WaitIdleEmpty(psav);
}

#ifdef HAVE_XAA_H
/*
 * The XAA ROP helper routines all assume that a solid color is a 
 * "pattern".  The Savage chips, however, apply a non-stippled solid
 * color as "source".  Thus, we use a slightly customized version.
 */

static int
SavageHelpPatternROP(ScrnInfoPtr pScrn, int *fg, int *bg, unsigned int pm, int *rop)
{
    XAAInfoRecPtr infoRec = GET_XAAINFORECPTR_FROM_SCRNINFOPTR(pScrn);
    int ret = 0;
    
    pm &= infoRec->FullPlanemask;

    if(pm == infoRec->FullPlanemask) {
	if(!NO_SRC_ROP(*rop)) 
	   ret |= ROP_PAT;
	*rop = XAAGetCopyROP(*rop);
    } else {	
	switch(*rop) {
	case GXnoop:
	    break;
	case GXset:
	case GXclear:
	case GXinvert:
	    ret |= ROP_PAT;
	    *fg = pm;
	    if(*bg != -1)
		*bg = pm;
	    break;
	default:
	    ret |= ROP_PAT | ROP_SRC;
	    break;
	}
	*rop = XAAGetCopyROP_PM(*rop);
    }

    return ret;
}


static int
SavageHelpSolidROP(ScrnInfoPtr pScrn, int *fg, unsigned int pm, int *rop)
{
    XAAInfoRecPtr infoRec = GET_XAAINFORECPTR_FROM_SCRNINFOPTR(pScrn);
    int ret = 0;
    
    pm &= infoRec->FullPlanemask;

    if(pm == infoRec->FullPlanemask) {
	if(!NO_SRC_ROP(*rop)) 
	   ret |= ROP_PAT;
	*rop = XAAGetCopyROP(*rop);
    } else {	
	switch(*rop) {
	case GXnoop:
	    break;
	case GXset:
	case GXclear:
	case GXinvert:
	    ret |= ROP_PAT;
	    *fg = pm;
	    break;
	default:
	    ret |= ROP_PAT | ROP_SRC;
	    break;
	}
	*rop = XAAGetCopyROP_PM(*rop);
    }

    return ret;
}



/* These are the ScreenToScreen bitblt functions. We support all ROPs, all
 * directions, and a planemask by adjusting the ROP and using the mono pattern
 * registers.
 *
 * (That's a lie; we don't really support planemask.)
 */

static void 
SavageSetupForScreenToScreenCopy(
    ScrnInfoPtr pScrn,
    int xdir, 
    int ydir,
    int rop,
    unsigned planemask,
    int transparency_color)
{
    SavagePtr psav = SAVPTR(pScrn);
    int cmd;

    cmd = BCI_CMD_RECT | BCI_CMD_DEST_PBD_NEW | BCI_CMD_SRC_SBD_COLOR_NEW;

    BCI_CMD_SET_ROP( cmd, XAAGetCopyROP(rop) );
    if (transparency_color != -1)
        cmd |= BCI_CMD_SEND_COLOR | BCI_CMD_SRC_TRANSPARENT;

    if (xdir == 1 ) cmd |= BCI_CMD_RECT_XP;
    if (ydir == 1 ) cmd |= BCI_CMD_RECT_YP;

    psav->SavedBciCmd = cmd;
    psav->SavedBgColor = transparency_color;
}

static void 
SavageSubsequentScreenToScreenCopy(
    ScrnInfoPtr pScrn,
    int x1,
    int y1,
    int x2,
    int y2,
    int w,
    int h)
{
    SavagePtr psav = SAVPTR(pScrn);

    BCI_GET_PTR;

    if (!w || !h) return;

    if (!(psav->SavedBciCmd & BCI_CMD_RECT_XP)) {
        w --;
        x1 += w;
        x2 += w;
        w ++;
    }
    if (!(psav->SavedBciCmd & BCI_CMD_RECT_YP)) {
        h --;
        y1 += h;
        y2 += h;
        h ++;
    }

    psav->WaitQueue(psav,9);


    BCI_SEND(psav->SavedBciCmd);

    BCI_SEND(psav->GlobalBD.bd2.LoPart);
    BCI_SEND(psav->GlobalBD.bd2.HiPart);

    BCI_SEND(psav->GlobalBD.bd2.LoPart);
    BCI_SEND(psav->GlobalBD.bd2.HiPart);

    if (psav->SavedBgColor != 0xffffffff) 
	BCI_SEND(psav->SavedBgColor);
    BCI_SEND(BCI_X_Y(x1, y1));
    BCI_SEND(BCI_X_Y(x2, y2));
    BCI_SEND(BCI_W_H(w, h));
}


/*
 * SetupForSolidFill is also called to set up for lines.
 */ 

static void 
SavageSetupForSolidFill(
    ScrnInfoPtr pScrn,
    int color, 
    int rop,
    unsigned int planemask)
{
    SavagePtr psav = SAVPTR(pScrn);
    XAAInfoRecPtr xaaptr = GET_XAAINFORECPTR_FROM_SCRNINFOPTR( pScrn );
    int cmd;
    int mix;

    cmd = BCI_CMD_RECT
        | BCI_CMD_RECT_XP | BCI_CMD_RECT_YP
        | BCI_CMD_DEST_PBD_NEW | BCI_CMD_SRC_SOLID;

    /* Don't send a color if we don't have to. */

    if( rop == GXcopy )
    {
	if( color == 0 )
	    rop = GXclear;
	else if( (unsigned int)color == xaaptr->FullPlanemask )
	    rop = GXset;
    }

    mix = SavageHelpSolidROP( pScrn, &color, planemask, &rop );

    if( mix & ROP_PAT )
	cmd |= BCI_CMD_SEND_COLOR;

    BCI_CMD_SET_ROP( cmd, rop );

    psav->SavedBciCmd = cmd;
    psav->SavedFgColor = color;
}
    
    
static void 
SavageSubsequentSolidFillRect(
    ScrnInfoPtr pScrn,
    int x,
    int y,
    int w,
    int h)
{
    SavagePtr psav = SAVPTR(pScrn);
    BCI_GET_PTR;
    
    if( !w || !h )
	return;

    psav->WaitQueue(psav,7);

    BCI_SEND(psav->SavedBciCmd);

    BCI_SEND(psav->GlobalBD.bd2.LoPart);
    BCI_SEND(psav->GlobalBD.bd2.HiPart);

    if( psav->SavedBciCmd & BCI_CMD_SEND_COLOR )
	BCI_SEND(psav->SavedFgColor);
    BCI_SEND(BCI_X_Y(x, y));
    BCI_SEND(BCI_W_H(w, h));
}

static void
SavageSetupForCPUToScreenColorExpandFill(
    ScrnInfoPtr pScrn,
    int fg,
    int bg,
    int rop,
    unsigned int planemask)
{
    SavagePtr psav = SAVPTR(pScrn);
    int cmd;
    int mix;

    cmd = BCI_CMD_RECT | BCI_CMD_RECT_XP | BCI_CMD_RECT_YP
	| BCI_CMD_CLIP_LR
        | BCI_CMD_DEST_PBD_NEW | BCI_CMD_SRC_MONO;

    mix = SavageHelpPatternROP( pScrn, &fg, &bg, planemask, &rop );

    if( mix & ROP_PAT )
        cmd |= BCI_CMD_SEND_COLOR;

    BCI_CMD_SET_ROP( cmd, rop );

    if (bg != -1)
        cmd |= BCI_CMD_SEND_COLOR;
    else 
	cmd |= BCI_CMD_SRC_TRANSPARENT;

    psav->SavedBciCmd = cmd;
    psav->SavedFgColor = fg;
    psav->SavedBgColor = bg;
}


static void
SavageSubsequentScanlineCPUToScreenColorExpandFill(
    ScrnInfoPtr pScrn,
    int x,
    int y,
    int w,
    int h,
    int skipleft)
{
    SavagePtr psav = SAVPTR(pScrn);
    BCI_GET_PTR;

    /* XAA will be sending bitmap data next.  */
    /* We should probably wait for empty/idle here. */

    psav->WaitQueue(psav,22);

    BCI_SEND(psav->SavedBciCmd);

    BCI_SEND(psav->GlobalBD.bd2.LoPart);
    BCI_SEND(psav->GlobalBD.bd2.HiPart);

    BCI_SEND(BCI_CLIP_LR(x+skipleft, x+w-1));
    w = (w + 31) & ~31;
    if( psav->SavedBciCmd & BCI_CMD_SEND_COLOR )
	BCI_SEND(psav->SavedFgColor);
    if( psav->SavedBgColor != 0xffffffff )
	BCI_SEND(psav->SavedBgColor);
    BCI_SEND(BCI_X_Y(x, y));
    BCI_SEND(BCI_W_H(w, 1));
    
    psav->Rect.x = x;
    psav->Rect.y = y + 1;
    psav->Rect.width = w;
    psav->Rect.height = h - 1;
}

static void
SavageSubsequentColorExpandScanline(
    ScrnInfoPtr pScrn,
    int buffer_no)
{
    /* This gets call after each scanline's image data has been sent. */
    SavagePtr psav = SAVPTR(pScrn);
    xRectangle xr = psav->Rect;
    BCI_GET_PTR;

    if( xr.height )
    {
	psav->WaitQueue(psav,20);
	BCI_SEND(BCI_X_Y( xr.x, xr.y));
	BCI_SEND(BCI_W_H( xr.width, 1 ));
        psav->Rect.height--;
	psav->Rect.y++;
    }
}


/*
 * The meaning of the two pattern paremeters to Setup & Subsequent for
 * Mono8x8Patterns varies depending on the flag bits.  We specify
 * HW_PROGRAMMED_BITS, which means our hardware can handle 8x8 patterns
 * without caching in the frame buffer.  Thus, Setup gets the pattern bits.
 * There is no way with BCI to rotate an 8x8 pattern, so we do NOT specify
 * HW_PROGRAMMED_ORIGIN.  XAA wil rotate it for us and pass the rotated
 * pattern to both Setup and Subsequent.  If we DID specify PROGRAMMED_ORIGIN,
 * then Setup would get the unrotated pattern, and Subsequent gets the
 * origin values.
 */

static void
SavageSetupForMono8x8PatternFill(
    ScrnInfoPtr pScrn,
    int patternx,
    int patterny,
    int fg, 
    int bg,
    int rop,
    unsigned int planemask)
{
    SavagePtr psav = SAVPTR(pScrn);
    int cmd;
    int mix;

    mix = XAAHelpPatternROP( pScrn, &fg, &bg, planemask, &rop );

    cmd = BCI_CMD_RECT | BCI_CMD_RECT_XP | BCI_CMD_RECT_YP
        | BCI_CMD_DEST_PBD_NEW;

    if( mix & ROP_PAT )
	cmd |= BCI_CMD_SEND_COLOR | BCI_CMD_PAT_MONO;

    if (bg == -1)
	cmd |= BCI_CMD_PAT_TRANSPARENT;

    BCI_CMD_SET_ROP(cmd, rop);

    psav->SavedBciCmd = cmd;
    psav->SavedFgColor = fg;
    psav->SavedBgColor = bg;
}


static void
SavageSubsequentMono8x8PatternFillRect(
    ScrnInfoPtr pScrn,
    int pattern0,
    int pattern1,
    int x,
    int y,
    int w,
    int h)
{
    SavagePtr psav = SAVPTR(pScrn);
    BCI_GET_PTR;

    /*
     * I didn't think it was my job to do trivial rejection, but 
     * miFillGeneralPolygon definitely generates null spans, and XAA
     * just passes them through.
     */

    if( !w || !h )
	return;

    psav->WaitQueue(psav,9);
    BCI_SEND(psav->SavedBciCmd);

    BCI_SEND(psav->GlobalBD.bd2.LoPart);
    BCI_SEND(psav->GlobalBD.bd2.HiPart);

    if( psav->SavedBciCmd & BCI_CMD_SEND_COLOR )
	BCI_SEND(psav->SavedFgColor);
    if( psav->SavedBgColor != 0xffffffff )
	BCI_SEND(psav->SavedBgColor);
    BCI_SEND(BCI_X_Y(x, y));
    BCI_SEND(BCI_W_H(w, h));
    if( psav->SavedBciCmd & BCI_CMD_PAT_MONO )
    {
	BCI_SEND(pattern0);
	BCI_SEND(pattern1);
    }
}


#if 0
static void 
SavageSetupForColor8x8PatternFill(
    ScrnInfoPtr pScrn,
    int patternx,
    int patterny,
    int rop,
    unsigned planemask,
    int trans_col)
{
    SavagePtr psav = SAVPTR(pScrn);

    int cmd;
    unsigned int bd;
    int pat_offset;
    
    /* ViRGEs and Savages do not support transparent color patterns. */
    /* We set the NO_TRANSPARENCY bit, so we should never receive one. */

    pat_offset = (int) (patternx * psav->Bpp + patterny * psav->Bpl);

    cmd = BCI_CMD_RECT | BCI_CMD_RECT_XP | BCI_CMD_RECT_YP
        | BCI_CMD_DEST_GBD | BCI_CMD_PAT_PBD_COLOR_NEW;
        
    (void) XAAHelpSolidROP( pScrn, &trans_col, planemask, &rop );

    BCI_CMD_SET_ROP(cmd, rop);
    bd = BCI_BD_BW_DISABLE;
    BCI_BD_SET_BPP(bd, pScrn->bitsPerPixel);
    BCI_BD_SET_STRIDE(bd, 8);

    psav->SavedBciCmd = cmd;
    psav->SavedSbdOffset = pat_offset;
    psav->SavedSbd = bd;
    psav->SavedBgColor = trans_col;
}


static void
SavageSubsequentColor8x8PatternFillRect(
    ScrnInfoPtr pScrn,
    int patternx,
    int patterny,
    int x,
    int y,
    int w,
    int h)
{
    SavagePtr psav = SAVPTR(pScrn);
    BCI_GET_PTR;

    if( !w || !h )
	return;

    psav->WaitQueue(psav,6);
    BCI_SEND(psav->SavedBciCmd);
    BCI_SEND(psav->SavedSbdOffset);
    BCI_SEND(psav->SavedSbd);
    BCI_SEND(BCI_X_Y(patternx,patterny));
    BCI_SEND(BCI_X_Y(x, y));
    BCI_SEND(BCI_W_H(w, h));
}
#endif

static void
SavageSubsequentSolidBresenhamLine(
    ScrnInfoPtr pScrn,
    int x1,
    int y1,
    int e1,
    int e2,
    int err,
    int length,
    int octant)
{
    SavagePtr psav = SAVPTR(pScrn);
    BCI_GET_PTR;
    int cmd;

    cmd = (psav->SavedBciCmd & 0x00ffffff);
    cmd |= BCI_CMD_LINE_LAST_PIXEL;

#ifdef DEBUG_EXTRA
    ErrorF("BresenhamLine, (%4d,%4d), len %4d, oct %d, err %4d,%4d,%4d clr %08x\n",
        x1, y1, length, octant, e1, e2, err, psav->SavedFgColor );
#endif

    psav->WaitQueue(psav, 7 );
    BCI_SEND(cmd);

    BCI_SEND(psav->GlobalBD.bd2.LoPart);
    BCI_SEND(psav->GlobalBD.bd2.HiPart);

    if( cmd & BCI_CMD_SEND_COLOR )
	BCI_SEND( psav->SavedFgColor );
    BCI_SEND(BCI_LINE_X_Y(x1, y1));
    BCI_SEND(BCI_LINE_STEPS(e2-e1, e2));
    BCI_SEND(BCI_LINE_MISC(length, 
    			   (octant & YMAJOR),
			   !(octant & XDECREASING),
			   !(octant & YDECREASING),
			   e2+err));
}

static void 
SavageSetClippingRectangle(
    ScrnInfoPtr pScrn,
    int x1,
    int y1,
    int x2,
    int y2)
{
    SavagePtr psav = SAVPTR(pScrn);
    BCI_GET_PTR;
    int cmd;

#ifdef DEBUG_EXTRA
    ErrorF("ClipRect, (%4d,%4d)-(%4d,%4d) \n", x1, y1, x2, y2 );
#endif

    cmd = BCI_CMD_NOP | BCI_CMD_CLIP_NEW;
    psav->WaitQueue(psav,3);
    BCI_SEND(cmd);
    BCI_SEND(BCI_CLIP_TL(y1, x1));
    BCI_SEND(BCI_CLIP_BR(y2, x2));
    psav->SavedBciCmd |= BCI_CMD_CLIP_CURRENT;
}


static void SavageDisableClipping( ScrnInfoPtr pScrn )
{
    SavagePtr psav = SAVPTR(pScrn);
#ifdef DEBUG_EXTRA
    ErrorF("Kill ClipRect\n");
#endif
    psav->SavedBciCmd &= ~BCI_CMD_CLIP_CURRENT;
}

void 
SavageWriteBitmapCPUToScreenColorExpand (
    ScrnInfoPtr pScrn,
    int x, int y, int w, int h,
    unsigned char * src,
    int srcwidth,
    int skipleft,
    int fg, int bg,
    int rop,
    unsigned int planemask
)
{
    SavagePtr psav = SAVPTR(pScrn);
    BCI_GET_PTR;
    int i, j, count, reset;
    unsigned int cmd;
    CARD32 * srcp;

/* We aren't using planemask at all here... */

    if( !srcwidth )
	return;

    cmd = BCI_CMD_RECT | BCI_CMD_RECT_XP | BCI_CMD_RECT_YP
        | BCI_CMD_SEND_COLOR | BCI_CMD_CLIP_LR
        | BCI_CMD_DEST_PBD_NEW | BCI_CMD_SRC_MONO;
    cmd |= XAAGetCopyROP(rop) << 16;

    if( bg == -1 )
        cmd |= BCI_CMD_SRC_TRANSPARENT;

    BCI_SEND(cmd);

    BCI_SEND(psav->GlobalBD.bd2.LoPart);
    BCI_SEND(psav->GlobalBD.bd2.HiPart);

    BCI_SEND(BCI_CLIP_LR(x+skipleft, x+w-1));
    BCI_SEND(fg);
    if( bg != -1 )
	BCI_SEND(bg);

    /* Bitmaps come in in units of DWORDS, LSBFirst.  This is exactly */
    /* reversed of what we expect.  */

    count = (w + 31) / 32;
/*    src += ((srcx & ~31) / 8); */

    /* The BCI region is 128k bytes.  A screen-sized mono bitmap can */
    /* exceed that. */

    reset = 65536 / count;
    
    for (j = 0; j < h; j ++) {
        BCI_SEND(BCI_X_Y(x, y+j));
        BCI_SEND(BCI_W_H(w, 1));
        srcp = (CARD32 *) src;
        for (i = count; i > 0; srcp ++, i --) {
            /* We have to invert the bits in each byte. */
            CARD32 u = *srcp;
            u = ((u & 0x0f0f0f0f) << 4) | ((u & 0xf0f0f0f0) >> 4);
            u = ((u & 0x33333333) << 2) | ((u & 0xcccccccc) >> 2);
            u = ((u & 0x55555555) << 1) | ((u & 0xaaaaaaaa) >> 1);
            BCI_SEND(u);
        }
        src += srcwidth;
        if( !--reset ) {
	    BCI_RESET;
            reset = 65536 / count;
        }
    }
}

void
SavageSetupForImageWrite(
    ScrnInfoPtr pScrn,
    int rop,
    unsigned planemask,
    int transparency_color,
    int bpp,
    int depth)
{
    SavagePtr psav = SAVPTR(pScrn);
    int cmd;

    cmd = BCI_CMD_RECT | BCI_CMD_RECT_XP | BCI_CMD_RECT_YP
        | BCI_CMD_CLIP_LR
        | BCI_CMD_DEST_PBD_NEW | BCI_CMD_SRC_COLOR;

    cmd |= XAAGetCopyROP(rop) << 16;

    if( transparency_color != -1 )
        cmd |= BCI_CMD_SRC_TRANSPARENT;

    psav->SavedBciCmd = cmd;
    psav->SavedBgColor = transparency_color;
}


void SavageSubsequentImageWriteRect
(
    ScrnInfoPtr pScrn,
    int x,
    int y,
    int w,
    int h,
    int skipleft)
{
    SavagePtr psav = SAVPTR(pScrn);
    BCI_GET_PTR;

    psav->WaitQueue( psav, 8 );
    BCI_SEND(psav->SavedBciCmd);

    BCI_SEND(psav->GlobalBD.bd2.LoPart);
    BCI_SEND(psav->GlobalBD.bd2.HiPart);

    BCI_SEND(BCI_CLIP_LR(x+skipleft, x+w-1));
    if( psav->SavedBgColor != 0xffffffff )
        BCI_SEND(psav->SavedBgColor);
    BCI_SEND(BCI_X_Y(x, y));
    BCI_SEND(BCI_W_H(w, h));
}

#endif
