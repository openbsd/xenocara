/* $OpenBSD: mouse.c,v 1.1 2007/05/25 15:33:32 matthieu Exp $ */
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
#include "inputstr.h"
#include "scrnintstr.h"
#include "kdrive.h"

static void
MouseRead(int mousePort, void *closure)
{
	KdMouseInfo *mi;
	int dx, dy;
	unsigned long flags;

	/* flags: 
	   KD_MOUSE_DELTA
	   KD_BUTTON_{1,2,3} */
	/* Read and post mouse events */
	KdEnqueueMouseEvent(mi, flags, dx, dy);
	
}

int MouseInputType;
char *kdefaultMouse[] = {
	"/dev/wsmouse",
	"/dev/wsmouse0",
	"/dev/tty00",
	"/dev/tty01"
};

#define NUM_DEFAULT_MOUSE    (sizeof (kdefaultMouse) / sizeof (kdefaultMouse[0]))

static Bool
MouseInit(void)
{
	if (!MouseInputType)
		MouseInputType = KdAllocInputType();

	return TRUE;
}
		
static void
MouseFini(void)
{
	KdMouseInfo *mi;
	
	KdUnregisterFds(MouseInputType, NULL);
	for (mi = kdMouseInfo; mi != NULL;  mi = mi->next) {
		if (mi->inputType == MouseInputType) {
			free(mi->driver);
			mi->driver = NULL;
			mi->inputType = 0;
		}
	}
}

KdMouseFuncs WsconsMouseFuncs = {
	MouseInit,
	MouseFini
};
