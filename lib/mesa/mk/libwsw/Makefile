#	$OpenBSD: Makefile,v 1.2 2025/06/05 14:18:52 jsg Exp $

LIB=	wsw

NOPROFILE=

SRCS=	wrapper_sw_winsys.c

.include "../Makefile.inc"

CFLAGS+=	${C_VIS_ARGS}
CPPFLAGS+=	-I${MESA_SRC}/src/gallium/include \
		-I${MESA_SRC}/src/gallium/auxiliary \
		-I${MESA_SRC}/generated/src

install:

obj: _xenocara_obj

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

.PATH: ${MESA_SRC}/src/gallium/winsys/sw/wrapper
