#ifndef __RIVA_PROTO_H__
#define __RIVA_PROTO_H__

/* in riva_driver.c */
Bool    RivaSwitchMode(SWITCH_MODE_ARGS_DECL);
void    RivaAdjustFrame(ADJUST_FRAME_ARGS_DECL);
Bool    RivaI2CInit(ScrnInfoPtr pScrn);
const   OptionInfoRec * RivaAvailableOptions(int chipid, int busid);
Bool    RivaGetScrnInfoRec(PciChipsets *chips, int chip);

/* in riva_dac.c */
Bool    RivaDACInit(ScrnInfoPtr pScrn, DisplayModePtr mode);
void    RivaDACSave(ScrnInfoPtr pScrn, vgaRegPtr vgaReg,
                  RivaRegPtr rivaReg, Bool saveFonts);
void    RivaDACRestore(ScrnInfoPtr pScrn, vgaRegPtr vgaReg,
                     RivaRegPtr rivaReg, Bool restoreFonts);
void    RivaDACLoadPalette(ScrnInfoPtr pScrn, int numColors, int *indices,
                         LOCO *colors, VisualPtr pVisual );
Bool    RivaDACi2cInit(ScrnInfoPtr pScrn);


/* in riva_setup.c */
void    RivaEnterLeave(ScrnInfoPtr pScrn, Bool enter);
void    Riva3Setup(ScrnInfoPtr pScrn);

/* in riva_cursor.c */
Bool    RivaCursorInit(ScreenPtr pScreen);

/* in riva_xaa.c */
Bool    RivaAccelInit(ScreenPtr pScreen);
void    RivaSync(ScrnInfoPtr pScrn);
void    RivaResetGraphics(ScrnInfoPtr pScrn);

/* in riva_dga.c */
Bool    RivaDGAInit(ScreenPtr pScreen);


#endif /* __RIVA_PROTO_H__ */
