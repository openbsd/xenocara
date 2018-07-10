/************************************************************

Copyright 1998 by Thomas E. Dickey <dickey@clark.net>

                        All Rights Reserved

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE ABOVE LISTED COPYRIGHT HOLDER(S) BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Except as contained in this notice, the name(s) of the above copyright
holders shall not be used in advertising or otherwise to promote the
sale, use or other dealings in this Software without prior written
authorization.

********************************************************/

#ifndef _DM_AUTH_H_
# define _DM_AUTH_H_ 1

# include "dm.h"	/* for struct display */

extern void	MitInitAuth (unsigned short name_len, char *name);
extern Xauth	*MitGetAuth (unsigned short namelen, char *name);

/* auth.c */
extern bool ValidAuthorization (unsigned short name_length, char *name);


extern bool SaveServerAuthorizations (struct display *d, Xauth **auths, int count);
extern void CleanUpFileName (char *src, char *dst, int len);
extern void RemoveUserAuthorization (struct display *d, struct verify_info *verify);
extern void SetAuthorization (struct display *d);
extern void SetLocalAuthorization (struct display *d);
extern void SetUserAuthorization (struct display *d, struct verify_info *verify);

/* genauth.c */
extern int GenerateAuthData (char *auth, int len);

#endif /* _DM_AUTH_H_ */
