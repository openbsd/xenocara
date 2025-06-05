
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

   vk_foreach_struct(ext, pProperties->pNext) {
      switch ((int32_t)ext->sType) {
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
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PUSH_DESCRIPTOR_PROPERTIES: {
         VkPhysicalDevicePushDescriptorProperties *properties = (void *)ext;
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
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MAINTENANCE_5_PROPERTIES: {
         VkPhysicalDeviceMaintenance5Properties *properties = (void *)ext;
         properties->earlyFragmentMultisampleCoverageAfterSampleCounting = pdevice->properties.earlyFragmentMultisampleCoverageAfterSampleCounting;
         properties->earlyFragmentSampleMaskTestBeforeSampleCounting = pdevice->properties.earlyFragmentSampleMaskTestBeforeSampleCounting;
         properties->depthStencilSwizzleOneSupport = pdevice->properties.depthStencilSwizzleOneSupport;
         properties->polygonModePointSize = pdevice->properties.polygonModePointSize;
         properties->nonStrictSinglePixelWideLinesUseParallelogram = pdevice->properties.nonStrictSinglePixelWideLinesUseParallelogram;
         properties->nonStrictWideLinesUseParallelogram = pdevice->properties.nonStrictWideLinesUseParallelogram;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MAINTENANCE_6_PROPERTIES: {
         VkPhysicalDeviceMaintenance6Properties *properties = (void *)ext;
         properties->blockTexelViewCompatibleMultipleLayers = pdevice->properties.blockTexelViewCompatibleMultipleLayers;
         properties->maxCombinedImageSamplerDescriptorCount = pdevice->properties.maxCombinedImageSamplerDescriptorCount;
         properties->fragmentShadingRateClampCombinerInputs = pdevice->properties.fragmentShadingRateClampCombinerInputs;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MAINTENANCE_7_PROPERTIES_KHR: {
         VkPhysicalDeviceMaintenance7PropertiesKHR *properties = (void *)ext;
         properties->robustFragmentShadingRateAttachmentAccess = pdevice->properties.robustFragmentShadingRateAttachmentAccess;
         properties->separateDepthStencilAttachmentAccess = pdevice->properties.separateDepthStencilAttachmentAccess;
         properties->maxDescriptorSetTotalUniformBuffersDynamic = pdevice->properties.maxDescriptorSetTotalUniformBuffersDynamic;
         properties->maxDescriptorSetTotalStorageBuffersDynamic = pdevice->properties.maxDescriptorSetTotalStorageBuffersDynamic;
         properties->maxDescriptorSetTotalBuffersDynamic = pdevice->properties.maxDescriptorSetTotalBuffersDynamic;
         properties->maxDescriptorSetUpdateAfterBindTotalUniformBuffersDynamic = pdevice->properties.maxDescriptorSetUpdateAfterBindTotalUniformBuffersDynamic;
         properties->maxDescriptorSetUpdateAfterBindTotalStorageBuffersDynamic = pdevice->properties.maxDescriptorSetUpdateAfterBindTotalStorageBuffersDynamic;
         properties->maxDescriptorSetUpdateAfterBindTotalBuffersDynamic = pdevice->properties.maxDescriptorSetUpdateAfterBindTotalBuffersDynamic;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_LAYERED_API_PROPERTIES_LIST_KHR: {
         VkPhysicalDeviceLayeredApiPropertiesListKHR *properties = (void *)ext;
         
         
    if (properties->pLayeredApis != NULL) {
        uint32_t count = MIN2(properties->layeredApiCount, pdevice->properties.layeredApiCount);
        for (uint32_t i = 0; i < count; i++)
            properties->pLayeredApis[i] = pdevice->properties.pLayeredApis[i];
        properties->layeredApiCount = count;
    } else {
        properties->layeredApiCount = pdevice->properties.layeredApiCount;
    }

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
#if DETECT_OS_ANDROID
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PRESENTATION_PROPERTIES_ANDROID: {
         VkPhysicalDevicePresentationPropertiesANDROID *properties = (void *)ext;
         properties->sharedImage = pdevice->properties.sharedImage;
         break;
      }
#endif /* DETECT_OS_ANDROID */
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
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VERTEX_ATTRIBUTE_DIVISOR_PROPERTIES: {
         VkPhysicalDeviceVertexAttributeDivisorProperties *properties = (void *)ext;
         properties->maxVertexAttribDivisor = pdevice->properties.maxVertexAttribDivisor;
         properties->supportsNonZeroFirstInstance = pdevice->properties.supportsNonZeroFirstInstance;
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
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_COMPUTE_SHADER_DERIVATIVES_PROPERTIES_KHR: {
         VkPhysicalDeviceComputeShaderDerivativesPropertiesKHR *properties = (void *)ext;
         properties->meshAndTaskShaderDerivatives = pdevice->properties.meshAndTaskShaderDerivatives;
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
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_LINE_RASTERIZATION_PROPERTIES: {
         VkPhysicalDeviceLineRasterizationProperties *properties = (void *)ext;
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
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_4_PROPERTIES: {
         VkPhysicalDeviceVulkan14Properties *properties = (void *)ext;
         properties->lineSubPixelPrecisionBits = pdevice->properties.lineSubPixelPrecisionBits;
         properties->maxVertexAttribDivisor = pdevice->properties.maxVertexAttribDivisor;
         properties->supportsNonZeroFirstInstance = pdevice->properties.supportsNonZeroFirstInstance;
         properties->maxPushDescriptors = pdevice->properties.maxPushDescriptors;
         properties->dynamicRenderingLocalReadDepthStencilAttachments = pdevice->properties.dynamicRenderingLocalReadDepthStencilAttachments;
         properties->dynamicRenderingLocalReadMultisampledAttachments = pdevice->properties.dynamicRenderingLocalReadMultisampledAttachments;
         properties->earlyFragmentMultisampleCoverageAfterSampleCounting = pdevice->properties.earlyFragmentMultisampleCoverageAfterSampleCounting;
         properties->earlyFragmentSampleMaskTestBeforeSampleCounting = pdevice->properties.earlyFragmentSampleMaskTestBeforeSampleCounting;
         properties->depthStencilSwizzleOneSupport = pdevice->properties.depthStencilSwizzleOneSupport;
         properties->polygonModePointSize = pdevice->properties.polygonModePointSize;
         properties->nonStrictSinglePixelWideLinesUseParallelogram = pdevice->properties.nonStrictSinglePixelWideLinesUseParallelogram;
         properties->nonStrictWideLinesUseParallelogram = pdevice->properties.nonStrictWideLinesUseParallelogram;
         properties->blockTexelViewCompatibleMultipleLayers = pdevice->properties.blockTexelViewCompatibleMultipleLayers;
         properties->maxCombinedImageSamplerDescriptorCount = pdevice->properties.maxCombinedImageSamplerDescriptorCount;
         properties->fragmentShadingRateClampCombinerInputs = pdevice->properties.fragmentShadingRateClampCombinerInputs;
         properties->defaultRobustnessStorageBuffers = pdevice->properties.defaultRobustnessStorageBuffers;
         properties->defaultRobustnessUniformBuffers = pdevice->properties.defaultRobustnessUniformBuffers;
         properties->defaultRobustnessVertexInputs = pdevice->properties.defaultRobustnessVertexInputs;
         properties->defaultRobustnessImages = pdevice->properties.defaultRobustnessImages;
         
         
    if (properties->pCopySrcLayouts != NULL) {
        uint32_t count = MIN2(properties->copySrcLayoutCount, pdevice->properties.copySrcLayoutCount);
        for (uint32_t i = 0; i < count; i++)
            properties->pCopySrcLayouts[i] = pdevice->properties.pCopySrcLayouts[i];
        properties->copySrcLayoutCount = count;
    } else {
        properties->copySrcLayoutCount = pdevice->properties.copySrcLayoutCount;
    }

         
         
    if (properties->pCopyDstLayouts != NULL) {
        uint32_t count = MIN2(properties->copyDstLayoutCount, pdevice->properties.copyDstLayoutCount);
        for (uint32_t i = 0; i < count; i++)
            properties->pCopyDstLayouts[i] = pdevice->properties.pCopyDstLayouts[i];
        properties->copyDstLayoutCount = count;
    } else {
        properties->copyDstLayoutCount = pdevice->properties.copyDstLayoutCount;
    }

         memcpy(properties->optimalTilingLayoutUUID, pdevice->properties.optimalTilingLayoutUUID, sizeof(properties->optimalTilingLayoutUUID));
         properties->identicalMemoryTypeRequirements = pdevice->properties.identicalMemoryTypeRequirements;
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
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_LEGACY_VERTEX_ATTRIBUTES_PROPERTIES_EXT: {
         VkPhysicalDeviceLegacyVertexAttributesPropertiesEXT *properties = (void *)ext;
         properties->nativeUnalignedPerformance = pdevice->properties.nativeUnalignedPerformance;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DEVICE_GENERATED_COMMANDS_PROPERTIES_EXT: {
         VkPhysicalDeviceDeviceGeneratedCommandsPropertiesEXT *properties = (void *)ext;
         properties->maxIndirectPipelineCount = pdevice->properties.maxIndirectPipelineCount;
         properties->maxIndirectShaderObjectCount = pdevice->properties.maxIndirectShaderObjectCount;
         properties->maxIndirectSequenceCount = pdevice->properties.maxIndirectSequenceCount;
         properties->maxIndirectCommandsTokenCount = pdevice->properties.maxIndirectCommandsTokenCount;
         properties->maxIndirectCommandsTokenOffset = pdevice->properties.maxIndirectCommandsTokenOffset;
         properties->maxIndirectCommandsIndirectStride = pdevice->properties.maxIndirectCommandsIndirectStride;
         properties->supportedIndirectCommandsInputModes = pdevice->properties.supportedIndirectCommandsInputModes;
         properties->supportedIndirectCommandsShaderStages = pdevice->properties.supportedIndirectCommandsShaderStages;
         properties->supportedIndirectCommandsShaderStagesPipelineBinding = pdevice->properties.supportedIndirectCommandsShaderStagesPipelineBinding;
         properties->supportedIndirectCommandsShaderStagesShaderBinding = pdevice->properties.supportedIndirectCommandsShaderStagesShaderBinding;
         properties->deviceGeneratedCommandsTransformFeedback = pdevice->properties.deviceGeneratedCommandsTransformFeedback;
         properties->deviceGeneratedCommandsMultiDrawIndirectCount = pdevice->properties.deviceGeneratedCommandsMultiDrawIndirectCount;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_HOST_IMAGE_COPY_PROPERTIES: {
         VkPhysicalDeviceHostImageCopyProperties *properties = (void *)ext;
         
         
    if (properties->pCopySrcLayouts != NULL) {
        uint32_t count = MIN2(properties->copySrcLayoutCount, pdevice->properties.copySrcLayoutCount);
        for (uint32_t i = 0; i < count; i++)
            properties->pCopySrcLayouts[i] = pdevice->properties.pCopySrcLayouts[i];
        properties->copySrcLayoutCount = count;
    } else {
        properties->copySrcLayoutCount = pdevice->properties.copySrcLayoutCount;
    }

         
         
    if (properties->pCopyDstLayouts != NULL) {
        uint32_t count = MIN2(properties->copyDstLayoutCount, pdevice->properties.copyDstLayoutCount);
        for (uint32_t i = 0; i < count; i++)
            properties->pCopyDstLayouts[i] = pdevice->properties.pCopyDstLayouts[i];
        properties->copyDstLayoutCount = count;
    } else {
        properties->copyDstLayoutCount = pdevice->properties.copyDstLayoutCount;
    }

         memcpy(properties->optimalTilingLayoutUUID, pdevice->properties.optimalTilingLayoutUUID, sizeof(properties->optimalTilingLayoutUUID));
         properties->identicalMemoryTypeRequirements = pdevice->properties.identicalMemoryTypeRequirements;
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
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PIPELINE_BINARY_PROPERTIES_KHR: {
         VkPhysicalDevicePipelineBinaryPropertiesKHR *properties = (void *)ext;
         properties->pipelineBinaryInternalCache = pdevice->properties.pipelineBinaryInternalCache;
         properties->pipelineBinaryInternalCacheControl = pdevice->properties.pipelineBinaryInternalCacheControl;
         properties->pipelineBinaryPrefersInternalCache = pdevice->properties.pipelineBinaryPrefersInternalCache;
         properties->pipelineBinaryPrecompiledInternalCache = pdevice->properties.pipelineBinaryPrecompiledInternalCache;
         properties->pipelineBinaryCompressedData = pdevice->properties.pipelineBinaryCompressedData;
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
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PIPELINE_ROBUSTNESS_PROPERTIES: {
         VkPhysicalDevicePipelineRobustnessProperties *properties = (void *)ext;
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
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SCHEDULING_CONTROLS_PROPERTIES_ARM: {
         VkPhysicalDeviceSchedulingControlsPropertiesARM *properties = (void *)ext;
         properties->schedulingControlsFlags = pdevice->properties.schedulingControlsFlags;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RENDER_PASS_STRIPED_PROPERTIES_ARM: {
         VkPhysicalDeviceRenderPassStripedPropertiesARM *properties = (void *)ext;
         properties->renderPassStripeGranularity = pdevice->properties.renderPassStripeGranularity;
         properties->maxRenderPassStripes = pdevice->properties.maxRenderPassStripes;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MAP_MEMORY_PLACED_PROPERTIES_EXT: {
         VkPhysicalDeviceMapMemoryPlacedPropertiesEXT *properties = (void *)ext;
         properties->minPlacedMemoryMapAlignment = pdevice->properties.minPlacedMemoryMapAlignment;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_IMAGE_ALIGNMENT_CONTROL_PROPERTIES_MESA: {
         VkPhysicalDeviceImageAlignmentControlPropertiesMESA *properties = (void *)ext;
         properties->supportedImageAlignmentMask = pdevice->properties.supportedImageAlignmentMask;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_COOPERATIVE_MATRIX_2_PROPERTIES_NV: {
         VkPhysicalDeviceCooperativeMatrix2PropertiesNV *properties = (void *)ext;
         properties->cooperativeMatrixWorkgroupScopeMaxWorkgroupSize = pdevice->properties.cooperativeMatrixWorkgroupScopeMaxWorkgroupSize;
         properties->cooperativeMatrixFlexibleDimensionsMaxDimension = pdevice->properties.cooperativeMatrixFlexibleDimensionsMaxDimension;
         properties->cooperativeMatrixWorkgroupScopeReservedSharedMemory = pdevice->properties.cooperativeMatrixWorkgroupScopeReservedSharedMemory;
         break;
      }

      /* Specialized propery handling defined in vk_physical_device_properties_gen.py */

      default:
         break;
      }
   }
}

void
vk_set_physical_device_properties_struct(struct vk_properties *all_properties,
                                         const VkBaseInStructure *pProperties)
{
   switch ((int32_t)pProperties->sType) {
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2: {
         const VkPhysicalDeviceProperties *properties = &((const VkPhysicalDeviceProperties2 *)pProperties)->properties;
         all_properties->apiVersion = properties->apiVersion;
         all_properties->driverVersion = properties->driverVersion;
         all_properties->vendorID = properties->vendorID;
         all_properties->deviceID = properties->deviceID;
         all_properties->deviceType = properties->deviceType;
         memcpy(all_properties->deviceName, properties->deviceName, sizeof(all_properties->deviceName));
         memcpy(all_properties->pipelineCacheUUID, properties->pipelineCacheUUID, sizeof(all_properties->pipelineCacheUUID));
         all_properties->maxImageDimension1D = properties->limits.maxImageDimension1D;
         all_properties->maxImageDimension2D = properties->limits.maxImageDimension2D;
         all_properties->maxImageDimension3D = properties->limits.maxImageDimension3D;
         all_properties->maxImageDimensionCube = properties->limits.maxImageDimensionCube;
         all_properties->maxImageArrayLayers = properties->limits.maxImageArrayLayers;
         all_properties->maxTexelBufferElements = properties->limits.maxTexelBufferElements;
         all_properties->maxUniformBufferRange = properties->limits.maxUniformBufferRange;
         all_properties->maxStorageBufferRange = properties->limits.maxStorageBufferRange;
         all_properties->maxPushConstantsSize = properties->limits.maxPushConstantsSize;
         all_properties->maxMemoryAllocationCount = properties->limits.maxMemoryAllocationCount;
         all_properties->maxSamplerAllocationCount = properties->limits.maxSamplerAllocationCount;
         all_properties->bufferImageGranularity = properties->limits.bufferImageGranularity;
         all_properties->sparseAddressSpaceSize = properties->limits.sparseAddressSpaceSize;
         all_properties->maxBoundDescriptorSets = properties->limits.maxBoundDescriptorSets;
         all_properties->maxPerStageDescriptorSamplers = properties->limits.maxPerStageDescriptorSamplers;
         all_properties->maxPerStageDescriptorUniformBuffers = properties->limits.maxPerStageDescriptorUniformBuffers;
         all_properties->maxPerStageDescriptorStorageBuffers = properties->limits.maxPerStageDescriptorStorageBuffers;
         all_properties->maxPerStageDescriptorSampledImages = properties->limits.maxPerStageDescriptorSampledImages;
         all_properties->maxPerStageDescriptorStorageImages = properties->limits.maxPerStageDescriptorStorageImages;
         all_properties->maxPerStageDescriptorInputAttachments = properties->limits.maxPerStageDescriptorInputAttachments;
         all_properties->maxPerStageResources = properties->limits.maxPerStageResources;
         all_properties->maxDescriptorSetSamplers = properties->limits.maxDescriptorSetSamplers;
         all_properties->maxDescriptorSetUniformBuffers = properties->limits.maxDescriptorSetUniformBuffers;
         all_properties->maxDescriptorSetUniformBuffersDynamic = properties->limits.maxDescriptorSetUniformBuffersDynamic;
         all_properties->maxDescriptorSetStorageBuffers = properties->limits.maxDescriptorSetStorageBuffers;
         all_properties->maxDescriptorSetStorageBuffersDynamic = properties->limits.maxDescriptorSetStorageBuffersDynamic;
         all_properties->maxDescriptorSetSampledImages = properties->limits.maxDescriptorSetSampledImages;
         all_properties->maxDescriptorSetStorageImages = properties->limits.maxDescriptorSetStorageImages;
         all_properties->maxDescriptorSetInputAttachments = properties->limits.maxDescriptorSetInputAttachments;
         all_properties->maxVertexInputAttributes = properties->limits.maxVertexInputAttributes;
         all_properties->maxVertexInputBindings = properties->limits.maxVertexInputBindings;
         all_properties->maxVertexInputAttributeOffset = properties->limits.maxVertexInputAttributeOffset;
         all_properties->maxVertexInputBindingStride = properties->limits.maxVertexInputBindingStride;
         all_properties->maxVertexOutputComponents = properties->limits.maxVertexOutputComponents;
         all_properties->maxTessellationGenerationLevel = properties->limits.maxTessellationGenerationLevel;
         all_properties->maxTessellationPatchSize = properties->limits.maxTessellationPatchSize;
         all_properties->maxTessellationControlPerVertexInputComponents = properties->limits.maxTessellationControlPerVertexInputComponents;
         all_properties->maxTessellationControlPerVertexOutputComponents = properties->limits.maxTessellationControlPerVertexOutputComponents;
         all_properties->maxTessellationControlPerPatchOutputComponents = properties->limits.maxTessellationControlPerPatchOutputComponents;
         all_properties->maxTessellationControlTotalOutputComponents = properties->limits.maxTessellationControlTotalOutputComponents;
         all_properties->maxTessellationEvaluationInputComponents = properties->limits.maxTessellationEvaluationInputComponents;
         all_properties->maxTessellationEvaluationOutputComponents = properties->limits.maxTessellationEvaluationOutputComponents;
         all_properties->maxGeometryShaderInvocations = properties->limits.maxGeometryShaderInvocations;
         all_properties->maxGeometryInputComponents = properties->limits.maxGeometryInputComponents;
         all_properties->maxGeometryOutputComponents = properties->limits.maxGeometryOutputComponents;
         all_properties->maxGeometryOutputVertices = properties->limits.maxGeometryOutputVertices;
         all_properties->maxGeometryTotalOutputComponents = properties->limits.maxGeometryTotalOutputComponents;
         all_properties->maxFragmentInputComponents = properties->limits.maxFragmentInputComponents;
         all_properties->maxFragmentOutputAttachments = properties->limits.maxFragmentOutputAttachments;
         all_properties->maxFragmentDualSrcAttachments = properties->limits.maxFragmentDualSrcAttachments;
         all_properties->maxFragmentCombinedOutputResources = properties->limits.maxFragmentCombinedOutputResources;
         all_properties->maxComputeSharedMemorySize = properties->limits.maxComputeSharedMemorySize;
         memcpy(all_properties->maxComputeWorkGroupCount, properties->limits.maxComputeWorkGroupCount, sizeof(all_properties->maxComputeWorkGroupCount));
         all_properties->maxComputeWorkGroupInvocations = properties->limits.maxComputeWorkGroupInvocations;
         memcpy(all_properties->maxComputeWorkGroupSize, properties->limits.maxComputeWorkGroupSize, sizeof(all_properties->maxComputeWorkGroupSize));
         all_properties->subPixelPrecisionBits = properties->limits.subPixelPrecisionBits;
         all_properties->subTexelPrecisionBits = properties->limits.subTexelPrecisionBits;
         all_properties->mipmapPrecisionBits = properties->limits.mipmapPrecisionBits;
         all_properties->maxDrawIndexedIndexValue = properties->limits.maxDrawIndexedIndexValue;
         all_properties->maxDrawIndirectCount = properties->limits.maxDrawIndirectCount;
         all_properties->maxSamplerLodBias = properties->limits.maxSamplerLodBias;
         all_properties->maxSamplerAnisotropy = properties->limits.maxSamplerAnisotropy;
         all_properties->maxViewports = properties->limits.maxViewports;
         memcpy(all_properties->maxViewportDimensions, properties->limits.maxViewportDimensions, sizeof(all_properties->maxViewportDimensions));
         memcpy(all_properties->viewportBoundsRange, properties->limits.viewportBoundsRange, sizeof(all_properties->viewportBoundsRange));
         all_properties->viewportSubPixelBits = properties->limits.viewportSubPixelBits;
         all_properties->minMemoryMapAlignment = properties->limits.minMemoryMapAlignment;
         all_properties->minTexelBufferOffsetAlignment = properties->limits.minTexelBufferOffsetAlignment;
         all_properties->minUniformBufferOffsetAlignment = properties->limits.minUniformBufferOffsetAlignment;
         all_properties->minStorageBufferOffsetAlignment = properties->limits.minStorageBufferOffsetAlignment;
         all_properties->minTexelOffset = properties->limits.minTexelOffset;
         all_properties->maxTexelOffset = properties->limits.maxTexelOffset;
         all_properties->minTexelGatherOffset = properties->limits.minTexelGatherOffset;
         all_properties->maxTexelGatherOffset = properties->limits.maxTexelGatherOffset;
         all_properties->minInterpolationOffset = properties->limits.minInterpolationOffset;
         all_properties->maxInterpolationOffset = properties->limits.maxInterpolationOffset;
         all_properties->subPixelInterpolationOffsetBits = properties->limits.subPixelInterpolationOffsetBits;
         all_properties->maxFramebufferWidth = properties->limits.maxFramebufferWidth;
         all_properties->maxFramebufferHeight = properties->limits.maxFramebufferHeight;
         all_properties->maxFramebufferLayers = properties->limits.maxFramebufferLayers;
         all_properties->framebufferColorSampleCounts = properties->limits.framebufferColorSampleCounts;
         all_properties->framebufferDepthSampleCounts = properties->limits.framebufferDepthSampleCounts;
         all_properties->framebufferStencilSampleCounts = properties->limits.framebufferStencilSampleCounts;
         all_properties->framebufferNoAttachmentsSampleCounts = properties->limits.framebufferNoAttachmentsSampleCounts;
         all_properties->maxColorAttachments = properties->limits.maxColorAttachments;
         all_properties->sampledImageColorSampleCounts = properties->limits.sampledImageColorSampleCounts;
         all_properties->sampledImageIntegerSampleCounts = properties->limits.sampledImageIntegerSampleCounts;
         all_properties->sampledImageDepthSampleCounts = properties->limits.sampledImageDepthSampleCounts;
         all_properties->sampledImageStencilSampleCounts = properties->limits.sampledImageStencilSampleCounts;
         all_properties->storageImageSampleCounts = properties->limits.storageImageSampleCounts;
         all_properties->maxSampleMaskWords = properties->limits.maxSampleMaskWords;
         all_properties->timestampComputeAndGraphics = properties->limits.timestampComputeAndGraphics;
         all_properties->timestampPeriod = properties->limits.timestampPeriod;
         all_properties->maxClipDistances = properties->limits.maxClipDistances;
         all_properties->maxCullDistances = properties->limits.maxCullDistances;
         all_properties->maxCombinedClipAndCullDistances = properties->limits.maxCombinedClipAndCullDistances;
         all_properties->discreteQueuePriorities = properties->limits.discreteQueuePriorities;
         memcpy(all_properties->pointSizeRange, properties->limits.pointSizeRange, sizeof(all_properties->pointSizeRange));
         memcpy(all_properties->lineWidthRange, properties->limits.lineWidthRange, sizeof(all_properties->lineWidthRange));
         all_properties->pointSizeGranularity = properties->limits.pointSizeGranularity;
         all_properties->lineWidthGranularity = properties->limits.lineWidthGranularity;
         all_properties->strictLines = properties->limits.strictLines;
         all_properties->standardSampleLocations = properties->limits.standardSampleLocations;
         all_properties->optimalBufferCopyOffsetAlignment = properties->limits.optimalBufferCopyOffsetAlignment;
         all_properties->optimalBufferCopyRowPitchAlignment = properties->limits.optimalBufferCopyRowPitchAlignment;
         all_properties->nonCoherentAtomSize = properties->limits.nonCoherentAtomSize;
         all_properties->sparseResidencyStandard2DBlockShape = properties->sparseProperties.residencyStandard2DBlockShape;
         all_properties->sparseResidencyStandard2DMultisampleBlockShape = properties->sparseProperties.residencyStandard2DMultisampleBlockShape;
         all_properties->sparseResidencyStandard3DBlockShape = properties->sparseProperties.residencyStandard3DBlockShape;
         all_properties->sparseResidencyAlignedMipSize = properties->sparseProperties.residencyAlignedMipSize;
         all_properties->sparseResidencyNonResidentStrict = properties->sparseProperties.residencyNonResidentStrict;
         break;
      }

      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DEVICE_GENERATED_COMMANDS_PROPERTIES_NV: {
         const VkPhysicalDeviceDeviceGeneratedCommandsPropertiesNV *properties = (const VkPhysicalDeviceDeviceGeneratedCommandsPropertiesNV *)pProperties;
         all_properties->maxGraphicsShaderGroupCount = properties->maxGraphicsShaderGroupCount;
         all_properties->maxIndirectSequenceCount = properties->maxIndirectSequenceCount;
         all_properties->maxIndirectCommandsTokenCount = properties->maxIndirectCommandsTokenCount;
         all_properties->maxIndirectCommandsStreamCount = properties->maxIndirectCommandsStreamCount;
         all_properties->maxIndirectCommandsTokenOffset = properties->maxIndirectCommandsTokenOffset;
         all_properties->maxIndirectCommandsStreamStride = properties->maxIndirectCommandsStreamStride;
         all_properties->minSequencesCountBufferOffsetAlignment = properties->minSequencesCountBufferOffsetAlignment;
         all_properties->minSequencesIndexBufferOffsetAlignment = properties->minSequencesIndexBufferOffsetAlignment;
         all_properties->minIndirectCommandsBufferOffsetAlignment = properties->minIndirectCommandsBufferOffsetAlignment;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MULTI_DRAW_PROPERTIES_EXT: {
         const VkPhysicalDeviceMultiDrawPropertiesEXT *properties = (const VkPhysicalDeviceMultiDrawPropertiesEXT *)pProperties;
         all_properties->maxMultiDrawCount = properties->maxMultiDrawCount;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PUSH_DESCRIPTOR_PROPERTIES: {
         const VkPhysicalDevicePushDescriptorProperties *properties = (const VkPhysicalDevicePushDescriptorProperties *)pProperties;
         all_properties->maxPushDescriptors = properties->maxPushDescriptors;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DRIVER_PROPERTIES: {
         const VkPhysicalDeviceDriverProperties *properties = (const VkPhysicalDeviceDriverProperties *)pProperties;
         all_properties->driverID = properties->driverID;
         memcpy(all_properties->driverName, properties->driverName, sizeof(all_properties->driverName));
         memcpy(all_properties->driverInfo, properties->driverInfo, sizeof(all_properties->driverInfo));
         all_properties->conformanceVersion = properties->conformanceVersion;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ID_PROPERTIES: {
         const VkPhysicalDeviceIDProperties *properties = (const VkPhysicalDeviceIDProperties *)pProperties;
         memcpy(all_properties->deviceUUID, properties->deviceUUID, sizeof(all_properties->deviceUUID));
         memcpy(all_properties->driverUUID, properties->driverUUID, sizeof(all_properties->driverUUID));
         memcpy(all_properties->deviceLUID, properties->deviceLUID, sizeof(all_properties->deviceLUID));
         all_properties->deviceNodeMask = properties->deviceNodeMask;
         all_properties->deviceLUIDValid = properties->deviceLUIDValid;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MULTIVIEW_PROPERTIES: {
         const VkPhysicalDeviceMultiviewProperties *properties = (const VkPhysicalDeviceMultiviewProperties *)pProperties;
         all_properties->maxMultiviewViewCount = properties->maxMultiviewViewCount;
         all_properties->maxMultiviewInstanceIndex = properties->maxMultiviewInstanceIndex;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DISCARD_RECTANGLE_PROPERTIES_EXT: {
         const VkPhysicalDeviceDiscardRectanglePropertiesEXT *properties = (const VkPhysicalDeviceDiscardRectanglePropertiesEXT *)pProperties;
         all_properties->maxDiscardRectangles = properties->maxDiscardRectangles;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MULTIVIEW_PER_VIEW_ATTRIBUTES_PROPERTIES_NVX: {
         const VkPhysicalDeviceMultiviewPerViewAttributesPropertiesNVX *properties = (const VkPhysicalDeviceMultiviewPerViewAttributesPropertiesNVX *)pProperties;
         all_properties->perViewPositionAllComponents = properties->perViewPositionAllComponents;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SUBGROUP_PROPERTIES: {
         const VkPhysicalDeviceSubgroupProperties *properties = (const VkPhysicalDeviceSubgroupProperties *)pProperties;
         all_properties->subgroupSize = properties->subgroupSize;
         all_properties->subgroupSupportedStages = properties->supportedStages;
         all_properties->subgroupSupportedOperations = properties->supportedOperations;
         all_properties->subgroupQuadOperationsInAllStages = properties->quadOperationsInAllStages;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_POINT_CLIPPING_PROPERTIES: {
         const VkPhysicalDevicePointClippingProperties *properties = (const VkPhysicalDevicePointClippingProperties *)pProperties;
         all_properties->pointClippingBehavior = properties->pointClippingBehavior;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROTECTED_MEMORY_PROPERTIES: {
         const VkPhysicalDeviceProtectedMemoryProperties *properties = (const VkPhysicalDeviceProtectedMemoryProperties *)pProperties;
         all_properties->protectedNoFault = properties->protectedNoFault;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SAMPLER_FILTER_MINMAX_PROPERTIES: {
         const VkPhysicalDeviceSamplerFilterMinmaxProperties *properties = (const VkPhysicalDeviceSamplerFilterMinmaxProperties *)pProperties;
         all_properties->filterMinmaxSingleComponentFormats = properties->filterMinmaxSingleComponentFormats;
         all_properties->filterMinmaxImageComponentMapping = properties->filterMinmaxImageComponentMapping;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SAMPLE_LOCATIONS_PROPERTIES_EXT: {
         const VkPhysicalDeviceSampleLocationsPropertiesEXT *properties = (const VkPhysicalDeviceSampleLocationsPropertiesEXT *)pProperties;
         all_properties->sampleLocationSampleCounts = properties->sampleLocationSampleCounts;
         all_properties->maxSampleLocationGridSize = properties->maxSampleLocationGridSize;
         memcpy(all_properties->sampleLocationCoordinateRange, properties->sampleLocationCoordinateRange, sizeof(all_properties->sampleLocationCoordinateRange));
         all_properties->sampleLocationSubPixelBits = properties->sampleLocationSubPixelBits;
         all_properties->variableSampleLocations = properties->variableSampleLocations;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_BLEND_OPERATION_ADVANCED_PROPERTIES_EXT: {
         const VkPhysicalDeviceBlendOperationAdvancedPropertiesEXT *properties = (const VkPhysicalDeviceBlendOperationAdvancedPropertiesEXT *)pProperties;
         all_properties->advancedBlendMaxColorAttachments = properties->advancedBlendMaxColorAttachments;
         all_properties->advancedBlendIndependentBlend = properties->advancedBlendIndependentBlend;
         all_properties->advancedBlendNonPremultipliedSrcColor = properties->advancedBlendNonPremultipliedSrcColor;
         all_properties->advancedBlendNonPremultipliedDstColor = properties->advancedBlendNonPremultipliedDstColor;
         all_properties->advancedBlendCorrelatedOverlap = properties->advancedBlendCorrelatedOverlap;
         all_properties->advancedBlendAllOperations = properties->advancedBlendAllOperations;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_INLINE_UNIFORM_BLOCK_PROPERTIES: {
         const VkPhysicalDeviceInlineUniformBlockProperties *properties = (const VkPhysicalDeviceInlineUniformBlockProperties *)pProperties;
         all_properties->maxInlineUniformBlockSize = properties->maxInlineUniformBlockSize;
         all_properties->maxPerStageDescriptorInlineUniformBlocks = properties->maxPerStageDescriptorInlineUniformBlocks;
         all_properties->maxPerStageDescriptorUpdateAfterBindInlineUniformBlocks = properties->maxPerStageDescriptorUpdateAfterBindInlineUniformBlocks;
         all_properties->maxDescriptorSetInlineUniformBlocks = properties->maxDescriptorSetInlineUniformBlocks;
         all_properties->maxDescriptorSetUpdateAfterBindInlineUniformBlocks = properties->maxDescriptorSetUpdateAfterBindInlineUniformBlocks;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MAINTENANCE_3_PROPERTIES: {
         const VkPhysicalDeviceMaintenance3Properties *properties = (const VkPhysicalDeviceMaintenance3Properties *)pProperties;
         all_properties->maxPerSetDescriptors = properties->maxPerSetDescriptors;
         all_properties->maxMemoryAllocationSize = properties->maxMemoryAllocationSize;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MAINTENANCE_4_PROPERTIES: {
         const VkPhysicalDeviceMaintenance4Properties *properties = (const VkPhysicalDeviceMaintenance4Properties *)pProperties;
         all_properties->maxBufferSize = properties->maxBufferSize;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MAINTENANCE_5_PROPERTIES: {
         const VkPhysicalDeviceMaintenance5Properties *properties = (const VkPhysicalDeviceMaintenance5Properties *)pProperties;
         all_properties->earlyFragmentMultisampleCoverageAfterSampleCounting = properties->earlyFragmentMultisampleCoverageAfterSampleCounting;
         all_properties->earlyFragmentSampleMaskTestBeforeSampleCounting = properties->earlyFragmentSampleMaskTestBeforeSampleCounting;
         all_properties->depthStencilSwizzleOneSupport = properties->depthStencilSwizzleOneSupport;
         all_properties->polygonModePointSize = properties->polygonModePointSize;
         all_properties->nonStrictSinglePixelWideLinesUseParallelogram = properties->nonStrictSinglePixelWideLinesUseParallelogram;
         all_properties->nonStrictWideLinesUseParallelogram = properties->nonStrictWideLinesUseParallelogram;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MAINTENANCE_6_PROPERTIES: {
         const VkPhysicalDeviceMaintenance6Properties *properties = (const VkPhysicalDeviceMaintenance6Properties *)pProperties;
         all_properties->blockTexelViewCompatibleMultipleLayers = properties->blockTexelViewCompatibleMultipleLayers;
         all_properties->maxCombinedImageSamplerDescriptorCount = properties->maxCombinedImageSamplerDescriptorCount;
         all_properties->fragmentShadingRateClampCombinerInputs = properties->fragmentShadingRateClampCombinerInputs;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MAINTENANCE_7_PROPERTIES_KHR: {
         const VkPhysicalDeviceMaintenance7PropertiesKHR *properties = (const VkPhysicalDeviceMaintenance7PropertiesKHR *)pProperties;
         all_properties->robustFragmentShadingRateAttachmentAccess = properties->robustFragmentShadingRateAttachmentAccess;
         all_properties->separateDepthStencilAttachmentAccess = properties->separateDepthStencilAttachmentAccess;
         all_properties->maxDescriptorSetTotalUniformBuffersDynamic = properties->maxDescriptorSetTotalUniformBuffersDynamic;
         all_properties->maxDescriptorSetTotalStorageBuffersDynamic = properties->maxDescriptorSetTotalStorageBuffersDynamic;
         all_properties->maxDescriptorSetTotalBuffersDynamic = properties->maxDescriptorSetTotalBuffersDynamic;
         all_properties->maxDescriptorSetUpdateAfterBindTotalUniformBuffersDynamic = properties->maxDescriptorSetUpdateAfterBindTotalUniformBuffersDynamic;
         all_properties->maxDescriptorSetUpdateAfterBindTotalStorageBuffersDynamic = properties->maxDescriptorSetUpdateAfterBindTotalStorageBuffersDynamic;
         all_properties->maxDescriptorSetUpdateAfterBindTotalBuffersDynamic = properties->maxDescriptorSetUpdateAfterBindTotalBuffersDynamic;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_LAYERED_API_PROPERTIES_LIST_KHR: {
         const VkPhysicalDeviceLayeredApiPropertiesListKHR *properties = (const VkPhysicalDeviceLayeredApiPropertiesListKHR *)pProperties;
         
         
    if (all_properties->pLayeredApis != NULL) {
        uint32_t count = MIN2(all_properties->layeredApiCount, properties->layeredApiCount);
        for (uint32_t i = 0; i < count; i++)
            all_properties->pLayeredApis[i] = properties->pLayeredApis[i];
        all_properties->layeredApiCount = count;
    } else {
        all_properties->layeredApiCount = properties->layeredApiCount;
    }

         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FLOAT_CONTROLS_PROPERTIES: {
         const VkPhysicalDeviceFloatControlsProperties *properties = (const VkPhysicalDeviceFloatControlsProperties *)pProperties;
         all_properties->denormBehaviorIndependence = properties->denormBehaviorIndependence;
         all_properties->roundingModeIndependence = properties->roundingModeIndependence;
         all_properties->shaderSignedZeroInfNanPreserveFloat16 = properties->shaderSignedZeroInfNanPreserveFloat16;
         all_properties->shaderSignedZeroInfNanPreserveFloat32 = properties->shaderSignedZeroInfNanPreserveFloat32;
         all_properties->shaderSignedZeroInfNanPreserveFloat64 = properties->shaderSignedZeroInfNanPreserveFloat64;
         all_properties->shaderDenormPreserveFloat16 = properties->shaderDenormPreserveFloat16;
         all_properties->shaderDenormPreserveFloat32 = properties->shaderDenormPreserveFloat32;
         all_properties->shaderDenormPreserveFloat64 = properties->shaderDenormPreserveFloat64;
         all_properties->shaderDenormFlushToZeroFloat16 = properties->shaderDenormFlushToZeroFloat16;
         all_properties->shaderDenormFlushToZeroFloat32 = properties->shaderDenormFlushToZeroFloat32;
         all_properties->shaderDenormFlushToZeroFloat64 = properties->shaderDenormFlushToZeroFloat64;
         all_properties->shaderRoundingModeRTEFloat16 = properties->shaderRoundingModeRTEFloat16;
         all_properties->shaderRoundingModeRTEFloat32 = properties->shaderRoundingModeRTEFloat32;
         all_properties->shaderRoundingModeRTEFloat64 = properties->shaderRoundingModeRTEFloat64;
         all_properties->shaderRoundingModeRTZFloat16 = properties->shaderRoundingModeRTZFloat16;
         all_properties->shaderRoundingModeRTZFloat32 = properties->shaderRoundingModeRTZFloat32;
         all_properties->shaderRoundingModeRTZFloat64 = properties->shaderRoundingModeRTZFloat64;
         break;
      }
#if DETECT_OS_ANDROID
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PRESENTATION_PROPERTIES_ANDROID: {
         const VkPhysicalDevicePresentationPropertiesANDROID *properties = (const VkPhysicalDevicePresentationPropertiesANDROID *)pProperties;
         all_properties->sharedImage = properties->sharedImage;
         break;
      }
#endif /* DETECT_OS_ANDROID */
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTERNAL_MEMORY_HOST_PROPERTIES_EXT: {
         const VkPhysicalDeviceExternalMemoryHostPropertiesEXT *properties = (const VkPhysicalDeviceExternalMemoryHostPropertiesEXT *)pProperties;
         all_properties->minImportedHostPointerAlignment = properties->minImportedHostPointerAlignment;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_CONSERVATIVE_RASTERIZATION_PROPERTIES_EXT: {
         const VkPhysicalDeviceConservativeRasterizationPropertiesEXT *properties = (const VkPhysicalDeviceConservativeRasterizationPropertiesEXT *)pProperties;
         all_properties->primitiveOverestimationSize = properties->primitiveOverestimationSize;
         all_properties->maxExtraPrimitiveOverestimationSize = properties->maxExtraPrimitiveOverestimationSize;
         all_properties->extraPrimitiveOverestimationSizeGranularity = properties->extraPrimitiveOverestimationSizeGranularity;
         all_properties->primitiveUnderestimation = properties->primitiveUnderestimation;
         all_properties->conservativePointAndLineRasterization = properties->conservativePointAndLineRasterization;
         all_properties->degenerateTrianglesRasterized = properties->degenerateTrianglesRasterized;
         all_properties->degenerateLinesRasterized = properties->degenerateLinesRasterized;
         all_properties->fullyCoveredFragmentShaderInputVariable = properties->fullyCoveredFragmentShaderInputVariable;
         all_properties->conservativeRasterizationPostDepthCoverage = properties->conservativeRasterizationPostDepthCoverage;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_CORE_PROPERTIES_AMD: {
         const VkPhysicalDeviceShaderCorePropertiesAMD *properties = (const VkPhysicalDeviceShaderCorePropertiesAMD *)pProperties;
         all_properties->shaderEngineCount = properties->shaderEngineCount;
         all_properties->shaderArraysPerEngineCount = properties->shaderArraysPerEngineCount;
         all_properties->computeUnitsPerShaderArray = properties->computeUnitsPerShaderArray;
         all_properties->simdPerComputeUnit = properties->simdPerComputeUnit;
         all_properties->wavefrontsPerSimd = properties->wavefrontsPerSimd;
         all_properties->wavefrontSize = properties->wavefrontSize;
         all_properties->sgprsPerSimd = properties->sgprsPerSimd;
         all_properties->minSgprAllocation = properties->minSgprAllocation;
         all_properties->maxSgprAllocation = properties->maxSgprAllocation;
         all_properties->sgprAllocationGranularity = properties->sgprAllocationGranularity;
         all_properties->vgprsPerSimd = properties->vgprsPerSimd;
         all_properties->minVgprAllocation = properties->minVgprAllocation;
         all_properties->maxVgprAllocation = properties->maxVgprAllocation;
         all_properties->vgprAllocationGranularity = properties->vgprAllocationGranularity;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_CORE_PROPERTIES_2_AMD: {
         const VkPhysicalDeviceShaderCoreProperties2AMD *properties = (const VkPhysicalDeviceShaderCoreProperties2AMD *)pProperties;
         all_properties->shaderCoreFeatures = properties->shaderCoreFeatures;
         all_properties->activeComputeUnitCount = properties->activeComputeUnitCount;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_INDEXING_PROPERTIES: {
         const VkPhysicalDeviceDescriptorIndexingProperties *properties = (const VkPhysicalDeviceDescriptorIndexingProperties *)pProperties;
         all_properties->maxUpdateAfterBindDescriptorsInAllPools = properties->maxUpdateAfterBindDescriptorsInAllPools;
         all_properties->shaderUniformBufferArrayNonUniformIndexingNative = properties->shaderUniformBufferArrayNonUniformIndexingNative;
         all_properties->shaderSampledImageArrayNonUniformIndexingNative = properties->shaderSampledImageArrayNonUniformIndexingNative;
         all_properties->shaderStorageBufferArrayNonUniformIndexingNative = properties->shaderStorageBufferArrayNonUniformIndexingNative;
         all_properties->shaderStorageImageArrayNonUniformIndexingNative = properties->shaderStorageImageArrayNonUniformIndexingNative;
         all_properties->shaderInputAttachmentArrayNonUniformIndexingNative = properties->shaderInputAttachmentArrayNonUniformIndexingNative;
         all_properties->robustBufferAccessUpdateAfterBind = properties->robustBufferAccessUpdateAfterBind;
         all_properties->quadDivergentImplicitLod = properties->quadDivergentImplicitLod;
         all_properties->maxPerStageDescriptorUpdateAfterBindSamplers = properties->maxPerStageDescriptorUpdateAfterBindSamplers;
         all_properties->maxPerStageDescriptorUpdateAfterBindUniformBuffers = properties->maxPerStageDescriptorUpdateAfterBindUniformBuffers;
         all_properties->maxPerStageDescriptorUpdateAfterBindStorageBuffers = properties->maxPerStageDescriptorUpdateAfterBindStorageBuffers;
         all_properties->maxPerStageDescriptorUpdateAfterBindSampledImages = properties->maxPerStageDescriptorUpdateAfterBindSampledImages;
         all_properties->maxPerStageDescriptorUpdateAfterBindStorageImages = properties->maxPerStageDescriptorUpdateAfterBindStorageImages;
         all_properties->maxPerStageDescriptorUpdateAfterBindInputAttachments = properties->maxPerStageDescriptorUpdateAfterBindInputAttachments;
         all_properties->maxPerStageUpdateAfterBindResources = properties->maxPerStageUpdateAfterBindResources;
         all_properties->maxDescriptorSetUpdateAfterBindSamplers = properties->maxDescriptorSetUpdateAfterBindSamplers;
         all_properties->maxDescriptorSetUpdateAfterBindUniformBuffers = properties->maxDescriptorSetUpdateAfterBindUniformBuffers;
         all_properties->maxDescriptorSetUpdateAfterBindUniformBuffersDynamic = properties->maxDescriptorSetUpdateAfterBindUniformBuffersDynamic;
         all_properties->maxDescriptorSetUpdateAfterBindStorageBuffers = properties->maxDescriptorSetUpdateAfterBindStorageBuffers;
         all_properties->maxDescriptorSetUpdateAfterBindStorageBuffersDynamic = properties->maxDescriptorSetUpdateAfterBindStorageBuffersDynamic;
         all_properties->maxDescriptorSetUpdateAfterBindSampledImages = properties->maxDescriptorSetUpdateAfterBindSampledImages;
         all_properties->maxDescriptorSetUpdateAfterBindStorageImages = properties->maxDescriptorSetUpdateAfterBindStorageImages;
         all_properties->maxDescriptorSetUpdateAfterBindInputAttachments = properties->maxDescriptorSetUpdateAfterBindInputAttachments;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TIMELINE_SEMAPHORE_PROPERTIES: {
         const VkPhysicalDeviceTimelineSemaphoreProperties *properties = (const VkPhysicalDeviceTimelineSemaphoreProperties *)pProperties;
         all_properties->maxTimelineSemaphoreValueDifference = properties->maxTimelineSemaphoreValueDifference;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VERTEX_ATTRIBUTE_DIVISOR_PROPERTIES_EXT: {
         const VkPhysicalDeviceVertexAttributeDivisorPropertiesEXT *properties = (const VkPhysicalDeviceVertexAttributeDivisorPropertiesEXT *)pProperties;
         all_properties->maxVertexAttribDivisor = properties->maxVertexAttribDivisor;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VERTEX_ATTRIBUTE_DIVISOR_PROPERTIES: {
         const VkPhysicalDeviceVertexAttributeDivisorProperties *properties = (const VkPhysicalDeviceVertexAttributeDivisorProperties *)pProperties;
         all_properties->maxVertexAttribDivisor = properties->maxVertexAttribDivisor;
         all_properties->supportsNonZeroFirstInstance = properties->supportsNonZeroFirstInstance;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PCI_BUS_INFO_PROPERTIES_EXT: {
         const VkPhysicalDevicePCIBusInfoPropertiesEXT *properties = (const VkPhysicalDevicePCIBusInfoPropertiesEXT *)pProperties;
         all_properties->pciDomain = properties->pciDomain;
         all_properties->pciBus = properties->pciBus;
         all_properties->pciDevice = properties->pciDevice;
         all_properties->pciFunction = properties->pciFunction;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DEPTH_STENCIL_RESOLVE_PROPERTIES: {
         const VkPhysicalDeviceDepthStencilResolveProperties *properties = (const VkPhysicalDeviceDepthStencilResolveProperties *)pProperties;
         all_properties->supportedDepthResolveModes = properties->supportedDepthResolveModes;
         all_properties->supportedStencilResolveModes = properties->supportedStencilResolveModes;
         all_properties->independentResolveNone = properties->independentResolveNone;
         all_properties->independentResolve = properties->independentResolve;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TRANSFORM_FEEDBACK_PROPERTIES_EXT: {
         const VkPhysicalDeviceTransformFeedbackPropertiesEXT *properties = (const VkPhysicalDeviceTransformFeedbackPropertiesEXT *)pProperties;
         all_properties->maxTransformFeedbackStreams = properties->maxTransformFeedbackStreams;
         all_properties->maxTransformFeedbackBuffers = properties->maxTransformFeedbackBuffers;
         all_properties->maxTransformFeedbackBufferSize = properties->maxTransformFeedbackBufferSize;
         all_properties->maxTransformFeedbackStreamDataSize = properties->maxTransformFeedbackStreamDataSize;
         all_properties->maxTransformFeedbackBufferDataSize = properties->maxTransformFeedbackBufferDataSize;
         all_properties->maxTransformFeedbackBufferDataStride = properties->maxTransformFeedbackBufferDataStride;
         all_properties->transformFeedbackQueries = properties->transformFeedbackQueries;
         all_properties->transformFeedbackStreamsLinesTriangles = properties->transformFeedbackStreamsLinesTriangles;
         all_properties->transformFeedbackRasterizationStreamSelect = properties->transformFeedbackRasterizationStreamSelect;
         all_properties->transformFeedbackDraw = properties->transformFeedbackDraw;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_COMPUTE_SHADER_DERIVATIVES_PROPERTIES_KHR: {
         const VkPhysicalDeviceComputeShaderDerivativesPropertiesKHR *properties = (const VkPhysicalDeviceComputeShaderDerivativesPropertiesKHR *)pProperties;
         all_properties->meshAndTaskShaderDerivatives = properties->meshAndTaskShaderDerivatives;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_COPY_MEMORY_INDIRECT_PROPERTIES_NV: {
         const VkPhysicalDeviceCopyMemoryIndirectPropertiesNV *properties = (const VkPhysicalDeviceCopyMemoryIndirectPropertiesNV *)pProperties;
         all_properties->supportedQueues = properties->supportedQueues;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MEMORY_DECOMPRESSION_PROPERTIES_NV: {
         const VkPhysicalDeviceMemoryDecompressionPropertiesNV *properties = (const VkPhysicalDeviceMemoryDecompressionPropertiesNV *)pProperties;
         all_properties->decompressionMethods = properties->decompressionMethods;
         all_properties->maxDecompressionIndirectCount = properties->maxDecompressionIndirectCount;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADING_RATE_IMAGE_PROPERTIES_NV: {
         const VkPhysicalDeviceShadingRateImagePropertiesNV *properties = (const VkPhysicalDeviceShadingRateImagePropertiesNV *)pProperties;
         all_properties->shadingRateTexelSize = properties->shadingRateTexelSize;
         all_properties->shadingRatePaletteSize = properties->shadingRatePaletteSize;
         all_properties->shadingRateMaxCoarseSamples = properties->shadingRateMaxCoarseSamples;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MESH_SHADER_PROPERTIES_NV: {
         const VkPhysicalDeviceMeshShaderPropertiesNV *properties = (const VkPhysicalDeviceMeshShaderPropertiesNV *)pProperties;
         all_properties->maxDrawMeshTasksCount = properties->maxDrawMeshTasksCount;
         all_properties->maxTaskWorkGroupInvocations = properties->maxTaskWorkGroupInvocations;
         memcpy(all_properties->maxTaskWorkGroupSize, properties->maxTaskWorkGroupSize, sizeof(all_properties->maxTaskWorkGroupSize));
         all_properties->maxTaskTotalMemorySize = properties->maxTaskTotalMemorySize;
         all_properties->maxTaskOutputCount = properties->maxTaskOutputCount;
         all_properties->maxMeshWorkGroupInvocations = properties->maxMeshWorkGroupInvocations;
         memcpy(all_properties->maxMeshWorkGroupSize, properties->maxMeshWorkGroupSize, sizeof(all_properties->maxMeshWorkGroupSize));
         all_properties->maxMeshTotalMemorySize = properties->maxMeshTotalMemorySize;
         all_properties->maxMeshOutputVertices = properties->maxMeshOutputVertices;
         all_properties->maxMeshOutputPrimitives = properties->maxMeshOutputPrimitives;
         all_properties->maxMeshMultiviewViewCount = properties->maxMeshMultiviewViewCount;
         all_properties->meshOutputPerVertexGranularity = properties->meshOutputPerVertexGranularity;
         all_properties->meshOutputPerPrimitiveGranularity = properties->meshOutputPerPrimitiveGranularity;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MESH_SHADER_PROPERTIES_EXT: {
         const VkPhysicalDeviceMeshShaderPropertiesEXT *properties = (const VkPhysicalDeviceMeshShaderPropertiesEXT *)pProperties;
         all_properties->maxTaskWorkGroupTotalCount = properties->maxTaskWorkGroupTotalCount;
         memcpy(all_properties->maxTaskWorkGroupCount, properties->maxTaskWorkGroupCount, sizeof(all_properties->maxTaskWorkGroupCount));
         all_properties->maxTaskWorkGroupInvocations = properties->maxTaskWorkGroupInvocations;
         memcpy(all_properties->maxTaskWorkGroupSize, properties->maxTaskWorkGroupSize, sizeof(all_properties->maxTaskWorkGroupSize));
         all_properties->maxTaskPayloadSize = properties->maxTaskPayloadSize;
         all_properties->maxTaskSharedMemorySize = properties->maxTaskSharedMemorySize;
         all_properties->maxTaskPayloadAndSharedMemorySize = properties->maxTaskPayloadAndSharedMemorySize;
         all_properties->maxMeshWorkGroupTotalCount = properties->maxMeshWorkGroupTotalCount;
         memcpy(all_properties->maxMeshWorkGroupCount, properties->maxMeshWorkGroupCount, sizeof(all_properties->maxMeshWorkGroupCount));
         all_properties->maxMeshWorkGroupInvocations = properties->maxMeshWorkGroupInvocations;
         memcpy(all_properties->maxMeshWorkGroupSize, properties->maxMeshWorkGroupSize, sizeof(all_properties->maxMeshWorkGroupSize));
         all_properties->maxMeshSharedMemorySize = properties->maxMeshSharedMemorySize;
         all_properties->maxMeshPayloadAndSharedMemorySize = properties->maxMeshPayloadAndSharedMemorySize;
         all_properties->maxMeshOutputMemorySize = properties->maxMeshOutputMemorySize;
         all_properties->maxMeshPayloadAndOutputMemorySize = properties->maxMeshPayloadAndOutputMemorySize;
         all_properties->maxMeshOutputComponents = properties->maxMeshOutputComponents;
         all_properties->maxMeshOutputVertices = properties->maxMeshOutputVertices;
         all_properties->maxMeshOutputPrimitives = properties->maxMeshOutputPrimitives;
         all_properties->maxMeshOutputLayers = properties->maxMeshOutputLayers;
         all_properties->maxMeshMultiviewViewCount = properties->maxMeshMultiviewViewCount;
         all_properties->meshOutputPerVertexGranularity = properties->meshOutputPerVertexGranularity;
         all_properties->meshOutputPerPrimitiveGranularity = properties->meshOutputPerPrimitiveGranularity;
         all_properties->maxPreferredTaskWorkGroupInvocations = properties->maxPreferredTaskWorkGroupInvocations;
         all_properties->maxPreferredMeshWorkGroupInvocations = properties->maxPreferredMeshWorkGroupInvocations;
         all_properties->prefersLocalInvocationVertexOutput = properties->prefersLocalInvocationVertexOutput;
         all_properties->prefersLocalInvocationPrimitiveOutput = properties->prefersLocalInvocationPrimitiveOutput;
         all_properties->prefersCompactVertexOutput = properties->prefersCompactVertexOutput;
         all_properties->prefersCompactPrimitiveOutput = properties->prefersCompactPrimitiveOutput;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ACCELERATION_STRUCTURE_PROPERTIES_KHR: {
         const VkPhysicalDeviceAccelerationStructurePropertiesKHR *properties = (const VkPhysicalDeviceAccelerationStructurePropertiesKHR *)pProperties;
         all_properties->maxGeometryCount = properties->maxGeometryCount;
         all_properties->maxInstanceCount = properties->maxInstanceCount;
         all_properties->maxPrimitiveCount = properties->maxPrimitiveCount;
         all_properties->maxPerStageDescriptorAccelerationStructures = properties->maxPerStageDescriptorAccelerationStructures;
         all_properties->maxPerStageDescriptorUpdateAfterBindAccelerationStructures = properties->maxPerStageDescriptorUpdateAfterBindAccelerationStructures;
         all_properties->maxDescriptorSetAccelerationStructures = properties->maxDescriptorSetAccelerationStructures;
         all_properties->maxDescriptorSetUpdateAfterBindAccelerationStructures = properties->maxDescriptorSetUpdateAfterBindAccelerationStructures;
         all_properties->minAccelerationStructureScratchOffsetAlignment = properties->minAccelerationStructureScratchOffsetAlignment;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_PIPELINE_PROPERTIES_KHR: {
         const VkPhysicalDeviceRayTracingPipelinePropertiesKHR *properties = (const VkPhysicalDeviceRayTracingPipelinePropertiesKHR *)pProperties;
         all_properties->shaderGroupHandleSize = properties->shaderGroupHandleSize;
         all_properties->maxRayRecursionDepth = properties->maxRayRecursionDepth;
         all_properties->maxShaderGroupStride = properties->maxShaderGroupStride;
         all_properties->shaderGroupBaseAlignment = properties->shaderGroupBaseAlignment;
         all_properties->shaderGroupHandleCaptureReplaySize = properties->shaderGroupHandleCaptureReplaySize;
         all_properties->maxRayDispatchInvocationCount = properties->maxRayDispatchInvocationCount;
         all_properties->shaderGroupHandleAlignment = properties->shaderGroupHandleAlignment;
         all_properties->maxRayHitAttributeSize = properties->maxRayHitAttributeSize;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_PROPERTIES_NV: {
         const VkPhysicalDeviceRayTracingPropertiesNV *properties = (const VkPhysicalDeviceRayTracingPropertiesNV *)pProperties;
         all_properties->shaderGroupHandleSize = properties->shaderGroupHandleSize;
         all_properties->maxRecursionDepth = properties->maxRecursionDepth;
         all_properties->maxShaderGroupStride = properties->maxShaderGroupStride;
         all_properties->shaderGroupBaseAlignment = properties->shaderGroupBaseAlignment;
         all_properties->maxGeometryCount = properties->maxGeometryCount;
         all_properties->maxInstanceCount = properties->maxInstanceCount;
         all_properties->maxTriangleCount = properties->maxTriangleCount;
         all_properties->maxDescriptorSetAccelerationStructures = properties->maxDescriptorSetAccelerationStructures;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FRAGMENT_DENSITY_MAP_PROPERTIES_EXT: {
         const VkPhysicalDeviceFragmentDensityMapPropertiesEXT *properties = (const VkPhysicalDeviceFragmentDensityMapPropertiesEXT *)pProperties;
         all_properties->minFragmentDensityTexelSize = properties->minFragmentDensityTexelSize;
         all_properties->maxFragmentDensityTexelSize = properties->maxFragmentDensityTexelSize;
         all_properties->fragmentDensityInvocations = properties->fragmentDensityInvocations;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FRAGMENT_DENSITY_MAP_2_PROPERTIES_EXT: {
         const VkPhysicalDeviceFragmentDensityMap2PropertiesEXT *properties = (const VkPhysicalDeviceFragmentDensityMap2PropertiesEXT *)pProperties;
         all_properties->subsampledLoads = properties->subsampledLoads;
         all_properties->subsampledCoarseReconstructionEarlyAccess = properties->subsampledCoarseReconstructionEarlyAccess;
         all_properties->maxSubsampledArrayLayers = properties->maxSubsampledArrayLayers;
         all_properties->maxDescriptorSetSubsampledSamplers = properties->maxDescriptorSetSubsampledSamplers;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FRAGMENT_DENSITY_MAP_OFFSET_PROPERTIES_QCOM: {
         const VkPhysicalDeviceFragmentDensityMapOffsetPropertiesQCOM *properties = (const VkPhysicalDeviceFragmentDensityMapOffsetPropertiesQCOM *)pProperties;
         all_properties->fragmentDensityOffsetGranularity = properties->fragmentDensityOffsetGranularity;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_COOPERATIVE_MATRIX_PROPERTIES_NV: {
         const VkPhysicalDeviceCooperativeMatrixPropertiesNV *properties = (const VkPhysicalDeviceCooperativeMatrixPropertiesNV *)pProperties;
         all_properties->cooperativeMatrixSupportedStages = properties->cooperativeMatrixSupportedStages;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PERFORMANCE_QUERY_PROPERTIES_KHR: {
         const VkPhysicalDevicePerformanceQueryPropertiesKHR *properties = (const VkPhysicalDevicePerformanceQueryPropertiesKHR *)pProperties;
         all_properties->allowCommandBufferQueryCopies = properties->allowCommandBufferQueryCopies;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_SM_BUILTINS_PROPERTIES_NV: {
         const VkPhysicalDeviceShaderSMBuiltinsPropertiesNV *properties = (const VkPhysicalDeviceShaderSMBuiltinsPropertiesNV *)pProperties;
         all_properties->shaderSMCount = properties->shaderSMCount;
         all_properties->shaderWarpsPerSM = properties->shaderWarpsPerSM;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TEXEL_BUFFER_ALIGNMENT_PROPERTIES: {
         const VkPhysicalDeviceTexelBufferAlignmentProperties *properties = (const VkPhysicalDeviceTexelBufferAlignmentProperties *)pProperties;
         all_properties->storageTexelBufferOffsetAlignmentBytes = properties->storageTexelBufferOffsetAlignmentBytes;
         all_properties->storageTexelBufferOffsetSingleTexelAlignment = properties->storageTexelBufferOffsetSingleTexelAlignment;
         all_properties->uniformTexelBufferOffsetAlignmentBytes = properties->uniformTexelBufferOffsetAlignmentBytes;
         all_properties->uniformTexelBufferOffsetSingleTexelAlignment = properties->uniformTexelBufferOffsetSingleTexelAlignment;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SUBGROUP_SIZE_CONTROL_PROPERTIES: {
         const VkPhysicalDeviceSubgroupSizeControlProperties *properties = (const VkPhysicalDeviceSubgroupSizeControlProperties *)pProperties;
         all_properties->minSubgroupSize = properties->minSubgroupSize;
         all_properties->maxSubgroupSize = properties->maxSubgroupSize;
         all_properties->maxComputeWorkgroupSubgroups = properties->maxComputeWorkgroupSubgroups;
         all_properties->requiredSubgroupSizeStages = properties->requiredSubgroupSizeStages;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SUBPASS_SHADING_PROPERTIES_HUAWEI: {
         const VkPhysicalDeviceSubpassShadingPropertiesHUAWEI *properties = (const VkPhysicalDeviceSubpassShadingPropertiesHUAWEI *)pProperties;
         all_properties->maxSubpassShadingWorkgroupSizeAspectRatio = properties->maxSubpassShadingWorkgroupSizeAspectRatio;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_CLUSTER_CULLING_SHADER_PROPERTIES_HUAWEI: {
         const VkPhysicalDeviceClusterCullingShaderPropertiesHUAWEI *properties = (const VkPhysicalDeviceClusterCullingShaderPropertiesHUAWEI *)pProperties;
         memcpy(all_properties->maxWorkGroupCount, properties->maxWorkGroupCount, sizeof(all_properties->maxWorkGroupCount));
         memcpy(all_properties->maxWorkGroupSize, properties->maxWorkGroupSize, sizeof(all_properties->maxWorkGroupSize));
         all_properties->maxOutputClusterCount = properties->maxOutputClusterCount;
         all_properties->indirectBufferOffsetAlignment = properties->indirectBufferOffsetAlignment;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_LINE_RASTERIZATION_PROPERTIES: {
         const VkPhysicalDeviceLineRasterizationProperties *properties = (const VkPhysicalDeviceLineRasterizationProperties *)pProperties;
         all_properties->lineSubPixelPrecisionBits = properties->lineSubPixelPrecisionBits;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_PROPERTIES: {
         const VkPhysicalDeviceVulkan11Properties *properties = (const VkPhysicalDeviceVulkan11Properties *)pProperties;
         memcpy(all_properties->deviceUUID, properties->deviceUUID, sizeof(all_properties->deviceUUID));
         memcpy(all_properties->driverUUID, properties->driverUUID, sizeof(all_properties->driverUUID));
         memcpy(all_properties->deviceLUID, properties->deviceLUID, sizeof(all_properties->deviceLUID));
         all_properties->deviceNodeMask = properties->deviceNodeMask;
         all_properties->deviceLUIDValid = properties->deviceLUIDValid;
         all_properties->subgroupSize = properties->subgroupSize;
         all_properties->subgroupSupportedStages = properties->subgroupSupportedStages;
         all_properties->subgroupSupportedOperations = properties->subgroupSupportedOperations;
         all_properties->subgroupQuadOperationsInAllStages = properties->subgroupQuadOperationsInAllStages;
         all_properties->pointClippingBehavior = properties->pointClippingBehavior;
         all_properties->maxMultiviewViewCount = properties->maxMultiviewViewCount;
         all_properties->maxMultiviewInstanceIndex = properties->maxMultiviewInstanceIndex;
         all_properties->protectedNoFault = properties->protectedNoFault;
         all_properties->maxPerSetDescriptors = properties->maxPerSetDescriptors;
         all_properties->maxMemoryAllocationSize = properties->maxMemoryAllocationSize;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_PROPERTIES: {
         const VkPhysicalDeviceVulkan12Properties *properties = (const VkPhysicalDeviceVulkan12Properties *)pProperties;
         all_properties->driverID = properties->driverID;
         memcpy(all_properties->driverName, properties->driverName, sizeof(all_properties->driverName));
         memcpy(all_properties->driverInfo, properties->driverInfo, sizeof(all_properties->driverInfo));
         all_properties->conformanceVersion = properties->conformanceVersion;
         all_properties->denormBehaviorIndependence = properties->denormBehaviorIndependence;
         all_properties->roundingModeIndependence = properties->roundingModeIndependence;
         all_properties->shaderSignedZeroInfNanPreserveFloat16 = properties->shaderSignedZeroInfNanPreserveFloat16;
         all_properties->shaderSignedZeroInfNanPreserveFloat32 = properties->shaderSignedZeroInfNanPreserveFloat32;
         all_properties->shaderSignedZeroInfNanPreserveFloat64 = properties->shaderSignedZeroInfNanPreserveFloat64;
         all_properties->shaderDenormPreserveFloat16 = properties->shaderDenormPreserveFloat16;
         all_properties->shaderDenormPreserveFloat32 = properties->shaderDenormPreserveFloat32;
         all_properties->shaderDenormPreserveFloat64 = properties->shaderDenormPreserveFloat64;
         all_properties->shaderDenormFlushToZeroFloat16 = properties->shaderDenormFlushToZeroFloat16;
         all_properties->shaderDenormFlushToZeroFloat32 = properties->shaderDenormFlushToZeroFloat32;
         all_properties->shaderDenormFlushToZeroFloat64 = properties->shaderDenormFlushToZeroFloat64;
         all_properties->shaderRoundingModeRTEFloat16 = properties->shaderRoundingModeRTEFloat16;
         all_properties->shaderRoundingModeRTEFloat32 = properties->shaderRoundingModeRTEFloat32;
         all_properties->shaderRoundingModeRTEFloat64 = properties->shaderRoundingModeRTEFloat64;
         all_properties->shaderRoundingModeRTZFloat16 = properties->shaderRoundingModeRTZFloat16;
         all_properties->shaderRoundingModeRTZFloat32 = properties->shaderRoundingModeRTZFloat32;
         all_properties->shaderRoundingModeRTZFloat64 = properties->shaderRoundingModeRTZFloat64;
         all_properties->maxUpdateAfterBindDescriptorsInAllPools = properties->maxUpdateAfterBindDescriptorsInAllPools;
         all_properties->shaderUniformBufferArrayNonUniformIndexingNative = properties->shaderUniformBufferArrayNonUniformIndexingNative;
         all_properties->shaderSampledImageArrayNonUniformIndexingNative = properties->shaderSampledImageArrayNonUniformIndexingNative;
         all_properties->shaderStorageBufferArrayNonUniformIndexingNative = properties->shaderStorageBufferArrayNonUniformIndexingNative;
         all_properties->shaderStorageImageArrayNonUniformIndexingNative = properties->shaderStorageImageArrayNonUniformIndexingNative;
         all_properties->shaderInputAttachmentArrayNonUniformIndexingNative = properties->shaderInputAttachmentArrayNonUniformIndexingNative;
         all_properties->robustBufferAccessUpdateAfterBind = properties->robustBufferAccessUpdateAfterBind;
         all_properties->quadDivergentImplicitLod = properties->quadDivergentImplicitLod;
         all_properties->maxPerStageDescriptorUpdateAfterBindSamplers = properties->maxPerStageDescriptorUpdateAfterBindSamplers;
         all_properties->maxPerStageDescriptorUpdateAfterBindUniformBuffers = properties->maxPerStageDescriptorUpdateAfterBindUniformBuffers;
         all_properties->maxPerStageDescriptorUpdateAfterBindStorageBuffers = properties->maxPerStageDescriptorUpdateAfterBindStorageBuffers;
         all_properties->maxPerStageDescriptorUpdateAfterBindSampledImages = properties->maxPerStageDescriptorUpdateAfterBindSampledImages;
         all_properties->maxPerStageDescriptorUpdateAfterBindStorageImages = properties->maxPerStageDescriptorUpdateAfterBindStorageImages;
         all_properties->maxPerStageDescriptorUpdateAfterBindInputAttachments = properties->maxPerStageDescriptorUpdateAfterBindInputAttachments;
         all_properties->maxPerStageUpdateAfterBindResources = properties->maxPerStageUpdateAfterBindResources;
         all_properties->maxDescriptorSetUpdateAfterBindSamplers = properties->maxDescriptorSetUpdateAfterBindSamplers;
         all_properties->maxDescriptorSetUpdateAfterBindUniformBuffers = properties->maxDescriptorSetUpdateAfterBindUniformBuffers;
         all_properties->maxDescriptorSetUpdateAfterBindUniformBuffersDynamic = properties->maxDescriptorSetUpdateAfterBindUniformBuffersDynamic;
         all_properties->maxDescriptorSetUpdateAfterBindStorageBuffers = properties->maxDescriptorSetUpdateAfterBindStorageBuffers;
         all_properties->maxDescriptorSetUpdateAfterBindStorageBuffersDynamic = properties->maxDescriptorSetUpdateAfterBindStorageBuffersDynamic;
         all_properties->maxDescriptorSetUpdateAfterBindSampledImages = properties->maxDescriptorSetUpdateAfterBindSampledImages;
         all_properties->maxDescriptorSetUpdateAfterBindStorageImages = properties->maxDescriptorSetUpdateAfterBindStorageImages;
         all_properties->maxDescriptorSetUpdateAfterBindInputAttachments = properties->maxDescriptorSetUpdateAfterBindInputAttachments;
         all_properties->supportedDepthResolveModes = properties->supportedDepthResolveModes;
         all_properties->supportedStencilResolveModes = properties->supportedStencilResolveModes;
         all_properties->independentResolveNone = properties->independentResolveNone;
         all_properties->independentResolve = properties->independentResolve;
         all_properties->filterMinmaxSingleComponentFormats = properties->filterMinmaxSingleComponentFormats;
         all_properties->filterMinmaxImageComponentMapping = properties->filterMinmaxImageComponentMapping;
         all_properties->maxTimelineSemaphoreValueDifference = properties->maxTimelineSemaphoreValueDifference;
         all_properties->framebufferIntegerColorSampleCounts = properties->framebufferIntegerColorSampleCounts;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_PROPERTIES: {
         const VkPhysicalDeviceVulkan13Properties *properties = (const VkPhysicalDeviceVulkan13Properties *)pProperties;
         all_properties->minSubgroupSize = properties->minSubgroupSize;
         all_properties->maxSubgroupSize = properties->maxSubgroupSize;
         all_properties->maxComputeWorkgroupSubgroups = properties->maxComputeWorkgroupSubgroups;
         all_properties->requiredSubgroupSizeStages = properties->requiredSubgroupSizeStages;
         all_properties->maxInlineUniformBlockSize = properties->maxInlineUniformBlockSize;
         all_properties->maxPerStageDescriptorInlineUniformBlocks = properties->maxPerStageDescriptorInlineUniformBlocks;
         all_properties->maxPerStageDescriptorUpdateAfterBindInlineUniformBlocks = properties->maxPerStageDescriptorUpdateAfterBindInlineUniformBlocks;
         all_properties->maxDescriptorSetInlineUniformBlocks = properties->maxDescriptorSetInlineUniformBlocks;
         all_properties->maxDescriptorSetUpdateAfterBindInlineUniformBlocks = properties->maxDescriptorSetUpdateAfterBindInlineUniformBlocks;
         all_properties->maxInlineUniformTotalSize = properties->maxInlineUniformTotalSize;
         all_properties->integerDotProduct8BitUnsignedAccelerated = properties->integerDotProduct8BitUnsignedAccelerated;
         all_properties->integerDotProduct8BitSignedAccelerated = properties->integerDotProduct8BitSignedAccelerated;
         all_properties->integerDotProduct8BitMixedSignednessAccelerated = properties->integerDotProduct8BitMixedSignednessAccelerated;
         all_properties->integerDotProduct4x8BitPackedUnsignedAccelerated = properties->integerDotProduct4x8BitPackedUnsignedAccelerated;
         all_properties->integerDotProduct4x8BitPackedSignedAccelerated = properties->integerDotProduct4x8BitPackedSignedAccelerated;
         all_properties->integerDotProduct4x8BitPackedMixedSignednessAccelerated = properties->integerDotProduct4x8BitPackedMixedSignednessAccelerated;
         all_properties->integerDotProduct16BitUnsignedAccelerated = properties->integerDotProduct16BitUnsignedAccelerated;
         all_properties->integerDotProduct16BitSignedAccelerated = properties->integerDotProduct16BitSignedAccelerated;
         all_properties->integerDotProduct16BitMixedSignednessAccelerated = properties->integerDotProduct16BitMixedSignednessAccelerated;
         all_properties->integerDotProduct32BitUnsignedAccelerated = properties->integerDotProduct32BitUnsignedAccelerated;
         all_properties->integerDotProduct32BitSignedAccelerated = properties->integerDotProduct32BitSignedAccelerated;
         all_properties->integerDotProduct32BitMixedSignednessAccelerated = properties->integerDotProduct32BitMixedSignednessAccelerated;
         all_properties->integerDotProduct64BitUnsignedAccelerated = properties->integerDotProduct64BitUnsignedAccelerated;
         all_properties->integerDotProduct64BitSignedAccelerated = properties->integerDotProduct64BitSignedAccelerated;
         all_properties->integerDotProduct64BitMixedSignednessAccelerated = properties->integerDotProduct64BitMixedSignednessAccelerated;
         all_properties->integerDotProductAccumulatingSaturating8BitUnsignedAccelerated = properties->integerDotProductAccumulatingSaturating8BitUnsignedAccelerated;
         all_properties->integerDotProductAccumulatingSaturating8BitSignedAccelerated = properties->integerDotProductAccumulatingSaturating8BitSignedAccelerated;
         all_properties->integerDotProductAccumulatingSaturating8BitMixedSignednessAccelerated = properties->integerDotProductAccumulatingSaturating8BitMixedSignednessAccelerated;
         all_properties->integerDotProductAccumulatingSaturating4x8BitPackedUnsignedAccelerated = properties->integerDotProductAccumulatingSaturating4x8BitPackedUnsignedAccelerated;
         all_properties->integerDotProductAccumulatingSaturating4x8BitPackedSignedAccelerated = properties->integerDotProductAccumulatingSaturating4x8BitPackedSignedAccelerated;
         all_properties->integerDotProductAccumulatingSaturating4x8BitPackedMixedSignednessAccelerated = properties->integerDotProductAccumulatingSaturating4x8BitPackedMixedSignednessAccelerated;
         all_properties->integerDotProductAccumulatingSaturating16BitUnsignedAccelerated = properties->integerDotProductAccumulatingSaturating16BitUnsignedAccelerated;
         all_properties->integerDotProductAccumulatingSaturating16BitSignedAccelerated = properties->integerDotProductAccumulatingSaturating16BitSignedAccelerated;
         all_properties->integerDotProductAccumulatingSaturating16BitMixedSignednessAccelerated = properties->integerDotProductAccumulatingSaturating16BitMixedSignednessAccelerated;
         all_properties->integerDotProductAccumulatingSaturating32BitUnsignedAccelerated = properties->integerDotProductAccumulatingSaturating32BitUnsignedAccelerated;
         all_properties->integerDotProductAccumulatingSaturating32BitSignedAccelerated = properties->integerDotProductAccumulatingSaturating32BitSignedAccelerated;
         all_properties->integerDotProductAccumulatingSaturating32BitMixedSignednessAccelerated = properties->integerDotProductAccumulatingSaturating32BitMixedSignednessAccelerated;
         all_properties->integerDotProductAccumulatingSaturating64BitUnsignedAccelerated = properties->integerDotProductAccumulatingSaturating64BitUnsignedAccelerated;
         all_properties->integerDotProductAccumulatingSaturating64BitSignedAccelerated = properties->integerDotProductAccumulatingSaturating64BitSignedAccelerated;
         all_properties->integerDotProductAccumulatingSaturating64BitMixedSignednessAccelerated = properties->integerDotProductAccumulatingSaturating64BitMixedSignednessAccelerated;
         all_properties->storageTexelBufferOffsetAlignmentBytes = properties->storageTexelBufferOffsetAlignmentBytes;
         all_properties->storageTexelBufferOffsetSingleTexelAlignment = properties->storageTexelBufferOffsetSingleTexelAlignment;
         all_properties->uniformTexelBufferOffsetAlignmentBytes = properties->uniformTexelBufferOffsetAlignmentBytes;
         all_properties->uniformTexelBufferOffsetSingleTexelAlignment = properties->uniformTexelBufferOffsetSingleTexelAlignment;
         all_properties->maxBufferSize = properties->maxBufferSize;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_4_PROPERTIES: {
         const VkPhysicalDeviceVulkan14Properties *properties = (const VkPhysicalDeviceVulkan14Properties *)pProperties;
         all_properties->lineSubPixelPrecisionBits = properties->lineSubPixelPrecisionBits;
         all_properties->maxVertexAttribDivisor = properties->maxVertexAttribDivisor;
         all_properties->supportsNonZeroFirstInstance = properties->supportsNonZeroFirstInstance;
         all_properties->maxPushDescriptors = properties->maxPushDescriptors;
         all_properties->dynamicRenderingLocalReadDepthStencilAttachments = properties->dynamicRenderingLocalReadDepthStencilAttachments;
         all_properties->dynamicRenderingLocalReadMultisampledAttachments = properties->dynamicRenderingLocalReadMultisampledAttachments;
         all_properties->earlyFragmentMultisampleCoverageAfterSampleCounting = properties->earlyFragmentMultisampleCoverageAfterSampleCounting;
         all_properties->earlyFragmentSampleMaskTestBeforeSampleCounting = properties->earlyFragmentSampleMaskTestBeforeSampleCounting;
         all_properties->depthStencilSwizzleOneSupport = properties->depthStencilSwizzleOneSupport;
         all_properties->polygonModePointSize = properties->polygonModePointSize;
         all_properties->nonStrictSinglePixelWideLinesUseParallelogram = properties->nonStrictSinglePixelWideLinesUseParallelogram;
         all_properties->nonStrictWideLinesUseParallelogram = properties->nonStrictWideLinesUseParallelogram;
         all_properties->blockTexelViewCompatibleMultipleLayers = properties->blockTexelViewCompatibleMultipleLayers;
         all_properties->maxCombinedImageSamplerDescriptorCount = properties->maxCombinedImageSamplerDescriptorCount;
         all_properties->fragmentShadingRateClampCombinerInputs = properties->fragmentShadingRateClampCombinerInputs;
         all_properties->defaultRobustnessStorageBuffers = properties->defaultRobustnessStorageBuffers;
         all_properties->defaultRobustnessUniformBuffers = properties->defaultRobustnessUniformBuffers;
         all_properties->defaultRobustnessVertexInputs = properties->defaultRobustnessVertexInputs;
         all_properties->defaultRobustnessImages = properties->defaultRobustnessImages;
         
         
    if (all_properties->pCopySrcLayouts != NULL) {
        uint32_t count = MIN2(all_properties->copySrcLayoutCount, properties->copySrcLayoutCount);
        for (uint32_t i = 0; i < count; i++)
            all_properties->pCopySrcLayouts[i] = properties->pCopySrcLayouts[i];
        all_properties->copySrcLayoutCount = count;
    } else {
        all_properties->copySrcLayoutCount = properties->copySrcLayoutCount;
    }

         
         
    if (all_properties->pCopyDstLayouts != NULL) {
        uint32_t count = MIN2(all_properties->copyDstLayoutCount, properties->copyDstLayoutCount);
        for (uint32_t i = 0; i < count; i++)
            all_properties->pCopyDstLayouts[i] = properties->pCopyDstLayouts[i];
        all_properties->copyDstLayoutCount = count;
    } else {
        all_properties->copyDstLayoutCount = properties->copyDstLayoutCount;
    }

         memcpy(all_properties->optimalTilingLayoutUUID, properties->optimalTilingLayoutUUID, sizeof(all_properties->optimalTilingLayoutUUID));
         all_properties->identicalMemoryTypeRequirements = properties->identicalMemoryTypeRequirements;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_CUSTOM_BORDER_COLOR_PROPERTIES_EXT: {
         const VkPhysicalDeviceCustomBorderColorPropertiesEXT *properties = (const VkPhysicalDeviceCustomBorderColorPropertiesEXT *)pProperties;
         all_properties->maxCustomBorderColorSamplers = properties->maxCustomBorderColorSamplers;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTENDED_DYNAMIC_STATE_3_PROPERTIES_EXT: {
         const VkPhysicalDeviceExtendedDynamicState3PropertiesEXT *properties = (const VkPhysicalDeviceExtendedDynamicState3PropertiesEXT *)pProperties;
         all_properties->dynamicPrimitiveTopologyUnrestricted = properties->dynamicPrimitiveTopologyUnrestricted;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ROBUSTNESS_2_PROPERTIES_EXT: {
         const VkPhysicalDeviceRobustness2PropertiesEXT *properties = (const VkPhysicalDeviceRobustness2PropertiesEXT *)pProperties;
         all_properties->robustStorageBufferAccessSizeAlignment = properties->robustStorageBufferAccessSizeAlignment;
         all_properties->robustUniformBufferAccessSizeAlignment = properties->robustUniformBufferAccessSizeAlignment;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FRAGMENT_SHADING_RATE_PROPERTIES_KHR: {
         const VkPhysicalDeviceFragmentShadingRatePropertiesKHR *properties = (const VkPhysicalDeviceFragmentShadingRatePropertiesKHR *)pProperties;
         all_properties->minFragmentShadingRateAttachmentTexelSize = properties->minFragmentShadingRateAttachmentTexelSize;
         all_properties->maxFragmentShadingRateAttachmentTexelSize = properties->maxFragmentShadingRateAttachmentTexelSize;
         all_properties->maxFragmentShadingRateAttachmentTexelSizeAspectRatio = properties->maxFragmentShadingRateAttachmentTexelSizeAspectRatio;
         all_properties->primitiveFragmentShadingRateWithMultipleViewports = properties->primitiveFragmentShadingRateWithMultipleViewports;
         all_properties->layeredShadingRateAttachments = properties->layeredShadingRateAttachments;
         all_properties->fragmentShadingRateNonTrivialCombinerOps = properties->fragmentShadingRateNonTrivialCombinerOps;
         all_properties->maxFragmentSize = properties->maxFragmentSize;
         all_properties->maxFragmentSizeAspectRatio = properties->maxFragmentSizeAspectRatio;
         all_properties->maxFragmentShadingRateCoverageSamples = properties->maxFragmentShadingRateCoverageSamples;
         all_properties->maxFragmentShadingRateRasterizationSamples = properties->maxFragmentShadingRateRasterizationSamples;
         all_properties->fragmentShadingRateWithShaderDepthStencilWrites = properties->fragmentShadingRateWithShaderDepthStencilWrites;
         all_properties->fragmentShadingRateWithSampleMask = properties->fragmentShadingRateWithSampleMask;
         all_properties->fragmentShadingRateWithShaderSampleMask = properties->fragmentShadingRateWithShaderSampleMask;
         all_properties->fragmentShadingRateWithConservativeRasterization = properties->fragmentShadingRateWithConservativeRasterization;
         all_properties->fragmentShadingRateWithFragmentShaderInterlock = properties->fragmentShadingRateWithFragmentShaderInterlock;
         all_properties->fragmentShadingRateWithCustomSampleLocations = properties->fragmentShadingRateWithCustomSampleLocations;
         all_properties->fragmentShadingRateStrictMultiplyCombiner = properties->fragmentShadingRateStrictMultiplyCombiner;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FRAGMENT_SHADING_RATE_ENUMS_PROPERTIES_NV: {
         const VkPhysicalDeviceFragmentShadingRateEnumsPropertiesNV *properties = (const VkPhysicalDeviceFragmentShadingRateEnumsPropertiesNV *)pProperties;
         all_properties->maxFragmentShadingRateInvocationCount = properties->maxFragmentShadingRateInvocationCount;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_LEGACY_VERTEX_ATTRIBUTES_PROPERTIES_EXT: {
         const VkPhysicalDeviceLegacyVertexAttributesPropertiesEXT *properties = (const VkPhysicalDeviceLegacyVertexAttributesPropertiesEXT *)pProperties;
         all_properties->nativeUnalignedPerformance = properties->nativeUnalignedPerformance;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DEVICE_GENERATED_COMMANDS_PROPERTIES_EXT: {
         const VkPhysicalDeviceDeviceGeneratedCommandsPropertiesEXT *properties = (const VkPhysicalDeviceDeviceGeneratedCommandsPropertiesEXT *)pProperties;
         all_properties->maxIndirectPipelineCount = properties->maxIndirectPipelineCount;
         all_properties->maxIndirectShaderObjectCount = properties->maxIndirectShaderObjectCount;
         all_properties->maxIndirectSequenceCount = properties->maxIndirectSequenceCount;
         all_properties->maxIndirectCommandsTokenCount = properties->maxIndirectCommandsTokenCount;
         all_properties->maxIndirectCommandsTokenOffset = properties->maxIndirectCommandsTokenOffset;
         all_properties->maxIndirectCommandsIndirectStride = properties->maxIndirectCommandsIndirectStride;
         all_properties->supportedIndirectCommandsInputModes = properties->supportedIndirectCommandsInputModes;
         all_properties->supportedIndirectCommandsShaderStages = properties->supportedIndirectCommandsShaderStages;
         all_properties->supportedIndirectCommandsShaderStagesPipelineBinding = properties->supportedIndirectCommandsShaderStagesPipelineBinding;
         all_properties->supportedIndirectCommandsShaderStagesShaderBinding = properties->supportedIndirectCommandsShaderStagesShaderBinding;
         all_properties->deviceGeneratedCommandsTransformFeedback = properties->deviceGeneratedCommandsTransformFeedback;
         all_properties->deviceGeneratedCommandsMultiDrawIndirectCount = properties->deviceGeneratedCommandsMultiDrawIndirectCount;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_HOST_IMAGE_COPY_PROPERTIES: {
         const VkPhysicalDeviceHostImageCopyProperties *properties = (const VkPhysicalDeviceHostImageCopyProperties *)pProperties;
         
         
    if (all_properties->pCopySrcLayouts != NULL) {
        uint32_t count = MIN2(all_properties->copySrcLayoutCount, properties->copySrcLayoutCount);
        for (uint32_t i = 0; i < count; i++)
            all_properties->pCopySrcLayouts[i] = properties->pCopySrcLayouts[i];
        all_properties->copySrcLayoutCount = count;
    } else {
        all_properties->copySrcLayoutCount = properties->copySrcLayoutCount;
    }

         
         
    if (all_properties->pCopyDstLayouts != NULL) {
        uint32_t count = MIN2(all_properties->copyDstLayoutCount, properties->copyDstLayoutCount);
        for (uint32_t i = 0; i < count; i++)
            all_properties->pCopyDstLayouts[i] = properties->pCopyDstLayouts[i];
        all_properties->copyDstLayoutCount = count;
    } else {
        all_properties->copyDstLayoutCount = properties->copyDstLayoutCount;
    }

         memcpy(all_properties->optimalTilingLayoutUUID, properties->optimalTilingLayoutUUID, sizeof(all_properties->optimalTilingLayoutUUID));
         all_properties->identicalMemoryTypeRequirements = properties->identicalMemoryTypeRequirements;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROVOKING_VERTEX_PROPERTIES_EXT: {
         const VkPhysicalDeviceProvokingVertexPropertiesEXT *properties = (const VkPhysicalDeviceProvokingVertexPropertiesEXT *)pProperties;
         all_properties->provokingVertexModePerPipeline = properties->provokingVertexModePerPipeline;
         all_properties->transformFeedbackPreservesTriangleFanProvokingVertex = properties->transformFeedbackPreservesTriangleFanProvokingVertex;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_BUFFER_PROPERTIES_EXT: {
         const VkPhysicalDeviceDescriptorBufferPropertiesEXT *properties = (const VkPhysicalDeviceDescriptorBufferPropertiesEXT *)pProperties;
         all_properties->combinedImageSamplerDescriptorSingleArray = properties->combinedImageSamplerDescriptorSingleArray;
         all_properties->bufferlessPushDescriptors = properties->bufferlessPushDescriptors;
         all_properties->allowSamplerImageViewPostSubmitCreation = properties->allowSamplerImageViewPostSubmitCreation;
         all_properties->descriptorBufferOffsetAlignment = properties->descriptorBufferOffsetAlignment;
         all_properties->maxDescriptorBufferBindings = properties->maxDescriptorBufferBindings;
         all_properties->maxResourceDescriptorBufferBindings = properties->maxResourceDescriptorBufferBindings;
         all_properties->maxSamplerDescriptorBufferBindings = properties->maxSamplerDescriptorBufferBindings;
         all_properties->maxEmbeddedImmutableSamplerBindings = properties->maxEmbeddedImmutableSamplerBindings;
         all_properties->maxEmbeddedImmutableSamplers = properties->maxEmbeddedImmutableSamplers;
         all_properties->bufferCaptureReplayDescriptorDataSize = properties->bufferCaptureReplayDescriptorDataSize;
         all_properties->imageCaptureReplayDescriptorDataSize = properties->imageCaptureReplayDescriptorDataSize;
         all_properties->imageViewCaptureReplayDescriptorDataSize = properties->imageViewCaptureReplayDescriptorDataSize;
         all_properties->samplerCaptureReplayDescriptorDataSize = properties->samplerCaptureReplayDescriptorDataSize;
         all_properties->accelerationStructureCaptureReplayDescriptorDataSize = properties->accelerationStructureCaptureReplayDescriptorDataSize;
         all_properties->samplerDescriptorSize = properties->samplerDescriptorSize;
         all_properties->combinedImageSamplerDescriptorSize = properties->combinedImageSamplerDescriptorSize;
         all_properties->sampledImageDescriptorSize = properties->sampledImageDescriptorSize;
         all_properties->storageImageDescriptorSize = properties->storageImageDescriptorSize;
         all_properties->uniformTexelBufferDescriptorSize = properties->uniformTexelBufferDescriptorSize;
         all_properties->robustUniformTexelBufferDescriptorSize = properties->robustUniformTexelBufferDescriptorSize;
         all_properties->storageTexelBufferDescriptorSize = properties->storageTexelBufferDescriptorSize;
         all_properties->robustStorageTexelBufferDescriptorSize = properties->robustStorageTexelBufferDescriptorSize;
         all_properties->uniformBufferDescriptorSize = properties->uniformBufferDescriptorSize;
         all_properties->robustUniformBufferDescriptorSize = properties->robustUniformBufferDescriptorSize;
         all_properties->storageBufferDescriptorSize = properties->storageBufferDescriptorSize;
         all_properties->robustStorageBufferDescriptorSize = properties->robustStorageBufferDescriptorSize;
         all_properties->inputAttachmentDescriptorSize = properties->inputAttachmentDescriptorSize;
         all_properties->accelerationStructureDescriptorSize = properties->accelerationStructureDescriptorSize;
         all_properties->maxSamplerDescriptorBufferRange = properties->maxSamplerDescriptorBufferRange;
         all_properties->maxResourceDescriptorBufferRange = properties->maxResourceDescriptorBufferRange;
         all_properties->samplerDescriptorBufferAddressSpaceSize = properties->samplerDescriptorBufferAddressSpaceSize;
         all_properties->resourceDescriptorBufferAddressSpaceSize = properties->resourceDescriptorBufferAddressSpaceSize;
         all_properties->descriptorBufferAddressSpaceSize = properties->descriptorBufferAddressSpaceSize;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_BUFFER_DENSITY_MAP_PROPERTIES_EXT: {
         const VkPhysicalDeviceDescriptorBufferDensityMapPropertiesEXT *properties = (const VkPhysicalDeviceDescriptorBufferDensityMapPropertiesEXT *)pProperties;
         all_properties->combinedImageSamplerDensityMapDescriptorSize = properties->combinedImageSamplerDensityMapDescriptorSize;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_INTEGER_DOT_PRODUCT_PROPERTIES: {
         const VkPhysicalDeviceShaderIntegerDotProductProperties *properties = (const VkPhysicalDeviceShaderIntegerDotProductProperties *)pProperties;
         all_properties->integerDotProduct8BitUnsignedAccelerated = properties->integerDotProduct8BitUnsignedAccelerated;
         all_properties->integerDotProduct8BitSignedAccelerated = properties->integerDotProduct8BitSignedAccelerated;
         all_properties->integerDotProduct8BitMixedSignednessAccelerated = properties->integerDotProduct8BitMixedSignednessAccelerated;
         all_properties->integerDotProduct4x8BitPackedUnsignedAccelerated = properties->integerDotProduct4x8BitPackedUnsignedAccelerated;
         all_properties->integerDotProduct4x8BitPackedSignedAccelerated = properties->integerDotProduct4x8BitPackedSignedAccelerated;
         all_properties->integerDotProduct4x8BitPackedMixedSignednessAccelerated = properties->integerDotProduct4x8BitPackedMixedSignednessAccelerated;
         all_properties->integerDotProduct16BitUnsignedAccelerated = properties->integerDotProduct16BitUnsignedAccelerated;
         all_properties->integerDotProduct16BitSignedAccelerated = properties->integerDotProduct16BitSignedAccelerated;
         all_properties->integerDotProduct16BitMixedSignednessAccelerated = properties->integerDotProduct16BitMixedSignednessAccelerated;
         all_properties->integerDotProduct32BitUnsignedAccelerated = properties->integerDotProduct32BitUnsignedAccelerated;
         all_properties->integerDotProduct32BitSignedAccelerated = properties->integerDotProduct32BitSignedAccelerated;
         all_properties->integerDotProduct32BitMixedSignednessAccelerated = properties->integerDotProduct32BitMixedSignednessAccelerated;
         all_properties->integerDotProduct64BitUnsignedAccelerated = properties->integerDotProduct64BitUnsignedAccelerated;
         all_properties->integerDotProduct64BitSignedAccelerated = properties->integerDotProduct64BitSignedAccelerated;
         all_properties->integerDotProduct64BitMixedSignednessAccelerated = properties->integerDotProduct64BitMixedSignednessAccelerated;
         all_properties->integerDotProductAccumulatingSaturating8BitUnsignedAccelerated = properties->integerDotProductAccumulatingSaturating8BitUnsignedAccelerated;
         all_properties->integerDotProductAccumulatingSaturating8BitSignedAccelerated = properties->integerDotProductAccumulatingSaturating8BitSignedAccelerated;
         all_properties->integerDotProductAccumulatingSaturating8BitMixedSignednessAccelerated = properties->integerDotProductAccumulatingSaturating8BitMixedSignednessAccelerated;
         all_properties->integerDotProductAccumulatingSaturating4x8BitPackedUnsignedAccelerated = properties->integerDotProductAccumulatingSaturating4x8BitPackedUnsignedAccelerated;
         all_properties->integerDotProductAccumulatingSaturating4x8BitPackedSignedAccelerated = properties->integerDotProductAccumulatingSaturating4x8BitPackedSignedAccelerated;
         all_properties->integerDotProductAccumulatingSaturating4x8BitPackedMixedSignednessAccelerated = properties->integerDotProductAccumulatingSaturating4x8BitPackedMixedSignednessAccelerated;
         all_properties->integerDotProductAccumulatingSaturating16BitUnsignedAccelerated = properties->integerDotProductAccumulatingSaturating16BitUnsignedAccelerated;
         all_properties->integerDotProductAccumulatingSaturating16BitSignedAccelerated = properties->integerDotProductAccumulatingSaturating16BitSignedAccelerated;
         all_properties->integerDotProductAccumulatingSaturating16BitMixedSignednessAccelerated = properties->integerDotProductAccumulatingSaturating16BitMixedSignednessAccelerated;
         all_properties->integerDotProductAccumulatingSaturating32BitUnsignedAccelerated = properties->integerDotProductAccumulatingSaturating32BitUnsignedAccelerated;
         all_properties->integerDotProductAccumulatingSaturating32BitSignedAccelerated = properties->integerDotProductAccumulatingSaturating32BitSignedAccelerated;
         all_properties->integerDotProductAccumulatingSaturating32BitMixedSignednessAccelerated = properties->integerDotProductAccumulatingSaturating32BitMixedSignednessAccelerated;
         all_properties->integerDotProductAccumulatingSaturating64BitUnsignedAccelerated = properties->integerDotProductAccumulatingSaturating64BitUnsignedAccelerated;
         all_properties->integerDotProductAccumulatingSaturating64BitSignedAccelerated = properties->integerDotProductAccumulatingSaturating64BitSignedAccelerated;
         all_properties->integerDotProductAccumulatingSaturating64BitMixedSignednessAccelerated = properties->integerDotProductAccumulatingSaturating64BitMixedSignednessAccelerated;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DRM_PROPERTIES_EXT: {
         const VkPhysicalDeviceDrmPropertiesEXT *properties = (const VkPhysicalDeviceDrmPropertiesEXT *)pProperties;
         all_properties->drmHasPrimary = properties->hasPrimary;
         all_properties->drmHasRender = properties->hasRender;
         all_properties->drmPrimaryMajor = properties->primaryMajor;
         all_properties->drmPrimaryMinor = properties->primaryMinor;
         all_properties->drmRenderMajor = properties->renderMajor;
         all_properties->drmRenderMinor = properties->renderMinor;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FRAGMENT_SHADER_BARYCENTRIC_PROPERTIES_KHR: {
         const VkPhysicalDeviceFragmentShaderBarycentricPropertiesKHR *properties = (const VkPhysicalDeviceFragmentShaderBarycentricPropertiesKHR *)pProperties;
         all_properties->triStripVertexOrderIndependentOfProvokingVertex = properties->triStripVertexOrderIndependentOfProvokingVertex;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PIPELINE_BINARY_PROPERTIES_KHR: {
         const VkPhysicalDevicePipelineBinaryPropertiesKHR *properties = (const VkPhysicalDevicePipelineBinaryPropertiesKHR *)pProperties;
         all_properties->pipelineBinaryInternalCache = properties->pipelineBinaryInternalCache;
         all_properties->pipelineBinaryInternalCacheControl = properties->pipelineBinaryInternalCacheControl;
         all_properties->pipelineBinaryPrefersInternalCache = properties->pipelineBinaryPrefersInternalCache;
         all_properties->pipelineBinaryPrecompiledInternalCache = properties->pipelineBinaryPrecompiledInternalCache;
         all_properties->pipelineBinaryCompressedData = properties->pipelineBinaryCompressedData;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_GRAPHICS_PIPELINE_LIBRARY_PROPERTIES_EXT: {
         const VkPhysicalDeviceGraphicsPipelineLibraryPropertiesEXT *properties = (const VkPhysicalDeviceGraphicsPipelineLibraryPropertiesEXT *)pProperties;
         all_properties->graphicsPipelineLibraryFastLinking = properties->graphicsPipelineLibraryFastLinking;
         all_properties->graphicsPipelineLibraryIndependentInterpolationDecoration = properties->graphicsPipelineLibraryIndependentInterpolationDecoration;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_NESTED_COMMAND_BUFFER_PROPERTIES_EXT: {
         const VkPhysicalDeviceNestedCommandBufferPropertiesEXT *properties = (const VkPhysicalDeviceNestedCommandBufferPropertiesEXT *)pProperties;
         all_properties->maxCommandBufferNestingLevel = properties->maxCommandBufferNestingLevel;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_MODULE_IDENTIFIER_PROPERTIES_EXT: {
         const VkPhysicalDeviceShaderModuleIdentifierPropertiesEXT *properties = (const VkPhysicalDeviceShaderModuleIdentifierPropertiesEXT *)pProperties;
         memcpy(all_properties->shaderModuleIdentifierAlgorithmUUID, properties->shaderModuleIdentifierAlgorithmUUID, sizeof(all_properties->shaderModuleIdentifierAlgorithmUUID));
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_OPACITY_MICROMAP_PROPERTIES_EXT: {
         const VkPhysicalDeviceOpacityMicromapPropertiesEXT *properties = (const VkPhysicalDeviceOpacityMicromapPropertiesEXT *)pProperties;
         all_properties->maxOpacity2StateSubdivisionLevel = properties->maxOpacity2StateSubdivisionLevel;
         all_properties->maxOpacity4StateSubdivisionLevel = properties->maxOpacity4StateSubdivisionLevel;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PIPELINE_ROBUSTNESS_PROPERTIES: {
         const VkPhysicalDevicePipelineRobustnessProperties *properties = (const VkPhysicalDevicePipelineRobustnessProperties *)pProperties;
         all_properties->defaultRobustnessStorageBuffers = properties->defaultRobustnessStorageBuffers;
         all_properties->defaultRobustnessUniformBuffers = properties->defaultRobustnessUniformBuffers;
         all_properties->defaultRobustnessVertexInputs = properties->defaultRobustnessVertexInputs;
         all_properties->defaultRobustnessImages = properties->defaultRobustnessImages;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_IMAGE_PROCESSING_PROPERTIES_QCOM: {
         const VkPhysicalDeviceImageProcessingPropertiesQCOM *properties = (const VkPhysicalDeviceImageProcessingPropertiesQCOM *)pProperties;
         all_properties->maxWeightFilterPhases = properties->maxWeightFilterPhases;
         all_properties->maxWeightFilterDimension = properties->maxWeightFilterDimension;
         all_properties->maxBlockMatchRegion = properties->maxBlockMatchRegion;
         all_properties->maxBoxFilterBlockSize = properties->maxBoxFilterBlockSize;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_OPTICAL_FLOW_PROPERTIES_NV: {
         const VkPhysicalDeviceOpticalFlowPropertiesNV *properties = (const VkPhysicalDeviceOpticalFlowPropertiesNV *)pProperties;
         all_properties->supportedOutputGridSizes = properties->supportedOutputGridSizes;
         all_properties->supportedHintGridSizes = properties->supportedHintGridSizes;
         all_properties->hintSupported = properties->hintSupported;
         all_properties->costSupported = properties->costSupported;
         all_properties->bidirectionalFlowSupported = properties->bidirectionalFlowSupported;
         all_properties->globalFlowSupported = properties->globalFlowSupported;
         all_properties->minWidth = properties->minWidth;
         all_properties->minHeight = properties->minHeight;
         all_properties->maxWidth = properties->maxWidth;
         all_properties->maxHeight = properties->maxHeight;
         all_properties->maxNumRegionsOfInterest = properties->maxNumRegionsOfInterest;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_CORE_BUILTINS_PROPERTIES_ARM: {
         const VkPhysicalDeviceShaderCoreBuiltinsPropertiesARM *properties = (const VkPhysicalDeviceShaderCoreBuiltinsPropertiesARM *)pProperties;
         all_properties->shaderCoreMask = properties->shaderCoreMask;
         all_properties->shaderCoreCount = properties->shaderCoreCount;
         all_properties->shaderWarpsPerCore = properties->shaderWarpsPerCore;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_INVOCATION_REORDER_PROPERTIES_NV: {
         const VkPhysicalDeviceRayTracingInvocationReorderPropertiesNV *properties = (const VkPhysicalDeviceRayTracingInvocationReorderPropertiesNV *)pProperties;
         all_properties->rayTracingInvocationReorderReorderingHint = properties->rayTracingInvocationReorderReorderingHint;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTENDED_SPARSE_ADDRESS_SPACE_PROPERTIES_NV: {
         const VkPhysicalDeviceExtendedSparseAddressSpacePropertiesNV *properties = (const VkPhysicalDeviceExtendedSparseAddressSpacePropertiesNV *)pProperties;
         all_properties->extendedSparseAddressSpaceSize = properties->extendedSparseAddressSpaceSize;
         all_properties->extendedSparseImageUsageFlags = properties->extendedSparseImageUsageFlags;
         all_properties->extendedSparseBufferUsageFlags = properties->extendedSparseBufferUsageFlags;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_CORE_PROPERTIES_ARM: {
         const VkPhysicalDeviceShaderCorePropertiesARM *properties = (const VkPhysicalDeviceShaderCorePropertiesARM *)pProperties;
         all_properties->pixelRate = properties->pixelRate;
         all_properties->texelRate = properties->texelRate;
         all_properties->fmaRate = properties->fmaRate;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_OBJECT_PROPERTIES_EXT: {
         const VkPhysicalDeviceShaderObjectPropertiesEXT *properties = (const VkPhysicalDeviceShaderObjectPropertiesEXT *)pProperties;
         memcpy(all_properties->shaderBinaryUUID, properties->shaderBinaryUUID, sizeof(all_properties->shaderBinaryUUID));
         all_properties->shaderBinaryVersion = properties->shaderBinaryVersion;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_TILE_IMAGE_PROPERTIES_EXT: {
         const VkPhysicalDeviceShaderTileImagePropertiesEXT *properties = (const VkPhysicalDeviceShaderTileImagePropertiesEXT *)pProperties;
         all_properties->shaderTileImageCoherentReadAccelerated = properties->shaderTileImageCoherentReadAccelerated;
         all_properties->shaderTileImageReadSampleFromPixelRateInvocation = properties->shaderTileImageReadSampleFromPixelRateInvocation;
         all_properties->shaderTileImageReadFromHelperInvocation = properties->shaderTileImageReadFromHelperInvocation;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_COOPERATIVE_MATRIX_PROPERTIES_KHR: {
         const VkPhysicalDeviceCooperativeMatrixPropertiesKHR *properties = (const VkPhysicalDeviceCooperativeMatrixPropertiesKHR *)pProperties;
         all_properties->cooperativeMatrixSupportedStages = properties->cooperativeMatrixSupportedStages;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_IMAGE_PROCESSING_2_PROPERTIES_QCOM: {
         const VkPhysicalDeviceImageProcessing2PropertiesQCOM *properties = (const VkPhysicalDeviceImageProcessing2PropertiesQCOM *)pProperties;
         all_properties->maxBlockMatchWindow = properties->maxBlockMatchWindow;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_LAYERED_DRIVER_PROPERTIES_MSFT: {
         const VkPhysicalDeviceLayeredDriverPropertiesMSFT *properties = (const VkPhysicalDeviceLayeredDriverPropertiesMSFT *)pProperties;
         all_properties->underlyingAPI = properties->underlyingAPI;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SCHEDULING_CONTROLS_PROPERTIES_ARM: {
         const VkPhysicalDeviceSchedulingControlsPropertiesARM *properties = (const VkPhysicalDeviceSchedulingControlsPropertiesARM *)pProperties;
         all_properties->schedulingControlsFlags = properties->schedulingControlsFlags;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RENDER_PASS_STRIPED_PROPERTIES_ARM: {
         const VkPhysicalDeviceRenderPassStripedPropertiesARM *properties = (const VkPhysicalDeviceRenderPassStripedPropertiesARM *)pProperties;
         all_properties->renderPassStripeGranularity = properties->renderPassStripeGranularity;
         all_properties->maxRenderPassStripes = properties->maxRenderPassStripes;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MAP_MEMORY_PLACED_PROPERTIES_EXT: {
         const VkPhysicalDeviceMapMemoryPlacedPropertiesEXT *properties = (const VkPhysicalDeviceMapMemoryPlacedPropertiesEXT *)pProperties;
         all_properties->minPlacedMemoryMapAlignment = properties->minPlacedMemoryMapAlignment;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_IMAGE_ALIGNMENT_CONTROL_PROPERTIES_MESA: {
         const VkPhysicalDeviceImageAlignmentControlPropertiesMESA *properties = (const VkPhysicalDeviceImageAlignmentControlPropertiesMESA *)pProperties;
         all_properties->supportedImageAlignmentMask = properties->supportedImageAlignmentMask;
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_COOPERATIVE_MATRIX_2_PROPERTIES_NV: {
         const VkPhysicalDeviceCooperativeMatrix2PropertiesNV *properties = (const VkPhysicalDeviceCooperativeMatrix2PropertiesNV *)pProperties;
         all_properties->cooperativeMatrixWorkgroupScopeMaxWorkgroupSize = properties->cooperativeMatrixWorkgroupScopeMaxWorkgroupSize;
         all_properties->cooperativeMatrixFlexibleDimensionsMaxDimension = properties->cooperativeMatrixFlexibleDimensionsMaxDimension;
         all_properties->cooperativeMatrixWorkgroupScopeReservedSharedMemory = properties->cooperativeMatrixWorkgroupScopeReservedSharedMemory;
         break;
      }

      /* Don't assume anything with this struct type, and just copy things over */

      default:
         break;
      }
}

