#	$OpenBSD: Makefile.inc,v 1.2 2009/05/22 16:53:05 matthieu Exp $

LIBXCB=	${.CURDIR}/../../../dist/libxcb

CPPFLAGS=	-I${X11BASE}/include \
		-I${.CURDIR}/../src -I${LIBXCB}/src \
		-DHAVE_CONFIG_H

.PATH:	${LIBXCB}
.PATH:	${LIBXCB}/src
.PATH:	${.CURDIR}/../src

obj: _xenocara_obj

HEADERS_SUBDIR=	xcb/

_SRCDIR?=	${.CURDIR}/../src

PACKAGE_VERSION!=m4 ${XSRCDIR}/share/mk/package_version.m4 ${LIBXCB}/configure.ac
