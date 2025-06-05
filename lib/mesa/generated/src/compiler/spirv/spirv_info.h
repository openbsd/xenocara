/* DO NOT EDIT - This file is generated automatically by spirv_info_c.py script */

/*
 * Copyright (C) 2017 Intel Corporation
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
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#ifndef _SPIRV_INFO_H_
#define _SPIRV_INFO_H_

#include <stdbool.h>

#include "compiler/spirv/spirv.h"

struct spirv_capabilities {
   bool Matrix;
   bool Shader;
   bool Geometry;
   bool Tessellation;
   bool Addresses;
   bool Linkage;
   bool Kernel;
   bool Vector16;
   bool Float16Buffer;
   bool Float16;
   bool Float64;
   bool Int64;
   bool Int64Atomics;
   bool ImageBasic;
   bool ImageReadWrite;
   bool ImageMipmap;
   bool Pipes;
   bool Groups;
   bool DeviceEnqueue;
   bool LiteralSampler;
   bool AtomicStorage;
   bool Int16;
   bool TessellationPointSize;
   bool GeometryPointSize;
   bool ImageGatherExtended;
   bool StorageImageMultisample;
   bool UniformBufferArrayDynamicIndexing;
   bool SampledImageArrayDynamicIndexing;
   bool StorageBufferArrayDynamicIndexing;
   bool StorageImageArrayDynamicIndexing;
   bool ClipDistance;
   bool CullDistance;
   bool ImageCubeArray;
   bool SampleRateShading;
   bool ImageRect;
   bool SampledRect;
   bool GenericPointer;
   bool Int8;
   bool InputAttachment;
   bool SparseResidency;
   bool MinLod;
   bool Sampled1D;
   bool Image1D;
   bool SampledCubeArray;
   bool SampledBuffer;
   bool ImageBuffer;
   bool ImageMSArray;
   bool StorageImageExtendedFormats;
   bool ImageQuery;
   bool DerivativeControl;
   bool InterpolationFunction;
   bool TransformFeedback;
   bool GeometryStreams;
   bool StorageImageReadWithoutFormat;
   bool StorageImageWriteWithoutFormat;
   bool MultiViewport;
   bool SubgroupDispatch;
   bool NamedBarrier;
   bool PipeStorage;
   bool GroupNonUniform;
   bool GroupNonUniformVote;
   bool GroupNonUniformArithmetic;
   bool GroupNonUniformBallot;
   bool GroupNonUniformShuffle;
   bool GroupNonUniformShuffleRelative;
   bool GroupNonUniformClustered;
   bool GroupNonUniformQuad;
   bool ShaderLayer;
   bool ShaderViewportIndex;
   bool UniformDecoration;
   bool CoreBuiltinsARM;
   bool TileImageColorReadAccessEXT;
   bool TileImageDepthReadAccessEXT;
   bool TileImageStencilReadAccessEXT;
   bool CooperativeMatrixLayoutsARM;
   bool FragmentShadingRateKHR;
   bool SubgroupBallotKHR;
   bool DrawParameters;
   bool WorkgroupMemoryExplicitLayoutKHR;
   bool WorkgroupMemoryExplicitLayout8BitAccessKHR;
   bool WorkgroupMemoryExplicitLayout16BitAccessKHR;
   bool SubgroupVoteKHR;
   union {
      bool StorageBuffer16BitAccess;
      bool StorageUniformBufferBlock16;
   };
   union {
      bool UniformAndStorageBuffer16BitAccess;
      bool StorageUniform16;
   };
   bool StoragePushConstant16;
   bool StorageInputOutput16;
   bool DeviceGroup;
   bool MultiView;
   bool VariablePointersStorageBuffer;
   bool VariablePointers;
   bool AtomicStorageOps;
   bool SampleMaskPostDepthCoverage;
   bool StorageBuffer8BitAccess;
   bool UniformAndStorageBuffer8BitAccess;
   bool StoragePushConstant8;
   bool DenormPreserve;
   bool DenormFlushToZero;
   bool SignedZeroInfNanPreserve;
   bool RoundingModeRTE;
   bool RoundingModeRTZ;
   bool RayQueryProvisionalKHR;
   bool RayQueryKHR;
   bool UntypedPointersKHR;
   bool RayTraversalPrimitiveCullingKHR;
   bool RayTracingKHR;
   bool TextureSampleWeightedQCOM;
   bool TextureBoxFilterQCOM;
   bool TextureBlockMatchQCOM;
   bool TextureBlockMatch2QCOM;
   bool Float16ImageAMD;
   bool ImageGatherBiasLodAMD;
   bool FragmentMaskAMD;
   bool StencilExportEXT;
   bool ImageReadWriteLodAMD;
   bool Int64ImageEXT;
   bool ShaderClockKHR;
   bool ShaderEnqueueAMDX;
   bool QuadControlKHR;
   bool SampleMaskOverrideCoverageNV;
   bool GeometryShaderPassthroughNV;
   union {
      bool ShaderViewportIndexLayerEXT;
      bool ShaderViewportIndexLayerNV;
   };
   bool ShaderViewportMaskNV;
   bool ShaderStereoViewNV;
   bool PerViewAttributesNV;
   bool FragmentFullyCoveredEXT;
   bool MeshShadingNV;
   bool ImageFootprintNV;
   bool MeshShadingEXT;
   union {
      bool FragmentBarycentricKHR;
      bool FragmentBarycentricNV;
   };
   union {
      bool ComputeDerivativeGroupQuadsKHR;
      bool ComputeDerivativeGroupQuadsNV;
   };
   union {
      bool FragmentDensityEXT;
      bool ShadingRateNV;
   };
   bool GroupNonUniformPartitionedNV;
   union {
      bool ShaderNonUniform;
      bool ShaderNonUniformEXT;
   };
   union {
      bool RuntimeDescriptorArray;
      bool RuntimeDescriptorArrayEXT;
   };
   union {
      bool InputAttachmentArrayDynamicIndexing;
      bool InputAttachmentArrayDynamicIndexingEXT;
   };
   union {
      bool UniformTexelBufferArrayDynamicIndexing;
      bool UniformTexelBufferArrayDynamicIndexingEXT;
   };
   union {
      bool StorageTexelBufferArrayDynamicIndexing;
      bool StorageTexelBufferArrayDynamicIndexingEXT;
   };
   union {
      bool UniformBufferArrayNonUniformIndexing;
      bool UniformBufferArrayNonUniformIndexingEXT;
   };
   union {
      bool SampledImageArrayNonUniformIndexing;
      bool SampledImageArrayNonUniformIndexingEXT;
   };
   union {
      bool StorageBufferArrayNonUniformIndexing;
      bool StorageBufferArrayNonUniformIndexingEXT;
   };
   union {
      bool StorageImageArrayNonUniformIndexing;
      bool StorageImageArrayNonUniformIndexingEXT;
   };
   union {
      bool InputAttachmentArrayNonUniformIndexing;
      bool InputAttachmentArrayNonUniformIndexingEXT;
   };
   union {
      bool UniformTexelBufferArrayNonUniformIndexing;
      bool UniformTexelBufferArrayNonUniformIndexingEXT;
   };
   union {
      bool StorageTexelBufferArrayNonUniformIndexing;
      bool StorageTexelBufferArrayNonUniformIndexingEXT;
   };
   bool RayTracingPositionFetchKHR;
   bool RayTracingNV;
   bool RayTracingMotionBlurNV;
   union {
      bool VulkanMemoryModel;
      bool VulkanMemoryModelKHR;
   };
   union {
      bool VulkanMemoryModelDeviceScope;
      bool VulkanMemoryModelDeviceScopeKHR;
   };
   union {
      bool PhysicalStorageBufferAddresses;
      bool PhysicalStorageBufferAddressesEXT;
   };
   union {
      bool ComputeDerivativeGroupLinearKHR;
      bool ComputeDerivativeGroupLinearNV;
   };
   bool RayTracingProvisionalKHR;
   bool CooperativeMatrixNV;
   bool FragmentShaderSampleInterlockEXT;
   bool FragmentShaderShadingRateInterlockEXT;
   bool ShaderSMBuiltinsNV;
   bool FragmentShaderPixelInterlockEXT;
   union {
      bool DemoteToHelperInvocation;
      bool DemoteToHelperInvocationEXT;
   };
   bool DisplacementMicromapNV;
   bool RayTracingOpacityMicromapEXT;
   bool ShaderInvocationReorderNV;
   bool BindlessTextureNV;
   bool RayQueryPositionFetchKHR;
   bool AtomicFloat16VectorNV;
   bool RayTracingDisplacementMicromapNV;
   bool RawAccessChainsNV;
   bool CooperativeMatrixReductionsNV;
   bool CooperativeMatrixConversionsNV;
   bool CooperativeMatrixPerElementOperationsNV;
   bool CooperativeMatrixTensorAddressingNV;
   bool CooperativeMatrixBlockLoadsNV;
   bool TensorAddressingNV;
   bool SubgroupShuffleINTEL;
   bool SubgroupBufferBlockIOINTEL;
   bool SubgroupImageBlockIOINTEL;
   bool SubgroupImageMediaBlockIOINTEL;
   bool RoundToInfinityINTEL;
   bool FloatingPointModeINTEL;
   bool IntegerFunctions2INTEL;
   bool FunctionPointersINTEL;
   bool IndirectReferencesINTEL;
   bool AsmINTEL;
   bool AtomicFloat32MinMaxEXT;
   bool AtomicFloat64MinMaxEXT;
   bool AtomicFloat16MinMaxEXT;
   bool VectorComputeINTEL;
   bool VectorAnyINTEL;
   bool ExpectAssumeKHR;
   bool SubgroupAvcMotionEstimationINTEL;
   bool SubgroupAvcMotionEstimationIntraINTEL;
   bool SubgroupAvcMotionEstimationChromaINTEL;
   bool VariableLengthArrayINTEL;
   bool FunctionFloatControlINTEL;
   bool FPGAMemoryAttributesINTEL;
   bool FPFastMathModeINTEL;
   bool ArbitraryPrecisionIntegersINTEL;
   bool ArbitraryPrecisionFloatingPointINTEL;
   bool UnstructuredLoopControlsINTEL;
   bool FPGALoopControlsINTEL;
   bool KernelAttributesINTEL;
   bool FPGAKernelAttributesINTEL;
   bool FPGAMemoryAccessesINTEL;
   bool FPGAClusterAttributesINTEL;
   bool LoopFuseINTEL;
   bool FPGADSPControlINTEL;
   bool MemoryAccessAliasingINTEL;
   bool FPGAInvocationPipeliningAttributesINTEL;
   bool FPGABufferLocationINTEL;
   bool ArbitraryPrecisionFixedPointINTEL;
   bool USMStorageClassesINTEL;
   bool RuntimeAlignedAttributeINTEL;
   bool IOPipesINTEL;
   bool BlockingPipesINTEL;
   bool FPGARegINTEL;
   union {
      bool DotProductInputAll;
      bool DotProductInputAllKHR;
   };
   union {
      bool DotProductInput4x8Bit;
      bool DotProductInput4x8BitKHR;
   };
   union {
      bool DotProductInput4x8BitPacked;
      bool DotProductInput4x8BitPackedKHR;
   };
   union {
      bool DotProduct;
      bool DotProductKHR;
   };
   bool RayCullMaskKHR;
   bool CooperativeMatrixKHR;
   bool ReplicatedCompositesEXT;
   bool BitInstructions;
   bool GroupNonUniformRotateKHR;
   bool FloatControls2;
   bool AtomicFloat32AddEXT;
   bool AtomicFloat64AddEXT;
   bool LongCompositesINTEL;
   union {
      bool OptNoneEXT;
      bool OptNoneINTEL;
   };
   bool AtomicFloat16AddEXT;
   bool DebugInfoModuleINTEL;
   bool BFloat16ConversionINTEL;
   bool SplitBarrierINTEL;
   bool ArithmeticFenceEXT;
   bool FPGAClusterAttributesV2INTEL;
   bool FPGAKernelAttributesv2INTEL;
   bool FPMaxErrorINTEL;
   bool FPGALatencyControlINTEL;
   bool FPGAArgumentInterfacesINTEL;
   bool GlobalVariableHostAccessINTEL;
   bool GlobalVariableFPGADecorationsINTEL;
   bool SubgroupBufferPrefetchINTEL;
   bool GroupUniformArithmeticKHR;
   bool MaskedGatherScatterINTEL;
   bool CacheControlsINTEL;
   bool RegisterLimitsINTEL;
};

bool spirv_capabilities_get(const struct spirv_capabilities *caps,
                            SpvCapability cap);
void spirv_capabilities_set(struct spirv_capabilities *caps,
                            SpvCapability cap, bool enabled);

const char *spirv_addressingmodel_to_string(SpvAddressingModel v);
const char *spirv_builtin_to_string(SpvBuiltIn v);
const char *spirv_capability_to_string(SpvCapability v);
const char *spirv_decoration_to_string(SpvDecoration v);
const char *spirv_dim_to_string(SpvDim v);
const char *spirv_executionmode_to_string(SpvExecutionMode v);
const char *spirv_executionmodel_to_string(SpvExecutionModel v);
const char *spirv_fproundingmode_to_string(SpvFPRoundingMode v);
const char *spirv_functionparameterattribute_to_string(SpvFunctionParameterAttribute v);
const char *spirv_imageformat_to_string(SpvImageFormat v);
const char *spirv_imageoperands_to_string(SpvImageOperandsMask v);
const char *spirv_memorymodel_to_string(SpvMemoryModel v);
const char *spirv_storageclass_to_string(SpvStorageClass v);
const char *spirv_op_to_string(SpvOp v);

#endif /* SPIRV_INFO_H */
