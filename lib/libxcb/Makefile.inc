#	$OpenBSD: Makefile.inc,v 1.1.1.1 2009/05/22 16:09:01 matthieu Exp $

LIBXCB=	${.CURDIR}/../../../dist/libxcb

CPPFLAGS=	-I${X11BASE}/include \
		-I${.CURDIR}/../src -I${LIBXCB}/src \
		-DHAVE_CONFIG_H

.PATH:	${LIBXCB}
.PATH:	${LIBXCB}/src
.PATH:	${.CURDIR}/../src

obj: _xenocara_obj

includes:
	@cd ${.CURDIR}/../src; for i in ${HDRS}; do \
	  cmp -s $$i ${DESTDIR}${INCSDIR}/xcb/$$i || \
	  ${INSTALL} ${INSTALL_COPY} -m 444 -o ${BINOWN} -g ${BINGRP} $$i \
	  ${DESTDIR}/${INCSDIR}/xcb; done

PACKAGE_VERSION!=m4 ${XSRCDIR}/share/mk/package_version.m4 ${LIBXCB}/configure.ac
