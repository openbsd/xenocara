void G80Sync(ScrnInfoPtr pScrn);
void G80DMAKickoffCallback(ScrnInfoPtr pScrn);
void G80SetPattern(G80Ptr pNv, int bg, int fg, int pat0, int pat1);
void G80SetRopSolid(G80Ptr pNv, CARD32 rop, CARD32 planemask);
void G80SetClip(G80Ptr pNv, int x, int y, int w, int h);
Bool G80XAAInit(ScreenPtr);
