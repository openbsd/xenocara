#	$OpenBSD: Makefile.inc,v 1.3 2009/07/18 20:54:10 matthieu Exp $

XCBUTIL=	${.CURDIR}/../../../dist/xcb-util

CPPFLAGS+=	-I${X11BASE}/include

.SUFFIXES:	.in

obj:		_xenocara_obj

HEADERS_SUBDIR=	xcb/

EXTRA_PKGCONFIG_SUBST=	'-e s,@LIBS@,,g'

LIB?=		xcb-${SUBLIB}
_SRCDIR?=	${XCBUTIL}/${SUBLIB}
PKGCONFIG?=	xcb-${SUBLIB}.pc

LDADD?=		`pkg-config --libs xcb`

.PATH:		${XCBUTIL}/${SUBLIB}

PACKAGE_VERSION!=m4 ${.CURDIR}/../../../share/mk/package_version.m4 ${XCBUTIL}/configure.ac
