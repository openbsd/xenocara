#	$OpenBSD: Makefile,v 1.4 2023/01/28 08:56:33 jsg Exp $

LIB=	addrlib

NOPROFILE=

SRCS=	addrinterface.cpp \
	addrelemlib.cpp \
	addrlib.cpp \
	addrlib1.cpp \
	addrlib2.cpp \
	addrobject.cpp \
	coord.cpp \
	gfx9addrlib.cpp \
	gfx10addrlib.cpp \
	gfx11addrlib.cpp \
	ciaddrlib.cpp \
	egbaddrlib.cpp \
	siaddrlib.cpp

.include "../Makefile.inc"

CXXFLAGS+=	${CXX_VIS_ARGS} -Wno-unused-variable -Wno-unused-local-typedefs \
		-Wno-unused-but-set-variable -Wno-sometimes-uninitialized \
		-Wno-self-assign
CPPFLAGS+=	-I${MESA_SRC}/src/gallium/include \
		-I${MESA_SRC}/src/gallium/auxiliary \
		-I${MESA_SRC}/src/amd/addrlib/inc \
		-I${MESA_SRC}/src/amd/addrlib/src \
		-I${MESA_SRC}/src/amd/addrlib/src/core \
		-I${MESA_SRC}/src/amd/addrlib/src/gfx9 \
		-I${MESA_SRC}/src/amd/addrlib/src/gfx10 \
		-I${MESA_SRC}/src/amd/addrlib/src/gfx11 \
		-I${MESA_SRC}/src/amd/addrlib/src/r800 \
		-I${MESA_SRC}/src/amd/addrlib/src/chip/gfx9 \
		-I${MESA_SRC}/src/amd/addrlib/src/chip/gfx10 \
		-I${MESA_SRC}/src/amd/addrlib/src/chip/gfx11 \
		-I${MESA_SRC}/src/amd/addrlib/src/chip/r800

install:

obj: _xenocara_obj

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

.PATH: ${MESA_SRC}/src/amd/addrlib/src
.PATH: ${MESA_SRC}/src/amd/addrlib/src/core
.PATH: ${MESA_SRC}/src/amd/addrlib/src/gfx9
.PATH: ${MESA_SRC}/src/amd/addrlib/src/gfx10
.PATH: ${MESA_SRC}/src/amd/addrlib/src/gfx11
.PATH: ${MESA_SRC}/src/amd/addrlib/src/r800
