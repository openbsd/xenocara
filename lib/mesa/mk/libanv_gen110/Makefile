#	$OpenBSD: Makefile,v 1.5 2024/04/02 10:42:12 jsg Exp $

GEN=	110
LIB=	anv_gen${GEN}

NOPROFILE=

SRCS=	genX_blorp_exec.c \
	genX_cmd_buffer.c \
	genX_gfx_state.c \
	genX_gpu_memcpy.c \
	genX_init_state.c \
	genX_pipeline.c \
	genX_query.c \
	genX_simple_shader.c \
	genX_video.c

.include "../Makefile.inc"

CFLAGS+=	${C_VIS_ARGS} ${NO_OVERRIDE_INIT_ARGS} ${C_SSE2_ARGS}
CPPFLAGS+=	-I${MESA_SRC}/src/intel \
		-I${MESA_SRC}/src/intel/vulkan \
		-I${MESA_SRC}/src/mesa \
		-I${MESA_SRC}/src/mapi \
		-I${MESA_SRC}/src/gallium/include \
		-I${MESA_SRC}/src/vulkan/runtime \
		-I${MESA_SRC}/src/vulkan/util \
		-I${MESA_SRC}/src/vulkan/wsi \
		-I${MESA_SRC}/src/compiler \
		-I${MESA_SRC}/src/compiler/nir \
		-I${MESA_SRC}/generated/src \
		-I${MESA_SRC}/generated/src/intel \
		-I${MESA_SRC}/generated/src/intel/ds \
		-I${MESA_SRC}/generated/src/intel/vulkan \
		-I${MESA_SRC}/generated/src/util \
		-I${MESA_SRC}/generated/src/vulkan/runtime \
		-I${MESA_SRC}/generated/src/vulkan/util \
		-I${MESA_SRC}/generated/src/compiler \
		-I${MESA_SRC}/generated/src/compiler/nir \
		-DVK_USE_PLATFORM_XCB_KHR \
		-DVK_USE_PLATFORM_XLIB_KHR \
		-DVK_USE_PLATFORM_DISPLAY_KHR \
		-DVK_USE_PLATFORM_XLIB_XRANDR_EXT \
		-DGFX_VERx10=${GEN} \
		-DANV_SUPPORT_RT=0

install:

obj: _xenocara_obj

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

.PATH: ${MESA_SRC}/src/intel/vulkan
.PATH: ${MESA_SRC}/generated/src/intel/vulkan
