# $OpenBSD: Makefile,v 1.2 2009/01/26 23:14:37 oga Exp $
.include <bsd.xconf.mk>

.if ${MACHINE_ARCH} == "i386" || ${MACHINE_ARCH} == "amd64"
SUBDIR=intel
.endif

LIB=		drm

DRM_MAJOR=	2
DRM_MINOR=	4
DRM_TINY=	4

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

obj: _xenocara_obj

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

libdrm.pc: libdrm.pc.in Makefile
	sed -e 's,@prefix@,$(X11BASE),' \
	    -e 's,@exec_prefix@,${X11BASE},' \
	    -e 's,@libdir@,${LIBDIR},' \
	    -e 's,@includedir@,${INCSDIR},' \
	    -e 's,@PACKAGE_VERSION@,$(DRM_MAJOR).$(DRM_MINOR).$(DRM_TINY),' \
	< ${.CURDIR}/libdrm.pc.in > libdrm.pc

afterinstall: libdrm.pc
	$(INSTALL) -c -m 644 -o root -g wheel libdrm.pc \
		${DESTDIR}${LIBDIR}/pkgconfig

CLEANFILES+= libdrm.pc

.include <bsd.subdir.mk>
