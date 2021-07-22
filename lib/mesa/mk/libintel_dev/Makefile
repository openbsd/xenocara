#	$OpenBSD: Makefile,v 1.2 2021/07/22 11:10:10 jsg Exp $

LIB=	intel_dev

NOPROFILE=

SRCS=	gen_debug.c \
	gen_device_info.c

.include "../Makefile.inc"

CFLAGS+=	${C_VIS_ARGS} ${NO_OVERRIDE_INIT_ARGS}
CPPFLAGS+=	-I${MESA_SRC}/src/intel \
		-I${MESA_SRC}/generated/src

install:

obj: _xenocara_obj

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

.PATH: ${MESA_SRC}/src/intel/dev
