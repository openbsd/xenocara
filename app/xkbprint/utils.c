  /*\
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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include 	"utils.h"
#include	<ctype.h>
#include	<stdlib.h>

unsigned int debugFlags;


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
uFatalError(const char *s, ...)
{
    va_list ap;

    va_start(ap, s);
    fprintf(errorFile, "Fatal Error:      ");
    vfprintf(errorFile, s, ap);
    fprintf(errorFile, "                  Exiting\n");
    fflush(errorFile);
    va_end(ap);
    exit(1);
    /* NOTREACHED */
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

#ifndef HAVE_ASPRINTF
int
uAsprintf(char ** ret, const char *format, ...)
{
    char buf[256];
    int len;
    va_list ap;

    va_start(ap, format);
    len = vsnprintf(buf, sizeof(buf), format, ap);
    va_end(ap);

    if (len < 0)
        return -1;

    if (len < sizeof(buf))
    {
        *ret = strdup(buf);
    }
    else
    {
        *ret = malloc(len + 1); /* snprintf doesn't count trailing '\0' */
        if (*ret != NULL)
        {
            va_start(ap, format);
            len = vsnprintf(*ret, len + 1, format, ap);
            va_end(ap);
            if (len < 0) {
                free(*ret);
                *ret = NULL;
            }
        }
    }

    if (*ret == NULL)
        return -1;

    return len;
}
#endif /* HAVE_ASPRINTF */
