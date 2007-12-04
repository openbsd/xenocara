/*
 * Copyright 2007  Luc Verhaegen <lverhaegen@novell.com>
 * Copyright 2007  Matthias Hopf <mhopf@novell.com>
 * Copyright 2007  Egbert Eich   <eich@novell.com>
 * Copyright 2007  Advanced Micro Devices, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE COPYRIGHT HOLDER(S) OR AUTHOR(S) BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "xf86.h"
#include "xf86_ansic.h"

#include "rhd.h"

#include <signal.h>

void
RhdGetOptValBool(const OptionInfoRec *table, int token,
                 RHDOptPtr optp, Bool def)
{
    if (!(optp->set = xf86GetOptValBool(table, token, &optp->val.bool))){
	optp->set = FALSE;
        optp->val.bool = def;
    } else
	optp->set = TRUE;
}

void
RhdGetOptValInteger(const OptionInfoRec *table, int token,
                 RHDOptPtr optp, int def)
{
    if (!(optp->set = xf86GetOptValInteger(table, token, &optp->val.integer))){
	optp->set = FALSE;
        optp->val.integer = def;
    } else
	optp->set = TRUE;
}

void
RhdGetOptValULong(const OptionInfoRec *table, int token,
                 RHDOptPtr optp, unsigned long def)
{
    if (!(optp->set = xf86GetOptValULong(table, token, &optp->val.uslong))) {
	optp->set = FALSE;
        optp->val.uslong = def;
    } else
	optp->set = TRUE;
}

void
RhdGetOptValReal(const OptionInfoRec *table, int token,
                 RHDOptPtr optp, double def)
{
    if (!(optp->set = xf86GetOptValReal(table, token, &optp->val.real))) {
	optp->set = FALSE;
        optp->val.real = def;
    } else
	optp->set = TRUE;
}

void
RhdGetOptValFreq(const OptionInfoRec *table, int token,
                 OptFreqUnits expectedUnits, RHDOptPtr optp, double def)
{
    if (!(optp->set = xf86GetOptValFreq(table, token, expectedUnits,
                                        &optp->val.freq))) {
	optp->set = FALSE;
        optp->val.freq = def;
    } else
	optp->set = TRUE;
}

void
RhdGetOptValString(const OptionInfoRec *table, int token,
                   RHDOptPtr optp, char *def)
{
    if (!(optp->val.string = xf86GetOptValString(table, token))) {
	optp->set = FALSE;
        optp->val.string = def;
    } else
	optp->set = TRUE;
}


void
RhdDebugDump(int scrnIndex, unsigned char *start, int size)
{
    int i,j;
    char *c = (char *)start;
    char line[256];

    for (j = 0; j <= (size >> 4); j++) {
	char *cur = line;
	char *d = c;
	int k = size < 16 ? size : 16;
	for (i = 0; i < k; i++)
	    cur += xf86snprintf(cur,4,"%2.2x ",(unsigned char) (*(c++)));
	c = d;
	for (i = 0; i < k; i++) {
	    cur += xf86snprintf(cur,2,"%c",((((CARD8)(*c)) > 32)
				&& (((CARD8)(*c)) < 128)) ?
				(unsigned char) (*(c)): '.');
	    c++;
	}
	xf86DrvMsg(scrnIndex,X_INFO,"%s\n",line);
    }
}


/*
 * X should have something like this itself.
 * Also used by the RHDFUNC macro.
 */
void
RHDDebug(int scrnIndex, const char *format, ...)
{
    va_list ap;

    va_start(ap, format);
    xf86VDrvMsgVerb(scrnIndex, X_INFO, LOG_DEBUG, format, ap);
    va_end(ap);
}

void
RHDDebugCont(const char *format, ...)
{
    va_list ap;

    va_start(ap, format);
    xf86VDrvMsgVerb(-1, X_NONE, LOG_DEBUG, format, ap);
    va_end(ap);
}

void
RHDDebugVerb(int scrnIndex, int verb, const char *format, ...)
{
    va_list ap;

    va_start(ap, format);
    xf86VDrvMsgVerb(scrnIndex, X_INFO, LOG_DEBUG + verb, format, ap);
    va_end(ap);
}

void
RHDDebugContVerb(int verb, const char *format, ...)
{
    va_list ap;

    va_start(ap, format);
    xf86VDrvMsgVerb(-1, X_NONE, LOG_DEBUG + verb, format, ap);
    va_end(ap);
}
/*
 * Create a new string where s2 is attached to s1, free s1.
 */
char *
RhdAppendString(char *s1, const char *s2)
{
    if (!s2)
	return s1;
    else if (!s1)
	return xf86strdup(s2);
    else {
	int len = strlen(s1) + strlen(s2) + 1;
	char *result  = (char *)xalloc(len);

	if (!result) return s1;

	xf86strcpy(result,s1);
	xf86strcat(result,s2);
	xfree(s1);
	return result;
    }
}

extern void xf86abort(void) NORETURN;
void RhdAssertFailed(const char *str,
		     const char *file, int line, const char *func)
{
    ErrorF("%s:%d: %s: Assertion '%s' failed.\n", file, line, func, str);

#if 0			/* Set to 1 to get backtraces */
    kill(getpid(), SIGSEGV);
    xf86abort();	/* Not executed, but make gcc happy */
#else
    FatalError("Server aborting\n");
#endif
}

