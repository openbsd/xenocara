#	$OpenBSD: Makefile,v 1.1 2020/08/26 06:02:38 jsg Exp $

GEN=	100
LIB=	i965_gen${GEN}

NOPROFILE=

SRCS=	genX_blorp_exec.c \
	genX_pipe_control.c \
	genX_state_upload.c

.include "../Makefile.inc"

CFLAGS+=	${C_VIS_ARGS} ${NO_OVERRIDE_INIT_ARGS} ${C_SSE2_ARGS}
CPPFLAGS+=	-I${MESA_SRC}/src/mesa/drivers/dri/i965 \
		-I${MESA_SRC}/src/mesa/drivers/dri/common \
		-I${MESA_SRC}/src/mesa \
		-I${MESA_SRC}/src/mapi \
		-I${MESA_SRC}/src/intel \
		-I${MESA_SRC}/src/gallium/include \
		-I${MESA_SRC}/generated/src/compiler/nir \
		-I${MESA_SRC}/generated/src/intel \
		-DGEN_VERSIONx10=${GEN}

install:

obj: _xenocara_obj

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

.PATH: ${MESA_SRC}/src/mesa/drivers/dri/i965
