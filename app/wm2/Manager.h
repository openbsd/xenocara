
#ifndef _MANAGER_H_
#define _MANAGER_H_

#include "General.h"
#include "listmacro2.h"

class Client;
declarePList(ClientList, Client);


class WindowManager {
public:
    WindowManager();
    ~WindowManager();

    void fatal(const char *);

    // for call from Client and within:

    Client *windowToClient(Window, Boolean create = False);
    Client *activeClient() { return m_activeClient; }
    Boolean raiseTransients(Client *); // true if raised any
    Time timestamp(Boolean reset);
    void clearFocus();

    void setActiveClient(Client *const c) { m_activeClient = c; }

    void addToHiddenList(Client *);
    void removeFromHiddenList(Client *);
    void skipInRevert(Client *, Client *);

    Display *display() { return m_display; }
    Window root() { return m_root; }

    enum RootCursor {
	NormalCursor, DeleteCursor, DownCursor, RightCursor, DownrightCursor
    };

    void installCursor(RootCursor);
    void installCursorOnWindow(RootCursor, Window);
    void installColormap(Colormap);
    unsigned long allocateColour(char *, char *);

    void considerFocusChange(Client *, Window, Time timestamp);
    void stopConsideringFocus();

    // shouldn't really be public
    int attemptGrab(Window, Window, int, int);
    void releaseGrab(XButtonEvent *);
    void eventExposure(XExposeEvent *);	// for exposures during client grab
    void showGeometry(int, int);
    void removeGeometry();

private:
    int loop();
    void release();

    Display *m_display;
    int m_screenNumber;

    Window m_root;

    Colormap m_defaultColormap;
    int m_minimumColormaps;

    Cursor m_cursor;
    Cursor m_xCursor;
    Cursor m_vCursor;
    Cursor m_hCursor;
    Cursor m_vhCursor;
    
    char *m_terminal;
    char *m_shell;

    ClientList m_clients;
    ClientList m_hiddenClients;
    Client *m_activeClient;

    int m_shapeEvent;
    int m_currentTime;

    Boolean m_looping;
    int m_returnCode;

    static Boolean m_initialising;
    static int errorHandler(Display *, XErrorEvent *);
    static void sigHandler(int);
    static int m_signalled;

    void initialiseScreen();
    void scanInitialWindows();

    GC m_menuGC;
    Window m_menuWindow;
    XFontStruct *m_menuFont;
    unsigned long m_menuForegroundPixel;
    unsigned long m_menuBackgroundPixel;
    unsigned long m_menuBorderPixel;
    static const char *const m_menuCreateLabel;
    const char *const menuLabel(int);
    void menu(XButtonEvent *);
    void spawn();
    void circulate(Boolean activeFirst);

    Boolean m_focusChanging;	// checking times for focus change
    Client *m_focusCandidate;
    Window  m_focusCandidateWindow;
    Time    m_focusTimestamp;	// time of last crossing event
    Boolean m_focusPointerMoved;
    Boolean m_focusPointerNowStill;
    void checkDelaysForFocus();

    void nextEvent(XEvent *);	// return

    void eventButton(XButtonEvent *);
    void eventMapRequest(XMapRequestEvent *);
    void eventConfigureRequest(XConfigureRequestEvent *);
    void eventUnmap(XUnmapEvent *);
    void eventCreate(XCreateWindowEvent *);
    void eventDestroy(XDestroyWindowEvent *);
    void eventClient(XClientMessageEvent *);
    void eventColormap(XColormapEvent *);
    void eventProperty(XPropertyEvent *);
    void eventEnter(XCrossingEvent *);
    void eventReparent(XReparentEvent *);
    void eventFocusIn(XFocusInEvent *);
};

#endif

