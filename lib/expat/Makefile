# $OpenBSD: Makefile,v 1.4 2006/12/02 16:28:48 matthieu Exp $
# $OpenBSD: Makefile,v 1.4 2006/12/02 16:28:48 matthieu Exp $
.include <bsd.own.mk>

DIST= ${.CURDIR}

LIB=		expat
SRCS=		xmlparse.c xmltok.c xmlrole.c
CFLAGS+=	-I${.CURDIR}
CPPFLAGS+=	-DHAVE_EXPAT_CONFIG_H

test:
	@echo "dist $(DIST)/lib"

includes:
	@cmp -s ${DESTDIR}${X11BASE}/include/expat.h \
	  ${DIST}/lib/expat.h || \
	  ${INSTALL} ${INSTALL_COPY} -m 444 -o $(BINOWN) -g $(BINGRP) \
	  ${DIST}/lib/expat.h ${DESTDIR}${X11BASE}/include/expat.h
	@cmp -s ${DESTDIR}${X11BASE}/include/expat_external.h \
	  ${DIST}/lib/expat_external.h || \
	  ${INSTALL} ${INSTALL_COPY} -m 444 -o $(BINOWN) -g $(BINGRP) \
	  ${DIST}/lib/expat_external.h \
	  ${DESTDIR}${X11BASE}/include/expat_external.h

obj: _xenocara_obj

NOPROFILE=

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

.PATH: ${DIST}/lib
