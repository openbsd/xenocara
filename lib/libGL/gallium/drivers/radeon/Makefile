# $OpenBSD: Makefile,v 1.2 2015/08/22 00:50:54 jsg Exp $
.include <bsd.xconf.mk>

GALLIUM=${.CURDIR}/../../../../../dist/Mesa/src/gallium
.include "${GALLIUM}/drivers/radeon/Makefile.sources"

LIB = radeon

SRCS = \
	$(C_SOURCES)

CPPFLAGS+= -I${GALLIUM}/drivers -I${GALLIUM}/drivers/radeon \
	   -I${MESA}/../../include -I${MESA}/../gallium/winsys \
	   -I${MESA} -I${X11BASE}/include -I${X11BASE}/include/libdrm

.if ${XENOCARA_BUILD_GALLIUM} == "llvm"
SRCS+=		${LLVM_C_FILES}

PKG_CONFIG?=	usr/bin/pkg-config
ELF_CFLAGS!=	pkg-config --cflags libelf
#ELF_LDFLAGS!=	pkg-config --libs libelf
CPPFLAGS+= ${ELF_CFLAGS}
.endif

.include <bsd.xorg.mk>

.PATH: ${MESA}/program
.PATH: ${GALLIUM}/drivers/radeon
