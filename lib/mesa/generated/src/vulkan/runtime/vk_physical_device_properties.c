
/* Copyright © 2021 Intel Corporation
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

/* This file generated from vk_physical_device_properties_gen.py, don"t edit directly. */

#include "vk_common_entrypoints.h"
#include "vk_log.h"
#include "vk_physical_device.h"
#include "vk_physical_device_properties.h"
#include "vk_util.h"

VKAPI_ATTR void VKAPI_CALL
vk_common_GetPhysicalDeviceProperties2(VkPhysicalDevice physicalDevice,
                                       VkPhysicalDeviceProperties2 *pProperties)
{
   VK_FROM_HANDLE(vk_physical_device, pdevice, physicalDevice);

   pProperties->properties.apiVersion = pdevice->properties.apiVersion;
   pProperties->properties.driverVersion = pdevice->properties.driverVersion;
   pProperties->properties.vendorID = pdevice->properties.vendorID;
   pProperties->properties.deviceID = pdevice->properties.deviceID;
   pProperties->properties.deviceType = pdevice->properties.deviceType;
   memcpy(pProperties->properties.deviceName, pdevice->properties.deviceName, sizeof(pProperties->properties.deviceName));
   memcpy(pProperties->properties.pipelineCacheUUID, pdevice->properties.pipelineCacheUUID, sizeof(pProperties->properties.pipelineCacheUUID));
   pProperties->properties.limits.maxImageDimension1D = pdevice->properties.maxImageDimension1D;
   pProperties->properties.limits.maxImageDimension2D = pdevice->properties.maxImageDimension2D;
   pProperties->properties.limits.maxImageDimension3D = pdevice->properties.maxImageDimension3D;
   pProperties->properties.limits.maxImageDimensionCube = pdevice->properties.maxImageDimensionCube;
   pProperties->properties.limits.maxImageArrayLayers = pdevice->properties.maxImageArrayLayers;
   pProperties->properties.limits.maxTexelBufferElements = pdevice->properties.maxTexelBufferElements;
   pProperties->properties.limits.maxUniformBufferRange = pdevice->properties.maxUniformBufferRange;
   pProperties->properties.limits.maxStorageBufferRange = pdevice->properties.maxStorageBufferRange;
   pProperties->properties.limits.maxPushConstantsSize = pdevice->properties.maxPushConstantsSize;
   pProperties->properties.limits.maxMemoryAllocationCount = pdevice->properties.maxMemoryAllocationCount;
   pProperties->properties.limits.maxSamplerAllocationCount = pdevice->properties.maxSamplerAllocationCount;
   pProperties->properties.limits.bufferImageGranularity = pdevice->properties.bufferImageGranularity;
   pProperties->properties.limits.sparseAddressSpaceSize = pdevice->properties.sparseAddressSpaceSize;
   pProperties->properties.limits.maxBoundDescriptorSets = pdevice->properties.maxBoundDescriptorSets;
   pProperties->properties.limits.maxPerStageDescriptorSamplers = pdevice->properties.maxPerStageDescriptorSamplers;
   pProperties->properties.limits.maxPerStageDescriptorUniformBuffers = pdevice->properties.maxPerStageDescriptorUniformBuffers;
   pProperties->properties.limits.maxPerStageDescriptorStorageBuffers = pdevice->properties.maxPerStageDescriptorStorageBuffers;
   pProperties->properties.limits.maxPerStageDescriptorSampledImages = pdevice->properties.maxPerStageDescriptorSampledImages;
   pProperties->properties.limits.maxPerStageDescriptorStorageImages = pdevice->properties.maxPerStageDescriptorStorageImages;
   pProperties->properties.limits.maxPerStageDescriptorInputAttachments = pdevice->properties.maxPerStageDescriptorInputAttachments;
   pProperties->properties.limits.maxPerStageResources = pdevice->properties.maxPerStageResources;
   pProperties->properties.limits.maxDescriptorSetSamplers = pdevice->properties.maxDescriptorSetSamplers;
   pProperties->properties.limits.maxDescriptorSetUniformBuffers = pdevice->properties.maxDescriptorSetUniformBuffers;
   pProperties->properties.limits.maxDescriptorSetUniformBuffersDynamic = pdevice->properties.maxDescriptorSetUniformBuffersDynamic;
   pProperties->properties.limits.maxDescriptorSetStorageBuffers = pdevice->properties.maxDescriptorSetStorageBuffers;
   pProperties->properties.limits.maxDescriptorSetStorageBuffersDynamic = pdevice->properties.maxDescriptorSetStorageBuffersDynamic;
   pProperties->properties.limits.maxDescriptorSetSampledImages = pdevice->properties.maxDescriptorSetSampledImages;
   pProperties->properties.limits.maxDescriptorSetStorageImages = pdevice->properties.maxDescriptorSetStorageImages;
   pProperties->properties.limits.maxDescriptorSetInputAttachments = pdevice->properties.maxDescriptorSetInputAttachments;
   pProperties->properties.limits.maxVertexInputAttributes = pdevice->properties.maxVertexInputAttributes;
   pProperties->properties.limits.maxVertexInputBindings = pdevice->properties.maxVertexInputBindings;
   pProperties->properties.limits.maxVertexInputAttributeOffset = pdevice->properties.maxVertexInputAttributeOffset;
   pProperties->properties.limits.maxVertexInputBindingStride = pdevice->properties.maxVertexInputBindingStride;
   pProperties->properties.limits.maxVertexOutputComponents = pdevice->properties.maxVertexOutputComponents;
   pProperties->properties.limits.maxTessellationGenerationLevel = pdevice->properties.maxTessellationGenerationLevel;
   pProperties->properties.limits.maxTessellationPatchSize = pdevice->properties.maxTessellationPatchSize;
   pProperties->properties.limits.maxTessellationControlPerVertexInputComponents = pdevice->properties.maxTessellationControlPerVertexInputComponents;
   pProperties->properties.limits.maxTessellationControlPerVertexOutputComponents = pdevice->properties.maxTessellationControlPerVertexOutputComponents;
   pProperties->properties.limits.maxTessellationControlPerPatchOutputComponents = pdevice->properties.maxTessellationControlPerPatchOutputComponents;
   pProperties->properties.limits.maxTessellationControlTotalOutputComponents = pdevice->properties.maxTessellationControlTotalOutputComponents;
   pProperties->properties.limits.maxTessellationEvaluationInputComponents = pdevice->properties.maxTessellationEvaluationInputComponents;
   pProperties->properties.limits.maxTessellationEvaluationOutputComponents = pdevice->properties.maxTessellationEvaluationOutputComponents;
   pProperties->properties.limits.maxGeometryShaderInvocations = pdevice->properties.maxGeometryShaderInvocations;
   pProperties->properties.limits.maxGeometryInputComponents = pdevice->properties.maxGeometryInputComponents;
   pProperties->properties.limits.maxGeometryOutputComponents = pdevice->properties.maxGeometryOutputComponents;
   pProperties->properties.limits.maxGeometryOutputVertices = pdevice->properties.maxGeometryOutputVertices;
   pProperties->properties.limits.maxGeometryTotalOutputComponents = pdevice->properties.maxGeometryTotalOutputComponents;
   pProperties->properties.limits.maxFragmentInputComponents = pdevice->properties.maxFragmentInputComponents;
   pProperties->properties.limits.maxFragmentOutputAttachments = pdevice->properties.maxFragmentOutputAttachments;
   pProperties->properties.limits.maxFragmentDualSrcAttachments = pdevice->properties.maxFragmentDualSrcAttachments;
   pProperties->properties.limits.maxFragmentCombinedOutputResources = pdevice->properties.maxFragmentCombinedOutputResources;
   pProperties->properties.limits.maxComputeSharedMemorySize = pdevice->properties.maxComputeSharedMemorySize;
   memcpy(pProperties->properties.limits.maxComputeWorkGroupCount, pdevice->properties.maxComputeWorkGroupCount, sizeof(pProperties->properties.limits.maxComputeWorkGroupCount));
   pProperties->properties.limits.maxComputeWorkGroupInvocations = pdevice->properties.maxComputeWorkGroupInvocations;
   memcpy(pProperties->properties.limits.maxComputeWorkGroupSize, pdevice->properties.maxComputeWorkGroupSize, sizeof(pProperties->properties.limits.maxComputeWorkGroupSize));
   pProperties->properties.limits.subPixelPrecisionBits = pdevice->properties.subPixelPrecisionBits;
   pProperties->properties.limits.subTexelPrecisionBits = pdevice->properties.subTexelPrecisionBits;
   pProperties->properties.limits.mipmapPrecisionBits = pdevice->properties.mipmapPrecisionBits;
   pProperties->properties.limits.maxDrawIndexedIndexValue = pdevice->properties.maxDrawIndexedIndexValue;
   pProperties->properties.limits.maxDrawIndirectCount = pdevice->properties.maxDrawIndirectCount;
   pProperties->properties.limits.maxSamplerLodBias = pdevice->properties.maxSamplerLodBias;
   pProperties->properties.limits.maxSamplerAnisotropy = pdevice->properties.maxSamplerAnisotropy;
   pProperties->properties.limits.maxViewports = pdevice->properties.maxViewports;
   memcpy(pProperties->properties.limits.maxViewportDimensions, pdevice->properties.maxViewportDimensions, sizeof(pProperties->properties.limits.maxViewportDimensions));
   memcpy(pProperties->properties.limits.viewportBoundsRange, pdevice->properties.viewportBoundsRange, sizeof(pProperties->properties.limits.viewportBoundsRange));
   pProperties->properties.limits.viewportSubPixelBits = pdevice->properties.viewportSubPixelBits;
   pProperties->properties.limits.minMemoryMapAlignment = pdevice->properties.minMemoryMapAlignment;
   pProperties->properties.limits.minTexelBufferOffsetAlignment = pdevice->properties.minTexelBufferOffsetAlignment;
   pProperties->properties.limits.minUniformBufferOffsetAlignment = pdevice->properties.minUniformBufferOffsetAlignment;
   pProperties->properties.limits.minStorageBufferOffsetAlignment = pdevice->properties.minStorageBufferOffsetAlignment;
   pProperties->properties.limits.minTexelOffset = pdevice->properties.minTexelOffset;
   pProperties->properties.limits.maxTexelOffset = pdevice->properties.maxTexelOffset;
   pProperties->properties.limits.minTexelGatherOffset = pdevice->properties.minTexelGatherOffset;
   pProperties->properties.limits.maxTexelGatherOffset = pdevice->properties.maxTexelGatherOffset;
   pProperties->properties.limits.minInterpolationOffset = pdevice->properties.minInterpolationOffset;
   pProperties->properties.limits.maxInterpolationOffset = pdevice->properties.maxInterpolationOffset;
   pProperties->properties.limits.subPixelInterpolationOffsetBits = pdevice->properties.subPixelInterpolationOffsetBits;
   pProperties->properties.limits.maxFramebufferWidth = pdevice->properties.maxFramebufferWidth;
   pProperties->properties.limits.maxFramebufferHeight = pdevice->properties.maxFramebufferHeight;
   pProperties->properties.limits.maxFramebufferLayers = pdevice->properties.maxFramebufferLayers;
   pProperties->properties.limits.framebufferColorSampleCounts = pdevice->properties.framebufferColorSampleCounts;
   pProperties->properties.limits.framebufferDepthSampleCounts = pdevice->properties.framebufferDepthSampleCounts;
   pProperties->properties.limits.framebufferStencilSampleCounts = pdevice->properties.framebufferStencilSampleCounts;
   pProperties->properties.limits.framebufferNoAttachmentsSampleCounts = pdevice->properties.framebufferNoAttachmentsSampleCounts;
   pProperties->properties.limits.maxColorAttachments = pdevice->properties.maxColorAttachments;
   pProperties->properties.limits.sampledImageColorSampleCounts = pdevice->properties.sampledImageColorSampleCounts;
   pProperties->properties.limits.sampledImageIntegerSampleCounts = pdevice->properties.sampledImageIntegerSampleCounts;
   pProperties->properties.limits.sampledImageDepthSampleCounts = pdevice->properties.sampledImageDepthSampleCounts;
   pProperties->properties.limits.sampledImageStencilSampleCounts = pdevice->properties.sampledImageStencilSampleCounts;
   pProperties->properties.limits.storageImageSampleCounts = pdevice->properties.storageImageSampleCounts;
   pProperties->properties.limits.maxSampleMaskWords = pdevice->properties.maxSampleMaskWords;
   pProperties->properties.limits.timestampComputeAndGraphics = pdevice->properties.timestampComputeAndGraphics;
   pProperties->properties.limits.timestampPeriod = pdevice->properties.timestampPeriod;
   pProperties->properties.limits.maxClipDistances = pdevice->properties.maxClipDistances;
   pProperties->properties.limits.maxCullDistances = pdevice->properties.maxCullDistances;
   pProperties->properties.limits.maxCombinedClipAndCullDistances = pdevice->properties.maxCombinedClipAndCullDistances;
   pProperties->properties.limits.discreteQueuePriorities = pdevice->properties.discreteQueuePriorities;
   memcpy(pProperties->properties.limits.pointSizeRange, pdevice->properties.pointSizeRange, sizeof(pProperties->properties.limits.pointSizeRange));
   memcpy(pProperties->properties.limits.lineWidthRange, pdevice->properties.lineWidthRange, sizeof(pProperties->properties.limits.lineWidthRange));
   pProperties->properties.limits.pointSizeGranularity = pdevice->properties.pointSizeGranularity;
   pProperties->properties.limits.lineWidthGranularity = pdevice->properties.lineWidthGranularity;
   pProperties->properties.limits.strictLines = pdevice->properties.strictLines;
   pProperties->properties.limits.standardSampleLocations = pdevice->properties.standardSampleLocations;
   pProperties->properties.limits.optimalBufferCopyOffsetAlignment = pdevice->properties.optimalBufferCopyOffsetAlignment;
   pProperties->properties.limits.optimalBufferCopyRowPitchAlignment = pdevice->properties.optimalBufferCopyRowPitchAlignment;
   pProperties->properties.limits.nonCoherentAtomSize = pdevice->properties.nonCoherentAtomSize;
   pProperties->properties.sparseProperties.residencyStandard2DBlockShape = pdevice->properties.sparseResidencyStandard2DBlockShape;
   pProperties->properties.sparseProperties.residencyStandard2DMultisampleBlockShape = pdevice->properties.sparseResidencyStandard2DMultisampleBlockShape;
   pProperties->properties.sparseProperties.residencyStandard3DBlockShape = pdevice->properties.sparseResidencyStandard3DBlockShape;
   pProperties->properties.sparseProperties.residencyAlignedMipSize = pdevice->properties.sparseResidencyAlignedMipSize;
   pProperties->properties.sparseProperties.residencyNonResidentStrict = pdevice->properties.sparseResidencyNonResidentStrict;

   vk_foreach_struct(ext, pProperties) {
      switch (ext->sType) {
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DEVICE_GENERATED_COMMANDS_PROPERTIES_NV: {
         VkPhysicalDeviceDeviceGeneratedCommandsPropertiesNV *properties = (void *)ext;
         properties->maxGraphicsShaderGroupCount = pdevice->properties.maxGraphicsShaderGroupCount;
         properties->maxIndirectSequenceCount = pdevice->properties.maxIndirectSequenceCount;
         properties->maxIndirectCommandsTokenCount = pdevice->properties.maxIndirectCommandsTokenCount;
         properties->maxIndirectCommandsStreamCount = pdevice->properties.maxIndirectCommandsStreamCount;
         properties->maxIndirectCommandsTokenOffset = pdevice->properties.maxIndirectCommandsTokenOffset;
         properties->maxIndirectCommandsStreamStride = pdevice->properties.maxIndirectCommandsStreamStride;
         properties->minSequencesCountBufferOffsetAlignment = pdevice->properties.minSequencesCountBufferOffsetAlignment;
         properties->minSequencesIndexBufferOffsetAlignment = pdevice->properties.minSequencesIndexBufferOffsetAlignment;
         properties->minIndirectCommandsBufferOffsetAlignment = pdevice->properties.minIndirectCommandsBufferOffsetAlignment;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MULTI_DRAW_PROPERTIES_EXT: {
         VkPhysicalDeviceMultiDrawPropertiesEXT *properties = (void *)ext;
         properties->maxMultiDrawCount = pdevice->properties.maxMultiDrawCount;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PUSH_DESCRIPTOR_PROPERTIES_KHR: {
         VkPhysicalDevicePushDescriptorPropertiesKHR *properties = (void *)ext;
         properties->maxPushDescriptors = pdevice->properties.maxPushDescriptors;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DRIVER_PROPERTIES: {
         VkPhysicalDeviceDriverProperties *properties = (void *)ext;
         properties->driverID = pdevice->properties.driverID;
         memcpy(properties->driverName, pdevice->properties.driverName, sizeof(properties->driverName));
         memcpy(properties->driverInfo, pdevice->properties.driverInfo, sizeof(properties->driverInfo));
         properties->conformanceVersion = pdevice->properties.conformanceVersion;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ID_PROPERTIES: {
         VkPhysicalDeviceIDProperties *properties = (void *)ext;
         memcpy(properties->deviceUUID, pdevice->properties.deviceUUID, sizeof(properties->deviceUUID));
         memcpy(properties->driverUUID, pdevice->properties.driverUUID, sizeof(properties->driverUUID));
         memcpy(properties->deviceLUID, pdevice->properties.deviceLUID, sizeof(properties->deviceLUID));
         properties->deviceNodeMask = pdevice->properties.deviceNodeMask;
         properties->deviceLUIDValid = pdevice->properties.deviceLUIDValid;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MULTIVIEW_PROPERTIES: {
         VkPhysicalDeviceMultiviewProperties *properties = (void *)ext;
         properties->maxMultiviewViewCount = pdevice->properties.maxMultiviewViewCount;
         properties->maxMultiviewInstanceIndex = pdevice->properties.maxMultiviewInstanceIndex;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DISCARD_RECTANGLE_PROPERTIES_EXT: {
         VkPhysicalDeviceDiscardRectanglePropertiesEXT *properties = (void *)ext;
         properties->maxDiscardRectangles = pdevice->properties.maxDiscardRectangles;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MULTIVIEW_PER_VIEW_ATTRIBUTES_PROPERTIES_NVX: {
         VkPhysicalDeviceMultiviewPerViewAttributesPropertiesNVX *properties = (void *)ext;
         properties->perViewPositionAllComponents = pdevice->properties.perViewPositionAllComponents;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SUBGROUP_PROPERTIES: {
         VkPhysicalDeviceSubgroupProperties *properties = (void *)ext;
         properties->subgroupSize = pdevice->properties.subgroupSize;
         properties->supportedStages = pdevice->properties.subgroupSupportedStages;
         properties->supportedOperations = pdevice->properties.subgroupSupportedOperations;
         properties->quadOperationsInAllStages = pdevice->properties.subgroupQuadOperationsInAllStages;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_POINT_CLIPPING_PROPERTIES: {
         VkPhysicalDevicePointClippingProperties *properties = (void *)ext;
         properties->pointClippingBehavior = pdevice->properties.pointClippingBehavior;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROTECTED_MEMORY_PROPERTIES: {
         VkPhysicalDeviceProtectedMemoryProperties *properties = (void *)ext;
         properties->protectedNoFault = pdevice->properties.protectedNoFault;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SAMPLER_FILTER_MINMAX_PROPERTIES: {
         VkPhysicalDeviceSamplerFilterMinmaxProperties *properties = (void *)ext;
         properties->filterMinmaxSingleComponentFormats = pdevice->properties.filterMinmaxSingleComponentFormats;
         properties->filterMinmaxImageComponentMapping = pdevice->properties.filterMinmaxImageComponentMapping;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SAMPLE_LOCATIONS_PROPERTIES_EXT: {
         VkPhysicalDeviceSampleLocationsPropertiesEXT *properties = (void *)ext;
         properties->sampleLocationSampleCounts = pdevice->properties.sampleLocationSampleCounts;
         properties->maxSampleLocationGridSize = pdevice->properties.maxSampleLocationGridSize;
         memcpy(properties->sampleLocationCoordinateRange, pdevice->properties.sampleLocationCoordinateRange, sizeof(properties->sampleLocationCoordinateRange));
         properties->sampleLocationSubPixelBits = pdevice->properties.sampleLocationSubPixelBits;
         properties->variableSampleLocations = pdevice->properties.variableSampleLocations;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_BLEND_OPERATION_ADVANCED_PROPERTIES_EXT: {
         VkPhysicalDeviceBlendOperationAdvancedPropertiesEXT *properties = (void *)ext;
         properties->advancedBlendMaxColorAttachments = pdevice->properties.advancedBlendMaxColorAttachments;
         properties->advancedBlendIndependentBlend = pdevice->properties.advancedBlendIndependentBlend;
         properties->advancedBlendNonPremultipliedSrcColor = pdevice->properties.advancedBlendNonPremultipliedSrcColor;
         properties->advancedBlendNonPremultipliedDstColor = pdevice->properties.advancedBlendNonPremultipliedDstColor;
         properties->advancedBlendCorrelatedOverlap = pdevice->properties.advancedBlendCorrelatedOverlap;
         properties->advancedBlendAllOperations = pdevice->properties.advancedBlendAllOperations;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_INLINE_UNIFORM_BLOCK_PROPERTIES: {
         VkPhysicalDeviceInlineUniformBlockProperties *properties = (void *)ext;
         properties->maxInlineUniformBlockSize = pdevice->properties.maxInlineUniformBlockSize;
         properties->maxPerStageDescriptorInlineUniformBlocks = pdevice->properties.maxPerStageDescriptorInlineUniformBlocks;
         properties->maxPerStageDescriptorUpdateAfterBindInlineUniformBlocks = pdevice->properties.maxPerStageDescriptorUpdateAfterBindInlineUniformBlocks;
         properties->maxDescriptorSetInlineUniformBlocks = pdevice->properties.maxDescriptorSetInlineUniformBlocks;
         properties->maxDescriptorSetUpdateAfterBindInlineUniformBlocks = pdevice->properties.maxDescriptorSetUpdateAfterBindInlineUniformBlocks;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MAINTENANCE_3_PROPERTIES: {
         VkPhysicalDeviceMaintenance3Properties *properties = (void *)ext;
         properties->maxPerSetDescriptors = pdevice->properties.maxPerSetDescriptors;
         properties->maxMemoryAllocationSize = pdevice->properties.maxMemoryAllocationSize;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MAINTENANCE_4_PROPERTIES: {
         VkPhysicalDeviceMaintenance4Properties *properties = (void *)ext;
         properties->maxBufferSize = pdevice->properties.maxBufferSize;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MAINTENANCE_5_PROPERTIES_KHR: {
         VkPhysicalDeviceMaintenance5PropertiesKHR *properties = (void *)ext;
         properties->earlyFragmentMultisampleCoverageAfterSampleCounting = pdevice->properties.earlyFragmentMultisampleCoverageAfterSampleCounting;
         properties->earlyFragmentSampleMaskTestBeforeSampleCounting = pdevice->properties.earlyFragmentSampleMaskTestBeforeSampleCounting;
         properties->depthStencilSwizzleOneSupport = pdevice->properties.depthStencilSwizzleOneSupport;
         properties->polygonModePointSize = pdevice->properties.polygonModePointSize;
         properties->nonStrictSinglePixelWideLinesUseParallelogram = pdevice->properties.nonStrictSinglePixelWideLinesUseParallelogram;
         properties->nonStrictWideLinesUseParallelogram = pdevice->properties.nonStrictWideLinesUseParallelogram;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FLOAT_CONTROLS_PROPERTIES: {
         VkPhysicalDeviceFloatControlsProperties *properties = (void *)ext;
         properties->denormBehaviorIndependence = pdevice->properties.denormBehaviorIndependence;
         properties->roundingModeIndependence = pdevice->properties.roundingModeIndependence;
         properties->shaderSignedZeroInfNanPreserveFloat16 = pdevice->properties.shaderSignedZeroInfNanPreserveFloat16;
         properties->shaderSignedZeroInfNanPreserveFloat32 = pdevice->properties.shaderSignedZeroInfNanPreserveFloat32;
         properties->shaderSignedZeroInfNanPreserveFloat64 = pdevice->properties.shaderSignedZeroInfNanPreserveFloat64;
         properties->shaderDenormPreserveFloat16 = pdevice->properties.shaderDenormPreserveFloat16;
         properties->shaderDenormPreserveFloat32 = pdevice->properties.shaderDenormPreserveFloat32;
         properties->shaderDenormPreserveFloat64 = pdevice->properties.shaderDenormPreserveFloat64;
         properties->shaderDenormFlushToZeroFloat16 = pdevice->properties.shaderDenormFlushToZeroFloat16;
         properties->shaderDenormFlushToZeroFloat32 = pdevice->properties.shaderDenormFlushToZeroFloat32;
         properties->shaderDenormFlushToZeroFloat64 = pdevice->properties.shaderDenormFlushToZeroFloat64;
         properties->shaderRoundingModeRTEFloat16 = pdevice->properties.shaderRoundingModeRTEFloat16;
         properties->shaderRoundingModeRTEFloat32 = pdevice->properties.shaderRoundingModeRTEFloat32;
         properties->shaderRoundingModeRTEFloat64 = pdevice->properties.shaderRoundingModeRTEFloat64;
         properties->shaderRoundingModeRTZFloat16 = pdevice->properties.shaderRoundingModeRTZFloat16;
         properties->shaderRoundingModeRTZFloat32 = pdevice->properties.shaderRoundingModeRTZFloat32;
         properties->shaderRoundingModeRTZFloat64 = pdevice->properties.shaderRoundingModeRTZFloat64;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTERNAL_MEMORY_HOST_PROPERTIES_EXT: {
         VkPhysicalDeviceExternalMemoryHostPropertiesEXT *properties = (void *)ext;
         properties->minImportedHostPointerAlignment = pdevice->properties.minImportedHostPointerAlignment;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_CONSERVATIVE_RASTERIZATION_PROPERTIES_EXT: {
         VkPhysicalDeviceConservativeRasterizationPropertiesEXT *properties = (void *)ext;
         properties->primitiveOverestimationSize = pdevice->properties.primitiveOverestimationSize;
         properties->maxExtraPrimitiveOverestimationSize = pdevice->properties.maxExtraPrimitiveOverestimationSize;
         properties->extraPrimitiveOverestimationSizeGranularity = pdevice->properties.extraPrimitiveOverestimationSizeGranularity;
         properties->primitiveUnderestimation = pdevice->properties.primitiveUnderestimation;
         properties->conservativePointAndLineRasterization = pdevice->properties.conservativePointAndLineRasterization;
         properties->degenerateTrianglesRasterized = pdevice->properties.degenerateTrianglesRasterized;
         properties->degenerateLinesRasterized = pdevice->properties.degenerateLinesRasterized;
         properties->fullyCoveredFragmentShaderInputVariable = pdevice->properties.fullyCoveredFragmentShaderInputVariable;
         properties->conservativeRasterizationPostDepthCoverage = pdevice->properties.conservativeRasterizationPostDepthCoverage;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_CORE_PROPERTIES_AMD: {
         VkPhysicalDeviceShaderCorePropertiesAMD *properties = (void *)ext;
         properties->shaderEngineCount = pdevice->properties.shaderEngineCount;
         properties->shaderArraysPerEngineCount = pdevice->properties.shaderArraysPerEngineCount;
         properties->computeUnitsPerShaderArray = pdevice->properties.computeUnitsPerShaderArray;
         properties->simdPerComputeUnit = pdevice->properties.simdPerComputeUnit;
         properties->wavefrontsPerSimd = pdevice->properties.wavefrontsPerSimd;
         properties->wavefrontSize = pdevice->properties.wavefrontSize;
         properties->sgprsPerSimd = pdevice->properties.sgprsPerSimd;
         properties->minSgprAllocation = pdevice->properties.minSgprAllocation;
         properties->maxSgprAllocation = pdevice->properties.maxSgprAllocation;
         properties->sgprAllocationGranularity = pdevice->properties.sgprAllocationGranularity;
         properties->vgprsPerSimd = pdevice->properties.vgprsPerSimd;
         properties->minVgprAllocation = pdevice->properties.minVgprAllocation;
         properties->maxVgprAllocation = pdevice->properties.maxVgprAllocation;
         properties->vgprAllocationGranularity = pdevice->properties.vgprAllocationGranularity;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_CORE_PROPERTIES_2_AMD: {
         VkPhysicalDeviceShaderCoreProperties2AMD *properties = (void *)ext;
         properties->shaderCoreFeatures = pdevice->properties.shaderCoreFeatures;
         properties->activeComputeUnitCount = pdevice->properties.activeComputeUnitCount;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_INDEXING_PROPERTIES: {
         VkPhysicalDeviceDescriptorIndexingProperties *properties = (void *)ext;
         properties->maxUpdateAfterBindDescriptorsInAllPools = pdevice->properties.maxUpdateAfterBindDescriptorsInAllPools;
         properties->shaderUniformBufferArrayNonUniformIndexingNative = pdevice->properties.shaderUniformBufferArrayNonUniformIndexingNative;
         properties->shaderSampledImageArrayNonUniformIndexingNative = pdevice->properties.shaderSampledImageArrayNonUniformIndexingNative;
         properties->shaderStorageBufferArrayNonUniformIndexingNative = pdevice->properties.shaderStorageBufferArrayNonUniformIndexingNative;
         properties->shaderStorageImageArrayNonUniformIndexingNative = pdevice->properties.shaderStorageImageArrayNonUniformIndexingNative;
         properties->shaderInputAttachmentArrayNonUniformIndexingNative = pdevice->properties.shaderInputAttachmentArrayNonUniformIndexingNative;
         properties->robustBufferAccessUpdateAfterBind = pdevice->properties.robustBufferAccessUpdateAfterBind;
         properties->quadDivergentImplicitLod = pdevice->properties.quadDivergentImplicitLod;
         properties->maxPerStageDescriptorUpdateAfterBindSamplers = pdevice->properties.maxPerStageDescriptorUpdateAfterBindSamplers;
         properties->maxPerStageDescriptorUpdateAfterBindUniformBuffers = pdevice->properties.maxPerStageDescriptorUpdateAfterBindUniformBuffers;
         properties->maxPerStageDescriptorUpdateAfterBindStorageBuffers = pdevice->properties.maxPerStageDescriptorUpdateAfterBindStorageBuffers;
         properties->maxPerStageDescriptorUpdateAfterBindSampledImages = pdevice->properties.maxPerStageDescriptorUpdateAfterBindSampledImages;
         properties->maxPerStageDescriptorUpdateAfterBindStorageImages = pdevice->properties.maxPerStageDescriptorUpdateAfterBindStorageImages;
         properties->maxPerStageDescriptorUpdateAfterBindInputAttachments = pdevice->properties.maxPerStageDescriptorUpdateAfterBindInputAttachments;
         properties->maxPerStageUpdateAfterBindResources = pdevice->properties.maxPerStageUpdateAfterBindResources;
         properties->maxDescriptorSetUpdateAfterBindSamplers = pdevice->properties.maxDescriptorSetUpdateAfterBindSamplers;
         properties->maxDescriptorSetUpdateAfterBindUniformBuffers = pdevice->properties.maxDescriptorSetUpdateAfterBindUniformBuffers;
         properties->maxDescriptorSetUpdateAfterBindUniformBuffersDynamic = pdevice->properties.maxDescriptorSetUpdateAfterBindUniformBuffersDynamic;
         properties->maxDescriptorSetUpdateAfterBindStorageBuffers = pdevice->properties.maxDescriptorSetUpdateAfterBindStorageBuffers;
         properties->maxDescriptorSetUpdateAfterBindStorageBuffersDynamic = pdevice->properties.maxDescriptorSetUpdateAfterBindStorageBuffersDynamic;
         properties->maxDescriptorSetUpdateAfterBindSampledImages = pdevice->properties.maxDescriptorSetUpdateAfterBindSampledImages;
         properties->maxDescriptorSetUpdateAfterBindStorageImages = pdevice->properties.maxDescriptorSetUpdateAfterBindStorageImages;
         properties->maxDescriptorSetUpdateAfterBindInputAttachments = pdevice->properties.maxDescriptorSetUpdateAfterBindInputAttachments;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TIMELINE_SEMAPHORE_PROPERTIES: {
         VkPhysicalDeviceTimelineSemaphoreProperties *properties = (void *)ext;
         properties->maxTimelineSemaphoreValueDifference = pdevice->properties.maxTimelineSemaphoreValueDifference;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VERTEX_ATTRIBUTE_DIVISOR_PROPERTIES_EXT: {
         VkPhysicalDeviceVertexAttributeDivisorPropertiesEXT *properties = (void *)ext;
         properties->maxVertexAttribDivisor = pdevice->properties.maxVertexAttribDivisor;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PCI_BUS_INFO_PROPERTIES_EXT: {
         VkPhysicalDevicePCIBusInfoPropertiesEXT *properties = (void *)ext;
         properties->pciDomain = pdevice->properties.pciDomain;
         properties->pciBus = pdevice->properties.pciBus;
         properties->pciDevice = pdevice->properties.pciDevice;
         properties->pciFunction = pdevice->properties.pciFunction;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DEPTH_STENCIL_RESOLVE_PROPERTIES: {
         VkPhysicalDeviceDepthStencilResolveProperties *properties = (void *)ext;
         properties->supportedDepthResolveModes = pdevice->properties.supportedDepthResolveModes;
         properties->supportedStencilResolveModes = pdevice->properties.supportedStencilResolveModes;
         properties->independentResolveNone = pdevice->properties.independentResolveNone;
         properties->independentResolve = pdevice->properties.independentResolve;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TRANSFORM_FEEDBACK_PROPERTIES_EXT: {
         VkPhysicalDeviceTransformFeedbackPropertiesEXT *properties = (void *)ext;
         properties->maxTransformFeedbackStreams = pdevice->properties.maxTransformFeedbackStreams;
         properties->maxTransformFeedbackBuffers = pdevice->properties.maxTransformFeedbackBuffers;
         properties->maxTransformFeedbackBufferSize = pdevice->properties.maxTransformFeedbackBufferSize;
         properties->maxTransformFeedbackStreamDataSize = pdevice->properties.maxTransformFeedbackStreamDataSize;
         properties->maxTransformFeedbackBufferDataSize = pdevice->properties.maxTransformFeedbackBufferDataSize;
         properties->maxTransformFeedbackBufferDataStride = pdevice->properties.maxTransformFeedbackBufferDataStride;
         properties->transformFeedbackQueries = pdevice->properties.transformFeedbackQueries;
         properties->transformFeedbackStreamsLinesTriangles = pdevice->properties.transformFeedbackStreamsLinesTriangles;
         properties->transformFeedbackRasterizationStreamSelect = pdevice->properties.transformFeedbackRasterizationStreamSelect;
         properties->transformFeedbackDraw = pdevice->properties.transformFeedbackDraw;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_COPY_MEMORY_INDIRECT_PROPERTIES_NV: {
         VkPhysicalDeviceCopyMemoryIndirectPropertiesNV *properties = (void *)ext;
         properties->supportedQueues = pdevice->properties.supportedQueues;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MEMORY_DECOMPRESSION_PROPERTIES_NV: {
         VkPhysicalDeviceMemoryDecompressionPropertiesNV *properties = (void *)ext;
         properties->decompressionMethods = pdevice->properties.decompressionMethods;
         properties->maxDecompressionIndirectCount = pdevice->properties.maxDecompressionIndirectCount;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADING_RATE_IMAGE_PROPERTIES_NV: {
         VkPhysicalDeviceShadingRateImagePropertiesNV *properties = (void *)ext;
         properties->shadingRateTexelSize = pdevice->properties.shadingRateTexelSize;
         properties->shadingRatePaletteSize = pdevice->properties.shadingRatePaletteSize;
         properties->shadingRateMaxCoarseSamples = pdevice->properties.shadingRateMaxCoarseSamples;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MESH_SHADER_PROPERTIES_NV: {
         VkPhysicalDeviceMeshShaderPropertiesNV *properties = (void *)ext;
         properties->maxDrawMeshTasksCount = pdevice->properties.maxDrawMeshTasksCount;
         properties->maxTaskWorkGroupInvocations = pdevice->properties.maxTaskWorkGroupInvocations;
         memcpy(properties->maxTaskWorkGroupSize, pdevice->properties.maxTaskWorkGroupSize, sizeof(properties->maxTaskWorkGroupSize));
         properties->maxTaskTotalMemorySize = pdevice->properties.maxTaskTotalMemorySize;
         properties->maxTaskOutputCount = pdevice->properties.maxTaskOutputCount;
         properties->maxMeshWorkGroupInvocations = pdevice->properties.maxMeshWorkGroupInvocations;
         memcpy(properties->maxMeshWorkGroupSize, pdevice->properties.maxMeshWorkGroupSize, sizeof(properties->maxMeshWorkGroupSize));
         properties->maxMeshTotalMemorySize = pdevice->properties.maxMeshTotalMemorySize;
         properties->maxMeshOutputVertices = pdevice->properties.maxMeshOutputVertices;
         properties->maxMeshOutputPrimitives = pdevice->properties.maxMeshOutputPrimitives;
         properties->maxMeshMultiviewViewCount = pdevice->properties.maxMeshMultiviewViewCount;
         properties->meshOutputPerVertexGranularity = pdevice->properties.meshOutputPerVertexGranularity;
         properties->meshOutputPerPrimitiveGranularity = pdevice->properties.meshOutputPerPrimitiveGranularity;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MESH_SHADER_PROPERTIES_EXT: {
         VkPhysicalDeviceMeshShaderPropertiesEXT *properties = (void *)ext;
         properties->maxTaskWorkGroupTotalCount = pdevice->properties.maxTaskWorkGroupTotalCount;
         memcpy(properties->maxTaskWorkGroupCount, pdevice->properties.maxTaskWorkGroupCount, sizeof(properties->maxTaskWorkGroupCount));
         properties->maxTaskWorkGroupInvocations = pdevice->properties.maxTaskWorkGroupInvocations;
         memcpy(properties->maxTaskWorkGroupSize, pdevice->properties.maxTaskWorkGroupSize, sizeof(properties->maxTaskWorkGroupSize));
         properties->maxTaskPayloadSize = pdevice->properties.maxTaskPayloadSize;
         properties->maxTaskSharedMemorySize = pdevice->properties.maxTaskSharedMemorySize;
         properties->maxTaskPayloadAndSharedMemorySize = pdevice->properties.maxTaskPayloadAndSharedMemorySize;
         properties->maxMeshWorkGroupTotalCount = pdevice->properties.maxMeshWorkGroupTotalCount;
         memcpy(properties->maxMeshWorkGroupCount, pdevice->properties.maxMeshWorkGroupCount, sizeof(properties->maxMeshWorkGroupCount));
         properties->maxMeshWorkGroupInvocations = pdevice->properties.maxMeshWorkGroupInvocations;
         memcpy(properties->maxMeshWorkGroupSize, pdevice->properties.maxMeshWorkGroupSize, sizeof(properties->maxMeshWorkGroupSize));
         properties->maxMeshSharedMemorySize = pdevice->properties.maxMeshSharedMemorySize;
         properties->maxMeshPayloadAndSharedMemorySize = pdevice->properties.maxMeshPayloadAndSharedMemorySize;
         properties->maxMeshOutputMemorySize = pdevice->properties.maxMeshOutputMemorySize;
         properties->maxMeshPayloadAndOutputMemorySize = pdevice->properties.maxMeshPayloadAndOutputMemorySize;
         properties->maxMeshOutputComponents = pdevice->properties.maxMeshOutputComponents;
         properties->maxMeshOutputVertices = pdevice->properties.maxMeshOutputVertices;
         properties->maxMeshOutputPrimitives = pdevice->properties.maxMeshOutputPrimitives;
         properties->maxMeshOutputLayers = pdevice->properties.maxMeshOutputLayers;
         properties->maxMeshMultiviewViewCount = pdevice->properties.maxMeshMultiviewViewCount;
         properties->meshOutputPerVertexGranularity = pdevice->properties.meshOutputPerVertexGranularity;
         properties->meshOutputPerPrimitiveGranularity = pdevice->properties.meshOutputPerPrimitiveGranularity;
         properties->maxPreferredTaskWorkGroupInvocations = pdevice->properties.maxPreferredTaskWorkGroupInvocations;
         properties->maxPreferredMeshWorkGroupInvocations = pdevice->properties.maxPreferredMeshWorkGroupInvocations;
         properties->prefersLocalInvocationVertexOutput = pdevice->properties.prefersLocalInvocationVertexOutput;
         properties->prefersLocalInvocationPrimitiveOutput = pdevice->properties.prefersLocalInvocationPrimitiveOutput;
         properties->prefersCompactVertexOutput = pdevice->properties.prefersCompactVertexOutput;
         properties->prefersCompactPrimitiveOutput = pdevice->properties.prefersCompactPrimitiveOutput;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ACCELERATION_STRUCTURE_PROPERTIES_KHR: {
         VkPhysicalDeviceAccelerationStructurePropertiesKHR *properties = (void *)ext;
         properties->maxGeometryCount = pdevice->properties.maxGeometryCount;
         properties->maxInstanceCount = pdevice->properties.maxInstanceCount;
         properties->maxPrimitiveCount = pdevice->properties.maxPrimitiveCount;
         properties->maxPerStageDescriptorAccelerationStructures = pdevice->properties.maxPerStageDescriptorAccelerationStructures;
         properties->maxPerStageDescriptorUpdateAfterBindAccelerationStructures = pdevice->properties.maxPerStageDescriptorUpdateAfterBindAccelerationStructures;
         properties->maxDescriptorSetAccelerationStructures = pdevice->properties.maxDescriptorSetAccelerationStructures;
         properties->maxDescriptorSetUpdateAfterBindAccelerationStructures = pdevice->properties.maxDescriptorSetUpdateAfterBindAccelerationStructures;
         properties->minAccelerationStructureScratchOffsetAlignment = pdevice->properties.minAccelerationStructureScratchOffsetAlignment;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_PIPELINE_PROPERTIES_KHR: {
         VkPhysicalDeviceRayTracingPipelinePropertiesKHR *properties = (void *)ext;
         properties->shaderGroupHandleSize = pdevice->properties.shaderGroupHandleSize;
         properties->maxRayRecursionDepth = pdevice->properties.maxRayRecursionDepth;
         properties->maxShaderGroupStride = pdevice->properties.maxShaderGroupStride;
         properties->shaderGroupBaseAlignment = pdevice->properties.shaderGroupBaseAlignment;
         properties->shaderGroupHandleCaptureReplaySize = pdevice->properties.shaderGroupHandleCaptureReplaySize;
         properties->maxRayDispatchInvocationCount = pdevice->properties.maxRayDispatchInvocationCount;
         properties->shaderGroupHandleAlignment = pdevice->properties.shaderGroupHandleAlignment;
         properties->maxRayHitAttributeSize = pdevice->properties.maxRayHitAttributeSize;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_PROPERTIES_NV: {
         VkPhysicalDeviceRayTracingPropertiesNV *properties = (void *)ext;
         properties->shaderGroupHandleSize = pdevice->properties.shaderGroupHandleSize;
         properties->maxRecursionDepth = pdevice->properties.maxRecursionDepth;
         properties->maxShaderGroupStride = pdevice->properties.maxShaderGroupStride;
         properties->shaderGroupBaseAlignment = pdevice->properties.shaderGroupBaseAlignment;
         properties->maxGeometryCount = pdevice->properties.maxGeometryCount;
         properties->maxInstanceCount = pdevice->properties.maxInstanceCount;
         properties->maxTriangleCount = pdevice->properties.maxTriangleCount;
         properties->maxDescriptorSetAccelerationStructures = pdevice->properties.maxDescriptorSetAccelerationStructures;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FRAGMENT_DENSITY_MAP_PROPERTIES_EXT: {
         VkPhysicalDeviceFragmentDensityMapPropertiesEXT *properties = (void *)ext;
         properties->minFragmentDensityTexelSize = pdevice->properties.minFragmentDensityTexelSize;
         properties->maxFragmentDensityTexelSize = pdevice->properties.maxFragmentDensityTexelSize;
         properties->fragmentDensityInvocations = pdevice->properties.fragmentDensityInvocations;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FRAGMENT_DENSITY_MAP_2_PROPERTIES_EXT: {
         VkPhysicalDeviceFragmentDensityMap2PropertiesEXT *properties = (void *)ext;
         properties->subsampledLoads = pdevice->properties.subsampledLoads;
         properties->subsampledCoarseReconstructionEarlyAccess = pdevice->properties.subsampledCoarseReconstructionEarlyAccess;
         properties->maxSubsampledArrayLayers = pdevice->properties.maxSubsampledArrayLayers;
         properties->maxDescriptorSetSubsampledSamplers = pdevice->properties.maxDescriptorSetSubsampledSamplers;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FRAGMENT_DENSITY_MAP_OFFSET_PROPERTIES_QCOM: {
         VkPhysicalDeviceFragmentDensityMapOffsetPropertiesQCOM *properties = (void *)ext;
         properties->fragmentDensityOffsetGranularity = pdevice->properties.fragmentDensityOffsetGranularity;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_COOPERATIVE_MATRIX_PROPERTIES_NV: {
         VkPhysicalDeviceCooperativeMatrixPropertiesNV *properties = (void *)ext;
         properties->cooperativeMatrixSupportedStages = pdevice->properties.cooperativeMatrixSupportedStages;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PERFORMANCE_QUERY_PROPERTIES_KHR: {
         VkPhysicalDevicePerformanceQueryPropertiesKHR *properties = (void *)ext;
         properties->allowCommandBufferQueryCopies = pdevice->properties.allowCommandBufferQueryCopies;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_SM_BUILTINS_PROPERTIES_NV: {
         VkPhysicalDeviceShaderSMBuiltinsPropertiesNV *properties = (void *)ext;
         properties->shaderSMCount = pdevice->properties.shaderSMCount;
         properties->shaderWarpsPerSM = pdevice->properties.shaderWarpsPerSM;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TEXEL_BUFFER_ALIGNMENT_PROPERTIES: {
         VkPhysicalDeviceTexelBufferAlignmentProperties *properties = (void *)ext;
         properties->storageTexelBufferOffsetAlignmentBytes = pdevice->properties.storageTexelBufferOffsetAlignmentBytes;
         properties->storageTexelBufferOffsetSingleTexelAlignment = pdevice->properties.storageTexelBufferOffsetSingleTexelAlignment;
         properties->uniformTexelBufferOffsetAlignmentBytes = pdevice->properties.uniformTexelBufferOffsetAlignmentBytes;
         properties->uniformTexelBufferOffsetSingleTexelAlignment = pdevice->properties.uniformTexelBufferOffsetSingleTexelAlignment;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SUBGROUP_SIZE_CONTROL_PROPERTIES: {
         VkPhysicalDeviceSubgroupSizeControlProperties *properties = (void *)ext;
         properties->minSubgroupSize = pdevice->properties.minSubgroupSize;
         properties->maxSubgroupSize = pdevice->properties.maxSubgroupSize;
         properties->maxComputeWorkgroupSubgroups = pdevice->properties.maxComputeWorkgroupSubgroups;
         properties->requiredSubgroupSizeStages = pdevice->properties.requiredSubgroupSizeStages;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SUBPASS_SHADING_PROPERTIES_HUAWEI: {
         VkPhysicalDeviceSubpassShadingPropertiesHUAWEI *properties = (void *)ext;
         properties->maxSubpassShadingWorkgroupSizeAspectRatio = pdevice->properties.maxSubpassShadingWorkgroupSizeAspectRatio;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_CLUSTER_CULLING_SHADER_PROPERTIES_HUAWEI: {
         VkPhysicalDeviceClusterCullingShaderPropertiesHUAWEI *properties = (void *)ext;
         memcpy(properties->maxWorkGroupCount, pdevice->properties.maxWorkGroupCount, sizeof(properties->maxWorkGroupCount));
         memcpy(properties->maxWorkGroupSize, pdevice->properties.maxWorkGroupSize, sizeof(properties->maxWorkGroupSize));
         properties->maxOutputClusterCount = pdevice->properties.maxOutputClusterCount;
         properties->indirectBufferOffsetAlignment = pdevice->properties.indirectBufferOffsetAlignment;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_LINE_RASTERIZATION_PROPERTIES_EXT: {
         VkPhysicalDeviceLineRasterizationPropertiesEXT *properties = (void *)ext;
         properties->lineSubPixelPrecisionBits = pdevice->properties.lineSubPixelPrecisionBits;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_PROPERTIES: {
         VkPhysicalDeviceVulkan11Properties *properties = (void *)ext;
         memcpy(properties->deviceUUID, pdevice->properties.deviceUUID, sizeof(properties->deviceUUID));
         memcpy(properties->driverUUID, pdevice->properties.driverUUID, sizeof(properties->driverUUID));
         memcpy(properties->deviceLUID, pdevice->properties.deviceLUID, sizeof(properties->deviceLUID));
         properties->deviceNodeMask = pdevice->properties.deviceNodeMask;
         properties->deviceLUIDValid = pdevice->properties.deviceLUIDValid;
         properties->subgroupSize = pdevice->properties.subgroupSize;
         properties->subgroupSupportedStages = pdevice->properties.subgroupSupportedStages;
         properties->subgroupSupportedOperations = pdevice->properties.subgroupSupportedOperations;
         properties->subgroupQuadOperationsInAllStages = pdevice->properties.subgroupQuadOperationsInAllStages;
         properties->pointClippingBehavior = pdevice->properties.pointClippingBehavior;
         properties->maxMultiviewViewCount = pdevice->properties.maxMultiviewViewCount;
         properties->maxMultiviewInstanceIndex = pdevice->properties.maxMultiviewInstanceIndex;
         properties->protectedNoFault = pdevice->properties.protectedNoFault;
         properties->maxPerSetDescriptors = pdevice->properties.maxPerSetDescriptors;
         properties->maxMemoryAllocationSize = pdevice->properties.maxMemoryAllocationSize;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_PROPERTIES: {
         VkPhysicalDeviceVulkan12Properties *properties = (void *)ext;
         properties->driverID = pdevice->properties.driverID;
         memcpy(properties->driverName, pdevice->properties.driverName, sizeof(properties->driverName));
         memcpy(properties->driverInfo, pdevice->properties.driverInfo, sizeof(properties->driverInfo));
         properties->conformanceVersion = pdevice->properties.conformanceVersion;
         properties->denormBehaviorIndependence = pdevice->properties.denormBehaviorIndependence;
         properties->roundingModeIndependence = pdevice->properties.roundingModeIndependence;
         properties->shaderSignedZeroInfNanPreserveFloat16 = pdevice->properties.shaderSignedZeroInfNanPreserveFloat16;
         properties->shaderSignedZeroInfNanPreserveFloat32 = pdevice->properties.shaderSignedZeroInfNanPreserveFloat32;
         properties->shaderSignedZeroInfNanPreserveFloat64 = pdevice->properties.shaderSignedZeroInfNanPreserveFloat64;
         properties->shaderDenormPreserveFloat16 = pdevice->properties.shaderDenormPreserveFloat16;
         properties->shaderDenormPreserveFloat32 = pdevice->properties.shaderDenormPreserveFloat32;
         properties->shaderDenormPreserveFloat64 = pdevice->properties.shaderDenormPreserveFloat64;
         properties->shaderDenormFlushToZeroFloat16 = pdevice->properties.shaderDenormFlushToZeroFloat16;
         properties->shaderDenormFlushToZeroFloat32 = pdevice->properties.shaderDenormFlushToZeroFloat32;
         properties->shaderDenormFlushToZeroFloat64 = pdevice->properties.shaderDenormFlushToZeroFloat64;
         properties->shaderRoundingModeRTEFloat16 = pdevice->properties.shaderRoundingModeRTEFloat16;
         properties->shaderRoundingModeRTEFloat32 = pdevice->properties.shaderRoundingModeRTEFloat32;
         properties->shaderRoundingModeRTEFloat64 = pdevice->properties.shaderRoundingModeRTEFloat64;
         properties->shaderRoundingModeRTZFloat16 = pdevice->properties.shaderRoundingModeRTZFloat16;
         properties->shaderRoundingModeRTZFloat32 = pdevice->properties.shaderRoundingModeRTZFloat32;
         properties->shaderRoundingModeRTZFloat64 = pdevice->properties.shaderRoundingModeRTZFloat64;
         properties->maxUpdateAfterBindDescriptorsInAllPools = pdevice->properties.maxUpdateAfterBindDescriptorsInAllPools;
         properties->shaderUniformBufferArrayNonUniformIndexingNative = pdevice->properties.shaderUniformBufferArrayNonUniformIndexingNative;
         properties->shaderSampledImageArrayNonUniformIndexingNative = pdevice->properties.shaderSampledImageArrayNonUniformIndexingNative;
         properties->shaderStorageBufferArrayNonUniformIndexingNative = pdevice->properties.shaderStorageBufferArrayNonUniformIndexingNative;
         properties->shaderStorageImageArrayNonUniformIndexingNative = pdevice->properties.shaderStorageImageArrayNonUniformIndexingNative;
         properties->shaderInputAttachmentArrayNonUniformIndexingNative = pdevice->properties.shaderInputAttachmentArrayNonUniformIndexingNative;
         properties->robustBufferAccessUpdateAfterBind = pdevice->properties.robustBufferAccessUpdateAfterBind;
         properties->quadDivergentImplicitLod = pdevice->properties.quadDivergentImplicitLod;
         properties->maxPerStageDescriptorUpdateAfterBindSamplers = pdevice->properties.maxPerStageDescriptorUpdateAfterBindSamplers;
         properties->maxPerStageDescriptorUpdateAfterBindUniformBuffers = pdevice->properties.maxPerStageDescriptorUpdateAfterBindUniformBuffers;
         properties->maxPerStageDescriptorUpdateAfterBindStorageBuffers = pdevice->properties.maxPerStageDescriptorUpdateAfterBindStorageBuffers;
         properties->maxPerStageDescriptorUpdateAfterBindSampledImages = pdevice->properties.maxPerStageDescriptorUpdateAfterBindSampledImages;
         properties->maxPerStageDescriptorUpdateAfterBindStorageImages = pdevice->properties.maxPerStageDescriptorUpdateAfterBindStorageImages;
         properties->maxPerStageDescriptorUpdateAfterBindInputAttachments = pdevice->properties.maxPerStageDescriptorUpdateAfterBindInputAttachments;
         properties->maxPerStageUpdateAfterBindResources = pdevice->properties.maxPerStageUpdateAfterBindResources;
         properties->maxDescriptorSetUpdateAfterBindSamplers = pdevice->properties.maxDescriptorSetUpdateAfterBindSamplers;
         properties->maxDescriptorSetUpdateAfterBindUniformBuffers = pdevice->properties.maxDescriptorSetUpdateAfterBindUniformBuffers;
         properties->maxDescriptorSetUpdateAfterBindUniformBuffersDynamic = pdevice->properties.maxDescriptorSetUpdateAfterBindUniformBuffersDynamic;
         properties->maxDescriptorSetUpdateAfterBindStorageBuffers = pdevice->properties.maxDescriptorSetUpdateAfterBindStorageBuffers;
         properties->maxDescriptorSetUpdateAfterBindStorageBuffersDynamic = pdevice->properties.maxDescriptorSetUpdateAfterBindStorageBuffersDynamic;
         properties->maxDescriptorSetUpdateAfterBindSampledImages = pdevice->properties.maxDescriptorSetUpdateAfterBindSampledImages;
         properties->maxDescriptorSetUpdateAfterBindStorageImages = pdevice->properties.maxDescriptorSetUpdateAfterBindStorageImages;
         properties->maxDescriptorSetUpdateAfterBindInputAttachments = pdevice->properties.maxDescriptorSetUpdateAfterBindInputAttachments;
         properties->supportedDepthResolveModes = pdevice->properties.supportedDepthResolveModes;
         properties->supportedStencilResolveModes = pdevice->properties.supportedStencilResolveModes;
         properties->independentResolveNone = pdevice->properties.independentResolveNone;
         properties->independentResolve = pdevice->properties.independentResolve;
         properties->filterMinmaxSingleComponentFormats = pdevice->properties.filterMinmaxSingleComponentFormats;
         properties->filterMinmaxImageComponentMapping = pdevice->properties.filterMinmaxImageComponentMapping;
         properties->maxTimelineSemaphoreValueDifference = pdevice->properties.maxTimelineSemaphoreValueDifference;
         properties->framebufferIntegerColorSampleCounts = pdevice->properties.framebufferIntegerColorSampleCounts;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_PROPERTIES: {
         VkPhysicalDeviceVulkan13Properties *properties = (void *)ext;
         properties->minSubgroupSize = pdevice->properties.minSubgroupSize;
         properties->maxSubgroupSize = pdevice->properties.maxSubgroupSize;
         properties->maxComputeWorkgroupSubgroups = pdevice->properties.maxComputeWorkgroupSubgroups;
         properties->requiredSubgroupSizeStages = pdevice->properties.requiredSubgroupSizeStages;
         properties->maxInlineUniformBlockSize = pdevice->properties.maxInlineUniformBlockSize;
         properties->maxPerStageDescriptorInlineUniformBlocks = pdevice->properties.maxPerStageDescriptorInlineUniformBlocks;
         properties->maxPerStageDescriptorUpdateAfterBindInlineUniformBlocks = pdevice->properties.maxPerStageDescriptorUpdateAfterBindInlineUniformBlocks;
         properties->maxDescriptorSetInlineUniformBlocks = pdevice->properties.maxDescriptorSetInlineUniformBlocks;
         properties->maxDescriptorSetUpdateAfterBindInlineUniformBlocks = pdevice->properties.maxDescriptorSetUpdateAfterBindInlineUniformBlocks;
         properties->maxInlineUniformTotalSize = pdevice->properties.maxInlineUniformTotalSize;
         properties->integerDotProduct8BitUnsignedAccelerated = pdevice->properties.integerDotProduct8BitUnsignedAccelerated;
         properties->integerDotProduct8BitSignedAccelerated = pdevice->properties.integerDotProduct8BitSignedAccelerated;
         properties->integerDotProduct8BitMixedSignednessAccelerated = pdevice->properties.integerDotProduct8BitMixedSignednessAccelerated;
         properties->integerDotProduct4x8BitPackedUnsignedAccelerated = pdevice->properties.integerDotProduct4x8BitPackedUnsignedAccelerated;
         properties->integerDotProduct4x8BitPackedSignedAccelerated = pdevice->properties.integerDotProduct4x8BitPackedSignedAccelerated;
         properties->integerDotProduct4x8BitPackedMixedSignednessAccelerated = pdevice->properties.integerDotProduct4x8BitPackedMixedSignednessAccelerated;
         properties->integerDotProduct16BitUnsignedAccelerated = pdevice->properties.integerDotProduct16BitUnsignedAccelerated;
         properties->integerDotProduct16BitSignedAccelerated = pdevice->properties.integerDotProduct16BitSignedAccelerated;
         properties->integerDotProduct16BitMixedSignednessAccelerated = pdevice->properties.integerDotProduct16BitMixedSignednessAccelerated;
         properties->integerDotProduct32BitUnsignedAccelerated = pdevice->properties.integerDotProduct32BitUnsignedAccelerated;
         properties->integerDotProduct32BitSignedAccelerated = pdevice->properties.integerDotProduct32BitSignedAccelerated;
         properties->integerDotProduct32BitMixedSignednessAccelerated = pdevice->properties.integerDotProduct32BitMixedSignednessAccelerated;
         properties->integerDotProduct64BitUnsignedAccelerated = pdevice->properties.integerDotProduct64BitUnsignedAccelerated;
         properties->integerDotProduct64BitSignedAccelerated = pdevice->properties.integerDotProduct64BitSignedAccelerated;
         properties->integerDotProduct64BitMixedSignednessAccelerated = pdevice->properties.integerDotProduct64BitMixedSignednessAccelerated;
         properties->integerDotProductAccumulatingSaturating8BitUnsignedAccelerated = pdevice->properties.integerDotProductAccumulatingSaturating8BitUnsignedAccelerated;
         properties->integerDotProductAccumulatingSaturating8BitSignedAccelerated = pdevice->properties.integerDotProductAccumulatingSaturating8BitSignedAccelerated;
         properties->integerDotProductAccumulatingSaturating8BitMixedSignednessAccelerated = pdevice->properties.integerDotProductAccumulatingSaturating8BitMixedSignednessAccelerated;
         properties->integerDotProductAccumulatingSaturating4x8BitPackedUnsignedAccelerated = pdevice->properties.integerDotProductAccumulatingSaturating4x8BitPackedUnsignedAccelerated;
         properties->integerDotProductAccumulatingSaturating4x8BitPackedSignedAccelerated = pdevice->properties.integerDotProductAccumulatingSaturating4x8BitPackedSignedAccelerated;
         properties->integerDotProductAccumulatingSaturating4x8BitPackedMixedSignednessAccelerated = pdevice->properties.integerDotProductAccumulatingSaturating4x8BitPackedMixedSignednessAccelerated;
         properties->integerDotProductAccumulatingSaturating16BitUnsignedAccelerated = pdevice->properties.integerDotProductAccumulatingSaturating16BitUnsignedAccelerated;
         properties->integerDotProductAccumulatingSaturating16BitSignedAccelerated = pdevice->properties.integerDotProductAccumulatingSaturating16BitSignedAccelerated;
         properties->integerDotProductAccumulatingSaturating16BitMixedSignednessAccelerated = pdevice->properties.integerDotProductAccumulatingSaturating16BitMixedSignednessAccelerated;
         properties->integerDotProductAccumulatingSaturating32BitUnsignedAccelerated = pdevice->properties.integerDotProductAccumulatingSaturating32BitUnsignedAccelerated;
         properties->integerDotProductAccumulatingSaturating32BitSignedAccelerated = pdevice->properties.integerDotProductAccumulatingSaturating32BitSignedAccelerated;
         properties->integerDotProductAccumulatingSaturating32BitMixedSignednessAccelerated = pdevice->properties.integerDotProductAccumulatingSaturating32BitMixedSignednessAccelerated;
         properties->integerDotProductAccumulatingSaturating64BitUnsignedAccelerated = pdevice->properties.integerDotProductAccumulatingSaturating64BitUnsignedAccelerated;
         properties->integerDotProductAccumulatingSaturating64BitSignedAccelerated = pdevice->properties.integerDotProductAccumulatingSaturating64BitSignedAccelerated;
         properties->integerDotProductAccumulatingSaturating64BitMixedSignednessAccelerated = pdevice->properties.integerDotProductAccumulatingSaturating64BitMixedSignednessAccelerated;
         properties->storageTexelBufferOffsetAlignmentBytes = pdevice->properties.storageTexelBufferOffsetAlignmentBytes;
         properties->storageTexelBufferOffsetSingleTexelAlignment = pdevice->properties.storageTexelBufferOffsetSingleTexelAlignment;
         properties->uniformTexelBufferOffsetAlignmentBytes = pdevice->properties.uniformTexelBufferOffsetAlignmentBytes;
         properties->uniformTexelBufferOffsetSingleTexelAlignment = pdevice->properties.uniformTexelBufferOffsetSingleTexelAlignment;
         properties->maxBufferSize = pdevice->properties.maxBufferSize;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_CUSTOM_BORDER_COLOR_PROPERTIES_EXT: {
         VkPhysicalDeviceCustomBorderColorPropertiesEXT *properties = (void *)ext;
         properties->maxCustomBorderColorSamplers = pdevice->properties.maxCustomBorderColorSamplers;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTENDED_DYNAMIC_STATE_3_PROPERTIES_EXT: {
         VkPhysicalDeviceExtendedDynamicState3PropertiesEXT *properties = (void *)ext;
         properties->dynamicPrimitiveTopologyUnrestricted = pdevice->properties.dynamicPrimitiveTopologyUnrestricted;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ROBUSTNESS_2_PROPERTIES_EXT: {
         VkPhysicalDeviceRobustness2PropertiesEXT *properties = (void *)ext;
         properties->robustStorageBufferAccessSizeAlignment = pdevice->properties.robustStorageBufferAccessSizeAlignment;
         properties->robustUniformBufferAccessSizeAlignment = pdevice->properties.robustUniformBufferAccessSizeAlignment;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FRAGMENT_SHADING_RATE_PROPERTIES_KHR: {
         VkPhysicalDeviceFragmentShadingRatePropertiesKHR *properties = (void *)ext;
         properties->minFragmentShadingRateAttachmentTexelSize = pdevice->properties.minFragmentShadingRateAttachmentTexelSize;
         properties->maxFragmentShadingRateAttachmentTexelSize = pdevice->properties.maxFragmentShadingRateAttachmentTexelSize;
         properties->maxFragmentShadingRateAttachmentTexelSizeAspectRatio = pdevice->properties.maxFragmentShadingRateAttachmentTexelSizeAspectRatio;
         properties->primitiveFragmentShadingRateWithMultipleViewports = pdevice->properties.primitiveFragmentShadingRateWithMultipleViewports;
         properties->layeredShadingRateAttachments = pdevice->properties.layeredShadingRateAttachments;
         properties->fragmentShadingRateNonTrivialCombinerOps = pdevice->properties.fragmentShadingRateNonTrivialCombinerOps;
         properties->maxFragmentSize = pdevice->properties.maxFragmentSize;
         properties->maxFragmentSizeAspectRatio = pdevice->properties.maxFragmentSizeAspectRatio;
         properties->maxFragmentShadingRateCoverageSamples = pdevice->properties.maxFragmentShadingRateCoverageSamples;
         properties->maxFragmentShadingRateRasterizationSamples = pdevice->properties.maxFragmentShadingRateRasterizationSamples;
         properties->fragmentShadingRateWithShaderDepthStencilWrites = pdevice->properties.fragmentShadingRateWithShaderDepthStencilWrites;
         properties->fragmentShadingRateWithSampleMask = pdevice->properties.fragmentShadingRateWithSampleMask;
         properties->fragmentShadingRateWithShaderSampleMask = pdevice->properties.fragmentShadingRateWithShaderSampleMask;
         properties->fragmentShadingRateWithConservativeRasterization = pdevice->properties.fragmentShadingRateWithConservativeRasterization;
         properties->fragmentShadingRateWithFragmentShaderInterlock = pdevice->properties.fragmentShadingRateWithFragmentShaderInterlock;
         properties->fragmentShadingRateWithCustomSampleLocations = pdevice->properties.fragmentShadingRateWithCustomSampleLocations;
         properties->fragmentShadingRateStrictMultiplyCombiner = pdevice->properties.fragmentShadingRateStrictMultiplyCombiner;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FRAGMENT_SHADING_RATE_ENUMS_PROPERTIES_NV: {
         VkPhysicalDeviceFragmentShadingRateEnumsPropertiesNV *properties = (void *)ext;
         properties->maxFragmentShadingRateInvocationCount = pdevice->properties.maxFragmentShadingRateInvocationCount;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROVOKING_VERTEX_PROPERTIES_EXT: {
         VkPhysicalDeviceProvokingVertexPropertiesEXT *properties = (void *)ext;
         properties->provokingVertexModePerPipeline = pdevice->properties.provokingVertexModePerPipeline;
         properties->transformFeedbackPreservesTriangleFanProvokingVertex = pdevice->properties.transformFeedbackPreservesTriangleFanProvokingVertex;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_BUFFER_PROPERTIES_EXT: {
         VkPhysicalDeviceDescriptorBufferPropertiesEXT *properties = (void *)ext;
         properties->combinedImageSamplerDescriptorSingleArray = pdevice->properties.combinedImageSamplerDescriptorSingleArray;
         properties->bufferlessPushDescriptors = pdevice->properties.bufferlessPushDescriptors;
         properties->allowSamplerImageViewPostSubmitCreation = pdevice->properties.allowSamplerImageViewPostSubmitCreation;
         properties->descriptorBufferOffsetAlignment = pdevice->properties.descriptorBufferOffsetAlignment;
         properties->maxDescriptorBufferBindings = pdevice->properties.maxDescriptorBufferBindings;
         properties->maxResourceDescriptorBufferBindings = pdevice->properties.maxResourceDescriptorBufferBindings;
         properties->maxSamplerDescriptorBufferBindings = pdevice->properties.maxSamplerDescriptorBufferBindings;
         properties->maxEmbeddedImmutableSamplerBindings = pdevice->properties.maxEmbeddedImmutableSamplerBindings;
         properties->maxEmbeddedImmutableSamplers = pdevice->properties.maxEmbeddedImmutableSamplers;
         properties->bufferCaptureReplayDescriptorDataSize = pdevice->properties.bufferCaptureReplayDescriptorDataSize;
         properties->imageCaptureReplayDescriptorDataSize = pdevice->properties.imageCaptureReplayDescriptorDataSize;
         properties->imageViewCaptureReplayDescriptorDataSize = pdevice->properties.imageViewCaptureReplayDescriptorDataSize;
         properties->samplerCaptureReplayDescriptorDataSize = pdevice->properties.samplerCaptureReplayDescriptorDataSize;
         properties->accelerationStructureCaptureReplayDescriptorDataSize = pdevice->properties.accelerationStructureCaptureReplayDescriptorDataSize;
         properties->samplerDescriptorSize = pdevice->properties.samplerDescriptorSize;
         properties->combinedImageSamplerDescriptorSize = pdevice->properties.combinedImageSamplerDescriptorSize;
         properties->sampledImageDescriptorSize = pdevice->properties.sampledImageDescriptorSize;
         properties->storageImageDescriptorSize = pdevice->properties.storageImageDescriptorSize;
         properties->uniformTexelBufferDescriptorSize = pdevice->properties.uniformTexelBufferDescriptorSize;
         properties->robustUniformTexelBufferDescriptorSize = pdevice->properties.robustUniformTexelBufferDescriptorSize;
         properties->storageTexelBufferDescriptorSize = pdevice->properties.storageTexelBufferDescriptorSize;
         properties->robustStorageTexelBufferDescriptorSize = pdevice->properties.robustStorageTexelBufferDescriptorSize;
         properties->uniformBufferDescriptorSize = pdevice->properties.uniformBufferDescriptorSize;
         properties->robustUniformBufferDescriptorSize = pdevice->properties.robustUniformBufferDescriptorSize;
         properties->storageBufferDescriptorSize = pdevice->properties.storageBufferDescriptorSize;
         properties->robustStorageBufferDescriptorSize = pdevice->properties.robustStorageBufferDescriptorSize;
         properties->inputAttachmentDescriptorSize = pdevice->properties.inputAttachmentDescriptorSize;
         properties->accelerationStructureDescriptorSize = pdevice->properties.accelerationStructureDescriptorSize;
         properties->maxSamplerDescriptorBufferRange = pdevice->properties.maxSamplerDescriptorBufferRange;
         properties->maxResourceDescriptorBufferRange = pdevice->properties.maxResourceDescriptorBufferRange;
         properties->samplerDescriptorBufferAddressSpaceSize = pdevice->properties.samplerDescriptorBufferAddressSpaceSize;
         properties->resourceDescriptorBufferAddressSpaceSize = pdevice->properties.resourceDescriptorBufferAddressSpaceSize;
         properties->descriptorBufferAddressSpaceSize = pdevice->properties.descriptorBufferAddressSpaceSize;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_BUFFER_DENSITY_MAP_PROPERTIES_EXT: {
         VkPhysicalDeviceDescriptorBufferDensityMapPropertiesEXT *properties = (void *)ext;
         properties->combinedImageSamplerDensityMapDescriptorSize = pdevice->properties.combinedImageSamplerDensityMapDescriptorSize;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_INTEGER_DOT_PRODUCT_PROPERTIES: {
         VkPhysicalDeviceShaderIntegerDotProductProperties *properties = (void *)ext;
         properties->integerDotProduct8BitUnsignedAccelerated = pdevice->properties.integerDotProduct8BitUnsignedAccelerated;
         properties->integerDotProduct8BitSignedAccelerated = pdevice->properties.integerDotProduct8BitSignedAccelerated;
         properties->integerDotProduct8BitMixedSignednessAccelerated = pdevice->properties.integerDotProduct8BitMixedSignednessAccelerated;
         properties->integerDotProduct4x8BitPackedUnsignedAccelerated = pdevice->properties.integerDotProduct4x8BitPackedUnsignedAccelerated;
         properties->integerDotProduct4x8BitPackedSignedAccelerated = pdevice->properties.integerDotProduct4x8BitPackedSignedAccelerated;
         properties->integerDotProduct4x8BitPackedMixedSignednessAccelerated = pdevice->properties.integerDotProduct4x8BitPackedMixedSignednessAccelerated;
         properties->integerDotProduct16BitUnsignedAccelerated = pdevice->properties.integerDotProduct16BitUnsignedAccelerated;
         properties->integerDotProduct16BitSignedAccelerated = pdevice->properties.integerDotProduct16BitSignedAccelerated;
         properties->integerDotProduct16BitMixedSignednessAccelerated = pdevice->properties.integerDotProduct16BitMixedSignednessAccelerated;
         properties->integerDotProduct32BitUnsignedAccelerated = pdevice->properties.integerDotProduct32BitUnsignedAccelerated;
         properties->integerDotProduct32BitSignedAccelerated = pdevice->properties.integerDotProduct32BitSignedAccelerated;
         properties->integerDotProduct32BitMixedSignednessAccelerated = pdevice->properties.integerDotProduct32BitMixedSignednessAccelerated;
         properties->integerDotProduct64BitUnsignedAccelerated = pdevice->properties.integerDotProduct64BitUnsignedAccelerated;
         properties->integerDotProduct64BitSignedAccelerated = pdevice->properties.integerDotProduct64BitSignedAccelerated;
         properties->integerDotProduct64BitMixedSignednessAccelerated = pdevice->properties.integerDotProduct64BitMixedSignednessAccelerated;
         properties->integerDotProductAccumulatingSaturating8BitUnsignedAccelerated = pdevice->properties.integerDotProductAccumulatingSaturating8BitUnsignedAccelerated;
         properties->integerDotProductAccumulatingSaturating8BitSignedAccelerated = pdevice->properties.integerDotProductAccumulatingSaturating8BitSignedAccelerated;
         properties->integerDotProductAccumulatingSaturating8BitMixedSignednessAccelerated = pdevice->properties.integerDotProductAccumulatingSaturating8BitMixedSignednessAccelerated;
         properties->integerDotProductAccumulatingSaturating4x8BitPackedUnsignedAccelerated = pdevice->properties.integerDotProductAccumulatingSaturating4x8BitPackedUnsignedAccelerated;
         properties->integerDotProductAccumulatingSaturating4x8BitPackedSignedAccelerated = pdevice->properties.integerDotProductAccumulatingSaturating4x8BitPackedSignedAccelerated;
         properties->integerDotProductAccumulatingSaturating4x8BitPackedMixedSignednessAccelerated = pdevice->properties.integerDotProductAccumulatingSaturating4x8BitPackedMixedSignednessAccelerated;
         properties->integerDotProductAccumulatingSaturating16BitUnsignedAccelerated = pdevice->properties.integerDotProductAccumulatingSaturating16BitUnsignedAccelerated;
         properties->integerDotProductAccumulatingSaturating16BitSignedAccelerated = pdevice->properties.integerDotProductAccumulatingSaturating16BitSignedAccelerated;
         properties->integerDotProductAccumulatingSaturating16BitMixedSignednessAccelerated = pdevice->properties.integerDotProductAccumulatingSaturating16BitMixedSignednessAccelerated;
         properties->integerDotProductAccumulatingSaturating32BitUnsignedAccelerated = pdevice->properties.integerDotProductAccumulatingSaturating32BitUnsignedAccelerated;
         properties->integerDotProductAccumulatingSaturating32BitSignedAccelerated = pdevice->properties.integerDotProductAccumulatingSaturating32BitSignedAccelerated;
         properties->integerDotProductAccumulatingSaturating32BitMixedSignednessAccelerated = pdevice->properties.integerDotProductAccumulatingSaturating32BitMixedSignednessAccelerated;
         properties->integerDotProductAccumulatingSaturating64BitUnsignedAccelerated = pdevice->properties.integerDotProductAccumulatingSaturating64BitUnsignedAccelerated;
         properties->integerDotProductAccumulatingSaturating64BitSignedAccelerated = pdevice->properties.integerDotProductAccumulatingSaturating64BitSignedAccelerated;
         properties->integerDotProductAccumulatingSaturating64BitMixedSignednessAccelerated = pdevice->properties.integerDotProductAccumulatingSaturating64BitMixedSignednessAccelerated;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DRM_PROPERTIES_EXT: {
         VkPhysicalDeviceDrmPropertiesEXT *properties = (void *)ext;
         properties->hasPrimary = pdevice->properties.drmHasPrimary;
         properties->hasRender = pdevice->properties.drmHasRender;
         properties->primaryMajor = pdevice->properties.drmPrimaryMajor;
         properties->primaryMinor = pdevice->properties.drmPrimaryMinor;
         properties->renderMajor = pdevice->properties.drmRenderMajor;
         properties->renderMinor = pdevice->properties.drmRenderMinor;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FRAGMENT_SHADER_BARYCENTRIC_PROPERTIES_KHR: {
         VkPhysicalDeviceFragmentShaderBarycentricPropertiesKHR *properties = (void *)ext;
         properties->triStripVertexOrderIndependentOfProvokingVertex = pdevice->properties.triStripVertexOrderIndependentOfProvokingVertex;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_GRAPHICS_PIPELINE_LIBRARY_PROPERTIES_EXT: {
         VkPhysicalDeviceGraphicsPipelineLibraryPropertiesEXT *properties = (void *)ext;
         properties->graphicsPipelineLibraryFastLinking = pdevice->properties.graphicsPipelineLibraryFastLinking;
         properties->graphicsPipelineLibraryIndependentInterpolationDecoration = pdevice->properties.graphicsPipelineLibraryIndependentInterpolationDecoration;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_NESTED_COMMAND_BUFFER_PROPERTIES_EXT: {
         VkPhysicalDeviceNestedCommandBufferPropertiesEXT *properties = (void *)ext;
         properties->maxCommandBufferNestingLevel = pdevice->properties.maxCommandBufferNestingLevel;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_MODULE_IDENTIFIER_PROPERTIES_EXT: {
         VkPhysicalDeviceShaderModuleIdentifierPropertiesEXT *properties = (void *)ext;
         memcpy(properties->shaderModuleIdentifierAlgorithmUUID, pdevice->properties.shaderModuleIdentifierAlgorithmUUID, sizeof(properties->shaderModuleIdentifierAlgorithmUUID));
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_OPACITY_MICROMAP_PROPERTIES_EXT: {
         VkPhysicalDeviceOpacityMicromapPropertiesEXT *properties = (void *)ext;
         properties->maxOpacity2StateSubdivisionLevel = pdevice->properties.maxOpacity2StateSubdivisionLevel;
         properties->maxOpacity4StateSubdivisionLevel = pdevice->properties.maxOpacity4StateSubdivisionLevel;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PIPELINE_ROBUSTNESS_PROPERTIES_EXT: {
         VkPhysicalDevicePipelineRobustnessPropertiesEXT *properties = (void *)ext;
         properties->defaultRobustnessStorageBuffers = pdevice->properties.defaultRobustnessStorageBuffers;
         properties->defaultRobustnessUniformBuffers = pdevice->properties.defaultRobustnessUniformBuffers;
         properties->defaultRobustnessVertexInputs = pdevice->properties.defaultRobustnessVertexInputs;
         properties->defaultRobustnessImages = pdevice->properties.defaultRobustnessImages;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_IMAGE_PROCESSING_PROPERTIES_QCOM: {
         VkPhysicalDeviceImageProcessingPropertiesQCOM *properties = (void *)ext;
         properties->maxWeightFilterPhases = pdevice->properties.maxWeightFilterPhases;
         properties->maxWeightFilterDimension = pdevice->properties.maxWeightFilterDimension;
         properties->maxBlockMatchRegion = pdevice->properties.maxBlockMatchRegion;
         properties->maxBoxFilterBlockSize = pdevice->properties.maxBoxFilterBlockSize;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_OPTICAL_FLOW_PROPERTIES_NV: {
         VkPhysicalDeviceOpticalFlowPropertiesNV *properties = (void *)ext;
         properties->supportedOutputGridSizes = pdevice->properties.supportedOutputGridSizes;
         properties->supportedHintGridSizes = pdevice->properties.supportedHintGridSizes;
         properties->hintSupported = pdevice->properties.hintSupported;
         properties->costSupported = pdevice->properties.costSupported;
         properties->bidirectionalFlowSupported = pdevice->properties.bidirectionalFlowSupported;
         properties->globalFlowSupported = pdevice->properties.globalFlowSupported;
         properties->minWidth = pdevice->properties.minWidth;
         properties->minHeight = pdevice->properties.minHeight;
         properties->maxWidth = pdevice->properties.maxWidth;
         properties->maxHeight = pdevice->properties.maxHeight;
         properties->maxNumRegionsOfInterest = pdevice->properties.maxNumRegionsOfInterest;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_CORE_BUILTINS_PROPERTIES_ARM: {
         VkPhysicalDeviceShaderCoreBuiltinsPropertiesARM *properties = (void *)ext;
         properties->shaderCoreMask = pdevice->properties.shaderCoreMask;
         properties->shaderCoreCount = pdevice->properties.shaderCoreCount;
         properties->shaderWarpsPerCore = pdevice->properties.shaderWarpsPerCore;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_INVOCATION_REORDER_PROPERTIES_NV: {
         VkPhysicalDeviceRayTracingInvocationReorderPropertiesNV *properties = (void *)ext;
         properties->rayTracingInvocationReorderReorderingHint = pdevice->properties.rayTracingInvocationReorderReorderingHint;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTENDED_SPARSE_ADDRESS_SPACE_PROPERTIES_NV: {
         VkPhysicalDeviceExtendedSparseAddressSpacePropertiesNV *properties = (void *)ext;
         properties->extendedSparseAddressSpaceSize = pdevice->properties.extendedSparseAddressSpaceSize;
         properties->extendedSparseImageUsageFlags = pdevice->properties.extendedSparseImageUsageFlags;
         properties->extendedSparseBufferUsageFlags = pdevice->properties.extendedSparseBufferUsageFlags;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_CORE_PROPERTIES_ARM: {
         VkPhysicalDeviceShaderCorePropertiesARM *properties = (void *)ext;
         properties->pixelRate = pdevice->properties.pixelRate;
         properties->texelRate = pdevice->properties.texelRate;
         properties->fmaRate = pdevice->properties.fmaRate;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_OBJECT_PROPERTIES_EXT: {
         VkPhysicalDeviceShaderObjectPropertiesEXT *properties = (void *)ext;
         memcpy(properties->shaderBinaryUUID, pdevice->properties.shaderBinaryUUID, sizeof(properties->shaderBinaryUUID));
         properties->shaderBinaryVersion = pdevice->properties.shaderBinaryVersion;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_TILE_IMAGE_PROPERTIES_EXT: {
         VkPhysicalDeviceShaderTileImagePropertiesEXT *properties = (void *)ext;
         properties->shaderTileImageCoherentReadAccelerated = pdevice->properties.shaderTileImageCoherentReadAccelerated;
         properties->shaderTileImageReadSampleFromPixelRateInvocation = pdevice->properties.shaderTileImageReadSampleFromPixelRateInvocation;
         properties->shaderTileImageReadFromHelperInvocation = pdevice->properties.shaderTileImageReadFromHelperInvocation;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_COOPERATIVE_MATRIX_PROPERTIES_KHR: {
         VkPhysicalDeviceCooperativeMatrixPropertiesKHR *properties = (void *)ext;
         properties->cooperativeMatrixSupportedStages = pdevice->properties.cooperativeMatrixSupportedStages;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_IMAGE_PROCESSING_2_PROPERTIES_QCOM: {
         VkPhysicalDeviceImageProcessing2PropertiesQCOM *properties = (void *)ext;
         properties->maxBlockMatchWindow = pdevice->properties.maxBlockMatchWindow;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_LAYERED_DRIVER_PROPERTIES_MSFT: {
         VkPhysicalDeviceLayeredDriverPropertiesMSFT *properties = (void *)ext;
         properties->underlyingAPI = pdevice->properties.underlyingAPI;
         break;
      }

      /* Specialized propery handling defined in vk_physical_device_properties_gen.py */

      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_HOST_IMAGE_COPY_PROPERTIES_EXT: {
         VkPhysicalDeviceHostImageCopyPropertiesEXT *properties = (void *)ext;

         if (properties->pCopySrcLayouts) {
            uint32_t written_layout_count = MIN2(properties->copySrcLayoutCount,
                                                 pdevice->properties.copySrcLayoutCount);
            memcpy(properties->pCopySrcLayouts, pdevice->properties.pCopySrcLayouts,
                   sizeof(VkImageLayout) * written_layout_count);
            properties->copySrcLayoutCount = written_layout_count;
         } else {
            properties->copySrcLayoutCount = pdevice->properties.copySrcLayoutCount;
         }

         if (properties->pCopyDstLayouts) {
            uint32_t written_layout_count = MIN2(properties->copyDstLayoutCount,
                                                 pdevice->properties.copyDstLayoutCount);
            memcpy(properties->pCopyDstLayouts, pdevice->properties.pCopyDstLayouts,
                   sizeof(VkImageLayout) * written_layout_count);
            properties->copyDstLayoutCount = written_layout_count;
         } else {
            properties->copyDstLayoutCount = pdevice->properties.copyDstLayoutCount;
         }

         memcpy(properties->optimalTilingLayoutUUID, pdevice->properties.optimalTilingLayoutUUID, VK_UUID_SIZE);
         properties->identicalMemoryTypeRequirements = pdevice->properties.identicalMemoryTypeRequirements;
         break;
      }

      default:
         break;
      }
   }
}
