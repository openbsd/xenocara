#	$OpenBSD: Makefile,v 1.6 2025/06/05 14:18:42 jsg Exp $

LIB=	intel_common

NOPROFILE=

SRCS=	i915_intel_engine.c \
	i915_intel_gem.c \
	intel_device_query.c \
	xe_intel_engine.c \
	xe_intel_gem.c \
	intel_queue.c \
	intel_aux_map.c \
	intel_bind_timeline.c \
	intel_common.c \
	intel_compute_slm.c \
	intel_debug_identifier.c \
	intel_engine.c \
	intel_gem.c \
	intel_l3_config.c \
	intel_measure.c \
	intel_mem.c \
	intel_sample_positions.c \
	intel_urb_config.c \
	intel_uuid.c

SRCS+=	intel_clflushopt.c
CFLAGS+= -mclflushopt

i915_intel_engine.c:
	ln -s ${MESA_SRC}/src/intel/common/i915/intel_engine.c $@
i915_intel_gem.c:
	ln -s ${MESA_SRC}/src/intel/common/i915/intel_gem.c $@
xe_intel_engine.c:
	ln -s ${MESA_SRC}/src/intel/common/xe/intel_engine.c $@
xe_intel_gem.c:
	ln -s ${MESA_SRC}/src/intel/common/xe/intel_gem.c $@

.include "../Makefile.inc"

CFLAGS+=	${C_VIS_ARGS} ${NO_OVERRIDE_INIT_ARGS} ${C_SSE2_ARGS}
CPPFLAGS+=	-I${MESA_SRC}/src/intel \
		-I${MESA_SRC}/src/intel/common \
		-I${MESA_SRC}/generated/src \
		-I${MESA_SRC}/generated/src/intel

install:

obj: _xenocara_obj

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

.PATH: ${MESA_SRC}/src/intel/common
.PATH: ${MESA_SRC}/src/intel/common/xe

CLEANFILES+=	i915_intel_engine.c \
		i915_intel_gem.c \
		xe_intel_engine.c \
		xe_intel_gem.c
