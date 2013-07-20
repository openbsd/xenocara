/*
 * Copyright © 2005-2009,2011 Matthieu Herrb
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
/* $OpenBSD: ws.c,v 1.58 2013/07/20 13:24:50 matthieu Exp $ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <unistd.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <dev/wscons/wsconsio.h>

#include <xorg-server.h>
#include <xf86.h>
#include <xf86_OSproc.h>
#include <X11/extensions/XI.h>
#include <X11/extensions/XIproto.h>
#include <xf86Xinput.h>
#include <exevents.h>
#include <xisb.h>
#include <mipointer.h>
#include <extinit.h>

#include "ws.h"

#include <X11/Xatom.h>
#include "ws-properties.h"
#include <xserver-properties.h>

static MODULESETUPPROTO(SetupProc);
static MODULETEARDOWNPROTO(TearDownProc);

static int wsPreInit(InputDriverPtr, InputInfoPtr, int);
static void wsUnInit(InputDriverPtr, InputInfoPtr, int);
static int wsProc(DeviceIntPtr, int);
static int wsDeviceInit(DeviceIntPtr);
static int wsDeviceOn(DeviceIntPtr);
static void wsDeviceOff(DeviceIntPtr);
static void wsReadInput(InputInfoPtr);
static void wsSendButtons(InputInfoPtr, int);
static int wsSwitchMode(ClientPtr, DeviceIntPtr, int);
static Bool wsOpen(InputInfoPtr);
static void wsClose(InputInfoPtr);
static void wsControlProc(DeviceIntPtr , PtrCtrl *);

static void wsInitCalibProperty(DeviceIntPtr);
static int wsSetCalibProperty(DeviceIntPtr, Atom, XIPropertyValuePtr, BOOL);

static Atom prop_calibration;
static Atom prop_swap;

#ifdef DEBUG
int ws_debug_level = 0;
#endif

static XF86ModuleVersionInfo VersionRec = {
	"ws",
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
	{0, 0, 0, 0}
};

_X_EXPORT XF86ModuleData wsModuleData = {
	&VersionRec,
	SetupProc,
	TearDownProc
};

_X_EXPORT InputDriverRec WS = {
	1,
	"ws",
	NULL,
	wsPreInit,
	wsUnInit,
	NULL,
	0
};

static pointer
SetupProc(pointer module, pointer options, int *errmaj, int *errmin)
{
	static Bool Initialised = FALSE;

	if (!Initialised) {
		xf86AddInputDriver(&WS, module, 0);
		Initialised = TRUE;
	}
	return module;
}

static void
TearDownProc(pointer p)
{
	DBG(1, ErrorF("WS TearDownProc called\n"));
}

static int
wsPreInit(InputDriverPtr drv, InputInfoPtr pInfo, int flags)
{
	WSDevicePtr priv;
	MessageType buttons_from = X_CONFIG;
	char *s;
	int i, phy_btn = 1, rc = BadValue;

	priv = (WSDevicePtr)calloc(1, sizeof(WSDeviceRec));
	if (priv == NULL) {
		rc = BadAlloc;
		goto fail;
	}
	pInfo->private = priv;

#ifdef DEBUG
	ws_debug_level = xf86SetIntOption(pInfo->options, "DebugLevel",
	    ws_debug_level);
	xf86IDrvMsg(pInfo, X_INFO, "debuglevel %d\n", ws_debug_level);
#endif
	priv->devName = xf86SetStrOption(pInfo->options, "Device", NULL);
	if (priv->devName == NULL) {
		xf86IDrvMsg(pInfo, X_ERROR, "No Device specified.\n");
		goto fail;
	}
	priv->buttons = xf86SetIntOption(pInfo->options, "Buttons", 0);
	if (priv->buttons == 0) {
		priv->buttons = DFLTBUTTONS;
		buttons_from = X_DEFAULT;
	}

	/* Check for user-defined button mapping */
	s = xf86SetStrOption(pInfo->options, "ButtonMapping", NULL);
	if (s) {
		char *map = s, *end;
		int btn;

		do {
			btn = strtol(map, &end, 10);

			if (end == map || btn < 0 || btn > NBUTTONS) {
				xf86IDrvMsg(pInfo, X_ERROR,
				    "Invalid button mapping. Using defaults\n");
				phy_btn = 1; /* ensure defaults start at 1 */
				break;
			}

			priv->btnmap[phy_btn++] = btn;
			map = end;
		} while (end && *end != '\0' && phy_btn <= NBUTTONS);
		free(s);
	}

	for (i = phy_btn; i <= NBUTTONS; i++)
		priv->btnmap[i] = i;

	wsWheelHandleButtonMap(pInfo, &(priv->Z), "ZAxisMapping", "4 5");
	wsWheelHandleButtonMap(pInfo, &(priv->W), "WAxisMapping", "6 7");

	priv->screen_no = xf86SetIntOption(pInfo->options, "ScreenNo", 0);
	xf86IDrvMsg(pInfo, X_CONFIG, "associated screen: %d\n",
	    priv->screen_no);
	if (priv->screen_no < 0 || priv->screen_no >= screenInfo.numScreens)
		priv->screen_no = 0;

	priv->swap_axes = xf86SetBoolOption(pInfo->options, "SwapXY", 0);
	if (priv->swap_axes) {
		xf86IDrvMsg(pInfo, X_CONFIG,
		    "device will work with X and Y axes swapped\n");
	}
	priv->inv_x = 0;
	priv->inv_y = 0;
	s = xf86SetStrOption(pInfo->options, "Rotate", NULL);
	if (s) {
		if (xf86NameCmp(s, "CW") == 0) {
			priv->inv_x = 1;
			priv->inv_y = 0;
			priv->swap_axes = 1;
		} else if (xf86NameCmp(s, "CCW") == 0) {
			priv->inv_x = 0;
			priv->inv_y = 1;
			priv->swap_axes = 1;
		} else if (xf86NameCmp(s, "UD") == 0) {
			priv->inv_x = 1;
			priv->inv_y = 1;
		} else {
			xf86IDrvMsg(pInfo, X_ERROR, "\"%s\" is not a valid "
			    "value for Option \"Rotate\"\n", s);
			xf86IDrvMsg(pInfo, X_ERROR, "Valid options are "
			    "\"CW\", \"CCW\", or \"UD\"\n");
		}
		free(s);
	}
	if (wsOpen(pInfo) != Success)
		goto fail;
	if (ioctl(pInfo->fd, WSMOUSEIO_GTYPE, &priv->type) != 0)
		goto fail;
	if (priv->type == WSMOUSE_TYPE_TPANEL) {
		pInfo->type_name = XI_TOUCHSCREEN;
		priv->raw = xf86SetBoolOption(pInfo->options, "Raw", 1);
		if (priv->raw) {
			xf86IDrvMsg(pInfo, X_CONFIG,
			    "device will work in raw mode\n");
		}
	} else {
		pInfo->type_name = XI_MOUSE;
		priv->raw = 0;
	}

	if (priv->type == WSMOUSE_TYPE_TPANEL && priv->raw) {
		if (ioctl(pInfo->fd, WSMOUSEIO_GCALIBCOORDS,
		    &priv->coords) != 0) {
			xf86IDrvMsg(pInfo, X_ERROR,
			    "GCALIBCOORS failed %s\n", strerror(errno));
			goto fail;
		}

		/* get default coordinate space from kernel */
		priv->min_x = priv->coords.minx;
		priv->max_x = priv->coords.maxx;
		priv->min_y = priv->coords.miny;
		priv->max_y = priv->coords.maxy;
	} else {
		/* in calibrated mode, coordinate space, is screen coords */
		priv->min_x = 0;
		priv->max_x = screenInfo.screens[priv->screen_no]->width - 1;
		priv->min_y = 0;
		priv->max_y = screenInfo.screens[priv->screen_no]->height - 1;
	}
	/* Allow options to override this */
	priv->min_x = xf86SetIntOption(pInfo->options, "MinX", priv->min_x);
	xf86IDrvMsg(pInfo, X_INFO, "minimum x position: %d\n", priv->min_x);
	priv->max_x = xf86SetIntOption(pInfo->options, "MaxX", priv->max_x);
	xf86IDrvMsg(pInfo, X_INFO, "maximum x position: %d\n", priv->max_x);
	priv->min_y = xf86SetIntOption(pInfo->options, "MinY", priv->min_y);
	xf86IDrvMsg(pInfo, X_INFO, "minimum y position: %d\n", priv->min_y);
	priv->max_y = xf86SetIntOption(pInfo->options, "MaxY", priv->max_y);
	xf86IDrvMsg(pInfo, X_INFO, "maximum y position: %d\n", priv->max_y);

	pInfo->device_control = wsProc;
	pInfo->read_input = wsReadInput;
	pInfo->switch_mode = wsSwitchMode;

	xf86IDrvMsg(pInfo, buttons_from, "Buttons: %d\n", priv->buttons);

	wsClose(pInfo);

	wsmbEmuPreInit(pInfo);
	wsWheelEmuPreInit(pInfo);
	return Success;

fail:
	if (pInfo->fd >= 0)
		wsClose(pInfo);
	if (priv != NULL) {
		free(priv);
		pInfo->private = NULL;
	}
	return rc;
}

static void
wsUnInit(InputDriverPtr drv, InputInfoPtr pInfo, int flags)
{
	WSDevicePtr priv = (WSDevicePtr)pInfo->private;

	if (priv) {
		free(priv->devName);
		priv->devName = NULL;
	}
	xf86DeleteInput(pInfo, flags);
}

static int
wsProc(DeviceIntPtr pWS, int what)
{
	InputInfoPtr pInfo = (InputInfoPtr)pWS->public.devicePrivate;

	switch (what) {
	case DEVICE_INIT:
		return wsDeviceInit(pWS);

	case DEVICE_ON:
		return wsDeviceOn(pWS);

	case DEVICE_OFF:
		wsDeviceOff(pWS);
		break;

	case DEVICE_CLOSE:
		DBG(1, ErrorF("WS DEVICE_CLOSE\n"));
		wsClose(pInfo);
		break;

	default:
		xf86IDrvMsg(pInfo, X_ERROR, "unknown command %d\n", what);
		return !Success;
	}
	return Success;
}

static int
wsDeviceInit(DeviceIntPtr pWS)
{
	InputInfoPtr pInfo = (InputInfoPtr)pWS->public.devicePrivate;
	WSDevicePtr priv = (WSDevicePtr)pInfo->private;
	int xmin, xmax, ymin, ymax;
	Atom btn_labels[NBUTTONS] = {0};
	Atom axes_labels[NAXES] = {0};

	DBG(1, ErrorF("WS DEVICE_INIT\n"));

	btn_labels[0] = XIGetKnownProperty(BTN_LABEL_PROP_BTN_LEFT);
	btn_labels[1] = XIGetKnownProperty(BTN_LABEL_PROP_BTN_MIDDLE);
	btn_labels[2] = XIGetKnownProperty(BTN_LABEL_PROP_BTN_RIGHT);
	if (!InitButtonClassDeviceStruct(pWS, min(priv->buttons, NBUTTONS),
	    btn_labels, priv->btnmap))
		return !Success;

	if (priv->type == WSMOUSE_TYPE_TPANEL) {
		xmin = priv->min_x;
		xmax = priv->max_x;
		ymin = priv->min_y;
		ymax = priv->max_y;
	} else {
		xmin = -1;
		xmax = -1;
		ymin = -1;
		ymax = -1;
	}

	if (priv->swap_axes) {
		int tmp;
		tmp = xmin;
		xmin = ymin;
		ymin = tmp;
		tmp = xmax;
		xmax = ymax;
		ymax = tmp;
	}
	if (priv->type == WSMOUSE_TYPE_TPANEL) {
		axes_labels[0] = XIGetKnownProperty(AXIS_LABEL_PROP_ABS_X);
		axes_labels[1] = XIGetKnownProperty(AXIS_LABEL_PROP_ABS_Y);
	} else {
		axes_labels[0] = XIGetKnownProperty(AXIS_LABEL_PROP_REL_X);
		axes_labels[1] = XIGetKnownProperty(AXIS_LABEL_PROP_REL_Y);
	}
	if (!InitValuatorClassDeviceStruct(pWS,
	    NAXES, axes_labels, GetMotionHistorySize(),
	    priv->type == WSMOUSE_TYPE_TPANEL ? Absolute : Relative))
		return !Success;
	if (!InitPtrFeedbackClassDeviceStruct(pWS, wsControlProc))
		return !Success;

	xf86InitValuatorAxisStruct(pWS, 0,
	    axes_labels[0],
	    xmin, xmax, 1, 0, 1,
	    priv->type == WSMOUSE_TYPE_TPANEL ? Absolute : Relative);
	xf86InitValuatorDefaults(pWS, 0);

	xf86InitValuatorAxisStruct(pWS, 1,
	    axes_labels[1],
	    ymin, ymax, 1, 0, 1,
	    priv->type == WSMOUSE_TYPE_TPANEL ? Absolute : Relative);
	xf86InitValuatorDefaults(pWS, 1);

	pWS->public.on = FALSE;
	if (wsOpen(pInfo) != Success) {
		return !Success;
	}

	if (priv->type == WSMOUSE_TYPE_TPANEL)
		wsInitCalibProperty(pWS);
	wsmbEmuInitProperty(pWS);
	wsWheelEmuInitProperty(pWS);
	return Success;
}

static int
wsDeviceOn(DeviceIntPtr pWS)
{
	InputInfoPtr pInfo = (InputInfoPtr)pWS->public.devicePrivate;
	WSDevicePtr priv = (WSDevicePtr)pInfo->private;
	struct wsmouse_calibcoords coords;

	DBG(1, ErrorF("WS DEVICE ON\n"));
	if ((pInfo->fd < 0) && (wsOpen(pInfo) != Success)) {
		xf86IDrvMsg(pInfo, X_ERROR, "wsOpen failed %s\n",
		    strerror(errno));
		return !Success;
	}

	if (priv->type == WSMOUSE_TYPE_TPANEL) {
		/* get calibration values */
		if (ioctl(pInfo->fd, WSMOUSEIO_GCALIBCOORDS, &coords) != 0) {
			xf86IDrvMsg(pInfo, X_ERROR, "GCALIBCOORS failed %s\n",
			    strerror(errno));
			return !Success;
		}
		memcpy(&priv->coords, &coords, sizeof coords);
		/* set raw mode */
		if (coords.samplelen != priv->raw) {
			coords.samplelen = priv->raw;
			if (ioctl(pInfo->fd, WSMOUSEIO_SCALIBCOORDS,
			    &coords) != 0) {
				xf86IDrvMsg(pInfo, X_ERROR,
				    "SCALIBCOORS failed %s\n", strerror(errno));
				return !Success;
			}
		}
	}
	priv->buffer = XisbNew(pInfo->fd,
	    sizeof(struct wscons_event) * NUMEVENTS);
	if (priv->buffer == NULL) {
		xf86IDrvMsg(pInfo, X_ERROR, "cannot alloc xisb buffer\n");
		wsClose(pInfo);
		return !Success;
	}
	xf86AddEnabledDevice(pInfo);
	wsmbEmuOn(pInfo);
	pWS->public.on = TRUE;
	return Success;
}

static void
wsDeviceOff(DeviceIntPtr pWS)
{
	InputInfoPtr pInfo = (InputInfoPtr)pWS->public.devicePrivate;
	WSDevicePtr priv = (WSDevicePtr)pInfo->private;
	struct wsmouse_calibcoords coords;

	DBG(1, ErrorF("WS DEVICE OFF\n"));
	wsmbEmuFinalize(pInfo);
	if (priv->type == WSMOUSE_TYPE_TPANEL) {
		/* Restore calibration data */
		memcpy(&coords, &priv->coords, sizeof coords);
		if (ioctl(pInfo->fd, WSMOUSEIO_SCALIBCOORDS, &coords) != 0) {
			xf86IDrvMsg(pInfo, X_ERROR, "SCALIBCOORS failed %s\n",
			    strerror(errno));
		}
	}
	if (pInfo->fd >= 0) {
		xf86RemoveEnabledDevice(pInfo);
		wsClose(pInfo);
	}
	if (priv->buffer) {
		XisbFree(priv->buffer);
		priv->buffer = NULL;
	}
	pWS->public.on = FALSE;
}

static void
wsReadInput(InputInfoPtr pInfo)
{
	WSDevicePtr priv = (WSDevicePtr)pInfo->private;
	static struct wscons_event eventList[NUMEVENTS];
	int n, c, dx, dy;
	struct wscons_event *event = eventList;
	unsigned char *pBuf;

	XisbBlockDuration(priv->buffer, -1);
	pBuf = (unsigned char *)eventList;
	n = 0;
	while (n < sizeof(eventList) && (c = XisbRead(priv->buffer)) >= 0) {
		pBuf[n++] = (unsigned char)c;
	}

	if (n == 0)
		return;

	dx = dy = 0;
	n /= sizeof(struct wscons_event);
	while (n--) {
		int buttons = priv->lastButtons;
		int newdx = 0, newdy = 0, dz = 0, dw = 0, ax = 0, ay = 0;
		int zbutton = 0, wbutton = 0;

		switch (event->type) {
		case WSCONS_EVENT_MOUSE_UP:
			buttons &= ~(1 << event->value);
			DBG(4, ErrorF("Button %d up %x\n", event->value,
			    buttons));
			break;
		case WSCONS_EVENT_MOUSE_DOWN:
			buttons |= (1 << event->value);
			DBG(4, ErrorF("Button %d down %x\n", event->value,
			    buttons));
			break;
		case WSCONS_EVENT_MOUSE_DELTA_X:
			if (!dx)
				dx = event->value;
			else
				newdx = event->value;
			DBG(4, ErrorF("Relative X %d\n", event->value));
			break;
		case WSCONS_EVENT_MOUSE_DELTA_Y:
			if (!dy)
				dy = -event->value;
			else
				newdy = -event->value;
			DBG(4, ErrorF("Relative Y %d\n", event->value));
			break;
		case WSCONS_EVENT_MOUSE_ABSOLUTE_X:
			DBG(4, ErrorF("Absolute X %d\n", event->value));
			if (event->value == 4095)
				break;
			ax = event->value;
			if (priv->inv_x)
				ax = priv->max_x - ax + priv->min_x;
			break;
		case WSCONS_EVENT_MOUSE_ABSOLUTE_Y:
			DBG(4, ErrorF("Absolute Y %d\n", event->value));
			ay = event->value;
			if (priv->inv_y)
				ay = priv->max_y - ay + priv->min_y;
			break;
		case WSCONS_EVENT_MOUSE_DELTA_Z:
			DBG(4, ErrorF("Relative Z %d\n", event->value));
			dz = event->value;
			break;
		case WSCONS_EVENT_MOUSE_ABSOLUTE_Z:
			/* ignore those */
			++event;
			continue;
			break;
		case WSCONS_EVENT_MOUSE_DELTA_W:
			DBG(4, ErrorF("Relative W %d\n", event->value));
			dw = event->value;
			break;
		default:
			xf86IDrvMsg(pInfo, X_WARNING,
			    "bad wsmouse event type=%d\n", event->type);
			++event;
			continue;
		}
		++event;

		if ((newdx || newdy) || ((dx || dy) &&
		    event->type != WSCONS_EVENT_MOUSE_DELTA_X &&
		    event->type != WSCONS_EVENT_MOUSE_DELTA_Y)) {
			int tmpx = dx, tmpy = dy;
			dx = newdx;
			dy = newdy;

			if (wsWheelEmuFilterMotion(pInfo, tmpx, tmpy))
				continue;

			/* relative motion event */
			DBG(3, ErrorF("postMotionEvent dX %d dY %d\n",
			    tmpx, tmpy));
			xf86PostMotionEvent(pInfo->dev, 0, 0, 2, tmpx, tmpy);
		}
		if (dz && priv->Z.negative != WS_NOMAP
		    && priv->Z.positive != WS_NOMAP) {
			zbutton = (dz < 0) ? priv->Z.negative :
			    priv->Z.positive;
			DBG(4, ErrorF("Z -> button %d\n", zbutton));
			wsButtonClicks(pInfo, zbutton, abs(dz));
		}
		if (dw && priv->W.negative != WS_NOMAP
		    && priv->W.positive != WS_NOMAP) {
			wbutton = (dw < 0) ? priv->W.negative :
			    priv->W.positive;
			DBG(4, ErrorF("W -> button %d\n", wbutton));
			wsButtonClicks(pInfo, wbutton, abs(dw));
		}
		if (priv->lastButtons != buttons) {
			/* button event */
			wsSendButtons(pInfo, buttons);
		}
		if (priv->swap_axes) {
			int tmp;

			tmp = ax;
			ax = ay;
			ay = tmp;
		}
		if (ax) {
			int xdelta = ax - priv->old_ax;
			priv->old_ax = ax;
			if (wsWheelEmuFilterMotion(pInfo, xdelta, 0))
				continue;

			/* absolute position event */
			DBG(3, ErrorF("postMotionEvent X %d\n", ax));
			xf86PostMotionEvent(pInfo->dev, 1, 0, 1, ax);
		}
		if (ay) {
			int ydelta = ay - priv->old_ay;
			priv->old_ay = ay;
			if (wsWheelEmuFilterMotion(pInfo, 0, ydelta))
				continue;

			/* absolute position event */
			DBG(3, ErrorF("postMotionEvent y %d\n", ay));
			xf86PostMotionEvent(pInfo->dev, 1, 1, 1, ay);
		}
	}
	if (dx || dy) {
		if (wsWheelEmuFilterMotion(pInfo, dx, dy))
			return;

		/* relative motion event */
		DBG(3, ErrorF("postMotionEvent dX %d dY %d\n",
		    dx, dy));
		xf86PostMotionEvent(pInfo->dev, 0, 0, 2, dx, dy);
	}
	return;
}

static void
wsSendButtons(InputInfoPtr pInfo, int buttons)
{
	WSDevicePtr priv = (WSDevicePtr)pInfo->private;
	int change, button, press;

	change = buttons ^ priv->lastButtons;
	while (change) {
		button = ffs(change);
		press = buttons & (1 << (button - 1));
		change &= ~(1 << (button - 1));

		if (wsWheelEmuFilterButton(pInfo, button, press))
			continue;

		if (wsmbEmuFilterEvent(pInfo, button, press))
			continue;

		xf86PostButtonEvent(pInfo->dev, TRUE, button, press, 0, 0);
		DBG(3, ErrorF("post button event %d %d\n", button, press));
	}
	priv->lastButtons = buttons;
}

static int
wsSwitchMode(ClientPtr client, DeviceIntPtr dev, int mode)
{
	return BadMatch;
}

static Bool
wsOpen(InputInfoPtr pInfo)
{
	WSDevicePtr priv = (WSDevicePtr)pInfo->private;
#ifdef __NetBSD__
	int version = WSMOUSE_EVENT_VERSION;
#endif

	DBG(1, ErrorF("WS open %s\n", priv->devName));
	pInfo->fd = xf86OpenSerial(pInfo->options);
	if (pInfo->fd == -1) {
		xf86IDrvMsg(pInfo, X_ERROR, "cannot open input device\n");
		return !Success;
	}
#ifdef __NetBSD__
	if (ioctl(pInfo->fd, WSMOUSEIO_SETVERSION, &version) == -1) {
		xf86IDrvMsg(pInfo, X_ERROR,
		    "cannot set wsmouse event version\n");
		return !Success;
	}
#endif
	return Success;
}

static void
wsClose(InputInfoPtr pInfo)
{
	xf86CloseSerial(pInfo->fd);
	pInfo->fd = -1;
}

static void
wsControlProc(DeviceIntPtr device, PtrCtrl *ctrl)
{
	/* Nothing to do, dix handles all settings */
	DBG(1, ErrorF("wsControlProc\n"));
}

static void
wsInitCalibProperty(DeviceIntPtr device)
{
	InputInfoPtr pInfo = (InputInfoPtr)device->public.devicePrivate;
	WSDevicePtr priv = (WSDevicePtr)pInfo->private;
	int rc;

	DBG(1, ErrorF("wsInitCalibProperty\n"));

	prop_calibration = MakeAtom(WS_PROP_CALIBRATION,
	    strlen(WS_PROP_CALIBRATION), TRUE);
	rc = XIChangeDeviceProperty(device, prop_calibration, XA_INTEGER, 32,
	    PropModeReplace, 4, &priv->min_x, FALSE);
	if (rc != Success) {
		xf86IDrvMsg(pInfo, X_ERROR,
		    "cannot create device property %s: %d\n",
		    WS_PROP_CALIBRATION, rc);
		return;
	}
	XISetDevicePropertyDeletable(device, prop_calibration, FALSE);

	prop_swap = MakeAtom(WS_PROP_SWAP_AXES,
	    strlen(WS_PROP_SWAP_AXES), TRUE);
	rc = XIChangeDeviceProperty(device, prop_swap, XA_INTEGER, 8,
	    PropModeReplace, 1, &priv->swap_axes, FALSE);
	if (rc != Success) {
		xf86IDrvMsg(pInfo, X_ERROR,
		    "cannot create device property %s: %d\n",
		    WS_PROP_SWAP_AXES, rc);
		return;
	}
	XISetDevicePropertyDeletable(device, prop_swap, FALSE);

	XIRegisterPropertyHandler(device, wsSetCalibProperty, NULL, NULL);

	return;
}

static int
wsSetCalibProperty(DeviceIntPtr device, Atom atom, XIPropertyValuePtr val,
    BOOL checkonly)
{
	InputInfoPtr pInfo = (InputInfoPtr)device->public.devicePrivate;
	WSDevicePtr priv = (WSDevicePtr)pInfo->private;
	struct wsmouse_calibcoords coords;
	int need_update = 0;
	AxisInfoPtr ax = device->valuator->axes,
		    ay = device->valuator->axes + 1;

	DBG(1, ErrorF("wsSetCalibProperty %s\n", NameForAtom(atom)));

	if (atom == prop_calibration) {
		if (val->format != 32 || val->type != XA_INTEGER)
			return BadMatch;
		if (val->size != 4 && val->size != 0)
			return BadMatch;
		if (!checkonly) {
			if (val->size == 0) {
				DBG(1, ErrorF(" uncalibrate\n"));
				priv->min_x = 0;
				priv->max_x = -1;
				priv->min_y = 0;
				priv->max_y = -1;
			} else {
				priv->min_x = ((int *)(val->data))[0];
				priv->max_x = ((int *)(val->data))[1];
				priv->min_y = ((int *)(val->data))[2];
				priv->max_y = ((int *)(val->data))[3];
				DBG(1, ErrorF(" calibrate %d %d %d %d\n",
					priv->min_x, priv->max_x,
					priv->min_y, priv->max_y));
				need_update++;
			}
			/* Update axes descriptors */
			if (!priv->swap_axes) {
				ax->min_value = priv->min_x;
				ax->max_value = priv->max_x;
				ay->min_value = priv->min_y;
				ay->max_value = priv->max_y;
			} else {
				ax->min_value = priv->min_y;
				ax->max_value = priv->max_y;
				ay->min_value = priv->min_x;
				ay->max_value = priv->max_x;
			}
		}
	} else if (atom == prop_swap) {
		if (val->format != 8 || val->type != XA_INTEGER ||
		    val->size != 1)
			return BadMatch;
		if (!checkonly) {
			priv->swap_axes = *((BOOL *)val->data);
			DBG(1, ErrorF("swap_axes %d\n", priv->swap_axes));
			need_update++;
		}
	}
	if (need_update) {
		/* Update the saved values to be restored on device off */
		priv->coords.minx = priv->min_x;
		priv->coords.maxx = priv->max_x;
		priv->coords.miny = priv->min_y;
		priv->coords.maxy = priv->max_y;
		priv->coords.swapxy = priv->swap_axes;

		/* Update the kernel calibration table */
		coords.minx = priv->min_x;
		coords.maxx = priv->max_x;
		coords.miny = priv->min_y;
		coords.maxy = priv->max_y;
		coords.swapxy = priv->swap_axes;
		coords.samplelen = priv->raw;
		coords.resx = priv->coords.resx;
		coords.resy = priv->coords.resy;
		if (ioctl(pInfo->fd, WSMOUSEIO_SCALIBCOORDS, &coords) != 0) {
			xf86IDrvMsg(pInfo, X_ERROR, "SCALIBCOORDS failed %s\n",
			    strerror(errno));
		}
	}
	return Success;
}

void
wsWheelHandleButtonMap(InputInfoPtr pInfo, WheelAxisPtr pAxis,
    const char* axis_name, const char* default_value)
{
	WSDevicePtr priv = (WSDevicePtr)pInfo->private;
	char *option_string;
	int b1, b2;

	pAxis->negative = pAxis->positive = WS_NOMAP;
	pAxis->traveled_distance = 0;

	option_string = xf86SetStrOption(pInfo->options, axis_name,
	    default_value);
	if (option_string) {
		if (sscanf(option_string, "%d %d", &b1, &b2) == 2 &&
		    b1 > 0 && b1 <= NBUTTONS &&
		    b2 > 0 && b2 <= NBUTTONS) {
			xf86IDrvMsg(pInfo, X_CONFIG, "%s: buttons %d and %d\n",
			    axis_name, b1, b2);

			pAxis->negative = b1;
			pAxis->positive = b2;

			if (max(b1, b2) > priv->buttons)
				priv->buttons = max(b1, b2);
		} else {
			xf86IDrvMsg(pInfo, X_WARNING,
			    "Invalid %s value: \"%s\"\n",
			    axis_name, option_string);
		}
		free(option_string);
	}
}

void
wsButtonClicks(InputInfoPtr pInfo, int button, int count)
{
	int i;

	for (i = 0; i < count; i++) {
		xf86PostButtonEvent(pInfo->dev, TRUE, button, 1, 0, 0);
		xf86PostButtonEvent(pInfo->dev, TRUE, button, 0, 0, 0);
	}
}
