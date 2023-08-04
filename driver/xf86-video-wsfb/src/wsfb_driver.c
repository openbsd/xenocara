/* $OpenBSD: wsfb_driver.c,v 1.45 2023/08/04 23:49:45 aoyama Exp $ */
/*
 * Copyright © 2001-2012 Matthieu Herrb
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *    - Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *    - Redistributions in binary form must reproduce the above
 *      copyright notice, this list of conditions and the following
 *      disclaimer in the documentation and/or other materials provided
 *      with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

/*
 * Based on fbdev.c written by:
 *
 * Authors:  Alan Hourihane, <alanh@fairlite.demon.co.uk>
 *	     Michel Dänzer, <michdaen@iiic.ethz.ch>
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <sys/utsname.h>
#include <dev/wscons/wsconsio.h>

/* All drivers need this. */
#include "xf86.h"
#include "xf86_OSproc.h"

#include "mipointer.h"
#include "micmap.h"
#include "colormapst.h"
#include "xf86cmap.h"
#include "shadow.h"
#include "dgaproc.h"

#include <xf86Priv.h>

/* For visuals */
#include "fb.h"

#if GET_ABI_MAJOR(ABI_VIDEODRV_VERSION) < 6
#include "xf86Resources.h"
#include "xf86RAC.h"
#endif

#ifdef XvExtension
#include "xf86xv.h"
#endif

#include "xorgVersion.h"
#include "compat-api.h"

#ifdef X_PRIVSEP
extern int priv_open_device(const char *);
#else
#define priv_open_device(n)    open(n,O_RDWR|O_NONBLOCK|O_EXCL)
#endif

#if defined(__NetBSD__)
#define WSFB_DEFAULT_DEV "/dev/ttyE0"
#else
#define WSFB_DEFAULT_DEV "/dev/ttyC0"
#endif

#define DEBUG 0

#if DEBUG
# define TRACE_ENTER(str)       ErrorF("wsfb: " str " %d\n",pScrn->scrnIndex)
# define TRACE_EXIT(str)        ErrorF("wsfb: " str " done\n")
# define TRACE(str)             ErrorF("wsfb trace: " str "\n")
#else
# define TRACE_ENTER(str)
# define TRACE_EXIT(str)
# define TRACE(str)
#endif

/* Prototypes */
static pointer WsfbSetup(pointer, pointer, int *, int *);
static Bool WsfbGetRec(ScrnInfoPtr);
static void WsfbFreeRec(ScrnInfoPtr);
static const OptionInfoRec * WsfbAvailableOptions(int, int);
static void WsfbIdentify(int);
static Bool WsfbProbe(DriverPtr, int);
static Bool WsfbPreInit(ScrnInfoPtr, int);
static Bool WsfbScreenInit(SCREEN_INIT_ARGS_DECL);
static Bool WsfbCloseScreen(CLOSE_SCREEN_ARGS_DECL);
static void *WsfbWindowLinear(ScreenPtr, CARD32, CARD32, int, CARD32 *,
			      void *);
static void *WsfbWindowAfb(ScreenPtr, CARD32, CARD32, int, CARD32 *,
			      void *);
static void WsfbPointerMoved(SCRN_ARG_TYPE, int, int);
static Bool WsfbEnterVT(VT_FUNC_ARGS_DECL);
static void WsfbLeaveVT(VT_FUNC_ARGS_DECL);
static Bool WsfbSwitchMode(SWITCH_MODE_ARGS_DECL);
static int WsfbValidMode(SCRN_ARG_TYPE, DisplayModePtr, Bool, int);
static void WsfbLoadPalette(ScrnInfoPtr, int, int *, LOCO *, VisualPtr);
static Bool WsfbSaveScreen(ScreenPtr, int);
static void WsfbSave(ScrnInfoPtr);
static void WsfbRestore(ScrnInfoPtr);

/* DGA stuff */
#ifdef XFreeXDGA
static Bool WsfbDGAOpenFramebuffer(ScrnInfoPtr, char **, unsigned char **,
				   int *, int *, int *);
static Bool WsfbDGASetMode(ScrnInfoPtr, DGAModePtr);
static void WsfbDGASetViewport(ScrnInfoPtr, int, int, int);
static Bool WsfbDGAInit(ScrnInfoPtr, ScreenPtr);
#endif
static Bool WsfbDriverFunc(ScrnInfoPtr pScrn, xorgDriverFuncOp op,
				pointer ptr);

/* Helper functions */
static int wsfb_open(const char *);
static pointer wsfb_mmap(size_t, off_t, int);

enum { WSFB_ROTATE_NONE = 0,
       WSFB_ROTATE_CCW = 90,
       WSFB_ROTATE_UD = 180,
       WSFB_ROTATE_CW = 270
};

/*
 * This is intentionally screen-independent.
 * It indicates the binding choice made in the first PreInit.
 */
static int pix24bpp = 0;

#define WSFB_VERSION		4000
#define WSFB_NAME		"wsfb"
#define WSFB_DRIVER_NAME	"wsfb"

_X_EXPORT DriverRec WSFB = {
	WSFB_VERSION,
	(char *)WSFB_DRIVER_NAME,
	WsfbIdentify,
	WsfbProbe,
	WsfbAvailableOptions,
	NULL,
	0,
	WsfbDriverFunc
};

/* Supported "chipsets" */
static SymTabRec WsfbChipsets[] = {
	{ 0, "wsfb" },
	{ -1, NULL }
};

/* Supported options */
typedef enum {
	OPTION_SHADOW_FB,
	OPTION_ROTATE
} WsfbOpts;

static const OptionInfoRec WsfbOptions[] = {
	{ OPTION_SHADOW_FB, "ShadowFB", OPTV_BOOLEAN, {0}, FALSE},
	{ OPTION_ROTATE, "Rotate", OPTV_STRING, {0}, FALSE},
	{ -1, NULL, OPTV_NONE, {0}, FALSE}
};

static XF86ModuleVersionInfo WsfbVersRec = {
	"wsfb",
	MODULEVENDORSTRING,
	MODINFOSTRING1,
	MODINFOSTRING2,
	XORG_VERSION_CURRENT,
	PACKAGE_VERSION_MAJOR,
	PACKAGE_VERSION_MINOR,
	PACKAGE_VERSION_PATCHLEVEL,
	ABI_CLASS_VIDEODRV,
	ABI_VIDEODRV_VERSION,
	NULL,
	{0, 0, 0, 0}
};

_X_EXPORT XF86ModuleData wsfbModuleData = { &WsfbVersRec, WsfbSetup, NULL };

static pointer
WsfbSetup(pointer module, pointer opts, int *errmaj, int *errmin)
{
	static Bool setupDone = FALSE;
	struct utsname name;

	/* Check that we're being loaded on a OpenBSD or NetBSD system. */
	if (uname(&name) == -1 || (strcmp(name.sysname, "OpenBSD") != 0 &&
			strcmp(name.sysname, "NetBSD") != 0)) {
		if (errmaj)
			*errmaj = LDR_BADOS;
		if (errmin)
			*errmin = 0;
		return NULL;
	}
	if (!setupDone) {
		setupDone = TRUE;
		xf86AddDriver(&WSFB, module, HaveDriverFuncs);
		return (pointer)1;
	} else {
		if (errmaj != NULL)
			*errmaj = LDR_ONCEONLY;
		return NULL;
	}
}

/* Private data */
typedef struct {
	int			fd; /* File descriptor of open device. */
	struct wsdisplay_fbinfo info; /* Frame buffer characteristics. */
	int			linebytes; /* Number of bytes per row. */
	int			wstype; /* wsdisplay type. */ 
	unsigned char*		fbstart;
	unsigned char*		fbmem;
	size_t			fbmem_len;
	int			rotate;
	Bool			shadowFB;
	Bool			planarAfb;
	void *			shadow;
	CloseScreenProcPtr	CloseScreen;
	CreateScreenResourcesProcPtr CreateScreenResources;
	void			(*PointerMoved)(SCRN_ARG_TYPE, int, int);
	EntityInfoPtr		pEnt;
	struct wsdisplay_cmap	saved_cmap;

#ifdef XFreeXDGA
	/* DGA info */
	DGAModePtr		pDGAMode;
	int			nDGAMode;
#endif
	OptionInfoPtr		Options;
} WsfbRec, *WsfbPtr;

#define WSFBPTR(p) ((WsfbPtr)((p)->driverPrivate))

static Bool
WsfbGetRec(ScrnInfoPtr pScrn)
{

	if (pScrn->driverPrivate != NULL)
		return TRUE;

	pScrn->driverPrivate = xnfcalloc(sizeof(WsfbRec), 1);
	return TRUE;
}

static void
WsfbFreeRec(ScrnInfoPtr pScrn)
{

	if (pScrn->driverPrivate == NULL)
		return;
	free(pScrn->driverPrivate);
	pScrn->driverPrivate = NULL;
}

static const OptionInfoRec *
WsfbAvailableOptions(int chipid, int busid)
{
	return WsfbOptions;
}

static void
WsfbIdentify(int flags)
{
	xf86PrintChipsets(WSFB_NAME, "driver for wsdisplay framebuffer",
			  WsfbChipsets);
}

/* Open the framebuffer device. */
static int
wsfb_open(const char *dev)
{
	int fd = -1;

	/* Try argument from xorg.conf first. */
	if (dev == NULL || ((fd = priv_open_device(dev)) == -1)) {
		/* Second: environment variable. */
		dev = getenv("XDEVICE");
		if (dev == NULL || ((fd = priv_open_device(dev)) == -1)) {
			if (xf86Info.consoleFd > 0) {
				fd = xf86Info.consoleFd;
			} else {
				/* Last try: default device. */
				dev = WSFB_DEFAULT_DEV;
				if ((fd = priv_open_device(dev)) == -1) {
					return -1;
				}
			}
		}
	}
	return fd;
}

/* Map the framebuffer's memory. */
static pointer
wsfb_mmap(size_t len, off_t off, int fd)
{
	int pagemask, mapsize;
	caddr_t addr;
	pointer mapaddr;

	pagemask = getpagesize() - 1;
	mapsize = ((int) len + pagemask) & ~pagemask;
	addr = 0;

	/*
	 * Try and make it private first, that way once we get it, an
	 * interloper, e.g. another server, can't get this frame buffer,
	 * and if another server already has it, this one won't.
	 */
	mapaddr = (pointer) mmap(addr, mapsize,
				 PROT_READ | PROT_WRITE, MAP_SHARED,
				 fd, off);
	if (mapaddr == (pointer) -1) {
		mapaddr = NULL;
	}
#if DEBUG
	ErrorF("mmap returns: addr %p len 0x%x\n", mapaddr, mapsize);
#endif
	return mapaddr;
}

static Bool
WsfbProbe(DriverPtr drv, int flags)
{
	int i, fd, entity;
	GDevPtr *devSections;
	int numDevSections;
	const char *dev;
	Bool foundScreen = FALSE;

	TRACE("probe start");

	/* For now, just bail out for PROBE_DETECT. */
	if (flags & PROBE_DETECT)
		return FALSE;

	if ((numDevSections = xf86MatchDevice(WSFB_DRIVER_NAME,
					      &devSections)) <= 0)
		return FALSE;

	for (i = 0; i < numDevSections; i++) {
		ScrnInfoPtr pScrn = NULL;

		dev = xf86FindOptionValue(devSections[i]->options, "device");
		if ((fd = wsfb_open(dev)) >= 0) {
			entity = xf86ClaimFbSlot(drv, 0, devSections[i], TRUE);
			pScrn = xf86ConfigFbEntity(NULL,0,entity,
						   NULL,NULL,NULL,NULL);
			if (pScrn != NULL) {
				foundScreen = TRUE;
				pScrn->driverVersion = WSFB_VERSION;
				pScrn->driverName = (char *)WSFB_DRIVER_NAME;
				pScrn->name = (char *)WSFB_NAME;
				pScrn->Probe = WsfbProbe;
				pScrn->PreInit = WsfbPreInit;
				pScrn->ScreenInit = WsfbScreenInit;
				pScrn->SwitchMode = WsfbSwitchMode;
				pScrn->AdjustFrame = NULL;
				pScrn->EnterVT = WsfbEnterVT;
				pScrn->LeaveVT = WsfbLeaveVT;
				pScrn->ValidMode = WsfbValidMode;

				xf86DrvMsg(pScrn->scrnIndex, X_INFO,
				    "using %s\n", dev != NULL ? dev :
				    "default device");
			}
		}
	}
	free(devSections);
	TRACE("probe done");
	return foundScreen;
}

static Bool
WsfbPreInit(ScrnInfoPtr pScrn, int flags)
{
	WsfbPtr fPtr;
	int defaultDepth, depths, flags24;
	const char *dev;
	char *s;
	Gamma zeros = {0.0, 0.0, 0.0};
	DisplayModePtr mode;

	if (flags & PROBE_DETECT) return FALSE;

	TRACE_ENTER("PreInit");

	if (pScrn->numEntities != 1) return FALSE;

	pScrn->monitor = pScrn->confScreen->monitor;

	WsfbGetRec(pScrn);
	fPtr = WSFBPTR(pScrn);

	fPtr->pEnt = xf86GetEntityInfo(pScrn->entityList[0]);

#if GET_ABI_MAJOR(ABI_VIDEODRV_VERSION) < 6
	pScrn->racMemFlags = RAC_FB | RAC_COLORMAP | RAC_CURSOR | RAC_VIEWPORT;
	pScrn->racIoFlags = pScrn->racMemFlags;
#endif

	dev = xf86FindOptionValue(fPtr->pEnt->device->options, "device");
	fPtr->fd = wsfb_open(dev);
	if (fPtr->fd == -1) {
		return FALSE;
	}
	if (ioctl(fPtr->fd, WSDISPLAYIO_GTYPE, &(fPtr->wstype)) == -1) {
		xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
			   "ioctl WSDISPLAY_GTYPE: %s\n",
			   strerror(errno));
		return FALSE;
	}
	/*
	 * Depth
	 */
	defaultDepth = 0;
	if (ioctl(fPtr->fd, WSDISPLAYIO_GETSUPPORTEDDEPTH,
		&depths) == 0) {
		/* Preferred order for default depth selection. */
		if (depths & WSDISPLAYIO_DEPTH_16)
			defaultDepth = 16;
		else if (depths & WSDISPLAYIO_DEPTH_15)
			defaultDepth = 15;
		else if (depths & WSDISPLAYIO_DEPTH_8)
			defaultDepth = 8;
		else if (depths & WSDISPLAYIO_DEPTH_24)
			defaultDepth = 24;
		else if (depths & WSDISPLAYIO_DEPTH_30)
			defaultDepth = 30;
		else if (depths & WSDISPLAYIO_DEPTH_4)
			defaultDepth = 4;
		else if (depths & WSDISPLAYIO_DEPTH_1)
			defaultDepth = 1;

		flags24 = 0;
		if (depths & WSDISPLAYIO_DEPTH_24_24)
			flags24 |= Support24bppFb;
		if (depths & WSDISPLAYIO_DEPTH_24_32)
			flags24 |= Support32bppFb;
		if (!flags24)
			flags24 = Support24bppFb;
	} else {
		/* Old way */
		if (ioctl(fPtr->fd, WSDISPLAYIO_GINFO, &fPtr->info) == -1) {
			xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
			    "no way to get depth info: %s\n",
			    strerror(errno));
			return FALSE;
		}
		if (fPtr->info.depth == 8) {
			/*
			 * We might run on a byte addressable frame buffer,
			 * but with less than 8 bits per pixel.
			 * We can know this from the colormap size.
			 */
			defaultDepth = 1;
			while ((1 << defaultDepth) < fPtr->info.cmsize)
				defaultDepth++;
		} else
			defaultDepth =
			    fPtr->info.depth <= 24 ? fPtr->info.depth : 24;

		if (fPtr->info.depth >= 24)
			flags24 = Support24bppFb|Support32bppFb;
		else
			flags24 = 0;
	}

	/* Prefer 24bpp for fb since it potentially allows larger modes. */
	if (flags24 & Support24bppFb)
		flags24 |= SupportConvert32to24 | PreferConvert32to24;

	if (!xf86SetDepthBpp(pScrn, defaultDepth, 0,
		fPtr->info.depth, flags24))
		return FALSE;

	if (fPtr->wstype == WSDISPLAY_TYPE_PCIVGA) {
		/* Set specified mode. */
		if (pScrn->display->modes != NULL &&
		    pScrn->display->modes[0] != NULL) {
			struct wsdisplay_gfx_mode gfxmode;

			if (sscanf(pScrn->display->modes[0], "%dx%d",
				&gfxmode.width, &gfxmode.height) != 2) {
				xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
				    "Can't parse mode name %s\n",
				    pScrn->display->modes[0]);
				return FALSE;
			}
			gfxmode.depth = pScrn->bitsPerPixel;
			if (ioctl(fPtr->fd, WSDISPLAYIO_SETGFXMODE,
				&gfxmode) == -1) {
				xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
				    "ioctl WSDISPLAY_SETGFXMODE: %s\n",
				    strerror(errno));
				return FALSE;
			}
		}
	}
	if (ioctl(fPtr->fd, WSDISPLAYIO_GINFO, &fPtr->info) == -1) {
		xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		    "ioctl WSDISPLAY_GINFO: %s\n",
		    strerror(errno));
		return FALSE;
	}
	if (ioctl(fPtr->fd, WSDISPLAYIO_LINEBYTES, &fPtr->linebytes) == -1) {
		xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
			   "ioctl WSDISPLAYIO_LINEBYTES: %s\n",
			   strerror(errno));
		return FALSE;
	}

	/*
	 * Quirk for LUNA: LUNA's video memory is organized in 'planar'.
	 */
	if (fPtr->wstype == WSDISPLAY_TYPE_LUNA) {
		if ((fPtr->info.depth == 8) && (pScrn->depth == 8)) {
			/*
			 * With 8bpp, use 'planar' conversion.
			 */
			fPtr->planarAfb = TRUE;
		} else {
			/*
			 * Otherwise, force to set 1bpp mode to use
			 * 1bpp Xserver.
			 */
			struct wsdisplay_gfx_mode gfxmode;
			gfxmode.width  = fPtr->info.width;
			gfxmode.height = fPtr->info.height;
			gfxmode.depth  = fPtr->info.depth = 1;

			if (ioctl(fPtr->fd, WSDISPLAYIO_SETGFXMODE,
			    &gfxmode) == -1) {
				xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
				    "ioctl WSDISPLAY_SETGFXMODE: %s\n",
				    strerror(errno));
				return FALSE;
			}
		}
	}

	/*
	 * Allocate room for saving the colormap.
	 */
	if (fPtr->info.cmsize != 0) {
		fPtr->saved_cmap.red =
		    (unsigned char *)malloc(fPtr->info.cmsize);
		if (fPtr->saved_cmap.red == NULL) {
			xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
			    "Cannot malloc %d bytes\n", fPtr->info.cmsize);
			return FALSE;
		}
		fPtr->saved_cmap.green =
		    (unsigned char *)malloc(fPtr->info.cmsize);
		if (fPtr->saved_cmap.green == NULL) {
			xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
			    "Cannot malloc %d bytes\n", fPtr->info.cmsize);
			free(fPtr->saved_cmap.red);
			return FALSE;
		}
		fPtr->saved_cmap.blue =
		    (unsigned char *)malloc(fPtr->info.cmsize);
		if (fPtr->saved_cmap.blue == NULL) {
			xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
			    "Cannot malloc %d bytes\n", fPtr->info.cmsize);
			free(fPtr->saved_cmap.red);
			free(fPtr->saved_cmap.green);
			return FALSE;
		}
	}


	/* Check consistency. */
	if (pScrn->bitsPerPixel != fPtr->info.depth) {
		xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		    "specified depth (%d) or bpp (%d) doesn't match "
		    "framebuffer depth (%d)\n", pScrn->depth,
		    pScrn->bitsPerPixel, fPtr->info.depth);
		return FALSE;
	}
	xf86PrintDepthBpp(pScrn);

	/* Get the depth24 pixmap format. */
	if (pScrn->depth == 24 && pix24bpp == 0)
		pix24bpp = xf86GetBppFromDepth(pScrn, 24);

	/* Color weight */
	if (pScrn->depth > 8) {
		rgb izeros = { 0, 0, 0 }, masks;

		switch (fPtr->wstype) {
		case WSDISPLAY_TYPE_SUN24:
		case WSDISPLAY_TYPE_SUNCG12:
		case WSDISPLAY_TYPE_SUNCG14:
		case WSDISPLAY_TYPE_SUNTCX:
		case WSDISPLAY_TYPE_SUNFFB:
		case WSDISPLAY_TYPE_AGTEN:
		case WSDISPLAY_TYPE_XVIDEO:
		case WSDISPLAY_TYPE_SUNLEO:
		case WSDISPLAY_TYPE_GBE:
			masks.red = 0x0000ff;
			masks.green = 0x00ff00;
			masks.blue = 0xff0000;
			break;
		case WSDISPLAY_TYPE_PXALCD:
		case WSDISPLAY_TYPE_SMFB:
			masks.red = 0x1f << 11;
			masks.green = 0x3f << 5;
			masks.blue = 0x1f;
			break;
		case WSDISPLAY_TYPE_MAC68K:
			if (pScrn->depth > 16) {
				masks.red = 0x0000ff;
				masks.green = 0x00ff00;
				masks.blue = 0xff0000;
			} else {
				masks.red = 0x1f << 11;
				masks.green = 0x3f << 5;
				masks.blue = 0x1f;
			}
			break;
		case WSDISPLAY_TYPE_PCIVGA:
			if (pScrn->depth > 16) {
				masks.red = 0xff0000;
				masks.green = 0x00ff00;
				masks.blue = 0x0000ff;
			} else {
				masks.red = 0x1f << 11;
				masks.green = 0x3f << 5;
				masks.blue = 0x1f;
			}
			break;
		default:
			masks.red = 0;
			masks.green = 0;
			masks.blue = 0;
			break;
		}

		if (!xf86SetWeight(pScrn, izeros, masks))
			return FALSE;
	}

	/* Visual init */
	if (!xf86SetDefaultVisual(pScrn, -1))
		return FALSE;

	/* We don't currently support DirectColor at > 8bpp . */
	if (pScrn->depth > 8 && pScrn->defaultVisual != TrueColor) {
		xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "Given default visual"
			   " (%s) is not supported at depth %d\n",
			   xf86GetVisualName(pScrn->defaultVisual),
			   pScrn->depth);
		return FALSE;
	}

	xf86SetGamma(pScrn,zeros);

	pScrn->progClock = TRUE;
	if (pScrn->depth == 8)
		pScrn->rgbBits = 6;
	else
		pScrn->rgbBits   = 8;
	pScrn->chipset   = (char *)"wsfb";
	pScrn->videoRam  = fPtr->linebytes * fPtr->info.height;

	xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Vidmem: %dk\n",
		   pScrn->videoRam/1024);

	/* Handle options. */
	xf86CollectOptions(pScrn, NULL);
	fPtr->Options = (OptionInfoRec *)malloc(sizeof(WsfbOptions));
	if (fPtr->Options == NULL)
		return FALSE;
	memcpy(fPtr->Options, WsfbOptions, sizeof(WsfbOptions));
	xf86ProcessOptions(pScrn->scrnIndex, fPtr->pEnt->device->options,
			   fPtr->Options);

	/* Use shadow framebuffer by default, on depth >= 8 */
	if (pScrn->depth >= 8)
		fPtr->shadowFB = xf86ReturnOptValBool(fPtr->Options,
						      OPTION_SHADOW_FB, TRUE);
	else
		if (xf86ReturnOptValBool(fPtr->Options,
					 OPTION_SHADOW_FB, FALSE)) {
			xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
				   "Shadow FB option ignored on depth < 8");
		}

	/* Rotation */
	fPtr->rotate = WSFB_ROTATE_NONE;
	if ((s = xf86GetOptValString(fPtr->Options, OPTION_ROTATE))) {
		if (pScrn->depth >= 8) {
			if (!xf86NameCmp(s, "CW")) {
				fPtr->shadowFB = TRUE;
				fPtr->rotate = WSFB_ROTATE_CW;
				xf86DrvMsg(pScrn->scrnIndex, X_CONFIG,
				    "Rotating screen clockwise\n");
			} else if (!xf86NameCmp(s, "CCW")) {
				fPtr->shadowFB = TRUE;
				fPtr->rotate = WSFB_ROTATE_CCW;
				xf86DrvMsg(pScrn->scrnIndex, X_CONFIG,
				    "Rotating screen counter clockwise\n");
			} else if (!xf86NameCmp(s, "UD")) {
				fPtr->shadowFB = TRUE;
				fPtr->rotate = WSFB_ROTATE_UD;
				xf86DrvMsg(pScrn->scrnIndex, X_CONFIG,
				    "Rotating screen upside down\n");
			} else {
				xf86DrvMsg(pScrn->scrnIndex, X_CONFIG,
				    "\"%s\" is not a valid value for Option "
				    "\"Rotate\"\n", s);
				xf86DrvMsg(pScrn->scrnIndex, X_INFO,
				    "Valid options are \"CW\", \"CCW\","
				    " or \"UD\"\n");
			}
		} else {
			xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
			    "Option \"Rotate\" ignored on depth < 8");
		}
	}

	/* Fake video mode struct. */
	mode = (DisplayModePtr)malloc(sizeof(DisplayModeRec));
	mode->prev = mode;
	mode->next = mode;
	mode->name = (char *)"wsfb current mode";
	mode->status = MODE_OK;
	mode->type = M_T_BUILTIN;
	mode->Clock = 0;
	mode->HDisplay = fPtr->info.width;
	mode->HSyncStart = 0;
	mode->HSyncEnd = 0;
	mode->HTotal = 0;
	mode->HSkew = 0;
	mode->VDisplay = fPtr->info.height;
	mode->VSyncStart = 0;
	mode->VSyncEnd = 0;
	mode->VTotal = 0;
	mode->VScan = 0;
	mode->Flags = 0;

	pScrn->currentMode = pScrn->modes = mode;
	pScrn->virtualX = fPtr->info.width;
	pScrn->virtualY = fPtr->info.height;
	pScrn->displayWidth = pScrn->virtualX;

	/* Quirk for LUNA's framebuffer */
	if (fPtr->wstype == WSDISPLAY_TYPE_LUNA)
		pScrn->displayWidth = fPtr->linebytes * 8;

	/* Set the display resolution. */
	xf86SetDpi(pScrn, 0, 0);


	/* Load shadow if needed. */
	if (fPtr->shadowFB) {
		xf86DrvMsg(pScrn->scrnIndex, X_CONFIG,
			   "Using \"Shadow Framebuffer\"\n");
		if (xf86LoadSubModule(pScrn, "shadow") == NULL) {
			WsfbFreeRec(pScrn);
			return FALSE;
		}
	}
	if (xf86LoadSubModule(pScrn, "fb") == NULL) {
		WsfbFreeRec(pScrn);
		return FALSE;
	}
	TRACE_EXIT("PreInit");
	return TRUE;
}

static void
wsfbUpdateRotatePacked(ScreenPtr pScreen, shadowBufPtr pBuf)
{
    shadowUpdateRotatePacked(pScreen, pBuf);
}

static void
wsfbUpdatePacked(ScreenPtr pScreen, shadowBufPtr pBuf)
{
    shadowUpdatePacked(pScreen, pBuf);
}

static void
wsfbUpdateAfb8(ScreenPtr pScreen, shadowBufPtr pBuf)
{
    shadowUpdateAfb8(pScreen, pBuf);
}

static Bool
WsfbCreateScreenResources(ScreenPtr pScreen)
{
	ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
	WsfbPtr fPtr = WSFBPTR(pScrn);
	PixmapPtr pPixmap;
	Bool ret;
	void (*shadowproc)(ScreenPtr, shadowBufPtr);
	ShadowWindowProc windowproc;

	pScreen->CreateScreenResources = fPtr->CreateScreenResources;
	ret = pScreen->CreateScreenResources(pScreen);
	pScreen->CreateScreenResources = WsfbCreateScreenResources;

	if (!ret)
		return FALSE;

	pPixmap = pScreen->GetScreenPixmap(pScreen);

	shadowproc =  fPtr->rotate ?
	    wsfbUpdateRotatePacked : wsfbUpdatePacked ;
	windowproc = WsfbWindowLinear;

 	if (fPtr->planarAfb) {
		shadowproc = wsfbUpdateAfb8;
		windowproc = WsfbWindowAfb;
	}

	if (!shadowAdd(pScreen, pPixmap, shadowproc,
		windowproc, fPtr->rotate, NULL)) {
		return FALSE;
	}
	return TRUE;
}


static Bool
WsfbShadowInit(ScreenPtr pScreen)
{
	ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
	WsfbPtr fPtr = WSFBPTR(pScrn);

	if (!shadowSetup(pScreen))
		return FALSE;
	fPtr->CreateScreenResources = pScreen->CreateScreenResources;
	pScreen->CreateScreenResources = WsfbCreateScreenResources;

	return TRUE;
}

static Bool
WsfbScreenInit(SCREEN_INIT_ARGS_DECL)
{
	ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
	WsfbPtr fPtr = WSFBPTR(pScrn);
	VisualPtr visual;
	int ret, flags, ncolors;
	int wsmode = WSDISPLAYIO_MODE_DUMBFB;
	size_t len;

	TRACE_ENTER("WsfbScreenInit");
#if DEBUG
	ErrorF("\tbitsPerPixel=%d, depth=%d, defaultVisual=%s\n"
	       "\tmask: %x,%x,%x, offset: %u,%u,%u\n",
	       pScrn->bitsPerPixel,
	       pScrn->depth,
	       xf86GetVisualName(pScrn->defaultVisual),
	       pScrn->mask.red,pScrn->mask.green,pScrn->mask.blue,
	       pScrn->offset.red,pScrn->offset.green,pScrn->offset.blue);
#endif
	switch (fPtr->info.depth) {
	case 1:
	case 4:
	case 8:
		len = fPtr->linebytes*fPtr->info.height;
		/* LUNA planar framebuffer needs some modification */
		if ((fPtr->wstype == WSDISPLAY_TYPE_LUNA) && fPtr->planarAfb)
			len *= fPtr->info.depth;
		break;
	case 16:
		if (fPtr->linebytes == fPtr->info.width) {
			len = fPtr->info.width*fPtr->info.height*sizeof(short);
		} else {
			len = fPtr->linebytes*fPtr->info.height;
		}
		break;
	case 24:
		if (fPtr->linebytes == fPtr->info.width) {
			len = fPtr->info.width*fPtr->info.height*3;
		} else {
			len = fPtr->linebytes*fPtr->info.height;
		}
		break;
	case 32:
		if (fPtr->linebytes == fPtr->info.width) {
			len = fPtr->info.width*fPtr->info.height*sizeof(int);
		} else {
			len = fPtr->linebytes*fPtr->info.height;
		}
		break;
	default:
		xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
			   "unsupported depth %d\n", fPtr->info.depth);
		return FALSE;
	}
	/* Switch to graphics mode - required before mmap. */
	if (ioctl(fPtr->fd, WSDISPLAYIO_SMODE, &wsmode) == -1) {
		xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
			   "ioctl WSDISPLAYIO_SMODE: %s\n",
			   strerror(errno));
		return FALSE;
	}

	/* On LUNA, no need to add 'offset' bytes, it is included in len */
	if (fPtr->wstype != WSDISPLAY_TYPE_LUNA)
		len += fPtr->info.offset;

	fPtr->fbmem = wsfb_mmap(len, 0, fPtr->fd);

	if (fPtr->fbmem == NULL) {
		xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
			   "wsfb_mmap: %s\n", strerror(errno));
		return FALSE;
	}
	fPtr->fbmem_len = len;

	WsfbSave(pScrn);
	pScrn->vtSema = TRUE;

	/* MI layer */
	miClearVisualTypes();
	if (pScrn->bitsPerPixel > 8) {
		if (!miSetVisualTypes(pScrn->depth, TrueColorMask,
				      pScrn->rgbBits, TrueColor))
			return FALSE;
	} else {
		if (!miSetVisualTypes(pScrn->depth,
				      miGetDefaultVisualMask(pScrn->depth),
				      pScrn->rgbBits, pScrn->defaultVisual))
			return FALSE;
	}
	if (!miSetPixmapDepths())
		return FALSE;

	if (fPtr->rotate == WSFB_ROTATE_CW
	    || fPtr->rotate == WSFB_ROTATE_CCW) {
		int tmp = pScrn->virtualX;
		pScrn->virtualX = pScrn->displayWidth = pScrn->virtualY;
		pScrn->virtualY = tmp;
	}
	if (fPtr->rotate && !fPtr->PointerMoved) {
		fPtr->PointerMoved = pScrn->PointerMoved;
		pScrn->PointerMoved = WsfbPointerMoved;
	}

	fPtr->fbstart = fPtr->fbmem + fPtr->info.offset;

	if (fPtr->shadowFB) {
		len = pScrn->virtualX * pScrn->virtualY *
		    pScrn->bitsPerPixel/8;

		/* LUNA planar framebuffer needs some modification */
		if ((fPtr->wstype == WSDISPLAY_TYPE_LUNA) && fPtr->planarAfb)
			len = pScrn->displayWidth * pScrn->virtualY;

		fPtr->shadow = calloc(1, len);

		if (!fPtr->shadow) {
			xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
			    "Failed to allocate shadow framebuffer\n");
			return FALSE;
		}
	}

	switch (pScrn->bitsPerPixel) {
	case 1:
	case 4:
	case 8:
	case 16:
	case 24:
	case 32:
		ret = fbScreenInit(pScreen,
		    fPtr->shadowFB ? fPtr->shadow : fPtr->fbstart,
		    pScrn->virtualX, pScrn->virtualY,
		    pScrn->xDpi, pScrn->yDpi,
		    pScrn->displayWidth, pScrn->bitsPerPixel);
		break;
	default:
		xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
			   "Unsupported bpp: %d", pScrn->bitsPerPixel);
		return FALSE;
	} /* case */

	if (!ret) {
		xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		    "fbScreenInit error");
		return FALSE;
	}

	if (pScrn->bitsPerPixel > 8) {
		/* Fixup RGB ordering. */
		visual = pScreen->visuals + pScreen->numVisuals;
		while (--visual >= pScreen->visuals) {
			if ((visual->class | DynamicClass) == DirectColor) {
				visual->offsetRed   = pScrn->offset.red;
				visual->offsetGreen = pScrn->offset.green;
				visual->offsetBlue  = pScrn->offset.blue;
				visual->redMask     = pScrn->mask.red;
				visual->greenMask   = pScrn->mask.green;
				visual->blueMask    = pScrn->mask.blue;
			}
		}
	}

	if (pScrn->bitsPerPixel >= 8) {
		if (!fbPictureInit(pScreen, NULL, 0))
			xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
				   "RENDER extension initialisation failed.");
	}
	if (fPtr->shadowFB && !WsfbShadowInit(pScreen)) {
		xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		    "shadow framebuffer initialization failed\n");
		return FALSE;
	}

#ifdef XFreeXDGA
	if (!fPtr->rotate)
		WsfbDGAInit(pScrn, pScreen);
	else
		xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Rotated display, "
		    "disabling DGA\n");
#endif
	if (fPtr->rotate) {
#if XORG_VERSION_CURRENT < XORG_VERSION_NUMERIC(1,19,99,1,0)
		xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		    "Enabling Driver Rotation, " "disabling RandR\n");
		xf86DisableRandR();
#endif
		if (pScrn->bitsPerPixel == 24)
			xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
			    "Rotation might be broken in 24 bpp\n");
	}

	xf86SetBlackWhitePixels(pScreen);
	xf86SetBackingStore(pScreen);

	/* Software cursor. */
	miDCInitialize(pScreen, xf86GetPointerScreenFuncs());

	/*
	 * Colormap
	 *
	 * Note that, even on less than 8 bit depth frame buffers, we
	 * expect the colormap to be programmable with 8 bit values.
	 * As of now, this is indeed the case on all OpenBSD supported
	 * graphics hardware.
	 */
	if (!miCreateDefColormap(pScreen))
		return FALSE;
	flags = CMAP_RELOAD_ON_MODE_SWITCH;
	ncolors = fPtr->info.cmsize;
	/* On StaticGray visuals, fake a 256 entries colormap. */
	if (ncolors == 0)
		ncolors = 256;
	if (pScrn->depth != 30 &&
	   !xf86HandleColormaps(pScreen, ncolors, 8, WsfbLoadPalette,
				NULL, flags))
		return FALSE;

	pScreen->SaveScreen = WsfbSaveScreen;

#ifdef XvExtension
	{
		XF86VideoAdaptorPtr *ptr;

		int n = xf86XVListGenericAdaptors(pScrn,&ptr);
		if (n) {
			xf86XVScreenInit(pScreen,ptr,n);
		}
	}
#endif

	/* Wrap the current CloseScreen function. */
	fPtr->CloseScreen = pScreen->CloseScreen;
	pScreen->CloseScreen = WsfbCloseScreen;

	TRACE_EXIT("WsfbScreenInit");
	return TRUE;
}

static Bool
WsfbCloseScreen(CLOSE_SCREEN_ARGS_DECL)
{
	ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
	PixmapPtr pPixmap;
	WsfbPtr fPtr = WSFBPTR(pScrn);


	TRACE_ENTER("WsfbCloseScreen");

	pPixmap = pScreen->GetScreenPixmap(pScreen);
	if (fPtr->shadowFB)
		shadowRemove(pScreen, pPixmap);

	if (pScrn->vtSema) {
		WsfbRestore(pScrn);
		if (munmap(fPtr->fbmem, fPtr->fbmem_len) == -1) {
			xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
				   "munmap: %s\n", strerror(errno));
		}

		fPtr->fbmem = NULL;
	}
#ifdef XFreeXDGA
	if (fPtr->pDGAMode) {
		free(fPtr->pDGAMode);
		fPtr->pDGAMode = NULL;
		fPtr->nDGAMode = 0;
	}
#endif
	pScrn->vtSema = FALSE;

	/* Unwrap CloseScreen. */
	pScreen->CloseScreen = fPtr->CloseScreen;
	TRACE_EXIT("WsfbCloseScreen");
	return (*pScreen->CloseScreen)(CLOSE_SCREEN_ARGS);
}

static void *
WsfbWindowLinear(ScreenPtr pScreen, CARD32 row, CARD32 offset, int mode,
		CARD32 *size, void *closure)
{
	ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
	WsfbPtr fPtr = WSFBPTR(pScrn);

	if (fPtr->linebytes)
		*size = fPtr->linebytes;
	else {
		if (ioctl(fPtr->fd, WSDISPLAYIO_LINEBYTES, size) == -1)
			return NULL;
		fPtr->linebytes = *size;
	}
	return ((CARD8 *)fPtr->fbstart + row *fPtr->linebytes + offset);
}

static void *
WsfbWindowAfb(ScreenPtr pScreen, CARD32 row, CARD32 offset, int mode,
		CARD32 *size, void *closure)
{
	ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
	WsfbPtr fPtr = WSFBPTR(pScrn);

	/* size is offset from start of bitplane to next bitplane */
	*size = fPtr->linebytes * fPtr->info.height;
	return ((CARD8 *)fPtr->fbstart + row * fPtr->linebytes + offset);
}

static void
WsfbPointerMoved(SCRN_ARG_TYPE arg, int x, int y)
{
    SCRN_INFO_PTR(arg);
    WsfbPtr fPtr = WSFBPTR(pScrn);
    int newX, newY;

    switch (fPtr->rotate)
    {
    case WSFB_ROTATE_CW:
	/* 90 degrees CW rotation. */
	newX = pScrn->pScreen->height - y - 1;
	newY = x;
	break;

    case WSFB_ROTATE_CCW:
	/* 90 degrees CCW rotation. */
	newX = y;
	newY = pScrn->pScreen->width - x - 1;
	break;

    case WSFB_ROTATE_UD:
	/* 180 degrees UD rotation. */
	newX = pScrn->pScreen->width - x - 1;
	newY = pScrn->pScreen->height - y - 1;
	break;

    default:
	/* No rotation. */
	newX = x;
	newY = y;
	break;
    }

    /* Pass adjusted pointer coordinates to wrapped PointerMoved function. */
    (*fPtr->PointerMoved)(arg, newX, newY);
}

static Bool
WsfbEnterVT(VT_FUNC_ARGS_DECL)
{
	SCRN_INFO_PTR(arg);
	WsfbPtr fPtr = WSFBPTR(pScrn);
	int wsmode = WSDISPLAYIO_MODE_DUMBFB;

	TRACE_ENTER("EnterVT");

	/* Switch to graphics mode. */
	if (ioctl(fPtr->fd, WSDISPLAYIO_SMODE, &wsmode) == -1) {
		xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		    "ioctl WSDISPLAYIO_SMODE: %s\n",
		    strerror(errno));
		return FALSE;
	}

	pScrn->vtSema = TRUE;
	TRACE_EXIT("EnterVT");
	return TRUE;
}

static void
WsfbLeaveVT(VT_FUNC_ARGS_DECL)
{
	SCRN_INFO_PTR(arg);

	TRACE_ENTER("LeaveVT");
	WsfbRestore(pScrn);
	TRACE_EXIT("LeaveVT");
}

static Bool
WsfbSwitchMode(SWITCH_MODE_ARGS_DECL)
{
#if DEBUG
	SCRN_INFO_PTR(arg);
#endif

	TRACE_ENTER("SwitchMode");
	/* Nothing else to do. */
	return TRUE;
}

static int
WsfbValidMode(SCRN_ARG_TYPE arg, DisplayModePtr mode, Bool verbose, int flags)
{
#if DEBUG
	SCRN_INFO_PTR(arg);
#endif

	TRACE_ENTER("ValidMode");
	return MODE_OK;
}

static void
WsfbLoadPalette(ScrnInfoPtr pScrn, int numColors, int *indices,
	       LOCO *colors, VisualPtr pVisual)
{
	WsfbPtr fPtr = WSFBPTR(pScrn);
	struct wsdisplay_cmap cmap;
	unsigned char red[256],green[256],blue[256];
	int i, indexMin=256, indexMax=0;

	TRACE_ENTER("LoadPalette");

	cmap.count   = 1;
	cmap.red   = red;
	cmap.green = green;
	cmap.blue  = blue;

	if (numColors == 1) {
		/* Optimisation */
		cmap.index = indices[0];
		red[0]   = colors[indices[0]].red;
		green[0] = colors[indices[0]].green;
		blue[0]  = colors[indices[0]].blue;
		if (ioctl(fPtr->fd,WSDISPLAYIO_PUTCMAP, &cmap) == -1)
			ErrorF("ioctl FBIOPUTCMAP: %s\n", strerror(errno));
	} else {
		/*
		 * Change all colors in 2 ioctls
		 * and limit the data to be transfered.
		 */
		for (i = 0; i < numColors; i++) {
			if (indices[i] < indexMin)
				indexMin = indices[i];
			if (indices[i] > indexMax)
				indexMax = indices[i];
		}
		cmap.index = indexMin;
		cmap.count = indexMax - indexMin + 1;
		cmap.red = &red[indexMin];
		cmap.green = &green[indexMin];
		cmap.blue = &blue[indexMin];
		/* Get current map. */
		if (ioctl(fPtr->fd, WSDISPLAYIO_GETCMAP, &cmap) == -1)
			ErrorF("ioctl FBIOGETCMAP: %s\n", strerror(errno));
		/* Change the colors that require updating. */
		for (i = 0; i < numColors; i++) {
			red[indices[i]]   = colors[indices[i]].red;
			green[indices[i]] = colors[indices[i]].green;
			blue[indices[i]]  = colors[indices[i]].blue;
		}
		/* Write the colormap back. */
		if (ioctl(fPtr->fd,WSDISPLAYIO_PUTCMAP, &cmap) == -1)
			ErrorF("ioctl FBIOPUTCMAP: %s\n", strerror(errno));
	}
	TRACE_EXIT("LoadPalette");
}

static Bool
WsfbSaveScreen(ScreenPtr pScreen, int mode)
{
	ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
	WsfbPtr fPtr = WSFBPTR(pScrn);
	int state;

	TRACE_ENTER("SaveScreen");

	if (!pScrn->vtSema)
		return TRUE;

	state = xf86IsUnblank(mode)?WSDISPLAYIO_VIDEO_ON:
	    WSDISPLAYIO_VIDEO_OFF;
	if (ioctl(fPtr->fd,
		WSDISPLAYIO_SVIDEO, &state) < 0) {
		xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		    "error in WSDISPLAY_SVIDEO %s\n", strerror(errno));
	}
	TRACE_EXIT("SaveScreen");
	return TRUE;
}


static void
WsfbSave(ScrnInfoPtr pScrn)
{
	WsfbPtr fPtr = WSFBPTR(pScrn);

	TRACE_ENTER("WsfbSave");

	if (fPtr->info.cmsize == 0)
		return;

	fPtr->saved_cmap.index = 0;
	fPtr->saved_cmap.count = fPtr->info.cmsize;
	if (ioctl(fPtr->fd, WSDISPLAYIO_GETCMAP,
		  &(fPtr->saved_cmap)) == -1) {
		xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
			   "error saving colormap %s\n", strerror(errno));
	}
	TRACE_EXIT("WsfbSave");

}

static void
WsfbRestore(ScrnInfoPtr pScrn)
{
	WsfbPtr fPtr = WSFBPTR(pScrn);
	int mode;

	TRACE_ENTER("WsfbRestore");

	if (fPtr->info.cmsize != 0) {
		/* reset colormap for text mode */
		if (ioctl(fPtr->fd, WSDISPLAYIO_PUTCMAP,
			  &(fPtr->saved_cmap)) == -1) {
			xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
				   "error restoring colormap %s\n",
				   strerror(errno));
		}
	}

	/* Clear the screen. */
	memset(fPtr->fbmem, 0, fPtr->fbmem_len);

	/* Quirk for LUNA: Restore default depth. */
	if (fPtr->wstype == WSDISPLAY_TYPE_LUNA) {
		struct wsdisplay_gfx_mode gfxmode;
		gfxmode.width  = fPtr->info.width;
		gfxmode.height = fPtr->info.height;
		gfxmode.depth  = 0;	/* set to default depth */
		
		if (ioctl(fPtr->fd, WSDISPLAYIO_SETGFXMODE, &gfxmode) == -1) {
			xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
			    "ioctl WSDISPLAY_SETGFXMODE: %s\n",
			    strerror(errno));
			return;
		}
	}

	/* Restore the text mode. */
	mode = WSDISPLAYIO_MODE_EMUL;
	if (ioctl(fPtr->fd, WSDISPLAYIO_SMODE, &mode) == -1) {
		xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
			   "error setting text mode %s\n", strerror(errno));
	}
	TRACE_EXIT("WsfbRestore");
}

#ifdef XFreeXDGA
/***********************************************************************
 * DGA stuff
 ***********************************************************************/

static Bool
WsfbDGAOpenFramebuffer(ScrnInfoPtr pScrn, char **DeviceName,
		       unsigned char **ApertureBase, int *ApertureSize,
		       int *ApertureOffset, int *flags)
{
	*DeviceName = NULL;		/* No special device */
	*ApertureBase = (unsigned char *)(pScrn->memPhysBase);
	*ApertureSize = pScrn->videoRam;
	*ApertureOffset = pScrn->fbOffset;
	*flags = 0;

	return TRUE;
}

static Bool
WsfbDGASetMode(ScrnInfoPtr pScrn, DGAModePtr pDGAMode)
{
	DisplayModePtr pMode;
	int frameX0, frameY0;

	if (pDGAMode) {
		pMode = pDGAMode->mode;
		frameX0 = frameY0 = 0;
	} else {
		if (!(pMode = pScrn->currentMode))
			return TRUE;

		frameX0 = pScrn->frameX0;
		frameY0 = pScrn->frameY0;
	}

	if (!(*pScrn->SwitchMode)(SWITCH_MODE_ARGS(pScrn, pMode)))
		return FALSE;
	(*pScrn->AdjustFrame)(ADJUST_FRAME_ARGS(pScrn, frameX0, frameY0));

	return TRUE;
}

static void
WsfbDGASetViewport(ScrnInfoPtr pScrn, int x, int y, int flags)
{
	(*pScrn->AdjustFrame)(ADJUST_FRAME_ARGS(pScrn, x, y));
}

static int
WsfbDGAGetViewport(ScrnInfoPtr pScrn)
{
	return (0);
}

static DGAFunctionRec WsfbDGAFunctions =
{
	WsfbDGAOpenFramebuffer,
	NULL,       /* CloseFramebuffer */
	WsfbDGASetMode,
	WsfbDGASetViewport,
	WsfbDGAGetViewport,
	NULL,       /* Sync */
	NULL,       /* FillRect */
	NULL,       /* BlitRect */
	NULL,       /* BlitTransRect */
};

static void
WsfbDGAAddModes(ScrnInfoPtr pScrn)
{
	WsfbPtr fPtr = WSFBPTR(pScrn);
	DisplayModePtr pMode = pScrn->modes;
	DGAModePtr pDGAMode;

	do {
		pDGAMode = realloc(fPtr->pDGAMode,
				    (fPtr->nDGAMode + 1) * sizeof(DGAModeRec));
		if (!pDGAMode)
			break;

		fPtr->pDGAMode = pDGAMode;
		pDGAMode += fPtr->nDGAMode;
		(void)memset(pDGAMode, 0, sizeof(DGAModeRec));

		++fPtr->nDGAMode;
		pDGAMode->mode = pMode;
		pDGAMode->flags = DGA_CONCURRENT_ACCESS | DGA_PIXMAP_AVAILABLE;
		pDGAMode->byteOrder = pScrn->imageByteOrder;
		pDGAMode->depth = pScrn->depth;
		pDGAMode->bitsPerPixel = pScrn->bitsPerPixel;
		pDGAMode->red_mask = pScrn->mask.red;
		pDGAMode->green_mask = pScrn->mask.green;
		pDGAMode->blue_mask = pScrn->mask.blue;
		pDGAMode->visualClass = pScrn->bitsPerPixel > 8 ?
			TrueColor : PseudoColor;
		pDGAMode->xViewportStep = 1;
		pDGAMode->yViewportStep = 1;
		pDGAMode->viewportWidth = pMode->HDisplay;
		pDGAMode->viewportHeight = pMode->VDisplay;

		if (fPtr->linebytes)
			pDGAMode->bytesPerScanline = fPtr->linebytes;
		else {
			ioctl(fPtr->fd, WSDISPLAYIO_LINEBYTES,
			      &fPtr->linebytes);
			pDGAMode->bytesPerScanline = fPtr->linebytes;
		}

		pDGAMode->imageWidth = pMode->HDisplay;
		pDGAMode->imageHeight =  pMode->VDisplay;
		pDGAMode->pixmapWidth = pDGAMode->imageWidth;
		pDGAMode->pixmapHeight = pDGAMode->imageHeight;
		pDGAMode->maxViewportX = pScrn->virtualX -
			pDGAMode->viewportWidth;
		/* LUNA planar framebuffer needs some modification */
		if ((fPtr->wstype == WSDISPLAY_TYPE_LUNA) && fPtr->planarAfb)
			pDGAMode->maxViewportX = pScrn->displayWidth -
			    pDGAMode->viewportWidth;
		pDGAMode->maxViewportY = pScrn->virtualY -
			pDGAMode->viewportHeight;

		pDGAMode->address = fPtr->fbstart;

		pMode = pMode->next;
	} while (pMode != pScrn->modes);
}

static Bool
WsfbDGAInit(ScrnInfoPtr pScrn, ScreenPtr pScreen)
{
	WsfbPtr fPtr = WSFBPTR(pScrn);

	if (pScrn->depth < 8)
		return FALSE;

	if (!fPtr->nDGAMode)
		WsfbDGAAddModes(pScrn);

	return (DGAInit(pScreen, &WsfbDGAFunctions,
			fPtr->pDGAMode, fPtr->nDGAMode));
}
#endif

static Bool
WsfbDriverFunc(ScrnInfoPtr pScrn, xorgDriverFuncOp op,
    pointer ptr)
{
	xorgHWFlags *flag;

	switch (op) {
	case GET_REQUIRED_HW_INTERFACES:
		flag = (CARD32*)ptr;
		(*flag) = 0;
		return TRUE;
	default:
		return FALSE;
	}
}
