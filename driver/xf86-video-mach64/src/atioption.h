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

#ifndef ___ATIOPTION_H___
#define ___ATIOPTION_H___ 1

#include "xf86str.h"

/*
 * Documented XF86Config options.
 */
typedef enum
{
    ATI_OPTION_PROBE_SPARSE,
    ATI_OPTION_ACCEL,
    ATI_OPTION_CRT_DISPLAY,
    ATI_OPTION_CSYNC,
    ATI_OPTION_HWCURSOR,

#ifdef XF86DRI_DEVEL

    ATI_OPTION_IS_PCI,
    ATI_OPTION_DMA_MODE,
    ATI_OPTION_AGP_MODE,
    ATI_OPTION_AGP_SIZE,
    ATI_OPTION_LOCAL_TEXTURES,
    ATI_OPTION_BUFFER_SIZE,

#endif /* XF86DRI_DEVEL */

#ifdef TV_OUT

    ATI_OPTION_TV_OUT,
    ATI_OPTION_TV_STD,

#endif /* TV_OUT */

    ATI_OPTION_MMIO_CACHE,
    ATI_OPTION_TEST_MMIO_CACHE,
    ATI_OPTION_PANEL_DISPLAY,
    ATI_OPTION_REFERENCE_CLOCK,
    ATI_OPTION_SHADOW_FB,
    ATI_OPTION_SWCURSOR,
    ATI_OPTION_ACCELMETHOD,
    ATI_OPTION_RENDER_ACCEL
} ATIPublicOptionType;

#ifdef TV_OUT

#define ATI_TV_STDS_MAX_VALID   11
#define ATI_TV_STDS_NAME_MAXLEN 9

typedef enum {
    ATI_TV_STD_NTSC = 0,
    ATI_TV_STD_PAL,
    ATI_TV_STD_PALM,
    ATI_TV_STD_PAL60,
    ATI_TV_STD_NTSCJ,
    ATI_TV_STD_PALCN,
    ATI_TV_STD_PALN,
    ATI_TV_STD_RESERVED1, /* NOT usable */
    ATI_TV_STD_RESERVED2, /* NOT usable */
    ATI_TV_STD_SCARTPAL,
    ATI_TV_STD_NONE,      /* OK, means no tv standard change requested */
    ATI_TV_STD_INVALID    /* Invalid tv standard requested */
} ATITVStandard;

extern const char          * ATITVStandardNames[];

#endif /* TV_OUT */

extern const OptionInfoRec * ATIOptionsWeak(void);

extern void                  ATIProcessOptions(ScrnInfoPtr, ATIPtr);

#endif /* ___ATIOPTION_H___ */
