# $OpenBSD: Makefile,v 1.1 2012/11/27 14:37:08 mpi Exp $
.include <bsd.xconf.mk>

LIB=		kms

INCSDIR=	${X11BASE}/include/

CPPFLAGS+=	-I${.CURDIR} \
		-I${X11BASE}/include \
		-I/usr/include/dev/pci/drm \
		-DX_PRIVSEP

INCS= 		libkms.h

SRCS=		api.c		\
		dumb.c		\
		intel.c		\
		linux.c		\
		radeon.c

PKGCONFIG=	libkms.pc

.include <bsd.lib.mk>
.include <bsd.xorg.mk>
