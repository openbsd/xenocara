/*
 * file rendition_shadow.h
 *
 * headfile for rendition_shadow.c
 */

#ifndef __RENDITION_SHADOW_H__
#define __RENDITION_SHADOW_H__

void renditionRefreshArea(ScrnInfoPtr pScrn, int num, BoxPtr pbox);
void renditionRefreshArea8(ScrnInfoPtr pScrn, int num, BoxPtr pbox);
void renditionRefreshArea16(ScrnInfoPtr pScrn, int num, BoxPtr pbox);
void renditionRefreshArea24(ScrnInfoPtr pScrn, int num, BoxPtr pbox);
void renditionRefreshArea32(ScrnInfoPtr pScrn, int num, BoxPtr pbox);
void renditionPointerMoved(int index, int x, int y);


#endif 

/*
 * end of file rendition_shadow.h
 */
