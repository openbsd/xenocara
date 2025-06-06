#	$OpenBSD: Makefile,v 1.2 2025/06/06 09:04:09 jsg Exp $

LIB=	drm

SRCS=	xf86drm.c \
	xf86drmHash.c \
	xf86drmMode.c \
	xf86drmRandom.c \
	xf86drmSL.c

NOPROFILE=

obj: _xenocara_obj

beforeinstall:
	rm -f ${DESTDIR}${LIBDIR}/lib${LIB}.la

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

.PATH: ${DRM_SRC}
