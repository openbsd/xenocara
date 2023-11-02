#	$OpenBSD: Makefile,v 1.5 2023/11/02 04:53:31 jsg Exp $

LIB=	intel_dev

NOPROFILE=

SRCS=	i915_intel_device_info.c \
	xe_intel_device_info.c \
	intel_debug.c \
	intel_device_info.c \
	intel_hwconfig.c \
	intel_kmd.c

SRCS+=	intel_wa.c

i915_intel_device_info.c:
	ln -s ${MESA_SRC}/src/intel/dev/i915/intel_device_info.c $@
xe_intel_device_info.c:
	ln -s ${MESA_SRC}/src/intel/dev/xe/intel_device_info.c $@

.include "../Makefile.inc"

CFLAGS+=	${C_VIS_ARGS} ${NO_OVERRIDE_INIT_ARGS}
CPPFLAGS+=	-I${MESA_SRC}/src/intel \
		-I${MESA_SRC}/src/intel/dev \
		-I${MESA_SRC}/generated/src \
		-I${MESA_SRC}/generated/src/intel \
		-I${MESA_SRC}/generated/src/intel/dev

install:

obj: _xenocara_obj

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

.PATH: ${MESA_SRC}/src/intel/dev
.PATH: ${MESA_SRC}/generated/src/intel/dev

CLEANFILES+=	i915_intel_device_info.c \
		xe_intel_device_info.c
