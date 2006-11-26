/* $XTermId: version.h,v 1.271 2006/06/21 00:34:24 tom Exp $ */
/* $XFree86: xc/programs/xterm/version.h,v 3.126 2006/06/20 00:42:38 dickey Exp $ */

/*
 * These definitions are used to build the string that's printed in response to
 * "xterm -version", or embedded in "xterm -help".  It usually indicates the
 * version of X to which this version of xterm has been built.  The number in
 * parentheses is my patch number (T.Dickey).
 */
#define XTERM_PATCH   216

#ifndef __vendorversion__
#define __vendorversion__ "XTerm/OpenBSD"
#endif
