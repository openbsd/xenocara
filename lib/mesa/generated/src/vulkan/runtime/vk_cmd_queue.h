
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

struct vk_device_dispatch_table;

struct vk_cmd_queue {
   const VkAllocationCallbacks *alloc;
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
   VK_CMD_DRAW_MESH_TASKS_EXT,
   VK_CMD_DRAW_MESH_TASKS_INDIRECT_EXT,
   VK_CMD_DRAW_MESH_TASKS_INDIRECT_COUNT_EXT,
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
   VK_CMD_TRACE_RAYS_INDIRECT2_KHR,
   VK_CMD_SET_RAY_TRACING_PIPELINE_STACK_SIZE_KHR,
   VK_CMD_SET_PERFORMANCE_MARKER_INTEL,
   VK_CMD_SET_PERFORMANCE_STREAM_MARKER_INTEL,
   VK_CMD_SET_PERFORMANCE_OVERRIDE_INTEL,
   VK_CMD_SET_LINE_STIPPLE_EXT,
   VK_CMD_BUILD_ACCELERATION_STRUCTURES_KHR,
   VK_CMD_BUILD_ACCELERATION_STRUCTURES_INDIRECT_KHR,
   VK_CMD_SET_CULL_MODE,
   VK_CMD_SET_FRONT_FACE,
   VK_CMD_SET_PRIMITIVE_TOPOLOGY,
   VK_CMD_SET_VIEWPORT_WITH_COUNT,
   VK_CMD_SET_SCISSOR_WITH_COUNT,
   VK_CMD_BIND_VERTEX_BUFFERS2,
   VK_CMD_SET_DEPTH_TEST_ENABLE,
   VK_CMD_SET_DEPTH_WRITE_ENABLE,
   VK_CMD_SET_DEPTH_COMPARE_OP,
   VK_CMD_SET_DEPTH_BOUNDS_TEST_ENABLE,
   VK_CMD_SET_STENCIL_TEST_ENABLE,
   VK_CMD_SET_STENCIL_OP,
   VK_CMD_SET_PATCH_CONTROL_POINTS_EXT,
   VK_CMD_SET_RASTERIZER_DISCARD_ENABLE,
   VK_CMD_SET_DEPTH_BIAS_ENABLE,
   VK_CMD_SET_LOGIC_OP_EXT,
   VK_CMD_SET_PRIMITIVE_RESTART_ENABLE,
   VK_CMD_SET_TESSELLATION_DOMAIN_ORIGIN_EXT,
   VK_CMD_SET_DEPTH_CLAMP_ENABLE_EXT,
   VK_CMD_SET_POLYGON_MODE_EXT,
   VK_CMD_SET_RASTERIZATION_SAMPLES_EXT,
   VK_CMD_SET_SAMPLE_MASK_EXT,
   VK_CMD_SET_ALPHA_TO_COVERAGE_ENABLE_EXT,
   VK_CMD_SET_ALPHA_TO_ONE_ENABLE_EXT,
   VK_CMD_SET_LOGIC_OP_ENABLE_EXT,
   VK_CMD_SET_COLOR_BLEND_ENABLE_EXT,
   VK_CMD_SET_COLOR_BLEND_EQUATION_EXT,
   VK_CMD_SET_COLOR_WRITE_MASK_EXT,
   VK_CMD_SET_RASTERIZATION_STREAM_EXT,
   VK_CMD_SET_CONSERVATIVE_RASTERIZATION_MODE_EXT,
   VK_CMD_SET_EXTRA_PRIMITIVE_OVERESTIMATION_SIZE_EXT,
   VK_CMD_SET_DEPTH_CLIP_ENABLE_EXT,
   VK_CMD_SET_SAMPLE_LOCATIONS_ENABLE_EXT,
   VK_CMD_SET_COLOR_BLEND_ADVANCED_EXT,
   VK_CMD_SET_PROVOKING_VERTEX_MODE_EXT,
   VK_CMD_SET_LINE_RASTERIZATION_MODE_EXT,
   VK_CMD_SET_LINE_STIPPLE_ENABLE_EXT,
   VK_CMD_SET_DEPTH_CLIP_NEGATIVE_ONE_TO_ONE_EXT,
   VK_CMD_SET_VIEWPORT_WSCALING_ENABLE_NV,
   VK_CMD_SET_VIEWPORT_SWIZZLE_NV,
   VK_CMD_SET_COVERAGE_TO_COLOR_ENABLE_NV,
   VK_CMD_SET_COVERAGE_TO_COLOR_LOCATION_NV,
   VK_CMD_SET_COVERAGE_MODULATION_MODE_NV,
   VK_CMD_SET_COVERAGE_MODULATION_TABLE_ENABLE_NV,
   VK_CMD_SET_COVERAGE_MODULATION_TABLE_NV,
   VK_CMD_SET_SHADING_RATE_IMAGE_ENABLE_NV,
   VK_CMD_SET_COVERAGE_REDUCTION_MODE_NV,
   VK_CMD_SET_REPRESENTATIVE_FRAGMENT_TEST_ENABLE_NV,
   VK_CMD_COPY_BUFFER2,
   VK_CMD_COPY_IMAGE2,
   VK_CMD_BLIT_IMAGE2,
   VK_CMD_COPY_BUFFER_TO_IMAGE2,
   VK_CMD_COPY_IMAGE_TO_BUFFER2,
   VK_CMD_RESOLVE_IMAGE2,
   VK_CMD_SET_FRAGMENT_SHADING_RATE_KHR,
   VK_CMD_SET_FRAGMENT_SHADING_RATE_ENUM_NV,
   VK_CMD_SET_VERTEX_INPUT_EXT,
   VK_CMD_SET_COLOR_WRITE_ENABLE_EXT,
   VK_CMD_SET_EVENT2,
   VK_CMD_RESET_EVENT2,
   VK_CMD_WAIT_EVENTS2,
   VK_CMD_PIPELINE_BARRIER2,
   VK_CMD_WRITE_TIMESTAMP2,
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
   VK_CMD_BEGIN_RENDERING,
   VK_CMD_END_RENDERING,
   VK_CMD_BUILD_MICROMAPS_EXT,
   VK_CMD_COPY_MICROMAP_EXT,
   VK_CMD_COPY_MICROMAP_TO_MEMORY_EXT,
   VK_CMD_COPY_MEMORY_TO_MICROMAP_EXT,
   VK_CMD_WRITE_MICROMAPS_PROPERTIES_EXT,
   VK_CMD_OPTICAL_FLOW_EXECUTE_NV,
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
struct vk_cmd_draw_mesh_tasks_ext {
   uint32_t group_count_x;
   uint32_t group_count_y;
   uint32_t group_count_z;
};
struct vk_cmd_draw_mesh_tasks_indirect_ext {
   VkBuffer buffer;
   VkDeviceSize offset;
   uint32_t draw_count;
   uint32_t stride;
};
struct vk_cmd_draw_mesh_tasks_indirect_count_ext {
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
struct vk_cmd_trace_rays_indirect2_khr {
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
   const VkAccelerationStructureBuildRangeInfoKHR* * pp_build_range_infos;
};
struct vk_cmd_build_acceleration_structures_indirect_khr {
   uint32_t                                           info_count;
   VkAccelerationStructureBuildGeometryInfoKHR* infos;
   VkDeviceAddress*             indirect_device_addresses;
   uint32_t*                    indirect_strides;
   const uint32_t* *             pp_max_primitive_counts;
};
struct vk_cmd_set_cull_mode {
   VkCullModeFlags cull_mode;
};
struct vk_cmd_set_front_face {
   VkFrontFace front_face;
};
struct vk_cmd_set_primitive_topology {
   VkPrimitiveTopology primitive_topology;
};
struct vk_cmd_set_viewport_with_count {
   uint32_t viewport_count;
   VkViewport* viewports;
};
struct vk_cmd_set_scissor_with_count {
   uint32_t scissor_count;
   VkRect2D* scissors;
};
struct vk_cmd_bind_vertex_buffers2 {
   uint32_t first_binding;
   uint32_t binding_count;
   VkBuffer* buffers;
   VkDeviceSize* offsets;
   VkDeviceSize* sizes;
   VkDeviceSize* strides;
};
struct vk_cmd_set_depth_test_enable {
   VkBool32 depth_test_enable;
};
struct vk_cmd_set_depth_write_enable {
   VkBool32 depth_write_enable;
};
struct vk_cmd_set_depth_compare_op {
   VkCompareOp depth_compare_op;
};
struct vk_cmd_set_depth_bounds_test_enable {
   VkBool32 depth_bounds_test_enable;
};
struct vk_cmd_set_stencil_test_enable {
   VkBool32 stencil_test_enable;
};
struct vk_cmd_set_stencil_op {
   VkStencilFaceFlags face_mask;
   VkStencilOp fail_op;
   VkStencilOp pass_op;
   VkStencilOp depth_fail_op;
   VkCompareOp compare_op;
};
struct vk_cmd_set_patch_control_points_ext {
   uint32_t patch_control_points;
};
struct vk_cmd_set_rasterizer_discard_enable {
   VkBool32 rasterizer_discard_enable;
};
struct vk_cmd_set_depth_bias_enable {
   VkBool32 depth_bias_enable;
};
struct vk_cmd_set_logic_op_ext {
   VkLogicOp logic_op;
};
struct vk_cmd_set_primitive_restart_enable {
   VkBool32 primitive_restart_enable;
};
struct vk_cmd_set_tessellation_domain_origin_ext {
   VkTessellationDomainOrigin domain_origin;
};
struct vk_cmd_set_depth_clamp_enable_ext {
   VkBool32 depth_clamp_enable;
};
struct vk_cmd_set_polygon_mode_ext {
   VkPolygonMode polygon_mode;
};
struct vk_cmd_set_rasterization_samples_ext {
   VkSampleCountFlagBits  rasterization_samples;
};
struct vk_cmd_set_sample_mask_ext {
   VkSampleCountFlagBits  samples;
   VkSampleMask*    sample_mask;
};
struct vk_cmd_set_alpha_to_coverage_enable_ext {
   VkBool32 alpha_to_coverage_enable;
};
struct vk_cmd_set_alpha_to_one_enable_ext {
   VkBool32 alpha_to_one_enable;
};
struct vk_cmd_set_logic_op_enable_ext {
   VkBool32 logic_op_enable;
};
struct vk_cmd_set_color_blend_enable_ext {
   uint32_t first_attachment;
   uint32_t attachment_count;
   VkBool32* color_blend_enables;
};
struct vk_cmd_set_color_blend_equation_ext {
   uint32_t first_attachment;
   uint32_t attachment_count;
   VkColorBlendEquationEXT* color_blend_equations;
};
struct vk_cmd_set_color_write_mask_ext {
   uint32_t first_attachment;
   uint32_t attachment_count;
   VkColorComponentFlags* color_write_masks;
};
struct vk_cmd_set_rasterization_stream_ext {
   uint32_t rasterization_stream;
};
struct vk_cmd_set_conservative_rasterization_mode_ext {
   VkConservativeRasterizationModeEXT conservative_rasterization_mode;
};
struct vk_cmd_set_extra_primitive_overestimation_size_ext {
   float extra_primitive_overestimation_size;
};
struct vk_cmd_set_depth_clip_enable_ext {
   VkBool32 depth_clip_enable;
};
struct vk_cmd_set_sample_locations_enable_ext {
   VkBool32 sample_locations_enable;
};
struct vk_cmd_set_color_blend_advanced_ext {
   uint32_t first_attachment;
   uint32_t attachment_count;
   VkColorBlendAdvancedEXT* color_blend_advanced;
};
struct vk_cmd_set_provoking_vertex_mode_ext {
   VkProvokingVertexModeEXT provoking_vertex_mode;
};
struct vk_cmd_set_line_rasterization_mode_ext {
   VkLineRasterizationModeEXT line_rasterization_mode;
};
struct vk_cmd_set_line_stipple_enable_ext {
   VkBool32 stippled_line_enable;
};
struct vk_cmd_set_depth_clip_negative_one_to_one_ext {
   VkBool32 negative_one_to_one;
};
struct vk_cmd_set_viewport_wscaling_enable_nv {
   VkBool32 viewport_wscaling_enable;
};
struct vk_cmd_set_viewport_swizzle_nv {
   uint32_t first_viewport;
   uint32_t viewport_count;
   VkViewportSwizzleNV* viewport_swizzles;
};
struct vk_cmd_set_coverage_to_color_enable_nv {
   VkBool32 coverage_to_color_enable;
};
struct vk_cmd_set_coverage_to_color_location_nv {
   uint32_t coverage_to_color_location;
};
struct vk_cmd_set_coverage_modulation_mode_nv {
   VkCoverageModulationModeNV coverage_modulation_mode;
};
struct vk_cmd_set_coverage_modulation_table_enable_nv {
   VkBool32 coverage_modulation_table_enable;
};
struct vk_cmd_set_coverage_modulation_table_nv {
   uint32_t coverage_modulation_table_count;
   float* coverage_modulation_table;
};
struct vk_cmd_set_shading_rate_image_enable_nv {
   VkBool32 shading_rate_image_enable;
};
struct vk_cmd_set_coverage_reduction_mode_nv {
   VkCoverageReductionModeNV coverage_reduction_mode;
};
struct vk_cmd_set_representative_fragment_test_enable_nv {
   VkBool32 representative_fragment_test_enable;
};
struct vk_cmd_copy_buffer2 {
   VkCopyBufferInfo2* copy_buffer_info;
};
struct vk_cmd_copy_image2 {
   VkCopyImageInfo2* copy_image_info;
};
struct vk_cmd_blit_image2 {
   VkBlitImageInfo2* blit_image_info;
};
struct vk_cmd_copy_buffer_to_image2 {
   VkCopyBufferToImageInfo2* copy_buffer_to_image_info;
};
struct vk_cmd_copy_image_to_buffer2 {
   VkCopyImageToBufferInfo2* copy_image_to_buffer_info;
};
struct vk_cmd_resolve_image2 {
   VkResolveImageInfo2* resolve_image_info;
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
struct vk_cmd_set_event2 {
   VkEvent                                             event;
   VkDependencyInfo*                             dependency_info;
};
struct vk_cmd_reset_event2 {
   VkEvent                                             event;
   VkPipelineStageFlags2               stage_mask;
};
struct vk_cmd_wait_events2 {
   uint32_t                                            event_count;
   VkEvent*                     events;
   VkDependencyInfo*            dependency_infos;
};
struct vk_cmd_pipeline_barrier2 {
   VkDependencyInfo*                             dependency_info;
};
struct vk_cmd_write_timestamp2 {
   VkPipelineStageFlags2               stage;
   VkQueryPool                                         query_pool;
   uint32_t                                            query;
};
struct vk_cmd_write_buffer_marker2_amd {
   VkPipelineStageFlags2               stage;
   VkBuffer                                            dst_buffer;
   VkDeviceSize                                        dst_offset;
   uint32_t                                            marker;
};
#ifdef VK_ENABLE_BETA_EXTENSIONS
struct vk_cmd_decode_video_khr {
   VkVideoDecodeInfoKHR* decode_info;
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
struct vk_cmd_begin_rendering {
   VkRenderingInfo*                              rendering_info;
};
struct vk_cmd_build_micromaps_ext {
   uint32_t info_count;
   VkMicromapBuildInfoEXT* infos;
};
struct vk_cmd_copy_micromap_ext {
   VkCopyMicromapInfoEXT* info;
};
struct vk_cmd_copy_micromap_to_memory_ext {
   VkCopyMicromapToMemoryInfoEXT* info;
};
struct vk_cmd_copy_memory_to_micromap_ext {
   VkCopyMemoryToMicromapInfoEXT* info;
};
struct vk_cmd_write_micromaps_properties_ext {
   uint32_t micromap_count;
   VkMicromapEXT* micromaps;
   VkQueryType query_type;
   VkQueryPool query_pool;
   uint32_t first_query;
};
struct vk_cmd_optical_flow_execute_nv {
   VkOpticalFlowSessionNV session;
   VkOpticalFlowExecuteInfoNV* execute_info;
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
      struct vk_cmd_draw_mesh_tasks_ext draw_mesh_tasks_ext;
      struct vk_cmd_draw_mesh_tasks_indirect_ext draw_mesh_tasks_indirect_ext;
      struct vk_cmd_draw_mesh_tasks_indirect_count_ext draw_mesh_tasks_indirect_count_ext;
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
      struct vk_cmd_trace_rays_indirect2_khr trace_rays_indirect2_khr;
      struct vk_cmd_set_ray_tracing_pipeline_stack_size_khr set_ray_tracing_pipeline_stack_size_khr;
      struct vk_cmd_set_performance_marker_intel set_performance_marker_intel;
      struct vk_cmd_set_performance_stream_marker_intel set_performance_stream_marker_intel;
      struct vk_cmd_set_performance_override_intel set_performance_override_intel;
      struct vk_cmd_set_line_stipple_ext set_line_stipple_ext;
      struct vk_cmd_build_acceleration_structures_khr build_acceleration_structures_khr;
      struct vk_cmd_build_acceleration_structures_indirect_khr build_acceleration_structures_indirect_khr;
      struct vk_cmd_set_cull_mode set_cull_mode;
      struct vk_cmd_set_front_face set_front_face;
      struct vk_cmd_set_primitive_topology set_primitive_topology;
      struct vk_cmd_set_viewport_with_count set_viewport_with_count;
      struct vk_cmd_set_scissor_with_count set_scissor_with_count;
      struct vk_cmd_bind_vertex_buffers2 bind_vertex_buffers2;
      struct vk_cmd_set_depth_test_enable set_depth_test_enable;
      struct vk_cmd_set_depth_write_enable set_depth_write_enable;
      struct vk_cmd_set_depth_compare_op set_depth_compare_op;
      struct vk_cmd_set_depth_bounds_test_enable set_depth_bounds_test_enable;
      struct vk_cmd_set_stencil_test_enable set_stencil_test_enable;
      struct vk_cmd_set_stencil_op set_stencil_op;
      struct vk_cmd_set_patch_control_points_ext set_patch_control_points_ext;
      struct vk_cmd_set_rasterizer_discard_enable set_rasterizer_discard_enable;
      struct vk_cmd_set_depth_bias_enable set_depth_bias_enable;
      struct vk_cmd_set_logic_op_ext set_logic_op_ext;
      struct vk_cmd_set_primitive_restart_enable set_primitive_restart_enable;
      struct vk_cmd_set_tessellation_domain_origin_ext set_tessellation_domain_origin_ext;
      struct vk_cmd_set_depth_clamp_enable_ext set_depth_clamp_enable_ext;
      struct vk_cmd_set_polygon_mode_ext set_polygon_mode_ext;
      struct vk_cmd_set_rasterization_samples_ext set_rasterization_samples_ext;
      struct vk_cmd_set_sample_mask_ext set_sample_mask_ext;
      struct vk_cmd_set_alpha_to_coverage_enable_ext set_alpha_to_coverage_enable_ext;
      struct vk_cmd_set_alpha_to_one_enable_ext set_alpha_to_one_enable_ext;
      struct vk_cmd_set_logic_op_enable_ext set_logic_op_enable_ext;
      struct vk_cmd_set_color_blend_enable_ext set_color_blend_enable_ext;
      struct vk_cmd_set_color_blend_equation_ext set_color_blend_equation_ext;
      struct vk_cmd_set_color_write_mask_ext set_color_write_mask_ext;
      struct vk_cmd_set_rasterization_stream_ext set_rasterization_stream_ext;
      struct vk_cmd_set_conservative_rasterization_mode_ext set_conservative_rasterization_mode_ext;
      struct vk_cmd_set_extra_primitive_overestimation_size_ext set_extra_primitive_overestimation_size_ext;
      struct vk_cmd_set_depth_clip_enable_ext set_depth_clip_enable_ext;
      struct vk_cmd_set_sample_locations_enable_ext set_sample_locations_enable_ext;
      struct vk_cmd_set_color_blend_advanced_ext set_color_blend_advanced_ext;
      struct vk_cmd_set_provoking_vertex_mode_ext set_provoking_vertex_mode_ext;
      struct vk_cmd_set_line_rasterization_mode_ext set_line_rasterization_mode_ext;
      struct vk_cmd_set_line_stipple_enable_ext set_line_stipple_enable_ext;
      struct vk_cmd_set_depth_clip_negative_one_to_one_ext set_depth_clip_negative_one_to_one_ext;
      struct vk_cmd_set_viewport_wscaling_enable_nv set_viewport_wscaling_enable_nv;
      struct vk_cmd_set_viewport_swizzle_nv set_viewport_swizzle_nv;
      struct vk_cmd_set_coverage_to_color_enable_nv set_coverage_to_color_enable_nv;
      struct vk_cmd_set_coverage_to_color_location_nv set_coverage_to_color_location_nv;
      struct vk_cmd_set_coverage_modulation_mode_nv set_coverage_modulation_mode_nv;
      struct vk_cmd_set_coverage_modulation_table_enable_nv set_coverage_modulation_table_enable_nv;
      struct vk_cmd_set_coverage_modulation_table_nv set_coverage_modulation_table_nv;
      struct vk_cmd_set_shading_rate_image_enable_nv set_shading_rate_image_enable_nv;
      struct vk_cmd_set_coverage_reduction_mode_nv set_coverage_reduction_mode_nv;
      struct vk_cmd_set_representative_fragment_test_enable_nv set_representative_fragment_test_enable_nv;
      struct vk_cmd_copy_buffer2 copy_buffer2;
      struct vk_cmd_copy_image2 copy_image2;
      struct vk_cmd_blit_image2 blit_image2;
      struct vk_cmd_copy_buffer_to_image2 copy_buffer_to_image2;
      struct vk_cmd_copy_image_to_buffer2 copy_image_to_buffer2;
      struct vk_cmd_resolve_image2 resolve_image2;
      struct vk_cmd_set_fragment_shading_rate_khr set_fragment_shading_rate_khr;
      struct vk_cmd_set_fragment_shading_rate_enum_nv set_fragment_shading_rate_enum_nv;
      struct vk_cmd_set_vertex_input_ext set_vertex_input_ext;
      struct vk_cmd_set_color_write_enable_ext set_color_write_enable_ext;
      struct vk_cmd_set_event2 set_event2;
      struct vk_cmd_reset_event2 reset_event2;
      struct vk_cmd_wait_events2 wait_events2;
      struct vk_cmd_pipeline_barrier2 pipeline_barrier2;
      struct vk_cmd_write_timestamp2 write_timestamp2;
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
      struct vk_cmd_begin_rendering begin_rendering;
      struct vk_cmd_build_micromaps_ext build_micromaps_ext;
      struct vk_cmd_copy_micromap_ext copy_micromap_ext;
      struct vk_cmd_copy_micromap_to_memory_ext copy_micromap_to_memory_ext;
      struct vk_cmd_copy_memory_to_micromap_ext copy_memory_to_micromap_ext;
      struct vk_cmd_write_micromaps_properties_ext write_micromaps_properties_ext;
      struct vk_cmd_optical_flow_execute_nv optical_flow_execute_nv;
   } u;
   void *driver_data;
   void (*driver_free_cb)(struct vk_cmd_queue *queue,
                          struct vk_cmd_queue_entry *cmd);
};

  VkResult vk_enqueue_cmd_bind_pipeline(struct vk_cmd_queue *queue
   , VkPipelineBindPoint pipelineBindPoint
   , VkPipeline pipeline
  );

  VkResult vk_enqueue_cmd_set_viewport(struct vk_cmd_queue *queue
   , uint32_t firstViewport
   , uint32_t viewportCount
   , const VkViewport* pViewports
  );

  VkResult vk_enqueue_cmd_set_scissor(struct vk_cmd_queue *queue
   , uint32_t firstScissor
   , uint32_t scissorCount
   , const VkRect2D* pScissors
  );

  VkResult vk_enqueue_cmd_set_line_width(struct vk_cmd_queue *queue
   , float lineWidth
  );

  VkResult vk_enqueue_cmd_set_depth_bias(struct vk_cmd_queue *queue
   , float depthBiasConstantFactor
   , float depthBiasClamp
   , float depthBiasSlopeFactor
  );

  VkResult vk_enqueue_cmd_set_blend_constants(struct vk_cmd_queue *queue
   , const float blendConstants[4]
  );

  VkResult vk_enqueue_cmd_set_depth_bounds(struct vk_cmd_queue *queue
   , float minDepthBounds
   , float maxDepthBounds
  );

  VkResult vk_enqueue_cmd_set_stencil_compare_mask(struct vk_cmd_queue *queue
   , VkStencilFaceFlags faceMask
   , uint32_t compareMask
  );

  VkResult vk_enqueue_cmd_set_stencil_write_mask(struct vk_cmd_queue *queue
   , VkStencilFaceFlags faceMask
   , uint32_t writeMask
  );

  VkResult vk_enqueue_cmd_set_stencil_reference(struct vk_cmd_queue *queue
   , VkStencilFaceFlags faceMask
   , uint32_t reference
  );

  VkResult vk_enqueue_cmd_bind_index_buffer(struct vk_cmd_queue *queue
   , VkBuffer buffer
   , VkDeviceSize offset
   , VkIndexType indexType
  );

  VkResult vk_enqueue_cmd_bind_vertex_buffers(struct vk_cmd_queue *queue
   , uint32_t firstBinding
   , uint32_t bindingCount
   , const VkBuffer* pBuffers
   , const VkDeviceSize* pOffsets
  );

  VkResult vk_enqueue_cmd_draw(struct vk_cmd_queue *queue
   , uint32_t vertexCount
   , uint32_t instanceCount
   , uint32_t firstVertex
   , uint32_t firstInstance
  );

  VkResult vk_enqueue_cmd_draw_indexed(struct vk_cmd_queue *queue
   , uint32_t indexCount
   , uint32_t instanceCount
   , uint32_t firstIndex
   , int32_t vertexOffset
   , uint32_t firstInstance
  );

  VkResult vk_enqueue_cmd_draw_indirect(struct vk_cmd_queue *queue
   , VkBuffer buffer
   , VkDeviceSize offset
   , uint32_t drawCount
   , uint32_t stride
  );

  VkResult vk_enqueue_cmd_draw_indexed_indirect(struct vk_cmd_queue *queue
   , VkBuffer buffer
   , VkDeviceSize offset
   , uint32_t drawCount
   , uint32_t stride
  );

  VkResult vk_enqueue_cmd_dispatch(struct vk_cmd_queue *queue
   , uint32_t groupCountX
   , uint32_t groupCountY
   , uint32_t groupCountZ
  );

  VkResult vk_enqueue_cmd_dispatch_indirect(struct vk_cmd_queue *queue
   , VkBuffer buffer
   , VkDeviceSize offset
  );

  VkResult vk_enqueue_cmd_subpass_shading_huawei(struct vk_cmd_queue *queue
  );

  VkResult vk_enqueue_cmd_copy_buffer(struct vk_cmd_queue *queue
   , VkBuffer srcBuffer
   , VkBuffer dstBuffer
   , uint32_t regionCount
   , const VkBufferCopy* pRegions
  );

  VkResult vk_enqueue_cmd_copy_image(struct vk_cmd_queue *queue
   , VkImage srcImage
   , VkImageLayout srcImageLayout
   , VkImage dstImage
   , VkImageLayout dstImageLayout
   , uint32_t regionCount
   , const VkImageCopy* pRegions
  );

  VkResult vk_enqueue_cmd_blit_image(struct vk_cmd_queue *queue
   , VkImage srcImage
   , VkImageLayout srcImageLayout
   , VkImage dstImage
   , VkImageLayout dstImageLayout
   , uint32_t regionCount
   , const VkImageBlit* pRegions
   , VkFilter filter
  );

  VkResult vk_enqueue_cmd_copy_buffer_to_image(struct vk_cmd_queue *queue
   , VkBuffer srcBuffer
   , VkImage dstImage
   , VkImageLayout dstImageLayout
   , uint32_t regionCount
   , const VkBufferImageCopy* pRegions
  );

  VkResult vk_enqueue_cmd_copy_image_to_buffer(struct vk_cmd_queue *queue
   , VkImage srcImage
   , VkImageLayout srcImageLayout
   , VkBuffer dstBuffer
   , uint32_t regionCount
   , const VkBufferImageCopy* pRegions
  );

  VkResult vk_enqueue_cmd_update_buffer(struct vk_cmd_queue *queue
   , VkBuffer dstBuffer
   , VkDeviceSize dstOffset
   , VkDeviceSize dataSize
   , const void* pData
  );

  VkResult vk_enqueue_cmd_fill_buffer(struct vk_cmd_queue *queue
   , VkBuffer dstBuffer
   , VkDeviceSize dstOffset
   , VkDeviceSize size
   , uint32_t data
  );

  VkResult vk_enqueue_cmd_clear_color_image(struct vk_cmd_queue *queue
   , VkImage image
   , VkImageLayout imageLayout
   , const VkClearColorValue* pColor
   , uint32_t rangeCount
   , const VkImageSubresourceRange* pRanges
  );

  VkResult vk_enqueue_cmd_clear_depth_stencil_image(struct vk_cmd_queue *queue
   , VkImage image
   , VkImageLayout imageLayout
   , const VkClearDepthStencilValue* pDepthStencil
   , uint32_t rangeCount
   , const VkImageSubresourceRange* pRanges
  );

  VkResult vk_enqueue_cmd_clear_attachments(struct vk_cmd_queue *queue
   , uint32_t attachmentCount
   , const VkClearAttachment* pAttachments
   , uint32_t rectCount
   , const VkClearRect* pRects
  );

  VkResult vk_enqueue_cmd_resolve_image(struct vk_cmd_queue *queue
   , VkImage srcImage
   , VkImageLayout srcImageLayout
   , VkImage dstImage
   , VkImageLayout dstImageLayout
   , uint32_t regionCount
   , const VkImageResolve* pRegions
  );

  VkResult vk_enqueue_cmd_set_event(struct vk_cmd_queue *queue
   , VkEvent event
   , VkPipelineStageFlags stageMask
  );

  VkResult vk_enqueue_cmd_reset_event(struct vk_cmd_queue *queue
   , VkEvent event
   , VkPipelineStageFlags stageMask
  );

  VkResult vk_enqueue_cmd_wait_events(struct vk_cmd_queue *queue
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

  VkResult vk_enqueue_cmd_pipeline_barrier(struct vk_cmd_queue *queue
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

  VkResult vk_enqueue_cmd_begin_query(struct vk_cmd_queue *queue
   , VkQueryPool queryPool
   , uint32_t query
   , VkQueryControlFlags flags
  );

  VkResult vk_enqueue_cmd_end_query(struct vk_cmd_queue *queue
   , VkQueryPool queryPool
   , uint32_t query
  );

  VkResult vk_enqueue_cmd_begin_conditional_rendering_ext(struct vk_cmd_queue *queue
   , const VkConditionalRenderingBeginInfoEXT* pConditionalRenderingBegin
  );

  VkResult vk_enqueue_cmd_end_conditional_rendering_ext(struct vk_cmd_queue *queue
  );

  VkResult vk_enqueue_cmd_reset_query_pool(struct vk_cmd_queue *queue
   , VkQueryPool queryPool
   , uint32_t firstQuery
   , uint32_t queryCount
  );

  VkResult vk_enqueue_cmd_write_timestamp(struct vk_cmd_queue *queue
   , VkPipelineStageFlagBits pipelineStage
   , VkQueryPool queryPool
   , uint32_t query
  );

  VkResult vk_enqueue_cmd_copy_query_pool_results(struct vk_cmd_queue *queue
   , VkQueryPool queryPool
   , uint32_t firstQuery
   , uint32_t queryCount
   , VkBuffer dstBuffer
   , VkDeviceSize dstOffset
   , VkDeviceSize stride
   , VkQueryResultFlags flags
  );

  VkResult vk_enqueue_cmd_push_constants(struct vk_cmd_queue *queue
   , VkPipelineLayout layout
   , VkShaderStageFlags stageFlags
   , uint32_t offset
   , uint32_t size
   , const void* pValues
  );

  VkResult vk_enqueue_cmd_begin_render_pass(struct vk_cmd_queue *queue
   , const VkRenderPassBeginInfo* pRenderPassBegin
   , VkSubpassContents contents
  );

  VkResult vk_enqueue_cmd_next_subpass(struct vk_cmd_queue *queue
   , VkSubpassContents contents
  );

  VkResult vk_enqueue_cmd_end_render_pass(struct vk_cmd_queue *queue
  );

  VkResult vk_enqueue_cmd_execute_commands(struct vk_cmd_queue *queue
   , uint32_t commandBufferCount
   , const VkCommandBuffer* pCommandBuffers
  );

  VkResult vk_enqueue_cmd_debug_marker_begin_ext(struct vk_cmd_queue *queue
   , const VkDebugMarkerMarkerInfoEXT* pMarkerInfo
  );

  VkResult vk_enqueue_cmd_debug_marker_end_ext(struct vk_cmd_queue *queue
  );

  VkResult vk_enqueue_cmd_debug_marker_insert_ext(struct vk_cmd_queue *queue
   , const VkDebugMarkerMarkerInfoEXT* pMarkerInfo
  );

  VkResult vk_enqueue_cmd_execute_generated_commands_nv(struct vk_cmd_queue *queue
   , VkBool32 isPreprocessed
   , const VkGeneratedCommandsInfoNV* pGeneratedCommandsInfo
  );

  VkResult vk_enqueue_cmd_preprocess_generated_commands_nv(struct vk_cmd_queue *queue
   , const VkGeneratedCommandsInfoNV* pGeneratedCommandsInfo
  );

  VkResult vk_enqueue_cmd_bind_pipeline_shader_group_nv(struct vk_cmd_queue *queue
   , VkPipelineBindPoint pipelineBindPoint
   , VkPipeline pipeline
   , uint32_t groupIndex
  );

  VkResult vk_enqueue_cmd_set_device_mask(struct vk_cmd_queue *queue
   , uint32_t deviceMask
  );

  VkResult vk_enqueue_cmd_dispatch_base(struct vk_cmd_queue *queue
   , uint32_t baseGroupX
   , uint32_t baseGroupY
   , uint32_t baseGroupZ
   , uint32_t groupCountX
   , uint32_t groupCountY
   , uint32_t groupCountZ
  );

  VkResult vk_enqueue_cmd_set_viewport_wscaling_nv(struct vk_cmd_queue *queue
   , uint32_t firstViewport
   , uint32_t viewportCount
   , const VkViewportWScalingNV* pViewportWScalings
  );

  VkResult vk_enqueue_cmd_set_discard_rectangle_ext(struct vk_cmd_queue *queue
   , uint32_t firstDiscardRectangle
   , uint32_t discardRectangleCount
   , const VkRect2D* pDiscardRectangles
  );

  VkResult vk_enqueue_cmd_set_sample_locations_ext(struct vk_cmd_queue *queue
   , const VkSampleLocationsInfoEXT* pSampleLocationsInfo
  );

  VkResult vk_enqueue_cmd_begin_debug_utils_label_ext(struct vk_cmd_queue *queue
   , const VkDebugUtilsLabelEXT* pLabelInfo
  );

  VkResult vk_enqueue_cmd_end_debug_utils_label_ext(struct vk_cmd_queue *queue
  );

  VkResult vk_enqueue_cmd_insert_debug_utils_label_ext(struct vk_cmd_queue *queue
   , const VkDebugUtilsLabelEXT* pLabelInfo
  );

  VkResult vk_enqueue_cmd_write_buffer_marker_amd(struct vk_cmd_queue *queue
   , VkPipelineStageFlagBits pipelineStage
   , VkBuffer dstBuffer
   , VkDeviceSize dstOffset
   , uint32_t marker
  );

  VkResult vk_enqueue_cmd_begin_render_pass2(struct vk_cmd_queue *queue
   , const VkRenderPassBeginInfo*      pRenderPassBegin
   , const VkSubpassBeginInfo*      pSubpassBeginInfo
  );

  VkResult vk_enqueue_cmd_next_subpass2(struct vk_cmd_queue *queue
   , const VkSubpassBeginInfo*      pSubpassBeginInfo
   , const VkSubpassEndInfo*        pSubpassEndInfo
  );

  VkResult vk_enqueue_cmd_end_render_pass2(struct vk_cmd_queue *queue
   , const VkSubpassEndInfo*        pSubpassEndInfo
  );

  VkResult vk_enqueue_cmd_draw_indirect_count(struct vk_cmd_queue *queue
   , VkBuffer buffer
   , VkDeviceSize offset
   , VkBuffer countBuffer
   , VkDeviceSize countBufferOffset
   , uint32_t maxDrawCount
   , uint32_t stride
  );

  VkResult vk_enqueue_cmd_draw_indexed_indirect_count(struct vk_cmd_queue *queue
   , VkBuffer buffer
   , VkDeviceSize offset
   , VkBuffer countBuffer
   , VkDeviceSize countBufferOffset
   , uint32_t maxDrawCount
   , uint32_t stride
  );

  VkResult vk_enqueue_cmd_set_checkpoint_nv(struct vk_cmd_queue *queue
   , const void* pCheckpointMarker
  );

  VkResult vk_enqueue_cmd_bind_transform_feedback_buffers_ext(struct vk_cmd_queue *queue
   , uint32_t firstBinding
   , uint32_t bindingCount
   , const VkBuffer* pBuffers
   , const VkDeviceSize* pOffsets
   , const VkDeviceSize* pSizes
  );

  VkResult vk_enqueue_cmd_begin_transform_feedback_ext(struct vk_cmd_queue *queue
   , uint32_t firstCounterBuffer
   , uint32_t counterBufferCount
   , const VkBuffer* pCounterBuffers
   , const VkDeviceSize* pCounterBufferOffsets
  );

  VkResult vk_enqueue_cmd_end_transform_feedback_ext(struct vk_cmd_queue *queue
   , uint32_t firstCounterBuffer
   , uint32_t counterBufferCount
   , const VkBuffer* pCounterBuffers
   , const VkDeviceSize* pCounterBufferOffsets
  );

  VkResult vk_enqueue_cmd_begin_query_indexed_ext(struct vk_cmd_queue *queue
   , VkQueryPool queryPool
   , uint32_t query
   , VkQueryControlFlags flags
   , uint32_t index
  );

  VkResult vk_enqueue_cmd_end_query_indexed_ext(struct vk_cmd_queue *queue
   , VkQueryPool queryPool
   , uint32_t query
   , uint32_t index
  );

  VkResult vk_enqueue_cmd_draw_indirect_byte_count_ext(struct vk_cmd_queue *queue
   , uint32_t instanceCount
   , uint32_t firstInstance
   , VkBuffer counterBuffer
   , VkDeviceSize counterBufferOffset
   , uint32_t counterOffset
   , uint32_t vertexStride
  );

  VkResult vk_enqueue_cmd_set_exclusive_scissor_nv(struct vk_cmd_queue *queue
   , uint32_t firstExclusiveScissor
   , uint32_t exclusiveScissorCount
   , const VkRect2D* pExclusiveScissors
  );

  VkResult vk_enqueue_cmd_bind_shading_rate_image_nv(struct vk_cmd_queue *queue
   , VkImageView imageView
   , VkImageLayout imageLayout
  );

  VkResult vk_enqueue_cmd_set_viewport_shading_rate_palette_nv(struct vk_cmd_queue *queue
   , uint32_t firstViewport
   , uint32_t viewportCount
   , const VkShadingRatePaletteNV* pShadingRatePalettes
  );

  VkResult vk_enqueue_cmd_set_coarse_sample_order_nv(struct vk_cmd_queue *queue
   , VkCoarseSampleOrderTypeNV sampleOrderType
   , uint32_t customSampleOrderCount
   , const VkCoarseSampleOrderCustomNV* pCustomSampleOrders
  );

  VkResult vk_enqueue_cmd_draw_mesh_tasks_nv(struct vk_cmd_queue *queue
   , uint32_t taskCount
   , uint32_t firstTask
  );

  VkResult vk_enqueue_cmd_draw_mesh_tasks_indirect_nv(struct vk_cmd_queue *queue
   , VkBuffer buffer
   , VkDeviceSize offset
   , uint32_t drawCount
   , uint32_t stride
  );

  VkResult vk_enqueue_cmd_draw_mesh_tasks_indirect_count_nv(struct vk_cmd_queue *queue
   , VkBuffer buffer
   , VkDeviceSize offset
   , VkBuffer countBuffer
   , VkDeviceSize countBufferOffset
   , uint32_t maxDrawCount
   , uint32_t stride
  );

  VkResult vk_enqueue_cmd_draw_mesh_tasks_ext(struct vk_cmd_queue *queue
   , uint32_t groupCountX
   , uint32_t groupCountY
   , uint32_t groupCountZ
  );

  VkResult vk_enqueue_cmd_draw_mesh_tasks_indirect_ext(struct vk_cmd_queue *queue
   , VkBuffer buffer
   , VkDeviceSize offset
   , uint32_t drawCount
   , uint32_t stride
  );

  VkResult vk_enqueue_cmd_draw_mesh_tasks_indirect_count_ext(struct vk_cmd_queue *queue
   , VkBuffer buffer
   , VkDeviceSize offset
   , VkBuffer countBuffer
   , VkDeviceSize countBufferOffset
   , uint32_t maxDrawCount
   , uint32_t stride
  );

  VkResult vk_enqueue_cmd_bind_invocation_mask_huawei(struct vk_cmd_queue *queue
   , VkImageView imageView
   , VkImageLayout imageLayout
  );

  VkResult vk_enqueue_cmd_copy_acceleration_structure_nv(struct vk_cmd_queue *queue
   , VkAccelerationStructureNV dst
   , VkAccelerationStructureNV src
   , VkCopyAccelerationStructureModeKHR mode
  );

  VkResult vk_enqueue_cmd_copy_acceleration_structure_khr(struct vk_cmd_queue *queue
   , const VkCopyAccelerationStructureInfoKHR* pInfo
  );

  VkResult vk_enqueue_cmd_copy_acceleration_structure_to_memory_khr(struct vk_cmd_queue *queue
   , const VkCopyAccelerationStructureToMemoryInfoKHR* pInfo
  );

  VkResult vk_enqueue_cmd_copy_memory_to_acceleration_structure_khr(struct vk_cmd_queue *queue
   , const VkCopyMemoryToAccelerationStructureInfoKHR* pInfo
  );

  VkResult vk_enqueue_cmd_write_acceleration_structures_properties_khr(struct vk_cmd_queue *queue
   , uint32_t accelerationStructureCount
   , const VkAccelerationStructureKHR* pAccelerationStructures
   , VkQueryType queryType
   , VkQueryPool queryPool
   , uint32_t firstQuery
  );

  VkResult vk_enqueue_cmd_write_acceleration_structures_properties_nv(struct vk_cmd_queue *queue
   , uint32_t accelerationStructureCount
   , const VkAccelerationStructureNV* pAccelerationStructures
   , VkQueryType queryType
   , VkQueryPool queryPool
   , uint32_t firstQuery
  );

  VkResult vk_enqueue_cmd_build_acceleration_structure_nv(struct vk_cmd_queue *queue
   , const VkAccelerationStructureInfoNV* pInfo
   , VkBuffer instanceData
   , VkDeviceSize instanceOffset
   , VkBool32 update
   , VkAccelerationStructureNV dst
   , VkAccelerationStructureNV src
   , VkBuffer scratch
   , VkDeviceSize scratchOffset
  );

  VkResult vk_enqueue_cmd_trace_rays_khr(struct vk_cmd_queue *queue
   , const VkStridedDeviceAddressRegionKHR* pRaygenShaderBindingTable
   , const VkStridedDeviceAddressRegionKHR* pMissShaderBindingTable
   , const VkStridedDeviceAddressRegionKHR* pHitShaderBindingTable
   , const VkStridedDeviceAddressRegionKHR* pCallableShaderBindingTable
   , uint32_t width
   , uint32_t height
   , uint32_t depth
  );

  VkResult vk_enqueue_cmd_trace_rays_nv(struct vk_cmd_queue *queue
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

  VkResult vk_enqueue_cmd_trace_rays_indirect_khr(struct vk_cmd_queue *queue
   , const VkStridedDeviceAddressRegionKHR* pRaygenShaderBindingTable
   , const VkStridedDeviceAddressRegionKHR* pMissShaderBindingTable
   , const VkStridedDeviceAddressRegionKHR* pHitShaderBindingTable
   , const VkStridedDeviceAddressRegionKHR* pCallableShaderBindingTable
   , VkDeviceAddress indirectDeviceAddress
  );

  VkResult vk_enqueue_cmd_trace_rays_indirect2_khr(struct vk_cmd_queue *queue
   , VkDeviceAddress indirectDeviceAddress
  );

  VkResult vk_enqueue_cmd_set_ray_tracing_pipeline_stack_size_khr(struct vk_cmd_queue *queue
   , uint32_t pipelineStackSize
  );

  VkResult vk_enqueue_cmd_set_line_stipple_ext(struct vk_cmd_queue *queue
   , uint32_t lineStippleFactor
   , uint16_t lineStipplePattern
  );

  VkResult vk_enqueue_cmd_build_acceleration_structures_khr(struct vk_cmd_queue *queue
   , uint32_t infoCount
   , const VkAccelerationStructureBuildGeometryInfoKHR* pInfos
   , const VkAccelerationStructureBuildRangeInfoKHR* const* ppBuildRangeInfos
  );

  VkResult vk_enqueue_cmd_build_acceleration_structures_indirect_khr(struct vk_cmd_queue *queue
   , uint32_t                                           infoCount
   , const VkAccelerationStructureBuildGeometryInfoKHR* pInfos
   , const VkDeviceAddress*             pIndirectDeviceAddresses
   , const uint32_t*                    pIndirectStrides
   , const uint32_t* const*             ppMaxPrimitiveCounts
  );

  VkResult vk_enqueue_cmd_set_cull_mode(struct vk_cmd_queue *queue
   , VkCullModeFlags cullMode
  );

  VkResult vk_enqueue_cmd_set_front_face(struct vk_cmd_queue *queue
   , VkFrontFace frontFace
  );

  VkResult vk_enqueue_cmd_set_primitive_topology(struct vk_cmd_queue *queue
   , VkPrimitiveTopology primitiveTopology
  );

  VkResult vk_enqueue_cmd_set_viewport_with_count(struct vk_cmd_queue *queue
   , uint32_t viewportCount
   , const VkViewport* pViewports
  );

  VkResult vk_enqueue_cmd_set_scissor_with_count(struct vk_cmd_queue *queue
   , uint32_t scissorCount
   , const VkRect2D* pScissors
  );

  VkResult vk_enqueue_cmd_bind_vertex_buffers2(struct vk_cmd_queue *queue
   , uint32_t firstBinding
   , uint32_t bindingCount
   , const VkBuffer* pBuffers
   , const VkDeviceSize* pOffsets
   , const VkDeviceSize* pSizes
   , const VkDeviceSize* pStrides
  );

  VkResult vk_enqueue_cmd_set_depth_test_enable(struct vk_cmd_queue *queue
   , VkBool32 depthTestEnable
  );

  VkResult vk_enqueue_cmd_set_depth_write_enable(struct vk_cmd_queue *queue
   , VkBool32 depthWriteEnable
  );

  VkResult vk_enqueue_cmd_set_depth_compare_op(struct vk_cmd_queue *queue
   , VkCompareOp depthCompareOp
  );

  VkResult vk_enqueue_cmd_set_depth_bounds_test_enable(struct vk_cmd_queue *queue
   , VkBool32 depthBoundsTestEnable
  );

  VkResult vk_enqueue_cmd_set_stencil_test_enable(struct vk_cmd_queue *queue
   , VkBool32 stencilTestEnable
  );

  VkResult vk_enqueue_cmd_set_stencil_op(struct vk_cmd_queue *queue
   , VkStencilFaceFlags faceMask
   , VkStencilOp failOp
   , VkStencilOp passOp
   , VkStencilOp depthFailOp
   , VkCompareOp compareOp
  );

  VkResult vk_enqueue_cmd_set_patch_control_points_ext(struct vk_cmd_queue *queue
   , uint32_t patchControlPoints
  );

  VkResult vk_enqueue_cmd_set_rasterizer_discard_enable(struct vk_cmd_queue *queue
   , VkBool32 rasterizerDiscardEnable
  );

  VkResult vk_enqueue_cmd_set_depth_bias_enable(struct vk_cmd_queue *queue
   , VkBool32 depthBiasEnable
  );

  VkResult vk_enqueue_cmd_set_logic_op_ext(struct vk_cmd_queue *queue
   , VkLogicOp logicOp
  );

  VkResult vk_enqueue_cmd_set_primitive_restart_enable(struct vk_cmd_queue *queue
   , VkBool32 primitiveRestartEnable
  );

  VkResult vk_enqueue_cmd_set_tessellation_domain_origin_ext(struct vk_cmd_queue *queue
   , VkTessellationDomainOrigin domainOrigin
  );

  VkResult vk_enqueue_cmd_set_depth_clamp_enable_ext(struct vk_cmd_queue *queue
   , VkBool32 depthClampEnable
  );

  VkResult vk_enqueue_cmd_set_polygon_mode_ext(struct vk_cmd_queue *queue
   , VkPolygonMode polygonMode
  );

  VkResult vk_enqueue_cmd_set_rasterization_samples_ext(struct vk_cmd_queue *queue
   , VkSampleCountFlagBits  rasterizationSamples
  );

  VkResult vk_enqueue_cmd_set_sample_mask_ext(struct vk_cmd_queue *queue
   , VkSampleCountFlagBits  samples
   , const VkSampleMask*    pSampleMask
  );

  VkResult vk_enqueue_cmd_set_alpha_to_coverage_enable_ext(struct vk_cmd_queue *queue
   , VkBool32 alphaToCoverageEnable
  );

  VkResult vk_enqueue_cmd_set_alpha_to_one_enable_ext(struct vk_cmd_queue *queue
   , VkBool32 alphaToOneEnable
  );

  VkResult vk_enqueue_cmd_set_logic_op_enable_ext(struct vk_cmd_queue *queue
   , VkBool32 logicOpEnable
  );

  VkResult vk_enqueue_cmd_set_color_blend_enable_ext(struct vk_cmd_queue *queue
   , uint32_t firstAttachment
   , uint32_t attachmentCount
   , const VkBool32* pColorBlendEnables
  );

  VkResult vk_enqueue_cmd_set_color_blend_equation_ext(struct vk_cmd_queue *queue
   , uint32_t firstAttachment
   , uint32_t attachmentCount
   , const VkColorBlendEquationEXT* pColorBlendEquations
  );

  VkResult vk_enqueue_cmd_set_color_write_mask_ext(struct vk_cmd_queue *queue
   , uint32_t firstAttachment
   , uint32_t attachmentCount
   , const VkColorComponentFlags* pColorWriteMasks
  );

  VkResult vk_enqueue_cmd_set_rasterization_stream_ext(struct vk_cmd_queue *queue
   , uint32_t rasterizationStream
  );

  VkResult vk_enqueue_cmd_set_conservative_rasterization_mode_ext(struct vk_cmd_queue *queue
   , VkConservativeRasterizationModeEXT conservativeRasterizationMode
  );

  VkResult vk_enqueue_cmd_set_extra_primitive_overestimation_size_ext(struct vk_cmd_queue *queue
   , float extraPrimitiveOverestimationSize
  );

  VkResult vk_enqueue_cmd_set_depth_clip_enable_ext(struct vk_cmd_queue *queue
   , VkBool32 depthClipEnable
  );

  VkResult vk_enqueue_cmd_set_sample_locations_enable_ext(struct vk_cmd_queue *queue
   , VkBool32 sampleLocationsEnable
  );

  VkResult vk_enqueue_cmd_set_color_blend_advanced_ext(struct vk_cmd_queue *queue
   , uint32_t firstAttachment
   , uint32_t attachmentCount
   , const VkColorBlendAdvancedEXT* pColorBlendAdvanced
  );

  VkResult vk_enqueue_cmd_set_provoking_vertex_mode_ext(struct vk_cmd_queue *queue
   , VkProvokingVertexModeEXT provokingVertexMode
  );

  VkResult vk_enqueue_cmd_set_line_rasterization_mode_ext(struct vk_cmd_queue *queue
   , VkLineRasterizationModeEXT lineRasterizationMode
  );

  VkResult vk_enqueue_cmd_set_line_stipple_enable_ext(struct vk_cmd_queue *queue
   , VkBool32 stippledLineEnable
  );

  VkResult vk_enqueue_cmd_set_depth_clip_negative_one_to_one_ext(struct vk_cmd_queue *queue
   , VkBool32 negativeOneToOne
  );

  VkResult vk_enqueue_cmd_set_viewport_wscaling_enable_nv(struct vk_cmd_queue *queue
   , VkBool32 viewportWScalingEnable
  );

  VkResult vk_enqueue_cmd_set_viewport_swizzle_nv(struct vk_cmd_queue *queue
   , uint32_t firstViewport
   , uint32_t viewportCount
   , const VkViewportSwizzleNV* pViewportSwizzles
  );

  VkResult vk_enqueue_cmd_set_coverage_to_color_enable_nv(struct vk_cmd_queue *queue
   , VkBool32 coverageToColorEnable
  );

  VkResult vk_enqueue_cmd_set_coverage_to_color_location_nv(struct vk_cmd_queue *queue
   , uint32_t coverageToColorLocation
  );

  VkResult vk_enqueue_cmd_set_coverage_modulation_mode_nv(struct vk_cmd_queue *queue
   , VkCoverageModulationModeNV coverageModulationMode
  );

  VkResult vk_enqueue_cmd_set_coverage_modulation_table_enable_nv(struct vk_cmd_queue *queue
   , VkBool32 coverageModulationTableEnable
  );

  VkResult vk_enqueue_cmd_set_coverage_modulation_table_nv(struct vk_cmd_queue *queue
   , uint32_t coverageModulationTableCount
   , const float* pCoverageModulationTable
  );

  VkResult vk_enqueue_cmd_set_shading_rate_image_enable_nv(struct vk_cmd_queue *queue
   , VkBool32 shadingRateImageEnable
  );

  VkResult vk_enqueue_cmd_set_coverage_reduction_mode_nv(struct vk_cmd_queue *queue
   , VkCoverageReductionModeNV coverageReductionMode
  );

  VkResult vk_enqueue_cmd_set_representative_fragment_test_enable_nv(struct vk_cmd_queue *queue
   , VkBool32 representativeFragmentTestEnable
  );

  VkResult vk_enqueue_cmd_copy_buffer2(struct vk_cmd_queue *queue
   , const VkCopyBufferInfo2* pCopyBufferInfo
  );

  VkResult vk_enqueue_cmd_copy_image2(struct vk_cmd_queue *queue
   , const VkCopyImageInfo2* pCopyImageInfo
  );

  VkResult vk_enqueue_cmd_blit_image2(struct vk_cmd_queue *queue
   , const VkBlitImageInfo2* pBlitImageInfo
  );

  VkResult vk_enqueue_cmd_copy_buffer_to_image2(struct vk_cmd_queue *queue
   , const VkCopyBufferToImageInfo2* pCopyBufferToImageInfo
  );

  VkResult vk_enqueue_cmd_copy_image_to_buffer2(struct vk_cmd_queue *queue
   , const VkCopyImageToBufferInfo2* pCopyImageToBufferInfo
  );

  VkResult vk_enqueue_cmd_resolve_image2(struct vk_cmd_queue *queue
   , const VkResolveImageInfo2* pResolveImageInfo
  );

  VkResult vk_enqueue_cmd_set_fragment_shading_rate_khr(struct vk_cmd_queue *queue
   , const VkExtent2D*                           pFragmentSize
   , const VkFragmentShadingRateCombinerOpKHR    combinerOps[2]
  );

  VkResult vk_enqueue_cmd_set_fragment_shading_rate_enum_nv(struct vk_cmd_queue *queue
   , VkFragmentShadingRateNV                     shadingRate
   , const VkFragmentShadingRateCombinerOpKHR    combinerOps[2]
  );

  VkResult vk_enqueue_cmd_set_vertex_input_ext(struct vk_cmd_queue *queue
   , uint32_t vertexBindingDescriptionCount
   , const VkVertexInputBindingDescription2EXT* pVertexBindingDescriptions
   , uint32_t vertexAttributeDescriptionCount
   , const VkVertexInputAttributeDescription2EXT* pVertexAttributeDescriptions
  );

  VkResult vk_enqueue_cmd_set_color_write_enable_ext(struct vk_cmd_queue *queue
   , uint32_t                                attachmentCount
   , const VkBool32*   pColorWriteEnables
  );

  VkResult vk_enqueue_cmd_set_event2(struct vk_cmd_queue *queue
   , VkEvent                                             event
   , const VkDependencyInfo*                             pDependencyInfo
  );

  VkResult vk_enqueue_cmd_reset_event2(struct vk_cmd_queue *queue
   , VkEvent                                             event
   , VkPipelineStageFlags2               stageMask
  );

  VkResult vk_enqueue_cmd_wait_events2(struct vk_cmd_queue *queue
   , uint32_t                                            eventCount
   , const VkEvent*                     pEvents
   , const VkDependencyInfo*            pDependencyInfos
  );

  VkResult vk_enqueue_cmd_pipeline_barrier2(struct vk_cmd_queue *queue
   , const VkDependencyInfo*                             pDependencyInfo
  );

  VkResult vk_enqueue_cmd_write_timestamp2(struct vk_cmd_queue *queue
   , VkPipelineStageFlags2               stage
   , VkQueryPool                                         queryPool
   , uint32_t                                            query
  );

  VkResult vk_enqueue_cmd_write_buffer_marker2_amd(struct vk_cmd_queue *queue
   , VkPipelineStageFlags2               stage
   , VkBuffer                                            dstBuffer
   , VkDeviceSize                                        dstOffset
   , uint32_t                                            marker
  );

#ifdef VK_ENABLE_BETA_EXTENSIONS
  VkResult vk_enqueue_cmd_decode_video_khr(struct vk_cmd_queue *queue
   , const VkVideoDecodeInfoKHR* pDecodeInfo
  );
#endif // VK_ENABLE_BETA_EXTENSIONS

#ifdef VK_ENABLE_BETA_EXTENSIONS
  VkResult vk_enqueue_cmd_begin_video_coding_khr(struct vk_cmd_queue *queue
   , const VkVideoBeginCodingInfoKHR* pBeginInfo
  );
#endif // VK_ENABLE_BETA_EXTENSIONS

#ifdef VK_ENABLE_BETA_EXTENSIONS
  VkResult vk_enqueue_cmd_control_video_coding_khr(struct vk_cmd_queue *queue
   , const VkVideoCodingControlInfoKHR* pCodingControlInfo
  );
#endif // VK_ENABLE_BETA_EXTENSIONS

#ifdef VK_ENABLE_BETA_EXTENSIONS
  VkResult vk_enqueue_cmd_end_video_coding_khr(struct vk_cmd_queue *queue
   , const VkVideoEndCodingInfoKHR* pEndCodingInfo
  );
#endif // VK_ENABLE_BETA_EXTENSIONS

#ifdef VK_ENABLE_BETA_EXTENSIONS
  VkResult vk_enqueue_cmd_encode_video_khr(struct vk_cmd_queue *queue
   , const VkVideoEncodeInfoKHR* pEncodeInfo
  );
#endif // VK_ENABLE_BETA_EXTENSIONS

  VkResult vk_enqueue_cmd_cu_launch_kernel_nvx(struct vk_cmd_queue *queue
   , const VkCuLaunchInfoNVX* pLaunchInfo
  );

  VkResult vk_enqueue_cmd_begin_rendering(struct vk_cmd_queue *queue
   , const VkRenderingInfo*                              pRenderingInfo
  );

  VkResult vk_enqueue_cmd_end_rendering(struct vk_cmd_queue *queue
  );

  VkResult vk_enqueue_cmd_build_micromaps_ext(struct vk_cmd_queue *queue
   , uint32_t infoCount
   , const VkMicromapBuildInfoEXT* pInfos
  );

  VkResult vk_enqueue_cmd_copy_micromap_ext(struct vk_cmd_queue *queue
   , const VkCopyMicromapInfoEXT* pInfo
  );

  VkResult vk_enqueue_cmd_copy_micromap_to_memory_ext(struct vk_cmd_queue *queue
   , const VkCopyMicromapToMemoryInfoEXT* pInfo
  );

  VkResult vk_enqueue_cmd_copy_memory_to_micromap_ext(struct vk_cmd_queue *queue
   , const VkCopyMemoryToMicromapInfoEXT* pInfo
  );

  VkResult vk_enqueue_cmd_write_micromaps_properties_ext(struct vk_cmd_queue *queue
   , uint32_t micromapCount
   , const VkMicromapEXT* pMicromaps
   , VkQueryType queryType
   , VkQueryPool queryPool
   , uint32_t firstQuery
  );

  VkResult vk_enqueue_cmd_optical_flow_execute_nv(struct vk_cmd_queue *queue
   , VkOpticalFlowSessionNV session
   , const VkOpticalFlowExecuteInfoNV* pExecuteInfo
  );


void vk_free_queue(struct vk_cmd_queue *queue);

static inline void
vk_cmd_queue_init(struct vk_cmd_queue *queue, VkAllocationCallbacks *alloc)
{
   queue->alloc = alloc;
   list_inithead(&queue->cmds);
}

static inline void
vk_cmd_queue_reset(struct vk_cmd_queue *queue)
{
   vk_free_queue(queue);
   list_inithead(&queue->cmds);
}

static inline void
vk_cmd_queue_finish(struct vk_cmd_queue *queue)
{
   vk_free_queue(queue);
   list_inithead(&queue->cmds);
}

void vk_cmd_queue_execute(struct vk_cmd_queue *queue,
                          VkCommandBuffer commandBuffer,
                          const struct vk_device_dispatch_table *disp);

#ifdef __cplusplus
}
#endif
