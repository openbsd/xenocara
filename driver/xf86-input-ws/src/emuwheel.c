/*	$OpenBSD: emuwheel.c,v 1.3 2015/12/25 15:08:28 matthieu Exp $	*/
/*
* Copyright 1990,91 by Thomas Roell, Dinkelscherben, Germany.
* Copyright 1993 by David Dawes <dawes@xfree86.org>
* Copyright 2002 by SuSE Linux AG, Author: Egbert Eich
* Copyright 1994-2002 by The XFree86 Project, Inc.
* Copyright 2002 by Paul Elliott
* (Ported from xf86-input-mouse, above copyrights taken from there)
* Copyright 2008 by Chris Salch
* Copyright © 2008 Red Hat, Inc.
*
* Permission to use, copy, modify, distribute, and sell this software
* and its documentation for any purpose is hereby granted without
* fee, provided that the above copyright notice appear in all copies
* and that both that copyright notice and this permission notice
* appear in supporting documentation, and that the name of the authors
* not be used in advertising or publicity pertaining to distribution of the
* software without specific, written prior permission.  The authors make no
* representations about the suitability of this software for any
* purpose.  It is provided "as is" without express or implied
* warranty.
*
* THE AUTHORS DISCLAIM ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
* INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN
* NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY SPECIAL, INDIRECT OR
* CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS
* OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT,
* NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
* CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*
*/

/* Mouse wheel emulation code. */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <sys/time.h>

#include <xorg-server.h>
#include <X11/Xatom.h>
#include <xf86.h>
#include <xf86_OSproc.h>
#include <X11/extensions/XI.h>
#include <X11/extensions/XIproto.h>
#include <xf86Xinput.h>
#include <exevents.h>

#include <ws-properties.h>

#include "ws.h"

static Atom prop_wheel_emu;
static Atom prop_wheel_axismap;
static Atom prop_wheel_inertia;
static Atom prop_wheel_timeout;
static Atom prop_wheel_button;

static int wsWheelEmuInertia(InputInfoPtr, WheelAxisPtr, int);
static int wsWheelEmuSetProperty(DeviceIntPtr, Atom, XIPropertyValuePtr, BOOL);

BOOL
wsWheelEmuFilterButton(InputInfoPtr pInfo, unsigned int button, int press)
{
	WSDevicePtr priv = (WSDevicePtr)pInfo->private;
	int ms;

	if (!priv->emulateWheel.enabled)
		return FALSE;

	if (priv->emulateWheel.button == button) {
		priv->emulateWheel.button_state = press;

		if (press) {
			priv->emulateWheel.expires = GetTimeInMillis() +
			    priv->emulateWheel.timeout;
		} else {
			ms = priv->emulateWheel.expires - GetTimeInMillis();

			if (ms > 0) {
				/*
				 * If the button is released early enough emit
				 * the button press/release events
				 */
				wsButtonClicks(pInfo, button, 1);
			}
		}

		return TRUE;
	}

	return FALSE;
}

BOOL
wsWheelEmuFilterMotion(InputInfoPtr pInfo, int dx, int dy)
{
	WSDevicePtr priv = (WSDevicePtr)pInfo->private;
	WheelAxisPtr pAxis = NULL, pOtherAxis = NULL;
	int value;

	if (!priv->emulateWheel.enabled)
		return FALSE;

	/*
	 * Handle our motion events if the emuWheel button is pressed.
	 * Wheel button of 0 means always emulate wheel.
	 */
	if (priv->emulateWheel.button_state || priv->emulateWheel.button == 0) {
		if (priv->emulateWheel.button) {
			int ms = priv->emulateWheel.expires - GetTimeInMillis();

			if (ms > 0)
				return TRUE;
		}

		if (dx) {
			pAxis = &(priv->emulateWheel.X);
			pOtherAxis = &(priv->emulateWheel.Y);
			value = dx;
		} else if (dy) {
			pAxis = &(priv->emulateWheel.Y);
			pOtherAxis = &(priv->emulateWheel.X);
			value = dy;
		} else
			return FALSE;

		/*
		 * Reset the inertia of the other axis when a scroll event
		 * was sent to avoid the buildup of erroneous scroll events if the
		 * user doesn't move in a perfectly straight line.
		 */
		if (pAxis) {
			if (wsWheelEmuInertia(pInfo, pAxis, value))
				pOtherAxis->traveled_distance = 0;
		}

		return TRUE;
	}

	return FALSE;
}

static int
wsWheelEmuInertia(InputInfoPtr pInfo, WheelAxisPtr axis, int value)
{
	WSDevicePtr priv = (WSDevicePtr)pInfo->private;
	int button, inertia;
	int rc = 0;

	if (axis->negative == WS_NOMAP)
		return rc;

	axis->traveled_distance += value;

	if (axis->traveled_distance < 0) {
		button = axis->negative;
		inertia = -priv->emulateWheel.inertia;
	} else {
		button = axis->positive;
		inertia = priv->emulateWheel.inertia;
	}

	while (abs(axis->traveled_distance) > priv->emulateWheel.inertia) {
		axis->traveled_distance -= inertia;
		wsButtonClicks(pInfo, button, 1);
		rc++;
	}

	return rc;
}

void
wsWheelEmuPreInit(InputInfoPtr pInfo)
{
	WSDevicePtr priv = (WSDevicePtr)pInfo->private;
	int button, inertia, timeout;

	priv->emulateWheel.enabled = xf86SetBoolOption(pInfo->options,
	    "EmulateWheel", FALSE);

	button = xf86SetIntOption(pInfo->options, "EmulateWheelButton", 4);
	if (button < 0 || button > NBUTTONS) {
		xf86IDrvMsg(pInfo, X_WARNING,
		    "Invalid EmulateWheelButton value: %d\n", button);
		xf86IDrvMsg(pInfo, X_WARNING, "Wheel emulation disabled\n");
		priv->emulateWheel.enabled = FALSE;
		button = 4;
	}
	priv->emulateWheel.button = button;

	inertia = xf86SetIntOption(pInfo->options, "EmulateWheelInertia", 10);
	if (inertia <= 0) {
		xf86IDrvMsg(pInfo, X_WARNING,
		    "Invalid EmulateWheelInertia value: %d\n", inertia);
		xf86IDrvMsg(pInfo, X_WARNING, "Using built-in inertia value\n");
		inertia = 10;
	}
	priv->emulateWheel.inertia = inertia;

	timeout = xf86SetIntOption(pInfo->options, "EmulateWheelTimeout", 200);
	if (timeout < 0) {
		xf86IDrvMsg(pInfo, X_WARNING,
		    "Invalid EmulateWheelTimeout value: %d\n", timeout);
		xf86IDrvMsg(pInfo, X_WARNING, "Using built-in timeout value\n");
		timeout = 200;
	}
	priv->emulateWheel.timeout = timeout;

	wsWheelHandleButtonMap(pInfo, &(priv->emulateWheel.Y),
	    "YAxisMapping", "4 5");
	wsWheelHandleButtonMap(pInfo, &(priv->emulateWheel.X),
	    "XAxisMapping", NULL);
}

static int
wsWheelEmuSetProperty(DeviceIntPtr dev, Atom atom, XIPropertyValuePtr val,
    BOOL checkonly)
{
	InputInfoPtr pInfo = (InputInfoPtr)dev->public.devicePrivate;
	WSDevicePtr priv = (WSDevicePtr)pInfo->private;

	if (atom == prop_wheel_emu) {
		if (val->format != 8 || val->size != 1 ||
		    val->type != XA_INTEGER)
			return BadMatch;

		if (!checkonly)
			priv->emulateWheel.enabled = *((BOOL*)val->data);
	} else if (atom == prop_wheel_button) {
		int button;

		if (val->format != 8 || val->size != 1 ||
		    val->type != XA_INTEGER)
			return BadMatch;

		button = *((CARD8*)val->data);

		if (button < 0 || button > NBUTTONS)
			return BadValue;

		if (!checkonly)
			priv->emulateWheel.button = button;
	} else if (atom == prop_wheel_axismap) {
		int x_negative, x_positive;
		int y_negative, y_positive;

		if (val->format != 8 || val->size != 4 ||
		    val->type != XA_INTEGER)
			return BadMatch;

		x_negative = *((CARD8*)val->data);
		x_positive = *(((CARD8*)val->data) + 1);
		y_negative = *(((CARD8*)val->data) + 2);
		y_positive = *(((CARD8*)val->data) + 3);

		if (x_negative < 0 || x_negative > NBUTTONS ||
		    x_positive < 0 || x_positive > NBUTTONS ||
		    y_negative < 0 || y_negative > NBUTTONS ||
		    y_positive < 0 || y_positive > NBUTTONS)
			return BadValue;

		if ((x_negative == WS_NOMAP && x_positive != WS_NOMAP) ||
		    (x_negative != WS_NOMAP && x_positive == WS_NOMAP) ||
		    (y_negative == WS_NOMAP && y_positive != WS_NOMAP) ||
		    (y_negative != WS_NOMAP && y_positive == WS_NOMAP))
			return BadValue;

		if (!checkonly) {
			priv->emulateWheel.X.negative = x_negative;
			priv->emulateWheel.X.positive = x_positive;
			priv->emulateWheel.Y.negative = y_negative;
			priv->emulateWheel.Y.positive = y_positive;
		}
	} else if (atom == prop_wheel_inertia) {
		int inertia;

		if (val->format != 16 || val->size != 1 ||
		    val->type != XA_INTEGER)
			return BadMatch;

		inertia = *((CARD16*)val->data);

		if (inertia <= 0)
			return BadValue;

		if (!checkonly)
			priv->emulateWheel.inertia = inertia;
	} else if (atom == prop_wheel_timeout) {
		int timeout;

		if (val->format != 32 || val->size != 1 ||
		    val->type != XA_INTEGER)
			return BadMatch;

		timeout = *((CARD32*)val->data);

		if (timeout < 0)
			return BadValue;

		if (!checkonly)
			priv->emulateWheel.timeout = timeout;
	}

	return Success;
}

void
wsWheelEmuInitProperty(DeviceIntPtr dev)
{
	InputInfoPtr pInfo = (InputInfoPtr)dev->public.devicePrivate;
	WSDevicePtr priv = (WSDevicePtr)pInfo->private;
	char vals[4];
	int rc;

	prop_wheel_emu = MakeAtom(WS_PROP_WHEEL, strlen(WS_PROP_WHEEL), TRUE);
	rc = XIChangeDeviceProperty(dev, prop_wheel_emu, XA_INTEGER, 8,
	    PropModeReplace, 1, &priv->emulateWheel.enabled, FALSE);
	if (rc != Success) {
		xf86IDrvMsg(pInfo, X_ERROR,
		    "cannot create device property %s: %d\n",
		    WS_PROP_WHEEL, rc);
		return;
	}
	XISetDevicePropertyDeletable(dev, prop_wheel_emu, FALSE);

	vals[0] = priv->emulateWheel.X.negative;
	vals[1] = priv->emulateWheel.X.positive;
	vals[2] = priv->emulateWheel.Y.negative;
	vals[3] = priv->emulateWheel.Y.positive;

	prop_wheel_axismap = MakeAtom(WS_PROP_WHEEL_AXES,
	    strlen(WS_PROP_WHEEL_AXES), TRUE);
	rc = XIChangeDeviceProperty(dev, prop_wheel_axismap, XA_INTEGER, 8,
	    PropModeReplace, 4, vals, FALSE);
	if (rc != Success) {
		xf86IDrvMsg(pInfo, X_ERROR,
		    "cannot create device property %s: %d\n",
		    WS_PROP_WHEEL_AXES, rc);
		return;
	}
	XISetDevicePropertyDeletable(dev, prop_wheel_axismap, FALSE);

	prop_wheel_inertia = MakeAtom(WS_PROP_WHEEL_INERTIA,
	    strlen(WS_PROP_WHEEL_INERTIA), TRUE);
	rc = XIChangeDeviceProperty(dev, prop_wheel_inertia, XA_INTEGER, 16,
	    PropModeReplace, 1, &priv->emulateWheel.inertia, FALSE);
	if (rc != Success) {
		xf86IDrvMsg(pInfo, X_ERROR,
		    "cannot create device property %s: %d\n",
		    WS_PROP_WHEEL_INERTIA, rc);
		return;
	}
	XISetDevicePropertyDeletable(dev, prop_wheel_inertia, FALSE);

	prop_wheel_timeout = MakeAtom(WS_PROP_WHEEL_TIMEOUT,
	    strlen(WS_PROP_WHEEL_TIMEOUT), TRUE);
	rc = XIChangeDeviceProperty(dev, prop_wheel_timeout, XA_INTEGER, 32,
	    PropModeReplace, 1, &priv->emulateWheel.timeout, FALSE);
	if (rc != Success) {
		xf86IDrvMsg(pInfo, X_ERROR,
		    "cannot create device property %s: %d\n",
		    WS_PROP_WHEEL_TIMEOUT, rc);
		return;
	}
	XISetDevicePropertyDeletable(dev, prop_wheel_timeout, FALSE);

	prop_wheel_button = MakeAtom(WS_PROP_WHEEL_BUTTON,
	    strlen(WS_PROP_WHEEL_BUTTON), TRUE);
	rc = XIChangeDeviceProperty(dev, prop_wheel_button, XA_INTEGER, 8,
	    PropModeReplace, 1, &priv->emulateWheel.button, FALSE);
	if (rc != Success) {
		xf86IDrvMsg(pInfo, X_ERROR,
		    "cannot create device property %s: %d\n",
		    WS_PROP_WHEEL_BUTTON, rc);
		return;
	}
	XISetDevicePropertyDeletable(dev, prop_wheel_button, FALSE);

	XIRegisterPropertyHandler(dev, wsWheelEmuSetProperty, NULL, NULL);
}
