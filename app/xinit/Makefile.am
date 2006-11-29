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

XINITDIR = $(datadir)/X11/xinit

bin_PROGRAMS = xinit
bin_SCRIPTS = startx

xinit_CFLAGS = $(XINIT_CFLAGS) -DXINITDIR=\"$(XINITDIR)\" -DBINDIR=\"$(bindir)\"
xinit_LDADD = $(XINIT_LIBS)

xinit_SOURCES =	\
        xinit.c

appmandir = $(APP_MAN_DIR)

appman_PRE = \
	startx.man \
        xinit.man

appman_DATA = $(appman_PRE:man=@APP_MAN_SUFFIX@)

# Translate XCOMM into pound sign with sed, rather than passing -DXCOMM=XCOMM
# to cpp, because that trick does not work on all ANSI C preprocessors.
# Delete line numbers from the cpp output (-P is not portable, I guess).
# Allow XCOMM to be preceded by whitespace and provide a means of generating
# output lines with trailing backslashes.
# Allow XHASH to always be substituted, even in cases where XCOMM isn't.

CPP_SED_MAGIC = $(SED) -e '/^\#  *[0-9][0-9]*  *.*$$/d' \
                       -e '/^\#line  *[0-9][0-9]*  *.*$$/d' \
                       -e '/^[ 	]*XCOMM$$/s/XCOMM/\#/' \
                       -e '/^[ 	]*XCOMM[^a-zA-Z0-9_]/s/XCOMM/\#/' \
                       -e '/^[ 	]*XHASH/s/XHASH/\#/' \
                       -e '/\@\@$$/s/\@\@$$/\\/'

# Strings to replace in man pages
XORGRELSTRING = @PACKAGE_STRING@
  XORGMANNAME = X Version 11

MANDEFS =  \
	-D__xorgversion__="\"$(XORGRELSTRING)\" \"$(XORGMANNAME)\"" \
	-D__appmansuffix__=$(APP_MAN_SUFFIX) \
	-D__filemansuffix__=$(FILE_MAN_SUFFIX) \
	-D__libmansuffix__=$(LIB_MAN_SUFFIX) \
	-D__miscmansuffix__=$(MISC_MAN_SUFFIX) \
	-D__XSERVERNAME__=Xorg -D__XCONFIGFILE__=xorg.conf \
	-D__xinitdir__=$(XINITDIR) \
	-DSHELL_CMD=$(SHELL_CMD) $(ARCHMANDEFS)

SUFFIXES = .$(APP_MAN_SUFFIX) .man

.man.$(APP_MAN_SUFFIX):
	$(RAWCPP) $(RAWCPPFLAGS) $(MANDEFS) $(EXTRAMANDEFS) < $< | $(CPP_SED_MAGIC) > $@

xinitrcdir = $(XINITDIR)

PROGCPPDEFS = \
	-DXRDB=@XRDB@ \
	-DXMODMAP=@XMODMAP@ \
	-DTWM=@TWM@ \
	-DXCLOCK=@XCLOCK@ \
	-DXTERM=@XTERM@ \
	-DXSERVER=@XSERVER@ \
	-DXAUTH=@XAUTH@ \
	-DXINIT=@XINIT@ \
	-DWM=@WM@ \
	-DXCONSOLE=@XCONSOLE@

SCRIPTDEFS = \
	-DXINITDIR=$(XINITDIR) $(PROGCPPDEFS) -DLIBDIR=$(libdir) \
	-DSHELL_CMD=$(SHELL_CMD) $(STARTX_COOKIE_FLAGS)

xinitrc: xinitrc.cpp
	$(RAWCPP) $(SCRIPTDEFS) < $(srcdir)/xinitrc.cpp | $(CPP_SED_MAGIC) > $@

startx: startx.cpp Makefile
	$(RAWCPP) $(SCRIPTDEFS) < $(srcdir)/startx.cpp | $(CPP_SED_MAGIC) > $@

xinitrc_DATA = xinitrc

CLEANFILES = xinitrc startx $(appman_DATA)

EXTRA_DIST = xinitrc.cpp startx.cpp $(appman_PRE)	\
		startx.cmd xinitrc.cmd xinit.def

