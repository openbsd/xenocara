#	$OpenBSD: Makefile,v 1.1 2021/02/11 10:34:51 jsg Exp $

LIB=	drm_radeon

SRCS=	radeon_bo.c \
	radeon_bo_gem.c \
	radeon_cs.c \
	radeon_cs_gem.c \
	radeon_cs_space.c \
	radeon_surface.c

.include "../Makefile.inc"

CPPFLAGS+=	-I${DRM_SRC}/radeon

LDADD+=	-L${.CURDIR}/../libdrm/${__objdir} -ldrm

obj: _xenocara_obj

beforeinstall:
	rm -f ${DESTDIR}${LIBDIR}/lib${LIB}.la

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

.PATH: ${DRM_SRC}/radeon
