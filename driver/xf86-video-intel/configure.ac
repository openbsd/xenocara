#  Copyright 2005 Adam Jackson.
#
#  Permission is hereby granted, free of charge, to any person obtaining a
#  copy of this software and associated documentation files (the "Software"),
#  to deal in the Software without restriction, including without limitation
#  on the rights to use, copy, modify, merge, publish, distribute, sub
#  license, and/or sell copies of the Software, and to permit persons to whom
#  the Software is furnished to do so, subject to the following conditions:
#
#  The above copyright notice and this permission notice (including the next
#  paragraph) shall be included in all copies or substantial portions of the
#  Software.
#
#  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
#  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
#  FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.  IN NO EVENT SHALL
#  ADAM JACKSON BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
#  IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
#  CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
#
# Process this file with autoconf to produce a configure script

# Initialize Autoconf
AC_PREREQ([2.60])
AC_INIT([xf86-video-intel],
        [2.99.916],
        [https://bugs.freedesktop.org/enter_bug.cgi?product=xorg],
        [xf86-video-intel])
AC_CONFIG_SRCDIR([Makefile.am])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_AUX_DIR(.)

# Initialize Automake
AM_INIT_AUTOMAKE([foreign dist-bzip2])

# Require X.Org macros 1.8 or later for MAN_SUBSTS set by XORG_MANPAGE_SECTIONS
m4_ifndef([XORG_MACROS_VERSION],
          [m4_fatal([must install xorg-macros 1.8 or later before running autoconf/autogen.
  Hint: either install from source, git://anongit.freedesktop.org/xorg/util/macros or,
  depending on you distribution, try package 'xutils-dev' or 'xorg-x11-util-macros'])])

XORG_MACROS_VERSION(1.8)
XORG_DEFAULT_OPTIONS

# And disable a few very noisy warnings
m4_ifdef([XORG_TESTSET_CFLAG], [
XORG_TESTSET_CFLAG([NOWARNFLAGS], [-Wno-cast-qual])
XORG_TESTSET_CFLAG([NOWARNFLAGS], [-Wno-redundant-decls])
XORG_TESTSET_CFLAG([NOWARNFLAGS], [-Wno-maybe-uninitialized])
])
AC_SUBST(NOWARNFLAGS)

# Require X.Org server macros (i.e. XORG_DRIVER_CHECK_EXT) to check for required modules 
m4_ifndef([XORG_DRIVER_CHECK_EXT],
          [m4_fatal([must install xorg-server macros before running autoconf/autogen.
  Hint: either install from source, git://anongit.freedesktop.org/xorg/xserver or,
  depending on your distribution, try package 'xserver-xorg-dev' or 'xorg-x11-server-devel'])])

# Initialize libtool
AC_DISABLE_STATIC
AC_PROG_LIBTOOL
AC_SYS_LARGEFILE

# Check for common libc routines redefined by os.h
AC_CHECK_FUNCS([strlcpy strlcat strndup], [], [])

# Platform specific settings
case $host_os in
  *linux*)
    backlight_helper=yes
    ;;
esac

AC_ARG_ENABLE(backlight,
              AS_HELP_STRING([--disable-backlight],
			     [Enable control over the backlight [default=yes]]),
              [backlight="$enableval"],
              [backlight="yes"])
if test "x$backlight" = "xyes"; then
	AC_DEFINE(USE_BACKLIGHT, 1, [Enable control of the backlight])
fi

AC_ARG_ENABLE(backlight-helper,
              AS_HELP_STRING([--disable-backlight-helper],
			     [Enable building the backlight helper executable for running X under a normal user [default=auto]]),
              [backlight_helper="$enableval"],)
AM_CONDITIONAL(BUILD_BACKLIGHT_HELPER, [test "x$backlight" = "xyes" -a "x$backlight_helper" = "xyes"])
if test "x$backlight_helper" = "xyes"; then
	tools_msg="$tools_msg xf86-video-intel-backlight-helper"
	AC_DEFINE(USE_BACKLIGHT_HELPER, 1, [Enable use of the backlight helper interfaces])
fi

# Are we in a git checkout?
dot_git=no
if test -e .git; then
	AC_DEFINE(HAVE_DOT_GIT, 1, [Are we in a git checkout?])
	dot_git=yes
fi
AM_CONDITIONAL(HAVE_DOT_GIT, test "x$dot_git" = "xyes")

# If so, we include the git description in our logs for sanity checking.
#
# However, for people not running their own drivers this is just noise.
# So we copy the xserver's builderstring idiom to allow for this to be
# overridden and perhaps replaced with something more useful.
AC_ARG_WITH(builderstring,
	    AS_HELP_STRING([--with-builderstring=BUILDERSTRING],
			   [Additional builder string (default: use git describe)]),
	    [BUILDERSTRING="$withval"],
	    [BUILDERSTRING="x-magic-git-describe"])

if test "x$BUILDERSTRING" = "xx-magic-git-describe" -a "x$dot_git" = "xyes"; then
	AC_DEFINE(USE_GIT_DESCRIBE, 1, [Use automagic builder description])
else
  if test "x$BUILDERSTRING" != x -a "x$BUILDERSTRING" != "xno" -a "x$BUILDERSTRING" != xx-magic-git-describe; then
	  AC_DEFINE_UNQUOTED(BUILDER_DESCRIPTION, ["$BUILDERSTRING"], [Builder description])
  fi
fi

AC_ARG_ENABLE(gen4asm,
              AS_HELP_STRING([--enable-gen4asm],
			     [Enable rebuilding the gen4 assembly files [default=no]]),
              [ASM="$enableval"],
              [ASM="no"])

gen4asm=no
if test "x$ASM" != "xno"; then
	AC_ARG_WITH(gen4asm,
		    AS_HELP_STRING([--with-gen4asm=PATH],
				   [Path to intel-gen4asm binary]),
		    [path="$withval"],
		    [path=""])
	if test -n "$path" ; then
		gen4asm=yes
	else
		PKG_CHECK_MODULES(GEN4ASM, [intel-gen4asm >= 1.2], [gen4asm=yes], [gen4asm=no])
		if test "x$ASM" = "xyes" -a "x$gen4asm" != "xyes"; then
			AC_MSG_ERROR([intel-gen4asm support requested but not found])
		fi
	fi
	if test "x$gen4asm" = "xyes"; then
		AC_MSG_CHECKING([path to use for intel-gen4asm])
		if test -n "$path" ; then
			INTEL_GEN4ASM="$path"
		else
			INTEL_GEN4ASM="`pkg-config intel-gen4asm --variable=exec_prefix`/bin/intel-gen4asm"
		fi
		if ! test -e "$INTEL_GEN4ASM"; then
			AC_MSG_ERROR([intel-gen4asm enabled, but not found. Tried '$INTEL_GEN4ASM'.])
		fi
		AC_MSG_RESULT([$INTEL_GEN4ASM])
		AC_SUBST([INTEL_GEN4ASM])
	fi
fi
AM_CONDITIONAL(HAVE_GEN4ASM, test "x$gen4asm" = "xyes")

# Check for atomic intrinsics
AC_CACHE_CHECK([for native atomic primitives], intel_cv_atomic_primitives,
[
    intel_cv_atomic_primitives="none"

    AC_LINK_IFELSE([AC_LANG_PROGRAM([[
int atomic_add(int i) { return __sync_fetch_and_add (&i, 1); }
int atomic_cmpxchg(int i, int j, int k) { return __sync_val_compare_and_swap (&i, j, k); }
				    ]],[[]])],
		   [intel_cv_atomic_primitives="Intel"],[])

    if test "x$intel_cv_atomic_primitives" = "xnone"; then
	    AC_CHECK_HEADER([atomic_ops.h], intel_cv_atomic_primitives="libatomic-ops")
    fi

    # atomic functions defined in <atomic.h> & libc on Solaris
    if test "x$intel_cv_atomic_primitives" = "xnone"; then
	    AC_CHECK_FUNC([atomic_cas_uint],
			  intel_cv_atomic_primitives="Solaris")
    fi

])
if test "x$intel_cv_atomic_primitives" = "xIntel"; then
    AC_DEFINE(HAVE_ATOMIC_PRIMITIVES, 1,
	      [Enable if your compiler supports the Intel __sync_* atomic primitives])
fi
if test "x$intel_cv_atomic_primitives" = "xlibatomic-ops"; then
    AC_DEFINE(HAVE_LIB_ATOMIC_OPS, 1, [Enable if you have libatomic-ops-dev installed])
fi

if test "x$intel_cv_atomic_primitives" = "xnone"; then
		AC_MSG_ERROR([xf86-video-intel depends upon atomic operations, which were not found for your compiler/cpu. Try compiling with -march=native, or install the libatomics-op-dev package.])
fi

AC_ARG_ENABLE(udev,
              AS_HELP_STRING([--disable-udev],
                             [Disable udev-based monitor hotplug detection [default=auto]]),
              [UDEV="$enableval"],
              [UDEV=auto])

if test "x$UDEV" != "xno"; then
	PKG_CHECK_MODULES(UDEV, [libudev], [udev="yes"], [udev="no"])
	if test "x$UDEV" = "xyes" -a "x$udev" != "xyes"; then
		AC_MSG_ERROR([udev support requested but not found (libudev)])
	fi
	if test "x$udev" = "xyes"; then
		AC_DEFINE(HAVE_UDEV,1,[Enable udev-based monitor hotplug detection])
	fi
fi

PKG_CHECK_MODULES(X11, [x11 xrender xrandr xext xfixes cairo cairo-xlib-xrender pixman-1 libpng], [x11="yes"], [x11="no"])
AM_CONDITIONAL(HAVE_X11, test "x$x11" = "xyes")

cpuid="yes"
AC_TRY_LINK([
	#include <cpuid.h>
	#include <stddef.h>
	],
	[
	int eax, ebx, ecx, edx;
	if (__get_cpuid_max(0, NULL) < 4)
		return 0;
	__cpuid_count(4, 0, eax, ebx, ecx, edx);
	],
	[cpuid="yes"],
	[cpuid="no"]
)
if test "x$cpuid" = "xyes"; then
	AC_DEFINE(HAVE_CPUID_H,1,[Found a useable cpuid.h])
fi

shm=yes
AC_CHECK_HEADERS([sys/ipc.h sys/ipc.h], [], [shm="no"])
AC_CHECK_HEADERS([X11/extensions/XShm.h], [], [shm="no"], [
#include <X11/Xlibint.h>
#include <X11/Xproto.h>
])
AC_CHECK_HEADERS([X11/extensions/shmproto.h X11/extensions/shmstr.h], [], [], [
#include <X11/Xlibint.h>
#include <X11/Xproto.h>
])
if test "x$ac_cv_header_X11_extensions_shmproto_h" != "xyes" -a "x$ac_cv_header_X11_extensions_shmstr_h" != "xyes"; then
	shm="no"
fi

if test "x$shm" = "xyes"; then
	AC_MSG_CHECKING(whether shmctl IPC_RMID allows subsequent attaches)
	AC_TRY_RUN([
		  #include <sys/types.h>
		  #include <sys/ipc.h>
		  #include <sys/shm.h>
		  int main()
		  {
		      char *shmaddr;
		      int id = shmget (IPC_PRIVATE, 4, IPC_CREAT | 0600);
		      if (id == -1) return 2;
		      shmaddr = shmat (id, 0, 0);
		      shmctl (id, IPC_RMID, 0);
		      if ((char*) shmat (id, 0, 0) == (char*) -1) {
			  shmdt (shmaddr);
			  return 1;
		      }
		      shmdt (shmaddr);
		      shmdt (shmaddr);
		      return 0;
		  }
		  ],
		  AC_DEFINE(IPC_RMID_DEFERRED_RELEASE, 1,
			    [Define to 1 if shared memory segments are released deferred.])
		  AC_MSG_RESULT(yes),
		  AC_MSG_RESULT(no),
		  AC_MSG_RESULT(assuming no))
	AC_DEFINE([HAVE_MIT_SHM], 1, [Define to 1 if MIT-SHM is available])
fi

PKG_CHECK_MODULES(X11_DRI3, [xcb-dri3 xcb-sync xcb-present x11-xcb xshmfence x11 xrender xext libdrm], [x11_dri3="yes"], [x11_dri3="no"])
AM_CONDITIONAL(X11_DRI3, test "x$x11_dri3" = "xyes" -a "x$shm" = "xyes")
AM_CONDITIONAL(X11_SHM, test "x$shm" = "xyes")

AC_ARG_ENABLE(tools,
              AS_HELP_STRING([--disable-tools],
			     [Enable building and installing the miscellaneous tools [default=auto]]),
              [tools="$enableval"], [tools="auto"])
if test "x$shm" != "xyes"; then
	if test "x$tools" = "xyes"; then
		AC_MSG_ERROR([Incomplete requirements for extra tools, X11 MIT-SHM extension required])
	fi
	tools="no"
fi
if test "x$tools" != "xno"; then
	ivo_requires="xrandr xdamage xfixes xcursor xtst xrender xext x11 pixman-1"
	extra_cflags=""

	ignore="xinerama"
	PKG_CHECK_MODULES(IVO_EXTRA, [$ignore],
			  [AC_CHECK_HEADERS([X11/extensions/Xinerama.h], [ivo_requires="$ignore $ivo_requires"], [],
					    [#include <X11/Xlibint.h>
					     #include <X11/Xproto.h>
					    ])], [ignore=""])

	ignore="xcb-dri3 xcb-sync x11-xcb xshmfence x11"
	PKG_CHECK_MODULES(IVO_EXTRA, [$ignore], [ivo_requires="$ivo_requires $ignore"; extra_cflags="-DDRI3"], [ignore=""])

	PKG_CHECK_MODULES(IVO, [$ivo_requires], [ivo="yes"], [ivo="no"])
	AC_CHECK_HEADER([sys/timerfd.h], [], [ivo="no"])
	if test "x$ivo" = "xno"; then
		if test "x$tools" = "xyes"; then
			AC_MSG_ERROR([Incomplete requirements for intel-virtual-output, requires $ivo_requires])
		fi
		tools="no"
	fi

	IVO_CFLAGS="$IVO_CFLAGS $extra_cflags"
fi
if test "x$tools" != "xno"; then
      tools_msg="$tools_msg intel-virtual-output"
fi
AC_MSG_CHECKING([whether to build additional tools])
AC_MSG_RESULT([$tools])
AM_CONDITIONAL(BUILD_TOOLS, test "x$tools" != "xno")

# Define a configure option for an alternate module directory
AC_ARG_WITH(xorg-module-dir,
            AS_HELP_STRING([--with-xorg-module-dir=DIR],
                           [Default xorg module directory [[default=$libdir/xorg/modules]]]),
            [moduledir="$withval"],
            [moduledir="$libdir/xorg/modules"])

AC_ARG_ENABLE(dri,
	      AS_HELP_STRING([--disable-dri],
			     [Disable DRI support [[default=auto]]]),
	      [DRI=$enableval],
	      [DRI=auto])
AC_ARG_ENABLE(dri1,
	      AS_HELP_STRING([--disable-dri1],
			     [Disable DRI1 support [[default=yes]]]),
	      [DRI1=$enableval],
	      [DRI1=yes])
AC_ARG_ENABLE(dri2,
	      AS_HELP_STRING([--disable-dri2],
			     [Disable DRI2 support [[default=yes]]]),
	      [DRI2=$enableval],
	      [DRI2=yes])
AC_ARG_ENABLE(dri3,
	      AS_HELP_STRING([--disable-dri3],
			     [Disable DRI3 support [[default=yes]]]),
	      [DRI3=$enableval],
	      [DRI3=yes])

AC_ARG_ENABLE(xvmc, AS_HELP_STRING([--disable-xvmc],
                                  [Disable XvMC support [[default=yes]]]),
              [XVMC="$enableval"],
              [XVMC="yes"])

AC_ARG_ENABLE(kms,
	      AS_HELP_STRING([--enable-kms],
			     [Assume KMS support [[default=yes]]]),
              [KMS="$enableval"],
              [KMS="yes"])
AC_ARG_ENABLE(ums,
	      AS_HELP_STRING([--enable-ums],
			     [Assume UMS support [[default=auto]]]),
              [UMS="$enableval"],
              [UMS="auto"])

AC_ARG_ENABLE(kms-only,
	      AS_HELP_STRING([--enable-kms-only],
			     [Only assume KMS support (no UMS) [[default=no]]]),
              [ONLY_KMS="$enableval"],
              [ONLY_KMS="no"])

AC_ARG_ENABLE(ums-only,
	      AS_HELP_STRING([--enable-ums-only],
			     [Only assume UMS support (no KMS) [[default=no]]]),
              [ONLY_UMS="$enableval"],
              [ONLY_UMS="no"])

required_xorg_server_version=1.6
required_pixman_version=0.16

if pkg-config --exists 'pixman-1 >= 0.27.1'; then
	AC_DEFINE([HAS_PIXMAN_GLYPHS], 1, [Enable pixman glyph cache])
fi

if pkg-config --exists 'pixman-1 >= 0.24.0'; then
	AC_DEFINE([HAS_PIXMAN_TRIANGLES], 1, [Enable pixman triangle rasterisation])
fi

# Store the list of server defined optional extensions in REQUIRED_MODULES
XORG_DRIVER_CHECK_EXT(RANDR, randrproto)
XORG_DRIVER_CHECK_EXT(RENDER, renderproto)
XORG_DRIVER_CHECK_EXT(DPMSExtension, xextproto)

# Obtain compiler/linker options for the driver dependencies
PKG_CHECK_MODULES(DRM, [libdrm >= 2.4.20]) # libdrm_intel is checked separately
PKG_CHECK_MODULES(PCIACCESS, [pciaccess >= 0.10])

AC_ARG_ENABLE(sna,
	      AS_HELP_STRING([--enable-sna],
			     [Enable SandyBridge\'s New Acceleration (SNA) [default=auto]]),
	      [SNA="$enableval"],
	      [SNA=auto])

if test "x$SNA" != "xno"; then
	AC_DEFINE(USE_SNA, 1, [Enable SNA support])
	AC_CHECK_HEADERS([sys/sysinfo.h], AC_CHECK_MEMBERS([struct sysinfo.totalram], [], [], [[#include <sys/sysinfo.h>]]))
fi

uxa_requires_libdrm=2.4.52
AC_ARG_ENABLE(uxa,
	      AS_HELP_STRING([--enable-uxa],
			     [Enable Unified Acceleration Architecture (UXA) [default=auto]]),
	      [UXA="$enableval"],
	      [UXA=auto])
if test "x$UXA" = "xauto"; then
	if ! pkg-config --exists "libdrm_intel >= $uxa_requires_libdrm"; then
		UXA=no
	fi
	if ! pkg-config --exists 'pixman-1 >= 0.24.0'; then
		UXA=no
	fi
fi
if test "x$UXA" != "xno"; then
	AC_DEFINE(USE_UXA, 1, [Enable UXA support])
	PKG_CHECK_MODULES(DRMINTEL, [libdrm_intel >= $uxa_requires_libdrm])
	required_pixman_version=0.24
	UXA=yes
fi

AC_ARG_ENABLE(glamor,
	      AS_HELP_STRING([--enable-glamor],
			     [Enable glamor, a new GL-based acceleration [default=no]]),
	      [GLAMOR="$enableval"],
	      [GLAMOR="no"])
if test "x$GLAMOR" != "xno"; then
	if test "x$UXA" != "xyes"; then
		AC_MSG_ERROR([Glamor acceleration requested but UXA is not enabled])
	fi
	if pkg-config --exists "xorg-server >= 1.15.99.901"; then
		GLAMOR="yes (using Xorg glamor module)"
	else
		PKG_CHECK_MODULES(LIBGLAMOR, [glamor >= 0.6.0])
		PKG_CHECK_MODULES(LIBGLAMOR_EGL, [glamor-egl])
		GLAMOR="yes (using libglamor)"
	fi
	AC_DEFINE(USE_GLAMOR, 1, [Enable glamor acceleration])
fi

PKG_CHECK_MODULES(XORG, [xorg-server >= $required_xorg_server_version xproto fontsproto pixman-1 >= $required_pixman_version $REQUIRED_MODULES])
ABI_VERSION=`$PKG_CONFIG --variable=abi_videodrv xorg-server`

if test "x$ONLY_UMS" = "xyes"; then
	UMS="yes"
	KMS="no"
fi

if test "x$ONLY_KMS" = "xyes"; then
	UMS="no"
	KMS="yes"
fi

save_CPPFLAGS=$CPPFLAGS
CPPFLAGS=$XORG_CFLAGS
AC_CHECK_HEADERS([vgaHW.h], legacy="yes", legacy="no")
CPPFLAGS=$save_CPPFLAGS
if test "x$UMS" = "xauto"; then
	UMS="$legacy"
fi
if test "x$UMS" = "xyes" -a "x$legacy" = "xno"; then
	AC_MSG_ERROR([vgaHW support required for UMS (i810) driver])
fi

if test "x$UMS" = "xyes"; then
	AC_ARG_ENABLE(xaa,
		      AS_HELP_STRING([--enable-xaa],
				     [Enable legacy X Acceleration Architecture (XAA) for i810 chipsets [default=auto]]),
		      [XAA="$enableval"],
		      [XAA="auto"])
	if test "x$XAA" != "xno"; then
		save_CPPFLAGS=$CPPFLAGS
		CPPFLAGS=$XORG_CFLAGS
		AC_CHECK_HEADERS([xaa.h], XAA="yes", XAA="no")
		CPPFLAGS=$save_CPPFLAGS
	fi
	AC_MSG_CHECKING([whether to include XAA support])
	AC_MSG_RESULT([$XAA])

	AC_ARG_ENABLE(dga,
		      AS_HELP_STRING([--enable-dga],
				     [Enable legacy Direct Graphics Access (DGA) for i810 chipsets [default=auto]]),
		      [DGA="$enableval"],
		      [DGA="auto"])
	if test "x$DGA" != "xno"; then
		save_CFLAGS=$CFLAGS
		CFLAGS=$XORG_CFLAGS
		AC_CHECK_HEADERS([dgaproc.h], DGA="yes", DGA="no", [#include <dixstruct.h>])
		CFLAGS=$save_CFLAGS
	fi
	AC_MSG_CHECKING([whether to include DGA support])
	AC_MSG_RESULT([$DGA])
fi
AM_CONDITIONAL(DGA, test "x$DGA" = "xyes")
AM_CONDITIONAL(XAA, test "x$XAA" = "xyes")

AM_CONDITIONAL(KMS, test "x$KMS" = "xyes")
if test "x$KMS" = "xyes"; then
	AC_DEFINE(KMS,1,[Assume KMS support])
fi
AM_CONDITIONAL(UMS, test "x$UMS" = "xyes")
if test "x$UMS" = "xyes"; then
	AC_DEFINE(UMS,1,[Assume UMS support])
fi

have_dri1=no
XORG_DRIVER_CHECK_EXT(XF86DRI, xf86driproto)
if test "x$_EXT_CHECK" != "xno" -a "x$DRI" != "xno" -a "x$DRI1" != "xno" -a "x$UMS" = "xyes"; then
	PKG_CHECK_MODULES(DRI1, [xf86driproto], [have_dri1=$DRI], [have_dri1=no])
        save_CFLAGS="$CFLAGS"
        save_CPPFLAGS="$CPPFLAGS"
        CFLAGS="$CFLAGS $XORG_CFLAGS $DRI1_CFLAGS $DRM_CFLAGS"
        CPPFLAGS="$CPPFLAGS $XORG_CFLAGS $DRI1_CFLAGS $DRM_CFLAGS"
        AC_CHECK_HEADERS([dri.h sarea.h dristruct.h], [], [have_dri1=no],
                [/* for dri.h */
                 #include <xf86str.h>
                 /* for dristruct.h */
                 #include <xorg-server.h>
                 #ifdef HAVE_DRI_H
                 # include <dri.h>
                 #endif
                 #ifdef HAVE_SAREA_H
                 # include <sarea.h>
                 #endif
                ])
        CFLAGS="$save_CFLAGS"
        CPPFLAGS="$save_CPPFLAGS"
fi

AC_MSG_CHECKING([whether to include DRI1 support])
AC_MSG_RESULT([$have_dri1])

AM_CONDITIONAL(DRI1, test "x$have_dri1" != "xno")
if test "x$have_dri1" != "xno"; then
        AC_DEFINE(HAVE_DRI1,1,[Enable DRI1 driver support])
	dri_msg="$dri_msg DRI1"
else
        DRI1_CFLAGS=""
        DRI1_LIBS=""

        if test "x$DRI" = "xyes" -a "x$UMS" = "xyes" -a "x$DRI1" != "xno"; then
                AC_MSG_ERROR([DRI1 requested but prerequisites not found])
        fi
fi

have_dri2=no
have_dri3=no
if test "x$DRI" != "xno"; then
	if test "x$DRI2" != "xno"; then
		PKG_CHECK_MODULES(DRI2, [dri2proto >= 2.6], [have_dri2=$DRI], [have_dri2=no])
	fi
	if test "x$have_dri2" != "xno"; then
		save_CFLAGS=$CFLAGS
		CFLAGS="$XORG_CFLAGS $DRM_CFLAGS $DRI1_CFLAGS $DRI2_CFLAGS"
		AC_CHECK_HEADERS([dri2.h], [], [have_dri2=no], [
#include <dixstruct.h>
#include <drm.h>
])
		CFLAGS=$save_CFLAGS
	fi
	if test "x$have_dri2" != "xno"; then
		dridriverdir=`$PKG_CONFIG --variable=dridriverdir dri`
		if test "x$dridriverdir" = "x"; then
			dridriverdir="$libdir/dri"
		fi
		AC_DEFINE_DIR(DRI_DRIVER_PATH, dridriverdir, [Default have_dri2 driver path])
	fi

	if test "x$DRI3" != "xno"; then
		XORG_DRIVER_CHECK_EXT(DRI3, dri3proto)
		if test "x$_EXT_CHECK" != "xno"; then
			PKG_CHECK_MODULES(DRI3, [dri3proto], [have_dri3=$DRI], [])
		fi
	fi
	if test "x$have_dri3" != "xno"; then
		save_CFLAGS=$CFLAGS
		CFLAGS="$XORG_CFLAGS $DRI3_CFLAGS"
		AC_CHECK_DECL(DRI3, [], [have_dri3=no], [#include <xorg-server.h>])
		AC_CHECK_HEADERS([misyncstr.h misyncshm.h], [], [have_dri3=no], [
#include <xorg-server.h>
#include <xf86str.h>
#include <misync.h>
])
		CFLAGS=$save_CFLAGS
	fi
fi

AC_MSG_CHECKING([whether to include DRI2 support])
AM_CONDITIONAL(DRI2, test "x$have_dri2" != "xno")
AC_MSG_RESULT([$have_dri2])
if test "x$have_dri2" != "xno"; then
        AC_DEFINE(HAVE_DRI2,1,[Enable DRI2 driver support])
	dri_msg="$dri_msg DRI2"
else
	if test "x$DRI" = "xyes" -a "x$DRI2" != "xno" -a "x$KMS" = "xyes"; then
		AC_MSG_ERROR([DRI2 requested but prerequisites not found])
	fi

	# UXA doesn't build without DRI2 headers, too late to fix
	UXA=no
fi

AC_MSG_CHECKING([whether to include DRI3 support])
AM_CONDITIONAL(DRI3, test "x$have_dri3" != "xno")
AC_MSG_RESULT([$have_dri3])
if test "x$have_dri3" != "xno"; then
        AC_DEFINE(HAVE_DRI3,1,[Enable DRI3 driver support])
	dri_msg="$dri_msg DRI3"
else
	if test "x$DRI" = "xyes" -a "x$DRI3" != "xno" -a "x$KMS" = "xyes"; then
		AC_MSG_ERROR([DRI3 requested but prerequisites not found])
	fi
fi

AC_CHECK_HEADERS([X11/extensions/dpmsconst.h])

PRESENT="no"
XORG_DRIVER_CHECK_EXT(PRESENT, presentproto)
if test "x$_EXT_CHECK" != "xno"; then
	PKG_CHECK_MODULES(PRESENT, [presentproto], [PRESENT="yes"], [])
fi
if test "x$PRESENT" != "xno"; then
	save_CFLAGS=$CFLAGS
	CFLAGS="$XORG_CFLAGS $PRESENT_CFLAGS"
	AC_CHECK_HEADERS([present.h], [], [PRESENT="no"], [
#include <xorg-server.h>
#include <xf86str.h>
])
	CFLAGS=$save_CFLAGS
fi
AC_MSG_CHECKING([whether to include PRESENT support])
AM_CONDITIONAL(PRESENT, test "x$PRESENT" != "xno")
AC_MSG_RESULT([$PRESENT])
if test "x$PRESENT" != "xno"; then
        AC_DEFINE(HAVE_PRESENT,1,[Enable PRESENT driver support])
	dri_msg="$dri_msg Present"
fi

AC_MSG_CHECKING([whether to include UXA support])
AC_MSG_RESULT([$UXA])
AM_CONDITIONAL(UXA, test "x$UXA" != "xno")

AC_MSG_CHECKING([whether to include GLAMOR support])
AC_MSG_RESULT([$GLAMOR])
AM_CONDITIONAL(GLAMOR, test "x$GLAMOR" != "xno")

AC_MSG_CHECKING([whether to include SNA support])
AM_CONDITIONAL(SNA, test "x$SNA" != "xno")
AC_MSG_RESULT([$SNA])

if test "$XVMC" = "yes"; then
	PKG_CHECK_MODULES(XVMCLIB, [xvmc dri2proto x11 x11-xcb xcb-dri2 xcb-aux libdrm_intel], [], [XVMC="no"])
fi
AC_MSG_CHECKING([whether to include XvMC support])
AC_MSG_RESULT([$XVMC])
AM_CONDITIONAL(XVMC, test "x$XVMC" = "xyes")
if test "x$XVMC" = "xyes"; then
	AC_DEFINE(ENABLE_XVMC,1,[Enable XvMC support])
	xvmc_msg=" yes"
else
	xvmc_msg=" no"
fi


AC_ARG_WITH(default-accel,
	    AS_HELP_STRING([--with-default-accel],
			   [Select the default acceleration method out of glamor, none, sna, or uxa  [default is sna if enabled, otherwise uxa]]),
			   [accel="$withval"],
			   [accel="auto"])
if test "x$accel" = "xyes"; then
	AC_MSG_WARN([No default acceleration specified, choosing automatic selection])
	accel="auto"
fi

AC_MSG_CHECKING([which acceleration method to use by default])
if test "x$accel" = "xauto"; then
	if test "x$SNA" != "xno"; then
		accel="sna"
	else
		if test "x$UXA" != "xno"; then
			accel="uxa"
		fi
	fi
	if test "x$accel" = "xauto" -a "x$KMS" = "xyes"; then
		AC_MSG_ERROR([No default acceleration option])
	fi
fi

have_accel="none"
if test "x$accel" = "xsna"; then
	if test "x$SNA" != "xno"; then
		AC_DEFINE(DEFAULT_ACCEL_METHOD, SNA, [Default acceleration method])
		have_accel="yes"
	else
		AC_MSG_ERROR([SNA requested as default, but is not enabled])
	fi
fi

if test "x$accel" = "xuxa"; then
	if test "x$UXA" != "xno"; then
		AC_DEFINE(DEFAULT_ACCEL_METHOD, UXA, [Default acceleration method])
		have_accel="yes"
	else
		AC_MSG_ERROR([UXA requested as default, but is not enabled])
	fi
fi

if test "x$accel" = "xglamor"; then
	if test "x$GLAMOR" != "xno"; then
		AC_DEFINE(DEFAULT_ACCEL_METHOD, GLAMOR, [Default acceleration method])
		have_accel="yes"
	else
		AC_MSG_ERROR([glamor acceleration requested as default, but is not enabled])
	fi
fi

if test "x$have_accel" = "xnone"; then
	if test "x$KMS" = "xyes"; then
		if test "x$SNA" != "xno" -o "x$UXA" != "xno"; then
			AC_DEFINE(DEFAULT_ACCEL_METHOD, NOACCEL, [Default acceleration method])
		else
			AC_MSG_ERROR([Invalid default acceleration option])
		fi
	fi
	accel="none"
fi
AC_MSG_RESULT($accel)

xp_msg=""

AC_ARG_ENABLE(tear-free,
	      AS_HELP_STRING([--enable-tear-free],
			     [Enable use of TearFree by default [default=no]]),
	      [TEARFREE="$enableval"],
	      [TEARFREE="no"])
if test "x$TEARFREE" = "xyes"; then
	AC_DEFINE(TEARFREE,1,[Enable "TearFree" by default])
	xp_msg="$xp_msg TearFree"
fi

AC_ARG_ENABLE(rendernode,
	      AS_HELP_STRING([--enable-rendernode],
			     [Enable use of render nodes (experimental) [default=no]]),
	      [RENDERNODE="$enableval"],
	      [RENDERNODE="no"])
AM_CONDITIONAL(USE_RENDERNODE, test "x$RENDERNODE" = "xyes")
if test "x$RENDERNODE" = "xyes"; then
	AC_DEFINE(USE_RENDERNODE,1,[Assume "rendernode" support])
	xp_msg="$xp_msg rendernode"
fi

AC_ARG_ENABLE(create2,
	      AS_HELP_STRING([--enable-create2],
			     [Enable use of create2 ioctl (experimental) [default=no]]),
	      [CREATE2="$enableval"],
	      [CREATE2="no"])
AM_CONDITIONAL(USE_CREATE2, test "x$CREATE2" = "xyes")
if test "x$CREATE2" = "xyes"; then
	AC_DEFINE(USE_CREATE2,1,[Assume "create2" support])
	xp_msg="$xp_msg create2"
fi

AC_ARG_ENABLE(async-swap,
	      AS_HELP_STRING([--enable-async-swap],
			     [Enable use of asynchronous swaps (experimental) [default=no]]),
	      [ASYNC_SWAP="$enableval"],
	      [ASYNC_SWAP="no"])
AM_CONDITIONAL(USE_ASYNC_SWAP, test "x$ASYNC_SWAP" = "xyes")
if test "x$ASYNC_SWAP" = "xyes"; then
	AC_DEFINE(USE_ASYNC_SWAP,1,[Assume asynchronous swap support])
	xp_msg="$xp_msg async-swap"
fi

AC_ARG_ENABLE(debug,
	      AS_HELP_STRING([--enable-debug],
			     [Enables internal debugging [default=no]]),
              [DEBUG="$enableval"],
              [DEBUG="no"])
AC_ARG_ENABLE(valgrind,
	      AS_HELP_STRING([--enable-valgrind],
			     [Enables valgrindified ioctls for debugging [default=no]]),
              [VG="$enableval"],
              [VG="no"])

LIBS=""
AC_SEARCH_LIBS(clock_gettime, rt, [CLOCK_GETTIME_LIBS=$LIBS])
AC_SUBST(CLOCK_GETTIME_LIBS)

sdkdir=`$PKG_CONFIG --variable=sdkdir xorg-server`

AM_CONDITIONAL(DEBUG, test "x$DEBUG" != "xno")
AM_CONDITIONAL(FULL_DEBUG, test "x$DEBUG" = "xfull")
if test "x$DEBUG" = "xno"; then
	AC_DEFINE(NDEBUG,1,[Disable internal debugging])
else
	if test "x$VG" != "xyes"; then
		VG=auto
	fi
fi
debug_msg=""
have_valgrind="no"
if test "x$VG" != "xno"; then
	PKG_CHECK_MODULES(VALGRIND, [valgrind], have_valgrind="yes", have_valgrind="no")
	AC_MSG_CHECKING([whether to include valgrind support])
	if test "x$have_valgrind" = "xyes"; then
		AC_DEFINE([HAVE_VALGRIND], 1, [Use valgrind intrinsics to suppress false warnings])
	else
		if test "x$VG" = "xyes"; then
			AC_MSG_ERROR([valgrind support requested, but valgrind-dev headers not found])
		fi
	fi
	AC_MSG_RESULT([$have_valgrind ($VG)])
fi
AM_CONDITIONAL(VALGRIND, test "x$have_valgrind" = "xyes")
if test "x$have_valgrind" = "xyes"; then
	debug_msg="$debug_msg valgrind"
fi
if test "x$DEBUG" = "xsync"; then
	AC_DEFINE(DEBUG_SYNC,1,[Enable synchronous rendering for debugging])
	debug_msg="$debug_msg sync"
fi
if test "x$DEBUG" = "xmemory"; then
	AC_DEFINE(DEBUG_MEMORY,1,[Enable memory debugging])
	debug_msg="$debug_msg memory"
fi
if test "x$DEBUG" = "xpixmap"; then
	AC_DEFINE(DEBUG_PIXMAP,1,[Enable pixmap debugging])
	debug_msg="$debug_msg pixmaps"
fi
if test "x$DEBUG" = "xfull"; then
	AC_DEFINE(DEBUG_MEMORY,1,[Enable memory debugging])
	AC_DEFINE(DEBUG_PIXMAP,1,[Enable pixmap debugging])
	AC_DEFINE(HAS_DEBUG_FULL,1,[Enable all debugging])
        CFLAGS="$CFLAGS -O0 -ggdb3"
	debug_msg=" full"
fi
if test "x$debug_msg" = "x"; then
	debug_msg=" none"
fi

AC_CONFIG_LIBOBJ_DIR(libobj)
AC_REPLACE_FUNCS(getline)

DRIVER_NAME="intel"
AC_SUBST([DRIVER_NAME])
AC_SUBST([moduledir])
AC_DEFINE_DIR([PREFIX_PATH], prefix, [installation prefix])
AC_DEFINE_DIR([LIBEXEC_PATH], libexecdir, [libexec directory])

AC_CONFIG_FILES([
                Makefile
                man/Makefile
		libobj/Makefile
                src/Makefile
                src/legacy/Makefile
                src/legacy/i810/Makefile
                src/legacy/i810/xvmc/Makefile
                src/render_program/Makefile
                src/sna/Makefile
                src/sna/brw/Makefile
                src/sna/fb/Makefile
                src/uxa/Makefile
                xvmc/Makefile
                xvmc/shader/Makefile
                xvmc/shader/mc/Makefile
                xvmc/shader/vld/Makefile
		test/Makefile
		tools/Makefile
		tools/org.x.xf86-video-intel.backlight-helper.policy
])
AC_OUTPUT

echo ""
echo ""
test -e `pwd $0`/README && cat `pwd $0`/README

accel_msg=""
if test "x$SNA" != "xno"; then
	if test "$accel" = "none"; then
		accel_msg="$accel_msg *none"
	else
		accel_msg="$accel_msg none"
	fi
	if test "$accel" = "sna"; then
		accel_msg="$accel_msg *sna"
	else
		accel_msg="$accel_msg sna"
	fi
fi
if test "x$UXA" != "xno"; then
	if test "x$SNA" = "xno"; then
		if test "$accel" = "none"; then
			accel_msg="$accel_msg *none"
		else
			accel_msg="$accel_msg none"
		fi
	fi
	if test "$accel" = "uxa"; then
		accel_msg="$accel_msg *uxa"
	else
		accel_msg="$accel_msg uxa"
	fi
fi
if test "x$GLAMOR" != "xno"; then
	if test "$accel" = "glamor"; then
		accel_msg="$accel_msg *glamor"
	else
		accel_msg="$accel_msg glamor"
	fi
fi

if test "x$dri_msg" = "x"; then
	dri_msg=" none"
fi

if test "x$tools_msg" = "x"; then
	tools_msg=" none"
fi

echo ""
echo "AC_PACKAGE_STRING will be compiled with:"
echo "  Xorg Video ABI version: $ABI_VERSION"
echo "  Acceleration backends:$accel_msg"
echo "  Additional debugging support?$debug_msg"
echo "  Support for Kernel Mode Setting? $KMS"
echo "  Support for legacy User Mode Setting (for i810)? $UMS"
echo "  Support for Direct Rendering Infrastructure:$dri_msg"
echo "  Support for Xv motion compensation (XvMC and libXvMC):$xvmc_msg"
echo "  Build additional tools and utilities?$tools_msg"
if test -n "$xp_msg"; then
echo "  Experimental support:$xp_msg"
fi
echo ""
