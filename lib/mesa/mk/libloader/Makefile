#	$OpenBSD: Makefile,v 1.4 2023/11/02 04:53:32 jsg Exp $

LIB=	loader

NOPROFILE=

SRCS=	loader_dri_helper.c \
	loader.c

.include "../Makefile.inc"

CFLAGS+=	${C_VIS_ARGS}
CPPFLAGS+=	-DUSE_DRICONF \
		-DDEFAULT_DRIVER_DIR=\"${X11BASE}/lib/modules/dri\" \
		-I${MESA_SRC}/src/loader \
		-I${MESA_SRC}/generated/src \
		-I${MESA_SRC}/generated/src/util

install:

obj: _xenocara_obj

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

.PATH: ${MESA_SRC}/src/loader
