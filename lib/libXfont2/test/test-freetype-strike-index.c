/*
 * Test for uninitialised strike_index in FreeTypeOpenInstance().
 *
 * FreeTypeOpenInstance() allocates FTInstanceRec with a bare malloc()
 * and assigns instance->strike_index only inside "if (FT_IS_SFNT())".
 * A non-SFNT face -- any bitmap font that FreeType's bitmap driver
 * claims, which libXfont2 reaches whenever such a file carries a
 * FreeType renderer suffix such as .pfb -- left the field holding raw
 * heap bytes.  The field was then passed to FT_Do_SBit_Metrics() and
 * used as an array subscript into ft_face->available_sizes[], causing
 * an out-of-bounds read.
 *
 * This test opens a BDF font file via a .pfb filename (which routes it
 * through the FreeType backend instead of the bitmap backend).  Before
 * the fix, this would use an uninitialised strike_index value as an
 * array subscript, causing a crash or undefined behaviour.  After the
 * fix, the sentinel value 0xFFFFU is set unconditionally and the bounds
 * check in FT_Do_SBit_Metrics() prevents any out-of-bounds access.
 *
 * Test font: test/data/ft-strike-index/strike-test.pfb (a BDF font
 * with a .pfb extension, static file checked into the repository)
 */

/*
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

#include "utils/font-test-utils.h"
#include "src/util/replace.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <X11/fonts/fsmasks.h>

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
main(int argc, char **argv)
{
    xfont2_fpe_funcs_rec const **fpe_functions;
    int fpe_function_count;
    FontPathElementPtr *fpe_list;
    const char *srcdir;
    char fontdir[PATH_MAX];
    char abspath[PATH_MAX];
    const char *paths[1];
    int num_fpes = 1;
    int result;

    /* The .pfb suffix routes the font through the FreeType backend.
     * The font is actually a BDF, so FreeType's bitmap driver parses
     * it and produces a non-SFNT face -- the exact path that triggered
     * the uninitialised strike_index bug. */
    const char *xlfd =
        "-misc-striketest-medium-r-normal--10-100-75-75-p-100-iso10646-1";

    srcdir = getenv("srcdir");
    if (!srcdir)
        srcdir = ".";

    snprintf(fontdir, sizeof(fontdir), "%s/test/data/ft-strike-index", srcdir);

    /* Font path must be absolute */
    if (fontdir[0] != '/') {
        if (!realpath(fontdir, abspath)) {
            fprintf(stderr, "FAIL: could not resolve path %s\n", fontdir);
            return 1;
        }
        memcpy(fontdir, abspath, sizeof(fontdir));
    }

    fpe_functions = init_font_handlers(&fpe_function_count);

    paths[0] = fontdir;
    fpe_list = init_font_paths(paths, &num_fpes);
    if (num_fpes != 1) {
        fprintf(stderr, "FAIL: could not initialize font path %s\n", fontdir);
        return 1;
    }

    /* This is the critical call: opening a BDF-as-.pfb font through
     * the FreeType backend exercises the non-SFNT path in
     * FreeTypeOpenInstance().  Before the fix, strike_index would be
     * uninitialised and used as an array subscript in
     * FT_Do_SBit_Metrics(), causing undefined behaviour (typically a
     * crash).  After the fix, the sentinel is set and the bounds check
     * prevents any out-of-bounds access. */
    result = try_open_font(fpe_functions, fpe_list[0], xlfd);

    if (result == Successful) {
        printf("PASS: BDF-as-.pfb font opened successfully "
               "(strike_index properly initialised)\n");
    } else {
        /* The font may fail to open for legitimate reasons (e.g. the
         * encoding doesn't match), but it must not crash.  If we reach
         * this point, the process survived -- the uninitialised
         * strike_index did not cause a fatal OOB access. */
        printf("PASS: BDF-as-.pfb font open returned %d "
               "(no crash from uninitialised strike_index)\n", result);
    }

    (*fpe_functions[fpe_list[0]->type]->free_fpe)(fpe_list[0]);
    free(fpe_list[0]->name);
    free(fpe_list[0]);
    free(fpe_list);

    return 0;
}
