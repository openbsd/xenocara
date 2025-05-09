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
#

# Initialize Autoconf
AC_PREREQ([2.60])
#
# Version should match the current Render version. XRenderQueryVersion
# returns the version from render.h, NOT the version we set here. But we
# try to keep these the same.  Note that the library has an extra
# digit in the version number to track changes which don't affect the
# protocol, so Xrender version l.n.m corresponds to protocol version l.n
#
AC_INIT(libXrender, [0.9.12],
	[https://gitlab.freedesktop.org/xorg/lib/libxrender/-/issues],
	[libXrender])
AC_CONFIG_SRCDIR([Makefile.am])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIRS([m4])

PACKAGE_BRIEF="Library for the Render Extension to the X11 protocol"
AC_SUBST(PACKAGE_BRIEF)

# Initialize Automake
AM_INIT_AUTOMAKE([foreign dist-xz])

# Initialize libtool
LT_INIT

# Require X.Org macros 1.8 or later for AC_PROG_INSTALL
m4_ifndef([XORG_MACROS_VERSION],
          [m4_fatal([must install xorg-macros 1.8 or later before running autoconf/autogen])])
XORG_MACROS_VERSION(1.8)
XORG_DEFAULT_OPTIONS
XORG_CHECK_MALLOC_ZERO

# Check render configuration, strip extra digits from package version to
# find the required protocol version
if test "$VERSION" = "" ; then
       VERSION=$PACKAGE_VERSION;
fi
RENDER_VERSION=[`echo $VERSION | sed 's/^\([0-9][0-9]*\.[0-9][0-9]*\).*$/\1/'`]
AC_SUBST(RENDER_VERSION)

# Obtain compiler/linker options for dependencies
PKG_CHECK_MODULES(RENDER, [x11 >= 1.6] renderproto >= $RENDER_VERSION)

AC_CONFIG_FILES([Makefile
		src/Makefile
		Doxyfile
		xrender.pc])
AC_OUTPUT
