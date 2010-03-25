/* $XTermId: trace.h,v 1.51 2009/11/27 18:31:16 tom Exp $ */

/*
 * 
 * Copyright 1997-2008,2009 by Thomas E. Dickey
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
 * 
 */

/*
 * Common/useful definitions for XTERM application
 */
#ifndef	included_trace_h
#define	included_trace_h

#include <xterm.h>

#if OPT_TRACE

extern	void	Trace ( const char *, ... )
#ifdef GCC_PRINTF
	__attribute__ ((format(printf,1,2)))
#endif
	;
#undef  TRACE
#define TRACE(p) Trace p

#if OPT_TRACE > 1
#define TRACE2(p) Trace p
#endif

extern	char *	visibleChars (Char * /* buf */, unsigned /* len */);
extern	char *	visibleIChar (IChar *, unsigned);
extern	char *	visibleIChars (IChar * /* buf */, unsigned /* len */);
extern	const char * visibleChrsetName(unsigned /* chrset */);
extern	const char * visibleEventType (int);
extern	const char * visibleNotifyDetail(int /* code */);
extern	const char * visibleSelectionTarget(Display * /* d */, Atom /* a */);
extern	const char * visibleXError (int /* code */);

extern	void	TraceArgv(const char * /* tag */, char ** /* argv */);
#undef  TRACE_ARGV
#define	TRACE_ARGV(tag,argv) TraceArgv(tag,argv)

extern	const	char *trace_who;
#undef  TRACE_CHILD
#define TRACE_CHILD int tracing_child = (trace_who = "child") != 0; (void) tracing_child;

extern	void	TraceFocus(Widget, XEvent *);
#undef  TRACE_FOCUS
#define	TRACE_FOCUS(w,e) TraceFocus((Widget)w, (XEvent *)e)

extern	void	TraceSizeHints(XSizeHints *);
#undef  TRACE_HINTS
#define	TRACE_HINTS(hints) TraceSizeHints(hints)

extern	void	TraceIds(const char * /* fname */, int  /* lnum */);
#undef  TRACE_IDS
#define	TRACE_IDS TraceIds(__FILE__, __LINE__)

extern	void	TraceOptions(OptionHelp * /* options */, XrmOptionDescRec * /* resources */, Cardinal  /* count */);
#undef  TRACE_OPTS
#define	TRACE_OPTS(opts,ress,lens) TraceOptions(opts,ress,lens)

extern	void	TraceTranslations(const char *, Widget);
#undef  TRACE_TRANS
#define	TRACE_TRANS(name,w) TraceTranslations(name,w)

extern	void	TraceWMSizeHints(XtermWidget);
#undef  TRACE_WM_HINTS
#define	TRACE_WM_HINTS(w) TraceWMSizeHints(w)

extern	void	TraceXtermResources(void);
#undef  TRACE_XRES
#define	TRACE_XRES() TraceXtermResources()

extern	int	TraceResizeRequest(const char * /* fn */, int  /* ln */, Widget  /* w */, Dimension  /* reqwide */, Dimension  /* reqhigh */, Dimension * /* gotwide */, Dimension * /* gothigh */);
#undef  REQ_RESIZE
#define REQ_RESIZE(w, reqwide, reqhigh, gotwide, gothigh) \
	TraceResizeRequest(__FILE__, __LINE__, w, reqwide, reqhigh, gotwide, gothigh)

#else

#define REQ_RESIZE(w, reqwide, reqhigh, gotwide, gothigh) \
	XtMakeResizeRequest((Widget) (w), \
			    (Dimension) (reqwide), (Dimension) (reqhigh), \
			    (gotwide), (gothigh))

#endif

#endif	/* included_trace_h */
