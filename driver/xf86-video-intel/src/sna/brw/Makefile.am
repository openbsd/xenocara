
#  Copyright 2005 Adam Jackson.
#
#  Permission is hereby granted, free of charge, to any person obtaining a
#  copy of this software and associated documentation files (the "Software"),
#  to deal in the Software without restriction, including without limitation
#  on the rights to use, copy, modify, merge, publish, distribute, sub
#  license, and/or sell copies of the Software, and to permit persons to whom
#  the Software is furnished to do so, subject to the following conditions:
#
#  The above copyright notice and this permission notice (including the next
#  paragraph) shall be included in all copies or substantial portions of the
#  Software.
#
#  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
#  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
#  FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.  IN NO EVENT SHALL
#  ADAM JACKSON BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
#  IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
#  CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

noinst_LTLIBRARIES = libbrw.la
noinst_PROGRAMS = brw_test

AM_CFLAGS = \
	@CWARNFLAGS@ \
	-I$(top_srcdir)/src \
	-I$(top_srcdir)/src/render_program \
	@XORG_CFLAGS@ \
	@UDEV_CFLAGS@ \
	@DRM_CFLAGS@ \
	$(NULL)

if DEBUG
AM_CFLAGS += @VALGRIND_CFLAGS@
endif

libbrw_la_SOURCES = \
	brw.h \
	brw_disasm.c \
	brw_eu.h \
	brw_eu.c \
	brw_eu_emit.c \
	brw_sf.c \
	brw_wm.c \
	$(NULL)

brw_test_SOURCES = \
	brw_test.c \
	brw_test.h \
	brw_test_gen4.c \
	brw_test_gen5.c \
	brw_test_gen6.c \
	brw_test_gen7.c \
	$(NULL)

brw_test_LDADD = \
	libbrw.la \
	$(NULL)

AM_CFLAGS += @NOWARNFLAGS@
