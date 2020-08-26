#	$OpenBSD: Makefile,v 1.1 2020/08/26 06:02:38 jsg Exp $

LIB=	intel_common

NOPROFILE=

SRCS=	gen_aux_map.c \
	gen_batch_decoder.c \
	gen_decoder.c \
	gen_disasm.c \
	gen_l3_config.c \
	gen_urb_config.c \
	intel_log.c

.include "../Makefile.inc"

CFLAGS+=	${C_VIS_ARGS} ${NO_OVERRIDE_INIT_ARGS}
CPPFLAGS+=	-I${MESA_SRC}/src/intel \
		-I${MESA_SRC}/src/mesa \
		-I${MESA_SRC}/src/gallium/include \
		-I${MESA_SRC}/generated/src/intel

install:

obj: _xenocara_obj

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

.PATH: ${MESA_SRC}/src/intel/common
