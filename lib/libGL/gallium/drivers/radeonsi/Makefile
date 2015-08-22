# $OpenBSD: Makefile,v 1.4 2015/08/22 00:50:54 jsg Exp $
.include <bsd.xconf.mk>

GALLIUM=${.CURDIR}/../../../../../dist/Mesa/src/gallium
.include "${GALLIUM}/drivers/radeonsi/Makefile.sources"

LIB = radeonsi

SRCS = \
	$(C_SOURCES)

CPPFLAGS+= -I${GALLIUM}/drivers/radeonsi \
	   -I${GALLIUM}/drivers -I${GALLIUM}/drivers/radeon \
	   -I${MESA}/../../include \
	   -I${MESA} -I${X11BASE}/include -I${X11BASE}/include/libdrm \
	   -I${MESA}/../gallium/winsys

CPPFLAGS+=	-DR600_USE_LLVM

PKG_CONFIG?=	usr/bin/pkg-config
ELF_CFLAGS!=	pkg-config --cflags libelf
#ELF_LDFLAGS!=	pkg-config --libs libelf
CPPFLAGS+= ${ELF_CFLAGS}

.include <bsd.xorg.mk>

.PATH: ${MESA}/program
.PATH: ${GALLIUM}/drivers/radeonsi
.PATH: ${GALLIUM}/drivers/radeon
