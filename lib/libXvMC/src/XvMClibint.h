#ifndef _XVMCLIBINT_H
#define _XVMCLIBINT_H

#include <X11/Xlibint.h>
#include <X11/extensions/Xvproto.h>
#include <X11/extensions/XvMCproto.h>
#include <X11/extensions/XvMClib.h>

#define XvMCCheckExtension(dpy, i, val) \
  XextCheckExtension(dpy, i, xvmc_extension_name, val)

/* names in XvMCproto.h don't match the expectation of Xlib's GetReq* macros,
   so we have to provide our own implementation */
#define XvMCGetReq(name, req) \
        req = (xvmc##name##Req *) _XGetRequest(                               \
            dpy, (CARD8) info->codes->major_opcode, sizeof(xvmc##name##Req)); \
        req->xvmcReqType = xvmc_##name

_XFUNCPROTOBEGIN

Status _xvmc_create_context(
    Display *           /* dpy */,
    XvMCContext *       /* context */,
    int *               /* priv_count */,
    CARD32 **           /* priv_data */
);

Status _xvmc_destroy_context(
    Display *           /* dpy */,
    XvMCContext *       /* context */
);

Status _xvmc_create_surface(
    Display *           /* dpy */,
    XvMCContext *       /* context */,
    XvMCSurface *       /* surface */,
    int *               /* priv_count */,
    CARD32 **           /* priv_data */
);

Status _xvmc_destroy_surface(
    Display *           /* dpy */,
    XvMCSurface *       /* surface */
);

Status _xvmc_create_subpicture(
    Display *           /* dpy */,
    XvMCContext *       /* context */,
    XvMCSubpicture *    /* subpicture */,
    int *               /* priv_count */,
    CARD32 **           /* priv_data */
);

Status _xvmc_destroy_subpicture(
    Display *           /* dpy */,
    XvMCSubpicture *    /* subpicture */
);

_XFUNCPROTOEND

#endif /* XVMCLIBINT_H */
