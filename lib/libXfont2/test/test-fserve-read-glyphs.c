/*
 * Security regression tests for fs_read_glyphs() in src/fc/fserve.c.
 *
 * Approach: include fserve.c directly to access the static fs_read_glyphs()
 * function.  Pre-fill the FSFpeRec.inBuf with a crafted protocol reply so
 * fs_get_reply() returns it without any network I/O.
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

/*
 * Include fserve.c directly to access the static fs_read_glyphs().
 * All non-static symbols from fserve.c are hidden in libXfont2.so
 * (via the linker version script), so there are no duplicate symbol
 * conflicts when linking against the library.
 */
#include "src/fc/fserve.c"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/*
 * Set up an FSFpeRec with its inBuf pre-filled with the given data.
 * fs_get_reply() will return this data without attempting any network I/O
 * because fs_inqueued(conn) >= size.
 */
static void
setup_conn(FSFpeRec *conn, const void *reply_data, long reply_size)
{
    memset(conn, 0, sizeof(*conn));

    /*
     * fs_get_reply() checks:  conn->fs_fd != -1 && conn->fs_listening
     * Use a dup'd fd so it's valid but harmless.
     */
    conn->fs_fd = dup(STDERR_FILENO);
    conn->fs_listening = TRUE;

    /* Pre-fill the input buffer with our crafted reply */
    conn->inBuf.buf = malloc(reply_size);
    if (!conn->inBuf.buf) {
        fprintf(stderr, "FAIL: malloc for inBuf\n");
        exit(1);
    }
    memcpy(conn->inBuf.buf, reply_data, reply_size);
    conn->inBuf.size = reply_size;
    conn->inBuf.insert = reply_size;
    conn->inBuf.remove = 0;
    conn->inNeed = 0;

    /* Allocate a minimal output buffer to keep _fs_flush happy */
    conn->outBuf.buf = calloc(1, FS_BUF_INC);
    conn->outBuf.size = FS_BUF_INC;
    conn->outBuf.insert = 0;
    conn->outBuf.remove = 0;
}

static void
cleanup_conn(FSFpeRec *conn)
{
    if (conn->fs_fd >= 0)
        close(conn->fs_fd);
    free(conn->inBuf.buf);
    free(conn->outBuf.buf);
}

/*
 * Set up the minimum font state needed by fs_read_glyphs():
 *   - FontPathElementRec (fpe) with fpe->private = conn
 *   - FontRec (pfont) with info, fontPrivate, fpePrivate
 *   - FSFontRec (fsfont) with encoding[] array
 *   - FSFontDataRec (fsd)
 *   - FSBlockDataRec (blockrec) of type FS_OPEN_FONT
 *   - FSBlockedFontRec (bfont) embedded in blockrec->data
 */
struct test_font_state {
    FontPathElementRec fpe;
    FontRec pfont;
    FSFontRec fsfont;
    FSFontDataRec fsd;
    FSBlockDataRec blockrec;
    FSBlockedFontRec bfont;
    CharInfoPtr encoding;
};

static void
setup_font_state(struct test_font_state *s, FSFpeRec *conn,
                 int num_encoding)
{
    int i;

    memset(s, 0, sizeof(*s));

    /* Font path element */
    s->fpe.name = (char *)"test-fserve";
    s->fpe.name_length = strlen(s->fpe.name);
    s->fpe.private = conn;

    /* Font data (fpePrivate) */
    s->fsd.name = (char *)"test-font";
    s->fsd.namelen = strlen(s->fsd.name);
    s->fsd.glyphs_to_get = 0;

    /* Encoding array -- this is what num_extents sized */
    s->encoding = calloc(num_encoding, sizeof(CharInfoRec));
    if (!s->encoding) {
        fprintf(stderr, "FAIL: calloc encoding\n");
        exit(1);
    }
    /* Mark all glyphs as having nonzero metrics and undefined bits
     * so fs_read_glyphs will try to process them */
    for (i = 0; i < num_encoding; i++) {
        s->encoding[i].metrics.ascent = 10;
        s->encoding[i].metrics.descent = 2;
        s->encoding[i].metrics.characterWidth = 8;
        s->encoding[i].metrics.leftSideBearing = 0;
        s->encoding[i].metrics.rightSideBearing = 8;
        s->encoding[i].bits = &_fs_glyph_undefined;
    }

    /* FSFontRec */
    s->fsfont.encoding = s->encoding;
    s->fsfont.num_encoding = num_encoding;
    s->fsfont.pDefault = NULL;
    s->fsfont.inkMetrics = s->encoding;
    s->fsfont.glyphs = NULL;

    /* FontRec */
    s->pfont.fontPrivate = &s->fsfont;
    s->pfont.fpePrivate = &s->fsd;
    s->pfont.fpe = &s->fpe;
    s->pfont.info.firstRow = 0;
    s->pfont.info.lastRow = 0;
    s->pfont.info.firstCol = 0;
    s->pfont.info.lastCol = num_encoding > 0 ? num_encoding - 1 : 0;
    s->pfont.info.maxbounds.ascent = 20;
    s->pfont.info.maxbounds.descent = 10;
    s->pfont.info.maxbounds.characterWidth = 20;

    /* Block record -- simulating FS_OPEN_FONT path */
    s->bfont.pfont = &s->pfont;
    s->bfont.flags = FontLoadBitmaps;
    s->bfont.state = FS_GLYPHS_REPLY;
    s->bfont.freeFont = FALSE;

    s->blockrec.type = FS_OPEN_FONT;
    s->blockrec.data = (pointer)&s->bfont;
    s->blockrec.client = NULL;
    s->blockrec.sequenceNumber = 0;
    s->blockrec.errcode = 0;
    s->blockrec.depending = NULL;
    s->blockrec.next = NULL;
}

static void
cleanup_font_state(struct test_font_state *s)
{
    FSGlyphPtr g, next;

    /* Free any glyph allocations made by fs_alloc_glyphs */
    for (g = s->fsfont.glyphs; g; g = next) {
        next = g->next;
        free(g);
    }
    free(s->encoding);
}

/*
 * Build a crafted fsQueryXBitmaps16Reply in a buffer.
 * Returns the total buffer size.  Caller must free *out_buf.
 *
 * The reply contains:
 *   - fsQueryXBitmaps16Reply header
 *   - num_chars fsOffset32 entries
 *   - nbytes of bitmap data
 */
static long
build_reply(char **out_buf,
            CARD32 num_chars, CARD32 nbytes,
            CARD32 off_position, CARD32 off_length)
{
    long hdr_size = SIZEOF(fsQueryXBitmaps16Reply);
    long offsets_size = SIZEOF(fsOffset32) * num_chars;
    /* Bitmap data area: at least nbytes, but we need off_position + off_length
     * to be valid source, so ensure bitmap area is large enough */
    long bitmap_size = nbytes;
    long total = hdr_size + offsets_size + bitmap_size;
    long total_padded = (total + 3) & ~3;  /* pad to 4 bytes */
    char *buf;
    fsQueryXBitmaps16Reply *rep;
    fsOffset32 off;
    long i;

    buf = calloc(1, total_padded);
    if (!buf) {
        fprintf(stderr, "FAIL: calloc reply buffer\n");
        exit(1);
    }

    /* Fill header */
    rep = (fsQueryXBitmaps16Reply *)buf;
    rep->type = FS_Reply;    /* normal reply (0), not FS_Error (1) */
    rep->sequenceNumber = 0;
    rep->length = total_padded >> 2;   /* length in 32-bit words */
    rep->replies_hint = 0;
    rep->num_chars = num_chars;
    rep->nbytes = nbytes;

    /* Fill offset entries -- all pointing to the same source range */
    off.position = off_position;
    off.length = off_length;
    for (i = 0; i < (long)num_chars; i++) {
        memcpy(buf + hdr_size + i * SIZEOF(fsOffset32),
               &off, SIZEOF(fsOffset32));
    }

    /* Fill bitmap data with recognizable pattern */
    memset(buf + hdr_size + offsets_size, 0xAA, bitmap_size);

    *out_buf = buf;
    return total_padded;
}

/*
 * Test 1: num_chars > num_encoding
 *
 * Allocate encoding[] with 2 entries, but send a reply with
 * num_chars = 100.  Without the fix, this would read/write
 * encoding[2..99] out of bounds.
 */
static int
test_num_chars_exceeds_encoding(void)
{
    FSFpeRec conn;
    struct test_font_state state;
    char *reply_buf;
    long reply_size;
    int result;
    int num_encoding = 2;
    CARD32 num_chars = 100;
    CARD32 nbytes = num_chars * 16;  /* enough bitmap data */

    /* Build a reply with num_chars=100 but valid source data */
    reply_size = build_reply(&reply_buf, num_chars, nbytes, 0, 16);
    setup_conn(&conn, reply_buf, reply_size);
    setup_font_state(&state, &conn, num_encoding);

    result = fs_read_glyphs(&state.fpe, &state.blockrec);

    cleanup_font_state(&state);
    cleanup_conn(&conn);
    free(reply_buf);

    if (result != Successful) {
        printf("ok 1 - num_chars (%u) > num_encoding (%d) rejected\n",
               (unsigned)num_chars, num_encoding);
        return 0;
    } else {
        printf("not ok 1 - num_chars (%u) > num_encoding (%d) "
               "should have been rejected\n",
               (unsigned)num_chars, num_encoding);
        return 1;
    }
}

/*
 * Test 2: cumulative glyph data overflow
 *
 * Allocate allbits with nbytes=64, but send 100 glyphs each
 * with offset {position:0, length:64}.  Each individual source
 * range is valid, but the cumulative writes total 6400 bytes
 * into a 64-byte buffer.
 */
static int
test_cumulative_allbits_overflow(void)
{
    FSFpeRec conn;
    struct test_font_state state;
    char *reply_buf;
    long reply_size;
    int result;
    int num_encoding = 100;  /* match num_chars so encoding[] is fine */
    CARD32 num_chars = 100;
    CARD32 nbytes = 64;      /* tiny destination buffer */

    /* All offsets point to {position:0, length:64} -- each source
     * range is valid but they overlap, causing 100*64=6400 bytes
     * to be written to a 64-byte buffer */
    reply_size = build_reply(&reply_buf, num_chars, nbytes, 0, 64);
    setup_conn(&conn, reply_buf, reply_size);
    setup_font_state(&state, &conn, num_encoding);

    result = fs_read_glyphs(&state.fpe, &state.blockrec);

    cleanup_font_state(&state);
    cleanup_conn(&conn);
    free(reply_buf);

    if (result != Successful) {
        printf("ok 2 - cumulative allbits overflow (100 * 64 into 64) rejected\n");
        return 0;
    } else {
        printf("not ok 2 - cumulative allbits overflow (100 * 64 into 64) "
               "should have been rejected\n");
        return 1;
    }
}

/*
 * Test 3: legitimate reply should still be accepted
 *
 * num_chars == num_encoding, each glyph has unique non-overlapping
 * offsets, and total data fits in nbytes.
 */
static int
test_legitimate_reply(void)
{
    FSFpeRec conn;
    struct test_font_state state;
    char *reply_buf;
    long hdr_size = SIZEOF(fsQueryXBitmaps16Reply);
    long offsets_size;
    int result;
    int num_encoding = 4;
    CARD32 num_chars = 4;
    CARD32 glyph_size = 16;
    CARD32 nbytes = num_chars * glyph_size;
    long total, total_padded;
    fsQueryXBitmaps16Reply *rep;
    fsOffset32 off;
    int i;

    offsets_size = SIZEOF(fsOffset32) * num_chars;
    total = hdr_size + offsets_size + nbytes;
    total_padded = (total + 3) & ~3;

    reply_buf = calloc(1, total_padded);
    if (!reply_buf) {
        fprintf(stderr, "FAIL: calloc\n");
        return 1;
    }

    rep = (fsQueryXBitmaps16Reply *)reply_buf;
    rep->type = FS_Reply;
    rep->sequenceNumber = 0;
    rep->length = total_padded >> 2;
    rep->replies_hint = 0;
    rep->num_chars = num_chars;
    rep->nbytes = nbytes;

    /* Each glyph gets its own non-overlapping slice */
    for (i = 0; i < (int)num_chars; i++) {
        off.position = i * glyph_size;
        off.length = glyph_size;
        memcpy(reply_buf + hdr_size + i * SIZEOF(fsOffset32),
               &off, SIZEOF(fsOffset32));
    }
    memset(reply_buf + hdr_size + offsets_size, 0xBB, nbytes);

    setup_conn(&conn, reply_buf, total_padded);
    setup_font_state(&state, &conn, num_encoding);

    result = fs_read_glyphs(&state.fpe, &state.blockrec);

    cleanup_font_state(&state);
    cleanup_conn(&conn);
    free(reply_buf);

    if (result == Successful) {
        printf("ok 3 - legitimate reply (4 glyphs, non-overlapping) accepted\n");
        return 0;
    } else {
        printf("not ok 3 - legitimate reply (4 glyphs, non-overlapping) "
               "rejected with error %d\n", result);
        return 1;
    }
}

int
main(int argc, char **argv)
{
    int failures = 0;

    printf("1..3\n");

    failures += test_num_chars_exceeds_encoding();
    failures += test_cumulative_allbits_overflow();
    failures += test_legitimate_reply();

    return failures ? 1 : 0;
}
