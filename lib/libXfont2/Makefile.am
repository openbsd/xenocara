#
#  Copyright © 2003 Keith Packard, Noah Levitt
#
#  Permission to use, copy, modify, distribute, and sell this software and its
#  documentation for any purpose is hereby granted without fee, provided that
#  the above copyright notice appear in all copies and that both that
#  copyright notice and this permission notice appear in supporting
#  documentation, and that the name of Keith Packard not be used in
#  advertising or publicity pertaining to distribution of the software without
#  specific, written prior permission.  Keith Packard makes no
#  representations about the suitability of this software for any purpose.  It
#  is provided "as is" without express or implied warranty.
#
#  KEITH PACKARD DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
#  INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO
#  EVENT SHALL KEITH PACKARD BE LIABLE FOR ANY SPECIAL, INDIRECT OR
#  CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
#  DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
#  TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
#  PERFORMANCE OF THIS SOFTWARE.

SUBDIRS=doc

libXfontincludedir = $(includedir)/X11/fonts
libXfontinclude_HEADERS = \
	include/X11/fonts/libxfont2.h

noinst_HEADERS = \
	include/X11/fonts/bdfint.h   \
	include/X11/fonts/bitmap.h   \
	include/X11/fonts/bufio.h    \
	include/X11/fonts/fntfil.h   \
	include/X11/fonts/fntfilio.h \
	include/X11/fonts/fntfilst.h \
	include/X11/fonts/fontmisc.h \
	include/X11/fonts/fontshow.h \
	include/X11/fonts/fontutil.h \
	include/X11/fonts/fontxlfd.h \
	include/X11/fonts/pcf.h      \
	include/libxfontint.h        \
	src/FreeType/ft.h	     \
	src/FreeType/ftfuncs.h       \
	src/util/replace.h

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = xfont2.pc

lib_LTLIBRARIES = libXfont2.la

AM_CPPFLAGS = -I${top_srcdir}/include
AM_CFLAGS = $(XFONT_CFLAGS) $(OS_CFLAGS) $(CWARNFLAGS)

libXfont2_la_SOURCES =			\
	src/stubs/atom.c		\
	src/stubs/libxfontstubs.c	\
	src/util/fontaccel.c		\
	src/util/fontnames.c		\
	src/util/fontutil.c		\
	src/util/fontxlfd.c		\
	src/util/format.c		\
	src/util/miscutil.c		\
	src/util/patcache.c		\
	src/util/private.c		\
	src/util/utilbitmap.c

libXfont2_la_LDFLAGS = -version-number 2:0:0 -no-undefined

libXfont2_la_LIBADD = $(ZLIB_LIBS) $(Z_LIBS) $(MATH_LIBS) $(XFONT_LIBS) $(LTLIBOBJS)

if XFONT_FONTFILE
libXfont2_la_SOURCES +=			\
	src/fontfile/bitsource.c	\
	src/fontfile/bufio.c		\
	src/fontfile/decompress.c	\
	src/fontfile/defaults.c		\
	src/fontfile/dirfile.c		\
	src/fontfile/fileio.c		\
	src/fontfile/filewr.c		\
	src/fontfile/fontdir.c		\
	src/fontfile/fontfile.c		\
	src/fontfile/fontscale.c	\
	src/fontfile/gunzip.c		\
	src/fontfile/register.c		\
	src/fontfile/renderers.c	\
	src/fontfile/catalogue.c

if X_BZIP2_FONT_COMPRESSION
libXfont2_la_SOURCES += src/fontfile/bunzip2.c
endif
endif

if XFONT_FREETYPE
AM_CFLAGS += $(FREETYPE_CFLAGS)

libXfont2_la_LIBADD += $(FREETYPE_LIBS)
libXfont2_la_SOURCES +=			\
	src/FreeType/ft.h		\
	src/FreeType/ftfuncs.h		\
	src/FreeType/xttcap.h		\
	src/FreeType/ftenc.c		\
	src/FreeType/ftfuncs.c		\
	src/FreeType/fttools.c		\
	src/FreeType/xttcap.c
endif

if XFONT_BITMAP
libXfont2_la_SOURCES +=			\
	src/bitmap/bitmap.c		\
	src/bitmap/bitmapfunc.c		\
	src/bitmap/bitmaputil.c		\
	src/bitmap/bitscale.c		\
	src/bitmap/fontink.c

if XFONT_BDFFORMAT
libXfont2_la_SOURCES +=			\
	src/bitmap/bdfread.c		\
	src/bitmap/bdfutils.c
endif

if XFONT_PCF_OR_BUILTIN
libXfont2_la_SOURCES += src/bitmap/pcfread.c
endif

if XFONT_PCFFORMAT
libXfont2_la_SOURCES += src/bitmap/pcfwrite.c
endif

if XFONT_SNFFORMAT
libXfont2_la_SOURCES +=			\
	src/bitmap/snfread.c		\
	src/bitmap/snfstr.h
endif
endif

if XFONT_BUILTINS
libXfont2_la_SOURCES +=			\
	src/builtins/builtin.h		\
	src/builtins/dir.c		\
	src/builtins/file.c		\
	src/builtins/fonts.c		\
	src/builtins/fpe.c		\
	src/builtins/render.c
endif

if XFONT_FC
libXfont2_la_SOURCES +=			\
	src/fc/fsconvert.c		\
	src/fc/fserve.c			\
	src/fc/fserve.h			\
	src/fc/fservestr.h		\
	src/fc/fsio.c			\
	src/fc/fsio.h			\
	src/fc/fslibos.h		\
	src/fc/fstrans.c
endif

EXTRA_DIST = src/builtins/buildfont README.md

# Test utilities
EXTRA_DIST += test/utils/README

TEST_UTIL_SRCS = test/utils/font-test-utils.c test/utils/font-test-utils.h

noinst_PROGRAMS = lsfontdir

lsfontdir_SOURCES = test/utils/lsfontdir.c $(TEST_UTIL_SRCS)
lsfontdir_LDADD = libXfont2.la $(LTLIBOBJS)

# Security regression tests
TESTS =
check_PROGRAMS =

if XFONT_FC
TESTS += test-fserve-read-glyphs
check_PROGRAMS += test-fserve-read-glyphs

# The test #includes fserve.c directly to access static functions so
# we statically link against libXfont2.a.
test_fserve_read_glyphs_SOURCES = test/test-fserve-read-glyphs.c $(TEST_UTIL_SRCS)
test_fserve_read_glyphs_CFLAGS = $(AM_CFLAGS) -I$(top_srcdir)/include -I$(top_srcdir)/src/fc
test_fserve_read_glyphs_LDFLAGS = -static
test_fserve_read_glyphs_LDADD = libXfont2.la $(LTLIBOBJS)
endif XFONT_FC

# PCF parser security regression tests
if HAVE_PYTHON
TESTS += test-pcf-repad test-pcf-props test-pcf-bitmapscale \
	test-pcf-encoding test-pcf-norepad
check_PROGRAMS += test-pcf-repad test-pcf-props test-pcf-bitmapscale \
		 test-pcf-encoding test-pcf-norepad

PCF_TEST_SRCS = test/pcf-test-common.c test/pcf-test-common.h $(TEST_UTIL_SRCS)
PCF_TEST_LDADD = libXfont2.la $(LTLIBOBJS)

test_pcf_repad_SOURCES = test/test-pcf-repad.c $(PCF_TEST_SRCS)
test_pcf_repad_LDADD = $(PCF_TEST_LDADD)

test_pcf_props_SOURCES = test/test-pcf-props.c $(PCF_TEST_SRCS)
test_pcf_props_LDADD = $(PCF_TEST_LDADD)

test_pcf_bitmapscale_SOURCES = test/test-pcf-bitmapscale.c $(PCF_TEST_SRCS)
test_pcf_bitmapscale_LDADD = $(PCF_TEST_LDADD)

test_pcf_encoding_SOURCES = test/test-pcf-encoding.c $(PCF_TEST_SRCS)
test_pcf_encoding_LDADD = $(PCF_TEST_LDADD)

test_pcf_norepad_SOURCES = test/test-pcf-norepad.c $(PCF_TEST_SRCS)
test_pcf_norepad_LDADD = $(PCF_TEST_LDADD)

# Generate malicious PCF test fonts from Python scripts at build time.
# Each script creates a .pcf and fonts.dir in its output directory.
TEST_FONT_DIRS = test/data/repad test/data/props test/data/scale \
		 test/data/encoding \
		 test/data/norepad

test/data/repad/evil-repad.pcf: $(srcdir)/test/gen_evil_pcf_repad.py
	$(AM_V_GEN)$(PYTHON) $< test/data/repad

test/data/props/evil-props.pcf: $(srcdir)/test/gen_evil_pcf_props.py
	$(AM_V_GEN)$(PYTHON) $< test/data/props

test/data/scale/evil-scale.pcf: $(srcdir)/test/gen_evil_pcf_bitmapscale.py
	$(AM_V_GEN)$(PYTHON) $< test/data/scale

test/data/encoding/evil-encoding.pcf: $(srcdir)/test/gen_evil_pcf_encoding.py
	$(AM_V_GEN)$(PYTHON) $< test/data/encoding

test/data/norepad/evil-norepad.pcf: $(srcdir)/test/gen_evil_pcf_norepad.py
	$(AM_V_GEN)$(PYTHON) $< test/data/norepad

TEST_FONT_FILES = test/data/repad/evil-repad.pcf	\
		  test/data/props/evil-props.pcf	\
		  test/data/scale/evil-scale.pcf	\
		  test/data/encoding/evil-encoding.pcf	\
		  test/data/norepad/evil-norepad.pcf

# Font data must be generated before the tests run
check_DATA = $(TEST_FONT_FILES)

CLEANFILES = $(TEST_FONT_FILES)			\
	     test/data/repad/fonts.dir		\
	     test/data/props/fonts.dir		\
	     test/data/scale/fonts.dir		\
	     test/data/scale/fonts.scale	\
	     test/data/encoding/fonts.dir	\
	     test/data/norepad/fonts.dir
clean-local:
	-rmdir $(TEST_FONT_DIRS) test/data 2>/dev/null || true
endif HAVE_PYTHON

EXTRA_DIST += test/gen_evil_pcf_repad.py		\
	      test/gen_evil_pcf_props.py		\
	      test/gen_evil_pcf_bitmapscale.py	\
	      test/gen_evil_pcf_encoding.py	\
	      test/gen_evil_pcf_norepad.py


MAINTAINERCLEANFILES = ChangeLog INSTALL

.PHONY: ChangeLog INSTALL

INSTALL:
	$(INSTALL_CMD)

ChangeLog:
	$(CHANGELOG_CMD)

dist-hook: ChangeLog INSTALL
