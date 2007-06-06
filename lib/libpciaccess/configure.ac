
dnl (C) Copyright IBM Corporation 2006
dnl All Rights Reserved.
dnl
dnl Permission is hereby granted, free of charge, to any person obtaining a
dnl copy of this software and associated documentation files (the "Software"),
dnl to deal in the Software without restriction, including without limitation
dnl on the rights to use, copy, modify, merge, publish, distribute, sub
dnl license, and/or sell copies of the Software, and to permit persons to whom
dnl the Software is furnished to do so, subject to the following conditions:
dnl
dnl The above copyright notice and this permission notice (including the next
dnl paragraph) shall be included in all copies or substantial portions of the
dnl Software.
dnl
dnl THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
dnl IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
dnl FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.  IN NO EVENT SHALL
dnl IBM AND/OR THEIR SUPPLIERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
dnl LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
dnl FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
dnl DEALINGS IN THE SOFTWARE.
dnl
dnl Process this file with autoconf to create configure.

# AC_DEFINE_DIR macro from autoconf-archive.cryp.to
AC_DEFUN([AC_DEFINE_DIR], [
  prefix_NONE=
  exec_prefix_NONE=
  test "x$prefix" = xNONE && prefix_NONE=yes && prefix=$ac_default_prefix
  test "x$exec_prefix" = xNONE && exec_prefix_NONE=yes && exec_prefix=$prefix
dnl In Autoconf 2.60, ${datadir} refers to ${datarootdir}, which in turn
dnl refers to ${prefix}.  Thus we have to use `eval' twice.
  eval ac_define_dir="\"[$]$2\""
  eval ac_define_dir="\"$ac_define_dir\""
  AC_SUBST($1, "$ac_define_dir")
  AC_DEFINE_UNQUOTED($1, "$ac_define_dir", [$3])
  test "$prefix_NONE" && prefix=NONE
  test "$exec_prefix_NONE" && exec_prefix=NONE
])

AC_PREREQ([2.57])

AC_INIT(libpciaccess, 0.8.0, [none yet], libpciaccess)
AM_INIT_AUTOMAKE([dist-bzip2])
AM_MAINTAINER_MODE

AM_CONFIG_HEADER(config.h)

# Check for progs
AC_PROG_CC
AC_PROG_LIBTOOL

m4_ifdef([AS_HELP_STRING], , [m4_define([AS_HELP_STRING], m4_defn([AC_HELP_STRING]))])

DEFAULT_PCIIDS_PATH=/usr/share/hwdata
AC_ARG_WITH(pciids-path,
	AS_HELP_STRING([--with-pciids-path=PCIIDS_PATH], [Path to pci.ids file]),
	[PCIIDS_PATH="$withval"],
	[PCIIDS_PATH="$DEFAULT_PCIIDS_PATH"])
AC_DEFINE_DIR(PCIIDS_PATH, PCIIDS_PATH, [Path to pci.ids])

if test "x$GCC" = "xyes"; then
       GCC_WARNINGS1="-Wall -Wpointer-arith -Wstrict-prototypes"
       GCC_WARNINGS2="-Wmissing-prototypes -Wmissing-declarations"
       GCC_WARNINGS3="-Wnested-externs"
       GCC_WARNINGS="$GCC_WARNINGS1 $GCC_WARNINGS2 $GCC_WARNINGS3"
       if test "x$WERROR" = "xyes"; then
               GCC_WARNINGS="${GCC_WARNINGS} -Werror"
       fi
       PCIACCESS_CFLAGS="$PCIACCESS_CFLAGS $GCC_WARNINGS"
fi

case $host_os in
	*freebsd*)
		freebsd=yes
		;;
	*linux*)
		linux=yes
		;;
	*openbsd*)
		openbsd=yes
		;;
esac

AM_CONDITIONAL(LINUX, [test "x$linux" = xyes])
AM_CONDITIONAL(FREEBSD, [test "x$freebsd" = xyes])
AM_CONDITIONAL(OPENBSD, [test "x$openbsd" = xyes])

AC_SUBST(PCIACCESS_CFLAGS)
AC_SUBST(PCIACCESS_LIBS)
		  
XORG_RELEASE_VERSION

AC_OUTPUT([Makefile
	   src/Makefile
           pciaccess.pc])
