/*	$OpenBSD: wsudl_driver.c,v 1.14 2022/07/21 18:21:06 kettenis Exp $ */

/*
 * Copyright (c) 2009 Marcus Glocker <mglocker@openbsd.org>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

/*
 * Copyright (c) 2001 Matthieu Herrb
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
 * This driver is based on the wsfb driver written by Matthieu Herrb.
 * wsfb has been mainly reduced by the stuff we don't need, and extended
 * for damage updates sent to the udl(4) device driver, using wsdisplay(4).
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <sys/utsname.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <dev/wscons/wsconsio.h>
#include <dev/usb/udlio.h>

#include "xf86.h"
#include "xf86_OSproc.h"

#include "mipointer.h"
#include "micmap.h"
#include "colormapst.h"
#include "xf86cmap.h"

#include "fb.h"

#if GET_ABI_MAJOR(ABI_VIDEODRV_VERSION) < 6
#include "xf86Resources.h"
#include "xf86RAC.h"
#endif

#include "damage.h"

#include "compat-api.h"

/*
 * Debugging.
 */
#define DEBUG	0
#if DEBUG
#define DEBUGP(x)	ErrorF("wsudl: " x "\n")
#else
#define DEBUGP(x)
#endif

/*
 * Prototypes.
 */
static pointer			WsudlSetup(pointer, pointer, int *, int *);
static void			WsudlIdentify(int);
static Bool			WsudlDriverFunc(ScrnInfoPtr, xorgDriverFuncOp,
				    pointer);
static Bool			WsudlProbe(DriverPtr, int);
static Bool			WsudlPreInit(ScrnInfoPtr, int);
static Bool			WsudlGetRec(ScrnInfoPtr);
static void			WsudlFreeRec(ScrnInfoPtr);
static const OptionInfoRec *	WsudlAvailableOptions(int, int);
static Bool			WsudlCreateScreenResources(ScreenPtr);
static Bool			WsudlScreenInit(SCREEN_INIT_ARGS_DECL);
static Bool			WsudlCloseScreen(CLOSE_SCREEN_ARGS_DECL);
static Bool			WsudlSwitchMode(SWITCH_MODE_ARGS_DECL);
static Bool			WsudlEnterVT(VT_FUNC_ARGS_DECL);
static void			WsudlLeaveVT(VT_FUNC_ARGS_DECL);
static int			WsudlValidMode(SCRN_ARG_TYPE, DisplayModePtr,
    				    Bool, int);
static void			WsudlLoadPalette(ScrnInfoPtr, int, int *,
				    LOCO *, VisualPtr);
static Bool			WsudlSaveScreen(ScreenPtr, int);
static void			WsudlRestore(ScrnInfoPtr);
static Bool			WsudlDamageInit(ScreenPtr);
static void			WsudlBlockHandler(pointer, pointer);
static void			WsudlWakeupHandler(pointer, int);
/* helper functions */
static int			wsudl_open(const char *);

/*
 * Respect privilege seperation when opening device nodes.
 */
#ifdef X_PRIVSEP
extern int priv_open_device(const char *);
#else
#define priv_open_device(n)	open(n, O_RDWR | O_NONBLOCK | O_EXCL)
#endif

/*
 * Default device node for wsdisplay.
 */
#if defined(__NetBSD__)
#define WSUDL_DEFAULT_DEV	"/dev/ttyE0"
#else
#define WSUDL_DEFAULT_DEV	"/dev/ttyC0"
#endif

/*
 * X driver glue.
 */
static int pix24bpp = 0;
#define WSUDL_VERSION 		1
#define WSUDL_NAME		"wsudl"
#define WSUDL_DRIVER_NAME	"wsudl"
_X_EXPORT DriverRec WSUDL = {
	WSUDL_VERSION,
	(char *)WSUDL_DRIVER_NAME,
	WsudlIdentify,
	WsudlProbe,
	WsudlAvailableOptions,
	NULL,
	0,
	WsudlDriverFunc
};

/*
 * Supported "chipsets".
 */
static SymTabRec WsudlChipsets[] = {
	{ 0,	"DisplayLink" },
	{ -1,	NULL }
};

/*
 * Supported options.
 */
typedef enum {
	NO_OPTS_YET
} WsudlOpts;

static const OptionInfoRec WsudlOptions[] = {
	{ -1, NULL, OPTV_NONE, { 0 }, FALSE }
};

/*
 * Private data.
 */
typedef struct {
	int				fd;
	struct wsdisplay_fbinfo		info;
	unsigned char *			fbstart;
	unsigned char *			fbmem;
	size_t				fbmem_len;
	int				linebytes;
	CloseScreenProcPtr		CloseScreen;
	CreateScreenResourcesProcPtr	CreateScreenResources;
	EntityInfoPtr			pEnt;
	OptionInfoPtr			Options;
	DamagePtr			pDamage;
	PixmapPtr			pPixmap;
} WsudlRec, *WsudlPtr;
#define WSUDLPTR(p)			((WsudlPtr)((p)->driverPrivate))

/*
 * XFree86LOADER.
 */
static XF86ModuleVersionInfo WsudlVersRec = {
	"wsudl",
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
	{ 0, 0, 0, 0 }
};

_X_EXPORT XF86ModuleData wsudlModuleData = {
	&WsudlVersRec,
	WsudlSetup,
	NULL
};

/*
 * Functions.
 */
static pointer
WsudlSetup(pointer module, pointer opts, int *errmaj, int *errmin)
{
	static Bool setupDone = FALSE;
	struct utsname name;

	DEBUGP("WsudlSetup");

	/* Check that we're being loaded on a OpenBSD or NetBSD system. */
	if (uname(&name) == -1 || (strcmp(name.sysname, "OpenBSD") != 0 &&
	                       strcmp(name.sysname, "NetBSD") != 0)) {
		if (errmaj)
			*errmaj = LDR_BADOS;
		if (errmin)
			*errmin = 0;
		return (NULL);
	}

	if (setupDone == 0) {
		setupDone = TRUE;
		xf86AddDriver(&WSUDL, module, HaveDriverFuncs);
		return ((pointer)1);
	} else {
		if (errmaj != NULL)
			*errmaj = LDR_ONCEONLY;
		return (NULL);
	}
}

static void
WsudlIdentify(int flags)
{
	DEBUGP("WsudlIdentify");

	xf86PrintChipsets(WSUDL_NAME, "driver for", WsudlChipsets);
}

static Bool
WsudlDriverFunc(ScrnInfoPtr pScrn, xorgDriverFuncOp op, pointer ptr)
{
	xorgHWFlags *flag;

	DEBUGP("WsudlDriverFunc");

	switch (op) {
	case GET_REQUIRED_HW_INTERFACES:
		flag = (CARD32*)ptr;
		(*flag) = 0;
		return (TRUE);
	default:
		return (FALSE);
	}
}

static Bool
WsudlProbe(DriverPtr drv, int flags)
{
	int i, fd, entity;
	int numDevSections;
	Bool foundScreen = FALSE;
	GDevPtr *devSections;
	const char *dev;

	DEBUGP("WsudlProbe");

	if (flags & PROBE_DETECT)
		return (FALSE);

	numDevSections = xf86MatchDevice(WSUDL_DRIVER_NAME, &devSections);
	if (numDevSections < 1)
		return (FALSE);

	for (i = 0; i < numDevSections; i++) {
		ScrnInfoPtr pScrn = NULL;

		dev = xf86FindOptionValue(devSections[i]->options, "device");
		fd = wsudl_open(dev);
		if (fd > 0) {
			entity = xf86ClaimFbSlot(drv, 0, devSections[i], TRUE);
			pScrn = xf86ConfigFbEntity(NULL, 0, entity, NULL, NULL,
			    NULL, NULL);

			if (pScrn != NULL) {
				foundScreen = TRUE;
				pScrn->driverVersion = WSUDL_VERSION;
				pScrn->driverName = (char *)WSUDL_DRIVER_NAME;
				pScrn->name = (char *)WSUDL_NAME;
				pScrn->Probe = WsudlProbe;
				pScrn->PreInit = WsudlPreInit;
				pScrn->ScreenInit = WsudlScreenInit;
				pScrn->SwitchMode = WsudlSwitchMode;
				pScrn->AdjustFrame = NULL;
				pScrn->EnterVT = WsudlEnterVT;
				pScrn->LeaveVT = WsudlLeaveVT;
				pScrn->ValidMode = WsudlValidMode;

				xf86DrvMsg(pScrn->scrnIndex, X_INFO,
				    "using %s\n", dev != NULL ? dev :						    "default device");
			}
		}
	}

	free(devSections);

	return (foundScreen);
}

static Bool
WsudlPreInit(ScrnInfoPtr pScrn, int flags)
{
	WsudlPtr fPtr;
	const char *dev;
	int r, wstype, defaultDepth, flags24;
	Gamma zeros = { 0.0, 0.0, 0.0 };
	DisplayModePtr mode;

	DEBUGP("WsudlPreInit start");

	if (flags & PROBE_DETECT)
		return (FALSE);

	if (pScrn->numEntities != 1)
		return (FALSE);

	pScrn->monitor = pScrn->confScreen->monitor;

	WsudlGetRec(pScrn);
	fPtr = WSUDLPTR(pScrn);

	fPtr->pEnt = xf86GetEntityInfo(pScrn->entityList[0]);

#ifndef XSERVER_LIBPCIACCESS
	pScrn->racMemFlags = RAC_FB | RAC_COLORMAP | RAC_CURSOR | RAC_VIEWPORT;
	pScrn->racIoFlags = pScrn->racMemFlags;
#endif
	/* open wsdisplay device */
	dev = xf86FindOptionValue(fPtr->pEnt->device->options, "device");
	fPtr->fd = wsudl_open(dev);
	if (fPtr->fd == -1)
		return (FALSE);

	/* check if we are attached to the right device driver */
	r = ioctl(fPtr->fd, WSDISPLAYIO_GTYPE, &wstype);
	if (r == -1) {
		xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		    "ioctl WSDISPLAYIO_GTYPE: %s\n", strerror(errno));
		return (FALSE);
	}
	if (wstype != WSDISPLAY_TYPE_DL) {
		xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		    "We are not attached to the udl driver\n");
		return (FALSE);
	}

	/* get resolution information */
	r = ioctl(fPtr->fd, WSDISPLAYIO_GINFO, &fPtr->info);
	if (r == -1) {
		xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		    "ioctl WSDISPLAYIO_GINFO: %s\n", strerror(errno));
		return (FALSE);
	}
	if (fPtr->info.depth == 8) {
		defaultDepth = 1;
		while ((1 << defaultDepth) < fPtr->info.cmsize)
			defaultDepth++;
	} else {
		defaultDepth =
		    fPtr->info.depth <= 24 ? fPtr->info.depth : 24;
	}
	if (fPtr->info.depth >= 24)
		flags24 = Support24bppFb | Support32bppFb;
	else
		flags24 = 0;
	if (flags24 & Support24bppFb)
		flags24 |= SupportConvert32to24 | PreferConvert32to24;
	if (!xf86SetDepthBpp(pScrn, defaultDepth, 0, 0, flags24))
		return (FALSE);
	xf86PrintDepthBpp(pScrn);

	/* get the depth24 pixmap format */
	if (pScrn->depth == 24 && pix24bpp == 0)
		pix24bpp = xf86GetBppFromDepth(pScrn, 24);

	/* color weight */
	if (pScrn->depth > 8) {
		rgb izeros = { 0, 0, 0 };
		if (!xf86SetWeight(pScrn, izeros, izeros))
			return (FALSE);
	}

	/* get bytes per line (width) */
	r = ioctl(fPtr->fd, WSDISPLAYIO_LINEBYTES, &fPtr->linebytes);
	if (r == -1) {
		xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		    "ioctl WSDISPLAYIO_LINEBYTES: %s\n", strerror(errno));
		return (FALSE);
	}

	/* visual init */
	if (!xf86SetDefaultVisual(pScrn, -1))
		return (FALSE);

	xf86SetGamma(pScrn, zeros);

	pScrn->progClock = TRUE;
	pScrn->rgbBits = 8;
	pScrn->chipset = (char *)"wsudl";
	pScrn->videoRam = fPtr->linebytes * fPtr->info.height;

	xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Vidmem: %dk\n",
	    pScrn->videoRam / 1024);

	/* handle options */
	xf86CollectOptions(pScrn, NULL);  
	if (!(fPtr->Options = malloc(sizeof(WsudlOptions))))
		return (FALSE);
	memcpy(fPtr->Options, WsudlOptions, sizeof(WsudlOptions));
	xf86ProcessOptions(pScrn->scrnIndex, fPtr->pEnt->device->options,
	    fPtr->Options);

	/* fake video mode struct */
	mode = (DisplayModePtr)malloc(sizeof(DisplayModeRec));
	mode->prev = mode;
	mode->next = mode;      
	mode->name = (char *)"wsudl current mode";
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

	/* set the display resolution */
	xf86SetDpi(pScrn, 0, 0);

	/* load fb module */
	if (xf86LoadSubModule(pScrn, "fb") == NULL) {
		WsudlFreeRec(pScrn);
		return (FALSE);
	}

	DEBUGP("WsudlPreInit done");

	return (TRUE);
}

static Bool
WsudlGetRec(ScrnInfoPtr pScrn)
{
	DEBUGP("WsudlGetRec");

	if (pScrn->driverPrivate != NULL)
		return (TRUE);

	/* allocate drivers private structure */
	pScrn->driverPrivate = xnfcalloc(sizeof(WsudlRec), 1);

	return (TRUE);
}

static void
WsudlFreeRec(ScrnInfoPtr pScrn)
{
	DEBUGP("WsudlFreeRec");

	if (pScrn->driverPrivate == NULL)
		return;

	free(pScrn->driverPrivate);
	pScrn->driverPrivate = NULL;
}

static const OptionInfoRec *
WsudlAvailableOptions(int chipid, int busid)
{
	DEBUGP("WsudlAvailableOptions");

	return (WsudlOptions);
}

static Bool
WsudlCreateScreenResources(ScreenPtr pScreen)
{
	ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
	WsudlPtr fPtr = WSUDLPTR(pScrn);
	Bool r;

	DEBUGP("WsudlCreateScreenResources");

	pScreen->CreateScreenResources = fPtr->CreateScreenResources;
	r = pScreen->CreateScreenResources(pScreen);
	pScreen->CreateScreenResources = WsudlCreateScreenResources;
	if (r == 0)
		return (FALSE);

	fPtr->pPixmap = pScreen->GetScreenPixmap(pScreen);

	fPtr->pDamage =
	    DamageCreate(NULL, NULL, DamageReportNone, TRUE, pScreen, pScreen);
	if (fPtr->pDamage == NULL)
		return (FALSE);

	r = RegisterBlockAndWakeupHandlers(WsudlBlockHandler,
	    WsudlWakeupHandler, (pointer)pScreen);
	if (r == 0)
		return (FALSE);

	DamageRegister(&fPtr->pPixmap->drawable, fPtr->pDamage);

	return (TRUE);
}

static Bool
WsudlScreenInit(SCREEN_INIT_ARGS_DECL)
{
	int r, wsmode, flags, ncolors;
	ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
	WsudlPtr fPtr = WSUDLPTR(pScrn);
	VisualPtr visual;

	DEBUGP("WsudlScreenInit start");

	/* we need to set DUMBFB mode before we can mmap the framebuffer */
	wsmode = WSDISPLAYIO_MODE_DUMBFB;
	r = ioctl(fPtr->fd, WSDISPLAYIO_SMODE, &wsmode);
	if (r == -1) {
		xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		    "ioctl WSDISPLAYIO_SMODE: %s\n",
		    strerror(errno));
		return (FALSE);
	}

	/* memory map framebuffer from kernel driver */
        fPtr->fbmem = mmap(NULL, pScrn->videoRam + fPtr->info.offset,
            PROT_READ | PROT_WRITE, MAP_SHARED, fPtr->fd, 0);
        if (fPtr->fbmem == MAP_FAILED) {
		xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		    "mmap failed: %s\n", strerror(errno));
		return (FALSE);
	}
	fPtr->fbmem_len = pScrn->videoRam + fPtr->info.offset;

	/* TODO: save colormap with WsudlSave()?  maybe not needed for us */
	pScrn->vtSema = TRUE;

	/* MI layer */
	miClearVisualTypes();
	if (pScrn->bitsPerPixel > 8) {
		if (!miSetVisualTypes(pScrn->depth, TrueColorMask,
		    pScrn->rgbBits, TrueColor))
			return (FALSE);
	} else {
		if (!miSetVisualTypes(pScrn->depth,
		    miGetDefaultVisualMask(pScrn->depth),
		    pScrn->rgbBits, pScrn->defaultVisual))
			return (FALSE);
        }
	if (!miSetPixmapDepths())
		return (FALSE);

	fPtr->fbstart = fPtr->fbmem + fPtr->info.offset;

	r = fbScreenInit(pScreen, fPtr->fbstart,
	    pScrn->virtualX,
	    pScrn->virtualY, pScrn->xDpi,
	    pScrn->yDpi, pScrn->displayWidth,
	    pScrn->bitsPerPixel);
	if (r == 0) {
		xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		    "fbScreenInit error");
		return (FALSE);
	}

	if (pScrn->bitsPerPixel > 8) {
		/* fixup RGB ordering */
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

	/* do damage initialization */
	if (!WsudlDamageInit(pScreen)) {
		xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		    "damage initialization failed\n");
		return (FALSE);
	}

	xf86SetBlackWhitePixels(pScreen);
	xf86SetBackingStore(pScreen);

	/* software cursor */
	miDCInitialize(pScreen, xf86GetPointerScreenFuncs());

	/* colormap */
	if (!miCreateDefColormap(pScreen))
		return (FALSE);
	flags = CMAP_RELOAD_ON_MODE_SWITCH;
	ncolors = fPtr->info.cmsize;
	/* On StaticGray visuals, fake a 256 entries colormap. */
	if (ncolors == 0)
		ncolors = 256;
	if (!xf86HandleColormaps(pScreen, ncolors, 8, WsudlLoadPalette,
	    NULL, flags))
		return (FALSE);

	pScreen->SaveScreen = WsudlSaveScreen;

	/* wrap the current CloseScreen function */
	fPtr->CloseScreen = pScreen->CloseScreen;
	pScreen->CloseScreen = WsudlCloseScreen;

	DEBUGP("WsudlScreenInit done");

	return (TRUE);
}

static Bool
WsudlCloseScreen(CLOSE_SCREEN_ARGS_DECL)
{
	ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
	WsudlPtr fPtr = WSUDLPTR(pScrn);

	DEBUGP("WsudlCloseScreen");

	if (pScrn->vtSema) {
		WsudlRestore(pScrn);

		if (munmap(fPtr->fbmem, fPtr->fbmem_len) == -1) {
			xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
			    "munmap: %s\n", strerror(errno));
		}
		fPtr->fbmem = NULL;
	}

	pScrn->vtSema = FALSE;

	/* unwrap CloseScreen */
	pScreen->CloseScreen = fPtr->CloseScreen;

	return ((*pScreen->CloseScreen)(CLOSE_SCREEN_ARGS));
}

static Bool
WsudlSwitchMode(SWITCH_MODE_ARGS_DECL)
{
	DEBUGP("WsudlSwitchMode");

	return (TRUE);
}

static Bool
WsudlEnterVT(VT_FUNC_ARGS_DECL)
{
	DEBUGP("WsudlEnterVT (TODO)");

	/* TODO */

	return (TRUE);
}

static void
WsudlLeaveVT(VT_FUNC_ARGS_DECL)
{
	DEBUGP("WsudlLeaveVT (TODO)");

	/* TODO */
}

static int
WsudlValidMode(SCRN_ARG_TYPE arg, DisplayModePtr mode, Bool verbose, int flags)
{
	DEBUGP("WsudlValidMode");

        return (MODE_OK);
}

static void
WsudlLoadPalette(ScrnInfoPtr pScrn, int numColors, int *indices,
    LOCO *colors, VisualPtr pVisual)
{
	DEBUGP("WsudlLoadPalette (TODO)");

	/* TODO: maybe not needed for us */
}

static Bool
WsudlSaveScreen(ScreenPtr pScreen, int mode)
{
	ScrnInfoPtr pScrn = xf86Screens[pScreen->myNum];
	WsudlPtr fPtr = WSUDLPTR(pScrn);
	int state;

	DEBUGP("WsudlSaveScreen");

	if (!pScrn->vtSema)
		return (TRUE);

	if (mode != SCREEN_SAVER_FORCER) {
		state = xf86IsUnblank(mode) ?
		WSDISPLAYIO_VIDEO_ON : WSDISPLAYIO_VIDEO_OFF;
		(void)ioctl(fPtr->fd, WSDISPLAYIO_SVIDEO, &state);
	}

	return (TRUE);
}

static void
WsudlRestore(ScrnInfoPtr pScrn)
{
	WsudlPtr fPtr = WSUDLPTR(pScrn);
	int mode, r;

	DEBUGP("WsudlRestore");

	/* clear the framebuffer memory */
	memset(fPtr->fbmem, 0, fPtr->fbmem_len);

	/* switch back to text mode */
	mode = WSDISPLAYIO_MODE_EMUL;
	r = ioctl(fPtr->fd, WSDISPLAYIO_SMODE, &mode);
	if (r == -1) {
		xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		    "error setting text mode %s\n", strerror(errno));
	}
}

static Bool
WsudlDamageInit(ScreenPtr pScreen)
{
	ScrnInfoPtr pScrn = xf86Screens[pScreen->myNum];
	WsudlPtr fPtr = WSUDLPTR(pScrn);

	DEBUGP("wsudlDamageInit");

	if (!DamageSetup(pScreen))
		return (FALSE);

	fPtr->CreateScreenResources = pScreen->CreateScreenResources;
	pScreen->CreateScreenResources = WsudlCreateScreenResources;

	return (TRUE);
}

static void
WsudlBlockHandler(pointer data, pointer waitTime)
{
	ScreenPtr pScreen = (ScreenPtr)data;
	ScrnInfoPtr pScrn = xf86Screens[pScreen->myNum];
	WsudlPtr fPtr = WSUDLPTR(pScrn);
	RegionPtr pRegion;
	BoxPtr pBox;
	struct udl_ioctl_damage d;
	int numrects, i;

	pRegion = DamageRegion(fPtr->pDamage);

	if (REGION_NOTEMPTY(pScreen, pRegion)) {
		/* get damaged areas */
		numrects = REGION_NUM_RECTS(pRegion);
		pBox = REGION_RECTS(pRegion);
 
		/* just redraw the largest damage area */
		for (i = 0; i < numrects; i++) {
			if (i == 0) {
				d.x1 = pBox->x1;
				d.x2 = pBox->x2;
				d.y1 = pBox->y1;
				d.y2 = pBox->y2;
			} else {
				if (pBox->x1 < d.x1)
					d.x1 = pBox->x1;
				if (pBox->y1 < d.y1)
					d.y1 = pBox->y1;
				if (pBox->x2 > d.x2)
					d.x2 = pBox->x2;
				if (pBox->y2 > d.y2)
					d.y2 = pBox->y2;
			}

			pBox++;
		}

		/* send damaged area to the device driver */
		(void)ioctl(fPtr->fd, UDLIO_DAMAGE, &d);
		if (d.status != UDLIO_STATUS_OK)
			ErrorF("damage command failed, giving up!\n");

		/* done */
		DamageEmpty(fPtr->pDamage);
	}
}

static void
WsudlWakeupHandler(pointer data, int i)
{
	/* nothing todo yet */
}

static int
wsudl_open(const char *dev)
{
	int fd = -1;

	/* try argument from xorg.conf first */
	if (dev == NULL || ((fd = priv_open_device(dev)) == -1)) {
		/* second: environment variable */
		dev = getenv("XDEVICE");
		if (dev == NULL || ((fd = priv_open_device(dev)) == -1)) {
			/* last try: default device */
			dev = WSUDL_DEFAULT_DEV;
			if ((fd = priv_open_device(dev)) == -1) {
				return (-1);
			}
		}
	}

	return (fd);
}
