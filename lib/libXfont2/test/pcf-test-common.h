/*
 * Common helpers for PCF security regression tests.
 *
 * Copyright (c) 2026, Red Hat, Inc.
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

#ifndef PCF_TEST_COMMON_H
#define PCF_TEST_COMMON_H

#include "utils/font-test-utils.h"

/*
 * Initialize libXfont2 and return the FPE function table.
 * Stores result in *fpe_function_count.
 */
xfont2_fpe_funcs_rec const **
pcf_test_init(int *fpe_function_count);

/*
 * Load a pre-built font directory, attempt to open the given XLFD,
 * and verify no crash occurs.
 *
 * Returns 0 on success (no crash), 1 on failure.
 */
int
pcf_test_open_font(xfont2_fpe_funcs_rec const **fpe_functions,
                   const char *test_name,
                   const char *font_dir,
                   const char *xlfd);

#endif /* PCF_TEST_COMMON_H */
