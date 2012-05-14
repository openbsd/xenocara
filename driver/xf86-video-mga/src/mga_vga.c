#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <unistd.h>

#include "misc.h"
#include "xf86.h"
#include "xf86_OSproc.h"
#include "vgaHW.h"
#include "compiler.h"
#include "xf86cmap.h"
#include "mga.h"
#include "mga_reg.h"

#define TEXT_AMOUNT 16384
#define FONT_AMOUNT (8*8192)

void
MGAG200SERestoreFonts(ScrnInfoPtr scrninfp, vgaRegPtr restore)
{
    vgaHWPtr hwp = VGAHWPTR(scrninfp);
    MGAPtr pMga = MGAPTR(scrninfp);
    int savedIOBase;
    unsigned char miscOut, attr10, gr1, gr3, gr4, gr5, gr6, gr8, seq2, seq4;
    Bool doMap = FALSE;
    unsigned char scrn;

    /* If nothing to do, return now */
    if (!hwp->FontInfo1 && !hwp->FontInfo2 && !hwp->TextInfo)
	return;

    if (hwp->Base == NULL) {
	doMap = TRUE;
	if (!vgaHWMapMem(scrninfp)) {
	    xf86DrvMsg(scrninfp->scrnIndex, X_ERROR,
		    "vgaHWRestoreFonts: vgaHWMapMem() failed\n");
	    return;
	}
    }

    /* save the registers that are needed here */
    miscOut = hwp->readMiscOut(hwp);
    attr10 = hwp->readAttr(hwp, 0x10);
    gr1 = hwp->readGr(hwp, 0x01);
    gr3 = hwp->readGr(hwp, 0x03);
    gr4 = hwp->readGr(hwp, 0x04);
    gr5 = hwp->readGr(hwp, 0x05);
    gr6 = hwp->readGr(hwp, 0x06);
    gr8 = hwp->readGr(hwp, 0x08);
    seq2 = hwp->readSeq(hwp, 0x02);
    seq4 = hwp->readSeq(hwp, 0x04);

    /* save hwp->IOBase and temporarily set it for colour mode */
    savedIOBase = hwp->IOBase;
    hwp->IOBase = VGA_IOBASE_COLOR;

    /* Force into colour mode */
    hwp->writeMiscOut(hwp, miscOut | 0x01);

    scrn = hwp->readSeq(hwp, 0x01);
    scrn |= 0x20;/* blank screen */
    vgaHWSeqReset(hwp, TRUE);
    MGAWAITVSYNC();
    MGAWAITBUSY();
    hwp->writeSeq(hwp, 0x01, scrn);/* change mode */
    usleep(20000);
    vgaHWSeqReset(hwp, FALSE);

    /*
     * here we temporarily switch to 16 colour planar mode, to simply
     * copy the font-info and saved text.
     *
     * BUG ALERT: The (S)VGA's segment-select register MUST be set correctly!
     */
#if 0
    hwp->writeAttr(hwp, 0x10, 0x01);   /* graphics mode */
#endif
    if (scrninfp->depth == 4) {
	/* GJA */
	hwp->writeGr(hwp, 0x03, 0x00);  /* don't rotate, write unmodified */
	hwp->writeGr(hwp, 0x08, 0xFF);  /* write all bits in a byte */
	hwp->writeGr(hwp, 0x01, 0x00);  /* all planes come from CPU */
    }

    hwp->writeSeq(hwp, 0x04, 0x06); /* enable plane graphics */
    hwp->writeGr(hwp, 0x05, 0x00);  /* write mode 0, read mode 0 */
    hwp->writeGr(hwp, 0x06, 0x05);  /* set graphics */

    if (hwp->FontInfo1) {
	hwp->writeSeq(hwp, 0x02, 0x04); /* write to plane 2 */
	hwp->writeGr(hwp, 0x04, 0x02);  /* read plane 2 */
	slowbcopy_tobus(hwp->FontInfo1, hwp->Base, FONT_AMOUNT);
    }

    if (hwp->FontInfo2) {
	hwp->writeSeq(hwp, 0x02, 0x08); /* write to plane 3 */
	hwp->writeGr(hwp, 0x04, 0x03);  /* read plane 3 */
	slowbcopy_tobus(hwp->FontInfo2, hwp->Base, FONT_AMOUNT);
    }

    if (hwp->TextInfo) {
	hwp->writeSeq(hwp, 0x02, 0x01); /* write to plane 0 */
	hwp->writeGr(hwp, 0x04, 0x00);  /* read plane 0 */
	slowbcopy_tobus(hwp->TextInfo, hwp->Base, TEXT_AMOUNT);
	hwp->writeSeq(hwp, 0x02, 0x02); /* write to plane 1 */
	hwp->writeGr(hwp, 0x04, 0x01);  /* read plane 1 */
	slowbcopy_tobus((unsigned char *)hwp->TextInfo + TEXT_AMOUNT,
		hwp->Base, TEXT_AMOUNT);
    }

    /* restore the registers that were changed */
    hwp->writeMiscOut(hwp, miscOut);
    hwp->writeAttr(hwp, 0x10, attr10);
    hwp->writeGr(hwp, 0x01, gr1);
    hwp->writeGr(hwp, 0x03, gr3);
    hwp->writeGr(hwp, 0x04, gr4);
    hwp->writeGr(hwp, 0x05, gr5);
    hwp->writeGr(hwp, 0x06, gr6);
    hwp->writeGr(hwp, 0x08, gr8);
    hwp->writeSeq(hwp, 0x02, seq2);
    hwp->writeSeq(hwp, 0x04, seq4);
    hwp->IOBase = savedIOBase;

    scrn = hwp->readSeq(hwp, 0x01);
    scrn &= ~0x20;/* enable screen */
    vgaHWSeqReset(hwp, TRUE);
    MGAWAITVSYNC();
    MGAWAITBUSY();
    hwp->writeSeq(hwp, 0x01, scrn);/* change mode */
    usleep(20000);
    vgaHWSeqReset(hwp, FALSE);

    if (doMap)
	vgaHWUnmapMem(scrninfp);
}


void
MGAG200SESaveFonts(ScrnInfoPtr scrninfp, vgaRegPtr save)
{
    vgaHWPtr hwp = VGAHWPTR(scrninfp);
    MGAPtr pMga = MGAPTR(scrninfp);
    int savedIOBase;
    unsigned char miscOut, attr10, gr4, gr5, gr6, seq2, seq4;
    Bool doMap = FALSE;
    unsigned char scrn;

    if (hwp->Base == NULL) {
	doMap = TRUE;
	if (!vgaHWMapMem(scrninfp)) {
	    xf86DrvMsg(scrninfp->scrnIndex, X_ERROR,
		    "vgaHWSaveFonts: vgaHWMapMem() failed\n");
	    return;
	}
    }

    /* If in graphics mode, don't save anything */
    attr10 = hwp->readAttr(hwp, 0x10);
    if (attr10 & 0x01)
	return;

    /* save the registers that are needed here */
    miscOut = hwp->readMiscOut(hwp);
    gr4 = hwp->readGr(hwp, 0x04);
    gr5 = hwp->readGr(hwp, 0x05);
    gr6 = hwp->readGr(hwp, 0x06);
    seq2 = hwp->readSeq(hwp, 0x02);
    seq4 = hwp->readSeq(hwp, 0x04);

    /* save hwp->IOBase and temporarily set it for colour mode */
    savedIOBase = hwp->IOBase;
    hwp->IOBase = VGA_IOBASE_COLOR;

    /* Force into colour mode */
    hwp->writeMiscOut(hwp, miscOut | 0x01);

    scrn = hwp->readSeq(hwp, 0x01);
    scrn |= 0x20;/* blank screen */
    vgaHWSeqReset(hwp, TRUE);
    MGAWAITVSYNC();
    MGAWAITBUSY();
    hwp->writeSeq(hwp, 0x01, scrn);/* change mode */
    usleep(20000);
    vgaHWSeqReset(hwp, FALSE);

    /*
     * get the character sets, and text screen if required
     */
    /*
     * Here we temporarily switch to 16 colour planar mode, to simply
     * copy the font-info
     *
     * BUG ALERT: The (S)VGA's segment-select register MUST be set correctly!
     */
#if 0
    hwp->writeAttr(hwp, 0x10, 0x01);   /* graphics mode */
#endif
    hwp->writeSeq(hwp, 0x04, 0x06); /* enable plane graphics */
    hwp->writeGr(hwp, 0x05, 0x00);  /* write mode 0, read mode 0 */
    hwp->writeGr(hwp, 0x06, 0x05);  /* set graphics */
    if (hwp->FontInfo1 || (hwp->FontInfo1 = malloc(FONT_AMOUNT))) {
	hwp->writeSeq(hwp, 0x02, 0x04); /* write to plane 2 */
	hwp->writeGr(hwp, 0x04, 0x02);  /* read plane 2 */
	slowbcopy_frombus(hwp->Base, hwp->FontInfo1, FONT_AMOUNT);
    }
    if (hwp->FontInfo2 || (hwp->FontInfo2 = malloc(FONT_AMOUNT))) {
	hwp->writeSeq(hwp, 0x02, 0x08); /* write to plane 3 */
	hwp->writeGr(hwp, 0x04, 0x03);  /* read plane 3 */
	slowbcopy_frombus(hwp->Base, hwp->FontInfo2, FONT_AMOUNT);
    }
    if (hwp->TextInfo || (hwp->TextInfo = malloc(2 * TEXT_AMOUNT))) {
	hwp->writeSeq(hwp, 0x02, 0x01); /* write to plane 0 */
	hwp->writeGr(hwp, 0x04, 0x00);  /* read plane 0 */
	slowbcopy_frombus(hwp->Base, hwp->TextInfo, TEXT_AMOUNT);
	hwp->writeSeq(hwp, 0x02, 0x02); /* write to plane 1 */
	hwp->writeGr(hwp, 0x04, 0x01);  /* read plane 1 */
	slowbcopy_frombus(hwp->Base,
		(unsigned char *)hwp->TextInfo + TEXT_AMOUNT, TEXT_AMOUNT);
    }

    /* Restore clobbered registers */
    hwp->writeAttr(hwp, 0x10, attr10);
    hwp->writeGr(hwp, 0x04, gr4);
    hwp->writeGr(hwp, 0x05, gr5);
    hwp->writeGr(hwp, 0x06, gr6);
    hwp->writeSeq(hwp, 0x02, seq2);
    hwp->writeSeq(hwp, 0x04, seq4);
    hwp->writeMiscOut(hwp, miscOut);
    hwp->IOBase = savedIOBase;

    scrn = hwp->readSeq(hwp, 0x01);
    scrn &= ~0x20;/* enable screen */
    vgaHWSeqReset(hwp, TRUE);
    MGAWAITVSYNC();
    MGAWAITBUSY();
    hwp->writeSeq(hwp, 0x01, scrn);/* change mode */
    usleep(20000);
    vgaHWSeqReset(hwp, FALSE);

    if (doMap)
	vgaHWUnmapMem(scrninfp);
}

void
MGAG200SERestoreMode(ScrnInfoPtr scrninfp, vgaRegPtr restore)
{
    vgaHWPtr hwp = VGAHWPTR(scrninfp);
    MGAPtr pMga = MGAPTR(scrninfp);
    int i;
    unsigned char scrn;

    if (restore->MiscOutReg & 0x01)
    hwp->IOBase = VGA_IOBASE_COLOR;
    else
    hwp->IOBase = VGA_IOBASE_MONO;

    hwp->writeMiscOut(hwp, restore->MiscOutReg);


    for (i = 1; i < restore->numSequencer; i++)
    {
    MGAWAITVSYNC();
    MGAWAITBUSY();
	hwp->writeSeq(hwp, i, restore->Sequencer[i]);
	usleep(20000);
    }

    scrn = hwp->readSeq(hwp, 0x01);
    scrn |= 0x20;/* blank screen */
    vgaHWSeqReset(hwp, TRUE);
    MGAWAITVSYNC();
    MGAWAITBUSY();
    hwp->writeSeq(hwp, 0x01, scrn);/* change mode */
    usleep(20000);

    /* Ensure CRTC registers 0-7 are unlocked by clearing bit 7 of CRTC[17] */
    hwp->writeCrtc(hwp, 17, restore->CRTC[17] & ~0x80);

    for (i = 0; i < restore->numCRTC; i++)
	hwp->writeCrtc(hwp, i, restore->CRTC[i]);

    for (i = 0; i < restore->numGraphics; i++)
    hwp->writeGr(hwp, i, restore->Graphics[i]);

    hwp->enablePalette(hwp);
    for (i = 0; i < restore->numAttribute; i++)
    hwp->writeAttr(hwp, i, restore->Attribute[i]);
    hwp->disablePalette(hwp);

    MGAWAITVSYNC();
    MGAWAITBUSY();
	hwp->writeSeq(hwp, 1, restore->Sequencer[1]);
    usleep(20000);
}

void
MGAG200SESaveMode(ScrnInfoPtr scrninfp, vgaRegPtr save)
{
    vgaHWPtr hwp = VGAHWPTR(scrninfp);
    int i;

    save->MiscOutReg = hwp->readMiscOut(hwp);
    if (save->MiscOutReg & 0x01)
    hwp->IOBase = VGA_IOBASE_COLOR;
    else
    hwp->IOBase = VGA_IOBASE_MONO;

    for (i = 0; i < save->numCRTC; i++) {
    save->CRTC[i] = hwp->readCrtc(hwp, i);
#ifdef DEBUG
    ErrorF("CRTC[0x%02x] = 0x%02x\n", i, save->CRTC[i]);
#endif
    }

    hwp->enablePalette(hwp);
    for (i = 0; i < save->numAttribute; i++) {
    save->Attribute[i] = hwp->readAttr(hwp, i);
#ifdef DEBUG
    ErrorF("Attribute[0x%02x] = 0x%02x\n", i, save->Attribute[i]);
#endif
    }
    hwp->disablePalette(hwp);

    for (i = 0; i < save->numGraphics; i++) {
    save->Graphics[i] = hwp->readGr(hwp, i);
#ifdef DEBUG
    ErrorF("Graphics[0x%02x] = 0x%02x\n", i, save->Graphics[i]);
#endif
    }

    for (i = 1; i < save->numSequencer; i++) {
    save->Sequencer[i] = hwp->readSeq(hwp, i);
#ifdef DEBUG
    ErrorF("Sequencer[0x%02x] = 0x%02x\n", i, save->Sequencer[i]);
#endif
    }
}

void
MGAG200SEHWProtect(ScrnInfoPtr pScrn, Bool on)
{
  vgaHWPtr hwp = VGAHWPTR(pScrn);
  MGAPtr pMga = MGAPTR(pScrn);
  
  unsigned char tmp;
  
  if (pScrn->vtSema) {
    if (on) {
      /*
       * Turn off screen and disable sequencer.
       */
      tmp = hwp->readSeq(hwp, 0x01);

      vgaHWSeqReset(hwp, TRUE);         /* start synchronous reset */
      MGAWAITVSYNC();
      MGAWAITBUSY();
      hwp->writeSeq(hwp, 0x01, tmp | 0x20); /* disable the display */
      usleep(20000);
      hwp->enablePalette(hwp);
    } else {
      /*
       * Reenable sequencer, then turn on screen.
       */
  
      tmp = hwp->readSeq(hwp, 0x01);

      MGAWAITVSYNC();
      MGAWAITBUSY();
      hwp->writeSeq(hwp, 0x01, tmp & ~0x20);    /* reenable display */
      usleep(20000);
      vgaHWSeqReset(hwp, FALSE);        /* clear synchronousreset */

      hwp->disablePalette(hwp);
    }
  }
}
