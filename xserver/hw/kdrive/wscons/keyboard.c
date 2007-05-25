/* $OpenBSD: keyboard.c,v 1.1 2007/05/25 15:33:32 matthieu Exp $ */
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

extern int WsconsConsoleFd;
static int WsconsKbdType;

static void
WsconsKeyboardLoad(void)
{
	/* Read kernel Mapping */
}

static void
WsconsKeyboardRead(int fd, void *closure)
{
	unsigned char b;

	/* read and enqueue events */
	KdEnqueueKeyboardEvent(b & 0x7f, b & 0x80);
}

static void
WsconsKeyboardEnable(int fd, void *closure)
{
	/* Switch to X mode */
}

static void
WsconsKeyboardDisable(int fd, void *closure)
{
	/* Back to console mode */
}

static int
WsconsKeyboardInit(void)
{
	if (!WsconsKbdType)
		WsconsKbdType = KdAllocInputType();
	KdRegisterFd(WsconsKbdType, WsconsConsoleFd, WsconsKeyboardRead, 0);
	WsconsKeyboardEnable(WsconsConsoleFd, 0);
	KdRegisterFdEnableDisable(WsconsConsoleFd,
	    WsconsKeyboardEnable, WsconsKeyboardDisable);
	return 1;
}

static void
WsconsKeyboardFini(void)
{
	WsconsKeyboardDisable(WsconsConsoleFd, 0);
	KdUnregisterFds(WsconsKbdType, FALSE);
}

static void
WsconsKeyboardLeds(int leds)
{
}

static void
WsconsKeyboardBell(int volume, int pitch, int duration)
{
}
	
KdKeyboardFuncs WsconsKeyboardFuncs = {
	WsconsKeyboardLoad,
	WsconsKeyboardInit,
	WsconsKeyboardLeds,
	WsconsKeyboardBell,
	WsconsKeyboardFini,
	3,
};
