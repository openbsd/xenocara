
#include "Border.h"
#include "Client.h"
#include "Manager.h"
#include "Rotated.h"

// These are degenerate initialisations, don't change them
int Border::m_tabWidth = -1;
XRotFontStruct *Border::m_tabFont = 0;
GC Border::m_drawGC = 0;

unsigned long Border::m_foregroundPixel;
unsigned long Border::m_backgroundPixel;
unsigned long Border::m_frameBackgroundPixel;
unsigned long Border::m_buttonBackgroundPixel;
unsigned long Border::m_borderPixel;

static int borderCounter = 0;


declareList(RectangleList, XRectangle);
implementList(RectangleList, XRectangle);

class BorderRectangleList : public RectangleList
{
public:
    BorderRectangleList() { }
    ~BorderRectangleList() { }

    void appendRect(int x, int y, int w, int h);
};

void BorderRectangleList::appendRect(int x, int y, int w, int h)
{
    XRectangle r;
    r.x = x; r.y = y; r.width = w; r.height = h;
    append(r);
}


Border::Border(Client *const c, Window child) :
    m_client(c), m_parent(0), m_tab(0),
    m_child(child), m_button(0), m_resize(0), m_label(0),
    m_tabHeight(-1), m_prevW(-1), m_prevH(-1)
{
    m_parent = root();

    if (m_tabFont == 0) {

	if (!(m_tabFont = XRotLoadFont(display(), CONFIG_NICE_FONT, 90.0)) &&
	    !(m_tabFont = XRotLoadFont(display(), CONFIG_NASTY_FONT, 90.0))) {
	    windowManager()->fatal
		("couldn't load default rotated font, bailing out");
	}

	m_tabWidth = m_tabFont->height + 4;
	if (m_tabWidth < TAB_TOP_HEIGHT * 2 + 8) {
	    m_tabWidth = TAB_TOP_HEIGHT * 2 + 8;
	}

	m_foregroundPixel = windowManager()->allocateColour
	    (CONFIG_TAB_FOREGROUND, "tab foreground");
	m_backgroundPixel = windowManager()->allocateColour
	    (CONFIG_TAB_BACKGROUND, "tab background");
	m_frameBackgroundPixel = windowManager()->allocateColour
	    (CONFIG_FRAME_BACKGROUND, "frame background");
	m_buttonBackgroundPixel = windowManager()->allocateColour
	    (CONFIG_BUTTON_BACKGROUND, "button background");
	m_borderPixel = windowManager()->allocateColour
	    (CONFIG_BORDERS, "border");

	XGCValues values;
	values.foreground = m_foregroundPixel;
	values.background = m_backgroundPixel;
	values.function = GXcopy;
	values.line_width = 0;
	values.subwindow_mode = IncludeInferiors;

	m_drawGC = XCreateGC(display(), root(),
			     GCForeground | GCBackground | GCFunction |
			     GCLineWidth | GCSubwindowMode,
			     &values);

	if (!m_drawGC) {
	    windowManager()->fatal("couldn't allocate border GC");
	}
    }

    ++borderCounter;
}


Border::~Border()
{
    if (m_parent != root()) {
	if (!m_parent) fprintf(stderr,"wm2: zero parent in Border::~Border\n");
	else {
	    XDestroyWindow(display(), m_tab);
	    XDestroyWindow(display(), m_button);
	    XDestroyWindow(display(), m_parent);

	    // bad window if its parent has already gone:
	    XDestroyWindow(display(), m_resize);
	}
    }

    if (m_label) free(m_label);

    if (--borderCounter == 0) {
	XFreeGC(display(), m_drawGC);
    }
}


void Border::fatal(char *s)
{
    windowManager()->fatal(s);
}


Display *Border::display()
{
    return m_client->display();
}


WindowManager *Border::windowManager()
{
    return m_client->windowManager();
}


Window Border::root()
{
    return m_client->root();
}


void Border::expose(XExposeEvent *e)
{
    if (e->window != m_tab) return;
    drawLabel();
}


void Border::drawLabel()
{
    if (m_label) {
	XClearWindow(display(), m_tab);
	XRotDrawString(display(), m_tabFont, m_tab, m_drawGC,
		       2 + m_tabFont->max_ascent, m_tabHeight - 1,
		       m_label, strlen(m_label));
    }
}


Boolean Border::isTransient(void)
{
    return m_client->isTransient();
}


Boolean Border::isFixedSize(void)
{
    return m_client->isFixedSize();
}


void Border::fixTabHeight(int maxHeight)
{
    m_tabHeight = 0x7fff;
    maxHeight -= m_tabWidth;	// for diagonal

    if (m_label) free(m_label);
    m_label = NewString(m_client->label());
    
    if (m_label) {
	m_tabHeight =
	    XRotTextWidth(m_tabFont, m_label, strlen(m_label)) + 6 + m_tabWidth;
    }

    if (m_tabHeight <= maxHeight) return;

    if (m_label) free(m_label);
    m_label = m_client->iconName() ?
	NewString(m_client->iconName()) : NewString("incognito");

    int len = strlen(m_label);
    m_tabHeight = XRotTextWidth(m_tabFont, m_label, len) + 6 + m_tabWidth;
    if (m_tabHeight <= maxHeight) return;

    char *newLabel = (char *)malloc(len + 4);

    do {
	strlcpy(newLabel, m_label, len + 4);
	strlcat(newLabel, "...", len + 4);
	m_tabHeight = XRotTextWidth(m_tabFont, newLabel,
				    strlen(newLabel)) + 6 + m_tabWidth;
	--len;

    } while (m_tabHeight > maxHeight && len > 2);

    free(m_label);
    m_label = newLabel;

    if (m_tabHeight > maxHeight) m_tabHeight = maxHeight;
}


void Border::shapeTransientParent(int w, int h)
{
    XRectangle r;

    r.x = xIndent() - 1; r.y = yIndent() - 1;
    r.width = w + 2; r.height = h + 2;

    XShapeCombineRectangles
	(display(), m_parent, ShapeBounding, 0, 0, &r, 1, ShapeSet, YXBanded);

    r.x = xIndent(); r.y = yIndent();
    r.width = w; r.height = h;

    XShapeCombineRectangles
	(display(), m_parent, ShapeClip, 0, 0, &r, 1, ShapeSet, YXBanded);
}


void Border::setTransientFrameVisibility(Boolean visible, int w, int h)
{
    int i;
    BorderRectangleList rl;

    rl.appendRect(0, 0, w + 1, yIndent() - 1);
    for (i = 1; i < yIndent(); ++i) {
	rl.appendRect(w + 1, i - 1, i + 1, 1);
    }
    rl.appendRect(0, yIndent() - 1, xIndent() - 1, h - yIndent() + 2);
    for (i = 1; i < yIndent(); ++i) {
	rl.appendRect(i - 1, h, 1, i + 2);
    }

    XShapeCombineRectangles
	(display(), m_parent, ShapeBounding,
	 0, 0, rl.array(0, rl.count()), rl.count(),
	 visible ? ShapeUnion : ShapeSubtract, YXSorted);

    rl.remove_all();

    rl.appendRect(1, 1, w, yIndent() - 2);
    for (i = 2; i < yIndent(); ++i) {
	rl.appendRect(w + 1, i - 1, i, 1);
    }
    rl.appendRect(1, yIndent() - 1, xIndent() - 2, h - yIndent() + 1);
    for (i = 2; i < yIndent(); ++i) {
	rl.appendRect(i - 1, h, 1, i + 1);
    }

    XShapeCombineRectangles
	(display(), m_parent, ShapeClip,
	 0, 0, rl.array(0, rl.count()), rl.count(),
	 visible ? ShapeUnion : ShapeSubtract, YXSorted);
}   


void Border::shapeParent(int w, int h)
{
    int i;
    int mainRect;
    BorderRectangleList rl;

    if (isTransient()) {
	shapeTransientParent(w, h);
	return;
    }

    // Bounding rectangles -- clipping will be the same except for
    // child window border

    // top of tab
    rl.appendRect(0, 0, w + m_tabWidth + 1, TAB_TOP_HEIGHT + 2);

    // struts in tab, left...
    rl.appendRect(0, TAB_TOP_HEIGHT + 1,
		  TAB_TOP_HEIGHT + 2, m_tabWidth - TAB_TOP_HEIGHT*2 - 1);

    // ...and right
    rl.appendRect(m_tabWidth - TAB_TOP_HEIGHT, TAB_TOP_HEIGHT + 1,
		  TAB_TOP_HEIGHT + 2, m_tabWidth - TAB_TOP_HEIGHT*2 - 1);

    mainRect = rl.count();
    rl.appendRect(xIndent() - 1, yIndent() - 1, w + 2, h + 2);

    // main tab
    rl.appendRect(0, m_tabWidth - TAB_TOP_HEIGHT, m_tabWidth + 2,
		  m_tabHeight - m_tabWidth + TAB_TOP_HEIGHT);

    // diagonal
    for (i = 1; i < m_tabWidth - 1; ++i) {
	rl.appendRect(i, m_tabHeight + i - 1, m_tabWidth - i + 2, 1);
    }

    XShapeCombineRectangles
	(display(), m_parent, ShapeBounding,
	 0, 0, rl.array(0, rl.count()), rl.count(), ShapeSet, YXSorted);

    rl.item(mainRect).x ++;
    rl.item(mainRect).y ++;
    rl.item(mainRect).width -= 2;
    rl.item(mainRect).height -= 2;

    XShapeCombineRectangles
	(display(), m_parent, ShapeClip,
	 0, 0, rl.array(0, rl.count()), rl.count(), ShapeSet, YXSorted);
}


void Border::shapeTab(int w, int)
{
    int i;
    BorderRectangleList rl;

    if (isTransient()) {
	return;
    }

    // Bounding rectangles

    rl.appendRect(0, 0, w + m_tabWidth + 1, TAB_TOP_HEIGHT + 2);
    rl.appendRect(0, TAB_TOP_HEIGHT + 1, TAB_TOP_HEIGHT + 2,
		  m_tabWidth - TAB_TOP_HEIGHT*2 - 1);

    rl.appendRect(m_tabWidth - TAB_TOP_HEIGHT, TAB_TOP_HEIGHT + 1,
		  TAB_TOP_HEIGHT + 2, m_tabWidth - TAB_TOP_HEIGHT*2 - 1);

    rl.appendRect(0, m_tabWidth - TAB_TOP_HEIGHT, m_tabWidth + 2,
		  m_tabHeight - m_tabWidth + TAB_TOP_HEIGHT);

    for (i = 1; i < m_tabWidth - 1; ++i) {
	rl.appendRect(i, m_tabHeight + i - 1, m_tabWidth - i + 2, 1);
    }

    XShapeCombineRectangles
	(display(), m_tab, ShapeBounding,
	 0, 0, rl.array(0, rl.count()), rl.count(), ShapeSet, YXSorted);

    rl.remove_all();

    // Clipping rectangles

    rl.appendRect(1, 1, w + m_tabWidth - 1, TAB_TOP_HEIGHT);

    rl.appendRect(1, TAB_TOP_HEIGHT + 1, TAB_TOP_HEIGHT,
		  m_tabWidth + TAB_TOP_HEIGHT*2 - 1);

    rl.appendRect(m_tabWidth - TAB_TOP_HEIGHT + 1, TAB_TOP_HEIGHT + 1,
		  TAB_TOP_HEIGHT, m_tabWidth + TAB_TOP_HEIGHT*2 - 1);

    rl.appendRect(1, m_tabWidth - TAB_TOP_HEIGHT + 1, m_tabWidth,
		  m_tabHeight - m_tabWidth + TAB_TOP_HEIGHT - 1);

    for (i = 1; i < m_tabWidth - 2; ++i) {
	rl.appendRect(i + 1, m_tabHeight + i - 1, m_tabWidth - i, 1);
    }

    XShapeCombineRectangles
	(display(), m_tab, ShapeClip,
	 0, 0, rl.array(0, rl.count()), rl.count(), ShapeSet, YXSorted);
}


void Border::resizeTab(int h)
{
    int i;
    XRectangle r;
    BorderRectangleList rl;
    int shorter, longer, operation;

    if (isTransient()) {
	return;
    }

    int prevTabHeight = m_tabHeight;
    fixTabHeight(h);
    if (m_tabHeight == prevTabHeight) return;

    XWindowChanges wc;
    wc.height = m_tabHeight + 2 + m_tabWidth;
    XConfigureWindow(display(), m_tab, CWHeight, &wc);

    if (m_tabHeight > prevTabHeight) {

	shorter = prevTabHeight;
	longer = m_tabHeight;
	operation = ShapeUnion;

    } else {

	shorter = m_tabHeight;
	longer = prevTabHeight + m_tabWidth;
	operation = ShapeSubtract;
    }

    r.x = 0; r.y = shorter /*- 2*/;
    r.width = m_tabWidth + 2; r.height = longer - shorter;

    XShapeCombineRectangles(display(), m_parent, ShapeBounding,
			    0, 0, &r, 1, operation, YXBanded);

    XShapeCombineRectangles(display(), m_parent, ShapeClip,
			    0, 0, &r, 1, operation, YXBanded);

    XShapeCombineRectangles(display(), m_tab, ShapeBounding,
			    0, 0, &r, 1, operation, YXBanded);

    r.x ++; r.width -= 2;

    XShapeCombineRectangles(display(), m_tab, ShapeClip,
			    0, 0, &r, 1, operation, YXBanded);

    if (m_client->isActive()) {
	// restore a bit of the frame edge
	r.x = m_tabWidth + 1; r.y = shorter;
	r.width = FRAME_WIDTH - 1; r.height = longer - shorter;
	XShapeCombineRectangles(display(), m_parent, ShapeBounding,
				0, 0, &r, 1, ShapeUnion, YXBanded);
    }

    for (i = 1; i < m_tabWidth - 1; ++i) {
	rl.appendRect(i, m_tabHeight + i - 1, m_tabWidth - i + 2, 1);
    }
	
    XShapeCombineRectangles
	(display(), m_parent, ShapeBounding,
	 0, 0, rl.array(0, rl.count()), rl.count(), ShapeUnion, YXBanded);

    XShapeCombineRectangles
	(display(), m_parent, ShapeClip,
	 0, 0, rl.array(0, rl.count()), rl.count(), ShapeUnion, YXBanded);

    XShapeCombineRectangles
	(display(), m_tab, ShapeBounding,
	 0, 0, rl.array(0, rl.count()), rl.count(), ShapeUnion, YXBanded);

    if (rl.count() < 2) return;

    for (i = 0; i < rl.count() - 1; ++i) {
	rl.item(i).x ++; rl.item(i).width -= 2;
    }

    XShapeCombineRectangles
	(display(), m_tab, ShapeClip,
	 0, 0, rl.array(0, rl.count() - 1), rl.count() - 1,
	 ShapeUnion, YXBanded);
}


void Border::shapeResize()
{
    int i;
    BorderRectangleList rl;

    for (i = 0; i < FRAME_WIDTH*2; ++i) {
	rl.appendRect(FRAME_WIDTH*2 - i - 1, i, i + 1, 1);
    }

    XShapeCombineRectangles
	(display(), m_resize, ShapeBounding, 0, 0,
	 rl.array(0, rl.count()), rl.count(), ShapeSet, YXBanded);

    rl.remove_all();

    for (i = 1; i < FRAME_WIDTH*2; ++i) {
	rl.appendRect(FRAME_WIDTH*2 - i, i, i, 1);
    }

    XShapeCombineRectangles
	(display(), m_resize, ShapeClip, 0, 0,
	 rl.array(0, rl.count()), rl.count(), ShapeSet, YXBanded);

    rl.remove_all();

    for (i = 0; i < FRAME_WIDTH*2 - 3; ++i) {
	rl.appendRect(FRAME_WIDTH*2 - i - 1, i + 3, 1, 1);
    }

    XShapeCombineRectangles
	(display(), m_resize, ShapeClip, 0, 0,
	 rl.array(0, rl.count()), rl.count(), ShapeSubtract, YXBanded);

    windowManager()->installCursorOnWindow
	(WindowManager::DownrightCursor, m_resize);
}


void Border::setFrameVisibility(Boolean visible, int w, int h)
{
    BorderRectangleList rl;

    if (CONFIG_PROD_SHAPE) {
	shapeParent(w, h);
	shapeTab(w, h);
    }

    if (isTransient()) {
	setTransientFrameVisibility(visible, w, h);
	return;
    }

    // Bounding rectangles

    rl.appendRect(m_tabWidth + w + 1, 0, FRAME_WIDTH + 1, FRAME_WIDTH);

    rl.appendRect(m_tabWidth + 2, TAB_TOP_HEIGHT + 2, w,
		  FRAME_WIDTH - TAB_TOP_HEIGHT - 2);

    // for button
    int ww = m_tabWidth - TAB_TOP_HEIGHT*2 - 4;
    rl.appendRect((m_tabWidth + 2 - ww) / 2, (m_tabWidth+2 - ww) / 2, ww, ww);

    rl.appendRect(m_tabWidth + 2, FRAME_WIDTH,
		  FRAME_WIDTH - 2, m_tabHeight + m_tabWidth - FRAME_WIDTH - 2);

    // swap last two if sorted wrong
    if (rl.item(rl.count()-2).y > rl.item(rl.count()-1).y) {
	rl.append(rl.item(rl.count()-2));
	rl.remove(rl.count()-3);
    }

    int final = rl.count();
    rl.append(rl.item(final-1));
    rl.item(final).x -= 1;
    rl.item(final).y += rl.item(final).height;
    rl.item(final).width += 1;
    rl.item(final).height = h - rl.item(final).height + 2;

    XShapeCombineRectangles(display(), m_parent, ShapeBounding,
			    0, 0, rl.array(0, rl.count()), rl.count(),
			    visible ? ShapeUnion : ShapeSubtract, YXSorted);
    rl.remove_all();

    // Clip rectangles

    rl.appendRect(m_tabWidth + w + 1, 1, FRAME_WIDTH, FRAME_WIDTH - 1);

    rl.appendRect(m_tabWidth + 2, TAB_TOP_HEIGHT + 2, w,
		  FRAME_WIDTH - TAB_TOP_HEIGHT - 2);

    // for button
    ww = m_tabWidth - TAB_TOP_HEIGHT*2 - 6;
    rl.appendRect((m_tabWidth + 2 - ww) / 2, (m_tabWidth+2 - ww) / 2, ww, ww);

    rl.appendRect(m_tabWidth + 2, FRAME_WIDTH,
		  FRAME_WIDTH - 2, h - FRAME_WIDTH);

    // swap last two if sorted wrong
    if (rl.item(rl.count()-2).y > rl.item(rl.count()-1).y) {
	rl.append(rl.item(rl.count()-2));
	rl.remove(rl.count()-3);
    }

    rl.appendRect(m_tabWidth + 2, h, FRAME_WIDTH - 2, FRAME_WIDTH + 1);

    XShapeCombineRectangles(display(), m_parent, ShapeClip,
			    0, 0, rl.array(0, rl.count()), rl.count(),
			    visible ? ShapeUnion : ShapeSubtract, YXSorted);
    rl.remove_all();

    if (visible && !isFixedSize()) {
	XMapRaised(display(), m_resize);
    } else {
	XUnmapWindow(display(), m_resize);
    }
}


void Border::configure(int x, int y, int w, int h,
		       unsigned long mask, int detail,
		       Boolean force) // must reshape everything
{
    if (!m_parent || m_parent == root()) {

	// create windows, then shape them afterwards

	m_parent = XCreateSimpleWindow
	    (display(), root(), 1, 1, 1, 1, 0,
	     m_borderPixel, m_frameBackgroundPixel);

	m_tab = XCreateSimpleWindow
	    (display(), m_parent, 1, 1, 1, 1, 0,
	     m_borderPixel, m_backgroundPixel);

	m_button = XCreateSimpleWindow
	    (display(), m_parent, 1, 1, 1, 1, 0,
	     m_borderPixel, m_buttonBackgroundPixel);

	m_resize = XCreateWindow
	    (display(), m_child, 1, 1, FRAME_WIDTH*2, FRAME_WIDTH*2, 0,
	     CopyFromParent, InputOutput, CopyFromParent, 0L, 0);

	shapeResize();

	XSelectInput(display(), m_parent,
		     SubstructureRedirectMask | SubstructureNotifyMask |
		     ButtonPressMask | ButtonReleaseMask);

	if (!isTransient()) {
	    XSelectInput(display(), m_tab,
			 ExposureMask | ButtonPressMask | ButtonReleaseMask |
			 EnterWindowMask/* | LeaveWindowMask*/);
	}

	XSelectInput(display(), m_button,
		     ButtonPressMask | ButtonReleaseMask/* | LeaveWindowMask*/);
	XSelectInput(display(), m_resize, ButtonPressMask | ButtonReleaseMask);
	mask |= CWX | CWY | CWWidth | CWHeight | CWBorderWidth;
    }

    XWindowChanges wc;
    wc.x = x - xIndent();
    wc.y = y - yIndent();
    wc.width  = w + xIndent() + 1;
    wc.height = h + yIndent() + 1;
    wc.border_width = 0;
    wc.sibling = None;
    wc.stack_mode = detail;
    XConfigureWindow(display(), m_parent, mask, &wc);

    unsigned long rmask = 0L;
    if (mask & CWWidth)  rmask |= CWX;
    if (mask & CWHeight) rmask |= CWY;
    wc.x = w - FRAME_WIDTH*2;
    wc.y = h - FRAME_WIDTH*2;
    XConfigureWindow(display(), m_resize, rmask, &wc);
    
    if (force ||
	(m_prevW < 0 || m_prevH < 0) ||
	((mask & (CWWidth | CWHeight)) && (w != m_prevW || h != m_prevH))) {

	int prevTabHeight = m_tabHeight;
	if (isTransient()) m_tabHeight = 10; // arbitrary
	else fixTabHeight(h);

	shapeParent(w, h);
	setFrameVisibility(m_client->isActive(), w, h);

	if (force ||
	    prevTabHeight != m_tabHeight || m_prevW < 0 || m_prevH < 0) {

	    wc.x = 0;
	    wc.y = 0;
	    wc.width = w + xIndent();
	    wc.height = m_tabHeight + 2 + m_tabWidth;
	    XConfigureWindow(display(), m_tab, mask, &wc);
	    shapeTab(w, h);
	}

	m_prevW = w;
	m_prevH = h;

    } else {

	resizeTab(h);
    }

    wc.x = TAB_TOP_HEIGHT + 2;
    wc.y = wc.x;
    wc.width = wc.height = m_tabWidth - TAB_TOP_HEIGHT*2 - 4;
    XConfigureWindow(display(), m_button, mask, &wc);
}


void Border::moveTo(int x, int y)
{
    XWindowChanges wc;
    wc.x = x - xIndent();
    wc.y = y - yIndent();
    XConfigureWindow(display(), m_parent, CWX | CWY, &wc);
}


void Border::map()
{
    if (m_parent == root()) {
	fprintf(stderr, "wm2: bad parent in Border::map()\n");
    } else {
	XMapWindow(display(), m_parent);

	if (!isTransient()) {
	    XMapWindow(display(), m_tab);
	    XMapWindow(display(), m_button);
	    if (!isFixedSize()) XMapWindow(display(), m_resize);
	}
    }
}


void Border::mapRaised()
{
    if (m_parent == root()) {
	fprintf(stderr, "wm2: bad parent in Border::mapRaised()\n");
    } else {
	XMapRaised(display(), m_parent);

	if (!isTransient()) {
	    XMapWindow(display(), m_tab);
	    XMapRaised(display(), m_button);
	    if (!isFixedSize()) XMapRaised(display(), m_resize);
	}
    }
}


void Border::lower()
{
    XLowerWindow(display(), m_parent);
}


void Border::unmap()
{
    if (m_parent == root()) {
	fprintf(stderr, "wm2: bad parent in Border::unmap()\n");
    } else {
	XUnmapWindow(display(), m_parent);

	if (!isTransient()) {
	    XUnmapWindow(display(), m_tab);
	    XUnmapWindow(display(), m_button);
//	    XUnmapWindow(display(), m_resize); // no, will unmap with parent
	}
    }
}


void Border::decorate(Boolean active, int w, int h)
{
    setFrameVisibility(active, w, h);
}


void Border::reparent()
{
    XReparentWindow(display(), m_child, m_parent, xIndent(), yIndent());
}

