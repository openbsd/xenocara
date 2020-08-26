#	$OpenBSD: Makefile,v 1.1 2020/08/26 06:02:37 jsg Exp $

LIB=	dricommon

NOPROFILE=

SRCS=	utils.c \
	dri_util.c

.include "../Makefile.inc"

CFLAGS+=	${C_VIS_ARGS}
CPPFLAGS+=	-I${MESA_SRC}/src/mesa/drivers/dri/common \
		-I${MESA_SRC}/src/mesa \
		-I${MESA_SRC}/src/mapi \
		-I${MESA_SRC}/generated/src/util

install:

obj: _xenocara_obj

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

.PATH:	${MESA_SRC}/src/mesa/drivers/dri/common
