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

#include "pcf-test-common.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <X11/fonts/fsmasks.h>
#include "src/util/replace.h"

xfont2_fpe_funcs_rec const **
pcf_test_init(int *fpe_function_count)
{
    return init_font_handlers(fpe_function_count);
}

/*
 * Try to open a font by XLFD name through the font path element.
 * Returns the result code from open_font (Successful or error).
 * If the font opens successfully, it is closed again.
 *
 * format/fmask request glyph pad 4 (typical x86_64 server) to trigger
 * the RepadBitmap path for PCF fonts stored at different padding.
 */
static int
try_open_font(xfont2_fpe_funcs_rec const **fpe_functions,
              FontPathElementPtr fpe, const char *xlfd)
{
    FontPtr pFont = NULL;
    char *aliasName = NULL;
    int result;

    fsBitmapFormat format = BitmapFormatScanlinePad32 |
                            BitmapFormatBitOrderLSB |
                            BitmapFormatByteOrderLSB |
                            BitmapFormatImageRectMin;
    fsBitmapFormatMask fmask = BitmapFormatMaskScanLinePad |
                               BitmapFormatMaskBit |
                               BitmapFormatMaskByte |
                               BitmapFormatMaskImageRectangle;

    result = (*fpe_functions[fpe->type]->open_font)
        (NULL, fpe, 0, xlfd, strlen(xlfd),
         format, fmask, (XID)1, &pFont, &aliasName, NULL);

    if (result == Successful && pFont) {
        (*fpe_functions[fpe->type]->close_font)(fpe, pFont);
    }

    return result;
}

int
pcf_test_open_font(xfont2_fpe_funcs_rec const **fpe_functions,
                   const char *test_name,
                   const char *font_dir,
                   const char *xlfd)
{
    FontPathElementPtr *fpe_list;
    const char *paths[1];
    int num_fpes = 1;
    int result;
    char abspath[PATH_MAX];

    /* Font path must be absolute; relative paths are misidentified as
     * font server addresses by the transport layer. */
    if (font_dir[0] != '/') {
        if (!realpath(font_dir, abspath)) {
            fprintf(stderr, "FAIL %s: could not resolve path %s\n",
                    test_name, font_dir);
            return 1;
        }
        font_dir = abspath;
    }

    paths[0] = font_dir;
    fpe_list = init_font_paths(paths, &num_fpes);
    if (num_fpes != 1) {
        fprintf(stderr, "FAIL %s: could not initialize font path %s\n",
                test_name, font_dir);
        return 1;
    }

    result = try_open_font(fpe_functions, fpe_list[0], xlfd);

    if (result != Successful) {
        printf("PASS %s: malicious font rejected (result=%d)\n",
               test_name, result);
    } else {
        printf("PASS %s: font loaded without crash (result=%d)\n",
               test_name, result);
    }

    (*fpe_functions[fpe_list[0]->type]->free_fpe)(fpe_list[0]);
    free(fpe_list[0]->name);
    free(fpe_list[0]);
    free(fpe_list);

    return 0;
}
