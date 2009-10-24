# $XdotOrg: app/xhost/configure.ac,v 1.10 2005/12/15 00:24:07 kem Exp $

AC_PREREQ([2.57])
AC_INIT(xhost,[1.0.3], [https://bugs.freedesktop.org/enter_bug.cgi?product=xorg],xhost)
AM_INIT_AUTOMAKE([dist-bzip2])
AM_MAINTAINER_MODE

# Require xorg-macros 1.3 or later: XORG_DEFAULT_OPTIONS
m4_ifndef([XORG_MACROS_VERSION],
	  [m4_fatal([must install xorg-macros 1.3 or later before running autoconf/autogen])])
XORG_MACROS_VERSION(1.3)

AM_CONFIG_HEADER(config.h)

AC_PROG_CC
AC_PROG_INSTALL

AC_TYPE_SIGNAL

XORG_DEFAULT_OPTIONS

# Checks for pkg-config packages
PKG_CHECK_MODULES(XHOST, x11 xmuu xau)
AC_SUBST(XHOST_CFLAGS)
AC_SUBST(XHOST_LIBS)

# Transport selection macro from xtrans.m4
XTRANS_CONNECTION_FLAGS

# Secure RPC detection macro from xtrans.m4
XTRANS_SECURE_RPC_FLAGS

AC_OUTPUT([Makefile])
