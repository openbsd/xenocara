
#include "Manager.h"
#include "Client.h"
#include <sys/time.h>

#define AllButtonMask	( Button1Mask | Button2Mask | Button3Mask \
			| Button4Mask | Button5Mask )
#define ButtonMask	( ButtonPressMask | ButtonReleaseMask )
#define DragMask        ( ButtonMask | ButtonMotionMask )
#define MenuMask	( ButtonMask | ButtonMotionMask | ExposureMask )
#define MenuGrabMask	( ButtonMask | ButtonMotionMask | StructureNotifyMask )


void WindowManager::eventButton(XButtonEvent *e)
{
    if (e->button == Button3) {
	circulate(e->window == e->root);
	return;
    }

    Client *c = windowToClient(e->window);

    if (e->window == e->root) {

	if (e->button == Button1) menu(e);

    } else if (c) {

	c->eventButton(e);
	return;
    }
}


void WindowManager::circulate(Boolean activeFirst)
{
    Client *c = 0;
    if (activeFirst) c = m_activeClient;

    if (!c) {

	int i, j;

	if (!m_activeClient) i = -1;
	else {
	    for (i = 0; i < m_clients.count(); ++i) {
		if (m_clients.item(i) == m_activeClient) break;
	    }

	    if (i >= m_clients.count()-1) i = -1;
	}

	for (j = i + 1;
	     (!m_clients.item(j)->isNormal() ||
	      m_clients.item(j)->isTransient()); ++j) {
	    if (j >= m_clients.count() - 1) j = -1;
	    if (j == i) return; // no suitable clients
	}

	c = m_clients.item(j);
    }

    c->activateAndWarp();
}


void Client::activateAndWarp()
{
    mapRaised();
    ensureVisible();
    XWarpPointer(display(), None, parent(), 0, 0, 0, 0,
		 m_border->xIndent() / 2, m_border->xIndent() + 8);
    activate();
}


void Client::eventButton(XButtonEvent *e)
{
    if (e->type != ButtonPress) return;

    mapRaised();

    if (e->button == Button1) {
	if (m_border->hasWindow(e->window)) {

	    m_border->eventButton(e);
	}
    }

    if (!isNormal() || isActive() || e->send_event) return;
    activate();
}


static int nobuttons(XButtonEvent *e) // straight outta 9wm
{
    int state;
    state = (e->state & AllButtonMask);
    return (e->type == ButtonRelease) && (state & (state - 1)) == 0;
}


int WindowManager::attemptGrab(Window w, Window constrain, int mask, int t)
{
    int status;
    if (t == 0) t = timestamp(False);
    status = XGrabPointer(display(), w, False, mask, GrabModeAsync,
			  GrabModeAsync, constrain, None, t);
    return status;
}


void WindowManager::releaseGrab(XButtonEvent *e)
{
    XEvent ev;
    if (!nobuttons(e)) {
	for (;;) {
	    XMaskEvent(display(), ButtonMask | ButtonMotionMask, &ev);
	    if (ev.type == MotionNotify) continue;
	    e = &ev.xbutton;
	    if (nobuttons(e)) break;
	}
    }

    XUngrabPointer(display(), e->time);
    m_currentTime = e->time;
}


void WindowManager::menu(XButtonEvent *e)
{
    if (e->window == m_menuWindow) return;
    
    int i;
    ClientList clients;
    Boolean allowExit = False;

#define MENU_LABEL(n) ((n)==0 ? m_menuCreateLabel : \
		       (allowExit && ((n) > clients.count())) ? "[Exit wm2]" \
		       : clients.item((n)-1)->label())

    for (i = 0; i < m_hiddenClients.count(); ++i) {
	clients.append(m_hiddenClients.item(i));
    }
    int nh = clients.count() + 1;

    if (CONFIG_EVERYTHING_ON_ROOT_MENU) {
	for (i = 0; i < m_clients.count(); ++i) {
	    if (m_clients.item(i)->isNormal()) {
		clients.append(m_clients.item(i));
	    }
	}
    }
    int n = clients.count() + 1;

    int mx = DisplayWidth (display(), m_screenNumber) - 1;
    int my = DisplayHeight(display(), m_screenNumber) - 1;

    allowExit = ((e->x > mx-3) && (e->y > my-3));
    if (allowExit) n += 1;

    int width, maxWidth = 10;
    for (i = 0; i < n; ++i) {
	width = XTextWidth(m_menuFont, MENU_LABEL(i), strlen(MENU_LABEL(i)));
	if (width > maxWidth) maxWidth = width;
    }
    maxWidth += 32;

    int selecting = -1, prev = -1;
    int entryHeight = m_menuFont->ascent + m_menuFont->descent + 4;
    int totalHeight = entryHeight * n + 13; 
    int x = e->x - maxWidth/2;
    int y = e->y - 2;
    Boolean warp = False;

    if (x < 0) {
	e->x -= x; x = 0; warp = True;
    } else if (x + maxWidth >= mx) {
	e->x -= x + maxWidth - mx; x = mx - maxWidth; warp = True;
    }

    if (y < 0) {
	e->y -= y; y = 0; warp = True;
    } else if (y + totalHeight >= my) {
	e->y -= y + totalHeight - my; y = my - totalHeight; warp = True;
    }

    if (warp) XWarpPointer(display(), None, root(),
			   None, None, None, None, e->x, e->y);

    XMoveResizeWindow(display(), m_menuWindow, x, y, maxWidth, totalHeight);
    XSelectInput(display(), m_menuWindow, MenuMask);
    XMapRaised(display(), m_menuWindow);

    if (attemptGrab(m_menuWindow, None, MenuGrabMask, e->time) != GrabSuccess){
	XUnmapWindow(display(), m_menuWindow);
	return;
    }

    Boolean done = False;
    Boolean drawn = False;
    XEvent event;

    while (!done) {
	XMaskEvent(display(), MenuMask, &event);
	
	switch (event.type) {

	default:
	    fprintf(stderr, "wm2: unknown event type %d\n", event.type);
	    break;

	case ButtonPress:
	    break;

	case ButtonRelease:
	    if (drawn) {

		if (event.xbutton.button != e->button) break;
		x = event.xbutton.x;
		y = event.xbutton.y - 11;
		i = y / entryHeight;

		if (selecting >= 0 && y >= selecting * entryHeight - 3 &&
		    y <= (selecting+1) * entryHeight - 3) i = selecting;

		if (x < 0 || x > maxWidth || y < -3) i = -1;
		else if (i < 0 || i >= n) i = -1;

	    } else {
		selecting = -1;
	    }

	    if (!nobuttons(&event.xbutton)) i = -1;
	    releaseGrab(&event.xbutton);
	    XUnmapWindow(display(), m_menuWindow);
	    selecting = i;
	    done = True;
	    break;

	case MotionNotify:
	    if (!drawn) break;
	    x = event.xbutton.x;
	    y = event.xbutton.y - 11;
	    prev = selecting;
	    selecting = y / entryHeight;

	    if (prev >= 0 && y >= prev * entryHeight - 3 &&
		y <= (prev+1) * entryHeight - 3) selecting = prev;

	    if (x < 0 || x > maxWidth || y < -3) selecting = -1;
	    else if (selecting < 0 || selecting > n) selecting = -1;

	    if (selecting == prev) break;

	    if (prev >= 0 && prev < n) {
		XFillRectangle(display(), m_menuWindow, m_menuGC,
			       4, prev * entryHeight + 9,
			       maxWidth - 8, entryHeight);
	    }

	    if (selecting >= 0 && selecting < n) {
		XFillRectangle(display(), m_menuWindow, m_menuGC,
			       4, selecting * entryHeight + 9,
			       maxWidth - 8, entryHeight);
	    }

	    break;

	case Expose:
	    XClearWindow(display(), m_menuWindow);

	    XDrawRectangle(display(), m_menuWindow, m_menuGC, 2, 7,
			   maxWidth - 5, totalHeight - 10);

	    for (i = 0; i < n; ++i) {

		int dx = XTextWidth(m_menuFont, MENU_LABEL(i),
				    strlen(MENU_LABEL(i)));
		int dy = i * entryHeight + m_menuFont->ascent + 10;
		
		if (i >= nh) {		    
		    XDrawString(display(), m_menuWindow, m_menuGC,
				maxWidth - 8 - dx, dy,
				MENU_LABEL(i), strlen(MENU_LABEL(i)));
		} else {
		    XDrawString(display(), m_menuWindow, m_menuGC, 8,
				dy, MENU_LABEL(i), strlen(MENU_LABEL(i)));
		}
	    }

	    if (selecting >= 0 && selecting < n) {
		XFillRectangle(display(), m_menuWindow, m_menuGC,
			       4, selecting * entryHeight + 9,
			       maxWidth - 8, entryHeight);
	    }

	    drawn = True;
	}
    }

    if (selecting == n-1 && allowExit) {
	m_signalled = True;
	return;
    }

    if (selecting >= 0) {
	if (selecting == 0) {
	    spawn();
	} else if (selecting < nh) {
	    clients.item(selecting - 1)->unhide(True);
	} else if (selecting < n) {
	    if (CONFIG_CLICK_TO_FOCUS) {
		clients.item(selecting - 1)->activate();
	    } else {
		clients.item(selecting - 1)->mapRaised();
	    }
	    clients.item(selecting - 1)->ensureVisible();
	}
    }

    clients.remove_all();
    return;
}


void WindowManager::showGeometry(int x, int y)
{
    char string[20];
    snprintf(string, sizeof(string), "%d %d\n", x, y);
    int width = XTextWidth(m_menuFont, string, strlen(string)) + 8;
    int height = m_menuFont->ascent + m_menuFont->descent + 8;
    int mx = DisplayWidth (display(), m_screenNumber) - 1;
    int my = DisplayHeight(display(), m_screenNumber) - 1;
  
    XMoveResizeWindow(display(), m_menuWindow,
		      (mx - width) / 2, (my - height) / 2, width, height);
    XClearWindow(display(), m_menuWindow);
    XMapRaised(display(), m_menuWindow);

    XDrawString(display(), m_menuWindow, m_menuGC, 4, 4 + m_menuFont->ascent,
		string, strlen(string));
}


void WindowManager::removeGeometry()
{
    XUnmapWindow(display(), m_menuWindow);
}


void Client::move(XButtonEvent *e)
{
    int x = -1, y = -1, xoff, yoff;
    Boolean done = False;

    if (m_windowManager->attemptGrab
	(root(), None, DragMask, e->time) != GrabSuccess) {
	return;
    }

    xoff = m_border->xIndent() - e->x;
    yoff = m_border->yIndent() - e->y;

    XEvent event;
    Boolean found;
    Boolean doSomething = False;
    struct timeval sleepval;

    while (!done) {

	found = False;

	while (XCheckMaskEvent(display(), DragMask | ExposureMask, &event)) {
	    found = True;
	    if (event.type != MotionNotify) break;
	}

	if (!found) {
	    sleepval.tv_sec = 0;
	    sleepval.tv_usec = 50000;
	    select(0, 0, 0, 0, &sleepval);
	    continue;
	}
	
	switch (event.type) {

	default:
	    fprintf(stderr, "wm2: unknown event type %d\n", event.type);
	    break;

	case Expose:
	    m_windowManager->eventExposure(&event.xexpose);
	    break;

	case ButtonPress:
	    // don't like this
	    XUngrabPointer(display(), event.xbutton.time);
	    doSomething = False;
	    done = True;
	    break;

	case ButtonRelease:

	    x = event.xbutton.x; y = event.xbutton.y;
	    if (!nobuttons(&event.xbutton)) doSomething = False;

//	    XUngrabPointer(display(), event.xbutton.time);
	    m_windowManager->releaseGrab(&event.xbutton);
	    done = True;
	    break;

	case MotionNotify:
	    x = event.xbutton.x; y = event.xbutton.y;
	    if (x + xoff != m_x || y + yoff != m_y) {
		windowManager()->showGeometry(x + xoff, y + yoff);
		m_border->moveTo(x + xoff, y + yoff);
		doSomething = True;
	    }
	    break;
	}
    }

    windowManager()->removeGeometry();

    if (x >= 0 && doSomething) {
	m_x = x + xoff;
	m_y = y + yoff;
    }

    if (CONFIG_CLICK_TO_FOCUS) activate();
    m_border->moveTo(m_x, m_y);
    sendConfigureNotify();
}


void Client::fixResizeDimensions(int &w, int &h, int &dw, int &dh)
{
    if (w < 50) w = 50;
    if (h < 50) h = 50;
    
    if (m_sizeHints.flags & PResizeInc) {
	w = m_minWidth  + (((w - m_minWidth) / m_sizeHints.width_inc) *
			   m_sizeHints.width_inc);
	h = m_minHeight + (((h - m_minHeight) / m_sizeHints.height_inc) *
			   m_sizeHints.height_inc);

	dw = (w - m_minWidth)  / m_sizeHints.width_inc;
	dh = (h - m_minHeight) / m_sizeHints.height_inc;
    } else {
	dw = w; dh = h;
    }

    if (m_sizeHints.flags & PMaxSize) {
	if (w > m_sizeHints.max_width)  w = m_sizeHints.max_width;
	if (h > m_sizeHints.max_height) h = m_sizeHints.max_height;
    }

    if (w < m_minWidth)  w = m_minWidth;
    if (h < m_minHeight) h = m_minHeight;
}


void Client::resize(XButtonEvent *e, Boolean horizontal, Boolean vertical)
{
    if (isFixedSize()) return;

    if (m_windowManager->attemptGrab
	(root(), None, DragMask, e->time) != GrabSuccess) {
	return;
    }

    if (vertical && horizontal)
	m_windowManager->installCursor(WindowManager::DownrightCursor);
    else if (vertical)
	m_windowManager->installCursor(WindowManager::DownCursor);
    else
	m_windowManager->installCursor(WindowManager::RightCursor);

    Window dummy;
    XTranslateCoordinates(display(), e->window, parent(),
			  e->x, e->y, &e->x, &e->y, &dummy);

    int xorig = e->x;
    int yorig = e->y;
    int x = xorig;
    int y = yorig;
    int w = m_w, h = m_h;
    int prevW, prevH;
    int dw, dh;

    XEvent event;
    Boolean found;
    Boolean doSomething = False;
    Boolean done = False;
    struct timeval sleepval;

    while (!done) {

	found = False;

	while (XCheckMaskEvent(display(), DragMask | ExposureMask, &event)) {
	    found = True;
	    if (event.type != MotionNotify) break;
	}

	if (!found) {
	    sleepval.tv_sec = 0;
	    sleepval.tv_usec = 50000;
	    select(0, 0, 0, 0, &sleepval);
	    continue;
	}
	
	switch (event.type) {

	default:
	    fprintf(stderr, "wm2: unknown event type %d\n", event.type);
	    break;

	case Expose:
	    m_windowManager->eventExposure(&event.xexpose);
	    break;

	case ButtonPress:
	    // don't like this
	    XUngrabPointer(display(), event.xbutton.time);
	    done = True;
	    break;

	case ButtonRelease:

	    x = event.xbutton.x; y = event.xbutton.y;

	    if (!nobuttons(&event.xbutton)) x = -1;
	    m_windowManager->releaseGrab(&event.xbutton);
	    
	    done = True;
	    break;

	case MotionNotify:
	    x = event.xbutton.x; y = event.xbutton.y;

	    if (vertical && horizontal) {
		prevH = h; h = y - m_y;
		prevW = w; w = x - m_x;
		fixResizeDimensions(w, h, dw, dh);
		if (h == prevH && w == prevW) break;
		m_border->configure(m_x, m_y, w, h, CWWidth | CWHeight, 0);
		windowManager()->showGeometry(dw, dh);
		doSomething = True;

	    } else if (vertical) {
		prevH = h; h = y - m_y;
		fixResizeDimensions(w, h, dw, dh);
		if (h == prevH) break;
		m_border->configure(m_x, m_y, w, h, CWHeight, 0);
		windowManager()->showGeometry(dw, dh);
		doSomething = True;

	    } else {
		prevW = w; w = x - m_x;
		fixResizeDimensions(w, h, dw, dh);
		if (w == prevW) break;
		m_border->configure(m_x, m_y, w, h, CWWidth, 0);
		windowManager()->showGeometry(dw, dh);
		doSomething = True;
	    }

	    break;
	}
    }

    if (doSomething) {

	windowManager()->removeGeometry();

	if (vertical && horizontal) {
	    m_w = x - m_x;
	    m_h = y - m_y;
	    fixResizeDimensions(m_w, m_h, dw, dh);
	    m_border->configure(m_x, m_y, m_w, m_h, CWWidth|CWHeight, 0, True);
	} else if (vertical) {
	    m_h = y - m_y;
	    fixResizeDimensions(m_w, m_h, dw, dh);
	    m_border->configure(m_x, m_y, m_w, m_h, CWHeight, 0, True);
	} else {
	    m_w = x - m_x;
	    fixResizeDimensions(m_w, m_h, dw, dh);
	    m_border->configure(m_x, m_y, m_w, m_h, CWWidth, 0, True);
	}

	XMoveResizeWindow(display(), m_window,
			  m_border->xIndent(), m_border->yIndent(), m_w, m_h);

	sendConfigureNotify();
    }

    m_windowManager->installCursor(WindowManager::NormalCursor);
}


void Client::moveOrResize(XButtonEvent *e)
{
    if (e->x < m_border->xIndent() && e->y > m_h) {
	resize(e, False, True);
    } else if (e->y < m_border->yIndent() &&
	       e->x > m_w + m_border->xIndent() - m_border->yIndent()) { //hack
	resize(e, True, False);
    } else {
	move(e);
    }
}


void Border::eventButton(XButtonEvent *e)
{
    if (e->window == m_parent) {

	if (!m_client->isActive()) return;
	if (isTransient()) {
	    if (e->x >= xIndent() && e->y >= yIndent()) {
		return;
	    } else {
		m_client->move(e);
		return;
	    }
	}

	m_client->moveOrResize(e);
	return;
	
    } else if (e->window == m_tab) {
	m_client->move(e);
	return;
    }

    if (e->window == m_resize) {
	m_client->resize(e, True, True);
	return;
    }

    if (e->window != m_button || e->type == ButtonRelease) return;

    if (windowManager()->attemptGrab(m_button, None, MenuGrabMask, e->time)
	!= GrabSuccess) {
	return;
    }

    XEvent event;
    Boolean found;
    Boolean done = False;
    struct timeval sleepval;
    unsigned long tdiff = 0L;
    int x = e->x;
    int y = e->y;
    int action = 1;
    int buttonSize = m_tabWidth - TAB_TOP_HEIGHT*2 - 4;

    XFillRectangle(display(), m_button, m_drawGC, 0, 0, buttonSize, buttonSize);

    while (!done) {

	found = False;

	if (tdiff > CONFIG_DESTROY_WINDOW_DELAY && action == 1) {
	    windowManager()->installCursor(WindowManager::DeleteCursor);
	    action = 2;
	}

	while (XCheckMaskEvent(display(), MenuMask, &event)) {
	    found = True;
	    if (event.type != MotionNotify) break;
	}

	if (!found) {
	    sleepval.tv_sec = 0;
	    sleepval.tv_usec = 50000;
	    select(0, 0, 0, 0, &sleepval);
	    tdiff += 50;
	    continue;
	}

	switch (event.type) {

	default:
	    fprintf(stderr, "wm2: unknown event type %d\n", event.type);
	    break;

	case Expose:
	    windowManager()->eventExposure(&event.xexpose);
	    break;

	case ButtonPress:
	    break;

	case ButtonRelease:

	    if (!nobuttons(&event.xbutton)) {
		action = 0;
	    }

	    if (x < 0 || y < 0 || x >= buttonSize || y >= buttonSize) {
		action = 0;
	    }

	    windowManager()->releaseGrab(&event.xbutton);
	    done = True;
	    break;

	case MotionNotify:
	    tdiff = event.xmotion.time - e->time;
	    if (tdiff > 5000L) tdiff = 5001L; // in case of overflow!

	    x = event.xmotion.x;
	    y = event.xmotion.y;

	    if (action == 0 || action == 2) {
		if (x < 0 || y < 0 || x >= buttonSize || y >= buttonSize) {
		    windowManager()->installCursor(WindowManager::NormalCursor);
		    action = 0;
		} else {
		    windowManager()->installCursor(WindowManager::DeleteCursor);
		    action = 2;
		}
	    }

	    break;
	}
    }

    XClearWindow(display(), m_button);
    windowManager()->installCursor(WindowManager::NormalCursor);

    if (tdiff > 5000L) {	// do nothing, they dithered too long
	return;
    }

    if (action == 1) m_client->hide();
    else if (action == 2) m_client->kill();
}

