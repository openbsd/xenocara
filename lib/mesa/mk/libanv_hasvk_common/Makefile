#	$OpenBSD: Makefile,v 1.1 2023/01/28 08:56:34 jsg Exp $

LIB=	anv_hasvk_common

NOPROFILE=

SRCS=	anv_allocator.c \
	anv_android.h \
	anv_batch_chain.c \
	anv_blorp.c \
	anv_bo_sync.c \
	anv_cmd_buffer.c \
	anv_descriptor_set.c \
	anv_device.c \
	anv_formats.c \
	anv_genX.h \
	anv_image.c \
	anv_measure.c \
	anv_measure.h \
	anv_nir.h \
	anv_nir_add_base_work_group_id.c \
	anv_nir_apply_pipeline_layout.c \
	anv_nir_compute_push_layout.c \
	anv_nir_lower_multiview.c \
	anv_nir_lower_ubo_loads.c \
	anv_nir_lower_ycbcr_textures.c \
	anv_perf.c \
	anv_pipeline.c \
	anv_pipeline_cache.c \
	anv_private.h \
	anv_queue.c \
	anv_util.c \
	anv_utrace.c \
	anv_wsi.c

SRCS+=	anv_android_stubs.c

SRCS+=	anv_entrypoints.c

.include "../Makefile.inc"

CFLAGS+=	${C_VIS_ARGS} ${NO_OVERRIDE_INIT_ARGS} ${C_SSE2_ARGS}
CPPFLAGS+=	-I${MESA_SRC}/src/intel \
		-I${MESA_SRC}/src/intel/vulkan_hasvk \
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
		-I${MESA_SRC}/generated/src/intel/vulkan_hasvk \
		-I${MESA_SRC}/generated/src/util \
		-I${MESA_SRC}/generated/src/vulkan/runtime \
		-I${MESA_SRC}/generated/src/vulkan/util \
		-I${MESA_SRC}/generated/src/compiler/nir \
		-DVK_USE_PLATFORM_XCB_KHR \
		-DVK_USE_PLATFORM_XLIB_KHR \
		-DVK_USE_PLATFORM_DISPLAY_KHR \
		-DVK_USE_PLATFORM_XLIB_XRANDR_EXT

install:

obj: _xenocara_obj

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

.PATH: ${MESA_SRC}/src/intel/vulkan_hasvk
.PATH: ${MESA_SRC}/generated/src/intel/vulkan_hasvk
