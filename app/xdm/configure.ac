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
AC_INIT([xdm], [1.1.11],
        [https://bugs.freedesktop.org/enter_bug.cgi?product=xorg], [xdm])
AC_USE_SYSTEM_EXTENSIONS
AC_CONFIG_SRCDIR([Makefile.am])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADERS(config.h)

# Initialize Automake
AM_INIT_AUTOMAKE([foreign dist-bzip2])
AM_MAINTAINER_MODE

# Initialize libtool
AC_DISABLE_STATIC
AC_LIBTOOL_DLOPEN
AC_PROG_LIBTOOL

# Require X.Org macros 1.8 or later for MAN_SUBSTS set by XORG_MANPAGE_SECTIONS
m4_ifndef([XORG_MACROS_VERSION],
          [m4_fatal([must install xorg-macros 1.8 or later before running autoconf/autogen])])
XORG_MACROS_VERSION(1.8)
XORG_DEFAULT_OPTIONS
XORG_PROG_RAWCPP
XORG_WITH_LINT

# Checks for programs.
# Check for mktemp to prevent security issue noted in Xorg bug #5898
# See https://bugs.freedesktop.org/show_bug.cgi?id=5898 for details.
AC_PATH_PROG(MKTEMP_COMMAND, mktemp)
AM_CONDITIONAL(HAVE_MKTEMP_COMMAND, test x$MKTEMP_COMMAND != x)

#------------------------------------------------------------------------------
#			Check for system libraries
#
# AC_SEARCH_LIBS: tests using $LIBS and tests again with each listed library.
#                 pre-pends the found library to $LIBS.
#
#------------------------------------------------------------------------------
# Function getifaddrs() comes from the C library, Solaris 11 -lsocket, or
# the -lnsl name service library
AC_SEARCH_LIBS([getifaddrs], [socket nsl])

# Function getspnam() comes from the C library, System V -lsec, UnixWare 7 -lgen
# or Old Linux versions -lshadow
AC_SEARCH_LIBS([getspnam], [sec gen shadow])

# Function setproctitle() is in BSD -lutil or GNU systems -lbsd but N/A on Solaris
AC_SEARCH_LIBS([setproctitle], [util bsd])

# Function setusercontext() is in BSD -lutil but N/A on Solaris or GNU systems
AC_SEARCH_LIBS([setusercontext], [util])

# Function arc4random() is in BSD standard C or GNU systems -lbsd
AC_SEARCH_LIBS([arc4random], [bsd], [HAVE_ARC4RANDOM="yes"])
AC_SUBST(HAVE_ARC4RANDOM)
AM_CONDITIONAL(HAVE_ARC4RANDOM, test x$HAVE_ARC4RANDOM = xyes)

# Function crypt() comes from the C library or the -lcrypt data encryption library
AC_SEARCH_LIBS(crypt, crypt)

# Function dl() comes from the C library or -ldl
AC_SEARCH_LIBS([dlopen], [dl])
#------------------------------------------------------------------------------

# Checks for header files.
AC_CHECK_HEADERS([grp.h syslog.h sys/param.h])
AC_CHECK_DECL(key_setnet,
	AC_DEFINE(HAVE_DECL_KEY_SETNET,1,
	     [Define to 1 if your headers define the key_setnet() function.]),
	[],[
#include <rpc/rpc.h>
#include <rpc/key_prot.h>
])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIGNAL

# Check for system library functions
# Required system libraries have been searched for and are listed in $LIBS
AC_FUNC_FORK
AC_CHECK_FUNCS([daemon sigaction openlog asprintf setproctitle setusercontext \
                mkstemp getifaddrs getspnam arc4random])

XDM_CFLAGS="$XDM_CFLAGS"
GREETER_CFLAGS="$GREETER_CFLAGS"
CHOOSER_CFLAGS="$CHOOSER_CFLAGS"

# Find needed libraries for TCP sockets (pre-pended in $LIBS)
XTRANS_CONNECTION_FLAGS

# Sets $SECURE_RPC to "yes" or "no" using libraries found above from $LIBS
XTRANS_SECURE_RPC_FLAGS
AM_CONDITIONAL(HAS_SECURE_RPC, test x$SECURE_RPC = xyes)

# Check for pluggable authentication modules (PAM) support
AC_ARG_WITH(pam, AS_HELP_STRING([--with-pam],
	[Use PAM for authentication (default is autodetected)]),
	[USE_PAM=$withval], [USE_PAM=auto])
if test "x$USE_PAM" != "xno" ; then
    AC_SEARCH_LIBS([pam_open_session], [pam],
	[AC_CHECK_FUNC([pam_open_session],
	    [AC_DEFINE(USE_PAM,1,[Use PAM for authentication])]
	)],
	[AS_IF([test "x$USE_PAM" = "xyes"],
	    [AC_MSG_ERROR([PAM support requested, but pam_open_session not found.])]
	)]
    )
fi

# Check for Security-Enhanced Linux (SELinux) support
AC_ARG_WITH(selinux, AS_HELP_STRING([--with-selinux],
	[Add support for SELinux (default is no support)]),
	[USE_SELINUX=$withval], [USE_SELINUX=no])
if test "x$USE_SELINUX" != "xno" ; then
    AC_SEARCH_LIBS(is_selinux_enabled, [selinux],
	[AC_CHECK_FUNC([is_selinux_enabled],
	    [AC_DEFINE(USE_SELINUX,1,[Use Security-Enhanced Linux (SELinux) support])]
	)],
	[AS_IF([test "x$USE_SELINUX" = "xyes"],
	    [AC_MSG_ERROR([SELinux support requested, but is_selinux_enabled not found.])]
	)]
    )
fi

# Check whether to install systemd unit files, as suggested in daemon(7).
# When a full path is specified, this does not require systemd installed.
AC_ARG_WITH([systemdsystemunitdir], AS_HELP_STRING([--with-systemdsystemunitdir=DIR],
	[Directory for systemd service files (default from the System and Service Manager)]),,
	[with_systemdsystemunitdir=auto])
AS_IF([test "x$with_systemdsystemunitdir" = "xyes" -o "x$with_systemdsystemunitdir" = "xauto"], [
    def_systemdsystemunitdir=$($PKG_CONFIG --variable=systemdsystemunitdir systemd)

    AS_IF([test "x$def_systemdsystemunitdir" = "x"], [
	AS_IF([test "x$with_systemdsystemunitdir" = "xyes"], [
	    AC_MSG_ERROR([systemd support requested but pkg-config unable to query systemd package])
	])
	with_systemdsystemunitdir=no
    ], [
	with_systemdsystemunitdir=$def_systemdsystemunitdir
    ])
])
AS_IF([test "x$with_systemdsystemunitdir" != "xno"], [
	AC_SUBST([systemdsystemunitdir], [$with_systemdsystemunitdir])
])
AM_CONDITIONAL(HAVE_SYSTEMD, [test "x$with_systemdsystemunitdir" != "xno"])

# Check whether to enable systemd startup notification.
# This requires libsystemd-daemon.
AC_ARG_WITH([systemd-daemon], AS_HELP_STRING([--with-systemd-daemon],
	[Add support for systemd startup notification (default is autodetected)]),
	[USE_SYSTEMD_DAEMON=$withval], [USE_SYSTEMD_DAEMON=auto])
AS_IF([test "x$USE_SYSTEMD_DAEMON" != "xno"], [
    PKG_CHECK_MODULES([SYSTEMD_DAEMON], [libsystemd-daemon],
	[AC_DEFINE(USE_SYSTEMD_DAEMON,1,[Use systemd startup notification])],
	[AS_IF([test "x$USE_SYSTEMD_DAEMON" = "xyes"],
	    [AC_MSG_ERROR([systemd startup notification support requested, but libsystemd-daemon not found.])]
	)]
    )
])
AM_CONDITIONAL(USE_SYSTEMD_DAEMON, [test "x$USE_SYSTEMD_DAEMON" != "xno"])

# FIXME: Find better test for which OS'es use su -m  - for now, just try to
# mirror the Imakefile setting of:
# if  defined(OpenBSDArchitecture) || defined(NetBSDArchitecture) || defined(FreeBSDArchitecture) || defined(DarwinArchitecture)


case $host_os in
    linux*|gnu*|*-gnu)
	SU="su"
        ;;
    darwin*)
	SU="su -m"
        ;;
    *bsd*|dragonfly*)
	SU="su -m"
        ;;
    solaris*|sysv4*)
	SU="su"
	;;
    *)
	SU="su"
        ;;
esac
AC_SUBST(SU)

# Define a configure option to locate a special file (/dev/random or /dev/urandom)
# that serves as a random or a pseudorandom number generator
AC_ARG_WITH(random-device, AS_HELP_STRING([--with-random-device\[=<pathname>\]],
	[Use <pathname> as a source of randomness (default is auto-detected)]),
	[USE_DEVICE="$withval"], [USE_DEVICE="auto"])
if test x$USE_DEVICE != xno ; then
    if test x$USE_DEVICE = xyes -o x$USE_DEVICE = xauto ; then
	AC_CHECK_FILE([/dev/urandom], [DEV_RANDOM=/dev/urandom],
	    [AC_CHECK_FILE([/dev/random], [DEV_RANDOM=/dev/random],
		[AS_IF([test "x$USE_DEVICE" = "xyes"],
		    [AC_MSG_ERROR([random device support requested, but no random device was found.])]
		)]
	    )]
	)
    else
	# assume the user input is a valid path to a random number generator
	DEV_RANDOM=$USE_DEVICE
    fi
    if test x$DEV_RANDOM != x ; then
	AC_DEFINE_UNQUOTED(DEV_RANDOM,"$DEV_RANDOM", [Define to device that provides random data source])
    fi
fi
AC_SUBST(DEV_RANDOM)

# FIXME: find out how to check for these things. Note that Kerberos was
# turned off everywhere but on OpenBSD.
AM_CONDITIONAL(HAS_KERBEROS_FIVE, false)

#
# Privsep
#
AC_ARG_ENABLE(privsep,
	AC_HELP_STRING([--enable-privsep],
         [Build support for X server privilege separation (default is NO)]),
	[ENABLE_PRIVSEP="$enableval"], [ENABLE_PRIVSEP="no"])
if test x$ENABLE_PRIVSEP = xyes ; then
	AC_DEFINE(X_PRIVSEP, 1, [Use X server privilege separation])
fi

#
# BSD auth
AC_ARG_ENABLE(bsdauth,
	AC_HELP_STRING([--enable-bsdauth],
	 [Build support for BSD authentification (default is NO)]),
	[ENABLE_BSDAUTH="$enableval"], [ENABLE_BSDAUTH="no"])
if test x$ENABLE_BSDAUTH = xyes ; then
	AC_DEFINE(USE_BSDAUTH, 1, [Use BSD authentification])
fi

# Figure out where everything goes in the filesystem

AC_ARG_WITH(xdmlibdir,
	AS_HELP_STRING([--with-xdmlibdir=<pathname>],
	  [specify directory for xdm scripts and modules (default=${libdir}/X11/xdm)]),
	[XDMLIBDIR="$withval"], [XDMLIBDIR="${libdir}/X11/xdm"])
AC_SUBST(XDMLIBDIR)

AC_ARG_WITH(xdmconfigdir,
	AS_HELP_STRING([--with-xdmconfigdir=<pathname>],
	  [specify directory for xdm config files (default is same as xdmlibdir)]),
	[XDMCONFIGDIR="$withval"], [XDMCONFIGDIR="${XDMLIBDIR}"])
AC_SUBST(XDMCONFIGDIR)

AC_ARG_WITH(xdmscriptdir,
	AS_HELP_STRING([--with-xdmscriptdir=<pathname>],
	  [specify directory for xdm scripts (default is same as xdmlibdir)]),
	[XDMSCRIPTDIR="$withval"], [XDMSCRIPTDIR="${XDMLIBDIR}"])
AC_SUBST(XDMSCRIPTDIR)

AC_ARG_WITH(logdir,
	AS_HELP_STRING([--with-logdir=<pathname>],
	  [specify directory for xdm log files (default=/var/log)]),
	[XDMLOGDIR="$withval"], [XDMLOGDIR="/var/log"])
AC_SUBST(XDMLOGDIR)

AC_ARG_WITH(piddir,
	AS_HELP_STRING([--with-piddir=<pathname>],
	  [specify directory for xdm pid files (default=/var/run)]),
	[XDMPIDDIR="$withval"], [XDMPIDDIR="/var/run"])
AC_SUBST(XDMPIDDIR)

AC_ARG_WITH(default-vt,
	AS_HELP_STRING([--with-default-vt=<vtname>],
	  [specify default vt to start xdm on (default=none)]),
	[DEFAULTVT="$withval"], [[DEFAULTVT=""]])
AC_SUBST(DEFAULTVT)

AC_ARG_WITH(config-type,
	AS_HELP_STRING([--with-config-type={ws|fs}],
	 [default config file type to use - workstation (ws) or fileserver (fs) (default=ws)]),
	[SERVERSTYPE="$withval"], [SERVERSTYPE="ws"])
AC_SUBST(SERVERSTYPE)

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

PKG_CHECK_MODULES(XDM, xmu x11 xau)

PKG_CHECK_EXISTS(xinerama, [
	AC_DEFINE([USE_XINERAMA], 1,
		[Define to 1 to use XINERAMA in greeter & chooser])
	PKG_CHECK_MODULES(XINERAMA, xinerama)
	CHOOSER_CFLAGS="$CHOOSER_CFLAGS $XINERAMA_CFLAGS"
	CHOOSER_LIBS="$CHOOSER_LIBS $XINERAMA_LIBS"
	GREETER_CFLAGS="$GREETER_CFLAGS $XINERAMA_CFLAGS"
	GREETER_LIBS="$GREETER_LIBS $XINERAMA_LIBS"
])

# Xft text drawing for the greeter screen
AC_ARG_WITH(xft,
	AS_HELP_STRING([--with-xft],
			[Use Xft to draw text (default is YES if installed)]),
	[USE_XFT="$withval"],
	PKG_CHECK_EXISTS(xft xrender, [USE_XFT="yes"], [USE_XFT="no"]))

if test "x$USE_XFT" = "xyes" ; then
	PKG_CHECK_MODULES(XFT, xft xrender)
	GREETER_CFLAGS="$GREETER_CFLAGS $XFT_CFLAGS"
	GREETER_LIBS="$GREETER_LIBS $XFT_LIBS"
	AC_DEFINE([USE_XFT], 1,
	    [Define to 1 to use Xft for text on greeter screen])
fi

# XPM logos for the greeter screen
AC_ARG_ENABLE(xpm-logos,
	AS_HELP_STRING([--enable-xpm-logos],
			[Display xpm logos in greeter (default is YES)]),
	[USE_XPM="$enableval"],
	PKG_CHECK_EXISTS(xpm, [USE_XPM="yes"], [USE_XPM="no"]))
if test "x$USE_XPM" = "xyes" ; then
	PKG_CHECK_MODULES(XPM, xpm)
	GREETER_CFLAGS="$GREETER_CFLAGS $XPM_CFLAGS"
	GREETER_LIBS="$GREETER_LIBS $XPM_LIBS"
	AC_DEFINE([XPM], 1,
		[Define to 1 to display XPM logos on greeter screen])
fi
AM_CONDITIONAL(USE_XPM, test "x$USE_XPM" = "xyes")

AC_ARG_WITH(color-pixmap,
	AS_HELP_STRING([--with-color-pixmap=<filename>],
		[specify xpm file to use as color logo (default=xorg.xpm)]),
	[XDM_PIXMAP="$withval"], [XDM_PIXMAP="xorg.xpm"])
AC_ARG_WITH(bw-pixmap,
	AS_HELP_STRING([--with-bw-pixmap=<filename>],
      [specify xpm file to use as black-and-white logo (default=xorg-bw.xpm)]),
	[XDM_BWPIXMAP="$withval"], [XDM_BWPIXMAP="xorg-bw.xpm"])
AC_ARG_WITH(pixmapdir,
	AS_HELP_STRING([--with-pixmapdir=<pathname>],
	[specify directory for xpm files (default=xdmlibdir/pixmaps)]),
	[XDM_PIXMAPDIR="$withval"],
	[XDM_PIXMAPDIR='${XDMLIBDIR}/pixmaps'])

AC_SUBST(XDM_PIXMAP)
AC_SUBST(XDM_BWPIXMAP)
AC_SUBST(XDM_PIXMAPDIR)

# Packages used by multiple programs

PKG_CHECK_MODULES(XDM_TOOLKIT, xaw7)
PKG_CHECK_MODULES(DMCP, xdmcp)
PKG_CHECK_MODULES(XLIB, x11)
PKG_CHECK_MODULES(AUTH, xau)

#
# Greeter
#

AC_MSG_CHECKING([if the greeter should be built as a dynamically loaded object])

AC_ARG_ENABLE(dynamic-greeter,
	AC_HELP_STRING([--enable-dynamic-greeter],
         [Build greeter as dynamically loaded shared object]),
	[DYNAMIC_GREETER="$enableval"], [DYNAMIC_GREETER="auto"])

if test "x$DYNAMIC_GREETER" = "xauto" ; then
	case $host_os in
	openbsd*)
		DYNAMIC_GREETER="no"
		;;
	    *)	DYNAMIC_GREETER="yes" ;;

	esac
fi
if test "x$DYNAMIC_GREETER" = "xno"; then
	STATIC_GREETER_CFLAGS="-DGREET_USER_STATIC"
fi

AC_MSG_RESULT([$DYNAMIC_GREETER])
AM_CONDITIONAL(DYNAMIC_GREETER, test x$DYNAMIC_GREETER = xyes)

PKG_CHECK_MODULES(XDMGREET, xt >= 1.0 x11 xext)

GREETERLIB="${XDMLIBDIR}/libXdmGreet.so"
AX_DEFINE_DIR(DEF_GREETER_LIB, GREETERLIB, [Location of Xdm Greeter library])

XDMGREET_CFLAGS="$XDMGREET_CFLAGS $XDM_TOOLKIT_CFLAGS $DMCP_CFLAGS $XLIB_CFLAGS $GREETER_CFLAGS $AUTH_CFLAGS"
XDMGREET_LIBS="$XDMGREET_LIBS $XDM_TOOLKIT_LIBS $DMCP_LIBS $GREETER_LIBS"

#
#  XDM
#

XDM_CFLAGS="$XDM_CFLAGS $DMCP_CFLAGS $XLIB_CFLAGS $AUTH_CFLAGS $STATIC_GREETER_CFLAGS"
XDM_LIBS="$XDM_LIBS $DMCP_LIBS"

AC_CHECK_LIB(Xdmcp, XdmcpWrap, [xdmauth="yes"], [xdmauth="no"], [$DMCP_LIBS])

AC_ARG_ENABLE(xdm-auth,
	AS_HELP_STRING([--enable-xdm-auth],
       [Support XDM-AUTHENTICATION-1 and XDM-AUTHORIZATION-1 (default: auto)]),
	[xdmauth_enable="$enableval"], [xdmauth_enable="auto"])
if test "x$xdmauth_enable" = "xyes" -a "x$xdmauth" = "xno" ; then
	AC_MSG_ERROR([XDM-AUTH support requested, but XdmcpWrap not found in libXdmcp.])
fi

AM_CONDITIONAL(HAS_XDM_AUTH, test x$xdmauth = xyes -a x$xdmauth_enable != xno)

AC_SUBST(XDM_CFLAGS)
AC_SUBST(XDM_LIBS)

#
# xdmshell
#

AC_ARG_ENABLE(xdmshell,
	AS_HELP_STRING([--enable-xdmshell],
         [Install xdmshell (default: disabled)]),
	[INSTALL_XDMSHELL="$enableval"], [INSTALL_XDMSHELL="no"])
AM_CONDITIONAL(INSTALL_XDMSHELL, test x$INSTALL_XDMSHELL = xyes)

PKG_CHECK_MODULES(XT, xt)
XDMSHELL_CFLAGS="$XDM_CFLAGS $XDM_TOOLKIT_CFLAGS $AUTH_CFLAGS $DMCP_CFLAGS $XLIB_CFLAGS $XT_CFLAGS"
XDMSHELL_LIBS="$XDM_LIBS $XDM_TOOLKIT_LIBS $DMCP_LIBS"

AC_SUBST(XDMSHELL_CFLAGS)
AC_SUBST(XDMSHELL_LIBS)

#
# Chooser
#

CHOOSER_CFLAGS="$CHOOSER_CFLAGS $XDM_TOOLKIT_CFLAGS $XLIB_CFLAGS $DMCP_CFLAGS $AUTH_CFLAGS"
CHOOSER_LIBS="$CHOOSER_LIBS $XDM_TOOLKIT_LIBS $DMCP_LIBS"

AC_SUBST(CHOOSER_CFLAGS)
AC_SUBST(CHOOSER_LIBS)

# Defaults for X session - should probably be configure --with-* flags
# Currently just trying to mirror what defaults were set in Imake

# -DDEF_SERVER_LINE=":0 local $(DEF_SERVER) :0"
if test -z "$DEF_SERVER" ; then
	DEF_SERVER="${bindir}/X"
fi
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

# -DDEF_XDM_CONFIG="$(XDMDIR)/xdm-config"

AX_DEFINE_DIR(DEF_XDM_CONFIG, XDMCONFIGDIR/xdm-config,
	[Define to pathname of default xdm-config file])

# -DDEF_CHOOSER="$(CHOOSERPATH)"

AX_DEFINE_DIR(DEF_CHOOSER, XDMLIBDIR/chooser,
	[Define to pathname of where chooser binary is installed])

# -DDEF_AUTH_DIR="$(XDMDIR)"

AC_ARG_WITH(authdir,
	AS_HELP_STRING([--with-authdir=<pathname>],
	  [specify directory for X auth files (default=/var/lib/xdm)]),
	[XDMXAUTHDIR="$withval"], [XDMXAUTHDIR="/var/lib/xdm"])
AC_SUBST(XDMXAUTHDIR)

AX_DEFINE_DIR(DEF_AUTH_DIR, XDMXAUTHDIR,
	[Define to pathname where authentication cookie files should be stored])

# -DDEF_GREETER_LIB="$(XDMDIR)/libXdmGreet.so"

AX_DEFINE_DIR(DEF_GREETER_LIB, XDMLIBDIR/libXdmGreet.so,
	[Define to pathname where greeter shared object is installed (if used)])

PKG_CHECK_MODULES(APPDEFS, xt)
xt_appdefaultdir=`$PKG_CONFIG --variable=appdefaultdir xt`
AC_ARG_WITH(appdefaultdir,
	AS_HELP_STRING([--with-appdefaultdir=<pathname>],
	  [specify directory for app-defaults files (default is autodetected)]),
	[appdefaultdir="$withval"], [appdefaultdir="${xt_appdefaultdir}"])
AC_SUBST(appdefaultdir)

AC_SUBST(MKTEMP_COMMAND)

AC_CONFIG_FILES([Makefile
		app-defaults/Makefile
		chooser/Makefile
		config/Makefile
		greeter/Makefile
		include/Makefile
		man/Makefile
		xdm/Makefile
		xdmshell/Makefile])
AC_OUTPUT
