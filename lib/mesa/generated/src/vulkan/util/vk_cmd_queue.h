
/* Copyright © 2015-2021 Intel Corporation
 * Copyright © 2021 Collabora, Ltd.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */
/* This file generated from vk_cmd_queue_gen.py, don't edit directly. */

#pragma once

#include "util/list.h"

#define VK_PROTOTYPES
#include <vulkan/vulkan.h>

#ifdef __cplusplus
extern "C" {
#endif

struct vk_cmd_queue {
   VkAllocationCallbacks *alloc;
   struct list_head cmds;
};

enum vk_cmd_type {
   VK_CMD_BIND_PIPELINE,
   VK_CMD_SET_VIEWPORT,
   VK_CMD_SET_SCISSOR,
   VK_CMD_SET_LINE_WIDTH,
   VK_CMD_SET_DEPTH_BIAS,
   VK_CMD_SET_BLEND_CONSTANTS,
   VK_CMD_SET_DEPTH_BOUNDS,
   VK_CMD_SET_STENCIL_COMPARE_MASK,
   VK_CMD_SET_STENCIL_WRITE_MASK,
   VK_CMD_SET_STENCIL_REFERENCE,
   VK_CMD_BIND_DESCRIPTOR_SETS,
   VK_CMD_BIND_INDEX_BUFFER,
   VK_CMD_BIND_VERTEX_BUFFERS,
   VK_CMD_DRAW,
   VK_CMD_DRAW_INDEXED,
   VK_CMD_DRAW_MULTI_EXT,
   VK_CMD_DRAW_MULTI_INDEXED_EXT,
   VK_CMD_DRAW_INDIRECT,
   VK_CMD_DRAW_INDEXED_INDIRECT,
   VK_CMD_DISPATCH,
   VK_CMD_DISPATCH_INDIRECT,
   VK_CMD_SUBPASS_SHADING_HUAWEI,
   VK_CMD_COPY_BUFFER,
   VK_CMD_COPY_IMAGE,
   VK_CMD_BLIT_IMAGE,
   VK_CMD_COPY_BUFFER_TO_IMAGE,
   VK_CMD_COPY_IMAGE_TO_BUFFER,
   VK_CMD_UPDATE_BUFFER,
   VK_CMD_FILL_BUFFER,
   VK_CMD_CLEAR_COLOR_IMAGE,
   VK_CMD_CLEAR_DEPTH_STENCIL_IMAGE,
   VK_CMD_CLEAR_ATTACHMENTS,
   VK_CMD_RESOLVE_IMAGE,
   VK_CMD_SET_EVENT,
   VK_CMD_RESET_EVENT,
   VK_CMD_WAIT_EVENTS,
   VK_CMD_PIPELINE_BARRIER,
   VK_CMD_BEGIN_QUERY,
   VK_CMD_END_QUERY,
   VK_CMD_BEGIN_CONDITIONAL_RENDERING_EXT,
   VK_CMD_END_CONDITIONAL_RENDERING_EXT,
   VK_CMD_RESET_QUERY_POOL,
   VK_CMD_WRITE_TIMESTAMP,
   VK_CMD_COPY_QUERY_POOL_RESULTS,
   VK_CMD_PUSH_CONSTANTS,
   VK_CMD_BEGIN_RENDER_PASS,
   VK_CMD_NEXT_SUBPASS,
   VK_CMD_END_RENDER_PASS,
   VK_CMD_EXECUTE_COMMANDS,
   VK_CMD_DEBUG_MARKER_BEGIN_EXT,
   VK_CMD_DEBUG_MARKER_END_EXT,
   VK_CMD_DEBUG_MARKER_INSERT_EXT,
   VK_CMD_EXECUTE_GENERATED_COMMANDS_NV,
   VK_CMD_PREPROCESS_GENERATED_COMMANDS_NV,
   VK_CMD_BIND_PIPELINE_SHADER_GROUP_NV,
   VK_CMD_PUSH_DESCRIPTOR_SET_KHR,
   VK_CMD_SET_DEVICE_MASK,
   VK_CMD_DISPATCH_BASE,
   VK_CMD_PUSH_DESCRIPTOR_SET_WITH_TEMPLATE_KHR,
   VK_CMD_SET_VIEWPORT_WSCALING_NV,
   VK_CMD_SET_DISCARD_RECTANGLE_EXT,
   VK_CMD_SET_SAMPLE_LOCATIONS_EXT,
   VK_CMD_BEGIN_DEBUG_UTILS_LABEL_EXT,
   VK_CMD_END_DEBUG_UTILS_LABEL_EXT,
   VK_CMD_INSERT_DEBUG_UTILS_LABEL_EXT,
   VK_CMD_WRITE_BUFFER_MARKER_AMD,
   VK_CMD_BEGIN_RENDER_PASS2,
   VK_CMD_NEXT_SUBPASS2,
   VK_CMD_END_RENDER_PASS2,
   VK_CMD_DRAW_INDIRECT_COUNT,
   VK_CMD_DRAW_INDEXED_INDIRECT_COUNT,
   VK_CMD_SET_CHECKPOINT_NV,
   VK_CMD_BIND_TRANSFORM_FEEDBACK_BUFFERS_EXT,
   VK_CMD_BEGIN_TRANSFORM_FEEDBACK_EXT,
   VK_CMD_END_TRANSFORM_FEEDBACK_EXT,
   VK_CMD_BEGIN_QUERY_INDEXED_EXT,
   VK_CMD_END_QUERY_INDEXED_EXT,
   VK_CMD_DRAW_INDIRECT_BYTE_COUNT_EXT,
   VK_CMD_SET_EXCLUSIVE_SCISSOR_NV,
   VK_CMD_BIND_SHADING_RATE_IMAGE_NV,
   VK_CMD_SET_VIEWPORT_SHADING_RATE_PALETTE_NV,
   VK_CMD_SET_COARSE_SAMPLE_ORDER_NV,
   VK_CMD_DRAW_MESH_TASKS_NV,
   VK_CMD_DRAW_MESH_TASKS_INDIRECT_NV,
   VK_CMD_DRAW_MESH_TASKS_INDIRECT_COUNT_NV,
   VK_CMD_BIND_INVOCATION_MASK_HUAWEI,
   VK_CMD_COPY_ACCELERATION_STRUCTURE_NV,
   VK_CMD_COPY_ACCELERATION_STRUCTURE_KHR,
   VK_CMD_COPY_ACCELERATION_STRUCTURE_TO_MEMORY_KHR,
   VK_CMD_COPY_MEMORY_TO_ACCELERATION_STRUCTURE_KHR,
   VK_CMD_WRITE_ACCELERATION_STRUCTURES_PROPERTIES_KHR,
   VK_CMD_WRITE_ACCELERATION_STRUCTURES_PROPERTIES_NV,
   VK_CMD_BUILD_ACCELERATION_STRUCTURE_NV,
   VK_CMD_TRACE_RAYS_KHR,
   VK_CMD_TRACE_RAYS_NV,
   VK_CMD_TRACE_RAYS_INDIRECT_KHR,
   VK_CMD_SET_RAY_TRACING_PIPELINE_STACK_SIZE_KHR,
   VK_CMD_SET_PERFORMANCE_MARKER_INTEL,
   VK_CMD_SET_PERFORMANCE_STREAM_MARKER_INTEL,
   VK_CMD_SET_PERFORMANCE_OVERRIDE_INTEL,
   VK_CMD_SET_LINE_STIPPLE_EXT,
   VK_CMD_BUILD_ACCELERATION_STRUCTURES_KHR,
   VK_CMD_BUILD_ACCELERATION_STRUCTURES_INDIRECT_KHR,
   VK_CMD_SET_CULL_MODE_EXT,
   VK_CMD_SET_FRONT_FACE_EXT,
   VK_CMD_SET_PRIMITIVE_TOPOLOGY_EXT,
   VK_CMD_SET_VIEWPORT_WITH_COUNT_EXT,
   VK_CMD_SET_SCISSOR_WITH_COUNT_EXT,
   VK_CMD_BIND_VERTEX_BUFFERS2_EXT,
   VK_CMD_SET_DEPTH_TEST_ENABLE_EXT,
   VK_CMD_SET_DEPTH_WRITE_ENABLE_EXT,
   VK_CMD_SET_DEPTH_COMPARE_OP_EXT,
   VK_CMD_SET_DEPTH_BOUNDS_TEST_ENABLE_EXT,
   VK_CMD_SET_STENCIL_TEST_ENABLE_EXT,
   VK_CMD_SET_STENCIL_OP_EXT,
   VK_CMD_SET_PATCH_CONTROL_POINTS_EXT,
   VK_CMD_SET_RASTERIZER_DISCARD_ENABLE_EXT,
   VK_CMD_SET_DEPTH_BIAS_ENABLE_EXT,
   VK_CMD_SET_LOGIC_OP_EXT,
   VK_CMD_SET_PRIMITIVE_RESTART_ENABLE_EXT,
   VK_CMD_COPY_BUFFER2_KHR,
   VK_CMD_COPY_IMAGE2_KHR,
   VK_CMD_BLIT_IMAGE2_KHR,
   VK_CMD_COPY_BUFFER_TO_IMAGE2_KHR,
   VK_CMD_COPY_IMAGE_TO_BUFFER2_KHR,
   VK_CMD_RESOLVE_IMAGE2_KHR,
   VK_CMD_SET_FRAGMENT_SHADING_RATE_KHR,
   VK_CMD_SET_FRAGMENT_SHADING_RATE_ENUM_NV,
   VK_CMD_SET_VERTEX_INPUT_EXT,
   VK_CMD_SET_COLOR_WRITE_ENABLE_EXT,
   VK_CMD_SET_EVENT2_KHR,
   VK_CMD_RESET_EVENT2_KHR,
   VK_CMD_WAIT_EVENTS2_KHR,
   VK_CMD_PIPELINE_BARRIER2_KHR,
   VK_CMD_WRITE_TIMESTAMP2_KHR,
   VK_CMD_WRITE_BUFFER_MARKER2_AMD,
#ifdef VK_ENABLE_BETA_EXTENSIONS
   VK_CMD_DECODE_VIDEO_KHR,
#endif // VK_ENABLE_BETA_EXTENSIONS
#ifdef VK_ENABLE_BETA_EXTENSIONS
   VK_CMD_BEGIN_VIDEO_CODING_KHR,
#endif // VK_ENABLE_BETA_EXTENSIONS
#ifdef VK_ENABLE_BETA_EXTENSIONS
   VK_CMD_CONTROL_VIDEO_CODING_KHR,
#endif // VK_ENABLE_BETA_EXTENSIONS
#ifdef VK_ENABLE_BETA_EXTENSIONS
   VK_CMD_END_VIDEO_CODING_KHR,
#endif // VK_ENABLE_BETA_EXTENSIONS
#ifdef VK_ENABLE_BETA_EXTENSIONS
   VK_CMD_ENCODE_VIDEO_KHR,
#endif // VK_ENABLE_BETA_EXTENSIONS
   VK_CMD_CU_LAUNCH_KERNEL_NVX,
};

extern const char *vk_cmd_queue_type_names[];

struct vk_cmd_bind_pipeline {
   VkPipelineBindPoint pipeline_bind_point;
   VkPipeline pipeline;
};
struct vk_cmd_set_viewport {
   uint32_t first_viewport;
   uint32_t viewport_count;
   VkViewport* viewports;
};
struct vk_cmd_set_scissor {
   uint32_t first_scissor;
   uint32_t scissor_count;
   VkRect2D* scissors;
};
struct vk_cmd_set_line_width {
   float line_width;
};
struct vk_cmd_set_depth_bias {
   float depth_bias_constant_factor;
   float depth_bias_clamp;
   float depth_bias_slope_factor;
};
struct vk_cmd_set_blend_constants {
   float blend_constants[4];
};
struct vk_cmd_set_depth_bounds {
   float min_depth_bounds;
   float max_depth_bounds;
};
struct vk_cmd_set_stencil_compare_mask {
   VkStencilFaceFlags face_mask;
   uint32_t compare_mask;
};
struct vk_cmd_set_stencil_write_mask {
   VkStencilFaceFlags face_mask;
   uint32_t write_mask;
};
struct vk_cmd_set_stencil_reference {
   VkStencilFaceFlags face_mask;
   uint32_t reference;
};
struct vk_cmd_bind_descriptor_sets {
   VkPipelineBindPoint pipeline_bind_point;
   VkPipelineLayout layout;
   uint32_t first_set;
   uint32_t descriptor_set_count;
   VkDescriptorSet* descriptor_sets;
   uint32_t dynamic_offset_count;
   uint32_t* dynamic_offsets;
};
struct vk_cmd_bind_index_buffer {
   VkBuffer buffer;
   VkDeviceSize offset;
   VkIndexType index_type;
};
struct vk_cmd_bind_vertex_buffers {
   uint32_t first_binding;
   uint32_t binding_count;
   VkBuffer* buffers;
   VkDeviceSize* offsets;
};
struct vk_cmd_draw {
   uint32_t vertex_count;
   uint32_t instance_count;
   uint32_t first_vertex;
   uint32_t first_instance;
};
struct vk_cmd_draw_indexed {
   uint32_t index_count;
   uint32_t instance_count;
   uint32_t first_index;
   int32_t vertex_offset;
   uint32_t first_instance;
};
struct vk_cmd_draw_multi_ext {
   uint32_t draw_count;
   VkMultiDrawInfoEXT* vertex_info;
   uint32_t instance_count;
   uint32_t first_instance;
   uint32_t stride;
};
struct vk_cmd_draw_multi_indexed_ext {
   uint32_t draw_count;
   VkMultiDrawIndexedInfoEXT* index_info;
   uint32_t instance_count;
   uint32_t first_instance;
   uint32_t stride;
   int32_t* vertex_offset;
};
struct vk_cmd_draw_indirect {
   VkBuffer buffer;
   VkDeviceSize offset;
   uint32_t draw_count;
   uint32_t stride;
};
struct vk_cmd_draw_indexed_indirect {
   VkBuffer buffer;
   VkDeviceSize offset;
   uint32_t draw_count;
   uint32_t stride;
};
struct vk_cmd_dispatch {
   uint32_t group_count_x;
   uint32_t group_count_y;
   uint32_t group_count_z;
};
struct vk_cmd_dispatch_indirect {
   VkBuffer buffer;
   VkDeviceSize offset;
};
struct vk_cmd_copy_buffer {
   VkBuffer src_buffer;
   VkBuffer dst_buffer;
   uint32_t region_count;
   VkBufferCopy* regions;
};
struct vk_cmd_copy_image {
   VkImage src_image;
   VkImageLayout src_image_layout;
   VkImage dst_image;
   VkImageLayout dst_image_layout;
   uint32_t region_count;
   VkImageCopy* regions;
};
struct vk_cmd_blit_image {
   VkImage src_image;
   VkImageLayout src_image_layout;
   VkImage dst_image;
   VkImageLayout dst_image_layout;
   uint32_t region_count;
   VkImageBlit* regions;
   VkFilter filter;
};
struct vk_cmd_copy_buffer_to_image {
   VkBuffer src_buffer;
   VkImage dst_image;
   VkImageLayout dst_image_layout;
   uint32_t region_count;
   VkBufferImageCopy* regions;
};
struct vk_cmd_copy_image_to_buffer {
   VkImage src_image;
   VkImageLayout src_image_layout;
   VkBuffer dst_buffer;
   uint32_t region_count;
   VkBufferImageCopy* regions;
};
struct vk_cmd_update_buffer {
   VkBuffer dst_buffer;
   VkDeviceSize dst_offset;
   VkDeviceSize data_size;
   void* data;
};
struct vk_cmd_fill_buffer {
   VkBuffer dst_buffer;
   VkDeviceSize dst_offset;
   VkDeviceSize size;
   uint32_t data;
};
struct vk_cmd_clear_color_image {
   VkImage image;
   VkImageLayout image_layout;
   VkClearColorValue* color;
   uint32_t range_count;
   VkImageSubresourceRange* ranges;
};
struct vk_cmd_clear_depth_stencil_image {
   VkImage image;
   VkImageLayout image_layout;
   VkClearDepthStencilValue* depth_stencil;
   uint32_t range_count;
   VkImageSubresourceRange* ranges;
};
struct vk_cmd_clear_attachments {
   uint32_t attachment_count;
   VkClearAttachment* attachments;
   uint32_t rect_count;
   VkClearRect* rects;
};
struct vk_cmd_resolve_image {
   VkImage src_image;
   VkImageLayout src_image_layout;
   VkImage dst_image;
   VkImageLayout dst_image_layout;
   uint32_t region_count;
   VkImageResolve* regions;
};
struct vk_cmd_set_event {
   VkEvent event;
   VkPipelineStageFlags stage_mask;
};
struct vk_cmd_reset_event {
   VkEvent event;
   VkPipelineStageFlags stage_mask;
};
struct vk_cmd_wait_events {
   uint32_t event_count;
   VkEvent* events;
   VkPipelineStageFlags src_stage_mask;
   VkPipelineStageFlags dst_stage_mask;
   uint32_t memory_barrier_count;
   VkMemoryBarrier* memory_barriers;
   uint32_t buffer_memory_barrier_count;
   VkBufferMemoryBarrier* buffer_memory_barriers;
   uint32_t image_memory_barrier_count;
   VkImageMemoryBarrier* image_memory_barriers;
};
struct vk_cmd_pipeline_barrier {
   VkPipelineStageFlags src_stage_mask;
   VkPipelineStageFlags dst_stage_mask;
   VkDependencyFlags dependency_flags;
   uint32_t memory_barrier_count;
   VkMemoryBarrier* memory_barriers;
   uint32_t buffer_memory_barrier_count;
   VkBufferMemoryBarrier* buffer_memory_barriers;
   uint32_t image_memory_barrier_count;
   VkImageMemoryBarrier* image_memory_barriers;
};
struct vk_cmd_begin_query {
   VkQueryPool query_pool;
   uint32_t query;
   VkQueryControlFlags flags;
};
struct vk_cmd_end_query {
   VkQueryPool query_pool;
   uint32_t query;
};
struct vk_cmd_begin_conditional_rendering_ext {
   VkConditionalRenderingBeginInfoEXT* conditional_rendering_begin;
};
struct vk_cmd_reset_query_pool {
   VkQueryPool query_pool;
   uint32_t first_query;
   uint32_t query_count;
};
struct vk_cmd_write_timestamp {
   VkPipelineStageFlagBits pipeline_stage;
   VkQueryPool query_pool;
   uint32_t query;
};
struct vk_cmd_copy_query_pool_results {
   VkQueryPool query_pool;
   uint32_t first_query;
   uint32_t query_count;
   VkBuffer dst_buffer;
   VkDeviceSize dst_offset;
   VkDeviceSize stride;
   VkQueryResultFlags flags;
};
struct vk_cmd_push_constants {
   VkPipelineLayout layout;
   VkShaderStageFlags stage_flags;
   uint32_t offset;
   uint32_t size;
   void* values;
};
struct vk_cmd_begin_render_pass {
   VkRenderPassBeginInfo* render_pass_begin;
   VkSubpassContents contents;
};
struct vk_cmd_next_subpass {
   VkSubpassContents contents;
};
struct vk_cmd_execute_commands {
   uint32_t command_buffer_count;
   VkCommandBuffer* command_buffers;
};
struct vk_cmd_debug_marker_begin_ext {
   VkDebugMarkerMarkerInfoEXT* marker_info;
};
struct vk_cmd_debug_marker_insert_ext {
   VkDebugMarkerMarkerInfoEXT* marker_info;
};
struct vk_cmd_execute_generated_commands_nv {
   VkBool32 is_preprocessed;
   VkGeneratedCommandsInfoNV* generated_commands_info;
};
struct vk_cmd_preprocess_generated_commands_nv {
   VkGeneratedCommandsInfoNV* generated_commands_info;
};
struct vk_cmd_bind_pipeline_shader_group_nv {
   VkPipelineBindPoint pipeline_bind_point;
   VkPipeline pipeline;
   uint32_t group_index;
};
struct vk_cmd_push_descriptor_set_khr {
   VkPipelineBindPoint pipeline_bind_point;
   VkPipelineLayout layout;
   uint32_t set;
   uint32_t descriptor_write_count;
   VkWriteDescriptorSet* descriptor_writes;
};
struct vk_cmd_set_device_mask {
   uint32_t device_mask;
};
struct vk_cmd_dispatch_base {
   uint32_t base_group_x;
   uint32_t base_group_y;
   uint32_t base_group_z;
   uint32_t group_count_x;
   uint32_t group_count_y;
   uint32_t group_count_z;
};
struct vk_cmd_push_descriptor_set_with_template_khr {
   VkDescriptorUpdateTemplate descriptor_update_template;
   VkPipelineLayout layout;
   uint32_t set;
   void* data;
};
struct vk_cmd_set_viewport_wscaling_nv {
   uint32_t first_viewport;
   uint32_t viewport_count;
   VkViewportWScalingNV* viewport_wscalings;
};
struct vk_cmd_set_discard_rectangle_ext {
   uint32_t first_discard_rectangle;
   uint32_t discard_rectangle_count;
   VkRect2D* discard_rectangles;
};
struct vk_cmd_set_sample_locations_ext {
   VkSampleLocationsInfoEXT* sample_locations_info;
};
struct vk_cmd_begin_debug_utils_label_ext {
   VkDebugUtilsLabelEXT* label_info;
};
struct vk_cmd_insert_debug_utils_label_ext {
   VkDebugUtilsLabelEXT* label_info;
};
struct vk_cmd_write_buffer_marker_amd {
   VkPipelineStageFlagBits pipeline_stage;
   VkBuffer dst_buffer;
   VkDeviceSize dst_offset;
   uint32_t marker;
};
struct vk_cmd_begin_render_pass2 {
   VkRenderPassBeginInfo*      render_pass_begin;
   VkSubpassBeginInfo*      subpass_begin_info;
};
struct vk_cmd_next_subpass2 {
   VkSubpassBeginInfo*      subpass_begin_info;
   VkSubpassEndInfo*        subpass_end_info;
};
struct vk_cmd_end_render_pass2 {
   VkSubpassEndInfo*        subpass_end_info;
};
struct vk_cmd_draw_indirect_count {
   VkBuffer buffer;
   VkDeviceSize offset;
   VkBuffer count_buffer;
   VkDeviceSize count_buffer_offset;
   uint32_t max_draw_count;
   uint32_t stride;
};
struct vk_cmd_draw_indexed_indirect_count {
   VkBuffer buffer;
   VkDeviceSize offset;
   VkBuffer count_buffer;
   VkDeviceSize count_buffer_offset;
   uint32_t max_draw_count;
   uint32_t stride;
};
struct vk_cmd_set_checkpoint_nv {
   void* checkpoint_marker;
};
struct vk_cmd_bind_transform_feedback_buffers_ext {
   uint32_t first_binding;
   uint32_t binding_count;
   VkBuffer* buffers;
   VkDeviceSize* offsets;
   VkDeviceSize* sizes;
};
struct vk_cmd_begin_transform_feedback_ext {
   uint32_t first_counter_buffer;
   uint32_t counter_buffer_count;
   VkBuffer* counter_buffers;
   VkDeviceSize* counter_buffer_offsets;
};
struct vk_cmd_end_transform_feedback_ext {
   uint32_t first_counter_buffer;
   uint32_t counter_buffer_count;
   VkBuffer* counter_buffers;
   VkDeviceSize* counter_buffer_offsets;
};
struct vk_cmd_begin_query_indexed_ext {
   VkQueryPool query_pool;
   uint32_t query;
   VkQueryControlFlags flags;
   uint32_t index;
};
struct vk_cmd_end_query_indexed_ext {
   VkQueryPool query_pool;
   uint32_t query;
   uint32_t index;
};
struct vk_cmd_draw_indirect_byte_count_ext {
   uint32_t instance_count;
   uint32_t first_instance;
   VkBuffer counter_buffer;
   VkDeviceSize counter_buffer_offset;
   uint32_t counter_offset;
   uint32_t vertex_stride;
};
struct vk_cmd_set_exclusive_scissor_nv {
   uint32_t first_exclusive_scissor;
   uint32_t exclusive_scissor_count;
   VkRect2D* exclusive_scissors;
};
struct vk_cmd_bind_shading_rate_image_nv {
   VkImageView image_view;
   VkImageLayout image_layout;
};
struct vk_cmd_set_viewport_shading_rate_palette_nv {
   uint32_t first_viewport;
   uint32_t viewport_count;
   VkShadingRatePaletteNV* shading_rate_palettes;
};
struct vk_cmd_set_coarse_sample_order_nv {
   VkCoarseSampleOrderTypeNV sample_order_type;
   uint32_t custom_sample_order_count;
   VkCoarseSampleOrderCustomNV* custom_sample_orders;
};
struct vk_cmd_draw_mesh_tasks_nv {
   uint32_t task_count;
   uint32_t first_task;
};
struct vk_cmd_draw_mesh_tasks_indirect_nv {
   VkBuffer buffer;
   VkDeviceSize offset;
   uint32_t draw_count;
   uint32_t stride;
};
struct vk_cmd_draw_mesh_tasks_indirect_count_nv {
   VkBuffer buffer;
   VkDeviceSize offset;
   VkBuffer count_buffer;
   VkDeviceSize count_buffer_offset;
   uint32_t max_draw_count;
   uint32_t stride;
};
struct vk_cmd_bind_invocation_mask_huawei {
   VkImageView image_view;
   VkImageLayout image_layout;
};
struct vk_cmd_copy_acceleration_structure_nv {
   VkAccelerationStructureNV dst;
   VkAccelerationStructureNV src;
   VkCopyAccelerationStructureModeKHR mode;
};
struct vk_cmd_copy_acceleration_structure_khr {
   VkCopyAccelerationStructureInfoKHR* info;
};
struct vk_cmd_copy_acceleration_structure_to_memory_khr {
   VkCopyAccelerationStructureToMemoryInfoKHR* info;
};
struct vk_cmd_copy_memory_to_acceleration_structure_khr {
   VkCopyMemoryToAccelerationStructureInfoKHR* info;
};
struct vk_cmd_write_acceleration_structures_properties_khr {
   uint32_t acceleration_structure_count;
   VkAccelerationStructureKHR* acceleration_structures;
   VkQueryType query_type;
   VkQueryPool query_pool;
   uint32_t first_query;
};
struct vk_cmd_write_acceleration_structures_properties_nv {
   uint32_t acceleration_structure_count;
   VkAccelerationStructureNV* acceleration_structures;
   VkQueryType query_type;
   VkQueryPool query_pool;
   uint32_t first_query;
};
struct vk_cmd_build_acceleration_structure_nv {
   VkAccelerationStructureInfoNV* info;
   VkBuffer instance_data;
   VkDeviceSize instance_offset;
   VkBool32 update;
   VkAccelerationStructureNV dst;
   VkAccelerationStructureNV src;
   VkBuffer scratch;
   VkDeviceSize scratch_offset;
};
struct vk_cmd_trace_rays_khr {
   VkStridedDeviceAddressRegionKHR* raygen_shader_binding_table;
   VkStridedDeviceAddressRegionKHR* miss_shader_binding_table;
   VkStridedDeviceAddressRegionKHR* hit_shader_binding_table;
   VkStridedDeviceAddressRegionKHR* callable_shader_binding_table;
   uint32_t width;
   uint32_t height;
   uint32_t depth;
};
struct vk_cmd_trace_rays_nv {
   VkBuffer raygen_shader_binding_table_buffer;
   VkDeviceSize raygen_shader_binding_offset;
   VkBuffer miss_shader_binding_table_buffer;
   VkDeviceSize miss_shader_binding_offset;
   VkDeviceSize miss_shader_binding_stride;
   VkBuffer hit_shader_binding_table_buffer;
   VkDeviceSize hit_shader_binding_offset;
   VkDeviceSize hit_shader_binding_stride;
   VkBuffer callable_shader_binding_table_buffer;
   VkDeviceSize callable_shader_binding_offset;
   VkDeviceSize callable_shader_binding_stride;
   uint32_t width;
   uint32_t height;
   uint32_t depth;
};
struct vk_cmd_trace_rays_indirect_khr {
   VkStridedDeviceAddressRegionKHR* raygen_shader_binding_table;
   VkStridedDeviceAddressRegionKHR* miss_shader_binding_table;
   VkStridedDeviceAddressRegionKHR* hit_shader_binding_table;
   VkStridedDeviceAddressRegionKHR* callable_shader_binding_table;
   VkDeviceAddress indirect_device_address;
};
struct vk_cmd_set_ray_tracing_pipeline_stack_size_khr {
   uint32_t pipeline_stack_size;
};
struct vk_cmd_set_performance_marker_intel {
   VkPerformanceMarkerInfoINTEL* marker_info;
};
struct vk_cmd_set_performance_stream_marker_intel {
   VkPerformanceStreamMarkerInfoINTEL* marker_info;
};
struct vk_cmd_set_performance_override_intel {
   VkPerformanceOverrideInfoINTEL* override_info;
};
struct vk_cmd_set_line_stipple_ext {
   uint32_t line_stipple_factor;
   uint16_t line_stipple_pattern;
};
struct vk_cmd_build_acceleration_structures_khr {
   uint32_t info_count;
   VkAccelerationStructureBuildGeometryInfoKHR* infos;
   VkAccelerationStructureBuildRangeInfoKHR* const* pp_build_range_infos;
};
struct vk_cmd_build_acceleration_structures_indirect_khr {
   uint32_t                                           info_count;
   VkAccelerationStructureBuildGeometryInfoKHR* infos;
   VkDeviceAddress*             indirect_device_addresses;
   uint32_t*                    indirect_strides;
   uint32_t* const*             pp_max_primitive_counts;
};
struct vk_cmd_set_cull_mode_ext {
   VkCullModeFlags cull_mode;
};
struct vk_cmd_set_front_face_ext {
   VkFrontFace front_face;
};
struct vk_cmd_set_primitive_topology_ext {
   VkPrimitiveTopology primitive_topology;
};
struct vk_cmd_set_viewport_with_count_ext {
   uint32_t viewport_count;
   VkViewport* viewports;
};
struct vk_cmd_set_scissor_with_count_ext {
   uint32_t scissor_count;
   VkRect2D* scissors;
};
struct vk_cmd_bind_vertex_buffers2_ext {
   uint32_t first_binding;
   uint32_t binding_count;
   VkBuffer* buffers;
   VkDeviceSize* offsets;
   VkDeviceSize* sizes;
   VkDeviceSize* strides;
};
struct vk_cmd_set_depth_test_enable_ext {
   VkBool32 depth_test_enable;
};
struct vk_cmd_set_depth_write_enable_ext {
   VkBool32 depth_write_enable;
};
struct vk_cmd_set_depth_compare_op_ext {
   VkCompareOp depth_compare_op;
};
struct vk_cmd_set_depth_bounds_test_enable_ext {
   VkBool32 depth_bounds_test_enable;
};
struct vk_cmd_set_stencil_test_enable_ext {
   VkBool32 stencil_test_enable;
};
struct vk_cmd_set_stencil_op_ext {
   VkStencilFaceFlags face_mask;
   VkStencilOp fail_op;
   VkStencilOp pass_op;
   VkStencilOp depth_fail_op;
   VkCompareOp compare_op;
};
struct vk_cmd_set_patch_control_points_ext {
   uint32_t patch_control_points;
};
struct vk_cmd_set_rasterizer_discard_enable_ext {
   VkBool32 rasterizer_discard_enable;
};
struct vk_cmd_set_depth_bias_enable_ext {
   VkBool32 depth_bias_enable;
};
struct vk_cmd_set_logic_op_ext {
   VkLogicOp logic_op;
};
struct vk_cmd_set_primitive_restart_enable_ext {
   VkBool32 primitive_restart_enable;
};
struct vk_cmd_copy_buffer2_khr {
   VkCopyBufferInfo2KHR* copy_buffer_info;
};
struct vk_cmd_copy_image2_khr {
   VkCopyImageInfo2KHR* copy_image_info;
};
struct vk_cmd_blit_image2_khr {
   VkBlitImageInfo2KHR* blit_image_info;
};
struct vk_cmd_copy_buffer_to_image2_khr {
   VkCopyBufferToImageInfo2KHR* copy_buffer_to_image_info;
};
struct vk_cmd_copy_image_to_buffer2_khr {
   VkCopyImageToBufferInfo2KHR* copy_image_to_buffer_info;
};
struct vk_cmd_resolve_image2_khr {
   VkResolveImageInfo2KHR* resolve_image_info;
};
struct vk_cmd_set_fragment_shading_rate_khr {
   VkExtent2D*                           fragment_size;
   VkFragmentShadingRateCombinerOpKHR    combiner_ops[2];
};
struct vk_cmd_set_fragment_shading_rate_enum_nv {
   VkFragmentShadingRateNV                     shading_rate;
   VkFragmentShadingRateCombinerOpKHR    combiner_ops[2];
};
struct vk_cmd_set_vertex_input_ext {
   uint32_t vertex_binding_description_count;
   VkVertexInputBindingDescription2EXT* vertex_binding_descriptions;
   uint32_t vertex_attribute_description_count;
   VkVertexInputAttributeDescription2EXT* vertex_attribute_descriptions;
};
struct vk_cmd_set_color_write_enable_ext {
   uint32_t                                attachment_count;
   VkBool32*   color_write_enables;
};
struct vk_cmd_set_event2_khr {
   VkEvent                                             event;
   VkDependencyInfoKHR*                          dependency_info;
};
struct vk_cmd_reset_event2_khr {
   VkEvent                                             event;
   VkPipelineStageFlags2KHR                            stage_mask;
};
struct vk_cmd_wait_events2_khr {
   uint32_t                                            event_count;
   VkEvent*                     events;
   VkDependencyInfoKHR*         dependency_infos;
};
struct vk_cmd_pipeline_barrier2_khr {
   VkDependencyInfoKHR*                                dependency_info;
};
struct vk_cmd_write_timestamp2_khr {
   VkPipelineStageFlags2KHR                            stage;
   VkQueryPool                                         query_pool;
   uint32_t                                            query;
};
struct vk_cmd_write_buffer_marker2_amd {
   VkPipelineStageFlags2KHR                            stage;
   VkBuffer                                            dst_buffer;
   VkDeviceSize                                        dst_offset;
   uint32_t                                            marker;
};
#ifdef VK_ENABLE_BETA_EXTENSIONS
struct vk_cmd_decode_video_khr {
   VkVideoDecodeInfoKHR* frame_info;
};
#endif // VK_ENABLE_BETA_EXTENSIONS
#ifdef VK_ENABLE_BETA_EXTENSIONS
struct vk_cmd_begin_video_coding_khr {
   VkVideoBeginCodingInfoKHR* begin_info;
};
#endif // VK_ENABLE_BETA_EXTENSIONS
#ifdef VK_ENABLE_BETA_EXTENSIONS
struct vk_cmd_control_video_coding_khr {
   VkVideoCodingControlInfoKHR* coding_control_info;
};
#endif // VK_ENABLE_BETA_EXTENSIONS
#ifdef VK_ENABLE_BETA_EXTENSIONS
struct vk_cmd_end_video_coding_khr {
   VkVideoEndCodingInfoKHR* end_coding_info;
};
#endif // VK_ENABLE_BETA_EXTENSIONS
#ifdef VK_ENABLE_BETA_EXTENSIONS
struct vk_cmd_encode_video_khr {
   VkVideoEncodeInfoKHR* encode_info;
};
#endif // VK_ENABLE_BETA_EXTENSIONS
struct vk_cmd_cu_launch_kernel_nvx {
   VkCuLaunchInfoNVX* launch_info;
};

struct vk_cmd_queue_entry {
   struct list_head cmd_link;
   enum vk_cmd_type type;
   union {
      struct vk_cmd_bind_pipeline bind_pipeline;
      struct vk_cmd_set_viewport set_viewport;
      struct vk_cmd_set_scissor set_scissor;
      struct vk_cmd_set_line_width set_line_width;
      struct vk_cmd_set_depth_bias set_depth_bias;
      struct vk_cmd_set_blend_constants set_blend_constants;
      struct vk_cmd_set_depth_bounds set_depth_bounds;
      struct vk_cmd_set_stencil_compare_mask set_stencil_compare_mask;
      struct vk_cmd_set_stencil_write_mask set_stencil_write_mask;
      struct vk_cmd_set_stencil_reference set_stencil_reference;
      struct vk_cmd_bind_descriptor_sets bind_descriptor_sets;
      struct vk_cmd_bind_index_buffer bind_index_buffer;
      struct vk_cmd_bind_vertex_buffers bind_vertex_buffers;
      struct vk_cmd_draw draw;
      struct vk_cmd_draw_indexed draw_indexed;
      struct vk_cmd_draw_multi_ext draw_multi_ext;
      struct vk_cmd_draw_multi_indexed_ext draw_multi_indexed_ext;
      struct vk_cmd_draw_indirect draw_indirect;
      struct vk_cmd_draw_indexed_indirect draw_indexed_indirect;
      struct vk_cmd_dispatch dispatch;
      struct vk_cmd_dispatch_indirect dispatch_indirect;
      struct vk_cmd_copy_buffer copy_buffer;
      struct vk_cmd_copy_image copy_image;
      struct vk_cmd_blit_image blit_image;
      struct vk_cmd_copy_buffer_to_image copy_buffer_to_image;
      struct vk_cmd_copy_image_to_buffer copy_image_to_buffer;
      struct vk_cmd_update_buffer update_buffer;
      struct vk_cmd_fill_buffer fill_buffer;
      struct vk_cmd_clear_color_image clear_color_image;
      struct vk_cmd_clear_depth_stencil_image clear_depth_stencil_image;
      struct vk_cmd_clear_attachments clear_attachments;
      struct vk_cmd_resolve_image resolve_image;
      struct vk_cmd_set_event set_event;
      struct vk_cmd_reset_event reset_event;
      struct vk_cmd_wait_events wait_events;
      struct vk_cmd_pipeline_barrier pipeline_barrier;
      struct vk_cmd_begin_query begin_query;
      struct vk_cmd_end_query end_query;
      struct vk_cmd_begin_conditional_rendering_ext begin_conditional_rendering_ext;
      struct vk_cmd_reset_query_pool reset_query_pool;
      struct vk_cmd_write_timestamp write_timestamp;
      struct vk_cmd_copy_query_pool_results copy_query_pool_results;
      struct vk_cmd_push_constants push_constants;
      struct vk_cmd_begin_render_pass begin_render_pass;
      struct vk_cmd_next_subpass next_subpass;
      struct vk_cmd_execute_commands execute_commands;
      struct vk_cmd_debug_marker_begin_ext debug_marker_begin_ext;
      struct vk_cmd_debug_marker_insert_ext debug_marker_insert_ext;
      struct vk_cmd_execute_generated_commands_nv execute_generated_commands_nv;
      struct vk_cmd_preprocess_generated_commands_nv preprocess_generated_commands_nv;
      struct vk_cmd_bind_pipeline_shader_group_nv bind_pipeline_shader_group_nv;
      struct vk_cmd_push_descriptor_set_khr push_descriptor_set_khr;
      struct vk_cmd_set_device_mask set_device_mask;
      struct vk_cmd_dispatch_base dispatch_base;
      struct vk_cmd_push_descriptor_set_with_template_khr push_descriptor_set_with_template_khr;
      struct vk_cmd_set_viewport_wscaling_nv set_viewport_wscaling_nv;
      struct vk_cmd_set_discard_rectangle_ext set_discard_rectangle_ext;
      struct vk_cmd_set_sample_locations_ext set_sample_locations_ext;
      struct vk_cmd_begin_debug_utils_label_ext begin_debug_utils_label_ext;
      struct vk_cmd_insert_debug_utils_label_ext insert_debug_utils_label_ext;
      struct vk_cmd_write_buffer_marker_amd write_buffer_marker_amd;
      struct vk_cmd_begin_render_pass2 begin_render_pass2;
      struct vk_cmd_next_subpass2 next_subpass2;
      struct vk_cmd_end_render_pass2 end_render_pass2;
      struct vk_cmd_draw_indirect_count draw_indirect_count;
      struct vk_cmd_draw_indexed_indirect_count draw_indexed_indirect_count;
      struct vk_cmd_set_checkpoint_nv set_checkpoint_nv;
      struct vk_cmd_bind_transform_feedback_buffers_ext bind_transform_feedback_buffers_ext;
      struct vk_cmd_begin_transform_feedback_ext begin_transform_feedback_ext;
      struct vk_cmd_end_transform_feedback_ext end_transform_feedback_ext;
      struct vk_cmd_begin_query_indexed_ext begin_query_indexed_ext;
      struct vk_cmd_end_query_indexed_ext end_query_indexed_ext;
      struct vk_cmd_draw_indirect_byte_count_ext draw_indirect_byte_count_ext;
      struct vk_cmd_set_exclusive_scissor_nv set_exclusive_scissor_nv;
      struct vk_cmd_bind_shading_rate_image_nv bind_shading_rate_image_nv;
      struct vk_cmd_set_viewport_shading_rate_palette_nv set_viewport_shading_rate_palette_nv;
      struct vk_cmd_set_coarse_sample_order_nv set_coarse_sample_order_nv;
      struct vk_cmd_draw_mesh_tasks_nv draw_mesh_tasks_nv;
      struct vk_cmd_draw_mesh_tasks_indirect_nv draw_mesh_tasks_indirect_nv;
      struct vk_cmd_draw_mesh_tasks_indirect_count_nv draw_mesh_tasks_indirect_count_nv;
      struct vk_cmd_bind_invocation_mask_huawei bind_invocation_mask_huawei;
      struct vk_cmd_copy_acceleration_structure_nv copy_acceleration_structure_nv;
      struct vk_cmd_copy_acceleration_structure_khr copy_acceleration_structure_khr;
      struct vk_cmd_copy_acceleration_structure_to_memory_khr copy_acceleration_structure_to_memory_khr;
      struct vk_cmd_copy_memory_to_acceleration_structure_khr copy_memory_to_acceleration_structure_khr;
      struct vk_cmd_write_acceleration_structures_properties_khr write_acceleration_structures_properties_khr;
      struct vk_cmd_write_acceleration_structures_properties_nv write_acceleration_structures_properties_nv;
      struct vk_cmd_build_acceleration_structure_nv build_acceleration_structure_nv;
      struct vk_cmd_trace_rays_khr trace_rays_khr;
      struct vk_cmd_trace_rays_nv trace_rays_nv;
      struct vk_cmd_trace_rays_indirect_khr trace_rays_indirect_khr;
      struct vk_cmd_set_ray_tracing_pipeline_stack_size_khr set_ray_tracing_pipeline_stack_size_khr;
      struct vk_cmd_set_performance_marker_intel set_performance_marker_intel;
      struct vk_cmd_set_performance_stream_marker_intel set_performance_stream_marker_intel;
      struct vk_cmd_set_performance_override_intel set_performance_override_intel;
      struct vk_cmd_set_line_stipple_ext set_line_stipple_ext;
      struct vk_cmd_build_acceleration_structures_khr build_acceleration_structures_khr;
      struct vk_cmd_build_acceleration_structures_indirect_khr build_acceleration_structures_indirect_khr;
      struct vk_cmd_set_cull_mode_ext set_cull_mode_ext;
      struct vk_cmd_set_front_face_ext set_front_face_ext;
      struct vk_cmd_set_primitive_topology_ext set_primitive_topology_ext;
      struct vk_cmd_set_viewport_with_count_ext set_viewport_with_count_ext;
      struct vk_cmd_set_scissor_with_count_ext set_scissor_with_count_ext;
      struct vk_cmd_bind_vertex_buffers2_ext bind_vertex_buffers2_ext;
      struct vk_cmd_set_depth_test_enable_ext set_depth_test_enable_ext;
      struct vk_cmd_set_depth_write_enable_ext set_depth_write_enable_ext;
      struct vk_cmd_set_depth_compare_op_ext set_depth_compare_op_ext;
      struct vk_cmd_set_depth_bounds_test_enable_ext set_depth_bounds_test_enable_ext;
      struct vk_cmd_set_stencil_test_enable_ext set_stencil_test_enable_ext;
      struct vk_cmd_set_stencil_op_ext set_stencil_op_ext;
      struct vk_cmd_set_patch_control_points_ext set_patch_control_points_ext;
      struct vk_cmd_set_rasterizer_discard_enable_ext set_rasterizer_discard_enable_ext;
      struct vk_cmd_set_depth_bias_enable_ext set_depth_bias_enable_ext;
      struct vk_cmd_set_logic_op_ext set_logic_op_ext;
      struct vk_cmd_set_primitive_restart_enable_ext set_primitive_restart_enable_ext;
      struct vk_cmd_copy_buffer2_khr copy_buffer2_khr;
      struct vk_cmd_copy_image2_khr copy_image2_khr;
      struct vk_cmd_blit_image2_khr blit_image2_khr;
      struct vk_cmd_copy_buffer_to_image2_khr copy_buffer_to_image2_khr;
      struct vk_cmd_copy_image_to_buffer2_khr copy_image_to_buffer2_khr;
      struct vk_cmd_resolve_image2_khr resolve_image2_khr;
      struct vk_cmd_set_fragment_shading_rate_khr set_fragment_shading_rate_khr;
      struct vk_cmd_set_fragment_shading_rate_enum_nv set_fragment_shading_rate_enum_nv;
      struct vk_cmd_set_vertex_input_ext set_vertex_input_ext;
      struct vk_cmd_set_color_write_enable_ext set_color_write_enable_ext;
      struct vk_cmd_set_event2_khr set_event2_khr;
      struct vk_cmd_reset_event2_khr reset_event2_khr;
      struct vk_cmd_wait_events2_khr wait_events2_khr;
      struct vk_cmd_pipeline_barrier2_khr pipeline_barrier2_khr;
      struct vk_cmd_write_timestamp2_khr write_timestamp2_khr;
      struct vk_cmd_write_buffer_marker2_amd write_buffer_marker2_amd;
#ifdef VK_ENABLE_BETA_EXTENSIONS
      struct vk_cmd_decode_video_khr decode_video_khr;
#endif // VK_ENABLE_BETA_EXTENSIONS
#ifdef VK_ENABLE_BETA_EXTENSIONS
      struct vk_cmd_begin_video_coding_khr begin_video_coding_khr;
#endif // VK_ENABLE_BETA_EXTENSIONS
#ifdef VK_ENABLE_BETA_EXTENSIONS
      struct vk_cmd_control_video_coding_khr control_video_coding_khr;
#endif // VK_ENABLE_BETA_EXTENSIONS
#ifdef VK_ENABLE_BETA_EXTENSIONS
      struct vk_cmd_end_video_coding_khr end_video_coding_khr;
#endif // VK_ENABLE_BETA_EXTENSIONS
#ifdef VK_ENABLE_BETA_EXTENSIONS
      struct vk_cmd_encode_video_khr encode_video_khr;
#endif // VK_ENABLE_BETA_EXTENSIONS
      struct vk_cmd_cu_launch_kernel_nvx cu_launch_kernel_nvx;
   } u;
   void *driver_data;
};

  void vk_enqueue_cmd_bind_pipeline(struct vk_cmd_queue *queue
   , VkPipelineBindPoint pipelineBindPoint
   , VkPipeline pipeline
  );

  void vk_enqueue_cmd_set_viewport(struct vk_cmd_queue *queue
   , uint32_t firstViewport
   , uint32_t viewportCount
   , const VkViewport* pViewports
  );

  void vk_enqueue_cmd_set_scissor(struct vk_cmd_queue *queue
   , uint32_t firstScissor
   , uint32_t scissorCount
   , const VkRect2D* pScissors
  );

  void vk_enqueue_cmd_set_line_width(struct vk_cmd_queue *queue
   , float lineWidth
  );

  void vk_enqueue_cmd_set_depth_bias(struct vk_cmd_queue *queue
   , float depthBiasConstantFactor
   , float depthBiasClamp
   , float depthBiasSlopeFactor
  );

  void vk_enqueue_cmd_set_blend_constants(struct vk_cmd_queue *queue
   , const float blendConstants[4]
  );

  void vk_enqueue_cmd_set_depth_bounds(struct vk_cmd_queue *queue
   , float minDepthBounds
   , float maxDepthBounds
  );

  void vk_enqueue_cmd_set_stencil_compare_mask(struct vk_cmd_queue *queue
   , VkStencilFaceFlags faceMask
   , uint32_t compareMask
  );

  void vk_enqueue_cmd_set_stencil_write_mask(struct vk_cmd_queue *queue
   , VkStencilFaceFlags faceMask
   , uint32_t writeMask
  );

  void vk_enqueue_cmd_set_stencil_reference(struct vk_cmd_queue *queue
   , VkStencilFaceFlags faceMask
   , uint32_t reference
  );

  void vk_enqueue_cmd_bind_index_buffer(struct vk_cmd_queue *queue
   , VkBuffer buffer
   , VkDeviceSize offset
   , VkIndexType indexType
  );

  void vk_enqueue_cmd_bind_vertex_buffers(struct vk_cmd_queue *queue
   , uint32_t firstBinding
   , uint32_t bindingCount
   , const VkBuffer* pBuffers
   , const VkDeviceSize* pOffsets
  );

  void vk_enqueue_cmd_draw(struct vk_cmd_queue *queue
   , uint32_t vertexCount
   , uint32_t instanceCount
   , uint32_t firstVertex
   , uint32_t firstInstance
  );

  void vk_enqueue_cmd_draw_indexed(struct vk_cmd_queue *queue
   , uint32_t indexCount
   , uint32_t instanceCount
   , uint32_t firstIndex
   , int32_t vertexOffset
   , uint32_t firstInstance
  );

  void vk_enqueue_cmd_draw_indirect(struct vk_cmd_queue *queue
   , VkBuffer buffer
   , VkDeviceSize offset
   , uint32_t drawCount
   , uint32_t stride
  );

  void vk_enqueue_cmd_draw_indexed_indirect(struct vk_cmd_queue *queue
   , VkBuffer buffer
   , VkDeviceSize offset
   , uint32_t drawCount
   , uint32_t stride
  );

  void vk_enqueue_cmd_dispatch(struct vk_cmd_queue *queue
   , uint32_t groupCountX
   , uint32_t groupCountY
   , uint32_t groupCountZ
  );

  void vk_enqueue_cmd_dispatch_indirect(struct vk_cmd_queue *queue
   , VkBuffer buffer
   , VkDeviceSize offset
  );

  void vk_enqueue_cmd_subpass_shading_huawei(struct vk_cmd_queue *queue
  );

  void vk_enqueue_cmd_copy_buffer(struct vk_cmd_queue *queue
   , VkBuffer srcBuffer
   , VkBuffer dstBuffer
   , uint32_t regionCount
   , const VkBufferCopy* pRegions
  );

  void vk_enqueue_cmd_copy_image(struct vk_cmd_queue *queue
   , VkImage srcImage
   , VkImageLayout srcImageLayout
   , VkImage dstImage
   , VkImageLayout dstImageLayout
   , uint32_t regionCount
   , const VkImageCopy* pRegions
  );

  void vk_enqueue_cmd_blit_image(struct vk_cmd_queue *queue
   , VkImage srcImage
   , VkImageLayout srcImageLayout
   , VkImage dstImage
   , VkImageLayout dstImageLayout
   , uint32_t regionCount
   , const VkImageBlit* pRegions
   , VkFilter filter
  );

  void vk_enqueue_cmd_copy_buffer_to_image(struct vk_cmd_queue *queue
   , VkBuffer srcBuffer
   , VkImage dstImage
   , VkImageLayout dstImageLayout
   , uint32_t regionCount
   , const VkBufferImageCopy* pRegions
  );

  void vk_enqueue_cmd_copy_image_to_buffer(struct vk_cmd_queue *queue
   , VkImage srcImage
   , VkImageLayout srcImageLayout
   , VkBuffer dstBuffer
   , uint32_t regionCount
   , const VkBufferImageCopy* pRegions
  );

  void vk_enqueue_cmd_update_buffer(struct vk_cmd_queue *queue
   , VkBuffer dstBuffer
   , VkDeviceSize dstOffset
   , VkDeviceSize dataSize
   , const void* pData
  );

  void vk_enqueue_cmd_fill_buffer(struct vk_cmd_queue *queue
   , VkBuffer dstBuffer
   , VkDeviceSize dstOffset
   , VkDeviceSize size
   , uint32_t data
  );

  void vk_enqueue_cmd_clear_color_image(struct vk_cmd_queue *queue
   , VkImage image
   , VkImageLayout imageLayout
   , const VkClearColorValue* pColor
   , uint32_t rangeCount
   , const VkImageSubresourceRange* pRanges
  );

  void vk_enqueue_cmd_clear_depth_stencil_image(struct vk_cmd_queue *queue
   , VkImage image
   , VkImageLayout imageLayout
   , const VkClearDepthStencilValue* pDepthStencil
   , uint32_t rangeCount
   , const VkImageSubresourceRange* pRanges
  );

  void vk_enqueue_cmd_clear_attachments(struct vk_cmd_queue *queue
   , uint32_t attachmentCount
   , const VkClearAttachment* pAttachments
   , uint32_t rectCount
   , const VkClearRect* pRects
  );

  void vk_enqueue_cmd_resolve_image(struct vk_cmd_queue *queue
   , VkImage srcImage
   , VkImageLayout srcImageLayout
   , VkImage dstImage
   , VkImageLayout dstImageLayout
   , uint32_t regionCount
   , const VkImageResolve* pRegions
  );

  void vk_enqueue_cmd_set_event(struct vk_cmd_queue *queue
   , VkEvent event
   , VkPipelineStageFlags stageMask
  );

  void vk_enqueue_cmd_reset_event(struct vk_cmd_queue *queue
   , VkEvent event
   , VkPipelineStageFlags stageMask
  );

  void vk_enqueue_cmd_wait_events(struct vk_cmd_queue *queue
   , uint32_t eventCount
   , const VkEvent* pEvents
   , VkPipelineStageFlags srcStageMask
   , VkPipelineStageFlags dstStageMask
   , uint32_t memoryBarrierCount
   , const VkMemoryBarrier* pMemoryBarriers
   , uint32_t bufferMemoryBarrierCount
   , const VkBufferMemoryBarrier* pBufferMemoryBarriers
   , uint32_t imageMemoryBarrierCount
   , const VkImageMemoryBarrier* pImageMemoryBarriers
  );

  void vk_enqueue_cmd_pipeline_barrier(struct vk_cmd_queue *queue
   , VkPipelineStageFlags srcStageMask
   , VkPipelineStageFlags dstStageMask
   , VkDependencyFlags dependencyFlags
   , uint32_t memoryBarrierCount
   , const VkMemoryBarrier* pMemoryBarriers
   , uint32_t bufferMemoryBarrierCount
   , const VkBufferMemoryBarrier* pBufferMemoryBarriers
   , uint32_t imageMemoryBarrierCount
   , const VkImageMemoryBarrier* pImageMemoryBarriers
  );

  void vk_enqueue_cmd_begin_query(struct vk_cmd_queue *queue
   , VkQueryPool queryPool
   , uint32_t query
   , VkQueryControlFlags flags
  );

  void vk_enqueue_cmd_end_query(struct vk_cmd_queue *queue
   , VkQueryPool queryPool
   , uint32_t query
  );

  void vk_enqueue_cmd_begin_conditional_rendering_ext(struct vk_cmd_queue *queue
   , const VkConditionalRenderingBeginInfoEXT* pConditionalRenderingBegin
  );

  void vk_enqueue_cmd_end_conditional_rendering_ext(struct vk_cmd_queue *queue
  );

  void vk_enqueue_cmd_reset_query_pool(struct vk_cmd_queue *queue
   , VkQueryPool queryPool
   , uint32_t firstQuery
   , uint32_t queryCount
  );

  void vk_enqueue_cmd_write_timestamp(struct vk_cmd_queue *queue
   , VkPipelineStageFlagBits pipelineStage
   , VkQueryPool queryPool
   , uint32_t query
  );

  void vk_enqueue_cmd_copy_query_pool_results(struct vk_cmd_queue *queue
   , VkQueryPool queryPool
   , uint32_t firstQuery
   , uint32_t queryCount
   , VkBuffer dstBuffer
   , VkDeviceSize dstOffset
   , VkDeviceSize stride
   , VkQueryResultFlags flags
  );

  void vk_enqueue_cmd_push_constants(struct vk_cmd_queue *queue
   , VkPipelineLayout layout
   , VkShaderStageFlags stageFlags
   , uint32_t offset
   , uint32_t size
   , const void* pValues
  );

  void vk_enqueue_cmd_begin_render_pass(struct vk_cmd_queue *queue
   , const VkRenderPassBeginInfo* pRenderPassBegin
   , VkSubpassContents contents
  );

  void vk_enqueue_cmd_next_subpass(struct vk_cmd_queue *queue
   , VkSubpassContents contents
  );

  void vk_enqueue_cmd_end_render_pass(struct vk_cmd_queue *queue
  );

  void vk_enqueue_cmd_execute_commands(struct vk_cmd_queue *queue
   , uint32_t commandBufferCount
   , const VkCommandBuffer* pCommandBuffers
  );

  void vk_enqueue_cmd_debug_marker_begin_ext(struct vk_cmd_queue *queue
   , const VkDebugMarkerMarkerInfoEXT* pMarkerInfo
  );

  void vk_enqueue_cmd_debug_marker_end_ext(struct vk_cmd_queue *queue
  );

  void vk_enqueue_cmd_debug_marker_insert_ext(struct vk_cmd_queue *queue
   , const VkDebugMarkerMarkerInfoEXT* pMarkerInfo
  );

  void vk_enqueue_cmd_execute_generated_commands_nv(struct vk_cmd_queue *queue
   , VkBool32 isPreprocessed
   , const VkGeneratedCommandsInfoNV* pGeneratedCommandsInfo
  );

  void vk_enqueue_cmd_preprocess_generated_commands_nv(struct vk_cmd_queue *queue
   , const VkGeneratedCommandsInfoNV* pGeneratedCommandsInfo
  );

  void vk_enqueue_cmd_bind_pipeline_shader_group_nv(struct vk_cmd_queue *queue
   , VkPipelineBindPoint pipelineBindPoint
   , VkPipeline pipeline
   , uint32_t groupIndex
  );

  void vk_enqueue_cmd_set_device_mask(struct vk_cmd_queue *queue
   , uint32_t deviceMask
  );

  void vk_enqueue_cmd_dispatch_base(struct vk_cmd_queue *queue
   , uint32_t baseGroupX
   , uint32_t baseGroupY
   , uint32_t baseGroupZ
   , uint32_t groupCountX
   , uint32_t groupCountY
   , uint32_t groupCountZ
  );

  void vk_enqueue_cmd_set_viewport_wscaling_nv(struct vk_cmd_queue *queue
   , uint32_t firstViewport
   , uint32_t viewportCount
   , const VkViewportWScalingNV* pViewportWScalings
  );

  void vk_enqueue_cmd_set_discard_rectangle_ext(struct vk_cmd_queue *queue
   , uint32_t firstDiscardRectangle
   , uint32_t discardRectangleCount
   , const VkRect2D* pDiscardRectangles
  );

  void vk_enqueue_cmd_set_sample_locations_ext(struct vk_cmd_queue *queue
   , const VkSampleLocationsInfoEXT* pSampleLocationsInfo
  );

  void vk_enqueue_cmd_begin_debug_utils_label_ext(struct vk_cmd_queue *queue
   , const VkDebugUtilsLabelEXT* pLabelInfo
  );

  void vk_enqueue_cmd_end_debug_utils_label_ext(struct vk_cmd_queue *queue
  );

  void vk_enqueue_cmd_insert_debug_utils_label_ext(struct vk_cmd_queue *queue
   , const VkDebugUtilsLabelEXT* pLabelInfo
  );

  void vk_enqueue_cmd_write_buffer_marker_amd(struct vk_cmd_queue *queue
   , VkPipelineStageFlagBits pipelineStage
   , VkBuffer dstBuffer
   , VkDeviceSize dstOffset
   , uint32_t marker
  );

  void vk_enqueue_cmd_begin_render_pass2(struct vk_cmd_queue *queue
   , const VkRenderPassBeginInfo*      pRenderPassBegin
   , const VkSubpassBeginInfo*      pSubpassBeginInfo
  );

  void vk_enqueue_cmd_next_subpass2(struct vk_cmd_queue *queue
   , const VkSubpassBeginInfo*      pSubpassBeginInfo
   , const VkSubpassEndInfo*        pSubpassEndInfo
  );

  void vk_enqueue_cmd_end_render_pass2(struct vk_cmd_queue *queue
   , const VkSubpassEndInfo*        pSubpassEndInfo
  );

  void vk_enqueue_cmd_draw_indirect_count(struct vk_cmd_queue *queue
   , VkBuffer buffer
   , VkDeviceSize offset
   , VkBuffer countBuffer
   , VkDeviceSize countBufferOffset
   , uint32_t maxDrawCount
   , uint32_t stride
  );

  void vk_enqueue_cmd_draw_indexed_indirect_count(struct vk_cmd_queue *queue
   , VkBuffer buffer
   , VkDeviceSize offset
   , VkBuffer countBuffer
   , VkDeviceSize countBufferOffset
   , uint32_t maxDrawCount
   , uint32_t stride
  );

  void vk_enqueue_cmd_set_checkpoint_nv(struct vk_cmd_queue *queue
   , const void* pCheckpointMarker
  );

  void vk_enqueue_cmd_bind_transform_feedback_buffers_ext(struct vk_cmd_queue *queue
   , uint32_t firstBinding
   , uint32_t bindingCount
   , const VkBuffer* pBuffers
   , const VkDeviceSize* pOffsets
   , const VkDeviceSize* pSizes
  );

  void vk_enqueue_cmd_begin_transform_feedback_ext(struct vk_cmd_queue *queue
   , uint32_t firstCounterBuffer
   , uint32_t counterBufferCount
   , const VkBuffer* pCounterBuffers
   , const VkDeviceSize* pCounterBufferOffsets
  );

  void vk_enqueue_cmd_end_transform_feedback_ext(struct vk_cmd_queue *queue
   , uint32_t firstCounterBuffer
   , uint32_t counterBufferCount
   , const VkBuffer* pCounterBuffers
   , const VkDeviceSize* pCounterBufferOffsets
  );

  void vk_enqueue_cmd_begin_query_indexed_ext(struct vk_cmd_queue *queue
   , VkQueryPool queryPool
   , uint32_t query
   , VkQueryControlFlags flags
   , uint32_t index
  );

  void vk_enqueue_cmd_end_query_indexed_ext(struct vk_cmd_queue *queue
   , VkQueryPool queryPool
   , uint32_t query
   , uint32_t index
  );

  void vk_enqueue_cmd_draw_indirect_byte_count_ext(struct vk_cmd_queue *queue
   , uint32_t instanceCount
   , uint32_t firstInstance
   , VkBuffer counterBuffer
   , VkDeviceSize counterBufferOffset
   , uint32_t counterOffset
   , uint32_t vertexStride
  );

  void vk_enqueue_cmd_set_exclusive_scissor_nv(struct vk_cmd_queue *queue
   , uint32_t firstExclusiveScissor
   , uint32_t exclusiveScissorCount
   , const VkRect2D* pExclusiveScissors
  );

  void vk_enqueue_cmd_bind_shading_rate_image_nv(struct vk_cmd_queue *queue
   , VkImageView imageView
   , VkImageLayout imageLayout
  );

  void vk_enqueue_cmd_set_viewport_shading_rate_palette_nv(struct vk_cmd_queue *queue
   , uint32_t firstViewport
   , uint32_t viewportCount
   , const VkShadingRatePaletteNV* pShadingRatePalettes
  );

  void vk_enqueue_cmd_set_coarse_sample_order_nv(struct vk_cmd_queue *queue
   , VkCoarseSampleOrderTypeNV sampleOrderType
   , uint32_t customSampleOrderCount
   , const VkCoarseSampleOrderCustomNV* pCustomSampleOrders
  );

  void vk_enqueue_cmd_draw_mesh_tasks_nv(struct vk_cmd_queue *queue
   , uint32_t taskCount
   , uint32_t firstTask
  );

  void vk_enqueue_cmd_draw_mesh_tasks_indirect_nv(struct vk_cmd_queue *queue
   , VkBuffer buffer
   , VkDeviceSize offset
   , uint32_t drawCount
   , uint32_t stride
  );

  void vk_enqueue_cmd_draw_mesh_tasks_indirect_count_nv(struct vk_cmd_queue *queue
   , VkBuffer buffer
   , VkDeviceSize offset
   , VkBuffer countBuffer
   , VkDeviceSize countBufferOffset
   , uint32_t maxDrawCount
   , uint32_t stride
  );

  void vk_enqueue_cmd_bind_invocation_mask_huawei(struct vk_cmd_queue *queue
   , VkImageView imageView
   , VkImageLayout imageLayout
  );

  void vk_enqueue_cmd_copy_acceleration_structure_nv(struct vk_cmd_queue *queue
   , VkAccelerationStructureNV dst
   , VkAccelerationStructureNV src
   , VkCopyAccelerationStructureModeKHR mode
  );

  void vk_enqueue_cmd_copy_acceleration_structure_khr(struct vk_cmd_queue *queue
   , const VkCopyAccelerationStructureInfoKHR* pInfo
  );

  void vk_enqueue_cmd_copy_acceleration_structure_to_memory_khr(struct vk_cmd_queue *queue
   , const VkCopyAccelerationStructureToMemoryInfoKHR* pInfo
  );

  void vk_enqueue_cmd_copy_memory_to_acceleration_structure_khr(struct vk_cmd_queue *queue
   , const VkCopyMemoryToAccelerationStructureInfoKHR* pInfo
  );

  void vk_enqueue_cmd_write_acceleration_structures_properties_khr(struct vk_cmd_queue *queue
   , uint32_t accelerationStructureCount
   , const VkAccelerationStructureKHR* pAccelerationStructures
   , VkQueryType queryType
   , VkQueryPool queryPool
   , uint32_t firstQuery
  );

  void vk_enqueue_cmd_write_acceleration_structures_properties_nv(struct vk_cmd_queue *queue
   , uint32_t accelerationStructureCount
   , const VkAccelerationStructureNV* pAccelerationStructures
   , VkQueryType queryType
   , VkQueryPool queryPool
   , uint32_t firstQuery
  );

  void vk_enqueue_cmd_build_acceleration_structure_nv(struct vk_cmd_queue *queue
   , const VkAccelerationStructureInfoNV* pInfo
   , VkBuffer instanceData
   , VkDeviceSize instanceOffset
   , VkBool32 update
   , VkAccelerationStructureNV dst
   , VkAccelerationStructureNV src
   , VkBuffer scratch
   , VkDeviceSize scratchOffset
  );

  void vk_enqueue_cmd_trace_rays_khr(struct vk_cmd_queue *queue
   , const VkStridedDeviceAddressRegionKHR* pRaygenShaderBindingTable
   , const VkStridedDeviceAddressRegionKHR* pMissShaderBindingTable
   , const VkStridedDeviceAddressRegionKHR* pHitShaderBindingTable
   , const VkStridedDeviceAddressRegionKHR* pCallableShaderBindingTable
   , uint32_t width
   , uint32_t height
   , uint32_t depth
  );

  void vk_enqueue_cmd_trace_rays_nv(struct vk_cmd_queue *queue
   , VkBuffer raygenShaderBindingTableBuffer
   , VkDeviceSize raygenShaderBindingOffset
   , VkBuffer missShaderBindingTableBuffer
   , VkDeviceSize missShaderBindingOffset
   , VkDeviceSize missShaderBindingStride
   , VkBuffer hitShaderBindingTableBuffer
   , VkDeviceSize hitShaderBindingOffset
   , VkDeviceSize hitShaderBindingStride
   , VkBuffer callableShaderBindingTableBuffer
   , VkDeviceSize callableShaderBindingOffset
   , VkDeviceSize callableShaderBindingStride
   , uint32_t width
   , uint32_t height
   , uint32_t depth
  );

  void vk_enqueue_cmd_trace_rays_indirect_khr(struct vk_cmd_queue *queue
   , const VkStridedDeviceAddressRegionKHR* pRaygenShaderBindingTable
   , const VkStridedDeviceAddressRegionKHR* pMissShaderBindingTable
   , const VkStridedDeviceAddressRegionKHR* pHitShaderBindingTable
   , const VkStridedDeviceAddressRegionKHR* pCallableShaderBindingTable
   , VkDeviceAddress indirectDeviceAddress
  );

  void vk_enqueue_cmd_set_ray_tracing_pipeline_stack_size_khr(struct vk_cmd_queue *queue
   , uint32_t pipelineStackSize
  );

  void vk_enqueue_cmd_set_performance_marker_intel(struct vk_cmd_queue *queue
   , const VkPerformanceMarkerInfoINTEL* pMarkerInfo
  );

  void vk_enqueue_cmd_set_performance_stream_marker_intel(struct vk_cmd_queue *queue
   , const VkPerformanceStreamMarkerInfoINTEL* pMarkerInfo
  );

  void vk_enqueue_cmd_set_performance_override_intel(struct vk_cmd_queue *queue
   , const VkPerformanceOverrideInfoINTEL* pOverrideInfo
  );

  void vk_enqueue_cmd_set_line_stipple_ext(struct vk_cmd_queue *queue
   , uint32_t lineStippleFactor
   , uint16_t lineStipplePattern
  );

  void vk_enqueue_cmd_build_acceleration_structures_khr(struct vk_cmd_queue *queue
   , uint32_t infoCount
   , const VkAccelerationStructureBuildGeometryInfoKHR* pInfos
   , const VkAccelerationStructureBuildRangeInfoKHR* const* ppBuildRangeInfos
  );

  void vk_enqueue_cmd_build_acceleration_structures_indirect_khr(struct vk_cmd_queue *queue
   , uint32_t                                           infoCount
   , const VkAccelerationStructureBuildGeometryInfoKHR* pInfos
   , const VkDeviceAddress*             pIndirectDeviceAddresses
   , const uint32_t*                    pIndirectStrides
   , const uint32_t* const*             ppMaxPrimitiveCounts
  );

  void vk_enqueue_cmd_set_cull_mode_ext(struct vk_cmd_queue *queue
   , VkCullModeFlags cullMode
  );

  void vk_enqueue_cmd_set_front_face_ext(struct vk_cmd_queue *queue
   , VkFrontFace frontFace
  );

  void vk_enqueue_cmd_set_primitive_topology_ext(struct vk_cmd_queue *queue
   , VkPrimitiveTopology primitiveTopology
  );

  void vk_enqueue_cmd_set_viewport_with_count_ext(struct vk_cmd_queue *queue
   , uint32_t viewportCount
   , const VkViewport* pViewports
  );

  void vk_enqueue_cmd_set_scissor_with_count_ext(struct vk_cmd_queue *queue
   , uint32_t scissorCount
   , const VkRect2D* pScissors
  );

  void vk_enqueue_cmd_bind_vertex_buffers2_ext(struct vk_cmd_queue *queue
   , uint32_t firstBinding
   , uint32_t bindingCount
   , const VkBuffer* pBuffers
   , const VkDeviceSize* pOffsets
   , const VkDeviceSize* pSizes
   , const VkDeviceSize* pStrides
  );

  void vk_enqueue_cmd_set_depth_test_enable_ext(struct vk_cmd_queue *queue
   , VkBool32 depthTestEnable
  );

  void vk_enqueue_cmd_set_depth_write_enable_ext(struct vk_cmd_queue *queue
   , VkBool32 depthWriteEnable
  );

  void vk_enqueue_cmd_set_depth_compare_op_ext(struct vk_cmd_queue *queue
   , VkCompareOp depthCompareOp
  );

  void vk_enqueue_cmd_set_depth_bounds_test_enable_ext(struct vk_cmd_queue *queue
   , VkBool32 depthBoundsTestEnable
  );

  void vk_enqueue_cmd_set_stencil_test_enable_ext(struct vk_cmd_queue *queue
   , VkBool32 stencilTestEnable
  );

  void vk_enqueue_cmd_set_stencil_op_ext(struct vk_cmd_queue *queue
   , VkStencilFaceFlags faceMask
   , VkStencilOp failOp
   , VkStencilOp passOp
   , VkStencilOp depthFailOp
   , VkCompareOp compareOp
  );

  void vk_enqueue_cmd_set_patch_control_points_ext(struct vk_cmd_queue *queue
   , uint32_t patchControlPoints
  );

  void vk_enqueue_cmd_set_rasterizer_discard_enable_ext(struct vk_cmd_queue *queue
   , VkBool32 rasterizerDiscardEnable
  );

  void vk_enqueue_cmd_set_depth_bias_enable_ext(struct vk_cmd_queue *queue
   , VkBool32 depthBiasEnable
  );

  void vk_enqueue_cmd_set_logic_op_ext(struct vk_cmd_queue *queue
   , VkLogicOp logicOp
  );

  void vk_enqueue_cmd_set_primitive_restart_enable_ext(struct vk_cmd_queue *queue
   , VkBool32 primitiveRestartEnable
  );

  void vk_enqueue_cmd_copy_buffer2_khr(struct vk_cmd_queue *queue
   , const VkCopyBufferInfo2KHR* pCopyBufferInfo
  );

  void vk_enqueue_cmd_copy_image2_khr(struct vk_cmd_queue *queue
   , const VkCopyImageInfo2KHR* pCopyImageInfo
  );

  void vk_enqueue_cmd_blit_image2_khr(struct vk_cmd_queue *queue
   , const VkBlitImageInfo2KHR* pBlitImageInfo
  );

  void vk_enqueue_cmd_copy_buffer_to_image2_khr(struct vk_cmd_queue *queue
   , const VkCopyBufferToImageInfo2KHR* pCopyBufferToImageInfo
  );

  void vk_enqueue_cmd_copy_image_to_buffer2_khr(struct vk_cmd_queue *queue
   , const VkCopyImageToBufferInfo2KHR* pCopyImageToBufferInfo
  );

  void vk_enqueue_cmd_resolve_image2_khr(struct vk_cmd_queue *queue
   , const VkResolveImageInfo2KHR* pResolveImageInfo
  );

  void vk_enqueue_cmd_set_fragment_shading_rate_khr(struct vk_cmd_queue *queue
   , const VkExtent2D*                           pFragmentSize
   , const VkFragmentShadingRateCombinerOpKHR    combinerOps[2]
  );

  void vk_enqueue_cmd_set_fragment_shading_rate_enum_nv(struct vk_cmd_queue *queue
   , VkFragmentShadingRateNV                     shadingRate
   , const VkFragmentShadingRateCombinerOpKHR    combinerOps[2]
  );

  void vk_enqueue_cmd_set_vertex_input_ext(struct vk_cmd_queue *queue
   , uint32_t vertexBindingDescriptionCount
   , const VkVertexInputBindingDescription2EXT* pVertexBindingDescriptions
   , uint32_t vertexAttributeDescriptionCount
   , const VkVertexInputAttributeDescription2EXT* pVertexAttributeDescriptions
  );

  void vk_enqueue_cmd_set_color_write_enable_ext(struct vk_cmd_queue *queue
   , uint32_t                                attachmentCount
   , const VkBool32*   pColorWriteEnables
  );

  void vk_enqueue_cmd_set_event2_khr(struct vk_cmd_queue *queue
   , VkEvent                                             event
   , const VkDependencyInfoKHR*                          pDependencyInfo
  );

  void vk_enqueue_cmd_reset_event2_khr(struct vk_cmd_queue *queue
   , VkEvent                                             event
   , VkPipelineStageFlags2KHR                            stageMask
  );

  void vk_enqueue_cmd_wait_events2_khr(struct vk_cmd_queue *queue
   , uint32_t                                            eventCount
   , const VkEvent*                     pEvents
   , const VkDependencyInfoKHR*         pDependencyInfos
  );

  void vk_enqueue_cmd_pipeline_barrier2_khr(struct vk_cmd_queue *queue
   , const VkDependencyInfoKHR*                                pDependencyInfo
  );

  void vk_enqueue_cmd_write_timestamp2_khr(struct vk_cmd_queue *queue
   , VkPipelineStageFlags2KHR                            stage
   , VkQueryPool                                         queryPool
   , uint32_t                                            query
  );

  void vk_enqueue_cmd_write_buffer_marker2_amd(struct vk_cmd_queue *queue
   , VkPipelineStageFlags2KHR                            stage
   , VkBuffer                                            dstBuffer
   , VkDeviceSize                                        dstOffset
   , uint32_t                                            marker
  );

#ifdef VK_ENABLE_BETA_EXTENSIONS
  void vk_enqueue_cmd_decode_video_khr(struct vk_cmd_queue *queue
   , const VkVideoDecodeInfoKHR* pFrameInfo
  );
#endif // VK_ENABLE_BETA_EXTENSIONS

#ifdef VK_ENABLE_BETA_EXTENSIONS
  void vk_enqueue_cmd_begin_video_coding_khr(struct vk_cmd_queue *queue
   , const VkVideoBeginCodingInfoKHR* pBeginInfo
  );
#endif // VK_ENABLE_BETA_EXTENSIONS

#ifdef VK_ENABLE_BETA_EXTENSIONS
  void vk_enqueue_cmd_control_video_coding_khr(struct vk_cmd_queue *queue
   , const VkVideoCodingControlInfoKHR* pCodingControlInfo
  );
#endif // VK_ENABLE_BETA_EXTENSIONS

#ifdef VK_ENABLE_BETA_EXTENSIONS
  void vk_enqueue_cmd_end_video_coding_khr(struct vk_cmd_queue *queue
   , const VkVideoEndCodingInfoKHR* pEndCodingInfo
  );
#endif // VK_ENABLE_BETA_EXTENSIONS

#ifdef VK_ENABLE_BETA_EXTENSIONS
  void vk_enqueue_cmd_encode_video_khr(struct vk_cmd_queue *queue
   , const VkVideoEncodeInfoKHR* pEncodeInfo
  );
#endif // VK_ENABLE_BETA_EXTENSIONS

  void vk_enqueue_cmd_cu_launch_kernel_nvx(struct vk_cmd_queue *queue
   , const VkCuLaunchInfoNVX* pLaunchInfo
  );


void vk_free_queue(struct vk_cmd_queue *queue);

#ifdef __cplusplus
}
#endif
