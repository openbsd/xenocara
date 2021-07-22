#	$OpenBSD: Makefile,v 1.2 2021/07/22 11:10:10 jsg Exp $

LIB=	intel_common

NOPROFILE=

SRCS=	intel_aux_map.c \
	intel_batch_decoder.c \
	intel_decoder.c \
	intel_disasm.c \
	intel_gem.c \
	intel_l3_config.c \
	intel_measure.c \
	intel_sample_positions.c \
	intel_urb_config.c \
	intel_uuid.c

.include "../Makefile.inc"

CFLAGS+=	${C_VIS_ARGS} ${NO_OVERRIDE_INIT_ARGS}
CPPFLAGS+=	-I${MESA_SRC}/src/intel \
		-I${MESA_SRC}/src/mesa \
		-I${MESA_SRC}/src/mapi \
		-I${MESA_SRC}/src/gallium/include \
		-I${MESA_SRC}/generated/src \
		-I${MESA_SRC}/generated/src/intel

install:

obj: _xenocara_obj

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

.PATH: ${MESA_SRC}/src/intel/common
