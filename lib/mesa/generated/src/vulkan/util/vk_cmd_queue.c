
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

#include "vk_cmd_queue.h"

#define VK_PROTOTYPES
#include <vulkan/vulkan.h>

#include "vk_alloc.h"

const char *vk_cmd_queue_type_names[] = {
   "VK_CMD_BIND_PIPELINE",
   "VK_CMD_SET_VIEWPORT",
   "VK_CMD_SET_SCISSOR",
   "VK_CMD_SET_LINE_WIDTH",
   "VK_CMD_SET_DEPTH_BIAS",
   "VK_CMD_SET_BLEND_CONSTANTS",
   "VK_CMD_SET_DEPTH_BOUNDS",
   "VK_CMD_SET_STENCIL_COMPARE_MASK",
   "VK_CMD_SET_STENCIL_WRITE_MASK",
   "VK_CMD_SET_STENCIL_REFERENCE",
   "VK_CMD_BIND_DESCRIPTOR_SETS",
   "VK_CMD_BIND_INDEX_BUFFER",
   "VK_CMD_BIND_VERTEX_BUFFERS",
   "VK_CMD_DRAW",
   "VK_CMD_DRAW_INDEXED",
   "VK_CMD_DRAW_MULTI_EXT",
   "VK_CMD_DRAW_MULTI_INDEXED_EXT",
   "VK_CMD_DRAW_INDIRECT",
   "VK_CMD_DRAW_INDEXED_INDIRECT",
   "VK_CMD_DISPATCH",
   "VK_CMD_DISPATCH_INDIRECT",
   "VK_CMD_SUBPASS_SHADING_HUAWEI",
   "VK_CMD_COPY_BUFFER",
   "VK_CMD_COPY_IMAGE",
   "VK_CMD_BLIT_IMAGE",
   "VK_CMD_COPY_BUFFER_TO_IMAGE",
   "VK_CMD_COPY_IMAGE_TO_BUFFER",
   "VK_CMD_UPDATE_BUFFER",
   "VK_CMD_FILL_BUFFER",
   "VK_CMD_CLEAR_COLOR_IMAGE",
   "VK_CMD_CLEAR_DEPTH_STENCIL_IMAGE",
   "VK_CMD_CLEAR_ATTACHMENTS",
   "VK_CMD_RESOLVE_IMAGE",
   "VK_CMD_SET_EVENT",
   "VK_CMD_RESET_EVENT",
   "VK_CMD_WAIT_EVENTS",
   "VK_CMD_PIPELINE_BARRIER",
   "VK_CMD_BEGIN_QUERY",
   "VK_CMD_END_QUERY",
   "VK_CMD_BEGIN_CONDITIONAL_RENDERING_EXT",
   "VK_CMD_END_CONDITIONAL_RENDERING_EXT",
   "VK_CMD_RESET_QUERY_POOL",
   "VK_CMD_WRITE_TIMESTAMP",
   "VK_CMD_COPY_QUERY_POOL_RESULTS",
   "VK_CMD_PUSH_CONSTANTS",
   "VK_CMD_BEGIN_RENDER_PASS",
   "VK_CMD_NEXT_SUBPASS",
   "VK_CMD_END_RENDER_PASS",
   "VK_CMD_EXECUTE_COMMANDS",
   "VK_CMD_DEBUG_MARKER_BEGIN_EXT",
   "VK_CMD_DEBUG_MARKER_END_EXT",
   "VK_CMD_DEBUG_MARKER_INSERT_EXT",
   "VK_CMD_EXECUTE_GENERATED_COMMANDS_NV",
   "VK_CMD_PREPROCESS_GENERATED_COMMANDS_NV",
   "VK_CMD_BIND_PIPELINE_SHADER_GROUP_NV",
   "VK_CMD_PUSH_DESCRIPTOR_SET_KHR",
   "VK_CMD_SET_DEVICE_MASK",
   "VK_CMD_DISPATCH_BASE",
   "VK_CMD_PUSH_DESCRIPTOR_SET_WITH_TEMPLATE_KHR",
   "VK_CMD_SET_VIEWPORT_WSCALING_NV",
   "VK_CMD_SET_DISCARD_RECTANGLE_EXT",
   "VK_CMD_SET_SAMPLE_LOCATIONS_EXT",
   "VK_CMD_BEGIN_DEBUG_UTILS_LABEL_EXT",
   "VK_CMD_END_DEBUG_UTILS_LABEL_EXT",
   "VK_CMD_INSERT_DEBUG_UTILS_LABEL_EXT",
   "VK_CMD_WRITE_BUFFER_MARKER_AMD",
   "VK_CMD_BEGIN_RENDER_PASS2",
   "VK_CMD_NEXT_SUBPASS2",
   "VK_CMD_END_RENDER_PASS2",
   "VK_CMD_DRAW_INDIRECT_COUNT",
   "VK_CMD_DRAW_INDEXED_INDIRECT_COUNT",
   "VK_CMD_SET_CHECKPOINT_NV",
   "VK_CMD_BIND_TRANSFORM_FEEDBACK_BUFFERS_EXT",
   "VK_CMD_BEGIN_TRANSFORM_FEEDBACK_EXT",
   "VK_CMD_END_TRANSFORM_FEEDBACK_EXT",
   "VK_CMD_BEGIN_QUERY_INDEXED_EXT",
   "VK_CMD_END_QUERY_INDEXED_EXT",
   "VK_CMD_DRAW_INDIRECT_BYTE_COUNT_EXT",
   "VK_CMD_SET_EXCLUSIVE_SCISSOR_NV",
   "VK_CMD_BIND_SHADING_RATE_IMAGE_NV",
   "VK_CMD_SET_VIEWPORT_SHADING_RATE_PALETTE_NV",
   "VK_CMD_SET_COARSE_SAMPLE_ORDER_NV",
   "VK_CMD_DRAW_MESH_TASKS_NV",
   "VK_CMD_DRAW_MESH_TASKS_INDIRECT_NV",
   "VK_CMD_DRAW_MESH_TASKS_INDIRECT_COUNT_NV",
   "VK_CMD_BIND_INVOCATION_MASK_HUAWEI",
   "VK_CMD_COPY_ACCELERATION_STRUCTURE_NV",
   "VK_CMD_COPY_ACCELERATION_STRUCTURE_KHR",
   "VK_CMD_COPY_ACCELERATION_STRUCTURE_TO_MEMORY_KHR",
   "VK_CMD_COPY_MEMORY_TO_ACCELERATION_STRUCTURE_KHR",
   "VK_CMD_WRITE_ACCELERATION_STRUCTURES_PROPERTIES_KHR",
   "VK_CMD_WRITE_ACCELERATION_STRUCTURES_PROPERTIES_NV",
   "VK_CMD_BUILD_ACCELERATION_STRUCTURE_NV",
   "VK_CMD_TRACE_RAYS_KHR",
   "VK_CMD_TRACE_RAYS_NV",
   "VK_CMD_TRACE_RAYS_INDIRECT_KHR",
   "VK_CMD_SET_RAY_TRACING_PIPELINE_STACK_SIZE_KHR",
   "VK_CMD_SET_PERFORMANCE_MARKER_INTEL",
   "VK_CMD_SET_PERFORMANCE_STREAM_MARKER_INTEL",
   "VK_CMD_SET_PERFORMANCE_OVERRIDE_INTEL",
   "VK_CMD_SET_LINE_STIPPLE_EXT",
   "VK_CMD_BUILD_ACCELERATION_STRUCTURES_KHR",
   "VK_CMD_BUILD_ACCELERATION_STRUCTURES_INDIRECT_KHR",
   "VK_CMD_SET_CULL_MODE_EXT",
   "VK_CMD_SET_FRONT_FACE_EXT",
   "VK_CMD_SET_PRIMITIVE_TOPOLOGY_EXT",
   "VK_CMD_SET_VIEWPORT_WITH_COUNT_EXT",
   "VK_CMD_SET_SCISSOR_WITH_COUNT_EXT",
   "VK_CMD_BIND_VERTEX_BUFFERS2_EXT",
   "VK_CMD_SET_DEPTH_TEST_ENABLE_EXT",
   "VK_CMD_SET_DEPTH_WRITE_ENABLE_EXT",
   "VK_CMD_SET_DEPTH_COMPARE_OP_EXT",
   "VK_CMD_SET_DEPTH_BOUNDS_TEST_ENABLE_EXT",
   "VK_CMD_SET_STENCIL_TEST_ENABLE_EXT",
   "VK_CMD_SET_STENCIL_OP_EXT",
   "VK_CMD_SET_PATCH_CONTROL_POINTS_EXT",
   "VK_CMD_SET_RASTERIZER_DISCARD_ENABLE_EXT",
   "VK_CMD_SET_DEPTH_BIAS_ENABLE_EXT",
   "VK_CMD_SET_LOGIC_OP_EXT",
   "VK_CMD_SET_PRIMITIVE_RESTART_ENABLE_EXT",
   "VK_CMD_COPY_BUFFER2_KHR",
   "VK_CMD_COPY_IMAGE2_KHR",
   "VK_CMD_BLIT_IMAGE2_KHR",
   "VK_CMD_COPY_BUFFER_TO_IMAGE2_KHR",
   "VK_CMD_COPY_IMAGE_TO_BUFFER2_KHR",
   "VK_CMD_RESOLVE_IMAGE2_KHR",
   "VK_CMD_SET_FRAGMENT_SHADING_RATE_KHR",
   "VK_CMD_SET_FRAGMENT_SHADING_RATE_ENUM_NV",
   "VK_CMD_SET_VERTEX_INPUT_EXT",
   "VK_CMD_SET_COLOR_WRITE_ENABLE_EXT",
   "VK_CMD_SET_EVENT2_KHR",
   "VK_CMD_RESET_EVENT2_KHR",
   "VK_CMD_WAIT_EVENTS2_KHR",
   "VK_CMD_PIPELINE_BARRIER2_KHR",
   "VK_CMD_WRITE_TIMESTAMP2_KHR",
   "VK_CMD_WRITE_BUFFER_MARKER2_AMD",
#ifdef VK_ENABLE_BETA_EXTENSIONS
   "VK_CMD_DECODE_VIDEO_KHR",
#endif // VK_ENABLE_BETA_EXTENSIONS
#ifdef VK_ENABLE_BETA_EXTENSIONS
   "VK_CMD_BEGIN_VIDEO_CODING_KHR",
#endif // VK_ENABLE_BETA_EXTENSIONS
#ifdef VK_ENABLE_BETA_EXTENSIONS
   "VK_CMD_CONTROL_VIDEO_CODING_KHR",
#endif // VK_ENABLE_BETA_EXTENSIONS
#ifdef VK_ENABLE_BETA_EXTENSIONS
   "VK_CMD_END_VIDEO_CODING_KHR",
#endif // VK_ENABLE_BETA_EXTENSIONS
#ifdef VK_ENABLE_BETA_EXTENSIONS
   "VK_CMD_ENCODE_VIDEO_KHR",
#endif // VK_ENABLE_BETA_EXTENSIONS
   "VK_CMD_CU_LAUNCH_KERNEL_NVX",
};

void vk_enqueue_cmd_bind_pipeline(struct vk_cmd_queue *queue
, VkPipelineBindPoint pipelineBindPoint
, VkPipeline pipeline
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_BIND_PIPELINE;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   cmd->u.bind_pipeline.pipeline_bind_point = pipelineBindPoint;
   cmd->u.bind_pipeline.pipeline = pipeline;
}

void vk_enqueue_cmd_set_viewport(struct vk_cmd_queue *queue
, uint32_t firstViewport
, uint32_t viewportCount
, const VkViewport* pViewports
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_SET_VIEWPORT;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   cmd->u.set_viewport.first_viewport = firstViewport;
   cmd->u.set_viewport.viewport_count = viewportCount;
   if (pViewports) {
      cmd->u.set_viewport.viewports = vk_zalloc(queue->alloc, sizeof(*cmd->u.set_viewport.viewports) * viewportCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   memcpy(( VkViewport* )cmd->u.set_viewport.viewports, pViewports, sizeof(*cmd->u.set_viewport.viewports) * viewportCount);
   }
}

void vk_enqueue_cmd_set_scissor(struct vk_cmd_queue *queue
, uint32_t firstScissor
, uint32_t scissorCount
, const VkRect2D* pScissors
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_SET_SCISSOR;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   cmd->u.set_scissor.first_scissor = firstScissor;
   cmd->u.set_scissor.scissor_count = scissorCount;
   if (pScissors) {
      cmd->u.set_scissor.scissors = vk_zalloc(queue->alloc, sizeof(*cmd->u.set_scissor.scissors) * scissorCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   memcpy(( VkRect2D* )cmd->u.set_scissor.scissors, pScissors, sizeof(*cmd->u.set_scissor.scissors) * scissorCount);
   }
}

void vk_enqueue_cmd_set_line_width(struct vk_cmd_queue *queue
, float lineWidth
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_SET_LINE_WIDTH;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   cmd->u.set_line_width.line_width = lineWidth;
}

void vk_enqueue_cmd_set_depth_bias(struct vk_cmd_queue *queue
, float depthBiasConstantFactor
, float depthBiasClamp
, float depthBiasSlopeFactor
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_SET_DEPTH_BIAS;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   cmd->u.set_depth_bias.depth_bias_constant_factor = depthBiasConstantFactor;
   cmd->u.set_depth_bias.depth_bias_clamp = depthBiasClamp;
   cmd->u.set_depth_bias.depth_bias_slope_factor = depthBiasSlopeFactor;
}

void vk_enqueue_cmd_set_blend_constants(struct vk_cmd_queue *queue
, const float blendConstants[4]
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_SET_BLEND_CONSTANTS;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   memcpy(cmd->u.set_blend_constants.blend_constants, blendConstants,
          sizeof(*blendConstants) * 4);
}

void vk_enqueue_cmd_set_depth_bounds(struct vk_cmd_queue *queue
, float minDepthBounds
, float maxDepthBounds
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_SET_DEPTH_BOUNDS;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   cmd->u.set_depth_bounds.min_depth_bounds = minDepthBounds;
   cmd->u.set_depth_bounds.max_depth_bounds = maxDepthBounds;
}

void vk_enqueue_cmd_set_stencil_compare_mask(struct vk_cmd_queue *queue
, VkStencilFaceFlags faceMask
, uint32_t compareMask
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_SET_STENCIL_COMPARE_MASK;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   cmd->u.set_stencil_compare_mask.face_mask = faceMask;
   cmd->u.set_stencil_compare_mask.compare_mask = compareMask;
}

void vk_enqueue_cmd_set_stencil_write_mask(struct vk_cmd_queue *queue
, VkStencilFaceFlags faceMask
, uint32_t writeMask
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_SET_STENCIL_WRITE_MASK;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   cmd->u.set_stencil_write_mask.face_mask = faceMask;
   cmd->u.set_stencil_write_mask.write_mask = writeMask;
}

void vk_enqueue_cmd_set_stencil_reference(struct vk_cmd_queue *queue
, VkStencilFaceFlags faceMask
, uint32_t reference
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_SET_STENCIL_REFERENCE;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   cmd->u.set_stencil_reference.face_mask = faceMask;
   cmd->u.set_stencil_reference.reference = reference;
}

void vk_enqueue_cmd_bind_index_buffer(struct vk_cmd_queue *queue
, VkBuffer buffer
, VkDeviceSize offset
, VkIndexType indexType
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_BIND_INDEX_BUFFER;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   cmd->u.bind_index_buffer.buffer = buffer;
   cmd->u.bind_index_buffer.offset = offset;
   cmd->u.bind_index_buffer.index_type = indexType;
}

void vk_enqueue_cmd_bind_vertex_buffers(struct vk_cmd_queue *queue
, uint32_t firstBinding
, uint32_t bindingCount
, const VkBuffer* pBuffers
, const VkDeviceSize* pOffsets
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_BIND_VERTEX_BUFFERS;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   cmd->u.bind_vertex_buffers.first_binding = firstBinding;
   cmd->u.bind_vertex_buffers.binding_count = bindingCount;
   if (pBuffers) {
      cmd->u.bind_vertex_buffers.buffers = vk_zalloc(queue->alloc, sizeof(*cmd->u.bind_vertex_buffers.buffers) * bindingCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   memcpy(( VkBuffer* )cmd->u.bind_vertex_buffers.buffers, pBuffers, sizeof(*cmd->u.bind_vertex_buffers.buffers) * bindingCount);
   }
   if (pOffsets) {
      cmd->u.bind_vertex_buffers.offsets = vk_zalloc(queue->alloc, sizeof(*cmd->u.bind_vertex_buffers.offsets) * bindingCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   memcpy(( VkDeviceSize* )cmd->u.bind_vertex_buffers.offsets, pOffsets, sizeof(*cmd->u.bind_vertex_buffers.offsets) * bindingCount);
   }
}

void vk_enqueue_cmd_draw(struct vk_cmd_queue *queue
, uint32_t vertexCount
, uint32_t instanceCount
, uint32_t firstVertex
, uint32_t firstInstance
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_DRAW;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   cmd->u.draw.vertex_count = vertexCount;
   cmd->u.draw.instance_count = instanceCount;
   cmd->u.draw.first_vertex = firstVertex;
   cmd->u.draw.first_instance = firstInstance;
}

void vk_enqueue_cmd_draw_indexed(struct vk_cmd_queue *queue
, uint32_t indexCount
, uint32_t instanceCount
, uint32_t firstIndex
, int32_t vertexOffset
, uint32_t firstInstance
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_DRAW_INDEXED;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   cmd->u.draw_indexed.index_count = indexCount;
   cmd->u.draw_indexed.instance_count = instanceCount;
   cmd->u.draw_indexed.first_index = firstIndex;
   cmd->u.draw_indexed.vertex_offset = vertexOffset;
   cmd->u.draw_indexed.first_instance = firstInstance;
}

void vk_enqueue_cmd_draw_indirect(struct vk_cmd_queue *queue
, VkBuffer buffer
, VkDeviceSize offset
, uint32_t drawCount
, uint32_t stride
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_DRAW_INDIRECT;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   cmd->u.draw_indirect.buffer = buffer;
   cmd->u.draw_indirect.offset = offset;
   cmd->u.draw_indirect.draw_count = drawCount;
   cmd->u.draw_indirect.stride = stride;
}

void vk_enqueue_cmd_draw_indexed_indirect(struct vk_cmd_queue *queue
, VkBuffer buffer
, VkDeviceSize offset
, uint32_t drawCount
, uint32_t stride
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_DRAW_INDEXED_INDIRECT;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   cmd->u.draw_indexed_indirect.buffer = buffer;
   cmd->u.draw_indexed_indirect.offset = offset;
   cmd->u.draw_indexed_indirect.draw_count = drawCount;
   cmd->u.draw_indexed_indirect.stride = stride;
}

void vk_enqueue_cmd_dispatch(struct vk_cmd_queue *queue
, uint32_t groupCountX
, uint32_t groupCountY
, uint32_t groupCountZ
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_DISPATCH;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   cmd->u.dispatch.group_count_x = groupCountX;
   cmd->u.dispatch.group_count_y = groupCountY;
   cmd->u.dispatch.group_count_z = groupCountZ;
}

void vk_enqueue_cmd_dispatch_indirect(struct vk_cmd_queue *queue
, VkBuffer buffer
, VkDeviceSize offset
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_DISPATCH_INDIRECT;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   cmd->u.dispatch_indirect.buffer = buffer;
   cmd->u.dispatch_indirect.offset = offset;
}

void vk_enqueue_cmd_subpass_shading_huawei(struct vk_cmd_queue *queue
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_SUBPASS_SHADING_HUAWEI;
   list_addtail(&cmd->cmd_link, &queue->cmds);

}

void vk_enqueue_cmd_copy_buffer(struct vk_cmd_queue *queue
, VkBuffer srcBuffer
, VkBuffer dstBuffer
, uint32_t regionCount
, const VkBufferCopy* pRegions
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_COPY_BUFFER;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   cmd->u.copy_buffer.src_buffer = srcBuffer;
   cmd->u.copy_buffer.dst_buffer = dstBuffer;
   cmd->u.copy_buffer.region_count = regionCount;
   if (pRegions) {
      cmd->u.copy_buffer.regions = vk_zalloc(queue->alloc, sizeof(*cmd->u.copy_buffer.regions) * regionCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   memcpy(( VkBufferCopy* )cmd->u.copy_buffer.regions, pRegions, sizeof(*cmd->u.copy_buffer.regions) * regionCount);
   }
}

void vk_enqueue_cmd_copy_image(struct vk_cmd_queue *queue
, VkImage srcImage
, VkImageLayout srcImageLayout
, VkImage dstImage
, VkImageLayout dstImageLayout
, uint32_t regionCount
, const VkImageCopy* pRegions
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_COPY_IMAGE;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   cmd->u.copy_image.src_image = srcImage;
   cmd->u.copy_image.src_image_layout = srcImageLayout;
   cmd->u.copy_image.dst_image = dstImage;
   cmd->u.copy_image.dst_image_layout = dstImageLayout;
   cmd->u.copy_image.region_count = regionCount;
   if (pRegions) {
      cmd->u.copy_image.regions = vk_zalloc(queue->alloc, sizeof(*cmd->u.copy_image.regions) * regionCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   memcpy(( VkImageCopy* )cmd->u.copy_image.regions, pRegions, sizeof(*cmd->u.copy_image.regions) * regionCount);
   }
}

void vk_enqueue_cmd_blit_image(struct vk_cmd_queue *queue
, VkImage srcImage
, VkImageLayout srcImageLayout
, VkImage dstImage
, VkImageLayout dstImageLayout
, uint32_t regionCount
, const VkImageBlit* pRegions
, VkFilter filter
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_BLIT_IMAGE;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   cmd->u.blit_image.src_image = srcImage;
   cmd->u.blit_image.src_image_layout = srcImageLayout;
   cmd->u.blit_image.dst_image = dstImage;
   cmd->u.blit_image.dst_image_layout = dstImageLayout;
   cmd->u.blit_image.region_count = regionCount;
   if (pRegions) {
      cmd->u.blit_image.regions = vk_zalloc(queue->alloc, sizeof(*cmd->u.blit_image.regions) * regionCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   memcpy(( VkImageBlit* )cmd->u.blit_image.regions, pRegions, sizeof(*cmd->u.blit_image.regions) * regionCount);
   }
   cmd->u.blit_image.filter = filter;
}

void vk_enqueue_cmd_copy_buffer_to_image(struct vk_cmd_queue *queue
, VkBuffer srcBuffer
, VkImage dstImage
, VkImageLayout dstImageLayout
, uint32_t regionCount
, const VkBufferImageCopy* pRegions
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_COPY_BUFFER_TO_IMAGE;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   cmd->u.copy_buffer_to_image.src_buffer = srcBuffer;
   cmd->u.copy_buffer_to_image.dst_image = dstImage;
   cmd->u.copy_buffer_to_image.dst_image_layout = dstImageLayout;
   cmd->u.copy_buffer_to_image.region_count = regionCount;
   if (pRegions) {
      cmd->u.copy_buffer_to_image.regions = vk_zalloc(queue->alloc, sizeof(*cmd->u.copy_buffer_to_image.regions) * regionCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   memcpy(( VkBufferImageCopy* )cmd->u.copy_buffer_to_image.regions, pRegions, sizeof(*cmd->u.copy_buffer_to_image.regions) * regionCount);
   }
}

void vk_enqueue_cmd_copy_image_to_buffer(struct vk_cmd_queue *queue
, VkImage srcImage
, VkImageLayout srcImageLayout
, VkBuffer dstBuffer
, uint32_t regionCount
, const VkBufferImageCopy* pRegions
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_COPY_IMAGE_TO_BUFFER;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   cmd->u.copy_image_to_buffer.src_image = srcImage;
   cmd->u.copy_image_to_buffer.src_image_layout = srcImageLayout;
   cmd->u.copy_image_to_buffer.dst_buffer = dstBuffer;
   cmd->u.copy_image_to_buffer.region_count = regionCount;
   if (pRegions) {
      cmd->u.copy_image_to_buffer.regions = vk_zalloc(queue->alloc, sizeof(*cmd->u.copy_image_to_buffer.regions) * regionCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   memcpy(( VkBufferImageCopy* )cmd->u.copy_image_to_buffer.regions, pRegions, sizeof(*cmd->u.copy_image_to_buffer.regions) * regionCount);
   }
}

void vk_enqueue_cmd_update_buffer(struct vk_cmd_queue *queue
, VkBuffer dstBuffer
, VkDeviceSize dstOffset
, VkDeviceSize dataSize
, const void* pData
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_UPDATE_BUFFER;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   cmd->u.update_buffer.dst_buffer = dstBuffer;
   cmd->u.update_buffer.dst_offset = dstOffset;
   cmd->u.update_buffer.data_size = dataSize;
   if (pData) {
      cmd->u.update_buffer.data = vk_zalloc(queue->alloc, 1 * dataSize, 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   memcpy(( void* )cmd->u.update_buffer.data, pData, 1 * dataSize);
   }
}

void vk_enqueue_cmd_fill_buffer(struct vk_cmd_queue *queue
, VkBuffer dstBuffer
, VkDeviceSize dstOffset
, VkDeviceSize size
, uint32_t data
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_FILL_BUFFER;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   cmd->u.fill_buffer.dst_buffer = dstBuffer;
   cmd->u.fill_buffer.dst_offset = dstOffset;
   cmd->u.fill_buffer.size = size;
   cmd->u.fill_buffer.data = data;
}

void vk_enqueue_cmd_clear_color_image(struct vk_cmd_queue *queue
, VkImage image
, VkImageLayout imageLayout
, const VkClearColorValue* pColor
, uint32_t rangeCount
, const VkImageSubresourceRange* pRanges
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_CLEAR_COLOR_IMAGE;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   cmd->u.clear_color_image.image = image;
   cmd->u.clear_color_image.image_layout = imageLayout;
   if (pColor) {
      cmd->u.clear_color_image.color = vk_zalloc(queue->alloc, sizeof(VkClearColorValue), 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
      memcpy((void*)cmd->u.clear_color_image.color, pColor, sizeof(VkClearColorValue));
   VkClearColorValue *tmp_dst1 = (void *) cmd->u.clear_color_image.color; (void) tmp_dst1;
   VkClearColorValue *tmp_src1 = (void *) pColor; (void) tmp_src1;   
      } else {
      cmd->u.clear_color_image.color = NULL;
   }
   cmd->u.clear_color_image.range_count = rangeCount;
   if (pRanges) {
      cmd->u.clear_color_image.ranges = vk_zalloc(queue->alloc, sizeof(*cmd->u.clear_color_image.ranges) * rangeCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   memcpy(( VkImageSubresourceRange* )cmd->u.clear_color_image.ranges, pRanges, sizeof(*cmd->u.clear_color_image.ranges) * rangeCount);
   }
}

void vk_enqueue_cmd_clear_depth_stencil_image(struct vk_cmd_queue *queue
, VkImage image
, VkImageLayout imageLayout
, const VkClearDepthStencilValue* pDepthStencil
, uint32_t rangeCount
, const VkImageSubresourceRange* pRanges
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_CLEAR_DEPTH_STENCIL_IMAGE;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   cmd->u.clear_depth_stencil_image.image = image;
   cmd->u.clear_depth_stencil_image.image_layout = imageLayout;
   if (pDepthStencil) {
      cmd->u.clear_depth_stencil_image.depth_stencil = vk_zalloc(queue->alloc, sizeof(VkClearDepthStencilValue), 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
      memcpy((void*)cmd->u.clear_depth_stencil_image.depth_stencil, pDepthStencil, sizeof(VkClearDepthStencilValue));
   VkClearDepthStencilValue *tmp_dst1 = (void *) cmd->u.clear_depth_stencil_image.depth_stencil; (void) tmp_dst1;
   VkClearDepthStencilValue *tmp_src1 = (void *) pDepthStencil; (void) tmp_src1;   
      } else {
      cmd->u.clear_depth_stencil_image.depth_stencil = NULL;
   }
   cmd->u.clear_depth_stencil_image.range_count = rangeCount;
   if (pRanges) {
      cmd->u.clear_depth_stencil_image.ranges = vk_zalloc(queue->alloc, sizeof(*cmd->u.clear_depth_stencil_image.ranges) * rangeCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   memcpy(( VkImageSubresourceRange* )cmd->u.clear_depth_stencil_image.ranges, pRanges, sizeof(*cmd->u.clear_depth_stencil_image.ranges) * rangeCount);
   }
}

void vk_enqueue_cmd_clear_attachments(struct vk_cmd_queue *queue
, uint32_t attachmentCount
, const VkClearAttachment* pAttachments
, uint32_t rectCount
, const VkClearRect* pRects
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_CLEAR_ATTACHMENTS;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   cmd->u.clear_attachments.attachment_count = attachmentCount;
   if (pAttachments) {
      cmd->u.clear_attachments.attachments = vk_zalloc(queue->alloc, sizeof(*cmd->u.clear_attachments.attachments) * attachmentCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   memcpy(( VkClearAttachment* )cmd->u.clear_attachments.attachments, pAttachments, sizeof(*cmd->u.clear_attachments.attachments) * attachmentCount);
   }
   cmd->u.clear_attachments.rect_count = rectCount;
   if (pRects) {
      cmd->u.clear_attachments.rects = vk_zalloc(queue->alloc, sizeof(*cmd->u.clear_attachments.rects) * rectCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   memcpy(( VkClearRect* )cmd->u.clear_attachments.rects, pRects, sizeof(*cmd->u.clear_attachments.rects) * rectCount);
   }
}

void vk_enqueue_cmd_resolve_image(struct vk_cmd_queue *queue
, VkImage srcImage
, VkImageLayout srcImageLayout
, VkImage dstImage
, VkImageLayout dstImageLayout
, uint32_t regionCount
, const VkImageResolve* pRegions
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_RESOLVE_IMAGE;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   cmd->u.resolve_image.src_image = srcImage;
   cmd->u.resolve_image.src_image_layout = srcImageLayout;
   cmd->u.resolve_image.dst_image = dstImage;
   cmd->u.resolve_image.dst_image_layout = dstImageLayout;
   cmd->u.resolve_image.region_count = regionCount;
   if (pRegions) {
      cmd->u.resolve_image.regions = vk_zalloc(queue->alloc, sizeof(*cmd->u.resolve_image.regions) * regionCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   memcpy(( VkImageResolve* )cmd->u.resolve_image.regions, pRegions, sizeof(*cmd->u.resolve_image.regions) * regionCount);
   }
}

void vk_enqueue_cmd_set_event(struct vk_cmd_queue *queue
, VkEvent event
, VkPipelineStageFlags stageMask
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_SET_EVENT;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   cmd->u.set_event.event = event;
   cmd->u.set_event.stage_mask = stageMask;
}

void vk_enqueue_cmd_reset_event(struct vk_cmd_queue *queue
, VkEvent event
, VkPipelineStageFlags stageMask
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_RESET_EVENT;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   cmd->u.reset_event.event = event;
   cmd->u.reset_event.stage_mask = stageMask;
}

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
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_WAIT_EVENTS;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   cmd->u.wait_events.event_count = eventCount;
   if (pEvents) {
      cmd->u.wait_events.events = vk_zalloc(queue->alloc, sizeof(*cmd->u.wait_events.events) * eventCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   memcpy(( VkEvent* )cmd->u.wait_events.events, pEvents, sizeof(*cmd->u.wait_events.events) * eventCount);
   }
   cmd->u.wait_events.src_stage_mask = srcStageMask;
   cmd->u.wait_events.dst_stage_mask = dstStageMask;
   cmd->u.wait_events.memory_barrier_count = memoryBarrierCount;
   if (pMemoryBarriers) {
      cmd->u.wait_events.memory_barriers = vk_zalloc(queue->alloc, sizeof(*cmd->u.wait_events.memory_barriers) * memoryBarrierCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   memcpy(( VkMemoryBarrier* )cmd->u.wait_events.memory_barriers, pMemoryBarriers, sizeof(*cmd->u.wait_events.memory_barriers) * memoryBarrierCount);
   }
   cmd->u.wait_events.buffer_memory_barrier_count = bufferMemoryBarrierCount;
   if (pBufferMemoryBarriers) {
      cmd->u.wait_events.buffer_memory_barriers = vk_zalloc(queue->alloc, sizeof(*cmd->u.wait_events.buffer_memory_barriers) * bufferMemoryBarrierCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   memcpy(( VkBufferMemoryBarrier* )cmd->u.wait_events.buffer_memory_barriers, pBufferMemoryBarriers, sizeof(*cmd->u.wait_events.buffer_memory_barriers) * bufferMemoryBarrierCount);
   }
   cmd->u.wait_events.image_memory_barrier_count = imageMemoryBarrierCount;
   if (pImageMemoryBarriers) {
      cmd->u.wait_events.image_memory_barriers = vk_zalloc(queue->alloc, sizeof(*cmd->u.wait_events.image_memory_barriers) * imageMemoryBarrierCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   memcpy(( VkImageMemoryBarrier* )cmd->u.wait_events.image_memory_barriers, pImageMemoryBarriers, sizeof(*cmd->u.wait_events.image_memory_barriers) * imageMemoryBarrierCount);
   }
}

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
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_PIPELINE_BARRIER;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   cmd->u.pipeline_barrier.src_stage_mask = srcStageMask;
   cmd->u.pipeline_barrier.dst_stage_mask = dstStageMask;
   cmd->u.pipeline_barrier.dependency_flags = dependencyFlags;
   cmd->u.pipeline_barrier.memory_barrier_count = memoryBarrierCount;
   if (pMemoryBarriers) {
      cmd->u.pipeline_barrier.memory_barriers = vk_zalloc(queue->alloc, sizeof(*cmd->u.pipeline_barrier.memory_barriers) * memoryBarrierCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   memcpy(( VkMemoryBarrier* )cmd->u.pipeline_barrier.memory_barriers, pMemoryBarriers, sizeof(*cmd->u.pipeline_barrier.memory_barriers) * memoryBarrierCount);
   }
   cmd->u.pipeline_barrier.buffer_memory_barrier_count = bufferMemoryBarrierCount;
   if (pBufferMemoryBarriers) {
      cmd->u.pipeline_barrier.buffer_memory_barriers = vk_zalloc(queue->alloc, sizeof(*cmd->u.pipeline_barrier.buffer_memory_barriers) * bufferMemoryBarrierCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   memcpy(( VkBufferMemoryBarrier* )cmd->u.pipeline_barrier.buffer_memory_barriers, pBufferMemoryBarriers, sizeof(*cmd->u.pipeline_barrier.buffer_memory_barriers) * bufferMemoryBarrierCount);
   }
   cmd->u.pipeline_barrier.image_memory_barrier_count = imageMemoryBarrierCount;
   if (pImageMemoryBarriers) {
      cmd->u.pipeline_barrier.image_memory_barriers = vk_zalloc(queue->alloc, sizeof(*cmd->u.pipeline_barrier.image_memory_barriers) * imageMemoryBarrierCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   memcpy(( VkImageMemoryBarrier* )cmd->u.pipeline_barrier.image_memory_barriers, pImageMemoryBarriers, sizeof(*cmd->u.pipeline_barrier.image_memory_barriers) * imageMemoryBarrierCount);
   }
}

void vk_enqueue_cmd_begin_query(struct vk_cmd_queue *queue
, VkQueryPool queryPool
, uint32_t query
, VkQueryControlFlags flags
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_BEGIN_QUERY;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   cmd->u.begin_query.query_pool = queryPool;
   cmd->u.begin_query.query = query;
   cmd->u.begin_query.flags = flags;
}

void vk_enqueue_cmd_end_query(struct vk_cmd_queue *queue
, VkQueryPool queryPool
, uint32_t query
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_END_QUERY;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   cmd->u.end_query.query_pool = queryPool;
   cmd->u.end_query.query = query;
}

void vk_enqueue_cmd_begin_conditional_rendering_ext(struct vk_cmd_queue *queue
, const VkConditionalRenderingBeginInfoEXT* pConditionalRenderingBegin
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_BEGIN_CONDITIONAL_RENDERING_EXT;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   if (pConditionalRenderingBegin) {
      cmd->u.begin_conditional_rendering_ext.conditional_rendering_begin = vk_zalloc(queue->alloc, sizeof(VkConditionalRenderingBeginInfoEXT), 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
      memcpy((void*)cmd->u.begin_conditional_rendering_ext.conditional_rendering_begin, pConditionalRenderingBegin, sizeof(VkConditionalRenderingBeginInfoEXT));
   VkConditionalRenderingBeginInfoEXT *tmp_dst1 = (void *) cmd->u.begin_conditional_rendering_ext.conditional_rendering_begin; (void) tmp_dst1;
   VkConditionalRenderingBeginInfoEXT *tmp_src1 = (void *) pConditionalRenderingBegin; (void) tmp_src1;   
      } else {
      cmd->u.begin_conditional_rendering_ext.conditional_rendering_begin = NULL;
   }
}

void vk_enqueue_cmd_end_conditional_rendering_ext(struct vk_cmd_queue *queue
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_END_CONDITIONAL_RENDERING_EXT;
   list_addtail(&cmd->cmd_link, &queue->cmds);

}

void vk_enqueue_cmd_reset_query_pool(struct vk_cmd_queue *queue
, VkQueryPool queryPool
, uint32_t firstQuery
, uint32_t queryCount
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_RESET_QUERY_POOL;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   cmd->u.reset_query_pool.query_pool = queryPool;
   cmd->u.reset_query_pool.first_query = firstQuery;
   cmd->u.reset_query_pool.query_count = queryCount;
}

void vk_enqueue_cmd_write_timestamp(struct vk_cmd_queue *queue
, VkPipelineStageFlagBits pipelineStage
, VkQueryPool queryPool
, uint32_t query
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_WRITE_TIMESTAMP;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   cmd->u.write_timestamp.pipeline_stage = pipelineStage;
   cmd->u.write_timestamp.query_pool = queryPool;
   cmd->u.write_timestamp.query = query;
}

void vk_enqueue_cmd_copy_query_pool_results(struct vk_cmd_queue *queue
, VkQueryPool queryPool
, uint32_t firstQuery
, uint32_t queryCount
, VkBuffer dstBuffer
, VkDeviceSize dstOffset
, VkDeviceSize stride
, VkQueryResultFlags flags
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_COPY_QUERY_POOL_RESULTS;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   cmd->u.copy_query_pool_results.query_pool = queryPool;
   cmd->u.copy_query_pool_results.first_query = firstQuery;
   cmd->u.copy_query_pool_results.query_count = queryCount;
   cmd->u.copy_query_pool_results.dst_buffer = dstBuffer;
   cmd->u.copy_query_pool_results.dst_offset = dstOffset;
   cmd->u.copy_query_pool_results.stride = stride;
   cmd->u.copy_query_pool_results.flags = flags;
}

void vk_enqueue_cmd_push_constants(struct vk_cmd_queue *queue
, VkPipelineLayout layout
, VkShaderStageFlags stageFlags
, uint32_t offset
, uint32_t size
, const void* pValues
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_PUSH_CONSTANTS;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   cmd->u.push_constants.layout = layout;
   cmd->u.push_constants.stage_flags = stageFlags;
   cmd->u.push_constants.offset = offset;
   cmd->u.push_constants.size = size;
   if (pValues) {
      cmd->u.push_constants.values = vk_zalloc(queue->alloc, 1 * size, 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   memcpy(( void* )cmd->u.push_constants.values, pValues, 1 * size);
   }
}

void vk_enqueue_cmd_begin_render_pass(struct vk_cmd_queue *queue
, const VkRenderPassBeginInfo* pRenderPassBegin
, VkSubpassContents contents
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_BEGIN_RENDER_PASS;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   if (pRenderPassBegin) {
      cmd->u.begin_render_pass.render_pass_begin = vk_zalloc(queue->alloc, sizeof(VkRenderPassBeginInfo), 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
      memcpy((void*)cmd->u.begin_render_pass.render_pass_begin, pRenderPassBegin, sizeof(VkRenderPassBeginInfo));
   VkRenderPassBeginInfo *tmp_dst1 = (void *) cmd->u.begin_render_pass.render_pass_begin; (void) tmp_dst1;
   VkRenderPassBeginInfo *tmp_src1 = (void *) pRenderPassBegin; (void) tmp_src1;   
   
      const VkBaseInStructure *pnext = tmp_dst1->pNext;
      if (pnext) {
         switch ((int32_t)pnext->sType) {
         
      case VK_STRUCTURE_TYPE_DEVICE_GROUP_RENDER_PASS_BEGIN_INFO:
         if (pnext) {
      tmp_dst1->pNext = vk_zalloc(queue->alloc, sizeof(VkDeviceGroupRenderPassBeginInfo), 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
      memcpy((void*)tmp_dst1->pNext, pnext, sizeof(VkDeviceGroupRenderPassBeginInfo));
   VkDeviceGroupRenderPassBeginInfo *tmp_dst2 = (void *) tmp_dst1->pNext; (void) tmp_dst2;
   VkDeviceGroupRenderPassBeginInfo *tmp_src2 = (void *) pnext; (void) tmp_src2;   
   tmp_dst2->pDeviceRenderAreas = vk_zalloc(queue->alloc, sizeof(*tmp_dst2->pDeviceRenderAreas) * tmp_dst2->deviceRenderAreaCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   memcpy(( VkRect2D*  )tmp_dst2->pDeviceRenderAreas, tmp_src2->pDeviceRenderAreas, sizeof(*tmp_dst2->pDeviceRenderAreas) * tmp_dst2->deviceRenderAreaCount);
   } else {
      tmp_dst1->pNext = NULL;
   }
         break;
      
      case VK_STRUCTURE_TYPE_RENDER_PASS_SAMPLE_LOCATIONS_BEGIN_INFO_EXT:
         if (pnext) {
      tmp_dst1->pNext = vk_zalloc(queue->alloc, sizeof(VkRenderPassSampleLocationsBeginInfoEXT), 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
      memcpy((void*)tmp_dst1->pNext, pnext, sizeof(VkRenderPassSampleLocationsBeginInfoEXT));
   VkRenderPassSampleLocationsBeginInfoEXT *tmp_dst2 = (void *) tmp_dst1->pNext; (void) tmp_dst2;
   VkRenderPassSampleLocationsBeginInfoEXT *tmp_src2 = (void *) pnext; (void) tmp_src2;   
   tmp_dst2->pAttachmentInitialSampleLocations = vk_zalloc(queue->alloc, sizeof(*tmp_dst2->pAttachmentInitialSampleLocations) * tmp_dst2->attachmentInitialSampleLocationsCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   memcpy(( VkAttachmentSampleLocationsEXT* )tmp_dst2->pAttachmentInitialSampleLocations, tmp_src2->pAttachmentInitialSampleLocations, sizeof(*tmp_dst2->pAttachmentInitialSampleLocations) * tmp_dst2->attachmentInitialSampleLocationsCount);
tmp_dst2->pPostSubpassSampleLocations = vk_zalloc(queue->alloc, sizeof(*tmp_dst2->pPostSubpassSampleLocations) * tmp_dst2->postSubpassSampleLocationsCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   memcpy(( VkSubpassSampleLocationsEXT* )tmp_dst2->pPostSubpassSampleLocations, tmp_src2->pPostSubpassSampleLocations, sizeof(*tmp_dst2->pPostSubpassSampleLocations) * tmp_dst2->postSubpassSampleLocationsCount);
   } else {
      tmp_dst1->pNext = NULL;
   }
         break;
      
      case VK_STRUCTURE_TYPE_RENDER_PASS_ATTACHMENT_BEGIN_INFO:
         if (pnext) {
      tmp_dst1->pNext = vk_zalloc(queue->alloc, sizeof(VkRenderPassAttachmentBeginInfo), 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
      memcpy((void*)tmp_dst1->pNext, pnext, sizeof(VkRenderPassAttachmentBeginInfo));
   VkRenderPassAttachmentBeginInfo *tmp_dst2 = (void *) tmp_dst1->pNext; (void) tmp_dst2;
   VkRenderPassAttachmentBeginInfo *tmp_src2 = (void *) pnext; (void) tmp_src2;   
   tmp_dst2->pAttachments = vk_zalloc(queue->alloc, sizeof(*tmp_dst2->pAttachments) * tmp_dst2->attachmentCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   memcpy(( VkImageView* )tmp_dst2->pAttachments, tmp_src2->pAttachments, sizeof(*tmp_dst2->pAttachments) * tmp_dst2->attachmentCount);
   } else {
      tmp_dst1->pNext = NULL;
   }
         break;
      
      case VK_STRUCTURE_TYPE_RENDER_PASS_TRANSFORM_BEGIN_INFO_QCOM:
         if (pnext) {
      tmp_dst1->pNext = vk_zalloc(queue->alloc, sizeof(VkRenderPassTransformBeginInfoQCOM), 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
      memcpy((void*)tmp_dst1->pNext, pnext, sizeof(VkRenderPassTransformBeginInfoQCOM));
   VkRenderPassTransformBeginInfoQCOM *tmp_dst2 = (void *) tmp_dst1->pNext; (void) tmp_dst2;
   VkRenderPassTransformBeginInfoQCOM *tmp_src2 = (void *) pnext; (void) tmp_src2;   
      } else {
      tmp_dst1->pNext = NULL;
   }
         break;
      
         }
      }
      tmp_dst1->pClearValues = vk_zalloc(queue->alloc, sizeof(*tmp_dst1->pClearValues) * tmp_dst1->clearValueCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   memcpy(( VkClearValue*    )tmp_dst1->pClearValues, tmp_src1->pClearValues, sizeof(*tmp_dst1->pClearValues) * tmp_dst1->clearValueCount);
   } else {
      cmd->u.begin_render_pass.render_pass_begin = NULL;
   }
   cmd->u.begin_render_pass.contents = contents;
}

void vk_enqueue_cmd_next_subpass(struct vk_cmd_queue *queue
, VkSubpassContents contents
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_NEXT_SUBPASS;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   cmd->u.next_subpass.contents = contents;
}

void vk_enqueue_cmd_end_render_pass(struct vk_cmd_queue *queue
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_END_RENDER_PASS;
   list_addtail(&cmd->cmd_link, &queue->cmds);

}

void vk_enqueue_cmd_execute_commands(struct vk_cmd_queue *queue
, uint32_t commandBufferCount
, const VkCommandBuffer* pCommandBuffers
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_EXECUTE_COMMANDS;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   cmd->u.execute_commands.command_buffer_count = commandBufferCount;
   if (pCommandBuffers) {
      cmd->u.execute_commands.command_buffers = vk_zalloc(queue->alloc, sizeof(*cmd->u.execute_commands.command_buffers) * commandBufferCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   memcpy(( VkCommandBuffer* )cmd->u.execute_commands.command_buffers, pCommandBuffers, sizeof(*cmd->u.execute_commands.command_buffers) * commandBufferCount);
   }
}

void vk_enqueue_cmd_debug_marker_begin_ext(struct vk_cmd_queue *queue
, const VkDebugMarkerMarkerInfoEXT* pMarkerInfo
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_DEBUG_MARKER_BEGIN_EXT;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   if (pMarkerInfo) {
      cmd->u.debug_marker_begin_ext.marker_info = vk_zalloc(queue->alloc, sizeof(VkDebugMarkerMarkerInfoEXT), 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
      memcpy((void*)cmd->u.debug_marker_begin_ext.marker_info, pMarkerInfo, sizeof(VkDebugMarkerMarkerInfoEXT));
   VkDebugMarkerMarkerInfoEXT *tmp_dst1 = (void *) cmd->u.debug_marker_begin_ext.marker_info; (void) tmp_dst1;
   VkDebugMarkerMarkerInfoEXT *tmp_src1 = (void *) pMarkerInfo; (void) tmp_src1;   
      } else {
      cmd->u.debug_marker_begin_ext.marker_info = NULL;
   }
}

void vk_enqueue_cmd_debug_marker_end_ext(struct vk_cmd_queue *queue
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_DEBUG_MARKER_END_EXT;
   list_addtail(&cmd->cmd_link, &queue->cmds);

}

void vk_enqueue_cmd_debug_marker_insert_ext(struct vk_cmd_queue *queue
, const VkDebugMarkerMarkerInfoEXT* pMarkerInfo
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_DEBUG_MARKER_INSERT_EXT;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   if (pMarkerInfo) {
      cmd->u.debug_marker_insert_ext.marker_info = vk_zalloc(queue->alloc, sizeof(VkDebugMarkerMarkerInfoEXT), 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
      memcpy((void*)cmd->u.debug_marker_insert_ext.marker_info, pMarkerInfo, sizeof(VkDebugMarkerMarkerInfoEXT));
   VkDebugMarkerMarkerInfoEXT *tmp_dst1 = (void *) cmd->u.debug_marker_insert_ext.marker_info; (void) tmp_dst1;
   VkDebugMarkerMarkerInfoEXT *tmp_src1 = (void *) pMarkerInfo; (void) tmp_src1;   
      } else {
      cmd->u.debug_marker_insert_ext.marker_info = NULL;
   }
}

void vk_enqueue_cmd_execute_generated_commands_nv(struct vk_cmd_queue *queue
, VkBool32 isPreprocessed
, const VkGeneratedCommandsInfoNV* pGeneratedCommandsInfo
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_EXECUTE_GENERATED_COMMANDS_NV;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   cmd->u.execute_generated_commands_nv.is_preprocessed = isPreprocessed;
   if (pGeneratedCommandsInfo) {
      cmd->u.execute_generated_commands_nv.generated_commands_info = vk_zalloc(queue->alloc, sizeof(VkGeneratedCommandsInfoNV), 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
      memcpy((void*)cmd->u.execute_generated_commands_nv.generated_commands_info, pGeneratedCommandsInfo, sizeof(VkGeneratedCommandsInfoNV));
   VkGeneratedCommandsInfoNV *tmp_dst1 = (void *) cmd->u.execute_generated_commands_nv.generated_commands_info; (void) tmp_dst1;
   VkGeneratedCommandsInfoNV *tmp_src1 = (void *) pGeneratedCommandsInfo; (void) tmp_src1;   
   tmp_dst1->pStreams = vk_zalloc(queue->alloc, sizeof(*tmp_dst1->pStreams) * tmp_dst1->streamCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   memcpy(( VkIndirectCommandsStreamNV*  )tmp_dst1->pStreams, tmp_src1->pStreams, sizeof(*tmp_dst1->pStreams) * tmp_dst1->streamCount);
   } else {
      cmd->u.execute_generated_commands_nv.generated_commands_info = NULL;
   }
}

void vk_enqueue_cmd_preprocess_generated_commands_nv(struct vk_cmd_queue *queue
, const VkGeneratedCommandsInfoNV* pGeneratedCommandsInfo
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_PREPROCESS_GENERATED_COMMANDS_NV;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   if (pGeneratedCommandsInfo) {
      cmd->u.preprocess_generated_commands_nv.generated_commands_info = vk_zalloc(queue->alloc, sizeof(VkGeneratedCommandsInfoNV), 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
      memcpy((void*)cmd->u.preprocess_generated_commands_nv.generated_commands_info, pGeneratedCommandsInfo, sizeof(VkGeneratedCommandsInfoNV));
   VkGeneratedCommandsInfoNV *tmp_dst1 = (void *) cmd->u.preprocess_generated_commands_nv.generated_commands_info; (void) tmp_dst1;
   VkGeneratedCommandsInfoNV *tmp_src1 = (void *) pGeneratedCommandsInfo; (void) tmp_src1;   
   tmp_dst1->pStreams = vk_zalloc(queue->alloc, sizeof(*tmp_dst1->pStreams) * tmp_dst1->streamCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   memcpy(( VkIndirectCommandsStreamNV*  )tmp_dst1->pStreams, tmp_src1->pStreams, sizeof(*tmp_dst1->pStreams) * tmp_dst1->streamCount);
   } else {
      cmd->u.preprocess_generated_commands_nv.generated_commands_info = NULL;
   }
}

void vk_enqueue_cmd_bind_pipeline_shader_group_nv(struct vk_cmd_queue *queue
, VkPipelineBindPoint pipelineBindPoint
, VkPipeline pipeline
, uint32_t groupIndex
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_BIND_PIPELINE_SHADER_GROUP_NV;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   cmd->u.bind_pipeline_shader_group_nv.pipeline_bind_point = pipelineBindPoint;
   cmd->u.bind_pipeline_shader_group_nv.pipeline = pipeline;
   cmd->u.bind_pipeline_shader_group_nv.group_index = groupIndex;
}

void vk_enqueue_cmd_set_device_mask(struct vk_cmd_queue *queue
, uint32_t deviceMask
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_SET_DEVICE_MASK;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   cmd->u.set_device_mask.device_mask = deviceMask;
}

void vk_enqueue_cmd_dispatch_base(struct vk_cmd_queue *queue
, uint32_t baseGroupX
, uint32_t baseGroupY
, uint32_t baseGroupZ
, uint32_t groupCountX
, uint32_t groupCountY
, uint32_t groupCountZ
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_DISPATCH_BASE;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   cmd->u.dispatch_base.base_group_x = baseGroupX;
   cmd->u.dispatch_base.base_group_y = baseGroupY;
   cmd->u.dispatch_base.base_group_z = baseGroupZ;
   cmd->u.dispatch_base.group_count_x = groupCountX;
   cmd->u.dispatch_base.group_count_y = groupCountY;
   cmd->u.dispatch_base.group_count_z = groupCountZ;
}

void vk_enqueue_cmd_set_viewport_wscaling_nv(struct vk_cmd_queue *queue
, uint32_t firstViewport
, uint32_t viewportCount
, const VkViewportWScalingNV* pViewportWScalings
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_SET_VIEWPORT_WSCALING_NV;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   cmd->u.set_viewport_wscaling_nv.first_viewport = firstViewport;
   cmd->u.set_viewport_wscaling_nv.viewport_count = viewportCount;
   if (pViewportWScalings) {
      cmd->u.set_viewport_wscaling_nv.viewport_wscalings = vk_zalloc(queue->alloc, sizeof(*cmd->u.set_viewport_wscaling_nv.viewport_wscalings) * viewportCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   memcpy(( VkViewportWScalingNV* )cmd->u.set_viewport_wscaling_nv.viewport_wscalings, pViewportWScalings, sizeof(*cmd->u.set_viewport_wscaling_nv.viewport_wscalings) * viewportCount);
   }
}

void vk_enqueue_cmd_set_discard_rectangle_ext(struct vk_cmd_queue *queue
, uint32_t firstDiscardRectangle
, uint32_t discardRectangleCount
, const VkRect2D* pDiscardRectangles
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_SET_DISCARD_RECTANGLE_EXT;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   cmd->u.set_discard_rectangle_ext.first_discard_rectangle = firstDiscardRectangle;
   cmd->u.set_discard_rectangle_ext.discard_rectangle_count = discardRectangleCount;
   if (pDiscardRectangles) {
      cmd->u.set_discard_rectangle_ext.discard_rectangles = vk_zalloc(queue->alloc, sizeof(*cmd->u.set_discard_rectangle_ext.discard_rectangles) * discardRectangleCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   memcpy(( VkRect2D* )cmd->u.set_discard_rectangle_ext.discard_rectangles, pDiscardRectangles, sizeof(*cmd->u.set_discard_rectangle_ext.discard_rectangles) * discardRectangleCount);
   }
}

void vk_enqueue_cmd_set_sample_locations_ext(struct vk_cmd_queue *queue
, const VkSampleLocationsInfoEXT* pSampleLocationsInfo
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_SET_SAMPLE_LOCATIONS_EXT;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   if (pSampleLocationsInfo) {
      cmd->u.set_sample_locations_ext.sample_locations_info = vk_zalloc(queue->alloc, sizeof(VkSampleLocationsInfoEXT), 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
      memcpy((void*)cmd->u.set_sample_locations_ext.sample_locations_info, pSampleLocationsInfo, sizeof(VkSampleLocationsInfoEXT));
   VkSampleLocationsInfoEXT *tmp_dst1 = (void *) cmd->u.set_sample_locations_ext.sample_locations_info; (void) tmp_dst1;
   VkSampleLocationsInfoEXT *tmp_src1 = (void *) pSampleLocationsInfo; (void) tmp_src1;   
   tmp_dst1->pSampleLocations = vk_zalloc(queue->alloc, sizeof(*tmp_dst1->pSampleLocations) * tmp_dst1->sampleLocationsCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   memcpy(( VkSampleLocationEXT* )tmp_dst1->pSampleLocations, tmp_src1->pSampleLocations, sizeof(*tmp_dst1->pSampleLocations) * tmp_dst1->sampleLocationsCount);
   } else {
      cmd->u.set_sample_locations_ext.sample_locations_info = NULL;
   }
}

void vk_enqueue_cmd_begin_debug_utils_label_ext(struct vk_cmd_queue *queue
, const VkDebugUtilsLabelEXT* pLabelInfo
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_BEGIN_DEBUG_UTILS_LABEL_EXT;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   if (pLabelInfo) {
      cmd->u.begin_debug_utils_label_ext.label_info = vk_zalloc(queue->alloc, sizeof(VkDebugUtilsLabelEXT), 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
      memcpy((void*)cmd->u.begin_debug_utils_label_ext.label_info, pLabelInfo, sizeof(VkDebugUtilsLabelEXT));
   VkDebugUtilsLabelEXT *tmp_dst1 = (void *) cmd->u.begin_debug_utils_label_ext.label_info; (void) tmp_dst1;
   VkDebugUtilsLabelEXT *tmp_src1 = (void *) pLabelInfo; (void) tmp_src1;   
      } else {
      cmd->u.begin_debug_utils_label_ext.label_info = NULL;
   }
}

void vk_enqueue_cmd_end_debug_utils_label_ext(struct vk_cmd_queue *queue
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_END_DEBUG_UTILS_LABEL_EXT;
   list_addtail(&cmd->cmd_link, &queue->cmds);

}

void vk_enqueue_cmd_insert_debug_utils_label_ext(struct vk_cmd_queue *queue
, const VkDebugUtilsLabelEXT* pLabelInfo
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_INSERT_DEBUG_UTILS_LABEL_EXT;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   if (pLabelInfo) {
      cmd->u.insert_debug_utils_label_ext.label_info = vk_zalloc(queue->alloc, sizeof(VkDebugUtilsLabelEXT), 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
      memcpy((void*)cmd->u.insert_debug_utils_label_ext.label_info, pLabelInfo, sizeof(VkDebugUtilsLabelEXT));
   VkDebugUtilsLabelEXT *tmp_dst1 = (void *) cmd->u.insert_debug_utils_label_ext.label_info; (void) tmp_dst1;
   VkDebugUtilsLabelEXT *tmp_src1 = (void *) pLabelInfo; (void) tmp_src1;   
      } else {
      cmd->u.insert_debug_utils_label_ext.label_info = NULL;
   }
}

void vk_enqueue_cmd_write_buffer_marker_amd(struct vk_cmd_queue *queue
, VkPipelineStageFlagBits pipelineStage
, VkBuffer dstBuffer
, VkDeviceSize dstOffset
, uint32_t marker
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_WRITE_BUFFER_MARKER_AMD;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   cmd->u.write_buffer_marker_amd.pipeline_stage = pipelineStage;
   cmd->u.write_buffer_marker_amd.dst_buffer = dstBuffer;
   cmd->u.write_buffer_marker_amd.dst_offset = dstOffset;
   cmd->u.write_buffer_marker_amd.marker = marker;
}

void vk_enqueue_cmd_begin_render_pass2(struct vk_cmd_queue *queue
, const VkRenderPassBeginInfo*      pRenderPassBegin
, const VkSubpassBeginInfo*      pSubpassBeginInfo
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_BEGIN_RENDER_PASS2;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   if (pRenderPassBegin) {
      cmd->u.begin_render_pass2.render_pass_begin = vk_zalloc(queue->alloc, sizeof(VkRenderPassBeginInfo), 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
      memcpy((void*)cmd->u.begin_render_pass2.render_pass_begin, pRenderPassBegin, sizeof(VkRenderPassBeginInfo));
   VkRenderPassBeginInfo *tmp_dst1 = (void *) cmd->u.begin_render_pass2.render_pass_begin; (void) tmp_dst1;
   VkRenderPassBeginInfo *tmp_src1 = (void *) pRenderPassBegin; (void) tmp_src1;   
   
      const VkBaseInStructure *pnext = tmp_dst1->pNext;
      if (pnext) {
         switch ((int32_t)pnext->sType) {
         
      case VK_STRUCTURE_TYPE_DEVICE_GROUP_RENDER_PASS_BEGIN_INFO:
         if (pnext) {
      tmp_dst1->pNext = vk_zalloc(queue->alloc, sizeof(VkDeviceGroupRenderPassBeginInfo), 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
      memcpy((void*)tmp_dst1->pNext, pnext, sizeof(VkDeviceGroupRenderPassBeginInfo));
   VkDeviceGroupRenderPassBeginInfo *tmp_dst2 = (void *) tmp_dst1->pNext; (void) tmp_dst2;
   VkDeviceGroupRenderPassBeginInfo *tmp_src2 = (void *) pnext; (void) tmp_src2;   
   tmp_dst2->pDeviceRenderAreas = vk_zalloc(queue->alloc, sizeof(*tmp_dst2->pDeviceRenderAreas) * tmp_dst2->deviceRenderAreaCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   memcpy(( VkRect2D*  )tmp_dst2->pDeviceRenderAreas, tmp_src2->pDeviceRenderAreas, sizeof(*tmp_dst2->pDeviceRenderAreas) * tmp_dst2->deviceRenderAreaCount);
   } else {
      tmp_dst1->pNext = NULL;
   }
         break;
      
      case VK_STRUCTURE_TYPE_RENDER_PASS_SAMPLE_LOCATIONS_BEGIN_INFO_EXT:
         if (pnext) {
      tmp_dst1->pNext = vk_zalloc(queue->alloc, sizeof(VkRenderPassSampleLocationsBeginInfoEXT), 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
      memcpy((void*)tmp_dst1->pNext, pnext, sizeof(VkRenderPassSampleLocationsBeginInfoEXT));
   VkRenderPassSampleLocationsBeginInfoEXT *tmp_dst2 = (void *) tmp_dst1->pNext; (void) tmp_dst2;
   VkRenderPassSampleLocationsBeginInfoEXT *tmp_src2 = (void *) pnext; (void) tmp_src2;   
   tmp_dst2->pAttachmentInitialSampleLocations = vk_zalloc(queue->alloc, sizeof(*tmp_dst2->pAttachmentInitialSampleLocations) * tmp_dst2->attachmentInitialSampleLocationsCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   memcpy(( VkAttachmentSampleLocationsEXT* )tmp_dst2->pAttachmentInitialSampleLocations, tmp_src2->pAttachmentInitialSampleLocations, sizeof(*tmp_dst2->pAttachmentInitialSampleLocations) * tmp_dst2->attachmentInitialSampleLocationsCount);
tmp_dst2->pPostSubpassSampleLocations = vk_zalloc(queue->alloc, sizeof(*tmp_dst2->pPostSubpassSampleLocations) * tmp_dst2->postSubpassSampleLocationsCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   memcpy(( VkSubpassSampleLocationsEXT* )tmp_dst2->pPostSubpassSampleLocations, tmp_src2->pPostSubpassSampleLocations, sizeof(*tmp_dst2->pPostSubpassSampleLocations) * tmp_dst2->postSubpassSampleLocationsCount);
   } else {
      tmp_dst1->pNext = NULL;
   }
         break;
      
      case VK_STRUCTURE_TYPE_RENDER_PASS_ATTACHMENT_BEGIN_INFO:
         if (pnext) {
      tmp_dst1->pNext = vk_zalloc(queue->alloc, sizeof(VkRenderPassAttachmentBeginInfo), 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
      memcpy((void*)tmp_dst1->pNext, pnext, sizeof(VkRenderPassAttachmentBeginInfo));
   VkRenderPassAttachmentBeginInfo *tmp_dst2 = (void *) tmp_dst1->pNext; (void) tmp_dst2;
   VkRenderPassAttachmentBeginInfo *tmp_src2 = (void *) pnext; (void) tmp_src2;   
   tmp_dst2->pAttachments = vk_zalloc(queue->alloc, sizeof(*tmp_dst2->pAttachments) * tmp_dst2->attachmentCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   memcpy(( VkImageView* )tmp_dst2->pAttachments, tmp_src2->pAttachments, sizeof(*tmp_dst2->pAttachments) * tmp_dst2->attachmentCount);
   } else {
      tmp_dst1->pNext = NULL;
   }
         break;
      
      case VK_STRUCTURE_TYPE_RENDER_PASS_TRANSFORM_BEGIN_INFO_QCOM:
         if (pnext) {
      tmp_dst1->pNext = vk_zalloc(queue->alloc, sizeof(VkRenderPassTransformBeginInfoQCOM), 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
      memcpy((void*)tmp_dst1->pNext, pnext, sizeof(VkRenderPassTransformBeginInfoQCOM));
   VkRenderPassTransformBeginInfoQCOM *tmp_dst2 = (void *) tmp_dst1->pNext; (void) tmp_dst2;
   VkRenderPassTransformBeginInfoQCOM *tmp_src2 = (void *) pnext; (void) tmp_src2;   
      } else {
      tmp_dst1->pNext = NULL;
   }
         break;
      
         }
      }
      tmp_dst1->pClearValues = vk_zalloc(queue->alloc, sizeof(*tmp_dst1->pClearValues) * tmp_dst1->clearValueCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   memcpy(( VkClearValue*    )tmp_dst1->pClearValues, tmp_src1->pClearValues, sizeof(*tmp_dst1->pClearValues) * tmp_dst1->clearValueCount);
   } else {
      cmd->u.begin_render_pass2.render_pass_begin = NULL;
   }
   if (pSubpassBeginInfo) {
      cmd->u.begin_render_pass2.subpass_begin_info = vk_zalloc(queue->alloc, sizeof(VkSubpassBeginInfo), 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
      memcpy((void*)cmd->u.begin_render_pass2.subpass_begin_info, pSubpassBeginInfo, sizeof(VkSubpassBeginInfo));
   VkSubpassBeginInfo *tmp_dst1 = (void *) cmd->u.begin_render_pass2.subpass_begin_info; (void) tmp_dst1;
   VkSubpassBeginInfo *tmp_src1 = (void *) pSubpassBeginInfo; (void) tmp_src1;   
      } else {
      cmd->u.begin_render_pass2.subpass_begin_info = NULL;
   }
}

void vk_enqueue_cmd_next_subpass2(struct vk_cmd_queue *queue
, const VkSubpassBeginInfo*      pSubpassBeginInfo
, const VkSubpassEndInfo*        pSubpassEndInfo
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_NEXT_SUBPASS2;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   if (pSubpassBeginInfo) {
      cmd->u.next_subpass2.subpass_begin_info = vk_zalloc(queue->alloc, sizeof(VkSubpassBeginInfo), 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
      memcpy((void*)cmd->u.next_subpass2.subpass_begin_info, pSubpassBeginInfo, sizeof(VkSubpassBeginInfo));
   VkSubpassBeginInfo *tmp_dst1 = (void *) cmd->u.next_subpass2.subpass_begin_info; (void) tmp_dst1;
   VkSubpassBeginInfo *tmp_src1 = (void *) pSubpassBeginInfo; (void) tmp_src1;   
      } else {
      cmd->u.next_subpass2.subpass_begin_info = NULL;
   }
   if (pSubpassEndInfo) {
      cmd->u.next_subpass2.subpass_end_info = vk_zalloc(queue->alloc, sizeof(VkSubpassEndInfo), 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
      memcpy((void*)cmd->u.next_subpass2.subpass_end_info, pSubpassEndInfo, sizeof(VkSubpassEndInfo));
   VkSubpassEndInfo *tmp_dst1 = (void *) cmd->u.next_subpass2.subpass_end_info; (void) tmp_dst1;
   VkSubpassEndInfo *tmp_src1 = (void *) pSubpassEndInfo; (void) tmp_src1;   
      } else {
      cmd->u.next_subpass2.subpass_end_info = NULL;
   }
}

void vk_enqueue_cmd_end_render_pass2(struct vk_cmd_queue *queue
, const VkSubpassEndInfo*        pSubpassEndInfo
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_END_RENDER_PASS2;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   if (pSubpassEndInfo) {
      cmd->u.end_render_pass2.subpass_end_info = vk_zalloc(queue->alloc, sizeof(VkSubpassEndInfo), 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
      memcpy((void*)cmd->u.end_render_pass2.subpass_end_info, pSubpassEndInfo, sizeof(VkSubpassEndInfo));
   VkSubpassEndInfo *tmp_dst1 = (void *) cmd->u.end_render_pass2.subpass_end_info; (void) tmp_dst1;
   VkSubpassEndInfo *tmp_src1 = (void *) pSubpassEndInfo; (void) tmp_src1;   
      } else {
      cmd->u.end_render_pass2.subpass_end_info = NULL;
   }
}

void vk_enqueue_cmd_draw_indirect_count(struct vk_cmd_queue *queue
, VkBuffer buffer
, VkDeviceSize offset
, VkBuffer countBuffer
, VkDeviceSize countBufferOffset
, uint32_t maxDrawCount
, uint32_t stride
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_DRAW_INDIRECT_COUNT;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   cmd->u.draw_indirect_count.buffer = buffer;
   cmd->u.draw_indirect_count.offset = offset;
   cmd->u.draw_indirect_count.count_buffer = countBuffer;
   cmd->u.draw_indirect_count.count_buffer_offset = countBufferOffset;
   cmd->u.draw_indirect_count.max_draw_count = maxDrawCount;
   cmd->u.draw_indirect_count.stride = stride;
}

void vk_enqueue_cmd_draw_indexed_indirect_count(struct vk_cmd_queue *queue
, VkBuffer buffer
, VkDeviceSize offset
, VkBuffer countBuffer
, VkDeviceSize countBufferOffset
, uint32_t maxDrawCount
, uint32_t stride
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_DRAW_INDEXED_INDIRECT_COUNT;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   cmd->u.draw_indexed_indirect_count.buffer = buffer;
   cmd->u.draw_indexed_indirect_count.offset = offset;
   cmd->u.draw_indexed_indirect_count.count_buffer = countBuffer;
   cmd->u.draw_indexed_indirect_count.count_buffer_offset = countBufferOffset;
   cmd->u.draw_indexed_indirect_count.max_draw_count = maxDrawCount;
   cmd->u.draw_indexed_indirect_count.stride = stride;
}

void vk_enqueue_cmd_set_checkpoint_nv(struct vk_cmd_queue *queue
, const void* pCheckpointMarker
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_SET_CHECKPOINT_NV;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   cmd->u.set_checkpoint_nv.checkpoint_marker = ( void* ) pCheckpointMarker;
}

void vk_enqueue_cmd_bind_transform_feedback_buffers_ext(struct vk_cmd_queue *queue
, uint32_t firstBinding
, uint32_t bindingCount
, const VkBuffer* pBuffers
, const VkDeviceSize* pOffsets
, const VkDeviceSize* pSizes
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_BIND_TRANSFORM_FEEDBACK_BUFFERS_EXT;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   cmd->u.bind_transform_feedback_buffers_ext.first_binding = firstBinding;
   cmd->u.bind_transform_feedback_buffers_ext.binding_count = bindingCount;
   if (pBuffers) {
      cmd->u.bind_transform_feedback_buffers_ext.buffers = vk_zalloc(queue->alloc, sizeof(*cmd->u.bind_transform_feedback_buffers_ext.buffers) * bindingCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   memcpy(( VkBuffer* )cmd->u.bind_transform_feedback_buffers_ext.buffers, pBuffers, sizeof(*cmd->u.bind_transform_feedback_buffers_ext.buffers) * bindingCount);
   }
   if (pOffsets) {
      cmd->u.bind_transform_feedback_buffers_ext.offsets = vk_zalloc(queue->alloc, sizeof(*cmd->u.bind_transform_feedback_buffers_ext.offsets) * bindingCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   memcpy(( VkDeviceSize* )cmd->u.bind_transform_feedback_buffers_ext.offsets, pOffsets, sizeof(*cmd->u.bind_transform_feedback_buffers_ext.offsets) * bindingCount);
   }
   if (pSizes) {
      cmd->u.bind_transform_feedback_buffers_ext.sizes = vk_zalloc(queue->alloc, sizeof(*cmd->u.bind_transform_feedback_buffers_ext.sizes) * bindingCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   memcpy(( VkDeviceSize* )cmd->u.bind_transform_feedback_buffers_ext.sizes, pSizes, sizeof(*cmd->u.bind_transform_feedback_buffers_ext.sizes) * bindingCount);
   }
}

void vk_enqueue_cmd_begin_transform_feedback_ext(struct vk_cmd_queue *queue
, uint32_t firstCounterBuffer
, uint32_t counterBufferCount
, const VkBuffer* pCounterBuffers
, const VkDeviceSize* pCounterBufferOffsets
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_BEGIN_TRANSFORM_FEEDBACK_EXT;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   cmd->u.begin_transform_feedback_ext.first_counter_buffer = firstCounterBuffer;
   cmd->u.begin_transform_feedback_ext.counter_buffer_count = counterBufferCount;
   if (pCounterBuffers) {
      cmd->u.begin_transform_feedback_ext.counter_buffers = vk_zalloc(queue->alloc, sizeof(*cmd->u.begin_transform_feedback_ext.counter_buffers) * counterBufferCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   memcpy(( VkBuffer* )cmd->u.begin_transform_feedback_ext.counter_buffers, pCounterBuffers, sizeof(*cmd->u.begin_transform_feedback_ext.counter_buffers) * counterBufferCount);
   }
   if (pCounterBufferOffsets) {
      cmd->u.begin_transform_feedback_ext.counter_buffer_offsets = vk_zalloc(queue->alloc, sizeof(*cmd->u.begin_transform_feedback_ext.counter_buffer_offsets) * counterBufferCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   memcpy(( VkDeviceSize* )cmd->u.begin_transform_feedback_ext.counter_buffer_offsets, pCounterBufferOffsets, sizeof(*cmd->u.begin_transform_feedback_ext.counter_buffer_offsets) * counterBufferCount);
   }
}

void vk_enqueue_cmd_end_transform_feedback_ext(struct vk_cmd_queue *queue
, uint32_t firstCounterBuffer
, uint32_t counterBufferCount
, const VkBuffer* pCounterBuffers
, const VkDeviceSize* pCounterBufferOffsets
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_END_TRANSFORM_FEEDBACK_EXT;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   cmd->u.end_transform_feedback_ext.first_counter_buffer = firstCounterBuffer;
   cmd->u.end_transform_feedback_ext.counter_buffer_count = counterBufferCount;
   if (pCounterBuffers) {
      cmd->u.end_transform_feedback_ext.counter_buffers = vk_zalloc(queue->alloc, sizeof(*cmd->u.end_transform_feedback_ext.counter_buffers) * counterBufferCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   memcpy(( VkBuffer* )cmd->u.end_transform_feedback_ext.counter_buffers, pCounterBuffers, sizeof(*cmd->u.end_transform_feedback_ext.counter_buffers) * counterBufferCount);
   }
   if (pCounterBufferOffsets) {
      cmd->u.end_transform_feedback_ext.counter_buffer_offsets = vk_zalloc(queue->alloc, sizeof(*cmd->u.end_transform_feedback_ext.counter_buffer_offsets) * counterBufferCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   memcpy(( VkDeviceSize* )cmd->u.end_transform_feedback_ext.counter_buffer_offsets, pCounterBufferOffsets, sizeof(*cmd->u.end_transform_feedback_ext.counter_buffer_offsets) * counterBufferCount);
   }
}

void vk_enqueue_cmd_begin_query_indexed_ext(struct vk_cmd_queue *queue
, VkQueryPool queryPool
, uint32_t query
, VkQueryControlFlags flags
, uint32_t index
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_BEGIN_QUERY_INDEXED_EXT;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   cmd->u.begin_query_indexed_ext.query_pool = queryPool;
   cmd->u.begin_query_indexed_ext.query = query;
   cmd->u.begin_query_indexed_ext.flags = flags;
   cmd->u.begin_query_indexed_ext.index = index;
}

void vk_enqueue_cmd_end_query_indexed_ext(struct vk_cmd_queue *queue
, VkQueryPool queryPool
, uint32_t query
, uint32_t index
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_END_QUERY_INDEXED_EXT;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   cmd->u.end_query_indexed_ext.query_pool = queryPool;
   cmd->u.end_query_indexed_ext.query = query;
   cmd->u.end_query_indexed_ext.index = index;
}

void vk_enqueue_cmd_draw_indirect_byte_count_ext(struct vk_cmd_queue *queue
, uint32_t instanceCount
, uint32_t firstInstance
, VkBuffer counterBuffer
, VkDeviceSize counterBufferOffset
, uint32_t counterOffset
, uint32_t vertexStride
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_DRAW_INDIRECT_BYTE_COUNT_EXT;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   cmd->u.draw_indirect_byte_count_ext.instance_count = instanceCount;
   cmd->u.draw_indirect_byte_count_ext.first_instance = firstInstance;
   cmd->u.draw_indirect_byte_count_ext.counter_buffer = counterBuffer;
   cmd->u.draw_indirect_byte_count_ext.counter_buffer_offset = counterBufferOffset;
   cmd->u.draw_indirect_byte_count_ext.counter_offset = counterOffset;
   cmd->u.draw_indirect_byte_count_ext.vertex_stride = vertexStride;
}

void vk_enqueue_cmd_set_exclusive_scissor_nv(struct vk_cmd_queue *queue
, uint32_t firstExclusiveScissor
, uint32_t exclusiveScissorCount
, const VkRect2D* pExclusiveScissors
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_SET_EXCLUSIVE_SCISSOR_NV;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   cmd->u.set_exclusive_scissor_nv.first_exclusive_scissor = firstExclusiveScissor;
   cmd->u.set_exclusive_scissor_nv.exclusive_scissor_count = exclusiveScissorCount;
   if (pExclusiveScissors) {
      cmd->u.set_exclusive_scissor_nv.exclusive_scissors = vk_zalloc(queue->alloc, sizeof(*cmd->u.set_exclusive_scissor_nv.exclusive_scissors) * exclusiveScissorCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   memcpy(( VkRect2D* )cmd->u.set_exclusive_scissor_nv.exclusive_scissors, pExclusiveScissors, sizeof(*cmd->u.set_exclusive_scissor_nv.exclusive_scissors) * exclusiveScissorCount);
   }
}

void vk_enqueue_cmd_bind_shading_rate_image_nv(struct vk_cmd_queue *queue
, VkImageView imageView
, VkImageLayout imageLayout
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_BIND_SHADING_RATE_IMAGE_NV;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   cmd->u.bind_shading_rate_image_nv.image_view = imageView;
   cmd->u.bind_shading_rate_image_nv.image_layout = imageLayout;
}

void vk_enqueue_cmd_set_viewport_shading_rate_palette_nv(struct vk_cmd_queue *queue
, uint32_t firstViewport
, uint32_t viewportCount
, const VkShadingRatePaletteNV* pShadingRatePalettes
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_SET_VIEWPORT_SHADING_RATE_PALETTE_NV;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   cmd->u.set_viewport_shading_rate_palette_nv.first_viewport = firstViewport;
   cmd->u.set_viewport_shading_rate_palette_nv.viewport_count = viewportCount;
   if (pShadingRatePalettes) {
      cmd->u.set_viewport_shading_rate_palette_nv.shading_rate_palettes = vk_zalloc(queue->alloc, sizeof(*cmd->u.set_viewport_shading_rate_palette_nv.shading_rate_palettes) * viewportCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   memcpy(( VkShadingRatePaletteNV* )cmd->u.set_viewport_shading_rate_palette_nv.shading_rate_palettes, pShadingRatePalettes, sizeof(*cmd->u.set_viewport_shading_rate_palette_nv.shading_rate_palettes) * viewportCount);
   }
}

void vk_enqueue_cmd_set_coarse_sample_order_nv(struct vk_cmd_queue *queue
, VkCoarseSampleOrderTypeNV sampleOrderType
, uint32_t customSampleOrderCount
, const VkCoarseSampleOrderCustomNV* pCustomSampleOrders
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_SET_COARSE_SAMPLE_ORDER_NV;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   cmd->u.set_coarse_sample_order_nv.sample_order_type = sampleOrderType;
   cmd->u.set_coarse_sample_order_nv.custom_sample_order_count = customSampleOrderCount;
   if (pCustomSampleOrders) {
      cmd->u.set_coarse_sample_order_nv.custom_sample_orders = vk_zalloc(queue->alloc, sizeof(*cmd->u.set_coarse_sample_order_nv.custom_sample_orders) * customSampleOrderCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   memcpy(( VkCoarseSampleOrderCustomNV* )cmd->u.set_coarse_sample_order_nv.custom_sample_orders, pCustomSampleOrders, sizeof(*cmd->u.set_coarse_sample_order_nv.custom_sample_orders) * customSampleOrderCount);
   }
}

void vk_enqueue_cmd_draw_mesh_tasks_nv(struct vk_cmd_queue *queue
, uint32_t taskCount
, uint32_t firstTask
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_DRAW_MESH_TASKS_NV;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   cmd->u.draw_mesh_tasks_nv.task_count = taskCount;
   cmd->u.draw_mesh_tasks_nv.first_task = firstTask;
}

void vk_enqueue_cmd_draw_mesh_tasks_indirect_nv(struct vk_cmd_queue *queue
, VkBuffer buffer
, VkDeviceSize offset
, uint32_t drawCount
, uint32_t stride
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_DRAW_MESH_TASKS_INDIRECT_NV;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   cmd->u.draw_mesh_tasks_indirect_nv.buffer = buffer;
   cmd->u.draw_mesh_tasks_indirect_nv.offset = offset;
   cmd->u.draw_mesh_tasks_indirect_nv.draw_count = drawCount;
   cmd->u.draw_mesh_tasks_indirect_nv.stride = stride;
}

void vk_enqueue_cmd_draw_mesh_tasks_indirect_count_nv(struct vk_cmd_queue *queue
, VkBuffer buffer
, VkDeviceSize offset
, VkBuffer countBuffer
, VkDeviceSize countBufferOffset
, uint32_t maxDrawCount
, uint32_t stride
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_DRAW_MESH_TASKS_INDIRECT_COUNT_NV;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   cmd->u.draw_mesh_tasks_indirect_count_nv.buffer = buffer;
   cmd->u.draw_mesh_tasks_indirect_count_nv.offset = offset;
   cmd->u.draw_mesh_tasks_indirect_count_nv.count_buffer = countBuffer;
   cmd->u.draw_mesh_tasks_indirect_count_nv.count_buffer_offset = countBufferOffset;
   cmd->u.draw_mesh_tasks_indirect_count_nv.max_draw_count = maxDrawCount;
   cmd->u.draw_mesh_tasks_indirect_count_nv.stride = stride;
}

void vk_enqueue_cmd_bind_invocation_mask_huawei(struct vk_cmd_queue *queue
, VkImageView imageView
, VkImageLayout imageLayout
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_BIND_INVOCATION_MASK_HUAWEI;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   cmd->u.bind_invocation_mask_huawei.image_view = imageView;
   cmd->u.bind_invocation_mask_huawei.image_layout = imageLayout;
}

void vk_enqueue_cmd_copy_acceleration_structure_nv(struct vk_cmd_queue *queue
, VkAccelerationStructureNV dst
, VkAccelerationStructureNV src
, VkCopyAccelerationStructureModeKHR mode
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_COPY_ACCELERATION_STRUCTURE_NV;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   cmd->u.copy_acceleration_structure_nv.dst = dst;
   cmd->u.copy_acceleration_structure_nv.src = src;
   cmd->u.copy_acceleration_structure_nv.mode = mode;
}

void vk_enqueue_cmd_copy_acceleration_structure_khr(struct vk_cmd_queue *queue
, const VkCopyAccelerationStructureInfoKHR* pInfo
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_COPY_ACCELERATION_STRUCTURE_KHR;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   if (pInfo) {
      cmd->u.copy_acceleration_structure_khr.info = vk_zalloc(queue->alloc, sizeof(VkCopyAccelerationStructureInfoKHR), 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
      memcpy((void*)cmd->u.copy_acceleration_structure_khr.info, pInfo, sizeof(VkCopyAccelerationStructureInfoKHR));
   VkCopyAccelerationStructureInfoKHR *tmp_dst1 = (void *) cmd->u.copy_acceleration_structure_khr.info; (void) tmp_dst1;
   VkCopyAccelerationStructureInfoKHR *tmp_src1 = (void *) pInfo; (void) tmp_src1;   
      } else {
      cmd->u.copy_acceleration_structure_khr.info = NULL;
   }
}

void vk_enqueue_cmd_copy_acceleration_structure_to_memory_khr(struct vk_cmd_queue *queue
, const VkCopyAccelerationStructureToMemoryInfoKHR* pInfo
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_COPY_ACCELERATION_STRUCTURE_TO_MEMORY_KHR;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   if (pInfo) {
      cmd->u.copy_acceleration_structure_to_memory_khr.info = vk_zalloc(queue->alloc, sizeof(VkCopyAccelerationStructureToMemoryInfoKHR), 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
      memcpy((void*)cmd->u.copy_acceleration_structure_to_memory_khr.info, pInfo, sizeof(VkCopyAccelerationStructureToMemoryInfoKHR));
   VkCopyAccelerationStructureToMemoryInfoKHR *tmp_dst1 = (void *) cmd->u.copy_acceleration_structure_to_memory_khr.info; (void) tmp_dst1;
   VkCopyAccelerationStructureToMemoryInfoKHR *tmp_src1 = (void *) pInfo; (void) tmp_src1;   
      } else {
      cmd->u.copy_acceleration_structure_to_memory_khr.info = NULL;
   }
}

void vk_enqueue_cmd_copy_memory_to_acceleration_structure_khr(struct vk_cmd_queue *queue
, const VkCopyMemoryToAccelerationStructureInfoKHR* pInfo
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_COPY_MEMORY_TO_ACCELERATION_STRUCTURE_KHR;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   if (pInfo) {
      cmd->u.copy_memory_to_acceleration_structure_khr.info = vk_zalloc(queue->alloc, sizeof(VkCopyMemoryToAccelerationStructureInfoKHR), 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
      memcpy((void*)cmd->u.copy_memory_to_acceleration_structure_khr.info, pInfo, sizeof(VkCopyMemoryToAccelerationStructureInfoKHR));
   VkCopyMemoryToAccelerationStructureInfoKHR *tmp_dst1 = (void *) cmd->u.copy_memory_to_acceleration_structure_khr.info; (void) tmp_dst1;
   VkCopyMemoryToAccelerationStructureInfoKHR *tmp_src1 = (void *) pInfo; (void) tmp_src1;   
      } else {
      cmd->u.copy_memory_to_acceleration_structure_khr.info = NULL;
   }
}

void vk_enqueue_cmd_write_acceleration_structures_properties_khr(struct vk_cmd_queue *queue
, uint32_t accelerationStructureCount
, const VkAccelerationStructureKHR* pAccelerationStructures
, VkQueryType queryType
, VkQueryPool queryPool
, uint32_t firstQuery
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_WRITE_ACCELERATION_STRUCTURES_PROPERTIES_KHR;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   cmd->u.write_acceleration_structures_properties_khr.acceleration_structure_count = accelerationStructureCount;
   if (pAccelerationStructures) {
      cmd->u.write_acceleration_structures_properties_khr.acceleration_structures = vk_zalloc(queue->alloc, sizeof(*cmd->u.write_acceleration_structures_properties_khr.acceleration_structures) * accelerationStructureCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   memcpy(( VkAccelerationStructureKHR* )cmd->u.write_acceleration_structures_properties_khr.acceleration_structures, pAccelerationStructures, sizeof(*cmd->u.write_acceleration_structures_properties_khr.acceleration_structures) * accelerationStructureCount);
   }
   cmd->u.write_acceleration_structures_properties_khr.query_type = queryType;
   cmd->u.write_acceleration_structures_properties_khr.query_pool = queryPool;
   cmd->u.write_acceleration_structures_properties_khr.first_query = firstQuery;
}

void vk_enqueue_cmd_write_acceleration_structures_properties_nv(struct vk_cmd_queue *queue
, uint32_t accelerationStructureCount
, const VkAccelerationStructureNV* pAccelerationStructures
, VkQueryType queryType
, VkQueryPool queryPool
, uint32_t firstQuery
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_WRITE_ACCELERATION_STRUCTURES_PROPERTIES_NV;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   cmd->u.write_acceleration_structures_properties_nv.acceleration_structure_count = accelerationStructureCount;
   if (pAccelerationStructures) {
      cmd->u.write_acceleration_structures_properties_nv.acceleration_structures = vk_zalloc(queue->alloc, sizeof(*cmd->u.write_acceleration_structures_properties_nv.acceleration_structures) * accelerationStructureCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   memcpy(( VkAccelerationStructureNV* )cmd->u.write_acceleration_structures_properties_nv.acceleration_structures, pAccelerationStructures, sizeof(*cmd->u.write_acceleration_structures_properties_nv.acceleration_structures) * accelerationStructureCount);
   }
   cmd->u.write_acceleration_structures_properties_nv.query_type = queryType;
   cmd->u.write_acceleration_structures_properties_nv.query_pool = queryPool;
   cmd->u.write_acceleration_structures_properties_nv.first_query = firstQuery;
}

void vk_enqueue_cmd_build_acceleration_structure_nv(struct vk_cmd_queue *queue
, const VkAccelerationStructureInfoNV* pInfo
, VkBuffer instanceData
, VkDeviceSize instanceOffset
, VkBool32 update
, VkAccelerationStructureNV dst
, VkAccelerationStructureNV src
, VkBuffer scratch
, VkDeviceSize scratchOffset
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_BUILD_ACCELERATION_STRUCTURE_NV;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   if (pInfo) {
      cmd->u.build_acceleration_structure_nv.info = vk_zalloc(queue->alloc, sizeof(VkAccelerationStructureInfoNV), 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
      memcpy((void*)cmd->u.build_acceleration_structure_nv.info, pInfo, sizeof(VkAccelerationStructureInfoNV));
   VkAccelerationStructureInfoNV *tmp_dst1 = (void *) cmd->u.build_acceleration_structure_nv.info; (void) tmp_dst1;
   VkAccelerationStructureInfoNV *tmp_src1 = (void *) pInfo; (void) tmp_src1;   
   tmp_dst1->pGeometries = vk_zalloc(queue->alloc, sizeof(*tmp_dst1->pGeometries) * tmp_dst1->geometryCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   memcpy(( VkGeometryNV* )tmp_dst1->pGeometries, tmp_src1->pGeometries, sizeof(*tmp_dst1->pGeometries) * tmp_dst1->geometryCount);
   } else {
      cmd->u.build_acceleration_structure_nv.info = NULL;
   }
   cmd->u.build_acceleration_structure_nv.instance_data = instanceData;
   cmd->u.build_acceleration_structure_nv.instance_offset = instanceOffset;
   cmd->u.build_acceleration_structure_nv.update = update;
   cmd->u.build_acceleration_structure_nv.dst = dst;
   cmd->u.build_acceleration_structure_nv.src = src;
   cmd->u.build_acceleration_structure_nv.scratch = scratch;
   cmd->u.build_acceleration_structure_nv.scratch_offset = scratchOffset;
}

void vk_enqueue_cmd_trace_rays_khr(struct vk_cmd_queue *queue
, const VkStridedDeviceAddressRegionKHR* pRaygenShaderBindingTable
, const VkStridedDeviceAddressRegionKHR* pMissShaderBindingTable
, const VkStridedDeviceAddressRegionKHR* pHitShaderBindingTable
, const VkStridedDeviceAddressRegionKHR* pCallableShaderBindingTable
, uint32_t width
, uint32_t height
, uint32_t depth
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_TRACE_RAYS_KHR;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   if (pRaygenShaderBindingTable) {
      cmd->u.trace_rays_khr.raygen_shader_binding_table = vk_zalloc(queue->alloc, sizeof(VkStridedDeviceAddressRegionKHR), 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
      memcpy((void*)cmd->u.trace_rays_khr.raygen_shader_binding_table, pRaygenShaderBindingTable, sizeof(VkStridedDeviceAddressRegionKHR));
   VkStridedDeviceAddressRegionKHR *tmp_dst1 = (void *) cmd->u.trace_rays_khr.raygen_shader_binding_table; (void) tmp_dst1;
   VkStridedDeviceAddressRegionKHR *tmp_src1 = (void *) pRaygenShaderBindingTable; (void) tmp_src1;   
      } else {
      cmd->u.trace_rays_khr.raygen_shader_binding_table = NULL;
   }
   if (pMissShaderBindingTable) {
      cmd->u.trace_rays_khr.miss_shader_binding_table = vk_zalloc(queue->alloc, sizeof(VkStridedDeviceAddressRegionKHR), 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
      memcpy((void*)cmd->u.trace_rays_khr.miss_shader_binding_table, pMissShaderBindingTable, sizeof(VkStridedDeviceAddressRegionKHR));
   VkStridedDeviceAddressRegionKHR *tmp_dst1 = (void *) cmd->u.trace_rays_khr.miss_shader_binding_table; (void) tmp_dst1;
   VkStridedDeviceAddressRegionKHR *tmp_src1 = (void *) pMissShaderBindingTable; (void) tmp_src1;   
      } else {
      cmd->u.trace_rays_khr.miss_shader_binding_table = NULL;
   }
   if (pHitShaderBindingTable) {
      cmd->u.trace_rays_khr.hit_shader_binding_table = vk_zalloc(queue->alloc, sizeof(VkStridedDeviceAddressRegionKHR), 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
      memcpy((void*)cmd->u.trace_rays_khr.hit_shader_binding_table, pHitShaderBindingTable, sizeof(VkStridedDeviceAddressRegionKHR));
   VkStridedDeviceAddressRegionKHR *tmp_dst1 = (void *) cmd->u.trace_rays_khr.hit_shader_binding_table; (void) tmp_dst1;
   VkStridedDeviceAddressRegionKHR *tmp_src1 = (void *) pHitShaderBindingTable; (void) tmp_src1;   
      } else {
      cmd->u.trace_rays_khr.hit_shader_binding_table = NULL;
   }
   if (pCallableShaderBindingTable) {
      cmd->u.trace_rays_khr.callable_shader_binding_table = vk_zalloc(queue->alloc, sizeof(VkStridedDeviceAddressRegionKHR), 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
      memcpy((void*)cmd->u.trace_rays_khr.callable_shader_binding_table, pCallableShaderBindingTable, sizeof(VkStridedDeviceAddressRegionKHR));
   VkStridedDeviceAddressRegionKHR *tmp_dst1 = (void *) cmd->u.trace_rays_khr.callable_shader_binding_table; (void) tmp_dst1;
   VkStridedDeviceAddressRegionKHR *tmp_src1 = (void *) pCallableShaderBindingTable; (void) tmp_src1;   
      } else {
      cmd->u.trace_rays_khr.callable_shader_binding_table = NULL;
   }
   cmd->u.trace_rays_khr.width = width;
   cmd->u.trace_rays_khr.height = height;
   cmd->u.trace_rays_khr.depth = depth;
}

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
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_TRACE_RAYS_NV;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   cmd->u.trace_rays_nv.raygen_shader_binding_table_buffer = raygenShaderBindingTableBuffer;
   cmd->u.trace_rays_nv.raygen_shader_binding_offset = raygenShaderBindingOffset;
   cmd->u.trace_rays_nv.miss_shader_binding_table_buffer = missShaderBindingTableBuffer;
   cmd->u.trace_rays_nv.miss_shader_binding_offset = missShaderBindingOffset;
   cmd->u.trace_rays_nv.miss_shader_binding_stride = missShaderBindingStride;
   cmd->u.trace_rays_nv.hit_shader_binding_table_buffer = hitShaderBindingTableBuffer;
   cmd->u.trace_rays_nv.hit_shader_binding_offset = hitShaderBindingOffset;
   cmd->u.trace_rays_nv.hit_shader_binding_stride = hitShaderBindingStride;
   cmd->u.trace_rays_nv.callable_shader_binding_table_buffer = callableShaderBindingTableBuffer;
   cmd->u.trace_rays_nv.callable_shader_binding_offset = callableShaderBindingOffset;
   cmd->u.trace_rays_nv.callable_shader_binding_stride = callableShaderBindingStride;
   cmd->u.trace_rays_nv.width = width;
   cmd->u.trace_rays_nv.height = height;
   cmd->u.trace_rays_nv.depth = depth;
}

void vk_enqueue_cmd_trace_rays_indirect_khr(struct vk_cmd_queue *queue
, const VkStridedDeviceAddressRegionKHR* pRaygenShaderBindingTable
, const VkStridedDeviceAddressRegionKHR* pMissShaderBindingTable
, const VkStridedDeviceAddressRegionKHR* pHitShaderBindingTable
, const VkStridedDeviceAddressRegionKHR* pCallableShaderBindingTable
, VkDeviceAddress indirectDeviceAddress
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_TRACE_RAYS_INDIRECT_KHR;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   if (pRaygenShaderBindingTable) {
      cmd->u.trace_rays_indirect_khr.raygen_shader_binding_table = vk_zalloc(queue->alloc, sizeof(VkStridedDeviceAddressRegionKHR), 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
      memcpy((void*)cmd->u.trace_rays_indirect_khr.raygen_shader_binding_table, pRaygenShaderBindingTable, sizeof(VkStridedDeviceAddressRegionKHR));
   VkStridedDeviceAddressRegionKHR *tmp_dst1 = (void *) cmd->u.trace_rays_indirect_khr.raygen_shader_binding_table; (void) tmp_dst1;
   VkStridedDeviceAddressRegionKHR *tmp_src1 = (void *) pRaygenShaderBindingTable; (void) tmp_src1;   
      } else {
      cmd->u.trace_rays_indirect_khr.raygen_shader_binding_table = NULL;
   }
   if (pMissShaderBindingTable) {
      cmd->u.trace_rays_indirect_khr.miss_shader_binding_table = vk_zalloc(queue->alloc, sizeof(VkStridedDeviceAddressRegionKHR), 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
      memcpy((void*)cmd->u.trace_rays_indirect_khr.miss_shader_binding_table, pMissShaderBindingTable, sizeof(VkStridedDeviceAddressRegionKHR));
   VkStridedDeviceAddressRegionKHR *tmp_dst1 = (void *) cmd->u.trace_rays_indirect_khr.miss_shader_binding_table; (void) tmp_dst1;
   VkStridedDeviceAddressRegionKHR *tmp_src1 = (void *) pMissShaderBindingTable; (void) tmp_src1;   
      } else {
      cmd->u.trace_rays_indirect_khr.miss_shader_binding_table = NULL;
   }
   if (pHitShaderBindingTable) {
      cmd->u.trace_rays_indirect_khr.hit_shader_binding_table = vk_zalloc(queue->alloc, sizeof(VkStridedDeviceAddressRegionKHR), 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
      memcpy((void*)cmd->u.trace_rays_indirect_khr.hit_shader_binding_table, pHitShaderBindingTable, sizeof(VkStridedDeviceAddressRegionKHR));
   VkStridedDeviceAddressRegionKHR *tmp_dst1 = (void *) cmd->u.trace_rays_indirect_khr.hit_shader_binding_table; (void) tmp_dst1;
   VkStridedDeviceAddressRegionKHR *tmp_src1 = (void *) pHitShaderBindingTable; (void) tmp_src1;   
      } else {
      cmd->u.trace_rays_indirect_khr.hit_shader_binding_table = NULL;
   }
   if (pCallableShaderBindingTable) {
      cmd->u.trace_rays_indirect_khr.callable_shader_binding_table = vk_zalloc(queue->alloc, sizeof(VkStridedDeviceAddressRegionKHR), 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
      memcpy((void*)cmd->u.trace_rays_indirect_khr.callable_shader_binding_table, pCallableShaderBindingTable, sizeof(VkStridedDeviceAddressRegionKHR));
   VkStridedDeviceAddressRegionKHR *tmp_dst1 = (void *) cmd->u.trace_rays_indirect_khr.callable_shader_binding_table; (void) tmp_dst1;
   VkStridedDeviceAddressRegionKHR *tmp_src1 = (void *) pCallableShaderBindingTable; (void) tmp_src1;   
      } else {
      cmd->u.trace_rays_indirect_khr.callable_shader_binding_table = NULL;
   }
   cmd->u.trace_rays_indirect_khr.indirect_device_address = indirectDeviceAddress;
}

void vk_enqueue_cmd_set_ray_tracing_pipeline_stack_size_khr(struct vk_cmd_queue *queue
, uint32_t pipelineStackSize
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_SET_RAY_TRACING_PIPELINE_STACK_SIZE_KHR;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   cmd->u.set_ray_tracing_pipeline_stack_size_khr.pipeline_stack_size = pipelineStackSize;
}

void vk_enqueue_cmd_set_performance_marker_intel(struct vk_cmd_queue *queue
, const VkPerformanceMarkerInfoINTEL* pMarkerInfo
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_SET_PERFORMANCE_MARKER_INTEL;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   if (pMarkerInfo) {
      cmd->u.set_performance_marker_intel.marker_info = vk_zalloc(queue->alloc, sizeof(VkPerformanceMarkerInfoINTEL), 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
      memcpy((void*)cmd->u.set_performance_marker_intel.marker_info, pMarkerInfo, sizeof(VkPerformanceMarkerInfoINTEL));
   VkPerformanceMarkerInfoINTEL *tmp_dst1 = (void *) cmd->u.set_performance_marker_intel.marker_info; (void) tmp_dst1;
   VkPerformanceMarkerInfoINTEL *tmp_src1 = (void *) pMarkerInfo; (void) tmp_src1;   
      } else {
      cmd->u.set_performance_marker_intel.marker_info = NULL;
   }
}

void vk_enqueue_cmd_set_performance_stream_marker_intel(struct vk_cmd_queue *queue
, const VkPerformanceStreamMarkerInfoINTEL* pMarkerInfo
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_SET_PERFORMANCE_STREAM_MARKER_INTEL;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   if (pMarkerInfo) {
      cmd->u.set_performance_stream_marker_intel.marker_info = vk_zalloc(queue->alloc, sizeof(VkPerformanceStreamMarkerInfoINTEL), 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
      memcpy((void*)cmd->u.set_performance_stream_marker_intel.marker_info, pMarkerInfo, sizeof(VkPerformanceStreamMarkerInfoINTEL));
   VkPerformanceStreamMarkerInfoINTEL *tmp_dst1 = (void *) cmd->u.set_performance_stream_marker_intel.marker_info; (void) tmp_dst1;
   VkPerformanceStreamMarkerInfoINTEL *tmp_src1 = (void *) pMarkerInfo; (void) tmp_src1;   
      } else {
      cmd->u.set_performance_stream_marker_intel.marker_info = NULL;
   }
}

void vk_enqueue_cmd_set_performance_override_intel(struct vk_cmd_queue *queue
, const VkPerformanceOverrideInfoINTEL* pOverrideInfo
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_SET_PERFORMANCE_OVERRIDE_INTEL;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   if (pOverrideInfo) {
      cmd->u.set_performance_override_intel.override_info = vk_zalloc(queue->alloc, sizeof(VkPerformanceOverrideInfoINTEL), 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
      memcpy((void*)cmd->u.set_performance_override_intel.override_info, pOverrideInfo, sizeof(VkPerformanceOverrideInfoINTEL));
   VkPerformanceOverrideInfoINTEL *tmp_dst1 = (void *) cmd->u.set_performance_override_intel.override_info; (void) tmp_dst1;
   VkPerformanceOverrideInfoINTEL *tmp_src1 = (void *) pOverrideInfo; (void) tmp_src1;   
      } else {
      cmd->u.set_performance_override_intel.override_info = NULL;
   }
}

void vk_enqueue_cmd_set_line_stipple_ext(struct vk_cmd_queue *queue
, uint32_t lineStippleFactor
, uint16_t lineStipplePattern
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_SET_LINE_STIPPLE_EXT;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   cmd->u.set_line_stipple_ext.line_stipple_factor = lineStippleFactor;
   cmd->u.set_line_stipple_ext.line_stipple_pattern = lineStipplePattern;
}

void vk_enqueue_cmd_build_acceleration_structures_khr(struct vk_cmd_queue *queue
, uint32_t infoCount
, const VkAccelerationStructureBuildGeometryInfoKHR* pInfos
, const VkAccelerationStructureBuildRangeInfoKHR* const* ppBuildRangeInfos
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_BUILD_ACCELERATION_STRUCTURES_KHR;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   cmd->u.build_acceleration_structures_khr.info_count = infoCount;
   if (pInfos) {
      cmd->u.build_acceleration_structures_khr.infos = vk_zalloc(queue->alloc, sizeof(*cmd->u.build_acceleration_structures_khr.infos) * infoCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   memcpy(( VkAccelerationStructureBuildGeometryInfoKHR* )cmd->u.build_acceleration_structures_khr.infos, pInfos, sizeof(*cmd->u.build_acceleration_structures_khr.infos) * infoCount);
   }
   if (ppBuildRangeInfos) {
      cmd->u.build_acceleration_structures_khr.pp_build_range_infos = vk_zalloc(queue->alloc, sizeof(*cmd->u.build_acceleration_structures_khr.pp_build_range_infos) * infoCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   memcpy(( VkAccelerationStructureBuildRangeInfoKHR* * )cmd->u.build_acceleration_structures_khr.pp_build_range_infos, ppBuildRangeInfos, sizeof(*cmd->u.build_acceleration_structures_khr.pp_build_range_infos) * infoCount);
   }
}

void vk_enqueue_cmd_build_acceleration_structures_indirect_khr(struct vk_cmd_queue *queue
, uint32_t                                           infoCount
, const VkAccelerationStructureBuildGeometryInfoKHR* pInfos
, const VkDeviceAddress*             pIndirectDeviceAddresses
, const uint32_t*                    pIndirectStrides
, const uint32_t* const*             ppMaxPrimitiveCounts
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_BUILD_ACCELERATION_STRUCTURES_INDIRECT_KHR;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   cmd->u.build_acceleration_structures_indirect_khr.info_count = infoCount;
   if (pInfos) {
      cmd->u.build_acceleration_structures_indirect_khr.infos = vk_zalloc(queue->alloc, sizeof(*cmd->u.build_acceleration_structures_indirect_khr.infos) * infoCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   memcpy(( VkAccelerationStructureBuildGeometryInfoKHR* )cmd->u.build_acceleration_structures_indirect_khr.infos, pInfos, sizeof(*cmd->u.build_acceleration_structures_indirect_khr.infos) * infoCount);
   }
   if (pIndirectDeviceAddresses) {
      cmd->u.build_acceleration_structures_indirect_khr.indirect_device_addresses = vk_zalloc(queue->alloc, sizeof(*cmd->u.build_acceleration_structures_indirect_khr.indirect_device_addresses) * infoCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   memcpy(( VkDeviceAddress*             )cmd->u.build_acceleration_structures_indirect_khr.indirect_device_addresses, pIndirectDeviceAddresses, sizeof(*cmd->u.build_acceleration_structures_indirect_khr.indirect_device_addresses) * infoCount);
   }
   if (pIndirectStrides) {
      cmd->u.build_acceleration_structures_indirect_khr.indirect_strides = vk_zalloc(queue->alloc, sizeof(*cmd->u.build_acceleration_structures_indirect_khr.indirect_strides) * infoCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   memcpy(( uint32_t*                    )cmd->u.build_acceleration_structures_indirect_khr.indirect_strides, pIndirectStrides, sizeof(*cmd->u.build_acceleration_structures_indirect_khr.indirect_strides) * infoCount);
   }
   if (ppMaxPrimitiveCounts) {
      cmd->u.build_acceleration_structures_indirect_khr.pp_max_primitive_counts = vk_zalloc(queue->alloc, sizeof(*cmd->u.build_acceleration_structures_indirect_khr.pp_max_primitive_counts) * infoCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   memcpy(( uint32_t* *             )cmd->u.build_acceleration_structures_indirect_khr.pp_max_primitive_counts, ppMaxPrimitiveCounts, sizeof(*cmd->u.build_acceleration_structures_indirect_khr.pp_max_primitive_counts) * infoCount);
   }
}

void vk_enqueue_cmd_set_cull_mode_ext(struct vk_cmd_queue *queue
, VkCullModeFlags cullMode
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_SET_CULL_MODE_EXT;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   cmd->u.set_cull_mode_ext.cull_mode = cullMode;
}

void vk_enqueue_cmd_set_front_face_ext(struct vk_cmd_queue *queue
, VkFrontFace frontFace
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_SET_FRONT_FACE_EXT;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   cmd->u.set_front_face_ext.front_face = frontFace;
}

void vk_enqueue_cmd_set_primitive_topology_ext(struct vk_cmd_queue *queue
, VkPrimitiveTopology primitiveTopology
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_SET_PRIMITIVE_TOPOLOGY_EXT;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   cmd->u.set_primitive_topology_ext.primitive_topology = primitiveTopology;
}

void vk_enqueue_cmd_set_viewport_with_count_ext(struct vk_cmd_queue *queue
, uint32_t viewportCount
, const VkViewport* pViewports
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_SET_VIEWPORT_WITH_COUNT_EXT;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   cmd->u.set_viewport_with_count_ext.viewport_count = viewportCount;
   if (pViewports) {
      cmd->u.set_viewport_with_count_ext.viewports = vk_zalloc(queue->alloc, sizeof(*cmd->u.set_viewport_with_count_ext.viewports) * viewportCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   memcpy(( VkViewport* )cmd->u.set_viewport_with_count_ext.viewports, pViewports, sizeof(*cmd->u.set_viewport_with_count_ext.viewports) * viewportCount);
   }
}

void vk_enqueue_cmd_set_scissor_with_count_ext(struct vk_cmd_queue *queue
, uint32_t scissorCount
, const VkRect2D* pScissors
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_SET_SCISSOR_WITH_COUNT_EXT;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   cmd->u.set_scissor_with_count_ext.scissor_count = scissorCount;
   if (pScissors) {
      cmd->u.set_scissor_with_count_ext.scissors = vk_zalloc(queue->alloc, sizeof(*cmd->u.set_scissor_with_count_ext.scissors) * scissorCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   memcpy(( VkRect2D* )cmd->u.set_scissor_with_count_ext.scissors, pScissors, sizeof(*cmd->u.set_scissor_with_count_ext.scissors) * scissorCount);
   }
}

void vk_enqueue_cmd_bind_vertex_buffers2_ext(struct vk_cmd_queue *queue
, uint32_t firstBinding
, uint32_t bindingCount
, const VkBuffer* pBuffers
, const VkDeviceSize* pOffsets
, const VkDeviceSize* pSizes
, const VkDeviceSize* pStrides
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_BIND_VERTEX_BUFFERS2_EXT;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   cmd->u.bind_vertex_buffers2_ext.first_binding = firstBinding;
   cmd->u.bind_vertex_buffers2_ext.binding_count = bindingCount;
   if (pBuffers) {
      cmd->u.bind_vertex_buffers2_ext.buffers = vk_zalloc(queue->alloc, sizeof(*cmd->u.bind_vertex_buffers2_ext.buffers) * bindingCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   memcpy(( VkBuffer* )cmd->u.bind_vertex_buffers2_ext.buffers, pBuffers, sizeof(*cmd->u.bind_vertex_buffers2_ext.buffers) * bindingCount);
   }
   if (pOffsets) {
      cmd->u.bind_vertex_buffers2_ext.offsets = vk_zalloc(queue->alloc, sizeof(*cmd->u.bind_vertex_buffers2_ext.offsets) * bindingCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   memcpy(( VkDeviceSize* )cmd->u.bind_vertex_buffers2_ext.offsets, pOffsets, sizeof(*cmd->u.bind_vertex_buffers2_ext.offsets) * bindingCount);
   }
   if (pSizes) {
      cmd->u.bind_vertex_buffers2_ext.sizes = vk_zalloc(queue->alloc, sizeof(*cmd->u.bind_vertex_buffers2_ext.sizes) * bindingCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   memcpy(( VkDeviceSize* )cmd->u.bind_vertex_buffers2_ext.sizes, pSizes, sizeof(*cmd->u.bind_vertex_buffers2_ext.sizes) * bindingCount);
   }
   if (pStrides) {
      cmd->u.bind_vertex_buffers2_ext.strides = vk_zalloc(queue->alloc, sizeof(*cmd->u.bind_vertex_buffers2_ext.strides) * bindingCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   memcpy(( VkDeviceSize* )cmd->u.bind_vertex_buffers2_ext.strides, pStrides, sizeof(*cmd->u.bind_vertex_buffers2_ext.strides) * bindingCount);
   }
}

void vk_enqueue_cmd_set_depth_test_enable_ext(struct vk_cmd_queue *queue
, VkBool32 depthTestEnable
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_SET_DEPTH_TEST_ENABLE_EXT;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   cmd->u.set_depth_test_enable_ext.depth_test_enable = depthTestEnable;
}

void vk_enqueue_cmd_set_depth_write_enable_ext(struct vk_cmd_queue *queue
, VkBool32 depthWriteEnable
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_SET_DEPTH_WRITE_ENABLE_EXT;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   cmd->u.set_depth_write_enable_ext.depth_write_enable = depthWriteEnable;
}

void vk_enqueue_cmd_set_depth_compare_op_ext(struct vk_cmd_queue *queue
, VkCompareOp depthCompareOp
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_SET_DEPTH_COMPARE_OP_EXT;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   cmd->u.set_depth_compare_op_ext.depth_compare_op = depthCompareOp;
}

void vk_enqueue_cmd_set_depth_bounds_test_enable_ext(struct vk_cmd_queue *queue
, VkBool32 depthBoundsTestEnable
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_SET_DEPTH_BOUNDS_TEST_ENABLE_EXT;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   cmd->u.set_depth_bounds_test_enable_ext.depth_bounds_test_enable = depthBoundsTestEnable;
}

void vk_enqueue_cmd_set_stencil_test_enable_ext(struct vk_cmd_queue *queue
, VkBool32 stencilTestEnable
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_SET_STENCIL_TEST_ENABLE_EXT;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   cmd->u.set_stencil_test_enable_ext.stencil_test_enable = stencilTestEnable;
}

void vk_enqueue_cmd_set_stencil_op_ext(struct vk_cmd_queue *queue
, VkStencilFaceFlags faceMask
, VkStencilOp failOp
, VkStencilOp passOp
, VkStencilOp depthFailOp
, VkCompareOp compareOp
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_SET_STENCIL_OP_EXT;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   cmd->u.set_stencil_op_ext.face_mask = faceMask;
   cmd->u.set_stencil_op_ext.fail_op = failOp;
   cmd->u.set_stencil_op_ext.pass_op = passOp;
   cmd->u.set_stencil_op_ext.depth_fail_op = depthFailOp;
   cmd->u.set_stencil_op_ext.compare_op = compareOp;
}

void vk_enqueue_cmd_set_patch_control_points_ext(struct vk_cmd_queue *queue
, uint32_t patchControlPoints
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_SET_PATCH_CONTROL_POINTS_EXT;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   cmd->u.set_patch_control_points_ext.patch_control_points = patchControlPoints;
}

void vk_enqueue_cmd_set_rasterizer_discard_enable_ext(struct vk_cmd_queue *queue
, VkBool32 rasterizerDiscardEnable
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_SET_RASTERIZER_DISCARD_ENABLE_EXT;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   cmd->u.set_rasterizer_discard_enable_ext.rasterizer_discard_enable = rasterizerDiscardEnable;
}

void vk_enqueue_cmd_set_depth_bias_enable_ext(struct vk_cmd_queue *queue
, VkBool32 depthBiasEnable
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_SET_DEPTH_BIAS_ENABLE_EXT;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   cmd->u.set_depth_bias_enable_ext.depth_bias_enable = depthBiasEnable;
}

void vk_enqueue_cmd_set_logic_op_ext(struct vk_cmd_queue *queue
, VkLogicOp logicOp
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_SET_LOGIC_OP_EXT;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   cmd->u.set_logic_op_ext.logic_op = logicOp;
}

void vk_enqueue_cmd_set_primitive_restart_enable_ext(struct vk_cmd_queue *queue
, VkBool32 primitiveRestartEnable
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_SET_PRIMITIVE_RESTART_ENABLE_EXT;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   cmd->u.set_primitive_restart_enable_ext.primitive_restart_enable = primitiveRestartEnable;
}

void vk_enqueue_cmd_copy_buffer2_khr(struct vk_cmd_queue *queue
, const VkCopyBufferInfo2KHR* pCopyBufferInfo
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_COPY_BUFFER2_KHR;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   if (pCopyBufferInfo) {
      cmd->u.copy_buffer2_khr.copy_buffer_info = vk_zalloc(queue->alloc, sizeof(VkCopyBufferInfo2KHR), 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
      memcpy((void*)cmd->u.copy_buffer2_khr.copy_buffer_info, pCopyBufferInfo, sizeof(VkCopyBufferInfo2KHR));
   VkCopyBufferInfo2KHR *tmp_dst1 = (void *) cmd->u.copy_buffer2_khr.copy_buffer_info; (void) tmp_dst1;
   VkCopyBufferInfo2KHR *tmp_src1 = (void *) pCopyBufferInfo; (void) tmp_src1;   
   tmp_dst1->pRegions = vk_zalloc(queue->alloc, sizeof(*tmp_dst1->pRegions) * tmp_dst1->regionCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   memcpy(( VkBufferCopy2KHR* )tmp_dst1->pRegions, tmp_src1->pRegions, sizeof(*tmp_dst1->pRegions) * tmp_dst1->regionCount);
   } else {
      cmd->u.copy_buffer2_khr.copy_buffer_info = NULL;
   }
}

void vk_enqueue_cmd_copy_image2_khr(struct vk_cmd_queue *queue
, const VkCopyImageInfo2KHR* pCopyImageInfo
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_COPY_IMAGE2_KHR;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   if (pCopyImageInfo) {
      cmd->u.copy_image2_khr.copy_image_info = vk_zalloc(queue->alloc, sizeof(VkCopyImageInfo2KHR), 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
      memcpy((void*)cmd->u.copy_image2_khr.copy_image_info, pCopyImageInfo, sizeof(VkCopyImageInfo2KHR));
   VkCopyImageInfo2KHR *tmp_dst1 = (void *) cmd->u.copy_image2_khr.copy_image_info; (void) tmp_dst1;
   VkCopyImageInfo2KHR *tmp_src1 = (void *) pCopyImageInfo; (void) tmp_src1;   
   tmp_dst1->pRegions = vk_zalloc(queue->alloc, sizeof(*tmp_dst1->pRegions) * tmp_dst1->regionCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   memcpy(( VkImageCopy2KHR* )tmp_dst1->pRegions, tmp_src1->pRegions, sizeof(*tmp_dst1->pRegions) * tmp_dst1->regionCount);
   } else {
      cmd->u.copy_image2_khr.copy_image_info = NULL;
   }
}

void vk_enqueue_cmd_blit_image2_khr(struct vk_cmd_queue *queue
, const VkBlitImageInfo2KHR* pBlitImageInfo
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_BLIT_IMAGE2_KHR;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   if (pBlitImageInfo) {
      cmd->u.blit_image2_khr.blit_image_info = vk_zalloc(queue->alloc, sizeof(VkBlitImageInfo2KHR), 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
      memcpy((void*)cmd->u.blit_image2_khr.blit_image_info, pBlitImageInfo, sizeof(VkBlitImageInfo2KHR));
   VkBlitImageInfo2KHR *tmp_dst1 = (void *) cmd->u.blit_image2_khr.blit_image_info; (void) tmp_dst1;
   VkBlitImageInfo2KHR *tmp_src1 = (void *) pBlitImageInfo; (void) tmp_src1;   
   tmp_dst1->pRegions = vk_zalloc(queue->alloc, sizeof(*tmp_dst1->pRegions) * tmp_dst1->regionCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   memcpy(( VkImageBlit2KHR* )tmp_dst1->pRegions, tmp_src1->pRegions, sizeof(*tmp_dst1->pRegions) * tmp_dst1->regionCount);
   } else {
      cmd->u.blit_image2_khr.blit_image_info = NULL;
   }
}

void vk_enqueue_cmd_copy_buffer_to_image2_khr(struct vk_cmd_queue *queue
, const VkCopyBufferToImageInfo2KHR* pCopyBufferToImageInfo
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_COPY_BUFFER_TO_IMAGE2_KHR;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   if (pCopyBufferToImageInfo) {
      cmd->u.copy_buffer_to_image2_khr.copy_buffer_to_image_info = vk_zalloc(queue->alloc, sizeof(VkCopyBufferToImageInfo2KHR), 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
      memcpy((void*)cmd->u.copy_buffer_to_image2_khr.copy_buffer_to_image_info, pCopyBufferToImageInfo, sizeof(VkCopyBufferToImageInfo2KHR));
   VkCopyBufferToImageInfo2KHR *tmp_dst1 = (void *) cmd->u.copy_buffer_to_image2_khr.copy_buffer_to_image_info; (void) tmp_dst1;
   VkCopyBufferToImageInfo2KHR *tmp_src1 = (void *) pCopyBufferToImageInfo; (void) tmp_src1;   
   tmp_dst1->pRegions = vk_zalloc(queue->alloc, sizeof(*tmp_dst1->pRegions) * tmp_dst1->regionCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   memcpy(( VkBufferImageCopy2KHR* )tmp_dst1->pRegions, tmp_src1->pRegions, sizeof(*tmp_dst1->pRegions) * tmp_dst1->regionCount);
   } else {
      cmd->u.copy_buffer_to_image2_khr.copy_buffer_to_image_info = NULL;
   }
}

void vk_enqueue_cmd_copy_image_to_buffer2_khr(struct vk_cmd_queue *queue
, const VkCopyImageToBufferInfo2KHR* pCopyImageToBufferInfo
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_COPY_IMAGE_TO_BUFFER2_KHR;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   if (pCopyImageToBufferInfo) {
      cmd->u.copy_image_to_buffer2_khr.copy_image_to_buffer_info = vk_zalloc(queue->alloc, sizeof(VkCopyImageToBufferInfo2KHR), 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
      memcpy((void*)cmd->u.copy_image_to_buffer2_khr.copy_image_to_buffer_info, pCopyImageToBufferInfo, sizeof(VkCopyImageToBufferInfo2KHR));
   VkCopyImageToBufferInfo2KHR *tmp_dst1 = (void *) cmd->u.copy_image_to_buffer2_khr.copy_image_to_buffer_info; (void) tmp_dst1;
   VkCopyImageToBufferInfo2KHR *tmp_src1 = (void *) pCopyImageToBufferInfo; (void) tmp_src1;   
   tmp_dst1->pRegions = vk_zalloc(queue->alloc, sizeof(*tmp_dst1->pRegions) * tmp_dst1->regionCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   memcpy(( VkBufferImageCopy2KHR* )tmp_dst1->pRegions, tmp_src1->pRegions, sizeof(*tmp_dst1->pRegions) * tmp_dst1->regionCount);
   } else {
      cmd->u.copy_image_to_buffer2_khr.copy_image_to_buffer_info = NULL;
   }
}

void vk_enqueue_cmd_resolve_image2_khr(struct vk_cmd_queue *queue
, const VkResolveImageInfo2KHR* pResolveImageInfo
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_RESOLVE_IMAGE2_KHR;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   if (pResolveImageInfo) {
      cmd->u.resolve_image2_khr.resolve_image_info = vk_zalloc(queue->alloc, sizeof(VkResolveImageInfo2KHR), 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
      memcpy((void*)cmd->u.resolve_image2_khr.resolve_image_info, pResolveImageInfo, sizeof(VkResolveImageInfo2KHR));
   VkResolveImageInfo2KHR *tmp_dst1 = (void *) cmd->u.resolve_image2_khr.resolve_image_info; (void) tmp_dst1;
   VkResolveImageInfo2KHR *tmp_src1 = (void *) pResolveImageInfo; (void) tmp_src1;   
   tmp_dst1->pRegions = vk_zalloc(queue->alloc, sizeof(*tmp_dst1->pRegions) * tmp_dst1->regionCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   memcpy(( VkImageResolve2KHR* )tmp_dst1->pRegions, tmp_src1->pRegions, sizeof(*tmp_dst1->pRegions) * tmp_dst1->regionCount);
   } else {
      cmd->u.resolve_image2_khr.resolve_image_info = NULL;
   }
}

void vk_enqueue_cmd_set_fragment_shading_rate_khr(struct vk_cmd_queue *queue
, const VkExtent2D*                           pFragmentSize
, const VkFragmentShadingRateCombinerOpKHR    combinerOps[2]
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_SET_FRAGMENT_SHADING_RATE_KHR;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   if (pFragmentSize) {
      cmd->u.set_fragment_shading_rate_khr.fragment_size = vk_zalloc(queue->alloc, sizeof(VkExtent2D), 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
      memcpy((void*)cmd->u.set_fragment_shading_rate_khr.fragment_size, pFragmentSize, sizeof(VkExtent2D));
   VkExtent2D *tmp_dst1 = (void *) cmd->u.set_fragment_shading_rate_khr.fragment_size; (void) tmp_dst1;
   VkExtent2D *tmp_src1 = (void *) pFragmentSize; (void) tmp_src1;   
      } else {
      cmd->u.set_fragment_shading_rate_khr.fragment_size = NULL;
   }
   memcpy(cmd->u.set_fragment_shading_rate_khr.combiner_ops, combinerOps,
          sizeof(*combinerOps) * 2);
}

void vk_enqueue_cmd_set_fragment_shading_rate_enum_nv(struct vk_cmd_queue *queue
, VkFragmentShadingRateNV                     shadingRate
, const VkFragmentShadingRateCombinerOpKHR    combinerOps[2]
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_SET_FRAGMENT_SHADING_RATE_ENUM_NV;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   cmd->u.set_fragment_shading_rate_enum_nv.shading_rate = shadingRate;
   memcpy(cmd->u.set_fragment_shading_rate_enum_nv.combiner_ops, combinerOps,
          sizeof(*combinerOps) * 2);
}

void vk_enqueue_cmd_set_vertex_input_ext(struct vk_cmd_queue *queue
, uint32_t vertexBindingDescriptionCount
, const VkVertexInputBindingDescription2EXT* pVertexBindingDescriptions
, uint32_t vertexAttributeDescriptionCount
, const VkVertexInputAttributeDescription2EXT* pVertexAttributeDescriptions
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_SET_VERTEX_INPUT_EXT;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   cmd->u.set_vertex_input_ext.vertex_binding_description_count = vertexBindingDescriptionCount;
   if (pVertexBindingDescriptions) {
      cmd->u.set_vertex_input_ext.vertex_binding_descriptions = vk_zalloc(queue->alloc, sizeof(*cmd->u.set_vertex_input_ext.vertex_binding_descriptions) * vertexBindingDescriptionCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   memcpy(( VkVertexInputBindingDescription2EXT* )cmd->u.set_vertex_input_ext.vertex_binding_descriptions, pVertexBindingDescriptions, sizeof(*cmd->u.set_vertex_input_ext.vertex_binding_descriptions) * vertexBindingDescriptionCount);
   }
   cmd->u.set_vertex_input_ext.vertex_attribute_description_count = vertexAttributeDescriptionCount;
   if (pVertexAttributeDescriptions) {
      cmd->u.set_vertex_input_ext.vertex_attribute_descriptions = vk_zalloc(queue->alloc, sizeof(*cmd->u.set_vertex_input_ext.vertex_attribute_descriptions) * vertexAttributeDescriptionCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   memcpy(( VkVertexInputAttributeDescription2EXT* )cmd->u.set_vertex_input_ext.vertex_attribute_descriptions, pVertexAttributeDescriptions, sizeof(*cmd->u.set_vertex_input_ext.vertex_attribute_descriptions) * vertexAttributeDescriptionCount);
   }
}

void vk_enqueue_cmd_set_color_write_enable_ext(struct vk_cmd_queue *queue
, uint32_t                                attachmentCount
, const VkBool32*   pColorWriteEnables
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_SET_COLOR_WRITE_ENABLE_EXT;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   cmd->u.set_color_write_enable_ext.attachment_count = attachmentCount;
   if (pColorWriteEnables) {
      cmd->u.set_color_write_enable_ext.color_write_enables = vk_zalloc(queue->alloc, sizeof(*cmd->u.set_color_write_enable_ext.color_write_enables) * attachmentCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   memcpy(( VkBool32*   )cmd->u.set_color_write_enable_ext.color_write_enables, pColorWriteEnables, sizeof(*cmd->u.set_color_write_enable_ext.color_write_enables) * attachmentCount);
   }
}

void vk_enqueue_cmd_set_event2_khr(struct vk_cmd_queue *queue
, VkEvent                                             event
, const VkDependencyInfoKHR*                          pDependencyInfo
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_SET_EVENT2_KHR;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   cmd->u.set_event2_khr.event = event;
   if (pDependencyInfo) {
      cmd->u.set_event2_khr.dependency_info = vk_zalloc(queue->alloc, sizeof(VkDependencyInfoKHR), 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
      memcpy((void*)cmd->u.set_event2_khr.dependency_info, pDependencyInfo, sizeof(VkDependencyInfoKHR));
   VkDependencyInfoKHR *tmp_dst1 = (void *) cmd->u.set_event2_khr.dependency_info; (void) tmp_dst1;
   VkDependencyInfoKHR *tmp_src1 = (void *) pDependencyInfo; (void) tmp_src1;   
   tmp_dst1->pMemoryBarriers = vk_zalloc(queue->alloc, sizeof(*tmp_dst1->pMemoryBarriers) * tmp_dst1->memoryBarrierCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   memcpy(( VkMemoryBarrier2KHR* )tmp_dst1->pMemoryBarriers, tmp_src1->pMemoryBarriers, sizeof(*tmp_dst1->pMemoryBarriers) * tmp_dst1->memoryBarrierCount);
tmp_dst1->pBufferMemoryBarriers = vk_zalloc(queue->alloc, sizeof(*tmp_dst1->pBufferMemoryBarriers) * tmp_dst1->bufferMemoryBarrierCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   memcpy(( VkBufferMemoryBarrier2KHR* )tmp_dst1->pBufferMemoryBarriers, tmp_src1->pBufferMemoryBarriers, sizeof(*tmp_dst1->pBufferMemoryBarriers) * tmp_dst1->bufferMemoryBarrierCount);
tmp_dst1->pImageMemoryBarriers = vk_zalloc(queue->alloc, sizeof(*tmp_dst1->pImageMemoryBarriers) * tmp_dst1->imageMemoryBarrierCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   memcpy(( VkImageMemoryBarrier2KHR* )tmp_dst1->pImageMemoryBarriers, tmp_src1->pImageMemoryBarriers, sizeof(*tmp_dst1->pImageMemoryBarriers) * tmp_dst1->imageMemoryBarrierCount);
   } else {
      cmd->u.set_event2_khr.dependency_info = NULL;
   }
}

void vk_enqueue_cmd_reset_event2_khr(struct vk_cmd_queue *queue
, VkEvent                                             event
, VkPipelineStageFlags2KHR                            stageMask
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_RESET_EVENT2_KHR;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   cmd->u.reset_event2_khr.event = event;
   cmd->u.reset_event2_khr.stage_mask = stageMask;
}

void vk_enqueue_cmd_wait_events2_khr(struct vk_cmd_queue *queue
, uint32_t                                            eventCount
, const VkEvent*                     pEvents
, const VkDependencyInfoKHR*         pDependencyInfos
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_WAIT_EVENTS2_KHR;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   cmd->u.wait_events2_khr.event_count = eventCount;
   if (pEvents) {
      cmd->u.wait_events2_khr.events = vk_zalloc(queue->alloc, sizeof(*cmd->u.wait_events2_khr.events) * eventCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   memcpy(( VkEvent*                     )cmd->u.wait_events2_khr.events, pEvents, sizeof(*cmd->u.wait_events2_khr.events) * eventCount);
   }
   if (pDependencyInfos) {
      cmd->u.wait_events2_khr.dependency_infos = vk_zalloc(queue->alloc, sizeof(*cmd->u.wait_events2_khr.dependency_infos) * eventCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   memcpy(( VkDependencyInfoKHR*         )cmd->u.wait_events2_khr.dependency_infos, pDependencyInfos, sizeof(*cmd->u.wait_events2_khr.dependency_infos) * eventCount);
   }
}

void vk_enqueue_cmd_pipeline_barrier2_khr(struct vk_cmd_queue *queue
, const VkDependencyInfoKHR*                                pDependencyInfo
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_PIPELINE_BARRIER2_KHR;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   if (pDependencyInfo) {
      cmd->u.pipeline_barrier2_khr.dependency_info = vk_zalloc(queue->alloc, sizeof(VkDependencyInfoKHR), 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
      memcpy((void*)cmd->u.pipeline_barrier2_khr.dependency_info, pDependencyInfo, sizeof(VkDependencyInfoKHR));
   VkDependencyInfoKHR *tmp_dst1 = (void *) cmd->u.pipeline_barrier2_khr.dependency_info; (void) tmp_dst1;
   VkDependencyInfoKHR *tmp_src1 = (void *) pDependencyInfo; (void) tmp_src1;   
   tmp_dst1->pMemoryBarriers = vk_zalloc(queue->alloc, sizeof(*tmp_dst1->pMemoryBarriers) * tmp_dst1->memoryBarrierCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   memcpy(( VkMemoryBarrier2KHR* )tmp_dst1->pMemoryBarriers, tmp_src1->pMemoryBarriers, sizeof(*tmp_dst1->pMemoryBarriers) * tmp_dst1->memoryBarrierCount);
tmp_dst1->pBufferMemoryBarriers = vk_zalloc(queue->alloc, sizeof(*tmp_dst1->pBufferMemoryBarriers) * tmp_dst1->bufferMemoryBarrierCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   memcpy(( VkBufferMemoryBarrier2KHR* )tmp_dst1->pBufferMemoryBarriers, tmp_src1->pBufferMemoryBarriers, sizeof(*tmp_dst1->pBufferMemoryBarriers) * tmp_dst1->bufferMemoryBarrierCount);
tmp_dst1->pImageMemoryBarriers = vk_zalloc(queue->alloc, sizeof(*tmp_dst1->pImageMemoryBarriers) * tmp_dst1->imageMemoryBarrierCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   memcpy(( VkImageMemoryBarrier2KHR* )tmp_dst1->pImageMemoryBarriers, tmp_src1->pImageMemoryBarriers, sizeof(*tmp_dst1->pImageMemoryBarriers) * tmp_dst1->imageMemoryBarrierCount);
   } else {
      cmd->u.pipeline_barrier2_khr.dependency_info = NULL;
   }
}

void vk_enqueue_cmd_write_timestamp2_khr(struct vk_cmd_queue *queue
, VkPipelineStageFlags2KHR                            stage
, VkQueryPool                                         queryPool
, uint32_t                                            query
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_WRITE_TIMESTAMP2_KHR;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   cmd->u.write_timestamp2_khr.stage = stage;
   cmd->u.write_timestamp2_khr.query_pool = queryPool;
   cmd->u.write_timestamp2_khr.query = query;
}

void vk_enqueue_cmd_write_buffer_marker2_amd(struct vk_cmd_queue *queue
, VkPipelineStageFlags2KHR                            stage
, VkBuffer                                            dstBuffer
, VkDeviceSize                                        dstOffset
, uint32_t                                            marker
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_WRITE_BUFFER_MARKER2_AMD;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   cmd->u.write_buffer_marker2_amd.stage = stage;
   cmd->u.write_buffer_marker2_amd.dst_buffer = dstBuffer;
   cmd->u.write_buffer_marker2_amd.dst_offset = dstOffset;
   cmd->u.write_buffer_marker2_amd.marker = marker;
}

#ifdef VK_ENABLE_BETA_EXTENSIONS
void vk_enqueue_cmd_decode_video_khr(struct vk_cmd_queue *queue
, const VkVideoDecodeInfoKHR* pFrameInfo
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_DECODE_VIDEO_KHR;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   if (pFrameInfo) {
      cmd->u.decode_video_khr.frame_info = vk_zalloc(queue->alloc, sizeof(VkVideoDecodeInfoKHR), 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
      memcpy((void*)cmd->u.decode_video_khr.frame_info, pFrameInfo, sizeof(VkVideoDecodeInfoKHR));
   VkVideoDecodeInfoKHR *tmp_dst1 = (void *) cmd->u.decode_video_khr.frame_info; (void) tmp_dst1;
   VkVideoDecodeInfoKHR *tmp_src1 = (void *) pFrameInfo; (void) tmp_src1;   
   
      const VkBaseInStructure *pnext = tmp_dst1->pNext;
      if (pnext) {
         switch ((int32_t)pnext->sType) {
         
      case VK_STRUCTURE_TYPE_VIDEO_DECODE_H264_PICTURE_INFO_EXT:
         if (pnext) {
      tmp_dst1->pNext = vk_zalloc(queue->alloc, sizeof(VkVideoDecodeH264PictureInfoEXT), 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
      memcpy((void*)tmp_dst1->pNext, pnext, sizeof(VkVideoDecodeH264PictureInfoEXT));
   VkVideoDecodeH264PictureInfoEXT *tmp_dst2 = (void *) tmp_dst1->pNext; (void) tmp_dst2;
   VkVideoDecodeH264PictureInfoEXT *tmp_src2 = (void *) pnext; (void) tmp_src2;   
   
      const VkBaseInStructure *pnext = tmp_dst2->pNext;
      if (pnext) {
         switch ((int32_t)pnext->sType) {
         
      case VK_STRUCTURE_TYPE_VIDEO_DECODE_H264_MVC_EXT:
         if (pnext) {
      tmp_dst2->pNext = vk_zalloc(queue->alloc, sizeof(VkVideoDecodeH264MvcEXT), 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
      memcpy((void*)tmp_dst2->pNext, pnext, sizeof(VkVideoDecodeH264MvcEXT));
   VkVideoDecodeH264MvcEXT *tmp_dst3 = (void *) tmp_dst2->pNext; (void) tmp_dst3;
   VkVideoDecodeH264MvcEXT *tmp_src3 = (void *) pnext; (void) tmp_src3;   
      } else {
      tmp_dst2->pNext = NULL;
   }
         break;
      
         }
      }
      tmp_dst2->pSlicesDataOffsets = vk_zalloc(queue->alloc, sizeof(*tmp_dst2->pSlicesDataOffsets) * tmp_dst2->slicesCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   memcpy(( uint32_t*     )tmp_dst2->pSlicesDataOffsets, tmp_src2->pSlicesDataOffsets, sizeof(*tmp_dst2->pSlicesDataOffsets) * tmp_dst2->slicesCount);
   } else {
      tmp_dst1->pNext = NULL;
   }
         break;
      
      case VK_STRUCTURE_TYPE_VIDEO_DECODE_H265_PICTURE_INFO_EXT:
         if (pnext) {
      tmp_dst1->pNext = vk_zalloc(queue->alloc, sizeof(VkVideoDecodeH265PictureInfoEXT), 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
      memcpy((void*)tmp_dst1->pNext, pnext, sizeof(VkVideoDecodeH265PictureInfoEXT));
   VkVideoDecodeH265PictureInfoEXT *tmp_dst2 = (void *) tmp_dst1->pNext; (void) tmp_dst2;
   VkVideoDecodeH265PictureInfoEXT *tmp_src2 = (void *) pnext; (void) tmp_src2;   
   tmp_dst2->pSlicesDataOffsets = vk_zalloc(queue->alloc, sizeof(*tmp_dst2->pSlicesDataOffsets) * tmp_dst2->slicesCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   memcpy(( uint32_t*  )tmp_dst2->pSlicesDataOffsets, tmp_src2->pSlicesDataOffsets, sizeof(*tmp_dst2->pSlicesDataOffsets) * tmp_dst2->slicesCount);
   } else {
      tmp_dst1->pNext = NULL;
   }
         break;
      
         }
      }
      tmp_dst1->pReferenceSlots = vk_zalloc(queue->alloc, sizeof(*tmp_dst1->pReferenceSlots) * tmp_dst1->referenceSlotCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   memcpy(( VkVideoReferenceSlotKHR* )tmp_dst1->pReferenceSlots, tmp_src1->pReferenceSlots, sizeof(*tmp_dst1->pReferenceSlots) * tmp_dst1->referenceSlotCount);
   } else {
      cmd->u.decode_video_khr.frame_info = NULL;
   }
}
#endif // VK_ENABLE_BETA_EXTENSIONS

#ifdef VK_ENABLE_BETA_EXTENSIONS
void vk_enqueue_cmd_begin_video_coding_khr(struct vk_cmd_queue *queue
, const VkVideoBeginCodingInfoKHR* pBeginInfo
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_BEGIN_VIDEO_CODING_KHR;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   if (pBeginInfo) {
      cmd->u.begin_video_coding_khr.begin_info = vk_zalloc(queue->alloc, sizeof(VkVideoBeginCodingInfoKHR), 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
      memcpy((void*)cmd->u.begin_video_coding_khr.begin_info, pBeginInfo, sizeof(VkVideoBeginCodingInfoKHR));
   VkVideoBeginCodingInfoKHR *tmp_dst1 = (void *) cmd->u.begin_video_coding_khr.begin_info; (void) tmp_dst1;
   VkVideoBeginCodingInfoKHR *tmp_src1 = (void *) pBeginInfo; (void) tmp_src1;   
   tmp_dst1->pReferenceSlots = vk_zalloc(queue->alloc, sizeof(*tmp_dst1->pReferenceSlots) * tmp_dst1->referenceSlotCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   memcpy(( VkVideoReferenceSlotKHR* )tmp_dst1->pReferenceSlots, tmp_src1->pReferenceSlots, sizeof(*tmp_dst1->pReferenceSlots) * tmp_dst1->referenceSlotCount);
   } else {
      cmd->u.begin_video_coding_khr.begin_info = NULL;
   }
}
#endif // VK_ENABLE_BETA_EXTENSIONS

#ifdef VK_ENABLE_BETA_EXTENSIONS
void vk_enqueue_cmd_control_video_coding_khr(struct vk_cmd_queue *queue
, const VkVideoCodingControlInfoKHR* pCodingControlInfo
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_CONTROL_VIDEO_CODING_KHR;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   if (pCodingControlInfo) {
      cmd->u.control_video_coding_khr.coding_control_info = vk_zalloc(queue->alloc, sizeof(VkVideoCodingControlInfoKHR), 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
      memcpy((void*)cmd->u.control_video_coding_khr.coding_control_info, pCodingControlInfo, sizeof(VkVideoCodingControlInfoKHR));
   VkVideoCodingControlInfoKHR *tmp_dst1 = (void *) cmd->u.control_video_coding_khr.coding_control_info; (void) tmp_dst1;
   VkVideoCodingControlInfoKHR *tmp_src1 = (void *) pCodingControlInfo; (void) tmp_src1;   
   
      const VkBaseInStructure *pnext = tmp_dst1->pNext;
      if (pnext) {
         switch ((int32_t)pnext->sType) {
         
      case VK_STRUCTURE_TYPE_VIDEO_ENCODE_RATE_CONTROL_INFO_KHR:
         if (pnext) {
      tmp_dst1->pNext = vk_zalloc(queue->alloc, sizeof(VkVideoEncodeRateControlInfoKHR), 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
      memcpy((void*)tmp_dst1->pNext, pnext, sizeof(VkVideoEncodeRateControlInfoKHR));
   VkVideoEncodeRateControlInfoKHR *tmp_dst2 = (void *) tmp_dst1->pNext; (void) tmp_dst2;
   VkVideoEncodeRateControlInfoKHR *tmp_src2 = (void *) pnext; (void) tmp_src2;   
      } else {
      tmp_dst1->pNext = NULL;
   }
         break;
      
         }
      }
         } else {
      cmd->u.control_video_coding_khr.coding_control_info = NULL;
   }
}
#endif // VK_ENABLE_BETA_EXTENSIONS

#ifdef VK_ENABLE_BETA_EXTENSIONS
void vk_enqueue_cmd_end_video_coding_khr(struct vk_cmd_queue *queue
, const VkVideoEndCodingInfoKHR* pEndCodingInfo
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_END_VIDEO_CODING_KHR;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   if (pEndCodingInfo) {
      cmd->u.end_video_coding_khr.end_coding_info = vk_zalloc(queue->alloc, sizeof(VkVideoEndCodingInfoKHR), 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
      memcpy((void*)cmd->u.end_video_coding_khr.end_coding_info, pEndCodingInfo, sizeof(VkVideoEndCodingInfoKHR));
   VkVideoEndCodingInfoKHR *tmp_dst1 = (void *) cmd->u.end_video_coding_khr.end_coding_info; (void) tmp_dst1;
   VkVideoEndCodingInfoKHR *tmp_src1 = (void *) pEndCodingInfo; (void) tmp_src1;   
      } else {
      cmd->u.end_video_coding_khr.end_coding_info = NULL;
   }
}
#endif // VK_ENABLE_BETA_EXTENSIONS

#ifdef VK_ENABLE_BETA_EXTENSIONS
void vk_enqueue_cmd_encode_video_khr(struct vk_cmd_queue *queue
, const VkVideoEncodeInfoKHR* pEncodeInfo
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_ENCODE_VIDEO_KHR;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   if (pEncodeInfo) {
      cmd->u.encode_video_khr.encode_info = vk_zalloc(queue->alloc, sizeof(VkVideoEncodeInfoKHR), 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
      memcpy((void*)cmd->u.encode_video_khr.encode_info, pEncodeInfo, sizeof(VkVideoEncodeInfoKHR));
   VkVideoEncodeInfoKHR *tmp_dst1 = (void *) cmd->u.encode_video_khr.encode_info; (void) tmp_dst1;
   VkVideoEncodeInfoKHR *tmp_src1 = (void *) pEncodeInfo; (void) tmp_src1;   
   
      const VkBaseInStructure *pnext = tmp_dst1->pNext;
      if (pnext) {
         switch ((int32_t)pnext->sType) {
         
      case VK_STRUCTURE_TYPE_VIDEO_ENCODE_H264_VCL_FRAME_INFO_EXT:
         if (pnext) {
      tmp_dst1->pNext = vk_zalloc(queue->alloc, sizeof(VkVideoEncodeH264VclFrameInfoEXT), 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
      memcpy((void*)tmp_dst1->pNext, pnext, sizeof(VkVideoEncodeH264VclFrameInfoEXT));
   VkVideoEncodeH264VclFrameInfoEXT *tmp_dst2 = (void *) tmp_dst1->pNext; (void) tmp_dst2;
   VkVideoEncodeH264VclFrameInfoEXT *tmp_src2 = (void *) pnext; (void) tmp_src2;   
   tmp_dst2->pRefDefaultFinalList0Entries = vk_zalloc(queue->alloc, sizeof(*tmp_dst2->pRefDefaultFinalList0Entries) * tmp_dst2->refDefaultFinalList0EntryCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   memcpy(( VkVideoEncodeH264DpbSlotInfoEXT* )tmp_dst2->pRefDefaultFinalList0Entries, tmp_src2->pRefDefaultFinalList0Entries, sizeof(*tmp_dst2->pRefDefaultFinalList0Entries) * tmp_dst2->refDefaultFinalList0EntryCount);
tmp_dst2->pRefDefaultFinalList1Entries = vk_zalloc(queue->alloc, sizeof(*tmp_dst2->pRefDefaultFinalList1Entries) * tmp_dst2->refDefaultFinalList1EntryCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   memcpy(( VkVideoEncodeH264DpbSlotInfoEXT* )tmp_dst2->pRefDefaultFinalList1Entries, tmp_src2->pRefDefaultFinalList1Entries, sizeof(*tmp_dst2->pRefDefaultFinalList1Entries) * tmp_dst2->refDefaultFinalList1EntryCount);
tmp_dst2->pNaluSliceEntries = vk_zalloc(queue->alloc, sizeof(*tmp_dst2->pNaluSliceEntries) * tmp_dst2->naluSliceEntryCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   memcpy(( VkVideoEncodeH264NaluSliceEXT*     )tmp_dst2->pNaluSliceEntries, tmp_src2->pNaluSliceEntries, sizeof(*tmp_dst2->pNaluSliceEntries) * tmp_dst2->naluSliceEntryCount);
   } else {
      tmp_dst1->pNext = NULL;
   }
         break;
      
      case VK_STRUCTURE_TYPE_VIDEO_ENCODE_H264_EMIT_PICTURE_PARAMETERS_EXT:
         if (pnext) {
      tmp_dst1->pNext = vk_zalloc(queue->alloc, sizeof(VkVideoEncodeH264EmitPictureParametersEXT), 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
      memcpy((void*)tmp_dst1->pNext, pnext, sizeof(VkVideoEncodeH264EmitPictureParametersEXT));
   VkVideoEncodeH264EmitPictureParametersEXT *tmp_dst2 = (void *) tmp_dst1->pNext; (void) tmp_dst2;
   VkVideoEncodeH264EmitPictureParametersEXT *tmp_src2 = (void *) pnext; (void) tmp_src2;   
   tmp_dst2->ppsIdEntries = vk_zalloc(queue->alloc, sizeof(*tmp_dst2->ppsIdEntries) * tmp_dst2->ppsIdEntryCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   memcpy(( uint8_t*  )tmp_dst2->ppsIdEntries, tmp_src2->ppsIdEntries, sizeof(*tmp_dst2->ppsIdEntries) * tmp_dst2->ppsIdEntryCount);
   } else {
      tmp_dst1->pNext = NULL;
   }
         break;
      
         }
      }
      tmp_dst1->pReferenceSlots = vk_zalloc(queue->alloc, sizeof(*tmp_dst1->pReferenceSlots) * tmp_dst1->referenceSlotCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   memcpy(( VkVideoReferenceSlotKHR* )tmp_dst1->pReferenceSlots, tmp_src1->pReferenceSlots, sizeof(*tmp_dst1->pReferenceSlots) * tmp_dst1->referenceSlotCount);
   } else {
      cmd->u.encode_video_khr.encode_info = NULL;
   }
}
#endif // VK_ENABLE_BETA_EXTENSIONS

void vk_enqueue_cmd_cu_launch_kernel_nvx(struct vk_cmd_queue *queue
, const VkCuLaunchInfoNVX* pLaunchInfo
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   if (!cmd)
      return;

   cmd->type = VK_CMD_CU_LAUNCH_KERNEL_NVX;
   list_addtail(&cmd->cmd_link, &queue->cmds);

   if (pLaunchInfo) {
      cmd->u.cu_launch_kernel_nvx.launch_info = vk_zalloc(queue->alloc, sizeof(VkCuLaunchInfoNVX), 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
      memcpy((void*)cmd->u.cu_launch_kernel_nvx.launch_info, pLaunchInfo, sizeof(VkCuLaunchInfoNVX));
   VkCuLaunchInfoNVX *tmp_dst1 = (void *) cmd->u.cu_launch_kernel_nvx.launch_info; (void) tmp_dst1;
   VkCuLaunchInfoNVX *tmp_src1 = (void *) pLaunchInfo; (void) tmp_src1;   
   tmp_dst1->pParams = vk_zalloc(queue->alloc, sizeof(*tmp_dst1->pParams) * tmp_dst1->paramCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   memcpy(( void*  *    )tmp_dst1->pParams, tmp_src1->pParams, sizeof(*tmp_dst1->pParams) * tmp_dst1->paramCount);
tmp_dst1->pExtras = vk_zalloc(queue->alloc, sizeof(*tmp_dst1->pExtras) * tmp_dst1->extraCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
   memcpy(( void*  *    )tmp_dst1->pExtras, tmp_src1->pExtras, sizeof(*tmp_dst1->pExtras) * tmp_dst1->extraCount);
   } else {
      cmd->u.cu_launch_kernel_nvx.launch_info = NULL;
   }
}


void
vk_free_queue(struct vk_cmd_queue *queue)
{
   struct vk_cmd_queue_entry *tmp, *cmd;
   LIST_FOR_EACH_ENTRY_SAFE(cmd, tmp, &queue->cmds, cmd_link) {
      switch(cmd->type) {
      case VK_CMD_BIND_PIPELINE:
         break;
      case VK_CMD_SET_VIEWPORT:
   vk_free(queue->alloc, ( VkViewport* )cmd->u.set_viewport.viewports);
         break;
      case VK_CMD_SET_SCISSOR:
   vk_free(queue->alloc, ( VkRect2D* )cmd->u.set_scissor.scissors);
         break;
      case VK_CMD_SET_LINE_WIDTH:
         break;
      case VK_CMD_SET_DEPTH_BIAS:
         break;
      case VK_CMD_SET_BLEND_CONSTANTS:
         break;
      case VK_CMD_SET_DEPTH_BOUNDS:
         break;
      case VK_CMD_SET_STENCIL_COMPARE_MASK:
         break;
      case VK_CMD_SET_STENCIL_WRITE_MASK:
         break;
      case VK_CMD_SET_STENCIL_REFERENCE:
         break;
      case VK_CMD_BIND_DESCRIPTOR_SETS:
   vk_free(queue->alloc, ( VkDescriptorSet* )cmd->u.bind_descriptor_sets.descriptor_sets);
   vk_free(queue->alloc, ( uint32_t* )cmd->u.bind_descriptor_sets.dynamic_offsets);
         break;
      case VK_CMD_BIND_INDEX_BUFFER:
         break;
      case VK_CMD_BIND_VERTEX_BUFFERS:
   vk_free(queue->alloc, ( VkBuffer* )cmd->u.bind_vertex_buffers.buffers);
   vk_free(queue->alloc, ( VkDeviceSize* )cmd->u.bind_vertex_buffers.offsets);
         break;
      case VK_CMD_DRAW:
         break;
      case VK_CMD_DRAW_INDEXED:
         break;
      case VK_CMD_DRAW_MULTI_EXT:
   vk_free(queue->alloc, ( VkMultiDrawInfoEXT* )cmd->u.draw_multi_ext.vertex_info);
         break;
      case VK_CMD_DRAW_MULTI_INDEXED_EXT:
   vk_free(queue->alloc, ( VkMultiDrawIndexedInfoEXT* )cmd->u.draw_multi_indexed_ext.index_info);
         vk_free(queue->alloc, cmd->driver_data);
      vk_free(queue->alloc, ( int32_t* )cmd->u.draw_multi_indexed_ext.vertex_offset);

         break;
      case VK_CMD_DRAW_INDIRECT:
         break;
      case VK_CMD_DRAW_INDEXED_INDIRECT:
         break;
      case VK_CMD_DISPATCH:
         break;
      case VK_CMD_DISPATCH_INDIRECT:
         break;
      case VK_CMD_SUBPASS_SHADING_HUAWEI:
         break;
      case VK_CMD_COPY_BUFFER:
   vk_free(queue->alloc, ( VkBufferCopy* )cmd->u.copy_buffer.regions);
         break;
      case VK_CMD_COPY_IMAGE:
   vk_free(queue->alloc, ( VkImageCopy* )cmd->u.copy_image.regions);
         break;
      case VK_CMD_BLIT_IMAGE:
   vk_free(queue->alloc, ( VkImageBlit* )cmd->u.blit_image.regions);
         break;
      case VK_CMD_COPY_BUFFER_TO_IMAGE:
   vk_free(queue->alloc, ( VkBufferImageCopy* )cmd->u.copy_buffer_to_image.regions);
         break;
      case VK_CMD_COPY_IMAGE_TO_BUFFER:
   vk_free(queue->alloc, ( VkBufferImageCopy* )cmd->u.copy_image_to_buffer.regions);
         break;
      case VK_CMD_UPDATE_BUFFER:
   vk_free(queue->alloc, ( void* )cmd->u.update_buffer.data);
         break;
      case VK_CMD_FILL_BUFFER:
         break;
      case VK_CMD_CLEAR_COLOR_IMAGE:
         vk_free(queue->alloc, cmd->driver_data);
      vk_free(queue->alloc, ( VkClearColorValue* )cmd->u.clear_color_image.color);

   vk_free(queue->alloc, ( VkImageSubresourceRange* )cmd->u.clear_color_image.ranges);
         break;
      case VK_CMD_CLEAR_DEPTH_STENCIL_IMAGE:
         vk_free(queue->alloc, cmd->driver_data);
      vk_free(queue->alloc, ( VkClearDepthStencilValue* )cmd->u.clear_depth_stencil_image.depth_stencil);

   vk_free(queue->alloc, ( VkImageSubresourceRange* )cmd->u.clear_depth_stencil_image.ranges);
         break;
      case VK_CMD_CLEAR_ATTACHMENTS:
   vk_free(queue->alloc, ( VkClearAttachment* )cmd->u.clear_attachments.attachments);
   vk_free(queue->alloc, ( VkClearRect* )cmd->u.clear_attachments.rects);
         break;
      case VK_CMD_RESOLVE_IMAGE:
   vk_free(queue->alloc, ( VkImageResolve* )cmd->u.resolve_image.regions);
         break;
      case VK_CMD_SET_EVENT:
         break;
      case VK_CMD_RESET_EVENT:
         break;
      case VK_CMD_WAIT_EVENTS:
   vk_free(queue->alloc, ( VkEvent* )cmd->u.wait_events.events);
   vk_free(queue->alloc, ( VkMemoryBarrier* )cmd->u.wait_events.memory_barriers);
   vk_free(queue->alloc, ( VkBufferMemoryBarrier* )cmd->u.wait_events.buffer_memory_barriers);
   vk_free(queue->alloc, ( VkImageMemoryBarrier* )cmd->u.wait_events.image_memory_barriers);
         break;
      case VK_CMD_PIPELINE_BARRIER:
   vk_free(queue->alloc, ( VkMemoryBarrier* )cmd->u.pipeline_barrier.memory_barriers);
   vk_free(queue->alloc, ( VkBufferMemoryBarrier* )cmd->u.pipeline_barrier.buffer_memory_barriers);
   vk_free(queue->alloc, ( VkImageMemoryBarrier* )cmd->u.pipeline_barrier.image_memory_barriers);
         break;
      case VK_CMD_BEGIN_QUERY:
         break;
      case VK_CMD_END_QUERY:
         break;
      case VK_CMD_BEGIN_CONDITIONAL_RENDERING_EXT:
         vk_free(queue->alloc, cmd->driver_data);
      vk_free(queue->alloc, ( VkConditionalRenderingBeginInfoEXT* )cmd->u.begin_conditional_rendering_ext.conditional_rendering_begin);

         break;
      case VK_CMD_END_CONDITIONAL_RENDERING_EXT:
         break;
      case VK_CMD_RESET_QUERY_POOL:
         break;
      case VK_CMD_WRITE_TIMESTAMP:
         break;
      case VK_CMD_COPY_QUERY_POOL_RESULTS:
         break;
      case VK_CMD_PUSH_CONSTANTS:
   vk_free(queue->alloc, ( void* )cmd->u.push_constants.values);
         break;
      case VK_CMD_BEGIN_RENDER_PASS:
   vk_free(queue->alloc, ( VkClearValue*    )cmd->u.begin_render_pass.render_pass_begin->pClearValues);
      vk_free(queue->alloc, cmd->driver_data);
      vk_free(queue->alloc, ( VkRenderPassBeginInfo* )cmd->u.begin_render_pass.render_pass_begin);

         break;
      case VK_CMD_NEXT_SUBPASS:
         break;
      case VK_CMD_END_RENDER_PASS:
         break;
      case VK_CMD_EXECUTE_COMMANDS:
   vk_free(queue->alloc, ( VkCommandBuffer* )cmd->u.execute_commands.command_buffers);
         break;
      case VK_CMD_DEBUG_MARKER_BEGIN_EXT:
         vk_free(queue->alloc, cmd->driver_data);
      vk_free(queue->alloc, ( VkDebugMarkerMarkerInfoEXT* )cmd->u.debug_marker_begin_ext.marker_info);

         break;
      case VK_CMD_DEBUG_MARKER_END_EXT:
         break;
      case VK_CMD_DEBUG_MARKER_INSERT_EXT:
         vk_free(queue->alloc, cmd->driver_data);
      vk_free(queue->alloc, ( VkDebugMarkerMarkerInfoEXT* )cmd->u.debug_marker_insert_ext.marker_info);

         break;
      case VK_CMD_EXECUTE_GENERATED_COMMANDS_NV:
   vk_free(queue->alloc, ( VkIndirectCommandsStreamNV*  )cmd->u.execute_generated_commands_nv.generated_commands_info->pStreams);
      vk_free(queue->alloc, cmd->driver_data);
      vk_free(queue->alloc, ( VkGeneratedCommandsInfoNV* )cmd->u.execute_generated_commands_nv.generated_commands_info);

         break;
      case VK_CMD_PREPROCESS_GENERATED_COMMANDS_NV:
   vk_free(queue->alloc, ( VkIndirectCommandsStreamNV*  )cmd->u.preprocess_generated_commands_nv.generated_commands_info->pStreams);
      vk_free(queue->alloc, cmd->driver_data);
      vk_free(queue->alloc, ( VkGeneratedCommandsInfoNV* )cmd->u.preprocess_generated_commands_nv.generated_commands_info);

         break;
      case VK_CMD_BIND_PIPELINE_SHADER_GROUP_NV:
         break;
      case VK_CMD_PUSH_DESCRIPTOR_SET_KHR:
   vk_free(queue->alloc, ( VkWriteDescriptorSet* )cmd->u.push_descriptor_set_khr.descriptor_writes);
         break;
      case VK_CMD_SET_DEVICE_MASK:
         break;
      case VK_CMD_DISPATCH_BASE:
         break;
      case VK_CMD_PUSH_DESCRIPTOR_SET_WITH_TEMPLATE_KHR:
         vk_free(queue->alloc, cmd->driver_data);
      vk_free(queue->alloc, ( void* )cmd->u.push_descriptor_set_with_template_khr.data);

         break;
      case VK_CMD_SET_VIEWPORT_WSCALING_NV:
   vk_free(queue->alloc, ( VkViewportWScalingNV* )cmd->u.set_viewport_wscaling_nv.viewport_wscalings);
         break;
      case VK_CMD_SET_DISCARD_RECTANGLE_EXT:
   vk_free(queue->alloc, ( VkRect2D* )cmd->u.set_discard_rectangle_ext.discard_rectangles);
         break;
      case VK_CMD_SET_SAMPLE_LOCATIONS_EXT:
   vk_free(queue->alloc, ( VkSampleLocationEXT* )cmd->u.set_sample_locations_ext.sample_locations_info->pSampleLocations);
      vk_free(queue->alloc, cmd->driver_data);
      vk_free(queue->alloc, ( VkSampleLocationsInfoEXT* )cmd->u.set_sample_locations_ext.sample_locations_info);

         break;
      case VK_CMD_BEGIN_DEBUG_UTILS_LABEL_EXT:
         vk_free(queue->alloc, cmd->driver_data);
      vk_free(queue->alloc, ( VkDebugUtilsLabelEXT* )cmd->u.begin_debug_utils_label_ext.label_info);

         break;
      case VK_CMD_END_DEBUG_UTILS_LABEL_EXT:
         break;
      case VK_CMD_INSERT_DEBUG_UTILS_LABEL_EXT:
         vk_free(queue->alloc, cmd->driver_data);
      vk_free(queue->alloc, ( VkDebugUtilsLabelEXT* )cmd->u.insert_debug_utils_label_ext.label_info);

         break;
      case VK_CMD_WRITE_BUFFER_MARKER_AMD:
         break;
      case VK_CMD_BEGIN_RENDER_PASS2:
   vk_free(queue->alloc, ( VkClearValue*    )cmd->u.begin_render_pass2.render_pass_begin->pClearValues);
      vk_free(queue->alloc, cmd->driver_data);
      vk_free(queue->alloc, ( VkRenderPassBeginInfo*      )cmd->u.begin_render_pass2.render_pass_begin);

         vk_free(queue->alloc, cmd->driver_data);
      vk_free(queue->alloc, ( VkSubpassBeginInfo*      )cmd->u.begin_render_pass2.subpass_begin_info);

         break;
      case VK_CMD_NEXT_SUBPASS2:
         vk_free(queue->alloc, cmd->driver_data);
      vk_free(queue->alloc, ( VkSubpassBeginInfo*      )cmd->u.next_subpass2.subpass_begin_info);

         vk_free(queue->alloc, cmd->driver_data);
      vk_free(queue->alloc, ( VkSubpassEndInfo*        )cmd->u.next_subpass2.subpass_end_info);

         break;
      case VK_CMD_END_RENDER_PASS2:
         vk_free(queue->alloc, cmd->driver_data);
      vk_free(queue->alloc, ( VkSubpassEndInfo*        )cmd->u.end_render_pass2.subpass_end_info);

         break;
      case VK_CMD_DRAW_INDIRECT_COUNT:
         break;
      case VK_CMD_DRAW_INDEXED_INDIRECT_COUNT:
         break;
      case VK_CMD_SET_CHECKPOINT_NV:
         vk_free(queue->alloc, cmd->driver_data);
      vk_free(queue->alloc, ( void* )cmd->u.set_checkpoint_nv.checkpoint_marker);

         break;
      case VK_CMD_BIND_TRANSFORM_FEEDBACK_BUFFERS_EXT:
   vk_free(queue->alloc, ( VkBuffer* )cmd->u.bind_transform_feedback_buffers_ext.buffers);
   vk_free(queue->alloc, ( VkDeviceSize* )cmd->u.bind_transform_feedback_buffers_ext.offsets);
   vk_free(queue->alloc, ( VkDeviceSize* )cmd->u.bind_transform_feedback_buffers_ext.sizes);
         break;
      case VK_CMD_BEGIN_TRANSFORM_FEEDBACK_EXT:
   vk_free(queue->alloc, ( VkBuffer* )cmd->u.begin_transform_feedback_ext.counter_buffers);
   vk_free(queue->alloc, ( VkDeviceSize* )cmd->u.begin_transform_feedback_ext.counter_buffer_offsets);
         break;
      case VK_CMD_END_TRANSFORM_FEEDBACK_EXT:
   vk_free(queue->alloc, ( VkBuffer* )cmd->u.end_transform_feedback_ext.counter_buffers);
   vk_free(queue->alloc, ( VkDeviceSize* )cmd->u.end_transform_feedback_ext.counter_buffer_offsets);
         break;
      case VK_CMD_BEGIN_QUERY_INDEXED_EXT:
         break;
      case VK_CMD_END_QUERY_INDEXED_EXT:
         break;
      case VK_CMD_DRAW_INDIRECT_BYTE_COUNT_EXT:
         break;
      case VK_CMD_SET_EXCLUSIVE_SCISSOR_NV:
   vk_free(queue->alloc, ( VkRect2D* )cmd->u.set_exclusive_scissor_nv.exclusive_scissors);
         break;
      case VK_CMD_BIND_SHADING_RATE_IMAGE_NV:
         break;
      case VK_CMD_SET_VIEWPORT_SHADING_RATE_PALETTE_NV:
   vk_free(queue->alloc, ( VkShadingRatePaletteNV* )cmd->u.set_viewport_shading_rate_palette_nv.shading_rate_palettes);
         break;
      case VK_CMD_SET_COARSE_SAMPLE_ORDER_NV:
   vk_free(queue->alloc, ( VkCoarseSampleOrderCustomNV* )cmd->u.set_coarse_sample_order_nv.custom_sample_orders);
         break;
      case VK_CMD_DRAW_MESH_TASKS_NV:
         break;
      case VK_CMD_DRAW_MESH_TASKS_INDIRECT_NV:
         break;
      case VK_CMD_DRAW_MESH_TASKS_INDIRECT_COUNT_NV:
         break;
      case VK_CMD_BIND_INVOCATION_MASK_HUAWEI:
         break;
      case VK_CMD_COPY_ACCELERATION_STRUCTURE_NV:
         break;
      case VK_CMD_COPY_ACCELERATION_STRUCTURE_KHR:
         vk_free(queue->alloc, cmd->driver_data);
      vk_free(queue->alloc, ( VkCopyAccelerationStructureInfoKHR* )cmd->u.copy_acceleration_structure_khr.info);

         break;
      case VK_CMD_COPY_ACCELERATION_STRUCTURE_TO_MEMORY_KHR:
         vk_free(queue->alloc, cmd->driver_data);
      vk_free(queue->alloc, ( VkCopyAccelerationStructureToMemoryInfoKHR* )cmd->u.copy_acceleration_structure_to_memory_khr.info);

         break;
      case VK_CMD_COPY_MEMORY_TO_ACCELERATION_STRUCTURE_KHR:
         vk_free(queue->alloc, cmd->driver_data);
      vk_free(queue->alloc, ( VkCopyMemoryToAccelerationStructureInfoKHR* )cmd->u.copy_memory_to_acceleration_structure_khr.info);

         break;
      case VK_CMD_WRITE_ACCELERATION_STRUCTURES_PROPERTIES_KHR:
   vk_free(queue->alloc, ( VkAccelerationStructureKHR* )cmd->u.write_acceleration_structures_properties_khr.acceleration_structures);
         break;
      case VK_CMD_WRITE_ACCELERATION_STRUCTURES_PROPERTIES_NV:
   vk_free(queue->alloc, ( VkAccelerationStructureNV* )cmd->u.write_acceleration_structures_properties_nv.acceleration_structures);
         break;
      case VK_CMD_BUILD_ACCELERATION_STRUCTURE_NV:
   vk_free(queue->alloc, ( VkGeometryNV* )cmd->u.build_acceleration_structure_nv.info->pGeometries);
      vk_free(queue->alloc, cmd->driver_data);
      vk_free(queue->alloc, ( VkAccelerationStructureInfoNV* )cmd->u.build_acceleration_structure_nv.info);

         break;
      case VK_CMD_TRACE_RAYS_KHR:
         vk_free(queue->alloc, cmd->driver_data);
      vk_free(queue->alloc, ( VkStridedDeviceAddressRegionKHR* )cmd->u.trace_rays_khr.raygen_shader_binding_table);

         vk_free(queue->alloc, cmd->driver_data);
      vk_free(queue->alloc, ( VkStridedDeviceAddressRegionKHR* )cmd->u.trace_rays_khr.miss_shader_binding_table);

         vk_free(queue->alloc, cmd->driver_data);
      vk_free(queue->alloc, ( VkStridedDeviceAddressRegionKHR* )cmd->u.trace_rays_khr.hit_shader_binding_table);

         vk_free(queue->alloc, cmd->driver_data);
      vk_free(queue->alloc, ( VkStridedDeviceAddressRegionKHR* )cmd->u.trace_rays_khr.callable_shader_binding_table);

         break;
      case VK_CMD_TRACE_RAYS_NV:
         break;
      case VK_CMD_TRACE_RAYS_INDIRECT_KHR:
         vk_free(queue->alloc, cmd->driver_data);
      vk_free(queue->alloc, ( VkStridedDeviceAddressRegionKHR* )cmd->u.trace_rays_indirect_khr.raygen_shader_binding_table);

         vk_free(queue->alloc, cmd->driver_data);
      vk_free(queue->alloc, ( VkStridedDeviceAddressRegionKHR* )cmd->u.trace_rays_indirect_khr.miss_shader_binding_table);

         vk_free(queue->alloc, cmd->driver_data);
      vk_free(queue->alloc, ( VkStridedDeviceAddressRegionKHR* )cmd->u.trace_rays_indirect_khr.hit_shader_binding_table);

         vk_free(queue->alloc, cmd->driver_data);
      vk_free(queue->alloc, ( VkStridedDeviceAddressRegionKHR* )cmd->u.trace_rays_indirect_khr.callable_shader_binding_table);

         break;
      case VK_CMD_SET_RAY_TRACING_PIPELINE_STACK_SIZE_KHR:
         break;
      case VK_CMD_SET_PERFORMANCE_MARKER_INTEL:
         vk_free(queue->alloc, cmd->driver_data);
      vk_free(queue->alloc, ( VkPerformanceMarkerInfoINTEL* )cmd->u.set_performance_marker_intel.marker_info);

         break;
      case VK_CMD_SET_PERFORMANCE_STREAM_MARKER_INTEL:
         vk_free(queue->alloc, cmd->driver_data);
      vk_free(queue->alloc, ( VkPerformanceStreamMarkerInfoINTEL* )cmd->u.set_performance_stream_marker_intel.marker_info);

         break;
      case VK_CMD_SET_PERFORMANCE_OVERRIDE_INTEL:
         vk_free(queue->alloc, cmd->driver_data);
      vk_free(queue->alloc, ( VkPerformanceOverrideInfoINTEL* )cmd->u.set_performance_override_intel.override_info);

         break;
      case VK_CMD_SET_LINE_STIPPLE_EXT:
         break;
      case VK_CMD_BUILD_ACCELERATION_STRUCTURES_KHR:
   vk_free(queue->alloc, ( VkAccelerationStructureBuildGeometryInfoKHR* )cmd->u.build_acceleration_structures_khr.infos);
   vk_free(queue->alloc, ( VkAccelerationStructureBuildRangeInfoKHR* * )cmd->u.build_acceleration_structures_khr.pp_build_range_infos);
         break;
      case VK_CMD_BUILD_ACCELERATION_STRUCTURES_INDIRECT_KHR:
   vk_free(queue->alloc, ( VkAccelerationStructureBuildGeometryInfoKHR* )cmd->u.build_acceleration_structures_indirect_khr.infos);
   vk_free(queue->alloc, ( VkDeviceAddress*             )cmd->u.build_acceleration_structures_indirect_khr.indirect_device_addresses);
   vk_free(queue->alloc, ( uint32_t*                    )cmd->u.build_acceleration_structures_indirect_khr.indirect_strides);
   vk_free(queue->alloc, ( uint32_t* *             )cmd->u.build_acceleration_structures_indirect_khr.pp_max_primitive_counts);
         break;
      case VK_CMD_SET_CULL_MODE_EXT:
         break;
      case VK_CMD_SET_FRONT_FACE_EXT:
         break;
      case VK_CMD_SET_PRIMITIVE_TOPOLOGY_EXT:
         break;
      case VK_CMD_SET_VIEWPORT_WITH_COUNT_EXT:
   vk_free(queue->alloc, ( VkViewport* )cmd->u.set_viewport_with_count_ext.viewports);
         break;
      case VK_CMD_SET_SCISSOR_WITH_COUNT_EXT:
   vk_free(queue->alloc, ( VkRect2D* )cmd->u.set_scissor_with_count_ext.scissors);
         break;
      case VK_CMD_BIND_VERTEX_BUFFERS2_EXT:
   vk_free(queue->alloc, ( VkBuffer* )cmd->u.bind_vertex_buffers2_ext.buffers);
   vk_free(queue->alloc, ( VkDeviceSize* )cmd->u.bind_vertex_buffers2_ext.offsets);
   vk_free(queue->alloc, ( VkDeviceSize* )cmd->u.bind_vertex_buffers2_ext.sizes);
   vk_free(queue->alloc, ( VkDeviceSize* )cmd->u.bind_vertex_buffers2_ext.strides);
         break;
      case VK_CMD_SET_DEPTH_TEST_ENABLE_EXT:
         break;
      case VK_CMD_SET_DEPTH_WRITE_ENABLE_EXT:
         break;
      case VK_CMD_SET_DEPTH_COMPARE_OP_EXT:
         break;
      case VK_CMD_SET_DEPTH_BOUNDS_TEST_ENABLE_EXT:
         break;
      case VK_CMD_SET_STENCIL_TEST_ENABLE_EXT:
         break;
      case VK_CMD_SET_STENCIL_OP_EXT:
         break;
      case VK_CMD_SET_PATCH_CONTROL_POINTS_EXT:
         break;
      case VK_CMD_SET_RASTERIZER_DISCARD_ENABLE_EXT:
         break;
      case VK_CMD_SET_DEPTH_BIAS_ENABLE_EXT:
         break;
      case VK_CMD_SET_LOGIC_OP_EXT:
         break;
      case VK_CMD_SET_PRIMITIVE_RESTART_ENABLE_EXT:
         break;
      case VK_CMD_COPY_BUFFER2_KHR:
   vk_free(queue->alloc, ( VkBufferCopy2KHR* )cmd->u.copy_buffer2_khr.copy_buffer_info->pRegions);
      vk_free(queue->alloc, cmd->driver_data);
      vk_free(queue->alloc, ( VkCopyBufferInfo2KHR* )cmd->u.copy_buffer2_khr.copy_buffer_info);

         break;
      case VK_CMD_COPY_IMAGE2_KHR:
   vk_free(queue->alloc, ( VkImageCopy2KHR* )cmd->u.copy_image2_khr.copy_image_info->pRegions);
      vk_free(queue->alloc, cmd->driver_data);
      vk_free(queue->alloc, ( VkCopyImageInfo2KHR* )cmd->u.copy_image2_khr.copy_image_info);

         break;
      case VK_CMD_BLIT_IMAGE2_KHR:
   vk_free(queue->alloc, ( VkImageBlit2KHR* )cmd->u.blit_image2_khr.blit_image_info->pRegions);
      vk_free(queue->alloc, cmd->driver_data);
      vk_free(queue->alloc, ( VkBlitImageInfo2KHR* )cmd->u.blit_image2_khr.blit_image_info);

         break;
      case VK_CMD_COPY_BUFFER_TO_IMAGE2_KHR:
   vk_free(queue->alloc, ( VkBufferImageCopy2KHR* )cmd->u.copy_buffer_to_image2_khr.copy_buffer_to_image_info->pRegions);
      vk_free(queue->alloc, cmd->driver_data);
      vk_free(queue->alloc, ( VkCopyBufferToImageInfo2KHR* )cmd->u.copy_buffer_to_image2_khr.copy_buffer_to_image_info);

         break;
      case VK_CMD_COPY_IMAGE_TO_BUFFER2_KHR:
   vk_free(queue->alloc, ( VkBufferImageCopy2KHR* )cmd->u.copy_image_to_buffer2_khr.copy_image_to_buffer_info->pRegions);
      vk_free(queue->alloc, cmd->driver_data);
      vk_free(queue->alloc, ( VkCopyImageToBufferInfo2KHR* )cmd->u.copy_image_to_buffer2_khr.copy_image_to_buffer_info);

         break;
      case VK_CMD_RESOLVE_IMAGE2_KHR:
   vk_free(queue->alloc, ( VkImageResolve2KHR* )cmd->u.resolve_image2_khr.resolve_image_info->pRegions);
      vk_free(queue->alloc, cmd->driver_data);
      vk_free(queue->alloc, ( VkResolveImageInfo2KHR* )cmd->u.resolve_image2_khr.resolve_image_info);

         break;
      case VK_CMD_SET_FRAGMENT_SHADING_RATE_KHR:
         vk_free(queue->alloc, cmd->driver_data);
      vk_free(queue->alloc, ( VkExtent2D*                           )cmd->u.set_fragment_shading_rate_khr.fragment_size);

         break;
      case VK_CMD_SET_FRAGMENT_SHADING_RATE_ENUM_NV:
         break;
      case VK_CMD_SET_VERTEX_INPUT_EXT:
   vk_free(queue->alloc, ( VkVertexInputBindingDescription2EXT* )cmd->u.set_vertex_input_ext.vertex_binding_descriptions);
   vk_free(queue->alloc, ( VkVertexInputAttributeDescription2EXT* )cmd->u.set_vertex_input_ext.vertex_attribute_descriptions);
         break;
      case VK_CMD_SET_COLOR_WRITE_ENABLE_EXT:
   vk_free(queue->alloc, ( VkBool32*   )cmd->u.set_color_write_enable_ext.color_write_enables);
         break;
      case VK_CMD_SET_EVENT2_KHR:
   vk_free(queue->alloc, ( VkMemoryBarrier2KHR* )cmd->u.set_event2_khr.dependency_info->pMemoryBarriers);
vk_free(queue->alloc, ( VkBufferMemoryBarrier2KHR* )cmd->u.set_event2_khr.dependency_info->pBufferMemoryBarriers);
vk_free(queue->alloc, ( VkImageMemoryBarrier2KHR* )cmd->u.set_event2_khr.dependency_info->pImageMemoryBarriers);
      vk_free(queue->alloc, cmd->driver_data);
      vk_free(queue->alloc, ( VkDependencyInfoKHR*                          )cmd->u.set_event2_khr.dependency_info);

         break;
      case VK_CMD_RESET_EVENT2_KHR:
         break;
      case VK_CMD_WAIT_EVENTS2_KHR:
   vk_free(queue->alloc, ( VkEvent*                     )cmd->u.wait_events2_khr.events);
   vk_free(queue->alloc, ( VkDependencyInfoKHR*         )cmd->u.wait_events2_khr.dependency_infos);
         break;
      case VK_CMD_PIPELINE_BARRIER2_KHR:
   vk_free(queue->alloc, ( VkMemoryBarrier2KHR* )cmd->u.pipeline_barrier2_khr.dependency_info->pMemoryBarriers);
vk_free(queue->alloc, ( VkBufferMemoryBarrier2KHR* )cmd->u.pipeline_barrier2_khr.dependency_info->pBufferMemoryBarriers);
vk_free(queue->alloc, ( VkImageMemoryBarrier2KHR* )cmd->u.pipeline_barrier2_khr.dependency_info->pImageMemoryBarriers);
      vk_free(queue->alloc, cmd->driver_data);
      vk_free(queue->alloc, ( VkDependencyInfoKHR*                                )cmd->u.pipeline_barrier2_khr.dependency_info);

         break;
      case VK_CMD_WRITE_TIMESTAMP2_KHR:
         break;
      case VK_CMD_WRITE_BUFFER_MARKER2_AMD:
         break;
#ifdef VK_ENABLE_BETA_EXTENSIONS
      case VK_CMD_DECODE_VIDEO_KHR:
   vk_free(queue->alloc, ( VkVideoReferenceSlotKHR* )cmd->u.decode_video_khr.frame_info->pReferenceSlots);
      vk_free(queue->alloc, cmd->driver_data);
      vk_free(queue->alloc, ( VkVideoDecodeInfoKHR* )cmd->u.decode_video_khr.frame_info);

         break;
#endif // VK_ENABLE_BETA_EXTENSIONS
#ifdef VK_ENABLE_BETA_EXTENSIONS
      case VK_CMD_BEGIN_VIDEO_CODING_KHR:
   vk_free(queue->alloc, ( VkVideoReferenceSlotKHR* )cmd->u.begin_video_coding_khr.begin_info->pReferenceSlots);
      vk_free(queue->alloc, cmd->driver_data);
      vk_free(queue->alloc, ( VkVideoBeginCodingInfoKHR* )cmd->u.begin_video_coding_khr.begin_info);

         break;
#endif // VK_ENABLE_BETA_EXTENSIONS
#ifdef VK_ENABLE_BETA_EXTENSIONS
      case VK_CMD_CONTROL_VIDEO_CODING_KHR:
         vk_free(queue->alloc, cmd->driver_data);
      vk_free(queue->alloc, ( VkVideoCodingControlInfoKHR* )cmd->u.control_video_coding_khr.coding_control_info);

         break;
#endif // VK_ENABLE_BETA_EXTENSIONS
#ifdef VK_ENABLE_BETA_EXTENSIONS
      case VK_CMD_END_VIDEO_CODING_KHR:
         vk_free(queue->alloc, cmd->driver_data);
      vk_free(queue->alloc, ( VkVideoEndCodingInfoKHR* )cmd->u.end_video_coding_khr.end_coding_info);

         break;
#endif // VK_ENABLE_BETA_EXTENSIONS
#ifdef VK_ENABLE_BETA_EXTENSIONS
      case VK_CMD_ENCODE_VIDEO_KHR:
   vk_free(queue->alloc, ( VkVideoReferenceSlotKHR* )cmd->u.encode_video_khr.encode_info->pReferenceSlots);
      vk_free(queue->alloc, cmd->driver_data);
      vk_free(queue->alloc, ( VkVideoEncodeInfoKHR* )cmd->u.encode_video_khr.encode_info);

         break;
#endif // VK_ENABLE_BETA_EXTENSIONS
      case VK_CMD_CU_LAUNCH_KERNEL_NVX:
   vk_free(queue->alloc, ( void*  *    )cmd->u.cu_launch_kernel_nvx.launch_info->pParams);
vk_free(queue->alloc, ( void*  *    )cmd->u.cu_launch_kernel_nvx.launch_info->pExtras);
      vk_free(queue->alloc, cmd->driver_data);
      vk_free(queue->alloc, ( VkCuLaunchInfoNVX* )cmd->u.cu_launch_kernel_nvx.launch_info);

         break;
      }
      vk_free(queue->alloc, cmd);
   }
}

