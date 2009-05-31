#	$OpenBSD: Makefile.inc,v 1.2 2009/05/31 18:01:28 matthieu Exp $

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

PACKAGE_VERSION!=m4 ${XSRCDIR}/share/mk/package_version.m4 ${XCBUTIL}/configure.ac
