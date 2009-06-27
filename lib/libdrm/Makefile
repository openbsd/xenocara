# $OpenBSD: Makefile,v 1.4 2009/06/27 10:02:53 matthieu Exp $
.include <bsd.xconf.mk>
.include "${.CURDIR}/Makefile.inc"

.if ${MACHINE_ARCH} == "i386" || ${MACHINE_ARCH} == "amd64"
SUBDIR=intel
.endif

LIB=		drm

INCSDIR=	${X11BASE}/include/

CPP=		cpp -notraditional
DEBUG?=

CPPFLAGS+=	-I${.CURDIR} \
		-I${X11BASE}/include \
		-I/usr/include/dev/pci/drm \
		-DX_PRIVSEP

INCS= 		xf86drm.h	\
		xf86drmMode.h

SRCS=		xf86drm.c	\
		xf86drmHash.c	\
		xf86drmMode.c	\
		xf86drmRandom.c	\
		xf86drmSL.c	

includes: _SUBDIRUSE
	cd ${.CURDIR}; for i in ${INCS}; do \
	    j="cmp -s $$i ${DESTDIR}${INCSDIR}/$$i || \
		${INSTALL} ${INSTALL_COPY} -o ${BINOWN} -g ${BINGRP} -m 444 \
		$$i ${DESTDIR}${INCSDIR}/"; \
		echo "\tinstalling $$i"; \
		eval "$$j"; \
	done

NOPROFILE =

PKGCONFIG=	libdrm.pc

obj: _xenocara_obj

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

.include <bsd.subdir.mk>
