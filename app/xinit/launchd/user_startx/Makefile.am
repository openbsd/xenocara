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

include $(top_srcdir)/cpprules.in

xinitrcdir = $(XINITDIR)

xinitrc_PROGRAMS = launchd_startx

AM_CPPFLAGS = -I$(srcdir)/.. -DXINITDIR=\"$(xinitrcdir)\" -DBINDIR=\"$(bindir)\"

dist_launchd_startx_SOURCES = \
	$(srcdir)/../console_redirect.c \
	launchd_startx.c

CPP_FILES_FLAGS = \
	-D__xinitrcdir__="$(xinitrcdir)" \
	-D__bindir__="$(bindir)" \
	-DBUNDLE_ID_PREFIX="$(bundleidprefix)"

$(bundleidprefix).startx.plist.cpp: startx.plist.cpp
	cp $< $@

launchagents_PRE = $(bundleidprefix).startx.plist.cpp
launchagents_DATA = $(launchagents_PRE:plist.cpp=plist)

EXTRA_DIST = startx.plist.cpp

CLEANFILES = \
	$(launchagents_DATA) \
	$(bundleidprefix).startx.plist.cpp
