#	$OpenBSD: Makefile,v 1.1 2020/08/26 06:02:38 jsg Exp $

LIB=	intel_dev

NOPROFILE=

SRCS=	gen_debug.c \
	gen_device_info.c

.include "../Makefile.inc"

CFLAGS+=	${C_VIS_ARGS} ${NO_OVERRIDE_INIT_ARGS}
CPPFLAGS+=	-I${MESA_SRC}/src/intel

install:

obj: _xenocara_obj

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

.PATH: ${MESA_SRC}/src/intel/dev
