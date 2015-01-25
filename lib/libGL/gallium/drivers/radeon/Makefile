# $OpenBSD: Makefile,v 1.1 2015/01/25 15:09:13 jsg Exp $
.include <bsd.xconf.mk>

GALLIUM=${.CURDIR}/../../../../../dist/Mesa/src/gallium
.include "${GALLIUM}/drivers/radeon/Makefile.sources"

LIB = radeon

SRCS = \
	$(C_SOURCES)

CPPFLAGS+= -I${GALLIUM}/drivers -I${GALLIUM}/drivers/radeon \
	   -I${MESA}/../../include -I${MESA}/../gallium/winsys \
	   -I${MESA} -I${X11BASE}/include -I/usr/include/dev/pci/drm

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
