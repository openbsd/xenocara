# $OpenBSD: Makefile,v 1.1 2009/01/26 23:14:37 oga Exp $
.include <bsd.xconf.mk>

LIB=		drm_intel

INCSDIR=	${X11BASE}/include/

CPPFLAGS+=	-I${.CURDIR}			\
		-I${.CURDIR}/..			\
		-I${X11BASE}/include		\
		-I/usr/include/dev/pci/drm 

INCS= 		intel_bufmgr.h

SRCS=		intel_bufmgr.c		\
		intel_bufmgr_fake.c	\
		intel_bufmgr_gem.c	\
		mm.c

LDADD+=		-L${X11BASE}/lib -lX11

includes:
	cd ${.CURDIR}; for i in ${INCS}; do \
	    j="cmp -s $$i ${DESTDIR}${INCSDIR}/$$i || \
		${INSTALL} ${INSTALL_COPY} -o ${BINOWN} -g ${BINGRP} -m 444 \
		$$i ${DESTDIR}${INCSDIR}/"; \
		echo "\tinstalling $$i"; \
		eval "$$j"; \
	done

NOPROFILE =

obj: _xenocara_obj

.include <bsd.lib.mk>
.include <bsd.xorg.mk>
