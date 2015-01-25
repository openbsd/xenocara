# $OpenBSD: Makefile,v 1.1 2015/01/25 15:09:13 jsg Exp $

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
	-I/usr/include/dev/pci/drm

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
