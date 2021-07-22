#	$OpenBSD: Makefile,v 1.3 2021/07/22 11:10:11 jsg Exp $

LIB=	isl

NOPROFILE=

SRCS=	isl.c \
	isl_aux_info.c \
	isl_drm.c \
	isl_format.c \
	isl_storage_image.c

SRCS+=	isl_format_layout.c

.include "../Makefile.inc"

CFLAGS+=	${C_VIS_ARGS} ${NO_OVERRIDE_INIT_ARGS}
CPPFLAGS+=	-I${MESA_SRC}/src/intel \
		-I${MESA_SRC}/src/mesa \
		-I${MESA_SRC}/src/mapi \
		-I${MESA_SRC}/src/gallium/include \
		-I${MESA_SRC}/generated/src/intel

install:

obj: _xenocara_obj

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

.PATH: ${MESA_SRC}/src/intel/isl
.PATH: ${MESA_SRC}/generated/src/intel/isl
