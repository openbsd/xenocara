/* $OpenBSD: mouse.c,v 1.5 2007/12/23 14:28:10 matthieu Exp $ */
/*
 * Copyright (c) 2007 Matthieu Herrb <matthieu@openbsd.org>
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

#ifdef HAVE_CONFIG_H
#include <kdrive-config.h>
#endif
#define NEED_EVENTS
#include <errno.h>
#include <termios.h>
#include <X11/X.h>
#include <X11/Xproto.h>
#include <dev/wscons/wsconsio.h>

#include "inputstr.h"
#include "scrnintstr.h"
#include "kdrive.h"

#define DBG(x) ErrorF x

#define NUMEVENTS 64

static unsigned long kdbuttons[] = {
	KD_BUTTON_1,
	KD_BUTTON_2,
	KD_BUTTON_3
};

static void
wsmouseRead(int mousePort, void *closure)
{
	KdPointerInfo *pi = closure;
	static struct wscons_event eventList[NUMEVENTS];
	struct wscons_event *event = eventList;
	int n;

	n = read(mousePort, &eventList, 
	    NUMEVENTS * sizeof(struct wscons_event));
	if (n <= 0)
		return;
	n /= sizeof(struct wscons_event);
	while (n--) {
		int dx = 0, dy = 0;
		unsigned long flags = 0;
		
		switch(event->type) {
		case WSCONS_EVENT_MOUSE_UP:
			flags &= ~kdbuttons[event->value];
			break;
		case WSCONS_EVENT_MOUSE_DOWN:
			flags |= kdbuttons[1<<event->value];
			break;
		case WSCONS_EVENT_MOUSE_DELTA_X:
			dx = event->value;
			flags |= KD_MOUSE_DELTA;
			break;
		case WSCONS_EVENT_MOUSE_DELTA_Y:
			dy = event->value;
			flags |= KD_MOUSE_DELTA;
			break;
		case WSCONS_EVENT_MOUSE_ABSOLUTE_X:
			dx = event->value;
			break;
		case WSCONS_EVENT_MOUSE_ABSOLUTE_Y:
			dy = event->value;
			break;
		default:
			ErrorF("wsmouseRead: bad wsmouse event type=%d\n",
			    event->type);
			continue;
		} /* case */
		KdEnqueuePointerEvent(pi, flags, dx, dy, 0);
	}	
}

int MouseInputType;

static Status
wsmouseInit(KdPointerInfo *pi)
{
	char *device = "/dev/wsmouse";

	DBG(("wsmouseInit\n"));

	if (pi->path == NULL)
		pi->path = KdSaveString(device);

	if (pi->name == NULL)
		pi->name = KdSaveString("Wscons mouse");
	return Success;
}

static Status
wsmouseEnable(KdPointerInfo *pi)
{
	int fd;

	DBG(("wsmouseEnable\n"));

	if (pi == NULL || pi->driverPrivate == NULL || pi->path == NULL)
		return BadImplementation;

	fd = open(pi->path, O_RDWR | O_NONBLOCK);
	if (fd < 0)
		return BadMatch;

	if (!KdRegisterFd(fd, wsmouseRead, pi)) {
		close(fd);
		return BadAlloc;
	}
	pi->driverPrivate = (void *)fd;
	return Success;
}

static void
wsmouseDisable(KdPointerInfo *pi)
{
	DBG(("wsmouseDisable\n"));
	KdUnregisterFd(pi, (int)pi->driverPrivate, TRUE);
}
		
static void
wsmouseFini(KdPointerInfo *pi)
{
	DBG(("wsmouseFini\n"));
}

KdPointerDriver WsconsMouseDriver = {
	"mouse",
	wsmouseInit,
	wsmouseEnable,
	wsmouseDisable,
	wsmouseFini,
	NULL,
};
