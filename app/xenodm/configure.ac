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

# Initialize Autoconf
AC_PREREQ([2.60])
AC_INIT([xenodm], [0.1],
        [https://xenocara.org], [xenodm])
AC_USE_SYSTEM_EXTENSIONS
AC_CONFIG_SRCDIR([Makefile.am])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADERS(config.h)

# Initialize Automake
AM_INIT_AUTOMAKE([foreign dist-bzip2])
AM_MAINTAINER_MODE

# Initialize libtool
AC_DISABLE_STATIC
AC_PROG_LIBTOOL

# Require X.Org macros 1.8 or later for MAN_SUBSTS set by XORG_MANPAGE_SECTIONS
m4_ifndef([XORG_MACROS_VERSION],
          [m4_fatal([must install xorg-macros 1.8 or later before running autoconf/autogen])])
XORG_MACROS_VERSION(1.8)
XORG_DEFAULT_OPTIONS
XORG_WITH_LINT

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIGNAL

# Check for system library functions
# Required system libraries have been searched for and are listed in $LIBS
AC_FUNC_FORK
AC_CHECK_FUNCS([sigaction setproctitle setusercontext \
                mkstemp getifaddrs])
AC_CHECK_LIB([bsd], [arc4random_buf])
AC_CHECK_LIB([util], [login_fbtab])

XENODM_CFLAGS="$XENODM_CFLAGS"
GREETER_CFLAGS="$GREETER_CFLAGS"

# Find needed libraries for TCP sockets (pre-pended in $LIBS)
XTRANS_CONNECTION_FLAGS

# Figure out where everything goes in the filesystem

AC_ARG_WITH(xenodmlibdir,
	AS_HELP_STRING([--with-xenodmlibdir=<pathname>],
	  [specify directory for xenodm scripts and modules (default=${libdir}/X11/xenodm)]),
	[XENODMLIBDIR="$withval"], [XENODMLIBDIR="${libdir}/X11/xenodm"])
AC_SUBST(XENODMLIBDIR)

AC_ARG_WITH(xenodmconfigdir,
	AS_HELP_STRING([--with-xenodmconfigdir=<pathname>],
	  [specify directory for xenodm config files (default is same as xenodmlibdir)]),
	[XENODMCONFIGDIR="$withval"], [XENODMCONFIGDIR="${XENODMLIBDIR}"])
AC_SUBST(XENODMCONFIGDIR)

AC_ARG_WITH(xenodmscriptdir,
	AS_HELP_STRING([--with-xenodmscriptdir=<pathname>],
	  [specify directory for xenodm scripts (default is same as xenodmlibdir)]),
	[XENODMSCRIPTDIR="$withval"], [XENODMSCRIPTDIR="${XENODMLIBDIR}"])
AC_SUBST(XENODMSCRIPTDIR)

AC_ARG_WITH(logdir,
	AS_HELP_STRING([--with-logdir=<pathname>],
	  [specify directory for xenodm log files (default=/var/log)]),
	[XENODMLOGDIR="$withval"], [XENODMLOGDIR="/var/log"])
AC_SUBST(XENODMLOGDIR)

AC_ARG_WITH(default-vt,
	AS_HELP_STRING([--with-default-vt=<vtname>],
	  [specify default vt to start xenodm on (default=none)]),
	[DEFAULTVT="$withval"], [[DEFAULTVT=""]])
AC_SUBST(DEFAULTVT)

AC_ARG_WITH(default-server,
	AS_HELP_STRING([--with-default-server=<server>],
	  [specify default server to start xenodm with (default=${bindir}/X)]),
	[DEF_SERVER="$withval"], [[DEF_SERVER="${bindir}/X"]])
AC_SUBST(DEF_SERVER)

# Ideally we'd just pull out UTMP_FILE & WTMP_FILE from <utmp.h>
# but that's not easy to do in autoconf
AC_ARG_WITH(utmp_file,
	AS_HELP_STRING([--with-utmp-file=<pathname>],
	 [specify file to pass to sessreg -u for current logins]),
	[UTMP_FILE="$withval"])
if test "x$UTMP_FILE" = "xyes" ; then
	AC_MSG_ERROR([path was not specified to --with-utmp-file])
fi
if test "x$UTMP_FILE" = "x" ; then
	for UTMP_FILE in /var/adm/utmpx /var/run/utmp /var/log/utmp /var/adm/utmp /usr/adm/utmp /etc/utmp ; do
		AC_CHECK_FILE([$UTMP_FILE], [break], [UTMP_FILE=""])
	done
	if test "x$UTMP_FILE" = "x" ; then
		UTMP_FILE="/var/run/utmp"
	fi
fi
AC_MSG_CHECKING([for path to file listing current logins for sessreg])
AC_MSG_RESULT([$UTMP_FILE])
AC_SUBST(UTMP_FILE)

AC_ARG_WITH(wtmp_file,
	AS_HELP_STRING([--with-wtmp-file=<pathname>],
	 [specify file to pass to sessreg -w for login history]),
	[WTMP_FILE="$withval"])
if test "x$WTMP_FILE" = "xyes" ; then
	AC_MSG_ERROR([path was not specified to --with-wtmp-file])
fi
if test "x$WTMP_FILE" = "x" ; then
	for WTMP_FILE in /var/adm/wtmpx /var/log/wtmp /var/adm/wtmp /usr/adm/wtmp /etc/wtmp ; do
		AC_CHECK_FILE([$WTMP_FILE], [break], [WTMP_FILE=""])
	done
	if test "x$WTMP_FILE" = "x" ; then
		WTMP_FILE="/var/log/wtmp"
	fi
fi
AC_MSG_CHECKING([for path to file listing login history for sessreg])
AC_MSG_RESULT([$WTMP_FILE])
AC_SUBST(WTMP_FILE)

case $host_os in
    *sco*|*solaris*)
		SHELL_CMD="/bin/ksh"	;;
    *)		SHELL_CMD="/bin/sh"	;;
esac
AC_SUBST(SHELL_CMD)

# Checks for pkg-config packages

PKG_CHECK_MODULES(XENODM, xmu x11 xau)

# Xinerama support
PKG_CHECK_MODULES(XINERAMA, xinerama)
GREETER_CFLAGS="$GREETER_CFLAGS $XINERAMA_CFLAGS"
GREETER_LIBS="$GREETER_LIBS $XINERAMA_LIBS"

# Xft text drawing for the greeter screen
PKG_CHECK_MODULES(XFT, xft xrender)
GREETER_CFLAGS="$GREETER_CFLAGS $XFT_CFLAGS"
GREETER_LIBS="$GREETER_LIBS $XFT_LIBS"

# XPM logos for the greeter screen
PKG_CHECK_MODULES(XPM, xpm)
GREETER_CFLAGS="$GREETER_CFLAGS $XPM_CFLAGS"
GREETER_LIBS="$GREETER_LIBS $XPM_LIBS"

AC_ARG_WITH(color-pixmap,
	AS_HELP_STRING([--with-color-pixmap=<filename>],
		[specify xpm file to use as color logo (default=xorg.xpm)]),
	[XENODM_PIXMAP="$withval"], [XENODM_PIXMAP="xorg.xpm"])
AC_ARG_WITH(bw-pixmap,
	AS_HELP_STRING([--with-bw-pixmap=<filename>],
      [specify xpm file to use as black-and-white logo (default=xorg-bw.xpm)]),
	[XENODM_BWPIXMAP="$withval"], [XENODM_BWPIXMAP="xorg-bw.xpm"])
AC_ARG_WITH(pixmapdir,
	AS_HELP_STRING([--with-pixmapdir=<pathname>],
	[specify directory for xpm files (default=xenodmlibdir/pixmaps)]),
	[XENODM_PIXMAPDIR="$withval"],
	[XENODM_PIXMAPDIR='${XENODMLIBDIR}/pixmaps'])

AC_SUBST(XENODM_PIXMAP)
AC_SUBST(XENODM_BWPIXMAP)
AC_SUBST(XENODM_PIXMAPDIR)

# Packages used by multiple programs

PKG_CHECK_MODULES(XENODM_TOOLKIT, xaw7)
PKG_CHECK_MODULES(XLIB, x11)
PKG_CHECK_MODULES(AUTH, xau)

#
# Greeter
#
PKG_CHECK_MODULES(XDMGREET, xt >= 1.0 x11 xext)

XDMGREET_CFLAGS="$XDMGREET_CFLAGS $XENODM_TOOLKIT_CFLAGS $DMCP_CFLAGS $XLIB_CFLAGS $GREETER_CFLAGS $AUTH_CFLAGS"
XDMGREET_LIBS="$XDMGREET_LIBS $XENODM_TOOLKIT_LIBS $DMCP_LIBS $GREETER_LIBS"

#
#  XENODM
#

XENODM_CFLAGS="$XENODM_CFLAGS $DMCP_CFLAGS $XLIB_CFLAGS $AUTH_CFLAGS $STATIC_GREETER_CFLAGS"
XENODM_LIBS="$XENODM_LIBS $DMCP_LIBS"

AC_SUBST(XENODM_CFLAGS)
AC_SUBST(XENODM_LIBS)


# Defaults for X session - should probably be configure --with-* flags
# Currently just trying to mirror what defaults were set in Imake

if test -z "$DEF_SERVER_LINE" ; then
	DEF_SERVER_LINE=":0 local ${DEF_SERVER} :0"
fi
AX_DEFINE_DIR(DEF_SERVER_LINE, DEF_SERVER_LINE,
	[Define to default Xserver file entry for local X server])

# -DXRDB_PROGRAM="$(BINDIR)/xrdb"
AC_ARG_WITH(xrdb, AS_HELP_STRING([--with-xrdb=path],
			[Use path for xrdb (default: bindir/xrdb)]),
	    [XRDB=$withval], [XRDB=${bindir}/xrdb])
AX_DEFINE_DIR(XRDB_PROGRAM, XRDB,
	[Define to pathname for xrdb program])

# -DDEF_SESSION="$(BINDIR)/xterm -ls"
AC_ARG_WITH(default-session, AS_HELP_STRING([--with-default-session=command],
		[Use command for default session (default: bindir/xterm -ls)]),
	    [DEF_SESSION=$withval], [DEF_SESSION="${bindir}/xterm -ls"])
AX_DEFINE_DIR(DEF_SESSION, DEF_SESSION,
	[Define to default command to start X session])

# -DDEF_USER_PATH="$(DEF_USER_PATH)"
# Default values taken from DefaultUserPath setting in the
# old Imake *.cf files for each platform
if test -z "$DEF_USER_PATH" ; then
	case $host_os in
	    dragonfly*)
		DEF_USER_PATH="/bin:/usr/bin:/sbin:/usr/sbin:${bindir}:/usr/local/bin"
		;;
	    freebsd*)
		DEF_USER_PATH="/bin:/usr/bin:/sbin:/usr/sbin:${bindir}:/usr/gnu/bin:/usr/local/bin"
		;;
	    netbsd*)
		DEF_USER_PATH="/bin:/usr/bin:/usr/pkg/bin:/usr/local/bin:${bindir}"
		;;
	    openbsd*)
		DEF_USER_PATH="/bin:/usr/bin:/sbin:/usr/sbin:${bindir}:/usr/local/bin:/usr/local/sbin"
		;;
	    bsdi*)
		DEF_USER_PATH="/bin:/usr/bin:/usr/local/bin:${bindir}"
		;;
	    solaris*)
		DEF_USER_PATH="/usr/bin:${bindir}"
		;;
	    *)
		DEF_USER_PATH="/bin:/usr/bin:${bindir}:/usr/ucb"
		;;
	esac
fi
AX_DEFINE_DIR(DEF_USER_PATH, DEF_USER_PATH,
	[Define to default PATH environment variable for user sessions])

# -DDEF_SYSTEM_PATH="$(DEF_SYSTEM_PATH)"
# Default values taken from DefaultSystemPath setting in the
# old Imake *.cf files for each platform
if test -z "$DEF_SYSTEM_PATH" ; then
	case $host_os in
	    freebsd*|dragonfly*)
		DEF_SYSTEM_PATH="/bin:/usr/bin:/sbin:/usr/sbin:${bindir}"
		;;
	    netbsd*|openbsd*|bsdi*)
		DEF_SYSTEM_PATH="/sbin:/usr/sbin:/bin:/usr/bin:${bindir}"
		;;
	    solaris*)
		DEF_SYSTEM_PATH="/sbin:/usr/sbin:/usr/bin:${bindir}"
		;;
	    *)
		DEF_SYSTEM_PATH="/etc:/bin:/usr/bin:${bindir}:/usr/ucb"
		;;
	esac
fi
AX_DEFINE_DIR(DEF_SYSTEM_PATH, DEF_SYSTEM_PATH,
	[Define to default PATH environment variable for system commands])

# -DDEF_SYSTEM_SHELL="$(BOURNE_SHELL)"
if test -z "$BOURNE_SHELL" ; then
	BOURNE_SHELL=/bin/sh
fi
AX_DEFINE_DIR(DEF_SYSTEM_SHELL, BOURNE_SHELL,
	[Define to default shell])

# -DDEF_FAILSAFE_CLIENT="$(BINDIR)/xterm"

AX_DEFINE_DIR(DEF_FAILSAFE_CLIENT, bindir/xterm,
	[Define to default command to start failsafe X session])

# -DDEF_XENODM_CONFIG="$(XENODMDIR)/xenodm-config"

AX_DEFINE_DIR(DEF_XENODM_CONFIG, XENODMCONFIGDIR/xenodm-config,
	[Define to pathname of default xenodm-config file])

# -DDEF_AUTH_DIR="$(XENODMDIR)"

AC_ARG_WITH(authdir,
	AS_HELP_STRING([--with-authdir=<pathname>],
	  [specify directory for X auth files (default=/var/lib/xenodm)]),
	[XENODMXAUTHDIR="$withval"], [XENODMXAUTHDIR="/var/lib/xenodm"])
AC_SUBST(XENODMXAUTHDIR)

AX_DEFINE_DIR(DEF_AUTH_DIR, XENODMXAUTHDIR,
	[Define to pathname where authentication cookie files should be stored])

PKG_CHECK_MODULES(APPDEFS, xt)
xt_appdefaultdir=`$PKG_CONFIG --variable=appdefaultdir xt`
AC_ARG_WITH(appdefaultdir,
	AS_HELP_STRING([--with-appdefaultdir=<pathname>],
	  [specify directory for app-defaults files (default is autodetected)]),
	[appdefaultdir="$withval"], [appdefaultdir="${xt_appdefaultdir}"])
AC_SUBST(appdefaultdir)

AC_SUBST(MKTEMP_COMMAND)

AC_CONFIG_FILES([Makefile
		config/Makefile
		greeter/Makefile
		include/Makefile
		man/Makefile
		xenodm/Makefile
		config/GiveConsole
		config/TakeConsole
		config/Xreset
		config/Xresources
		config/Xservers
		config/Xsession
		config/Xsetup_0
		config/Xstartup
		config/xenodm-config])
AC_OUTPUT
