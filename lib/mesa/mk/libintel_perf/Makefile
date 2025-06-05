#	$OpenBSD: Makefile,v 1.4 2025/06/05 14:18:43 jsg Exp $

LIB=	intel_perf

NOPROFILE=

SRCS=	i915_intel_perf.c \
	xe_intel_perf.c \
	intel_perf_common.c \
	intel_perf.c \
	intel_perf_query.c \
	intel_perf_mdapi.c

SRCS+=	intel_perf_metrics.c

i915_intel_perf.c:
	ln -s ${MESA_SRC}/src/intel/perf/i915/intel_perf.c $@
xe_intel_perf.c:
	ln -s ${MESA_SRC}/src/intel/perf/xe/intel_perf.c $@

.include "../Makefile.inc"

CFLAGS+=	${C_VIS_ARGS} ${NO_OVERRIDE_INIT_ARGS} -msse2
CPPFLAGS+=	-I${MESA_SRC}/src/intel \
		-I${MESA_SRC}/src/intel/perf \
		-I${MESA_SRC}/generated/src \
		-I${MESA_SRC}/generated/src/intel

install:

obj: _xenocara_obj

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

.PATH: ${MESA_SRC}/src/intel/perf/
.PATH: ${MESA_SRC}/generated/src/intel/perf

CLEANFILES+=	i915_intel_perf.c \
		xe_intel_perf.c
