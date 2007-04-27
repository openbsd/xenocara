/*
 * calmwm - the calm window manager
 *
 * Copyright (c) 2004 Marius Aamodt Eriksen <marius@monkey.org>
 * All rights reserved.
 *
 * $Id: draw.c,v 1.1.1.1 2007/04/27 17:58:48 bernd Exp $
 */

#include "headers.h"
#include "calmwm.h"

void
draw_outline(struct client_ctx *cc)
{
	struct screen_ctx *sc = CCTOSC(cc);

        XDrawRectangle(G_dpy, sc->rootwin, sc->invgc,
	    cc->geom.x - cc->bwidth, cc->geom.y - cc->bwidth,
	    cc->geom.width + cc->bwidth, cc->geom.height + cc->bwidth);
}
