#	$OpenBSD: Makefile,v 1.1 2020/08/26 06:02:40 jsg Exp $

LIB=	megadriver_stub

NOPROFILE=

SRCS=	megadriver_stub.c

.include "../Makefile.inc"

CFLAGS+=	${C_VIS_ARGS}
CPPFLAGS+=	-I${MESA_SRC}/src/mesa/drivers/dri/common \
		-I${MESA_SRC}/src/mesa \
		-I${MESA_SRC}/src/mapi

install:

obj: _xenocara_obj

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

.PATH:	${MESA_SRC}/src/mesa/drivers/dri/common
