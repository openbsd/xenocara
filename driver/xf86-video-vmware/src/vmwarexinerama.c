/*
 * Copyright 2006 by VMware, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE COPYRIGHT HOLDER(S) OR AUTHOR(S) BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the name of the copyright holder(s)
 * and author(s) shall not be used in advertising or otherwise to promote
 * the sale, use or other dealings in this Software without prior written
 * authorization from the copyright holder(s) and author(s).
 */

/*
 * vmwarexinerama.c --
 *
 *      The implementation of the Xinerama protocol extension.
 */


#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "dixstruct.h"
#include "extnsionst.h"
#include <X11/X.h>
#include <X11/extensions/panoramiXproto.h>

#include "vmware.h"

#ifndef HAVE_XORG_SERVER_1_5_0
#include <xf86_ansic.h>
#include <xf86_libc.h>
#endif

/* 
 * LookupWindow was removed with video abi 11.
 */
#if (GET_ABI_MAJOR(ABI_VIDEODRV_VERSION) < 4)
#ifndef DixGetAttrAccess
#define DixGetAttrAccess   (1<<4)
#endif
#endif

#if (GET_ABI_MAJOR(ABI_VIDEODRV_VERSION) < 2)
static inline int
dixLookupWindow(WindowPtr *pWin, XID id, ClientPtr client, Mask access)
{
    *pWin = LookupWindow(id, client);
    if (!*pWin)
	return BadWindow;
    return Success;
}
#endif


/*
 *----------------------------------------------------------------------------
 *
 * VMwareXineramaQueryVersion --
 *
 *      Implementation of QueryVersion command handler. Initialises and
 *      sends a reply.
 *
 * Results:
 *      Standard response codes.
 *
 * Side effects:
 *      Writes reply to client.
 *
 *----------------------------------------------------------------------------
 */

static int
VMwareXineramaQueryVersion(ClientPtr client)
{
    xPanoramiXQueryVersionReply	  rep;
    register int		  n;

    REQUEST_SIZE_MATCH(xPanoramiXQueryVersionReq);
    rep.type = X_Reply;
    rep.length = 0;
    rep.sequenceNumber = client->sequence;
    rep.majorVersion = 1;
    rep.minorVersion = 0;
    if(client->swapped) {
        _swaps(&rep.sequenceNumber, n);
        _swapl(&rep.length, n);
        _swaps(&rep.majorVersion, n);
        _swaps(&rep.minorVersion, n);
    }
    WriteToClient(client, sizeof(xPanoramiXQueryVersionReply), (char *)&rep);
    return (client->noClientException);
}


/*
 *----------------------------------------------------------------------------
 *
 * VMwareXineramaGetState --
 *
 *      Implementation of GetState command handler. Initialises and
 *      sends a reply.
 *
 * Results:
 *      Standard response codes.
 *
 * Side effects:
 *      Writes reply to client.
 *
 *----------------------------------------------------------------------------
 */

static int
VMwareXineramaGetState(ClientPtr client)
{
    REQUEST(xPanoramiXGetStateReq);
    WindowPtr			pWin;
    xPanoramiXGetStateReply	rep;
    register int		n;
    ExtensionEntry *ext;
    ScrnInfoPtr pScrn;
    VMWAREPtr pVMWARE;
    int rc;

    REQUEST_SIZE_MATCH(xPanoramiXGetStateReq);
    rc = dixLookupWindow(&pWin, stuff->window, client, DixGetAttrAccess);
    if (rc != Success)
	return rc;

    if (!(ext = CheckExtension(PANORAMIX_PROTOCOL_NAME))) {
       return BadMatch;
    }
    pScrn = ext->extPrivate;
    pVMWARE = VMWAREPTR(pScrn);

    rep.type = X_Reply;
    rep.length = 0;
    rep.sequenceNumber = client->sequence;
    rep.state = pVMWARE->xinerama;
    rep.window = stuff->window;
    if(client->swapped) {
       _swaps (&rep.sequenceNumber, n);
       _swapl (&rep.length, n);
       _swapl (&rep.window, n);
    }
    WriteToClient(client, sizeof(xPanoramiXGetStateReply), (char *)&rep);
    return client->noClientException;
}


/*
 *----------------------------------------------------------------------------
 *
 * VMwareXineramaGetScreenCount --
 *
 *      Implementation of GetScreenCount command handler. Initialises and
 *      sends a reply.
 *
 * Results:
 *      Standard response codes.
 *
 * Side effects:
 *      Writes reply to client.
 *
 *----------------------------------------------------------------------------
 */

static int
VMwareXineramaGetScreenCount(ClientPtr client)
{
    REQUEST(xPanoramiXGetScreenCountReq);
    WindowPtr				pWin;
    xPanoramiXGetScreenCountReply	rep;
    register int			n;
    ExtensionEntry *ext;
    ScrnInfoPtr pScrn;
    VMWAREPtr pVMWARE;
    int rc;

    REQUEST_SIZE_MATCH(xPanoramiXGetScreenCountReq);
    rc = dixLookupWindow(&pWin, stuff->window, client, DixGetAttrAccess);
    if (rc != Success)
	return rc;

    if (!(ext = CheckExtension(PANORAMIX_PROTOCOL_NAME))) {
       return BadMatch;
    }
    pScrn = ext->extPrivate;
    pVMWARE = VMWAREPTR(pScrn);

    rep.type = X_Reply;
    rep.length = 0;
    rep.sequenceNumber = client->sequence;
    rep.ScreenCount = pVMWARE->xineramaNumOutputs;
    rep.window = stuff->window;
    
    if(client->swapped) {
       _swaps(&rep.sequenceNumber, n);
       _swapl(&rep.length, n);
       _swapl(&rep.window, n);
    }
    WriteToClient(client, sizeof(xPanoramiXGetScreenCountReply), (char *)&rep);
    return client->noClientException;
}


/*
 *----------------------------------------------------------------------------
 *
 * VMwareXineramaGetScreenSize --
 *
 *      Implementation of GetScreenSize command handler. Initialises and
 *      sends a reply.
 *
 * Results:
 *      Standard response codes.
 *
 * Side effects:
 *      Writes reply to client.
 *
 *----------------------------------------------------------------------------
 */

static int
VMwareXineramaGetScreenSize(ClientPtr client)
{
    REQUEST(xPanoramiXGetScreenSizeReq);
    WindowPtr				pWin;
    xPanoramiXGetScreenSizeReply	rep;
    register int			n;
    ExtensionEntry *ext;
    ScrnInfoPtr pScrn;
    VMWAREPtr pVMWARE;
    int rc;


    REQUEST_SIZE_MATCH(xPanoramiXGetScreenSizeReq);
    rc = dixLookupWindow(&pWin, stuff->window, client, DixGetAttrAccess);
    if (rc != Success)
	return rc;

    if (!(ext = CheckExtension(PANORAMIX_PROTOCOL_NAME))) {
       return BadMatch;
    }
    pScrn = ext->extPrivate;
    pVMWARE = VMWAREPTR(pScrn);

    rep.type = X_Reply;
    rep.length = 0;
    rep.sequenceNumber = client->sequence;
    rep.width  = pVMWARE->xineramaState[stuff->screen].width;
    rep.height  = pVMWARE->xineramaState[stuff->screen].height;
    rep.window = stuff->window;
    rep.screen = stuff->screen;
    if(client->swapped) {
       _swaps(&rep.sequenceNumber, n);
       _swapl(&rep.length, n);
       _swapl(&rep.width, n);
       _swapl(&rep.height, n);
       _swapl(&rep.window, n);
       _swapl(&rep.screen, n);
    }
    WriteToClient(client, sizeof(xPanoramiXGetScreenSizeReply), (char *)&rep);
    return client->noClientException;
}


/*
 *----------------------------------------------------------------------------
 *
 * VMwareXineramaIsActive --
 *
 *      Implementation of IsActive command handler. Initialises and
 *      sends a reply.
 *
 * Results:
 *      Standard response codes.
 *
 * Side effects:
 *      Writes reply to client.
 *
 *----------------------------------------------------------------------------
 */

static int
VMwareXineramaIsActive(ClientPtr client)
{
    xXineramaIsActiveReply	rep;
    ExtensionEntry *ext;
    ScrnInfoPtr pScrn;
    VMWAREPtr pVMWARE;

    REQUEST_SIZE_MATCH(xXineramaIsActiveReq);

    if (!(ext = CheckExtension(PANORAMIX_PROTOCOL_NAME))) {
       return BadMatch;
    }
    pScrn = ext->extPrivate;
    pVMWARE = VMWAREPTR(pScrn);

    rep.type = X_Reply;
    rep.length = 0;
    rep.sequenceNumber = client->sequence;
    rep.state = pVMWARE->xinerama;
    if(client->swapped) {
	register int n;
	_swaps(&rep.sequenceNumber, n);
	_swapl(&rep.length, n);
	_swapl(&rep.state, n);
    }
    WriteToClient(client, sizeof(xXineramaIsActiveReply), (char *) &rep);
    return client->noClientException;
}


/*
 *----------------------------------------------------------------------------
 *
 * VMwareXineramaQueryScreens --
 *
 *      Implementation of QueryScreens command handler. Initialises and
 *      sends a reply.
 *
 * Results:
 *      Standard response codes.
 *
 * Side effects:
 *      Writes reply to client.
 *
 *----------------------------------------------------------------------------
 */

static int
VMwareXineramaQueryScreens(ClientPtr client)
{
    xXineramaQueryScreensReply	rep;
    ExtensionEntry *ext;
    ScrnInfoPtr pScrn;
    VMWAREPtr pVMWARE;

    REQUEST_SIZE_MATCH(xXineramaQueryScreensReq);

    if (!(ext = CheckExtension(PANORAMIX_PROTOCOL_NAME))) {
       return BadMatch;
    }
    pScrn = ext->extPrivate;
    pVMWARE = VMWAREPTR(pScrn);

    rep.type = X_Reply;
    rep.sequenceNumber = client->sequence;
    rep.number = pVMWARE->xinerama ? pVMWARE->xineramaNumOutputs : 0;
    rep.length = rep.number * sz_XineramaScreenInfo >> 2;
    if(client->swapped) {
       register int n;
       _swaps(&rep.sequenceNumber, n);
       _swapl(&rep.length, n);
       _swapl(&rep.number, n);
    }
    WriteToClient(client, sizeof(xXineramaQueryScreensReply), (char *)&rep);

    if(pVMWARE->xinerama) {
       xXineramaScreenInfo scratch;
       int i;

       for(i = 0; i < pVMWARE->xineramaNumOutputs; i++) {
	  scratch.x_org  = pVMWARE->xineramaState[i].x_org;
	  scratch.y_org  = pVMWARE->xineramaState[i].y_org;
	  scratch.width  = pVMWARE->xineramaState[i].width;
	  scratch.height = pVMWARE->xineramaState[i].height;
	  if(client->swapped) {
	     register int n;
	     _swaps(&scratch.x_org, n);
	     _swaps(&scratch.y_org, n);
	     _swaps(&scratch.width, n);
	     _swaps(&scratch.height, n);
	  }
	  WriteToClient(client, sz_XineramaScreenInfo, (char *)&scratch);
       }
    }

    return client->noClientException;
}


/*
 *----------------------------------------------------------------------------
 *
 * VMwareXineramaDispatch --
 *
 *      Dispatcher for Xinerama commands. Calls the correct handler for
 *      each command type.
 *
 * Results:
 *      Standard response codes.
 *
 * Side effects:
 *      Side effects of individual command handlers.
 *
 *----------------------------------------------------------------------------
 */

static int
VMwareXineramaDispatch(ClientPtr client)
{
    REQUEST(xReq);
    switch (stuff->data) {
	case X_PanoramiXQueryVersion:
	     return VMwareXineramaQueryVersion(client);
	case X_PanoramiXGetState:
	     return VMwareXineramaGetState(client);
	case X_PanoramiXGetScreenCount:
	     return VMwareXineramaGetScreenCount(client);
	case X_PanoramiXGetScreenSize:
	     return VMwareXineramaGetScreenSize(client);
	case X_XineramaIsActive:
	     return VMwareXineramaIsActive(client);
	case X_XineramaQueryScreens:
	     return VMwareXineramaQueryScreens(client);
    }
    return BadRequest;
}


/*
 *----------------------------------------------------------------------------
 *
 * SVMwareXineramaQueryVersion --
 *
 *      Wrapper for QueryVersion handler that handles input from other-endian
 *      clients.
 *
 * Results:
 *      Standard response codes.
 *
 * Side effects:
 *      Side effects of unswapped implementation.
 *
 *----------------------------------------------------------------------------
 */

static int
SVMwareXineramaQueryVersion (ClientPtr client)
{
    REQUEST(xPanoramiXQueryVersionReq);
    register int n;
    _swaps(&stuff->length,n);
    REQUEST_SIZE_MATCH (xPanoramiXQueryVersionReq);
    return VMwareXineramaQueryVersion(client);
}


/*
 *----------------------------------------------------------------------------
 *
 * SVMwareXineramaGetState --
 *
 *      Wrapper for GetState handler that handles input from other-endian
 *      clients.
 *
 * Results:
 *      Standard response codes.
 *
 * Side effects:
 *      Side effects of unswapped implementation.
 *
 *----------------------------------------------------------------------------
 */

static int
SVMwareXineramaGetState(ClientPtr client)
{
    REQUEST(xPanoramiXGetStateReq);
    register int n;
    _swaps (&stuff->length, n);
    REQUEST_SIZE_MATCH(xPanoramiXGetStateReq);
    return VMwareXineramaGetState(client);
}


/*
 *----------------------------------------------------------------------------
 *
 * SVMwareXineramaGetScreenCount --
 *
 *      Wrapper for GetScreenCount handler that handles input from other-endian
 *      clients.
 *
 * Results:
 *      Standard response codes.
 *
 * Side effects:
 *      Side effects of unswapped implementation.
 *
 *----------------------------------------------------------------------------
 */

static int
SVMwareXineramaGetScreenCount(ClientPtr client)
{
    REQUEST(xPanoramiXGetScreenCountReq);
    register int n;
    _swaps (&stuff->length, n);
    REQUEST_SIZE_MATCH(xPanoramiXGetScreenCountReq);
    return VMwareXineramaGetScreenCount(client);
}


/*
 *----------------------------------------------------------------------------
 *
 * SVMwareXineramaGetScreenSize --
 *
 *      Wrapper for GetScreenSize handler that handles input from other-endian
 *      clients.
 *
 * Results:
 *      Standard response codes.
 *
 * Side effects:
 *      Side effects of unswapped implementation.
 *
 *----------------------------------------------------------------------------
 */

static int
SVMwareXineramaGetScreenSize(ClientPtr client)
{
    REQUEST(xPanoramiXGetScreenSizeReq);
    register int n;
    _swaps (&stuff->length, n);
    REQUEST_SIZE_MATCH(xPanoramiXGetScreenSizeReq);
    return VMwareXineramaGetScreenSize(client);
}


/*
 *----------------------------------------------------------------------------
 *
 * SVMwareXineramaIsActive --
 *
 *      Wrapper for IsActive handler that handles input from other-endian
 *      clients.
 *
 * Results:
 *      Standard response codes.
 *
 * Side effects:
 *      Side effects of unswapped implementation.
 *
 *----------------------------------------------------------------------------
 */

static int
SVMwareXineramaIsActive(ClientPtr client)
{
    REQUEST(xXineramaIsActiveReq);
    register int n;
    _swaps (&stuff->length, n);
    REQUEST_SIZE_MATCH(xXineramaIsActiveReq);
    return VMwareXineramaIsActive(client);
}


/*
 *----------------------------------------------------------------------------
 *
 * SVMwareXineramaQueryScreens --
 *
 *      Wrapper for QueryScreens handler that handles input from other-endian
 *      clients.
 *
 * Results:
 *      Standard response codes.
 *
 * Side effects:
 *      Side effects of unswapped implementation.
 *
 *----------------------------------------------------------------------------
 */

static int
SVMwareXineramaQueryScreens(ClientPtr client)
{
    REQUEST(xXineramaQueryScreensReq);
    register int n;
    _swaps (&stuff->length, n);
    REQUEST_SIZE_MATCH(xXineramaQueryScreensReq);
    return VMwareXineramaQueryScreens(client);
}


/*
 *----------------------------------------------------------------------------
 *
 * SVMwareXineramaDispatch --
 *
 *      Wrapper for dispatcher that handles input from other-endian clients.
 *
 * Results:
 *      Standard response codes.
 *
 * Side effects:
 *      Side effects of individual command handlers.
 *
 *----------------------------------------------------------------------------
 */

static int
SVMwareXineramaDispatch(ClientPtr client)
{
    REQUEST(xReq);
    switch (stuff->data) {
	case X_PanoramiXQueryVersion:
	     return SVMwareXineramaQueryVersion(client);
	case X_PanoramiXGetState:
	     return SVMwareXineramaGetState(client);
	case X_PanoramiXGetScreenCount:
	     return SVMwareXineramaGetScreenCount(client);
	case X_PanoramiXGetScreenSize:
	     return SVMwareXineramaGetScreenSize(client);
	case X_XineramaIsActive:
	     return SVMwareXineramaIsActive(client);
	case X_XineramaQueryScreens:
	     return SVMwareXineramaQueryScreens(client);
    }
    return BadRequest;
}


/*
 *----------------------------------------------------------------------------
 *
 * VMwareXineramaResetProc --
 *
 *      Cleanup handler called when the extension is removed.
 *
 * Results:
 *      None
 *
 * Side effects:
 *      None
 *
 *----------------------------------------------------------------------------
 */

static void
VMwareXineramaResetProc(ExtensionEntry* extEntry)
{
    /* Called by CloseDownExtensions() */

   ScrnInfoPtr pScrn = extEntry->extPrivate;
   VMWAREPtr pVMWARE = VMWAREPTR(pScrn);

   if (pVMWARE->xineramaState) {
      free(pVMWARE->xineramaState);
      pVMWARE->xineramaState = NULL;
      pVMWARE->xineramaNumOutputs = 0;
      pVMWARE->xinerama = FALSE;
   }
}


/*
 *----------------------------------------------------------------------------
 *
 * VMwareCtrl_ExitInit --
 *
 *      Initialiser for the Xinerama protocol extension.
 *
 * Results:
 *      None.
 *
 * Side effects:
 *      Protocol extension will be registered if successful.
 *
 *----------------------------------------------------------------------------
 */

void
VMwareXinerama_ExtInit(ScrnInfoPtr pScrn)
{
   ExtensionEntry *myext;
   VMWAREPtr pVMWARE = VMWAREPTR(pScrn);

#ifdef PANORAMIX
   if(!noPanoramiXExtension) {
      xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                 "Built-in Xinerama active, not initializing VMware Xinerama\n");
      pVMWARE->xinerama = FALSE;
      return;
   }
#endif

   if (!(myext = CheckExtension(PANORAMIX_PROTOCOL_NAME))) {
      if (!(myext = AddExtension(PANORAMIX_PROTOCOL_NAME, 0, 0,
                                 VMwareXineramaDispatch,
                                 SVMwareXineramaDispatch,
                                 VMwareXineramaResetProc,
                                 StandardMinorOpcode))) {
         xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                    "Failed to add VMware Xinerama extension.\n");
         return;
      }

      pVMWARE->xinerama = TRUE;

      myext->extPrivate = pScrn;

      xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                 "Initialized VMware Xinerama extension.\n");
   }
}
