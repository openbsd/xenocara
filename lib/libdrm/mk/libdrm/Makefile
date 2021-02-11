#	$OpenBSD: Makefile,v 1.1 2021/02/11 10:34:51 jsg Exp $

LIB=	drm

SRCS=	xf86drm.c \
	xf86drmHash.c \
	xf86drmMode.c \
	xf86drmRandom.c \
	xf86drmSL.c

obj: _xenocara_obj

beforeinstall:
	rm -f ${DESTDIR}${LIBDIR}/lib${LIB}.la

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

.PATH: ${DRM_SRC}
