#	$OpenBSD: Makefile.inc,v 1.5 2009/05/31 18:01:28 matthieu Exp $

LIBXCB=	${.CURDIR}/../../../dist/libxcb

CPPFLAGS=	-I${X11BASE}/include \
		-I${.CURDIR}/../src -I${LIBXCB}/src \
		-DHAVE_CONFIG_H

LDADD?=		`pkg-config --libs xcb`

.SUFFIXES: .in

.PATH:	${LIBXCB}/src
.PATH:	${.CURDIR}/../src
.PATH.in: ${LIBXCB}
.PATH.xml: ${DATADIR}/xcb

obj: _xenocara_obj

HEADERS_SUBDIR=	xcb/

_SRCDIR?=	${.CURDIR}/../src

PACKAGE_VERSION!=m4 ${XSRCDIR}/share/mk/package_version.m4 ${LIBXCB}/configure.ac
