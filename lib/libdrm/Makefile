# $OpenBSD: Makefile,v 1.9 2012/11/28 21:13:16 matthieu Exp $
.include <bsd.xconf.mk>
.include "${.CURDIR}/Makefile.inc"

.if ${MACHINE_ARCH} == "i386" || ${MACHINE_ARCH} == "amd64"
SUBDIR=intel radeon
.elif ${MACHINE} == "macppc"
SUBDIR=radeon
.endif

LIB=		drm

INCSDIR=	${X11BASE}/include/

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

PKGCONFIG=	libdrm.pc

obj: _xenocara_obj

.include <bsd.lib.mk>
.include <bsd.xorg.mk>
