/*
 * vim:ts=4:sw=4:expandtab
 *
 * Copyright © 2016 Ingo Bürk
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
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the names of the authors or their
 * institutions shall not be used in advertising or otherwise to promote the
 * sale, use or other dealings in this Software without prior written
 * authorization from the authors.
 *
 */
#ifndef __UTIL_H__
#define __UTIL_H__

#include "externals.h"

#define FREE(p)   \
    do {          \
        free(p);  \
        p = NULL; \
    } while (0)

#undef MAX
#define MAX(x, y) ((x) > (y) ? (x) : (y))
#undef MIN
#define MIN(x, y) ((x) < (y) ? (x) : (y))

#define SUCCESS 0
#define FAILURE 1

int str2long(long *out, const char *input, const int base);

char *get_home_dir_file(const char *filename);

char *resolve_path(const char *path, const char *base);

char *file_get_contents(const char *filename);

char *xcb_util_get_property(xcb_connection_t *conn, xcb_window_t window, xcb_atom_t atom,
        xcb_atom_t type, size_t size);

#endif /* __UTIL_H__ */
