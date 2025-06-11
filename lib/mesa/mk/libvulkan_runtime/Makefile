#	$OpenBSD: Makefile,v 1.6 2025/06/11 13:08:26 jsg Exp $

LIB=	vulkan_runtime

NOPROFILE=

SRCS=	vk_rmv_common.c \
	vk_rmv_exporter.c \
	vk_blend.c \
	vk_buffer.c \
	vk_buffer_view.c \
	vk_cmd_copy.c \
	vk_cmd_enqueue.c \
	vk_command_buffer.c \
	vk_command_pool.c \
	vk_debug_report.c \
	vk_debug_utils.c \
	vk_deferred_operation.c \
	vk_descriptor_set_layout.c \
	vk_descriptors.c \
	vk_descriptor_update_template.c \
	vk_device.c \
	vk_device_generated_commands.c \
	vk_device_memory.c \
	vk_fence.c \
	vk_framebuffer.c \
	vk_graphics_state.c \
	vk_image.c \
	vk_log.c \
	vk_meta_object_list.c \
	vk_object.c \
	vk_physical_device.c \
	vk_pipeline_layout.c \
	vk_query_pool.c \
	vk_queue.c \
	vk_render_pass.c \
	vk_sampler.c \
	vk_semaphore.c \
	vk_standard_sample_locations.c \
	vk_sync.c \
	vk_sync_binary.c \
	vk_sync_dummy.c \
	vk_sync_timeline.c \
	vk_synchronization.c \
	vk_video.c \
	vk_ycbcr_conversion.c

SRCS+=	vk_meta.c \
	vk_meta_blit_resolve.c \
	vk_meta_clear.c \
	vk_meta_copy_fill_update.c \
	vk_meta_draw_rects.c \
	vk_nir.c \
	vk_nir_convert_ycbcr.c \
	vk_pipeline.c \
	vk_pipeline_cache.c \
	vk_shader.c \
	vk_shader_module.c \
	vk_texcompress_etc2.c

SRCS+=	vk_drm_syncobj.c \
	vk_cmd_enqueue_entrypoints.c \
	vk_cmd_queue.c \
	vk_common_entrypoints.c \
	vk_dispatch_trampolines.c \
	vk_format_info.c \
	vk_physical_device_features.c \
	vk_physical_device_properties.c \
	vk_physical_device_spirv_caps.c \
	vk_synchronization_helpers.c

SRCS+=	vk_texcompress_astc.c \
	vk_acceleration_structure.c

# from src/vulkan/runtime/radix_sort/meson.build
SRCS+=	barrier.c \
	util.c \
	radix_sort_u64.c \
	radix_sort_vk.c

.include "../Makefile.inc"

CFLAGS+=	${C_VIS_ARGS}
CPPFLAGS+=	-I${MESA_SRC}/src/vulkan/runtime \
		-I${MESA_SRC}/src/vulkan/util \
		-I${MESA_SRC}/src/util \
		-I${MESA_SRC}/src/gallium/include \
		-I${MESA_SRC}/src/compiler \
		-I${MESA_SRC}/src/compiler/nir \
		-I${MESA_SRC}/generated/src \
		-I${MESA_SRC}/generated/src/compiler \
		-I${MESA_SRC}/generated/src/compiler/nir \
		-I${MESA_SRC}/generated/src/vulkan/runtime \
		-I${MESA_SRC}/generated/src/vulkan/util

install:

obj: _xenocara_obj

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

.PATH: ${MESA_SRC}/src/vulkan/runtime
.PATH: ${MESA_SRC}/src/vulkan/runtime/radix_sort
.PATH: ${MESA_SRC}/src/vulkan/runtime/radix_sort/common
.PATH: ${MESA_SRC}/src/vulkan/runtime/radix_sort/common/vk
.PATH: ${MESA_SRC}/src/vulkan/runtime/rmv
.PATH: ${MESA_SRC}/generated/src/vulkan/runtime
