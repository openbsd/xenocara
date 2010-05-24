enum G80ScaleMode {
    G80_SCALE_OFF,
    G80_SCALE_ASPECT,
    G80_SCALE_FILL,
    G80_SCALE_CENTER,
};

Bool G80DispPreInit(ScrnInfoPtr);
Bool G80DispInit(ScrnInfoPtr);
void G80DispShutdown(ScrnInfoPtr);

void G80DispCommand(ScrnInfoPtr, CARD32 addr, CARD32 data);
#define C(mthd, data) G80DispCommand(pScrn, (mthd), (data))

Head G80CrtcGetHead(xf86CrtcPtr);

void G80CrtcDoModeFixup(DisplayModePtr dst, const DisplayModePtr src);
void G80CrtcBlankScreen(xf86CrtcPtr, Bool blank);
void G80CrtcEnableCursor(xf86CrtcPtr, Bool update);
void G80CrtcDisableCursor(xf86CrtcPtr, Bool update);
void G80CrtcSetCursorPosition(xf86CrtcPtr, int x, int y);
void G80CrtcSkipModeFixup(xf86CrtcPtr);
void G80CrtcSetDither(xf86CrtcPtr, Bool dither, Bool update);
void G80CrtcSetScale(xf86CrtcPtr, DisplayModePtr, enum G80ScaleMode);
void G80LoadPalette(ScrnInfoPtr, int numColors, int *indices, LOCO *, VisualPtr);

void G80DispCreateCrtcs(ScrnInfoPtr pScrn);
