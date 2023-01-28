
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
#include "vk_cmd_enqueue_entrypoints.h"
#include "vk_command_buffer.h"
#include "vk_dispatch_table.h"
#include "vk_device.h"

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
   "VK_CMD_DRAW_MESH_TASKS_EXT",
   "VK_CMD_DRAW_MESH_TASKS_INDIRECT_EXT",
   "VK_CMD_DRAW_MESH_TASKS_INDIRECT_COUNT_EXT",
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
   "VK_CMD_TRACE_RAYS_INDIRECT2_KHR",
   "VK_CMD_SET_RAY_TRACING_PIPELINE_STACK_SIZE_KHR",
   "VK_CMD_SET_PERFORMANCE_MARKER_INTEL",
   "VK_CMD_SET_PERFORMANCE_STREAM_MARKER_INTEL",
   "VK_CMD_SET_PERFORMANCE_OVERRIDE_INTEL",
   "VK_CMD_SET_LINE_STIPPLE_EXT",
   "VK_CMD_BUILD_ACCELERATION_STRUCTURES_KHR",
   "VK_CMD_BUILD_ACCELERATION_STRUCTURES_INDIRECT_KHR",
   "VK_CMD_SET_CULL_MODE",
   "VK_CMD_SET_FRONT_FACE",
   "VK_CMD_SET_PRIMITIVE_TOPOLOGY",
   "VK_CMD_SET_VIEWPORT_WITH_COUNT",
   "VK_CMD_SET_SCISSOR_WITH_COUNT",
   "VK_CMD_BIND_VERTEX_BUFFERS2",
   "VK_CMD_SET_DEPTH_TEST_ENABLE",
   "VK_CMD_SET_DEPTH_WRITE_ENABLE",
   "VK_CMD_SET_DEPTH_COMPARE_OP",
   "VK_CMD_SET_DEPTH_BOUNDS_TEST_ENABLE",
   "VK_CMD_SET_STENCIL_TEST_ENABLE",
   "VK_CMD_SET_STENCIL_OP",
   "VK_CMD_SET_PATCH_CONTROL_POINTS_EXT",
   "VK_CMD_SET_RASTERIZER_DISCARD_ENABLE",
   "VK_CMD_SET_DEPTH_BIAS_ENABLE",
   "VK_CMD_SET_LOGIC_OP_EXT",
   "VK_CMD_SET_PRIMITIVE_RESTART_ENABLE",
   "VK_CMD_SET_TESSELLATION_DOMAIN_ORIGIN_EXT",
   "VK_CMD_SET_DEPTH_CLAMP_ENABLE_EXT",
   "VK_CMD_SET_POLYGON_MODE_EXT",
   "VK_CMD_SET_RASTERIZATION_SAMPLES_EXT",
   "VK_CMD_SET_SAMPLE_MASK_EXT",
   "VK_CMD_SET_ALPHA_TO_COVERAGE_ENABLE_EXT",
   "VK_CMD_SET_ALPHA_TO_ONE_ENABLE_EXT",
   "VK_CMD_SET_LOGIC_OP_ENABLE_EXT",
   "VK_CMD_SET_COLOR_BLEND_ENABLE_EXT",
   "VK_CMD_SET_COLOR_BLEND_EQUATION_EXT",
   "VK_CMD_SET_COLOR_WRITE_MASK_EXT",
   "VK_CMD_SET_RASTERIZATION_STREAM_EXT",
   "VK_CMD_SET_CONSERVATIVE_RASTERIZATION_MODE_EXT",
   "VK_CMD_SET_EXTRA_PRIMITIVE_OVERESTIMATION_SIZE_EXT",
   "VK_CMD_SET_DEPTH_CLIP_ENABLE_EXT",
   "VK_CMD_SET_SAMPLE_LOCATIONS_ENABLE_EXT",
   "VK_CMD_SET_COLOR_BLEND_ADVANCED_EXT",
   "VK_CMD_SET_PROVOKING_VERTEX_MODE_EXT",
   "VK_CMD_SET_LINE_RASTERIZATION_MODE_EXT",
   "VK_CMD_SET_LINE_STIPPLE_ENABLE_EXT",
   "VK_CMD_SET_DEPTH_CLIP_NEGATIVE_ONE_TO_ONE_EXT",
   "VK_CMD_SET_VIEWPORT_WSCALING_ENABLE_NV",
   "VK_CMD_SET_VIEWPORT_SWIZZLE_NV",
   "VK_CMD_SET_COVERAGE_TO_COLOR_ENABLE_NV",
   "VK_CMD_SET_COVERAGE_TO_COLOR_LOCATION_NV",
   "VK_CMD_SET_COVERAGE_MODULATION_MODE_NV",
   "VK_CMD_SET_COVERAGE_MODULATION_TABLE_ENABLE_NV",
   "VK_CMD_SET_COVERAGE_MODULATION_TABLE_NV",
   "VK_CMD_SET_SHADING_RATE_IMAGE_ENABLE_NV",
   "VK_CMD_SET_COVERAGE_REDUCTION_MODE_NV",
   "VK_CMD_SET_REPRESENTATIVE_FRAGMENT_TEST_ENABLE_NV",
   "VK_CMD_COPY_BUFFER2",
   "VK_CMD_COPY_IMAGE2",
   "VK_CMD_BLIT_IMAGE2",
   "VK_CMD_COPY_BUFFER_TO_IMAGE2",
   "VK_CMD_COPY_IMAGE_TO_BUFFER2",
   "VK_CMD_RESOLVE_IMAGE2",
   "VK_CMD_SET_FRAGMENT_SHADING_RATE_KHR",
   "VK_CMD_SET_FRAGMENT_SHADING_RATE_ENUM_NV",
   "VK_CMD_SET_VERTEX_INPUT_EXT",
   "VK_CMD_SET_COLOR_WRITE_ENABLE_EXT",
   "VK_CMD_SET_EVENT2",
   "VK_CMD_RESET_EVENT2",
   "VK_CMD_WAIT_EVENTS2",
   "VK_CMD_PIPELINE_BARRIER2",
   "VK_CMD_WRITE_TIMESTAMP2",
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
   "VK_CMD_BEGIN_RENDERING",
   "VK_CMD_END_RENDERING",
   "VK_CMD_BUILD_MICROMAPS_EXT",
   "VK_CMD_COPY_MICROMAP_EXT",
   "VK_CMD_COPY_MICROMAP_TO_MEMORY_EXT",
   "VK_CMD_COPY_MEMORY_TO_MICROMAP_EXT",
   "VK_CMD_WRITE_MICROMAPS_PROPERTIES_EXT",
   "VK_CMD_OPTICAL_FLOW_EXECUTE_NV",
};

static void
vk_free_cmd_bind_pipeline(struct vk_cmd_queue *queue,
                          struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_bind_pipeline(struct vk_cmd_queue *queue
, VkPipelineBindPoint pipelineBindPoint
, VkPipeline pipeline
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_BIND_PIPELINE;
      
   cmd->u.bind_pipeline.pipeline_bind_point = pipelineBindPoint;
   cmd->u.bind_pipeline.pipeline = pipeline;

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

}

static void
vk_free_cmd_set_viewport(struct vk_cmd_queue *queue,
                         struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, ( VkViewport* )cmd->u.set_viewport.viewports);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_set_viewport(struct vk_cmd_queue *queue
, uint32_t firstViewport
, uint32_t viewportCount
, const VkViewport* pViewports
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_SET_VIEWPORT;
      
   cmd->u.set_viewport.first_viewport = firstViewport;
   cmd->u.set_viewport.viewport_count = viewportCount;
   if (pViewports) {
      cmd->u.set_viewport.viewports = vk_zalloc(queue->alloc, sizeof(*cmd->u.set_viewport.viewports) * (viewportCount), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (cmd->u.set_viewport.viewports == NULL) goto err;

   memcpy(( VkViewport* )cmd->u.set_viewport.viewports, pViewports, sizeof(*cmd->u.set_viewport.viewports) * (viewportCount));
   }   

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

err:
   if (cmd)
      vk_free_cmd_set_viewport(queue, cmd);
   return VK_ERROR_OUT_OF_HOST_MEMORY;
}

static void
vk_free_cmd_set_scissor(struct vk_cmd_queue *queue,
                        struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, ( VkRect2D* )cmd->u.set_scissor.scissors);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_set_scissor(struct vk_cmd_queue *queue
, uint32_t firstScissor
, uint32_t scissorCount
, const VkRect2D* pScissors
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_SET_SCISSOR;
      
   cmd->u.set_scissor.first_scissor = firstScissor;
   cmd->u.set_scissor.scissor_count = scissorCount;
   if (pScissors) {
      cmd->u.set_scissor.scissors = vk_zalloc(queue->alloc, sizeof(*cmd->u.set_scissor.scissors) * (scissorCount), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (cmd->u.set_scissor.scissors == NULL) goto err;

   memcpy(( VkRect2D* )cmd->u.set_scissor.scissors, pScissors, sizeof(*cmd->u.set_scissor.scissors) * (scissorCount));
   }   

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

err:
   if (cmd)
      vk_free_cmd_set_scissor(queue, cmd);
   return VK_ERROR_OUT_OF_HOST_MEMORY;
}

static void
vk_free_cmd_set_line_width(struct vk_cmd_queue *queue,
                           struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_set_line_width(struct vk_cmd_queue *queue
, float lineWidth
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_SET_LINE_WIDTH;
      
   cmd->u.set_line_width.line_width = lineWidth;

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

}

static void
vk_free_cmd_set_depth_bias(struct vk_cmd_queue *queue,
                           struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_set_depth_bias(struct vk_cmd_queue *queue
, float depthBiasConstantFactor
, float depthBiasClamp
, float depthBiasSlopeFactor
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_SET_DEPTH_BIAS;
      
   cmd->u.set_depth_bias.depth_bias_constant_factor = depthBiasConstantFactor;
   cmd->u.set_depth_bias.depth_bias_clamp = depthBiasClamp;
   cmd->u.set_depth_bias.depth_bias_slope_factor = depthBiasSlopeFactor;

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

}

static void
vk_free_cmd_set_blend_constants(struct vk_cmd_queue *queue,
                                struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_set_blend_constants(struct vk_cmd_queue *queue
, const float blendConstants[4]
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_SET_BLEND_CONSTANTS;
      
   memcpy(cmd->u.set_blend_constants.blend_constants, blendConstants,
          sizeof(*blendConstants) * 4);

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

}

static void
vk_free_cmd_set_depth_bounds(struct vk_cmd_queue *queue,
                             struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_set_depth_bounds(struct vk_cmd_queue *queue
, float minDepthBounds
, float maxDepthBounds
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_SET_DEPTH_BOUNDS;
      
   cmd->u.set_depth_bounds.min_depth_bounds = minDepthBounds;
   cmd->u.set_depth_bounds.max_depth_bounds = maxDepthBounds;

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

}

static void
vk_free_cmd_set_stencil_compare_mask(struct vk_cmd_queue *queue,
                                     struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_set_stencil_compare_mask(struct vk_cmd_queue *queue
, VkStencilFaceFlags faceMask
, uint32_t compareMask
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_SET_STENCIL_COMPARE_MASK;
      
   cmd->u.set_stencil_compare_mask.face_mask = faceMask;
   cmd->u.set_stencil_compare_mask.compare_mask = compareMask;

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

}

static void
vk_free_cmd_set_stencil_write_mask(struct vk_cmd_queue *queue,
                                   struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_set_stencil_write_mask(struct vk_cmd_queue *queue
, VkStencilFaceFlags faceMask
, uint32_t writeMask
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_SET_STENCIL_WRITE_MASK;
      
   cmd->u.set_stencil_write_mask.face_mask = faceMask;
   cmd->u.set_stencil_write_mask.write_mask = writeMask;

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

}

static void
vk_free_cmd_set_stencil_reference(struct vk_cmd_queue *queue,
                                  struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_set_stencil_reference(struct vk_cmd_queue *queue
, VkStencilFaceFlags faceMask
, uint32_t reference
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_SET_STENCIL_REFERENCE;
      
   cmd->u.set_stencil_reference.face_mask = faceMask;
   cmd->u.set_stencil_reference.reference = reference;

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

}

static void
vk_free_cmd_bind_descriptor_sets(struct vk_cmd_queue *queue,
                                 struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, ( VkDescriptorSet* )cmd->u.bind_descriptor_sets.descriptor_sets);
   vk_free(queue->alloc, ( uint32_t* )cmd->u.bind_descriptor_sets.dynamic_offsets);
   vk_free(queue->alloc, cmd);
}


static void
vk_free_cmd_bind_index_buffer(struct vk_cmd_queue *queue,
                              struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_bind_index_buffer(struct vk_cmd_queue *queue
, VkBuffer buffer
, VkDeviceSize offset
, VkIndexType indexType
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_BIND_INDEX_BUFFER;
      
   cmd->u.bind_index_buffer.buffer = buffer;
   cmd->u.bind_index_buffer.offset = offset;
   cmd->u.bind_index_buffer.index_type = indexType;

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

}

static void
vk_free_cmd_bind_vertex_buffers(struct vk_cmd_queue *queue,
                                struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, ( VkBuffer* )cmd->u.bind_vertex_buffers.buffers);
   vk_free(queue->alloc, ( VkDeviceSize* )cmd->u.bind_vertex_buffers.offsets);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_bind_vertex_buffers(struct vk_cmd_queue *queue
, uint32_t firstBinding
, uint32_t bindingCount
, const VkBuffer* pBuffers
, const VkDeviceSize* pOffsets
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_BIND_VERTEX_BUFFERS;
      
   cmd->u.bind_vertex_buffers.first_binding = firstBinding;
   cmd->u.bind_vertex_buffers.binding_count = bindingCount;
   if (pBuffers) {
      cmd->u.bind_vertex_buffers.buffers = vk_zalloc(queue->alloc, sizeof(*cmd->u.bind_vertex_buffers.buffers) * (bindingCount), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (cmd->u.bind_vertex_buffers.buffers == NULL) goto err;

   memcpy(( VkBuffer* )cmd->u.bind_vertex_buffers.buffers, pBuffers, sizeof(*cmd->u.bind_vertex_buffers.buffers) * (bindingCount));
   }   
   if (pOffsets) {
      cmd->u.bind_vertex_buffers.offsets = vk_zalloc(queue->alloc, sizeof(*cmd->u.bind_vertex_buffers.offsets) * (bindingCount), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (cmd->u.bind_vertex_buffers.offsets == NULL) goto err;

   memcpy(( VkDeviceSize* )cmd->u.bind_vertex_buffers.offsets, pOffsets, sizeof(*cmd->u.bind_vertex_buffers.offsets) * (bindingCount));
   }   

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

err:
   if (cmd)
      vk_free_cmd_bind_vertex_buffers(queue, cmd);
   return VK_ERROR_OUT_OF_HOST_MEMORY;
}

static void
vk_free_cmd_draw(struct vk_cmd_queue *queue,
                 struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_draw(struct vk_cmd_queue *queue
, uint32_t vertexCount
, uint32_t instanceCount
, uint32_t firstVertex
, uint32_t firstInstance
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_DRAW;
      
   cmd->u.draw.vertex_count = vertexCount;
   cmd->u.draw.instance_count = instanceCount;
   cmd->u.draw.first_vertex = firstVertex;
   cmd->u.draw.first_instance = firstInstance;

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

}

static void
vk_free_cmd_draw_indexed(struct vk_cmd_queue *queue,
                         struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_draw_indexed(struct vk_cmd_queue *queue
, uint32_t indexCount
, uint32_t instanceCount
, uint32_t firstIndex
, int32_t vertexOffset
, uint32_t firstInstance
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_DRAW_INDEXED;
      
   cmd->u.draw_indexed.index_count = indexCount;
   cmd->u.draw_indexed.instance_count = instanceCount;
   cmd->u.draw_indexed.first_index = firstIndex;
   cmd->u.draw_indexed.vertex_offset = vertexOffset;
   cmd->u.draw_indexed.first_instance = firstInstance;

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

}

static void
vk_free_cmd_draw_multi_ext(struct vk_cmd_queue *queue,
                           struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, ( VkMultiDrawInfoEXT* )cmd->u.draw_multi_ext.vertex_info);
   vk_free(queue->alloc, cmd);
}


static void
vk_free_cmd_draw_multi_indexed_ext(struct vk_cmd_queue *queue,
                                   struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, ( VkMultiDrawIndexedInfoEXT* )cmd->u.draw_multi_indexed_ext.index_info);
         vk_free(queue->alloc, ( int32_t* )cmd->u.draw_multi_indexed_ext.vertex_offset);

   vk_free(queue->alloc, cmd);
}


static void
vk_free_cmd_draw_indirect(struct vk_cmd_queue *queue,
                          struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_draw_indirect(struct vk_cmd_queue *queue
, VkBuffer buffer
, VkDeviceSize offset
, uint32_t drawCount
, uint32_t stride
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_DRAW_INDIRECT;
      
   cmd->u.draw_indirect.buffer = buffer;
   cmd->u.draw_indirect.offset = offset;
   cmd->u.draw_indirect.draw_count = drawCount;
   cmd->u.draw_indirect.stride = stride;

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

}

static void
vk_free_cmd_draw_indexed_indirect(struct vk_cmd_queue *queue,
                                  struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_draw_indexed_indirect(struct vk_cmd_queue *queue
, VkBuffer buffer
, VkDeviceSize offset
, uint32_t drawCount
, uint32_t stride
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_DRAW_INDEXED_INDIRECT;
      
   cmd->u.draw_indexed_indirect.buffer = buffer;
   cmd->u.draw_indexed_indirect.offset = offset;
   cmd->u.draw_indexed_indirect.draw_count = drawCount;
   cmd->u.draw_indexed_indirect.stride = stride;

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

}

static void
vk_free_cmd_dispatch(struct vk_cmd_queue *queue,
                     struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_dispatch(struct vk_cmd_queue *queue
, uint32_t groupCountX
, uint32_t groupCountY
, uint32_t groupCountZ
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_DISPATCH;
      
   cmd->u.dispatch.group_count_x = groupCountX;
   cmd->u.dispatch.group_count_y = groupCountY;
   cmd->u.dispatch.group_count_z = groupCountZ;

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

}

static void
vk_free_cmd_dispatch_indirect(struct vk_cmd_queue *queue,
                              struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_dispatch_indirect(struct vk_cmd_queue *queue
, VkBuffer buffer
, VkDeviceSize offset
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_DISPATCH_INDIRECT;
      
   cmd->u.dispatch_indirect.buffer = buffer;
   cmd->u.dispatch_indirect.offset = offset;

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

}

static void
vk_free_cmd_subpass_shading_huawei(struct vk_cmd_queue *queue,
                                   struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_subpass_shading_huawei(struct vk_cmd_queue *queue
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_SUBPASS_SHADING_HUAWEI;
      

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

}

static void
vk_free_cmd_copy_buffer(struct vk_cmd_queue *queue,
                        struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, ( VkBufferCopy* )cmd->u.copy_buffer.regions);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_copy_buffer(struct vk_cmd_queue *queue
, VkBuffer srcBuffer
, VkBuffer dstBuffer
, uint32_t regionCount
, const VkBufferCopy* pRegions
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_COPY_BUFFER;
      
   cmd->u.copy_buffer.src_buffer = srcBuffer;
   cmd->u.copy_buffer.dst_buffer = dstBuffer;
   cmd->u.copy_buffer.region_count = regionCount;
   if (pRegions) {
      cmd->u.copy_buffer.regions = vk_zalloc(queue->alloc, sizeof(*cmd->u.copy_buffer.regions) * (regionCount), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (cmd->u.copy_buffer.regions == NULL) goto err;

   memcpy(( VkBufferCopy* )cmd->u.copy_buffer.regions, pRegions, sizeof(*cmd->u.copy_buffer.regions) * (regionCount));
   }   

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

err:
   if (cmd)
      vk_free_cmd_copy_buffer(queue, cmd);
   return VK_ERROR_OUT_OF_HOST_MEMORY;
}

static void
vk_free_cmd_copy_image(struct vk_cmd_queue *queue,
                       struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, ( VkImageCopy* )cmd->u.copy_image.regions);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_copy_image(struct vk_cmd_queue *queue
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
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_COPY_IMAGE;
      
   cmd->u.copy_image.src_image = srcImage;
   cmd->u.copy_image.src_image_layout = srcImageLayout;
   cmd->u.copy_image.dst_image = dstImage;
   cmd->u.copy_image.dst_image_layout = dstImageLayout;
   cmd->u.copy_image.region_count = regionCount;
   if (pRegions) {
      cmd->u.copy_image.regions = vk_zalloc(queue->alloc, sizeof(*cmd->u.copy_image.regions) * (regionCount), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (cmd->u.copy_image.regions == NULL) goto err;

   memcpy(( VkImageCopy* )cmd->u.copy_image.regions, pRegions, sizeof(*cmd->u.copy_image.regions) * (regionCount));
   }   

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

err:
   if (cmd)
      vk_free_cmd_copy_image(queue, cmd);
   return VK_ERROR_OUT_OF_HOST_MEMORY;
}

static void
vk_free_cmd_blit_image(struct vk_cmd_queue *queue,
                       struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, ( VkImageBlit* )cmd->u.blit_image.regions);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_blit_image(struct vk_cmd_queue *queue
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
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_BLIT_IMAGE;
      
   cmd->u.blit_image.src_image = srcImage;
   cmd->u.blit_image.src_image_layout = srcImageLayout;
   cmd->u.blit_image.dst_image = dstImage;
   cmd->u.blit_image.dst_image_layout = dstImageLayout;
   cmd->u.blit_image.region_count = regionCount;
   if (pRegions) {
      cmd->u.blit_image.regions = vk_zalloc(queue->alloc, sizeof(*cmd->u.blit_image.regions) * (regionCount), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (cmd->u.blit_image.regions == NULL) goto err;

   memcpy(( VkImageBlit* )cmd->u.blit_image.regions, pRegions, sizeof(*cmd->u.blit_image.regions) * (regionCount));
   }   
   cmd->u.blit_image.filter = filter;

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

err:
   if (cmd)
      vk_free_cmd_blit_image(queue, cmd);
   return VK_ERROR_OUT_OF_HOST_MEMORY;
}

static void
vk_free_cmd_copy_buffer_to_image(struct vk_cmd_queue *queue,
                                 struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, ( VkBufferImageCopy* )cmd->u.copy_buffer_to_image.regions);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_copy_buffer_to_image(struct vk_cmd_queue *queue
, VkBuffer srcBuffer
, VkImage dstImage
, VkImageLayout dstImageLayout
, uint32_t regionCount
, const VkBufferImageCopy* pRegions
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_COPY_BUFFER_TO_IMAGE;
      
   cmd->u.copy_buffer_to_image.src_buffer = srcBuffer;
   cmd->u.copy_buffer_to_image.dst_image = dstImage;
   cmd->u.copy_buffer_to_image.dst_image_layout = dstImageLayout;
   cmd->u.copy_buffer_to_image.region_count = regionCount;
   if (pRegions) {
      cmd->u.copy_buffer_to_image.regions = vk_zalloc(queue->alloc, sizeof(*cmd->u.copy_buffer_to_image.regions) * (regionCount), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (cmd->u.copy_buffer_to_image.regions == NULL) goto err;

   memcpy(( VkBufferImageCopy* )cmd->u.copy_buffer_to_image.regions, pRegions, sizeof(*cmd->u.copy_buffer_to_image.regions) * (regionCount));
   }   

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

err:
   if (cmd)
      vk_free_cmd_copy_buffer_to_image(queue, cmd);
   return VK_ERROR_OUT_OF_HOST_MEMORY;
}

static void
vk_free_cmd_copy_image_to_buffer(struct vk_cmd_queue *queue,
                                 struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, ( VkBufferImageCopy* )cmd->u.copy_image_to_buffer.regions);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_copy_image_to_buffer(struct vk_cmd_queue *queue
, VkImage srcImage
, VkImageLayout srcImageLayout
, VkBuffer dstBuffer
, uint32_t regionCount
, const VkBufferImageCopy* pRegions
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_COPY_IMAGE_TO_BUFFER;
      
   cmd->u.copy_image_to_buffer.src_image = srcImage;
   cmd->u.copy_image_to_buffer.src_image_layout = srcImageLayout;
   cmd->u.copy_image_to_buffer.dst_buffer = dstBuffer;
   cmd->u.copy_image_to_buffer.region_count = regionCount;
   if (pRegions) {
      cmd->u.copy_image_to_buffer.regions = vk_zalloc(queue->alloc, sizeof(*cmd->u.copy_image_to_buffer.regions) * (regionCount), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (cmd->u.copy_image_to_buffer.regions == NULL) goto err;

   memcpy(( VkBufferImageCopy* )cmd->u.copy_image_to_buffer.regions, pRegions, sizeof(*cmd->u.copy_image_to_buffer.regions) * (regionCount));
   }   

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

err:
   if (cmd)
      vk_free_cmd_copy_image_to_buffer(queue, cmd);
   return VK_ERROR_OUT_OF_HOST_MEMORY;
}

static void
vk_free_cmd_update_buffer(struct vk_cmd_queue *queue,
                          struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, ( void* )cmd->u.update_buffer.data);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_update_buffer(struct vk_cmd_queue *queue
, VkBuffer dstBuffer
, VkDeviceSize dstOffset
, VkDeviceSize dataSize
, const void* pData
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_UPDATE_BUFFER;
      
   cmd->u.update_buffer.dst_buffer = dstBuffer;
   cmd->u.update_buffer.dst_offset = dstOffset;
   cmd->u.update_buffer.data_size = dataSize;
   if (pData) {
      cmd->u.update_buffer.data = vk_zalloc(queue->alloc, 1 * (dataSize), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (cmd->u.update_buffer.data == NULL) goto err;

   memcpy(( void* )cmd->u.update_buffer.data, pData, 1 * (dataSize));
   }   

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

err:
   if (cmd)
      vk_free_cmd_update_buffer(queue, cmd);
   return VK_ERROR_OUT_OF_HOST_MEMORY;
}

static void
vk_free_cmd_fill_buffer(struct vk_cmd_queue *queue,
                        struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_fill_buffer(struct vk_cmd_queue *queue
, VkBuffer dstBuffer
, VkDeviceSize dstOffset
, VkDeviceSize size
, uint32_t data
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_FILL_BUFFER;
      
   cmd->u.fill_buffer.dst_buffer = dstBuffer;
   cmd->u.fill_buffer.dst_offset = dstOffset;
   cmd->u.fill_buffer.size = size;
   cmd->u.fill_buffer.data = data;

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

}

static void
vk_free_cmd_clear_color_image(struct vk_cmd_queue *queue,
                              struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
         vk_free(queue->alloc, ( VkClearColorValue* )cmd->u.clear_color_image.color);

   vk_free(queue->alloc, ( VkImageSubresourceRange* )cmd->u.clear_color_image.ranges);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_clear_color_image(struct vk_cmd_queue *queue
, VkImage image
, VkImageLayout imageLayout
, const VkClearColorValue* pColor
, uint32_t rangeCount
, const VkImageSubresourceRange* pRanges
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_CLEAR_COLOR_IMAGE;
      
   cmd->u.clear_color_image.image = image;
   cmd->u.clear_color_image.image_layout = imageLayout;
   if (pColor) {
      cmd->u.clear_color_image.color = vk_zalloc(queue->alloc, sizeof(VkClearColorValue), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
      if (cmd->u.clear_color_image.color == NULL) goto err;

      memcpy((void*)cmd->u.clear_color_image.color, pColor, sizeof(VkClearColorValue));
   VkClearColorValue *tmp_dst1 = (void *) cmd->u.clear_color_image.color; (void) tmp_dst1;
   VkClearColorValue *tmp_src1 = (void *) pColor; (void) tmp_src1;   
      } else {
      cmd->u.clear_color_image.color = NULL;
   }   
   cmd->u.clear_color_image.range_count = rangeCount;
   if (pRanges) {
      cmd->u.clear_color_image.ranges = vk_zalloc(queue->alloc, sizeof(*cmd->u.clear_color_image.ranges) * (rangeCount), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (cmd->u.clear_color_image.ranges == NULL) goto err;

   memcpy(( VkImageSubresourceRange* )cmd->u.clear_color_image.ranges, pRanges, sizeof(*cmd->u.clear_color_image.ranges) * (rangeCount));
   }   

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

err:
   if (cmd)
      vk_free_cmd_clear_color_image(queue, cmd);
   return VK_ERROR_OUT_OF_HOST_MEMORY;
}

static void
vk_free_cmd_clear_depth_stencil_image(struct vk_cmd_queue *queue,
                                      struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
         vk_free(queue->alloc, ( VkClearDepthStencilValue* )cmd->u.clear_depth_stencil_image.depth_stencil);

   vk_free(queue->alloc, ( VkImageSubresourceRange* )cmd->u.clear_depth_stencil_image.ranges);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_clear_depth_stencil_image(struct vk_cmd_queue *queue
, VkImage image
, VkImageLayout imageLayout
, const VkClearDepthStencilValue* pDepthStencil
, uint32_t rangeCount
, const VkImageSubresourceRange* pRanges
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_CLEAR_DEPTH_STENCIL_IMAGE;
      
   cmd->u.clear_depth_stencil_image.image = image;
   cmd->u.clear_depth_stencil_image.image_layout = imageLayout;
   if (pDepthStencil) {
      cmd->u.clear_depth_stencil_image.depth_stencil = vk_zalloc(queue->alloc, sizeof(VkClearDepthStencilValue), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
      if (cmd->u.clear_depth_stencil_image.depth_stencil == NULL) goto err;

      memcpy((void*)cmd->u.clear_depth_stencil_image.depth_stencil, pDepthStencil, sizeof(VkClearDepthStencilValue));
   VkClearDepthStencilValue *tmp_dst1 = (void *) cmd->u.clear_depth_stencil_image.depth_stencil; (void) tmp_dst1;
   VkClearDepthStencilValue *tmp_src1 = (void *) pDepthStencil; (void) tmp_src1;   
      } else {
      cmd->u.clear_depth_stencil_image.depth_stencil = NULL;
   }   
   cmd->u.clear_depth_stencil_image.range_count = rangeCount;
   if (pRanges) {
      cmd->u.clear_depth_stencil_image.ranges = vk_zalloc(queue->alloc, sizeof(*cmd->u.clear_depth_stencil_image.ranges) * (rangeCount), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (cmd->u.clear_depth_stencil_image.ranges == NULL) goto err;

   memcpy(( VkImageSubresourceRange* )cmd->u.clear_depth_stencil_image.ranges, pRanges, sizeof(*cmd->u.clear_depth_stencil_image.ranges) * (rangeCount));
   }   

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

err:
   if (cmd)
      vk_free_cmd_clear_depth_stencil_image(queue, cmd);
   return VK_ERROR_OUT_OF_HOST_MEMORY;
}

static void
vk_free_cmd_clear_attachments(struct vk_cmd_queue *queue,
                              struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, ( VkClearAttachment* )cmd->u.clear_attachments.attachments);
   vk_free(queue->alloc, ( VkClearRect* )cmd->u.clear_attachments.rects);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_clear_attachments(struct vk_cmd_queue *queue
, uint32_t attachmentCount
, const VkClearAttachment* pAttachments
, uint32_t rectCount
, const VkClearRect* pRects
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_CLEAR_ATTACHMENTS;
      
   cmd->u.clear_attachments.attachment_count = attachmentCount;
   if (pAttachments) {
      cmd->u.clear_attachments.attachments = vk_zalloc(queue->alloc, sizeof(*cmd->u.clear_attachments.attachments) * (attachmentCount), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (cmd->u.clear_attachments.attachments == NULL) goto err;

   memcpy(( VkClearAttachment* )cmd->u.clear_attachments.attachments, pAttachments, sizeof(*cmd->u.clear_attachments.attachments) * (attachmentCount));
   }   
   cmd->u.clear_attachments.rect_count = rectCount;
   if (pRects) {
      cmd->u.clear_attachments.rects = vk_zalloc(queue->alloc, sizeof(*cmd->u.clear_attachments.rects) * (rectCount), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (cmd->u.clear_attachments.rects == NULL) goto err;

   memcpy(( VkClearRect* )cmd->u.clear_attachments.rects, pRects, sizeof(*cmd->u.clear_attachments.rects) * (rectCount));
   }   

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

err:
   if (cmd)
      vk_free_cmd_clear_attachments(queue, cmd);
   return VK_ERROR_OUT_OF_HOST_MEMORY;
}

static void
vk_free_cmd_resolve_image(struct vk_cmd_queue *queue,
                          struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, ( VkImageResolve* )cmd->u.resolve_image.regions);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_resolve_image(struct vk_cmd_queue *queue
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
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_RESOLVE_IMAGE;
      
   cmd->u.resolve_image.src_image = srcImage;
   cmd->u.resolve_image.src_image_layout = srcImageLayout;
   cmd->u.resolve_image.dst_image = dstImage;
   cmd->u.resolve_image.dst_image_layout = dstImageLayout;
   cmd->u.resolve_image.region_count = regionCount;
   if (pRegions) {
      cmd->u.resolve_image.regions = vk_zalloc(queue->alloc, sizeof(*cmd->u.resolve_image.regions) * (regionCount), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (cmd->u.resolve_image.regions == NULL) goto err;

   memcpy(( VkImageResolve* )cmd->u.resolve_image.regions, pRegions, sizeof(*cmd->u.resolve_image.regions) * (regionCount));
   }   

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

err:
   if (cmd)
      vk_free_cmd_resolve_image(queue, cmd);
   return VK_ERROR_OUT_OF_HOST_MEMORY;
}

static void
vk_free_cmd_set_event(struct vk_cmd_queue *queue,
                      struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_set_event(struct vk_cmd_queue *queue
, VkEvent event
, VkPipelineStageFlags stageMask
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_SET_EVENT;
      
   cmd->u.set_event.event = event;
   cmd->u.set_event.stage_mask = stageMask;

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

}

static void
vk_free_cmd_reset_event(struct vk_cmd_queue *queue,
                        struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_reset_event(struct vk_cmd_queue *queue
, VkEvent event
, VkPipelineStageFlags stageMask
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_RESET_EVENT;
      
   cmd->u.reset_event.event = event;
   cmd->u.reset_event.stage_mask = stageMask;

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

}

static void
vk_free_cmd_wait_events(struct vk_cmd_queue *queue,
                        struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, ( VkEvent* )cmd->u.wait_events.events);
   vk_free(queue->alloc, ( VkMemoryBarrier* )cmd->u.wait_events.memory_barriers);
   vk_free(queue->alloc, ( VkBufferMemoryBarrier* )cmd->u.wait_events.buffer_memory_barriers);
   vk_free(queue->alloc, ( VkImageMemoryBarrier* )cmd->u.wait_events.image_memory_barriers);
   vk_free(queue->alloc, cmd);
}

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
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_WAIT_EVENTS;
      
   cmd->u.wait_events.event_count = eventCount;
   if (pEvents) {
      cmd->u.wait_events.events = vk_zalloc(queue->alloc, sizeof(*cmd->u.wait_events.events) * (eventCount), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (cmd->u.wait_events.events == NULL) goto err;

   memcpy(( VkEvent* )cmd->u.wait_events.events, pEvents, sizeof(*cmd->u.wait_events.events) * (eventCount));
   }   
   cmd->u.wait_events.src_stage_mask = srcStageMask;
   cmd->u.wait_events.dst_stage_mask = dstStageMask;
   cmd->u.wait_events.memory_barrier_count = memoryBarrierCount;
   if (pMemoryBarriers) {
      cmd->u.wait_events.memory_barriers = vk_zalloc(queue->alloc, sizeof(*cmd->u.wait_events.memory_barriers) * (memoryBarrierCount), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (cmd->u.wait_events.memory_barriers == NULL) goto err;

   memcpy(( VkMemoryBarrier* )cmd->u.wait_events.memory_barriers, pMemoryBarriers, sizeof(*cmd->u.wait_events.memory_barriers) * (memoryBarrierCount));
   }   
   cmd->u.wait_events.buffer_memory_barrier_count = bufferMemoryBarrierCount;
   if (pBufferMemoryBarriers) {
      cmd->u.wait_events.buffer_memory_barriers = vk_zalloc(queue->alloc, sizeof(*cmd->u.wait_events.buffer_memory_barriers) * (bufferMemoryBarrierCount), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (cmd->u.wait_events.buffer_memory_barriers == NULL) goto err;

   memcpy(( VkBufferMemoryBarrier* )cmd->u.wait_events.buffer_memory_barriers, pBufferMemoryBarriers, sizeof(*cmd->u.wait_events.buffer_memory_barriers) * (bufferMemoryBarrierCount));
   }   
   cmd->u.wait_events.image_memory_barrier_count = imageMemoryBarrierCount;
   if (pImageMemoryBarriers) {
      cmd->u.wait_events.image_memory_barriers = vk_zalloc(queue->alloc, sizeof(*cmd->u.wait_events.image_memory_barriers) * (imageMemoryBarrierCount), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (cmd->u.wait_events.image_memory_barriers == NULL) goto err;

   memcpy(( VkImageMemoryBarrier* )cmd->u.wait_events.image_memory_barriers, pImageMemoryBarriers, sizeof(*cmd->u.wait_events.image_memory_barriers) * (imageMemoryBarrierCount));
   }   

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

err:
   if (cmd)
      vk_free_cmd_wait_events(queue, cmd);
   return VK_ERROR_OUT_OF_HOST_MEMORY;
}

static void
vk_free_cmd_pipeline_barrier(struct vk_cmd_queue *queue,
                             struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, ( VkMemoryBarrier* )cmd->u.pipeline_barrier.memory_barriers);
   vk_free(queue->alloc, ( VkBufferMemoryBarrier* )cmd->u.pipeline_barrier.buffer_memory_barriers);
   vk_free(queue->alloc, ( VkImageMemoryBarrier* )cmd->u.pipeline_barrier.image_memory_barriers);
   vk_free(queue->alloc, cmd);
}

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
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_PIPELINE_BARRIER;
      
   cmd->u.pipeline_barrier.src_stage_mask = srcStageMask;
   cmd->u.pipeline_barrier.dst_stage_mask = dstStageMask;
   cmd->u.pipeline_barrier.dependency_flags = dependencyFlags;
   cmd->u.pipeline_barrier.memory_barrier_count = memoryBarrierCount;
   if (pMemoryBarriers) {
      cmd->u.pipeline_barrier.memory_barriers = vk_zalloc(queue->alloc, sizeof(*cmd->u.pipeline_barrier.memory_barriers) * (memoryBarrierCount), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (cmd->u.pipeline_barrier.memory_barriers == NULL) goto err;

   memcpy(( VkMemoryBarrier* )cmd->u.pipeline_barrier.memory_barriers, pMemoryBarriers, sizeof(*cmd->u.pipeline_barrier.memory_barriers) * (memoryBarrierCount));
   }   
   cmd->u.pipeline_barrier.buffer_memory_barrier_count = bufferMemoryBarrierCount;
   if (pBufferMemoryBarriers) {
      cmd->u.pipeline_barrier.buffer_memory_barriers = vk_zalloc(queue->alloc, sizeof(*cmd->u.pipeline_barrier.buffer_memory_barriers) * (bufferMemoryBarrierCount), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (cmd->u.pipeline_barrier.buffer_memory_barriers == NULL) goto err;

   memcpy(( VkBufferMemoryBarrier* )cmd->u.pipeline_barrier.buffer_memory_barriers, pBufferMemoryBarriers, sizeof(*cmd->u.pipeline_barrier.buffer_memory_barriers) * (bufferMemoryBarrierCount));
   }   
   cmd->u.pipeline_barrier.image_memory_barrier_count = imageMemoryBarrierCount;
   if (pImageMemoryBarriers) {
      cmd->u.pipeline_barrier.image_memory_barriers = vk_zalloc(queue->alloc, sizeof(*cmd->u.pipeline_barrier.image_memory_barriers) * (imageMemoryBarrierCount), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (cmd->u.pipeline_barrier.image_memory_barriers == NULL) goto err;

   memcpy(( VkImageMemoryBarrier* )cmd->u.pipeline_barrier.image_memory_barriers, pImageMemoryBarriers, sizeof(*cmd->u.pipeline_barrier.image_memory_barriers) * (imageMemoryBarrierCount));
   }   

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

err:
   if (cmd)
      vk_free_cmd_pipeline_barrier(queue, cmd);
   return VK_ERROR_OUT_OF_HOST_MEMORY;
}

static void
vk_free_cmd_begin_query(struct vk_cmd_queue *queue,
                        struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_begin_query(struct vk_cmd_queue *queue
, VkQueryPool queryPool
, uint32_t query
, VkQueryControlFlags flags
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_BEGIN_QUERY;
      
   cmd->u.begin_query.query_pool = queryPool;
   cmd->u.begin_query.query = query;
   cmd->u.begin_query.flags = flags;

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

}

static void
vk_free_cmd_end_query(struct vk_cmd_queue *queue,
                      struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_end_query(struct vk_cmd_queue *queue
, VkQueryPool queryPool
, uint32_t query
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_END_QUERY;
      
   cmd->u.end_query.query_pool = queryPool;
   cmd->u.end_query.query = query;

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

}

static void
vk_free_cmd_begin_conditional_rendering_ext(struct vk_cmd_queue *queue,
                                            struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
         vk_free(queue->alloc, ( VkConditionalRenderingBeginInfoEXT* )cmd->u.begin_conditional_rendering_ext.conditional_rendering_begin);

   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_begin_conditional_rendering_ext(struct vk_cmd_queue *queue
, const VkConditionalRenderingBeginInfoEXT* pConditionalRenderingBegin
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_BEGIN_CONDITIONAL_RENDERING_EXT;
      
   if (pConditionalRenderingBegin) {
      cmd->u.begin_conditional_rendering_ext.conditional_rendering_begin = vk_zalloc(queue->alloc, sizeof(VkConditionalRenderingBeginInfoEXT), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
      if (cmd->u.begin_conditional_rendering_ext.conditional_rendering_begin == NULL) goto err;

      memcpy((void*)cmd->u.begin_conditional_rendering_ext.conditional_rendering_begin, pConditionalRenderingBegin, sizeof(VkConditionalRenderingBeginInfoEXT));
   VkConditionalRenderingBeginInfoEXT *tmp_dst1 = (void *) cmd->u.begin_conditional_rendering_ext.conditional_rendering_begin; (void) tmp_dst1;
   VkConditionalRenderingBeginInfoEXT *tmp_src1 = (void *) pConditionalRenderingBegin; (void) tmp_src1;   
      } else {
      cmd->u.begin_conditional_rendering_ext.conditional_rendering_begin = NULL;
   }   

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

err:
   if (cmd)
      vk_free_cmd_begin_conditional_rendering_ext(queue, cmd);
   return VK_ERROR_OUT_OF_HOST_MEMORY;
}

static void
vk_free_cmd_end_conditional_rendering_ext(struct vk_cmd_queue *queue,
                                          struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_end_conditional_rendering_ext(struct vk_cmd_queue *queue
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_END_CONDITIONAL_RENDERING_EXT;
      

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

}

static void
vk_free_cmd_reset_query_pool(struct vk_cmd_queue *queue,
                             struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_reset_query_pool(struct vk_cmd_queue *queue
, VkQueryPool queryPool
, uint32_t firstQuery
, uint32_t queryCount
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_RESET_QUERY_POOL;
      
   cmd->u.reset_query_pool.query_pool = queryPool;
   cmd->u.reset_query_pool.first_query = firstQuery;
   cmd->u.reset_query_pool.query_count = queryCount;

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

}

static void
vk_free_cmd_write_timestamp(struct vk_cmd_queue *queue,
                            struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_write_timestamp(struct vk_cmd_queue *queue
, VkPipelineStageFlagBits pipelineStage
, VkQueryPool queryPool
, uint32_t query
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_WRITE_TIMESTAMP;
      
   cmd->u.write_timestamp.pipeline_stage = pipelineStage;
   cmd->u.write_timestamp.query_pool = queryPool;
   cmd->u.write_timestamp.query = query;

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

}

static void
vk_free_cmd_copy_query_pool_results(struct vk_cmd_queue *queue,
                                    struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_copy_query_pool_results(struct vk_cmd_queue *queue
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
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_COPY_QUERY_POOL_RESULTS;
      
   cmd->u.copy_query_pool_results.query_pool = queryPool;
   cmd->u.copy_query_pool_results.first_query = firstQuery;
   cmd->u.copy_query_pool_results.query_count = queryCount;
   cmd->u.copy_query_pool_results.dst_buffer = dstBuffer;
   cmd->u.copy_query_pool_results.dst_offset = dstOffset;
   cmd->u.copy_query_pool_results.stride = stride;
   cmd->u.copy_query_pool_results.flags = flags;

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

}

static void
vk_free_cmd_push_constants(struct vk_cmd_queue *queue,
                           struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, ( void* )cmd->u.push_constants.values);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_push_constants(struct vk_cmd_queue *queue
, VkPipelineLayout layout
, VkShaderStageFlags stageFlags
, uint32_t offset
, uint32_t size
, const void* pValues
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_PUSH_CONSTANTS;
      
   cmd->u.push_constants.layout = layout;
   cmd->u.push_constants.stage_flags = stageFlags;
   cmd->u.push_constants.offset = offset;
   cmd->u.push_constants.size = size;
   if (pValues) {
      cmd->u.push_constants.values = vk_zalloc(queue->alloc, 1 * (size), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (cmd->u.push_constants.values == NULL) goto err;

   memcpy(( void* )cmd->u.push_constants.values, pValues, 1 * (size));
   }   

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

err:
   if (cmd)
      vk_free_cmd_push_constants(queue, cmd);
   return VK_ERROR_OUT_OF_HOST_MEMORY;
}

static void
vk_free_cmd_begin_render_pass(struct vk_cmd_queue *queue,
                              struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, ( VkClearValue*    )cmd->u.begin_render_pass.render_pass_begin->pClearValues);
      vk_free(queue->alloc, ( VkRenderPassBeginInfo* )cmd->u.begin_render_pass.render_pass_begin);

   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_begin_render_pass(struct vk_cmd_queue *queue
, const VkRenderPassBeginInfo* pRenderPassBegin
, VkSubpassContents contents
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_BEGIN_RENDER_PASS;
      
   if (pRenderPassBegin) {
      cmd->u.begin_render_pass.render_pass_begin = vk_zalloc(queue->alloc, sizeof(VkRenderPassBeginInfo), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
      if (cmd->u.begin_render_pass.render_pass_begin == NULL) goto err;

      memcpy((void*)cmd->u.begin_render_pass.render_pass_begin, pRenderPassBegin, sizeof(VkRenderPassBeginInfo));
   VkRenderPassBeginInfo *tmp_dst1 = (void *) cmd->u.begin_render_pass.render_pass_begin; (void) tmp_dst1;
   VkRenderPassBeginInfo *tmp_src1 = (void *) pRenderPassBegin; (void) tmp_src1;   
   
      const VkBaseInStructure *pnext = tmp_dst1->pNext;
      if (pnext) {
         switch ((int32_t)pnext->sType) {
         
      case VK_STRUCTURE_TYPE_DEVICE_GROUP_RENDER_PASS_BEGIN_INFO:
         if (pnext) {
      tmp_dst1->pNext = vk_zalloc(queue->alloc, sizeof(VkDeviceGroupRenderPassBeginInfo), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
      if (tmp_dst1->pNext == NULL) goto err;

      memcpy((void*)tmp_dst1->pNext, pnext, sizeof(VkDeviceGroupRenderPassBeginInfo));
   VkDeviceGroupRenderPassBeginInfo *tmp_dst2 = (void *) tmp_dst1->pNext; (void) tmp_dst2;
   VkDeviceGroupRenderPassBeginInfo *tmp_src2 = (void *) pnext; (void) tmp_src2;   
   if (tmp_src2->pDeviceRenderAreas) {
   tmp_dst2->pDeviceRenderAreas = vk_zalloc(queue->alloc, sizeof(*tmp_dst2->pDeviceRenderAreas) * tmp_dst2->deviceRenderAreaCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (tmp_dst2->pDeviceRenderAreas == NULL) goto err;

   memcpy(( VkRect2D*  )tmp_dst2->pDeviceRenderAreas, tmp_src2->pDeviceRenderAreas, sizeof(*tmp_dst2->pDeviceRenderAreas) * tmp_dst2->deviceRenderAreaCount);
}
   } else {
      tmp_dst1->pNext = NULL;
   }
         break;
      
      case VK_STRUCTURE_TYPE_RENDER_PASS_SAMPLE_LOCATIONS_BEGIN_INFO_EXT:
         if (pnext) {
      tmp_dst1->pNext = vk_zalloc(queue->alloc, sizeof(VkRenderPassSampleLocationsBeginInfoEXT), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
      if (tmp_dst1->pNext == NULL) goto err;

      memcpy((void*)tmp_dst1->pNext, pnext, sizeof(VkRenderPassSampleLocationsBeginInfoEXT));
   VkRenderPassSampleLocationsBeginInfoEXT *tmp_dst2 = (void *) tmp_dst1->pNext; (void) tmp_dst2;
   VkRenderPassSampleLocationsBeginInfoEXT *tmp_src2 = (void *) pnext; (void) tmp_src2;   
   if (tmp_src2->pAttachmentInitialSampleLocations) {
   tmp_dst2->pAttachmentInitialSampleLocations = vk_zalloc(queue->alloc, sizeof(*tmp_dst2->pAttachmentInitialSampleLocations) * tmp_dst2->attachmentInitialSampleLocationsCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (tmp_dst2->pAttachmentInitialSampleLocations == NULL) goto err;

   memcpy(( VkAttachmentSampleLocationsEXT* )tmp_dst2->pAttachmentInitialSampleLocations, tmp_src2->pAttachmentInitialSampleLocations, sizeof(*tmp_dst2->pAttachmentInitialSampleLocations) * tmp_dst2->attachmentInitialSampleLocationsCount);
}
if (tmp_src2->pPostSubpassSampleLocations) {
   tmp_dst2->pPostSubpassSampleLocations = vk_zalloc(queue->alloc, sizeof(*tmp_dst2->pPostSubpassSampleLocations) * tmp_dst2->postSubpassSampleLocationsCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (tmp_dst2->pPostSubpassSampleLocations == NULL) goto err;

   memcpy(( VkSubpassSampleLocationsEXT* )tmp_dst2->pPostSubpassSampleLocations, tmp_src2->pPostSubpassSampleLocations, sizeof(*tmp_dst2->pPostSubpassSampleLocations) * tmp_dst2->postSubpassSampleLocationsCount);
}
   } else {
      tmp_dst1->pNext = NULL;
   }
         break;
      
      case VK_STRUCTURE_TYPE_RENDER_PASS_ATTACHMENT_BEGIN_INFO:
         if (pnext) {
      tmp_dst1->pNext = vk_zalloc(queue->alloc, sizeof(VkRenderPassAttachmentBeginInfo), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
      if (tmp_dst1->pNext == NULL) goto err;

      memcpy((void*)tmp_dst1->pNext, pnext, sizeof(VkRenderPassAttachmentBeginInfo));
   VkRenderPassAttachmentBeginInfo *tmp_dst2 = (void *) tmp_dst1->pNext; (void) tmp_dst2;
   VkRenderPassAttachmentBeginInfo *tmp_src2 = (void *) pnext; (void) tmp_src2;   
   if (tmp_src2->pAttachments) {
   tmp_dst2->pAttachments = vk_zalloc(queue->alloc, sizeof(*tmp_dst2->pAttachments) * tmp_dst2->attachmentCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (tmp_dst2->pAttachments == NULL) goto err;

   memcpy(( VkImageView* )tmp_dst2->pAttachments, tmp_src2->pAttachments, sizeof(*tmp_dst2->pAttachments) * tmp_dst2->attachmentCount);
}
   } else {
      tmp_dst1->pNext = NULL;
   }
         break;
      
      case VK_STRUCTURE_TYPE_RENDER_PASS_TRANSFORM_BEGIN_INFO_QCOM:
         if (pnext) {
      tmp_dst1->pNext = vk_zalloc(queue->alloc, sizeof(VkRenderPassTransformBeginInfoQCOM), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
      if (tmp_dst1->pNext == NULL) goto err;

      memcpy((void*)tmp_dst1->pNext, pnext, sizeof(VkRenderPassTransformBeginInfoQCOM));
   VkRenderPassTransformBeginInfoQCOM *tmp_dst2 = (void *) tmp_dst1->pNext; (void) tmp_dst2;
   VkRenderPassTransformBeginInfoQCOM *tmp_src2 = (void *) pnext; (void) tmp_src2;   
      } else {
      tmp_dst1->pNext = NULL;
   }
         break;
      
         }
      }
      if (tmp_src1->pClearValues) {
   tmp_dst1->pClearValues = vk_zalloc(queue->alloc, sizeof(*tmp_dst1->pClearValues) * tmp_dst1->clearValueCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (tmp_dst1->pClearValues == NULL) goto err;

   memcpy(( VkClearValue*    )tmp_dst1->pClearValues, tmp_src1->pClearValues, sizeof(*tmp_dst1->pClearValues) * tmp_dst1->clearValueCount);
}
   } else {
      cmd->u.begin_render_pass.render_pass_begin = NULL;
   }   
   cmd->u.begin_render_pass.contents = contents;

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

err:
   if (cmd)
      vk_free_cmd_begin_render_pass(queue, cmd);
   return VK_ERROR_OUT_OF_HOST_MEMORY;
}

static void
vk_free_cmd_next_subpass(struct vk_cmd_queue *queue,
                         struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_next_subpass(struct vk_cmd_queue *queue
, VkSubpassContents contents
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_NEXT_SUBPASS;
      
   cmd->u.next_subpass.contents = contents;

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

}

static void
vk_free_cmd_end_render_pass(struct vk_cmd_queue *queue,
                            struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_end_render_pass(struct vk_cmd_queue *queue
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_END_RENDER_PASS;
      

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

}

static void
vk_free_cmd_execute_commands(struct vk_cmd_queue *queue,
                             struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, ( VkCommandBuffer* )cmd->u.execute_commands.command_buffers);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_execute_commands(struct vk_cmd_queue *queue
, uint32_t commandBufferCount
, const VkCommandBuffer* pCommandBuffers
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_EXECUTE_COMMANDS;
      
   cmd->u.execute_commands.command_buffer_count = commandBufferCount;
   if (pCommandBuffers) {
      cmd->u.execute_commands.command_buffers = vk_zalloc(queue->alloc, sizeof(*cmd->u.execute_commands.command_buffers) * (commandBufferCount), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (cmd->u.execute_commands.command_buffers == NULL) goto err;

   memcpy(( VkCommandBuffer* )cmd->u.execute_commands.command_buffers, pCommandBuffers, sizeof(*cmd->u.execute_commands.command_buffers) * (commandBufferCount));
   }   

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

err:
   if (cmd)
      vk_free_cmd_execute_commands(queue, cmd);
   return VK_ERROR_OUT_OF_HOST_MEMORY;
}

static void
vk_free_cmd_debug_marker_begin_ext(struct vk_cmd_queue *queue,
                                   struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
         vk_free(queue->alloc, ( VkDebugMarkerMarkerInfoEXT* )cmd->u.debug_marker_begin_ext.marker_info);

   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_debug_marker_begin_ext(struct vk_cmd_queue *queue
, const VkDebugMarkerMarkerInfoEXT* pMarkerInfo
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_DEBUG_MARKER_BEGIN_EXT;
      
   if (pMarkerInfo) {
      cmd->u.debug_marker_begin_ext.marker_info = vk_zalloc(queue->alloc, sizeof(VkDebugMarkerMarkerInfoEXT), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
      if (cmd->u.debug_marker_begin_ext.marker_info == NULL) goto err;

      memcpy((void*)cmd->u.debug_marker_begin_ext.marker_info, pMarkerInfo, sizeof(VkDebugMarkerMarkerInfoEXT));
   VkDebugMarkerMarkerInfoEXT *tmp_dst1 = (void *) cmd->u.debug_marker_begin_ext.marker_info; (void) tmp_dst1;
   VkDebugMarkerMarkerInfoEXT *tmp_src1 = (void *) pMarkerInfo; (void) tmp_src1;   
      } else {
      cmd->u.debug_marker_begin_ext.marker_info = NULL;
   }   

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

err:
   if (cmd)
      vk_free_cmd_debug_marker_begin_ext(queue, cmd);
   return VK_ERROR_OUT_OF_HOST_MEMORY;
}

static void
vk_free_cmd_debug_marker_end_ext(struct vk_cmd_queue *queue,
                                 struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_debug_marker_end_ext(struct vk_cmd_queue *queue
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_DEBUG_MARKER_END_EXT;
      

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

}

static void
vk_free_cmd_debug_marker_insert_ext(struct vk_cmd_queue *queue,
                                    struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
         vk_free(queue->alloc, ( VkDebugMarkerMarkerInfoEXT* )cmd->u.debug_marker_insert_ext.marker_info);

   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_debug_marker_insert_ext(struct vk_cmd_queue *queue
, const VkDebugMarkerMarkerInfoEXT* pMarkerInfo
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_DEBUG_MARKER_INSERT_EXT;
      
   if (pMarkerInfo) {
      cmd->u.debug_marker_insert_ext.marker_info = vk_zalloc(queue->alloc, sizeof(VkDebugMarkerMarkerInfoEXT), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
      if (cmd->u.debug_marker_insert_ext.marker_info == NULL) goto err;

      memcpy((void*)cmd->u.debug_marker_insert_ext.marker_info, pMarkerInfo, sizeof(VkDebugMarkerMarkerInfoEXT));
   VkDebugMarkerMarkerInfoEXT *tmp_dst1 = (void *) cmd->u.debug_marker_insert_ext.marker_info; (void) tmp_dst1;
   VkDebugMarkerMarkerInfoEXT *tmp_src1 = (void *) pMarkerInfo; (void) tmp_src1;   
      } else {
      cmd->u.debug_marker_insert_ext.marker_info = NULL;
   }   

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

err:
   if (cmd)
      vk_free_cmd_debug_marker_insert_ext(queue, cmd);
   return VK_ERROR_OUT_OF_HOST_MEMORY;
}

static void
vk_free_cmd_execute_generated_commands_nv(struct vk_cmd_queue *queue,
                                          struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, ( VkIndirectCommandsStreamNV*  )cmd->u.execute_generated_commands_nv.generated_commands_info->pStreams);
      vk_free(queue->alloc, ( VkGeneratedCommandsInfoNV* )cmd->u.execute_generated_commands_nv.generated_commands_info);

   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_execute_generated_commands_nv(struct vk_cmd_queue *queue
, VkBool32 isPreprocessed
, const VkGeneratedCommandsInfoNV* pGeneratedCommandsInfo
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_EXECUTE_GENERATED_COMMANDS_NV;
      
   cmd->u.execute_generated_commands_nv.is_preprocessed = isPreprocessed;
   if (pGeneratedCommandsInfo) {
      cmd->u.execute_generated_commands_nv.generated_commands_info = vk_zalloc(queue->alloc, sizeof(VkGeneratedCommandsInfoNV), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
      if (cmd->u.execute_generated_commands_nv.generated_commands_info == NULL) goto err;

      memcpy((void*)cmd->u.execute_generated_commands_nv.generated_commands_info, pGeneratedCommandsInfo, sizeof(VkGeneratedCommandsInfoNV));
   VkGeneratedCommandsInfoNV *tmp_dst1 = (void *) cmd->u.execute_generated_commands_nv.generated_commands_info; (void) tmp_dst1;
   VkGeneratedCommandsInfoNV *tmp_src1 = (void *) pGeneratedCommandsInfo; (void) tmp_src1;   
   if (tmp_src1->pStreams) {
   tmp_dst1->pStreams = vk_zalloc(queue->alloc, sizeof(*tmp_dst1->pStreams) * tmp_dst1->streamCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (tmp_dst1->pStreams == NULL) goto err;

   memcpy(( VkIndirectCommandsStreamNV*  )tmp_dst1->pStreams, tmp_src1->pStreams, sizeof(*tmp_dst1->pStreams) * tmp_dst1->streamCount);
}
   } else {
      cmd->u.execute_generated_commands_nv.generated_commands_info = NULL;
   }   

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

err:
   if (cmd)
      vk_free_cmd_execute_generated_commands_nv(queue, cmd);
   return VK_ERROR_OUT_OF_HOST_MEMORY;
}

static void
vk_free_cmd_preprocess_generated_commands_nv(struct vk_cmd_queue *queue,
                                             struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, ( VkIndirectCommandsStreamNV*  )cmd->u.preprocess_generated_commands_nv.generated_commands_info->pStreams);
      vk_free(queue->alloc, ( VkGeneratedCommandsInfoNV* )cmd->u.preprocess_generated_commands_nv.generated_commands_info);

   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_preprocess_generated_commands_nv(struct vk_cmd_queue *queue
, const VkGeneratedCommandsInfoNV* pGeneratedCommandsInfo
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_PREPROCESS_GENERATED_COMMANDS_NV;
      
   if (pGeneratedCommandsInfo) {
      cmd->u.preprocess_generated_commands_nv.generated_commands_info = vk_zalloc(queue->alloc, sizeof(VkGeneratedCommandsInfoNV), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
      if (cmd->u.preprocess_generated_commands_nv.generated_commands_info == NULL) goto err;

      memcpy((void*)cmd->u.preprocess_generated_commands_nv.generated_commands_info, pGeneratedCommandsInfo, sizeof(VkGeneratedCommandsInfoNV));
   VkGeneratedCommandsInfoNV *tmp_dst1 = (void *) cmd->u.preprocess_generated_commands_nv.generated_commands_info; (void) tmp_dst1;
   VkGeneratedCommandsInfoNV *tmp_src1 = (void *) pGeneratedCommandsInfo; (void) tmp_src1;   
   if (tmp_src1->pStreams) {
   tmp_dst1->pStreams = vk_zalloc(queue->alloc, sizeof(*tmp_dst1->pStreams) * tmp_dst1->streamCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (tmp_dst1->pStreams == NULL) goto err;

   memcpy(( VkIndirectCommandsStreamNV*  )tmp_dst1->pStreams, tmp_src1->pStreams, sizeof(*tmp_dst1->pStreams) * tmp_dst1->streamCount);
}
   } else {
      cmd->u.preprocess_generated_commands_nv.generated_commands_info = NULL;
   }   

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

err:
   if (cmd)
      vk_free_cmd_preprocess_generated_commands_nv(queue, cmd);
   return VK_ERROR_OUT_OF_HOST_MEMORY;
}

static void
vk_free_cmd_bind_pipeline_shader_group_nv(struct vk_cmd_queue *queue,
                                          struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_bind_pipeline_shader_group_nv(struct vk_cmd_queue *queue
, VkPipelineBindPoint pipelineBindPoint
, VkPipeline pipeline
, uint32_t groupIndex
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_BIND_PIPELINE_SHADER_GROUP_NV;
      
   cmd->u.bind_pipeline_shader_group_nv.pipeline_bind_point = pipelineBindPoint;
   cmd->u.bind_pipeline_shader_group_nv.pipeline = pipeline;
   cmd->u.bind_pipeline_shader_group_nv.group_index = groupIndex;

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

}

static void
vk_free_cmd_push_descriptor_set_khr(struct vk_cmd_queue *queue,
                                    struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, ( VkWriteDescriptorSet* )cmd->u.push_descriptor_set_khr.descriptor_writes);
   vk_free(queue->alloc, cmd);
}


static void
vk_free_cmd_set_device_mask(struct vk_cmd_queue *queue,
                            struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_set_device_mask(struct vk_cmd_queue *queue
, uint32_t deviceMask
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_SET_DEVICE_MASK;
      
   cmd->u.set_device_mask.device_mask = deviceMask;

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

}

static void
vk_free_cmd_dispatch_base(struct vk_cmd_queue *queue,
                          struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_dispatch_base(struct vk_cmd_queue *queue
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
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_DISPATCH_BASE;
      
   cmd->u.dispatch_base.base_group_x = baseGroupX;
   cmd->u.dispatch_base.base_group_y = baseGroupY;
   cmd->u.dispatch_base.base_group_z = baseGroupZ;
   cmd->u.dispatch_base.group_count_x = groupCountX;
   cmd->u.dispatch_base.group_count_y = groupCountY;
   cmd->u.dispatch_base.group_count_z = groupCountZ;

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

}

static void
vk_free_cmd_push_descriptor_set_with_template_khr(struct vk_cmd_queue *queue,
                                                  struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
         vk_free(queue->alloc, ( void* )cmd->u.push_descriptor_set_with_template_khr.data);

   vk_free(queue->alloc, cmd);
}


static void
vk_free_cmd_set_viewport_wscaling_nv(struct vk_cmd_queue *queue,
                                     struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, ( VkViewportWScalingNV* )cmd->u.set_viewport_wscaling_nv.viewport_wscalings);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_set_viewport_wscaling_nv(struct vk_cmd_queue *queue
, uint32_t firstViewport
, uint32_t viewportCount
, const VkViewportWScalingNV* pViewportWScalings
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_SET_VIEWPORT_WSCALING_NV;
      
   cmd->u.set_viewport_wscaling_nv.first_viewport = firstViewport;
   cmd->u.set_viewport_wscaling_nv.viewport_count = viewportCount;
   if (pViewportWScalings) {
      cmd->u.set_viewport_wscaling_nv.viewport_wscalings = vk_zalloc(queue->alloc, sizeof(*cmd->u.set_viewport_wscaling_nv.viewport_wscalings) * (viewportCount), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (cmd->u.set_viewport_wscaling_nv.viewport_wscalings == NULL) goto err;

   memcpy(( VkViewportWScalingNV* )cmd->u.set_viewport_wscaling_nv.viewport_wscalings, pViewportWScalings, sizeof(*cmd->u.set_viewport_wscaling_nv.viewport_wscalings) * (viewportCount));
   }   

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

err:
   if (cmd)
      vk_free_cmd_set_viewport_wscaling_nv(queue, cmd);
   return VK_ERROR_OUT_OF_HOST_MEMORY;
}

static void
vk_free_cmd_set_discard_rectangle_ext(struct vk_cmd_queue *queue,
                                      struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, ( VkRect2D* )cmd->u.set_discard_rectangle_ext.discard_rectangles);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_set_discard_rectangle_ext(struct vk_cmd_queue *queue
, uint32_t firstDiscardRectangle
, uint32_t discardRectangleCount
, const VkRect2D* pDiscardRectangles
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_SET_DISCARD_RECTANGLE_EXT;
      
   cmd->u.set_discard_rectangle_ext.first_discard_rectangle = firstDiscardRectangle;
   cmd->u.set_discard_rectangle_ext.discard_rectangle_count = discardRectangleCount;
   if (pDiscardRectangles) {
      cmd->u.set_discard_rectangle_ext.discard_rectangles = vk_zalloc(queue->alloc, sizeof(*cmd->u.set_discard_rectangle_ext.discard_rectangles) * (discardRectangleCount), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (cmd->u.set_discard_rectangle_ext.discard_rectangles == NULL) goto err;

   memcpy(( VkRect2D* )cmd->u.set_discard_rectangle_ext.discard_rectangles, pDiscardRectangles, sizeof(*cmd->u.set_discard_rectangle_ext.discard_rectangles) * (discardRectangleCount));
   }   

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

err:
   if (cmd)
      vk_free_cmd_set_discard_rectangle_ext(queue, cmd);
   return VK_ERROR_OUT_OF_HOST_MEMORY;
}

static void
vk_free_cmd_set_sample_locations_ext(struct vk_cmd_queue *queue,
                                     struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, ( VkSampleLocationEXT* )cmd->u.set_sample_locations_ext.sample_locations_info->pSampleLocations);
      vk_free(queue->alloc, ( VkSampleLocationsInfoEXT* )cmd->u.set_sample_locations_ext.sample_locations_info);

   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_set_sample_locations_ext(struct vk_cmd_queue *queue
, const VkSampleLocationsInfoEXT* pSampleLocationsInfo
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_SET_SAMPLE_LOCATIONS_EXT;
      
   if (pSampleLocationsInfo) {
      cmd->u.set_sample_locations_ext.sample_locations_info = vk_zalloc(queue->alloc, sizeof(VkSampleLocationsInfoEXT), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
      if (cmd->u.set_sample_locations_ext.sample_locations_info == NULL) goto err;

      memcpy((void*)cmd->u.set_sample_locations_ext.sample_locations_info, pSampleLocationsInfo, sizeof(VkSampleLocationsInfoEXT));
   VkSampleLocationsInfoEXT *tmp_dst1 = (void *) cmd->u.set_sample_locations_ext.sample_locations_info; (void) tmp_dst1;
   VkSampleLocationsInfoEXT *tmp_src1 = (void *) pSampleLocationsInfo; (void) tmp_src1;   
   if (tmp_src1->pSampleLocations) {
   tmp_dst1->pSampleLocations = vk_zalloc(queue->alloc, sizeof(*tmp_dst1->pSampleLocations) * tmp_dst1->sampleLocationsCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (tmp_dst1->pSampleLocations == NULL) goto err;

   memcpy(( VkSampleLocationEXT* )tmp_dst1->pSampleLocations, tmp_src1->pSampleLocations, sizeof(*tmp_dst1->pSampleLocations) * tmp_dst1->sampleLocationsCount);
}
   } else {
      cmd->u.set_sample_locations_ext.sample_locations_info = NULL;
   }   

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

err:
   if (cmd)
      vk_free_cmd_set_sample_locations_ext(queue, cmd);
   return VK_ERROR_OUT_OF_HOST_MEMORY;
}

static void
vk_free_cmd_begin_debug_utils_label_ext(struct vk_cmd_queue *queue,
                                        struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
         vk_free(queue->alloc, ( VkDebugUtilsLabelEXT* )cmd->u.begin_debug_utils_label_ext.label_info);

   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_begin_debug_utils_label_ext(struct vk_cmd_queue *queue
, const VkDebugUtilsLabelEXT* pLabelInfo
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_BEGIN_DEBUG_UTILS_LABEL_EXT;
      
   if (pLabelInfo) {
      cmd->u.begin_debug_utils_label_ext.label_info = vk_zalloc(queue->alloc, sizeof(VkDebugUtilsLabelEXT), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
      if (cmd->u.begin_debug_utils_label_ext.label_info == NULL) goto err;

      memcpy((void*)cmd->u.begin_debug_utils_label_ext.label_info, pLabelInfo, sizeof(VkDebugUtilsLabelEXT));
   VkDebugUtilsLabelEXT *tmp_dst1 = (void *) cmd->u.begin_debug_utils_label_ext.label_info; (void) tmp_dst1;
   VkDebugUtilsLabelEXT *tmp_src1 = (void *) pLabelInfo; (void) tmp_src1;   
      } else {
      cmd->u.begin_debug_utils_label_ext.label_info = NULL;
   }   

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

err:
   if (cmd)
      vk_free_cmd_begin_debug_utils_label_ext(queue, cmd);
   return VK_ERROR_OUT_OF_HOST_MEMORY;
}

static void
vk_free_cmd_end_debug_utils_label_ext(struct vk_cmd_queue *queue,
                                      struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_end_debug_utils_label_ext(struct vk_cmd_queue *queue
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_END_DEBUG_UTILS_LABEL_EXT;
      

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

}

static void
vk_free_cmd_insert_debug_utils_label_ext(struct vk_cmd_queue *queue,
                                         struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
         vk_free(queue->alloc, ( VkDebugUtilsLabelEXT* )cmd->u.insert_debug_utils_label_ext.label_info);

   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_insert_debug_utils_label_ext(struct vk_cmd_queue *queue
, const VkDebugUtilsLabelEXT* pLabelInfo
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_INSERT_DEBUG_UTILS_LABEL_EXT;
      
   if (pLabelInfo) {
      cmd->u.insert_debug_utils_label_ext.label_info = vk_zalloc(queue->alloc, sizeof(VkDebugUtilsLabelEXT), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
      if (cmd->u.insert_debug_utils_label_ext.label_info == NULL) goto err;

      memcpy((void*)cmd->u.insert_debug_utils_label_ext.label_info, pLabelInfo, sizeof(VkDebugUtilsLabelEXT));
   VkDebugUtilsLabelEXT *tmp_dst1 = (void *) cmd->u.insert_debug_utils_label_ext.label_info; (void) tmp_dst1;
   VkDebugUtilsLabelEXT *tmp_src1 = (void *) pLabelInfo; (void) tmp_src1;   
      } else {
      cmd->u.insert_debug_utils_label_ext.label_info = NULL;
   }   

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

err:
   if (cmd)
      vk_free_cmd_insert_debug_utils_label_ext(queue, cmd);
   return VK_ERROR_OUT_OF_HOST_MEMORY;
}

static void
vk_free_cmd_write_buffer_marker_amd(struct vk_cmd_queue *queue,
                                    struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_write_buffer_marker_amd(struct vk_cmd_queue *queue
, VkPipelineStageFlagBits pipelineStage
, VkBuffer dstBuffer
, VkDeviceSize dstOffset
, uint32_t marker
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_WRITE_BUFFER_MARKER_AMD;
      
   cmd->u.write_buffer_marker_amd.pipeline_stage = pipelineStage;
   cmd->u.write_buffer_marker_amd.dst_buffer = dstBuffer;
   cmd->u.write_buffer_marker_amd.dst_offset = dstOffset;
   cmd->u.write_buffer_marker_amd.marker = marker;

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

}

static void
vk_free_cmd_begin_render_pass2(struct vk_cmd_queue *queue,
                               struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, ( VkClearValue*    )cmd->u.begin_render_pass2.render_pass_begin->pClearValues);
      vk_free(queue->alloc, ( VkRenderPassBeginInfo*      )cmd->u.begin_render_pass2.render_pass_begin);

         vk_free(queue->alloc, ( VkSubpassBeginInfo*      )cmd->u.begin_render_pass2.subpass_begin_info);

   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_begin_render_pass2(struct vk_cmd_queue *queue
, const VkRenderPassBeginInfo*      pRenderPassBegin
, const VkSubpassBeginInfo*      pSubpassBeginInfo
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_BEGIN_RENDER_PASS2;
      
   if (pRenderPassBegin) {
      cmd->u.begin_render_pass2.render_pass_begin = vk_zalloc(queue->alloc, sizeof(VkRenderPassBeginInfo), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
      if (cmd->u.begin_render_pass2.render_pass_begin == NULL) goto err;

      memcpy((void*)cmd->u.begin_render_pass2.render_pass_begin, pRenderPassBegin, sizeof(VkRenderPassBeginInfo));
   VkRenderPassBeginInfo *tmp_dst1 = (void *) cmd->u.begin_render_pass2.render_pass_begin; (void) tmp_dst1;
   VkRenderPassBeginInfo *tmp_src1 = (void *) pRenderPassBegin; (void) tmp_src1;   
   
      const VkBaseInStructure *pnext = tmp_dst1->pNext;
      if (pnext) {
         switch ((int32_t)pnext->sType) {
         
      case VK_STRUCTURE_TYPE_DEVICE_GROUP_RENDER_PASS_BEGIN_INFO:
         if (pnext) {
      tmp_dst1->pNext = vk_zalloc(queue->alloc, sizeof(VkDeviceGroupRenderPassBeginInfo), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
      if (tmp_dst1->pNext == NULL) goto err;

      memcpy((void*)tmp_dst1->pNext, pnext, sizeof(VkDeviceGroupRenderPassBeginInfo));
   VkDeviceGroupRenderPassBeginInfo *tmp_dst2 = (void *) tmp_dst1->pNext; (void) tmp_dst2;
   VkDeviceGroupRenderPassBeginInfo *tmp_src2 = (void *) pnext; (void) tmp_src2;   
   if (tmp_src2->pDeviceRenderAreas) {
   tmp_dst2->pDeviceRenderAreas = vk_zalloc(queue->alloc, sizeof(*tmp_dst2->pDeviceRenderAreas) * tmp_dst2->deviceRenderAreaCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (tmp_dst2->pDeviceRenderAreas == NULL) goto err;

   memcpy(( VkRect2D*  )tmp_dst2->pDeviceRenderAreas, tmp_src2->pDeviceRenderAreas, sizeof(*tmp_dst2->pDeviceRenderAreas) * tmp_dst2->deviceRenderAreaCount);
}
   } else {
      tmp_dst1->pNext = NULL;
   }
         break;
      
      case VK_STRUCTURE_TYPE_RENDER_PASS_SAMPLE_LOCATIONS_BEGIN_INFO_EXT:
         if (pnext) {
      tmp_dst1->pNext = vk_zalloc(queue->alloc, sizeof(VkRenderPassSampleLocationsBeginInfoEXT), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
      if (tmp_dst1->pNext == NULL) goto err;

      memcpy((void*)tmp_dst1->pNext, pnext, sizeof(VkRenderPassSampleLocationsBeginInfoEXT));
   VkRenderPassSampleLocationsBeginInfoEXT *tmp_dst2 = (void *) tmp_dst1->pNext; (void) tmp_dst2;
   VkRenderPassSampleLocationsBeginInfoEXT *tmp_src2 = (void *) pnext; (void) tmp_src2;   
   if (tmp_src2->pAttachmentInitialSampleLocations) {
   tmp_dst2->pAttachmentInitialSampleLocations = vk_zalloc(queue->alloc, sizeof(*tmp_dst2->pAttachmentInitialSampleLocations) * tmp_dst2->attachmentInitialSampleLocationsCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (tmp_dst2->pAttachmentInitialSampleLocations == NULL) goto err;

   memcpy(( VkAttachmentSampleLocationsEXT* )tmp_dst2->pAttachmentInitialSampleLocations, tmp_src2->pAttachmentInitialSampleLocations, sizeof(*tmp_dst2->pAttachmentInitialSampleLocations) * tmp_dst2->attachmentInitialSampleLocationsCount);
}
if (tmp_src2->pPostSubpassSampleLocations) {
   tmp_dst2->pPostSubpassSampleLocations = vk_zalloc(queue->alloc, sizeof(*tmp_dst2->pPostSubpassSampleLocations) * tmp_dst2->postSubpassSampleLocationsCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (tmp_dst2->pPostSubpassSampleLocations == NULL) goto err;

   memcpy(( VkSubpassSampleLocationsEXT* )tmp_dst2->pPostSubpassSampleLocations, tmp_src2->pPostSubpassSampleLocations, sizeof(*tmp_dst2->pPostSubpassSampleLocations) * tmp_dst2->postSubpassSampleLocationsCount);
}
   } else {
      tmp_dst1->pNext = NULL;
   }
         break;
      
      case VK_STRUCTURE_TYPE_RENDER_PASS_ATTACHMENT_BEGIN_INFO:
         if (pnext) {
      tmp_dst1->pNext = vk_zalloc(queue->alloc, sizeof(VkRenderPassAttachmentBeginInfo), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
      if (tmp_dst1->pNext == NULL) goto err;

      memcpy((void*)tmp_dst1->pNext, pnext, sizeof(VkRenderPassAttachmentBeginInfo));
   VkRenderPassAttachmentBeginInfo *tmp_dst2 = (void *) tmp_dst1->pNext; (void) tmp_dst2;
   VkRenderPassAttachmentBeginInfo *tmp_src2 = (void *) pnext; (void) tmp_src2;   
   if (tmp_src2->pAttachments) {
   tmp_dst2->pAttachments = vk_zalloc(queue->alloc, sizeof(*tmp_dst2->pAttachments) * tmp_dst2->attachmentCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (tmp_dst2->pAttachments == NULL) goto err;

   memcpy(( VkImageView* )tmp_dst2->pAttachments, tmp_src2->pAttachments, sizeof(*tmp_dst2->pAttachments) * tmp_dst2->attachmentCount);
}
   } else {
      tmp_dst1->pNext = NULL;
   }
         break;
      
      case VK_STRUCTURE_TYPE_RENDER_PASS_TRANSFORM_BEGIN_INFO_QCOM:
         if (pnext) {
      tmp_dst1->pNext = vk_zalloc(queue->alloc, sizeof(VkRenderPassTransformBeginInfoQCOM), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
      if (tmp_dst1->pNext == NULL) goto err;

      memcpy((void*)tmp_dst1->pNext, pnext, sizeof(VkRenderPassTransformBeginInfoQCOM));
   VkRenderPassTransformBeginInfoQCOM *tmp_dst2 = (void *) tmp_dst1->pNext; (void) tmp_dst2;
   VkRenderPassTransformBeginInfoQCOM *tmp_src2 = (void *) pnext; (void) tmp_src2;   
      } else {
      tmp_dst1->pNext = NULL;
   }
         break;
      
         }
      }
      if (tmp_src1->pClearValues) {
   tmp_dst1->pClearValues = vk_zalloc(queue->alloc, sizeof(*tmp_dst1->pClearValues) * tmp_dst1->clearValueCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (tmp_dst1->pClearValues == NULL) goto err;

   memcpy(( VkClearValue*    )tmp_dst1->pClearValues, tmp_src1->pClearValues, sizeof(*tmp_dst1->pClearValues) * tmp_dst1->clearValueCount);
}
   } else {
      cmd->u.begin_render_pass2.render_pass_begin = NULL;
   }   
   if (pSubpassBeginInfo) {
      cmd->u.begin_render_pass2.subpass_begin_info = vk_zalloc(queue->alloc, sizeof(VkSubpassBeginInfo), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
      if (cmd->u.begin_render_pass2.subpass_begin_info == NULL) goto err;

      memcpy((void*)cmd->u.begin_render_pass2.subpass_begin_info, pSubpassBeginInfo, sizeof(VkSubpassBeginInfo));
   VkSubpassBeginInfo *tmp_dst1 = (void *) cmd->u.begin_render_pass2.subpass_begin_info; (void) tmp_dst1;
   VkSubpassBeginInfo *tmp_src1 = (void *) pSubpassBeginInfo; (void) tmp_src1;   
      } else {
      cmd->u.begin_render_pass2.subpass_begin_info = NULL;
   }   

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

err:
   if (cmd)
      vk_free_cmd_begin_render_pass2(queue, cmd);
   return VK_ERROR_OUT_OF_HOST_MEMORY;
}

static void
vk_free_cmd_next_subpass2(struct vk_cmd_queue *queue,
                          struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
         vk_free(queue->alloc, ( VkSubpassBeginInfo*      )cmd->u.next_subpass2.subpass_begin_info);

         vk_free(queue->alloc, ( VkSubpassEndInfo*        )cmd->u.next_subpass2.subpass_end_info);

   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_next_subpass2(struct vk_cmd_queue *queue
, const VkSubpassBeginInfo*      pSubpassBeginInfo
, const VkSubpassEndInfo*        pSubpassEndInfo
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_NEXT_SUBPASS2;
      
   if (pSubpassBeginInfo) {
      cmd->u.next_subpass2.subpass_begin_info = vk_zalloc(queue->alloc, sizeof(VkSubpassBeginInfo), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
      if (cmd->u.next_subpass2.subpass_begin_info == NULL) goto err;

      memcpy((void*)cmd->u.next_subpass2.subpass_begin_info, pSubpassBeginInfo, sizeof(VkSubpassBeginInfo));
   VkSubpassBeginInfo *tmp_dst1 = (void *) cmd->u.next_subpass2.subpass_begin_info; (void) tmp_dst1;
   VkSubpassBeginInfo *tmp_src1 = (void *) pSubpassBeginInfo; (void) tmp_src1;   
      } else {
      cmd->u.next_subpass2.subpass_begin_info = NULL;
   }   
   if (pSubpassEndInfo) {
      cmd->u.next_subpass2.subpass_end_info = vk_zalloc(queue->alloc, sizeof(VkSubpassEndInfo), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
      if (cmd->u.next_subpass2.subpass_end_info == NULL) goto err;

      memcpy((void*)cmd->u.next_subpass2.subpass_end_info, pSubpassEndInfo, sizeof(VkSubpassEndInfo));
   VkSubpassEndInfo *tmp_dst1 = (void *) cmd->u.next_subpass2.subpass_end_info; (void) tmp_dst1;
   VkSubpassEndInfo *tmp_src1 = (void *) pSubpassEndInfo; (void) tmp_src1;   
   
      const VkBaseInStructure *pnext = tmp_dst1->pNext;
      if (pnext) {
         switch ((int32_t)pnext->sType) {
         
      case VK_STRUCTURE_TYPE_SUBPASS_FRAGMENT_DENSITY_MAP_OFFSET_END_INFO_QCOM:
         if (pnext) {
      tmp_dst1->pNext = vk_zalloc(queue->alloc, sizeof(VkSubpassFragmentDensityMapOffsetEndInfoQCOM), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
      if (tmp_dst1->pNext == NULL) goto err;

      memcpy((void*)tmp_dst1->pNext, pnext, sizeof(VkSubpassFragmentDensityMapOffsetEndInfoQCOM));
   VkSubpassFragmentDensityMapOffsetEndInfoQCOM *tmp_dst2 = (void *) tmp_dst1->pNext; (void) tmp_dst2;
   VkSubpassFragmentDensityMapOffsetEndInfoQCOM *tmp_src2 = (void *) pnext; (void) tmp_src2;   
   if (tmp_src2->pFragmentDensityOffsets) {
   tmp_dst2->pFragmentDensityOffsets = vk_zalloc(queue->alloc, sizeof(*tmp_dst2->pFragmentDensityOffsets) * tmp_dst2->fragmentDensityOffsetCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (tmp_dst2->pFragmentDensityOffsets == NULL) goto err;

   memcpy(( VkOffset2D* )tmp_dst2->pFragmentDensityOffsets, tmp_src2->pFragmentDensityOffsets, sizeof(*tmp_dst2->pFragmentDensityOffsets) * tmp_dst2->fragmentDensityOffsetCount);
}
   } else {
      tmp_dst1->pNext = NULL;
   }
         break;
      
         }
      }
         } else {
      cmd->u.next_subpass2.subpass_end_info = NULL;
   }   

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

err:
   if (cmd)
      vk_free_cmd_next_subpass2(queue, cmd);
   return VK_ERROR_OUT_OF_HOST_MEMORY;
}

static void
vk_free_cmd_end_render_pass2(struct vk_cmd_queue *queue,
                             struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
         vk_free(queue->alloc, ( VkSubpassEndInfo*        )cmd->u.end_render_pass2.subpass_end_info);

   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_end_render_pass2(struct vk_cmd_queue *queue
, const VkSubpassEndInfo*        pSubpassEndInfo
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_END_RENDER_PASS2;
      
   if (pSubpassEndInfo) {
      cmd->u.end_render_pass2.subpass_end_info = vk_zalloc(queue->alloc, sizeof(VkSubpassEndInfo), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
      if (cmd->u.end_render_pass2.subpass_end_info == NULL) goto err;

      memcpy((void*)cmd->u.end_render_pass2.subpass_end_info, pSubpassEndInfo, sizeof(VkSubpassEndInfo));
   VkSubpassEndInfo *tmp_dst1 = (void *) cmd->u.end_render_pass2.subpass_end_info; (void) tmp_dst1;
   VkSubpassEndInfo *tmp_src1 = (void *) pSubpassEndInfo; (void) tmp_src1;   
   
      const VkBaseInStructure *pnext = tmp_dst1->pNext;
      if (pnext) {
         switch ((int32_t)pnext->sType) {
         
      case VK_STRUCTURE_TYPE_SUBPASS_FRAGMENT_DENSITY_MAP_OFFSET_END_INFO_QCOM:
         if (pnext) {
      tmp_dst1->pNext = vk_zalloc(queue->alloc, sizeof(VkSubpassFragmentDensityMapOffsetEndInfoQCOM), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
      if (tmp_dst1->pNext == NULL) goto err;

      memcpy((void*)tmp_dst1->pNext, pnext, sizeof(VkSubpassFragmentDensityMapOffsetEndInfoQCOM));
   VkSubpassFragmentDensityMapOffsetEndInfoQCOM *tmp_dst2 = (void *) tmp_dst1->pNext; (void) tmp_dst2;
   VkSubpassFragmentDensityMapOffsetEndInfoQCOM *tmp_src2 = (void *) pnext; (void) tmp_src2;   
   if (tmp_src2->pFragmentDensityOffsets) {
   tmp_dst2->pFragmentDensityOffsets = vk_zalloc(queue->alloc, sizeof(*tmp_dst2->pFragmentDensityOffsets) * tmp_dst2->fragmentDensityOffsetCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (tmp_dst2->pFragmentDensityOffsets == NULL) goto err;

   memcpy(( VkOffset2D* )tmp_dst2->pFragmentDensityOffsets, tmp_src2->pFragmentDensityOffsets, sizeof(*tmp_dst2->pFragmentDensityOffsets) * tmp_dst2->fragmentDensityOffsetCount);
}
   } else {
      tmp_dst1->pNext = NULL;
   }
         break;
      
         }
      }
         } else {
      cmd->u.end_render_pass2.subpass_end_info = NULL;
   }   

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

err:
   if (cmd)
      vk_free_cmd_end_render_pass2(queue, cmd);
   return VK_ERROR_OUT_OF_HOST_MEMORY;
}

static void
vk_free_cmd_draw_indirect_count(struct vk_cmd_queue *queue,
                                struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_draw_indirect_count(struct vk_cmd_queue *queue
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
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_DRAW_INDIRECT_COUNT;
      
   cmd->u.draw_indirect_count.buffer = buffer;
   cmd->u.draw_indirect_count.offset = offset;
   cmd->u.draw_indirect_count.count_buffer = countBuffer;
   cmd->u.draw_indirect_count.count_buffer_offset = countBufferOffset;
   cmd->u.draw_indirect_count.max_draw_count = maxDrawCount;
   cmd->u.draw_indirect_count.stride = stride;

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

}

static void
vk_free_cmd_draw_indexed_indirect_count(struct vk_cmd_queue *queue,
                                        struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_draw_indexed_indirect_count(struct vk_cmd_queue *queue
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
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_DRAW_INDEXED_INDIRECT_COUNT;
      
   cmd->u.draw_indexed_indirect_count.buffer = buffer;
   cmd->u.draw_indexed_indirect_count.offset = offset;
   cmd->u.draw_indexed_indirect_count.count_buffer = countBuffer;
   cmd->u.draw_indexed_indirect_count.count_buffer_offset = countBufferOffset;
   cmd->u.draw_indexed_indirect_count.max_draw_count = maxDrawCount;
   cmd->u.draw_indexed_indirect_count.stride = stride;

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

}

static void
vk_free_cmd_set_checkpoint_nv(struct vk_cmd_queue *queue,
                              struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
         vk_free(queue->alloc, ( void* )cmd->u.set_checkpoint_nv.checkpoint_marker);

   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_set_checkpoint_nv(struct vk_cmd_queue *queue
, const void* pCheckpointMarker
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_SET_CHECKPOINT_NV;
      
   cmd->u.set_checkpoint_nv.checkpoint_marker = ( void* ) pCheckpointMarker;

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

}

static void
vk_free_cmd_bind_transform_feedback_buffers_ext(struct vk_cmd_queue *queue,
                                                struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, ( VkBuffer* )cmd->u.bind_transform_feedback_buffers_ext.buffers);
   vk_free(queue->alloc, ( VkDeviceSize* )cmd->u.bind_transform_feedback_buffers_ext.offsets);
   vk_free(queue->alloc, ( VkDeviceSize* )cmd->u.bind_transform_feedback_buffers_ext.sizes);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_bind_transform_feedback_buffers_ext(struct vk_cmd_queue *queue
, uint32_t firstBinding
, uint32_t bindingCount
, const VkBuffer* pBuffers
, const VkDeviceSize* pOffsets
, const VkDeviceSize* pSizes
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_BIND_TRANSFORM_FEEDBACK_BUFFERS_EXT;
      
   cmd->u.bind_transform_feedback_buffers_ext.first_binding = firstBinding;
   cmd->u.bind_transform_feedback_buffers_ext.binding_count = bindingCount;
   if (pBuffers) {
      cmd->u.bind_transform_feedback_buffers_ext.buffers = vk_zalloc(queue->alloc, sizeof(*cmd->u.bind_transform_feedback_buffers_ext.buffers) * (bindingCount), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (cmd->u.bind_transform_feedback_buffers_ext.buffers == NULL) goto err;

   memcpy(( VkBuffer* )cmd->u.bind_transform_feedback_buffers_ext.buffers, pBuffers, sizeof(*cmd->u.bind_transform_feedback_buffers_ext.buffers) * (bindingCount));
   }   
   if (pOffsets) {
      cmd->u.bind_transform_feedback_buffers_ext.offsets = vk_zalloc(queue->alloc, sizeof(*cmd->u.bind_transform_feedback_buffers_ext.offsets) * (bindingCount), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (cmd->u.bind_transform_feedback_buffers_ext.offsets == NULL) goto err;

   memcpy(( VkDeviceSize* )cmd->u.bind_transform_feedback_buffers_ext.offsets, pOffsets, sizeof(*cmd->u.bind_transform_feedback_buffers_ext.offsets) * (bindingCount));
   }   
   if (pSizes) {
      cmd->u.bind_transform_feedback_buffers_ext.sizes = vk_zalloc(queue->alloc, sizeof(*cmd->u.bind_transform_feedback_buffers_ext.sizes) * (bindingCount), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (cmd->u.bind_transform_feedback_buffers_ext.sizes == NULL) goto err;

   memcpy(( VkDeviceSize* )cmd->u.bind_transform_feedback_buffers_ext.sizes, pSizes, sizeof(*cmd->u.bind_transform_feedback_buffers_ext.sizes) * (bindingCount));
   }   

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

err:
   if (cmd)
      vk_free_cmd_bind_transform_feedback_buffers_ext(queue, cmd);
   return VK_ERROR_OUT_OF_HOST_MEMORY;
}

static void
vk_free_cmd_begin_transform_feedback_ext(struct vk_cmd_queue *queue,
                                         struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, ( VkBuffer* )cmd->u.begin_transform_feedback_ext.counter_buffers);
   vk_free(queue->alloc, ( VkDeviceSize* )cmd->u.begin_transform_feedback_ext.counter_buffer_offsets);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_begin_transform_feedback_ext(struct vk_cmd_queue *queue
, uint32_t firstCounterBuffer
, uint32_t counterBufferCount
, const VkBuffer* pCounterBuffers
, const VkDeviceSize* pCounterBufferOffsets
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_BEGIN_TRANSFORM_FEEDBACK_EXT;
      
   cmd->u.begin_transform_feedback_ext.first_counter_buffer = firstCounterBuffer;
   cmd->u.begin_transform_feedback_ext.counter_buffer_count = counterBufferCount;
   if (pCounterBuffers) {
      cmd->u.begin_transform_feedback_ext.counter_buffers = vk_zalloc(queue->alloc, sizeof(*cmd->u.begin_transform_feedback_ext.counter_buffers) * (counterBufferCount), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (cmd->u.begin_transform_feedback_ext.counter_buffers == NULL) goto err;

   memcpy(( VkBuffer* )cmd->u.begin_transform_feedback_ext.counter_buffers, pCounterBuffers, sizeof(*cmd->u.begin_transform_feedback_ext.counter_buffers) * (counterBufferCount));
   }   
   if (pCounterBufferOffsets) {
      cmd->u.begin_transform_feedback_ext.counter_buffer_offsets = vk_zalloc(queue->alloc, sizeof(*cmd->u.begin_transform_feedback_ext.counter_buffer_offsets) * (counterBufferCount), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (cmd->u.begin_transform_feedback_ext.counter_buffer_offsets == NULL) goto err;

   memcpy(( VkDeviceSize* )cmd->u.begin_transform_feedback_ext.counter_buffer_offsets, pCounterBufferOffsets, sizeof(*cmd->u.begin_transform_feedback_ext.counter_buffer_offsets) * (counterBufferCount));
   }   

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

err:
   if (cmd)
      vk_free_cmd_begin_transform_feedback_ext(queue, cmd);
   return VK_ERROR_OUT_OF_HOST_MEMORY;
}

static void
vk_free_cmd_end_transform_feedback_ext(struct vk_cmd_queue *queue,
                                       struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, ( VkBuffer* )cmd->u.end_transform_feedback_ext.counter_buffers);
   vk_free(queue->alloc, ( VkDeviceSize* )cmd->u.end_transform_feedback_ext.counter_buffer_offsets);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_end_transform_feedback_ext(struct vk_cmd_queue *queue
, uint32_t firstCounterBuffer
, uint32_t counterBufferCount
, const VkBuffer* pCounterBuffers
, const VkDeviceSize* pCounterBufferOffsets
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_END_TRANSFORM_FEEDBACK_EXT;
      
   cmd->u.end_transform_feedback_ext.first_counter_buffer = firstCounterBuffer;
   cmd->u.end_transform_feedback_ext.counter_buffer_count = counterBufferCount;
   if (pCounterBuffers) {
      cmd->u.end_transform_feedback_ext.counter_buffers = vk_zalloc(queue->alloc, sizeof(*cmd->u.end_transform_feedback_ext.counter_buffers) * (counterBufferCount), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (cmd->u.end_transform_feedback_ext.counter_buffers == NULL) goto err;

   memcpy(( VkBuffer* )cmd->u.end_transform_feedback_ext.counter_buffers, pCounterBuffers, sizeof(*cmd->u.end_transform_feedback_ext.counter_buffers) * (counterBufferCount));
   }   
   if (pCounterBufferOffsets) {
      cmd->u.end_transform_feedback_ext.counter_buffer_offsets = vk_zalloc(queue->alloc, sizeof(*cmd->u.end_transform_feedback_ext.counter_buffer_offsets) * (counterBufferCount), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (cmd->u.end_transform_feedback_ext.counter_buffer_offsets == NULL) goto err;

   memcpy(( VkDeviceSize* )cmd->u.end_transform_feedback_ext.counter_buffer_offsets, pCounterBufferOffsets, sizeof(*cmd->u.end_transform_feedback_ext.counter_buffer_offsets) * (counterBufferCount));
   }   

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

err:
   if (cmd)
      vk_free_cmd_end_transform_feedback_ext(queue, cmd);
   return VK_ERROR_OUT_OF_HOST_MEMORY;
}

static void
vk_free_cmd_begin_query_indexed_ext(struct vk_cmd_queue *queue,
                                    struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_begin_query_indexed_ext(struct vk_cmd_queue *queue
, VkQueryPool queryPool
, uint32_t query
, VkQueryControlFlags flags
, uint32_t index
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_BEGIN_QUERY_INDEXED_EXT;
      
   cmd->u.begin_query_indexed_ext.query_pool = queryPool;
   cmd->u.begin_query_indexed_ext.query = query;
   cmd->u.begin_query_indexed_ext.flags = flags;
   cmd->u.begin_query_indexed_ext.index = index;

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

}

static void
vk_free_cmd_end_query_indexed_ext(struct vk_cmd_queue *queue,
                                  struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_end_query_indexed_ext(struct vk_cmd_queue *queue
, VkQueryPool queryPool
, uint32_t query
, uint32_t index
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_END_QUERY_INDEXED_EXT;
      
   cmd->u.end_query_indexed_ext.query_pool = queryPool;
   cmd->u.end_query_indexed_ext.query = query;
   cmd->u.end_query_indexed_ext.index = index;

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

}

static void
vk_free_cmd_draw_indirect_byte_count_ext(struct vk_cmd_queue *queue,
                                         struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_draw_indirect_byte_count_ext(struct vk_cmd_queue *queue
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
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_DRAW_INDIRECT_BYTE_COUNT_EXT;
      
   cmd->u.draw_indirect_byte_count_ext.instance_count = instanceCount;
   cmd->u.draw_indirect_byte_count_ext.first_instance = firstInstance;
   cmd->u.draw_indirect_byte_count_ext.counter_buffer = counterBuffer;
   cmd->u.draw_indirect_byte_count_ext.counter_buffer_offset = counterBufferOffset;
   cmd->u.draw_indirect_byte_count_ext.counter_offset = counterOffset;
   cmd->u.draw_indirect_byte_count_ext.vertex_stride = vertexStride;

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

}

static void
vk_free_cmd_set_exclusive_scissor_nv(struct vk_cmd_queue *queue,
                                     struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, ( VkRect2D* )cmd->u.set_exclusive_scissor_nv.exclusive_scissors);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_set_exclusive_scissor_nv(struct vk_cmd_queue *queue
, uint32_t firstExclusiveScissor
, uint32_t exclusiveScissorCount
, const VkRect2D* pExclusiveScissors
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_SET_EXCLUSIVE_SCISSOR_NV;
      
   cmd->u.set_exclusive_scissor_nv.first_exclusive_scissor = firstExclusiveScissor;
   cmd->u.set_exclusive_scissor_nv.exclusive_scissor_count = exclusiveScissorCount;
   if (pExclusiveScissors) {
      cmd->u.set_exclusive_scissor_nv.exclusive_scissors = vk_zalloc(queue->alloc, sizeof(*cmd->u.set_exclusive_scissor_nv.exclusive_scissors) * (exclusiveScissorCount), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (cmd->u.set_exclusive_scissor_nv.exclusive_scissors == NULL) goto err;

   memcpy(( VkRect2D* )cmd->u.set_exclusive_scissor_nv.exclusive_scissors, pExclusiveScissors, sizeof(*cmd->u.set_exclusive_scissor_nv.exclusive_scissors) * (exclusiveScissorCount));
   }   

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

err:
   if (cmd)
      vk_free_cmd_set_exclusive_scissor_nv(queue, cmd);
   return VK_ERROR_OUT_OF_HOST_MEMORY;
}

static void
vk_free_cmd_bind_shading_rate_image_nv(struct vk_cmd_queue *queue,
                                       struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_bind_shading_rate_image_nv(struct vk_cmd_queue *queue
, VkImageView imageView
, VkImageLayout imageLayout
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_BIND_SHADING_RATE_IMAGE_NV;
      
   cmd->u.bind_shading_rate_image_nv.image_view = imageView;
   cmd->u.bind_shading_rate_image_nv.image_layout = imageLayout;

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

}

static void
vk_free_cmd_set_viewport_shading_rate_palette_nv(struct vk_cmd_queue *queue,
                                                 struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, ( VkShadingRatePaletteNV* )cmd->u.set_viewport_shading_rate_palette_nv.shading_rate_palettes);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_set_viewport_shading_rate_palette_nv(struct vk_cmd_queue *queue
, uint32_t firstViewport
, uint32_t viewportCount
, const VkShadingRatePaletteNV* pShadingRatePalettes
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_SET_VIEWPORT_SHADING_RATE_PALETTE_NV;
      
   cmd->u.set_viewport_shading_rate_palette_nv.first_viewport = firstViewport;
   cmd->u.set_viewport_shading_rate_palette_nv.viewport_count = viewportCount;
   if (pShadingRatePalettes) {
      cmd->u.set_viewport_shading_rate_palette_nv.shading_rate_palettes = vk_zalloc(queue->alloc, sizeof(*cmd->u.set_viewport_shading_rate_palette_nv.shading_rate_palettes) * (viewportCount), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (cmd->u.set_viewport_shading_rate_palette_nv.shading_rate_palettes == NULL) goto err;

   memcpy(( VkShadingRatePaletteNV* )cmd->u.set_viewport_shading_rate_palette_nv.shading_rate_palettes, pShadingRatePalettes, sizeof(*cmd->u.set_viewport_shading_rate_palette_nv.shading_rate_palettes) * (viewportCount));
   }   

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

err:
   if (cmd)
      vk_free_cmd_set_viewport_shading_rate_palette_nv(queue, cmd);
   return VK_ERROR_OUT_OF_HOST_MEMORY;
}

static void
vk_free_cmd_set_coarse_sample_order_nv(struct vk_cmd_queue *queue,
                                       struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, ( VkCoarseSampleOrderCustomNV* )cmd->u.set_coarse_sample_order_nv.custom_sample_orders);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_set_coarse_sample_order_nv(struct vk_cmd_queue *queue
, VkCoarseSampleOrderTypeNV sampleOrderType
, uint32_t customSampleOrderCount
, const VkCoarseSampleOrderCustomNV* pCustomSampleOrders
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_SET_COARSE_SAMPLE_ORDER_NV;
      
   cmd->u.set_coarse_sample_order_nv.sample_order_type = sampleOrderType;
   cmd->u.set_coarse_sample_order_nv.custom_sample_order_count = customSampleOrderCount;
   if (pCustomSampleOrders) {
      cmd->u.set_coarse_sample_order_nv.custom_sample_orders = vk_zalloc(queue->alloc, sizeof(*cmd->u.set_coarse_sample_order_nv.custom_sample_orders) * (customSampleOrderCount), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (cmd->u.set_coarse_sample_order_nv.custom_sample_orders == NULL) goto err;

   memcpy(( VkCoarseSampleOrderCustomNV* )cmd->u.set_coarse_sample_order_nv.custom_sample_orders, pCustomSampleOrders, sizeof(*cmd->u.set_coarse_sample_order_nv.custom_sample_orders) * (customSampleOrderCount));
   }   

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

err:
   if (cmd)
      vk_free_cmd_set_coarse_sample_order_nv(queue, cmd);
   return VK_ERROR_OUT_OF_HOST_MEMORY;
}

static void
vk_free_cmd_draw_mesh_tasks_nv(struct vk_cmd_queue *queue,
                               struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_draw_mesh_tasks_nv(struct vk_cmd_queue *queue
, uint32_t taskCount
, uint32_t firstTask
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_DRAW_MESH_TASKS_NV;
      
   cmd->u.draw_mesh_tasks_nv.task_count = taskCount;
   cmd->u.draw_mesh_tasks_nv.first_task = firstTask;

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

}

static void
vk_free_cmd_draw_mesh_tasks_indirect_nv(struct vk_cmd_queue *queue,
                                        struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_draw_mesh_tasks_indirect_nv(struct vk_cmd_queue *queue
, VkBuffer buffer
, VkDeviceSize offset
, uint32_t drawCount
, uint32_t stride
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_DRAW_MESH_TASKS_INDIRECT_NV;
      
   cmd->u.draw_mesh_tasks_indirect_nv.buffer = buffer;
   cmd->u.draw_mesh_tasks_indirect_nv.offset = offset;
   cmd->u.draw_mesh_tasks_indirect_nv.draw_count = drawCount;
   cmd->u.draw_mesh_tasks_indirect_nv.stride = stride;

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

}

static void
vk_free_cmd_draw_mesh_tasks_indirect_count_nv(struct vk_cmd_queue *queue,
                                              struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_draw_mesh_tasks_indirect_count_nv(struct vk_cmd_queue *queue
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
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_DRAW_MESH_TASKS_INDIRECT_COUNT_NV;
      
   cmd->u.draw_mesh_tasks_indirect_count_nv.buffer = buffer;
   cmd->u.draw_mesh_tasks_indirect_count_nv.offset = offset;
   cmd->u.draw_mesh_tasks_indirect_count_nv.count_buffer = countBuffer;
   cmd->u.draw_mesh_tasks_indirect_count_nv.count_buffer_offset = countBufferOffset;
   cmd->u.draw_mesh_tasks_indirect_count_nv.max_draw_count = maxDrawCount;
   cmd->u.draw_mesh_tasks_indirect_count_nv.stride = stride;

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

}

static void
vk_free_cmd_draw_mesh_tasks_ext(struct vk_cmd_queue *queue,
                                struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_draw_mesh_tasks_ext(struct vk_cmd_queue *queue
, uint32_t groupCountX
, uint32_t groupCountY
, uint32_t groupCountZ
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_DRAW_MESH_TASKS_EXT;
      
   cmd->u.draw_mesh_tasks_ext.group_count_x = groupCountX;
   cmd->u.draw_mesh_tasks_ext.group_count_y = groupCountY;
   cmd->u.draw_mesh_tasks_ext.group_count_z = groupCountZ;

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

}

static void
vk_free_cmd_draw_mesh_tasks_indirect_ext(struct vk_cmd_queue *queue,
                                         struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_draw_mesh_tasks_indirect_ext(struct vk_cmd_queue *queue
, VkBuffer buffer
, VkDeviceSize offset
, uint32_t drawCount
, uint32_t stride
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_DRAW_MESH_TASKS_INDIRECT_EXT;
      
   cmd->u.draw_mesh_tasks_indirect_ext.buffer = buffer;
   cmd->u.draw_mesh_tasks_indirect_ext.offset = offset;
   cmd->u.draw_mesh_tasks_indirect_ext.draw_count = drawCount;
   cmd->u.draw_mesh_tasks_indirect_ext.stride = stride;

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

}

static void
vk_free_cmd_draw_mesh_tasks_indirect_count_ext(struct vk_cmd_queue *queue,
                                               struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_draw_mesh_tasks_indirect_count_ext(struct vk_cmd_queue *queue
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
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_DRAW_MESH_TASKS_INDIRECT_COUNT_EXT;
      
   cmd->u.draw_mesh_tasks_indirect_count_ext.buffer = buffer;
   cmd->u.draw_mesh_tasks_indirect_count_ext.offset = offset;
   cmd->u.draw_mesh_tasks_indirect_count_ext.count_buffer = countBuffer;
   cmd->u.draw_mesh_tasks_indirect_count_ext.count_buffer_offset = countBufferOffset;
   cmd->u.draw_mesh_tasks_indirect_count_ext.max_draw_count = maxDrawCount;
   cmd->u.draw_mesh_tasks_indirect_count_ext.stride = stride;

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

}

static void
vk_free_cmd_bind_invocation_mask_huawei(struct vk_cmd_queue *queue,
                                        struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_bind_invocation_mask_huawei(struct vk_cmd_queue *queue
, VkImageView imageView
, VkImageLayout imageLayout
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_BIND_INVOCATION_MASK_HUAWEI;
      
   cmd->u.bind_invocation_mask_huawei.image_view = imageView;
   cmd->u.bind_invocation_mask_huawei.image_layout = imageLayout;

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

}

static void
vk_free_cmd_copy_acceleration_structure_nv(struct vk_cmd_queue *queue,
                                           struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_copy_acceleration_structure_nv(struct vk_cmd_queue *queue
, VkAccelerationStructureNV dst
, VkAccelerationStructureNV src
, VkCopyAccelerationStructureModeKHR mode
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_COPY_ACCELERATION_STRUCTURE_NV;
      
   cmd->u.copy_acceleration_structure_nv.dst = dst;
   cmd->u.copy_acceleration_structure_nv.src = src;
   cmd->u.copy_acceleration_structure_nv.mode = mode;

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

}

static void
vk_free_cmd_copy_acceleration_structure_khr(struct vk_cmd_queue *queue,
                                            struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
         vk_free(queue->alloc, ( VkCopyAccelerationStructureInfoKHR* )cmd->u.copy_acceleration_structure_khr.info);

   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_copy_acceleration_structure_khr(struct vk_cmd_queue *queue
, const VkCopyAccelerationStructureInfoKHR* pInfo
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_COPY_ACCELERATION_STRUCTURE_KHR;
      
   if (pInfo) {
      cmd->u.copy_acceleration_structure_khr.info = vk_zalloc(queue->alloc, sizeof(VkCopyAccelerationStructureInfoKHR), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
      if (cmd->u.copy_acceleration_structure_khr.info == NULL) goto err;

      memcpy((void*)cmd->u.copy_acceleration_structure_khr.info, pInfo, sizeof(VkCopyAccelerationStructureInfoKHR));
   VkCopyAccelerationStructureInfoKHR *tmp_dst1 = (void *) cmd->u.copy_acceleration_structure_khr.info; (void) tmp_dst1;
   VkCopyAccelerationStructureInfoKHR *tmp_src1 = (void *) pInfo; (void) tmp_src1;   
      } else {
      cmd->u.copy_acceleration_structure_khr.info = NULL;
   }   

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

err:
   if (cmd)
      vk_free_cmd_copy_acceleration_structure_khr(queue, cmd);
   return VK_ERROR_OUT_OF_HOST_MEMORY;
}

static void
vk_free_cmd_copy_acceleration_structure_to_memory_khr(struct vk_cmd_queue *queue,
                                                      struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
         vk_free(queue->alloc, ( VkCopyAccelerationStructureToMemoryInfoKHR* )cmd->u.copy_acceleration_structure_to_memory_khr.info);

   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_copy_acceleration_structure_to_memory_khr(struct vk_cmd_queue *queue
, const VkCopyAccelerationStructureToMemoryInfoKHR* pInfo
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_COPY_ACCELERATION_STRUCTURE_TO_MEMORY_KHR;
      
   if (pInfo) {
      cmd->u.copy_acceleration_structure_to_memory_khr.info = vk_zalloc(queue->alloc, sizeof(VkCopyAccelerationStructureToMemoryInfoKHR), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
      if (cmd->u.copy_acceleration_structure_to_memory_khr.info == NULL) goto err;

      memcpy((void*)cmd->u.copy_acceleration_structure_to_memory_khr.info, pInfo, sizeof(VkCopyAccelerationStructureToMemoryInfoKHR));
   VkCopyAccelerationStructureToMemoryInfoKHR *tmp_dst1 = (void *) cmd->u.copy_acceleration_structure_to_memory_khr.info; (void) tmp_dst1;
   VkCopyAccelerationStructureToMemoryInfoKHR *tmp_src1 = (void *) pInfo; (void) tmp_src1;   
      } else {
      cmd->u.copy_acceleration_structure_to_memory_khr.info = NULL;
   }   

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

err:
   if (cmd)
      vk_free_cmd_copy_acceleration_structure_to_memory_khr(queue, cmd);
   return VK_ERROR_OUT_OF_HOST_MEMORY;
}

static void
vk_free_cmd_copy_memory_to_acceleration_structure_khr(struct vk_cmd_queue *queue,
                                                      struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
         vk_free(queue->alloc, ( VkCopyMemoryToAccelerationStructureInfoKHR* )cmd->u.copy_memory_to_acceleration_structure_khr.info);

   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_copy_memory_to_acceleration_structure_khr(struct vk_cmd_queue *queue
, const VkCopyMemoryToAccelerationStructureInfoKHR* pInfo
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_COPY_MEMORY_TO_ACCELERATION_STRUCTURE_KHR;
      
   if (pInfo) {
      cmd->u.copy_memory_to_acceleration_structure_khr.info = vk_zalloc(queue->alloc, sizeof(VkCopyMemoryToAccelerationStructureInfoKHR), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
      if (cmd->u.copy_memory_to_acceleration_structure_khr.info == NULL) goto err;

      memcpy((void*)cmd->u.copy_memory_to_acceleration_structure_khr.info, pInfo, sizeof(VkCopyMemoryToAccelerationStructureInfoKHR));
   VkCopyMemoryToAccelerationStructureInfoKHR *tmp_dst1 = (void *) cmd->u.copy_memory_to_acceleration_structure_khr.info; (void) tmp_dst1;
   VkCopyMemoryToAccelerationStructureInfoKHR *tmp_src1 = (void *) pInfo; (void) tmp_src1;   
      } else {
      cmd->u.copy_memory_to_acceleration_structure_khr.info = NULL;
   }   

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

err:
   if (cmd)
      vk_free_cmd_copy_memory_to_acceleration_structure_khr(queue, cmd);
   return VK_ERROR_OUT_OF_HOST_MEMORY;
}

static void
vk_free_cmd_write_acceleration_structures_properties_khr(struct vk_cmd_queue *queue,
                                                         struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, ( VkAccelerationStructureKHR* )cmd->u.write_acceleration_structures_properties_khr.acceleration_structures);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_write_acceleration_structures_properties_khr(struct vk_cmd_queue *queue
, uint32_t accelerationStructureCount
, const VkAccelerationStructureKHR* pAccelerationStructures
, VkQueryType queryType
, VkQueryPool queryPool
, uint32_t firstQuery
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_WRITE_ACCELERATION_STRUCTURES_PROPERTIES_KHR;
      
   cmd->u.write_acceleration_structures_properties_khr.acceleration_structure_count = accelerationStructureCount;
   if (pAccelerationStructures) {
      cmd->u.write_acceleration_structures_properties_khr.acceleration_structures = vk_zalloc(queue->alloc, sizeof(*cmd->u.write_acceleration_structures_properties_khr.acceleration_structures) * (accelerationStructureCount), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (cmd->u.write_acceleration_structures_properties_khr.acceleration_structures == NULL) goto err;

   memcpy(( VkAccelerationStructureKHR* )cmd->u.write_acceleration_structures_properties_khr.acceleration_structures, pAccelerationStructures, sizeof(*cmd->u.write_acceleration_structures_properties_khr.acceleration_structures) * (accelerationStructureCount));
   }   
   cmd->u.write_acceleration_structures_properties_khr.query_type = queryType;
   cmd->u.write_acceleration_structures_properties_khr.query_pool = queryPool;
   cmd->u.write_acceleration_structures_properties_khr.first_query = firstQuery;

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

err:
   if (cmd)
      vk_free_cmd_write_acceleration_structures_properties_khr(queue, cmd);
   return VK_ERROR_OUT_OF_HOST_MEMORY;
}

static void
vk_free_cmd_write_acceleration_structures_properties_nv(struct vk_cmd_queue *queue,
                                                        struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, ( VkAccelerationStructureNV* )cmd->u.write_acceleration_structures_properties_nv.acceleration_structures);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_write_acceleration_structures_properties_nv(struct vk_cmd_queue *queue
, uint32_t accelerationStructureCount
, const VkAccelerationStructureNV* pAccelerationStructures
, VkQueryType queryType
, VkQueryPool queryPool
, uint32_t firstQuery
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_WRITE_ACCELERATION_STRUCTURES_PROPERTIES_NV;
      
   cmd->u.write_acceleration_structures_properties_nv.acceleration_structure_count = accelerationStructureCount;
   if (pAccelerationStructures) {
      cmd->u.write_acceleration_structures_properties_nv.acceleration_structures = vk_zalloc(queue->alloc, sizeof(*cmd->u.write_acceleration_structures_properties_nv.acceleration_structures) * (accelerationStructureCount), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (cmd->u.write_acceleration_structures_properties_nv.acceleration_structures == NULL) goto err;

   memcpy(( VkAccelerationStructureNV* )cmd->u.write_acceleration_structures_properties_nv.acceleration_structures, pAccelerationStructures, sizeof(*cmd->u.write_acceleration_structures_properties_nv.acceleration_structures) * (accelerationStructureCount));
   }   
   cmd->u.write_acceleration_structures_properties_nv.query_type = queryType;
   cmd->u.write_acceleration_structures_properties_nv.query_pool = queryPool;
   cmd->u.write_acceleration_structures_properties_nv.first_query = firstQuery;

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

err:
   if (cmd)
      vk_free_cmd_write_acceleration_structures_properties_nv(queue, cmd);
   return VK_ERROR_OUT_OF_HOST_MEMORY;
}

static void
vk_free_cmd_build_acceleration_structure_nv(struct vk_cmd_queue *queue,
                                            struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, ( VkGeometryNV* )cmd->u.build_acceleration_structure_nv.info->pGeometries);
      vk_free(queue->alloc, ( VkAccelerationStructureInfoNV* )cmd->u.build_acceleration_structure_nv.info);

   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_build_acceleration_structure_nv(struct vk_cmd_queue *queue
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
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_BUILD_ACCELERATION_STRUCTURE_NV;
      
   if (pInfo) {
      cmd->u.build_acceleration_structure_nv.info = vk_zalloc(queue->alloc, sizeof(VkAccelerationStructureInfoNV), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
      if (cmd->u.build_acceleration_structure_nv.info == NULL) goto err;

      memcpy((void*)cmd->u.build_acceleration_structure_nv.info, pInfo, sizeof(VkAccelerationStructureInfoNV));
   VkAccelerationStructureInfoNV *tmp_dst1 = (void *) cmd->u.build_acceleration_structure_nv.info; (void) tmp_dst1;
   VkAccelerationStructureInfoNV *tmp_src1 = (void *) pInfo; (void) tmp_src1;   
   if (tmp_src1->pGeometries) {
   tmp_dst1->pGeometries = vk_zalloc(queue->alloc, sizeof(*tmp_dst1->pGeometries) * tmp_dst1->geometryCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (tmp_dst1->pGeometries == NULL) goto err;

   memcpy(( VkGeometryNV* )tmp_dst1->pGeometries, tmp_src1->pGeometries, sizeof(*tmp_dst1->pGeometries) * tmp_dst1->geometryCount);
}
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

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

err:
   if (cmd)
      vk_free_cmd_build_acceleration_structure_nv(queue, cmd);
   return VK_ERROR_OUT_OF_HOST_MEMORY;
}

static void
vk_free_cmd_trace_rays_khr(struct vk_cmd_queue *queue,
                           struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
         vk_free(queue->alloc, ( VkStridedDeviceAddressRegionKHR* )cmd->u.trace_rays_khr.raygen_shader_binding_table);

         vk_free(queue->alloc, ( VkStridedDeviceAddressRegionKHR* )cmd->u.trace_rays_khr.miss_shader_binding_table);

         vk_free(queue->alloc, ( VkStridedDeviceAddressRegionKHR* )cmd->u.trace_rays_khr.hit_shader_binding_table);

         vk_free(queue->alloc, ( VkStridedDeviceAddressRegionKHR* )cmd->u.trace_rays_khr.callable_shader_binding_table);

   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_trace_rays_khr(struct vk_cmd_queue *queue
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
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_TRACE_RAYS_KHR;
      
   if (pRaygenShaderBindingTable) {
      cmd->u.trace_rays_khr.raygen_shader_binding_table = vk_zalloc(queue->alloc, sizeof(VkStridedDeviceAddressRegionKHR), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
      if (cmd->u.trace_rays_khr.raygen_shader_binding_table == NULL) goto err;

      memcpy((void*)cmd->u.trace_rays_khr.raygen_shader_binding_table, pRaygenShaderBindingTable, sizeof(VkStridedDeviceAddressRegionKHR));
   VkStridedDeviceAddressRegionKHR *tmp_dst1 = (void *) cmd->u.trace_rays_khr.raygen_shader_binding_table; (void) tmp_dst1;
   VkStridedDeviceAddressRegionKHR *tmp_src1 = (void *) pRaygenShaderBindingTable; (void) tmp_src1;   
      } else {
      cmd->u.trace_rays_khr.raygen_shader_binding_table = NULL;
   }   
   if (pMissShaderBindingTable) {
      cmd->u.trace_rays_khr.miss_shader_binding_table = vk_zalloc(queue->alloc, sizeof(VkStridedDeviceAddressRegionKHR), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
      if (cmd->u.trace_rays_khr.miss_shader_binding_table == NULL) goto err;

      memcpy((void*)cmd->u.trace_rays_khr.miss_shader_binding_table, pMissShaderBindingTable, sizeof(VkStridedDeviceAddressRegionKHR));
   VkStridedDeviceAddressRegionKHR *tmp_dst1 = (void *) cmd->u.trace_rays_khr.miss_shader_binding_table; (void) tmp_dst1;
   VkStridedDeviceAddressRegionKHR *tmp_src1 = (void *) pMissShaderBindingTable; (void) tmp_src1;   
      } else {
      cmd->u.trace_rays_khr.miss_shader_binding_table = NULL;
   }   
   if (pHitShaderBindingTable) {
      cmd->u.trace_rays_khr.hit_shader_binding_table = vk_zalloc(queue->alloc, sizeof(VkStridedDeviceAddressRegionKHR), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
      if (cmd->u.trace_rays_khr.hit_shader_binding_table == NULL) goto err;

      memcpy((void*)cmd->u.trace_rays_khr.hit_shader_binding_table, pHitShaderBindingTable, sizeof(VkStridedDeviceAddressRegionKHR));
   VkStridedDeviceAddressRegionKHR *tmp_dst1 = (void *) cmd->u.trace_rays_khr.hit_shader_binding_table; (void) tmp_dst1;
   VkStridedDeviceAddressRegionKHR *tmp_src1 = (void *) pHitShaderBindingTable; (void) tmp_src1;   
      } else {
      cmd->u.trace_rays_khr.hit_shader_binding_table = NULL;
   }   
   if (pCallableShaderBindingTable) {
      cmd->u.trace_rays_khr.callable_shader_binding_table = vk_zalloc(queue->alloc, sizeof(VkStridedDeviceAddressRegionKHR), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
      if (cmd->u.trace_rays_khr.callable_shader_binding_table == NULL) goto err;

      memcpy((void*)cmd->u.trace_rays_khr.callable_shader_binding_table, pCallableShaderBindingTable, sizeof(VkStridedDeviceAddressRegionKHR));
   VkStridedDeviceAddressRegionKHR *tmp_dst1 = (void *) cmd->u.trace_rays_khr.callable_shader_binding_table; (void) tmp_dst1;
   VkStridedDeviceAddressRegionKHR *tmp_src1 = (void *) pCallableShaderBindingTable; (void) tmp_src1;   
      } else {
      cmd->u.trace_rays_khr.callable_shader_binding_table = NULL;
   }   
   cmd->u.trace_rays_khr.width = width;
   cmd->u.trace_rays_khr.height = height;
   cmd->u.trace_rays_khr.depth = depth;

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

err:
   if (cmd)
      vk_free_cmd_trace_rays_khr(queue, cmd);
   return VK_ERROR_OUT_OF_HOST_MEMORY;
}

static void
vk_free_cmd_trace_rays_nv(struct vk_cmd_queue *queue,
                          struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, cmd);
}

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
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_TRACE_RAYS_NV;
      
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

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

}

static void
vk_free_cmd_trace_rays_indirect_khr(struct vk_cmd_queue *queue,
                                    struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
         vk_free(queue->alloc, ( VkStridedDeviceAddressRegionKHR* )cmd->u.trace_rays_indirect_khr.raygen_shader_binding_table);

         vk_free(queue->alloc, ( VkStridedDeviceAddressRegionKHR* )cmd->u.trace_rays_indirect_khr.miss_shader_binding_table);

         vk_free(queue->alloc, ( VkStridedDeviceAddressRegionKHR* )cmd->u.trace_rays_indirect_khr.hit_shader_binding_table);

         vk_free(queue->alloc, ( VkStridedDeviceAddressRegionKHR* )cmd->u.trace_rays_indirect_khr.callable_shader_binding_table);

   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_trace_rays_indirect_khr(struct vk_cmd_queue *queue
, const VkStridedDeviceAddressRegionKHR* pRaygenShaderBindingTable
, const VkStridedDeviceAddressRegionKHR* pMissShaderBindingTable
, const VkStridedDeviceAddressRegionKHR* pHitShaderBindingTable
, const VkStridedDeviceAddressRegionKHR* pCallableShaderBindingTable
, VkDeviceAddress indirectDeviceAddress
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_TRACE_RAYS_INDIRECT_KHR;
      
   if (pRaygenShaderBindingTable) {
      cmd->u.trace_rays_indirect_khr.raygen_shader_binding_table = vk_zalloc(queue->alloc, sizeof(VkStridedDeviceAddressRegionKHR), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
      if (cmd->u.trace_rays_indirect_khr.raygen_shader_binding_table == NULL) goto err;

      memcpy((void*)cmd->u.trace_rays_indirect_khr.raygen_shader_binding_table, pRaygenShaderBindingTable, sizeof(VkStridedDeviceAddressRegionKHR));
   VkStridedDeviceAddressRegionKHR *tmp_dst1 = (void *) cmd->u.trace_rays_indirect_khr.raygen_shader_binding_table; (void) tmp_dst1;
   VkStridedDeviceAddressRegionKHR *tmp_src1 = (void *) pRaygenShaderBindingTable; (void) tmp_src1;   
      } else {
      cmd->u.trace_rays_indirect_khr.raygen_shader_binding_table = NULL;
   }   
   if (pMissShaderBindingTable) {
      cmd->u.trace_rays_indirect_khr.miss_shader_binding_table = vk_zalloc(queue->alloc, sizeof(VkStridedDeviceAddressRegionKHR), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
      if (cmd->u.trace_rays_indirect_khr.miss_shader_binding_table == NULL) goto err;

      memcpy((void*)cmd->u.trace_rays_indirect_khr.miss_shader_binding_table, pMissShaderBindingTable, sizeof(VkStridedDeviceAddressRegionKHR));
   VkStridedDeviceAddressRegionKHR *tmp_dst1 = (void *) cmd->u.trace_rays_indirect_khr.miss_shader_binding_table; (void) tmp_dst1;
   VkStridedDeviceAddressRegionKHR *tmp_src1 = (void *) pMissShaderBindingTable; (void) tmp_src1;   
      } else {
      cmd->u.trace_rays_indirect_khr.miss_shader_binding_table = NULL;
   }   
   if (pHitShaderBindingTable) {
      cmd->u.trace_rays_indirect_khr.hit_shader_binding_table = vk_zalloc(queue->alloc, sizeof(VkStridedDeviceAddressRegionKHR), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
      if (cmd->u.trace_rays_indirect_khr.hit_shader_binding_table == NULL) goto err;

      memcpy((void*)cmd->u.trace_rays_indirect_khr.hit_shader_binding_table, pHitShaderBindingTable, sizeof(VkStridedDeviceAddressRegionKHR));
   VkStridedDeviceAddressRegionKHR *tmp_dst1 = (void *) cmd->u.trace_rays_indirect_khr.hit_shader_binding_table; (void) tmp_dst1;
   VkStridedDeviceAddressRegionKHR *tmp_src1 = (void *) pHitShaderBindingTable; (void) tmp_src1;   
      } else {
      cmd->u.trace_rays_indirect_khr.hit_shader_binding_table = NULL;
   }   
   if (pCallableShaderBindingTable) {
      cmd->u.trace_rays_indirect_khr.callable_shader_binding_table = vk_zalloc(queue->alloc, sizeof(VkStridedDeviceAddressRegionKHR), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
      if (cmd->u.trace_rays_indirect_khr.callable_shader_binding_table == NULL) goto err;

      memcpy((void*)cmd->u.trace_rays_indirect_khr.callable_shader_binding_table, pCallableShaderBindingTable, sizeof(VkStridedDeviceAddressRegionKHR));
   VkStridedDeviceAddressRegionKHR *tmp_dst1 = (void *) cmd->u.trace_rays_indirect_khr.callable_shader_binding_table; (void) tmp_dst1;
   VkStridedDeviceAddressRegionKHR *tmp_src1 = (void *) pCallableShaderBindingTable; (void) tmp_src1;   
      } else {
      cmd->u.trace_rays_indirect_khr.callable_shader_binding_table = NULL;
   }   
   cmd->u.trace_rays_indirect_khr.indirect_device_address = indirectDeviceAddress;

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

err:
   if (cmd)
      vk_free_cmd_trace_rays_indirect_khr(queue, cmd);
   return VK_ERROR_OUT_OF_HOST_MEMORY;
}

static void
vk_free_cmd_trace_rays_indirect2_khr(struct vk_cmd_queue *queue,
                                     struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_trace_rays_indirect2_khr(struct vk_cmd_queue *queue
, VkDeviceAddress indirectDeviceAddress
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_TRACE_RAYS_INDIRECT2_KHR;
      
   cmd->u.trace_rays_indirect2_khr.indirect_device_address = indirectDeviceAddress;

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

}

static void
vk_free_cmd_set_ray_tracing_pipeline_stack_size_khr(struct vk_cmd_queue *queue,
                                                    struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_set_ray_tracing_pipeline_stack_size_khr(struct vk_cmd_queue *queue
, uint32_t pipelineStackSize
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_SET_RAY_TRACING_PIPELINE_STACK_SIZE_KHR;
      
   cmd->u.set_ray_tracing_pipeline_stack_size_khr.pipeline_stack_size = pipelineStackSize;

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

}

static void
vk_free_cmd_set_performance_marker_intel(struct vk_cmd_queue *queue,
                                         struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
         vk_free(queue->alloc, ( VkPerformanceMarkerInfoINTEL* )cmd->u.set_performance_marker_intel.marker_info);

   vk_free(queue->alloc, cmd);
}


static void
vk_free_cmd_set_performance_stream_marker_intel(struct vk_cmd_queue *queue,
                                                struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
         vk_free(queue->alloc, ( VkPerformanceStreamMarkerInfoINTEL* )cmd->u.set_performance_stream_marker_intel.marker_info);

   vk_free(queue->alloc, cmd);
}


static void
vk_free_cmd_set_performance_override_intel(struct vk_cmd_queue *queue,
                                           struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
         vk_free(queue->alloc, ( VkPerformanceOverrideInfoINTEL* )cmd->u.set_performance_override_intel.override_info);

   vk_free(queue->alloc, cmd);
}


static void
vk_free_cmd_set_line_stipple_ext(struct vk_cmd_queue *queue,
                                 struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_set_line_stipple_ext(struct vk_cmd_queue *queue
, uint32_t lineStippleFactor
, uint16_t lineStipplePattern
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_SET_LINE_STIPPLE_EXT;
      
   cmd->u.set_line_stipple_ext.line_stipple_factor = lineStippleFactor;
   cmd->u.set_line_stipple_ext.line_stipple_pattern = lineStipplePattern;

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

}

static void
vk_free_cmd_build_acceleration_structures_khr(struct vk_cmd_queue *queue,
                                              struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, ( VkAccelerationStructureBuildGeometryInfoKHR* )cmd->u.build_acceleration_structures_khr.infos);
   vk_free(queue->alloc, ( VkAccelerationStructureBuildRangeInfoKHR* * )cmd->u.build_acceleration_structures_khr.pp_build_range_infos);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_build_acceleration_structures_khr(struct vk_cmd_queue *queue
, uint32_t infoCount
, const VkAccelerationStructureBuildGeometryInfoKHR* pInfos
, const VkAccelerationStructureBuildRangeInfoKHR* const* ppBuildRangeInfos
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_BUILD_ACCELERATION_STRUCTURES_KHR;
      
   cmd->u.build_acceleration_structures_khr.info_count = infoCount;
   if (pInfos) {
      cmd->u.build_acceleration_structures_khr.infos = vk_zalloc(queue->alloc, sizeof(*cmd->u.build_acceleration_structures_khr.infos) * (infoCount), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (cmd->u.build_acceleration_structures_khr.infos == NULL) goto err;

   memcpy(( VkAccelerationStructureBuildGeometryInfoKHR* )cmd->u.build_acceleration_structures_khr.infos, pInfos, sizeof(*cmd->u.build_acceleration_structures_khr.infos) * (infoCount));
   }   
   if (ppBuildRangeInfos) {
      cmd->u.build_acceleration_structures_khr.pp_build_range_infos = vk_zalloc(queue->alloc, sizeof(*cmd->u.build_acceleration_structures_khr.pp_build_range_infos) * (infoCount), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (cmd->u.build_acceleration_structures_khr.pp_build_range_infos == NULL) goto err;

   memcpy(( VkAccelerationStructureBuildRangeInfoKHR* * )cmd->u.build_acceleration_structures_khr.pp_build_range_infos, ppBuildRangeInfos, sizeof(*cmd->u.build_acceleration_structures_khr.pp_build_range_infos) * (infoCount));
   }   

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

err:
   if (cmd)
      vk_free_cmd_build_acceleration_structures_khr(queue, cmd);
   return VK_ERROR_OUT_OF_HOST_MEMORY;
}

static void
vk_free_cmd_build_acceleration_structures_indirect_khr(struct vk_cmd_queue *queue,
                                                       struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, ( VkAccelerationStructureBuildGeometryInfoKHR* )cmd->u.build_acceleration_structures_indirect_khr.infos);
   vk_free(queue->alloc, ( VkDeviceAddress*             )cmd->u.build_acceleration_structures_indirect_khr.indirect_device_addresses);
   vk_free(queue->alloc, ( uint32_t*                    )cmd->u.build_acceleration_structures_indirect_khr.indirect_strides);
   vk_free(queue->alloc, ( uint32_t* *             )cmd->u.build_acceleration_structures_indirect_khr.pp_max_primitive_counts);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_build_acceleration_structures_indirect_khr(struct vk_cmd_queue *queue
, uint32_t                                           infoCount
, const VkAccelerationStructureBuildGeometryInfoKHR* pInfos
, const VkDeviceAddress*             pIndirectDeviceAddresses
, const uint32_t*                    pIndirectStrides
, const uint32_t* const*             ppMaxPrimitiveCounts
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_BUILD_ACCELERATION_STRUCTURES_INDIRECT_KHR;
      
   cmd->u.build_acceleration_structures_indirect_khr.info_count = infoCount;
   if (pInfos) {
      cmd->u.build_acceleration_structures_indirect_khr.infos = vk_zalloc(queue->alloc, sizeof(*cmd->u.build_acceleration_structures_indirect_khr.infos) * (infoCount), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (cmd->u.build_acceleration_structures_indirect_khr.infos == NULL) goto err;

   memcpy(( VkAccelerationStructureBuildGeometryInfoKHR* )cmd->u.build_acceleration_structures_indirect_khr.infos, pInfos, sizeof(*cmd->u.build_acceleration_structures_indirect_khr.infos) * (infoCount));
   }   
   if (pIndirectDeviceAddresses) {
      cmd->u.build_acceleration_structures_indirect_khr.indirect_device_addresses = vk_zalloc(queue->alloc, sizeof(*cmd->u.build_acceleration_structures_indirect_khr.indirect_device_addresses) * (infoCount), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (cmd->u.build_acceleration_structures_indirect_khr.indirect_device_addresses == NULL) goto err;

   memcpy(( VkDeviceAddress*             )cmd->u.build_acceleration_structures_indirect_khr.indirect_device_addresses, pIndirectDeviceAddresses, sizeof(*cmd->u.build_acceleration_structures_indirect_khr.indirect_device_addresses) * (infoCount));
   }   
   if (pIndirectStrides) {
      cmd->u.build_acceleration_structures_indirect_khr.indirect_strides = vk_zalloc(queue->alloc, sizeof(*cmd->u.build_acceleration_structures_indirect_khr.indirect_strides) * (infoCount), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (cmd->u.build_acceleration_structures_indirect_khr.indirect_strides == NULL) goto err;

   memcpy(( uint32_t*                    )cmd->u.build_acceleration_structures_indirect_khr.indirect_strides, pIndirectStrides, sizeof(*cmd->u.build_acceleration_structures_indirect_khr.indirect_strides) * (infoCount));
   }   
   if (ppMaxPrimitiveCounts) {
      cmd->u.build_acceleration_structures_indirect_khr.pp_max_primitive_counts = vk_zalloc(queue->alloc, sizeof(*cmd->u.build_acceleration_structures_indirect_khr.pp_max_primitive_counts) * (infoCount), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (cmd->u.build_acceleration_structures_indirect_khr.pp_max_primitive_counts == NULL) goto err;

   memcpy(( uint32_t* *             )cmd->u.build_acceleration_structures_indirect_khr.pp_max_primitive_counts, ppMaxPrimitiveCounts, sizeof(*cmd->u.build_acceleration_structures_indirect_khr.pp_max_primitive_counts) * (infoCount));
   }   

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

err:
   if (cmd)
      vk_free_cmd_build_acceleration_structures_indirect_khr(queue, cmd);
   return VK_ERROR_OUT_OF_HOST_MEMORY;
}

static void
vk_free_cmd_set_cull_mode(struct vk_cmd_queue *queue,
                          struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_set_cull_mode(struct vk_cmd_queue *queue
, VkCullModeFlags cullMode
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_SET_CULL_MODE;
      
   cmd->u.set_cull_mode.cull_mode = cullMode;

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

}

static void
vk_free_cmd_set_front_face(struct vk_cmd_queue *queue,
                           struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_set_front_face(struct vk_cmd_queue *queue
, VkFrontFace frontFace
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_SET_FRONT_FACE;
      
   cmd->u.set_front_face.front_face = frontFace;

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

}

static void
vk_free_cmd_set_primitive_topology(struct vk_cmd_queue *queue,
                                   struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_set_primitive_topology(struct vk_cmd_queue *queue
, VkPrimitiveTopology primitiveTopology
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_SET_PRIMITIVE_TOPOLOGY;
      
   cmd->u.set_primitive_topology.primitive_topology = primitiveTopology;

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

}

static void
vk_free_cmd_set_viewport_with_count(struct vk_cmd_queue *queue,
                                    struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, ( VkViewport* )cmd->u.set_viewport_with_count.viewports);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_set_viewport_with_count(struct vk_cmd_queue *queue
, uint32_t viewportCount
, const VkViewport* pViewports
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_SET_VIEWPORT_WITH_COUNT;
      
   cmd->u.set_viewport_with_count.viewport_count = viewportCount;
   if (pViewports) {
      cmd->u.set_viewport_with_count.viewports = vk_zalloc(queue->alloc, sizeof(*cmd->u.set_viewport_with_count.viewports) * (viewportCount), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (cmd->u.set_viewport_with_count.viewports == NULL) goto err;

   memcpy(( VkViewport* )cmd->u.set_viewport_with_count.viewports, pViewports, sizeof(*cmd->u.set_viewport_with_count.viewports) * (viewportCount));
   }   

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

err:
   if (cmd)
      vk_free_cmd_set_viewport_with_count(queue, cmd);
   return VK_ERROR_OUT_OF_HOST_MEMORY;
}

static void
vk_free_cmd_set_scissor_with_count(struct vk_cmd_queue *queue,
                                   struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, ( VkRect2D* )cmd->u.set_scissor_with_count.scissors);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_set_scissor_with_count(struct vk_cmd_queue *queue
, uint32_t scissorCount
, const VkRect2D* pScissors
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_SET_SCISSOR_WITH_COUNT;
      
   cmd->u.set_scissor_with_count.scissor_count = scissorCount;
   if (pScissors) {
      cmd->u.set_scissor_with_count.scissors = vk_zalloc(queue->alloc, sizeof(*cmd->u.set_scissor_with_count.scissors) * (scissorCount), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (cmd->u.set_scissor_with_count.scissors == NULL) goto err;

   memcpy(( VkRect2D* )cmd->u.set_scissor_with_count.scissors, pScissors, sizeof(*cmd->u.set_scissor_with_count.scissors) * (scissorCount));
   }   

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

err:
   if (cmd)
      vk_free_cmd_set_scissor_with_count(queue, cmd);
   return VK_ERROR_OUT_OF_HOST_MEMORY;
}

static void
vk_free_cmd_bind_vertex_buffers2(struct vk_cmd_queue *queue,
                                 struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, ( VkBuffer* )cmd->u.bind_vertex_buffers2.buffers);
   vk_free(queue->alloc, ( VkDeviceSize* )cmd->u.bind_vertex_buffers2.offsets);
   vk_free(queue->alloc, ( VkDeviceSize* )cmd->u.bind_vertex_buffers2.sizes);
   vk_free(queue->alloc, ( VkDeviceSize* )cmd->u.bind_vertex_buffers2.strides);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_bind_vertex_buffers2(struct vk_cmd_queue *queue
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
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_BIND_VERTEX_BUFFERS2;
      
   cmd->u.bind_vertex_buffers2.first_binding = firstBinding;
   cmd->u.bind_vertex_buffers2.binding_count = bindingCount;
   if (pBuffers) {
      cmd->u.bind_vertex_buffers2.buffers = vk_zalloc(queue->alloc, sizeof(*cmd->u.bind_vertex_buffers2.buffers) * (bindingCount), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (cmd->u.bind_vertex_buffers2.buffers == NULL) goto err;

   memcpy(( VkBuffer* )cmd->u.bind_vertex_buffers2.buffers, pBuffers, sizeof(*cmd->u.bind_vertex_buffers2.buffers) * (bindingCount));
   }   
   if (pOffsets) {
      cmd->u.bind_vertex_buffers2.offsets = vk_zalloc(queue->alloc, sizeof(*cmd->u.bind_vertex_buffers2.offsets) * (bindingCount), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (cmd->u.bind_vertex_buffers2.offsets == NULL) goto err;

   memcpy(( VkDeviceSize* )cmd->u.bind_vertex_buffers2.offsets, pOffsets, sizeof(*cmd->u.bind_vertex_buffers2.offsets) * (bindingCount));
   }   
   if (pSizes) {
      cmd->u.bind_vertex_buffers2.sizes = vk_zalloc(queue->alloc, sizeof(*cmd->u.bind_vertex_buffers2.sizes) * (bindingCount), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (cmd->u.bind_vertex_buffers2.sizes == NULL) goto err;

   memcpy(( VkDeviceSize* )cmd->u.bind_vertex_buffers2.sizes, pSizes, sizeof(*cmd->u.bind_vertex_buffers2.sizes) * (bindingCount));
   }   
   if (pStrides) {
      cmd->u.bind_vertex_buffers2.strides = vk_zalloc(queue->alloc, sizeof(*cmd->u.bind_vertex_buffers2.strides) * (bindingCount), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (cmd->u.bind_vertex_buffers2.strides == NULL) goto err;

   memcpy(( VkDeviceSize* )cmd->u.bind_vertex_buffers2.strides, pStrides, sizeof(*cmd->u.bind_vertex_buffers2.strides) * (bindingCount));
   }   

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

err:
   if (cmd)
      vk_free_cmd_bind_vertex_buffers2(queue, cmd);
   return VK_ERROR_OUT_OF_HOST_MEMORY;
}

static void
vk_free_cmd_set_depth_test_enable(struct vk_cmd_queue *queue,
                                  struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_set_depth_test_enable(struct vk_cmd_queue *queue
, VkBool32 depthTestEnable
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_SET_DEPTH_TEST_ENABLE;
      
   cmd->u.set_depth_test_enable.depth_test_enable = depthTestEnable;

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

}

static void
vk_free_cmd_set_depth_write_enable(struct vk_cmd_queue *queue,
                                   struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_set_depth_write_enable(struct vk_cmd_queue *queue
, VkBool32 depthWriteEnable
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_SET_DEPTH_WRITE_ENABLE;
      
   cmd->u.set_depth_write_enable.depth_write_enable = depthWriteEnable;

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

}

static void
vk_free_cmd_set_depth_compare_op(struct vk_cmd_queue *queue,
                                 struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_set_depth_compare_op(struct vk_cmd_queue *queue
, VkCompareOp depthCompareOp
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_SET_DEPTH_COMPARE_OP;
      
   cmd->u.set_depth_compare_op.depth_compare_op = depthCompareOp;

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

}

static void
vk_free_cmd_set_depth_bounds_test_enable(struct vk_cmd_queue *queue,
                                         struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_set_depth_bounds_test_enable(struct vk_cmd_queue *queue
, VkBool32 depthBoundsTestEnable
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_SET_DEPTH_BOUNDS_TEST_ENABLE;
      
   cmd->u.set_depth_bounds_test_enable.depth_bounds_test_enable = depthBoundsTestEnable;

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

}

static void
vk_free_cmd_set_stencil_test_enable(struct vk_cmd_queue *queue,
                                    struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_set_stencil_test_enable(struct vk_cmd_queue *queue
, VkBool32 stencilTestEnable
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_SET_STENCIL_TEST_ENABLE;
      
   cmd->u.set_stencil_test_enable.stencil_test_enable = stencilTestEnable;

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

}

static void
vk_free_cmd_set_stencil_op(struct vk_cmd_queue *queue,
                           struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_set_stencil_op(struct vk_cmd_queue *queue
, VkStencilFaceFlags faceMask
, VkStencilOp failOp
, VkStencilOp passOp
, VkStencilOp depthFailOp
, VkCompareOp compareOp
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_SET_STENCIL_OP;
      
   cmd->u.set_stencil_op.face_mask = faceMask;
   cmd->u.set_stencil_op.fail_op = failOp;
   cmd->u.set_stencil_op.pass_op = passOp;
   cmd->u.set_stencil_op.depth_fail_op = depthFailOp;
   cmd->u.set_stencil_op.compare_op = compareOp;

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

}

static void
vk_free_cmd_set_patch_control_points_ext(struct vk_cmd_queue *queue,
                                         struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_set_patch_control_points_ext(struct vk_cmd_queue *queue
, uint32_t patchControlPoints
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_SET_PATCH_CONTROL_POINTS_EXT;
      
   cmd->u.set_patch_control_points_ext.patch_control_points = patchControlPoints;

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

}

static void
vk_free_cmd_set_rasterizer_discard_enable(struct vk_cmd_queue *queue,
                                          struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_set_rasterizer_discard_enable(struct vk_cmd_queue *queue
, VkBool32 rasterizerDiscardEnable
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_SET_RASTERIZER_DISCARD_ENABLE;
      
   cmd->u.set_rasterizer_discard_enable.rasterizer_discard_enable = rasterizerDiscardEnable;

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

}

static void
vk_free_cmd_set_depth_bias_enable(struct vk_cmd_queue *queue,
                                  struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_set_depth_bias_enable(struct vk_cmd_queue *queue
, VkBool32 depthBiasEnable
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_SET_DEPTH_BIAS_ENABLE;
      
   cmd->u.set_depth_bias_enable.depth_bias_enable = depthBiasEnable;

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

}

static void
vk_free_cmd_set_logic_op_ext(struct vk_cmd_queue *queue,
                             struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_set_logic_op_ext(struct vk_cmd_queue *queue
, VkLogicOp logicOp
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_SET_LOGIC_OP_EXT;
      
   cmd->u.set_logic_op_ext.logic_op = logicOp;

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

}

static void
vk_free_cmd_set_primitive_restart_enable(struct vk_cmd_queue *queue,
                                         struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_set_primitive_restart_enable(struct vk_cmd_queue *queue
, VkBool32 primitiveRestartEnable
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_SET_PRIMITIVE_RESTART_ENABLE;
      
   cmd->u.set_primitive_restart_enable.primitive_restart_enable = primitiveRestartEnable;

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

}

static void
vk_free_cmd_set_tessellation_domain_origin_ext(struct vk_cmd_queue *queue,
                                               struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_set_tessellation_domain_origin_ext(struct vk_cmd_queue *queue
, VkTessellationDomainOrigin domainOrigin
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_SET_TESSELLATION_DOMAIN_ORIGIN_EXT;
      
   cmd->u.set_tessellation_domain_origin_ext.domain_origin = domainOrigin;

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

}

static void
vk_free_cmd_set_depth_clamp_enable_ext(struct vk_cmd_queue *queue,
                                       struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_set_depth_clamp_enable_ext(struct vk_cmd_queue *queue
, VkBool32 depthClampEnable
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_SET_DEPTH_CLAMP_ENABLE_EXT;
      
   cmd->u.set_depth_clamp_enable_ext.depth_clamp_enable = depthClampEnable;

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

}

static void
vk_free_cmd_set_polygon_mode_ext(struct vk_cmd_queue *queue,
                                 struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_set_polygon_mode_ext(struct vk_cmd_queue *queue
, VkPolygonMode polygonMode
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_SET_POLYGON_MODE_EXT;
      
   cmd->u.set_polygon_mode_ext.polygon_mode = polygonMode;

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

}

static void
vk_free_cmd_set_rasterization_samples_ext(struct vk_cmd_queue *queue,
                                          struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_set_rasterization_samples_ext(struct vk_cmd_queue *queue
, VkSampleCountFlagBits  rasterizationSamples
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_SET_RASTERIZATION_SAMPLES_EXT;
      
   cmd->u.set_rasterization_samples_ext.rasterization_samples = rasterizationSamples;

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

}

static void
vk_free_cmd_set_sample_mask_ext(struct vk_cmd_queue *queue,
                                struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, ( VkSampleMask*    )cmd->u.set_sample_mask_ext.sample_mask);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_set_sample_mask_ext(struct vk_cmd_queue *queue
, VkSampleCountFlagBits  samples
, const VkSampleMask*    pSampleMask
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_SET_SAMPLE_MASK_EXT;
      
   cmd->u.set_sample_mask_ext.samples = samples;
   if (pSampleMask) {
      cmd->u.set_sample_mask_ext.sample_mask = vk_zalloc(queue->alloc, sizeof(*cmd->u.set_sample_mask_ext.sample_mask) * ((samples + 31) / 32), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (cmd->u.set_sample_mask_ext.sample_mask == NULL) goto err;

   memcpy(( VkSampleMask*    )cmd->u.set_sample_mask_ext.sample_mask, pSampleMask, sizeof(*cmd->u.set_sample_mask_ext.sample_mask) * ((samples + 31) / 32));
   }   

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

err:
   if (cmd)
      vk_free_cmd_set_sample_mask_ext(queue, cmd);
   return VK_ERROR_OUT_OF_HOST_MEMORY;
}

static void
vk_free_cmd_set_alpha_to_coverage_enable_ext(struct vk_cmd_queue *queue,
                                             struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_set_alpha_to_coverage_enable_ext(struct vk_cmd_queue *queue
, VkBool32 alphaToCoverageEnable
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_SET_ALPHA_TO_COVERAGE_ENABLE_EXT;
      
   cmd->u.set_alpha_to_coverage_enable_ext.alpha_to_coverage_enable = alphaToCoverageEnable;

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

}

static void
vk_free_cmd_set_alpha_to_one_enable_ext(struct vk_cmd_queue *queue,
                                        struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_set_alpha_to_one_enable_ext(struct vk_cmd_queue *queue
, VkBool32 alphaToOneEnable
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_SET_ALPHA_TO_ONE_ENABLE_EXT;
      
   cmd->u.set_alpha_to_one_enable_ext.alpha_to_one_enable = alphaToOneEnable;

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

}

static void
vk_free_cmd_set_logic_op_enable_ext(struct vk_cmd_queue *queue,
                                    struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_set_logic_op_enable_ext(struct vk_cmd_queue *queue
, VkBool32 logicOpEnable
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_SET_LOGIC_OP_ENABLE_EXT;
      
   cmd->u.set_logic_op_enable_ext.logic_op_enable = logicOpEnable;

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

}

static void
vk_free_cmd_set_color_blend_enable_ext(struct vk_cmd_queue *queue,
                                       struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, ( VkBool32* )cmd->u.set_color_blend_enable_ext.color_blend_enables);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_set_color_blend_enable_ext(struct vk_cmd_queue *queue
, uint32_t firstAttachment
, uint32_t attachmentCount
, const VkBool32* pColorBlendEnables
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_SET_COLOR_BLEND_ENABLE_EXT;
      
   cmd->u.set_color_blend_enable_ext.first_attachment = firstAttachment;
   cmd->u.set_color_blend_enable_ext.attachment_count = attachmentCount;
   if (pColorBlendEnables) {
      cmd->u.set_color_blend_enable_ext.color_blend_enables = vk_zalloc(queue->alloc, sizeof(*cmd->u.set_color_blend_enable_ext.color_blend_enables) * (attachmentCount), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (cmd->u.set_color_blend_enable_ext.color_blend_enables == NULL) goto err;

   memcpy(( VkBool32* )cmd->u.set_color_blend_enable_ext.color_blend_enables, pColorBlendEnables, sizeof(*cmd->u.set_color_blend_enable_ext.color_blend_enables) * (attachmentCount));
   }   

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

err:
   if (cmd)
      vk_free_cmd_set_color_blend_enable_ext(queue, cmd);
   return VK_ERROR_OUT_OF_HOST_MEMORY;
}

static void
vk_free_cmd_set_color_blend_equation_ext(struct vk_cmd_queue *queue,
                                         struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, ( VkColorBlendEquationEXT* )cmd->u.set_color_blend_equation_ext.color_blend_equations);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_set_color_blend_equation_ext(struct vk_cmd_queue *queue
, uint32_t firstAttachment
, uint32_t attachmentCount
, const VkColorBlendEquationEXT* pColorBlendEquations
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_SET_COLOR_BLEND_EQUATION_EXT;
      
   cmd->u.set_color_blend_equation_ext.first_attachment = firstAttachment;
   cmd->u.set_color_blend_equation_ext.attachment_count = attachmentCount;
   if (pColorBlendEquations) {
      cmd->u.set_color_blend_equation_ext.color_blend_equations = vk_zalloc(queue->alloc, sizeof(*cmd->u.set_color_blend_equation_ext.color_blend_equations) * (attachmentCount), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (cmd->u.set_color_blend_equation_ext.color_blend_equations == NULL) goto err;

   memcpy(( VkColorBlendEquationEXT* )cmd->u.set_color_blend_equation_ext.color_blend_equations, pColorBlendEquations, sizeof(*cmd->u.set_color_blend_equation_ext.color_blend_equations) * (attachmentCount));
   }   

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

err:
   if (cmd)
      vk_free_cmd_set_color_blend_equation_ext(queue, cmd);
   return VK_ERROR_OUT_OF_HOST_MEMORY;
}

static void
vk_free_cmd_set_color_write_mask_ext(struct vk_cmd_queue *queue,
                                     struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, ( VkColorComponentFlags* )cmd->u.set_color_write_mask_ext.color_write_masks);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_set_color_write_mask_ext(struct vk_cmd_queue *queue
, uint32_t firstAttachment
, uint32_t attachmentCount
, const VkColorComponentFlags* pColorWriteMasks
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_SET_COLOR_WRITE_MASK_EXT;
      
   cmd->u.set_color_write_mask_ext.first_attachment = firstAttachment;
   cmd->u.set_color_write_mask_ext.attachment_count = attachmentCount;
   if (pColorWriteMasks) {
      cmd->u.set_color_write_mask_ext.color_write_masks = vk_zalloc(queue->alloc, sizeof(*cmd->u.set_color_write_mask_ext.color_write_masks) * (attachmentCount), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (cmd->u.set_color_write_mask_ext.color_write_masks == NULL) goto err;

   memcpy(( VkColorComponentFlags* )cmd->u.set_color_write_mask_ext.color_write_masks, pColorWriteMasks, sizeof(*cmd->u.set_color_write_mask_ext.color_write_masks) * (attachmentCount));
   }   

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

err:
   if (cmd)
      vk_free_cmd_set_color_write_mask_ext(queue, cmd);
   return VK_ERROR_OUT_OF_HOST_MEMORY;
}

static void
vk_free_cmd_set_rasterization_stream_ext(struct vk_cmd_queue *queue,
                                         struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_set_rasterization_stream_ext(struct vk_cmd_queue *queue
, uint32_t rasterizationStream
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_SET_RASTERIZATION_STREAM_EXT;
      
   cmd->u.set_rasterization_stream_ext.rasterization_stream = rasterizationStream;

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

}

static void
vk_free_cmd_set_conservative_rasterization_mode_ext(struct vk_cmd_queue *queue,
                                                    struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_set_conservative_rasterization_mode_ext(struct vk_cmd_queue *queue
, VkConservativeRasterizationModeEXT conservativeRasterizationMode
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_SET_CONSERVATIVE_RASTERIZATION_MODE_EXT;
      
   cmd->u.set_conservative_rasterization_mode_ext.conservative_rasterization_mode = conservativeRasterizationMode;

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

}

static void
vk_free_cmd_set_extra_primitive_overestimation_size_ext(struct vk_cmd_queue *queue,
                                                        struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_set_extra_primitive_overestimation_size_ext(struct vk_cmd_queue *queue
, float extraPrimitiveOverestimationSize
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_SET_EXTRA_PRIMITIVE_OVERESTIMATION_SIZE_EXT;
      
   cmd->u.set_extra_primitive_overestimation_size_ext.extra_primitive_overestimation_size = extraPrimitiveOverestimationSize;

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

}

static void
vk_free_cmd_set_depth_clip_enable_ext(struct vk_cmd_queue *queue,
                                      struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_set_depth_clip_enable_ext(struct vk_cmd_queue *queue
, VkBool32 depthClipEnable
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_SET_DEPTH_CLIP_ENABLE_EXT;
      
   cmd->u.set_depth_clip_enable_ext.depth_clip_enable = depthClipEnable;

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

}

static void
vk_free_cmd_set_sample_locations_enable_ext(struct vk_cmd_queue *queue,
                                            struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_set_sample_locations_enable_ext(struct vk_cmd_queue *queue
, VkBool32 sampleLocationsEnable
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_SET_SAMPLE_LOCATIONS_ENABLE_EXT;
      
   cmd->u.set_sample_locations_enable_ext.sample_locations_enable = sampleLocationsEnable;

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

}

static void
vk_free_cmd_set_color_blend_advanced_ext(struct vk_cmd_queue *queue,
                                         struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, ( VkColorBlendAdvancedEXT* )cmd->u.set_color_blend_advanced_ext.color_blend_advanced);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_set_color_blend_advanced_ext(struct vk_cmd_queue *queue
, uint32_t firstAttachment
, uint32_t attachmentCount
, const VkColorBlendAdvancedEXT* pColorBlendAdvanced
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_SET_COLOR_BLEND_ADVANCED_EXT;
      
   cmd->u.set_color_blend_advanced_ext.first_attachment = firstAttachment;
   cmd->u.set_color_blend_advanced_ext.attachment_count = attachmentCount;
   if (pColorBlendAdvanced) {
      cmd->u.set_color_blend_advanced_ext.color_blend_advanced = vk_zalloc(queue->alloc, sizeof(*cmd->u.set_color_blend_advanced_ext.color_blend_advanced) * (attachmentCount), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (cmd->u.set_color_blend_advanced_ext.color_blend_advanced == NULL) goto err;

   memcpy(( VkColorBlendAdvancedEXT* )cmd->u.set_color_blend_advanced_ext.color_blend_advanced, pColorBlendAdvanced, sizeof(*cmd->u.set_color_blend_advanced_ext.color_blend_advanced) * (attachmentCount));
   }   

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

err:
   if (cmd)
      vk_free_cmd_set_color_blend_advanced_ext(queue, cmd);
   return VK_ERROR_OUT_OF_HOST_MEMORY;
}

static void
vk_free_cmd_set_provoking_vertex_mode_ext(struct vk_cmd_queue *queue,
                                          struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_set_provoking_vertex_mode_ext(struct vk_cmd_queue *queue
, VkProvokingVertexModeEXT provokingVertexMode
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_SET_PROVOKING_VERTEX_MODE_EXT;
      
   cmd->u.set_provoking_vertex_mode_ext.provoking_vertex_mode = provokingVertexMode;

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

}

static void
vk_free_cmd_set_line_rasterization_mode_ext(struct vk_cmd_queue *queue,
                                            struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_set_line_rasterization_mode_ext(struct vk_cmd_queue *queue
, VkLineRasterizationModeEXT lineRasterizationMode
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_SET_LINE_RASTERIZATION_MODE_EXT;
      
   cmd->u.set_line_rasterization_mode_ext.line_rasterization_mode = lineRasterizationMode;

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

}

static void
vk_free_cmd_set_line_stipple_enable_ext(struct vk_cmd_queue *queue,
                                        struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_set_line_stipple_enable_ext(struct vk_cmd_queue *queue
, VkBool32 stippledLineEnable
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_SET_LINE_STIPPLE_ENABLE_EXT;
      
   cmd->u.set_line_stipple_enable_ext.stippled_line_enable = stippledLineEnable;

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

}

static void
vk_free_cmd_set_depth_clip_negative_one_to_one_ext(struct vk_cmd_queue *queue,
                                                   struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_set_depth_clip_negative_one_to_one_ext(struct vk_cmd_queue *queue
, VkBool32 negativeOneToOne
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_SET_DEPTH_CLIP_NEGATIVE_ONE_TO_ONE_EXT;
      
   cmd->u.set_depth_clip_negative_one_to_one_ext.negative_one_to_one = negativeOneToOne;

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

}

static void
vk_free_cmd_set_viewport_wscaling_enable_nv(struct vk_cmd_queue *queue,
                                            struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_set_viewport_wscaling_enable_nv(struct vk_cmd_queue *queue
, VkBool32 viewportWScalingEnable
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_SET_VIEWPORT_WSCALING_ENABLE_NV;
      
   cmd->u.set_viewport_wscaling_enable_nv.viewport_wscaling_enable = viewportWScalingEnable;

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

}

static void
vk_free_cmd_set_viewport_swizzle_nv(struct vk_cmd_queue *queue,
                                    struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, ( VkViewportSwizzleNV* )cmd->u.set_viewport_swizzle_nv.viewport_swizzles);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_set_viewport_swizzle_nv(struct vk_cmd_queue *queue
, uint32_t firstViewport
, uint32_t viewportCount
, const VkViewportSwizzleNV* pViewportSwizzles
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_SET_VIEWPORT_SWIZZLE_NV;
      
   cmd->u.set_viewport_swizzle_nv.first_viewport = firstViewport;
   cmd->u.set_viewport_swizzle_nv.viewport_count = viewportCount;
   if (pViewportSwizzles) {
      cmd->u.set_viewport_swizzle_nv.viewport_swizzles = vk_zalloc(queue->alloc, sizeof(*cmd->u.set_viewport_swizzle_nv.viewport_swizzles) * (viewportCount), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (cmd->u.set_viewport_swizzle_nv.viewport_swizzles == NULL) goto err;

   memcpy(( VkViewportSwizzleNV* )cmd->u.set_viewport_swizzle_nv.viewport_swizzles, pViewportSwizzles, sizeof(*cmd->u.set_viewport_swizzle_nv.viewport_swizzles) * (viewportCount));
   }   

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

err:
   if (cmd)
      vk_free_cmd_set_viewport_swizzle_nv(queue, cmd);
   return VK_ERROR_OUT_OF_HOST_MEMORY;
}

static void
vk_free_cmd_set_coverage_to_color_enable_nv(struct vk_cmd_queue *queue,
                                            struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_set_coverage_to_color_enable_nv(struct vk_cmd_queue *queue
, VkBool32 coverageToColorEnable
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_SET_COVERAGE_TO_COLOR_ENABLE_NV;
      
   cmd->u.set_coverage_to_color_enable_nv.coverage_to_color_enable = coverageToColorEnable;

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

}

static void
vk_free_cmd_set_coverage_to_color_location_nv(struct vk_cmd_queue *queue,
                                              struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_set_coverage_to_color_location_nv(struct vk_cmd_queue *queue
, uint32_t coverageToColorLocation
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_SET_COVERAGE_TO_COLOR_LOCATION_NV;
      
   cmd->u.set_coverage_to_color_location_nv.coverage_to_color_location = coverageToColorLocation;

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

}

static void
vk_free_cmd_set_coverage_modulation_mode_nv(struct vk_cmd_queue *queue,
                                            struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_set_coverage_modulation_mode_nv(struct vk_cmd_queue *queue
, VkCoverageModulationModeNV coverageModulationMode
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_SET_COVERAGE_MODULATION_MODE_NV;
      
   cmd->u.set_coverage_modulation_mode_nv.coverage_modulation_mode = coverageModulationMode;

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

}

static void
vk_free_cmd_set_coverage_modulation_table_enable_nv(struct vk_cmd_queue *queue,
                                                    struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_set_coverage_modulation_table_enable_nv(struct vk_cmd_queue *queue
, VkBool32 coverageModulationTableEnable
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_SET_COVERAGE_MODULATION_TABLE_ENABLE_NV;
      
   cmd->u.set_coverage_modulation_table_enable_nv.coverage_modulation_table_enable = coverageModulationTableEnable;

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

}

static void
vk_free_cmd_set_coverage_modulation_table_nv(struct vk_cmd_queue *queue,
                                             struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, ( float* )cmd->u.set_coverage_modulation_table_nv.coverage_modulation_table);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_set_coverage_modulation_table_nv(struct vk_cmd_queue *queue
, uint32_t coverageModulationTableCount
, const float* pCoverageModulationTable
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_SET_COVERAGE_MODULATION_TABLE_NV;
      
   cmd->u.set_coverage_modulation_table_nv.coverage_modulation_table_count = coverageModulationTableCount;
   if (pCoverageModulationTable) {
      cmd->u.set_coverage_modulation_table_nv.coverage_modulation_table = vk_zalloc(queue->alloc, sizeof(*cmd->u.set_coverage_modulation_table_nv.coverage_modulation_table) * (coverageModulationTableCount), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (cmd->u.set_coverage_modulation_table_nv.coverage_modulation_table == NULL) goto err;

   memcpy(( float* )cmd->u.set_coverage_modulation_table_nv.coverage_modulation_table, pCoverageModulationTable, sizeof(*cmd->u.set_coverage_modulation_table_nv.coverage_modulation_table) * (coverageModulationTableCount));
   }   

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

err:
   if (cmd)
      vk_free_cmd_set_coverage_modulation_table_nv(queue, cmd);
   return VK_ERROR_OUT_OF_HOST_MEMORY;
}

static void
vk_free_cmd_set_shading_rate_image_enable_nv(struct vk_cmd_queue *queue,
                                             struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_set_shading_rate_image_enable_nv(struct vk_cmd_queue *queue
, VkBool32 shadingRateImageEnable
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_SET_SHADING_RATE_IMAGE_ENABLE_NV;
      
   cmd->u.set_shading_rate_image_enable_nv.shading_rate_image_enable = shadingRateImageEnable;

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

}

static void
vk_free_cmd_set_coverage_reduction_mode_nv(struct vk_cmd_queue *queue,
                                           struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_set_coverage_reduction_mode_nv(struct vk_cmd_queue *queue
, VkCoverageReductionModeNV coverageReductionMode
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_SET_COVERAGE_REDUCTION_MODE_NV;
      
   cmd->u.set_coverage_reduction_mode_nv.coverage_reduction_mode = coverageReductionMode;

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

}

static void
vk_free_cmd_set_representative_fragment_test_enable_nv(struct vk_cmd_queue *queue,
                                                       struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_set_representative_fragment_test_enable_nv(struct vk_cmd_queue *queue
, VkBool32 representativeFragmentTestEnable
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_SET_REPRESENTATIVE_FRAGMENT_TEST_ENABLE_NV;
      
   cmd->u.set_representative_fragment_test_enable_nv.representative_fragment_test_enable = representativeFragmentTestEnable;

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

}

static void
vk_free_cmd_copy_buffer2(struct vk_cmd_queue *queue,
                         struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, ( VkBufferCopy2* )cmd->u.copy_buffer2.copy_buffer_info->pRegions);
      vk_free(queue->alloc, ( VkCopyBufferInfo2* )cmd->u.copy_buffer2.copy_buffer_info);

   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_copy_buffer2(struct vk_cmd_queue *queue
, const VkCopyBufferInfo2* pCopyBufferInfo
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_COPY_BUFFER2;
      
   if (pCopyBufferInfo) {
      cmd->u.copy_buffer2.copy_buffer_info = vk_zalloc(queue->alloc, sizeof(VkCopyBufferInfo2), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
      if (cmd->u.copy_buffer2.copy_buffer_info == NULL) goto err;

      memcpy((void*)cmd->u.copy_buffer2.copy_buffer_info, pCopyBufferInfo, sizeof(VkCopyBufferInfo2));
   VkCopyBufferInfo2 *tmp_dst1 = (void *) cmd->u.copy_buffer2.copy_buffer_info; (void) tmp_dst1;
   VkCopyBufferInfo2 *tmp_src1 = (void *) pCopyBufferInfo; (void) tmp_src1;   
   if (tmp_src1->pRegions) {
   tmp_dst1->pRegions = vk_zalloc(queue->alloc, sizeof(*tmp_dst1->pRegions) * tmp_dst1->regionCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (tmp_dst1->pRegions == NULL) goto err;

   memcpy(( VkBufferCopy2* )tmp_dst1->pRegions, tmp_src1->pRegions, sizeof(*tmp_dst1->pRegions) * tmp_dst1->regionCount);
}
   } else {
      cmd->u.copy_buffer2.copy_buffer_info = NULL;
   }   

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

err:
   if (cmd)
      vk_free_cmd_copy_buffer2(queue, cmd);
   return VK_ERROR_OUT_OF_HOST_MEMORY;
}

static void
vk_free_cmd_copy_image2(struct vk_cmd_queue *queue,
                        struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, ( VkImageCopy2* )cmd->u.copy_image2.copy_image_info->pRegions);
      vk_free(queue->alloc, ( VkCopyImageInfo2* )cmd->u.copy_image2.copy_image_info);

   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_copy_image2(struct vk_cmd_queue *queue
, const VkCopyImageInfo2* pCopyImageInfo
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_COPY_IMAGE2;
      
   if (pCopyImageInfo) {
      cmd->u.copy_image2.copy_image_info = vk_zalloc(queue->alloc, sizeof(VkCopyImageInfo2), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
      if (cmd->u.copy_image2.copy_image_info == NULL) goto err;

      memcpy((void*)cmd->u.copy_image2.copy_image_info, pCopyImageInfo, sizeof(VkCopyImageInfo2));
   VkCopyImageInfo2 *tmp_dst1 = (void *) cmd->u.copy_image2.copy_image_info; (void) tmp_dst1;
   VkCopyImageInfo2 *tmp_src1 = (void *) pCopyImageInfo; (void) tmp_src1;   
   if (tmp_src1->pRegions) {
   tmp_dst1->pRegions = vk_zalloc(queue->alloc, sizeof(*tmp_dst1->pRegions) * tmp_dst1->regionCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (tmp_dst1->pRegions == NULL) goto err;

   memcpy(( VkImageCopy2* )tmp_dst1->pRegions, tmp_src1->pRegions, sizeof(*tmp_dst1->pRegions) * tmp_dst1->regionCount);
}
   } else {
      cmd->u.copy_image2.copy_image_info = NULL;
   }   

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

err:
   if (cmd)
      vk_free_cmd_copy_image2(queue, cmd);
   return VK_ERROR_OUT_OF_HOST_MEMORY;
}

static void
vk_free_cmd_blit_image2(struct vk_cmd_queue *queue,
                        struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, ( VkImageBlit2*  )cmd->u.blit_image2.blit_image_info->pRegions);
      vk_free(queue->alloc, ( VkBlitImageInfo2* )cmd->u.blit_image2.blit_image_info);

   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_blit_image2(struct vk_cmd_queue *queue
, const VkBlitImageInfo2* pBlitImageInfo
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_BLIT_IMAGE2;
      
   if (pBlitImageInfo) {
      cmd->u.blit_image2.blit_image_info = vk_zalloc(queue->alloc, sizeof(VkBlitImageInfo2), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
      if (cmd->u.blit_image2.blit_image_info == NULL) goto err;

      memcpy((void*)cmd->u.blit_image2.blit_image_info, pBlitImageInfo, sizeof(VkBlitImageInfo2));
   VkBlitImageInfo2 *tmp_dst1 = (void *) cmd->u.blit_image2.blit_image_info; (void) tmp_dst1;
   VkBlitImageInfo2 *tmp_src1 = (void *) pBlitImageInfo; (void) tmp_src1;   
   if (tmp_src1->pRegions) {
   tmp_dst1->pRegions = vk_zalloc(queue->alloc, sizeof(*tmp_dst1->pRegions) * tmp_dst1->regionCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (tmp_dst1->pRegions == NULL) goto err;

   memcpy(( VkImageBlit2*  )tmp_dst1->pRegions, tmp_src1->pRegions, sizeof(*tmp_dst1->pRegions) * tmp_dst1->regionCount);
}
   } else {
      cmd->u.blit_image2.blit_image_info = NULL;
   }   

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

err:
   if (cmd)
      vk_free_cmd_blit_image2(queue, cmd);
   return VK_ERROR_OUT_OF_HOST_MEMORY;
}

static void
vk_free_cmd_copy_buffer_to_image2(struct vk_cmd_queue *queue,
                                  struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, ( VkBufferImageCopy2* )cmd->u.copy_buffer_to_image2.copy_buffer_to_image_info->pRegions);
      vk_free(queue->alloc, ( VkCopyBufferToImageInfo2* )cmd->u.copy_buffer_to_image2.copy_buffer_to_image_info);

   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_copy_buffer_to_image2(struct vk_cmd_queue *queue
, const VkCopyBufferToImageInfo2* pCopyBufferToImageInfo
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_COPY_BUFFER_TO_IMAGE2;
      
   if (pCopyBufferToImageInfo) {
      cmd->u.copy_buffer_to_image2.copy_buffer_to_image_info = vk_zalloc(queue->alloc, sizeof(VkCopyBufferToImageInfo2), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
      if (cmd->u.copy_buffer_to_image2.copy_buffer_to_image_info == NULL) goto err;

      memcpy((void*)cmd->u.copy_buffer_to_image2.copy_buffer_to_image_info, pCopyBufferToImageInfo, sizeof(VkCopyBufferToImageInfo2));
   VkCopyBufferToImageInfo2 *tmp_dst1 = (void *) cmd->u.copy_buffer_to_image2.copy_buffer_to_image_info; (void) tmp_dst1;
   VkCopyBufferToImageInfo2 *tmp_src1 = (void *) pCopyBufferToImageInfo; (void) tmp_src1;   
   if (tmp_src1->pRegions) {
   tmp_dst1->pRegions = vk_zalloc(queue->alloc, sizeof(*tmp_dst1->pRegions) * tmp_dst1->regionCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (tmp_dst1->pRegions == NULL) goto err;

   memcpy(( VkBufferImageCopy2* )tmp_dst1->pRegions, tmp_src1->pRegions, sizeof(*tmp_dst1->pRegions) * tmp_dst1->regionCount);
}
   } else {
      cmd->u.copy_buffer_to_image2.copy_buffer_to_image_info = NULL;
   }   

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

err:
   if (cmd)
      vk_free_cmd_copy_buffer_to_image2(queue, cmd);
   return VK_ERROR_OUT_OF_HOST_MEMORY;
}

static void
vk_free_cmd_copy_image_to_buffer2(struct vk_cmd_queue *queue,
                                  struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, ( VkBufferImageCopy2*   )cmd->u.copy_image_to_buffer2.copy_image_to_buffer_info->pRegions);
      vk_free(queue->alloc, ( VkCopyImageToBufferInfo2* )cmd->u.copy_image_to_buffer2.copy_image_to_buffer_info);

   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_copy_image_to_buffer2(struct vk_cmd_queue *queue
, const VkCopyImageToBufferInfo2* pCopyImageToBufferInfo
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_COPY_IMAGE_TO_BUFFER2;
      
   if (pCopyImageToBufferInfo) {
      cmd->u.copy_image_to_buffer2.copy_image_to_buffer_info = vk_zalloc(queue->alloc, sizeof(VkCopyImageToBufferInfo2), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
      if (cmd->u.copy_image_to_buffer2.copy_image_to_buffer_info == NULL) goto err;

      memcpy((void*)cmd->u.copy_image_to_buffer2.copy_image_to_buffer_info, pCopyImageToBufferInfo, sizeof(VkCopyImageToBufferInfo2));
   VkCopyImageToBufferInfo2 *tmp_dst1 = (void *) cmd->u.copy_image_to_buffer2.copy_image_to_buffer_info; (void) tmp_dst1;
   VkCopyImageToBufferInfo2 *tmp_src1 = (void *) pCopyImageToBufferInfo; (void) tmp_src1;   
   if (tmp_src1->pRegions) {
   tmp_dst1->pRegions = vk_zalloc(queue->alloc, sizeof(*tmp_dst1->pRegions) * tmp_dst1->regionCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (tmp_dst1->pRegions == NULL) goto err;

   memcpy(( VkBufferImageCopy2*   )tmp_dst1->pRegions, tmp_src1->pRegions, sizeof(*tmp_dst1->pRegions) * tmp_dst1->regionCount);
}
   } else {
      cmd->u.copy_image_to_buffer2.copy_image_to_buffer_info = NULL;
   }   

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

err:
   if (cmd)
      vk_free_cmd_copy_image_to_buffer2(queue, cmd);
   return VK_ERROR_OUT_OF_HOST_MEMORY;
}

static void
vk_free_cmd_resolve_image2(struct vk_cmd_queue *queue,
                           struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, ( VkImageResolve2*   )cmd->u.resolve_image2.resolve_image_info->pRegions);
      vk_free(queue->alloc, ( VkResolveImageInfo2* )cmd->u.resolve_image2.resolve_image_info);

   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_resolve_image2(struct vk_cmd_queue *queue
, const VkResolveImageInfo2* pResolveImageInfo
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_RESOLVE_IMAGE2;
      
   if (pResolveImageInfo) {
      cmd->u.resolve_image2.resolve_image_info = vk_zalloc(queue->alloc, sizeof(VkResolveImageInfo2), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
      if (cmd->u.resolve_image2.resolve_image_info == NULL) goto err;

      memcpy((void*)cmd->u.resolve_image2.resolve_image_info, pResolveImageInfo, sizeof(VkResolveImageInfo2));
   VkResolveImageInfo2 *tmp_dst1 = (void *) cmd->u.resolve_image2.resolve_image_info; (void) tmp_dst1;
   VkResolveImageInfo2 *tmp_src1 = (void *) pResolveImageInfo; (void) tmp_src1;   
   if (tmp_src1->pRegions) {
   tmp_dst1->pRegions = vk_zalloc(queue->alloc, sizeof(*tmp_dst1->pRegions) * tmp_dst1->regionCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (tmp_dst1->pRegions == NULL) goto err;

   memcpy(( VkImageResolve2*   )tmp_dst1->pRegions, tmp_src1->pRegions, sizeof(*tmp_dst1->pRegions) * tmp_dst1->regionCount);
}
   } else {
      cmd->u.resolve_image2.resolve_image_info = NULL;
   }   

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

err:
   if (cmd)
      vk_free_cmd_resolve_image2(queue, cmd);
   return VK_ERROR_OUT_OF_HOST_MEMORY;
}

static void
vk_free_cmd_set_fragment_shading_rate_khr(struct vk_cmd_queue *queue,
                                          struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
         vk_free(queue->alloc, ( VkExtent2D*                           )cmd->u.set_fragment_shading_rate_khr.fragment_size);

   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_set_fragment_shading_rate_khr(struct vk_cmd_queue *queue
, const VkExtent2D*                           pFragmentSize
, const VkFragmentShadingRateCombinerOpKHR    combinerOps[2]
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_SET_FRAGMENT_SHADING_RATE_KHR;
      
   if (pFragmentSize) {
      cmd->u.set_fragment_shading_rate_khr.fragment_size = vk_zalloc(queue->alloc, sizeof(VkExtent2D), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
      if (cmd->u.set_fragment_shading_rate_khr.fragment_size == NULL) goto err;

      memcpy((void*)cmd->u.set_fragment_shading_rate_khr.fragment_size, pFragmentSize, sizeof(VkExtent2D));
   VkExtent2D *tmp_dst1 = (void *) cmd->u.set_fragment_shading_rate_khr.fragment_size; (void) tmp_dst1;
   VkExtent2D *tmp_src1 = (void *) pFragmentSize; (void) tmp_src1;   
      } else {
      cmd->u.set_fragment_shading_rate_khr.fragment_size = NULL;
   }   
   memcpy(cmd->u.set_fragment_shading_rate_khr.combiner_ops, combinerOps,
          sizeof(*combinerOps) * 2);

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

err:
   if (cmd)
      vk_free_cmd_set_fragment_shading_rate_khr(queue, cmd);
   return VK_ERROR_OUT_OF_HOST_MEMORY;
}

static void
vk_free_cmd_set_fragment_shading_rate_enum_nv(struct vk_cmd_queue *queue,
                                              struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_set_fragment_shading_rate_enum_nv(struct vk_cmd_queue *queue
, VkFragmentShadingRateNV                     shadingRate
, const VkFragmentShadingRateCombinerOpKHR    combinerOps[2]
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_SET_FRAGMENT_SHADING_RATE_ENUM_NV;
      
   cmd->u.set_fragment_shading_rate_enum_nv.shading_rate = shadingRate;
   memcpy(cmd->u.set_fragment_shading_rate_enum_nv.combiner_ops, combinerOps,
          sizeof(*combinerOps) * 2);

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

}

static void
vk_free_cmd_set_vertex_input_ext(struct vk_cmd_queue *queue,
                                 struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, ( VkVertexInputBindingDescription2EXT* )cmd->u.set_vertex_input_ext.vertex_binding_descriptions);
   vk_free(queue->alloc, ( VkVertexInputAttributeDescription2EXT* )cmd->u.set_vertex_input_ext.vertex_attribute_descriptions);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_set_vertex_input_ext(struct vk_cmd_queue *queue
, uint32_t vertexBindingDescriptionCount
, const VkVertexInputBindingDescription2EXT* pVertexBindingDescriptions
, uint32_t vertexAttributeDescriptionCount
, const VkVertexInputAttributeDescription2EXT* pVertexAttributeDescriptions
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_SET_VERTEX_INPUT_EXT;
      
   cmd->u.set_vertex_input_ext.vertex_binding_description_count = vertexBindingDescriptionCount;
   if (pVertexBindingDescriptions) {
      cmd->u.set_vertex_input_ext.vertex_binding_descriptions = vk_zalloc(queue->alloc, sizeof(*cmd->u.set_vertex_input_ext.vertex_binding_descriptions) * (vertexBindingDescriptionCount), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (cmd->u.set_vertex_input_ext.vertex_binding_descriptions == NULL) goto err;

   memcpy(( VkVertexInputBindingDescription2EXT* )cmd->u.set_vertex_input_ext.vertex_binding_descriptions, pVertexBindingDescriptions, sizeof(*cmd->u.set_vertex_input_ext.vertex_binding_descriptions) * (vertexBindingDescriptionCount));
   }   
   cmd->u.set_vertex_input_ext.vertex_attribute_description_count = vertexAttributeDescriptionCount;
   if (pVertexAttributeDescriptions) {
      cmd->u.set_vertex_input_ext.vertex_attribute_descriptions = vk_zalloc(queue->alloc, sizeof(*cmd->u.set_vertex_input_ext.vertex_attribute_descriptions) * (vertexAttributeDescriptionCount), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (cmd->u.set_vertex_input_ext.vertex_attribute_descriptions == NULL) goto err;

   memcpy(( VkVertexInputAttributeDescription2EXT* )cmd->u.set_vertex_input_ext.vertex_attribute_descriptions, pVertexAttributeDescriptions, sizeof(*cmd->u.set_vertex_input_ext.vertex_attribute_descriptions) * (vertexAttributeDescriptionCount));
   }   

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

err:
   if (cmd)
      vk_free_cmd_set_vertex_input_ext(queue, cmd);
   return VK_ERROR_OUT_OF_HOST_MEMORY;
}

static void
vk_free_cmd_set_color_write_enable_ext(struct vk_cmd_queue *queue,
                                       struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, ( VkBool32*   )cmd->u.set_color_write_enable_ext.color_write_enables);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_set_color_write_enable_ext(struct vk_cmd_queue *queue
, uint32_t                                attachmentCount
, const VkBool32*   pColorWriteEnables
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_SET_COLOR_WRITE_ENABLE_EXT;
      
   cmd->u.set_color_write_enable_ext.attachment_count = attachmentCount;
   if (pColorWriteEnables) {
      cmd->u.set_color_write_enable_ext.color_write_enables = vk_zalloc(queue->alloc, sizeof(*cmd->u.set_color_write_enable_ext.color_write_enables) * (attachmentCount), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (cmd->u.set_color_write_enable_ext.color_write_enables == NULL) goto err;

   memcpy(( VkBool32*   )cmd->u.set_color_write_enable_ext.color_write_enables, pColorWriteEnables, sizeof(*cmd->u.set_color_write_enable_ext.color_write_enables) * (attachmentCount));
   }   

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

err:
   if (cmd)
      vk_free_cmd_set_color_write_enable_ext(queue, cmd);
   return VK_ERROR_OUT_OF_HOST_MEMORY;
}

static void
vk_free_cmd_set_event2(struct vk_cmd_queue *queue,
                       struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, ( VkMemoryBarrier2*             )cmd->u.set_event2.dependency_info->pMemoryBarriers);
vk_free(queue->alloc, ( VkBufferMemoryBarrier2* )cmd->u.set_event2.dependency_info->pBufferMemoryBarriers);
vk_free(queue->alloc, ( VkImageMemoryBarrier2*   )cmd->u.set_event2.dependency_info->pImageMemoryBarriers);
      vk_free(queue->alloc, ( VkDependencyInfo*                             )cmd->u.set_event2.dependency_info);

   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_set_event2(struct vk_cmd_queue *queue
, VkEvent                                             event
, const VkDependencyInfo*                             pDependencyInfo
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_SET_EVENT2;
      
   cmd->u.set_event2.event = event;
   if (pDependencyInfo) {
      cmd->u.set_event2.dependency_info = vk_zalloc(queue->alloc, sizeof(VkDependencyInfo), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
      if (cmd->u.set_event2.dependency_info == NULL) goto err;

      memcpy((void*)cmd->u.set_event2.dependency_info, pDependencyInfo, sizeof(VkDependencyInfo));
   VkDependencyInfo *tmp_dst1 = (void *) cmd->u.set_event2.dependency_info; (void) tmp_dst1;
   VkDependencyInfo *tmp_src1 = (void *) pDependencyInfo; (void) tmp_src1;   
   if (tmp_src1->pMemoryBarriers) {
   tmp_dst1->pMemoryBarriers = vk_zalloc(queue->alloc, sizeof(*tmp_dst1->pMemoryBarriers) * tmp_dst1->memoryBarrierCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (tmp_dst1->pMemoryBarriers == NULL) goto err;

   memcpy(( VkMemoryBarrier2*             )tmp_dst1->pMemoryBarriers, tmp_src1->pMemoryBarriers, sizeof(*tmp_dst1->pMemoryBarriers) * tmp_dst1->memoryBarrierCount);
}
if (tmp_src1->pBufferMemoryBarriers) {
   tmp_dst1->pBufferMemoryBarriers = vk_zalloc(queue->alloc, sizeof(*tmp_dst1->pBufferMemoryBarriers) * tmp_dst1->bufferMemoryBarrierCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (tmp_dst1->pBufferMemoryBarriers == NULL) goto err;

   memcpy(( VkBufferMemoryBarrier2* )tmp_dst1->pBufferMemoryBarriers, tmp_src1->pBufferMemoryBarriers, sizeof(*tmp_dst1->pBufferMemoryBarriers) * tmp_dst1->bufferMemoryBarrierCount);
}
if (tmp_src1->pImageMemoryBarriers) {
   tmp_dst1->pImageMemoryBarriers = vk_zalloc(queue->alloc, sizeof(*tmp_dst1->pImageMemoryBarriers) * tmp_dst1->imageMemoryBarrierCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (tmp_dst1->pImageMemoryBarriers == NULL) goto err;

   memcpy(( VkImageMemoryBarrier2*   )tmp_dst1->pImageMemoryBarriers, tmp_src1->pImageMemoryBarriers, sizeof(*tmp_dst1->pImageMemoryBarriers) * tmp_dst1->imageMemoryBarrierCount);
}
   } else {
      cmd->u.set_event2.dependency_info = NULL;
   }   

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

err:
   if (cmd)
      vk_free_cmd_set_event2(queue, cmd);
   return VK_ERROR_OUT_OF_HOST_MEMORY;
}

static void
vk_free_cmd_reset_event2(struct vk_cmd_queue *queue,
                         struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_reset_event2(struct vk_cmd_queue *queue
, VkEvent                                             event
, VkPipelineStageFlags2               stageMask
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_RESET_EVENT2;
      
   cmd->u.reset_event2.event = event;
   cmd->u.reset_event2.stage_mask = stageMask;

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

}

static void
vk_free_cmd_wait_events2(struct vk_cmd_queue *queue,
                         struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, ( VkEvent*                     )cmd->u.wait_events2.events);
   vk_free(queue->alloc, ( VkDependencyInfo*            )cmd->u.wait_events2.dependency_infos);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_wait_events2(struct vk_cmd_queue *queue
, uint32_t                                            eventCount
, const VkEvent*                     pEvents
, const VkDependencyInfo*            pDependencyInfos
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_WAIT_EVENTS2;
      
   cmd->u.wait_events2.event_count = eventCount;
   if (pEvents) {
      cmd->u.wait_events2.events = vk_zalloc(queue->alloc, sizeof(*cmd->u.wait_events2.events) * (eventCount), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (cmd->u.wait_events2.events == NULL) goto err;

   memcpy(( VkEvent*                     )cmd->u.wait_events2.events, pEvents, sizeof(*cmd->u.wait_events2.events) * (eventCount));
   }   
   if (pDependencyInfos) {
      cmd->u.wait_events2.dependency_infos = vk_zalloc(queue->alloc, sizeof(*cmd->u.wait_events2.dependency_infos) * (eventCount), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (cmd->u.wait_events2.dependency_infos == NULL) goto err;

   memcpy(( VkDependencyInfo*            )cmd->u.wait_events2.dependency_infos, pDependencyInfos, sizeof(*cmd->u.wait_events2.dependency_infos) * (eventCount));
   }   

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

err:
   if (cmd)
      vk_free_cmd_wait_events2(queue, cmd);
   return VK_ERROR_OUT_OF_HOST_MEMORY;
}

static void
vk_free_cmd_pipeline_barrier2(struct vk_cmd_queue *queue,
                              struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, ( VkMemoryBarrier2*             )cmd->u.pipeline_barrier2.dependency_info->pMemoryBarriers);
vk_free(queue->alloc, ( VkBufferMemoryBarrier2* )cmd->u.pipeline_barrier2.dependency_info->pBufferMemoryBarriers);
vk_free(queue->alloc, ( VkImageMemoryBarrier2*   )cmd->u.pipeline_barrier2.dependency_info->pImageMemoryBarriers);
      vk_free(queue->alloc, ( VkDependencyInfo*                             )cmd->u.pipeline_barrier2.dependency_info);

   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_pipeline_barrier2(struct vk_cmd_queue *queue
, const VkDependencyInfo*                             pDependencyInfo
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_PIPELINE_BARRIER2;
      
   if (pDependencyInfo) {
      cmd->u.pipeline_barrier2.dependency_info = vk_zalloc(queue->alloc, sizeof(VkDependencyInfo), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
      if (cmd->u.pipeline_barrier2.dependency_info == NULL) goto err;

      memcpy((void*)cmd->u.pipeline_barrier2.dependency_info, pDependencyInfo, sizeof(VkDependencyInfo));
   VkDependencyInfo *tmp_dst1 = (void *) cmd->u.pipeline_barrier2.dependency_info; (void) tmp_dst1;
   VkDependencyInfo *tmp_src1 = (void *) pDependencyInfo; (void) tmp_src1;   
   if (tmp_src1->pMemoryBarriers) {
   tmp_dst1->pMemoryBarriers = vk_zalloc(queue->alloc, sizeof(*tmp_dst1->pMemoryBarriers) * tmp_dst1->memoryBarrierCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (tmp_dst1->pMemoryBarriers == NULL) goto err;

   memcpy(( VkMemoryBarrier2*             )tmp_dst1->pMemoryBarriers, tmp_src1->pMemoryBarriers, sizeof(*tmp_dst1->pMemoryBarriers) * tmp_dst1->memoryBarrierCount);
}
if (tmp_src1->pBufferMemoryBarriers) {
   tmp_dst1->pBufferMemoryBarriers = vk_zalloc(queue->alloc, sizeof(*tmp_dst1->pBufferMemoryBarriers) * tmp_dst1->bufferMemoryBarrierCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (tmp_dst1->pBufferMemoryBarriers == NULL) goto err;

   memcpy(( VkBufferMemoryBarrier2* )tmp_dst1->pBufferMemoryBarriers, tmp_src1->pBufferMemoryBarriers, sizeof(*tmp_dst1->pBufferMemoryBarriers) * tmp_dst1->bufferMemoryBarrierCount);
}
if (tmp_src1->pImageMemoryBarriers) {
   tmp_dst1->pImageMemoryBarriers = vk_zalloc(queue->alloc, sizeof(*tmp_dst1->pImageMemoryBarriers) * tmp_dst1->imageMemoryBarrierCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (tmp_dst1->pImageMemoryBarriers == NULL) goto err;

   memcpy(( VkImageMemoryBarrier2*   )tmp_dst1->pImageMemoryBarriers, tmp_src1->pImageMemoryBarriers, sizeof(*tmp_dst1->pImageMemoryBarriers) * tmp_dst1->imageMemoryBarrierCount);
}
   } else {
      cmd->u.pipeline_barrier2.dependency_info = NULL;
   }   

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

err:
   if (cmd)
      vk_free_cmd_pipeline_barrier2(queue, cmd);
   return VK_ERROR_OUT_OF_HOST_MEMORY;
}

static void
vk_free_cmd_write_timestamp2(struct vk_cmd_queue *queue,
                             struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_write_timestamp2(struct vk_cmd_queue *queue
, VkPipelineStageFlags2               stage
, VkQueryPool                                         queryPool
, uint32_t                                            query
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_WRITE_TIMESTAMP2;
      
   cmd->u.write_timestamp2.stage = stage;
   cmd->u.write_timestamp2.query_pool = queryPool;
   cmd->u.write_timestamp2.query = query;

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

}

static void
vk_free_cmd_write_buffer_marker2_amd(struct vk_cmd_queue *queue,
                                     struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_write_buffer_marker2_amd(struct vk_cmd_queue *queue
, VkPipelineStageFlags2               stage
, VkBuffer                                            dstBuffer
, VkDeviceSize                                        dstOffset
, uint32_t                                            marker
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_WRITE_BUFFER_MARKER2_AMD;
      
   cmd->u.write_buffer_marker2_amd.stage = stage;
   cmd->u.write_buffer_marker2_amd.dst_buffer = dstBuffer;
   cmd->u.write_buffer_marker2_amd.dst_offset = dstOffset;
   cmd->u.write_buffer_marker2_amd.marker = marker;

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

}

#ifdef VK_ENABLE_BETA_EXTENSIONS
static void
vk_free_cmd_decode_video_khr(struct vk_cmd_queue *queue,
                             struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, ( VkVideoReferenceSlotInfoKHR* )cmd->u.decode_video_khr.decode_info->pSetupReferenceSlot);
vk_free(queue->alloc, ( VkVideoReferenceSlotInfoKHR* )cmd->u.decode_video_khr.decode_info->pReferenceSlots);
      vk_free(queue->alloc, ( VkVideoDecodeInfoKHR* )cmd->u.decode_video_khr.decode_info);

   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_decode_video_khr(struct vk_cmd_queue *queue
, const VkVideoDecodeInfoKHR* pDecodeInfo
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_DECODE_VIDEO_KHR;
      
   if (pDecodeInfo) {
      cmd->u.decode_video_khr.decode_info = vk_zalloc(queue->alloc, sizeof(VkVideoDecodeInfoKHR), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
      if (cmd->u.decode_video_khr.decode_info == NULL) goto err;

      memcpy((void*)cmd->u.decode_video_khr.decode_info, pDecodeInfo, sizeof(VkVideoDecodeInfoKHR));
   VkVideoDecodeInfoKHR *tmp_dst1 = (void *) cmd->u.decode_video_khr.decode_info; (void) tmp_dst1;
   VkVideoDecodeInfoKHR *tmp_src1 = (void *) pDecodeInfo; (void) tmp_src1;   
   
      const VkBaseInStructure *pnext = tmp_dst1->pNext;
      if (pnext) {
         switch ((int32_t)pnext->sType) {
         
      case VK_STRUCTURE_TYPE_VIDEO_DECODE_H264_PICTURE_INFO_EXT:
         if (pnext) {
      tmp_dst1->pNext = vk_zalloc(queue->alloc, sizeof(VkVideoDecodeH264PictureInfoEXT), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
      if (tmp_dst1->pNext == NULL) goto err;

      memcpy((void*)tmp_dst1->pNext, pnext, sizeof(VkVideoDecodeH264PictureInfoEXT));
   VkVideoDecodeH264PictureInfoEXT *tmp_dst2 = (void *) tmp_dst1->pNext; (void) tmp_dst2;
   VkVideoDecodeH264PictureInfoEXT *tmp_src2 = (void *) pnext; (void) tmp_src2;   
   if (tmp_src2->pStdPictureInfo) {
   tmp_dst2->pStdPictureInfo = vk_zalloc(queue->alloc, sizeof(*tmp_dst2->pStdPictureInfo), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (tmp_dst2->pStdPictureInfo == NULL) goto err;

   memcpy(( StdVideoDecodeH264PictureInfo*  )tmp_dst2->pStdPictureInfo, tmp_src2->pStdPictureInfo, sizeof(*tmp_dst2->pStdPictureInfo));
}
if (tmp_src2->pSliceOffsets) {
   tmp_dst2->pSliceOffsets = vk_zalloc(queue->alloc, sizeof(*tmp_dst2->pSliceOffsets) * tmp_dst2->sliceCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (tmp_dst2->pSliceOffsets == NULL) goto err;

   memcpy(( uint32_t*      )tmp_dst2->pSliceOffsets, tmp_src2->pSliceOffsets, sizeof(*tmp_dst2->pSliceOffsets) * tmp_dst2->sliceCount);
}
   } else {
      tmp_dst1->pNext = NULL;
   }
         break;
      
      case VK_STRUCTURE_TYPE_VIDEO_DECODE_H265_PICTURE_INFO_EXT:
         if (pnext) {
      tmp_dst1->pNext = vk_zalloc(queue->alloc, sizeof(VkVideoDecodeH265PictureInfoEXT), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
      if (tmp_dst1->pNext == NULL) goto err;

      memcpy((void*)tmp_dst1->pNext, pnext, sizeof(VkVideoDecodeH265PictureInfoEXT));
   VkVideoDecodeH265PictureInfoEXT *tmp_dst2 = (void *) tmp_dst1->pNext; (void) tmp_dst2;
   VkVideoDecodeH265PictureInfoEXT *tmp_src2 = (void *) pnext; (void) tmp_src2;   
   if (tmp_src2->pStdPictureInfo) {
   tmp_dst2->pStdPictureInfo = vk_zalloc(queue->alloc, sizeof(*tmp_dst2->pStdPictureInfo), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (tmp_dst2->pStdPictureInfo == NULL) goto err;

   memcpy((StdVideoDecodeH265PictureInfo*     )tmp_dst2->pStdPictureInfo, tmp_src2->pStdPictureInfo, sizeof(*tmp_dst2->pStdPictureInfo));
}
if (tmp_src2->pSliceOffsets) {
   tmp_dst2->pSliceOffsets = vk_zalloc(queue->alloc, sizeof(*tmp_dst2->pSliceOffsets) * tmp_dst2->sliceCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (tmp_dst2->pSliceOffsets == NULL) goto err;

   memcpy(( uint32_t*   )tmp_dst2->pSliceOffsets, tmp_src2->pSliceOffsets, sizeof(*tmp_dst2->pSliceOffsets) * tmp_dst2->sliceCount);
}
   } else {
      tmp_dst1->pNext = NULL;
   }
         break;
      
         }
      }
      if (tmp_src1->pSetupReferenceSlot) {
   tmp_dst1->pSetupReferenceSlot = vk_zalloc(queue->alloc, sizeof(*tmp_dst1->pSetupReferenceSlot), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (tmp_dst1->pSetupReferenceSlot == NULL) goto err;

   memcpy(( VkVideoReferenceSlotInfoKHR* )tmp_dst1->pSetupReferenceSlot, tmp_src1->pSetupReferenceSlot, sizeof(*tmp_dst1->pSetupReferenceSlot));
}
if (tmp_src1->pReferenceSlots) {
   tmp_dst1->pReferenceSlots = vk_zalloc(queue->alloc, sizeof(*tmp_dst1->pReferenceSlots) * tmp_dst1->referenceSlotCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (tmp_dst1->pReferenceSlots == NULL) goto err;

   memcpy(( VkVideoReferenceSlotInfoKHR* )tmp_dst1->pReferenceSlots, tmp_src1->pReferenceSlots, sizeof(*tmp_dst1->pReferenceSlots) * tmp_dst1->referenceSlotCount);
}
   } else {
      cmd->u.decode_video_khr.decode_info = NULL;
   }   

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

err:
   if (cmd)
      vk_free_cmd_decode_video_khr(queue, cmd);
   return VK_ERROR_OUT_OF_HOST_MEMORY;
}
#endif // VK_ENABLE_BETA_EXTENSIONS

#ifdef VK_ENABLE_BETA_EXTENSIONS
static void
vk_free_cmd_begin_video_coding_khr(struct vk_cmd_queue *queue,
                                   struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, ( VkVideoReferenceSlotInfoKHR* )cmd->u.begin_video_coding_khr.begin_info->pReferenceSlots);
      vk_free(queue->alloc, ( VkVideoBeginCodingInfoKHR* )cmd->u.begin_video_coding_khr.begin_info);

   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_begin_video_coding_khr(struct vk_cmd_queue *queue
, const VkVideoBeginCodingInfoKHR* pBeginInfo
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_BEGIN_VIDEO_CODING_KHR;
      
   if (pBeginInfo) {
      cmd->u.begin_video_coding_khr.begin_info = vk_zalloc(queue->alloc, sizeof(VkVideoBeginCodingInfoKHR), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
      if (cmd->u.begin_video_coding_khr.begin_info == NULL) goto err;

      memcpy((void*)cmd->u.begin_video_coding_khr.begin_info, pBeginInfo, sizeof(VkVideoBeginCodingInfoKHR));
   VkVideoBeginCodingInfoKHR *tmp_dst1 = (void *) cmd->u.begin_video_coding_khr.begin_info; (void) tmp_dst1;
   VkVideoBeginCodingInfoKHR *tmp_src1 = (void *) pBeginInfo; (void) tmp_src1;   
   if (tmp_src1->pReferenceSlots) {
   tmp_dst1->pReferenceSlots = vk_zalloc(queue->alloc, sizeof(*tmp_dst1->pReferenceSlots) * tmp_dst1->referenceSlotCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (tmp_dst1->pReferenceSlots == NULL) goto err;

   memcpy(( VkVideoReferenceSlotInfoKHR* )tmp_dst1->pReferenceSlots, tmp_src1->pReferenceSlots, sizeof(*tmp_dst1->pReferenceSlots) * tmp_dst1->referenceSlotCount);
}
   } else {
      cmd->u.begin_video_coding_khr.begin_info = NULL;
   }   

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

err:
   if (cmd)
      vk_free_cmd_begin_video_coding_khr(queue, cmd);
   return VK_ERROR_OUT_OF_HOST_MEMORY;
}
#endif // VK_ENABLE_BETA_EXTENSIONS

#ifdef VK_ENABLE_BETA_EXTENSIONS
static void
vk_free_cmd_control_video_coding_khr(struct vk_cmd_queue *queue,
                                     struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
         vk_free(queue->alloc, ( VkVideoCodingControlInfoKHR* )cmd->u.control_video_coding_khr.coding_control_info);

   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_control_video_coding_khr(struct vk_cmd_queue *queue
, const VkVideoCodingControlInfoKHR* pCodingControlInfo
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_CONTROL_VIDEO_CODING_KHR;
      
   if (pCodingControlInfo) {
      cmd->u.control_video_coding_khr.coding_control_info = vk_zalloc(queue->alloc, sizeof(VkVideoCodingControlInfoKHR), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
      if (cmd->u.control_video_coding_khr.coding_control_info == NULL) goto err;

      memcpy((void*)cmd->u.control_video_coding_khr.coding_control_info, pCodingControlInfo, sizeof(VkVideoCodingControlInfoKHR));
   VkVideoCodingControlInfoKHR *tmp_dst1 = (void *) cmd->u.control_video_coding_khr.coding_control_info; (void) tmp_dst1;
   VkVideoCodingControlInfoKHR *tmp_src1 = (void *) pCodingControlInfo; (void) tmp_src1;   
   
      const VkBaseInStructure *pnext = tmp_dst1->pNext;
      if (pnext) {
         switch ((int32_t)pnext->sType) {
         
      case VK_STRUCTURE_TYPE_VIDEO_ENCODE_RATE_CONTROL_INFO_KHR:
         if (pnext) {
      tmp_dst1->pNext = vk_zalloc(queue->alloc, sizeof(VkVideoEncodeRateControlInfoKHR), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
      if (tmp_dst1->pNext == NULL) goto err;

      memcpy((void*)tmp_dst1->pNext, pnext, sizeof(VkVideoEncodeRateControlInfoKHR));
   VkVideoEncodeRateControlInfoKHR *tmp_dst2 = (void *) tmp_dst1->pNext; (void) tmp_dst2;
   VkVideoEncodeRateControlInfoKHR *tmp_src2 = (void *) pnext; (void) tmp_src2;   
   if (tmp_src2->pLayerConfigs) {
   tmp_dst2->pLayerConfigs = vk_zalloc(queue->alloc, sizeof(*tmp_dst2->pLayerConfigs) * tmp_dst2->layerCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (tmp_dst2->pLayerConfigs == NULL) goto err;

   memcpy(( VkVideoEncodeRateControlLayerInfoKHR* )tmp_dst2->pLayerConfigs, tmp_src2->pLayerConfigs, sizeof(*tmp_dst2->pLayerConfigs) * tmp_dst2->layerCount);
}
   } else {
      tmp_dst1->pNext = NULL;
   }
         break;
      
      case VK_STRUCTURE_TYPE_VIDEO_ENCODE_RATE_CONTROL_LAYER_INFO_KHR:
         if (pnext) {
      tmp_dst1->pNext = vk_zalloc(queue->alloc, sizeof(VkVideoEncodeRateControlLayerInfoKHR), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
      if (tmp_dst1->pNext == NULL) goto err;

      memcpy((void*)tmp_dst1->pNext, pnext, sizeof(VkVideoEncodeRateControlLayerInfoKHR));
   VkVideoEncodeRateControlLayerInfoKHR *tmp_dst2 = (void *) tmp_dst1->pNext; (void) tmp_dst2;
   VkVideoEncodeRateControlLayerInfoKHR *tmp_src2 = (void *) pnext; (void) tmp_src2;   
   
      const VkBaseInStructure *pnext = tmp_dst2->pNext;
      if (pnext) {
         switch ((int32_t)pnext->sType) {
         
      case VK_STRUCTURE_TYPE_VIDEO_ENCODE_H264_RATE_CONTROL_LAYER_INFO_EXT:
         if (pnext) {
      tmp_dst2->pNext = vk_zalloc(queue->alloc, sizeof(VkVideoEncodeH264RateControlLayerInfoEXT), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
      if (tmp_dst2->pNext == NULL) goto err;

      memcpy((void*)tmp_dst2->pNext, pnext, sizeof(VkVideoEncodeH264RateControlLayerInfoEXT));
   VkVideoEncodeH264RateControlLayerInfoEXT *tmp_dst3 = (void *) tmp_dst2->pNext; (void) tmp_dst3;
   VkVideoEncodeH264RateControlLayerInfoEXT *tmp_src3 = (void *) pnext; (void) tmp_src3;   
      } else {
      tmp_dst2->pNext = NULL;
   }
         break;
      
      case VK_STRUCTURE_TYPE_VIDEO_ENCODE_H265_RATE_CONTROL_LAYER_INFO_EXT:
         if (pnext) {
      tmp_dst2->pNext = vk_zalloc(queue->alloc, sizeof(VkVideoEncodeH265RateControlLayerInfoEXT), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
      if (tmp_dst2->pNext == NULL) goto err;

      memcpy((void*)tmp_dst2->pNext, pnext, sizeof(VkVideoEncodeH265RateControlLayerInfoEXT));
   VkVideoEncodeH265RateControlLayerInfoEXT *tmp_dst3 = (void *) tmp_dst2->pNext; (void) tmp_dst3;
   VkVideoEncodeH265RateControlLayerInfoEXT *tmp_src3 = (void *) pnext; (void) tmp_src3;   
      } else {
      tmp_dst2->pNext = NULL;
   }
         break;
      
         }
      }
         } else {
      tmp_dst1->pNext = NULL;
   }
         break;
      
      case VK_STRUCTURE_TYPE_VIDEO_ENCODE_H264_RATE_CONTROL_INFO_EXT:
         if (pnext) {
      tmp_dst1->pNext = vk_zalloc(queue->alloc, sizeof(VkVideoEncodeH264RateControlInfoEXT), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
      if (tmp_dst1->pNext == NULL) goto err;

      memcpy((void*)tmp_dst1->pNext, pnext, sizeof(VkVideoEncodeH264RateControlInfoEXT));
   VkVideoEncodeH264RateControlInfoEXT *tmp_dst2 = (void *) tmp_dst1->pNext; (void) tmp_dst2;
   VkVideoEncodeH264RateControlInfoEXT *tmp_src2 = (void *) pnext; (void) tmp_src2;   
      } else {
      tmp_dst1->pNext = NULL;
   }
         break;
      
      case VK_STRUCTURE_TYPE_VIDEO_ENCODE_H264_RATE_CONTROL_LAYER_INFO_EXT:
         if (pnext) {
      tmp_dst1->pNext = vk_zalloc(queue->alloc, sizeof(VkVideoEncodeH264RateControlLayerInfoEXT), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
      if (tmp_dst1->pNext == NULL) goto err;

      memcpy((void*)tmp_dst1->pNext, pnext, sizeof(VkVideoEncodeH264RateControlLayerInfoEXT));
   VkVideoEncodeH264RateControlLayerInfoEXT *tmp_dst2 = (void *) tmp_dst1->pNext; (void) tmp_dst2;
   VkVideoEncodeH264RateControlLayerInfoEXT *tmp_src2 = (void *) pnext; (void) tmp_src2;   
      } else {
      tmp_dst1->pNext = NULL;
   }
         break;
      
      case VK_STRUCTURE_TYPE_VIDEO_ENCODE_H265_RATE_CONTROL_INFO_EXT:
         if (pnext) {
      tmp_dst1->pNext = vk_zalloc(queue->alloc, sizeof(VkVideoEncodeH265RateControlInfoEXT), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
      if (tmp_dst1->pNext == NULL) goto err;

      memcpy((void*)tmp_dst1->pNext, pnext, sizeof(VkVideoEncodeH265RateControlInfoEXT));
   VkVideoEncodeH265RateControlInfoEXT *tmp_dst2 = (void *) tmp_dst1->pNext; (void) tmp_dst2;
   VkVideoEncodeH265RateControlInfoEXT *tmp_src2 = (void *) pnext; (void) tmp_src2;   
      } else {
      tmp_dst1->pNext = NULL;
   }
         break;
      
      case VK_STRUCTURE_TYPE_VIDEO_ENCODE_H265_RATE_CONTROL_LAYER_INFO_EXT:
         if (pnext) {
      tmp_dst1->pNext = vk_zalloc(queue->alloc, sizeof(VkVideoEncodeH265RateControlLayerInfoEXT), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
      if (tmp_dst1->pNext == NULL) goto err;

      memcpy((void*)tmp_dst1->pNext, pnext, sizeof(VkVideoEncodeH265RateControlLayerInfoEXT));
   VkVideoEncodeH265RateControlLayerInfoEXT *tmp_dst2 = (void *) tmp_dst1->pNext; (void) tmp_dst2;
   VkVideoEncodeH265RateControlLayerInfoEXT *tmp_src2 = (void *) pnext; (void) tmp_src2;   
      } else {
      tmp_dst1->pNext = NULL;
   }
         break;
      
         }
      }
         } else {
      cmd->u.control_video_coding_khr.coding_control_info = NULL;
   }   

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

err:
   if (cmd)
      vk_free_cmd_control_video_coding_khr(queue, cmd);
   return VK_ERROR_OUT_OF_HOST_MEMORY;
}
#endif // VK_ENABLE_BETA_EXTENSIONS

#ifdef VK_ENABLE_BETA_EXTENSIONS
static void
vk_free_cmd_end_video_coding_khr(struct vk_cmd_queue *queue,
                                 struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
         vk_free(queue->alloc, ( VkVideoEndCodingInfoKHR* )cmd->u.end_video_coding_khr.end_coding_info);

   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_end_video_coding_khr(struct vk_cmd_queue *queue
, const VkVideoEndCodingInfoKHR* pEndCodingInfo
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_END_VIDEO_CODING_KHR;
      
   if (pEndCodingInfo) {
      cmd->u.end_video_coding_khr.end_coding_info = vk_zalloc(queue->alloc, sizeof(VkVideoEndCodingInfoKHR), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
      if (cmd->u.end_video_coding_khr.end_coding_info == NULL) goto err;

      memcpy((void*)cmd->u.end_video_coding_khr.end_coding_info, pEndCodingInfo, sizeof(VkVideoEndCodingInfoKHR));
   VkVideoEndCodingInfoKHR *tmp_dst1 = (void *) cmd->u.end_video_coding_khr.end_coding_info; (void) tmp_dst1;
   VkVideoEndCodingInfoKHR *tmp_src1 = (void *) pEndCodingInfo; (void) tmp_src1;   
      } else {
      cmd->u.end_video_coding_khr.end_coding_info = NULL;
   }   

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

err:
   if (cmd)
      vk_free_cmd_end_video_coding_khr(queue, cmd);
   return VK_ERROR_OUT_OF_HOST_MEMORY;
}
#endif // VK_ENABLE_BETA_EXTENSIONS

#ifdef VK_ENABLE_BETA_EXTENSIONS
static void
vk_free_cmd_encode_video_khr(struct vk_cmd_queue *queue,
                             struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, ( VkVideoReferenceSlotInfoKHR* )cmd->u.encode_video_khr.encode_info->pSetupReferenceSlot);
vk_free(queue->alloc, ( VkVideoReferenceSlotInfoKHR* )cmd->u.encode_video_khr.encode_info->pReferenceSlots);
      vk_free(queue->alloc, ( VkVideoEncodeInfoKHR* )cmd->u.encode_video_khr.encode_info);

   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_encode_video_khr(struct vk_cmd_queue *queue
, const VkVideoEncodeInfoKHR* pEncodeInfo
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_ENCODE_VIDEO_KHR;
      
   if (pEncodeInfo) {
      cmd->u.encode_video_khr.encode_info = vk_zalloc(queue->alloc, sizeof(VkVideoEncodeInfoKHR), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
      if (cmd->u.encode_video_khr.encode_info == NULL) goto err;

      memcpy((void*)cmd->u.encode_video_khr.encode_info, pEncodeInfo, sizeof(VkVideoEncodeInfoKHR));
   VkVideoEncodeInfoKHR *tmp_dst1 = (void *) cmd->u.encode_video_khr.encode_info; (void) tmp_dst1;
   VkVideoEncodeInfoKHR *tmp_src1 = (void *) pEncodeInfo; (void) tmp_src1;   
   
      const VkBaseInStructure *pnext = tmp_dst1->pNext;
      if (pnext) {
         switch ((int32_t)pnext->sType) {
         
      case VK_STRUCTURE_TYPE_VIDEO_ENCODE_H264_VCL_FRAME_INFO_EXT:
         if (pnext) {
      tmp_dst1->pNext = vk_zalloc(queue->alloc, sizeof(VkVideoEncodeH264VclFrameInfoEXT), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
      if (tmp_dst1->pNext == NULL) goto err;

      memcpy((void*)tmp_dst1->pNext, pnext, sizeof(VkVideoEncodeH264VclFrameInfoEXT));
   VkVideoEncodeH264VclFrameInfoEXT *tmp_dst2 = (void *) tmp_dst1->pNext; (void) tmp_dst2;
   VkVideoEncodeH264VclFrameInfoEXT *tmp_src2 = (void *) pnext; (void) tmp_src2;   
   if (tmp_src2->pReferenceFinalLists) {
   tmp_dst2->pReferenceFinalLists = vk_zalloc(queue->alloc, sizeof(*tmp_dst2->pReferenceFinalLists), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (tmp_dst2->pReferenceFinalLists == NULL) goto err;

   memcpy(( VkVideoEncodeH264ReferenceListsInfoEXT*      )tmp_dst2->pReferenceFinalLists, tmp_src2->pReferenceFinalLists, sizeof(*tmp_dst2->pReferenceFinalLists));
}
if (tmp_src2->pNaluSliceEntries) {
   tmp_dst2->pNaluSliceEntries = vk_zalloc(queue->alloc, sizeof(*tmp_dst2->pNaluSliceEntries) * tmp_dst2->naluSliceEntryCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (tmp_dst2->pNaluSliceEntries == NULL) goto err;

   memcpy(( VkVideoEncodeH264NaluSliceInfoEXT* )tmp_dst2->pNaluSliceEntries, tmp_src2->pNaluSliceEntries, sizeof(*tmp_dst2->pNaluSliceEntries) * tmp_dst2->naluSliceEntryCount);
}
if (tmp_src2->pCurrentPictureInfo) {
   tmp_dst2->pCurrentPictureInfo = vk_zalloc(queue->alloc, sizeof(*tmp_dst2->pCurrentPictureInfo), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (tmp_dst2->pCurrentPictureInfo == NULL) goto err;

   memcpy(( StdVideoEncodeH264PictureInfo*                               )tmp_dst2->pCurrentPictureInfo, tmp_src2->pCurrentPictureInfo, sizeof(*tmp_dst2->pCurrentPictureInfo));
}
   } else {
      tmp_dst1->pNext = NULL;
   }
         break;
      
      case VK_STRUCTURE_TYPE_VIDEO_ENCODE_H264_EMIT_PICTURE_PARAMETERS_INFO_EXT:
         if (pnext) {
      tmp_dst1->pNext = vk_zalloc(queue->alloc, sizeof(VkVideoEncodeH264EmitPictureParametersInfoEXT), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
      if (tmp_dst1->pNext == NULL) goto err;

      memcpy((void*)tmp_dst1->pNext, pnext, sizeof(VkVideoEncodeH264EmitPictureParametersInfoEXT));
   VkVideoEncodeH264EmitPictureParametersInfoEXT *tmp_dst2 = (void *) tmp_dst1->pNext; (void) tmp_dst2;
   VkVideoEncodeH264EmitPictureParametersInfoEXT *tmp_src2 = (void *) pnext; (void) tmp_src2;   
   if (tmp_src2->ppsIdEntries) {
   tmp_dst2->ppsIdEntries = vk_zalloc(queue->alloc, sizeof(*tmp_dst2->ppsIdEntries) * tmp_dst2->ppsIdEntryCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (tmp_dst2->ppsIdEntries == NULL) goto err;

   memcpy(( uint8_t*    )tmp_dst2->ppsIdEntries, tmp_src2->ppsIdEntries, sizeof(*tmp_dst2->ppsIdEntries) * tmp_dst2->ppsIdEntryCount);
}
   } else {
      tmp_dst1->pNext = NULL;
   }
         break;
      
      case VK_STRUCTURE_TYPE_VIDEO_ENCODE_H265_VCL_FRAME_INFO_EXT:
         if (pnext) {
      tmp_dst1->pNext = vk_zalloc(queue->alloc, sizeof(VkVideoEncodeH265VclFrameInfoEXT), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
      if (tmp_dst1->pNext == NULL) goto err;

      memcpy((void*)tmp_dst1->pNext, pnext, sizeof(VkVideoEncodeH265VclFrameInfoEXT));
   VkVideoEncodeH265VclFrameInfoEXT *tmp_dst2 = (void *) tmp_dst1->pNext; (void) tmp_dst2;
   VkVideoEncodeH265VclFrameInfoEXT *tmp_src2 = (void *) pnext; (void) tmp_src2;   
   if (tmp_src2->pReferenceFinalLists) {
   tmp_dst2->pReferenceFinalLists = vk_zalloc(queue->alloc, sizeof(*tmp_dst2->pReferenceFinalLists), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (tmp_dst2->pReferenceFinalLists == NULL) goto err;

   memcpy(( VkVideoEncodeH265ReferenceListsInfoEXT*     )tmp_dst2->pReferenceFinalLists, tmp_src2->pReferenceFinalLists, sizeof(*tmp_dst2->pReferenceFinalLists));
}
if (tmp_src2->pNaluSliceSegmentEntries) {
   tmp_dst2->pNaluSliceSegmentEntries = vk_zalloc(queue->alloc, sizeof(*tmp_dst2->pNaluSliceSegmentEntries) * tmp_dst2->naluSliceSegmentEntryCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (tmp_dst2->pNaluSliceSegmentEntries == NULL) goto err;

   memcpy(( VkVideoEncodeH265NaluSliceSegmentInfoEXT* )tmp_dst2->pNaluSliceSegmentEntries, tmp_src2->pNaluSliceSegmentEntries, sizeof(*tmp_dst2->pNaluSliceSegmentEntries) * tmp_dst2->naluSliceSegmentEntryCount);
}
if (tmp_src2->pCurrentPictureInfo) {
   tmp_dst2->pCurrentPictureInfo = vk_zalloc(queue->alloc, sizeof(*tmp_dst2->pCurrentPictureInfo), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (tmp_dst2->pCurrentPictureInfo == NULL) goto err;

   memcpy(( StdVideoEncodeH265PictureInfo*                              )tmp_dst2->pCurrentPictureInfo, tmp_src2->pCurrentPictureInfo, sizeof(*tmp_dst2->pCurrentPictureInfo));
}
   } else {
      tmp_dst1->pNext = NULL;
   }
         break;
      
      case VK_STRUCTURE_TYPE_VIDEO_ENCODE_H265_EMIT_PICTURE_PARAMETERS_INFO_EXT:
         if (pnext) {
      tmp_dst1->pNext = vk_zalloc(queue->alloc, sizeof(VkVideoEncodeH265EmitPictureParametersInfoEXT), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
      if (tmp_dst1->pNext == NULL) goto err;

      memcpy((void*)tmp_dst1->pNext, pnext, sizeof(VkVideoEncodeH265EmitPictureParametersInfoEXT));
   VkVideoEncodeH265EmitPictureParametersInfoEXT *tmp_dst2 = (void *) tmp_dst1->pNext; (void) tmp_dst2;
   VkVideoEncodeH265EmitPictureParametersInfoEXT *tmp_src2 = (void *) pnext; (void) tmp_src2;   
   if (tmp_src2->ppsIdEntries) {
   tmp_dst2->ppsIdEntries = vk_zalloc(queue->alloc, sizeof(*tmp_dst2->ppsIdEntries) * tmp_dst2->ppsIdEntryCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (tmp_dst2->ppsIdEntries == NULL) goto err;

   memcpy(( uint8_t*    )tmp_dst2->ppsIdEntries, tmp_src2->ppsIdEntries, sizeof(*tmp_dst2->ppsIdEntries) * tmp_dst2->ppsIdEntryCount);
}
   } else {
      tmp_dst1->pNext = NULL;
   }
         break;
      
         }
      }
      if (tmp_src1->pSetupReferenceSlot) {
   tmp_dst1->pSetupReferenceSlot = vk_zalloc(queue->alloc, sizeof(*tmp_dst1->pSetupReferenceSlot), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (tmp_dst1->pSetupReferenceSlot == NULL) goto err;

   memcpy(( VkVideoReferenceSlotInfoKHR* )tmp_dst1->pSetupReferenceSlot, tmp_src1->pSetupReferenceSlot, sizeof(*tmp_dst1->pSetupReferenceSlot));
}
if (tmp_src1->pReferenceSlots) {
   tmp_dst1->pReferenceSlots = vk_zalloc(queue->alloc, sizeof(*tmp_dst1->pReferenceSlots) * tmp_dst1->referenceSlotCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (tmp_dst1->pReferenceSlots == NULL) goto err;

   memcpy(( VkVideoReferenceSlotInfoKHR* )tmp_dst1->pReferenceSlots, tmp_src1->pReferenceSlots, sizeof(*tmp_dst1->pReferenceSlots) * tmp_dst1->referenceSlotCount);
}
   } else {
      cmd->u.encode_video_khr.encode_info = NULL;
   }   

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

err:
   if (cmd)
      vk_free_cmd_encode_video_khr(queue, cmd);
   return VK_ERROR_OUT_OF_HOST_MEMORY;
}
#endif // VK_ENABLE_BETA_EXTENSIONS

static void
vk_free_cmd_cu_launch_kernel_nvx(struct vk_cmd_queue *queue,
                                 struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, ( void*  *    )cmd->u.cu_launch_kernel_nvx.launch_info->pParams);
vk_free(queue->alloc, ( void*  *    )cmd->u.cu_launch_kernel_nvx.launch_info->pExtras);
      vk_free(queue->alloc, ( VkCuLaunchInfoNVX* )cmd->u.cu_launch_kernel_nvx.launch_info);

   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_cu_launch_kernel_nvx(struct vk_cmd_queue *queue
, const VkCuLaunchInfoNVX* pLaunchInfo
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_CU_LAUNCH_KERNEL_NVX;
      
   if (pLaunchInfo) {
      cmd->u.cu_launch_kernel_nvx.launch_info = vk_zalloc(queue->alloc, sizeof(VkCuLaunchInfoNVX), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
      if (cmd->u.cu_launch_kernel_nvx.launch_info == NULL) goto err;

      memcpy((void*)cmd->u.cu_launch_kernel_nvx.launch_info, pLaunchInfo, sizeof(VkCuLaunchInfoNVX));
   VkCuLaunchInfoNVX *tmp_dst1 = (void *) cmd->u.cu_launch_kernel_nvx.launch_info; (void) tmp_dst1;
   VkCuLaunchInfoNVX *tmp_src1 = (void *) pLaunchInfo; (void) tmp_src1;   
   if (tmp_src1->pParams) {
   tmp_dst1->pParams = vk_zalloc(queue->alloc, sizeof(*tmp_dst1->pParams) * tmp_dst1->paramCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (tmp_dst1->pParams == NULL) goto err;

   memcpy(( void*  *    )tmp_dst1->pParams, tmp_src1->pParams, sizeof(*tmp_dst1->pParams) * tmp_dst1->paramCount);
}
if (tmp_src1->pExtras) {
   tmp_dst1->pExtras = vk_zalloc(queue->alloc, sizeof(*tmp_dst1->pExtras) * tmp_dst1->extraCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (tmp_dst1->pExtras == NULL) goto err;

   memcpy(( void*  *    )tmp_dst1->pExtras, tmp_src1->pExtras, sizeof(*tmp_dst1->pExtras) * tmp_dst1->extraCount);
}
   } else {
      cmd->u.cu_launch_kernel_nvx.launch_info = NULL;
   }   

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

err:
   if (cmd)
      vk_free_cmd_cu_launch_kernel_nvx(queue, cmd);
   return VK_ERROR_OUT_OF_HOST_MEMORY;
}

static void
vk_free_cmd_begin_rendering(struct vk_cmd_queue *queue,
                            struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, ( VkRenderingAttachmentInfo*                )cmd->u.begin_rendering.rendering_info->pColorAttachments);
vk_free(queue->alloc, ( VkRenderingAttachmentInfo*                           )cmd->u.begin_rendering.rendering_info->pDepthAttachment);
vk_free(queue->alloc, ( VkRenderingAttachmentInfo*                           )cmd->u.begin_rendering.rendering_info->pStencilAttachment);
      vk_free(queue->alloc, ( VkRenderingInfo*                              )cmd->u.begin_rendering.rendering_info);

   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_begin_rendering(struct vk_cmd_queue *queue
, const VkRenderingInfo*                              pRenderingInfo
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_BEGIN_RENDERING;
      
   if (pRenderingInfo) {
      cmd->u.begin_rendering.rendering_info = vk_zalloc(queue->alloc, sizeof(VkRenderingInfo), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
      if (cmd->u.begin_rendering.rendering_info == NULL) goto err;

      memcpy((void*)cmd->u.begin_rendering.rendering_info, pRenderingInfo, sizeof(VkRenderingInfo));
   VkRenderingInfo *tmp_dst1 = (void *) cmd->u.begin_rendering.rendering_info; (void) tmp_dst1;
   VkRenderingInfo *tmp_src1 = (void *) pRenderingInfo; (void) tmp_src1;   
   
      const VkBaseInStructure *pnext = tmp_dst1->pNext;
      if (pnext) {
         switch ((int32_t)pnext->sType) {
         
      case VK_STRUCTURE_TYPE_DEVICE_GROUP_RENDER_PASS_BEGIN_INFO:
         if (pnext) {
      tmp_dst1->pNext = vk_zalloc(queue->alloc, sizeof(VkDeviceGroupRenderPassBeginInfo), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
      if (tmp_dst1->pNext == NULL) goto err;

      memcpy((void*)tmp_dst1->pNext, pnext, sizeof(VkDeviceGroupRenderPassBeginInfo));
   VkDeviceGroupRenderPassBeginInfo *tmp_dst2 = (void *) tmp_dst1->pNext; (void) tmp_dst2;
   VkDeviceGroupRenderPassBeginInfo *tmp_src2 = (void *) pnext; (void) tmp_src2;   
   if (tmp_src2->pDeviceRenderAreas) {
   tmp_dst2->pDeviceRenderAreas = vk_zalloc(queue->alloc, sizeof(*tmp_dst2->pDeviceRenderAreas) * tmp_dst2->deviceRenderAreaCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (tmp_dst2->pDeviceRenderAreas == NULL) goto err;

   memcpy(( VkRect2D*  )tmp_dst2->pDeviceRenderAreas, tmp_src2->pDeviceRenderAreas, sizeof(*tmp_dst2->pDeviceRenderAreas) * tmp_dst2->deviceRenderAreaCount);
}
   } else {
      tmp_dst1->pNext = NULL;
   }
         break;
      
      case VK_STRUCTURE_TYPE_MULTISAMPLED_RENDER_TO_SINGLE_SAMPLED_INFO_EXT:
         if (pnext) {
      tmp_dst1->pNext = vk_zalloc(queue->alloc, sizeof(VkMultisampledRenderToSingleSampledInfoEXT), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
      if (tmp_dst1->pNext == NULL) goto err;

      memcpy((void*)tmp_dst1->pNext, pnext, sizeof(VkMultisampledRenderToSingleSampledInfoEXT));
   VkMultisampledRenderToSingleSampledInfoEXT *tmp_dst2 = (void *) tmp_dst1->pNext; (void) tmp_dst2;
   VkMultisampledRenderToSingleSampledInfoEXT *tmp_src2 = (void *) pnext; (void) tmp_src2;   
      } else {
      tmp_dst1->pNext = NULL;
   }
         break;
      
      case VK_STRUCTURE_TYPE_RENDERING_FRAGMENT_SHADING_RATE_ATTACHMENT_INFO_KHR:
         if (pnext) {
      tmp_dst1->pNext = vk_zalloc(queue->alloc, sizeof(VkRenderingFragmentShadingRateAttachmentInfoKHR), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
      if (tmp_dst1->pNext == NULL) goto err;

      memcpy((void*)tmp_dst1->pNext, pnext, sizeof(VkRenderingFragmentShadingRateAttachmentInfoKHR));
   VkRenderingFragmentShadingRateAttachmentInfoKHR *tmp_dst2 = (void *) tmp_dst1->pNext; (void) tmp_dst2;
   VkRenderingFragmentShadingRateAttachmentInfoKHR *tmp_src2 = (void *) pnext; (void) tmp_src2;   
      } else {
      tmp_dst1->pNext = NULL;
   }
         break;
      
      case VK_STRUCTURE_TYPE_RENDERING_FRAGMENT_DENSITY_MAP_ATTACHMENT_INFO_EXT:
         if (pnext) {
      tmp_dst1->pNext = vk_zalloc(queue->alloc, sizeof(VkRenderingFragmentDensityMapAttachmentInfoEXT), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
      if (tmp_dst1->pNext == NULL) goto err;

      memcpy((void*)tmp_dst1->pNext, pnext, sizeof(VkRenderingFragmentDensityMapAttachmentInfoEXT));
   VkRenderingFragmentDensityMapAttachmentInfoEXT *tmp_dst2 = (void *) tmp_dst1->pNext; (void) tmp_dst2;
   VkRenderingFragmentDensityMapAttachmentInfoEXT *tmp_src2 = (void *) pnext; (void) tmp_src2;   
      } else {
      tmp_dst1->pNext = NULL;
   }
         break;
      
      case VK_STRUCTURE_TYPE_MULTIVIEW_PER_VIEW_ATTRIBUTES_INFO_NVX:
         if (pnext) {
      tmp_dst1->pNext = vk_zalloc(queue->alloc, sizeof(VkMultiviewPerViewAttributesInfoNVX), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
      if (tmp_dst1->pNext == NULL) goto err;

      memcpy((void*)tmp_dst1->pNext, pnext, sizeof(VkMultiviewPerViewAttributesInfoNVX));
   VkMultiviewPerViewAttributesInfoNVX *tmp_dst2 = (void *) tmp_dst1->pNext; (void) tmp_dst2;
   VkMultiviewPerViewAttributesInfoNVX *tmp_src2 = (void *) pnext; (void) tmp_src2;   
      } else {
      tmp_dst1->pNext = NULL;
   }
         break;
      
         }
      }
      if (tmp_src1->pColorAttachments) {
   tmp_dst1->pColorAttachments = vk_zalloc(queue->alloc, sizeof(*tmp_dst1->pColorAttachments) * tmp_dst1->colorAttachmentCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (tmp_dst1->pColorAttachments == NULL) goto err;

   memcpy(( VkRenderingAttachmentInfo*                )tmp_dst1->pColorAttachments, tmp_src1->pColorAttachments, sizeof(*tmp_dst1->pColorAttachments) * tmp_dst1->colorAttachmentCount);
}
if (tmp_src1->pDepthAttachment) {
   tmp_dst1->pDepthAttachment = vk_zalloc(queue->alloc, sizeof(*tmp_dst1->pDepthAttachment), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (tmp_dst1->pDepthAttachment == NULL) goto err;

   memcpy(( VkRenderingAttachmentInfo*                           )tmp_dst1->pDepthAttachment, tmp_src1->pDepthAttachment, sizeof(*tmp_dst1->pDepthAttachment));
}
if (tmp_src1->pStencilAttachment) {
   tmp_dst1->pStencilAttachment = vk_zalloc(queue->alloc, sizeof(*tmp_dst1->pStencilAttachment), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (tmp_dst1->pStencilAttachment == NULL) goto err;

   memcpy(( VkRenderingAttachmentInfo*                           )tmp_dst1->pStencilAttachment, tmp_src1->pStencilAttachment, sizeof(*tmp_dst1->pStencilAttachment));
}
   } else {
      cmd->u.begin_rendering.rendering_info = NULL;
   }   

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

err:
   if (cmd)
      vk_free_cmd_begin_rendering(queue, cmd);
   return VK_ERROR_OUT_OF_HOST_MEMORY;
}

static void
vk_free_cmd_end_rendering(struct vk_cmd_queue *queue,
                          struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_end_rendering(struct vk_cmd_queue *queue
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_END_RENDERING;
      

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

}

static void
vk_free_cmd_build_micromaps_ext(struct vk_cmd_queue *queue,
                                struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, ( VkMicromapBuildInfoEXT* )cmd->u.build_micromaps_ext.infos);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_build_micromaps_ext(struct vk_cmd_queue *queue
, uint32_t infoCount
, const VkMicromapBuildInfoEXT* pInfos
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_BUILD_MICROMAPS_EXT;
      
   cmd->u.build_micromaps_ext.info_count = infoCount;
   if (pInfos) {
      cmd->u.build_micromaps_ext.infos = vk_zalloc(queue->alloc, sizeof(*cmd->u.build_micromaps_ext.infos) * (infoCount), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (cmd->u.build_micromaps_ext.infos == NULL) goto err;

   memcpy(( VkMicromapBuildInfoEXT* )cmd->u.build_micromaps_ext.infos, pInfos, sizeof(*cmd->u.build_micromaps_ext.infos) * (infoCount));
   }   

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

err:
   if (cmd)
      vk_free_cmd_build_micromaps_ext(queue, cmd);
   return VK_ERROR_OUT_OF_HOST_MEMORY;
}

static void
vk_free_cmd_copy_micromap_ext(struct vk_cmd_queue *queue,
                              struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
         vk_free(queue->alloc, ( VkCopyMicromapInfoEXT* )cmd->u.copy_micromap_ext.info);

   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_copy_micromap_ext(struct vk_cmd_queue *queue
, const VkCopyMicromapInfoEXT* pInfo
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_COPY_MICROMAP_EXT;
      
   if (pInfo) {
      cmd->u.copy_micromap_ext.info = vk_zalloc(queue->alloc, sizeof(VkCopyMicromapInfoEXT), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
      if (cmd->u.copy_micromap_ext.info == NULL) goto err;

      memcpy((void*)cmd->u.copy_micromap_ext.info, pInfo, sizeof(VkCopyMicromapInfoEXT));
   VkCopyMicromapInfoEXT *tmp_dst1 = (void *) cmd->u.copy_micromap_ext.info; (void) tmp_dst1;
   VkCopyMicromapInfoEXT *tmp_src1 = (void *) pInfo; (void) tmp_src1;   
      } else {
      cmd->u.copy_micromap_ext.info = NULL;
   }   

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

err:
   if (cmd)
      vk_free_cmd_copy_micromap_ext(queue, cmd);
   return VK_ERROR_OUT_OF_HOST_MEMORY;
}

static void
vk_free_cmd_copy_micromap_to_memory_ext(struct vk_cmd_queue *queue,
                                        struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
         vk_free(queue->alloc, ( VkCopyMicromapToMemoryInfoEXT* )cmd->u.copy_micromap_to_memory_ext.info);

   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_copy_micromap_to_memory_ext(struct vk_cmd_queue *queue
, const VkCopyMicromapToMemoryInfoEXT* pInfo
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_COPY_MICROMAP_TO_MEMORY_EXT;
      
   if (pInfo) {
      cmd->u.copy_micromap_to_memory_ext.info = vk_zalloc(queue->alloc, sizeof(VkCopyMicromapToMemoryInfoEXT), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
      if (cmd->u.copy_micromap_to_memory_ext.info == NULL) goto err;

      memcpy((void*)cmd->u.copy_micromap_to_memory_ext.info, pInfo, sizeof(VkCopyMicromapToMemoryInfoEXT));
   VkCopyMicromapToMemoryInfoEXT *tmp_dst1 = (void *) cmd->u.copy_micromap_to_memory_ext.info; (void) tmp_dst1;
   VkCopyMicromapToMemoryInfoEXT *tmp_src1 = (void *) pInfo; (void) tmp_src1;   
      } else {
      cmd->u.copy_micromap_to_memory_ext.info = NULL;
   }   

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

err:
   if (cmd)
      vk_free_cmd_copy_micromap_to_memory_ext(queue, cmd);
   return VK_ERROR_OUT_OF_HOST_MEMORY;
}

static void
vk_free_cmd_copy_memory_to_micromap_ext(struct vk_cmd_queue *queue,
                                        struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
         vk_free(queue->alloc, ( VkCopyMemoryToMicromapInfoEXT* )cmd->u.copy_memory_to_micromap_ext.info);

   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_copy_memory_to_micromap_ext(struct vk_cmd_queue *queue
, const VkCopyMemoryToMicromapInfoEXT* pInfo
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_COPY_MEMORY_TO_MICROMAP_EXT;
      
   if (pInfo) {
      cmd->u.copy_memory_to_micromap_ext.info = vk_zalloc(queue->alloc, sizeof(VkCopyMemoryToMicromapInfoEXT), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
      if (cmd->u.copy_memory_to_micromap_ext.info == NULL) goto err;

      memcpy((void*)cmd->u.copy_memory_to_micromap_ext.info, pInfo, sizeof(VkCopyMemoryToMicromapInfoEXT));
   VkCopyMemoryToMicromapInfoEXT *tmp_dst1 = (void *) cmd->u.copy_memory_to_micromap_ext.info; (void) tmp_dst1;
   VkCopyMemoryToMicromapInfoEXT *tmp_src1 = (void *) pInfo; (void) tmp_src1;   
      } else {
      cmd->u.copy_memory_to_micromap_ext.info = NULL;
   }   

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

err:
   if (cmd)
      vk_free_cmd_copy_memory_to_micromap_ext(queue, cmd);
   return VK_ERROR_OUT_OF_HOST_MEMORY;
}

static void
vk_free_cmd_write_micromaps_properties_ext(struct vk_cmd_queue *queue,
                                           struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, ( VkMicromapEXT* )cmd->u.write_micromaps_properties_ext.micromaps);
   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_write_micromaps_properties_ext(struct vk_cmd_queue *queue
, uint32_t micromapCount
, const VkMicromapEXT* pMicromaps
, VkQueryType queryType
, VkQueryPool queryPool
, uint32_t firstQuery
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_WRITE_MICROMAPS_PROPERTIES_EXT;
      
   cmd->u.write_micromaps_properties_ext.micromap_count = micromapCount;
   if (pMicromaps) {
      cmd->u.write_micromaps_properties_ext.micromaps = vk_zalloc(queue->alloc, sizeof(*cmd->u.write_micromaps_properties_ext.micromaps) * (micromapCount), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (cmd->u.write_micromaps_properties_ext.micromaps == NULL) goto err;

   memcpy(( VkMicromapEXT* )cmd->u.write_micromaps_properties_ext.micromaps, pMicromaps, sizeof(*cmd->u.write_micromaps_properties_ext.micromaps) * (micromapCount));
   }   
   cmd->u.write_micromaps_properties_ext.query_type = queryType;
   cmd->u.write_micromaps_properties_ext.query_pool = queryPool;
   cmd->u.write_micromaps_properties_ext.first_query = firstQuery;

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

err:
   if (cmd)
      vk_free_cmd_write_micromaps_properties_ext(queue, cmd);
   return VK_ERROR_OUT_OF_HOST_MEMORY;
}

static void
vk_free_cmd_optical_flow_execute_nv(struct vk_cmd_queue *queue,
                                    struct vk_cmd_queue_entry *cmd)
{
   if (cmd->driver_free_cb)
      cmd->driver_free_cb(queue, cmd);
   else
      vk_free(queue->alloc, cmd->driver_data);
   vk_free(queue->alloc, ( VkRect2D*                  )cmd->u.optical_flow_execute_nv.execute_info->pRegions);
      vk_free(queue->alloc, ( VkOpticalFlowExecuteInfoNV* )cmd->u.optical_flow_execute_nv.execute_info);

   vk_free(queue->alloc, cmd);
}

VkResult vk_enqueue_cmd_optical_flow_execute_nv(struct vk_cmd_queue *queue
, VkOpticalFlowSessionNV session
, const VkOpticalFlowExecuteInfoNV* pExecuteInfo
)
{
   struct vk_cmd_queue_entry *cmd = vk_zalloc(queue->alloc,
                                              sizeof(*cmd), 8,
                                              VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (!cmd) return VK_ERROR_OUT_OF_HOST_MEMORY;

   cmd->type = VK_CMD_OPTICAL_FLOW_EXECUTE_NV;
      
   cmd->u.optical_flow_execute_nv.session = session;
   if (pExecuteInfo) {
      cmd->u.optical_flow_execute_nv.execute_info = vk_zalloc(queue->alloc, sizeof(VkOpticalFlowExecuteInfoNV), 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
      if (cmd->u.optical_flow_execute_nv.execute_info == NULL) goto err;

      memcpy((void*)cmd->u.optical_flow_execute_nv.execute_info, pExecuteInfo, sizeof(VkOpticalFlowExecuteInfoNV));
   VkOpticalFlowExecuteInfoNV *tmp_dst1 = (void *) cmd->u.optical_flow_execute_nv.execute_info; (void) tmp_dst1;
   VkOpticalFlowExecuteInfoNV *tmp_src1 = (void *) pExecuteInfo; (void) tmp_src1;   
   if (tmp_src1->pRegions) {
   tmp_dst1->pRegions = vk_zalloc(queue->alloc, sizeof(*tmp_dst1->pRegions) * tmp_dst1->regionCount, 8, VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
   if (tmp_dst1->pRegions == NULL) goto err;

   memcpy(( VkRect2D*                  )tmp_dst1->pRegions, tmp_src1->pRegions, sizeof(*tmp_dst1->pRegions) * tmp_dst1->regionCount);
}
   } else {
      cmd->u.optical_flow_execute_nv.execute_info = NULL;
   }   

   list_addtail(&cmd->cmd_link, &queue->cmds);
   return VK_SUCCESS;

err:
   if (cmd)
      vk_free_cmd_optical_flow_execute_nv(queue, cmd);
   return VK_ERROR_OUT_OF_HOST_MEMORY;
}


void
vk_free_queue(struct vk_cmd_queue *queue)
{
   struct vk_cmd_queue_entry *tmp, *cmd;
   LIST_FOR_EACH_ENTRY_SAFE(cmd, tmp, &queue->cmds, cmd_link) {
      switch(cmd->type) {
      case VK_CMD_BIND_PIPELINE:
         vk_free_cmd_bind_pipeline(queue, cmd);
         break;
      case VK_CMD_SET_VIEWPORT:
         vk_free_cmd_set_viewport(queue, cmd);
         break;
      case VK_CMD_SET_SCISSOR:
         vk_free_cmd_set_scissor(queue, cmd);
         break;
      case VK_CMD_SET_LINE_WIDTH:
         vk_free_cmd_set_line_width(queue, cmd);
         break;
      case VK_CMD_SET_DEPTH_BIAS:
         vk_free_cmd_set_depth_bias(queue, cmd);
         break;
      case VK_CMD_SET_BLEND_CONSTANTS:
         vk_free_cmd_set_blend_constants(queue, cmd);
         break;
      case VK_CMD_SET_DEPTH_BOUNDS:
         vk_free_cmd_set_depth_bounds(queue, cmd);
         break;
      case VK_CMD_SET_STENCIL_COMPARE_MASK:
         vk_free_cmd_set_stencil_compare_mask(queue, cmd);
         break;
      case VK_CMD_SET_STENCIL_WRITE_MASK:
         vk_free_cmd_set_stencil_write_mask(queue, cmd);
         break;
      case VK_CMD_SET_STENCIL_REFERENCE:
         vk_free_cmd_set_stencil_reference(queue, cmd);
         break;
      case VK_CMD_BIND_DESCRIPTOR_SETS:
         vk_free_cmd_bind_descriptor_sets(queue, cmd);
         break;
      case VK_CMD_BIND_INDEX_BUFFER:
         vk_free_cmd_bind_index_buffer(queue, cmd);
         break;
      case VK_CMD_BIND_VERTEX_BUFFERS:
         vk_free_cmd_bind_vertex_buffers(queue, cmd);
         break;
      case VK_CMD_DRAW:
         vk_free_cmd_draw(queue, cmd);
         break;
      case VK_CMD_DRAW_INDEXED:
         vk_free_cmd_draw_indexed(queue, cmd);
         break;
      case VK_CMD_DRAW_MULTI_EXT:
         vk_free_cmd_draw_multi_ext(queue, cmd);
         break;
      case VK_CMD_DRAW_MULTI_INDEXED_EXT:
         vk_free_cmd_draw_multi_indexed_ext(queue, cmd);
         break;
      case VK_CMD_DRAW_INDIRECT:
         vk_free_cmd_draw_indirect(queue, cmd);
         break;
      case VK_CMD_DRAW_INDEXED_INDIRECT:
         vk_free_cmd_draw_indexed_indirect(queue, cmd);
         break;
      case VK_CMD_DISPATCH:
         vk_free_cmd_dispatch(queue, cmd);
         break;
      case VK_CMD_DISPATCH_INDIRECT:
         vk_free_cmd_dispatch_indirect(queue, cmd);
         break;
      case VK_CMD_SUBPASS_SHADING_HUAWEI:
         vk_free_cmd_subpass_shading_huawei(queue, cmd);
         break;
      case VK_CMD_COPY_BUFFER:
         vk_free_cmd_copy_buffer(queue, cmd);
         break;
      case VK_CMD_COPY_IMAGE:
         vk_free_cmd_copy_image(queue, cmd);
         break;
      case VK_CMD_BLIT_IMAGE:
         vk_free_cmd_blit_image(queue, cmd);
         break;
      case VK_CMD_COPY_BUFFER_TO_IMAGE:
         vk_free_cmd_copy_buffer_to_image(queue, cmd);
         break;
      case VK_CMD_COPY_IMAGE_TO_BUFFER:
         vk_free_cmd_copy_image_to_buffer(queue, cmd);
         break;
      case VK_CMD_UPDATE_BUFFER:
         vk_free_cmd_update_buffer(queue, cmd);
         break;
      case VK_CMD_FILL_BUFFER:
         vk_free_cmd_fill_buffer(queue, cmd);
         break;
      case VK_CMD_CLEAR_COLOR_IMAGE:
         vk_free_cmd_clear_color_image(queue, cmd);
         break;
      case VK_CMD_CLEAR_DEPTH_STENCIL_IMAGE:
         vk_free_cmd_clear_depth_stencil_image(queue, cmd);
         break;
      case VK_CMD_CLEAR_ATTACHMENTS:
         vk_free_cmd_clear_attachments(queue, cmd);
         break;
      case VK_CMD_RESOLVE_IMAGE:
         vk_free_cmd_resolve_image(queue, cmd);
         break;
      case VK_CMD_SET_EVENT:
         vk_free_cmd_set_event(queue, cmd);
         break;
      case VK_CMD_RESET_EVENT:
         vk_free_cmd_reset_event(queue, cmd);
         break;
      case VK_CMD_WAIT_EVENTS:
         vk_free_cmd_wait_events(queue, cmd);
         break;
      case VK_CMD_PIPELINE_BARRIER:
         vk_free_cmd_pipeline_barrier(queue, cmd);
         break;
      case VK_CMD_BEGIN_QUERY:
         vk_free_cmd_begin_query(queue, cmd);
         break;
      case VK_CMD_END_QUERY:
         vk_free_cmd_end_query(queue, cmd);
         break;
      case VK_CMD_BEGIN_CONDITIONAL_RENDERING_EXT:
         vk_free_cmd_begin_conditional_rendering_ext(queue, cmd);
         break;
      case VK_CMD_END_CONDITIONAL_RENDERING_EXT:
         vk_free_cmd_end_conditional_rendering_ext(queue, cmd);
         break;
      case VK_CMD_RESET_QUERY_POOL:
         vk_free_cmd_reset_query_pool(queue, cmd);
         break;
      case VK_CMD_WRITE_TIMESTAMP:
         vk_free_cmd_write_timestamp(queue, cmd);
         break;
      case VK_CMD_COPY_QUERY_POOL_RESULTS:
         vk_free_cmd_copy_query_pool_results(queue, cmd);
         break;
      case VK_CMD_PUSH_CONSTANTS:
         vk_free_cmd_push_constants(queue, cmd);
         break;
      case VK_CMD_BEGIN_RENDER_PASS:
         vk_free_cmd_begin_render_pass(queue, cmd);
         break;
      case VK_CMD_NEXT_SUBPASS:
         vk_free_cmd_next_subpass(queue, cmd);
         break;
      case VK_CMD_END_RENDER_PASS:
         vk_free_cmd_end_render_pass(queue, cmd);
         break;
      case VK_CMD_EXECUTE_COMMANDS:
         vk_free_cmd_execute_commands(queue, cmd);
         break;
      case VK_CMD_DEBUG_MARKER_BEGIN_EXT:
         vk_free_cmd_debug_marker_begin_ext(queue, cmd);
         break;
      case VK_CMD_DEBUG_MARKER_END_EXT:
         vk_free_cmd_debug_marker_end_ext(queue, cmd);
         break;
      case VK_CMD_DEBUG_MARKER_INSERT_EXT:
         vk_free_cmd_debug_marker_insert_ext(queue, cmd);
         break;
      case VK_CMD_EXECUTE_GENERATED_COMMANDS_NV:
         vk_free_cmd_execute_generated_commands_nv(queue, cmd);
         break;
      case VK_CMD_PREPROCESS_GENERATED_COMMANDS_NV:
         vk_free_cmd_preprocess_generated_commands_nv(queue, cmd);
         break;
      case VK_CMD_BIND_PIPELINE_SHADER_GROUP_NV:
         vk_free_cmd_bind_pipeline_shader_group_nv(queue, cmd);
         break;
      case VK_CMD_PUSH_DESCRIPTOR_SET_KHR:
         vk_free_cmd_push_descriptor_set_khr(queue, cmd);
         break;
      case VK_CMD_SET_DEVICE_MASK:
         vk_free_cmd_set_device_mask(queue, cmd);
         break;
      case VK_CMD_DISPATCH_BASE:
         vk_free_cmd_dispatch_base(queue, cmd);
         break;
      case VK_CMD_PUSH_DESCRIPTOR_SET_WITH_TEMPLATE_KHR:
         vk_free_cmd_push_descriptor_set_with_template_khr(queue, cmd);
         break;
      case VK_CMD_SET_VIEWPORT_WSCALING_NV:
         vk_free_cmd_set_viewport_wscaling_nv(queue, cmd);
         break;
      case VK_CMD_SET_DISCARD_RECTANGLE_EXT:
         vk_free_cmd_set_discard_rectangle_ext(queue, cmd);
         break;
      case VK_CMD_SET_SAMPLE_LOCATIONS_EXT:
         vk_free_cmd_set_sample_locations_ext(queue, cmd);
         break;
      case VK_CMD_BEGIN_DEBUG_UTILS_LABEL_EXT:
         vk_free_cmd_begin_debug_utils_label_ext(queue, cmd);
         break;
      case VK_CMD_END_DEBUG_UTILS_LABEL_EXT:
         vk_free_cmd_end_debug_utils_label_ext(queue, cmd);
         break;
      case VK_CMD_INSERT_DEBUG_UTILS_LABEL_EXT:
         vk_free_cmd_insert_debug_utils_label_ext(queue, cmd);
         break;
      case VK_CMD_WRITE_BUFFER_MARKER_AMD:
         vk_free_cmd_write_buffer_marker_amd(queue, cmd);
         break;
      case VK_CMD_BEGIN_RENDER_PASS2:
         vk_free_cmd_begin_render_pass2(queue, cmd);
         break;
      case VK_CMD_NEXT_SUBPASS2:
         vk_free_cmd_next_subpass2(queue, cmd);
         break;
      case VK_CMD_END_RENDER_PASS2:
         vk_free_cmd_end_render_pass2(queue, cmd);
         break;
      case VK_CMD_DRAW_INDIRECT_COUNT:
         vk_free_cmd_draw_indirect_count(queue, cmd);
         break;
      case VK_CMD_DRAW_INDEXED_INDIRECT_COUNT:
         vk_free_cmd_draw_indexed_indirect_count(queue, cmd);
         break;
      case VK_CMD_SET_CHECKPOINT_NV:
         vk_free_cmd_set_checkpoint_nv(queue, cmd);
         break;
      case VK_CMD_BIND_TRANSFORM_FEEDBACK_BUFFERS_EXT:
         vk_free_cmd_bind_transform_feedback_buffers_ext(queue, cmd);
         break;
      case VK_CMD_BEGIN_TRANSFORM_FEEDBACK_EXT:
         vk_free_cmd_begin_transform_feedback_ext(queue, cmd);
         break;
      case VK_CMD_END_TRANSFORM_FEEDBACK_EXT:
         vk_free_cmd_end_transform_feedback_ext(queue, cmd);
         break;
      case VK_CMD_BEGIN_QUERY_INDEXED_EXT:
         vk_free_cmd_begin_query_indexed_ext(queue, cmd);
         break;
      case VK_CMD_END_QUERY_INDEXED_EXT:
         vk_free_cmd_end_query_indexed_ext(queue, cmd);
         break;
      case VK_CMD_DRAW_INDIRECT_BYTE_COUNT_EXT:
         vk_free_cmd_draw_indirect_byte_count_ext(queue, cmd);
         break;
      case VK_CMD_SET_EXCLUSIVE_SCISSOR_NV:
         vk_free_cmd_set_exclusive_scissor_nv(queue, cmd);
         break;
      case VK_CMD_BIND_SHADING_RATE_IMAGE_NV:
         vk_free_cmd_bind_shading_rate_image_nv(queue, cmd);
         break;
      case VK_CMD_SET_VIEWPORT_SHADING_RATE_PALETTE_NV:
         vk_free_cmd_set_viewport_shading_rate_palette_nv(queue, cmd);
         break;
      case VK_CMD_SET_COARSE_SAMPLE_ORDER_NV:
         vk_free_cmd_set_coarse_sample_order_nv(queue, cmd);
         break;
      case VK_CMD_DRAW_MESH_TASKS_NV:
         vk_free_cmd_draw_mesh_tasks_nv(queue, cmd);
         break;
      case VK_CMD_DRAW_MESH_TASKS_INDIRECT_NV:
         vk_free_cmd_draw_mesh_tasks_indirect_nv(queue, cmd);
         break;
      case VK_CMD_DRAW_MESH_TASKS_INDIRECT_COUNT_NV:
         vk_free_cmd_draw_mesh_tasks_indirect_count_nv(queue, cmd);
         break;
      case VK_CMD_DRAW_MESH_TASKS_EXT:
         vk_free_cmd_draw_mesh_tasks_ext(queue, cmd);
         break;
      case VK_CMD_DRAW_MESH_TASKS_INDIRECT_EXT:
         vk_free_cmd_draw_mesh_tasks_indirect_ext(queue, cmd);
         break;
      case VK_CMD_DRAW_MESH_TASKS_INDIRECT_COUNT_EXT:
         vk_free_cmd_draw_mesh_tasks_indirect_count_ext(queue, cmd);
         break;
      case VK_CMD_BIND_INVOCATION_MASK_HUAWEI:
         vk_free_cmd_bind_invocation_mask_huawei(queue, cmd);
         break;
      case VK_CMD_COPY_ACCELERATION_STRUCTURE_NV:
         vk_free_cmd_copy_acceleration_structure_nv(queue, cmd);
         break;
      case VK_CMD_COPY_ACCELERATION_STRUCTURE_KHR:
         vk_free_cmd_copy_acceleration_structure_khr(queue, cmd);
         break;
      case VK_CMD_COPY_ACCELERATION_STRUCTURE_TO_MEMORY_KHR:
         vk_free_cmd_copy_acceleration_structure_to_memory_khr(queue, cmd);
         break;
      case VK_CMD_COPY_MEMORY_TO_ACCELERATION_STRUCTURE_KHR:
         vk_free_cmd_copy_memory_to_acceleration_structure_khr(queue, cmd);
         break;
      case VK_CMD_WRITE_ACCELERATION_STRUCTURES_PROPERTIES_KHR:
         vk_free_cmd_write_acceleration_structures_properties_khr(queue, cmd);
         break;
      case VK_CMD_WRITE_ACCELERATION_STRUCTURES_PROPERTIES_NV:
         vk_free_cmd_write_acceleration_structures_properties_nv(queue, cmd);
         break;
      case VK_CMD_BUILD_ACCELERATION_STRUCTURE_NV:
         vk_free_cmd_build_acceleration_structure_nv(queue, cmd);
         break;
      case VK_CMD_TRACE_RAYS_KHR:
         vk_free_cmd_trace_rays_khr(queue, cmd);
         break;
      case VK_CMD_TRACE_RAYS_NV:
         vk_free_cmd_trace_rays_nv(queue, cmd);
         break;
      case VK_CMD_TRACE_RAYS_INDIRECT_KHR:
         vk_free_cmd_trace_rays_indirect_khr(queue, cmd);
         break;
      case VK_CMD_TRACE_RAYS_INDIRECT2_KHR:
         vk_free_cmd_trace_rays_indirect2_khr(queue, cmd);
         break;
      case VK_CMD_SET_RAY_TRACING_PIPELINE_STACK_SIZE_KHR:
         vk_free_cmd_set_ray_tracing_pipeline_stack_size_khr(queue, cmd);
         break;
      case VK_CMD_SET_PERFORMANCE_MARKER_INTEL:
         vk_free_cmd_set_performance_marker_intel(queue, cmd);
         break;
      case VK_CMD_SET_PERFORMANCE_STREAM_MARKER_INTEL:
         vk_free_cmd_set_performance_stream_marker_intel(queue, cmd);
         break;
      case VK_CMD_SET_PERFORMANCE_OVERRIDE_INTEL:
         vk_free_cmd_set_performance_override_intel(queue, cmd);
         break;
      case VK_CMD_SET_LINE_STIPPLE_EXT:
         vk_free_cmd_set_line_stipple_ext(queue, cmd);
         break;
      case VK_CMD_BUILD_ACCELERATION_STRUCTURES_KHR:
         vk_free_cmd_build_acceleration_structures_khr(queue, cmd);
         break;
      case VK_CMD_BUILD_ACCELERATION_STRUCTURES_INDIRECT_KHR:
         vk_free_cmd_build_acceleration_structures_indirect_khr(queue, cmd);
         break;
      case VK_CMD_SET_CULL_MODE:
         vk_free_cmd_set_cull_mode(queue, cmd);
         break;
      case VK_CMD_SET_FRONT_FACE:
         vk_free_cmd_set_front_face(queue, cmd);
         break;
      case VK_CMD_SET_PRIMITIVE_TOPOLOGY:
         vk_free_cmd_set_primitive_topology(queue, cmd);
         break;
      case VK_CMD_SET_VIEWPORT_WITH_COUNT:
         vk_free_cmd_set_viewport_with_count(queue, cmd);
         break;
      case VK_CMD_SET_SCISSOR_WITH_COUNT:
         vk_free_cmd_set_scissor_with_count(queue, cmd);
         break;
      case VK_CMD_BIND_VERTEX_BUFFERS2:
         vk_free_cmd_bind_vertex_buffers2(queue, cmd);
         break;
      case VK_CMD_SET_DEPTH_TEST_ENABLE:
         vk_free_cmd_set_depth_test_enable(queue, cmd);
         break;
      case VK_CMD_SET_DEPTH_WRITE_ENABLE:
         vk_free_cmd_set_depth_write_enable(queue, cmd);
         break;
      case VK_CMD_SET_DEPTH_COMPARE_OP:
         vk_free_cmd_set_depth_compare_op(queue, cmd);
         break;
      case VK_CMD_SET_DEPTH_BOUNDS_TEST_ENABLE:
         vk_free_cmd_set_depth_bounds_test_enable(queue, cmd);
         break;
      case VK_CMD_SET_STENCIL_TEST_ENABLE:
         vk_free_cmd_set_stencil_test_enable(queue, cmd);
         break;
      case VK_CMD_SET_STENCIL_OP:
         vk_free_cmd_set_stencil_op(queue, cmd);
         break;
      case VK_CMD_SET_PATCH_CONTROL_POINTS_EXT:
         vk_free_cmd_set_patch_control_points_ext(queue, cmd);
         break;
      case VK_CMD_SET_RASTERIZER_DISCARD_ENABLE:
         vk_free_cmd_set_rasterizer_discard_enable(queue, cmd);
         break;
      case VK_CMD_SET_DEPTH_BIAS_ENABLE:
         vk_free_cmd_set_depth_bias_enable(queue, cmd);
         break;
      case VK_CMD_SET_LOGIC_OP_EXT:
         vk_free_cmd_set_logic_op_ext(queue, cmd);
         break;
      case VK_CMD_SET_PRIMITIVE_RESTART_ENABLE:
         vk_free_cmd_set_primitive_restart_enable(queue, cmd);
         break;
      case VK_CMD_SET_TESSELLATION_DOMAIN_ORIGIN_EXT:
         vk_free_cmd_set_tessellation_domain_origin_ext(queue, cmd);
         break;
      case VK_CMD_SET_DEPTH_CLAMP_ENABLE_EXT:
         vk_free_cmd_set_depth_clamp_enable_ext(queue, cmd);
         break;
      case VK_CMD_SET_POLYGON_MODE_EXT:
         vk_free_cmd_set_polygon_mode_ext(queue, cmd);
         break;
      case VK_CMD_SET_RASTERIZATION_SAMPLES_EXT:
         vk_free_cmd_set_rasterization_samples_ext(queue, cmd);
         break;
      case VK_CMD_SET_SAMPLE_MASK_EXT:
         vk_free_cmd_set_sample_mask_ext(queue, cmd);
         break;
      case VK_CMD_SET_ALPHA_TO_COVERAGE_ENABLE_EXT:
         vk_free_cmd_set_alpha_to_coverage_enable_ext(queue, cmd);
         break;
      case VK_CMD_SET_ALPHA_TO_ONE_ENABLE_EXT:
         vk_free_cmd_set_alpha_to_one_enable_ext(queue, cmd);
         break;
      case VK_CMD_SET_LOGIC_OP_ENABLE_EXT:
         vk_free_cmd_set_logic_op_enable_ext(queue, cmd);
         break;
      case VK_CMD_SET_COLOR_BLEND_ENABLE_EXT:
         vk_free_cmd_set_color_blend_enable_ext(queue, cmd);
         break;
      case VK_CMD_SET_COLOR_BLEND_EQUATION_EXT:
         vk_free_cmd_set_color_blend_equation_ext(queue, cmd);
         break;
      case VK_CMD_SET_COLOR_WRITE_MASK_EXT:
         vk_free_cmd_set_color_write_mask_ext(queue, cmd);
         break;
      case VK_CMD_SET_RASTERIZATION_STREAM_EXT:
         vk_free_cmd_set_rasterization_stream_ext(queue, cmd);
         break;
      case VK_CMD_SET_CONSERVATIVE_RASTERIZATION_MODE_EXT:
         vk_free_cmd_set_conservative_rasterization_mode_ext(queue, cmd);
         break;
      case VK_CMD_SET_EXTRA_PRIMITIVE_OVERESTIMATION_SIZE_EXT:
         vk_free_cmd_set_extra_primitive_overestimation_size_ext(queue, cmd);
         break;
      case VK_CMD_SET_DEPTH_CLIP_ENABLE_EXT:
         vk_free_cmd_set_depth_clip_enable_ext(queue, cmd);
         break;
      case VK_CMD_SET_SAMPLE_LOCATIONS_ENABLE_EXT:
         vk_free_cmd_set_sample_locations_enable_ext(queue, cmd);
         break;
      case VK_CMD_SET_COLOR_BLEND_ADVANCED_EXT:
         vk_free_cmd_set_color_blend_advanced_ext(queue, cmd);
         break;
      case VK_CMD_SET_PROVOKING_VERTEX_MODE_EXT:
         vk_free_cmd_set_provoking_vertex_mode_ext(queue, cmd);
         break;
      case VK_CMD_SET_LINE_RASTERIZATION_MODE_EXT:
         vk_free_cmd_set_line_rasterization_mode_ext(queue, cmd);
         break;
      case VK_CMD_SET_LINE_STIPPLE_ENABLE_EXT:
         vk_free_cmd_set_line_stipple_enable_ext(queue, cmd);
         break;
      case VK_CMD_SET_DEPTH_CLIP_NEGATIVE_ONE_TO_ONE_EXT:
         vk_free_cmd_set_depth_clip_negative_one_to_one_ext(queue, cmd);
         break;
      case VK_CMD_SET_VIEWPORT_WSCALING_ENABLE_NV:
         vk_free_cmd_set_viewport_wscaling_enable_nv(queue, cmd);
         break;
      case VK_CMD_SET_VIEWPORT_SWIZZLE_NV:
         vk_free_cmd_set_viewport_swizzle_nv(queue, cmd);
         break;
      case VK_CMD_SET_COVERAGE_TO_COLOR_ENABLE_NV:
         vk_free_cmd_set_coverage_to_color_enable_nv(queue, cmd);
         break;
      case VK_CMD_SET_COVERAGE_TO_COLOR_LOCATION_NV:
         vk_free_cmd_set_coverage_to_color_location_nv(queue, cmd);
         break;
      case VK_CMD_SET_COVERAGE_MODULATION_MODE_NV:
         vk_free_cmd_set_coverage_modulation_mode_nv(queue, cmd);
         break;
      case VK_CMD_SET_COVERAGE_MODULATION_TABLE_ENABLE_NV:
         vk_free_cmd_set_coverage_modulation_table_enable_nv(queue, cmd);
         break;
      case VK_CMD_SET_COVERAGE_MODULATION_TABLE_NV:
         vk_free_cmd_set_coverage_modulation_table_nv(queue, cmd);
         break;
      case VK_CMD_SET_SHADING_RATE_IMAGE_ENABLE_NV:
         vk_free_cmd_set_shading_rate_image_enable_nv(queue, cmd);
         break;
      case VK_CMD_SET_COVERAGE_REDUCTION_MODE_NV:
         vk_free_cmd_set_coverage_reduction_mode_nv(queue, cmd);
         break;
      case VK_CMD_SET_REPRESENTATIVE_FRAGMENT_TEST_ENABLE_NV:
         vk_free_cmd_set_representative_fragment_test_enable_nv(queue, cmd);
         break;
      case VK_CMD_COPY_BUFFER2:
         vk_free_cmd_copy_buffer2(queue, cmd);
         break;
      case VK_CMD_COPY_IMAGE2:
         vk_free_cmd_copy_image2(queue, cmd);
         break;
      case VK_CMD_BLIT_IMAGE2:
         vk_free_cmd_blit_image2(queue, cmd);
         break;
      case VK_CMD_COPY_BUFFER_TO_IMAGE2:
         vk_free_cmd_copy_buffer_to_image2(queue, cmd);
         break;
      case VK_CMD_COPY_IMAGE_TO_BUFFER2:
         vk_free_cmd_copy_image_to_buffer2(queue, cmd);
         break;
      case VK_CMD_RESOLVE_IMAGE2:
         vk_free_cmd_resolve_image2(queue, cmd);
         break;
      case VK_CMD_SET_FRAGMENT_SHADING_RATE_KHR:
         vk_free_cmd_set_fragment_shading_rate_khr(queue, cmd);
         break;
      case VK_CMD_SET_FRAGMENT_SHADING_RATE_ENUM_NV:
         vk_free_cmd_set_fragment_shading_rate_enum_nv(queue, cmd);
         break;
      case VK_CMD_SET_VERTEX_INPUT_EXT:
         vk_free_cmd_set_vertex_input_ext(queue, cmd);
         break;
      case VK_CMD_SET_COLOR_WRITE_ENABLE_EXT:
         vk_free_cmd_set_color_write_enable_ext(queue, cmd);
         break;
      case VK_CMD_SET_EVENT2:
         vk_free_cmd_set_event2(queue, cmd);
         break;
      case VK_CMD_RESET_EVENT2:
         vk_free_cmd_reset_event2(queue, cmd);
         break;
      case VK_CMD_WAIT_EVENTS2:
         vk_free_cmd_wait_events2(queue, cmd);
         break;
      case VK_CMD_PIPELINE_BARRIER2:
         vk_free_cmd_pipeline_barrier2(queue, cmd);
         break;
      case VK_CMD_WRITE_TIMESTAMP2:
         vk_free_cmd_write_timestamp2(queue, cmd);
         break;
      case VK_CMD_WRITE_BUFFER_MARKER2_AMD:
         vk_free_cmd_write_buffer_marker2_amd(queue, cmd);
         break;
#ifdef VK_ENABLE_BETA_EXTENSIONS
      case VK_CMD_DECODE_VIDEO_KHR:
         vk_free_cmd_decode_video_khr(queue, cmd);
         break;
#endif // VK_ENABLE_BETA_EXTENSIONS
#ifdef VK_ENABLE_BETA_EXTENSIONS
      case VK_CMD_BEGIN_VIDEO_CODING_KHR:
         vk_free_cmd_begin_video_coding_khr(queue, cmd);
         break;
#endif // VK_ENABLE_BETA_EXTENSIONS
#ifdef VK_ENABLE_BETA_EXTENSIONS
      case VK_CMD_CONTROL_VIDEO_CODING_KHR:
         vk_free_cmd_control_video_coding_khr(queue, cmd);
         break;
#endif // VK_ENABLE_BETA_EXTENSIONS
#ifdef VK_ENABLE_BETA_EXTENSIONS
      case VK_CMD_END_VIDEO_CODING_KHR:
         vk_free_cmd_end_video_coding_khr(queue, cmd);
         break;
#endif // VK_ENABLE_BETA_EXTENSIONS
#ifdef VK_ENABLE_BETA_EXTENSIONS
      case VK_CMD_ENCODE_VIDEO_KHR:
         vk_free_cmd_encode_video_khr(queue, cmd);
         break;
#endif // VK_ENABLE_BETA_EXTENSIONS
      case VK_CMD_CU_LAUNCH_KERNEL_NVX:
         vk_free_cmd_cu_launch_kernel_nvx(queue, cmd);
         break;
      case VK_CMD_BEGIN_RENDERING:
         vk_free_cmd_begin_rendering(queue, cmd);
         break;
      case VK_CMD_END_RENDERING:
         vk_free_cmd_end_rendering(queue, cmd);
         break;
      case VK_CMD_BUILD_MICROMAPS_EXT:
         vk_free_cmd_build_micromaps_ext(queue, cmd);
         break;
      case VK_CMD_COPY_MICROMAP_EXT:
         vk_free_cmd_copy_micromap_ext(queue, cmd);
         break;
      case VK_CMD_COPY_MICROMAP_TO_MEMORY_EXT:
         vk_free_cmd_copy_micromap_to_memory_ext(queue, cmd);
         break;
      case VK_CMD_COPY_MEMORY_TO_MICROMAP_EXT:
         vk_free_cmd_copy_memory_to_micromap_ext(queue, cmd);
         break;
      case VK_CMD_WRITE_MICROMAPS_PROPERTIES_EXT:
         vk_free_cmd_write_micromaps_properties_ext(queue, cmd);
         break;
      case VK_CMD_OPTICAL_FLOW_EXECUTE_NV:
         vk_free_cmd_optical_flow_execute_nv(queue, cmd);
         break;
      }
   }
}

void
vk_cmd_queue_execute(struct vk_cmd_queue *queue,
                     VkCommandBuffer commandBuffer,
                     const struct vk_device_dispatch_table *disp)
{
   list_for_each_entry(struct vk_cmd_queue_entry, cmd, &queue->cmds, cmd_link) {
      switch (cmd->type) {
      case VK_CMD_BIND_PIPELINE:
          disp->CmdBindPipeline(commandBuffer
             , cmd->u.bind_pipeline.pipeline_bind_point             , cmd->u.bind_pipeline.pipeline          );
          break;
      case VK_CMD_SET_VIEWPORT:
          disp->CmdSetViewport(commandBuffer
             , cmd->u.set_viewport.first_viewport             , cmd->u.set_viewport.viewport_count             , cmd->u.set_viewport.viewports          );
          break;
      case VK_CMD_SET_SCISSOR:
          disp->CmdSetScissor(commandBuffer
             , cmd->u.set_scissor.first_scissor             , cmd->u.set_scissor.scissor_count             , cmd->u.set_scissor.scissors          );
          break;
      case VK_CMD_SET_LINE_WIDTH:
          disp->CmdSetLineWidth(commandBuffer
             , cmd->u.set_line_width.line_width          );
          break;
      case VK_CMD_SET_DEPTH_BIAS:
          disp->CmdSetDepthBias(commandBuffer
             , cmd->u.set_depth_bias.depth_bias_constant_factor             , cmd->u.set_depth_bias.depth_bias_clamp             , cmd->u.set_depth_bias.depth_bias_slope_factor          );
          break;
      case VK_CMD_SET_BLEND_CONSTANTS:
          disp->CmdSetBlendConstants(commandBuffer
             , cmd->u.set_blend_constants.blend_constants          );
          break;
      case VK_CMD_SET_DEPTH_BOUNDS:
          disp->CmdSetDepthBounds(commandBuffer
             , cmd->u.set_depth_bounds.min_depth_bounds             , cmd->u.set_depth_bounds.max_depth_bounds          );
          break;
      case VK_CMD_SET_STENCIL_COMPARE_MASK:
          disp->CmdSetStencilCompareMask(commandBuffer
             , cmd->u.set_stencil_compare_mask.face_mask             , cmd->u.set_stencil_compare_mask.compare_mask          );
          break;
      case VK_CMD_SET_STENCIL_WRITE_MASK:
          disp->CmdSetStencilWriteMask(commandBuffer
             , cmd->u.set_stencil_write_mask.face_mask             , cmd->u.set_stencil_write_mask.write_mask          );
          break;
      case VK_CMD_SET_STENCIL_REFERENCE:
          disp->CmdSetStencilReference(commandBuffer
             , cmd->u.set_stencil_reference.face_mask             , cmd->u.set_stencil_reference.reference          );
          break;
      case VK_CMD_BIND_DESCRIPTOR_SETS:
          disp->CmdBindDescriptorSets(commandBuffer
             , cmd->u.bind_descriptor_sets.pipeline_bind_point             , cmd->u.bind_descriptor_sets.layout             , cmd->u.bind_descriptor_sets.first_set             , cmd->u.bind_descriptor_sets.descriptor_set_count             , cmd->u.bind_descriptor_sets.descriptor_sets             , cmd->u.bind_descriptor_sets.dynamic_offset_count             , cmd->u.bind_descriptor_sets.dynamic_offsets          );
          break;
      case VK_CMD_BIND_INDEX_BUFFER:
          disp->CmdBindIndexBuffer(commandBuffer
             , cmd->u.bind_index_buffer.buffer             , cmd->u.bind_index_buffer.offset             , cmd->u.bind_index_buffer.index_type          );
          break;
      case VK_CMD_BIND_VERTEX_BUFFERS:
          disp->CmdBindVertexBuffers(commandBuffer
             , cmd->u.bind_vertex_buffers.first_binding             , cmd->u.bind_vertex_buffers.binding_count             , cmd->u.bind_vertex_buffers.buffers             , cmd->u.bind_vertex_buffers.offsets          );
          break;
      case VK_CMD_DRAW:
          disp->CmdDraw(commandBuffer
             , cmd->u.draw.vertex_count             , cmd->u.draw.instance_count             , cmd->u.draw.first_vertex             , cmd->u.draw.first_instance          );
          break;
      case VK_CMD_DRAW_INDEXED:
          disp->CmdDrawIndexed(commandBuffer
             , cmd->u.draw_indexed.index_count             , cmd->u.draw_indexed.instance_count             , cmd->u.draw_indexed.first_index             , cmd->u.draw_indexed.vertex_offset             , cmd->u.draw_indexed.first_instance          );
          break;
      case VK_CMD_DRAW_MULTI_EXT:
          disp->CmdDrawMultiEXT(commandBuffer
             , cmd->u.draw_multi_ext.draw_count             , cmd->u.draw_multi_ext.vertex_info             , cmd->u.draw_multi_ext.instance_count             , cmd->u.draw_multi_ext.first_instance             , cmd->u.draw_multi_ext.stride          );
          break;
      case VK_CMD_DRAW_MULTI_INDEXED_EXT:
          disp->CmdDrawMultiIndexedEXT(commandBuffer
             , cmd->u.draw_multi_indexed_ext.draw_count             , cmd->u.draw_multi_indexed_ext.index_info             , cmd->u.draw_multi_indexed_ext.instance_count             , cmd->u.draw_multi_indexed_ext.first_instance             , cmd->u.draw_multi_indexed_ext.stride             , cmd->u.draw_multi_indexed_ext.vertex_offset          );
          break;
      case VK_CMD_DRAW_INDIRECT:
          disp->CmdDrawIndirect(commandBuffer
             , cmd->u.draw_indirect.buffer             , cmd->u.draw_indirect.offset             , cmd->u.draw_indirect.draw_count             , cmd->u.draw_indirect.stride          );
          break;
      case VK_CMD_DRAW_INDEXED_INDIRECT:
          disp->CmdDrawIndexedIndirect(commandBuffer
             , cmd->u.draw_indexed_indirect.buffer             , cmd->u.draw_indexed_indirect.offset             , cmd->u.draw_indexed_indirect.draw_count             , cmd->u.draw_indexed_indirect.stride          );
          break;
      case VK_CMD_DISPATCH:
          disp->CmdDispatch(commandBuffer
             , cmd->u.dispatch.group_count_x             , cmd->u.dispatch.group_count_y             , cmd->u.dispatch.group_count_z          );
          break;
      case VK_CMD_DISPATCH_INDIRECT:
          disp->CmdDispatchIndirect(commandBuffer
             , cmd->u.dispatch_indirect.buffer             , cmd->u.dispatch_indirect.offset          );
          break;
      case VK_CMD_SUBPASS_SHADING_HUAWEI:
          disp->CmdSubpassShadingHUAWEI(commandBuffer
          );
          break;
      case VK_CMD_COPY_BUFFER:
          disp->CmdCopyBuffer(commandBuffer
             , cmd->u.copy_buffer.src_buffer             , cmd->u.copy_buffer.dst_buffer             , cmd->u.copy_buffer.region_count             , cmd->u.copy_buffer.regions          );
          break;
      case VK_CMD_COPY_IMAGE:
          disp->CmdCopyImage(commandBuffer
             , cmd->u.copy_image.src_image             , cmd->u.copy_image.src_image_layout             , cmd->u.copy_image.dst_image             , cmd->u.copy_image.dst_image_layout             , cmd->u.copy_image.region_count             , cmd->u.copy_image.regions          );
          break;
      case VK_CMD_BLIT_IMAGE:
          disp->CmdBlitImage(commandBuffer
             , cmd->u.blit_image.src_image             , cmd->u.blit_image.src_image_layout             , cmd->u.blit_image.dst_image             , cmd->u.blit_image.dst_image_layout             , cmd->u.blit_image.region_count             , cmd->u.blit_image.regions             , cmd->u.blit_image.filter          );
          break;
      case VK_CMD_COPY_BUFFER_TO_IMAGE:
          disp->CmdCopyBufferToImage(commandBuffer
             , cmd->u.copy_buffer_to_image.src_buffer             , cmd->u.copy_buffer_to_image.dst_image             , cmd->u.copy_buffer_to_image.dst_image_layout             , cmd->u.copy_buffer_to_image.region_count             , cmd->u.copy_buffer_to_image.regions          );
          break;
      case VK_CMD_COPY_IMAGE_TO_BUFFER:
          disp->CmdCopyImageToBuffer(commandBuffer
             , cmd->u.copy_image_to_buffer.src_image             , cmd->u.copy_image_to_buffer.src_image_layout             , cmd->u.copy_image_to_buffer.dst_buffer             , cmd->u.copy_image_to_buffer.region_count             , cmd->u.copy_image_to_buffer.regions          );
          break;
      case VK_CMD_UPDATE_BUFFER:
          disp->CmdUpdateBuffer(commandBuffer
             , cmd->u.update_buffer.dst_buffer             , cmd->u.update_buffer.dst_offset             , cmd->u.update_buffer.data_size             , cmd->u.update_buffer.data          );
          break;
      case VK_CMD_FILL_BUFFER:
          disp->CmdFillBuffer(commandBuffer
             , cmd->u.fill_buffer.dst_buffer             , cmd->u.fill_buffer.dst_offset             , cmd->u.fill_buffer.size             , cmd->u.fill_buffer.data          );
          break;
      case VK_CMD_CLEAR_COLOR_IMAGE:
          disp->CmdClearColorImage(commandBuffer
             , cmd->u.clear_color_image.image             , cmd->u.clear_color_image.image_layout             , cmd->u.clear_color_image.color             , cmd->u.clear_color_image.range_count             , cmd->u.clear_color_image.ranges          );
          break;
      case VK_CMD_CLEAR_DEPTH_STENCIL_IMAGE:
          disp->CmdClearDepthStencilImage(commandBuffer
             , cmd->u.clear_depth_stencil_image.image             , cmd->u.clear_depth_stencil_image.image_layout             , cmd->u.clear_depth_stencil_image.depth_stencil             , cmd->u.clear_depth_stencil_image.range_count             , cmd->u.clear_depth_stencil_image.ranges          );
          break;
      case VK_CMD_CLEAR_ATTACHMENTS:
          disp->CmdClearAttachments(commandBuffer
             , cmd->u.clear_attachments.attachment_count             , cmd->u.clear_attachments.attachments             , cmd->u.clear_attachments.rect_count             , cmd->u.clear_attachments.rects          );
          break;
      case VK_CMD_RESOLVE_IMAGE:
          disp->CmdResolveImage(commandBuffer
             , cmd->u.resolve_image.src_image             , cmd->u.resolve_image.src_image_layout             , cmd->u.resolve_image.dst_image             , cmd->u.resolve_image.dst_image_layout             , cmd->u.resolve_image.region_count             , cmd->u.resolve_image.regions          );
          break;
      case VK_CMD_SET_EVENT:
          disp->CmdSetEvent(commandBuffer
             , cmd->u.set_event.event             , cmd->u.set_event.stage_mask          );
          break;
      case VK_CMD_RESET_EVENT:
          disp->CmdResetEvent(commandBuffer
             , cmd->u.reset_event.event             , cmd->u.reset_event.stage_mask          );
          break;
      case VK_CMD_WAIT_EVENTS:
          disp->CmdWaitEvents(commandBuffer
             , cmd->u.wait_events.event_count             , cmd->u.wait_events.events             , cmd->u.wait_events.src_stage_mask             , cmd->u.wait_events.dst_stage_mask             , cmd->u.wait_events.memory_barrier_count             , cmd->u.wait_events.memory_barriers             , cmd->u.wait_events.buffer_memory_barrier_count             , cmd->u.wait_events.buffer_memory_barriers             , cmd->u.wait_events.image_memory_barrier_count             , cmd->u.wait_events.image_memory_barriers          );
          break;
      case VK_CMD_PIPELINE_BARRIER:
          disp->CmdPipelineBarrier(commandBuffer
             , cmd->u.pipeline_barrier.src_stage_mask             , cmd->u.pipeline_barrier.dst_stage_mask             , cmd->u.pipeline_barrier.dependency_flags             , cmd->u.pipeline_barrier.memory_barrier_count             , cmd->u.pipeline_barrier.memory_barriers             , cmd->u.pipeline_barrier.buffer_memory_barrier_count             , cmd->u.pipeline_barrier.buffer_memory_barriers             , cmd->u.pipeline_barrier.image_memory_barrier_count             , cmd->u.pipeline_barrier.image_memory_barriers          );
          break;
      case VK_CMD_BEGIN_QUERY:
          disp->CmdBeginQuery(commandBuffer
             , cmd->u.begin_query.query_pool             , cmd->u.begin_query.query             , cmd->u.begin_query.flags          );
          break;
      case VK_CMD_END_QUERY:
          disp->CmdEndQuery(commandBuffer
             , cmd->u.end_query.query_pool             , cmd->u.end_query.query          );
          break;
      case VK_CMD_BEGIN_CONDITIONAL_RENDERING_EXT:
          disp->CmdBeginConditionalRenderingEXT(commandBuffer
             , cmd->u.begin_conditional_rendering_ext.conditional_rendering_begin          );
          break;
      case VK_CMD_END_CONDITIONAL_RENDERING_EXT:
          disp->CmdEndConditionalRenderingEXT(commandBuffer
          );
          break;
      case VK_CMD_RESET_QUERY_POOL:
          disp->CmdResetQueryPool(commandBuffer
             , cmd->u.reset_query_pool.query_pool             , cmd->u.reset_query_pool.first_query             , cmd->u.reset_query_pool.query_count          );
          break;
      case VK_CMD_WRITE_TIMESTAMP:
          disp->CmdWriteTimestamp(commandBuffer
             , cmd->u.write_timestamp.pipeline_stage             , cmd->u.write_timestamp.query_pool             , cmd->u.write_timestamp.query          );
          break;
      case VK_CMD_COPY_QUERY_POOL_RESULTS:
          disp->CmdCopyQueryPoolResults(commandBuffer
             , cmd->u.copy_query_pool_results.query_pool             , cmd->u.copy_query_pool_results.first_query             , cmd->u.copy_query_pool_results.query_count             , cmd->u.copy_query_pool_results.dst_buffer             , cmd->u.copy_query_pool_results.dst_offset             , cmd->u.copy_query_pool_results.stride             , cmd->u.copy_query_pool_results.flags          );
          break;
      case VK_CMD_PUSH_CONSTANTS:
          disp->CmdPushConstants(commandBuffer
             , cmd->u.push_constants.layout             , cmd->u.push_constants.stage_flags             , cmd->u.push_constants.offset             , cmd->u.push_constants.size             , cmd->u.push_constants.values          );
          break;
      case VK_CMD_BEGIN_RENDER_PASS:
          disp->CmdBeginRenderPass(commandBuffer
             , cmd->u.begin_render_pass.render_pass_begin             , cmd->u.begin_render_pass.contents          );
          break;
      case VK_CMD_NEXT_SUBPASS:
          disp->CmdNextSubpass(commandBuffer
             , cmd->u.next_subpass.contents          );
          break;
      case VK_CMD_END_RENDER_PASS:
          disp->CmdEndRenderPass(commandBuffer
          );
          break;
      case VK_CMD_EXECUTE_COMMANDS:
          disp->CmdExecuteCommands(commandBuffer
             , cmd->u.execute_commands.command_buffer_count             , cmd->u.execute_commands.command_buffers          );
          break;
      case VK_CMD_DEBUG_MARKER_BEGIN_EXT:
          disp->CmdDebugMarkerBeginEXT(commandBuffer
             , cmd->u.debug_marker_begin_ext.marker_info          );
          break;
      case VK_CMD_DEBUG_MARKER_END_EXT:
          disp->CmdDebugMarkerEndEXT(commandBuffer
          );
          break;
      case VK_CMD_DEBUG_MARKER_INSERT_EXT:
          disp->CmdDebugMarkerInsertEXT(commandBuffer
             , cmd->u.debug_marker_insert_ext.marker_info          );
          break;
      case VK_CMD_EXECUTE_GENERATED_COMMANDS_NV:
          disp->CmdExecuteGeneratedCommandsNV(commandBuffer
             , cmd->u.execute_generated_commands_nv.is_preprocessed             , cmd->u.execute_generated_commands_nv.generated_commands_info          );
          break;
      case VK_CMD_PREPROCESS_GENERATED_COMMANDS_NV:
          disp->CmdPreprocessGeneratedCommandsNV(commandBuffer
             , cmd->u.preprocess_generated_commands_nv.generated_commands_info          );
          break;
      case VK_CMD_BIND_PIPELINE_SHADER_GROUP_NV:
          disp->CmdBindPipelineShaderGroupNV(commandBuffer
             , cmd->u.bind_pipeline_shader_group_nv.pipeline_bind_point             , cmd->u.bind_pipeline_shader_group_nv.pipeline             , cmd->u.bind_pipeline_shader_group_nv.group_index          );
          break;
      case VK_CMD_PUSH_DESCRIPTOR_SET_KHR:
          disp->CmdPushDescriptorSetKHR(commandBuffer
             , cmd->u.push_descriptor_set_khr.pipeline_bind_point             , cmd->u.push_descriptor_set_khr.layout             , cmd->u.push_descriptor_set_khr.set             , cmd->u.push_descriptor_set_khr.descriptor_write_count             , cmd->u.push_descriptor_set_khr.descriptor_writes          );
          break;
      case VK_CMD_SET_DEVICE_MASK:
          disp->CmdSetDeviceMask(commandBuffer
             , cmd->u.set_device_mask.device_mask          );
          break;
      case VK_CMD_DISPATCH_BASE:
          disp->CmdDispatchBase(commandBuffer
             , cmd->u.dispatch_base.base_group_x             , cmd->u.dispatch_base.base_group_y             , cmd->u.dispatch_base.base_group_z             , cmd->u.dispatch_base.group_count_x             , cmd->u.dispatch_base.group_count_y             , cmd->u.dispatch_base.group_count_z          );
          break;
      case VK_CMD_PUSH_DESCRIPTOR_SET_WITH_TEMPLATE_KHR:
          disp->CmdPushDescriptorSetWithTemplateKHR(commandBuffer
             , cmd->u.push_descriptor_set_with_template_khr.descriptor_update_template             , cmd->u.push_descriptor_set_with_template_khr.layout             , cmd->u.push_descriptor_set_with_template_khr.set             , cmd->u.push_descriptor_set_with_template_khr.data          );
          break;
      case VK_CMD_SET_VIEWPORT_WSCALING_NV:
          disp->CmdSetViewportWScalingNV(commandBuffer
             , cmd->u.set_viewport_wscaling_nv.first_viewport             , cmd->u.set_viewport_wscaling_nv.viewport_count             , cmd->u.set_viewport_wscaling_nv.viewport_wscalings          );
          break;
      case VK_CMD_SET_DISCARD_RECTANGLE_EXT:
          disp->CmdSetDiscardRectangleEXT(commandBuffer
             , cmd->u.set_discard_rectangle_ext.first_discard_rectangle             , cmd->u.set_discard_rectangle_ext.discard_rectangle_count             , cmd->u.set_discard_rectangle_ext.discard_rectangles          );
          break;
      case VK_CMD_SET_SAMPLE_LOCATIONS_EXT:
          disp->CmdSetSampleLocationsEXT(commandBuffer
             , cmd->u.set_sample_locations_ext.sample_locations_info          );
          break;
      case VK_CMD_BEGIN_DEBUG_UTILS_LABEL_EXT:
          disp->CmdBeginDebugUtilsLabelEXT(commandBuffer
             , cmd->u.begin_debug_utils_label_ext.label_info          );
          break;
      case VK_CMD_END_DEBUG_UTILS_LABEL_EXT:
          disp->CmdEndDebugUtilsLabelEXT(commandBuffer
          );
          break;
      case VK_CMD_INSERT_DEBUG_UTILS_LABEL_EXT:
          disp->CmdInsertDebugUtilsLabelEXT(commandBuffer
             , cmd->u.insert_debug_utils_label_ext.label_info          );
          break;
      case VK_CMD_WRITE_BUFFER_MARKER_AMD:
          disp->CmdWriteBufferMarkerAMD(commandBuffer
             , cmd->u.write_buffer_marker_amd.pipeline_stage             , cmd->u.write_buffer_marker_amd.dst_buffer             , cmd->u.write_buffer_marker_amd.dst_offset             , cmd->u.write_buffer_marker_amd.marker          );
          break;
      case VK_CMD_BEGIN_RENDER_PASS2:
          disp->CmdBeginRenderPass2(commandBuffer
             , cmd->u.begin_render_pass2.render_pass_begin             , cmd->u.begin_render_pass2.subpass_begin_info          );
          break;
      case VK_CMD_NEXT_SUBPASS2:
          disp->CmdNextSubpass2(commandBuffer
             , cmd->u.next_subpass2.subpass_begin_info             , cmd->u.next_subpass2.subpass_end_info          );
          break;
      case VK_CMD_END_RENDER_PASS2:
          disp->CmdEndRenderPass2(commandBuffer
             , cmd->u.end_render_pass2.subpass_end_info          );
          break;
      case VK_CMD_DRAW_INDIRECT_COUNT:
          disp->CmdDrawIndirectCount(commandBuffer
             , cmd->u.draw_indirect_count.buffer             , cmd->u.draw_indirect_count.offset             , cmd->u.draw_indirect_count.count_buffer             , cmd->u.draw_indirect_count.count_buffer_offset             , cmd->u.draw_indirect_count.max_draw_count             , cmd->u.draw_indirect_count.stride          );
          break;
      case VK_CMD_DRAW_INDEXED_INDIRECT_COUNT:
          disp->CmdDrawIndexedIndirectCount(commandBuffer
             , cmd->u.draw_indexed_indirect_count.buffer             , cmd->u.draw_indexed_indirect_count.offset             , cmd->u.draw_indexed_indirect_count.count_buffer             , cmd->u.draw_indexed_indirect_count.count_buffer_offset             , cmd->u.draw_indexed_indirect_count.max_draw_count             , cmd->u.draw_indexed_indirect_count.stride          );
          break;
      case VK_CMD_SET_CHECKPOINT_NV:
          disp->CmdSetCheckpointNV(commandBuffer
             , cmd->u.set_checkpoint_nv.checkpoint_marker          );
          break;
      case VK_CMD_BIND_TRANSFORM_FEEDBACK_BUFFERS_EXT:
          disp->CmdBindTransformFeedbackBuffersEXT(commandBuffer
             , cmd->u.bind_transform_feedback_buffers_ext.first_binding             , cmd->u.bind_transform_feedback_buffers_ext.binding_count             , cmd->u.bind_transform_feedback_buffers_ext.buffers             , cmd->u.bind_transform_feedback_buffers_ext.offsets             , cmd->u.bind_transform_feedback_buffers_ext.sizes          );
          break;
      case VK_CMD_BEGIN_TRANSFORM_FEEDBACK_EXT:
          disp->CmdBeginTransformFeedbackEXT(commandBuffer
             , cmd->u.begin_transform_feedback_ext.first_counter_buffer             , cmd->u.begin_transform_feedback_ext.counter_buffer_count             , cmd->u.begin_transform_feedback_ext.counter_buffers             , cmd->u.begin_transform_feedback_ext.counter_buffer_offsets          );
          break;
      case VK_CMD_END_TRANSFORM_FEEDBACK_EXT:
          disp->CmdEndTransformFeedbackEXT(commandBuffer
             , cmd->u.end_transform_feedback_ext.first_counter_buffer             , cmd->u.end_transform_feedback_ext.counter_buffer_count             , cmd->u.end_transform_feedback_ext.counter_buffers             , cmd->u.end_transform_feedback_ext.counter_buffer_offsets          );
          break;
      case VK_CMD_BEGIN_QUERY_INDEXED_EXT:
          disp->CmdBeginQueryIndexedEXT(commandBuffer
             , cmd->u.begin_query_indexed_ext.query_pool             , cmd->u.begin_query_indexed_ext.query             , cmd->u.begin_query_indexed_ext.flags             , cmd->u.begin_query_indexed_ext.index          );
          break;
      case VK_CMD_END_QUERY_INDEXED_EXT:
          disp->CmdEndQueryIndexedEXT(commandBuffer
             , cmd->u.end_query_indexed_ext.query_pool             , cmd->u.end_query_indexed_ext.query             , cmd->u.end_query_indexed_ext.index          );
          break;
      case VK_CMD_DRAW_INDIRECT_BYTE_COUNT_EXT:
          disp->CmdDrawIndirectByteCountEXT(commandBuffer
             , cmd->u.draw_indirect_byte_count_ext.instance_count             , cmd->u.draw_indirect_byte_count_ext.first_instance             , cmd->u.draw_indirect_byte_count_ext.counter_buffer             , cmd->u.draw_indirect_byte_count_ext.counter_buffer_offset             , cmd->u.draw_indirect_byte_count_ext.counter_offset             , cmd->u.draw_indirect_byte_count_ext.vertex_stride          );
          break;
      case VK_CMD_SET_EXCLUSIVE_SCISSOR_NV:
          disp->CmdSetExclusiveScissorNV(commandBuffer
             , cmd->u.set_exclusive_scissor_nv.first_exclusive_scissor             , cmd->u.set_exclusive_scissor_nv.exclusive_scissor_count             , cmd->u.set_exclusive_scissor_nv.exclusive_scissors          );
          break;
      case VK_CMD_BIND_SHADING_RATE_IMAGE_NV:
          disp->CmdBindShadingRateImageNV(commandBuffer
             , cmd->u.bind_shading_rate_image_nv.image_view             , cmd->u.bind_shading_rate_image_nv.image_layout          );
          break;
      case VK_CMD_SET_VIEWPORT_SHADING_RATE_PALETTE_NV:
          disp->CmdSetViewportShadingRatePaletteNV(commandBuffer
             , cmd->u.set_viewport_shading_rate_palette_nv.first_viewport             , cmd->u.set_viewport_shading_rate_palette_nv.viewport_count             , cmd->u.set_viewport_shading_rate_palette_nv.shading_rate_palettes          );
          break;
      case VK_CMD_SET_COARSE_SAMPLE_ORDER_NV:
          disp->CmdSetCoarseSampleOrderNV(commandBuffer
             , cmd->u.set_coarse_sample_order_nv.sample_order_type             , cmd->u.set_coarse_sample_order_nv.custom_sample_order_count             , cmd->u.set_coarse_sample_order_nv.custom_sample_orders          );
          break;
      case VK_CMD_DRAW_MESH_TASKS_NV:
          disp->CmdDrawMeshTasksNV(commandBuffer
             , cmd->u.draw_mesh_tasks_nv.task_count             , cmd->u.draw_mesh_tasks_nv.first_task          );
          break;
      case VK_CMD_DRAW_MESH_TASKS_INDIRECT_NV:
          disp->CmdDrawMeshTasksIndirectNV(commandBuffer
             , cmd->u.draw_mesh_tasks_indirect_nv.buffer             , cmd->u.draw_mesh_tasks_indirect_nv.offset             , cmd->u.draw_mesh_tasks_indirect_nv.draw_count             , cmd->u.draw_mesh_tasks_indirect_nv.stride          );
          break;
      case VK_CMD_DRAW_MESH_TASKS_INDIRECT_COUNT_NV:
          disp->CmdDrawMeshTasksIndirectCountNV(commandBuffer
             , cmd->u.draw_mesh_tasks_indirect_count_nv.buffer             , cmd->u.draw_mesh_tasks_indirect_count_nv.offset             , cmd->u.draw_mesh_tasks_indirect_count_nv.count_buffer             , cmd->u.draw_mesh_tasks_indirect_count_nv.count_buffer_offset             , cmd->u.draw_mesh_tasks_indirect_count_nv.max_draw_count             , cmd->u.draw_mesh_tasks_indirect_count_nv.stride          );
          break;
      case VK_CMD_DRAW_MESH_TASKS_EXT:
          disp->CmdDrawMeshTasksEXT(commandBuffer
             , cmd->u.draw_mesh_tasks_ext.group_count_x             , cmd->u.draw_mesh_tasks_ext.group_count_y             , cmd->u.draw_mesh_tasks_ext.group_count_z          );
          break;
      case VK_CMD_DRAW_MESH_TASKS_INDIRECT_EXT:
          disp->CmdDrawMeshTasksIndirectEXT(commandBuffer
             , cmd->u.draw_mesh_tasks_indirect_ext.buffer             , cmd->u.draw_mesh_tasks_indirect_ext.offset             , cmd->u.draw_mesh_tasks_indirect_ext.draw_count             , cmd->u.draw_mesh_tasks_indirect_ext.stride          );
          break;
      case VK_CMD_DRAW_MESH_TASKS_INDIRECT_COUNT_EXT:
          disp->CmdDrawMeshTasksIndirectCountEXT(commandBuffer
             , cmd->u.draw_mesh_tasks_indirect_count_ext.buffer             , cmd->u.draw_mesh_tasks_indirect_count_ext.offset             , cmd->u.draw_mesh_tasks_indirect_count_ext.count_buffer             , cmd->u.draw_mesh_tasks_indirect_count_ext.count_buffer_offset             , cmd->u.draw_mesh_tasks_indirect_count_ext.max_draw_count             , cmd->u.draw_mesh_tasks_indirect_count_ext.stride          );
          break;
      case VK_CMD_BIND_INVOCATION_MASK_HUAWEI:
          disp->CmdBindInvocationMaskHUAWEI(commandBuffer
             , cmd->u.bind_invocation_mask_huawei.image_view             , cmd->u.bind_invocation_mask_huawei.image_layout          );
          break;
      case VK_CMD_COPY_ACCELERATION_STRUCTURE_NV:
          disp->CmdCopyAccelerationStructureNV(commandBuffer
             , cmd->u.copy_acceleration_structure_nv.dst             , cmd->u.copy_acceleration_structure_nv.src             , cmd->u.copy_acceleration_structure_nv.mode          );
          break;
      case VK_CMD_COPY_ACCELERATION_STRUCTURE_KHR:
          disp->CmdCopyAccelerationStructureKHR(commandBuffer
             , cmd->u.copy_acceleration_structure_khr.info          );
          break;
      case VK_CMD_COPY_ACCELERATION_STRUCTURE_TO_MEMORY_KHR:
          disp->CmdCopyAccelerationStructureToMemoryKHR(commandBuffer
             , cmd->u.copy_acceleration_structure_to_memory_khr.info          );
          break;
      case VK_CMD_COPY_MEMORY_TO_ACCELERATION_STRUCTURE_KHR:
          disp->CmdCopyMemoryToAccelerationStructureKHR(commandBuffer
             , cmd->u.copy_memory_to_acceleration_structure_khr.info          );
          break;
      case VK_CMD_WRITE_ACCELERATION_STRUCTURES_PROPERTIES_KHR:
          disp->CmdWriteAccelerationStructuresPropertiesKHR(commandBuffer
             , cmd->u.write_acceleration_structures_properties_khr.acceleration_structure_count             , cmd->u.write_acceleration_structures_properties_khr.acceleration_structures             , cmd->u.write_acceleration_structures_properties_khr.query_type             , cmd->u.write_acceleration_structures_properties_khr.query_pool             , cmd->u.write_acceleration_structures_properties_khr.first_query          );
          break;
      case VK_CMD_WRITE_ACCELERATION_STRUCTURES_PROPERTIES_NV:
          disp->CmdWriteAccelerationStructuresPropertiesNV(commandBuffer
             , cmd->u.write_acceleration_structures_properties_nv.acceleration_structure_count             , cmd->u.write_acceleration_structures_properties_nv.acceleration_structures             , cmd->u.write_acceleration_structures_properties_nv.query_type             , cmd->u.write_acceleration_structures_properties_nv.query_pool             , cmd->u.write_acceleration_structures_properties_nv.first_query          );
          break;
      case VK_CMD_BUILD_ACCELERATION_STRUCTURE_NV:
          disp->CmdBuildAccelerationStructureNV(commandBuffer
             , cmd->u.build_acceleration_structure_nv.info             , cmd->u.build_acceleration_structure_nv.instance_data             , cmd->u.build_acceleration_structure_nv.instance_offset             , cmd->u.build_acceleration_structure_nv.update             , cmd->u.build_acceleration_structure_nv.dst             , cmd->u.build_acceleration_structure_nv.src             , cmd->u.build_acceleration_structure_nv.scratch             , cmd->u.build_acceleration_structure_nv.scratch_offset          );
          break;
      case VK_CMD_TRACE_RAYS_KHR:
          disp->CmdTraceRaysKHR(commandBuffer
             , cmd->u.trace_rays_khr.raygen_shader_binding_table             , cmd->u.trace_rays_khr.miss_shader_binding_table             , cmd->u.trace_rays_khr.hit_shader_binding_table             , cmd->u.trace_rays_khr.callable_shader_binding_table             , cmd->u.trace_rays_khr.width             , cmd->u.trace_rays_khr.height             , cmd->u.trace_rays_khr.depth          );
          break;
      case VK_CMD_TRACE_RAYS_NV:
          disp->CmdTraceRaysNV(commandBuffer
             , cmd->u.trace_rays_nv.raygen_shader_binding_table_buffer             , cmd->u.trace_rays_nv.raygen_shader_binding_offset             , cmd->u.trace_rays_nv.miss_shader_binding_table_buffer             , cmd->u.trace_rays_nv.miss_shader_binding_offset             , cmd->u.trace_rays_nv.miss_shader_binding_stride             , cmd->u.trace_rays_nv.hit_shader_binding_table_buffer             , cmd->u.trace_rays_nv.hit_shader_binding_offset             , cmd->u.trace_rays_nv.hit_shader_binding_stride             , cmd->u.trace_rays_nv.callable_shader_binding_table_buffer             , cmd->u.trace_rays_nv.callable_shader_binding_offset             , cmd->u.trace_rays_nv.callable_shader_binding_stride             , cmd->u.trace_rays_nv.width             , cmd->u.trace_rays_nv.height             , cmd->u.trace_rays_nv.depth          );
          break;
      case VK_CMD_TRACE_RAYS_INDIRECT_KHR:
          disp->CmdTraceRaysIndirectKHR(commandBuffer
             , cmd->u.trace_rays_indirect_khr.raygen_shader_binding_table             , cmd->u.trace_rays_indirect_khr.miss_shader_binding_table             , cmd->u.trace_rays_indirect_khr.hit_shader_binding_table             , cmd->u.trace_rays_indirect_khr.callable_shader_binding_table             , cmd->u.trace_rays_indirect_khr.indirect_device_address          );
          break;
      case VK_CMD_TRACE_RAYS_INDIRECT2_KHR:
          disp->CmdTraceRaysIndirect2KHR(commandBuffer
             , cmd->u.trace_rays_indirect2_khr.indirect_device_address          );
          break;
      case VK_CMD_SET_RAY_TRACING_PIPELINE_STACK_SIZE_KHR:
          disp->CmdSetRayTracingPipelineStackSizeKHR(commandBuffer
             , cmd->u.set_ray_tracing_pipeline_stack_size_khr.pipeline_stack_size          );
          break;
      case VK_CMD_SET_PERFORMANCE_MARKER_INTEL:
          disp->CmdSetPerformanceMarkerINTEL(commandBuffer
             , cmd->u.set_performance_marker_intel.marker_info          );
          break;
      case VK_CMD_SET_PERFORMANCE_STREAM_MARKER_INTEL:
          disp->CmdSetPerformanceStreamMarkerINTEL(commandBuffer
             , cmd->u.set_performance_stream_marker_intel.marker_info          );
          break;
      case VK_CMD_SET_PERFORMANCE_OVERRIDE_INTEL:
          disp->CmdSetPerformanceOverrideINTEL(commandBuffer
             , cmd->u.set_performance_override_intel.override_info          );
          break;
      case VK_CMD_SET_LINE_STIPPLE_EXT:
          disp->CmdSetLineStippleEXT(commandBuffer
             , cmd->u.set_line_stipple_ext.line_stipple_factor             , cmd->u.set_line_stipple_ext.line_stipple_pattern          );
          break;
      case VK_CMD_BUILD_ACCELERATION_STRUCTURES_KHR:
          disp->CmdBuildAccelerationStructuresKHR(commandBuffer
             , cmd->u.build_acceleration_structures_khr.info_count             , cmd->u.build_acceleration_structures_khr.infos             , cmd->u.build_acceleration_structures_khr.pp_build_range_infos          );
          break;
      case VK_CMD_BUILD_ACCELERATION_STRUCTURES_INDIRECT_KHR:
          disp->CmdBuildAccelerationStructuresIndirectKHR(commandBuffer
             , cmd->u.build_acceleration_structures_indirect_khr.info_count             , cmd->u.build_acceleration_structures_indirect_khr.infos             , cmd->u.build_acceleration_structures_indirect_khr.indirect_device_addresses             , cmd->u.build_acceleration_structures_indirect_khr.indirect_strides             , cmd->u.build_acceleration_structures_indirect_khr.pp_max_primitive_counts          );
          break;
      case VK_CMD_SET_CULL_MODE:
          disp->CmdSetCullMode(commandBuffer
             , cmd->u.set_cull_mode.cull_mode          );
          break;
      case VK_CMD_SET_FRONT_FACE:
          disp->CmdSetFrontFace(commandBuffer
             , cmd->u.set_front_face.front_face          );
          break;
      case VK_CMD_SET_PRIMITIVE_TOPOLOGY:
          disp->CmdSetPrimitiveTopology(commandBuffer
             , cmd->u.set_primitive_topology.primitive_topology          );
          break;
      case VK_CMD_SET_VIEWPORT_WITH_COUNT:
          disp->CmdSetViewportWithCount(commandBuffer
             , cmd->u.set_viewport_with_count.viewport_count             , cmd->u.set_viewport_with_count.viewports          );
          break;
      case VK_CMD_SET_SCISSOR_WITH_COUNT:
          disp->CmdSetScissorWithCount(commandBuffer
             , cmd->u.set_scissor_with_count.scissor_count             , cmd->u.set_scissor_with_count.scissors          );
          break;
      case VK_CMD_BIND_VERTEX_BUFFERS2:
          disp->CmdBindVertexBuffers2(commandBuffer
             , cmd->u.bind_vertex_buffers2.first_binding             , cmd->u.bind_vertex_buffers2.binding_count             , cmd->u.bind_vertex_buffers2.buffers             , cmd->u.bind_vertex_buffers2.offsets             , cmd->u.bind_vertex_buffers2.sizes             , cmd->u.bind_vertex_buffers2.strides          );
          break;
      case VK_CMD_SET_DEPTH_TEST_ENABLE:
          disp->CmdSetDepthTestEnable(commandBuffer
             , cmd->u.set_depth_test_enable.depth_test_enable          );
          break;
      case VK_CMD_SET_DEPTH_WRITE_ENABLE:
          disp->CmdSetDepthWriteEnable(commandBuffer
             , cmd->u.set_depth_write_enable.depth_write_enable          );
          break;
      case VK_CMD_SET_DEPTH_COMPARE_OP:
          disp->CmdSetDepthCompareOp(commandBuffer
             , cmd->u.set_depth_compare_op.depth_compare_op          );
          break;
      case VK_CMD_SET_DEPTH_BOUNDS_TEST_ENABLE:
          disp->CmdSetDepthBoundsTestEnable(commandBuffer
             , cmd->u.set_depth_bounds_test_enable.depth_bounds_test_enable          );
          break;
      case VK_CMD_SET_STENCIL_TEST_ENABLE:
          disp->CmdSetStencilTestEnable(commandBuffer
             , cmd->u.set_stencil_test_enable.stencil_test_enable          );
          break;
      case VK_CMD_SET_STENCIL_OP:
          disp->CmdSetStencilOp(commandBuffer
             , cmd->u.set_stencil_op.face_mask             , cmd->u.set_stencil_op.fail_op             , cmd->u.set_stencil_op.pass_op             , cmd->u.set_stencil_op.depth_fail_op             , cmd->u.set_stencil_op.compare_op          );
          break;
      case VK_CMD_SET_PATCH_CONTROL_POINTS_EXT:
          disp->CmdSetPatchControlPointsEXT(commandBuffer
             , cmd->u.set_patch_control_points_ext.patch_control_points          );
          break;
      case VK_CMD_SET_RASTERIZER_DISCARD_ENABLE:
          disp->CmdSetRasterizerDiscardEnable(commandBuffer
             , cmd->u.set_rasterizer_discard_enable.rasterizer_discard_enable          );
          break;
      case VK_CMD_SET_DEPTH_BIAS_ENABLE:
          disp->CmdSetDepthBiasEnable(commandBuffer
             , cmd->u.set_depth_bias_enable.depth_bias_enable          );
          break;
      case VK_CMD_SET_LOGIC_OP_EXT:
          disp->CmdSetLogicOpEXT(commandBuffer
             , cmd->u.set_logic_op_ext.logic_op          );
          break;
      case VK_CMD_SET_PRIMITIVE_RESTART_ENABLE:
          disp->CmdSetPrimitiveRestartEnable(commandBuffer
             , cmd->u.set_primitive_restart_enable.primitive_restart_enable          );
          break;
      case VK_CMD_SET_TESSELLATION_DOMAIN_ORIGIN_EXT:
          disp->CmdSetTessellationDomainOriginEXT(commandBuffer
             , cmd->u.set_tessellation_domain_origin_ext.domain_origin          );
          break;
      case VK_CMD_SET_DEPTH_CLAMP_ENABLE_EXT:
          disp->CmdSetDepthClampEnableEXT(commandBuffer
             , cmd->u.set_depth_clamp_enable_ext.depth_clamp_enable          );
          break;
      case VK_CMD_SET_POLYGON_MODE_EXT:
          disp->CmdSetPolygonModeEXT(commandBuffer
             , cmd->u.set_polygon_mode_ext.polygon_mode          );
          break;
      case VK_CMD_SET_RASTERIZATION_SAMPLES_EXT:
          disp->CmdSetRasterizationSamplesEXT(commandBuffer
             , cmd->u.set_rasterization_samples_ext.rasterization_samples          );
          break;
      case VK_CMD_SET_SAMPLE_MASK_EXT:
          disp->CmdSetSampleMaskEXT(commandBuffer
             , cmd->u.set_sample_mask_ext.samples             , cmd->u.set_sample_mask_ext.sample_mask          );
          break;
      case VK_CMD_SET_ALPHA_TO_COVERAGE_ENABLE_EXT:
          disp->CmdSetAlphaToCoverageEnableEXT(commandBuffer
             , cmd->u.set_alpha_to_coverage_enable_ext.alpha_to_coverage_enable          );
          break;
      case VK_CMD_SET_ALPHA_TO_ONE_ENABLE_EXT:
          disp->CmdSetAlphaToOneEnableEXT(commandBuffer
             , cmd->u.set_alpha_to_one_enable_ext.alpha_to_one_enable          );
          break;
      case VK_CMD_SET_LOGIC_OP_ENABLE_EXT:
          disp->CmdSetLogicOpEnableEXT(commandBuffer
             , cmd->u.set_logic_op_enable_ext.logic_op_enable          );
          break;
      case VK_CMD_SET_COLOR_BLEND_ENABLE_EXT:
          disp->CmdSetColorBlendEnableEXT(commandBuffer
             , cmd->u.set_color_blend_enable_ext.first_attachment             , cmd->u.set_color_blend_enable_ext.attachment_count             , cmd->u.set_color_blend_enable_ext.color_blend_enables          );
          break;
      case VK_CMD_SET_COLOR_BLEND_EQUATION_EXT:
          disp->CmdSetColorBlendEquationEXT(commandBuffer
             , cmd->u.set_color_blend_equation_ext.first_attachment             , cmd->u.set_color_blend_equation_ext.attachment_count             , cmd->u.set_color_blend_equation_ext.color_blend_equations          );
          break;
      case VK_CMD_SET_COLOR_WRITE_MASK_EXT:
          disp->CmdSetColorWriteMaskEXT(commandBuffer
             , cmd->u.set_color_write_mask_ext.first_attachment             , cmd->u.set_color_write_mask_ext.attachment_count             , cmd->u.set_color_write_mask_ext.color_write_masks          );
          break;
      case VK_CMD_SET_RASTERIZATION_STREAM_EXT:
          disp->CmdSetRasterizationStreamEXT(commandBuffer
             , cmd->u.set_rasterization_stream_ext.rasterization_stream          );
          break;
      case VK_CMD_SET_CONSERVATIVE_RASTERIZATION_MODE_EXT:
          disp->CmdSetConservativeRasterizationModeEXT(commandBuffer
             , cmd->u.set_conservative_rasterization_mode_ext.conservative_rasterization_mode          );
          break;
      case VK_CMD_SET_EXTRA_PRIMITIVE_OVERESTIMATION_SIZE_EXT:
          disp->CmdSetExtraPrimitiveOverestimationSizeEXT(commandBuffer
             , cmd->u.set_extra_primitive_overestimation_size_ext.extra_primitive_overestimation_size          );
          break;
      case VK_CMD_SET_DEPTH_CLIP_ENABLE_EXT:
          disp->CmdSetDepthClipEnableEXT(commandBuffer
             , cmd->u.set_depth_clip_enable_ext.depth_clip_enable          );
          break;
      case VK_CMD_SET_SAMPLE_LOCATIONS_ENABLE_EXT:
          disp->CmdSetSampleLocationsEnableEXT(commandBuffer
             , cmd->u.set_sample_locations_enable_ext.sample_locations_enable          );
          break;
      case VK_CMD_SET_COLOR_BLEND_ADVANCED_EXT:
          disp->CmdSetColorBlendAdvancedEXT(commandBuffer
             , cmd->u.set_color_blend_advanced_ext.first_attachment             , cmd->u.set_color_blend_advanced_ext.attachment_count             , cmd->u.set_color_blend_advanced_ext.color_blend_advanced          );
          break;
      case VK_CMD_SET_PROVOKING_VERTEX_MODE_EXT:
          disp->CmdSetProvokingVertexModeEXT(commandBuffer
             , cmd->u.set_provoking_vertex_mode_ext.provoking_vertex_mode          );
          break;
      case VK_CMD_SET_LINE_RASTERIZATION_MODE_EXT:
          disp->CmdSetLineRasterizationModeEXT(commandBuffer
             , cmd->u.set_line_rasterization_mode_ext.line_rasterization_mode          );
          break;
      case VK_CMD_SET_LINE_STIPPLE_ENABLE_EXT:
          disp->CmdSetLineStippleEnableEXT(commandBuffer
             , cmd->u.set_line_stipple_enable_ext.stippled_line_enable          );
          break;
      case VK_CMD_SET_DEPTH_CLIP_NEGATIVE_ONE_TO_ONE_EXT:
          disp->CmdSetDepthClipNegativeOneToOneEXT(commandBuffer
             , cmd->u.set_depth_clip_negative_one_to_one_ext.negative_one_to_one          );
          break;
      case VK_CMD_SET_VIEWPORT_WSCALING_ENABLE_NV:
          disp->CmdSetViewportWScalingEnableNV(commandBuffer
             , cmd->u.set_viewport_wscaling_enable_nv.viewport_wscaling_enable          );
          break;
      case VK_CMD_SET_VIEWPORT_SWIZZLE_NV:
          disp->CmdSetViewportSwizzleNV(commandBuffer
             , cmd->u.set_viewport_swizzle_nv.first_viewport             , cmd->u.set_viewport_swizzle_nv.viewport_count             , cmd->u.set_viewport_swizzle_nv.viewport_swizzles          );
          break;
      case VK_CMD_SET_COVERAGE_TO_COLOR_ENABLE_NV:
          disp->CmdSetCoverageToColorEnableNV(commandBuffer
             , cmd->u.set_coverage_to_color_enable_nv.coverage_to_color_enable          );
          break;
      case VK_CMD_SET_COVERAGE_TO_COLOR_LOCATION_NV:
          disp->CmdSetCoverageToColorLocationNV(commandBuffer
             , cmd->u.set_coverage_to_color_location_nv.coverage_to_color_location          );
          break;
      case VK_CMD_SET_COVERAGE_MODULATION_MODE_NV:
          disp->CmdSetCoverageModulationModeNV(commandBuffer
             , cmd->u.set_coverage_modulation_mode_nv.coverage_modulation_mode          );
          break;
      case VK_CMD_SET_COVERAGE_MODULATION_TABLE_ENABLE_NV:
          disp->CmdSetCoverageModulationTableEnableNV(commandBuffer
             , cmd->u.set_coverage_modulation_table_enable_nv.coverage_modulation_table_enable          );
          break;
      case VK_CMD_SET_COVERAGE_MODULATION_TABLE_NV:
          disp->CmdSetCoverageModulationTableNV(commandBuffer
             , cmd->u.set_coverage_modulation_table_nv.coverage_modulation_table_count             , cmd->u.set_coverage_modulation_table_nv.coverage_modulation_table          );
          break;
      case VK_CMD_SET_SHADING_RATE_IMAGE_ENABLE_NV:
          disp->CmdSetShadingRateImageEnableNV(commandBuffer
             , cmd->u.set_shading_rate_image_enable_nv.shading_rate_image_enable          );
          break;
      case VK_CMD_SET_COVERAGE_REDUCTION_MODE_NV:
          disp->CmdSetCoverageReductionModeNV(commandBuffer
             , cmd->u.set_coverage_reduction_mode_nv.coverage_reduction_mode          );
          break;
      case VK_CMD_SET_REPRESENTATIVE_FRAGMENT_TEST_ENABLE_NV:
          disp->CmdSetRepresentativeFragmentTestEnableNV(commandBuffer
             , cmd->u.set_representative_fragment_test_enable_nv.representative_fragment_test_enable          );
          break;
      case VK_CMD_COPY_BUFFER2:
          disp->CmdCopyBuffer2(commandBuffer
             , cmd->u.copy_buffer2.copy_buffer_info          );
          break;
      case VK_CMD_COPY_IMAGE2:
          disp->CmdCopyImage2(commandBuffer
             , cmd->u.copy_image2.copy_image_info          );
          break;
      case VK_CMD_BLIT_IMAGE2:
          disp->CmdBlitImage2(commandBuffer
             , cmd->u.blit_image2.blit_image_info          );
          break;
      case VK_CMD_COPY_BUFFER_TO_IMAGE2:
          disp->CmdCopyBufferToImage2(commandBuffer
             , cmd->u.copy_buffer_to_image2.copy_buffer_to_image_info          );
          break;
      case VK_CMD_COPY_IMAGE_TO_BUFFER2:
          disp->CmdCopyImageToBuffer2(commandBuffer
             , cmd->u.copy_image_to_buffer2.copy_image_to_buffer_info          );
          break;
      case VK_CMD_RESOLVE_IMAGE2:
          disp->CmdResolveImage2(commandBuffer
             , cmd->u.resolve_image2.resolve_image_info          );
          break;
      case VK_CMD_SET_FRAGMENT_SHADING_RATE_KHR:
          disp->CmdSetFragmentShadingRateKHR(commandBuffer
             , cmd->u.set_fragment_shading_rate_khr.fragment_size             , cmd->u.set_fragment_shading_rate_khr.combiner_ops          );
          break;
      case VK_CMD_SET_FRAGMENT_SHADING_RATE_ENUM_NV:
          disp->CmdSetFragmentShadingRateEnumNV(commandBuffer
             , cmd->u.set_fragment_shading_rate_enum_nv.shading_rate             , cmd->u.set_fragment_shading_rate_enum_nv.combiner_ops          );
          break;
      case VK_CMD_SET_VERTEX_INPUT_EXT:
          disp->CmdSetVertexInputEXT(commandBuffer
             , cmd->u.set_vertex_input_ext.vertex_binding_description_count             , cmd->u.set_vertex_input_ext.vertex_binding_descriptions             , cmd->u.set_vertex_input_ext.vertex_attribute_description_count             , cmd->u.set_vertex_input_ext.vertex_attribute_descriptions          );
          break;
      case VK_CMD_SET_COLOR_WRITE_ENABLE_EXT:
          disp->CmdSetColorWriteEnableEXT(commandBuffer
             , cmd->u.set_color_write_enable_ext.attachment_count             , cmd->u.set_color_write_enable_ext.color_write_enables          );
          break;
      case VK_CMD_SET_EVENT2:
          disp->CmdSetEvent2(commandBuffer
             , cmd->u.set_event2.event             , cmd->u.set_event2.dependency_info          );
          break;
      case VK_CMD_RESET_EVENT2:
          disp->CmdResetEvent2(commandBuffer
             , cmd->u.reset_event2.event             , cmd->u.reset_event2.stage_mask          );
          break;
      case VK_CMD_WAIT_EVENTS2:
          disp->CmdWaitEvents2(commandBuffer
             , cmd->u.wait_events2.event_count             , cmd->u.wait_events2.events             , cmd->u.wait_events2.dependency_infos          );
          break;
      case VK_CMD_PIPELINE_BARRIER2:
          disp->CmdPipelineBarrier2(commandBuffer
             , cmd->u.pipeline_barrier2.dependency_info          );
          break;
      case VK_CMD_WRITE_TIMESTAMP2:
          disp->CmdWriteTimestamp2(commandBuffer
             , cmd->u.write_timestamp2.stage             , cmd->u.write_timestamp2.query_pool             , cmd->u.write_timestamp2.query          );
          break;
      case VK_CMD_WRITE_BUFFER_MARKER2_AMD:
          disp->CmdWriteBufferMarker2AMD(commandBuffer
             , cmd->u.write_buffer_marker2_amd.stage             , cmd->u.write_buffer_marker2_amd.dst_buffer             , cmd->u.write_buffer_marker2_amd.dst_offset             , cmd->u.write_buffer_marker2_amd.marker          );
          break;
#ifdef VK_ENABLE_BETA_EXTENSIONS
      case VK_CMD_DECODE_VIDEO_KHR:
          disp->CmdDecodeVideoKHR(commandBuffer
             , cmd->u.decode_video_khr.decode_info          );
          break;
#endif // VK_ENABLE_BETA_EXTENSIONS
#ifdef VK_ENABLE_BETA_EXTENSIONS
      case VK_CMD_BEGIN_VIDEO_CODING_KHR:
          disp->CmdBeginVideoCodingKHR(commandBuffer
             , cmd->u.begin_video_coding_khr.begin_info          );
          break;
#endif // VK_ENABLE_BETA_EXTENSIONS
#ifdef VK_ENABLE_BETA_EXTENSIONS
      case VK_CMD_CONTROL_VIDEO_CODING_KHR:
          disp->CmdControlVideoCodingKHR(commandBuffer
             , cmd->u.control_video_coding_khr.coding_control_info          );
          break;
#endif // VK_ENABLE_BETA_EXTENSIONS
#ifdef VK_ENABLE_BETA_EXTENSIONS
      case VK_CMD_END_VIDEO_CODING_KHR:
          disp->CmdEndVideoCodingKHR(commandBuffer
             , cmd->u.end_video_coding_khr.end_coding_info          );
          break;
#endif // VK_ENABLE_BETA_EXTENSIONS
#ifdef VK_ENABLE_BETA_EXTENSIONS
      case VK_CMD_ENCODE_VIDEO_KHR:
          disp->CmdEncodeVideoKHR(commandBuffer
             , cmd->u.encode_video_khr.encode_info          );
          break;
#endif // VK_ENABLE_BETA_EXTENSIONS
      case VK_CMD_CU_LAUNCH_KERNEL_NVX:
          disp->CmdCuLaunchKernelNVX(commandBuffer
             , cmd->u.cu_launch_kernel_nvx.launch_info          );
          break;
      case VK_CMD_BEGIN_RENDERING:
          disp->CmdBeginRendering(commandBuffer
             , cmd->u.begin_rendering.rendering_info          );
          break;
      case VK_CMD_END_RENDERING:
          disp->CmdEndRendering(commandBuffer
          );
          break;
      case VK_CMD_BUILD_MICROMAPS_EXT:
          disp->CmdBuildMicromapsEXT(commandBuffer
             , cmd->u.build_micromaps_ext.info_count             , cmd->u.build_micromaps_ext.infos          );
          break;
      case VK_CMD_COPY_MICROMAP_EXT:
          disp->CmdCopyMicromapEXT(commandBuffer
             , cmd->u.copy_micromap_ext.info          );
          break;
      case VK_CMD_COPY_MICROMAP_TO_MEMORY_EXT:
          disp->CmdCopyMicromapToMemoryEXT(commandBuffer
             , cmd->u.copy_micromap_to_memory_ext.info          );
          break;
      case VK_CMD_COPY_MEMORY_TO_MICROMAP_EXT:
          disp->CmdCopyMemoryToMicromapEXT(commandBuffer
             , cmd->u.copy_memory_to_micromap_ext.info          );
          break;
      case VK_CMD_WRITE_MICROMAPS_PROPERTIES_EXT:
          disp->CmdWriteMicromapsPropertiesEXT(commandBuffer
             , cmd->u.write_micromaps_properties_ext.micromap_count             , cmd->u.write_micromaps_properties_ext.micromaps             , cmd->u.write_micromaps_properties_ext.query_type             , cmd->u.write_micromaps_properties_ext.query_pool             , cmd->u.write_micromaps_properties_ext.first_query          );
          break;
      case VK_CMD_OPTICAL_FLOW_EXECUTE_NV:
          disp->CmdOpticalFlowExecuteNV(commandBuffer
             , cmd->u.optical_flow_execute_nv.session             , cmd->u.optical_flow_execute_nv.execute_info          );
          break;
      default: unreachable("Unsupported command");
      }
   }
}




VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdBindPipeline(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipeline pipeline)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_bind_pipeline(&cmd_buffer->cmd_queue,
                                       pipelineBindPoint, pipeline);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdBindPipeline(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipeline pipeline)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdBindPipeline(commandBuffer, pipelineBindPoint, pipeline);
   } else {
      vk_cmd_enqueue_CmdBindPipeline(commandBuffer, pipelineBindPoint, pipeline);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdSetViewport(VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const VkViewport* pViewports)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_set_viewport(&cmd_buffer->cmd_queue,
                                       firstViewport, viewportCount, pViewports);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdSetViewport(VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const VkViewport* pViewports)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdSetViewport(commandBuffer, firstViewport, viewportCount, pViewports);
   } else {
      vk_cmd_enqueue_CmdSetViewport(commandBuffer, firstViewport, viewportCount, pViewports);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdSetScissor(VkCommandBuffer commandBuffer, uint32_t firstScissor, uint32_t scissorCount, const VkRect2D* pScissors)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_set_scissor(&cmd_buffer->cmd_queue,
                                       firstScissor, scissorCount, pScissors);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdSetScissor(VkCommandBuffer commandBuffer, uint32_t firstScissor, uint32_t scissorCount, const VkRect2D* pScissors)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdSetScissor(commandBuffer, firstScissor, scissorCount, pScissors);
   } else {
      vk_cmd_enqueue_CmdSetScissor(commandBuffer, firstScissor, scissorCount, pScissors);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdSetLineWidth(VkCommandBuffer commandBuffer, float lineWidth)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_set_line_width(&cmd_buffer->cmd_queue,
                                       lineWidth);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdSetLineWidth(VkCommandBuffer commandBuffer, float lineWidth)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdSetLineWidth(commandBuffer, lineWidth);
   } else {
      vk_cmd_enqueue_CmdSetLineWidth(commandBuffer, lineWidth);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdSetDepthBias(VkCommandBuffer commandBuffer, float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_set_depth_bias(&cmd_buffer->cmd_queue,
                                       depthBiasConstantFactor, depthBiasClamp, depthBiasSlopeFactor);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdSetDepthBias(VkCommandBuffer commandBuffer, float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdSetDepthBias(commandBuffer, depthBiasConstantFactor, depthBiasClamp, depthBiasSlopeFactor);
   } else {
      vk_cmd_enqueue_CmdSetDepthBias(commandBuffer, depthBiasConstantFactor, depthBiasClamp, depthBiasSlopeFactor);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdSetBlendConstants(VkCommandBuffer commandBuffer, const float blendConstants[4])
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_set_blend_constants(&cmd_buffer->cmd_queue,
                                       blendConstants);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdSetBlendConstants(VkCommandBuffer commandBuffer, const float blendConstants[4])
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdSetBlendConstants(commandBuffer, blendConstants);
   } else {
      vk_cmd_enqueue_CmdSetBlendConstants(commandBuffer, blendConstants);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdSetDepthBounds(VkCommandBuffer commandBuffer, float minDepthBounds, float maxDepthBounds)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_set_depth_bounds(&cmd_buffer->cmd_queue,
                                       minDepthBounds, maxDepthBounds);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdSetDepthBounds(VkCommandBuffer commandBuffer, float minDepthBounds, float maxDepthBounds)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdSetDepthBounds(commandBuffer, minDepthBounds, maxDepthBounds);
   } else {
      vk_cmd_enqueue_CmdSetDepthBounds(commandBuffer, minDepthBounds, maxDepthBounds);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdSetStencilCompareMask(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t compareMask)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_set_stencil_compare_mask(&cmd_buffer->cmd_queue,
                                       faceMask, compareMask);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdSetStencilCompareMask(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t compareMask)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdSetStencilCompareMask(commandBuffer, faceMask, compareMask);
   } else {
      vk_cmd_enqueue_CmdSetStencilCompareMask(commandBuffer, faceMask, compareMask);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdSetStencilWriteMask(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t writeMask)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_set_stencil_write_mask(&cmd_buffer->cmd_queue,
                                       faceMask, writeMask);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdSetStencilWriteMask(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t writeMask)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdSetStencilWriteMask(commandBuffer, faceMask, writeMask);
   } else {
      vk_cmd_enqueue_CmdSetStencilWriteMask(commandBuffer, faceMask, writeMask);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdSetStencilReference(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t reference)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_set_stencil_reference(&cmd_buffer->cmd_queue,
                                       faceMask, reference);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdSetStencilReference(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t reference)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdSetStencilReference(commandBuffer, faceMask, reference);
   } else {
      vk_cmd_enqueue_CmdSetStencilReference(commandBuffer, faceMask, reference);
   }
}



/* vk_cmd_enqueue_CmdBindDescriptorSets() is hand-typed in vk_cmd_enqueue.c */

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdBindDescriptorSets(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t firstSet, uint32_t descriptorSetCount, const VkDescriptorSet* pDescriptorSets, uint32_t dynamicOffsetCount, const uint32_t* pDynamicOffsets)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdBindDescriptorSets(commandBuffer, pipelineBindPoint, layout, firstSet, descriptorSetCount, pDescriptorSets, dynamicOffsetCount, pDynamicOffsets);
   } else {
      vk_cmd_enqueue_CmdBindDescriptorSets(commandBuffer, pipelineBindPoint, layout, firstSet, descriptorSetCount, pDescriptorSets, dynamicOffsetCount, pDynamicOffsets);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdBindIndexBuffer(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkIndexType indexType)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_bind_index_buffer(&cmd_buffer->cmd_queue,
                                       buffer, offset, indexType);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdBindIndexBuffer(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkIndexType indexType)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdBindIndexBuffer(commandBuffer, buffer, offset, indexType);
   } else {
      vk_cmd_enqueue_CmdBindIndexBuffer(commandBuffer, buffer, offset, indexType);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdBindVertexBuffers(VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_bind_vertex_buffers(&cmd_buffer->cmd_queue,
                                       firstBinding, bindingCount, pBuffers, pOffsets);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdBindVertexBuffers(VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdBindVertexBuffers(commandBuffer, firstBinding, bindingCount, pBuffers, pOffsets);
   } else {
      vk_cmd_enqueue_CmdBindVertexBuffers(commandBuffer, firstBinding, bindingCount, pBuffers, pOffsets);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdDraw(VkCommandBuffer commandBuffer, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_draw(&cmd_buffer->cmd_queue,
                                       vertexCount, instanceCount, firstVertex, firstInstance);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdDraw(VkCommandBuffer commandBuffer, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdDraw(commandBuffer, vertexCount, instanceCount, firstVertex, firstInstance);
   } else {
      vk_cmd_enqueue_CmdDraw(commandBuffer, vertexCount, instanceCount, firstVertex, firstInstance);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdDrawIndexed(VkCommandBuffer commandBuffer, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_draw_indexed(&cmd_buffer->cmd_queue,
                                       indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdDrawIndexed(VkCommandBuffer commandBuffer, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdDrawIndexed(commandBuffer, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
   } else {
      vk_cmd_enqueue_CmdDrawIndexed(commandBuffer, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
   }
}



/* vk_cmd_enqueue_CmdDrawMultiEXT() is hand-typed in vk_cmd_enqueue.c */

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdDrawMultiEXT(VkCommandBuffer commandBuffer, uint32_t drawCount, const VkMultiDrawInfoEXT* pVertexInfo, uint32_t instanceCount, uint32_t firstInstance, uint32_t stride)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdDrawMultiEXT(commandBuffer, drawCount, pVertexInfo, instanceCount, firstInstance, stride);
   } else {
      vk_cmd_enqueue_CmdDrawMultiEXT(commandBuffer, drawCount, pVertexInfo, instanceCount, firstInstance, stride);
   }
}



/* vk_cmd_enqueue_CmdDrawMultiIndexedEXT() is hand-typed in vk_cmd_enqueue.c */

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdDrawMultiIndexedEXT(VkCommandBuffer commandBuffer, uint32_t drawCount, const VkMultiDrawIndexedInfoEXT* pIndexInfo, uint32_t instanceCount, uint32_t firstInstance, uint32_t stride, const int32_t* pVertexOffset)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdDrawMultiIndexedEXT(commandBuffer, drawCount, pIndexInfo, instanceCount, firstInstance, stride, pVertexOffset);
   } else {
      vk_cmd_enqueue_CmdDrawMultiIndexedEXT(commandBuffer, drawCount, pIndexInfo, instanceCount, firstInstance, stride, pVertexOffset);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdDrawIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_draw_indirect(&cmd_buffer->cmd_queue,
                                       buffer, offset, drawCount, stride);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdDrawIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdDrawIndirect(commandBuffer, buffer, offset, drawCount, stride);
   } else {
      vk_cmd_enqueue_CmdDrawIndirect(commandBuffer, buffer, offset, drawCount, stride);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdDrawIndexedIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_draw_indexed_indirect(&cmd_buffer->cmd_queue,
                                       buffer, offset, drawCount, stride);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdDrawIndexedIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdDrawIndexedIndirect(commandBuffer, buffer, offset, drawCount, stride);
   } else {
      vk_cmd_enqueue_CmdDrawIndexedIndirect(commandBuffer, buffer, offset, drawCount, stride);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdDispatch(VkCommandBuffer commandBuffer, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_dispatch(&cmd_buffer->cmd_queue,
                                       groupCountX, groupCountY, groupCountZ);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdDispatch(VkCommandBuffer commandBuffer, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdDispatch(commandBuffer, groupCountX, groupCountY, groupCountZ);
   } else {
      vk_cmd_enqueue_CmdDispatch(commandBuffer, groupCountX, groupCountY, groupCountZ);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdDispatchIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_dispatch_indirect(&cmd_buffer->cmd_queue,
                                       buffer, offset);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdDispatchIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdDispatchIndirect(commandBuffer, buffer, offset);
   } else {
      vk_cmd_enqueue_CmdDispatchIndirect(commandBuffer, buffer, offset);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdSubpassShadingHUAWEI(VkCommandBuffer commandBuffer)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_subpass_shading_huawei(&cmd_buffer->cmd_queue);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdSubpassShadingHUAWEI(VkCommandBuffer commandBuffer)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdSubpassShadingHUAWEI(commandBuffer);
   } else {
      vk_cmd_enqueue_CmdSubpassShadingHUAWEI(commandBuffer);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdCopyBuffer(VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkBuffer dstBuffer, uint32_t regionCount, const VkBufferCopy* pRegions)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_copy_buffer(&cmd_buffer->cmd_queue,
                                       srcBuffer, dstBuffer, regionCount, pRegions);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdCopyBuffer(VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkBuffer dstBuffer, uint32_t regionCount, const VkBufferCopy* pRegions)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, regionCount, pRegions);
   } else {
      vk_cmd_enqueue_CmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, regionCount, pRegions);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdCopyImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageCopy* pRegions)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_copy_image(&cmd_buffer->cmd_queue,
                                       srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdCopyImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageCopy* pRegions)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdCopyImage(commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions);
   } else {
      vk_cmd_enqueue_CmdCopyImage(commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdBlitImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageBlit* pRegions, VkFilter filter)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_blit_image(&cmd_buffer->cmd_queue,
                                       srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions, filter);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdBlitImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageBlit* pRegions, VkFilter filter)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdBlitImage(commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions, filter);
   } else {
      vk_cmd_enqueue_CmdBlitImage(commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions, filter);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdCopyBufferToImage(VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkBufferImageCopy* pRegions)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_copy_buffer_to_image(&cmd_buffer->cmd_queue,
                                       srcBuffer, dstImage, dstImageLayout, regionCount, pRegions);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdCopyBufferToImage(VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkBufferImageCopy* pRegions)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdCopyBufferToImage(commandBuffer, srcBuffer, dstImage, dstImageLayout, regionCount, pRegions);
   } else {
      vk_cmd_enqueue_CmdCopyBufferToImage(commandBuffer, srcBuffer, dstImage, dstImageLayout, regionCount, pRegions);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdCopyImageToBuffer(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkBuffer dstBuffer, uint32_t regionCount, const VkBufferImageCopy* pRegions)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_copy_image_to_buffer(&cmd_buffer->cmd_queue,
                                       srcImage, srcImageLayout, dstBuffer, regionCount, pRegions);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdCopyImageToBuffer(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkBuffer dstBuffer, uint32_t regionCount, const VkBufferImageCopy* pRegions)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdCopyImageToBuffer(commandBuffer, srcImage, srcImageLayout, dstBuffer, regionCount, pRegions);
   } else {
      vk_cmd_enqueue_CmdCopyImageToBuffer(commandBuffer, srcImage, srcImageLayout, dstBuffer, regionCount, pRegions);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdUpdateBuffer(VkCommandBuffer commandBuffer, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize dataSize, const void* pData)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_update_buffer(&cmd_buffer->cmd_queue,
                                       dstBuffer, dstOffset, dataSize, pData);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdUpdateBuffer(VkCommandBuffer commandBuffer, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize dataSize, const void* pData)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdUpdateBuffer(commandBuffer, dstBuffer, dstOffset, dataSize, pData);
   } else {
      vk_cmd_enqueue_CmdUpdateBuffer(commandBuffer, dstBuffer, dstOffset, dataSize, pData);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdFillBuffer(VkCommandBuffer commandBuffer, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize size, uint32_t data)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_fill_buffer(&cmd_buffer->cmd_queue,
                                       dstBuffer, dstOffset, size, data);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdFillBuffer(VkCommandBuffer commandBuffer, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize size, uint32_t data)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdFillBuffer(commandBuffer, dstBuffer, dstOffset, size, data);
   } else {
      vk_cmd_enqueue_CmdFillBuffer(commandBuffer, dstBuffer, dstOffset, size, data);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdClearColorImage(VkCommandBuffer commandBuffer, VkImage image, VkImageLayout imageLayout, const VkClearColorValue* pColor, uint32_t rangeCount, const VkImageSubresourceRange* pRanges)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_clear_color_image(&cmd_buffer->cmd_queue,
                                       image, imageLayout, pColor, rangeCount, pRanges);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdClearColorImage(VkCommandBuffer commandBuffer, VkImage image, VkImageLayout imageLayout, const VkClearColorValue* pColor, uint32_t rangeCount, const VkImageSubresourceRange* pRanges)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdClearColorImage(commandBuffer, image, imageLayout, pColor, rangeCount, pRanges);
   } else {
      vk_cmd_enqueue_CmdClearColorImage(commandBuffer, image, imageLayout, pColor, rangeCount, pRanges);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdClearDepthStencilImage(VkCommandBuffer commandBuffer, VkImage image, VkImageLayout imageLayout, const VkClearDepthStencilValue* pDepthStencil, uint32_t rangeCount, const VkImageSubresourceRange* pRanges)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_clear_depth_stencil_image(&cmd_buffer->cmd_queue,
                                       image, imageLayout, pDepthStencil, rangeCount, pRanges);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdClearDepthStencilImage(VkCommandBuffer commandBuffer, VkImage image, VkImageLayout imageLayout, const VkClearDepthStencilValue* pDepthStencil, uint32_t rangeCount, const VkImageSubresourceRange* pRanges)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdClearDepthStencilImage(commandBuffer, image, imageLayout, pDepthStencil, rangeCount, pRanges);
   } else {
      vk_cmd_enqueue_CmdClearDepthStencilImage(commandBuffer, image, imageLayout, pDepthStencil, rangeCount, pRanges);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdClearAttachments(VkCommandBuffer commandBuffer, uint32_t attachmentCount, const VkClearAttachment* pAttachments, uint32_t rectCount, const VkClearRect* pRects)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_clear_attachments(&cmd_buffer->cmd_queue,
                                       attachmentCount, pAttachments, rectCount, pRects);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdClearAttachments(VkCommandBuffer commandBuffer, uint32_t attachmentCount, const VkClearAttachment* pAttachments, uint32_t rectCount, const VkClearRect* pRects)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdClearAttachments(commandBuffer, attachmentCount, pAttachments, rectCount, pRects);
   } else {
      vk_cmd_enqueue_CmdClearAttachments(commandBuffer, attachmentCount, pAttachments, rectCount, pRects);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdResolveImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageResolve* pRegions)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_resolve_image(&cmd_buffer->cmd_queue,
                                       srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdResolveImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageResolve* pRegions)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdResolveImage(commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions);
   } else {
      vk_cmd_enqueue_CmdResolveImage(commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdSetEvent(VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags stageMask)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_set_event(&cmd_buffer->cmd_queue,
                                       event, stageMask);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdSetEvent(VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags stageMask)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdSetEvent(commandBuffer, event, stageMask);
   } else {
      vk_cmd_enqueue_CmdSetEvent(commandBuffer, event, stageMask);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdResetEvent(VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags stageMask)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_reset_event(&cmd_buffer->cmd_queue,
                                       event, stageMask);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdResetEvent(VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags stageMask)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdResetEvent(commandBuffer, event, stageMask);
   } else {
      vk_cmd_enqueue_CmdResetEvent(commandBuffer, event, stageMask);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdWaitEvents(VkCommandBuffer commandBuffer, uint32_t eventCount, const VkEvent* pEvents, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier* pImageMemoryBarriers)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_wait_events(&cmd_buffer->cmd_queue,
                                       eventCount, pEvents, srcStageMask, dstStageMask, memoryBarrierCount, pMemoryBarriers, bufferMemoryBarrierCount, pBufferMemoryBarriers, imageMemoryBarrierCount, pImageMemoryBarriers);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdWaitEvents(VkCommandBuffer commandBuffer, uint32_t eventCount, const VkEvent* pEvents, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier* pImageMemoryBarriers)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdWaitEvents(commandBuffer, eventCount, pEvents, srcStageMask, dstStageMask, memoryBarrierCount, pMemoryBarriers, bufferMemoryBarrierCount, pBufferMemoryBarriers, imageMemoryBarrierCount, pImageMemoryBarriers);
   } else {
      vk_cmd_enqueue_CmdWaitEvents(commandBuffer, eventCount, pEvents, srcStageMask, dstStageMask, memoryBarrierCount, pMemoryBarriers, bufferMemoryBarrierCount, pBufferMemoryBarriers, imageMemoryBarrierCount, pImageMemoryBarriers);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdPipelineBarrier(VkCommandBuffer commandBuffer, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, VkDependencyFlags dependencyFlags, uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier* pImageMemoryBarriers)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_pipeline_barrier(&cmd_buffer->cmd_queue,
                                       srcStageMask, dstStageMask, dependencyFlags, memoryBarrierCount, pMemoryBarriers, bufferMemoryBarrierCount, pBufferMemoryBarriers, imageMemoryBarrierCount, pImageMemoryBarriers);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdPipelineBarrier(VkCommandBuffer commandBuffer, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, VkDependencyFlags dependencyFlags, uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier* pImageMemoryBarriers)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdPipelineBarrier(commandBuffer, srcStageMask, dstStageMask, dependencyFlags, memoryBarrierCount, pMemoryBarriers, bufferMemoryBarrierCount, pBufferMemoryBarriers, imageMemoryBarrierCount, pImageMemoryBarriers);
   } else {
      vk_cmd_enqueue_CmdPipelineBarrier(commandBuffer, srcStageMask, dstStageMask, dependencyFlags, memoryBarrierCount, pMemoryBarriers, bufferMemoryBarrierCount, pBufferMemoryBarriers, imageMemoryBarrierCount, pImageMemoryBarriers);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdBeginQuery(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, VkQueryControlFlags flags)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_begin_query(&cmd_buffer->cmd_queue,
                                       queryPool, query, flags);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdBeginQuery(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, VkQueryControlFlags flags)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdBeginQuery(commandBuffer, queryPool, query, flags);
   } else {
      vk_cmd_enqueue_CmdBeginQuery(commandBuffer, queryPool, query, flags);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdEndQuery(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_end_query(&cmd_buffer->cmd_queue,
                                       queryPool, query);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdEndQuery(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdEndQuery(commandBuffer, queryPool, query);
   } else {
      vk_cmd_enqueue_CmdEndQuery(commandBuffer, queryPool, query);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdBeginConditionalRenderingEXT(VkCommandBuffer commandBuffer, const VkConditionalRenderingBeginInfoEXT* pConditionalRenderingBegin)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_begin_conditional_rendering_ext(&cmd_buffer->cmd_queue,
                                       pConditionalRenderingBegin);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdBeginConditionalRenderingEXT(VkCommandBuffer commandBuffer, const VkConditionalRenderingBeginInfoEXT* pConditionalRenderingBegin)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdBeginConditionalRenderingEXT(commandBuffer, pConditionalRenderingBegin);
   } else {
      vk_cmd_enqueue_CmdBeginConditionalRenderingEXT(commandBuffer, pConditionalRenderingBegin);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdEndConditionalRenderingEXT(VkCommandBuffer commandBuffer)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_end_conditional_rendering_ext(&cmd_buffer->cmd_queue);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdEndConditionalRenderingEXT(VkCommandBuffer commandBuffer)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdEndConditionalRenderingEXT(commandBuffer);
   } else {
      vk_cmd_enqueue_CmdEndConditionalRenderingEXT(commandBuffer);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdResetQueryPool(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_reset_query_pool(&cmd_buffer->cmd_queue,
                                       queryPool, firstQuery, queryCount);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdResetQueryPool(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdResetQueryPool(commandBuffer, queryPool, firstQuery, queryCount);
   } else {
      vk_cmd_enqueue_CmdResetQueryPool(commandBuffer, queryPool, firstQuery, queryCount);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdWriteTimestamp(VkCommandBuffer commandBuffer, VkPipelineStageFlagBits pipelineStage, VkQueryPool queryPool, uint32_t query)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_write_timestamp(&cmd_buffer->cmd_queue,
                                       pipelineStage, queryPool, query);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdWriteTimestamp(VkCommandBuffer commandBuffer, VkPipelineStageFlagBits pipelineStage, VkQueryPool queryPool, uint32_t query)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdWriteTimestamp(commandBuffer, pipelineStage, queryPool, query);
   } else {
      vk_cmd_enqueue_CmdWriteTimestamp(commandBuffer, pipelineStage, queryPool, query);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdCopyQueryPoolResults(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize stride, VkQueryResultFlags flags)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_copy_query_pool_results(&cmd_buffer->cmd_queue,
                                       queryPool, firstQuery, queryCount, dstBuffer, dstOffset, stride, flags);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdCopyQueryPoolResults(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize stride, VkQueryResultFlags flags)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdCopyQueryPoolResults(commandBuffer, queryPool, firstQuery, queryCount, dstBuffer, dstOffset, stride, flags);
   } else {
      vk_cmd_enqueue_CmdCopyQueryPoolResults(commandBuffer, queryPool, firstQuery, queryCount, dstBuffer, dstOffset, stride, flags);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdPushConstants(VkCommandBuffer commandBuffer, VkPipelineLayout layout, VkShaderStageFlags stageFlags, uint32_t offset, uint32_t size, const void* pValues)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_push_constants(&cmd_buffer->cmd_queue,
                                       layout, stageFlags, offset, size, pValues);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdPushConstants(VkCommandBuffer commandBuffer, VkPipelineLayout layout, VkShaderStageFlags stageFlags, uint32_t offset, uint32_t size, const void* pValues)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdPushConstants(commandBuffer, layout, stageFlags, offset, size, pValues);
   } else {
      vk_cmd_enqueue_CmdPushConstants(commandBuffer, layout, stageFlags, offset, size, pValues);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdBeginRenderPass(VkCommandBuffer commandBuffer, const VkRenderPassBeginInfo* pRenderPassBegin, VkSubpassContents contents)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_begin_render_pass(&cmd_buffer->cmd_queue,
                                       pRenderPassBegin, contents);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdBeginRenderPass(VkCommandBuffer commandBuffer, const VkRenderPassBeginInfo* pRenderPassBegin, VkSubpassContents contents)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdBeginRenderPass(commandBuffer, pRenderPassBegin, contents);
   } else {
      vk_cmd_enqueue_CmdBeginRenderPass(commandBuffer, pRenderPassBegin, contents);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdNextSubpass(VkCommandBuffer commandBuffer, VkSubpassContents contents)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_next_subpass(&cmd_buffer->cmd_queue,
                                       contents);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdNextSubpass(VkCommandBuffer commandBuffer, VkSubpassContents contents)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdNextSubpass(commandBuffer, contents);
   } else {
      vk_cmd_enqueue_CmdNextSubpass(commandBuffer, contents);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdEndRenderPass(VkCommandBuffer commandBuffer)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_end_render_pass(&cmd_buffer->cmd_queue);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdEndRenderPass(VkCommandBuffer commandBuffer)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdEndRenderPass(commandBuffer);
   } else {
      vk_cmd_enqueue_CmdEndRenderPass(commandBuffer);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdExecuteCommands(VkCommandBuffer commandBuffer, uint32_t commandBufferCount, const VkCommandBuffer* pCommandBuffers)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_execute_commands(&cmd_buffer->cmd_queue,
                                       commandBufferCount, pCommandBuffers);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdExecuteCommands(VkCommandBuffer commandBuffer, uint32_t commandBufferCount, const VkCommandBuffer* pCommandBuffers)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdExecuteCommands(commandBuffer, commandBufferCount, pCommandBuffers);
   } else {
      vk_cmd_enqueue_CmdExecuteCommands(commandBuffer, commandBufferCount, pCommandBuffers);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdDebugMarkerBeginEXT(VkCommandBuffer commandBuffer, const VkDebugMarkerMarkerInfoEXT* pMarkerInfo)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_debug_marker_begin_ext(&cmd_buffer->cmd_queue,
                                       pMarkerInfo);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdDebugMarkerBeginEXT(VkCommandBuffer commandBuffer, const VkDebugMarkerMarkerInfoEXT* pMarkerInfo)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdDebugMarkerBeginEXT(commandBuffer, pMarkerInfo);
   } else {
      vk_cmd_enqueue_CmdDebugMarkerBeginEXT(commandBuffer, pMarkerInfo);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdDebugMarkerEndEXT(VkCommandBuffer commandBuffer)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_debug_marker_end_ext(&cmd_buffer->cmd_queue);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdDebugMarkerEndEXT(VkCommandBuffer commandBuffer)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdDebugMarkerEndEXT(commandBuffer);
   } else {
      vk_cmd_enqueue_CmdDebugMarkerEndEXT(commandBuffer);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdDebugMarkerInsertEXT(VkCommandBuffer commandBuffer, const VkDebugMarkerMarkerInfoEXT* pMarkerInfo)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_debug_marker_insert_ext(&cmd_buffer->cmd_queue,
                                       pMarkerInfo);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdDebugMarkerInsertEXT(VkCommandBuffer commandBuffer, const VkDebugMarkerMarkerInfoEXT* pMarkerInfo)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdDebugMarkerInsertEXT(commandBuffer, pMarkerInfo);
   } else {
      vk_cmd_enqueue_CmdDebugMarkerInsertEXT(commandBuffer, pMarkerInfo);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdExecuteGeneratedCommandsNV(VkCommandBuffer commandBuffer, VkBool32 isPreprocessed, const VkGeneratedCommandsInfoNV* pGeneratedCommandsInfo)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_execute_generated_commands_nv(&cmd_buffer->cmd_queue,
                                       isPreprocessed, pGeneratedCommandsInfo);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdExecuteGeneratedCommandsNV(VkCommandBuffer commandBuffer, VkBool32 isPreprocessed, const VkGeneratedCommandsInfoNV* pGeneratedCommandsInfo)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdExecuteGeneratedCommandsNV(commandBuffer, isPreprocessed, pGeneratedCommandsInfo);
   } else {
      vk_cmd_enqueue_CmdExecuteGeneratedCommandsNV(commandBuffer, isPreprocessed, pGeneratedCommandsInfo);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdPreprocessGeneratedCommandsNV(VkCommandBuffer commandBuffer, const VkGeneratedCommandsInfoNV* pGeneratedCommandsInfo)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_preprocess_generated_commands_nv(&cmd_buffer->cmd_queue,
                                       pGeneratedCommandsInfo);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdPreprocessGeneratedCommandsNV(VkCommandBuffer commandBuffer, const VkGeneratedCommandsInfoNV* pGeneratedCommandsInfo)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdPreprocessGeneratedCommandsNV(commandBuffer, pGeneratedCommandsInfo);
   } else {
      vk_cmd_enqueue_CmdPreprocessGeneratedCommandsNV(commandBuffer, pGeneratedCommandsInfo);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdBindPipelineShaderGroupNV(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipeline pipeline, uint32_t groupIndex)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_bind_pipeline_shader_group_nv(&cmd_buffer->cmd_queue,
                                       pipelineBindPoint, pipeline, groupIndex);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdBindPipelineShaderGroupNV(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipeline pipeline, uint32_t groupIndex)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdBindPipelineShaderGroupNV(commandBuffer, pipelineBindPoint, pipeline, groupIndex);
   } else {
      vk_cmd_enqueue_CmdBindPipelineShaderGroupNV(commandBuffer, pipelineBindPoint, pipeline, groupIndex);
   }
}



/* vk_cmd_enqueue_CmdPushDescriptorSetKHR() is hand-typed in vk_cmd_enqueue.c */

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdPushDescriptorSetKHR(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t set, uint32_t descriptorWriteCount, const VkWriteDescriptorSet* pDescriptorWrites)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdPushDescriptorSetKHR(commandBuffer, pipelineBindPoint, layout, set, descriptorWriteCount, pDescriptorWrites);
   } else {
      vk_cmd_enqueue_CmdPushDescriptorSetKHR(commandBuffer, pipelineBindPoint, layout, set, descriptorWriteCount, pDescriptorWrites);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdSetDeviceMask(VkCommandBuffer commandBuffer, uint32_t deviceMask)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_set_device_mask(&cmd_buffer->cmd_queue,
                                       deviceMask);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdSetDeviceMask(VkCommandBuffer commandBuffer, uint32_t deviceMask)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdSetDeviceMask(commandBuffer, deviceMask);
   } else {
      vk_cmd_enqueue_CmdSetDeviceMask(commandBuffer, deviceMask);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdDispatchBase(VkCommandBuffer commandBuffer, uint32_t baseGroupX, uint32_t baseGroupY, uint32_t baseGroupZ, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_dispatch_base(&cmd_buffer->cmd_queue,
                                       baseGroupX, baseGroupY, baseGroupZ, groupCountX, groupCountY, groupCountZ);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdDispatchBase(VkCommandBuffer commandBuffer, uint32_t baseGroupX, uint32_t baseGroupY, uint32_t baseGroupZ, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdDispatchBase(commandBuffer, baseGroupX, baseGroupY, baseGroupZ, groupCountX, groupCountY, groupCountZ);
   } else {
      vk_cmd_enqueue_CmdDispatchBase(commandBuffer, baseGroupX, baseGroupY, baseGroupZ, groupCountX, groupCountY, groupCountZ);
   }
}
/* TODO: Generate vk_cmd_enqueue_CmdPushDescriptorSetWithTemplateKHR() */



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdSetViewportWScalingNV(VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const VkViewportWScalingNV* pViewportWScalings)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_set_viewport_wscaling_nv(&cmd_buffer->cmd_queue,
                                       firstViewport, viewportCount, pViewportWScalings);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdSetViewportWScalingNV(VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const VkViewportWScalingNV* pViewportWScalings)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdSetViewportWScalingNV(commandBuffer, firstViewport, viewportCount, pViewportWScalings);
   } else {
      vk_cmd_enqueue_CmdSetViewportWScalingNV(commandBuffer, firstViewport, viewportCount, pViewportWScalings);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdSetDiscardRectangleEXT(VkCommandBuffer commandBuffer, uint32_t firstDiscardRectangle, uint32_t discardRectangleCount, const VkRect2D* pDiscardRectangles)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_set_discard_rectangle_ext(&cmd_buffer->cmd_queue,
                                       firstDiscardRectangle, discardRectangleCount, pDiscardRectangles);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdSetDiscardRectangleEXT(VkCommandBuffer commandBuffer, uint32_t firstDiscardRectangle, uint32_t discardRectangleCount, const VkRect2D* pDiscardRectangles)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdSetDiscardRectangleEXT(commandBuffer, firstDiscardRectangle, discardRectangleCount, pDiscardRectangles);
   } else {
      vk_cmd_enqueue_CmdSetDiscardRectangleEXT(commandBuffer, firstDiscardRectangle, discardRectangleCount, pDiscardRectangles);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdSetSampleLocationsEXT(VkCommandBuffer commandBuffer, const VkSampleLocationsInfoEXT* pSampleLocationsInfo)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_set_sample_locations_ext(&cmd_buffer->cmd_queue,
                                       pSampleLocationsInfo);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdSetSampleLocationsEXT(VkCommandBuffer commandBuffer, const VkSampleLocationsInfoEXT* pSampleLocationsInfo)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdSetSampleLocationsEXT(commandBuffer, pSampleLocationsInfo);
   } else {
      vk_cmd_enqueue_CmdSetSampleLocationsEXT(commandBuffer, pSampleLocationsInfo);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdBeginDebugUtilsLabelEXT(VkCommandBuffer commandBuffer, const VkDebugUtilsLabelEXT* pLabelInfo)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_begin_debug_utils_label_ext(&cmd_buffer->cmd_queue,
                                       pLabelInfo);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdBeginDebugUtilsLabelEXT(VkCommandBuffer commandBuffer, const VkDebugUtilsLabelEXT* pLabelInfo)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdBeginDebugUtilsLabelEXT(commandBuffer, pLabelInfo);
   } else {
      vk_cmd_enqueue_CmdBeginDebugUtilsLabelEXT(commandBuffer, pLabelInfo);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdEndDebugUtilsLabelEXT(VkCommandBuffer commandBuffer)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_end_debug_utils_label_ext(&cmd_buffer->cmd_queue);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdEndDebugUtilsLabelEXT(VkCommandBuffer commandBuffer)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdEndDebugUtilsLabelEXT(commandBuffer);
   } else {
      vk_cmd_enqueue_CmdEndDebugUtilsLabelEXT(commandBuffer);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdInsertDebugUtilsLabelEXT(VkCommandBuffer commandBuffer, const VkDebugUtilsLabelEXT* pLabelInfo)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_insert_debug_utils_label_ext(&cmd_buffer->cmd_queue,
                                       pLabelInfo);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdInsertDebugUtilsLabelEXT(VkCommandBuffer commandBuffer, const VkDebugUtilsLabelEXT* pLabelInfo)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdInsertDebugUtilsLabelEXT(commandBuffer, pLabelInfo);
   } else {
      vk_cmd_enqueue_CmdInsertDebugUtilsLabelEXT(commandBuffer, pLabelInfo);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdWriteBufferMarkerAMD(VkCommandBuffer commandBuffer, VkPipelineStageFlagBits pipelineStage, VkBuffer dstBuffer, VkDeviceSize dstOffset, uint32_t marker)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_write_buffer_marker_amd(&cmd_buffer->cmd_queue,
                                       pipelineStage, dstBuffer, dstOffset, marker);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdWriteBufferMarkerAMD(VkCommandBuffer commandBuffer, VkPipelineStageFlagBits pipelineStage, VkBuffer dstBuffer, VkDeviceSize dstOffset, uint32_t marker)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdWriteBufferMarkerAMD(commandBuffer, pipelineStage, dstBuffer, dstOffset, marker);
   } else {
      vk_cmd_enqueue_CmdWriteBufferMarkerAMD(commandBuffer, pipelineStage, dstBuffer, dstOffset, marker);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdBeginRenderPass2(VkCommandBuffer commandBuffer, const VkRenderPassBeginInfo*      pRenderPassBegin, const VkSubpassBeginInfo*      pSubpassBeginInfo)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_begin_render_pass2(&cmd_buffer->cmd_queue,
                                       pRenderPassBegin, pSubpassBeginInfo);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdBeginRenderPass2(VkCommandBuffer commandBuffer, const VkRenderPassBeginInfo*      pRenderPassBegin, const VkSubpassBeginInfo*      pSubpassBeginInfo)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdBeginRenderPass2(commandBuffer, pRenderPassBegin, pSubpassBeginInfo);
   } else {
      vk_cmd_enqueue_CmdBeginRenderPass2(commandBuffer, pRenderPassBegin, pSubpassBeginInfo);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdNextSubpass2(VkCommandBuffer commandBuffer, const VkSubpassBeginInfo*      pSubpassBeginInfo, const VkSubpassEndInfo*        pSubpassEndInfo)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_next_subpass2(&cmd_buffer->cmd_queue,
                                       pSubpassBeginInfo, pSubpassEndInfo);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdNextSubpass2(VkCommandBuffer commandBuffer, const VkSubpassBeginInfo*      pSubpassBeginInfo, const VkSubpassEndInfo*        pSubpassEndInfo)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdNextSubpass2(commandBuffer, pSubpassBeginInfo, pSubpassEndInfo);
   } else {
      vk_cmd_enqueue_CmdNextSubpass2(commandBuffer, pSubpassBeginInfo, pSubpassEndInfo);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdEndRenderPass2(VkCommandBuffer commandBuffer, const VkSubpassEndInfo*        pSubpassEndInfo)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_end_render_pass2(&cmd_buffer->cmd_queue,
                                       pSubpassEndInfo);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdEndRenderPass2(VkCommandBuffer commandBuffer, const VkSubpassEndInfo*        pSubpassEndInfo)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdEndRenderPass2(commandBuffer, pSubpassEndInfo);
   } else {
      vk_cmd_enqueue_CmdEndRenderPass2(commandBuffer, pSubpassEndInfo);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdDrawIndirectCount(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_draw_indirect_count(&cmd_buffer->cmd_queue,
                                       buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdDrawIndirectCount(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdDrawIndirectCount(commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
   } else {
      vk_cmd_enqueue_CmdDrawIndirectCount(commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdDrawIndexedIndirectCount(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_draw_indexed_indirect_count(&cmd_buffer->cmd_queue,
                                       buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdDrawIndexedIndirectCount(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdDrawIndexedIndirectCount(commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
   } else {
      vk_cmd_enqueue_CmdDrawIndexedIndirectCount(commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdSetCheckpointNV(VkCommandBuffer commandBuffer, const void* pCheckpointMarker)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_set_checkpoint_nv(&cmd_buffer->cmd_queue,
                                       pCheckpointMarker);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdSetCheckpointNV(VkCommandBuffer commandBuffer, const void* pCheckpointMarker)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdSetCheckpointNV(commandBuffer, pCheckpointMarker);
   } else {
      vk_cmd_enqueue_CmdSetCheckpointNV(commandBuffer, pCheckpointMarker);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdBindTransformFeedbackBuffersEXT(VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets, const VkDeviceSize* pSizes)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_bind_transform_feedback_buffers_ext(&cmd_buffer->cmd_queue,
                                       firstBinding, bindingCount, pBuffers, pOffsets, pSizes);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdBindTransformFeedbackBuffersEXT(VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets, const VkDeviceSize* pSizes)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdBindTransformFeedbackBuffersEXT(commandBuffer, firstBinding, bindingCount, pBuffers, pOffsets, pSizes);
   } else {
      vk_cmd_enqueue_CmdBindTransformFeedbackBuffersEXT(commandBuffer, firstBinding, bindingCount, pBuffers, pOffsets, pSizes);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdBeginTransformFeedbackEXT(VkCommandBuffer commandBuffer, uint32_t firstCounterBuffer, uint32_t counterBufferCount, const VkBuffer* pCounterBuffers, const VkDeviceSize* pCounterBufferOffsets)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_begin_transform_feedback_ext(&cmd_buffer->cmd_queue,
                                       firstCounterBuffer, counterBufferCount, pCounterBuffers, pCounterBufferOffsets);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdBeginTransformFeedbackEXT(VkCommandBuffer commandBuffer, uint32_t firstCounterBuffer, uint32_t counterBufferCount, const VkBuffer* pCounterBuffers, const VkDeviceSize* pCounterBufferOffsets)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdBeginTransformFeedbackEXT(commandBuffer, firstCounterBuffer, counterBufferCount, pCounterBuffers, pCounterBufferOffsets);
   } else {
      vk_cmd_enqueue_CmdBeginTransformFeedbackEXT(commandBuffer, firstCounterBuffer, counterBufferCount, pCounterBuffers, pCounterBufferOffsets);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdEndTransformFeedbackEXT(VkCommandBuffer commandBuffer, uint32_t firstCounterBuffer, uint32_t counterBufferCount, const VkBuffer* pCounterBuffers, const VkDeviceSize* pCounterBufferOffsets)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_end_transform_feedback_ext(&cmd_buffer->cmd_queue,
                                       firstCounterBuffer, counterBufferCount, pCounterBuffers, pCounterBufferOffsets);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdEndTransformFeedbackEXT(VkCommandBuffer commandBuffer, uint32_t firstCounterBuffer, uint32_t counterBufferCount, const VkBuffer* pCounterBuffers, const VkDeviceSize* pCounterBufferOffsets)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdEndTransformFeedbackEXT(commandBuffer, firstCounterBuffer, counterBufferCount, pCounterBuffers, pCounterBufferOffsets);
   } else {
      vk_cmd_enqueue_CmdEndTransformFeedbackEXT(commandBuffer, firstCounterBuffer, counterBufferCount, pCounterBuffers, pCounterBufferOffsets);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdBeginQueryIndexedEXT(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, VkQueryControlFlags flags, uint32_t index)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_begin_query_indexed_ext(&cmd_buffer->cmd_queue,
                                       queryPool, query, flags, index);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdBeginQueryIndexedEXT(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, VkQueryControlFlags flags, uint32_t index)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdBeginQueryIndexedEXT(commandBuffer, queryPool, query, flags, index);
   } else {
      vk_cmd_enqueue_CmdBeginQueryIndexedEXT(commandBuffer, queryPool, query, flags, index);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdEndQueryIndexedEXT(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, uint32_t index)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_end_query_indexed_ext(&cmd_buffer->cmd_queue,
                                       queryPool, query, index);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdEndQueryIndexedEXT(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, uint32_t index)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdEndQueryIndexedEXT(commandBuffer, queryPool, query, index);
   } else {
      vk_cmd_enqueue_CmdEndQueryIndexedEXT(commandBuffer, queryPool, query, index);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdDrawIndirectByteCountEXT(VkCommandBuffer commandBuffer, uint32_t instanceCount, uint32_t firstInstance, VkBuffer counterBuffer, VkDeviceSize counterBufferOffset, uint32_t counterOffset, uint32_t vertexStride)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_draw_indirect_byte_count_ext(&cmd_buffer->cmd_queue,
                                       instanceCount, firstInstance, counterBuffer, counterBufferOffset, counterOffset, vertexStride);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdDrawIndirectByteCountEXT(VkCommandBuffer commandBuffer, uint32_t instanceCount, uint32_t firstInstance, VkBuffer counterBuffer, VkDeviceSize counterBufferOffset, uint32_t counterOffset, uint32_t vertexStride)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdDrawIndirectByteCountEXT(commandBuffer, instanceCount, firstInstance, counterBuffer, counterBufferOffset, counterOffset, vertexStride);
   } else {
      vk_cmd_enqueue_CmdDrawIndirectByteCountEXT(commandBuffer, instanceCount, firstInstance, counterBuffer, counterBufferOffset, counterOffset, vertexStride);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdSetExclusiveScissorNV(VkCommandBuffer commandBuffer, uint32_t firstExclusiveScissor, uint32_t exclusiveScissorCount, const VkRect2D* pExclusiveScissors)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_set_exclusive_scissor_nv(&cmd_buffer->cmd_queue,
                                       firstExclusiveScissor, exclusiveScissorCount, pExclusiveScissors);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdSetExclusiveScissorNV(VkCommandBuffer commandBuffer, uint32_t firstExclusiveScissor, uint32_t exclusiveScissorCount, const VkRect2D* pExclusiveScissors)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdSetExclusiveScissorNV(commandBuffer, firstExclusiveScissor, exclusiveScissorCount, pExclusiveScissors);
   } else {
      vk_cmd_enqueue_CmdSetExclusiveScissorNV(commandBuffer, firstExclusiveScissor, exclusiveScissorCount, pExclusiveScissors);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdBindShadingRateImageNV(VkCommandBuffer commandBuffer, VkImageView imageView, VkImageLayout imageLayout)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_bind_shading_rate_image_nv(&cmd_buffer->cmd_queue,
                                       imageView, imageLayout);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdBindShadingRateImageNV(VkCommandBuffer commandBuffer, VkImageView imageView, VkImageLayout imageLayout)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdBindShadingRateImageNV(commandBuffer, imageView, imageLayout);
   } else {
      vk_cmd_enqueue_CmdBindShadingRateImageNV(commandBuffer, imageView, imageLayout);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdSetViewportShadingRatePaletteNV(VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const VkShadingRatePaletteNV* pShadingRatePalettes)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_set_viewport_shading_rate_palette_nv(&cmd_buffer->cmd_queue,
                                       firstViewport, viewportCount, pShadingRatePalettes);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdSetViewportShadingRatePaletteNV(VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const VkShadingRatePaletteNV* pShadingRatePalettes)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdSetViewportShadingRatePaletteNV(commandBuffer, firstViewport, viewportCount, pShadingRatePalettes);
   } else {
      vk_cmd_enqueue_CmdSetViewportShadingRatePaletteNV(commandBuffer, firstViewport, viewportCount, pShadingRatePalettes);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdSetCoarseSampleOrderNV(VkCommandBuffer commandBuffer, VkCoarseSampleOrderTypeNV sampleOrderType, uint32_t customSampleOrderCount, const VkCoarseSampleOrderCustomNV* pCustomSampleOrders)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_set_coarse_sample_order_nv(&cmd_buffer->cmd_queue,
                                       sampleOrderType, customSampleOrderCount, pCustomSampleOrders);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdSetCoarseSampleOrderNV(VkCommandBuffer commandBuffer, VkCoarseSampleOrderTypeNV sampleOrderType, uint32_t customSampleOrderCount, const VkCoarseSampleOrderCustomNV* pCustomSampleOrders)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdSetCoarseSampleOrderNV(commandBuffer, sampleOrderType, customSampleOrderCount, pCustomSampleOrders);
   } else {
      vk_cmd_enqueue_CmdSetCoarseSampleOrderNV(commandBuffer, sampleOrderType, customSampleOrderCount, pCustomSampleOrders);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdDrawMeshTasksNV(VkCommandBuffer commandBuffer, uint32_t taskCount, uint32_t firstTask)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_draw_mesh_tasks_nv(&cmd_buffer->cmd_queue,
                                       taskCount, firstTask);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdDrawMeshTasksNV(VkCommandBuffer commandBuffer, uint32_t taskCount, uint32_t firstTask)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdDrawMeshTasksNV(commandBuffer, taskCount, firstTask);
   } else {
      vk_cmd_enqueue_CmdDrawMeshTasksNV(commandBuffer, taskCount, firstTask);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdDrawMeshTasksIndirectNV(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_draw_mesh_tasks_indirect_nv(&cmd_buffer->cmd_queue,
                                       buffer, offset, drawCount, stride);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdDrawMeshTasksIndirectNV(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdDrawMeshTasksIndirectNV(commandBuffer, buffer, offset, drawCount, stride);
   } else {
      vk_cmd_enqueue_CmdDrawMeshTasksIndirectNV(commandBuffer, buffer, offset, drawCount, stride);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdDrawMeshTasksIndirectCountNV(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_draw_mesh_tasks_indirect_count_nv(&cmd_buffer->cmd_queue,
                                       buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdDrawMeshTasksIndirectCountNV(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdDrawMeshTasksIndirectCountNV(commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
   } else {
      vk_cmd_enqueue_CmdDrawMeshTasksIndirectCountNV(commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdDrawMeshTasksEXT(VkCommandBuffer commandBuffer, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_draw_mesh_tasks_ext(&cmd_buffer->cmd_queue,
                                       groupCountX, groupCountY, groupCountZ);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdDrawMeshTasksEXT(VkCommandBuffer commandBuffer, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdDrawMeshTasksEXT(commandBuffer, groupCountX, groupCountY, groupCountZ);
   } else {
      vk_cmd_enqueue_CmdDrawMeshTasksEXT(commandBuffer, groupCountX, groupCountY, groupCountZ);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdDrawMeshTasksIndirectEXT(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_draw_mesh_tasks_indirect_ext(&cmd_buffer->cmd_queue,
                                       buffer, offset, drawCount, stride);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdDrawMeshTasksIndirectEXT(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdDrawMeshTasksIndirectEXT(commandBuffer, buffer, offset, drawCount, stride);
   } else {
      vk_cmd_enqueue_CmdDrawMeshTasksIndirectEXT(commandBuffer, buffer, offset, drawCount, stride);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdDrawMeshTasksIndirectCountEXT(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_draw_mesh_tasks_indirect_count_ext(&cmd_buffer->cmd_queue,
                                       buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdDrawMeshTasksIndirectCountEXT(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdDrawMeshTasksIndirectCountEXT(commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
   } else {
      vk_cmd_enqueue_CmdDrawMeshTasksIndirectCountEXT(commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdBindInvocationMaskHUAWEI(VkCommandBuffer commandBuffer, VkImageView imageView, VkImageLayout imageLayout)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_bind_invocation_mask_huawei(&cmd_buffer->cmd_queue,
                                       imageView, imageLayout);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdBindInvocationMaskHUAWEI(VkCommandBuffer commandBuffer, VkImageView imageView, VkImageLayout imageLayout)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdBindInvocationMaskHUAWEI(commandBuffer, imageView, imageLayout);
   } else {
      vk_cmd_enqueue_CmdBindInvocationMaskHUAWEI(commandBuffer, imageView, imageLayout);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdCopyAccelerationStructureNV(VkCommandBuffer commandBuffer, VkAccelerationStructureNV dst, VkAccelerationStructureNV src, VkCopyAccelerationStructureModeKHR mode)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_copy_acceleration_structure_nv(&cmd_buffer->cmd_queue,
                                       dst, src, mode);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdCopyAccelerationStructureNV(VkCommandBuffer commandBuffer, VkAccelerationStructureNV dst, VkAccelerationStructureNV src, VkCopyAccelerationStructureModeKHR mode)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdCopyAccelerationStructureNV(commandBuffer, dst, src, mode);
   } else {
      vk_cmd_enqueue_CmdCopyAccelerationStructureNV(commandBuffer, dst, src, mode);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdCopyAccelerationStructureKHR(VkCommandBuffer commandBuffer, const VkCopyAccelerationStructureInfoKHR* pInfo)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_copy_acceleration_structure_khr(&cmd_buffer->cmd_queue,
                                       pInfo);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdCopyAccelerationStructureKHR(VkCommandBuffer commandBuffer, const VkCopyAccelerationStructureInfoKHR* pInfo)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdCopyAccelerationStructureKHR(commandBuffer, pInfo);
   } else {
      vk_cmd_enqueue_CmdCopyAccelerationStructureKHR(commandBuffer, pInfo);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdCopyAccelerationStructureToMemoryKHR(VkCommandBuffer commandBuffer, const VkCopyAccelerationStructureToMemoryInfoKHR* pInfo)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_copy_acceleration_structure_to_memory_khr(&cmd_buffer->cmd_queue,
                                       pInfo);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdCopyAccelerationStructureToMemoryKHR(VkCommandBuffer commandBuffer, const VkCopyAccelerationStructureToMemoryInfoKHR* pInfo)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdCopyAccelerationStructureToMemoryKHR(commandBuffer, pInfo);
   } else {
      vk_cmd_enqueue_CmdCopyAccelerationStructureToMemoryKHR(commandBuffer, pInfo);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdCopyMemoryToAccelerationStructureKHR(VkCommandBuffer commandBuffer, const VkCopyMemoryToAccelerationStructureInfoKHR* pInfo)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_copy_memory_to_acceleration_structure_khr(&cmd_buffer->cmd_queue,
                                       pInfo);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdCopyMemoryToAccelerationStructureKHR(VkCommandBuffer commandBuffer, const VkCopyMemoryToAccelerationStructureInfoKHR* pInfo)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdCopyMemoryToAccelerationStructureKHR(commandBuffer, pInfo);
   } else {
      vk_cmd_enqueue_CmdCopyMemoryToAccelerationStructureKHR(commandBuffer, pInfo);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdWriteAccelerationStructuresPropertiesKHR(VkCommandBuffer commandBuffer, uint32_t accelerationStructureCount, const VkAccelerationStructureKHR* pAccelerationStructures, VkQueryType queryType, VkQueryPool queryPool, uint32_t firstQuery)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_write_acceleration_structures_properties_khr(&cmd_buffer->cmd_queue,
                                       accelerationStructureCount, pAccelerationStructures, queryType, queryPool, firstQuery);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdWriteAccelerationStructuresPropertiesKHR(VkCommandBuffer commandBuffer, uint32_t accelerationStructureCount, const VkAccelerationStructureKHR* pAccelerationStructures, VkQueryType queryType, VkQueryPool queryPool, uint32_t firstQuery)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdWriteAccelerationStructuresPropertiesKHR(commandBuffer, accelerationStructureCount, pAccelerationStructures, queryType, queryPool, firstQuery);
   } else {
      vk_cmd_enqueue_CmdWriteAccelerationStructuresPropertiesKHR(commandBuffer, accelerationStructureCount, pAccelerationStructures, queryType, queryPool, firstQuery);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdWriteAccelerationStructuresPropertiesNV(VkCommandBuffer commandBuffer, uint32_t accelerationStructureCount, const VkAccelerationStructureNV* pAccelerationStructures, VkQueryType queryType, VkQueryPool queryPool, uint32_t firstQuery)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_write_acceleration_structures_properties_nv(&cmd_buffer->cmd_queue,
                                       accelerationStructureCount, pAccelerationStructures, queryType, queryPool, firstQuery);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdWriteAccelerationStructuresPropertiesNV(VkCommandBuffer commandBuffer, uint32_t accelerationStructureCount, const VkAccelerationStructureNV* pAccelerationStructures, VkQueryType queryType, VkQueryPool queryPool, uint32_t firstQuery)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdWriteAccelerationStructuresPropertiesNV(commandBuffer, accelerationStructureCount, pAccelerationStructures, queryType, queryPool, firstQuery);
   } else {
      vk_cmd_enqueue_CmdWriteAccelerationStructuresPropertiesNV(commandBuffer, accelerationStructureCount, pAccelerationStructures, queryType, queryPool, firstQuery);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdBuildAccelerationStructureNV(VkCommandBuffer commandBuffer, const VkAccelerationStructureInfoNV* pInfo, VkBuffer instanceData, VkDeviceSize instanceOffset, VkBool32 update, VkAccelerationStructureNV dst, VkAccelerationStructureNV src, VkBuffer scratch, VkDeviceSize scratchOffset)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_build_acceleration_structure_nv(&cmd_buffer->cmd_queue,
                                       pInfo, instanceData, instanceOffset, update, dst, src, scratch, scratchOffset);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdBuildAccelerationStructureNV(VkCommandBuffer commandBuffer, const VkAccelerationStructureInfoNV* pInfo, VkBuffer instanceData, VkDeviceSize instanceOffset, VkBool32 update, VkAccelerationStructureNV dst, VkAccelerationStructureNV src, VkBuffer scratch, VkDeviceSize scratchOffset)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdBuildAccelerationStructureNV(commandBuffer, pInfo, instanceData, instanceOffset, update, dst, src, scratch, scratchOffset);
   } else {
      vk_cmd_enqueue_CmdBuildAccelerationStructureNV(commandBuffer, pInfo, instanceData, instanceOffset, update, dst, src, scratch, scratchOffset);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdTraceRaysKHR(VkCommandBuffer commandBuffer, const VkStridedDeviceAddressRegionKHR* pRaygenShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pMissShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pHitShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pCallableShaderBindingTable, uint32_t width, uint32_t height, uint32_t depth)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_trace_rays_khr(&cmd_buffer->cmd_queue,
                                       pRaygenShaderBindingTable, pMissShaderBindingTable, pHitShaderBindingTable, pCallableShaderBindingTable, width, height, depth);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdTraceRaysKHR(VkCommandBuffer commandBuffer, const VkStridedDeviceAddressRegionKHR* pRaygenShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pMissShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pHitShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pCallableShaderBindingTable, uint32_t width, uint32_t height, uint32_t depth)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdTraceRaysKHR(commandBuffer, pRaygenShaderBindingTable, pMissShaderBindingTable, pHitShaderBindingTable, pCallableShaderBindingTable, width, height, depth);
   } else {
      vk_cmd_enqueue_CmdTraceRaysKHR(commandBuffer, pRaygenShaderBindingTable, pMissShaderBindingTable, pHitShaderBindingTable, pCallableShaderBindingTable, width, height, depth);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdTraceRaysNV(VkCommandBuffer commandBuffer, VkBuffer raygenShaderBindingTableBuffer, VkDeviceSize raygenShaderBindingOffset, VkBuffer missShaderBindingTableBuffer, VkDeviceSize missShaderBindingOffset, VkDeviceSize missShaderBindingStride, VkBuffer hitShaderBindingTableBuffer, VkDeviceSize hitShaderBindingOffset, VkDeviceSize hitShaderBindingStride, VkBuffer callableShaderBindingTableBuffer, VkDeviceSize callableShaderBindingOffset, VkDeviceSize callableShaderBindingStride, uint32_t width, uint32_t height, uint32_t depth)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_trace_rays_nv(&cmd_buffer->cmd_queue,
                                       raygenShaderBindingTableBuffer, raygenShaderBindingOffset, missShaderBindingTableBuffer, missShaderBindingOffset, missShaderBindingStride, hitShaderBindingTableBuffer, hitShaderBindingOffset, hitShaderBindingStride, callableShaderBindingTableBuffer, callableShaderBindingOffset, callableShaderBindingStride, width, height, depth);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdTraceRaysNV(VkCommandBuffer commandBuffer, VkBuffer raygenShaderBindingTableBuffer, VkDeviceSize raygenShaderBindingOffset, VkBuffer missShaderBindingTableBuffer, VkDeviceSize missShaderBindingOffset, VkDeviceSize missShaderBindingStride, VkBuffer hitShaderBindingTableBuffer, VkDeviceSize hitShaderBindingOffset, VkDeviceSize hitShaderBindingStride, VkBuffer callableShaderBindingTableBuffer, VkDeviceSize callableShaderBindingOffset, VkDeviceSize callableShaderBindingStride, uint32_t width, uint32_t height, uint32_t depth)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdTraceRaysNV(commandBuffer, raygenShaderBindingTableBuffer, raygenShaderBindingOffset, missShaderBindingTableBuffer, missShaderBindingOffset, missShaderBindingStride, hitShaderBindingTableBuffer, hitShaderBindingOffset, hitShaderBindingStride, callableShaderBindingTableBuffer, callableShaderBindingOffset, callableShaderBindingStride, width, height, depth);
   } else {
      vk_cmd_enqueue_CmdTraceRaysNV(commandBuffer, raygenShaderBindingTableBuffer, raygenShaderBindingOffset, missShaderBindingTableBuffer, missShaderBindingOffset, missShaderBindingStride, hitShaderBindingTableBuffer, hitShaderBindingOffset, hitShaderBindingStride, callableShaderBindingTableBuffer, callableShaderBindingOffset, callableShaderBindingStride, width, height, depth);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdTraceRaysIndirectKHR(VkCommandBuffer commandBuffer, const VkStridedDeviceAddressRegionKHR* pRaygenShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pMissShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pHitShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pCallableShaderBindingTable, VkDeviceAddress indirectDeviceAddress)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_trace_rays_indirect_khr(&cmd_buffer->cmd_queue,
                                       pRaygenShaderBindingTable, pMissShaderBindingTable, pHitShaderBindingTable, pCallableShaderBindingTable, indirectDeviceAddress);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdTraceRaysIndirectKHR(VkCommandBuffer commandBuffer, const VkStridedDeviceAddressRegionKHR* pRaygenShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pMissShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pHitShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pCallableShaderBindingTable, VkDeviceAddress indirectDeviceAddress)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdTraceRaysIndirectKHR(commandBuffer, pRaygenShaderBindingTable, pMissShaderBindingTable, pHitShaderBindingTable, pCallableShaderBindingTable, indirectDeviceAddress);
   } else {
      vk_cmd_enqueue_CmdTraceRaysIndirectKHR(commandBuffer, pRaygenShaderBindingTable, pMissShaderBindingTable, pHitShaderBindingTable, pCallableShaderBindingTable, indirectDeviceAddress);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdTraceRaysIndirect2KHR(VkCommandBuffer commandBuffer, VkDeviceAddress indirectDeviceAddress)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_trace_rays_indirect2_khr(&cmd_buffer->cmd_queue,
                                       indirectDeviceAddress);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdTraceRaysIndirect2KHR(VkCommandBuffer commandBuffer, VkDeviceAddress indirectDeviceAddress)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdTraceRaysIndirect2KHR(commandBuffer, indirectDeviceAddress);
   } else {
      vk_cmd_enqueue_CmdTraceRaysIndirect2KHR(commandBuffer, indirectDeviceAddress);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdSetRayTracingPipelineStackSizeKHR(VkCommandBuffer commandBuffer, uint32_t pipelineStackSize)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_set_ray_tracing_pipeline_stack_size_khr(&cmd_buffer->cmd_queue,
                                       pipelineStackSize);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdSetRayTracingPipelineStackSizeKHR(VkCommandBuffer commandBuffer, uint32_t pipelineStackSize)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdSetRayTracingPipelineStackSizeKHR(commandBuffer, pipelineStackSize);
   } else {
      vk_cmd_enqueue_CmdSetRayTracingPipelineStackSizeKHR(commandBuffer, pipelineStackSize);
   }
}
/* TODO: Generate vk_cmd_enqueue_CmdSetPerformanceMarkerINTEL() */
/* TODO: Generate vk_cmd_enqueue_CmdSetPerformanceStreamMarkerINTEL() */
/* TODO: Generate vk_cmd_enqueue_CmdSetPerformanceOverrideINTEL() */



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdSetLineStippleEXT(VkCommandBuffer commandBuffer, uint32_t lineStippleFactor, uint16_t lineStipplePattern)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_set_line_stipple_ext(&cmd_buffer->cmd_queue,
                                       lineStippleFactor, lineStipplePattern);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdSetLineStippleEXT(VkCommandBuffer commandBuffer, uint32_t lineStippleFactor, uint16_t lineStipplePattern)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdSetLineStippleEXT(commandBuffer, lineStippleFactor, lineStipplePattern);
   } else {
      vk_cmd_enqueue_CmdSetLineStippleEXT(commandBuffer, lineStippleFactor, lineStipplePattern);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdBuildAccelerationStructuresKHR(VkCommandBuffer                                    commandBuffer, uint32_t infoCount, const VkAccelerationStructureBuildGeometryInfoKHR* pInfos, const VkAccelerationStructureBuildRangeInfoKHR* const* ppBuildRangeInfos)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_build_acceleration_structures_khr(&cmd_buffer->cmd_queue,
                                       infoCount, pInfos, ppBuildRangeInfos);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdBuildAccelerationStructuresKHR(VkCommandBuffer                                    commandBuffer, uint32_t infoCount, const VkAccelerationStructureBuildGeometryInfoKHR* pInfos, const VkAccelerationStructureBuildRangeInfoKHR* const* ppBuildRangeInfos)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdBuildAccelerationStructuresKHR(commandBuffer, infoCount, pInfos, ppBuildRangeInfos);
   } else {
      vk_cmd_enqueue_CmdBuildAccelerationStructuresKHR(commandBuffer, infoCount, pInfos, ppBuildRangeInfos);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdBuildAccelerationStructuresIndirectKHR(VkCommandBuffer                  commandBuffer, uint32_t                                           infoCount, const VkAccelerationStructureBuildGeometryInfoKHR* pInfos, const VkDeviceAddress*             pIndirectDeviceAddresses, const uint32_t*                    pIndirectStrides, const uint32_t* const*             ppMaxPrimitiveCounts)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_build_acceleration_structures_indirect_khr(&cmd_buffer->cmd_queue,
                                       infoCount, pInfos, pIndirectDeviceAddresses, pIndirectStrides, ppMaxPrimitiveCounts);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdBuildAccelerationStructuresIndirectKHR(VkCommandBuffer                  commandBuffer, uint32_t                                           infoCount, const VkAccelerationStructureBuildGeometryInfoKHR* pInfos, const VkDeviceAddress*             pIndirectDeviceAddresses, const uint32_t*                    pIndirectStrides, const uint32_t* const*             ppMaxPrimitiveCounts)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdBuildAccelerationStructuresIndirectKHR(commandBuffer, infoCount, pInfos, pIndirectDeviceAddresses, pIndirectStrides, ppMaxPrimitiveCounts);
   } else {
      vk_cmd_enqueue_CmdBuildAccelerationStructuresIndirectKHR(commandBuffer, infoCount, pInfos, pIndirectDeviceAddresses, pIndirectStrides, ppMaxPrimitiveCounts);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdSetCullMode(VkCommandBuffer commandBuffer, VkCullModeFlags cullMode)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_set_cull_mode(&cmd_buffer->cmd_queue,
                                       cullMode);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdSetCullMode(VkCommandBuffer commandBuffer, VkCullModeFlags cullMode)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdSetCullMode(commandBuffer, cullMode);
   } else {
      vk_cmd_enqueue_CmdSetCullMode(commandBuffer, cullMode);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdSetFrontFace(VkCommandBuffer commandBuffer, VkFrontFace frontFace)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_set_front_face(&cmd_buffer->cmd_queue,
                                       frontFace);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdSetFrontFace(VkCommandBuffer commandBuffer, VkFrontFace frontFace)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdSetFrontFace(commandBuffer, frontFace);
   } else {
      vk_cmd_enqueue_CmdSetFrontFace(commandBuffer, frontFace);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdSetPrimitiveTopology(VkCommandBuffer commandBuffer, VkPrimitiveTopology primitiveTopology)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_set_primitive_topology(&cmd_buffer->cmd_queue,
                                       primitiveTopology);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdSetPrimitiveTopology(VkCommandBuffer commandBuffer, VkPrimitiveTopology primitiveTopology)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdSetPrimitiveTopology(commandBuffer, primitiveTopology);
   } else {
      vk_cmd_enqueue_CmdSetPrimitiveTopology(commandBuffer, primitiveTopology);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdSetViewportWithCount(VkCommandBuffer commandBuffer, uint32_t viewportCount, const VkViewport* pViewports)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_set_viewport_with_count(&cmd_buffer->cmd_queue,
                                       viewportCount, pViewports);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdSetViewportWithCount(VkCommandBuffer commandBuffer, uint32_t viewportCount, const VkViewport* pViewports)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdSetViewportWithCount(commandBuffer, viewportCount, pViewports);
   } else {
      vk_cmd_enqueue_CmdSetViewportWithCount(commandBuffer, viewportCount, pViewports);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdSetScissorWithCount(VkCommandBuffer commandBuffer, uint32_t scissorCount, const VkRect2D* pScissors)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_set_scissor_with_count(&cmd_buffer->cmd_queue,
                                       scissorCount, pScissors);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdSetScissorWithCount(VkCommandBuffer commandBuffer, uint32_t scissorCount, const VkRect2D* pScissors)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdSetScissorWithCount(commandBuffer, scissorCount, pScissors);
   } else {
      vk_cmd_enqueue_CmdSetScissorWithCount(commandBuffer, scissorCount, pScissors);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdBindVertexBuffers2(VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets, const VkDeviceSize* pSizes, const VkDeviceSize* pStrides)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_bind_vertex_buffers2(&cmd_buffer->cmd_queue,
                                       firstBinding, bindingCount, pBuffers, pOffsets, pSizes, pStrides);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdBindVertexBuffers2(VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets, const VkDeviceSize* pSizes, const VkDeviceSize* pStrides)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdBindVertexBuffers2(commandBuffer, firstBinding, bindingCount, pBuffers, pOffsets, pSizes, pStrides);
   } else {
      vk_cmd_enqueue_CmdBindVertexBuffers2(commandBuffer, firstBinding, bindingCount, pBuffers, pOffsets, pSizes, pStrides);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdSetDepthTestEnable(VkCommandBuffer commandBuffer, VkBool32 depthTestEnable)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_set_depth_test_enable(&cmd_buffer->cmd_queue,
                                       depthTestEnable);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdSetDepthTestEnable(VkCommandBuffer commandBuffer, VkBool32 depthTestEnable)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdSetDepthTestEnable(commandBuffer, depthTestEnable);
   } else {
      vk_cmd_enqueue_CmdSetDepthTestEnable(commandBuffer, depthTestEnable);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdSetDepthWriteEnable(VkCommandBuffer commandBuffer, VkBool32 depthWriteEnable)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_set_depth_write_enable(&cmd_buffer->cmd_queue,
                                       depthWriteEnable);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdSetDepthWriteEnable(VkCommandBuffer commandBuffer, VkBool32 depthWriteEnable)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdSetDepthWriteEnable(commandBuffer, depthWriteEnable);
   } else {
      vk_cmd_enqueue_CmdSetDepthWriteEnable(commandBuffer, depthWriteEnable);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdSetDepthCompareOp(VkCommandBuffer commandBuffer, VkCompareOp depthCompareOp)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_set_depth_compare_op(&cmd_buffer->cmd_queue,
                                       depthCompareOp);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdSetDepthCompareOp(VkCommandBuffer commandBuffer, VkCompareOp depthCompareOp)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdSetDepthCompareOp(commandBuffer, depthCompareOp);
   } else {
      vk_cmd_enqueue_CmdSetDepthCompareOp(commandBuffer, depthCompareOp);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdSetDepthBoundsTestEnable(VkCommandBuffer commandBuffer, VkBool32 depthBoundsTestEnable)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_set_depth_bounds_test_enable(&cmd_buffer->cmd_queue,
                                       depthBoundsTestEnable);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdSetDepthBoundsTestEnable(VkCommandBuffer commandBuffer, VkBool32 depthBoundsTestEnable)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdSetDepthBoundsTestEnable(commandBuffer, depthBoundsTestEnable);
   } else {
      vk_cmd_enqueue_CmdSetDepthBoundsTestEnable(commandBuffer, depthBoundsTestEnable);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdSetStencilTestEnable(VkCommandBuffer commandBuffer, VkBool32 stencilTestEnable)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_set_stencil_test_enable(&cmd_buffer->cmd_queue,
                                       stencilTestEnable);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdSetStencilTestEnable(VkCommandBuffer commandBuffer, VkBool32 stencilTestEnable)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdSetStencilTestEnable(commandBuffer, stencilTestEnable);
   } else {
      vk_cmd_enqueue_CmdSetStencilTestEnable(commandBuffer, stencilTestEnable);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdSetStencilOp(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, VkStencilOp failOp, VkStencilOp passOp, VkStencilOp depthFailOp, VkCompareOp compareOp)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_set_stencil_op(&cmd_buffer->cmd_queue,
                                       faceMask, failOp, passOp, depthFailOp, compareOp);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdSetStencilOp(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, VkStencilOp failOp, VkStencilOp passOp, VkStencilOp depthFailOp, VkCompareOp compareOp)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdSetStencilOp(commandBuffer, faceMask, failOp, passOp, depthFailOp, compareOp);
   } else {
      vk_cmd_enqueue_CmdSetStencilOp(commandBuffer, faceMask, failOp, passOp, depthFailOp, compareOp);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdSetPatchControlPointsEXT(VkCommandBuffer commandBuffer, uint32_t patchControlPoints)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_set_patch_control_points_ext(&cmd_buffer->cmd_queue,
                                       patchControlPoints);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdSetPatchControlPointsEXT(VkCommandBuffer commandBuffer, uint32_t patchControlPoints)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdSetPatchControlPointsEXT(commandBuffer, patchControlPoints);
   } else {
      vk_cmd_enqueue_CmdSetPatchControlPointsEXT(commandBuffer, patchControlPoints);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdSetRasterizerDiscardEnable(VkCommandBuffer commandBuffer, VkBool32 rasterizerDiscardEnable)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_set_rasterizer_discard_enable(&cmd_buffer->cmd_queue,
                                       rasterizerDiscardEnable);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdSetRasterizerDiscardEnable(VkCommandBuffer commandBuffer, VkBool32 rasterizerDiscardEnable)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdSetRasterizerDiscardEnable(commandBuffer, rasterizerDiscardEnable);
   } else {
      vk_cmd_enqueue_CmdSetRasterizerDiscardEnable(commandBuffer, rasterizerDiscardEnable);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdSetDepthBiasEnable(VkCommandBuffer commandBuffer, VkBool32 depthBiasEnable)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_set_depth_bias_enable(&cmd_buffer->cmd_queue,
                                       depthBiasEnable);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdSetDepthBiasEnable(VkCommandBuffer commandBuffer, VkBool32 depthBiasEnable)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdSetDepthBiasEnable(commandBuffer, depthBiasEnable);
   } else {
      vk_cmd_enqueue_CmdSetDepthBiasEnable(commandBuffer, depthBiasEnable);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdSetLogicOpEXT(VkCommandBuffer commandBuffer, VkLogicOp logicOp)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_set_logic_op_ext(&cmd_buffer->cmd_queue,
                                       logicOp);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdSetLogicOpEXT(VkCommandBuffer commandBuffer, VkLogicOp logicOp)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdSetLogicOpEXT(commandBuffer, logicOp);
   } else {
      vk_cmd_enqueue_CmdSetLogicOpEXT(commandBuffer, logicOp);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdSetPrimitiveRestartEnable(VkCommandBuffer commandBuffer, VkBool32 primitiveRestartEnable)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_set_primitive_restart_enable(&cmd_buffer->cmd_queue,
                                       primitiveRestartEnable);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdSetPrimitiveRestartEnable(VkCommandBuffer commandBuffer, VkBool32 primitiveRestartEnable)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdSetPrimitiveRestartEnable(commandBuffer, primitiveRestartEnable);
   } else {
      vk_cmd_enqueue_CmdSetPrimitiveRestartEnable(commandBuffer, primitiveRestartEnable);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdSetTessellationDomainOriginEXT(VkCommandBuffer commandBuffer, VkTessellationDomainOrigin domainOrigin)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_set_tessellation_domain_origin_ext(&cmd_buffer->cmd_queue,
                                       domainOrigin);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdSetTessellationDomainOriginEXT(VkCommandBuffer commandBuffer, VkTessellationDomainOrigin domainOrigin)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdSetTessellationDomainOriginEXT(commandBuffer, domainOrigin);
   } else {
      vk_cmd_enqueue_CmdSetTessellationDomainOriginEXT(commandBuffer, domainOrigin);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdSetDepthClampEnableEXT(VkCommandBuffer commandBuffer, VkBool32 depthClampEnable)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_set_depth_clamp_enable_ext(&cmd_buffer->cmd_queue,
                                       depthClampEnable);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdSetDepthClampEnableEXT(VkCommandBuffer commandBuffer, VkBool32 depthClampEnable)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdSetDepthClampEnableEXT(commandBuffer, depthClampEnable);
   } else {
      vk_cmd_enqueue_CmdSetDepthClampEnableEXT(commandBuffer, depthClampEnable);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdSetPolygonModeEXT(VkCommandBuffer commandBuffer, VkPolygonMode polygonMode)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_set_polygon_mode_ext(&cmd_buffer->cmd_queue,
                                       polygonMode);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdSetPolygonModeEXT(VkCommandBuffer commandBuffer, VkPolygonMode polygonMode)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdSetPolygonModeEXT(commandBuffer, polygonMode);
   } else {
      vk_cmd_enqueue_CmdSetPolygonModeEXT(commandBuffer, polygonMode);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdSetRasterizationSamplesEXT(VkCommandBuffer commandBuffer, VkSampleCountFlagBits  rasterizationSamples)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_set_rasterization_samples_ext(&cmd_buffer->cmd_queue,
                                       rasterizationSamples);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdSetRasterizationSamplesEXT(VkCommandBuffer commandBuffer, VkSampleCountFlagBits  rasterizationSamples)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdSetRasterizationSamplesEXT(commandBuffer, rasterizationSamples);
   } else {
      vk_cmd_enqueue_CmdSetRasterizationSamplesEXT(commandBuffer, rasterizationSamples);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdSetSampleMaskEXT(VkCommandBuffer commandBuffer, VkSampleCountFlagBits  samples, const VkSampleMask*    pSampleMask)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_set_sample_mask_ext(&cmd_buffer->cmd_queue,
                                       samples, pSampleMask);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdSetSampleMaskEXT(VkCommandBuffer commandBuffer, VkSampleCountFlagBits  samples, const VkSampleMask*    pSampleMask)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdSetSampleMaskEXT(commandBuffer, samples, pSampleMask);
   } else {
      vk_cmd_enqueue_CmdSetSampleMaskEXT(commandBuffer, samples, pSampleMask);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdSetAlphaToCoverageEnableEXT(VkCommandBuffer commandBuffer, VkBool32 alphaToCoverageEnable)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_set_alpha_to_coverage_enable_ext(&cmd_buffer->cmd_queue,
                                       alphaToCoverageEnable);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdSetAlphaToCoverageEnableEXT(VkCommandBuffer commandBuffer, VkBool32 alphaToCoverageEnable)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdSetAlphaToCoverageEnableEXT(commandBuffer, alphaToCoverageEnable);
   } else {
      vk_cmd_enqueue_CmdSetAlphaToCoverageEnableEXT(commandBuffer, alphaToCoverageEnable);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdSetAlphaToOneEnableEXT(VkCommandBuffer commandBuffer, VkBool32 alphaToOneEnable)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_set_alpha_to_one_enable_ext(&cmd_buffer->cmd_queue,
                                       alphaToOneEnable);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdSetAlphaToOneEnableEXT(VkCommandBuffer commandBuffer, VkBool32 alphaToOneEnable)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdSetAlphaToOneEnableEXT(commandBuffer, alphaToOneEnable);
   } else {
      vk_cmd_enqueue_CmdSetAlphaToOneEnableEXT(commandBuffer, alphaToOneEnable);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdSetLogicOpEnableEXT(VkCommandBuffer commandBuffer, VkBool32 logicOpEnable)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_set_logic_op_enable_ext(&cmd_buffer->cmd_queue,
                                       logicOpEnable);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdSetLogicOpEnableEXT(VkCommandBuffer commandBuffer, VkBool32 logicOpEnable)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdSetLogicOpEnableEXT(commandBuffer, logicOpEnable);
   } else {
      vk_cmd_enqueue_CmdSetLogicOpEnableEXT(commandBuffer, logicOpEnable);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdSetColorBlendEnableEXT(VkCommandBuffer commandBuffer, uint32_t firstAttachment, uint32_t attachmentCount, const VkBool32* pColorBlendEnables)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_set_color_blend_enable_ext(&cmd_buffer->cmd_queue,
                                       firstAttachment, attachmentCount, pColorBlendEnables);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdSetColorBlendEnableEXT(VkCommandBuffer commandBuffer, uint32_t firstAttachment, uint32_t attachmentCount, const VkBool32* pColorBlendEnables)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdSetColorBlendEnableEXT(commandBuffer, firstAttachment, attachmentCount, pColorBlendEnables);
   } else {
      vk_cmd_enqueue_CmdSetColorBlendEnableEXT(commandBuffer, firstAttachment, attachmentCount, pColorBlendEnables);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdSetColorBlendEquationEXT(VkCommandBuffer commandBuffer, uint32_t firstAttachment, uint32_t attachmentCount, const VkColorBlendEquationEXT* pColorBlendEquations)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_set_color_blend_equation_ext(&cmd_buffer->cmd_queue,
                                       firstAttachment, attachmentCount, pColorBlendEquations);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdSetColorBlendEquationEXT(VkCommandBuffer commandBuffer, uint32_t firstAttachment, uint32_t attachmentCount, const VkColorBlendEquationEXT* pColorBlendEquations)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdSetColorBlendEquationEXT(commandBuffer, firstAttachment, attachmentCount, pColorBlendEquations);
   } else {
      vk_cmd_enqueue_CmdSetColorBlendEquationEXT(commandBuffer, firstAttachment, attachmentCount, pColorBlendEquations);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdSetColorWriteMaskEXT(VkCommandBuffer commandBuffer, uint32_t firstAttachment, uint32_t attachmentCount, const VkColorComponentFlags* pColorWriteMasks)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_set_color_write_mask_ext(&cmd_buffer->cmd_queue,
                                       firstAttachment, attachmentCount, pColorWriteMasks);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdSetColorWriteMaskEXT(VkCommandBuffer commandBuffer, uint32_t firstAttachment, uint32_t attachmentCount, const VkColorComponentFlags* pColorWriteMasks)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdSetColorWriteMaskEXT(commandBuffer, firstAttachment, attachmentCount, pColorWriteMasks);
   } else {
      vk_cmd_enqueue_CmdSetColorWriteMaskEXT(commandBuffer, firstAttachment, attachmentCount, pColorWriteMasks);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdSetRasterizationStreamEXT(VkCommandBuffer commandBuffer, uint32_t rasterizationStream)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_set_rasterization_stream_ext(&cmd_buffer->cmd_queue,
                                       rasterizationStream);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdSetRasterizationStreamEXT(VkCommandBuffer commandBuffer, uint32_t rasterizationStream)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdSetRasterizationStreamEXT(commandBuffer, rasterizationStream);
   } else {
      vk_cmd_enqueue_CmdSetRasterizationStreamEXT(commandBuffer, rasterizationStream);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdSetConservativeRasterizationModeEXT(VkCommandBuffer commandBuffer, VkConservativeRasterizationModeEXT conservativeRasterizationMode)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_set_conservative_rasterization_mode_ext(&cmd_buffer->cmd_queue,
                                       conservativeRasterizationMode);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdSetConservativeRasterizationModeEXT(VkCommandBuffer commandBuffer, VkConservativeRasterizationModeEXT conservativeRasterizationMode)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdSetConservativeRasterizationModeEXT(commandBuffer, conservativeRasterizationMode);
   } else {
      vk_cmd_enqueue_CmdSetConservativeRasterizationModeEXT(commandBuffer, conservativeRasterizationMode);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdSetExtraPrimitiveOverestimationSizeEXT(VkCommandBuffer commandBuffer, float extraPrimitiveOverestimationSize)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_set_extra_primitive_overestimation_size_ext(&cmd_buffer->cmd_queue,
                                       extraPrimitiveOverestimationSize);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdSetExtraPrimitiveOverestimationSizeEXT(VkCommandBuffer commandBuffer, float extraPrimitiveOverestimationSize)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdSetExtraPrimitiveOverestimationSizeEXT(commandBuffer, extraPrimitiveOverestimationSize);
   } else {
      vk_cmd_enqueue_CmdSetExtraPrimitiveOverestimationSizeEXT(commandBuffer, extraPrimitiveOverestimationSize);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdSetDepthClipEnableEXT(VkCommandBuffer commandBuffer, VkBool32 depthClipEnable)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_set_depth_clip_enable_ext(&cmd_buffer->cmd_queue,
                                       depthClipEnable);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdSetDepthClipEnableEXT(VkCommandBuffer commandBuffer, VkBool32 depthClipEnable)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdSetDepthClipEnableEXT(commandBuffer, depthClipEnable);
   } else {
      vk_cmd_enqueue_CmdSetDepthClipEnableEXT(commandBuffer, depthClipEnable);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdSetSampleLocationsEnableEXT(VkCommandBuffer commandBuffer, VkBool32 sampleLocationsEnable)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_set_sample_locations_enable_ext(&cmd_buffer->cmd_queue,
                                       sampleLocationsEnable);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdSetSampleLocationsEnableEXT(VkCommandBuffer commandBuffer, VkBool32 sampleLocationsEnable)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdSetSampleLocationsEnableEXT(commandBuffer, sampleLocationsEnable);
   } else {
      vk_cmd_enqueue_CmdSetSampleLocationsEnableEXT(commandBuffer, sampleLocationsEnable);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdSetColorBlendAdvancedEXT(VkCommandBuffer commandBuffer, uint32_t firstAttachment, uint32_t attachmentCount, const VkColorBlendAdvancedEXT* pColorBlendAdvanced)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_set_color_blend_advanced_ext(&cmd_buffer->cmd_queue,
                                       firstAttachment, attachmentCount, pColorBlendAdvanced);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdSetColorBlendAdvancedEXT(VkCommandBuffer commandBuffer, uint32_t firstAttachment, uint32_t attachmentCount, const VkColorBlendAdvancedEXT* pColorBlendAdvanced)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdSetColorBlendAdvancedEXT(commandBuffer, firstAttachment, attachmentCount, pColorBlendAdvanced);
   } else {
      vk_cmd_enqueue_CmdSetColorBlendAdvancedEXT(commandBuffer, firstAttachment, attachmentCount, pColorBlendAdvanced);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdSetProvokingVertexModeEXT(VkCommandBuffer commandBuffer, VkProvokingVertexModeEXT provokingVertexMode)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_set_provoking_vertex_mode_ext(&cmd_buffer->cmd_queue,
                                       provokingVertexMode);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdSetProvokingVertexModeEXT(VkCommandBuffer commandBuffer, VkProvokingVertexModeEXT provokingVertexMode)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdSetProvokingVertexModeEXT(commandBuffer, provokingVertexMode);
   } else {
      vk_cmd_enqueue_CmdSetProvokingVertexModeEXT(commandBuffer, provokingVertexMode);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdSetLineRasterizationModeEXT(VkCommandBuffer commandBuffer, VkLineRasterizationModeEXT lineRasterizationMode)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_set_line_rasterization_mode_ext(&cmd_buffer->cmd_queue,
                                       lineRasterizationMode);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdSetLineRasterizationModeEXT(VkCommandBuffer commandBuffer, VkLineRasterizationModeEXT lineRasterizationMode)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdSetLineRasterizationModeEXT(commandBuffer, lineRasterizationMode);
   } else {
      vk_cmd_enqueue_CmdSetLineRasterizationModeEXT(commandBuffer, lineRasterizationMode);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdSetLineStippleEnableEXT(VkCommandBuffer commandBuffer, VkBool32 stippledLineEnable)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_set_line_stipple_enable_ext(&cmd_buffer->cmd_queue,
                                       stippledLineEnable);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdSetLineStippleEnableEXT(VkCommandBuffer commandBuffer, VkBool32 stippledLineEnable)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdSetLineStippleEnableEXT(commandBuffer, stippledLineEnable);
   } else {
      vk_cmd_enqueue_CmdSetLineStippleEnableEXT(commandBuffer, stippledLineEnable);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdSetDepthClipNegativeOneToOneEXT(VkCommandBuffer commandBuffer, VkBool32 negativeOneToOne)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_set_depth_clip_negative_one_to_one_ext(&cmd_buffer->cmd_queue,
                                       negativeOneToOne);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdSetDepthClipNegativeOneToOneEXT(VkCommandBuffer commandBuffer, VkBool32 negativeOneToOne)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdSetDepthClipNegativeOneToOneEXT(commandBuffer, negativeOneToOne);
   } else {
      vk_cmd_enqueue_CmdSetDepthClipNegativeOneToOneEXT(commandBuffer, negativeOneToOne);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdSetViewportWScalingEnableNV(VkCommandBuffer commandBuffer, VkBool32 viewportWScalingEnable)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_set_viewport_wscaling_enable_nv(&cmd_buffer->cmd_queue,
                                       viewportWScalingEnable);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdSetViewportWScalingEnableNV(VkCommandBuffer commandBuffer, VkBool32 viewportWScalingEnable)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdSetViewportWScalingEnableNV(commandBuffer, viewportWScalingEnable);
   } else {
      vk_cmd_enqueue_CmdSetViewportWScalingEnableNV(commandBuffer, viewportWScalingEnable);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdSetViewportSwizzleNV(VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const VkViewportSwizzleNV* pViewportSwizzles)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_set_viewport_swizzle_nv(&cmd_buffer->cmd_queue,
                                       firstViewport, viewportCount, pViewportSwizzles);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdSetViewportSwizzleNV(VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const VkViewportSwizzleNV* pViewportSwizzles)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdSetViewportSwizzleNV(commandBuffer, firstViewport, viewportCount, pViewportSwizzles);
   } else {
      vk_cmd_enqueue_CmdSetViewportSwizzleNV(commandBuffer, firstViewport, viewportCount, pViewportSwizzles);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdSetCoverageToColorEnableNV(VkCommandBuffer commandBuffer, VkBool32 coverageToColorEnable)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_set_coverage_to_color_enable_nv(&cmd_buffer->cmd_queue,
                                       coverageToColorEnable);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdSetCoverageToColorEnableNV(VkCommandBuffer commandBuffer, VkBool32 coverageToColorEnable)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdSetCoverageToColorEnableNV(commandBuffer, coverageToColorEnable);
   } else {
      vk_cmd_enqueue_CmdSetCoverageToColorEnableNV(commandBuffer, coverageToColorEnable);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdSetCoverageToColorLocationNV(VkCommandBuffer commandBuffer, uint32_t coverageToColorLocation)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_set_coverage_to_color_location_nv(&cmd_buffer->cmd_queue,
                                       coverageToColorLocation);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdSetCoverageToColorLocationNV(VkCommandBuffer commandBuffer, uint32_t coverageToColorLocation)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdSetCoverageToColorLocationNV(commandBuffer, coverageToColorLocation);
   } else {
      vk_cmd_enqueue_CmdSetCoverageToColorLocationNV(commandBuffer, coverageToColorLocation);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdSetCoverageModulationModeNV(VkCommandBuffer commandBuffer, VkCoverageModulationModeNV coverageModulationMode)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_set_coverage_modulation_mode_nv(&cmd_buffer->cmd_queue,
                                       coverageModulationMode);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdSetCoverageModulationModeNV(VkCommandBuffer commandBuffer, VkCoverageModulationModeNV coverageModulationMode)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdSetCoverageModulationModeNV(commandBuffer, coverageModulationMode);
   } else {
      vk_cmd_enqueue_CmdSetCoverageModulationModeNV(commandBuffer, coverageModulationMode);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdSetCoverageModulationTableEnableNV(VkCommandBuffer commandBuffer, VkBool32 coverageModulationTableEnable)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_set_coverage_modulation_table_enable_nv(&cmd_buffer->cmd_queue,
                                       coverageModulationTableEnable);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdSetCoverageModulationTableEnableNV(VkCommandBuffer commandBuffer, VkBool32 coverageModulationTableEnable)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdSetCoverageModulationTableEnableNV(commandBuffer, coverageModulationTableEnable);
   } else {
      vk_cmd_enqueue_CmdSetCoverageModulationTableEnableNV(commandBuffer, coverageModulationTableEnable);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdSetCoverageModulationTableNV(VkCommandBuffer commandBuffer, uint32_t coverageModulationTableCount, const float* pCoverageModulationTable)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_set_coverage_modulation_table_nv(&cmd_buffer->cmd_queue,
                                       coverageModulationTableCount, pCoverageModulationTable);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdSetCoverageModulationTableNV(VkCommandBuffer commandBuffer, uint32_t coverageModulationTableCount, const float* pCoverageModulationTable)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdSetCoverageModulationTableNV(commandBuffer, coverageModulationTableCount, pCoverageModulationTable);
   } else {
      vk_cmd_enqueue_CmdSetCoverageModulationTableNV(commandBuffer, coverageModulationTableCount, pCoverageModulationTable);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdSetShadingRateImageEnableNV(VkCommandBuffer commandBuffer, VkBool32 shadingRateImageEnable)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_set_shading_rate_image_enable_nv(&cmd_buffer->cmd_queue,
                                       shadingRateImageEnable);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdSetShadingRateImageEnableNV(VkCommandBuffer commandBuffer, VkBool32 shadingRateImageEnable)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdSetShadingRateImageEnableNV(commandBuffer, shadingRateImageEnable);
   } else {
      vk_cmd_enqueue_CmdSetShadingRateImageEnableNV(commandBuffer, shadingRateImageEnable);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdSetCoverageReductionModeNV(VkCommandBuffer commandBuffer, VkCoverageReductionModeNV coverageReductionMode)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_set_coverage_reduction_mode_nv(&cmd_buffer->cmd_queue,
                                       coverageReductionMode);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdSetCoverageReductionModeNV(VkCommandBuffer commandBuffer, VkCoverageReductionModeNV coverageReductionMode)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdSetCoverageReductionModeNV(commandBuffer, coverageReductionMode);
   } else {
      vk_cmd_enqueue_CmdSetCoverageReductionModeNV(commandBuffer, coverageReductionMode);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdSetRepresentativeFragmentTestEnableNV(VkCommandBuffer commandBuffer, VkBool32 representativeFragmentTestEnable)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_set_representative_fragment_test_enable_nv(&cmd_buffer->cmd_queue,
                                       representativeFragmentTestEnable);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdSetRepresentativeFragmentTestEnableNV(VkCommandBuffer commandBuffer, VkBool32 representativeFragmentTestEnable)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdSetRepresentativeFragmentTestEnableNV(commandBuffer, representativeFragmentTestEnable);
   } else {
      vk_cmd_enqueue_CmdSetRepresentativeFragmentTestEnableNV(commandBuffer, representativeFragmentTestEnable);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdCopyBuffer2(VkCommandBuffer commandBuffer, const VkCopyBufferInfo2* pCopyBufferInfo)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_copy_buffer2(&cmd_buffer->cmd_queue,
                                       pCopyBufferInfo);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdCopyBuffer2(VkCommandBuffer commandBuffer, const VkCopyBufferInfo2* pCopyBufferInfo)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdCopyBuffer2(commandBuffer, pCopyBufferInfo);
   } else {
      vk_cmd_enqueue_CmdCopyBuffer2(commandBuffer, pCopyBufferInfo);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdCopyImage2(VkCommandBuffer commandBuffer, const VkCopyImageInfo2* pCopyImageInfo)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_copy_image2(&cmd_buffer->cmd_queue,
                                       pCopyImageInfo);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdCopyImage2(VkCommandBuffer commandBuffer, const VkCopyImageInfo2* pCopyImageInfo)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdCopyImage2(commandBuffer, pCopyImageInfo);
   } else {
      vk_cmd_enqueue_CmdCopyImage2(commandBuffer, pCopyImageInfo);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdBlitImage2(VkCommandBuffer commandBuffer, const VkBlitImageInfo2* pBlitImageInfo)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_blit_image2(&cmd_buffer->cmd_queue,
                                       pBlitImageInfo);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdBlitImage2(VkCommandBuffer commandBuffer, const VkBlitImageInfo2* pBlitImageInfo)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdBlitImage2(commandBuffer, pBlitImageInfo);
   } else {
      vk_cmd_enqueue_CmdBlitImage2(commandBuffer, pBlitImageInfo);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdCopyBufferToImage2(VkCommandBuffer commandBuffer, const VkCopyBufferToImageInfo2* pCopyBufferToImageInfo)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_copy_buffer_to_image2(&cmd_buffer->cmd_queue,
                                       pCopyBufferToImageInfo);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdCopyBufferToImage2(VkCommandBuffer commandBuffer, const VkCopyBufferToImageInfo2* pCopyBufferToImageInfo)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdCopyBufferToImage2(commandBuffer, pCopyBufferToImageInfo);
   } else {
      vk_cmd_enqueue_CmdCopyBufferToImage2(commandBuffer, pCopyBufferToImageInfo);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdCopyImageToBuffer2(VkCommandBuffer commandBuffer, const VkCopyImageToBufferInfo2* pCopyImageToBufferInfo)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_copy_image_to_buffer2(&cmd_buffer->cmd_queue,
                                       pCopyImageToBufferInfo);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdCopyImageToBuffer2(VkCommandBuffer commandBuffer, const VkCopyImageToBufferInfo2* pCopyImageToBufferInfo)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdCopyImageToBuffer2(commandBuffer, pCopyImageToBufferInfo);
   } else {
      vk_cmd_enqueue_CmdCopyImageToBuffer2(commandBuffer, pCopyImageToBufferInfo);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdResolveImage2(VkCommandBuffer commandBuffer, const VkResolveImageInfo2* pResolveImageInfo)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_resolve_image2(&cmd_buffer->cmd_queue,
                                       pResolveImageInfo);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdResolveImage2(VkCommandBuffer commandBuffer, const VkResolveImageInfo2* pResolveImageInfo)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdResolveImage2(commandBuffer, pResolveImageInfo);
   } else {
      vk_cmd_enqueue_CmdResolveImage2(commandBuffer, pResolveImageInfo);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdSetFragmentShadingRateKHR(VkCommandBuffer           commandBuffer, const VkExtent2D*                           pFragmentSize, const VkFragmentShadingRateCombinerOpKHR    combinerOps[2])
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_set_fragment_shading_rate_khr(&cmd_buffer->cmd_queue,
                                       pFragmentSize, combinerOps);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdSetFragmentShadingRateKHR(VkCommandBuffer           commandBuffer, const VkExtent2D*                           pFragmentSize, const VkFragmentShadingRateCombinerOpKHR    combinerOps[2])
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdSetFragmentShadingRateKHR(commandBuffer, pFragmentSize, combinerOps);
   } else {
      vk_cmd_enqueue_CmdSetFragmentShadingRateKHR(commandBuffer, pFragmentSize, combinerOps);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdSetFragmentShadingRateEnumNV(VkCommandBuffer           commandBuffer, VkFragmentShadingRateNV                     shadingRate, const VkFragmentShadingRateCombinerOpKHR    combinerOps[2])
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_set_fragment_shading_rate_enum_nv(&cmd_buffer->cmd_queue,
                                       shadingRate, combinerOps);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdSetFragmentShadingRateEnumNV(VkCommandBuffer           commandBuffer, VkFragmentShadingRateNV                     shadingRate, const VkFragmentShadingRateCombinerOpKHR    combinerOps[2])
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdSetFragmentShadingRateEnumNV(commandBuffer, shadingRate, combinerOps);
   } else {
      vk_cmd_enqueue_CmdSetFragmentShadingRateEnumNV(commandBuffer, shadingRate, combinerOps);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdSetVertexInputEXT(VkCommandBuffer commandBuffer, uint32_t vertexBindingDescriptionCount, const VkVertexInputBindingDescription2EXT* pVertexBindingDescriptions, uint32_t vertexAttributeDescriptionCount, const VkVertexInputAttributeDescription2EXT* pVertexAttributeDescriptions)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_set_vertex_input_ext(&cmd_buffer->cmd_queue,
                                       vertexBindingDescriptionCount, pVertexBindingDescriptions, vertexAttributeDescriptionCount, pVertexAttributeDescriptions);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdSetVertexInputEXT(VkCommandBuffer commandBuffer, uint32_t vertexBindingDescriptionCount, const VkVertexInputBindingDescription2EXT* pVertexBindingDescriptions, uint32_t vertexAttributeDescriptionCount, const VkVertexInputAttributeDescription2EXT* pVertexAttributeDescriptions)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdSetVertexInputEXT(commandBuffer, vertexBindingDescriptionCount, pVertexBindingDescriptions, vertexAttributeDescriptionCount, pVertexAttributeDescriptions);
   } else {
      vk_cmd_enqueue_CmdSetVertexInputEXT(commandBuffer, vertexBindingDescriptionCount, pVertexBindingDescriptions, vertexAttributeDescriptionCount, pVertexAttributeDescriptions);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdSetColorWriteEnableEXT(VkCommandBuffer       commandBuffer, uint32_t                                attachmentCount, const VkBool32*   pColorWriteEnables)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_set_color_write_enable_ext(&cmd_buffer->cmd_queue,
                                       attachmentCount, pColorWriteEnables);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdSetColorWriteEnableEXT(VkCommandBuffer       commandBuffer, uint32_t                                attachmentCount, const VkBool32*   pColorWriteEnables)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdSetColorWriteEnableEXT(commandBuffer, attachmentCount, pColorWriteEnables);
   } else {
      vk_cmd_enqueue_CmdSetColorWriteEnableEXT(commandBuffer, attachmentCount, pColorWriteEnables);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdSetEvent2(VkCommandBuffer                   commandBuffer, VkEvent                                             event, const VkDependencyInfo*                             pDependencyInfo)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_set_event2(&cmd_buffer->cmd_queue,
                                       event, pDependencyInfo);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdSetEvent2(VkCommandBuffer                   commandBuffer, VkEvent                                             event, const VkDependencyInfo*                             pDependencyInfo)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdSetEvent2(commandBuffer, event, pDependencyInfo);
   } else {
      vk_cmd_enqueue_CmdSetEvent2(commandBuffer, event, pDependencyInfo);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdResetEvent2(VkCommandBuffer                   commandBuffer, VkEvent                                             event, VkPipelineStageFlags2               stageMask)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_reset_event2(&cmd_buffer->cmd_queue,
                                       event, stageMask);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdResetEvent2(VkCommandBuffer                   commandBuffer, VkEvent                                             event, VkPipelineStageFlags2               stageMask)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdResetEvent2(commandBuffer, event, stageMask);
   } else {
      vk_cmd_enqueue_CmdResetEvent2(commandBuffer, event, stageMask);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdWaitEvents2(VkCommandBuffer                   commandBuffer, uint32_t                                            eventCount, const VkEvent*                     pEvents, const VkDependencyInfo*            pDependencyInfos)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_wait_events2(&cmd_buffer->cmd_queue,
                                       eventCount, pEvents, pDependencyInfos);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdWaitEvents2(VkCommandBuffer                   commandBuffer, uint32_t                                            eventCount, const VkEvent*                     pEvents, const VkDependencyInfo*            pDependencyInfos)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdWaitEvents2(commandBuffer, eventCount, pEvents, pDependencyInfos);
   } else {
      vk_cmd_enqueue_CmdWaitEvents2(commandBuffer, eventCount, pEvents, pDependencyInfos);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdPipelineBarrier2(VkCommandBuffer                   commandBuffer, const VkDependencyInfo*                             pDependencyInfo)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_pipeline_barrier2(&cmd_buffer->cmd_queue,
                                       pDependencyInfo);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdPipelineBarrier2(VkCommandBuffer                   commandBuffer, const VkDependencyInfo*                             pDependencyInfo)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdPipelineBarrier2(commandBuffer, pDependencyInfo);
   } else {
      vk_cmd_enqueue_CmdPipelineBarrier2(commandBuffer, pDependencyInfo);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdWriteTimestamp2(VkCommandBuffer                   commandBuffer, VkPipelineStageFlags2               stage, VkQueryPool                                         queryPool, uint32_t                                            query)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_write_timestamp2(&cmd_buffer->cmd_queue,
                                       stage, queryPool, query);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdWriteTimestamp2(VkCommandBuffer                   commandBuffer, VkPipelineStageFlags2               stage, VkQueryPool                                         queryPool, uint32_t                                            query)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdWriteTimestamp2(commandBuffer, stage, queryPool, query);
   } else {
      vk_cmd_enqueue_CmdWriteTimestamp2(commandBuffer, stage, queryPool, query);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdWriteBufferMarker2AMD(VkCommandBuffer                   commandBuffer, VkPipelineStageFlags2               stage, VkBuffer                                            dstBuffer, VkDeviceSize                                        dstOffset, uint32_t                                            marker)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_write_buffer_marker2_amd(&cmd_buffer->cmd_queue,
                                       stage, dstBuffer, dstOffset, marker);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdWriteBufferMarker2AMD(VkCommandBuffer                   commandBuffer, VkPipelineStageFlags2               stage, VkBuffer                                            dstBuffer, VkDeviceSize                                        dstOffset, uint32_t                                            marker)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdWriteBufferMarker2AMD(commandBuffer, stage, dstBuffer, dstOffset, marker);
   } else {
      vk_cmd_enqueue_CmdWriteBufferMarker2AMD(commandBuffer, stage, dstBuffer, dstOffset, marker);
   }
}

#ifdef VK_ENABLE_BETA_EXTENSIONS


VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdDecodeVideoKHR(VkCommandBuffer commandBuffer, const VkVideoDecodeInfoKHR* pDecodeInfo)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_decode_video_khr(&cmd_buffer->cmd_queue,
                                       pDecodeInfo);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdDecodeVideoKHR(VkCommandBuffer commandBuffer, const VkVideoDecodeInfoKHR* pDecodeInfo)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdDecodeVideoKHR(commandBuffer, pDecodeInfo);
   } else {
      vk_cmd_enqueue_CmdDecodeVideoKHR(commandBuffer, pDecodeInfo);
   }
}
#endif // VK_ENABLE_BETA_EXTENSIONS

#ifdef VK_ENABLE_BETA_EXTENSIONS


VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdBeginVideoCodingKHR(VkCommandBuffer commandBuffer, const VkVideoBeginCodingInfoKHR* pBeginInfo)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_begin_video_coding_khr(&cmd_buffer->cmd_queue,
                                       pBeginInfo);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdBeginVideoCodingKHR(VkCommandBuffer commandBuffer, const VkVideoBeginCodingInfoKHR* pBeginInfo)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdBeginVideoCodingKHR(commandBuffer, pBeginInfo);
   } else {
      vk_cmd_enqueue_CmdBeginVideoCodingKHR(commandBuffer, pBeginInfo);
   }
}
#endif // VK_ENABLE_BETA_EXTENSIONS

#ifdef VK_ENABLE_BETA_EXTENSIONS


VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdControlVideoCodingKHR(VkCommandBuffer commandBuffer, const VkVideoCodingControlInfoKHR* pCodingControlInfo)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_control_video_coding_khr(&cmd_buffer->cmd_queue,
                                       pCodingControlInfo);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdControlVideoCodingKHR(VkCommandBuffer commandBuffer, const VkVideoCodingControlInfoKHR* pCodingControlInfo)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdControlVideoCodingKHR(commandBuffer, pCodingControlInfo);
   } else {
      vk_cmd_enqueue_CmdControlVideoCodingKHR(commandBuffer, pCodingControlInfo);
   }
}
#endif // VK_ENABLE_BETA_EXTENSIONS

#ifdef VK_ENABLE_BETA_EXTENSIONS


VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdEndVideoCodingKHR(VkCommandBuffer commandBuffer, const VkVideoEndCodingInfoKHR* pEndCodingInfo)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_end_video_coding_khr(&cmd_buffer->cmd_queue,
                                       pEndCodingInfo);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdEndVideoCodingKHR(VkCommandBuffer commandBuffer, const VkVideoEndCodingInfoKHR* pEndCodingInfo)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdEndVideoCodingKHR(commandBuffer, pEndCodingInfo);
   } else {
      vk_cmd_enqueue_CmdEndVideoCodingKHR(commandBuffer, pEndCodingInfo);
   }
}
#endif // VK_ENABLE_BETA_EXTENSIONS

#ifdef VK_ENABLE_BETA_EXTENSIONS


VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdEncodeVideoKHR(VkCommandBuffer commandBuffer, const VkVideoEncodeInfoKHR* pEncodeInfo)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_encode_video_khr(&cmd_buffer->cmd_queue,
                                       pEncodeInfo);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdEncodeVideoKHR(VkCommandBuffer commandBuffer, const VkVideoEncodeInfoKHR* pEncodeInfo)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdEncodeVideoKHR(commandBuffer, pEncodeInfo);
   } else {
      vk_cmd_enqueue_CmdEncodeVideoKHR(commandBuffer, pEncodeInfo);
   }
}
#endif // VK_ENABLE_BETA_EXTENSIONS



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdCuLaunchKernelNVX(VkCommandBuffer commandBuffer, const VkCuLaunchInfoNVX* pLaunchInfo)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_cu_launch_kernel_nvx(&cmd_buffer->cmd_queue,
                                       pLaunchInfo);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdCuLaunchKernelNVX(VkCommandBuffer commandBuffer, const VkCuLaunchInfoNVX* pLaunchInfo)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdCuLaunchKernelNVX(commandBuffer, pLaunchInfo);
   } else {
      vk_cmd_enqueue_CmdCuLaunchKernelNVX(commandBuffer, pLaunchInfo);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdBeginRendering(VkCommandBuffer                   commandBuffer, const VkRenderingInfo*                              pRenderingInfo)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_begin_rendering(&cmd_buffer->cmd_queue,
                                       pRenderingInfo);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdBeginRendering(VkCommandBuffer                   commandBuffer, const VkRenderingInfo*                              pRenderingInfo)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdBeginRendering(commandBuffer, pRenderingInfo);
   } else {
      vk_cmd_enqueue_CmdBeginRendering(commandBuffer, pRenderingInfo);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdEndRendering(VkCommandBuffer                   commandBuffer)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_end_rendering(&cmd_buffer->cmd_queue);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdEndRendering(VkCommandBuffer                   commandBuffer)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdEndRendering(commandBuffer);
   } else {
      vk_cmd_enqueue_CmdEndRendering(commandBuffer);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdBuildMicromapsEXT(VkCommandBuffer                                    commandBuffer, uint32_t infoCount, const VkMicromapBuildInfoEXT* pInfos)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_build_micromaps_ext(&cmd_buffer->cmd_queue,
                                       infoCount, pInfos);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdBuildMicromapsEXT(VkCommandBuffer                                    commandBuffer, uint32_t infoCount, const VkMicromapBuildInfoEXT* pInfos)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdBuildMicromapsEXT(commandBuffer, infoCount, pInfos);
   } else {
      vk_cmd_enqueue_CmdBuildMicromapsEXT(commandBuffer, infoCount, pInfos);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdCopyMicromapEXT(VkCommandBuffer commandBuffer, const VkCopyMicromapInfoEXT* pInfo)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_copy_micromap_ext(&cmd_buffer->cmd_queue,
                                       pInfo);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdCopyMicromapEXT(VkCommandBuffer commandBuffer, const VkCopyMicromapInfoEXT* pInfo)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdCopyMicromapEXT(commandBuffer, pInfo);
   } else {
      vk_cmd_enqueue_CmdCopyMicromapEXT(commandBuffer, pInfo);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdCopyMicromapToMemoryEXT(VkCommandBuffer commandBuffer, const VkCopyMicromapToMemoryInfoEXT* pInfo)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_copy_micromap_to_memory_ext(&cmd_buffer->cmd_queue,
                                       pInfo);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdCopyMicromapToMemoryEXT(VkCommandBuffer commandBuffer, const VkCopyMicromapToMemoryInfoEXT* pInfo)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdCopyMicromapToMemoryEXT(commandBuffer, pInfo);
   } else {
      vk_cmd_enqueue_CmdCopyMicromapToMemoryEXT(commandBuffer, pInfo);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdCopyMemoryToMicromapEXT(VkCommandBuffer commandBuffer, const VkCopyMemoryToMicromapInfoEXT* pInfo)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_copy_memory_to_micromap_ext(&cmd_buffer->cmd_queue,
                                       pInfo);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdCopyMemoryToMicromapEXT(VkCommandBuffer commandBuffer, const VkCopyMemoryToMicromapInfoEXT* pInfo)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdCopyMemoryToMicromapEXT(commandBuffer, pInfo);
   } else {
      vk_cmd_enqueue_CmdCopyMemoryToMicromapEXT(commandBuffer, pInfo);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdWriteMicromapsPropertiesEXT(VkCommandBuffer commandBuffer, uint32_t micromapCount, const VkMicromapEXT* pMicromaps, VkQueryType queryType, VkQueryPool queryPool, uint32_t firstQuery)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_write_micromaps_properties_ext(&cmd_buffer->cmd_queue,
                                       micromapCount, pMicromaps, queryType, queryPool, firstQuery);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdWriteMicromapsPropertiesEXT(VkCommandBuffer commandBuffer, uint32_t micromapCount, const VkMicromapEXT* pMicromaps, VkQueryType queryType, VkQueryPool queryPool, uint32_t firstQuery)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdWriteMicromapsPropertiesEXT(commandBuffer, micromapCount, pMicromaps, queryType, queryPool, firstQuery);
   } else {
      vk_cmd_enqueue_CmdWriteMicromapsPropertiesEXT(commandBuffer, micromapCount, pMicromaps, queryType, queryPool, firstQuery);
   }
}



VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_CmdOpticalFlowExecuteNV(VkCommandBuffer commandBuffer, VkOpticalFlowSessionNV session, const VkOpticalFlowExecuteInfoNV* pExecuteInfo)
{
   VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (vk_command_buffer_has_error(cmd_buffer))
      return;
   VkResult result = vk_enqueue_cmd_optical_flow_execute_nv(&cmd_buffer->cmd_queue,
                                       session, pExecuteInfo);
   if (unlikely(result != VK_SUCCESS))
      vk_command_buffer_set_error(cmd_buffer, result);
}

VKAPI_ATTR void VKAPI_CALL
vk_cmd_enqueue_unless_primary_CmdOpticalFlowExecuteNV(VkCommandBuffer commandBuffer, VkOpticalFlowSessionNV session, const VkOpticalFlowExecuteInfoNV* pExecuteInfo)
{
    VK_FROM_HANDLE(vk_command_buffer, cmd_buffer, commandBuffer);

   if (cmd_buffer->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
      const struct vk_device_dispatch_table *disp =
         cmd_buffer->base.device->command_dispatch_table;

      disp->CmdOpticalFlowExecuteNV(commandBuffer, session, pExecuteInfo);
   } else {
      vk_cmd_enqueue_CmdOpticalFlowExecuteNV(commandBuffer, session, pExecuteInfo);
   }
}
