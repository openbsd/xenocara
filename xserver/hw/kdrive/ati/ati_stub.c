/*
 * $Id: ati_stub.c,v 1.1.1.1 2006/11/26 18:22:13 matthieu Exp $
 *
 * Copyright © 2003 Eric Anholt
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of Eric Anholt not be used in
 * advertising or publicity pertaining to distribution of the software without
 * specific, written prior permission.  Eric Anholt makes no
 * representations about the suitability of this software for any purpose.  It
 * is provided "as is" without express or implied warranty.
 *
 * ERIC ANHOLT DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO
 * EVENT SHALL ERIC ANHOLT BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
 * DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */
/* $Header: /home/cvs/xenocara/xserver/hw/kdrive/ati/Attic/ati_stub.c,v 1.1.1.1 2006/11/26 18:22:13 matthieu Exp $ */

#ifdef HAVE_CONFIG_H
#include <kdrive-config.h>
#endif
#include "ati.h"
#include "klinux.h"

extern struct pci_id_entry ati_pci_ids[];

void
InitCard(char *name)
{
	struct pci_id_entry *id;
	KdCardAttr attr;

	for (id = ati_pci_ids; id->name != NULL; id++) {
		int j = 0;
		while (LinuxFindPci(id->vendor, id->device, j++, &attr))
			KdCardInfoAdd(&ATIFuncs, &attr, 0);
	}
}

void
InitOutput(ScreenInfo *pScreenInfo, int argc, char **argv)
{
	KdInitOutput(pScreenInfo, argc, argv);
}

void
InitInput(int argc, char **argv)
{
	KdInitInput(&LinuxMouseFuncs, &LinuxKeyboardFuncs);
}

void
ddxUseMsg (void)
{
	KdUseMsg();
#ifdef KDRIVEVESA
	vesaUseMsg();
#endif
}

int
ddxProcessArgument(int argc, char **argv, int i)
{
	int	ret;
    
#ifdef KDRIVEVESA
	if (!(ret = vesaProcessArgument (argc, argv, i)))
#endif
		ret = KdProcessArgument(argc, argv, i);

	return ret;
}
