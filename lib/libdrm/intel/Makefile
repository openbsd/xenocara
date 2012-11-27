# $OpenBSD: Makefile,v 1.4 2012/11/27 14:37:08 mpi Exp $
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

LDADD+=		-L${X11BASE}/lib -lX11 -lpciaccess

.include <bsd.lib.mk>
.include <bsd.xorg.mk>
