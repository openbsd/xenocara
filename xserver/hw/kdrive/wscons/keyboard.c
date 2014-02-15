/* $OpenBSD: keyboard.c,v 1.5 2014/02/15 15:00:28 matthieu Exp $ */
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
#include <sys/ioctl.h>
#include <dev/wscons/wsconsio.h>

#define DBG(x) ErrorF x

extern int WsconsConsoleFd;

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

	DBG(("wskbdRead\n"));
	if ((n = read(fd, events, sizeof(events))) > 0) {
		n /= sizeof(struct wscons_event);
		for (i = 0; i < n; i++) {
			type = events[i].type;
			if (type == WSCONS_EVENT_KEY_UP || 
			    type == WSCONS_EVENT_KEY_DOWN) {
				
				KdEnqueueKeyboardEvent(closure, 
				    events[i].value, 
				    type == WSCONS_EVENT_KEY_DOWN ? 
				    TRUE : FALSE);
			}
		} /* for */
	}
}

static int
wskbdEnable(KdKeyboardInfo *ki)
{
	int option = WSKBD_RAW;
	int fd = WsconsConsoleFd;
	
	DBG(("wskbdEnable\n"));
	if (ki == NULL) 
		return !Success;
	ki->driverPrivate = (void *)(intptr_t)fd;

	/* Switch to X mode */
	if (ioctl(fd, WSKBDIO_SETMODE, &option) == -1) {
		ErrorF("wskbdEnable: WSKBDIO_SETMODE: %d\n", errno);
		return -1;
	}
	KdRegisterFd (fd, wskbdRead, ki);
	return Success;
}

static void
wskbdDisable(KdKeyboardInfo *ki)
{
	int option = WSKBD_TRANSLATED;
	int fd;

	DBG(("wskbdDisable\n"));
	if (ki == NULL)
		return;
	fd = (int)(intptr_t)ki->driverPrivate;
	/* Back to console mode */
	ioctl(fd, WSKBDIO_SETMODE, &option);
}

static int
wskbdInit(KdKeyboardInfo *ki)
{
	DBG(("wskbdInit\n"));
	if (!ki)
		return !Success;

	if (ki->path)
		free(ki->path);
	ki->path = strdup("console");
	if (ki->name)
		free(ki->name);
	ki->name = strdup("Wscons keyboard");
	
	wskbdLoad();
	return Success;
}


static void
wskbdLeds(KdKeyboardInfo *ki, int leds)
{
	DBG(("wskbdLeds %d\n", leds));
	if (!ki)
		return;
	if (ioctl(WsconsConsoleFd, WSKBDIO_SETLEDS, &leds) == -1)
		ErrorF("wskbd WSKBDIO_SETLEDS: %s\n", 
		    strerror(errno));
}
	
KdKeyboardDriver WsconsKeyboardDriver = {
	"keyboard",
	.Init = wskbdInit,
	.Enable = wskbdEnable,
	.Leds = wskbdLeds,
	.Disable = wskbdDisable
};
