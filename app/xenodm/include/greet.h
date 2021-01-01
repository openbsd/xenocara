/*

Copyright 1994, 1998  The Open Group

Permission to use, copy, modify, distribute, and sell this software and its
documentation for any purpose is hereby granted without fee, provided that
the above copyright notice appear in all copies and that both that
copyright notice and this permission notice appear in supporting
documentation.

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE OPEN GROUP BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.

Except as contained in this notice, the name of The Open Group shall
not be used in advertising or otherwise to promote the sale, use or
other dealings in this Software without prior written authorization
from The Open Group.

*/

/*
 * greet.h - interface to xdm's dynamically-loadable modular greeter
 */
#include <pwd.h>
#include <X11/Xlib.h>

/*
 * Return values for GreetUser();
 * Values must be explicitly defined because the greet library
 * may come from a different vendor.
 * Negative values indicate an error.
 */
typedef enum {
    Greet_Session_Over = 0,	/* session managed and over */
    Greet_Success = 1,		/* greet succeeded, session not managed */
    Greet_Failure = -1		/* greet failed */
} greet_user_rtn;

/*
 * GreetUser can either handle the user's session or allow xdm to do it.
 * The return or exit status of GreetUser indicates to xdm whether it
 * should start a session.
 *
 * GreetUser is passed the xdm struct display pointer, a pointer to a
 * Display, and pointers to greet and verify structs.  If it expects xdm
 * to run the session, it fills in the Display pointer and the fields
 * of the greet and verify structs.
 *
 * The verify struct includes the uid, gid, arguments to run the session,
 * environment for the session, and environment for startup/reset.
 *
 * The greet struct includes the user's name and password but these are
 * really only needed if xdm is compiled with a user-based authorization
 * option such as SECURE_RPC or K5AUTH.
 */

extern greet_user_rtn GreetUser(struct display *,
    struct verify_info *, struct greet_info *);

extern greet_user_rtn AutoLogin(struct display *,
    struct verify_info *, struct greet_info *);
