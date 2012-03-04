/*
 * Copyright (c) 2011, Oracle and/or its affiliates. All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

/*
  Simple test case for FSGetErrorText.
  When run with arguments, prints messages for the given codes.
  When run with no arguments, prints messages for known valid error codes
  and some invalid codes.
*/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <X11/fonts/FSlib.h>

static int
CheckErrorMessage(FSServer *srv, int code, const char *codestr)
{
    char buf[128] = "";

    if (!FSGetErrorText(srv, code, buf, sizeof(buf))) {
	fprintf(stderr, "FSGetErrorText(srv, %s (%d), buf, %d) failed",
		codestr, code, (int) sizeof(buf));
	return 0;
    }

    printf("FSGetErrorText for code %s (%d) returned:\n|%s|\n\n",
	   codestr, code, buf);
    return 1;
}

#define CheckNamedErrorMessage(s, e) \
    CheckErrorMessage(s, e, #e)

int main(int argc, char **argv)
{
    FSServer *srv = FSOpenServer(NULL);

    if (srv == NULL) {
	fprintf(stderr, "Failed to open fontserver connection to: %s\n"
	"Aborting test. Make sure FONTSERVER is set to a valid xfs server.\n",
		FSServerName(NULL));
	exit(1);
    }

    if (argc > 1) {
	int i;

	for (i = 1; i < argc; i++) {
	    int c;
	    errno = 0;
	    c = strtol(argv[i], NULL, 0);
	    if (errno != 0) {
		perror(argv[i]);
		exit(1);
	    }
	    CheckErrorMessage (srv, c, "");
	}
	exit (0);
    }

    /* Default list to check if no arguments specified */
    CheckNamedErrorMessage(srv, FSSuccess);
    CheckNamedErrorMessage(srv, FSBadRequest);
    CheckNamedErrorMessage(srv, FSBadFormat);
    CheckNamedErrorMessage(srv, FSBadFont);
    CheckNamedErrorMessage(srv, FSBadRange);
    CheckNamedErrorMessage(srv, FSBadEventMask);
    CheckNamedErrorMessage(srv, FSBadAccessContext);
    CheckNamedErrorMessage(srv, FSBadIDChoice);
    CheckNamedErrorMessage(srv, FSBadName);
    CheckNamedErrorMessage(srv, FSBadResolution);
    CheckNamedErrorMessage(srv, FSBadAlloc);
    CheckNamedErrorMessage(srv, FSBadLength);
    CheckNamedErrorMessage(srv, FSBadImplementation);
    CheckErrorMessage (srv, 12, "<out of range value>");
    CheckErrorMessage (srv, 256, "<out of range value>");
    CheckErrorMessage (srv, 0xffff, "<out of range value>");

    exit(0);
}
