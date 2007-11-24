/* -*- c-basic-offset: 4 -*- */

#ifdef HAVE_XORG_CONFIG_H
#include <xorg-config.h>
#endif

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "xf86Module.h"

static MODULESETUPPROTO(tfp410Setup);

static XF86ModuleVersionInfo tfp410VersRec = {
    "tfp410",
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

_X_EXPORT XF86ModuleData tfp410ModuleData = {
    &tfp410VersRec,
    tfp410Setup,
    NULL
};

static pointer
tfp410Setup(pointer module, pointer opts, int *errmaj, int *errmin)
{
    return (pointer)1;
}
