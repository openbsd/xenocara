/* $XConsortium: mga_driver.c /main/12 1996/10/28 05:13:26 kaleb $ */
/*
 * MGA Millennium (MGA2064W) with Ti3026 RAMDAC driver v.1.1
 *
 * The driver is written without any chip documentation. All extended ports
 * and registers come from tracing the VESA-ROM functions.
 * The BitBlt Engine comes from tracing the windows BitBlt function.
 *
 * Author:	Radoslaw Kapitan, Tarnow, Poland
 *			kapitan@student.uci.agh.edu.pl
 *		original source
 *
 * Now that MATROX has released documentation to the public, enhancing
 * this driver has become much easier. Nevertheless, this work continues
 * to be based on Radoslaw's original source
 *
 * Contributors:
 *		Andrew van der Stock
 *			ajv@greebo.net
 *		additions, corrections, cleanups
 *
 *		Dirk Hohndel
 *			hohndel@XFree86.Org
 *		integrated into XFree86-3.1.2Gg
 *		fixed some problems with PCI probing and mapping
 *
 *		David Dawes
 *			dawes@XFree86.Org
 *		some cleanups, and fixed some problems
 *
 *		Andrew E. Mileski
 *			aem@ott.hookup.net
 *		RAMDAC timing, and BIOS stuff
 *
 *		Leonard N. Zubkoff
 *			lnz@dandelion.com
 *		Support for 8MB boards, RGB Sync-on-Green, and DPMS.
 *		Guy DESBIEF
 *			g.desbief@aix.pacwan.net
 *		RAMDAC MGA1064 timing,
 *		Doug Merritt
 *			doug@netcom.com
 *		Fixed 32bpp hires 8MB horizontal line glitch at middle right
 *		Niels Gram Jeppesen
 *		Added digital screen option for first head
 */
 
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

/* All drivers should typically include these */
#include "xf86.h"
#include "xf86_OSproc.h"
#include "xf86Resources.h"

/* All drivers need this */

#include "compiler.h"

/* Drivers for PCI hardware need this */
#include "xf86PciInfo.h"

/* Drivers that need to access the PCI config space directly need this */
#ifndef XSERVER_LIBPCIACCESS
#include "xf86Pci.h"
#endif

/* All drivers initialising the SW cursor need this */
#include "mipointer.h"

/* All drivers implementing backing store need this */
#include "mibstore.h"

#include "micmap.h"

#include "xf86DDC.h"
#include "xf86RAC.h"
#include "vbe.h"

#include "fb.h"
#include "dixstruct.h"

#include "mga_reg.h"
#include "mga.h"
#include "mga_macros.h"
#include "mga_maven.h"

#ifdef USE_XAA
#include "xaa.h"
#endif

#include "xf86cmap.h"
#include "shadowfb.h"
#include "fbdevhw.h"

#ifdef XF86DRI
#include "dri.h"
#endif

#include <unistd.h>

/*
 * Forward definitions for the functions that make up the driver.
 */

/* Mandatory functions */
static const OptionInfoRec *	MGAAvailableOptions(int chipid, int busid);
static void	MGAIdentify(int flags);
#ifdef XSERVER_LIBPCIACCESS
static Bool MGAPciProbe(DriverPtr drv, int entity_num,
    struct pci_device * dev, intptr_t match_data);
#else
static Bool	MGAProbe(DriverPtr drv, int flags);
#endif
static Bool	MGAPreInit(ScrnInfoPtr pScrn, int flags);
static Bool	MGAScreenInit(int Index, ScreenPtr pScreen, int argc,
			      char **argv);
static Bool	MGAEnterVT(int scrnIndex, int flags);
static Bool	MGAEnterVTFBDev(int scrnIndex, int flags);
static void	MGALeaveVT(int scrnIndex, int flags);
static Bool	MGACloseScreen(int scrnIndex, ScreenPtr pScreen);
static Bool	MGASaveScreen(ScreenPtr pScreen, int mode);
static Bool	MGASaveScreenCrtc2(ScreenPtr pScreen, int mode);

/* This shouldn't be needed since RAC will disable all I/O for MGA cards. */
#ifdef DISABLE_VGA_IO
static void     VgaIOSave(int i, void *arg);
static void     VgaIORestore(int i, void *arg);
#endif

/* Optional functions */
static void	MGAFreeScreen(int scrnIndex, int flags);
static ModeStatus MGAValidMode(int scrnIndex, DisplayModePtr mode,
			       Bool verbose, int flags);

#if ((__GNUC__ == 3) && (__GNUC_MINOR__ >= 4)) || (__GNUC__ >= 4)
#define __must_check  __attribute__((warn_unused_result))
#else
#define __must_check  /* */
#endif

/* Internally used functions */
static Bool __must_check MGAMapMem(ScrnInfoPtr pScrn);
static Bool	MGAUnmapMem(ScrnInfoPtr pScrn);
static void	MGASave(ScrnInfoPtr pScrn);
static void	MGARestore(ScrnInfoPtr pScrn);
static Bool	MGAModeInit(ScrnInfoPtr pScrn, DisplayModePtr mode);
static void 	MGABlockHandler(int, pointer, pointer, pointer);
static void	MGAG100BlackMagic(ScrnInfoPtr pScrn);

static int MGAEntityIndex = -1;

#include "mga_merge.h"

static const struct mga_device_attributes attribs[] = {
    /* 2064 */
    [0] = { 1, 0, 0, 1, 0, 0, 0, 0, old_BARs,  
	    (BLK_OPAQUE_EXPANSION | FASTBLT_BUG | USE_LINEAR_EXPANSION),
	{
	    { 0, 0 },          /* System VCO frequencies */
	    { 50000, 220000 }, /* Pixel VCO frequencies */
	    { 0, 0 },          /* Video VCO frequencies */
	    50000,             /* Memory clock */
	    14318,             /* PLL reference frequency */
	    0,                 /* Supports fast bitblt? */
	    MGA_HOST_PCI       /* Host interface */
	},

	8192, 0x1000,          /* Memory probe size & offset values */
    },

    /* 1064 */
    [1] = { 0, 1, 0, 0, 1, 0, 0, 0, probe_BARs,
            (USE_LINEAR_EXPANSION),
	{
	    /* There used to be code in MGARamdacInit (mga_dacG.c) that would
	     * set this to 170000 if the chip revision was less than 3.  Is
	     * that needed here?
	     */
	    { 50000, 230000 }, /* System VCO frequencies */
	    { 50000, 230000 }, /* Pixel VCO frequencies */
	    { 0, 0 },          /* Video VCO frequencies */
	    50000,             /* Memory clock */
	    14318,             /* PLL reference frequency */
	    0,                 /* Supports fast bitblt? */
	    MGA_HOST_PCI       /* Host interface */
	},

	8192, 0x1000,          /* Memory probe size & offset values */
    },

    /* 2164 */
    [2] = { 1, 0, 0, 1, 0, 0, 0, 0, new_BARs,
            (BLK_OPAQUE_EXPANSION | TRANSC_SOLID_FILL | USE_RECTS_FOR_LINES
	     | USE_LINEAR_EXPANSION),
	{
	    { 0, 0 },          /* System VCO frequencies */
	    { 50000, 220000 }, /* Pixel VCO frequencies */
	    { 0, 0 },          /* Video VCO frequencies */
	    50000,             /* Memory clock */
	    14318,             /* PLL reference frequency */
	    0,                 /* Supports fast bitblt? */
	    MGA_HOST_PCI       /* Host interface */
	},

	8192, 0x1000,          /* Memory probe size & offset values */
    },

    /* 2164 AGP */
    [3] = { 1, 0, 0, 1, 0, 0, 0, 0, new_BARs,
            (BLK_OPAQUE_EXPANSION | TRANSC_SOLID_FILL | USE_RECTS_FOR_LINES
	     | USE_LINEAR_EXPANSION),
	{
	    { 0, 0 },          /* System VCO frequencies */
	    { 50000, 220000 }, /* Pixel VCO frequencies */
	    { 0, 0 },          /* Video VCO frequencies */
	    50000,             /* Memory clock */
	    14318,             /* PLL reference frequency */
	    0,                 /* Supports fast bitblt? */
	    MGA_HOST_AGP_1x    /* Host interface */
	},

	8192, 0x1000,          /* Memory probe size & offset values */
    },

    /* G100 PCI */
    [4] = { 0, 1, 0, 0, 1, 0, 0, 0, new_BARs,
            (MGA_NO_PLANEMASK | USE_LINEAR_EXPANSION),
	{
	    { 50000, 230000 }, /* System VCO frequencies */
	    { 50000, 230000 }, /* Pixel VCO frequencies */
	    { 0, 0 },          /* Video VCO frequencies */
	    50000,             /* Memory clock */
	    27050,             /* PLL reference frequency */
	    0,                 /* Supports fast bitblt? */
	    MGA_HOST_PCI       /* Host interface */
	},

	8192, 0x1000,          /* Memory probe size & offset values */
    },

    /* G100 AGP */
    [5] = { 0, 1, 0, 0, 1, 0, 0, 0, new_BARs,
            (MGA_NO_PLANEMASK | USE_LINEAR_EXPANSION),
	{
	    { 50000, 230000 }, /* System VCO frequencies */
	    { 50000, 230000 }, /* Pixel VCO frequencies */
	    { 0, 0 },          /* Video VCO frequencies */
	    50000,             /* Memory clock */
	    27050,             /* PLL reference frequency */
	    0,                 /* Supports fast bitblt? */
	    MGA_HOST_AGP_1x    /* Host interface */
	},

	8192, 0x1000,          /* Memory probe size & offset values */
    },

    /* G200 PCI */
    [6] = { 0, 1, 0, 0, 1, 1, 1, 1, new_BARs,
            (TRANSC_SOLID_FILL | TWO_PASS_COLOR_EXPAND | USE_LINEAR_EXPANSION),
	{
	    { 50000, 230000 }, /* System VCO frequencies */
	    { 50000, 230000 }, /* Pixel VCO frequencies */
	    { 0, 0 },          /* Video VCO frequencies */
	    50000,             /* Memory clock */
	    27050,             /* PLL reference frequency */
	    0,                 /* Supports fast bitblt? */
	    MGA_HOST_PCI       /* Host interface */
	},

	8192, 0x1000,          /* Memory probe size & offset values */
    },

    /* G200 AGP */
    [7] = { 0, 1, 0, 0, 1, 1, 1, 1, new_BARs,
            (TRANSC_SOLID_FILL | TWO_PASS_COLOR_EXPAND | USE_LINEAR_EXPANSION),
	{
	    { 50000, 230000 }, /* System VCO frequencies */
	    { 50000, 230000 }, /* Pixel VCO frequencies */
	    { 0, 0 },          /* Video VCO frequencies */
	    50000,             /* Memory clock */
	    27050,             /* PLL reference frequency */
	    0,                 /* Supports fast bitblt? */
	    MGA_HOST_AGP_2x    /* Host interface */
	},

	8192, 0x1000,          /* Memory probe size & offset values */
    },

    /* G400 / G450 */
    [8] = { 0, 1, 1, 0, 1, 1, 2, 1, new_BARs,
            (TRANSC_SOLID_FILL | TWO_PASS_COLOR_EXPAND | USE_LINEAR_EXPANSION),
	{
	    { 50000, 252000 }, /* System VCO frequencies */
	    { 50000, 252000 }, /* Pixel VCO frequencies */
	    { 0, 0 },          /* Video VCO frequencies */
	    200000,            /* Memory clock */
	    27050,             /* PLL reference frequency */
	    0,                 /* Supports fast bitblt? */
	    MGA_HOST_AGP_4x    /* Host interface */
	},

	32768, 0x1000,         /* Memory probe size & offset values */
    },

    /* G550 */
    [9] = { 0, 1, 1, 0, 1, 1, 2, 1, new_BARs,
            (TRANSC_SOLID_FILL | TWO_PASS_COLOR_EXPAND | USE_LINEAR_EXPANSION),
	{
	    { 256000, 600000 }, /* System VCO frequencies */
	    { 256000, 600000 }, /* Pixel VCO frequencies */
	    { 256000, 600000 }, /* Video VCO frequencies */
	    284000,            /* Memory clock */
	    27050,             /* PLL reference frequency */
	    0,                 /* Supports fast bitblt? */
	    MGA_HOST_AGP_4x    /* Host interface */
	},

	32768, 0x1000,         /* Memory probe size & offset values */
    },

    /* G200SE A PCI */
    [10] = { 0, 1, 0, 0, 1, 0, 0, 1, new_BARs,
            (TRANSC_SOLID_FILL | TWO_PASS_COLOR_EXPAND | USE_LINEAR_EXPANSION),
	{
	    { 50000, 230000 }, /* System VCO frequencies */
	    { 50000, 230000 }, /* Pixel VCO frequencies */
	    { 0, 0 },          /* Video VCO frequencies */
	    50000,            /* Memory clock */
	    27050,             /* PLL reference frequency */
	    0,                 /* Supports fast bitblt? */
	    MGA_HOST_PCI       /* Host interface */
	},

	4096, 0x800,           /* Memory probe size & offset values */
    },

    /* G200SE B PCI */
    [11] = { 0, 1, 0, 0, 1, 0, 0, 1, new_BARs,
            (TRANSC_SOLID_FILL | TWO_PASS_COLOR_EXPAND | USE_LINEAR_EXPANSION),
	{
	    { 50000, 114000 }, /* System VCO frequencies */
	    { 50000, 114000 }, /* Pixel VCO frequencies */
	    { 0, 0 },          /* Video VCO frequencies */
	    45000,            /* Memory clock */
	    27050,             /* PLL reference frequency */
	    0,                 /* Supports fast bitblt? */
	    MGA_HOST_PCI       /* Host interface */
	},

	4096, 0x800,           /* Memory probe size & offset values */
    },

    /* G200EV */
    [12] = { 0, 1, 0, 0, 1, 0, 0, 0, new_BARs,
            (TRANSC_SOLID_FILL | TWO_PASS_COLOR_EXPAND | USE_LINEAR_EXPANSION),
	{
	    { 50000, 230000 }, /* System VCO frequencies */
	    { 50000, 230000 }, /* Pixel VCO frequencies */
	    { 0, 0 },          /* Video VCO frequencies */
	    45000,            /* Memory clock */
	    27050,             /* PLL reference frequency */
	    0,                 /* Supports fast bitblt? */
	    MGA_HOST_PCI       /* Host interface */
	},

	8192, 0x4000,          /* Memory probe size & offset values */
    },

    /* G200WB */
    [13] = { 0, 1, 0, 0, 1, 0, 0, 0, new_BARs,
            (TRANSC_SOLID_FILL | TWO_PASS_COLOR_EXPAND | USE_LINEAR_EXPANSION),
	{
	    { 50000, 230000 }, /* System VCO frequencies */
	    { 50000, 203400 }, /* Pixel VCO frequencies */
	    { 0, 0 },          /* Video VCO frequencies */
	    45000,            /* Memory clock */
	    27050,             /* PLL reference frequency */
	    0,                 /* Supports fast bitblt? */
	    MGA_HOST_PCI       /* Host interface */
	},

	8192, 0x4000,          /* Memory probe size & offset values */
    },

};

#ifdef XSERVER_LIBPCIACCESS
#define MGA_DEVICE_MATCH(d, i) \
    { 0x102B, (d), PCI_MATCH_ANY, PCI_MATCH_ANY, 0, 0, (i) }
#define MGA_SUBDEVICE_MATCH(d, s, i) \
    { 0x102B, (d), 0x102B, (s), 0, 0, (i) }

static const struct pci_id_match mga_device_match[] = {
    MGA_DEVICE_MATCH(PCI_CHIP_MGA2064,     0),
    MGA_DEVICE_MATCH(PCI_CHIP_MGA1064,     1),
    MGA_DEVICE_MATCH(PCI_CHIP_MGA2164,     2),
    MGA_DEVICE_MATCH(PCI_CHIP_MGA2164_AGP, 3),
    MGA_DEVICE_MATCH(PCI_CHIP_MGAG100,     4),
    MGA_DEVICE_MATCH(PCI_CHIP_MGAG100_PCI, 5),
    MGA_DEVICE_MATCH(PCI_CHIP_MGAG200,     6),
    MGA_DEVICE_MATCH(PCI_CHIP_MGAG200_PCI, 7),
    MGA_DEVICE_MATCH(PCI_CHIP_MGAG400,     8),
    MGA_DEVICE_MATCH(PCI_CHIP_MGAG550,     9),

    MGA_DEVICE_MATCH(PCI_CHIP_MGAG200_SE_A_PCI, 10),
    MGA_DEVICE_MATCH(PCI_CHIP_MGAG200_SE_B_PCI, 11),

    MGA_DEVICE_MATCH(PCI_CHIP_MGAG200_EV_PCI, 12),

    MGA_DEVICE_MATCH( PCI_CHIP_MGAG200_WINBOND_PCI, 13 ),

    { 0, 0, 0 },
};
#endif

/* Supported chipsets */
static SymTabRec MGAChipsets[] = {
    { PCI_CHIP_MGA2064,		"mga2064w" },
    { PCI_CHIP_MGA1064,		"mga1064sg" },
    { PCI_CHIP_MGA2164,		"mga2164w" },
    { PCI_CHIP_MGA2164_AGP,	"mga2164w AGP" },
    { PCI_CHIP_MGAG100,		"mgag100" },
    { PCI_CHIP_MGAG100_PCI,	"mgag100 PCI" },
    { PCI_CHIP_MGAG200,		"mgag200" },
    { PCI_CHIP_MGAG200_PCI,	"mgag200 PCI" },
    { PCI_CHIP_MGAG200_SE_A_PCI,	"mgag200 SE A PCI" },
    { PCI_CHIP_MGAG200_SE_B_PCI,	"mgag200 SE B PCI" },
    { PCI_CHIP_MGAG200_EV_PCI,	"mgag200 Maxim" },
    { PCI_CHIP_MGAG200_WINBOND_PCI,	"mgag200 Winbond" },
    { PCI_CHIP_MGAG400,		"mgag400" },
    { PCI_CHIP_MGAG550,		"mgag550" },
    {-1,			NULL }
};

static PciChipsets MGAPciChipsets[] = {
    { PCI_CHIP_MGA2064,	    PCI_CHIP_MGA2064,	(resRange*)RES_SHARED_VGA },
    { PCI_CHIP_MGA1064,	    PCI_CHIP_MGA1064,	(resRange*)RES_SHARED_VGA },
    { PCI_CHIP_MGA2164,	    PCI_CHIP_MGA2164,	(resRange*)RES_SHARED_VGA },
    { PCI_CHIP_MGA2164_AGP, PCI_CHIP_MGA2164_AGP,(resRange*)RES_SHARED_VGA },
    { PCI_CHIP_MGAG100,	    PCI_CHIP_MGAG100,	(resRange*)RES_SHARED_VGA },
    { PCI_CHIP_MGAG100_PCI, PCI_CHIP_MGAG100_PCI,(resRange*)RES_SHARED_VGA },
    { PCI_CHIP_MGAG200,	    PCI_CHIP_MGAG200,	(resRange*)RES_SHARED_VGA },
    { PCI_CHIP_MGAG200_PCI, PCI_CHIP_MGAG200_PCI,(resRange*)RES_SHARED_VGA },
    { PCI_CHIP_MGAG200_SE_B_PCI, PCI_CHIP_MGAG200_SE_B_PCI,
	(resRange*)RES_SHARED_VGA },
    { PCI_CHIP_MGAG200_SE_A_PCI, PCI_CHIP_MGAG200_SE_A_PCI,
	(resRange*)RES_SHARED_VGA },
    { PCI_CHIP_MGAG200_EV_PCI, PCI_CHIP_MGAG200_EV_PCI,
	(resRange*)RES_SHARED_VGA },
    { PCI_CHIP_MGAG200_WINBOND_PCI, PCI_CHIP_MGAG200_WINBOND_PCI,
	(resRange*)RES_SHARED_VGA },
    { PCI_CHIP_MGAG400,	    PCI_CHIP_MGAG400,	(resRange*)RES_SHARED_VGA },
    { PCI_CHIP_MGAG550,	    PCI_CHIP_MGAG550,	(resRange*)RES_SHARED_VGA },
    { -1,			-1,		(resRange*)RES_UNDEFINED }
};

/*
 * This contains the functions needed by the server after loading the
 * driver module.  It must be supplied, and gets added the driver list by
 * the Module Setup funtion in the dynamic case.  In the static case a
 * reference to this is compiled in, and this requires that the name of
 * this DriverRec be an upper-case version of the driver name.
 */

_X_EXPORT DriverRec MGA_C_NAME = {
    MGA_VERSION,
    MGA_DRIVER_NAME,
    MGAIdentify,
#ifdef XSERVER_LIBPCIACCESS
    NULL,
#else
    MGAProbe,
#endif
    MGAAvailableOptions,
    NULL,
    0,
    NULL,
     
#ifdef XSERVER_LIBPCIACCESS
    mga_device_match,
    MGAPciProbe
#endif
};


static const OptionInfoRec MGAOptions[] = {
    { OPTION_SW_CURSOR,		"SWcursor",	OPTV_BOOLEAN,	{0}, FALSE },
    { OPTION_HW_CURSOR,		"HWcursor",	OPTV_BOOLEAN,	{0}, FALSE },
    { OPTION_PCI_RETRY,		"PciRetry",	OPTV_BOOLEAN,	{0}, FALSE },
    { OPTION_SYNC_ON_GREEN,	"SyncOnGreen",	OPTV_BOOLEAN,	{0}, FALSE },
    { OPTION_NOACCEL,		"NoAccel",	OPTV_BOOLEAN,	{0}, FALSE },
    { OPTION_SHOWCACHE,		"ShowCache",	OPTV_BOOLEAN,	{0}, FALSE },
    { OPTION_OVERLAY,		"Overlay",	OPTV_ANYSTR,	{0}, FALSE },
    { OPTION_MGA_SDRAM,		"MGASDRAM",	OPTV_BOOLEAN,	{0}, FALSE },
    { OPTION_SHADOW_FB,		"ShadowFB",	OPTV_BOOLEAN,	{0}, FALSE },
    { OPTION_FBDEV,		"UseFBDev",	OPTV_BOOLEAN,	{0}, FALSE },
    { OPTION_COLOR_KEY,		"ColorKey",	OPTV_INTEGER,	{0}, FALSE },
    { OPTION_SET_MCLK,		"SetMclk",	OPTV_FREQ,	{0}, FALSE },
    { OPTION_OVERCLOCK_MEM,	"OverclockMem",	OPTV_BOOLEAN,	{0}, FALSE },
    { OPTION_VIDEO_KEY,		"VideoKey",	OPTV_INTEGER,	{0}, FALSE },
    { OPTION_ROTATE,		"Rotate",	OPTV_ANYSTR,	{0}, FALSE },
    { OPTION_TEXTURED_VIDEO,	"TexturedVideo",OPTV_BOOLEAN,	{0}, FALSE },
    { OPTION_CRTC2HALF,		"Crtc2Half",	OPTV_BOOLEAN,	{0}, FALSE },
    { OPTION_CRTC2RAM,		"Crtc2Ram",	OPTV_INTEGER,	{0}, FALSE },
    { OPTION_INT10,		"Int10",	OPTV_BOOLEAN,	{0}, FALSE },
    { OPTION_AGP_MODE,		"AGPMode",	OPTV_INTEGER,	{0}, FALSE },
    { OPTION_AGP_SIZE,		"AGPSize",      OPTV_INTEGER,   {0}, FALSE },
    { OPTION_DIGITAL1,		"DigitalScreen1",OPTV_BOOLEAN,	{0}, FALSE },
    { OPTION_DIGITAL2,		"DigitalScreen2",OPTV_BOOLEAN,	{0}, FALSE },
    { OPTION_TV,		"TV",		OPTV_BOOLEAN,	{0}, FALSE },
    { OPTION_TVSTANDARD,	"TVStandard",	OPTV_ANYSTR,	{0}, FALSE },
    { OPTION_CABLETYPE,		"CableType",	OPTV_ANYSTR,	{0}, FALSE },
    { OPTION_NOHAL,		"NoHal",	OPTV_BOOLEAN,	{0}, FALSE },
    { OPTION_SWAPPED_HEAD,	"SwappedHead",	OPTV_BOOLEAN,	{0}, FALSE },
    { OPTION_DRI,		"DRI",		OPTV_BOOLEAN,	{0}, FALSE },
    { OPTION_MERGEDFB,		"MergedFB",	OPTV_BOOLEAN,	{0}, FALSE },
    { OPTION_HSYNC2,	"Monitor2HSync",	OPTV_ANYSTR,	{0}, FALSE },
    { OPTION_VREFRESH2,	"Monitor2VRefresh",	OPTV_ANYSTR,	{0}, FALSE },
    { OPTION_MONITOR2POS,   "Monitor2Position",	OPTV_ANYSTR,	{0}, FALSE },
    { OPTION_METAMODES,   "MetaModes",  	OPTV_ANYSTR,	{0}, FALSE },
    { OPTION_OLDDMA,		"OldDmaInit",	OPTV_BOOLEAN,	{0}, FALSE },
    { OPTION_PCIDMA,		"ForcePciDma",	OPTV_BOOLEAN,	{0}, FALSE },
    { OPTION_ACCELMETHOD,	"AccelMethod",	OPTV_ANYSTR,	{0}, FALSE },
    { OPTION_KVM,		"KVM",		OPTV_BOOLEAN,	{0}, FALSE },
    { -1,			NULL,		OPTV_NONE,	{0}, FALSE }
};


/*
 * List of symbols from other modules that this module references.  This
 * list is used to tell the loader that it is OK for symbols here to be
 * unresolved providing that it hasn't been told that they haven't been
 * told that they are essential via a call to xf86LoaderReqSymbols() or
 * xf86LoaderReqSymLists().  The purpose is this is to avoid warnings about
 * unresolved symbols that are not required.
 */

static const char *vgahwSymbols[] = {
    "vgaHWFreeHWRec",
    "vgaHWGetHWRec",
    "vgaHWGetIOBase",
    "vgaHWGetIndex",
    "vgaHWInit",
    "vgaHWLock",
    "vgaHWMapMem",
    "vgaHWProtect",
    "vgaHWRestore",
    "vgaHWSave",
    "vgaHWSaveScreen",
    "vgaHWSetMmioFuncs",
    "vgaHWUnlock",
    "vgaHWUnmapMem",
    "vgaHWddc1SetSpeedWeak",
    NULL
};

static const char *fbSymbols[] = {
    "fbPictureInit",
    "fbScreenInit",
    NULL
};

#ifdef USE_EXA
static const char *exaSymbols[] = {
    "exaDriverInit",
    "exaDriverFini",
    "exaGetPixmapOffset",
    "exaGetVersion",
    NULL
};
#endif

#ifdef USE_XAA
static const char *xaaSymbols[] = {
    "XAACachePlanarMonoStipple",
    "XAACreateInfoRec",
    "XAADestroyInfoRec",
    "XAAGetFallbackOps",
    "XAAInit",
    "XAAMoveDWORDS",
    "XAA_888_plus_PICT_a8_to_8888",
    NULL
};
#endif

static const char *ramdacSymbols[] = {
    "xf86CreateCursorInfoRec",
    "xf86DestroyCursorInfoRec",
    "xf86InitCursor",
    NULL
};

#ifdef XF86DRI
static const char *drmSymbols[] = {
    "drmAddBufs",
    "drmAddMap",
    "drmAgpAcquire",
    "drmAgpAlloc",
    "drmAgpBind",
    "drmAgpDeviceId",
    "drmAgpEnable",
    "drmAgpFree",
    "drmAgpGetMode",
    "drmAgpRelease",
    "drmAgpUnbind",
    "drmAgpVendorId",
    "drmCommandNone",
    "drmCommandWrite",
    "drmCtlInstHandler",
    "drmCtlUninstHandler",
    "drmFreeVersion",
    "drmGetInterruptFromBusID",
    "drmGetLibVersion",
    "drmGetVersion",
    "drmMap",
    "drmMapBufs",
    "drmUnmap",
    "drmUnmapBufs",
    NULL
};

static const char *driSymbols[] = {
    "DRICloseScreen",
    "DRICreateInfoRec",
    "DRIDestroyInfoRec",
    "DRIFinishScreenInit",
    "DRIGetDeviceInfo",
    "DRILock",
    "DRIQueryVersion",
    "DRIScreenInit",
    "DRIUnlock",
    "GlxSetVisualConfigs",
    "DRICreatePCIBusID",
    NULL
};
#endif

#define MGAuseI2C 1

static const char *ddcSymbols[] = {
    "xf86DoEDID_DDC1",
#if MGAuseI2C
    "xf86DoEDID_DDC2",
#endif
    "xf86PrintEDID",
    "xf86SetDDCproperties",
    NULL
};

static const char *i2cSymbols[] = {
    "xf86CreateI2CBusRec",
    "xf86I2CBusInit",
    NULL
};

static const char *shadowSymbols[] = {
    "ShadowFBInit",
    NULL
};

#ifdef XFree86LOADER
static const char *vbeSymbols[] = {
    "VBEInit",
    "vbeDoEDID",
    "vbeFree",
    NULL
};
#endif

static const char *int10Symbols[] = {
    "xf86FreeInt10",
    "xf86InitInt10",
    NULL
};

static const char *fbdevHWSymbols[] = {
    "fbdevHWAdjustFrameWeak",
    "fbdevHWEnterVT",
    "fbdevHWGetVidmem",
    "fbdevHWInit",
    "fbdevHWLeaveVTWeak",
    "fbdevHWLoadPaletteWeak",
    "fbdevHWMapMMIO",
    "fbdevHWMapVidmem",
    "fbdevHWModeInit",
    "fbdevHWRestore",
    "fbdevHWSave",
    "fbdevHWSwitchModeWeak",
    "fbdevHWUnmapMMIO",
    "fbdevHWUnmapVidmem",
    "fbdevHWUseBuildinMode",
    "fbdevHWValidModeWeak",
    NULL
};

#ifdef USEMGAHAL
static const char *halSymbols[] = {
  "MGACloseLibrary",
  "MGAGetBOARDHANDLESize",
  "MGAGetHardwareInfo",
  "MGAOpenLibrary",
  "MGARestoreVgaState",
  "MGASaveVgaState",
  "MGASetMode",
  "MGASetVgaMode",
  "MGAValidateMode",
  "MGAValidateVideoParameters",
  "HALSetDisplayStart",
  NULL
};
#endif
#ifdef XFree86LOADER

static MODULESETUPPROTO(mgaSetup);

static XF86ModuleVersionInfo mgaVersRec =
{
	MGA_DRIVER_NAME,
	MODULEVENDORSTRING,
	MODINFOSTRING1,
	MODINFOSTRING2,
	XORG_VERSION_CURRENT,
    PACKAGE_VERSION_MAJOR, PACKAGE_VERSION_MINOR, PACKAGE_VERSION_PATCHLEVEL,
	ABI_CLASS_VIDEODRV,			/* This is a video driver */
	ABI_VIDEODRV_VERSION,
	MOD_CLASS_VIDEODRV,
	{0,0,0,0}
};

_X_EXPORT XF86ModuleData MGA_MODULE_DATA = { &mgaVersRec, mgaSetup, NULL };

static pointer
mgaSetup(pointer module, pointer opts, int *errmaj, int *errmin)
{
    static Bool setupDone = FALSE;

    /* This module should be loaded only once, but check to be sure. */

    if (!setupDone) {
	setupDone = TRUE;
	xf86AddDriver(&MGA_C_NAME, module, 1);

	/*
	 * Modules that this driver always requires may be loaded here
	 * by calling LoadSubModule().
	 */

	/*
	 * Tell the loader about symbols from other modules that this module
	 * might refer to.
	 */
	LoaderRefSymLists(vgahwSymbols,
#ifdef USE_XAA
                          xaaSymbols,
#endif
#ifdef USE_EXA
                          exaSymbols,
#endif
			  ramdacSymbols, ddcSymbols, i2cSymbols,
			  shadowSymbols, fbdevHWSymbols, vbeSymbols,
			  fbSymbols, int10Symbols,
#ifdef XF86DRI
			  drmSymbols, driSymbols,
#endif
#ifdef USEMGAHAL
			  halSymbols,
#endif
			  NULL);

	/*
	 * The return value must be non-NULL on success even though there
	 * is no TearDownProc.
	 */
	return (pointer)1;
    } else {
	if (errmaj) *errmaj = LDR_ONCEONLY;
	return NULL;
    }
}


#endif /* XFree86LOADER */

/*
 * ramdac info structure initialization
 */
static MGARamdacRec DacInit = {
	FALSE, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, NULL,
	90000, /* maxPixelClock */
	0, X_DEFAULT, X_DEFAULT, FALSE
};

Bool
MGAGetRec(ScrnInfoPtr pScrn)
{
    /*
     * Allocate an MGARec, and hook it into pScrn->driverPrivate.
     * pScrn->driverPrivate is initialised to NULL, so we can check if
     * the allocation has already been done.
     */
    if (pScrn->driverPrivate != NULL)
	return TRUE;

    pScrn->driverPrivate = xnfcalloc(sizeof(MGARec), 1);
    /* Initialise it */

    MGAPTR(pScrn)->Dac = DacInit;
    return TRUE;
}

void
MGAFreeRec(ScrnInfoPtr pScrn)
{
    if (pScrn->driverPrivate == NULL)
	return;
    xfree(pScrn->driverPrivate);
    pScrn->driverPrivate = NULL;
}

static const OptionInfoRec *
MGAAvailableOptions(int chipid, int busid)
{
    return MGAOptions;
}

/* Mandatory */
static void
MGAIdentify(int flags)
{
    xf86PrintChipsets(MGA_NAME, "driver for Matrox chipsets", MGAChipsets);
}


#ifdef XSERVER_LIBPCIACCESS
Bool
MGAPciProbe(DriverPtr drv, int entity_num, struct pci_device * dev,
	    intptr_t match_data)
{
    ScrnInfoPtr pScrn = NULL;
    EntityInfoPtr pEnt;
    MGAPtr pMga;
#ifdef DISABLE_VGA_IO
    MgaSavePtr smga;


    smga = xnfalloc(sizeof(MgaSave));
    smga->pvp = dev;
#endif

    /* Allocate a ScrnInfoRec and claim the slot */
    pScrn = xf86ConfigPciEntity(pScrn, 0, entity_num, MGAPciChipsets,
				NULL,
#ifndef DISABLE_VGA_IO
				NULL, NULL, NULL, NULL
#else
				VgaIOSave, VgaIOSave, VgaIORestore, smga
#endif
				);
    if (pScrn != NULL) {
	/* Fill in what we can of the ScrnInfoRec */
	pScrn->driverVersion	= MGA_VERSION;
	pScrn->driverName	= MGA_DRIVER_NAME;
	pScrn->name		= MGA_NAME;
	pScrn->Probe		= NULL;
	pScrn->PreInit		= MGAPreInit;
	pScrn->ScreenInit	= MGAScreenInit;
	pScrn->SwitchMode	= MGASwitchMode;
	pScrn->AdjustFrame	= MGAAdjustFrame;
	pScrn->EnterVT		= MGAEnterVT;
	pScrn->LeaveVT		= MGALeaveVT;
	pScrn->FreeScreen	= MGAFreeScreen;
	pScrn->ValidMode	= MGAValidMode;


	/* Allocate the MGARec driverPrivate */
	if (!MGAGetRec(pScrn)) {
	    return FALSE;
	}

	pMga = MGAPTR(pScrn);
	pMga->chip_attribs = & attribs[ match_data ];
	pMga->PciInfo = dev;


	/*
	 * For cards that can do dual head per entity, mark the entity
	 * as sharable. 
	 */
	pEnt = xf86GetEntityInfo(entity_num);
	if (pMga->chip_attribs->dual_head_possible) {
	    MGAEntPtr pMgaEnt = NULL;
	    DevUnion *pPriv;

	    xf86SetEntitySharable(entity_num);
	    /* Allocate an entity private if necessary */
	    if (MGAEntityIndex < 0)
	      MGAEntityIndex = xf86AllocateEntityPrivateIndex();
	    pPriv = xf86GetEntityPrivate(pScrn->entityList[0], MGAEntityIndex);
	    if (!pPriv->ptr) {
		pPriv->ptr = xnfcalloc(sizeof(MGAEntRec), 1);
		pMgaEnt = pPriv->ptr;
		pMgaEnt->lastInstance = -1;
	    } else {
		pMgaEnt = pPriv->ptr;
	    }
	    /*
	     * Set the entity instance for this instance of the driver.  For
	     * dual head per card, instance 0 is the "master" instance, driving
	     * the primary head, and instance 1 is the "slave".
	     */
	    pMgaEnt->lastInstance++;
	    xf86SetEntityInstanceForScreen(pScrn, pScrn->entityList[0],
					   pMgaEnt->lastInstance);
	}
    }

    return (pScrn != NULL);
}

#else

/* Mandatory */
static Bool
MGAProbe(DriverPtr drv, int flags)
{
    int i;
    GDevPtr *devSections;
    int *usedChips = NULL;
    int numDevSections;
    int numUsed;
    Bool foundScreen = FALSE;

    /*
     * The aim here is to find all cards that this driver can handle,
     * and for the ones not already claimed by another driver, claim the
     * slot, and allocate a ScrnInfoRec.
     *
     * This should be a minimal probe, and it should under no circumstances
     * change the state of the hardware.  Because a device is found, don't
     * assume that it will be used.  Don't do any initialisations other than
     * the required ScrnInfoRec initialisations.  Don't allocate any new
     * data structures.
     */

    /*
     * Check if there has been a chipset override in the config file.
     * For this we must find out if there is an active device section which
     * is relevant, i.e., which has no driver specified or has THIS driver
     * specified.
     */

    if ((numDevSections = xf86MatchDevice(MGA_DRIVER_NAME,
					  &devSections)) <= 0) {
	/*
	 * There's no matching device section in the config file, so quit
	 * now.
	 */
	return FALSE;
    }

    /*
     * We need to probe the hardware first.  We then need to see how this
     * fits in with what is given in the config file, and allow the config
     * file info to override any contradictions.
     */

    /*
     * All of the cards this driver supports are PCI, so the "probing" just
     * amounts to checking the PCI data that the server has already collected.
     */
    if (xf86GetPciVideoInfo() == NULL) {
	/*
	 * We won't let anything in the config file override finding no
	 * PCI video cards at all.  This seems reasonable now, but we'll see.
	 */
	return FALSE;
    }

    numUsed = xf86MatchPciInstances(MGA_NAME, PCI_VENDOR_MATROX,
			            MGAChipsets, MGAPciChipsets, devSections,
			            numDevSections, drv, &usedChips);
    /* Free it since we don't need that list after this */
    xfree(devSections);
    if (numUsed <= 0)
	return FALSE;

   
    if (flags & PROBE_DETECT)
	foundScreen = TRUE;
    else for (i = 0; i < numUsed; i++) {
	ScrnInfoPtr pScrn = NULL;
	EntityInfoPtr pEnt;
	int attrib_no;
#ifdef DISABLE_VGA_IO
	MgaSavePtr smga;
#endif

	/* Allocate a ScrnInfoRec and claim the slot */
#ifndef DISABLE_VGA_IO
	pScrn = xf86ConfigPciEntity(pScrn, 0,usedChips[i],
				    MGAPciChipsets, NULL, NULL,
				    NULL, NULL, NULL);
#else
	smga = xnfalloc(sizeof(MgaSave));
	smga->pvp = xf86GetPciInfoForEntity(usedChips[i]);
	pScrn = xf86ConfigPciEntity(pScrn, 0,usedChips[i],
				    MGAPciChipsets, NULL,VgaIOSave,
				    VgaIOSave, VgaIORestore,smga);
#endif
        if (pScrn != NULL) {
	    MGAPtr pMga;

	    /* Fill in what we can of the ScrnInfoRec */
	    pScrn->driverVersion = MGA_VERSION;
	    pScrn->driverName	= MGA_DRIVER_NAME;
	    pScrn->name		= MGA_NAME;
	    pScrn->Probe	= MGAProbe;
	    pScrn->PreInit	= MGAPreInit;
	    pScrn->ScreenInit	= MGAScreenInit;
	    pScrn->SwitchMode	= MGASwitchMode;
	    pScrn->AdjustFrame	= MGAAdjustFrame;
	    pScrn->EnterVT	= MGAEnterVT;
	    pScrn->LeaveVT	= MGALeaveVT;
	    pScrn->FreeScreen	= MGAFreeScreen;
	    pScrn->ValidMode	= MGAValidMode;

	    foundScreen = TRUE;

	    /* Allocate the MGARec driverPrivate */
	    if (!MGAGetRec(pScrn)) {
		return FALSE;
	    }

	    pMga = MGAPTR(pScrn);

	    /*
	     * For cards that can do dual head per entity, mark the entity
	     * as sharable. 
	     */
	    pEnt = xf86GetEntityInfo(usedChips[i]);

            switch (pEnt->chipset) {
            case PCI_CHIP_MGA2064:
                attrib_no = 0;
                break;

            case PCI_CHIP_MGA1064: 
                attrib_no = 1;
                break;

            case PCI_CHIP_MGA2164:
                attrib_no = 2;
                break;
		
            case PCI_CHIP_MGA2164_AGP:
                attrib_no = 3;
                break;

            case PCI_CHIP_MGAG100:
                attrib_no = 4;
                break;

            case PCI_CHIP_MGAG100_PCI:
                attrib_no = 5;
                break;

            case PCI_CHIP_MGAG200:
                attrib_no = 6;
                break;

            case PCI_CHIP_MGAG200_PCI:
                attrib_no = 7;
                break;

            case PCI_CHIP_MGAG400:
                attrib_no = 8;
                break;

            case PCI_CHIP_MGAG550:
                attrib_no = 9;
                break;

            case PCI_CHIP_MGAG200_SE_A_PCI:
                attrib_no = 10;
                break;

            case PCI_CHIP_MGAG200_SE_B_PCI:
                attrib_no = 11;
                break;

            case PCI_CHIP_MGAG200_EV_PCI:
                attrib_no = 12;
                break;

            case PCI_CHIP_MGAG200_WINBOND_PCI:
                attrib_no = 13;
                break;

	    default:
		return FALSE;
            }

	    pMga->chip_attribs = & attribs[attrib_no];

	    if (pMga->chip_attribs->dual_head_possible) {
		MGAEntPtr pMgaEnt = NULL;
		DevUnion *pPriv;

		xf86SetEntitySharable(usedChips[i]);
		/* Allocate an entity private if necessary */
		if (MGAEntityIndex < 0)
		    MGAEntityIndex = xf86AllocateEntityPrivateIndex();
		pPriv = xf86GetEntityPrivate(pScrn->entityList[0], MGAEntityIndex);
		if (!pPriv->ptr) {
		    pPriv->ptr = xnfcalloc(sizeof(MGAEntRec), 1);
		    pMgaEnt = pPriv->ptr;
		    pMgaEnt->lastInstance = -1;
		} else {
		    pMgaEnt = pPriv->ptr;
		}
		/*
		 * Set the entity instance for this instance of the driver.  For
		 * dual head per card, instance 0 is the "master" instance, driving
		 * the primary head, and instance 1 is the "slave".
		 */
		pMgaEnt->lastInstance++;
		xf86SetEntityInstanceForScreen(pScrn, pScrn->entityList[0],
					       pMgaEnt->lastInstance);
	    }
        }
    }
    xfree(usedChips);

    return foundScreen;
}
#endif


/*
 * MGASoftReset --
 *
 * Resets drawing engine
 */
void
MGASoftReset(ScrnInfoPtr pScrn)
{
	MGAPtr pMga = MGAPTR(pScrn);

	pMga->FbMapSize = 8192 * 1024;
	MGAMapMem(pScrn);

	/* set soft reset bit */
	OUTREG(MGAREG_Reset, 1);
	usleep(200);
	OUTREG(MGAREG_Reset, 0);

	/* reset memory */
	OUTREG(MGAREG_MACCESS, 1<<15);
	usleep(10);

#if 0
	/* This will hang if the PLLs aren't on */

	/* wait until drawing engine is ready */
	while ( MGAISBUSY() )
	    usleep(1000);

	/* flush FIFO */
	i = 32;
	WAITFIFO(i);
	while ( i-- )
	    OUTREG(MGAREG_SHIFT, 0);
#endif

	MGAUnmapMem(pScrn);
}

/*
 * MGACountRAM --
 *
 * Counts amount of installed RAM
 */
static int
MGACountRam(ScrnInfoPtr pScrn)
{
    MGAPtr pMga = MGAPTR(pScrn);
    int ProbeSize = pMga->chip_attribs->probe_size;
    int ProbeSizeOffset = pMga->chip_attribs->probe_offset;
    int SizeFound = 2048;
    CARD32 biosInfo = 0;
    CARD8 seq1;

#if 0
    /* This isn't correct. It looks like this can have arbitrary
	data for the memconfig even when the bios has initialized
	it.  At least, my cards don't advertise the documented
	values (my 8 and 16 Meg G200s have the same values) */
    if (pMga->Primary) { /* can only trust this for primary cards */
#ifdef XSERVER_LIBPCIACCESS
	pci_device_cfg_read_u32(pMga->PciInfo, & biosInfo,
				PCI_OPTION_REG);
#else
	biosInfo = pciReadLong(pMga->PciTag, PCI_OPTION_REG);
#endif
    }
#endif

    switch(pMga->Chipset) {
    case PCI_CHIP_MGA2164:
    case PCI_CHIP_MGA2164_AGP:
	xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
		"Unable to probe memory amount due to hardware bug.  "
		"Assuming 4096 KB\n");
	return 4096;
    case PCI_CHIP_MGAG400:
    case PCI_CHIP_MGAG550:
	if(biosInfo) {
	    switch((biosInfo >> 10) & 0x07) {
	    case 0:
		return (biosInfo & (1 << 14)) ? 32768 : 16384;
	    case 1:
	    case 2:
		return 16384;
	    case 3:
	    case 5:
		return 65536;
	    case 4:
		return 32768;
	    }
	}
	break;
    case PCI_CHIP_MGAG200:
    case PCI_CHIP_MGAG200_PCI:
	if(biosInfo) {
	    switch((biosInfo >> 11) & 0x03) {
	    case 0:
		return 8192;
	    default:
		return 16384;
	    }
	}
	break;
    case PCI_CHIP_MGAG100:
    case PCI_CHIP_MGAG100_PCI:
	if(biosInfo) /* I'm not sure if the docs are correct */
	    return (biosInfo & (1 << 12)) ? 16384 : 8192;
        break;
    default:
        break;
    }

    if (pMga->FbAddress) {
	volatile unsigned char* base;
	unsigned char tmp;
	int i;

	pMga->FbMapSize = ProbeSize * 1024;
	if (!MGAMapMem(pScrn)) {
	    return 0;
	}

	base = pMga->FbBase;

	if (pMga->is_G200SE)
	    pMga->reg_1e24 = INREG(0x1e24); /* stash the model for later */
	if (pMga->reg_1e24 == 0x01) {
	    MGAUnmapMem(pScrn);
	    ProbeSize = 16384;
	    ProbeSizeOffset = 0x10000;
	    pMga->FbMapSize = ProbeSize * 1024;
	    MGAMapMem(pScrn);
	    base = pMga->FbBase;
	}

	if (pMga->is_G200SE) {
	    OUTREG8(MGAREG_SEQ_INDEX, 0x01);
	    seq1 = INREG8(MGAREG_SEQ_DATA);
	    seq1 |= 0x20;
	    MGAWAITVSYNC();
	    MGAWAITBUSY();
	    OUTREG8(MGAREG_SEQ_DATA, seq1);
	    usleep(20000);
	}

	/* turn MGA mode on - enable linear frame buffer (CRTCEXT3) */
	OUTREG8(MGAREG_CRTCEXT_INDEX, 3);
	tmp = INREG8(MGAREG_CRTCEXT_DATA);
	OUTREG8(MGAREG_CRTCEXT_DATA, tmp | 0x80);

	/* apparently the G200 IP don't have a BIOS to read */
	if (pMga->is_G200SE || pMga->is_G200EV || pMga->is_G200WB) {
	    CARD32 MemoryAt0, MemoryAt1, Offset;
	    CARD32 FirstMemoryVal1, FirstMemoryVal2;
	    CARD32 SecondMemoryVal1, SecondMemoryVal2;
	    CARD32 TestMemoryLocA, TestMemoryLocB;
	    CARD32 TestMemoryLoc0, TestMemoryLoc1;
	    CARD32 TestA, TestB;

	    MemoryAt0 = base[0];
	    MemoryAt1 = base[1];
	    base[0] = 0;
	    base[1] = 0;

	    for (Offset = 0x100000; Offset < (ProbeSize * 1024);
		    Offset += ProbeSizeOffset) {
		FirstMemoryVal1 = base[Offset];
		FirstMemoryVal2 = base[Offset+1];
		SecondMemoryVal1 = base[Offset+0x100];
		SecondMemoryVal2 = base[Offset+0x101];

		base[Offset] = 0x55;
		base[Offset+1] = 0xaa;
		base[Offset+0x100] = 0x55;
		base[Offset+0x101] = 0xaa;

		OUTREG(MGAREG_CRTC_INDEX, 0);
		usleep(8);

		TestMemoryLocA = base[Offset];
		TestMemoryLocB = base[Offset+1];
		TestMemoryLoc0 = base[0];
		TestMemoryLoc1 = base[1];

		base[Offset] = FirstMemoryVal1;
		base[Offset+1] = FirstMemoryVal2;
		base[Offset+0x100] = SecondMemoryVal1;
		base[Offset+0x101] = SecondMemoryVal2;

		TestA = ((TestMemoryLocB << 8) + TestMemoryLocA);
		TestB = ((TestMemoryLoc1 << 8) + TestMemoryLoc0);
		if ((TestA != 0xAA55) || (TestB)) {
		    break;
		}
	    }

	    base[0] = MemoryAt0;
	    base[1] = MemoryAt1;

	    SizeFound = (Offset / 1024) - 64;
	} else {
	    /* write, read and compare method
	       split into two loops to make it more reliable on RS/6k -ReneR */
	    for(i = ProbeSize; i > 2048; i -= 2048) {
		base[(i * 1024) - 1] = 0xAA;
	    }
	    OUTREG8(MGAREG_CRTC_INDEX, 0);  /* flush the cache */
	    usleep(4);  /* twart write combination */
	    for(i = ProbeSize; i > 2048; i -= 2048) {
		if(base[(i * 1024) - 1] == 0xAA) {
		    SizeFound = i;
		    break;
		}
	    }
	}

	/* restore CRTCEXT3 state */
	OUTREG8(MGAREG_CRTCEXT_INDEX, 3);
	OUTREG8(MGAREG_CRTCEXT_DATA, tmp);

	if (pMga->is_G200SE) {
	    OUTREG8(MGAREG_SEQ_INDEX, 0x01);
	    seq1 = INREG8(MGAREG_SEQ_DATA);
	    seq1 &= ~0x20;
	    MGAWAITVSYNC();
	    MGAWAITBUSY();
	    OUTREG8(MGAREG_SEQ_DATA, seq1);
	    usleep(20000);
	}
	MGAUnmapMem(pScrn);
    }
   return SizeFound;
}

static xf86MonPtr
MGAdoDDC(ScrnInfoPtr pScrn)
{
  vgaHWPtr hwp;
  MGAPtr pMga;
  xf86MonPtr MonInfo = NULL;

  hwp = VGAHWPTR(pScrn);
  pMga = MGAPTR(pScrn);

  /* Load DDC if we have the code to use it */
  /* This gives us DDC1 */
  if (pMga->ddc1Read || pMga->i2cInit) {
      if (xf86LoadSubModule(pScrn, "ddc")) {
	  xf86LoaderReqSymLists(ddcSymbols, NULL);
	} else {
	  /* ddc module not found, we can do without it */
	  pMga->ddc1Read = NULL;
	  pMga->DDC_Bus1 = NULL;
	  pMga->DDC_Bus2 = NULL;
	  return NULL;
	}
    } else 
      return NULL;

#if MGAuseI2C
    /* - DDC can use I2C bus */
    /* Load I2C if we have the code to use it */
    if (pMga->i2cInit) {
      if ( xf86LoadSubModule(pScrn, "i2c") ) {
	xf86LoaderReqSymLists(i2cSymbols,NULL);
      } else {
	/* i2c module not found, we can do without it */
	pMga->i2cInit = NULL;
	pMga->DDC_Bus1 = NULL;
	pMga->DDC_Bus2 = NULL;
      }
    }
#endif /* MGAuseI2C */

  /* Map the MGA memory and MMIO areas */
  if (!MGAMapMem(pScrn))
    return NULL;

  /* Initialise the MMIO vgahw functions */
  vgaHWSetMmioFuncs(hwp, pMga->IOBase, PORT_OFFSET);
  vgaHWGetIOBase(hwp);

  /* Map the VGA memory when the primary video */
  if (pMga->Primary) {
    hwp->MapSize = 0x10000;
    if (!vgaHWMapMem(pScrn))
      return NULL;
  } else {
    /* XXX Need to write an MGA mode ddc1SetSpeed */
    if (pMga->DDC1SetSpeed == vgaHWddc1SetSpeedWeak()) {
      pMga->DDC1SetSpeed = NULL;
      xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, 2,
		     "DDC1 disabled - chip not in VGA mode\n");
    }
  }

  /* Save the current state */
  MGASave(pScrn);

  /* It is now safe to talk to the card */

#if MGAuseI2C
  /* Initialize I2C buses - used by DDC if available */
  if (pMga->i2cInit) {
    pMga->i2cInit(pScrn);
  }

   /* DDC for second head... */
  if (pMga->SecondCrtc && pMga->DDC_Bus2) {
    MonInfo = xf86DoEDID_DDC2(pScrn->scrnIndex,pMga->DDC_Bus2);
    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "I2C Monitor info: %p\n",
		(void *)MonInfo);
    xf86PrintEDID(MonInfo);
    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "end of I2C Monitor info\n");
    xf86SetDDCproperties(pScrn, MonInfo);
    return MonInfo;
  }

  else {
	/* Its the first head... */ 
	  if (pMga->DDC_Bus1) {
	    MonInfo = xf86DoEDID_DDC2(pScrn->scrnIndex,pMga->DDC_Bus1);
	    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "I2C Monitor info: %p\n", (void *) MonInfo);
	    xf86PrintEDID(MonInfo);
	    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "end of I2C Monitor info\n");
	  }
	  if (!MonInfo)
#endif /* MGAuseI2C */
	  /* Read and output monitor info using DDC1 */
	  if (pMga->ddc1Read && pMga->DDC1SetSpeed) {
	    MonInfo = xf86DoEDID_DDC1(pScrn->scrnIndex,
						 pMga->DDC1SetSpeed,
						 pMga->ddc1Read ) ;
	    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "DDC Monitor info: %p\n", (void *) MonInfo);
	    xf86PrintEDID( MonInfo );
	    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "end of DDC Monitor info\n");
	  }
	  if (!MonInfo){
	    vbeInfoPtr pVbe;
	    if (xf86LoadSubModule(pScrn, "vbe")) {
	      pVbe = VBEInit(NULL,pMga->pEnt->index);
	      MonInfo = vbeDoEDID(pVbe, NULL);
	      vbeFree(pVbe);
	
	      if (MonInfo){
		xf86DrvMsg(pScrn->scrnIndex, X_INFO, "VBE DDC Monitor info: %p\n", (void *) MonInfo);
		xf86PrintEDID( MonInfo );
		xf86DrvMsg(pScrn->scrnIndex, X_INFO, "end of VBE DDC Monitor info\n\n");
	      }
	    }
	  }
#if MGAuseI2C
   }
#endif
  /* Restore previous state and unmap MGA memory and MMIO areas */
  MGARestore(pScrn);
  MGAUnmapMem(pScrn);
  /* Unmap vga memory if we mapped it */
  if (xf86IsPrimaryPci(pMga->PciInfo) && !pMga->FBDev) {
    vgaHWUnmapMem(pScrn);
  }

  xf86SetDDCproperties(pScrn, MonInfo);

  return MonInfo;
}

#ifdef DISABLE_VGA_IO
static void
VgaIOSave(int i, void *arg)
{
    MgaSavePtr sMga = arg;
#ifndef XSERVER_LIBPCIACCESS
    PCITAG tag = pciTag(sMga->pvp->bus,sMga->pvp->device,sMga->pvp->func);
#endif
    uint32_t temp;

#ifdef DEBUG
    ErrorF("mga: VgaIOSave: %d:%d:%d\n", sMga->pvp->bus, sMga->pvp->device,
	   sMga->pvp->func);
#endif
#ifdef XSERVER_LIBPCIACCESS
    pci_device_cfg_read_u32(pMga->PciInfo, & temp, PCI_OPTION_REG);
#else
    temp = pciReadLong(tag, PCI_OPTION_REG);
#endif
    sMga->enable = (temp & 0x100) != 0;
}

static void
VgaIORestore(int i, void *arg)
{
    MgaSavePtr sMga = arg;
#ifndef XSERVER_LIBPCIACCESS
    PCITAG tag = pciTag(sMga->pvp->bus,sMga->pvp->device,sMga->pvp->func);
#endif

#ifdef DEBUG
    ErrorF("mga: VgaIORestore: %d:%d:%d\n", sMga->pvp->bus, sMga->pvp->device,
	   sMga->pvp->func);
#endif
#ifdef XSERVER_LIBPCIACCESS
    pci_device_cfg_write_bits(pMga->PciInfo, 0x00000100, sMga->enable,
			      PCI_OPTION_REG);
#else
    pciSetBitsLong(tag, PCI_OPTION_REG, 0x100, sMga->enable ? 0x100 : 0x000);
#endif
}

static void
VgaIODisable(void *arg)
{
    MGAPtr pMga = arg;

#ifdef DEBUG
    ErrorF("mga: VgaIODisable: %d:%d:%d, %s, xf86ResAccessEnter is %s\n",
	   pMga->PciInfo->bus, pMga->PciInfo->device, pMga->PciInfo->func,
	   pMga->Primary ? "primary" : "secondary",
	   BOOLTOSTRING(xf86ResAccessEnter));
#endif
    /* Turn off the vgaioen bit. */
#ifdef XSERVER_LIBPCIACCESS
    pci_device_cfg_write_bits(pMga->PciInfo, 0x00000100, 0x00000000,
			      PCI_OPTION_REG);
#else
    pciSetBitsLong(pMga->PciTag, PCI_OPTION_REG, 0x100, 0x000);
#endif
}

static void
VgaIOEnable(void *arg)
{
    MGAPtr pMga = arg;

#ifdef DEBUG
    ErrorF("mga: VgaIOEnable: %d:%d:%d, %s, xf86ResAccessEnter is %s\n",
	   pMga->PciInfo->bus, pMga->PciInfo->device, pMga->PciInfo->func,
	   pMga->Primary ? "primary" : "secondary",
	   BOOLTOSTRING(xf86ResAccessEnter));
#endif
    /* Turn on the vgaioen bit. */
    if (pMga->Primary) {
#ifdef XSERVER_LIBPCIACCESS
	pci_device_cfg_write_bits(pMga->PciInfo, 0x00000100, 0x00000100,
				  PCI_OPTION_REG);
#else
	pciSetBitsLong(pMga->PciTag, PCI_OPTION_REG, 0x100, 0x100);
#endif
    }
}
#endif /* DISABLE_VGA_IO */

void
MGAProbeDDC(ScrnInfoPtr pScrn, int index)
{
    vbeInfoPtr pVbe;
    if (xf86LoadSubModule(pScrn, "vbe")) {
	pVbe = VBEInit(NULL,index);
	ConfiguredMonitor = vbeDoEDID(pVbe, NULL);
	vbeFree(pVbe); 
    }
}

Bool
MGAMavenRead(ScrnInfoPtr pScrn, I2CByte reg, I2CByte *val)
{
	MGAPtr pMga = MGAPTR(pScrn);

	if (!pMga->Maven) return FALSE;

	/* FIXME: Using private interfaces for the moment until a more
	 * flexible xf86I2CWriteRead() variant shows up for us
	 *
	 * MAVEN does _not_ like a start bit in the middle of its transaction
	 * MAVEN does _not_ like ACK at the end of the transaction
	 */

	if (!pMga->Maven_Bus->I2CStart(pMga->Maven_Bus, pMga->Maven->ByteTimeout)) return FALSE;
	if (!pMga->Maven_Bus->I2CPutByte(pMga->Maven, MAVEN_READ)) return FALSE;
	if (!pMga->Maven_Bus->I2CPutByte(pMga->Maven, reg)) return FALSE;
	pMga->Maven_Bus->I2CStop(pMga->Maven);
	if (!pMga->Maven_Bus->I2CGetByte(pMga->Maven, val, 0)) return FALSE;
	pMga->Maven_Bus->I2CStop(pMga->Maven);

	return TRUE;
}

/* Mandatory */
static Bool
MGAPreInit(ScrnInfoPtr pScrn, int flags)
{
    MGAPtr pMga;
    MessageType from;
    int i;
    double real;
    int bytesPerPixel;
    ClockRangePtr clockRanges;
    const char *s;
    int flags24;
    MGAEntPtr pMgaEnt = NULL;
    Bool Default;
#ifdef USEMGAHAL
    ULONG status;
    CARD8 MiscCtlReg;
#endif

    /*
     * Note: This function is only called once at server startup, and
     * not at the start of each server generation.  This means that
     * only things that are persistent across server generations can
     * be initialised here.  xf86Screens[] is (pScrn is a pointer to one
     * of these).  Privates allocated using xf86AllocateScrnInfoPrivateIndex()
     * are too, and should be used for data that must persist across
     * server generations.
     *
     * Per-generation data should be allocated with
     * AllocateScreenPrivateIndex() from the ScreenInit() function.
     */

    /* Check the number of entities, and fail if it isn't one. */
    if (pScrn->numEntities != 1)
	return FALSE;


    pMga = MGAPTR(pScrn);
    /* Set here until dri is enabled */
#ifdef XF86DRI
    pMga->haveQuiescense = 1;
#endif
    /* Get the entity, and make sure it is PCI. */
    pMga->pEnt = xf86GetEntityInfo(pScrn->entityList[0]);
    if (pMga->pEnt->location.type != BUS_PCI)
	return FALSE;

    /* Allocate an entity private if necessary */
    if (xf86IsEntityShared(pScrn->entityList[0])) {
	pMgaEnt = xf86GetEntityPrivate(pScrn->entityList[0],
					MGAEntityIndex)->ptr;
        pMga->entityPrivate = pMgaEnt;
    }

    /* Set pMga->device to the relevant Device section */
    pMga->device = xf86GetDevFromEntity(pScrn->entityList[0],
					pScrn->entityInstanceList[0]);

    if (flags & PROBE_DETECT) {
	MGAProbeDDC(pScrn, pMga->pEnt->index);
	return TRUE;
    }

    /* The vgahw module should be loaded here when needed */
    if (!xf86LoadSubModule(pScrn, "vgahw"))
	return FALSE;

    xf86LoaderReqSymLists(vgahwSymbols, NULL);

    /*
     * Allocate a vgaHWRec
     */
    if (!vgaHWGetHWRec(pScrn))
	return FALSE;

#ifndef XSERVER_LIBPCIACCESS
    /* Find the PCI info for this screen */
    pMga->PciInfo = xf86GetPciInfoForEntity(pMga->pEnt->index);
    pMga->PciTag = pciTag(pMga->PciInfo->bus, pMga->PciInfo->device,
			  pMga->PciInfo->func);
#endif

    pMga->Primary = xf86IsPrimaryPci(pMga->PciInfo);

#ifndef DISABLE_VGA_IO
    xf86SetOperatingState(resVgaIo, pMga->pEnt->index, ResUnusedOpr);
    xf86SetOperatingState(resVgaMem, pMga->pEnt->index, ResDisableOpr);
#else
    /*
     * Set our own access functions, which control the vgaioen bit.
     */
    pMga->Access.AccessDisable = VgaIODisable;
    pMga->Access.AccessEnable = VgaIOEnable;
    pMga->Access.arg = pMga;
    xf86SetAccessFuncs(pMga->pEnt, &pMga->Access, &pMga->Access);
#endif

    /* Set pScrn->monitor */
    pScrn->monitor = pScrn->confScreen->monitor;

    /*
     * Set the Chipset and ChipRev, allowing config file entries to
     * override.
     */
    if (pMga->device->chipset && *pMga->device->chipset) {
	pScrn->chipset = pMga->device->chipset;
        pMga->Chipset = xf86StringToToken(MGAChipsets, pScrn->chipset);
        from = X_CONFIG;
    } else if (pMga->device->chipID >= 0) {
	pMga->Chipset = pMga->device->chipID;
	pScrn->chipset = (char *)xf86TokenToString(MGAChipsets, pMga->Chipset);
	from = X_CONFIG;
	xf86DrvMsg(pScrn->scrnIndex, X_CONFIG, "ChipID override: 0x%04X\n",
		   pMga->Chipset);
    } else {
	from = X_PROBED;
	pMga->Chipset = DEVICE_ID(pMga->PciInfo);
	pScrn->chipset = (char *)xf86TokenToString(MGAChipsets, pMga->Chipset);
    }

    if (pMga->device->chipRev >= 0) {
	pMga->ChipRev = pMga->device->chipRev;
	xf86DrvMsg(pScrn->scrnIndex, X_CONFIG, "ChipRev override: %d\n",
		   pMga->ChipRev);
    } else {
	pMga->ChipRev = CHIP_REVISION(pMga->PciInfo);
    }

    /*
     * This shouldn't happen because such problems should be caught in
     * MGAProbe(), but check it just in case.
     */
    if (pScrn->chipset == NULL) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		   "ChipID 0x%04X is not recognised\n", pMga->Chipset);
	return FALSE;
    }
    if (pMga->Chipset < 0) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		   "Chipset \"%s\" is not recognised\n", pScrn->chipset);
	return FALSE;
    }

    xf86DrvMsg(pScrn->scrnIndex, from, "Chipset: \"%s\"", pScrn->chipset);
    if (pMga->Chipset == PCI_CHIP_MGAG400) {
	if (pMga->ChipRev >= 0x80) 
	    xf86ErrorF(" (G450)\n");
	else
	    xf86ErrorF(" (G400)\n");
    } else {
	xf86ErrorF("\n");
    }

    pMga->is_Gx50 = ((pMga->Chipset == PCI_CHIP_MGAG400) && (pMga->ChipRev >= 0x80))
	|| (pMga->Chipset == PCI_CHIP_MGAG550);
    pMga->is_G200SE = (pMga->Chipset == PCI_CHIP_MGAG200_SE_A_PCI)
	|| (pMga->Chipset == PCI_CHIP_MGAG200_SE_B_PCI);
    pMga->is_G200EV = (pMga->Chipset == PCI_CHIP_MGAG200_EV_PCI);
    pMga->is_G200WB = (pMga->Chipset == PCI_CHIP_MGAG200_WINBOND_PCI);

#ifdef USEMGAHAL
    if (pMga->chip_attribs->HAL_chipset) {
	Bool loadHal = TRUE;
	
	from = X_DEFAULT;
	if (xf86FindOption(pMga->device->options, "NoHal")) {
	    loadHal = !xf86SetBoolOption(pMga->device->options,
					 "NoHal", !loadHal);
	    from = X_CONFIG;
	} else if (xf86FindOption(pMga->device->options, "Hal")) {
	    loadHal = xf86SetBoolOption(pMga->device->options,
					"Hal", loadHal);
	    from = X_CONFIG;
	}
        if (loadHal && xf86LoadSubModule(pScrn, "mga_hal")) {
	  xf86LoaderReqSymLists(halSymbols, NULL);
	  xf86DrvMsg(pScrn->scrnIndex, from,"Matrox HAL module used\n");
	  pMga->HALLoaded = TRUE;
	} else {
	  xf86DrvMsg(pScrn->scrnIndex, from, "Matrox HAL module not loaded "
		     "- using builtin mode setup instead\n");
	  pMga->HALLoaded = FALSE;
	}
    }
#endif

    pMga->DualHeadEnabled = FALSE;
    if (xf86IsEntityShared(pScrn->entityList[0])) {/* dual-head mode requested*/
	if (
#ifdef USEMGAHAL
	    pMga->HALLoaded ||
#endif
	    !MGA_DH_NEEDS_HAL(pMga)) {
	    pMga->DualHeadEnabled = TRUE;
	} else if (xf86IsPrimInitDone(pScrn->entityList[0])) {
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
	 "This card requires the \"mga_hal\" module for dual-head operation\n"
	 "\tIt can be found at the Matrox web site <http://www.matrox.com>\n");
	}
    }

    /*
     * In case of DualHead, we need to determine if we are the 'master' head
     * or the 'slave' head. In order to do that, at the end of the first
     * initialisation, PrimInit is set as DONE to the shared entity. So that
     * the second initialisation knows that something has been done before it.
     * This always assume that the first device initialised is the master
     * head, and the second the slave.
     * 
     */
    if (xf86IsEntityShared(pScrn->entityList[0])) {      /* dual-head mode */
        if (!xf86IsPrimInitDone(pScrn->entityList[0])) { /* Is it the first initialisation? */
            /* First CRTC  */
            pMga->SecondCrtc = FALSE;
            pMga->HWCursor = TRUE;
            pMgaEnt->pScrn_1 = pScrn;
        } else if (pMga->DualHeadEnabled) {
            /* Second CRTC */
            pMga->SecondCrtc = TRUE;
            pMga->HWCursor = FALSE;
            pMgaEnt->pScrn_2 = pScrn;
            pScrn->AdjustFrame = MGAAdjustFrameCrtc2;
	    /*
	     * Fail initialization of second head if we are in MergeFB mode,
	     * since we do it ourselfs.
             */
            if(pMgaEnt->pScrn_1 && MGAPTR(pMgaEnt->pScrn_1)->MergedFB) {
		xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
			   "Primary head in Merged framebuffer mode. \n"
			   "Don't let Xfree try to manage the second head.\n"
			   "Remove the second screen in the \"ServerLayout\"\n"
			   "Section of the config file.");
		return FALSE;
	    }
        } else {
	    return FALSE;
	}
    }

    if (pMga->DualHeadEnabled) {
#ifdef XF86DRI
        pMga->GetQuiescence = MGAGetQuiescenceShared;
#endif
    } else {                                              /* single-head mode */
        pMga->SecondCrtc = FALSE;
        pMga->HWCursor = TRUE;
#ifdef XF86DRI
        pMga->GetQuiescence = MGAGetQuiescence;
#endif
    }

   
    
    /*
     * The first thing we should figure out is the depth, bpp, etc.
     * Our default depth is 8, so pass it to the helper function.
     * We support both 24bpp and 32bpp layouts, so indicate that.
     */

    /* Prefer 24bpp fb unless the Overlay option is set, or DRI is
     * supported.
     */
    flags24 = Support24bppFb | Support32bppFb | SupportConvert32to24;
    s = xf86TokenToOptName(MGAOptions, OPTION_OVERLAY);
#ifndef XF86DRI
    if (!(xf86FindOption(pScrn->confScreen->options, s) ||
	  xf86FindOption(pMga->device->options, s))) {
	flags24 |= PreferConvert32to24;
    }
#endif

    if (pMga->SecondCrtc)
	flags24 = Support32bppFb;

    if (pMga->is_G200SE)
	pScrn->confScreen->defaultdepth = 16;

    if (!xf86SetDepthBpp(pScrn, 0, 0, 0, flags24)) {
	return FALSE;
    } else {
	/* Check that the returned depth is one we support */
	switch (pScrn->depth) {
	case 8:
	case 16:
	case 24:
	    /* OK */
	    break;
	case 15:
	    if (pMga->Chipset != PCI_CHIP_MGAG200_SE_A_PCI)
		break;
	default:
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		       "Given depth (%d) is not supported by this driver\n",
		       pScrn->depth);
	    return FALSE;
	}
    }
    xf86PrintDepthBpp(pScrn);

    /*
     * This must happen after pScrn->display has been set because
     * xf86SetWeight references it.
     */
    if (pScrn->depth > 8) {
	/* The defaults are OK for us */
	rgb zeros = {0, 0, 0};

	if (!xf86SetWeight(pScrn, zeros, zeros)) {
	    return FALSE;
	} else {
	    /* XXX check that weight returned is supported */
            ;
        }
    }

    bytesPerPixel = pScrn->bitsPerPixel / 8;

    /* We use a programmable clock */
    pScrn->progClock = TRUE;

    /* Collect all of the relevant option flags (fill in pScrn->options) */
    xf86CollectOptions(pScrn, NULL);

    /* Process the options */
    if (!(pMga->Options = xalloc(sizeof(MGAOptions))))
	return FALSE;
    memcpy(pMga->Options, MGAOptions, sizeof(MGAOptions));
    xf86ProcessOptions(pScrn->scrnIndex, pScrn->options, pMga->Options);

    if (pMga->is_G200SE) {
        /* Disable MTRR support on PCIe systems */
#ifdef XSERVER_LIBPCIACCESS
	uint32_t temp;

	pci_device_cfg_read_u32(pMga->PciInfo, & temp, 0xDC);
#else
        CARD32 temp = pciReadLong(pMga->PciTag, 0xDC);
#endif

        if ((temp & 0x0000FF00) != 0x0) {
            xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Disabling MTRR support.\n");
            pScrn->options = xf86ReplaceBoolOption(pScrn->options, "MTRR", FALSE);
        }
    }

    if (pMga->is_G200WB && xf86ReturnOptValBool(pMga->Options, OPTION_KVM, TRUE)) {
        pMga->KVM = TRUE;
        xf86DrvMsg(pScrn->scrnIndex, X_CONFIG, "Enabling KVM\n");
    }
    
#if !defined(__powerpc__)
    pMga->softbooted = FALSE;
    Default = (pMga->chip_attribs->dual_head_possible
	       && !pMga->Primary && !pMga->SecondCrtc);

    if (xf86ReturnOptValBool(pMga->Options, OPTION_INT10, Default) &&
        xf86LoadSubModule(pScrn, "int10")) {
        xf86Int10InfoPtr pInt;

	xf86LoaderReqSymLists(int10Symbols, NULL);
        xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Initializing int10\n");
        pInt = xf86InitInt10(pMga->pEnt->index);
	if (pInt) pMga->softbooted = TRUE;
        xf86FreeInt10(pInt);
    }
#endif

    /* Set the bits per RGB for 8bpp mode */
    if (pScrn->depth == 8)
	pScrn->rgbBits = 8;

#ifdef XF86DRI
    from = X_DEFAULT;
    pMga->agpMode = MGA_DEFAULT_AGP_MODE;

    if (xf86GetOptValInteger(pMga->Options,
			     OPTION_AGP_MODE, &(pMga->agpMode))) {
       if (pMga->agpMode < 1) {
	  pMga->agpMode = 1;
       }
       if (pMga->agpMode > MGA_MAX_AGP_MODE) {
	  pMga->agpMode = MGA_MAX_AGP_MODE;
       }
       from = X_CONFIG;
    }
    if (xf86GetOptValInteger(pMga->Options,
                             OPTION_AGP_SIZE, &(pMga->agpSize))) {
                             /* check later */
       xf86DrvMsg(pScrn->scrnIndex, from, "Using %d MB of AGP memory\n",
	          pMga->agpSize);
    }

    xf86DrvMsg(pScrn->scrnIndex, from, "Using AGP %dx mode\n",
	       pMga->agpMode);

    if (xf86ReturnOptValBool(pMga->Options, OPTION_OLDDMA, FALSE)) {
	pMga->useOldDmaInit = TRUE;
    }

    if (xf86ReturnOptValBool(pMga->Options, OPTION_PCIDMA, FALSE)) {
	pMga->forcePciDma = TRUE;
    }
#endif

    from = X_DEFAULT;

    /*
     * The preferred method is to use the "hw cursor" option as a tri-state
     * option, with the default set above.
     */
    if (xf86GetOptValBool(pMga->Options, OPTION_HW_CURSOR, &pMga->HWCursor)) {
	from = X_CONFIG;
    }

    /* For compatibility, accept this too (as an override) */
    if (xf86ReturnOptValBool(pMga->Options, OPTION_SW_CURSOR, FALSE)) {
    from = X_CONFIG;
    pMga->HWCursor = FALSE;
    }
    xf86DrvMsg(pScrn->scrnIndex, from, "Using %s cursor\n",
        pMga->HWCursor ? "HW" : "SW");

    if (xf86ReturnOptValBool(pMga->Options, OPTION_NOACCEL, FALSE)) {
	pMga->NoAccel = TRUE;
	xf86DrvMsg(pScrn->scrnIndex, X_CONFIG, "Acceleration disabled\n");
    } else {
	int from = X_DEFAULT;
#ifdef USE_EXA
	char *s = xf86GetOptValString(pMga->Options, OPTION_ACCELMETHOD);
#endif
	pMga->NoAccel = FALSE;
	pMga->Exa = FALSE;
#ifdef USE_EXA
	if (!xf86NameCmp(s, "EXA")) {
	    pMga->Exa = TRUE;
	    from = X_CONFIG;
	}
#endif
	xf86DrvMsg(pScrn->scrnIndex, from, "Using %s acceleration\n",
		   pMga->Exa ? "EXA" : "XAA");
    }
    if (xf86ReturnOptValBool(pMga->Options, OPTION_PCI_RETRY, FALSE)) {
	pMga->UsePCIRetry = TRUE;
	xf86DrvMsg(pScrn->scrnIndex, X_CONFIG, "PCI retry enabled\n");
    }
    if (xf86ReturnOptValBool(pMga->Options, OPTION_SYNC_ON_GREEN, FALSE)) {
	pMga->SyncOnGreen = TRUE;
	xf86DrvMsg(pScrn->scrnIndex, X_CONFIG, "Sync-on-Green enabled\n");
    }
    if (xf86ReturnOptValBool(pMga->Options, OPTION_SHOWCACHE, FALSE)) {
	pMga->ShowCache = TRUE;
	xf86DrvMsg(pScrn->scrnIndex, X_CONFIG, "ShowCache enabled\n");
    }
    if (xf86ReturnOptValBool(pMga->Options, OPTION_MGA_SDRAM, FALSE)) {
	pMga->HasSDRAM = TRUE;
	xf86DrvMsg(pScrn->scrnIndex, X_CONFIG, "Has SDRAM\n");
    }
    if (xf86GetOptValFreq(pMga->Options, OPTION_SET_MCLK, OPTUNITS_MHZ, &real)) {
	pMga->MemClk = (int)(real * 1000.0);
    }

    if(xf86GetOptValInteger(pMga->Options, OPTION_VIDEO_KEY, &(pMga->videoKey))) {
	xf86DrvMsg(pScrn->scrnIndex, X_CONFIG, "video key set to 0x%x\n",
				pMga->videoKey);
    } else {
	pMga->videoKey =  (1 << pScrn->offset.red) |
			  (1 << pScrn->offset.green) |
        (((pScrn->mask.blue >> pScrn->offset.blue) - 1) << pScrn->offset.blue);
    }
    if (xf86ReturnOptValBool(pMga->Options, OPTION_SHADOW_FB, FALSE)) {
	pMga->ShadowFB = TRUE;
	pMga->NoAccel = TRUE;
	xf86DrvMsg(pScrn->scrnIndex, X_CONFIG,
		"Using \"Shadow Framebuffer\" - acceleration disabled\n");
    }
    if (xf86ReturnOptValBool(pMga->Options, OPTION_FBDEV, FALSE)) {
	pMga->FBDev = TRUE;
	xf86DrvMsg(pScrn->scrnIndex, X_CONFIG,
		"Using framebuffer device\n");
    }
    if (xf86ReturnOptValBool(pMga->Options, OPTION_OVERCLOCK_MEM, FALSE)) {
	pMga->OverclockMem = TRUE;
	xf86DrvMsg(pScrn->scrnIndex, X_CONFIG, "Overclocking memory\n");
    }
    if (xf86ReturnOptValBool(pMga->Options, OPTION_TEXTURED_VIDEO, FALSE)) {
	pMga->TexturedVideo = TRUE;
    }
    if (xf86ReturnOptValBool(pMga->Options, OPTION_MERGEDFB, FALSE)) {
        if(!MGAISGx50(pMga)) {
            xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                "\"Merged Framebuffer\" mode only supported on G450 and G550 boards.\n");
        } else { 
#ifdef USEMGAHAL
            if(pMga->HALLoaded)
            { 
                pMga->MergedFB = TRUE;
                xf86DrvMsg(pScrn->scrnIndex, X_CONFIG,
                        "Using \"Merged Framebuffer\" mode.\n");
                /*
                * a few options that won't work well together
                */
                if(pMga->HWCursor) /*Should we give the choice? */
                    xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                        " -- Hardware Cursor disabled.\n");
                pMga->HWCursor = FALSE; 
                if(pMga->ShadowFB) 
                    xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                        " -- Shadow Framebuffer disabled.\n");
                pMga->ShadowFB = FALSE;
                if(pMga->FBDev) 
                    xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                        " -- Framebuffer device disabled.\n");
                pMga->FBDev = FALSE;
            } /* MGA_HAL */
            else
#endif
            { 
                xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                    "HALLib not loaded! NOT using \"Merged Framebuffer\" mode.\n");
            } /* MGA_NOT_HAL */
        } /* ISMGAGx50() */
    }
    if (pMga->FBDev) {
	/* check for linux framebuffer device */
	if (!xf86LoadSubModule(pScrn, "fbdevhw"))
	    return FALSE;
	xf86LoaderReqSymLists(fbdevHWSymbols, NULL);
	if (!fbdevHWInit(pScrn, pMga->PciInfo, NULL))
	    return FALSE;
	pScrn->SwitchMode    = fbdevHWSwitchModeWeak();
	pScrn->AdjustFrame   = fbdevHWAdjustFrameWeak();
	pScrn->EnterVT       = MGAEnterVTFBDev;
	pScrn->LeaveVT       = fbdevHWLeaveVTWeak();
	pScrn->ValidMode     = fbdevHWValidModeWeak();
    }
    pMga->Rotate = 0;
    if ((s = xf86GetOptValString(pMga->Options, OPTION_ROTATE))) {
        if(!pMga->MergedFB) {
            if(!xf86NameCmp(s, "CW")) {
                pMga->ShadowFB = TRUE;
                pMga->NoAccel = TRUE;
                pMga->HWCursor = FALSE;
                pMga->Rotate = 1;
                xf86DrvMsg(pScrn->scrnIndex, X_CONFIG,
                        "Rotating screen clockwise - acceleration disabled\n");
            } else
            if(!xf86NameCmp(s, "CCW")) {
                pMga->ShadowFB = TRUE;
                pMga->NoAccel = TRUE;
                pMga->HWCursor = FALSE;
                pMga->Rotate = -1;
                xf86DrvMsg(pScrn->scrnIndex, X_CONFIG,
                        "Rotating screen counter clockwise - acceleration disabled\n");
            } else {
                xf86DrvMsg(pScrn->scrnIndex, X_CONFIG,
                        "\"%s\" is not a valid value for Option \"Rotate\"\n", s);
                xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Valid options are \"CW\" or \"CCW\"\n");
            }
        } else {
            xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                " -- Rotation disabled.\n");
        }
    }

    switch (pMga->Chipset) {
    case PCI_CHIP_MGA2064:
    case PCI_CHIP_MGA2164:
    case PCI_CHIP_MGA2164_AGP:
	MGA2064SetupFuncs(pScrn);
	break;
    case PCI_CHIP_MGA1064:
    case PCI_CHIP_MGAG100:
    case PCI_CHIP_MGAG100_PCI:
    case PCI_CHIP_MGAG200:
    case PCI_CHIP_MGAG200_PCI:
    case PCI_CHIP_MGAG200_SE_A_PCI:
    case PCI_CHIP_MGAG200_SE_B_PCI:
    case PCI_CHIP_MGAG200_WINBOND_PCI:
    case PCI_CHIP_MGAG200_EV_PCI:
    case PCI_CHIP_MGAG400:
    case PCI_CHIP_MGAG550:
	MGAGSetupFuncs(pScrn);
	break;
    }

    /* ajv changes to reflect actual values. see sdk pp 3-2. */
    /* these masks just get rid of the crap in the lower bits */

    /* For the 2064 and older rev 1064, base0 is the MMIO and base1 is
     * the framebuffer.
     */

    switch (pMga->chip_attribs->BARs) {
    case old_BARs:
	pMga->framebuffer_bar = 1;
	pMga->io_bar = 0;
	pMga->iload_bar = -1;
	break;
    case probe_BARs:
	if (pMga->ChipRev < 3) {
	    pMga->framebuffer_bar = 1;
	    pMga->io_bar = 0;
	    pMga->iload_bar = 2;
	    break;
	}
	/* FALLTHROUGH */
    case new_BARs:
	pMga->framebuffer_bar = 0;
	pMga->io_bar = 1;
	pMga->iload_bar = 2;
	break;
    }


#ifdef XSERVER_LIBPCIACCESS
    pMga->FbAddress = pMga->PciInfo->regions[pMga->framebuffer_bar].base_addr;
#else
    pMga->FbAddress = pMga->PciInfo->memBase[pMga->framebuffer_bar] & 0xff800000;
#endif

    xf86DrvMsg(pScrn->scrnIndex, X_PROBED, "Linear framebuffer at 0x%lX\n",
	       (unsigned long)pMga->FbAddress);

#ifdef XSERVER_LIBPCIACCESS
    xf86DrvMsg(pScrn->scrnIndex, X_PROBED, "MMIO registers at 0x%lX\n",
	       (unsigned long) pMga->PciInfo->regions[pMga->io_bar].base_addr);
#else
    pMga->IOAddress = pMga->PciInfo->memBase[pMga->io_bar] & 0xffffc000;

    xf86DrvMsg(pScrn->scrnIndex, from, "MMIO registers at 0x%lX\n",
	       (unsigned long)pMga->IOAddress);
#endif

    if (pMga->iload_bar != -1) {
#ifdef XSERVER_LIBPCIACCESS
	xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
		   "Pseudo-DMA transfer window at 0x%lX\n",
		   (unsigned long) pMga->PciInfo->regions[pMga->iload_bar].base_addr);
#else
	if (pMga->PciInfo->memBase[2] != 0) {
	    pMga->ILOADAddress = pMga->PciInfo->memBase[2] & 0xffffc000;
	    xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
		       "Pseudo-DMA transfer window at 0x%lX\n",
		       (unsigned long)pMga->ILOADAddress);
	}
#endif
    }


#ifndef XSERVER_LIBPCIACCESS
    /*
     * Find the BIOS base.  Get it from the PCI config if possible.  Otherwise
     * use the VGA default.  Allow the config file to override this.
     */

    pMga->BiosFrom = X_NONE;
    if (pMga->device->BiosBase != 0) {
	/* XXX This isn't used */
	pMga->BiosAddress = pMga->device->BiosBase;
	pMga->BiosFrom = X_CONFIG;
    } else {
	/* details: rombase sdk pp 4-15 */
	if (pMga->PciInfo->biosBase != 0) {
	    pMga->BiosAddress = pMga->PciInfo->biosBase & 0xffff0000;
	    pMga->BiosFrom = X_PROBED;
	} else if (pMga->Primary) {
	    pMga->BiosAddress = 0xc0000;
	    pMga->BiosFrom = X_DEFAULT;
	}
    }
    if (pMga->BiosAddress) {
	xf86DrvMsg(pScrn->scrnIndex, pMga->BiosFrom, "BIOS at 0x%lX\n",
		   (unsigned long)pMga->BiosAddress);
    }
#endif


    if (xf86RegisterResources(pMga->pEnt->index, NULL, ResExclusive)) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		"xf86RegisterResources() found resource conflicts\n");
	MGAFreeRec(pScrn);
	return FALSE;
    }

    /*
     * Read the BIOS data struct
     */

#if defined(__alpha__) && !defined(XSERVER_LIBPCIACCESS)
    /* 
     * Some old Digital-OEMed Matrox Millennium I cards have a VGA
     * disable switch.  If the disable is on, we can't read the BIOS,
     * and pMga->BiosAddress = 0x0. The disable switch is needed to
     * allow multi-head operation with brain-dead console code... ;-}
     */
    
    if ((pMga->BiosAddress == 0) && !xf86IsPrimaryPci(pMga->PciInfo))
        xf86DrvMsg(pScrn->scrnIndex, pMga->BiosFrom,
                   "BIOS not found, skipping read\n");
    else
#endif
    mga_read_and_process_bios( pScrn );


    /* Since the BIOS can swap DACs during the initialisation of G550, we need to
     * store which DAC this instance of the driver is taking care of. This is done
     * by checking a flag stored in the ROM by the BIOS at a fixed address. */

    if (!pMga->SecondCrtc) 
        pMga->SecondOutput = FALSE;
    else
        pMga->SecondOutput = TRUE;

    if (pMga->Chipset == PCI_CHIP_MGAG550) {
        if (!pMga->SecondCrtc) {
            pMga->SecondOutput = (pMga->BiosOutputMode & 0x1) ? TRUE : FALSE;
        } else {
            pMga->SecondOutput = (pMga->BiosOutputMode & 0x1) ? FALSE : TRUE;
        }
    }


    /* HW bpp matches reported bpp */
    pMga->HwBpp = pScrn->bitsPerPixel;

    /*
     * Reset card if it isn't primary one
     */
    if ( (!pMga->Primary && !pMga->FBDev) || xf86IsPc98() )
        MGASoftReset(pScrn);

    /*
     * If the user has specified the amount of memory in the XF86Config
     * file, we respect that setting.
     */
    from = X_PROBED;
    if (pMga->device->videoRam != 0) {
	pScrn->videoRam = pMga->device->videoRam;
	from = X_CONFIG;
    } else if (pMga->FBDev) {
	pScrn->videoRam = fbdevHWGetVidmem(pScrn)/1024;
    } else {
	pScrn->videoRam = MGACountRam(pScrn);
    }

    if (pScrn->videoRam == 0) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		   "Unable to detect video RAM.\n");
	return FALSE;
    }

    if (pMga->DualHeadEnabled) {
       /* This takes gives either half or 8 meg to the second head
	* whichever is less. */
        if(pMga->SecondCrtc == FALSE) {
	    Bool UseHalf = FALSE;
	    int adjust;

	    xf86GetOptValBool(pMga->Options, OPTION_CRTC2HALF, &UseHalf);
	    adjust = pScrn->videoRam / 2;

	    if (UseHalf == TRUE ||
		  xf86GetOptValInteger(pMga->Options, OPTION_CRTC2RAM, &adjust)) {
	        xf86DrvMsg(pScrn->scrnIndex, X_CONFIG,
			   "Crtc2 will use %dK of VideoRam\n",
			   adjust);
	    } else {
	        adjust = min(adjust, 8192);
	        xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
			   "Crtc2 will use %dK of VideoRam\n",
			   adjust);
	    }
	    pMgaEnt->mastervideoRam = pScrn->videoRam - adjust;
	    pScrn->videoRam = pMgaEnt->mastervideoRam;
	    pMgaEnt->slavevideoRam = adjust;
	    pMgaEnt->masterFbAddress = pMga->FbAddress;
	    pMga->FbMapSize =
	       pMgaEnt->masterFbMapSize = pScrn->videoRam * 1024;
	    pMgaEnt->slaveFbAddress = pMga->FbAddress + 
	       pMgaEnt->masterFbMapSize;
	    pMgaEnt->slaveFbMapSize = pMgaEnt->slavevideoRam * 1024;
	    pMga->realSrcOrg = pMga->SrcOrg = 0;
	    pMga->DstOrg = 0;
	} else {
	    pMga->FbAddress = pMgaEnt->slaveFbAddress;
	    pMga->FbMapSize = pMgaEnt->slaveFbMapSize;
	    pScrn->videoRam = pMgaEnt->slavevideoRam;
	    pMga->DstOrg = pMga->realSrcOrg =
	      pMgaEnt->slaveFbAddress - pMgaEnt->masterFbAddress;
	    pMga->SrcOrg = 0; /* This is not stored in hw format!! */
	}
        pMgaEnt->refCount++;
    } else {
        /* Normal Handling of video ram etc */
        pMga->FbMapSize = pScrn->videoRam * 1024;
        switch(pMga->Chipset) {
	  case PCI_CHIP_MGAG550:
	  case PCI_CHIP_MGAG400:
	  case PCI_CHIP_MGAG200:
	  case PCI_CHIP_MGAG200_PCI:
	  case PCI_CHIP_MGAG200_SE_A_PCI:
	  case PCI_CHIP_MGAG200_SE_B_PCI:
          case PCI_CHIP_MGAG200_WINBOND_PCI:
	  case PCI_CHIP_MGAG200_EV_PCI:
	    pMga->SrcOrg = 0;
	    pMga->DstOrg = 0;
	    break;
	  default:
	    break;
	}
    }
    xf86DrvMsg(pScrn->scrnIndex, from, "VideoRAM: %d kByte\n",
               pScrn->videoRam);

   /* Set the bpp shift value */
    pMga->BppShifts[0] = 0;
    pMga->BppShifts[1] = 1;
    pMga->BppShifts[2] = 0;
    pMga->BppShifts[3] = 2;

    /*
     * fill MGAdac struct
     * Warning: currently, it should be after RAM counting
     */
    (*pMga->PreInit)(pScrn);

#if !defined(__powerpc__)

    /* Read and print the Monitor DDC info */
    pScrn->monitor->DDC = MGAdoDDC(pScrn);
#endif /* !__powerpc__ */

    if (!pScrn->monitor->DDC && pMga->is_G200SE) {
	/* Jam in ranges big enough for 1024x768 */
	if (!pScrn->monitor->nHsync) {
	    pScrn->monitor->nHsync = 1;
	    pScrn->monitor->hsync[0].lo = 31.5;
	    pScrn->monitor->hsync[0].hi = 48.0;
	}
	if (!pScrn->monitor->nVrefresh) {
	    pScrn->monitor->nVrefresh = 1;
	    pScrn->monitor->vrefresh[0].lo = 56.0;
	    pScrn->monitor->vrefresh[0].hi = 75.0;
	}
    }
	    

    /*
     * If the driver can do gamma correction, it should call xf86SetGamma()
     * here.
     */

    {
	Gamma zeros = {0.0, 0.0, 0.0};

	if (!xf86SetGamma(pScrn, zeros)) {
	    return FALSE;
	}
    }


    /* XXX Set HW cursor use */

    /* Set the min pixel clock */
    pMga->MinClock = 17750;
    xf86DrvMsg(pScrn->scrnIndex, X_DEFAULT, "Min pixel clock is %d MHz\n",
	       pMga->MinClock / 1000);
    /*
     * If the user has specified ramdac speed in the XF86Config
     * file, we respect that setting.
     */
    if (pMga->device->dacSpeeds[0]) {
	int speed = 0;

	switch (pScrn->bitsPerPixel) {
	case 8:
	   speed = pMga->device->dacSpeeds[DAC_BPP8];
	   break;
	case 16:
	   speed = pMga->device->dacSpeeds[DAC_BPP16];
	   break;
	case 24:
	   speed = pMga->device->dacSpeeds[DAC_BPP24];
	   break;
	case 32:
	   speed = pMga->device->dacSpeeds[DAC_BPP32];
	   break;
	}
	if (speed == 0)
	    pMga->MaxClock = pMga->device->dacSpeeds[0];
	else
	    pMga->MaxClock = speed;
	from = X_CONFIG;
    } else {
	pMga->MaxClock = pMga->Dac.maxPixelClock;
	from = pMga->Dac.ClockFrom;
    }
    if(pMga->SecondCrtc == TRUE) {
        /* Override on 2nd crtc */
	if ((pMga->ChipRev >= 0x80) || (pMga->Chipset == PCI_CHIP_MGAG550)) {
	    /* G450, G550 */
	    pMga->MaxClock = 234000;
	} else {
	    pMga->MaxClock = 135000;
	}
    }
    xf86DrvMsg(pScrn->scrnIndex, from, "Max pixel clock is %d MHz\n",
	       pMga->MaxClock / 1000);
    /*
     * Setup the ClockRanges, which describe what clock ranges are available,
     * and what sort of modes they can be used for.
     */
    clockRanges = xnfcalloc(sizeof(ClockRange), 1);
    clockRanges->next = NULL;
    clockRanges->minClock = pMga->MinClock;
    clockRanges->maxClock = pMga->MaxClock;
    clockRanges->clockIndex = -1;		/* programmable */
    clockRanges->interlaceAllowed = TRUE;
    clockRanges->doubleScanAllowed = TRUE;
#ifdef USEMGAHAL
    MGA_HAL(clockRanges->interlaceAllowed = FALSE);
    MGA_HAL(clockRanges->doubleScanAllowed = FALSE);
#endif
    if (pMga->SecondCrtc == TRUE) 
	clockRanges->interlaceAllowed = FALSE;

    clockRanges->ClockMulFactor = 1;
    clockRanges->ClockDivFactor = 1;

    /* Only set MemClk if appropriate for the ramdac */
    if (pMga->Dac.SetMemClk) {
	if (pMga->MemClk == 0) {
	    pMga->MemClk = pMga->Dac.MemoryClock;
	    from = pMga->Dac.MemClkFrom;
	} else
	    from = X_CONFIG;
	xf86DrvMsg(pScrn->scrnIndex, from, "MCLK used is %.1f MHz\n",
		   pMga->MemClk / 1000.0);
    }

    /*
     * xf86ValidateModes will check that the mode HTotal and VTotal values
     * don't exceed the chipset's limit if pScrn->maxHValue and
     * pScrn->maxVValue are set.  Since our MGAValidMode() already takes
     * care of this, we don't worry about setting them here.
     */
    {
	int Pitches1[] =
	  {640, 768, 800, 960, 1024, 1152, 1280, 1600, 1920, 2048, 0};
	int Pitches2[] =
	  {512, 640, 768, 800, 832, 960, 1024, 1152, 1280, 1600, 1664,
		1920, 2048, 0};
	int *linePitches = NULL;
	int minPitch = 256;
	int maxPitch = 2048;

        switch(pMga->Chipset) {
	case PCI_CHIP_MGA2064:
	   if (!pMga->NoAccel) {
		linePitches = xalloc(sizeof(Pitches1));
		memcpy(linePitches, Pitches1, sizeof(Pitches1));
		minPitch = maxPitch = 0;
	   }
	   break;
	case PCI_CHIP_MGA2164:
	case PCI_CHIP_MGA2164_AGP:
	case PCI_CHIP_MGA1064:
	   if (!pMga->NoAccel) {
		linePitches = xalloc(sizeof(Pitches2));
		memcpy(linePitches, Pitches2, sizeof(Pitches2));
		minPitch = maxPitch = 0;
	   }
	   break;
	case PCI_CHIP_MGAG100:
	case PCI_CHIP_MGAG100_PCI:
	   maxPitch = 2048;
	   break;
	case PCI_CHIP_MGAG200:
	case PCI_CHIP_MGAG200_PCI:
	case PCI_CHIP_MGAG200_SE_A_PCI:
	case PCI_CHIP_MGAG200_SE_B_PCI:
        case PCI_CHIP_MGAG200_WINBOND_PCI:
	case PCI_CHIP_MGAG200_EV_PCI:
	case PCI_CHIP_MGAG400:
	case PCI_CHIP_MGAG550:
	   maxPitch = 4096;
	   break;
	}

	i = xf86ValidateModes(pScrn, pScrn->monitor->Modes,
			      pScrn->display->modes, clockRanges,
			      linePitches, minPitch, maxPitch,
			      pMga->Roundings[(pScrn->bitsPerPixel >> 3) - 1] *
					pScrn->bitsPerPixel, 128, 2048,
			      pScrn->display->virtualX,
			      pScrn->display->virtualY,
			      pMga->FbMapSize,
			      LOOKUP_BEST_REFRESH);
      
	if (linePitches)
	   xfree(linePitches);
    }


    if (i < 1 && pMga->FBDev) {
	fbdevHWUseBuildinMode(pScrn);
	pScrn->displayWidth = pScrn->virtualX; /* FIXME: might be wrong */
	i = 1;
    }
    if (i == -1) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "Validate Modes Failed\n");
	MGAFreeRec(pScrn);
	return FALSE;
    }

    /* Prune the modes marked as invalid */
    xf86PruneDriverModes(pScrn);

    if (i == 0 || pScrn->modes == NULL) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "No valid modes found\n");
	MGAFreeRec(pScrn);
	return FALSE;
    }
#ifdef USEMGAHAL
    MGA_HAL(

    if(pMga->SecondCrtc == FALSE) {
	
        pMga->pBoard = xalloc(sizeof(CLIENTDATA) + MGAGetBOARDHANDLESize());
        pMga->pClientStruct = xalloc(sizeof(CLIENTDATA));
        pMga->pClientStruct->pMga = (MGAPtr) pMga;

        MGAMapMem(pScrn);
	/* 
	 * For some reason the MGAOPM_DMA_BLIT bit needs to be set
	 * on G200 before opening the HALlib. I don't know why.
	 * MATROX: hint, hint.
	 */
	/*if (pMga->Chipset == PCI_CHIP_MGAG200 ||
	  pMga->Chipset == PCI_CHIP_MGAG200_PCI) */{
	    CARD32 opmode;
	    opmode = INREG(MGAREG_OPMODE);
	    OUTREG(MGAREG_OPMODE,  MGAOPM_DMA_BLIT | opmode);
	}
	/* wrapping OpenLibrary to fix broken registers. MATROX: hint, hint. */
	MiscCtlReg = inMGAdac(MGA1064_MISC_CTL);
        MGAOpenLibrary(pMga->pBoard,pMga->pClientStruct,sizeof(CLIENTDATA));
	outMGAdac(MGA1064_MISC_CTL,MiscCtlReg);
        MGAUnmapMem(pScrn);
        pMga->pMgaHwInfo = xalloc(sizeof(MGAHWINFO));
        MGAGetHardwareInfo(pMga->pBoard,pMga->pMgaHwInfo);

        /* copy the board handles */
        if (pMga->DualHeadEnabled) {
            pMgaEnt->pClientStruct = pMga->pClientStruct;
            pMgaEnt->pBoard = pMga->pBoard;
            pMgaEnt->pMgaHwInfo = pMga->pMgaHwInfo;
        }

    } else { /* Second CRTC && entity is shared */ 
        pMga->pBoard = pMgaEnt->pBoard;
        pMga->pClientStruct = pMgaEnt->pClientStruct;
        pMga->pMgaHwInfo = pMgaEnt->pMgaHwInfo;

    }

    MGAFillModeInfoStruct(pScrn,NULL);
    /* Fields usually handled by MGAFillModeInfoStruct, but are unavailable
     * because no mode is given
     */
    pMga->pMgaModeInfo->ulDispWidth = pScrn->virtualX;
    pMga->pMgaModeInfo->ulDispHeight = pScrn->virtualY;

    
    if (ISDIGITAL1(pMga)) 
        xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                    "Digital screen detected on first head.\n");
    if (ISTV1(pMga)) 
        xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                    "TV detected on first head.\n");
    if (ISDIGITAL2(pMga)) 
        xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                    "Digital screen detected on second head.\n");
    if (ISTV2(pMga))
        xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                    "TV detected on second head.\n");

    
    if((status = MGAValidateMode(pMga->pBoard,pMga->pMgaModeInfo)) != 0) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		   "MGAValidateMode from HALlib found the mode to be invalid.\n"
		   "\tError: 0x%lx\n", status);
        return FALSE;
    }
    pScrn->displayWidth = pMga->pMgaModeInfo->ulFBPitch;
    );	/* MGA_HAL */
#endif

    /* If the Device section explicitly set HasSDRAM, don't bother checking.
     */
    if (!pMga->HasSDRAM) {
	if ((pMga->softbooted || pMga->Primary)
	     && pMga->chip_attribs->probe_for_sdram) {
	    uint32_t option_reg;
	    
#ifdef XSERVER_LIBPCIACCESS
	    pci_device_cfg_read_u32(pMga->PciInfo, & option_reg,
				    PCI_OPTION_REG);
#else
	    option_reg = pciReadLong(pMga->PciTag, PCI_OPTION_REG);
#endif	    
	    pMga->HasSDRAM = ((option_reg & (1 << 14)) == 0);
	}
	else {
	    pMga->HasSDRAM = pMga->chip_attribs->has_sdram;
	}

	if (pMga->HasSDRAM) {
	    xf86DrvMsg(pScrn->scrnIndex, X_PROBED, "Has SDRAM\n");
	}
    }

    /*
     * Set the CRTC parameters for all of the modes based on the type
     * of mode, and the chipset's interlace requirements.
     *
     * Calling this is required if the mode->Crtc* values are used by the
     * driver and if the driver doesn't provide code to set them.  They
     * are not pre-initialised at all.
     */
#ifdef USEMGAHAL
    MGA_HAL(xf86SetCrtcForModes(pScrn, 0));
#endif
    MGA_NOT_HAL(xf86SetCrtcForModes(pScrn, INTERLACE_HALVE_V));

    /* Set the current mode to the first in the list */
    pScrn->currentMode = pScrn->modes;

    /* Print the list of modes being used */
    xf86PrintModes(pScrn);

    /* Set display resolution */
    xf86SetDpi(pScrn, 0, 0);

    /*
     * Compute the byte offset into the linear frame buffer where the
     * frame buffer data should actually begin.  According to DDK misc.c
     * line 1023, if more than 4MB is to be displayed, YDSTORG must be set
     * appropriately to align memory bank switching, and this requires a
     * corresponding offset on linear frame buffer access.
     * This is only needed for WRAM.
     */

    pMga->YDstOrg = 0;
    if (pMga->chip_attribs->fb_4mb_quirk &&
	(pScrn->virtualX * pScrn->virtualY * bytesPerPixel > 4*1024*1024)) {
	int offset;
	int offset_modulo = (pScrn->bitsPerPixel == 24) ? 12 : 4;
	int ydstorg_modulo = 64;


	if (pMga->Interleave) {
	    offset_modulo <<= 1;
	    ydstorg_modulo <<= 1;
	}

	offset = (4*1024*1024) % (pScrn->displayWidth * bytesPerPixel);
	pMga->YDstOrg = offset / bytesPerPixel;

	/*
	 * When this was unconditional, it caused a line of horizontal garbage
	 * at the middle right of the screen at the 4Meg boundary in 32bpp
	 * (and presumably any other modes that use more than 4M). But it's
	 * essential for 24bpp (it may not matter either way for 8bpp & 16bpp,
	 * I'm not sure; I didn't notice problems when I checked with and
	 * without.)
	 * DRM Doug Merritt 12/97, submitted to XFree86 6/98 (oops)
	 */
	if (bytesPerPixel < 4) {
	    while ((offset % offset_modulo) != 0 ||
		   (pMga->YDstOrg % ydstorg_modulo) != 0) {
		offset++;
		pMga->YDstOrg = offset / bytesPerPixel;
	    }
	}
    }

    xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, 2, "YDstOrg is set to %d\n",
		   pMga->YDstOrg);
    if(pMga->DualHeadEnabled) {
        if(pMga->SecondCrtc == FALSE) {
	    pMga->FbUsableSize = pMgaEnt->masterFbMapSize;
            /* Allocate HW cursor buffer at the end of video ram */
	    if( pMga->HWCursor && pMga->Dac.CursorOffscreenMemSize ) {
	        if( pScrn->virtualY * pScrn->displayWidth *
		    pScrn->bitsPerPixel / 8 <=
		    pMga->FbUsableSize - pMga->Dac.CursorOffscreenMemSize ) {
		    pMga->FbUsableSize -= pMga->Dac.CursorOffscreenMemSize;
		    pMga->FbCursorOffset =
		      pMgaEnt->masterFbMapSize -
		      pMga->Dac.CursorOffscreenMemSize;
		} else {
		    pMga->HWCursor = FALSE;
		    xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
			       "Too little offscreen memory for HW cursor; "
			       "using SW cursor\n");
		}
	    }
	} else { /* Second CRTC */
	    pMga->FbUsableSize = pMgaEnt->slaveFbMapSize;
	    pMga->HWCursor = FALSE;
	}
    } else { 
        pMga->FbUsableSize = pMga->FbMapSize - pMga->YDstOrg * bytesPerPixel;
           /* Allocate HW cursor buffer at the end of video ram */
        if( pMga->HWCursor && pMga->Dac.CursorOffscreenMemSize ) {
	    if( pScrn->virtualY * pScrn->displayWidth *
	        pScrn->bitsPerPixel / 8 <=
        	pMga->FbUsableSize - pMga->Dac.CursorOffscreenMemSize ) {
	        pMga->FbUsableSize -= pMga->Dac.CursorOffscreenMemSize;
	        pMga->FbCursorOffset =
		  pMga->FbMapSize - pMga->Dac.CursorOffscreenMemSize;
	    } else {
	        pMga->HWCursor = FALSE;
	        xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
			   "Too little offscreen memory for HW cursor; "
			   "using SW cursor\n");
	    }
	}
    }
    /*
     * XXX This should be taken into account in some way in the mode valdation
     * section.
     */


    /* Load the required framebuffer */
    if (!xf86LoadSubModule(pScrn, "fb")) {
	MGAFreeRec(pScrn);
	return FALSE;
    }
    xf86LoaderReqSymLists(fbSymbols, NULL);


    /* Load XAA if needed */
    if (!pMga->NoAccel) {
#ifdef USE_EXA
	if (pMga->Exa) {
	    if (!xf86LoadSubModule(pScrn, "exa")) {
		MGAFreeRec(pScrn);
		return FALSE;
	    } else xf86LoaderReqSymLists(exaSymbols, NULL);
	} else {
#endif
#ifdef USE_XAA
	    if (!xf86LoadSubModule(pScrn, "xaa")) {
		MGAFreeRec(pScrn);
		return FALSE;
	    } else xf86LoaderReqSymLists(xaaSymbols, NULL);
#endif
#ifdef USE_EXA
	}
#endif
    }

    /* Load ramdac if needed */
    if (pMga->HWCursor) {
	if (!xf86LoadSubModule(pScrn, "ramdac")) {
	    MGAFreeRec(pScrn);
	    return FALSE;
	}
	xf86LoaderReqSymLists(ramdacSymbols, NULL);
    }

    /* Load shadowfb if needed */
    if (pMga->ShadowFB) {
	if (!xf86LoadSubModule(pScrn, "shadowfb")) {
	    MGAFreeRec(pScrn);
	    return FALSE;
	}
	xf86LoaderReqSymLists(shadowSymbols, NULL);
    }

#ifdef XF86DRI
    /* Load the dri module if requested. */
    if (xf86ReturnOptValBool(pMga->Options, OPTION_DRI, FALSE)) {
       if (xf86LoadSubModule(pScrn, "dri")) {
	  xf86LoaderReqSymLists(driSymbols, drmSymbols, NULL);
       }
    }
#endif
    pMga->CurrentLayout.bitsPerPixel = pScrn->bitsPerPixel;
    pMga->CurrentLayout.depth = pScrn->depth;
    pMga->CurrentLayout.displayWidth = pScrn->displayWidth;
    pMga->CurrentLayout.weight.red = pScrn->weight.red;
    pMga->CurrentLayout.weight.green = pScrn->weight.green;
    pMga->CurrentLayout.weight.blue = pScrn->weight.blue;
    pMga->CurrentLayout.mode = pScrn->currentMode;
	

    
    if(pMga->MergedFB) {
        MGAPreInitMergedFB(pScrn,flags);
    };

    
#ifdef USEMGAHAL
    MGA_HAL(
    /* Close the library after preinit */
    /* This needs to only happen after this board has completed preinit
     * both times
     */

      if(pMga->DualHeadEnabled) {
	  /* Entity is shared make sure refcount == 2 */
	  /* If ref count is 2 then reset it to 0 */
	  if(pMgaEnt->refCount == 2) {
	      /* Both boards have done there initialization */
	      MGACloseLibrary(pMga->pBoard);

	      if (pMga->pBoard)
	        xfree(pMga->pBoard);
	      if (pMga->pClientStruct)
	        xfree(pMga->pClientStruct);
	      if (pMga->pMgaModeInfo)
	        xfree(pMga->pMgaModeInfo);
	      if (pMga->pMgaHwInfo)
	        xfree(pMga->pMgaHwInfo);
	      pMgaEnt->refCount = 0;
	  }
      } else {
	  MGACloseLibrary(pMga->pBoard);

	  if (pMga->pBoard)
	    xfree(pMga->pBoard);
	  if (pMga->pClientStruct)
	    xfree(pMga->pClientStruct);
	  if (pMga->pMgaModeInfo)
	    xfree(pMga->pMgaModeInfo);
	  if (pMga->pMgaHwInfo)
	    xfree(pMga->pMgaHwInfo);
      }

    );	/* MGA_HAL */
#endif

    xf86SetPrimInitDone(pScrn->entityList[0]);
    
    return TRUE;
}


/*
 * Map the framebuffer and MMIO memory.
 */

static Bool
MGAMapMem(ScrnInfoPtr pScrn)
{
    MGAPtr pMga = MGAPTR(pScrn);
#ifdef XSERVER_LIBPCIACCESS
    struct pci_device *const dev = pMga->PciInfo;
    struct pci_mem_region *region;
    void **memory[2];
    int i, err;
#endif


    if (!pMga->FBDev) {
#ifdef XSERVER_LIBPCIACCESS
        memory[pMga->io_bar] = &pMga->IOBase;
        memory[pMga->framebuffer_bar] = &pMga->FbBase;

        for (i = 0; i < 2; i++) {
            region = &dev->regions[i];
            err = pci_device_map_range(dev,
                                       region->base_addr, region->size,
                                       PCI_DEV_MAP_FLAG_WRITABLE,
                                       memory[i]);

            if (err) {
                xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                           "Unable to map BAR %i.  %s (%d)\n",
                           i, strerror(err), err);
                return FALSE;
            }
        }
#else
	/*
	 * For Alpha, we need to map SPARSE memory, since we need
	 * byte/short access.  This is taken care of automatically by the
	 * os-support layer.
	 */
	pMga->IOBase = xf86MapPciMem(pScrn->scrnIndex,
				     VIDMEM_MMIO | VIDMEM_READSIDEEFFECT,
				     pMga->PciTag, pMga->IOAddress, 0x4000);
	if (pMga->IOBase == NULL)
	    return FALSE;

	pMga->FbBase = xf86MapPciMem(pScrn->scrnIndex, VIDMEM_FRAMEBUFFER,
				     pMga->PciTag, pMga->FbAddress,
				     pMga->FbMapSize);
	if (pMga->FbBase == NULL)
	    return FALSE;
#endif
    }
    else {
	pMga->FbBase = fbdevHWMapVidmem(pScrn);
	if (pMga->FbBase == NULL) {
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		       "Unable to map framebuffer.\n");
	    return FALSE;
	}

	pMga->IOBase = fbdevHWMapMMIO(pScrn);
	if (pMga->IOBase == NULL) {
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "Unable to map MMIO.\n");
	    return FALSE;
	}
    }


    pMga->FbStart = pMga->FbBase + pMga->YDstOrg * (pScrn->bitsPerPixel / 8);

    pMga->ILOADBase = NULL;
    if (pMga->iload_bar != -1) {
#ifdef XSERVER_LIBPCIACCESS
        region = &dev->regions[pMga->iload_bar];
        err = pci_device_map_range(dev,
                                   region->base_addr, region->size,
                                   PCI_DEV_MAP_FLAG_WRITABLE,
                                   (void *) &pMga->ILOADBase);
	if (err) {
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		       "Unable to map BAR 2 (ILOAD region).  %s (%d)\n",
		       strerror(err), err);
	    return FALSE;
	}
#else
	pMga->ILOADBase = xf86MapPciMem(pScrn->scrnIndex,
					VIDMEM_MMIO | VIDMEM_MMIO_32BIT |
					VIDMEM_READSIDEEFFECT,
					pMga->PciTag, pMga->ILOADAddress,
					0x800000);
#endif
    }
	

    return TRUE;
}


/*
 * Unmap the framebuffer and MMIO memory.
 */

static Bool
MGAUnmapMem(ScrnInfoPtr pScrn)
{
    MGAPtr pMga = MGAPTR(pScrn);
#ifdef XSERVER_LIBPCIACCESS
    struct pci_device * const dev = pMga->PciInfo;
#endif

    
    if (!pMga->FBDev) {
#ifdef XSERVER_LIBPCIACCESS
        pci_device_unmap_range(dev, pMga->IOBase, 
			       dev->regions[pMga->io_bar].size);
        pci_device_unmap_range(dev, pMga->FbBase, 
			       dev->regions[pMga->framebuffer_bar].size);
#else
	xf86UnMapVidMem(pScrn->scrnIndex, (pointer)pMga->IOBase, 0x4000);
	xf86UnMapVidMem(pScrn->scrnIndex, (pointer)pMga->FbBase, pMga->FbMapSize);
#endif
    }
    else {
	fbdevHWUnmapVidmem(pScrn);
	fbdevHWUnmapMMIO(pScrn);
    }

    if ((pMga->iload_bar != -1) && (pMga->ILOADBase != NULL)) {
#ifdef XSERVER_LIBPCIACCESS
        pci_device_unmap_range(dev, pMga->ILOADBase,
			       dev->regions[pMga->iload_bar].size);
#else
	xf86UnMapVidMem(pScrn->scrnIndex, (pointer)pMga->ILOADBase, 0x800000);
#endif
    }

    pMga->IOBase = NULL;
    pMga->FbBase = NULL;
    pMga->FbStart = NULL;
    pMga->ILOADBase = NULL;

    return TRUE;
}


/*
 * This function saves the video state.
 */
static void
MGASave(ScrnInfoPtr pScrn)
{
    vgaHWPtr hwp = VGAHWPTR(pScrn);
    vgaRegPtr vgaReg = &hwp->SavedReg;
    MGAPtr pMga = MGAPTR(pScrn);
    MGARegPtr mgaReg = &pMga->SavedReg;

    if(pMga->SecondCrtc == TRUE) return;
#ifdef USEMGAHAL
    MGA_HAL(if (pMga->pBoard != NULL) MGASaveVgaState(pMga->pBoard));
#endif

    /* I need to save the registers for the second head also */
    /* Save the register for 0x80 to 0xa0 */
    /* Could call it dac2Saved */

    /* Only save text mode fonts/text for the primary card */
    (*pMga->Save)(pScrn, vgaReg, mgaReg, pMga->Primary);
}

#ifdef USEMGAHAL
/* Convert DisplayModeRec parameters in MGAMODEINFO parameters.
*  mode parameter optionnal. */
void
MGAFillModeInfoStruct(ScrnInfoPtr pScrn, DisplayModePtr mode)
{
     const char *s;
    MGAPtr pMga = MGAPTR(pScrn);

    Bool digital1 = FALSE;
    Bool digital2 = FALSE;
    Bool tv1 = FALSE;
    Bool tv2 = FALSE;
    Bool swap_head
      = xf86ReturnOptValBool(pMga->Options, OPTION_SWAPPED_HEAD, FALSE);
        
    if(pMga->MergedFB && mode && mode->Private && (mode->PrivSize == 0)) {
        mode = pMga->SecondCrtc ? 
             ((MergedDisplayModePtr)mode->Private)->Monitor2
            : ((MergedDisplayModePtr)mode->Private)->Monitor1;
    }
    
    
    if (pMga->pMgaHwInfo)
    {
	digital1 = ISDIGITAL1(pMga);
	digital2 = ISDIGITAL2(pMga);
	tv1 = ISTV1(pMga);
	tv2 = ISTV2(pMga);
    }

    /*FIXME: causes segfault elsewhere if not commented*/
    /*if(!pMga->pMgaModeInfo)*/ pMga->pMgaModeInfo = xalloc(sizeof(MGAMODEINFO));
    pMga->pMgaModeInfo->flOutput = 0;
    pMga->pMgaModeInfo->ulDeskWidth = pScrn->virtualX;
    pMga->pMgaModeInfo->ulDeskHeight = pScrn->virtualY;
    pMga->pMgaModeInfo->ulFBPitch = 0;
    pMga->pMgaModeInfo->ulBpp = pScrn->bitsPerPixel;
    pMga->pMgaModeInfo->ulZoom = 1;
    pMga->pMgaModeInfo->flSignalMode = 0x10;

    /* Set TV standard */
    if ((s = xf86GetOptValString(pMga->Options, OPTION_TVSTANDARD))) {
    	if (!xf86NameCmp(s, "PAL")) {
    		pMga->pMgaModeInfo->flSignalMode = 0x00;
    		pMga->pMgaModeInfo->ulRefreshRate = 50;
    		pMga->pMgaModeInfo->ulTVStandard = TV_PAL;
    	} else {
    		pMga->pMgaModeInfo->ulRefreshRate = 60;
    		pMga->pMgaModeInfo->ulTVStandard = TV_NTSC;
    	}
    } else {
    	pMga->pMgaModeInfo->ulRefreshRate = 0;
    	pMga->pMgaModeInfo->ulTVStandard = TV_NTSC;
    }

    /* Set Cable Type */
    if ((s = xf86GetOptValString(pMga->Options, OPTION_CABLETYPE))) {
    	if (!xf86NameCmp(s, "SCART_RGB")) {
    		pMga->pMgaModeInfo->ulCableType = TV_SCART_RGB;
    	} else if (!xf86NameCmp(s, "SCART_COMPOSITE")) {
    		pMga->pMgaModeInfo->ulCableType = TV_SCART_COMPOSITE;
    	} else if (!xf86NameCmp(s, "SCART_TYPE2")) {
    		pMga->pMgaModeInfo->ulCableType = TV_SCART_TYPE2;
    	} else {
    		pMga->pMgaModeInfo->ulCableType = TV_YC_COMPOSITE;
    	}
    } else {
    	pMga->pMgaModeInfo->ulCableType = TV_YC_COMPOSITE;
    }

    if(mode) {
        pMga->pMgaModeInfo->ulHorizRate = 0;
        pMga->pMgaModeInfo->ulDispWidth = mode->HDisplay;
        pMga->pMgaModeInfo->ulDispHeight = mode->VDisplay;
        pMga->pMgaModeInfo->ulPixClock = mode->Clock;
        pMga->pMgaModeInfo->ulHFPorch = mode->HSyncStart - mode->HDisplay;
        pMga->pMgaModeInfo->ulHSync = mode->HSyncEnd - mode->HSyncStart;
        pMga->pMgaModeInfo->ulHBPorch = mode->HTotal - mode->HSyncEnd;
        pMga->pMgaModeInfo->ulVFPorch = mode->VSyncStart - mode->VDisplay;
        pMga->pMgaModeInfo->ulVSync = mode->VSyncEnd - mode->VSyncStart;
        pMga->pMgaModeInfo->ulVBPorch = mode->VTotal - mode->VSyncEnd;
    }
    /* Use DstOrg directly */
    /* This is an offset in pixels not memory */
    pMga->pMgaModeInfo->ulDstOrg = pMga->DstOrg / (pScrn->bitsPerPixel / 8);
    pMga->pMgaModeInfo->ulDisplayOrg = pMga->DstOrg / (pScrn->bitsPerPixel / 8);
    pMga->pMgaModeInfo->ulPanXGran = 0;
    pMga->pMgaModeInfo->ulPanYGran = 0;

    if(pMga->SecondCrtc == TRUE) {
	pMga->pMgaModeInfo->flOutput = MGAMODEINFO_SECOND_CRTC |
				       MGAMODEINFO_FORCE_PITCH |
				       MGAMODEINFO_FORCE_DISPLAYORG;
	if (digital2) {
	    pMga->pMgaModeInfo->flOutput |= MGAMODEINFO_DIGITAL2;
	} else if (tv2) {
	    pMga->pMgaModeInfo->flOutput |= MGAMODEINFO_TV;
	} else {
            if (!swap_head) {
	      pMga->pMgaModeInfo->flOutput |= MGAMODEINFO_ANALOG2;
            } else {
              pMga->pMgaModeInfo->flOutput |= MGAMODEINFO_ANALOG1;
            }
	}
    } else { 
	pMga->pMgaModeInfo->flOutput = MGAMODEINFO_FORCE_PITCH;
        if (digital1) {
	    if ((pMga->Chipset == PCI_CHIP_MGAG200) ||
		(pMga->Chipset == PCI_CHIP_MGAG200_PCI)) {
		pMga->pMgaModeInfo->flOutput |= MGAMODEINFO_FLATPANEL1;
		pMga->pMgaModeInfo->flOutput |= MGAMODEINFO_DIGITAL2;
	    } else {
		pMga->pMgaModeInfo->flOutput |= MGAMODEINFO_DIGITAL1;
	    }
        } else if (tv1) {
	    pMga->pMgaModeInfo->flOutput |= MGAMODEINFO_TV;
       	} else {
            if (!swap_head) {
	        pMga->pMgaModeInfo->flOutput |= MGAMODEINFO_ANALOG1;
            } else {
                pMga->pMgaModeInfo->flOutput |= MGAMODEINFO_ANALOG2;
            }
        }
    }
    pMga->pMgaModeInfo->ulFBPitch = pScrn->displayWidth;
}
#endif

/*
 * Initialise a new mode.  This is currently still using the old
 * "initialise struct, restore/write struct to HW" model.  That could
 * be changed.
 */

static Bool
MGAModeInit(ScrnInfoPtr pScrn, DisplayModePtr mode)
{
    vgaHWPtr hwp = VGAHWPTR(pScrn);
    vgaRegPtr vgaReg;
    MGAPtr pMga = MGAPTR(pScrn);
    MGARegPtr mgaReg;

#ifdef USEMGAHAL
   ULONG status;
#endif
    vgaHWUnlock(hwp);

/*    if(pMga->MergedFB && mode && mode->Private && (mode->PrivSize == 0)) {
        mode = (DisplayModePtr)mode->Private;
    }*/
    
    /* Initialise the ModeReg values */
    if (!vgaHWInit(pScrn, mode))
	return FALSE;
    pScrn->vtSema = TRUE;

    if (!(*pMga->ModeInit)(pScrn, mode))
	return FALSE;

    /* Program the registers */
    if (pMga->is_G200SE) {
	MGAG200SEHWProtect(pScrn, TRUE);
    } else {
	vgaHWProtect(pScrn, TRUE);
    }
    vgaReg = &hwp->ModeReg;
    mgaReg = &pMga->ModeReg;
#ifdef USEMGAHAL
    MGA_HAL( 
        MGAFillModeInfoStruct(pScrn,mode);
                    
        /* Validate the parameters */
        if ((status = MGAValidateMode(pMga->pBoard, pMga->pMgaModeInfo)) != 0) {
            xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                    "MGAValidateMode from HALlib found the mode to be invalid.\n"
                    "\tError: %lx\n", status);
            return FALSE;
        }

        /*
         * Find mode for second head. 
         */
        if(pMga->MergedFB) {
    
            MGAFillModeInfoStruct(pMga->pScrn2,mode);
            /* Validates the Video parameters */
            if ((status = MGAValidateVideoParameters(pMga->pBoard, MGAPTR(pMga->pScrn2)->pMgaModeInfo))
                != 0) {
                xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                        "MGAValidateVideoParameters from HALlib found the mode to be invalid.\n\tError: %lx\n", status);
                return FALSE;
            }
        }
    );	 /*MGA_HAL */
    
#endif

#ifdef USEMGAHAL
MGA_HAL(

    /*************************** ESC *****************************/
    TmpMgaModeInfo[0] =  *pMga->pMgaModeInfo;
	
    if(pMga->SecondCrtc == TRUE)
        pMgaModeInfo[1] = pMga->pMgaModeInfo;
    else
        pMgaModeInfo[0] = pMga->pMgaModeInfo;
    
	TmpMgaModeInfo[0].ulDispWidth = 0;
    
    if(!pMga->MergedFB) /* FIXME: Must deal with this once PowerDesk & MergedFB 
                           compatibility will exist */
        MGAFillDisplayModeStruct(mode, pMga->pMgaModeInfo);  
    /*************************************************************/

);	/* MGA_HAL */
#endif

#ifdef XF86DRI
   if (pMga->directRenderingEnabled) {
       DRILock(screenInfo.screens[pScrn->scrnIndex], 0);
   }
#endif

#ifdef USEMGAHAL
    MGA_HAL(
    /* Initialize the board */
    if(MGASetMode(pMga->pBoard,pMga->pMgaModeInfo) != 0) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		"MGASetMode returned an error."
		"  Make sure to validate the mode before.\n");
	return FALSE;
    }
    if(pMga->MergedFB
       && MGASetMode(pMga->pBoard,MGAPTR(pMga->pScrn2)->pMgaModeInfo) != 0) {
  	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		   "MGASetMode returned an error."
		   "  Make sure to validate the mode before.\n");
    }

    );	/* MGA_HAL */

    /* getting around bugs in the HAL lib. MATROX: hint, hint. */
    MGA_HAL(
	    if (pMga->chip_attribs->hwcursor_1064) {
		if(pMga->SecondCrtc == FALSE && pMga->HWCursor == TRUE) {
		outMGAdac(MGA1064_CURSOR_BASE_ADR_LOW, 
			  pMga->FbCursorOffset >> 10);
		outMGAdac(MGA1064_CURSOR_BASE_ADR_HI, 
			  pMga->FbCursorOffset >> 18);
		outMGAdac(MGA1064_CURSOR_CTL, 0x00);
	      }
	    }
    );	/* MGA_HAL */
#endif

    MGA_NOT_HAL((*pMga->Restore)(pScrn, vgaReg, mgaReg, FALSE));

    MGAStormSync(pScrn);
    MGAStormEngineInit(pScrn);

    if (pMga->is_G200SE) {
	MGAG200SEHWProtect(pScrn,FALSE);
    } else {
	vgaHWProtect(pScrn, FALSE);
    }

    if (xf86IsPc98()) {
	if (pMga->Chipset == PCI_CHIP_MGA2064)
	    outb(0xfac, 0x01);
	else
	    outb(0xfac, 0x02);
    }

    MGA_NOT_HAL(
	if (pMga->is_G200SE) {
            OUTREG8(0x1FDE, 0x06);
	    if (pMga->reg_1e24 == 0x01)
		OUTREG8(0x1FDF, 0x03);
	    else 
		OUTREG8(0x1FDF, 0x14);
        }
    );

    pMga->CurrentLayout.mode = mode;

    if(pMga->MergedFB && mode->Private && (mode->PrivSize == 0)) {
	pMga->M1currentMode = (DisplayModePtr)mode->Private;
    }

#ifdef XF86DRI
   if (pMga->directRenderingEnabled)
     DRIUnlock(screenInfo.screens[pScrn->scrnIndex]);
#endif
#ifdef DEBUG
   MGAG450PrintPLL(pScrn);
#endif
    return TRUE;
}

static
void MGARestoreSecondCrtc(ScrnInfoPtr pScrn) 
{
    MGAPtr pMga = MGAPTR(pScrn);
    
    if (MGAISGx50(pMga)) {
        /* Force to return in clone mode */
        if (pMga->SecondOutput 
                && (xf86IsEntityShared(pScrn->entityList[0]) || pMga->SecondCrtc) 
                && !pMga->MergedFB) {
            /* Do this branch if 
             * SecondOutput 
             * and not Unshared Primary 
             * and not Merged Mode (usualy means Unshared Primary) 
             */
            CARD8 ucXDispCtrl = inMGAdac(MGA1064_DISP_CTL);
            
            ucXDispCtrl &= ~MGA1064_DISP_CTL_DAC2OUTSEL_MASK;
            ucXDispCtrl |= MGA1064_DISP_CTL_DAC2OUTSEL_CRTC1;
                
            outMGAdac(MGA1064_DISP_CTL, ucXDispCtrl);
            
       } else {
            CARD8 ucXDispCtrl = inMGAdac(MGA1064_DISP_CTL);
            CARD32 ulC2CTL = INREG(MGAREG_C2CTL);
                
            ucXDispCtrl &= ~MGA1064_DISP_CTL_DAC2OUTSEL_MASK;
            ucXDispCtrl |= MGA1064_DISP_CTL_DAC1OUTSEL_EN;
            ucXDispCtrl |= MGA1064_DISP_CTL_DAC2OUTSEL_CRTC1;

            /* crtcdacsel -> crtc1 */
            ulC2CTL &= ~MGAREG_C2CTL_CRTCDACSEL_CRTC2;
            ulC2CTL |= MGAREG_C2CTL_CRTCDACSEL_CRTC1;
                
            outMGAdac(MGA1064_DISP_CTL, ucXDispCtrl);
            OUTREG(MGAREG_C2CTL, ulC2CTL);
       }
        
    } else {
        /* Force to close second crtc */
        CARD32 ulC2CTL = INREG(MGAREG_C2CTL);
        
        ulC2CTL &= ~MGAREG_C2CTL_C2_EN;

        OUTREG(MGAREG_C2CTL, ulC2CTL);
    }
}

/*
 * Restore the initial (text) mode.
 */
static void
MGARestore(ScrnInfoPtr pScrn)
{
    vgaHWPtr hwp = VGAHWPTR(pScrn);
    vgaRegPtr vgaReg = &hwp->SavedReg;
    MGAPtr pMga = MGAPTR(pScrn);
    MGARegPtr mgaReg = &pMga->SavedReg;

    if (pScrn->pScreen != NULL)
	MGAStormSync(pScrn);

    /*
     * Restore the second crtc if:
     * first and only driver entity
     * second entity
     * Merged Framebuffer mode (first and only driver entity)
     */
    if((!xf86IsEntityShared(pScrn->entityList[0]) && !pMga->SecondCrtc)
       || pMga->SecondCrtc || pMga->MergedFB) {
	/*       if(pMga->MergedFB) {
		 if(pMga->pScrn2) 
                 MGARestoreSecondCrtc(pMga->pScrn2);
		 } else*/
	MGARestoreSecondCrtc(pScrn);
	/* if we are second instance of driver, we've done our job, exit */
	if(pMga->SecondCrtc) return;
    }
    
    /* Only restore text mode fonts/text for the primary card */
    if (pMga->is_G200SE) {
	MGAG200SEHWProtect(pScrn,TRUE);
    } else {
	vgaHWProtect(pScrn, TRUE);
    }
    if (pMga->Primary) {
#ifdef USEMGAHAL
	MGA_HAL(
	    if(pMga->pBoard != NULL) {
		MGASetVgaMode(pMga->pBoard);
		MGARestoreVgaState(pMga->pBoard);
	    }
	    );	/* MGA_HAL */
#endif
        (*pMga->Restore)(pScrn, vgaReg, mgaReg, TRUE);
    } else {
        vgaHWRestore(pScrn, vgaReg, VGA_SR_MODE);
    }

    if (pMga->is_G200SE) {
	MGAG200SEHWProtect(pScrn,FALSE);
    } else {
	vgaHWProtect(pScrn,FALSE);
    }
}


/* Workaround for a G400 CRTC2 display problem */
static void
MGACrtc2FillStrip(ScrnInfoPtr pScrn)
{
    MGAPtr pMga = MGAPTR(pScrn);

    if (pMga->NoAccel) {
	/* Clears the whole screen, but ... */
	bzero(pMga->FbStart,
	    (pScrn->bitsPerPixel >> 3) * pScrn->displayWidth * pScrn->virtualY);
    } else {
	xf86SetLastScrnFlag(pScrn->entityList[0], pScrn->scrnIndex);
	pMga->RestoreAccelState(pScrn);
	pMga->SetupForSolidFill(pScrn, 0, GXcopy, 0xFFFFFFFF);
	pMga->SubsequentSolidFillRect(pScrn, pScrn->virtualX, 0,
				  pScrn->displayWidth - pScrn->virtualX,
				  pScrn->virtualY);
	MGAStormSync(pScrn);
    }
}


/* Mandatory */

/* This gets called at the start of each server generation */

static Bool
MGAScreenInit(int scrnIndex, ScreenPtr pScreen, int argc, char **argv)
{
    ScrnInfoPtr pScrn;
    vgaHWPtr hwp;
    MGAPtr pMga;
    MGARamdacPtr MGAdac;
    int ret;
    VisualPtr visual;
    unsigned char *FBStart;
    int width, height, displayWidth;
    MGAEntPtr pMgaEnt = NULL;
    int f;
    CARD32 VRTemp, FBTemp;
#ifdef XF86DRI
    MessageType driFrom = X_DEFAULT;
#endif
    DPMSSetProcPtr mga_dpms_set_proc = NULL;

    /*
     * First get the ScrnInfoRec
     */
    pScrn = xf86Screens[pScreen->myNum];

    hwp = VGAHWPTR(pScrn);
    pMga = MGAPTR(pScrn);
    MGAdac = &pMga->Dac;

    if (pMga->is_G200SE) {
	VRTemp = pScrn->videoRam;
	FBTemp = pMga->FbMapSize;
	pScrn->videoRam = 8192;
	pMga->FbMapSize = pScrn->videoRam * 1024;
    }
    

    /* Map the MGA memory and MMIO areas */
    if (!MGAMapMem(pScrn))
	return FALSE;


    /* Select functions that vary based on the CRTC configureation of the
     * screen.
     */
    if (!pMga->MergedFB) {
	if (pMga->SecondCrtc) { 
	    mga_dpms_set_proc = MGADisplayPowerManagementSetCrtc2;
	    pScreen->SaveScreen = MGASaveScreenCrtc2;
	}
	else {
	    mga_dpms_set_proc = MGADisplayPowerManagementSet;
	    pScreen->SaveScreen = MGASaveScreen;
	}
    }
    else {
        pScreen->SaveScreen = MGASaveScreenMerged;
	mga_dpms_set_proc = MGADisplayPowerManagementSetMerged;
    }


    if ((pMga->Chipset == PCI_CHIP_MGAG100)
	|| (pMga->Chipset == PCI_CHIP_MGAG100_PCI))
        MGAG100BlackMagic(pScrn);

    if (pMga->DualHeadEnabled) {
       DevUnion *pPriv;
       pPriv = xf86GetEntityPrivate(pScrn->entityList[0], MGAEntityIndex);
       pMgaEnt = pPriv->ptr;
       pMgaEnt->refCount++;
#ifdef USEMGAHAL
       MGA_HAL(
       if(pMgaEnt->refCount == 1) {
	   CARD8 MiscCtlReg;
	  pMga->pBoard = xalloc(sizeof(CLIENTDATA) + MGAGetBOARDHANDLESize());
	  pMga->pClientStruct = xalloc(sizeof(CLIENTDATA));
	  pMga->pClientStruct->pMga = (MGAPtr) pMga;
	  
	  /* wrapping OpenLibrary to fix broken registers. MATROX: hint,hint.*/
          MiscCtlReg = inMGAdac(MGA1064_MISC_CTL);
	  MGAOpenLibrary(pMga->pBoard,pMga->pClientStruct,sizeof(CLIENTDATA));
	  outMGAdac(MGA1064_MISC_CTL,MiscCtlReg);
	  pMga->pMgaHwInfo = xalloc(sizeof(MGAHWINFO));
	  MGAGetHardwareInfo(pMga->pBoard,pMga->pMgaHwInfo);

	  /* Detecting for type of display */
	  if (pMga->pMgaHwInfo->ulCapsSecondOutput & MGAHWINFOCAPS_OUTPUT_TV) {
	  	xf86DrvMsg(pScrn->scrnIndex, X_PROBED, "TV detected\n");
	  }
	  if (pMga->pMgaHwInfo->ulCapsFirstOutput &
			MGAHWINFOCAPS_OUTPUT_DIGITAL) {
	  	xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
				"Digital Screen detected\n");
	  }
	  if (pMga->pMgaHwInfo->ulCapsSecondOutput &
			MGAHWINFOCAPS_OUTPUT_DIGITAL) {
	  	xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
				"Digital Screen detected\n");
	  }

	  /* Now copy these to the entitystructure */
	  pMgaEnt->pClientStruct = pMga->pClientStruct;
	  pMgaEnt->pBoard = pMga->pBoard;
	  pMgaEnt->pMgaHwInfo = pMga->pMgaHwInfo;
       } else { /* Ref count is 2 */
	  pMga->pClientStruct = pMgaEnt->pClientStruct;
	  pMga->pBoard = pMgaEnt->pBoard;
	  pMga->pMgaHwInfo = pMgaEnt->pMgaHwInfo;
       }
       );	/* MGA_HAL */
#endif
    } else {
#ifdef USEMGAHAL
	CARD8 MiscCtlReg;

	  MGA_HAL(
	  pMga->pBoard = xalloc(sizeof(CLIENTDATA) + MGAGetBOARDHANDLESize());
	  pMga->pClientStruct = xalloc(sizeof(CLIENTDATA));
	  pMga->pClientStruct->pMga = (MGAPtr) pMga;

	  MiscCtlReg = inMGAdac(MGA1064_MISC_CTL);
	  /* wrapping OpenLibrary to fix broken registers. MATROX: hint,hint.*/
	  MGAOpenLibrary(pMga->pBoard,pMga->pClientStruct,sizeof(CLIENTDATA));
	  outMGAdac(MGA1064_MISC_CTL,MiscCtlReg);
	  pMga->pMgaHwInfo = xalloc(sizeof(MGAHWINFO));
	  MGAGetHardwareInfo(pMga->pBoard,pMga->pMgaHwInfo);
	  );	/* MGA_HAL */
#endif
    }
    if (pMga->is_G200SE) {
	pScrn->videoRam = VRTemp;
	pMga->FbMapSize = FBTemp;
    }
#ifdef USEMGAHAL
    MGA_HAL(
	/* There is a problem in the HALlib: set soft reset bit */
	/* MATROX: hint, hint. */
	if (!pMga->Primary && !pMga->FBDev &&
	    (SUBSYS_ID(pMga->PciInfo) == PCI_CARD_MILL_G200_SG)) {
	    OUTREG(MGAREG_Reset, 1);
	    usleep(200);
	    OUTREG(MGAREG_Reset, 0);
	}
    );	/* MGA_HAL */
#endif

    /* Initialise the MMIO vgahw functions */
    vgaHWSetMmioFuncs(hwp, pMga->IOBase, PORT_OFFSET);
    vgaHWGetIOBase(hwp);

    /* Map the VGA memory when the primary video */
    if (!pMga->FBDev) {
	if (pMga->Primary) {
	    hwp->MapSize = 0x10000;
	    if (!vgaHWMapMem(pScrn))
		return FALSE;
	}

	/* Save the current state */
	MGASave(pScrn);
	/* Initialise the first mode */
	if (!MGAModeInit(pScrn, pScrn->currentMode))
	    return FALSE;
    }
    else {
	fbdevHWSave(pScrn);
	/* Disable VGA core, and leave memory access on */
#ifdef XSERVER_LIBPCIACCESS
	pci_device_cfg_write_bits(pMga->PciInfo, 0x00000100, 0x00000000, 
				  PCI_OPTION_REG);
#else
	pciSetBitsLong(pMga->PciTag, PCI_OPTION_REG, 0x100, 0x000);
#endif
	if (!fbdevHWModeInit(pScrn, pScrn->currentMode))
	    return FALSE;

        if (!pMga->SecondCrtc && pMga->HWCursor
	    && pMga->chip_attribs->hwcursor_1064) {
	    outMGAdac(MGA1064_CURSOR_BASE_ADR_LOW, pMga->FbCursorOffset >> 10);
	    outMGAdac(MGA1064_CURSOR_BASE_ADR_HI, pMga->FbCursorOffset >> 18);
	}

	MGAStormEngineInit(pScrn);
    }

    /* Darken the screen for aesthetic reasons and set the viewport 
     */
    (*pScreen->SaveScreen)(pScreen, SCREEN_SAVER_ON);
    pScrn->AdjustFrame(scrnIndex, pScrn->frameX0, pScrn->frameY0, 0);


    /*
     * The next step is to setup the screen's visuals, and initialise the
     * framebuffer code.  In cases where the framebuffer's default
     * choices for things like visual layouts and bits per RGB are OK,
     * this may be as simple as calling the framebuffer's ScreenInit()
     * function.  If not, the visuals will need to be setup before calling
     * a fb ScreenInit() function and fixed up after.
     *
     * For most PC hardware at depths >= 8, the defaults that cfb uses
     * are not appropriate.  In this driver, we fixup the visuals after.
     */

    /*
     * Reset the visual list.
     */
    miClearVisualTypes();

    /* Setup the visuals we support. */

    /* All MGA support DirectColor */
    if (pMga->SecondCrtc) {
	/* No DirectColor on the second head */
	if (!miSetVisualTypes(pScrn->depth, TrueColorMask, pScrn->rgbBits,
			      TrueColor))
		return FALSE;
	if (!miSetPixmapDepths ())
	    return FALSE;
    } else {
	if (!xf86SetDefaultVisual(pScrn, -1))
	    return FALSE;

	if (!miSetVisualTypes(pScrn->depth,
			      miGetDefaultVisualMask(pScrn->depth),
			      pScrn->rgbBits, pScrn->defaultVisual))
	    return FALSE;
	if (!miSetPixmapDepths ())
	    return FALSE;
    }

    /*
     * Call the framebuffer layer's ScreenInit function, and fill in other
     * pScreen fields.
     */

    width = pScrn->virtualX;
    height = pScrn->virtualY;
    displayWidth = pScrn->displayWidth;


    if(pMga->Rotate) {
	height = pScrn->virtualX;
	width = pScrn->virtualY;
    }

    if(pMga->ShadowFB) {
 	pMga->ShadowPitch = BitmapBytePad(pScrn->bitsPerPixel * width);
	pMga->ShadowPtr = xalloc(pMga->ShadowPitch * height);
	displayWidth = pMga->ShadowPitch / (pScrn->bitsPerPixel >> 3);
        FBStart = pMga->ShadowPtr;
    } else {
	pMga->ShadowPtr = NULL;
	FBStart = pMga->FbStart;
    }

#ifdef XF86DRI
     /*
      * Setup DRI after visuals have been established.
      *
      * The DRI does not work when textured video is enabled at this time.
      */
    if (pMga->is_G200SE) {
	xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
		   "Not supported by hardware, not initializing the DRI\n");
	pMga->directRenderingEnabled = FALSE;
	driFrom = X_PROBED;
    } else if (!xf86ReturnOptValBool(pMga->Options, OPTION_DRI, TRUE)) {
	driFrom = X_CONFIG;
    } else if ( pMga->NoAccel ) {
       xf86DrvMsg( pScrn->scrnIndex, X_ERROR,
		   "Acceleration disabled, not initializing the DRI\n" );
       pMga->directRenderingEnabled = FALSE;
       driFrom = X_CONFIG;
    }
    else if ( pMga->TexturedVideo == TRUE ) {
       xf86DrvMsg( pScrn->scrnIndex, X_ERROR,
		   "Textured video enabled, not initializing the DRI\n" );
       pMga->directRenderingEnabled = FALSE;
       driFrom = X_CONFIG;
    }
    else if (pMga->SecondCrtc == TRUE) {
       xf86DrvMsg( pScrn->scrnIndex, X_ERROR,
		   "Not initializing the DRI on the second head\n" );
       pMga->directRenderingEnabled = FALSE;
    }
    else if ((pMga->FbMapSize /
	       (width * (pScrn->bitsPerPixel >> 3))) <= height * 3) {
       xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
	  "Static buffer allocation failed, not initializing the DRI\n");
       xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
	  "Need at least %d kB video memory at this resolution, bit depth\n",
	  (3 * displayWidth * height * (pScrn->bitsPerPixel >> 3)) / 1024 );
       pMga->directRenderingEnabled = FALSE;
       driFrom = X_PROBED;
    }
    else {
       pMga->directRenderingEnabled = MGADRIScreenInit(pScreen);
    }
#endif


    if (!fbScreenInit(pScreen, FBStart, width, height, pScrn->xDpi,
		      pScrn->yDpi, displayWidth, pScrn->bitsPerPixel)) {
	return FALSE;
    }


    if (pScrn->bitsPerPixel > 8) {
        /* Fixup RGB ordering */
        visual = pScreen->visuals + pScreen->numVisuals;
        while (--visual >= pScreen->visuals) {
	    if ((visual->class | DynamicClass) == DirectColor) {
		visual->offsetRed = pScrn->offset.red;
		visual->offsetGreen = pScrn->offset.green;
		visual->offsetBlue = pScrn->offset.blue;
		visual->redMask = pScrn->mask.red;
		visual->greenMask = pScrn->mask.green;
		visual->blueMask = pScrn->mask.blue;
	    }
	}
    }

    /* must be after RGB ordering fixed */
    fbPictureInit (pScreen, 0, 0);

    xf86SetBlackWhitePixels(pScreen);

    pMga->BlockHandler = pScreen->BlockHandler;
    pScreen->BlockHandler = MGABlockHandler;

    if(!pMga->ShadowFB) /* hardware cursor needs to wrap this layer */
	MGADGAInit(pScreen);

    if (!pMga->NoAccel) {
#ifdef USE_EXA
	if (pMga->Exa)
	    mgaExaInit(pScreen);
	else
#endif
#ifdef USE_XAA
	    MGAStormAccelInit(pScreen);
#endif
    }

    miInitializeBackingStore(pScreen);
    xf86SetBackingStore(pScreen);
    xf86SetSilkenMouse(pScreen);

    /* Initialize software cursor.
	Must precede creation of the default colormap */
    miDCInitialize(pScreen, xf86GetPointerScreenFuncs());

    /* Initialize HW cursor layer.
	Must follow software cursor initialization*/
    if (pMga->HWCursor) {
	if(!MGAHWCursorInit(pScreen))
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		"Hardware cursor initialization failed\n");
    }
    if(pMga->MergedFB) {
        /* Rotate and MergedFB are mutiualy exclusive, so we can use this 
         * variable.
         */
        if (!pMga->PointerMoved) 
            pMga->PointerMoved = pScrn->PointerMoved;
        pScrn->PointerMoved = MGAMergePointerMoved; 
        
    }

    /* Initialise default colourmap */
    if (!miCreateDefColormap(pScreen))
	return FALSE;

    /* Initialize colormap layer.
	Must follow initialization of the default colormap */
    if (!pMga->SecondCrtc)
	f = CMAP_PALETTED_TRUECOLOR | CMAP_RELOAD_ON_MODE_SWITCH;
    else
	f = CMAP_RELOAD_ON_MODE_SWITCH;
    if(!xf86HandleColormaps(pScreen, 256, 8,
	pMga->FBDev ? fbdevHWLoadPaletteWeak() : MGAdac->LoadPalette,
	NULL, f))
	return FALSE;

    if(pMga->ShadowFB) {
	RefreshAreaFuncPtr refreshArea = MGARefreshArea;

	if(pMga->Rotate) {
	    if (!pMga->PointerMoved) {
	    pMga->PointerMoved = pScrn->PointerMoved;
	    pScrn->PointerMoved = MGAPointerMoved;
	    }

	   switch(pScrn->bitsPerPixel) {
	   case 8:	refreshArea = MGARefreshArea8;	break;
	   case 16:	refreshArea = MGARefreshArea16;	break;
	   case 24:	refreshArea = MGARefreshArea24;	break;
	   case 32:	refreshArea = MGARefreshArea32;	break;
	   }
	}

	ShadowFBInit(pScreen, refreshArea);
    }

    xf86DPMSInit(pScreen, mga_dpms_set_proc, 0);

    pScrn->memPhysBase = pMga->FbAddress;
    pScrn->fbOffset = pMga->YDstOrg * (pScrn->bitsPerPixel / 8);

    MGAInitVideo(pScreen);

#ifdef XF86DRI
    if (pMga->directRenderingEnabled) {
       /* Now that mi, drm and others have done their thing,
	* complete the DRI setup.
	*/
       pMga->directRenderingEnabled = MGADRIFinishScreenInit(pScreen);
    }
    if (pMga->directRenderingEnabled) {
        xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Direct rendering enabled\n");
    } else {
        xf86DrvMsg(pScrn->scrnIndex, X_WARNING, "Direct rendering disabled\n");
    }
    if (pMga->DualHeadEnabled && pMga->SecondCrtc == FALSE)
	pMgaEnt->directRenderingEnabled = pMga->directRenderingEnabled;
    pMga->haveQuiescense = 1;
#endif

    /* Wrap the current CloseScreen function */
    pMga->CloseScreen = pScreen->CloseScreen;
    pScreen->CloseScreen = MGACloseScreen;

    /* Report any unused options (only for the first generation) */
    if (serverGeneration == 1) {
	xf86ShowUnusedOptions(pScrn->scrnIndex, pScrn->options);
    }

    /* For the second head, work around display problem. */
    if (!pMga->MergedFB && pMga->SecondCrtc) {
	MGACrtc2FillStrip(pScrn);
    }

    /* Done */
    return TRUE;
}


/* Usually mandatory */
Bool
MGASwitchMode(int scrnIndex, DisplayModePtr mode, int flags)
{
#ifdef USEMGAHAL
    char sCmdIn[256];
    char sCmdOut[256];
    FILE* fdIn;
# ifdef MATROX_WRITEBACK
    FILE* fdOut;
# endif
#endif
 
    if  (mode->Flags & 0x80000000) {
#ifdef USEMGAHAL

# ifdef MATROX_WRITEBACK
#  define MWB(x) { x; }
#  define MWB_COND(x) x
# else
#  define MWB(x)
#  define MWB_COND(x) 1
# endif
	ScrnInfoPtr pScrn = xf86Screens[scrnIndex];

	MGA_HAL(
	fdIn = fopen("/tmp/mgaDriverIn", "rt");
	MWB(fdOut = fopen("/tmp/mgaDriverOut", "wt"))

	if(fdIn && MWB_COND(fdOut))
	{
 
	    fgets(sCmdIn, 255, fdIn);
 
	    if(sCmdIn)
	    {
 
		MGAExecuteEscCmd(xf86Screens[scrnIndex], sCmdIn, sCmdOut, mode);
 
		/* Remove file and close file descriptor */
		remove("/tmp/mgaDriverIn");
		fclose(fdIn);
		MWB(
		    /* Write output data to output file for
		       calling application */
		    fputs(sCmdOut, fdOut);
		    fclose(fdOut);
		    )
		mode->Flags &= 0x7FFFFFFF;
		return TRUE;
	    }
	    else
	    {
		mode->Flags &= 0x7FFFFFFF;
		return FALSE;
	    }
	}
	else
	{
	    mode->Flags &= 0x7FFFFFFF;
	    return FALSE;
	}
   )
#endif
 	return FALSE; 
    }   else
	return MGAModeInit(xf86Screens[scrnIndex], mode);
}
 
 /* Adjusts coordinates to match Panning granularity.
  * does nothing if the HALlib is not loaded
  */
void
MGAAdjustGranularity(ScrnInfoPtr pScrn, int* x, int* y)
{
#ifdef USEMGAHAL
    MGA_HAL(
	MGAPtr pMga = MGAPTR(pScrn);
	MGAPtr pMga2;
	int xg = 1;
	int yg = 1;
	if(pMga->pMgaModeInfo && pMga->pMgaModeInfo->ulPanXGran && pMga->pMgaModeInfo->ulPanYGran) {
	    xg = pMga->pMgaModeInfo->ulPanXGran;
	    yg = pMga->pMgaModeInfo->ulPanYGran;
	}
	if(pMga->pScrn2 && (pMga2 = MGAPTR(pMga->pScrn2)) ) {

	    if(pMga2->pMgaModeInfo && pMga2->pMgaModeInfo->ulPanXGran && pMga2->pMgaModeInfo->ulPanYGran) {
		xg = max(xg,pMga2->pMgaModeInfo->ulPanXGran);
		yg = max(yg,pMga2->pMgaModeInfo->ulPanYGran);
	    }
	}
	xg=16; /*ncoder: temporary */ 
	*x -= *x % xg;
	*y -= *y % yg;
	); 
#endif
}


/*
 * This function is used to initialize the Start Address - the first
 * displayed location in the video memory.
 */
/* Usually mandatory */
void
MGAAdjustFrame(int scrnIndex, int x, int y, int flags)
{
    ScrnInfoPtr pScrn;
    int Base, tmp, count;

    MGAFBLayout *pLayout;
    MGAPtr pMga;

    
    pScrn = xf86Screens[scrnIndex];
    pMga = MGAPTR(pScrn);
    pLayout = &pMga->CurrentLayout;
    
        /* wanted to improve panning granularity problems without risking
         * compatibility issues. Existing code looked hardware dependent.
         */
#ifdef USEMGAHAL
    MGA_HAL(
	    pMga->HALGranularityOffX = x;
	    pMga->HALGranularityOffY = y;
        MGAAdjustGranularity(pScrn,&x,&y);
	    pMga->HALGranularityOffX = pMga->HALGranularityOffX - x;
	    pMga->HALGranularityOffY = pMga->HALGranularityOffY - y;
        HALSetDisplayStart(pMga->pBoard,x,y,0);
    );
#endif
    MGA_NOT_HAL(
        if(pMga->ShowCache && y && pScrn->vtSema)
            y += pScrn->virtualY - 1;
            
        Base = (y * pLayout->displayWidth + x + pMga->YDstOrg) >>
                    (3 - pMga->BppShifts[(pLayout->bitsPerPixel >> 3) - 1]);

        if (pLayout->bitsPerPixel == 24) {
            if (pMga->Chipset == PCI_CHIP_MGAG400 
                || pMga->Chipset == PCI_CHIP_MGAG550)
                Base &= ~1;  /*1 Not sure why */

            Base *= 3;
        }

        /* find start of retrace */
        while (INREG8(0x1FDA) & 0x08);
        while (!(INREG8(0x1FDA) & 0x08));
        /* wait until we're past the start (fixseg.c in the DDK) */
        count = INREG(MGAREG_VCOUNT) + 2;
        while(INREG(MGAREG_VCOUNT) < count);

        OUTREG16(MGAREG_CRTC_INDEX, (Base & 0x00FF00) | 0x0C);
        OUTREG16(MGAREG_CRTC_INDEX, ((Base & 0x0000FF) << 8) | 0x0D);
        OUTREG8(MGAREG_CRTCEXT_INDEX, 0x00);
        tmp = INREG8(MGAREG_CRTCEXT_DATA);
        OUTREG8(MGAREG_CRTCEXT_DATA, (tmp & 0xF0) | ((Base & 0x0F0000) >> 16));
    );

}

void
MGAAdjustFrameCrtc2(int scrnIndex, int x, int y, int flags)
{
    ScrnInfoPtr pScrn;
    int Base;
    MGAFBLayout *pLayout;
    MGAPtr pMga;

    pScrn = xf86Screens[scrnIndex];
    pMga = MGAPTR(pScrn);
    pLayout = &pMga->CurrentLayout;
#ifdef USEMGAHAL
    MGA_HAL(
        MGAAdjustGranularity(pScrn,&x,&y);
        HALSetDisplayStart(pMga->pBoard,x,y,1);
    );
#endif
    MGA_NOT_HAL(
        if(pMga->ShowCache && y && pScrn->vtSema)
            y += pScrn->virtualY - 1;

        /* 3-85 c2offset
            * 3-93 c2startadd0
            * 3-96 c2vcount
            */

        Base = (y * pLayout->displayWidth + x) * pLayout->bitsPerPixel >> 3;
        Base += pMga->DstOrg;
        Base &= 0x01ffffc0;
        OUTREG(MGAREG_C2STARTADD0, Base);
    );
}

/*
 * This is called when VT switching back to the X server.  Its job is
 * to reinitialise the video mode.
 *
 * We may wish to unmap video/MMIO memory too.
 */

/* Mandatory */
static Bool
MGAEnterVT(int scrnIndex, int flags)
{
    ScrnInfoPtr pScrn = xf86Screens[scrnIndex];
    MGAPtr pMga;

    pMga = MGAPTR(pScrn);

#ifdef XF86DRI
    if (pMga->directRenderingEnabled) {
	if (pMga->irq) {
	    /* Need to make sure interrupts are enabled */
	    OUTREG(MGAREG_IEN, pMga->reg_ien);
	}
        DRIUnlock(screenInfo.screens[scrnIndex]);
    }
#endif

    if (!MGAModeInit(pScrn, pScrn->currentMode))
	return FALSE;
    pScrn->AdjustFrame(scrnIndex, pScrn->frameX0, pScrn->frameY0, 0);

    /* For the second head, work around display problem. */
   if (pMga->SecondCrtc) {
	MGACrtc2FillStrip(pScrn);
    }

    return TRUE;
}

static Bool
MGAEnterVTFBDev(int scrnIndex, int flags)
{
    ScrnInfoPtr pScrn = xf86Screens[scrnIndex];
#ifdef XF86DRI
    ScreenPtr pScreen;
    MGAPtr pMga;

    pMga = MGAPTR(pScrn);
    if (pMga->directRenderingEnabled) {
        pScreen = screenInfo.screens[scrnIndex];
        DRIUnlock(pScreen);
    }
#endif

    fbdevHWEnterVT(scrnIndex,flags);
    MGAStormEngineInit(pScrn);
    return TRUE;
}

#define RESTORE_TEXTMODE_ON_DVI(x)                      \
    if (MGAISGx50(x) &&                                 \
       (ISDIGITAL1(x) || ISDIGITAL2(x))) {              \
        /* Reset DUALDVI register */                    \
        outMGAdac(MGA1064_DVI_PIPE_CTL, 0x0);           \
        /* Set Panel mode between 20 and 54 MHz */      \
        outMGAdac(MGA1064_PAN_CTL, 0x7);                \
    }
    

/*
 * This is called when VT switching away from the X server.  Its job is
 * to restore the previous (text) mode.
 *
 * We may wish to remap video/MMIO memory too.
 */

/* Mandatory */
static void
MGALeaveVT(int scrnIndex, int flags)
{
    ScrnInfoPtr pScrn = xf86Screens[scrnIndex];
    vgaHWPtr hwp = VGAHWPTR(pScrn);
#ifdef XF86DRI
    MGAPtr pMga = MGAPTR(pScrn);
    ScreenPtr pScreen;
#endif

    MGARestore(pScrn);
    vgaHWLock(hwp);

    if (xf86IsPc98())
	outb(0xfac, 0x00);
#ifdef XF86DRI
    if (pMga->directRenderingEnabled) {
        pScreen = screenInfo.screens[scrnIndex];
        DRILock(pScreen, 0);
    }
#endif
#ifdef USEMGAHAL
    MGA_HAL( RESTORE_TEXTMODE_ON_DVI(pMga); );
#endif
}


/*
 * This is called at the end of each server generation.  It restores the
 * original (text) mode.  It should also unmap the video memory, and free
 * any per-generation data allocated by the driver.  It should finish
 * by unwrapping and calling the saved CloseScreen function.
 */

/* Mandatory */
static Bool
MGACloseScreen(int scrnIndex, ScreenPtr pScreen)
{
    ScrnInfoPtr pScrn = xf86Screens[scrnIndex];
    vgaHWPtr hwp = VGAHWPTR(pScrn);
    MGAPtr pMga = MGAPTR(pScrn);
    MGAEntPtr pMgaEnt = NULL;

#ifdef USEMGAHAL    
    MGA_HAL( RESTORE_TEXTMODE_ON_DVI(pMga); );
#endif
    if (pMga->MergedFB)
         MGACloseScreenMerged(scrnIndex, pScreen);

    if (pScrn->vtSema) {
	if (pMga->FBDev) {
	    fbdevHWRestore(pScrn);
	    MGAUnmapMem(pScrn);
        } else {
	    MGARestore(pScrn);
	    vgaHWLock(hwp);
	    MGAUnmapMem(pScrn);
	    vgaHWUnmapMem(pScrn);
	}
    }
#ifdef XF86DRI
   if (pMga->directRenderingEnabled) {
       MGADRICloseScreen(pScreen);
       pMga->directRenderingEnabled=FALSE;
   }
#endif

   if (pMga->DualHeadEnabled) {
       DevUnion *pPriv;
       pPriv = xf86GetEntityPrivate(pScrn->entityList[0], MGAEntityIndex);
       pMgaEnt = pPriv->ptr;
       pMgaEnt->refCount--;
   }

#ifdef USEMGAHAL
   MGA_HAL(
   if(pMga->DualHeadEnabled) {
      if(pMgaEnt->refCount == 0) {
	 /* Both boards have closed there screen */
	 MGACloseLibrary(pMga->pBoard);

	 if (pMga->pBoard)
	   xfree(pMga->pBoard);
	 if (pMga->pClientStruct)
	   xfree(pMga->pClientStruct);
	 if (pMga->pMgaModeInfo)
	   xfree(pMga->pMgaModeInfo);
	 if (pMga->pMgaHwInfo)
	   xfree(pMga->pMgaHwInfo);
      }
   } else {
      MGACloseLibrary(pMga->pBoard);

      if (pMga->pBoard)
	xfree(pMga->pBoard);
      if (pMga->pClientStruct)
	xfree(pMga->pClientStruct);
      if (pMga->pMgaModeInfo)
	xfree(pMga->pMgaModeInfo);
      if (pMga->pMgaHwInfo)
	xfree(pMga->pMgaHwInfo);
   }
   );	/* MGA_HAL */
#endif

#ifdef USE_XAA
    if (pMga->AccelInfoRec)
	XAADestroyInfoRec(pMga->AccelInfoRec);
#endif
#ifdef USE_EXA
    if (pMga->ExaDriver) {
	exaDriverFini(pScreen);
	xfree(pMga->ExaDriver);
    }
#endif
    if (pMga->CursorInfoRec)
    	xf86DestroyCursorInfoRec(pMga->CursorInfoRec);
    if (pMga->ShadowPtr)
	xfree(pMga->ShadowPtr);
    if (pMga->DGAModes)
	xfree(pMga->DGAModes);
    if (pMga->adaptor)
	xfree(pMga->adaptor);
    if (pMga->portPrivate)
	xfree(pMga->portPrivate);
    if (pMga->ScratchBuffer)
	xfree(pMga->ScratchBuffer);

    pScrn->vtSema = FALSE;

    if (xf86IsPc98())
	outb(0xfac, 0x00);

    xf86ClearPrimInitDone(pScrn->entityList[0]);

    if(pMga->BlockHandler)
	pScreen->BlockHandler = pMga->BlockHandler;

    pScreen->CloseScreen = pMga->CloseScreen;

    return (*pScreen->CloseScreen)(scrnIndex, pScreen);
}


/* Free up any persistent data structures */

/* Optional */
static void
MGAFreeScreen(int scrnIndex, int flags)
{
	
    /*
     * This only gets called when a screen is being deleted.  It does not
     * get called routinely at the end of a server generation.
     */
    if (xf86LoaderCheckSymbol("vgaHWFreeHWRec"))
	vgaHWFreeHWRec(xf86Screens[scrnIndex]);
    MGAFreeRec(xf86Screens[scrnIndex]);

}

#ifndef HAVE_XF86MODEBANDWIDTH

#define MODE_BANDWIDTH MODE_BAD

/** Calculates the memory bandwidth (in MiB/sec) of a mode. */
static unsigned int
xf86ModeBandwidth(DisplayModePtr mode, int depth)
{
    float a_active, a_total, active_percent, pixels_per_second;
    int bytes_per_pixel = (depth + 7) / 8;

    if (!mode->HTotal || !mode->VTotal || !mode->Clock)
	return 0;

    a_active = mode->HDisplay * mode->VDisplay;
    a_total = mode->HTotal * mode->VTotal;
    active_percent = a_active / a_total;
    pixels_per_second = active_percent * mode->Clock * 1000.0;

    return (unsigned int)(pixels_per_second * bytes_per_pixel / (1024 * 1024));
}
#endif

/* Checks if a mode is suitable for the selected chipset. */

/* Optional */
static ModeStatus
MGAValidMode(int scrnIndex, DisplayModePtr mode, Bool verbose, int flags)
{
    int lace;
    ScrnInfoPtr pScrn = xf86Screens[scrnIndex];
    MGAPtr pMga = MGAPTR(pScrn);

    if (pMga->Chipset == PCI_CHIP_MGAG200_SE_A_PCI) {
	if (mode->HDisplay > 1600)
	    return MODE_VIRTUAL_X;
	if (mode->VDisplay > 1200)
	    return MODE_VIRTUAL_Y;
	if (pMga->reg_1e24 == 0x01 &&
	    xf86ModeBandwidth(mode, pScrn->bitsPerPixel) > 256)
	    return MODE_BANDWIDTH;
    } else if (pMga->is_G200WB){
	if (pMga->KVM && mode->HDisplay > 1280)
	    return MODE_VIRTUAL_X;
	if (pMga->KVM && mode->VDisplay > 1024)
	    return MODE_VIRTUAL_Y;
	if (xf86ModeBandwidth(mode, pScrn->bitsPerPixel) > 315)
	    return MODE_BANDWIDTH;
    } else if (pMga->is_G200EV
	       && (xf86ModeBandwidth(mode, pScrn->bitsPerPixel) > 327)) {
	return MODE_BANDWIDTH;
    }

    lace = 1 + ((mode->Flags & V_INTERLACE) != 0);

    if ((mode->CrtcHDisplay <= 2048) &&
	(mode->CrtcHSyncStart <= 4096) &&
	(mode->CrtcHSyncEnd <= 4096) &&
	(mode->CrtcHTotal <= 4096) &&
	(mode->CrtcVDisplay <= 2048 * lace) &&
	(mode->CrtcVSyncStart <= 4096 * lace) &&
	(mode->CrtcVSyncEnd <= 4096 * lace) &&
	(mode->CrtcVTotal <= 4096 * lace)) {

	/* Can't have horizontal panning for second head of G400 */
	if (pMga->SecondCrtc) {
	    if (flags == MODECHECK_FINAL) {
		if (pMga->allowedWidth == 0)
		    pMga->allowedWidth = pScrn->virtualX;
		if (mode->HDisplay != pMga->allowedWidth)
		    return(MODE_ONE_WIDTH);
	    }
	}

	return(MODE_OK);
    } else {
	return(MODE_BAD);
    }
}


/*
 * This routine is required but since we can't easily blank the
 * second display without risking powering off the monitor, return
 * FALSE and let the X server do something generic.
 */
static Bool
MGASaveScreenCrtc2(ScreenPtr pScreen, int mode)
{
    return FALSE;
}

/* Do screen blanking */

static Bool
MGASaveScreen(ScreenPtr pScreen, int mode)
{
    return vgaHWSaveScreen(pScreen, mode);
}


/*
 * MGADisplayPowerManagementSet --
 *
 * Sets VESA Display Power Management Signaling (DPMS) Mode.
 *
 * XXX This needs fixing for sync-on-green!
 */
void
MGADisplayPowerManagementSet(ScrnInfoPtr pScrn, int PowerManagementMode,
			     int flags)
{
	MGAPtr pMga = MGAPTR(pScrn);
	unsigned char seq1 = 0, crtcext1 = 0;

	switch (PowerManagementMode)
	{
	case DPMSModeOn:
	    /* Screen: On; HSync: On, VSync: On */
	    seq1 = 0x00;
	    crtcext1 = 0x00;
	    break;
	case DPMSModeStandby:
	    /* Screen: Off; HSync: Off, VSync: On */
	    seq1 = 0x20;
	    crtcext1 = 0x10;
	    break;
	case DPMSModeSuspend:
	    /* Screen: Off; HSync: On, VSync: Off */
	    seq1 = 0x20;
	    crtcext1 = 0x20;
	    break;
	case DPMSModeOff:
	    /* Screen: Off; HSync: Off, VSync: Off */
	    seq1 = 0x20;
	    crtcext1 = 0x30;
	    break;
	}

	/* XXX Prefer an implementation that doesn't depend on VGA specifics */
	OUTREG8(MGAREG_SEQ_INDEX, 0x01);	/* Select SEQ1 */
	seq1 |= INREG8(MGAREG_SEQ_DATA) & ~0x20;
	MGAWAITVSYNC();
	MGAWAITBUSY();
	OUTREG8(MGAREG_SEQ_DATA, seq1);
	usleep(20000);
	OUTREG8(MGAREG_CRTCEXT_INDEX, 0x01);	/* Select CRTCEXT1 */
	crtcext1 |= INREG8(MGAREG_CRTCEXT_DATA) & ~0x30;
	OUTREG8(MGAREG_CRTCEXT_DATA, crtcext1);
}


void
MGADisplayPowerManagementSetCrtc2(ScrnInfoPtr pScrn, int PowerManagementMode,
				  int flags)
{
	MGAPtr pMga = MGAPTR(pScrn);
	CARD32 val = INREG(MGAREG_C2CTL);

	if (PowerManagementMode==DPMSModeOn) {
		/* Enable CRTC2 */
		val |= MGAREG_C2CTL_C2_EN;
		val &= ~MGAREG_C2CTL_PIXCLKDIS_DISABLE;
		OUTREG(MGAREG_C2CTL, val);
		/* Restore normal MAVEN values */
		if (pMga->Maven) {
			/* if TV MODE -- for later implementation
				MAVW(MONEN, 0xb3);
				MAVW(MONSET, 0x20);
				MAVW(OUTMODE, 0x08);    output: SVideo/Composite
			        MAVW(STABLE, 0x02);             makes picture stable?
				fixme? linux uses 0x14...
				MAVW(TEST, (MAVR(TEST) & 0x10));
			 
			 */
			/* else monitor mode */

			xf86I2CWriteByte(pMga->Maven, MGAMAV_MONEN, 0xb2);
			/* must be set to this in monitor mode */
			xf86I2CWriteByte(pMga->Maven, MGAMAV_MONSET, 0x20);
			/* output: monitor mode */
			xf86I2CWriteByte(pMga->Maven, MGAMAV_OUTMODE, 0x03);
			/* makes picture stable? */
			xf86I2CWriteByte(pMga->Maven, MGAMAV_STABLE, 0x22);
			/* turn off test signal */
			xf86I2CWriteByte(pMga->Maven, MGAMAV_TEST, 0x00);
		}
	}
	else {
		/* Disable CRTC2 video */
		val |= MGAREG_C2CTL_PIXCLKDIS_DISABLE;
		val &= ~MGAREG_C2CTL_C2_EN;
		OUTREG(MGAREG_C2CTL, val);

		/* Disable MAVEN display */
		if (pMga->Maven) {
		/* In order to blank the 2nd display, we must set some MAVEN registers.
		 * It seems that not always the same values work on different hardware so
		 * we try a few different (possibly redundant) ones. */
			/* xf86I2CWriteByte(pMga->Maven, MGAMAV_STABLE, 0x6a); */
			/* xf86I2CWriteByte(pMga->Maven, MGAMAV_TEST, 0x03); */
			/* xf86I2CWriteByte(pMga->Maven, MGAMAV_TEST, 0x10); */
			xf86I2CWriteByte(pMga->Maven, MGAMAV_OUTMODE, 0x80);
		}

	}
}


static void
MGABlockHandler (
    int i,
    pointer     blockData,
    pointer     pTimeout,
    pointer     pReadmask
){
    ScreenPtr      pScreen = screenInfo.screens[i];
    ScrnInfoPtr    pScrn = xf86Screens[i];
    MGAPtr         pMga = MGAPTR(pScrn);

    if(pMga->PaletteLoadCallback)
	(*pMga->PaletteLoadCallback)(pScrn);

    pScreen->BlockHandler = pMga->BlockHandler;
    (*pScreen->BlockHandler) (i, blockData, pTimeout, pReadmask);
    pScreen->BlockHandler = MGABlockHandler;

    if(pMga->VideoTimerCallback) {
	UpdateCurrentTime();
	(*pMga->VideoTimerCallback)(pScrn, currentTime.milliseconds);
    }

    if(pMga->RenderCallback)
	(*pMga->RenderCallback)(pScrn);
}

#if defined (EXTRADEBUG)
/*
 * some functions to track input/output in the server
 */

CARD8
MGAdbg_inreg8(ScrnInfoPtr pScrn,int addr,int verbose, char* func)
{
    CARD8 ret;

    ret = MMIO_IN8(MGAPTR(pScrn)->IOBase,addr);
    if(verbose)
	xf86DrvMsg(pScrn->scrnIndex, X_INFO,
			"inreg8 : %s: 0x%8x = 0x%x\n",func, addr,ret);
    return ret;
}

CARD16
MGAdbg_inreg16(ScrnInfoPtr pScrn,int addr,int verbose, char* func)
{
    CARD16 ret;

    ret = MMIO_IN16(MGAPTR(pScrn)->IOBase,addr);
    if(verbose)
	xf86DrvMsg(pScrn->scrnIndex, X_INFO,
			"inreg16: %s: 0x%8x = 0x%x\n",func, addr,ret);
    return ret;
}

CARD32
MGAdbg_inreg32(ScrnInfoPtr pScrn,int addr,int verbose, char* func)
{
    CARD32 ret;

    ret = MMIO_IN32(MGAPTR(pScrn)->IOBase,addr);
    if(verbose)
	xf86DrvMsg(pScrn->scrnIndex, X_INFO,
			"inreg32: %s: 0x%8x = 0x%x\n",func, addr,ret);
    return ret;
}

void
MGAdbg_outreg8(ScrnInfoPtr pScrn,int addr,int val, char* func)
{
    CARD8 ret;

#if 0
    if( addr = MGAREG_CRTCEXT_DATA )
    	return;
#endif
    if( addr != 0x3c00 ) {
	ret = MGAdbg_inreg8(pScrn,addr,0,func);
	xf86DrvMsg(pScrn->scrnIndex, X_INFO,
			"outreg8 : %s: 0x%8x = 0x%x was 0x%x\n",
			func,addr,val,ret);
    }
    else {
	xf86DrvMsg(pScrn->scrnIndex, X_INFO, "outreg8 : %s: index 0x%x\n",
	func,val);
    }
    MMIO_OUT8(MGAPTR(pScrn)->IOBase,addr,val);
}

void
MGAdbg_outreg16(ScrnInfoPtr pScrn,int addr,int val, char* func)
{
    CARD16 ret;

#if 0
    if (addr == MGAREG_CRTCEXT_INDEX)
    	return;
#endif
    ret = MGAdbg_inreg16(pScrn,addr,0, func);
    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
			"outreg16: %s: 0x%8x = 0x%x was 0x%x\n",
			func,addr,val,ret);
    MMIO_OUT16(MGAPTR(pScrn)->IOBase,addr,val);
}

void
MGAdbg_outreg32(ScrnInfoPtr pScrn,int addr,int val, char* func)
{
    CARD32 ret;

    if (((addr & 0xff00) == 0x1c00)
    	&& (addr != 0x1c04)
/*    	&& (addr != 0x1c1c) */
    	&& (addr != 0x1c20)
    	&& (addr != 0x1c24)
    	&& (addr != 0x1c80)
    	&& (addr != 0x1c8c)
    	&& (addr != 0x1c94)
    	&& (addr != 0x1c98)
    	&& (addr != 0x1c9c)
	 ) {
	 xf86DrvMsg(pScrn->scrnIndex, X_INFO, "%s: refused address 0x%x\n",
			func,addr);
    	return;
    }
    ret = MGAdbg_inreg32(pScrn,addr,0, func);
    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
			"outreg32: %s: 0x%8x = 0x%x was 0x%x\n",
			func,addr,val,ret);
    MMIO_OUT32(MGAPTR(pScrn)->IOBase,addr,val);
}
#endif /* DEBUG */

static void
MGAG100BlackMagic(ScrnInfoPtr pScrn)
{
    MGAPtr pMga = MGAPTR(pScrn);

    OUTREG(MGAREG_PLNWT, ~(CARD32)0x0);
    /* reset memory */
    OUTREG(MGAREG_MACCESS, 1<<15);
    usleep(10);
}

