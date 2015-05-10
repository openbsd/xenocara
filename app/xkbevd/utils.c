
  /*\
   *
   *                          COPYRIGHT 1990
   *                    DIGITAL EQUIPMENT CORPORATION
   *                       MAYNARD, MASSACHUSETTS
   *                        ALL RIGHTS RESERVED.
   *
   * THE INFORMATION IN THIS SOFTWARE IS SUBJECT TO CHANGE WITHOUT NOTICE AND
   * SHOULD NOT BE CONSTRUED AS A COMMITMENT BY DIGITAL EQUIPMENT CORPORATION.
   * DIGITAL MAKES NO REPRESENTATIONS ABOUT THE SUITABILITY OF THIS SOFTWARE
   * FOR ANY PURPOSE.  IT IS SUPPLIED "AS IS" WITHOUT EXPRESS OR IMPLIED
   * WARRANTY.
   *
   * IF THE SOFTWARE IS MODIFIED IN A MANNER CREATING DERIVATIVE COPYRIGHT
   * RIGHTS, APPROPRIATE LEGENDS MAY BE PLACED ON THE DERIVATIVE WORK IN
   * ADDITION TO THAT SET FORTH ABOVE.
   *
   * Permission to use, copy, modify, and distribute this software and its
   * documentation for any purpose and without fee is hereby granted, provided
   * that the above copyright notice appear in all copies and that both that
   * copyright notice and this permission notice appear in supporting
   * documentation, and that the name of Digital Equipment Corporation not be
   * used in advertising or publicity pertaining to distribution of the
   * software without specific, written prior permission.
  \*/

#include 	"utils.h"
#include	<ctype.h>
#include	<stdlib.h>

/***====================================================================***/

static FILE *errorFile = NULL;

Boolean
uSetErrorFile(const char *name)
{
    if ((errorFile != NULL) && (errorFile != stderr)) {
        fprintf(errorFile, "switching to %s\n", name ? name : "stderr");
        fclose(errorFile);
    }
    if (name != NullString)
        errorFile = fopen(name, "w");
    else
        errorFile = stderr;
    if (errorFile == NULL) {
        errorFile = stderr;
        return (False);
    }
    return (True);
}

void
uInformation(const char *s, ...)
{
    va_list ap;

    va_start(ap, s);
    vfprintf(errorFile, s, ap);
    fflush(errorFile);
    va_end(ap);
    return;
}

/***====================================================================***/

void
uAction(const char *s, ...)
{
    va_list ap;

    va_start(ap, s);
    fprintf(errorFile, "                  ");
    vfprintf(errorFile, s, ap);
    fflush(errorFile);
    va_end(ap);
    return;
}

/***====================================================================***/

void
uWarning(const char *s, ...)
{
    va_list ap;

    va_start(ap, s);
    fprintf(errorFile, "Warning:          ");
    vfprintf(errorFile, s, ap);
    fflush(errorFile);
    va_end(ap);
    return;
}

/***====================================================================***/

void
uError(const char *s, ...)
{
    va_list ap;

    va_start(ap, s);
    fprintf(errorFile, "Error:            ");
    vfprintf(errorFile, s, ap);
    fflush(errorFile);
    va_end(ap);
    return;
}

/***====================================================================***/

void
uInternalError(const char *s, ...)
{
    va_list ap;

    va_start(ap, s);
    fprintf(errorFile, "Internal error:   ");
    vfprintf(errorFile, s, ap);
    fflush(errorFile);
    va_end(ap);
    return;
}

/***====================================================================***/

#ifndef HAVE_STRCASECMP
int
uStrCaseCmp(const char *str1, const char *str2)
{
    char buf1[512], buf2[512];

    char c, *s;

    register int n;

    for (n = 0, s = buf1; (c = *str1++); n++) {
        if (isupper(c))
            c = tolower(c);
        if (n > 510)
            break;
        *s++ = c;
    }
    *s = '\0';
    for (n = 0, s = buf2; (c = *str2++); n++) {
        if (isupper(c))
            c = tolower(c);
        if (n > 510)
            break;
        *s++ = c;
    }
    *s = '\0';
    return (strcmp(buf1, buf2));
}
#endif
