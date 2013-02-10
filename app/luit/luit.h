/*
Copyright (c) 2001 by Juliusz Chroboczek

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#ifndef LUIT_LUIT_H
#define LUIT_LUIT_H 1

extern const char *locale_alias;
extern int iso2022;
extern int verbose;
extern int sevenbit;
extern int ilog;
extern int olog;

/* Not using Xfuncproto.h since that would pull in extra dependencies */
#if (defined(__GNUC__) && ((__GNUC__ * 100 + __GNUC_MINOR__) >= 205)) \
        || (defined(__SUNPRO_C) && (__SUNPRO_C >= 0x590))
# define LUIT_NORETURN __attribute__((noreturn))
#else
# define LUIT_NORETURN /* nothing */
#endif

#if defined(__GNUC__) && ((__GNUC__ * 100 + __GNUC_MINOR__) >= 203)
# define LUIT_PRINTF(x,y) __attribute__((__format__(__printf__,x,y)))
#else /* not gcc >= 2.3 */
# define LUIT_PRINTF(x,y) /* nothing */
#endif

void child(char *, char *, char *const *) LUIT_NORETURN;
void parent(int, int);

void ErrorF(const char *f,...) LUIT_PRINTF(1,2);

void FatalError(const char *f,...) LUIT_NORETURN LUIT_PRINTF(1,2);

#endif /* LUIT_LUIT_H */
