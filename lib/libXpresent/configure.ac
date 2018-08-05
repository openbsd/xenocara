#
#  Copyright © 2012 Keith Packard
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
# Version should match the current XPresent version. XPresentQueryVersion
# returns the version from xpresentwire.h, NOT the version we set here. But we
# try to keep these the same.  Note that the library has an extra
# digit in the version number to track changes which don't affect the
# protocol, so Xpresent version l.n.m corresponds to protocol version l.n,
# that 'revision' number appears in Xpresent.h and has to be manually
# synchronized.
#
AC_INIT(libXpresent, [1.0.0],
	[https://bugs.freedesktop.org/enter_bug.cgi?product=xorg], [libXpresent])
AC_CONFIG_SRCDIR([Makefile.am])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_AUX_DIR([.])
AC_CONFIG_MACRO_DIR([m4])

# Initialize Automake
AM_INIT_AUTOMAKE([foreign dist-bzip2])
AM_MAINTAINER_MODE

# Initialize libtool
AC_PROG_LIBTOOL

# Require xorg-macros 1.8 or later for MAN_SUBSTS set by XORG_MANPAGE_SECTIONS
m4_ifndef([XORG_MACROS_VERSION],
	  [m4_fatal([must install xorg-macros 1.8 or later before running autoconf/autogen])])
XORG_MACROS_VERSION(1.8)
XORG_DEFAULT_OPTIONS

# Check presentext configuration, strip extra digits from package version to
# find the required protocol version
PRESENTEXT_VERSION=[`echo $VERSION | sed 's/^\([0-9][0-9]*\.[0-9][0-9]*\).*$/\1/'`]
AC_SUBST(PRESENTEXT_VERSION)

# Obtain compiler/linker options for depedencies
PKG_CHECK_MODULES(PRESENTEXT, xproto [presentproto >= $PRESENTEXT_VERSION] xextproto x11)

AC_CONFIG_FILES([Makefile
		src/Makefile
		man/Makefile
		xpresent.pc])
AC_OUTPUT
