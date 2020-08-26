#	$OpenBSD: Makefile,v 1.1 2020/08/26 06:02:41 jsg Exp $

LIB=	ws_null

NOPROFILE=

SRCS=	null_sw_winsys.c

.include "../Makefile.inc"

CFLAGS+=	${C_VIS_ARGS}
CPPFLAGS+=	-I${MESA_SRC}/src/gallium/include \
		-I${MESA_SRC}/src/gallium/auxiliary

install:

obj: _xenocara_obj

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

.PATH: ${MESA_SRC}/src/gallium/winsys/sw/null
