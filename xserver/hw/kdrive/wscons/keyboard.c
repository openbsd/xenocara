/* $OpenBSD: keyboard.c,v 1.3 2007/05/27 05:17:06 matthieu Exp $ */
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
#include "kdrive.h"

#include <X11/keysym.h>

#include <errno.h>
#include <dev/wscons/wsconsio.h>

#define DBG(x) ErrorF x

extern int WsconsConsoleFd;
static int WsconsKbdType;

static void
wskbdLoad(void)
{
	DBG(("wskbdLoad\n"));

	/* Read kernel Mapping */
}

#define NUM_EVENTS 64

static void
wskbdRead(int fd, void *closure)
{
	struct wscons_event events[NUM_EVENTS];
	int i, n, type;
	unsigned char b;

	DBG(("wskbdRead\n"));
	if ((n = read(WsconsConsoleFd, events, sizeof(events))) > 0) {
		n /= sizeof(struct wscons_event);
		for (i = 0; i < n; i++) {
			type = events[i].type;
			if (type == WSCONS_EVENT_KEY_UP || 
			    type == WSCONS_EVENT_KEY_DOWN) {
				
				KdEnqueueKeyboardEvent(events[i].value, 
				    type == WSCONS_EVENT_KEY_DOWN ? 
				    TRUE : FALSE);
			}
		} /* for */
	}
}

static int
wskbdEnable(int fd, void *closure)
{
	int option = WSKBD_RAW;
	
	DBG(("wskbdEnable\n"));
	/* Switch to X mode */
	if (ioctl(fd, WSKBDIO_SETMODE, &option) == -1) {
		ErrorF("wskbdEnable: WSKBDIO_SETMODE: %d\n", errno);
		return -1;
	}
	return fd;
}

static void
wskbdDisable(int fd, void *closure)
{
	int option = WSKBD_TRANSLATED;

	DBG(("wskbdDisable\n"));
	/* Back to console mode */
	ioctl(fd, WSKBDIO_SETMODE, &option);
}

static int
wskbdInit(void)
{
	DBG(("wskbdInit\n"));
	if (!WsconsKbdType)
		WsconsKbdType = KdAllocInputType();
	KdRegisterFd(WsconsKbdType, WsconsConsoleFd, wskbdRead, 0);
	wskbdEnable(WsconsConsoleFd, 0);
	KdRegisterFdEnableDisable(WsconsConsoleFd,
	    wskbdEnable, wskbdDisable);
	return 1;
}

static void
wskbdFini(void)
{
	DBG(("wskbdFini\n"));
	wskbdDisable(WsconsConsoleFd, 0);
	KdUnregisterFds(WsconsKbdType, FALSE);
}

static void
wskbdLeds(int leds)
{
	DBG(("wskbdLeds %d\n", leds));
	if (ioctl(WsconsConsoleFd, WSKBDIO_SETLEDS, &leds) == -1)
		ErrorF("wskbd WSKBDIO_SETLEDS: %s\n", 
		    strerror(errno));
}

static void
wskbdBell(int volume, int pitch, int duration)
{
	struct wskbd_bell_data wsb;

	DBG(("wskbdBell volume %d pictch %d duration %d\n",
		volume, pitch, duration));
	wsb.which = WSKBD_BELL_DOALL;
	wsb.pitch = pitch;
	wsb.period = duration;
	wsb.volume = volume;
	if (ioctl(WsconsConsoleFd, WSKBDIO_COMPLEXBELL, &wsb) == -1)
		ErrorF("WsconsKeyboardBell: %s\n", strerror(errno));
}
	
KdKeyboardFuncs WsconsKeyboardFuncs = {
	wskbdLoad,
	wskbdInit,
	wskbdLeds,
	wskbdBell,
	wskbdFini,
	3,
};
