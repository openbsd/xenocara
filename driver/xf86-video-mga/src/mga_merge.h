/* Merged mode stuff */
/* include file for mga_driver.c  DO NOT try to generate a .o with this file.*/

void		MGADisplayPowerManagementSetMerged(ScrnInfoPtr pScrn, 
					     int PowerManagementMode,
				             int flags);
void	        MGAMergePointerMoved(SCRN_ARG_TYPE arg, int x, int y);
void            MGAAdjustMergeFrames(ADJUST_FRAME_ARGS_DECL);
Bool            MGAPreInitMergedFB(ScrnInfoPtr pScrn1, int flags);
Bool		MGACloseScreenMerged(ScreenPtr pScreen);
Bool		MGASaveScreenMerged(ScreenPtr pScreen, int mode);

typedef struct _MergedDisplayModeRec {
    DisplayModePtr Monitor1;
    DisplayModePtr Monitor2;
    MgaScrn2Rel    Monitor2Pos; 
} MergedDisplayModeRec, *MergedDisplayModePtr;

#define MDMPTR(x) ((MergedDisplayModePtr)(x->currentMode->Private))
