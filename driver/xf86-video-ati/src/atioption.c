/* $XFree86: xc/programs/Xserver/hw/xfree86/drivers/ati/atioption.c,v 1.22 2003/04/23 21:51:29 tsi Exp $ */
/*
 * Copyright 1999 through 2004 by Marc Aurele La France (TSI @ UQV), tsi@xfree86.org
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

#include "atioption.h"
#include "atiutil.h"

#include "radeon_probe.h"
#include "r128_probe.h"

#ifdef TV_OUT

/*
 * List of supported TV standard names
 */
const char *ATITVStandardNames[ATI_TV_STDS_MAX_VALID+1] = {
    "NTSC",
    "PAL",
    "PAL-M",
    "PAL-60",
    "NTSC-J",
    "PAL-CN",
    "PAL-N",
    "Reserved1",
    "Reserved2",
    "SCART-PAL",
    "None",
    "Invalid"
};

#endif /* TV_OUT */

/*
 * Recognised XF86Config options.
 */
const OptionInfoRec ATIPublicOptions[] =
{
    {
        ATI_OPTION_ACCEL,
        "accel",
        OPTV_BOOLEAN,
        {0, },
        FALSE
    },
    {
        ATI_OPTION_CRT_DISPLAY,
        "crt_display",
        OPTV_BOOLEAN,
        {0, },
        FALSE
    },
    {
        ATI_OPTION_CSYNC,
        "composite_sync",
        OPTV_BOOLEAN,
        {0, },
        FALSE
    },
    {
        ATI_OPTION_HWCURSOR,
        "hw_cursor",
        OPTV_BOOLEAN,
        {0, },
        FALSE,
    },

#ifndef AVOID_CPIO

    {
        ATI_OPTION_LINEAR,
        "linear",
        OPTV_BOOLEAN,
        {0, },
        FALSE
    },

#endif /* AVOID_CPIO */

#ifdef XF86DRI_DEVEL

    {
        ATI_OPTION_IS_PCI,
        "force_pci_mode",
        OPTV_BOOLEAN,
        {0, },
        FALSE,
    },
    {
        ATI_OPTION_DMA_MODE,
        "dma_mode",
        OPTV_STRING,
        {0, },
        FALSE,
    },
    {
        ATI_OPTION_AGP_MODE,
        "agp_mode",
        OPTV_INTEGER,
        {0, },
        FALSE,
    },
    {
        ATI_OPTION_AGP_SIZE,
        "agp_size",
        OPTV_INTEGER,
        {0, },
        FALSE,
    },
    {
        ATI_OPTION_LOCAL_TEXTURES,
        "local_textures",
        OPTV_BOOLEAN,
        {0, },
        FALSE,
    },
    {
        ATI_OPTION_BUFFER_SIZE,
        "buffer_size",
        OPTV_INTEGER,
        {0, },
        FALSE,
    },

#endif /* XF86DRI_DEVEL */

#ifdef TV_OUT
    {
        ATI_OPTION_TV_OUT,
        "tv_out",
        OPTV_BOOLEAN,
        {0, },
        FALSE
    },
    {
        ATI_OPTION_TV_STD,
        "tv_standard",
        OPTV_STRING,
        {0, },
        FALSE
    },

#endif /* TV_OUT */

    {
        ATI_OPTION_MMIO_CACHE,
        "mmio_cache",
        OPTV_BOOLEAN,
        {0, },
        FALSE
    },


    {
        ATI_OPTION_TEST_MMIO_CACHE,
        "test_mmio_cache",
        OPTV_BOOLEAN,
        {0, },
        FALSE
    },
    {
        ATI_OPTION_PANEL_DISPLAY,
        "panel_display",
        OPTV_BOOLEAN,
        {0, },
        FALSE
    },
    {
        ATI_OPTION_PROBE_CLOCKS,
        "probe_clocks",
        OPTV_BOOLEAN,
        {0, },
        FALSE
    },
    {
        ATI_OPTION_REFERENCE_CLOCK,
        "reference_clock",
        OPTV_FREQ,
        {0, },
        FALSE
    },
    {
        ATI_OPTION_SHADOW_FB,
        "shadow_fb",
        OPTV_BOOLEAN,
        {0, },
        FALSE
    },
    {
        ATI_OPTION_SWCURSOR,
        "sw_cursor",
        OPTV_BOOLEAN,
        {0, },
        FALSE,
    },
    {
        ATI_OPTION_ACCELMETHOD,
        "AccelMethod",
        OPTV_STRING,
        {0, },
        FALSE
    },
    {
        ATI_OPTION_RENDER_ACCEL,
        "RenderAccel",
        OPTV_BOOLEAN,
        {0, },
        FALSE
    },
    {
        -1,
        NULL,
        OPTV_NONE,
        {0, },
        FALSE
    }
};

const unsigned long ATIPublicOptionSize = SizeOf(ATIPublicOptions);

/*
 * ATIAvailableOptions --
 *
 * Return recognised options that are intended for public consumption.
 */
const OptionInfoRec *
ATIAvailableOptions
(
    int ChipId,
    int BusId
)
{
    const OptionInfoRec *pOptions;

    if ((pOptions = R128AvailableOptions(ChipId, BusId)))
        return pOptions;

    if ((pOptions = RADEONAvailableOptions(ChipId, BusId)))
        return pOptions;

    return ATIPublicOptions;
}
