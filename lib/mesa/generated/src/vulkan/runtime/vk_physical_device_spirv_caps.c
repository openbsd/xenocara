
/* Copyright 2024 Valve Corporation
 * Copyright 2021 Intel Corporation
 * SPDX-License-Identifier: MIT
 */

/* This file generated from vk_physical_device_spirv_caps_gen.py, don't edit directly. */

#include "vk_physical_device.h"
#include "vk_instance.h"
#include "vk_shader.h"

/* for spirv_supported_capabilities */
#include "compiler/spirv/spirv_info.h"

struct spirv_capabilities
vk_physical_device_get_spirv_capabilities(const struct vk_physical_device *pdev)
{
   const struct vk_features *f = &pdev->supported_features;
   const struct vk_device_extension_table *e = &pdev->supported_extensions;
   const struct vk_properties *p = &pdev->properties;
   uint32_t api_version = pdev->instance->app_info.api_version;

   struct spirv_capabilities caps = { false, };

   /* We |= for everything because some caps have multiple names but the
    * same enum value and they sometimes have different enables in the
    * Vulkan spec.  To handle this, we just | all the enables together.
    */
    caps.Matrix |= (api_version >= VK_API_VERSION_1_0);
    caps.Shader |= (api_version >= VK_API_VERSION_1_0);
    caps.InputAttachment |= (api_version >= VK_API_VERSION_1_0);
    caps.Sampled1D |= (api_version >= VK_API_VERSION_1_0);
    caps.Image1D |= (api_version >= VK_API_VERSION_1_0);
    caps.SampledBuffer |= (api_version >= VK_API_VERSION_1_0);
    caps.ImageBuffer |= (api_version >= VK_API_VERSION_1_0);
    caps.ImageQuery |= (api_version >= VK_API_VERSION_1_0);
    caps.DerivativeControl |= (api_version >= VK_API_VERSION_1_0);
    caps.Geometry |= f->geometryShader;
    caps.Tessellation |= f->tessellationShader;
    caps.Float64 |= f->shaderFloat64;
    caps.Int64 |= f->shaderInt64;
    caps.Int64Atomics |= f->shaderBufferInt64Atomics | f->shaderSharedInt64Atomics | f->shaderImageInt64Atomics;
    caps.AtomicFloat16AddEXT |= f->shaderBufferFloat16AtomicAdd | f->shaderSharedFloat16AtomicAdd;
    caps.AtomicFloat32AddEXT |= f->shaderBufferFloat32AtomicAdd | f->shaderSharedFloat32AtomicAdd | f->shaderImageFloat32AtomicAdd;
    caps.AtomicFloat64AddEXT |= f->shaderBufferFloat64AtomicAdd | f->shaderSharedFloat64AtomicAdd;
    caps.AtomicFloat16MinMaxEXT |= f->shaderBufferFloat16AtomicMinMax | f->shaderSharedFloat16AtomicMinMax;
    caps.AtomicFloat32MinMaxEXT |= f->shaderBufferFloat32AtomicMinMax | f->shaderSharedFloat32AtomicMinMax | f->shaderImageFloat32AtomicMinMax;
    caps.AtomicFloat64MinMaxEXT |= f->shaderBufferFloat64AtomicMinMax | f->shaderSharedFloat64AtomicMinMax;
    caps.AtomicFloat16VectorNV |= f->shaderFloat16VectorAtomics;
    caps.Int64ImageEXT |= f->shaderImageInt64Atomics;
    caps.Int16 |= f->shaderInt16;
    caps.TessellationPointSize |= f->shaderTessellationAndGeometryPointSize;
    caps.GeometryPointSize |= f->shaderTessellationAndGeometryPointSize;
    caps.ImageGatherExtended |= f->shaderImageGatherExtended;
    caps.StorageImageMultisample |= f->shaderStorageImageMultisample;
    caps.UniformBufferArrayDynamicIndexing |= f->shaderUniformBufferArrayDynamicIndexing;
    caps.SampledImageArrayDynamicIndexing |= f->shaderSampledImageArrayDynamicIndexing;
    caps.StorageBufferArrayDynamicIndexing |= f->shaderStorageBufferArrayDynamicIndexing;
    caps.StorageImageArrayDynamicIndexing |= f->shaderStorageImageArrayDynamicIndexing;
    caps.ClipDistance |= f->shaderClipDistance;
    caps.CullDistance |= f->shaderCullDistance;
    caps.ImageCubeArray |= f->imageCubeArray;
    caps.SampleRateShading |= f->sampleRateShading;
    caps.SparseResidency |= f->shaderResourceResidency;
    caps.MinLod |= f->shaderResourceMinLod;
    caps.SampledCubeArray |= f->imageCubeArray;
    caps.ImageMSArray |= f->shaderStorageImageMultisample;
    caps.StorageImageExtendedFormats |= (api_version >= VK_API_VERSION_1_0);
    caps.InterpolationFunction |= f->sampleRateShading;
    caps.StorageImageReadWithoutFormat |= f->shaderStorageImageReadWithoutFormat | (api_version >= VK_API_VERSION_1_3) | e->KHR_format_feature_flags2;
    caps.StorageImageWriteWithoutFormat |= f->shaderStorageImageWriteWithoutFormat | (api_version >= VK_API_VERSION_1_3) | e->KHR_format_feature_flags2;
    caps.MultiViewport |= f->multiViewport;
    caps.DrawParameters |= f->shaderDrawParameters | e->KHR_shader_draw_parameters;
    caps.MultiView |= f->multiview;
    caps.DeviceGroup |= (api_version >= VK_API_VERSION_1_1) | e->KHR_device_group;
    caps.VariablePointersStorageBuffer |= f->variablePointersStorageBuffer;
    caps.VariablePointers |= f->variablePointers;
    caps.ShaderClockKHR |= e->KHR_shader_clock;
    caps.StencilExportEXT |= e->EXT_shader_stencil_export;
    caps.SubgroupBallotKHR |= e->EXT_shader_subgroup_ballot;
    caps.SubgroupVoteKHR |= e->EXT_shader_subgroup_vote;
    caps.ImageReadWriteLodAMD |= e->AMD_shader_image_load_store_lod;
    caps.ImageGatherBiasLodAMD |= e->AMD_texture_gather_bias_lod;
    caps.FragmentMaskAMD |= e->AMD_shader_fragment_mask;
    caps.SampleMaskOverrideCoverageNV |= e->NV_sample_mask_override_coverage;
    caps.GeometryShaderPassthroughNV |= e->NV_geometry_shader_passthrough;
    caps.ShaderViewportIndex |= f->shaderOutputViewportIndex;
    caps.ShaderLayer |= f->shaderOutputLayer;
    caps.ShaderViewportIndexLayerEXT |= e->EXT_shader_viewport_index_layer | e->NV_viewport_array2;
    caps.ShaderViewportMaskNV |= e->NV_viewport_array2;
    caps.PerViewAttributesNV |= e->NVX_multiview_per_view_attributes;
    caps.StorageBuffer16BitAccess |= f->storageBuffer16BitAccess;
    caps.UniformAndStorageBuffer16BitAccess |= f->uniformAndStorageBuffer16BitAccess;
    caps.StoragePushConstant16 |= f->storagePushConstant16;
    caps.StorageInputOutput16 |= f->storageInputOutput16;
    caps.GroupNonUniform |= (p->subgroupSupportedOperations & VK_SUBGROUP_FEATURE_BASIC_BIT);
    caps.GroupNonUniformVote |= (p->subgroupSupportedOperations & VK_SUBGROUP_FEATURE_VOTE_BIT);
    caps.GroupNonUniformArithmetic |= (p->subgroupSupportedOperations & VK_SUBGROUP_FEATURE_ARITHMETIC_BIT);
    caps.GroupNonUniformBallot |= (p->subgroupSupportedOperations & VK_SUBGROUP_FEATURE_BALLOT_BIT);
    caps.GroupNonUniformShuffle |= (p->subgroupSupportedOperations & VK_SUBGROUP_FEATURE_SHUFFLE_BIT);
    caps.GroupNonUniformShuffleRelative |= (p->subgroupSupportedOperations & VK_SUBGROUP_FEATURE_SHUFFLE_RELATIVE_BIT);
    caps.GroupNonUniformClustered |= (p->subgroupSupportedOperations & VK_SUBGROUP_FEATURE_CLUSTERED_BIT);
    caps.GroupNonUniformQuad |= (p->subgroupSupportedOperations & VK_SUBGROUP_FEATURE_QUAD_BIT);
    caps.GroupNonUniformPartitionedNV |= (p->subgroupSupportedOperations & VK_SUBGROUP_FEATURE_PARTITIONED_BIT_NV);
    caps.SampleMaskPostDepthCoverage |= e->EXT_post_depth_coverage;
    caps.ShaderNonUniform |= (api_version >= VK_API_VERSION_1_2) | e->EXT_descriptor_indexing;
    caps.RuntimeDescriptorArray |= f->runtimeDescriptorArray;
    caps.InputAttachmentArrayDynamicIndexing |= f->shaderInputAttachmentArrayDynamicIndexing;
    caps.UniformTexelBufferArrayDynamicIndexing |= f->shaderUniformTexelBufferArrayDynamicIndexing;
    caps.StorageTexelBufferArrayDynamicIndexing |= f->shaderStorageTexelBufferArrayDynamicIndexing;
    caps.UniformBufferArrayNonUniformIndexing |= f->shaderUniformBufferArrayNonUniformIndexing;
    caps.SampledImageArrayNonUniformIndexing |= f->shaderSampledImageArrayNonUniformIndexing;
    caps.StorageBufferArrayNonUniformIndexing |= f->shaderStorageBufferArrayNonUniformIndexing;
    caps.StorageImageArrayNonUniformIndexing |= f->shaderStorageImageArrayNonUniformIndexing;
    caps.InputAttachmentArrayNonUniformIndexing |= f->shaderInputAttachmentArrayNonUniformIndexing;
    caps.UniformTexelBufferArrayNonUniformIndexing |= f->shaderUniformTexelBufferArrayNonUniformIndexing;
    caps.StorageTexelBufferArrayNonUniformIndexing |= f->shaderStorageTexelBufferArrayNonUniformIndexing;
    caps.FragmentFullyCoveredEXT |= e->EXT_conservative_rasterization;
    caps.Float16 |= f->shaderFloat16 | e->AMD_gpu_shader_half_float;
    caps.Int8 |= f->shaderInt8;
    caps.StorageBuffer8BitAccess |= f->storageBuffer8BitAccess;
    caps.UniformAndStorageBuffer8BitAccess |= f->uniformAndStorageBuffer8BitAccess;
    caps.StoragePushConstant8 |= f->storagePushConstant8;
    caps.VulkanMemoryModel |= f->vulkanMemoryModel;
    caps.VulkanMemoryModelDeviceScope |= f->vulkanMemoryModelDeviceScope;
    caps.DenormPreserve |= p->shaderDenormPreserveFloat16 | p->shaderDenormPreserveFloat32 | p->shaderDenormPreserveFloat64;
    caps.DenormFlushToZero |= p->shaderDenormFlushToZeroFloat16 | p->shaderDenormFlushToZeroFloat32 | p->shaderDenormFlushToZeroFloat64;
    caps.SignedZeroInfNanPreserve |= p->shaderSignedZeroInfNanPreserveFloat16 | p->shaderSignedZeroInfNanPreserveFloat32 | p->shaderSignedZeroInfNanPreserveFloat64;
    caps.RoundingModeRTE |= p->shaderRoundingModeRTEFloat16 | p->shaderRoundingModeRTEFloat32 | p->shaderRoundingModeRTEFloat64;
    caps.RoundingModeRTZ |= p->shaderRoundingModeRTZFloat16 | p->shaderRoundingModeRTZFloat32 | p->shaderRoundingModeRTZFloat64;
    caps.ComputeDerivativeGroupQuadsKHR |= f->computeDerivativeGroupQuads;
    caps.ComputeDerivativeGroupLinearKHR |= f->computeDerivativeGroupLinear;
    caps.ImageFootprintNV |= f->imageFootprint;
    caps.MeshShadingNV |= e->NV_mesh_shader;
    caps.RayTracingKHR |= f->rayTracingPipeline;
    caps.RayQueryKHR |= f->rayQuery;
    caps.RayTraversalPrimitiveCullingKHR |= f->rayTraversalPrimitiveCulling | f->rayQuery;
    caps.RayCullMaskKHR |= f->rayTracingMaintenance1;
    caps.RayTracingNV |= e->NV_ray_tracing;
    caps.RayTracingMotionBlurNV |= f->rayTracingMotionBlur;
    caps.TransformFeedback |= f->transformFeedback;
    caps.GeometryStreams |= f->geometryStreams;
    caps.FragmentDensityEXT |= f->fragmentDensityMap | f->shadingRateImage;
    caps.PhysicalStorageBufferAddresses |= f->bufferDeviceAddress;
    caps.CooperativeMatrixNV |= f->cooperativeMatrixNV;
    caps.IntegerFunctions2INTEL |= f->shaderIntegerFunctions2;
    caps.ShaderSMBuiltinsNV |= f->shaderSMBuiltins;
    caps.FragmentShaderSampleInterlockEXT |= f->fragmentShaderSampleInterlock;
    caps.FragmentShaderPixelInterlockEXT |= f->fragmentShaderPixelInterlock;
    caps.FragmentShaderShadingRateInterlockEXT |= f->fragmentShaderShadingRateInterlock | f->shadingRateImage;
    caps.DemoteToHelperInvocation |= f->shaderDemoteToHelperInvocation;
    caps.FragmentShadingRateKHR |= f->pipelineFragmentShadingRate | f->primitiveFragmentShadingRate | f->attachmentFragmentShadingRate;
    caps.WorkgroupMemoryExplicitLayoutKHR |= f->workgroupMemoryExplicitLayout;
    caps.WorkgroupMemoryExplicitLayout8BitAccessKHR |= f->workgroupMemoryExplicitLayout8BitAccess;
    caps.WorkgroupMemoryExplicitLayout16BitAccessKHR |= f->workgroupMemoryExplicitLayout16BitAccess;
    caps.DotProductInputAll |= f->shaderIntegerDotProduct;
    caps.DotProductInput4x8Bit |= f->shaderIntegerDotProduct;
    caps.DotProductInput4x8BitPacked |= f->shaderIntegerDotProduct;
    caps.DotProduct |= f->shaderIntegerDotProduct;
    caps.FragmentBarycentricKHR |= f->fragmentShaderBarycentric;
    caps.TextureSampleWeightedQCOM |= f->textureSampleWeighted;
    caps.TextureBoxFilterQCOM |= f->textureBoxFilter;
    caps.TextureBlockMatchQCOM |= f->textureBlockMatch;
    caps.TextureBlockMatch2QCOM |= f->textureBlockMatch2;
    caps.MeshShadingEXT |= e->EXT_mesh_shader;
    caps.RayTracingOpacityMicromapEXT |= e->EXT_opacity_micromap;
    caps.CoreBuiltinsARM |= f->shaderCoreBuiltins;
    caps.ShaderInvocationReorderNV |= e->NV_ray_tracing_invocation_reorder;
    caps.RayTracingPositionFetchKHR |= f->rayTracingPositionFetch;
    caps.RayQueryPositionFetchKHR |= f->rayTracingPositionFetch;
    caps.TileImageColorReadAccessEXT |= f->shaderTileImageColorReadAccess;
    caps.TileImageDepthReadAccessEXT |= f->shaderTileImageDepthReadAccess;
    caps.TileImageStencilReadAccessEXT |= f->shaderTileImageStencilReadAccess;
    caps.CooperativeMatrixKHR |= f->cooperativeMatrix;
    caps.GroupNonUniformRotateKHR |= f->shaderSubgroupRotate;
    caps.ExpectAssumeKHR |= f->shaderExpectAssume;
    caps.FloatControls2 |= f->shaderFloatControls2;
    caps.QuadControlKHR |= f->shaderQuadControl;
    caps.RawAccessChainsNV |= f->shaderRawAccessChains;
    caps.ReplicatedCompositesEXT |= f->shaderReplicatedComposites;
    caps.TensorAddressingNV |= f->cooperativeMatrixTensorAddressing;
    caps.CooperativeMatrixReductionsNV |= f->cooperativeMatrixReductions;
    caps.CooperativeMatrixConversionsNV |= f->cooperativeMatrixConversions;
    caps.CooperativeMatrixPerElementOperationsNV |= f->cooperativeMatrixPerElementOperations;
    caps.CooperativeMatrixTensorAddressingNV |= f->cooperativeMatrixTensorAddressing;
    caps.CooperativeMatrixBlockLoadsNV |= f->cooperativeMatrixBlockLoads;

   return caps;
}
