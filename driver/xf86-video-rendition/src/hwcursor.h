/*
 * file hwcursor.h
 */

#ifndef __HWCURSOR_H__
#define __HWCURSOR_H__

/*
 * functions prototypes
 */

void RenditionHWCursorPreInit(ScrnInfoPtr pScreenInfo);
Bool RenditionHWCursorInit(int scrnIndex, ScreenPtr pScreen);
void RenditionHWCursorRelease (ScrnInfoPtr pScreenInfo);

#define HC_SIZE  (64*64*2)/8  /* 1024 */

/* end of __HWCURSOR_H__ */
#endif 

/*
 * end of file hwcursor.h
 */

