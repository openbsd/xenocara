
# Initialize Autoconf
AC_PREREQ(2.60)
AC_INIT([libSM], [1.2.6],
        [https://gitlab.freedesktop.org/xorg/lib/libsm/-/issues], [libSM])
AC_CONFIG_SRCDIR([Makefile.am])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIRS([m4])

# Initialize Automake
AM_INIT_AUTOMAKE([foreign dist-xz])

# Initialize libtool
LT_INIT

# Require xorg-macros minimum of 1.12 for DocBook external references
m4_ifndef([XORG_MACROS_VERSION],
          [m4_fatal([must install xorg-macros 1.12 or later before running autoconf/autogen])])
XORG_MACROS_VERSION(1.12)
XORG_DEFAULT_OPTIONS
XORG_ENABLE_DOCS
XORG_WITH_XMLTO(0.0.22)
XORG_WITH_FOP
XORG_WITH_XSLTPROC
XORG_CHECK_SGML_DOCTOOLS(1.8)

# Check for UUID support
AC_ARG_WITH(libuuid, AS_HELP_STRING([--with-libuuid], [Build with libuuid support for client IDs]))

PKG_PROG_PKG_CONFIG()
AC_CHECK_FUNCS([uuid_create], [], [
    if test x"$with_libuuid" != xno && test x"$have_system_uuid" != xyes; then
        PKG_CHECK_MODULES(LIBUUID, uuid, [HAVE_LIBUUID=yes], [HAVE_LIBUUID=no])
    fi
    if test x"$with_libuuid" = xyes && test x"$HAVE_LIBUUID" = xno; then
        AC_MSG_ERROR([requested libuuid support but uuid.pc not found
        and LIBUUID_CFLAGS and LIBUUID_LIBS not set])
    fi
])
UUID_LIB_PRIVATE=""
UUID_PC=""
if test x"$HAVE_LIBUUID" = xyes ; then
   PKG_CHECK_EXISTS(uuid, [UUID_PC="uuid"])
   if test x"$UUID_PC" = x ; then
       UUID_LIB_PRIVATE="$LIBUUID_LIBS"
   fi
fi
AC_SUBST([UUID_LIB_PRIVATE])
AC_SUBST([UUID_PC])

AM_CONDITIONAL(WITH_LIBUUID, test x"$HAVE_LIBUUID" = xyes)

# If UUID support is not found, fall back to using network addresses
# to generate client ids
AS_IF([test x"$HAVE_LIBUUID" != xyes && test x"$ac_cv_func_uuid_create" != xyes],
      [genid_module="xtrans"
# Needed to check for TCP & IPv6 support and set flags appropriately
XTRANS_CONNECTION_FLAGS
AC_CHECK_FUNCS([getaddrinfo]) ])

# Obtain compiler/linker options for dependencies
PKG_CHECK_MODULES(SM, [ice >= 1.1.0 xproto $genid_module])


AC_CONFIG_FILES([Makefile
		doc/Makefile
		src/Makefile
		sm.pc])
AC_OUTPUT
