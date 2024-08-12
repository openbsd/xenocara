/*****************************************************************************
Copyright 1988, 1989 by Digital Equipment Corporation, Maynard, Massachusetts.

                        All Rights Reserved

Permission to use, copy, modify, and distribute this software and its
documentation for any purpose and without fee is hereby granted,
provided that the above copyright notice appear in all copies and that
both that copyright notice and this permission notice appear in
supporting documentation, and that the name of Digital not be
used in advertising or publicity pertaining to distribution of the
software without specific, written prior permission.

DIGITAL DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL
DIGITAL BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR
ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
SOFTWARE.

******************************************************************************/

#include "x11perf.h"

static XSegment *segments;
static GC       pgc;

static void
GenerateSegments(XParms xp, Parms p, Bool ddashed)
{
    int     size;
    int     half;
    int     i;
    int     rows;	    /* Number of rows filled in current column      */
    int     x, y;	    /* base of square to draw in		    */
    int     x1=0, y1=0, x2=0, y2=0; /* offsets into square		    */
    int     phase;	    /* how far into 0..8*size we are		    */
    int     phaseinc;       /* how much to increment phase at each segment  */
    int     size8;	    /* 8 * size					    */
    XGCValues   gcv;

    if(ddashed)
	pgc = xp->ddfggc;
    else
	pgc = xp->fggc;


    size = p->special;
    size8 = 8 * size;
    half = (size + 19) / 20;

    segments = malloc((p->objects) * sizeof(XSegment));

    /* All this x, x1, etc. stuff is to create a pattern that
	(1) scans down the screen vertically, with each new segment going
	    into a square of size^2.

	(2) rotates the endpoints clockwise around the square

	(3) rotates by ``large'' steps if we aren't going to paint enough
	    segments to get full coverage

	(4) uses CapNotLast so we can create segments of length 1 that
	    nonetheless have a distinct direction
    */

    x     = half;  y     = half;
    phase = 0;
    phaseinc = size8 / p->objects;
    if (phaseinc == 0) phaseinc = 1;
    rows = 0;

    for (i = 0; i != p->objects; i++) {
	switch (phase / size) {
	case 0:
	    x1 = 0;
	    y1 = 0;
	    x2 = size;
	    y2 = phase;
	    break;

	case 1:
	    x1 = phase % size;
	    y1 = 0;
	    x2 = size;
	    y2 = size;
	    break;

	case 2:
	    x1 = size;
	    y1 = 0;
	    x2 = size - phase % size;
	    y2 = size;
	    break;

	case 3:
	    x1 = size;
	    y1 = phase % size;
	    x2 = 0;
	    y2 = size;
	    break;

	case 4:
	    x1 = size;
	    y1 = size;
	    x2 = 0;
	    y2 = size - phase % size;
	    break;

	case 5:
	    x1 = size - phase % size;
	    y1 = size;
	    x2 = 0;
	    y2 = 0;
	    break;

	case 6:
	    x1 = 0;
	    y1 = size;
	    x2 = phase % size;
	    y2 = 0;
	    break;

	case 7:
	    x1 = 0;
	    y1 = size - phase % size;
	    x2 = size;
	    y2 = 0;
	    break;
	} /* end switch */

	segments[i].x1 = x + x1;
	segments[i].y1 = y + y1;
	segments[i].x2 = x + x2;
	segments[i].y2 = y + y2;

	/* Change square to draw segment in */
	rows++;
	y += size;
	if (y >= HEIGHT - size - half || rows == MAXROWS) {
	    /* Go to next column */
	    rows = 0;
	    y = half;
	    x += size;
	    if (x >= WIDTH - size - half) {
		x = half;
	    }
	}

	/* Increment phase */
	phase += phaseinc;
	if (phase >= size8) phase -= size8;

    }

    gcv.cap_style = CapNotLast;

    if(ddashed) {
	XChangeGC(xp->d, xp->ddfggc, GCCapStyle, &gcv);
	XChangeGC(xp->d, xp->ddbggc, GCCapStyle, &gcv);
    } else {
	XChangeGC(xp->d, xp->fggc, GCCapStyle, &gcv);
	XChangeGC(xp->d, xp->bggc, GCCapStyle, &gcv);
    }
}

int
InitSegments(XParms xp, Parms p, int64_t reps)
{
    GenerateSegments(xp, p, False);
    return reps;
}

int
InitDashedSegments(XParms xp, Parms p, int64_t reps)
{
    char dashes[2];

    GenerateSegments(xp, p, False);

    /* Modify GCs to draw dashed */
    XSetLineAttributes
	(xp->d, xp->bggc, 0, LineOnOffDash, CapNotLast, JoinMiter);
    XSetLineAttributes
	(xp->d, xp->fggc, 0, LineOnOffDash, CapNotLast, JoinMiter);
    dashes[0] = 3;   dashes[1] = 2;
    XSetDashes(xp->d, xp->fggc, 0, dashes, 2);
    XSetDashes(xp->d, xp->bggc, 0, dashes, 2);
    return reps;
}

int
InitDoubleDashedSegments(XParms xp, Parms p, int64_t reps)
{
    char dashes[2];

    GenerateSegments(xp, p, True);

    /* Modify GCs to draw dashed */
    XSetLineAttributes
	(xp->d, xp->ddbggc, 0, LineDoubleDash, CapNotLast, JoinMiter);
    XSetLineAttributes
	(xp->d, xp->ddfggc, 0, LineDoubleDash, CapNotLast, JoinMiter);
    dashes[0] = 3;   dashes[1] = 2;
    XSetDashes(xp->d, xp->ddfggc, 0, dashes, 2);
    XSetDashes(xp->d, xp->ddbggc, 0, dashes, 2);
    return reps;
}

static int
InitHorizSegmentsWidth(XParms xp, Parms p, int64_t reps, int width)
{
    int     size;
    int     i;
    int     x, y;	/* base of square to draw in			*/
    int     y1;		/* y position inside square			*/
    int     inc;
    XGCValues   gcv;

    pgc = xp->fggc;

    size = p->special;

    segments = malloc((p->objects) * sizeof(XSegment));

    x = width / 2 + 1;
    y = width / 2 + 1;
    y1 = 0;
    inc = width + 1;

    for (i = 0; i != p->objects; i++) {
	if (i % 2) {
	    segments[i].x1 = x + size;
	    segments[i].x2 = x;
	    segments[i].y1 = y + (HEIGHT - width - 2) - y1;
	    segments[i].y2 = y + (HEIGHT - width - 2) - y1;
            y1 += inc;
	} else {
	    segments[i].x1 = x;
	    segments[i].x2 = x + size;
	    segments[i].y1 = y + y1;
	    segments[i].y2 = y + y1;
	}
        /* Go to next row */
	if (y1 >= HEIGHT / 2 - (width + 2)) {
	    y1 =0;
	    x += size + inc;
	    if (x >= WIDTH - size - width)
		x = width/2 + 1;
	}
    }
    gcv.cap_style = CapNotLast;
    XChangeGC(xp->d, xp->fggc, GCCapStyle, &gcv);
    XChangeGC(xp->d, xp->bggc, GCCapStyle, &gcv);
    return reps;
}

int
InitHorizSegments(XParms xp, Parms p, int64_t reps)
{
    return InitHorizSegmentsWidth(xp, p, reps, 1);
}

int
InitWideHorizSegments(XParms xp, Parms p, int64_t reps)
{
    int size = p->special;

    (void)InitHorizSegmentsWidth(xp, p, reps, (int) ((size + 9) / 10));

    XSetLineAttributes(xp->d, xp->bggc, (int) ((size + 9) / 10),
	LineSolid, CapRound, JoinRound);
    XSetLineAttributes(xp->d, xp->fggc, (int) ((size + 9) / 10),
	LineSolid, CapRound, JoinRound);

    return reps;
}

static int
InitVertSegmentsWidth(XParms xp, Parms p, int64_t reps, int width)
{
    int     size;
    int     i;
    int     x, y;	/* base of square to draw in			*/
    int     x1;		/* x position inside square			*/
    int     inc;
    XGCValues   gcv;

    pgc = xp->fggc;

    size = p->special;

    segments = malloc((p->objects) * sizeof(XSegment));

    x = width / 2 + 1;
    y = width / 2 + 1;
    x1 = 0;
    inc = width + 1;

    for (i = 0; i != p->objects; i++) {
	if (i % 2) {
	    segments[i].x1 = x + (WIDTH - width - 2) - x1;
	    segments[i].x2 = x + (WIDTH - width - 2) - x1;
	    segments[i].y1 = y + size;
	    segments[i].y2 = y;
            x1 += inc;
	} else {
	    segments[i].x1 = x + x1;
	    segments[i].x2 = x + x1;
	    segments[i].y1 = y;
	    segments[i].y2 = y + size;
	}
        /* Go to next column */
	if (x1 >= WIDTH / 2 - (width + 2)) {
	    x1 = 0;
            y += size + inc;
            if (y >= HEIGHT - size - width)
                y = width/2 + 1;
	}
    }
    gcv.cap_style = CapNotLast;
    XChangeGC(xp->d, xp->fggc, GCCapStyle, &gcv);
    XChangeGC(xp->d, xp->bggc, GCCapStyle, &gcv);
    return reps;
}

int
InitVertSegments(XParms xp, Parms p, int64_t reps)
{
    return InitVertSegmentsWidth(xp, p, reps, 1);
}

int
InitWideVertSegments(XParms xp, Parms p, int64_t reps)
{
    int    size = p->special;

    (void)InitVertSegmentsWidth(xp, p, reps, (size + 9) / 10);

    XSetLineAttributes(xp->d, xp->bggc, (int) ((size + 9) / 10),
	LineSolid, CapRound, JoinRound);
    XSetLineAttributes(xp->d, xp->fggc, (int) ((size + 9) / 10),
	LineSolid, CapRound, JoinRound);

    return reps;
}

void
DoSegments(XParms xp, Parms p, int64_t reps)
{
    int i;

    for (i = 0; i != reps; i++) {
        XDrawSegments(xp->d, xp->w, pgc, segments, p->objects);
        if (pgc == xp->ddbggc)
            pgc = xp->ddfggc;
        else if(pgc == xp->ddfggc)
            pgc = xp->ddbggc;
        else if (pgc == xp->bggc)
            pgc = xp->fggc;
        else
            pgc = xp->bggc;
	CheckAbort ();
    }
}

void
EndSegments(XParms xp, Parms p)
{
    free(segments);
}
