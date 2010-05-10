#
# Copyright 2007 Sun Microsystems, Inc.  All rights reserved.
# 
# Permission to use, copy, modify, distribute, and sell this software and its
# documentation for any purpose is hereby granted without fee, provided that
# the above copyright notice appear in all copies and that both that
# copyright notice and this permission notice appear in supporting
# documentation.
# 
# The above copyright notice and this permission notice shall be included
# in all copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
# OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
# IN NO EVENT SHALL THE OPEN GROUP BE LIABLE FOR ANY CLAIM, DAMAGES OR
# OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
# ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
# OTHER DEALINGS IN THE SOFTWARE.
# 
# Except as contained in this notice, the name of the copyright holders shall
# not be used in advertising or otherwise to promote the sale, use or
# other dealings in this Software without prior written authorization
# from the copyright holders.
# 

libmandir = $(LIB_MAN_DIR)

LIB_MAN_DIR_SUFFIX = $(LIB_MAN_DIR:@mandir@/man%=%)

libman_xml = XTest.xml

libman_PRE =	\
	XTestQueryExtension.man \
	XTestCompareCursorWithWindow.man \
	XTestCompareCurrentCursorWithWindow.man \
	XTestFakeKeyEvent.man \
	XTestFakeButtonEvent.man \
	XTestFakeMotionEvent.man \
	XTestFakeRelativeMotionEvent.man \
	XTestGrabControl.man \
	XTestSetGContextOfGC.man \
	XTestSetVisualIDOfVisual.man \
	XTestDiscard.man

EXTRA_DIST = $(libman_PRE) $(libman_xml) x.stamp

CLEANFILES = $(libman_DATA) $(BUILT_SOURCES)

MAINTAINERCLEANFILES = $(libman_PRE)

if HAVE_XMLTO
libman_DATA = $(libman_PRE:man=@LIB_MAN_SUFFIX@)
endif

SED = sed

# Strings to replace in man pages
XORGRELSTRING = @PACKAGE_STRING@
  XORGMANNAME = X Version 11

MAN_SUBSTS = \
	-e 's/__xorgversion__/"$(XORGRELSTRING)" "$(XORGMANNAME)"/' \
	-e 's/__xservername__/Xorg/g' \
	-e 's/__xconfigfile__/xorg.conf/g' \
	-e 's/__appmansuffix__/$(APP_MAN_SUFFIX)/g' \
	-e 's/__libmansuffix__/$(LIB_MAN_SUFFIX)/g' \
	-e 's/__filemansuffix__/$(FILE_MAN_SUFFIX)/g'

SUFFIXES = .$(LIB_MAN_SUFFIX) .man

.man.$(LIB_MAN_SUFFIX):
	sed $(MAN_SUBSTS) < $< > $@

# This is ugly, but handling of tools with multiple outputs in make is a
# huge PITA. The commands below are taken from the automake manual:
# http://www.gnu.org/software/automake/manual/automake.html#Multiple-Outputs
if HAVE_XMLTO
x.stamp: $(libman_xml)
	@rm -f x.tmp
	@touch x.tmp
	$(XMLTO) man $^
	@for man in $(libman_PRE); do \
	  out=`echo $$man | sed 's/\.man/.__libmansuffix__/'`; \
	  echo mv -f $$out $$man; \
	  mv -f $$out $$man || exit 1; \
	done
	@mv -f x.tmp $@
$(libman_PRE): x.stamp
	@if test -f $@; then :; else \
	  rm -f $<; \
	  $(MAKE) $(AM_MAKEFLAGS) $<; \
	fi
CLEANFILES += x.tmp
MAINTAINERCLEANFILES += x.stamp
endif # HAVE_XMLTO
