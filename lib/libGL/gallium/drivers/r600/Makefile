# $OpenBSD: Makefile,v 1.5 2015/08/22 00:50:54 jsg Exp $
.include <bsd.xconf.mk>

GALLIUM=${.CURDIR}/../../../../../dist/Mesa/src/gallium
.include "${GALLIUM}/drivers/r600/Makefile.sources"

LIB = r600

SRCS = \
	${C_SOURCES} \
	${CXX_SOURCES:S/sb\///g}

CPPFLAGS+= -I${GALLIUM}/drivers/r600 -I${GALLIUM}/drivers/r600/sb \
	   -I${GALLIUM}/drivers -I${GALLIUM}/drivers/radeon \
	   -I${MESA}/../../include \
	   -I${MESA} -I${X11BASE}/include -I${X11BASE}/include/libdrm \
	   -I${MESA}/../gallium/winsys

.if ${XENOCARA_BUILD_GALLIUM} == "llvm"
SRCS+=		${LLVM_C_SOURCES}
CPPFLAGS+=	-DR600_USE_LLVM

PKG_CONFIG?=	usr/bin/pkg-config
ELF_CFLAGS!=	pkg-config --cflags libelf
#ELF_LDFLAGS!=	pkg-config --libs libelf
CPPFLAGS+= ${ELF_CFLAGS}
.endif

.include <bsd.xorg.mk>

.PATH: ${MESA}/program
.PATH: ${GALLIUM}/drivers/r600
.PATH: ${GALLIUM}/drivers/r600/sb
.PATH: ${GALLIUM}/drivers/radeon
