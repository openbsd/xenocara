# $OpenBSD: Makefile,v 1.3 2010/05/08 14:15:29 matthieu Exp $
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

PKGCONFIG=	libdrm_intel.pc

LDADD+=		-L${X11BASE}/lib -lX11

includes:
	cd ${.CURDIR}; for i in ${INCS}; do \
	    j="cmp -s $$i ${DESTDIR}${INCSDIR}/$$i || \
		${INSTALL_DATA} $$i ${DESTDIR}${INCSDIR}/"; \
		echo "\tinstalling $$i"; \
		eval "$$j"; \
	done

NOPROFILE =

obj: _xenocara_obj

.include <bsd.lib.mk>
.include <bsd.xorg.mk>
