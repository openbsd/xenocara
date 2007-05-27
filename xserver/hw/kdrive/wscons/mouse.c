/* $OpenBSD: mouse.c,v 1.4 2007/05/27 05:17:06 matthieu Exp $ */
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
		KdEnqueueMouseEvent(kdMouseInfo, flags, dx, dy);
	}	
}

int MouseInputType;

static Bool
wsmouseInit(void)
{
	char *device = "/dev/wsmouse";
	int port;

	DBG(("wsmouseInit\n"));

	if (!MouseInputType)
		MouseInputType = KdAllocInputType();

	port = open(device, O_RDWR | O_NONBLOCK);
	if (port == -1) {
		ErrorF("wsmouseInit: couldn't open %s (%d)\n", device, errno);
		return FALSE;
	}
	return KdRegisterFd(MouseInputType, port, wsmouseRead, NULL);
}
		
static void
wsmouseFini(void)
{
	KdMouseInfo *mi;

	DBG(("wsmouseFini\n"));
	KdUnregisterFds(MouseInputType, TRUE);
}

KdMouseFuncs WsconsMouseFuncs = {
	wsmouseInit,
	wsmouseFini
};
