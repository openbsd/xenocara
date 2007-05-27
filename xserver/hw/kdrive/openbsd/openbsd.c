/* $OpenBSD: openbsd.c,v 1.3 2007/05/27 00:53:47 matthieu Exp $ */
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
#define XK_PUBLISHING
#include <X11/keysym.h>
#include <X11/Xdefs.h>

#include <errno.h>
#include <fcntl.h>
#include <dev/wscons/wsconsio.h>

#include "kdrive.h"
#include "kopenbsd.h"

static int vtno;
int WsconsConsoleFd;
int OpenBSDApmFd = -1;
static int activeVT;
static Bool enabled;

#define WSCONS_DEV "/dev/ttyC0"

#define DBG(x) ErrorF x

static int
OpenBSDInit(void)
{
	DBG(("OpenBSDInit\n"));
	WsconsConsoleFd = open(WSCONS_DEV, O_RDWR);

	if (WsconsConsoleFd == -1) {
		FatalError("OpenBSDInit: error opening %s (%d)\n",
		    WSCONS_DEV, errno);
	}
	return 1;
}

Bool
OpenBSDFindPci(CARD16 vendor, CARD16 device, CARD32 count, KdCardAttr *attr)
{
	DBG(("OpenBSDFindPci %04hx %04hx\n", vendor, device));
	/* Find a PCI device matching vendor/device */
	/* return KdCardAttr */
	/* XXX */
	attr->bus = 0;
	attr->slot = 0;
	attr->func = 0;
	
	return TRUE;
}

unsigned char *
OpenBSDGetPciCfg(KdCardAttr *attr)
{
	/* Get Config registers for card at attr */
	DBG(("OpenBSDGetPciCfg %d:%d:%d\n", 
		attr->bus, attr->slot, attr->func));
	return NULL;
}

static void
OpenBSDEnable(void)
{
	DBG(("OpenBSDEnable\n"));
	if (enabled)
		return;
	if (kdSwitchPending) {
		kdSwitchPending = FALSE;
	}
	/* Open / init APM */
	/* Get a VT */
	/* Switch to graphics mode */
	enabled = TRUE;
}


static Bool
OpenBSDSpecialKey(KeySym sym)
{
	DBG(("OpenBSDSpecialKey\n"));
	/* Initiate VT switch if needed */
	return FALSE;
}

static void
OpenBSDDisable(void)
{
	DBG(("OpenBSDDisable\n"));
	enabled = FALSE;
	/* Back to text mode */
	/* Remove apm hooks */
}

static void
OpenBSDFini(void)
{
	int mode = WSDISPLAYIO_MODE_EMUL;

	DBG(("OpenBSDFini\n"));
	if (WsconsConsoleFd < 0)
		return;

	/* switch back to initial VT */
	if (ioctl(WsconsConsoleFd, WSDISPLAYIO_SMODE, &mode) == -1) {
		ErrorF("WSDISPLAYIO_SMODE(EMUL): %s\n", strerror(errno));
	}
}

KdOsFuncs OpenBSDFuncs = {
	OpenBSDInit,
	OpenBSDEnable,
	OpenBSDSpecialKey,
	OpenBSDDisable,
	OpenBSDFini,
	0
};

void
OsVendorInit(void)
{
	DBG(("OsVendorInit\n"));
	KdOsInit(&OpenBSDFuncs);
}
