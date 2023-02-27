#	$OpenBSD: Makefile,v 1.2 2023/02/27 12:46:20 jsg Exp $

LIB=	drm_intel

SRCS=	intel_bufmgr.c \
	intel_bufmgr_fake.c \
	intel_bufmgr_gem.c \
	intel_decode.c \
	mm.c

.include "../Makefile.inc"

CPPFLAGS+=	-I${DRM_SRC}/intel \
		-I${X11BASE}/include

LDADD+=	-L${.CURDIR}/../libdrm/${__objdir} -ldrm \
	-L${X11BASE}/lib -lpciaccess

obj: _xenocara_obj

beforeinstall:
	rm -f ${DESTDIR}${LIBDIR}/lib${LIB}.la

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

.PATH: ${DRM_SRC}/intel
