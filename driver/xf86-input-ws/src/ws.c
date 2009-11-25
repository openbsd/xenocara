/*
 * Copyright (c) 2005-2009 Matthieu Herrb
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
/* $OpenBSD: ws.c,v 1.17 2009/11/25 17:59:42 matthieu Exp $ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <unistd.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <dev/wscons/wsconsio.h>

#include <xf86.h>

#include <xf86_OSproc.h>
#include <X11/extensions/XI.h>
#include <X11/extensions/XIproto.h>
#include <xf86Xinput.h>
#include <exevents.h>
#include <xisb.h>
#include <mipointer.h>
#include <extinit.h>

#if GET_ABI_MAJOR(ABI_XINPUT_VERSION) >= 7
#include <X11/Xatom.h>
#include <xserver-properties.h>
#endif

#define NAXES 2			/* X and Y axes only */
#define NBUTTONS 32		/* max theoretical buttons */
#define DFLTBUTTONS 3		/* default number of buttons */
#define NUMEVENTS 16		/* max # of ws events to read at once */

typedef struct WSDevice {
	char *devName;		/* device name */
	int type;		/* ws device type */
	unsigned int buttons;	/* # of buttons */
	unsigned int lastButtons; /* last state of buttons */
	int x, y;		/* current abs coordinates */
	int min_x, max_x, min_y, max_y; /* coord space */
	int swap_axes;
	int raw;
	int inv_x, inv_y;
	int screen_width, screen_height;
	int screen_no;
	int num, den, threshold; /* relative accel params */
	pointer buffer;
	int negativeZ, positiveZ; /* mappings for Z axis */
	int negativeW, positiveW; /* mappings for W axis */
} WSDeviceRec, *WSDevicePtr;

static MODULESETUPPROTO(SetupProc);
static void TearDownProc(pointer);

static InputInfoPtr wsPreInit(InputDriverPtr, IDevPtr, int);
static int wsProc(DeviceIntPtr, int);
static int wsDeviceInit(DeviceIntPtr);
static int wsDeviceOn(DeviceIntPtr);
static void wsDeviceOff(DeviceIntPtr);
static void wsReadInput(InputInfoPtr);
static void wsSendButtons(InputInfoPtr, int);
static int wsChangeControl(InputInfoPtr, xDeviceCtl *);
static int wsSwitchMode(ClientPtr, DeviceIntPtr, int);
static Bool wsOpen(InputInfoPtr);
static void wsClose(InputInfoPtr);
static void wsControlProc(DeviceIntPtr , PtrCtrl *);


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

#define WS_NOZMAP 0

XF86ModuleData wsModuleData = {&VersionRec,
			       SetupProc, TearDownProc };


InputDriverRec WS = {
	1,
	"ws",
	NULL,
	wsPreInit,
	NULL,
	NULL,
	0
};

/* #undef DEBUG */
#define DEBUG
#undef DBG
static int debug_level = 0;
#ifdef DEBUG
# define DBG(lvl, f) { if ((lvl) <= debug_level) f;}
#else
# define DBG(lvl, f)
#endif

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

static InputInfoPtr
wsPreInit(InputDriverPtr drv, IDevPtr dev, int flags)
{
	struct wsmouse_calibcoords coords;
	InputInfoPtr pInfo = NULL;
	WSDevicePtr priv;
	MessageType buttons_from = X_CONFIG;
	char *s;

	pInfo = xf86AllocateInput(drv, 0);
	if (pInfo == NULL) {
		return NULL;
	}
	priv = (WSDevicePtr)xcalloc(1, sizeof(WSDeviceRec));
	if (priv == NULL)
		goto fail;
	pInfo->flags = XI86_POINTER_CAPABLE | XI86_SEND_DRAG_EVENTS;
	pInfo->conf_idev = dev;
	pInfo->name = "ws";
	pInfo->private = priv;

	xf86CollectInputOptions(pInfo, NULL, NULL);
	xf86ProcessCommonOptions(pInfo, pInfo->options);
#ifdef DEBUG
	debug_level = xf86SetIntOption(pInfo->options, "DebugLevel",
	    debug_level);
	xf86Msg(X_INFO, "%s: debuglevel %d\n", dev->identifier, debug_level);
#endif
	priv->devName = xf86FindOptionValue(pInfo->options, "Device");
	if (priv->devName == NULL) {
		xf86Msg(X_ERROR, "%s: No Device specified.\n",
			dev->identifier);
		goto fail;
	}
	priv->buttons = xf86SetIntOption(pInfo->options, "Buttons", 0);
	if (priv->buttons == 0) {
		priv->buttons = DFLTBUTTONS;
		buttons_from = X_DEFAULT;
	}
	priv->negativeZ =  priv->positiveZ = WS_NOZMAP;
	s = xf86SetStrOption(pInfo->options, "ZAxisMapping", NULL);
	if (s) {
		int b1, b2;

		if (sscanf(s, "%d %d", &b1, &b2) == 2 &&
		    b1 > 0 && b1 <= NBUTTONS &&
		    b2 > 0 && b2 <= NBUTTONS) {
			priv->negativeZ = b1;
			priv->positiveZ = b2;
			xf86Msg(X_CONFIG,
			    "%s: ZAxisMapping: buttons %d and %d\n",
			    pInfo->name, b1, b2);
		} else {
			xf86Msg(X_WARNING, "%s: invalid ZAxisMapping value: "
			    "\"%s\"\n", pInfo->name, s);
		}
	}
	if (priv->negativeZ > priv->buttons) {
		priv->buttons = priv->negativeZ;
		buttons_from = X_CONFIG;
	}
	if (priv->positiveZ > priv->buttons) {
		priv->buttons = priv->positiveZ;
		buttons_from = X_CONFIG;
	}
	priv->negativeW =  priv->positiveW = WS_NOZMAP;
	s = xf86SetStrOption(pInfo->options, "WAxisMapping", NULL);
	if (s) {
		int b1, b2;

		if (sscanf(s, "%d %d", &b1, &b2) == 2 &&
		    b1 > 0 && b1 <= NBUTTONS &&
		    b2 > 0 && b2 <= NBUTTONS) {
			priv->negativeW = b1;
			priv->positiveW = b2;
			xf86Msg(X_CONFIG,
			    "%s: WAxisMapping: buttons %d and %d\n",
			    pInfo->name, b1, b2);
		} else {
			xf86Msg(X_WARNING, "%s: invalid WAxisMapping value: "
			    "\"%s\"\n", pInfo->name, s);
		}
	}
	if (priv->negativeW > priv->buttons) {
		priv->buttons = priv->negativeW;
		buttons_from = X_CONFIG;
	}
	if (priv->positiveW > priv->buttons) {
		priv->buttons = priv->positiveW;
		buttons_from = X_CONFIG;
	}

	priv->screen_no = xf86SetIntOption(pInfo->options, "ScreenNo", 0);
	xf86Msg(X_CONFIG, "%s associated screen: %d\n",
	    dev->identifier, priv->screen_no);
	if (priv->screen_no >= screenInfo.numScreens ||
	    priv->screen_no < 0) {
		priv->screen_no = 0;
	}


	priv->swap_axes = xf86SetBoolOption(pInfo->options, "SwapXY", 0);
	if (priv->swap_axes) {
		xf86Msg(X_CONFIG,
		    "%s device will work with X and Y axes swapped\n",
		    dev->identifier);
	}
	priv->inv_x = 0;
	priv->inv_y = 0;
	s = xf86FindOptionValue(pInfo->options, "Rotate");
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
			xf86Msg(X_ERROR, "\"%s\" is not a valid value "
				"for Option \"Rotate\"\n", s);
			xf86Msg(X_ERROR, "Valid options are \"CW\", \"CCW\","
				" or \"UD\"\n");
		}
	}
	priv->raw = xf86SetBoolOption(pInfo->options, "Raw", 0);
	if (wsOpen(pInfo) != Success) {
		goto fail;
	}
	if (ioctl(pInfo->fd, WSMOUSEIO_GTYPE, &priv->type) != 0) {
		wsClose(pInfo);
		goto fail;
	}
	if (priv->type == WSMOUSE_TYPE_TPANEL)
		pInfo->type_name = XI_TOUCHSCREEN;
	else {
		pInfo->type_name = XI_MOUSE;
		if (priv->raw) {
			xf86Msg(X_WARNING, "Device is not a touch panel,"
			    "ignoring 'Option \"Raw\"'\n");
			priv->raw = 0;
		}
	}
	if (priv->raw) {
		xf86Msg(X_CONFIG,
		    "%s device will work in raw mode\n",
		    dev->identifier);
	}

	if (priv->type == WSMOUSE_TYPE_TPANEL && priv->raw) {
		if (ioctl(pInfo->fd, WSMOUSEIO_GCALIBCOORDS, &coords) != 0) {
			xf86Msg(X_ERROR, "GCALIBCOORS failed %s\n",
			    strerror(errno));
			wsClose(pInfo);
			goto fail;
		}

		/* get default coordinate space from kernel */
		priv->min_x = coords.minx;
		priv->max_x = coords.maxx;
		priv->min_y = coords.miny;
		priv->max_y = coords.maxy;
	} else {
		/* in calibrated mode, coordinate space, is screen coords */
		priv->min_x = 0;
		priv->max_x = screenInfo.screens[priv->screen_no]->width - 1;
		priv->min_y = 0;
		priv->max_y = screenInfo.screens[priv->screen_no]->height - 1;
	}
	/* Allow options to override this */
	priv->min_x = xf86SetIntOption(pInfo->options, "MinX", priv->min_x);
	xf86Msg(X_INFO, "%s minimum x position: %d\n",
	    dev->identifier, priv->min_x);
	priv->max_x = xf86SetIntOption(pInfo->options, "MaxX", priv->max_x);
	xf86Msg(X_INFO, "%s maximum x position: %d\n",
	    dev->identifier, priv->max_x);
	priv->min_y = xf86SetIntOption(pInfo->options, "MinY", priv->min_y);
	xf86Msg(X_INFO, "%s minimum y position: %d\n",
	    dev->identifier, priv->min_y);
	priv->max_y = xf86SetIntOption(pInfo->options, "MaxY", priv->max_y);
	xf86Msg(X_INFO, "%s maximum y position: %d\n",
	    dev->identifier, priv->max_y);

	pInfo->name = dev->identifier;
	pInfo->device_control = wsProc;
	pInfo->read_input = wsReadInput;
	pInfo->control_proc = wsChangeControl;
	pInfo->switch_mode = wsSwitchMode;
	pInfo->conversion_proc = NULL;
	pInfo->reverse_conversion_proc = NULL;
	pInfo->private = priv;
	pInfo->old_x = -1;
	pInfo->old_y = -1;
	xf86Msg(buttons_from, "%s: Buttons: %d\n", pInfo->name, priv->buttons);

	wsClose(pInfo);

	/* mark the device configured */
	pInfo->flags |= XI86_CONFIGURED;
	return pInfo;
fail:
	if (priv != NULL)
		xfree(priv);
	if (pInfo != NULL)
		xfree(pInfo);
	return NULL;
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
		xf86Msg(X_ERROR, "WS: unknown command %d\n", what);
		return !Success;
	} /* switch */
	return Success;
} /* wsProc */

static int
wsDeviceInit(DeviceIntPtr pWS)
{
	InputInfoPtr pInfo = (InputInfoPtr)pWS->public.devicePrivate;
	WSDevicePtr priv = (WSDevicePtr)XI_PRIVATE(pWS);
	unsigned char map[NBUTTONS + 1];
	int i, xmin, xmax, ymin, ymax;
#if GET_ABI_MAJOR(ABI_XINPUT_VERSION) >= 7
	Atom btn_labels[NBUTTONS] = {0};
	Atom axes_labels[NAXES] = {0};
#endif

	DBG(1, ErrorF("WS DEVICE_INIT\n"));

#if GET_ABI_MAJOR(ABI_XINPUT_VERSION) >= 7
	btn_labels[0] = XIGetKnownProperty(BTN_LABEL_PROP_BTN_LEFT);
	btn_labels[1] = XIGetKnownProperty(BTN_LABEL_PROP_BTN_MIDDLE);
	btn_labels[2] = XIGetKnownProperty(BTN_LABEL_PROP_BTN_RIGHT);
#endif
	priv->screen_width = screenInfo.screens[priv->screen_no]->width;
	priv->screen_height = screenInfo.screens[priv->screen_no]->height;
	
	for (i = 0; i < NBUTTONS; i++)
		map[i + 1] = i + 1;
	if (!InitButtonClassDeviceStruct(pWS,
		min(priv->buttons, NBUTTONS),
#if GET_ABI_MAJOR(ABI_XINPUT_VERSION) >= 7
		btn_labels,
#endif
		map))
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

#if GET_ABI_MAJOR(ABI_XINPUT_VERSION) >= 7
	if ((priv->type == WSMOUSE_TYPE_TPANEL)) {
		axes_labels[0] = XIGetKnownProperty(AXIS_LABEL_PROP_ABS_X);
		axes_labels[1] = XIGetKnownProperty(AXIS_LABEL_PROP_ABS_Y);
	} else {
		axes_labels[0] = XIGetKnownProperty(AXIS_LABEL_PROP_REL_X);
		axes_labels[1] = XIGetKnownProperty(AXIS_LABEL_PROP_REL_Y);
	}
#endif
	if (!InitValuatorClassDeviceStruct(pWS,
		NAXES,
#if GET_ABI_MAJOR(ABI_XINPUT_VERSION) >= 7
		axes_labels,
#endif
#if GET_ABI_MAJOR(ABI_XINPUT_VERSION) < 3
		xf86GetMotionEvents,
#endif
		GetMotionHistorySize(),
		priv->type == WSMOUSE_TYPE_TPANEL ?
		Absolute : Relative))
		return !Success;
	if (!InitPtrFeedbackClassDeviceStruct(pWS, wsControlProc))
		return !Success;
	
	xf86InitValuatorAxisStruct(pWS, 0,
#if GET_ABI_MAJOR(ABI_XINPUT_VERSION) >= 7
	    axes_labels[0],
#endif
	    xmin, xmax, 1, 0, 1);
	xf86InitValuatorDefaults(pWS, 0);
	
	xf86InitValuatorAxisStruct(pWS, 1,
#if GET_ABI_MAJOR(ABI_XINPUT_VERSION) >= 7
	    axes_labels[1],
#endif
	    ymin, ymax, 1, 0, 1);
	xf86InitValuatorDefaults(pWS, 1);
	xf86MotionHistoryAllocate(pInfo);
	AssignTypeAndName(pWS, pInfo->atom, pInfo->name);
	pWS->public.on = FALSE;
	/* This should correspond to the center of the screen */
	priv->x = (priv->max_x - priv->min_x) / 2;
	priv->y = (priv->max_y - priv->min_y) / 2;
	if (wsOpen(pInfo) != Success) {
		return !Success;
	}
	return Success;
}

static int
wsDeviceOn(DeviceIntPtr pWS)
{
	InputInfoPtr pInfo = (InputInfoPtr)pWS->public.devicePrivate;
	WSDevicePtr priv = (WSDevicePtr)XI_PRIVATE(pWS);
	struct wsmouse_calibcoords coords;
	int raw;

	DBG(1, ErrorF("WS DEVICE ON\n"));
	if ((pInfo->fd < 0) && (wsOpen(pInfo) != Success)) {
		xf86Msg(X_ERROR, "wsOpen failed %s\n",
		    strerror(errno));
			return !Success;
	}

	if (priv->type == WSMOUSE_TYPE_TPANEL) {
		/* Set raw mode */
		raw = priv->raw;
		if (ioctl(pInfo->fd, WSMOUSEIO_GCALIBCOORDS, &coords) != 0) {
			xf86Msg(X_ERROR, "GCALIBCOORS failed %s\n",
			    strerror(errno));
			return !Success;
		}
		priv->raw = coords.samplelen;
		coords.samplelen = raw;
		if (ioctl(pInfo->fd, WSMOUSEIO_SCALIBCOORDS, &coords) != 0) {
			xf86Msg(X_ERROR, "GCALIBCOORS failed %s\n",
			    strerror(errno));
			return !Success;
		}
	}
	priv->buffer = XisbNew(pInfo->fd,
	    sizeof(struct wscons_event) * NUMEVENTS);
	if (priv->buffer == NULL) {
		xf86Msg(X_ERROR, "cannot alloc xisb buffer\n");
		wsClose(pInfo);
		return !Success;
	}
	xf86AddEnabledDevice(pInfo);
	pWS->public.on = TRUE;
	return Success;
}

static void
wsDeviceOff(DeviceIntPtr pWS)
{
	InputInfoPtr pInfo = (InputInfoPtr)pWS->public.devicePrivate;
	WSDevicePtr priv = (WSDevicePtr)XI_PRIVATE(pWS);
	struct wsmouse_calibcoords coords;
	int raw;

	DBG(1, ErrorF("WS DEVICE OFF\n"));
	if (priv->type == WSMOUSE_TYPE_TPANEL) {
		/* Restore raw mode */
		raw = priv->raw;
		if (ioctl(pInfo->fd, WSMOUSEIO_GCALIBCOORDS, &coords) != 0) {
			xf86Msg(X_ERROR, "GCALIBCOORS failed %s\n",
			    strerror(errno));
		}
		priv->raw = coords.samplelen;
		coords.samplelen = raw;
		if (ioctl(pInfo->fd, WSMOUSEIO_SCALIBCOORDS, &coords) != 0) {
			xf86Msg(X_ERROR, "GCALIBCOORS failed %s\n",
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
	WSDevicePtr priv;
	static struct wscons_event eventList[NUMEVENTS];
	int n, c;
	struct wscons_event *event = eventList;
	unsigned char *pBuf;
	int ax, ay;

	priv = pInfo->private;

	XisbBlockDuration(priv->buffer, -1);
	pBuf = (unsigned char *)eventList;
	n = 0;
	while (n < sizeof(eventList) && (c = XisbRead(priv->buffer)) >= 0) {
		pBuf[n++] = (unsigned char)c;
	}

	if (n == 0)
		return;

	n /= sizeof(struct wscons_event);
	while( n-- ) {
		int buttons = priv->lastButtons;
		int dx = 0, dy = 0, dz = 0, dw = 0;
		int zbutton = 0, wbutton = 0;

		ax = 0; ay = 0;
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
			dx = event->value;
			DBG(4, ErrorF("Relative X %d\n", event->value));
			break;
		case WSCONS_EVENT_MOUSE_DELTA_Y:
			dy = -event->value;
			DBG(4, ErrorF("Relative Y %d\n", event->value));
			break;
		case WSCONS_EVENT_MOUSE_ABSOLUTE_X:
			DBG(4, ErrorF("Absolute X %d\n", event->value));
			if (event->value != 4095) {
				ax = event->value;
				if (priv->inv_x)
					ax = priv->max_x - ax + priv->min_x;
			}
			break;
		case WSCONS_EVENT_MOUSE_ABSOLUTE_Y:
			DBG(4, ErrorF("Absolute Y %d\n", event->value));
			ay = event->value;
			if (priv->inv_y)
				ay = priv->max_y - ay + priv->min_y;
			break;
#ifdef WSCONS_EVENT_MOUSE_DELTA_Z
		case WSCONS_EVENT_MOUSE_DELTA_Z:
			DBG(4, ErrorF("Relative Z %d\n", event->value));
			dz = event->value;
			break;
#endif
#ifdef WSCONS_EVENT_MOUSE_ABSOLUTE_Z
		case WSCONS_EVENT_MOUSE_ABSOLUTE_Z:
			/* ignore those */
			++event;
			continue;
			break;
#endif
#ifdef WSCONS_EVENT_MOUSE_DELTA_W
		case WSCONS_EVENT_MOUSE_DELTA_W:
			DBG(4, ErrorF("Relative W %d\n", event->value));
			dw = event->value;
			break;
#endif
		default:
			xf86Msg(X_WARNING, "%s: bad wsmouse event type=%d\n",
			    pInfo->name, event->type);
			++event;
			continue;
		} /* case */

		if (dx || dy) {
			/* relative motion event */
			DBG(3, ErrorF("postMotionEvent dX %d dY %d\n",
				      dx, dy));
			xf86PostMotionEvent(pInfo->dev, 0, 0, 2,
			    dx, dy);
		}
		if (dz && priv->negativeZ != WS_NOZMAP
		    && priv->positiveZ != WS_NOZMAP) {
			buttons &= ~(priv->negativeZ | priv->positiveZ);
			if (dz < 0) {
				DBG(4, ErrorF("Z -> button %d\n",
					priv->negativeZ));
				zbutton = 1 << (priv->negativeZ - 1);
			} else {
				DBG(4, ErrorF("Z -> button %d\n",
					priv->positiveZ));
				zbutton = 1 << (priv->positiveZ - 1);
			}
			buttons |= zbutton;
			dz = 0;
		}
		if (dw && priv->negativeW != WS_NOZMAP
		    && priv->positiveW != WS_NOZMAP) {
			buttons &= ~(priv->negativeW | priv->positiveW);
			if (dw < 0) {
				DBG(4, ErrorF("W -> button %d\n",
					priv->negativeW));
				wbutton = 1 << (priv->negativeW - 1);
			} else {
				DBG(4, ErrorF("W -> button %d\n",
					priv->positiveW));
				wbutton = 1 << (priv->positiveW - 1);
			}
			buttons |= wbutton;
			dw = 0;
		}
		if (priv->lastButtons != buttons) {
			/* button event */
			wsSendButtons(pInfo, buttons);
		}
		if (zbutton != 0) {
			/* generate a button up event */
			buttons &= ~zbutton;
			wsSendButtons(pInfo, buttons);
		}
		if (ax) {
			/* absolute position event */
			DBG(3, ErrorF("postMotionEvent X %d\n", ax));
			xf86PostMotionEvent(pInfo->dev, 1, 0, 1, ax);
			priv->x = ax;
		}
		if (ay) {
			/* absolute position event */
			DBG(3, ErrorF("postMotionEvent y %d\n", ay));
			xf86PostMotionEvent(pInfo->dev, 1, 1, 1, ay);
			priv->y = ay;
		}
		++event;
	}
	return;
} /* wsReadInput */

static void
wsSendButtons(InputInfoPtr pInfo, int buttons)
{
	WSDevicePtr priv = (WSDevicePtr)pInfo->private;
	int button, mask;

	for (button = 1; button < NBUTTONS; button++) {
		mask = 1 << (button - 1);
		if ((mask & priv->lastButtons) != (mask & buttons)) {
			xf86PostButtonEvent(pInfo->dev, TRUE,
			    button, (buttons & mask) != 0,
					    0, 0);
			DBG(3, ErrorF("post button event %d %d\n",
				button, (buttons & mask) != 0))
		}
	} /* for */
	priv->lastButtons = buttons;
} /* wsSendButtons */


static int
wsChangeControl(InputInfoPtr pInfo, xDeviceCtl *control)
{
	return BadMatch;
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
	    xf86Msg(X_ERROR, "%s: cannot open input device\n", pInfo->name);
	    return !Success;
	}
#ifdef __NetBSD__
	if (ioctl(pInfo->fd, WSMOUSEIO_SETVERSION, &version) == -1) {
		xf86Msg(X_ERROR, "%s: cannot set wsmouse event version\n",
		    pInfo->name);
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
	InputInfoPtr pInfo = device->public.devicePrivate;
	WSDevicePtr priv = (WSDevicePtr)pInfo->private;

	DBG(1, ErrorF("wsControlProc\n"));
	priv->num = ctrl->num;
	priv->den = ctrl->den;
	priv->threshold = ctrl->threshold;
}
