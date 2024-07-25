/* $XTermId: ptydata.c,v 1.160 2024/05/10 22:54:17 tom Exp $ */

/*
 * Copyright 1999-2023,2024 by Thomas E. Dickey
 *
 *                         All Rights Reserved
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE ABOVE LISTED COPYRIGHT HOLDER(S) BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the name(s) of the above copyright
 * holders shall not be used in advertising or otherwise to promote the
 * sale, use or other dealings in this Software without prior written
 * authorization.
 */

#include <data.h>

#if OPT_WIDE_CHARS
#include <menu.h>
#include <wcwidth.h>
#endif

#ifdef TEST_DRIVER
#undef TRACE
#define TRACE(p) if (1) printf p
#undef TRACE2
#define TRACE2(p) if (0) printf p
#define visibleChars(buf, len) "buffer"
#endif

/*
 * Check for both EAGAIN and EWOULDBLOCK, because some supposedly POSIX
 * systems are broken and return EWOULDBLOCK when they should return EAGAIN.
 * Note that this macro may evaluate its argument more than once.
 */
#if defined(EAGAIN) && defined(EWOULDBLOCK)
#define E_TEST(err) ((err) == EAGAIN || (err) == EWOULDBLOCK)
#else
#ifdef EAGAIN
#define E_TEST(err) ((err) == EAGAIN)
#else
#define E_TEST(err) ((err) == EWOULDBLOCK)
#endif
#endif

#if OPT_WIDE_CHARS
/*
 * Convert the 8-bit codes in data->buffer[] into Unicode in data->utf_data.
 * The number of bytes converted will be nonzero iff there is data.
 */
Bool
decodeUtf8(TScreen *screen, PtyData *data)
{
    size_t i;
    size_t length = (size_t) (data->last - data->next);
    int utf_count = 0;
    unsigned utf_char = 0;

    data->utf_size = 0;
    for (i = 0; i < length; i++) {
	unsigned c = data->next[i];

	/* Combine UTF-8 into Unicode */
	if (c < 0x80) {
	    /* We received an ASCII character */
	    if (utf_count > 0) {
		data->utf_data = UCS_REPL;	/* prev. sequence incomplete */
		data->utf_size = i;
	    } else {
		data->utf_data = (IChar) c;
		data->utf_size = 1;
	    }
	    break;
	} else if (screen->vt100_graphics
		   && (c < 0x100)
		   && (utf_count == 0)
		   && screen->gsets[(int) screen->curgr] != nrc_ASCII) {
	    data->utf_data = (IChar) c;
	    data->utf_size = 1;
	    break;
	} else if (c < 0xc0) {
	    /* We received a continuation byte */
	    if (utf_count < 1) {
		if (screen->c1_printable) {
		    data->utf_data = (IChar) c;
		} else if ((i + 1) < length
			   && data->next[i + 1] > 0x20
			   && data->next[i + 1] < 0x80) {
		    /*
		     * Allow for C1 control string if the next byte is
		     * available for inspection.
		     */
		    data->utf_data = (IChar) c;
		} else {
		    /*
		     * We received a continuation byte before receiving a
		     * sequence state, or a failed attempt to use a C1 control
		     * string.
		     */
		    data->utf_data = (IChar) UCS_REPL;
		}
		data->utf_size = (i + 1);
		break;
	    } else if (screen->utf8_weblike
		       && (utf_count == 3
			   && utf_char == 0x04
			   && c >= 0x90)) {
		/* The encoding would form a code point beyond U+10FFFF. */
		data->utf_size = i;
		data->utf_data = UCS_REPL;
		break;
	    } else if (screen->utf8_weblike
		       && (utf_count == 2
			   && utf_char == 0x0d
			   && c >= 0xa0)) {
		/* The encoding would form a surrogate code point. */
		data->utf_size = i;
		data->utf_data = UCS_REPL;
		break;
	    } else {
		/* Check for overlong UTF-8 sequences for which a shorter
		 * encoding would exist and replace them with UCS_REPL.
		 * An overlong UTF-8 sequence can have any of the following
		 * forms:
		 *   1100000x 10xxxxxx
		 *   11100000 100xxxxx 10xxxxxx
		 *   11110000 1000xxxx 10xxxxxx 10xxxxxx
		 *   11111000 10000xxx 10xxxxxx 10xxxxxx 10xxxxxx
		 *   11111100 100000xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
		 */
		if (!utf_char && !((c & 0x7f) >> (7 - utf_count))) {
		    if (screen->utf8_weblike) {
			/* overlong sequence continued */
			data->utf_data = UCS_REPL;
			data->utf_size = i;
			break;
		    } else {
			utf_char = UCS_REPL;
		    }
		}
		utf_char <<= 6;
		utf_char |= (c & 0x3f);
		if ((utf_char >= 0xd800 &&
		     utf_char <= 0xdfff) ||
		    (utf_char == 0xfffe) ||
		    (utf_char == HIDDEN_CHAR)) {
		    utf_char = UCS_REPL;
		}
		utf_count--;
		if (utf_count == 0) {
#if !OPT_WIDER_ICHAR
		    /* characters outside UCS-2 become UCS_REPL */
		    if (utf_char > NARROW_ICHAR) {
			TRACE(("using replacement for %#x\n", utf_char));
			utf_char = UCS_REPL;
		    }
#endif
		    data->utf_data = (IChar) utf_char;
		    data->utf_size = (i + 1);
		    break;
		}
	    }
	} else {
	    /* We received a sequence start byte */
	    if (utf_count > 0) {
		/* previous sequence is incomplete */
		data->utf_data = UCS_REPL;
		data->utf_size = i;
		break;
	    }
	    if (screen->utf8_weblike) {
		if (c < 0xe0) {
		    if (!(c & 0x1e)) {
			/* overlong sequence start */
			data->utf_data = UCS_REPL;
			data->utf_size = (i + 1);
			break;
		    }
		    utf_count = 1;
		    utf_char = (c & 0x1f);
		} else if (c < 0xf0) {
		    utf_count = 2;
		    utf_char = (c & 0x0f);
		} else if (c < 0xf5) {
		    utf_count = 3;
		    utf_char = (c & 0x07);
		} else {
		    data->utf_data = UCS_REPL;
		    data->utf_size = (i + 1);
		    break;
		}
	    } else {
		if (c < 0xe0) {
		    utf_count = 1;
		    utf_char = (c & 0x1f);
		    if (!(c & 0x1e)) {
			/* overlong sequence */
			utf_char = UCS_REPL;
		    }
		} else if (c < 0xf0) {
		    utf_count = 2;
		    utf_char = (c & 0x0f);
		} else if (c < 0xf8) {
		    utf_count = 3;
		    utf_char = (c & 0x07);
		} else if (c < 0xfc) {
		    utf_count = 4;
		    utf_char = (c & 0x03);
		} else if (c < 0xfe) {
		    utf_count = 5;
		    utf_char = (c & 0x01);
		} else {
		    data->utf_data = UCS_REPL;
		    data->utf_size = (i + 1);
		    break;
		}
	    }
	}
    }
#if OPT_TRACE > 1
    TRACE(("UTF-8 char %04X [%lu..%lu]\n",
	   data->utf_data,
	   (unsigned long) (data->next - data->buffer),
	   (unsigned long) (data->next - data->buffer + data->utf_size - 1)));
#endif

    return (data->utf_size != 0);
}
#endif

int
readPtyData(XtermWidget xw, PtySelect * select_mask, PtyData *data)
{
    TScreen *screen = TScreenOf(xw);
    int size = 0;

#ifdef VMS
    if (*select_mask & pty_mask) {
	trimPtyData(xw, data);
	if (read_queue.flink != 0) {
	    size = tt_read(data->next);
	    if (size == 0) {
		Panic("input: read returned zero\n", 0);
	    }
	} else {
	    sys$hiber();
	}
    }
#else /* !VMS */
    if (FD_ISSET(screen->respond, select_mask)) {
	int save_err;
	trimPtyData(xw, data);

	size = (int) read(screen->respond, (char *) data->last, (size_t) FRG_SIZE);
	save_err = errno;
#if (defined(i386) && defined(SVR4) && defined(sun)) || defined(__CYGWIN__)
	/*
	 * Yes, I know this is a majorly f*ugly hack, however it seems to
	 * be necessary for Solaris x86.  DWH 11/15/94
	 * Dunno why though..
	 * (and now CYGWIN, alanh@xfree86.org 08/15/01
	 */
	if (size <= 0) {
	    if (save_err == EIO || save_err == 0)
		NormalExit();
	    else if (!E_TEST(save_err))
		Panic("input: read returned unexpected error (%d)\n", save_err);
	    size = 0;
	}
#else /* !f*ugly */
	if (size < 0) {
	    if (save_err == EIO)
		NormalExit();
	    else if (!E_TEST(save_err))
		Panic("input: read returned unexpected error (%d)\n", save_err);
	    size = 0;
	} else if (size == 0) {
#if defined(__FreeBSD__) || defined(__OpenBSD__)
	    NormalExit();
#else
	    Panic("input: read returned zero\n", 0);
#endif
	}
#endif /* f*ugly */
    }
#endif /* VMS */

    if (size) {
#if OPT_TRACE
	int i;

	TRACE(("read %d bytes from pty\n", size));
	for (i = 0; i < size; i++) {
	    if (!(i % 16))
		TRACE(("%s", i ? "\n    " : "READ"));
	    TRACE((" %02X", data->last[i]));
	}
	TRACE(("\n"));
#endif
	data->last += size;
#ifdef ALLOWLOGGING
	TScreenOf(term)->logstart = VTbuffer->next;
#endif
    }

    return (size);
}

/*
 * Return the next value from the input buffer.  Note that morePtyData() is
 * always called before this function, so we can do the UTF-8 input conversion
 * in that function and simply return the result here.
 */
#if OPT_WIDE_CHARS
IChar
nextPtyData(TScreen *screen, PtyData *data)
{
    IChar result;
    if (screen->utf8_inparse) {
	skipPtyData(data, result);
    } else {
	result = *((data)->next++);
	if (!screen->output_eight_bits) {
	    result = (IChar) (result & 0x7f);
	}
    }
    TRACE2(("nextPtyData returns %#x\n", result));
    return result;
}
#endif

#if OPT_WIDE_CHARS
/*
 * Called when UTF-8 mode has been turned on/off.
 */
void
switchPtyData(TScreen *screen, int flag)
{
    if (screen->utf8_mode != flag) {
	screen->utf8_mode = flag;
	screen->utf8_inparse = (Boolean) (flag != 0);
	mk_wcwidth_init(screen->utf8_mode);

	TRACE(("turning UTF-8 mode %s\n", BtoS(flag)));
	update_font_utf8_mode();
    }
}
#endif

/*
 * Allocate a buffer.
 */
void
initPtyData(PtyData **result)
{
    PtyData *data;

    TRACE2(("initPtyData given minBufSize %d, maxBufSize %d\n",
	    FRG_SIZE, BUF_SIZE));

    if (FRG_SIZE < 64)
	FRG_SIZE = 64;
    if (BUF_SIZE < FRG_SIZE)
	BUF_SIZE = FRG_SIZE;
    if (BUF_SIZE % FRG_SIZE)
	BUF_SIZE = BUF_SIZE + FRG_SIZE - (BUF_SIZE % FRG_SIZE);

    TRACE2(("initPtyData using minBufSize %d, maxBufSize %d\n",
	    FRG_SIZE, BUF_SIZE));

    data = TypeXtMallocX(PtyData, (BUF_SIZE + FRG_SIZE));

    memset(data, 0, sizeof(*data));
    data->next = data->buffer;
    data->last = data->buffer;
    *result = data;
}

/*
 * Initialize a buffer for the caller, using its data in 'next'.
 */
#if OPT_WIDE_CHARS
PtyData *
fakePtyData(PtyData *result, Char *next, Char *last)
{
    PtyData *data = result;

    memset(data, 0, sizeof(*data));
    data->next = next;
    data->last = last;

    return data;
}
#endif

/*
 * Remove used data by shifting the buffer down, to make room for more data,
 * e.g., a continuation-read.
 */
void
trimPtyData(XtermWidget xw, PtyData *data)
{
    (void) xw;
    FlushLog(xw);

    if (data->next != data->buffer) {
	size_t i;
	size_t n = (size_t) (data->last - data->next);

	TRACE(("shifting buffer down by %lu\n", (unsigned long) n));
	for (i = 0; i < n; ++i) {
	    data->buffer[i] = data->next[i];
	}
	data->next = data->buffer;
	data->last = data->next + n;
    }

}

/*
 * Insert new data into the input buffer so the next calls to morePtyData()
 * and nextPtyData() will return that.
 */
void
fillPtyData(XtermWidget xw, PtyData *data, const char *value, size_t length)
{
    size_t size;
    size_t n;

    /* remove the used portion of the buffer */
    trimPtyData(xw, data);

    VTbuffer->last += length;
    size = (size_t) (VTbuffer->last - VTbuffer->next);

    /* shift the unused portion up to make room */
    for (n = size; n >= length; --n)
	VTbuffer->next[n] = VTbuffer->next[n - length];

    /* insert the new bytes to interpret */
    for (n = 0; n < length; n++)
	VTbuffer->next[n] = CharOf(value[n]);
}

#if OPT_WIDE_CHARS
/*
 * Convert an ISO-8859-1 code 'c' to UTF-8, storing the result in the target
 * 'lp', and returning a pointer past the converted character.
 */
Char *
convertToUTF8(Char *lp, unsigned c)
{
#define CH(n) (Char)((c) >> ((n) * 8))
    if (c < 0x80) {
	/*  0*******  */
	*lp++ = (Char) CH(0);
    } else if (c < 0x800) {
	/*  110***** 10******  */
	*lp++ = (Char) (0xc0 | (CH(0) >> 6) | ((CH(1) & 0x07) << 2));
	*lp++ = (Char) (0x80 | (CH(0) & 0x3f));
    } else if (c < 0x00010000) {
	/*  1110**** 10****** 10******  */
	*lp++ = (Char) (0xe0 | ((int) (CH(1) & 0xf0) >> 4));
	*lp++ = (Char) (0x80 | (CH(0) >> 6) | ((CH(1) & 0x0f) << 2));
	*lp++ = (Char) (0x80 | (CH(0) & 0x3f));
    } else if (c < 0x00200000) {
	*lp++ = (Char) (0xf0 | ((int) (CH(2) & 0x1f) >> 2));
	*lp++ = (Char) (0x80 |
			((int) (CH(1) & 0xf0) >> 4) |
			((int) (CH(2) & 0x03) << 4));
	*lp++ = (Char) (0x80 | (CH(0) >> 6) | ((CH(1) & 0x0f) << 2));
	*lp++ = (Char) (0x80 | (CH(0) & 0x3f));
    } else if (c < 0x04000000) {
	*lp++ = (Char) (0xf8 | (CH(3) & 0x03));
	*lp++ = (Char) (0x80 | (CH(2) >> 2));
	*lp++ = (Char) (0x80 |
			((int) (CH(1) & 0xf0) >> 4) |
			((int) (CH(2) & 0x03) << 4));
	*lp++ = (Char) (0x80 | (CH(0) >> 6) | ((CH(1) & 0x0f) << 2));
	*lp++ = (Char) (0x80 | (CH(0) & 0x3f));
    } else {
	*lp++ = (Char) (0xfc | ((int) (CH(3) & 0x40) >> 6));
	*lp++ = (Char) (0x80 | (CH(3) & 0x3f));
	*lp++ = (Char) (0x80 | (CH(2) >> 2));
	*lp++ = (Char) (0x80 | (CH(1) >> 4) | ((CH(2) & 0x03) << 4));
	*lp++ = (Char) (0x80 | (CH(0) >> 6) | ((CH(1) & 0x0f) << 2));
	*lp++ = (Char) (0x80 | (CH(0) & 0x3f));
    }
    return lp;
#undef CH
}

/*
 * Convert a UTF-8 multibyte character to an Unicode value, returning a pointer
 * past the converted UTF-8 input.  The first 256 values align with ISO-8859-1,
 * making it possible to use this to convert to Latin-1.
 *
 * If the conversion fails, return null.
 */
Char *
convertFromUTF8(Char *lp, unsigned *cp)
{
    int want;

    /*
     * Find the number of bytes we will need from the source.
     */
    if ((*lp & 0x80) == 0) {
	want = 1;
    } else if ((*lp & 0xe0) == 0xc0) {
	want = 2;
    } else if ((*lp & 0xf0) == 0xe0) {
	want = 3;
    } else if ((*lp & 0xf8) == 0xf0) {
	want = 4;
    } else if ((*lp & 0xfc) == 0xf8) {
	want = 5;
    } else if ((*lp & 0xfe) == 0xfc) {
	want = 6;
    } else {
	want = 0;
    }

    if (want) {
	int have = 1;

	while (lp[have] != '\0') {
	    if ((lp[have] & 0xc0) != 0x80)
		break;
	    ++have;
	}
	if (want == have) {
	    unsigned mask = 0;
	    int j;
	    int shift = 0;

	    *cp = 0;
	    switch (want) {
	    case 1:
		mask = (*lp);
		break;
	    case 2:
		mask = (*lp & 0x1f);
		break;
	    case 3:
		mask = (*lp & 0x0f);
		break;
	    case 4:
		mask = (*lp & 0x07);
		break;
	    case 5:
		mask = (*lp & 0x03);
		break;
	    case 6:
		mask = (*lp & 0x01);
		break;
	    default:
		mask = 0;
		break;
	    }

	    for (j = 1; j < want; j++) {
		*cp |= (unsigned) ((lp[want - j] & 0x3f) << shift);
		shift += 6;
	    }
	    *cp |= mask << shift;
	    lp += want;
	} else {
	    *cp = BAD_ASCII;
	    lp = NULL;
	}
    } else {
	*cp = BAD_ASCII;
	lp = NULL;
    }
    return lp;
}

/*
 * Returns true if the entire string is valid UTF-8.
 */
Boolean
isValidUTF8(Char *lp)
{
    Boolean result = True;
    while (*lp) {
	unsigned ch;
	Char *next = convertFromUTF8(lp, &ch);
	if (next == NULL || ch == 0) {
	    result = False;
	    break;
	}
	lp = next;
    }
    return result;
}

/*
 * Write data back to the PTY
 */
void
writePtyData(int f, IChar *d, size_t len)
{
    size_t n = (len << 1);

    if (VTbuffer->write_len <= len) {
	VTbuffer->write_len = n;
	VTbuffer->write_buf = realloc(VTbuffer->write_buf, VTbuffer->write_len);
    }

    for (n = 0; n < len; n++)
	VTbuffer->write_buf[n] = (Char) d[n];

    TRACE(("writePtyData %lu:%s\n", (unsigned long) n,
	   visibleChars(VTbuffer->write_buf, n)));
    v_write(f, VTbuffer->write_buf, n);
}
#endif /* OPT_WIDE_CHARS */

#ifdef NO_LEAKS
void
noleaks_ptydata(void)
{
    if (VTbuffer != 0) {
#if OPT_WIDE_CHARS
	free(VTbuffer->write_buf);
#endif
	FreeAndNull(VTbuffer);
    }
}
#endif

#ifdef TEST_DRIVER

#include "data.c"

void
NormalExit(void)
{
    fprintf(stderr, "NormalExit!\n");
    exit(EXIT_SUCCESS);
}

void
Panic(const char *s, int a)
{
    (void) s;
    (void) a;
    fprintf(stderr, "Panic!\n");
    exit(EXIT_FAILURE);
}

#if OPT_WIDE_CHARS

#ifdef ALLOWLOGGING
void
FlushLog(XtermWidget xw)
{
    (void) xw;
}
#endif

void
v_write(int f, const Char *data, size_t len)
{
    (void) f;
    (void) data;
    (void) len;
}

void
mk_wcwidth_init(int mode)
{
    (void) mode;
}

void
update_font_utf8_mode(void)
{
}

static int message_level = 0;
static int opt_all = 0;
static int opt_illegal = 0;
static int opt_convert = 0;
static int opt_reverse = 0;
static long total_test = 0;
static long total_errs = 0;

static void
usage(void)
{
    static const char *msg[] =
    {
	"Usage: test_ptydata [options] [c1[-c1b] [c2-[c2b] [...]]]",
	"",
	"Options:",
	" -a  exercise all legal encode/decode to/from UTF-8",
	" -c  call convertFromUTF8 rather than decodeUTF8",
	" -i  ignore illegal UTF-8 when testing -r option",
	" -q  quieter",
	" -r  reverse/decode from UTF-8 byte-string to/from Unicode",
	" -v  more verbose"
    };
    size_t n;
    for (n = 0; n < sizeof(msg) / sizeof(msg[0]); ++n) {
	fprintf(stderr, "%s\n", msg[n]);
    }
    exit(EXIT_FAILURE);
}

/*
 * http://www.unicode.org/versions/corrigendum1.html, table 3.1B
 */
#define OkRange(n,lo,hi) \
 	if (value[n] < lo || value[n] > hi) { \
	    result = False; \
	    break; \
	}
static Bool
is_legal_utf8(const Char *value)
{
    Bool result = True;
    Char ch;
    while ((ch = *value) != '\0') {
	if (ch <= 0x7f) {
	    ++value;
	} else if (ch >= 0xc2 && ch <= 0xdf) {
	    OkRange(1, 0x80, 0xbf);
	    value += 2;
	} else if (ch == 0xe0) {
	    OkRange(1, 0xa0, 0xbf);
	    OkRange(2, 0x80, 0xbf);
	    value += 3;
	} else if (ch >= 0xe1 && ch <= 0xef) {
	    OkRange(1, 0x80, 0xbf);
	    OkRange(2, 0x80, 0xbf);
	    value += 3;
	} else if (ch == 0xf0) {
	    OkRange(1, 0x90, 0xbf);
	    OkRange(2, 0x80, 0xbf);
	    OkRange(3, 0x80, 0xbf);
	    value += 4;
	} else if (ch >= 0xf1 && ch <= 0xf3) {
	    OkRange(1, 0x80, 0xbf);
	    OkRange(2, 0x80, 0xbf);
	    OkRange(3, 0x80, 0xbf);
	    value += 4;
	} else if (ch == 0xf4) {
	    OkRange(1, 0x80, 0x8f);
	    OkRange(2, 0x80, 0xbf);
	    OkRange(3, 0x80, 0xbf);
	    value += 4;
	} else {
	    result = False;
	    break;
	}
    }
    return result;
}

static void
test_utf8_convert(void)
{
    unsigned c_in, c_out;
    Char buffer[10];
    Char *result;
    unsigned limit = 0x110000;
    unsigned success = 0;
    unsigned bucket[256];

    memset(bucket, 0, sizeof(bucket));
    for (c_in = 0; c_in < limit; ++c_in) {
	memset(buffer, 0, sizeof(buffer));
	if ((result = convertToUTF8(buffer, c_in)) == 0) {
	    TRACE(("conversion of U+%04X to UTF-8 failed\n", c_in));
	} else {
	    if ((result = convertFromUTF8(buffer, &c_out)) == 0) {
		TRACE(("conversion of U+%04X from UTF-8 failed\n", c_in));
	    } else if (c_in != c_out) {
		TRACE(("conversion of U+%04X to/from UTF-8 gave U+%04X\n",
		       c_in, c_out));
	    } else {
		while (result-- != buffer) {
		    bucket[*result]++;
		}
		++success;
	    }
	}
    }
    TRACE(("%u/%u successful\n", success, limit));
    for (c_in = 0; c_in < 256; ++c_in) {
	if ((c_in % 8) == 0) {
	    TRACE((" %02X:", c_in));
	}
	TRACE((" %8X", bucket[c_in]));
	if (((c_in + 1) % 8) == 0) {
	    TRACE(("\n"));
	}
    }
}

static int
decode_one(const char *source, char **target)
{
    int result = -1;
    long check;
    int radix = 0;
    if ((source[0] == 'u' || source[0] == 'U') && source[1] == '+') {
	source += 2;
	radix = 16;
    } else if (source[0] == '0' && source[1] == 'b') {
	source += 2;
	radix = 2;
    }
    check = strtol(source, target, radix);
    if (*target != NULL && *target != source)
	result = (int) check;
    return result;
}

static int
decode_range(const char *source, int *lo, int *hi)
{
    int result = 0;
    char *after1;
    char *after2;
    if ((*lo = decode_one(source, &after1)) >= 0) {
	after1 += strspn(after1, ":-.\t ");
	if ((*hi = decode_one(after1, &after2)) < 0) {
	    *hi = *lo;
	}
	result = 1;
    }
    return result;
}

#define MAX_BYTES 6

static void
do_range(const char *source)
{
    int lo, hi;

    TScreen screen;
    memset(&screen, 0, sizeof(screen));

    if (decode_range(source, &lo, &hi)) {
	while (lo <= hi) {
	    unsigned c_in = (unsigned) lo++;
	    PtyData *data;
	    Char *next;
	    Char buffer[MAX_BYTES + 1];

	    if (opt_reverse) {
		Bool skip = False;
		Bool first = True;
		int j, k;
		for (j = 0; j < MAX_BYTES; ++j) {
		    unsigned long bits = ((unsigned long) c_in >> (8 * j));
		    if ((buffer[j] = (Char) bits) == 0) {
			skip = (bits != 0);
			break;
		    }
		}
		if (skip)
		    continue;
		initPtyData(&data);
		for (k = 0; k <= j; ++k) {
		    data->buffer[k] = buffer[j - k - 1];
		}
		if (opt_illegal && !is_legal_utf8(data->buffer)) {
		    free(data);
		    continue;
		}
		if (message_level > 1) {
		    printf("TEST ");
		    for (k = 0; k < j; ++k) {
			printf("%02X", data->buffer[k]);
		    }
		}
		data->next = data->buffer;
		data->last = data->buffer + j;
		while (decodeUtf8(&screen, data)) {
		    total_test++;
		    if (is_UCS_SPECIAL(data->utf_data))
			total_errs++;
		    data->next += data->utf_size;
		    if (message_level > 1) {
			printf("%s%04X", first ? " ->" : ", ", data->utf_data);
		    }
		    first = False;
		}
		if (!first)
		    total_test--;
		if (message_level > 1) {
		    printf("\n");
		    fflush(stdout);
		}
		free(data);
	    } else if (opt_convert) {
		unsigned c_out;
		Char *result;

		memset(buffer, 0, sizeof(buffer));
		if ((result = next = convertToUTF8(buffer, c_in)) == 0) {
		    fprintf(stderr,
			    "conversion of U+%04X to UTF-8 failed\n", c_in);
		} else if ((result = convertFromUTF8(buffer, &c_out)) == 0) {
		    fprintf(stderr,
			    "conversion of U+%04X from UTF-8 failed\n", c_in);
		    total_errs++;
		} else if (c_in != c_out) {
		    fprintf(stderr,
			    "conversion of U+%04X to/from UTF-8 gave U+%04X\n",
			    c_in, c_out);
		} else if (message_level > 1) {
		    *next = '\0';
		    printf("TEST %04X (%lu:%s) ->%04X\n", c_in,
			   (unsigned long) (next - buffer),
			   buffer,
			   c_out);
		    fflush(stdout);
		}
	    } else {
		initPtyData(&data);
		next = convertToUTF8(data->buffer, c_in);
		*next = 0;
		data->next = data->buffer;
		data->last = next;
		decodeUtf8(&screen, data);
		if (message_level > 1) {
		    printf("TEST %04X (%lu:%s) ->%04X\n", c_in,
			   (unsigned long) (next - data->buffer),
			   data->buffer,
			   data->utf_data);
		    fflush(stdout);
		}
		if (c_in != data->utf_data) {
		    fprintf(stderr, "Mismatch: %04X vs %04X\n", c_in, data->utf_data);
		    total_errs++;
		}
		free(data);
	    }
	    total_test++;
	}
    }
}

int
main(int argc, char **argv)
{
    int ch;

    setlocale(LC_ALL, "");
    while ((ch = getopt(argc, argv, "aciqrv")) != -1) {
	switch (ch) {
	case 'a':
	    opt_all = 1;
	    break;
	case 'c':
	    opt_convert = 1;
	    break;
	case 'i':
	    opt_illegal = 1;
	    break;
	case 'q':
	    message_level--;
	    break;
	case 'r':
	    opt_reverse = 1;
	    break;
	case 'v':
	    message_level++;
	    break;
	default:
	    usage();
	}
    }
    if (opt_all) {
	test_utf8_convert();
    } else {
	if (optind >= argc)
	    usage();
	while (optind < argc) {
	    do_range(argv[optind++]);
	}
	if (total_test) {
	    printf("%ld/%ld mismatches (%.0f%%)\n",
		   total_errs,
		   total_test,
		   (100.0 * (double) total_errs) / (double) total_test);
	}
    }
    return EXIT_SUCCESS;
}
#else
int
main(int argc, char **argv)
{
    (void) argc;
    (void) argv;
    printf("Nothing to be done here...\n");
    return EXIT_SUCCESS;
}
#endif /* OPT_WIDE_CHARS */
#endif
