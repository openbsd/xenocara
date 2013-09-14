/* Copyright (c) 2011-2012 Apple Inc.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation files
 * (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT.  IN NO EVENT SHALL THE ABOVE LISTED COPYRIGHT
 * HOLDER(S) BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the name(s) of the above
 * copyright holders shall not be used in advertising or otherwise to
 * promote the sale, use or other dealings in this Software without
 * prior written authorization.
 */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <asl.h>
#include <unistd.h>
#include <stdio.h>
#include <assert.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#include <spawn.h>

#include "console_redirect.h"

int main(int argc, char **argv, char **envp) {
    aslclient aslc;
    pid_t child;
    int pstat;

    if(argc < 2 || strcmp(argv[1], "--help") == 0) {
        fprintf(stderr, "Usage: %s prog [args...]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    aslc = asl_open(BUNDLE_ID_PREFIX".startx", BUNDLE_ID_PREFIX, ASL_OPT_NO_DELAY);

    xi_asl_capture_fd(aslc, NULL, ASL_LEVEL_INFO, STDOUT_FILENO);
    xi_asl_capture_fd(aslc, NULL, ASL_LEVEL_NOTICE, STDERR_FILENO);

    assert(posix_spawnp(&child, argv[1], NULL, NULL, &argv[1], envp) == 0);
    wait4(child, &pstat, 0, (struct rusage *)0);

    return pstat;
}
