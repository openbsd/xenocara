# 
#  Copyright 2005  Red Hat, Inc.
# 
#  Permission to use, copy, modify, distribute, and sell this software and its
#  documentation for any purpose is hereby granted without fee, provided that
#  the above copyright notice appear in all copies and that both that
#  copyright notice and this permission notice appear in supporting
#  documentation, and that the name of Red Hat not be used in
#  advertising or publicity pertaining to distribution of the software without
#  specific, written prior permission.  Red Hat makes no
#  representations about the suitability of this software for any purpose.  It
#  is provided "as is" without express or implied warranty.
# 
#  RED HAT DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
#  INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO
#  EVENT SHALL RED HAT BE LIABLE FOR ANY SPECIAL, INDIRECT OR
#  CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
#  DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
#  TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
#  PERFORMANCE OF THIS SOFTWARE.

AM_YFLAGS = -d

SUBDIRS = man
bin_PROGRAMS = xgc

AM_CFLAGS = $(CWARNFLAGS) $(XGC_CFLAGS) -D_BSD_SOURCE
xgc_LDADD = $(XGC_LIBS) -lm

xgc_SOURCES =	\
        choice.c \
        constants.h \
        dashlist.c \
        getfile.c \
	gram.y \
        interpret.c \
	lex.l \
        main.c \
        planemask.c \
        record.c \
        testfrac.c \
        tests.c \
        text.c \
        xgc.h

BUILT_SOURCES = gram.h
MAINTAINERCLEANFILES = ChangeLog INSTALL

# App default files

DISTCHECK_CONFIGURE_FLAGS = --with-appdefaultdir=\$${datadir}/X11/app-defaults

appdefaultdir = @appdefaultdir@

dist_appdefault_DATA = \
        app-defaults/Xgc \
        app-defaults/Xgc-color

EXTRA_DIST = tile Bugs	\
	Written/FilledRects						\
	Written/Interface						\
	Written/Jim							\
	Written/Notes							\
	Written/Notes2							\
	Written/Outline							\
	Written/Widget

.PHONY: ChangeLog INSTALL

INSTALL:
	$(INSTALL_CMD)

ChangeLog:
	$(CHANGELOG_CMD)

dist-hook: ChangeLog INSTALL
