#	$OpenBSD: Makefile.inc,v 1.6 2009/06/04 16:11:25 matthieu Exp $

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

PACKAGE_VERSION!=m4 ${.CURDIR}/../../../share/mk/package_version.m4 ${LIBXCB}/configure.ac
