# $OpenBSD: Makefile,v 1.2 2015/08/22 00:50:54 jsg Exp $

GALLIUM=${.CURDIR}/../../../../../../dist/Mesa/src/gallium
.include "${GALLIUM}/winsys/radeon/drm/Makefile.sources"

LIB = radeonwinsys

SRCS = \
	$(C_SOURCES)

CPPFLAGS+= \
	-I${GALLIUM}/auxiliary \
	-I${GALLIUM}/include \
	-I${.CURDIR}/../../../../../../dist/Mesa/include \
	-I${X11BASE}/include \
	-I${X11BASE}/include/libdrm

all: lib${LIB}_pic.a

obj: _xenocara_obj

install:
	@echo "Not installing lib${LIB}"

clean:
	rm -f lib${LIB}_pic.a ${OBJS}

cleandir:	clean

.PATH: ${GALLIUM}/winsys/radeon/drm

.include <bsd.xorg.mk>
.include "../../../Makefile.inc"
