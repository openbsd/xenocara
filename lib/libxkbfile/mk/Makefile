# $OpenBSD: Makefile,v 1.1 2026/05/14 14:21:28 matthieu Exp $

.include <bsd.own.mk>

PACKAGE_VERSION = 1.2.0

.ifmake install
SUBDIR = include/X11/extensions

.endif

LIB = xkbfile

TOP = ${.CURDIR}/..
_SRCDIR = $(TOP)

NOPROFILE =

SRCS = \
	cout.c \
	maprules.c \
	srvmisc.c \
	xkbatom.c \
	xkbbells.c \
	xkbconfig.c \
	xkbdraw.c \
	xkberrs.c \
	xkbmisc.c \
	xkbout.c \
	xkbtext.c \
	xkmout.c \
	xkmread.c


CPPFLAGS += -DHAVE_CONFIG_H -I${.CURDIR} -I${_SRCDIR} \
	-I${_SRCDIR}/include -I${_SRCDIR}/include/X11/extensions \
	-I${X11BASE}/include

CFLAGS += -std=gnu99  \
        -Wall  -Winvalid-pch -Winline -Wundef -Wno-unused-local-typedefs

PKGCONFIG = xkbfile.pc

obj: _xenocara_obj

.PATH: $(TOP)
.PATH: $(TOP)/src
.PATH: $(TOP)/include/X11/extensions

.include <bsd.lib.mk>
.include <bsd.xorg.mk>
.include <bsd.subdir.mk>
