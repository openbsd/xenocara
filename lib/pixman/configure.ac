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

#   Pixman versioning scheme
#
#   - If the changes don't affect API or ABI, then increment pixman_micro
#   - If API is added, then increment PIXMAN_MINOR, and set MICRO to 0
#
#   - If you break ABI, then
#        - In the first development release where you break ABI, find all instances of
#          "pixman-n" and change it to pixman-(n+1)
#
#          This needs to be done at least in 
#                    configure.ac
#                    all Makefile.am's
#                    pixman-n.pc.in
#
#      This ensures that binary incompatible versions can be installed in parallel.
#      See http://www106.pair.com/rhp/parallel.html for more information
#

m4_define([pixman_major], 0)
m4_define([pixman_minor], 9)
m4_define([pixman_micro], 5)

m4_define([pixman_version],[pixman_major.pixman_minor.pixman_micro])

AC_INIT(pixman, pixman_version, "sandmann@daimi.au.dk", pixman)
AM_INIT_AUTOMAKE([dist-bzip2])

AM_CONFIG_HEADER(config.h)

AC_PROG_CC
AC_PROG_LIBTOOL
AC_CHECK_FUNCS([getisax])
AC_C_BIGENDIAN

# 
# We ignore pixman_major in the version here because the major version should
# always be encoded in the actual library name. Ie., the soname is:
#
#      pixman-$(pixman_major).0.minor.micro
#
m4_define([lt_current], [pixman_minor])
m4_define([lt_revision], [pixman_micro])
m4_define([lt_age], [pixman_minor])

LT_VERSION_INFO="lt_current:lt_revision:lt_age"

PIXMAN_MAJOR=pixman_major
AC_SUBST(PIXMAN_MAJOR)

AC_SUBST(LT_VERSION_INFO)

# Check for dependencies
#PKG_CHECK_MODULES(DEP, x11)

changequote(,)dnl
if test "x$GCC" = "xyes"; then
  case " $CFLAGS " in
  *[\ \	]-Wall[\ \	]*) ;;
  *) CFLAGS="$CFLAGS -Wall" ;;
  esac fi changequote([,])dnl

dnl ===========================================================================
dnl Check for MMX

MMX_CFLAGS="-mmmx -msse -Winline --param inline-unit-growth=10000 --param large-function-growth=10000"

have_mmx_intrinsics=no
AC_MSG_CHECKING(For MMX/SSE intrinsics in the compiler)
xserver_save_CFLAGS=$CFLAGS
CFLAGS="$CFLAGS $MMX_CFLAGS"
AC_COMPILE_IFELSE([
#if defined(__GNUC__) && (__GNUC__ < 3 || (__GNUC__ == 3 && __GNUC_MINOR__ < 4))
#error "Need GCC >= 3.4 for MMX intrinsics"
#endif
#include <mmintrin.h>
#include <xmmintrin.h>
int main () {
    __m64 v = _mm_cvtsi32_si64 (1);
    v = _mm_shuffle_pi16 (v, _MM_SHUFFLE(3, 3, 3, 3));
    return _mm_cvtsi64_si32 (v);
}], have_mmx_intrinsics=yes)
CFLAGS=$xserver_save_CFLAGS
AC_MSG_RESULT($have_mmx_intrinsics)

if test $have_mmx_intrinsics = yes ; then
   AC_DEFINE(USE_MMX, 1, [use MMX compiler intrinsics])
else
   MMX_CFLAGS=
fi
AC_SUBST(MMX_CFLAGS)

AM_CONDITIONAL(USE_MMX, test $have_mmx_intrinsics = yes)

dnl ========================================================

#PKG_CHECK_MODULES(GTK, [gtk+-2.0], [HAVE_GTK=yes], [HAVE_GTK=no])
HAVE_GTK=no
AM_CONDITIONAL(HAVE_GTK, [test "x$HAVE_GTK" = xyes])

AC_SUBST(GTK_CFLAGS)
AC_SUBST(GTK_LIBS)
AC_SUBST(DEP_CFLAGS)
AC_SUBST(DEP_LIBS)
		  
AC_OUTPUT([pixman-1.pc
           Makefile
	   pixman/Makefile
	   test/Makefile])
