/* $Xorg: StrKeysym.c,v 1.5 2001/02/09 02:03:37 xorgcvs Exp $ */
/*

Copyright 1985, 1987, 1990, 1998  The Open Group

Permission to use, copy, modify, distribute, and sell this software and its
documentation for any purpose is hereby granted without fee, provided that
the above copyright notice appear in all copies and that both that
copyright notice and this permission notice appear in supporting
documentation.

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
OPEN GROUP BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Except as contained in this notice, the name of The Open Group shall not be
used in advertising or otherwise to promote the sale, use or other dealings
in this Software without prior written authorization from The Open Group.

*/
/* $XFree86: xc/lib/X11/StrKeysym.c,v 3.7 2003/04/13 19:22:18 dawes Exp $ */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <xf86.h>
#include <X11/keysymdef.h>

#define NEEDKTABLE
#include "ks_tables.h"

#include "StrKeysym.h"


KeySym
XStringToKeysym(_Xconst char *s)
{
    register int i, n;
    int h;
    register unsigned long sig = 0;
    register const char *p = s;
    register int c;
    register int idx;
    const unsigned char *entry;
    unsigned char sig1, sig2;
    KeySym val;

    while ((c = *p++))
	sig = (sig << 1) + c;
    i = sig % KTABLESIZE;
    h = i + 1;
    sig1 = (sig >> 8) & 0xff;
    sig2 = sig & 0xff;
    n = KMAXHASH;
    while ((idx = hashString[i]))
    {
	entry = &_XkeyTable[idx];
	if ((entry[0] == sig1) && (entry[1] == sig2) &&
	    !strcmp(s, (char *)entry + 6))
	{
	    val = (entry[2] << 24) | (entry[3] << 16) |
	          (entry[4] << 8)  | entry[5];
	    if (!val)
		val = XK_VoidSymbol;
	    return val;
	}
	if (!--n)
	    break;
	i += h;
	if (i >= KTABLESIZE)
	    i -= KTABLESIZE;
    }

    return NoSymbol;
}
