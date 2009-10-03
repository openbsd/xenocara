#	$OpenBSD: Makefile.inc,v 1.7 2009/10/03 13:57:56 matthieu Exp $

LIBXCB=	${.CURDIR}/../../../dist/libxcb

CPPFLAGS=	-I${X11BASE}/include \
		-I${.CURDIR}/../src -I${LIBXCB}/src \
		-DHAVE_CONFIG_H

LDADD?=		`pkg-config --libs xcb`

NOPROFILE=

.SUFFIXES: .in

.PATH:	${LIBXCB}/src
.PATH:	${.CURDIR}/../src
.PATH.in: ${LIBXCB}
.PATH.xml: ${DATADIR}/xcb

obj: _xenocara_obj

HEADERS_SUBDIR=	xcb/

_SRCDIR?=	${.CURDIR}/../src

PACKAGE_VERSION!=m4 ${.CURDIR}/../../../share/mk/package_version.m4 ${LIBXCB}/configure.ac
