#	$OpenBSD: Makefile,v 1.2 2023/01/28 08:56:34 jsg Exp $

LIB=	intel-driver-ds

NOPROFILE=

SRCS=	intel_driver_ds.cc

SRCS+=	intel_tracepoints.c

.include "../Makefile.inc"

CFLAGS+=	${C_VIS_ARGS} ${NO_OVERRIDE_INIT_ARGS}
CXXFLAGS+=	${CXX_VIS_ARGS}
CPPFLAGS+=	-I${MESA_SRC}/src/intel \
		-I${MESA_SRC}/src/intel/ds \
		-I${MESA_SRC}/src/mesa \
		-I${MESA_SRC}/src/mapi \
		-I${MESA_SRC}/src/gallium/include \
		-I${MESA_SRC}/generated/src/intel/ds \
		-I${MESA_SRC}/generated/src/compiler/nir

install:

obj: _xenocara_obj

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

.PATH: ${MESA_SRC}/src/intel/ds
.PATH: ${MESA_SRC}/generated/src/intel/ds
