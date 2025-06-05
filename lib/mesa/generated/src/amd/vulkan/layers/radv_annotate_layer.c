
/* Copyright © 2021 Intel Corporation
 * Copyright © 2024 Valve Corporation
 *
 * SPDX-License-Identifier: MIT
 */

/* This file generated from radv_annotate_layer_gen.py, don't edit directly. */

#include "radv_cmd_buffer.h"
#include "radv_entrypoints.h"

#define ANNOTATE(command, ...)    struct radv_cmd_buffer *cmd_buffer = radv_cmd_buffer_from_handle(commandBuffer);    struct radv_device *device = radv_cmd_buffer_device(cmd_buffer);    radv_cmd_buffer_annotate(cmd_buffer, #command);    device->layer_dispatch.annotate.command(__VA_ARGS__)

VKAPI_ATTR void VKAPI_CALL
annotate_CmdBindPipeline(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipeline pipeline)
{
   ANNOTATE(CmdBindPipeline, commandBuffer, pipelineBindPoint, pipeline);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdSetAttachmentFeedbackLoopEnableEXT(VkCommandBuffer commandBuffer, VkImageAspectFlags aspectMask)
{
   ANNOTATE(CmdSetAttachmentFeedbackLoopEnableEXT, commandBuffer, aspectMask);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdSetViewport(VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const VkViewport* pViewports)
{
   ANNOTATE(CmdSetViewport, commandBuffer, firstViewport, viewportCount, pViewports);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdSetScissor(VkCommandBuffer commandBuffer, uint32_t firstScissor, uint32_t scissorCount, const VkRect2D* pScissors)
{
   ANNOTATE(CmdSetScissor, commandBuffer, firstScissor, scissorCount, pScissors);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdSetLineWidth(VkCommandBuffer commandBuffer, float lineWidth)
{
   ANNOTATE(CmdSetLineWidth, commandBuffer, lineWidth);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdSetDepthBias(VkCommandBuffer commandBuffer, float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor)
{
   ANNOTATE(CmdSetDepthBias, commandBuffer, depthBiasConstantFactor, depthBiasClamp, depthBiasSlopeFactor);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdSetBlendConstants(VkCommandBuffer commandBuffer, const float blendConstants[4])
{
   ANNOTATE(CmdSetBlendConstants, commandBuffer, blendConstants);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdSetDepthBounds(VkCommandBuffer commandBuffer, float minDepthBounds, float maxDepthBounds)
{
   ANNOTATE(CmdSetDepthBounds, commandBuffer, minDepthBounds, maxDepthBounds);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdSetStencilCompareMask(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t compareMask)
{
   ANNOTATE(CmdSetStencilCompareMask, commandBuffer, faceMask, compareMask);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdSetStencilWriteMask(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t writeMask)
{
   ANNOTATE(CmdSetStencilWriteMask, commandBuffer, faceMask, writeMask);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdSetStencilReference(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t reference)
{
   ANNOTATE(CmdSetStencilReference, commandBuffer, faceMask, reference);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdBindDescriptorSets(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t firstSet, uint32_t descriptorSetCount, const VkDescriptorSet* pDescriptorSets, uint32_t dynamicOffsetCount, const uint32_t* pDynamicOffsets)
{
   ANNOTATE(CmdBindDescriptorSets, commandBuffer, pipelineBindPoint, layout, firstSet, descriptorSetCount, pDescriptorSets, dynamicOffsetCount, pDynamicOffsets);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdBindIndexBuffer(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkIndexType indexType)
{
   ANNOTATE(CmdBindIndexBuffer, commandBuffer, buffer, offset, indexType);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdBindVertexBuffers(VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets)
{
   ANNOTATE(CmdBindVertexBuffers, commandBuffer, firstBinding, bindingCount, pBuffers, pOffsets);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdDraw(VkCommandBuffer commandBuffer, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance)
{
   ANNOTATE(CmdDraw, commandBuffer, vertexCount, instanceCount, firstVertex, firstInstance);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdDrawIndexed(VkCommandBuffer commandBuffer, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance)
{
   ANNOTATE(CmdDrawIndexed, commandBuffer, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdDrawMultiEXT(VkCommandBuffer commandBuffer, uint32_t drawCount, const VkMultiDrawInfoEXT* pVertexInfo, uint32_t instanceCount, uint32_t firstInstance, uint32_t stride)
{
   ANNOTATE(CmdDrawMultiEXT, commandBuffer, drawCount, pVertexInfo, instanceCount, firstInstance, stride);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdDrawMultiIndexedEXT(VkCommandBuffer commandBuffer, uint32_t drawCount, const VkMultiDrawIndexedInfoEXT* pIndexInfo, uint32_t instanceCount, uint32_t firstInstance, uint32_t stride, const int32_t* pVertexOffset)
{
   ANNOTATE(CmdDrawMultiIndexedEXT, commandBuffer, drawCount, pIndexInfo, instanceCount, firstInstance, stride, pVertexOffset);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdDrawIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride)
{
   ANNOTATE(CmdDrawIndirect, commandBuffer, buffer, offset, drawCount, stride);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdDrawIndexedIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride)
{
   ANNOTATE(CmdDrawIndexedIndirect, commandBuffer, buffer, offset, drawCount, stride);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdDispatchIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset)
{
   ANNOTATE(CmdDispatchIndirect, commandBuffer, buffer, offset);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdSubpassShadingHUAWEI(VkCommandBuffer commandBuffer)
{
   ANNOTATE(CmdSubpassShadingHUAWEI, commandBuffer);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdDrawClusterHUAWEI(VkCommandBuffer commandBuffer, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ)
{
   ANNOTATE(CmdDrawClusterHUAWEI, commandBuffer, groupCountX, groupCountY, groupCountZ);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdDrawClusterIndirectHUAWEI(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset)
{
   ANNOTATE(CmdDrawClusterIndirectHUAWEI, commandBuffer, buffer, offset);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdUpdatePipelineIndirectBufferNV(VkCommandBuffer commandBuffer, VkPipelineBindPoint           pipelineBindPoint, VkPipeline                    pipeline)
{
   ANNOTATE(CmdUpdatePipelineIndirectBufferNV, commandBuffer, pipelineBindPoint, pipeline);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdCopyBuffer(VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkBuffer dstBuffer, uint32_t regionCount, const VkBufferCopy* pRegions)
{
   ANNOTATE(CmdCopyBuffer, commandBuffer, srcBuffer, dstBuffer, regionCount, pRegions);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdCopyImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageCopy* pRegions)
{
   ANNOTATE(CmdCopyImage, commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdBlitImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageBlit* pRegions, VkFilter filter)
{
   ANNOTATE(CmdBlitImage, commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions, filter);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdCopyBufferToImage(VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkBufferImageCopy* pRegions)
{
   ANNOTATE(CmdCopyBufferToImage, commandBuffer, srcBuffer, dstImage, dstImageLayout, regionCount, pRegions);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdCopyImageToBuffer(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkBuffer dstBuffer, uint32_t regionCount, const VkBufferImageCopy* pRegions)
{
   ANNOTATE(CmdCopyImageToBuffer, commandBuffer, srcImage, srcImageLayout, dstBuffer, regionCount, pRegions);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdCopyMemoryIndirectNV(VkCommandBuffer commandBuffer, VkDeviceAddress copyBufferAddress, uint32_t copyCount, uint32_t stride)
{
   ANNOTATE(CmdCopyMemoryIndirectNV, commandBuffer, copyBufferAddress, copyCount, stride);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdCopyMemoryToImageIndirectNV(VkCommandBuffer commandBuffer, VkDeviceAddress copyBufferAddress, uint32_t copyCount, uint32_t stride, VkImage dstImage, VkImageLayout dstImageLayout, const VkImageSubresourceLayers* pImageSubresources)
{
   ANNOTATE(CmdCopyMemoryToImageIndirectNV, commandBuffer, copyBufferAddress, copyCount, stride, dstImage, dstImageLayout, pImageSubresources);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdUpdateBuffer(VkCommandBuffer commandBuffer, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize dataSize, const void* pData)
{
   ANNOTATE(CmdUpdateBuffer, commandBuffer, dstBuffer, dstOffset, dataSize, pData);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdFillBuffer(VkCommandBuffer commandBuffer, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize size, uint32_t data)
{
   ANNOTATE(CmdFillBuffer, commandBuffer, dstBuffer, dstOffset, size, data);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdClearColorImage(VkCommandBuffer commandBuffer, VkImage image, VkImageLayout imageLayout, const VkClearColorValue* pColor, uint32_t rangeCount, const VkImageSubresourceRange* pRanges)
{
   ANNOTATE(CmdClearColorImage, commandBuffer, image, imageLayout, pColor, rangeCount, pRanges);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdClearDepthStencilImage(VkCommandBuffer commandBuffer, VkImage image, VkImageLayout imageLayout, const VkClearDepthStencilValue* pDepthStencil, uint32_t rangeCount, const VkImageSubresourceRange* pRanges)
{
   ANNOTATE(CmdClearDepthStencilImage, commandBuffer, image, imageLayout, pDepthStencil, rangeCount, pRanges);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdClearAttachments(VkCommandBuffer commandBuffer, uint32_t attachmentCount, const VkClearAttachment* pAttachments, uint32_t rectCount, const VkClearRect* pRects)
{
   ANNOTATE(CmdClearAttachments, commandBuffer, attachmentCount, pAttachments, rectCount, pRects);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdResolveImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageResolve* pRegions)
{
   ANNOTATE(CmdResolveImage, commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdSetEvent(VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags stageMask)
{
   ANNOTATE(CmdSetEvent, commandBuffer, event, stageMask);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdResetEvent(VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags stageMask)
{
   ANNOTATE(CmdResetEvent, commandBuffer, event, stageMask);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdWaitEvents(VkCommandBuffer commandBuffer, uint32_t eventCount, const VkEvent* pEvents, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier* pImageMemoryBarriers)
{
   ANNOTATE(CmdWaitEvents, commandBuffer, eventCount, pEvents, srcStageMask, dstStageMask, memoryBarrierCount, pMemoryBarriers, bufferMemoryBarrierCount, pBufferMemoryBarriers, imageMemoryBarrierCount, pImageMemoryBarriers);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdPipelineBarrier(VkCommandBuffer commandBuffer, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, VkDependencyFlags dependencyFlags, uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier* pImageMemoryBarriers)
{
   ANNOTATE(CmdPipelineBarrier, commandBuffer, srcStageMask, dstStageMask, dependencyFlags, memoryBarrierCount, pMemoryBarriers, bufferMemoryBarrierCount, pBufferMemoryBarriers, imageMemoryBarrierCount, pImageMemoryBarriers);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdBeginQuery(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, VkQueryControlFlags flags)
{
   ANNOTATE(CmdBeginQuery, commandBuffer, queryPool, query, flags);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdEndQuery(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query)
{
   ANNOTATE(CmdEndQuery, commandBuffer, queryPool, query);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdBeginConditionalRenderingEXT(VkCommandBuffer commandBuffer, const VkConditionalRenderingBeginInfoEXT* pConditionalRenderingBegin)
{
   ANNOTATE(CmdBeginConditionalRenderingEXT, commandBuffer, pConditionalRenderingBegin);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdEndConditionalRenderingEXT(VkCommandBuffer commandBuffer)
{
   ANNOTATE(CmdEndConditionalRenderingEXT, commandBuffer);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdResetQueryPool(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount)
{
   ANNOTATE(CmdResetQueryPool, commandBuffer, queryPool, firstQuery, queryCount);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdWriteTimestamp(VkCommandBuffer commandBuffer, VkPipelineStageFlagBits pipelineStage, VkQueryPool queryPool, uint32_t query)
{
   ANNOTATE(CmdWriteTimestamp, commandBuffer, pipelineStage, queryPool, query);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdCopyQueryPoolResults(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize stride, VkQueryResultFlags flags)
{
   ANNOTATE(CmdCopyQueryPoolResults, commandBuffer, queryPool, firstQuery, queryCount, dstBuffer, dstOffset, stride, flags);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdPushConstants(VkCommandBuffer commandBuffer, VkPipelineLayout layout, VkShaderStageFlags stageFlags, uint32_t offset, uint32_t size, const void* pValues)
{
   ANNOTATE(CmdPushConstants, commandBuffer, layout, stageFlags, offset, size, pValues);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdNextSubpass(VkCommandBuffer commandBuffer, VkSubpassContents contents)
{
   ANNOTATE(CmdNextSubpass, commandBuffer, contents);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdExecuteCommands(VkCommandBuffer commandBuffer, uint32_t commandBufferCount, const VkCommandBuffer* pCommandBuffers)
{
   ANNOTATE(CmdExecuteCommands, commandBuffer, commandBufferCount, pCommandBuffers);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdDebugMarkerBeginEXT(VkCommandBuffer commandBuffer, const VkDebugMarkerMarkerInfoEXT* pMarkerInfo)
{
   ANNOTATE(CmdDebugMarkerBeginEXT, commandBuffer, pMarkerInfo);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdDebugMarkerEndEXT(VkCommandBuffer commandBuffer)
{
   ANNOTATE(CmdDebugMarkerEndEXT, commandBuffer);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdDebugMarkerInsertEXT(VkCommandBuffer commandBuffer, const VkDebugMarkerMarkerInfoEXT* pMarkerInfo)
{
   ANNOTATE(CmdDebugMarkerInsertEXT, commandBuffer, pMarkerInfo);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdExecuteGeneratedCommandsNV(VkCommandBuffer commandBuffer, VkBool32 isPreprocessed, const VkGeneratedCommandsInfoNV* pGeneratedCommandsInfo)
{
   ANNOTATE(CmdExecuteGeneratedCommandsNV, commandBuffer, isPreprocessed, pGeneratedCommandsInfo);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdPreprocessGeneratedCommandsNV(VkCommandBuffer commandBuffer, const VkGeneratedCommandsInfoNV* pGeneratedCommandsInfo)
{
   ANNOTATE(CmdPreprocessGeneratedCommandsNV, commandBuffer, pGeneratedCommandsInfo);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdBindPipelineShaderGroupNV(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipeline pipeline, uint32_t groupIndex)
{
   ANNOTATE(CmdBindPipelineShaderGroupNV, commandBuffer, pipelineBindPoint, pipeline, groupIndex);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdExecuteGeneratedCommandsEXT(VkCommandBuffer commandBuffer, VkBool32 isPreprocessed, const VkGeneratedCommandsInfoEXT* pGeneratedCommandsInfo)
{
   ANNOTATE(CmdExecuteGeneratedCommandsEXT, commandBuffer, isPreprocessed, pGeneratedCommandsInfo);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdPreprocessGeneratedCommandsEXT(VkCommandBuffer commandBuffer, const VkGeneratedCommandsInfoEXT* pGeneratedCommandsInfo, VkCommandBuffer stateCommandBuffer)
{
   ANNOTATE(CmdPreprocessGeneratedCommandsEXT, commandBuffer, pGeneratedCommandsInfo, stateCommandBuffer);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdPushDescriptorSet(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t set, uint32_t descriptorWriteCount, const VkWriteDescriptorSet* pDescriptorWrites)
{
   ANNOTATE(CmdPushDescriptorSet, commandBuffer, pipelineBindPoint, layout, set, descriptorWriteCount, pDescriptorWrites);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdSetDeviceMask(VkCommandBuffer commandBuffer, uint32_t deviceMask)
{
   ANNOTATE(CmdSetDeviceMask, commandBuffer, deviceMask);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdDispatchBase(VkCommandBuffer commandBuffer, uint32_t baseGroupX, uint32_t baseGroupY, uint32_t baseGroupZ, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ)
{
   ANNOTATE(CmdDispatchBase, commandBuffer, baseGroupX, baseGroupY, baseGroupZ, groupCountX, groupCountY, groupCountZ);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdPushDescriptorSetWithTemplate(VkCommandBuffer commandBuffer, VkDescriptorUpdateTemplate descriptorUpdateTemplate, VkPipelineLayout layout, uint32_t set, const void* pData)
{
   ANNOTATE(CmdPushDescriptorSetWithTemplate, commandBuffer, descriptorUpdateTemplate, layout, set, pData);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdSetViewportWScalingNV(VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const VkViewportWScalingNV* pViewportWScalings)
{
   ANNOTATE(CmdSetViewportWScalingNV, commandBuffer, firstViewport, viewportCount, pViewportWScalings);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdSetDiscardRectangleEXT(VkCommandBuffer commandBuffer, uint32_t firstDiscardRectangle, uint32_t discardRectangleCount, const VkRect2D* pDiscardRectangles)
{
   ANNOTATE(CmdSetDiscardRectangleEXT, commandBuffer, firstDiscardRectangle, discardRectangleCount, pDiscardRectangles);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdSetDiscardRectangleEnableEXT(VkCommandBuffer commandBuffer, VkBool32 discardRectangleEnable)
{
   ANNOTATE(CmdSetDiscardRectangleEnableEXT, commandBuffer, discardRectangleEnable);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdSetDiscardRectangleModeEXT(VkCommandBuffer commandBuffer, VkDiscardRectangleModeEXT discardRectangleMode)
{
   ANNOTATE(CmdSetDiscardRectangleModeEXT, commandBuffer, discardRectangleMode);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdSetSampleLocationsEXT(VkCommandBuffer commandBuffer, const VkSampleLocationsInfoEXT* pSampleLocationsInfo)
{
   ANNOTATE(CmdSetSampleLocationsEXT, commandBuffer, pSampleLocationsInfo);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdBeginDebugUtilsLabelEXT(VkCommandBuffer commandBuffer, const VkDebugUtilsLabelEXT* pLabelInfo)
{
   ANNOTATE(CmdBeginDebugUtilsLabelEXT, commandBuffer, pLabelInfo);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdEndDebugUtilsLabelEXT(VkCommandBuffer commandBuffer)
{
   ANNOTATE(CmdEndDebugUtilsLabelEXT, commandBuffer);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdInsertDebugUtilsLabelEXT(VkCommandBuffer commandBuffer, const VkDebugUtilsLabelEXT* pLabelInfo)
{
   ANNOTATE(CmdInsertDebugUtilsLabelEXT, commandBuffer, pLabelInfo);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdWriteBufferMarkerAMD(VkCommandBuffer commandBuffer, VkPipelineStageFlagBits pipelineStage, VkBuffer dstBuffer, VkDeviceSize dstOffset, uint32_t marker)
{
   ANNOTATE(CmdWriteBufferMarkerAMD, commandBuffer, pipelineStage, dstBuffer, dstOffset, marker);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdDrawIndirectCount(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride)
{
   ANNOTATE(CmdDrawIndirectCount, commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdDrawIndexedIndirectCount(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride)
{
   ANNOTATE(CmdDrawIndexedIndirectCount, commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdSetCheckpointNV(VkCommandBuffer commandBuffer, const void* pCheckpointMarker)
{
   ANNOTATE(CmdSetCheckpointNV, commandBuffer, pCheckpointMarker);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdBindTransformFeedbackBuffersEXT(VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets, const VkDeviceSize* pSizes)
{
   ANNOTATE(CmdBindTransformFeedbackBuffersEXT, commandBuffer, firstBinding, bindingCount, pBuffers, pOffsets, pSizes);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdBeginTransformFeedbackEXT(VkCommandBuffer commandBuffer, uint32_t firstCounterBuffer, uint32_t counterBufferCount, const VkBuffer* pCounterBuffers, const VkDeviceSize* pCounterBufferOffsets)
{
   ANNOTATE(CmdBeginTransformFeedbackEXT, commandBuffer, firstCounterBuffer, counterBufferCount, pCounterBuffers, pCounterBufferOffsets);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdEndTransformFeedbackEXT(VkCommandBuffer commandBuffer, uint32_t firstCounterBuffer, uint32_t counterBufferCount, const VkBuffer* pCounterBuffers, const VkDeviceSize* pCounterBufferOffsets)
{
   ANNOTATE(CmdEndTransformFeedbackEXT, commandBuffer, firstCounterBuffer, counterBufferCount, pCounterBuffers, pCounterBufferOffsets);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdBeginQueryIndexedEXT(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, VkQueryControlFlags flags, uint32_t index)
{
   ANNOTATE(CmdBeginQueryIndexedEXT, commandBuffer, queryPool, query, flags, index);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdEndQueryIndexedEXT(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, uint32_t index)
{
   ANNOTATE(CmdEndQueryIndexedEXT, commandBuffer, queryPool, query, index);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdDrawIndirectByteCountEXT(VkCommandBuffer commandBuffer, uint32_t instanceCount, uint32_t firstInstance, VkBuffer counterBuffer, VkDeviceSize counterBufferOffset, uint32_t counterOffset, uint32_t vertexStride)
{
   ANNOTATE(CmdDrawIndirectByteCountEXT, commandBuffer, instanceCount, firstInstance, counterBuffer, counterBufferOffset, counterOffset, vertexStride);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdSetExclusiveScissorNV(VkCommandBuffer commandBuffer, uint32_t firstExclusiveScissor, uint32_t exclusiveScissorCount, const VkRect2D* pExclusiveScissors)
{
   ANNOTATE(CmdSetExclusiveScissorNV, commandBuffer, firstExclusiveScissor, exclusiveScissorCount, pExclusiveScissors);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdSetExclusiveScissorEnableNV(VkCommandBuffer commandBuffer, uint32_t firstExclusiveScissor, uint32_t exclusiveScissorCount, const VkBool32* pExclusiveScissorEnables)
{
   ANNOTATE(CmdSetExclusiveScissorEnableNV, commandBuffer, firstExclusiveScissor, exclusiveScissorCount, pExclusiveScissorEnables);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdBindShadingRateImageNV(VkCommandBuffer commandBuffer, VkImageView imageView, VkImageLayout imageLayout)
{
   ANNOTATE(CmdBindShadingRateImageNV, commandBuffer, imageView, imageLayout);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdSetViewportShadingRatePaletteNV(VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const VkShadingRatePaletteNV* pShadingRatePalettes)
{
   ANNOTATE(CmdSetViewportShadingRatePaletteNV, commandBuffer, firstViewport, viewportCount, pShadingRatePalettes);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdSetCoarseSampleOrderNV(VkCommandBuffer commandBuffer, VkCoarseSampleOrderTypeNV sampleOrderType, uint32_t customSampleOrderCount, const VkCoarseSampleOrderCustomNV* pCustomSampleOrders)
{
   ANNOTATE(CmdSetCoarseSampleOrderNV, commandBuffer, sampleOrderType, customSampleOrderCount, pCustomSampleOrders);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdDrawMeshTasksNV(VkCommandBuffer commandBuffer, uint32_t taskCount, uint32_t firstTask)
{
   ANNOTATE(CmdDrawMeshTasksNV, commandBuffer, taskCount, firstTask);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdDrawMeshTasksIndirectNV(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride)
{
   ANNOTATE(CmdDrawMeshTasksIndirectNV, commandBuffer, buffer, offset, drawCount, stride);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdDrawMeshTasksIndirectCountNV(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride)
{
   ANNOTATE(CmdDrawMeshTasksIndirectCountNV, commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdDrawMeshTasksEXT(VkCommandBuffer commandBuffer, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ)
{
   ANNOTATE(CmdDrawMeshTasksEXT, commandBuffer, groupCountX, groupCountY, groupCountZ);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdDrawMeshTasksIndirectEXT(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride)
{
   ANNOTATE(CmdDrawMeshTasksIndirectEXT, commandBuffer, buffer, offset, drawCount, stride);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdDrawMeshTasksIndirectCountEXT(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride)
{
   ANNOTATE(CmdDrawMeshTasksIndirectCountEXT, commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdBindInvocationMaskHUAWEI(VkCommandBuffer commandBuffer, VkImageView imageView, VkImageLayout imageLayout)
{
   ANNOTATE(CmdBindInvocationMaskHUAWEI, commandBuffer, imageView, imageLayout);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdCopyAccelerationStructureNV(VkCommandBuffer commandBuffer, VkAccelerationStructureNV dst, VkAccelerationStructureNV src, VkCopyAccelerationStructureModeKHR mode)
{
   ANNOTATE(CmdCopyAccelerationStructureNV, commandBuffer, dst, src, mode);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdCopyAccelerationStructureKHR(VkCommandBuffer commandBuffer, const VkCopyAccelerationStructureInfoKHR* pInfo)
{
   ANNOTATE(CmdCopyAccelerationStructureKHR, commandBuffer, pInfo);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdCopyAccelerationStructureToMemoryKHR(VkCommandBuffer commandBuffer, const VkCopyAccelerationStructureToMemoryInfoKHR* pInfo)
{
   ANNOTATE(CmdCopyAccelerationStructureToMemoryKHR, commandBuffer, pInfo);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdCopyMemoryToAccelerationStructureKHR(VkCommandBuffer commandBuffer, const VkCopyMemoryToAccelerationStructureInfoKHR* pInfo)
{
   ANNOTATE(CmdCopyMemoryToAccelerationStructureKHR, commandBuffer, pInfo);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdWriteAccelerationStructuresPropertiesKHR(VkCommandBuffer commandBuffer, uint32_t accelerationStructureCount, const VkAccelerationStructureKHR* pAccelerationStructures, VkQueryType queryType, VkQueryPool queryPool, uint32_t firstQuery)
{
   ANNOTATE(CmdWriteAccelerationStructuresPropertiesKHR, commandBuffer, accelerationStructureCount, pAccelerationStructures, queryType, queryPool, firstQuery);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdWriteAccelerationStructuresPropertiesNV(VkCommandBuffer commandBuffer, uint32_t accelerationStructureCount, const VkAccelerationStructureNV* pAccelerationStructures, VkQueryType queryType, VkQueryPool queryPool, uint32_t firstQuery)
{
   ANNOTATE(CmdWriteAccelerationStructuresPropertiesNV, commandBuffer, accelerationStructureCount, pAccelerationStructures, queryType, queryPool, firstQuery);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdBuildAccelerationStructureNV(VkCommandBuffer commandBuffer, const VkAccelerationStructureInfoNV* pInfo, VkBuffer instanceData, VkDeviceSize instanceOffset, VkBool32 update, VkAccelerationStructureNV dst, VkAccelerationStructureNV src, VkBuffer scratch, VkDeviceSize scratchOffset)
{
   ANNOTATE(CmdBuildAccelerationStructureNV, commandBuffer, pInfo, instanceData, instanceOffset, update, dst, src, scratch, scratchOffset);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdTraceRaysKHR(VkCommandBuffer commandBuffer, const VkStridedDeviceAddressRegionKHR* pRaygenShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pMissShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pHitShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pCallableShaderBindingTable, uint32_t width, uint32_t height, uint32_t depth)
{
   ANNOTATE(CmdTraceRaysKHR, commandBuffer, pRaygenShaderBindingTable, pMissShaderBindingTable, pHitShaderBindingTable, pCallableShaderBindingTable, width, height, depth);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdTraceRaysNV(VkCommandBuffer commandBuffer, VkBuffer raygenShaderBindingTableBuffer, VkDeviceSize raygenShaderBindingOffset, VkBuffer missShaderBindingTableBuffer, VkDeviceSize missShaderBindingOffset, VkDeviceSize missShaderBindingStride, VkBuffer hitShaderBindingTableBuffer, VkDeviceSize hitShaderBindingOffset, VkDeviceSize hitShaderBindingStride, VkBuffer callableShaderBindingTableBuffer, VkDeviceSize callableShaderBindingOffset, VkDeviceSize callableShaderBindingStride, uint32_t width, uint32_t height, uint32_t depth)
{
   ANNOTATE(CmdTraceRaysNV, commandBuffer, raygenShaderBindingTableBuffer, raygenShaderBindingOffset, missShaderBindingTableBuffer, missShaderBindingOffset, missShaderBindingStride, hitShaderBindingTableBuffer, hitShaderBindingOffset, hitShaderBindingStride, callableShaderBindingTableBuffer, callableShaderBindingOffset, callableShaderBindingStride, width, height, depth);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdTraceRaysIndirectKHR(VkCommandBuffer commandBuffer, const VkStridedDeviceAddressRegionKHR* pRaygenShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pMissShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pHitShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pCallableShaderBindingTable, VkDeviceAddress indirectDeviceAddress)
{
   ANNOTATE(CmdTraceRaysIndirectKHR, commandBuffer, pRaygenShaderBindingTable, pMissShaderBindingTable, pHitShaderBindingTable, pCallableShaderBindingTable, indirectDeviceAddress);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdSetRayTracingPipelineStackSizeKHR(VkCommandBuffer commandBuffer, uint32_t pipelineStackSize)
{
   ANNOTATE(CmdSetRayTracingPipelineStackSizeKHR, commandBuffer, pipelineStackSize);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdSetLineStipple(VkCommandBuffer commandBuffer, uint32_t lineStippleFactor, uint16_t lineStipplePattern)
{
   ANNOTATE(CmdSetLineStipple, commandBuffer, lineStippleFactor, lineStipplePattern);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdBuildAccelerationStructuresKHR(VkCommandBuffer                                    commandBuffer, uint32_t infoCount, const VkAccelerationStructureBuildGeometryInfoKHR* pInfos, const VkAccelerationStructureBuildRangeInfoKHR* const* ppBuildRangeInfos)
{
   ANNOTATE(CmdBuildAccelerationStructuresKHR, commandBuffer, infoCount, pInfos, ppBuildRangeInfos);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdBuildAccelerationStructuresIndirectKHR(VkCommandBuffer                  commandBuffer, uint32_t                                           infoCount, const VkAccelerationStructureBuildGeometryInfoKHR* pInfos, const VkDeviceAddress*             pIndirectDeviceAddresses, const uint32_t*                    pIndirectStrides, const uint32_t* const*             ppMaxPrimitiveCounts)
{
   ANNOTATE(CmdBuildAccelerationStructuresIndirectKHR, commandBuffer, infoCount, pInfos, pIndirectDeviceAddresses, pIndirectStrides, ppMaxPrimitiveCounts);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdSetCullMode(VkCommandBuffer commandBuffer, VkCullModeFlags cullMode)
{
   ANNOTATE(CmdSetCullMode, commandBuffer, cullMode);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdSetFrontFace(VkCommandBuffer commandBuffer, VkFrontFace frontFace)
{
   ANNOTATE(CmdSetFrontFace, commandBuffer, frontFace);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdSetPrimitiveTopology(VkCommandBuffer commandBuffer, VkPrimitiveTopology primitiveTopology)
{
   ANNOTATE(CmdSetPrimitiveTopology, commandBuffer, primitiveTopology);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdSetViewportWithCount(VkCommandBuffer commandBuffer, uint32_t viewportCount, const VkViewport* pViewports)
{
   ANNOTATE(CmdSetViewportWithCount, commandBuffer, viewportCount, pViewports);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdSetScissorWithCount(VkCommandBuffer commandBuffer, uint32_t scissorCount, const VkRect2D* pScissors)
{
   ANNOTATE(CmdSetScissorWithCount, commandBuffer, scissorCount, pScissors);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdSetDepthTestEnable(VkCommandBuffer commandBuffer, VkBool32 depthTestEnable)
{
   ANNOTATE(CmdSetDepthTestEnable, commandBuffer, depthTestEnable);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdSetDepthWriteEnable(VkCommandBuffer commandBuffer, VkBool32 depthWriteEnable)
{
   ANNOTATE(CmdSetDepthWriteEnable, commandBuffer, depthWriteEnable);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdSetDepthCompareOp(VkCommandBuffer commandBuffer, VkCompareOp depthCompareOp)
{
   ANNOTATE(CmdSetDepthCompareOp, commandBuffer, depthCompareOp);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdSetDepthBoundsTestEnable(VkCommandBuffer commandBuffer, VkBool32 depthBoundsTestEnable)
{
   ANNOTATE(CmdSetDepthBoundsTestEnable, commandBuffer, depthBoundsTestEnable);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdSetStencilTestEnable(VkCommandBuffer commandBuffer, VkBool32 stencilTestEnable)
{
   ANNOTATE(CmdSetStencilTestEnable, commandBuffer, stencilTestEnable);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdSetStencilOp(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, VkStencilOp failOp, VkStencilOp passOp, VkStencilOp depthFailOp, VkCompareOp compareOp)
{
   ANNOTATE(CmdSetStencilOp, commandBuffer, faceMask, failOp, passOp, depthFailOp, compareOp);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdSetPatchControlPointsEXT(VkCommandBuffer commandBuffer, uint32_t patchControlPoints)
{
   ANNOTATE(CmdSetPatchControlPointsEXT, commandBuffer, patchControlPoints);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdSetRasterizerDiscardEnable(VkCommandBuffer commandBuffer, VkBool32 rasterizerDiscardEnable)
{
   ANNOTATE(CmdSetRasterizerDiscardEnable, commandBuffer, rasterizerDiscardEnable);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdSetDepthBiasEnable(VkCommandBuffer commandBuffer, VkBool32 depthBiasEnable)
{
   ANNOTATE(CmdSetDepthBiasEnable, commandBuffer, depthBiasEnable);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdSetLogicOpEXT(VkCommandBuffer commandBuffer, VkLogicOp logicOp)
{
   ANNOTATE(CmdSetLogicOpEXT, commandBuffer, logicOp);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdSetPrimitiveRestartEnable(VkCommandBuffer commandBuffer, VkBool32 primitiveRestartEnable)
{
   ANNOTATE(CmdSetPrimitiveRestartEnable, commandBuffer, primitiveRestartEnable);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdSetTessellationDomainOriginEXT(VkCommandBuffer commandBuffer, VkTessellationDomainOrigin domainOrigin)
{
   ANNOTATE(CmdSetTessellationDomainOriginEXT, commandBuffer, domainOrigin);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdSetDepthClampEnableEXT(VkCommandBuffer commandBuffer, VkBool32 depthClampEnable)
{
   ANNOTATE(CmdSetDepthClampEnableEXT, commandBuffer, depthClampEnable);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdSetPolygonModeEXT(VkCommandBuffer commandBuffer, VkPolygonMode polygonMode)
{
   ANNOTATE(CmdSetPolygonModeEXT, commandBuffer, polygonMode);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdSetRasterizationSamplesEXT(VkCommandBuffer commandBuffer, VkSampleCountFlagBits  rasterizationSamples)
{
   ANNOTATE(CmdSetRasterizationSamplesEXT, commandBuffer, rasterizationSamples);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdSetSampleMaskEXT(VkCommandBuffer commandBuffer, VkSampleCountFlagBits  samples, const VkSampleMask*    pSampleMask)
{
   ANNOTATE(CmdSetSampleMaskEXT, commandBuffer, samples, pSampleMask);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdSetAlphaToCoverageEnableEXT(VkCommandBuffer commandBuffer, VkBool32 alphaToCoverageEnable)
{
   ANNOTATE(CmdSetAlphaToCoverageEnableEXT, commandBuffer, alphaToCoverageEnable);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdSetAlphaToOneEnableEXT(VkCommandBuffer commandBuffer, VkBool32 alphaToOneEnable)
{
   ANNOTATE(CmdSetAlphaToOneEnableEXT, commandBuffer, alphaToOneEnable);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdSetLogicOpEnableEXT(VkCommandBuffer commandBuffer, VkBool32 logicOpEnable)
{
   ANNOTATE(CmdSetLogicOpEnableEXT, commandBuffer, logicOpEnable);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdSetColorBlendEnableEXT(VkCommandBuffer commandBuffer, uint32_t firstAttachment, uint32_t attachmentCount, const VkBool32* pColorBlendEnables)
{
   ANNOTATE(CmdSetColorBlendEnableEXT, commandBuffer, firstAttachment, attachmentCount, pColorBlendEnables);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdSetColorBlendEquationEXT(VkCommandBuffer commandBuffer, uint32_t firstAttachment, uint32_t attachmentCount, const VkColorBlendEquationEXT* pColorBlendEquations)
{
   ANNOTATE(CmdSetColorBlendEquationEXT, commandBuffer, firstAttachment, attachmentCount, pColorBlendEquations);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdSetColorWriteMaskEXT(VkCommandBuffer commandBuffer, uint32_t firstAttachment, uint32_t attachmentCount, const VkColorComponentFlags* pColorWriteMasks)
{
   ANNOTATE(CmdSetColorWriteMaskEXT, commandBuffer, firstAttachment, attachmentCount, pColorWriteMasks);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdSetRasterizationStreamEXT(VkCommandBuffer commandBuffer, uint32_t rasterizationStream)
{
   ANNOTATE(CmdSetRasterizationStreamEXT, commandBuffer, rasterizationStream);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdSetConservativeRasterizationModeEXT(VkCommandBuffer commandBuffer, VkConservativeRasterizationModeEXT conservativeRasterizationMode)
{
   ANNOTATE(CmdSetConservativeRasterizationModeEXT, commandBuffer, conservativeRasterizationMode);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdSetExtraPrimitiveOverestimationSizeEXT(VkCommandBuffer commandBuffer, float extraPrimitiveOverestimationSize)
{
   ANNOTATE(CmdSetExtraPrimitiveOverestimationSizeEXT, commandBuffer, extraPrimitiveOverestimationSize);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdSetDepthClipEnableEXT(VkCommandBuffer commandBuffer, VkBool32 depthClipEnable)
{
   ANNOTATE(CmdSetDepthClipEnableEXT, commandBuffer, depthClipEnable);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdSetSampleLocationsEnableEXT(VkCommandBuffer commandBuffer, VkBool32 sampleLocationsEnable)
{
   ANNOTATE(CmdSetSampleLocationsEnableEXT, commandBuffer, sampleLocationsEnable);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdSetColorBlendAdvancedEXT(VkCommandBuffer commandBuffer, uint32_t firstAttachment, uint32_t attachmentCount, const VkColorBlendAdvancedEXT* pColorBlendAdvanced)
{
   ANNOTATE(CmdSetColorBlendAdvancedEXT, commandBuffer, firstAttachment, attachmentCount, pColorBlendAdvanced);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdSetProvokingVertexModeEXT(VkCommandBuffer commandBuffer, VkProvokingVertexModeEXT provokingVertexMode)
{
   ANNOTATE(CmdSetProvokingVertexModeEXT, commandBuffer, provokingVertexMode);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdSetLineRasterizationModeEXT(VkCommandBuffer commandBuffer, VkLineRasterizationModeEXT lineRasterizationMode)
{
   ANNOTATE(CmdSetLineRasterizationModeEXT, commandBuffer, lineRasterizationMode);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdSetLineStippleEnableEXT(VkCommandBuffer commandBuffer, VkBool32 stippledLineEnable)
{
   ANNOTATE(CmdSetLineStippleEnableEXT, commandBuffer, stippledLineEnable);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdSetDepthClipNegativeOneToOneEXT(VkCommandBuffer commandBuffer, VkBool32 negativeOneToOne)
{
   ANNOTATE(CmdSetDepthClipNegativeOneToOneEXT, commandBuffer, negativeOneToOne);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdSetViewportWScalingEnableNV(VkCommandBuffer commandBuffer, VkBool32 viewportWScalingEnable)
{
   ANNOTATE(CmdSetViewportWScalingEnableNV, commandBuffer, viewportWScalingEnable);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdSetViewportSwizzleNV(VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const VkViewportSwizzleNV* pViewportSwizzles)
{
   ANNOTATE(CmdSetViewportSwizzleNV, commandBuffer, firstViewport, viewportCount, pViewportSwizzles);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdSetCoverageToColorEnableNV(VkCommandBuffer commandBuffer, VkBool32 coverageToColorEnable)
{
   ANNOTATE(CmdSetCoverageToColorEnableNV, commandBuffer, coverageToColorEnable);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdSetCoverageToColorLocationNV(VkCommandBuffer commandBuffer, uint32_t coverageToColorLocation)
{
   ANNOTATE(CmdSetCoverageToColorLocationNV, commandBuffer, coverageToColorLocation);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdSetCoverageModulationModeNV(VkCommandBuffer commandBuffer, VkCoverageModulationModeNV coverageModulationMode)
{
   ANNOTATE(CmdSetCoverageModulationModeNV, commandBuffer, coverageModulationMode);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdSetCoverageModulationTableEnableNV(VkCommandBuffer commandBuffer, VkBool32 coverageModulationTableEnable)
{
   ANNOTATE(CmdSetCoverageModulationTableEnableNV, commandBuffer, coverageModulationTableEnable);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdSetCoverageModulationTableNV(VkCommandBuffer commandBuffer, uint32_t coverageModulationTableCount, const float* pCoverageModulationTable)
{
   ANNOTATE(CmdSetCoverageModulationTableNV, commandBuffer, coverageModulationTableCount, pCoverageModulationTable);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdSetShadingRateImageEnableNV(VkCommandBuffer commandBuffer, VkBool32 shadingRateImageEnable)
{
   ANNOTATE(CmdSetShadingRateImageEnableNV, commandBuffer, shadingRateImageEnable);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdSetCoverageReductionModeNV(VkCommandBuffer commandBuffer, VkCoverageReductionModeNV coverageReductionMode)
{
   ANNOTATE(CmdSetCoverageReductionModeNV, commandBuffer, coverageReductionMode);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdSetRepresentativeFragmentTestEnableNV(VkCommandBuffer commandBuffer, VkBool32 representativeFragmentTestEnable)
{
   ANNOTATE(CmdSetRepresentativeFragmentTestEnableNV, commandBuffer, representativeFragmentTestEnable);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdSetFragmentShadingRateKHR(VkCommandBuffer           commandBuffer, const VkExtent2D*                           pFragmentSize, const VkFragmentShadingRateCombinerOpKHR    combinerOps[2])
{
   ANNOTATE(CmdSetFragmentShadingRateKHR, commandBuffer, pFragmentSize, combinerOps);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdSetFragmentShadingRateEnumNV(VkCommandBuffer           commandBuffer, VkFragmentShadingRateNV                     shadingRate, const VkFragmentShadingRateCombinerOpKHR    combinerOps[2])
{
   ANNOTATE(CmdSetFragmentShadingRateEnumNV, commandBuffer, shadingRate, combinerOps);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdSetVertexInputEXT(VkCommandBuffer commandBuffer, uint32_t vertexBindingDescriptionCount, const VkVertexInputBindingDescription2EXT* pVertexBindingDescriptions, uint32_t vertexAttributeDescriptionCount, const VkVertexInputAttributeDescription2EXT* pVertexAttributeDescriptions)
{
   ANNOTATE(CmdSetVertexInputEXT, commandBuffer, vertexBindingDescriptionCount, pVertexBindingDescriptions, vertexAttributeDescriptionCount, pVertexAttributeDescriptions);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdSetColorWriteEnableEXT(VkCommandBuffer       commandBuffer, uint32_t                                attachmentCount, const VkBool32*   pColorWriteEnables)
{
   ANNOTATE(CmdSetColorWriteEnableEXT, commandBuffer, attachmentCount, pColorWriteEnables);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdWriteBufferMarker2AMD(VkCommandBuffer                   commandBuffer, VkPipelineStageFlags2               stage, VkBuffer                                            dstBuffer, VkDeviceSize                                        dstOffset, uint32_t                                            marker)
{
   ANNOTATE(CmdWriteBufferMarker2AMD, commandBuffer, stage, dstBuffer, dstOffset, marker);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdDecodeVideoKHR(VkCommandBuffer commandBuffer, const VkVideoDecodeInfoKHR* pDecodeInfo)
{
   ANNOTATE(CmdDecodeVideoKHR, commandBuffer, pDecodeInfo);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdBeginVideoCodingKHR(VkCommandBuffer commandBuffer, const VkVideoBeginCodingInfoKHR* pBeginInfo)
{
   ANNOTATE(CmdBeginVideoCodingKHR, commandBuffer, pBeginInfo);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdControlVideoCodingKHR(VkCommandBuffer commandBuffer, const VkVideoCodingControlInfoKHR* pCodingControlInfo)
{
   ANNOTATE(CmdControlVideoCodingKHR, commandBuffer, pCodingControlInfo);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdEndVideoCodingKHR(VkCommandBuffer commandBuffer, const VkVideoEndCodingInfoKHR* pEndCodingInfo)
{
   ANNOTATE(CmdEndVideoCodingKHR, commandBuffer, pEndCodingInfo);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdEncodeVideoKHR(VkCommandBuffer commandBuffer, const VkVideoEncodeInfoKHR* pEncodeInfo)
{
   ANNOTATE(CmdEncodeVideoKHR, commandBuffer, pEncodeInfo);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdDecompressMemoryNV(VkCommandBuffer commandBuffer, uint32_t decompressRegionCount, const VkDecompressMemoryRegionNV* pDecompressMemoryRegions)
{
   ANNOTATE(CmdDecompressMemoryNV, commandBuffer, decompressRegionCount, pDecompressMemoryRegions);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdDecompressMemoryIndirectCountNV(VkCommandBuffer commandBuffer, VkDeviceAddress indirectCommandsAddress, VkDeviceAddress indirectCommandsCountAddress, uint32_t stride)
{
   ANNOTATE(CmdDecompressMemoryIndirectCountNV, commandBuffer, indirectCommandsAddress, indirectCommandsCountAddress, stride);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdCuLaunchKernelNVX(VkCommandBuffer commandBuffer, const VkCuLaunchInfoNVX* pLaunchInfo)
{
   ANNOTATE(CmdCuLaunchKernelNVX, commandBuffer, pLaunchInfo);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdBindDescriptorBuffersEXT(VkCommandBuffer commandBuffer, uint32_t bufferCount, const VkDescriptorBufferBindingInfoEXT* pBindingInfos)
{
   ANNOTATE(CmdBindDescriptorBuffersEXT, commandBuffer, bufferCount, pBindingInfos);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdSetDescriptorBufferOffsetsEXT(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t firstSet, uint32_t setCount, const uint32_t* pBufferIndices, const VkDeviceSize* pOffsets)
{
   ANNOTATE(CmdSetDescriptorBufferOffsetsEXT, commandBuffer, pipelineBindPoint, layout, firstSet, setCount, pBufferIndices, pOffsets);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdBindDescriptorBufferEmbeddedSamplersEXT(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t set)
{
   ANNOTATE(CmdBindDescriptorBufferEmbeddedSamplersEXT, commandBuffer, pipelineBindPoint, layout, set);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdBeginRendering(VkCommandBuffer                   commandBuffer, const VkRenderingInfo*                              pRenderingInfo)
{
   ANNOTATE(CmdBeginRendering, commandBuffer, pRenderingInfo);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdEndRendering(VkCommandBuffer                   commandBuffer)
{
   ANNOTATE(CmdEndRendering, commandBuffer);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdBuildMicromapsEXT(VkCommandBuffer                                    commandBuffer, uint32_t infoCount, const VkMicromapBuildInfoEXT* pInfos)
{
   ANNOTATE(CmdBuildMicromapsEXT, commandBuffer, infoCount, pInfos);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdCopyMicromapEXT(VkCommandBuffer commandBuffer, const VkCopyMicromapInfoEXT* pInfo)
{
   ANNOTATE(CmdCopyMicromapEXT, commandBuffer, pInfo);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdCopyMicromapToMemoryEXT(VkCommandBuffer commandBuffer, const VkCopyMicromapToMemoryInfoEXT* pInfo)
{
   ANNOTATE(CmdCopyMicromapToMemoryEXT, commandBuffer, pInfo);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdCopyMemoryToMicromapEXT(VkCommandBuffer commandBuffer, const VkCopyMemoryToMicromapInfoEXT* pInfo)
{
   ANNOTATE(CmdCopyMemoryToMicromapEXT, commandBuffer, pInfo);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdWriteMicromapsPropertiesEXT(VkCommandBuffer commandBuffer, uint32_t micromapCount, const VkMicromapEXT* pMicromaps, VkQueryType queryType, VkQueryPool queryPool, uint32_t firstQuery)
{
   ANNOTATE(CmdWriteMicromapsPropertiesEXT, commandBuffer, micromapCount, pMicromaps, queryType, queryPool, firstQuery);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdOpticalFlowExecuteNV(VkCommandBuffer commandBuffer, VkOpticalFlowSessionNV session, const VkOpticalFlowExecuteInfoNV* pExecuteInfo)
{
   ANNOTATE(CmdOpticalFlowExecuteNV, commandBuffer, session, pExecuteInfo);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdBindShadersEXT(VkCommandBuffer commandBuffer, uint32_t stageCount, const VkShaderStageFlagBits* pStages, const VkShaderEXT* pShaders)
{
   ANNOTATE(CmdBindShadersEXT, commandBuffer, stageCount, pStages, pShaders);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdSetRenderingAttachmentLocations(VkCommandBuffer commandBuffer, const VkRenderingAttachmentLocationInfo* pLocationInfo)
{
   ANNOTATE(CmdSetRenderingAttachmentLocations, commandBuffer, pLocationInfo);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdSetRenderingInputAttachmentIndices(VkCommandBuffer commandBuffer, const VkRenderingInputAttachmentIndexInfo* pInputAttachmentIndexInfo)
{
   ANNOTATE(CmdSetRenderingInputAttachmentIndices, commandBuffer, pInputAttachmentIndexInfo);
}

VKAPI_ATTR void VKAPI_CALL
annotate_CmdSetDepthClampRangeEXT(VkCommandBuffer commandBuffer, VkDepthClampModeEXT depthClampMode, const VkDepthClampRangeEXT* pDepthClampRange)
{
   ANNOTATE(CmdSetDepthClampRangeEXT, commandBuffer, depthClampMode, pDepthClampRange);
}

