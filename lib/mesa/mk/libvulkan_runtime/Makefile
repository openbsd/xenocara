#	$OpenBSD: Makefile,v 1.1 2022/09/02 05:46:41 jsg Exp $

LIB=	vulkan_runtime

NOPROFILE=

SRCS=	vk_cmd_copy.c \
	vk_cmd_enqueue.c \
	vk_command_buffer.c \
	vk_command_pool.c \
	vk_debug_report.c \
	vk_debug_utils.c \
	vk_deferred_operation.c \
	vk_descriptors.c \
	vk_device.c \
	vk_fence.c \
	vk_framebuffer.c \
	vk_image.c \
	vk_instance.c \
	vk_log.c \
	vk_nir.c \
	vk_object.c \
	vk_physical_device.c \
	vk_queue.c \
	vk_render_pass.c \
	vk_semaphore.c \
	vk_shader_module.c \
	vk_sync.c \
	vk_sync_binary.c \
	vk_sync_dummy.c \
	vk_sync_timeline.c \
	vk_synchronization2.c

SRCS+=	vk_drm_syncobj.c \
	vk_common_entrypoints.c \
	vk_cmd_queue.c \
	vk_cmd_enqueue_entrypoints.c \
	vk_dispatch_trampolines.c \
	vk_physical_device_features.c

.include "../Makefile.inc"

CFLAGS+=	${C_VIS_ARGS}
CPPFLAGS+=	-I${MESA_SRC}/src/vulkan/runtime \
		-I${MESA_SRC}/src/vulkan/util \
		-I${MESA_SRC}/src/util \
		-I${MESA_SRC}/src/gallium/include \
		-I${MESA_SRC}/src/compiler/nir \
		-I${MESA_SRC}/generated/src \
		-I${MESA_SRC}/generated/src/compiler/nir \
		-I${MESA_SRC}/generated/src/vulkan/runtime \
		-I${MESA_SRC}/generated/src/vulkan/util

install:

obj: _xenocara_obj

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

.PATH: ${MESA_SRC}/src/vulkan/runtime
.PATH: ${MESA_SRC}/generated/src/vulkan/runtime
