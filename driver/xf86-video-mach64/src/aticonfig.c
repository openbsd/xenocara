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

#include <string.h>

#include "ati.h"
#include "atichip.h"
#include "aticursor.h"
#include "atioption.h"
#include "atistruct.h"

#include "mach64_common.h"

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
static const OptionInfoRec ATIPublicOptions[] =
{
    {
        ATI_OPTION_PROBE_SPARSE,
        "probe_sparse",
        OPTV_BOOLEAN,
        {0, },
        FALSE
    },
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

static const unsigned long ATIPublicOptionSize = SizeOf(ATIPublicOptions);

const OptionInfoRec *
ATIOptionsWeak(void) { return ATIPublicOptions; }

/*
 * Non-publicised XF86Config options.
 */
typedef enum
{
    ATI_OPTION_BIOS_DISPLAY,    /* Allow BIOS interference */
    ATI_OPTION_CRT_SCREEN,      /* Legacy negation of "PanelDisplay" */
    ATI_OPTION_DEVEL,           /* Intentionally undocumented */
    ATI_OPTION_BLEND,           /* Force horizontal blending of small modes */
    ATI_OPTION_LCDSYNC          /* Use XF86Config panel mode porches */
} ATIPrivateOptionType;

/*
 * ATIProcessOptions --
 *
 * This function extracts options from what was parsed out of the XF86Config
 * file.
 */
void
ATIProcessOptions
(
    ScrnInfoPtr pScreenInfo,
    ATIPtr      pATI
)
{
    OptionInfoPtr PublicOption = xnfalloc(ATIPublicOptionSize);
    OptionInfoRec PrivateOption[] =
    {
        {                       /* ON:  Let BIOS change display(s) */
            ATI_OPTION_BIOS_DISPLAY,    /* OFF:  Don't */
            "biosdisplay",
            OPTV_BOOLEAN,
            {0, },
            FALSE
        },
        {                       /* Negation of "PanelDisplay" public option */
            ATI_OPTION_CRT_SCREEN,
            "crtscreen",
            OPTV_BOOLEAN,
            {0, },
            FALSE
        },
        {                       /* ON:   Ease exploration of loose ends */
            ATI_OPTION_DEVEL,   /* OFF:  Fit for public consumption */
            "tsi",
            OPTV_BOOLEAN,
            {0, },
            FALSE
        },
        {                       /* ON:   Horizontally blend most modes */
            ATI_OPTION_BLEND,   /* OFF:  Use pixel replication more often */
            "lcdblend",
            OPTV_BOOLEAN,
            {0, },
            FALSE
        },
        {                       /* ON:   Use XF86Config porch timings */
            ATI_OPTION_LCDSYNC, /* OFF:  Use porches from mode on entry */
            "lcdsync",
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

    (void)memcpy(PublicOption, ATIPublicOptions, ATIPublicOptionSize);

#if GET_ABI_MAJOR(ABI_VIDEODRV_VERSION) < 25
# define boolean bool
#endif
    
#   define ProbeSparse   PublicOption[ATI_OPTION_PROBE_SPARSE].value.boolean
#   define Accel         PublicOption[ATI_OPTION_ACCEL].value.boolean
#   define BIOSDisplay   PrivateOption[ATI_OPTION_BIOS_DISPLAY].value.boolean
#   define Blend         PrivateOption[ATI_OPTION_BLEND].value.boolean
#   define CRTDisplay    PublicOption[ATI_OPTION_CRT_DISPLAY].value.boolean
#   define CRTScreen     PrivateOption[ATI_OPTION_CRT_SCREEN].value.boolean
#   define CSync         PublicOption[ATI_OPTION_CSYNC].value.boolean
#   define Devel         PrivateOption[ATI_OPTION_DEVEL].value.boolean
#   define HWCursor      PublicOption[ATI_OPTION_HWCURSOR].value.boolean

#ifdef XF86DRI_DEVEL

#   define IsPCI       PublicOption[ATI_OPTION_IS_PCI].value.boolean
#   define DMAMode     PublicOption[ATI_OPTION_DMA_MODE].value.str
#   define AGPMode     PublicOption[ATI_OPTION_AGP_MODE].value.num
#   define AGPSize     PublicOption[ATI_OPTION_AGP_SIZE].value.num
#   define LocalTex    PublicOption[ATI_OPTION_LOCAL_TEXTURES].value.boolean
#   define BufferSize  PublicOption[ATI_OPTION_BUFFER_SIZE].value.num

#endif /* XF86DRI_DEVEL */

#ifdef TV_OUT

#   define TvOut        PublicOption[ATI_OPTION_TV_OUT].value.boolean
#   define TvStd        PublicOption[ATI_OPTION_TV_STD].value.str

#endif /* TV_OUT */

#   define CacheMMIO     PublicOption[ATI_OPTION_MMIO_CACHE].value.boolean
#   define TestCacheMMIO PublicOption[ATI_OPTION_TEST_MMIO_CACHE].value.boolean
#   define PanelDisplay  PublicOption[ATI_OPTION_PANEL_DISPLAY].value.boolean
#   define ShadowFB      PublicOption[ATI_OPTION_SHADOW_FB].value.boolean
#   define SWCursor      PublicOption[ATI_OPTION_SWCURSOR].value.boolean
#   define AccelMethod   PublicOption[ATI_OPTION_ACCELMETHOD].value.str
#   define RenderAccel   PublicOption[ATI_OPTION_RENDER_ACCEL].value.boolean
#   define LCDSync       PrivateOption[ATI_OPTION_LCDSYNC].value.boolean

#   define ReferenceClock \
        PublicOption[ATI_OPTION_REFERENCE_CLOCK].value.freq.freq

    /* Pick up XF86Config options */
    xf86CollectOptions(pScreenInfo, NULL);

    /* Set non-zero defaults */
    Accel = CacheMMIO = HWCursor = TRUE;

    ReferenceClock = ((double)157500000.0) / ((double)11.0);
#ifdef __sparc__
    if (pATI->Chip == ATI_CHIP_264XL || pATI->Chip == ATI_CHIP_MOBILITY)
           ReferenceClock = 29500000.0;
#endif

    ShadowFB = TRUE;

    Blend = PanelDisplay = TRUE;

#ifdef USE_EXA
    RenderAccel = FALSE;
#endif

#ifdef XF86DRI_DEVEL
    DMAMode = "async";
#endif

#ifdef TV_OUT
    TvStd = "None";  /* No tv standard change requested */
#endif

    xf86ProcessOptions(pScreenInfo->scrnIndex, pScreenInfo->options,
        PublicOption);
    xf86ProcessOptions(pScreenInfo->scrnIndex, pScreenInfo->options,
        PrivateOption);

    /* Move option values into driver private structure */
    pATI->OptionProbeSparse = ProbeSparse;
    pATI->OptionAccel = Accel;
    pATI->OptionBIOSDisplay = BIOSDisplay;
    pATI->OptionBlend = Blend;
    pATI->OptionCRTDisplay = CRTDisplay;
    pATI->OptionCSync = CSync;
    pATI->OptionDevel = Devel;

#ifdef TV_OUT

    if (TvOut && pATI->Chip < ATI_CHIP_264GT) {
       /* Only allow this for 3D Rage (I) or greater chip ID
	* AFAIK, no chips before this supported TV-Out
	* mach64VT has support for TV tuner, but no TV-Out
	*/
	xf86DrvMsg(pScreenInfo->scrnIndex, X_WARNING,
                "TV Out not supported for this chip.\n");
    } else {
	ATITVStandard std;
	pATI->OptionTvOut = TvOut;
	pATI->OptionTvStd = ATI_TV_STD_INVALID;
	for (std = 0; std < ATI_TV_STDS_MAX_VALID; std++) {
	    if (std != ATI_TV_STD_RESERVED1 && std != ATI_TV_STD_RESERVED2) {
		if (strncasecmp(TvStd, ATITVStandardNames[std], ATI_TV_STDS_NAME_MAXLEN)==0) {
		    pATI->OptionTvStd = std;
		    break;
		}
	    }
	}
    }

#endif /* TV_OUT */

    pATI->OptionMMIOCache = CacheMMIO;
    pATI->OptionTestMMIOCache = TestCacheMMIO;
    pATI->OptionShadowFB = ShadowFB;
    pATI->OptionLCDSync = LCDSync;

    /* "CRTScreen" is now "NoPanelDisplay" */
    if ((PanelDisplay != CRTScreen) ||
        PublicOption[ATI_OPTION_PANEL_DISPLAY].found)
        pATI->OptionPanelDisplay = PanelDisplay;
    else
        pATI->OptionPanelDisplay = !CRTScreen;

#ifdef XF86DRI_DEVEL

    pATI->OptionIsPCI = IsPCI;
    pATI->OptionAGPMode = AGPMode;
    pATI->OptionAGPSize = AGPSize;
    pATI->OptionLocalTextures = LocalTex;
    pATI->OptionBufferSize = BufferSize;

    if (strcasecmp(DMAMode, "async")==0)
        pATI->OptionDMAMode = MACH64_MODE_DMA_ASYNC;
    else if (strcasecmp(DMAMode, "sync")==0)
        pATI->OptionDMAMode = MACH64_MODE_DMA_SYNC;
    else if (strcasecmp(DMAMode, "mmio")==0 )
        pATI->OptionDMAMode = MACH64_MODE_MMIO;
    else {
        xf86DrvMsg(pScreenInfo->scrnIndex, X_WARNING,
		   "Unknown dma_mode: '%s'\n", DMAMode);
	xf86DrvMsg(pScreenInfo->scrnIndex, X_WARNING, 
		   "Valid dma_mode options are: 'async','sync','mmio'\n");
        xf86DrvMsg(pScreenInfo->scrnIndex, X_WARNING, 
		   "Defaulting to async DMA mode\n");
	pATI->OptionDMAMode = MACH64_MODE_DMA_ASYNC;
    }

#endif /* XF86DRI_DEVEL */

    /* Validate and set cursor options */
    pATI->Cursor = ATI_CURSOR_SOFTWARE;
    if (SWCursor || !HWCursor)
    {
        if (HWCursor && PublicOption[ATI_OPTION_HWCURSOR].found)
            xf86DrvMsg(pScreenInfo->scrnIndex, X_WARNING,
                "Option \"sw_cursor\" overrides Option \"hw_cursor\".\n");
    }
    else if (pATI->Chip < ATI_CHIP_264CT)
    {
        if (HWCursor && PublicOption[ATI_OPTION_HWCURSOR].found)
            xf86DrvMsg(pScreenInfo->scrnIndex, X_WARNING,
                "Option \"hw_cursor\" not supported in this configuration.\n");
    }
    else
    {
        pATI->Cursor = ATI_CURSOR_HARDWARE;
    }

    pATI->refclk = (int)ReferenceClock;

    pATI->useEXA = FALSE;
    if (pATI->OptionAccel)
    {
        MessageType from = X_DEFAULT;
#if defined(USE_EXA)
#if defined(USE_XAA)
        if (AccelMethod != NULL)
        {
            from = X_CONFIG;
            if (xf86NameCmp(AccelMethod, "EXA") == 0)
                pATI->useEXA = TRUE;
        }
#else /* USE_XAA */
        pATI->useEXA = TRUE;
#endif /* !USE_XAA */
#endif /* USE_EXA */
        xf86DrvMsg(pScreenInfo->scrnIndex, from,
            "Using %s acceleration architecture\n",
            pATI->useEXA ? "EXA" : "XAA");

#if defined(USE_EXA)
        if (pATI->useEXA && pATI->Chip >= ATI_CHIP_264GTPRO)
            pATI->RenderAccelEnabled = TRUE;

        if (pATI->useEXA && !RenderAccel)
            pATI->RenderAccelEnabled = FALSE;
#endif
    }

    free(PublicOption);
}
