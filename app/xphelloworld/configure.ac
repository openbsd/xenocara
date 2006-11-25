
dnl  Copyright 2005 Red Hat, Inc.
dnl 
dnl  Permission to use, copy, modify, distribute, and sell this software and its
dnl  documentation for any purpose is hereby granted without fee, provided that
dnl  the above copyright notice appear in all copies and that both that
dnl  copyright notice and this permission notice appear in supporting
dnl  documentation, and that the name of Red Hat not be used in
dnl  advertising or publicity pertaining to distribution of the software without
dnl  specific, written prior permission.  Red Hat makes no
dnl  representations about the suitability of this software for any purpose.  It
dnl  is provided "as is" without express or implied warranty.
dnl 
dnl  RED HAT DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
dnl  INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO
dnl  EVENT SHALL RED HAT BE LIABLE FOR ANY SPECIAL, INDIRECT OR
dnl  CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
dnl  DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
dnl  TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
dnl  PERFORMANCE OF THIS SOFTWARE.
dnl
dnl Process this file with autoconf to create configure.

AC_PREREQ([2.57])
AC_INIT(xphelloworld,[1.0.1], [https://bugs.freedesktop.org/enter_bug.cgi?product=xorg],xphelloworld)
AM_INIT_AUTOMAKE([dist-bzip2])
AM_MAINTAINER_MODE

AM_CONFIG_HEADER(config.h)

AC_PROG_CC
AC_PROG_INSTALL

# Checking for Motif
#
build_motifapps=yes

AC_ARG_WITH(motif-libraries, [Directory containing the Motif libraries], 
			     motif_libs=$withval)
AC_ARG_WITH(motif-includes, [Directory containing the Xm direcotry with the Motif header files (eg. /usr/X11R6/include)],
			    motif_includes=$withval)

if test x$with_motif_libraries = xno; then
   echo no motif - commandline  $with_motif_libraries
   build_motifapps=no
fi

if [[ -z $motif_libs ]] ; then
   PKG_CHECK_MODULES(X11, x11)
   AC_CHECK_LIB(Xm, XmFontListCreate, build_motifapps=no, [$X11])
   echo motif library not found
   build_motifapps=no
fi

if test x$build_motifapps = xyes ; then
   if [[ ! -z $motif_includes ]] ; then
      MOTIF_CFLAGS=-I$motif_includes
   fi

   if [[ ! -z $motif_libs ]] ; then
      MOTIF_LIBS="-L$motif_libs -lXm"
   else
      MOTIF_LIBS="-lXm"
   fi
fi

echo build: Build Motif Applications: $build_motifapps

AM_CONDITIONAL([BUILD_MOTIFAPPS], test "$build_motifapps" = yes)

# xpawhelloworld
#
if test x$build_motifapps = xyes ; then
   PKG_CHECK_MODULES(XPAWHELLOWORLD, xaw8 xprintutil xp)
   XPAWHELLOWORLD_CFLAGS="$XPAWHELLOWORLD_CFLAGS $MOTIF_CFLAGS"
   XPAWHELLOWORLD_LIBS="$XPAWHELLOWORLD_LIBS $MOTIF_LIBS"
fi

AC_SUBST(XPAWHELLOWORLD_CFLAGS)
AC_SUBST(XPAWHELLOWORLD_LIBS)

#
# xphelloworld
#

PKG_CHECK_MODULES(XPHELLOWORLD, xprintapputil xprintutil xp)

#
# xpsimplehelloworld
#

PKG_CHECK_MODULES(XPSIMPLEHELLOWORLD, xprintutil xp)

#
# xpxmhellowolrd
#

if test x$build_motifapps = xyes ; then
   PKG_CHECK_MODULES(XPXMHELLOWORLD, xprintutil xt xp)
   XPXMHELLOWORLD_CFLAGS="$XPXMHELLOWORLD_CFLAGS $MOTIF_CFLAGS"
   XPXMHELLOWORLD_LIBS="$XPXMHELLOWORLD_LIBS $MOTIF_LIBS"
fi

#
# xpxthelloworld
#

PKG_CHECK_MODULES(XPXTHELLOWORLD, xaw8 xprintutil xt xp)

XORG_RELEASE_VERSION

AC_OUTPUT([
	Makefile
	xpawhelloworld/Makefile
	xphelloworld/Makefile
	xpsimplehelloworld/Makefile
	xpxmhelloworld/Makefile
	xpxthelloworld/Makefile
])
