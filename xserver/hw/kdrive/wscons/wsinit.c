/* $OpenBSD: wsinit.c,v 1.4 2014/02/15 15:00:28 matthieu Exp $ */
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
#include <dev/wscons/wsconsio.h>

#include "kdrive.h"
#include "wsfb.h"

#define DBG(x) ErrorF x

void
InitCard(char *name)
{

	DBG(("InitCard\n"));
	KdCardInfoAdd(&wsfbFuncs, 0);
}

void
InitOutput(ScreenInfo *pScreenInfo, int argc, char **argv)
{
	DBG(("InitOutput\n"));
	KdInitOutput(pScreenInfo, argc, argv);
}

void
InitInput(int argc, char **argv)
{
	KdKeyboardInfo *ki;

	DBG(("InitInput\n"));
	KdAddKeyboardDriver(&WsconsKeyboardDriver);
	KdAddPointerDriver(&WsconsMouseDriver);
	ki = KdParseKeyboard("keybd");
	KdAddKeyboard(ki);

	KdInitInput();
}

void
CloseInput(void)
{
	KdCloseInput();
}

void
ddxUseMsg(void)
{
	KdUseMsg();
	ErrorF("\nXwsfb device Usage:\n");
	ErrorF("-foo bar\n");
	ErrorF("\n");
}

int
ddxProcessArgument(int argc, char **argv, int i)
{
	DBG(("ddxProcessArgument\n"));
	return KdProcessArgument(argc, argv, i);
}

KdCardFuncs wsfbFuncs = {
	wsfbCardInit,
	wsfbScreenInit,
	wsfbInitScreen,
	wsfbFinishInitScreen,
	wsfbCreateResources,
	wsfbPreserve,
	wsfbEnable,
	wsfbDPMS,
	wsfbDisable,
	wsfbRestore,
	wsfbScreenFini,
	wsfbCardFini,
	0,			/* initCursor */
	0,			/* enableCursor */
	0,			/* disqbleCursor */
	0,			/* finiCursor */
	0,			/* recolorCursor */
	0,			/* initAccel */
	0,			/* enableAccel */
	0,			/* disableAccel */
	0,			/* finiAccel */
	wsfbGetColors,
	wsfbPutColors,
};
