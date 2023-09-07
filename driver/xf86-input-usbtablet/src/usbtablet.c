/*
 * Copyright (c) 1999 Lennart Augustsson <augustss@netbsd.org>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

/* $OpenBSD: usbtablet.c,v 1.15 2023/09/07 09:13:51 robert Exp $ */

/*
 * Driver for USB HID tablet devices.
 * Works for:
 *   Wacom PenPartner
 *   Wacom Graphire
 */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <unistd.h>
#include <errno.h>

#include <dev/usb/usb.h>
#include <dev/usb/usbhid.h>

#include <xorg-server.h>
#include <misc.h>
#include <xf86.h>
#include <xf86_OSproc.h>
#include <X11/extensions/XI.h>
#include <X11/extensions/XIproto.h>
#include <xf86Xinput.h>
#include <xisb.h>
#include <exevents.h>		/* Needed for InitValuator/Proximity stuff */
#include <extnsionst.h>
#include <extinit.h>

#include <usbhid.h>

#define SYSCALL(call) while(((call) == -1) && (errno == EINTR))
#define ABS(x) ((x) > 0 ? (x) : -(x))
#define mils(res) (res * 1000 / 2.54) /* resolution */

#define STYLUS_XI	"Stylus"    /* X device name for the stylus */
#define STYLUS_ID	1	    /* local id */

#define ERASER_XI	"Eraser"    /* X device name for the stylus */
#define ERASER_ID	2	    /* local id */

#define ABSOLUTE_FLAG	0x10000

#define NBUTTONS 4
#define NAXES 5	/* X, Y, Pressure, Tilt-X, Tilt-Y */

typedef struct USBTDevice USBTDevice, *USBTDevicePtr;

typedef struct {
	const char	*devName;
	int		nDevs;
	InputInfoPtr	*devices;
	double		factorX;
	double		factorY;
	hid_item_t	hidX;
	hid_item_t	hidY;
	hid_item_t	hidTiltX;
	hid_item_t	hidTiltY;
	hid_item_t	hidIn_Range;
	hid_item_t	hidTip_Pressure;
	hid_item_t	hidBarrel_Switch[NBUTTONS];
	hid_item_t	hidInvert;
	int		reportSize;
	int		reportId;
	int		nSwitch;
	USBTDevicePtr	currentProxDev;
} USBTCommon, *USBTCommonPtr;

typedef struct {
	int	x, y, pressure, buttons, xTilt, yTilt, proximity;
} USBTState;

struct USBTDevice {
	USBTCommonPtr	comm;
	USBTDevicePtr	next;
	InputInfoPtr	info;
	USBTState	state;
	int		threshold;
	int		thresCent;
	int		suppress;
	int		flags;
};

/* Function prototypes */
static MODULESETUPPROTO(SetupProc);
static void TearDownProc(pointer);

static int UsbTabletAllocateStylus(InputDriverPtr, InputInfoPtr);
static int UsbTabletAllocateEraser(InputDriverPtr, InputInfoPtr);
static int  UsbTabletAllocate(InputDriverPtr, InputInfoPtr, char *, int);
#if GET_ABI_MAJOR(ABI_XINPUT_VERSION) < 12
static InputInfoPtr UsbTabletPreInitOld(InputDriverPtr, IDevPtr, int);
#endif
static int UsbTabletPreInit(InputDriverPtr, InputInfoPtr, int);
static int UsbTabletProc(DeviceIntPtr, int);
static void UsbTabletReadInput(InputInfoPtr);
static void UsbTabletClose(InputInfoPtr);
static int UsbTabletOpenDevice(DeviceIntPtr);
static void UsbTabletSendEvents(InputInfoPtr, int, USBTState *);
static void UsbTabletSendButtons(InputInfoPtr, int, int, int, int, int, int);
static void UsbTabletOutOfProx(USBTDevicePtr prx);
static void UsbTabletIntoProx(USBTDevicePtr prx, USBTState *ds);

static XF86ModuleVersionInfo VersionRec = {
	"usbtablet",
	MODULEVENDORSTRING,
	MODINFOSTRING1,
	MODINFOSTRING2,
	XORG_VERSION_CURRENT,
	PACKAGE_VERSION_MAJOR,
	PACKAGE_VERSION_MINOR,
	PACKAGE_VERSION_PATCHLEVEL,
	ABI_CLASS_XINPUT,
	ABI_XINPUT_VERSION,
	MOD_CLASS_XINPUT,
	{0, 0, 0, 0}		/* signature to be patched into the file */
};

_X_EXPORT XF86ModuleData usbtabletModuleData = {&VersionRec,
				      SetupProc, TearDownProc };

_X_EXPORT InputDriverRec USBTABLET = {
	1,
	"usbtablet",
	NULL,
#if GET_ABI_MAJOR(ABI_XINPUT_VERSION) < 12
	UsbTabletPreInitOld,
#else
	UsbTabletPreInit,
#endif
	NULL,
	0
};

/*
 * Debugging macro
 */
#ifdef DBG
#undef DBG
#endif
#ifdef DEBUG
#undef DEBUG
#endif

static int debug_level = 0;
#define DEBUG 1
#if DEBUG
#define DBG(lvl, f) {if ((lvl) <= debug_level) f;}
#else
#define DBG(lvl, f)
#endif


/**
 ** Function definitions
 **/

static pointer
SetupProc(pointer module,
	  pointer options,
	  int *errmaj,
	  int *errmin)
{
	static Bool Initialised = FALSE;

	if (!Initialised) {
		Initialised = TRUE;

		xf86Msg(X_INFO, "USB Tablet driver\n");
		xf86AddInputDriver(&USBTABLET, module, 0);
	}
	return module;
}

static void
TearDownProc(pointer p)
{
	DBG(1, ErrorF("USB Tablet TearDownProc Called\n"));
}


static int
UsbTabletProc(DeviceIntPtr pUSBT, int what)
{
	InputInfoPtr pInfo = (InputInfoPtr)pUSBT->public.devicePrivate;
	USBTDevicePtr priv = (USBTDevicePtr)pInfo->private;
	CARD8 map[NBUTTONS+1];
	int i;
#if GET_ABI_MAJOR(ABI_XINPUT_VERSION) >= 7
	Atom btn_labels[NBUTTONS] = {0};
	Atom axes_labels[NAXES] = {0};
#endif

	switch (what) {
	case DEVICE_INIT:
		DBG(1, ErrorF("UsbTabletProc DEVICE_INIT\n"));
		pUSBT->public.on = FALSE;
		for (i = 1; i <= NBUTTONS; i++) {
			map[i] = i;
		}
		if (InitButtonClassDeviceStruct(pUSBT, NBUTTONS,
#if GET_ABI_MAJOR(ABI_XINPUT_VERSION) >= 7
			btn_labels,
#endif
			map) == FALSE) {
			xf86Msg(X_ERROR,
				"unable to allocate Button class device\n");
			return !Success;
		}
		if (InitFocusClassDeviceStruct(pUSBT) == FALSE) {
			xf86Msg(X_ERROR,
				"unable to init Focus class device\n");
			return !Success;
		}

		if (InitProximityClassDeviceStruct(pUSBT) == FALSE) {
			xf86Msg(X_ERROR,
				"unable to init proximity class device\n");
			return !Success;
		}

		if (InitValuatorClassDeviceStruct(
			pUSBT, NAXES,
#if GET_ABI_MAJOR(ABI_XINPUT_VERSION) >= 7
			axes_labels,
#endif
#if GET_ABI_MAJOR(ABI_XINPUT_VERSION) < 3
			xf86GetMotionEvents,
#endif
			GetMotionHistorySize(),
			((priv->flags & ABSOLUTE_FLAG) ? Absolute : Relative) |
			 OutOfProximity) == FALSE) {
			xf86Msg(X_ERROR,
				"unable to allocate Valuator class device\n");
			return !Success;
		}
#if GET_ABI_MAJOR(ABI_XINPUT_VERSION) < 12
		/* allocate the motion history buffer if needed */
		xf86MotionHistoryAllocate(pInfo);
		AssignTypeAndName(pUSBT, pInfo->atom, pInfo->name);
#endif

		/* open the device to gather informations */
		if (!UsbTabletOpenDevice(pUSBT))
		    return !Success;
		break;

	case DEVICE_ON:
		DBG(1, ErrorF("UsbTabletProc DEVICE_ON\n"));
		if ((pInfo->fd < 0) && (!UsbTabletOpenDevice(pUSBT))) {
			return !Success;
		}
		xf86AddEnabledDevice(pInfo);
		pUSBT->public.on = TRUE;

		break;
	case DEVICE_OFF:
		DBG(1, ErrorF("UsbTabletProc DEVICE_OFF\n"));
		if (pInfo->fd >= 0) {
			xf86RemoveEnabledDevice(pInfo);
			UsbTabletClose(pInfo);
		}
		pUSBT->public.on = FALSE;
		break;

	case DEVICE_CLOSE:
		DBG(1, ErrorF("UsbTabletProc DEVICE_CLOSE\n"));
		UsbTabletClose(pInfo);
		break;

	default:
		xf86Msg(X_ERROR, "UsbTabletProc: unsupported mode %d\n",
			what);
		return !Success;
	} /* switch */
	return Success;
}

static void
UsbTabletReadInput(InputInfoPtr pInfo)
{
	USBTDevicePtr	priv = (USBTDevicePtr)pInfo->private;
	USBTCommonPtr	comm = priv->comm;
	int		invert, len, i;
	unsigned char	buffer[200], *p;
	USBTState	ds;

	DBG(7, ErrorF("UsbTabletReadInput BEGIN device=%s fd=%d\n",
		      comm->devName, pInfo->fd));

	for(;;) {
		p = buffer;
		DBG(10, ErrorF("UsbTabletReadInput reading fd=%d len=%d\n",
			       pInfo->fd, comm->reportSize));

		len = xf86ReadSerial(pInfo->fd, p, comm->reportSize);
		DBG(8, ErrorF("UsbTabletReadInput len=%d\n", len));

		if (len <= 0) {
			if (errno != EAGAIN) {
				ErrorF("error reading USBT device\n");
			}
			break;
		}

		ds.x = hid_get_data(p, &comm->hidX);
		ds.y = hid_get_data(p, &comm->hidY);
		ds.buttons = 0;
		for (i = 0; i < comm->nSwitch; i++) {
			if (hid_get_data(p, &comm->hidBarrel_Switch[i])) {
				ds.buttons |= (1 << (i+1));
			}
		}
		invert = hid_get_data(p, &comm->hidInvert);
		ds.pressure = hid_get_data(p, &comm->hidTip_Pressure);
		if (ds.pressure > priv->threshold)
			ds.buttons |= 1;
		ds.proximity = hid_get_data(p, &comm->hidIn_Range);
		ds.xTilt = hid_get_data(p, &comm->hidTiltX);
		ds.yTilt = hid_get_data(p, &comm->hidTiltY);

		if (!ds.proximity)
			UsbTabletOutOfProx(comm->currentProxDev);

		for (i = 0; i < comm->nDevs; i++) {
			DBG(7, ErrorF("UsbTabletReadInput sending to %s\n",
				      comm->devices[i]->name));

			UsbTabletSendEvents(comm->devices[i], invert, &ds);
		}
	}
	DBG(7, ErrorF("UsbTabletReadInput END   pInfo=%p priv=%p\n",
		      pInfo, priv));
}

static void
UsbTabletOutOfProx(USBTDevicePtr prx)
{
	USBTState *ods;

	if (!prx)
		return;

	DBG(1, ErrorF("Out of proximity %s\n", prx->info->name));

	ods = &prx->state;
	prx->comm->currentProxDev = 0;

	if (prx->state.buttons) {
		/* Report buttons up when the device goes out of proximity. */
		DBG(9, ErrorF("xf86USBTOutOfProx: reset buttons\n"));
		UsbTabletSendButtons(prx->info, 0,
				   ods->x, ods->y, ods->pressure,
				   ods->xTilt, ods->yTilt);
		prx->state.buttons = 0;
	}
	DBG(1, ErrorF("xf86USBTSendEvents: out proximity\n"));
	xf86PostProximityEvent(prx->info->dev, 0, 0, 5,
			       ods->x, ods->y, ods->pressure,
			       ods->xTilt, ods->yTilt);
}

static void
UsbTabletIntoProx(USBTDevicePtr prx, USBTState *ds)
{
	if (prx->comm->currentProxDev == prx)
		return;
	UsbTabletOutOfProx(prx->comm->currentProxDev);
	prx->comm->currentProxDev = prx;

	DBG(1, ErrorF("Into proximity %s\n", prx->info->name));

	DBG(1, ErrorF("xf86USBTSendEvents: in proximity\n"));
	xf86PostProximityEvent(prx->info->dev, 1, 0, 5,
			       ds->x, ds->y, ds->pressure,
			       ds->xTilt, ds->yTilt);

}

static void
UsbTabletSendButtons(InputInfoPtr pInfo, int buttons,
		     int rx, int ry, int rz,
		     int rtx, int rty)
{
	USBTDevicePtr  priv = (USBTDevicePtr) pInfo->private;
	int           button, mask;

	for (button = 1; button < NBUTTONS; button++) {
		mask = 1 << (button-1);

		if ((mask & priv->state.buttons) != (mask & buttons)) {
			DBG(4, ErrorF("UsbTabletSendButtons button=%d is %d\n",
				      button, (buttons & mask) != 0));
			xf86PostButtonEvent(pInfo->dev,
					    (priv->flags & ABSOLUTE_FLAG),
					    button, (buttons & mask) != 0,
					    0, 5, rx, ry, rz, rtx, rty);
		}
	}
}

static void
UsbTabletSendEvents(InputInfoPtr pInfo, int invert, USBTState *ds)
{
	USBTDevicePtr	priv = (USBTDevicePtr)pInfo->private;
	USBTCommonPtr	comm = priv->comm;
	USBTState	*ods = &priv->state;
	int		is_abs;
	int		rx, ry, rz, rtx, rty;

	DBG(9, ErrorF("UsbTabletSendEvents %s x=%d y=%d pressure=%d buttons=%x "
		      "xTilt=%d yTilt=%d proximity=%d, invert=%d, eraser=%d\n",
		      pInfo->name,
		      ds->x, ds->y, ds->pressure, ds->buttons,
		      ds->xTilt, ds->yTilt, ds->proximity,
		      invert, (priv->flags & ERASER_ID) != 0));

	if (!ds->proximity)
		return;
	if (((priv->flags & ERASER_ID) != 0) != invert)
		return;

	UsbTabletIntoProx(priv, ds);

	if (ds->buttons == ods->buttons && ds->proximity == ods->proximity &&
	    ABS(ds->x - ods->x) < priv->suppress &&
	    ABS(ds->y - ods->y) < priv->suppress &&
	    ds->pressure == ods->pressure &&
	    ds->xTilt == ods->xTilt &&
	    ds->yTilt == ods->yTilt) {
		DBG(9, ErrorF("UsbTabletSendEvents no change\n"));
		return;
	}
	is_abs = 1;
	/* XXX scaling done here, since Xorg 7.3 does not call UsbTabletConvert */
	rx = ds->x * comm->factorX; ry = ds->y * comm->factorY;
	rz = ds->pressure;
	rtx = ds->xTilt; rty = ds->yTilt;

	if (rx != ods->x || ry != ods->y || rz != ods->pressure ||
	    rtx != ods->xTilt || rty != ods->yTilt) {
		DBG(9, ErrorF("UsbTabletSendEvents: motion\n"));
		xf86PostMotionEvent(pInfo->dev, is_abs, 0, 5,
				    rx, ry, rz, rtx, rty);

	}
	if (ds->buttons != ods->buttons)
		UsbTabletSendButtons(pInfo, ds->buttons,
				   rx, ry, rz, rtx, rty);

	*ods = *ds;
}

static void
UsbTabletClose(InputInfoPtr pInfo)
{
	USBTDevicePtr	priv = (USBTDevicePtr)pInfo->private;
	USBTCommonPtr	comm = priv->comm;
	int		num, i;

	for (num = 0, i = 0; i < comm->nDevs;  i++)
		if (comm->devices[i]->fd >= 0)
			num++;
	DBG(4, ErrorF("USB tablet number of open devices = %d\n", num));

	if (num == 1) {
		SYSCALL(close(pInfo->fd));
	}

	pInfo->fd = -1;
}

static Bool
UsbTabletOpen(InputInfoPtr pInfo)
{
	USBTDevicePtr	priv = (USBTDevicePtr)pInfo->private;
	USBTCommonPtr	comm = priv->comm;
	InputInfoPtr	dev;
	hid_data_t     d;
	hid_item_t     h;
	report_desc_t rd;
	int nSwitch = 0;
	int            i, r;

	DBG(1, ErrorF("opening %s\n", comm->devName));

	for (dev = comm->devices[0]; dev != NULL; dev = dev->next) {
	    if (dev->fd != -1 && dev != pInfo 
		&& strcmp(dev->type_name, XI_TABLET) == 0) {
		pInfo->fd = dev->fd;
		break;
	    }
	}
	if (pInfo->fd != -1) {
	    DBG(1, ErrorF("UsbTabletOpen: shared device already open %x\n",
			  (unsigned int)pInfo->fd));
	    return Success;
	}
	/* Use this since O_NDELAY is not implemented by libc open wrapper */
	pInfo->fd = xf86OpenSerial(pInfo->options);
	if (pInfo->fd == -1) {
		xf86Msg(X_ERROR, "Error opening %s: %s\n",
			comm->devName, strerror(errno));
		return !Success;
	}
	SYSCALL(r = ioctl(pInfo->fd, USB_GET_REPORT_ID, &comm->reportId));
	if (r == -1) {
		ErrorF("Error ioctl USB_GET_REPORT_ID on %s : %s\n",
		       comm->devName, strerror(errno));
		return !Success;
	}

	DBG(1, ErrorF("initializing tablet\n"));

	rd = hid_get_report_desc(pInfo->fd);
	if (rd == 0) {
		ErrorF("%s\n", comm->devName);
		SYSCALL(close(pInfo->fd));
		return !Success;
	}

	memset(&comm->hidX, 0, sizeof (hid_item_t));
	memset(&comm->hidY, 0, sizeof (hid_item_t));
	memset(&comm->hidTiltX, 0, sizeof (hid_item_t));
	memset(&comm->hidTiltY, 0, sizeof (hid_item_t));
	memset(&comm->hidIn_Range, 0, sizeof (hid_item_t));
	memset(&comm->hidInvert, 0, sizeof (hid_item_t));
	memset(&comm->hidTip_Pressure, 0, sizeof (hid_item_t));
	for (i = 0; i < NBUTTONS; i++) {
		memset(&comm->hidBarrel_Switch[i], 0, sizeof (hid_item_t));
	}
	for (d = hid_start_parse(rd, 1<<hid_input, comm->reportId);
	     hid_get_item(d, &h); ) {
		if (h.kind != hid_input || (h.flags & HIO_CONST))
			continue;
		if (h.usage == HID_USAGE2(HUP_GENERIC_DESKTOP, HUG_X))
			comm->hidX = h;
		if (h.usage == HID_USAGE2(HUP_GENERIC_DESKTOP, HUG_Y))
			comm->hidY = h;
		if (h.usage == HID_USAGE2(HUP_GENERIC_DESKTOP, HUD_X_TILT))
			comm->hidTiltX = h;
		if (h.usage == HID_USAGE2(HUP_GENERIC_DESKTOP, HUD_Y_TILT))
			comm->hidTiltY = h;
		if (h.usage == HID_USAGE2(HUP_DIGITIZERS, HUD_INVERT))
			comm->hidInvert = h;
		if (h.usage == HID_USAGE2(HUP_DIGITIZERS, HUD_IN_RANGE))
			comm->hidIn_Range = h;
		if (h.usage == HID_USAGE2(HUP_DIGITIZERS, HUD_TIP_PRESSURE))
			comm->hidTip_Pressure = h;
		if (h.usage == HID_USAGE2(HUP_DIGITIZERS, HUD_BARREL_SWITCH))
			comm->hidBarrel_Switch[nSwitch++] = h;
	}
	hid_end_parse(d);
	comm->nSwitch = nSwitch;
	comm->reportSize = hid_report_size(rd, hid_input, comm->reportId);
	hid_dispose_report_desc(rd);
	if (comm->hidX.report_size == 0 ||
	    comm->hidY.report_size == 0 ||
	    comm->hidIn_Range.report_size == 0) {
		xf86Msg(X_ERROR, "%s has no X, Y, or In_Range report\n",
			comm->devName);
		return !Success;
	}
	DBG(2, ErrorF("Found X at %d, size=%d\n",
		      comm->hidX.pos, comm->hidX.report_size));
	DBG(2, ErrorF("Found Y at %d, size=%d\n",
		      comm->hidY.pos, comm->hidY.report_size));
	DBG(2, ErrorF("Found Invert at %d, size=%d\n",
		      comm->hidInvert.pos, comm->hidInvert.report_size));
	DBG(2, ErrorF("Found In_Range at %d, size=%d\n",
		      comm->hidIn_Range.pos, comm->hidIn_Range.report_size));
	DBG(2, ErrorF("Found Tip_Pressure at %d, size=%d\n",
		      comm->hidTip_Pressure.pos,
		      comm->hidTip_Pressure.report_size));
	for (i = 0; i < comm->nSwitch; i++) {
		DBG(2, ErrorF("Found Barrel_Switch at %d, size=%d\n",
			      comm->hidBarrel_Switch[i].pos,
			      comm->hidBarrel_Switch[i].report_size));
	}
	DBG(2, ErrorF("Report size=%d, report id=%d\n",
		      comm->reportSize, comm->reportId));

	comm->factorX = ((double) screenInfo.screens[0]->width)
		/ (comm->hidX.logical_maximum - comm->hidX.logical_minimum);
	comm->factorY = ((double) screenInfo.screens[0]->height)
		/ (comm->hidY.logical_maximum - comm->hidY.logical_minimum);

	xf86Msg(X_PROBED, "USBT tablet X=%d..%d, Y=%d..%d",
		    comm->hidX.logical_minimum,
		    comm->hidX.logical_maximum,
		    comm->hidY.logical_minimum,
		    comm->hidY.logical_maximum);
	if (comm->hidTip_Pressure.report_size != 0)
		xf86Msg(X_NONE, ", pressure=%d..%d",
			    comm->hidTip_Pressure.logical_minimum,
			    comm->hidTip_Pressure.logical_maximum);
	xf86Msg(X_NONE, "\n");

	return Success;
}

static int
UsbTabletOpenDevice(DeviceIntPtr pUSBT)
{
	InputInfoPtr pInfo = (InputInfoPtr)pUSBT->public.devicePrivate;
	USBTDevicePtr priv = (USBTDevicePtr)pInfo->private;
	USBTCommonPtr comm = priv->comm;
	int i;
#if GET_ABI_MAJOR(ABI_XINPUT_VERSION) >= 7
	Atom axes_labels[NAXES] = {0};
#endif
	hid_item_t	*h = &comm->hidTip_Pressure;

	DBG(1, ErrorF("UsbTabletOpenDevice start\n"));
	if (pInfo->fd < 0) {
		DBG(2, ErrorF("UsbTabletOpenDevice really open\n"));
		if (UsbTabletOpen(pInfo) != Success) {
			if (pInfo->fd >= 0) {
				SYSCALL(close(pInfo->fd));
			}
			pInfo->fd = -1;
			return 0;
		}
		/* report the file descriptor to all devices */
		for (i = 0; i < comm->nDevs; i++) {
			comm->devices[i]->fd = pInfo->fd;
		}
	}

	priv->threshold =
	    h->logical_minimum +
	    (h->logical_maximum - h->logical_minimum) * priv->thresCent / 100;
	if (h->report_size != 0)
		xf86Msg(X_PROBED,
			"USBT %s pressure threshold=%d, suppress=%d\n",
			pInfo->name, priv->threshold, priv->suppress);


	/* Set the real values */
	/* XXX scaling done here, since Xorg 7.3 does not call UsbTabletConvert */
	InitValuatorAxisStruct(pUSBT,
			       0,
#if GET_ABI_MAJOR(ABI_XINPUT_VERSION) >= 7
			       axes_labels[0],
#endif
			       comm->hidX.logical_minimum * comm->factorX, /* min val */
			       comm->hidX.logical_maximum * comm->factorX, /* max val */
			       mils(1000), /* resolution */
			       0, /* min_res */
			       mils(1000) /* max_res */
#if GET_ABI_MAJOR(ABI_XINPUT_VERSION) >= 12
	    		       , Absolute
#endif
			       );
	InitValuatorAxisStruct(pUSBT,
			       1,
#if GET_ABI_MAJOR(ABI_XINPUT_VERSION) >= 7
			       axes_labels[1],
#endif
			       comm->hidY.logical_minimum * comm->factorY, /* min val */
			       comm->hidY.logical_maximum * comm->factorY, /* max val */
			       mils(1000), /* resolution */
			       0, /* min_res */
			       mils(1000) /* max_res */
#if GET_ABI_MAJOR(ABI_XINPUT_VERSION) >= 12
	    		       , Absolute
#endif
			       );
	InitValuatorAxisStruct(pUSBT,
			       2,
#if GET_ABI_MAJOR(ABI_XINPUT_VERSION) >= 7
			       axes_labels[2],
#endif
			       h->logical_minimum, /* min val */
			       h->logical_maximum, /* max val */
			       mils(1000), /* resolution */
			       0, /* min_res */
			       mils(1000) /* max_res */
#if GET_ABI_MAJOR(ABI_XINPUT_VERSION) >= 12
	    		       , Absolute
#endif
			       );
	InitValuatorAxisStruct(pUSBT,
			       3,
#if GET_ABI_MAJOR(ABI_XINPUT_VERSION) >= 7
			       axes_labels[3],
#endif
			       comm->hidTiltX.logical_minimum, /* min val */
			       comm->hidTiltX.logical_maximum, /* max val */
			       mils(1000), /* resolution */
			       0, /* min_res */
			       mils(1000) /* max_res */
#if GET_ABI_MAJOR(ABI_XINPUT_VERSION) >= 12
	    		       , Absolute
#endif
			       );
	InitValuatorAxisStruct(pUSBT,
			       4,
#if GET_ABI_MAJOR(ABI_XINPUT_VERSION) >= 7
			       axes_labels[4],
#endif
			       comm->hidTiltY.logical_minimum, /* min val */
			       comm->hidTiltY.logical_maximum, /* max val */
			       mils(1000), /* resolution */
			       0, /* min_res */
			       mils(1000) /* max_res */
#if GET_ABI_MAJOR(ABI_XINPUT_VERSION) >= 12
	    		       , Absolute
#endif
			       );
	return 1;
}

static int
UsbTabletAllocate(InputDriverPtr drv, InputInfoPtr pInfo, char *name, int flag)
{
	USBTDevicePtr priv;
	USBTCommonPtr comm;

	if (pInfo == NULL) {
		return BadValue;
	}

	priv = (USBTDevicePtr)malloc(sizeof(USBTDevice));
	if (priv == NULL) {
		return BadAlloc;
	}

	comm = (USBTCommonPtr)malloc(sizeof(USBTCommon));
	if (comm == NULL) {
		free(priv);
		return BadAlloc;
	}
	memset(priv, 0, sizeof *priv);
	memset(comm, 0, sizeof *comm);

	pInfo->name = xnfstrdup(name);
	pInfo->type_name = XI_TABLET;
	pInfo->device_control = UsbTabletProc;
	pInfo->read_input = UsbTabletReadInput;
	pInfo->control_proc = NULL;
	pInfo->switch_mode = NULL;
#if GET_ABI_MAJOR(ABI_XINPUT_VERSION) < 12
	pInfo->conversion_proc = NULL;
	pInfo->reverse_conversion_proc = NULL;
	pInfo->old_x = -1;
	pInfo->old_y = -1;
#endif
	pInfo->fd = -1;
	pInfo->private = priv;

	priv->info = pInfo;
	priv->comm = comm;
	priv->flags = ABSOLUTE_FLAG | flag;
	priv->suppress = 2;
	priv->thresCent = 5;

	comm->nDevs = 1;
	comm->devices = (InputInfoPtr*)malloc(sizeof(InputInfoPtr));
	if (comm->devices == NULL) {
		free(comm);
		free(priv);
		return BadAlloc;
	}
	comm->devices[0] = pInfo;

	return Success;
}

static int
UsbTabletAllocateStylus(InputDriverPtr drv, InputInfoPtr pInfo)
{
	return UsbTabletAllocate(drv, pInfo, STYLUS_XI, STYLUS_ID);
}

static int
UsbTabletAllocateEraser(InputDriverPtr drv, InputInfoPtr pInfo)
{
	return  UsbTabletAllocate(drv, pInfo, ERASER_XI, ERASER_ID);
}

/*
 * Called when the InputDevice Section is found in XF86Config
 */
static int
UsbTabletPreInit(InputDriverPtr drv, InputInfoPtr pInfo, int flags)
{
	USBTCommonPtr comm = NULL, c;
	USBTDevicePtr priv = NULL, p;
	InputInfoPtr localDevices;
	const char *s;
	int i, rc = Success;

#if GET_ABI_MAJOR(ABI_XINPUT_VERSION) < 12
	xf86CollectInputOptions(pInfo, NULL, NULL);
#else 
	xf86CollectInputOptions(pInfo, NULL);
#endif
	s = xf86FindOptionValue(pInfo->options, "Type");

	if (s != NULL) {
		if (xf86NameCmp(s, "stylus") == 0) {
			rc = UsbTabletAllocateStylus(drv, pInfo);
		} else if (xf86NameCmp(s, "eraser") == 0) {
			rc = UsbTabletAllocateEraser(drv, pInfo);
		} else {
			xf86Msg(X_ERROR, "%s: Invalid type specified.\n"
				"Must be one of stylus or eraser.\n",
				pInfo->name);
			rc = BadValue;
		}
		if (rc != Success)
			goto PreInit_fail;
	} else {
		xf86Msg(X_ERROR, "%s: No type specified.\n", pInfo->name);
		rc = BadValue;
		goto PreInit_fail;
	}

	priv = (USBTDevicePtr)pInfo->private;
	comm = priv->comm;

	comm->devName = xf86FindOptionValue(pInfo->options, "Device");
	if (comm->devName == NULL) {
		xf86Msg(X_ERROR, "%s: No Device specified.\n",
			pInfo->name);
		goto PreInit_fail;
	}

	/* Lookup to see if there is another device sharing
	  * the same device
	  */
	localDevices = xf86FirstLocalDevice();

	while (localDevices) {
		p = (USBTDevicePtr)localDevices->private;
		c = p->comm;

		if ((pInfo != localDevices) &&
		    (localDevices->device_control == UsbTabletProc) &&
		    (strcmp(c->devName, comm->devName) == 0)) {
			DBG(2, ErrorF("UsbTabletPreInit port share between"
				      " %s and %s\n",
				      pInfo->name, localDevices->name));
			free(comm->devices);
			free(comm);
			comm = priv->comm = c;
			comm->nDevs++;
			comm->devices = (InputInfoPtr *)
				realloc(comm->devices,
					 sizeof(InputInfoPtr)* comm->nDevs);
			comm->devices[comm->nDevs - 1] = pInfo;
			break;
		}
		localDevices = localDevices->next;
	} /* while */

	xf86ProcessCommonOptions(pInfo, pInfo->options);

	xf86Msg(X_CONFIG, "%s device is %s\n", pInfo->name, comm->devName);

	/* XXX Handle options */
	debug_level = xf86SetIntOption(pInfo->options, "DebugLevel",
				       debug_level);
	if (debug_level > 0) {
		xf86Msg(X_CONFIG, "UsbTablet: debug level set to %d\n",
			debug_level);
	}

	s = xf86FindOptionValue(pInfo->options, "Mode");
	if (s != NULL) {
		if (xf86NameCmp(s, "absolute") == 0) {
			priv->flags |= ABSOLUTE_FLAG;
		} else if (xf86NameCmp(s, "relative") == 0) {
			priv->flags &= ~ABSOLUTE_FLAG;
		} else {
			xf86Msg(X_ERROR, "%s: invalid Mode "
				"(should be absolute or relative). "
				"Using default.\n", pInfo->name);
		}
	}
	xf86Msg(X_CONFIG, "%s is in %s mode\n", pInfo->name,
		(priv->flags & ABSOLUTE_FLAG) ? "absolute" : "relative");


	i = xf86SetIntOption(pInfo->options, "ThreshHold", -1);
	if (i != -1) {
		priv->thresCent = i;
	}
	xf86Msg(i != -1 ? X_CONFIG : X_DEFAULT, "%s: threshold = %d\n",
		pInfo->name, priv->thresCent);


	i = xf86SetIntOption(pInfo->options, "Suppress", -1);
	if (i != -1) {
		priv->suppress = i;
	}
	xf86Msg(i != -1 ? X_CONFIG : X_DEFAULT, "%s: suppress = %d\n",
		pInfo->name, priv->suppress);

	return Success;

PreInit_fail:
	if (comm) {
		free(comm);
	}
	if (priv) {
		free(priv);
	}
	if (pInfo) {
		free(pInfo);
	}
	return rc;
}

#if GET_ABI_MAJOR(ABI_XINPUT_VERSION) < 12
static InputInfoPtr
UsbTabletPreInitOld(InputDriverPtr drv, IDevPtr dev, int flags)
{
	InputInfoPtr pInfo = NULL;

	pInfo = xf86AllocateInput(drv, 0);
	if (pInfo == NULL)
		return NULL;
	pInfo->name = dev->identifier;
	pInfo->flags = XI86_POINTER_CAPABLE | XI86_SEND_DRAG_EVENTS;
	pInfo->conf_idev = dev;
	pInfo->close_proc = NULL;
	pInfo->private_flags = NULL;
	pInfo->always_core_feedback = NULL;
	pInfo->flags |= XI86_POINTER_CAPABLE | XI86_CONFIGURED;
	
	if (UsbTabletPreInit(drv, pInfo, flags) != Success) {
		xf86DeleteInput(pInfo, 0);
		return NULL;
	}
	pInfo->flags |= XI86_CONFIGURED;
	return pInfo;
}
#endif
