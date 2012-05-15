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
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "ati.h"
#include "aticursor.h"
#include "atiload.h"
#include "atistruct.h"

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

    /* Load depth-specific entry points */
    switch (pATI->bitsPerPixel)
    {
        case 8:
        case 16:
        case 24:
        case 32:
            fbPtr = xf86LoadSubModule(pScreenInfo, "fb");
            break;

        default:
            return NULL;
    }
    if (!fbPtr)
        return NULL;

    /* Load ramdac module if needed */
    if ((pATI->Cursor > ATI_CURSOR_SOFTWARE) &&
        !xf86LoadSubModule(pScreenInfo, "ramdac"))
        return NULL;

#ifdef USE_EXA
    /* Load EXA if needed */
    if (pATI->useEXA && pATI->OptionAccel)
    {
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
    }
#endif
#ifdef USE_XAA
    /* Load XAA if needed */
    if (!pATI->useEXA && pATI->OptionAccel &&
        !xf86LoadSubModule(pScreenInfo, "xaa")) {
	xf86DrvMsg(pScreenInfo->scrnIndex, X_INFO,
		   "Falling back to shadowfb\n");
	pATI->OptionAccel = 0;
	pATI->OptionShadowFB = 1;
    }
#endif

    /* Load shadow frame buffer code if needed */
    if (pATI->OptionShadowFB &&
        !xf86LoadSubModule(pScreenInfo, "shadowfb"))
        return NULL;

    return fbPtr;
}
