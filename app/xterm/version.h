/* $XTermId: version.h,v 1.306 2009/10/10 13:03:04 tom Exp $ */

/*
 * These definitions are used to build the string that's printed in response to
 * "xterm -version", or embedded in "xterm -help".  It usually indicates the
 * version of X to which this version of xterm has been built.  The number in
 * parentheses is my patch number (Thomas E. Dickey).
 */
#define XTERM_PATCH   250

#ifndef __vendorversion__
#define __vendorversion__ "XTerm/OpenBSD"
#endif
