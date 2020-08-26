#	$OpenBSD: Makefile,v 1.1 2020/08/26 06:02:38 jsg Exp $

LIB=	intel_perf

NOPROFILE=

SRCS=	gen_perf.c \
	gen_perf_query.c \
	gen_perf_mdapi.c

SRCS+=	gen_perf_metrics.c

.include "../Makefile.inc"

CFLAGS+=	${C_VIS_ARGS} ${NO_OVERRIDE_INIT_ARGS} -msse2
CPPFLAGS+=	-I${MESA_SRC}/src/intel \
		-I${MESA_SRC}/generated/src/intel

install:

obj: _xenocara_obj

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

.PATH: ${MESA_SRC}/src/intel/perf/
.PATH: ${MESA_SRC}/generated/src/intel/perf
