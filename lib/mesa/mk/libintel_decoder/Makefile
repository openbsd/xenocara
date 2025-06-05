#	$OpenBSD: Makefile,v 1.1 2025/06/05 14:18:43 jsg Exp $

LIB=	intel_decoder

NOPROFILE=

SRCS=	intel_decoder.c \
	intel_batch_decoder.c

SRCS+=	intel_batch_decoder_brw.c \
	intel_batch_decoder_elk.c

.include "../Makefile.inc"

CFLAGS+=	${C_VIS_ARGS} ${NO_OVERRIDE_INIT_ARGS}
CPPFLAGS+=	-I${MESA_SRC}/src/intel \
		-I${MESA_SRC}/generated/src \
		-I${MESA_SRC}/generated/src/intel

install:

obj: _xenocara_obj

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

.PATH: ${MESA_SRC}/src/intel/decoder
