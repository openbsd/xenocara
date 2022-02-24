#	$OpenBSD: Makefile,v 1.3 2022/02/24 02:29:49 jsg Exp $

LIB=	vulkan_util

NOPROFILE=

SRCS=	vk_alloc.c \
	vk_cmd_copy.c \
	vk_command_buffer.c \
	vk_debug_report.c \
	vk_debug_utils.c \
	vk_deferred_operation.c \
	vk_descriptors.c \
	vk_device.c \
	vk_format.c \
	vk_image.c \
	vk_instance.c \
	vk_log.c \
	vk_object.c \
	vk_physical_device.c \
	vk_queue.c \
	vk_render_pass.c \
	vk_shader_module.c \
	vk_synchronization2.c \
	vk_util.c

SRCS+=	vk_cmd_queue.c \
	vk_common_entrypoints.c \
	vk_dispatch_table.c \
	vk_enum_to_str.c \
	vk_extensions.c \
	vk_physical_device_features.c

.include "../Makefile.inc"

CFLAGS+=	${C_VIS_ARGS}
CPPFLAGS+=	-I${MESA_SRC}/src/vulkan/util \
		-I${MESA_SRC}/src/gallium/include \
		-I${MESA_SRC}/src/util \
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

.PATH: ${MESA_SRC}/src/vulkan/util
.PATH: ${MESA_SRC}/generated/src/vulkan/util
