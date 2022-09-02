#	$OpenBSD: Makefile,v 1.2 2022/09/02 05:46:37 jsg Exp $

LIB=	blorp

NOPROFILE=

SRCS=	blorp.c \
	blorp_blit.c \
	blorp_clear.c

.include "../Makefile.inc"

CFLAGS+=	${C_VIS_ARGS} ${NO_OVERRIDE_INIT_ARGS}
CPPFLAGS+=	-I${MESA_SRC}/src/intel \
		-I${MESA_SRC}/src/mesa \
		-I${MESA_SRC}/src/mapi \
		-I${MESA_SRC}/src/gallium/include \
		-I${MESA_SRC}/src/compiler/nir \
		-I${MESA_SRC}/generated/src/intel \
		-I${MESA_SRC}/generated/src/compiler/nir

install:

obj: _xenocara_obj

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

.PATH: ${MESA_SRC}/src/intel/blorp
