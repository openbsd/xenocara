
#include "Manager.h"
#include "Client.h"
#include <string.h>
#include <X11/Xproto.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "Cursors.h"

Atom    Atoms::wm_state;
Atom    Atoms::wm_changeState;
Atom    Atoms::wm_protocols;
Atom    Atoms::wm_delete;
Atom    Atoms::wm_takeFocus;
Atom    Atoms::wm_colormaps;
Atom    Atoms::wm2_running;

int     WindowManager::m_signalled = False;
Boolean WindowManager::m_initialising = False;
Boolean ignoreBadWindowErrors;

const char *const WindowManager::m_menuCreateLabel = "New";

implementPList(ClientList, Client);


WindowManager::WindowManager() :
    m_menuGC(0), m_menuWindow(0), m_menuFont(0), m_focusChanging(False)
{
    fprintf(stderr, "\nwm2: Copyright (c) 1996-7 Chris Cannam."
	    "  Fourth release, March 1997\n"
	    "     Parts derived from 9wm Copyright (c) 1994-96 David Hogan\n"
	    "     %s\n     Copying and redistribution encouraged.  "
	    "No warranty.\n\n", XV_COPYRIGHT);

    if (CONFIG_AUTO_RAISE) {
	if (CONFIG_CLICK_TO_FOCUS) {
	    fatal("can't have auto-raise-with-delay with click-to-focus");
	} else if (CONFIG_RAISE_ON_FOCUS) {
	    fatal("can't have raise-on-focus AND auto-raise-with-delay");
	} else {
	    fprintf(stderr, "     Focus follows, auto-raise with delay.  ");
	}

    } else {
	if (CONFIG_CLICK_TO_FOCUS) {
	    if (CONFIG_RAISE_ON_FOCUS) {
		fprintf(stderr, "     Click to focus.  ");
	    } else {
		fatal("can't have click-to-focus without raise-on-focus");
	    }
	} else {
	    if (CONFIG_RAISE_ON_FOCUS) {
		fprintf(stderr, "     Focus follows, auto-raise.  ");
	    } else {
		fprintf(stderr, "     Focus follows pointer.  ");
	    }
	}
    }

    if (CONFIG_EVERYTHING_ON_ROOT_MENU) {
	fprintf(stderr, "All clients on menu.\n");
    } else {
	fprintf(stderr, "Hidden clients only on menu.\n");
    }

    if (CONFIG_PROD_SHAPE) {
	fprintf(stderr, "     Shape prodding on.  ");
    } else {
	fprintf(stderr, "     Shape prodding off.  ");
    }

    fprintf(stderr, "\n     (To reconfigure, simply edit and recompile.)\n\n");

    m_display = XOpenDisplay(NULL);
    if (!m_display) fatal("can't open display");

    m_shell = (char *)getenv("SHELL");
    if (!m_shell) m_shell = NewString("/bin/sh");

    m_initialising = True;
    XSetErrorHandler(errorHandler);
    ignoreBadWindowErrors = False;

    // 9wm does more, I think for nohup
    signal(SIGTERM, sigHandler);
    signal(SIGINT,  sigHandler);
    signal(SIGHUP,  sigHandler);

    m_currentTime = -1;
    m_activeClient = 0;

    Atoms::wm_state      = XInternAtom(m_display, "WM_STATE",            False);
    Atoms::wm_changeState= XInternAtom(m_display, "WM_CHANGE_STATE",     False);
    Atoms::wm_protocols  = XInternAtom(m_display, "WM_PROTOCOLS",        False);
    Atoms::wm_delete     = XInternAtom(m_display, "WM_DELETE_WINDOW",    False);
    Atoms::wm_takeFocus  = XInternAtom(m_display, "WM_TAKE_FOCUS",       False);
    Atoms::wm_colormaps  = XInternAtom(m_display, "WM_COLORMAP_WINDOWS", False);
    Atoms::wm2_running   = XInternAtom(m_display, "_WM2_RUNNING",        False);

    int dummy;
    if (!XShapeQueryExtension(m_display, &m_shapeEvent, &dummy))
	fatal("no shape extension, can't run without it");

    // we only cope with one screen!
    initialiseScreen();

    XSetSelectionOwner(m_display, Atoms::wm2_running,
		       m_menuWindow, timestamp(True));
    XSync(m_display, False);
    m_initialising = False;
    m_returnCode = 0;
    
    clearFocus();
    scanInitialWindows();
    loop();
}
    

WindowManager::~WindowManager()
{
    // empty
}


void WindowManager::release()
{
    if (m_returnCode != 0) return; // hasty exit

    ClientList normalList, unparentList;
    Client *c;
    int i;

    for (i = 0; i < m_clients.count(); ++i) {
	c = m_clients.item(i);
//    fprintf(stderr, "client %d is %p\n", i, c);

	if (c->isNormal()) normalList.append(c);
	else unparentList.append(c);
    }

    for (i = normalList.count()-1; i >= 0; --i) {
	unparentList.append(normalList.item(i));
    }

    m_clients.remove_all();
    
    for (i = 0; i < unparentList.count(); ++i) {
//	fprintf(stderr, "unparenting client %p\n",unparentList.item(i));
	unparentList.item(i)->unreparent();
	unparentList.item(i)->release();
	unparentList.item(i) = 0;
    }

    XSetInputFocus(m_display, PointerRoot, RevertToPointerRoot,
		   timestamp(False));
    installColormap(None);

    XFreeCursor(m_display, m_cursor);
    XFreeCursor(m_display, m_xCursor);
    XFreeCursor(m_display, m_vCursor);
    XFreeCursor(m_display, m_hCursor);
    XFreeCursor(m_display, m_vhCursor);

    XFreeFont(m_display, m_menuFont);
    XFreeGC(m_display, m_menuGC);

    XCloseDisplay(m_display);
}


void WindowManager::fatal(const char *message)
{
    fprintf(stderr, "wm2: ");
    perror(message);
    fprintf(stderr, "\n");
    exit(1);
}


int WindowManager::errorHandler(Display *d, XErrorEvent *e)
{
    if (m_initialising && (e->request_code == X_ChangeWindowAttributes) &&
	e->error_code == BadAccess) {
	fprintf(stderr, "wm2: another window manager running?\n");
	exit(1);
    }

    // ugh
    if (ignoreBadWindowErrors == True && e->error_code == BadWindow) return 0;

    char msg[100], number[30], request[100];
    XGetErrorText(d, e->error_code, msg, 100);
    snprintf(number, sizeof(number), "%d", e->request_code);
    XGetErrorDatabaseText(d, "XRequest", number, "", request, 100);

    if (request[0] == '\0') snprintf(request, sizeof(request), 
	"<request-code-%d>", e->request_code);

    fprintf(stderr, "wm2: %s (0x%lx): %s\n", request, e->resourceid, msg);

    if (m_initialising) {
	fprintf(stderr, "wm2: failure during initialisation, abandoning\n");
	exit(1);
    }

    return 0;
}    


static Cursor makeCursor(Display *d, Window w,
			 unsigned char *bits, unsigned char *mask_bits,
			 int width, int height, int xhot, int yhot,
			 XColor *fg, XColor *bg)
{
    Pixmap pixmap =
 	XCreateBitmapFromData(d, w, (const char *)bits, width, height);

    Pixmap mask =
 	XCreateBitmapFromData(d, w, (const char *)mask_bits, width, height);

    Cursor cursor = XCreatePixmapCursor(d, pixmap, mask, fg, bg, xhot, yhot);
    XFreePixmap(d, pixmap);
    XFreePixmap(d, mask);

    return cursor;
}


void WindowManager::initialiseScreen()
{
    int i = 0;
    m_screenNumber = i;

    m_root = RootWindow(m_display, i);
    m_defaultColormap = DefaultColormap(m_display, i);
    m_minimumColormaps = MinCmapsOfScreen(ScreenOfDisplay(m_display, i));

    XColor black, white, temp;

    if (!XAllocNamedColor(m_display, m_defaultColormap, "black", &black, &temp))
	fatal("couldn't load colour \"black\"!");
    if (!XAllocNamedColor(m_display, m_defaultColormap, "white", &white, &temp))
	fatal("couldn't load colour \"white\"!");

    m_cursor = makeCursor
	(m_display, m_root, cursor_bits, cursor_mask_bits,
	 cursor_width, cursor_height, cursor_x_hot,
	 cursor_y_hot, &black, &white);

    m_xCursor = makeCursor
	(m_display, m_root, ninja_cross_bits, ninja_cross_mask_bits,
	 ninja_cross_width, ninja_cross_height, ninja_cross_x_hot,
	 ninja_cross_y_hot, &black, &white);

    m_hCursor = makeCursor
	(m_display, m_root, cursor_right_bits, cursor_right_mask_bits,
	 cursor_right_width, cursor_right_height, cursor_right_x_hot,
	 cursor_right_y_hot, &black, &white);

    m_vCursor = makeCursor
	(m_display, m_root, cursor_down_bits, cursor_down_mask_bits,
	 cursor_down_width, cursor_down_height, cursor_down_x_hot,
	 cursor_down_y_hot, &black, &white);

    m_vhCursor = makeCursor
	(m_display, m_root, cursor_down_right_bits, cursor_down_right_mask_bits,
	 cursor_down_right_width, cursor_down_right_height,
	 cursor_down_right_x_hot, cursor_down_right_y_hot, &black, &white);

    XSetWindowAttributes attr;
    attr.cursor = m_cursor;
    attr.event_mask = SubstructureRedirectMask | SubstructureNotifyMask |
	ColormapChangeMask | ButtonPressMask | ButtonReleaseMask | 
	PropertyChangeMask;
    XChangeWindowAttributes(m_display, m_root, CWCursor | CWEventMask, &attr);
    XSync(m_display, False);

    m_menuForegroundPixel =
	allocateColour(CONFIG_MENU_FOREGROUND, "menu foreground");
    m_menuBackgroundPixel =
	allocateColour(CONFIG_MENU_BACKGROUND, "menu background");
    m_menuBorderPixel =
	allocateColour(CONFIG_MENU_BORDERS, "menu border");

    m_menuWindow = XCreateSimpleWindow
	(m_display, m_root, 0, 0, 1, 1, 1,
	 m_menuBorderPixel, m_menuBackgroundPixel);

    if (DoesSaveUnders(ScreenOfDisplay(m_display, m_screenNumber))) {
	XSetWindowAttributes attr;
	attr.save_under = True;
	XChangeWindowAttributes(m_display, m_menuWindow, CWSaveUnder, &attr);
    }

    XGCValues values;
    values.background = m_menuBackgroundPixel;
    values.foreground = m_menuForegroundPixel ^ m_menuBackgroundPixel;
    values.function = GXxor;
    values.line_width = 0;
    values.subwindow_mode = IncludeInferiors;

    m_menuFont = XLoadQueryFont(display(), CONFIG_NICE_MENU_FONT);
    if (!m_menuFont) m_menuFont = XLoadQueryFont(display(),
						 CONFIG_NASTY_FONT);
    if (!m_menuFont) fatal("couldn't load default menu font\n");
    
    values.font = m_menuFont->fid;
    m_menuGC = XCreateGC
	(display(), root(), GCForeground | GCBackground |
	 GCFunction | GCLineWidth | GCSubwindowMode | GCFont, &values);
}


unsigned long WindowManager::allocateColour(char *name, char *desc)
{
    XColor nearest, ideal;

    if (!XAllocNamedColor
	(display(), DefaultColormap(display(), m_screenNumber), name,
	 &nearest, &ideal)) {

	char error[100];
	snprintf(error, sizeof(error), "couldn't load %s colour", desc);
	fatal(error);

    } else return nearest.pixel;
}


void WindowManager::installCursor(RootCursor c)
{
    installCursorOnWindow(c, m_root);
}


void WindowManager::installCursorOnWindow(RootCursor c, Window w)
{
    XSetWindowAttributes attr;

    switch (c) {
    case DeleteCursor:    attr.cursor = m_xCursor;  break;
    case DownCursor:      attr.cursor = m_vCursor;  break;
    case RightCursor:     attr.cursor = m_hCursor;  break;
    case DownrightCursor: attr.cursor = m_vhCursor; break;
    case NormalCursor:    attr.cursor = m_cursor;   break;
    }

    XChangeWindowAttributes(m_display, w, CWCursor, &attr);
}
	

Time WindowManager::timestamp(Boolean reset)
{
    if (reset) m_currentTime = CurrentTime;

    if (m_currentTime == CurrentTime) {

	XEvent event;
	XChangeProperty(m_display, m_root, Atoms::wm2_running,
			Atoms::wm2_running, 8, PropModeAppend,
			(unsigned char *)"", 0);
	XMaskEvent(m_display, PropertyChangeMask, &event);

	m_currentTime = event.xproperty.time;
    }

    return m_currentTime;
}

void WindowManager::sigHandler(int)
{
    m_signalled = True;
}

void WindowManager::scanInitialWindows()
{
    unsigned int i, n;
    Window w1, w2, *wins;
    XWindowAttributes attr;

    XQueryTree(m_display, m_root, &w1, &w2, &wins, &n);

    for (i = 0; i < n; ++i) {

	XGetWindowAttributes(m_display, wins[i], &attr);
	if (attr.override_redirect || wins[i] == m_menuWindow) continue;

	(void)windowToClient(wins[i], True);
    }

    XFree((void *)wins);
}

Client *WindowManager::windowToClient(Window w, Boolean create)
{
    if (w == 0) return 0;

    for (int i = m_clients.count()-1; i >= 0; --i) {

	if (m_clients.item(i)->hasWindow(w)) {
	    return m_clients.item(i);
	}
    }

    if (!create) return 0;
    else {
	Client *newC = new Client(this, w);
	m_clients.append(newC);
	return newC;
    }
}

void WindowManager::installColormap(Colormap cmap)
{
    if (cmap == None) {
	XInstallColormap(m_display, m_defaultColormap);
    } else {
	XInstallColormap(m_display, cmap);
    }
}

void WindowManager::clearFocus()
{
    static Window w = 0;
    Client *active = activeClient();

    if (CONFIG_AUTO_RAISE || !CONFIG_CLICK_TO_FOCUS) {
	setActiveClient(0);
	return;
    }

    if (active) {

	setActiveClient(0);
	active->deactivate();

	for (Client *c = active->revertTo(); c; c = c->revertTo()) {
	    if (c->isNormal()) {
		c->activate();
		return;
	    }
	}

	installColormap(None);
    }

    if (w == 0) {

	XSetWindowAttributes attr;
	int mask = CWOverrideRedirect;
	attr.override_redirect = 1;

	w = XCreateWindow(display(), root(), 0, 0, 1, 1, 0,
			  CopyFromParent, InputOnly, CopyFromParent,
			  mask, &attr);

	XMapWindow(display(), w);
    }

    XSetInputFocus(display(), w, RevertToPointerRoot, timestamp(False));
}


void WindowManager::skipInRevert(Client *c, Client *myRevert)
{
    for (int i = 0; i < m_clients.count(); ++i) {
	if (m_clients.item(i) != c &&
	    m_clients.item(i)->revertTo() == c) {
	    m_clients.item(i)->setRevertTo(myRevert);
	}
    }
}


void WindowManager::addToHiddenList(Client *c)
{
    for (int i = 0; i < m_hiddenClients.count(); ++i) {
	if (m_hiddenClients.item(i) == c) return;
    }

    m_hiddenClients.append(c);
}


void WindowManager::removeFromHiddenList(Client *c)
{
    for (int i = 0; i < m_hiddenClients.count(); ++i) {
	if (m_hiddenClients.item(i) == c) {
	    m_hiddenClients.remove(i);
	    return;
	}
    }
}


Boolean WindowManager::raiseTransients(Client *c)
{
    Client *first = 0;

    if (!c->isNormal()) return False;

    for (int i = 0; i < m_clients.count(); ++i) {

	if (m_clients.item(i)->isNormal() &&
	    m_clients.item(i)->isTransient()) {

	    if (c->hasWindow(m_clients.item(i)->transientFor())) {

		if (!first) first = m_clients.item(i);
		else m_clients.item(i)->mapRaised();
	    }
	}
    }

    if (first) {
	first->mapRaised();
	return True;
    } else {
	return False;
    }
}


void WindowManager::spawn()
{
    // strange code thieved from 9wm to avoid leaving zombies

    char *displayName = DisplayString(m_display);

    if (fork() == 0) {
	if (fork() == 0) {

	    close(ConnectionNumber(m_display));

	    // if you don't have putenv, miss out this next
	    // conditional and its contents

	    if (displayName && (displayName[0] != '\0')) {

		char *pstring;
		asprintf(&pstring, "DISPLAY=%s", displayName);
		putenv(pstring);
	    }

	    if (CONFIG_EXEC_USING_SHELL) {
		execl(m_shell, m_shell, "-c", CONFIG_NEW_WINDOW_COMMAND, 0);
		fprintf(stderr, "wm2: exec %s", m_shell);
		perror(" failed");
	    }

	    execlp(CONFIG_NEW_WINDOW_COMMAND, CONFIG_NEW_WINDOW_COMMAND, 0);
	    fprintf(stderr, "wm2: exec %s", CONFIG_NEW_WINDOW_COMMAND);
	    perror(" failed");

	    execlp("xterm", "xterm", "-ut", 0);
	    perror("wm2: exec xterm failed");
	    exit(1);
	}
	exit(0);
    }
    wait((int *) 0);
}


