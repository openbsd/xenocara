/* $XFree86: xc/programs/Xserver/hw/xfree86/drivers/ati/atiload.c,v 1.15 2003/08/29 21:07:57 tsi Exp $ */
/*
 * Copyright 2000 through 2004 by Marc Aurele La France (TSI @ UQV), tsi@xfree86.org
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that copyright
 * notice and this permission notice appear in supporting documentation, and
 * that the name of Marc Aurele La France not be used in advertising or
 * publicity pertaining to distribution of the software without specific,
 * written prior permission.  Marc Aurele La France makes no representations
 * about the suitability of this software for any purpose.  It is provided
 * "as-is" without express or implied warranty.
 *
 * MARC AURELE LA FRANCE DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS.  IN NO
 * EVENT SHALL MARC AURELE LA FRANCE BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
 * DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 *
 * DRI support by:
 *    Leif Delgass <ldelgass@retinalburn.net>
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef XFree86LOADER

#include "ati.h"
#include "aticursor.h"
#include "atiload.h"
#include "atistruct.h"

/*
 * All symbol lists belong here.  They are externalised so that they can be
 * referenced elsewhere.  Note the naming convention for these things...
 */

const char *ATIint10Symbols[] =
{
    "xf86FreeInt10",
    "xf86InitInt10",
    "xf86int10Addr",
    NULL
};

const char *ATIddcSymbols[] =
{
    "xf86PrintEDID",
    "xf86SetDDCproperties",
    NULL
};

const char *ATIvbeSymbols[] =
{
    "VBEInit",
    "vbeDoEDID",
    "vbeFree",
    NULL
};

#ifndef AVOID_CPIO

const char *ATIxf1bppSymbols[] =
{
    "xf1bppScreenInit",
    NULL
};

const char *ATIxf4bppSymbols[] =
{
    "xf4bppScreenInit",
    NULL
};

#endif /* AVOID_CPIO */

#ifdef XF86DRI_DEVEL

const char *ATIdrmSymbols[] = {
    "drmAddBufs",
    "drmAddMap",
    "drmAgpAcquire",
    "drmAgpAlloc",
    "drmAgpBase",
    "drmAgpBind",
    "drmAgpDeviceId",
    "drmAgpEnable",
    "drmAgpFree",
    "drmAgpGetMode",
    "drmAgpRelease",
    "drmAgpUnbind",
    "drmAgpVendorId",
    "drmAvailable",
    "drmCommandNone",
    "drmCommandRead",
    "drmCommandWrite",
    "drmCommandWriteRead",
    "drmCtlInstHandler",
    "drmCtlUninstHandler",
    "drmFreeVersion",
    "drmGetInterruptFromBusID",
    "drmGetLibVersion",
    "drmGetVersion",
    "drmMap",
    "drmMapBufs",
    "drmDMA",
    "drmUnmap",
    "drmUnmapBufs",
    NULL
};

const char *ATIdriSymbols[] = {
    "DRICloseScreen",
    "DRICreateInfoRec",
    "DRIDestroyInfoRec",
    "DRIFinishScreenInit",
    "DRIGetSAREAPrivate",
    "DRILock",
    "DRIQueryVersion",
    "DRIScreenInit",
    "DRIUnlock",
    "GlxSetVisualConfigs",
    "DRICreatePCIBusID",
    NULL
};

#endif /* XF86DRI_DEVEL */

const char *ATIfbSymbols[] =
{
    "fbPictureInit",
    "fbScreenInit",
    NULL
};

const char *ATIshadowfbSymbols[] =
{
    "ShadowFBInit",
    NULL
};

#ifdef USE_EXA
const char *ATIexaSymbols[] =
{
    "exaDriverAlloc",
    "exaDriverInit",
    "exaDriverFini",
    "exaOffscreenAlloc",
    "exaOffscreenFree",
    NULL
};
#endif

#ifdef USE_XAA
const char *ATIxaaSymbols[] =
{
    "XAACreateInfoRec",
    "XAADestroyInfoRec",
    "XAAInit",
    NULL
};
#endif

const char *ATIramdacSymbols[] =
{
    "xf86CreateCursorInfoRec",
    "xf86DestroyCursorInfoRec",
    "xf86InitCursor",
    NULL
};

const char *ATIi2cSymbols[] =
{
    "xf86CreateI2CBusRec",
    "xf86DestroyI2CBusRec",
    "xf86I2CBusInit",
    "xf86I2CDevInit",
    "xf86I2CFindDev",
    "xf86I2CGetScreenBuses",
    NULL
};

/*
 * ATILoadModule --
 *
 * Load a specific module and register with the loader those of its entry
 * points that are referenced by this driver.
 */
pointer
ATILoadModule
(
    ScrnInfoPtr  pScreenInfo,
    const char  *Module,
    const char **SymbolList
)
{
    pointer pModule = xf86LoadSubModule(pScreenInfo, Module);

    if (pModule)
        xf86LoaderReqSymLists(SymbolList, NULL);

    return pModule;
}

/*
 * ATILoadModules --
 *
 * This function loads other modules required for a screen.
 */
pointer
ATILoadModules
(
    ScrnInfoPtr pScreenInfo,
    ATIPtr      pATI
)
{
    pointer fbPtr = NULL;

    /* Load shadow frame buffer code if needed */
    if (pATI->OptionShadowFB &&
        !ATILoadModule(pScreenInfo, "shadowfb", ATIshadowfbSymbols))
        return NULL;

    /* Load depth-specific entry points */
    switch (pATI->bitsPerPixel)
    {

#ifndef AVOID_CPIO

        case 1:
            fbPtr = ATILoadModule(pScreenInfo, "xf1bpp", ATIxf1bppSymbols);
            break;

        case 4:
            fbPtr = ATILoadModule(pScreenInfo, "xf4bpp", ATIxf4bppSymbols);
            break;

#endif /* AVOID_CPIO */

        case 8:
        case 16:
        case 24:
        case 32:
            fbPtr = ATILoadModule(pScreenInfo, "fb", ATIfbSymbols);
            break;

        default:
            return NULL;
    }
    if (!fbPtr)
        return NULL;

    /* Load ramdac module if needed */
    if ((pATI->Cursor > ATI_CURSOR_SOFTWARE) &&
        !ATILoadModule(pScreenInfo, "ramdac", ATIramdacSymbols))
        return NULL;

#ifdef USE_EXA
    /* Load EXA if needed */
    if (pATI->useEXA && pATI->OptionAccel)
    {
        /* Cannot use ATILoadModule(), because of version checking */
        XF86ModReqInfo req;
        int errmaj, errmin;

        memset(&req, 0, sizeof(XF86ModReqInfo));
        req.majorversion = 2;
        req.minorversion = 0;
        if (!LoadSubModule(pScreenInfo->module, "exa", NULL, NULL, NULL, &req,
            &errmaj, &errmin))
        {
            LoaderErrorMsg(NULL, "exa", errmaj, errmin);
            return NULL;
        }
        xf86LoaderReqSymLists(ATIexaSymbols, NULL);
    }
#endif
#ifdef USE_XAA
    /* Load XAA if needed */
    if (!pATI->useEXA && pATI->OptionAccel &&
        !ATILoadModule(pScreenInfo, "xaa", ATIxaaSymbols))
        return NULL;
#endif

    return fbPtr;
}

#endif /* XFree86LOADER */
