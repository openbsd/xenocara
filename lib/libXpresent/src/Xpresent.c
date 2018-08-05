/*
 * Copyright © 2013 Keith Packard
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

#include <stdint.h>
#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xlibint.h>
#include <X11/Xutil.h>

#include <X11/extensions/extutil.h>
#include <X11/extensions/geproto.h>
#include <X11/extensions/ge.h>
#include <X11/extensions/Xge.h>

#include <X11/extensions/Xpresent.h>
#include <X11/extensions/presentproto.h>

typedef struct _XPresentExtDisplayInfo {
    struct _XPresentExtDisplayInfo      *next;          /* keep a linked list */
    Display                             *display;	/* which display this is */
    XExtCodes                           *codes;	        /* the extension protocol codes */
    int			                major_version;  /* -1 means we don't know */
    int			                minor_version;  /* -1 means we don't know */
} XPresentExtDisplayInfo;

/* replaces XExtensionInfo */
typedef struct _XPresentExtInfo {
    XPresentExtDisplayInfo              *head;          /* start of the list */
    XPresentExtDisplayInfo              *cur;           /* most recently used */
    int                                 ndisplays;      /* number of displays */
} XPresentExtInfo;

extern XPresentExtInfo XPresentExtensionInfo;
extern char XPresentExtensionName[];

XPresentExtDisplayInfo *
XPresentFindDisplay (Display *dpy);

#define XPresentHasExtension(i) ((i) && ((i)->codes))

#define XPresentCheckExtension(dpy,i,val)               \
    if (!XPresentHasExtension(i)) { return val; }

#define XPresentSimpleCheckExtension(dpy,i)     \
    if (!XPresentHasExtension(i)) { return; }

XPresentExtInfo XPresentExtensionInfo;
char XPresentExtensionName[] = PRESENT_NAME;

static int
XPresentCloseDisplay (Display *dpy, XExtCodes *codes);

static Bool
XPresentCopyCookie(Display              *dpy,
                   XGenericEventCookie  *in,
                   XGenericEventCookie  *out)
{
    int                         ret = True;
    XPresentExtDisplayInfo        *info = XPresentFindDisplay(dpy);

    if (in->extension != info->codes->major_opcode)
    {
        printf("XFixesCopyCookie: wrong extension opcode %d\n",
                in->extension);
        return False;
    }

    *out = *in;
    out->data = NULL;
    out->cookie = 0;

    switch(in->evtype) {
    case PresentConfigureNotify:
    case PresentCompleteNotify:
#if PRESENT_FUTURE_VERSION
    case PresentRedirectNotify:
#endif
        break;
    default:
        printf("XPresentCopyCookie: unknown evtype %d\n", in->evtype);
        ret = False;
    }

    if (!ret)
        printf("XPresentCopyCookie: Failed to copy evtype %d", in->evtype);
    return ret;
}

static Bool
XPresentWireToCookie(Display	                *dpy,
                     XGenericEventCookie        *cookie,
                     xEvent	                *wire_event)
{
    XPresentExtDisplayInfo      *info = XPresentFindDisplay(dpy);
    xGenericEvent               *ge = (xGenericEvent*)wire_event;

    if (ge->extension != info->codes->major_opcode)
    {
        printf("XInputWireToCookie: wrong extension opcode %d\n",
                ge->extension);
        return False;
    }

    cookie->type = ge->type & 0x7f;
    cookie->serial = _XSetLastRequestRead(dpy, (xGenericReply *) ge);
    cookie->send_event = ((ge->type & 0x80) != 0);
    cookie->display = dpy;
    cookie->extension = ge->extension;
    cookie->evtype = ge->evtype;

    switch(ge->evtype) {
    case PresentConfigureNotify:  {
        xPresentConfigureNotify *proto = (xPresentConfigureNotify *) ge;
        XPresentConfigureNotifyEvent *ce = malloc (sizeof (XPresentConfigureNotifyEvent));
        cookie->data = ce;

        ce->type = cookie->type;
        ce->serial = cookie->serial;
        ce->send_event = cookie->send_event;
        ce->display = cookie->display;
        ce->extension = cookie->extension;
        ce->evtype = cookie->evtype;

        ce->eid = proto->eid;
        ce->window = proto->window;
        ce->x = proto->x;
        ce->y = proto->y;
        ce->width = proto->width;
        ce->height = proto->height;
        ce->off_x = proto->off_x;
        ce->off_y = proto->off_y;
        ce->pixmap_width = proto->pixmap_width;
        ce->pixmap_height = proto->pixmap_height;
        ce->pixmap_flags = proto->pixmap_flags;

        break;
    }
    case PresentCompleteNotify: {
        xPresentCompleteNotify *proto = (xPresentCompleteNotify *) ge;
        XPresentCompleteNotifyEvent *ce = malloc (sizeof (XPresentCompleteNotifyEvent));
        cookie->data = ce;

        ce->type = cookie->type;
        ce->serial = cookie->serial;
        ce->send_event = cookie->send_event;
        ce->display = cookie->display;
        ce->extension = cookie->extension;
        ce->evtype = cookie->evtype;

        ce->eid = proto->eid;
        ce->window = proto->window;
        ce->serial_number = proto->serial;
        ce->ust = proto->ust;
        ce->msc = proto->msc;
        ce->kind = proto->kind;
        ce->mode = proto->mode;

        break;
    }
    case PresentIdleNotify: {
        xPresentIdleNotify *proto = (xPresentIdleNotify *) ge;
        XPresentIdleNotifyEvent *ce = malloc (sizeof (XPresentIdleNotifyEvent));
        cookie->data = ce;

        ce->type = cookie->type;
        ce->serial = cookie->serial;
        ce->send_event = cookie->send_event;
        ce->display = cookie->display;
        ce->extension = cookie->extension;
        ce->evtype = cookie->evtype;

        ce->eid = proto->eid;
        ce->window = proto->window;
        ce->serial_number = proto->serial;
        ce->pixmap = proto->pixmap;
        ce->idle_fence = proto->idle_fence;

        break;
    }
#if PRESENT_FUTURE_VERSION
    case PresentRedirectNotify: {
        xPresentRedirectNotify *proto = (xPresentRedirectNotify *) ge;
        xPresentNotify *xNotify = (xPresentNotify *) (proto + 1);
        int nnotifies = (((proto->length + 8) - (sizeof (xPresentRedirectNotify) >> 2))) >> 1;
        XPresentRedirectNotifyEvent *re = malloc (sizeof (XPresentRedirectNotifyEvent) + nnotifies * sizeof (XPresentNotify));
        XPresentNotify *XNotify = (XPresentNotify *) (re + 1);
        int i;
        cookie->data = re;

        re->type = cookie->type;
        re->serial = cookie->serial;
        re->send_event = cookie->send_event;
        re->display = cookie->display;
        re->extension = cookie->extension;
        re->evtype = cookie->evtype;

        re->eid = proto->eid;
        re->event_window = proto->event_window;

        re->window = proto->window;
        re->pixmap = proto->pixmap;
        re->serial_number = proto->serial;

        re->valid_region = proto->valid_region;
        re->update_region = proto->update_region;

        re->valid_rect = *(XRectangle *) &(proto->valid_rect);
        re->update_rect = *(XRectangle *) &(proto->update_rect);

        re->x_off = proto->x_off;
        re->y_off = proto->y_off;
        re->target_crtc = proto->target_crtc;

        re->wait_fence = proto->wait_fence;
        re->idle_fence = proto->idle_fence;

        re->options = proto->options;

        re->target_msc = proto->target_msc;
        re->divisor = proto->divisor;
        re->remainder = proto->remainder;

        re->nnotifies = nnotifies;
        re->notifies = XNotify;
        for (i = 0; i < nnotifies; i++) {
            XNotify[i].window = xNotify[i].window;
            XNotify[i].serial = xNotify[i].serial;
        }
        break;
    }
#endif
    default:
        printf("XPresentWireToCookie: Unknown generic event. type %d\n", ge->evtype);

    }
    return False;
}

/*
 * XPresentExtAddDisplay - add a display to this extension. (Replaces
 * XextAddDisplay)
 */
static XPresentExtDisplayInfo *
XPresentExtAddDisplay (XPresentExtInfo *extinfo,
                      Display        *dpy,
                      char           *ext_name)
{
    XPresentExtDisplayInfo    *info;

    info = (XPresentExtDisplayInfo *) Xmalloc (sizeof (XPresentExtDisplayInfo));
    if (!info) return NULL;
    info->display = dpy;

    info->codes = XInitExtension (dpy, ext_name);

    /*
     * if the server has the extension, then we can initialize the
     * appropriate function vectors
     */
    if (info->codes) {
	xPresentQueryVersionReply	rep;
	xPresentQueryVersionReq	        *req;

        XESetCloseDisplay (dpy, info->codes->extension, XPresentCloseDisplay);

        XESetWireToEventCookie(dpy, info->codes->major_opcode, XPresentWireToCookie);
        XESetCopyEventCookie(dpy, info->codes->major_opcode, XPresentCopyCookie);

	/*
	 * Get the version info
	 */
	LockDisplay (dpy);
	GetReq (PresentQueryVersion, req);
	req->reqType = info->codes->major_opcode;
	req->presentReqType = X_PresentQueryVersion;
	req->majorVersion = PRESENT_MAJOR;
	req->minorVersion = PRESENT_MINOR;
	if (!_XReply (dpy, (xReply *) &rep, 0, xTrue))
	{
	    UnlockDisplay (dpy);
	    SyncHandle ();
	    Xfree(info);
	    return NULL;
	}
	info->major_version = rep.majorVersion;
	info->minor_version = rep.minorVersion;
	UnlockDisplay (dpy);
	SyncHandle ();
    } else {
	/* The server doesn't have this extension.
	 * Use a private Xlib-internal extension to hang the close_display
	 * hook on so that the "cache" (extinfo->cur) is properly cleaned.
	 * (XBUG 7955)
	 */
	XExtCodes *codes = XAddExtension(dpy);
	if (!codes) {
	    XFree(info);
	    return NULL;
	}
        XESetCloseDisplay (dpy, codes->extension, XPresentCloseDisplay);
    }

    /*
     * now, chain it onto the list
     */
    _XLockMutex(_Xglobal_lock);
    info->next = extinfo->head;
    extinfo->head = info;
    extinfo->cur = info;
    extinfo->ndisplays++;
    _XUnlockMutex(_Xglobal_lock);
    return info;
}


/*
 * XPresentExtRemoveDisplay - remove the indicated display from the
 * extension object. (Replaces XextRemoveDisplay.)
 */
static int
XPresentExtRemoveDisplay (XPresentExtInfo *extinfo, Display *dpy)
{
    XPresentExtDisplayInfo *info, *prev;

    /*
     * locate this display and its back link so that it can be removed
     */
    _XLockMutex(_Xglobal_lock);
    prev = NULL;
    for (info = extinfo->head; info; info = info->next) {
	if (info->display == dpy) break;
	prev = info;
    }
    if (!info) {
	_XUnlockMutex(_Xglobal_lock);
	return 0;		/* hmm, actually an error */
    }

    /*
     * remove the display from the list; handles going to zero
     */
    if (prev)
	prev->next = info->next;
    else
	extinfo->head = info->next;

    extinfo->ndisplays--;
    if (info == extinfo->cur) extinfo->cur = NULL;  /* flush cache */
    _XUnlockMutex(_Xglobal_lock);

    Xfree ((char *) info);
    return 1;
}

/*
 * XPresentExtFindDisplay - look for a display in this extension; keeps a
 * cache of the most-recently used for efficiency. (Replaces
 * XextFindDisplay.)
 */
static XPresentExtDisplayInfo *
XPresentExtFindDisplay (XPresentExtInfo *extinfo,
                        Display         *dpy)
{
    XPresentExtDisplayInfo *info;

    /*
     * see if this was the most recently accessed display
     */
    if ((info = extinfo->cur) && info->display == dpy)
	return info;

    /*
     * look for display in list
     */
    _XLockMutex(_Xglobal_lock);
    for (info = extinfo->head; info; info = info->next) {
	if (info->display == dpy) {
	    extinfo->cur = info;     /* cache most recently used */
	    _XUnlockMutex(_Xglobal_lock);
	    return info;
	}
    }
    _XUnlockMutex(_Xglobal_lock);

    return NULL;
}

XPresentExtDisplayInfo *
XPresentFindDisplay (Display *dpy)
{
    XPresentExtDisplayInfo *info;

    info = XPresentExtFindDisplay (&XPresentExtensionInfo, dpy);
    if (!info)
	info = XPresentExtAddDisplay (&XPresentExtensionInfo, dpy,
				    XPresentExtensionName);
    return info;
}

static int
XPresentCloseDisplay (Display *dpy, XExtCodes *codes)
{
    return XPresentExtRemoveDisplay (&XPresentExtensionInfo, dpy);
}

Bool
XPresentQueryExtension (Display *dpy,
                        int *major_opcode_return,
			int *event_base_return,
			int *error_base_return)
{
    XPresentExtDisplayInfo *info = XPresentFindDisplay (dpy);

    if (XPresentHasExtension(info))
    {
        if (major_opcode_return)
            *major_opcode_return = info->codes->major_opcode;
        if (event_base_return)
            *event_base_return = info->codes->first_event;
        if (error_base_return)
            *error_base_return = info->codes->first_error;
	return True;
    }
    else
	return False;
}

Status
XPresentQueryVersion (Display *dpy,
		    int	    *major_version_return,
		    int	    *minor_version_return)
{
    XPresentExtDisplayInfo	*info = XPresentFindDisplay (dpy);

    XPresentCheckExtension (dpy, info, 0);

    *major_version_return = info->major_version;
    *minor_version_return = info->minor_version;
    return 1;
}

int
XPresentVersion (void)
{
    return PRESENT_VERSION;
}

void
XPresentPixmap(Display *dpy,
               Window window,
               Pixmap pixmap,
               uint32_t serial,
               XserverRegion valid,
               XserverRegion update,
               int x_off,
               int y_off,
               RRCrtc target_crtc,
               XSyncFence wait_fence,
               XSyncFence idle_fence,
               uint32_t options,
               uint64_t target_msc,
               uint64_t divisor,
               uint64_t remainder,
               XPresentNotify *notifies,
               int nnotifies)
{
    XPresentExtDisplayInfo	*info = XPresentFindDisplay (dpy);
    xPresentPixmapReq           *req;
    long                        len = ((long) nnotifies) << 1;

    XPresentSimpleCheckExtension (dpy, info);

    LockDisplay (dpy);
    GetReq(PresentPixmap, req);
    req->reqType = info->codes->major_opcode;
    req->presentReqType = X_PresentPixmap;
    req->window = window;
    req->pixmap = pixmap;
    req->serial = serial;
    req->valid = valid;
    req->update = update;
    req->x_off = x_off;
    req->y_off = y_off;
    req->target_crtc = target_crtc;
    req->wait_fence = wait_fence;
    req->idle_fence = idle_fence;
    req->options = options;
    req->target_msc = target_msc;
    req->divisor = divisor;
    req->remainder = remainder;
    SetReqLen(req, len, len);
    Data32(dpy, (CARD32 *) notifies, len);
    UnlockDisplay (dpy);
    SyncHandle();
}

void
XPresentNotifyMSC(Display *dpy,
                  Window window,
                  uint32_t serial,
                  uint64_t target_msc,
                  uint64_t divisor,
                  uint64_t remainder)
{
    XPresentExtDisplayInfo	*info = XPresentFindDisplay (dpy);
    xPresentNotifyMSCReq        *req;

    XPresentSimpleCheckExtension (dpy, info);

    LockDisplay (dpy);
    GetReq(PresentNotifyMSC, req);
    req->reqType = info->codes->major_opcode;
    req->presentReqType = X_PresentNotifyMSC;
    req->window = window;
    req->serial = serial;
    req->target_msc = target_msc;
    req->divisor = divisor;
    req->remainder = remainder;
    UnlockDisplay (dpy);
    SyncHandle();
}

XID
XPresentSelectInput(Display *dpy,
                    Window window,
                    unsigned event_mask)
{
    XPresentExtDisplayInfo	*info = XPresentFindDisplay (dpy);
    XID                         eid;
    xPresentSelectInputReq      *req;

    XPresentCheckExtension (dpy, info, 0);
    LockDisplay (dpy);
    GetReq(PresentSelectInput, req);
    req->reqType = info->codes->major_opcode;
    req->presentReqType = X_PresentSelectInput;
    req->eid = eid = XAllocID(dpy);
    req->window = window;
    req->eventMask = event_mask;
    UnlockDisplay (dpy);
    SyncHandle();
    return eid;
}

void
XPresentFreeInput(Display *dpy,
                  Window window,
                  XID event_id)
{
    XPresentExtDisplayInfo	*info = XPresentFindDisplay (dpy);
    xPresentSelectInputReq      *req;

    XPresentSimpleCheckExtension (dpy, info);
    LockDisplay (dpy);
    GetReq(PresentSelectInput, req);
    req->reqType = info->codes->major_opcode;
    req->presentReqType = X_PresentSelectInput;
    req->eid = event_id;
    req->window = window;
    req->eventMask = 0;
    UnlockDisplay (dpy);
    SyncHandle();
}

uint32_t
XPresentQueryCapabilities(Display *dpy,
                          XID target)
{
    XPresentExtDisplayInfo	        *info = XPresentFindDisplay (dpy);
    xPresentQueryCapabilitiesReq        *req;
    xPresentQueryCapabilitiesReply      rep;

    XPresentCheckExtension (dpy, info, 0);
    LockDisplay (dpy);
    GetReq(PresentQueryCapabilities, req);
    req->reqType = info->codes->major_opcode;
    req->presentReqType = X_PresentQueryCapabilities;
    req->target = target;

    if (!_XReply (dpy, (xReply *) &rep, 0, xFalse)) {
	UnlockDisplay (dpy);
	SyncHandle ();
        return 0;
    }

    UnlockDisplay (dpy);
    SyncHandle();
    return rep.capabilities;
}

