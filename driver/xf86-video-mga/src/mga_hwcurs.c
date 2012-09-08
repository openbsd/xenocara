#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "xf86.h"
#include "xf86_OSproc.h"

#include "xf86Pci.h"

#include "mga.h"
#include "mga_reg.h"

Bool 
MGAHWCursorInit(ScreenPtr pScreen)
{
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
    MGAPtr pMga = MGAPTR(pScrn);
    MGARamdacPtr MGAdac = &pMga->Dac;
    xf86CursorInfoPtr infoPtr;

    if (!MGAdac->isHwCursor) 
        return FALSE;

    infoPtr = xf86CreateCursorInfoRec();
    if(!infoPtr) return FALSE;
    
    pMga->CursorInfoRec = infoPtr;

    infoPtr->MaxWidth = MGAdac->CursorMaxWidth;
    infoPtr->MaxHeight = MGAdac->CursorMaxHeight;
    infoPtr->Flags = MGAdac->CursorFlags;
    infoPtr->SetCursorColors = MGAdac->SetCursorColors;
    infoPtr->SetCursorPosition = MGAdac->SetCursorPosition;
    infoPtr->LoadCursorImage = MGAdac->LoadCursorImage;
    infoPtr->HideCursor = MGAdac->HideCursor;
    infoPtr->ShowCursor = MGAdac->ShowCursor;
    infoPtr->UseHWCursor = MGAdac->UseHWCursor;

    return(xf86InitCursor(pScreen, infoPtr));
}
