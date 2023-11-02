#	$OpenBSD: Makefile,v 1.3 2023/11/02 04:53:31 jsg Exp $

LIB=	intel_perf

NOPROFILE=

SRCS=	intel_perf.c \
	intel_perf_query.c \
	intel_perf_mdapi.c

SRCS+=	intel_perf_metrics.c

.include "../Makefile.inc"

CFLAGS+=	${C_VIS_ARGS} ${NO_OVERRIDE_INIT_ARGS} -msse2
CPPFLAGS+=	-I${MESA_SRC}/src/intel \
		-I${MESA_SRC}/generated/src \
		-I${MESA_SRC}/generated/src/intel

install:

obj: _xenocara_obj

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

.PATH: ${MESA_SRC}/src/intel/perf/
.PATH: ${MESA_SRC}/generated/src/intel/perf
