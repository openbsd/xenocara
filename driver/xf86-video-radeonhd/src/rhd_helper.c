/*
 * Copyright 2007  Luc Verhaegen <libv@exsuse.de>
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

#define SEGV_ON_ASSERT 1		/* Define to 1 if you want backtraces on ASSERT() */

#include "xf86.h"

#if HAVE_XF86_ANSIC_H
# include "xf86_ansic.h"
#else
# include <sys/types.h>
# include <unistd.h>
# include <string.h>
# include <stdio.h>
# include <stdlib.h>
#endif

#if SEGV_ON_ASSERT
# include <signal.h>
#endif

#include "rhd.h"

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

/*
 *
 */
enum rhdOptStatus
RhdParseBooleanOption(struct RHDOpt *Option, char *Name)
{
    unsigned int i;
    char* c;
    char* str = strdup(Name);

    const char* off[] = {
	"false",
	"off",
	"no",
	"0"
    };
    const char* on[] = {
	"true",
	"on",
	"yes",
	"1"
    };

    /* first fixup the name to match the randr names */
    for (c = str; *c; c++)
	if (isspace(*c))
	    *c='_';

    if (Option->set) {
	char *ptr = Option->val.string;
	while (*ptr != '\0') {
	    while (isspace(*ptr))
		ptr++;
	    if (*ptr == '\0')
		break;

	    if (!strncasecmp(str,ptr,strlen(str)) || !strncasecmp("all",ptr,3)) {
		if(!strncasecmp("all",ptr,3))
		    ptr += 3;
		else
		    ptr += strlen(str);
		xfree(str);

		if (isspace(*ptr) || *ptr == '=') {
		    ptr++;
		}

		for(i=0; i<sizeof(on)/sizeof(char*); i++)
		    if (!strncasecmp(on[i],ptr,strlen(on[i])))
			return RHD_OPTION_OFF;

		for(i=0; i<sizeof(off)/sizeof(char*); i++)
		    if (!strncasecmp(off[i],ptr,strlen(off[i])))
			return RHD_OPTION_ON;

		return RHD_OPTION_DEFAULT;
	    } else
		while (*ptr != '\0' && !isspace(*ptr))
		    ptr++;
	}
    }
    xfree(str);
    return RHD_OPTION_NOT_SET;
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
	    cur += snprintf(cur,4,"%2.2x ",(unsigned char) (*(c++)));
	c = d;
	for (i = 0; i < k; i++) {
	    cur += snprintf(cur,2,"%c",((((CARD8)(*c)) > 32)
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
	return xstrdup(s2);
    else {
	int len = strlen(s1) + strlen(s2) + 1;
	char *result  = (char *)xalloc(len);

	if (!result) return s1;

	strcpy(result,s1);
	strcat(result,s2);
	xfree(s1);
	return result;
    }
}

void RhdAssertFailed(const char *str,
		     const char *file, int line, const char *func)
{
    ErrorF("%s:%d: %s: Assertion '%s' failed.\n", file, line, func, str);

#if SEGV_ON_ASSERT
    kill(getpid(), SIGSEGV);
#endif
    FatalError ("Server aborting\n");
}

void RhdAssertFailedFormat(const char *str,
			   const char *file, int line, const char *func,
			   const char *format, ...)
{
    va_list args;
    ErrorF("%s:%d: %s: Assertion '%s' failed.\n  ", file, line, func, str);
    va_start(args, format);
    VErrorF(format, args);
    va_end(args);
    ErrorF("\n");

#if SEGV_ON_ASSERT
    kill(getpid(), SIGSEGV);
#endif
    FatalError ("Server aborting\n");
}
