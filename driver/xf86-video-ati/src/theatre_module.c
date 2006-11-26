#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "xf86Module.h"

static MODULESETUPPROTO(theatreSetup);


static XF86ModuleVersionInfo theatreVersRec =
{
        "theatre",
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
 
_X_EXPORT XF86ModuleData theatreModuleData = {
        &theatreVersRec,
        theatreSetup,
        NULL
}; 

static pointer
theatreSetup(pointer module, pointer opts, int *errmaj, int *errmin) {
   return (pointer)1;
}
