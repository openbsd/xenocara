
#include "Manager.h"
#include "Client.h"

#include <X11/Xutil.h>

const char *const Client::m_defaultLabel = "incognito";



Client::Client(WindowManager *const wm, Window w) :
    m_window(w),
    m_transient(None),
    m_revert(0),
    m_fixedSize(False),
    m_state(WithdrawnState),
    m_managed(False),
    m_reparenting(False),
    m_stubborn(False),
    m_lastPopTime(0L),
    m_colormap(None),
    m_colormapWinCount(0),
    m_colormapWindows(NULL),
    m_windowColormaps(NULL),
    m_windowManager(wm)
{
    XWindowAttributes attr;
    XGetWindowAttributes(display(), m_window, &attr);

    m_x = attr.x;
    m_y = attr.y;
    m_w = attr.width;
    m_h = attr.height;
    m_bw = attr.border_width;
    m_name = m_iconName = 0;
    m_sizeHints.flags = 0L;

    m_label = NewString(m_defaultLabel);
    m_border = new Border(this, w);

    if (attr.map_state == IsViewable) manage(True);
}


Client::~Client()
{
    // empty
}    


void Client::release()
{
    // assume wm called for this, and will remove me from its list itself

//    fprintf(stderr, "deleting client %p\n",this);

    if (m_window == None) {
	fprintf(stderr,
		"wm2: invalid parent in Client::release (released twice?)\n");
    }

    windowManager()->skipInRevert(this, m_revert);

//    fprintf(stderr, "deleting %lx\n",m_window);

    if (isHidden()) unhide(False);

    delete m_border;
    m_window = None;

    if (isActive()) {
	if (CONFIG_CLICK_TO_FOCUS) {
	    if (m_revert) {
		windowManager()->setActiveClient(m_revert);
		m_revert->activate();
	    } else windowManager()->setActiveClient(0);// windowManager()->clearFocus();
	} else {
	    windowManager()->setActiveClient(0);
	}
    }

    if (m_colormapWinCount > 0) {
	XFree((char *)m_colormapWindows);
	free((char *)m_windowColormaps); // not allocated through X
    }

    if (m_iconName) XFree(m_iconName);
    if (m_name)     XFree(m_name);
    if (m_label) free((void *)m_label);

    delete this;
}


void Client::unreparent()
{
    XWindowChanges wc;

    if (!isWithdrawn()) {
	gravitate(True);
	XReparentWindow(display(), m_window, root(), m_x, m_y);
    }

    wc.border_width = m_bw;
    XConfigureWindow(display(), m_window, CWBorderWidth, &wc);

    XSync(display(), True);
}


void Client::installColormap()
{
    Client *cc = 0;
    int i, found;

    if (m_colormapWinCount != 0) {

	found = 0;

	for (i = m_colormapWinCount - 1; i >= 0; --i) {
	    windowManager()->installColormap(m_windowColormaps[i]);
	    if (m_colormapWindows[i] == m_window) ++found;
	}

	if (found == 0) {
	    windowManager()->installColormap(m_colormap);
	}

    } else if (m_transient != None &&
	       (cc = windowManager()->windowToClient(m_transient))) {

	cc->installColormap();
    } else {
	windowManager()->installColormap(m_colormap);
    }
}


void Client::manage(Boolean mapped)
{
    Boolean shouldHide, reshape;
    XWMHints *hints;
    Display *d = display();
    long mSize;
    int state;

    XSelectInput(d, m_window, ColormapChangeMask | EnterWindowMask |
		 PropertyChangeMask | FocusChangeMask);

    m_iconName = getProperty(XA_WM_ICON_NAME);
    m_name = getProperty(XA_WM_NAME);
    setLabel();

    getColormaps();
    getProtocols();
    getTransient();

    hints = XGetWMHints(d, m_window);

    if (!getState(&state)) {
	state = hints ? hints->initial_state : NormalState;
    }

    shouldHide = (state == IconicState);
    if (hints) XFree(hints);

    if (XGetWMNormalHints(d, m_window, &m_sizeHints, &mSize) == 0 ||
	m_sizeHints.flags == 0) {
	m_sizeHints.flags = PSize;
    }

    m_fixedSize = False;
//    if ((m_sizeHints.flags & (USSize | PSize))) m_fixedSize = True;
    if ((m_sizeHints.flags & (PMinSize | PMaxSize)) == (PMinSize | PMaxSize) &&
	(m_sizeHints.min_width  == m_sizeHints.max_width &&
	 m_sizeHints.min_height == m_sizeHints.max_height)) m_fixedSize = True;

    reshape = !mapped;

    if (m_fixedSize) {
	if ((m_sizeHints.flags & USPosition)) reshape = False;
	if ((m_sizeHints.flags & PPosition) && shouldHide) reshape = False;
	if ((m_transient != None)) reshape = False;
    }

    if ((m_sizeHints.flags & PBaseSize)) {
	m_minWidth  = m_sizeHints.base_width;
	m_minHeight = m_sizeHints.base_height;
    } else if ((m_sizeHints.flags & PMinSize)) {
	m_minWidth  = m_sizeHints.min_width;
	m_minHeight = m_sizeHints.min_height;
    } else {
	m_minWidth = m_minHeight = 50;
    }

    // act

    gravitate(False);

    // zeros are iffy, should be calling some Manager method
    int dw = DisplayWidth(display(), 0), dh = DisplayHeight(display(), 0);

    if (m_w < m_minWidth) {
	m_w = m_minWidth; m_fixedSize = False; reshape = True;
    }
    if (m_h < m_minHeight) {
	m_h = m_minHeight; m_fixedSize = False; reshape = True;
    }

    if (m_w > dw - 8) m_w = dw - 8;
    if (m_h > dh - 8) m_h = dh - 8;

    if (m_x > dw - m_border->xIndent()) {
	m_x = dw - m_border->xIndent();
    }

    if (m_y > dh - m_border->yIndent()) {
	m_y = dh - m_border->yIndent();
    }

    if (m_x < m_border->xIndent()) m_x = m_border->xIndent();
    if (m_y < m_border->yIndent()) m_y = m_border->yIndent();
    
    m_border->configure(m_x, m_y, m_w, m_h, 0L, Above);

    if (mapped) m_reparenting = True;
    if (reshape && !m_fixedSize) XResizeWindow(d, m_window, m_w, m_h);
    XSetWindowBorderWidth(d, m_window, 0);

    m_border->reparent();

    // (support for shaped windows absent)

    XAddToSaveSet(d, m_window);
    m_managed = True;

    if (shouldHide) hide();
    else {
	XMapWindow(d, m_window);
	m_border->map();
	setState(NormalState);

	if (CONFIG_CLICK_TO_FOCUS ||
	    (m_transient != None && activeClient() &&
	    activeClient()->m_window == m_transient)) {
	    activate();
	    mapRaised();
	} else {
	    deactivate();
	}
    }
    
    if (activeClient() && !isActive()) {
	activeClient()->installColormap();
    }

    if (CONFIG_AUTO_RAISE) {
	m_windowManager->stopConsideringFocus();
	focusIfAppropriate(False);
    }
}


void Client::selectOnMotion(Window w, Boolean select)
{
    if (!CONFIG_AUTO_RAISE) return;
    if (!w || w == root()) return;

    if (w == m_window || m_border->hasWindow(w)) {
	XSelectInput(display(), m_window, // not "w"
		     ColormapChangeMask | EnterWindowMask |
		     PropertyChangeMask | FocusChangeMask |
		     (select ? PointerMotionMask : 0L));
    } else {
	XSelectInput(display(), w, select ? PointerMotionMask : 0L);
    }
}


void Client::decorate(Boolean active)
{
    m_border->decorate(active, m_w, m_h);
}


void Client::activate()
{
//    fprintf(stderr, "Client::activate (this = %p, window = %x, parent = %x)\n",
//	    this, m_window, parent());

    if (parent() == root()) {
	fprintf(stderr, "wm2: warning: bad parent in Client::activate\n");
	return;
    }

    if (!m_managed || isHidden() || isWithdrawn()) return;

    if (isActive()) {
	decorate(True);
	if (CONFIG_AUTO_RAISE || CONFIG_RAISE_ON_FOCUS) mapRaised();
	return;
    }

    if (activeClient()) {
	activeClient()->deactivate();
	// & some other-screen business
    }

    XUngrabButton(display(), AnyButton, AnyModifier, parent());

    XSetInputFocus(display(), m_window, RevertToPointerRoot,
		   windowManager()->timestamp(False));

    if (m_protocol & PtakeFocus) {
	sendMessage(Atoms::wm_protocols, Atoms::wm_takeFocus);
    }

    // now set revert of window that reverts to this one so as to
    // revert to the window this one used to revert to (huh?)

    windowManager()->skipInRevert(this, m_revert);

    m_revert = activeClient();
    while (m_revert && !m_revert->isNormal()) m_revert = m_revert->revertTo();

    windowManager()->setActiveClient(this);
//    if (CONFIG_AUTO_RAISE || CONFIG_RAISE_ON_FOCUS) mapRaised();
    decorate(True);

    installColormap();		// new!
}


void Client::deactivate()	// called from wm?
{
//    fprintf(stderr, 
//	    "Client::deactivate (this = %p, window = %x, parent = %x)\n",
//	    this, m_window, parent());

    if (parent() == root()) {
	fprintf(stderr, "wm2: warning: bad parent in Client::deactivate\n");
	return;
    }

    XGrabButton(display(), AnyButton, AnyModifier, parent(), False,
		ButtonPressMask | ButtonReleaseMask,
		GrabModeAsync, GrabModeSync, None, None);

    decorate(False);
}


void Client::sendMessage(Atom a, long l)
{
    XEvent ev;
    int status;
    long mask;

    memset(&ev, 0, sizeof(ev));
    ev.xclient.type = ClientMessage;
    ev.xclient.window = m_window;
    ev.xclient.message_type = a;
    ev.xclient.format = 32;
    ev.xclient.data.l[0] = l;
    ev.xclient.data.l[1] = windowManager()->timestamp(False);
    mask = 0L;
    status = XSendEvent(display(), m_window, False, mask, &ev);

    if (status == 0) {
	fprintf(stderr, "wm2: warning: Client::sendMessage failed\n");
    }
}


static int getProperty_aux(Display *d, Window w, Atom a, Atom type, long len,
			   unsigned char **p)
{
    Atom realType;
    int format;
    unsigned long n, extra;
    int status;

    status = XGetWindowProperty(d, w, a, 0L, len, False, type, &realType,
				&format, &n, &extra, p);

    if (status != Success || *p == 0) return -1;
    if (n == 0) XFree((void *) *p);

    return n;
}


char *Client::getProperty(Atom a)
{
    unsigned char *p;
    if (getProperty_aux(display(), m_window, a, XA_STRING, 100L, &p) <= 0) {
	return NULL;
    }
    return (char *)p;
}


int Client::getAtomProperty(Atom a, Atom type)
{
    char **p, *x;
    if (getProperty_aux(display(), m_window, a, type, 1L,
			(unsigned char **)&p) <= 0) {
	return 0;
    }

    x = *p;
    XFree((void *)p);
    return (int)x;
}


int Client::getIntegerProperty(Atom a)
{
    return getAtomProperty(a, XA_INTEGER);
}


void Client::setState(int state)
{
    m_state = state;

//    fprintf(stderr, "state set to %d\n",state);

    long data[2];
    data[0] = (long)state;
    data[1] = (long)None;

    XChangeProperty(display(), m_window, Atoms::wm_state, Atoms::wm_state,
		    32, PropModeReplace, (unsigned char *)data, 2);
}


Boolean Client::getState(int *state)
{
    long *p = 0;

    if (getProperty_aux(display(), m_window, Atoms::wm_state, Atoms::wm_state,
			2L, (unsigned char **)&p) <= 0) {
	return False;
    }

    *state = (int) *p;
    XFree((char *)p);
    return True;
}


void Client::getProtocols()
{
    long n;
    Atom *p;

    m_protocol = 0;
    if ((n = getProperty_aux(display(), m_window, Atoms::wm_protocols, XA_ATOM,
			     20L, (unsigned char **)&p)) <= 0) {
	return;
    }

    for (int i = 0; i < n; ++i) {
	if (p[i] == Atoms::wm_delete) {
	    m_protocol |= Pdelete;
	} else if (p[i] == Atoms::wm_takeFocus) {
	    m_protocol |= PtakeFocus;
	}
    }

    XFree((char *) p);
}


void Client::gravitate(Boolean invert)
{
    int gravity;
    int w = 0, h = 0, xdelta, ydelta;

    // possibly shouldn't work if we haven't been managed yet?

    gravity = NorthWestGravity;
    if (m_sizeHints.flags & PWinGravity) gravity = m_sizeHints.win_gravity;

    xdelta = m_bw - m_border->xIndent();
    ydelta = m_bw - m_border->yIndent();

    // note that right and bottom borders have indents of 1

    switch (gravity) {

    case NorthWestGravity:
	break;

    case NorthGravity:
	w = xdelta;
	break;

    case NorthEastGravity:
	w = xdelta + m_bw-1;
	break;

    case WestGravity:
	h = ydelta;
	break;

    case CenterGravity:
    case StaticGravity:
	w = xdelta;
	h = ydelta;
	break;

    case EastGravity:
	w = xdelta + m_bw-1;
	h = ydelta;
	break;

    case SouthWestGravity:
	h = ydelta + m_bw-1;
	break;

    case SouthGravity:
	w = xdelta;
	h = ydelta + m_bw-1;
	break;

    case SouthEastGravity:
	w = xdelta + m_bw-1;
	h = ydelta + m_bw-1;
	break;

    default:
	fprintf(stderr, "wm2: bad window gravity %d for window 0x%lx\n",
		gravity, m_window);
	return;
    }

    w += m_border->xIndent();
    h += m_border->yIndent();

    if (invert) { w = -w; h = -h; }

    m_x += w;
    m_y += h;
}


Boolean Client::setLabel(void)
{
    const char *newLabel;

    if (m_name) newLabel = m_name;
    else if (m_iconName) newLabel = m_iconName;
    else newLabel = m_defaultLabel;

    if (!m_label) {

	m_label = NewString(newLabel);
	return True;

    } else if (strcmp(m_label, newLabel)) {

	free((void *)m_label);
	m_label = NewString(newLabel);
	return True;

    } else return True;//False;// dammit!
}


void Client::getColormaps(void)
{
    int i, n;
    Window *cw;
    XWindowAttributes attr;

    if (!m_managed) {
	XGetWindowAttributes(display(), m_window, &attr);
	m_colormap = attr.colormap;

//	fprintf(stderr, "colormap for %s is %p\n",m_label, (void *)m_colormap);
    }

    n = getProperty_aux(display(), m_window, Atoms::wm_colormaps, XA_WINDOW,
			100L, (unsigned char **)&cw);

    if (m_colormapWinCount != 0) {
	XFree((char *)m_colormapWindows);
	free((char *)m_windowColormaps);
    }

    if (n <= 0) {
	m_colormapWinCount = 0;
	return;
    }
    
    m_colormapWinCount = n;
    m_colormapWindows = cw;

    m_windowColormaps = (Colormap *)malloc(n * sizeof(Colormap));

    for (i = 0; i < n; ++i) {
	if (cw[i] == m_window) {
	    m_windowColormaps[i] = m_colormap;
	} else {
	    XSelectInput(display(), cw[i], ColormapChangeMask);
	    XGetWindowAttributes(display(), cw[i], &attr);
	    m_windowColormaps[i] = attr.colormap;
	}
    }
}


void Client::getTransient()
{
    Window t = None;

    if (XGetTransientForHint(display(), m_window, &t) != 0) {

	if (windowManager()->windowToClient(t) == this) {
	    fprintf(stderr,
		    "wm2: warning: client \"%s\" thinks it's a transient "
		    "for\nitself -- ignoring WM_TRANSIENT_FOR property...\n",
		    m_label ? m_label : "(no name)");
	    m_transient = None;
	} else {		
	    m_transient = t;
	}
    } else {
	m_transient = None;
    }
}


void Client::hide()
{
    if (isHidden()) {
	fprintf(stderr, "wm2: Client already hidden in Client::hide\n");
	return;
    }

    m_border->unmap();
    XUnmapWindow(display(), m_window);

//    if (isActive()) windowManager()->setActiveClient(0);
    if (isActive()) windowManager()->clearFocus();

    setState(IconicState);
    windowManager()->addToHiddenList(this);
}


void Client::unhide(Boolean map)
{
    if (!isHidden()) {
	fprintf(stderr, "wm2: Client not hidden in Client::unhide\n");
	return;
    }

    windowManager()->removeFromHiddenList(this);

    if (map) {
	setState(NormalState);
	XMapWindow(display(), m_window);
	mapRaised();

	if (CONFIG_AUTO_RAISE) focusIfAppropriate(False);
	else if (CONFIG_CLICK_TO_FOCUS) activate();
    }
}


void Client::sendConfigureNotify()
{
    XConfigureEvent ce;

    ce.type   = ConfigureNotify;
    ce.event  = m_window;
    ce.window = m_window;

    ce.x = m_x;
    ce.y = m_y;
    ce.width  = m_w;
    ce.height = m_h;
    ce.border_width = m_bw;
    ce.above = None;
    ce.override_redirect = 0;

    XSendEvent(display(), m_window, False, StructureNotifyMask, (XEvent*)&ce);
}


void Client::withdraw(Boolean changeState)
{
//    fprintf(stderr,"withdrawing\n");

    m_border->unmap();

    gravitate(True);
    XReparentWindow(display(), m_window, root(), m_x, m_y);

    gravitate(False);

    if (changeState) {
	XRemoveFromSaveSet(display(), m_window);
	setState(WithdrawnState);
    }

    ignoreBadWindowErrors = True;
    XSync(display(), False);
    ignoreBadWindowErrors = False;
}


void Client::rename()
{
    m_border->configure(0, 0, m_w, m_h, CWWidth | CWHeight, Above);
}


void Client::mapRaised()
{
    m_border->mapRaised();
    windowManager()->raiseTransients(this);
}


void Client::kill()
{
    if (m_protocol & Pdelete) {
	sendMessage(Atoms::wm_protocols, Atoms::wm_delete);
    } else {
	XKillClient(display(), m_window);
    }
}


void Client::ensureVisible()
{
    int mx = DisplayWidth(display(), 0) - 1; // hack
    int my = DisplayHeight(display(), 0) - 1;
    int px = m_x;
    int py = m_y;
    
    if (m_x + m_w > mx) m_x = mx - m_w;
    if (m_y + m_h > my) m_y = my - m_h;
    if (m_x < 0) m_x = 0;
    if (m_y < 0) m_y = 0;

    if (m_x != px || m_y != py) m_border->moveTo(m_x, m_y);
}


void Client::lower()
{
    m_border->lower();
}


