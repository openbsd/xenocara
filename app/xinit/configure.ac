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
AC_INIT([xinit], [1.3.4],
        [https://bugs.freedesktop.org/enter_bug.cgi?product=xorg], [xinit])
AC_CONFIG_SRCDIR([Makefile.am])
AC_CONFIG_HEADERS([config.h])

# Initialize Automake
AM_INIT_AUTOMAKE([foreign dist-bzip2])
AM_MAINTAINER_MODE

# Require X.Org macros 1.19 or later for TRADITIONALCPPFLAGS
m4_ifndef([XORG_MACROS_VERSION],
          [m4_fatal([must install xorg-macros 1.19 or later before running autoconf/autogen])])
XORG_MACROS_VERSION(1.19)
XORG_DEFAULT_OPTIONS

XORG_PROG_RAWCPP
AC_CANONICAL_HOST

# Build options
DEFAULT_XRDB=xrdb
DEFAULT_XMODMAP=xmodmap
DEFAULT_TWM=twm
DEFAULT_XCLOCK=xclock
DEFAULT_XTERM=xterm
# You always want to specify the full path to the X server
DEFAULT_XSERVER=${bindir}/X
DEFAULT_XAUTH=xauth
DEFAULT_XINIT=xinit
DEFAULT_XINITDIR=${sysconfdir}/X11/xinit
DEFAULT_WM=fvwm
DEFAULT_XCONSOLE=xconsole

AC_ARG_WITH(xrdb,
	 AS_HELP_STRING([--with-xrdb=XRDB], [Path to xrdb]),
	[XRDB="$withval"],
	[XRDB="$DEFAULT_XRDB"])

AC_ARG_WITH(xmodmap,
	 AS_HELP_STRING([--with-xmodmap=XMODMAP], [Path to xmodmap]),
	[XMODMAP="$withval"],
	[XMODMAP="$DEFAULT_XMODMAP"])

AC_ARG_WITH(twm,
	 AS_HELP_STRING([--with-twm=TWM], [Path to twm]),
	[TWM="$withval"],
	[TWM="$DEFAULT_TWM"])

AC_ARG_WITH(xclock,
	 AS_HELP_STRING([--with-xclock=XCLOCK], [Path to xclock]),
	[XCLOCK="$withval"],
	[XCLOCK="$DEFAULT_XCLOCK"])

AC_ARG_WITH(xterm,
	 AS_HELP_STRING([--with-xterm=XTERM], [Path to xterm]),
	[XTERM="$withval"],
	[XTERM="$DEFAULT_XTERM"])

AC_ARG_WITH(xserver,
	 AS_HELP_STRING([--with-xserver=XSERVER], [Path to default X server]),
	[XSERVER="$withval"],
	[XSERVER="$DEFAULT_XSERVER"])

AC_ARG_WITH(xauth,
	 AS_HELP_STRING([--with-xauth=XAUTH], [Path to xauth]),
	[XAUTH="$withval"],
	[XAUTH="$DEFAULT_XAUTH"])

AC_ARG_WITH(xinit,
	 AS_HELP_STRING([--with-xinit=XINIT], [Path to xinit]),
	[XINIT="$withval"],
	[XINIT="$DEFAULT_XINIT"])

AC_ARG_WITH(xinitdir,
	 AS_HELP_STRING([--with-xinitdir=XINITDIR], [Path to xinitdir]),
	[XINITDIR="$withval"],
	[XINITDIR="$DEFAULT_XINITDIR"])

AC_ARG_WITH(wm,
	AS_HELP_STRING([--with-wm=WM], [Path to default window manager]),
	[WM="$withval"],
	[WM="$DEFAULT_WM"])

AC_ARG_WITH(xconsole,
	 AS_HELP_STRING([--with-xconsole=XCONSOLE], [Path to xconsole]),
	[XCONSOLE="$withval"],
	[XCONSOLE="$DEFAULT_XCONSOLE"])

AC_ARG_WITH(launchd,            AS_HELP_STRING([--with-launchd], [Build with support for Apple's launchd (default: auto)]), [LAUNCHD=$withval], [LAUNCHD=auto])
AC_ARG_WITH(launchagents-dir,   AS_HELP_STRING([--with-launchagents-dir=PATH], [Path to launchd's LaunchAgents directory (default: /Library/LaunchAgents)]),
                                [ launchagentsdir="${withval}" ],
                                [ launchagentsdir="/Library/LaunchAgents" ])
AC_ARG_WITH(launchagent-xserver, AS_HELP_STRING([--with-launchagent-xserver=PATH], [Path to the X server which the LaunchAgent should start (if not provided, let startx decide)"]),
                                 [ launchagentxserver="${withval}" ],
                                 [ launchagentxserver="no" ])
AC_ARG_WITH(launchdaemons-dir,  AS_HELP_STRING([--with-launchdaemons-dir=PATH], [Path to launchd's LaunchDaemonss directory (default: /Library/LaunchDaemons)]),
                                [ launchdaemonsdir="${withval}" ],
                                [ launchdaemonsdir="/Library/LaunchDaemons" ])
AC_ARG_WITH(launchd-id-prefix,  AS_HELP_STRING([--with-launchd-id-prefix=PATH], [Deprecated: Use --with-bundle-id-prefix.]),
                                [ bundleidprefix="${withval}" ],
                                [ bundleidprefix="org.x" ])
AC_ARG_WITH(bundle-id-prefix,  AS_HELP_STRING([--with-bundle-id-prefix=PATH], [Prefix to use for bundle identifiers (default: org.x)]),
                                [ bundleidprefix="${withval}" ])

if test "x$LAUNCHD" = "xauto"; then
	unset LAUNCHD
	AC_CHECK_PROG(LAUNCHD, [launchd], [yes], [no], [$PATH$PATH_SEPARATOR/sbin])
fi

TIGER_LAUNCHD=no
if test "x$LAUNCHD" = "xyes" ; then
	AC_DEFINE(HAVE_LAUNCHD, 1, [launchd support available])
	case $host_os in
		darwin8*)
			TIGER_LAUNCHD=yes
		;;
	esac
        AC_CHECK_FUNC(dispatch_async,
                              AC_DEFINE([HAVE_LIBDISPATCH], 1, [Define to 1 if you have the libdispatch (GCD) available]),
                              [])
fi

AC_DEFINE_UNQUOTED(BUNDLE_ID_PREFIX, "$bundleidprefix", [Prefix to use for launchd identifiers])
AC_SUBST([launchagentsdir])
AC_SUBST([launchdaemonsdir])
AC_SUBST([bundleidprefix])
AC_SUBST([launchagentxserver])
AM_CONDITIONAL(LAUNCHD, [test "x$LAUNCHD" = "xyes"])
AM_CONDITIONAL(TIGER_LAUNCHD, [test "x$TIGER_LAUNCHD" = "xyes"])
AM_CONDITIONAL(LAUNCHAGENT_XSERVER, [test "x$launchagentxserver" != "xno"])

# Checks for pkg-config packages
PKG_CHECK_MODULES(XINIT, x11 xproto >= 7.0.17)

case $host_os in
    *bsd*)
        XINIT_CFLAGS="$XINIT_CFLAGS -DCSRG_BASED"
        ;;
esac


AC_PATH_PROGS(MCOOKIE, [mcookie], [$MCOOKIE],
  [$PATH:/bin:/usr/bin:/usr/lib:/usr/libexec:/usr/local/bin])

# If mcookie not found, try openssl, else per-system fallbacks
if test "x$MCOOKIE" = x ; then
    AC_PATH_PROGS(OPENSSL, [openssl], [$OPENSSL],
        [$PATH:/bin:/usr/bin:/usr/sbin:/usr/lib:/usr/libexec:/usr/local/bin])
    if test "x$OPENSSL" != x ; then
        MCOOKIE="$OPENSSL rand -hex 16"
    else
        case $host_os in
            *solaris*)
                MCOOKIE="/usr/bin/od -X -A n -N 16 /dev/urandom | /usr/bin/tr -d ' '"
                ;;
        esac
    fi
fi
if test "x$MCOOKIE" != x ; then
	STARTX_COOKIE_FLAGS='-DHAS_COOKIE_MAKER -DMK_COOKIE="$(MCOOKIE)"'
	AC_MSG_NOTICE([startx will depend on "$MCOOKIE" to generate xauth cookies])
fi
AC_SUBST(STARTX_COOKIE_FLAGS)

# Additional substitutions in startx, xinitrc & man pages
SHELL_CMD="/bin/sh"
XSERVERNAME="Xorg"
XCONFIGFILE="xorg.conf"
XCONFIGFILEMAN='${XCONFIGFILE} (${FILE_MAN_SUFFIX})'
case $host_os in
    cygwin*)
	XSERVERNAME="XWin"
	XCONFIGFILE="XWinrc"
	;;
    darwin*)
	XSERVERNAME="Xquartz"
	XCONFIGFILE="defaults"
	XCONFIGFILEMAN='defaults (1)'
	;;
    *solaris*)
	SHELL_CMD="/bin/ksh"
	;;
    *sco*)
	SHELL_CMD="/bin/ksh"
	SCOMAN=1
	;;
esac
AC_SUBST(SHELL_CMD)
AC_SUBST(SCOMAN)
AC_SUBST(XSERVERNAME)
AC_SUBST(XCONFIGFILE)
AC_SUBST(XCONFIGFILEMAN)

AC_SUBST(XRDB)
AC_SUBST(XMODMAP)
AC_SUBST(TWM)
AC_SUBST(XCLOCK)
AC_SUBST(XTERM)
AC_SUBST(XSERVER)
AC_SUBST(XAUTH)
AC_SUBST(XINIT)
AC_SUBST(XINITDIR)
AC_SUBST(WM)
AC_SUBST(XCONSOLE)

AC_CONFIG_FILES([Makefile
	man/Makefile
	launchd/Makefile
	launchd/privileged_startx/Makefile
	launchd/user_startx/Makefile])
AC_OUTPUT
