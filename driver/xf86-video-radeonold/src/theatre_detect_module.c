/*
 * Created by Bogdan D. bogdand@users.sourceforge.net
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "xf86Module.h"

static MODULESETUPPROTO(theatre_detectSetup);


static XF86ModuleVersionInfo theatre_detectVersRec =
{
        "theatre_detect",
        MODULEVENDORSTRING,
        MODINFOSTRING1,
        MODINFOSTRING2,
        XORG_VERSION_CURRENT,
        1, 0, 0,
        ABI_CLASS_VIDEODRV,             /* This needs the video driver ABI */
        ABI_VIDEODRV_VERSION,
        MOD_CLASS_NONE,
        {0,0,0,0}
};
 
_X_EXPORT XF86ModuleData theatre_detectModuleData = {
        &theatre_detectVersRec,
        theatre_detectSetup,
        NULL
}; 

static pointer
theatre_detectSetup(pointer module, pointer opts, int *errmaj, int *errmin) {
   return (pointer)1;
}
