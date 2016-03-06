/*
 * Copyright © 2014 Keith Packard
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that copyright
 * notice and this permission notice appear in supporting documentation, and
 * that the name of the copyright holders not be used in advertising or
 * publicity pertaining to distribution of the software without specific,
 * written prior permission.  The copyright holders make no representations
 * about the suitability of this software for any purpose.  It is provided "as
 * is" without express or implied warranty.
 *
 * THE COPYRIGHT HOLDERS DISCLAIM ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO
 * EVENT SHALL THE COPYRIGHT HOLDERS BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
 * DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE
 * OF THIS SOFTWARE.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>
#include <X11/Xlib.h>
/* we need to be able to manipulate the Display structure on events */
#include <X11/Xlibint.h>
#include <X11/extensions/render.h>
#include <X11/extensions/Xrender.h>
#include "Xrandrint.h"

XRRMonitorInfo *
XRRGetMonitors(Display *dpy, Window window, Bool get_active, int *nmonitors)
{
    XExtDisplayInfo	    *info = XRRFindDisplay(dpy);
    xRRGetMonitorsReply	    rep;
    xRRGetMonitorsReq	    *req;
    int			    nbytes, nbytesRead, rbytes;
    int			    nmon, noutput;
    int			    m, o;
    char		    *buf, *buf_head;
    xRRMonitorInfo	    *xmon;
    CARD32		    *xoutput;
    XRRMonitorInfo	    *mon = NULL;
    RROutput		    *output;

    RRCheckExtension (dpy, info, NULL);

    *nmonitors = -1;

    LockDisplay (dpy);
    GetReq (RRGetMonitors, req);
    req->reqType = info->codes->major_opcode;
    req->randrReqType = X_RRGetMonitors;
    req->window = window;
    req->get_active = get_active;

    if (!_XReply (dpy, (xReply *) &rep, 0, xFalse))
    {
	UnlockDisplay (dpy);
	SyncHandle ();
	return NULL;
    }

    nbytes = (long) rep.length << 2;
    nmon = rep.nmonitors;
    noutput = rep.noutputs;
    nbytesRead = nmon * SIZEOF(xRRMonitorInfo) + noutput * 4;

    if (nmon > 0) {

	/*
	 * first we must compute how much space to allocate for
	 * randr library's use; we'll allocate the structures in a single
	 * allocation, on cleanlyness grounds.
	 */

	rbytes = nmon * sizeof (XRRMonitorInfo) + noutput * sizeof(RROutput);

	buf = buf_head = Xmalloc (nbytesRead);
	mon = Xmalloc (rbytes);

	if (buf == NULL || mon == NULL) {
	    if (buf != NULL) Xfree(buf);
	    if (mon != NULL) Xfree(mon);
	    _XEatDataWords (dpy, rep.length);
	    UnlockDisplay (dpy);
	    SyncHandle ();
	    return NULL;
	}

	_XReadPad(dpy, buf, nbytesRead);

	output = (RROutput *) (mon + nmon);

	for (m = 0; m < nmon; m++) {
	    xmon = (xRRMonitorInfo *) buf;
	    mon[m].name = xmon->name;
	    mon[m].primary = xmon->primary;
	    mon[m].automatic = xmon->automatic;
	    mon[m].noutput = xmon->noutput;
	    mon[m].x = xmon->x;
	    mon[m].y = xmon->y;
	    mon[m].width = xmon->width;
	    mon[m].height = xmon->height;
	    mon[m].mwidth = xmon->widthInMillimeters;
	    mon[m].mheight = xmon->heightInMillimeters;
	    mon[m].outputs = output;
	    buf += SIZEOF (xRRMonitorInfo);
	    xoutput = (CARD32 *) buf;
	    for (o = 0; o < xmon->noutput; o++)
		output[o] = xoutput[o];
	    output += xmon->noutput;
	    buf += xmon->noutput * 4;
	}
	Xfree(buf_head);
    }

    /*
     * Skip any extra data
     */
    if (nbytes > nbytesRead)
	_XEatData (dpy, (unsigned long) (nbytes - nbytesRead));

    UnlockDisplay (dpy);
    SyncHandle ();

    *nmonitors = nmon;
    return mon;
}

void
XRRSetMonitor(Display *dpy, Window window, XRRMonitorInfo *monitor)
{
    XExtDisplayInfo	    *info = XRRFindDisplay(dpy);
    xRRSetMonitorReq	    *req;

    RRSimpleCheckExtension (dpy, info);

    LockDisplay(dpy);
    GetReq (RRSetMonitor, req);
    req->reqType = info->codes->major_opcode;
    req->randrReqType = X_RRSetMonitor;
    req->length += monitor->noutput;
    req->window = window;
    req->monitor.name = monitor->name;
    req->monitor.primary = monitor->primary;
    req->monitor.automatic = False;
    req->monitor.noutput = monitor->noutput;
    req->monitor.x = monitor->x;
    req->monitor.y = monitor->y;
    req->monitor.width = monitor->width;
    req->monitor.height = monitor->height;
    req->monitor.widthInMillimeters = monitor->mwidth;
    req->monitor.heightInMillimeters = monitor->mheight;
    Data32 (dpy, monitor->outputs, monitor->noutput * 4);

    UnlockDisplay (dpy);
    SyncHandle ();
}

void
XRRDeleteMonitor(Display *dpy, Window window, Atom name)
{
    XExtDisplayInfo	    *info = XRRFindDisplay(dpy);
    xRRDeleteMonitorReq	    *req;

    RRSimpleCheckExtension (dpy, info);

    LockDisplay(dpy);
    GetReq (RRDeleteMonitor, req);
    req->reqType = info->codes->major_opcode;
    req->randrReqType = X_RRDeleteMonitor;
    req->window = window;
    req->name = name;
    UnlockDisplay (dpy);
    SyncHandle ();
}

XRRMonitorInfo *
XRRAllocateMonitor(Display *dpy, int noutput)
{
    XRRMonitorInfo *monitor = calloc(1, sizeof (XRRMonitorInfo) + noutput * sizeof (RROutput));
    if (!monitor)
	return NULL;
    monitor->outputs = (RROutput *) (monitor + 1);
    monitor->noutput = noutput;
    return monitor;
}

void
XRRFreeMonitors(XRRMonitorInfo *monitors)
{
    if (monitors)
	Xfree(monitors);
}

