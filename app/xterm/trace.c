/* $XTermId: trace.c,v 1.243 2024/09/30 07:36:29 tom Exp $ */

/*
 * Copyright 1997-2023,2024 by Thomas E. Dickey
 *
 *                         All Rights Reserved
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE ABOVE LISTED COPYRIGHT HOLDER(S) BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the name(s) of the above copyright
 * holders shall not be used in advertising or otherwise to promote the
 * sale, use or other dealings in this Software without prior written
 * authorization.
 */

/*
 * debugging support via TRACE macro.
 */

#include <xterm.h>		/* for definition of GCC_UNUSED */
#include <xstrings.h>
#include <wcwidth.h>
#include <version.h>

#if OPT_TRACE

#include <data.h>
#include <trace.h>

#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <assert.h>

#include <X11/Xatom.h>
#include <X11/Xmu/Atoms.h>
#include <X11/Xmu/Error.h>

#ifdef HAVE_X11_TRANSLATEI_H
#include <X11/ConvertI.h>
#include <X11/TranslateI.h>
#else
#ifdef __cplusplus
extern "C" {
#endif

    extern String _XtPrintXlations(Widget w,
				   XtTranslations xlations,
				   Widget accelWidget,
				   _XtBoolean includeRHS);
#ifdef __cplusplus
}
#endif
#endif
const char *trace_who = "parent";

static FILE *trace_fp;

static FILE *
TraceOpen(void)
{
    static const char *trace_out;

    if (trace_fp != 0
	&& trace_who != trace_out) {
	fclose(trace_fp);
	trace_fp = 0;
    }
    trace_out = trace_who;

    if (!trace_fp) {
	static char dot[] = ".";
	mode_t oldmask = umask(077);
	char *home;
	char *name;
	/*
	 * Put the trace-file in user's home-directory if the current
	 * directory is not writable.
	 */
	home = ((access(dot, R_OK | W_OK) == 0)
		? dot
		: getenv("HOME"));
	if (home != NULL &&
	    (name = malloc(strlen(home) + strlen(trace_who) + 50)) != NULL) {
#if OPT_TRACE_UNIQUE		/* usually I do not want unique names */
	    int unique;
	    for (unique = 0;; ++unique) {
		if (unique)
		    sprintf(name, "%s/Trace-%s.out-%d", home, trace_who, unique);
		else
		    sprintf(name, "%s/Trace-%s.out", home, trace_who);
		if ((trace_fp = fopen(name, "r")) == 0) {
		    break;
		}
		fclose(trace_fp);
	    }
#else
	    sprintf(name, "%s/Trace-%s.out", home, trace_who);
#endif
	    trace_fp = fopen(name, "w");
	    if (trace_fp != 0) {
		fprintf(trace_fp, "%s\n", xtermVersion());
		TraceIds(NULL, 0);
	    }
	    if (!trace_fp) {
		xtermWarning("Cannot open \"%s\"\n", name);
		exit(EXIT_FAILURE);
	    }
	    (void) umask(oldmask);
	    free(name);
	}
    }
    return trace_fp;
}

void
Trace(const char *fmt, ...)
{
    va_list ap;
    FILE *fp = TraceOpen();

    va_start(ap, fmt);
    vfprintf(fp, fmt, ap);
    (void) fflush(fp);
    va_end(ap);
}

void
TraceVA(const char *fmt, va_list ap)
{
    FILE *fp = TraceOpen();

    vfprintf(fp, fmt, ap);
    (void) fflush(fp);
}

void
TraceClose(void)
{
    if (trace_fp != 0) {
	(void) fclose(trace_fp);
	(void) fflush(stdout);
	(void) fflush(stderr);
	(void) visibleChars(NULL, 0);
	(void) visibleIChars(NULL, 0);
	trace_fp = 0;
    }
}

void
TraceXError(Display *d, XErrorEvent *ev)
{
    FILE *fp = TraceOpen();
    (void) XmuPrintDefaultErrorMessage(d, ev, fp);
    (void) fflush(fp);
}

void
TraceIds(const char *fname, int lnum)
{
    Trace("process %d ", (int) getpid());
#ifdef HAVE_UNISTD_H
    Trace("real (%u/%u) effective (%u/%u)",
	  (unsigned) getuid(), (unsigned) getgid(),
	  (unsigned) geteuid(), (unsigned) getegid());
#endif
    if (fname != 0) {
	Trace(" (%s@%d)\n", fname, lnum);
    } else {
	time_t now = time((time_t *) 0);
	Trace("-- %s", ctime(&now));
    }
}

void
TraceTime(const char *fname, int lnum)
{
    time_t now;
    if (fname != 0) {
	Trace("datetime (%s@%d) ", fname, lnum);
    }
    now = time((time_t *) 0);
    Trace("-- %s", ctime(&now));
}

static void
formatAscii(char *dst, unsigned value)
{
    switch (value) {
    case '\\':
	sprintf(dst, "\\\\");
	break;
    case '\b':
	sprintf(dst, "\\b");
	break;
    case '\n':
	sprintf(dst, "\\n");
	break;
    case '\r':
	sprintf(dst, "\\r");
	break;
    case '\t':
	sprintf(dst, "\\t");
	break;
    default:
	if (value < 32 || (value >= 127 && value < 160))
	    sprintf(dst, "\\%03o", value & 0xff);
	else
	    sprintf(dst, "%c", CharOf(value));
	break;
    }
}

#if OPT_DEC_CHRSET

const char *
visibleDblChrset(unsigned chrset)
{
    const char *result = "?";
    switch (chrset) {
    case CSET_SWL:
	result = "CSET_SWL";
	break;
    case CSET_DHL_TOP:
	result = "CSET_DHL_TOP";
	break;
    case CSET_DHL_BOT:
	result = "CSET_DHL_BOT";
	break;
    case CSET_DWL:
	result = "CSET_DWL";
	break;
    }
    return result;
}
#endif

const char *
visibleScsCode(DECNRCM_codes chrset)
{
#define MAP(to,from) case from: result = to ":" #from; break
    const char *result = "<ERR>";
    switch ((DECNRCM_codes) chrset) {
	MAP("B", nrc_ASCII);
	MAP("A", nrc_British);
	MAP("A", nrc_British_Latin_1);
	MAP("&4", nrc_DEC_Cyrillic);
	MAP("0", nrc_DEC_Spec_Graphic);
	MAP("1", nrc_DEC_Alt_Chars);
	MAP("2", nrc_DEC_Alt_Graphics);
	MAP("<", nrc_DEC_Supp);
	MAP("<", nrc_DEC_UPSS);
	MAP("%5", nrc_DEC_Supp_Graphic);
	MAP(">", nrc_DEC_Technical);
	MAP("4", nrc_Dutch);
	MAP("5", nrc_Finnish);
	MAP("C", nrc_Finnish2);
	MAP("R", nrc_French);
	MAP("f", nrc_French2);
	MAP("Q", nrc_French_Canadian);
	MAP("9", nrc_French_Canadian2);
	MAP("K", nrc_German);
	MAP("\"?", nrc_DEC_Greek_Supp);
	MAP("\">", nrc_Greek);
	MAP("F", nrc_ISO_Greek_Supp);
	MAP("\"4", nrc_DEC_Hebrew_Supp);
	MAP("%=", nrc_Hebrew);
	MAP("H", nrc_ISO_Hebrew_Supp);
	MAP("Y", nrc_Italian);
	MAP("A", nrc_ISO_Latin_1_Supp);
	MAP("B", nrc_ISO_Latin_2_Supp);
	MAP("I", nrc_JIS_Katakana);
	MAP("J", nrc_JIS_Roman);
	MAP("M", nrc_ISO_Latin_5_Supp);
	MAP("L", nrc_ISO_Latin_Cyrillic);
	MAP("`", nrc_Norwegian_Danish);
	MAP("E", nrc_Norwegian_Danish2);
	MAP("6", nrc_Norwegian_Danish3);
	MAP("%6", nrc_Portugese);
	MAP("&5", nrc_Russian);
	MAP("%3", nrc_SCS_NRCS);
	MAP("Z", nrc_Spanish);
	MAP("7", nrc_Swedish);
	MAP("H", nrc_Swedish2);
	MAP("=", nrc_Swiss);
	MAP("%2", nrc_Turkish);
	MAP("%0", nrc_DEC_Turkish_Supp);
	MAP("<UNK>", nrc_Unknown);
    }
#undef MAP
    return result;
}

const char *
visibleChars(const Char *buf, size_t len)
{
    static char *result;
    static size_t used;

    if (buf != 0) {
	size_t limit = ((len + 1) * 8) + 1;

	if (limit > used) {
	    used = limit;
	    result = realloc(result, used);
	}
	if (result != 0) {
	    char *dst = result;
	    *dst = '\0';
	    while (len--) {
		unsigned value = *buf++;
		formatAscii(dst, value);
		dst += strlen(dst);
	    }
	}
    } else {
	FreeAndNull(result);
	used = 0;
    }
    return NonNull(result);
}

const char *
visibleEventMode(EventMode value)
{
    const char *result;
    switch (value) {
    case NORMAL:
	result = "NORMAL";
	break;
    case LEFTEXTENSION:
	result = "LEFTEXTENSION";
	break;
    case RIGHTEXTENSION:
	result = "RIGHTEXTENSION";
	break;
    default:
	result = "?";
	break;
    }
    return result;
}

const char *
visibleIChars(const IChar *buf, size_t len)
{
    static char *result;
    static size_t used;

    if (buf != 0) {
	size_t limit = ((len + 1) * 12) + 1;

	if (limit > used) {
	    used = limit;
	    result = realloc(result, used);
	}
	if (result != 0) {
	    char *dst = result;
	    *dst = '\0';
	    while (len--) {
		unsigned value = *buf++;
#if OPT_WIDE_CHARS
		if (value > 255)
		    sprintf(dst, "\\U+%04X", value);
		else
#endif
		    formatAscii(dst, value);
		dst += strlen(dst);
	    }
	}
    } else {
	FreeAndNull(result);
	used = 0;
    }
    return NonNull(result);
}

const char *
visibleUChar(unsigned chr)
{
    IChar buf[1];
    buf[0] = (IChar) chr;
    return visibleIChars(buf, 1);
}

const char *
visibleEventType(int type)
{
    const char *result = "?";
    switch (type) {
	CASETYPE(KeyPress);
	CASETYPE(KeyRelease);
	CASETYPE(ButtonPress);
	CASETYPE(ButtonRelease);
	CASETYPE(MotionNotify);
	CASETYPE(EnterNotify);
	CASETYPE(LeaveNotify);
	CASETYPE(FocusIn);
	CASETYPE(FocusOut);
	CASETYPE(KeymapNotify);
	CASETYPE(Expose);
	CASETYPE(GraphicsExpose);
	CASETYPE(NoExpose);
	CASETYPE(VisibilityNotify);
	CASETYPE(CreateNotify);
	CASETYPE(DestroyNotify);
	CASETYPE(UnmapNotify);
	CASETYPE(MapNotify);
	CASETYPE(MapRequest);
	CASETYPE(ReparentNotify);
	CASETYPE(ConfigureNotify);
	CASETYPE(ConfigureRequest);
	CASETYPE(GravityNotify);
	CASETYPE(ResizeRequest);
	CASETYPE(CirculateNotify);
	CASETYPE(CirculateRequest);
	CASETYPE(PropertyNotify);
	CASETYPE(SelectionClear);
	CASETYPE(SelectionRequest);
	CASETYPE(SelectionNotify);
	CASETYPE(ColormapNotify);
	CASETYPE(ClientMessage);
	CASETYPE(MappingNotify);
    }
    return result;
}

const char *
visibleMappingMode(int code)
{
    const char *result = "?";
    switch (code) {
	CASETYPE(MappingModifier);
	CASETYPE(MappingKeyboard);
	CASETYPE(MappingPointer);
    }
    return result;
}

const char *
visibleNotifyMode(int code)
{
    const char *result = "?";
    switch (code) {
	CASETYPE(NotifyNormal);
	CASETYPE(NotifyGrab);
	CASETYPE(NotifyUngrab);
	CASETYPE(NotifyWhileGrabbed);
    }
    return result;
}

const char *
visibleNotifyDetail(int code)
{
    const char *result = "?";
    switch (code) {
	CASETYPE(NotifyAncestor);
	CASETYPE(NotifyVirtual);
	CASETYPE(NotifyInferior);
	CASETYPE(NotifyNonlinear);
	CASETYPE(NotifyNonlinearVirtual);
	CASETYPE(NotifyPointer);
	CASETYPE(NotifyPointerRoot);
	CASETYPE(NotifyDetailNone);
    }
    return result;
}

#if OPT_TEK4014
const char *
visibleSelectionTarget(Display *d, Atom a)
{
    const char *result = "?";

    if (a == XA_STRING) {
	result = "XA_STRING";
    } else if (a == XA_TEXT(d)) {
	result = "XA_TEXT()";
    } else if (a == XA_COMPOUND_TEXT(d)) {
	result = "XA_COMPOUND_TEXT()";
    } else if (a == XA_UTF8_STRING(d)) {
	result = "XA_UTF8_STRING()";
    } else if (a == XA_TARGETS(d)) {
	result = "XA_TARGETS()";
    }

    return result;
}

#if OPT_TEK4014
const char *
visibleTekparse(int code)
{
    static const struct {
	int code;
	const char *name;
    } table[] = {
#include "Tekparse.cin"
    };
    const char *result = "?";
    Cardinal n;
    for (n = 0; n < XtNumber(table); ++n) {
	if (table[n].code == code) {
	    result = table[n].name;
	    break;
	}
    }
    return result;
}
#endif

const char *
visibleVTparse(int code)
{
    static const struct {
	int code;
	const char *name;
    } table[] = {
#include "VTparse.cin"
    };
    const char *result = "?";
    Cardinal n;
    for (n = 0; n < XtNumber(table); ++n) {
	if (table[n].code == code) {
	    result = table[n].name;
	    break;
	}
    }
    return result;
}
#endif

const char *
visibleXError(int code)
{
    static char temp[80];
    const char *result = "?";
    switch (code) {
	CASETYPE(Success);
	CASETYPE(BadRequest);
	CASETYPE(BadValue);
	CASETYPE(BadWindow);
	CASETYPE(BadPixmap);
	CASETYPE(BadAtom);
	CASETYPE(BadCursor);
	CASETYPE(BadFont);
	CASETYPE(BadMatch);
	CASETYPE(BadDrawable);
	CASETYPE(BadAccess);
	CASETYPE(BadAlloc);
	CASETYPE(BadColor);
	CASETYPE(BadGC);
	CASETYPE(BadIDChoice);
	CASETYPE(BadName);
	CASETYPE(BadLength);
	CASETYPE(BadImplementation);
    default:
	sprintf(temp, "%d", code);
	result = temp;
	break;
    }
    return result;
}

#if OPT_TRACE_FLAGS
#define isScrnFlag(flag) ((flag) == LINEWRAPPED)

static char *
ScrnText(LineData *ld)
{
    return visibleIChars(ld->charData, ld->lineSize);
}

#define SHOW_BAD_LINE(name, ld) \
	Trace("OOPS " #name " bad row\n")

#define SHOW_SCRN_FLAG(name,code) \
	Trace(#name " %s:%s\n", \
	      code ? "*" : "", \
	      ScrnText(ld))

void
LineClrFlag(LineData *ld, int flag)
{
    if (ld == 0) {
	SHOW_BAD_LINE(LineClrFlag, ld);
	assert(0);
    } else if (isScrnFlag(flag)) {
	SHOW_SCRN_FLAG(LineClrFlag, 0);
    }

    LineFlags(ld) &= ~flag;
}

void
LineSetFlag(LineData *ld, int flag)
{
    if (ld == 0) {
	SHOW_BAD_LINE(LineSetFlag, ld);
	assert(0);
    } else if (isScrnFlag(flag)) {
	SHOW_SCRN_FLAG(LineSetFlag, 1);
    }

    LineFlags(ld) |= flag;
}

int
LineTstFlag(LineData ld, int flag)
{
    int code = 0;
    if (ld == 0) {
	SHOW_BAD_LINE(LineTstFlag, ld);
    } else {
	code = LineFlags(ld);

	if (isScrnFlag(flag)) {
	    SHOW_SCRN_FLAG(LineTstFlag, code);
	}
    }
    return code;
}
#endif /* OPT_TRACE_FLAGS */

const char *
TraceAtomName(Display *dpy, Atom atom)
{
    static char *result;
    free(result);
    if (atom != 0) {
	result = XGetAtomName(dpy, atom);
    } else {
	result = x_strdup("NONE");
    }
    return result;
}

/*
 * Trace the normal or alternate screen, showing color values up to 16, e.g.,
 * for debugging with vttest.
 */
void
TraceScreen(XtermWidget xw, int whichBuf)
{
    TScreen *screen = TScreenOf(xw);

    if (screen->editBuf_index[whichBuf]) {
	int row;

	TRACE(("TraceScreen %d:\n", whichBuf));
	for (row = 0; row <= LastRowNumber(screen); ++row) {
	    LineData *ld = getLineData(screen, row);

	    TRACE((" %3d:", row));
	    if (ld != 0) {
		int col;

		for (col = 0; col < ld->lineSize; ++col) {
		    int ch = (int) ld->charData[col];
		    if (ch < ' ')
			ch = ' ';
		    if (ch >= 127)
			ch = '#';
		    TRACE(("%c", ch));
		}
		TRACE((":\n"));

#if 0
		TRACE(("  xx:"));
		for (col = 0; col < ld->lineSize; ++col) {
		    unsigned attrs = ld->attribs[col];
		    char ch;
		    if (attrs & PROTECTED) {
			ch = '*';
		    } else if (attrs & BLINK) {
			ch = 'B';
		    } else if (attrs & CHARDRAWN) {
			ch = '+';
		    } else {
			ch = ' ';
		    }
		    TRACE(("%c", ch));
		}
		TRACE((":\n"));
#endif

#if 0
		TRACE(("  fg:"));
		for (col = 0; col < ld->lineSize; ++col) {
		    unsigned fg = extract_fg(xw, ld->color[col], ld->attribs[col]);
		    if (fg > 15)
			fg = 15;
		    TRACE(("%1x", fg));
		}
		TRACE((":\n"));

		TRACE(("  bg:"));
		for (col = 0; col < ld->lineSize; ++col) {
		    unsigned bg = extract_bg(xw, ld->color[col], ld->attribs[col]);
		    if (bg > 15)
			bg = 15;
		    TRACE(("%1x", bg));
		}
		TRACE((":\n"));
#endif
	    } else {
		TRACE(("null lineData\n"));
	    }
	}
    } else {
	TRACE(("TraceScreen %d is nil\n", whichBuf));
    }
}

static char *
formatEventMask(char *target, unsigned source, Boolean buttons)
{
#define DATA(name) { name ## Mask, #name }
    static struct {
	unsigned mask;
	const char *name;
    } table[] = {
	DATA(Shift),
	    DATA(Lock),
	    DATA(Control),
	    DATA(Mod1),
	    DATA(Mod2),
	    DATA(Mod3),
	    DATA(Mod4),
	    DATA(Mod5),
	    DATA(Button1),
	    DATA(Button2),
	    DATA(Button3),
	    DATA(Button4),
	    DATA(Button5),
    };
#undef DATA
    Cardinal n;
    char marker = L_CURL;
    char *base = target;

    for (n = 0; n < XtNumber(table); ++n) {
	if (!buttons && (table[n].mask >= Button1Mask))
	    continue;
	if ((table[n].mask & source)) {
	    UIntClr(source, table[n].mask);
	    sprintf(target, "%c%s", marker, table[n].name);
	    target += strlen(target);
	    marker = '|';
	}
    }

    if (source != 0) {
	sprintf(target, "%c?%#x", marker, source);
	target += strlen(target);
	marker = '|';
    }

    if (marker == L_CURL)
	*target++ = L_CURL;
    *target++ = R_CURL;

    *target = '\0';
    return base;
}

void
TraceEvent(const char *tag, XEvent *ev, String *params, const Cardinal *num_params)
{
    char mask_buffer[160];

    TRACE(("Event #%lu %s: %#lx %s",
	   ev->xany.serial,
	   tag,
	   ev->xany.window,
	   visibleEventType(ev->type)));

    switch (ev->type) {
    case KeyPress:
	/* FALLTHRU */
    case KeyRelease:
	TRACE((" keycode 0x%04X %s",
	       ev->xkey.keycode,
	       formatEventMask(mask_buffer, ev->xkey.state, False)));
	break;
    case ButtonPress:
	/* FALLTHRU */
    case ButtonRelease:
	TRACE((" button %u state %#x %s",
	       ev->xbutton.button,
	       ev->xbutton.state,
	       formatEventMask(mask_buffer, ev->xbutton.state, True)));
	break;
    case MotionNotify:
	TRACE((" (%d,%d) state %#x %s",
	       ev->xmotion.y_root,
	       ev->xmotion.x_root,
	       ev->xmotion.state,
	       formatEventMask(mask_buffer, ev->xmotion.state, True)));
	break;
    case EnterNotify:
    case LeaveNotify:
	TRACE((" at %d,%d root %d,%d %s %s",
	       ev->xcrossing.y,
	       ev->xcrossing.x,
	       ev->xcrossing.y_root,
	       ev->xcrossing.x_root,
	       visibleNotifyMode(ev->xcrossing.mode),
	       visibleNotifyDetail(ev->xcrossing.detail)));
	break;
    case FocusIn:
    case FocusOut:
	TRACE((" %s %s",
	       visibleNotifyMode(ev->xfocus.mode),
	       visibleNotifyDetail(ev->xfocus.detail)));
	break;
    case MapNotify:
	TRACE((" event %#lx %s",
	       ev->xmap.event,
	       ev->xmap.override_redirect ? "override" : ""));
	break;
    case UnmapNotify:
	TRACE((" event %#lx %s",
	       ev->xunmap.event,
	       ev->xunmap.from_configure ? "configure" : ""));
	break;
    case ReparentNotify:
	TRACE((" at %d,%d event %#lx parent %#lx %s",
	       ev->xreparent.y,
	       ev->xreparent.x,
	       ev->xreparent.event,
	       ev->xreparent.parent,
	       ev->xreparent.override_redirect ? "override" : ""));
	break;
    case ConfigureNotify:
	TRACE((" at %d,%d size %dx%d bd %d above %#lx",
	       ev->xconfigure.y,
	       ev->xconfigure.x,
	       ev->xconfigure.height,
	       ev->xconfigure.width,
	       ev->xconfigure.border_width,
	       ev->xconfigure.above));
	break;
    case CreateNotify:
	TRACE((" at %d,%d size %dx%d bd %d",
	       ev->xcreatewindow.y,
	       ev->xcreatewindow.x,
	       ev->xcreatewindow.height,
	       ev->xcreatewindow.width,
	       ev->xcreatewindow.border_width));
	break;
    case ResizeRequest:
	TRACE((" size %dx%d",
	       ev->xresizerequest.height,
	       ev->xresizerequest.width));
	break;
    case PropertyNotify:
	TRACE((" %s %s",
	       TraceAtomName(XtDisplay(toplevel), ev->xproperty.atom),
	       ((ev->xproperty.state == PropertyNewValue)
		? "NewValue"
		: ((ev->xproperty.state == PropertyDelete)
		   ? "Delete"
		   : "?"))));

	break;
    case Expose:
	TRACE((" at %d,%d size %dx%d count %d",
	       ev->xexpose.y,
	       ev->xexpose.x,
	       ev->xexpose.height,
	       ev->xexpose.width,
	       ev->xexpose.count));
	break;
    case MappingNotify:
	TRACE((" %s first_keycode %d count %d",
	       visibleMappingMode(ev->xmapping.request),
	       ev->xmapping.first_keycode,
	       ev->xmapping.count));
	break;
    case VisibilityNotify:
	TRACE((" state %d",
	       ev->xvisibility.state));
	break;
    case KeymapNotify:
	{
	    Cardinal j;
	    for (j = 0; j < XtNumber(ev->xkeymap.key_vector); ++j) {
		if (ev->xkeymap.key_vector[j] != 0) {
		    Cardinal k;
		    for (k = 0; k < CHAR_BIT; ++k) {
			if (ev->xkeymap.key_vector[j] & (1 << k)) {
			    TRACE((" key%u", (j * CHAR_BIT) + k));
			}
		    }
		}
	    }
	}
	break;
    case NoExpose:
	TRACE((" send_event:%d display %p major:%d minor:%d",
	       ev->xnoexpose.send_event,
	       (void *) ev->xnoexpose.display,
	       ev->xnoexpose.major_code,
	       ev->xnoexpose.minor_code));
	break;
    case GraphicsExpose:
	TRACE((" send_event:%d display %p major:%d minor:%d",
	       ev->xgraphicsexpose.send_event,
	       (void *) ev->xgraphicsexpose.display,
	       ev->xgraphicsexpose.major_code,
	       ev->xgraphicsexpose.minor_code));
	break;
    case SelectionClear:
	TRACE((" selection:%s",
	       TraceAtomName(ev->xselectionclear.display,
			     ev->xselectionclear.selection)));
	break;
    case SelectionRequest:
	TRACE((" owner:%#lx requestor:%#lx",
	       ev->xselectionrequest.owner,
	       ev->xselectionrequest.requestor));
	TRACE((" selection:%s",
	       TraceAtomName(ev->xselectionrequest.display,
			     ev->xselectionrequest.selection)));
	TRACE((" target:%s",
	       TraceAtomName(ev->xselectionrequest.display,
			     ev->xselectionrequest.target)));
	TRACE((" property:%s",
	       TraceAtomName(ev->xselectionrequest.display,
			     ev->xselectionrequest.property)));
	break;
    default:
	TRACE((":FIXME"));
	break;
    }
    TRACE(("\n"));
    if (params != 0 && *num_params != 0) {
	Cardinal n;
	for (n = 0; n < *num_params; ++n) {
	    TRACE(("  param[%d] = %s\n", n, params[n]));
	}
    }
}

#if OPT_RENDERFONT && OPT_WIDE_CHARS
void
TraceFallback(XtermWidget xw, const char *tag, unsigned wc, int n, XftFont *font)
{
    TScreen *screen = TScreenOf(xw);
    XGlyphInfo gi;
    int expect = my_wcwidth((wchar_t) wc);
    int hijack = mk_wcwidth_cjk((wchar_t) wc);
    int actual;

    XftTextExtents32(screen->display, font, &wc, 1, &gi);
    actual = ((gi.xOff + FontWidth(screen) - 1)
	      / FontWidth(screen));

    TRACE(("FALLBACK #%d %s U+%04X %d,%d pos,"
	   " %d,%d off," " %dx%d size,"
	   " %d/%d next," " %d vs %d/%d cells%s\n",
	   n + 1, tag, wc,
	   gi.y, gi.x,
	   gi.yOff, gi.xOff,
	   gi.height, gi.width,
	   font->max_advance_width,
	   FontWidth(screen),
	   actual, expect, hijack,
	   ((actual != expect)
	    ? ((actual == hijack)
	       ? " OOPS"
	       : " oops")
	    : "")));
}
#endif /* OPT_RENDERFONT */

void
TraceFocus(Widget w, XEvent *ev)
{
    TRACE(("trace_focus event type %d:%s\n",
	   ev->type, visibleEventType(ev->type)));
    switch (ev->type) {
    case FocusIn:
    case FocusOut:
	{
	    XFocusChangeEvent *event = (XFocusChangeEvent *) ev;
	    TRACE(("\tdetail: %s\n", visibleNotifyDetail(event->detail)));
	    TRACE(("\tmode:   %s\n", visibleNotifyMode(event->mode)));
	    TRACE(("\twindow: %#lx\n", event->window));
	}
	break;
    case EnterNotify:
    case LeaveNotify:
	{
	    XCrossingEvent *event = (XCrossingEvent *) ev;
	    TRACE(("\tdetail:    %s\n", visibleNotifyDetail(event->detail)));
	    TRACE(("\tmode:      %s\n", visibleNotifyMode(event->mode)));
	    TRACE(("\twindow:    %#lx\n", event->window));
	    TRACE(("\tfocus:     %d\n", event->focus));
	    TRACE(("\troot:      %#lx\n", event->root));
	    TRACE(("\tsubwindow: %#lx\n", event->subwindow));
	}
	break;
    }
    while (w != 0) {
	TRACE(("w %p -> %#lx\n", (void *) w, XtWindow(w)));
	w = XtParent(w);
    }
}

void
TraceSizeHints(XSizeHints * hints)
{
    TRACE(("size hints:\n"));
    if (hints->flags & (USPosition | PPosition)) {
	TRACE(("   position   %d,%d%s%s\n", hints->y, hints->x,
	       (hints->flags & USPosition) ? " user" : "",
	       (hints->flags & PPosition) ? " prog" : ""));
    }
    if (hints->flags & (USSize | PSize)) {
	TRACE(("   size       %d,%d%s%s\n", hints->height, hints->width,
	       (hints->flags & USSize) ? " user" : "",
	       (hints->flags & PSize) ? " prog" : ""));
    }
    if (hints->flags & PMinSize) {
	TRACE(("   min        %d,%d\n", hints->min_height, hints->min_width));
    }
    if (hints->flags & PMaxSize) {
	TRACE(("   max        %d,%d\n", hints->max_height, hints->max_width));
    }
    if (hints->flags & PResizeInc) {
	TRACE(("   inc        %d,%d\n", hints->height_inc, hints->width_inc));
    } else {
	TRACE(("   inc        NONE!\n"));
    }
    if (hints->flags & PAspect) {
	TRACE(("   min aspect %d/%d\n", hints->min_aspect.y, hints->min_aspect.y));
	TRACE(("   max aspect %d/%d\n", hints->max_aspect.y, hints->max_aspect.y));
    }
    if (hints->flags & PBaseSize) {
	TRACE(("   base       %d,%d\n", hints->base_height, hints->base_width));
    }
    if (hints->flags & PWinGravity) {
	TRACE(("   gravity    %d\n", hints->win_gravity));
    }
}

static void
TraceEventMask(const char *tag, long mask)
{
#define DATA(name) { name##Mask, #name }
    /* *INDENT-OFF* */
    static struct {
	long mask;
	const char *name;
    } table[] = {
	DATA(KeyPress),
	DATA(KeyRelease),
	DATA(ButtonPress),
	DATA(ButtonRelease),
	DATA(EnterWindow),
	DATA(LeaveWindow),
	DATA(PointerMotion),
	DATA(PointerMotionHint),
	DATA(Button1Motion),
	DATA(Button2Motion),
	DATA(Button3Motion),
	DATA(Button4Motion),
	DATA(Button5Motion),
	DATA(ButtonMotion),
	DATA(KeymapState),
	DATA(Exposure),
	DATA(VisibilityChange),
	DATA(StructureNotify),
	DATA(ResizeRedirect),
	DATA(SubstructureNotify),
	DATA(SubstructureRedirect),
	DATA(FocusChange),
	DATA(PropertyChange),
	DATA(ColormapChange),
	DATA(OwnerGrabButton),
    };
#undef DATA
    Cardinal n;
    /* *INDENT-ON* */

    for (n = 0; n < XtNumber(table); ++n) {
	if (table[n].mask & mask) {
	    TRACE(("%s %s\n", tag, table[n].name));
	}
    }
}

void
TraceWindowAttributes(XWindowAttributes * attrs)
{
    TRACE(("window attributes:\n"));
    TRACE(("   position     %d,%d\n", attrs->y, attrs->x));
    TRACE(("   size         %dx%d\n", attrs->height, attrs->width));
    TRACE(("   border       %d\n", attrs->border_width));
    TRACE(("   depth        %d\n", attrs->depth));
    TRACE(("   bit_gravity  %d\n", attrs->bit_gravity));
    TRACE(("   win_gravity  %d\n", attrs->win_gravity));
    TRACE(("   root         %#lx\n", (long) attrs->root));
    TRACE(("   class        %s\n", ((attrs->class == InputOutput)
				    ? "InputOutput"
				    : ((attrs->class == InputOnly)
				       ? "InputOnly"
				       : "unknown"))));
    TRACE(("   map_state    %s\n", ((attrs->map_state == IsUnmapped)
				    ? "IsUnmapped"
				    : ((attrs->map_state == IsUnviewable)
				       ? "IsUnviewable"
				       : ((attrs->map_state == IsViewable)
					  ? "IsViewable"
					  : "unknown")))));
    TRACE(("   all_events\n"));
    TraceEventMask("        ", attrs->all_event_masks);
    TRACE(("   your_events\n"));
    TraceEventMask("        ", attrs->your_event_mask);
    TRACE(("   no_propagate\n"));
    TraceEventMask("        ", attrs->do_not_propagate_mask);
}

void
TraceWMSizeHints(XtermWidget xw)
{
    XSizeHints sizehints = xw->hints;

    getXtermSizeHints(xw);
    TraceSizeHints(&xw->hints);
    xw->hints = sizehints;
}

const char *
ModifierName(unsigned modifier)
{
    const char *s = "";
    if (modifier & ShiftMask)
	s = " Shift";
    else if (modifier & LockMask)
	s = " Lock";
    else if (modifier & ControlMask)
	s = " Control";
    else if (modifier & Mod1Mask)
	s = " Mod1";
    else if (modifier & Mod2Mask)
	s = " Mod2";
    else if (modifier & Mod3Mask)
	s = " Mod3";
    else if (modifier & Mod4Mask)
	s = " Mod4";
    else if (modifier & Mod5Mask)
	s = " Mod5";
    return s;
}

void
TraceTranslations(const char *name, Widget w)
{
    String result;
    XErrorHandler save = XSetErrorHandler(ignore_x11_error);
    XtTranslations xlations;
    Widget xcelerat;

    TRACE(("TraceTranslations for %s (widget %#lx) " TRACE_L "\n",
	   name, (long) w));
    if (w) {
	XtVaGetValues(w,
		      XtNtranslations, &xlations,
		      XtNaccelerators, &xcelerat,
		      (XtPointer) 0);
	TRACE(("... xlations %#08lx\n", (long) xlations));
	TRACE(("... xcelerat %#08lx\n", (long) xcelerat));
	result = _XtPrintXlations(w, xlations, xcelerat, True);
	TRACE(("%s\n", NonNull(result)));
	if (result)
	    XFree((char *) result);
    } else {
	TRACE(("none (widget is null)\n"));
    }
    TRACE((TRACE_R "\n"));
    XSetErrorHandler(save);
}

XtGeometryResult
TraceResizeRequest(const char *fn, int ln, Widget w,
		   unsigned reqwide,
		   unsigned reqhigh,
		   Dimension *gotwide,
		   Dimension *gothigh)
{
    XtGeometryResult rc;

    TRACE(("%s@%d ResizeRequest %ux%u\n", fn, ln, reqhigh, reqwide));
    rc = XtMakeResizeRequest((Widget) w,
			     (Dimension) reqwide,
			     (Dimension) reqhigh,
			     gotwide, gothigh);
    TRACE(("... ResizeRequest -> "));
    if (gothigh && gotwide)
	TRACE(("%dx%d ", *gothigh, *gotwide));
    TRACE(("(%d)\n", rc));
    return rc;
}

#define XRES_S(name) Trace(#name " = %s\n", NonNull(resp->name))
#define XRES_B(name) Trace(#name " = %s\n", MtoS(resp->name))
#define XRES_I(name) Trace(#name " = %d\n", resp->name)

void
TraceXtermResources(void)
{
    XTERM_RESOURCE *resp = &resource;

    Trace("XTERM_RESOURCE settings:\n");
    XRES_S(icon_geometry);
    XRES_S(title);
    XRES_S(icon_hint);
    XRES_S(icon_name);
    XRES_S(term_name);
    XRES_S(tty_modes);
    XRES_I(minBufSize);
    XRES_I(maxBufSize);
    XRES_B(hold_screen);
    XRES_B(utmpInhibit);
    XRES_B(utmpDisplayId);
    XRES_B(messages);
    XRES_S(menuLocale);
    XRES_S(omitTranslation);
    XRES_S(keyboardType);
#ifdef HAVE_LIB_XCURSOR
    XRES_S(cursorTheme);
#endif
#if OPT_PRINT_ON_EXIT
    XRES_I(printModeNow);
    XRES_I(printModeOnXError);
    XRES_I(printOptsNow);
    XRES_I(printOptsOnXError);
    XRES_S(printFileNow);
    XRES_S(printFileOnXError);
#endif
#if OPT_SUNPC_KBD
    XRES_B(sunKeyboard);
#endif
#if OPT_HP_FUNC_KEYS
    XRES_B(hpFunctionKeys);
#endif
#if OPT_SCO_FUNC_KEYS
    XRES_B(scoFunctionKeys);
#endif
#if OPT_SUN_FUNC_KEYS
    XRES_B(sunFunctionKeys);
#endif
#if OPT_INITIAL_ERASE
    XRES_B(ptyInitialErase);
    XRES_B(backarrow_is_erase);
#endif
    XRES_B(useInsertMode);
#if OPT_ZICONBEEP
    XRES_I(zIconBeep);
    XRES_S(zIconFormat);
#endif
#if OPT_PTY_HANDSHAKE
    XRES_B(wait_for_map);
    XRES_B(ptyHandshake);
    XRES_B(ptySttySize);
#endif
#if OPT_REPORT_CCLASS
    XRES_B(reportCClass);
#endif
#if OPT_REPORT_COLORS
    XRES_B(reportColors);
#endif
#if OPT_REPORT_FONTS
    XRES_B(reportFonts);
#endif
#if OPT_REPORT_ICONS
    XRES_B(reportIcons);
#endif
#if OPT_SAME_NAME
    XRES_B(sameName);
#endif
#if OPT_SESSION_MGT
    XRES_B(sessionMgt);
#endif
#if OPT_TOOLBAR
    XRES_B(toolBar);
#endif
#if OPT_MAXIMIZE
    XRES_B(maximized);
    XRES_S(fullscreen_s);
#endif
#if USE_DOUBLE_BUFFER
    XRES_B(buffered);
    XRES_I(buffered_fps);
#endif
}

void
TraceArgv(const char *tag, char **argv)
{
    TRACE(("%s:\n", tag));
    if (argv != 0) {
	int n = 0;

	while (*argv != 0) {
	    TRACE(("  %d:%s\n", n++, *argv++));
	}
    }
}

static char *
parse_option(char *dst, String src, int first)
{
    char *s;

    if (!strncmp(src, "-/+", (size_t) 3)) {
	dst[0] = (char) first;
	strcpy(dst + 1, src + 3);
    } else {
	strcpy(dst, src);
    }
    for (s = dst; *s != '\0'; s++) {
	if (*s == '#' || *s == '%' || *s == 'S') {
	    s[1] = '\0';
	} else if (*s == ' ') {
	    *s = '\0';
	    break;
	}
    }
    return dst;
}

static Bool
same_option(OptionHelp * opt, XrmOptionDescRec * res)
{
    char temp[BUFSIZ];
    return !strcmp(parse_option(temp, opt->opt, res->option[0]), res->option);
}

static Bool
standard_option(String opt)
{
    static const char *table[] =
    {
	"+rv",
	"+synchronous",
	"-background",
	"-bd",
	"-bg",
	"-bordercolor",
	"-borderwidth",
	"-bw",
	"-display",
	"-fg",
	"-fn",
	"-font",
	"-foreground",
	"-geometry",
	"-iconic",
	"-name",
	"-reverse",
	"-rv",
	"-selectionTimeout",
	"-synchronous",
	"-title",
	"-xnllanguage",
	"-xrm",
	"-xtsessionID",
    };
    Cardinal n;
    char temp[BUFSIZ];

    opt = parse_option(temp, opt, '-');
    for (n = 0; n < XtNumber(table); n++) {
	if (!strcmp(opt, table[n]))
	    return True;
    }
    return False;
}

/*
 * Analyse the options/help messages for inconsistencies.
 */
void
TraceOptions(OptionHelp * options, XrmOptionDescRec * resources, Cardinal res_count)
{
    OptionHelp *opt_array = sortedOpts(options, resources, res_count);
    size_t j, k;
    XrmOptionDescRec *res_array = sortedOptDescs(resources, res_count);
    Bool first, found;

    TRACE(("Checking options-tables for inconsistencies:\n"));

#if 0
    TRACE(("Options listed in help-message:\n"));
    for (j = 0; options[j].opt != 0; j++)
	TRACE(("%5d %-28s %s\n", j, opt_array[j].opt, opt_array[j].desc));
    TRACE(("Options listed in resource-table:\n"));
    for (j = 0; j < res_count; j++)
	TRACE(("%5d %-28s %s\n", j, res_array[j].option, res_array[j].specifier));
#endif

    /* list all options[] not found in resources[] */
    for (j = 0, first = True; options[j].opt != 0; j++) {
	found = False;
	for (k = 0; k < res_count; k++) {
	    if (same_option(&opt_array[j], &res_array[k])) {
		found = True;
		break;
	    }
	}
	if (!found) {
	    if (first) {
		TRACE(("Options listed in help, not found in resource list:\n"));
		first = False;
	    }
	    TRACE(("  %-28s%s\n", opt_array[j].opt,
		   standard_option(opt_array[j].opt) ? " (standard)" : ""));
	}
    }

    /* list all resources[] not found in options[] */
    for (j = 0, first = True; j < res_count; j++) {
	found = False;
	for (k = 0; options[k].opt != 0; k++) {
	    if (same_option(&opt_array[k], &res_array[j])) {
		found = True;
		break;
	    }
	}
	if (!found) {
	    if (first) {
		TRACE(("Resource list items not found in options-help:\n"));
		first = False;
	    }
	    TRACE(("  %s\n", res_array[j].option));
	}
    }

    TRACE(("Resource list items that will be ignored by XtOpenApplication:\n"));
    for (j = 0; j < res_count; j++) {
	switch (res_array[j].argKind) {
	case XrmoptionSkipArg:
	    TRACE(("  %-28s {param}\n", res_array[j].option));
	    break;
	case XrmoptionSkipNArgs:
	    TRACE(("  %-28s {%ld params}\n", res_array[j].option, (long)
		   res_array[j].value));
	    break;
	case XrmoptionSkipLine:
	    TRACE(("  %-28s {remainder of line}\n", res_array[j].option));
	    break;
	case XrmoptionIsArg:
	case XrmoptionNoArg:
	case XrmoptionResArg:
	case XrmoptionSepArg:
	case XrmoptionStickyArg:
	default:
	    break;
	}
    }
}
#else
extern void empty_trace(void);
void
empty_trace(void)
{
}
#endif
