#ifdef HAVE_XORG_CONFIG_H
#include <xorg-config.h>
#endif

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "xf86Module.h"

static MODULESETUPPROTO(ch7xxxSetup);

static XF86ModuleVersionInfo ch7xxxVersRec =  {
    "ch7xxx",
    MODULEVENDORSTRING,
    MODINFOSTRING1,
    MODINFOSTRING2,
    XORG_VERSION_CURRENT,
    1, 0, 0,
    ABI_CLASS_VIDEODRV,
    ABI_VIDEODRV_VERSION,
    MOD_CLASS_NONE,
    { 0,0,0,0 }
};

_X_EXPORT XF86ModuleData ch7xxxModuleData = {
    &ch7xxxVersRec,
    ch7xxxSetup,
    NULL
};

static pointer
ch7xxxSetup(pointer module, pointer opts, int *errmaj, int *errmin) {
    return (pointer)1;
}
