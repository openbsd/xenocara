#	$OpenBSD: Makefile,v 1.4 2022/09/02 05:46:39 jsg Exp $

LIB=	intel_dev

NOPROFILE=

SRCS=	intel_debug.c \
	intel_device_info.c \
	intel_hwconfig.c

.include "../Makefile.inc"

CFLAGS+=	${C_VIS_ARGS} ${NO_OVERRIDE_INIT_ARGS}
CPPFLAGS+=	-I${MESA_SRC}/src/intel \
		-I${MESA_SRC}/generated/src

install:

obj: _xenocara_obj

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

.PATH: ${MESA_SRC}/src/intel/dev
