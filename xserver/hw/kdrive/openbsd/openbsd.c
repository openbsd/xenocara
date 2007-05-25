/* $OpenBSD: openbsd.c,v 1.1 2007/05/25 15:33:32 matthieu Exp $ */
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
#include "kdrive.h"
#include "kopenbsd.h"

static int vtno;
int WsconsConsoleFd;
int OpenBSDApmFd = -1;
static int activeVT;
static Bool enabled;

static int
OpenBSDInit(void)
{
	WsconsConsoleFd = -1;
	return 1;
}

Bool
OpenBSDFindPci(CARD16 vendor, CARD16 device, CARD32 count, KdCardAttr *attr)
{
	/* Find a PCI device matching vendor/device */
	/* return KdCardAttr */
	return FALSE;
}

unsigned char *
OpenBSDGetPciCfg(KdCardAttr *attr)
{
	/* Get Config registers for card at attr */
	return NULL;
}

static void
OpenBSDEnable(void)
{
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
	/* Initiate VT switch if needed */
	return FALSE;
}

static void
OpenBSDDisable(void)
{
	enabled = FALSE;
	/* Back to text mode */
	/* Remove apm hooks */
}

static void
OpenBSDFini(void)
{
	if (WsconsConsoleFd < 0)
		return;
	/* switch back to initial VT */
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
	KdOsInit(&OpenBSDFuncs);
}
