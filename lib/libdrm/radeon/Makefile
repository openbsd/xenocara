# $OpenBSD: Makefile,v 1.1 2011/05/02 21:52:34 oga Exp $
.include <bsd.xconf.mk>

LIB=		drm_radeon

INCSDIR=	${X11BASE}/include/

CPPFLAGS+=	-I${.CURDIR}			\
		-I${.CURDIR}/..			\
		-I${X11BASE}/include		\
		-I/usr/include/dev/pci/drm 

INCS=		radeon_bo.h	\
		radeon_cs.h	\
		radeon_bo_gem.h	\
		radeon_cs_gem.h	\
		radeon_bo_int.h	\
		radeon_cs_int.h

SRCS=		radeon_bo_gem.c		\
		radeon_cs_gem.c		\
		radeon_cs_space.c	\
		radeon_bo.c		\
		radeon_cs.c		\
		bof.c


PKGCONFIG=	libdrm_radeon.pc

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
