
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

/* This file generated from vk_physical_device_features.py, don't edit directly. */

#include "vk_log.h"
#include "vk_physical_device.h"
#include "vk_util.h"

static VkResult
check_physical_device_features(struct vk_physical_device *physical_device,
                               const VkPhysicalDeviceFeatures *supported,
                               const VkPhysicalDeviceFeatures *enabled,
                               const char *struct_name)
{
   if (enabled->robustBufferAccess && !supported->robustBufferAccess)
      return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                       "%s.%s not supported", struct_name, "robustBufferAccess");
   if (enabled->fullDrawIndexUint32 && !supported->fullDrawIndexUint32)
      return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                       "%s.%s not supported", struct_name, "fullDrawIndexUint32");
   if (enabled->imageCubeArray && !supported->imageCubeArray)
      return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                       "%s.%s not supported", struct_name, "imageCubeArray");
   if (enabled->independentBlend && !supported->independentBlend)
      return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                       "%s.%s not supported", struct_name, "independentBlend");
   if (enabled->geometryShader && !supported->geometryShader)
      return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                       "%s.%s not supported", struct_name, "geometryShader");
   if (enabled->tessellationShader && !supported->tessellationShader)
      return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                       "%s.%s not supported", struct_name, "tessellationShader");
   if (enabled->sampleRateShading && !supported->sampleRateShading)
      return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                       "%s.%s not supported", struct_name, "sampleRateShading");
   if (enabled->dualSrcBlend && !supported->dualSrcBlend)
      return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                       "%s.%s not supported", struct_name, "dualSrcBlend");
   if (enabled->logicOp && !supported->logicOp)
      return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                       "%s.%s not supported", struct_name, "logicOp");
   if (enabled->multiDrawIndirect && !supported->multiDrawIndirect)
      return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                       "%s.%s not supported", struct_name, "multiDrawIndirect");
   if (enabled->drawIndirectFirstInstance && !supported->drawIndirectFirstInstance)
      return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                       "%s.%s not supported", struct_name, "drawIndirectFirstInstance");
   if (enabled->depthClamp && !supported->depthClamp)
      return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                       "%s.%s not supported", struct_name, "depthClamp");
   if (enabled->depthBiasClamp && !supported->depthBiasClamp)
      return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                       "%s.%s not supported", struct_name, "depthBiasClamp");
   if (enabled->fillModeNonSolid && !supported->fillModeNonSolid)
      return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                       "%s.%s not supported", struct_name, "fillModeNonSolid");
   if (enabled->depthBounds && !supported->depthBounds)
      return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                       "%s.%s not supported", struct_name, "depthBounds");
   if (enabled->wideLines && !supported->wideLines)
      return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                       "%s.%s not supported", struct_name, "wideLines");
   if (enabled->largePoints && !supported->largePoints)
      return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                       "%s.%s not supported", struct_name, "largePoints");
   if (enabled->alphaToOne && !supported->alphaToOne)
      return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                       "%s.%s not supported", struct_name, "alphaToOne");
   if (enabled->multiViewport && !supported->multiViewport)
      return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                       "%s.%s not supported", struct_name, "multiViewport");
   if (enabled->samplerAnisotropy && !supported->samplerAnisotropy)
      return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                       "%s.%s not supported", struct_name, "samplerAnisotropy");
   if (enabled->textureCompressionETC2 && !supported->textureCompressionETC2)
      return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                       "%s.%s not supported", struct_name, "textureCompressionETC2");
   if (enabled->textureCompressionASTC_LDR && !supported->textureCompressionASTC_LDR)
      return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                       "%s.%s not supported", struct_name, "textureCompressionASTC_LDR");
   if (enabled->textureCompressionBC && !supported->textureCompressionBC)
      return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                       "%s.%s not supported", struct_name, "textureCompressionBC");
   if (enabled->occlusionQueryPrecise && !supported->occlusionQueryPrecise)
      return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                       "%s.%s not supported", struct_name, "occlusionQueryPrecise");
   if (enabled->pipelineStatisticsQuery && !supported->pipelineStatisticsQuery)
      return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                       "%s.%s not supported", struct_name, "pipelineStatisticsQuery");
   if (enabled->vertexPipelineStoresAndAtomics && !supported->vertexPipelineStoresAndAtomics)
      return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                       "%s.%s not supported", struct_name, "vertexPipelineStoresAndAtomics");
   if (enabled->fragmentStoresAndAtomics && !supported->fragmentStoresAndAtomics)
      return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                       "%s.%s not supported", struct_name, "fragmentStoresAndAtomics");
   if (enabled->shaderTessellationAndGeometryPointSize && !supported->shaderTessellationAndGeometryPointSize)
      return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                       "%s.%s not supported", struct_name, "shaderTessellationAndGeometryPointSize");
   if (enabled->shaderImageGatherExtended && !supported->shaderImageGatherExtended)
      return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                       "%s.%s not supported", struct_name, "shaderImageGatherExtended");
   if (enabled->shaderStorageImageExtendedFormats && !supported->shaderStorageImageExtendedFormats)
      return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                       "%s.%s not supported", struct_name, "shaderStorageImageExtendedFormats");
   if (enabled->shaderStorageImageMultisample && !supported->shaderStorageImageMultisample)
      return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                       "%s.%s not supported", struct_name, "shaderStorageImageMultisample");
   if (enabled->shaderStorageImageReadWithoutFormat && !supported->shaderStorageImageReadWithoutFormat)
      return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                       "%s.%s not supported", struct_name, "shaderStorageImageReadWithoutFormat");
   if (enabled->shaderStorageImageWriteWithoutFormat && !supported->shaderStorageImageWriteWithoutFormat)
      return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                       "%s.%s not supported", struct_name, "shaderStorageImageWriteWithoutFormat");
   if (enabled->shaderUniformBufferArrayDynamicIndexing && !supported->shaderUniformBufferArrayDynamicIndexing)
      return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                       "%s.%s not supported", struct_name, "shaderUniformBufferArrayDynamicIndexing");
   if (enabled->shaderSampledImageArrayDynamicIndexing && !supported->shaderSampledImageArrayDynamicIndexing)
      return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                       "%s.%s not supported", struct_name, "shaderSampledImageArrayDynamicIndexing");
   if (enabled->shaderStorageBufferArrayDynamicIndexing && !supported->shaderStorageBufferArrayDynamicIndexing)
      return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                       "%s.%s not supported", struct_name, "shaderStorageBufferArrayDynamicIndexing");
   if (enabled->shaderStorageImageArrayDynamicIndexing && !supported->shaderStorageImageArrayDynamicIndexing)
      return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                       "%s.%s not supported", struct_name, "shaderStorageImageArrayDynamicIndexing");
   if (enabled->shaderClipDistance && !supported->shaderClipDistance)
      return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                       "%s.%s not supported", struct_name, "shaderClipDistance");
   if (enabled->shaderCullDistance && !supported->shaderCullDistance)
      return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                       "%s.%s not supported", struct_name, "shaderCullDistance");
   if (enabled->shaderFloat64 && !supported->shaderFloat64)
      return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                       "%s.%s not supported", struct_name, "shaderFloat64");
   if (enabled->shaderInt64 && !supported->shaderInt64)
      return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                       "%s.%s not supported", struct_name, "shaderInt64");
   if (enabled->shaderInt16 && !supported->shaderInt16)
      return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                       "%s.%s not supported", struct_name, "shaderInt16");
   if (enabled->shaderResourceResidency && !supported->shaderResourceResidency)
      return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                       "%s.%s not supported", struct_name, "shaderResourceResidency");
   if (enabled->shaderResourceMinLod && !supported->shaderResourceMinLod)
      return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                       "%s.%s not supported", struct_name, "shaderResourceMinLod");
   if (enabled->sparseBinding && !supported->sparseBinding)
      return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                       "%s.%s not supported", struct_name, "sparseBinding");
   if (enabled->sparseResidencyBuffer && !supported->sparseResidencyBuffer)
      return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                       "%s.%s not supported", struct_name, "sparseResidencyBuffer");
   if (enabled->sparseResidencyImage2D && !supported->sparseResidencyImage2D)
      return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                       "%s.%s not supported", struct_name, "sparseResidencyImage2D");
   if (enabled->sparseResidencyImage3D && !supported->sparseResidencyImage3D)
      return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                       "%s.%s not supported", struct_name, "sparseResidencyImage3D");
   if (enabled->sparseResidency2Samples && !supported->sparseResidency2Samples)
      return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                       "%s.%s not supported", struct_name, "sparseResidency2Samples");
   if (enabled->sparseResidency4Samples && !supported->sparseResidency4Samples)
      return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                       "%s.%s not supported", struct_name, "sparseResidency4Samples");
   if (enabled->sparseResidency8Samples && !supported->sparseResidency8Samples)
      return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                       "%s.%s not supported", struct_name, "sparseResidency8Samples");
   if (enabled->sparseResidency16Samples && !supported->sparseResidency16Samples)
      return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                       "%s.%s not supported", struct_name, "sparseResidency16Samples");
   if (enabled->sparseResidencyAliased && !supported->sparseResidencyAliased)
      return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                       "%s.%s not supported", struct_name, "sparseResidencyAliased");
   if (enabled->variableMultisampleRate && !supported->variableMultisampleRate)
      return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                       "%s.%s not supported", struct_name, "variableMultisampleRate");
   if (enabled->inheritedQueries && !supported->inheritedQueries)
      return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                       "%s.%s not supported", struct_name, "inheritedQueries");

   return VK_SUCCESS;
}

VkResult
vk_physical_device_check_device_features(struct vk_physical_device *physical_device,
                                         const VkDeviceCreateInfo *pCreateInfo)
{
   VkPhysicalDevice vk_physical_device =
      vk_physical_device_to_handle(physical_device);

   /* Query the device what kind of features are supported. */
   VkPhysicalDeviceFeatures2 supported_features2 = {
      .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2,
   };

   VkPhysicalDeviceDeviceGeneratedCommandsFeaturesNV supported_VkPhysicalDeviceDeviceGeneratedCommandsFeaturesNV = { .pNext = NULL };
   VkPhysicalDevicePrivateDataFeaturesEXT supported_VkPhysicalDevicePrivateDataFeaturesEXT = { .pNext = NULL };
   VkPhysicalDeviceVariablePointersFeatures supported_VkPhysicalDeviceVariablePointersFeatures = { .pNext = NULL };
   VkPhysicalDeviceMultiviewFeatures supported_VkPhysicalDeviceMultiviewFeatures = { .pNext = NULL };
   VkPhysicalDevicePresentIdFeaturesKHR supported_VkPhysicalDevicePresentIdFeaturesKHR = { .pNext = NULL };
   VkPhysicalDevicePresentWaitFeaturesKHR supported_VkPhysicalDevicePresentWaitFeaturesKHR = { .pNext = NULL };
   VkPhysicalDevice16BitStorageFeatures supported_VkPhysicalDevice16BitStorageFeatures = { .pNext = NULL };
   VkPhysicalDeviceShaderSubgroupExtendedTypesFeatures supported_VkPhysicalDeviceShaderSubgroupExtendedTypesFeatures = { .pNext = NULL };
   VkPhysicalDeviceSamplerYcbcrConversionFeatures supported_VkPhysicalDeviceSamplerYcbcrConversionFeatures = { .pNext = NULL };
   VkPhysicalDeviceProtectedMemoryFeatures supported_VkPhysicalDeviceProtectedMemoryFeatures = { .pNext = NULL };
   VkPhysicalDeviceBlendOperationAdvancedFeaturesEXT supported_VkPhysicalDeviceBlendOperationAdvancedFeaturesEXT = { .pNext = NULL };
   VkPhysicalDeviceMultiDrawFeaturesEXT supported_VkPhysicalDeviceMultiDrawFeaturesEXT = { .pNext = NULL };
   VkPhysicalDeviceInlineUniformBlockFeaturesEXT supported_VkPhysicalDeviceInlineUniformBlockFeaturesEXT = { .pNext = NULL };
   VkPhysicalDeviceMaintenance4FeaturesKHR supported_VkPhysicalDeviceMaintenance4FeaturesKHR = { .pNext = NULL };
   VkPhysicalDeviceShaderDrawParametersFeatures supported_VkPhysicalDeviceShaderDrawParametersFeatures = { .pNext = NULL };
   VkPhysicalDeviceShaderFloat16Int8Features supported_VkPhysicalDeviceShaderFloat16Int8Features = { .pNext = NULL };
   VkPhysicalDeviceHostQueryResetFeatures supported_VkPhysicalDeviceHostQueryResetFeatures = { .pNext = NULL };
   VkPhysicalDeviceGlobalPriorityQueryFeaturesEXT supported_VkPhysicalDeviceGlobalPriorityQueryFeaturesEXT = { .pNext = NULL };
   VkPhysicalDeviceDeviceMemoryReportFeaturesEXT supported_VkPhysicalDeviceDeviceMemoryReportFeaturesEXT = { .pNext = NULL };
   VkPhysicalDeviceDescriptorIndexingFeatures supported_VkPhysicalDeviceDescriptorIndexingFeatures = { .pNext = NULL };
   VkPhysicalDeviceTimelineSemaphoreFeatures supported_VkPhysicalDeviceTimelineSemaphoreFeatures = { .pNext = NULL };
   VkPhysicalDevice8BitStorageFeatures supported_VkPhysicalDevice8BitStorageFeatures = { .pNext = NULL };
   VkPhysicalDeviceConditionalRenderingFeaturesEXT supported_VkPhysicalDeviceConditionalRenderingFeaturesEXT = { .pNext = NULL };
   VkPhysicalDeviceVulkanMemoryModelFeatures supported_VkPhysicalDeviceVulkanMemoryModelFeatures = { .pNext = NULL };
   VkPhysicalDeviceShaderAtomicInt64Features supported_VkPhysicalDeviceShaderAtomicInt64Features = { .pNext = NULL };
   VkPhysicalDeviceShaderAtomicFloatFeaturesEXT supported_VkPhysicalDeviceShaderAtomicFloatFeaturesEXT = { .pNext = NULL };
   VkPhysicalDeviceShaderAtomicFloat2FeaturesEXT supported_VkPhysicalDeviceShaderAtomicFloat2FeaturesEXT = { .pNext = NULL };
   VkPhysicalDeviceVertexAttributeDivisorFeaturesEXT supported_VkPhysicalDeviceVertexAttributeDivisorFeaturesEXT = { .pNext = NULL };
   VkPhysicalDeviceASTCDecodeFeaturesEXT supported_VkPhysicalDeviceASTCDecodeFeaturesEXT = { .pNext = NULL };
   VkPhysicalDeviceTransformFeedbackFeaturesEXT supported_VkPhysicalDeviceTransformFeedbackFeaturesEXT = { .pNext = NULL };
   VkPhysicalDeviceRepresentativeFragmentTestFeaturesNV supported_VkPhysicalDeviceRepresentativeFragmentTestFeaturesNV = { .pNext = NULL };
   VkPhysicalDeviceExclusiveScissorFeaturesNV supported_VkPhysicalDeviceExclusiveScissorFeaturesNV = { .pNext = NULL };
   VkPhysicalDeviceCornerSampledImageFeaturesNV supported_VkPhysicalDeviceCornerSampledImageFeaturesNV = { .pNext = NULL };
   VkPhysicalDeviceComputeShaderDerivativesFeaturesNV supported_VkPhysicalDeviceComputeShaderDerivativesFeaturesNV = { .pNext = NULL };
   VkPhysicalDeviceFragmentShaderBarycentricFeaturesNV supported_VkPhysicalDeviceFragmentShaderBarycentricFeaturesNV = { .pNext = NULL };
   VkPhysicalDeviceShaderImageFootprintFeaturesNV supported_VkPhysicalDeviceShaderImageFootprintFeaturesNV = { .pNext = NULL };
   VkPhysicalDeviceDedicatedAllocationImageAliasingFeaturesNV supported_VkPhysicalDeviceDedicatedAllocationImageAliasingFeaturesNV = { .pNext = NULL };
   VkPhysicalDeviceShadingRateImageFeaturesNV supported_VkPhysicalDeviceShadingRateImageFeaturesNV = { .pNext = NULL };
   VkPhysicalDeviceInvocationMaskFeaturesHUAWEI supported_VkPhysicalDeviceInvocationMaskFeaturesHUAWEI = { .pNext = NULL };
   VkPhysicalDeviceMeshShaderFeaturesNV supported_VkPhysicalDeviceMeshShaderFeaturesNV = { .pNext = NULL };
   VkPhysicalDeviceAccelerationStructureFeaturesKHR supported_VkPhysicalDeviceAccelerationStructureFeaturesKHR = { .pNext = NULL };
   VkPhysicalDeviceRayTracingPipelineFeaturesKHR supported_VkPhysicalDeviceRayTracingPipelineFeaturesKHR = { .pNext = NULL };
   VkPhysicalDeviceRayQueryFeaturesKHR supported_VkPhysicalDeviceRayQueryFeaturesKHR = { .pNext = NULL };
   VkPhysicalDeviceFragmentDensityMapFeaturesEXT supported_VkPhysicalDeviceFragmentDensityMapFeaturesEXT = { .pNext = NULL };
   VkPhysicalDeviceFragmentDensityMap2FeaturesEXT supported_VkPhysicalDeviceFragmentDensityMap2FeaturesEXT = { .pNext = NULL };
   VkPhysicalDeviceScalarBlockLayoutFeatures supported_VkPhysicalDeviceScalarBlockLayoutFeatures = { .pNext = NULL };
   VkPhysicalDeviceUniformBufferStandardLayoutFeatures supported_VkPhysicalDeviceUniformBufferStandardLayoutFeatures = { .pNext = NULL };
   VkPhysicalDeviceDepthClipEnableFeaturesEXT supported_VkPhysicalDeviceDepthClipEnableFeaturesEXT = { .pNext = NULL };
   VkPhysicalDeviceMemoryPriorityFeaturesEXT supported_VkPhysicalDeviceMemoryPriorityFeaturesEXT = { .pNext = NULL };
   VkPhysicalDevicePageableDeviceLocalMemoryFeaturesEXT supported_VkPhysicalDevicePageableDeviceLocalMemoryFeaturesEXT = { .pNext = NULL };
   VkPhysicalDeviceBufferDeviceAddressFeatures supported_VkPhysicalDeviceBufferDeviceAddressFeatures = { .pNext = NULL };
   VkPhysicalDeviceBufferDeviceAddressFeaturesEXT supported_VkPhysicalDeviceBufferDeviceAddressFeaturesEXT = { .pNext = NULL };
   VkPhysicalDeviceImagelessFramebufferFeatures supported_VkPhysicalDeviceImagelessFramebufferFeatures = { .pNext = NULL };
   VkPhysicalDeviceTextureCompressionASTCHDRFeaturesEXT supported_VkPhysicalDeviceTextureCompressionASTCHDRFeaturesEXT = { .pNext = NULL };
   VkPhysicalDeviceCooperativeMatrixFeaturesNV supported_VkPhysicalDeviceCooperativeMatrixFeaturesNV = { .pNext = NULL };
   VkPhysicalDeviceYcbcrImageArraysFeaturesEXT supported_VkPhysicalDeviceYcbcrImageArraysFeaturesEXT = { .pNext = NULL };
   VkPhysicalDevicePerformanceQueryFeaturesKHR supported_VkPhysicalDevicePerformanceQueryFeaturesKHR = { .pNext = NULL };
   VkPhysicalDeviceCoverageReductionModeFeaturesNV supported_VkPhysicalDeviceCoverageReductionModeFeaturesNV = { .pNext = NULL };
   VkPhysicalDeviceShaderIntegerFunctions2FeaturesINTEL supported_VkPhysicalDeviceShaderIntegerFunctions2FeaturesINTEL = { .pNext = NULL };
   VkPhysicalDeviceShaderClockFeaturesKHR supported_VkPhysicalDeviceShaderClockFeaturesKHR = { .pNext = NULL };
   VkPhysicalDeviceIndexTypeUint8FeaturesEXT supported_VkPhysicalDeviceIndexTypeUint8FeaturesEXT = { .pNext = NULL };
   VkPhysicalDeviceShaderSMBuiltinsFeaturesNV supported_VkPhysicalDeviceShaderSMBuiltinsFeaturesNV = { .pNext = NULL };
   VkPhysicalDeviceFragmentShaderInterlockFeaturesEXT supported_VkPhysicalDeviceFragmentShaderInterlockFeaturesEXT = { .pNext = NULL };
   VkPhysicalDeviceSeparateDepthStencilLayoutsFeatures supported_VkPhysicalDeviceSeparateDepthStencilLayoutsFeatures = { .pNext = NULL };
   VkPhysicalDevicePrimitiveTopologyListRestartFeaturesEXT supported_VkPhysicalDevicePrimitiveTopologyListRestartFeaturesEXT = { .pNext = NULL };
   VkPhysicalDevicePipelineExecutablePropertiesFeaturesKHR supported_VkPhysicalDevicePipelineExecutablePropertiesFeaturesKHR = { .pNext = NULL };
   VkPhysicalDeviceShaderDemoteToHelperInvocationFeaturesEXT supported_VkPhysicalDeviceShaderDemoteToHelperInvocationFeaturesEXT = { .pNext = NULL };
   VkPhysicalDeviceTexelBufferAlignmentFeaturesEXT supported_VkPhysicalDeviceTexelBufferAlignmentFeaturesEXT = { .pNext = NULL };
   VkPhysicalDeviceSubgroupSizeControlFeaturesEXT supported_VkPhysicalDeviceSubgroupSizeControlFeaturesEXT = { .pNext = NULL };
   VkPhysicalDeviceLineRasterizationFeaturesEXT supported_VkPhysicalDeviceLineRasterizationFeaturesEXT = { .pNext = NULL };
   VkPhysicalDevicePipelineCreationCacheControlFeaturesEXT supported_VkPhysicalDevicePipelineCreationCacheControlFeaturesEXT = { .pNext = NULL };
   VkPhysicalDeviceVulkan11Features supported_VkPhysicalDeviceVulkan11Features = { .pNext = NULL };
   VkPhysicalDeviceVulkan12Features supported_VkPhysicalDeviceVulkan12Features = { .pNext = NULL };
   VkPhysicalDeviceCoherentMemoryFeaturesAMD supported_VkPhysicalDeviceCoherentMemoryFeaturesAMD = { .pNext = NULL };
   VkPhysicalDeviceCustomBorderColorFeaturesEXT supported_VkPhysicalDeviceCustomBorderColorFeaturesEXT = { .pNext = NULL };
   VkPhysicalDeviceExtendedDynamicStateFeaturesEXT supported_VkPhysicalDeviceExtendedDynamicStateFeaturesEXT = { .pNext = NULL };
   VkPhysicalDeviceExtendedDynamicState2FeaturesEXT supported_VkPhysicalDeviceExtendedDynamicState2FeaturesEXT = { .pNext = NULL };
   VkPhysicalDeviceDiagnosticsConfigFeaturesNV supported_VkPhysicalDeviceDiagnosticsConfigFeaturesNV = { .pNext = NULL };
   VkPhysicalDeviceZeroInitializeWorkgroupMemoryFeaturesKHR supported_VkPhysicalDeviceZeroInitializeWorkgroupMemoryFeaturesKHR = { .pNext = NULL };
   VkPhysicalDeviceShaderSubgroupUniformControlFlowFeaturesKHR supported_VkPhysicalDeviceShaderSubgroupUniformControlFlowFeaturesKHR = { .pNext = NULL };
   VkPhysicalDeviceRobustness2FeaturesEXT supported_VkPhysicalDeviceRobustness2FeaturesEXT = { .pNext = NULL };
   VkPhysicalDeviceImageRobustnessFeaturesEXT supported_VkPhysicalDeviceImageRobustnessFeaturesEXT = { .pNext = NULL };
   VkPhysicalDeviceWorkgroupMemoryExplicitLayoutFeaturesKHR supported_VkPhysicalDeviceWorkgroupMemoryExplicitLayoutFeaturesKHR = { .pNext = NULL };
   VkPhysicalDevice4444FormatsFeaturesEXT supported_VkPhysicalDevice4444FormatsFeaturesEXT = { .pNext = NULL };
   VkPhysicalDeviceSubpassShadingFeaturesHUAWEI supported_VkPhysicalDeviceSubpassShadingFeaturesHUAWEI = { .pNext = NULL };
   VkPhysicalDeviceShaderImageAtomicInt64FeaturesEXT supported_VkPhysicalDeviceShaderImageAtomicInt64FeaturesEXT = { .pNext = NULL };
   VkPhysicalDeviceFragmentShadingRateFeaturesKHR supported_VkPhysicalDeviceFragmentShadingRateFeaturesKHR = { .pNext = NULL };
   VkPhysicalDeviceShaderTerminateInvocationFeaturesKHR supported_VkPhysicalDeviceShaderTerminateInvocationFeaturesKHR = { .pNext = NULL };
   VkPhysicalDeviceFragmentShadingRateEnumsFeaturesNV supported_VkPhysicalDeviceFragmentShadingRateEnumsFeaturesNV = { .pNext = NULL };
   VkPhysicalDeviceMutableDescriptorTypeFeaturesVALVE supported_VkPhysicalDeviceMutableDescriptorTypeFeaturesVALVE = { .pNext = NULL };
   VkPhysicalDeviceVertexInputDynamicStateFeaturesEXT supported_VkPhysicalDeviceVertexInputDynamicStateFeaturesEXT = { .pNext = NULL };
   VkPhysicalDeviceExternalMemoryRDMAFeaturesNV supported_VkPhysicalDeviceExternalMemoryRDMAFeaturesNV = { .pNext = NULL };
   VkPhysicalDeviceColorWriteEnableFeaturesEXT supported_VkPhysicalDeviceColorWriteEnableFeaturesEXT = { .pNext = NULL };
   VkPhysicalDeviceSynchronization2FeaturesKHR supported_VkPhysicalDeviceSynchronization2FeaturesKHR = { .pNext = NULL };
   VkPhysicalDeviceInheritedViewportScissorFeaturesNV supported_VkPhysicalDeviceInheritedViewportScissorFeaturesNV = { .pNext = NULL };
   VkPhysicalDeviceYcbcr2Plane444FormatsFeaturesEXT supported_VkPhysicalDeviceYcbcr2Plane444FormatsFeaturesEXT = { .pNext = NULL };
   VkPhysicalDeviceProvokingVertexFeaturesEXT supported_VkPhysicalDeviceProvokingVertexFeaturesEXT = { .pNext = NULL };
   VkPhysicalDeviceShaderIntegerDotProductFeaturesKHR supported_VkPhysicalDeviceShaderIntegerDotProductFeaturesKHR = { .pNext = NULL };
   VkPhysicalDeviceRayTracingMotionBlurFeaturesNV supported_VkPhysicalDeviceRayTracingMotionBlurFeaturesNV = { .pNext = NULL };
   VkPhysicalDeviceRGBA10X6FormatsFeaturesEXT supported_VkPhysicalDeviceRGBA10X6FormatsFeaturesEXT = { .pNext = NULL };

   vk_foreach_struct_const(feat, pCreateInfo->pNext) {
      VkBaseOutStructure *supported = NULL;
      switch (feat->sType) {
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DEVICE_GENERATED_COMMANDS_FEATURES_NV:
         supported = (VkBaseOutStructure *) &supported_VkPhysicalDeviceDeviceGeneratedCommandsFeaturesNV;
         break;
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PRIVATE_DATA_FEATURES_EXT:
         supported = (VkBaseOutStructure *) &supported_VkPhysicalDevicePrivateDataFeaturesEXT;
         break;
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VARIABLE_POINTERS_FEATURES:
         supported = (VkBaseOutStructure *) &supported_VkPhysicalDeviceVariablePointersFeatures;
         break;
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MULTIVIEW_FEATURES:
         supported = (VkBaseOutStructure *) &supported_VkPhysicalDeviceMultiviewFeatures;
         break;
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PRESENT_ID_FEATURES_KHR:
         supported = (VkBaseOutStructure *) &supported_VkPhysicalDevicePresentIdFeaturesKHR;
         break;
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PRESENT_WAIT_FEATURES_KHR:
         supported = (VkBaseOutStructure *) &supported_VkPhysicalDevicePresentWaitFeaturesKHR;
         break;
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_16BIT_STORAGE_FEATURES:
         supported = (VkBaseOutStructure *) &supported_VkPhysicalDevice16BitStorageFeatures;
         break;
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_SUBGROUP_EXTENDED_TYPES_FEATURES:
         supported = (VkBaseOutStructure *) &supported_VkPhysicalDeviceShaderSubgroupExtendedTypesFeatures;
         break;
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SAMPLER_YCBCR_CONVERSION_FEATURES:
         supported = (VkBaseOutStructure *) &supported_VkPhysicalDeviceSamplerYcbcrConversionFeatures;
         break;
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROTECTED_MEMORY_FEATURES:
         supported = (VkBaseOutStructure *) &supported_VkPhysicalDeviceProtectedMemoryFeatures;
         break;
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_BLEND_OPERATION_ADVANCED_FEATURES_EXT:
         supported = (VkBaseOutStructure *) &supported_VkPhysicalDeviceBlendOperationAdvancedFeaturesEXT;
         break;
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MULTI_DRAW_FEATURES_EXT:
         supported = (VkBaseOutStructure *) &supported_VkPhysicalDeviceMultiDrawFeaturesEXT;
         break;
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_INLINE_UNIFORM_BLOCK_FEATURES_EXT:
         supported = (VkBaseOutStructure *) &supported_VkPhysicalDeviceInlineUniformBlockFeaturesEXT;
         break;
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MAINTENANCE_4_FEATURES_KHR:
         supported = (VkBaseOutStructure *) &supported_VkPhysicalDeviceMaintenance4FeaturesKHR;
         break;
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_DRAW_PARAMETERS_FEATURES:
         supported = (VkBaseOutStructure *) &supported_VkPhysicalDeviceShaderDrawParametersFeatures;
         break;
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_FLOAT16_INT8_FEATURES:
         supported = (VkBaseOutStructure *) &supported_VkPhysicalDeviceShaderFloat16Int8Features;
         break;
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_HOST_QUERY_RESET_FEATURES:
         supported = (VkBaseOutStructure *) &supported_VkPhysicalDeviceHostQueryResetFeatures;
         break;
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_GLOBAL_PRIORITY_QUERY_FEATURES_EXT:
         supported = (VkBaseOutStructure *) &supported_VkPhysicalDeviceGlobalPriorityQueryFeaturesEXT;
         break;
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DEVICE_MEMORY_REPORT_FEATURES_EXT:
         supported = (VkBaseOutStructure *) &supported_VkPhysicalDeviceDeviceMemoryReportFeaturesEXT;
         break;
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_INDEXING_FEATURES:
         supported = (VkBaseOutStructure *) &supported_VkPhysicalDeviceDescriptorIndexingFeatures;
         break;
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TIMELINE_SEMAPHORE_FEATURES:
         supported = (VkBaseOutStructure *) &supported_VkPhysicalDeviceTimelineSemaphoreFeatures;
         break;
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_8BIT_STORAGE_FEATURES:
         supported = (VkBaseOutStructure *) &supported_VkPhysicalDevice8BitStorageFeatures;
         break;
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_CONDITIONAL_RENDERING_FEATURES_EXT:
         supported = (VkBaseOutStructure *) &supported_VkPhysicalDeviceConditionalRenderingFeaturesEXT;
         break;
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_MEMORY_MODEL_FEATURES:
         supported = (VkBaseOutStructure *) &supported_VkPhysicalDeviceVulkanMemoryModelFeatures;
         break;
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_ATOMIC_INT64_FEATURES:
         supported = (VkBaseOutStructure *) &supported_VkPhysicalDeviceShaderAtomicInt64Features;
         break;
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_ATOMIC_FLOAT_FEATURES_EXT:
         supported = (VkBaseOutStructure *) &supported_VkPhysicalDeviceShaderAtomicFloatFeaturesEXT;
         break;
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_ATOMIC_FLOAT_2_FEATURES_EXT:
         supported = (VkBaseOutStructure *) &supported_VkPhysicalDeviceShaderAtomicFloat2FeaturesEXT;
         break;
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VERTEX_ATTRIBUTE_DIVISOR_FEATURES_EXT:
         supported = (VkBaseOutStructure *) &supported_VkPhysicalDeviceVertexAttributeDivisorFeaturesEXT;
         break;
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ASTC_DECODE_FEATURES_EXT:
         supported = (VkBaseOutStructure *) &supported_VkPhysicalDeviceASTCDecodeFeaturesEXT;
         break;
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TRANSFORM_FEEDBACK_FEATURES_EXT:
         supported = (VkBaseOutStructure *) &supported_VkPhysicalDeviceTransformFeedbackFeaturesEXT;
         break;
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_REPRESENTATIVE_FRAGMENT_TEST_FEATURES_NV:
         supported = (VkBaseOutStructure *) &supported_VkPhysicalDeviceRepresentativeFragmentTestFeaturesNV;
         break;
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXCLUSIVE_SCISSOR_FEATURES_NV:
         supported = (VkBaseOutStructure *) &supported_VkPhysicalDeviceExclusiveScissorFeaturesNV;
         break;
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_CORNER_SAMPLED_IMAGE_FEATURES_NV:
         supported = (VkBaseOutStructure *) &supported_VkPhysicalDeviceCornerSampledImageFeaturesNV;
         break;
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_COMPUTE_SHADER_DERIVATIVES_FEATURES_NV:
         supported = (VkBaseOutStructure *) &supported_VkPhysicalDeviceComputeShaderDerivativesFeaturesNV;
         break;
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FRAGMENT_SHADER_BARYCENTRIC_FEATURES_NV:
         supported = (VkBaseOutStructure *) &supported_VkPhysicalDeviceFragmentShaderBarycentricFeaturesNV;
         break;
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_IMAGE_FOOTPRINT_FEATURES_NV:
         supported = (VkBaseOutStructure *) &supported_VkPhysicalDeviceShaderImageFootprintFeaturesNV;
         break;
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DEDICATED_ALLOCATION_IMAGE_ALIASING_FEATURES_NV:
         supported = (VkBaseOutStructure *) &supported_VkPhysicalDeviceDedicatedAllocationImageAliasingFeaturesNV;
         break;
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADING_RATE_IMAGE_FEATURES_NV:
         supported = (VkBaseOutStructure *) &supported_VkPhysicalDeviceShadingRateImageFeaturesNV;
         break;
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_INVOCATION_MASK_FEATURES_HUAWEI:
         supported = (VkBaseOutStructure *) &supported_VkPhysicalDeviceInvocationMaskFeaturesHUAWEI;
         break;
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MESH_SHADER_FEATURES_NV:
         supported = (VkBaseOutStructure *) &supported_VkPhysicalDeviceMeshShaderFeaturesNV;
         break;
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ACCELERATION_STRUCTURE_FEATURES_KHR:
         supported = (VkBaseOutStructure *) &supported_VkPhysicalDeviceAccelerationStructureFeaturesKHR;
         break;
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_PIPELINE_FEATURES_KHR:
         supported = (VkBaseOutStructure *) &supported_VkPhysicalDeviceRayTracingPipelineFeaturesKHR;
         break;
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_QUERY_FEATURES_KHR:
         supported = (VkBaseOutStructure *) &supported_VkPhysicalDeviceRayQueryFeaturesKHR;
         break;
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FRAGMENT_DENSITY_MAP_FEATURES_EXT:
         supported = (VkBaseOutStructure *) &supported_VkPhysicalDeviceFragmentDensityMapFeaturesEXT;
         break;
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FRAGMENT_DENSITY_MAP_2_FEATURES_EXT:
         supported = (VkBaseOutStructure *) &supported_VkPhysicalDeviceFragmentDensityMap2FeaturesEXT;
         break;
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SCALAR_BLOCK_LAYOUT_FEATURES:
         supported = (VkBaseOutStructure *) &supported_VkPhysicalDeviceScalarBlockLayoutFeatures;
         break;
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_UNIFORM_BUFFER_STANDARD_LAYOUT_FEATURES:
         supported = (VkBaseOutStructure *) &supported_VkPhysicalDeviceUniformBufferStandardLayoutFeatures;
         break;
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DEPTH_CLIP_ENABLE_FEATURES_EXT:
         supported = (VkBaseOutStructure *) &supported_VkPhysicalDeviceDepthClipEnableFeaturesEXT;
         break;
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MEMORY_PRIORITY_FEATURES_EXT:
         supported = (VkBaseOutStructure *) &supported_VkPhysicalDeviceMemoryPriorityFeaturesEXT;
         break;
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PAGEABLE_DEVICE_LOCAL_MEMORY_FEATURES_EXT:
         supported = (VkBaseOutStructure *) &supported_VkPhysicalDevicePageableDeviceLocalMemoryFeaturesEXT;
         break;
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_BUFFER_DEVICE_ADDRESS_FEATURES:
         supported = (VkBaseOutStructure *) &supported_VkPhysicalDeviceBufferDeviceAddressFeatures;
         break;
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_BUFFER_DEVICE_ADDRESS_FEATURES_EXT:
         supported = (VkBaseOutStructure *) &supported_VkPhysicalDeviceBufferDeviceAddressFeaturesEXT;
         break;
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_IMAGELESS_FRAMEBUFFER_FEATURES:
         supported = (VkBaseOutStructure *) &supported_VkPhysicalDeviceImagelessFramebufferFeatures;
         break;
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TEXTURE_COMPRESSION_ASTC_HDR_FEATURES_EXT:
         supported = (VkBaseOutStructure *) &supported_VkPhysicalDeviceTextureCompressionASTCHDRFeaturesEXT;
         break;
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_COOPERATIVE_MATRIX_FEATURES_NV:
         supported = (VkBaseOutStructure *) &supported_VkPhysicalDeviceCooperativeMatrixFeaturesNV;
         break;
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_YCBCR_IMAGE_ARRAYS_FEATURES_EXT:
         supported = (VkBaseOutStructure *) &supported_VkPhysicalDeviceYcbcrImageArraysFeaturesEXT;
         break;
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PERFORMANCE_QUERY_FEATURES_KHR:
         supported = (VkBaseOutStructure *) &supported_VkPhysicalDevicePerformanceQueryFeaturesKHR;
         break;
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_COVERAGE_REDUCTION_MODE_FEATURES_NV:
         supported = (VkBaseOutStructure *) &supported_VkPhysicalDeviceCoverageReductionModeFeaturesNV;
         break;
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_INTEGER_FUNCTIONS_2_FEATURES_INTEL:
         supported = (VkBaseOutStructure *) &supported_VkPhysicalDeviceShaderIntegerFunctions2FeaturesINTEL;
         break;
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_CLOCK_FEATURES_KHR:
         supported = (VkBaseOutStructure *) &supported_VkPhysicalDeviceShaderClockFeaturesKHR;
         break;
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_INDEX_TYPE_UINT8_FEATURES_EXT:
         supported = (VkBaseOutStructure *) &supported_VkPhysicalDeviceIndexTypeUint8FeaturesEXT;
         break;
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_SM_BUILTINS_FEATURES_NV:
         supported = (VkBaseOutStructure *) &supported_VkPhysicalDeviceShaderSMBuiltinsFeaturesNV;
         break;
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FRAGMENT_SHADER_INTERLOCK_FEATURES_EXT:
         supported = (VkBaseOutStructure *) &supported_VkPhysicalDeviceFragmentShaderInterlockFeaturesEXT;
         break;
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SEPARATE_DEPTH_STENCIL_LAYOUTS_FEATURES:
         supported = (VkBaseOutStructure *) &supported_VkPhysicalDeviceSeparateDepthStencilLayoutsFeatures;
         break;
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PRIMITIVE_TOPOLOGY_LIST_RESTART_FEATURES_EXT:
         supported = (VkBaseOutStructure *) &supported_VkPhysicalDevicePrimitiveTopologyListRestartFeaturesEXT;
         break;
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PIPELINE_EXECUTABLE_PROPERTIES_FEATURES_KHR:
         supported = (VkBaseOutStructure *) &supported_VkPhysicalDevicePipelineExecutablePropertiesFeaturesKHR;
         break;
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_DEMOTE_TO_HELPER_INVOCATION_FEATURES_EXT:
         supported = (VkBaseOutStructure *) &supported_VkPhysicalDeviceShaderDemoteToHelperInvocationFeaturesEXT;
         break;
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TEXEL_BUFFER_ALIGNMENT_FEATURES_EXT:
         supported = (VkBaseOutStructure *) &supported_VkPhysicalDeviceTexelBufferAlignmentFeaturesEXT;
         break;
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SUBGROUP_SIZE_CONTROL_FEATURES_EXT:
         supported = (VkBaseOutStructure *) &supported_VkPhysicalDeviceSubgroupSizeControlFeaturesEXT;
         break;
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_LINE_RASTERIZATION_FEATURES_EXT:
         supported = (VkBaseOutStructure *) &supported_VkPhysicalDeviceLineRasterizationFeaturesEXT;
         break;
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PIPELINE_CREATION_CACHE_CONTROL_FEATURES_EXT:
         supported = (VkBaseOutStructure *) &supported_VkPhysicalDevicePipelineCreationCacheControlFeaturesEXT;
         break;
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_FEATURES:
         supported = (VkBaseOutStructure *) &supported_VkPhysicalDeviceVulkan11Features;
         break;
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES:
         supported = (VkBaseOutStructure *) &supported_VkPhysicalDeviceVulkan12Features;
         break;
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_COHERENT_MEMORY_FEATURES_AMD:
         supported = (VkBaseOutStructure *) &supported_VkPhysicalDeviceCoherentMemoryFeaturesAMD;
         break;
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_CUSTOM_BORDER_COLOR_FEATURES_EXT:
         supported = (VkBaseOutStructure *) &supported_VkPhysicalDeviceCustomBorderColorFeaturesEXT;
         break;
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTENDED_DYNAMIC_STATE_FEATURES_EXT:
         supported = (VkBaseOutStructure *) &supported_VkPhysicalDeviceExtendedDynamicStateFeaturesEXT;
         break;
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTENDED_DYNAMIC_STATE_2_FEATURES_EXT:
         supported = (VkBaseOutStructure *) &supported_VkPhysicalDeviceExtendedDynamicState2FeaturesEXT;
         break;
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DIAGNOSTICS_CONFIG_FEATURES_NV:
         supported = (VkBaseOutStructure *) &supported_VkPhysicalDeviceDiagnosticsConfigFeaturesNV;
         break;
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ZERO_INITIALIZE_WORKGROUP_MEMORY_FEATURES_KHR:
         supported = (VkBaseOutStructure *) &supported_VkPhysicalDeviceZeroInitializeWorkgroupMemoryFeaturesKHR;
         break;
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_SUBGROUP_UNIFORM_CONTROL_FLOW_FEATURES_KHR:
         supported = (VkBaseOutStructure *) &supported_VkPhysicalDeviceShaderSubgroupUniformControlFlowFeaturesKHR;
         break;
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ROBUSTNESS_2_FEATURES_EXT:
         supported = (VkBaseOutStructure *) &supported_VkPhysicalDeviceRobustness2FeaturesEXT;
         break;
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_IMAGE_ROBUSTNESS_FEATURES_EXT:
         supported = (VkBaseOutStructure *) &supported_VkPhysicalDeviceImageRobustnessFeaturesEXT;
         break;
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_WORKGROUP_MEMORY_EXPLICIT_LAYOUT_FEATURES_KHR:
         supported = (VkBaseOutStructure *) &supported_VkPhysicalDeviceWorkgroupMemoryExplicitLayoutFeaturesKHR;
         break;
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_4444_FORMATS_FEATURES_EXT:
         supported = (VkBaseOutStructure *) &supported_VkPhysicalDevice4444FormatsFeaturesEXT;
         break;
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SUBPASS_SHADING_FEATURES_HUAWEI:
         supported = (VkBaseOutStructure *) &supported_VkPhysicalDeviceSubpassShadingFeaturesHUAWEI;
         break;
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_IMAGE_ATOMIC_INT64_FEATURES_EXT:
         supported = (VkBaseOutStructure *) &supported_VkPhysicalDeviceShaderImageAtomicInt64FeaturesEXT;
         break;
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FRAGMENT_SHADING_RATE_FEATURES_KHR:
         supported = (VkBaseOutStructure *) &supported_VkPhysicalDeviceFragmentShadingRateFeaturesKHR;
         break;
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_TERMINATE_INVOCATION_FEATURES_KHR:
         supported = (VkBaseOutStructure *) &supported_VkPhysicalDeviceShaderTerminateInvocationFeaturesKHR;
         break;
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FRAGMENT_SHADING_RATE_ENUMS_FEATURES_NV:
         supported = (VkBaseOutStructure *) &supported_VkPhysicalDeviceFragmentShadingRateEnumsFeaturesNV;
         break;
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MUTABLE_DESCRIPTOR_TYPE_FEATURES_VALVE:
         supported = (VkBaseOutStructure *) &supported_VkPhysicalDeviceMutableDescriptorTypeFeaturesVALVE;
         break;
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VERTEX_INPUT_DYNAMIC_STATE_FEATURES_EXT:
         supported = (VkBaseOutStructure *) &supported_VkPhysicalDeviceVertexInputDynamicStateFeaturesEXT;
         break;
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTERNAL_MEMORY_RDMA_FEATURES_NV:
         supported = (VkBaseOutStructure *) &supported_VkPhysicalDeviceExternalMemoryRDMAFeaturesNV;
         break;
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_COLOR_WRITE_ENABLE_FEATURES_EXT:
         supported = (VkBaseOutStructure *) &supported_VkPhysicalDeviceColorWriteEnableFeaturesEXT;
         break;
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SYNCHRONIZATION_2_FEATURES_KHR:
         supported = (VkBaseOutStructure *) &supported_VkPhysicalDeviceSynchronization2FeaturesKHR;
         break;
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_INHERITED_VIEWPORT_SCISSOR_FEATURES_NV:
         supported = (VkBaseOutStructure *) &supported_VkPhysicalDeviceInheritedViewportScissorFeaturesNV;
         break;
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_YCBCR_2_PLANE_444_FORMATS_FEATURES_EXT:
         supported = (VkBaseOutStructure *) &supported_VkPhysicalDeviceYcbcr2Plane444FormatsFeaturesEXT;
         break;
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROVOKING_VERTEX_FEATURES_EXT:
         supported = (VkBaseOutStructure *) &supported_VkPhysicalDeviceProvokingVertexFeaturesEXT;
         break;
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_INTEGER_DOT_PRODUCT_FEATURES_KHR:
         supported = (VkBaseOutStructure *) &supported_VkPhysicalDeviceShaderIntegerDotProductFeaturesKHR;
         break;
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_MOTION_BLUR_FEATURES_NV:
         supported = (VkBaseOutStructure *) &supported_VkPhysicalDeviceRayTracingMotionBlurFeaturesNV;
         break;
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RGBA10X6_FORMATS_FEATURES_EXT:
         supported = (VkBaseOutStructure *) &supported_VkPhysicalDeviceRGBA10X6FormatsFeaturesEXT;
         break;
      default:
         break;
      }

      /* Not a feature struct. */
      if (!supported)
         continue;

      /* Check for cycles in the list */
      if (supported->pNext != NULL || supported->sType != 0)
         return VK_ERROR_UNKNOWN;

      supported->sType = feat->sType;
      __vk_append_struct(&supported_features2, supported);
   }

   physical_device->dispatch_table.GetPhysicalDeviceFeatures2(
      vk_physical_device, &supported_features2);

   if (pCreateInfo->pEnabledFeatures) {
      VkResult result =
        check_physical_device_features(physical_device,
                                       &supported_features2.features,
                                       pCreateInfo->pEnabledFeatures,
                                       "VkPhysicalDeviceFeatures");
      if (result != VK_SUCCESS)
         return result;
   }

   /* Iterate through additional feature structs */
   vk_foreach_struct_const(feat, pCreateInfo->pNext) {
      /* Check each feature boolean for given structure. */
      switch (feat->sType) {
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2: {
         const VkPhysicalDeviceFeatures2 *features2 = (const void *)feat;
         VkResult result =
            check_physical_device_features(physical_device,
                                           &supported_features2.features,
                                           &features2->features,
                                           "VkPhysicalDeviceFeatures2.features");
         if (result != VK_SUCCESS)
            return result;
        break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DEVICE_GENERATED_COMMANDS_FEATURES_NV : {
         VkPhysicalDeviceDeviceGeneratedCommandsFeaturesNV *a = &supported_VkPhysicalDeviceDeviceGeneratedCommandsFeaturesNV;
         VkPhysicalDeviceDeviceGeneratedCommandsFeaturesNV *b = (VkPhysicalDeviceDeviceGeneratedCommandsFeaturesNV *) feat;
         if (b->deviceGeneratedCommands && !a->deviceGeneratedCommands)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceDeviceGeneratedCommandsFeaturesNV", "deviceGeneratedCommands");
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PRIVATE_DATA_FEATURES_EXT : {
         VkPhysicalDevicePrivateDataFeaturesEXT *a = &supported_VkPhysicalDevicePrivateDataFeaturesEXT;
         VkPhysicalDevicePrivateDataFeaturesEXT *b = (VkPhysicalDevicePrivateDataFeaturesEXT *) feat;
         if (b->privateData && !a->privateData)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDevicePrivateDataFeaturesEXT", "privateData");
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VARIABLE_POINTERS_FEATURES : {
         VkPhysicalDeviceVariablePointersFeatures *a = &supported_VkPhysicalDeviceVariablePointersFeatures;
         VkPhysicalDeviceVariablePointersFeatures *b = (VkPhysicalDeviceVariablePointersFeatures *) feat;
         if (b->variablePointersStorageBuffer && !a->variablePointersStorageBuffer)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceVariablePointersFeatures", "variablePointersStorageBuffer");
         if (b->variablePointers && !a->variablePointers)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceVariablePointersFeatures", "variablePointers");
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MULTIVIEW_FEATURES : {
         VkPhysicalDeviceMultiviewFeatures *a = &supported_VkPhysicalDeviceMultiviewFeatures;
         VkPhysicalDeviceMultiviewFeatures *b = (VkPhysicalDeviceMultiviewFeatures *) feat;
         if (b->multiview && !a->multiview)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceMultiviewFeatures", "multiview");
         if (b->multiviewGeometryShader && !a->multiviewGeometryShader)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceMultiviewFeatures", "multiviewGeometryShader");
         if (b->multiviewTessellationShader && !a->multiviewTessellationShader)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceMultiviewFeatures", "multiviewTessellationShader");
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PRESENT_ID_FEATURES_KHR : {
         VkPhysicalDevicePresentIdFeaturesKHR *a = &supported_VkPhysicalDevicePresentIdFeaturesKHR;
         VkPhysicalDevicePresentIdFeaturesKHR *b = (VkPhysicalDevicePresentIdFeaturesKHR *) feat;
         if (b->presentId && !a->presentId)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDevicePresentIdFeaturesKHR", "presentId");
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PRESENT_WAIT_FEATURES_KHR : {
         VkPhysicalDevicePresentWaitFeaturesKHR *a = &supported_VkPhysicalDevicePresentWaitFeaturesKHR;
         VkPhysicalDevicePresentWaitFeaturesKHR *b = (VkPhysicalDevicePresentWaitFeaturesKHR *) feat;
         if (b->presentWait && !a->presentWait)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDevicePresentWaitFeaturesKHR", "presentWait");
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_16BIT_STORAGE_FEATURES : {
         VkPhysicalDevice16BitStorageFeatures *a = &supported_VkPhysicalDevice16BitStorageFeatures;
         VkPhysicalDevice16BitStorageFeatures *b = (VkPhysicalDevice16BitStorageFeatures *) feat;
         if (b->storageBuffer16BitAccess && !a->storageBuffer16BitAccess)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDevice16BitStorageFeatures", "storageBuffer16BitAccess");
         if (b->uniformAndStorageBuffer16BitAccess && !a->uniformAndStorageBuffer16BitAccess)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDevice16BitStorageFeatures", "uniformAndStorageBuffer16BitAccess");
         if (b->storagePushConstant16 && !a->storagePushConstant16)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDevice16BitStorageFeatures", "storagePushConstant16");
         if (b->storageInputOutput16 && !a->storageInputOutput16)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDevice16BitStorageFeatures", "storageInputOutput16");
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_SUBGROUP_EXTENDED_TYPES_FEATURES : {
         VkPhysicalDeviceShaderSubgroupExtendedTypesFeatures *a = &supported_VkPhysicalDeviceShaderSubgroupExtendedTypesFeatures;
         VkPhysicalDeviceShaderSubgroupExtendedTypesFeatures *b = (VkPhysicalDeviceShaderSubgroupExtendedTypesFeatures *) feat;
         if (b->shaderSubgroupExtendedTypes && !a->shaderSubgroupExtendedTypes)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceShaderSubgroupExtendedTypesFeatures", "shaderSubgroupExtendedTypes");
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SAMPLER_YCBCR_CONVERSION_FEATURES : {
         VkPhysicalDeviceSamplerYcbcrConversionFeatures *a = &supported_VkPhysicalDeviceSamplerYcbcrConversionFeatures;
         VkPhysicalDeviceSamplerYcbcrConversionFeatures *b = (VkPhysicalDeviceSamplerYcbcrConversionFeatures *) feat;
         if (b->samplerYcbcrConversion && !a->samplerYcbcrConversion)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceSamplerYcbcrConversionFeatures", "samplerYcbcrConversion");
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROTECTED_MEMORY_FEATURES : {
         VkPhysicalDeviceProtectedMemoryFeatures *a = &supported_VkPhysicalDeviceProtectedMemoryFeatures;
         VkPhysicalDeviceProtectedMemoryFeatures *b = (VkPhysicalDeviceProtectedMemoryFeatures *) feat;
         if (b->protectedMemory && !a->protectedMemory)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceProtectedMemoryFeatures", "protectedMemory");
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_BLEND_OPERATION_ADVANCED_FEATURES_EXT : {
         VkPhysicalDeviceBlendOperationAdvancedFeaturesEXT *a = &supported_VkPhysicalDeviceBlendOperationAdvancedFeaturesEXT;
         VkPhysicalDeviceBlendOperationAdvancedFeaturesEXT *b = (VkPhysicalDeviceBlendOperationAdvancedFeaturesEXT *) feat;
         if (b->advancedBlendCoherentOperations && !a->advancedBlendCoherentOperations)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceBlendOperationAdvancedFeaturesEXT", "advancedBlendCoherentOperations");
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MULTI_DRAW_FEATURES_EXT : {
         VkPhysicalDeviceMultiDrawFeaturesEXT *a = &supported_VkPhysicalDeviceMultiDrawFeaturesEXT;
         VkPhysicalDeviceMultiDrawFeaturesEXT *b = (VkPhysicalDeviceMultiDrawFeaturesEXT *) feat;
         if (b->multiDraw && !a->multiDraw)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceMultiDrawFeaturesEXT", "multiDraw");
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_INLINE_UNIFORM_BLOCK_FEATURES_EXT : {
         VkPhysicalDeviceInlineUniformBlockFeaturesEXT *a = &supported_VkPhysicalDeviceInlineUniformBlockFeaturesEXT;
         VkPhysicalDeviceInlineUniformBlockFeaturesEXT *b = (VkPhysicalDeviceInlineUniformBlockFeaturesEXT *) feat;
         if (b->inlineUniformBlock && !a->inlineUniformBlock)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceInlineUniformBlockFeaturesEXT", "inlineUniformBlock");
         if (b->descriptorBindingInlineUniformBlockUpdateAfterBind && !a->descriptorBindingInlineUniformBlockUpdateAfterBind)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceInlineUniformBlockFeaturesEXT", "descriptorBindingInlineUniformBlockUpdateAfterBind");
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MAINTENANCE_4_FEATURES_KHR : {
         VkPhysicalDeviceMaintenance4FeaturesKHR *a = &supported_VkPhysicalDeviceMaintenance4FeaturesKHR;
         VkPhysicalDeviceMaintenance4FeaturesKHR *b = (VkPhysicalDeviceMaintenance4FeaturesKHR *) feat;
         if (b->maintenance4 && !a->maintenance4)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceMaintenance4FeaturesKHR", "maintenance4");
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_DRAW_PARAMETERS_FEATURES : {
         VkPhysicalDeviceShaderDrawParametersFeatures *a = &supported_VkPhysicalDeviceShaderDrawParametersFeatures;
         VkPhysicalDeviceShaderDrawParametersFeatures *b = (VkPhysicalDeviceShaderDrawParametersFeatures *) feat;
         if (b->shaderDrawParameters && !a->shaderDrawParameters)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceShaderDrawParametersFeatures", "shaderDrawParameters");
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_FLOAT16_INT8_FEATURES : {
         VkPhysicalDeviceShaderFloat16Int8Features *a = &supported_VkPhysicalDeviceShaderFloat16Int8Features;
         VkPhysicalDeviceShaderFloat16Int8Features *b = (VkPhysicalDeviceShaderFloat16Int8Features *) feat;
         if (b->shaderFloat16 && !a->shaderFloat16)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceShaderFloat16Int8Features", "shaderFloat16");
         if (b->shaderInt8 && !a->shaderInt8)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceShaderFloat16Int8Features", "shaderInt8");
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_HOST_QUERY_RESET_FEATURES : {
         VkPhysicalDeviceHostQueryResetFeatures *a = &supported_VkPhysicalDeviceHostQueryResetFeatures;
         VkPhysicalDeviceHostQueryResetFeatures *b = (VkPhysicalDeviceHostQueryResetFeatures *) feat;
         if (b->hostQueryReset && !a->hostQueryReset)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceHostQueryResetFeatures", "hostQueryReset");
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_GLOBAL_PRIORITY_QUERY_FEATURES_EXT : {
         VkPhysicalDeviceGlobalPriorityQueryFeaturesEXT *a = &supported_VkPhysicalDeviceGlobalPriorityQueryFeaturesEXT;
         VkPhysicalDeviceGlobalPriorityQueryFeaturesEXT *b = (VkPhysicalDeviceGlobalPriorityQueryFeaturesEXT *) feat;
         if (b->globalPriorityQuery && !a->globalPriorityQuery)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceGlobalPriorityQueryFeaturesEXT", "globalPriorityQuery");
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DEVICE_MEMORY_REPORT_FEATURES_EXT : {
         VkPhysicalDeviceDeviceMemoryReportFeaturesEXT *a = &supported_VkPhysicalDeviceDeviceMemoryReportFeaturesEXT;
         VkPhysicalDeviceDeviceMemoryReportFeaturesEXT *b = (VkPhysicalDeviceDeviceMemoryReportFeaturesEXT *) feat;
         if (b->deviceMemoryReport && !a->deviceMemoryReport)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceDeviceMemoryReportFeaturesEXT", "deviceMemoryReport");
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_INDEXING_FEATURES : {
         VkPhysicalDeviceDescriptorIndexingFeatures *a = &supported_VkPhysicalDeviceDescriptorIndexingFeatures;
         VkPhysicalDeviceDescriptorIndexingFeatures *b = (VkPhysicalDeviceDescriptorIndexingFeatures *) feat;
         if (b->shaderInputAttachmentArrayDynamicIndexing && !a->shaderInputAttachmentArrayDynamicIndexing)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceDescriptorIndexingFeatures", "shaderInputAttachmentArrayDynamicIndexing");
         if (b->shaderUniformTexelBufferArrayDynamicIndexing && !a->shaderUniformTexelBufferArrayDynamicIndexing)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceDescriptorIndexingFeatures", "shaderUniformTexelBufferArrayDynamicIndexing");
         if (b->shaderStorageTexelBufferArrayDynamicIndexing && !a->shaderStorageTexelBufferArrayDynamicIndexing)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceDescriptorIndexingFeatures", "shaderStorageTexelBufferArrayDynamicIndexing");
         if (b->shaderUniformBufferArrayNonUniformIndexing && !a->shaderUniformBufferArrayNonUniformIndexing)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceDescriptorIndexingFeatures", "shaderUniformBufferArrayNonUniformIndexing");
         if (b->shaderSampledImageArrayNonUniformIndexing && !a->shaderSampledImageArrayNonUniformIndexing)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceDescriptorIndexingFeatures", "shaderSampledImageArrayNonUniformIndexing");
         if (b->shaderStorageBufferArrayNonUniformIndexing && !a->shaderStorageBufferArrayNonUniformIndexing)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceDescriptorIndexingFeatures", "shaderStorageBufferArrayNonUniformIndexing");
         if (b->shaderStorageImageArrayNonUniformIndexing && !a->shaderStorageImageArrayNonUniformIndexing)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceDescriptorIndexingFeatures", "shaderStorageImageArrayNonUniformIndexing");
         if (b->shaderInputAttachmentArrayNonUniformIndexing && !a->shaderInputAttachmentArrayNonUniformIndexing)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceDescriptorIndexingFeatures", "shaderInputAttachmentArrayNonUniformIndexing");
         if (b->shaderUniformTexelBufferArrayNonUniformIndexing && !a->shaderUniformTexelBufferArrayNonUniformIndexing)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceDescriptorIndexingFeatures", "shaderUniformTexelBufferArrayNonUniformIndexing");
         if (b->shaderStorageTexelBufferArrayNonUniformIndexing && !a->shaderStorageTexelBufferArrayNonUniformIndexing)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceDescriptorIndexingFeatures", "shaderStorageTexelBufferArrayNonUniformIndexing");
         if (b->descriptorBindingUniformBufferUpdateAfterBind && !a->descriptorBindingUniformBufferUpdateAfterBind)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceDescriptorIndexingFeatures", "descriptorBindingUniformBufferUpdateAfterBind");
         if (b->descriptorBindingSampledImageUpdateAfterBind && !a->descriptorBindingSampledImageUpdateAfterBind)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceDescriptorIndexingFeatures", "descriptorBindingSampledImageUpdateAfterBind");
         if (b->descriptorBindingStorageImageUpdateAfterBind && !a->descriptorBindingStorageImageUpdateAfterBind)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceDescriptorIndexingFeatures", "descriptorBindingStorageImageUpdateAfterBind");
         if (b->descriptorBindingStorageBufferUpdateAfterBind && !a->descriptorBindingStorageBufferUpdateAfterBind)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceDescriptorIndexingFeatures", "descriptorBindingStorageBufferUpdateAfterBind");
         if (b->descriptorBindingUniformTexelBufferUpdateAfterBind && !a->descriptorBindingUniformTexelBufferUpdateAfterBind)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceDescriptorIndexingFeatures", "descriptorBindingUniformTexelBufferUpdateAfterBind");
         if (b->descriptorBindingStorageTexelBufferUpdateAfterBind && !a->descriptorBindingStorageTexelBufferUpdateAfterBind)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceDescriptorIndexingFeatures", "descriptorBindingStorageTexelBufferUpdateAfterBind");
         if (b->descriptorBindingUpdateUnusedWhilePending && !a->descriptorBindingUpdateUnusedWhilePending)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceDescriptorIndexingFeatures", "descriptorBindingUpdateUnusedWhilePending");
         if (b->descriptorBindingPartiallyBound && !a->descriptorBindingPartiallyBound)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceDescriptorIndexingFeatures", "descriptorBindingPartiallyBound");
         if (b->descriptorBindingVariableDescriptorCount && !a->descriptorBindingVariableDescriptorCount)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceDescriptorIndexingFeatures", "descriptorBindingVariableDescriptorCount");
         if (b->runtimeDescriptorArray && !a->runtimeDescriptorArray)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceDescriptorIndexingFeatures", "runtimeDescriptorArray");
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TIMELINE_SEMAPHORE_FEATURES : {
         VkPhysicalDeviceTimelineSemaphoreFeatures *a = &supported_VkPhysicalDeviceTimelineSemaphoreFeatures;
         VkPhysicalDeviceTimelineSemaphoreFeatures *b = (VkPhysicalDeviceTimelineSemaphoreFeatures *) feat;
         if (b->timelineSemaphore && !a->timelineSemaphore)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceTimelineSemaphoreFeatures", "timelineSemaphore");
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_8BIT_STORAGE_FEATURES : {
         VkPhysicalDevice8BitStorageFeatures *a = &supported_VkPhysicalDevice8BitStorageFeatures;
         VkPhysicalDevice8BitStorageFeatures *b = (VkPhysicalDevice8BitStorageFeatures *) feat;
         if (b->storageBuffer8BitAccess && !a->storageBuffer8BitAccess)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDevice8BitStorageFeatures", "storageBuffer8BitAccess");
         if (b->uniformAndStorageBuffer8BitAccess && !a->uniformAndStorageBuffer8BitAccess)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDevice8BitStorageFeatures", "uniformAndStorageBuffer8BitAccess");
         if (b->storagePushConstant8 && !a->storagePushConstant8)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDevice8BitStorageFeatures", "storagePushConstant8");
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_CONDITIONAL_RENDERING_FEATURES_EXT : {
         VkPhysicalDeviceConditionalRenderingFeaturesEXT *a = &supported_VkPhysicalDeviceConditionalRenderingFeaturesEXT;
         VkPhysicalDeviceConditionalRenderingFeaturesEXT *b = (VkPhysicalDeviceConditionalRenderingFeaturesEXT *) feat;
         if (b->conditionalRendering && !a->conditionalRendering)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceConditionalRenderingFeaturesEXT", "conditionalRendering");
         if (b->inheritedConditionalRendering && !a->inheritedConditionalRendering)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceConditionalRenderingFeaturesEXT", "inheritedConditionalRendering");
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_MEMORY_MODEL_FEATURES : {
         VkPhysicalDeviceVulkanMemoryModelFeatures *a = &supported_VkPhysicalDeviceVulkanMemoryModelFeatures;
         VkPhysicalDeviceVulkanMemoryModelFeatures *b = (VkPhysicalDeviceVulkanMemoryModelFeatures *) feat;
         if (b->vulkanMemoryModel && !a->vulkanMemoryModel)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceVulkanMemoryModelFeatures", "vulkanMemoryModel");
         if (b->vulkanMemoryModelDeviceScope && !a->vulkanMemoryModelDeviceScope)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceVulkanMemoryModelFeatures", "vulkanMemoryModelDeviceScope");
         if (b->vulkanMemoryModelAvailabilityVisibilityChains && !a->vulkanMemoryModelAvailabilityVisibilityChains)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceVulkanMemoryModelFeatures", "vulkanMemoryModelAvailabilityVisibilityChains");
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_ATOMIC_INT64_FEATURES : {
         VkPhysicalDeviceShaderAtomicInt64Features *a = &supported_VkPhysicalDeviceShaderAtomicInt64Features;
         VkPhysicalDeviceShaderAtomicInt64Features *b = (VkPhysicalDeviceShaderAtomicInt64Features *) feat;
         if (b->shaderBufferInt64Atomics && !a->shaderBufferInt64Atomics)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceShaderAtomicInt64Features", "shaderBufferInt64Atomics");
         if (b->shaderSharedInt64Atomics && !a->shaderSharedInt64Atomics)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceShaderAtomicInt64Features", "shaderSharedInt64Atomics");
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_ATOMIC_FLOAT_FEATURES_EXT : {
         VkPhysicalDeviceShaderAtomicFloatFeaturesEXT *a = &supported_VkPhysicalDeviceShaderAtomicFloatFeaturesEXT;
         VkPhysicalDeviceShaderAtomicFloatFeaturesEXT *b = (VkPhysicalDeviceShaderAtomicFloatFeaturesEXT *) feat;
         if (b->shaderBufferFloat32Atomics && !a->shaderBufferFloat32Atomics)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceShaderAtomicFloatFeaturesEXT", "shaderBufferFloat32Atomics");
         if (b->shaderBufferFloat32AtomicAdd && !a->shaderBufferFloat32AtomicAdd)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceShaderAtomicFloatFeaturesEXT", "shaderBufferFloat32AtomicAdd");
         if (b->shaderBufferFloat64Atomics && !a->shaderBufferFloat64Atomics)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceShaderAtomicFloatFeaturesEXT", "shaderBufferFloat64Atomics");
         if (b->shaderBufferFloat64AtomicAdd && !a->shaderBufferFloat64AtomicAdd)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceShaderAtomicFloatFeaturesEXT", "shaderBufferFloat64AtomicAdd");
         if (b->shaderSharedFloat32Atomics && !a->shaderSharedFloat32Atomics)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceShaderAtomicFloatFeaturesEXT", "shaderSharedFloat32Atomics");
         if (b->shaderSharedFloat32AtomicAdd && !a->shaderSharedFloat32AtomicAdd)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceShaderAtomicFloatFeaturesEXT", "shaderSharedFloat32AtomicAdd");
         if (b->shaderSharedFloat64Atomics && !a->shaderSharedFloat64Atomics)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceShaderAtomicFloatFeaturesEXT", "shaderSharedFloat64Atomics");
         if (b->shaderSharedFloat64AtomicAdd && !a->shaderSharedFloat64AtomicAdd)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceShaderAtomicFloatFeaturesEXT", "shaderSharedFloat64AtomicAdd");
         if (b->shaderImageFloat32Atomics && !a->shaderImageFloat32Atomics)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceShaderAtomicFloatFeaturesEXT", "shaderImageFloat32Atomics");
         if (b->shaderImageFloat32AtomicAdd && !a->shaderImageFloat32AtomicAdd)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceShaderAtomicFloatFeaturesEXT", "shaderImageFloat32AtomicAdd");
         if (b->sparseImageFloat32Atomics && !a->sparseImageFloat32Atomics)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceShaderAtomicFloatFeaturesEXT", "sparseImageFloat32Atomics");
         if (b->sparseImageFloat32AtomicAdd && !a->sparseImageFloat32AtomicAdd)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceShaderAtomicFloatFeaturesEXT", "sparseImageFloat32AtomicAdd");
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_ATOMIC_FLOAT_2_FEATURES_EXT : {
         VkPhysicalDeviceShaderAtomicFloat2FeaturesEXT *a = &supported_VkPhysicalDeviceShaderAtomicFloat2FeaturesEXT;
         VkPhysicalDeviceShaderAtomicFloat2FeaturesEXT *b = (VkPhysicalDeviceShaderAtomicFloat2FeaturesEXT *) feat;
         if (b->shaderBufferFloat16Atomics && !a->shaderBufferFloat16Atomics)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceShaderAtomicFloat2FeaturesEXT", "shaderBufferFloat16Atomics");
         if (b->shaderBufferFloat16AtomicAdd && !a->shaderBufferFloat16AtomicAdd)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceShaderAtomicFloat2FeaturesEXT", "shaderBufferFloat16AtomicAdd");
         if (b->shaderBufferFloat16AtomicMinMax && !a->shaderBufferFloat16AtomicMinMax)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceShaderAtomicFloat2FeaturesEXT", "shaderBufferFloat16AtomicMinMax");
         if (b->shaderBufferFloat32AtomicMinMax && !a->shaderBufferFloat32AtomicMinMax)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceShaderAtomicFloat2FeaturesEXT", "shaderBufferFloat32AtomicMinMax");
         if (b->shaderBufferFloat64AtomicMinMax && !a->shaderBufferFloat64AtomicMinMax)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceShaderAtomicFloat2FeaturesEXT", "shaderBufferFloat64AtomicMinMax");
         if (b->shaderSharedFloat16Atomics && !a->shaderSharedFloat16Atomics)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceShaderAtomicFloat2FeaturesEXT", "shaderSharedFloat16Atomics");
         if (b->shaderSharedFloat16AtomicAdd && !a->shaderSharedFloat16AtomicAdd)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceShaderAtomicFloat2FeaturesEXT", "shaderSharedFloat16AtomicAdd");
         if (b->shaderSharedFloat16AtomicMinMax && !a->shaderSharedFloat16AtomicMinMax)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceShaderAtomicFloat2FeaturesEXT", "shaderSharedFloat16AtomicMinMax");
         if (b->shaderSharedFloat32AtomicMinMax && !a->shaderSharedFloat32AtomicMinMax)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceShaderAtomicFloat2FeaturesEXT", "shaderSharedFloat32AtomicMinMax");
         if (b->shaderSharedFloat64AtomicMinMax && !a->shaderSharedFloat64AtomicMinMax)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceShaderAtomicFloat2FeaturesEXT", "shaderSharedFloat64AtomicMinMax");
         if (b->shaderImageFloat32AtomicMinMax && !a->shaderImageFloat32AtomicMinMax)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceShaderAtomicFloat2FeaturesEXT", "shaderImageFloat32AtomicMinMax");
         if (b->sparseImageFloat32AtomicMinMax && !a->sparseImageFloat32AtomicMinMax)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceShaderAtomicFloat2FeaturesEXT", "sparseImageFloat32AtomicMinMax");
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VERTEX_ATTRIBUTE_DIVISOR_FEATURES_EXT : {
         VkPhysicalDeviceVertexAttributeDivisorFeaturesEXT *a = &supported_VkPhysicalDeviceVertexAttributeDivisorFeaturesEXT;
         VkPhysicalDeviceVertexAttributeDivisorFeaturesEXT *b = (VkPhysicalDeviceVertexAttributeDivisorFeaturesEXT *) feat;
         if (b->vertexAttributeInstanceRateDivisor && !a->vertexAttributeInstanceRateDivisor)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceVertexAttributeDivisorFeaturesEXT", "vertexAttributeInstanceRateDivisor");
         if (b->vertexAttributeInstanceRateZeroDivisor && !a->vertexAttributeInstanceRateZeroDivisor)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceVertexAttributeDivisorFeaturesEXT", "vertexAttributeInstanceRateZeroDivisor");
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ASTC_DECODE_FEATURES_EXT : {
         VkPhysicalDeviceASTCDecodeFeaturesEXT *a = &supported_VkPhysicalDeviceASTCDecodeFeaturesEXT;
         VkPhysicalDeviceASTCDecodeFeaturesEXT *b = (VkPhysicalDeviceASTCDecodeFeaturesEXT *) feat;
         if (b->decodeModeSharedExponent && !a->decodeModeSharedExponent)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceASTCDecodeFeaturesEXT", "decodeModeSharedExponent");
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TRANSFORM_FEEDBACK_FEATURES_EXT : {
         VkPhysicalDeviceTransformFeedbackFeaturesEXT *a = &supported_VkPhysicalDeviceTransformFeedbackFeaturesEXT;
         VkPhysicalDeviceTransformFeedbackFeaturesEXT *b = (VkPhysicalDeviceTransformFeedbackFeaturesEXT *) feat;
         if (b->transformFeedback && !a->transformFeedback)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceTransformFeedbackFeaturesEXT", "transformFeedback");
         if (b->geometryStreams && !a->geometryStreams)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceTransformFeedbackFeaturesEXT", "geometryStreams");
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_REPRESENTATIVE_FRAGMENT_TEST_FEATURES_NV : {
         VkPhysicalDeviceRepresentativeFragmentTestFeaturesNV *a = &supported_VkPhysicalDeviceRepresentativeFragmentTestFeaturesNV;
         VkPhysicalDeviceRepresentativeFragmentTestFeaturesNV *b = (VkPhysicalDeviceRepresentativeFragmentTestFeaturesNV *) feat;
         if (b->representativeFragmentTest && !a->representativeFragmentTest)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceRepresentativeFragmentTestFeaturesNV", "representativeFragmentTest");
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXCLUSIVE_SCISSOR_FEATURES_NV : {
         VkPhysicalDeviceExclusiveScissorFeaturesNV *a = &supported_VkPhysicalDeviceExclusiveScissorFeaturesNV;
         VkPhysicalDeviceExclusiveScissorFeaturesNV *b = (VkPhysicalDeviceExclusiveScissorFeaturesNV *) feat;
         if (b->exclusiveScissor && !a->exclusiveScissor)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceExclusiveScissorFeaturesNV", "exclusiveScissor");
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_CORNER_SAMPLED_IMAGE_FEATURES_NV : {
         VkPhysicalDeviceCornerSampledImageFeaturesNV *a = &supported_VkPhysicalDeviceCornerSampledImageFeaturesNV;
         VkPhysicalDeviceCornerSampledImageFeaturesNV *b = (VkPhysicalDeviceCornerSampledImageFeaturesNV *) feat;
         if (b->cornerSampledImage && !a->cornerSampledImage)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceCornerSampledImageFeaturesNV", "cornerSampledImage");
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_COMPUTE_SHADER_DERIVATIVES_FEATURES_NV : {
         VkPhysicalDeviceComputeShaderDerivativesFeaturesNV *a = &supported_VkPhysicalDeviceComputeShaderDerivativesFeaturesNV;
         VkPhysicalDeviceComputeShaderDerivativesFeaturesNV *b = (VkPhysicalDeviceComputeShaderDerivativesFeaturesNV *) feat;
         if (b->computeDerivativeGroupQuads && !a->computeDerivativeGroupQuads)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceComputeShaderDerivativesFeaturesNV", "computeDerivativeGroupQuads");
         if (b->computeDerivativeGroupLinear && !a->computeDerivativeGroupLinear)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceComputeShaderDerivativesFeaturesNV", "computeDerivativeGroupLinear");
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FRAGMENT_SHADER_BARYCENTRIC_FEATURES_NV : {
         VkPhysicalDeviceFragmentShaderBarycentricFeaturesNV *a = &supported_VkPhysicalDeviceFragmentShaderBarycentricFeaturesNV;
         VkPhysicalDeviceFragmentShaderBarycentricFeaturesNV *b = (VkPhysicalDeviceFragmentShaderBarycentricFeaturesNV *) feat;
         if (b->fragmentShaderBarycentric && !a->fragmentShaderBarycentric)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceFragmentShaderBarycentricFeaturesNV", "fragmentShaderBarycentric");
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_IMAGE_FOOTPRINT_FEATURES_NV : {
         VkPhysicalDeviceShaderImageFootprintFeaturesNV *a = &supported_VkPhysicalDeviceShaderImageFootprintFeaturesNV;
         VkPhysicalDeviceShaderImageFootprintFeaturesNV *b = (VkPhysicalDeviceShaderImageFootprintFeaturesNV *) feat;
         if (b->imageFootprint && !a->imageFootprint)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceShaderImageFootprintFeaturesNV", "imageFootprint");
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DEDICATED_ALLOCATION_IMAGE_ALIASING_FEATURES_NV : {
         VkPhysicalDeviceDedicatedAllocationImageAliasingFeaturesNV *a = &supported_VkPhysicalDeviceDedicatedAllocationImageAliasingFeaturesNV;
         VkPhysicalDeviceDedicatedAllocationImageAliasingFeaturesNV *b = (VkPhysicalDeviceDedicatedAllocationImageAliasingFeaturesNV *) feat;
         if (b->dedicatedAllocationImageAliasing && !a->dedicatedAllocationImageAliasing)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceDedicatedAllocationImageAliasingFeaturesNV", "dedicatedAllocationImageAliasing");
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADING_RATE_IMAGE_FEATURES_NV : {
         VkPhysicalDeviceShadingRateImageFeaturesNV *a = &supported_VkPhysicalDeviceShadingRateImageFeaturesNV;
         VkPhysicalDeviceShadingRateImageFeaturesNV *b = (VkPhysicalDeviceShadingRateImageFeaturesNV *) feat;
         if (b->shadingRateImage && !a->shadingRateImage)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceShadingRateImageFeaturesNV", "shadingRateImage");
         if (b->shadingRateCoarseSampleOrder && !a->shadingRateCoarseSampleOrder)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceShadingRateImageFeaturesNV", "shadingRateCoarseSampleOrder");
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_INVOCATION_MASK_FEATURES_HUAWEI : {
         VkPhysicalDeviceInvocationMaskFeaturesHUAWEI *a = &supported_VkPhysicalDeviceInvocationMaskFeaturesHUAWEI;
         VkPhysicalDeviceInvocationMaskFeaturesHUAWEI *b = (VkPhysicalDeviceInvocationMaskFeaturesHUAWEI *) feat;
         if (b->invocationMask && !a->invocationMask)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceInvocationMaskFeaturesHUAWEI", "invocationMask");
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MESH_SHADER_FEATURES_NV : {
         VkPhysicalDeviceMeshShaderFeaturesNV *a = &supported_VkPhysicalDeviceMeshShaderFeaturesNV;
         VkPhysicalDeviceMeshShaderFeaturesNV *b = (VkPhysicalDeviceMeshShaderFeaturesNV *) feat;
         if (b->taskShader && !a->taskShader)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceMeshShaderFeaturesNV", "taskShader");
         if (b->meshShader && !a->meshShader)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceMeshShaderFeaturesNV", "meshShader");
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ACCELERATION_STRUCTURE_FEATURES_KHR : {
         VkPhysicalDeviceAccelerationStructureFeaturesKHR *a = &supported_VkPhysicalDeviceAccelerationStructureFeaturesKHR;
         VkPhysicalDeviceAccelerationStructureFeaturesKHR *b = (VkPhysicalDeviceAccelerationStructureFeaturesKHR *) feat;
         if (b->accelerationStructure && !a->accelerationStructure)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceAccelerationStructureFeaturesKHR", "accelerationStructure");
         if (b->accelerationStructureCaptureReplay && !a->accelerationStructureCaptureReplay)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceAccelerationStructureFeaturesKHR", "accelerationStructureCaptureReplay");
         if (b->accelerationStructureIndirectBuild && !a->accelerationStructureIndirectBuild)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceAccelerationStructureFeaturesKHR", "accelerationStructureIndirectBuild");
         if (b->accelerationStructureHostCommands && !a->accelerationStructureHostCommands)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceAccelerationStructureFeaturesKHR", "accelerationStructureHostCommands");
         if (b->descriptorBindingAccelerationStructureUpdateAfterBind && !a->descriptorBindingAccelerationStructureUpdateAfterBind)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceAccelerationStructureFeaturesKHR", "descriptorBindingAccelerationStructureUpdateAfterBind");
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_PIPELINE_FEATURES_KHR : {
         VkPhysicalDeviceRayTracingPipelineFeaturesKHR *a = &supported_VkPhysicalDeviceRayTracingPipelineFeaturesKHR;
         VkPhysicalDeviceRayTracingPipelineFeaturesKHR *b = (VkPhysicalDeviceRayTracingPipelineFeaturesKHR *) feat;
         if (b->rayTracingPipeline && !a->rayTracingPipeline)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceRayTracingPipelineFeaturesKHR", "rayTracingPipeline");
         if (b->rayTracingPipelineShaderGroupHandleCaptureReplay && !a->rayTracingPipelineShaderGroupHandleCaptureReplay)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceRayTracingPipelineFeaturesKHR", "rayTracingPipelineShaderGroupHandleCaptureReplay");
         if (b->rayTracingPipelineShaderGroupHandleCaptureReplayMixed && !a->rayTracingPipelineShaderGroupHandleCaptureReplayMixed)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceRayTracingPipelineFeaturesKHR", "rayTracingPipelineShaderGroupHandleCaptureReplayMixed");
         if (b->rayTracingPipelineTraceRaysIndirect && !a->rayTracingPipelineTraceRaysIndirect)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceRayTracingPipelineFeaturesKHR", "rayTracingPipelineTraceRaysIndirect");
         if (b->rayTraversalPrimitiveCulling && !a->rayTraversalPrimitiveCulling)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceRayTracingPipelineFeaturesKHR", "rayTraversalPrimitiveCulling");
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_QUERY_FEATURES_KHR : {
         VkPhysicalDeviceRayQueryFeaturesKHR *a = &supported_VkPhysicalDeviceRayQueryFeaturesKHR;
         VkPhysicalDeviceRayQueryFeaturesKHR *b = (VkPhysicalDeviceRayQueryFeaturesKHR *) feat;
         if (b->rayQuery && !a->rayQuery)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceRayQueryFeaturesKHR", "rayQuery");
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FRAGMENT_DENSITY_MAP_FEATURES_EXT : {
         VkPhysicalDeviceFragmentDensityMapFeaturesEXT *a = &supported_VkPhysicalDeviceFragmentDensityMapFeaturesEXT;
         VkPhysicalDeviceFragmentDensityMapFeaturesEXT *b = (VkPhysicalDeviceFragmentDensityMapFeaturesEXT *) feat;
         if (b->fragmentDensityMap && !a->fragmentDensityMap)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceFragmentDensityMapFeaturesEXT", "fragmentDensityMap");
         if (b->fragmentDensityMapDynamic && !a->fragmentDensityMapDynamic)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceFragmentDensityMapFeaturesEXT", "fragmentDensityMapDynamic");
         if (b->fragmentDensityMapNonSubsampledImages && !a->fragmentDensityMapNonSubsampledImages)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceFragmentDensityMapFeaturesEXT", "fragmentDensityMapNonSubsampledImages");
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FRAGMENT_DENSITY_MAP_2_FEATURES_EXT : {
         VkPhysicalDeviceFragmentDensityMap2FeaturesEXT *a = &supported_VkPhysicalDeviceFragmentDensityMap2FeaturesEXT;
         VkPhysicalDeviceFragmentDensityMap2FeaturesEXT *b = (VkPhysicalDeviceFragmentDensityMap2FeaturesEXT *) feat;
         if (b->fragmentDensityMapDeferred && !a->fragmentDensityMapDeferred)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceFragmentDensityMap2FeaturesEXT", "fragmentDensityMapDeferred");
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SCALAR_BLOCK_LAYOUT_FEATURES : {
         VkPhysicalDeviceScalarBlockLayoutFeatures *a = &supported_VkPhysicalDeviceScalarBlockLayoutFeatures;
         VkPhysicalDeviceScalarBlockLayoutFeatures *b = (VkPhysicalDeviceScalarBlockLayoutFeatures *) feat;
         if (b->scalarBlockLayout && !a->scalarBlockLayout)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceScalarBlockLayoutFeatures", "scalarBlockLayout");
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_UNIFORM_BUFFER_STANDARD_LAYOUT_FEATURES : {
         VkPhysicalDeviceUniformBufferStandardLayoutFeatures *a = &supported_VkPhysicalDeviceUniformBufferStandardLayoutFeatures;
         VkPhysicalDeviceUniformBufferStandardLayoutFeatures *b = (VkPhysicalDeviceUniformBufferStandardLayoutFeatures *) feat;
         if (b->uniformBufferStandardLayout && !a->uniformBufferStandardLayout)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceUniformBufferStandardLayoutFeatures", "uniformBufferStandardLayout");
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DEPTH_CLIP_ENABLE_FEATURES_EXT : {
         VkPhysicalDeviceDepthClipEnableFeaturesEXT *a = &supported_VkPhysicalDeviceDepthClipEnableFeaturesEXT;
         VkPhysicalDeviceDepthClipEnableFeaturesEXT *b = (VkPhysicalDeviceDepthClipEnableFeaturesEXT *) feat;
         if (b->depthClipEnable && !a->depthClipEnable)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceDepthClipEnableFeaturesEXT", "depthClipEnable");
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MEMORY_PRIORITY_FEATURES_EXT : {
         VkPhysicalDeviceMemoryPriorityFeaturesEXT *a = &supported_VkPhysicalDeviceMemoryPriorityFeaturesEXT;
         VkPhysicalDeviceMemoryPriorityFeaturesEXT *b = (VkPhysicalDeviceMemoryPriorityFeaturesEXT *) feat;
         if (b->memoryPriority && !a->memoryPriority)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceMemoryPriorityFeaturesEXT", "memoryPriority");
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PAGEABLE_DEVICE_LOCAL_MEMORY_FEATURES_EXT : {
         VkPhysicalDevicePageableDeviceLocalMemoryFeaturesEXT *a = &supported_VkPhysicalDevicePageableDeviceLocalMemoryFeaturesEXT;
         VkPhysicalDevicePageableDeviceLocalMemoryFeaturesEXT *b = (VkPhysicalDevicePageableDeviceLocalMemoryFeaturesEXT *) feat;
         if (b->pageableDeviceLocalMemory && !a->pageableDeviceLocalMemory)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDevicePageableDeviceLocalMemoryFeaturesEXT", "pageableDeviceLocalMemory");
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_BUFFER_DEVICE_ADDRESS_FEATURES : {
         VkPhysicalDeviceBufferDeviceAddressFeatures *a = &supported_VkPhysicalDeviceBufferDeviceAddressFeatures;
         VkPhysicalDeviceBufferDeviceAddressFeatures *b = (VkPhysicalDeviceBufferDeviceAddressFeatures *) feat;
         if (b->bufferDeviceAddress && !a->bufferDeviceAddress)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceBufferDeviceAddressFeatures", "bufferDeviceAddress");
         if (b->bufferDeviceAddressCaptureReplay && !a->bufferDeviceAddressCaptureReplay)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceBufferDeviceAddressFeatures", "bufferDeviceAddressCaptureReplay");
         if (b->bufferDeviceAddressMultiDevice && !a->bufferDeviceAddressMultiDevice)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceBufferDeviceAddressFeatures", "bufferDeviceAddressMultiDevice");
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_BUFFER_DEVICE_ADDRESS_FEATURES_EXT : {
         VkPhysicalDeviceBufferDeviceAddressFeaturesEXT *a = &supported_VkPhysicalDeviceBufferDeviceAddressFeaturesEXT;
         VkPhysicalDeviceBufferDeviceAddressFeaturesEXT *b = (VkPhysicalDeviceBufferDeviceAddressFeaturesEXT *) feat;
         if (b->bufferDeviceAddress && !a->bufferDeviceAddress)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceBufferDeviceAddressFeaturesEXT", "bufferDeviceAddress");
         if (b->bufferDeviceAddressCaptureReplay && !a->bufferDeviceAddressCaptureReplay)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceBufferDeviceAddressFeaturesEXT", "bufferDeviceAddressCaptureReplay");
         if (b->bufferDeviceAddressMultiDevice && !a->bufferDeviceAddressMultiDevice)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceBufferDeviceAddressFeaturesEXT", "bufferDeviceAddressMultiDevice");
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_IMAGELESS_FRAMEBUFFER_FEATURES : {
         VkPhysicalDeviceImagelessFramebufferFeatures *a = &supported_VkPhysicalDeviceImagelessFramebufferFeatures;
         VkPhysicalDeviceImagelessFramebufferFeatures *b = (VkPhysicalDeviceImagelessFramebufferFeatures *) feat;
         if (b->imagelessFramebuffer && !a->imagelessFramebuffer)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceImagelessFramebufferFeatures", "imagelessFramebuffer");
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TEXTURE_COMPRESSION_ASTC_HDR_FEATURES_EXT : {
         VkPhysicalDeviceTextureCompressionASTCHDRFeaturesEXT *a = &supported_VkPhysicalDeviceTextureCompressionASTCHDRFeaturesEXT;
         VkPhysicalDeviceTextureCompressionASTCHDRFeaturesEXT *b = (VkPhysicalDeviceTextureCompressionASTCHDRFeaturesEXT *) feat;
         if (b->textureCompressionASTC_HDR && !a->textureCompressionASTC_HDR)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceTextureCompressionASTCHDRFeaturesEXT", "textureCompressionASTC_HDR");
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_COOPERATIVE_MATRIX_FEATURES_NV : {
         VkPhysicalDeviceCooperativeMatrixFeaturesNV *a = &supported_VkPhysicalDeviceCooperativeMatrixFeaturesNV;
         VkPhysicalDeviceCooperativeMatrixFeaturesNV *b = (VkPhysicalDeviceCooperativeMatrixFeaturesNV *) feat;
         if (b->cooperativeMatrix && !a->cooperativeMatrix)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceCooperativeMatrixFeaturesNV", "cooperativeMatrix");
         if (b->cooperativeMatrixRobustBufferAccess && !a->cooperativeMatrixRobustBufferAccess)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceCooperativeMatrixFeaturesNV", "cooperativeMatrixRobustBufferAccess");
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_YCBCR_IMAGE_ARRAYS_FEATURES_EXT : {
         VkPhysicalDeviceYcbcrImageArraysFeaturesEXT *a = &supported_VkPhysicalDeviceYcbcrImageArraysFeaturesEXT;
         VkPhysicalDeviceYcbcrImageArraysFeaturesEXT *b = (VkPhysicalDeviceYcbcrImageArraysFeaturesEXT *) feat;
         if (b->ycbcrImageArrays && !a->ycbcrImageArrays)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceYcbcrImageArraysFeaturesEXT", "ycbcrImageArrays");
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PERFORMANCE_QUERY_FEATURES_KHR : {
         VkPhysicalDevicePerformanceQueryFeaturesKHR *a = &supported_VkPhysicalDevicePerformanceQueryFeaturesKHR;
         VkPhysicalDevicePerformanceQueryFeaturesKHR *b = (VkPhysicalDevicePerformanceQueryFeaturesKHR *) feat;
         if (b->performanceCounterQueryPools && !a->performanceCounterQueryPools)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDevicePerformanceQueryFeaturesKHR", "performanceCounterQueryPools");
         if (b->performanceCounterMultipleQueryPools && !a->performanceCounterMultipleQueryPools)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDevicePerformanceQueryFeaturesKHR", "performanceCounterMultipleQueryPools");
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_COVERAGE_REDUCTION_MODE_FEATURES_NV : {
         VkPhysicalDeviceCoverageReductionModeFeaturesNV *a = &supported_VkPhysicalDeviceCoverageReductionModeFeaturesNV;
         VkPhysicalDeviceCoverageReductionModeFeaturesNV *b = (VkPhysicalDeviceCoverageReductionModeFeaturesNV *) feat;
         if (b->coverageReductionMode && !a->coverageReductionMode)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceCoverageReductionModeFeaturesNV", "coverageReductionMode");
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_INTEGER_FUNCTIONS_2_FEATURES_INTEL : {
         VkPhysicalDeviceShaderIntegerFunctions2FeaturesINTEL *a = &supported_VkPhysicalDeviceShaderIntegerFunctions2FeaturesINTEL;
         VkPhysicalDeviceShaderIntegerFunctions2FeaturesINTEL *b = (VkPhysicalDeviceShaderIntegerFunctions2FeaturesINTEL *) feat;
         if (b->shaderIntegerFunctions2 && !a->shaderIntegerFunctions2)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceShaderIntegerFunctions2FeaturesINTEL", "shaderIntegerFunctions2");
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_CLOCK_FEATURES_KHR : {
         VkPhysicalDeviceShaderClockFeaturesKHR *a = &supported_VkPhysicalDeviceShaderClockFeaturesKHR;
         VkPhysicalDeviceShaderClockFeaturesKHR *b = (VkPhysicalDeviceShaderClockFeaturesKHR *) feat;
         if (b->shaderSubgroupClock && !a->shaderSubgroupClock)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceShaderClockFeaturesKHR", "shaderSubgroupClock");
         if (b->shaderDeviceClock && !a->shaderDeviceClock)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceShaderClockFeaturesKHR", "shaderDeviceClock");
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_INDEX_TYPE_UINT8_FEATURES_EXT : {
         VkPhysicalDeviceIndexTypeUint8FeaturesEXT *a = &supported_VkPhysicalDeviceIndexTypeUint8FeaturesEXT;
         VkPhysicalDeviceIndexTypeUint8FeaturesEXT *b = (VkPhysicalDeviceIndexTypeUint8FeaturesEXT *) feat;
         if (b->indexTypeUint8 && !a->indexTypeUint8)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceIndexTypeUint8FeaturesEXT", "indexTypeUint8");
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_SM_BUILTINS_FEATURES_NV : {
         VkPhysicalDeviceShaderSMBuiltinsFeaturesNV *a = &supported_VkPhysicalDeviceShaderSMBuiltinsFeaturesNV;
         VkPhysicalDeviceShaderSMBuiltinsFeaturesNV *b = (VkPhysicalDeviceShaderSMBuiltinsFeaturesNV *) feat;
         if (b->shaderSMBuiltins && !a->shaderSMBuiltins)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceShaderSMBuiltinsFeaturesNV", "shaderSMBuiltins");
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FRAGMENT_SHADER_INTERLOCK_FEATURES_EXT : {
         VkPhysicalDeviceFragmentShaderInterlockFeaturesEXT *a = &supported_VkPhysicalDeviceFragmentShaderInterlockFeaturesEXT;
         VkPhysicalDeviceFragmentShaderInterlockFeaturesEXT *b = (VkPhysicalDeviceFragmentShaderInterlockFeaturesEXT *) feat;
         if (b->fragmentShaderSampleInterlock && !a->fragmentShaderSampleInterlock)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceFragmentShaderInterlockFeaturesEXT", "fragmentShaderSampleInterlock");
         if (b->fragmentShaderPixelInterlock && !a->fragmentShaderPixelInterlock)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceFragmentShaderInterlockFeaturesEXT", "fragmentShaderPixelInterlock");
         if (b->fragmentShaderShadingRateInterlock && !a->fragmentShaderShadingRateInterlock)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceFragmentShaderInterlockFeaturesEXT", "fragmentShaderShadingRateInterlock");
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SEPARATE_DEPTH_STENCIL_LAYOUTS_FEATURES : {
         VkPhysicalDeviceSeparateDepthStencilLayoutsFeatures *a = &supported_VkPhysicalDeviceSeparateDepthStencilLayoutsFeatures;
         VkPhysicalDeviceSeparateDepthStencilLayoutsFeatures *b = (VkPhysicalDeviceSeparateDepthStencilLayoutsFeatures *) feat;
         if (b->separateDepthStencilLayouts && !a->separateDepthStencilLayouts)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceSeparateDepthStencilLayoutsFeatures", "separateDepthStencilLayouts");
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PRIMITIVE_TOPOLOGY_LIST_RESTART_FEATURES_EXT : {
         VkPhysicalDevicePrimitiveTopologyListRestartFeaturesEXT *a = &supported_VkPhysicalDevicePrimitiveTopologyListRestartFeaturesEXT;
         VkPhysicalDevicePrimitiveTopologyListRestartFeaturesEXT *b = (VkPhysicalDevicePrimitiveTopologyListRestartFeaturesEXT *) feat;
         if (b->primitiveTopologyListRestart && !a->primitiveTopologyListRestart)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDevicePrimitiveTopologyListRestartFeaturesEXT", "primitiveTopologyListRestart");
         if (b->primitiveTopologyPatchListRestart && !a->primitiveTopologyPatchListRestart)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDevicePrimitiveTopologyListRestartFeaturesEXT", "primitiveTopologyPatchListRestart");
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PIPELINE_EXECUTABLE_PROPERTIES_FEATURES_KHR : {
         VkPhysicalDevicePipelineExecutablePropertiesFeaturesKHR *a = &supported_VkPhysicalDevicePipelineExecutablePropertiesFeaturesKHR;
         VkPhysicalDevicePipelineExecutablePropertiesFeaturesKHR *b = (VkPhysicalDevicePipelineExecutablePropertiesFeaturesKHR *) feat;
         if (b->pipelineExecutableInfo && !a->pipelineExecutableInfo)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDevicePipelineExecutablePropertiesFeaturesKHR", "pipelineExecutableInfo");
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_DEMOTE_TO_HELPER_INVOCATION_FEATURES_EXT : {
         VkPhysicalDeviceShaderDemoteToHelperInvocationFeaturesEXT *a = &supported_VkPhysicalDeviceShaderDemoteToHelperInvocationFeaturesEXT;
         VkPhysicalDeviceShaderDemoteToHelperInvocationFeaturesEXT *b = (VkPhysicalDeviceShaderDemoteToHelperInvocationFeaturesEXT *) feat;
         if (b->shaderDemoteToHelperInvocation && !a->shaderDemoteToHelperInvocation)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceShaderDemoteToHelperInvocationFeaturesEXT", "shaderDemoteToHelperInvocation");
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TEXEL_BUFFER_ALIGNMENT_FEATURES_EXT : {
         VkPhysicalDeviceTexelBufferAlignmentFeaturesEXT *a = &supported_VkPhysicalDeviceTexelBufferAlignmentFeaturesEXT;
         VkPhysicalDeviceTexelBufferAlignmentFeaturesEXT *b = (VkPhysicalDeviceTexelBufferAlignmentFeaturesEXT *) feat;
         if (b->texelBufferAlignment && !a->texelBufferAlignment)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceTexelBufferAlignmentFeaturesEXT", "texelBufferAlignment");
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SUBGROUP_SIZE_CONTROL_FEATURES_EXT : {
         VkPhysicalDeviceSubgroupSizeControlFeaturesEXT *a = &supported_VkPhysicalDeviceSubgroupSizeControlFeaturesEXT;
         VkPhysicalDeviceSubgroupSizeControlFeaturesEXT *b = (VkPhysicalDeviceSubgroupSizeControlFeaturesEXT *) feat;
         if (b->subgroupSizeControl && !a->subgroupSizeControl)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceSubgroupSizeControlFeaturesEXT", "subgroupSizeControl");
         if (b->computeFullSubgroups && !a->computeFullSubgroups)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceSubgroupSizeControlFeaturesEXT", "computeFullSubgroups");
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_LINE_RASTERIZATION_FEATURES_EXT : {
         VkPhysicalDeviceLineRasterizationFeaturesEXT *a = &supported_VkPhysicalDeviceLineRasterizationFeaturesEXT;
         VkPhysicalDeviceLineRasterizationFeaturesEXT *b = (VkPhysicalDeviceLineRasterizationFeaturesEXT *) feat;
         if (b->rectangularLines && !a->rectangularLines)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceLineRasterizationFeaturesEXT", "rectangularLines");
         if (b->bresenhamLines && !a->bresenhamLines)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceLineRasterizationFeaturesEXT", "bresenhamLines");
         if (b->smoothLines && !a->smoothLines)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceLineRasterizationFeaturesEXT", "smoothLines");
         if (b->stippledRectangularLines && !a->stippledRectangularLines)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceLineRasterizationFeaturesEXT", "stippledRectangularLines");
         if (b->stippledBresenhamLines && !a->stippledBresenhamLines)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceLineRasterizationFeaturesEXT", "stippledBresenhamLines");
         if (b->stippledSmoothLines && !a->stippledSmoothLines)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceLineRasterizationFeaturesEXT", "stippledSmoothLines");
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PIPELINE_CREATION_CACHE_CONTROL_FEATURES_EXT : {
         VkPhysicalDevicePipelineCreationCacheControlFeaturesEXT *a = &supported_VkPhysicalDevicePipelineCreationCacheControlFeaturesEXT;
         VkPhysicalDevicePipelineCreationCacheControlFeaturesEXT *b = (VkPhysicalDevicePipelineCreationCacheControlFeaturesEXT *) feat;
         if (b->pipelineCreationCacheControl && !a->pipelineCreationCacheControl)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDevicePipelineCreationCacheControlFeaturesEXT", "pipelineCreationCacheControl");
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_FEATURES : {
         VkPhysicalDeviceVulkan11Features *a = &supported_VkPhysicalDeviceVulkan11Features;
         VkPhysicalDeviceVulkan11Features *b = (VkPhysicalDeviceVulkan11Features *) feat;
         if (b->storageBuffer16BitAccess && !a->storageBuffer16BitAccess)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceVulkan11Features", "storageBuffer16BitAccess");
         if (b->uniformAndStorageBuffer16BitAccess && !a->uniformAndStorageBuffer16BitAccess)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceVulkan11Features", "uniformAndStorageBuffer16BitAccess");
         if (b->storagePushConstant16 && !a->storagePushConstant16)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceVulkan11Features", "storagePushConstant16");
         if (b->storageInputOutput16 && !a->storageInputOutput16)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceVulkan11Features", "storageInputOutput16");
         if (b->multiview && !a->multiview)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceVulkan11Features", "multiview");
         if (b->multiviewGeometryShader && !a->multiviewGeometryShader)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceVulkan11Features", "multiviewGeometryShader");
         if (b->multiviewTessellationShader && !a->multiviewTessellationShader)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceVulkan11Features", "multiviewTessellationShader");
         if (b->variablePointersStorageBuffer && !a->variablePointersStorageBuffer)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceVulkan11Features", "variablePointersStorageBuffer");
         if (b->variablePointers && !a->variablePointers)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceVulkan11Features", "variablePointers");
         if (b->protectedMemory && !a->protectedMemory)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceVulkan11Features", "protectedMemory");
         if (b->samplerYcbcrConversion && !a->samplerYcbcrConversion)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceVulkan11Features", "samplerYcbcrConversion");
         if (b->shaderDrawParameters && !a->shaderDrawParameters)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceVulkan11Features", "shaderDrawParameters");
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES : {
         VkPhysicalDeviceVulkan12Features *a = &supported_VkPhysicalDeviceVulkan12Features;
         VkPhysicalDeviceVulkan12Features *b = (VkPhysicalDeviceVulkan12Features *) feat;
         if (b->samplerMirrorClampToEdge && !a->samplerMirrorClampToEdge)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceVulkan12Features", "samplerMirrorClampToEdge");
         if (b->drawIndirectCount && !a->drawIndirectCount)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceVulkan12Features", "drawIndirectCount");
         if (b->storageBuffer8BitAccess && !a->storageBuffer8BitAccess)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceVulkan12Features", "storageBuffer8BitAccess");
         if (b->uniformAndStorageBuffer8BitAccess && !a->uniformAndStorageBuffer8BitAccess)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceVulkan12Features", "uniformAndStorageBuffer8BitAccess");
         if (b->storagePushConstant8 && !a->storagePushConstant8)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceVulkan12Features", "storagePushConstant8");
         if (b->shaderBufferInt64Atomics && !a->shaderBufferInt64Atomics)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceVulkan12Features", "shaderBufferInt64Atomics");
         if (b->shaderSharedInt64Atomics && !a->shaderSharedInt64Atomics)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceVulkan12Features", "shaderSharedInt64Atomics");
         if (b->shaderFloat16 && !a->shaderFloat16)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceVulkan12Features", "shaderFloat16");
         if (b->shaderInt8 && !a->shaderInt8)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceVulkan12Features", "shaderInt8");
         if (b->descriptorIndexing && !a->descriptorIndexing)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceVulkan12Features", "descriptorIndexing");
         if (b->shaderInputAttachmentArrayDynamicIndexing && !a->shaderInputAttachmentArrayDynamicIndexing)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceVulkan12Features", "shaderInputAttachmentArrayDynamicIndexing");
         if (b->shaderUniformTexelBufferArrayDynamicIndexing && !a->shaderUniformTexelBufferArrayDynamicIndexing)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceVulkan12Features", "shaderUniformTexelBufferArrayDynamicIndexing");
         if (b->shaderStorageTexelBufferArrayDynamicIndexing && !a->shaderStorageTexelBufferArrayDynamicIndexing)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceVulkan12Features", "shaderStorageTexelBufferArrayDynamicIndexing");
         if (b->shaderUniformBufferArrayNonUniformIndexing && !a->shaderUniformBufferArrayNonUniformIndexing)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceVulkan12Features", "shaderUniformBufferArrayNonUniformIndexing");
         if (b->shaderSampledImageArrayNonUniformIndexing && !a->shaderSampledImageArrayNonUniformIndexing)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceVulkan12Features", "shaderSampledImageArrayNonUniformIndexing");
         if (b->shaderStorageBufferArrayNonUniformIndexing && !a->shaderStorageBufferArrayNonUniformIndexing)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceVulkan12Features", "shaderStorageBufferArrayNonUniformIndexing");
         if (b->shaderStorageImageArrayNonUniformIndexing && !a->shaderStorageImageArrayNonUniformIndexing)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceVulkan12Features", "shaderStorageImageArrayNonUniformIndexing");
         if (b->shaderInputAttachmentArrayNonUniformIndexing && !a->shaderInputAttachmentArrayNonUniformIndexing)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceVulkan12Features", "shaderInputAttachmentArrayNonUniformIndexing");
         if (b->shaderUniformTexelBufferArrayNonUniformIndexing && !a->shaderUniformTexelBufferArrayNonUniformIndexing)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceVulkan12Features", "shaderUniformTexelBufferArrayNonUniformIndexing");
         if (b->shaderStorageTexelBufferArrayNonUniformIndexing && !a->shaderStorageTexelBufferArrayNonUniformIndexing)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceVulkan12Features", "shaderStorageTexelBufferArrayNonUniformIndexing");
         if (b->descriptorBindingUniformBufferUpdateAfterBind && !a->descriptorBindingUniformBufferUpdateAfterBind)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceVulkan12Features", "descriptorBindingUniformBufferUpdateAfterBind");
         if (b->descriptorBindingSampledImageUpdateAfterBind && !a->descriptorBindingSampledImageUpdateAfterBind)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceVulkan12Features", "descriptorBindingSampledImageUpdateAfterBind");
         if (b->descriptorBindingStorageImageUpdateAfterBind && !a->descriptorBindingStorageImageUpdateAfterBind)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceVulkan12Features", "descriptorBindingStorageImageUpdateAfterBind");
         if (b->descriptorBindingStorageBufferUpdateAfterBind && !a->descriptorBindingStorageBufferUpdateAfterBind)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceVulkan12Features", "descriptorBindingStorageBufferUpdateAfterBind");
         if (b->descriptorBindingUniformTexelBufferUpdateAfterBind && !a->descriptorBindingUniformTexelBufferUpdateAfterBind)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceVulkan12Features", "descriptorBindingUniformTexelBufferUpdateAfterBind");
         if (b->descriptorBindingStorageTexelBufferUpdateAfterBind && !a->descriptorBindingStorageTexelBufferUpdateAfterBind)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceVulkan12Features", "descriptorBindingStorageTexelBufferUpdateAfterBind");
         if (b->descriptorBindingUpdateUnusedWhilePending && !a->descriptorBindingUpdateUnusedWhilePending)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceVulkan12Features", "descriptorBindingUpdateUnusedWhilePending");
         if (b->descriptorBindingPartiallyBound && !a->descriptorBindingPartiallyBound)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceVulkan12Features", "descriptorBindingPartiallyBound");
         if (b->descriptorBindingVariableDescriptorCount && !a->descriptorBindingVariableDescriptorCount)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceVulkan12Features", "descriptorBindingVariableDescriptorCount");
         if (b->runtimeDescriptorArray && !a->runtimeDescriptorArray)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceVulkan12Features", "runtimeDescriptorArray");
         if (b->samplerFilterMinmax && !a->samplerFilterMinmax)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceVulkan12Features", "samplerFilterMinmax");
         if (b->scalarBlockLayout && !a->scalarBlockLayout)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceVulkan12Features", "scalarBlockLayout");
         if (b->imagelessFramebuffer && !a->imagelessFramebuffer)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceVulkan12Features", "imagelessFramebuffer");
         if (b->uniformBufferStandardLayout && !a->uniformBufferStandardLayout)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceVulkan12Features", "uniformBufferStandardLayout");
         if (b->shaderSubgroupExtendedTypes && !a->shaderSubgroupExtendedTypes)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceVulkan12Features", "shaderSubgroupExtendedTypes");
         if (b->separateDepthStencilLayouts && !a->separateDepthStencilLayouts)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceVulkan12Features", "separateDepthStencilLayouts");
         if (b->hostQueryReset && !a->hostQueryReset)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceVulkan12Features", "hostQueryReset");
         if (b->timelineSemaphore && !a->timelineSemaphore)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceVulkan12Features", "timelineSemaphore");
         if (b->bufferDeviceAddress && !a->bufferDeviceAddress)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceVulkan12Features", "bufferDeviceAddress");
         if (b->bufferDeviceAddressCaptureReplay && !a->bufferDeviceAddressCaptureReplay)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceVulkan12Features", "bufferDeviceAddressCaptureReplay");
         if (b->bufferDeviceAddressMultiDevice && !a->bufferDeviceAddressMultiDevice)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceVulkan12Features", "bufferDeviceAddressMultiDevice");
         if (b->vulkanMemoryModel && !a->vulkanMemoryModel)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceVulkan12Features", "vulkanMemoryModel");
         if (b->vulkanMemoryModelDeviceScope && !a->vulkanMemoryModelDeviceScope)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceVulkan12Features", "vulkanMemoryModelDeviceScope");
         if (b->vulkanMemoryModelAvailabilityVisibilityChains && !a->vulkanMemoryModelAvailabilityVisibilityChains)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceVulkan12Features", "vulkanMemoryModelAvailabilityVisibilityChains");
         if (b->shaderOutputViewportIndex && !a->shaderOutputViewportIndex)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceVulkan12Features", "shaderOutputViewportIndex");
         if (b->shaderOutputLayer && !a->shaderOutputLayer)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceVulkan12Features", "shaderOutputLayer");
         if (b->subgroupBroadcastDynamicId && !a->subgroupBroadcastDynamicId)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceVulkan12Features", "subgroupBroadcastDynamicId");
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_COHERENT_MEMORY_FEATURES_AMD : {
         VkPhysicalDeviceCoherentMemoryFeaturesAMD *a = &supported_VkPhysicalDeviceCoherentMemoryFeaturesAMD;
         VkPhysicalDeviceCoherentMemoryFeaturesAMD *b = (VkPhysicalDeviceCoherentMemoryFeaturesAMD *) feat;
         if (b->deviceCoherentMemory && !a->deviceCoherentMemory)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceCoherentMemoryFeaturesAMD", "deviceCoherentMemory");
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_CUSTOM_BORDER_COLOR_FEATURES_EXT : {
         VkPhysicalDeviceCustomBorderColorFeaturesEXT *a = &supported_VkPhysicalDeviceCustomBorderColorFeaturesEXT;
         VkPhysicalDeviceCustomBorderColorFeaturesEXT *b = (VkPhysicalDeviceCustomBorderColorFeaturesEXT *) feat;
         if (b->customBorderColors && !a->customBorderColors)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceCustomBorderColorFeaturesEXT", "customBorderColors");
         if (b->customBorderColorWithoutFormat && !a->customBorderColorWithoutFormat)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceCustomBorderColorFeaturesEXT", "customBorderColorWithoutFormat");
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTENDED_DYNAMIC_STATE_FEATURES_EXT : {
         VkPhysicalDeviceExtendedDynamicStateFeaturesEXT *a = &supported_VkPhysicalDeviceExtendedDynamicStateFeaturesEXT;
         VkPhysicalDeviceExtendedDynamicStateFeaturesEXT *b = (VkPhysicalDeviceExtendedDynamicStateFeaturesEXT *) feat;
         if (b->extendedDynamicState && !a->extendedDynamicState)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceExtendedDynamicStateFeaturesEXT", "extendedDynamicState");
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTENDED_DYNAMIC_STATE_2_FEATURES_EXT : {
         VkPhysicalDeviceExtendedDynamicState2FeaturesEXT *a = &supported_VkPhysicalDeviceExtendedDynamicState2FeaturesEXT;
         VkPhysicalDeviceExtendedDynamicState2FeaturesEXT *b = (VkPhysicalDeviceExtendedDynamicState2FeaturesEXT *) feat;
         if (b->extendedDynamicState2 && !a->extendedDynamicState2)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceExtendedDynamicState2FeaturesEXT", "extendedDynamicState2");
         if (b->extendedDynamicState2LogicOp && !a->extendedDynamicState2LogicOp)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceExtendedDynamicState2FeaturesEXT", "extendedDynamicState2LogicOp");
         if (b->extendedDynamicState2PatchControlPoints && !a->extendedDynamicState2PatchControlPoints)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceExtendedDynamicState2FeaturesEXT", "extendedDynamicState2PatchControlPoints");
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DIAGNOSTICS_CONFIG_FEATURES_NV : {
         VkPhysicalDeviceDiagnosticsConfigFeaturesNV *a = &supported_VkPhysicalDeviceDiagnosticsConfigFeaturesNV;
         VkPhysicalDeviceDiagnosticsConfigFeaturesNV *b = (VkPhysicalDeviceDiagnosticsConfigFeaturesNV *) feat;
         if (b->diagnosticsConfig && !a->diagnosticsConfig)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceDiagnosticsConfigFeaturesNV", "diagnosticsConfig");
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ZERO_INITIALIZE_WORKGROUP_MEMORY_FEATURES_KHR : {
         VkPhysicalDeviceZeroInitializeWorkgroupMemoryFeaturesKHR *a = &supported_VkPhysicalDeviceZeroInitializeWorkgroupMemoryFeaturesKHR;
         VkPhysicalDeviceZeroInitializeWorkgroupMemoryFeaturesKHR *b = (VkPhysicalDeviceZeroInitializeWorkgroupMemoryFeaturesKHR *) feat;
         if (b->shaderZeroInitializeWorkgroupMemory && !a->shaderZeroInitializeWorkgroupMemory)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceZeroInitializeWorkgroupMemoryFeaturesKHR", "shaderZeroInitializeWorkgroupMemory");
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_SUBGROUP_UNIFORM_CONTROL_FLOW_FEATURES_KHR : {
         VkPhysicalDeviceShaderSubgroupUniformControlFlowFeaturesKHR *a = &supported_VkPhysicalDeviceShaderSubgroupUniformControlFlowFeaturesKHR;
         VkPhysicalDeviceShaderSubgroupUniformControlFlowFeaturesKHR *b = (VkPhysicalDeviceShaderSubgroupUniformControlFlowFeaturesKHR *) feat;
         if (b->shaderSubgroupUniformControlFlow && !a->shaderSubgroupUniformControlFlow)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceShaderSubgroupUniformControlFlowFeaturesKHR", "shaderSubgroupUniformControlFlow");
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ROBUSTNESS_2_FEATURES_EXT : {
         VkPhysicalDeviceRobustness2FeaturesEXT *a = &supported_VkPhysicalDeviceRobustness2FeaturesEXT;
         VkPhysicalDeviceRobustness2FeaturesEXT *b = (VkPhysicalDeviceRobustness2FeaturesEXT *) feat;
         if (b->robustBufferAccess2 && !a->robustBufferAccess2)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceRobustness2FeaturesEXT", "robustBufferAccess2");
         if (b->robustImageAccess2 && !a->robustImageAccess2)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceRobustness2FeaturesEXT", "robustImageAccess2");
         if (b->nullDescriptor && !a->nullDescriptor)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceRobustness2FeaturesEXT", "nullDescriptor");
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_IMAGE_ROBUSTNESS_FEATURES_EXT : {
         VkPhysicalDeviceImageRobustnessFeaturesEXT *a = &supported_VkPhysicalDeviceImageRobustnessFeaturesEXT;
         VkPhysicalDeviceImageRobustnessFeaturesEXT *b = (VkPhysicalDeviceImageRobustnessFeaturesEXT *) feat;
         if (b->robustImageAccess && !a->robustImageAccess)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceImageRobustnessFeaturesEXT", "robustImageAccess");
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_WORKGROUP_MEMORY_EXPLICIT_LAYOUT_FEATURES_KHR : {
         VkPhysicalDeviceWorkgroupMemoryExplicitLayoutFeaturesKHR *a = &supported_VkPhysicalDeviceWorkgroupMemoryExplicitLayoutFeaturesKHR;
         VkPhysicalDeviceWorkgroupMemoryExplicitLayoutFeaturesKHR *b = (VkPhysicalDeviceWorkgroupMemoryExplicitLayoutFeaturesKHR *) feat;
         if (b->workgroupMemoryExplicitLayout && !a->workgroupMemoryExplicitLayout)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceWorkgroupMemoryExplicitLayoutFeaturesKHR", "workgroupMemoryExplicitLayout");
         if (b->workgroupMemoryExplicitLayoutScalarBlockLayout && !a->workgroupMemoryExplicitLayoutScalarBlockLayout)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceWorkgroupMemoryExplicitLayoutFeaturesKHR", "workgroupMemoryExplicitLayoutScalarBlockLayout");
         if (b->workgroupMemoryExplicitLayout8BitAccess && !a->workgroupMemoryExplicitLayout8BitAccess)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceWorkgroupMemoryExplicitLayoutFeaturesKHR", "workgroupMemoryExplicitLayout8BitAccess");
         if (b->workgroupMemoryExplicitLayout16BitAccess && !a->workgroupMemoryExplicitLayout16BitAccess)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceWorkgroupMemoryExplicitLayoutFeaturesKHR", "workgroupMemoryExplicitLayout16BitAccess");
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_4444_FORMATS_FEATURES_EXT : {
         VkPhysicalDevice4444FormatsFeaturesEXT *a = &supported_VkPhysicalDevice4444FormatsFeaturesEXT;
         VkPhysicalDevice4444FormatsFeaturesEXT *b = (VkPhysicalDevice4444FormatsFeaturesEXT *) feat;
         if (b->formatA4R4G4B4 && !a->formatA4R4G4B4)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDevice4444FormatsFeaturesEXT", "formatA4R4G4B4");
         if (b->formatA4B4G4R4 && !a->formatA4B4G4R4)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDevice4444FormatsFeaturesEXT", "formatA4B4G4R4");
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SUBPASS_SHADING_FEATURES_HUAWEI : {
         VkPhysicalDeviceSubpassShadingFeaturesHUAWEI *a = &supported_VkPhysicalDeviceSubpassShadingFeaturesHUAWEI;
         VkPhysicalDeviceSubpassShadingFeaturesHUAWEI *b = (VkPhysicalDeviceSubpassShadingFeaturesHUAWEI *) feat;
         if (b->subpassShading && !a->subpassShading)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceSubpassShadingFeaturesHUAWEI", "subpassShading");
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_IMAGE_ATOMIC_INT64_FEATURES_EXT : {
         VkPhysicalDeviceShaderImageAtomicInt64FeaturesEXT *a = &supported_VkPhysicalDeviceShaderImageAtomicInt64FeaturesEXT;
         VkPhysicalDeviceShaderImageAtomicInt64FeaturesEXT *b = (VkPhysicalDeviceShaderImageAtomicInt64FeaturesEXT *) feat;
         if (b->shaderImageInt64Atomics && !a->shaderImageInt64Atomics)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceShaderImageAtomicInt64FeaturesEXT", "shaderImageInt64Atomics");
         if (b->sparseImageInt64Atomics && !a->sparseImageInt64Atomics)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceShaderImageAtomicInt64FeaturesEXT", "sparseImageInt64Atomics");
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FRAGMENT_SHADING_RATE_FEATURES_KHR : {
         VkPhysicalDeviceFragmentShadingRateFeaturesKHR *a = &supported_VkPhysicalDeviceFragmentShadingRateFeaturesKHR;
         VkPhysicalDeviceFragmentShadingRateFeaturesKHR *b = (VkPhysicalDeviceFragmentShadingRateFeaturesKHR *) feat;
         if (b->pipelineFragmentShadingRate && !a->pipelineFragmentShadingRate)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceFragmentShadingRateFeaturesKHR", "pipelineFragmentShadingRate");
         if (b->primitiveFragmentShadingRate && !a->primitiveFragmentShadingRate)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceFragmentShadingRateFeaturesKHR", "primitiveFragmentShadingRate");
         if (b->attachmentFragmentShadingRate && !a->attachmentFragmentShadingRate)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceFragmentShadingRateFeaturesKHR", "attachmentFragmentShadingRate");
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_TERMINATE_INVOCATION_FEATURES_KHR : {
         VkPhysicalDeviceShaderTerminateInvocationFeaturesKHR *a = &supported_VkPhysicalDeviceShaderTerminateInvocationFeaturesKHR;
         VkPhysicalDeviceShaderTerminateInvocationFeaturesKHR *b = (VkPhysicalDeviceShaderTerminateInvocationFeaturesKHR *) feat;
         if (b->shaderTerminateInvocation && !a->shaderTerminateInvocation)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceShaderTerminateInvocationFeaturesKHR", "shaderTerminateInvocation");
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FRAGMENT_SHADING_RATE_ENUMS_FEATURES_NV : {
         VkPhysicalDeviceFragmentShadingRateEnumsFeaturesNV *a = &supported_VkPhysicalDeviceFragmentShadingRateEnumsFeaturesNV;
         VkPhysicalDeviceFragmentShadingRateEnumsFeaturesNV *b = (VkPhysicalDeviceFragmentShadingRateEnumsFeaturesNV *) feat;
         if (b->fragmentShadingRateEnums && !a->fragmentShadingRateEnums)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceFragmentShadingRateEnumsFeaturesNV", "fragmentShadingRateEnums");
         if (b->supersampleFragmentShadingRates && !a->supersampleFragmentShadingRates)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceFragmentShadingRateEnumsFeaturesNV", "supersampleFragmentShadingRates");
         if (b->noInvocationFragmentShadingRates && !a->noInvocationFragmentShadingRates)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceFragmentShadingRateEnumsFeaturesNV", "noInvocationFragmentShadingRates");
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MUTABLE_DESCRIPTOR_TYPE_FEATURES_VALVE : {
         VkPhysicalDeviceMutableDescriptorTypeFeaturesVALVE *a = &supported_VkPhysicalDeviceMutableDescriptorTypeFeaturesVALVE;
         VkPhysicalDeviceMutableDescriptorTypeFeaturesVALVE *b = (VkPhysicalDeviceMutableDescriptorTypeFeaturesVALVE *) feat;
         if (b->mutableDescriptorType && !a->mutableDescriptorType)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceMutableDescriptorTypeFeaturesVALVE", "mutableDescriptorType");
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VERTEX_INPUT_DYNAMIC_STATE_FEATURES_EXT : {
         VkPhysicalDeviceVertexInputDynamicStateFeaturesEXT *a = &supported_VkPhysicalDeviceVertexInputDynamicStateFeaturesEXT;
         VkPhysicalDeviceVertexInputDynamicStateFeaturesEXT *b = (VkPhysicalDeviceVertexInputDynamicStateFeaturesEXT *) feat;
         if (b->vertexInputDynamicState && !a->vertexInputDynamicState)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceVertexInputDynamicStateFeaturesEXT", "vertexInputDynamicState");
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTERNAL_MEMORY_RDMA_FEATURES_NV : {
         VkPhysicalDeviceExternalMemoryRDMAFeaturesNV *a = &supported_VkPhysicalDeviceExternalMemoryRDMAFeaturesNV;
         VkPhysicalDeviceExternalMemoryRDMAFeaturesNV *b = (VkPhysicalDeviceExternalMemoryRDMAFeaturesNV *) feat;
         if (b->externalMemoryRDMA && !a->externalMemoryRDMA)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceExternalMemoryRDMAFeaturesNV", "externalMemoryRDMA");
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_COLOR_WRITE_ENABLE_FEATURES_EXT : {
         VkPhysicalDeviceColorWriteEnableFeaturesEXT *a = &supported_VkPhysicalDeviceColorWriteEnableFeaturesEXT;
         VkPhysicalDeviceColorWriteEnableFeaturesEXT *b = (VkPhysicalDeviceColorWriteEnableFeaturesEXT *) feat;
         if (b->colorWriteEnable && !a->colorWriteEnable)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceColorWriteEnableFeaturesEXT", "colorWriteEnable");
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SYNCHRONIZATION_2_FEATURES_KHR : {
         VkPhysicalDeviceSynchronization2FeaturesKHR *a = &supported_VkPhysicalDeviceSynchronization2FeaturesKHR;
         VkPhysicalDeviceSynchronization2FeaturesKHR *b = (VkPhysicalDeviceSynchronization2FeaturesKHR *) feat;
         if (b->synchronization2 && !a->synchronization2)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceSynchronization2FeaturesKHR", "synchronization2");
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_INHERITED_VIEWPORT_SCISSOR_FEATURES_NV : {
         VkPhysicalDeviceInheritedViewportScissorFeaturesNV *a = &supported_VkPhysicalDeviceInheritedViewportScissorFeaturesNV;
         VkPhysicalDeviceInheritedViewportScissorFeaturesNV *b = (VkPhysicalDeviceInheritedViewportScissorFeaturesNV *) feat;
         if (b->inheritedViewportScissor2D && !a->inheritedViewportScissor2D)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceInheritedViewportScissorFeaturesNV", "inheritedViewportScissor2D");
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_YCBCR_2_PLANE_444_FORMATS_FEATURES_EXT : {
         VkPhysicalDeviceYcbcr2Plane444FormatsFeaturesEXT *a = &supported_VkPhysicalDeviceYcbcr2Plane444FormatsFeaturesEXT;
         VkPhysicalDeviceYcbcr2Plane444FormatsFeaturesEXT *b = (VkPhysicalDeviceYcbcr2Plane444FormatsFeaturesEXT *) feat;
         if (b->ycbcr2plane444Formats && !a->ycbcr2plane444Formats)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceYcbcr2Plane444FormatsFeaturesEXT", "ycbcr2plane444Formats");
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROVOKING_VERTEX_FEATURES_EXT : {
         VkPhysicalDeviceProvokingVertexFeaturesEXT *a = &supported_VkPhysicalDeviceProvokingVertexFeaturesEXT;
         VkPhysicalDeviceProvokingVertexFeaturesEXT *b = (VkPhysicalDeviceProvokingVertexFeaturesEXT *) feat;
         if (b->provokingVertexLast && !a->provokingVertexLast)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceProvokingVertexFeaturesEXT", "provokingVertexLast");
         if (b->transformFeedbackPreservesProvokingVertex && !a->transformFeedbackPreservesProvokingVertex)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceProvokingVertexFeaturesEXT", "transformFeedbackPreservesProvokingVertex");
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_INTEGER_DOT_PRODUCT_FEATURES_KHR : {
         VkPhysicalDeviceShaderIntegerDotProductFeaturesKHR *a = &supported_VkPhysicalDeviceShaderIntegerDotProductFeaturesKHR;
         VkPhysicalDeviceShaderIntegerDotProductFeaturesKHR *b = (VkPhysicalDeviceShaderIntegerDotProductFeaturesKHR *) feat;
         if (b->shaderIntegerDotProduct && !a->shaderIntegerDotProduct)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceShaderIntegerDotProductFeaturesKHR", "shaderIntegerDotProduct");
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_MOTION_BLUR_FEATURES_NV : {
         VkPhysicalDeviceRayTracingMotionBlurFeaturesNV *a = &supported_VkPhysicalDeviceRayTracingMotionBlurFeaturesNV;
         VkPhysicalDeviceRayTracingMotionBlurFeaturesNV *b = (VkPhysicalDeviceRayTracingMotionBlurFeaturesNV *) feat;
         if (b->rayTracingMotionBlur && !a->rayTracingMotionBlur)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceRayTracingMotionBlurFeaturesNV", "rayTracingMotionBlur");
         if (b->rayTracingMotionBlurPipelineTraceRaysIndirect && !a->rayTracingMotionBlurPipelineTraceRaysIndirect)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceRayTracingMotionBlurFeaturesNV", "rayTracingMotionBlurPipelineTraceRaysIndirect");
         break;
      }
      case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RGBA10X6_FORMATS_FEATURES_EXT : {
         VkPhysicalDeviceRGBA10X6FormatsFeaturesEXT *a = &supported_VkPhysicalDeviceRGBA10X6FormatsFeaturesEXT;
         VkPhysicalDeviceRGBA10X6FormatsFeaturesEXT *b = (VkPhysicalDeviceRGBA10X6FormatsFeaturesEXT *) feat;
         if (b->formatRgba10x6WithoutYCbCrSampler && !a->formatRgba10x6WithoutYCbCrSampler)
            return vk_errorf(physical_device, VK_ERROR_FEATURE_NOT_PRESENT,
                             "%s.%s not supported", "VkPhysicalDeviceRGBA10X6FormatsFeaturesEXT", "formatRgba10x6WithoutYCbCrSampler");
         break;
      }
      default:
         break;
      }
   } // for each extension structure
   return VK_SUCCESS;
}

