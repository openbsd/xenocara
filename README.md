# Notes on building Xenocara for OpenBSD X hackers

This document presents some techniques that can be useful for people
wanting to hack the xenocara tree. It assumes some basic knowledge of
the OpenBSD build system, as described in the release(8) manual page.

## About Xenocara

Xenocara is the name chosen for OpenBSD's version of X. It's
currently based on X.Org 7.7 and its dependencies. The goal of
Xenocara is to provide a framework to host local modifications and to
automate the build of the modular X.Org components, including 3rd
party packages and some software maintained by OpenBSD developers.

## Source tree

The organisation of the xenocara directory follows the general
organisation used in X.Org:

- app: X applications and utilities
- data: various data files (keyboard mappings and bitmaps)
- doc: documentation
- driver: input and video drivers
- font: fonts
- lib: libraries
- proto: X protocol headers
- util: utilities that don't fit anywhere else
- xserver: the source for the X servers

In addition Xenocara uses the following directories:

- dist: contains some of the 3rd party sources, when keeping them separate
  helps the build system (fontconfig, xcb and xkeyboard-config)
- distrib: all binary distribution related tools and data
- etc: some default config files
- share: make(1) configuration for Xenocara

At the top-level directory two files describe the individual
components of Xenocara:

- MODULES: lists all X.Org components (imported from the X.Org distribution
  at http://xorg.freedesktop.org/archive/)
- 3RDPARTY: lists all 3rd party software components provided in Xenocara,
  either as dependencies of the X.Org software, or as complements
  to it to provide a more useable default environment.

## Compiling and installing

Xenocara is made up of almost three hundred different independent
packages that need to be built and installed in the right order,
especially while bootstrapping (while /usr/X11R6 is still empty). The
Xenocara Makefiles take care of that using the 'build' target.

### Quick startup guide

The following steps will build and install everything for the first time.

    cd /usr/xenocara
    doas make bootstrap
    doas make obj
    doas make build

If you want to use another obj directory see below.

### Requirements

A freshly checked out xenocara tree is buildable without any external
tool. Only the xenocara and the src (currently only the
src/sys/dev/pci/pcidevs file) trees are needed.

However if you start modifying things in the automake build
system used by many packages, you will need to have the following
GNU autotools packages installed:

- automake 1.12 (devel/automake/1.12)
- autoconf 2.71 (devel/autoconf/2.71)
- metaauto (devel/metaauto)
- libtool (devel/libtool)
- gettext-tools (devel/gettext)

If you have your source tree on an NFS partition, make sure the clocks
of your server and client are properly synchronised. Any significant
drift will cause various problems during builds.

### Path

To build Xenocara, you need to have /usr/X11R6/bin in your PATH.

If you have installed the full Xenocara X sets on your system, you
don't need to build all of Xenocara to patch one element. You can go
to any module sub-directory and run 'make build' from there.

#### Source directory

The variable XSRCDIR can be set either in the environment or in
/etc/mk.conf to point to the xenocara source tree, in case you keep it
in a non-standard directory (the default is /usr/xenocara).

#### Objdirs

Xenocara requires objdirs. Just run 'make obj' as root at any level
before 'make build' to make sure that the object directories are
created. XOBJDIR defines the obj directory that is used (defaults to
/usr/xobj). It should be created before running 'make obj'.

## Regenerating configure scripts

Whenever you touched an import file for GNU autotools (Makefile.am,
configure.ac mostly), you need to rebuild the configure script and
makefiles skeletons. For that use the following command in the
directory where you edited the autotools source files:

    make -f Makefile.bsd-wrapper autoreconf
    doas make -f Makefile.bsd-wrapper build

# Cleaning in packages managed by autotools

One common problem when building xenocara is the case where the obj
directory didn't exist (or the symbolic link pointed to a non-existent
directory) when the source was first built. After fixing this problem,
'configure' will refuse to work in the obj dir, because the source
is already configured.

To recover from this in one package:

    rm -f obj
    make -f Makefile.bsd-wrapper cleandir
    mkdir XOBJDIR
    make -f Makefile.bsd-wrapper obj
    doas make -f Makefile.bsd-wrapper build

or from the root of the xenocara tree:

    find . -type l -name obj | xargs rm -f
    make cleandir
    mkdir XOBJDIR
    make obj
    doas make build

for more desperate cases, remove all files from XSRCDIR not in CVS:

    cd XSRCDIR
    cvs -q update -PAd -I - | awk '$1=="?" {print $2}' | xargs rm -f

# Updating XCB to a new release

libxcb uses C source files that are generated from the XML protocol
specification using xcbgen, written in Python. On OpenBSD those files
cannot be generated during a normal 'make build' since Python is not
in the base system. So the generated version are checked in CVS
(in lib/libxcb/src/). Here is the receipt to update them when updating
to a new release of XCB:

1. Update proto/xcb-proto.
2. Update the x11/py-xcbgen port to the same version and install the
   python3 package.
3. Update dist/libxcb.
4. Check lib/libxcb/src/Makefile if new files need to be generated.
5. Run make in lib/xcb/src to generate the files for the new version.
6. Check lib/libxcb/ for other files needing updates.
7. Commit the result.

## How to build something with debug information?

You can use env CFLAGS=-g make -f Makefile.bsd-wrapper build to
build any module with debugging information, but you'll need to remove
XOBJDIR/xorg-config.cache.${MACHINE} before doing that because
autoconf caches the value of CFLAGS in its cache.

## How to get a core file out of the X server?

Several things are needed:

1. set kern.nosuidcoredump=3 in /etc/sysctl.conf
2. start the X server as root, with the -keepPriv option. If you use
   xenodm, you can add the option in /etc/X11/xenodm/Xservers. If you
   want to use startx, you need to run it as root, like this:

    startx -- /usr/X11R6/bin/X -keepPriv

Now the X server should dump core when catching a fatal signal and the
core dump should be in /var/crash/Xorg/<pid>.core.

Alternatively, if the X server is using the modesetting(4) driver
(it's the case with most recent AMD and Intel GPUs), it can be started
as a regular user, without setting kern.nosuidcoredump=3, and the core
dump will be in the current directory where startx was executed.

See also http://xorg.freedesktop.org/wiki/Development/Documentation/ServerDebugging

--
$OpenBSD: README.md,v 1.2 2024/07/22 19:36:31 rsadowski Exp $
