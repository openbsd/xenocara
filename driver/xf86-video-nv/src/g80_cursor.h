Bool G80CursorInit(ScreenPtr);
Bool G80CursorAcquire(ScrnInfoPtr);
void G80CursorRelease(ScrnInfoPtr);

/* CRTC cursor functions */
void G80SetCursorPosition(xf86CrtcPtr crtc, int x, int y);
void G80LoadCursorARGB(xf86CrtcPtr crtc, CARD32 *src);
