#	$OpenBSD: Makefile.inc,v 1.6 2012/11/07 15:50:22 espie Exp $

XCBUTIL=	${.CURDIR}/../../../dist/xcb-util

CPPFLAGS+=	-I${XCBUTIL}/${SUBLIB} -I${X11BASE}/include

.SUFFIXES:	.in

NOPROFILE=

obj:		_xenocara_obj

HEADERS_SUBDIR=	xcb/

EXTRA_PKGCONFIG_SUBST=	'-e s,@LIBS@,,g'

LIB?=		xcb-${SUBLIB}
_SRCDIR?=	${XCBUTIL}/${SUBLIB}
PKGCONFIG?=	xcb-${SUBLIB}.pc

LDADD?=		`pkg-config --libs xcb`

.PATH:		${XCBUTIL}/${SUBLIB}

PACKAGE_VERSION =`m4 ${DESTDIR}${X11BASE}/share/mk/package_version.m4 ${XCBUTIL}/configure.ac`
