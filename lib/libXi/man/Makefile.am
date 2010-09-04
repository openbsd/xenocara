#
# Copyright 2005 Sun Microsystems, Inc.  All rights reserved.
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

XI2_manpages = \
	XIChangeHierarchy.txt \
	XIDefineCursor.txt \
	XIGrabButton.txt \
	XIGrabDevice.txt \
        XIGrabEnter.txt \
	XIQueryDevice.txt \
	XIQueryPointer.txt \
	XIQueryVersion.txt \
	XISelectEvents.txt \
	XISetClientPointer.txt \
	XISetFocus.txt \
	XIWarpPointer.txt \
	XIListProperties.txt \
	XIChangeProperty.txt

libman_txt = \
	XAllowDeviceEvents.txt \
	XChangeDeviceDontPropagateList.txt \
	XChangeKeyboardDevice.txt \
	XChangePointerDevice.txt \
	XDeviceBell.txt \
	XGetDeviceControl.txt \
	XGetDeviceKeyMapping.txt \
	XGetDeviceModifierMapping.txt \
	XGetDeviceMotionEvents.txt \
	XGetDeviceProperty.txt \
	XGetExtensionVersion.txt \
	XGetFeedbackControl.txt \
	XGrabDeviceButton.txt \
	XGrabDeviceKey.txt \
	XGrabDevice.txt \
	XListDeviceProperties.txt \
	XListInputDevices.txt \
	XOpenDevice.txt \
	XQueryDeviceState.txt \
	XSelectExtensionEvent.txt \
	XSendExtensionEvent.txt \
	XSetDeviceButtonMapping.txt \
	XSetDeviceFocus.txt \
	XSetDeviceMode.txt \
	XSetDeviceValuators.txt \
        $(XI2_manpages)

libman_xml = $(libman_txt:.txt=.xml)
# manpages that are references to another page. These are produced as a
# side effect of the xmlto rule, so they need to be handled specially.

XI2_refpages = \
	XIUndefineCursor.man \
	XIUngrabButton.man \
	XIGrabKeycode.man \
	XIUngrabKeycode.man \
	XIUngrabDevice.man  \
        XIUngrabEnter.man \
        XIGrabFocusIn.man \
        XIUngrabFocusIn.man \
	XIGetClientPointer.man \
	XIGetFocus.man \
	XIGetSelectedEvents.man \
	XIDeleteProperty.man \
	XIGetProperty.man \
	XIFreeDeviceInfo.man

libman_ref = \
	XGetDeviceDontPropagateList.man \
	XChangeDeviceControl.man \
	XChangeDeviceKeyMapping.man \
	XSetDeviceModifierMapping.man \
	XChangeFeedbackControl.man \
	XUngrabDeviceButton.man \
	XUngrabDeviceKey.man \
	XUngrabDevice.man \
	XDeviceTimeCoord.man \
	XFreeDeviceList.man \
	XCloseDevice.man \
	XGetSelectedExtensionEvents.man \
	XGetDeviceButtonMapping.man \
	XGetDeviceFocus.man \
	XChangeDeviceProperty.man \
	XDeleteDeviceProperty.man \
        $(XI2_refpages)


libman_pre = $(libman_txt:.txt=.man) $(libman_ref)

if INSTALL_MANPAGES
nodist_libman_DATA = $(libman_pre:.man=.$(LIB_MAN_SUFFIX))
endif

EXTRA_DIST = $(libman_txt) $(libman_pre)
CLEANFILES = $(nodist_libman_DATA) *.libmansuffix
MAINTAINERCLEANFILES = $(libman_pre) *.xml

if HAVE_DOCTOOLS
# prerequisites for the side effect pages
XGetDeviceDontPropagateList.man: XChangeDeviceDontPropagateList.man
XChangeDeviceControl.man: XGetDeviceControl.man
XChangeDeviceKeyMapping.man: XGetDeviceKeyMapping.man
XSetDeviceModifierMapping.man: XGetDeviceModifierMapping.man
XDeviceTimeCoord.man: XGetDeviceMotionEvents.man
XChangeFeedbackControl.man: XGetFeedbackControl.man
XUngrabDeviceButton.man: XGrabDeviceButton.man
XUngrabDeviceKey.man: XGrabDeviceKey.man
XUngrabDevice.man: XGrabDevice.man
XFreeDeviceList.man: XListInputDevices.man
XCloseDevice.man: XOpenDevice.man
XGetSelectedExtensionEvents.man: XSelectExtensionEvent.man
XGetDeviceButtonMapping.man: XSetDeviceButtonMapping.man
XGetDeviceFocus.man: XSetDeviceFocus.man
XChangeDeviceProperty.man: XGetDeviceProperty.man
XDeleteDeviceProperty.man: XGetDeviceProperty.man

# XI2 prereqs
XIUndefineCursor.man: XIDefineCursor.man
XIUngrabButton.man XIGrabKeycode.man XIUngrabKeycode.man: XIGrabButton.man
XIGetClientPointer.man: XISetClientPointer.man
XIGetFocus.man: XISetFocus.man
XIUngrabDevice.man: XIGrabDevice.man
XIGetProperty.man: XIChangeProperty.man
XIDeleteProperty.man: XIChangeProperty.man
XIUngrabEnter.man XIGrabFocusIn.man XIUngrabFocusIn.man: XIGrabEnter.man
XIGetSelectedEvents.man: XISelectEvents.man
XIFreeDeviceInfo.man: XIQueryDevice.man
endif

# Strings to replace in man pages
XORGRELSTRING = $(PACKAGE_STRING)
  XORGMANNAME = X Version 11

MAN_SUBSTS = \
	-e 's/__xorgversion__/"$(XORGRELSTRING)" "$(XORGMANNAME)"/' \
	-e 's/__xservername__/Xorg/g' \
	-e 's/__xconfigfile__/xorg.conf/g' \
	-e 's/__appmansuffix__/$(APP_MAN_SUFFIX)/g' \
	-e 's/libmansuffix/$(LIB_MAN_SUFFIX)/g' \
	-e 's/__filemansuffix__/$(FILE_MAN_SUFFIX)/g'

SUFFIXES = .man .$(LIB_MAN_SUFFIX)
.man.$(LIB_MAN_SUFFIX):
	$(SED) $(MAN_SUBSTS) < $< > $@

if HAVE_DOCTOOLS
SUFFIXES += .txt .xml
.txt.xml:
	$(ASCIIDOC) -b docbook -d manpage -o $@ $<
.xml.man:
	$(XMLTO) man $<
	mv -f $(@:.man=.libmansuffix) $@

# need to force asciidoc/xmlto to run if necessary for side effect pages
$(libman_ref):
	@if test ! -f $(@:.man=.libmansuffix); then \
	  rm -f $<; \
	  $(MAKE) $(AM_MAKEFLAGS) $< || exit 1; \
	fi
	mv -f $(@:.man=.libmansuffix) $@
endif
