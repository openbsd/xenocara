/*
 * Copyright © 2011 Dave Airlie
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

XRRProviderResources *
XRRGetProviderResources(Display *dpy, Window window)
{
    XExtDisplayInfo		*info = XRRFindDisplay(dpy);
    xRRGetProvidersReply rep;
    xRRGetProvidersReq *req;
    XRRProviderResources *xrpr;
    long nbytes, nbytesRead;
    int rbytes;

    RRCheckExtension (dpy, info, NULL);

    LockDisplay (dpy);

    GetReq(RRGetProviders, req);
    req->reqType = info->codes->major_opcode;
    req->randrReqType = X_RRGetProviders;
    req->window = window;
    
    if (!_XReply (dpy, (xReply *) &rep, 0, xFalse))
    {
      UnlockDisplay (dpy);
      SyncHandle ();
      return NULL;
    }

    nbytes = (long) rep.length << 2;

    nbytesRead = (long) (rep.nProviders * 4);

    rbytes = (sizeof(XRRProviderResources) + rep.nProviders * sizeof(RRProvider));
    xrpr = (XRRProviderResources *) Xmalloc(rbytes);

    if (xrpr == NULL) {
       _XEatDataWords (dpy, rep.length);
       UnlockDisplay (dpy);
       SyncHandle ();
       return NULL;
    }

    xrpr->timestamp = rep.timestamp;
    xrpr->nproviders = rep.nProviders;
    xrpr->providers = (RRProvider *)(xrpr + 1);

    _XRead32(dpy, (long *) xrpr->providers, rep.nProviders << 2);

    if (nbytes > nbytesRead)
      _XEatData (dpy, (unsigned long) (nbytes - nbytesRead));


    UnlockDisplay (dpy);
    SyncHandle();

    return (XRRProviderResources *) xrpr;
}

void
XRRFreeProviderResources(XRRProviderResources *provider_resources)
{
    free(provider_resources);
}

#define ProviderInfoExtra	(SIZEOF(xRRGetProviderInfoReply) - 32)  
XRRProviderInfo *
XRRGetProviderInfo(Display *dpy, XRRScreenResources *resources, RRProvider provider)
{
    XExtDisplayInfo	    *info = XRRFindDisplay(dpy);
    xRRGetProviderInfoReply rep;
    xRRGetProviderInfoReq *req;
    int nbytes, nbytesRead, rbytes;
    XRRProviderInfo *xpi;

    RRCheckExtension (dpy, info, NULL);

    LockDisplay (dpy);
    GetReq (RRGetProviderInfo, req);
    req->reqType = info->codes->major_opcode;
    req->randrReqType = X_RRGetProviderInfo;
    req->provider = provider;
    req->configTimestamp = resources->configTimestamp;

    if (!_XReply (dpy, (xReply *) &rep, ProviderInfoExtra >> 2, xFalse))
    {
	UnlockDisplay (dpy);
	SyncHandle ();
	return NULL;
    }

    nbytes = ((long) rep.length << 2) - ProviderInfoExtra;

    nbytesRead = (long)(rep.nCrtcs * 4 +
			rep.nOutputs * 4 +
			rep.nAssociatedProviders * 8 +
			((rep.nameLength + 3) & ~3));

    rbytes = (sizeof(XRRProviderInfo) +
	      rep.nCrtcs * sizeof(RRCrtc) +
	      rep.nOutputs * sizeof(RROutput) +
	      rep.nAssociatedProviders * (sizeof(RRProvider) + sizeof(unsigned int))+
	      rep.nameLength + 1);

    xpi = (XRRProviderInfo *)Xmalloc(rbytes);
    if (xpi == NULL) {
	_XEatDataWords (dpy, rep.length - (ProviderInfoExtra >> 2));
	UnlockDisplay (dpy);
	SyncHandle ();
	return NULL;
    }

    xpi->capabilities = rep.capabilities;
    xpi->ncrtcs = rep.nCrtcs;
    xpi->noutputs = rep.nOutputs;
    xpi->nassociatedproviders = rep.nAssociatedProviders;
    xpi->crtcs = (RRCrtc *)(xpi + 1);
    xpi->outputs = (RROutput *)(xpi->crtcs + rep.nCrtcs);
    xpi->associated_providers = (RRProvider *)(xpi->outputs + rep.nOutputs);
    xpi->associated_capability = (unsigned int *)(xpi->associated_providers + rep.nAssociatedProviders);
    xpi->name = (char *)(xpi->associated_capability + rep.nAssociatedProviders);

    _XRead32(dpy, (long *) xpi->crtcs, rep.nCrtcs << 2);
    _XRead32(dpy, (long *) xpi->outputs, rep.nOutputs << 2);

    _XRead32(dpy, (long *) xpi->associated_providers, rep.nAssociatedProviders << 2);

    /*
     * _XRead32 reads a series of 32-bit values from the protocol and writes
     * them out as a series of "long int" values, but associated_capability
     * is defined as unsigned int *, so that won't work for this array.
     * Instead we assume for now that "unsigned int" is also 32-bits, so
     * the values can be read without any conversion.
     */
    _XRead(dpy, (char *) xpi->associated_capability,
           rep.nAssociatedProviders << 2);

    _XReadPad(dpy, xpi->name, rep.nameLength);
    xpi->name[rep.nameLength] = '\0';

    /*
     * Skip any extra data
     */
    if (nbytes > nbytesRead)
	_XEatData (dpy, (unsigned long) (nbytes - nbytesRead));

    UnlockDisplay (dpy);
    SyncHandle ();
    return (XRRProviderInfo *) xpi;
}

void
XRRFreeProviderInfo(XRRProviderInfo *provider)
{
    free(provider);
}

int
XRRSetProviderOutputSource(Display *dpy, XID provider,
			   XID source_provider)
{
    XExtDisplayInfo	    *info = XRRFindDisplay(dpy);
    xRRSetProviderOutputSourceReq *req;

    RRCheckExtension (dpy, info, 0);
    LockDisplay (dpy);
    GetReq (RRSetProviderOutputSource, req);
    req->reqType = info->codes->major_opcode;
    req->randrReqType = X_RRSetProviderOutputSource;
    req->provider = provider;
    req->source_provider = source_provider;
    UnlockDisplay (dpy);
    SyncHandle ();
    return 0;
}

int
XRRSetProviderOffloadSink(Display *dpy, XID provider,
			  XID sink_provider)
{
    XExtDisplayInfo	    *info = XRRFindDisplay(dpy);
    xRRSetProviderOffloadSinkReq *req;

    RRCheckExtension (dpy, info, 0);
    LockDisplay (dpy);
    GetReq (RRSetProviderOffloadSink, req);
    req->reqType = info->codes->major_opcode;
    req->randrReqType = X_RRSetProviderOffloadSink;
    req->provider = provider;
    req->sink_provider = sink_provider;
    UnlockDisplay (dpy);
    SyncHandle ();
    return 0;
}
