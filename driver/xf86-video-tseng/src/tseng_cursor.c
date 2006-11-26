/* $XFree86: xc/programs/Xserver/hw/xfree86/drivers/tseng/tseng_cursor.c,v 1.16 2000/09/19 12:46:19 eich Exp $ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "tseng.h"

/*
 *
 * ET4000 HWCursor is disabled currently: we need to find a way to set the
 * sprite colours.
 *
 */

/*
 * ET4000W32 sprite engine.
 */
static CARD8
ET4000CursorRead(CARD8 Index)
{
    outb(0x217A, Index);
    return inb(0x217B);
}

static void
ET4000CursorWrite(CARD8 Index, CARD8 Value)
{
    outb(0x217A, Index);
    outb(0x217B, Value);
}

/*
 *
 */
static Bool
TsengUseHWCursor(ScreenPtr pScreen, CursorPtr pCurs)
{
    /* have this return false for DoubleScan and Interlaced ? */
    return TRUE;
}

static void
TsengShowCursor(ScrnInfoPtr pScrn)
{
    unsigned char tmp;
    TsengPtr pTseng = TsengPTR(pScrn);

    /* Enable the hardware cursor. */
    if (pTseng->ChipType == ET6000) {
	tmp = ET6000IORead(pTseng, 0x46);
	ET6000IOWrite(pTseng, 0x46, (tmp | 0x01));
    } else {
	tmp = ET4000CursorRead(0xF7);
        ET4000CursorWrite(0xF7, tmp | 0x80);
    }
}

static void
TsengHideCursor(ScrnInfoPtr pScrn)
{
    unsigned char tmp;
    TsengPtr pTseng = TsengPTR(pScrn);

    /* Disable the hardware cursor. */
    if (pTseng->ChipType == ET6000) {
	tmp = ET6000IORead(pTseng, 0x46);
	ET6000IOWrite(pTseng, 0x46, (tmp & 0xfe));
    } else {
	tmp = ET4000CursorRead(0xF7);
        ET4000CursorWrite(0xF7, tmp & ~0x80);
    }
}

static void
TsengSetCursorPosition(ScrnInfoPtr pScrn, int x, int y)
{
    int xorigin, yorigin;
    TsengPtr pTseng = TsengPTR(pScrn);

    /*
     * If the cursor is partly out of screen at the left or top,
     * we need to modify the origin.
     */
    xorigin = 0;
    yorigin = 0;
    if (x < 0) {
	xorigin = -x;
	x = 0;
    }
    if (y < 0) {
	yorigin = -y;
	y = 0;
    }
#ifdef TODO
    /* Correct cursor position in DoubleScan modes */
    if (XF86SCRNINFO(pScr)->modes->Flags & V_DBLSCAN)
	y *= 2;
#endif

    if (pTseng->ChipType == ET6000) {
	ET6000IOWrite(pTseng, 0x82, xorigin);
	ET6000IOWrite(pTseng, 0x83, yorigin);

	ET6000IOWrite(pTseng, 0x84, (x & 0xff));
	ET6000IOWrite(pTseng, 0x85, ((x >> 8) & 0x0f));

	ET6000IOWrite(pTseng, 0x86, (y & 0xff));
	ET6000IOWrite(pTseng, 0x87, ((y >> 8) & 0x0f));
    } else {
	ET4000CursorWrite(0xE2, xorigin);
        ET4000CursorWrite(0xE6, yorigin);

	ET4000CursorWrite(0xE0, x & 0xFF);
        ET4000CursorWrite(0xE1, (x >> 8) & 0x0F);

        ET4000CursorWrite(0xE4, y & 0xFF);
        ET4000CursorWrite(0xE5, (y >> 8) & 0x0F);
    }
}

/*
 * The ET6000 cursor color is only 6 bits, with 2 bits per color. This
 * is of course very inaccurate, but high-bit-depth color differences
 * are only visible on _large_ planes of equal color. i.e. small areas
 * of a certain color (like a cursor) don't need many bits per pixel at
 * all, because the difference will not be seen.
 * 
 * So it won't be as bad, but should still be documented nonetheless.
 */
static void
TsengSetCursorColors(ScrnInfoPtr pScrn, int bg, int fg)
{
    TsengPtr pTseng = TsengPTR(pScrn);
    unsigned char et6k_fg, et6k_bg;

    if (pTseng->ChipType == ET6000) {
	et6k_fg = (fg & 0x00000003)
	    | ((fg & 0x00000300) >> 6)
	    | ((fg & 0x00030000) >> 12);
	et6k_bg = (bg & 0x00000003)
	    | ((bg & 0x00000300) >> 6)
	    | ((bg & 0x00030000) >> 12);

	ET6000IOWrite(pTseng, 0x67, 0x09);	/* prepare for colour data */
	ET6000IOWrite(pTseng, 0x69, et6k_bg);
	ET6000IOWrite(pTseng, 0x69, et6k_fg);
    } else {
	/*
	 * The ET4000 uses color 0 as sprite color "0", and color 0xFF as
	 * sprite color "1". Changing colors implies changing colors 0 and
	 * 255. This is currently not implemented.
	 *
	 * In non-8bpp modes, this would result in always black and white
	 * colors (since the colormap isn't there to translate 0 and 255 to
	 * other colors). And besides, in non-8bpp, there seem to be TWO
	 * cursor images on the screen...
	 */
	xf86Msg(X_ERROR, "Internal error: ET4000 hardware cursor color changes not implemented\n");
    }
}

void 
TsengLoadCursorImage(ScrnInfoPtr pScrn, unsigned char *bits)
{
    vgaHWPtr hwp = VGAHWPTR(pScrn);
    TsengPtr pTseng = TsengPTR(pScrn);
    CARD8 tmp;

#ifdef DEBUG_HWC
    int i;
    int d;

    for (i = 0; i < 1024; i++) {
	d = *(bits + i);
	ErrorF("%d%d%d%d", d & 0x03, (d >> 2) & 0x03, (d >> 4) & 0x03, (d >> 6) & 0x03);
	if ((i & 15) == 15)
	    ErrorF("\n");
    }
#endif

    /*
     * Program the cursor image address in video memory. 
     * We need to set it here or we might loose it on mode/vt switches.
     */

    if (pTseng->ChipType == ET6000) {
	/* bits 19:16 */
	tmp = hwp->readCrtc(hwp, 0x0E) & 0xF0;
	tmp |= ((pTseng->HWCursorBufferOffset / 4) >> 16) & 0x0F;
        hwp->writeCrtc(hwp, 0x0E, tmp);

	/* bits 15:8 */
	hwp->writeCrtc(hwp, 0x0F, ((pTseng->HWCursorBufferOffset / 4) >> 8) & 0xFF);
	/* on the ET6000, bits (7:0) are always 0 */
    } else {
	/* bits 19:16 */
	tmp = ET4000CursorRead(0xEA) & 0xF0;
	ET4000CursorWrite(0xEA, tmp | (((pTseng->HWCursorBufferOffset / 4) >> 16) & 0x0F));
	/* bits 15:8 */
        ET4000CursorWrite(0xE9, ((pTseng->HWCursorBufferOffset / 4) >> 8) & 0xFF);
	/* bits 7:0 */
        ET4000CursorWrite(0xE8, (pTseng->HWCursorBufferOffset / 4) & 0xFF);

	/* this needs to be set for the sprite */
	ET4000CursorWrite(0xEB, 0x02);

        tmp = ET4000CursorRead(0xEC);
        ET4000CursorWrite(0xEC, tmp & 0xFE);

        tmp = ET4000CursorRead(0xEF) & 0xF8;
        ET4000CursorWrite(0xEF, tmp | 0x02);

        ET4000CursorWrite(0xEE, 0x01);
    }
    /* this assumes the apertures have been set up correctly for banked mode */
    memcpy(pTseng->HWCursorBuffer, bits, 1024);
}

/*
  *
  *
  */
Bool 
TsengHWCursorInit(ScreenPtr pScreen)
{
    ScrnInfoPtr pScrn = xf86Screens[pScreen->myNum];
    TsengPtr pTseng = TsengPTR(pScrn);
    xf86CursorInfoPtr infoPtr;

    PDEBUG("	TsengHWCursorInit\n");

    if (!pTseng->HWCursor)
	return FALSE;

    infoPtr = xf86CreateCursorInfoRec();
    if (!infoPtr)
	return FALSE;

    pTseng->CursorInfoRec = infoPtr;

    /* calculate memory addres from video memory offsets */
    pTseng->HWCursorBuffer =
	pTseng->FbBase + pTseng->HWCursorBufferOffset;

    /* set up the XAA HW cursor structure */
    infoPtr->MaxWidth = 64;
    infoPtr->MaxHeight = 64;
    infoPtr->Flags =
	HARDWARE_CURSOR_TRUECOLOR_AT_8BPP |
	HARDWARE_CURSOR_SOURCE_MASK_INTERLEAVE_1 |
	HARDWARE_CURSOR_AND_SOURCE_WITH_MASK |
	HARDWARE_CURSOR_INVERT_MASK;
    infoPtr->SetCursorColors = TsengSetCursorColors;
    infoPtr->SetCursorPosition = TsengSetCursorPosition;
    infoPtr->LoadCursorImage = TsengLoadCursorImage;
    infoPtr->HideCursor = TsengHideCursor;
    infoPtr->ShowCursor = TsengShowCursor;
    infoPtr->UseHWCursor = TsengUseHWCursor;

    return (xf86InitCursor(pScreen, infoPtr));
}

/*
 *
 */
void
TsengCursorStore(ScrnInfoPtr pScrn, TsengRegPtr Reg)
{
    TsengPtr pTseng = TsengPTR(pScrn);

    if (pTseng->ChipType == ET4000)
        Reg->CursorCtrl = ET4000CursorRead(0xF7);
}

/*
 *
 */
void
TsengCursorRestore(ScrnInfoPtr pScrn, TsengRegPtr Reg)
{
    TsengPtr pTseng = TsengPTR(pScrn);
    
    if (pTseng->ChipType == ET4000)
        ET4000CursorWrite(0xF7, Reg->CursorCtrl);
}
