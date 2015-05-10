# (C) Copyright IBM Corporation 2006
# All Rights Reserved.
#
# Permission is hereby granted, free of charge, to any person obtaining a
# copy of this software and associated documentation files (the "Software"),
# to deal in the Software without restriction, including without limitation
# on the rights to use, copy, modify, merge, publish, distribute, sub
# license, and/or sell copies of the Software, and to permit persons to whom
# the Software is furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice (including the next
# paragraph) shall be included in all copies or substantial portions of the
# Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.  IN NO EVENT SHALL
# IBM AND/OR THEIR SUPPLIERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
# FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
# DEALINGS IN THE SOFTWARE.
#

# Initialize Autoconf
AC_PREREQ([2.60])
AC_INIT([libpciaccess],[0.13.4],
        [https://bugs.freedesktop.org/enter_bug.cgi?product=xorg&component=libpciaccess],[libpciaccess])
AC_CONFIG_SRCDIR([Makefile.am])
AC_CONFIG_HEADERS([config.h])

# Initialize Automake
AM_INIT_AUTOMAKE([foreign dist-bzip2])
AC_USE_SYSTEM_EXTENSIONS

# Initialize libtool
AC_PROG_LIBTOOL

# Require X.Org macros 1.8 or later
m4_ifndef([XORG_MACROS_VERSION],
          [m4_fatal([must install xorg-macros 1.8 or later before running autoconf/autogen])])
XORG_MACROS_VERSION(1.8)
XORG_DEFAULT_OPTIONS

pciids_path=/usr/share/hwdata
AC_ARG_WITH(pciids-path, AS_HELP_STRING([--with-pciids-path=PCIIDS_PATH],
	[Path to pci.ids file]), [pciids_path="$withval"])
AX_DEFINE_DIR(PCIIDS_PATH, pciids_path, [Path to pci.ids])

AC_ARG_ENABLE(linux-rom-fallback, AS_HELP_STRING([--enable-linux-rom-fallback],
		[Enable support for falling back to /dev/mem for roms (default: disabled)]),
		[LINUX_ROM=$enableval],[LINUX_ROM=no])

if test "x$LINUX_ROM" = xyes; then
	AC_DEFINE(LINUX_ROM, 1, [Linux ROM read fallback])
fi

use_zlib=no
AC_ARG_WITH(zlib, AS_HELP_STRING([--with-zlib],
	[Enable zlib support to read gzip compressed pci.ids]),
	[use_zlib="$withval"])
if test "x$use_zlib" = xyes; then
	AC_CHECK_LIB(z, gzopen,
	[PCIACCESS_LIBS="$PCIACCESS_LIBS -lz"],
	[AC_MSG_ERROR(Check for zlib library failed)])
	AC_CHECK_HEADER([zlib.h],
	[AC_DEFINE(HAVE_ZLIB, 1, [Use zlib to read gzip compressed pci.ids])],
	[AC_MSG_ERROR(Check for zlib.h header file failed)])
fi

case $host_os in
	*freebsd* | *dragonfly*)
		freebsd=yes
		;;
	*linux*)
		linux=yes
		;;
	*netbsd*)
		case $host in
		*i[3-9]86*)
			PCIACCESS_LIBS="$PCIACCESS_LIBS -li386"
			;;
		*x86_64*|*amd64*)
			PCIACCESS_LIBS="$PCIACCESS_LIBS -lx86_64"
			;;
		*alpha*)
			PCIACCESS_LIBS="$PCIACCESS_LIBS -lalpha"
			;;
		esac
		PCIACCESS_LIBS="$PCIACCESS_LIBS -lpci"
		netbsd=yes
		;;
	*openbsd*)
		openbsd=yes
		;;
	*solaris*)
		solaris=yes
		PCIACCESS_LIBS="$PCIACCESS_LIBS -ldevinfo"
		;;
	gnu*)
		gnu=yes
		;;
	*cygwin*)
		cygwin=yes
		PCIACCESS_LIBS="$PCIACCESS_LIBS"
		;;
esac

AM_CONDITIONAL(LINUX, [test "x$linux" = xyes])
AM_CONDITIONAL(FREEBSD, [test "x$freebsd" = xyes])
AM_CONDITIONAL(NETBSD, [test "x$netbsd" = xyes])
AM_CONDITIONAL(OPENBSD, [test "x$openbsd" = xyes])
AM_CONDITIONAL(SOLARIS, [test "x$solaris" = xyes])
AM_CONDITIONAL(GNU, [test "x$gnu" = xyes])
AM_CONDITIONAL(CYGWIN, [test "x$cygwin" = xyes])

AC_SYS_LARGEFILE

AC_CHECK_HEADERS([err.h])

if test "x$netbsd" = xyes; then
AC_CHECK_HEADERS([machine/sysarch.h])
AC_CHECK_HEADERS([machine/mtrr.h], [have_mtrr_h="yes"], [have_mtrr_h="no"],
[#ifdef HAVE_MACHINE_SYSARCH_H
#include <sys/types.h>
#include <machine/sysarch.h>
#endif
])
else
AC_CHECK_HEADER([asm/mtrr.h], [have_mtrr_h="yes"], [have_mtrr_h="no"])
fi

if test "x$have_mtrr_h" = xyes; then
    AC_DEFINE(HAVE_MTRR, 1, [Use MTRRs on mappings])
fi

# check for the pci_io.pi_sel.pc_domain
AC_CHECK_MEMBER([struct pci_io.pi_sel.pc_domain],
               [AC_DEFINE(HAVE_PCI_IO_PC_DOMAIN,1,[Have the pci_io.pi_sel.pc_domain member.])],
               [],
               [ #include <sys/types.h>
                 #include <sys/pciio.h>
               ])

AC_SUBST(PCIACCESS_CFLAGS)
AC_SUBST(PCIACCESS_LIBS)

AC_CONFIG_FILES([Makefile
		include/Makefile
		man/Makefile
		src/Makefile
		scanpci/Makefile
		pciaccess.pc])
AC_OUTPUT
